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
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "drvSYS.h"

// Internal Definition
#include "asmCPU.h"
#include "osalTSP.h"
#include "halTSP.h"
#include "drvTSP.h"
#include "ULog.h"

#if DMX_DEBUG
#include "../../api/dmx/Internal_DMX_debug.h"
#endif

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
    #include "mdrv_firmware_io.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define _TSP_SetEvent(flag)                                 OSAL_TSP_EventSet(_s32TspEventId, (flag))
#define _TSP_GetEvent(events, flag, opt, timeout)           OSAL_TSP_EventGet(_s32TspEventId, (flag), &(events), (opt), (timeout))

#define _TSP_SetSecRdyId(fltid)                             OSAL_TSP_EventSet(_s32TspSecRdyId[fltid>>5UL], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetSecRdyId(fltid, events, opt)                OSAL_TSP_EventGet(_s32TspSecRdyId[fltid>>5UL], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeSecRdy(idx, mask,events, opt)           OSAL_TSP_EventGet(_s32TspSecRdyId[idx], mask, (events), (opt), 0)

#define _TSP_SetSecOvfId(fltid)                             OSAL_TSP_EventSet(_s32TspSecOvfId[fltid>>5UL], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetSecOvfId(fltid, events, opt)                OSAL_TSP_EventGet(_s32TspSecOvfId[fltid>>5UL], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeSecOvf(idx, mask,events, opt)           OSAL_TSP_EventGet(_s32TspSecOvfId[idx], mask, (events), (opt), 0)

#define _TSP_SetSecCrcId(fltid)                             OSAL_TSP_EventSet(_s32TspSecCrcId[fltid>>5UL], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetSecCrcId(fltid, events, opt)                OSAL_TSP_EventGet(_s32TspSecCrcId[fltid>>5UL], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeSecCrc(idx, mask,events, opt)           OSAL_TSP_EventGet(_s32TspSecCrcId[idx], mask, (events), (opt), 0)

#define _TSP_SetFltDisableId(fltid)                         OSAL_TSP_EventSet(_s32TspFltDisableId[fltid>>5UL], (1UL<<(fltid&0x1FUL)))
#define _TSP_GetFltDisableId(fltid, events, opt)            OSAL_TSP_EventGet(_s32TspFltDisableId[fltid>>5UL], (1UL<<(fltid&0x1FUL)), (events), (opt), 0)
#define _TSP_GetModeFltDisable(idx, mask,events, opt)       OSAL_TSP_EventGet(_s32TspFltDisableId[idx], mask, (events), (opt), 0)

#if 0
#define _TSP_FLAG_FREE_TIMEOUT                              4
#define _TSP_SetEventFree0Id(fltid)                         OSAL_TSP_EventSet(_s32EventFree0[fltid>>5], (1<<(fltid&0x1F)))
#define _TSP_GetEventFree0Id(fltid, events, opt)            OSAL_TSP_EventGet(_s32EventFree0[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define _TSP_GetModeEventFree0(idx, mask,events, opt)       OSAL_TSP_EventGet(_s32EventFree0[idx], mask, (events), (opt), 0)
#define _TSP_FLAG_SetDone(fltid)                            OSAL_TSP_EventSet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)))
#define _TSP_FLAG_GetDone(fltid, events, opt)               OSAL_TSP_EventGet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), _TSP_FLAG_FREE_TIMEOUT)

#define _TSP_SetEventFreeDefer0Id(fltid)                    OSAL_TSP_EventSet(_s32EventFreeDefer0[fltid>>5], (1<<(fltid&0x1F)))
#define _TSP_GetEventFreeDefer0Id(fltid, events, opt)       OSAL_TSP_EventGet(_s32EventFreeDefer0[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0)
#define _TSP_GetEventFreeDefer0(idx, mask,events, opt)      OSAL_TSP_EventGet(_s32EventFreeDefer0[idx], mask, (events), (opt), 0)
#endif

// for mutiple process using, record which filter is enabled by this process
#define _TSP_SetSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5UL)] |= (1 << (fltid&0x1FUL)))
#define _TSP_RemoveSecNotifyId(fltid)                       (_u32SecNotifyId[(fltid>>5UL)] &= ~(1 << (fltid&0x1FUL)))
#define _TSP_ChkSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5UL)] & (1 << (fltid&0x1FUL)))

// for mutiple process using, record which pvr engine is enabled by this process
#ifndef NO_PVR
#define _TSP_SetPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5UL)] |= (1 << (fltid&0x1FUL)))
#define _TSP_RemovePvrNotifyId(fltid)                       (_u32PvrNotifyId[(fltid>>5UL)] &= ~(1 << (fltid&0x1FUL)))
#define _TSP_ChkPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5UL)] & (1 << (fltid&0x1FUL)))
#endif

// for mutiple process using, record which hw pcr is enabled by this process
#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
#define _TSP_SetHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5UL)] |= (1 << (fltid&0x1FUL)))
#define _TSP_RemoveHwPcrNotifyId(fltid)                     (_u32HwPcrNotifyId[(fltid>>5UL)] &= ~(1 << (fltid&0x1FUL)))
#define _TSP_ChkHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5UL)] & (1 << (fltid&0x1FUL)))
#endif

#ifdef TS2_LIVE_SUPPORT
#define _TSP_GET_SECFLT_REG(fltid)                          ((fltid<TSP_SECFLT_NUM)? (&(_TspSec[0].Flt[fltid])):(&(_TspSec2[0].Flt[fltid-TSP_SECFLT_NUM])))
#else
#define _TSP_GET_SECFLT_REG(fltid)                          (&(_TspSec[0].Flt[fltid]))
#endif

#define _TSP_MUTEX_TIMEOUT                                  10000UL //OSAL_TSP_MUTEX_TIMEOUT

#define _TSP_ENTRY()                                    do { \
                                                            while (!OSAL_TSP_MutexObtain(_ptsp_res->_s32TspMutexId , _TSP_MUTEX_TIMEOUT))      \
                                                            {                                                                       \
                                                                _ptsp_res->_u32LastErr = DRVTSP_FUNC_ERROR;                                    \
                                                                ULOGE("TSP", "[%s][%d] TSP mutex get error\n", __FUNCTION__, __LINE__);   \
                                                                MsOS_DelayTask(1); \
                                                            }                                                                       \
                                                            while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IndMutexId, _TSP_MUTEX_TIMEOUT))   \
                                                            {   \
                                                                _ptsp_res->_u32LastErr = DRVTSP_FUNC_ERROR;        \
                                                                ULOGE("TSP", "[%s][%d] IND mutex get error\n", __FUNCTION__, __LINE__);   \
                                                                MsOS_DelayTask(1);  \
                                                            }  \
                                                        }while(0);

#define _TSP_RETURN(_ret)                               do { \
                                                            if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IndMutexId))  \
                                                            {  \
                                                                if (_u32TSPDGBLevel >= E_DRVTSP_DBG_L1) { ULOGE("TSP", "[%s][%d] IND mutex free error\n", __FUNCTION__, __LINE__);}   \
                                                            }  \
                                                            if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32TspMutexId))   \
                                                            {  \
                                                               if (_u32TSPDGBLevel >= E_DRVTSP_DBG_L1) {ULOGE("TSP", "[%s][%d] TSP mutex free error\n", __FUNCTION__, __LINE__); }  \
                                                            }  \
                                                            if (_ret != DRVTSP_OK) { _ptsp_res->_u32LastErr = _ret; }                          \
                                                            return _ret;                                                         \
                                                        }while(0);   // Should be optimized by compiler

#define _TSP_IND_LOCK()                                 while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IndMutexId, _TSP_MUTEX_TIMEOUT))  \
                                                        {   \
                                                            ULOGE("TSP", "[%s][%d] IND mutex get error\n", __FUNCTION__, __LINE__);   \
                                                            MsOS_DelayTask(1);  \
                                                        }

#define _TSP_IND_UNLOCK()                               if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IndMutexId))  \
                                                        {   \
                                                            ULOGE("TSP", "[%s][%d] IND mutex free error\n", __FUNCTION__, __LINE__);   \
                                                        }

#define _TSP_INT_LOCK()                                 while(!OSAL_TSP_MutexObtain(_ptsp_res->_s32IntRegMutexId, _TSP_MUTEX_TIMEOUT)) \
                                                        {   \
                                                            ULOGE("TSP", "[%s][%d] INT mutex get error\n", __FUNCTION__, __LINE__);   \
                                                            MsOS_DelayTask(1);  \
                                                        }

#define _TSP_INT_UNLOCK()                               if(!OSAL_TSP_MutexRelease(_ptsp_res->_s32IntRegMutexId))   \
                                                        {   \
                                                            ULOGE("TSP", "[%s][%d] INT mutex free error\n", __FUNCTION__, __LINE__);   \
                                                        }

#define _TSP_EXT_LOCK()                                 while(!OSAL_TSP_MutexObtain(_s32ExtMutexId, _TSP_MUTEX_TIMEOUT))  \
                                                        {   \
                                                            ULOGE("TSP", "[%s][%d] EXT mutex get error\n", __FUNCTION__, __LINE__);   \
                                                            MsOS_DelayTask(1);  \
                                                        }

#define _TSP_EXT_UNLOCK()                               if(!OSAL_TSP_MutexRelease(_s32ExtMutexId)){  \
                                                            ULOGE("TSP", "[%s][%d] EXT mutex free error\n", __FUNCTION__, __LINE__);    \
                                                        }

#define TSP_ASSERT(_bool, _f)                           if(!(_bool)) {                      \
                                                            if (_u32TSPDGBLevel == E_DRVTSP_DBG_L1) { (_f); return DRVTSP_FAIL;} \
                                                            else if (_u32TSPDGBLevel == E_DRVTSP_DBG_L2) { (_f); ULOGE("TSP", "[%s][%d] ASSERT %d\n", __FUNCTION__, __LINE__,(unsigned int)MsOS_GetSystemTime()); MsOS_DelayTask(MSOS_WAIT_FOREVER); return DRVTSP_FAIL;} \
                                                            else { return DRVTSP_FAIL; }             \
                                                        }while(0);

#define TSP_ASSERT2(_bool, _f)                          if(!(_bool)) { \
                                                            if (_u32TSPDGBLevel == E_DRVTSP_DBG_L1) { (_f); _TSP_RETURN(DRVTSP_FAIL);} \
                                                            else if (_u32TSPDGBLevel == E_DRVTSP_DBG_L2) { (_f); ULOGE("TSP", "[%s][%d] ASSERT2 %d\n", __FUNCTION__, __LINE__,(unsigned int)MsOS_GetSystemTime()); MsOS_DelayTask(MSOS_WAIT_FOREVER); _TSP_RETURN(DRVTSP_FAIL);} \
                                                            else { _TSP_RETURN(DRVTSP_FAIL);}                                                                                                                  \
                                                        }while(0);
#define TSP_DBG(_f, _a...)                              { if ((_u32TSPDGBLevel == E_DRVTSP_DBG_L1)||(_u32TSPDGBLevel == E_DRVTSP_DBG_L2)) { OSAL_TSP_Print("[TSP_DBG]"); OSAL_TSP_Print(_f, ##_a); } }


#define DRVTSP_MAP_NULL                                 0xFFFFFFFFUL

#define PVRCA_DBG(x)                                    //x

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  HW architecture 1-1: 0~31: pid filters.   32~47: PVR filters.
//  HW architecture 1-2: For Extend filter support, 0~63: pid filters, 64~83: PVR pid filter
//  HW architecture 3    : General pid filters 0~127, only 0~63 for section filter
//-----------------------------------------------------------------------------
typedef struct _DrvTSP_State
{
    DrvTSP_SW_Status                TSPStatus;
    DrvTSP_FltState                 FltState[TSP_PIDFLT_NUM_ALL];
    DrvTSP_FltType                  FltType[TSP_PIDFLT_NUM_ALL];
    MS_U32                          FltSource[TSP_PIDFLT_NUM_ALL];
    MS_U32                          FltMap[TSP_PIDFLT_NUM_ALL];
    DrvTSP_FltState                 SecFltState[TSP_SECFLT_NUM_All];
    DrvTSP_Event                    SecFltEvtNotify[TSP_SECFLT_NUM_All];
    P_DrvTSP_EvtCallback            SecFltCallback[TSP_SECFLT_NUM_All];
    DrvTSP_FltState                 SecBufState[TSP_SECFLT_NUM_All];
    DrvTSP_FltMode                  SecFltMode[TSP_SECFLT_NUM_All];
} DrvTSP_State;

typedef struct _DrvTSP_FW_DBG_INFO
{
    MS_PHY                      phyBufAddr;
    MS_U32                          u32Size;
    MS_U32                          u32ReadPos;
    MS_U32                          u32WritePos;
    MS_BOOL                         bEnable;
} DrvTSP_FW_DBG_INFO;

#ifndef NO_PVR
typedef struct _DrvTSP_PVRCAParams
{
    #ifndef NO_CA
    MS_U16                  _u16Flt1PidList[TSP_PIDFLT1_NUM];   //PIDs of PVR1 group filters
    MS_U16                  _u16Flt0DscmbPidList[DSCMB_FLT_NUM_ALL]; //PIDs of TS0 DSCMB group filters
    MS_U32                  _u32Flt1Flag_DscmbPVR;  //bit flag of filter that are recording in PVR1 group filters
    MS_U32                  _u32Flt1Flag_DscmbRef;  //bit flag of filter that are live descrambling PVR1 group filters
    MS_BOOL                 _bFLT1Dup_Enable;
    MS_BOOL                 _bCIPHSSMode;
    MS_U16                  _u16Flt2PidList[DSCMB_FLT_NUM_ALL];
    #endif //end of NO_CA

    MS_BOOL                 _bPVRCA_Start;

}DrvTSP_PVRCAParams;
#endif //end of NO_PVR

typedef struct _TSP_RESOURCE_PRIVATE
{
    MS_S32                  _s32TspMutexId;
    MS_S32                  _s32IndMutexId;
    MS_S32                  _s32IntRegMutexId;

    MS_U32                  _u32LastErr;
    MS_BOOL                 _bSuspend;
    DrvTSP_State            _TspState[TSP_ENGINE_NUM];

    MS_U32                  _u32ProcessNum;

    MS_BOOL                 _bSWInitReady;
    MS_PHY                  _phyFwAddr;
    MS_U32                  _u32FwSize;
    MS_U32                  _u32DbgIntrCount;

    DrvTSP_If_Set           _ifset_save;

    MS_BOOL                 _bLiveSING_FilePP; //live in tto single and file in to ping-pong

    #ifdef VQ_ENABLE
    MS_U32                  _u32NULLFileFilterID; //for switch live & file path
    #endif //end of VQ_ENABLE

    #ifndef NO_PVR
    DrvTSP_PVRCAParams      TSPPvrcaParams;
    DrvTSP_Event            _PvrEvtNotify[TSP_PVR_IF_NUM];
    P_DrvTSP_EvtCallback    _PvrCallback[TSP_PVR_IF_NUM];
    #endif //end of NO_PVR

    MS_PHY                  _phyTspFWDramBuf;
    MS_U32                  _u32TspFwDramBufSize;

    MS_U32                  _u32OrgCAMode;

}TSP_RESOURCE_PRIVATE;


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _drv_tsp_version = {
    .DDI = { TSP_DRV_VERSION, },
};

#ifndef TSP_UTOPIA_20
static TSP_RESOURCE_PRIVATE     _tsp_res = {
    ._s32TspMutexId                     = -1,
    ._s32IndMutexId                     = -1,
    ._s32IntRegMutexId                  = -1,

    ._u32LastErr                        = DRVTSP_OK,
    ._bSuspend                          = FALSE,
    ._u32ProcessNum                     = 0UL,

    ._bSWInitReady                      = FALSE,
    ._u32FwAddr                         = 0UL,
    ._u32FwSize                         = 0UL,
    ._u32DbgIntrCount                   = 0UL,
    ._ifset_save                        = {E_DRVTSP_PAD_EXT_INPUT0, FALSE, TRUE, TRUE},
    ._bLiveSING_FilePP                  = FALSE,

    #ifdef VQ_ENABLE
    ._u32NULLFileFilterID               = 0xFFFFFFFFUL,
    #endif

    #ifndef NO_PVR
    .TSPPvrcaParams                     = {
        #ifndef NO_CA
        ._u16Flt1PidList                = {[0 ... (TSP_PIDFLT1_NUM-1)] = DRVTSP_PID_NULL},
        ._u16Flt0DscmbPidList           = {[0 ... (DSCMB_FLT_NUM_ALL-1)] = DRVTSP_PID_NULL},
        ._u32Flt1Flag_DscmbPVR          = 0UL,
        ._u32Flt1Flag_DscmbRef          = 0UL,
        ._bFLT1Dup_Enable               = FALSE,
        ._bCIPHSSMode                   = FALSE,
        ._u16Flt2PidList                = {[0 ... (DSCMB_FLT_NUM_ALL-1)] = DRVTSP_PID_NULL},
        #endif

        ._bPVRCA_Start                  = FALSE,

    },
    #endif

    #ifndef NO_PVR
    ._PvrEvtNotify                      = {[0 ... (TSP_PVR_IF_NUM-1)] = E_DRVTSP_EVENT_DATA_INIT},
    ._PvrCallback                       = {[0 ... (TSP_PVR_IF_NUM-1)] = 0},
    #endif

    ._phyTspFWDramBuf                   = 0UL,
    ._u32TspFwDramBufSize               = 0UL,

    ._u32OrgCAMode                      = 0UL,

};

static TSP_RESOURCE_PRIVATE*    _ptsp_res = &_tsp_res;
#else
static TSP_RESOURCE_PRIVATE*    _ptsp_res = NULL;
#endif

static MS_U8                    _db_flag = 0;
static MS_S32                   _s32ExtMutexId = -1; //For exiting ISR process task

static MS_U32                   _u32SecNotifyId[(TSP_SECFLT_NUM_All+31UL)>>5UL] = {0UL}; //for ISR processing

#ifndef NO_PVR
static MS_U32                   _u32PvrNotifyId[(TSP_PVR_IF_NUM+31UL)>>5UL] = {0UL};
#endif

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
static MS_U32                   _u32HwPcrNotifyId[(TSP_PCRFLT_NUM+31UL)>>5UL] = {0UL};
static MS_U8                    _u8ResetPcr[TSP_PCRFLT_NUM] = {[0 ... (TSP_PCRFLT_NUM-1)] = 3UL};
static MS_U32                   _u32PrevStcBase[TSP_PCRFLT_NUM] = {[0 ... (TSP_PCRFLT_NUM-1)] = 0UL};
static MS_U32                   _u32PrevPllControl[TSP_PCRFLT_NUM] = {[0 ... (TSP_PCRFLT_NUM-1)] = 0UL};
#endif

static MS_S32                   _s32TspEventId = -1;
static MS_S32                   _s32TspSecRdyId[(TSP_SECFLT_NUM_All+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM_All+31UL)>>5UL)-1UL)] = -1};
static MS_S32                   _s32TspSecOvfId[(TSP_SECFLT_NUM_All+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM_All+31UL)>>5UL)-1UL)] = -1};
static MS_S32                   _s32TspSecCrcId[(TSP_SECFLT_NUM_All+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM_All+31UL)>>5UL)-1UL)] = -1};
static MS_S32                   _s32TspFltDisableId[(TSP_SECFLT_NUM_All+31UL)>>5UL] = {[0 ... (((TSP_SECFLT_NUM_All+31UL)>>5UL)-1UL)] = -1};

#if 0
static MS_S32                   _s32EventFree0[(TSP_PIDFLT_NUM_ALL+31)>>5] = {[0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1};
static MS_S32                   _s32EventFreeDefer0[(TSP_PIDFLT_NUM_ALL+31)>>5] = {[0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1};
static MS_S32                   _s32DoneFlags[(TSP_PIDFLT_NUM_ALL+31)>>5] = { [0 ... (((TSP_PIDFLT_NUM_ALL+31)>>5)-1)] = -1};
#endif

static MS_S32                   _s32TspTaskId = -1;
static DrvTSP_DbgLevel          _u32TSPDGBLevel = E_DRVTSP_DBG_L1;

static DrvTSP_FW_DBG_INFO       _FwDbgInfo;

static MS_BOOL                  _bBankInit = FALSE;

MS_U32                          _u32IntNum = E_INT_IRQ_TSP2HK; //for hal extern using
MS_U32                          _u32TaskLine = 0UL;

#ifdef MSOS_TYPE_LINUX_KERNEL
wait_queue_head_t               tsp_wq_head[TSP_SECFLT_NUM_All];
#endif // MSOS_TYPE_LINUX_KERNEL

//-------------------------------------------------------------------------------------------------
//  Local Functions prototype
//-------------------------------------------------------------------------------------------------

static void _DumpFwMsg(void);
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if 0
static MS_U32 _TSP_GetTrailOne64(MS_U64 u64Flags)
{
    MS_U32 u32Temp = (MS_U32)(u64Flags & 0xFFFFFFFF);

    if (u32Temp)
        return MAsm_CPU_GetTrailOne(u32Temp);
    return (32 + MAsm_CPU_GetTrailOne((MS_U32)(u64Flags>> 32)));
}
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
extern int MDrv_FW_TSP_Addr(void);
extern int MDrv_FW_TSP_Size(void);
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

#ifndef NO_FILEIN
static void _TSP_CmdQ_Init(void)
{
    while (HAL_TSP_CmdQ_CmdCount()); // wait command finish

}
#endif //udef NO_FILEIN

static MS_BOOL _TSP_InitResource(TSP_RESOURCE_PRIVATE* presource)
{
    #ifndef NO_PVR
    MS_U32 u32ii = 0;
    #endif

    if(presource == NULL)
    {
        return FALSE;
    }

    presource->_s32TspMutexId = -1;
    presource->_s32IndMutexId = -1;
    presource->_s32IntRegMutexId = -1;

    presource->_u32LastErr = 1;
    presource->_bSuspend = FALSE;

    presource->_u32ProcessNum = 0UL;

    presource->_bSWInitReady = FALSE;
    presource->_phyFwAddr = 0UL;
    presource->_u32FwSize = 0UL;
    presource->_u32DbgIntrCount = 0UL;

    presource->_ifset_save.ePad = E_DRVTSP_PAD_EXT_INPUT0;
    presource->_ifset_save.bClkInv = FALSE;
    presource->_ifset_save.bExtSync = TRUE;
    presource->_ifset_save.bParallel = TRUE;

    presource->_bLiveSING_FilePP = FALSE; //live in single and file in ping-pong

    #ifdef VQ_ENABLE
    presource->_u32NULLFileFilterID = 0xFFFFFFFFUL;
    #endif

    #ifndef NO_PVR
    #ifndef NO_CA
    for(u32ii = 0; u32ii < TSP_PIDFLT1_NUM; u32ii++)
    {
        presource->TSPPvrcaParams._u16Flt1PidList[u32ii] = DRVTSP_PID_NULL;
        presource->TSPPvrcaParams._u16Flt2PidList[u32ii] = DRVTSP_PID_NULL;
    }
    for(u32ii = 0; u32ii < (DSCMB_FLT_NUM + DSCMB_FLT_SHAREKEY_NUM); u32ii++)
    {
        presource->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii] = DRVTSP_PID_NULL;
    }
    presource->TSPPvrcaParams._u32Flt1Flag_DscmbPVR = 0UL;
    presource->TSPPvrcaParams._u32Flt1Flag_DscmbRef = 0UL;
    presource->TSPPvrcaParams._bFLT1Dup_Enable = FALSE;
    presource->TSPPvrcaParams._bCIPHSSMode = FALSE;
    #endif
    presource->TSPPvrcaParams._bPVRCA_Start = FALSE;

    for(u32ii = 0; u32ii < TSP_PVR_IF_NUM; u32ii++)
    {
        presource->_PvrEvtNotify[u32ii] = E_DRVTSP_EVENT_DATA_INIT;
        presource->_PvrCallback[u32ii] = 0;
    }
    #endif

    presource->_phyTspFWDramBuf = 0UL;
    presource->_u32TspFwDramBufSize = 0UL;

    presource->_u32OrgCAMode = 0UL;

    return TRUE;

}

static void _TSP_SwStateReset(MS_BOOL bInited)
{
    MS_U32 u32EngId, u32FilterId;
    MS_U32 u32SecId = 0;

    if(bInited == TRUE)
        return;

    for (u32EngId = 0; u32EngId < TSP_ENGINE_NUM; u32EngId++)
    {
        //TS0 Pid filter
        for (u32FilterId = 0; u32FilterId < TSP_SECFLT_NUM; u32FilterId++)
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32FilterId]        = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]         = E_DRVTSP_FLT_TYPE_SECTION; // for section filter
            _ptsp_res->_TspState[u32EngId].SecFltState[u32SecId]        = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecBufState[u32SecId]        = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecId]         = E_DRVTSP_FLT_MODE_CONTI;
            _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecId]    = E_DRVTSP_EVENT_DATA_INIT;
            _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecId++]   = NULL;
        }

        //TS1 Pid filter
        for (; u32FilterId < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM); u32FilterId++)
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32FilterId] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]  = E_DRVTSP_FLT_TYPE_PVR;
        }

        //TS2 Pid filter
#ifdef TS2_IF_SUPPORT

        #ifndef TS2_LIVE_SUPPORT
        for (; u32FilterId < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM + TSP_PIDFLT2_NUM); u32FilterId++)
        #else
        for (; u32FilterId < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM + TSP_SECFLT2_NUM); u32FilterId++)
        #endif
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32FilterId]        = E_DRVTSP_FLT_STATE_FREE;

            #ifndef TS2_LIVE_SUPPORT
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]         = E_DRVTSP_FLT_TYPE_CB;
            #else
            _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]         = E_DRVTSP_FLT_TYPE_SECTION; // for section filter
            _ptsp_res->_TspState[u32EngId].SecFltState[u32SecId]        = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecBufState[u32SecId]        = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecId]         = E_DRVTSP_FLT_MODE_CONTI;
            _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecId]    = E_DRVTSP_EVENT_DATA_INIT;
            _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecId++]   = NULL;
            #endif
        }
#endif

        //HWPCR Pid filter
#ifdef HWPCR_ENABLE
        _ptsp_res->_TspState[u32EngId].FltType[u32FilterId]  = E_DRVTSP_FLT_TYPE_PCR;
#endif

    }

}

static void _TSP_SW_Exit(void)
{
    MS_U8 u8ii;

    #ifndef MSOS_TYPE_LINUX_KERNEL
    MS_U32 u32Timer, u32CurTime = 0;

    _TSP_EXT_LOCK();
    _db_flag = 1;
    _TSP_EXT_UNLOCK();

    _TSP_IND_UNLOCK();
    u32Timer = MsOS_GetSystemTime();
    // check _s32TspTaskId for NOS case
    while(_s32TspTaskId > 0)
    {
        _TSP_SetEvent(TSP_TASK_EVENT_FLT_FREE);
        _TSP_EXT_LOCK();
        if (_db_flag == 2)
        {
            _TSP_EXT_UNLOCK();
            break;
        }
        _TSP_EXT_UNLOCK();

        u32CurTime = MsOS_GetSystemTime();
        if(u32CurTime < u32Timer)
        {
            ULOGD("TSP", "[%s][%d] Systemtime Change %d \n", __FUNCTION__, __LINE__, (unsigned int)u32CurTime);
            u32Timer = u32CurTime;
        }
        if ((u32CurTime-u32Timer) > 1000)
        {
            ULOGE("TSP", "[%s][%d] TSP task exit timeout!!!!!!\n", __FUNCTION__, __LINE__);
            break;
        }

    }

    _TSP_IND_LOCK();
    /*if(_s32TspTaskId > 0)
    {
        MsOS_DeleteTask(_s32TspTaskId);
        _s32TspTaskId = -1;
    }*/
    #else
    //to free CB process task, so send one event
    _TSP_SetEvent(TSP_TASK_EVENT_GROUP_SELF);
    #endif

    for (u8ii = 0; u8ii < ((TSP_SECFLT_NUM_All+31)>>5); u8ii++)
    {
        if(_s32TspSecRdyId[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32TspSecRdyId[u8ii]);
            _s32TspSecRdyId[u8ii] = -1;
        }
        if(_s32TspSecOvfId[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32TspSecOvfId[u8ii]);
            _s32TspSecOvfId[u8ii] = -1;
        }
        if(_s32TspSecCrcId[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32TspSecCrcId[u8ii]);
            _s32TspSecCrcId[u8ii] = -1;
        }
        if(_s32TspFltDisableId[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32TspFltDisableId[u8ii]);
            _s32TspFltDisableId[u8ii] = -1;
        }
    }

    #if 0
    for (u8ii = 0; u8ii < ((TSP_PIDFLT_NUM_ALL+31)>>5); u8ii++)
    {
        if(_s32EventFree0[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32EventFree0[u8ii]);
            _s32EventFree0[u8ii] = -1;
        }
        if(_s32EventFreeDefer0[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32EventFreeDefer0[u8ii]);
            _s32EventFreeDefer0[u8ii] = -1;
        }
        if(_s32DoneFlags[u8ii] != -1)
        {
            OSAL_TSP_EventDelete(_s32DoneFlags[u8ii]);
            _s32DoneFlags[u8ii] = -1;
        }
    }
    #endif

    if (_s32ExtMutexId != -1)
    {
        _TSP_EXT_LOCK();
        _TSP_EXT_UNLOCK();
        MsOS_DeleteMutex(_s32ExtMutexId);
        _s32ExtMutexId = -1;
        _db_flag = 0;
    }

    if (_s32TspEventId != -1)
    {
        #ifdef DMX_UTOPIA_2K
        MsOS_DelayTask(3);
        #endif

        while(OSAL_TSP_EventDelete(_s32TspEventId) == FALSE)
        {
            MsOS_DelayTask(3);
            printf("Waiting for evet deleting... \n");
        }
        _s32TspEventId = -1;
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

    _ptsp_res->_TspState[0].TSPStatus       = E_DRVTSP_SW_EXIT;
    _ptsp_res->_bSWInitReady                = FALSE;

}


static void _TSP_Close(void)
{
    HAL_TSP_Int_Disable(TSP_HWINT_ALL);
#ifdef TS2_IF_SUPPORT
    HAL_TSP_Int2_Disable(TSP_HWINT2_ALL);
#endif
    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();
    HAL_TSP_WbDmaEnable(FALSE);
    HAL_TSP_SetCtrlMode(0, 0, TSP_IF_NUM);
    _TSP_SW_Exit();
    HAL_TSP_PowerCtrl(DISABLE);
    HAL_TSP_HW_Lock_Release();
}


static void _TSP_RegStateStore(void)
{
    HAL_TSP_Int_Disable(TSP_HWINT_ALL);
#ifdef TS2_IF_SUPPORT
    HAL_TSP_Int2_Disable(TSP_HWINT2_ALL);
#endif
    OSAL_TSP_IntDisable();
    OSAL_TSP_IntDetach();
    HAL_TSP_SaveFltState();
}


static void _TSP_RegStateRestore(void)
{
    HAL_TSP_RestoreFltState();
}

#ifndef NO_CA

#if 0
static MS_BOOL _TSP_DumpDscmbDupArray(void)
{
    MS_U32 u32ii = 0;

    ULOGD("TSP", "[%s][%d]\n", __FUNCTION__, __LINE__);

    for(u32ii = 0; u32ii < DSCMB_FLT_NUM_ALL; u32ii++)
    {
        ULOGD("TSP", "FLT1 %ld, %x \n", u32ii, _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii]);
        ULOGD("TSP", "FLT0 %ld, %x \n", u32ii, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii]);
        ULOGD("TSP", "FLT2 %ld, %x \n", u32ii, _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii]);
    }
    ULOGD("TSP", "_u32Flt1Flag_DscmbRef %lx , _u32Flt1Flag_DscmbPVR %lx\n",
        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR);

    return TRUE;
}
#endif

static MS_BOOL _TSP_SetNewDscmbPid2FLT1(MS_U32 u32EngId, MS_U32 u32Flt1Id, MS_U16 u32Pid)
{
    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32Flt1Id + TSP_SECFLT_NUM), u32Pid);
    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32Flt1Id] = u32Pid;
    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32Flt1Id);

    return TRUE;
}

static MS_BOOL _TSP_FreeDscmbPid_In_FLT1(MS_U32 u32EngId, MS_U32 u32Flt1Id)
{
    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32Flt1Id] = DRVTSP_PID_NULL;
    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32Flt1Id);
    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32Flt1Id + TSP_SECFLT_NUM), DRVTSP_PID_NULL);

    return TRUE;
}

static MS_BOOL _TSP_FreeDupDscmbPidFlt(MS_U32 u32EngId, MS_U32 u32FltGroupId, MS_U32 u32CaMode)
{
    MS_U32 u32ii = 0, u32jj = 0, u32kk = 0;

    #ifdef EXT_PVR1_FLT
    MS_U32  u32DscmbFltNum = DSCMB_FLT_NUM_ALL;
    #else
    MS_U32  u32DscmbFltNum = DSCMB_FLT_NUM;
    #endif

    //MS_BOOL bScmbLive0 = ((u32CaMode & TSP_CA_OUTPUT_PLAY_LIVE) ? TRUE: FALSE);

    #ifdef TS2_LIVE_SUPPORT
    MS_BOOL bScmbLive1 = (((u32CaMode & (TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2)) == TSP_CA_OUTPUT_PLAY_LIVE2)? TRUE: FALSE);
    MS_BOOL bScmbRc1 = ((u32CaMode &TSP_CA_OUTPUT_PLAY_LIVE2) ? TRUE: FALSE);
    #endif

    if(((u32CaMode & TSP_CA_INPUT_TSIF1) == 0) || ((u32CaMode & TSP_CA_OUTPUT_REC) != 0))
        return FALSE;

    //Free FLT0
    if((u32FltGroupId == 0) &&
        ((u32CaMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)))
    {
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] Free FLT0 list \n", __FUNCTION__, __LINE__));

        for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii] == DRVTSP_PID_NULL)
                continue;

            //Remove all duplicate filter in Group1
            for(u32jj = 0; u32jj < u32DscmbFltNum; u32jj++)
            {
                if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32jj])
                {
                    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32jj);
                    if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef|_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR) & (1 << u32jj)) == 0)
                    {
                        _TSP_FreeDscmbPid_In_FLT1(u32EngId, u32jj);
                        PVRCA_DBG(sd_print("Free Group0: FLT0 id %ld, FLT1 id %ld, PID 0x%x \n", u32ii, u32jj, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii]));
                    }
                }
            }

            for(u32kk = 0; u32kk < u32DscmbFltNum; u32kk++)
            {
                if(u32kk == u32ii)
                    continue;

                if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32kk] == _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii])
                {
                    PVRCA_DBG(ULOGD("TSP", "[%s]Free Group0: FLT0 id %ld, FLT1 id %ld, PID 0x%x \n", __FUNCTION__, u32kk, u32jj, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32kk]));
                    _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32kk] = DRVTSP_PID_NULL;
                }
            }

            PVRCA_DBG(ULOGD("TSP", "[%s]Free Group0: FLT0 id %ld, FLT1 id %ld, PID 0x%x \n", __FUNCTION__, u32ii, u32jj, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii]));
            _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii] = DRVTSP_PID_NULL;

        }

        return TRUE;
    }

    //Free FLT2
    #ifdef TS2_LIVE_SUPPORT
    if((u32FltGroupId == 2) &&
        ((u32CaMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2)))
    {
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] Free FLT2 list \n", __FUNCTION__, __LINE__));

        for(u32ii = 0; u32ii < DSCMB_FLT_NUM; u32ii++)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii] == DRVTSP_PID_NULL)
                continue;

            //Remove all duplicate filter in Group1
            for(u32jj = 0; u32jj < DSCMB_FLT_NUM; u32jj++)
            {
                if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32jj])
                {
                    if(bScmbLive1)
                    {
                        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32jj);
                    }
                    if(bScmbRc1)
                    {
                        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR &= ~(1 << u32jj);
                    }
                    if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef|_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR) & (1 << u32jj)) == 0)
                    {
                        _TSP_FreeDscmbPid_In_FLT1(u32EngId, u32jj);
                        PVRCA_DBG(sd_print("Free Group2: FLT2 id %ld, FLT1 id %ld, PID 0x%x \n", u32ii, u32jj, _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii]));
                    }
                }
            }

            //check if any other FLT2 filter using the same pid
            for(u32kk = 0; u32kk < DSCMB_FLT_NUM; u32kk++)
            {
                if(u32kk == u32ii)
                    continue;

                if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32kk] == _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii])
                {
                    PVRCA_DBG(ULOGD("TSP", "[%s]Free Group2: FLT2 id %ld, FLT1 id %ld, PID 0x%x \n", __FUNCTION__, u32kk, u32jj, _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32kk]));
                    _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32kk] = DRVTSP_PID_NULL;
                }
            }
            PVRCA_DBG(ULOGD("TSP", "[%s]Free Group2: FLT2 id %ld, FLT1 id %ld, PID 0x%x \n", __FUNCTION__, u32ii, u32jj, _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii]));
            _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii] = DRVTSP_PID_NULL;

        }

        return TRUE;
    }
    #endif

    return FALSE;
}

static MS_BOOL _TSP_DupDscmbPidFlt(MS_U32 u32EngId, MS_U32 OrgFltId, MS_U32 DupFltId)
{
    MS_U32 u32pid = DRVTSP_PID_NULL, u32ii = 0;
    MS_U32 u32OrgFltListIdx = 0;

    #ifdef EXT_PVR1_FLT
    MS_U32  u32DscmbFltNum = DSCMB_FLT_NUM_ALL;
    #else
    MS_U32  u32DscmbFltNum = DSCMB_FLT_NUM;
    #endif

    PVRCA_DBG(ULOGD("TSP", "[%s][%d] OrgFltId %ld, DupFltId %ld, FltType %lx\n",
        __FUNCTION__, __LINE__, OrgFltId, DupFltId, _ptsp_res->_TspState[u32EngId].FltType[OrgFltId]));

    if( (_ptsp_res->_TspState[u32EngId].FltType[OrgFltId] == E_DRVTSP_FLT_TYPE_SECTION) ||
        (_ptsp_res->_TspState[u32EngId].FltType[OrgFltId] == E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI) ||
        (_ptsp_res->_TspState[u32EngId].FltType[OrgFltId] == E_DRVTSP_FLT_TYPE_PACKET) ||
        (_ptsp_res->_TspState[u32EngId].FltType[OrgFltId] == E_DRVTSP_FLT_TYPE_SECTION_VER) ||
        (_ptsp_res->_TspState[u32EngId].FltType[OrgFltId] == E_DRVTSP_FLT_TYPE_PCR))
    {
        return FALSE;
    }

    u32pid = HAL_TSP_PidFlt_GetPid(HAL_TSP_GetPidFltReg(u32EngId, OrgFltId));
    PVRCA_DBG(ULOGD("TSP", "[%s][%d] Org pid %lx\n", __FUNCTION__, __LINE__, u32pid));
    if(u32pid == DRVTSP_PID_NULL)
    {
        return FALSE;
    }

    //Group TS0 -> Group TS1
    if(OrgFltId <=  DSCMB_FLT_END_ID)
    {
        #ifdef EXT_PVR1_FLT
        if(OrgFltId < DSCMB_FLT_START_ID)
        {
            u32OrgFltListIdx = OrgFltId + DSCMB_FLT_NUM;
        }
        else
        #endif
        {
            u32OrgFltListIdx = OrgFltId - DSCMB_FLT_START_ID;
        }

        PVRCA_DBG(ULOGD("TSP", "[%s][%d] #1 u32OrgFltListIdx %ld \n" ,__FUNCTION__, __LINE__, u32OrgFltListIdx));

        //searching the same pid in FLT1
        for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == u32pid)
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ii);
                _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32OrgFltListIdx] = u32pid;
                PVRCA_DBG(ULOGD("TSP", "[%s] Repeate: OrgFltId %ld, DupFltId %ld, u32pid 0x%lx\n", __FUNCTION__, OrgFltId, u32ii+TSP_SECFLT_NUM, u32pid));
                return TRUE;
            }
        }

        _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32OrgFltListIdx] = u32pid;

    }
    #ifdef TS2_LIVE_SUPPORT
    else if((OrgFltId >=  (TSP_SECFLT_NUM+TSP_PIDFLT1_NUM)) &&  (OrgFltId < (TSP_SECFLT_NUM+TSP_PIDFLT1_NUM + DSCMB_FLT_NUM)))//Group TS2 -> Group TS1
    {
        u32OrgFltListIdx = OrgFltId - TSP_SECFLT_NUM - TSP_PIDFLT1_NUM;

        PVRCA_DBG(ULOGD("TSP", "[%s][%d] #2 u32OrgFltListIdx %ld \n" , __FUNCTION__, __LINE__, u32OrgFltListIdx));

        //TS2 has no share key filters
        if(u32OrgFltListIdx >= DSCMB_FLT_NUM)
        {
            return FALSE;
        }

        //searching the same pid in FLT1 and before current index
        for(u32ii = 0; u32ii < DSCMB_FLT_NUM; u32ii++)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == u32pid)
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ii);
                _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32OrgFltListIdx] = u32pid;
                PVRCA_DBG(ULOGD("TSP", "[%s][%d] Repeate: OrgFltId %ld, DupFltId %ld, u32pid 0x%lx\n", __FUNCTION__, __LINE__, OrgFltId, u32ii+TSP_SECFLT_NUM+TSP_PIDFLT1_NUM, u32pid));
                return TRUE;
            }
        }

        _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32OrgFltListIdx] = u32pid;
    }
    #endif
    else
    {
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] 333 OrgFltId %ld \n" , __FUNCTION__, __LINE__, OrgFltId));
        return FALSE;
    }

    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, DupFltId), u32pid);
    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[DupFltId - TSP_SECFLT_NUM] = u32pid;
    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << (DupFltId - TSP_SECFLT_NUM));

    PVRCA_DBG(ULOGD("TSP", "[%s][%d] OrgFltId %ld, DupFltId %ld, u32pid 0x%lx\n", __FUNCTION__, __LINE__, OrgFltId, DupFltId, u32pid));

    return TRUE;
}

// If dscmb source from TS1, when live filter PIDs changing, also change dscmb PID mapping. (PVR filters)
static MS_BOOL _TSP_FLT1_DupFLT0_Pid(MS_U32 u32EngId, MS_U32 u32Flt0Id, MS_U32 u32Pid)
{
    MS_U32  u32ii = 0, u32jj = 0, u32kk = 0, u32Flt1_id = 0;
    MS_U32  u32mask = 0, u32Flt0ListId = 0;
    MS_BOOL bFreeDscmbPid = FALSE;

#ifdef EXT_PVR1_FLT
    MS_U32  u32DscmbFltNum = DSCMB_FLT_NUM_ALL;
#else
    MS_U32  u32DscmbFltNum = DSCMB_FLT_NUM;
#endif

    if( (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_PACKET) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION_VER) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_PCR))
    {
        return FALSE;
    }

    // Not in TS1 CA mode , not need to duplicate FLT0 pid  to FLT1
    if((HAL_TSP_CSA_Get_ScrmPath() & TSP_CA_INPUT_TSIF1) == 0)
    {
        return FALSE;
    }

    //If CA output to TS2 only, not need to duplicate FLT0 to FLT1
    #ifdef TS2_LIVE_SUPPORT
    if((HAL_TSP_CSA_Get_ScrmPath() & (TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2)) == TSP_CA_OUTPUT_PLAY_LIVE2)
    {
        return FALSE;
    }
    #endif

    //If Not PVRCA case, just duplicate FLT0 to FLT1 directly
    if(_ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable == FALSE)
    {
        if((HAL_TSP_CSA_Get_ScrmPath() & TSP_CA_OUTPUT_PLAY_LIVE) == 0)
        {
            return FALSE;
        }

        if(u32Flt0Id > DSCMB_FLT_END_ID)
        {
            return FALSE;
        }
        #ifdef EXT_PVR1_FLT
        // mapping FLT0 #0~#15 to FLT1 #16~#31
        if(u32Flt0Id <= DSCMB_FLT_SHAREKEY_END_ID)
        {
            u32Flt1_id = DSCMB_FLT_NUM + u32Flt0Id;
        }
        #endif

        // mapping FLT0 #16~#31 to FLT1 #0~#15
        if((u32Flt0Id <= DSCMB_FLT_END_ID) && (u32Flt0Id >= DSCMB_FLT_START_ID))
        {
            u32Flt1_id = u32Flt0Id - DSCMB_FLT_START_ID;
        }
        u32Flt0ListId = u32Flt1_id;

        PVRCA_DBG(ULOGD("TSP", "[%s] u32Flt0Id %ld, New PID 0x%lx, Old PID 0x%x\n",
            __FUNCTION__, u32Flt0Id, u32Pid, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0Id]));

        //=============================================//
        // Only put one pid in DSCMB list                                                       //
        //=============================================//

        //Free PID filter
        if(u32Pid == DRVTSP_PID_NULL)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] == DRVTSP_PID_NULL)
            {
                return FALSE;
            }

            for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
            {
                // Find the same Pid in FLT1 list
                if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii])
                {
                    //Check if the sam pid in FLT0, still use for dscmbling...
                    for(u32jj = 0; u32jj < u32DscmbFltNum; u32jj++)
                    {
                        if(u32jj == u32Flt0ListId)
                            continue;
                        if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32jj] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii])
                        {
                            break; //dscmb pid is still used
                        }
                    }
                    if(u32jj == u32DscmbFltNum)
                    {
                        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32ii);
                    }
                    _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] = DRVTSP_PID_NULL;

                    if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR|_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32ii)) != 0)
                    {
                        return FALSE;
                    }
                    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32ii + TSP_SECFLT_NUM), DRVTSP_PID_NULL);
                    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] = DRVTSP_PID_NULL;
                    PVRCA_DBG(ULOGD("TSP", "[%s] Free Null pid, u32Flt0Id %ld, Flt1Id 0x%ld\n", __FUNCTION__, u32Flt0Id, u32ii));
                    return TRUE;
                }
            }

            _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] = DRVTSP_PID_NULL;
            return FALSE;
        }

        //--- Checking mapping position first ----

        //---  1st, find the same pid already in FLT1 list.....
        for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
        {
            // Find the same Pid in FLT1 list
            if(u32Pid == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii])
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ii);
                break;
            }
        }

        //Sam pid found, check if the mapping position pid used, if not, free it
        if(u32ii < u32DscmbFltNum)
        {
            //ckeck if current pid is still for dscmbling....
            for(u32jj = 0; u32jj < u32DscmbFltNum; u32jj++)
            {
                if(u32jj == u32Flt0ListId)
                    continue;
                if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] == _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32jj])
                {
                    break;
                }
            }
            //if mapping dscmb pid is not used....
            if(u32jj == u32DscmbFltNum)
            {
                for(u32kk = 0; u32kk < u32DscmbFltNum; u32kk++)
                {
                    if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32kk])
                    {
                        break;
                    }
                }

                if(u32kk < u32DscmbFltNum)
                {
                    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32kk);
                    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32kk + TSP_SECFLT_NUM), DRVTSP_PID_NULL);
                    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32kk] = DRVTSP_PID_NULL;
                }
            }

            PVRCA_DBG(ULOGD("TSP", "[%s]Du32Flt0ListId %ld , u32Flt1_id %ld ---------------------\n", __FUNCTION__, u32Flt0ListId, u32ii));

            _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] = u32Pid;
             return TRUE;
        }
        //---  1st, end...

        //2nd, find mapping position if empty, use it
        if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR| _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32Flt1_id)) == 0)
        {
            goto UPDATE_PARAM;
        }

        //--- 3rd, if mapping position is not empty, checking if the dscmb pid still used...
        for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
        {
            if(u32ii == u32Flt0ListId)
                continue;
            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32Flt1_id] == _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii])
            {
                break;
            }
        }
        if(u32ii == u32DscmbFltNum)
        {
            goto UPDATE_PARAM;
        }
        //--- 3rd, end...

        //4th, find any empty position for new pid, because mapping postion is occupied
        for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
        {
            if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR| _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32ii)) == 0)
            {
                u32Flt1_id = u32ii;
                goto UPDATE_PARAM;
            }
        }

        PVRCA_DBG(ULOGD("TSP", "[%s][%d] failed \n", __FUNCTION__, __LINE__));

        return FALSE;

UPDATE_PARAM:
        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32Flt1_id);
        _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt0ListId] = u32Pid;
        _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32Flt1_id] = u32Pid;
        HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32Flt1_id + TSP_SECFLT_NUM), u32Pid);

        PVRCA_DBG(ULOGD("TSP", "[%s] u32Flt0Id %ld, u32Flt1_id %ld, PID 0x%lx\n", __FUNCTION__, u32Flt0Id, u32Flt1_id, u32Pid));

        return TRUE;
    }

    PVRCA_DBG(ULOGD("TSP", "[%s] u32Flt0Id %ld, PID 0x%lx\n", __FUNCTION__, u32Flt0Id, u32Pid));

    // transfer FLT0 index to FLT1 index
    if((u32Flt0Id >= DSCMB_FLT_START_ID) && (u32Flt0Id <= DSCMB_FLT_END_ID))
    {
        u32Flt1_id = u32Flt0Id - DSCMB_FLT_START_ID;
    }
#ifdef EXT_PVR1_FLT
    else if((u32Flt0Id >= DSCMB_FLT_SHAREKEY_START_ID) && (u32Flt0Id <= DSCMB_FLT_SHAREKEY_END_ID))
    {
        u32Flt1_id = DSCMB_FLT_NUM + u32Flt0Id - DSCMB_FLT_SHAREKEY_START_ID;
    }
#endif
    else
    {
        return FALSE;   //not in dscmb filter range, not need to duplicate
    }

    // if pid is null or section type, free this dscmb pid setting
    if( (u32Pid == DRVTSP_PID_NULL) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_PACKET) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION_VER))
    {
        if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id] == DRVTSP_PID_NULL)
        {
            return TRUE;
        }
        else
        {
            bFreeDscmbPid = TRUE;
        }
    }

    PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32Flt0Id %ld, PID 0x%lx\n", __FUNCTION__, __LINE__, u32Flt0Id, u32Pid));

    //------------------------------------------------------------------------------ //

    // Finding the same pid with live dscmb pid in PVR pid list, and release it
    if(_ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id] != DRVTSP_PID_NULL)
    {
        for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == DRVTSP_PID_NULL)
                continue;

            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id])
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32ii);
                if((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR & (1 << u32ii)) == 0)
                {
                    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] = DRVTSP_PID_NULL;
                    if(bFreeDscmbPid)
                    {
                        HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32ii + TSP_SECFLT_NUM), DRVTSP_PID_NULL);
                    }
                }
                break;
            }
        }
    }

    // free dscmb pid
    if(bFreeDscmbPid)
    {
        _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id] = DRVTSP_PID_NULL;

        PVRCA_DBG(ULOGD("TSP", "_u32Flt1Flag_DscmbRef 0x%lx, _u32Flt1Flag_DscmbPVR 0x%lx\n", \
                         _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR));
        PVRCA_DBG(ULOGD("TSP", "#1 _u16Flt0DscmbPidList[%ld] 0x%x\n", u32Flt1_id, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id]));
        return TRUE;
    }

    _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id] = u32Pid;

    // Searching new pid in FLT1 list
    for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
    {
        if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == DRVTSP_PID_NULL)
            continue;

        if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == u32Pid)
        {
            // If the same pid index is in the back, free it and setting the new pid in the front
            if((u32ii > u32Flt1_id) && ((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR & (1 << u32Flt1_id)) == 0))
            {
                _TSP_SetNewDscmbPid2FLT1(u32EngId, u32Flt1_id, u32Pid);
                _TSP_FreeDscmbPid_In_FLT1(u32EngId, u32ii);
            }
            else
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ii);
            }
            PVRCA_DBG(ULOGD("TSP", "_u32Flt1Flag_DscmbRef 0x%lx, _u32Flt1Flag_DscmbPVR 0x%lx\n", \
                             _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR));
            PVRCA_DBG(ULOGD("TSP", "#2  _u16Flt0DscmbPidList[%ld] 0x%x, _u16Flt1PidList[%ld] 0x%x\n", \
                             u32Flt1_id, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id], u32ii, _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii]));
            return TRUE;
        }
    }

    // If not found the new pid in FLT1 list, setting one to FLT1 list
    if(u32ii == u32DscmbFltNum)
    {
        u32mask = _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef | _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR;

        //check corresponding  position first
        if((u32mask & (1 << u32Flt1_id)) == 0)
        {
            u32ii = u32Flt1_id;
        }
        else
        {
            //finding a new empty position
            for(u32ii = 0; u32ii < u32DscmbFltNum; u32ii++)
            {
                if((u32mask & 0x01) == 0)
                {
                    break;
                }
                u32mask >>= 1;
            }
        }

        if(u32ii < u32DscmbFltNum)
        {
            _TSP_SetNewDscmbPid2FLT1(u32EngId, u32ii, u32Pid);
            PVRCA_DBG(ULOGD("TSP", "_u32Flt1Flag_DscmbRef 0x%lx, _u32Flt1Flag_DscmbPVR 0x%lx\n", \
                             _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR));
            PVRCA_DBG(ULOGD("TSP", "#3  _u16Flt0DscmbPidList[%ld] 0x%x, _u16Flt1PidList[%ld] 0x%x\n", \
                             u32Flt1_id, _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32Flt1_id], u32ii, _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii]));
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;
}

// If dscmb source from TS1, and output to TS2, must duplicate FLT2 Pid to  FLT1
#ifdef TS2_LIVE_SUPPORT
static MS_BOOL _TSP_FLT1_DupFLT2_Pid(MS_U32 u32EngId, MS_U32 u32Flt2Id, MS_U32 u32Pid)
{
    MS_U32 u32CaPath = 0;
    MS_U32 u32ArrayIdx = 0, u32ii = 0, u32jj = 0, u32kk = 0, u32FLt1Id = 0;

    //only process FLT2
    if(u32Flt2Id < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM))
        return FALSE;

    // if pid is section type, free this dscmb pid setting
    if( (_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_SECTION) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_PACKET) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_SECTION_VER) ||
        (_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_PCR))
    {
        return FALSE;
    }

    u32CaPath = HAL_TSP_CSA_Get_ScrmPath();

    //If CA input is TS2, not need to duplicate FLT2 to FLT1
    if(u32CaPath & TSP_CA_INPUT_TSIF2_EN)
    {
        return FALSE;
    }

    if((u32CaPath & TSP_CA_OUTPUT_PLAY_LIVE2) == 0)
    {
        return FALSE;
    }

    //=============================================//
    // Only put one pid in DSCMB list                                                       //
    //=============================================//

    u32ArrayIdx = u32Flt2Id - TSP_PIDFLT1_NUM - TSP_SECFLT_NUM;

    PVRCA_DBG(ULOGD("TSP", "[%s] u32Flt2Id %ld, New PID 0x%lx, Old PID 0x%x\n",
            __FUNCTION__, u32Flt2Id, u32Pid, _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx]));

    //Free PID filter
    if(u32Pid == DRVTSP_PID_NULL)
    {
        if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] == DRVTSP_PID_NULL)
        {
            return FALSE;
        }

        for(u32ii = 0; u32ii < DSCMB_FLT_NUM; u32ii++)
        {
            // Find the same Pid in FLT1 list
            if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii])
            {
                //Check if the sam pid in FLT2, still use this dscmb pid...
                for(u32jj = 0; u32jj < DSCMB_FLT_NUM; u32jj++)
                {
                    if(u32jj == u32ArrayIdx)
                        continue;
                    if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32jj] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii])
                    {
                        break; //dscmb pid is still used
                    }
                }
                if(u32jj == DSCMB_FLT_NUM)
                {
                    if(_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_CB)
                    {
                        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR &= ~(1 << u32ii);
                    }
                    else
                    {
                        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32ii);
                    }
                }
                _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] = DRVTSP_PID_NULL;
                if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR|_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32ii)) != 0)
                {
                    PVRCA_DBG(ULOGE("TSP", "[%s][%d] Free Null pid used, u32Flt2Id %ld, Flt1Id 0x%ld\n", __FUNCTION__, __LINE__, u32ArrayIdx, u32ii));
                    return FALSE;
                }
                PVRCA_DBG(ULOGD("TSP", "[%s] Free Null pid, u32Flt2Id %ld, Flt1Id 0x%ld\n", __FUNCTION__, u32ArrayIdx, u32ii));
                HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32ii + TSP_SECFLT_NUM), DRVTSP_PID_NULL);
                _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] = DRVTSP_PID_NULL;
                return TRUE;
            }
        }

        _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] = DRVTSP_PID_NULL;

        return FALSE;
    }

    //--- Checking mapping position first ----

    //---  1st, find the same pid already in FLT1 list.....
    for(u32ii = 0; u32ii < DSCMB_FLT_NUM; u32ii++)
    {
        // Find the same Pid in FLT1 list
        if(u32Pid == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii])
        {
            if(_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_CB)
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR |= (1 << u32ii);
            }
            else
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ii);
            }
            break;
        }
    }
    //Sam pid found, check if the mapping position pid used, if not, free it
    if(u32ii < DSCMB_FLT_NUM)
    {
        for(u32jj = 0; u32jj < DSCMB_FLT_NUM; u32jj++)
        {
            if(u32jj == u32ArrayIdx)
                continue;
            if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] == _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32jj])
            {
                break;
            }
        }
        //Finding the same dscmb pid, if it is not used any more..., free it
        if(u32jj == DSCMB_FLT_NUM)
        {
            for(u32kk = 0; u32kk < DSCMB_FLT_NUM; u32kk++)
            {
                if(_ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] == _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32kk])
                {
                    break;
                }
            }
            if(u32kk < DSCMB_FLT_NUM)
            {
                if(_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_CB)
                {
                    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR &= ~(1 << u32kk);
                }
                else
                {
                    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef &= ~(1 << u32kk);
                }
                if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR| _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32kk)) == 0)
                {
                    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32kk + TSP_SECFLT_NUM), DRVTSP_PID_NULL);
                    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32kk] = DRVTSP_PID_NULL;
                }
            }
        }
        _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] = u32Pid;
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32Flt2ListId %ld , u32Flt1_id %ld ---------------------\n",
            __FUNCTION__, __LINE__, u32ArrayIdx, u32ii));
        return TRUE;
    }
    //---  1st, end...

    //2nd, find mapping position if empty, use it
    if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR| _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32ArrayIdx)) == 0)
    {
        if(_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_CB)
        {
            _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR |= (1 << u32ArrayIdx);
        }
        else
        {
            _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ArrayIdx);
        }
        u32FLt1Id = u32ArrayIdx;
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32Flt2ListId %ld , u32Flt1_id %ld ---------------------\n",
            __FUNCTION__, __LINE__, u32ArrayIdx, u32FLt1Id));
        goto SET_PID;
    }

    //--- 3rd, if mapping position is not empty, checking if the dscmb pid still used...
    for(u32ii = 0; u32ii < DSCMB_FLT_NUM; u32ii++)
    {
        if(u32ii == u32ArrayIdx)
            continue;
        if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ArrayIdx] == _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ii])
        {
            break;
        }
    }
    if(u32ii == DSCMB_FLT_NUM)
    {
        if(_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_CB)
        {
            _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR |= (1 << u32ArrayIdx);
        }
        else
        {
            _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ArrayIdx);
        }
        u32FLt1Id = u32ArrayIdx;
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32Flt2ListId %ld , u32Flt1_id %ld ---------------------\n",
            __FUNCTION__, __LINE__, u32ArrayIdx, u32FLt1Id));
        goto SET_PID;
    }
    //--- 3rd, end...

    //4th, find any empty position for new pid, because mapping postion is occupied
    for(u32ii = 0; u32ii < DSCMB_FLT_NUM; u32ii++)
    {
        if(((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR| _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef) & (1 << u32ii)) == 0)
        {
            if(_ptsp_res->_TspState[u32EngId].FltType[u32Flt2Id] == E_DRVTSP_FLT_TYPE_CB)
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR |= (1 << u32ii);
            }
            else
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32ii);
            }
            u32FLt1Id = u32ii;
            PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32Flt2ListId %ld , u32Flt1_id %ld ---------------------\n",
                __FUNCTION__, __LINE__, u32ArrayIdx, u32FLt1Id));
            goto SET_PID;
        }
    }

    PVRCA_DBG(ULOGE("TSP", "[%s][%d] failed \n", __FUNCTION__, __LINE__));

    return FALSE;

SET_PID:
    HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32FLt1Id + TSP_SECFLT_NUM), u32Pid);
    _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32FLt1Id] = u32Pid;
    _ptsp_res->TSPPvrcaParams._u16Flt2PidList[u32ArrayIdx] = u32Pid;
    PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32Flt2Id %ld, u32Flt1Id %ld, PID 0x%lx\n", __FUNCTION__, __LINE__, u32Flt2Id, u32FLt1Id, u32Pid));
    return TRUE;

}
#endif //end TS2_IF_SUPPORT

#endif

static void _TSP_PidFlt_Init(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_HWPCR_FLT_START_ID)
    {
        MS_U32 idx = u32PidFltId - TSP_HWPCR_FLT_START_ID;

        HAL_TSP_PidFlt_HWPcrFlt_Enable(idx, FALSE);
        HAL_TSP_PidFlt_SetHWPcrPid(idx, DRVTSP_PID_NULL);
        HAL_TSP_HWPcr_SelSrc(idx, 0);
    }
    else
    {
#endif
        REG_PidFlt *pPidFilter = HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId);

        HAL_TSP_PidFlt_SetPid(pPidFilter, DRVTSP_PID_NULL);
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_NONE);
        HAL_TSP_PidFlt_SelSecFlt(pPidFilter, TSP_PIDFLT_SECFLT_NULL);
        _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] = DRVTSP_MAP_NULL;

        #ifndef NO_CA
        _TSP_FLT1_DupFLT0_Pid(0, u32PidFltId, DRVTSP_PID_NULL);
        #endif

#ifdef HWPCR_ENABLE
    }
#endif
}

static void _TSP_PidFlt_Disable(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_HWPCR_FLT_START_ID)
    {
        MS_U32 u32Idx = u32PidFltId - TSP_HWPCR_FLT_START_ID;

        HAL_TSP_PidFlt_HWPcrFlt_Enable(u32Idx, FALSE);
        #if (TSP_HWPCR_BY_HK == 1)
        HAL_TSP_HWPcr_Int_Enable(u32Idx, FALSE);
        _TSP_RemoveHwPcrNotifyId(u32Idx);
        #endif
    }
    else
    {
#endif

        REG_PidFlt *pPidFilter = HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId);

        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_NONE);
        //HAL_TSP_SecFlt_SetRmnCount(&(_TspSec[u32EngId].Flt[u32PidFltId]), 0);

#ifdef HWPCR_ENABLE
    }
#endif

    _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] &= (DrvTSP_FltState)(~E_DRVTSP_FLT_STATE_ENABLE);
}

//Note: This is only for PID filter & Section filter to be one by one mapping, and section filter number is equal to pid filter number.
static MS_BOOL _TSP_PidFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32PidFltId)
{
    int  i = 0;
    MS_U32  pid_idx_start = 0;
    MS_U32  pid_idx_end = 0;
    MS_BOOL bFound = FALSE;

    *pu32PidFltId = 0xFFFFFFFF;

    // input parameter validation
    if ( (u32EngId >= TSP_ENGINE_NUM) ||
         ((eFilterType&~E_DRVTSP_FLT_MASK) >= E_DRVTSP_FLT_TYPE_LAST_ENUM) )
    {
        TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] (EngId, FilterType)= (%d %x)\n", __LINE__, (unsigned int)u32EngId, (unsigned int)eFilterType));
        return FALSE;
    }

    //TS2 live pid filter allocate
#ifdef TS2_IF_SUPPORT
    if ((E_DRVTSP_FLT_TYPE_CB== (eFilterType&~E_DRVTSP_FLT_MASK))
        || ((eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_TS2) &&
            ((E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK)) || ((eFilterType & E_DRVTSP_FLT_TYPE_SECTION_MASK) == 0))))
    {
        pid_idx_start = (TSP_PIDFLT_NUM_ALL & 0xFFFFFFFE) - 1; //except of HW PCR
        pid_idx_end = TSP_SECFLT_NUM + TSP_PIDFLT1_NUM;
        for(i = (int)pid_idx_start; i >= (int)pid_idx_end; i--)
        {
            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
    }
    else if((eFilterType&E_DRVTSP_FLT_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_TS2)
    {
        pid_idx_start = TSP_SECFLT_NUM+TSP_PIDFLT1_NUM;
        pid_idx_end =  TSP_PIDFLT_NUM_ALL & 0xFFFFFFFE; //except of HW PCR
        for(i = (int)pid_idx_start; i < (int)pid_idx_end; i++)
        {
            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
    }
    else
#endif

#ifdef HWPCR_ENABLE
    if(E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK))
    {
        for(i = TSP_HWPCR_FLT_START_ID; i < TSP_PIDFLT_NUM_ALL; i++)
        {
            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
    }
    else
#endif
#ifdef DSCMB_SHAREKEY_FLT
    if (E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY == (eFilterType & E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY))
    {
        for (i = DSCMB_FLT_SHAREKEY_END_ID; i >= DSCMB_FLT_SHAREKEY_START_ID; i--)
        {
            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
    }
    else
#endif
    if ((E_DRVTSP_FLT_TYPE_SCMB == (eFilterType & E_DRVTSP_FLT_SCMB_MASK)) ||
#ifndef HWPCR_ENABLE
         (E_DRVTSP_FLT_TYPE_PCR    == (eFilterType&~E_DRVTSP_FLT_MASK))      ||
#endif
         (E_DRVTSP_FLT_TYPE_VIDEO  == (eFilterType&~E_DRVTSP_FLT_MASK))      ||
         (E_DRVTSP_FLT_TYPE_AUDIO  == (eFilterType&~E_DRVTSP_FLT_MASK))      ||
         (E_DRVTSP_FLT_TYPE_AUDIO2 == (eFilterType&~E_DRVTSP_FLT_MASK))      ||
         (E_DRVTSP_FLT_TYPE_VIDEO3D == (eFilterType&~E_DRVTSP_FLT_MASK))      )
    {
        #ifndef NO_CA
        for (i = DSCMB_FLT_END_ID; i >= DSCMB_FLT_START_ID; i--)
        #else
        for (i = TSP_SECFLT_NUM - 1; i >= 0; i--)
        #endif
        {
            //Reserved filter #31 for NMatch temp writting, can not be used for section data
            if((i == 31) && (!((E_DRVTSP_FLT_TYPE_VIDEO == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_AUDIO == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_AUDIO2 == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_VIDEO3D == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK)))))
            {
                continue;
            }

            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }

        if((bFound == FALSE) && (eFilterType & E_DRVTSP_FLT_SCMB_MASK))
        {
            TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] No Free dscmb filters !!!\n", __LINE__));
        }

        //if not scmb type, find another filter range
        #ifndef NO_CA
        if((bFound == FALSE) && ((eFilterType & E_DRVTSP_FLT_SCMB_MASK) == 0))
        {
            pid_idx_start=          TSP_SECFLT_NUM - 1;
            pid_idx_end=            0;
            for (i = (int)pid_idx_start; i >= (int)pid_idx_end; i--)
            {
               //dscmb range has checked, skip
               if(i == DSCMB_FLT_END_ID)
               {
                   i = DSCMB_FLT_START_ID;
                   continue;
               }
               if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
               {
                   bFound = TRUE;
                   break;
               }
           }
        }
        #endif
    }
    else
    {
        if (E_DRVTSP_FLT_TYPE_PVR== (eFilterType&~E_DRVTSP_FLT_MASK))
        {
#ifndef NO_PVR
            if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
            {
                pid_idx_start=          0;
                pid_idx_end=            TSP_SECFLT_NUM;
            }
            else
            {
                pid_idx_start=          TSP_SECFLT_NUM;
                pid_idx_end=            TSP_SECFLT_NUM + TSP_PIDFLT1_NUM;
            }
#endif
        }
        else
        {
            pid_idx_start=          0;
            pid_idx_end=            TSP_SECFLT_NUM;
        }

        for (i = (int)pid_idx_start; i < (int)pid_idx_end; i++)
        {
            //Reserved filter #31 for NMatch temp writting, can not be used for section data
            if(i == 31)
            {
                continue;
            }

            if (_ptsp_res->_TspState[u32EngId].FltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }

    }

    if (bFound == FALSE)
    {
        TSP_ASSERT(0, TSP_DBG("[TSP_ERROR][%06d] No free fitler found 0x%02x\n", __LINE__, i));
        return FALSE;
    }

    _ptsp_res->_TspState[u32EngId].FltState[i] = E_DRVTSP_FLT_STATE_ALLOC;
    _ptsp_res->_TspState[u32EngId].FltType[i] = (eFilterType&(DrvTSP_FltType)(~E_DRVTSP_FLT_MASK));

    if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_LIVE)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[i] = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
    }
    else if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[i] = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
    }
    else if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_TS2)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[i] = E_DRVTSP_FLT_SOURCE_TYPE_TS2;
    }

    _TSP_PidFlt_Init(u32EngId, i);
    *pu32PidFltId = i;

    //ULOGD("TSP", "[%s] eFilterType %lx , id %d\n", __FUNCTION__, eFilterType, i);

    return TRUE;
}

static TSP_Result _TSP_PidFlt_Free(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bResetState)
{
    MS_U32 u32SecFltId = _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId];

    if ( (u32EngId    >= TSP_ENGINE_NUM) ||
         (u32PidFltId >= TSP_PIDFLT_NUM_ALL) ||
         (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] == E_DRVTSP_FLT_STATE_FREE) )
    {
        //TSP_DBG("[TSP_ERROR][%06d] (EngId, PidFltId, FilterState)= (%ld %ld %d)\n", __LINE__, u32EngId, u32PidFltId, _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId]);
        MS_CRITICAL_MSG(TSP_DBG("[TSP_ERROR][%06d] Bad free parmeter\n",__LINE__));;
        return (TSP_Result)FALSE;
    }

    _TSP_PidFlt_Disable(u32EngId, u32PidFltId);

    // Release Filter
    _TSP_PidFlt_Init(u32EngId, u32PidFltId);

#ifdef HWPCR_ENABLE
    if(u32PidFltId < TSP_HWPCR_FLT_START_ID)
    {
#endif
        HAL_TSP_PidFlt_SelSecFlt(HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId), TSP_PIDFLT_SECFLT_NULL);
        _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] = DRVTSP_MAP_NULL;
#ifdef HWPCR_ENABLE
    }
#endif

    // clear all event while filter free
    if(u32SecFltId < TSP_SECFLT_NUM_All)
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

    // Reset call back function without PVR
    if (u32SecFltId < TSP_SECFLT_NUM_All)
    {
        _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecFltId] = NULL;
        _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId] = E_DRVTSP_FLT_MODE_CONTI;
    }

    return (TSP_Result)TRUE;
}

static MS_BOOL _TSP_PidFlt_StateGet(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltState *peState)
{
    MS_U32 u32SecFltId;
    MS_U32 u32GroupId = (((u32PidFltId & 0x1f) > 0) ? 1: 0);
    REG_SecFlt *pSecFilter = NULL;
    REG_PidFlt *pPidFilter = NULL;

    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFlt Id %d\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT(peState, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    *peState=   _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId];

#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_HWPCR_FLT_START_ID)
    {
        return TRUE;
    }
#endif

    pPidFilter= HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId);
    if(u32PidFltId < TSP_SECFLT_NUM)
    {
        if (HAL_TSP_Scmb_Status(0, u32GroupId, u32PidFltId) || HAL_TSP_Scmb_Status(1, u32GroupId, u32PidFltId))
        {
            *peState |= E_DRVTSP_FLT_STATE_SCRAMBLED;
        }
    }

    switch (_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
    case E_DRVTSP_FLT_TYPE_VIDEO:
    case E_DRVTSP_FLT_TYPE_AUDIO:
    case E_DRVTSP_FLT_TYPE_AUDIO2:
    case E_DRVTSP_FLT_TYPE_VIDEO3D:
        break;
    case E_DRVTSP_FLT_TYPE_SECTION:
    case E_DRVTSP_FLT_TYPE_PES:
    case E_DRVTSP_FLT_TYPE_PACKET:
    case E_DRVTSP_FLT_TYPE_TELETEXT:
    case E_DRVTSP_FLT_TYPE_SECTION_VER:
    case E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI:
        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(pPidFilter);
        pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);
        if (HAL_TSP_SecBuf_GetBufRead(pSecFilter) != HAL_TSP_SecBuf_GetBufWrite(pSecFilter))
        {
            *peState |= E_DRVTSP_FLT_STATE_STREAM_AVAIL;
        }
        break;
    case E_DRVTSP_FLT_TYPE_PCR:
    default:
        break;
    }
    return TRUE;
}


static void _TSP_SecFlt_SetType(REG_SecFlt *pSecFilter, DrvTSP_FltType eSecFltType)
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
#ifdef TS2_IF_SUPPORT
    case E_DRVTSP_FLT_TYPE_CB:
#endif
    default:
        break;
    }
    HAL_TSP_SecFlt_SetType(pSecFilter, u32FltType);
}

static MS_BOOL _TSP_SecFlt_StateGet(MS_U32 u32EngId, MS_U32 u32SecFltId, DrvTSP_FltState *peState)
{
    REG_SecFlt      *pSecFilter = NULL;

    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT(peState, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);
    *peState =  _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId];
    if (HAL_TSP_SecFlt_GetState(pSecFilter) & TSP_SECFLT_STATE_OVERFLOW)
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
    REG_SecFlt      *pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_SecFlt_ClrCtrl(pSecFilter);

    HAL_TSP_SecFlt_SelSecBuf(pSecFilter, TSP_SECFLT_BUF_NULL, FALSE);
    HAL_TSP_SecFlt_SetMatch(pSecFilter, u8Mask);
    HAL_TSP_SecFlt_SetMask(pSecFilter, u8NMask);
    //HAL_TSP_SecFlt_SetNMask(&(_TspSec[u32EngId].Flt[u32SecFltId]), u8Mask);
    _TSP_SecFlt_SetType(pSecFilter, E_DRVTSP_FLT_TYPE_SECTION);
    HAL_TSP_SecFlt_SetReqCount(pSecFilter, 0);
    HAL_TSP_SecFlt_ResetState(pSecFilter);
    _ptsp_res->_TspState[0].SecFltEvtNotify[u32SecFltId] = E_DRVTSP_EVENT_DATA_INIT;
    _ptsp_res->_TspState[0].SecFltCallback[u32SecFltId]  = NULL;
}

static MS_BOOL _TSP_SecFlt_Alloc(MS_U32 u32EngId, DrvTSP_FltType eFilterType, MS_U32 *pu32SecFltId)
{
    int     i = 0, flt_idx_start = 0, flt_idx_end = 0;
    MS_BOOL bFound = FALSE;
    MS_U8 u8SecFlt_num = TSP_SECFLT_NUM;

    i = *pu32SecFltId;
    *pu32SecFltId = 0xFFFFFFFF;

    if ((i < TSP_SECFLT_NUM_All) && (_ptsp_res->_TspState[u32EngId].SecFltState[i] == E_DRVTSP_FLT_STATE_FREE))
    {
        bFound = TRUE;
        goto SECFLT_ALC_END;
    }

#ifdef DSCMB_SHAREKEY_FLT
    if (E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY == (eFilterType & E_DRVTSP_FLT_TYPE_SCMB_SHAREKEY))
    {
        for (i = DSCMB_FLT_SHAREKEY_END_ID; i >= DSCMB_FLT_SHAREKEY_START_ID; i--)
        {
            if (_ptsp_res->_TspState[u32EngId].SecFltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
    }
    else
#endif
    if ((E_DRVTSP_FLT_TYPE_SCMB == (eFilterType & E_DRVTSP_FLT_SCMB_MASK)) ||
    (E_DRVTSP_FLT_TYPE_VIDEO== (eFilterType&~E_DRVTSP_FLT_MASK))||
    (E_DRVTSP_FLT_TYPE_AUDIO== (eFilterType&~E_DRVTSP_FLT_MASK))||
    (E_DRVTSP_FLT_TYPE_AUDIO2== (eFilterType&~E_DRVTSP_FLT_MASK))||
    (E_DRVTSP_FLT_TYPE_VIDEO3D== (eFilterType&~E_DRVTSP_FLT_MASK))||
    (E_DRVTSP_FLT_TYPE_PCR== (eFilterType&~E_DRVTSP_FLT_MASK)))
    {
        for (i = DSCMB_FLT_END_ID; i >= 0; i--)
        {
            //Reserved filter #31 for NMatch temp writting, can not be used for section data
            if((i == 31) && (!((E_DRVTSP_FLT_TYPE_VIDEO == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_AUDIO == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_AUDIO2 == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_VIDEO3D == (eFilterType&~E_DRVTSP_FLT_MASK)) ||
                (E_DRVTSP_FLT_TYPE_PCR == (eFilterType&~E_DRVTSP_FLT_MASK)))))
            {
                    continue;
            }

            if (_ptsp_res->_TspState[u32EngId].SecFltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
        if((bFound == FALSE) && (u8SecFlt_num > (DSCMB_FLT_END_ID+1)))
        {
            flt_idx_start=          TSP_SECFLT_NUM - 1;
            flt_idx_end=            DSCMB_FLT_END_ID;
            for (i = flt_idx_start ; i > flt_idx_end ; i--)
            {
                if (_ptsp_res->_TspState[u32EngId].SecFltState[i] == E_DRVTSP_FLT_STATE_FREE)
                {
                    bFound = TRUE;
                    break;
                }
            }
        }
    }
    else
    {
        for (i = 0; i < TSP_SECFLT_NUM; i++)
        {
            //Reserved filter #31 for NMatch temp writting, can not be used for section data
            if(i == 31)
            {
                continue;
            }

            if (_ptsp_res->_TspState[u32EngId].SecFltState[i] == E_DRVTSP_FLT_STATE_FREE)
            {
                bFound = TRUE;
                break;
            }
        }
    }

SECFLT_ALC_END:

    if (bFound == FALSE)
    {
        TSP_DBG("[TSP_ERROR][%06d] Allocate Sec Filter Failed\n", __LINE__);
        return FALSE;
    }

    _ptsp_res->_TspState[u32EngId].SecFltState[i] = E_DRVTSP_FLT_STATE_ALLOC;
    _TSP_SecFlt_Init(u32EngId, i);
    *pu32SecFltId = i;
    _TSP_SetSecNotifyId(*pu32SecFltId); //for isr processing checking

    //ULOGD("TSP", "[%s] %d \n", __FUNCTION__, i);

    return TRUE;
}

static MS_BOOL _TSP_SecFlt_Free(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_BOOL bResetState)
{
    //REG_SecFlt      *pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    //HAL_TSP_SecFlt_ResetEmmIdx(pSecFilter);
    //HAL_TSP_SecFlt_SelSecBuf(pSecFilter, TSP_SECFLT_BUF_NULL, FALSE);
    if (bResetState)
    {
        // clear all event while filter free,
        MS_U32 u32Dummy = 0;
        _TSP_GetSecOvfId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecCrcId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecRdyId    (u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);
        _TSP_GetFltDisableId(u32SecFltId, &u32Dummy, OSAL_EVENT_OR_CLEAR);

        _TSP_RemoveSecNotifyId(u32SecFltId);
        _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId] = E_DRVTSP_FLT_STATE_FREE;

        //ULOGD("TSP", "[%s] %ld \n", __FUNCTION__, u32SecFltId);
    }

    _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] = E_DRVTSP_EVENT_DATA_INIT;
    return TRUE;
}


static void _TSP_SecBuf_Init(MS_U32 u32EngId, MS_U32 u32SecBufId)
{
    REG_SecFlt      *pSecBuf= _TSP_GET_SECFLT_REG(u32SecBufId);

    HAL_TSP_SecBuf_SetBuffer(pSecBuf, 0UL, 0UL);
    HAL_TSP_SecBuf_ResetBuffer(pSecBuf);
}

static MS_BOOL _TSP_SecBuf_Alloc(MS_U32 u32EngId, MS_U32 *pu32SecBufId)
{
    int     i = *pu32SecBufId;

    *pu32SecBufId = 0xFFFFFFFF;

    if ((i < TSP_SECBUF_NUM) && (_ptsp_res->_TspState[u32EngId].SecBufState[i] == E_DRVTSP_FLT_STATE_FREE))
    {
        goto SECBUF_ALC_END;
    }

    for (i = 0; i < TSP_SECBUF_NUM; i++)
    {
        //Reserved filter #31 for NMatch temp writting, can not be used for section data
        if(i == 31)
        {
            continue;
        }

        if (_ptsp_res->_TspState[u32EngId].SecBufState[i] == E_DRVTSP_FLT_STATE_FREE)
        {
            break;
        }
    }
    if (i == TSP_SECBUF_NUM)
    {
        TSP_DBG("[TSP_ERROR][%06d] Allocate Sec Buf Failed\n", __LINE__);
        return FALSE;
    }

SECBUF_ALC_END:
    _ptsp_res->_TspState[u32EngId].SecBufState[i] = E_DRVTSP_FLT_STATE_ALLOC;
    *pu32SecBufId = i;

    //ULOGD("TSP", "[%s] %d \n", __FUNCTION__, i);
    return TRUE;
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
        for (u32FilterId = 0; u32FilterId < TSP_SECFLT_NUM_All; u32FilterId++)
        {
            _TSP_SecBuf_Init(u32EngId, u32FilterId);
            //HAL_TSP_SecBuf_SetBuffer(&(_TspSec[u32EngId].Flt[u32FilterId]), 0, 0);
            _TSP_SecFlt_Init(u32EngId, u32FilterId);
        }
    }
}

static TSP_Result _TSP_SelPad(MS_U32 u32EngId, DrvTSP_If eIf, DrvTSP_If_Set* pIfSet)
{
    MS_U32 u32Flow = 0;

    if (u32EngId>= TSP_ENGINE_NUM)
    {
        return DRVTSP_FAIL;
    }

    switch (eIf)
    {
    case E_DRVTSP_IF_PLAYBACK:
        u32Flow = 0;
        break;
#ifndef NO_PVR
    case E_DRVTSP_IF_PVR0:
        u32Flow = 1;
        break;
#endif
#ifdef TS2_IF_SUPPORT
    case E_DRVTSP_IF_PVR1:
        u32Flow = 2;
        break;
#endif
    default:
        MS_ASSERT(0);
        return DRVTSP_FAIL;
    }

    HAL_TSP_SelPad(u32EngId, u32Flow, (MS_U32)pIfSet->ePad);
    HAL_TSP_SelPad_ClkInv(u32EngId, u32Flow, pIfSet->bClkInv);
    HAL_TSP_SelPad_ExtSync(u32EngId, pIfSet->bExtSync, u32Flow);
    HAL_TSP_SelPad_Parl(u32EngId, pIfSet->bParallel, u32Flow);

#ifdef TS2_IF_SUPPORT
    if((pIfSet->bParallel == FALSE) && (pIfSet->ePad == E_DRVTSP_PAD_EXT_INPUT2))
    {
        HAL_TSP_TS2Out_En(TRUE);
    }
    else
    {
        HAL_TSP_TS2Out_En(FALSE);
    }
#endif

    return DRVTSP_OK;
}

#if 0
static MS_BOOL _TSP_IsPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_U32 u32Flag = 0;

    if (u32PidFltId < TSP_SECFLT_NUM)
    {
        _TSP_GetEventFreeDefer0Id(u32PidFltId, &u32Flag, E_AND);
        return ((u32Flag & (1 << (u32PidFltId & 0x1F))) ? TRUE: FALSE);
    }
    else
    {
        return FALSE;
    }
}

static void _TSP_SetPidFltFreeDefer(MS_U32 u32PidFltId)
{
    if (u32PidFltId < TSP_SECFLT_NUM)
    {
        _TSP_SetEventFreeDefer0Id(u32PidFltId);
    }
}

static void _TSP_ResetPidFltFreeDefer(MS_U32 u32PidFltId)
{
    MS_U32 u32Flag = 0;

    if (u32PidFltId < TSP_SECFLT_NUM)
    {
        _TSP_GetEventFreeDefer0Id(u32PidFltId, &u32Flag, E_OR_CLEAR);
    }
}
#endif

#if 0
static MS_BOOL _TSP_IsAskPidFltFree(MS_U32 u32PidFltId)
{
    MS_U32 u32Flag = 0;

    //PVR pid filter, not set to section filter free queue
#ifndef TS2_LIVE_SUPPORT
    if(u32PidFltId >= TSP_SECFLT_NUM)
        return TRUE;
#else
    if((u32PidFltId >= TSP_SECFLT_NUM) && (u32PidFltId < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM)))
        return TRUE;
#endif

    _TSP_GetEventFree0Id(u32PidFltId, &u32Flag, E_AND);
    return ((u32Flag & (1 << (u32PidFltId & 0x1F))) ? TRUE: FALSE);
}

static void _TSP_AskPidFltFree(MS_U32 u32PidFltId)
{
    //PVR pid filter, not set to section filter free queue
#ifndef TS2_LIVE_SUPPORT
    if(u32PidFltId >= TSP_SECFLT_NUM)
        return;
#else
    if((u32PidFltId >= TSP_SECFLT_NUM) && (u32PidFltId < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM)))
        return;
#endif

    _TSP_SetEventFree0Id(u32PidFltId);

}

//Free Section PID filters, 32 filters for one group
//For 64 pid filters, there are 2 gropus
//For 128 pid filters, there are 4 groups
//Free Section PID filters
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
    MS_U32              u32SecId = u32FltId;
    DrvTSP_FltState     PidFltState;
    DrvTSP_FltState     SecFltState;

    if (!_TSP_PidFlt_StateGet(u32EngId, u32FltId, &PidFltState))
    {
         return FALSE;
    }
    *peState=   PidFltState;

    if (u32FltId >= TSP_SECFLT_NUM
#ifdef TS2_LIVE_SUPPORT
        && (u32FltId < (TSP_SECFLT_NUM+TSP_PIDFLT1_NUM))
#endif
        )
    {
         return TRUE;
    }

#ifdef TS2_LIVE_SUPPORT
    if (u32FltId >= TSP_SECFLT_NUM)
    {
        u32SecId -= TSP_PIDFLT1_NUM;
    }
#endif

    if (!_TSP_SecFlt_StateGet(u32EngId, u32SecId, &SecFltState))
    {
         return FALSE;
    }
    *peState|=  SecFltState;
    return TRUE;
}

#ifndef NO_CA
// Duplicate live dscmb PIDs to FLT1 for DSCMB reference when siwtcing to TS1 PVRCA
// Paired with _TSP_Free_AllFLT1
//
// PVR1 filter arrange
//  -----------------   0
//  |   DSCM                |
//  -----------------   16
//  |   Share DSCMB    |
//  -----------------   32
static MS_BOOL _TSP_FLT1_Duplicate_FLT0(MS_U32 u32EngId)
{
    MS_U32 u32ii, u32jj;
    MS_U32 u32DMXID = 0, u32PID = DRVTSP_PID_NULL;
    MS_U32 u32Flt0Id = 0;

    #ifdef EXT_PVR1_FLT
    MS_U32 u32DscmbFltNume = DSCMB_FLT_NUM_ALL;
    #else
    MS_U32 u32DscmbFltNume = DSCMB_FLT_NUM;
    #endif

    PVRCA_DBG(ULOGD("TSP", "\n[%s]\n", __FUNCTION__));

    if(_ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable == TRUE)
    {
        return FALSE;
    }

    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef = 0;
    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR = 0;

    for(u32ii = 0; u32ii < TSP_PIDFLT1_NUM; u32ii++)
    {
        u32DMXID = u32ii + TSP_SECFLT_NUM;
        _ptsp_res->_TspState[0].FltState[u32ii + TSP_SECFLT_NUM] = E_DRVTSP_FLT_STATE_ALLOC;
        _ptsp_res->_TspState[0].FltType[u32ii + TSP_SECFLT_NUM] = E_DRVTSP_FLT_TYPE_PVR;
        _ptsp_res->_TspState[0].FltSource[u32ii + TSP_SECFLT_NUM] = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;

        //Getting PIDs of PVR1 filter, if pid filter is allocated for recording
        u32PID = HAL_TSP_PidFlt_GetPid(HAL_TSP_GetPidFltReg(u32EngId, u32DMXID));
        _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] = u32PID;

        if(u32ii < DSCMB_FLT_NUM)
        {
            u32Flt0Id = u32ii + DSCMB_FLT_START_ID;
        }
        #ifdef EXT_PVR1_FLT
        else if (u32ii < u32DscmbFltNume)//share dscmb filter
        {
            u32Flt0Id = u32ii - DSCMB_FLT_NUM + DSCMB_FLT_SHAREKEY_START_ID;
        }
        #endif
        else
        {
            continue;
        }

        PVRCA_DBG(ULOGD("TSP", "[%s]u32Flt0Id %ld\n", __FUNCTION__, u32Flt0Id));

        if (_ptsp_res->_TspState[u32EngId].FltState[u32Flt0Id] == E_DRVTSP_FLT_STATE_FREE)
            continue;

        if((_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION_NO_PUSI) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_PACKET) ||
            (_ptsp_res->_TspState[u32EngId].FltType[u32Flt0Id] == E_DRVTSP_FLT_TYPE_SECTION_VER))
        {
            //not dscmb type
            continue;
        }

        u32PID = HAL_TSP_PidFlt_GetPid(HAL_TSP_GetPidFltReg(u32EngId, u32Flt0Id));

        if(u32PID == DRVTSP_PID_NULL)
        {
            continue;
        }

        _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii] = (MS_U16)u32PID;

        PVRCA_DBG(ULOGD("TSP", "[%s]u32Flt0Id %ld, u32PID %lx \n", __FUNCTION__, u32Flt0Id, u32PID));

        // Searching FLT1 list, if pid repeated, do not set it again
        for(u32jj = 0; u32jj < u32DscmbFltNume; u32jj++)
        {
            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32jj] == DRVTSP_PID_NULL)
                continue;

            if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32jj] == (MS_U16)u32PID)
            {
                _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef |= (1 << u32jj);
                break;
            }
        }

        // If not exsit in FLT1 list, setting a new one
        if(u32jj == u32DscmbFltNume)
        {
            _TSP_SetNewDscmbPid2FLT1(u32EngId, u32ii, u32PID);
            PVRCA_DBG(ULOGD("TSP", "[%s]FLT %ld: PID 0x%lx, _u32Flt1Flag_DscmbRef 0x%lx\n", __FUNCTION__,u32ii, u32PID, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef));
        }
    }

    _ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable = TRUE;

    return TRUE;
}

// Free all FLT1 when PVRCA close
// Paired with _TSP_FLT1_Duplicate_FLT0
static MS_BOOL _TSP_Free_AllFLT1(void)
{
    MS_U32 u32ii, u32Dmxid;
    TSP_Result Res = DRVTSP_OK;

    PVRCA_DBG(ULOGD("TSP", "[%s]\n", __FUNCTION__));

    if(_ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable == FALSE)
    {
        return TRUE;
    }

    for(u32ii = 0; u32ii < TSP_PIDFLT1_NUM; u32ii++)
    {
        u32Dmxid = u32ii+TSP_SECFLT_NUM;
        HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(0, u32Dmxid), DRVTSP_PID_NULL);
        if(_TSP_PidFlt_Free(0, u32Dmxid, TRUE) != DRVTSP_OK)
        {
            Res = DRVTSP_FAIL;
        }
        _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] = DRVTSP_PID_NULL;
    }

    for(u32ii = 0; u32ii < DSCMB_FLT_NUM_ALL; u32ii++)
    {
        _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii] = DRVTSP_PID_NULL;
    }

    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR = 0;
    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef = 0;

    _ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable = FALSE;

    return (Res == DRVTSP_OK);
}

#endif //udef NO_PVR


#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
static MS_BOOL _TSP_PCR_Adjust(MS_U32 u32EngId)
{
    MS_U32 u32Pcr = 0, u32Pcr_H = 0, u32Diff = 0, u32STC = 0;

    HAL_TSP_HWPcr_Read(u32EngId, &u32Pcr, &u32Pcr_H);

    if(_u8ResetPcr[u32EngId] >= 2)
    {
        if(_u8ResetPcr[u32EngId] == 3)
        {
            HAL_TSP_Stc_ctrl(u32EngId, STC_SYNTH_DEFAULT);
        }
        HAL_TSP_SetSTC(u32EngId, u32Pcr, u32Pcr_H);
        _u8ResetPcr[u32EngId] = 0;
        _u32PrevStcBase[u32EngId] = u32Pcr;
        _u32PrevPllControl[u32EngId] = HAL_TSP_GetSTCSynth(u32EngId);
    }

    HAL_TSP_STC_Update_Disable(u32EngId, TRUE);
    u32STC = HAL_TSP_GetSTC(u32EngId);
    HAL_TSP_STC_Update_Disable(u32EngId, FALSE);

    //ULOGD("TSP", "Pcr %08lx, STC %08lx \n", u32Pcr, u32STC);
    if((u32STC - _u32PrevStcBase[u32EngId]) > 0x4000)
    {
        //ULOGD("TSP", "STC_DIFF = %08lx ==== \n\n",u32STC -_u32PrevStcBase[u32EngId]);
        _u32PrevStcBase[u32EngId] = u32STC;

        if(_u32PrevStcBase[u32EngId] > u32Pcr)
        {
            u32Diff = _u32PrevStcBase[u32EngId] - u32Pcr;
            if( u32Diff  <= 0x02)
            {
                _u8ResetPcr[u32EngId] = 0;
            }
            else if( u32Diff  < 0x1000)
            {
                _u32PrevPllControl[u32EngId] += ((u32Diff) >> 3 );                      //now = error x alpha + prev
                HAL_TSP_Stc_ctrl(u32EngId, _u32PrevPllControl[u32EngId] + ((u32Diff)>>6));        //out = now + error x beta
                _u8ResetPcr[u32EngId] = 0;
                //ULOGD("TSP", "======== Set STC CONTROL <++> %08lx  PCR/Diff = %08lx/%08lx \n",
                //    _u32PrevPllControl[u32EngId] + ((_u32PrevStcBase[u32EngId] - u32Pcr)>>6),u32Pcr,_u32PrevStcBase[u32EngId]-u32Pcr);
            }
            else
            {
                _u8ResetPcr[u32EngId]++;
            }
        }
        else
        {
            u32Diff = u32Pcr - _u32PrevStcBase[u32EngId] ;
            if(( u32Diff) <= 0x02)
            {
                _u8ResetPcr[u32EngId] = 0;
            }
            else if((u32Diff) < 0x1000)
            {
                _u32PrevPllControl[u32EngId] -= ((u32Diff)>> 3);                //now = error x alpha + prev
                HAL_TSP_Stc_ctrl(u32EngId, _u32PrevPllControl[u32EngId] - ((u32Diff)>> 6));        //out = now + error x beta
                _u8ResetPcr[u32EngId] = 0;
                //ULOGD("TSP", "======== Set STC CONTROL <--> %08lx  PCR/Diff = %08lx/%08lx \n",
                //    _u32PrevPllControl[u32EngId] - ((u32Pcr - _u32PrevStcBase[u32EngId] )>>6),u32Pcr,u32Pcr-_u32PrevStcBase[u32EngId] );
            }
            else
            {
                _u8ResetPcr[u32EngId]++;
            }
        }
    }

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
#ifdef TS2_IF_SUPPORT
    MS_U32                 u32HwInt2;
#endif

    _ptsp_res->_u32DbgIntrCount++;
    //HAL_TSP_ISR_SAVE_ALL();

    _TSP_INT_LOCK();
    u32HwInt = HAL_TSP_HW_INT_STATUS();
    u32SwInt = HAL_TSP_SW_INT_STATUS();

#ifdef TS2_IF_SUPPORT
    u32HwInt2 = HAL_TSP_HW_INT2_STATUS();
#endif

    _TSP_INT_UNLOCK();

    //ULOGD("TSP", "u32HwInt %lx, u32SwInt %lx\n", u32HwInt, u32SwInt);

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    if (u32HwInt2 & TSP_HWINT2_PCR_UPDATE_END)
    {
        if(_TSP_ChkHwPcrNotifyId(0)
        {
            _TSP_PCR_Adjust(0);
        }
        else
        {
            u32HwInt2 &= ~TSP_HWINT2_PCR_UPDATE_END;
        }
    }
#endif

#ifndef NO_PVR
    if (HAS_FLAG(u32HwInt, TSP_HWINT_HW_PVR_MASK))
    {
        if(_TSP_ChkPvrNotifyId(0))
        {
            _TSP_SetEvent(HAS_FLAG(u32HwInt, TSP_HWINT_HW_PVR_MASK)); // Trigger task
        }
        else
        {
            u32HwInt &= ~TSP_HWINT_HW_PVR_MASK;
        }
    }
#endif //udef NO_PVR

#ifdef TS2_IF_SUPPORT
    if (HAS_FLAG(u32HwInt2, TSP_HWINT2_HW_PVR1_MASK))
    {
        if(_TSP_ChkPvrNotifyId(1))
        {
            _TSP_SetEvent(HAS_FLAG(u32HwInt2, TSP_HWINT2_HW_PVR1_MASK)); // Trigger task
        }
        else
        {
            u32HwInt2 &= ~TSP_HWINT2_HW_PVR1_MASK;
        }
    }
#endif

    if (u32SwInt & TSP_SWINT_CTRL_FIRE)
    {
        MS_U32          u32EngId;
        //MS_U32          u32PidFltId;
        MS_U32          u32SecFltId;
        MS_U32          u32IntStat;
        //REG_SecFlt*     pSecFilter;

        u32EngId    = (u32SwInt & TSP_SWINT_INFO_ENG_MASK) >> TSP_SWINT_INFO_ENG_SHFT;
        u32IntStat  = (u32SwInt & TSP_SWINT_STATUS_CMD_MASK) >> TSP_SWINT_STATUS_CMD_SHFT;
        u32SecFltId = (u32SwInt & TSP_SWINT_INFO_SEC_MASK) >> TSP_SWINT_INFO_SEC_SHFT;
        //pSecFilter  = &(_TspSec[u32EngId].Flt[u32SecFltId]);

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

        switch (u32IntStat)
        {
#ifndef NO_FILEIN
        case TSP_SWINT_CMD_DMA_PAUSE:
            HAL_TSP_TsDma_Pause();
            break;

        case TSP_SWINT_CMD_DMA_RESUME:
            HAL_TSP_TsDma_Resume();
            break;
#endif //udef NO_FILEIN
        case TSP_SWINT_STATUS_SEC_RDY:
        case TSP_SWINT_STATUS_REQ_RDY:
        case TSP_SWINT_STATUS_SEC_CRCERR:
            //ULOGD("TSP", "SEC %ld\n", u32SecFltId);
            if (TSP_SWINT_STATUS_SEC_CRCERR == u32IntStat && (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId] & E_DRVTSP_FLT_MODE_CRCCHK))
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
                _TSP_SetEvent(TSP_TASK_EVENT_SECTION_POLL);                      // Trigger task
            }
            else if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_SELF)
            {
                _TSP_SetEvent(TSP_TASK_EVENT_SECTION_SELF);                      // Trigger task
#ifdef MSOS_TYPE_LINUX_KERNEL
                wake_up_interruptible(&(tsp_wq_head[u32SecFltId]));
#endif
            }
            break;

        case TSP_SWINT_STATUS_BUF_OVFLOW:
            //ULOGD("TSP", "OVF %ld\n", u32SecFltId);
            _TSP_SetSecOvfId(u32SecFltId);             // Set section event flag
            if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_POLL)
            {
                _TSP_SetEvent(TSP_TASK_EVENT_SECTION_POLL);                      // Trigger task
            }
            else if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId] & E_DRVTSP_EVENT_CB_MASK) == E_DRVTSP_EVENT_CB_SELF)
            {
                _TSP_SetEvent(TSP_TASK_EVENT_SECTION_SELF);                      // Trigger task
#ifdef MSOS_TYPE_LINUX_KERNEL
                wake_up_interruptible(&(tsp_wq_head[u32SecFltId]));
#endif

            }
            break;

        case TSP_SWINT_STATUS_PKT_OVRUN:
            //[TODO] Enable debug message when PCR filter can be handle by hardware.
//            TSP_DBG("Packet Overrun %d\n", u32EngId);
            break;

        case TSP_SWINT_STATUS_SEC_ERROR:
            //[ISR print]
//            TSP_DBG("SEC error %d-0x%02X\n", u32EngId, u32SecFltId);
            break;

        case TSP_SWINT_STATUS_DEBUG:
            //[ISR print]
//            TSP_DBG("DEBUG %d-0x%02X\n", u32EngId, u32SecFltId);
            if(_FwDbgInfo.bEnable)
            {
                //@TODO : after init/exit tsp drvier couple times, event _s32TspEventId creating will fail
                _TSP_SetEvent(TSP_TASK_EVENT_FWMSG);
#ifdef MSOS_TYPE_LINUX_KERNEL
                wake_up_interruptible(&(tsp_wq_head[u32SecFltId]));
#endif

            }
            break;

        default:
            //[ISR print]
            //DBG_TSP(TSP_DBG("Unknown TSP interrupt %d\n", u32EngId));
            break;
        }
    }

CLEAR_INT:

    //If it still has interrupt status, it should be processed by another process again, do not clear interrupt status

    _TSP_INT_LOCK();

    if(u32SwInt)
    {
        HAL_TSP_Int_ClearSw();
    }

    if(u32HwInt)
    {
        HAL_TSP_Int_ClearHw(u32HwInt);
    }
#ifdef TS2_IF_SUPPORT
    if(u32HwInt2)
    {
        HAL_TSP_Int_ClearHw2(u32HwInt2);
    }
#endif

    _TSP_INT_UNLOCK();

    //HAL_TSP_ISR_RESTORE_ALL();
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
    P_DrvTSP_EvtCallback pfEvtCallback = NULL;
    static DrvTSP_Msg msg;
    MS_PHY phyRead;
    MS_U32 u32PIDFlt;
    REG_SecFlt *pSecFilter = _TSP_GET_SECFLT_REG(u32FltId);

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        _TSP_GetEvent(u32Events, TSP_TASK_EVENT_SECTION_POLL, OSAL_EVENT_OR_CLEAR, 0);
    }

    _TSP_IND_LOCK();

    //using section filter ID to find the mapping pid filter id
    for(u32PIDFlt = 0; u32PIDFlt < TSP_PIDFLT_NUM; u32PIDFlt++)
    {
        if(_ptsp_res->_TspState[u32EngId].FltMap[u32PIDFlt] == u32FltId)
        {
            break;
        }
    }

    if (TSP_PIDFLT_NUM == u32PIDFlt)
        goto PRCSEC_END;

    if (!(_ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] & E_DRVTSP_FLT_STATE_ENABLE))
    {
        // TSP_DBG("[%s][%d] %d 0x%08x\n", __FUNCTION__, __LINE__,
        //(int)u32FltId, (int)_ptsp_res->_TspState[u32EngId].FltState[u32FltId]);
        goto PRCSEC_END;
    }

    if (_TSP_GetFltDisableId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR))
    {
        //No need to disable filter here or Overflow callback will miss.
        _TSP_PidFlt_Disable(u32EngId, u32PIDFlt);  // critical section
        //_TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
        //_TSP_GetSecOvfId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
    }
    phyRead = HAL_TSP_SecBuf_GetBufRead(pSecFilter);
    bData = (phyRead != HAL_TSP_SecBuf_GetBufWrite(pSecFilter))? TRUE: FALSE; // critical section

    if (bData)
    {
        if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
        {
            *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_READY;
        }
        else
        {
            //_TSP_SECFLT_LOCK(u32EngId, u32FltId);
            u32EvtNotify  = _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32FltId];
            pfEvtCallback = _ptsp_res->_TspState[u32EngId].SecFltCallback[u32FltId];
            if ((u32EvtNotify & E_DRVTSP_EVENT_DATA_READY) && pfEvtCallback)
            {
                if(_ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] & E_DRVTSP_FLT_STATE_FREEING)
                {
                    //ULOGD("TSP", "[%s][%d]Warning!! Filter %u is closing %x\n", __FUNCTION__, __LINE__, (unsigned int)u32PIDFlt, (unsigned int)stDrvState);
                    _TSP_IND_UNLOCK();
                    return FALSE;
                }
                else
                {
                    _ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] |= E_DRVTSP_FLT_STATE_CBRUN;
                }

                msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32FltId <<MSG_FLTINFO_SEC_ID_SHFT);
                _u32TaskLine = __LINE__;
                //_TSP_SECFLT_UNLOCK(u32EngId, u32FltId);
                _TSP_IND_UNLOCK();
                pfEvtCallback(E_DRVTSP_EVENT_DATA_READY, &msg);
                _u32TaskLine = __LINE__;

                _TSP_IND_LOCK();
                _ptsp_res->_TspState[u32EngId].FltState[u32PIDFlt] &= ~E_DRVTSP_FLT_STATE_CBRUN;
                if (phyRead == HAL_TSP_SecBuf_GetBufRead(pSecFilter))
                {
                    _TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
                }
            }
        }
    }
    else
    {
        //_TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
    }

    //Clear evets,
    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        _TSP_GetSecRdyId(u32FltId, &u32Event, OSAL_EVENT_OR_CLEAR);
    }

PRCSEC_END:

    _TSP_IND_UNLOCK();

    if (bData)
    {
        return TRUE;
    }

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
        _TSP_GetEvent(u32Events, TSP_TASK_EVENT_SECTION_POLL, OSAL_EVENT_OR_CLEAR, 0);
    }

    //using section filter ID to find the mapping pid filter id
    for(u32PIDFlt = 0; u32PIDFlt < TSP_PIDFLT_NUM; u32PIDFlt++)
    {
        if(_ptsp_res->_TspState[u32EngId].FltMap[u32PIDFlt] == u32FltId)
            break;
    }
    if(u32PIDFlt == TSP_PIDFLT_NUM)
    {
        return FALSE;
    }

    if ((_ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32FltId] & E_DRVTSP_EVENT_SEC_CRCERR) == 0)
    {
        return FALSE;
    }

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        *pu32Evt = (MS_U32)E_DRVTSP_EVENT_SEC_CRCERR;
    }
    else
    {
        //_TSP_SECFLT_LOCK(u32EngId, u32SecFltId);
        _TSP_IND_LOCK();
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
        }
        else
        {
            _TSP_IND_UNLOCK();
        }
    }

    return TRUE;
}

#ifndef NO_PVR
static void _TSP_Isr_Proc_Pvr(MS_U32 u32CbOpt)
{
    MS_U32 u32Events = 0;
    static DrvTSP_Msg   msg;

    if (((_ptsp_res->_PvrEvtNotify[0] & E_DRVTSP_EVENT_CB_MASK) != u32CbOpt)
    #ifdef TS2_IF_SUPPORT
        && ((_ptsp_res->_PvrEvtNotify[1] & E_DRVTSP_EVENT_CB_MASK) != u32CbOpt)
    #endif
    )
    {
        return;
    }

    if (u32CbOpt == E_DRVTSP_EVENT_CB_POLL)
    {
        _TSP_GetEvent(u32Events, TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL |
            TSP_TASK_EVENT_CBPVR0_RDY_POLL, OSAL_EVENT_OR_CLEAR, 0);
    }
    else
    {
        _TSP_GetEvent(u32Events, TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF |
            TSP_TASK_EVENT_CBPVR0_RDY_SELF, OSAL_EVENT_OR_CLEAR, 0);
    }

    if (_ptsp_res->_PvrEvtNotify[0] && _ptsp_res->_PvrCallback[0])
    {
        switch (u32Events & (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL | TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF))
        {
            case TSP_TASK_EVENT_PVR0_RDY_SELF:
            case TSP_TASK_EVENT_PVR0_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x0 << MSG_PVRBUF_ID_SHFT);
                    _ptsp_res->_PvrCallback[0](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                break;
            case TSP_TASK_EVENT_PVR1_RDY_SELF:
            case TSP_TASK_EVENT_PVR1_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                {
                    msg.PvrBufId = (0x1 << MSG_PVRBUF_ID_SHFT);
                    _ptsp_res->_PvrCallback[0](E_DRVTSP_EVENT_PVRBUF_FULL, &msg);
                }
                break;
            case (TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF):
            case (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL):
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                {
                    msg.PvrBufId = (MSG_PVRBUF_ID_NULL << MSG_PVRBUF_ID_SHFT);
                    _ptsp_res->_PvrCallback[0](E_DRVTSP_EVENT_PVRBUF_OVERFLOW, &msg);
                }
                break;
            default:
                break;
        }
    }
#ifdef TS2_IF_SUPPORT
    if (_ptsp_res->_PvrEvtNotify[1] && _ptsp_res->_PvrCallback[1])
    {
        switch (u32Events & (TSP_TASK_EVENT_CBPVR0_RDY_POLL | TSP_TASK_EVENT_CBPVR0_RDY_SELF))
        {
            case TSP_TASK_EVENT_CBPVR0_RDY_POLL:
                if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_CBBUF_FULL))
                {
                    msg.Pvr1BufId = (0x0 << MSG_PVRBUF_ID_SHFT);
                    _ptsp_res->_PvrCallback[1](E_DRVTSP_EVENT_CBBUF_FULL, &msg);
                }
                break;
        }
    }
#endif
}
#endif //udef NO_PVR

static MS_BOOL _TSP_Isr_Proc_SecOvf(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32CbOpt, MS_U32* pu32Evt)
{
    static DrvTSP_Msg       msg;
    MS_U32                  u32EvtNotify, u32Events = 0;
    P_DrvTSP_EvtCallback    pfEvtCallback = NULL;
    MS_U32 u32PidFltId;
    MS_BOOL                 bDisabled = FALSE;

    *pu32Evt = (MS_U32)E_DRVTSP_EVENT_DATA_INIT;

    _TSP_IND_LOCK();
    for (u32PidFltId = 0; u32PidFltId < TSP_PIDFLT_NUM; u32PidFltId++)
    {
        if(_ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] == 0xFF)
            continue;
        if (_ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] == u32SecFltId)
        {
            if(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] & E_DRVTSP_FLT_STATE_ENABLE)
            {
                _TSP_PidFlt_Disable(u32EngId, u32PidFltId);  // ???? move out --> OK  // // critical section
                bDisabled = TRUE;
            }
            break;
        }
    }

    if(u32PidFltId == TSP_PIDFLT_NUM)
    {
        _TSP_IND_UNLOCK();
        return TRUE;
    }

    if (E_DRVTSP_EVENT_CB_POLL == u32CbOpt)
    {
        *pu32Evt = (MS_U32)E_DRVTSP_EVENT_BUF_OVERFLOW;

        //----  if overflow, clear evets to wait restarting--------
        _TSP_GetSecOvfId    (u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecCrcId    (u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
        _TSP_GetSecRdyId    (u32SecFltId, &u32Events, OSAL_EVENT_OR_CLEAR);
        //-------------------------------------------------------------------------
    }
    else
    {
        //_TSP_SECFLT_LOCK(u32EngId, u32SecFltId);
        u32EvtNotify  = _ptsp_res->_TspState[u32EngId].SecFltEvtNotify[u32SecFltId];
        pfEvtCallback = _ptsp_res->_TspState[u32EngId].SecFltCallback[u32SecFltId];
        if (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] == E_DRVTSP_FLT_STATE_FREE && (bDisabled == FALSE))
        {
            goto PROC_END;
        }

        if ((u32EvtNotify & E_DRVTSP_EVENT_BUF_OVERFLOW) && pfEvtCallback)
        {
            if(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] & E_DRVTSP_FLT_STATE_FREEING)
            {
                //ULOGD("TSP", "[%s][%d]Warning!! Filter %u is closing %x\n", __FUNCTION__, __LINE__, (unsigned int)u32PidFltId, (unsigned int)stDrvState);
                goto PROC_END;
            }
            else
            {
                _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] |= E_DRVTSP_FLT_STATE_CBRUN;
            }
            _TSP_IND_UNLOCK();
            msg.FltInfo = (u32EngId <<MSG_FLTINFO_ENG_ID_SHFT) | (u32SecFltId <<MSG_FLTINFO_SEC_ID_SHFT);
            _u32TaskLine = __LINE__;
            pfEvtCallback(E_DRVTSP_EVENT_BUF_OVERFLOW, &msg);
            _u32TaskLine = __LINE__;

            _TSP_IND_LOCK();
            _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] &= ~E_DRVTSP_FLT_STATE_CBRUN;
        }
    }

PROC_END:

    _TSP_IND_UNLOCK();
    return TRUE;
}

#ifdef MSOS_TYPE_LINUX_KERNEL
static void _TSP_Isr_ProcSelf(MS_U32 u32PrcEvt, TSP_Result* pRet)
{
#else
static void _TSP_Isr_ProcSelf(OSAL_TSP_ProcTypeParam* pData)
{
    TSP_Result*         pRet = (TSP_Result*)pData->pRet;
#endif
    MS_U32              u32Events = 0, u32SecRdy = 0, u32SecOvf = 0, u32SecCrc = 0;;//, u32SecFree = 0;
    MS_U32              i = 0, j = 0;
    MS_U32              u32Evt = 0;
    //MS_U32              u32PidFltId;
    //MS_U32              u32Mask, u32AskFreeFltList = 0;
    //MS_U32              u32GroupNum = (TSP_PIDFLT_NUM_ALL+31)>>5;
    MS_U32              u32SecFltId = 0;
    //MS_U32              u32TempEvent = 0;

    #if 0
    _TSP_GetEvent(u32TempEvent, TSP_TASK_EVENT_FWMSG, OSAL_EVENT_OR_CLEAR, 0);
    if(u32TempEvent & (TSP_TASK_EVENT_FWMSG))
    {
        _DumpFwMsg();
    }
    #endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    u32Events = u32PrcEvt;
#else
    _TSP_GetEvent(u32Events, TSP_TASK_EVENT_GROUP_SELF | TSP_TASK_EVENT_FLT_FREE, OSAL_EVENT_OR_CLEAR, MSOS_WAIT_FOREVER);
    *pRet = DRVTSP_FAIL;
#endif

    //ULOGD("TSP", "TSP Evt %lx %ld\n", u32Events, MsOS_GetSystemTime());

    _u32TaskLine = __LINE__;

#ifndef NO_PVR
    if (u32Events & (TSP_TASK_EVENT_PVR0_RDY_POLL|TSP_TASK_EVENT_PVR1_RDY_POLL))
    {
        _TSP_Isr_Proc_Pvr(E_DRVTSP_EVENT_CB_SELF);
    }
#endif //udef NO_PVR
    if (u32Events & TSP_TASK_EVENT_SECTION_SELF)
    {
        for (i = 0; i < TSP_ENGINE_NUM; i++)
        {
            for (j = 0; j < ((TSP_SECFLT_NUM_All+31)>>5); j++)
            {
                _TSP_GetModeSecOvf(j, 0xFFFFFFFF, &u32SecOvf, OSAL_EVENT_OR_CLEAR);
                _TSP_GetModeSecRdy(j, 0xFFFFFFFF, &u32SecRdy, OSAL_EVENT_OR_CLEAR);
                _TSP_GetModeSecCrc(j, 0xFFFFFFFF, &u32SecCrc, OSAL_EVENT_OR_CLEAR);
                while (u32SecRdy)
                {
                    u32SecFltId = MAsm_CPU_GetTrailOne(u32SecRdy) + (j << 5);  //[TODO] balance every filter priority
                                                                //[NOTE] it may be already balance for long term running
                    *pRet = (_TSP_Isr_Proc_SecRdyCrc(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                    u32SecRdy &= ~(1 << (u32SecFltId & 0x1F));
                }

                while (u32SecCrc)
                {
                    u32SecFltId = MAsm_CPU_GetTrailOne(u32SecCrc) + (j << 5);  //[TODO] balance every filter priority
                                                                //[NOTE] it may be already balance for long term running
                    *pRet = (_TSP_Isr_Proc_SecCrcErr(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                    u32SecCrc &= ~(1 << (u32SecFltId & 0x1F));
                }

                while(u32SecOvf)
                {
                    u32SecFltId = MAsm_CPU_GetTrailOne(u32SecOvf) + (j << 5);  //[TODO] balance every filter priority
                                                                //[NOTE] it may be already balance for long term running
                    *pRet = (_TSP_Isr_Proc_SecOvf(i, u32SecFltId, E_DRVTSP_EVENT_CB_SELF, &u32Evt))? DRVTSP_OK: DRVTSP_FAIL;
                    u32SecOvf &= ~(1 << (u32SecFltId & 0x1F));
                }
            }
        }
    }

    #if 0
    if (u32Events & TSP_TASK_EVENT_FLT_FREE)
    {
        for (j = 0; j < u32GroupNum; j++)
        {
            u32Mask = u32AskFreeFltList = _TSP_GetAskPidFltFree0List(j);

            if (u32Mask)
            {
                _TSP_IND_LOCK();
            }

            while (u32AskFreeFltList)
            {
                u32PidFltId = MAsm_CPU_GetTrailOne(u32AskFreeFltList) + (j << 5);

                if (_ptsp_res->_TspState[0].FltState[u32PidFltId] != E_DRVTSP_FLT_STATE_FREE)
                {
                    u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(HAL_TSP_GetPidFltReg(0, u32PidFltId));
                    _TSP_GetSecRdyId(u32SecFltId, &u32SecRdy, OSAL_EVENT_OR_CLEAR);
                    _TSP_GetSecOvfId(u32SecFltId, &u32SecOvf, OSAL_EVENT_OR_CLEAR);
                    _TSP_SecBuf_Free(0, u32SecFltId);
                    _TSP_SecFlt_Free(0, u32SecFltId, TRUE);
                    _TSP_PidFlt_Free(0, u32PidFltId, FALSE);
                }
                _TSP_FLAG_SetDone(u32PidFltId);
                u32AskFreeFltList &= (~(1<<(u32PidFltId & 0x1F)));
            }
            if (u32Mask)
            {
                _TSP_ClrAskPidFltFree0List(j, u32Mask);
                _TSP_IND_UNLOCK();
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

    _TSP_GetEvent(u32FwEvent, TSP_TASK_EVENT_FWMSG, OSAL_EVENT_OR_CLEAR, 0);
    if(u32FwEvent & (TSP_TASK_EVENT_FWMSG))
    {
        _DumpFwMsg();
    }

    _TSP_GetEvent(u32Events, TSP_TASK_EVENT_GROUP_POLL, OSAL_EVENT_OR, 0);

    *pRet = DRVTSP_FAIL;

#ifndef NO_PVR
    if ((u32Events & (TSP_TASK_EVENT_PVR0_RDY_POLL|TSP_TASK_EVENT_PVR1_RDY_POLL|TSP_TASK_EVENT_CBPVR0_RDY_POLL))
        && (u32FltId >= TSP_SECFLT_NUM) && (u32FltId < (TSP_SECFLT_NUM+ TSP_PIDFLT1_NUM)))
    {
        _TSP_Isr_Proc_Pvr(E_DRVTSP_EVENT_CB_POLL);
        return;
    }
#endif //udef NO_PVR

    for (i = 0; i < TSP_ENGINE_NUM; i++)
    {
        MS_U32                  u32SecFltId;

        REG_PidFlt *pPidFilter = HAL_TSP_GetPidFltReg(i, u32FltId);
        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(pPidFilter);

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

#ifndef MSOS_TYPE_LINUX_KERNEL
static void _TSP_Isr_Proc_Task(OSAL_TSP_ProcTypeParam* pDummy, MS_U32* pu32Dummy)
{
    TSP_Result          Ret;
    OSAL_TSP_ProcTypeParam Data;

    Data.u32FltId = 0xFFFFFFFF;
    Data.pRet = (void*)&Ret;
    Data.bInfiniteWait = TRUE;
    Data.bFromTask = TRUE;

     while (1)
    {
        _TSP_EXT_LOCK();
        if (_db_flag == 1)
        {
            _db_flag = 2;
            _TSP_EXT_UNLOCK();
            break;
        }
        _TSP_EXT_UNLOCK();
        _TSP_Isr_ProcSelf(&Data);
    } // Task while loop

    _s32TspTaskId = -1;
    TSP_DBG("TSP ISR Proc End!!!\n");

}
#endif

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
//static char freelistdefer0[] = "TSP_FreeListDefer0";
//static char tsp_done[] = "TSP_Done";
#ifndef MSOS_TYPE_LINUX_KERNEL
static char tsp_isr_task[] = "TSP_ISR_Task";
#endif

static TSP_Result _TSP_Init(MS_BOOL bResetSwState, MS_BOOL bRestoreRegState)
{
    MS_VIRT virtTspBank,virtIRBank;
    MS_PHY phyTspBankSize,phyIrBankSize;
    MS_BOOL bAlive;
    MS_U32 u32FwSize = 0;
    MS_PHY phyFwAddr = 0;
    MS_U8 u8ii;

    if (FALSE == MDrv_MMIO_GetBASE(&virtTspBank, &phyTspBankSize, MS_MODULE_TSP))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
        MS_ASSERT(0);
    }
    if (FALSE == MDrv_MMIO_GetBASE(&virtIRBank, &phyIrBankSize, MS_MODULE_IR))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE (PM base)fail\n"));
        MS_ASSERT(0);
    }

    HAL_TSP_SetBank(virtTspBank, virtIRBank);

#ifdef STC64_SUPPORT
    HAL_TSP_STC64_Mode_En(TRUE);
#endif

    bAlive = HAL_TSP_Alive();
    if (!bAlive)
    {
        HAL_TSP_PowerCtrl(ENABLE);
        HAL_TSP_Reset(0);
        // Diable interrupt
        HAL_TSP_Int_Disable(TSP_HWINT_ALL);
#ifdef TS2_IF_SUPPORT
        HAL_TSP_Int2_Disable(TSP_HWINT2_STATUS_MASK);
#endif
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

        // Richard: Enable indirect access
        HAL_TSP_Ind_Enable();
        HAL_TSP_HW_Lock_Init(); //Note: SW_Reset must be free first, then XIU can be written
#ifndef NO_FILEIN
        _TSP_CmdQ_Init();
#endif
        HAL_TSP_WbDmaEnable(TRUE);

#ifndef NO_FILEIN
        HAL_TSP_Scmb_Detect(TRUE);
#endif
        HAL_TSP_DoubleBuf_Disable();

        // Mark : Byte enable setting, fix HW bug. (Important)
        HAL_TSP_HwPatch();
    }

    if (bResetSwState)
    {
        _TSP_SwStateReset(FALSE);
    }

    if (!bAlive)
    {
        //if(bRestoreRegState == 0)
        {
            _TSP_RegStateReset();
        }

        if (_TSP_GetFirmware(&phyFwAddr, &u32FwSize) == FALSE)
        {
            MS_CRITICAL_MSG(TSP_DBG ("TSP init fail: Please set FW address and size\n"));
            return DRVTSP_FAIL;
        }

        //TSP_DBG("[%s][%d]Reset CMDQ\n",__FUNCTION__,__LINE__);
#ifndef NO_FILEIN
        if(!HAL_TSP_CmdQ_Reset())
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

        //TSP_DBG ("**************************DDD TSP protect miu00000000000000000\n");
        HAL_TSP_OrzWriteProtect_Enable(TRUE, 0, 0);

        HAL_Reset_WB();

        HAL_TSP_SetCtrlMode(0, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU

        HAL_TSP_Int_ClearSw();
        HAL_TSP_Int_ClearHw(TSP_HWINT_ALL);
#ifdef  TS2_IF_SUPPORT
        HAL_TSP_Int_ClearHw2(TSP_HWINT2_STATUS_MASK);
#endif

        while (FALSE == HAL_TSP_Alive())
        {
            MS_CRITICAL_MSG(TSP_DBG("Reset TSP CPU fail\n"));
            HAL_TSP_SetCtrlMode(0, 0, TSP_IF_NUM);
            HAL_TSP_CPU_SetBase(phyFwAddr, u32FwSize);
            HAL_TSP_SetCtrlMode(0, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);// | TSP_CTRL_CLK_GATING_DISABLE); // Enable TSP CPU
        }

        HAL_TSP_Check_FW_VER();

        HAL_TSP_Int_ClearSw();
        HAL_TSP_Int_ClearHw(TSP_HWINT_ALL);
#ifdef TS2_IF_SUPPORT
        HAL_TSP_Int_ClearHw2(TSP_HWINT2_STATUS_MASK);
#endif

        // Implicity MDrv_TSP_Reset()
#ifndef NO_FILEIN
        HAL_TSP_TsDma_SetDelay(0x0A);                                          // Set TsDma delay, //jyliu.tsp, how come the value?
#endif

        _FwDbgInfo.bEnable      = FALSE;
        _FwDbgInfo.phyBufAddr   = 0x0;
        _FwDbgInfo.u32Size      = 0x0;
        _FwDbgInfo.u32ReadPos   = 0x0;
        _FwDbgInfo.u32WritePos  = 0x0;

    }

    // STC control by TSP need indirect R/W enable
    for(u8ii = 0; u8ii < STC_SYNTH_NUM; u8ii++)
    {
        HAL_TSP_Stc_ctrl(u8ii, STC_SYNTH_DEFAULT);
    }

    // Enable SYNC_RISING_DETECT and VALID_FALLING_DETECT for Serial mode
    HAL_TSP_Enable_ValidSync_Dectect();

    // Disable TSIF1
#ifndef NO_PVR
    //HAL_TSP_PVR_Enable(FALSE);
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
    for (u8ii= 0; u8ii< TSP_SECFLT_NUM_All; u8ii++)
    {
        init_waitqueue_head(&(tsp_wq_head[u8ii]));
    }
#endif

    OSAL_TSP_IntAttach(_TSP_Isr);
//#ifndef MSOS_TYPE_LINUX_KERNEL
    OSAL_TSP_IntEnable();
//#endif

    if (!bAlive)
    {
        // [URANUS] Richard: Refine the interrupt selection later.
        HAL_TSP_Int_Enable(TSP_HWINT_TSP_SW_INT_STATUS);        // Enable TSP hardware interrupt
    }

#ifndef NO_PVR
    for (u8ii = 0; u8ii < TSP_PVR_IF_NUM; u8ii++)
    {
        _ptsp_res->_PvrEvtNotify[u8ii] = E_DRVTSP_EVENT_DATA_INIT;
        _ptsp_res->_PvrCallback[u8ii] = NULL;
    }
    for (u8ii = 0; u8ii < ((TSP_PVR_IF_NUM+31)>>5); u8ii++)
    {
        _u32PvrNotifyId[u8ii] = 0;
    }
#endif

    _ptsp_res->_s32TspMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_mutex); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _ptsp_res->_s32IndMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_ind_mutex); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _ptsp_res->_s32IntRegMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_int_mutex);
    _s32ExtMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_ext_mutex);
    _s32TspEventId = OSAL_TSP_EventCreate((MS_U8*)tsp_event); // MsOS_CreateEventGroup("TSP_Event");

    for (u8ii = 0; u8ii < (MS_U8)((TSP_SECFLT_NUM_All+31)>>5); u8ii++)
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

    #if 0
    for (u8ii = 0; u8ii < ((TSP_PIDFLT_NUM_ALL+31)>>5); u8ii++)
    {
        _s32EventFree0[u8ii] = OSAL_TSP_EventCreate((MS_U8*)freelist0);
        _s32EventFreeDefer0[u8ii] = OSAL_TSP_EventCreate((MS_U8*)freelistdefer0);
        _s32DoneFlags[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_done);
    }
    #endif

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    if(bResetSwState)
    {
        for (u8ii = 0; u8ii < (MS_U8)((TSP_PCRFLT_NUM+31)>>5); u8ii++)
        {
            _u32HwPcrNotifyId[u8ii] = 0;
        }
    }
#endif

    #ifndef MSOS_TYPE_LINUX_KERNEL
    _s32TspTaskId = OSAL_TSP_TaskCreate(_TSP_Isr_Proc_Task, (MS_U8*)tsp_isr_task);
    #endif

    _ptsp_res->_bSWInitReady = TRUE;
    _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_INIT;
    _ptsp_res->_u32LastErr = DRVTSP_OK;
    _ptsp_res->_u32ProcessNum = 1UL;

    if(bResetSwState)
    {
#ifdef VQ_ENABLE
        _ptsp_res->_u32NULLFileFilterID = 0xFFFFFFFFUL;
#endif
        _ptsp_res->_bLiveSING_FilePP = FALSE;
    }
#ifdef VQ_ENABLE
    else
    {
        _ptsp_res->_u32NULLFileFilterID = 15UL;
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
//  Global Functions
//-------------------------------------------------------------------------------------------------

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

    for (i = 0; i < TSP_ENGINE_NUM; i++)
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
    MS_U16 u16Clk = 0;

    if(!((_ptsp_res->_TspState[0].TSPStatus == E_DRVTSP_SW_INIT) || (_ptsp_res->_TspState[0].TSPStatus == E_DRVTSP_SW_RESUME)))
        return DRVTSP_FAIL;

    MDrv_TSP_GetTSIFStatus(E_DRVTSP_IF_PLAYBACK, &(_ptsp_res->_ifset_save), &u16Clk);

    _TSP_ENTRY();

    _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_SUSPEND;
    _ptsp_res->_bSuspend = TRUE;
    _TSP_RegStateStore();
    HAL_TSP_WbDmaEnable(FALSE);
    HAL_TSP_SetCtrlMode(0, 0, TSP_IF_NUM);
    _TSP_SW_Exit();
    HAL_TSP_HW_Lock_Release();
    if(_ptsp_res->_u32ProcessNum > 0)
    {
         _TSP_RETURN(DRVTSP_OK);
    }
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
    TSP_Result ret;

    // Reset TSP register
    //HAL_ResetAll();
    //MsOS_DelayTask(5);

    _ptsp_res->_bSuspend = FALSE;
    _ptsp_res->_TspState[0].TSPStatus = E_DRVTSP_SW_RESUME;

    _ptsp_res->_phyFwAddr = phyFWAddr;
    _ptsp_res->_u32FwSize = u32FWSize;

    if(_TSP_Init(FALSE, TRUE) != DRVTSP_OK)
        return DRVTSP_FAIL;

    if(MDrv_TSP_SetOperateMode(0, E_DRVTSP_CTRL_MODE_TS0) != DRVTSP_OK)
        return DRVTSP_FAIL;

    _TSP_ENTRY();
    ret = _TSP_SelPad(0, E_DRVTSP_IF_PLAYBACK, &(_ptsp_res->_ifset_save));
    _TSP_RETURN(ret);
}

// Misc API
//-------------------------------------------------------------------------------------------------
/// Set TSP engine operating mode
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eCtrlMode               \b IN: TSP engine operating mode
/// @return TSP_Result
/// @note
/// Only one engine can be set to use memory stream input mode, and all kinds
/// of input are mutually exclusive for each engine.\n
/// Only one engine can output to MAD. The TSP engine which is not selected to
/// MAD can not filter any audio stream, it will block the stream.
/// @sa MDrv_TSP_PidFlt_Alloc, MDrv_TSP_PidFlt_Enable, E_DRVTSP_FLT_TYPE_AUDIO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetOperateMode(MS_U32 u32EngId, DrvTSP_CtrlMode eCtrlMode)      // Set TSP internal operate
{
    MS_U32 u32CaMode = 0,u32ii = 0;
    REG_PidFlt *pPidFilter = NULL;

    PVRCA_DBG(ULOGD("TSP", "[%s][%d] eCtrlMode %ld\n", __FUNCTION__, __LINE__, (MS_U32)eCtrlMode));

    _TSP_ENTRY();

    if (u32EngId >= TSP_ENGINE_NUM)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

// Since VQ enable, using single buffer for live, ping-pong buffer for file as deault path, becase file has no VQ
// If no VQ, using ping-pong buffer for live, single buffer for file as default path.
// when VQ enable, reserve filter 15 for NULL file section PID filter for switch live to single buffer
//-----------------------------------------------------------------------------
// For new VQ architecture with file VQ, not need to create dummy file filter for switch section buffer path
#ifdef VQ_ENABLE
    if((_ptsp_res->_u32NULLFileFilterID == 0xFFFFFFFF) && _ptsp_res->_bLiveSING_FilePP)
    {
        HAL_TSP_DoubleBuf_En(DOUBLE_BUFFER_SWITCH);

        _ptsp_res->_u32NULLFileFilterID = 15;
        pPidFilter = HAL_TSP_GetPidFltReg(0, _ptsp_res->_u32NULLFileFilterID);
        HAL_TSP_PidFlt_SetPid(pPidFilter, DRVTSP_PID_NULL);
        HAL_TSP_PidFlt_SelSecFlt(pPidFilter, _ptsp_res->_u32NULLFileFilterID);
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT | TSP_PIDFLT_OUT_SECFLT_AF);
        HAL_TSP_PidFlt_SelFltSource(pPidFilter, TSP_PIDFLT_IN_FILE);
        _ptsp_res->_TspState[u32EngId].FltMap[_ptsp_res->_u32NULLFileFilterID] = DRVTSP_MAP_NULL;
        _ptsp_res->_TspState[u32EngId].FltState[_ptsp_res->_u32NULLFileFilterID] = E_DRVTSP_FLT_STATE_ALLOC;
        _ptsp_res->_TspState[u32EngId].FltState[_ptsp_res->_u32NULLFileFilterID]|= E_DRVTSP_FLT_STATE_ENABLE;
    }
#endif

    //reset audio BD mode
#ifndef NO_FILEIN
    HAL_TSP_AU_BD_Mode_Enable(FALSE);
#endif

    switch (eCtrlMode)
    {
    case E_DRVTSP_CTRL_MODE_TS0_AUD:
        //HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_TS0:
        HAL_TSP_PS_Path_Disable();
        if(!_ptsp_res->_bLiveSING_FilePP)
        {
            HAL_TSP_DoubleBuf_Disable();
        }
        #ifndef NO_CA

        u32CaMode = HAL_TSP_CSA_Get_ScrmPath();
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] Org u32CaMode %lx\n", __FUNCTION__, __LINE__, u32CaMode));
        if((u32CaMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) != (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) //if TS1 dscmb, not change it...
        {
            if((u32CaMode & TSP_CA_OUTPUT_REC) == 0)
            {
                HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_LIVEIN|TSP_CA_OUTPUT_PLAY_LIVE); //|TSP_CA_AVPAUSE);
                PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode %lx\n", __FUNCTION__, __LINE__, u32CaMode));
            }
        }

        if(_ptsp_res->TSPPvrcaParams._bCIPHSSMode)
        {
            HAL_TSP_TSIF1_Enable(FALSE);
            _TSP_Free_AllFLT1();
            HAL_TSP_LiveAB_En(FALSE);
            _ptsp_res->TSPPvrcaParams._bCIPHSSMode = FALSE;
        }
        #endif

        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);
        break;
    case E_DRVTSP_CTRL_MODE_TS0_NOCA:
        HAL_TSP_PS_Path_Disable();
        if(!_ptsp_res->_bLiveSING_FilePP)
        {
            HAL_TSP_DoubleBuf_Disable();
        }
        u32CaMode = HAL_TSP_CSA_Get_ScrmPath();
        _TSP_FreeDupDscmbPidFlt(u32EngId, 0, u32CaMode);
        #ifdef TS2_LIVE_SUPPORT
        HAL_TSP_CSA_Set_ScrmPath(u32CaMode & ~(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2|TSP_CA_OUTPUT_PLAY_LIVE));
        #else
        HAL_TSP_CSA_Set_ScrmPath(u32CaMode & ~(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE));
        #endif
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);
        break;

#ifndef NO_PVR
    case E_DRVTSP_CTRL_MODE_TS1_OUT_LIVE0:
        _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before switch CA input from TSIF1
        //Free duplicate FLT2
        #ifdef TS2_LIVE_SUPPORT
        if((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2))
            == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2))
        {
            _TSP_FreeDupDscmbPidFlt(u32EngId, 2, _ptsp_res->_u32OrgCAMode);
            HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode & ~TSP_CA_OUTPUT_PLAY_LIVE2);
        }
        #endif

        //duplicateFLT 0 to FLT1
        if((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) != (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE))
        {
            for(u32ii = DSCMB_FLT_START_ID; u32ii <= DSCMB_FLT_END_ID; u32ii++)
            {
                _TSP_DupDscmbPidFlt(u32EngId, u32ii, TSP_SECFLT_NUM+u32ii-DSCMB_FLT_START_ID);
            }
            #ifdef EXT_PVR1_FLT
            for(u32ii = DSCMB_FLT_SHAREKEY_START_ID; u32ii <= DSCMB_FLT_SHAREKEY_END_ID; u32ii++)
            {
                _TSP_DupDscmbPidFlt(u32EngId, u32ii, TSP_SECFLT_NUM+DSCMB_FLT_NUM+u32ii);
            }
            #endif
        }
        if(_ptsp_res->_u32OrgCAMode & TSP_CA_INPUT_TSIF1)
        {
            HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode|TSP_CA_OUTPUT_PLAY_LIVE);
            PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%lx\n", __FUNCTION__, __LINE__, _ptsp_res->_u32OrgCAMode|TSP_CA_OUTPUT_PLAY_LIVE));
        }
        else
        {
            HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE);
            PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%x\n", __FUNCTION__, __LINE__, TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE));
        }
    case E_DRVTSP_CTRL_MODE_TS1_AUD:
        //HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_TS1:
    case E_DRVTSP_CTRL_MODE_TS1_FILEFLT:
        if(eCtrlMode == E_DRVTSP_CTRL_MODE_TS1_FILEFLT)
        {
            HAL_TSP_LiveAB_En(TRUE);
            HAL_TSP_TSIF1_Enable(TRUE);

            #ifndef NO_CA
            _ptsp_res->TSPPvrcaParams._bCIPHSSMode = TRUE;
            _TSP_FLT1_Duplicate_FLT0(0);
            _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before switch CA input from TSIF1
            if(_ptsp_res->TSPPvrcaParams._bPVRCA_Start == TRUE)
            {

                HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_OUTPUT_REC);
                PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%x\n", __FUNCTION__, __LINE__, TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_OUTPUT_REC));
                _TSP_RETURN(DRVTSP_FAIL);
            }
            else
            {
                HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_FILE);
                PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%x\n", __FUNCTION__, __LINE__, TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_FILE));
            }
            #endif

        }
        HAL_TSP_PS_Path_Disable();
        if(!_ptsp_res->_bLiveSING_FilePP)
        {
            HAL_TSP_DoubleBuf_Disable();
        }
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 1);
        break;
#endif

#ifndef NO_FILEIN
    case E_DRVTSP_CTRL_MODE_MEM_AUD:
        //HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_MEM_PVR:
    case E_DRVTSP_CTRL_MODE_MEM:
    case E_DRVTSP_CTRL_MODE_MEM_NOCA:
        HAL_TSP_LiveAB_En(FALSE);
        HAL_TSP_PS_Path_Disable();

        #ifndef FIVQ_ENABLE
        if(!_ptsp_res->_bLiveSING_FilePP)
        {
            HAL_TSP_DoubleBuf_En(DOUBLE_BUFFER_SWITCH);        // [OBERON]:Enable double buffer, set filein->pinpon livein->single
        }
        #endif

#ifndef NO_CA

        _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before switch CA input from File0
        if(eCtrlMode == E_DRVTSP_CTRL_MODE_MEM_PVR)
        {
            HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_FILEIN|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_OUTPUT_REC|TSP_CA_AVPAUSE);
        }
        else if ((eCtrlMode == E_DRVTSP_CTRL_MODE_MEM) && ((_ptsp_res->_u32OrgCAMode & TSP_CA_OUTPUT_REC) == 0))
        {
            HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_FILEIN|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_AVPAUSE);
        }
        else
        {
            //ts2 record ts0, free live0 duplicate
            if(((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) &&
                ((_ptsp_res->_u32OrgCAMode & TSP_CA_OUTPUT_REC) == 0))
            {
                _TSP_FreeDupDscmbPidFlt(u32EngId, 0, _ptsp_res->_u32OrgCAMode);
            }
            #ifdef TS2_LIVE_SUPPORT
            if((_ptsp_res->_u32OrgCAMode & TSP_CA_INPUT_TSIF1) &&
                ((_ptsp_res->_u32OrgCAMode & (TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2|TSP_CA_OUTPUT_REC)) == 0))
            #else
            if((_ptsp_res->_u32OrgCAMode & TSP_CA_INPUT_TSIF1) &&
                ((_ptsp_res->_u32OrgCAMode & (TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_REC)) == 0))
            #endif
            {
                HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode & ~TSP_CA_INPUT_TSIF1);
                PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%lx\n", __FUNCTION__, __LINE__, _ptsp_res->_u32OrgCAMode & ~TSP_CA_INPUT_TSIF1));
            }
            else
            {
                HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode & ~TSP_CA_OUTPUT_PLAY_LIVE);
                PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%lx\n", __FUNCTION__, __LINE__, _ptsp_res->_u32OrgCAMode & ~TSP_CA_OUTPUT_PLAY_LIVE));
            }
        }
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] _ptsp_res->_u32OrgCAMode 0x%lx\n", __FUNCTION__, __LINE__, _ptsp_res->_u32OrgCAMode));

#endif
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST| TSP_CTRL_TSFILE_EN, TSP_IF_NUM);
        break;
#endif //udef   NO_FILEIN

    case E_DRVTSP_CTRL_MODE_VID:
#ifndef NO_CA
        HAL_TSP_CSA_Set_ScrmPath(0);
#endif
        if(!_ptsp_res->_bLiveSING_FilePP)
        {
            HAL_TSP_DoubleBuf_Disable();
        }
        break;

#ifndef NO_PVR
    case E_DRVTSP_CTRL_MODE_PVR_TS0:
        break;
#ifndef NO_CA
    case E_DRVTSP_CTRL_MODE_PVR_TS0_CA:
        _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before set CA ouput from Rec0
        HAL_TSP_CSA_Set_ScrmPath(HAL_TSP_CSA_Get_ScrmPath() | TSP_CA_OUTPUT_REC);
        break;
    case E_DRVTSP_CTRL_MODE_PVR_CA:
        //switch CA form TS0 to TS1
        HAL_TSP_TSIF1_Enable(TRUE);
        _TSP_FLT1_Duplicate_FLT0(0);
        _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before set CA input from TSIF1
        HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_REC/*|TSP_CA_AVPAUSE*/);
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%x\n", __FUNCTION__, __LINE__, TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_REC));

        break;

#endif //udef NO_CA
#endif //udef NO_PVR

#ifdef TS2_LIVE_SUPPORT
    case E_DRVTSP_CTRL_MODE_PVR_TS2_CA:
        _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before set CA input from TSIF1
        if(((_ptsp_res->_u32OrgCAMode & TSP_CA_INPUT_TSIF1) != 0) &&
                 ((_ptsp_res->_u32OrgCAMode & TSP_CA_OUTPUT_REC) != 0))
        {
            HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode|TSP_CA_OUTPUT_PLAY_LIVE2);
            break;
        }
        //Free duplicate FLT0
        #if 0
        if(_TSP_FreeDupDscmbPidFlt(u32EngId, 0, _ptsp_res->_u32OrgCAMode))
        {
            HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode & ~TSP_CA_OUTPUT_PLAY_LIVE);
        }
        #endif
        //duplicateFLT 2 to FLT1
        if((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2))
            != (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2))
        {
            for(u32ii = (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM); u32ii < (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM + DSCMB_FLT_NUM); u32ii++)
            {
                _TSP_DupDscmbPidFlt(u32EngId, u32ii, u32ii-TSP_PIDFLT1_NUM);
            }
            HAL_TSP_TSIF1_Enable(TRUE);
            if((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE))
            {
                HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2);
            }
            else
            {
                HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2);
            }
        }

        PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%lx\n", __FUNCTION__, __LINE__, _ptsp_res->_u32OrgCAMode));
        break;

    case E_DRVTSP_CTRL_MODE_TS2_AUD:
        HAL_TSP_SelAudOut(u32EngId);
    case E_DRVTSP_CTRL_MODE_TS2:
        HAL_TSP_PS_Path_Disable();
        if(!_ptsp_res->_bLiveSING_FilePP)
        {
            HAL_TSP_DoubleBuf_Disable();
        }
        #if 0
        u32CaMode = HAL_TSP_CSA_Get_ScrmPath();
        if((u32CaMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2|TSP_CA_OUTPUT_PLAY_LIVE)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2))
        {
            _TSP_FreeDupDscmbPidFlt(u32EngId, 2, _ptsp_res->_u32OrgCAMode);
            HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode & ~TSP_CA_OUTPUT_PLAY_LIVE2);
        }
        PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%lx\n", __FUNCTION__, __LINE__, _ptsp_res->_u32OrgCAMode & ~TSP_CA_OUTPUT_PLAY_LIVE2));
        #endif
        HAL_TSP_SetCtrlMode(u32EngId, TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 2);
        break;

    #ifdef TS2_LIVE_SUPPORT
    case E_DRVTSP_CTRL_MODE_PVR1_CA:
        //switch CA to TS1
        HAL_TSP_CMD_Run(HAL_CMD_CHK_TSIF2_DSCMB_EN, 0, 0, &u32CaMode);
        if(u32CaMode != 0)
        {
            _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record CACtrl before set CA input from TSIF2
            HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF2_EN|TSP_CA_OUTPUT_PLAY_LIVE2);
            PVRCA_DBG(ULOGD("TSP", "[%s][%d] u32CaMode 0x%x\n", __FUNCTION__, __LINE__, TSP_CA_INPUT_TSIF2_EN|TSP_CA_OUTPUT_PLAY_LIVE2));
        }
        break;
    #endif
#endif
    default:
        _TSP_RETURN(DRVTSP_FAIL);
    }

    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Select ChipTop PAD for ts0 and ts1
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eIf                     \b IN: TSP TS interface
/// @param  pIfSet                  \b IN: TS interface setting
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
    case E_DRVTSP_IF_PLAYBACK:
        u32Flow = 0;
        break;

#ifndef NO_PVR
    case E_DRVTSP_IF_PVR0:
        u32Flow = 1;
        break;
#endif

#ifdef TS2_IF_SUPPORT
    case E_DRVTSP_IF_PVR1:
        u32Flow = 2;
        break;
#endif
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
    default:
        _TSP_RETURN(DRVTSP_FAIL);
    }
    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_AVFifo_Status(DrvTSP_FltType eFilterType, MS_U32 *u32FifoLevel)
{
    *u32FifoLevel = -1;

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
    default:
        _TSP_RETURN(DRVTSP_FAIL);
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
/// @param  u32FltId                  \b IN: index of Pid Filter [ 0 .. (@ref TSP_PIDFLT_NUM_ALL-1) ]
/// @param  peState                 \b OUT: Flter status
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Flt_GetState(MS_U32 u32EngId, MS_U32 u32FltId, DrvTSP_FltState *peState)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32FltId, TSP_DBG("[TSP_ERROR][%06d] Bad FltId %d\n", __LINE__, (unsigned int)u32FltId));
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
/// Set TS Interface packet mode
/// @param  u32TSIf                         \b IN: TSIF
/// @param  eDrvPktMode                  \b IN: Packet mode
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetPacketMode(MS_U32 u32TSIf, DrvTSP_PacketMode eDrvPktMode)
{
    _TSP_ENTRY();
    HAL_TSP_SetPacketMode(u32TSIf, (MS_U32)eDrvPktMode);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Sync Byte value to Stream source ID or TS source ID
/// @param u32SrcID \b               IN: Index of stream source id or TS source id
///                                                   Stream source ID is from 0 ~7
///                                                   TS source ID is mapping TSIF index
/// @param u8SyncByte \b            IN: Sync byte, defalut value is 0x47
///
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetMergeStrSyncByte(MS_U32 u32SrcID, MS_U8 u8SyncByte)
{
    _TSP_ENTRY();
    HAL_TSP_SetMergeStrSyncByte(u32SrcID, u8SyncByte);
    _TSP_RETURN(DRVTSP_OK);
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
    else
    {
        _TSP_RETURN(DRVTSP_INVALID_PARAM);
    }

    HAL_TSP_RemoveDupAVFifoPkt(u32FifoIdx, bEnable);

    _TSP_RETURN(DRVTSP_OK);
}

#ifdef STC64_SUPPORT
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
#endif //STC64_SUPPORT

//-------------------------------------------------------------------------------------------------
/// STC Clock Rate Adjust (27MHz clock)
/// @param u32EngId                          \b IN: STC engine ID
/// @param u32Percentage                  \b IN: The percentage of STC clock ratio
/// @return TSP_Result
/// @note
/// StcClk = (InClk * (Stc_CW / (2^27))) / 3.2
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STCClk_Adjust(MS_U32 u32EngId, MS_BOOL bUpClk, MS_U32 u32Percentage)
{
    MS_U32 u32CW_New = 0;

    _TSP_ENTRY();

    if(bUpClk)
    {
        u32CW_New = (((STC_SYNTH_DEFAULT >> 16) * 100) / u32Percentage) << 16;
        u32CW_New += (((STC_SYNTH_DEFAULT & 0xFFFF) * 100) / u32Percentage);
    }
    else
    {
        u32CW_New = (((STC_SYNTH_DEFAULT >> 16) * u32Percentage) / 100) << 16;
        u32CW_New += (((STC_SYNTH_DEFAULT & 0xFFFF) * u32Percentage) / 100);
    }

    HAL_TSP_Stc_ctrl(u32EngId, u32CW_New);

    _TSP_RETURN(DRVTSP_OK);
}

// Hidden interface. For Mstar use only
//-------------------------------------------------------------------------------------------------
// Set/Get TSP filter information
// @param  u32EngId                 \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  pu32StartFlt             \b IN/OUT: starting filter id which is availabe
// @param  pu32EndFlt               \b IN/OUT: starting filter id which is availabe
// @param  bSet                     \b IN: TRUE set, and FALSE for get
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Flt_SetOwner(MS_U32 u32EngId, MS_U32 u32FltIdStart, MS_U32 u32FltIdEnd, MS_BOOL bOwner)
{
    MS_U32 i = 0, u32PidFltId = 0;
    REG_SecFlt *pSecFilter = NULL;

    _TSP_ENTRY();
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32FltIdStart, TSP_DBG("[TSP_ERROR][%06d] Bad FltIdStart\n", __LINE__));
    TSP_ASSERT2(TSP_SECFLT_NUM_All > u32FltIdEnd, TSP_DBG("[TSP_ERROR][%06d] Bad FltIdEnd\n", __LINE__));
    TSP_ASSERT2(u32FltIdEnd >= u32FltIdStart, TSP_DBG("[TSP_ERROR][%06d] u32FltIdEnd < u32FltIdStart\n", __LINE__));

    for (i = u32FltIdStart; i <= u32FltIdEnd; i++)
    {
        u32PidFltId = i;
        #ifdef TS2_LIVE_SUPPORT
        if(i >= TSP_SECFLT_NUM)
    {
            u32PidFltId += TSP_PIDFLT1_NUM;
        }
        #endif

        if ((_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] != E_DRVTSP_FLT_STATE_FREE) &&
            (_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] != E_DRVTSP_FLT_STATE_NA))
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

        pSecFilter = _TSP_GET_SECFLT_REG(i);
        if (bOwner)
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].SecFltState[i] = E_DRVTSP_FLT_STATE_FREE;
            _ptsp_res->_TspState[u32EngId].SecBufState[i] = E_DRVTSP_FLT_STATE_FREE;
            HAL_TSP_SetOwner(pSecFilter, TRUE);
        }
        else
        {
            _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId] = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecFltState[i] = E_DRVTSP_FLT_STATE_NA;
            _ptsp_res->_TspState[u32EngId].SecBufState[i] = E_DRVTSP_FLT_STATE_NA;
            HAL_TSP_SetOwner(pSecFilter, FALSE);
        }

    }
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
// Ask TSP to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @param  u32EngId                 \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32FltId                 \b IN: Which filter id to process. 0xFFFFFFFF means process all filters (DMXID)
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return TSP_Result
// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Proc(MS_U32 u32EngId, MS_U32 u32FltId, TSP_Result* pRet, DrvTSP_Event* pEvt)                        // for non-OS TSP scheduling
{
    OSAL_TSP_ProcTypeParam Data;
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL > u32FltId, TSP_DBG("[TSP_ERROR][%06d] Bad FltId %d\n", __LINE__, (unsigned int)u32FltId));
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
/// Get current system time clock (STC) of TSP
/// @param  u32EngId                  \b IN: index of STC engine
/// @param  pu32STC_32              \b OUT: pointer of STC[32] return
/// @param  pu32STC                   \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC)
{
    _TSP_ENTRY();

    //TSP_ASSERT2(TSP_ENGINE_NUM > u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad EngId %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(NULL != pu32STC_32, TSP_DBG("[TSP_ERROR][%06d] Null pointer: pu32STC_32\n", __LINE__));
    TSP_ASSERT2(NULL != pu32STC, TSP_DBG("[TSP_ERROR][%06d] Null pointer: pu32STC\n", __LINE__));

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
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Opt)
{
    _TSP_ENTRY();
    if (HAL_TSP_STC_UpdateCtrl(u8Eng, u8Opt))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

//-------------------------------------------------------------------------------------------------
/// Get PRC of TSP
/// @param  u32EngId                 \b IN: index of STC engine
/// @param  pu32Pcr_32              \b OUT: pointer of STC[32] return
/// @param  pu32Pcr                   \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32EngId, MS_U32 *pu32Pcr_32, MS_U32 *pu32Pcr)
{
    _TSP_ENTRY();

    //TSP_ASSERT2(TSP_ENGINE_NUM > u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad EngId %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(NULL != pu32Pcr_32, TSP_DBG("[TSP_ERROR][%06d] Null pointer: pu32Pcr_32\n", __LINE__));
    TSP_ASSERT2(NULL != pu32Pcr, TSP_DBG("[TSP_ERROR][%06d] Null pointer: pu32Pcr\n", __LINE__));

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

#ifndef MSOS_TYPE_LINUX_KERNEL
    if(MDrv_SYS_Query(E_SYS_QUERY_DTV_SUPPORTED) == FALSE)
    {
        if((eFilterType & E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_LIVE)
        {
            // block DTV function
            ULOGE("TSP", "[TSP_ERROR][%06d] DTV function not supported on this chip\n", __LINE__);
            return DRVTSP_NOT_SUPPORTED;
        }
    }
#endif //end of undef MSOS_TYPE_LINUX_KERNEL

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));

    *pu32PidFltId=      0xFFFFFFFF;
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
        REG_SecFlt *pSecFilter = NULL;

        if(*pu32PidFltId >= TSP_SECFLT_NUM)
        {
            u32SecFltId -= TSP_PIDFLT1_NUM;
        }
        if (!_TSP_SecFlt_Alloc(u32EngId, eFilterType, &u32SecFltId))
        {
            TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate Section Filter fail\n", __LINE__));
        }
        HAL_TSP_PidFlt_SelSecFlt(HAL_TSP_GetPidFltReg(u32EngId, *pu32PidFltId), u32SecFltId);
        _ptsp_res->_TspState[u32EngId].FltMap[*pu32PidFltId] = u32SecFltId;
        pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);
        _TSP_SecFlt_SetType(pSecFilter, _ptsp_res->_TspState[u32EngId].FltType[*pu32PidFltId]);
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
    MS_U32 u32SecFltId, u32Flag;
    TSP_Result tRes = DRVTSP_FAIL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    // this API only free pidflt which had section
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));

    //_TSP_IND_UNLOCK();

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId])
    {
        _TSP_RETURN(DRVTSP_OK);
    }

    // PVR pid filters and AV filters do not have SecFltEvtNotify, so call free function directly.
    u32SecFltId = _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId];
    #ifndef TS2_LIVE_SUPPORT
    if((u32PidFltId >= TSP_SECFLT_NUM)
    #else
    if(((u32PidFltId >= TSP_SECFLT_NUM) && (u32PidFltId < (TSP_SECFLT_NUM+TSP_PIDFLT1_NUM)))
    #endif
        || ((_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId] & E_DRVTSP_FLT_TYPE_SECTION_MASK) == 0))
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
            //ULOGE("TSP", "[%s][%d]Warning !! Filter %d callback is running %x! Try Again!\n", __FUNCTION__, __LINE__, (unsigned int)u32PidFltId, (unsigned int)stDrvState);
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
TSP_Result MDrv_TSP_PidFlt_SetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32PID)
{
    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_HWPCR_FLT_START_ID)
    {
        HAL_TSP_PidFlt_SetHWPcrPid(u32PidFltId - TSP_HWPCR_FLT_START_ID, u32PID);
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
#endif
        HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId), u32PID);

        #ifndef NO_CA
        _TSP_FLT1_DupFLT0_Pid(0, u32PidFltId, u32PID);

        #ifdef TS2_LIVE_SUPPORT
        //Duplicate  FLT2 pid to FLT1 for dscmb
        _TSP_FLT1_DupFLT2_Pid(0, u32PidFltId, u32PID);
        #endif //end TS2_IF_SUPPORT define

        #endif //end NO_CA define

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

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
    //TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

#ifdef HWPCR_ENABLE
    if(u32PidFltId >= TSP_HWPCR_FLT_START_ID)
    {
        *pu32PID = HAL_TSP_PidFlt_GetHWPcrPid(u32PidFltId - TSP_HWPCR_FLT_START_ID);
    }
    else
    {
#endif

        *pu32PID = HAL_TSP_PidFlt_GetPid(HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId));

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
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] SecFltId= %d\n", __LINE__, (unsigned int)u32SecFltId));

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_PidFlt_SelSecFlt(HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId), u32SecFltId);
    _ptsp_res->_TspState[u32EngId].FltMap[u32PidFltId] = u32SecFltId;
    _TSP_SecFlt_SetType(pSecFilter, _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId]);
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
    REG_PidFlt*         pPidFilter= NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
    TSP_ASSERT2(E_DRVTSP_FLT_STATE_FREE!= _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], TSP_DBG("[TSP_ERROR][%06d] Bad Flt state\n", __LINE__));

    if (!bEnable)
    {
        if (!HAS_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], E_DRVTSP_FLT_STATE_ENABLE))
        {
            // TSP_DBG("[TSP_WARNNING][%06d] disable an inactive PidFlt %d\n", __LINE__, u32PidFltId);
            _TSP_RETURN(DRVTSP_OK);
        }

        _TSP_PidFlt_Disable(u32EngId, u32PidFltId);
        _TSP_RETURN(DRVTSP_OK);
    }

    if (HAS_FLAG(_ptsp_res->_TspState[u32EngId].FltState[u32PidFltId], E_DRVTSP_FLT_STATE_ENABLE))
    {
        // TSP_DBG("[TSP_WARNNING][%06d] Enable an active PidFlt %d\n", __LINE__, u32PidFltId);
        _TSP_RETURN(DRVTSP_OK);
    }

    pPidFilter=  HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId);
    if (E_DRVTSP_FLT_TYPE_PVR == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        if (E_DRVTSP_FLT_SOURCE_TYPE_FILE == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
        {
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_PVR_ENABLE);
        }
#ifdef EXT_PVR1_FLT
        else
        {
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT);
        }
#endif

#ifndef NO_PVR
        TSP_ASSERT2(TSP_SECFLT_NUM <= u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
#endif //udef NO_PVR
    }
#ifdef TS2_IF_SUPPORT
    else if (E_DRVTSP_FLT_TYPE_CB == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        #ifdef TS2_LIVE_SUPPORT
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_REC);
        #endif
        TSP_ASSERT2((TSP_PIDFLT_BASE0_NUM+TSP_PIDFLT_BASE1_NUM) <= u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
    }
#endif
    else if (E_DRVTSP_FLT_TYPE_VIDEO == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        #ifdef TS2_LIVE_SUPPORT
        HAL_TSP_AVFIFO_Src_SelTs2(TSP_FIFO_VD, (E_DRVTSP_FLT_SOURCE_TYPE_TS2 == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId]));
        #endif
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_VFIFO);
    }
    else if (E_DRVTSP_FLT_TYPE_AUDIO == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        #ifdef TS2_LIVE_SUPPORT
        HAL_TSP_AVFIFO_Src_SelTs2(TSP_FIFO_AU, (E_DRVTSP_FLT_SOURCE_TYPE_TS2 == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId]));
        #endif
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_AFIFO);
    }
#ifndef NO_AUBFIFO
    else if (E_DRVTSP_FLT_TYPE_AUDIO2 == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        #ifdef TS2_LIVE_SUPPORT
        HAL_TSP_AVFIFO_Src_SelTs2(TSP_FIFO_AUB, (E_DRVTSP_FLT_SOURCE_TYPE_TS2 == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId]));
        #endif
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_AFIFO2);
    }
#endif
#ifdef MMFI_VD3D
    else if (E_DRVTSP_FLT_TYPE_VIDEO3D == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        #ifdef TS2_LIVE_SUPPORT
        HAL_TSP_AVFIFO_Src_SelTs2(TSP_FIFO_V3D, (E_DRVTSP_FLT_SOURCE_TYPE_TS2 == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId]));
        #endif
        HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_VFIFO3D);
    }
#endif
#ifdef HWPCR_ENABLE
    else if (E_DRVTSP_FLT_TYPE_PCR == _ptsp_res->_TspState[u32EngId].FltType[u32PidFltId])
    {
        MS_U32 u32Idx = u32PidFltId - TSP_HWPCR_FLT_START_ID;
        MS_U32 u32PktSrc = TSP_SRC_FROM_PKTDMX0;

        switch(_ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
        {
            case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
                u32PktSrc = TSP_SRC_FROM_PKTDMX0;
                break;
            case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
                u32PktSrc = TSP_SRC_FROM_PKTDMXFL;
                break;
        }

        #if (TSP_HWPCR_BY_HK == 1)
        _u8ResetPcr[u32Idx] = 3;
        _u32PrevStcBase[u32Idx] = 0;
        _u32PrevPllControl[u32Idx] = 0;
        #endif

        HAL_TSP_HWPcr_Reset(u32Idx, TRUE);
        #if (TSP_HWPCR_BY_HK == 1)
        HAL_TSP_HWPcr_Reset(u32Idx, FALSE);
        HAL_TSP_HWPcr_Int_Enable(u32Idx, TRUE);
        _TSP_SetHwPcrNotifyId(u32Idx);
        #endif

        HAL_TSP_HWPcr_SelSrc(u32Idx, u32PktSrc);
        HAL_TSP_PidFlt_HWPcrFlt_Enable(u32Idx, TRUE);
        _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId]|= E_DRVTSP_FLT_STATE_ENABLE;
        _TSP_RETURN(DRVTSP_OK);
    }
#endif
    else
    {
        REG_SecFlt*     pSecFilter = NULL;
        MS_U32          u32SecFltId;

        REG_SecFlt*     pSecBufFilter = NULL;
        MS_U32          u32SecBufId;
        TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));

        u32SecFltId = HAL_TSP_PidFlt_GetSecFlt(pPidFilter);
        TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] SecFltId= %d\n", __LINE__, (unsigned int)u32SecFltId));
        pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);;
        //u32SecBufId = HAL_TSP_SecFlt_GetSecBuf(pSecFilter);
        u32SecBufId = u32SecFltId;

        // reset ver/crc check
        if (_ptsp_res->_TspState[u32EngId].FltType[u32PidFltId] == E_DRVTSP_FLT_TYPE_SECTION_VER)
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
            TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecBufId, TSP_DBG("[TSP_ERROR][%06d] SecBufId= %d\n", __LINE__, (unsigned int)u32SecBufId));
            pSecBufFilter=  _TSP_GET_SECFLT_REG(u32SecBufId);;
            HAL_TSP_SecBuf_ResetBuffer(pSecBufFilter);
            HAL_TSP_SecFlt_ResetState(pSecFilter);
            HAL_TSP_SecFlt_SetRmnCount(pSecFilter, 0);
            //HAL_TSP_Reset_SecDisCont_PktCount(u32PidFltId);
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT);
            break;
        case E_DRVTSP_FLT_TYPE_EMM:
        case E_DRVTSP_FLT_TYPE_ECM:
            HAL_TSP_SecFlt_ResetState(pSecFilter);
            HAL_TSP_SecFlt_SetRmnCount(pSecFilter, 0);
            HAL_TSP_SecBuf_ResetBuffer(pSecFilter);
            //HAL_TSP_Reset_SecDisCont_PktCount(u32PidFltId);
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT);
            break;
        case E_DRVTSP_FLT_TYPE_PCR:
            HAL_TSP_SecFlt_PcrReset(pSecFilter);
#if (defined(NO_FILEIN) || !defined(VQ_ENABLE))
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT|TSP_PIDFLT_OUT_SECFLT_AF);
#else
            HAL_TSP_PidFlt_SelFltOutput(pPidFilter, TSP_PIDFLT_OUT_SECFLT_AF);
#endif
            break;
        default:
            break;
        }
    }

    if (E_DRVTSP_FLT_SOURCE_TYPE_LIVE == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltSource(pPidFilter, TSP_PIDFLT_IN_LIVE);
    }
#ifndef NO_FILEIN
    else if (E_DRVTSP_FLT_SOURCE_TYPE_FILE == _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId])
    {
        HAL_TSP_PidFlt_SelFltSource(pPidFilter, TSP_PIDFLT_IN_FILE);
    }
#endif //udef NO_FILEIN

    _ptsp_res->_TspState[u32EngId].FltState[u32PidFltId]|= E_DRVTSP_FLT_STATE_ENABLE;
    _TSP_RETURN(DRVTSP_OK);
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
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_PIDFLT_NUM_ALL> u32PidFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId %d\n", __LINE__, (unsigned int)u32PidFltId));
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

//only for debug
TSP_Result MDrv_TSP_PidFlt_PVREnable(MS_U32 u32EngId, MS_U32 u32PidFltId)
{
#ifndef NO_PVR
    REG_PidFlt*         pPidFilter= NULL;

    pPidFilter=  HAL_TSP_GetPidFltReg(u32EngId, u32PidFltId);

    HAL_TSP_PidFlt_PVREnable(pPidFilter);

    return DRVTSP_OK;
#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//-------------------------------------------------------------------------------------------------
/// Change PID filter source
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eFilterType             \b IN: index of filter to be enable
/// @param  bEnable                  \b IN: New filter source type to change
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PidFlt_ChangeSource(MS_U32 u32EngId, MS_U32 u32PidFltId, DrvTSP_FltType eFilterType)
{
    _TSP_ENTRY();
    if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_LIVE)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_LIVE;
    }
    else if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_FILE)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_FILE;
    }
    else if ((eFilterType&E_DRVTSP_FLT_SOURCE_TYPE_MASK) == E_DRVTSP_FLT_SOURCE_TYPE_TS2)
    {
        _ptsp_res->_TspState[u32EngId].FltSource[u32PidFltId] = E_DRVTSP_FLT_SOURCE_TYPE_TS2;
    }

    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get PID filter scramble status
/// @param  FltSrc                         \b IN     : Filter source
/// @param  u32FltGroupId             \b IN     : Pid filter group id, every 32 filter for 1 group. 0: filter 0~31, 1: 32~63
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

    switch(FltSrc)
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            u32Src = 0;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_FILE:
            u32Src = 1;
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS2:
            *pu32ScmbSts = 0;
            _TSP_RETURN(DRVTSP_OK);
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
    REG_SecFlt     *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM > u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    #ifdef TS2_LIVE_SUPPORT
    if(*pu32SecFltId >= TSP_SECFLT_NUM)
    {
        *pu32SecFltId -= TSP_PIDFLT1_NUM;
    }
    #endif

    if (FALSE== _TSP_SecFlt_Alloc(u32EngId, E_DRVTSP_FLT_TYPE_SECTION, pu32SecFltId))
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate section filter fail\n", __LINE__));
    }
    TSP_ASSERT2(TSP_SECFLT_NUM_All> *pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)*pu32SecFltId));
    u32SecBufId = *pu32SecFltId;
    if (FALSE == _TSP_SecBuf_Alloc(u32EngId, &u32SecBufId))
    {
        TSP_ASSERT2(0, TSP_DBG("[TSP_ERROR][%06d] Allocate section buffer fail\n", __LINE__));
    }
    TSP_ASSERT2(TSP_SECBUF_NUM > u32SecBufId, TSP_DBG("[TSP_ERROR][%06d] Bad SecBufId %d\n", __LINE__, (unsigned int)u32SecBufId));
    TSP_ASSERT2(u32SecBufId == *pu32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad PidFltId/SecFltId alignment\n", __LINE__));
    _TSP_SecBuf_Init(u32EngId, u32SecBufId);
    pSecFilter = _TSP_GET_SECFLT_REG(*pu32SecFltId);
    HAL_TSP_SecFlt_SelSecBuf(pSecFilter, u32SecBufId, TRUE);
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

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
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
    REG_SecFlt     *pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));

    if (eSecFltMode & E_DRVTSP_FLT_MODE_ONESHOT)
    {
        uMode |= TSP_SECFLT_MODE_ONESHOT;
    }

    if (eSecFltMode & E_DRVTSP_FLT_MODE_CRCCHK)
    {
        uMode |= TSP_SECFLT_MODE_CRCCHK;
    }

    _ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId] = eSecFltMode;

    HAL_TSP_SecFlt_SetMode(pSecFilter, uMode);
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
    int i = 0;
    REG_SecFlt *pSecFilter = NULL;

    if((pu8Match == NULL) || (pu8Mask == NULL) || (pu8NotMask == NULL))
    {
        return (DRVTSP_INVALID_PARAM);
    }

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);
    for (i = 0 ; i < 16 ; i++)
    {
        *(pu8Mask+i) = 0xFF^(*(pu8Mask+i));
    }

    HAL_TSP_SecFlt_SetMask(pSecFilter, pu8Mask);
    HAL_TSP_SecFlt_SetMatch(pSecFilter, pu8Match);
    HAL_TSP_SecFlt_SetNMask(pSecFilter, pu8NotMask);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Reset the section buffer read/write pointer to start address and resolve overflow condition
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be reset
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_ResetBuffer(MS_U32 u32EngId, MS_U32 u32SecFltId)
{
    REG_SecFlt *pSecFilter = NULL;

    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    // Richard: Be careful since it works based the assumption that SecFltId is equal to SecBufId
    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_SecBuf_ResetBuffer(pSecFilter);
    HAL_TSP_SecFlt_ResetState(pSecFilter);
    HAL_TSP_SecFlt_ResetEmmIdx(pSecFilter);
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
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_SecBuf_SetBuffer(pSecFilter, phyStartAddr, u32BufSize);
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
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_SecFlt_SetReqCount(pSecFilter, u32ReqCount);
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
#if 0
    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %ld\n", __LINE__, u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %ld\n", __LINE__, u32SecFltId));
    REG_SecFlt *pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_SecFlt_SetEcmIdx(pSecFilter, u32EcmIdx);
    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_OK;
#endif
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
    REG_SecFlt *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pphyBufStart, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    *pphyBufStart= HAL_TSP_SecBuf_GetBufStart(pSecFilter);
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
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pu32BufSize, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    *pu32BufSize = 0;
    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    *pu32BufSize=  (MS_U32)(HAL_TSP_SecBuf_GetBufEnd(pSecFilter)- HAL_TSP_SecBuf_GetBufStart(pSecFilter));
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32ReadAddr            \b OUT: pointer of address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetReadAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyReadAddr)
{
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pphyReadAddr, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    // move write point to read point while E_DRVTSP_FLT_MODE_AUTO_ADDR mode
    if (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId]&E_DRVTSP_FLT_MODE_AUTO_ADDR)
    {
        HAL_TSP_SecBuf_SetBufRead(pSecFilter, HAL_TSP_SecBuf_GetBufWrite(pSecFilter));
    }
    *pphyReadAddr= HAL_TSP_SecBuf_GetBufRead(pSecFilter);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get current section data write address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32WriteAddr           \b OUT: pointer of address return
/// @return TSP_Result
/// @note
/// User can get current write address to know where is the end of section data
/// received in the section buffer.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_GetWriteAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY *pphyWriteAddr)
{
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT2(pphyWriteAddr, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    // move write point to read point while E_DRVTSP_FLT_MODE_AUTO_ADDR mode
    if (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId]&E_DRVTSP_FLT_MODE_AUTO_ADDR)
    {
        HAL_TSP_SecBuf_SetBufRead(pSecFilter, HAL_TSP_SecBuf_GetBufWrite(pSecFilter));
    }
    *pphyWriteAddr= HAL_TSP_SecBuf_GetBufWrite(pSecFilter);
    _TSP_RETURN(DRVTSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  u32ReadAddr             \b IN: address of read pointer
/// @return TSP_Result
/// @note
/// User can update the read address to notify TSP where is the end of section
/// data already read back by user.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SecFlt_SetReadAddr(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_PHY phyReadAddr)
{
    REG_SecFlt      *pSecFilter = NULL;

    _TSP_ENTRY();
    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));

    if (E_DRVTSP_FLT_STATE_FREE == _ptsp_res->_TspState[u32EngId].SecFltState[u32SecFltId])
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    HAL_TSP_SecBuf_SetBufRead(pSecFilter, phyReadAddr);
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

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__,(unsigned int)u32SecFltId));

    if ((eEvents & (E_DRVTSP_EVENT_DATA_READY   |
                    E_DRVTSP_EVENT_BUF_OVERFLOW | E_DRVTSP_EVENT_SEC_CRCERR)) == 0)
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

    TSP_ASSERT2(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT2(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
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
    REG_SecFlt      *pSecFilter = NULL;

    TSP_ASSERT(TSP_ENGINE_NUM> u32EngId, TSP_DBG("[TSP_ERROR][%06d] Bad Engine Id %d\n", __LINE__, (unsigned int)u32EngId));
    TSP_ASSERT(TSP_SECFLT_NUM_All> u32SecFltId, TSP_DBG("[TSP_ERROR][%06d] Bad SecFltId %d\n", __LINE__, (unsigned int)u32SecFltId));
    TSP_ASSERT(pphyWriteAddr, TSP_DBG("[TSP_ERROR][%06d] NULL pointer\n", __LINE__));
    // move write point to read point while E_DRVTSP_FLT_MODE_AUTO_ADDR mode

    pSecFilter = _TSP_GET_SECFLT_REG(u32SecFltId);

    if (_ptsp_res->_TspState[u32EngId].SecFltMode[u32SecFltId]&E_DRVTSP_FLT_MODE_AUTO_ADDR)
    {
        HAL_TSP_SecBuf_SetBufRead_tmp(pSecFilter, HAL_TSP_SecBuf_GetBufWrite_tmp(pSecFilter));
    }
    *pphyWriteAddr= HAL_TSP_SecBuf_GetBufWrite_tmp(pSecFilter);
    return DRVTSP_OK;
}


// PVR API
//-------------------------------------------------------------------------------------------------
/// Set PVR double record buffer start addresses and buffer size
/// @param  u32BufStart0            \b IN: start address of PVR buffer 0
/// @param  u32BufStart1            \b IN: start address of PVR buffer 1
/// @param  u32BufSize0             \b IN: size of PVR buffer 0
/// @param  u32BufSize1             \b IN: size of PVR buffer 1
/// @return TSP_Result
/// @note
/// Buffer start address and size should be 128-bit (16-byte) aligned\n
/// The maximum support size is 0xFFFF0 (1048560 bytes).
/// @sa MDrv_TSP_PVR_Notify
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_SetBuffer(MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
#ifndef NO_PVR
    _TSP_ENTRY();
    HAL_TSP_PVR_SetBuffer(phyBufStart0, phyBufStart1, u32BufSize0, u32BufSize1);

    // flush PVR buffer
    HAL_TSP_PVR_WaitFlush();

    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


//-------------------------------------------------------------------------------------------------
/// Set PVR record mode and START/STOP recording.
/// @param  eRecMode                \b IN: mode of recording
/// @param  bStart                  \b IN: TRUE(START), FALSE(STOP)
/// @return TSP_Result
/// @note
/// Parameter eRecMode will be ignored when bStart is FALSE(STOP)\n
/// @note
/// It's a synchronous function. When STOP, it flushs internal record fifo
/// and update current PVR record buffer write address before function return.\n
/// User can call MDrv_TSP_PVR_GetWriteAddr to get the final valid data address
/// after recording.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Start(DrvTSP_RecMode eRecMode, MS_BOOL bStart)
{
#ifndef NO_PVR

    _TSP_ENTRY();
    if (bStart)
    {
        //HAL_TSP_PVR_WaitFlush();
        if(eRecMode == E_DRVTSP_REC_MODE_ENG0_FI_BYPASS)
        {
            HAL_TSP_PVR_Filein_Enable(TRUE, TRUE, TRUE);
            HAL_TSP_PVR_All(TRUE);
        }
        else if(eRecMode == E_DRVTSP_REC_MODE_ENG0_FI_FLTTYPE)
        {
            HAL_TSP_PVR_Filein_Enable(TRUE, TRUE, FALSE);
            HAL_TSP_PVR_All(FALSE);
        }
        else if(eRecMode == E_DRVTSP_REC_MODE_ENG0_FLT1CA)
        {
            //HAL_TSP_TSIF1_Enable(TRUE);
            HAL_TSP_PVR_REC_FLT_Enable(TRUE);
            _ptsp_res->TSPPvrcaParams._bPVRCA_Start = TRUE;
        }
        else
        {
            HAL_TSP_PVR_All((HAS_FLAG(eRecMode, 0x02))? TRUE: FALSE);
        }
        //HAL_TSP_LiveAB_En(TRUE); // set pvr to pidflt0_file
        HAL_TSP_PVR_Enable(TRUE);
    }
    else
    {
        //HAL_TSP_PVR_WaitFlush();
        if((eRecMode == E_DRVTSP_REC_MODE_ENG0_FI_BYPASS) || (eRecMode == E_DRVTSP_REC_MODE_ENG0_FI_FLTTYPE))
        {
            HAL_TSP_PVR_Filein_Enable(FALSE, FALSE, FALSE);
        }
        else if(eRecMode == E_DRVTSP_REC_MODE_ENG0_FLT1CA)
        {
            HAL_TSP_PVR_REC_FLT_Enable(FALSE);
        }

        //HAL_TSP_LiveAB_En(FALSE); // disable pvr to pidflt0_file
        HAL_TSP_PVR_Enable(FALSE);

        HAL_TSP_PVR_WaitFlush();

        _ptsp_res->TSPPvrcaParams._bPVRCA_Start = FALSE;
    }

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


//-------------------------------------------------------------------------------------------------
/// Get final write address of PVR record buffer
/// @param  pu32WriteAddr           \b OUT: pointer of address return
/// @return TSP_Result
/// @note
/// User can get current write address to know where is the end of section data
/// received in the section buffer.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_GetWriteAddr(MS_PHY *pu32WriteAddr)
{
#ifndef NO_PVR

    _TSP_ENTRY();

    *pu32WriteAddr = (MS_PHY)HAL_TSP_PVR_GetBufWrite();

    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


//-------------------------------------------------------------------------------------------------
/// Subscribe event notification callback function for PVR record buffer
/// @param  eEvents                 \b IN: events need to be subscribed\n
///                                        @ref E_DRVTSP_EVENT_PVRBUF_FULL
/// @param  pfCallback              \b IN: callback function (NULL to disable)
/// @return TSP_Result
/// @note
/// TSP PVR recording uses double buffer mechanism. This function register a callback
/// function for recording. TSP calls callback function each time when one of
/// double buffer is full and switch to another buffer to record.\n
/// @ref E_DRVTSP_FLT_TYPE_PVR
/// @attention
/// Callback function resides in OS TSP interrupt context, it recommends
/// that callback function should not take too much time to block the system.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Notify(DrvTSP_Event eEvents, P_DrvTSP_EvtCallback pfCallback)
{
#ifndef NO_PVR

    _TSP_ENTRY();

    if(eEvents & (E_DRVTSP_EVENT_PVRBUF_FULL | E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
    {
        if (pfCallback)
        {
            _TSP_SetPvrNotifyId(0);
            HAL_TSP_Int_Enable(TSP_HWINT_HW_PVR_MASK);
        }
        else
        {
            _TSP_RemovePvrNotifyId(0);
            HAL_TSP_Int_Disable(TSP_HWINT_HW_PVR_MASK);
        }
        _ptsp_res->_PvrEvtNotify[0] = eEvents & (E_DRVTSP_EVENT_PVRBUF_FULL | E_DRVTSP_EVENT_PVRBUF_OVERFLOW);
        _ptsp_res->_PvrCallback[0] = pfCallback;
    }

#ifdef TS2_IF_SUPPORT
    if(eEvents & (E_DRVTSP_EVENT_CBBUF_FULL | E_DRVTSP_EVENT_CBBUF_OVERFLOW))
    {
        if (pfCallback)
        {
            _TSP_SetPvrNotifyId(1);
            HAL_TSP_Int2_Enable(TSP_HWINT2_HW_PVR1_MASK);
        }
        else
        {
            _TSP_RemovePvrNotifyId(1);
            HAL_TSP_Int2_Disable((TSP_HWINT2_HW_PVR1_MASK));
        }
        _ptsp_res->_PvrEvtNotify[1] = eEvents & (E_DRVTSP_EVENT_CBBUF_FULL | E_DRVTSP_EVENT_CBBUF_OVERFLOW);
        _ptsp_res->_PvrCallback[1] = pfCallback;
    }
#endif

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


TSP_Result MDrv_TSP_PVR_SetPacketMode(MS_BOOL bSet)
{
#ifndef NO_PVR

    _TSP_ENTRY();
    HAL_TSP_PVR_PacketMode(bSet);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


//-------------------------------------------------------------------------------------------------
/// PVR time stamp set
/// @param  u32Stamp           \b IN: PVR time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStampSetRecordStamp(MS_U32 u32Stamp)
{
#ifndef NO_PVR

    _TSP_ENTRY();
    HAL_TSP_SetPVRTimeStamp(u32Stamp);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR

}


//-------------------------------------------------------------------------------------------------
/// PVR time stamp get
/// @param  pu32Stamp           \b OUT: pointer of PVR time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStampGetRecordStamp(MS_U32* u32Stamp)
{
#ifndef NO_PVR

    _TSP_ENTRY();
    *u32Stamp = HAL_TSP_GetPVRTimeStamp();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


//-------------------------------------------------------------------------------------------------
/// Playback time stamp Set
/// @param  pu32Stamp           \b IN: Playback time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp)
{
#ifndef NO_PVR

    _TSP_ENTRY();
    HAL_TSP_SetPlayBackTimeStamp(u32Stamp);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}


//-------------------------------------------------------------------------------------------------
/// Playback time stamp get
/// @param  pu32Stamp           \b OUT: pointer of Playback time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp)
{
#ifndef NO_PVR

    _TSP_ENTRY();
    * u32Stamp = HAL_TSP_GetPlayBackTimeStamp();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR

}


//-------------------------------------------------------------------------------------------------
/// PVR time stamp Enable/Disable
/// @param  pu32Stamp           \b IN: Enable/Disable time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_TimeStamp(MS_BOOL bEnable)
{
#ifndef NO_PVR

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

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//only for debug
TSP_Result MDrv_TSP_PVR_Filein_Enable(MS_BOOL bBypassHD, MS_BOOL bEnable)
{
#ifndef NO_PVR

    HAL_TSP_PVR_Filein_Enable(bBypassHD, bEnable, FALSE);

    return DRVTSP_OK;

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//PVRCA API
// If PVRCA PID open, reserved FLT1 for recording, beforehand for PVRCA start. (PVR1FLT_CAPVR)
// If PVRCA PID close, reserved FLT1 for release FLT1, beforehand for PVRCA stop. (PVR1FLT_CA)
TSP_Result MDrv_TSP_PVR_PidFlt_Reserved(MS_U32 Pid, MS_U8* pu8DmxId, MS_BOOL bReserved)
{
#ifndef NO_CA

    MS_U32 u32ii = (MS_U32)(*pu8DmxId & 0xFF) ;
    MS_U32 u32mask = 0;
    MS_BOOL bFound = FALSE;

    _TSP_ENTRY();

    if(!bReserved)
    {
        TSP_ASSERT2(TSP_SECFLT_NUM <= *pu8DmxId, TSP_DBG("[TSP_ERROR][%06d] Bad DMX ID %d\n", __LINE__, (int)(*pu8DmxId)));
        TSP_ASSERT2((TSP_SECFLT_NUM + TSP_PIDFLT1_NUM) > *pu8DmxId, TSP_DBG("[TSP_ERROR][%06d] Bad DMX ID %d\n", __LINE__, (int)(*pu8DmxId)));

        HAL_TSP_PidFlt_SelFltOutput(HAL_TSP_GetPidFltReg(0, u32ii), TSP_PIDFLT_OUT_NONE);

        u32mask = 1 << (u32ii - TSP_SECFLT_NUM);  //bit flag
        if((_ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef & u32mask) == 0)
        {
            HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(0, u32ii), DRVTSP_PID_NULL);
            _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii - TSP_SECFLT_NUM] = DRVTSP_PID_NULL;
        }
        _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR &= ~u32mask;

        PVRCA_DBG(ULOGD("TSP", "[%s] Unreserved FLT %ld _u32Flt1Flag_DscmbPVR 0x%lx, _u32Flt1Flag_DscmbRef %lx \n", \
            __FUNCTION__, u32ii, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef));
        PVRCA_DBG(ULOGD("TSP", "_u16Flt1PidList %x , _u16Flt0DscmbPidList %x \n", \
                _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii - TSP_SECFLT_NUM], _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii - TSP_SECFLT_NUM]));
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        *pu8DmxId = 0xFF;
    }

    //checking, if PVR pid is set, use the filter for record directly
    for(u32ii = 0; u32ii < TSP_PIDFLT1_NUM; u32ii++)
    {
        if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == Pid)
        {
            bFound = TRUE;
            break;
        }
    }

    //finding free PVR1 filters for recording
    if(bFound == FALSE)
    {
    for(u32ii = 0; u32ii < TSP_PIDFLT1_NUM; u32ii++)
    {
        if(_ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] == DRVTSP_PID_NULL)
        {
            //set new pid for dscmb pvr
            HAL_TSP_PidFlt_SetPid(HAL_TSP_GetPidFltReg(0, u32ii+TSP_SECFLT_NUM), Pid);
            _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii] = Pid;
                bFound = TRUE;
                break;
            }
        }
    }

    if(bFound == FALSE)
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

    _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR |= (1 << u32ii);
    *pu8DmxId = (MS_U8)(u32ii & 0xFF)+TSP_SECFLT_NUM;

    PVRCA_DBG(ULOGD("TSP", "[%s] FLT %ld PID 0x%lx, _u32Flt1Flag_DscmbPVR 0x%lx\n", __FUNCTION__, u32ii, Pid, _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR));
    PVRCA_DBG(ULOGD("TSP", "_u32Flt1Flag_DscmbRef %lx, _u16Flt1PidList %x , _u16Flt0DscmbPidList %x \n", _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbRef, \
                _ptsp_res->TSPPvrcaParams._u16Flt1PidList[u32ii], _ptsp_res->TSPPvrcaParams._u16Flt0DscmbPidList[u32ii]));

    if(_ptsp_res->TSPPvrcaParams._bPVRCA_Start)
    {
        HAL_TSP_PidFlt_SelFltOutput(HAL_TSP_GetPidFltReg(0, (MS_U32)(*pu8DmxId & 0xFF)), TSP_PIDFLT_OUT_SECFLT);
    }

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

TSP_Result MDrv_TSP_PVR_FLT1_StartRec(MS_BOOL bEnable)
{
#ifndef NO_CA

    MS_U8 u32ii;
    MS_U32 u32Output;
    MS_U32 u32mask = _ptsp_res->TSPPvrcaParams._u32Flt1Flag_DscmbPVR;

    _TSP_ENTRY();

    u32Output = TSP_PIDFLT_OUT_NONE;

    if(bEnable)
    {
        u32Output = TSP_PIDFLT_OUT_SECFLT;
    }

    for(u32ii = 0; u32ii < TSP_PIDFLT1_NUM; u32ii++)
    {
        if(u32mask & 0x01)
        {
            HAL_TSP_PidFlt_SelFltOutput(HAL_TSP_GetPidFltReg(0, (u32ii+TSP_SECFLT_NUM)), u32Output);
            //_bPVRFLT_Opened_Flag[u32ii] = bEnable;
        }
        u32mask >>= 1;
    }

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

TSP_Result MDrv_TSP_PVRCA_Close(MS_U8 u8PVRId)
{
#ifndef NO_CA
    MS_U32 u32CaCtrl = 0;

    if(u8PVRId >= TSP_PVR_IF_NUM)
        return DRVTSP_FAIL;

    _TSP_ENTRY();

    u32CaCtrl = HAL_TSP_CSA_Get_ScrmPath();
    if(u8PVRId == 0)
    {
        if(u32CaCtrl & TSP_CA_OUTPUT_REC)
        {
            if(_ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable == TRUE)
            {
                HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_LIVEIN|TSP_CA_OUTPUT_PLAY_LIVE);
                HAL_TSP_TSIF1_Enable(FALSE);
                _TSP_Free_AllFLT1();
            }
            else
            {
                HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode);
            }
        }
    }
    #ifdef TS2_LIVE_SUPPORT
    else if(u8PVRId == 1)
    {
        //input source is TS0, Just remove TS2 record, when PVR stop
        //input source is TS0, Just remove TS2 record, when PVR stop
        if((u32CaCtrl & (TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2)) == (TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2))
        {
            _TSP_FreeDupDscmbPidFlt(0, 2, u32CaCtrl);
            HAL_TSP_CSA_Set_ScrmPath(u32CaCtrl & ~TSP_CA_OUTPUT_PLAY_LIVE2);
        }
    }
    #endif

    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

// Channel Browser API
//-------------------------------------------------------------------------------------------------
/// Set Channel Browser record buffer start addresses and buffer size
/// @param  u8PVRId                   \b IN:PVR ID
/// @param  phyBufStart0            \b IN: start address of PVR buffer 0
/// @param  phyBufStart1            \b IN: start address of PVR buffer 1
/// @param  u32BufSize0             \b IN: size of PVR buffer 0
/// @param  u32BufSize1             \b IN: size of PVR buffer 1
/// @return TSP_Result
/// @note: PVR1 only has single buffer
/// Buffer start address and size should be 128-bit (16-byte) aligned\n
/// The maximum support size is 0xFFFF0 (1048560 bytes).
/// @sa MDrv_TSP_PVR_Notify
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SetBuffer(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1)
{
#ifndef NO_PVR

    switch(u8PVRId)
    {
        case 0:
            return (MDrv_TSP_PVR_SetBuffer(phyBufStart0, phyBufStart1, u32BufSize0, u32BufSize1));

#ifdef TS2_IF_SUPPORT
        case 1:
            _TSP_ENTRY();
            #ifndef TS2_LIVE_SUPPORT
            if((u32BufSize1 > 0) && ((phyBufStart0 + u32BufSize0) == phyBufStart1))
                u32BufSize0 += u32BufSize1;
            #endif
            HAL_TSP_PVR1_SetBuffer((MS_U32)phyBufStart0, u32BufSize0, (MS_U32)phyBufStart1, u32BufSize1);

            _TSP_RETURN(DRVTSP_OK);
#endif

        default:
            return (DRVTSP_NOT_SUPPORTED);
    }

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR

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
#ifndef NO_PVR

    if(u8PVRId == 0)
    {
        return (MDrv_TSP_PVR_Start(eRecMode, bStart));
    }
#ifdef TS2_IF_SUPPORT
    if(u8PVRId == 1)
    {
        _TSP_ENTRY();
        if (bStart)
        {
            HAL_TSP_PVR1_WaitFlush();
            HAL_TSP_PVR1_All((HAS_FLAG(eRecMode, 0x02))? TRUE: FALSE);
            //HAL_TSP_LiveAB_En(TRUE); // set pvr to pidflt0_file
            HAL_TSP_PVR1_Enable(TRUE);
        }
        else
        {
            //HAL_TSP_LiveAB_En(FALSE); // disable pvr to pidflt0_file
            HAL_TSP_PVR1_Enable(FALSE);
            HAL_TSP_PVR1_WaitFlush();
        }
        _TSP_RETURN(DRVTSP_OK);
    }
#endif

    return (DRVTSP_NOT_SUPPORTED);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR

}

//-------------------------------------------------------------------------------------------------
/// Get final write address of Channel Browser record buffer
/// @param  u8PVRId                    \b IN:PVR ID
/// @param  pphyWriteAddr           \b OUT: pointer of address return
/// @return TSP_Result
/// @note
/// User can get current write address to know where is the end of section data
/// received in the section buffer.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_GetWriteAddr(MS_U8 u8PVRId, MS_PHY *pphyWriteAddr)
{
#ifndef NO_PVR

    _TSP_ENTRY();

    switch(u8PVRId)
    {
        case 0:
            *pphyWriteAddr = HAL_TSP_PVR_GetBufWrite();
            break;
#ifdef TS2_IF_SUPPORT
        case 1:
            *pphyWriteAddr = HAL_TSP_PVR1_GetBufWrite();
            break;
#endif
        default:
            _TSP_RETURN(DRVTSP_NOT_SUPPORTED);
    }

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
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
#ifndef NO_PVR

    return (MDrv_TSP_PVR_Notify(eEvents, pfCallback));

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR

}

TSP_Result MDrv_TSP_PVR_Eng_SetPacketMode(MS_U8 u8PVRId, MS_BOOL bSet)
{
#ifndef NO_PVR

    switch(u8PVRId)
    {
        case 0:
            return (MDrv_TSP_PVR_SetPacketMode(bSet));
#ifdef TS2_IF_SUPPORT
        case 1:
            _TSP_ENTRY();
            HAL_TSP_PVR1_PacketMode(bSet);
            _TSP_RETURN(DRVTSP_OK);
#endif
        default:
            return (DRVTSP_NOT_SUPPORTED);
    }

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//-------------------------------------------------------------------------------------------------
/// Channel Browser time stamp set
/// @param  u8PVRId              \b IN:PVR ID
/// @param  u32Stamp           \b IN: Channel Browser time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(MS_U8 u8PVRId, MS_U32 u32Stamp)
{
#ifndef NO_PVR

    switch(u8PVRId)
    {
        case 0:
            return (MDrv_TSP_PVR_TimeStampSetRecordStamp(u32Stamp));
#ifdef TS2_IF_SUPPORT
        case 1:
            _TSP_ENTRY();
            HAL_TSP_SetPVR1TimeStamp(u32Stamp);
            _TSP_RETURN(DRVTSP_OK);
#endif
        default:
            return (DRVTSP_NOT_SUPPORTED);
    }

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//-------------------------------------------------------------------------------------------------
/// Channel Browser time stamp get
/// @param  u8PVRId               \b IN:PVR ID
/// @param  pu32Stamp           \b OUT: pointer of Channel Browser time stamp
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(MS_U8 u8PVRId, MS_U32* u32Stamp)
{
#ifndef NO_PVR

    switch(u8PVRId)
    {
        case 0:
            return (MDrv_TSP_PVR_TimeStampGetRecordStamp(u32Stamp));
#ifdef TS2_IF_SUPPORT
        case 1:
            _TSP_ENTRY();
            *u32Stamp = HAL_TSP_GetPVR1TimeStamp();
            _TSP_RETURN(DRVTSP_OK);
#endif
        default:
            return (DRVTSP_NOT_SUPPORTED);
    }

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//-------------------------------------------------------------------------------------------------
/// Playback time stamp clock set
/// @param  u8EngId               \b IN:playback ID
/// @param  eClk           \b IN: clock source type
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SetPlaybackStampClk(MS_U32 u32EngId, MS_U32 u32Clk)
{
#ifndef NO_PVR
    _TSP_ENTRY();
    if(HAL_TSP_SetPlayBackTimeStampClk(u32EngId, u32Clk))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }
#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
}

//-------------------------------------------------------------------------------------------------
/// PVR time stamp clock set
/// @param  u8PvrId               \b IN:PVR ID
/// @param  eClk                    \b IN: clock source type
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PVR_Eng_SetRecordStampClk(MS_U32 u32PvrId, MS_U32 u32Clk)
{
#ifndef NO_PVR
    _TSP_ENTRY();
    if(HAL_TSP_SetPVRTimeStampClk(u32PvrId, u32Clk))
    {
        _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        _TSP_RETURN(DRVTSP_FAIL);
    }

#else
        return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_PVR
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

    *pbIsStart = HAL_TSP_PVR_Eng_IsEnabled(u32PvrId);

    return DRVTSP_OK;
}

// File API
//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream start address
/// @param  u32StreamAddr           \b IN: pointer of transport stream in memory
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetAddr(MS_PHY phyStreamAddr)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();
    HAL_TSP_CmdQ_TsDma_SetAddr(phyStreamAddr);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}


//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream data size
/// @param  u32StreamSize           \b IN: size of transport stream data to be copied
/// @return TSP_Result
//  @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetSize(MS_U32 u32StreamSize)
{
#ifndef NO_FILEIN

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

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
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
    //HAL_TSP_SetSTC_32(u32EngId, u32STC_32);
    _TSP_RETURN(DRVTSP_OK);
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
/// Memory to TSP stream control : Start stream input
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
/// @note Filein Command Size: 1
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Start(DrvTSP_FileinMode eFileinMode)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();
    // @FIXME: Good to assume enum and register definition identical?
    //if (E_DRVTSP_FILEIN_MODE_ENG0_TS != eFileinMode)
    {
        HAL_TSP_PS_Path_Enable((MS_U32)eFileinMode);
    }
    HAL_TSP_CmdQ_TsDma_Start((MS_U32)eFileinMode);

    HAL_TSP_filein_enable(TRUE); //enable cmdQ after cmds are set

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stop stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Stop(void)
{
#ifndef NO_FILEIN

    #ifndef NO_CA
    MS_U32 u32data;
    #endif

    _TSP_ENTRY();

    HAL_TSP_filein_enable(FALSE);

    HAL_TSP_PS_Path_Disable();

    if(!_ptsp_res->_bLiveSING_FilePP)
    {
        HAL_TSP_DoubleBuf_Disable();
    }

    #ifndef NO_CA
    u32data = HAL_TSP_CSA_Get_ScrmPath();
    if(u32data & TSP_CA_INPUT_TSIF0_FILEIN)
    {
        HAL_TSP_CSA_Set_ScrmPath(_ptsp_res->_u32OrgCAMode); //restore original CA Ctrl
    }
    else
    {
        if(_ptsp_res->TSPPvrcaParams._bCIPHSSMode)
        {
            HAL_TSP_CSA_Set_ScrmPath(u32data | TSP_CA_OUTPUT_PLAY_FILE);
            HAL_TSP_LiveAB_En(TRUE);
        }
        else if(_ptsp_res->TSPPvrcaParams._bFLT1Dup_Enable == TRUE || (_ptsp_res->_u32OrgCAMode & TSP_CA_OUTPUT_PLAY_LIVE)) //recover live0 display only in PVRCA flow
        {
            HAL_TSP_CSA_Set_ScrmPath(u32data | TSP_CA_OUTPUT_PLAY_LIVE);
        }
    }
    #endif

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : pause stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Pause(void)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();
    HAL_TSP_TsDma_Pause();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}


//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : resume stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_Resume(void)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();
    HAL_TSP_TsDma_Resume();
    _TSP_RETURN(DRVTSP_OK);

#else
            return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}


//-------------------------------------------------------------------------------------------------
/// Get current file in state
/// @return DrvTSP_M2tStatus
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_GetState(DrvTSP_FileinState* peFileinState)
{
#ifndef NO_FILEIN

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

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
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
#ifndef NO_FILEIN

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

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}


//-------------------------------------------------------------------------------------------------
/// Set packet size to TSP file
/// @param  PacketMode                 \b IN: Mode of TSP file packet mode (192, 204, 188)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_SetPacketMode(DrvTSP_PacketMode PKT_Mode)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();

    HAL_TSP_filein_enable(FALSE); //disable cmdQ before cmds are set

    switch (PKT_Mode)
    {
        case E_DRVTSP_PKTMODE_188:
            HAL_TSP_SetPKTSize(0xBB);
            break;
        case E_DRVTSP_PKTMODE_192:
            HAL_TSP_SetPKTSize(0xBF);
            break;
        case E_DRVTSP_PKTMODE_204:
            HAL_TSP_SetPKTSize(0xCB);
            break;
        case E_DRVTSP_PKTMODE_130:
            HAL_TSP_SetPKTSize(0x82);
            break;
        case E_DRVTSP_PKTMODE_134:
            HAL_TSP_SetPKTSize(0x86);
            break;
        default:
            break;
    }

    HAL_TSP_PS_Path_Disable();

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
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
#ifndef NO_FILEIN

    _TSP_ENTRY();
    *pu32EmptySlot = HAL_TSP_CmdQ_EmptyCount();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
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
#ifndef NO_FILEIN

    MS_BOOL bRet;

    _TSP_ENTRY();
    bRet = HAL_TSP_CmdQ_Reset();
    if (TRUE == bRet)
    {
         _TSP_RETURN(DRVTSP_OK);
    }
    _TSP_RETURN(DRVTSP_FAIL);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

//--------------------------------------------------------------------------------------------------
/// Get command queue fifo level
/// @param  pu8FifoLevel      \b OUT: fifo level, 0~3
/// @return TSP_Result
/// @note
//--------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();

    *pu8FifoLevel = HAL_TSP_Get_CmdQFifoLevel();

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

//-------------------------------------------------------------------------------------------------
/// Enable timestamp block scheme
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();
    HAL_TSP_FileIn_192BlockScheme_En(bEnable);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

//-------------------------------------------------------------------------------------------------
/// Enable PS path
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_PS_Path_Enable(DrvTSP_FileinMode eFileinMode)
{
#ifndef NO_FILEIN

    _TSP_ENTRY();
    HAL_TSP_PS_Path_Enable((MS_U32)eFileinMode);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

//-------------------------------------------------------------------------------------------------
/// Enable PS path
/// @param  pu32ReadAddr                \b OUT: Pointer to store file-in current reand pointer
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_GetReadAddr(MS_PHY *pphyReadAddr)
{
#ifndef NO_FILEIN
    _TSP_ENTRY();
    if(!HAL_TSP_GetFilinReadAddr(pphyReadAddr))
    {
        _TSP_RETURN(DRVTSP_NOT_SUPPORTED);
    }
    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
}

// Legacy
//-------------------------------------------------------------------------------------------------
/// Get Scmb states
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pScmbLevel              \b IN: TS interface setting
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Scmb_Status(MS_U32 u32EngId, DrvTSP_Scmb_Level* pScmbLevel)
{
    _TSP_ENTRY();
    *pScmbLevel=        E_DRVTSP_SCMB_NONE;

    if (HAL_TSP_Scmb_Status(0, 0, 0xFFFFFFFF) || HAL_TSP_Scmb_Status(1, 0, 0xFFFFFFFF)
        || HAL_TSP_Scmb_Status(0, 1, 0xFFFFFFFF) || HAL_TSP_Scmb_Status(1, 1, 0xFFFFFFFF))
    {
        *pScmbLevel=    E_DRVTSP_SCMB_TS;
    }
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get/Set  Dscrambling path
/// @param  u32EngId                 \b IN: index of DSCMB engine
/// @param  pScmbPath               \b IN: pointer to store dscmb path
/// @param  bSet                        \b IN: If TURE, setting scmb pathl if false, getting scmb path
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Dscmb_Path(MS_U32 u32EngId, DrvTSP_DscmbCtrl *pScmbPath, MS_BOOL bSet)
{
    MS_U32 u32ii = 0;
    MS_BOOL bFlt0DupOld = FALSE, bFlt0DupNew = FALSE, bFlt2DupOld = FALSE, bFlt2DupNew = FALSE;

    if(u32EngId > 0)
    {
        return DRVTSP_FAIL;
    }

    _TSP_ENTRY();

    if(bSet == FALSE)
    {
        *pScmbPath = (DrvTSP_DscmbCtrl)HAL_TSP_CSA_Get_ScrmPath();
    }
    else
    {
        _ptsp_res->_u32OrgCAMode = HAL_TSP_CSA_Get_ScrmPath(); //record old CA Ctrl mode

        bFlt0DupOld = ((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE));
        bFlt0DupNew = ((*pScmbPath & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE));
        if((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_REC)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_REC))
            bFlt0DupOld = TRUE;
        #ifdef TS2_LIVE_SUPPORT
        bFlt2DupOld = ((_ptsp_res->_u32OrgCAMode & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2));
        bFlt2DupNew = ((*pScmbPath & (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE|TSP_CA_OUTPUT_PLAY_LIVE2)) == (TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE2));
        #endif

        if((bFlt0DupOld == TRUE) && (bFlt0DupNew == FALSE))
        {
            _TSP_FreeDupDscmbPidFlt(u32EngId, 0, _ptsp_res->_u32OrgCAMode);
        }
        if((bFlt2DupOld == TRUE) && (bFlt2DupNew == FALSE))
        {
            _TSP_FreeDupDscmbPidFlt(u32EngId, 2, _ptsp_res->_u32OrgCAMode);
        }

        //duplicateFLT 0 to FLT1
        if(bFlt0DupNew)
        {
            for(u32ii = DSCMB_FLT_START_ID; u32ii <= DSCMB_FLT_END_ID; u32ii++)
            {
                _TSP_DupDscmbPidFlt(u32EngId, u32ii, TSP_SECFLT_NUM+u32ii-DSCMB_FLT_START_ID);
            }
            #ifdef EXT_PVR1_FLT
            for(u32ii = DSCMB_FLT_SHAREKEY_START_ID; u32ii <= DSCMB_FLT_SHAREKEY_END_ID; u32ii++)
            {
                _TSP_DupDscmbPidFlt(u32EngId, u32ii, TSP_SECFLT_NUM+DSCMB_FLT_NUM+u32ii);
            }
            #endif
        }

        //duplicateFLT 2 to FLT1
        #ifdef TS2_LIVE_SUPPORT
        if(bFlt2DupNew)
        {
            for(u32ii = (TSP_SECFLT_NUM + TSP_PIDFLT1_NUM); u32ii < TSP_PIDFLT_NUM_ALL; u32ii++)
            {
                _TSP_DupDscmbPidFlt(u32EngId, u32ii, u32ii-TSP_PIDFLT1_NUM);
            }
        }
        #endif

        HAL_TSP_CSA_Set_ScrmPath((DrvTSP_DscmbCtrl)(*pScmbPath));
    }

    _TSP_RETURN(DRVTSP_OK);

}

TSP_Result MDrv_TSP_GetCap(DrvTSP_Cap eCap, void* pOutput)
{
    *((MS_U32*)pOutput) = 0;

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
    _u32TSPDGBLevel = DbgLevel;
    //TSP_DBG("[%s][%d] _u32TSPDGBLevel %d \n", __FUNCTION__, __LINE__, (int)_u32TSPDGBLevel);

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_GetTSIFStatus(DrvTSP_If eIf, DrvTSP_If_Set* pIfSet, MS_U16 u16Clock)
{
    TSP_Result Res = DRVTSP_OK;
    MS_U16 u16pad, u16clk;

    _TSP_ENTRY();

    switch(eIf)
    {
        case E_DRVTSP_IF_PLAYBACK:
            HAL_TSP_GetTSIF_Status(0, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
#ifndef NO_PVR
        case E_DRVTSP_IF_PVR0:
            HAL_TSP_GetTSIF_Status(1, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
#endif //udef NO_PVR
#ifdef TS2_IF_SUPPORT
        case E_DRVTSP_IF_PVR1:
            HAL_TSP_GetTSIF_Status(2, &u16pad, &u16clk, &(pIfSet->bExtSync), &(pIfSet->bParallel));
            break;
#endif
        default:
            _TSP_RETURN(DRVTSP_NOT_SUPPORTED);
    }

    u16Clock = u16clk;

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
        case TSP_MUX_TS1:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS1)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT1;
            break;
#ifdef TS2_IF_SUPPORT
        case TSP_MUX_TS2:
            if((u16clk & CLKGEN0_TSP_CLK_MASK) != TSP_CLK_TS2)
                Res = DRVTSP_FUNC_ERROR;
            pIfSet->ePad = E_DRVTSP_PAD_EXT_INPUT2;
            break;
#endif
        default:
            break;
    }

    pIfSet->bClkInv = ((u16clk & TSP_CLK_INVERSE) == TSP_CLK_INVERSE);

    _TSP_RETURN(Res);
}

//-------------------------------------------------------------------------------------------------
/// Get the PES scrambling control status
/// @param  u8FltId                     \b IN: filter id
/// @param  pu8status                 \b OUT: pointer to store PES scrambled statis. Every bit indicates one status of one packet.
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPesScmbSts(MS_U8 u8FltId, MS_U8 *pu8status)
{
    _TSP_ENTRY();
    *pu8status = HAL_TSP_Get_PesScmb_Sts(u8FltId);
    _TSP_RETURN(DRVTSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get the TS header scrambling control status
/// @param  u8FltId                     \b IN: filter id
/// @param  pu8status                 \b OUT: pointer to store TS scrambled statis. Every bit indicates one status of one packet.
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetTsScmbSts(MS_U8 u8FltId, MS_U8 *pu8status)
{
    _TSP_ENTRY();
    *pu8status = HAL_TSP_Get_TsScmb_Sts(u8FltId);
    _TSP_RETURN(DRVTSP_OK);
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
TSP_Result MDrv_TSP_InitLibResource(void *pu32ResMemAddr)
{
    TSP_ASSERT((pu32ResMemAddr != 0),
        TSP_DBG("[TSP_ERROR][%s][%06d] pointer is 0x%p!\n", __FUNCTION__, __LINE__, pu32ResMemAddr));

    MS_VIRT virtTspBank, virtIRBank;
    MS_PHY phyIrBankSize, phyTspBankSize;

    _ptsp_res = (TSP_RESOURCE_PRIVATE*)pu32ResMemAddr;

    if(_ptsp_res->_bSWInitReady != TRUE)
    {
        if(_TSP_InitResource(_ptsp_res) == FALSE)
            return DRVTSP_FAIL;
    }

    // For multi-process use case. (different process should set the value of bank again)
    if(_bBankInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&virtTspBank, &phyTspBankSize, MS_MODULE_TSP))
        {
            MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
            MS_ASSERT(0);
        }
        if (FALSE == MDrv_MMIO_GetBASE(&virtIRBank, &phyIrBankSize, MS_MODULE_IR))
        {
            MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE (PM base)fail\n"));
            MS_ASSERT(0);
        }

        HAL_TSP_SetBank(virtTspBank, virtIRBank);
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
    TSP_Result bRet;

    MS_DEBUG_MSG(TSP_DBG ("******************start TSP init*********************************\n"));

    //if already init, set the same HK parameter & interrupt num to this process, and attached Isr for this new process
    if((_ptsp_res->_bSWInitReady == TRUE) && (_s32TspTaskId != -1))
    {
        return MDrv_TSP_Reinit(_bIsHK);
    }

    MS_DEBUG_MSG(TSP_DBG ("******************start TSP init*********************************\n"));

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
    MS_VIRT virtTspBank, virtIRBank;
    MS_PHY u32IrBankSize, u32TspBankSize;
    MS_U8  u8ii = 0;

    _bIsHK = bHK;
    _u32IntNum = (bHK) ? E_INT_IRQ_TSP2HK : E_INT_FIQ_TSP2AEON; // use this once MHEG5 using utopia msos

    if (FALSE == MDrv_MMIO_GetBASE(&virtTspBank, &u32TspBankSize, MS_MODULE_TSP))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE fail\n"));
        MS_ASSERT(0);
    }
    if (FALSE == MDrv_MMIO_GetBASE(&virtIRBank, &u32IrBankSize, MS_MODULE_IR))
    {
        MS_CRITICAL_MSG(TSP_DBG("MDrv_MMIO_GetBASE (PM base)fail\n"));
        MS_ASSERT(0);
    }

    HAL_TSP_SetBank(virtTspBank, virtIRBank);


#ifdef STC64_SUPPORT
    HAL_TSP_STC64_Mode_En(TRUE);
#endif

    _s32ExtMutexId = OSAL_TSP_MutexCreate((MS_U8*)tsp_ext_mutex);
    _s32TspEventId = OSAL_TSP_EventCreate((MS_U8*)tsp_event); // MsOS_CreateEventGroup("TSP_Event");

    for (u8ii = 0; u8ii < ((TSP_SECFLT_NUM+31)>>5); u8ii++)
    {
        _s32TspSecRdyId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_secRdy);
        _s32TspSecOvfId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_crcerr);
        _s32TspSecCrcId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_secovf);
        _s32TspFltDisableId[u8ii] = OSAL_TSP_EventCreate((MS_U8*)tsp_fltdis);
        _u32SecNotifyId[u8ii] = 0;
    }

    #if 0
    for (u8ii = 0; u8ii < ((TSP_PIDFLT_NUM_ALL+31)>>5); u8ii++)
    {
        _s32EventFree0[u8ii] = OSAL_TSP_EventCreate((MS_U8*)freelist0);
        _s32EventFreeDefer0[u8ii] = OSAL_TSP_EventCreate((MS_U8*)freelistdefer0);
    }
    #endif

#if (defined(HWPCR_ENABLE) && (TSP_HWPCR_BY_HK == 1))
    for (u8ii = 0; u8ii < ((TSP_PCRFLT_NUM+31)>>5); u8ii++)
    {
        _u32HwPcrNotifyId[u8ii] = 0;
    }
#endif

#ifndef NO_PVR
    for (u8ii = 0; u8ii < ((TSP_PVR_IF_NUM+31)>>5); u8ii++)
    {
        _u32PvrNotifyId[u8ii] = 0;
    }
#endif

    #ifndef MSOS_TYPE_LINUX_KERNEL
    _s32TspTaskId = OSAL_TSP_TaskCreate(_TSP_Isr_Proc_Task, (MS_U8*)tsp_isr_task);
    #endif

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
#ifndef NO_FILEIN

    _TSP_ENTRY();
    *u32TSLen = HAL_TSP_GetFileInTimeStamp();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif //udef NO_FILEIN
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
    _bIsHK = bIsHK;
    _u32IntNum = (bIsHK) ? E_INT_IRQ_TSP2HK : E_INT_FIQ_TSP2AEON; // use this once MHEG5 using utopia msos
    return DRVTSP_OK;
}
//[LEGACY] //[OBSOLETE]

TSP_Result MDrv_TSP_SetAVPause(MS_BOOL bSet)
{
    _TSP_ENTRY();

#ifndef NO_CA
    HAL_TSP_Set_AVPAUSE(bSet);
#endif

    _TSP_RETURN(DRVTSP_OK);
}

TSP_Result MDrv_TSP_FLT_LiveSrcSwitch(DrvTSP_FltType eFltSrcType)
{
#ifndef NO_PVR
    MS_U16 u16pad, u16clk = 0;
    MS_BOOL bParall = TRUE, bExtSync = TRUE;
    DrvTSP_PadIn epad = E_DRVTSP_PAD_DEMOD;

    _TSP_ENTRY();

    switch(eFltSrcType & E_DRVTSP_FLT_SOURCE_TYPE_MASK)
    {
        case E_DRVTSP_FLT_SOURCE_TYPE_LIVE:
            HAL_TSP_TSIF1_Enable(FALSE);
            #ifndef NO_CA
            HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF0_LIVEIN|TSP_CA_OUTPUT_PLAY_LIVE);
            #endif
            break;
        case E_DRVTSP_FLT_SOURCE_TYPE_TS1:
            HAL_TSP_GetTSIF_Status(0, &u16pad, &u16clk, &bParall, &bExtSync);
            if(u16pad == TSP_MUX_TS0)
            {
                epad = E_DRVTSP_PAD_EXT_INPUT0;
            }
            else if(u16pad == TSP_MUX_TS1)
            {
                epad = E_DRVTSP_PAD_EXT_INPUT1;
            }
    #ifdef TS2_IF_SUPPORT
            else if(u16pad == TSP_MUX_TS2)
            {
                epad = E_DRVTSP_PAD_EXT_INPUT2;
            }
    #endif
            HAL_TSP_SelPad(0, 1, (MS_U32)epad);
            HAL_TSP_SelPad_ClkInv(0, 1, (u16clk & TSP_CLK_INVERSE));
            HAL_TSP_SelPad_ExtSync(0, bExtSync, 1);
            HAL_TSP_SelPad_Parl(0, bParall, 1);
            #ifndef NO_CA
            HAL_TSP_CSA_Set_ScrmPath(TSP_CA_INPUT_TSIF1|TSP_CA_OUTPUT_PLAY_LIVE);
            #endif
            HAL_TSP_TSIF1_Enable(TRUE);
            break;
        default:
            _TSP_RETURN(DRVTSP_NOT_SUPPORTED);
            break;

    }

    _TSP_RETURN(DRVTSP_OK);
#endif //NO_PVR

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
#ifndef NO_FILEIN
    if(HAL_TSP_AU_BD_Mode_Enable(bEnable))
    {
        return DRVTSP_OK;
    }
    else
#endif
    {
        return DRVTSP_NOT_SUPPORTED;
    }
}

TSP_Result MDrv_TSP_Get_FW_VER(MS_U32* u32FWVer)
{
    MS_U32 _u32fw;
    _u32fw = HAL_TSP_Get_FW_VER();
    *u32FWVer = _u32fw;
    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Get_Intr_Count(MS_U32* pu32Count)
{
    *pu32Count = _ptsp_res->_u32DbgIntrCount;
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
        case E_DRVTSP_IF_PLAYBACK:
            HAL_TSP_TSIF0_Enable(bEnable);
            break;
        case E_DRVTSP_IF_PVR0:
            HAL_TSP_TSIF1_Enable(bEnable);
            break;
#ifdef DTS2_IF_SUPPORT
        case E_DRVTSP_IF_PVR1:
            HAL_TSP_TSIF2_Enable(bEnable);
            break;
#endif
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
static void _DumpFwMsg(void)
{
    int tmp = 0;
    int len = 0;
    char p8buf[256];
    char* p = NULL;
    MS_U32 u32WritePos = *(volatile MS_U32*)MS_PA2KSEG1(_FwDbgInfo.phyBufAddr);

    p8buf[255] = '\0';
    p = (char*)(MS_PA2KSEG1(_FwDbgInfo.phyBufAddr) + _FwDbgInfo.u32ReadPos);
    //ULOGD("TSP", "_pFwDbgInfo->phyBufAddr = 0x%x\n", (unsigned int)_FwDbgInfo.phyBufAddr);
    //ULOGD("TSP", "_pFwDbgInfo->u32ReadPos = 0x%x\n", (unsigned int)_FwDbgInfo.u32ReadPos);
    //ULOGD("TSP", "_pFwDbgInfo->u32WritePos = 0x%x\n", (unsigned int)u32WritePos);
    if(u32WritePos > _FwDbgInfo.u32Size)
    {
        TSP_DBG("[%s], WARN : write pointer incorrect!!!\n", __FUNCTION__);
        return;
    }
    if(u32WritePos > _FwDbgInfo.u32ReadPos)
    {
        while(1)
        {
            len = (u32WritePos - _FwDbgInfo.u32ReadPos) > 255 ? 255 : (u32WritePos - _FwDbgInfo.u32ReadPos);
            if((tmp = snprintf(p8buf, len, "%s", p)) == 0)
                break;
            tmp++;
            p+=tmp;
            _FwDbgInfo.u32ReadPos += tmp;
            ULOGD("TSP", "[TSP] %s", p8buf);
        }
    }
    else if(u32WritePos < _FwDbgInfo.u32ReadPos)
    {
        while(1)
        {
            len = (_FwDbgInfo.u32Size - _FwDbgInfo.u32ReadPos) > 255? 255 : (_FwDbgInfo.u32Size - _FwDbgInfo.u32ReadPos);
            if((tmp = snprintf(p8buf, len, "%s", p)) == 0)
            {
                break;
            }
            tmp++;
            p+=tmp;
            _FwDbgInfo.u32ReadPos += tmp;
            ULOGD("TSP", "[TSP] %s", p8buf);
        }

        _FwDbgInfo.u32ReadPos = sizeof(MS_U32);
        p = (char*)(MS_PA2KSEG1(_FwDbgInfo.phyBufAddr) + _FwDbgInfo.u32ReadPos);

        while(1)
        {
            len = (u32WritePos - _FwDbgInfo.u32ReadPos) > 255 ? 255 : (u32WritePos - _FwDbgInfo.u32ReadPos);
            if((tmp = snprintf(p8buf, len, "%s", p)) == 0)
            {
                break;
            }
            tmp++;
            p+=tmp;
            _FwDbgInfo.u32ReadPos += tmp;
            ULOGD("TSP", "[TSP] %s", p8buf);
        }
    }
}

TSP_Result MDrv_TSP_SetFwDBGParam(MS_PHY phyAddr, MS_U32 u32Size, MS_U32 u32DbgWord)
{
    TSP_ASSERT((phyAddr != 0) && (u32Size != 0),
        TSP_DBG("[TSP_ERROR][%06d] memory information for debug buffer incorrect address : %lx, size : %d\n", __LINE__, (unsigned long)phyAddr, (unsigned int)u32Size));


    _TSP_ENTRY();

    HAL_TSP_OrzWriteProtect_Enable(FALSE, 0, 0);
    if(HAL_TSP_SetFwDbgMem(phyAddr, u32Size) && HAL_TSP_SetFwDbgWord(u32DbgWord))
    {
            _FwDbgInfo.bEnable = TRUE;
            _FwDbgInfo.phyBufAddr       = phyAddr;
            _FwDbgInfo.u32Size          = u32Size;
            _FwDbgInfo.u32ReadPos      = sizeof(MS_U32);
            _TSP_RETURN(DRVTSP_OK);
    }
    else
    {
        TSP_DBG("********HAL_TSP_SetFwDbgMem fail**************\n");
        _TSP_RETURN(DRVTSP_FAIL);
    }
}

#if 0
TSP_Result MDrv_TSP_ResetFileinTimestamp(void)
{
    HAL_TSP_ResetTimeStamp();
    return DRVTSP_OK;
}
#endif

//-------------------------------------------------------------------------------------------------
/// Set Virtual Queue buffer
/// @param  phyVQBaseAddr                \b IN: Virtual Queue memorry address
/// @param  u32VQBufLen_192PKT        \b IN: Number of packets for Virtual Queue (192 bytes per packet)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetVQueBuf(MS_PHY phyVQBaseAddr, MS_U32 u32VQBufLen)
{
#ifdef VQ_ENABLE
    _TSP_ENTRY();
    HAL_TSP_SetVQBuffer(phyVQBaseAddr, u32VQBufLen);
    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable Virtual Quere
/// @param  bEnable                \b IN: Virtual Queue memorry Enable flag
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_VQueEnable(MS_BOOL bEnable)
{
#ifdef VQ_ENABLE

    _TSP_ENTRY();
    HAL_TSP_VQueue_Enable(bEnable);

    if(!bEnable)
    {
        //HAL_TSP_SetVQBuffer(0, 0);
        HAL_TSP_VQueue_Reset();
    }

    #ifndef FIVQ_ENABLE
    _ptsp_res->_bLiveSING_FilePP = bEnable;
    HAL_TSP_DoubleBuf_En(!_ptsp_res->_bLiveSING_FilePP);
    #endif

    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Reset Virtual Quere
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_VQueReset(void)
{
#ifdef VQ_ENABLE

    _TSP_ENTRY();
    HAL_TSP_VQueue_Reset();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

TSP_Result MDrv_TSP_VQueue_OverflowInt_En(MS_BOOL bEnable)
{
#ifdef VQ_ENABLE

    _TSP_ENTRY();
    HAL_TSP_VQueue_OverflowInt_En(bEnable);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Virtual Quere request threshold
/// @param  u8req_len                \b IN: Request length
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Set_VQ_Threshold(MS_U8 u8req_len)
{
#ifdef VQ_ENABLE

    _TSP_ENTRY();
    HAL_TSP_Set_Req_VQ_RX_Threshold(u8req_len);
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

TSP_Result MDrv_TSP_Get_VQStatus(MS_U32* pu32Status)
{
#ifdef VQ_ENABLE

    _TSP_ENTRY();
    *pu32Status = HAL_TSP_Get_VQStatus();
    _TSP_RETURN(DRVTSP_OK);

#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF encrypt key
/// @param  u32Key0                \b IN: Encrypt Key0
/// @param  u32Key1                \b IN: Encrypt Key1
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Set_MOBF_PVRKey(MS_U32 u32Key0, MS_U32 u32Key1)
{
#ifdef MOBF_ENABLE
    _TSP_ENTRY();
    HAL_TSP_MOBF_Set_PVRKey(u32Key0, u32Key1);
    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set MOBF encrypt key
/// @param  u32Key0                \b IN: Encrypt Key0
/// @param  u32Key1                \b IN: Encrypt Key1
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Set_MOBF_PVR1Key(MS_U32 u32Key0, MS_U32 u32Key1)
{
#ifdef MOBF_ENABLE
    _TSP_ENTRY();
    HAL_TSP_MOBF_Set_PVRKey_EX(1, u32Key0, u32Key1);
    _TSP_RETURN(DRVTSP_OK);
#else
    return DRVTSP_NOT_SUPPORTED;
#endif
}


//-------------------------------------------------------------------------------------------------
/// Set MOBF decrypt key
/// @param  u32Key                \b IN: Decrypt Key
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Set_MOBF_FileinKey(MS_U32 u32Key)
{
#ifdef MOBF_ENABLE
    _TSP_ENTRY();
    HAL_TSP_MOBF_Set_FileinKey(u32Key);
    _TSP_RETURN(DRVTSP_OK);
#else
        return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable MOBF encrypt mode
/// @param  benable                \b IN: Enable flag
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_MOBF_PVR_Enable(MS_BOOL benable)
{
#ifdef MOBF_ENABLE
    _TSP_ENTRY();
    HAL_TSP_MOBF_PVR_Enable(benable);
    _TSP_RETURN(DRVTSP_OK);
#else
            return DRVTSP_NOT_SUPPORTED;
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enable/Disable MOBF decrypt mode
/// @param  benable                \b IN: Enable flag
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_MOBF_FileIn_Enable(MS_BOOL benable)
{
#ifdef MOBF_ENABLE
    _TSP_ENTRY();
    HAL_TSP_MOBF_Filein_Enable(benable);
    _TSP_RETURN(DRVTSP_OK);
#else
            return DRVTSP_NOT_SUPPORTED;
#endif
}

TSP_Result MDrv_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData)
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

TSP_Result MDrv_TSP_SetFwDataAddr(MS_PHY phyDataAddr, MS_U32 u32Size)
{
    TSP_ASSERT((phyDataAddr & _TSP_QMEM_D_MASK),
        TSP_DBG("[TSP_ERROR][%06d] setFwDataAddr 0x%x <= 0x%x\n", __LINE__, (unsigned int)phyDataAddr, (unsigned int)(~_TSP_QMEM_D_MASK)));

    _TSP_ENTRY();

    HAL_TSP_OrzWriteProtect_Enable(TRUE, phyDataAddr, phyDataAddr+u32Size);
    HAL_TSP_SecFlt_SetDataAddr(phyDataAddr);
    _ptsp_res->_phyTspFWDramBuf = phyDataAddr;
    _ptsp_res->_u32TspFwDramBufSize = u32Size;
    _TSP_RETURN(DRVTSP_OK);
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
    TSP_DisconPktCnt_Info TspDropPktCntInfo;

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

//-------------------------------------------------------------------------------------------------------
// Refer to _TSP_Isr_ProcSelf, this is for user mode to getting events when using kernel dmx driver
// Plese update both  _TSP_Isr_ProcSelf & MDrv_TSP_Get_IoSignal at the same time for synchrony ISR evets processing flow
//-------------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Wait_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 u32ii, u32Evt = 0;

    for(u32ii = 0; u32ii < TSP_MAX_PVRSIGFLAG_NUM; u32ii++)
    {
        pstIoSignal->u32PvrBufFullFlag[u32ii] = 0;
    }
    for(u32ii = 0; u32ii < TSP_MAX_SIGFLAG_NUM; u32ii++)
    {
        pstIoSignal->u32SecRdyFlag[u32ii] = 0;
        pstIoSignal->u32SecOvfFlag[u32ii] = 0;
    }

    _TSP_GetEvent(u32Evt, TSP_TASK_EVENT_GROUP_SELF|TSP_TASK_EVENT_FLT_FREE, OSAL_EVENT_OR_CLEAR, MSOS_WAIT_FOREVER);
    pstIoSignal->u32TspEvt = u32Evt;
    //ULOGD("TSP", "u32TspEvt %lx\n", pstIoSignal->u32TspEvt);

#endif  //end of MSOS_TYPE_LINUX_KERNEL

    return DRVTSP_OK;
}

TSP_Result MDrv_TSP_Proc_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{

#ifdef MSOS_TYPE_LINUX_KERNEL
    TSP_Result Ret = DRVTSP_OK;
    MS_U32 u32ii, u32jj;
    MS_U32 u32GroupNum = (TSP_SECFLT_NUM_All + 31) >> 5;

    if(pstIoSignal->u32TspEvt & TSP_TASK_EVENT_FLT_FREE)
    {
        _TSP_Isr_ProcSelf(TSP_TASK_EVENT_FLT_FREE, &Ret);
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
                        pstIoSignal->u32PvrBufFullFlag[0] = 1;
                    }
                    break;
                case TSP_TASK_EVENT_PVR1_RDY_SELF:
                case TSP_TASK_EVENT_PVR1_RDY_POLL:
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_FULL))
                    {
                        pstIoSignal->u32PvrBufFullFlag[0] |= 2;
                    }
                    break;
                case (TSP_TASK_EVENT_PVR0_RDY_SELF | TSP_TASK_EVENT_PVR1_RDY_SELF):
                case (TSP_TASK_EVENT_PVR0_RDY_POLL | TSP_TASK_EVENT_PVR1_RDY_POLL):
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[0], E_DRVTSP_EVENT_PVRBUF_OVERFLOW))
                    {
                        pstIoSignal->u32PvrBufFullFlag[0] = 0xFF;
                    }
                    break;
                default:
                    break;
            }
        }
    }

#ifdef TS2_IF_SUPPORT
    if (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_CBPVR0_RDY_POLL|TSP_TASK_EVENT_CBPVR0_RDY_SELF))
    {
        if (_ptsp_res->_PvrEvtNotify[1] && _ptsp_res->_PvrCallback[1])
        {
            switch (pstIoSignal->u32TspEvt & (TSP_TASK_EVENT_CBPVR0_RDY_POLL | TSP_TASK_EVENT_CBPVR0_RDY_SELF))
            {
                case TSP_TASK_EVENT_CBPVR0_RDY_POLL:
                case TSP_TASK_EVENT_CBPVR0_RDY_SELF:
                    if (HAS_FLAG(_ptsp_res->_PvrEvtNotify[1], E_DRVTSP_EVENT_CBBUF_FULL))
                    {
                        pstIoSignal->u32PvrBufFullFlag[1] = 1;
                    }
                    break;
            }
        }
    }
#endif //endof TS2_IF_SUPPORT

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

    _TSP_GetModeSecRdy(u32Group, 0xFFFFFFFF, &u32RdyMask, OSAL_EVENT_OR_CLEAR);
    _TSP_GetModeSecOvf(u32Group, 0xFFFFFFFF, &u32OvfMask, OSAL_EVENT_OR_CLEAR);

    u32SecFlt = u32Group << 5;
    while(u32RdyMask)
    {
        if(u32RdyMask & 0x01)
        {
            if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltMap[u32SecFlt] != u32SecFlt)
            {
                for(u32PidFlt = 0; u32PidFlt < TSP_PIDFLT_NUM_ALL; u32PidFlt++)
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
                    u32PidFltGroup = u32PidFlt >> 5;
                    pstIoSignal->u32SecRdyFlag[u32PidFltGroup] |= (1 << (u32PidFlt & 0x1F));
                }
            }
        }
        u32RdyMask >>= 1;
        u32SecFlt++;
    }

    u32SecFlt = u32Group << 5;
    while(u32OvfMask)
    {
        if(u32OvfMask & 0x01)
        {
            if(_ptsp_res->_TspState[pstIoSignal->u32Eng].FltMap[u32SecFlt] != u32SecFlt)
            {
                for(u32PidFlt = 0; u32PidFlt < TSP_PIDFLT_NUM_ALL; u32PidFlt++)
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
                    _TSP_IND_LOCK();
                    _TSP_PidFlt_Disable(pstIoSignal->u32Eng, u32PidFlt);  // ???? move out --> OK  // // critical section
                    _TSP_IND_UNLOCK();
                }
                if ((_ptsp_res->_TspState[pstIoSignal->u32Eng].FltState[u32PidFlt] != E_DRVTSP_FLT_STATE_FREE) &&
                    (_ptsp_res->_TspState[pstIoSignal->u32Eng].SecFltEvtNotify[u32SecFlt] & E_DRVTSP_EVENT_BUF_OVERFLOW) &&
                    (_ptsp_res->_TspState[pstIoSignal->u32Eng].SecFltCallback[u32SecFlt]))
                {
                    u32PidFltGroup = u32PidFlt >> 5;
                    pstIoSignal->u32SecOvfFlag[u32PidFltGroup] |= (1 << (u32PidFlt & 0x1F));
                }
            }
        }
        u32OvfMask >>= 1;
        u32SecFlt++;
    }

    return DRVTSP_OK;
}


