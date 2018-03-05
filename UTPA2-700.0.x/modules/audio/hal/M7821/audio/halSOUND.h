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
// reverse bEngineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _DEV_SOUND_H_
#define _DEV_SOUND_H_

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    HAL_SOUND_Init(void);
void    HAL_SOUND_Init2(void);
void    HAL_SOUND_SetMute(MS_U8 u8Path, MS_BOOL bEnable);
void    HAL_SOUND_AbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2);
void    HAL_SOUND_SetPreScale(MS_U8 u8Path, MS_U8 u8Prescale);
void    HAL_SOUND_SetBalance(MS_U8 u8Balance);
void    HAL_SOUND_SetBalance_L(MS_U8 u8Balance_L);
void    HAL_SOUND_SetBalance_R(MS_U8 u8Balance_R);
void    HAL_SOUND_SetBass(MS_U8 u8Level);
void    HAL_SOUND_SetDynamicBass(MS_U8 u8Level);
void    HAL_SOUND_AbsoluteBass(MS_U8 u8Bass);
void    HAL_SOUND_SetTreble(MS_U8 u8Level);
void    HAL_SOUND_AbsoluteTreble(MS_U8 u8Treble);
void    HAL_SOUND_SetEq(MS_U8 band, MS_U8 u8Level);
void    HAL_SOUND_SetEq7(MS_U8 band, MS_U8 u8Level);
void    HAL_SOUND_SetADCThreshold(MS_U8 u8Threshold);
void    HAL_SOUND_SetNRAttenuate(MS_U8 u8Mode);
void    HAL_SOUND_SetDRCThreshold(MS_U8 u8Level);
void    HAL_SOUND_SetDRCEntry(MS_U8 u8Mode);
void    HAL_SOUND_SetAVCThreshold(MS_U8 u8Level);
void    HAL_SOUND_SetAvcMode(MS_U8 u8AvcMode );
void    HAL_SOUND_SetAvcAT(MS_U8 u8AvcAT );
void    HAL_SOUND_SetAvcRT(MS_U8 u8AvcRT );
void    HAL_SOUND_SetSurroundXA(MS_U8 u8Mode);
void    HAL_SOUND_SetSurroundXB(MS_U8 u8Mode);
void    HAL_SOUND_SetSurroundXK(MS_U8 u8Mode);
void    HAL_SOUND_SetSurroundLPFGain(MS_U8 u8Mode);

void    HAL_SOUND_PowerDown_Wait(MS_BOOL bEnable);
void    HAL_SOUND_SetCH1AudioDelay(MS_U16 u16Delay);
void    HAL_SOUND_SetSpdifAudioDelay(MS_U16 u16Delay);
void    HAL_SOUND_PlayMenuSound(void);

MS_U16  HAL_SOUND_GetPreScale(MS_U8 u8Path);
MS_U16  HAL_SOUND_GetEq(MS_U8 u8Band);
MS_U16  HAL_SOUND_GetBalance_L(void);
MS_U16  HAL_SOUND_GetBalance_R(void);
MS_U16  HAL_SOUND_GetSurroundXA(void);
MS_U16  HAL_SOUND_GetSurroundXB(void);
MS_U16  HAL_SOUND_GetSurroundXK(void);
MS_U16  HAL_SOUND_GetSurroundLPFGain(void);
MS_U16  HAL_SOUND_GetTreble(void);
MS_U16  HAL_SOUND_GetBass(void);
MS_U16  HAL_SOUND_GetAvcMode(void);
MS_U16  HAL_SOUND_GetDRCThreshold(void);
MS_U16  HAL_SOUND_GetAVCThreshold(void);
MS_U16  HAL_SOUND_GetAvcAT(void);
MS_U16  HAL_SOUND_GetAvcRT(void);
MS_U16  HAL_SOUND_GetPEQ_Status(void);
MS_U16  HAL_SOUND_GetHPF_Status(void);
MS_U16  HAL_SOUND_GetEQ_Status(void);
MS_U16  HAL_SOUND_GetSurround_Status(void);
MS_U16  HAL_SOUND_GetTone_Status(void);
MS_U16  HAL_SOUND_GetAutoVolume_Status(void);

MS_U16  HAL_SOUND_GetCH1AudioDelay(void);
//======================================================
void    HAL_SOUND_EnablePEQ(MS_BOOL Enpeq);
void    HAL_SOUND_EnableHPF(MS_BOOL bEnable);
MS_BOOL    HAL_SOUND_EnableDcRemove(MS_BOOL EnDcRemove);
void    HAL_SOUND_EnableKTVEcho(MS_BOOL bEnKTVEcho);
void    HAL_SOUND_EnableEQ(MS_BOOL bEnEQ);
void    HAL_SOUND_EnableSurround(MS_BOOL bEnSurround);
void    HAL_SOUND_EnableTone(MS_BOOL bEnTone);
void    HAL_SOUND_EnableAutoVolume(MS_BOOL bEnAVC);
void    HAL_SOUND_EnableDRC(MS_BOOL bEnDRC);
void    HAL_SOUND_EnableNR(MS_U8 u8Level);
void    HAL_SOUND_EnableBalance(MS_U8 u8Enable);
void    HAL_SOUND_EnableEaseVol(MS_U8 u8Enable);
//================From DrvAudioProcess=======================
void    HAL_SOUND_ReLoadCode(MS_U8 u8Type);

//==========================================================
void    HAL_SOUND_SetAdAbsoluteVolume(MS_U8 u8Vol1, MS_U8 u8Vol2);
void    HAL_SOUND_SetAdMute(MS_BOOL bEnable);
void    HAL_SOUND_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef);
void    HAL_SOUND_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  );
void    HAL_SOUND_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute );
MS_BOOL HAL_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param );
MS_BOOL HAL_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable);
MS_BOOL HAL_SND_SetParam( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2);
MS_U16  HAL_SND_GetParam( Sound_GET_PARAM_Type Type, MS_U16 param1);

#endif  //_DEV_SOUND_H_
