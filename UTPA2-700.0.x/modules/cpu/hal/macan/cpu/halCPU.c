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

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "regCHIP.h"
#include "regCPU.h"
#include "drvCPU.h"
#include "drvBDMA.h"
#include "drvMMIO.h"
#include "halCPU.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define COPRO_HAL_INFO(x, args...)        //{printf(x, ##args);}
#define COPRO_HAL_ERR(x, args...)        //{printf(x, ##args);}
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT u32NonPMBaseAdd;
static MS_VIRT u32CoProBaseAdd;
static MS_VIRT u32PMRiuBaseAdd;
#if 1//frcr2_integration###
static MS_VIRT u32FrcR2BaseAdd;
#endif
static MS_U8 u8CoProEnable_flag=0;
//static MS_BOOL BEONRun; //Mask for Coverity
//static MS_BOOL AEONRun; //Mask for Coverity

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

void HAL_COPRO_RegSetBase(MS_VIRT u32Base)
{
    MS_VIRT u32RiuBaseAddr;
    MS_PHY u32RiuBaseSize;

    //(1) Set Non-PM base address got grom MS_MODULE_HW
    u32NonPMBaseAdd = u32Base;
    //printf("[COPRO] HAL_COPRO_RegSetBase: u32NonPMBaseAdd = 0x%08lX\n",u32NonPMBaseAdd);

    //(2) Get & Set Secure base address
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAddr, &u32RiuBaseSize, MS_MODULE_HW))
    {
        COPRO_HAL_ERR("HAL_COPRO_RegSetBase: IOMap failure for Secure R2\n");
    }
    else
    {
        u32CoProBaseAdd = u32RiuBaseAddr;
        COPRO_HAL_INFO("HAL_COPRO_RegSetBase: [R2] base address = %lx\n", u32RiuBaseAddr);
    }

    //(3) Get & Set PM base address
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAddr, &u32RiuBaseSize, MS_MODULE_PM))
    {
        COPRO_HAL_ERR("HAL_COPRO_RegSetBase: IOMap failure\n");
    }
    else
    {
        u32PMRiuBaseAdd = u32RiuBaseAddr;
        COPRO_HAL_INFO("HAL_COPRO_RegSetBase: [PM] base address = %lx\n", u32RiuBaseAddr);
    }
    #if 1//frcr2_integration###
    //(4) Get & Set FRC-R2 base address //frcr2_integration###
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAddr, &u32RiuBaseSize, MS_MODULE_FRC))
    {
        COPRO_HAL_ERR("HAL_COPRO_RegSetBase: IOMap failure\n");
    }
    else
    {
        u32FrcR2BaseAdd = u32RiuBaseAddr;
        COPRO_HAL_INFO("HAL_COPRO_RegSetBase: [FRC-R2] base address = %lx\n", u32RiuBaseAddr);
    }
    #endif
}

////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////
//###############
//
// For Non-PM Banks
//
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_NPM_RegReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_CPU_NPM_RegReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(u32NonPMBaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_NPM_RegWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_CPU_NPM_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(u32NonPMBaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_NPM_RegRead2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_CPU_NPM_RegRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(u32NonPMBaseAdd))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_NPM_RegWrite2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_CPU_NPM_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(u32NonPMBaseAdd))[u32RegAddr] = u16Val;
    return TRUE;
}

//###############
//
// For PM Banks
//
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_PM_RegReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_CPU_PM_RegReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(u32PMRiuBaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_PM_RegWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_CPU_PM_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(u32PMRiuBaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_PM_RegRead2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_CPU_PM_RegRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(u32PMRiuBaseAdd))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_CPU_PM_RegWrite2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_CPU_PM_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(u32PMRiuBaseAdd))[u32RegAddr] = u16Val;
    return TRUE;
}


//###############
//
// For R2 Banks
//
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_COPRO_RegReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(u32CoProBaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Read4Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_COPRO_RegRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(u32CoProBaseAdd))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Read4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_COPRO_RegRead4Byte(MS_U32 u32RegAddr)
{
    return (HAL_COPRO_RegRead2Byte(u32RegAddr) | HAL_COPRO_RegRead2Byte(u32RegAddr+2) << 16);
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
MS_BOOL HAL_COPRO_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(u32CoProBaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
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
MS_BOOL HAL_COPRO_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(u32CoProBaseAdd))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_BDMA_Write4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_COPRO_RegWrite4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_COPRO_RegWrite2Byte(u32RegAddr, u32Val & 0x0000FFFFUL);
    HAL_COPRO_RegWrite2Byte(u32RegAddr+2, u32Val >> 16);

    return TRUE;
}

#if 1//frcr2_integration###
//###############
//
// For FRC-R2 Banks
//
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_RegReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_FRCR2_RegReadByte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U8*)(u32FrcR2BaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_RegRead2Byte
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_FRCR2_RegRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(u32FrcR2BaseAdd))[u32RegAddr];
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_RegRead4Byte
/// @brief \b Function  \b Description: read 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U32
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_FRCR2_RegRead4Byte(MS_U32 u32RegAddr)
{
    return (HAL_FRCR2_RegRead2Byte(u32RegAddr) | HAL_FRCR2_RegRead2Byte(u32RegAddr+2) << 16);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_RegWriteByte
/// @brief \b Function  \b Description: write 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u8Val : 1 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_FRCR2_RegWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U8*)(u32FrcR2BaseAdd))[(u32RegAddr << 1) - (u32RegAddr & 1)] = u8Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_RegWrite2Byte
/// @brief \b Function  \b Description: write 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 2 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_FRCR2_RegWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    ((volatile MS_U16*)(u32FrcR2BaseAdd))[u32RegAddr] = u16Val;
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_RegWrite4Byte
/// @brief \b Function  \b Description: write 4 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u32Val : 4 byte data
/// @param <OUT>        \b None :
/// @param <RET>        \b TRUE: Ok FALSE: Fail
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_FRCR2_RegWrite4Byte(MS_U32 u32RegAddr, MS_U32 u32Val)
{
    if (!u32RegAddr)
    {
        COPRO_HAL_ERR("%s reg error!\n", __FUNCTION__);
        return FALSE;
    }

    HAL_FRCR2_RegWrite2Byte(u32RegAddr, u32Val & 0x0000FFFFUL);
    HAL_FRCR2_RegWrite2Byte(u32RegAddr+2, u32Val >> 16);

    return TRUE;
}
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_COPRO_Disable
/// @brief \b Function  \b Description: coprocessor disable
/// @param <IN>         \b none:
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_COPRO_Disable(void)
{
    //if(u8CoProEnable_flag != 0)
    {
#if 1//defined(MCU_ARM_CA7) || defined(__aarch64__)// HK = ARM 
        COPRO_HAL_INFO("Disable R2\r\n");
        HAL_COPRO_RegWriteByte(R2_REG_STOP, 0x00UL); // reg_r2_enable = 0x00
        MsOS_DelayTask(50);
        COPRO_HAL_INFO("Disable R2 Done\r\n");
#else
        COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
#endif
        u8CoProEnable_flag=0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_COPRO_Enable()
/// @brief \b Function \b Description: Enable Aeon
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_COPRO_Enable(MS_U32 u32_ADR)
{
#if 1//defined(MCU_ARM_CA7) || defined(__aarch64__)// HK = ARM 
    MS_U32 base_addr;

    //(1) Disable R2 -> Stop(Reset)
    HAL_COPRO_Disable();

    //(2) Set R2 clock
    // [1:0] = 2'b00
    // [3:2] =  2'b00: 240MHz,  2'b01: 192HMz, 2'b10: 172MHz, 2'b11: Xtal Clock
    // Set to 216 MHz
    HAL_CPU_NPM_RegWriteByte(NPM_REG_CLKGEN1_SECR2, 0x00UL);

    #if 0
    //(3) Set CPU SDR base
    base_addr = u32_ADR;
    HAL_COPRO_RegWrite2Byte(R2_REG_SDR_LO_INST_BASE, (base_addr&0x0000FFFFUL)); //I Fetch Offset - Low
    HAL_COPRO_RegWrite2Byte(R2_REG_SDR_HI_INST_BASE, (base_addr>>16)); //I Fetch Offset - High
    HAL_COPRO_RegWrite2Byte(R2_REG_SDR_LO_DATA_BASE, (base_addr&0x0000FFFFUL)); //D Fetch Offset - Low
    HAL_COPRO_RegWrite2Byte(R2_REG_SDR_HI_DATA_BASE, (base_addr>>16)); //D Fetch Offset - High
    #else
    //(3) Set CPU reset base(vector)
    base_addr = u32_ADR >> 16;
    HAL_COPRO_RegWrite2Byte(R2_REG_RST_BASE, base_addr); // reset vector address 0x0(64K alignment)
    #endif
    //(4)Set MAU Mapping
    HAL_COPRO_RegWrite2Byte(R2_MAULV1_REG, 0x8800UL);
    HAL_COPRO_RegWrite2Byte(R2_MAULV2_REG, 0x8400UL);
    //(4) Set RIU   base
    HAL_COPRO_RegWrite2Byte(R2_REG_RIU_BASE, 0xFA00UL); // RIU  base address
    //(5) Set UART base
    HAL_COPRO_RegWrite2Byte(R2_REG_IO1_BASE, 0xF800UL); // UART base address

    HAL_COPRO_RegWrite2Byte(R2_REG_SPI_BASE, 0xF900UL);

    HAL_COPRO_RegWrite2Byte(R2_REG_SPI_BASE1, 0xF900UL); 

    HAL_COPRO_RegWrite2Byte(R2_REG_DQMEM_BASE, 0xFB00UL); 

    HAL_COPRO_RegWrite2Byte(R2_REG_QMEM_MASK_HIGH, 0xFF00UL);
    //(7) Set IO space enable (UART, RIU) with QMEM space disabled
    HAL_COPRO_RegWrite2Byte(R2_REG_SPACE_EN, 0x0003UL); // io_space_en[3:0]: UART,RIU, qmem_space_en[4] : disable
    //(8) CPU SW reset R2
    // reg_cpux_sw_rstz =  aeon sw rstz(low)
    //*(volatile MS_U32*)(0xBF000000+0x5CA4UL) &= ~BIT13;
    HAL_CPU_PM_RegWrite2Byte(PM_REG_CPUX_SW_RSTZ, HAL_CPU_PM_RegRead2Byte(PM_REG_CPUX_SW_RSTZ) &(~BIT13));
    HAL_CPU_PM_RegWrite2Byte(PM_REG_CPUX_SW_RSTZ, HAL_CPU_PM_RegRead2Byte(PM_REG_CPUX_SW_RSTZ) | (BIT13));

    //(7) Without QMEM 
    #if 0//R2_MMAP_MIU1
    HAL_COPRO_RegWrite2Byte(R2_REG_QMEM_BASE_HIGH, 0x8000UL); // QMEM base address = 0x80000000
    HAL_COPRO_RegWrite2Byte(R2_REG_QMEM_MASK_HIGH, 0xF000UL); // QMEM less, set DCU Decoding..
    #endif

    //(1) Enable R2 -> Start
    HAL_COPRO_RegWriteByte(R2_REG_STOP, 0x24UL); // miu_sw_rst and sdram_boot = 1
    HAL_COPRO_RegWriteByte(R2_REG_STOP, 0x27UL); // sdram_boot and (miu/r2/r2_rst) =1

#else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
#endif
    u8CoProEnable_flag=1;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_COPRO_Init_Front()
/// @brief \b Function \b Description:
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_COPRO_Init_Front(void)
{
#if 1//defined(MCU_ARM_CA7) || defined(__aarch64__)// HK = ARM 
    COPRO_HAL_INFO("ARM is HK!\n");
    HAL_COPRO_Disable();
#else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
#endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_COPRO_Init_End()
/// @brief \b Function \b Description:
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_COPRO_Init_End(MS_U32 u32_ADR)
{
#if 1//defined(MCU_ARM_CA7) || defined(__aarch64__)// HK = ARM 
    COPRO_HAL_INFO("ARM is HK!\n");

    /// enable R2
    HAL_COPRO_Enable(u32_ADR);

    // Enable Interrupt
    MsOS_EnableInterrupt(E_INT_FIQ_AEON_TO_MIPS_VPE0); //should be secure-R2

#elif defined(MCU_AEON) // HK = AEON
    COPRO_HAL_INFO("AEON is HK:END!\n");

    HAL_COPRO_Enable(u32_ADR);

    MsOS_EnableInterrupt(E_INT_FIQ_MIPS_VPE0_TO_AEON); //should be secure-R2

#else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
#endif

}
#if 1//frcr2_integration###
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_FRCR2_Disable
/// @brief \b Function  \b Description: coprocessor disable
/// @param <IN>         \b none:
/// @param <OUT>        \b None :
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_FRCR2_Disable(void)
{
  #if defined(MCU_ARM_CA7) || defined(__aarch64__)
    COPRO_HAL_INFO("Disable FRC-R2\r\n");
    HAL_FRCR2_RegWriteByte(FRC_R2_REG_STOP, 0x00UL);
    MsOS_DelayTask(50);
    COPRO_HAL_INFO("Disable FRC-R2 Done\r\n");
  #else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
  #endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_FRCR2_Enable()
/// @brief \b Function \b Description: Enable Aeon
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_FRCR2_Enable(MS_U32 u32_ADR)
{
  #if defined(MCU_ARM_CA7) || defined(__aarch64__)
    MS_U32 base_addr;

    //(1) Disable R2 -> Stop(Reset)
    HAL_FRCR2_Disable();

    //(2) Set FRC clocks
    //(2-1) R2 MCU(RIU) clock -216 MHz(2'b100000)
    // [1:0] : 2'b00
    // [4:2] : 000: 216MHz, 001: 192MHz, 010: 172MHz, 011: 160MHz, 100: 144MHz, 101: 108MHz
    HAL_CPU_NPM_RegWriteByte(NPM_REG_CLKGEN1_FRCMCU, 0x00UL);
    HAL_CPU_NPM_RegWriteByte(NPM_REG_CLKGEN1_FRCMCU, 0x20UL); //switch ckg_mcu_frc by glitch-free clock switch
    //(2-2) FRC-R2 clcok - 345 MHz(2'b01000)
    // [1:0] : 2'b00
    // [4:2] :  000: 216MHz, 001: 144MHz, 010: 345MHz, 011: 108MHz, 100: 54MHz, 101: 172MHz, 110: 320MHz, 111: xtal_12MHz"
    HAL_CPU_NPM_RegWriteByte(NPM_REG_CLKGEN1_FRCR2, 0x08UL);


    #if 1 
    //(3) Set CPU SDR base
    base_addr = u32_ADR;
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SDR_LO_INST_BASE, (base_addr&0x0000FFFFUL)); //I Fetch Offset - Low
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SDR_HI_INST_BASE, (base_addr>>16)); //I Fetch Offset - High
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SDR_LO_DATA_BASE, (base_addr&0x0000FFFFUL)); //D Fetch Offset - Low
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SDR_HI_DATA_BASE, (base_addr>>16)); //D Fetch Offset - High
    #else
    //(3) Set CPU reset base(vector)
    base_addr = u32_ADR >> 16;
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_RST_BASE, base_addr); // reset vector address 0x0(64K alignment)
    #endif

    //(4)Set MAU Mapping
    HAL_FRCR2_RegWrite2Byte(FRC_R2_MAULV_REG, 0x8500UL);
    
    //(5) Set RIU   base
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_RIU_BASE, 0xFA00UL); // RIU  base address
    //(6) Set SPI/UART base
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_IO1_BASE, 0xF800UL); // UART base address
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SPI_BASE, 0xF900UL);
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SPI_BASE1, 0xF900UL); 
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_DQMEM_BASE, 0xFB00UL); 
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_QMEM_MASK_HIGH, 0xFF00UL);
    //(7) Set IO space enable (UART, RIU) with QMEM space disabled
    HAL_FRCR2_RegWrite2Byte(FRC_R2_REG_SPACE_EN, 0x0003UL); // io_space_en[3:0]: UART,RIU, qmem_space_en[4] : disable

    //(8) Set QMEM 
    HAL_FRCR2_RegWrite2Byte(R2_REG_QMEM_BASE_HIGH, 0x8000UL); // QMEM base address = 0x80000000
    HAL_FRCR2_RegWrite2Byte(R2_REG_QMEM_MASK_HIGH, 0xF000UL); // QMEM less, set DCU Decoding..

    //(1) Enable R2 -> Start
    HAL_FRCR2_RegWriteByte(FRC_R2_REG_STOP, 0x24UL); // miu_sw_rst and sdram_boot = 1
    HAL_FRCR2_RegWriteByte(FRC_R2_REG_STOP, 0x27UL); // sdram_boot and (miu/r2/r2_rst) =1
  #else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
  #endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_FRCR2_Init_Front()
/// @brief \b Function \b Description:
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_FRCR2_Init_Front(void)
{
  #if defined(MCU_ARM_CA7) || defined(__aarch64__)
    MS_BOOL bResult = FALSE; 
    MS_BOOL bDisableFRC = FALSE; 
    MS_BOOL bIgnoreDisableFRC = FALSE; 
    
    HAL_CPU_PM_RegWrite2Byte(0x2050, 0x21BC); 
    MsOS_DelayTaskUs(5);
    bDisableFRC = ((HAL_CPU_PM_RegRead2Byte(0x2058) & 0x0100) >> 8);
	bIgnoreDisableFRC = HAL_CPU_PM_RegRead2Byte(0x2058) & 0x0001;
    if (bIgnoreDisableFRC || (bDisableFRC == FALSE))
	{
		bResult = TRUE;
	}
    if (bResult == FALSE) {
		return;
	}
    COPRO_HAL_INFO("ARM is HK!\n");
    HAL_FRCR2_Disable();
    MsOS_DisableInterrupt(E_INT_IRQ_FRC_INT_FIQ2HST0 );
  #else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
  #endif

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_FRCR2_Init_End()
/// @brief \b Function \b Description:
/// @param  <IN>     \b None :
/// @param  <OUT>    \b None :
/// @param  <RET>    \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_FRCR2_Init_End(MS_U32 u32_ADR)
{
  #if defined(MCU_ARM_CA7) || defined(__aarch64__) 
    MS_BOOL bResult = FALSE; 
    MS_BOOL bDisableFRC = FALSE; 
    MS_BOOL bIgnoreDisableFRC = FALSE; 
    
    HAL_CPU_PM_RegWrite2Byte(0x2050, 0x21BC); 
    MsOS_DelayTaskUs(5);
	bDisableFRC = ((HAL_CPU_PM_RegRead2Byte(0x2058) & 0x0100) >> 8);
	bIgnoreDisableFRC = HAL_CPU_PM_RegRead2Byte(0x2058) & 0x0001;
    if (bIgnoreDisableFRC || (bDisableFRC == FALSE))
	{
		bResult = TRUE;
	}
    if (bResult == FALSE) {
		printf("FRC down!\n");
		HAL_FRCR2_RegWriteByte(FRC_R2_REG_FRC_R2_EN_BY_EFUSE, HAL_FRCR2_RegReadByte(FRC_R2_REG_FRC_R2_EN_BY_EFUSE) | (BIT0));
		return;
	}
    COPRO_HAL_INFO("ARM is HK!\n");
    HAL_FRCR2_Enable(u32_ADR);
    MsOS_EnableInterrupt(E_INT_IRQ_FRC_INT_FIQ2HST0 );
  #else
    COPRO_HAL_ERR("_COPRO_Register_Base: Code base error\n");
  #endif

}
#endif

MS_U32 HAL_CPU_QueryClock(void)
{
    MS_U32  dwCpuSpeed = 0;

    dwCpuSpeed = (MS_U32) ((MS_U32) HAL_COPRO_RegRead2Byte(NPM_REG_LPF_HIGH) & 0x0000FFFFUL);
    dwCpuSpeed <<= 16;
    dwCpuSpeed |= (MS_U32) ((MS_U32) HAL_COPRO_RegRead2Byte(NPM_REG_LPF_LOW) & 0x0000FFFFUL);
    
    dwCpuSpeed = (2717909UL * 1000UL) / dwCpuSpeed;

    COPRO_HAL_ERR("(CPU Speed %d MHz)\n", (MS_U32) dwCpuSpeed);

    return (dwCpuSpeed * 1000000UL);
}

