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
////////////////////////////////////////////////////////////////////////////////
#define _HAL_HWI2C_C

////////////////////////////////////////////////////////////////////////////////
/// @file halHWI2C.c
/// @author MStar Semiconductor Inc.
/// @brief DRAM byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <asm/io.h>
#include <chip_setup.h>
#endif
#else
#include <string.h>
#endif 
#include "MsCommon.h"
#include "MsTypes.h"
#include "halHWI2C.h"
#include "halCHIP.h"
#include "regHWI2C.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
#define HWI2C_HAL_RETRY_TIMES     (3)
#define HWI2C_HAL_WAIT_TIMEOUT    30000//(1500)
#define HWI2C_HAL_FUNC()              //{printf("%s\n",  __FUNCTION__);}
#define HWI2C_HAL_INFO(x, args...)    //{printf(x, ##args);}
#define HWI2C_HAL_ERR(x, args...)     //{printf(x, ##args);}
#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define HWI2C_DMA_CMD_DATA_LEN      7
#define HWI2C_DMA_WAIT_TIMEOUT      (30000)
#define HWI2C_DMA_WRITE             0
#define HWI2C_DMA_READ              1
#define _PA2VA(x) (MS_VIRT)MsOS_PA2KSEG1((x))
#define _VA2PA(x) (MS_VIRT)MsOS_VA2PA((x))

////////////////////////////////////////////////////////////////////////////////
// Local variable
////////////////////////////////////////////////////////////////////////////////
static MS_VIRT _gMIO_MapBase = 0;
static MS_BOOL g_bLastByte[HAL_HWI2C_PORTS];
static MS_VIRT g_u32DmaPhyAddr[HAL_HWI2C_PORTS];
static HAL_HWI2C_PortCfg g_stPortCfg[HAL_HWI2C_PORTS];
static MS_U16 g_u16DmaDelayFactor[HAL_HWI2C_PORTS];

#ifdef MSOS_TYPE_LINUX_KERNEL
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
static miic_private g_miic_dev[HAL_HWI2C_PORTS];

static MS_U32 _gu32MIU0_BUS_BASE=0;
static MS_U32 _gu32MIU0_SIZE = 0;
static void __iomem *Addr[HAL_HWI2C_PORTS] = {NULL};
MS_U8 u8HWIICSpecial = 0;
MS_U32 u32HWIICSpecialDelay = 0;
MS_U8 u8ResumeFlag = 0;
extern void Chip_Flush_Memory(void);
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Function Declaration
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////
void HAL_HWI2C_ExtraDelay(MS_U32 u32Us)
{
    // volatile is necessary to avoid optimization
    MS_U32 volatile u32Dummy = 0;
    //MS_U32 u32Loop;
    MS_U32 volatile u32Loop;

    u32Loop = (MS_U32)(50 * u32Us);
    while (u32Loop--)
    {
        u32Dummy++;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_SetIOMapBase
/// @brief \b Function  \b Description: Dump bdma all register
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_HWI2C_SetIOMapBase(MS_VIRT u32Base)
{
    HWI2C_HAL_FUNC();

    _gMIO_MapBase = u32Base;
    HWI2C_HAL_INFO("HWI2C IOMap base:%8lx Reg offset:%4x\n", u32Base, (MS_U16)HWI2C_REG_BASE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_HWI2C_ReadByte(MS_VIRT u32RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_HWI2C_Read2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_HWI2C_Read4Byte(MS_U32 u32RegAddr)
{
    return (HAL_HWI2C_Read2Byte(u32RegAddr) | HAL_HWI2C_Read2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        HWI2C_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        HWI2C_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        HWI2C_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_HWI2C_Write2Byte(u32RegAddr, u32Val & 0x0000FFFF);
    HAL_HWI2C_Write2Byte(u32RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_WriteRegBit
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_U8 u8Val = 0;

    if (!u32RegAddr)
    {
        HWI2C_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_HWI2C_ReadByte(u32RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_HWI2C_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_WriteByteMask
/// @brief \b Function  \b Description: write data with mask bits
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <IN>         \b u8Mask : mask bits
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask)
{
    if (!u32RegAddr)
    {
        HWI2C_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }
    
    u8Val = (HAL_HWI2C_ReadByte(u32RegAddr) & ~u8Mask) | (u8Val & u8Mask);
    HAL_HWI2C_WriteByte(u32RegAddr, u8Val);
    return TRUE;
}

//#####################
//
//  MIIC STD Related Functions
//  Static or Internal use
//
//#####################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_EnINT
/// @brief \b Function  \b Description: Enable Interrupt
/// @param <IN>         \b bEnable : TRUE: Enable, FALSE: Disable
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_EnINT(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_EN_INT, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_EnDMA
/// @brief \b Function  \b Description: Enable DMA
/// @param <IN>         \b bEnable : TRUE: Enable, FALSE: Disable
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_EnDMA(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_EN_DMA, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_EnClkStretch
/// @brief \b Function  \b Description: Enable Clock Stretch
/// @param <IN>         \b bEnable : TRUE: Enable, FALSE: Disable
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_EnClkStretch(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_EN_CLKSTR, bEnable);
}

#if 0//RFU
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_EnTimeoutINT
/// @brief \b Function  \b Description: Enable Timeout Interrupt
/// @param <IN>         \b bEnable : TRUE: Enable, FALSE: Disable
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_EnTimeoutINT(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_EN_TMTINT, bEnable);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_EnFilter
/// @brief \b Function  \b Description: Enable Filter
/// @param <IN>         \b bEnable : TRUE: Enable, FALSE: Disable
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_EnFilter(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_EN_FILTER, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_EnPushSda
/// @brief \b Function  \b Description: Enable push current for SDA
/// @param <IN>         \b bEnable : TRUE: Enable, FALSE: Disable
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_EnPushSda(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_EN_PUSH1T, bEnable);
}

//#####################
//
//  MIIC DMA Related Functions
//  Static or Internal use
//
//#####################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetINT
/// @brief \b Function  \b Description: Initialize HWI2C DMA
/// @param <IN>         \b bEnable : TRUE: enable INT, FALSE: disable INT
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetINT(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CFG+u32PortOffset, _DMA_CFG_INTEN, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_Reset
/// @brief \b Function  \b Description: Reset HWI2C DMA
/// @param <IN>         \b bReset : TRUE: Not Reset FALSE: Reset
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_Reset(MS_U32 u32PortOffset, MS_BOOL bReset)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CFG+u32PortOffset, _DMA_CFG_RESET, bReset);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_MiuReset
/// @brief \b Function  \b Description: Reset HWI2C DMA MIU
/// @param <IN>         \b bReset : TRUE: Not Reset FALSE: Reset
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_MiuReset(MS_U32 u32PortOffset, MS_BOOL bReset)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CFG+u32PortOffset, _DMA_CFG_MIURST, bReset);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetMiuPri
/// @brief \b Function  \b Description: Set HWI2C DMA MIU Priority
/// @param <IN>         \b eMiuPri : E_HAL_HWI2C_DMA_PRI_LOW, E_HAL_HWI2C_DMA_PRI_HIGH
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetMiuPri(MS_U32 u32PortOffset, HAL_HWI2C_DMA_MIUPRI eMiuPri)
{
    MS_BOOL bHighPri;

    HWI2C_HAL_FUNC();
    if(eMiuPri>=E_HAL_HWI2C_DMA_PRI_MAX)
        return FALSE;
    bHighPri = (eMiuPri==E_HAL_HWI2C_DMA_PRI_HIGH)? TRUE : FALSE;
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CFG+u32PortOffset, _DMA_CFG_MIUPRI, bHighPri);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetMiuAddr
/// @brief \b Function  \b Description: Set HWI2C DMA MIU Address
/// @param <IN>         \b u32MiuAddr : MIU Address
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetMiuAddr(MS_U32 u32PortOffset, MS_U32 u32MiuAddr)
{
    MS_U8 u8Port;
	MS_U32 u32StartOffset;
    MS_U8  u8MiuSel;
    
    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    g_u32DmaPhyAddr[u8Port] = u32MiuAddr;

	_phy_to_miu_offset(u8MiuSel, u32StartOffset, u32MiuAddr);

    return HAL_HWI2C_Write4Byte(REG_HWI2C_DMA_MIU_ADR+u32PortOffset, u32StartOffset);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_Trigger
/// @brief \b Function  \b Description: Trigger HWI2C DMA
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_Trigger(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CTL_TRIG+u32PortOffset, _DMA_CTL_TRIG, TRUE);
}

#if 0 //will be used later
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_ReTrigger
/// @brief \b Function  \b Description: Re-Trigger HWI2C DMA
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_ReTrigger(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CTL+u32PortOffset, _DMA_CTL_RETRIG, TRUE);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetTxfrStop
/// @brief \b Function  \b Description: Control HWI2C DMA Transfer Format with or w/o STOP
/// @param <IN>         \b bEnable : TRUE:  with STOP, FALSE: without STOP 
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetTxfrStop(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    MS_BOOL bTxNoStop;
    
    HWI2C_HAL_FUNC();
    bTxNoStop = (bEnable)? FALSE : TRUE;
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CTL+u32PortOffset, _DMA_CTL_TXNOSTOP, bTxNoStop);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetReadMode
/// @brief \b Function  \b Description: Control HWI2C DMA Transfer Format with or w/o STOP
/// @param <IN>         \b eReadMode : E_HAL_HWI2C_DMA_READ_NOSTOP, E_HAL_HWI2C_DMA_READ_STOP
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetReadMode(MS_U32 u32PortOffset, HAL_HWI2C_ReadMode eReadMode)
{
    HWI2C_HAL_FUNC();
    if(eReadMode>=E_HAL_HWI2C_READ_MODE_MAX)
        return FALSE;
    if(eReadMode==E_HAL_HWI2C_READ_MODE_DIRECTION_CHANGE)
        return HAL_HWI2C_DMA_SetTxfrStop(u32PortOffset, FALSE);
    else
    if(eReadMode==E_HAL_HWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START)
        return HAL_HWI2C_DMA_SetTxfrStop(u32PortOffset, TRUE);
    else
        return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetRdWrt
/// @brief \b Function  \b Description: Control HWI2C DMA Read or Write
/// @param <IN>         \b bRdWrt : TRUE: read ,FALSE: write
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetRdWrt(MS_U32 u32PortOffset, MS_BOOL bRdWrt)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CTL+u32PortOffset, _DMA_CTL_RDWTCMD, bRdWrt);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetMiuChannel
/// @brief \b Function  \b Description: Control HWI2C DMA MIU channel
/// @param <IN>         \b u8MiuCh : E_HAL_HWI2C_DMA_MIU_CH0 , E_HAL_HWI2C_DMA_MIU_CH1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetMiuChannel(MS_U32 u32PortOffset, HAL_HWI2C_DMA_MIUCH eMiuCh)
{
    MS_BOOL bMiuCh1;

    HWI2C_HAL_FUNC();
    if(eMiuCh>=E_HAL_HWI2C_DMA_MIU_MAX)
        return FALSE;
    bMiuCh1 = (eMiuCh==E_HAL_HWI2C_DMA_MIU_CH1)? TRUE : FALSE;
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CTL+u32PortOffset, _DMA_CTL_MIUCHSEL, bMiuCh1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_TxfrDone
/// @brief \b Function  \b Description: Enable interrupt for HWI2C
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_TxfrDone(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_TXR+u32PortOffset, _DMA_TXR_DONE, TRUE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_IsTxfrDone
/// @brief \b Function  \b Description: Check HWI2C DMA Tx done or not
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: DMA TX Done, FALSE: DMA TX Not Done
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_IsTxfrDone(MS_U32 u32PortOffset, MS_U8 u8Port)
{
    HWI2C_HAL_FUNC();
    //##########################
    //
    // [Note] : IMPORTANT !!! 
    // Need to put some delay here,
    // Otherwise, reading data will fail
    //
    //##########################
    if(u8Port>=HAL_HWI2C_PORTS)
        return FALSE;
    HAL_HWI2C_ExtraDelay(g_u16DmaDelayFactor[u8Port]);
    return (HAL_HWI2C_ReadByte(REG_HWI2C_DMA_TXR+u32PortOffset) & _DMA_TXR_DONE) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetTxfrCmd
/// @brief \b Function  \b Description: Set Transfer HWI2C DMA Command & Length
/// @param <IN>         \b pu8CmdBuf : data pointer
/// @param <IN>         \b u8CmdLen : command length
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: cmd len in range, FALSE: cmd len out of range
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetTxfrCmd(MS_U32 u32PortOffset, MS_U8 u8CmdLen, MS_U8* pu8CmdBuf)
{
    MS_U8 k,u8CmdData;
    MS_U32 u32RegAdr;

    HWI2C_HAL_FUNC();
    if(u8CmdLen>HWI2C_DMA_CMD_DATA_LEN)
        return FALSE;
    for( k=0 ; (k<u8CmdLen)&&(k<HWI2C_DMA_CMD_DATA_LEN); k++ )
    {
        u32RegAdr = REG_HWI2C_DMA_CMDDAT0 + k;
        u8CmdData = *(pu8CmdBuf + k);
        HAL_HWI2C_WriteByte(u32RegAdr+u32PortOffset, u8CmdData);
    }
    HAL_HWI2C_WriteByte(REG_HWI2C_DMA_CMDLEN+u32PortOffset, u8CmdLen&_DMA_CMDLEN_MSK);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetCmdLen
/// @brief \b Function  \b Description: Set HWI2C DMA MIU command length
/// @param <IN>         \b u8CmdLen : command length
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetCmdLen(MS_U32 u32PortOffset, MS_U8 u8CmdLen)
{
    HWI2C_HAL_FUNC();
    if(u8CmdLen>HWI2C_DMA_CMD_DATA_LEN)
        return FALSE;
    HAL_HWI2C_WriteByte(REG_HWI2C_DMA_CMDLEN+u32PortOffset, u8CmdLen&_DMA_CMDLEN_MSK);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetDataLen
/// @brief \b Function  \b Description: Set HWI2C DMA data length
/// @param <IN>         \b u32DataLen : data length
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetDataLen(MS_U32 u32PortOffset, MS_U32 u32DataLen)
{
    MS_U32 u32DataLenSet;

    HWI2C_HAL_FUNC();
    u32DataLenSet = u32DataLen;
    return HAL_HWI2C_Write4Byte(REG_HWI2C_DMA_DATLEN+u32PortOffset, u32DataLenSet);
}

#if 0 //will be used later
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_GetTxfrCnt
/// @brief \b Function  \b Description: Get MIIC DMA Transfer Count 
/// @param <IN>         \b u32TxfrCnt : transfer count
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_U32 HAL_HWI2C_DMA_GetTxfrCnt(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();
    return HAL_HWI2C_Read4Byte(REG_HWI2C_DMA_TXFRCNT+u32PortOffset);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_GetAddrMode
/// @brief \b Function  \b Description: Set MIIC DMA Slave Device Address length mode 
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b E_HAL_HWI2C_DMA_ADDR_10BIT(10 bits mode), 
///                                \b E_HAL_HWI2C_DMA_ADDR_NORMAL(7 bits mode)
////////////////////////////////////////////////////////////////////////////////
static HAL_HWI2C_DMA_ADDRMODE HAL_HWI2C_DMA_GetAddrMode(MS_U32 u32PortOffset)
{
    HAL_HWI2C_DMA_ADDRMODE eAddrMode;

    HWI2C_HAL_FUNC();
    if(HAL_HWI2C_ReadByte(REG_HWI2C_DMA_SLVCFG+u32PortOffset) & _DMA_10BIT_MODE)
        eAddrMode = E_HAL_HWI2C_DMA_ADDR_10BIT;
    else
        eAddrMode = E_HAL_HWI2C_DMA_ADDR_NORMAL;
    return eAddrMode;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetSlaveAddr
/// @brief \b Function  \b Description: Set MIIC DMA Slave Device Address 
/// @param <IN>         \b u32TxfrCnt : slave device address
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetSlaveAddr(MS_U32 u32PortOffset, MS_U16 u16SlaveAddr)
{
    HWI2C_HAL_FUNC();
    if(HAL_HWI2C_DMA_GetAddrMode(u32PortOffset)==E_HAL_HWI2C_DMA_ADDR_10BIT)
        return HAL_HWI2C_Write2Byte(REG_HWI2C_DMA_SLVADR+u32PortOffset, u16SlaveAddr&_DMA_SLVADR_10BIT_MSK);
    else
        return HAL_HWI2C_Write2Byte(REG_HWI2C_DMA_SLVADR+u32PortOffset, u16SlaveAddr&_DMA_SLVADR_NORML_MSK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_SetAddrMode
/// @brief \b Function  \b Description: Set MIIC DMA Slave Device Address length mode 
/// @param <IN>         \b eAddrMode : E_HAL_HWI2C_DMA_ADDR_NORMAL, E_HAL_HWI2C_DMA_ADDR_10BIT
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL HAL_HWI2C_DMA_SetAddrMode(MS_U32 u32PortOffset, HAL_HWI2C_DMA_ADDRMODE eAddrMode)
{
    MS_BOOL b10BitMode;
    
    HWI2C_HAL_FUNC();
    if(eAddrMode>=E_HAL_HWI2C_DMA_ADDR_MAX)
        return FALSE;
    b10BitMode = (eAddrMode==E_HAL_HWI2C_DMA_ADDR_10BIT)? TRUE : FALSE;
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_SLVCFG+u32PortOffset, _DMA_10BIT_MODE, b10BitMode);
}

static MS_BOOL HAL_HWI2C_DMA_SetMiuData(MS_U32 u32PortOffset, MS_U32 u32Length, MS_U8* pu8SrcData)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
    MS_U32 u32PhyAddr = 0;
    void __iomem *pMiuData = 0;
    MS_U8 u8Port;

    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;

    u32PhyAddr = (_gu32MIU0_BUS_BASE+g_u32DmaPhyAddr[u8Port]);

    //printf("port %d PA: %x", u8Port, u32PhyAddr);
    if(u32PhyAddr > _gu32MIU0_SIZE)
    {
        HWI2C_HAL_ERR("[IIC][Error][%s] u32PhyAddr is over size = 0x%X \n\n", __FUNCTION__, u32PhyAddr);
        return FALSE;
    }

    if(Addr[u8Port] != NULL)
    {
        pMiuData = Addr[u8Port];
        HWI2C_HAL_INFO("\n[NOT FRC port, already has VA][%s], viraddr: 0x%p, size : %d\n", __func__, pMiuData, u32Length);
        memcpy_toio(pMiuData,(void*)pu8SrcData,u32Length);
        HAL_HWI2C_DMA_SetDataLen(u32PortOffset,u32Length);
    }
    else
    {
        HWI2C_HAL_INFO("\ng_u32DmaPhyAddr[u8Port]=%lu,g_stPortCfg[u8Port].u32PAlen=%lu,u8Port=%d,",g_u32DmaPhyAddr[u8Port],g_stPortCfg[u8Port].u32PAlen,u8Port);
        pMiuData = ioremap_nocache(u32PhyAddr, g_stPortCfg[u8Port].u32PAlen);
        HWI2C_HAL_INFO("\n[NOT FRC port, first map VA][%s], phyaddr: 0x%08x, viraddr: 0x%p, size : %d\n", __func__, u32PhyAddr, pMiuData, g_stPortCfg[u8Port].u32PAlen);
        if(pMiuData == NULL)
        {
            HWI2C_HAL_ERR("[IIC][Error][%s] ioremap_nocache fail! u32PhyAddr = 0x%X \n\n", __FUNCTION__, u32PhyAddr);
            return FALSE;
        }
        Addr[u8Port] = pMiuData;
        memcpy_toio(pMiuData, (void *)pu8SrcData, u32Length);
        HAL_HWI2C_DMA_SetDataLen(u32PortOffset,u32Length);
    }
    return TRUE;
#else
    MS_U32 u32PhyAddr = 0;
    MS_VIRT *pMiuData = 0;
    MS_U8 u8Port;

    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    u32PhyAddr =(MS_U32) g_u32DmaPhyAddr[u8Port];
    pMiuData = (MS_VIRT*)_PA2VA((MS_VIRT)u32PhyAddr);
    memcpy((void*)pMiuData,(void*)pu8SrcData,u32Length);
    HAL_HWI2C_DMA_SetDataLen(u32PortOffset,u32Length);
    return TRUE;
#endif
}

static MS_BOOL HAL_HWI2C_DMA_GetMiuData(MS_U32 u32PortOffset, MS_U32 u32Length, MS_U8* pu8DstData)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
    MS_U32 u32PhyAddr = 0;
    void __iomem *pMiuData = 0;
    MS_U8 u8Port;

    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;

    u32PhyAddr = (_gu32MIU0_BUS_BASE+g_u32DmaPhyAddr[u8Port]);

    if(u32PhyAddr > _gu32MIU0_SIZE)
    {
        HWI2C_HAL_ERR("[IIC][Error][%s] u32PhyAddr is over size = 0x%X \n\n", __FUNCTION__, u32PhyAddr);
        return FALSE;
    }

    if(Addr[u8Port] != NULL)
    {
        pMiuData = Addr[u8Port];
        HWI2C_HAL_INFO("\n[NOT FRC port, already has VA][%s], viraddr: 0x%p, size : %d\n", __func__, pMiuData, u32Length);
        memcpy_fromio((void *)pu8DstData, pMiuData, u32Length);
    }
    else
    {
        HWI2C_HAL_INFO("\ng_u32DmaPhyAddr[u8Port]=%lu,g_stPortCfg[u8Port].u32PAlen=%lu,u8Port=%d,",g_u32DmaPhyAddr[u8Port],g_stPortCfg[u8Port].u32PAlen,u8Port);
        pMiuData = ioremap_nocache(u32PhyAddr, g_stPortCfg[u8Port].u32PAlen);
        HWI2C_HAL_INFO("\n[NOT FRC port, first map VA][%s], phyaddr: 0x%08x, viraddr: 0x%p, size : %d\n", __func__, u32PhyAddr, pMiuData, g_stPortCfg[u8Port].u32PAlen);
        if(pMiuData == NULL)
        {
            HWI2C_HAL_ERR("[IIC][Error][%s] ioremap_nocache fail! u32PhyAddr = 0x%X \n\n", __FUNCTION__, u32PhyAddr);
            return FALSE;
        }
        Addr[u8Port] = pMiuData;
        memcpy_fromio((void *)pu8DstData, pMiuData, u32Length);
    }

    /*spin_unlock_irq(&g_miic_dev[u8Port].lock);*/

    return TRUE;
#else
    MS_U32 u32PhyAddr = 0;
    MS_U8 *pMiuData = 0;
    MS_U8 u8Port;

    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    u32PhyAddr = (MS_U32) g_u32DmaPhyAddr[u8Port];
    pMiuData = (MS_U8*)_PA2VA((MS_VIRT)u32PhyAddr);
    memcpy((void*)pu8DstData,(void*)pMiuData,u32Length);
    return TRUE;
#endif
}

static MS_BOOL HAL_HWI2C_DMA_WaitDone(MS_U32 u32PortOffset, MS_U8 u8ReadWrite, MS_BOOL bWait)
{
    MS_U16 volatile u16Timeout = HWI2C_DMA_WAIT_TIMEOUT;
    MS_U8 u8Port;

    HWI2C_HAL_FUNC();

    //################
    //
    // IMPORTANT HERE !!!
    //
    //#################
	#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
	Chip_Flush_Memory();
	#else
    MsOS_FlushMemory();
	#endif
    //(2-1) reset DMA engine
    HAL_HWI2C_DMA_Reset(u32PortOffset,TRUE);
    HAL_HWI2C_DMA_Reset(u32PortOffset,FALSE);
    //(2-2)  reset MIU module in DMA engine
    HAL_HWI2C_DMA_MiuReset(u32PortOffset,TRUE);
    HAL_HWI2C_DMA_MiuReset(u32PortOffset,FALSE);


    //get port index for delay factor
    HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port);
    //clear transfer dine first for savfty
    HAL_HWI2C_DMA_TxfrDone(u32PortOffset);
    //set command : 0 for Write, 1 for Read
    HAL_HWI2C_DMA_SetRdWrt(u32PortOffset,u8ReadWrite);
    //issue write trigger
    HAL_HWI2C_DMA_Trigger(u32PortOffset);
    //check transfer done
	if (bWait)
	{
    while(u16Timeout--)
    {
        if(HAL_HWI2C_DMA_IsTxfrDone(u32PortOffset,u8Port))
        {
            HAL_HWI2C_DMA_TxfrDone(u32PortOffset);
            HWI2C_HAL_INFO("[DMA]: Transfer DONE!\n");
            return TRUE;
        }
    }
    HWI2C_HAL_ERR("[DMA]: Transfer NOT Completely!\n");
    return FALSE;
}
	else
	{
		return TRUE;
	}
}

static MS_BOOL HAL_HWI2C_DMA_SetDelayFactor(MS_U32 u32PortOffset, HAL_HWI2C_CLKSEL eClkSel)
{
    MS_U8 u8Port;
    
    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
    {
        g_u16DmaDelayFactor[u8Port]=5;
        return FALSE;
    }
    switch(eClkSel)//use Xtal = 24M Hz
    {
        case E_HAL_HWI2C_CLKSEL_HIGH: // 400 KHz
            g_u16DmaDelayFactor[u8Port]=1; break;
        case E_HAL_HWI2C_CLKSEL_NORMAL: //300 KHz
            g_u16DmaDelayFactor[u8Port]=1; break;
        case E_HAL_HWI2C_CLKSEL_SLOW: //200 KHz
            g_u16DmaDelayFactor[u8Port]=1; break; 
        case E_HAL_HWI2C_CLKSEL_VSLOW: //100 KHz
            g_u16DmaDelayFactor[u8Port]=2; break;
        case E_HAL_HWI2C_CLKSEL_USLOW: //50 KHz
            g_u16DmaDelayFactor[u8Port]=3; break;
        case E_HAL_HWI2C_CLKSEL_UVSLOW: //25 KHz
            g_u16DmaDelayFactor[u8Port]=3; break;
        default:
            g_u16DmaDelayFactor[u8Port]=5;
            return FALSE;
    }
    return TRUE;
}

#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
/* Enable/disable interrupt generation */
static inline void miic_interrupt_mode(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
	HAL_HWI2C_DMA_SetINT(u32PortOffset,bEnable);
}

/* ISR */
static irqreturn_t miic_handler(int irq, void *dev_id)
{
    HAL_HWI2C_PortCfg *pstPortCfg = dev_id;
    MS_U32 u32Offset = 0x0000;
    MS_U8 u8Port = 0;
    unsigned long flags;

    HAL_HWI2C_SetPortRegOffset(pstPortCfg->ePort, &u32Offset);
    HAL_HWI2C_GetPortIdxByOffset(u32Offset, &u8Port);
    spin_lock_irqsave(&g_miic_dev[u8Port].lock, flags); //(### REVIEW ###)
    //g_miic_dev[u8Port].ack_result = -1;

    if( HAL_HWI2C_DMA_IsTxfrDone(u32Offset, u8Port) == TRUE )/*dma done*/
    {
        g_miic_dev[u8Port].ack_result = -1;
        //step 0: check ack or not
        //Check if ack is ok?
        if (HAL_HWI2C_Read4Byte(REG_HWI2C_DMA_DATLEN+u32Offset) == HAL_HWI2C_Read4Byte(REG_HWI2C_DMA_TXFRCNT+u32Offset))
        {
             g_miic_dev[u8Port].ack_result = 1;
        }
        else
            HWI2C_HAL_INFO("[iic]%s DATLEN = %d, TXFRCNT = %d \n",__FUNCTION__,HAL_HWI2C_Read4Byte(REG_HWI2C_DMA_DATLEN+u32Offset),HAL_HWI2C_Read4Byte(REG_HWI2C_DMA_TXFRCNT+u32Offset));

        //step 1: clear the "interrupt-pending" bit
        HAL_HWI2C_DMA_TxfrDone(u32Offset);

        HWI2C_HAL_INFO("[iic]%s irq = %d, port = %d, offset = %d\n",__FUNCTION__,irq,u8Port,u32Offset);

        //step 2: is awakening processes sleeping on the device
        if (HAL_HWI2C_ReadByte(REG_HWI2C_DMA_CTL+u32Offset) & _DMA_CTL_RDWTCMD ) {
            /* read interrupt */
            //HWI2C_HAL_INFO("[iic]%s read\n",__FUNCTION__);
            g_miic_dev[u8Port].read_condition = 1;
            wake_up( &g_miic_dev[u8Port].read_wait );
        }else {
            /* write interrupt */
            //HWI2C_HAL_INFO("[iic]%s write\n",__FUNCTION__);
            g_miic_dev[u8Port].write_condition = 1;
            wake_up( &g_miic_dev[u8Port].write_wait );
        }

        spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
        return IRQ_HANDLED;
    }
    else/*not dma interrupt*/
    {
        spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
        return IRQ_NONE;
    }
}
#endif


//#####################
//
//  MIIC STD Related Functions
//  External
//
//#####################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Init_Chip
/// @brief \b Function  \b Description: Init HWI2C chip
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Init_Chip(void)
{
    MS_BOOL bRet = TRUE;

    HWI2C_HAL_FUNC();
    //not set all pads (except SPI) as input
    bRet &= HAL_HWI2C_WriteRegBit(CHIP_REG_ALLPADIN, CHIP_ALLPAD_IN, FALSE);
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_IsMaster
/// @brief \b Function  \b Description: Check if Master I2C
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Master, FALSE: Slave
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_IsMaster(void)
{
    HWI2C_HAL_FUNC();
    return TRUE;
}

#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_Pass_IntEnable
/// @brief \b Function  \b Description: MIIC to dma mode pass int_enable
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Master, FALSE: Slave
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_DMA_Pass_IntEnable(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_ADV_CTL+u32PortOffset, _ADV_DMA_PASS_INT, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Set_Start2ByteDelay_Counter
/// @brief \b Function  \b Description: Set counter value for byte to byte delay
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Master, FALSE: Slave
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Set_Start2ByteDelay_Counter(MS_U32 u32PortOffset, MS_U16 u16DelayCount)
{
    return HAL_HWI2C_Write2Byte(REG_HWI2C_START2B_DELAY_COUNTER+u32PortOffset, u16DelayCount);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Start2ByteDelay_Enable
/// @brief \b Function  \b Description: Enable byte to byte delay
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Master, FALSE: Slave
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Start2ByteDelay_Enable(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_ADV_CTL+u32PortOffset, _ADV_START2B_DELAY, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Byte2ByteDelay_Enable
/// @brief \b Function  \b Description: Enable byte to byte delay
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Master, FALSE: Slave
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Byte2ByteDelay_Enable(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    return HAL_HWI2C_WriteRegBit(REG_HWI2C_ADV_CTL+u32PortOffset, _ADV_BYTE2BYTE_DELAY, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Set_Byte2ByteDelay_Counter
/// @brief \b Function  \b Description: Set counter value for byte to byte delay
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Master, FALSE: Slave
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Set_Byte2ByteDelay_Counter(MS_U32 u32PortOffset, MS_U16 u16DelayCount)
{
    return HAL_HWI2C_Write2Byte(REG_HWI2C_BYTE2BYTE_DELAY_COUNTER+u32PortOffset, u16DelayCount);
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Master_Enable
/// @brief \b Function  \b Description: Master I2C enable
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Master_Enable(MS_U32 u32PortOffset)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
    MS_U8 u8Port;
    MS_BOOL bRet;

    HWI2C_HAL_FUNC();
    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    g_bLastByte[u8Port] = FALSE;

    //(1) clear interrupt
    HAL_HWI2C_EnINT(u32PortOffset,FALSE);
    HAL_HWI2C_Clear_INT(u32PortOffset);
    //(2) reset standard master iic
    HAL_HWI2C_Reset(u32PortOffset,TRUE);
    HAL_HWI2C_Reset(u32PortOffset,FALSE);
    //(3) configuration

    /* Disabe interrupt while Enable DMA interrupt */
    HAL_HWI2C_EnINT(u32PortOffset,!g_stPortCfg[u8Port].bDmaEnable);
    HAL_HWI2C_EnClkStretch(u32PortOffset,TRUE);
    HAL_HWI2C_Byte2ByteDelay_Enable(u32PortOffset,TRUE);
    HAL_HWI2C_EnFilter(u32PortOffset,TRUE);
    HAL_HWI2C_EnPushSda(u32PortOffset,TRUE);

    #if 0
    HAL_HWI2C_EnTimeoutINT(u32PortOffset,TRUE);
    HAL_HWI2C_Write2Byte(REG_HWI2C_TMT_CNT+u32PortOffset, 0x100);
    #endif
    //(4) Disable DMA
    bRet = HAL_HWI2C_DMA_Enable(u32PortOffset,g_stPortCfg[u8Port].bDmaEnable);

    return bRet;
#else
    MS_U8 u8Port;
    MS_BOOL bRet;
    
    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    g_bLastByte[u8Port] = FALSE;

    //(1) clear interrupt
    HAL_HWI2C_Clear_INT(u32PortOffset);
    //(2) reset standard master iic
    HAL_HWI2C_Reset(u32PortOffset,TRUE);
    HAL_HWI2C_Reset(u32PortOffset,FALSE);
    //(3) configuration
    HAL_HWI2C_EnINT(u32PortOffset,TRUE);
    HAL_HWI2C_EnClkStretch(u32PortOffset,TRUE);
    HAL_HWI2C_EnFilter(u32PortOffset,TRUE);
    HAL_HWI2C_EnPushSda(u32PortOffset,TRUE);
    #if 0
    HAL_HWI2C_EnTimeoutINT(u32PortOffset,TRUE);
    HAL_HWI2C_Write2Byte(REG_HWI2C_TMT_CNT+u32PortOffset, 0x100);
    #endif
    //(4) Disable DMA
    bRet = HAL_HWI2C_DMA_Enable(u32PortOffset,FALSE);

    return bRet;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_SetPortRegOffset
/// @brief \b Function  \b Description: Set HWI2C port register offset
/// @param <IN>         \b ePort : HWI2C port number
/// @param <OUT>         \b pu32PortOffset : port register offset
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_SetPortRegOffset(HAL_HWI2C_PORT ePort, MS_U32* pu32PortOffset)
{
    HWI2C_HAL_FUNC();

    if((ePort>=E_HAL_HWI2C_PORT0_0)&&(ePort<=E_HAL_HWI2C_PORT0_7))
    {//port 0 : bank register address 0x111800
        *pu32PortOffset = (MS_U32)0x00;
    }
    else if((ePort>=E_HAL_HWI2C_PORT1_0)&&(ePort<=E_HAL_HWI2C_PORT1_7))
    {//port 1 : bank register address 0x111900
        *pu32PortOffset = (MS_U32)0x100;
    }
    else if((ePort>=E_HAL_HWI2C_PORT2_0)&&(ePort<=E_HAL_HWI2C_PORT2_7))
    {//port 2 : bank register address 0x111A00
        *pu32PortOffset = (MS_U32)0x200; 
    }
    else if((ePort>=E_HAL_HWI2C_PORT3_0)&&(ePort<=E_HAL_HWI2C_PORT3_7))
    {//port 3 : bank register address 0x111B00
        *pu32PortOffset = (MS_U32)0x300; 
    }
	else if((ePort>=E_HAL_HWI2C_PORT4_0)&&(ePort<=E_HAL_HWI2C_PORT4_7))
    {//port 4 : bank register address 0x121C00
        *pu32PortOffset = (MS_U32)0x10400; 
    }
	else if((ePort>=E_HAL_HWI2C_PORT5_0)&&(ePort<=E_HAL_HWI2C_PORT5_7))
    {//port 5 : bank register address 0x122C00
        *pu32PortOffset = (MS_U32)0x10500; 
    }
    else
    {
        *pu32PortOffset = (MS_U32)0x00;
        return FALSE;
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_GetPortIdxByRegOffset
/// @brief \b Function  \b Description: Get HWI2C port index by register offset
/// @param <IN>         \b u16Offset : port register offset
/// @param <OUT>         \b pu8Port :  port index
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_GetPortIdxByOffset(MS_U32 u32PortOffset, MS_U8* pu8Port)
{
    HWI2C_HAL_FUNC();

    if(u32PortOffset==(MS_U32)0x00)
    {//port 0 : bank register address 0x11800
        *pu8Port = HAL_HWI2C_PORT0;
    }
    else if(u32PortOffset==(MS_U32)0x100)
    {//port 1 : bank register address 0x11900
        *pu8Port = HAL_HWI2C_PORT1;
    }
    else if(u32PortOffset==(MS_U32)0x200)
    {//port 2 : bank register address 0x11A00
        *pu8Port = HAL_HWI2C_PORT2;
    }
    else if(u32PortOffset==(MS_U32)0x300)
    {//port 3 : bank register address 0x11B00
        *pu8Port = HAL_HWI2C_PORT3;
    }
	else if(u32PortOffset==(MS_U32)0x10400)
    {//port 3 : bank register address 0x121C00
        *pu8Port = HAL_HWI2C_PORT4;
    }
	else if(u32PortOffset==(MS_U32)0x10500)
    {//port 3 : bank register address 0x122C00
        *pu8Port = HAL_HWI2C_PORT5;
    }
    else
    {
        *pu8Port = HAL_HWI2C_PORT0;
        return FALSE;
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_GetPortIdxByPort
/// @brief \b Function  \b Description: Get HWI2C port index by port number
/// @param <IN>         \b ePort : port number
/// @param <OUT>         \b pu8Port :  port index
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_GetPortIdxByPort(HAL_HWI2C_PORT ePort, MS_U8* pu8Port)
{
    HWI2C_HAL_FUNC();

    if((ePort>=E_HAL_HWI2C_PORT0_0)&&(ePort<=E_HAL_HWI2C_PORT0_7))
    {
        *pu8Port = HAL_HWI2C_PORT0;
    }
    else if((ePort>=E_HAL_HWI2C_PORT1_0)&&(ePort<=E_HAL_HWI2C_PORT1_7))
    {
        *pu8Port = HAL_HWI2C_PORT1;
    }
    else if((ePort>=E_HAL_HWI2C_PORT2_0)&&(ePort<=E_HAL_HWI2C_PORT2_7))
    {
        *pu8Port = HAL_HWI2C_PORT2;
    }
    else if((ePort>=E_HAL_HWI2C_PORT3_0)&&(ePort<=E_HAL_HWI2C_PORT3_7))
    {
        *pu8Port = HAL_HWI2C_PORT3;
    }
	 else if((ePort>=E_HAL_HWI2C_PORT4_0)&&(ePort<=E_HAL_HWI2C_PORT4_7))
    {
        *pu8Port = HAL_HWI2C_PORT4;
    }
	  else if((ePort>=E_HAL_HWI2C_PORT5_0)&&(ePort<=E_HAL_HWI2C_PORT5_7))
    {
        *pu8Port = HAL_HWI2C_PORT5;
    }
    else
    {
        *pu8Port = HAL_HWI2C_PORT0;
        return FALSE;
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_SelectPort
/// @brief \b Function  \b Description: Select HWI2C port
/// @param <IN>         \b None : HWI2C port
/// @param param        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_SelectPort(HAL_HWI2C_PORT ePort)
{
    MS_U8 u8Value1=0;

    HWI2C_HAL_FUNC();

    //decide port mask
    if((ePort>=E_HAL_HWI2C_PORT0_0)&&(ePort<=E_HAL_HWI2C_PORT0_7))//port 0
    {
        switch(ePort)
        {
            case E_HAL_HWI2C_PORT0_0: //miic0 disable
                HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC0, CHIP_MIIC0_PAD_0, CHIP_MIIC0_PAD_MSK);
                break;
            case E_HAL_HWI2C_PORT0_1: //miic0 using PAD_GPIO28/PAD_GPIO29
                 HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC0, CHIP_MIIC0_PAD_1, CHIP_MIIC0_PAD_MSK);
                break;
            default:
                return FALSE;
        }
    }
    else if((ePort>=E_HAL_HWI2C_PORT1_0)&&(ePort<=E_HAL_HWI2C_PORT1_7))//port 1
    {
        switch(ePort)
        {
            case E_HAL_HWI2C_PORT1_0: //miic1 disable
                u8Value1 = CHIP_MIIC1_PAD_0;
                break;
            case E_HAL_HWI2C_PORT1_1: //miic1 using PAD_TGPIO2/PAD_TGPIO3
                u8Value1 = CHIP_MIIC1_PAD_1;
                break;
            default:
                return FALSE;
        }
        HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC1, u8Value1, CHIP_MIIC1_PAD_MSK);
    }
    else if((ePort>=E_HAL_HWI2C_PORT2_0)&&(ePort<=E_HAL_HWI2C_PORT2_7))//port 2
    {
        HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC2, CHIP_MIIC2_PAD_0, CHIP_MIIC2_PAD_MSK);
        HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC3, CHIP_MIIC3_PAD_0, CHIP_MIIC3_PAD_MSK);
        switch(ePort)
        {
            case E_HAL_HWI2C_PORT2_0: //miic2 disable
                break;
            case E_HAL_HWI2C_PORT2_1: //miic2 using PAD_I2S_IN_BCK/PAD_I2S_IN_SD
        		HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC2, CHIP_MIIC2_PAD_1, CHIP_MIIC2_PAD_MSK);
                break;
			case E_HAL_HWI2C_PORT2_2:  //miic3 using PAD_GPIO36/PAD_GPIO37
				HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC3, CHIP_MIIC3_PAD_1, CHIP_MIIC3_PAD_MSK);
				break;
            default:
                return FALSE;
        }
    }
    else if((ePort>=E_HAL_HWI2C_PORT3_0)&&(ePort<=E_HAL_HWI2C_PORT3_7))//port 3
    {
        HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_DDCR, CHIP_DDCR_PAD_0, CHIP_DDCR_PAD_MSK);
        switch(ePort)
        {
            case E_HAL_HWI2C_PORT3_0: //ddcr_miic disable
        		HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_DDCR, CHIP_DDCR_PAD_0, CHIP_DDCR_PAD_MSK);
                break;
            case E_HAL_HWI2C_PORT3_1: //ddcr_miic using PAD_DDCR_CK/PAD_DDCR_DA
                HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_DDCR, CHIP_DDCR_PAD_1, CHIP_DDCR_PAD_MSK); 
                break;
            default:
                return FALSE;
        }
    }
	else if((ePort>=E_HAL_HWI2C_PORT4_0)&&(ePort<=E_HAL_HWI2C_PORT4_7))//port 4
    {
        switch(ePort)
        {
            case E_HAL_HWI2C_PORT4_0: //miic2 disable
                u8Value1 = CHIP_MIIC4_PAD_0;
                break;
            case E_HAL_HWI2C_PORT4_1: //miic2 using PAD_GPIO30/PAD_GPIO31
                u8Value1 = CHIP_MIIC4_PAD_1;
                break;
            default:
                return FALSE;
        }
        HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC4, u8Value1, CHIP_MIIC4_PAD_MSK);
    }
	else if((ePort>=E_HAL_HWI2C_PORT5_0)&&(ePort<=E_HAL_HWI2C_PORT5_7))//port 5
    {
        switch(ePort)
        {
            case E_HAL_HWI2C_PORT5_0: //miic2 disable
                u8Value1 = CHIP_MIIC5_PAD_0;
                break;
            case E_HAL_HWI2C_PORT5_1: //miic2 using PAD_GPIO32/PAD_GPIO33
                u8Value1 = CHIP_MIIC5_PAD_1;
                break;
            default:
                return FALSE;
        }
        HAL_HWI2C_WriteByteMask(CHIP_REG_HWI2C_MIIC5, u8Value1, CHIP_MIIC5_PAD_MSK);
    }
    else
    {
        return FALSE;
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_SetClk
/// @brief \b Function  \b Description: Set I2C clock
/// @param <IN>         \b u8Clk: clock rate
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_SetClk(MS_U32 u32PortOffset, HAL_HWI2C_CLKSEL eClkSel)
{
    MS_U16 u16ClkHCnt=0,u16ClkLCnt=0;
    MS_U16 u16StpCnt=0,u16SdaCnt=0,u16SttCnt=0,u16LchCnt=0;

    HWI2C_HAL_FUNC();

    if(eClkSel>=E_HAL_HWI2C_CLKSEL_NOSUP)
        return FALSE;

    switch(eClkSel)//use Xtal = 12M Hz
    {
        case E_HAL_HWI2C_CLKSEL_HIGH: // 400 KHz
            u16ClkHCnt =   9; u16ClkLCnt =   15; break;
        case E_HAL_HWI2C_CLKSEL_NORMAL: //300 KHz
            u16ClkHCnt =  15; u16ClkLCnt =   17; break;
        case E_HAL_HWI2C_CLKSEL_SLOW: //200 KHz
            u16ClkHCnt =  25; u16ClkLCnt =   27; break; 
        case E_HAL_HWI2C_CLKSEL_VSLOW: //100 KHz
            u16ClkHCnt =  55; u16ClkLCnt =   57; break;
        case E_HAL_HWI2C_CLKSEL_USLOW: //50 KHz
            u16ClkHCnt =  115; u16ClkLCnt = 117; break;
        case E_HAL_HWI2C_CLKSEL_UVSLOW: //25 KHz
            u16ClkHCnt =  235; u16ClkLCnt = 237; break;
        default:
            u16ClkHCnt =  15; u16ClkLCnt =  17; break;
    }
    u16SttCnt=8; u16StpCnt=8; u16SdaCnt=5; u16LchCnt=5; 

    HAL_HWI2C_Write2Byte(REG_HWI2C_CKH_CNT+u32PortOffset, u16ClkHCnt);
    HAL_HWI2C_Write2Byte(REG_HWI2C_CKL_CNT+u32PortOffset, u16ClkLCnt);
    HAL_HWI2C_Write2Byte(REG_HWI2C_STP_CNT+u32PortOffset, u16StpCnt);
    HAL_HWI2C_Write2Byte(REG_HWI2C_SDA_CNT+u32PortOffset, u16SdaCnt);
    HAL_HWI2C_Write2Byte(REG_HWI2C_STT_CNT+u32PortOffset, u16SttCnt);
    HAL_HWI2C_Write2Byte(REG_HWI2C_LTH_CNT+u32PortOffset, u16LchCnt);
    //HAL_HWI2C_Write2Byte(REG_HWI2C_TMT_CNT+u32PortOffset, 0x0000);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Start
/// @brief \b Function  \b Description: Send start condition
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Start(MS_U32 u32PortOffset)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE

    MS_U16 u16Count = HWI2C_HAL_WAIT_TIMEOUT;
    HWI2C_HAL_FUNC();

    //let SDA delay 2T
    HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_CMDDAT7+u32PortOffset, _DMA_CMDDAT7_MSK, TRUE); //(### REVIEW ###)
    //reset I2C
    HAL_HWI2C_WriteRegBit(REG_HWI2C_CMD_START+u32PortOffset, _CMD_START, TRUE);
    while((!HAL_HWI2C_Is_INT(u32PortOffset))&&(u16Count > 0))
        u16Count--;
    HAL_HWI2C_Clear_INT(u32PortOffset);
    return (u16Count)? TRUE:FALSE;
#else
    MS_U16 u16Count = HWI2C_HAL_WAIT_TIMEOUT;

    HWI2C_HAL_FUNC();

    //reset I2C
    HAL_HWI2C_WriteRegBit(REG_HWI2C_CMD_START+u32PortOffset, _CMD_START, TRUE);
    while((!HAL_HWI2C_Is_INT(u32PortOffset))&&(u16Count > 0))
        u16Count--;
    HAL_HWI2C_Clear_INT(u32PortOffset);
    return (u16Count)? TRUE:FALSE;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Stop
/// @brief \b Function  \b Description: Send Stop condition
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Stop(MS_U32 u32PortOffset)
{
    MS_U16 u16Count = HWI2C_HAL_WAIT_TIMEOUT;

    HWI2C_HAL_FUNC();

    HAL_HWI2C_WriteRegBit(REG_HWI2C_CMD_STOP+u32PortOffset, _CMD_STOP, TRUE);
    while((!HAL_HWI2C_Is_Idle(u32PortOffset))&&(!HAL_HWI2C_Is_INT(u32PortOffset))&&(u16Count > 0))
        u16Count--;
    HAL_HWI2C_Clear_INT(u32PortOffset);
    return (u16Count)? TRUE:FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_ReadRdy
/// @brief \b Function  \b Description: Start byte reading
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_ReadRdy(MS_U32 u32PortOffset)
{
    MS_U8 u8Value=0;
    MS_U8 u8Port;
    
    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    u8Value = (g_bLastByte[u8Port])? (_RDATA_CFG_TRIG|_RDATA_CFG_ACKBIT) : (_RDATA_CFG_TRIG);
    g_bLastByte[u8Port] = FALSE;
    return HAL_HWI2C_WriteByte(REG_HWI2C_RDATA_CFG+u32PortOffset, u8Value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_SendData
/// @brief \b Function  \b Description: Send 1 byte data to SDA
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_SendData(MS_U32 u32PortOffset, MS_U8 u8Data)
{
    HWI2C_HAL_FUNC();

    return HAL_HWI2C_WriteByte(REG_HWI2C_WDATA+u32PortOffset, u8Data);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_RecvData
/// @brief \b Function  \b Description: Receive 1 byte data from SDA
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8 :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_HWI2C_RecvData(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();

    return HAL_HWI2C_ReadByte(REG_HWI2C_RDATA+u32PortOffset);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Get_SendAck
/// @brief \b Function  \b Description: Get ack after sending data
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Valid ack, FALSE: No ack
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Get_SendAck(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();

    return (HAL_HWI2C_ReadByte(REG_HWI2C_WDATA_GET+u32PortOffset) & _WDATA_GET_ACKBIT) ? FALSE : TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_NoAck
/// @brief \b Function  \b Description: generate no ack pulse
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_NoAck(MS_U32 u32PortOffset)
{
    MS_U8 u8Port;
    
    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    g_bLastByte[u8Port] = TRUE;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Ack
/// @brief \b Function  \b Description: generate ack pulse
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Ack(MS_U32 u32PortOffset)
{
    MS_U8 u8Port;

    HWI2C_HAL_FUNC();

    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port)==FALSE)
        return FALSE;
    g_bLastByte[u8Port] = FALSE;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_GetStae
/// @brief \b Function  \b Description: Get i2c Current State
/// @param <IN>         \b u32PortOffset: HWI2C Port Offset
/// @param <OUT>        \b None
/// @param <RET>        \b HWI2C current status
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_HWI2C_GetState(MS_U32 u32PortOffset)
{
	HWI2C_HAL_FUNC();
	
	MS_U8 cur_state =  HAL_HWI2C_ReadByte(REG_HWI2C_CUR_STATE+u32PortOffset) & _CUR_STATE_MSK;

	if (cur_state <= 0) // 0: idle
		return E_HAL_HWI2C_STATE_IDEL;
	else if (cur_state <= 2) // 1~2:start 
		return E_HAL_HWI2C_STATE_START;
	else if (cur_state <= 6) // 3~6:write
		return E_HAL_HWI2C_STATE_WRITE;
	else if (cur_state <= 10) // 7~10:read
		return E_HAL_HWI2C_STATE_READ;
	else if (cur_state <= 11) // 11:interrupt
		return E_HAL_HWI2C_STATE_INT;
	else if (cur_state <= 12) // 12:wait
		return E_HAL_HWI2C_STATE_WAIT;
	else  // 13~15:stop
		return E_HAL_HWI2C_STATE_STOP;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Is_Idle
/// @brief \b Function  \b Description: Check if i2c is idle
/// @param <IN>         \b u32PortOffset: HWI2C Port Offset
/// @param <OUT>        \b None
/// @param <RET>        \b TRUE : idle, FALSE : not idle
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Is_Idle(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();

    return ((HAL_HWI2C_GetState(u32PortOffset)==E_HAL_HWI2C_STATE_IDEL) ? TRUE : FALSE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Is_INT
/// @brief \b Function  \b Description: Check if i2c is interrupted
/// @param <IN>         \b u8Status : queried status
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Is_INT(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();

    return (HAL_HWI2C_ReadByte(REG_HWI2C_INT_CTL+u32PortOffset) & _INT_CTL) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Clear_INT
/// @brief \b Function  \b Description: Enable interrupt for HWI2C
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Clear_INT(MS_U32 u32PortOffset)
{
    HWI2C_HAL_FUNC();

    return HAL_HWI2C_WriteRegBit(REG_HWI2C_INT_CTL+u32PortOffset, _INT_CTL, TRUE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Reset
/// @brief \b Function  \b Description: Reset HWI2C state machine
/// @param <IN>         \b bReset : TRUE: Reset FALSE: Not reset
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok, FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Reset(MS_U32 u32PortOffset, MS_BOOL bReset)
{
    HWI2C_HAL_FUNC();

    return HAL_HWI2C_WriteRegBit(REG_HWI2C_MIIC_CFG+u32PortOffset, _MIIC_CFG_RESET, bReset);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Send_Byte
/// @brief \b Function  \b Description: Send one byte
/// @param u8Data       \b IN: 1 byte data
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Send_Byte(MS_U32 u32PortOffset, MS_U8 u8Data)
{
    MS_U8 u8Retry = HWI2C_HAL_RETRY_TIMES;
    MS_U16 u16Count = HWI2C_HAL_WAIT_TIMEOUT;

    HWI2C_HAL_FUNC();

    while(u8Retry--)
    {
        HAL_HWI2C_Clear_INT(u32PortOffset);
        if (HAL_HWI2C_SendData(u32PortOffset,u8Data))
        {
            u16Count = HWI2C_HAL_WAIT_TIMEOUT;
            while(u16Count--)
            {
                if (HAL_HWI2C_Is_INT(u32PortOffset))
                {
                    HAL_HWI2C_Clear_INT(u32PortOffset);
                    if (HAL_HWI2C_Get_SendAck(u32PortOffset))
                    {
                        #if 1
                        HAL_HWI2C_ExtraDelay(1);
                        #else
                        MsOS_DelayTaskUs(1);
                        #endif
                        return TRUE;
                    }
                    break;
                }
            }
        }
    }
    HWI2C_HAL_ERR("Send byte 0x%X fail!\n", u8Data);
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Recv_Byte
/// @brief \b Function  \b Description: Init HWI2C driver and auto generate ACK
/// @param *pData       \b Out: received data
/// @return             \b TRUE: Success FALSE: Fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_Recv_Byte(MS_U32 u32PortOffset, MS_U8 *pData)
{
    MS_U16 u16Count = HWI2C_HAL_WAIT_TIMEOUT;

    HWI2C_HAL_FUNC();

    if (!pData)
        return FALSE;

    HAL_HWI2C_ReadRdy(u32PortOffset);
    while((!HAL_HWI2C_Is_INT(u32PortOffset))&&(u16Count > 0))
        u16Count--;
    HAL_HWI2C_Clear_INT(u32PortOffset);
    if (u16Count)
    {
        //get data before clear int and stop
        *pData = HAL_HWI2C_RecvData(u32PortOffset);
        //clear interrupt
        HAL_HWI2C_Clear_INT(u32PortOffset);
        #if 1
        HAL_HWI2C_ExtraDelay(1);
        #else
        MsOS_DelayTaskUs(1);
        #endif
        return TRUE;
    }
    HWI2C_HAL_ERR("Recv byte fail!\n");
    return FALSE;
}

//#####################
//
//  MIIC DMA Related Functions
//  External
//
//#####################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_Enable
/// @brief \b Function  \b Description: Enable HWI2C DMA
/// @param <IN>         \b bEnable : TRUE: enable DMA, FALSE: disable DMA
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_DMA_Enable(MS_U32 u32PortOffset, MS_BOOL bEnable)
{
    MS_BOOL bRet=TRUE;

    HWI2C_HAL_FUNC();

    bRet &= HAL_HWI2C_DMA_SetINT(u32PortOffset,bEnable);
    bRet &= HAL_HWI2C_EnDMA(u32PortOffset,bEnable);
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_Init
/// @brief \b Function  \b Description: Initialize HWI2C DMA
/// @param <IN>         \b pstCfg : Init structure
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_DMA_Init(MS_U32 u32PortOffset, HAL_HWI2C_PortCfg* pstPortCfg)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
    MS_U8 u8Port = 0;
    MS_U8 i = 0;
    MS_BOOL bRet=TRUE;
    MS_BOOL result=1;
    HWI2C_HAL_FUNC();
    //check pointer
    if(!pstPortCfg)
    {
        HWI2C_HAL_ERR("Port cfg null pointer!\n");
        return FALSE;
    }

    //(1) clear interrupt
    HAL_HWI2C_DMA_TxfrDone(u32PortOffset);
    //(2) reset DMA
    //(2-1) reset DMA engine
    HAL_HWI2C_DMA_Reset(u32PortOffset,TRUE);
    HAL_HWI2C_DMA_Reset(u32PortOffset,FALSE);
    //(2-2)  reset MIU module in DMA engine
    HAL_HWI2C_DMA_MiuReset(u32PortOffset,TRUE);
    HAL_HWI2C_DMA_MiuReset(u32PortOffset,FALSE);
    //(3) default configursation
    bRet &= HAL_HWI2C_DMA_SetAddrMode(u32PortOffset,pstPortCfg->eDmaAddrMode);
    bRet &= HAL_HWI2C_DMA_SetMiuPri(u32PortOffset,pstPortCfg->eDmaMiuPri);
    bRet &= HAL_HWI2C_DMA_SetMiuChannel(u32PortOffset,pstPortCfg->eDmaMiuCh);
    bRet &= HAL_HWI2C_DMA_SetMiuAddr(u32PortOffset,pstPortCfg->u32DmaPhyAddr);
    bRet &= HAL_HWI2C_DMA_Enable(u32PortOffset,pstPortCfg->bDmaEnable);
    bRet &= HAL_HWI2C_DMA_SetDelayFactor(u32PortOffset,pstPortCfg->eSpeed);
    //bRet &= HAL_HWI2C_WriteRegBit(REG_HWI2C_DMA_RESERV+u32PortOffset, _DMA_RESERV_CTL, 1);
    //(4) backup configuration info
    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port))
    {
        memcpy(&g_stPortCfg[u8Port], pstPortCfg, sizeof(HAL_HWI2C_PortCfg));
    }


    if(u8ResumeFlag == 0)
    {
        if(g_stPortCfg[u8Port].bDmaEnable != 0)
        {
            g_miic_dev[u8Port].timeout = HWI2C_HAL_WAIT_TIMEOUT;
            spin_lock_init(&g_miic_dev[u8Port].lock);
            init_waitqueue_head( &g_miic_dev[u8Port].read_wait );
            init_waitqueue_head( &g_miic_dev[u8Port].write_wait );

            //iic dma mode supports the interrupt.
            if ( g_stPortCfg[u8Port].irq > 0 ) {
                HAL_HWI2C_DMA_Pass_IntEnable(u32PortOffset,TRUE);

                result = request_irq(g_stPortCfg[u8Port].irq, miic_handler, IRQF_DISABLED,
                                        "mstar_iic", &g_stPortCfg[u8Port]);

                if ( result ) {
                    HWI2C_HAL_INFO("mstar iic : result(%d) can't get assigned irq %i\n",
                          result, g_stPortCfg[u8Port].irq);
                    g_stPortCfg[u8Port].irq = -1;
                }
            }
        }
    }
    else
    {
        if(g_stPortCfg[u8Port].bDmaEnable != 0)
            HAL_HWI2C_DMA_Pass_IntEnable(u32PortOffset,TRUE);
    }
    return bRet;
#else
    MS_U8 u8Port = 0;
    MS_BOOL bRet=TRUE;
    
    HWI2C_HAL_FUNC();

    //check pointer
    if(!pstPortCfg)
    {
        HWI2C_HAL_ERR("Port cfg null pointer!\n");
        return FALSE;
    }
    //(1) clear interrupt    
    HAL_HWI2C_DMA_TxfrDone(u32PortOffset);
    //(2) reset DMA
    //(2-1) reset DMA engine
    HAL_HWI2C_DMA_Reset(u32PortOffset,TRUE);
    HAL_HWI2C_DMA_Reset(u32PortOffset,FALSE);
    //(2-2)  reset MIU module in DMA engine
    HAL_HWI2C_DMA_MiuReset(u32PortOffset,TRUE);
    HAL_HWI2C_DMA_MiuReset(u32PortOffset,FALSE);
    //(3) default configursation
    bRet &= HAL_HWI2C_DMA_SetAddrMode(u32PortOffset,pstPortCfg->eDmaAddrMode);
    bRet &= HAL_HWI2C_DMA_SetMiuPri(u32PortOffset,pstPortCfg->eDmaMiuPri);
    bRet &= HAL_HWI2C_DMA_SetMiuChannel(u32PortOffset,pstPortCfg->eDmaMiuCh);
    bRet &= HAL_HWI2C_DMA_SetMiuAddr(u32PortOffset,pstPortCfg->u32DmaPhyAddr);
    bRet &= HAL_HWI2C_DMA_Enable(u32PortOffset,pstPortCfg->bDmaEnable);
    bRet &= HAL_HWI2C_DMA_SetDelayFactor(u32PortOffset,pstPortCfg->eSpeed);
    //(4) backup configuration info
    if(HAL_HWI2C_GetPortIdxByOffset(u32PortOffset,&u8Port))
    {
        memcpy(&g_stPortCfg[u8Port], pstPortCfg, sizeof(HAL_HWI2C_PortCfg));
    }

    return bRet;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_WriteBytes
/// @brief \b Function  \b Description: Initialize HWI2C DMA
/// @param <IN>         \b u16SlaveCfg : slave id
/// @param <IN>         \b uAddrCnt : address size in bytes
/// @param <IN>         \b pRegAddr : address pointer
/// @param <IN>         \b uSize : data size in bytes
/// @param <IN>         \b pData : data pointer
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_DMA_WriteBytes(MS_U32 u32PortOffset, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
    int ret = 0;
    MS_U8 u8Port = 0;
    MS_U8 u8SlaveAddr = LOW_BYTE(u16SlaveCfg)>>1;

    HWI2C_HAL_FUNC();

    if (!pRegAddr)
        uAddrCnt = 0;
    if (!pData)
        uSize = 0;
    //no meaning operation
    if (!uSize)
    {
        HWI2C_HAL_ERR("[DMA_W]: No data for writing!\n");
        return FALSE;
    }
    HAL_HWI2C_GetPortIdxByOffset(u32PortOffset, &u8Port);
    //spin_lock_irqsave(&g_miic_dev[u8Port].lock, flags);
    miic_interrupt_mode(u32PortOffset, 0);

    //set transfer with stop
    HAL_HWI2C_DMA_SetTxfrStop(u32PortOffset,TRUE);
    //set slave address
    HAL_HWI2C_DMA_SetSlaveAddr(u32PortOffset,u8SlaveAddr);

    //#################
    //  Set WRITE command
    //#################
    //set command buffer
    if(HAL_HWI2C_DMA_SetTxfrCmd(u32PortOffset,(MS_U8)uAddrCnt,pRegAddr)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_W]: Set command buffer error!\n");
        //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
        return FALSE;
    }
    //set data to dram
    if(HAL_HWI2C_DMA_SetMiuData(u32PortOffset,uSize,pData)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_W]: Set MIU data error!\n");
        //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
        return FALSE;
    }

    if ( g_stPortCfg[u8Port].irq > 0) {
            //Reset write Condiftion of ISR
            g_miic_dev[u8Port].write_condition = 0;
            g_miic_dev[u8Port].ack_result = -1;
            /* Interrupt mode */
            miic_interrupt_mode(u32PortOffset, 1);
            //##################
            //  Trigger to WRITE
            HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_WRITE, 0);
            //printf("\nu8Port=%d %x\n",u8Port, g_miic_dev[u8Port].write_wait);
            ret = wait_event_timeout(g_miic_dev[u8Port].write_wait,
                    g_miic_dev[u8Port].write_condition,
                    g_miic_dev[u8Port].timeout);
            if(ret == 0)
            {
                HWI2C_HAL_INFO("[iic] (%s) wait timeout. (Port:%d;Ret:%d)\n",__FUNCTION__, u8Port, ret);
                return FALSE;
            }

    }
    else {
            /* Polling mode */
            //##################
            //  Trigger to WRITE
            if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_WRITE, 1)==FALSE)
            {
                HWI2C_HAL_ERR("[IIC DMA_W]: Polling Transfer command error!\n");
                return FALSE;
            }
    }

    //Debug message
    if(HAL_HWI2C_Get_SendAck(u32PortOffset) == 0)
        HWI2C_HAL_INFO("[IIC_Debug] No Ack \n\n");

    if( g_miic_dev[u8Port].ack_result > 0 )
        return TRUE;
    else
        return FALSE;
#else
    MS_U8 u8SlaveAddr = LOW_BYTE(u16SlaveCfg)>>1;

    HWI2C_HAL_FUNC();

    if (!pRegAddr) 
        uAddrCnt = 0;
    if (!pData)
        uSize = 0;
    //no meaning operation
    if (!uSize)
    {
        HWI2C_HAL_ERR("[DMA_W]: No data for writing!\n");
        return FALSE;
    }
    
    //set transfer with stop
    HAL_HWI2C_DMA_SetTxfrStop(u32PortOffset,TRUE);
    //set slave address
    HAL_HWI2C_DMA_SetSlaveAddr(u32PortOffset,u8SlaveAddr);

    //#################
    //  Set WRITE command
    //#################
    //set command buffer
    if(HAL_HWI2C_DMA_SetTxfrCmd(u32PortOffset,(MS_U8)uAddrCnt,pRegAddr)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_W]: Set command buffer error!\n");
        return FALSE;
    }
    //set data to dram
    if(HAL_HWI2C_DMA_SetMiuData(u32PortOffset,uSize,pData)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_W]: Set MIU data error!\n");
        return FALSE;
    }
    //##################
    //  Trigger to WRITE 
    if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_WRITE, 1)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_W]: Transfer command error!\n");
        return FALSE;
    }

    return TRUE;
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_DMA_ReadBytes
/// @brief \b Function  \b Description: Initialize HWI2C DMA
/// @param <IN>         \b u16SlaveCfg : slave id
/// @param <IN>         \b uAddrCnt : address size in bytes
/// @param <IN>         \b pRegAddr : address pointer
/// @param <IN>         \b uSize : data size in bytes
/// @param <IN>         \b pData : data pointer
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE : ok, FALSE : fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_HWI2C_DMA_ReadBytes(MS_U32 u32PortOffset, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData)
{
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
    int ret = 0;
    MS_U8 u8SlaveAddr = LOW_BYTE(u16SlaveCfg)>>1;
    MS_U8 u8Port = HIGH_BYTE(u16SlaveCfg);
    HAL_HWI2C_ReadMode eReadMode;

    HWI2C_HAL_FUNC();

    if (!pRegAddr) 
        uAddrCnt = 0;
    if (!pData)
        uSize = 0;
    //no meaning operation
    if (!uSize)
    {
        HWI2C_HAL_ERR("[DMA_R]: No data for reading!\n");
        return FALSE;
    }

    HAL_HWI2C_GetPortIdxByOffset(u32PortOffset, &u8Port);

    if (u8Port>=HAL_HWI2C_PORTS)
    {
        HWI2C_HAL_ERR("[DMA_R]: Port failure!\n");
        return FALSE;
    }

    eReadMode = g_stPortCfg[u8Port].eReadMode;
    if(eReadMode>=E_HAL_HWI2C_READ_MODE_MAX)
    {
        HWI2C_HAL_ERR("[DMA_R]: Read mode failure!\n");
        return FALSE;
    }

    miic_interrupt_mode(u32PortOffset, 0);

    if(eReadMode!=E_HAL_HWI2C_READ_MODE_DIRECT)
    {
        //set transfer read mode
        HAL_HWI2C_DMA_SetReadMode(u32PortOffset,eReadMode);
        //set slave address
        HAL_HWI2C_DMA_SetSlaveAddr(u32PortOffset,u8SlaveAddr);

        //#################
        //  Set WRITE command
        //#################
        //set command buffer
        if(HAL_HWI2C_DMA_SetTxfrCmd(u32PortOffset,(MS_U8)uAddrCnt,pRegAddr)==FALSE)
        {
            HWI2C_HAL_ERR("[DMA_R:W]: Set command buffer error!\n");
            //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
            return FALSE;
        }
        HAL_HWI2C_DMA_SetDataLen(u32PortOffset, 0);

        //##################
        //  Trigger to WRITE
        if ( uAddrCnt != 0 )
        {
            if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_WRITE, 1)==FALSE)
            {
                HWI2C_HAL_ERR("[DMA_R:W]: Transfer command error!\n");
                return FALSE;
            }
        }
    }

    //#################
    //  Set READ command
    //#################
    //set transfer with stop
    HAL_HWI2C_DMA_SetTxfrStop(u32PortOffset,TRUE);
    //set slave address
    HAL_HWI2C_DMA_SetSlaveAddr(u32PortOffset,u8SlaveAddr);
    //set command length to 0
    HAL_HWI2C_DMA_SetCmdLen(u32PortOffset,0);
    //set command length for reading
    HAL_HWI2C_DMA_SetDataLen(u32PortOffset,uSize);

    if ( g_stPortCfg[u8Port].irq > 0 ) {
        //Reset read Condition of ISR
        g_miic_dev[u8Port].read_condition = 0;
        g_miic_dev[u8Port].ack_result = -1;
        /* Interrupt mode */
        miic_interrupt_mode(u32PortOffset, 1);
        //##################
        //  Trigger to READ
        if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_READ, 0)==FALSE)
        {
            HWI2C_HAL_ERR("[IIC DMA_R:R]: Transfer command error!\n");
            //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
            return FALSE;
        }

        //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
        ret = wait_event_timeout(g_miic_dev[u8Port].read_wait,
                g_miic_dev[u8Port].read_condition,
                g_miic_dev[u8Port].timeout);

        if(ret == 0)
        {
            printf("[iic] (%s) wait timeout. (Port:%d;Ret:%d)\n",__FUNCTION__, u8Port, ret);
            return FALSE;
        }
    }
    else
	{
        /* Polling mode */
        //##################
        //  Trigger to READ
        if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_READ, 1)==FALSE)
        {
            HWI2C_HAL_ERR("[IIC DMA_R:R]: Polling Transfer command error!\n");
            //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
            return FALSE;
        }
        //spin_unlock_irqrestore(&g_miic_dev[u8Port].lock, flags);
    }

    //get data to dram
    if(HAL_HWI2C_DMA_GetMiuData(u32PortOffset,uSize,pData)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_R:R]: Get MIU data error!\n");
        return FALSE;
    }


    //Check if ack is ok?
    if( g_miic_dev[u8Port].ack_result > 0 )
        return TRUE;
    else
        return FALSE;
#else
    MS_U8 u8SlaveAddr = LOW_BYTE(u16SlaveCfg)>>1;
    MS_U8 u8Port = HIGH_BYTE(u16SlaveCfg);
    HAL_HWI2C_ReadMode eReadMode;

    HWI2C_HAL_FUNC();

    if (!pRegAddr)
        uAddrCnt = 0;
    if (!pData)
        uSize = 0;
    //no meaning operation
    if (!uSize)
    {
        HWI2C_HAL_ERR("[DMA_R]: No data for reading!\n");
        return FALSE;
    }
    if (u8Port>=HAL_HWI2C_PORTS)
    {
        HWI2C_HAL_ERR("[DMA_R]: Port failure!\n");
        return FALSE;
    }

    eReadMode = g_stPortCfg[u8Port].eReadMode;
    if(eReadMode>=E_HAL_HWI2C_READ_MODE_MAX)
    {
        HWI2C_HAL_ERR("[DMA_R]: Read mode failure!\n");
        return FALSE;
    }

    if(eReadMode!=E_HAL_HWI2C_READ_MODE_DIRECT)
    {
        //set transfer read mode
        HAL_HWI2C_DMA_SetReadMode(u32PortOffset,eReadMode);
        //set slave address
        HAL_HWI2C_DMA_SetSlaveAddr(u32PortOffset,u8SlaveAddr);

        //#################
        //  Set WRITE command
        //#################
        //set command buffer
        if(HAL_HWI2C_DMA_SetTxfrCmd(u32PortOffset,(MS_U8)uAddrCnt,pRegAddr)==FALSE)
        {
            HWI2C_HAL_ERR("[DMA_R:W]: Set command buffer error!\n");
            return FALSE;
        }
        HAL_HWI2C_DMA_SetDataLen(u32PortOffset,0);
    
        //##################
        //  Trigger to WRITE 
        if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_WRITE, 1)==FALSE)
        {
            HWI2C_HAL_ERR("[DMA_R:W]: Transfer command error!\n");
            return FALSE;
        }
    }

   
    //#################
    //  Set READ command
    //#################
    //set transfer with stop
    HAL_HWI2C_DMA_SetTxfrStop(u32PortOffset,TRUE);
    //set slave address
    HAL_HWI2C_DMA_SetSlaveAddr(u32PortOffset,u8SlaveAddr);
    //set command length to 0
    HAL_HWI2C_DMA_SetCmdLen(u32PortOffset,0);
    //set command length for reading
    HAL_HWI2C_DMA_SetDataLen(u32PortOffset,uSize);
    //##################
    //  Trigger to READ 
    if(HAL_HWI2C_DMA_WaitDone(u32PortOffset,HWI2C_DMA_READ, 1)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_R:R]: Transfer command error!\n");
        return FALSE;
    }
    //get data to dram
    if(HAL_HWI2C_DMA_GetMiuData(u32PortOffset,uSize,pData)==FALSE)
    {
        HWI2C_HAL_ERR("[DMA_R:R]: Get MIU data error!\n");
        return FALSE;
    }

    return TRUE;
#endif
}

//#####################
//
//  MIIC Miscellaneous Functions
//
//#####################
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_HWI2C_Init_ExtraProc
/// @brief \b Function  \b Description: Do extral procedure after initialization
/// @param <IN>         \b None :
/// @param param        \b None :
/// @param <RET>        \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_HWI2C_Init_ExtraProc(void)
{
    HWI2C_HAL_FUNC();
    //Extra procedure TODO
}


