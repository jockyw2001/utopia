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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
// file    halOTV.h
// @brief  OTV HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_OTV_H__
#define __HAL_OTV_H__


//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  OTV Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

// OTV define

//#########################################################################
//#### Hardware Capability Macro Start
//#########################################################################

#define OTV_ENG_NUM                 4
#define OTV_PIDFLT_START_NUM        0
#define OTV_PIDFLT_END_NUM          4
#define OTV_PIDFLT_NUM_ALL          4
#define OTV_EVENT_FIFO_DEPTH        32


//#########################################################################
//#### Hardware Capability Macro End
//#########################################################################


#define OTV_PID_NULL                0x1FFF


// HW characteristic
#define MIU(_addr_)                 ((_addr_)>>4)

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

#if 0
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))
#endif

#define _SET_(flag, bit)            ((flag) |   (bit) )
#define _CLR_(flag, bit)            ((flag) & (~(bit)))


////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
// For MISC part
//--------------------------------------------------------------------------------------------------
void HAL_OTV_SetBank(MS_VIRT u32BankAddr);

// OTV Init
void HAL_OTV_Init(MS_U32 u32OTVEng);

// OTV Config Setting
void HAL_OTV_PktTimerSrcFromPVR_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable);
void HAL_OTV_PktNumSrcFromPktDemux_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable);
//void HAL_OTV_FirstPktTimerBase_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable);


// OTV Interrupt
void HAL_OTV_INT_Timer_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable);
void HAL_OTV_INT_Event_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable);
void HAL_OTV_INT_SetEventWaterLevel(MS_U32 u32OTVEng, MS_U32 u32EventWaterLevel);
void HAL_OTV_INT_SetTimerWaterLevel(MS_U32 u32OTVEng, MS_U32 u32TimerWaterLevel);
MS_BOOL HAL_OTV_INT_GetEventHW(MS_U32 u32OTVEng);
MS_BOOL HAL_OTV_INT_GetTimerHW(MS_U32 u32OTVEng);
void HAL_OTV_INT_ClrEventHW(MS_U32 u32OTVEng);
void HAL_OTV_INT_ClrTimerHW(MS_U32 u32OTVEng);

// OTV Event Queue
void HAL_OTV_EventQ_Enable(MS_U32 u32OTVEng, MS_BOOL bEnable);
void HAL_OTV_EventQ_GetEventNum(MS_U32 u32OTVEng, MS_U32 *pu32EventNum);
MS_BOOL HAL_OTV_EventQ_PopEvent(MS_U32 u32OTVEng);
void HAL_OTV_EventQ_GetEventDesciptor(MS_U32 u32OTVEng, MS_U32 *pu32EventDesciptor);
void HAL_OTV_EventQ_GetEventPktNum(MS_U32 u32OTVEng, MS_U32 *pu32EventPktNum);
void HAL_OTV_EventQ_GetEventPktTimer(MS_U32 u32OTVEng, MS_U32 *pu32EventPktTimer);
void HAL_OTV_EventQ_GetEventPktPCR(MS_U32 u32OTVEng, MS_U32 *pu32EventPktPCR);
void HAL_OTV_EventQ_GetEventPktPID(MS_U32 u32OTVEng, MS_U16 *pu16EventPktPID);


// Get OTV Current Packet Number/TimeStamp
void HAL_OTV_GetCurPktTimer(MS_U32 u32OTVEng, MS_U32 *pu32PktTimer);
void HAL_OTV_GetCurPktNum(MS_U32 u32OTVEng, MS_U32 *pu32PktNum);

// OTV Pid Filter
void HAL_OTV_SetFltPid(MS_U32 u32OTVEng , MS_U32 u32FltId , MS_U16 u16Pid);
void HAL_OTV_GetFltPid(MS_U32 u32OTVEng , MS_U32 u32FltId , MS_U16 *pu16Pid);
void HAL_OTV_SetFltEvent(MS_U32 u32OTVEng , MS_U32 u32FltId, MS_U32 u32FltEvent);
void HAL_OTV_GetFltEvent(MS_U32 u32OTVEng , MS_U32 u32FltId, MS_U32 *pu32FltEvent);
void HAL_OTV_RstFltEvent(MS_U32 u32OTVEng);




#endif // #ifndef __HAL_OTV_H__
