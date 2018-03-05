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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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
/// file    drvTSP2.c
/// @brief  Transport Stream Processer (TSP) Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSP DDI are not allowed to use in any interrupt context other than TSP ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "tsp2_msos.h"
#include "tsp2_nos.h"
#include "ULog.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#else
#include "string.h"
#endif

#ifdef SECURE_PVR_ENABLE
#include "halCHIP.h"
#include "../../drv/mbx/apiMBX_St.h"
#include "../../drv/mbx/apiMBX.h"
#include "../drv/sys/tee_client_api.h"
#include "drvSYS.h"
#endif

#if defined(MSOS_TYPE_LINUX)
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
    #include "mdrv_firmware_io.h"
#endif

#include "drvMMIO.h"

#if defined(OTV_ENABLE)
#include "drvOTV.h"
#endif

#include "drvTSP2.h"

// Internal Definition
#include "asmCPU.h"
#include "regTSP.h"
#include "halTSP.h"
#include "halCHIP.h"

#include "drvSEM.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SWITCH_TSP_UART                             0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


#define FWTSP_CHECK_CODE        "<FWTSP>"

// Blocking events
#define TSP_SetEvent(eventid, flag)                          ({ MS_BOOL bRet = TSP_OS_EventSet(eventid, (flag)); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_GetEvent(eventid, events, flag, opt, timeout)    ({ MS_BOOL bRet = TSP_OS_EventGet(eventid, (flag), (events), (opt), (timeout)); (bRet == TRUE)? TRUE : FALSE; })

// Filter event flags
#define TSP_FLAG_SetSecRdy(fltid)                   ({ MS_BOOL bRet = TSP_OS_EventSet(_s32SecRdyFlags[fltid>>5], (1<<(fltid&0x1F))); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_GetSecRdy(fltid, events, opt)      ({ MS_BOOL bRet = TSP_OS_EventGet(_s32SecRdyFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_ModeSecRdy(idx, mask,events, opt)  ({ MS_BOOL bRet = TSP_OS_EventGet(_s32SecRdyFlags[idx], mask, (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })

#define TSP_FLAG_SetCrcErr(fltid)                   ({ MS_BOOL bRet = TSP_OS_EventSet(_s32CrcErrFlags[fltid>>5], (1<<(fltid&0x1F))); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_GetCrcErr(fltid, events, opt)      ({ MS_BOOL bRet = TSP_OS_EventGet(_s32CrcErrFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_ModeCrcErr(idx,mask ,events, opt)  ({ MS_BOOL bRet = TSP_OS_EventGet(_s32CrcErrFlags[idx], mask, (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })

#define TSP_FLAG_SetSecOvf(fltid)                   ({ MS_BOOL bRet = TSP_OS_EventSet(_s32SecOvfFlags[fltid>>5], (1<<(fltid&0x1F))); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_GetSecOvf(fltid, events, opt)      ({ MS_BOOL bRet = TSP_OS_EventGet(_s32SecOvfFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_ModeSecOvf(idx,mask ,events, opt)  ({ MS_BOOL bRet = TSP_OS_EventGet(_s32SecOvfFlags[idx], mask, (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })

#define TSP_FLAG_SetFltDis(fltid)                   ({ MS_BOOL bRet = TSP_OS_EventSet(_s32FltDisFlags[fltid>>5], (1<<(fltid&0x1F))); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_GetFltDis(fltid, events, opt)      ({ MS_BOOL bRet = TSP_OS_EventGet(_s32FltDisFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_ModeFltDis(idx, mask,events, opt)  ({ MS_BOOL bRet = TSP_OS_EventGet(_s32FltDisFlags[idx], mask, (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })

#define TSP_FLAG_FREE_TIMEOUT                       8
#define TSP_FLAG_SetFree(fltid)                     ({ MS_BOOL bRet = TSP_OS_EventSet(_s32FreeFlags[fltid>>5], (1<<(fltid&0x1F))); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_GetFree(fltid, events, opt)        ({ MS_BOOL bRet = TSP_OS_EventGet(_s32FreeFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_ModeFree(idx, mask,events, opt)    ({ MS_BOOL bRet = TSP_OS_EventGet(_s32FreeFlags[idx], mask, (events), (opt), 0); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_SetDone(fltid)                     ({ MS_BOOL bRet = TSP_OS_EventSet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F))); (bRet == TRUE)? TRUE : FALSE; })
#define TSP_FLAG_GetDone(fltid, events, opt)        ({ MS_BOOL bRet = TSP_OS_EventGet(_s32DoneFlags[fltid>>5], (1<<(fltid&0x1F)), (events), (opt), TSP_FLAG_FREE_TIMEOUT); (bRet == TRUE)? TRUE : FALSE; })

#define TSP_FLAG_SetModeFlags(fltid)                { _s32ModeFlags[fltid>>5] |= (1<<(fltid&0x1F)) ;}
#define TSP_FLAG_GetModeFlags(idx ,flags)           {  flags = _s32ModeFlags[idx] ; }
#define TSP_FLAG_ClrModeFlags(fltid)                { _s32ModeFlags[fltid>>5] &= (~(1<<(fltid&0x1F))); }

#define TSP_FLAG_SetOvfRst(fltid)                   (_s32OvfRstFlags[fltid>>5] |= (1<<(fltid&0x1F)))
#define TSP_FLAG_GetOvfRst(fltid)                   ((_s32OvfRstFlags[fltid>>5] & (1<<(fltid&0x1F)))?TRUE:FALSE )
#define TSP_FLAG_ClrOvfRst(fltid)                   (_s32OvfRstFlags[fltid>>5] &= (~(1<<(fltid&0x1F))))

// for mutiple process using, record which filter is enabled by this process
#define _TSP_SetSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5)] |= (1 << (fltid&0x1F)))
#define _TSP_RemoveSecNotifyId(fltid)                       (_u32SecNotifyId[(fltid>>5)] &= ~(1 << (fltid&0x1F)))
#define _TSP_ChkSecNotifyId(fltid)                          (_u32SecNotifyId[(fltid>>5)] & (1 << (fltid&0x1F)))

// for mutiple process using, record which pvr engine is enabled by this process
#define _TSP_SetPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5)] |= (1 << (fltid&0x1F)))
#define _TSP_RemovePvrNotifyId(fltid)                       (_u32PvrNotifyId[(fltid>>5)] &= ~(1 << (fltid&0x1F)))
#define _TSP_ChkPvrNotifyId(fltid)                          (_u32PvrNotifyId[(fltid>>5)] & (1 << (fltid&0x1F)))

// for mutiple process using, record which hw pcr is enabled by this process
#define _TSP_SetHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5)] |= (1 << (fltid&0x1F)))
#define _TSP_RemoveHwPcrNotifyId(fltid)                     (_u32HwPcrNotifyId[(fltid>>5)] &= ~(1 << (fltid&0x1F)))
#define _TSP_ChkHwPcrNotifyId(fltid)                        (_u32HwPcrNotifyId[(fltid>>5)] & (1 << (fltid&0x1F)))

#define TSP_ENTRY()                                 if (!TSP_OS_MutexObtain(_ptsp_res->_s32MutexId, TSP_OS_MUTEX_TIMEOUT)) \
                                                    {                                                           \
                                                        _ptsp_res->_tspInfo.LastErr = E_TSP_FAIL_FUNCTION;                 \
                                                        return E_TSP_FAIL_FUNCTION;                             \
                                                    }                                                           \
                                                    TSP_LOCK();                                                 \



#define TSP_RETURN(_ret)                            TSP_UNLOCK();                                       \
                                                    TSP_OS_MutexRelease(_ptsp_res->_s32MutexId);                   \
                                                    if (_ret != E_TSP_OK) { _ptsp_res->_tspInfo.LastErr = _ret; }  \
                                                    return _ret; // Should be optimized by compiler

// @TODO: Jerry
// Review and check the rule of LOCK
#define TSP_LOCK()                                  TSP_OS_MutexObtain(_ptsp_res->_s32LockId, TSP_OS_MUTEX_TIMEOUT); \
                                                    MDrv_SEM_Lock(E_SEM_CA , MSOS_WAIT_FOREVER)
                                                    // if you got compile error for E_SEM_CA, you should update your drvSEM
                                                    // to CL#817352 CL provide by johnson.chen
#define TSP_UNLOCK()                                MDrv_SEM_Unlock(E_SEM_CA); \
                                                    TSP_OS_MutexRelease(_ptsp_res->_s32LockId)
#ifdef MSOS_TYPE_LINUX_KERNEL
#define _TSP_INT_LOCK()                             spin_lock_irqsave(&_ptsp_res->_s32IntRegMutexId, _ptsp_res->_IntRegFlags)
#define _TSP_INT_UNLOCK()                           spin_unlock_irqrestore(&_ptsp_res->_s32IntRegMutexId,_ptsp_res->_IntRegFlags)
#else
#define _TSP_INT_LOCK()                             TSP_OS_MutexObtain(_ptsp_res->_s32IntRegMutexId, TSP_OS_MUTEX_TIMEOUT)
#define _TSP_INT_UNLOCK()                           TSP_OS_MutexRelease(_ptsp_res->_s32IntRegMutexId)
#endif

#define TSP_PVR_ENTRY()                             if (!TSP_OS_MutexObtain(_ptsp_res->_s32PvrMutexId, TSP_OS_MUTEX_TIMEOUT)) \
                                                    {                                                              \
                                                        _ptsp_res->_tspInfo.LastErr = E_TSP_FAIL_FUNCTION;                    \
                                                        return E_TSP_FAIL_FUNCTION;                                \
                                                    }

#define TSP_PVR_RETURN(_ret)                        TSP_OS_MutexRelease(_ptsp_res->_s32PvrMutexId);                   \
                                                    if (_ret != E_TSP_OK) { _ptsp_res->_tspInfo.LastErr = _ret; }     \
                                                    return _ret; // Should be optimized by compiler

#define TSP_FQ_NUM_CHECK_RET(idx)                   if( (MS_U32)idx >= (MS_U32)TSP_FQ_NUM ) \
                                                    { printf("[TSP][FQ][ERR] FQ Engine [%uld] not exist ! \n", idx);   \
                                                     return 0; }

#define TSP_FLT2BUF_NULL                            0xFFFFFFFF
#define TSP_BUF2FLT_NULL                            TSP_FLT2BUF_NULL

#define TSP_CHECK(_b, _f, _a...)                    { if (!(_b)) { TSP_OS_Print("[TSP_ASSERT]"); TSP_OS_Print(_f, ##_a); return E_TSP_FAIL;} }
#define TSP_CHECK_UNLOCK(_b, _f, _a...)             { if (!(_b)) { TSP_OS_Print("[TSP_ASSERT]"); TSP_OS_Print(_f, ##_a); TSP_UNLOCK(); return E_TSP_FAIL; } }
#define TSP_RES_ALLOC_STC_CHECK(u32StcEng)          {                                                                                                                           \
                                                        if((_ptsp_res->_bIsStcEverAlloc == TRUE) && (_ptsp_res->_stTspStcState[u32StcEng].bUsed == FALSE))                      \
                                                        {                                                                                                                       \
                                                            TSP_OS_Print("[TSP_ERROR][%s][%d]: Mix old & new STC resource allocation scheme !!\n", __FUNCTION__, __LINE__);     \
                                                        }                                                                                                                       \
                                                    }

#define TSP_RES_ALLOC_TSPPATH_CHECK(PhyTspPath)     {                                                                                                                               \
                                                        if((_ptsp_res->_bIsTspPathEverAlloc == TRUE) && (_ptsp_res->_stTspPathState[PhyTspPath].bUsed == FALSE))                    \
                                                        {                                                                                                                           \
                                                            TSP_OS_Print("[TSP_ERROR][%s][%d]: Mix old & new TSP Path resource allocation scheme !!\n", __FUNCTION__, __LINE__);    \
                                                        }                                                                                                                           \
                                                    }

#define TSP_PCRFLT_GET_ID(NUM)                      (_tspFltIdMap.u32PcrFltId_Start + (NUM))


#define TSP_DBG(_l, _f, _a...)                      { if (_u32DbgLevel >= (_l)) { TSP_OS_Print("[TSP_DBG]"); TSP_OS_Print(_f, ##_a); } }

#ifdef MS_DEBUG
#define TSP_DBG_TASK()                              { _ptsp_res->_tspInfo.u32TaskLine = __LINE__; }
#else
#define TSP_DBG_TASK()                              { }
#endif


#ifdef MSOS_TYPE_LINUX_KERNEL
#define DEMUX_EVENT_DATA_DISCARDED                   (0x00000001U)
#define DEMUX_EVENT_CRC_FAILED                       (0x00000010U)
#endif

#define TSP_NEED_SEC_FILTER(FltType)                (E_TSP_FLT_USER_SEC == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_PES == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_PKT == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_PCR == (FltType & E_TSP_FLT_USER_MASK)) || \
                                                    (E_TSP_FLT_USER_TTX == (FltType & E_TSP_FLT_USER_MASK))

#define TSP_POLLING_TASK_DELAY                      50

#define DRVTSP_MAX_PROCESS_NUM                      (10)

#ifdef MSOS_TYPE_LINUX_KERNEL
#define current                                     get_current()
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

typedef struct DLL_PACKED _TSP_Info
{
    MS_BOOL                         bSWInit;
    MS_BOOL                         bISRTaskEn;
    TSP_SW_Status                   Status;
    TSP_Result                      LastErr;

    MS_U32                          Pid[TSP_PIDFLT_NUM_ALL-1];
    EN_TSP_Id                       OwnerId [TSP_PIDFLT_NUM_ALL-1];
    TSP_FltState                    FltState[TSP_PIDFLT_NUM_ALL-1];
    TSP_FltType                     FltType [TSP_PIDFLT_NUM_ALL-1];
    MS_U32                          Flt2Buf [TSP_PIDFLT_NUM];

    TSP_FltMode                     FltMode    [TSP_SECFLT_NUM];
    TSP_Event                       FltEvent   [TSP_SECFLT_NUM];
    P_TSP_Callback                  FltCallback[TSP_SECFLT_NUM];
#if !defined (__aarch64__)
    MS_U32                          u32AlignDummy[TSP_SECFLT_NUM];  //align size for MI init share mem size check fail
#endif

    TSP_FltState                    BufState[TSP_SECBUF_NUM]; // @TODO: FltState, SecFltState?
    MS_U32                          Buf2Flt [TSP_SECBUF_NUM];

    TSP_InitParam                   Param;

    // Legacy
    MS_BOOL                         bSuspend;
    // Debug information
    MS_U32                          u32TaskLine;

    MS_U8                           MulFlt2Buf[TSP_SECFLT_NUM];

    MS_S32                          s32TspEvtId[TSP_SECFLT_NUM];
} TSP_Info;

typedef struct
{
    MS_U32  u32StcOffset;
    MS_BOOL bAdd;
}STC_Offset;

typedef struct
{
    MS_BOOL bEnable;
    MS_BOOL bUpdateOnce;
}STC_UpdateCtrl;

typedef struct _DrvTSP_Stc_State
{
    MS_U32  u32PcrFltId;
    MS_BOOL bUsed;
}DrvTSP_Stc_State;

typedef struct
{
    MS_BOOL bUsed;
} DrvTSP_Path_State;

typedef struct
{
    // all pid filter ids
    MS_U32  u32PidFltIdAll;
    // section filter , section buffer
    MS_U32  u32SecFltId_Start, u32SecFltId_End;
    MS_U32  u32SecBufId_Start, u32SecBufId_End;
    // pid filter
    MS_U32  u32PidFltId_Start, u32PidFltId_End;
    // PCR filter
    MS_U32  u32PcrFltId_Start, u32PcrFltId_End;
    // REC filter idx
    MS_U32  u32RecFltIdx;
} DrvTSP_FltIdMap;

#ifdef DMX_UTOPIA_2K
typedef struct _DrvTSP_KMode_ProcEvtIdTbl
{
    #ifdef MSOS_TYPE_LINUX_KERNEL
    pid_t                   tgid;
    #endif

    MS_S32                  s32TspEvtId;
}DrvTSP_KMode_ProcEvtIdTbl;
#endif

typedef struct DLL_PACKED _TSP_RESOURCE_PRIVATE
{
    MS_S32                  _s32TaskId;
    MS_S32                  _s32MutexId;
    MS_S32                  _s32LockId;
#ifdef MSOS_TYPE_LINUX_KERNEL
    spinlock_t              _s32IntRegMutexId;
    unsigned long           _IntRegFlags;
#else
    MS_S32                  _s32IntRegMutexId;
#endif
    MS_S32                  _s32PvrMutexId;

    TSP_Info                _tspInfo;
    #if defined(__LEGACY__)
    TSP_Event               _PvrEvtNotify;
    P_TSP_Callback          _PvrCallback;
    #if !defined (__aarch64__)
    MS_U32                  u32AlignDummy;  //align size for MI init share mem size check fail
    #endif
    MS_S32                  _s32PvrEvtId;
    #endif

    MS_U8                   _u8_ResetPcr[STC_ENG_NUM];
    MS_U32                  _u32_PrevStcBase[STC_ENG_NUM];
    MS_U32                  _u32_PrevPllControl[STC_ENG_NUM];
    STC_Offset              _u32_StcOffset[STC_ENG_NUM];
    STC_UpdateCtrl          _u32_StcUpdateCtrl[STC_ENG_NUM];
    DrvTSP_Stc_State        _stTspStcState[STC_ENG_NUM];
    MS_BOOL                 _bIsStcEverAlloc;
    DrvTSP_Path_State       _stTspPathState[TSP_TSIF_NUM];
    MS_BOOL                 _bIsTspPathEverAlloc;

    MS_U32                  _Current_PVR;

    //[NOTE][Eddie]
    // Since STB and TV architecture are different, STB have to emulate TV behavior to avoid
    // Live/File collision of PLAYBACK channel.
    TSP_FltType             _Current_Live;

    //for dummy record filter compatibility
    MS_U32                  u32RecFltBufId;
    MS_U32                  u32RecFltPID;
    DrvTSP_TsIf_Set         TsIf_Src[TSP_TSIF_NUM];
#ifdef SECURE_PVR_ENABLE
    SYS_TEEINFO             _teemode;
    DrvTSP_SecureBuf        _stSecBuf;
#endif

    MS_U32                  _u32StcAdjustUnit;

}TSP_RESOURCE_PRIVATE;

static MS_S32                  _s32EventId = -1;
static MS_S32                  _s32CrcErrFlags[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };
static MS_S32                  _s32SecOvfFlags[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };
static MS_S32                  _s32SecRdyFlags[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };
static MS_S32                  _s32FltDisFlags[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };
static MS_S32                  _s32FreeFlags  [(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };
static MS_S32                  _s32DoneFlags  [(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };

static MS_S32                  _s32ModeFlags[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };
static MS_S32                  _s32OvfRstFlags[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = -1 };

static MS_U32                  _u32SecNotifyId[(TSP_SECFLT_NUM+31)>>5] = { [0 ... (((TSP_SECFLT_NUM+31)>>5)-1)] = 0 }; //for ISR processing
static MS_U32                  _u32PvrNotifyId[(TSP_PVR_IF_NUM+31)>>5] = { [0 ... (((TSP_PVR_IF_NUM+31)>>5)-1)] = 0 };
static MS_U32                  _u32HwPcrNotifyId[(TSP_PCRFLT_NUM+31)>>5] = { [0 ... (((TSP_PCRFLT_NUM+31)>>5)-1)] = 0 };

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifndef TSP_UTOPIA_20
static TSP_RESOURCE_PRIVATE         _tsp_res = {
    ._s32TaskId                     = -1,
    ._s32MutexId                    = -1,
    ._s32LockId                     = -1,
#ifdef MSOS_TYPE_LINUX_KERNEL
    ._s32IntRegMutexId              = __SPIN_LOCK_UNLOCKED(_s32IntRegMutexId),
    ._IntRegFlags                   =  0,
#else
    ._s32IntRegMutexId              = -1,
#endif
    ._s32PvrMutexId                 = -1,

    ._tspInfo                       = {
        .bSWInit                    = FALSE,
        .bISRTaskEn                 = FALSE,
        .LastErr                    = E_TSP_OK,
        .bSuspend                   = FALSE,
        .u32TaskLine                = 0,
    },

    #if defined(__LEGACY__)
    ._PvrEvtNotify                  = E_TSP_EVENT_DATA_IDLE,
    ._PvrCallback                   = NULL,
    ._s32PvrEvtId                   = -1,
    #endif

    ._u8_ResetPcr                   = {[0 ... (STC_ENG_NUM-1)] = 3},
    ._u32_PrevStcBase               = {[0 ... (STC_ENG_NUM-1)] = 0},
    ._u32_PrevPllControl            = {[0 ... (STC_ENG_NUM-1)] = 0},
    ._u32_StcOffset                 = {[0 ... (STC_ENG_NUM-1)] = {0,TRUE}},
    ._u32_StcUpdateCtrl             = {[0 ... (STC_ENG_NUM-1)] = {TRUE,FALSE}},
    ._stTspStcState                 = {[0 ... (STC_ENG_NUM-1)] = {0xFFFFFFFF, FALSE}},

    ._bIsStcEverAlloc               = FALSE,
    ._stTspPathState                = {[0 ... (TSP_TSIF_NUM-1)] = {FALSE}},
    ._bIsTspPathEverAlloc           = FALSE,

    ._Current_PVR                   = 0,
    ._Current_Live                  = E_TSP_FLT_SRC_LIVE0,
    .u32RecFltBufId                 = TSP_SECFLT_SECBUF_MAX,
    .u32RecFltPID                   = TSP_PID_NULL,
    .TsIf_Src                       = {[0 ... (TSP_TSIF_NUM-1)] = {E_TSP_TS_INVALID,FALSE,TRUE,TRUE}},
#ifdef SECURE_PVR_ENABLE
    ._teemode                       = {.OsType = SYS_TEEINFO_OSTYPE_NOTEE},
    ._stSecBuf                      = { 0, 0, 0, 0},
#endif
    ._u32StcAdjustUnit              = 100,
};

static TSP_RESOURCE_PRIVATE*        _ptsp_res = &_tsp_res;
#else
static TSP_RESOURCE_PRIVATE*        _ptsp_res = NULL;
#endif

static DrvTSP_FltIdMap         _tspFltIdMap ={
                .u32SecFltId_Start          = TSP_SECFLT_START_ID,
                .u32SecFltId_End            = TSP_SECFLT_END_ID,
                .u32SecBufId_Start          = TSP_SECBUF_START_ID,
                .u32SecBufId_End            = TSP_SECBUF_END_ID,
                .u32PidFltId_Start          = TSP_PIDFLT_START_ID,
                .u32PidFltId_End            = TSP_PIDFLT_END_ID,
                .u32PcrFltId_Start          = TSP_PCRFLT_START_ID,
                .u32PcrFltId_End            = TSP_PCRFLT_END_ID,
                .u32RecFltIdx               = TSP_RECFLT_IDX,
            };



//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MSIF_Version                 _drv_tsp_version = {
                                        .DDI = { TSP_DRV_VERSION, },
                                    };

static TSP_DbgLevel                 _u32DbgLevel = E_TSP_DBG_MUST;

MS_U32                              _u32IntNum = E_INT_IRQ_TSP2HK;

#ifdef MSOS_TYPE_LINUX_KERNEL
wait_queue_head_t                   tsp_wq_head[TSP_SECFLT_NUM];
static MS_U32                       _u32SecEvent[TSP_SECFLT_NUM];
#endif // MSOS_TYPE_LINUX_KERNEL

static MS_BOOL                      _bBankInit = FALSE;

static MS_BOOL                      _s32_IsrTaskReturn = FALSE;

static MS_BOOL                      _bIsFltMapped;

#ifdef MSOS_TYPE_LINUX_KERNEL
static DrvTSP_KMode_ProcEvtIdTbl    _stKModePrcEvtTbl[DRVTSP_MAX_PROCESS_NUM];
static MS_U32                       _u32KPrcEvtTblUseFlag = 0;  //bit control for event table used
#endif

//-----      For optee   ------------------
#ifdef SECURE_PVR_ENABLE
#define MSTAR_UPA_DMXTA_UUID {0x7a7c8b61, 0x5d74, 0x4b6c, {0xb2, 0xa9, 0x92, 0xe4, 0x9c, 0xe8, 0x66, 0xbd} }
#define SYS_TEEC_OPERATION_INITIALIZER { 0 }

static const char optee_dmx[] = "opteearmtz00";
char *_dmx_device = (char *)optee_dmx;
TEEC_Context dmx_mstar_teec_ctx;
TEEC_Session dmx_session = { 0 };
TEEC_UUID dmx_uuid = MSTAR_UPA_DMXTA_UUID;
TEEC_Operation dmx_op = SYS_TEEC_OPERATION_INITIALIZER;
#endif //SECURE_PVR_ENABLE

//For Optee
#ifdef SECURE_PVR_ENABLE
extern MS_U32 MDrv_SYS_TEEC_InitializeContext(const char *name, TEEC_Context *context);
extern MS_U32 MDrv_SYS_TEEC_Open(TEEC_Context *context, TEEC_Session *session, const TEEC_UUID *destination, MS_U32 connection_method, const void *connection_data, TEEC_Operation *operation, MS_U32 *error_origin);
extern MS_U32 MDrv_SYS_TEEC_InvokeCmd(TEEC_Session *session, MS_U32 cmd_id, TEEC_Operation *operation, MS_U32 *error_origin);
extern void   MDrv_SYS_TEEC_Close(TEEC_Session *session);
extern void   MDrv_SYS_TEEC_FinalizeContext(TEEC_Context *context);
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

TSP_Result MDrv_TSP_FLT_DropEnable(MS_BOOL bSet)
{
    TSP_ENTRY();
    HAL_TSP_SecFlt_DropEnable(bSet);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static TSP_Result _MDrv_FltAllocForRec(MS_U32 u32Eng, TSP_SRC_SEQ eEngSrc, MS_U32 *pu32FltId);
static TSP_Result _MDrv_FltFreeForRec(MS_U32 u32FltId);
static TSP_Result _MDrv_FltSetPIDForRec(MS_U32 u32FltId, MS_U32 u32PID);
static TSP_Result _MDrv_FltGetPIDForRec(MS_U32 u32FltId, MS_U32* pu32PID);
static TSP_Result _MDrv_FltEnableForRec(MS_U32 u32FltId, MS_BOOL bEnable);
static TSP_Result _MDrv_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1);
static TSP_Result _MDrv_PVR_Eng_Start(MS_U32 u32Eng, TSP_DRV_PVR_RecMode eRecMode, MS_BOOL bStart)                                 ;
static TSP_Result _MDrv_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause);
static TSP_Result _MDrv_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHY *pu32WriteAddr)                                     ;
static TSP_Result _MDrv_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback)                        ;
static TSP_Result _MDrv_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet);
static TSP_Result _MDrv_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp);
static TSP_Result _MDrv_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp);
static void _MDrv_TSP_FLT_ResetFltId(void);
/*
static void _TSP_CMDQ_Init(void)
{
    while (HAL_TSP_CMDQ_Count()); // wait command finish
}
*/
// Switch Uart to TSP Fw, function works only when fw support uart functionality
// Compile Fwtsp.c with UART_ENABLE in chip.h

#if SWITCH_TSP_UART
static void _TSP_Switch_Uart(void)
{
        MS_U32 u32Base = 0 ,u32Size;
        MS_U16 u16Temp;

        TSP_DBG(E_TSP_DBG_MUST,"Switch Uart to TSP firmware !!\n");

        MDrv_MMIO_GetBASE( &u32Base, &u32Size, MS_MODULE_CHIPTOP );
        TSP_DBG(E_TSP_DBG_MUST,"ChipTop MMIO base = %08lX \n",u32Base);

        if(u32Base)
        {
        u16Temp = READ_WORD((u32Base + 0x1EA6*2));
        TSP_DBG(E_TSP_DBG_MUST,"ChipTop Uart setting = %x \n",u16Temp);
        u16Temp = ((u16Temp & 0xFFF0) | 0x3 );
        WRITE_WORD((u32Base + 0x1EA6*2),u16Temp);
        }
        else
        {
        TSP_DBG(E_TSP_DBG_MUST,"MMIO ChipTOP GetBase Fail\n");
        }

}
#endif

static MS_BOOL _TSP_InitResource(TSP_RESOURCE_PRIVATE* presource)
{
    MS_U32 u32ii = 0;

    if(presource == NULL)
    {
        TSP_DBG(E_TSP_DBG_MUST,"Utopia2 Resource address is NULL!\n");
        return FALSE;
    }

    presource->_s32TaskId = -1;
    presource->_s32MutexId = -1;
    presource->_s32LockId = -1;
#ifdef MSOS_TYPE_LINUX_KERNEL
    spin_lock_init(&presource->_s32IntRegMutexId);
#else
    presource->_s32IntRegMutexId = -1;
#endif
    presource->_s32PvrMutexId = -1;

    presource->_tspInfo.bSWInit = FALSE;
    presource->_tspInfo.bISRTaskEn = FALSE;
    presource->_tspInfo.LastErr = E_TSP_OK;
    presource->_tspInfo.bSuspend = FALSE;
    presource->_tspInfo.u32TaskLine = 0;

    #if defined(__LEGACY__)
    presource->_PvrEvtNotify = E_TSP_EVENT_DATA_IDLE;
    presource->_PvrCallback = NULL;
    presource->_s32PvrEvtId = -1;
    #endif

    for(u32ii = 0; u32ii < STC_ENG_NUM; u32ii++)
    {
        presource->_u8_ResetPcr[u32ii] = 3;
        presource->_u32_PrevStcBase[u32ii] = 0;
        presource->_u32_PrevPllControl[u32ii] = 0;

        presource->_stTspStcState[u32ii].u32PcrFltId = 0xFFFFFFFF;
        presource->_stTspStcState[u32ii].bUsed = FALSE;
    }

    presource->_bIsStcEverAlloc = FALSE;

    for(u32ii = 0; u32ii < TSP_TSIF_NUM; u32ii++)
    {
        presource->_stTspPathState[u32ii].bUsed = FALSE;
    }

    presource->_bIsTspPathEverAlloc = FALSE;

    presource->_Current_PVR = 0;
    presource->_Current_Live = E_TSP_FLT_SRC_LIVE0;

    for(u32ii = 0; u32ii < TSP_TSIF_NUM; u32ii++)
    {
        presource->TsIf_Src[u32ii].ePad = E_TSP_TS_INVALID;
        presource->TsIf_Src[u32ii].bClkInv = FALSE;
        presource->TsIf_Src[u32ii].bExtSync = TRUE;
        presource->TsIf_Src[u32ii].bParallel = TRUE;
    }

	#ifdef SECURE_PVR_ENABLE
    presource->_teemode.OsType = SYS_TEEINFO_OSTYPE_NOTEE;
    presource->_stSecBuf.u32BufId = 0;
    presource->_stSecBuf.u32BufOpt = 0;
    presource->_stSecBuf.u32BufSize = 0;
    presource->_stSecBuf.phyBufAddr = 0;
    #endif

    presource->_u32StcAdjustUnit = 100;
    _MDrv_TSP_FLT_ResetFltId();

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
        TSP_DBG(E_TSP_DBG_ERROR, "DMX_REE MApi_MBX_Init fail\n");
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
        TSP_DBG(E_TSP_DBG_ERROR, "DMX_REE MApi_MBX_QueryDynamicClass fail\n");
        return FALSE;
    }

    result = MApi_MBX_RegisterMSG(ClassNum, 10);
    if (( E_MBX_SUCCESS != result) && ( E_MBX_ERR_SLOT_AREADY_OPENNED != result ))
    {
        TSP_DBG(E_TSP_DBG_ERROR, "%s fail\n",__FUNCTION__);
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
                u8Id = (MS_U8)(pstSecBuf->u32BufId);
                _TSPReeToTeeMbxMsg.u8Parameters[0] = u8Id;
                _TSPReeToTeeMbxMsg.u8ParameterCount = 1;
                break;
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
                u8Opt = (MS_U8)(pstSecBuf->u32BufOpt);
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

static TSP_TS_PAD _TSP_DrvHal_PadMapping(TSP_TSPad eTSPad)
{
    switch (eTSPad)
    {
        case E_TSP_TS_PAD0:
            return E_TSP_TS_PAD_EXT0;
        case E_TSP_TS_PAD1:
            return E_TSP_TS_PAD_EXT1;
        case E_TSP_TS_PAD2:
            return E_TSP_TS_PAD_EXT2;
        case E_TSP_TS_PAD3:
            return E_TSP_TS_PAD_EXT3;
        case E_TSP_TS_PAD4:
            return E_TSP_TS_PAD_EXT4;
        case E_TSP_TS_PAD5:
            return E_TSP_TS_PAD_EXT5;
        case E_TSP_TS_PAD6:
            return E_TSP_TS_PAD_EXT6;
        case E_TSP_TS_PAD7:
            return E_TSP_TS_PAD_EXT7;
        case E_TSP_TS_PAD0_3WIRE:
            return HAL_TSP_3WirePadMapping(0);
        case E_TSP_TS_PAD1_3WIRE:
            return HAL_TSP_3WirePadMapping(1);
        case E_TSP_TS_PAD2_3WIRE:
            return HAL_TSP_3WirePadMapping(2);
        case E_TSP_TS_PAD3_3WIRE:
            return HAL_TSP_3WirePadMapping(3);
        case E_TSP_TS_PAD4_3WIRE:
            return HAL_TSP_3WirePadMapping(4);
        case E_TSP_TS_PAD5_3WIRE:
            return HAL_TSP_3WirePadMapping(5);
        case E_TSP_TS_PAD6_3WIRE:
            return HAL_TSP_3WirePadMapping(6);
        case E_TSP_TS_PAD7_3WIRE:
            return HAL_TSP_3WirePadMapping(7);
        case E_TSP_TS_DEMOD0:
            return E_TSP_TS_PAD_INTER0;
        case E_TSP_TS_DEMOD1:
            return E_TSP_TS_PAD_INTER1;
        case E_TSP_TS_PAD_TSO0:
            return E_TSP_TS_PAD_TSOUT0;
        case E_TSP_TS_PAD_TSO1:
            return E_TSP_TS_PAD_TSOUT1;
#ifdef TSIO_ENABLE
        case E_TSP_TS_PAD_TSIO0:
            return E_TSP_TS_PAD_TSIOOUT0;
#endif

        default:
            return E_TSP_TS_PAD_INVALID;
    }
}

static TSP_HAL_TSIF _TSP_DrvHal_TSIFMapping(TSP_TSIF eTSIF)
{
    switch(eTSIF)
    {
        case E_TSP_TSIF_0:
            return E_TSP_HAL_TSIF_0;
        case E_TSP_TSIF_1:
            return E_TSP_HAL_TSIF_1;
        case E_TSP_TSIF_2:
            return E_TSP_HAL_TSIF_2;
        case E_TSP_TSIF_3:
            return E_TSP_HAL_TSIF_3;
        case E_TSP_TSIF_4:
            return E_TSP_HAL_TSIF_4;
        case E_TSP_TSIF_5:
            return E_TSP_HAL_TSIF_5;
        case E_TSP_TSIF_6:
            return E_TSP_HAL_TSIF_6;
        case E_TSP_TSIF_PVR0:
            return E_TSP_HAL_TSIF_PVR0;
        case E_TSP_TSIF_PVR1:
            return E_TSP_HAL_TSIF_PVR1;
        case E_TSP_TSIF_PVR2:
            return E_TSP_HAL_TSIF_PVR2;
        case E_TSP_TSIF_PVR3:
            return E_TSP_HAL_TSIF_PVR3;
        default:
            return E_TSP_HAL_TSIF_INVALID;
    }
}

TSP_PIDFLT_SRC _TSP_DrvHal_FltSrcMapping(MS_U32 u32FltSrc)
{
    TSP_PIDFLT_SRC  ePidFltSrc = E_TSP_PIDFLT_INVALID;

    switch(u32FltSrc)
    {
        case E_TSP_FLT_SRC_LIVE0:
            ePidFltSrc = E_TSP_PIDFLT_LIVE0;
            break;
        case E_TSP_FLT_SRC_LIVE1:
            ePidFltSrc = E_TSP_PIDFLT_LIVE1;
            break;
        case E_TSP_FLT_SRC_LIVE2:
            ePidFltSrc = E_TSP_PIDFLT_LIVE2;
            break;
        case E_TSP_FLT_SRC_LIVE3:
            ePidFltSrc = E_TSP_PIDFLT_LIVE3;
            break;
        case E_TSP_FLT_SRC_FILE0:
            ePidFltSrc = E_TSP_PIDFLT_FILE0;
            break;
        case E_TSP_FLT_SRC_FILE1:
            ePidFltSrc = E_TSP_PIDFLT_FILE1;
            break;
        case E_TSP_FLT_SRC_FILE2:
            ePidFltSrc = E_TSP_PIDFLT_FILE2;
            break;
        case E_TSP_FLT_SRC_FILE3:
            ePidFltSrc = E_TSP_PIDFLT_FILE3;
            break;
        case E_TSP_FLT_SRC_LIVE4:
        case E_TSP_FLT_SRC_FILE4:
            ePidFltSrc = E_TSP_PIDFLT_LIVE4;
            break;
        case E_TSP_FLT_SRC_LIVE5:
        case E_TSP_FLT_SRC_FILE5:
            ePidFltSrc = E_TSP_PIDFLT_LIVE5;
            break;
        case E_TSP_FLT_SRC_LIVE6:
        case E_TSP_FLT_SRC_FILE6:
            ePidFltSrc = E_TSP_PIDFLT_LIVE6;
            break;
        default:
            TSP_DBG(E_TSP_DBG_WARNING, "[%04d] Filter input source is not assigned !! \n", __LINE__);
            ePidFltSrc = E_TSP_PIDFLT_LIVE0;
            break;
    }

    return ePidFltSrc;
}

TSP_Result MDrv_TSP_FIFO_SourceSelect(TSP_TSId eTspTSid, TSP_FileMode eFifoType)
{
    TSP_ENTRY();

    TSP_DST_SEQ eTspDstSeq = E_TSP_DST_INVALID;
    TSP_SRC_SEQ eTspSrcSeq = E_TSP_SRC_INVALID;

    switch (eFifoType)
    {
        case E_TSP_FILE_2_AUDIO:
            eTspDstSeq = E_TSP_DST_FIFO_AUDIO;
            break;
        case E_TSP_FILE_2_AUDIO2:
            eTspDstSeq = E_TSP_DST_FIFO_AUDIO2;
            break;
        case E_TSP_FILE_2_AUDIO3:
            eTspDstSeq = E_TSP_DST_FIFO_AUDIO3;
            break;
        case E_TSP_FILE_2_AUDIO4:
            eTspDstSeq = E_TSP_DST_FIFO_AUDIO4;
            break;
        case E_TSP_FILE_2_AUDIO5:
            eTspDstSeq = E_TSP_DST_FIFO_AUDIO5;
            break;
        case E_TSP_FILE_2_AUDIO6:
            eTspDstSeq = E_TSP_DST_FIFO_AUDIO6;
            break;
        case E_TSP_FILE_2_VIDEO:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO;
            break;
        case E_TSP_FILE_2_VIDEO3D:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO3D;
            break;
        case E_TSP_FILE_2_VIDEO3:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO3;
            break;
        case E_TSP_FILE_2_VIDEO4:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO4;
            break;
        case E_TSP_FILE_2_VIDEO5:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO5;
            break;
        case E_TSP_FILE_2_VIDEO6:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO6;
            break;
        case E_TSP_FILE_2_VIDEO7:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO7;
            break;
        case E_TSP_FILE_2_VIDEO8:
            eTspDstSeq = E_TSP_DST_FIFO_VIDEO8;
            break;
        default:
            TSP_OS_Print("[TSP Error][%04d] INVALID FIFO TYPE\n",__LINE__);
            TSP_RETURN(E_TSP_FAIL_INVALID);
   }

   switch (eTspTSid)
   {
        case E_TSP_TSID_TSIF0:
            eTspSrcSeq = E_TSP_SRC_PKTDMX0;
            break;
        case E_TSP_TSID_TSIF1:
            eTspSrcSeq = E_TSP_SRC_PKTDMX1;
            break;
        case E_TSP_TSID_TSIF2:
            eTspSrcSeq = E_TSP_SRC_PKTDMX3;
            break;
        case E_TSP_TSID_TSIF3:
            eTspSrcSeq = E_TSP_SRC_PKTDMX4;
            break;
        case E_TSP_TSID_MMFI0:
            eTspSrcSeq = E_TSP_SRC_MMFI0;
            break;
        case E_TSP_TSID_MMFI1:
            eTspSrcSeq = E_TSP_SRC_MMFI1;
            break;
        default:
            TSP_OS_Print("[TSP Error][%04d] INVALID PKDMX SRC\n",__LINE__);
            TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_FIFO_SetSrc(eTspDstSeq, eTspSrcSeq);

    if((eTspTSid == E_TSP_TSID_MMFI0) || (eTspTSid == E_TSP_TSID_MMFI1))
    {
        HAL_TSP_FIFO_BlockDis(eTspDstSeq, FALSE);
    }
    else
    {
        HAL_TSP_FIFO_BlockDis(eTspDstSeq, TRUE);
    }

    TSP_RETURN(E_TSP_OK);
}


static TSP_DST_SEQ _TSP_DrvHal_FltType2DstMapping(TSP_FltType eFltType)
{
    TSP_DST_SEQ eRetDst = E_TSP_DST_INVALID;

    switch(eFltType)
    {
        case E_TSP_FLT_FIFO_VIDEO ... E_TSP_FLT_FIFO_VIDEO8:
            eRetDst= (TSP_DST_SEQ)(E_TSP_DST_FIFO_VIDEO + (eFltType - E_TSP_FLT_FIFO_VIDEO));
            break;
        case E_TSP_FLT_FIFO_AUDIO ... E_TSP_FLT_FIFO_AUDIO6:
            eRetDst = (TSP_DST_SEQ)(E_TSP_DST_FIFO_AUDIO + (eFltType - E_TSP_FLT_FIFO_AUDIO));
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d] UnSupport Flt Type!\n", __FUNCTION__, __LINE__);
            break;
    }

    return eRetDst;
}


//-------------------------------------------------------------------------------------------------
/// Reset audio/video fifo
/// @param  bReset                  \b IN   0->1:reset, 1->0:activate
/// @return TSP_Result
/// @note
/// 0->1 reset, 1->0 activate
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FIFO_Reset(TSP_FltType eFltType, MS_BOOL bReset)
{
    TSP_ENTRY();

    TSP_DST_SEQ eDst = _TSP_DrvHal_FltType2DstMapping(eFltType);

    HAL_TSP_FIFO_Reset(eDst,  bReset);

    TSP_RETURN(E_TSP_OK);
}


TSP_Result MDrv_TSP_FIFO_BlockEnable(TSP_FltType eFltType, MS_BOOL bDisable)
{
    TSP_ENTRY();

    TSP_DST_SEQ eDst = _TSP_DrvHal_FltType2DstMapping(eFltType);

    HAL_TSP_FIFO_BlockDis(eDst, bDisable);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Reset audio/video fifo
/// @param  u32Level                \b OUT  0:0-25%, 1:25-50%, 2:50-75%, 3:75-100%, 0xFFFFFFFF:N/A
/// @return TSP_Result
/// @note
/// 0->1 reset, 1->0 activate
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FIFO_GetStatus(TSP_FltType eFltType, MS_U32 *pu32Level)
{
    TSP_ENTRY();

    eFltType &= E_TSP_FLT_FIFO_MASK;

    TSP_DST_SEQ eDst = _TSP_DrvHal_FltType2DstMapping(eFltType);

    *pu32Level = HAL_TSP_FIFO_GetStatus(eDst);
    if (*pu32Level == HAL_TSP_RET_NULL)
    {
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FIFO_Overflow_Status(TSP_FltType eFltType, MS_BOOL *pbOverflow)
{
    *pbOverflow = FALSE;

    TSP_ENTRY();
    *pbOverflow = HAL_TSP_FIFO_Overflow(eFltType);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable)
{
    TSP_ENTRY();

    HAL_TSP_BD_AUD_En(0, bEnable);

    TSP_RETURN(E_TSP_OK);
}

static TSP_Result _TSP_Path_Alloc(DRV_TSP_PATH_TYPE eResType, MS_U32 *pu32PathId)
{
    if( (eResType == E_DRV_TSP_PATH_LIVE) || (eResType == E_DRV_TSP_PATH_FILE) )
    {
        TSP_TSIF    eTSIF = E_TSP_TSIF_INVALID;
        MS_U32      u32PhyTspPath = 0;

        *pu32PathId = (MS_U32)-1;
        for(eTSIF = E_TSP_TSIF_0; eTSIF < TSP_TSIF_NUM; eTSIF++)
        {
            //@Note: Currently, TSP_TSIF for live-in & TSP HW path are 1-1 mapping, and
            //       TSP_TSIF for file-in & TSP HW path are also 1-1 mapping
            TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
            u32PhyTspPath = HAL_TSP_TsifMapping(eHalTSIF, (eResType == E_DRV_TSP_PATH_FILE));

            if(_ptsp_res->_stTspPathState[u32PhyTspPath].bUsed == FALSE)
            {
                _ptsp_res->_stTspPathState[u32PhyTspPath].bUsed = TRUE;
                *pu32PathId = (MS_U32) eTSIF;
                return E_TSP_OK;
            }
        }
    }

    return E_TSP_FAIL;
}

static TSP_Result _TSP_Path_Free(DRV_TSP_PATH_TYPE eResType, TSP_TSIF ePath)
{
    if( (eResType == E_DRV_TSP_PATH_LIVE) || (eResType == E_DRV_TSP_PATH_FILE) )
    {
        MS_U32      u32PhyTspPath = 0;

        TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(ePath);
        u32PhyTspPath = HAL_TSP_TsifMapping(eHalTSIF, (eResType == E_DRV_TSP_PATH_FILE));

        if(_ptsp_res->_stTspPathState[u32PhyTspPath].bUsed == FALSE)
            return E_TSP_FAIL;

        _ptsp_res->_stTspPathState[u32PhyTspPath].bUsed = FALSE;
        return E_TSP_OK;
    }

    return E_TSP_FAIL;
}

static TSP_Result _TSP_STC_Alloc(MS_U32 *pu32EngId)
{
    MS_U32 i;
    *pu32EngId = 0xFFFFFFFF;

    for(i = 0; i < STC_ENG_NUM; i++)
    {
        if(_ptsp_res->_stTspStcState[i].bUsed == FALSE)
        {
            _ptsp_res->_stTspStcState[i].bUsed = TRUE;
            *pu32EngId = i;
            break;
        }
    }

    if(*pu32EngId == 0xFFFFFFFF)
        return E_TSP_FAIL;

    return E_TSP_OK;
}

static TSP_Result _TSP_STC_Free(MS_U32 u32EngId)
{
    if(_ptsp_res->_stTspStcState[u32EngId].bUsed == FALSE)
        return E_TSP_FAIL;

    _ptsp_res->_stTspStcState[u32EngId].bUsed = FALSE;
    _ptsp_res->_stTspStcState[u32EngId].u32PcrFltId = 0xFFFFFFFF;

    return E_TSP_OK;
}

static TSP_Result _TSP_STC_SetPcrFlt(MS_U32 u32EngId, MS_U32 u32PcrFltId)
{
    _ptsp_res->_stTspStcState[u32EngId].u32PcrFltId = u32PcrFltId;

    return E_TSP_OK;
}

static TSP_Result _TSP_PcrId_To_StcId(MS_U32 u32PcrFltId,MS_U32 *pu32EngId)
{
    MS_U32 i;

    *pu32EngId = 0xFFFFFFFF;

    for(i = 0; i < STC_ENG_NUM; i++)
    {
        if((_ptsp_res->_stTspStcState[i].u32PcrFltId == u32PcrFltId)
            && (_ptsp_res->_stTspStcState[i].bUsed == TRUE))
        {
            *pu32EngId = i;
            return E_TSP_OK;
        }
    }

    return E_TSP_FAIL;
}

// initilize or reset the FLT to be initial state
// PID and ctrls are clear
static void _TSP_FLT_Init(MS_U32 u32FltId)
{
    static MS_U8        u8Mask[TSP_FILTER_DEPTH]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    static MS_U8        u8NMask[TSP_FILTER_DEPTH] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                                      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
#if HW_PCRFLT_ENABLE

    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        MS_U32  Pcr_H, Pcr;
        MS_U32  u32PcrId;

        if ((u32FltId >= TSP_PCRFLT_GET_ID(0)) && (u32FltId <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
        {
            u32PcrId = u32FltId - _tspFltIdMap.u32PcrFltId_Start;

            HAL_TSP_PcrFlt_SetPid(u32PcrId, TSP_PID_NULL);
            HAL_TSP_PcrFlt_SetSrc(u32PcrId, E_TSP_PCR_SRC_INVALID);
#ifdef MERGE_STR_SUPPORT
            HAL_TSP_PcrFlt_SetSrcId(u32PcrId, (E_TSP_FLT_SRCID_0 & E_TSP_FLT_SRCID_MASK) >> E_TSP_FLT_SRCID_SHIFT);
#endif // end MERGE_STR_SUPPORT
            HAL_TSP_PcrFlt_Enable(u32PcrId, FALSE);
            HAL_TSP_PcrFlt_ClearInt(u32PcrId);
            HAL_TSP_INT_Disable(HAL_TSP_PcrFlt_GetIntMask(u32PcrId));

            //always read PCR after PCR interrupt has been disabled or cleared to avoid that interrupt's not trggering again.
            HAL_TSP_PcrFlt_GetPcr(u32PcrId, &Pcr_H, &Pcr);
        }
        else
        {
            TSP_DBG(E_TSP_DBG_MUST,"[%s][%d] Incorrect PCR filter ID !!\n",__FUNCTION__,__LINE__);
            return;
        }
    }
    else

#endif
    {
        HAL_TSP_PidFlt_SetPid(u32FltId, TSP_PID_NULL);
        HAL_TSP_PidFlt_SetFltIn(u32FltId, TSP_PIDFLT_TSIF_MAX);                                         // default MAX
        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE);
        HAL_TSP_PidFlt_SetSecFlt(u32FltId, u32FltId);                                                   // PIDFLT <-> SECFLT

        _ptsp_res->_tspInfo.Flt2Buf[u32FltId]     = TSP_FLT2BUF_NULL;
    }

    _ptsp_res->_tspInfo.Pid[u32FltId]         = TSP_PID_NULL ;
    _ptsp_res->_tspInfo.FltState[u32FltId]    = E_TSP_FLT_STATE_FREE;
    _ptsp_res->_tspInfo.OwnerId[u32FltId]     = E_TSP_ID_NULL;

    // @NOTE section filters may be less than PID filters
    if(u32FltId < _tspFltIdMap.u32SecFltId_End)
    {
        REG_SecFlt *        pSecFlt =     &(_REGSec->Flt[u32FltId]);

        HAL_TSP_SecFlt_ClrCtrl(pSecFlt);
        HAL_TSP_SecFlt_SelSecBuf(pSecFlt, TSP_SECFLT_SECBUF_MAX);                                       // default MAX
        HAL_TSP_SecFlt_SetMask(pSecFlt, u8Mask);
        HAL_TSP_SecFlt_SetNMask(pSecFlt, u8NMask);
        HAL_TSP_SecFlt_SetType(pSecFlt, TSP_SECFLT_USER_NULL);                                          // default SECTION

        _ptsp_res->_tspInfo.FltMode[u32FltId]     = E_TSP_FLT_MODE_CONTI;
        _ptsp_res->_tspInfo.FltEvent[u32FltId]    = E_TSP_EVENT_DATA_IDLE;
        _ptsp_res->_tspInfo.FltCallback[u32FltId] = NULL;
    }

}

static void _TSP_FLT_Free(MS_U32 u32FltId)
{
    if (_tspFltIdMap.u32SecFltId_End > u32FltId)
    {
        HAL_TSP_SecFlt_Free(&(_REGSec->Flt[u32FltId]));
        TSP_FLAG_ClrModeFlags(u32FltId);
    }

    _ptsp_res->_tspInfo.OwnerId[u32FltId]  = E_TSP_ID_NULL;
    _ptsp_res->_tspInfo.FltState[u32FltId] = E_TSP_FLT_STATE_FREE;
    _ptsp_res->_tspInfo.FltType[u32FltId] = E_TSP_FLT_USER_NULL;

}

// Disable FLT output (SEC, AUD, VID)
// NOTE: other bypass route to ECM/EMM/REC/etc. is not disable.
static void _TSP_FLT_Disable(MS_U32 u32FltId)
{
#if 1

    #if HW_PCRFLT_ENABLE

    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        MS_U32  Pcr_H, Pcr;
        MS_U32  u32PcrId;

        if ((u32FltId >= TSP_PCRFLT_GET_ID(0)) && (u32FltId <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
        {
            u32PcrId = u32FltId - _tspFltIdMap.u32PcrFltId_Start;

            //HAL_TSP_PcrFlt_SetSrc(u32PcrId, E_TSP_PCR_SRC_INVALID);
            HAL_TSP_PcrFlt_Enable(u32PcrId, FALSE);

            HAL_TSP_PcrFlt_ClearInt(u32PcrId);
            HAL_TSP_INT_Disable(HAL_TSP_PcrFlt_GetIntMask(u32PcrId));

            //always read PCR after PCR interrupt has been disabled or cleared to avoid that interrupt's not trggering again.
            HAL_TSP_PcrFlt_GetPcr(u32PcrId, &Pcr_H, &Pcr);
        }
        else
        {
            TSP_DBG(E_TSP_DBG_MUST,"[%s][%d] Incorrect PCR filter ID !!\n",__FUNCTION__,__LINE__);
            return;
        }
    }
    else

    #endif
    {
        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE);  //@FIXME do we have to clear pvr and section out at this function?
    }

    _ptsp_res->_tspInfo.FltState[u32FltId] &= (TSP_FltState)(~E_TSP_FLT_STATE_ENABLE);

#else
    HAL_TSP_PidFlt_SetFltOut(&(_REGPid->Flt[u32FltId]), TSP_PIDFLT_OUT_NULL);
    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        HAL_TSP_SecFlt_PcrReset(&(_REGSec->Flt[u32FltId]));
        HAL_TSP_PCRFlt_SetPid(0x1FFF,false);
        HAL_TSP_PCRFlt_ClearInt();
        HAL_TSP_INT_Disable(TSP_HWINT2_PCR_UPD << 8 );
    }
    _ptsp_res->_tspInfo.FltState[u32FltId] &= (TSP_FltState)(~E_TSP_FLT_STATE_ENABLE);
#endif


/*
//[Note] Delay for VQ flush, open it if necessary.
#if defined (MSOS_TYPE_LINUX_KERNEL)
    mdelay(5);
#elif defined (MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)
#else
    MsOS_DelayTask(5);
#endif
*/

}


static void _TSP_SEC_Init(MS_U32 u32BufId)
{
    _ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] = 0 ;        // Multi Filter to buffer count.
    _ptsp_res->_tspInfo.Buf2Flt[u32BufId] = TSP_BUF2FLT_NULL;
    HAL_TSP_SecBuf_SetBuf(&(_REGBuf->Buf[u32BufId]), 0, 0);
    HAL_TSP_SecBuf_Reset(&(_REGBuf->Buf[u32BufId]));
}


static void _TSP_SEC_Free(MS_U32 u32BufId)
{
    HAL_TSP_SecBuf_Free(&(_REGBuf->Buf[u32BufId]));
    _TSP_RemoveSecNotifyId(u32BufId);
    _ptsp_res->_tspInfo.BufState[u32BufId] = E_TSP_FLT_STATE_FREE;
}

static TSP_Result _TSP_PROC_CrcErr(MS_U32 u32FltId, TSP_Event *pEvent)
{
    static TSP_EventMsg msg;
    MS_U32              flags; // dummy

    TSP_FLAG_GetCrcErr(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR); // if TSP not enable

        if (pEvent)
        {
            if ((_ptsp_res->_tspInfo.FltMode[u32FltId] & E_TSP_FLT_MODE_CRCCHK) ||
                (_ptsp_res->_tspInfo.FltMode[u32FltId] & E_TSP_FLT_MODE_AUTO_CRCCHK))
            {
                *pEvent = (TSP_Event) E_TSP_EVENT_SEC_CRCERROR;
            }
        }
        else
        {
            if (!(_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE)) // if TSP not enable
            {
                return E_TSP_FAIL;
            }
            if ((_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_SEC_CRCERROR) &&
                (_ptsp_res->_tspInfo.FltCallback[u32FltId]) )
            {
                msg.FltInfo = (_ptsp_res->_tspInfo.OwnerId[u32FltId] << TSP_MSG_ENGID_SHFT) | (u32FltId << TSP_MSG_FLTID_SHFT);
                TSP_DBG_TASK();
                _ptsp_res->_tspInfo.FltCallback[u32FltId](E_TSP_EVENT_SEC_CRCERROR, &msg);
                TSP_DBG_TASK();
            }
        }

    return E_TSP_OK;
}




static TSP_Result _TSP_PROC_SecOvf(MS_U32 u32FltId, TSP_Event *pEvent)
{
    static TSP_EventMsg msg;
    MS_U32              flags; // dummy

    //(POLL) Check any possible filter should be disabled
    //(TASK) Check only overflowed filter and to disable
    TSP_LOCK(); //{
    if (HAL_TSP_PidFlt_GetFltOutput(&(_REGPid1->Flt[u32FltId])) & TSP_PIDFLT_OUT_SECFLT)
    {
        _TSP_FLT_Disable(u32FltId);
    }
    TSP_FLAG_SetOvfRst(u32FltId);
    TSP_UNLOCK(); //}

    TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);
    //[Eddie]Clear Event at Filter Enable , Try to avoid redundant Overflow event.
    //       Driver will not call overflow call back until previous overflow have been serverd(Reset Buffer)
    //TSP_FLAG_GetSecOvf(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);


    if (pEvent ) // MDrv_TSP_Proc -> _TSP_Poll_Proc
    {
        *pEvent = (MS_U32)E_TSP_EVENT_BUF_OVERFLOW;
    }
    else
    {
        // required event callback
        if ( (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_BUF_OVERFLOW) &&
             (_ptsp_res->_tspInfo.FltCallback[u32FltId]) )
        {
            msg.FltInfo = (_ptsp_res->_tspInfo.OwnerId[u32FltId] << TSP_MSG_ENGID_SHFT) | (u32FltId << TSP_MSG_FLTID_SHFT);

            TSP_DBG_TASK();
            _ptsp_res->_tspInfo.FltCallback[u32FltId](E_TSP_EVENT_BUF_OVERFLOW, &msg);
            TSP_DBG_TASK();
        }
    }

    return E_TSP_OK;
}


void MDrv_TSP_SEC_Update( MS_U32 u32SecFltId)
{
    TSP_FLAG_SetSecRdy(u32SecFltId);
}


static TSP_Result _TSP_PROC_SecRdy(MS_U32 u32FltId, TSP_Event *pEvent)
{
    static TSP_EventMsg msg;

    MS_U16              bufid;
    MS_BOOL             data;
    MS_U32              flags;

/*
    if (E_TSP_EVENT_CB_POLL == u32CbOpt)
    {
        TSP_GetEvent(u32Events, TSP_EVENT_SECTION_POLL, TSP_OS_EVENT_OR_CLEAR, 0);
    }
    else
    {
    }
*/

    TSP_FLAG_GetSecRdy(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR); // clear for next incoming

    TSP_LOCK();
    bufid = HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[u32FltId]));

    #if 1
    if ((_ptsp_res->_tspInfo.FltState[u32FltId] == E_TSP_FLT_STATE_FREE) || (bufid != _ptsp_res->_tspInfo.Flt2Buf[u32FltId]))
    {
        for(flags = 0 ; flags < 10 ; flags ++)
        TSP_OS_Print("[TSP Error][%04d] u32FltId:%u eFltType:%08X Pid:%08X, FltStatus = %08X ,Mode = %08X , Event =%08X , AllMode= %08X ,Pevent = %08X,HwBufId %d != tspInfo Bufid %d\n",
                                                          __LINE__,
                                                          (unsigned int)u32FltId,
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltType[u32FltId]),
                                                          (unsigned int)_ptsp_res->_tspInfo.Pid[u32FltId],
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltState[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltMode[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltEvent[u32FltId]),
                                                          (unsigned int)_s32ModeFlags[u32FltId],
                                                          (unsigned int)*pEvent,
                                                          bufid,
                                                          (unsigned int)_ptsp_res->_tspInfo.Flt2Buf[u32FltId]);
        //while(1);
        TSP_UNLOCK();

        return E_TSP_FAIL;
    }

    #else

    TSP_CHECK_UNLOCK(bufid == _ptsp_res->_tspInfo.Flt2Buf[u32FltId], "[TSP Error][%04d] u32FltId:%u eFltType:%08X Pid:%08X, FltStatus = %08X ,Mode = %08X , Event =%08X , AllMode= %08lX ,Pevent = %08lX,HwBufId %ld != tspInfo Bufid %ld\n",
                                                          __LINE__,
                                                          (unsigned int)u32FltId,
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltType[u32FltId]),
                                                          (unsigned int)_ptsp_res->_tspInfo.Pid[u32FltId],
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltState[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltMode[u32FltId]),
                                                          (unsigned int)(_ptsp_res->_tspInfo.FltEvent[u32FltId]),
                                                          (MS_U32)_s32ModeFlags,
                                                          (MS_U32)pEvent,
                                                          (MS_U32)bufid,
                                                          _ptsp_res->_tspInfo.Flt2Buf[u32FltId]);
    #endif

    data = (HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[bufid])) != HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[bufid]))) ? TRUE : FALSE;
    TSP_UNLOCK();


    if (data)
    {
        if (pEvent) // POLL mode
        {
            *pEvent = (TSP_Event)E_TSP_EVENT_DATA_READY;
            // if TSP not enable
            if (TSP_FLAG_GetSecOvf(u32FltId,&flags,TSP_OS_EVENT_OR)||TSP_FLAG_GetCrcErr(u32FltId,&flags,TSP_OS_EVENT_OR))
            {
                return E_TSP_FAIL;
            }
        }
        else
        {
            if (!(_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE)) // if TSP not enable
            {
                return E_TSP_FAIL;
            }
            if ((_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_DATA_READY) &&
                (_ptsp_res->_tspInfo.FltCallback[u32FltId]) )
            {
                msg.FltInfo = (_ptsp_res->_tspInfo.OwnerId[u32FltId] << TSP_MSG_ENGID_SHFT) | (u32FltId << TSP_MSG_FLTID_SHFT);

                TSP_DBG_TASK();
                _ptsp_res->_tspInfo.FltCallback[u32FltId](E_TSP_EVENT_DATA_READY, &msg);
                TSP_DBG_TASK();
            }
        }
    }

    //(POLL) Check any possible filter should be disabled
    //(TASK) Check only one-shot filter and to disable
    if (TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR))
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId);
        TSP_UNLOCK();
    }

    if (data)
    {
        return E_TSP_OK;
    }
    return E_TSP_FAIL; // no data valid || not enable
}

static void _TSP_Proc_Pvr(MS_U32 u32Events, MS_U32 u32CbOpt)
{
    TSP_DBG(E_TSP_DBG_MUST, "%s Wait implementation\n", __FUNCTION__);
}

static void _TSP_ISR_Proc(MS_U32 u32Events)
{
    MS_U32              fltid = TSP_SECFLT_NUM, flags = 0,ModeMask,dummy,BaseFlt;
    int                 i;

    TSP_DBG_TASK();

#if __LEGACY__
    if ( (TSP_EVENT_PVR0_RDY_POLL | TSP_EVENT_PVR1_RDY_POLL) & u32Events)
    {
        _TSP_Proc_Pvr(u32Events, E_TSP_EVENT_CB_AUTO);
    }
#endif


    // @NOTE: Jerry
    //
    // WATCH OUT!!
    // It has many issues before due to race condition between TSP_TASK and USER_TASK dealing with FLT_FREE
    // Please pay more attention on it
    //
    // Check all freeing filters
    if (u32Events & TSP_EVENT_FREEFLT)
    {
        for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
        {
            // Add to FREE list
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeFree(i, ModeMask,&flags, TSP_OS_EVENT_OR_CLEAR);
            while (flags)
            {
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);
                TSP_LOCK();
                _TSP_FLT_Disable(fltid); // disable output
                TSP_UNLOCK();
                flags &= ~(1 << BaseFlt);
                TSP_FLAG_SetDone(fltid);

                // Clear other filter activities
                TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
                TSP_FLAG_GetSecOvf(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
                TSP_FLAG_GetSecRdy(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
            }
        }
    }

    MsOS_DelayTask(1);
    // Check all section avaiable filters
    if (u32Events & TSP_EVENT_SECTION)
    {
        // @TODO Jerry
        // Is it necessary to balance every filter priority even the event process is at TASK?

        for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
        {
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeCrcErr(i, ModeMask,&flags, TSP_OS_EVENT_OR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only one-shot filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                // cause we serve free event before section rdy
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE))
                {
                    _TSP_PROC_CrcErr(fltid, NULL); // Pass events directly to callback
                }
                flags &= ~(1 << BaseFlt);
            }

            TSP_FLAG_ModeSecRdy(i, ModeMask,&flags, TSP_OS_EVENT_OR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only one-shot filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                // cause we serve free event before section rdy
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE))
                {
                    _TSP_PROC_SecRdy(fltid, NULL); // Pass events directly to callback
                }
                flags &= ~(1 << BaseFlt);
            }
            TSP_FLAG_ModeSecOvf(i, ModeMask,&flags, TSP_OS_EVENT_OR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only overflowed filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                //if (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE )
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) &&
                    (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE) &&
                    (!TSP_FLAG_GetOvfRst(fltid)))
                {
                    //TSP_OS_Print("TSP_FLT_OVFLOW [%ld] , Buf = %ld ,PID = %04lX \n",fltid, _ptsp_res->_tspInfo.Flt2Buf[fltid], _ptsp_res->_tspInfo.Pid[fltid]);
                    _TSP_PROC_SecOvf(fltid, NULL); // Pass events directly to callback
                }
                flags &= ~(1 << BaseFlt );
                // Clear other filter activities
                TSP_FLAG_GetSecRdy(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR); // Clear section ready also
            }
            MsOS_DelayTask(1);
        }
    }


    if (u32Events & TSP_EVENT_DISABLEFLT)//Disable Polling mode filter,CB mode filter disable at SecRdy or SecOvf
    {
        for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
        {
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeFltDis(i,~ModeMask, &flags ,TSP_OS_EVENT_OR_CLEAR); // Polling mode
            while (flags)
            {
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);
                if (TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR))
                {
                    TSP_LOCK();
                    _TSP_FLT_Disable(fltid); // disable output
                    TSP_UNLOCK();
                }
                flags &= ~(1 << BaseFlt);
            }
        }
   }

   MsOS_DelayTask(1);


}


static void _TSP_ISR_Task(void)
{
    MS_U32              events = 0;

    while (_ptsp_res->_tspInfo.bISRTaskEn)
    {
        // @NOTE Jerry
        // Clear task events directly in case there has any incoming event before _TSP_ISR_Proc to clear
        TSP_GetEvent(_s32EventId, &events, TSP_TASK_EVENTS, TSP_OS_EVENT_OR_CLEAR, MSOS_WAIT_FOREVER);

        if(events & TSP_EVENT_TASKEND)
        {
            break;
        }

        _TSP_ISR_Proc(events);
    }

    _s32_IsrTaskReturn = TRUE;
}

#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _TSP_ISR(int irq, void *dev_id)
#else
static void _TSP_ISR(void)
#endif
{
    MS_U32                 hw_int;
    MS_U32                 sw_int;
    MS_U32                 Pcr_H, Pcr, Stc_H = 0, Stc = 0, u32Diff;
    MS_U32                 i = 0;

    _TSP_INT_LOCK();
    hw_int = HAL_TSP_INT_GetHW();
    //@TODO Need to check why call clr twice will cause one of pcr not work
    sw_int = HAL_TSP_INT_GetSW();
    _TSP_INT_UNLOCK();

#if __LEGACY__
    // Richard: @FIXME: do we need to detect all the int
    if (hw_int & TSP_HWINT_PVR)
    {
        if(_TSP_ChkPvrNotifyId(0))
        {
            TSP_SetEvent(_ptsp_res->_s32PvrEvtId, hw_int & TSP_HWINT_PVR); // Trigger task
        }
        else
        {
            hw_int &= ~TSP_HWINT_PVR;
        }
    }
#endif

#if defined(OTV_ENABLE)

    if ( hw_int & (TSP_HWINT2_OTV) )
    {
        MS_U32  u32OTV_Eng;
        MS_BOOL bInterrupted;

        //TSP_DBG(E_TSP_DBG_TRACK, "[%s][%04d] ____TSP_OTV_ISR____ \n", __FUNCTION__, __LINE__);

        for( u32OTV_Eng=0; u32OTV_Eng<TSP_OTVENG_NUM; u32OTV_Eng++)
        {
            if( MDrv_OTV_HWINT_Get(u32OTV_Eng, E_DRVOTV_HWINT_EVENT, &bInterrupted) )
            {
                if(bInterrupted)
                {
                    MDrv_OTV_ISR(u32OTV_Eng, E_DRVOTV_HWINT_EVENT);
                    MDrv_OTV_HWINT_Clear(u32OTV_Eng, E_DRVOTV_HWINT_EVENT);
                }
            }

            if( MDrv_OTV_HWINT_Get(u32OTV_Eng, E_DRVOTV_HWINT_TIMER, &bInterrupted) )
            {
                if(bInterrupted)
                {
                    MDrv_OTV_ISR(u32OTV_Eng, E_DRVOTV_HWINT_TIMER);
                    MDrv_OTV_HWINT_Clear(u32OTV_Eng, E_DRVOTV_HWINT_TIMER);
                }
            }

        }

        // clear OTV HW Interrrupt
        HAL_TSP_INT_ClrHW(TSP_HWINT2_OTV);

    }
    else
    {
        hw_int &= ~TSP_HWINT2_OTV;
    }

#endif


/******************PCR INT*********************/

    MS_U32 u32PcrDstSTC = 0;
    MS_U32 u32HwPcrIntMask = 0x0;


    #define STC_SYNTH_DEFAULT 0x28000000   // @F_TODO do we have to seperate 27M and 90K clk mode? plz define this value to right place...

    for(i = 0; i < STC_ENG_NUM; i++)
    {
        u32HwPcrIntMask = HAL_TSP_PcrFlt_GetIntMask(i);

        if (_TSP_ChkHwPcrNotifyId(i) && (hw_int & u32HwPcrIntMask))
        {
            _TSP_PcrId_To_StcId(TSP_PCRFLT_GET_ID(i), &u32PcrDstSTC);

            _TSP_INT_LOCK();
            HAL_TSP_PcrFlt_ClearInt(i);
            _TSP_INT_UNLOCK();

            //always read PCR after PCR interrupt has been disabled or cleared to avoid that interrupt's not trggering again.
            HAL_TSP_PcrFlt_GetPcr(i, &Pcr_H, &Pcr);

            MS_U32 u32STC_Add_Offset = ( _ptsp_res->_u32_StcOffset[i].bAdd == TRUE) ? (Pcr + _ptsp_res->_u32_StcOffset[i].u32StcOffset) : (Pcr - _ptsp_res->_u32_StcOffset[i].u32StcOffset);

            if(_ptsp_res->_u8_ResetPcr[i]>=2)
            {
                if(_ptsp_res->_u8_ResetPcr[i] == 3)
                {
                    HAL_TSP_SetSTCSynth(u32PcrDstSTC, STC_SYNTH_DEFAULT);
                }
                //HAL_TSP_CMDQ_SetSTC(Pcr_H,Pcr);

                if(_ptsp_res->_u32_StcUpdateCtrl[i].bEnable == TRUE)
                {
                    HAL_TSP_STC64_Set(u32PcrDstSTC, Pcr_H,u32STC_Add_Offset);

                    if(_ptsp_res->_u32_StcUpdateCtrl[i].bUpdateOnce == TRUE)
                    {
                        _ptsp_res->_u32_StcUpdateCtrl[i].bEnable = FALSE;
                        _ptsp_res->_u32_StcUpdateCtrl[i].bUpdateOnce = FALSE;
                    }
                }

                _ptsp_res->_u8_ResetPcr[i] = 0;
                _ptsp_res->_u32_PrevStcBase[i] = u32STC_Add_Offset;
                HAL_TSP_GetSTCSynth(u32PcrDstSTC, &_ptsp_res->_u32_PrevPllControl[i]);
            }

            //HAL_TSP_CMDQ_GetSTC(&Stc_H, &Stc);
            HAL_TSP_STC64_Get(u32PcrDstSTC, &Stc_H, &Stc);
            if(( Stc - _ptsp_res->_u32_PrevStcBase[i]) > 0x4000)
            {
                //ULOGD("TSP", "STC_DIFF = %08X ==== \n\n",Stc - _ptsp_res->_u32_PrevStcBase);
                _ptsp_res->_u32_PrevStcBase[i] = Stc;

                if(_ptsp_res->_u32_PrevStcBase[i] > u32STC_Add_Offset) // faster then PCR comming
                {
                    u32Diff = _ptsp_res->_u32_PrevStcBase[i] - u32STC_Add_Offset;
                    if( u32Diff  <= 0x02) //pretty close
                    {
                        _ptsp_res->_u8_ResetPcr[i] = 0;
                    }
                    else if( u32Diff  < 0x1000)
                    {
                        _ptsp_res->_u32_PrevPllControl[i] += ((u32Diff) >> 3 );                         //now = error x alpha + prev
                        HAL_TSP_SetSTCSynth(u32PcrDstSTC, _ptsp_res->_u32_PrevPllControl[i] + ((u32Diff)>>6));        //out = now + error x beta
                        _ptsp_res->_u8_ResetPcr[i] = 0;
                        //ULOGD("TSP", "======== Set STC CONTROL <++> %08X  PCR/Diff = %08X/%08X \n",_ptsp_res->_u32_PrevPllControl[i] + ((_ptsp_res->_u32_PrevStcBase[i] - u32STC_Add_Offset)>>6),u32STC_Add_Offset,_ptsp_res->_u32_PrevStcBase[i]-u32STC_Add_Offset);
                    }
                    else
                    {
                        _ptsp_res->_u8_ResetPcr[i]++;
                    }
                }
                else // slower or equal
                {
                    u32Diff = u32STC_Add_Offset - _ptsp_res->_u32_PrevStcBase[i];
                    if(( u32Diff) <= 0x02)
                    {
                        _ptsp_res->_u8_ResetPcr[i] = 0;
                    }
                    else if((u32Diff) < 0x1000)
                    {
                        _ptsp_res->_u32_PrevPllControl[i]-=((u32Diff)>> 3);                                //now = error x alpha + prev
                        HAL_TSP_SetSTCSynth(u32PcrDstSTC, _ptsp_res->_u32_PrevPllControl[i] - (( u32Diff )>> 6));        //out = now + error x beta
                        _ptsp_res->_u8_ResetPcr[i] = 0;
                        //ULOGD("TSP", "======== Set STC CONTROL <--> %08X  PCR/Diff = %08X/%08X \n",_ptsp_res->_u32_PrevPllControl[i] - ((u32STC_Add_Offset - _ptsp_res->_u32_PrevStcBase[i] )>>6),u32STC_Add_Offset,u32STC_Add_Offset-_ptsp_res->_u32_PrevStcBase[i] );
                    }
                    else
                    {
                        _ptsp_res->_u8_ResetPcr[i]++;
                    }
                }
            }
        }// end pcr
        else
        {
            hw_int &=( ~u32HwPcrIntMask);
        }
    }

/**********************************************/


    if (sw_int & TSP_SWINT_CTRL_FIRE)
    {
        //MS_U32          engid;
        MS_U32          fltid;
        MS_U32          status;
        //REG_SecFlt*     pFilter;
        MS_U32          dummy;

        //engid   = (sw_int & TSP_SWINT_INFO_ENG_MASK)   >> TSP_SWINT_INFO_ENG_SHFT;
        fltid   = (sw_int & TSP_SWINT_INFO_SEC_MASK)   >> TSP_SWINT_INFO_SEC_SHFT;
        status  = (sw_int & TSP_SWINT_STATUS_CMD_MASK) >> TSP_SWINT_STATUS_CMD_SHFT;
        //pFilter = &(_REGSec->Flt[fltid]);

        if(_ptsp_res->_tspInfo.BufState[fltid] == E_TSP_FLT_STATE_FREE)
        {
            goto CLEAR_INT;;
        }

        if(_TSP_ChkSecNotifyId(fltid) == FALSE)
        {
            sw_int &= ~TSP_SWINT_CTRL_FIRE;
            hw_int &= ~TSP_HWINT_TSP_SW_INT_EN;
            status = 0;
        }
        else
        {
            _s32EventId = _ptsp_res->_tspInfo.s32TspEvtId[fltid];
        }

        switch (status)
        {

        case TSP_SWINT_STATUS_SEC_RDY_CRCERR:  // CRC error But accept section ;
            //When there's no CRC in the section & E_TSP_FLT_MODE_AUTO_CRCCHK is used,
            //don't report CRC error
            if(!(_ptsp_res->_tspInfo.FltMode[fltid] & E_TSP_FLT_MODE_AUTO_CRCCHK))
        case TSP_SWINT_STATUS_SEC_CRCERR:  // Filter Didn't check CRC and Section Rdy
            {
                TSP_FLAG_SetCrcErr(fltid);                                                          // Set section event flag
            }

            if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
            {
                TSP_SetEvent(_s32EventId, TSP_EVENT_SECTION);                                                    // Wake task up
            }
#ifdef MSOS_TYPE_LINUX_KERNEL
//            TSP_OS_Print("%s[%04d]: 0x%08X\n", __FILE__, __LINE__, status);
            MDrv_TSP_SEC_SetEvent(fltid, DEMUX_EVENT_CRC_FAILED);
            wake_up_interruptible(&(tsp_wq_head[fltid]));
#endif

        if (TSP_SWINT_STATUS_SEC_CRCERR == status)
            break;

        case TSP_SWINT_STATUS_SEC_RDY:

            if ( (E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[fltid]) &&
                 (E_TSP_FLT_MODE_ONESHOT  & _ptsp_res->_tspInfo.FltMode[fltid]))
            {
                TSP_FLAG_SetFltDis(fltid);                                                    // Set section event flag
                if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                    TSP_SetEvent(_s32EventId, TSP_EVENT_DISABLEFLT);
            }

            // During over-night test, we have got section ready interrupt from non-section filter.
            // The interrupt is illegal, and will cause TSP ASSERT "HwBufId != tspInfo Bufid" in _TSP_PROC_SecRdy.
            // Since the root cause was not found yet, we add a patch here to reject the non-section filter interrupt.
            if(((_ptsp_res->_tspInfo.FltType[fltid] & E_TSP_FLT_USER_MASK) == 0) || ((_ptsp_res->_tspInfo.FltState[fltid] & E_TSP_FLT_STATE_ENABLE) == 0) )
            {
                break;
            }

            TSP_FLAG_SetSecRdy(fltid);                                                          // Set section event flag
            // @NOTE Jerry
            // It's set by user and ALL section filters should be the same beucase the TASK will race with POLL
            // Since it's for backward compatible, it's not revised at this time and may be remove in the future.
            if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
            {
                TSP_SetEvent(_s32EventId, TSP_EVENT_SECTION);                                                    // Wake task up
            }
            else if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
            {
                TSP_SetEvent(_s32EventId, TSP_EVENT_SECTION_POLL);                                               // Set POLL flags

            }

#ifdef MSOS_TYPE_LINUX_KERNEL
            wake_up_interruptible(&(tsp_wq_head[fltid]));
#endif
            break;

        case TSP_SWINT_STATUS_BUF_OVFLOW:
            if (!(TSP_FLAG_GetSecOvf(fltid, &dummy, TSP_OS_EVENT_OR))) // ignore redundant overflow
            {
                //TSP_ISR_Print("TSP_SWINT_STATUS_PKT_OVFLOW  , Flt = %ld, Buf = %ld , PID= %04lX\n",fltid,_ptsp_res->_tspInfo.Flt2Buf[fltid],_ptsp_res->_tspInfo.Pid[fltid]);
                if ( (E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[fltid]) )
                {
                    TSP_FLAG_SetFltDis(fltid);                                                          // Set section event flag
                    if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                        TSP_SetEvent(_s32EventId, TSP_EVENT_DISABLEFLT);

                }

                TSP_FLAG_SetSecOvf(fltid);             // Set section event flag
                if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                {
                    TSP_SetEvent(_s32EventId, TSP_EVENT_SECTION);                                                    // Wake task up
                }
                else if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_CB_MASK))
                {
                   TSP_SetEvent(_s32EventId, TSP_EVENT_SECTION_POLL);                                               // Set POLL flags
                }
            }

#ifdef MSOS_TYPE_LINUX_KERNEL
            MDrv_TSP_SEC_SetEvent(fltid, DEMUX_EVENT_DATA_DISCARDED);
            wake_up_interruptible(&(tsp_wq_head[fltid]));
#endif
            break;

        case TSP_SWINT_STATUS_PKT_OVRUN:
            TSP_ISR_Print("TSP_SWINT_STATUS_PKT_OVRUN [%d] \n",(unsigned int)fltid);
            break;

        case TSP_SWINT_STATUS_SEC_ERROR:
        case TSP_SWINT_STATUS_DEBUG:
            //[ISR print]
//            TSP_OS_Print("%s[%04d]: 0x%08X\n", __FILE__, __LINE__, status);
            break;

        default:
            //[ISR print]
//            TSP_OS_Print(OSAL_TSP_Print("Unknown TSP interrupt %d\n", u32EngId));
            break;
        }
    }

CLEAR_INT:

    _TSP_INT_LOCK();
    if(hw_int)
    {
        HAL_TSP_INT_ClrHW(TSP_HWINT_ALL); // @F_TODO: it's changed from U4 make sure we clr them all
        //HAL_TSP_INT_ClrHW(hw_int >> 8); // @F_TODO: it's changed from U4 make sure we clr them all
    }
    if(sw_int)
    {
        HAL_TSP_INT_ClrSW();
    }
    _TSP_INT_UNLOCK();

    TSP_OS_IntEnable();

#ifdef MSOS_TYPE_LINUX_KERNEL
    return IRQ_HANDLED;
#endif
}


static void _TSP_POLL_Proc(MS_U32 u32Events, MS_U32 u32FltId, TSP_Event *pEvent)
{
    MS_U32              flags; // dummy
//    int                 i;

#if __LEGACY__
    if ( (TSP_EVENT_PVR0_RDY_POLL | TSP_EVENT_PVR1_RDY_POLL) & u32Events )
    {
        _TSP_Proc_Pvr(u32Events, E_TSP_EVENT_CB_POLL);
    }
#endif

/*  [Eddie] Disable filter at ISR_Task
    // Check any possible filter should be disabled (POLL)
    for (i = 0; i < TSP_PIDFLT_NUM; i++)
    {
        if (TSP_FLAG_GetFltDis(i, &flags, TSP_OS_EVENT_OR_CLEAR))
        {
            TSP_LOCK();
            _TSP_FLT_Disable(i);
            TSP_UNLOCK();
        }
    }
*/
    if (TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR))
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId);
        TSP_UNLOCK();
    }

    // Process singl specified filter

    // @FIXME: Jerry
    // Check the behavior between ISR_Proc and _Poll_Proc
    // Why isn't _Proc_SecOvf first than _Proc_SecRdy as _ISR_Proc

    if ((_ptsp_res->_tspInfo.FltState[u32FltId] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[u32FltId] != E_TSP_FLT_STATE_ISR_FREE))
    {
        *pEvent = E_TSP_EVENT_DATA_IDLE;
        if (E_TSP_FAIL == _TSP_PROC_SecRdy(u32FltId, pEvent))
        {
            if (TSP_FLAG_GetSecOvf(u32FltId, &flags, TSP_OS_EVENT_OR)) // @FIXME: clear events at Proc_SecOvf?
            {
                _TSP_PROC_SecOvf(u32FltId, pEvent);
            }
            else if (TSP_FLAG_GetCrcErr(u32FltId, &flags, TSP_OS_EVENT_OR)) // @Note:  Priority Overflow > CRCError > SecRdy
            {
                _TSP_PROC_CrcErr(u32FltId, pEvent);
            }
        }
    }
}


static void _TSP_HW_ResetState(void)
{
    MS_U32              fltid;

    for (fltid = 0; fltid < (TSP_PIDFLT_NUM_ALL-1); fltid++)
    {
        _TSP_FLT_Disable(fltid);
        _TSP_FLT_Init(fltid); // state => FREE
        _TSP_FLT_Free(fltid);
    }

    for (fltid = 0; fltid < TSP_SECBUF_END_ID; fltid++)
    {
        _TSP_SEC_Init(fltid); // state => FREE
        _TSP_SEC_Free(fltid);
    }
}


static void _TSP_SW_ResetState(void)
{
    MS_U32              fltid;

    for (fltid = 0; fltid < TSP_SECFLT_END_ID; fltid++)
    {
        _ptsp_res->_tspInfo.BufState[fltid]         = E_TSP_FLT_STATE_NA;
        _ptsp_res->_tspInfo.FltMode[fltid]          = E_TSP_FLT_MODE_CONTI;
        _ptsp_res->_tspInfo.FltEvent[fltid]         = E_TSP_EVENT_DATA_IDLE;
        _ptsp_res->_tspInfo.FltCallback[fltid]      = NULL;
        _ptsp_res->_tspInfo.s32TspEvtId[fltid]      = -1;
    }

    for (fltid = 0; fltid < (TSP_PIDFLT_NUM_ALL - 1); fltid++)
    {
        _ptsp_res->_tspInfo.Pid[fltid]         = TSP_PID_NULL;
        _ptsp_res->_tspInfo.FltState[fltid]    = E_TSP_FLT_STATE_NA;
        _ptsp_res->_tspInfo.FltType[fltid]     = E_TSP_FLT_USER_NULL;
    }

}

static TSP_Result _TSP_Init(MS_BOOL bResetSW, MS_BOOL bRestoreHW)
{
    int             i;

    //Disable CPU to prevent cpu enable by other module before tsp_init.
    HAL_TSP_CPU(0);
    HAL_TSP_Power(1);//@F_TODO not implement yet disable unused clk in TSP compare with K2

    // Diable interrupt
    HAL_TSP_INT_Disable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)|(TSP_HWINT3_ALL<<16));//@F_TODO need to check what it really disabled.... 0x6c????

    // IntDisable
    // IntDetach
    TSP_OS_IntDisable();
    TSP_OS_IntDetach();

    //@F_TODO DISABLE tsif 0~3
    for (i = 0; i < TSP_TSIF_NUM; i++)
    {
        HAL_TSP_SetTSIF(i, E_TSP_TSIF_DIS, FALSE);
    }

    //HAL_TSP_HW_Lock_Init();//@NOTE HW LOCK is a mechenism to use regiter to protect register modify race between HK and TSP CPU

    HAL_TSP_Reset(1);
    HAL_TSP_Reset(0); // indirect access activated, WB reset, CMDQ reset
//    HAL_TSP_HWPatch();

    HAL_TSP_INT_ClrSW();
    HAL_TSP_INT_ClrHW(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)|(TSP_HWINT3_ALL<<16)); // @F_TODO: need to check this step with Stephen for which interrup to clear

#ifdef MERGE_STR_SUPPORT
    HAL_TSP_PktConverter_Init();
#endif

    HAL_TSP_HwPatch(); // @F_TODO no source yet
    //HAL_TSP_SetCAPath(TSP_CA_ROUTE_EN | TSP_CA_OUT_EN | TSP_CA_FLOWCTRL_EN);

/*
    // XXX: Jericho: temp solution to prevent TSP write memory
    HAL_TSP_CPU_WProtect(TRUE, 0, 0);
*/

    if (bResetSW)
    {
        _TSP_SW_ResetState();
    }

    if (bRestoreHW)
    {
        HAL_TSP_RestoreFltState();
    }
    else
    {
        _TSP_HW_ResetState();
    }

#ifndef SECURE_PVR_ENABLE
    if (TRUE != HAL_TSP_SetVQ(_ptsp_res->_tspInfo.Param.phyVQAddr, _ptsp_res->_tspInfo.Param.u32VQSize))
    {
        TSP_DBG(E_TSP_DBG_MUST, "TSP VQ is not aligned\n");
        return E_TSP_FAIL;
    }
#else
    _ptsp_res->_stSecBuf.u32BufId = 0;
    _ptsp_res->_stSecBuf.u32BufOpt = 0;
    _ptsp_res->_stSecBuf.u32BufSize = _ptsp_res->_tspInfo.Param.u32VQSize;
    _ptsp_res->_stSecBuf.phyBufAddr = _ptsp_res->_tspInfo.Param.phyVQAddr;
    //printf("[%s][%d] u32BufId %d, opte %d , bufsize %x, addr %x \n", __FUNCTION__, __LINE__, (int)_ptsp_res->_stSecBuf.u32BufId,
    //    (int)_ptsp_res->_stSecBuf.u32BufOpt, (int)_ptsp_res->_stSecBuf.u32BufSize, (int)_ptsp_res->_stSecBuf.phyBufAddr);
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_SET_VqBuf, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != E_TSP_OK)
    {
        TSP_DBG(E_TSP_DBG_MUST, "[MDRV TSP][%06d] Set VQ buf fail!\n", __LINE__);
        return E_TSP_FAIL;
    }
    HAL_TSP_VQ_Enable(TRUE);
#endif  //SECURE_PVR_ENABLE

#ifdef SECURE_PVR_ENABLE   //load fwtsp.dat from Tee
    _ptsp_res->_stSecBuf.u32BufId = 0;
    _ptsp_res->_stSecBuf.u32BufOpt = 0;
    _ptsp_res->_stSecBuf.u32BufSize = _ptsp_res->_tspInfo.Param.u32FWSize;
    _ptsp_res->_stSecBuf.phyBufAddr = _ptsp_res->_tspInfo.Param.phyFWAddr;
    //printf("[%s][%d] u32BufId %d, opte %d , bufsize %x, addr %x \n", __FUNCTION__, __LINE__, (int)_ptsp_res->_stSecBuf.u32BufId,
    //    (int)_ptsp_res->_stSecBuf.u32BufOpt, (int)_ptsp_res->_stSecBuf.u32BufSize, (int)_ptsp_res->_stSecBuf.phyBufAddr);
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != E_TSP_OK)
    {
        return E_TSP_FAIL;
    }
#endif

    if (TRUE != HAL_TSP_LoadFW(_ptsp_res->_tspInfo.Param.phyFWAddr, _ptsp_res->_tspInfo.Param.u32FWSize))
    {
        TSP_DBG(E_TSP_DBG_MUST, "TSP firmware is not aligned\n");
        return E_TSP_FAIL;
    }

    HAL_TSP_CPU(1);
#if defined (MSOS_TYPE_LINUX_KERNEL)
    mdelay(3);
#else
    MsOS_DelayTask(3);
#endif

    while (FALSE == HAL_TSP_HCMD_Alive())
    {
        TSP_OS_Print("Reset TSP CPU fail, Retry.\n");
        HAL_TSP_CPU(0);
        #ifdef SECURE_PVR_ENABLE   //load fwtsp.dat from Tee
        if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != E_TSP_OK)
        {
            return E_TSP_FAIL;
        }
        #endif
        HAL_TSP_LoadFW(_ptsp_res->_tspInfo.Param.phyFWAddr, _ptsp_res->_tspInfo.Param.u32FWSize);
        HAL_TSP_CPU(1);
        #if defined (MSOS_TYPE_LINUX_KERNEL)
            mdelay(3);
        #else
            MsOS_DelayTask(3);
        #endif
    }
    TSP_OS_Print("Reset TSP CPU Success.\n");
    TSP_OS_Print(">> FwVersion:%08X , Date:%08X \n",(unsigned int)HAL_TSP_HCMD_GetInfo(INFO_FW_VERSION),(unsigned int)HAL_TSP_HCMD_GetInfo(INFO_FW_DATE));

#ifdef SECURE_PVR_ENABLE
    _ptsp_res->_stSecBuf.u32BufId = 0xFF;
    _ptsp_res->_stSecBuf.u32BufOpt = 0;
    _ptsp_res->_stSecBuf.u32BufSize = 0;
    _ptsp_res->_stSecBuf.phyBufAddr = 0;
    if(MDrv_TSP_Ree_SendTeeCmd(E_DRVTSP_REE_TO_TEE_CMD_FW_LoadCode, (void*)(&(_ptsp_res->_stSecBuf)), sizeof(DrvTSP_SecureBuf)) != E_TSP_OK)
    {
        return E_TSP_FAIL;
    }
#endif

    HAL_TSP_INT_ClrSW();
    HAL_TSP_INT_ClrHW(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)|(TSP_HWINT3_ALL<<16)); // @FIXME : it's changed from U4 we need to make sure kaiser is not chaged

    // Implicity MDrv_TSP_Reset()
#if 1
    for(i=0; i<TSP_TSIF_NUM; i++)
    {
        switch(i)
        {
            case TSP_TSIF0:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF0, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            case TSP_TSIF1:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF1, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            case TSP_TSIF2:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF2, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
            case TSP_TSIF3:
                HAL_TSP_Filein_ByteDelay(E_FILEENG_TSIF3, 0x0A, TRUE);  //@FIXME why use 0xA
                break;
        }
    }
#else
   HAL_TSP_File_SetDelay(0x0A);                                          // Set TsDma delay, //jyliu.tsp, how come the value?
#endif

    // STC control by TSP need indirect R/W enable
    HAL_TSP_STC_Init();
   // HAL_TSP_SetSTCSynth(STC_SYNTH_DEFAULT); // @NOTE this is done in HAL_TSP_STC_Init


    TSP_OS_IntAttach((void*)_TSP_ISR);
    TSP_OS_IntEnable();

    // [URANUS] Richard: Refine the interrupt selection later.
    HAL_TSP_INT_Enable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)|(TSP_HWINT3_ALL<<16));                                     // Enable TSP hardware interrupt

    if (_ptsp_res->_tspInfo.bSWInit)
    {
        return E_TSP_OK;
    }

#if defined(__LEGACY__)
    _ptsp_res->_PvrEvtNotify = E_TSP_EVENT_DATA_IDLE;
    _ptsp_res->_PvrCallback = NULL;
#endif

    _ptsp_res->_s32MutexId = TSP_OS_MutexCreate((MS_U8*)"TSP_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
    _ptsp_res->_s32LockId =  TSP_OS_MutexCreate((MS_U8*)"TSP_Ind_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Mutex");
#ifndef MSOS_TYPE_LINUX_KERNEL
    _ptsp_res->_s32IntRegMutexId =  TSP_OS_MutexCreate((MS_U8*)"TSP_Int_Mutex");
#endif
    _ptsp_res->_s32PvrMutexId = TSP_OS_MutexCreate((MS_U8*)"TSP_Pvr_Mutex"); // MsOS_CreateMutex(E_MSOS_FIFO, "TSP_Pvr_Mutex");
#ifndef MSOS_TYPE_LINUX_KERNEL
    _s32EventId = TSP_OS_EventCreate((MS_U8*)"TSP_Event"); // MsOS_CreateEventGroup("TSP_Event");
#endif

    _ptsp_res->_tspInfo.bISRTaskEn = TRUE;
#ifndef MSOS_TYPE_LINUX_KERNEL
    // we dont create driver task in kernel mode, thus related checking should be remove too.
    _ptsp_res->_s32TaskId =  TSP_OS_TaskCreate(_TSP_ISR_Task, (MS_U8*)"TSP_ISR_Task");

    if ( _ptsp_res->_s32TaskId == -1 )
    {
        return E_TSP_FAIL;
    }
#endif

#ifndef MSOS_TYPE_LINUX_KERNEL
    if ( (_ptsp_res->_s32MutexId == -1) || (_ptsp_res->_s32LockId == -1) || (_ptsp_res->_s32PvrMutexId == -1) || (_s32EventId == -1) )
#else
    if ( (_ptsp_res->_s32MutexId == -1) || (_ptsp_res->_s32LockId == -1) || (_ptsp_res->_s32PvrMutexId == -1))
#endif
    {
        return E_TSP_FAIL;
    }

    for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
    {
        _s32SecRdyFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_SecRdy");
        if(_s32SecRdyFlags[i] == -1)
        { TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Create _s32SecRdyFlags[%d] failed", __LINE__, i); }

        _s32CrcErrFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_CrcErr");
        if(_s32CrcErrFlags[i] == -1)
        { TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Create _s32CrcErrFlags[%d] failed", __LINE__, i); }

        _s32SecOvfFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_SecOvf");
        if(_s32SecOvfFlags[i] == -1)
        { TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Create _s32SecOvfFlags[%d] failed", __LINE__, i); }

        _s32FltDisFlags[i] = TSP_OS_EventCreate((MS_U8*)"TSP_FltDis");
        if(_s32FltDisFlags[i] == -1)
        { TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Create _s32FltDisFlags[%d] failed", __LINE__, i); }

        _s32FreeFlags[i]   = TSP_OS_EventCreate((MS_U8*)"TSP_FltFree");
        if(_s32FreeFlags[i] == -1)
        { TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Create _s32FreeFlags[%d] failed", __LINE__, i); }

        _s32DoneFlags[i]   = TSP_OS_EventCreate((MS_U8*)"TSP_FltDone");
        if(_s32DoneFlags[i] == -1)
        { TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Create _s32DoneFlags[%d] failed", __LINE__, i); }

        _s32OvfRstFlags[i]= 0 ;
        _s32ModeFlags[i] = 0 ;

        _u32SecNotifyId[i] = 0;

    }

    _ptsp_res->_tspInfo.Status  = E_TSP_SW_INIT;
    _ptsp_res->_tspInfo.bSWInit = TRUE;
    _ptsp_res->_tspInfo.LastErr = E_TSP_OK;

    // Default Live Path as TSIF0
    _ptsp_res->_Current_Live = E_TSP_FLT_SRC_LIVE0;
    _ptsp_res->_Current_PVR = 0 ;

#ifdef MSOS_TYPE_LINUX_KERNEL
    for (i = 0; i < TSP_SECFLT_END_ID; i++)
    {
        init_waitqueue_head(&tsp_wq_head[i]);
    }
    memset(_u32SecEvent, 0, sizeof(_u32SecEvent));

    if(_u32KPrcEvtTblUseFlag == 0)
    {
        for (i= 0; i< DRVTSP_MAX_PROCESS_NUM; i++)
        {
            _stKModePrcEvtTbl[i].tgid = 0;
            _stKModePrcEvtTbl[i].s32TspEvtId = -1;
        }
    }
#endif

    return E_TSP_OK;

}


static void _TSP_Exit(void)
{
    MS_U32                 i;
    MS_U32                 Pcr_H, Pcr;

    HAL_TSP_INT_Disable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)|(TSP_HWINT3_ALL<<16));

    //always read PCR after PCR interrupt has been disabled or cleared to avoid that interrupt's not trggering again.
    for (i = 0; i < TSP_PCRFLT_NUM; i++)
    {
          HAL_TSP_PcrFlt_GetPcr(i, &Pcr_H, &Pcr);
    }

    TSP_OS_IntDisable();
    TSP_OS_IntDetach();

    for (i = 0; i < TSP_TSIF_NUM; i++) //@FIXME is this enough?
    {
        HAL_TSP_SetTSIF(i, E_TSP_TSIF_DIS, FALSE);
    }
    HAL_TSP_CPU(0);
    HAL_TSP_Reset(1);
    HAL_TSP_Power(0);
}

static void _TSP_SW_Exit(void)
{
    int                 i;

    if(_ptsp_res->_s32TaskId != -1)
    {
        while(!_s32_IsrTaskReturn)
        {
            TSP_SetEvent(_s32EventId, TSP_EVENT_TASKEND);

#if defined (MSOS_TYPE_LINUX_KERNEL)
            mdelay(TSP_POLLING_TASK_DELAY);
#else
            MsOS_DelayTask(TSP_POLLING_TASK_DELAY);
#endif
        }
        _s32_IsrTaskReturn = FALSE;

        _ptsp_res->_s32TaskId = -1;
        _ptsp_res->_tspInfo.bISRTaskEn = FALSE;
    }

    if(_ptsp_res->_s32MutexId != -1)
    {
        TSP_OS_MutexRelease(_ptsp_res->_s32MutexId);
        TSP_OS_MutexDelete(_ptsp_res->_s32MutexId);
        _ptsp_res->_s32MutexId = -1;
    }
    if(_ptsp_res->_s32LockId != -1)
    {
        TSP_OS_MutexRelease(_ptsp_res->_s32LockId);
        TSP_OS_MutexDelete(_ptsp_res->_s32LockId);
        _ptsp_res->_s32LockId = -1;
    }
#ifdef MSOS_TYPE_LINUX_KERNEL
    spin_lock_init(&_ptsp_res->_s32IntRegMutexId);
#else
    if(_ptsp_res->_s32IntRegMutexId != -1)
    {
        TSP_OS_MutexRelease(_ptsp_res->_s32IntRegMutexId);
        TSP_OS_MutexDelete(_ptsp_res->_s32IntRegMutexId);
        _ptsp_res->_s32IntRegMutexId = -1;
    }
#endif
    if(_ptsp_res->_s32PvrMutexId != -1)
    {
        TSP_OS_MutexRelease(_ptsp_res->_s32PvrMutexId);
        TSP_OS_MutexDelete(_ptsp_res->_s32PvrMutexId);
        _ptsp_res->_s32PvrMutexId = -1;
    }

#ifndef MSOS_TYPE_LINUX_KERNEL
    if(_s32EventId != -1)
    {
        TSP_OS_EventDelete(_s32EventId);
        _s32EventId = -1;
    }
#endif

    for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
    {
        if(_s32SecRdyFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32SecRdyFlags[i]);
            _s32SecRdyFlags[i] = -1;
        }
        if(_s32CrcErrFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32CrcErrFlags[i]);
            _s32CrcErrFlags[i] = -1;
        }
        if(_s32SecOvfFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32SecOvfFlags[i]);
            _s32SecOvfFlags[i] = -1;
        }
        if(_s32FltDisFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32FltDisFlags[i]);
            _s32FltDisFlags[i] = -1;
        }
        if(_s32FreeFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32FreeFlags[i]);
            _s32FreeFlags[i] = -1;
        }
        if(_s32DoneFlags[i] != -1)
        {
            TSP_OS_EventDelete(_s32DoneFlags[i]);
            _s32DoneFlags[i] = -1;
        }
    }
    _MDrv_TSP_FLT_ResetFltId();

#ifdef SECURE_PVR_ENABLE
        MDrv_TSP_Ree_TeeCmdSystem_Exit();
#endif

    _ptsp_res->_tspInfo.bSWInit = FALSE;
}

static MS_U32 _TSP_Drv_DmxFlow2PVREng_Mapping(DRV_TSP_FLOW DmxFlow)
{
    switch (DmxFlow)
    {
        case E_DRV_TSP_FLOW_PVR0:
            return 0;
            break;
        case E_DRV_TSP_FLOW_PVR1:
            return 1;
            break;
        case E_DRV_TSP_FLOW_PVR2:
            return 2;
            break;
        case E_DRV_TSP_FLOW_PVR3:
            return 3;
            break;
        default:
            return 0xFFFFFFFF;
            break;
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pu32ResMemAddr                \b IN: Pointer to store resource memory address
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_InitLibResource(void *pu32ResMemAddr)
{
    TSP_CHECK((pu32ResMemAddr != 0),
        "[TSP_ERROR][%s][%06d] pointer is NULL!\n", __FUNCTION__, __LINE__);

    MS_VIRT u32Base;
    MS_PHY u32Size;

    _ptsp_res = (TSP_RESOURCE_PRIVATE*)pu32ResMemAddr;

    //  For multi-process use case, driver will direct return to avoid re-init HW again on api layer thus the second process will not set bank.
    // And the parameter on hal layer is static (ex:bank base), so the bank base on the second process will be NULL.
    // We need to avoid this case thus we set bank again on initLibResource function.
    if(_ptsp_res->_tspInfo.bSWInit != TRUE)
    {
        if(_TSP_InitResource(_ptsp_res) == FALSE)
            return E_TSP_FAIL;
    }
    else
    {
        if(_bBankInit == FALSE)
        {
            MDrv_MMIO_Init();
            if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
            {
                TSP_OS_Print("Get MS_MODULE_PM fail\n");
                MS_ASSERT(0);
            }
            HAL_TSP_SetBank(u32Base);
            _bBankInit = TRUE;
        }
    }

    return E_TSP_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize TSP driver and TSP engine
/// @return TSP_Result
/// @note
/// It should be called before calling any other TSP DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Init(TSP_InitParam *pParam)
{
    MS_VIRT             u32Base;
    MS_PHY              u32Size;
    MS_U32              *u32Check;
    TSP_Result          ret;

#ifdef SECURE_PVR_ENABLE
    if(MDrv_TSP_Ree_TeeCmdSystem_Init() == E_TSP_FAIL)
    {
        MS_DEBUG_MSG(TSP_DBG ("[%s] Register Mailbox failed!\n", __FUNCTION__));
        return E_TSP_FAIL;
    }
#endif //SECURE_PVR_ENABLE

    // MDrv_DSCMB_Init(); // sometimes good/bad // random
    MDrv_MMIO_Init();
#if 1
    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
    {
        TSP_OS_Print("Get MS_MODULE_PM fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_SetBank(u32Base);

    TSP_DBG(E_TSP_DBG_INFO, "MMIO(MS_MODULE_PM) 0x%08x\n", u32Base);
#else
    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_TSP))
    {
        TSP_OS_Print("Get MS_MODULE_TSP fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_RegMap(u32Base);
    TSP_DBG(E_TSP_DBG_INFO, "MMIO(MS_MODULE_TSP) 0x%08lx\n", u32Base);

    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
    {
        TSP_OS_Print("Get MS_MODULE_HW fail\n");
        MS_ASSERT(0);
    }
    // MDrv_DSCMB_Init(); // sometimes good/bad // random
    HAL_TSP_HW_RegMap(u32Base);
    TSP_DBG(E_TSP_DBG_INFO, "MMIO(MS_MODULE_HW) 0x%08lx\n", u32Base);


    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_PM))
    {
        TSP_OS_Print("Get MS_MODULE_PM fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_PM_RegMap(u32Base);
    TSP_DBG(E_TSP_DBG_INFO, "MMIO(MS_MODULE_PM) 0x%08lx\n", u32Base);
#endif

    TSP_DBG(E_TSP_DBG_MUST, "************************ Start TSP_Init ************************\n");



    u32Check = (MS_U32*)MsOS_PA2KSEG1(pParam->phyFWAddr);

    if (strcmp((char*)u32Check,FWTSP_CHECK_CODE) != 0 )
    {
        TSP_OS_Print("**** [TSP Firmware] Check Fail ****\n");
    }
    TSP_OS_Print(" %s : ",(char*)u32Check);

    memcpy(&(_ptsp_res->_tspInfo.Param), pParam, sizeof(TSP_InitParam));
    if (E_TSP_OK != (ret = _TSP_Init(TRUE, FALSE)))
    {
        TSP_DBG(E_TSP_DBG_MUST, "************************   TSP [FAIL]   ************************\n");
        return ret;
    }
    // MDrv_DSCMB_Init(); // bad

    TSP_DBG(E_TSP_DBG_MUST, "************************* End TSP_Init *************************\n");


    //[NOTE] Switch Uart0 port to TSP
    #if SWITCH_TSP_UART
    _TSP_Switch_Uart();
    #endif

    return E_TSP_OK;
}





//-------------------------------------------------------------------------------------------------
/// Close TSP driver and reset software/register state
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Exit(void)
{
    TSP_DBG(E_TSP_DBG_TRACK , "%s\n", __FUNCTION__);

    MS_PHY  u32Size=0;
    MS_VIRT u32Base=0;
    MS_BOOL bEnterEntry = FALSE;
    MS_U32  i = 0;

    TSP_DBG(E_TSP_DBG_MUST, "============[%s] TSP Exit Start ==============\n",__FUNCTION__);

    if (_ptsp_res->_s32MutexId != -1 )
    {
        TSP_ENTRY();
        bEnterEntry = TRUE;
    }

#ifndef MSOS_TYPE_LINUX_KERNEL
    if (_ptsp_res->_s32IntRegMutexId != -1 )
    {
        _TSP_INT_LOCK();
    }
#endif

    if (_ptsp_res->_s32PvrMutexId != -1 )
    {
        TSP_PVR_ENTRY();
    }


    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32Size, MS_MODULE_HW))
    {
        TSP_OS_Print("Get MS_MODULE_PM fail\n");
        MS_ASSERT(0);
    }
    HAL_TSP_SetBank(u32Base);
    TSP_DBG(E_TSP_DBG_INFO, "MMIO(MS_MODULE_PM) 0x" MPRI_VIRT "\n", u32Base);


    if (!HAL_TSP_HCMD_Alive())
    {

        MS_U32                 Pcr_H, Pcr;

        HAL_TSP_Power(1);

        HAL_TSP_INT_Disable(TSP_HWINT_ALL|(TSP_HWINT2_ALL<<8)|(TSP_HWINT3_ALL<<16));

        //always read PCR after PCR interrupt has been disabled or cleared to avoid that interrupt's not trggering again.
        for (i = 0; i < TSP_PCRFLT_NUM; i++)
        {
            HAL_TSP_PcrFlt_GetPcr(i, &Pcr_H, &Pcr);
        }
        //HAL_TSP_HW_Lock_Init(); //@F_TODO HW lock is a mechanism we use to avoid race between HK and TSP CPU for those register like filter disable and stc but we dont need that anymore

        HAL_TSP_Reset(1);
        HAL_TSP_Reset(0); // indirect access activated, WB reset, CMDQ reset
    }
    _ptsp_res->_tspInfo.Status = E_TSP_SW_EXIT;

    // SW driver state
    _TSP_SW_ResetState();
    // HW filter state
    _TSP_HW_ResetState();

    _TSP_Exit();
    _TSP_SW_Exit();

    if ( TRUE == bEnterEntry )
    {
        MDrv_SEM_Unlock(E_SEM_CA);
    }

    TSP_DBG(E_TSP_DBG_MUST, "============[%s] TSP Exit End ============\n",__FUNCTION__);

    return E_TSP_OK;
}


//-------------------------------------------------------------------------------------------------
/// Poweroff TSP driver and TSP engine
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PowerOff(void)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    _ptsp_res->_tspInfo.Status = E_TSP_SW_POWEROFF;

    _TSP_Exit();
    _TSP_SW_Exit();
    MDrv_SEM_Unlock(E_SEM_CA);

    return E_TSP_OK;
}


///[RESERVED]
TSP_Result MDrv_TSP_Reset(void)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    if(_ptsp_res->_tspInfo.bSWInit != TRUE)
    {
        TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Please call MDrv_TSP_Init() first\n", __FUNCTION__, __LINE__);
        return E_TSP_FAIL;
    }

    TSP_ENTRY();

    _TSP_Init(TRUE, FALSE);

    TSP_RETURN(E_TSP_OK);

}


//[RESERVED]
TSP_Result MDrv_TSP_Suspend(void)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    _ptsp_res->_tspInfo.Status = E_TSP_SW_SUSPEND;

    HAL_TSP_SaveFltState();

    _TSP_Exit();
    _TSP_SW_Exit();

    _ptsp_res->_tspInfo.bSuspend = TRUE;

    TSP_RETURN(E_TSP_OK);
}


//[RESERVED]
TSP_Result MDrv_TSP_Resume(MS_U32 u32FWAddr, MS_U32 u32FWSize)
{
    TSP_Result          ret;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    // we can't operate TSP register before power on
    // HAL_TSP_Reset(1);
    // HAL_TSP_Reset(0);
    // HAL_TSP_HwPatch();
    // MsOS_DelayTask(500);

    _ptsp_res->_tspInfo.Status = E_TSP_SW_RESUME;
    _ptsp_res->_tspInfo.bSuspend = FALSE;
    _ptsp_res->_tspInfo.Param.phyFWAddr = u32FWAddr;
    _ptsp_res->_tspInfo.Param.u32FWSize = u32FWSize;

    ret = _TSP_Init(FALSE, TRUE);

    return ret;
}

TSP_Result MDrv_TSP_PVR_SetFlowSource(MS_U32 u32Eng, MS_U32 u32PvrSrc)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & u32PvrSrc;
    TSP_PIDFLT_SRC ePidFltSrc = _TSP_DrvHal_FltSrcMapping(u32FltSrc);
    MS_U32 u32Tid = HAL_TSP_FltSrc2PktDmx_Mapping(ePidFltSrc);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32Tid);

    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        HAL_PVR_Init(u32Eng, u32Tid);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SetFlowSource(DRV_TSP_FLOW DmxFlow, TSP_TSId eTid)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    // @F_TODO add another layer for adaption, concider for flexibility
    // @NOTE PVR has no source mmfi take care
    // @NOTE setting FIFO SRC is moved to MDrv_TSP_FLT_Enable()
    // @NOTE because setting flowset playback may be for record...
    if (   (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK0) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK1)
        || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK2) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK3)
        || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK4) || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK5)
        || (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK6)    )
    {
        /*
        if (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK0)
        {
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO, eTid); //@FIXME change pktdmx id don't use drv.h enum as parameter to hal layer
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_VIDEO, eTid);
        }
        else if (DmxFlow == E_DRV_TSP_FLOW_PLAYBACK1)
        {
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_AUDIO2 , eTid); //@FIXME change pktdmx id don't use drv.h enum as parameter to hal layer
            HAL_TSP_FIFO_SetSrc(E_TSP_DST_FIFO_VIDEO3D, eTid);
        }
        */
        TSP_RETURN(E_TSP_OK);
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_PVR0) || (DmxFlow == E_DRV_TSP_FLOW_PVR1) || (DmxFlow == E_DRV_TSP_FLOW_PVR2) || (DmxFlow == E_DRV_TSP_FLOW_PVR3))
    {
        MS_U32 u32pktDmxId = (MS_U32)eTid;
        MS_U32 u32Eng = _TSP_Drv_DmxFlow2PVREng_Mapping(DmxFlow);
        TSP_SRC_SEQ ePktDmx = HAL_TSP_Eng2PktDmx_Mapping(u32Eng);
        switch(ePktDmx)
        {
            case E_TSP_SRC_PKTDMX0:
                u32pktDmxId = 0;
                break;
            case E_TSP_SRC_PKTDMX1:
                u32pktDmxId = 1;
                break;
            case E_TSP_SRC_PKTDMX2:
                u32pktDmxId = 2;
                break;
            case E_TSP_SRC_PKTDMX3:
                u32pktDmxId = 3;
                break;
            case E_TSP_SRC_PKTDMX4:
                u32pktDmxId = 4;
                break;
            case E_TSP_SRC_PKTDMX5:
                u32pktDmxId = 5;
                break;
            default:
                u32pktDmxId = 0;
                TSP_DBG(E_TSP_DBG_MUST, "[TSP_ERR][%s][%d] Wrong PktDmx type!!!\n",__FUNCTION__,__LINE__);
                break;
        }

        if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
        {
            HAL_PVR_Init(u32Eng, u32pktDmxId);
        }
        else
        {
            TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
        }
    }
    else if ((DmxFlow == E_DRV_TSP_FLOW_TSO0) || (DmxFlow == E_DRV_TSP_FLOW_TSO1))
    {
        //@F_TODO not implement yet
        //@F_TODO remember to change TSO src to pktdmx
        TSP_RETURN(E_TSP_FAIL);
    }
    else
    {
        // @FIXME add error msg
        TSP_RETURN(E_TSP_FAIL);
    }


    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_SelPad(TSP_TSIF eTSIF, TSP_TSPad eTSPad)
{
    TSP_Result          ret;
    TSP_TS_PAD          ePad = E_TSP_TS_PAD_INVALID;
    MS_U32 u32TSIF = 0;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    ePad = _TSP_DrvHal_PadMapping(eTSPad);

    if((eTSPad >= E_TSP_TS_PAD0_3WIRE) && (eTSPad <= E_TSP_TS_PAD7_3WIRE))
    {
        HAL_TSP_PAD_3Wire(ePad, TRUE);
    }
    else
    {
        HAL_TSP_PAD_3Wire(ePad, FALSE);
    }

    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

    if (HAL_TSP_TSIF_SelPad(u32TSIF, ePad))
    {
        _ptsp_res->TsIf_Src[u32TSIF].ePad= eTSPad;
        ret = E_TSP_OK;
    }
    else
    {
        ret = E_TSP_FAIL;
    }

    TSP_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
///// Set TSP MUX Clk Invert mode
///// @param  u16TSIF                \b IN: TS MUX Index
///// @param  bClkInv                \b IN: Clock Invert
///// @return TSP_Result
///// @note
///// Set TSP PAD Clk gen invert if the bClkInv is TRUE
///// u16TSIF = TSMux0 , TSMux1 ,TSMux2
/////
///// @sa
////-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SelPad_ClkInv(TSP_TSIF eTSIF ,MS_BOOL bClkInv)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    MS_U32 u32TSIF;
    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

    _ptsp_res->TsIf_Src[u32TSIF].bClkInv = bClkInv;

    HAL_TSP_TSIF_SelPad_ClkInv(u32TSIF , bClkInv);

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
///// Set TSP MUX Clk disable mode
///// @param  u16TSIF                \b IN: TS MUX Index
///// @param  bClkdis                \b IN: Clock Disable
///// @return TSP_Result
///// @note
///// Set TSP PAD Clk gen disabled if the bClkDis is TRUE
///// u16TSIF = TSMux0 , TSMux1 ,TSMux2
/////
///// @sa
////-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SelPad_ClkDis(TSP_TSIF eTSIF ,MS_BOOL bClkDis)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    MS_U32 u32TSIF;
    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

    HAL_TSP_TSIF_SelPad_ClkDis(u32TSIF , bClkDis);

    TSP_RETURN(E_TSP_OK);
}


TSP_Result MDrv_TSP_TSO_ConfigPad(MS_U32 u32TSOEng, TSP_TSPad eTSPad)
{
    TSP_Result eRet = E_TSP_OK;
    TSP_TS_PAD ePad = E_TSP_TS_PAD_INVALID;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_ENTRY();

    HAL_TSO_SetTSOOutMUX(TRUE);

    ePad = _TSP_DrvHal_PadMapping(eTSPad);
    if(!HAL_TSP_TSO_TSIF_SelPad(u32TSOEng, ePad))
        eRet = E_TSP_FAIL;

    TSP_RETURN(eRet);
}

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
/// @sa MDrv_TSP_PidFlt_Alloc, MDrv_TSP_PidFlt_Enable, E_TSP_FLT_TYPE_AUDIO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetTSIF(TSP_TSIF eTSIF, TSP_TSIFCfg u32Cfg, MS_BOOL bFileIn)
{
    TSP_Result          ret;
    TSP_TSIF_CFG        cfg = E_TSP_TSIF_CFG_DIS;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    if (u32Cfg & E_TSP_TSIF_EN)
    {
        cfg |= E_TSP_TSIF_CFG_EN;
    }
    if (u32Cfg & E_TSP_TSIF_PARL)
    {
        cfg |= E_TSP_TSIF_CFG_PARA;
    }
    if (u32Cfg & E_TSP_TSIF_EXTSYNC)
    {
        cfg |= E_TSP_TSIF_CFG_EXTSYNC;
    }
    if (u32Cfg & E_TSP_TSIF_CFG_BITSWAP)
    {
        cfg |= E_TSP_TSIF_CFG_BITSWAP;
    }
    if (u32Cfg & E_TSP_TSIF_3WIRE)
    {
        cfg |= E_TSP_TSIF_CFG_3WIRE;
    }

    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
    MS_U32 u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, bFileIn);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

    if(!bFileIn)
    {
        _ptsp_res->TsIf_Src[u32TSIF].bExtSync = !!(u32Cfg & E_TSP_TSIF_EXTSYNC);
        _ptsp_res->TsIf_Src[u32TSIF].bParallel = !!(u32Cfg & E_TSP_TSIF_PARL);
    }
#ifdef MERGE_STR_SUPPORT
    else
        HAL_TSP_Reset_TSIF_MergeSetting(u32TSIF);
#endif
    if (HAL_TSP_SetTSIF(u32TSIF, cfg, bFileIn))
    {
        ret = E_TSP_OK;
    }
    else
    {
        ret = E_TSP_FAIL;
    }

    TSP_RETURN(ret);
}

/*
TSP_Result MDrv_TSP_SetOperateMode(MS_U32 u32EngId, TSP_CtrlMode eCtrlMode) // @FIXME
{
    TSP_ENTRY();

    if (u32EngId >= TSP_ENGINE_NUM)
    {
        TSP_RETURN(TSP_FAIL);
    }

    if ((HAL_TSP_GetCtrlMode() & TSP_CTRL_TSIF0_FILE_EN) ||
        (eCtrlMode == E_TSP_CTRL_MODE_MEM))
    {
        if (!HAL_TSP_CmdQ_IsEmpty())
        {
            TSP_RETURN(TSP_FAIL);
        }
        HAL_TSP_CmdQ_TsDma_Reset();
    }

    switch (eCtrlMode)
    {
    case E_TSP_CTRL_MODE_TS0:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_SetCA(TSP_CA_INPUT_TSIF0_LIVEIN|TSP_CA_OUTPUT_PLAY_LIVE); //|TSP_CA_AVPAUSE);
#if 0
        HAL_TSP_DoubleBuf_Disable();
        HAL_TSP_SetCtrlMode(TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 0);
#else
        HAL_TSP_SetTSIF(0, TSP_TSIF0_EN  | TSP_TSIFx_EXTSYNC | TSP_TSIFx_PARL, FALSE);
#endif
        break;
    case E_TSP_CTRL_MODE_TS1:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_DoubleBuf_Disable();
        HAL_TSP_SetCtrlMode(TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST, 1);
        break;
    case E_TSP_CTRL_MODE_MEM:
        HAL_TSP_PS_Path_Disable();
        HAL_TSP_DoubleBuf_En(DOUBLE_BUFFER_SWITCH);        // [OBERON]:Enable double buffer, set filein->pinpon livein->single
        HAL_TSP_SetCA(TSP_CA_INPUT_TSIF0_FILEIN|TSP_CA_OUTPUT_PLAY_FILE|TSP_CA_AVPAUSE);
        HAL_TSP_SetCtrlMode(TSP_CTRL_CPU_EN | TSP_CTRL_SW_RST| TSP_CTRL_TSIF0_FILE_EN, 2);
        break;
    default:
        TSP_RETURN(TSP_FAIL);
        break;
    }

    TSP_RETURN(TSP_OK);

}
*/

//-------------------------------------------------------------------------------------------------
// Ask TSP to process section/pvr data for Non-OS driver. Calling this function in OS environment causes nothing
// @OBSOLETED
// @param  u32EngId                 \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32FltId                 \b IN: Which filter id to process. 0xFFFFFFFF means process all filters
// @param  u32Timeout               \b IN: Max time for TSP to process
// @return TSP_Result
// @note
// It's only for backward compatible
// @seealso
// MDrv_TSP_FLT_SetCallback
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Proc(MS_U32 u32TSPId, MS_U32 u32FltId, TSP_Event* pEvent)                        // for non-OS TSP scheduling
{
    MS_U32              events = 0;

    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);

    if((u32FltId >= _tspFltIdMap.u32PcrFltId_Start) && (u32FltId <= _tspFltIdMap.u32RecFltIdx))
        return E_TSP_OK;

    TSP_ENTRY();

    if (_ptsp_res->_tspInfo.FltState[u32FltId] == E_TSP_FLT_STATE_NA)
    {
        TSP_RETURN(E_TSP_FAIL);
    }

    if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        TSP_OS_Print("[%s] Flt %d is CallBack Mode Filter, Polling this filter will cause unexpected result !!\n",__FUNCTION__,(unsigned int)u32FltId);
        //*pEvent = E_TSP_EVENT_DATA_IDLE ;
        //TSP_RETURN(E_TSP_FAIL_NODATA);
    }// Driver do not support Callback mode and Polling mode coexistance at the same filter.


    // @FIXME Jerry
    // For legacy driver compatibility, it call polling and hopes polling will call it back in the same context.
    // Does this kind of misbehavior still live?
    TSP_UNLOCK();
    // @NOTE Jerry
    // Clear poll events directly in case there has any incoming event before _TSP_ISR_Proc to clear
    TSP_GetEvent(_s32EventId, &events, TSP_POLL_EVENTS, TSP_OS_EVENT_OR_CLEAR, 0);
    _TSP_POLL_Proc(events, u32FltId, pEvent);
    TSP_LOCK();

    if (*pEvent == E_TSP_EVENT_DATA_IDLE )
    {
        TSP_RETURN(E_TSP_FAIL_NODATA);
    }
    else
    {
        TSP_RETURN(E_TSP_OK);
    }
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSP Path Id
/// @param  eResType        \b IN: TSP path type
/// @param  pu32PathId      \b OUT: TSP path id. If no free path ID, it will be 0xFFFFFFFF and return FAIL.
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Path_Alloc(DRV_TSP_PATH_TYPE eResType, MS_U32 *pu32PathId)
{
    TSP_Result  ret = E_TSP_FAIL;

    TSP_ENTRY();

    ret = _TSP_Path_Alloc(eResType, pu32PathId);

    if(ret == E_TSP_OK)
    {
        if( (eResType == E_DRV_TSP_PATH_LIVE) || (eResType == E_DRV_TSP_PATH_FILE) )
        {
            _ptsp_res->_bIsTspPathEverAlloc = TRUE;
        }
    }

    TSP_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Free TSP Path Id
/// @param  eResType        \b IN: TSP path type
/// @param  u32PathId       \b IN: TSP path id
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_Path_Free(DRV_TSP_PATH_TYPE eResType, TSP_TSIF ePath)
{
    TSP_Result  ret = E_TSP_FAIL;

    TSP_ENTRY();

    ret = _TSP_Path_Free(eResType, ePath);

    TSP_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Allocate STC engine.
/// @param  pu32EngId                  \b OUT: STC engine id. If no free STC engine, it will be 0xFFFFFFFF and return FAIL.
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_Alloc(MS_U32 *pu32EngId)
{
    TSP_Result ret;

    TSP_ENTRY();

    ret = _TSP_STC_Alloc(pu32EngId);

    if(ret == E_TSP_OK)
    {
        _ptsp_res->_bIsStcEverAlloc = TRUE;
    }

    TSP_RETURN(ret);
}


//-------------------------------------------------------------------------------------------------
/// Free STC engine
/// @param  u32EngId                  \b IN: index of STC engine to be free
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_Free(MS_U32 u32EngId)
{
    TSP_Result ret = E_TSP_FAIL;

    TSP_ENTRY();

    ret = _TSP_STC_Free(u32EngId);

    TSP_RETURN(ret);
}

//-------------------------------------------------------------------------------------------------
/// Free STC engine
/// @param  u32PcrFltId                  \b IN: index of PCR PID filter
/// @param  pu32EngId                    \b Out: index of STC engine which u32PcrFltId mapping to
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_PcrId_To_StcId(MS_U32 u32PcrFltId,MS_U32 *pu32EngId)
{
    TSP_Result ret = E_TSP_FAIL;

    TSP_ENTRY();

    ret = _TSP_PcrId_To_StcId(u32PcrFltId, pu32EngId);

    TSP_RETURN(ret);
}

void _MDrv_TSP_FLT_RemapFltId(void)
{
    TSP_DBG(E_TSP_DBG_TRACK, "[%s][%d]\n", __FUNCTION__, __LINE__);
    // 255 for Err Flt
    // 254 for REC Flt
    // 253 ~ (254 - PCR_NUM) for PCR Flt
    // others for Pid Flt/Sec Flt/Sec Buf
    _tspFltIdMap.u32PidFltIdAll = 254;

    _tspFltIdMap.u32RecFltIdx = 254;
    _tspFltIdMap.u32PcrFltId_End = _tspFltIdMap.u32RecFltIdx;
    _tspFltIdMap.u32PcrFltId_Start = _tspFltIdMap.u32PcrFltId_End - TSP_PCRFLT_NUM;

    _tspFltIdMap.u32PidFltId_End = _tspFltIdMap.u32PcrFltId_Start;

#if (TSP_SECFLT_END_ID > 255)
    _tspFltIdMap.u32SecFltId_End = _tspFltIdMap.u32PcrFltId_Start;
    _tspFltIdMap.u32SecBufId_End = _tspFltIdMap.u32PcrFltId_Start;
#endif

    _tspFltIdMap.u32PidFltId_Start = TSP_PIDFLT_START_ID;
    _tspFltIdMap.u32SecFltId_Start = TSP_SECFLT_START_ID;
    _tspFltIdMap.u32SecBufId_Start = TSP_SECBUF_START_ID;

}
void _MDrv_TSP_FLT_ResetFltId()
{
    TSP_DBG(E_TSP_DBG_TRACK, "[%s][%d]\n", __FUNCTION__, __LINE__);

    _tspFltIdMap.u32SecFltId_Start    = TSP_SECFLT_START_ID;
    _tspFltIdMap.u32SecFltId_End      = TSP_SECFLT_END_ID;
    _tspFltIdMap.u32SecBufId_Start    = TSP_SECBUF_START_ID;
    _tspFltIdMap.u32SecBufId_End      = TSP_SECBUF_END_ID;
    _tspFltIdMap.u32PidFltId_Start    = TSP_PIDFLT_START_ID;
    _tspFltIdMap.u32PidFltId_End      = TSP_PIDFLT_END_ID;
    _tspFltIdMap.u32PcrFltId_Start    = TSP_PCRFLT_START_ID;
    _tspFltIdMap.u32PcrFltId_End      = TSP_PCRFLT_END_ID;
    _tspFltIdMap.u32RecFltIdx         = TSP_RECFLT_IDX;

    _tspFltIdMap.u32PidFltIdAll       = TSP_PIDFLT_NUM_ALL;



}
//-------------------------------------------------------------------------------------------------
/// Allocate a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  eFilterType             \b IN: type of PID filter to be allocated
/// @param  pu32PidFltId            \b OUT: pointer of PID filter id return
/// @return TSP_Result
/// @note
/// These filter types have to select a section filter\n
/// @ref E_TSP_FLT_TYPE_SECTION\n
/// @ref E_TSP_FLT_TYPE_PCR\n
/// @ref E_TSP_FLT_TYPE_PES\n
/// @note
/// These filter types also have to setup section buffer for data output\n
/// @ref E_TSP_FLT_TYPE_SECTION\n
/// @ref E_TSP_FLT_TYPE_PES\n
/// @sa MDrv_TSP_PidFlt_SelSecFlt, MDrv_TSP_SecFlt_SetBuffer
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_Alloc(MS_U32 u32TSPId, TSP_FltType eFltType, MS_U32 *pu32FltId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "[%s][%d]\n", __FUNCTION__, __LINE__);
    TSP_Result eResult = E_TSP_FAIL;
    if(!_bIsFltMapped)
    {
        MS_U32  u32CapInfo[2] = {0};

        HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, u32CapInfo);
        if(u32CapInfo[0] > 0xFF)
        {
            _MDrv_TSP_FLT_RemapFltId();
        }
        _bIsFltMapped = TRUE;
    }

    eResult = MDrv_TSP_FLT_Alloc_Common(u32TSPId, eFltType, pu32FltId);

    if( eResult !=  E_TSP_OK)
    {
        return E_TSP_FAIL;
    }
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_FLT_Alloc_Ex(MS_U32 u32TSPId, TSP_FltType eFltType, MS_U32 *pu32FltId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "[%s][%d]\n", __FUNCTION__, __LINE__);
    TSP_Result eResult = E_TSP_FAIL;
    eResult = MDrv_TSP_FLT_Alloc_Common(u32TSPId, eFltType, pu32FltId);
    if( eResult !=  E_TSP_OK)
    {
        return E_TSP_FAIL;
    }
    return E_TSP_OK;

}

TSP_Result MDrv_TSP_FLT_Alloc_Common(MS_U32 u32TSPId, TSP_FltType eFltType, MS_U32 *pu32FltId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "[%s][%d]\n", __FUNCTION__, __LINE__);
    //We do nothing when create dmx flt to record
    if( E_TSP_FLT_FIFO_REC == (eFltType & E_TSP_FLT_FIFO_MASK) )
    {
        MS_U32 u32CapInfo[2] = {0};

        HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, u32CapInfo);
        *pu32FltId = u32CapInfo[0];
        return E_TSP_OK;
    }

    REG_SecFlt *        pSecFlt=0;
    MS_U32              type;
    int                 start_id, end_id, i;
    MS_U32              u32StcEng = 0xFFFFFFFF;

    TSP_ENTRY();

    *pu32FltId = 0xFFFFFFFF;

    if ( E_TSP_FLT_USER_PCR == (eFltType & E_TSP_FLT_USER_MASK) )
    {
        if(_TSP_STC_Alloc(&u32StcEng) == FALSE)
        {
            TSP_DBG(E_TSP_DBG_ERROR, "[%04d] No free STC !!\n", __LINE__);
            TSP_RETURN(E_TSP_FAIL);
        }

        start_id = _tspFltIdMap.u32PcrFltId_Start;
        end_id   = _tspFltIdMap.u32PcrFltId_End;
    }
    else if( (E_TSP_FLT_USER_SEC == (eFltType & E_TSP_FLT_USER_MASK)) ||
        (E_TSP_FLT_USER_PES == (eFltType & E_TSP_FLT_USER_MASK)) ||
        (E_TSP_FLT_USER_PKT == (eFltType & E_TSP_FLT_USER_MASK)) ||
        (E_TSP_FLT_USER_TTX == (eFltType & E_TSP_FLT_USER_MASK)) )
    {
        start_id = _tspFltIdMap.u32SecFltId_Start;
        end_id   = _tspFltIdMap.u32SecFltId_End;
    }
    else
    {
        end_id = _tspFltIdMap.u32PidFltId_End;
#if 0
        if (E_TSP_FLT_FLAG_CA & eFltType)
        {
            start_id = TSP_CAFLT_0;
        }
        else
        {
            start_id = 0;
        }
#else
        start_id = _tspFltIdMap.u32PidFltId_Start;
#endif
    }

    if(TSP_NEED_SEC_FILTER(eFltType))
    {
        for (i = start_id; i < end_id; i++) // check which filter is free
        {
            if (_ptsp_res->_tspInfo.FltState[i] == E_TSP_FLT_STATE_FREE)
            {
            //if TSP support PCR hardware , we don't use section filter to capture PCR from TS packet
            //otherwise , try to allocate it
            #if HW_PCRFLT_ENABLE
                if(_tspFltIdMap.u32PcrFltId_Start <= start_id)
                {
                    break;
                }
            #endif
                pSecFlt = &(_REGSec->Flt[i]);
                if (HAL_TSP_SecFlt_TryAlloc(pSecFlt, (MS_U16)u32TSPId))
                {
                    break;
                }
            }
        }
        if (i >= end_id)
        {
            TSP_DBG(E_TSP_DBG_ERROR, "[%04d] No free fitler found 0x%02x\n", __LINE__, (MS_U32)i);
            TSP_RETURN(E_TSP_FAIL);
        }

#ifdef MSOS_TYPE_LINUX_KERNEL
        MS_U32 u32ii;

        for(u32ii = 0; u32ii < DRVTSP_MAX_PROCESS_NUM; u32ii++)
        {
            if(_u32KPrcEvtTblUseFlag & (1 << u32ii))
            {
                if(_stKModePrcEvtTbl[u32ii].tgid == current->tgid)
                {
                    _ptsp_res->_tspInfo.s32TspEvtId[i] = _stKModePrcEvtTbl[u32ii].s32TspEvtId;
                    break;
                }
            }
        }
#else
        _ptsp_res->_tspInfo.s32TspEvtId[i] = _s32EventId;
#endif
    }
    else
    {
        for (i = end_id - 1; i >= start_id; i--) // check which filter is free, allocate from back
            if (_ptsp_res->_tspInfo.FltState[i] == E_TSP_FLT_STATE_FREE)
                break;
        //when section filters are less than PID filters
    #if (TSP_SECFLT_END_ID != TSP_PIDFLT_REC_NUM) // for coverity
        if ((i < _tspFltIdMap.u32SecFltId_End) && (_tspFltIdMap.u32SecFltId_End < _tspFltIdMap.u32PidFltId_End))
    #else
        if (i < _tspFltIdMap.u32SecFltId_End)
    #endif
        {
            TSP_DBG(E_TSP_DBG_ERROR, "[%04d] Warning, PID filter 0x%02x for section is allocated for non-section usage. \n", __LINE__, (MS_U32)i);
        }
        if (i < start_id)
        {
            TSP_DBG(E_TSP_DBG_ERROR, "[%04d] No free fitler found 0x%02x\n", __LINE__, (MS_U32)i);
            TSP_RETURN(E_TSP_FAIL);
        }
    }


    //[NOTE] Referace the comment of definition of _ptsp_res->_Current_Live.
#if 0    //@F_TODO do we have to support this here?
    if ((eFltType & E_TSP_FLT_SRC_TSIF0) && (_ptsp_res->_Current_Live != E_TSP_FLT_SRC_TSIF0)) // if current live not @ playback channel
    {
        if (FALSE == (E_TSP_FLT_FIFO_MASK & eFltType ))  // For A/V/PCR are always belone to channel playback.
        {
            eFltType = (eFltType & ~E_TSP_FLT_SRC_MASK) | _ptsp_res->_Current_Live ;
        }
    }
#endif

    _ptsp_res->_tspInfo.FltType[i]  = eFltType;

    _TSP_FLT_Init(i);

    if ( E_TSP_FLT_USER_PCR == (eFltType & E_TSP_FLT_USER_MASK) )
    {
        _TSP_STC_SetPcrFlt(u32StcEng, (MS_U32)i);
    }

    _ptsp_res->_tspInfo.FltState[i] = E_TSP_FLT_STATE_ALLOC;
    _ptsp_res->_tspInfo.OwnerId[i]  = (EN_TSP_Id)u32TSPId;

    // Select FLT in
    //pPidFlt = &(_REGPid->Flt[i]);

    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & eFltType;
    TSP_PIDFLT_SRC eTspPidSrc = _TSP_DrvHal_FltSrcMapping(u32FltSrc);
    MS_U32 u32PktDmx = HAL_TSP_FltSrc2PktDmx_Mapping(eTspPidSrc);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32PktDmx);

    //if TSP support PCR hardware , we don't use section filter to capture PCR from TS packet
    //otherwise , set source paket demux of section filter
#if HW_PCRFLT_ENABLE
    if(E_TSP_FLT_USER_PCR != (eFltType & E_TSP_FLT_USER_MASK))
#endif
        HAL_TSP_PidFlt_SetFltIn(i,u32PktDmx);

#ifdef MERGE_STR_SUPPORT
    {
        MS_U32 u32SrcId = (eFltType & E_TSP_FLT_SRCID_MASK) >> E_TSP_FLT_SRCID_SHIFT;

        if(u32SrcId >= TSP_MERGESTREAM_NUM)
        {
            TSP_DBG(E_TSP_DBG_MUST,"[%s][%d] Stream index out of range!!\n",__FUNCTION__,__LINE__);
        }

    #if HW_PCRFLT_ENABLE
        if(E_TSP_FLT_USER_PCR == (eFltType & E_TSP_FLT_USER_MASK))
        {
            if ((i >= HAL_TSP_PCRFLT_GET_ID(0)) && (i <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
            {
                MS_U32 u32PcrId = i - _tspFltIdMap.u32PcrFltId_Start;;

                HAL_TSP_PcrFlt_SetSrcId(u32PcrId, u32SrcId);
            }
        }
        else
    #endif // end HW_PCRFLT_ENABLE
        {
            HAL_TSP_PidFlt_SetSrcID(i,u32SrcId);
        }
    }
#endif // end MERGE_STR_SUPPORT


    // Select FLT out
    // MDrv_TSP_FLT_Enable

    // Set FLT_USR Type
    type = E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[i];
    if (type)
    {
        switch (type)
        {
        case E_TSP_FLT_USER_SEC:
        case E_TSP_FLT_USER_PES:
        case E_TSP_FLT_USER_PKT:
        case E_TSP_FLT_USER_TTX:
            HAL_TSP_SecFlt_SetType(pSecFlt, type >> E_TSP_FLT_USER_SHFT);
            break;
        case E_TSP_FLT_USER_PCR:
        //if TSP support PCR hardware , we don't use section filter to capture PCR from TS packet
        //otherwise , set operation type of section filter to PCR
        #if !HW_PCRFLT_ENABLE
            HAL_TSP_SecFlt_SetType(pSecFlt, type >> E_TSP_FLT_USER_SHFT);
        #endif
            break;
        // Non-SECFLT allocated : PIDFLT & special FLT only
        case E_TSP_FLT_USER_EMM:
        case E_TSP_FLT_USER_ECM:
            // select no section type;
            HAL_TSP_SecFlt_SetType(pSecFlt, 0);
            break;
#if defined(__LEGACY__)
        case E_TSP_FLT_USER_OAD:
#endif
        default:
            TSP_DBG(E_TSP_DBG_WARNING, "[%04d] Unsupport FlType = %x\n", __LINE__, type);
            HAL_TSP_SecFlt_SetType(pSecFlt, 0);
            break;
        }
    }

    *pu32FltId = i;
    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Free a PID filter of a TSP unit
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_Free(MS_U32 u32FltId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);
    //We do nothing when create dmx flt to record
    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
        return E_TSP_OK;

    MS_U32              flags;
// NO Blocking check in blocking TSP_FLT_Free
//    TSP_ENTRY();

    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        return E_TSP_OK ;
    }

    // The conditon is used to distinguish the u32FltId is section filter or not.
    // Here we free all filter type except section filter, including stream filters, PCR filters, etc.
    // Free non-section filter do not call _TSP_FLT_Free
    if(u32FltId >= _tspFltIdMap.u32SecFltId_End)
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId); // disable output
        _TSP_FLT_Init(u32FltId); // reset pid & status

        if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
        {
            MS_U32 u32STCEng;

            if(_TSP_PcrId_To_StcId(u32FltId, &u32STCEng) == E_TSP_OK)
            {
                _TSP_STC_Free(u32STCEng);
            }
        }

        _ptsp_res->_tspInfo.OwnerId[u32FltId]  = E_TSP_ID_NULL;
        _ptsp_res->_tspInfo.FltState[u32FltId] = E_TSP_FLT_STATE_FREE;
        _ptsp_res->_tspInfo.FltType[u32FltId] = E_TSP_FLT_USER_NULL;
        _ptsp_res->_tspInfo.s32TspEvtId[u32FltId] = -1;
        TSP_UNLOCK();
        return E_TSP_OK ;
    }

    // Free section filter need to call _TSP_FLT_Free
    if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        TSP_LOCK();
        _TSP_FLT_Disable(u32FltId); // disable output
        _TSP_FLT_Init(u32FltId); // reset pid & status
        _TSP_FLT_Free(u32FltId);
        TSP_UNLOCK();
        TSP_FLAG_GetFltDis(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetSecOvf(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);
        TSP_FLAG_GetSecRdy(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR);

    }
    else if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        if (_ptsp_res->_tspInfo.FltState[u32FltId] == E_TSP_FLT_STATE_ISR_FREE)
        {
            // it's freeing
        }
        else
        {
            TSP_LOCK();
            _ptsp_res->_tspInfo.FltState[u32FltId] = E_TSP_FLT_STATE_ISR_FREE;
            TSP_UNLOCK();

            // Add to FREE list
            TSP_FLAG_SetFree(u32FltId);
        }
        // Wakeup Task
        TSP_SetEvent(_ptsp_res->_tspInfo.s32TspEvtId[u32FltId], TSP_EVENT_FREEFLT);

        //Since MApi_DMX_Close() already has a mechanism to wait free done without continuously blocking the mutex.
        //Here we only return the result of free event is finished or not to MApi_DMX_Close().
        if (FALSE == TSP_FLAG_GetDone(u32FltId, &flags, TSP_OS_EVENT_OR_CLEAR))
        {
            return E_TSP_FAIL;
        }

        //To avoid the filter being allocated during ISR free process,
        //_TSP_FLT_Init and _TSP_FLT_Free are moved from ISR free process to here.
        //During the ISR free process, we use E_TSP_FLT_STATE_ISR_FREE instead of E_TSP_FLT_STATE_FREE to indicate a freeing filter.
        TSP_LOCK();
        _TSP_FLT_Init(u32FltId); // reset pid & status
        _TSP_FLT_Free(u32FltId);
        TSP_UNLOCK();
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST,"[DMX_ERR][%s][%d] Free flt error!\n", __FUNCTION__,__LINE__);
    }


// NO Blocking check in blocking TSP_FLT_Free
//    TSP_RETURN(E_TSP_OK);
    return E_TSP_OK;
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
TSP_Result MDrv_TSP_FLT_SelSEC(MS_U32 u32FltId, MS_U32 u32BufId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
    {
        _ptsp_res->u32RecFltBufId = u32BufId;
        return E_TSP_OK;
    }

    TSP_ENTRY();
    if ( (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId]) ||
         (E_TSP_FLT_USER_PCR   == (_ptsp_res->_tspInfo.FltType[u32FltId] & E_TSP_FLT_USER_MASK)) )
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_SecFlt_SelSecBuf(&(_REGSec->Flt[u32FltId]), u32BufId);

    _ptsp_res->_tspInfo.Flt2Buf[u32FltId] = u32BufId;

    _ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] += 1;

    _ptsp_res->_tspInfo.Buf2Flt[u32BufId] = u32FltId;
    //ULOGD("TSP", "[%s] MulFlt2Buf %ld = %d \n",__FUNCTION__ ,u32BufId ,_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId]);

    TSP_RETURN(E_TSP_OK);
}


///RESERVED
TSP_Result MDrv_TSP_FLT_GetSEC(MS_U32 u32FltId, MS_U32 *pu32BufId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
    {
        *pu32BufId = _ptsp_res->u32RecFltBufId;
        return E_TSP_OK;
    }

    TSP_ENTRY();
    if ( (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId]) ||
         (E_TSP_FLT_USER_PCR   == (_ptsp_res->_tspInfo.FltType[u32FltId] & E_TSP_FLT_USER_MASK)) )
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    //u32BufId = _ptsp_res->_tspInfo.Flt2Buf[u32Fltid];
    *pu32BufId = (MS_U32)HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[u32FltId]));

    TSP_RETURN(E_TSP_OK);
}
///RESERVED

TSP_Result MDrv_TSP_FLT_SetPCRSrc(MS_U32 u32FltId,MS_U32 u32PCRSrc)
{
    // @TODO need to take care PCR SRC from MM here
    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & u32PCRSrc;
    TSP_PIDFLT_SRC ePidFltSrc = _TSP_DrvHal_FltSrcMapping(u32FltSrc);
    TSP_PCR_SRC ePcrSrc = HAL_TSP_FltSrc2PCRSrc_Mapping(ePidFltSrc);

    TSP_ENTRY();

    if ((u32FltId >= TSP_PCRFLT_GET_ID(0)) && (u32FltId <= TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
    {
        HAL_TSP_PcrFlt_SetSrc((u32FltId - _tspFltIdMap.u32PcrFltId_Start), ePcrSrc);
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}

TSP_Result MDrv_TSP_FLT_SetFltRushPass(MS_U32 u32FltId, MS_U8 u8Enable)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
        return E_TSP_OK;

    TSP_ENTRY();
    if ( (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId]) ||
         (E_TSP_FLT_USER_PCR   == (_ptsp_res->_tspInfo.FltType[u32FltId] & E_TSP_FLT_USER_MASK)) )
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_PidFlt_SetFltRushPass(u32FltId, u8Enable);

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter to be set
/// @param  u32PID                  \b IN: PID value
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetPID(MS_U32 u32FltId, MS_U32 u32PID)
{
   TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX ,&u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
    {
        _ptsp_res->u32RecFltPID = u32PID;
        return E_TSP_OK;
    }

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    _ptsp_res->_tspInfo.Pid[u32FltId] = u32PID ;

    if(u32FltId < _tspFltIdMap.u32PidFltId_End)
    {
        HAL_TSP_PidFlt_SetPid(u32FltId, u32PID);
    }

    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK) == E_TSP_FLT_USER_PCR )
    {
        TSP_DBG(E_TSP_DBG_MUST, "Set PCR FLT=%d PID = %8X \n\n",(unsigned int)u32FltId,(unsigned int)u32PID);

         if ((u32FltId >= TSP_PCRFLT_GET_ID(0)) && (u32FltId <= TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
        {
            HAL_TSP_PcrFlt_SetPid((u32FltId - _tspFltIdMap.u32PcrFltId_Start), u32PID);
            //HAL_TSP_PcrFlt_Enable((u32FltId - TSP_PCRFLT_START_ID), TRUE);
        }
        else
        {
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Incorrect PCR filter ID !!\n",__FUNCTION__,__LINE__);
        }
    }

    TSP_RETURN(E_TSP_OK);
}


///RESERVED
TSP_Result MDrv_TSP_FLT_GetPID(MS_U32 u32FltId, MS_U32 *pu32PID)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

   // HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
    {
        *pu32PID = _ptsp_res->u32RecFltPID;
        return E_TSP_OK;
    }

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }
    else
    {
    #if HW_PCRFLT_ENABLE
        if((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK) == E_TSP_FLT_USER_PCR)
        {
            if ((u32FltId >= HAL_TSP_PCRFLT_GET_ID(0)) && (u32FltId <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
            {
                *pu32PID = HAL_TSP_PcrFlt_GetPid(u32FltId - _tspFltIdMap.u32PcrFltId_Start);
            }
        }
        else
    #endif
        {
            *pu32PID = HAL_TSP_PidFlt_GetPid(&(_REGPid0->Flt[u32FltId]));
        }
    }

    if (_ptsp_res->_tspInfo.Pid[u32FltId] != *pu32PID)
        TSP_OS_Print("[%s] Sw/Hw conflict => Pid Value mismatch \n",__FUNCTION__);

    TSP_RETURN(E_TSP_OK);
}

///RESERVED


//-------------------------------------------------------------------------------------------------
// Set section filtering mode
// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32SecFltId             \b IN: section filter of TSP to be free
// @param  eSecFltMode             \b IN: continue/one-shot mode of section filter
// @return TSP_Result
// @attention
// One-shot filter has the disadvantage of interrupt lost becuase it stops filter, a timeout
// to check filter status is better for usage.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetMode(MS_U32 u32FltId, TSP_FltMode eFltMode)
{
    MS_U32         uMode = 0;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    if((eFltMode & E_TSP_FLT_MODE_CRCCHK) && (eFltMode & E_TSP_FLT_MODE_AUTO_CRCCHK))
    {
        TSP_DBG(E_TSP_DBG_WARNING, "[%04d] E_TSP_FLT_MODE_CRCCHK and E_TSP_FLT_MODE_AUTO_CRCCHK can't be set together. \n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    if (eFltMode & E_TSP_FLT_MODE_ONESHOT)
    {
        uMode |= TSP_SECFLT_MODE_ONESHOT;
    }

    if (eFltMode & E_TSP_FLT_MODE_CRCCHK)
    {
        uMode |= TSP_SECFLT_MODE_CRCCHK;
    }

    if (eFltMode & E_TSP_FLT_MODE_AUTO_CRCCHK)
        HAL_TSP_SecFlt_SetAutoCRCChk(&(_REGSec->Flt[u32FltId]), TRUE);
    else
        HAL_TSP_SecFlt_SetAutoCRCChk(&(_REGSec->Flt[u32FltId]), FALSE);

    _ptsp_res->_tspInfo.FltMode[u32FltId] = eFltMode;

    HAL_TSP_SecFlt_SetMode(&(_REGSec->Flt[u32FltId]), uMode);

    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FLT_SetMatch(MS_U32 u32FltId, MS_U8 *pu8Match, MS_U8 *pu8Mask, MS_U8 *pu8NMask)
{
    static MS_U8        mask[TSP_FILTER_DEPTH];
    int                 i;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    TSP_CHECK(_tspFltIdMap.u32SecFltId_End  > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    TSP_CHECK(pu8Match, "[%04d] Invalid Match Pointer\n", __LINE__);
    TSP_CHECK(pu8Mask,  "[%04d] Invalid Mask  Pointer\n", __LINE__);
    TSP_CHECK(pu8NMask, "[%04d] Invalid NMask Pointer\n", __LINE__);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    for (i = 0 ; i < TSP_FILTER_DEPTH ; i++)
    {   // invert the mask for HW specified
        //*((MS_U32*)(mask+(i<<2))) = ~(*((MS_U32*)(pu8Mask+(i<<2))));
         mask[i] = ~pu8Mask[i];
    }

    HAL_TSP_SecFlt_SetMask (&(_REGSec->Flt[u32FltId]), mask);
    HAL_TSP_SecFlt_SetMatch(&(_REGSec->Flt[u32FltId]), pu8Match);
    HAL_TSP_SecFlt_SetNMask(&(_REGSec->Flt[u32FltId]), pu8NMask);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Subscribe event notification callback function for specified section filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  eEvents                 \b IN: events need to be subscribed\n
///                                        @ref E_TSP_EVENT_DATA_READY\n
///                                        @ref E_TSP_EVENT_BUF_OVERFLOW
/// @param  pfCallback              \b IN: callback function (NULL to disable)
/// @return TSP_Result
/// @note
/// This function register a callback function for a section filter to TSP.
/// TSP calls callback function each time when data is ready in section buffer.\n
/// Data ready of section filter:\n
/// @ref E_TSP_FLT_TYPE_SECTION : a section ready\n
/// @ref E_TSP_FLT_TYPE_PES : PES packet ready or received data over than request size.
/// @sa MDrv_TSP_SecFlt_SetReqCount
/// @attention
/// Callback function resides in OS TSP interrupt context, it recommends
/// that callback function should not take too much time to block the system.
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_SetCallback(MS_U32 u32FltId, TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    TSP_ENTRY();

    if ((eEvents & (E_TSP_EVENT_DATA_READY | E_TSP_EVENT_BUF_OVERFLOW)) == 0)
    {
        TSP_RETURN(E_TSP_FAIL_PARAMETER);
    }

    _ptsp_res->_tspInfo.FltEvent[u32FltId]    = eEvents;
    _ptsp_res->_tspInfo.FltCallback[u32FltId] = pfCallback;

    if (E_TSP_EVENT_CB_AUTO == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
    {
        TSP_FLAG_SetModeFlags(u32FltId);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result _MDrv_TSP_FLT_Enable(MS_U32 u32FltId, MS_BOOL bEnable)
{
    REG_SecFlt *        pSecFlt = NULL;
    REG_SecBuf *        pSecBuf = NULL;
    MS_U32              type;
    TSP_PIDFLT_SRC      ePidFltSrc = E_TSP_PIDFLT_INVALID;
    MS_U32              u32Src, u32PvrLutDest = 0;
    MS_U16              u16BufId = 0;
    MS_U32              dummy;

    // Enable PVR I Frame LUT.
    if( E_TSP_DST_IFRAME_LUT & _ptsp_res->_tspInfo.FltType[u32FltId])
        u32PvrLutDest = TSP_PIDFLT_OUT_LUT;

    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
        return E_TSP_OK;

    if(u32FltId < _tspFltIdMap.u32SecFltId_End)
    {
        TSP_FLAG_GetFltDis(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);

        if (bEnable) //clear all filter event.
        {
            // MsOS_DelayTask(1); //[Note] For VQ flush,
            TSP_FLAG_GetSecRdy(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
            TSP_FLAG_GetSecOvf(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
            TSP_FLAG_GetCrcErr(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
            TSP_FLAG_GetFltDis(u32FltId, &dummy, TSP_OS_EVENT_OR_CLEAR);
            TSP_FLAG_ClrOvfRst(u32FltId);
        }
    }



    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.FltState[u32FltId])
    {
        TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Filter\n", __LINE__);
        return E_TSP_FAIL_INVALID;
    }

    // TSP_FLT_Disable
    if (!bEnable)
    {
        if (_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE) // if enable
        {
            _TSP_FLT_Disable(u32FltId);
        }
        // TSP_DBG(E_TSP_DBG_FAIL, "[TSP_WARNNING][%04d] Enable an active PidFlt %d\n", __LINE__, u32FltId);

        return E_TSP_OK;
    }

    // TSP_FLT_Enable
    //connect avfifo to pid flt~
    u32Src = E_TSP_FLT_SRC_MASK & _ptsp_res->_tspInfo.FltType[u32FltId];
    ePidFltSrc = _TSP_DrvHal_FltSrcMapping(u32Src);
    u32Src = HAL_TSP_FltSrc2PktDmx_Mapping(ePidFltSrc);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32Src);

    type = E_TSP_FLT_FIFO_MASK & _ptsp_res->_tspInfo.FltType[u32FltId];

    if (type)
    {
        switch (type)
        {
            case E_TSP_FLT_FIFO_VIDEO ... E_TSP_FLT_FIFO_VIDEO8:
                HAL_TSP_FIFO_SetSrc((TSP_DST_SEQ)(E_TSP_DST_FIFO_VIDEO + (type - E_TSP_FLT_FIFO_VIDEO)), u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PidFltDstMapping(E_TSP_PIDFLT_DST_VIDEO, (type - E_TSP_FLT_FIFO_VIDEO)));
                break;
            case E_TSP_FLT_FIFO_AUDIO ... E_TSP_FLT_FIFO_AUDIO6:
                HAL_TSP_FIFO_SetSrc((TSP_DST_SEQ)(E_TSP_DST_FIFO_AUDIO + (type - E_TSP_FLT_FIFO_AUDIO)), u32Src);
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PidFltDstMapping(E_TSP_PIDFLT_DST_AUDIO, (type - E_TSP_FLT_FIFO_AUDIO)));
                break;
            case E_TSP_FLT_FIFO_REC:
                // @F_TODO for old style. we might have to check the filter source and it's destination
                break;
            case E_TSP_FLT_FIFO_PVR ... E_TSP_FLT_FIFO_PVR9:
                HAL_TSP_PidFlt_SetFltOut(u32FltId, HAL_TSP_PidFltDstMapping(E_TSP_PIDFLT_DST_PVR, (type - E_TSP_FLT_FIFO_PVR)) | u32PvrLutDest);
                break;

            default:
    //            TSP_DBG(E_TSP_DBG_MUST, "[%04d] Wrong Destination type : %d!\n", __LINE__, type);
    //            TSP_RETURN(E_TSP_FAIL);
                break;
        }
    }
    else
    {
        type = E_TSP_FLT_USER_MASK & _ptsp_res->_tspInfo.FltType[u32FltId];
        if (type)
        {
#if 0// defined(__LEGACY__)  we don't need this in kaiser we have bit to set for pvr eng
            if (E_TSP_FLT_FIFO_PVR == type) // @FIXME WHY??????????
            {
                TSP_DBG(0, "[%04d] Bad PidFltId %ld\n", __LINE__, u32FltId);
            }
            else
#endif
                if(u32FltId < _tspFltIdMap.u32SecFltId_End)
                {
                    pSecFlt = &(_REGSec->Flt[u32FltId]);
                    u16BufId = HAL_TSP_SecFlt_GetSecBuf(pSecFlt);
                    pSecBuf = &(_REGBuf->Buf[u16BufId]);
                }

                //TSP_DBG(0, "[%04d] SecBufId = %d\n", __LINE__, u16BufId);

                switch (type)
                {
                    case E_TSP_FLT_USER_SEC:
                    case E_TSP_FLT_USER_PES:
                    case E_TSP_FLT_USER_PKT:
                    case E_TSP_FLT_USER_TTX:
                        if (_ptsp_res->_tspInfo.Flt2Buf[u32FltId] == TSP_FLT2BUF_NULL)
                        {
                            return E_TSP_FAIL_FUNCTION;
                        }
                        if(_ptsp_res->_tspInfo.MulFlt2Buf[u16BufId] == 1) // single filter and single buffer
                        {
                            HAL_TSP_SecBuf_Reset(pSecBuf);
                        }
                        if (E_TSP_EVENT_CB_POLL == (_ptsp_res->_tspInfo.FltEvent[u32FltId] & E_TSP_EVENT_CB_MASK))
                        {
    #ifdef MSOS_TYPE_LINUX_KERNEL
                            HAL_TSP_HCMD_SecRdyInt_Disable(u32FltId,FALSE);
    #else
                            HAL_TSP_HCMD_SecRdyInt_Disable(u32FltId,TRUE);
    #endif
                        }
                        else // E_TSP_EVENT_CB_AUTO
                        {
                            HAL_TSP_HCMD_SecRdyInt_Disable(u32FltId,FALSE);
                        }

                        HAL_TSP_SecFlt_ResetState(pSecFlt);
                        HAL_TSP_SecFlt_ResetRmnCnt(pSecFlt);
                        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_SECFLT);
                        break;
                    case E_TSP_FLT_USER_EMM: // @NOTE LEGACY
                    case E_TSP_FLT_USER_ECM:
                        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE);
                        break;

                    case E_TSP_FLT_USER_PCR:
                    #if HW_PCRFLT_ENABLE

                        if ((u32FltId >= HAL_TSP_PCRFLT_GET_ID(0)) && (u32FltId <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
                        {
                            MS_U32 u32PcrId = u32FltId - _tspFltIdMap.u32PcrFltId_Start;
                            MS_U32 u32PcrUpdateEndEn = HAL_TSP_PcrFlt_GetIntMask(u32PcrId);

                            _TSP_SetHwPcrNotifyId(u32PcrId);
                            _ptsp_res->_u8_ResetPcr[u32PcrId] = 3;
                            _ptsp_res->_u32_PrevStcBase[u32PcrId] = 0;
                            _ptsp_res->_u32_PrevPllControl[u32PcrId] = 0;
                            _ptsp_res->_u32_StcOffset[u32PcrId].u32StcOffset = 0;
                            _ptsp_res->_u32_StcOffset[u32PcrId].bAdd = TRUE;
                            _ptsp_res->_u32_StcUpdateCtrl[u32PcrId].bEnable = TRUE;
                            _ptsp_res->_u32_StcUpdateCtrl[u32PcrId].bUpdateOnce = FALSE;
                            HAL_TSP_PcrFlt_Enable(u32PcrId, TRUE);
                            HAL_TSP_INT_Enable(u32PcrUpdateEndEn);
                        }
                        else
                        {
                            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Incorrect PCR filter ID !!\n", __FUNCTION__, __LINE__);
                            return E_TSP_FAIL;
                        }

                    #else // @F_TODO i think we do not have to support SW PCR update anymore
                        HAL_TSP_SecFlt_ResetState(pSecFlt);
                        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_SECFLT| TSP_PIDFLT_OUT_SECAF); //@NOTE what is SECAF for???? SECAF is for those who has adaption field packet usecase: SWPCR
                    #endif
                        break;
                    default:
                        TSP_DBG(E_TSP_DBG_MUST, "[%04d] Unsupport FlType = %x\n", __LINE__, _ptsp_res->_tspInfo.FltType[u32FltId]);
                        return E_TSP_FAIL;
                        break;
                }
        }
    }

    _ptsp_res->_tspInfo.FltState[u32FltId] |= E_TSP_FLT_STATE_ENABLE;
    return E_TSP_OK;
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
TSP_Result MDrv_TSP_FLT_Enable(MS_U32 u32FltId, MS_BOOL bEnable)
{
    TSP_Result eResult;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

    TSP_ENTRY();

    eResult = _MDrv_TSP_FLT_Enable(u32FltId, bEnable);

    TSP_RETURN(eResult);
}


//[RESERVED] obsoleted
TSP_Result MDrv_TSP_FLT_SetOwner(MS_U32 u32EngId, MS_U32 u32FltStart, MS_U32 u32FltEnd, MS_BOOL bOwner)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    return E_TSP_FAIL_NOT_SUPPORTED;
}


//[RESERVED] obsoleted
TSP_Result MDrv_TSP_FLT_ChkOwner(MS_U32 u32EngId, MS_U32 u32FltId)
{
    TSP_ENTRY();

    if (_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ALLOC)
    {
        if (_ptsp_res->_tspInfo.OwnerId[u32FltId] == u32EngId)
        {
            TSP_RETURN(E_TSP_OK);
        }
        else
        {
            TSP_RETURN(E_TSP_FAIL);
        }
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }
}


//-------------------------------------------------------------------------------------------------
/// Get current PID filter status
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32PidFltId             \b IN: index of PID filter
/// @param  peState                 \b OUT: current ORed state flag of PID filter\n
///                                         E_TSP_FLT_STATE_ALLOC\n
///                                         E_TSP_FLT_STATE_ENABLE\n
///                                         E_TSP_FLT_STATE_SCRAMBLED (last TS scrambling control status)
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FLT_GetState(MS_U32 u32FltId, TSP_FltState *pState)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll  > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);
    //We do nothing when create dmx flt to record
    //HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
    {
        *pState = E_TSP_FLT_STATE_NA;
        return E_TSP_OK;
    }

    MS_U32              u32BufId;

    // ALLOC / FREE / ENABLE
    *pState =  _ptsp_res->_tspInfo.FltState[u32FltId];

#if 0  // HW remove scramble status register
    // Scramble bit status
    if (HAL_TSP_GetSCMB((MS_U16)u32FltId))
    {
        *pState |= E_TSP_FLT_STATE_SCRAMBLED;
    }
#endif

    // PidFlt Status
    switch (_ptsp_res->_tspInfo.FltType[u32FltId])
    {
        case E_TSP_FLT_FIFO_VIDEO ... E_TSP_FLT_FIFO_VIDEO8:
        case E_TSP_FLT_FIFO_AUDIO ... E_TSP_FLT_FIFO_AUDIO6:
            break;
        case E_TSP_FLT_USER_SEC:
        case E_TSP_FLT_USER_PES:
        case E_TSP_FLT_USER_PKT:
        case E_TSP_FLT_USER_TTX:
            u32BufId = HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[u32FltId]));
            if (HAL_TSP_SecFlt_GetState(&(_REGSec->Flt[u32BufId])) & TSP_SECFLT_STATE_OVERFLOW)
            {
                *pState |= E_TSP_FLT_STATE_OVERFLOW;
            }
            if (HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[u32BufId])) != HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[u32BufId])))
            {
                *pState |= E_TSP_FLT_STATE_STREAM_AVAIL;
            }
            break;

        case E_TSP_FLT_USER_PCR:
        default:
            break;
    }

    return E_TSP_OK;
}


//-------------------------------------------------------------------------------------------------
/// Allocate a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32SecFltId            \b OUT: pointer of section filter id return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_Alloc(MS_U32 u32TSPId, MS_U32 *pu32BufId)
{
    REG_SecBuf *        pSecBuf;
    int                 i;

    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    TSP_ENTRY();

    *pu32BufId = 0xFFFFFFFF;

    for (i = _tspFltIdMap.u32SecBufId_Start; i < _tspFltIdMap.u32SecBufId_End; i++)
    {
        if (_ptsp_res->_tspInfo.BufState[i] == E_TSP_FLT_STATE_FREE)
        {
            pSecBuf = &(_REGBuf->Buf[i]);
            if (HAL_TSP_SecBuf_TryAlloc(pSecBuf, (MS_U16)u32TSPId))
            {
                break;
            }
        }
    }
    if (i >= _tspFltIdMap.u32SecBufId_End)
    {
        TSP_DBG(E_TSP_DBG_ERROR, "[%04d] No free buffer found 0x%02x\n", __LINE__, (MS_U32)i);
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_DBG(E_TSP_DBG_TRACK, "[%04d] SEC_Alloc BUF %d\n", __LINE__, i);

    _ptsp_res->_tspInfo.BufState[i] = E_TSP_FLT_STATE_ALLOC;
    _TSP_SEC_Init(i);

    *pu32BufId = i;
    _TSP_SetSecNotifyId(*pu32BufId); //for isr processing checking

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Free a section filter of a PID filter
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: section filter of TSP to be free
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result  MDrv_TSP_SEC_Free(MS_U32 u32BufId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);

    TSP_ENTRY();
    if (_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] == 1)
    {
        if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
        {
            TSP_DBG(E_TSP_DBG_FAIL, "[%04d] Invalid Buffer\n", __LINE__);
            TSP_DBG(E_TSP_DBG_INFO, "[%04d] Invalid Buffer %x, %x\n", __LINE__, u32BufId, _ptsp_res->_tspInfo.BufState[u32BufId]);
            TSP_RETURN(E_TSP_FAIL_INVALID);
        }

        _TSP_SEC_Free(u32BufId);
    }
    else
    {
        _ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] -= 1;
    }

    //ULOGD("TSP", "[%s] MulFlt2Buf %ld = %d ", __FUNCTION__,u32BufId,_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId]);

    TSP_RETURN(E_TSP_OK);

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
TSP_Result MDrv_TSP_SEC_SetBuffer(MS_U32 u32BufId, MS_PHYADDR u32Start, MS_U32 u32Size)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    TSP_DBG(E_TSP_DBG_TRACK, "SEC_SetBuffer addr 0x%08x, size  0x%08x\n", u32Start, u32Size);

    if (_ptsp_res->_tspInfo.MulFlt2Buf[u32BufId] == 1)
    {
        HAL_TSP_SecBuf_SetBuf(&(_REGBuf->Buf[u32BufId]), u32Start, u32Size);
    }

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Reset the section buffer read/write pointer to start address and resolve overflow condition
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer to be reset
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_Reset(MS_U32 u32BufId)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_SecBuf_Reset(&(_REGBuf->Buf[u32BufId]));

    HAL_TSP_SecFlt_ResetRmnCnt(&(_REGSec->Flt[(_ptsp_res->_tspInfo.Buf2Flt[u32BufId])]));

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
///[RESERVED]
/// Get buffer start address of setction buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32BufStart            \b OUT:  pointer of buffer start address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_GetStart(MS_U32 u32BufId, MS_PHY *pu32Start)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_CHECK(pu32Start, "[%04d] NULL pointer\n", __LINE__);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Start = HAL_TSP_SecBuf_GetStart(&(_REGBuf->Buf[u32BufId]));

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
///[RESERVED]
/// Get buffer size of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32BufSize             \b OUT: pointer of buffer size return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_GetSize(MS_U32 u32BufId, MS_U32 *pu32Size)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_CHECK(pu32Size, "[%04d] NULL pointer\n", __LINE__);

    *pu32Size = 0;

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Size=  HAL_TSP_SecBuf_GetEnd(&(_REGBuf->Buf[u32BufId])) - HAL_TSP_SecBuf_GetStart(&(_REGBuf->Buf[u32BufId]));
    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get current read address of section buffer
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  u32SecFltId             \b IN: index of section buffer
/// @param  pu32ReadAddr            \b OUT: pointer of address return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SEC_GetRead(MS_U32 u32BufId, MS_PHY *pu32Read)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_CHECK(pu32Read, "[%04d] NULL pointer\n", __LINE__);

    *pu32Read = 0;

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Read= (MS_PHY)HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[u32BufId]));
    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_SEC_GetWrite(MS_U32 u32BufId, MS_PHY *pu32Write)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_CHECK(pu32Write, "[%04d] NULL pointer\n", __LINE__);

    *pu32Write = 0;

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    *pu32Write= (MS_PHY)HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[u32BufId]));
    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_SEC_SetRead(MS_U32 u32BufId, MS_PHY u32Read)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);

    TSP_ENTRY();
    if (E_TSP_FLT_STATE_FREE == _ptsp_res->_tspInfo.BufState[u32BufId])
    {
        TSP_RETURN(E_TSP_FAIL_INVALID);
    }

    HAL_TSP_SecBuf_SetRead(&(_REGBuf->Buf[u32BufId]), (MS_U32)u32Read);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SECFLT_BurstLen(MS_U32 u32BurstMode)
{
    TSP_ENTRY();

    HAL_TSP_SecFlt_BurstLen(u32BurstMode);

    TSP_RETURN(E_TSP_OK);
}

#ifdef MSOS_TYPE_LINUX_KERNEL
TSP_Result MDrv_TSP_SEC_ClrEvent(MS_U32 u32BufId)
{
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_ENTRY();
    _u32SecEvent[u32BufId] = 0;
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SEC_GetEvent(MS_U32 u32BufId, MS_U32 *pu32Event)
{
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_ENTRY();
    *pu32Event = _u32SecEvent[u32BufId];
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SEC_SetEvent(MS_U32 u32BufId, MS_U32 u32Event)
{
    TSP_CHECK(_tspFltIdMap.u32SecFltId_End > u32BufId, "[%04d] Invalid u32BufId %d\n", __LINE__, (unsigned int)u32BufId);
    TSP_ENTRY();
    _u32SecEvent[u32BufId] |= u32Event;
    TSP_RETURN(E_TSP_OK);
}
#endif // MSOS_TYPE_LINUX_KERNEL


//-------------------------------------------------------------------------------------------------
/// Set packet size to TSP file
/// @param  PacketMode                 \b IN: Mode of TSP file packet mode (192, 204, 188)
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_SetPktMode(TSP_PktMode mode)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    switch (mode)
    {
        case E_TSP_PKTMODE_188:
            HAL_TSP_Filein_PktSize(eFileEng,188);
            break;
        case E_TSP_PKTMODE_192:
            HAL_TSP_Filein_PktSize(eFileEng,192);
            break;
        case E_TSP_PKTMODE_204:
            HAL_TSP_Filein_PktSize(eFileEng,204);
            break;
        case E_TSP_PKTMODE_130:
            HAL_TSP_Filein_PktSize(eFileEng,130);
            break;
        case E_TSP_PKTMODE_134:
            HAL_TSP_Filein_PktSize(eFileEng,134);
            break;
        default:
            break;
    }

    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FILE_SetRate(MS_U32 u32Div2)
{
    TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // TsDma pause can be access by TSP CPU
    // TsDma pause it's hard to control because read/write in different register
    // When setting TsDma it should be disable interrupt to prevent ISR access
    // but it still can't prevent TSP_cpu access at the same time.
    //[SW PATCH] Add a special firmware command to lock TSP_cpu DMA pause/resume.
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    TSP_OS_IntDisable();
    HAL_TSP_Filein_ByteDelay(eFileEng,u32Div2,TRUE);
    TSP_OS_IntEnable();

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream start address
/// @param  u32StreamAddr           \b IN: pointer of transport stream in memory
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_CmdAddr(MS_U32 u32Addr)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    HAL_TSP_Filein_Addr(eFileEng, u32Addr);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Stream data size
/// @param  u32StreamSize           \b IN: size of transport stream data to be copied
/// @return TSP_Result
//  @note u32StreamSize must be larger than 16
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_CmdSize(MS_U32 u32Size)
{
    if(u32Size <= 16)
        return E_TSP_FAIL;

    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    HAL_TSP_Filein_Size(eFileEng, u32Size);
    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FILE_CmdSTC(MS_U32 u32EngId, MS_U32 u32STC_H, MS_U32 u32STC)
{
    TSP_ENTRY();

    //[HW TODO][HW LIMIT]
    // STC register mapping is different between MCU and CPU.
    //HAL_TSP_CMDQ_SetSTC(u32STC_H, u32STC);
    //HAL_TSP_SetSTC(u32STC_H, u32STC);
    TSP_RES_ALLOC_STC_CHECK(u32EngId);
    HAL_TSP_STC33_CmdQSet(u32STC_H, u32STC);

    TSP_RETURN(E_TSP_OK);
}



//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : Start stream input
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
/// @note Filein Command Size: 1
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_CmdStart(TSP_FileMode mode)
{
     TSP_ENTRY();

    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    MS_BOOL bBypass = TRUE;  // Enable PES file-in or not
    TSP_DST_SEQ eDst = E_TSP_DST_INVALID;

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    for(eDst = E_TSP_DST_FIFO_VIDEO; eDst < E_TSP_DST_INVALID; eDst++)
    {
        HAL_TSP_FIFO_Bypass(eDst, FALSE); // for bypass clear
    }

    // @FIXME: Good to assume enum and register definition identical?
    switch (mode)
    {
        case E_TSP_FILE_2_TSP:
            bBypass = FALSE;
            break;
        case E_TSP_FILE_2_VIDEO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO);
            break;
        case E_TSP_FILE_2_VIDEO3D:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO3D);
            break;
        case E_TSP_FILE_2_VIDEO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO3);
            break;
        case E_TSP_FILE_2_VIDEO4:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO4, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO4);
            break;
        case E_TSP_FILE_2_VIDEO5:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO5, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO5);
            break;
        case E_TSP_FILE_2_VIDEO6:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO6, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO6);
            break;
        case E_TSP_FILE_2_VIDEO7:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO7, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO7);
            break;
        case E_TSP_FILE_2_VIDEO8:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO8, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO8);
            break;
        case E_TSP_FILE_2_AUDIO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO);
            break;
        case E_TSP_FILE_2_AUDIO2:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO2, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO2);
            break;
        case E_TSP_FILE_2_AUDIO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO3, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO3);
            break;
        case E_TSP_FILE_2_AUDIO4:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO4, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO4);
            break;
        case E_TSP_FILE_2_AUDIO5:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO5, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO5);
            break;
        case E_TSP_FILE_2_AUDIO6:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO6, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO6);
            break;
        default:
            break;
    }

    HAL_TSP_Filein_Bypass(eFileEng, bBypass);
    HAL_TSP_Filein_Start(eFileEng);

    TSP_RETURN(E_TSP_OK);
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
TSP_Result MDrv_TSP_FILE_GetCmdSlot(MS_U32 *pu32EmptySlot)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    *pu32EmptySlot = HAL_TSP_Filein_CmdQSlot(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset command queue
/// @return TSP_Result
/// @note
/// SW patch--wait command queue empty
/// wait HW ECO
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_Reset(void)
{
    MS_U32 u32Counter;

    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_CmdQRst(eFileEng, TRUE);
    HAL_TSP_Filein_CmdQRst(eFileEng, FALSE);
    HAL_TSP_Filein_Abort(eFileEng, TRUE);
    HAL_TSP_Filein_Abort(eFileEng, FALSE);

    //reset the last data that hw is excuting --> HW new design
    HAL_TSP_Filein_WbFsmRst(eFileEng, TRUE);

    for(u32Counter = 0; u32Counter < 100; u32Counter++)
    {
        if(!HAL_TSP_Filein_Status(eFileEng))
        {
            break;
        }
        MsOS_DelayTask(1);
    }

    HAL_TSP_Filein_WbFsmRst(eFileEng, FALSE);

    if(u32Counter == 100)
    {
        TSP_DBG(E_TSP_DBG_ERROR, "[%s][%d] Wait file in done timeout\n", __FUNCTION__, __LINE__);
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : pause stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_Pause(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    HAL_TSP_File_Pause(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Memory to TSP stream control : resume stream input
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_Resume(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    HAL_TSP_File_Resume(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Stop(void)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    HAL_TSP_TSIF_FileEn(eFileEng, FALSE);                    // data port disable @FIXME in k2 we switch data port only but we close every file setting here will there be side effect?

    // @TODO Does it need to call HAL_TSP_FIFO_Bypass() function?
    //HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, FALSE); // for clear all bypass setting

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current file in state
/// @return TSP_M2tStatus
/// @sa MDrv_TSP_GetM2tSlot
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_GetState(TSP_FileState *pState)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    *pState = HAL_TSP_Filein_GetState(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable timestamp block scheme
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_File_192BlockScheme_En(MS_BOOL bEnable)
{
    TSP_ENTRY();
//  HAL_TSP_FileIn_192BlockScheme_En(bEnable);
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);
    HAL_TSP_Filein_BlockTimeStamp(eFileEng, bEnable); //@F_TODO fix engine problen and make sure they are the same
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Setting PS path
/// @param  eM2tMode                \b IN: input source control of filein
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_FILE_PS_Path_Enable(TSP_FileMode mode)
{
    TSP_ENTRY();

    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO,   FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3,  FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO4,  FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO,   FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO2,  FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO3,  FALSE); // for bypass clear
    HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO4,  FALSE); // for bypass clear

    switch (mode)
    {
        case E_TSP_FILE_2_TSP:
            break;
        case E_TSP_FILE_2_VIDEO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, TRUE);
            break;
        case E_TSP_FILE_2_VIDEO3D:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, TRUE);
            break;
        case E_TSP_FILE_2_VIDEO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3, TRUE);
            break;
        case E_TSP_FILE_2_VIDEO4:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO4, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO2:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO2, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO3, TRUE);
            break;
        case E_TSP_FILE_2_AUDIO4:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO4, TRUE);
            break;
        default:
            break;
    }
    TSP_RETURN(E_TSP_OK);
}

// file driver with engine
TSP_Result MDrv_TSP_FILE_Eng_SetPktMode(TSP_FILE_ENG Eng, TSP_PktMode mode)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    MS_U32 u32PktSize = 0;
    switch (mode)
    {
        case E_TSP_PKTMODE_188:
            u32PktSize = 188;
            break;
        case E_TSP_PKTMODE_192:
            u32PktSize = 192;
            break;
        case E_TSP_PKTMODE_204:
            u32PktSize = 204;
            break;
        case E_TSP_PKTMODE_130:
            u32PktSize = 130;
            break;
        case E_TSP_PKTMODE_134:
            u32PktSize = 134;
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "Invalid File-in Packet Size!\n");
            TSP_RETURN(E_TSP_FAIL);
            break;
    }

    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_PktSize(eFileEng, u32PktSize);

    TSP_RETURN(E_TSP_OK);
}

#ifdef MERGE_STR_SUPPORT
static TSP_HAL_PKT_MODE _TSP_DrvHal_PktConverter_PktModeMapping(TSP_PktMode ePktMode)
{
    TSP_HAL_PKT_MODE eOutPktMode = E_TSP_HAL_PKT_MODE_NORMAL;

    switch(ePktMode)
    {
        case E_TSP_PKTMODE_CIPLUS:
            eOutPktMode = E_TSP_HAL_PKT_MODE_CI;
            break;
        case E_TSP_PKTMODE_ATS:
            eOutPktMode = E_TSP_HAL_PKT_MODE_ATS;
            break;
        case E_TSP_PKTMODE_OPENCABLE:
            eOutPktMode = E_TSP_HAL_PKT_MODE_OPEN_CABLE;
            break;
        case E_TSP_PKTMODE_MXL192:
            eOutPktMode = E_TSP_HAL_PKT_MODE_MXL_192;
            break;
        case E_TSP_PKTMODE_MXL196:
            eOutPktMode = E_TSP_HAL_PKT_MODE_MXL_196;
            break;
        case E_TSP_PKTMODE_MXL200:
            eOutPktMode = E_TSP_HAL_PKT_MODE_MXL_200;
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Invalid Packet Mode !!\n", __FUNCTION__, __LINE__);
            break;
    }

    return eOutPktMode;
}
#endif

TSP_Result MDrv_TSP_SetPacketMode(TSP_TSIF u32TSIf, TSP_PktMode ePktMode)
{
#ifdef MERGE_STR_SUPPORT
    TSP_ENTRY();

    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(u32TSIf);
    MS_U32 u32TSIF = 0;

    if((ePktMode >= E_TSP_PKTMODE_LIVEIN_MIN) && (ePktMode <= E_TSP_PKTMODE_LIVEIN_MAX)) // live-in
    {
        u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

        TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

        HAL_TSP_PktConverter_PktMode(u32TSIF, _TSP_DrvHal_PktConverter_PktModeMapping(ePktMode));
    }

    TSP_RETURN(E_TSP_OK);
#else
    return E_TSP_FAIL_NOT_SUPPORTED;
#endif
}

TSP_Result MDrv_TSP_FILE_Eng_SetRate(TSP_FILE_ENG Eng, MS_U32 u32Div2)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    //[HW TODO][HW LIMIT]
    // TsDma pause can be access by TSP CPU
    // TsDma pause it's hard to control because read/write in different register
    // When setting TsDma it should be disable interrupt to prevent ISR access
    // but it still can't prevent TSP_cpu access at the same time.
    //[SW PATCH] Add a special firmware command to lock TSP_cpu DMA pause/resume.
    TSP_OS_IntDisable();
    HAL_TSP_Filein_ByteDelay(eFileEng, u32Div2, TRUE); //@F_TODO when to set false???
    TSP_OS_IntEnable();

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_CmdAddr(TSP_FILE_ENG Eng, MS_U32 u32Addr)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_Addr((MS_U32)eFileEng, u32Addr);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_CmdSize(TSP_FILE_ENG Eng, MS_U32 u32Size)
{
    if(u32Size <= 16)
        return E_TSP_FAIL;

    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_Size(eFileEng, u32Size);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_CmdSTC(TSP_FILE_ENG Eng, MS_U32 u32EngId, MS_U32 u32STC_H, MS_U32 u32STC)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}

TSP_Result MDrv_TSP_FILE_Eng_CmdStart(TSP_FILE_ENG Eng, TSP_FileMode mode)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    MS_BOOL bBypass = TRUE;  // Enable PES file-in or not
    TSP_DST_SEQ eDst = E_TSP_DST_INVALID;

    for(eDst = E_TSP_DST_FIFO_VIDEO; eDst < E_TSP_DST_INVALID; eDst++)
    {
        HAL_TSP_FIFO_Bypass(eDst, FALSE); // for bypass clear
    }

    switch (mode)
    {
        case E_TSP_FILE_2_TSP:
            bBypass = FALSE;
            break;
        case E_TSP_FILE_2_VIDEO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO);
            break;
        case E_TSP_FILE_2_VIDEO3D:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO3D);
            break;
        case E_TSP_FILE_2_VIDEO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO3);
            break;
        case E_TSP_FILE_2_VIDEO4:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO4, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO4);
            break;
        case E_TSP_FILE_2_VIDEO5:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO5, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO5);
            break;
        case E_TSP_FILE_2_VIDEO6:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO6, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO6);
            break;
        case E_TSP_FILE_2_VIDEO7:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO7, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO7);
            break;
        case E_TSP_FILE_2_VIDEO8:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO8, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_VIDEO8);
            break;
        case E_TSP_FILE_2_AUDIO:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO);
            break;
        case E_TSP_FILE_2_AUDIO2:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO2, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO2);
            break;
        case E_TSP_FILE_2_AUDIO3:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO3, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO3);
            break;
        case E_TSP_FILE_2_AUDIO4:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO4, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO4);
            break;
        case E_TSP_FILE_2_AUDIO5:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO5, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO5);
            break;
        case E_TSP_FILE_2_AUDIO6:
            HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_AUDIO6, TRUE);
            HAL_TSP_FIFO_Bypass_Src(eFileEng, E_TSP_DST_FIFO_AUDIO6);
            break;
        default:
            break;
    }

    HAL_TSP_Filein_Bypass(eFileEng, bBypass);
    HAL_TSP_Filein_Start(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_Stop(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_TSIF_FileEn(eFileEng, FALSE);             // data port disable @FIXME in k2 we switch data port only but we close every file setting here will there be side effect?

    // @TODO not completely implement yet
    // Fix Path : tsif 0 for video and tsif 1 for video3D
    // need to flexible modification in the future
   /*
    if(eFileEng == E_FILEENG_TSIF0)
    {
        HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO, FALSE); // for clear all bypass setting
    }
    else if(eFileEng == E_FILEENG_TSIF2)
    {
        HAL_TSP_FIFO_Bypass(E_TSP_DST_FIFO_VIDEO3D, FALSE); // for clear all bypass setting
    }
    */


    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_GetCmdSlot(TSP_FILE_ENG Eng, MS_U32 *pu32EmptySlot)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    *pu32EmptySlot = HAL_TSP_Filein_CmdQSlot(eFileEng);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_Pause(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    if(HAL_TSP_File_Pause(eFileEng))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}

TSP_Result MDrv_TSP_FILE_Eng_Resume(TSP_FILE_ENG Eng)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;

    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    if(HAL_TSP_File_Resume(eFileEng))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }

}

TSP_Result MDrv_TSP_FILE_Eng_GetState(TSP_FILE_ENG Eng, TSP_FileState *pState)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    *pState = HAL_TSP_Filein_GetState(eFileEng);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_Reset(TSP_FILE_ENG Eng)
{
    MS_U32 u32Counter;

    TSP_ENTRY();
    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_CmdQRst(eFileEng, TRUE);
    HAL_TSP_Filein_CmdQRst(eFileEng, FALSE);
    HAL_TSP_Filein_Abort(eFileEng, TRUE);
    HAL_TSP_Filein_Abort(eFileEng, FALSE);

    //reset the last data that hw is excuting --> HW new design
    HAL_TSP_Filein_WbFsmRst(eFileEng, TRUE);

    for(u32Counter = 0; u32Counter < 100; u32Counter++)
    {
        if(!HAL_TSP_Filein_Status(eFileEng))
        {
            break;
        }
        MsOS_DelayTask(1);
    }

    HAL_TSP_Filein_WbFsmRst(eFileEng, FALSE);

    if(u32Counter == 100)
    {
        TSP_DBG(E_TSP_DBG_ERROR, "[%s][%d] Wait file in done timeout\n", __FUNCTION__, __LINE__);
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_192BlockScheme_En(TSP_FILE_ENG Eng, MS_BOOL bEnable)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_BlockTimeStamp(eFileEng, bEnable);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_PS_Path_Enable(TSP_FILE_ENG Eng, TSP_FileMode mode)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}

TSP_Result MDrv_TSP_FILE_Eng_CmdQFifo_Status(TSP_FILE_ENG Eng, MS_U8 *pu8FifoLevel)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    *pu8FifoLevel = HAL_TSP_Filein_CmdQLv(eFileEng);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_TimeStampEnablePlaybackStamp(TSP_FILE_ENG Eng,MS_BOOL bEnable)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_PacketMode(eFileEng,bEnable);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStamp(TSP_FILE_ENG Eng, MS_U32 u32Stamp)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_SetTimeStamp(eFileEng, u32Stamp);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_TimeStampSetPlaybackStampClk(TSP_FILE_ENG Eng, TSP_TimeStamp_Clk eClk)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    TSP_HAL_TimeStamp_Clk eTimeStampClk = E_TSP_HAL_TIMESTAMP_CLK_INVALID;

    switch(eClk)
    {
        case E_TSP_TIMESTAMP_CLK_90K:
            eTimeStampClk= E_TSP_HAL_TIMESTAMP_CLK_90K;
            break;
        case E_TSP_TIMESTAMP_CLK_27M:
            eTimeStampClk= E_TSP_HAL_TIMESTAMP_CLK_27M;
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d] UnSupport TimeStamp Clock!\n",__FUNCTION__,__LINE__);
            TSP_RETURN(E_TSP_FAIL_PARAMETER);
    }

    HAL_TSP_Filein_SetTimeStampClk(eFileEng, eTimeStampClk);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_TimeStampGetPlaybackStamp(TSP_FILE_ENG Eng, MS_U32* u32Stamp)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    *u32Stamp = HAL_TSP_Filein_GetTimeStamp(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_GetFileInTimeStamp(TSP_FILE_ENG Eng, MS_U32* u32TSLen)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    *u32TSLen = HAL_TSP_Filein_PktTimeStamp(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_GetFileInCurReadAddr(TSP_FILE_ENG Eng, MS_PHY *pu32Addr)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)Eng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_GetCurAddr(eFileEng,pu32Addr);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FILE_Eng_MOBF_Enable(TSP_FILE_ENG eEng, MS_BOOL bEnable, MS_U32 u32Key)
{
    TSP_ENTRY();

    FILEENG_SEQ eFileEng = E_FILEENG_INVALID;
    eFileEng = HAL_TSP_FilePath2Tsif_Mapping((MS_U32)eEng);

    TSP_RES_ALLOC_TSPPATH_CHECK(eFileEng);

    HAL_TSP_Filein_MOBF_Enable(eFileEng, bEnable, u32Key);

    TSP_RETURN(E_TSP_OK);
}

// -------------------------------------------------------------
// Debug table
// -------------------------------------------------------------
#ifdef DEBUG_TABLE_SUPPORT

static TSP_DST_SEQ _TSP_DrvHal_DstMapping(DrvTSP_AVFIFO eDebugFifo)
{
    TSP_DST_SEQ eRetDst = E_TSP_DST_INVALID;

    switch(eDebugFifo)
    {
        case E_DRVTSP_AVFIFO_VIDEO ... E_DRVTSP_AVFIFO_VIDEO8:
            eRetDst= (TSP_DST_SEQ)(E_TSP_DST_FIFO_VIDEO + (eDebugFifo - E_DRVTSP_AVFIFO_VIDEO));
            break;
        case E_DRVTSP_AVFIFO_AUDIO ... E_DRVTSP_AVFIFO_AUDIOF:
            eRetDst = (TSP_DST_SEQ)(E_TSP_DST_FIFO_AUDIO + (eDebugFifo - E_DRVTSP_AVFIFO_AUDIO));
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d] UnSupport Debug AV Type!\n",__FUNCTION__,__LINE__);
            break;
    }

    return eRetDst;
}

static TSP_HAL_FLOW _TSP_DrvHal_FlowMapping(DrvTSP_Flow eDebugFlow)
{
    TSP_HAL_FLOW eHalFlow = E_TSP_HAL_FLOW_INVALID;

    switch(eDebugFlow)
    {
        case E_DRVTSP_FLOW_LIVE0:
            eHalFlow= E_TSP_HAL_FLOW_LIVE0;
            break;
        case E_DRVTSP_FLOW_LIVE1:
            eHalFlow= E_TSP_HAL_FLOW_LIVE1;
            break;
        case E_DRVTSP_FLOW_LIVE2:
            eHalFlow= E_TSP_HAL_FLOW_LIVE2;
            break;
        case E_DRVTSP_FLOW_LIVE3:
            eHalFlow= E_TSP_HAL_FLOW_LIVE3;
            break;
       case E_DRVTSP_FLOW_FILE0:
            eHalFlow= E_TSP_HAL_FLOW_FILE0;
            break;
        case E_DRVTSP_FLOW_FILE1:
            eHalFlow= E_TSP_HAL_FLOW_FILE1;
            break;
        case E_DRVTSP_FLOW_FILE2:
            eHalFlow= E_TSP_HAL_FLOW_FILE2;
            break;
        case E_DRVTSP_FLOW_FILE3:
            eHalFlow= E_TSP_HAL_FLOW_FILE3;
            break;
        case E_DRVTSP_FLOW_LIVE4:
        case E_DRVTSP_FLOW_FILE4:
            eHalFlow= E_TSP_HAL_FLOW_LIVE4;
            break;
        case E_DRVTSP_FLOW_LIVE5:
        case E_DRVTSP_FLOW_FILE5:
            eHalFlow= E_TSP_HAL_FLOW_LIVE5;
            break;
        case E_DRVTSP_FLOW_LIVE6:
        case E_DRVTSP_FLOW_FILE6:
            eHalFlow= E_TSP_HAL_FLOW_LIVE6;
            break;
        case E_DRVTSP_FLOW_MMFI0:
            eHalFlow= E_TSP_HAL_FLOW_MMFI0;
            break;
        case E_DRVTSP_FLOW_MMFI1:
            eHalFlow= E_TSP_HAL_FLOW_MMFI1;
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d] UnSupport Debug Flow !\n",__FUNCTION__,__LINE__);
            break;
    }

    return eHalFlow;
}

TSP_Result MDrv_TSP_Get_DisContiCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_AVFIFO eAvType, DrvTSP_Flow eFlow, MS_U32 *pu32Cnt)
{
    TSP_DST_SEQ  eDstType = _TSP_DrvHal_DstMapping(eAvType);
    TSP_HAL_FLOW eHalFlow = _TSP_DrvHal_FlowMapping(eFlow);
    TSP_SRC_SEQ  ePktDmx  = HAL_TSP_Debug_Flow2PktDmx_Mapping(eHalFlow);

    if(eDstType==E_TSP_DST_INVALID || eHalFlow==E_TSP_HAL_FLOW_INVALID || ePktDmx==E_TSP_SRC_INVALID)
    {
        return E_TSP_FAIL;
    }

    TSP_ENTRY();

    HAL_TSP_Debug_DropDisPktCnt_Src(eDstType,ePktDmx);
    HAL_TSP_Debug_DisPktCnt_Load(eDstType,FALSE,TRUE);
    *pu32Cnt = HAL_TSP_Debug_DropDisPktCnt_Get(ePktDmx, FALSE);

    switch(eCmd)
    {
        case E_DRVTSP_DEBUG_CMD_NONE:
        case E_DRVTSP_DEBUG_CMD_DISABLE:
            break;
        case E_DRVTSP_DEBUG_CMD_CLEAR:
            HAL_TSP_Debug_ClrSrcSel(ePktDmx);
            HAL_TSP_Debug_DisPktCnt_Clear(eDstType);
            break;
        case E_DRVTSP_DEBUG_CMD_ENABLE:
            HAL_TSP_Debug_DisPktCnt_Load(eDstType,TRUE,TRUE);
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] UnSupported Debug Cmd  !!\n",__FUNCTION__,__LINE__);
            TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_Get_DropPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_AVFIFO eAvType, DrvTSP_Flow eFlow, MS_U32 *pu32Cnt)
{
    TSP_DST_SEQ  eDstType = _TSP_DrvHal_DstMapping(eAvType);
    TSP_HAL_FLOW eHalFlow = _TSP_DrvHal_FlowMapping(eFlow);
    TSP_SRC_SEQ  ePktDmx  = HAL_TSP_Debug_Flow2PktDmx_Mapping(eHalFlow);

    if(eDstType==E_TSP_DST_INVALID || eHalFlow==E_TSP_HAL_FLOW_INVALID || ePktDmx==E_TSP_SRC_INVALID)
    {
        return E_TSP_FAIL;
    }

    TSP_ENTRY();

    HAL_TSP_Debug_DropDisPktCnt_Src(eDstType,ePktDmx);
    HAL_TSP_Debug_DropPktCnt_Load(eDstType, FALSE);
    *pu32Cnt = HAL_TSP_Debug_DropDisPktCnt_Get(ePktDmx, TRUE);

    switch(eCmd)
    {
        case E_DRVTSP_DEBUG_CMD_NONE:
        case E_DRVTSP_DEBUG_CMD_DISABLE:
            break;
        case E_DRVTSP_DEBUG_CMD_CLEAR:
            HAL_TSP_Debug_ClrSrcSel(ePktDmx);
            HAL_TSP_Debug_DropPktCnt_Clear(eDstType);
            break;
        case E_DRVTSP_DEBUG_CMD_ENABLE:
            HAL_TSP_Debug_DropPktCnt_Load(eDstType, TRUE);
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] UnSupported Debug Cmd  !!\n",__FUNCTION__,__LINE__);
            TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_Get_LockPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_Flow eFlow, MS_U32 *pu32Cnt)
{
    TSP_TSIF eTSIF;

    switch(eFlow)
    {
        case E_DRVTSP_FLOW_LIVE0:
            eTSIF = E_TSP_TSIF_0;
            break;
        case E_DRVTSP_FLOW_LIVE1:
            eTSIF = E_TSP_TSIF_1;
            break;
        case E_DRVTSP_FLOW_LIVE2:
            eTSIF = E_TSP_TSIF_2;
            break;
        case E_DRVTSP_FLOW_LIVE3:
            eTSIF = E_TSP_TSIF_3;
            break;
        case E_DRVTSP_FLOW_LIVE4:
            eTSIF = E_TSP_TSIF_4;
            break;
        case E_DRVTSP_FLOW_LIVE5:
            eTSIF = E_TSP_TSIF_5;
            break;
        case E_DRVTSP_FLOW_LIVE6:
            eTSIF = E_TSP_TSIF_6;
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] UnSupport Debug Flow !!\n",__FUNCTION__,__LINE__);
            return E_TSP_FAIL;
    }

    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);

    if(eHalTSIF==E_TSP_HAL_TSIF_INVALID)
    {
        return E_TSP_FAIL;
    }

    MS_U32 u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

    TSP_ENTRY();

    HAL_TSP_Debug_LockPktCnt_Src(u32TSIF);
    HAL_TSP_Debug_LockPktCnt_Load(u32TSIF,FALSE);
    *pu32Cnt = HAL_TSP_Debug_LockPktCnt_Get(u32TSIF, TRUE);

    switch(eCmd)
    {
        case E_DRVTSP_DEBUG_CMD_NONE:
        case E_DRVTSP_DEBUG_CMD_DISABLE:
            break;
        case E_DRVTSP_DEBUG_CMD_CLEAR:
            HAL_TSP_Debug_LockPktCnt_Clear(u32TSIF);
            break;
        case E_DRVTSP_DEBUG_CMD_ENABLE:
            HAL_TSP_Debug_LockPktCnt_Load(u32TSIF,TRUE);
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] UnSupported Debug Cmd !!\n",__FUNCTION__,__LINE__);
            TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_Get_AVPktCnt(DrvTSP_Debug_Cmd eCmd, DrvTSP_AVFIFO eAvType, DrvTSP_Flow eFlow, MS_U32 *pu32Cnt)
{
    TSP_DST_SEQ  eDstType = _TSP_DrvHal_DstMapping(eAvType);
    TSP_HAL_FLOW eHalFlow = _TSP_DrvHal_FlowMapping(eFlow);
    TSP_SRC_SEQ  ePktDmx  = HAL_TSP_Debug_Flow2PktDmx_Mapping(eHalFlow);

    if(eDstType==E_TSP_DST_INVALID || eHalFlow==E_TSP_HAL_FLOW_INVALID || ePktDmx==E_TSP_SRC_INVALID)
    {
        return E_TSP_FAIL;
    }

    TSP_ENTRY();

    HAL_TSP_Debug_AvPktCnt_Src(eDstType,ePktDmx);
    HAL_TSP_Debug_AvPktCnt_Load(eDstType, FALSE);
    *pu32Cnt = HAL_TSP_Debug_AvPktCnt_Get(eDstType);

    switch(eCmd)
    {
        case E_DRVTSP_DEBUG_CMD_NONE:
        case E_DRVTSP_DEBUG_CMD_DISABLE:
            break;
        case E_DRVTSP_DEBUG_CMD_CLEAR:
            HAL_TSP_Debug_ClrSrcSel(ePktDmx);
            HAL_TSP_Debug_AvPktCnt_Clear(eDstType);
            break;
        case E_DRVTSP_DEBUG_CMD_ENABLE:
            HAL_TSP_Debug_AvPktCnt_Load(eDstType, TRUE);
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] UnSupported Debug Cmd !!\n",__FUNCTION__,__LINE__);
            TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);

}

#if 0
TSP_Result MDrv_TSP_Get_ErrPktCnt(DrvTSP_ErrPktCnt_info* tspInfo, MS_U32* pu32Cnt)
{
    MS_U32  u32SrcPath = 0;

    switch(tspInfo->TspTsif)
    {
        case E_DRVTSP_DEBUG_TSIF_TS0:
            u32SrcPath = 0;
            break;
        case E_DRVTSP_DEBUG_TSIF_TS1:
            u32SrcPath = 1;
            break;
        case E_DRVTSP_DEBUG_TSIF_TS2:
            u32SrcPath = 2;
            break;
        case E_DRVTSP_DEBUG_TSIF_TS3:
            u32SrcPath = 3;
            break;
        default:
            break;
    }

    TSP_ENTRY();

    switch(tspInfo->TspCmd)
    {
        case E_DRVTSP_DEBUG_CMD_NONE:
            break;
        case E_DRVTSP_DEBUG_CMD_CLEAR:
            HAL_TSP_Clr_Src_Set(u32SrcPath);
            HAL_TSP_ERRPKTCnt_Clear(u32SrcPath);
            break;
        case E_DRVTSP_DEBUG_CMD_ENABLE:
        case E_DRVTSP_DEBUG_CMD_DISABLE:
            HAL_TSP_ERRPKTCnt_Load_En(u32SrcPath,((tspInfo->TspCmd == E_DRVTSP_DEBUG_CMD_ENABLE)? TRUE : FALSE));
            break;
        default:
            ULOGE("TSP", "[%s][%d] operation not support !!\n",__FUNCTION__,__LINE__);
            TSP_RETURN(E_TSP_FAIL);
    }

    HAL_TSP_ERRPKTCnt_Sel(u32SrcPath);
    *pu32Cnt = HAL_TSP_ERRPKTCnt_Get();

    TSP_RETURN(E_TSP_OK);
}
#endif

#endif


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
TSP_Result MDrv_TSP_MStr_SyncByte(DrvTSP_If eTsIf, MS_U8 u8StrId, MS_U8* pu8SyncByte, MS_BOOL bSet)
{
#ifdef MERGE_STR_SUPPORT
    MS_U8 u8Path = 0;

    if(u8StrId >= TSP_MERGESTREAM_NUM)
    {
        TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Stream index out of range !!\n",__FUNCTION__,__LINE__);
        return E_TSP_FAIL;
    }
    TSP_HAL_TSIF eHalTSIF = MDrv_TSP_TsifMapping(eTsIf);
    u8Path = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

    TSP_RES_ALLOC_TSPPATH_CHECK(u8Path);

    TSP_ENTRY();

    if(HAL_TSP_PktConverter_SetSyncByte(u8Path, u8StrId, pu8SyncByte, bSet) == FALSE)
    {
        TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);

#else
    return E_TSP_FAIL_NOT_SUPPORTED;
#endif
}


//-------------------------------------------------------------------------------------------------
/// Get current system time clock (STC) of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32STC_32              \b OUT: pointer of STC[32] return
/// @param  pu32STC                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC(MS_U32 u32EngId, MS_U32 *pu32STC_H, MS_U32 *pu32STC)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(pu32STC_H, "[%04d] NULL pointer\n", __LINE__);
    TSP_CHECK(pu32STC, "[%04d] NULL pointer\n", __LINE__);

    TSP_ENTRY();

    TSP_RES_ALLOC_STC_CHECK(u32EngId);
    //HAL_TSP_CMDQ_GetSTC(pu32STC_H, pu32STC);
    //HAL_TSP_GetSTC(pu32STC_H, pu32STC);
    HAL_TSP_STC33_CmdQGet(pu32STC_H, pu32STC);

    TSP_RETURN(E_TSP_OK);
}


//-------------------------------------------------------------------------------------------------
/// Get PRC of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32Pcr_32              \b OUT: pointer of STC[32] return
/// @param  pu32Pcr                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetPCR(MS_U32 u32EngId, MS_U32 *pu32PCR_H, MS_U32 *pu32PCR)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    TSP_CHECK(pu32PCR_H, "[%04d] NULL pointer\n", __LINE__);
    TSP_CHECK(pu32PCR, "[%04d] NULL pointer\n", __LINE__);

    TSP_ENTRY();

    HAL_TSP_PcrFlt_GetPcr(u32EngId, pu32PCR_H, pu32PCR);

    TSP_RETURN(E_TSP_OK);
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
        return E_TSP_FAIL;
    }

    *ppVersion = &_drv_tsp_version;

    return E_TSP_OK;
}


TSP_Result MDrv_TSP_GetStatus(TSP_HW_Status *HW_Status, TSP_SW_Status *SW_Status)
{
    TSP_ENTRY();
    if (HAL_TSP_HCMD_Alive())
    {
        *HW_Status = E_TSP_HW_ALIVE;
    }
    else
    {
        *HW_Status = E_TSP_HW_DEALIVE;
    }
    *SW_Status = _ptsp_res->_tspInfo.Status;

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_GetFWVER(MS_U32* u32FWVer)
{
    TSP_ENTRY();
    *u32FWVer = HAL_TSP_HCMD_GetInfo(INFO_FW_VERSION);
    TSP_RETURN(E_TSP_OK);
}

//@F_TODO api layer throw a enum that we don't have. check u8IfUID
TSP_Result MDrv_TSP_GetTSIFStatus(TSP_TSIF eTSIF, TSP_TSPad* ePad, MS_BOOL* pbClkInv, MS_BOOL* pbExtSyc, MS_BOOL* pbParl)
{
    TSP_Result Res = E_TSP_OK;
    MS_U32 u32TSIF;
    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
    //if live-in and file-in of a DmxFlow map to diff TSIF, return the status of the live-in one
    u32TSIF = HAL_TSP_TsifMapping(eHalTSIF, FALSE);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32TSIF);

    *ePad = _ptsp_res->TsIf_Src[u32TSIF].ePad;
    *pbClkInv = _ptsp_res->TsIf_Src[u32TSIF].bClkInv;
    *pbExtSyc = _ptsp_res->TsIf_Src[u32TSIF].bExtSync;
    *pbParl = _ptsp_res->TsIf_Src[u32TSIF].bParallel;

    return Res;
}

//--------------------------------------------------------------------------------------------------
/// Get command queue fifo level
/// @param  pu8FifoLevel      \b OUT: fifo level, 0~3
/// @return TSP_Result
/// @note
//--------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_CmdQFifo_Status(MS_U8 *pu8FifoLevel)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *pu8FifoLevel = HAL_TSP_Filein_CmdQLv(eFileEng);
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_SetDbgLevel(TSP_DbgLevel DbgLevel)
{
    _u32DbgLevel = DbgLevel;
    return E_TSP_OK;
}

static TSP_HAL_CAP_TYPE _TSP_DrvHal_CapsMapping(TSP_Caps eCap)
{
    TSP_HAL_CAP_TYPE eRetCapType = E_TSP_HAL_CAP_VAL_NULL;

    switch(eCap)
    {
        case E_TSP_CAP_PIDFLT_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_PIDFLT_NUM;
            break;
        case E_TSP_CAP_SECFLT_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_SECFLT_NUM;
            break;
        case E_TSP_CAP_SECBUF_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_SECBUF_NUM;
            break;
        case E_TSP_CAP_RECENG_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_RECENG_NUM;
            break;
        case E_TSP_CAP_RECFLT_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_RECFLT_NUM;
            break;
        case E_TSP_CAP_MMFI_AUDIO_FILTER_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_MMFI_AUDIO_FILTER_NUM;
            break;
        case E_TSP_CAP_MMFI_V3D_FILTER_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_MMFI_V3D_FILTER_NUM;
            break;
        case E_TSP_CAP_TSIF_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_TSIF_NUM;
            break;
        case E_TSP_CAP_DEMOD_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_DEMOD_NUM;
            break;
        case E_TSP_CAP_TSPAD_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_TSPAD_NUM;
            break;
        case E_TSP_CAP_VQ_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_VQ_NUM;
            break;
        case E_TSP_CAP_CAFLT_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_CAFLT_NUM;
            break;
        case E_TSP_CAP_FW_ALIGN:
            eRetCapType = E_TSP_HAL_CAP_TYPE_FW_ALIGN;
            break;
        case E_TSP_CAP_VQ_ALIGN:
            eRetCapType = E_TSP_HAL_CAP_TYPE_VQ_ALIGN;
            break;
        case E_TSP_CAP_VQ_PITCH:
            eRetCapType = E_TSP_HAL_CAP_TYPE_VQ_PITCH;
            break;
        case E_TSP_CAP_SECBUF_ALIGN:
            eRetCapType = E_TSP_HAL_CAP_TYPE_SECBUF_ALIGN;
            break;
        case E_TSP_CAP_PVR_ALIGN:
            eRetCapType = E_TSP_HAL_CAP_TYPE_PVR_ALIGN;
            break;
        case E_TSP_CAP_HW_TYPE:
            eRetCapType = E_TSP_HAL_CAP_TYPE_HW_TYPE;
            break;
        case E_TSP_CAP_VFIFO_NUM:
            eRetCapType = E_TSP_HAL_CAP_VAL_VFIFO_NUM;
            break;
        case E_TSP_CAP_AFIFO_NUM:
            eRetCapType = E_TSP_HAL_CAP_TYPE_AFIFO_NUM;
            break;
        case E_TSP_CAP_HWPCR_SUPPORT:
            eRetCapType = E_TSP_HAL_CAP_TYPE_HWPCR_SUPPORT;
            break;
        case E_TSP_CAP_PCRFLT_START_IDX:
            eRetCapType = E_TSP_HAL_CAP_TYPE_PCRFLT_START_IDX;
            break;
        case E_TSP_CAP_FIQ_NUM:
            eRetCapType = E_TSP_HAL_CAP_FIQ_NUM;
            break;
        case E_TSP_CAP_RECFLT_IDX:
            eRetCapType = E_TSP_HAL_CAP_TYPE_RECFLT_IDX;
            break;
        case E_TSP_CAP_FW_BUF_SIZE:
            eRetCapType = E_TSP_HAL_CAP_FW_BUF_SIZE;
            break;
        case E_TSP_CAP_FW_BUF_RANGE:
            eRetCapType = E_TSP_HAL_CAP_FW_BUF_RANGE;
            break;
        case E_TSP_CAP_VQ_BUF_RANGE:
            eRetCapType = E_TSP_HAL_CAP_VQ_BUF_RANGE;
            break;
        case E_TSP_CAP_SEC_BUF_RANGE:
            eRetCapType = E_TSP_HAL_CAP_SEC_BUF_RANGE;
            break;
        default:
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Not support !!\n",__FUNCTION__,__LINE__);
            break;
    }

    return eRetCapType;
}

//-------------------------------------------------------------------------------------------------
/// Query CAPs and get it's information
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetCaps(TSP_Caps eCap, MS_U32 *pu32CapInfo)
{
    TSP_HAL_CAP_TYPE _eCap;

    if (eCap >= E_TSP_CAP_NULL)
    {
        return E_TSP_FAIL;
    }

    if(eCap == E_TSP_CAP_RESOURCE_SIZE)
    {
        *pu32CapInfo = sizeof(TSP_RESOURCE_PRIVATE);
        return E_TSP_OK;
    }

    _eCap = _TSP_DrvHal_CapsMapping(eCap);
    if(_eCap == E_TSP_HAL_CAP_TYPE_NULL) //Not support
    {
        return E_TSP_FAIL;
    }

    switch(_eCap)
    {
        case E_TSP_HAL_CAP_TYPE_PIDFLT_NUM:
            *pu32CapInfo = (_tspFltIdMap.u32PcrFltId_End - _tspFltIdMap.u32PidFltId_Start);
            return E_TSP_OK;
        case E_TSP_HAL_CAP_TYPE_SECFLT_NUM:
            *pu32CapInfo = (_tspFltIdMap.u32SecFltId_End - _tspFltIdMap.u32SecFltId_Start);
            return E_TSP_OK;
        case E_TSP_HAL_CAP_TYPE_SECBUF_NUM:
            *pu32CapInfo = (_tspFltIdMap.u32SecBufId_End - _tspFltIdMap.u32SecBufId_Start);
            return E_TSP_OK;
        case E_TSP_HAL_CAP_TYPE_RECFLT_NUM:
            *pu32CapInfo = _tspFltIdMap.u32PidFltId_End;
            return E_TSP_OK;
        case E_TSP_HAL_CAP_TYPE_CAFLT_NUM:
            *pu32CapInfo = (_tspFltIdMap.u32PidFltId_End - _tspFltIdMap.u32PidFltId_Start);
            return E_TSP_OK;
        case E_TSP_HAL_CAP_TYPE_PCRFLT_START_IDX:
            *pu32CapInfo = _tspFltIdMap.u32PcrFltId_Start;
            return E_TSP_OK;
        case E_TSP_HAL_CAP_TYPE_RECFLT_IDX:
            *pu32CapInfo = _tspFltIdMap.u32RecFltIdx;
            return E_TSP_OK;
        default:
            return (HAL_TSP_GetCaps(_eCap, pu32CapInfo) == FALSE)? E_TSP_FAIL : E_TSP_OK;
    }

}

TSP_Result MDrv_TSP_GetFileInTimeStamp(MS_U32* u32TSLen)
{
    TSP_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *u32TSLen = HAL_TSP_Filein_PktTimeStamp(eFileEng);
    TSP_RETURN(E_TSP_OK);
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
    ret = (TSP_Result) _ptsp_res->_tspInfo.LastErr;
    _ptsp_res->_tspInfo.LastErr = E_TSP_OK;
    return ret;
}

TSP_Result MDRV_TSP_FAIL_NOT_SUPPORTED(void)
{
    return E_TSP_FAIL_NOT_SUPPORTED;
}


//[RESERVED]
TSP_Result MDrv_TSP_Alive(MS_U32 u32EngId)
{
    TSP_ENTRY();
    if (HAL_TSP_HCMD_Alive())
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}


//[RESERVED]
TSP_Result MDrv_TSP_DBG_GetDBGStatus(MS_U32 u32Sel, MS_U32* u32Status)
{
    TSP_ENTRY();

   // *u32Status = HAL_TSP_GetDBGStatus((MS_U16)u32Sel);//@F_TODO not implement yet

    TSP_RETURN(E_TSP_OK);
}


//[RESERVED]
MS_U32 MDrv_TSP_DBG_TaskLine(void)
{
    return _ptsp_res->_tspInfo.u32TaskLine;
}


//[RESERVED]
TSP_Result MDrv_TSP_DBG_FltInfo(MS_U32 u32FltId, MS_U32 u32BufId)
{
    TSP_ENTRY();

 //   HAL_TSP_DBG_DumpFlt((MS_U16)u32FltId, (MS_U16)u32BufId); //@F_TODO not implement yet

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_Eng_FltAlloc(MS_U32 u32Eng, MS_U32 *pu32FltId)
{
    TSP_PVR_ENTRY();
    MS_U32 ret = E_TSP_FAIL;

    TSP_SRC_SEQ eEngSrc = E_TSP_SRC_INVALID;
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        HAL_PVR_GetEngSrc(u32Eng, &eEngSrc);
        ret = _MDrv_FltAllocForRec(u32Eng, eEngSrc, pu32FltId);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    TSP_PVR_RETURN(ret);
}

TSP_Result MDrv_TSP_PVR_Eng_FltFree(MS_U32 u32Eng, MS_U32 u32FltId)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_FltFreeForRec(u32FltId);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }
    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltSetPID(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32 u32PID)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_FltSetPIDForRec(u32FltId, u32PID);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltEnable(MS_U32 u32Eng, MS_U32 u32FltId, MS_BOOL bEnable)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_FltEnableForRec(u32FltId, bEnable);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_FltGetPID(MS_U32 u32Eng, MS_U32 u32FltId, MS_U32* u32PID)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_FltGetPIDForRec(u32FltId, u32PID);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_SetBuffer(u32Eng, u32Start0, u32Start1, u32Size0, u32Size1);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_Start(MS_U32 u32Eng, MS_BOOL bPvrAll, MS_BOOL bStart)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        if(bPvrAll)
            _MDrv_PVR_Eng_Start(u32Eng, E_TSP_DRV_PVR_RecMode_ALL, bStart);
        else
            _MDrv_PVR_Eng_Start(u32Eng, E_TSP_DRV_PVR_RecMode_PID, bStart);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_Pause(u32Eng, bPause);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHY *pu32WriteAddr)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_GetWriteAddr(u32Eng, pu32WriteAddr);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_Notify(u32Eng, eEvents, pfCallback);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_SetPacketMode(u32Eng, bSet);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_TimeStampSetRecordStamp(u32Eng, u32Stamp);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp)
{
    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        _MDrv_PVR_Eng_TimeStampGetRecordStamp(u32Eng, u32Stamp);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_PVR_Eng_SetCaMode(MS_U32 u32EngId, MS_U16 u16CaMode, MS_BOOL bSpsEnable)
{
    TSP_ENTRY();

    if(HAL_TSP_CAPVR_SPSEnable(u32EngId, u16CaMode, bSpsEnable))
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_OK);
    }
}

TSP_Result MDrv_TSP_PVR_Eng_MOBF_Enable(MS_U32 u32Eng, MS_BOOL bEnable, MS_U32 u32Key)
{
    TSP_ENTRY();

    if (E_TSP_PVR_PVRENG_END > u32Eng) // PVR Eng
    {
        HAL_PVR_MOBF_Enable(u32Eng, bEnable, u32Key);
    }
    else
    {
        TSP_DBG(E_TSP_DBG_MUST, "[ERROR][%s][%d]Incorrect Engine type!\n",__FUNCTION__,__LINE__);
    }

    TSP_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltAllocForRec(MS_U32 u32Eng, TSP_SRC_SEQ eEngSrc, MS_U32 *pu32FltId)
{
    TSP_PIDFLT_SRC ePidFltSrc = HAL_TSP_PktDmx2FltSrc_Mapping(eEngSrc);
    TSP_FltType eTspFltSrcType = E_TSP_FLT_SRC_LIVE0;
    TSP_FltType eTspFltDstType = E_TSP_FLT_FIFO_PVR0;

    switch (ePidFltSrc)
    {
        case E_TSP_PIDFLT_LIVE0:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE0;
            break;
        case E_TSP_PIDFLT_LIVE1:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE1;
            break;
        case E_TSP_PIDFLT_LIVE2:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE2;
            break;
        case E_TSP_PIDFLT_LIVE3:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE3;
            break;
        case E_TSP_PIDFLT_FILE0:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE0;
            break;
        case E_TSP_PIDFLT_FILE1:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE1;
            break;
        case E_TSP_PIDFLT_FILE2:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE2;
            break;
        case E_TSP_PIDFLT_FILE3:
            eTspFltSrcType = E_TSP_FLT_SRC_FILE3;
            break;
        case E_TSP_PIDFLT_LIVE4:
        case E_TSP_PIDFLT_FILE4:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE4;
            break;
        case E_TSP_PIDFLT_LIVE5:
        case E_TSP_PIDFLT_FILE5:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE5;
            break;
        case E_TSP_PIDFLT_LIVE6:
        case E_TSP_PIDFLT_FILE6:
            eTspFltSrcType = E_TSP_FLT_SRC_LIVE6;
            break;
        default:
            break;
    }

    if(u32Eng < TSP_PVRENG_NUM)
    {
        eTspFltDstType = E_TSP_FLT_FIFO_PVR0 + u32Eng;
    }

    if (E_TSP_OK != MDrv_TSP_FLT_Alloc(0, (eTspFltSrcType | eTspFltDstType), pu32FltId))
    {
        return E_TSP_FAIL;
    }

    return E_TSP_OK;
}

static TSP_Result _MDrv_FltFreeForRec(MS_U32 u32FltId)
{
    TSP_PVR_ENTRY();

    //@NOTE shouldn't we modify sw flt status here? No, since it's maitain in fle_enable and flt_free
    MDrv_TSP_FLT_Enable( u32FltId, FALSE);
    MDrv_TSP_FLT_SetPID( u32FltId, TSP_PID_NULL);
    MDrv_TSP_FLT_Free( u32FltId);

    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltSetPIDForRec(MS_U32 u32FltId, MS_U32 u32PID)
{
    TSP_PVR_ENTRY();
    MDrv_TSP_FLT_SetPID( u32FltId, u32PID);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltGetPIDForRec(MS_U32 u32FltId, MS_U32* pu32PID)
{
    TSP_PVR_ENTRY();
    MDrv_TSP_FLT_GetPID(u32FltId, pu32PID);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_FltEnableForRec(MS_U32 u32FltId, MS_BOOL bEnable)
{
    TSP_PVR_ENTRY();
    MDrv_TSP_FLT_Enable( u32FltId, bEnable);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_SetBuffer(MS_U32 u32Eng, MS_PHYADDR u32Start0, MS_PHYADDR u32Start1, MS_U32 u32Size0, MS_U32 u32Size1)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetStr2Miu_StartAddr(u32Eng, u32Start0, u32Start1);
    HAL_PVR_SetStr2Miu_MidAddr(u32Eng, u32Start0 + u32Size0 , u32Start1 + u32Size1 );
    HAL_PVR_SetStr2Miu_EndAddr(u32Eng, u32Start0 + u32Size0 , u32Start1 + u32Size1 );
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_Start(MS_U32 u32Eng, TSP_DRV_PVR_RecMode eRecMode, MS_BOOL bStart)
{
    TSP_PVR_ENTRY();

    if (!bStart)
    {
        HAL_PVR_Stop(u32Eng);
    }
    else
    {
        switch (eRecMode)
        {
            case E_TSP_DRV_PVR_RecMode_PID:
            {
                HAL_PVR_RecPid(u32Eng , TRUE);
                break;
            }
            case E_TSP_DRV_PVR_RecMode_ALL:
            {
                HAL_PVR_RecPid(u32Eng , FALSE);
                break;
            }
            default :
                break;
        }
        HAL_PVR_Start(u32Eng);
    }

    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_Pause(MS_U32 u32Eng, MS_BOOL bPause)
{
    TSP_PVR_ENTRY();
    HAL_PVR_Pause(u32Eng, bPause);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_GetWriteAddr(MS_U32 u32Eng, MS_PHY *pu32WriteAddr)
{
    TSP_PVR_ENTRY();
    *pu32WriteAddr = HAL_PVR_GetWritePtr(u32Eng);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_Notify(MS_U32 u32Eng, TSP_Event eEvents, P_TSP_Callback pfCallback)
{
    #if 0
    if(pfCallback)
    {
        _TSP_SetPvrNotifyId(u32Eng);
    }
    else
    {
        _TSP_RemovePvrNotifyId;
    }
    #endif

    return E_TSP_FAIL_NOT_SUPPORTED;
}

static TSP_Result _MDrv_PVR_Eng_SetPacketMode(MS_U32 u32Eng, MS_BOOL bSet)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetStrPacketMode(u32Eng, bSet);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_TimeStampSetRecordStamp(MS_U32 u32Eng, MS_U32 u32Stamp)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetPVRTimeStamp(u32Eng, u32Stamp);
    TSP_PVR_RETURN(E_TSP_OK);
}

static TSP_Result _MDrv_PVR_Eng_TimeStampGetRecordStamp(MS_U32 u32Eng, MS_U32* u32Stamp)
{
    TSP_PVR_ENTRY();
    *u32Stamp = HAL_PVR_GetPVRTimeStamp(u32Eng);
    TSP_PVR_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_TimeStampSetPlaybackStamp(MS_U32 u32Stamp)
{
    TSP_PVR_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    HAL_TSP_Filein_SetTimeStamp(eFileEng, u32Stamp);
    TSP_PVR_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_TimeStampGetPlaybackStamp(MS_U32* u32Stamp)
{
    TSP_PVR_ENTRY();
    FILEENG_SEQ eFileEng = HAL_TSP_GetDefaultFileinEng();
    *u32Stamp = HAL_TSP_Filein_GetTimeStamp(eFileEng);
    TSP_PVR_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PVR_TimeStamp(MS_BOOL bEnable)
{
    TSP_PVR_ENTRY();
    HAL_PVR_SetStrPacketMode(0,bEnable);
    TSP_PVR_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable remove duplicate A/V packets
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_RemoveDupAVPkt(MS_BOOL bEnable)
{
    TSP_ENTRY();
    HAL_TSP_PktDmx_RmDupAVPkt(bEnable);
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable or Disable to remove audio,or vedio error packets.
/// @param  bEnable                \b IN: Enable or Disable
/// @return TSP_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_TEI_RemoveErrorPkt(TSP_DRV_TEI_RmPktType eDrvPktType, MS_BOOL bEnable)
{
    TSP_ENTRY();

    TSP_HAL_TEI_RmPktType eHalPktType = E_TSP_HAL_TEI_REMOVE_AUDIO_PKT;

    switch ( eDrvPktType )
    {
        case E_TSP_DRV_TEI_REMOVE_AUDIO_PKT:
            eHalPktType = E_TSP_HAL_TEI_REMOVE_AUDIO_PKT;
            break;
        case E_TSP_DRV_TEI_REMOVE_VIDEO_PKT:
            eHalPktType = E_TSP_HAL_TEI_REMOVE_VIDEO_PKT;
            break;
        default:
            TSP_DBG(E_TSP_DBG_WARNING, "[%04d] Unsupport PktType = %u\n", __LINE__, (MS_U32) eDrvPktType);
            TSP_RETURN(E_TSP_FAIL);
    }

    HAL_TSP_TEI_RemoveErrorPkt(eHalPktType, bEnable);

    TSP_RETURN(E_TSP_OK);
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
    TSP_ENTRY();
//  HAL_TSP_STC64_Mode_En(bEnable); // @F_TODO not implement yet do we have to enable this in kaiser?
    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
//[Reserved]
// Memory to TSP stream command : Update Stream STC64
// @param  u32EngId                  \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
// @param  u32STC_32               \b IN: STC[63:32] for next input stream
// @param  u32STC                    \b IN: STC[31:0] for next input stream
// @return TSP_Result
// @note M2T Command Size: 3
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_SetSTC64(MS_U32 u32EngId, MS_U32 u32STC_32, MS_U32 u32STC)
{
    TSP_ENTRY();

    TSP_RES_ALLOC_STC_CHECK(u32EngId);
    HAL_TSP_STC64_Set(u32EngId, u32STC_32, u32STC);

    TSP_RETURN(E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set STC control
/// @param u8Eng \b         IN: STC Engine ID
/// @param u8Opt \b         IN: STC update control options
///                                         0x00 -> Update STC by TSP FW (driver)
///                                         0x01 -> Update STC by HK (Set by AP)
///                                         0x02 -> Update STC Once by driver when PCR reset
/// @note
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Mode)
{
    TSP_ENTRY();

    TSP_RES_ALLOC_STC_CHECK(u8Eng);
    if (u8Mode ==0x00)
    {
        _ptsp_res->_u32_StcUpdateCtrl[u8Eng].bEnable = TRUE;
        _ptsp_res->_u32_StcUpdateCtrl[u8Eng].bUpdateOnce = FALSE;
        TSP_RETURN(E_TSP_OK);
    }
    else if( u8Mode ==0x01)
    {
        _ptsp_res->_u32_StcUpdateCtrl[u8Eng].bEnable = FALSE;
        _ptsp_res->_u32_StcUpdateCtrl[u8Eng].bUpdateOnce = FALSE;
        TSP_RETURN(E_TSP_OK);
    }
    else if( u8Mode ==0x02)
    {
        _ptsp_res->_u32_StcUpdateCtrl[u8Eng].bEnable = TRUE;
        _ptsp_res->_u32_StcUpdateCtrl[u8Eng].bUpdateOnce = TRUE;
        TSP_RETURN(E_TSP_OK);
    }

    TSP_RETURN(E_TSP_FAIL);
}

TSP_Result MDrv_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd)
{
    TSP_ENTRY();

    TSP_RES_ALLOC_STC_CHECK(u32EngId);
    _ptsp_res->_u32_StcOffset[u32EngId].u32StcOffset = u32Offset;
    _ptsp_res->_u32_StcOffset[u32EngId].bAdd= bAdd;

    TSP_RETURN (E_TSP_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get current system time clock (STC64) of TSP
/// @param  u32EngId                \b IN: index of TSP engine [ 0 .. (@ref DRVTSP_ENGINE_NUM-1) ]
/// @param  pu32STC_32              \b OUT: pointer of STC[32:63] return
/// @param  pu32STC                 \b OUT: pointer of STC[31:0] return
/// @return TSP_Result
//-------------------------------------------------------------------------------------------------
TSP_Result MDrv_TSP_GetSTC64(MS_U32 u32EngId, MS_U32 *pu32STC_32, MS_U32 *pu32STC)
{
    TSP_ENTRY();
    TSP_RES_ALLOC_STC_CHECK(u32EngId);

    HAL_TSP_STC64_Get(u32EngId, pu32STC_32, pu32STC);

    TSP_RETURN(E_TSP_OK);
}

#endif

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
    if(u32Percentage == 0)
    {
        return E_TSP_FAIL;
    }

    MS_U64 u64CW_New = 0;

    TSP_ENTRY();

    u64CW_New = (((MS_U64)STC_SYNTH_DEFAULT) & 0xFFFFFFFFUL) * (((MS_U64)_ptsp_res->_u32StcAdjustUnit) & 0xFFFFFFFFUL) / (((MS_U64)u32Percentage) & 0xFFFFFFFFUL);

    //ULOGD("TSP", "[%s](%d, %d, %d) CW %d\n", __FUNCTION__, (unsigned int)u32EngId, (unsigned int)bUpClk, (unsigned int)u32Percentage, (unsigned int)u64CW_New);
    HAL_TSP_SetSTCSynth(u32EngId, (MS_U32)u64CW_New);

    TSP_RETURN(E_TSP_OK);
}



TSP_Result _TSP_FLT_SrcSwitch(TSP_FltType SrcFlow ,TSP_FltType TgtFlow)
{
    MS_U32 i ;//,dummy;

    // Define check policy for filters //
    #define CHECK_FILTER_PROPERTY(_idx_) ((_ptsp_res->_tspInfo.FltType[_idx_] & SrcFlow)\
                                        && (_ptsp_res->_tspInfo.FltState[_idx_] & E_TSP_FLT_STATE_ALLOC) \
                                        && (_ptsp_res->_tspInfo.FltState[_idx_] != E_TSP_FLT_STATE_NA) \
                                        && (FALSE ==(_ptsp_res->_tspInfo.FltType[_idx_] & E_TSP_FLT_FIFO_MASK ))\
                                        && (E_TSP_FLT_USER_PCR != (_ptsp_res->_tspInfo.FltType[_idx_]& E_TSP_FLT_USER_MASK )))


    //ULOGD("TSP", "Src Flow = %08lX  -- TgtFlow = %08lx \n",SrcFlow,TgtFlow);
#if 0 // @FIXME seeems we dont need this
    if (TgtFlow & E_TSP_FLT_SRC_FILE )
    {
        TSP_OS_Print("[%s] Target Flow Error , Flow = %08lx \n",__FUNCTION__,TgtFlow);
        return E_TSP_FAIL;
    }
#endif

    // Stop all target source Filter
    for ( i = 0 ; i < _tspFltIdMap.u32PidFltId_End  ; i ++)
    {
        if (CHECK_FILTER_PROPERTY(i))
        {
            TSP_LOCK();
            HAL_TSP_PidFlt_SetFltOut(i, TSP_PIDFLT_OUT_NONE);
            TSP_UNLOCK();
        }
    }

    // wait for all Task solved
    MsOS_DelayTask(100);


    for ( i = 0 ; i < _tspFltIdMap.u32PidFltId_End  ; i ++)
    {
        if (CHECK_FILTER_PROPERTY(i))
        {

            TSP_LOCK();
            // Setting new Filter input source.

            // @TODO  not review and implement yet: check everthing
            #if 0
            if (TgtFlow & E_TSP_FLT_SRC_LIVE0)
            {
                HAL_TSP_PidFlt_SetFltIn(i, TSP_PIDFLT_TSIF0);
            }
            else if (TgtFlow & E_TSP_FLT_SRC_TSIF1)
            {
                HAL_TSP_PidFlt_SetFltIn(i, TSP_PIDFLT_TSIF1);
            }
            else if (TgtFlow & E_TSP_FLT_SRC_TSIF2)
            {
                HAL_TSP_PidFlt_SetFltIn(i, TSP_PIDFLT_TSIF2);
            }
            else
            {
                TSP_OS_Print("[%s][%d]Error in target source = %lx \n",__FUNCTION__,__LINE__,TgtFlow);
            }
            #endif

            _ptsp_res->_tspInfo.FltType[i] = (_ptsp_res->_tspInfo.FltType[i] & ~E_TSP_FLT_SRC_MASK) | TgtFlow ;
            TSP_UNLOCK();

            // Restart Running Filter
            if (_ptsp_res->_tspInfo.FltState[i] & E_TSP_FLT_STATE_ENABLE)
            {
                MDrv_TSP_FLT_Enable( i , TRUE );
            }
        }
    }

    TSP_LOCK();
    if (SrcFlow == _ptsp_res->_Current_Live)
    {
        _ptsp_res->_Current_Live = TgtFlow & E_TSP_FLT_SRC_MASK;
    }
    TSP_UNLOCK();

    return (E_TSP_OK);

    #undef CHECK_FILTER_PROPERTY

}

TSP_Result MDrv_TSP_FLT_LiveSrcSwitch(TSP_FltType TgtFlow)
{
    //TSP_OS_Print("[%s][%d]\n",__FUNCTION__,__LINE__);

    TSP_Result ret = E_TSP_FAIL;

    if ((TgtFlow&E_TSP_FLT_SRC_MASK ) == _ptsp_res->_Current_Live)
    {
        return E_TSP_FAIL;
    }

    ret = _TSP_FLT_SrcSwitch(_ptsp_res->_Current_Live, TgtFlow);

    return ret;
}


TSP_Result MDrv_TSP_FLT_ChangeSource(MS_U32 u32FltId, TSP_FltType eTSPFltTypeSrc)
{
    TSP_CHECK(_tspFltIdMap.u32PidFltIdAll > u32FltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)u32FltId);

   // HAL_TSP_GetCaps(E_TSP_HAL_CAP_TYPE_RECFLT_IDX, &u32RecFltIdx);
    if(u32FltId == _tspFltIdMap.u32RecFltIdx)
        return E_TSP_OK;

    TSP_ENTRY();

    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        //stop hw pcr
        if ((u32FltId >= TSP_PCRFLT_GET_ID(0)) && (u32FltId <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
        {
            HAL_TSP_PcrFlt_Enable(u32FltId - _tspFltIdMap.u32PcrFltId_Start, FALSE);
        }
        else
        {
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Incorrect PCR filter ID !!\n",__FUNCTION__,__LINE__);
        }
    }
    else
    {
        //stop pid flts
        HAL_TSP_PidFlt_SetFltOut(u32FltId, TSP_PIDFLT_OUT_NONE); // stop filter
    }

    // wait for all Task solved
    MsOS_DelayTask(20);

    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & eTSPFltTypeSrc;
    TSP_PIDFLT_SRC eTspPidSrc = _TSP_DrvHal_FltSrcMapping(u32FltSrc);

    if ((_ptsp_res->_tspInfo.FltType[u32FltId]& E_TSP_FLT_USER_MASK)== E_TSP_FLT_USER_PCR)
    {
        //change hw pcr src
        if ((u32FltId >= HAL_TSP_PCRFLT_GET_ID(0)) && (u32FltId <= HAL_TSP_PCRFLT_GET_ID(TSP_PCRFLT_NUM - 1)))
        {
            TSP_PCR_SRC ePcrSrc = HAL_TSP_FltSrc2PCRSrc_Mapping(eTspPidSrc);

            TSP_RES_ALLOC_TSPPATH_CHECK(ePcrSrc);

            HAL_TSP_PcrFlt_SetSrc(u32FltId - _tspFltIdMap.u32PcrFltId_Start, ePcrSrc);
        }
        else
        {
            TSP_DBG(E_TSP_DBG_MUST, "[%s][%d] Incorrect PCR filter ID !!\n",__FUNCTION__,__LINE__);
        }
    }
    else
    {
        //change pid flt src
        MS_U32 u32PktDmx = HAL_TSP_FltSrc2PktDmx_Mapping(eTspPidSrc);

        TSP_RES_ALLOC_TSPPATH_CHECK(u32PktDmx);

        HAL_TSP_PidFlt_SetFltIn(u32FltId,u32PktDmx);
    }

    _ptsp_res->_tspInfo.FltType[u32FltId] = (_ptsp_res->_tspInfo.FltType[u32FltId] & ~E_TSP_FLT_SRC_MASK) | eTSPFltTypeSrc ;

    if (_ptsp_res->_tspInfo.FltState[u32FltId] & E_TSP_FLT_STATE_ENABLE)
    {
        _MDrv_TSP_FLT_Enable( u32FltId, TRUE );
    }

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_PARL_BitOrderSwap(MS_U16 u16TSIF, MS_BOOL bInvert)
{
    TSP_RES_ALLOC_TSPPATH_CHECK(u16TSIF);

    TSP_ENTRY();
    HAL_TSP_TSIF_BitSwap(u16TSIF, bInvert); //@F_TODO check if this is the same with `HAL_TSP_SetDataSwap'
    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_FQ_FltNullPkt(MS_U32 u32FQEng, MS_BOOL bFltNull)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);

    MS_U32 u32MapingFQEng = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
    TSP_FQ_NUM_CHECK_RET(u32MapingFQEng);

    TSP_Result ret;

    TSP_ENTRY();

    ret = HAL_TSP_FQ_FLT_NULL_PKT(u32MapingFQEng, bFltNull);

    if(ret==FALSE)
    {
        TSP_RETURN(E_TSP_FAIL_NOT_SUPPORTED);
    }
    else
    {
        TSP_RETURN(E_TSP_OK);
    }
}

TSP_Result MDrv_TSP_FQ_SetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf eTsSrc)
{
    TSP_DBG(E_TSP_DBG_TRACK, "%s\n", __FUNCTION__);
    MS_U32 u32FQSrc = 0;

    TSP_FQ_NUM_CHECK_RET(u32FQEng);

    TSP_ENTRY();

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
        case E_DRVFQ_SRC_DEFAULT:
            u32FQSrc = HAL_TSP_Tsif2Fq_Mapping(u32FQEng);
            break;
        default:
            TSP_RETURN(E_TSP_FAIL_PARAMETER);
    }

    if(!HAL_TSP_FQ_SetMuxSwitch(u32FQEng,u32FQSrc))
    {
        TSP_RETURN(E_TSP_FAIL);
    }
    else
    {
        TSP_RETURN(E_TSP_OK);
    }
}

TSP_Result MDrv_TSP_FQ_GetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf* peTsSrc)
{
    MS_U32 u32FQSrc = E_DRVFQ_SRC_DEFAULT;

    TSP_FQ_NUM_CHECK_RET(u32FQEng);

    TSP_ENTRY();

    u32FQSrc = HAL_TSP_FQ_GetMuxSwitch(u32FQEng);

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
            *peTsSrc = E_DRVFQ_SRC_DEFAULT;
            TSP_RETURN(E_TSP_FAIL);
    }

    TSP_RETURN(E_TSP_OK);
}


TSP_Result MDrv_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData)
{

    if(u32Cmd == DRV_DMX_CMD_STC_ADJUST_UNIT)
    {
        _ptsp_res->_u32StcAdjustUnit = u32Config0;
        return (E_TSP_OK);
    }

    if(HAL_TSP_CMD_Run(u32Cmd, u32Config0, u32Config1,pData))
        return (E_TSP_OK);
    else
        return (E_TSP_FAIL);
}

TSP_Result MDrv_TSP_DSCMB_Functionality(TSP_DSCMB_FuncCMD eCmd, MS_U32 u32Config0, MS_U32 u32Config1 ,void *pData)
{//this func should only be called by DSCMB driver to do DSCMB functionality, and maintained by DSCMB owner
//the mutex _s32MutexId is utilized to avoid race condition between DSCMB & TSP accessing shared registers
    TSP_ENTRY();

    switch(eCmd)
    {
        default:
            break;
    }

    TSP_RETURN(E_TSP_OK);
}


TSP_Result MDrv_TSP_Flt_GetScmbSts(TSP_FltType eFltType, MS_U32 u32FltGroupId, MS_U32 PidFltId, MS_U32 *pu32ScmbSts)
{
    TSP_CHECK(_tspFltIdMap.u32PidFltId_End > PidFltId, "[%04d] Invalid u32FltId %d\n", __LINE__, (unsigned int)PidFltId);

    TSP_ENTRY();

    MS_U32 u32FltSrc = E_TSP_FLT_SRC_MASK & eFltType;
    TSP_PIDFLT_SRC eTspPidSrc = _TSP_DrvHal_FltSrcMapping(u32FltSrc);
    MS_U32 u32PktDmx = HAL_TSP_FltSrc2PktDmx_Mapping(eTspPidSrc);

    TSP_RES_ALLOC_TSPPATH_CHECK(u32PktDmx);

    if( HAL_DSCMB_GetStatus( u32PktDmx, u32FltGroupId, PidFltId, pu32ScmbSts) )
    {
        TSP_RETURN(E_TSP_OK);
    }
    else
    {
        TSP_RETURN(E_TSP_FAIL);
    }
}

TSP_Result MDrv_TSP_DropScmbPkt(TSP_FltType eFilterType, MS_BOOL bEnable)
{
    TSP_DST_SEQ eDst = _TSP_DrvHal_FltType2DstMapping(eFilterType);

    TSP_ENTRY();

    HAL_TSP_FIFO_Skip_Scrmb(eDst, bEnable);

    TSP_RETURN(E_TSP_OK);
}

TSP_TSIF MDrv_TSP_TsifMapping(TSP_TSIF eTSIF)
{
    TSP_HAL_TSIF eHalTSIF = _TSP_DrvHal_TSIFMapping(eTSIF);
    MS_U32 u32TSIF = HAL_TSP_TsifMapping(eHalTSIF,FALSE);

    switch(u32TSIF)
    {
        case TSP_TSIF0:
            return E_TSP_TSIF_0;
        case TSP_TSIF1:
            return E_TSP_TSIF_1;
        case TSP_TSIF2:
            return E_TSP_TSIF_2;
        case TSP_TSIF3:
            return E_TSP_TSIF_3;
        case TSP_TSIF4:
            return E_TSP_TSIF_4;
        case TSP_TSIF5:
            return E_TSP_TSIF_5;
        case TSP_TSIF6:
            return E_TSP_TSIF_6;
        default:
            TSP_DBG(E_TSP_DBG_WARNING, "[%04d] TSIF %u is wrong !! \n", __LINE__, u32TSIF);
            return E_TSP_TSIF_0;
    }
}

TSP_Result MDrv_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY *pphyStartAddr, MS_PHY *pphyEndAddr)
{
    TSP_ENTRY();

    // SEC #0 , #1
    HAL_TSP_SEC_Address_Protect(0, pphyEndAddr[0], pphyStartAddr[0]);
    HAL_TSP_SEC_Address_Protect(1, pphyEndAddr[1], pphyStartAddr[1]);
    HAL_TSP_SEC_Address_Protect_En(bEnable);

    // PVR #1 ~ #3
    HAL_TSP_PVR_Address_Protect(0, pphyEndAddr[2], pphyStartAddr[2]);
    HAL_TSP_PVR_Address_Protect_En(0,bEnable);
    HAL_TSP_PVR_Address_Protect(1, pphyEndAddr[3], pphyStartAddr[3]);
    HAL_TSP_PVR_Address_Protect_En(1,bEnable);
    HAL_TSP_PVR_Address_Protect(2, pphyEndAddr[4], pphyStartAddr[4]);
    HAL_TSP_PVR_Address_Protect_En(2,bEnable);

    TSP_RETURN(E_TSP_OK);
}

TSP_Result MDrv_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr)
{
    TSP_ENTRY();

    HAL_TSP_OR_Address_Protect(phyEndAddr, phyStartAddr);
    HAL_TSP_OR_Address_Protect_En(bEnable);

    TSP_RETURN(E_TSP_OK);
}

//for kernel mode multiple process support, create eventid of callback thread of user mode in fitst time
#ifdef MSOS_TYPE_LINUX_KERNEL
static char tspevt_name[] = "tspevt";
#endif
TSP_Result MDrv_TSP_Create_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
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
        if((pstIoSignal->s32KerModeTspEvtId = TSP_OS_EventCreate((MS_U8*)tspevt_name)) != -1)
        {
            for(u32ii = 0; u32ii < DRVTSP_MAX_PROCESS_NUM; u32ii++)
            {
                if((_u32KPrcEvtTblUseFlag & (1 << u32ii)) == 0)
                {
                    pstIoSignal->u32TblIndex = u32ii;
                    _stKModePrcEvtTbl[u32ii].tgid = current->tgid;
                    _stKModePrcEvtTbl[u32ii].s32TspEvtId = pstIoSignal->s32KerModeTspEvtId;
                    _u32KPrcEvtTblUseFlag |= (1 << u32ii);
                    break;
                }
            }

            //printf("[%s][%d] _u32KPrcEvtTblUseFlag %x , s32TspEvtId %x \n", __FUNCTION__, __LINE__, (int)_u32KPrcEvtTblUseFlag, (int)pstIoSignal->s32KerModeTspEvtId);

            return E_TSP_OK;
        }
        else
        {
            return E_TSP_FAIL;
        }
    }

    return E_TSP_OK;
#else
    return E_TSP_FAIL_NOT_SUPPORTED;
#endif
}

// Send free event to end user mode callback thread, and delete event id
TSP_Result MDrv_TSP_Close_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
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
        return E_TSP_FAIL;

    //printf("%s][%d] s32KerModeTspEvtId %x\n", __FUNCTION__, __LINE__, (int)pstIoSignal->s32KerModeTspEvtId);

    TSP_SetEvent(pstIoSignal->s32KerModeTspEvtId, TSP_EVENT_TASKEND);

    //to free CB process task, so send one event
    MsOS_DelayTask(3);

    while(_s32_IsrTaskReturn != TRUE)
    {
        TSP_SetEvent(pstIoSignal->s32KerModeTspEvtId, TSP_EVENT_TASKEND);
        MsOS_DelayTask(3);
    }

    while(TSP_OS_EventDelete(pstIoSignal->s32KerModeTspEvtId) == FALSE)
    {
        TSP_SetEvent(pstIoSignal->s32KerModeTspEvtId, TSP_EVENT_TASKEND);
        printf("Waiting for evet deleting... \n");
        MsOS_DelayTask(3);
    }
    pstIoSignal->s32KerModeTspEvtId = -1;

    _u32KPrcEvtTblUseFlag &= ~(1 << u32ii);
    _stKModePrcEvtTbl[u32ii].s32TspEvtId = -1;
    _stKModePrcEvtTbl[u32ii].tgid = 0;

#endif

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_Wait_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U8 u8Index;
    MS_S32 s32Ret = 0;
    MS_U32 u32Event = 0;

    for(u8Index = 0; u8Index < TSP_MAX_PVRSIGFLAG_NUM; ++u8Index)
    {
        pstIoSignal->u32PvrBufFullFlag[u8Index] = 0;
    }

    for(u8Index = 0; u8Index < TSP_MAX_SIGFLAG_NUM; ++u8Index)
    {
        pstIoSignal->u32SecRdyFlag[u8Index] = 0;
        pstIoSignal->u32SecOvfFlag[u8Index] = 0;
        pstIoSignal->u32SecCrcErrFlag[u8Index] = 0;
    }

#if 1
    if((s32Ret = MsOS_WaitEvent_Interrupt(pstIoSignal->s32KerModeTspEvtId, TSP_TASK_EVENTS, &u32Event, E_OR_CLEAR, MSOS_WAIT_FOREVER)) <= 0)
    {
        pstIoSignal->u32TspEvt = 0;

        if(s32Ret < 0) // -ERESTARTSYS
        {
            return E_TSP_FAIL_FUNCTION;
        }
        else
        {
            return E_TSP_FAIL;
        }
    }
#else
    s32Ret = TSP_GetEvent(_s32EventId, &u32Event, TSP_TASK_EVENTS, E_OR_CLEAR, MSOS_WAIT_FOREVER);
    if((u32Event & TSP_TASK_EVENTS) == 0)
    {
        return E_TSP_FAIL;
    }
#endif

    if(u32Event & TSP_EVENT_TASKEND)
    {
        _s32_IsrTaskReturn = TRUE;
    }

    pstIoSignal->u32TspEvt = u32Event;
#endif

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_Proc_IoSignal(DrvTSP_IoSignal *pstIoSignal)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32  fltid, flags = 0, ModeMask, dummy, BaseFlt;
    MS_U16  i, u16BufId;
    MS_BOOL bData;

    if(pstIoSignal->u32TspEvt & TSP_EVENT_FREEFLT)
    {
        for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
        {
            // Add to FREE list
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeFree(i, ModeMask,&flags, TSP_OS_EVENT_OR_CLEAR);

            while (flags)
            {
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);
                TSP_LOCK();
                _TSP_FLT_Disable(fltid); // disable output
                TSP_UNLOCK();
                flags &= ~(1 << BaseFlt);
                TSP_FLAG_SetDone(fltid);

                // Clear other filter activities
                TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
                TSP_FLAG_GetSecOvf(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
                TSP_FLAG_GetSecRdy(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);
            }
        }
    }

    MsOS_DelayTask(1);

    // Check all section avaiable filters
    if (pstIoSignal->u32TspEvt & TSP_EVENT_SECTION)
    {
        for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
        {
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask

            TSP_FLAG_ModeCrcErr(i, ModeMask,&flags, TSP_OS_EVENT_OR_CLEAR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only one-shot filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                // cause we serve free event before section rdy
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE))
                {
                    if ((_ptsp_res->_tspInfo.FltState[fltid] & E_TSP_FLT_STATE_ENABLE) &&
                        (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_SEC_CRCERROR) &&
                        (_ptsp_res->_tspInfo.FltCallback[fltid]))
                    {
                        pstIoSignal->u32SecCrcErrFlag[i] |= (1UL << (BaseFlt & 0x1FUL));
                    }

                }
                flags &= ~(1 << BaseFlt);
            }

            TSP_FLAG_ModeSecRdy(i, ModeMask,&flags, TSP_OS_EVENT_OR_CLEAR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only one-shot filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                // cause we serve free event before section rdy
                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) && (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE))
                {
                    TSP_LOCK();
                    u16BufId = HAL_TSP_SecFlt_GetSecBuf(&(_REGSec->Flt[fltid]));
                    bData = (HAL_TSP_SecBuf_GetRead(&(_REGBuf->Buf[u16BufId])) != HAL_TSP_SecBuf_GetWrite(&(_REGBuf->Buf[u16BufId]))) ? TRUE : FALSE;
                    TSP_UNLOCK();

                    if(bData)
                    {
                        if ((_ptsp_res->_tspInfo.FltState[fltid] & E_TSP_FLT_STATE_ENABLE) &&
                            (_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_DATA_READY) &&
                            (_ptsp_res->_tspInfo.FltCallback[fltid]))
                        {
                            pstIoSignal->u32SecRdyFlag[i] |= (1UL << (BaseFlt & 0x1FUL));
                        }

                    }

                    if (TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR))
                    {
                        TSP_LOCK();
                        _TSP_FLT_Disable(fltid);
                        TSP_UNLOCK();
                    }
                }
                flags &= ~(1 << BaseFlt);
            }

            TSP_FLAG_ModeSecOvf(i, ModeMask,&flags, TSP_OS_EVENT_OR_CLEAR);
            while (flags)
            {
                //(POLL) Check any possible filter should be disabled
                //(TASK) Check only overflowed filter and to disable
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);

                if ((_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_FREE) &&
                    (_ptsp_res->_tspInfo.FltState[fltid] != E_TSP_FLT_STATE_ISR_FREE) &&
                    (!TSP_FLAG_GetOvfRst(fltid)))
                {
                    TSP_LOCK();
                    if (HAL_TSP_PidFlt_GetFltOutput(&(_REGPid1->Flt[fltid])) & TSP_PIDFLT_OUT_SECFLT)
                    {
                        _TSP_FLT_Disable(fltid);
                    }
                    TSP_FLAG_SetOvfRst(fltid);
                    TSP_UNLOCK();

                    TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR);

                    if ((_ptsp_res->_tspInfo.FltEvent[fltid] & E_TSP_EVENT_BUF_OVERFLOW) &&
                        (_ptsp_res->_tspInfo.FltCallback[fltid]))
                    {
                        pstIoSignal->u32SecOvfFlag[i] |= (1UL << (BaseFlt & 0x1FUL));
                    }
                }
                flags &= ~(1 << BaseFlt);
                // Clear other filter activities
                TSP_FLAG_GetSecRdy(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR); // Clear section ready also
            }

            MsOS_DelayTask(1);
        }
    }

    MsOS_DelayTask(1);

    if (pstIoSignal->u32TspEvt & TSP_EVENT_DISABLEFLT) //Disable Polling mode filter,CB mode filter disable at SecRdy or SecOvf
    {
        for (i = 0; i < ((TSP_SECFLT_NUM+31)>>5); i++)
        {
            TSP_FLAG_GetModeFlags(i, ModeMask); // get CB mode Filter Mask
            TSP_FLAG_ModeFltDis(i,~ModeMask, &flags ,TSP_OS_EVENT_OR_CLEAR); // Polling mode
            while (flags)
            {
                BaseFlt = MAsm_CPU_GetTrailOne(flags);
                fltid = BaseFlt + (i<<5);
                if (TSP_FLAG_GetFltDis(fltid, &dummy, TSP_OS_EVENT_OR_CLEAR))
                {
                    TSP_LOCK();
                    _TSP_FLT_Disable(fltid); // disable output
                    TSP_UNLOCK();
                }
                flags &= ~(1 << BaseFlt);
            }
        }
    }

#endif

    return E_TSP_OK;
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
            return E_TSP_FAIL;
        }
    }

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_Ree_TeeCmdSystem_Exit()
{
    _bTSPMbxInitFlag = FALSE;
    _u8TSPMbxMsgClass = 0;

    return E_TSP_OK;
}

TSP_Result MDrv_TSP_Ree_SendTeeCmd(TSP_REE_TO_TEE_CMD_TYPE cmd_type, void* param, size_t datasize)
{

    if(_ptsp_res->_teemode.OsType == SYS_TEEINFO_OSTYPE_NUTTX)
    {
        if(_TSP_Ree_SendMBXMsg(cmd_type, param) == TRUE)
            return E_TSP_OK;
        else
            return E_TSP_FAIL;
    }
    else if(_ptsp_res->_teemode.OsType == SYS_TEEINFO_OSTYPE_OPTEE)
    {
        unsigned int ret, ret_orig= 0;

        if ((ret = MDrv_SYS_TEEC_InitializeContext(_dmx_device, &dmx_mstar_teec_ctx)) != TEEC_SUCCESS)
        {
            printf("%s %d: Init Context failed, err code: %x\n",__FUNCTION__,__LINE__, (int)ret);
            return E_TSP_FAIL;
        }
        if ((ret = MDrv_SYS_TEEC_Open(&dmx_mstar_teec_ctx, &dmx_session, &dmx_uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &ret_orig)) != TEEC_SUCCESS)
        {
            printf("%s %d: TEEC Open session failed, err code: %x\n",__FUNCTION__,__LINE__, (int)ret);
            return E_TSP_FAIL;
        }

        if(datasize == 0)
        {
            dmx_op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,TEEC_NONE, TEEC_NONE);
        }
        else
        {
            dmx_op.params[0].tmpref.buffer = param;
            dmx_op.params[0].tmpref.size = datasize;
            dmx_op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_NONE, TEEC_NONE, TEEC_NONE);
        }

        if ((ret = MDrv_SYS_TEEC_InvokeCmd(&dmx_session, cmd_type, &dmx_op, &ret_orig)) != TEEC_SUCCESS)
        {
            printf("%s %d: TEEC Invoke command failed, err code: %x\n",__FUNCTION__,__LINE__, (int)ret);
        }
        MDrv_SYS_TEEC_Close(&dmx_session);
        MDrv_SYS_TEEC_FinalizeContext(&dmx_mstar_teec_ctx);
        if(ret == TEEC_SUCCESS)
        {
            return E_TSP_OK;
        }
    }

    return E_TSP_FAIL;
}

#endif //SECURE_PVR_ENABLE
