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
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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
/// file    drvTSO.c
/// @brief  TS I/O Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
/// All TSO DDI are not allowed to use in any interrupt context other than TSO ISR and Callback
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"

// Internal Definition
#include "asmCPU.h"
#include "regTSIO.h"
#include "drvTSIO.h"
#include "halTSIO.h"


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DYNAMIC_VOLTAGE 0

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define TSIO_MUTEX_WAIT_TIME                            0xFFFFFFFFUL
#define TSIO_LOCDEC_SETKEY_TIMEOUT                      0x00000014UL //20 ms
#define TSIO_STACK_SIZE                                 0x1000

#define TSIO_DBGERR(fmt,args...)        {if(_tsiodbglevel >= E_DRVTSIO_DBG_ERR)             \
                                        printf("[ERROR][%s][%d]" fmt "\n" ,__FUNCTION__,__LINE__,##args);}
#define TSIO_DBGINFO(fmt,args...)       {if(_tsiodbglevel >= E_DRVTSIO_DBG_INFO)            \
                                        printf("[%s][%d]" fmt "\n" ,__FUNCTION__,__LINE__,##args);}
#define TSIO_DBGTRACE(fmt,args...)      {if(_tsiodbglevel >= E_DRVTSIO_DBG_TRACE)           \
                                        printf("[%s][%d]" fmt "\n" ,__FUNCTION__,__LINE__,##args);}

#define _TSIO_ENTRY()           if (!MsOS_ObtainMutex(_ptsio_res->_s32TSIOMutexId, TSIO_MUTEX_WAIT_TIME))   \
                                {                                                                           \
                                    return DRVTSIO_FAIL;                                                    \
                                }

#define _TSIO_RETURN(_ret)      do{                                                                         \
                                    MsOS_ReleaseMutex(_ptsio_res->_s32TSIOMutexId);                         \
                                    return _ret;                                                            \
                                }while(0);

#define TSIO_CB(Type, Value, ext1, ext2)    do{                                                     \
                                                if (_TSIO_CbFn != NULL)                             \
                                                {                                                   \
                                                    _TSIO_CbFn(Type, Value, ext1, ext2);            \
                                                }                                                   \
                                            }while(0);


#define TSIO_TASK_EVENTS                0xFFFFFFFF
#define TSIO_EVENT(type)                (0x1 << type)
#define TSIO_EVENT_GET(event,type)      (event & (0x1 << type))
#define TSIO_SGDMAIN_INT_SET(sid)       _u64SgdmainInt |= ((MS_U64)0x1 << sid);
#define TSIO_SGDMAIN_INT_GET(sid)       ((_u64SgdmainInt >> sid) & 0x1)
#define TSIO_SGDMAIN_INT_CLR(sid)       (_u64SgdmainInt &=~((MS_U64)0x1 << sid))
#define TSIO_SGDMAOUT_INT_SET(sid, type)      _u64SgdmaoutInt[type-1] |= ((MS_U64)0x1 << sid);
#define TSIO_SGDMAOUT_INT_GET(sid, type)      ((_u64SgdmaoutInt[type-1] >> sid) & 0x1)
#define TSIO_SGDMAOUT_INT_CLR(sid, type)      (_u64SgdmaoutInt[type-1] &=~((MS_U64)0x1 << sid))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

typedef enum
{
    E_TSIO_SERVICESTATE_FREE           = 0x00000000UL,
    E_TSIO_SERVICESTATE_ALLOC          = 0x00000001UL,
    E_TSIO_SERVICESTATE_ENABLE         = 0x00000002UL,
    E_TSIO_SERVICESTATE_NA             = 0xFFFFFFFFUL,
}TSIO_SERVICESTATE;

typedef struct
{
    TSIO_SERVICESTATE eState;
    MS_U8 u8Sid;
    MS_BOOL btoTSP; //1: TSP, 0: SGDMA_out
    MS_BOOL bDecrypt;
    MS_BOOL bkeyEvenSet;
    MS_U8 u8keyEven[TSIO_LOCKEY_LEN];
    MS_BOOL bkeyOddSet;
    MS_U8 u8keyOdd[TSIO_LOCKEY_LEN];
}TSIO_SERVICE;

typedef struct
{
    TSIO_SERVICESTATE eState;
    MS_U16 u16FltPid;
    MS_U8 u8Sid;
}TSIO_SGDMAIN_PIDFLT;

typedef struct _TSIO_RESOURCE_PRIVATE
{
    MS_S32          _s32TSIOMutexId;
    MS_S32          _s32EventId;
    MS_S32          _s32TSIOISRTaskId;
    MS_BOOL         _bTSIOISRTaskEn;
    MS_S32          _s32TSIOMonitorTaskId;
    MS_BOOL         _bTSIOMonitorTaskEn;
    MS_BOOL         _bAnalogphaseEn; // for analog phase issue
    MS_U8           _u8Analogphase; // for analog phase issue
    MS_BOOL         _bBittrainingEn; //for bittraining
    MS_U8           _u8offset; //for bittraining
    HAL_TSIO_CLK    _eClk;
    MS_PHY          _phyLastpktBufAddr; // 192 bytes (pkt size) x 64 (service num)
    MS_U32          _u32LastpktBufSize;
    MS_PHY          _phyDMAVQBufAddr;
    MS_U32          _u32DMAVQBufSize;
    MS_PHY          _phyDMAinNodeAddr; // 1 node need 16 bytes.
    MS_U32          _u32DMAinNodeSize;
    MS_PHY          _phyDMAoutNodeAddr; // 1 node need 16 bytes.
    MS_U32          _u32DMAoutNodeSize;
    TSIO_SERVICE    _Servic[TSIO_SERVICE_NUM];
    TSIO_SGDMAIN_PIDFLT _PidFlt[TSIO_SGDMAIN_PIDFLT_NUM];
    MS_BOOL         _bDMAoutLSB[TSIO_SERVICE_NUM];
    MS_BOOL         _bSWInit;
}TSIO_RESOURCE_PRIVATE;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static TSIO_RESOURCE_PRIVATE*    _ptsio_res = NULL;
static DRVTSIO_SGDMA_NODE* SGDMAIN_NODE = NULL;
static DRVTSIO_SGDMA_NODE* SGDMAOUT_NODE = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static DRVTSIO_DBGLEVEL         _tsiodbglevel = E_DRVTSIO_DBG_ERR;
static MS_BOOL                  _bBankInit = FALSE;
#ifndef MSOS_TYPE_LINUX_KERNEL
static MS_U32                   _TSIO_ISR_Stack[TSIO_STACK_SIZE/sizeof(MS_U32)];
#endif
static MS_U32                   _TSIO_Monitor_Stack[TSIO_STACK_SIZE/sizeof(MS_U32)];
static MS_BOOL                  _CC_RX_INT = FALSE;
static MS_BOOL                  _CC_TX_INT = FALSE;
static MS_U16                   _CC_RX_ERROR_STATUS = 0;
static TSIOIntCB                _TSIO_CbFn = NULL;
static MS_BOOL                  _bInvSelect = 0;
static MS_U64                   _u64SgdmainInt = 0;
static MS_U64                   _u64SgdmaoutInt[4] = {0};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _TSIO_InitResource(TSIO_RESOURCE_PRIVATE* preSource)
{
    MS_U16 u16ii;

    if(preSource == NULL)
    {
        return FALSE;
    }

    for (u16ii = 0; u16ii < TSIO_SERVICE_NUM; u16ii++)
    {
        preSource->_Servic[u16ii].eState = E_TSIO_SERVICESTATE_FREE;
        preSource->_Servic[u16ii].bDecrypt = FALSE;
        preSource->_Servic[u16ii].bkeyEvenSet = FALSE;
        preSource->_Servic[u16ii].bkeyOddSet = FALSE;
        preSource->_Servic[u16ii].u8Sid = 0xFF;
    }

    for (u16ii = 0; u16ii < TSIO_SGDMAIN_PIDFLT_NUM; u16ii++)
    {
        preSource->_PidFlt[u16ii].eState = E_TSIO_SERVICESTATE_FREE;
        preSource->_PidFlt[u16ii].u16FltPid = TSIO_PID_NULL;
        preSource->_PidFlt[u16ii].u8Sid = 0xFF;
        HAL_TSIO_SGDMAIN_Flt_SetPid(u16ii, TSIO_PID_NULL);
    }

    preSource->_s32TSIOMutexId = -1;
    preSource->_s32TSIOISRTaskId = -1;
    preSource->_s32TSIOMonitorTaskId = -1;
    preSource->_s32EventId = -1;
    preSource->_bTSIOISRTaskEn = FALSE;
    preSource->_bTSIOMonitorTaskEn = FALSE;
    preSource->_bSWInit = FALSE;
    preSource->_bAnalogphaseEn = FALSE; // for analog phase issue
    preSource->_u8Analogphase = 0; // for analog phase issue
    preSource->_bBittrainingEn = FALSE;
    preSource->_u8offset = 0;
    preSource->_eClk = HAL_TSIO_CLK_200M;

    return TRUE;
}

TSIO_Result MDrv_TSIO_Wait_IoSignal(DRVTSIO_IOSIGNAL * stIosig)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U32 u32Event = 0;
    MS_S32 s32Ret = 0;
    MS_U32 index = 0;

    if (stIosig == NULL)
        return DRVTSIO_INVALID_PARAM;

    memset(stIosig, '\0', sizeof(DRVTSIO_IOSIGNAL));

    if((s32Ret = MsOS_WaitEvent_Interrupt(_ptsio_res->_s32EventId, TSIO_TASK_EVENTS, &u32Event, E_OR_CLEAR, MSOS_WAIT_FOREVER)) <= 0)
    {
        if(s32Ret < 0) // -ERESTARTSYS
        {
            return DRVTSIO_FUNC_ERROR;
        }
        else
        {
            return DRVTSIO_FAIL;
        }
    }

    stIosig->u32Event = u32Event;

    if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_LASTNODE))
    {
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_LASTNODE))
            {
                TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_LASTNODE);
                stIosig->u64DmaoutInt[0] |= ((MS_U64)0x1 << index);
            }
        }
    }

    if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_NODEINT))
    {
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_NODEINT))
            {
                TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_NODEINT);
                 stIosig->u64DmaoutInt[1] |= ((MS_U64)0x1 << index);
            }
        }
    }

    if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_DROP))
    {
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_DROP))
            {
                TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_DROP);
                 stIosig->u64DmaoutInt[2] |= ((MS_U64)0x1 << index);
            }
        }
    }

    if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_DMAEND))
    {
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_DMAEND))
            {
                TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_DMAEND);
                 stIosig->u64DmaoutInt[3] |= ((MS_U64)0x1 << index);
            }
        }
    }

    if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAIN_DONE))
    {
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            if (TSIO_SGDMAIN_INT_GET(index))
            {
                TSIO_SGDMAIN_INT_CLR(index);
                 stIosig->u64DmainInt |= ((MS_U64)0x1 << index);
            }
        }
    }

    TSIO_DBGINFO("stIosig->u32Event = 0x%08X \n", stIosig->u32Event);
    TSIO_DBGINFO("stIosig->u64DmainInt_0-31 = 0x%08X \n", (MS_U32)(stIosig->u64DmainInt & 0xFFFFFFFF));
    TSIO_DBGINFO("stIosig->u64DmainInt_31-63 = 0x%08X \n", (MS_U32)((stIosig->u64DmainInt & 0xFFFFFFFF00000000) >> 32));
#endif
    return DRVTSIO_OK;
}

#ifndef MSOS_TYPE_LINUX_KERNEL
static void _TSIO_ISR_Task(void)
{
    MS_U32 u32Event;
    MS_U32 index = 0;
    _ptsio_res->_bTSIOISRTaskEn = TRUE;
    while (_ptsio_res->_bTSIOISRTaskEn)
    {
        MsOS_WaitEvent(_ptsio_res->_s32EventId, TSIO_TASK_EVENTS, &u32Event, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        TSIO_DBGINFO("u32Event = 0x%08X \n", u32Event);

        if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_LASTNODE))
        {
            for (index = 0; index < TSIO_SERVICE_NUM; index ++)
            {
                if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_LASTNODE))
                {
                    TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_LASTNODE);
                    TSIO_CB(E_TSIO_SGDMAOUT_LASTNODE, index, 0, NULL);
                }
            }
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_NODEINT))
        {
            for (index = 0; index < TSIO_SERVICE_NUM; index ++)
            {
                if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_NODEINT))
                {
                    TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_NODEINT);
                    TSIO_CB(E_TSIO_SGDMAOUT_NODEINT, index, NULL, NULL);
                }
            }
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_DROP))
        {
            for (index = 0; index < TSIO_SERVICE_NUM; index ++)
            {
                if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_DROP))
                {
                    TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_DROP);
                    TSIO_CB(E_TSIO_SGDMAOUT_DROP, index, NULL, NULL);
                }
            }
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAOUT_DMAEND))
        {
            for (index = 0; index < TSIO_SERVICE_NUM; index ++)
            {
                if (TSIO_SGDMAOUT_INT_GET(index, E_TSIO_SGDMAOUT_DMAEND))
                {
                    TSIO_SGDMAOUT_INT_CLR(index, E_TSIO_SGDMAOUT_DMAEND);
                    TSIO_CB(E_TSIO_SGDMAOUT_DMAEND, index, 0, NULL);
                }
            }
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_SGDMAIN_DONE))
        {
            for (index = 0; index < TSIO_SERVICE_NUM; index ++)
            {
                if (TSIO_SGDMAIN_INT_GET(index))
                {
                    TSIO_SGDMAIN_INT_CLR(index);
                    TSIO_CB(E_TSIO_SGDMAIN_DONE, index, NULL, NULL);
                }
            }
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_CC_TX_DONE))
        {
            TSIO_CB(E_TSIO_CC_TX_DONE, 0, NULL, NULL);
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_CC_RX_DONE))
        {
            TSIO_CB(E_TSIO_CC_RX_DONE, 0, NULL, NULL);
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_LIVE_TSO_OVERFLOW))
        {
            TSIO_CB(E_TSIO_LIVE_TSO_OVERFLOW, 0, NULL, NULL);
        }

        if (TSIO_EVENT_GET(u32Event, E_TSIO_OPERATION_START))
        {
            TSIO_CB(E_TSIO_OPERATION_START, 0, NULL, NULL);
        }
    }
}
#endif

static void _TSIO_Monitor(void)
{
    MS_U32 u32Data = 0;
    MS_U8 u8Gap = 0;//ANALOG_PHASE_GAP;
    MS_BOOL bEarly = 0, bLate = 0;
#if (DYNAMIC_VOLTAGE == 1)
    static MS_U8 u8Voltage = 1; //1.8v
    static MS_BOOL bAdd = TRUE;
    static MS_U16 u16Count = 0;
#endif
    //for analog phase issue...
    _ptsio_res->_bTSIOMonitorTaskEn = TRUE;
    while (_ptsio_res->_bTSIOMonitorTaskEn)
    {
        if (_ptsio_res->_bAnalogphaseEn) //dynamic adjust analog phase
        {
            if (HAL_TSIO_Analogphase_Read(1))
            {
                bEarly = 1;
            }
            else
            {
                bEarly = 0;
            }
            if (HAL_TSIO_Analogphase_Read(0))
            {
                bLate = 1;
            }
            else
            {
                bLate = 0;
            }

            TSIO_DBGTRACE("Phase = %d, Early = 0x%X, late = 0x%X", _ptsio_res->_u8Analogphase, bEarly, bLate);
#if 0
            if (bEarly || bLate)
            {
                TSIO_DBGINFO("Phase = %d, Early = 0x%X, late = 0x%X", _ptsio_res->_u8Analogphase, bEarly, bLate);
                TSIO_DBGINFO("PHASE CHANGE!!!!!!!!!!");
                HAL_TSIO_Bittraining_Setvalue(bEarly, bLate);
            }
#else
            if (bEarly)
            {
                TSIO_DBGTRACE("[Early] now Phase = %d", _ptsio_res->_u8Analogphase);
                _ptsio_res->_u8Analogphase++;
                if (_ptsio_res->_u8Analogphase > ANALOG_PHASE_MAX)
                {
                    _ptsio_res->_u8Analogphase = ANALOG_PHASE_MIN;
                }
                HAL_TSIO_Analogphase_Set(_bInvSelect, _ptsio_res->_u8Analogphase, ANALOG_PHASE_GAP);
                _bInvSelect = !_bInvSelect;
            }
            else if (bLate)
            {
                TSIO_DBGTRACE("[Late] now Phase = %d", _ptsio_res->_u8Analogphase);
                if (_ptsio_res->_u8Analogphase == ANALOG_PHASE_MIN)
                {
                    _ptsio_res->_u8Analogphase = ANALOG_PHASE_MAX;
                }
                else
                {
                    _ptsio_res->_u8Analogphase--;
                }
                HAL_TSIO_Analogphase_Set(_bInvSelect, _ptsio_res->_u8Analogphase, ANALOG_PHASE_GAP);
                _bInvSelect = !_bInvSelect;
            }
#endif
        }
        else if (_ptsio_res->_bBittrainingEn)
        {
            if(HAL_TSIO_Bittraining_Getremapstate())
            {

                u32Data = HAL_TSIO_DBG_Read(0x0D55);
                TSIO_DBGTRACE("remap state");
                TSIO_DBGTRACE("0x0D55 = 0x%X",u32Data);
                u32Data = HAL_TSIO_DBG_Read(0x0D45);
                TSIO_DBGTRACE("0x0D45 = 0x%X",u32Data);

                if (u32Data & D45_PH_INCR)
                {
                    if (_ptsio_res->_u8Analogphase == ANALOG_PHASE_MAX)
                        _ptsio_res->_u8Analogphase = ANALOG_PHASE_MIN;
                    else
                        _ptsio_res->_u8Analogphase++;
                    HAL_TSIO_Analogphase_Set(_bInvSelect, _ptsio_res->_u8Analogphase, u8Gap);
                    _bInvSelect = !_bInvSelect;
                }
                else if (u32Data & D45_PH_DECR)
                {
                    if (_ptsio_res->_u8Analogphase == ANALOG_PHASE_MIN)
                        _ptsio_res->_u8Analogphase = ANALOG_PHASE_MAX;
                    else
                        _ptsio_res->_u8Analogphase--;

                    HAL_TSIO_Analogphase_Set(_bInvSelect, _ptsio_res->_u8Analogphase, u8Gap);
                    _bInvSelect = !_bInvSelect;
                }
                else if (u32Data & D45_GUARD_PH_SMALL)
                {
                    HAL_TSIO_Analogphase_Set(_bInvSelect, _ptsio_res->_u8Analogphase, --u8Gap);
                    _bInvSelect = !_bInvSelect;
                }
                else if (u32Data & D45_GUARD_PH_LARGE)
                {
                    HAL_TSIO_Analogphase_Set(_bInvSelect, _ptsio_res->_u8Analogphase, ++u8Gap);
                    _bInvSelect = !_bInvSelect;
                }
                TSIO_DBGTRACE("Select = %d, phase = 0x%X, Gap = %d",_bInvSelect, _ptsio_res->_u8Analogphase, u8Gap);

                if (HAL_TSIO_Analogphase_Read(1))
                {
                    bEarly = 1;
                }
                else
                {
                    bEarly = 0;
                }
                if (HAL_TSIO_Analogphase_Read(0))
                {
                    bLate = 1;
                }
                else
                {
                    bLate = 0;
                }
#if (DYNAMIC_VOLTAGE == 1)
                if (HAL_TSIO_Analogphase_GetPuhstatus() == 7)
                {
                    if (u16Count++ >= 10)
                    {
                        if (bAdd)
                        {
                            u8Voltage++;
                            if (u8Voltage == 3)
                            {
                                bAdd = FALSE;
                            }
                        }
                        else
                        {
                            u8Voltage--;
                            if (u8Voltage == 0)
                            {
                                bAdd = TRUE;
                            }
                        }
                        HAL_TSIO_STRLD_SET(u8Voltage);
                        TSIO_DBGTRACE("==================");
                        TSIO_DBGTRACE("Change Voltage");
                        TSIO_DBGTRACE("==================\n\n");
                        u16Count = 0;
                    }
                    TSIO_DBGTRACE("==================");
                    TSIO_DBGTRACE("Voltage = %.02f v",(float)(u8Voltage*0.05 + 1.75));
                    TSIO_DBGTRACE("==================\n\n");
                }
#endif
                HAL_TSIO_Bittraining_Setvalue(bEarly, bLate);

                TSIO_DBGTRACE("==================");
                TSIO_DBGTRACE("power state = 0x%04X", HAL_TSIO_Analogphase_GetPuhstatus());
                TSIO_DBGTRACE("dbg(D45) = 0x%08X", HAL_TSIO_DBG_Read(0xD45));
                TSIO_DBGTRACE("phase = %d", HAL_TSIO_Bittraining_GetPhase(0));
                TSIO_DBGTRACE("early = %d", HAL_TSIO_Bittraining_GetPhase(1));
                TSIO_DBGTRACE("late = %d", HAL_TSIO_Bittraining_GetPhase(2));
                TSIO_DBGTRACE("==== early's el info=====");
                HAL_TSIO_Bittraining_GetElinfo(1);
                TSIO_DBGTRACE("==== late's el info=====");
                HAL_TSIO_Bittraining_GetElinfo(0);
                TSIO_DBGTRACE("==== HW phase info=====");
                HAL_TSIO_Bittraining_GetPhaseinfo();
                TSIO_DBGTRACE("Prbs Errcnt = 0x%04X",HAL_TSIO_Bittraining_CheckPrbsErrcnt());
                TSIO_DBGTRACE("==================\n\n");
            }
        }
        //printf("0x%08X\n",HAL_TSIO_DBG_Read(0x0D21));
        MsOS_DelayTask(20);
    }
}

static MS_BOOL _TSIO_Sgdmaoutinterrupt(MS_U8 u8Chid, E_TSIO_INT_TYPE *Type)
{
    MS_U16 u16Status = 0;

    if (Type == NULL)
    {
        return FALSE;
    }

    HAL_TSIO_SGDMAOUT_IntRead(u8Chid, &u16Status);

    if (u16Status == 0)
    {
        *Type = E_TSIO_INT_NONE;
        return FALSE;
    }
    else if (u16Status & TSIO2_SGDMA_OUT_VC_STATUS_DMAEND)
    {
        *Type = E_TSIO_SGDMAOUT_DMAEND;
        return TRUE;
    }
    else if (u16Status & TSIO2_SGDMA_OUT_VC_STATUS_DROP)
    {
        *Type = E_TSIO_SGDMAOUT_DROP;
        return TRUE;
    }
    else if (u16Status & TSIO2_SGDMA_OUT_VC_STATUS_NODEINT)
    {
        *Type = E_TSIO_SGDMAOUT_NODEINT;
        return TRUE;
    }
    else if (u16Status & TSIO2_SGDMA_OUT_VC_STATUS_LASTNODE)
    {
        *Type = E_TSIO_SGDMAOUT_LASTNODE;
        return TRUE;
    }

    TSIO_DBGINFO("SGDMA out interrupt status error, Chid = %d  Status = 0x%X \n", u8Chid, u16Status);
    return TRUE;
}

static void _TSIO_Interrupt_CB_TSIO(InterruptNum eIntNum)
{
    MS_U8 index = 0;
    E_TSIO_INT_TYPE Type;

    TSIO_DBGTRACE("E_INT_IRQ_TSIO_INT");
    if (HAL_TSIO_SGDMAOUT_IntAll())
    {
        TSIO_DBGTRACE("TSIO_SGDMAOUT_INT_EVENT");
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            if (_TSIO_Sgdmaoutinterrupt(index, &Type))
            {
                TSIO_DBGINFO("idx = %d Type = %d",index, Type);
                HAL_TSIO_SGDMAOUT_IntClr(index);
                break;
            }
        }
        if (index < TSIO_SERVICE_NUM)
        {
            //TSIO_CB(Type, index, NULL, NULL); //for nos test, should be move to ISR_TASK
            TSIO_SGDMAOUT_INT_SET(index, Type);
            if (Type == E_TSIO_SGDMAOUT_NODEINT)
            {
                _ptsio_res->_bDMAoutLSB[index] = !_ptsio_res->_bDMAoutLSB[index];
            }
            MsOS_SetEvent(_ptsio_res->_s32EventId, TSIO_EVENT(Type));
        }
    }
    else if(HAL_TSIO_CC_TxInt())
    {
        TSIO_DBGTRACE("TSIO_CC_TX_INT_EVENT");
        //TSIO_CB(E_TSIO_CC_TX_DONE, 0, NULL, NULL); //for nos test, should be move to ISR_TASK
        MsOS_SetEvent(_ptsio_res->_s32EventId, TSIO_EVENT(E_TSIO_CC_TX_DONE));
        _CC_TX_INT = TRUE;
        HAL_TSIO_CC_TxIntClr();
    }
    else if (HAL_TSIO_CC_RxInt())
    {
        TSIO_DBGTRACE("TSIO_CC_RX_INT_EVENT");
        _CC_RX_INT = TRUE;
        _CC_RX_ERROR_STATUS = HAL_TSIO_CC_RxErrStatus();
        if (_CC_RX_ERROR_STATUS)
        {
            HAL_TSIO_CC_RxClr();
            HAL_TSIO_CC_RxEnable(FALSE);
        }
        //TSIO_CB(E_TSIO_CC_RX_DONE, _CC_RX_ERROR_STATUS, NULL, NULL); //for nos test, should be move to ISR_TASK
        MsOS_SetEvent(_ptsio_res->_s32EventId, TSIO_EVENT(E_TSIO_CC_RX_DONE));
        HAL_TSIO_CC_RxIntClr();
    }
    else if (HAL_TSIO_Int_Bittraining_Read())
    {
        //TBD.
        HAL_TSIO_Int_Bittraining_Clear();
    }
    else if (HAL_TSIO_Int_Operation_Read())
    {
        HAL_TSIO_Int_Operation_Clear();
        //TSIO_CB(E_TSIO_OPERATION_START, 0, NULL, NULL); //for nos test, should be move to ISR_TASK
        MsOS_SetEvent(_ptsio_res->_s32EventId, TSIO_EVENT(E_TSIO_OPERATION_START));
    }
    MsOS_EnableInterrupt(E_INT_IRQ_TSIO_INT);
}

static void _TSIO_Interrupt_CB_TSO(InterruptNum eIntNum)
{
    MS_U8 index = 0;
    MS_U16 u16Status = 0;

    TSIO_DBGTRACE("E_INT_IRQ_TSP_TSO0");
    if (HAL_TSIO_SGDMAIN_IntAll())
    {
        for (index = 0; index < TSIO_SERVICE_NUM; index ++)
        {
            HAL_TSIO_SGDMAIN_IntRead(index, &u16Status);
            if (u16Status)
            {
                TSIO_DBGINFO("idx = %d u16Status = %d",index, u16Status);
                HAL_TSIO_SGDMAIN_IntClr(index);
                break;
            }
        }
        if (index < TSIO_SERVICE_NUM)
        {
            //TSIO_CB(E_TSIO_SGDMAIN_DONE, index, NULL, NULL); //for nos test, should be move to ISR_TASK
            TSIO_SGDMAIN_INT_SET(index);
            MsOS_SetEvent(_ptsio_res->_s32EventId, TSIO_EVENT(E_TSIO_SGDMAIN_DONE));
        }
    }
    else
    {
        /*
        extern MS_U16 HAL_TSO_HWInt_Status(MS_U8 u8Eng);
        extern void HAL_TSO_HWInt_Clear(MS_U8 u8Eng, MS_U16 u16Int);
        extern void HAL_TSO_HWInt_Enable(MS_U8 u8Eng, MS_BOOL benable, MS_U16 u16init);
        extern MS_BOOL HAL_TSO_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16 u16Cfg, MS_BOOL bEnable);
        MS_U16 u16TsoIntStatus = 0;
        for(index = 0; index < 2; index++)
        index = 0;
        {
            u16TsoIntStatus = HAL_TSO_HWInt_Status(index);
            if(u16TsoIntStatus)
            {
                HAL_TSO_ChIf_Cfg(index, 1, 0x8000, TRUE);
                HAL_TSO_ChIf_Cfg(index, 1, 0x8000, FALSE);
                HAL_TSO_ChIf_Cfg(index, 2, 0x8000, TRUE);
                HAL_TSO_ChIf_Cfg(index, 2, 0x8000, FALSE);
                HAL_TSO_ChIf_Cfg(index, 3, 0x8000, TRUE);
                HAL_TSO_ChIf_Cfg(index, 3, 0x8000, FALSE);
                HAL_TSO_ChIf_Cfg(index, 4, 0x8000, TRUE);
                HAL_TSO_ChIf_Cfg(index, 4, 0x8000, FALSE);
                HAL_TSO_HWInt_Clear(index, u16TsoIntStatus);
                TSIO_CB(E_TSIO_LIVE_TSO_OVERFLOW, (u16TsoIntStatus >> 8), NULL, NULL); //for nos test, should be move to ISR_TASK
                HAL_TSO_HWInt_Enable(index, TRUE, 0xFF);
            }
        }
        */
    }

    MsOS_EnableInterrupt(E_INT_IRQ_TSP_TSO0);
}

static MS_BOOL _TSIO_Interrupt_Init(void)
{
    MsOS_AttachInterrupt(E_INT_IRQ_TSIO_INT, _TSIO_Interrupt_CB_TSIO);
    MsOS_EnableInterrupt(E_INT_IRQ_TSIO_INT);
    MsOS_AttachInterrupt(E_INT_IRQ_TSP_TSO0, _TSIO_Interrupt_CB_TSO);
    MsOS_EnableInterrupt(E_INT_IRQ_TSP_TSO0);
    return TRUE;
}

static MS_BOOL _TSIO_Interrupt_DeInit(void)
{
    MsOS_DisableInterrupt(E_INT_IRQ_TSIO_INT);
    MsOS_DetachInterrupt(E_INT_IRQ_TSIO_INT);
    MsOS_DisableInterrupt(E_INT_IRQ_TSP_TSO0);
    MsOS_DetachInterrupt(E_INT_IRQ_TSP_TSO0);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_InitLibResource(void* pResMemAddr)
{
    MS_VIRT virtBank;
    MS_PHY u32BankSize;

    TSIO_DBGTRACE();

    if(pResMemAddr == NULL)
        return DRVTSIO_FAIL;

    _ptsio_res = (TSIO_RESOURCE_PRIVATE*)pResMemAddr;

    // For multi-process use case. (different process should set the value of bank again)
    if(_bBankInit == FALSE)
    {
        if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_TSO)) //MODULE_TSIO = ??
        {
            TSIO_DBGERR("MDrv_TSIO_Init failed");
            return DRVTSIO_FAIL;
        }
        TSIO_DBGINFO("TSIO Base Bank 0x%08X", (unsigned int)virtBank);
        HAL_TSIO_SetBank(virtBank);
        _bBankInit = TRUE;
    }

    if(_ptsio_res->_bSWInit != TRUE)
    {
        if(_TSIO_InitResource(_ptsio_res) == FALSE)
            return DRVTSIO_FAIL;
    }

    return DRVTSIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSIO Hardware capability
/// @param  eCap                                 \b IN: Capability item to get
/// @param  pOutput                             \b IN: Pointer to store getting result
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_GetCap(DRVTSIO_CAP eCap, void* pOutput)
{
    *((MS_U32*)pOutput) = 0;

    switch (eCap)
    {
        case DRVTSIO_CAP_RESOURCE_SIZE:
            *((MS_U32*)pOutput) = (MS_U32)sizeof(TSIO_RESOURCE_PRIVATE);
            break;

        default:
            return DRVTSIO_INVALID_PARAM;
    }

    return DRVTSIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Initialize TSIO driver
/// @return TSIO_Result
/// @note
/// It should be called before calling any other TSIO functions.
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Init(void)
{
    MS_VIRT virtBank;
    MS_PHY u32BankSize;

    TSIO_DBGTRACE();

    if (_ptsio_res->_bSWInit)
    {
        TSIO_DBGINFO("MDrv_TSIO_Init already initialize");
        return DRVTSIO_OK;
    }
    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &u32BankSize, MS_MODULE_TSO)) //MODULE_TSIO = ??
    {
        TSIO_DBGERR("MDrv_TSIO_Init failed");
        return DRVTSIO_FAIL;
    }
    TSIO_DBGINFO("TSIO Base Bank 0x%08X", (unsigned int)virtBank);
    HAL_TSIO_SetBank(virtBank);

    _ptsio_res->_s32TSIOMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex TSIO", MSOS_PROCESS_SHARED);
    if(_ptsio_res->_s32TSIOMutexId == -1)
    {
        TSIO_DBGERR("MsOS_CreateMutex failed");
        return DRVTSIO_FAIL;
    }

    _ptsio_res->_s32EventId = MsOS_CreateEventGroup("TSIO_EVENT");
    if (_ptsio_res->_s32EventId == -1)
    {
        TSIO_DBGERR("MsOS_CreateEventGroup failed");
    }

#ifndef MSOS_TYPE_LINUX_KERNEL
    _ptsio_res->_s32TSIOISRTaskId = MsOS_CreateTask((TaskEntry)_TSIO_ISR_Task,
                                                    (MS_U32)NULL,
                                                    E_TASK_PRI_SYS,
                                                    TRUE,
                                                    _TSIO_ISR_Stack,
                                                    TSIO_STACK_SIZE,
                                                    (char *)"TSIO_ISR_TASK");
    if (_ptsio_res->_s32TSIOISRTaskId == -1)
    {
        TSIO_DBGERR("MsOS_CreateTask failed");
    }
#endif
    _ptsio_res->_s32TSIOMonitorTaskId = MsOS_CreateTask((TaskEntry)_TSIO_Monitor,
                                                    (MS_U32)NULL,
                                                    E_TASK_PRI_MEDIUM,
                                                    TRUE,
                                                    _TSIO_Monitor_Stack,
                                                    TSIO_STACK_SIZE,
                                                    (char *)"TSIO_Monitor");
    if (_ptsio_res->_s32TSIOMonitorTaskId == -1)
    {
        TSIO_DBGERR("MsOS_CreateTask failed");
    }

    _TSIO_ENTRY();

    HAL_TSIO_ClkOpen(1);
    HAL_TSIO_Reset();
    _ptsio_res->_bSWInit = TRUE;
    _TSIO_Interrupt_Init();

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Open TSIO
/// @param eClk                         \b IN: clk speed
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Open(DRVTSIO_CLK eClk)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    _ptsio_res->_eClk = (HAL_TSIO_CLK)eClk;
    HAL_TSIO_Analogphase_Init(_ptsio_res->_eClk,0);
#if (BITTRAINING_SW_MODE_ENABLE == 1)
    //bit training SW patch initial
    HAL_TSIO_Bittraining_init();
#endif
    HAL_TSIO_PowerInit();
    MsOS_DelayTask(3); // for bittraining init
    HAL_TSIO_SGDMAIN_Reset();
    HAL_TSIO_SGDMAOUT_Init();

    HAL_TSIO_STRLD_SET(3); //for tornado borad, reset voltage have to 1.9v

    //reset vqbuffer & lastnode buffer
    HAL_TSIO_SGDMAIN_LastPktBuf_Set(_ptsio_res->_phyLastpktBufAddr, _ptsio_res->_phyLastpktBufAddr+_ptsio_res->_u32LastpktBufSize);
    HAL_TSIO_SGDMAIN_SGVQBuf_Set(_ptsio_res->_phyDMAVQBufAddr, _ptsio_res->_u32DMAVQBufSize);

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Exit TSIO driver
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Exit(void)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    _TSIO_Interrupt_DeInit();
    HAL_TSIO_ClkOpen(0);

    MsOS_ReleaseMutex(_ptsio_res->_s32TSIOMutexId);
    MsOS_DeleteMutex(_ptsio_res->_s32TSIOMutexId);


    if (_ptsio_res->_s32EventId != -1)
        MsOS_DeleteEventGroup(_ptsio_res->_s32EventId);

    _ptsio_res->_bTSIOISRTaskEn = FALSE;
    _ptsio_res->_bTSIOMonitorTaskEn = FALSE;
    _ptsio_res->_s32TSIOMutexId = -1;
    _ptsio_res->_s32EventId = -1;
    if (_ptsio_res->_s32TSIOISRTaskId != -1)
        MsOS_DeleteTask(_ptsio_res->_s32TSIOISRTaskId);
    if (_ptsio_res->_s32TSIOMonitorTaskId != -1)
        MsOS_DeleteTask(_ptsio_res->_s32TSIOMonitorTaskId);
    _ptsio_res->_s32TSIOISRTaskId = -1;
    _ptsio_res->_s32TSIOMonitorTaskId = -1;
    _ptsio_res->_bSWInit = FALSE;
    _ptsio_res->_bAnalogphaseEn = FALSE;
    _ptsio_res->_bBittrainingEn = FALSE;
    _ptsio_res->_u8offset = 0;
    _ptsio_res->_u8Analogphase = 0;
    _ptsio_res->_eClk = HAL_TSIO_CLK_200M;
    return DRVTSIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Close TSIO
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Close(void)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    HAL_TSIO_Analogphase_Exit();

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Search and setting Analog phase
/// @param u8offset                         \b IN: shift phase
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Analogphase_adj_bittraining(MS_U8 u8offset)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();
#if (BITTRAINING_SW_MODE_ENABLE == 1)
    _ptsio_res->_bBittrainingEn = TRUE;//!_ptsio_res->_bBittrainingEn;
    _ptsio_res->_u8offset = u8offset;
#endif
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Search and setting Analog phase
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Analogphase_adj(void)
{
    MS_BOOL bGoodPhase[ANALOG_PHASE_NUM] = {0};
    MS_U8 u8index = 0;
    MS_U16 status = 0;
    MS_U32 u32Timeout = 0;
    MS_U8 gp_start = 0xFF, gp_max_start = 0;
    MS_U8 gp_end = 0, gp_max_end = 0;
    MS_U8 gp_count = 0, gp_max_count = 0;
    MS_U8 selectphase = 0;
    MS_U16 u16early = 0, u16late = 0;
    MS_BOOL bFixSel = FALSE;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    //wait training state
    do
    {
        status = HAL_TSIO_Analogphase_GetPuhstatus();
        if (status == 5)
        {
            break;
        }
        MsOS_DelayTask(10);
        if (u32Timeout++ > 1000)
        {
            TSIO_DBGERR("wait training status timeout!!!!!");
            _TSIO_RETURN(DRVTSIO_FAIL);
        }
    }while(1);

    do
    {
        for (u8index = ANALOG_PHASE_MIN; u8index <= ANALOG_PHASE_MAX; u8index++)
        {
            HAL_TSIO_Analogphase_Set(_bInvSelect, u8index, ANALOG_PHASE_GAP);
            MsOS_DelayTask(10);
            u16early = HAL_TSIO_Analogphase_Read(1);
            u16late = HAL_TSIO_Analogphase_Read(0);
            TSIO_DBGINFO("Early = 0x%X, late = 0x%X", u16early, u16late);
            if (u16early || u16late)
            {
                bGoodPhase[u8index] = FALSE;
            }
            else
            {
                bGoodPhase[u8index] = TRUE;
            }
            TSIO_DBGINFO("bGoodPhase = %d ", bGoodPhase[u8index]);
            if (!bFixSel)
            {
                _bInvSelect = !_bInvSelect;
            }
        }

        gp_start = 0xFF;
        gp_max_start = 0;
        gp_end = 0;
        gp_max_end = 0;
        gp_count = 0;
        gp_max_count = 0;
        selectphase = 0;

        for (u8index = ANALOG_PHASE_MIN; u8index <= ANALOG_PHASE_MAX; u8index++)
        {
            if (bGoodPhase[u8index] )
            {
                if (gp_start == 0xFF)
                {
                    gp_end = 0xFF;
                    gp_start = u8index;
                }
                gp_count++;

                if (u8index == ANALOG_PHASE_MAX) //last phase
                {
                    gp_end = u8index;
                    if (gp_count > gp_max_count)
                    {
                        gp_max_start = gp_start;
                        gp_max_end = gp_end;
                        gp_max_count = gp_count;
                    }
                    TSIO_DBGINFO("normal: %d %d %d", gp_start, gp_end, gp_count);
                    gp_start = 0xFF;
                    gp_count = 0;
                }
            }
            else if (!bGoodPhase[u8index] && (gp_end == 0xFF))
            {
                gp_end = u8index;
                if (gp_count > gp_max_count)
                {
                    gp_max_start = gp_start;
                    gp_max_end = gp_end;
                    gp_max_count = gp_count;
                }
                TSIO_DBGINFO("normal: %d %d %d", gp_start, gp_end, gp_count);
                gp_start = 0xFF;
                gp_count = 0;
            }
        }
        TSIO_DBGINFO("max: %d %d %d", gp_max_start, gp_max_end, gp_max_count);

        selectphase = (gp_max_start + gp_max_end)/2;
        TSIO_DBGINFO("selectphase: %d", selectphase);
        if (gp_max_count <=1)
        {
            HAL_TSIO_Analogphase_Pllreset();
            bFixSel = TRUE;
            _bInvSelect = !_bInvSelect;
        }
    }while(0);//(gp_max_count <=1);

    HAL_TSIO_Analogphase_Set(_bInvSelect, selectphase, ANALOG_PHASE_GAP);
    _bInvSelect = !_bInvSelect;
    HAL_TSIO_Analogphase_Start();
    _ptsio_res->_u8Analogphase = selectphase;
    _ptsio_res->_bAnalogphaseEn = TRUE;

    _TSIO_RETURN(DRVTSIO_OK);
}


//-------------------------------------------------------------------------------------------------
/// Set debug level of TSIO
/// @param  DbgLevel                \b IN: Debug Level Value
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SetDbgLevel(DRVTSIO_DBGLEVEL DbgLevel)
{
    EN_HAL_TSIO_DBGMSG_LEVEL halLevel = E_HAL_TSIO_DBG_LEVEL_NONE;

    TSIO_DBGTRACE();

    _tsiodbglevel = DbgLevel;
    TSIO_DBGINFO("Debug level : %d", _tsiodbglevel);

    switch (DbgLevel)
    {
        case E_DRVTSIO_DBG_Release:
            halLevel = E_HAL_TSIO_DBG_LEVEL_NONE;
            break;

        case E_DRVTSIO_DBG_ERR:
            halLevel = E_HAL_TSIO_DBG_LEVEL_ERR;
            break;

        case E_DRVTSIO_DBG_INFO:
            halLevel = E_HAL_TSIO_DBG_LEVEL_INFO;
            break;

        default:
        case E_DRVTSIO_DBG_TRACE:
            halLevel = E_HAL_TSIO_DBG_LEVEL_TRACE;
            break;
    }

    HAL_TSIO_DbgLevel(halLevel);
    return DRVTSIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Reset TSIO driver
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Reset(void)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    HAL_TSIO_Reset();
    MsOS_DelayTask(500); //delay for analog phase issue
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable bypass SMC setting on TSIO module
/// @param u8Mode                  \b IN: 0: disable, 1: direct8, 2: direct 16, 3: loopback , bypass SMC setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_BypassSMC_Set(MS_U8 u8Mode)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    switch (u8Mode)
    {
        case 0:
        default:
            HAL_TSIO_Tx2Rx_InsertCTS(FALSE);
            HAL_TSIO_Tx2Rx_Direct8(FALSE);
            HAL_TSIO_Tx2Rx_Direct16(FALSE);
            HAL_TSIO_Tx2Rx_Loopback(FALSE);
            break;

        case 1:
            HAL_TSIO_Tx2Rx_InsertCTS(TRUE);
            HAL_TSIO_Tx2Rx_Direct8(TRUE);
            HAL_TSIO_Tx2Rx_Direct16(FALSE);
            HAL_TSIO_Tx2Rx_Loopback(FALSE);
            break;

        case 2:
            HAL_TSIO_Tx2Rx_InsertCTS(TRUE);
            HAL_TSIO_Tx2Rx_Direct8(FALSE);
            HAL_TSIO_Tx2Rx_Direct16(TRUE);
            HAL_TSIO_Tx2Rx_Loopback(FALSE);
            break;

        case 3:
            HAL_TSIO_Tx2Rx_InsertCTS(TRUE);
            HAL_TSIO_Tx2Rx_Direct8(FALSE);
            HAL_TSIO_Tx2Rx_Direct16(FALSE);
            HAL_TSIO_Tx2Rx_Loopback(TRUE);
            break;
    }
   _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Stuffing ServiceID to TSIO module
/// @param u32StuffingSID           \b IN: stuffing service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_StuffingSID_Set(MS_U32 u32StuffingSID)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if (u32StuffingSID >= TSIO_SERVICE_NUM)
    {
        TSIO_DBGERR("Invalid stuffing id, it should be less than %d",TSIO_SERVICE_NUM);
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    HAL_TSIO_StuffingSID_Set(u32StuffingSID);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable Loc decrypt engine on TSIO module
/// @param bEnable           \b IN: enable/disable loc decrypt engine
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_LocDecrypt_Set(MS_BOOL bEnable)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    HAL_TSIO_Decrypt_Set(bEnable);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Sync threshold value to TSIO module
/// @param u8Value           \b IN: threshold value
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Syncthreshold_Set(MS_U8 u8Value)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if (u8Value > TSIO_MAX_SYNCTHRESHOLD || u8Value < TSIO_MIN_SYNCTHRESHOLD)
    {
        TSIO_DBGERR("Invalid sync threshold value, it should be %d to %d",TSIO_MIN_SYNCTHRESHOLD, TSIO_MAX_SYNCTHRESHOLD);
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    HAL_TSIO_Threshold_Set(u8Value);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable Bypass packet merge on TSIO module
/// @param bEnable           \b IN: enable/disable bypass packet merge
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_BypassMerge_Set(MS_BOOL bEnable)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    HAL_TSIO_BypassMerge_Set(bEnable);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Reset SGDMAIN
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_Reset(void)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    HAL_TSIO_SGDMAIN_Reset();
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set SGDMAIN Last packet buffer
/// @param u32LastpktBufAddr                          \b IN: Last packet buffer physic address
/// @param u32LastpktBufSize                           \b IN: Last packet buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_LastPktBuf_Set(MS_U32 u32LastpktBufAddr, MS_U32 u32LastpktBufSize)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    _ptsio_res->_phyLastpktBufAddr = u32LastpktBufAddr;
    _ptsio_res->_u32LastpktBufSize = u32LastpktBufSize;
    HAL_TSIO_SGDMAIN_LastPktBuf_Set(u32LastpktBufAddr, u32LastpktBufAddr+u32LastpktBufSize);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set SGDMAIN VQ buffer
/// @param u32VQBufAddr                          \b IN: SGDMAIN VQ buffer physic address
/// @param u32VQBufSize                           \b IN: SGDMAIN VQ buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_SGVQBuf_Set(MS_U32 u32VQBufAddr, MS_U32 u32VQBufSize)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    _ptsio_res->_phyDMAVQBufAddr = u32VQBufAddr;
    _ptsio_res->_u32DMAVQBufSize = u32VQBufSize;
    HAL_TSIO_SGDMAIN_SGVQBuf_Set(u32VQBufAddr, u32VQBufSize);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set SGDMAIN Node buffer
/// @param u32NodeAddr                          \b IN: SGDMAIN Node buffer physic address
/// @param u32NodeSize                           \b IN: SGDMAIN Node buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_NodeBuf_Set(MS_U32 u32NodeAddr, MS_U32 u32NodeSize)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    _ptsio_res->_phyDMAinNodeAddr = u32NodeAddr;
    _ptsio_res->_u32DMAinNodeSize = u32NodeSize;
    SGDMAIN_NODE = (DRVTSIO_SGDMA_NODE *)MsOS_PA2KSEG1(u32NodeAddr);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set SGDMAOUT Node buffer
/// @param u32NodeAddr                          \b IN: SGDMAOUT Node buffer physic address
/// @param u32NodeSize                           \b IN: SGDMAOUT Node buffer size
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAOUT_NodeBuf_Set(MS_U32 u32NodeAddr, MS_U32 u32NodeSize)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    _ptsio_res->_phyDMAoutNodeAddr = u32NodeAddr;
    _ptsio_res->_u32DMAoutNodeSize = u32NodeSize;
    SGDMAOUT_NODE = (DRVTSIO_SGDMA_NODE *)MsOS_PA2KSEG1(u32NodeAddr);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO service
/// @param  pu16Servicehandle                         \b OUT: pointer of service handle for return
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_Alloc(MS_U16 *pu16Servicehandle)
{
    MS_U16 u16ii;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if (pu16Servicehandle == NULL)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    *pu16Servicehandle = 0xFFFF;
    for (u16ii = 0; u16ii < TSIO_SERVICE_NUM; u16ii++)
    {
        if (_ptsio_res->_Servic[u16ii].eState == E_TSIO_SERVICESTATE_FREE)
        {
            TSIO_DBGINFO("Allocate service handle %d", u16ii);
            _ptsio_res->_Servic[u16ii].eState = E_TSIO_SERVICESTATE_ALLOC;
            *pu16Servicehandle = u16ii;
            _TSIO_RETURN(DRVTSIO_OK)
        }
    }

    TSIO_DBGERR("service handle is full, max number is %d",TSIO_SERVICE_NUM);
    _TSIO_RETURN(DRVTSIO_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Free TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_Free(MS_U16 u16Servicehandle)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState == E_TSIO_SERVICESTATE_FREE)
    {
        _TSIO_RETURN(DRVTSIO_OK);
    }

    HAL_TSIO_SGDMAIN_VC_Close((MS_U8)u16Servicehandle);
    HAL_TSIO_SVIDTBL_Set((MS_U8)u16Servicehandle, TSIO_SERVICE_NUM-1, 0, 0);
    HAL_TSIO_SGDMAOUT_VC_Close((MS_U8)u16Servicehandle);
    HAL_TSIO_SGDMAIN_IntClr((MS_U8)u16Servicehandle);
    HAL_TSIO_SGDMAOUT_IntClr((MS_U8)u16Servicehandle);
    HAL_TSIO_SGDMAIN_PidFltEnable(_ptsio_res->_Servic[u16Servicehandle].u8Sid, FALSE);

    //close loc key
    HAL_TSIO_LOCDEC_Xiu_Lock(0);
    HAL_TSIO_LOCDEC_KeyEnable(_ptsio_res->_Servic[u16Servicehandle].u8Sid, FALSE);
    HAL_TSIO_LOCDEC_PrivilegeSlotEnable(_ptsio_res->_Servic[u16Servicehandle].u8Sid, FALSE);
    HAL_TSIO_LOCDEC_Xiu_Unlock();

    //reset resource parameters
    _ptsio_res->_Servic[u16Servicehandle].eState = E_TSIO_SERVICESTATE_FREE;
    _ptsio_res->_Servic[u16Servicehandle].bDecrypt = FALSE;
    _ptsio_res->_Servic[u16Servicehandle].bkeyEvenSet = FALSE;
    _ptsio_res->_Servic[u16Servicehandle].bkeyOddSet = FALSE;
    _ptsio_res->_Servic[u16Servicehandle].u8Sid = 0xFF;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Service ID to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param u8Sid                                            \b IN: Service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetSid(MS_U16 u16Servicehandle, MS_U8 u8Sid)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(u8Sid >= TSIO_SERVICE_NUM)
    {
        TSIO_DBGERR("invalid service ID, the max service ID is %d", TSIO_SERVICE_NUM-1);
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    _ptsio_res->_Servic[u16Servicehandle].u8Sid = u8Sid;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get Service ID with target Service handle
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param u8Sid                                            \b OUT: Service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_GetSid(MS_U16 u16Servicehandle, MS_U8 *u8Sid)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(u8Sid ==  NULL)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    *u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set DMAIN Virture channel to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param eSet                                             \b IN: Sgdma in param setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetDMAinVC(MS_U16 u16Servicehandle, DRVTSIO_DMAIN_SET *peSet)
{
    MS_U8 u8Sid = 0xFF; ;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].u8Sid == 0xFF)
    {
        TSIO_DBGERR("invalid service id");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }
    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;

    SGDMAIN_NODE[u16Servicehandle].u32Bufferaddr = peSet->phyBufAddr;
    SGDMAIN_NODE[u16Servicehandle].u32Buffersize = peSet->u32BufSize;
    SGDMAIN_NODE[u16Servicehandle].u32Endnodeflag = TRUE;
    SGDMAIN_NODE[u16Servicehandle].u32Nextnodeaddr = 0;

    MsOS_FlushMemory(); // Node flush

    HAL_TSIO_SGDMAIN_VC_Open(MsOS_VA2PA((MS_VIRT)&SGDMAIN_NODE[u16Servicehandle]), (MS_U8)u16Servicehandle,
        u8Sid, peSet->bTSmode, peSet->bMidPriority, peSet->bLastNodeFlush, peSet->bNewPkt);

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// start DMAIN Virture channel
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param u32Pacing                                     \b IN: Sgdma in pacing rate
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_DMAinVC_Start(MS_U16 u16Servicehandle, MS_U32 u32Pacing)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].u8Sid == 0xFF)
    {
        TSIO_DBGERR("invalid service id");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    if (u32Pacing > 0)
    {
        HAL_TSIO_SGDMAIN_Global_Pacing();
        HAL_TSIO_SGDMAIN_VC_Pacing((MS_U8)u16Servicehandle, TRUE, u32Pacing);
    }

    HAL_TSIO_SGDMAIN_Start();
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set DMAOUT VC to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetDMAoutVC(MS_U16 u16Servicehandle, DRVTSIO_DMAOUT_SET* peSet)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    TSIO_DBGINFO("0x%X (0x%X)",(MS_U32)&SGDMAOUT_NODE[u16Servicehandle], (MS_U32)MsOS_VA2PA((MS_VIRT)&SGDMAOUT_NODE[u16Servicehandle]));
    TSIO_DBGINFO("eSet.phyBufAddr = 0x%X",(MS_U32)peSet->phyBufAddr);
    TSIO_DBGINFO("eSet.u32BufSize = 0x%X",peSet->u32BufSize);

    SGDMAOUT_NODE[u16Servicehandle].u32Bufferaddr = (MS_U32)peSet->phyBufAddr;
    SGDMAOUT_NODE[u16Servicehandle].u32Buffersize = peSet->u32BufSize;
    SGDMAOUT_NODE[u16Servicehandle].u32Endnodeflag = 2; //ring buffer
    SGDMAOUT_NODE[u16Servicehandle].u32Nextnodeaddr = MsOS_VA2PA((MS_VIRT)&SGDMAOUT_NODE[u16Servicehandle]); //ring buffer

    MsOS_FlushMemory(); // Node flush

    HAL_TSIO_SGDMAOUT_VC_Open(MsOS_VA2PA((MS_VIRT)&SGDMAOUT_NODE[u16Servicehandle]), (MS_U8)u16Servicehandle, peSet->bSecurity);
    HAL_TSIO_SGDMAOUT_Start();
    _ptsio_res->_bDMAoutLSB[u16Servicehandle] = 0;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Get DMAOUT VC  write address
/// @param u16Servicehandle                          \b IN: Service handle ID
/// @param pphyWrite                                    \b OUT: dma out write address
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_DMAout_WriteGet(MS_U16 u16Servicehandle, MS_PHY* pphyWrite)
{
    MS_U32 u32Addr = 0;
    MS_U32 u32Len = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if (pphyWrite == NULL)
    {
        TSIO_DBGERR("invalid input");
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u32Addr = HAL_TSIO_SGDMAOUT_ReadSram(u16Servicehandle, HAL_SGDMA_SRAM_BUF_ADDR, _ptsio_res->_bDMAoutLSB[u16Servicehandle]);
    u32Len = HAL_TSIO_SGDMAOUT_ReadSram(u16Servicehandle, HAL_SGDMA_SRAM_BUF_USED_SIZE, _ptsio_res->_bDMAoutLSB[u16Servicehandle]);

    *pphyWrite = u32Addr + u32Len;

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set destination to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param eSet                                            \b IN: Sgdma out param setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetDestination(MS_U16 u16Servicehandle, DRVTSIO_SERVICEDEST eDest)
{
    MS_BOOL bLive = FALSE;
    MS_U8 u8Sid = 0xFF;
    MS_U8 u8SouceID = (MS_U8)u16Servicehandle;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].u8Sid == 0xFF)
    {
        TSIO_DBGERR("invalid service id");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;
    if (eDest == E_DRVTSIO_SERVICEDEST_LIVE)
    {
        u8SouceID = 0x47;
        bLive = TRUE;
    }

    HAL_TSIO_SVIDTBL_Set((MS_U8)u16Servicehandle, u8Sid, u8SouceID, bLive);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable bypass SMC merge setting to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                  \b IN: enable/disable bypass SMC setting
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_BypassSMCMergeEnable(MS_U16 u16Servicehandle, MS_BOOL bEnable)
{
    MS_U8 u8Sid = 0xFF;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;

    HAL_TSIO_SIDBypassSMCMergeEnable(u8Sid, bEnable);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Locdecrypt key to TSIO service
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param pKey                                            \b IN: Pointer to store loc decrypt key address
/// @param u16KeySize                                   \b IN: Decrypt key size
/// @param bOddKey                                       \b IN: 1: odd key, 0: even key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetLocdecKey(MS_U16 u16Servicehandle, MS_U8 *pKey, MS_U16 u16KeySize, MS_BOOL bOddKey)
{
    MS_U32 u32Timeout = 0;
    MS_U8 u8Sid = 0xFF;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if (pKey == NULL || u16KeySize != TSIO_LOCDEC_KEY_LEN)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;

    HAL_TSIO_LOCDEC_Xiu_Lock(0);
    HAL_TSIO_LOCDEC_ScbFixRule(TRUE);
    HAL_TSIO_LOCDEC_Key_Set(u8Sid, pKey, u16KeySize, bOddKey);

    while (!HAL_TSIO_LOCDEC_KeySetDone())
    {
        MsOS_DelayTask(1);
        if (u32Timeout >= TSIO_LOCDEC_SETKEY_TIMEOUT)
        {
            HAL_TSIO_LOCDEC_Xiu_Unlock();
            _TSIO_RETURN(DRVTSIO_FAIL);
        }
    }
    HAL_TSIO_LOCDEC_Xiu_Unlock();
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable/disable locdec key
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable locdec key
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_LocdecKeyEnable(MS_U16 u16Servicehandle, MS_BOOL bEnable)
{
    MS_U8 u8Sid = 0xFF;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;

    HAL_TSIO_LOCDEC_Xiu_Lock(0);
    HAL_TSIO_LOCDEC_KeyEnable(u8Sid, bEnable);
    HAL_TSIO_LOCDEC_Xiu_Unlock();
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable secure keyslot on Locdecrypt
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable / disable secure keyslot
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetLocdecSecureKeyslot(MS_U16 u16Servicehandle, MS_BOOL bEnable)
{
    MS_U8 u8Sid = 0xFF;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;

    HAL_TSIO_LOCDEC_Xiu_Lock(0);
    HAL_TSIO_LOCDEC_SecureSlotEnable(u8Sid, bEnable);
    HAL_TSIO_LOCDEC_Xiu_Unlock();
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable privilege keyslot on Locdecrypt
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable / disable secure flag
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SetLocdecPrivilegeKeySlot(MS_U16 u16Servicehandle, MS_BOOL bEnable)
{
    MS_U8 u8Sid = 0xFF;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;

    HAL_TSIO_LOCDEC_Xiu_Lock(0);
    HAL_TSIO_LOCDEC_PrivilegeSlotEnable(u8Sid, bEnable);
    HAL_TSIO_LOCDEC_Xiu_Unlock();
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Enable SGDMAIN PidFlt
/// @param u16Servicehandle                           \b IN: Service handle ID
/// @param bEnable                                        \b IN: Enable
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_Service_SgdmaInPidFltEnable(MS_U16 u16Servicehandle, MS_BOOL bEnable)
{
    MS_U8 u8Sid = 0xFF;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_Servic[u16Servicehandle].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        TSIO_DBGERR("unknow service handle");
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    u8Sid = _ptsio_res->_Servic[u16Servicehandle].u8Sid;
    HAL_TSIO_SGDMAIN_PidFltEnable(u8Sid, bEnable);

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Allocate TSIO SGDMA_in PID filter
/// @param  pu16PidFltId                         \b OUT: pointer of PID filter ID for return
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_Alloc(MS_U16 *pu16PidFltId)
{
    MS_U16 u16ii;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if (pu16PidFltId == NULL)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    *pu16PidFltId = 0xFFFF;

    for(u16ii = 0; u16ii < TSIO_SGDMAIN_PIDFLT_NUM; u16ii++)
    {
        if(_ptsio_res->_PidFlt[u16ii].eState == E_TSIO_SERVICESTATE_FREE)
        {
            TSIO_DBGTRACE("Allocate PID Flter %d", u16ii);
            _ptsio_res->_PidFlt[u16ii].eState = E_TSIO_SERVICESTATE_ALLOC;
            *pu16PidFltId = u16ii;
            _TSIO_RETURN(DRVTSIO_OK);
        }
    }
    _TSIO_RETURN(DRVTSIO_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Free TSIO SGDMA_in PID filter
/// @param u16PidFltId                           \b IN: PID filter ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_Free(MS_U16 u16PidFltId)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_PidFlt[u16PidFltId].eState == E_TSIO_SERVICESTATE_FREE)
    {
        _TSIO_RETURN(DRVTSIO_OK);
    }

    HAL_TSIO_SGDMAIN_Flt_SetPid(u16PidFltId, TSIO_PID_NULL);
    HAL_TSIO_SGDMAIN_Flt_SetSid(u16PidFltId, TSIO_SERVICE_NULL);

    //reset resource parameters
    _ptsio_res->_PidFlt[u16PidFltId].u16FltPid = TSIO_PID_NULL;
    _ptsio_res->_PidFlt[u16PidFltId].u8Sid = TSIO_SERVICE_NULL;
    _ptsio_res->_PidFlt[u16PidFltId].eState = E_TSIO_SERVICESTATE_FREE;

    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set PID to SGDMA_in PID filter
/// @param  u16PidFltId                       \b IN: PID filter ID
/// @param  u16Pid                             \b IN: PID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_SetPid(MS_U16 u16PidFltId, MS_U16 u16Pid)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_PidFlt[u16PidFltId].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    HAL_TSIO_SGDMAIN_Flt_SetPid(u16PidFltId, u16Pid);

    _ptsio_res->_PidFlt[u16PidFltId].u16FltPid = u16Pid;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Set Service ID to PID filter
/// @param  u16PidFltId                         \b IN: PID filter ID
/// @param  u8Sid                                \b IN: Service ID
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_SGDMAIN_PidFlt_SetSid(MS_U16 u16PidFltId, MS_U8 u8Sid)
{
    TSIO_DBGTRACE();
    _TSIO_ENTRY();

    if(_ptsio_res->_PidFlt[u16PidFltId].eState != E_TSIO_SERVICESTATE_ALLOC)
    {
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    HAL_TSIO_SGDMAIN_Flt_SetSid(u16PidFltId, u8Sid);
    _ptsio_res->_PidFlt[u16PidFltId].u8Sid = u8Sid;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Send C&C cmd
/// @param pu8Cmd                                        \b IN: Pointer to store C&C cmd address
/// @param u16CmdLen                                   \b IN: C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_SendCmd(MS_U8 *pu8Cmd, MS_U16 u16CmdLen)
{
    MS_U16 u16ii = 0;
    MS_U32 u32Data = 0;
    MS_U8 u8Addr = 0;
    MS_U32 u32Timeout = 0;
    MS_U8 u8Data[4] = {0xFF, 0xFF, 0xFF, 0xFF};
    MS_U16 u16jj = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    if (pu8Cmd == NULL || u16CmdLen < TSIO_CC_CMD_MIN_LEN || u16CmdLen > TSIO_CC_CMD_MAX_LEN)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    HAL_TSIO_CC_RxEnable(TRUE);
    for (u16ii = 0; u16ii < u16CmdLen; u16ii+=4)
    {
        for (u16jj = 0; u16jj < 4; u16jj++)
        {
            if ((u16ii + u16jj) >= u16CmdLen)
                u8Data[u16jj] = 0xFF;
            else
                u8Data[u16jj] = pu8Cmd[u16ii+u16jj];
        }
        u32Data = u8Data[0] + (u8Data[1] << 8)
            + (u8Data[2] << 16) + (u8Data[3] << 24);
        HAL_TSIO_CC_TxWriteData(u8Addr, u32Data);
        u8Addr++;
    }
    HAL_TSIO_CC_TxWriteSize(u16CmdLen);
    HAL_TSIO_CC_TxEnable(TRUE);

    while (!_CC_TX_INT)
    {
        MsOS_DelayTask(1);
        if (u32Timeout++ > TSIO_CC_CMD_TIMEOUT)
        {
            HAL_TSIO_CC_TxEnable(FALSE);
            _TSIO_RETURN(DRVTSIO_FAIL);
        }
    }

    _CC_TX_INT = FALSE;
    HAL_TSIO_CC_TxEnable(FALSE);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Send C&C cmd in speed-up mode
/// @param pu8Cmd                                        \b IN: Pointer to store C&C cmd address
/// @param u16CmdLen                                   \b IN: C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_SendCmd_Fast(MS_U8 *pu8Cmd, MS_U16 u16CmdLen)
{
    MS_U16 u16ii = 0;
    MS_U32 u32Data = 0;
    MS_U32 u32Timeout = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    if (pu8Cmd == NULL || u16CmdLen < TSIO_CC_CMD_MIN_LEN || u16CmdLen > TSIO_CC_CMD_MAX_LEN)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    HAL_TSIO_CC_RxEnable(TRUE);

    HAL_TSIO_CC_TxWriteData_FastEnable(TRUE);
    for (u16ii = 0; u16ii < u16CmdLen; u16ii+=4)
    {
        u32Data = pu8Cmd[u16ii] + (pu8Cmd[u16ii+1] << 8)
            + (pu8Cmd[u16ii+2] << 16) + (pu8Cmd[u16ii+3] << 24);
        HAL_TSIO_CC_TxWriteData_Fast(u32Data);
    }
    HAL_TSIO_CC_TxWriteData_FastEnable(FALSE);

    HAL_TSIO_CC_TxWriteSize(u16CmdLen);
    HAL_TSIO_CC_TxEnable(TRUE);


    while (!_CC_TX_INT)
    {
        MsOS_DelayTask(1);
        if (u32Timeout++ > TSIO_CC_CMD_TIMEOUT)
        {
            HAL_TSIO_CC_TxEnable(FALSE);
            _TSIO_RETURN(DRVTSIO_FAIL);
        }
    }

    _CC_TX_INT = FALSE;
    HAL_TSIO_CC_TxEnable(FALSE);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Receive C&C cmd
/// @param pu8Cmd                                        \b OUT: Pointer to store C&C cmd address
/// @param pu16CmdLen                                  \b OUT: Pointer to store C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_ReceiveCmd(MS_U8 *pu8Cmd, MS_U16 *pu16CmdLen)
{
    MS_U16 u16ii = 0;
    MS_U32 u32Data = 0;
    MS_U8 u8Addr = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    if (pu8Cmd == NULL || pu16CmdLen == NULL)
    {
        HAL_TSIO_CC_RxEnable(FALSE);
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    *pu16CmdLen = HAL_TSIO_CC_RxReadSize();
    if (*pu16CmdLen < TSIO_CC_CMD_MIN_LEN || *pu16CmdLen > TSIO_CC_CMD_MAX_LEN)
    {
        HAL_TSIO_CC_RxEnable(FALSE);
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    for (u16ii = 0; u16ii < *pu16CmdLen; u16ii+=4)
    {
        u32Data = HAL_TSIO_CC_RxReadData(u8Addr);
        pu8Cmd[u16ii] = (u32Data & 0x000000FF);
        pu8Cmd[u16ii+1] = (u32Data & 0x0000FF00)>>8;
        pu8Cmd[u16ii+2] = (u32Data & 0x00FF0000)>>16;
        pu8Cmd[u16ii+3] = (u32Data & 0xFF000000)>>24;
        u8Addr++;
    }

    HAL_TSIO_CC_RxEnable(FALSE);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Receive C&C cmd in speed-up mode
/// @param pu8Cmd                                        \b OUT: Pointer to store C&C cmd address
/// @param pu16CmdLen                                  \b OUT: Pointer to store C&C cmd length
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_ReceiveCmd_Fast(MS_U8 *pu8Cmd, MS_U16 *pu16CmdLen)
{
    MS_U16 u16ii = 0;
    MS_U32 u32Data = 0;
    MS_U8 u8Addr = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    if (pu8Cmd == NULL || pu16CmdLen == NULL)
    {
        HAL_TSIO_CC_RxEnable(FALSE);
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    *pu16CmdLen = HAL_TSIO_CC_RxReadSize();
    if (*pu16CmdLen < TSIO_CC_CMD_MIN_LEN || *pu16CmdLen > TSIO_CC_CMD_MAX_LEN)
    {
        HAL_TSIO_CC_RxEnable(FALSE);
        _TSIO_RETURN(DRVTSIO_FAIL);
    }

    HAL_TSIO_CC_RxReadData_FastEnable(TRUE);
    for (u16ii = 0; u16ii < *pu16CmdLen; u16ii+=4)
    {
        u32Data = HAL_TSIO_CC_RxReadData_Fast(u8Addr);
        pu8Cmd[u16ii] = (u32Data & 0x000000FF);
        pu8Cmd[u16ii+1] = (u32Data & 0x0000FF00)>>8;
        pu8Cmd[u16ii+2] = (u32Data & 0x00FF0000)>>16;
        pu8Cmd[u16ii+3] = (u32Data & 0xFF000000)>>24;
        u8Addr++;
    }
    HAL_TSIO_CC_RxReadData_FastEnable(FALSE);

    HAL_TSIO_CC_RxEnable(FALSE);
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Receive interupt status
/// @param u32Timeout                                  \b IN: Timeout value
/// @param pu16Status                                  \b OUT: Pointer to store C&C receiver status address
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_CC_ReceiveIntStatus(MS_U32 u32Timeout, MS_U16 *pu16Status)
{
    MS_U32 u32Timer = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    if (pu16Status == NULL)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    while (!_CC_RX_INT)
    {
        MsOS_DelayTask(1);
        if (u32Timer++ > u32Timeout)
        {
            _TSIO_RETURN(DRVTSIO_FAIL);
        }
    }

    *pu16Status = _CC_RX_ERROR_STATUS;
    _CC_RX_INT = FALSE;
    _CC_RX_ERROR_STATUS = 0;
    _TSIO_RETURN(DRVTSIO_OK);
}

//-------------------------------------------------------------------------------------------------
/// Register interrupt callback function
/// @param TSIOIntCB                                  \b IN: Callback function pointer
/// @return TSIO_Result
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_RegisterIntCb(TSIOIntCB efn)
{
    _TSIO_CbFn = efn;
    return DRVTSIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get TSIO informaiton
/// @param eInfo                                        \b IN: Info type
/// @param u32Arg                                      \b IN: detail value
/// @param u32Retinfo                                 \b OUT: Pointer to store Info address
/// @return DMX_FILTER_STATUS
/// @note
//-------------------------------------------------------------------------------------------------
TSIO_Result MDrv_TSIO_GetInfo(DRVTSIO_INFO eInfo, MS_U32 u32Arg, MS_U32 *u32Retinfo)
{
    MS_U32 u32RetData = 0;

    TSIO_DBGTRACE();
    _TSIO_ENTRY();
    if (u32Retinfo == NULL)
    {
        _TSIO_RETURN(DRVTSIO_INVALID_PARAM);
    }

    switch (eInfo)
    {
        case DRVTSIO_INFO_LOSELOCKCNT:
            u32RetData = HAL_TSIO_GetLoselockcnt();
            break;

        case DRVTSIO_INFO_USEBUFLEN:
            u32RetData = HAL_TSIO_SGDMAOUT_ReadSram(u32Arg, HAL_SGDMA_SRAM_BUF_USED_SIZE, _ptsio_res->_bDMAoutLSB[u32Arg]);
            break;

        default:
            u32RetData = 0;
            break;
    }

    *u32Retinfo = u32RetData;
    _TSIO_RETURN(DRVTSIO_OK);
}


