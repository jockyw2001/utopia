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
/// file    sc_msos.h
/// @brief  SmartCard Driver MsOS Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SC_MSOS_H_
#define _SC_MSOS_H_


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsOS.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define OS_SC_MUTEX_TIMEOUT         1000UL
#define OS_SC_ENTRY()               if (!OS_OBTAIN_MUTEX(os_SC_MutexID, OS_SC_MUTEX_TIMEOUT)) {   return E_SC_FAIL;   }
#define OS_SC_RETURN(_ret)          { OS_RELEASE_MUTEX(os_SC_MutexID);return (_ret); } // Should be optimized by compiler
#define OS_SC_DELAY(_msec)          OS_DELAY_TASK(_msec)
#define OS_SC_TIME()                OS_SYSTEM_TIME()
#define OS_SC_EXPIRE(_msec)         (OS_SYSTEM_TIME() + (_msec))

#define OS_SC_EVENT_MASK            0x000000FFUL
#define OS_SC_EVENT_CARD            0x00000001UL
#define OS_SC_EVENT_SEND            0x00000010UL
#define OS_SC_EVENT_RECV            0x00000020UL
#define OS_SC_EVENT_MASK2           0x0000FF00UL
#define OS_SC_EVENT_CARD2           0x00000100UL
#define OS_SC_EVENT_SEND2           0x00001000UL
#define OS_SC_EVENT_RECV2           0x00002000UL

#define OS_SC_TX_LOCK()             { OS_OBTAIN_MUTEX(os_SC_MutexID_TX, MSOS_WAIT_FOREVER); }
#define OS_SC_TX_UNLOCK()           { OS_RELEASE_MUTEX(os_SC_MutexID_TX); }

#define OS_SC_SHM_LOCK()             { OS_OBTAIN_MUTEX(os_SC_MutexID_SHM, MSOS_WAIT_FOREVER); }
#define OS_SC_SHM_UNLOCK()           { OS_RELEASE_MUTEX(os_SC_MutexID_SHM); }







//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef MS_BOOL(*P_SC_ISR_Proc)(MS_U8 u8SCID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_S32   os_SC_MutexID;
extern MS_S32   os_SC_MutexID_TX;
extern MS_S32   os_SC_MutexID_SHM;

extern MS_BOOL  OS_SC_Init(MS_U8 u8SCID);
extern MS_BOOL  OS_SC_Exit(MS_U8 u8SCID);
extern void     OS_SC_ISR_Enable(MS_U8 u8SCID, P_SC_ISR_Proc pfISRProc);
extern void     OS_SC_ISR_Disable(MS_U8 u8SCID);
extern void     OS_SC_SetEvent(MS_U32 u32Events);
extern MS_U32   OS_SC_WaitEvent(void);
extern void     OS_SC_ClearEvent(MS_U32 u32Events);
extern void     OS_SC_EnterCS(void);
extern void     OS_SC_ExitCS(void);


#endif // _SC_MSOS_H_


