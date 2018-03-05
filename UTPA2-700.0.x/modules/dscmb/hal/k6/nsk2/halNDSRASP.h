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
// file   halRASP.h
// @brief  RASP HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_NDSRASP_H__
#define __HAL_NDSRASP_H__

#include "regNDSRASP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  RASP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

// rasp define
#define RASP_NUM 2

#define RASP_DBGLV_EMERG     0  //Used for emergency messages, usually those that precede a crash.
#define RASP_DBGLV_ALERT     1  //A situation requiring immediate action.
#define RASP_DBGLV_CRIT      2  //Critical conditions, often related to serious hardware or software failures.
#define RASP_DBGLV_ERR       3  //Used to report error conditions; device drivers often use KERN_ERR to report hardware difficulties.
#define RASP_DBGLV_WARNING   4  //Warnings about problematic situations that do not, in themselves, create serious problems with the system.
#define RASP_DBGLV_NOTICE    5  //Situations that are normal, but still worthy of note. A number of security-related conditions are reported at this level.
#define RASP_DBGLV_INFO      6  //Informational messages. Many drivers print information about the hardware they find at startup time at this level.
#define RASP_DBGLV_DEBUG     7  //Used for debugging messages.
#define RASP_DBGLV_ARRAY     8  //Uesd for array data.

// TSP Register
#define _TspPid                     ((REG_Pid*)(REG_PIDFLT_BASE))
#define _TspSec                     ((REG_Sec*)(REG_SECFLT_BASE))

// HW characteristic
#define MIU(_addr_)                    ((_addr_)>>4)

// HW characteristic
#define MIUNDS(_addr_)                 ((_addr_)>>3)

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))

typedef enum
{
    RASP_SRC_TS0,
    RASP_SRC_TS1,
    RASP_SRC_TS2,
    RASP_SRC_TS3,
    RASP_SRC_TS4,
    RASP_SRC_TS5,
    RASP_SRC_TS6,
    RASP_SRC_DEMOD0,
    RASP_SRC_DEMOD1,
} RASP_LIVEIN_SRC_e;

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_NDSRASP_SetBank(MS_U32 u32BankAddr);
void HAL_NDSRASP_Init(MS_U32 u32RASPEng);
void HAL_NDSRASP_Exit(MS_U32 u32RASPEng);
void HAL_NDSRASP_Ctrl(MS_U32 u32RASPEng, MS_U16 u16FromTo, MS_U16 u16PktSize2);
void HAL_NDSRASP_Pvr_Enable(MS_U32 u32RASPEng);
void HAL_NDSRASP_Stop(MS_U32 u32RASPEng);
void HAL_NDSRASP_Pause(MS_U32 u32RASPEng , MS_BOOL bPause);
void HAL_NDSRASP_SWReset(MS_U32 u32RASPEng);
void HAL_NDSRASP_SetPidflt(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid);
void HAL_NDSRASP_GetPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 *u16Pid);
void HAL_NDSRASP_SetEcmPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 u16Pid);
void HAL_NDSRASP_SetECM_Init_1(MS_U32 u32RASPEng);
void HAL_NDSRASP_SetECM_Init_2(MS_U32 u32RASPEng);
MS_U16 HAL_NDSRASP_SetECM_GetHW(MS_U32 u32RASPEng);
void HAL_NDSRASP_SetECM_ResetHW(MS_U32 u32RASPEng, MS_U16 IntStatus);
void HAL_NDSRASP_SetEventMask(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Mask);
void HAL_NDSRASP_SetPayloadMask(MS_U32 u32RASPEng, MS_U16 u16Flt, MS_U32 u32Mask);
void HAL_NDSRASP_SetPayloadPacketMode(MS_U32 u32RASPEng,MS_BOOL bDisable192);
void HAL_NDSRASP_SetECMPacketMode(MS_U32 u32RASPEng,MS_BOOL bDisable192);

void HAL_NDSRASP_SetCorptData(MS_U32 u32RASPEng, MS_U16 u16From , MS_U16 u16To, MS_U8 u8Data);
void HAL_NDSRASP_SetCorptFlt(MS_U32 u32RASPEng , MS_U16 u16Fltid, MS_BOOL Enable);

void HAL_NDSRASP_SetStr2Miu_StartAddr(MS_U32 u32RASPEng, MS_U32 u32StartAddr0, MS_U32 u32StartAddr1);
void HAL_NDSRASP_SetStr2Miu_MidAddr(MS_U32 u32RASPEng, MS_U32 u32MidAddr0, MS_U32 u32MidAddr1);
void HAL_NDSRASP_SetStr2Miu_EndAddr(MS_U32 u32RASPEng, MS_U32 u32EndAddr0, MS_U32 u32EndAddr1);

void HAL_NDSRASP_Payload_Enable(MS_U32 u32RASPEng ,MS_BOOL enable );
//void HAL_NDSRASP_SetPayload_StartAddr(MS_U32 u32RASPEng, MS_U32 u32StartAddr0, MS_U32 u32StartAddr1);
//void HAL_NDSRASP_SetPayload_MidAddr(MS_U32 u32RASPEng, MS_U32 u32MidAddr0, MS_U32 u32MidAddr1);
//void HAL_NDSRASP_SetPayload_EndAddr(MS_U32 u32RASPEng, MS_U32 u32EndAddr0, MS_U32 u32EndAddr1);
MS_U32 HAL_NDSRASP_GetPayloadWritePtr(MS_U32 u32RASPEng);

void HAL_NDSRASP_ECM_Enable(MS_U32 u32RASPEng , MS_BOOL enable );
//void HAL_NDSRASP_SetECM_StartAddr(MS_U32 u32RASPEng, MS_U32 u32StartAddr);
//void HAL_NDSRASP_SetECM_MidAddr(MS_U32 u32RASPEng, MS_U32 u32MidAddr);
//void HAL_NDSRASP_SetECM_EndAddr(MS_U32 u32RASPEng, MS_U32 u32EndAddr);

//void HAL_NDSRASP_SetECM_Pidflt(MS_U32 u32RASPEng, MS_U16 u16Fltid, MS_U16 u16Pid);
void HAL_NDSRASP_SetStrPacketMode(MS_U32 u32RASPEng, MS_BOOL bSet);
void HAL_NDSRASP_PowerCtrl(MS_U32 u32RASPEng, MS_BOOL bSet);

void HAL_NDSRASP_SetPVRTimeStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp);
void HAL_NDSRASP_SetECMTimeStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp);
void HAL_NDSRASP_SetPayloadTimeStamp(MS_U32 u32RASPEng, MS_U32 u32Stamp);
//void HAL_NDSRASP_SetTimerWaterMark(MS_U32 u32RASPEng, MS_U16 u16TimerWaterMask);
//void HAL_NDSRASP_SetEventWaterMark(MS_U32 u32RASPEng, MS_U16 u16EventWaterMask);

void HAL_NDSRASP_Rec_PID(MS_U32 u32RASPEng, MS_BOOL bSet);

MS_U32 HAL_NDSRASP_GetPVRTimeStamp(MS_U32 u32RASPEng);
MS_U32 HAL_NDSRASP_GetECMTimeStamp(MS_U32 u32RASPEng);
MS_U32 HAL_NDSRASP_GetPayloadTimeStamp(MS_U32 u32RASPEng);


MS_U32 HAL_NDSRASP_GetWritePtr(MS_U32 u32RASPEng);
MS_U32 HAL_NDSRASP_GetECMWritePtr(MS_U32 u32RASPEng);
void HAL_NDSRASP_SetFileinStart(MS_U32 u32RASPEng, MS_BOOL bEn);
MS_BOOL HAL_NDSRASP_GetFileinStart(MS_U32 u32RASPEng);

void HAL_NDSRASP_SetTSIF(MS_U32 u32RASPEng, MS_BOOL bPara, MS_BOOL bExtSync, MS_BOOL bDataSWP);
void HAL_NDSRASP_GetTSIF(MS_U32 u32RASPEng , MS_BOOL* pbPara, MS_BOOL* pbExtSync, MS_BOOL* pbDataSWP);



MS_U16  HAL_NDSRASP_INT_GetHW(MS_U32 u32RASPEng);
void    HAL_NDSRASP_INT_ClrHW(MS_U32 u32RASPEng, MS_U16 u16Mask);
void    HAL_NDSRASP_INT_Enable(MS_U32 u32RASPEng, MS_U16 u16Mask);
void    HAL_NDSRASP_INT_Disable(MS_U32 u32RASPEng, MS_U16 u16Mask);
void    HAL_NDSRASP_EVENT_EventWM_Enable(MS_U32 u32RASPEng, MS_BOOL enable);
void    HAL_NDSRASP_EVENT_TimeWM_Enable(MS_U32 u32RASPEng, MS_BOOL enable);
void    HAL_NDSRASP_SetDataSwap(MS_U32 u32RASPEng, MS_BOOL bEn);
MS_BOOL HAL_NDSRASP_EVENT_GetEventDescriptor(MS_U32 u32RASPEng,MS_U32 *DataArray , MS_U32 ArrSize );
MS_U16  HAL_NDSRASP_EVENT_GetEventNum(MS_U32 u32RASPEng);
void HAL_NDSRASP_SetBufferBlockLevel(MS_U32 u32RASPEng, MS_U32 bufBlockLv);
MS_BOOL HAL_NDSRASP_EVENT_RstCounter(MS_U32 u32RASPEng);
MS_BOOL	HAL_NDSRASP_EVENT_RstTimer(MS_U32 u32RASPEng);
MS_BOOL HAL_NDSRASP_EVENT_EFrame_Read(MS_U32 u32RASPEng);


MS_U32 HAL_NDSRASP_GetPktTimer(MS_U32 u32RASPEng);
MS_U32 HAL_NDSRASP_GetPktNum(MS_U32 u32RASPEng);
MS_U32 HAL_NDSRASP_GetEcmPktNum(MS_U32 u32RASPEng);

MS_U32 HAL_RASP_SetPVRDstBuf(MS_U32 RaspEng, MS_U32 u32StartAddr0, MS_U32 u32EndAddr0, MS_U32 u32StartAddr1, MS_U32 u32EndAddr1);
MS_U32 HAL_RASP_RecOpenPid(MS_U32 RaspEng, MS_U16 Pid, MS_U8 RaspFltId);
MS_U32 HAL_RASP_DisableRec(MS_U32 RaspEng);
MS_U32 HAL_RASP_Close(MS_U32 RaspEng);

MS_U32 HAL_RASP_SetFileIn_Config(MS_U32 RaspEng, MS_U32 StartAddr, MS_U32 FileInSize);
MS_U32 HAL_RASP_FileIn_Start(MS_U32 RaspEng);
MS_U32 HAL_RASP_SetFileIn_Timer(MS_U32 RaspEng, MS_U16 u16Timer);
MS_U32 HAL_RASP_SetFileIn_PktSize(MS_U32 RaspEng, MS_U16 PktSize);
MS_BOOL HAL_RASP_IsFileIn_Done(MS_U32 RaspEng);
MS_U32 HAL_RASP_FileIn_Flush(MS_U32 RaspEng);

MS_U32 HAL_RASP_FileIn_BlockTimeStamp(MS_U32 RaspEng,MS_BOOL enbl);
MS_U32 HAL_RASP_FileIn_SetPlaybackTimeStamp(MS_U32 RaspEng,MS_U32 u32Stamp);
MS_U32 HAL_RASP_FileIn_GetPlaybackTimeStamp(MS_U32 RaspEng);
MS_U32 HAL_RASP_FileIn_Timer(MS_U32 RaspEng,MS_BOOL bEnFileinTimer,MS_U16 u16TimerValue);
MS_U32 HAL_RASP_FileIn_Reset_TStamp(MS_U32 RaspEng,MS_BOOL bResetPktTimeStamp);

MS_U32 HAL_RASP_SetPayloadDstBuf(MS_U32 RaspEng,MS_U32 u32StartAddr0,MS_U32 u32MidAddr0,MS_U32 u32EndAddr0,MS_U32 u32StartAddr1,MS_U32 u32MidAddr1,MS_U32 u32EndAddr1);
MS_U32 HAL_RASP_SetECMDstBuf(MS_U32 RaspEng,MS_U32 u32StartAddr0,MS_U32 u32MidAddr0,MS_U32 u32EndAddr0);
MS_U32 HAL_NDSRASP_GetEVENT_Status(MS_U32 u32RASPEng, MS_U16 *pEventLog , MS_U32 *pEventDescr);
void HAL_NDSRASP_SetDbgLevel(MS_U32 level);
void HAL_NDSRASP_ReadEcmPidflt(MS_U32 u32RASPEng , MS_U16 u16Fltid , MS_U16 *pu16Pid);

MS_BOOL HAL_RASP_FileIn_IsCMDQ_Full(MS_U32 RaspEng);
MS_BOOL HAL_RASP_FileIn_IsCMDQ_Empty(MS_U32 RaspEng);
MS_BOOL HAL_RASP_FileIn_GetCmdQueueLevel(MS_U32 RaspEng,MS_U8 * CmdQLvl);
MS_BOOL HAL_RASP_FileIn_GetEmptyNum(MS_U32 RaspEng,MS_U8 * CmdQLvl);
MS_BOOL HAL_NDSRASP_Livein_Config(MS_U32 rasp_eng, RASP_LIVEIN_SRC_e InputSrc);
MS_BOOL HAL_NDSRASP_LiveIn_Init(MS_U32 u32RASPEng,MS_U8 u8PacketLength);
MS_BOOL HAL_NDSRASP_FileIn_Init(MS_U32 u32RASPEng,MS_U8 u8PacketLength);
MS_BOOL HAL_NDSRASP_SetFileinConfig(MS_U32 RaspEng, MS_BOOL bFileIn, MS_U16 BlockLevel);


MS_BOOL HAL_NDSRASP_EVENT_SetEventThreshold(MS_U32 u32RASPEng,MS_U32 u32threshold);
MS_BOOL HAL_NDSRASP_EVENT_SetTimerThreshold(MS_U32 u32RASPEng,MS_U16 u16timeout);
MS_U32 HAL_NDSRASP_SetExtendConfig(MS_U32 RaspEng, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte);

#endif // #ifndef __HAL_NDSRASP_H__

