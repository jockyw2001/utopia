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
//      drvKTV.c
//  [Abstract]:
//      This module contains code for Audio Processor driver
//      procedure and subroutin
//  [Author(s)]:
//      Allen Chen
//  [Reversion History]:
//      Initial release:    15 Aprial, 2009
//
//  [Doxygen]
/// file drvKTV.c
/// @brief Driver for KTV functions
/// @author MStarSemi Inc.
//*******************************************************************************

#define _DRVKTV_C_

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
// Internal
#include "drvAUDIO.h"
#include "drvAUDIO_if.h"

#include "./internal/drvKTV.h"
#include "./internal/drvSOUND.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvAUDIO_internal.h"

#include "halAUDIO.h"
#include "halMAD2.h"
#include "halKTV.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetKTVInfo()
/// @brief \b Function \b Description: This routine is used to set KTV parameter to DSP .
/// @param <IN>        \b modelType: KTV model type (T3, U3, Janus)
/// @param <IN>        \b infoType : KTV info type
/// @param <IN>        \b param1   : KTV parameter1
/// @param <IN>        \b param2   : KTV parameter2
/// @param <OUT>       \b NONE     :
/// @param <RET>       \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    return (HAL_KTV_SetKTVInfo(modelType, paramType, param1, param2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetKTVInfo()
/// @brief \b Function \b Description: This routine is used to get KTV data from DSP .
/// @param <IN>        \b modelType: KTV model type (T3, U3, Janus)
/// @param <IN>        \b infoType : KTV info type
/// @param <OUT>       \b NONE     :
/// @param <RET>       \b MS_U32   : Return parameter
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_AUDIO_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    return (HAL_KTV_GetKTVInfo (modelType, infoType));
}

