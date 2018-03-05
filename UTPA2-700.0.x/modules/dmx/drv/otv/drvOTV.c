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
/// file   drvOTV.c
/// @brief  OTV Driver Interface
/// @author MStar Semiconductor,Inc.
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

// Common Definition

#include "MsCommon.h"
#include "MsVersion.h"


#include "drvOTV.h"
#include "drvMMIO.h"
#include "regOTV.h"
#include "halOTV.h"


#include "asmCPU.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#endif

//--------------------------------------------------------------------------------------------------
//  Debug Macro
//--------------------------------------------------------------------------------------------------

#define DRV_OTV_DBGMSG(_level,_model,_fmt,_args...)  if(_u32DrvOtvDbgLevel >= (_level)&&((_u32DrvOtvDbgModel&_model)!=0)) { printf("[%s][%d]",__FUNCTION__,__LINE__); printf(_fmt,##_args);}
static MS_U32  _u32DrvOtvDbgLevel = E_DRV_OTV_DBG_LEVEL_ERR;
static MS_U32  _u32DrvOtvDbgModel = E_DRV_OTV_DBG_MODEL_ALL;


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------

#define _OTV_ENTRY()       if (!MsOS_ObtainMutex(_pDrvOTV_Res->_s32MutexId, MSOS_WAIT_FOREVER))      \
                            {                                                                        \
                                _pDrvOTV_Res->_u32LastErr = DRVOTV_FUNC_ERROR;                       \
                                return DRVOTV_FUNC_ERROR;                                            \
                            }                                                                        \
                            if (_pDrvOTV_Res->_bSuspend)                                             \
                            {                                                                        \
                                _OTV_RETURN(DRVOTV_FAIL);                                            \
                            }

#define _OTV_RETURN(_ret)  if(MsOS_ReleaseMutex(_pDrvOTV_Res->_s32MutexId)) {return _ret;}           \
                            else                                                                     \
                            {                                                                        \
                                _pDrvOTV_Res->_u32LastErr = DRVOTV_FUNC_ERROR;                       \
                                return DRVOTV_FUNC_ERROR;                                            \
                            }



#define _OTV_CHECK(idx)    if( (MS_U32)idx >= (MS_U32)OTV_ENG_NUM ) \
                                                        { printf("[OTV][ERR] OTV Engine [%u] not exist ! \n",(unsigned int)idx);   \
                                                         _OTV_RETURN(FALSE); }


//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------

typedef struct _DrvOTV_State
{
    MS_U32          _OTV_init;
    DrvOTV_FltState _OTV_FltState;
} DrvOTV_State;

typedef struct _DrvOTV_RESOURCE_PRIVATE
{
    MS_S32                       _s32TaskId;
    MS_S32                       _s32MutexId;
    MS_U32                       _u32LastErr;
    MS_BOOL                      _bSuspend;
    MS_BOOL                      _bISRTaskEn;
    DrvOTV_State                 _OTVState[OTV_ENG_NUM][OTV_PIDFLT_NUM_ALL];
    DrvOTV_NotifyCb              _pNotifyCb[OTV_ENG_NUM];                ///< OTV Event Array Callback

    MS_BOOL                      _bInitReady;
}DrvOTV_RESOURCE_PRIVATE;


//static MSIF_Version _drv_OTV_version = {
//    .DDI = { OTV_DRV_VERSION, },
//};

#ifndef OTV_UTOPIA_20
static DrvOTV_RESOURCE_PRIVATE _DrvOTV_Res = {

    ._s32TaskId                            = -1,
    ._s32MutexId                           = -1,
    ._u32LastErr                           = DRVOTV_OK,
    ._bSuspend                             = FALSE,
    ._bISRTaskEn                           = FALSE,
    ._bInitReady                           = FALSE,
};

static DrvOTV_RESOURCE_PRIVATE*    _pDrvOTV_Res = &_DrvOTV_Res;
#else
static DrvOTV_RESOURCE_PRIVATE*    _pDrvOTV_Res = NULL;
#endif


//------------------------------------------------------------------------//
//       OTV Task Variable declaration
//------------------------------------------------------------------------//

#define OTV_TASK_STACK_SIZE      (1024 * 16 )
#define OTV_POLLING_TASK_DELAY   50

static MS_S32  _s32OTV_EventId = -1 ;
static MS_BOOL _s32OTV_IsrTaskReturn = FALSE;


#if defined (MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)
static MS_U32            _OTV_Stack[OTV_TASK_STACK_SIZE/sizeof(MS_U32)];
#endif

static DrvOTV_EventArray _EventDataArray[OTV_ENG_NUM][OTV_EVENT_FIFO_DEPTH];

//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------

static MS_BOOL _OTV_InitResource(DrvOTV_RESOURCE_PRIVATE* presource)
{
    if(presource == NULL)
        return FALSE;

    presource->_s32TaskId  = -1;
    presource->_s32MutexId = -1;
    presource->_bSuspend   = FALSE;
    presource->_bInitReady = FALSE;
    presource->_u32LastErr = DRVOTV_OK;

    return TRUE;
}

static void _OTV_IsrTask(void)
{

    MS_U32 u32Events;
    MS_U32 u32OTV_Eng;
    MS_U32 u32EventNum;

    DrvOTV_NotifyCb _Event_CallBackFun;

    while(_pDrvOTV_Res->_bISRTaskEn)
    {
        MsOS_WaitEvent(_s32OTV_EventId, 0xFFFFFFFF, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        if(u32Events & E_DRVOTV_HWINT_TASKEND)
        {
            break;
        }

        DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_INFO, E_DRV_OTV_DBG_MODEL_ALL, "_OTV_IsrTask!!----- u32Events = %08x \n",(unsigned int)u32Events);

        u32OTV_Eng  = (u32Events & E_DRVOTV_ENG_MASK)      >> E_DRVOTV_ENG_MASK_SHIFT;
        u32EventNum = (u32Events & E_DRVOTV_EVENT_NUM_MASK)>> E_DRVOTV_EVENT_NUM_MASK_SHIFT;

        if(u32OTV_Eng >= OTV_ENG_NUM)
        {
            DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_ERR, E_DRV_OTV_DBG_MODEL_ALL, "Invalid OTV Eng %u \n",(unsigned int)u32OTV_Eng);
            continue;
        }

        if( u32Events & E_DRVOTV_HWINT_EVENT  )
        {
             _Event_CallBackFun = _pDrvOTV_Res->_pNotifyCb[u32OTV_Eng];
             _Event_CallBackFun(u32OTV_Eng, E_DRVOTV_HWINT_EVENT, u32EventNum, _EventDataArray[u32OTV_Eng], NULL);
        }

        if( u32Events &  E_DRVOTV_HWINT_TIMER )
        {
            _Event_CallBackFun = _pDrvOTV_Res->_pNotifyCb[u32OTV_Eng];
            _Event_CallBackFun(u32OTV_Eng, E_DRVOTV_HWINT_TIMER, u32EventNum, _EventDataArray[u32OTV_Eng], NULL);
        }

        MsOS_DelayTask(OTV_POLLING_TASK_DELAY);

    }// end while

    _s32OTV_IsrTaskReturn = TRUE;
}

static void _OTV_SwStateReset(void)
{
    MS_U32 _u32OTVnum = 0;
    MS_U32 i = 0;
    for (_u32OTVnum = 0 ; _u32OTVnum < OTV_ENG_NUM ; _u32OTVnum++)
    {
        for (i = OTV_PIDFLT_START_NUM; i < OTV_PIDFLT_END_NUM; i++)
        {
            _pDrvOTV_Res->_OTVState[_u32OTVnum][i]._OTV_init = 0;
            _pDrvOTV_Res->_OTVState[_u32OTVnum][i]._OTV_FltState = E_DRVOTV_FLT_STATE_FREE;
        }
    }
}

static void _OTV_RegStateReset(void)
{
    MS_U32 _u32fltnum = 0;
    MS_U32 _u32OTVnum = 0;

    for (_u32OTVnum = 0 ; _u32OTVnum < OTV_ENG_NUM ; _u32OTVnum++)
    {

        for (_u32fltnum = OTV_PIDFLT_START_NUM ; _u32fltnum < OTV_PIDFLT_END_NUM ; _u32fltnum++)
        {
            HAL_OTV_SetFltPid(_u32OTVnum, _u32fltnum, OTV_PID_NULL);
            HAL_OTV_SetFltEvent(_u32OTVnum, _u32fltnum, ~OTV_EVENT_MASK);
        }
    }
}

static DrvOTV_Result _OTV_Init(void)
{
    MS_U32 u32Idx;
    MS_VIRT u32Bank;
    MS_PHY u32BankSize;

    if (FALSE == MDrv_MMIO_GetBASE(&u32Bank, &u32BankSize, MS_MODULE_HW))
    {
        DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_ERR, E_DRV_OTV_DBG_MODEL_ALL, "MDrv_MMIO_GetBASE fail \n");
        MS_ASSERT(0);
    }

    HAL_OTV_SetBank(u32Bank);

    for(u32Idx = 0; u32Idx < OTV_ENG_NUM; u32Idx++)
    {
        _pDrvOTV_Res->_pNotifyCb[u32Idx] = NULL;

        //HAL_OTV_PowerCtrl(u8ii, ENABLE);
        HAL_OTV_Init(u32Idx);

        HAL_OTV_INT_Event_Enable(u32Idx,FALSE);
        HAL_OTV_INT_Timer_Enable(u32Idx,FALSE);
        HAL_OTV_INT_ClrEventHW(u32Idx);
        HAL_OTV_INT_ClrTimerHW(u32Idx);

        // OTV Config Setting
        HAL_OTV_PktTimerSrcFromPVR_Enable(u32Idx, TRUE);
        HAL_OTV_PktNumSrcFromPktDemux_Enable(u32Idx, TRUE);

    }

    _OTV_RegStateReset();
    _OTV_SwStateReset();

    if (-1 == _pDrvOTV_Res->_s32MutexId)
    {
        _pDrvOTV_Res->_s32MutexId = MsOS_CreateMutex(E_MSOS_FIFO , "OTV_Mutex" , MSOS_PROCESS_SHARED);
    }
    else
    {
        DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_ERR, E_DRV_OTV_DBG_MODEL_ALL, "Create _s32MutexId more than once \n");
    }

    return DRVOTV_OK;
}

static void _OTV_Close(void)
{
    MS_U32 u32Idx;

    for(u32Idx = 0; u32Idx < OTV_ENG_NUM; u32Idx++)
    {
        HAL_OTV_Init(u32Idx);
    }

}

// General API
//-------------------------------------------------------------------------------------------------
/// Initialize lib resource API
/// @param pResMemAddr                \b IN: Pointer to store resource memory address
/// @return MMFI_Result
/// @note
//-------------------------------------------------------------------------------------------------
DrvOTV_Result MDrv_OTV_InitLibResource(void* pResMemAddr)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, " ");

    if(pResMemAddr == 0)
        return DRVOTV_FAIL;

    _pDrvOTV_Res = (DrvOTV_RESOURCE_PRIVATE*)pResMemAddr;

    if(_pDrvOTV_Res->_bInitReady != TRUE)
    {
        if(_OTV_InitResource(_pDrvOTV_Res) == FALSE)
        {
            _pDrvOTV_Res->_u32LastErr = DRVOTV_FAIL;
        }
        else
            _pDrvOTV_Res->_u32LastErr = DRVOTV_OK;
    }

    return (_pDrvOTV_Res->_u32LastErr);
}

//--------------------------------------------------------------------------------------------------
/// Initialize OTV driver and OTV engine
/// @return DrvOTV_Result
/// @note
/// It should be called before calling any other OTV DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//--------------------------------------------------------------------------------------------------
DrvOTV_Result MDrv_OTV_Init(void)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, " ");

    DrvOTV_Result bRet;

    bRet = _OTV_Init();


    if (-1 == _s32OTV_EventId)
    {
       _s32OTV_EventId = MsOS_CreateEventGroup("OTV_Event");
    }

    _OTV_ENTRY();

    if (-1 == _pDrvOTV_Res->_s32TaskId)
    {
#if defined (MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)

       _pDrvOTV_Res->_s32TaskId = MsOS_CreateTask(  (TaskEntry) _OTV_IsrTask,
                                                    (MS_U32)NULL,
                                                    (TaskPriority) E_TASK_PRI_SYS,
                                                    TRUE,
                                                    _OTV_Stack,
                                                    OTV_TASK_STACK_SIZE,
                                                    "OTV_IsrTask");
#else
       _pDrvOTV_Res->_s32TaskId = MsOS_CreateTask(  (TaskEntry) _OTV_IsrTask,
                                                    (MS_U32)NULL,
                                                    (TaskPriority) E_TASK_PRI_SYS,
                                                    TRUE,
                                                    NULL,
                                                    0,
                                                    "OTV_IsrTask");
#endif
    }

    _pDrvOTV_Res->_bInitReady = TRUE;
    _pDrvOTV_Res->_bISRTaskEn = TRUE;
    _OTV_RETURN(bRet);
}

//--------------------------------------------------------------------------------------------------
/// Poweroff OTV driver and OTV engine
/// @return DrvOTV_Result
/// @note
//--------------------------------------------------------------------------------------------------
#if 0
DrvOTV_Result MDrv_DrvOTV_Reset(void)
{
    _OTV_ENTRY();
    _OTV_SwStateReset();
    _OTV_RegStateReset();
    _OTV_RETURN(DRVOTV_OK);
}
#endif
//--------------------------------------------------------------------------------------------------
/// Close OTV driver and reset software/register state
/// @return DrvOTV_Result
/// @note
//--------------------------------------------------------------------------------------------------
DrvOTV_Result MDrv_OTV_Exit(void)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, " ");

    if(_pDrvOTV_Res->_s32TaskId != -1)
    {
        while(!_s32OTV_IsrTaskReturn)
        {
            MsOS_SetEvent(_s32OTV_EventId, E_DRVOTV_HWINT_TASKEND);
            MsOS_DelayTask(OTV_POLLING_TASK_DELAY);
        }

        _s32OTV_IsrTaskReturn = FALSE;

        _pDrvOTV_Res->_s32TaskId = -1;
        _pDrvOTV_Res->_bISRTaskEn = FALSE;
    }

    if (_pDrvOTV_Res->_s32MutexId != -1)
    {
        _OTV_ENTRY();
        MsOS_ReleaseMutex(_pDrvOTV_Res->_s32MutexId);
        MsOS_DeleteMutex(_pDrvOTV_Res->_s32MutexId);
        _pDrvOTV_Res->_s32MutexId = -1;
    }

    if(_pDrvOTV_Res->_bInitReady == TRUE)
    {
        _OTV_SwStateReset();
        _OTV_RegStateReset();
        _OTV_Close();
        _pDrvOTV_Res->_bInitReady = FALSE;
    }

    return DRVOTV_OK;
}

DrvOTV_Result MDrv_OTV_ISR(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, HWINT_TYPE = %x \n",(unsigned int)u32OTVEng, (unsigned int)HWINT_TYPE);

    _OTV_CHECK(u32OTVEng);

    MS_U32  u32OTV_IntStatus = 0;
    MS_U32  u32EventNum;
    MS_U32  u32EventIdx = 0;
    MS_U32  u32Idx;

    HAL_OTV_EventQ_GetEventNum(u32OTVEng, &u32EventNum);

    while( u32EventNum )
    {
        for(u32Idx=0; u32Idx <u32EventNum; u32Idx++)
        {
            if( HAL_OTV_EventQ_PopEvent(u32OTVEng) )
            {
                HAL_OTV_EventQ_GetEventDesciptor(u32OTVEng, &_EventDataArray[u32OTVEng][u32EventIdx].Event_Descriptor);
                HAL_OTV_EventQ_GetEventPktPID(   u32OTVEng, &_EventDataArray[u32OTVEng][u32EventIdx].Pid);
                HAL_OTV_EventQ_GetEventPktNum(   u32OTVEng, &_EventDataArray[u32OTVEng][u32EventIdx].PacketNum);
                HAL_OTV_EventQ_GetEventPktTimer( u32OTVEng, &_EventDataArray[u32OTVEng][u32EventIdx].Timer);
                HAL_OTV_EventQ_GetEventPktPCR(   u32OTVEng, &_EventDataArray[u32OTVEng][u32EventIdx].PCR);
            }
            u32EventIdx++;
        }

        HAL_OTV_EventQ_GetEventNum(u32OTVEng, &u32EventNum);
    }// end while

    u32OTV_IntStatus |= ((  u32OTVEng <<       E_DRVOTV_ENG_MASK_SHIFT) & E_DRVOTV_ENG_MASK);
    u32OTV_IntStatus |= ((u32EventIdx << E_DRVOTV_EVENT_NUM_MASK_SHIFT) & E_DRVOTV_EVENT_NUM_MASK);
    u32OTV_IntStatus |= (( HWINT_TYPE <<     E_DRVOTV_HWINT_MASK_SHIFT) & E_DRVOTV_HWINT_MASK);

    if(u32OTV_IntStatus == 0)
    {
        return DRVOTV_FAIL;
    }
    else
    {
        MsOS_SetEvent(_s32OTV_EventId, u32OTV_IntStatus);
    }

    return DRVOTV_OK;
}

DrvOTV_Result MDrv_OTV_AllocFlt(MS_U32 u32OTVEng, MS_U32 *pu32FltId)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u \n",(unsigned int)u32OTVEng);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    MS_U32 i = 0;

    for (i = OTV_PIDFLT_START_NUM ; i < OTV_PIDFLT_END_NUM ; i++)
    {
        if (_pDrvOTV_Res->_OTVState[u32OTVEng][i]._OTV_FltState == E_DRVOTV_FLT_STATE_FREE)
        {
            _pDrvOTV_Res->_OTVState[u32OTVEng][i]._OTV_FltState = E_DRVOTV_FLT_STATE_ALLOC;
            *pu32FltId = i;
            break;
        }
    }

    if (i == OTV_PIDFLT_END_NUM)
    {
        DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_ERR, E_DRV_OTV_DBG_MODEL_ALL, "No free fitler found 0x%02x \n", (unsigned int)i);
        _OTV_RETURN(DRVOTV_FAIL);
    }

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_SetFltPid(MS_U32 u32OTVEng, MS_U32 u32FltId, MS_U16 u16Pid)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, FltId = %u, Pid = 0x%x\n",(unsigned int)u32OTVEng, (unsigned int)u32FltId, u16Pid);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    HAL_OTV_SetFltPid(u32OTVEng, u32FltId, u16Pid);

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_GetFltPid(MS_U32 u32OTVEng, MS_U32 u32FltId, MS_U16 *pu16Pid)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, FltId = %u\n",(unsigned int)u32OTVEng, (unsigned int)u32FltId);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    HAL_OTV_GetFltPid(u32OTVEng, u32FltId, pu16Pid);

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_SetFltEvent(MS_U32 u32OTVEng, MS_U32 u32FltId, DRVOTV_EVENT_TYPE eEvent)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, FltId = %u, eEvent = %x\n",(unsigned int)u32OTVEng, (unsigned int)u32FltId, (unsigned int)eEvent);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    HAL_OTV_SetFltEvent(u32OTVEng, u32FltId, eEvent);

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_GetFltEvent(MS_U32 u32OTVEng, MS_U32 u32FltId, DRVOTV_EVENT_TYPE* pEvent)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, FltId = %u \n",(unsigned int)u32OTVEng, (unsigned int)u32FltId);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    HAL_OTV_GetFltEvent(u32OTVEng, u32FltId, pEvent);

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_ReleaseFlt(MS_U32 u32OTVEng, MS_U32 u32FltId)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, FltId = %u \n",(unsigned int)u32OTVEng, (unsigned int)u32FltId);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    _pDrvOTV_Res->_OTVState[u32OTVEng][u32FltId]._OTV_FltState = E_DRVOTV_FLT_STATE_FREE;
    HAL_OTV_SetFltPid(u32OTVEng, u32FltId, OTV_PID_NULL);

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_GetCurPktStatus(MS_U32 u32OTVEng, MS_U32* u32PktStamp, MS_U32* u32PktNumber)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u \n",(unsigned int)u32OTVEng);

    HAL_OTV_GetCurPktTimer(u32OTVEng, u32PktStamp);
    HAL_OTV_GetCurPktNum(u32OTVEng, u32PktNumber);

    return TRUE;
}

DrvOTV_Result MDrv_OTV_SetNotifyCb(MS_U32 u32OTVEng, DrvOTV_NotifyCb pfCallback)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u \n",(unsigned int)u32OTVEng);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    if (pfCallback)
    {
        _pDrvOTV_Res->_pNotifyCb[u32OTVEng] = pfCallback;
    }
    else
    {
        _pDrvOTV_Res->_pNotifyCb[u32OTVEng] = NULL;
    }

    _OTV_RETURN(DRVOTV_OK);
}

DrvOTV_Result MDrv_OTV_HWINT_Enable(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE, MS_BOOL bEnable)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, HWINT_TYPE = %x, bEnable = %d\n",(unsigned int)u32OTVEng, (unsigned int)HWINT_TYPE, bEnable);

    _OTV_ENTRY();
    _OTV_CHECK(u32OTVEng);

    if(HWINT_TYPE == E_DRVOTV_HWINT_EVENT)
    {
        HAL_OTV_EventQ_Enable(u32OTVEng, bEnable);
        HAL_OTV_INT_Event_Enable(u32OTVEng, bEnable);
    }
    else if(HWINT_TYPE == E_DRVOTV_HWINT_TIMER)
    {
        HAL_OTV_EventQ_Enable(u32OTVEng, bEnable);
        HAL_OTV_INT_Timer_Enable(u32OTVEng, bEnable);
    }

    _OTV_RETURN(DRVOTV_OK);
}


DrvOTV_Result MDrv_OTV_HWINT_Get(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE, MS_BOOL* bInterrupted)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, HWINT_TYPE = %x \n",(unsigned int)u32OTVEng, (unsigned int)HWINT_TYPE);

    if(!bInterrupted)
        return DRVOTV_FAIL;

     _OTV_CHECK(u32OTVEng);

    switch (HWINT_TYPE)
    {
        case E_DRVOTV_HWINT_EVENT:
            if(HAL_OTV_INT_GetEventHW(u32OTVEng))
                *bInterrupted = TRUE;
            else
                *bInterrupted = FALSE;
            break;

        case E_DRVOTV_HWINT_TIMER:
            if(HAL_OTV_INT_GetTimerHW(u32OTVEng))
                *bInterrupted = TRUE;
            else
                *bInterrupted = FALSE;
            break;

        default:
            return DRVOTV_FAIL;
    }

    return DRVOTV_OK;
}

DrvOTV_Result MDrv_OTV_HWINT_Clear(MS_U32 u32OTVEng, DRVOTV_HWINT_TYPE HWINT_TYPE)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, HWINT_TYPE = %x \n",(unsigned int)u32OTVEng, (unsigned int)HWINT_TYPE);

    _OTV_CHECK(u32OTVEng);

    switch (HWINT_TYPE)
    {
        case E_DRVOTV_HWINT_EVENT:
            HAL_OTV_INT_ClrEventHW(u32OTVEng);
            break;

        case E_DRVOTV_HWINT_TIMER:
            HAL_OTV_INT_ClrTimerHW(u32OTVEng);
            break;

        default:
            return DRVOTV_FAIL;
    }

    return DRVOTV_OK;
}

DrvOTV_Result MDrv_OTV_HWINT_SetEvent_Threshold(MS_U32 u32OTVEng, MS_U32 u32Threshold /*Maximum value is 31*/)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, Event Threshold = %u \n",(unsigned int)u32OTVEng, (unsigned int)u32Threshold);

    _OTV_CHECK(u32OTVEng);

    if (u32Threshold <= 31 )
    {
        HAL_OTV_INT_SetEventWaterLevel(u32OTVEng, u32Threshold);
        return DRVOTV_OK;
    }
    else
    {
        DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_ERR, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u  Event Threshold Exceeds \n",(unsigned int)u32OTVEng);
        return DRVOTV_FAIL;
    }

}

DrvOTV_Result MDrv_OTV_HWINT_SetTimer_Timeout(MS_U32 u32OTVEng, DrvOTV_Time_Watermark Timeout)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u, Timeout = %x \n",(unsigned int)u32OTVEng, (unsigned int)Timeout);

    _OTV_CHECK(u32OTVEng);

    if ( Timeout>=E_DRVOTV_TIME_WATERMARK_50MS && Timeout<=E_DRVOTV_TIME_WATERMARK_500MS )
    {
        HAL_OTV_INT_SetTimerWaterLevel(u32OTVEng, (MS_U32)Timeout );
        return DRVOTV_OK;
    }
    else
    {
        DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_ERR, E_DRV_OTV_DBG_MODEL_ALL, "OTV_Eng = %u  Timer Threshold Exceeds \n",(unsigned int)u32OTVEng);
        return DRVOTV_FAIL;
    }
}

DrvOTV_Result MDrv_OTV_SetDbgLevel(DRV_OTV_DBGMSG_LEVEL level, DRV_OTV_DBGMSG_MODEL model)
{
    DRV_OTV_DBGMSG(E_DRV_OTV_DBG_LEVEL_FUNC, E_DRV_OTV_DBG_MODEL_ALL, "OTV_DBG_Level = %u, OTV_DBG_Model = %u \n", (unsigned int)level, (unsigned int)model);

    _u32DrvOtvDbgLevel = level;
    _u32DrvOtvDbgModel = model;

    return DRVOTV_OK;
}


// [ToDo]
#if 0
DrvOTV_Result MDrv_OTV_GetCap(DrvOTV_Cap eCap, void* pOutput)
{
    *((MS_U32*)pOutput) = 0;

    switch (eCap)
    {
        case E_DRVOTV_CAP_RESOURCE_SIZE:
            *((MS_U32*)pOutput) = sizeof(DrvOTV_RESOURCE_PRIVATE);
            return DRVOTV_OK;
        case E_DRVOTV_CAP_FILTER_NUM:
        case E_DRVOTV_CAP_FILTER_PVR_NUM:
            *((MS_U32*)pOutput) = OTV_PIDFLT_NUM_ALL;
            return DRVOTV_OK;
        case E_DRVOTV_CAP_PVR_ALIGN:
            *((MS_U32*)pOutput) = 16;
            return DRVOTV_OK;
        case E_DRVOTV_CAP_OTV_ENG_NUM:
            *((MS_U32*)pOutput) = OTV_ENG_NUM;
            return DRVOTV_OK;
        default:
            return DRVOTV_INVALID_PARAM;
    }
}


#endif
