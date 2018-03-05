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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvNDS.c
/// @brief  NDS Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#ifdef MSOS_TYPE_LINUX
#include "string.h"
#endif

#include "asmCPU.h"
#include "regNDS.h"
#include "drvNDS.h"
#include "halNDS.h"
#if defined(CHIP_U3)
#include "../tsp/drvTSP.h"
// TSP2 to TSP wrapper
#define E_TSP_OK                        DRVTSP_OK
#define E_TSP_FAIL                      DRVTSP_FAIL
#define E_TSP_FLT_SRC_TSIF0             E_DRVTSP_FLT_SOURCE_TYPE_LIVE
#define E_TSP_FLT_USER_SEC              E_DRVTSP_FLT_TYPE_SECTION
#define E_TSP_FLT_USER_PKT              E_DRVTSP_FLT_TYPE_PACKET
#define E_TSP_FLT_USER_EMM              E_DRVTSP_FLT_TYPE_EMM
#define E_TSP_FLT_USER_ECM              E_DRVTSP_FLT_TYPE_ECM
#define TSP_FltState                    DrvTSP_FltState
#define E_TSP_FLT_STATE_FREE            E_DRVTSP_FLT_STATE_FREE
#define TSP_PID_NULL                    DRVTSP_PID_NULL
#define MDrv_TSP_FLT_Alloc(i, p...)     MDrv_TSP_PidFlt_Alloc(0, p)
#define MDrv_TSP_FLT_SetPID(p...)       MDrv_TSP_PidFlt_SetPid(0, p)
#define MDrv_TSP_FLT_Enable(p...)       MDrv_TSP_PidFlt_Enable(0, p)
#define MDrv_TSP_FLT_Free(p...)         MDrv_TSP_PidFlt_Free(0, p)
#define MDrv_TSP_FLT_GetState(p...)     MDrv_TSP_PidFlt_GetState(0, p)
#else
#include "../tsp2/drvTSP2.h"
#endif

#include "ddiNDS_HDI.h"

#include "nds.h"

#define NDS_FLT_DBG(_fmt, _args...)     { } // printf(_fmt, ##_args)


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define NDS_EMM_PASSIVE                 1

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//FIXME: it's no use?
#define NDS_FLT_ENTRY()                 MsOS_ObtainMutex(_nds_flt_mutexid, MSOS_WAIT_FOREVER)
#define NDS_FLT_RETURN(_ret)            { MsOS_ReleaseMutex(_nds_flt_mutexid); return _ret; }
#define NDS_FLT_LOCK()                  MsOS_ObtainMutex(_nds_flt_mutexid, MSOS_WAIT_FOREVER)
#define NDS_FLT_UNLOCK()                MsOS_ReleaseMutex(_nds_flt_mutexid)
// -jerry

#define NDS_EMM_EVENT_BUFFER            0x00000010      // 0x00000010-0x00000019
#define NDS_EMM_EVENT_BUFFER_MASK       0x0000000F
#define NDS_EMM_EVENT_OVERFLOW_HW       0x00000020
#define NDS_EMM_EVENT_OVERFLOW_SW       0x00000040
#define NDS_ECM_EVENT_FILTER            0x00FFFF00
#define NDS_ECM_EVENT_FILTER_SHFT       8
#define NDS_ECM_EVENT_OVERFLOW          0xFF000000
#define NDS_ECM_EVENT_OVERFLOW_SHFT     24
#define NDS_ALL_EVENT                   0xFFFFFFFF

#define NDS_FLT_OS_AttachInterrupt(isr)  MsOS_AttachInterrupt(NDS_INT_EMM_ECM, isr)
#define NDS_FLT_OS_EnableInterrupt()     MsOS_EnableInterrupt(NDS_INT_EMM_ECM );
#define NDS_FLT_OS_DisableInterrupt()    MsOS_DisableInterrupt(NDS_INT_EMM_ECM );
#define NDS_FLT_OS_DetachInterrupt()     MsOS_DetachInterrupt(NDS_INT_EMM_ECM);


#define GET_4BYTE( cp )                 ( ( ( (MS_U32)(*cp) )<< 24 ) | ( ( (MS_U32)(*(cp+1)) ) << 16 ) | ( ( (MS_U32)(*(cp+2)) ) << 8 ) | ( (MS_U32)(*(cp+3)) ) )

#define NDS_FLT_NULL                    0xFFFFFFFF

#define NDS_FLT_TASK_STACK_SIZE         4096


//------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct _NDS_FLT_Dev
{
    NDS_FLT_Param                   param;

} NDS_FLT_Dev;


typedef struct
{
    MS_U16                          pid;                                // PID status for REG_CAM_ECM_PID (W_ONLY)
    MS_U32                          flt_id;
#if defined(CHIP_U3)
    MS_U32                          buf_id;
#endif

    MS_U32                          x_connect;
    MS_U8                           ecm_id;
    MS_BOOL                         b_defined;

#if NDS_ECM_SAME_PID_DROP_EN
    MS_U8                           u8Drop;
    MS_U8                           u8Ctrl;
#endif

} NDS_ECM_Flt;


typedef struct
{
    MS_U16                          pid;                                // PID status for REG_CAM_EMM_PID (W_ONLY)
    MS_U32                          flt_id;
#if defined(CHIP_U3)
    MS_U32                          buf_id;
#endif

    MS_U32                          x_connect;
    MS_U8                           flt_ctrl;

} NDS_EMM_Flt;

//[NEW] Refine?
#define MAX_EMM_LIST_NUM            11 //(NDS_CAM_EMM_BUF_NUM+1) // N for N-1 slots

MS_U8 _u8EmmRead = 0;
MS_U8 _u8EmmWrite = 0;
MS_U8 _u8EmmCur = 0;

#define emmQueueInc()                                   \
{                                                       \
    _u8EmmWrite++;                                      \
    if (MAX_EMM_LIST_NUM <= _u8EmmWrite)                \
    {                                                   \
        _u8EmmWrite = 0;                                \
    }                                                   \
}

#define emmQueueDec()                                   \
{                                                       \
    _u8EmmRead++;                                       \
    if (MAX_EMM_LIST_NUM <= _u8EmmRead)                 \
    {                                                   \
        _u8EmmRead = 0;                                 \
    }                                                   \
}

#define emmQueueIsEmpty()       (_u8EmmRead == _u8EmmWrite)
#define emmQueueIsFull()        (0 == emmQueueFreeSize())
#define emmQueueUsedSize()      ((_u8EmmWrite >= _u8EmmRead) ? (_u8EmmWrite - _u8EmmRead) : (MAX_EMM_LIST_NUM - _u8EmmRead + _u8EmmWrite))
#define emmQueueFreeSize()      (MAX_EMM_LIST_NUM - emmQueueUsedSize() - 1)
#define emmQueueReset()         { _u8EmmRead = _u8EmmWrite = _u8EmmCur = 0 }
// -Jerry


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static NDS_FLT_Dev                  _nds_flt_dev = {
                                        .param.ecm_emm_addr = (MS_PHYADDR)NULL,
                                        .param.pu8ecm_emm_buf = (MS_U8*)NULL,
                                    };

#if !defined(MSOS_TYPE_LINUX_KERNEL)
static MS_U32                       _nds_flt_stack[NDS_FLT_TASK_STACK_SIZE/sizeof(MS_U32)];
#endif
static MS_S32                       _nds_flt_eventid = -1, _nds_flt_mutexid = -1, _nds_flt_taskid = -1;

static NDS_ECM_Flt                  _EcmFlt[NDS_CAM_ECM_FLT_NUM];
static NDS_EMM_Flt                  _EmmFlt;

MS_U8                               _u8EmmBuf[MAX_EMM_LIST_NUM][NDS_CAM_FLT_BUF_SIZE];
MS_U8                               _u8EcmBuf[NDS_CAM_ECM_FLT_NUM][2][NDS_CAM_FLT_BUF_SIZE];

extern void _verSC_PrintData(MS_U8 *pu8Data, MS_U16 u16DataLen);

#if 0
extern NDS_Result NDS_RASP_DefineEcmFilter (NDS_ULONG Xconn , NDS_BYTE ecm_filter_num , NDS_BYTE ecm_filter_control);
extern NDS_Result NDS_RASP_DefineEcmPid (NDS_ULONG Xconn , NDS_BYTE ecm_filter_num ,NDS_USHORT Pid,NDS_BYTE odd_ecm_table_id,NDS_BYTE even_ecm_table_id );
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//[NEW] refine? is it necessary to reduce OVERFLOW?
MS_BOOL bEmmOverflow = TRUE;
// -Jerry


void _EMM_FreeFilter(void)
{
    int                 i;

    MDrv_TSP_FLT_Enable(_EmmFlt.flt_id, FALSE);
#if defined(CHIP_U3)
    MDrv_TSP_SecFlt_Free(0, _EmmFlt.buf_id);
#endif
    MDrv_TSP_FLT_Free(_EmmFlt.flt_id);

    for (i = 0; i < NDS_CAM_EMM_BUF_NUM; i++)
    {
        memset(_u8EmmBuf[i], 0, NDS_CAM_FLT_BUF_SIZE);
    }
    memset((void*)MS_PA2KSEG1(_nds_flt_dev.param.ecm_emm_addr+NDS_CAM_ECM_BUFS_SIZE), 0, NDS_CAM_FLT_BUF_SIZE * NDS_CAM_EMM_BUF_NUM);
    MsOS_FlushMemory();

    _EmmFlt.flt_id = NDS_FLT_NULL;
    _EmmFlt.pid = TSP_PID_NULL;

    bEmmOverflow = TRUE;
}


MS_BOOL _EMM_AllocFilter(MS_U32 x_connection, MS_U16 pid, MS_U8 emm_filter_control)
{
    if (NDS_FLT_NULL != _EmmFlt.flt_id)
    {
        TSP_FltState        eTspState;
        TSP_Result          eTspRet = E_TSP_FAIL;
        eTspRet = MDrv_TSP_FLT_GetState(_EmmFlt.flt_id, &eTspState);
        if (eTspRet == E_TSP_OK && eTspState != E_TSP_FLT_STATE_FREE)
        {
            _EMM_FreeFilter();
        }
    }

#if 0 // debugging
    MS_U32          secbuf = 0x0, bufsize = 0x0;
    MS_U32          secid;
    if (secbuf != 0)
    {
        printf("[%s]-[%d] EMM debugging buffer enable\n", __FUNCTION__, __LINE__);
        MDrv_TSP_FLT_Alloc(0, E_TSP_FLT_SRC_TSIF0 | E_TSP_FLT_USER_SEC, &_EmmFlt.flt_id);
//        MDrv_TSP_FLT_SetMode(_EmmFlt.flt_id, E_TSP_FLT_MODE_CONTI);
        MDrv_TSP_SEC_Alloc(0, &secid);
        MDrv_TSP_SEC_SetBuffer(secid, secbuf, bufsize);
        MDrv_TSP_FLT_SelSEC(_EmmFlt.flt_id, secid);
    }
    else
#endif // debugging
    if (E_TSP_OK != MDrv_TSP_FLT_Alloc(0, E_TSP_FLT_SRC_TSIF0 | E_TSP_FLT_USER_EMM, &_EmmFlt.flt_id))
    {
        return FALSE;
    }

#if defined(CHIP_U3)
    if (E_TSP_OK != MDrv_TSP_SecFlt_Alloc(0, &_EmmFlt.buf_id))
    {
        return FALSE;
    }
    MDrv_TSP_PidFlt_SelSecFlt(0, _EmmFlt.flt_id, _EmmFlt.buf_id);
    MDrv_TSP_SecFlt_SetMode(0, _EmmFlt.buf_id, E_DRVTSP_FLT_MODE_CONTI);
    MDrv_TSP_SecFlt_SetBuffer(0, _EmmFlt.buf_id, (_nds_flt_dev.param.ecm_emm_addr + NDS_CAM_ECM_BUFS_SIZE), NDS_CAM_FLT_BUF_SIZE * NDS_CAM_EMM_BUF_NUM);
#endif

    MDrv_TSP_FLT_SetPID(_EmmFlt.flt_id, pid);
    MDrv_TSP_FLT_Enable(_EmmFlt.flt_id, TRUE);

    _EmmFlt.pid = pid;
    _EmmFlt.flt_ctrl = emm_filter_control;
    _EmmFlt.x_connect = x_connection;

    return TRUE;
}


// Enable at EMM SetPid
void _EMM_EnableFilter(MS_BOOL bEnable)
{
    MS_U16  emm_pid;

    if (bEnable)
    {
        emm_pid = _EmmFlt.pid & CAM_EMM_PID_MASK;
        if (_EmmFlt.flt_ctrl & HDICA_EMM_RECEPTION_ENABLE)
        {
            emm_pid = emm_pid | CAM_EMM_PID_ENABLE;
        }
        if (_EmmFlt.flt_ctrl & HDICA_TABLE_ID_FILTERING_USE)
        {
            emm_pid = emm_pid | CAM_EMM_PID_TID_ENABLE;
        }

        CAM_REG(REG_CAM_EMM_PID) = emm_pid;
    }
    else
    {
        // disable EMM filtering
        CAM_REG(REG_CAM_EMM_PID) = _EmmFlt.pid; // Clear [15:14]
    }
}


void _ECM_FreeFilter(MS_U8 ecm_id)
{
    MDrv_TSP_FLT_Enable(_EcmFlt[ecm_id].flt_id, FALSE);
#if defined(CHIP_U3)
    MDrv_TSP_SecFlt_Free(0, _EcmFlt[ecm_id].buf_id);
#endif
    MDrv_TSP_FLT_Free(_EcmFlt[ecm_id].flt_id);

    memset(_u8EcmBuf[ecm_id][0], 0, NDS_CAM_FLT_BUF_SIZE);
    memset(_u8EcmBuf[ecm_id][1], 0, NDS_CAM_FLT_BUF_SIZE);
    memset((void*)MS_PA2KSEG1(_nds_flt_dev.param.ecm_emm_addr + NDS_CAM_FLT_BUF_SIZE*ecm_id), 0, NDS_CAM_FLT_BUF_SIZE);
    MsOS_FlushMemory();

    _EcmFlt[ecm_id].flt_id = NDS_FLT_NULL;
    _EcmFlt[ecm_id].pid = TSP_PID_NULL;
    _EcmFlt[ecm_id].b_defined = FALSE;
}


MS_BOOL _ECM_AllocFilter(MS_U32 x_connection, MS_U8 ecm_filter_num, MS_U16 pid)
{
    if (NDS_FLT_NULL != _EcmFlt[ecm_filter_num].flt_id)
    {
        TSP_FltState        eTspState;
        TSP_Result          eTspRet = E_TSP_OK;
        eTspRet = MDrv_TSP_FLT_GetState(_EcmFlt[ecm_filter_num].flt_id, &eTspState);
        if (eTspState != E_TSP_FLT_STATE_FREE && eTspRet == E_TSP_OK)
        {
            _ECM_FreeFilter(ecm_filter_num);
        }
    }

    if (E_TSP_OK != MDrv_TSP_FLT_Alloc(0, E_TSP_FLT_SRC_TSIF0 | E_TSP_FLT_USER_ECM, &_EcmFlt[ecm_filter_num].flt_id))
    {
        return FALSE;
    }
#if defined(CHIP_U3)
    if (E_TSP_OK != MDrv_TSP_SecFlt_Alloc(0, &_EcmFlt[ecm_filter_num].buf_id))
    {
        return FALSE;
    }
    MDrv_TSP_SecFlt_SetEcmIdx(0, _EcmFlt[ecm_filter_num].buf_id, _EcmFlt[ecm_filter_num].ecm_id);
    MDrv_TSP_PidFlt_SelSecFlt(0, _EcmFlt[ecm_filter_num].flt_id, _EcmFlt[ecm_filter_num].buf_id);
    MDrv_TSP_SecFlt_SetMode(0, _EcmFlt[ecm_filter_num].buf_id, E_DRVTSP_FLT_MODE_ONESHOT | E_DRVTSP_FLT_MODE_CRCCHK);
    MDrv_TSP_SecFlt_SetBuffer(0, _EcmFlt[ecm_filter_num].buf_id, (_nds_flt_dev.param.ecm_emm_addr + (NDS_CAM_FLT_BUF_SIZE*ecm_filter_num)), NDS_CAM_FLT_BUF_SIZE);
#endif

    _EcmFlt[ecm_filter_num].pid = pid;
    _EcmFlt[ecm_filter_num].ecm_id = ecm_filter_num;
    _EcmFlt[ecm_filter_num].x_connect = x_connection;

    return TRUE;
}


// Enable at ECM Define
void _ECM_EnableFilter(MS_U32 x_connection, MS_U8 u8Filter_num, MS_BOOL bEnable, MS_U8 u8Ctrl)
{
    MS_BOOL             bFristSet = FALSE;
    MS_U16              u16Pid = _EcmFlt[u8Filter_num].pid;
    int                 i;

    if (_EcmFlt[u8Filter_num].flt_id != NDS_FLT_NULL)
    {
#if NDS_ECM_SAME_PID_DROP_EN
        if (bEnable)
        {
            _EcmFlt[u8Filter_num].u8Ctrl = u8Ctrl;
            for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
            {
                if (_EcmFlt[i].pid == u16Pid)
                {
                    if (_EcmFlt[i].u8Ctrl > 0 && bFristSet == FALSE)
                    {
                        bFristSet = TRUE;
                        _EcmFlt[i].u8Drop = 0;
                    }
                    else
                    {
                        _EcmFlt[i].u8Drop = 1;
                    }
                }
            }
        }
        else
        {
            _EcmFlt[u8Filter_num].u8Ctrl = 0;
        }
#endif
        if (bEnable)
        {
            // non-NULL PID to enable filter
            MDrv_TSP_FLT_SetPID(_EcmFlt[u8Filter_num].flt_id, _EcmFlt[u8Filter_num].pid);
        }
        else
        {
            MDrv_TSP_FLT_SetPID(_EcmFlt[u8Filter_num].flt_id, TSP_PID_NULL);
        }

        MDrv_TSP_FLT_Enable(_EcmFlt[u8Filter_num].flt_id, bEnable);
    }
}


void _NDS_REC_GetPktCount(MS_U32 x_connection, MS_U32 *hi_count, MS_U32 *lo_count)
{
    *hi_count = 0xFFFFFFFF;
    *lo_count = 0xFFFFFFFF;

    return;
}


#ifdef MSOS_TYPE_LINUX_KERNEL
static irqreturn_t _NDS_FLT_Isr(int irq, void *dev_id)
#else
static void _NDS_FLT_Isr(InterruptNum eIntNum)
#endif
{
    MS_U8               u8IntReg, emm_idx, u8EcmReg;
    MS_U8               ecm_polarity;
    int                 i;

    u8IntReg = CAM_REG(REG_CAM_CA_INT);

    // EMM
    if (u8IntReg & (CAM_CA_INT_EMM_OVERFLOW))
    {
        CAM_REG(REG_CAM_CA_INT) = (CAM_CA_INT_MASK_RESET_MODE | CAM_CA_INT_EMM_OVERFLOW); // clear EMM overflow
        CAM_REG(REG_CAM_CA_INT) = (CAM_CA_INT_ECM | CAM_CA_INT_EMM | CAM_CA_INT_EMM_OVERFLOW); // enable interrupt

#if NDS_EMM_PASSIVE
        if (bEmmOverflow == FALSE)
        {
            bEmmOverflow = TRUE;
        }
#endif

        MsOS_SetEvent(_nds_flt_eventid, NDS_EMM_EVENT_BUFFER | NDS_EMM_EVENT_OVERFLOW_HW);
    }
    else if (u8IntReg & (CAM_CA_INT_EMM))
    {
        // @TODO: add R/W pointer of _u8EmmBuf
        // Make sure no event lost at IPC
        CAM_REG(REG_CAM_CA_INT) = (CAM_CA_INT_ECM | CAM_CA_INT_EMM | CAM_CA_INT_EMM_OVERFLOW);

        // u8EmmReg is Emm index 0~9, 0x1000 is ack.
        emm_idx = CAM_REG(REG_CAM_EMM_INT_STAT) & CAM_EMM_INT_BUF_MASK;

//[NEW] EMM queue management
// it's the only way to handle no-sync index for ACT at overflow

#if NDS_EMM_PASSIVE
        if (bEmmOverflow == FALSE)
        {
#endif
            if (emmQueueIsFull())
            {
#if NDS_EMM_PASSIVE
                bEmmOverflow = TRUE;
#endif
                MsOS_SetEvent(_nds_flt_eventid, NDS_EMM_EVENT_BUFFER | NDS_EMM_EVENT_OVERFLOW_SW);
            }
            else
            {
                MsOS_ReadMemory();
                memcpy(_u8EmmBuf[_u8EmmWrite], (void*)MS_PA2KSEG1(_nds_flt_dev.param.ecm_emm_addr+NDS_CAM_ECM_BUFS_SIZE+(NDS_CAM_FLT_BUF_SIZE*emm_idx)), 188);
                MsOS_SetEvent(_nds_flt_eventid, NDS_EMM_EVENT_BUFFER+emm_idx);
                emmQueueInc();
            }
#if NDS_EMM_PASSIVE
        }
        else
        {
            MsOS_SetEvent(_nds_flt_eventid, NDS_EMM_EVENT_BUFFER);
        }
#endif
// -jerry

        // Ack and Clear EMM interrupt
        CAM_REG(REG_CAM_EMM_INT_STAT) = CAM_EMM_INT_ACK;
    }

    // ECM
    if (u8IntReg & (CAM_CA_INT_ECM))
    {
        u8EcmReg = CAM_REG(REG_CAM_ECM_INT_STAT);

        // bit0~5 to indicate six slot
        for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
        {
            if (u8EcmReg & (0x1 << i))
            {
                MS_U8   *pEcmData;

                CAM_REG(REG_CAM_ECM_INT_STAT) = (0x1 << i);

#if NDS_ECM_SAME_PID_DROP_EN
                if (_EcmFlt[i].u8Drop == 1)
                    continue;
#endif

                pEcmData = (void *)MS_PA2KSEG1(_nds_flt_dev.param.ecm_emm_addr+(NDS_CAM_FLT_BUF_SIZE*i));
                MsOS_ReadMemory();
                if (pEcmData[0] != 0x47)
                {
                    continue; //_NDS_ASSERT_;
                }
                if (pEcmData[5] == 0x80)
                {
                    ecm_polarity = 0; // EVEN
                }
                else if (pEcmData[5] == 0x81)
                {
                    ecm_polarity = 1; // ODD
                }
                else
                {
                    NDS_ASSERT(FALSE, , "[%s]-[%d]\n", __FUNCTION__, __LINE__);
                }

                if (_u8EcmBuf[i][ecm_polarity][0] == 0) // if buffer empty
                {
                    // @TODO: Manual Lock
                    // Check auto/manual mode
                    // Set correspondant setting

                    MsOS_ReadMemory();
                    memcpy(_u8EcmBuf[i][ecm_polarity], pEcmData, 188);

                    // if ECM incomming at this time, the buffer could be dirty
                    if (CAM_REG(REG_CAM_ECM_INT_STAT) & (0x1 << i))
                    {
                        // ECM overfolw
                        MsOS_SetEvent(_nds_flt_eventid, 0x1 << (i + NDS_ECM_EVENT_OVERFLOW_SHFT));
                        continue;
                    }

                    MsOS_SetEvent(_nds_flt_eventid, (0x1 << (i*2 + NDS_ECM_EVENT_FILTER_SHFT)) << ecm_polarity);
                }
                else
                {
                    //_NDS_ASSERT_;
                    // ECM overfolw
                    MsOS_SetEvent(_nds_flt_eventid, 0x1 << (i + NDS_ECM_EVENT_OVERFLOW_SHFT));
                    break;
                }
            }
        }
    }
#ifdef MSOS_TYPE_LINUX_KERNEL
    return IRQ_HANDLED;
#else
    // IRQ handled
    NDS_FLT_OS_EnableInterrupt();
#endif
}


//[NEW]
MS_U32 emm_overflow_time = 0;
// -jerry


static void _NDS_FLT_IsrTask(void)
{
    MS_U32              u32Events, hi_count, lo_count;
    XECM_MSG            ecm_msg;
    XEMM_MSG            emm_msg;
    int                 i, p;

    while(1)
    {
        MsOS_WaitEvent(_nds_flt_eventid, NDS_ALL_EVENT, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        //u8EmmReg = CAM_REG(REG_CAM_EMM_INT_STAT);
        //MsOS_DisableInterrupt(NDS_EMMECM_IRQ);

#if (NDS_EMM_PASSIVE==0)
        if (u32Events & (NDS_EMM_EVENT_OVERFLOW_HW | NDS_EMM_EVENT_OVERFLOW_SW))
        {
            NDS_FLT_LOCK(); // for disable interrupt protection
            if (bEmmOverflow == FALSE)
            {
                _EMM_EnableFilter(FALSE);
                bEmmOverflow = TRUE;
            }
            NDS_FLT_UNLOCK();
        }
#endif

        for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
        {
            if (u32Events & (0x1 << (i + NDS_ECM_EVENT_OVERFLOW_SHFT)))
            {
                printf("\n\nECM_OVERFLOW\n\n");
                //[FIXME] for ecm test , reset buffer at XCORECA_EcmReceived
                //memset(_u8EcmBuf[i][0], 0, 188);
                //memset(_u8EcmBuf[i][1], 0, 188);

                ecm_msg.ecm_buf_ptr = _u8EcmBuf[i][0];
                ecm_msg.rcv_status  = ECM_OVERFLOW;
                _NDS_REC_GetPktCount(_EcmFlt[i].x_connect, &hi_count, &lo_count);

                // if USER return fail, it will release the buffer
                //XCORECA_EcmReceived(0, &ecm_msg , hi_count, lo_count);
                XCORECA_EcmReceived(_EcmFlt[i].x_connect, &ecm_msg , hi_count, lo_count);
                //_NDS_ASSERT_;
            }
            else // if (u32Events & (0x1 << (i + NDS_ECM_EVENT_ODD_SHFT)))
            {
                MS_U8   valid = (u32Events >> (i*2 + NDS_ECM_EVENT_FILTER_SHFT)) & 0x3;
                if (valid) // any ODD/EVEN buffer arrival
                {
                    for (p = 0; p < 2; p++)
                    {
                        if ( (valid & (0x1<<p)) && _u8EcmBuf[i][p][0] == 0x47 && _u8EcmBuf[i][p][5] == (0x80|p)) // 0x80, 0x81
                        {
                            ecm_msg.ecm_filter_num = i;
                            ecm_msg.ecm_buf_ptr    = _u8EcmBuf[i][p]+8;
                            ecm_msg.ecm_polarity   = ((p==0) ? HDICA_EVEN_BUFFER : HDICA_ODD_BUFFER);
                            ecm_msg.rcv_status     = ECM_OK;
                            _NDS_REC_GetPktCount(_EcmFlt[i].x_connect, &hi_count, &lo_count);

                            // if USER return fail, it will release the buffer
                            NDS_FLT_DBG("\n\nECM_RECV %d %d %ld\n\n", i, ecm_msg.ecm_polarity, MsOS_GetSystemTime());
                            if (XCORECA_EcmReceived(_EcmFlt[i].x_connect, &ecm_msg , hi_count, lo_count) != CA_OK)
                            {
                                XHDICA_ReleaseEcmBuffer(_EcmFlt[i].x_connect, ecm_msg.ecm_filter_num, ecm_msg.ecm_polarity, ecm_msg.ecm_buf_ptr);
                            }
                        }
                    }
                }

            }
        }

        // Issue all EMM received
        if (u32Events & NDS_EMM_EVENT_BUFFER)
        {
            NDS_FLT_DBG("[%s][%d] ======> evt r c w s = %08x %02d %02d %02d %02d\n", __FUNCTION__, __LINE__, u32Events, _u8EmmRead, _u8EmmCur, _u8EmmWrite, u32UsedSize);
// #if NDS_EMM_PASSIVE
#if 0
            if (_u8EmmCur != _u8EmmWrite)
#else // NDS_EMM_PASSIVE==0
            MS_U32 u32UsedSize = ((_u8EmmWrite >= _u8EmmCur) ? (_u8EmmWrite - _u8EmmCur) : (MAX_EMM_LIST_NUM - _u8EmmCur + _u8EmmWrite));
            for (; u32UsedSize > 0; u32UsedSize--)
#endif
            // while (!emmQueueIsEmpty())
            {
                if (_u8EmmBuf[_u8EmmCur][0] != 0x47)
                {
                    NDS_FLT_DBG("[%s] %d is NULL\n", __FUNCTION__, _u8EmmCur);
                    goto jump_next_emm;
                    // MY_ASSERT(0);
                }
                emm_msg.emm_buf        = &_u8EmmBuf[_u8EmmCur][8]; // emm payload
                emm_msg.receive_status = EMM_OK;
                NDS_FLT_DBG("\n\nEMM_RECV %ld\n\n", MsOS_GetSystemTime());
                if (XCORECA_EmmReceived( _EmmFlt.x_connect, &emm_msg ) != CA_OK)
                {
                    XHDICA_ReleaseEmmBuffer(_EmmFlt.x_connect, emm_msg.emm_buf);
                }
jump_next_emm:
                _u8EmmCur++;
                if (MAX_EMM_LIST_NUM <= _u8EmmCur)
                {
                    _u8EmmCur = 0;
                }
                //emmQueueDec();
            }
        }

        if ( (u32Events & (NDS_EMM_EVENT_OVERFLOW_HW | NDS_EMM_EVENT_OVERFLOW_SW)) )
        {
            //MDrv_TSP_FLT_Enable(_EmmFlt.flt_id, FALSE);
            //FIXME: remove it            emmQueueReset();

            NDS_FLT_DBG("[%s][%d] ======> evt r c w = %08x %02d %02d %02d\n", __FUNCTION__, __LINE__, u32Events, _u8EmmRead, _u8EmmCur, _u8EmmWrite);

            if (u32Events & NDS_EMM_EVENT_OVERFLOW_HW)
            {
                printf("\n\nEMM_OVERFLOW(HW)\n\n");
                #if defined(CHIP_U3)
                MDrv_TSP_SecFlt_ResetBuffer(0, _EmmFlt.buf_id);
                #endif
            }
            if (u32Events & NDS_EMM_EVENT_OVERFLOW_SW)
            {
                printf("\n\n\nEMM_OVERFLOW(SW)\n\n");
            }
            emm_msg.emm_buf        = NULL;
            emm_msg.receive_status = EMM_OVERFLOW;

            XCORECA_EmmReceived(_EmmFlt.x_connect, &emm_msg);

            emm_overflow_time = MsOS_GetSystemTime();
/*
            for (i = 0; i < NDS_CAM_EMM_BUF_NUM; i++)
            {
                memset(_u8EmmBuf[i], 0, 188); // release all emm buf
            }
*/

//FIXME: enable after all release!!
//            _EnableFilter(TRUE);
        }

        u32Events = 0;

        // enable all interrupt
        // @TODO: add R/W pointer of _u8EmmBuf to make sure no EMM lost.
        // @NOTE: only effective for EMM now
        CAM_REG(REG_CAM_CA_INT) = (CAM_CA_INT_ECM | CAM_CA_INT_EMM | CAM_CA_INT_EMM_OVERFLOW);

        //MsOS_DelayTask(5);
    }
}


NDS_Result NDS_FLT_Init(NDS_FLT_Param *param)
{
    int i;

    if ( ((MS_PHYADDR)NULL == param->ecm_emm_addr) ||
         (param->ecm_emm_size < NDS_CAM_FLT_BUFS_SIZE) )
    {
        return E_NDS_FAIL;
    }

    HAL_NDS_FLT_Init(param);
    _nds_flt_dev.param.ecm_emm_addr = param->ecm_emm_addr;
    _nds_flt_dev.param.pu8ecm_emm_buf = param->pu8ecm_emm_buf;
    _nds_flt_dev.param.ecm_emm_size = param->ecm_emm_size;

    _EmmFlt.pid = TSP_PID_NULL;
    _EmmFlt.flt_id = NDS_FLT_NULL;
    for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
    {
        _EcmFlt[i].pid = TSP_PID_NULL;
        _EcmFlt[i].flt_id = NDS_FLT_NULL;
        _EcmFlt[i].b_defined = FALSE;
#if NDS_ECM_SAME_PID_DROP_EN
        _EcmFlt[i].u8Ctrl = 0;
        _EcmFlt[i].u8Drop = 0;
#endif
    }

    for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
    {
        memset(_u8EcmBuf[i][0], 0, NDS_CAM_FLT_BUF_SIZE);
        memset(_u8EcmBuf[i][1], 0, NDS_CAM_FLT_BUF_SIZE);
    }
    for (i = 0; i < MAX_EMM_LIST_NUM; i++)
    {
        memset(_u8EmmBuf[i], 0, NDS_CAM_FLT_BUF_SIZE);
    }

#if 1
    //*((volatile unsigned int*)( 0xbf202d00 )) = 0x11;// allow TS packet to flow through CA module
    //*((volatile unsigned int*)( 0xbf202d00 )) = 0x59;// 49:pvr, 59: live in+pvr

    //*((volatile unsigned int*)( 0xBF2216CC )) = 0x0080;//olny for FPGA of NSK ignored , set to 0 = disable all
    //*((volatile unsigned int*)( 0xBF2018b0 )) = 0x0080;//accept_en
    //*((volatile unsigned int*)( 0xBF203c50)) = 0x1022;//tsp clk
    *((volatile unsigned int*)( 0xBF221840)) = 0x0010; // Descambler bank 2 enable  reg_latch_even_odd for ca_value
#endif

#if 0 //------ for FPGA TSO ----

    *((volatile MS_U32*)0xbf202d78) = 0x027;//TS0 config only for ts out , 0x021:only video, clk/2 ,,, 0x27:all ts, clk/2
    *((volatile MS_U32*)0xBF202AB0) = 0xbb32; //bypass VPID
#endif

    _nds_flt_eventid = MsOS_CreateEventGroup("NDS_FLT_Event");
    MS_ASSERT(_nds_flt_eventid >= 0);

    _nds_flt_mutexid = MsOS_CreateMutex(E_MSOS_FIFO, "NDS_FLT_Mutex", MSOS_PROCESS_SHARED);
    MS_ASSERT(_nds_flt_mutexid >= 0);

#if defined(MSOS_TYPE_LINUX_KERNEL)
    _nds_flt_taskid = MsOS_CreateTask(  (TaskEntry) _NDS_FLT_IsrTask,
                                        (MS_U32)NULL,
                                        (TaskPriority) (E_TASK_PRI_SYS+1),
                                        TRUE,
                                        NULL,
                                        0,
                                        "NDS_FLT_IsrTask");
#else
    _nds_flt_taskid = MsOS_CreateTask(  (TaskEntry) _NDS_FLT_IsrTask,
                                        (MS_U32)NULL,
                                        (TaskPriority) (E_TASK_PRI_SYS+1),
                                        TRUE,
                                        _nds_flt_stack,
                                        NDS_FLT_TASK_STACK_SIZE,
                                        "NDS_FLT_IsrTask");
#endif
    MS_ASSERT(_nds_flt_taskid >= 0);

    NDS_FLT_OS_AttachInterrupt(_NDS_FLT_Isr);
    NDS_FLT_OS_EnableInterrupt();

    CAM_REG(REG_CAM_ECM_INT_STAT) = CAM_REG(REG_CAM_ECM_INT_STAT);
    CAM_REG(REG_CAM_CA_INT) = CAM_CA_INT_EMM;
    while (CAM_REG(REG_CAM_CA_INT) & CAM_CA_INT_EMM)
    {
        CAM_REG(REG_CAM_EMM_INT_STAT) = CAM_EMM_INT_ACK;
    }
    CAM_REG(REG_CAM_CA_INT) = 0;

    // Enable (unmask) all interrupts
    CAM_REG(REG_CAM_CA_INT) = (CAM_CA_INT_ECM | CAM_CA_INT_EMM | CAM_CA_INT_EMM_OVERFLOW);

    return E_NDS_OK;
}


NDS_Result NDS_FLT_Exit(void)
{
    // Disable (mask) all interrupts
    CAM_REG(REG_CAM_CA_INT) = 0;

    NDS_FLT_OS_DisableInterrupt();
    NDS_FLT_OS_DetachInterrupt();

    MsOS_DeleteTask(_nds_flt_taskid);
    MsOS_DeleteMutex(_nds_flt_mutexid);
    MsOS_DeleteEventGroup(_nds_flt_eventid);

    HAL_NDS_FLT_Exit();

    return E_NDS_OK;
}


// 7.2.1
NDS_STATUS XHDICA_SetEmmPid (NDS_ULONG  x_connection,
                             NDS_USHORT pid,
                             NDS_BYTE   emm_filter_control)
{
    NDS_FUNC("[%s]-[%d] pid[0x%x]\n", __FUNCTION__, __LINE__, pid);

    NDS_FLT_ENTRY(); // for disable interrupt protection

    if ((pid < 2) || (pid > 0x1FFE))
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        NDS_FLT_RETURN(CA_REQUEST_NOT_SUPPORTED_BY_DRIVER);
    }

    _EMM_EnableFilter(FALSE);

#if 0
    if (_EmmFlt.pid != TSP_PID_NULL)
    {
        NDS_ASSERT((_EmmFlt.pid == pid), CA_REQUEST_NOT_SUPPORTED_BY_DRIVER, "[%s]-[%d]\n", __FUNCTION__, __LINE__);
    }
#endif
    // @TODO:
    // should filter and PID be disable and set to NULL for disabling EMM filter?

    // Save pid and allocate fitler
    //_EmmFlt.pid = pid & CAM_EMM_PID_MASK; // Get new pid
    _EMM_AllocFilter(x_connection, pid, emm_filter_control);

    // Enable EMM Filter
    NDS_FLT_OS_DisableInterrupt();
    bEmmOverflow = FALSE;
    NDS_FLT_OS_EnableInterrupt();

    _EMM_EnableFilter(TRUE);

    NDS_FLT_RETURN(CA_OK);
}


// 7.2.2
/*-----------------
source byte --------------- byte 0 ---------------  --------------- byte 1 ---------------  ...    2,
            msb                                lsb  msb                                lsb
source bit    7,   6,   5,   4,   3,   2,   1,   0,  15,  14,  13,  12,  11,  10,   9,   8, ... 25,  24, <= bit index
TID(lsn)        x         x         2         3         4         5         6         7     ...    F     <= tid index

            --------------- byte 0 ---------------  --------------- byte 1 ---------------
TID(lsn)      7    6    5    4    3    2    x    x   15   14   13   12   11   10    9    8               <= tid index
lsb           8   10   12   14    0    2    4    6   24   26   28   30   16   18   20   22               <= new position with old index
msb          10   11   13   15    1    3    5    7   25   27   29   31   17   19   21   23               <= new position with old index
------------------*/
NDS_STATUS XHDICA_EmmSetTableIdFilter (NDS_ULONG    x_connection,
                                       NDS_BYTE     table_id[4])
{
    MS_U32              u32TMode;
    MS_U16              lsb = 0, msb = 0;
    int                 i;

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    CAM_REG(REG_CAM_EMM_TID_MODE_L) = lsb;
    CAM_REG(REG_CAM_EMM_TID_MODE_H) = msb;
    CAM_REG(REG_CAM_EMM_TID) = 0x80; // EMM TID

    u32TMode = (table_id[0]<<24) | (table_id[1]<<16) | (table_id[2]<<8) | (table_id[3]);
    for (i = 2; i <= 0xF; i++) // 0x82-0x8F
    {
        lsb |= ( (u32TMode >> ((0xF-i)*2)     ) & 0x1 ) << i;
        msb |= ( (u32TMode >> (((0xF-i)*2)+1) ) & 0x1 ) << i;
    }

    CAM_REG(REG_CAM_EMM_TID_MODE_L) = lsb;
    CAM_REG(REG_CAM_EMM_TID_MODE_H) = msb;


    return CA_OK;
}


// 7.2.3
/*
    data byte       0x01 0x23 0x45 0x67
    nds reg_mem     0x01 0x23 0x45 0x67
    nds reg_word    0x01234567
    mstar reg_word  0x01234567
    mstar_reg16     0x4567(L) 0x0123(H)
*/
NDS_STATUS XHDICA_SetPrivateEmmFilter (NDS_ULONG    x_connection,
                                       EMM_REG *    emm_reg)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    CAM_REG(REG_CAM_EMM_DATA_ID_H(1)) = *(MS_U8*)(emm_reg->reg1_address+1) + (*(MS_U8*)(emm_reg->reg1_address)<<8);
    CAM_REG(REG_CAM_EMM_DATA_ID_L(1)) = *(MS_U8*)(emm_reg->reg1_address+3) + (*(MS_U8*)(emm_reg->reg1_address+2)<<8);
    CAM_REG(REG_CAM_EMM_MASK_ID_H(1)) = *(MS_U8*)(emm_reg->reg1_mask+1) + (*(MS_U8*)(emm_reg->reg1_mask)<<8);
    CAM_REG(REG_CAM_EMM_MASK_ID_L(1)) = *(MS_U8*)(emm_reg->reg1_mask+3) + (*(MS_U8*)(emm_reg->reg1_mask+2)<<8);
    CAM_REG(REG_CAM_EMM_DATA_ID_H(2)) = *(MS_U8*)(emm_reg->reg2_address+1) + (*(MS_U8*)(emm_reg->reg2_address)<<8);
    CAM_REG(REG_CAM_EMM_DATA_ID_L(2)) = *(MS_U8*)(emm_reg->reg2_address+3) + (*(MS_U8*)(emm_reg->reg2_address+2)<<8);
    CAM_REG(REG_CAM_EMM_MASK_ID_H(2)) = *(MS_U8*)(emm_reg->reg2_mask+1) + (*(MS_U8*)(emm_reg->reg2_mask)<<8);
    CAM_REG(REG_CAM_EMM_MASK_ID_L(2)) = *(MS_U8*)(emm_reg->reg2_mask+3) + (*(MS_U8*)(emm_reg->reg2_mask+2)<<8);
    CAM_REG(REG_CAM_EMM_DATA_ID_H(3)) = *(MS_U8*)(emm_reg->reg3_address+1) + (*(MS_U8*)(emm_reg->reg3_address)<<8);
    CAM_REG(REG_CAM_EMM_DATA_ID_L(3)) = *(MS_U8*)(emm_reg->reg3_address+3) + (*(MS_U8*)(emm_reg->reg3_address+2)<<8);
    CAM_REG(REG_CAM_EMM_MASK_ID_H(3)) = *(MS_U8*)(emm_reg->reg3_mask+1) + (*(MS_U8*)(emm_reg->reg3_mask)<<8);
    CAM_REG(REG_CAM_EMM_MASK_ID_L(3)) = *(MS_U8*)(emm_reg->reg3_mask+3) + (*(MS_U8*)(emm_reg->reg3_mask+2)<<8);

    return CA_OK;
}


// 7.2.5
NDS_STATUS XHDICA_ReleaseEmmBuffer (NDS_ULONG   x_connection,
                                    NDS_BYTE *  emm_buf)
{
    NDS_FUNC("[%s]-[%d]\n", __FUNCTION__, __LINE__);

    NDS_FLT_DBG("free EMM %x\n", emm_buf);

    NDS_FLT_ENTRY(); // for disable interrupt protection

    if (emm_buf != NULL)
    {
        //_verSC_PrintData(emm_buf-8, 32);
//[NEW]
        if (bEmmOverflow == TRUE)
        {
            NDS_FLT_DBG("(read, cur, write) = (%02d %02d %02d)\n", _u8EmmRead, _u8EmmCur, _u8EmmWrite);
            NDS_FLT_DBG("free Emm %d %02x %02x\n", MsOS_GetSystemTime() - emm_overflow_time, *(emm_buf), *(emm_buf-8));
        }
// -jerry

        NDS_FLT_OS_DisableInterrupt();
        *(emm_buf) = 0;
        *(emm_buf-8) = 0;
        emmQueueDec();

//[NEW]
        if ( (bEmmOverflow == TRUE) && (emmQueueIsEmpty()) )
        {
#if (NDS_EMM_PASSIVE==0)
            _EMM_EnableFilter(TRUE);
#endif
            bEmmOverflow = FALSE;
        }
// -jerry
        NDS_FLT_OS_EnableInterrupt();
    }
    else
    {
        NDS_FLT_RETURN(CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW);
    }

//    MsOS_DelayTask(5);

    NDS_FLT_RETURN(CA_OK);
}


// 8.2.1
NDS_STATUS XHDICA_DefineEcmPid (NDS_ULONG   x_connection,
                                NDS_BYTE    ecm_filter_num,
                                NDS_USHORT  pid,
                                NDS_BYTE    odd_ecm_table_id,
                                NDS_BYTE    even_ecm_table_id)
{
    //MS_U16  ecm_pid;
    MS_U16  u16EcmTid = 0;

#if 0
    if(x_connection == 1 )
    {
        NDS_RASP_DefineEcmPid(x_connection,ecm_filter_num,pid,odd_ecm_table_id,even_ecm_table_id);
        return CA_OK ;
    }
#endif


    NDS_FUNC("(%ld)-[%s]-[%d] pid [0x%x]\n", MsOS_GetSystemTime(), __FUNCTION__, __LINE__, pid);

    if (ecm_filter_num >= NDS_CAM_ECM_FLT_NUM)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    // disable filter before setting
    _ECM_EnableFilter(x_connection, ecm_filter_num, FALSE, 0);
    CAM_REG(REG_CAM_ECM_PID(ecm_filter_num)) = pid & CAM_ECM_PID_MASK; // clear [15:13] for later DefineEcmPid

    u16EcmTid |= (even_ecm_table_id & CAM_ECM_TID_EVEN_MASK);
    u16EcmTid |= ((odd_ecm_table_id<<8) & CAM_ECM_TID_ODD_MASK);
    CAM_REG(REG_CAM_ECM_TID) = u16EcmTid;

    if (pid == 0xFFFF)
    {
        // disable the filter
        return CA_OK;
    }

    if ((pid < 0x2 || pid > 0x1FFE))
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }

    // Save pid and allocate fitler
    //_EcmFlt[ecm_filter_num].pid = pid & CAM_ECM_PID_MASK;
    _ECM_AllocFilter(x_connection, ecm_filter_num, pid);

    return CA_OK;
}


//8.2.2
NDS_STATUS XHDICA_DefineEcmFilter (NDS_ULONG    x_connection,
                                   NDS_BYTE     ecm_filter_num,
                                   NDS_BYTE     ecm_filter_control)
{
    MS_U16              ecm_pid = 0;
    int                 i;

    NDS_FUNC("[%s]-[%d] -- ecm_filter_num[%d]\n", __FUNCTION__, __LINE__, ecm_filter_num);

#if 0
    if(x_connection == 1 )
    {
        NDS_RASP_DefineEcmFilter(x_connection,ecm_filter_num,ecm_filter_control);
        return CA_OK ;
    }
#endif


    if (ecm_filter_num >= NDS_CAM_ECM_FLT_NUM)
    {
        NDS_DBG("[%s]-[%d]\n", __FUNCTION__, __LINE__);
        return CA_REQUEST_NOT_SUPPORTED_BY_DRIVER;
    }
    for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++) // 8.2.2 Note
    {
        if ( (i != ecm_filter_num) &&
             (_EcmFlt[i].pid == _EcmFlt[ecm_filter_num].pid) &&
             (_EcmFlt[i].x_connect == _EcmFlt[ecm_filter_num].x_connect) &&
             (_EcmFlt[i].b_defined == TRUE) )
        {
            return CA_ECM_PID_FILTER_ALREADY_DEFINED;
        }
    }

    // @FIXME: is it possible and return CA_OK?
    if ((_EcmFlt[ecm_filter_num].pid < 0x2 || _EcmFlt[ecm_filter_num].pid > 0x1FFE))
    {
        // Invaliad PID
        return CA_OK;
    }

    ecm_pid = _EcmFlt[ecm_filter_num].pid;
    if (ecm_filter_control & HDICA_ECM_FILTER_CONTROL_ENABLE_ODD_RECEPTION)
    {
        ecm_pid |= CAM_ECM_PID_TID_ODD;
    }
    if (ecm_filter_control & HDICA_ECM_FILTER_CONTROL_ENABLE_EVEN_RECEPTION)
    {
        ecm_pid |= CAM_ECM_PID_TID_EVEN;
    }
    if (ecm_filter_control & HDICA_ECM_ENABLE_RECEPTION)
    {
        ecm_pid |= CAM_ECM_PID_ENABLE;
        _EcmFlt[ecm_filter_num].b_defined = TRUE;
    }
    else
    {
        _EcmFlt[ecm_filter_num].b_defined = FALSE;
    }

    // Enable
    // NOTE: It's always AUTO for current NDS implementation
    CAM_REG(REG_CAM_ECM_PID(ecm_filter_num)) = 0; // disable filter & reset LOCK
    CAM_REG(REG_CAM_ECM_LOCK(ecm_filter_num)) |=  CAM_ECM_LOCK_AUTO(ecm_filter_num);
    CAM_REG(REG_CAM_ECM_PID(ecm_filter_num)) = ecm_pid;

    _ECM_EnableFilter(x_connection, ecm_filter_num, (ecm_filter_control & HDICA_ECM_ENABLE_RECEPTION) ? TRUE : FALSE, ecm_filter_control);

    return CA_OK;
}


// 8.2.4
NDS_STATUS XHDICA_ReleaseEcmBuffer (NDS_ULONG   x_connection,
                                    NDS_BYTE    ecm_filter_num,
                                    NDS_BYTE    ecm_polarity,
                                    NDS_BYTE *  ecm_buf)
{
    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    NDS_FLT_DBG("free ECM %d %d\n", ecm_filter_num, ecm_polarity);

    NDS_FLT_ENTRY(); // for disable interrupt protection

    if (ecm_buf != NULL)
    {
        NDS_FLT_OS_DisableInterrupt();
        memset(ecm_buf-8, 0, 188);
        NDS_FLT_OS_EnableInterrupt();

        // @TODO: Manual UnLock
        // Check auto/manual mode
        // Set correspondant setting
    }
    else
    {
        NDS_FLT_RETURN(CA_DRIVER_CAN_NOT_PERFORM_FUNCTION_NOW);
    }

    NDS_FLT_RETURN(CA_OK);
}


// 8.2.5
NDS_STATUS XHDICA_ReleaseAllEcmBuffers (NDS_ULONG   x_connection)
{
    int                 i = 0;

    NDS_FUNC("[%s]-[%d] \n", __FUNCTION__, __LINE__);

    NDS_FLT_ENTRY(); // for disable interrupt protection

    for (i = 0; i < NDS_CAM_ECM_FLT_NUM; i++)
    {
        NDS_FLT_OS_DisableInterrupt();
        memset(_u8EcmBuf[i][0], 0, 188);
        memset(_u8EcmBuf[i][1], 0, 188);
        NDS_FLT_OS_EnableInterrupt();
    }

    NDS_FLT_RETURN(CA_OK);
}

