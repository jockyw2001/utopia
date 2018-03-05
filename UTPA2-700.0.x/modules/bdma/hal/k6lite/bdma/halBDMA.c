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
#define _HAL_BDMA_C

////////////////////////////////////////////////////////////////////////////////
/// @file mhal_bdma.c
/// @author MStar Semiconductor Inc.
/// @brief DRAM byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

#if defined (MSOS_TYPE_LINUX)
#include <errno.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#if !defined (MSOS_TYPE_LINUX_KERNEL)
#include "tee_ioc.h"
#endif
#include "drvSYS.h"

typedef enum
{
    TEE_PM51CTL_OP_CMD_ENABLE_RESET_FIRE = 0,
    TEE_PM51CTL_OP_CMD_DISABLE_RESET_FIRE,
    TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE,
    TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE,
}TEE_PM51CTL_OP_CMD;



////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#include "MsCommon.h"
#include "MsTypes.h"
#include "halBDMA.h"
#include "regBDMA.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/delay.h>
#endif

////////////////////////////////////////////////////////////////////////////////
// Define & data type
///////////////////////////////////////////////////////////////////////////////
#define HAL_BDMA_REG_CH02CH1(ch0Reg)    (ch0Reg+BDMA_CH_REG_OFFSET)
#define HAL_BDMA_REG_CH12CH0(ch1Reg)    (ch1Reg-BDMA_CH_REG_OFFSET)

#define BDMA_HAL_ERR(x, args...)        //{printf(x, ##args);}
#define BDMA_HAL_NOTSUPPORT()           {printf("%s not support!\n", __FUNCTION__);}

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
static MS_PHY _HAL_BDMA_Get_RegAddr(MS_PHY phy64RegAddr, MS_U8 u8Ch);
static MS_BOOL  _HAL_BDMA_Is_Status_On(MS_U8 u8Status, MS_U8 u8Ch);

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _HAL_BDMA_Get_RegAddr
/// @brief \b Function  \b Description: Get byte DMA register address for channel 0/1
/// @param <IN>         \b u32RegAddr: Source address
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b register address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_PHY _HAL_BDMA_Get_RegAddr(MS_PHY phy64RegAddr, MS_U8 u8Ch)
{
    if (E_BDMA_CH1 == u8Ch)
        return HAL_BDMA_REG_CH02CH1(phy64RegAddr);
    return phy64RegAddr;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: _HAL_BDMA_Is_Status_On
/// @brief \b Function  \b Description: Get byte DMA status for channel 0/1
/// @param <IN>         \b u8Status : queried status
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL _HAL_BDMA_Is_Status_On(MS_U8 u8Status, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_STATUS, u8Ch);
    BDMA_HAL_ERR("%s reg:%lx val:%u status:%u\n",  __FUNCTION__, u32Reg, HAL_BDMA_ReadByte(phy64Reg), u8Status);
    return (HAL_BDMA_ReadByte(phy64Reg) & u8Status) ? TRUE : FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_BDMA_ReadByte(MS_PHY phy64RegAddr)
{
    return ((volatile MS_U8*)(_gMIO_MapBase))[(phy64RegAddr << 1) - (phy64RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_BDMA_Read2Byte(MS_PHY phy64RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_MapBase))[phy64RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_Read4Byte(MS_PHY phy64RegAddr)
{
    return (HAL_BDMA_Read2Byte(phy64RegAddr) | HAL_BDMA_Read2Byte(phy64RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_WriteByte(MS_PHY phy64RegAddr, MS_U8 u8Val)
{
    if (!phy64RegAddr)
    {
        BDMA_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(_gMIO_MapBase))[(phy64RegAddr << 1) - (phy64RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Write2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Write2Byte(MS_PHY phy64RegAddr, MS_U16 u16Val)
{
    if (!phy64RegAddr)
    {
        BDMA_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(_gMIO_MapBase))[phy64RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b phy64RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Write4Byte(MS_PHY phy64RegAddr, MS_U32 u32Val)
{
    if (!phy64RegAddr)
    {
        BDMA_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_BDMA_Write2Byte(phy64RegAddr, u32Val & 0x0000FFFF);
    HAL_BDMA_Write2Byte(phy64RegAddr+2, u32Val >> 16);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_WriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b phy64RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_WriteRegBit(MS_PHY phy64RegAddr, MS_U8 u8Mask, MS_BOOL bEnable)
{
    MS_U8 u8Val = HAL_BDMA_ReadByte(phy64RegAddr);
    if (!phy64RegAddr)
    {
        BDMA_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    u8Val = HAL_BDMA_ReadByte(phy64RegAddr);
    u8Val = (bEnable) ? (u8Val | u8Mask) : (u8Val & ~u8Mask);
    HAL_BDMA_WriteByte(phy64RegAddr, u8Val);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetSrcAddr
/// @brief \b Function  \b Description: Set source address
/// @param <IN>         \b phy64RegAddr: Source address
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetSrcAddr(BDMA_Act eAct, MS_PHY phy64RegAddr, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_SRC_ADDR_L, u8Ch);
    return HAL_BDMA_Write4Byte(phy64Reg, phy64RegAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetSrcAddr
/// @brief \b Function  \b Description: Get destination address
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <RET>        \b destinaiton address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetSrcAddr(BDMA_Act eAct, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_SRC_ADDR_L, u8Ch);
    return HAL_BDMA_Read4Byte(phy64Reg);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetDstAddr
/// @brief \b Function  \b Description: Set destination address
/// @param <IN>         \b u32RegAddr: destination address
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetDstAddr(BDMA_Act eAct, MS_PHY phy64RegAddr, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_DST_ADDR_L, u8Ch);
    return HAL_BDMA_Write4Byte(phy64Reg, phy64RegAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetDstAddr
/// @brief \b Function  \b Description: Get destination address
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <RET>        \b destinaiton address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetDstAddr(BDMA_Act eAct, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_DST_ADDR_L, u8Ch);
    return HAL_BDMA_Read4Byte(phy64Reg);
}


static void __pm51ctl(TEE_PM51CTL_OP_CMD cmd)
{
    SYS_TEEINFO teemode;

#if defined(MSOS_TYPE_LINUX) || defined(CONFIG_LINUX)
    MDrv_SYS_ReadKernelCmdLine();
    MDrv_SYS_GetTEEInfo(&teemode);

    switch (teemode.OsType)
    {
        case SYS_TEEINFO_OSTYPE_OPTEE:
        {
            int fd;
            if ((fd = open("/dev/opteearmtz00", O_RDWR)) < 0)
            {
                printf("PM51CTL open optee node failed: %d\n", fd);
                return;
            }
            else
            {
                if (ioctl(fd, TEE_PM51CTL_IOC, &cmd)!=0)
                {
                   printf("PM51CTL reset fire failed\n");
                }
                close(fd);
            }
        }
        /* for Secure RIU, Direct access RIU again in order to prevent OPTEE function is not
           ready yet. */
        default:
        {
            // original flow
            switch(cmd)
            {
                case TEE_PM51CTL_OP_CMD_ENABLE_RESET_FIRE:
                    HAL_BDMA_WriteByte(0x0e53UL, HAL_BDMA_ReadByte(0x0e53UL) |__BIT0);
                    break;
                case TEE_PM51CTL_OP_CMD_DISABLE_RESET_FIRE:
                    HAL_BDMA_WriteByte(0x0e53, HAL_BDMA_ReadByte(0x0e53UL) & (~__BIT0));
                    break;
                case TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE:
                    HAL_BDMA_WriteByte(0x0e53UL, HAL_BDMA_ReadByte(0x0e53UL) | __BIT4);   //release 8051  reset
                    break;
                case TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE:
                    HAL_BDMA_WriteByte(0x0e53,HAL_BDMA_ReadByte(0x0e53) & (~__BIT4));   //8051 reset
                    break;
                default:
                    break;
            }
        }
    }
#elif defined(CONFIG_MBOOT)
    MDrv_SYS_GetTEEInfo(&teemode);

    switch (teemode.OsType)
    {
        case SYS_TEEINFO_OSTYPE_OPTEE:
        {
#define TEESMC32_OPTEE_FASTCALL_PM51CTL "0xb200685C"
            // fastcall
            __asm__ __volatile__(
                ".arch_extension sec\n\t"
                "ldr r0, =" TEESMC32_OPTEE_FASTCALL_PM51CTL " \n\t"
                "mov r1,%0 \n\t"
                "smc #0 \n\t"
                :
                : "r"(cmd)
                : "r0","r1"
            );
        }

        default:
        {
            // original flow
            switch(cmd)
            {
                case TEE_PM51CTL_OP_CMD_ENABLE_RESET_FIRE:
                    HAL_BDMA_WriteByte(0x0e53UL,HAL_BDMA_ReadByte(0x0e53UL)|__BIT0);
                    break;
                case TEE_PM51CTL_OP_CMD_DISABLE_RESET_FIRE:
                    HAL_BDMA_WriteByte(0x0e53,HAL_BDMA_ReadByte(0x0e53UL)&(~__BIT0));
                    break;
                case TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE:
                    HAL_BDMA_WriteByte(0x0e53UL,HAL_BDMA_ReadByte(0x0e53UL)|__BIT4);   //release 8051  reset
                    break;
                case TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE:
                    HAL_BDMA_WriteByte(0x0e53,HAL_BDMA_ReadByte(0x0e53)&(~__BIT4));   //8051 reset
                    break;
                default:
                    break;
            }
        }
    }
#else
    switch(cmd)
    {
        case TEE_PM51CTL_OP_CMD_ENABLE_RESET_FIRE:
            HAL_BDMA_WriteByte(0x0e53UL,HAL_BDMA_ReadByte(0x0e53UL)|__BIT0);
            break;
        case TEE_PM51CTL_OP_CMD_DISABLE_RESET_FIRE:
            HAL_BDMA_WriteByte(0x0e53,HAL_BDMA_ReadByte(0x0e53UL)&(~__BIT0));
            break;
        case TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE:
            HAL_BDMA_WriteByte(0x0e53UL,HAL_BDMA_ReadByte(0x0e53UL)|__BIT4);   //release 8051  reset
            break;
        case TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE:
            HAL_BDMA_WriteByte(0x0e53,HAL_BDMA_ReadByte(0x0e53)&(~__BIT4));   //8051 reset
            break;
        default:
            break;
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetDevDw
/// @brief \b Function  \b Description: Get device data width
/// @param <IN>         \b eDev: Device
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_BDMA_GetDevDw(BDMA_Dev eDev)
{
    switch (eDev)
    {
    case E_BDMA_DEV_MIU0:
    case E_BDMA_DEV_MIU1:
        return E_BDMA_DW_16BYTE;

    case E_BDMA_DEV_FLASH:
        return E_BDMA_DW_8BYTE;

    case E_BDMA_DEV_MEM_FILL:
        return E_BDMA_DW_4BYTE;
    default:
        return E_BDMA_DW_1BYTE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetDevId
/// @brief \b Function  \b Description: Get device id
/// @param <IN>         \b eDev: Device
/// @param <RET>        \b The device ID
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_BDMA_GetDevId(BDMA_Dev eDev)
{
    switch (eDev)
    {
        case E_BDMA_DEV_MIU0:
            return E_BDMA_HALDEV_MIU0;
        case E_BDMA_DEV_MIU1:
            return E_BDMA_HALDEV_MIU1;
        case E_BDMA_DEV_SEARCH:
            return E_BDMA_HALDEV_SEARCH;
        case E_BDMA_DEV_CRC32:
            return E_BDMA_HALDEV_CRC32;
        case E_BDMA_DEV_MEM_FILL:
            return E_BDMA_HALDEV_MEM_FILL;
        case E_BDMA_DEV_FLASH:
            return E_BDMA_HALDEV_FLASH;
        case E_BDMA_DEV_1KSRAM_HK51:
            return E_BDMA_HALDEV_1KSRAM_HK51;
        case E_BDMA_DEV_DMDMCU:
            return E_BDMA_HALDEV_DMDMCU;
        case E_BDMA_DEV_DSP:
            return E_BDMA_HALDEV_DSP;
        case E_BDMA_DEV_TSP:
            return E_BDMA_HALDEV_TSP;
        default:
            return E_BDMA_HALDEV_NOT_SUPPORT;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetSrcDev
/// @brief \b Function  \b Description: Set source device
/// @param <IN>         \b u8DevCfg: Device config
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetSrcDev(MS_U8 u8DevCfg, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_SRC_SEL, u8Ch);
    return HAL_BDMA_WriteByte(phy64Reg, u8DevCfg);
}

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetSrcDev
/// @brief \b Function  \b Description: Set source device
/// @param <IN>         \b u8DevCfg: Device config
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetDstDev(MS_U8 u8DevCfg, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_DST_SEL, u8Ch);

    return HAL_BDMA_WriteByte(phy64Reg, u8DevCfg);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetCmd0
/// @brief \b Function  \b Description: Set command 0
/// @param <IN>         \b u32Cmd: Command 0 value
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetCmd0(MS_U32 u32Cmd, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CMD0_L, u8Ch);
    return HAL_BDMA_Write4Byte(phy64Reg, u32Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetCmd0
/// @brief \b Function  \b Description: Get command 0
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b *pu32Cmd : Command 0 value
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetCmd0(MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CMD0_L, u8Ch);
    return HAL_BDMA_Read4Byte(phy64Reg);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetCmd1
/// @brief \b Function  \b Description: Set command 1
/// @param <IN>         \b u32Cmd: Command 1 value
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetCmd1(MS_U32 u32Cmd, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CMD1_L, u8Ch);
    return HAL_BDMA_Write4Byte(phy64Reg, u32Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetCmd1
/// @brief \b Function  \b Description: Get command 1
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b *pu32Cmd : Command 1 value
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetCmd1(MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CMD1_L, u8Ch);
    return HAL_BDMA_Read4Byte(phy64Reg);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetCmd2
/// @brief \b Function  \b Description: Set command 2
/// @param <IN>         \b u32Cmd: Command 2 value
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetCmd2(MS_U32 u32Cmd, MS_U8 u8Ch)
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetCmd2
/// @brief \b Function  \b Description: Get command 2
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b *pu32Cmd : Command 2 value
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetCmd2(MS_U8 u8Ch)
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetCRC32
/// @brief \b Function  \b Description: Get crc32 result
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None:
/// @param <RET>        \b Matched address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetCRC32(MS_U8 u8Ch)
{
    return HAL_BDMA_GetCmd1(u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetMatched
/// @brief \b Function  \b Description: Get matched address
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None:
/// @param <RET>        \b Matched address
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetMatched(MS_PHY phy64SrcAddr, MS_U8 u8Ch)
{
    BDMA_HAL_ERR("Matched address:%x\n", HAL_BDMA_GetSrcAddr(E_BDMA_ACT_SEARCH, u8Ch));
    return ((MS_U32) phy64SrcAddr + HAL_BDMA_GetSrcAddr(E_BDMA_ACT_SEARCH, u8Ch) - 1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetLen
/// @brief \b Function  \b Description: Set data size
/// @param <IN>         \b u32Len: Data size
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetLen(BDMA_Act eAct, MS_U32 u32Len, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_SIZE_L, u8Ch);
    return HAL_BDMA_Write4Byte(phy64Reg, u32Len);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_GetLen
/// @brief \b Function  \b Description: Get command 0
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b *pu32Len : data length
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_BDMA_GetLen(BDMA_Act eAct, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_SIZE_L, u8Ch);
    return HAL_BDMA_Read4Byte(phy64Reg);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Set_Addr_Dec
/// @brief \b Function  \b Description: Set address increasing/decreasing direction
/// @param <IN>         \b bDec: TRUE: Decreasing FALSE: Increasing
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Set_Addr_Dec(MS_BOOL bDec, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_MISC, u8Ch);

    BDMA_HAL_ERR("%s reg:%lx\n", __FUNCTION__,phy64Reg);
    return HAL_BDMA_WriteRegBit(phy64Reg, BDMA_CH_ADDR_DECDIR, bDec);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Set_CRC_Reflect
/// @brief \b Function  \b Description: Set CRC value reflection
/// @param <IN>         \b bReflect: TRUE: Reflection FALSE: Not Reflection
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Set_CRC_Reflect(MS_BOOL bReflect, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_MISC, u8Ch);

    BDMA_HAL_ERR("%s reg:%lx\n", __FUNCTION__,phy64Reg);
    return HAL_BDMA_WriteRegBit(phy64Reg, BDMA_CH_CRC_REFLECTION, bReflect);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetDmyWrCnt
/// @brief \b Function  \b Description: Set Dummy write count
/// @param <IN>         \b u8Cnt: Dummy count
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_SetDmyWrCnt(MS_U8 u8Cnt, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_DWUM_CNT, u8Ch);
    return HAL_BDMA_WriteByte(phy64Reg, u8Cnt);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_TrigOn
/// @brief \b Function  \b Description: Trigger on BDMA action for channel 0/1
/// @param <IN>         \b eAct: BDMA action
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <IN>         \b u8Para: trigger parameter
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_TrigOn(MS_U8 u8Src, MS_U8 u8Dst, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CTRL, u8Ch);
    return HAL_BDMA_WriteRegBit(phy64Reg, BDMA_CH_TRIGGER, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Stop
/// @brief \b Function  \b Description: Stop BDMA operations
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Stop(MS_U8 u8Ch)
{
    MS_BOOL bRet = TRUE;
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CTRL, u8Ch);

    //Set stop
    bRet &= HAL_BDMA_WriteRegBit(phy64Reg, BDMA_CH_STOP, ENABLE);
    //Clear stop
    bRet &= HAL_BDMA_WriteRegBit(phy64Reg, BDMA_CH_STOP, DISABLE);
    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Enable_Done_INT
/// @brief \b Function  \b Description: Enable interrupt when action done
/// @param <IN>         \b bEnable: TRUE: Enable FALSE: Disable
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Enable_INT(MS_BOOL bEnable, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_MISC, u8Ch);
    return HAL_BDMA_WriteRegBit(phy64Reg, BDMA_CH_DONE_INT_EN, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Is_Queued
/// @brief \b Function  \b Description: Check if any action is queued
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: queued FALSE: empty
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Is_Queued(MS_U8 u8Ch)
{
    return _HAL_BDMA_Is_Status_On(BDMA_CH_QUEUED, u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_DMA_Is_FlashBusy
/// @brief \b Function  \b Description: check if DMA is ready for flash copy
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Busy FALSE: Not busy
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_DMA_Is_FlashReady()
{
    BDMA_HAL_NOTSUPPORT();
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: MDrv_BDMA_Is_Busy
/// @brief \b Function  \b Description: Check if Byte DMA is busy
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Busy FALSE: Not busy
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Is_Busy(MS_U8 u8Ch)
{
    return _HAL_BDMA_Is_Status_On(BDMA_CH_BUSY, u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Is_Int
/// @brief \b Function  \b Description: Check if interrupted when Byte DMA is done
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Interrupted FALSE: No interrupt
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Is_Int(MS_U8 u8Ch)
{
    return _HAL_BDMA_Is_Status_On(BDMA_CH_INT, u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Is_Done
/// @brief \b Function  \b Description: Check if Byte DMA action is done
/// @param <IN>         \b eAct: BDMA action
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Done FALSE: Not Done
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Is_Done(BDMA_Act eAct, MS_U8 u8Ch)
{
    if (E_BDMA_ACT_SEARCH == eAct && HAL_BDMA_Is_Found(u8Ch))
        return TRUE;
    return _HAL_BDMA_Is_Status_On(BDMA_CH_DONE, u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Is_Found
/// @brief \b Function  \b Description: Check if Byte DMA find matched pattern
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: found FALSE: Not found
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Is_Found(MS_U8 u8Ch)
{
    return _HAL_BDMA_Is_Status_On(BDMA_CH_RESULT, u8Ch);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Clear_Status
/// @brief \b Function  \b Description: Clear BDMA action status
/// @param <IN>         \b eAct: BDMA action
/// @param <IN>         \b u8Ch: Channel 0/1
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Success FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_BDMA_Clear_Status(BDMA_Act eAct, MS_U8 u8Ch)
{
    MS_PHY phy64Reg = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_STATUS, u8Ch);
    return HAL_BDMA_WriteByte(phy64Reg, BDMA_CH_CLEAR_STATUS);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Dump_AllReg
/// @brief \b Function  \b Description: Dump bdma all register
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_BDMA_Dump_AllReg(void)
{
    MS_PHY phy64Reg = BDMA_REG_BASE;
    MS_PHY phy64EndAddr = _HAL_BDMA_Get_RegAddr(BDMA_REG_CH0_CMD1_H, E_BDMA_CH1);

    for (; phy64Reg < phy64EndAddr; phy64Reg+=2)
    {
        if (0 == (phy64Reg%0x10))
#if defined (__aarch64__)
            printf("\n%lx:   ", phy64Reg);
#else
            printf("\n%llx:   ", phy64Reg);
#endif
        printf("%4x ", HAL_BDMA_Read2Byte(phy64Reg));
        if (0x0E == (phy64Reg % 0x10))
            printf("\n");
    }
    printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_SetIOMapBase
/// @brief \b Function  \b Description: Set IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_BDMA_SetIOMapBase(MS_VIRT u32Base)
{
    _gMIO_MapBase = u32Base;
    BDMA_HAL_ERR("BDMA IOMap base:%8x Reg offset:%4x\n", u32Base, BDMA_REG_BASE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Get_IOMap_Base
/// @brief \b Function  \b Description: Get IO Map base
/// @param <IN>         \b None :
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_BDMA_GetIOMapBase()
{
    return _gMIO_MapBase;
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
#define REG_PM_CPUX_SW_RSTZ        (PMSLEEP_REG_BASE + 0x29*2)

void HAL_BDMA_SetSPIOffsetForMCU()
{
     MS_U32 rty_count = 0x5;
     MS_U32 start_addr = 0x00000;
     MS_U32 end_addr = 0xff0000;

     /*Notice: Patch for Randomly DC off fail due to MCU run on SPI and CPU access SPI at same time */
     HAL_BDMA_WriteByte(0x1700,HAL_BDMA_ReadByte(0x1700)|(0x6F));
    __pm51ctl(TEE_PM51CTL_OP_CMD_DISABLE_RESET_FIRE);
    __pm51ctl(TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE);
     /*Avoid SPI Clock gating and SPI Clock IN 27Mhz*/
     HAL_BDMA_WriteByte(0x0e41,HAL_BDMA_ReadByte(0x0e41)&(~__BIT0));
     HAL_BDMA_WriteByte(0x0e41,HAL_BDMA_ReadByte(0x0e41)|(__BIT2));
     HAL_BDMA_WriteByte(0x0e41,HAL_BDMA_ReadByte(0x0e41)&(~__BIT4));
     HAL_BDMA_WriteByte(0x0e41,HAL_BDMA_ReadByte(0x0e41)|(__BIT6));
     /*MCU in 108Mhz*/
     HAL_BDMA_WriteByte(0x0e40,HAL_BDMA_ReadByte(0x0e40)|(__BIT3));
     HAL_BDMA_WriteByte(0x0e40,HAL_BDMA_ReadByte(0x0e40)|(__BIT4));
     HAL_BDMA_WriteByte(0x0e40,HAL_BDMA_ReadByte(0x0e40)|(__BIT7));
     HAL_BDMA_WriteByte(0x1018,HAL_BDMA_ReadByte(0x1018)&(~__BIT3));
     HAL_BDMA_WriteByte(0x2ba0,HAL_BDMA_ReadByte(0x2ba0)|__BIT0);

/*
     HAL_BDMA_WriteRegBit(0x000e41, DISABLE, __BIT6);  //spi_clk=xtal

     HAL_BDMA_WriteRegBit(0x000e40, DISABLE, __BIT7);  //mcu51 clk=xtal

     HAL_BDMA_WriteRegBit(0x001018, DISABLE, __BIT3);  // i_cache rstz

     HAL_BDMA_WriteRegBit(0x002ba0, ENABLE,  __BIT0);  //disable i cache
 */
     HAL_BDMA_Write2Byte(MCU_REG_BASE+0x14,(MS_U16)start_addr & 0x0000FFFF);
     HAL_BDMA_Write2Byte(MCU_REG_BASE+0x10,(MS_U16)(start_addr>>16) & 0x000000FF);
     HAL_BDMA_Write2Byte(MCU_REG_BASE+0x16,(MS_U16)end_addr & 0x0000FFFF);
     HAL_BDMA_Write2Byte(MCU_REG_BASE+0x12,(MS_U16)(end_addr>>16) & 0x000000FF);


    //set spi offset set reg_spi_offset_addr[7:0]     0x0010f9 = 0x01     => 0x10000
    //set reg_spi_offset_en               0x0010f8 = 0x01
    HAL_BDMA_Write2Byte(MCU_REG_BASE+0xf8,0x0101);

    //set reset password 0x0e54=0x829f
    HAL_BDMA_Write2Byte(PMSLEEP_REG_BASE+0x54,0x829f);


    //reg_fire[3:0]                   0x000e53 = 0x01

    //reg_fire[3:0]                   0x000e53 = 0x00
    __pm51ctl(TEE_PM51CTL_OP_CMD_ENABLE_RESET_FIRE);
    __pm51ctl(TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE);
   MsOS_DelayTask(100);

    printf("Wait for PM51 standby 1st...........\n");
    while (rty_count--)
    {
        if (HAL_BDMA_Read2Byte(0x0e24) == 0x02)
        {
            printf("PM51 1st run ok...........\n");
            break;
        }
       __pm51ctl(TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE);
       __pm51ctl(TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE);
/*
#ifdef MSOS_TYPE_LINUX_KERNEL
    mdelay(100);
#else
    MsOS_DelayTask(100);
#endif
*/
    }

    if(rty_count==0)
       printf("PM51 1st run fail...........\n");

  /*
    __pm51ctl(TEE_PM51CTL_OP_CMD_DISABLE_RESET_FIRE);
    __pm51ctl(TEE_PM51CTL_OP_CMD_DISABLE_RESET_TOGGLE);
    HAL_BDMA_Write2Byte(MCU_REG_BASE+0xf8,0x0301);

    //set reset password 0x0e54=0x829f
    HAL_BDMA_Write2Byte(PMSLEEP_REG_BASE+0x54,0x829f);


    //reg_fire[3:0]                   0x000e53 = 0x01

    //reg_fire[3:0]                   0x000e53 = 0x00
    __pm51ctl(TEE_PM51CTL_OP_CMD_ENABLE_RESET_FIRE);
    __pm51ctl(TEE_PM51CTL_OP_CMD_ENABLE_RESET_TOGGLE);
   MsOS_DelayTask(100);

    printf("Wait for PM51 2nd standby...........\n");
    while(HAL_BDMA_Read2Byte(0x0e24)!=0x02);

    printf("PM51 2nd run ok...........\n");
*/
}


