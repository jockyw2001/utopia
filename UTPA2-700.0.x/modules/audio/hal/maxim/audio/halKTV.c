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
//  Copyright (c) 2003-2005 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      halKTV.c
//  [Abstract]:
//      This module contains code for Audio Processor driver
//      procedure and subroutin
//  [Author(s)]:
//      Desker Chuo
//  [Reversion History]:
//      Initial release:    15 July, 2005
//
//  [Doxygen]
/// @file DrvAuProcess.h
/// @brief Subroutine for sound effect
/// @author MStarSemi Inc.
///
/// This module contains code for Audio Processor driver
/// procedure and subroutine.
///
///@par Example
///@code
///  // Audio set mute
///  // Input: enable/disable
///  void msAPI_AUD_SetMute(BOOLEAN mute_en)
///  {
///      // Set mute enable for audio path 0
///      HAL_AUDIO_ProcessSetMute( AUDIO_PATH_0, mute_en );
///     MDrv_VAmp_Mute(mute_en);
///  }
///@endcode
//*******************************************************************************

#define _HALKTV_C_

// Internal
#include "halAUDIO.h"
#include "halAUR2.h"
#include "halKTV.h"

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_KTV_SetKTVInfo()
/// @brief \b Function \b Description: This routine set different paramter to ktv module
/// @param <IN>        \b AUDIO_KTV_MODEL    : common ktv module type
/// @param <IN>        \b AUDIO_KTV_PARAMTYPE: common kt  parameter type
/// @param <IN>        \b MS_U32    : common ktv parameter 1
/// @param <IN>        \b MS_U32    : common ktv parameter 2
/// @param <OUT>       \b NONE      :
/// @param <RET>       \b MS_BOOL   : TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE      :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_KTV_SetKTVInfo(AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    switch (modelType)
    {
    /* For T12 Multak KTV */
    case AUD_KTV_MODEL2: 
        switch (paramType)
        {
        case AUD_KTV_ParamType1:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_CUSFUNC_xxxx, AASE1, (MS_U16)param1, (MS_U16)param2);
            break;        
        case AUD_KTV_ParamType2:
            HAL_SND_R2_SetCommInfo(AUR2_CommParam_AASE_CUSFUNC_xxxx2, AASE1, (MS_U16)param1, (MS_U16)param2);
            break;
            
        default:
            break;
        }
        break;
        
    default:
        break;
    }
    
    return (TRUE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_KTV_GetKTVInfo()
/// @brief \b Function \b Description: This routine is used to get KTV data from DSP .
/// @param <IN>        \b modelType: KTV model type 
/// @param <IN>        \b infoType : KTV info type
/// @param <OUT>       \b NONE     :
/// @param <RET>       \b MS_U32   : Return parameter
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_KTV_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    MS_U32 result = FALSE;

    switch (modelType)
    {
    /* For T12 Multak KTV */
    case AUD_KTV_MODEL2:
        switch (infoType)
        {
        case AUD_KTV_infoType1:
            result = HAL_SND_R2_GetCommInfo(AUR2_CommInfo_AASE_CUSFUNC_xxxx, AASE1, 0, 0);
            break;        
            
        case AUD_KTV_infoType2:
            result = HAL_SND_R2_GetCommInfo(AUR2_CommInfo_AASE_CUSFUNC_xxxx2, AASE1, 0, 0);
            break;        
            
        default:
            break;
        }
        break;

    default:
        break;
    }

    return (result);
}

