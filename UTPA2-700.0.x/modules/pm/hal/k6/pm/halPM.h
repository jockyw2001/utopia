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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
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
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_PM_H_
#define _HAL_PM_H_

////////////////////////////////////////////////////////////////////////////////
/// @file halPM.h
/// @author MStar Semiconductor Inc.
/// @brief PM control hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    PM_RTCIDX_0 = 0,
    PM_RTCIDX_1 = 1,
    PM_RTCIDX_MAX = PM_RTCIDX_1,

} PM_RtcIndex;
////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_PM_ReadByte(MS_U32 u32RegAddr);
MS_U16 HAL_PM_Read2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_PM_Read4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_PM_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_PM_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_PM_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_PM_ReadRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask);
MS_BOOL HAL_PM_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8Mask);
MS_BOOL HAL_PM_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk);
void HAL_PM_SetIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_PM_GetIOMapBase(void);
//void HAL_PM_Dump_AllReg(void);
void HAL_PM_RtcInit(MS_U8 u8RtcIndex, MS_U32 u32RtcCtrlWord);
void HAL_PM_RTC_DisableInit(MS_U8 u8RtcIndex);
void HAL_PM_RtcSetCounter(MS_U8 u8RtcIndex, MS_U32 u32RtcSetCounter);
MS_U32 HAL_PM_RtcGetCounter(MS_U8 u8RtcIndex);
void HAL_PM_RtcSetMatchCounter(MS_U8 u8RtcIndex, MS_U32 u32RtcSetMatchCounter);
MS_U32 HAL_PM_RtcGetMatchCounter(MS_U8 u8RtcIndex);
void HAL_PM_SetSPIOffsetForMCU(MS_U32 BANK);
void HAL_PM_SetSRAMOffsetForMCU(void);
MS_BOOL HAL_PM_SetDRAMOffsetForMCU(MS_U32 u32Offset);
MS_U8 HAL_PM_PowerOnMode(void);
MS_U8 HAL_PM_GetWakeupSource(void);
MS_U8 HAL_PM_GetWakeupKey(void);
void HAL_PM_Disable51(void);
MS_BOOL HAL_PM_isRunning(void);
void HAL_PM_SetBackGroundTrigger(MS_BOOL bTrigger);
MS_BOOL HAL_PM_GetBackGroundActive(void);

void HAL_PM_GPIO4_SetPower(MS_BOOL bOn);

void HAL_PM_SetScratchPad(MS_U8 u8PadNum,MS_U16 u16Val);
MS_BOOL HAL_PM_GetScratchPad(MS_U8 u8PadNum,MS_U16 *u16Val);

MS_BOOL HAL_PM_GetVersion(MS_U8 *u8Val);
MS_U16 HAL_PM_GetSRAMSize(void);
MS_U8 HAL_PM_GetIRPowerOnKey(void);
void HAL_PM_SetWakupDevice(MS_U16 u16WakeSrc);
void HAL_PM_GetRT51Status(void);
#endif  //_HAL_PM_H_
