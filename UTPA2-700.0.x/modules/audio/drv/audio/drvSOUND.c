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
//******************************************************************************
//  Copyright (c) 2008-2009 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      drvSOUND.c
//  [Abstract]:
//      This module contains code for Audio Processor driver
//      procedure and subroutin
//  [Reversion History]:
//      Initial release:    15 July, 2005
//
//  [Doxygen]
/// file drvSOUND.c
/// @brief Subroutine for sound effect
/// @author MStarSemi Inc.
//*******************************************************************************

#define _DRVSOUND_C_

// Internal
#include "drvAUDIO.h"
#include "drvAUDIO_if.h"

#include "./internal/drvSOUND.h"

#include "halSOUND.h"
#include "halAUDIO.h"

#define GET_MAP_VALUE(_X_,_Y_,_Z_)  ((MS_U16)(_X_) * (_Y_) / (_Z_))


//====================================================================

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_Init()
/// @brief \b Function \b Description: This routine is the initialization for Audio sound effect module.
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_Init(void)
{
    HAL_SOUND_Init2();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetMute()
/// @brief \b Function \b Description: This routine is used to set audio u8Path S/W mute.
/// @param u8Path        \b : for audio u8Path0 ~ u8Path6
/// @param bEnable        \b : TRUE --Mute
///                             FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetMute(MS_U8 u8Path, MS_BOOL bEnable)
{
    AUDIO_PATH_GROUP_TYPE PathGroup = AUDIO_PATH_GROUP_1;

    PathGroup = MDrv_AUDIO_GetPathGroup();

    if (((PathGroup == AUDIO_PATH_GROUP_1) && (u8Path == AUDIO_PATH_3)) ||
        ((PathGroup == AUDIO_PATH_GROUP_2) && (u8Path == AUDIO_T3_PATH_SPDIF)))
    {
        MDrv_AUDIO_SPDIF_SetMute((MS_U8)bEnable);
    }
    else
    {
        HAL_SOUND_SetMute(u8Path, bEnable);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of audio u8Path.
/// @param u8Path        \b     : for audio u8Path0 ~ u8Path6
/// @param u8Vol1        \b     :     MSB 7-bit register value of 10-bit u8Volume
///                                range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2        \b     :     LSB 3-bit register value of 10-bit u8Volume
///                                range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_AbsoluteVolume(MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2)
{
    HAL_SOUND_AbsoluteVolume(u8Path, u8Vol1, u8Vol2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetPreScale()
/// @brief \b Function \b Description: This routine is used to set the prescale of audio u8Path.
/// @param u8Path        \b     : for audio u8Path0 ~ u8Path5
/// @param u8Prescale        \b     :     range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                                                   0x00: disable pre-scale
///                                                   0x6F: gain = 0db
///                                                   0xFF: gain = +18db
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetPreScale(MS_U8 u8Path, MS_U8 u8Prescale)
{
    HAL_SOUND_SetPreScale(u8Path, u8Prescale);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetBalance()
/// @brief \b Function \b Description: This routine is used to set the balance of main u8Path.
/// @param u8Balance        \b     : balance level (from 0~100)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetBalance(MS_U8 u8Balance)
{
    HAL_SOUND_SetBalance(u8Balance);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetBalance_L()
/// @brief \b Function \b Description: This routine is used to set absolute balance value of main u8Path L-channel.
/// @param u8Balance_L        \b     : balance register value (0x00~0xFF)
///                                                 Left channel attenuation level (0.25dB/step).
///                                                 0000-0000: 0 db.
///                                                 0000-0001: -0.25 db.
///                                                 1111-1110: -63.5 db.
///                                                 1111-1111: Mute.
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetBalance_L(MS_U8 u8Balance_L)
{
    HAL_SOUND_SetBalance_L(u8Balance_L);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetBalance_R()
/// @brief \b Function \b Description: This routine is used to set absolute balance value of main u8Path R-channel.
/// @param u8Balance_R        \b     : balance register value (0x00~0xFF)
///                                                 Right channel attenuation level (0.25dB/step).
///                                                 0000-0000: 0 db.
///                                                 0000-0001: -0.25 db.
///                                                 1111-1110: -63.5 db.
///                                                 1111-1111: Mute.
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetBalance_R(MS_U8 u8Balance_R)
{
    HAL_SOUND_SetBalance_R(u8Balance_R);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetBass()
/// @brief \b Function \b Description: This routine is used to set the Bass level of main u8Path .
/// @param u8Level        \b     : Bass level (0~100) mapping to -16~+15dB
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetBass(MS_U8 u8Level)
{
    HAL_SOUND_SetBass(u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetDynamicBass()
/// @brief \b Function \b Description: This routine is used to set the DynamicBass level of main u8Path .
/// @param u8Level        \b     :
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetDynamicBass(MS_U8 u8Level)
{
    HAL_SOUND_SetDynamicBass(u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AbsoluteBass()
/// @brief \b Function \b Description: This routine is used to set the absolute Bass value of main u8Path .
/// @param u8Bass        \b     :  -16~+15dB    ;  1dB/step
///                                                     0-0000: +00db.
///                                                     0-0001: +01db.
///                                                     0-1111: +15db. (Max.)
///                                                     1-1111: -1db.
///                                                     1-1110: -2db.
///                                                     1-0000: -16db. (Min.)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_AbsoluteBass(MS_U8 u8Bass)
{
    HAL_SOUND_AbsoluteBass(u8Bass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetTreble()
/// @brief \b Function \b Description: This routine is used to set the Treble level of main u8Path.
/// @param u8Level        \b     : Treble level (0~100) mapping to -16~+15dB
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetTreble(MS_U8 u8Level)
{
    HAL_SOUND_SetTreble(u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AbsoluteTreble()
/// @brief \b Function \b Description: This routine is used to set the absolute Treble value of main u8Path .
/// @param u8Treble        \b     :  -16~+15dB    ;  1dB/step
///                                                     0-0000: +00db.
///                                                     0-0001: +01db.
///                                                     0-1111: +15db. (Max.)
///                                                     1-1111: -1db.
///                                                     1-1110: -2db.
///                                                     1-0000: -16db. (Min.)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_AbsoluteTreble(MS_U8 u8Treble)
{
    HAL_SOUND_AbsoluteTreble(u8Treble);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetEq()
/// @brief \b Function \b Description: This routine is used to set the the 5-band EQ level  .
/// @param u8Band        \b     :  EQ band 0~4
/// @param u8Level        \b     :  Absolute EQ register value  ;
///                                             0011-0000: +12.00db. (Max)
///                                             0000-0001: +0.25db.
///                                             0000-0000: +0.00db.
///                                             1111-1111: -0.25db.
///                                             1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetEq(MS_U8 u8Band, MS_U8 u8Level)
{
    HAL_SOUND_SetEq(u8Band, u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetEq7()
/// @brief \b Function \b Description: This routine is used to set the the 7-band EQ level  .
/// @param u8Band        \b     :  EQ band 0~6
/// @param u8Level        \b     :  Absolute EQ register value  ;
///                                             0011-0000: +12.00db. (Max)
///                                             0000-0001: +0.25db.
///                                             0000-0000: +0.00db.
///                                             1111-1111: -0.25db.
///                                             1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetEq7(MS_U8 u8Band, MS_U8 u8Level)
{
    HAL_SOUND_SetEq7(u8Band, u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetADCThreshold()
/// @brief \b Function \b Description: This routine is used to set the ADC input energy threshold to reduce the background noise .
/// @param u8Threshold        \b     :  NR threshold level
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetADCThreshold(MS_U8 u8Threshold)
{
    HAL_SOUND_SetADCThreshold(u8Threshold);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetAVCThreshold()
/// @brief \b Function \b Description: This routine is used to set the AVC  threshold level.
/// @param u8Level        \b     :  AVC threshold level
///                                            0x00--  0    dBFS
///                                            0x01--  -0.5    dBFS
///                                            0x20--  -16    dBFS
///                                            0x50--  -40    dBFS
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetAVCThreshold(MS_U8 u8Level)
{
    HAL_SOUND_SetAVCThreshold(u8Level);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetAvcMode()
/// @brief \b Function \b Description: This routine is used to set the AVC  u8Mode.
/// @param u8AvcMode        \b     :  AVC threshold level
///                                                 0: L u8Mode
///                                                 1: S u8Mode
///                                                 2: M u8Mode
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetAvcMode(MS_U8 u8AvcMode)
{
    HAL_SOUND_SetAvcMode(u8AvcMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetAvcAT()
/// @brief \b Function \b Description: This routine is used to set the AVC  attack time .
/// @param u8AvcAT        \b     :  AVC attack time
///                                             AvcAT = 0 --> 20  ms
///                                             AvcAT = 1 --> 100 ms
///                                             AvcAT = 2 --> 200 ms
///                                             AvcAT = 3 --> 1 sec
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetAvcAT(MS_U8 u8AvcAT)
{
    HAL_SOUND_SetAvcAT(u8AvcAT);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetAvcRT()
/// @brief \b Function \b Description: This routine is used to set the AVC  release time .
/// @param u8AvcRT        \b     :  AVC release time
///                                             AvcRT = 0 --> 1 sec
///                                             AvcRT = 1 --> 2 sec
///                                             AvcRT = 2 --> 100 ms
///                                             AvcRT = 3 --> 200 ms
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetAvcRT(MS_U8 u8AvcRT )
{
    HAL_SOUND_SetAvcRT(u8AvcRT );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableEQ()
/// @brief \b Function \b Description: This routine is used to bEnable/disable EQ featue.
/// @param bEnable        \b     :     TRUE --Enable EQ
///                                               FALSE--Disable EQ
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnableEQ(MS_BOOL bEnable)
{
    HAL_SOUND_EnableEQ(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableSurround()
/// @brief \b Function \b Description: This routine is used to bEnable/disable Surround featue.
/// @param bEnable        \b     :     TRUE --Enable Surround
///                                               FALSE--Disable Surround
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnableSurround(MS_BOOL bEnable)
{
    HAL_SOUND_EnableSurround(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableTone()
/// @brief \b Function \b Description: This routine is used to bEnable/disable the treble & Bass featue.
/// @param bEnable        \b     :     TRUE --Enable Tone
///                                                FALSE--Disable Tone
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnableTone(MS_BOOL bEnable)
{
    HAL_SOUND_EnableTone(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableAutoVolume()
/// @brief \b Function \b Description: This routine is used to bEnable/disable the AVC featue.
/// @param bEnable        \b     :     TRUE --Enable AutoVolume
///                                               FALSE--Disable AutoVolume
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnableAutoVolume(MS_BOOL bEnable)
{
    HAL_SOUND_EnableAutoVolume(bEnable);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableNR()
/// @brief \b Function \b Description: This routine is used to bEnable/disable the Noise Reduction featue.
/// @param u8Level        \b     :     TRUE --Enable NR
///                                               FALSE--Disable NR
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnableNR(MS_U8 u8Level)
{
    HAL_SOUND_EnableNR(u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableBalance()
/// @brief \b Function \b Description: This routine is used to bEnable/disable Balance featue.
/// @param u8Enable        \b     :     Balanc Enable bits
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnableBalance(MS_U8 u8Enable)
{
    HAL_SOUND_EnableBalance(u8Enable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetNRAttenuate()
/// @brief \b Function \b Description: This routine is used to set the attenuate level while NR is bEnabled.
/// @param u8Mode        \b     :        0 :    Auto fading to -24 dB
///                                                  1 :    Auto fading to -24 dB
///                                                  2 :    Auto fading to -24 dB
///                                                  3 :    Auto fading to -24 dB
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetNRAttenuate(MS_U8 u8Mode)
{
    u8Mode = u8Mode;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_PowerDown_Wait()
/// @brief \b Function \b Description: This routine is used to DSP power-down wait function.
/// @param bEnable     \b : TRUE --Not wait
///                         FALSE--wait
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_PowerDown_Wait(MS_BOOL bEnable)
{
    HAL_SOUND_PowerDown_Wait(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetAdAbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of AD.
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetAdAbsoluteVolume(MS_U8 u8Vol1, MS_U8 u8Vol2)
{
    HAL_SOUND_SetAdAbsoluteVolume(u8Vol1, u8Vol2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetAdMute()
/// @brief \b Function \b Description: This routine is used to set AD mute.
/// @param bEnable        \b     :     TRUE --Mute AD
///                                               FALSE--Unmute AD
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetAdMute(MS_BOOL bEnable)
{
    HAL_SOUND_SetAdMute(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetCH1AudioDelay()
/// @brief \b Function \b Description: This routine is used to set the value of audio delay.
/// @param u8Delay       \b     :     0x14~0xC8(20~200), in ms unit
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetCH1AudioDelay(MS_U8 u8Delay)
{
    HAL_SOUND_SetCH1AudioDelay(u8Delay);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetMenuSound()
/// @brief \b Function \b Description: This routine is used to Play Menu Sound.
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_PlayMenuSound(void)
{
    HAL_SOUND_PlayMenuSound();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetSurroundXA()
/// @brief \b Function \b Description: This routine is used to set the Surround xA Gain.
/// @param u8Mode      \b     :        0 :    0.1         xA : 0x2D8A[3:2] 00: 0.1
///                                                  1 :    0.15                         01: 0.15
///                                                  2 :    0.2                          10: 0.2
///                                                  3 :    0.25                         11: 0.25
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetSurroundXA(MS_U8 u8Mode)
{
    HAL_SOUND_SetSurroundXA(u8Mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetSurroundXB()
/// @brief \b Function \b Description: This routine is used to set the Surround xB Gain.
/// @param u8Mode      \b :  0 :    0.25         xB : 0x2D8A[5:4] 00: 0.25
///                          1 :    0.3                           01: 0.3
///                          2 :    0.35                          10: 0.35
///                          3 :    0.45                          11: 0.45
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetSurroundXB(MS_U8 u8Mode)
{
    HAL_SOUND_SetSurroundXB(u8Mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetSurroundXK()
/// @brief \b Function \b Description: This routine is used to set the Surround xK Gain.
/// @param u8Mode      \b :  0 :    0.1         xK : 0x2D8A[7:6]  00: 0.1
///                                        1 :    0.2                           01: 0.2
///                                        2 :    0.3                           10: 0.3
///                                        3 :    0.4                           11: 0.4
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetSurroundXK(MS_U8 u8Mode)
{
    HAL_SOUND_SetSurroundXK(u8Mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetSurroundLPFGain()
/// @brief \b Function \b Description: This routine is used to set the Surround LPF Gain.
/// @param u8Mode      \b :            0 :    0 dB        LPF Gain : 0x2DA0[7:6]  00: 0 dB
///                                                  1 :    2 dB                                01: 2 dB
///                                                  2 :    4 dB                                10: 4 dB
///                                                  3 :    un-support                          11: un-support
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetSurroundLPFGain(MS_U8 u8Mode)
{
    HAL_SOUND_SetSurroundLPFGain(u8Mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetPEQCoef()
/// @brief \b Function \b Description: This routine is used to set PEQ Coefficient.
/// @param <IN>        \b peq_coef
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
{
    HAL_SOUND_SetPEQCoef(peq_coef);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnablePEQ()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b Enpeq
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_EnablePEQ(MS_BOOL Enpeq)
{
    HAL_SOUND_EnablePEQ(Enpeq);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_EnableDcRemove()
/// @brief \b Function \b Description: This routine is used to eable DC remove.
/// @param <IN>        \b EnDcRemove
/// @param <OUT>       \b True / False
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SOUND_EnableDcRemove(MS_BOOL EnDcRemove)
{
    return HAL_SOUND_EnableDcRemove(EnDcRemove);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetMixModeMute()
/// @brief \b Function \b Description: This routine is used to set audio mix mode mute
/// @param eSourceType    \b : for audio source
/// @param VolType        \b : for audio vol type
/// @param bEnable        \b : TRUE --Mute
///                            FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute )
{
    HAL_SOUND_SetMixModeMute(eSourceType, VolType, EnMute);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetMixModeVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of audio mix mode
/// @param eSourceType   \b     : for audio source
/// @param VolType       \b     : for audio vol type
/// @param u8Vol1        \b     :     MSB 7-bit register value of 10-bit u8Volume
///                                range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2        \b     :     LSB 3-bit register value of 10-bit u8Volume
///                                range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  )
{
    HAL_SOUND_SetMixModeVolume(eSourceType, VolType, u8Vol1, u8Vol2);
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MDrv_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param )
{
    MS_BOOL RET = FALSE;
    MS_U8 param1 = (MS_U8)(p_param[0]);
    MS_U8 param2 = (MS_U8)(p_param[1]);
    AUDIO_PATH_GROUP_TYPE PathGroup = AUDIO_PATH_GROUP_1;

    PathGroup = MDrv_AUDIO_GetPathGroup();

    if ((Type == Sound_SET_Type_SetMute) &&
        (((PathGroup == AUDIO_PATH_GROUP_1) && (param1 == AUDIO_PATH_3)) ||
        ((PathGroup == AUDIO_PATH_GROUP_2) && (param1 == AUDIO_T3_PATH_SPDIF))))
    {
        MDrv_AUDIO_SPDIF_SetMute(param2);
        RET= TRUE;
    }
    else
    {
        RET =  HAL_SOUND_SetParam(Type, p_param);
    }

    return  RET;
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_SND_ProcessEnable()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
//******************************************************************************
MS_BOOL MDrv_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
{
    return(HAL_SND_ProcessEnable(Type, enable));
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_SND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MDrv_SND_SetParam( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
{
    return(HAL_SND_SetParam(Type,  param1,  param2));
}


//******************************************************************************
/// @brief \b Function \b Name: MDrv_SOUND_GetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_U16 MDrv_SND_GetParam( Sound_GET_PARAM_Type Type, MS_U16 param1)
{
    return HAL_SND_GetParam(Type, param1);
}


