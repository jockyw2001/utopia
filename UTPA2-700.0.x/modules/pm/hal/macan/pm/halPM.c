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
#define _HAL_PM_C

////////////////////////////////////////////////////////////////////////////////
/// @file halPM.c
/// @author MStar Semiconductor Inc.
/// @brief PM control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "halPM.h"
#include "regPM.h"
//#include "datatype.h"
#include "MsOS.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
#define PM_HAL_ERR(x, args...)        //{printf(x, ##args);}



#if 0//def WORDS_BIGENDIAN

    #define VARBYTE(var, n)    (((MS_U8 *)&(var))[n])
#else

    #define VARBYTE(var, n)    (((MS_U8 *)&(var))[sizeof(var) - n - 1])
#endif

////////////////////////////////////////////////////////////////////////////////
// Global variable
////////////////////////////////////////////////////////////////////////////////
static MS_VIRT _gMIO_MapBase = 0;

////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function Declaration
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////
#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_PM_ReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_Read2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_PM_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_PM_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_PM_Read2Byte(u32RegAddr) | HAL_PM_Read2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PM_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        PM_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PM_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        PM_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PM_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        PM_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_PM_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_PM_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_ReadRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PM_ReadRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        PM_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_PM_ReadByte(u32RegAddr);
    return (u8Val & u8Mask);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_WriteRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PM_WriteRegBit(MS_U32 u32RegAddr, MS_BOOL bEnable, MS_U8 u8Mask)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        PM_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_PM_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_PM_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_WriteByteMask
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_PM_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8ValIn, MS_U8 u8Msk)
{
    MS_U8 u8Val;
    if (!u32RegAddr)
    {
        PM_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_PM_ReadByte(u32RegAddr);
    u8Val = (u8Val & ~(u8Msk)) | ((u8ValIn) & (u8Msk));
    HAL_PM_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_PM_SetIOMapBase(MS_VIRT virtBase)
{
    _gMIO_MapBase = virtBase;
    PM_HAL_ERR("PM IOMap base:%8lx Reg offset:%4x\n", virtBase, PMRTC_REG_BASE);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_PM_GetIOMapBase
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_PM_GetIOMapBase(void)
{
    return _gMIO_MapBase;
}

void HAL_PM_RtcInit(MS_U8 u8RtcIndex, MS_U32 u32RtcCtrlWord)
{
    HAL_PM_Write2Byte(REG_PM_CKG_RTC,
        (HAL_PM_Read2Byte(REG_PM_CKG_RTC)&(~(__BIT4|__BIT3|__BIT2|__BIT1|__BIT0)))|(1<<2)); //RTC clock switch to 12MHz

    switch(u8RtcIndex)
    {
        PM_HAL_ERR("RTC init\n");

        case PM_RTCIDX_0:
            PM_HAL_ERR("PM_RTCIDX_0\n");
            if((HAL_PM_ReadByte(REG_PMRTC_CTRL) & (0x00 | PMRTC_CTRL_CNT_EN))) //check if RTC was initialized before
            {
                HAL_PM_WriteRegBit(REG_PMRTC_CTRL, DISABLE, PMRTC_CTRL_CNT_EN); // Disable init
                HAL_PM_WriteRegBit(REG_PMRTC_CTRL, DISABLE, PMRTC_CTRL_NOT_RSTZ); // Disable RSTZ
            }
            //HAL_PM_WriteByte(REG_PMRTC_CTRL, 0x00);
            HAL_PM_WriteByte(REG_PMRTC_CTRL, (PMRTC_CTRL_NOT_RSTZ | PMRTC_CTRL_INT_MASK));
            HAL_PM_WriteByte(REG_PMRTC_FREQ_CW + 0, VARBYTE(u32RtcCtrlWord, 3));
            HAL_PM_WriteByte(REG_PMRTC_FREQ_CW + 1, VARBYTE(u32RtcCtrlWord, 2));
            HAL_PM_WriteByte(REG_PMRTC_FREQ_CW + 2, VARBYTE(u32RtcCtrlWord, 1));
            HAL_PM_WriteByte(REG_PMRTC_FREQ_CW + 3, VARBYTE(u32RtcCtrlWord, 0));
            HAL_PM_WriteByte(REG_PMRTC_CTRL, (PMRTC_CTRL_NOT_RSTZ | PMRTC_CTRL_CNT_EN | PMRTC_CTRL_INT_MASK | PMRTC_CTRL_INT_CLEAR));
            break;
        case PM_RTCIDX_1:
            PM_HAL_ERR("PM_RTCIDX_1\n");
            if((HAL_PM_ReadByte(REG_PMRTC1_CTRL) & (0x00 | PMRTC1_CTRL_CNT_EN))) //check if RTC was initialized before
            {
                HAL_PM_WriteRegBit(REG_PMRTC1_CTRL, DISABLE, PMRTC1_CTRL_CNT_EN); // Disable init
                HAL_PM_WriteRegBit(REG_PMRTC1_CTRL, DISABLE, PMRTC1_CTRL_NOT_RSTZ); // Disable RSTZ
            }
            //HAL_PM_WriteByte(REG_PMRTC1_CTRL, 0x00);
            HAL_PM_WriteByte(REG_PMRTC1_CTRL, (PMRTC1_CTRL_NOT_RSTZ | PMRTC1_CTRL_INT_MASK));
            HAL_PM_WriteByte(REG_PMRTC1_FREQ_CW + 0, VARBYTE(u32RtcCtrlWord, 3));
            HAL_PM_WriteByte(REG_PMRTC1_FREQ_CW + 1, VARBYTE(u32RtcCtrlWord, 2));
            HAL_PM_WriteByte(REG_PMRTC1_FREQ_CW + 2, VARBYTE(u32RtcCtrlWord, 1));
            HAL_PM_WriteByte(REG_PMRTC1_FREQ_CW + 3, VARBYTE(u32RtcCtrlWord, 0));
            HAL_PM_WriteByte(REG_PMRTC1_CTRL, (PMRTC1_CTRL_NOT_RSTZ | PMRTC1_CTRL_CNT_EN | PMRTC1_CTRL_INT_MASK | PMRTC1_CTRL_INT_CLEAR));
            break;
        default:
            PM_HAL_ERR("ERROR RTC Index Number\n");
            break;
    }
}

void HAL_PM_RTC_DisableInit(MS_U8 u8RtcIndex)
{
    switch(u8RtcIndex)
    {
        case PM_RTCIDX_0:
            HAL_PM_WriteRegBit(REG_PMRTC_CTRL, DISABLE, PMRTC_CTRL_CNT_EN); // Disable init
            HAL_PM_WriteRegBit(REG_PMRTC_CTRL, DISABLE, PMRTC_CTRL_NOT_RSTZ); // Disable RSTZ
            break;
        case PM_RTCIDX_1:
            HAL_PM_WriteRegBit(REG_PMRTC1_CTRL, DISABLE, PMRTC1_CTRL_CNT_EN); // Disable init
            HAL_PM_WriteRegBit(REG_PMRTC1_CTRL, DISABLE, PMRTC1_CTRL_NOT_RSTZ); // Disable RSTZ
            break;
        default:
            PM_HAL_ERR("ERROR RTC Index Number\n");
            break;
    }
}

void HAL_PM_RtcSetCounter(MS_U8 u8RtcIndex, MS_U32 u32RtcSetCounter)
{
    switch(u8RtcIndex)
    {
        PM_HAL_ERR("RTC SetCounter\n");
        case PM_RTCIDX_0:
            PM_HAL_ERR("PM_RTCIDX_0\n");
            HAL_PM_WriteByte(REG_PMRTC_LOAD_VAL + 0, VARBYTE(u32RtcSetCounter, 3));
            HAL_PM_WriteByte(REG_PMRTC_LOAD_VAL + 1, VARBYTE(u32RtcSetCounter, 2));
            HAL_PM_WriteByte(REG_PMRTC_LOAD_VAL + 2, VARBYTE(u32RtcSetCounter, 1));
            HAL_PM_WriteByte(REG_PMRTC_LOAD_VAL + 3, VARBYTE(u32RtcSetCounter, 0));
            HAL_PM_WriteByte(REG_PMRTC_CTRL, (HAL_PM_ReadByte(REG_PMRTC_CTRL)|PMRTC_CTRL_LOAD_EN));
            break;
        case PM_RTCIDX_1:
            PM_HAL_ERR("PM_RTCIDX_1\n");
            HAL_PM_WriteByte(REG_PMRTC1_LOAD_VAL + 0, VARBYTE(u32RtcSetCounter, 3));
            HAL_PM_WriteByte(REG_PMRTC1_LOAD_VAL + 1, VARBYTE(u32RtcSetCounter, 2));
            HAL_PM_WriteByte(REG_PMRTC1_LOAD_VAL + 2, VARBYTE(u32RtcSetCounter, 1));
            HAL_PM_WriteByte(REG_PMRTC1_LOAD_VAL + 3, VARBYTE(u32RtcSetCounter, 0));
            HAL_PM_WriteByte(REG_PMRTC1_CTRL, (HAL_PM_ReadByte(REG_PMRTC1_CTRL)|PMRTC1_CTRL_LOAD_EN));
            break;
        default:
            PM_HAL_ERR("ERROR RTC Index Number\n");
            break;
    }
}


MS_U32 HAL_PM_RtcGetCounter(MS_U8 u8RtcIndex)
{
    switch(u8RtcIndex)
    {
        case PM_RTCIDX_0:
            HAL_PM_WriteByte(REG_PMRTC_CTRL, (HAL_PM_ReadByte(REG_PMRTC_CTRL)|PMRTC_CTRL_READ_EN));
            MsOS_DelayTaskUs(100);
			return HAL_PM_Read4Byte(REG_PMRTC_CNT);
            break;
        case PM_RTCIDX_1:
            HAL_PM_WriteByte(REG_PMRTC1_CTRL, (HAL_PM_ReadByte(REG_PMRTC1_CTRL)|PMRTC1_CTRL_READ_EN));
            MsOS_DelayTaskUs(100);
			return HAL_PM_Read4Byte(REG_PMRTC1_CNT);
            break;
        default:
            PM_HAL_ERR("ERROR RTC Index Number\n");
            break;
    }
    PM_HAL_ERR("ERROR\n");
    return 0;
}

void HAL_PM_RtcSetMatchCounter(MS_U8 u8RtcIndex, MS_U32 u32RtcSetMatchCounter)
{
    switch(u8RtcIndex)
    {
        case PM_RTCIDX_0:
            HAL_PM_WriteByte(REG_PMRTC_CTRL, (HAL_PM_ReadByte(REG_PMRTC_CTRL)|(PMRTC_CTRL_INT_MASK|PMRTC_CTRL_INT_CLEAR)));
            if (u32RtcSetMatchCounter)
            {
                HAL_PM_Write4Byte(REG_PMRTC_MATCH_VAL + 4, 0x00000000);
                HAL_PM_WriteByte(REG_PMRTC_MATCH_VAL + 0, VARBYTE(u32RtcSetMatchCounter, 3));
                HAL_PM_WriteByte(REG_PMRTC_MATCH_VAL + 1, VARBYTE(u32RtcSetMatchCounter, 2));
                HAL_PM_WriteByte(REG_PMRTC_MATCH_VAL + 2, VARBYTE(u32RtcSetMatchCounter, 1));
                HAL_PM_WriteByte(REG_PMRTC_MATCH_VAL + 3, VARBYTE(u32RtcSetMatchCounter, 0));
                HAL_PM_WriteByte(REG_PMRTC_CTRL, (HAL_PM_ReadByte(REG_PMRTC_CTRL)&(~(PMRTC_CTRL_INT_MASK|PMRTC_CTRL_INT_CLEAR))));
            }
            break;
        case PM_RTCIDX_1:
            HAL_PM_WriteByte(REG_PMRTC1_CTRL, (HAL_PM_ReadByte(REG_PMRTC1_CTRL)|(PMRTC1_CTRL_INT_MASK|PMRTC1_CTRL_INT_CLEAR)));
            if (u32RtcSetMatchCounter)
            {
                HAL_PM_Write4Byte(REG_PMRTC1_MATCH_VAL + 4, 0x00000000);
                HAL_PM_WriteByte(REG_PMRTC1_MATCH_VAL + 0, VARBYTE(u32RtcSetMatchCounter, 3));
                HAL_PM_WriteByte(REG_PMRTC1_MATCH_VAL + 1, VARBYTE(u32RtcSetMatchCounter, 2));
                HAL_PM_WriteByte(REG_PMRTC1_MATCH_VAL + 2, VARBYTE(u32RtcSetMatchCounter, 1));
                HAL_PM_WriteByte(REG_PMRTC1_MATCH_VAL + 3, VARBYTE(u32RtcSetMatchCounter, 0));
                HAL_PM_WriteByte(REG_PMRTC1_CTRL, (HAL_PM_ReadByte(REG_PMRTC1_CTRL)&(~(PMRTC1_CTRL_INT_MASK|PMRTC1_CTRL_INT_CLEAR))));
            }
            break;
        default:
            PM_HAL_ERR("ERROR RTC Index Number\n");
            break;
    }
}

MS_U32 HAL_PM_RtcGetMatchCounter(MS_U8 u8RtcIndex)
{
    MS_U32 RTCCount=0;
    switch(u8RtcIndex)
    {
        case PM_RTCIDX_0:
            RTCCount= HAL_PM_Read4Byte(REG_PMRTC_MATCH_VAL);
            break;
        case PM_RTCIDX_1:
            RTCCount= HAL_PM_Read4Byte(REG_PMRTC1_MATCH_VAL);
            break;
        default:
            PM_HAL_ERR("ERROR RTC Index Number\n");
            break;
    }
    return RTCCount;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetSPIOffsetForMCU
/// @brief \b Function  \b Description:
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
#define MCU_REG_BASE               0x1000UL           //0x001000
#define PMSLEEP_REG_BASE           0x0e00UL           //0x000e00
#define REG_PM_CPUX_SW_RSTZ        (PMSLEEP_REG_BASE + 0x29UL*2)

void HAL_PM_SetSPIOffsetForMCU(MS_U32 BANK)
{

     MS_U32 start_addr = 0x00000UL;
     MS_U32 end_addr = 0xff0000UL;
     MS_U32	u32temCnt;

     HAL_PM_WriteRegBit(0x000e41UL, DISABLE, __BIT6);  //spi_clk=xtal

     HAL_PM_WriteRegBit(0x000e40UL, DISABLE, __BIT7);  //mcu51 clk=xtal

     HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT3);  // i_cache rstz

     HAL_PM_WriteRegBit(0x002ba0UL, ENABLE,  __BIT0);  //disable i cache

     HAL_PM_Write2Byte(0x0e24UL,0x0000UL);
     HAL_PM_Write2Byte(0x0e26UL,0x0000UL);

     HAL_PM_Write2Byte(0x2e52UL,0x4002UL);   //PM 51 rst

     HAL_PM_Write2Byte(0x2e3eUL,0x44UL);    // Chip_overwrite to mips boot


     HAL_PM_Write2Byte(MCU_REG_BASE+0x14UL,(MS_U16)start_addr & 0x0000FFFFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x10UL,(MS_U16)(start_addr>>16) & 0x000000FFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x16UL,(MS_U16)end_addr & 0x0000FFFFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x12UL,(MS_U16)(end_addr>>16) & 0x000000FFUL);




    //set spi offset set reg_spi_offset_addr[7:0]     0x0010f9 = 0x01     => 0x10000
    //set reg_spi_offset_en               0x0010f8 = 0x01
    //HAL_PM_Write2Byte(MCU_REG_BASE+0xf8,0x0101);
    HAL_PM_WriteByte(MCU_REG_BASE+0xf9UL,(MS_U8) (0xFFUL&BANK) );
    HAL_PM_WriteByte(MCU_REG_BASE+0xf8UL,0x01UL);
    //set reset password 0x0e54=0x829f
   // HAL_PM_Write2Byte(PMSLEEP_REG_BASE+0x54,0x829f);
    HAL_PM_Write2Byte(0x2e54,0x829fUL);


    //reg_fire[3:0]                   0x000e53 = 0x01

    //reg_fire[3:0]                   0x000e53 = 0x00

    HAL_PM_WriteByte(0x2e53UL,0x41UL);
    for(u32temCnt=0; u32temCnt<0xffffUL; u32temCnt++);
    HAL_PM_WriteByte(0x2e53UL,0x40UL);

    for(u32temCnt=0; u32temCnt<0xffffUL; u32temCnt++);
    HAL_PM_WriteByte(0x2e53UL,0x50UL);

             printf("PM Wait for PM51 standby...........\n");
	while(HAL_PM_Read2Byte(0x0e24UL)!=0x02UL);

             printf("PM51 run ok...........\n");


}

void HAL_PM_SetSRAMOffsetForMCU(void)
{
     MS_U32 start_addr = 0x0000000UL;
     MS_U32 end_addr = 0x00005FFFUL;
     HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)&(~__BIT0));
     HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)&(~__BIT4));   //8051 reset
     HAL_PM_WriteByte(0x0e24UL, 0x00UL);//Clear bit


     //HAL_PM_WriteRegBit(0x000e41, DISABLE, __BIT6);  //spi_clk=xtal

     HAL_PM_WriteRegBit(0x000e40UL, DISABLE, __BIT7);  //mcu51 clk=xtal

     HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT3);  // i_cache rstz

     HAL_PM_WriteRegBit(0x002ba0UL, ENABLE , __BIT0);  //disable i cache (enable icache bypass)

     HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT1); //SPI  disable

     HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT2); //DRAM disable

     HAL_PM_WriteRegBit(0x001018UL, ENABLE , __BIT0); //SRAM enable

     HAL_PM_Write2Byte(MCU_REG_BASE+0x04UL,(MS_U16)start_addr & 0x0000FFFFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x00UL,(MS_U16)(start_addr>>16) & 0x000000FFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x06UL,(MS_U16)end_addr & 0x0000FFFFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x02UL,(MS_U16)(end_addr>>16) & 0x000000FFUL);

    //set reset password 0x0e54=0x829f
    HAL_PM_Write2Byte(PMSLEEP_REG_BASE+0x54UL,0x829fUL);


    //reg_fire[3:0]                   0x000e53 = 0x01

    //reg_fire[3:0]                   0x000e53 = 0x00
    HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)|__BIT0);
    MsOS_DelayTaskUs(120);
    HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)|__BIT4);   //release 8051  reset

    printf("Wait for PM51 standby...........\n");
	while(HAL_PM_Read2Byte(0x0e24UL)!=0x02UL);

    printf("PM51 run ok...........\n");

}

MS_BOOL HAL_PM_SetDRAMOffsetForMCU(MS_U32 u32Offset)
{
     MS_U32 start_addr = 0x0000000UL;
     MS_U32 end_addr = 0xFFFFFFFFUL;
     MS_U8 times = 0;

     HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)&(~__BIT0));
     HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)&(~__BIT4));   //8051 reset
     HAL_PM_WriteByte(0x0e24UL, 0x00UL);//Clear bit


     //HAL_PM_WriteRegBit(0x000e41, DISABLE, __BIT6);  //spi_clk=xtal

     HAL_PM_WriteRegBit(0x000e40UL, DISABLE, __BIT7);  //mcu51 clk=xtal

     HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT3);  // i_cache rstz

     HAL_PM_WriteRegBit(0x002ba0UL, ENABLE , __BIT0);  //disable i cache (enable icache bypass)

     HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT1); //SPI  disable

     HAL_PM_WriteRegBit(0x001018UL, ENABLE, __BIT2); //DRAM enable

     HAL_PM_WriteRegBit(0x001018UL, DISABLE , __BIT0); //SRAM disable

     HAL_PM_Write2Byte(MCU_REG_BASE+0x0cUL,(MS_U16)start_addr & 0x0000FFFFUL);       //reg_dram_start_addr_0
     HAL_PM_Write2Byte(MCU_REG_BASE+0x08UL,(MS_U16)(start_addr>>16) & 0x0000FFFFUL);
     HAL_PM_Write2Byte(MCU_REG_BASE+0x0eUL,(MS_U16)end_addr & 0x0000FFFFUL);         //reg_dram_end_addr_0
     HAL_PM_Write2Byte(MCU_REG_BASE+0x0AUL,(MS_U16)(end_addr>>16) & 0x00000FFFFUL);

    //set reset password 0x0e54=0x829f
    HAL_PM_Write2Byte(0x2e54UL,0x829fUL);

    HAL_PM_Write2Byte(0x2B80,(u32Offset>>16));

    //reg_fire[3:0]                   0x000e53 = 0x01

    //reg_fire[3:0]                   0x000e53 = 0x00
    HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)|__BIT0);
    MsOS_DelayTaskUs(120);
    HAL_PM_WriteByte(0x2e53UL,HAL_PM_ReadByte(0x2e53UL)|__BIT4);   //release 8051  reset

    printf("Wait for PM51 standby DRAM ...........\n");

	while((HAL_PM_Read2Byte(0x0e24UL)!=0x02UL))
    {
        if(times > 20)
        {
            printf("PM51 run failed...........\n");            // it will try to run RT_PM in 400ms, when time's up, the function will return directly
            return FALSE;
        }
        MsOS_DelayTask(20);     // delay 20ms
        times++;
    }

    printf("PM51 run ok...........\n");
	return TRUE;
}

MS_U8 HAL_PM_PowerOnMode(void)
{
    MS_U8  u8PowerDownMode = HAL_PM_ReadByte(REG_PM_DUMMY_POWERON_MODE);

    return u8PowerDownMode;

}

MS_U8 HAL_PM_GetWakeupSource(void)
{
    MS_U8  u8WakeupSource = HAL_PM_ReadByte(REG_PM_DUMMY_WAKEUP_SOURCE);

    return u8WakeupSource;

}

MS_U8 HAL_PM_GetWakeupKey(void)
{
    PM_HAL_ERR("[%s] Not implemented yet\n", __FUNCTION__);
    return 0;
}

MS_BOOL HAL_PM_isRunning(void)
{
    MS_U16 u16Data_old;
    MS_U16 u16Data_new;
	
    HAL_PM_Write2Byte(0x0ea2UL,0x1234UL);				//dummy register
    u16Data_old = HAL_PM_Read2Byte(0x10fcUL);			//pc counter
    MsOS_DelayTaskUs(10);
    u16Data_new = HAL_PM_Read2Byte(0x10fcUL);
	
    if((u16Data_old != u16Data_new) && (HAL_PM_Read2Byte(0x0ea2UL)==0x4321UL))
        return TRUE;
    else
        return FALSE;	
}

void HAL_PM_Disable51(void)
{	
    HAL_PM_WriteByte(0x0e38UL, HAL_PM_ReadByte(0x0e38UL)&(~__BIT2));
    HAL_PM_WriteByte(0x0e38UL, HAL_PM_ReadByte(0x0e38UL)&(~__BIT3));
    HAL_PM_WriteRegBit(0x002e53UL, DISABLE, __BIT4); //8051  reset  disable
    HAL_PM_WriteRegBit(0x002e53UL, DISABLE, __BIT0); //8051  reset  disable
    HAL_PM_WriteRegBit(0x001018UL, ENABLE, __BIT1); //SPI  disable
    HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT2); //DRAM disable
    HAL_PM_WriteRegBit(0x001018UL, DISABLE , __BIT0); //SRAM enable
    HAL_PM_WriteRegBit(0x001018UL, DISABLE, __BIT3);  // i_cache rstz
    HAL_PM_WriteByte(0x0e24UL, 0x00UL);//Clear bit
    HAL_PM_Write2Byte(0x2B80UL,0x0000UL);
	
	#if 0
    HAL_PM_WriteRegBit(REG_PM_CHIP_CFG_OVERWRITE, 1, CHIP_CFG_MIPS_EN);
    HAL_PM_WriteRegBit(REG_PM_CHIP_CFG_OVERWRITE, 1, CHIP_CFG_MIPS_VAL);
    MsOS_DelayTask(10);
    HAL_PM_WriteRegBit(REG_PM_CPU_SW_RST, 0, PM_51_SW_RST); // active low
	#endif

}

#define ENABLE_PATTERN 0xAA
#define DISABLE_PATTERN 0x55
#define TRIGGER_MASK (__BIT7|__BIT6|__BIT5|__BIT4)
#define ACTIVE_MASK (__BIT3|__BIT2|__BIT1|__BIT0)

MS_BOOL HAL_PM_GetBackGroundActive(void)
{
    MS_U8 u8Mask,u8Data;
    u8Mask = (MS_U8)ACTIVE_MASK;
    u8Data = HAL_PM_ReadByte(REG_PM_DUMMY_ACTIVE_STANDBY);
    return ((ENABLE_PATTERN & u8Mask) == (u8Data & u8Mask));
}

void HAL_PM_SetBackGroundTrigger(MS_BOOL bTrigger)
{
    MS_U8 u8Mask,u8Data,u8CtrlData;
    u8Mask = (MS_U8)TRIGGER_MASK;
    u8Data = HAL_PM_ReadByte(REG_PM_DUMMY_ACTIVE_STANDBY);
    if(TRUE == bTrigger)
        u8CtrlData = (MS_U8)(ENABLE_PATTERN);
    else
        u8CtrlData = (MS_U8)(DISABLE_PATTERN);
    //HAL_PM_WriteByte(REG_PM_DUMMY_ACTIVE_STANDBY, ((u8Data & (~u8Mask)) | (u8CtrlData & u8Mask)));
    HAL_PM_WriteByte(REG_PM_DUMMY_ACTIVE_STANDBY, (u8CtrlData));
}

void HAL_PM_GPIO4_SetPower(MS_BOOL bOn)
{
    if(bOn)
    {
        HAL_PM_Write2Byte(REG_PM_GPIO4_OUT, (HAL_PM_Read2Byte(REG_PM_GPIO4_OUT))|(PMGPIO_OUT));
        HAL_PM_Write2Byte(REG_PM_GPIO4_OEN, (HAL_PM_Read2Byte(REG_PM_GPIO4_OEN))&(~PMGPIO_OEN));
    }
    else
    {
        HAL_PM_Write2Byte(REG_PM_GPIO4_OUT, (HAL_PM_Read2Byte(REG_PM_GPIO4_OUT))&(~PMGPIO_OUT));
        HAL_PM_Write2Byte(REG_PM_GPIO4_OEN, (HAL_PM_Read2Byte(REG_PM_GPIO4_OEN))&(~PMGPIO_OEN));
        HAL_PM_Write2Byte(REG_PM_GPIO_PM_LOCK, 0xBABEUL);
    }
}

void HAL_PM_SetScratchPad(MS_U8 u8PadNum,MS_U16 u16Val)
{
    switch(u8PadNum)
    {
        case 1:
            HAL_PM_Write2Byte(PM_REG1_ScratchPad,u16Val); 
            break;
        case 2:
            HAL_PM_Write2Byte(PM_REG2_ScratchPad,u16Val);
            break;
        case 3:
            HAL_PM_Write2Byte(PM_REG3_ScratchPad,u16Val);
            break;
        case 4:
            HAL_PM_Write2Byte(PM_REG4_ScratchPad,u16Val);
            break;
        default:
            break;
    }
}

MS_BOOL HAL_PM_GetScratchPad(MS_U8 u8PadNum, MS_U16 *u16Val)
{
    if(u16Val == NULL)
    {
        return FALSE;
    }
    
    switch(u8PadNum)
    {
        case 1:
            *u16Val = HAL_PM_Read2Byte(PM_REG1_ScratchPad);
            return TRUE;
        case 2:
            *u16Val = HAL_PM_Read2Byte(PM_REG2_ScratchPad);
            return TRUE;
        case 3:
            *u16Val = HAL_PM_Read2Byte(PM_REG3_ScratchPad);
            return TRUE;
        case 4:
            *u16Val = HAL_PM_Read2Byte(PM_REG4_ScratchPad);
            return TRUE;
        default:
            return FALSE;
    }
}

MS_BOOL HAL_PM_GetVersion(MS_U8 *u8Val)
{
    if(u8Val == NULL)
    {
        return FALSE;
    }
    
    *u8Val = HAL_PM_ReadByte(MCU_REG_BASE+0xA6UL);
    return TRUE; 
}

MS_U16 HAL_PM_GetSRAMSize(void)
{
    MS_U16 u16SRAMSize = 0;
    u16SRAMSize = 0x6000UL;
    return u16SRAMSize;

}

MS_U8 HAL_PM_GetIRPowerOnKey(void)
{
    MS_U8 u8IRKey = 0;
    u8IRKey = HAL_PM_ReadByte(MCU_REG_BASE+0xA7UL);
    return u8IRKey; 
}

void HAL_PM_SetWakupDevice(MS_U16 u16WakeSrc)
{
    //HAL_PM_Write2Byte(REG_PM_SLEEP_NOTIFY_WAKEUP_DEVICE_FLAG,u16WakeSrc);    
}
