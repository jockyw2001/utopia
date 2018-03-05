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
// file    osalTSP.c
// @brief  Transport Stream Processer (TSP) OSAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(MSOS_TYPE_NOS) || defined(MSOS_TYPE_NUTTX)

#include "MsCommon.h"
#include "osalTSP.h"

#define TSP_NOS_EVT_NUM         0x10

extern MS_U32 _u32IntNum;

MS_S32 u32EventId[TSP_NOS_EVT_NUM];
MS_U32 u32EventFree = 0xFFFFFFFF;

// Mutex
inline MS_S32 OSAL_TSP_MutexCreate(MS_U8* pu8Name)
{
    return 0;
}

inline MS_BOOL OSAL_TSP_MutexObtain(MS_S32 s32MutexId, MS_U32 u32Timeout)
{
    if(s32MutexId != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

inline MS_BOOL OSAL_TSP_MutexRelease(MS_S32 s32MutexId)
{
    if(s32MutexId != -1)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

// Process
inline void OSAL_TSP_Proc(OSAL_TSP_ProcType Proc, OSAL_TSP_ProcTypeParam* pData, MS_U32* pu32Evt)
{
    Proc(pData, pu32Evt);
}

// @FIXME: E_IRQ_TSP should not used here
// Interrupt
inline void OSAL_TSP_IntDisable(void)
{
    MsOS_DisableInterrupt((InterruptNum)_u32IntNum);
}

inline void OSAL_TSP_IntEnable(void)
{
    MsOS_EnableInterrupt((InterruptNum)_u32IntNum);
}

inline void OSAL_TSP_IntAttach(OSAL_TSP_IsrType Proc)
{
    MsOS_AttachInterrupt((InterruptNum)_u32IntNum, (InterruptCb)Proc);
}

inline void OSAL_TSP_IntDetach(void)
{
    MsOS_DetachInterrupt((InterruptNum)_u32IntNum);
}

// Event
inline MS_S32 OSAL_TSP_EventCreate(MS_U8* pu8Name)
{
    MS_U32 i;

    for (i= 0; i< TSP_NOS_EVT_NUM; i++)
    {
        if (u32EventFree & (1 << i))
        {
            u32EventFree &= ~(1<< i);
            u32EventId[i] = 0x0;
            return (MS_S32) (i | 0x87650000);
        }
    }
    return -1;
}

inline MS_BOOL OSAL_TSP_EventDelete(MS_S32 s32TspEventId)
{
    if (0x87650000 != (0xFFFF0000 & s32TspEventId))
    {
        return FALSE;
    }
    else
    {
        s32TspEventId &= 0x0000FFFF;
    }
    u32EventFree |= (1<< s32TspEventId);
    u32EventId[s32TspEventId] = 0x0;
    return TRUE;
}

inline MS_BOOL OSAL_TSP_EventSet(MS_S32 s32EventGroupId, MS_U32 u32EventFlag)
{
    MS_U32 u32Id;

    if (0x87650000 != (0xFFFF0000 & s32EventGroupId))
    {
        return FALSE;
    }

    u32Id = s32EventGroupId & 0x0000FFFF;
    u32EventId[u32Id] |= u32EventFlag;
    return TRUE;
}

inline MS_BOOL OSAL_TSP_EventGet(MS_S32 s32TspEventId, MS_U32 u32EventGroup, MS_U32* pu32RetrievedEventFlag, MS_U32 u32Opt, MS_U32 u32Timeout)
{
    MS_U32 u32Id;

    if (0x87650000 != (0xFFFF0000 & s32TspEventId))
    {
        return FALSE;
    }
    // @FIXME: nos verson don't support timeout at this stage

    u32Id = s32TspEventId & 0x0000FFFF;
    *pu32RetrievedEventFlag = u32EventId[u32Id] & u32EventGroup;
    if (OSAL_EVENT_OR_CLEAR == u32Opt)
    {
    u32EventId[u32Id] &= ~(u32EventGroup);
    }
    return (*pu32RetrievedEventFlag)? TRUE: FALSE;
}

/*
inline MS_BOOL OSAL_TSP_FltEventGet(MS_S32 s32TspEventId, MS_U32 u32FilterID)
{
    MS_U32  u32Id;
    MS_BOOL bResult;

    if (0x87650000 != (0xFFFF0000 & s32TspEventId))
    {
        return FALSE;
    }

    u32Id = s32TspEventId & 0x0000FFFF;

    //printf("u32EventId[u32Id] = %lx\n", u32EventId[u32Id]);

    bResult = u32EventId[u32Id] & BIT(u32FilterID);
    u32EventId[u32Id] &= ~BIT(u32FilterID);

    return bResult;
}
*/

inline MS_S32 OSAL_TSP_TaskCreate(OSAL_TSP_ProcType Proc, MS_U8* pu8Name)
{
    return 0;
}
#endif // #ifdef MSOS_TYPE_NOS
