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
#ifndef _HALVIF_C_
#define _HALVIF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvMMIO.h"

// Internal Definition
#include "regVIF.h"
#include "VIF.h"
#include "halVIF.h"
#include "halVIF_Customer.h"
#include "asmCPU.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  extern function
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// Address bus of RIU is 16 bits.

// Standard Form

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local code data
//-------------------------------------------------------------------------------------------------

void msWriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    return;
}

void msWriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    return;
}

void msWriteByte(U32 u32Reg, U8 u8Val )
{
    return;
}

void HAL_VIF_WriteByteMask(U32 u32Reg, U8 u8Val, U8 u8Mask)
{
    return;
}

void HAL_VIF_WriteBit(U32 u32Reg, BOOL bEnable, U8 u8Mask)
{
    return;
}

void HAL_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    return;
}

U8 HAL_VIF_ReadByte(U32 u32Reg )
{
    return 0;
}

void msWriteRegsTbl(MS_VIF_REG_TYPE *pRegTable)
{
    return;
}

void HAL_VIF_RegInit (void)
{
    return;
}


void HAL_VIF_SetClock(BOOL bEnable)
{
    return;
    //bEnable = (bEnable) ? 0:1; // 0 means enable
}

void msVifAdcInitial(void)
{
    return;
}

// For API
void msVifSetIfFreq(IfFrequencyType ucIfFreq)
{
    return;
}

void msVifGroupDelayFilter(VIFSoundSystem ucSoundSystem, FrequencyBand frequencyRange)
{
    return;
}

// For API
void msVifSetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    return;
}

// For API
void msVifTopAdjust(void)
{
    return;
}

void msVifDynamicTopAdjust(void)
{
    return;
}

void msVifLoad(void)
{
    return;
}

// For API
void msVifInitial(void)
{
    return;
}

void msVifExit(void)
{
    return;
}

// For API
void msVifHandler(BOOL bVifDbbAcq)
{
    return;
}

void msVifSeriousACIDetection(void)
{
    return;
 }

void msVifCrKpKiAutoAdjust(BYTE VifCrKpKiAdjustThr1, BYTE VifCrKpKiAdjustThr2)
{
    return;
}

U8 msVifReadCRFOE(void)
{
    return 0;
}

U8 msVifReadLockStatus(void)
{
    return 0;
}

void msVifLoadEQCoeff(BYTE VifSoundStandard)
{
    return;
}

void msVifShiftClk(BYTE VifShiftClk)
{   
    return;
}

void HAL_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    return;
}

BOOL HAL_VIF_GetInputLevelIndicator(void)
{
        return 0;
}

U8 HAL_VIF_GetCrPDInverse(void)
{
        return 0;
}

void HAL_VIF_SetCrPDInverse(BOOL bEnable)
{
    return;
}

#endif //_HALVIF_C_

