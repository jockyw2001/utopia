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
#include "halWDT.h"
#include "regWDT.h"

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

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_WDT_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_WDT_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_WDT_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_WDT_Read2Byte(u32RegAddr) | (HAL_WDT_Read2Byte(u32RegAddr+2) << 16));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        return FALSE;
    }

    HAL_WDT_Write2Byte(u32RegAddr, u32Val & 0x0000FFFFUL);
    HAL_WDT_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Start
/// @brief \b Function  \b Description: Stop WDT operations
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Start(void)
{
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Stop
/// @brief \b Function  \b Description: Stop WDT operations
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Stop(void)
{
    MS_BOOL bRet = TRUE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, WDT_CLOSE_KEY);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Clear
/// @brief \b Function  \b Description: Clear WDT.
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_Clear(void)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_WriteByte(REG_WDT_CLR, WDT_RST);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_ClearRstFlag
/// @brief \b Function  \b Description: Clear WDT reset flag.
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done ;FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_ClearRstFlag(void)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_WriteByte(REG_WDT_RST, WDT_CLR_RESET_FLAG);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_IsReset
/// @brief \b Function  \b Description: Read the reset status
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: reset at least once, FALSE: no reset occurs.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_IsReset(void)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = ((HAL_WDT_ReadByte(REG_WDT_RST)) & (WDT_RST));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_IsEnable
/// @brief \b Function  \b Description: Check wdt enable or  not.
/// @param <IN>         \b u8Ch:
/// @param <OUT>       \b None :
/// @param <RET>        \b TRUE: has started, FALSE: not yet.
/// @param <GLOBAL>   \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_IsEnable(void)
{
    MS_BOOL bRet = FALSE;

    if (HAL_WDT_Read4Byte(REG_WDT_MAX) != WDT_CLOSE_KEY) {
        bRet = TRUE;
    }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetTimer
/// @brief \b Function  \b Description: wdt interval 65536*(65536-wdt_sel) cycles
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetTimer(MS_U16 sec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, HAL_WDTCycles(sec));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetTimer
/// @brief \b Function  \b Description: wdt interval 65536*(65536-wdt_sel) cycles
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetTimer_ms(MS_U16 msec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, HAL_WDTCyclesM(msec));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetTimer
/// @brief \b Function  \b Description: wdt interval 65536*(65536-wdt_sel) cycles
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetTimer_us(MS_U16 usec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write4Byte(REG_WDT_MAX, HAL_WDTCyclesU(usec));
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetIntTimer
/// @brief \b Function  \b Description: when wdt_cnt[31:16]>reg_wdt_int_sel, wdt_int occurs
/// @param <IN>         \b u8Ch:
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: done, FALSE: not done.
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_WDT_SetIntTimer(MS_U16 sec)
{
    MS_BOOL bRet = FALSE;

    do {
        bRet = HAL_WDT_Write2Byte(REG_WDT_INT, HAL_WDTCycles(sec)>>16);
    } while(0);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_WDT_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_WDT_Get_IOMap_Base
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_WDT_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}
