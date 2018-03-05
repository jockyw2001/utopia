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
#define _HAL_RTC_C

////////////////////////////////////////////////////////////////////////////////
/// @file halWDT.h
/// @author MStar Semiconductor Inc.
/// @brief Piu Watch Dog Timer hal
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Include Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "halRTC.h"
#include "regRTC.h"

////////////////////////////////////////////////////////////////////////////////
//  Driver Compiler Options
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Global Variables
////////////////////////////////////////////////////////////////////////////////
static  MS_VIRT  _gMIO_MapBase = 0;

////////////////////////////////////////////////////////////////////////////////
//  Global Functions
////////////////////////////////////////////////////////////////////////////////

#if 0
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U8 HAL_RTC_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U16 HAL_RTC_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

static MS_U32 HAL_RTC_Read4Byte(MS_U32 u32RegAddr)
{
     MS_U32 u32Val =((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr+2];
     u32Val<<=16;
     u32Val|=((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
     return u32Val;
}

#if 0
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_RTC_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_RTC_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_RTC_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

static void HAL_RTC_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    HAL_RTC_Write2Byte(u32RegAddr,(MS_U16)(u32Val&0xFFFF));
    HAL_RTC_Write2Byte(u32RegAddr+2,(MS_U16)(u32Val>>16));
}

static MS_BOOL HAL_RTC_WriteBit(MS_U32 u32RegAddr, MS_U16 u16Val, MS_BOOL bEnable)
{
    MS_U16 u16Reg;

    if (!u32RegAddr)
    {
        return FALSE;
    }

    u16Reg = ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
    if(bEnable)
        u16Reg|=u16Val;
    else
        u16Reg&=(~u16Val);
    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Reg;
    return TRUE;
}

static MS_U32 HAL_RTC_GET_BASE(E_MS_RTC eRtc)
{
    MS_U32 u32RegAddr=0;
    switch(eRtc)
    {
        case E_RTC_0:
            u32RegAddr=REG_RTC_BASE_0;
            break;
        case E_RTC_2:
            u32RegAddr=REG_RTC_BASE_2;
            break;
    }
    return u32RegAddr;
}

void HAL_RTC_RESET(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_SOFT_RSTZ_BIT,bEnable);
}

void HAL_RTC_Counter(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_CNT_EN_BIT,bEnable);
}

void HAL_RTC_Wrap_Count (E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_WRAP_EN_BIT,bEnable);
}

void HAL_RTC_Loading (E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_LOAD_EN_BIT,bEnable);
}

void HAL_RTC_Reading(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_READ_EN_BIT,bEnable);
}

void HAL_RTC_IntMask(E_MS_RTC eRtc, MS_BOOL bEnable)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_INT_MASK_BIT,bEnable);
}

void HAL_RTC_IntForce(E_MS_RTC eRtc)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_INT_FORCE_BIT,ENABLE);
}

void HAL_RTC_IntClear(E_MS_RTC eRtc)
{
    HAL_RTC_WriteBit(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CTRL_REG,RTC_INT_CLEAR_BIT,ENABLE);
}

void HAL_RTC_Set_Frequency(E_MS_RTC eRtc,MS_U32 u32Freq, MS_U32 u32Xtal)
{
    if(u32Freq)
    {
        MS_U32 u32Reg = u32Xtal/u32Freq;
        HAL_RTC_Write4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_FREQ_CW,  u32Reg);
    }
}

void HAL_RTC_Set_Counter(E_MS_RTC eRtc,MS_U32 u32Val)
{
    HAL_RTC_Write4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_LOAD_VAL, u32Val);
}

void HAL_RTC_Match_Counter(E_MS_RTC eRtc,MS_U32 u32Val)
{
    HAL_RTC_Write4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_MATCH_VAL,  u32Val);
}

MS_U16 HAL_RTC_INT_Status(E_MS_RTC eRtc)
{
    return HAL_RTC_Read2Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_INT);
}

MS_U32 HAL_RTC_Read_Counter(E_MS_RTC eRtc)
{
    MS_U32 u32Reg;
    MS_U16 u16Dummy=100;
    HAL_RTC_Reading(eRtc, ENABLE);
    while(u16Dummy--); //wait for HW latch bits okay, otherwise sometimes it read wrong value
    u32Reg = HAL_RTC_Read4Byte(HAL_RTC_GET_BASE(eRtc)+REG_RTC_CNT);
    HAL_RTC_Reading(eRtc, DISABLE);
    return u32Reg;
}

void HAL_RTC_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
}

MS_VIRT HAL_RTC_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}
