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
/// file    sc_msos.c
/// @brief  SmartCard Driver MsOS Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "drvSC.h"

// Internal Definition
#include "sc_msos.h"
#include "halSC.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define OS_SC_TASK_STACK_SIZE       4096UL // 512*4

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_S32                              os_SC_MutexID = -1;
MS_S32                              os_SC_MutexID_TX = -1;
MS_S32                              os_SC_MutexID_SHM = -1;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32                       _os_SC_TaskStack[OS_SC_TASK_STACK_SIZE]; //TODO smaller stack
static MS_S32                       _os_SC_TaskID = -1;
static MS_S32                       _os_SC_EventID = -1;
static P_SC_ISR_Proc                _os_SC_ISR_Proc[SC_DEV_NUM]; // in case there should be multiple ISR
static MS_BOOL                      _os_SC_TaskRunning = FALSE;
static MS_BOOL                      _os_SC_TaskStart = FALSE;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void OS_SC_SetEvent(MS_U32 u32Events)
{
    MsOS_SetEvent(_os_SC_EventID, u32Events);
}

MS_U32 OS_SC_WaitEvent(void)
{
    MS_U32              u32Events;

    MsOS_WaitEvent(_os_SC_EventID, OS_SC_EVENT_MASK | OS_SC_EVENT_MASK2, &u32Events, E_OR, 100);

    return u32Events;
}


void OS_SC_ClearEvent(MS_U32 u32Events)
{
    MsOS_ClearEvent(_os_SC_EventID, u32Events);
}

static void _OS_SC_Task(void)
{
    _os_SC_TaskRunning = TRUE;
    while(_os_SC_TaskStart)
    {
        MDrv_SC_Task_Proc();
    }
    _os_SC_TaskRunning = FALSE;
}

static void _OS_SC_ISR(void)
{
    if (_os_SC_ISR_Proc[0](0) == TRUE)
    {
        // @TODO: Enabel IRQ at ISR or TASK?
        // If it's at TASK, ISR should EnableInterrupt if no CARD EVENT (why dummy interrupt incoming?)
        MsOS_EnableInterrupt(SC_IRQ);
    }
}

#if (SC_DEV_NUM > 1) // no more than 2
static void _OS_SC_ISR1(void)
{
    if (_os_SC_ISR_Proc[1](1) == TRUE)
    {
        MsOS_EnableInterrupt(SC_IRQ2);
    }
}
#endif

void OS_SC_ISR_Enable(MS_U8 u8SCID, P_SC_ISR_Proc pfISRProc)
{
    // u8SCID is checked by MDrv caller
    if (u8SCID == 0)
    {
        //printf("~~~~~~ENABLE [%s][%d]",__FUNCTION__,__LINE__);
        _os_SC_ISR_Proc[0] = pfISRProc;
        MsOS_AttachInterrupt(SC_IRQ, (InterruptCb)_OS_SC_ISR);
        MsOS_EnableInterrupt(SC_IRQ);
    }
#if (SC_DEV_NUM > 1) // no more than 2
    else if (u8SCID == 1)
    {
        _os_SC_ISR_Proc[1] = pfISRProc;
        MsOS_AttachInterrupt(SC_IRQ2, (InterruptCb)_OS_SC_ISR1);
        MsOS_EnableInterrupt(SC_IRQ2);
    }
#endif
}

void OS_SC_ISR_Disable(MS_U8 u8SCID)
{
    // u8SCID is checked by MDrv caller
    if (u8SCID == 0)
    {
        MsOS_DisableInterrupt(SC_IRQ);
        MsOS_DetachInterrupt (SC_IRQ);
        _os_SC_ISR_Proc[0] = NULL;
    }
#if (SC_DEV_NUM > 1) // no more than 2
    else if (u8SCID == 1)
    {
        MsOS_DisableInterrupt(SC_IRQ2);
        MsOS_DetachInterrupt (SC_IRQ2);
        _os_SC_ISR_Proc[1] = NULL;
    }
#endif
}

MS_BOOL OS_SC_Init(MS_U8 u8SCID)
{
    // Initialize inter-process variable --------------------------------------
    if (_os_SC_TaskID < 0)
    {
        _os_SC_EventID = MsOS_CreateEventGroup("OS_SC_Event");
        if (_os_SC_EventID < 0)
        {
            return FALSE;
        }

        os_SC_MutexID = OS_CREATE_MUTEX(SC);
        if (os_SC_MutexID < 0)
        {
            MsOS_DeleteMutex(os_SC_MutexID);
            os_SC_MutexID = -1;
            return FALSE;
        }
		os_SC_MutexID_TX = OS_CREATE_MUTEX(SC_TX);
        if (os_SC_MutexID_TX < 0)
        {
            MsOS_DeleteMutex(os_SC_MutexID_TX);
            os_SC_MutexID_TX = -1;
            return FALSE;
        }
        os_SC_MutexID_SHM = OS_CREATE_MUTEX(SC_SHM);
        if (os_SC_MutexID_SHM < 0)
        {
            MsOS_DeleteMutex(os_SC_MutexID_SHM);
            os_SC_MutexID_SHM = -1;
            return FALSE;
        }
        
        _os_SC_TaskStart = TRUE;
        _os_SC_TaskID = MsOS_CreateTask((TaskEntry)_OS_SC_Task,
                                        NULL,
                                        E_TASK_PRI_MEDIUM,
                                        TRUE,
                                        _os_SC_TaskStack,
                                        OS_SC_TASK_STACK_SIZE,
                                        "OS_SC_Task");
        if (_os_SC_TaskID < 0)
        {
            MsOS_DeleteEventGroup(_os_SC_EventID);
            _os_SC_EventID = -1;
            MsOS_DeleteMutex(os_SC_MutexID);
            os_SC_MutexID = -1;
            MsOS_DeleteMutex(os_SC_MutexID_SHM);
            os_SC_MutexID_SHM = -1;
            return FALSE;
        }

    }
    return TRUE;
}

MS_BOOL OS_SC_Exit(MS_U8 u8SCID)
{
    MS_U8 u8Loop;
    MS_BOOL bRetVal = TRUE;

    if (_os_SC_TaskID < 0)
    {
        return FALSE;
    }

    OS_DELETE_MUTEX(os_SC_MutexID);
    os_SC_MutexID = -1;

    OS_DELETE_MUTEX(os_SC_MutexID_TX);
    os_SC_MutexID_TX = -1;

    OS_DELETE_MUTEX(os_SC_MutexID_SHM);
    os_SC_MutexID_SHM = -1;

    // Stop task and wait for task stop
    _os_SC_TaskStart = FALSE;
    for (u8Loop = 0; u8Loop < 20; u8Loop++)
    {
        if (!_os_SC_TaskRunning)
            break;
        else
            MsOS_DelayTask(100);
    }

    // Delete task even task terminating is failed
    MsOS_DeleteTask(_os_SC_TaskID);
    if (_os_SC_TaskRunning == TRUE)
    {
        bRetVal = FALSE;
    }
    _os_SC_TaskID = -1;

    // Delete event
    for (u8Loop = 0; u8Loop < 20; u8Loop++)
    {
        if (MsOS_DeleteEventGroup(_os_SC_EventID) == TRUE)
        {
            _os_SC_EventID = -1;
            break;
        }
        else
        {
            MsOS_DelayTask(20);
        }
    }
    if (_os_SC_EventID >= 0)
    {
        bRetVal = FALSE;
    }

    return bRetVal;
}
