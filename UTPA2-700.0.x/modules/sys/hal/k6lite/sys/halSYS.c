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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "drvSYS.h"
#include "halSYS.h"
#include "drvMMIO.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include "drvSYS_priv.h"
#endif

#include "regSYS.h"

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
#include "halSYS_DVFS.h"
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define BIT0    (0x0001UL)
#define BIT1    (0x0002UL)
#define BIT2    (0x0004UL)
#define BIT3    (0x0008UL)
#define BIT4    (0x0010UL)
#define BIT5    (0x0020UL)
#define BIT6    (0x0040UL)
#define BIT7    (0x0080UL)
#define BIT8    (0x0100UL)
#define BIT9    (0x0200UL)
#define BIT10   (0x0400UL)
#define BIT11   (0x0800UL)
#define BIT12   (0x1000UL)
#define BIT13   (0x2000UL)
#define BIT14   (0x4000UL)
#define BIT15   (0x8000UL)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SYS_Info       sysInfo;

static MS_VIRT u32hal_sys_baseaddr=0;
static MS_VIRT u32hal_sys_baseaddr_otp=0;
static MS_U16 u16PadMuxTable[128] = { 0 };

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
extern MS_U32  g_bDvfsInitFlag;
extern SYS_IO_PROC  g_SysIoProc;
#endif

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U16 HAL_SYS_Read2Byte(MS_U32 u32RegAddr)
{
    MS_U16 u16Val=0;

    if ((u32RegAddr == 0) || (u32hal_sys_baseaddr == 0))
    {
        printf("drvSYS access RIU register error!!\r\n");
    }
    else
    {
        u16Val = ((volatile MS_U16*)(u32hal_sys_baseaddr))[u32RegAddr];
    }
    return u16Val;
}

void HAL_SYS_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    if ((u32RegAddr == 0) || (u32hal_sys_baseaddr == 0))
    {
        printf("drvSYS access RIU register error!!\r\n");
    }
    else
    {
        ((volatile MS_U16*)(u32hal_sys_baseaddr))[u32RegAddr] = u16Val;
    }
}

void XIU_TimeOutINTHandler(InterruptNum eIntNum)
{
	printf("[%s]- not support\n", __func__);
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void SYS_Init(MS_PHY phy64baseaddr)
{
    u32hal_sys_baseaddr=phy64baseaddr;

    sysInfo.Chip.DeviceId = HAL_SYS_Read2Byte(0x001E00);
    sysInfo.Chip.Version  = (HAL_SYS_Read2Byte(0x001E02) & CHIP_VERSION_MASK)  >> CHIP_VERSION_SHFT;
    sysInfo.Chip.Revision = (HAL_SYS_Read2Byte(0x001E02) & CHIP_REVISION_MASK) >> CHIP_REVISION_SHFT;
    sysInfo.Chip.MIU1Base = HAL_MIU1_BASE;
    sysInfo.Chip.MIU1BusBase = HAL_MIU1_BUS_BASE;

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
    if(g_bDvfsInitFlag == 0)
    {
        SysDvfsProc();
#if (CONFIG_DVFS_GPU_POWER_I2C_ENABLE)
        SysDvfsGpuProc();
#endif
        g_bDvfsInitFlag = 1;
    }
#endif
}


MS_U8 SYS_GetChipRev(void)
{
    return sysInfo.Chip.Revision;
}

MS_U16 SYS_GetChipID(void)
{
    return sysInfo.Chip.DeviceId;
}

const SYS_Info* SYS_GetInfo(void)
{
    return (const SYS_Info*)&sysInfo;
}


void SYS_ResetCPU(void)
{
	printf("[%s]- not support\n", __func__);
}

void HAL_SYS_RFAGC_Tristate(MS_BOOL bEnable)
{
	printf("[%s]- not support\n", __func__);
}

void HAL_SYS_IFAGC_Tristate(MS_BOOL bEnable)
{
	printf("[%s]- not support\n", __func__);
}

void HAL_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux)
{
	printf("[%s]- not support\n", __func__);
}

MS_BOOL HAL_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode)
{
	printf("[%s]- not support\n", __func__);
    return FALSE;
}

MS_BOOL HAL_SYS_DisableDebugPort(void)
{
	printf("[%s]- not support\n", __func__);
    return FALSE;
}

MS_BOOL HAL_SYS_EnableDebugPort(void)
{
	printf("[%s]- not support\n", __func__);
    return FALSE;
}

MS_BOOL HAL_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel)
{
    MS_BOOL ret = TRUE;
    MS_U16  u16data=0;
    MS_U16  u16data2=0;

    if((ePadMuxType >= E_PAD_SET_MAX) || (ePadSel >= E_PAD_SEL_MAX))
    {
        ret = FALSE;
        return ret;
    }

    if(ePadMuxType == E_TS0_PAD_SET)
    {
        u16data =  HAL_SYS_Read2Byte(REG_TS_MODE_OFFSET) & (~(REG_TS0_MODE_MASK <<REG_TS0_MODE_SHIFT));// TS0_MODE = 0

        if(ePadSel == E_PARALLEL_IN)
        {
            u16data |= REG_TS_MODE_1P<<REG_TS0_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
        }
        else if(ePadSel == E_PAD_DRIVING_4MA) // TS0, clk/sync/d0/vld driving current 4MA
        {
            u16data = HAL_SYS_Read2Byte(REG_TS0_3_WIRE_EN_OFFSET) & (~(REG_TS0_VLD_DRV|REG_TS0_SYNC_DRV|REG_TS0_D0_DRV|REG_TS0_CLK_DRV));
            HAL_SYS_Write2Byte(REG_TS0_3_WIRE_EN_OFFSET, u16data);
        }
        else if(ePadSel == E_PAD_DRIVING_8MA) // TS0, clk/sync/d0/vld driving current 8MA
        {
            u16data = HAL_SYS_Read2Byte(REG_TS0_3_WIRE_EN_OFFSET) | (REG_TS0_VLD_DRV|REG_TS0_SYNC_DRV|REG_TS0_D0_DRV|REG_TS0_CLK_DRV);
            HAL_SYS_Write2Byte(REG_TS0_3_WIRE_EN_OFFSET, u16data);
        }
        else if(ePadSel == E_SERIAL_IN)
        {
            u16data |= REG_TS_MODE_2S<<REG_TS0_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
        }
        else
        {
           ret = FALSE;
        }
    }
    else if(ePadMuxType == E_TS1_PAD_SET)
    {
        u16data =  HAL_SYS_Read2Byte(REG_TS_MODE_OFFSET) & (~(REG_TS1_MODE_MASK <<REG_TS1_MODE_SHIFT));
        u16data2 = HAL_SYS_Read2Byte(REG_PWM0_MODE_OFFSET) & (~(REG_TS_OUT_MODE_MASK <<REG_TS_OUT_MODE_SHIFT));
        if(ePadSel == E_PARALLEL_IN)
        {
            u16data |= REG_TS_MODE_1P<<REG_TS1_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
            HAL_SYS_Write2Byte(REG_PWM0_MODE_OFFSET, u16data2);
        }
        else if(ePadSel == E_PARALLEL_OUT_TSO)
        {
            u16data2 |= REG_TS_OUT_MODE_TSO<<REG_TS_OUT_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
            HAL_SYS_Write2Byte(REG_PWM0_MODE_OFFSET, u16data2);
        }
		else if(ePadSel == E_PARALLEL_OUT)
        {
            u16data2 |= REG_TS_OUT_MODE_DEMOD_0<<REG_TS_OUT_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
            HAL_SYS_Write2Byte(REG_PWM0_MODE_OFFSET, u16data2);
        }
        else if(ePadSel == E_PAD_DRIVING_4MA) // TS1, clk/sync/d0/vld driving current 4MA
        {
            u16data = HAL_SYS_Read2Byte(REG_TS0_3_WIRE_EN_OFFSET) & (~(REG_TS1_VLD_DRV|REG_TS1_SYNC_DRV|REG_TS1_D0_DRV|REG_TS1_CLK_DRV));
            HAL_SYS_Write2Byte(REG_TS0_3_WIRE_EN_OFFSET, u16data);
        }
        else if(ePadSel == E_PAD_DRIVING_8MA) // TS1, clk/sync/d0/vld driving current 8MA
        {
            u16data = HAL_SYS_Read2Byte(REG_TS0_3_WIRE_EN_OFFSET) | (REG_TS1_VLD_DRV|REG_TS1_SYNC_DRV|REG_TS1_D0_DRV|REG_TS1_CLK_DRV);
            HAL_SYS_Write2Byte(REG_TS0_3_WIRE_EN_OFFSET, u16data);
        }
        else if(ePadSel == E_MSPI_PAD_ON)
        {
            u16data = HAL_SYS_Read2Byte(REG_MCU_JTAG_MODE_OFFSET);
            u16data &= (~(REG_MSPI1_MODE1_MASK <<REG_MSPI1_MODE1_SHIFT));
            u16data &= (~(REG_MSPI1_MODE2_MASK <<REG_MSPI1_MODE2_SHIFT));
            HAL_SYS_Write2Byte(REG_MCU_JTAG_MODE_OFFSET, u16data);

            u16data = HAL_SYS_Read2Byte(REG_MCU_JTAG_MODE_OFFSET) & (~(REG_MSPI1_MODE3_MASK<<REG_MSPI1_MODE3_SHIFT));
            u16data |= REG_MSPI1_MODE1_TS1<<REG_MSPI1_MODE3_SHIFT;
            HAL_SYS_Write2Byte(REG_MCU_JTAG_MODE_OFFSET, u16data);
        }
		else if(ePadSel == E_SERIAL_IN)
        {
            u16data |= REG_TS_MODE_2S<<REG_TS1_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
            HAL_SYS_Write2Byte(REG_PWM0_MODE_OFFSET, u16data2);
        }
        else
        {
           ret = FALSE;
        }
    }
    else if(ePadMuxType == E_TS2_PAD_SET)
    {
        u16data = HAL_SYS_Read2Byte(REG_TS_MODE_OFFSET) & (~(REG_TS2_MODE_MASK <<REG_TS2_MODE_SHIFT));;
        if(ePadSel == E_PARALLEL_IN)
        {
            u16data |= REG_TS_MODE_1P<<REG_TS2_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
        }
        else if(ePadSel == E_SERIAL_IN) // SERIAL_IN
        {
            u16data |= REG_TS_MODE_2S<<REG_TS2_MODE_SHIFT;
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
        }
        else if(ePadSel == E_MSPI_PAD_ON)
        {
            HAL_SYS_Write2Byte(REG_TS_MODE_OFFSET, u16data);
            u16data = HAL_SYS_Read2Byte(REG_MCU_JTAG_MODE_OFFSET);
            u16data &= (~(REG_MSPI1_MODE3_MASK <<REG_MSPI1_MODE3_SHIFT));
            u16data |= REG_MSPI1_MODE1_TS2<<REG_MSPI1_MODE3_SHIFT;
            HAL_SYS_Write2Byte(REG_MCU_JTAG_MODE_OFFSET, u16data);
        }
        else
        {
           ret = FALSE;
        }
    }
    else
    {
        ret = FALSE;
    }

    return ret;


}

MS_BOOL HAL_SYS_SetTSOutClockPhase(MS_U16 u16Val)
{
    MS_BOOL ret = TRUE;
    MS_U16  u16data=0;

    u16data =  HAL_SYS_Read2Byte(REG_TSO0_OUT_PH_TUN_NUM_OFFSET) & (~(REG_TSO0_OUT_PH_TUNE_NUM_MASK<<REG_TSO0_OUT_PH_TUNE_NUM_SHIFT));

    u16data |= u16Val<<REG_TSO0_OUT_PH_TUNE_NUM_SHIFT;
    u16data |= 1<<REG_PH_TUN_EN_SHIFT;
	
    HAL_SYS_Write2Byte(REG_TSO0_OUT_PH_TUN_NUM_OFFSET, u16data);
	
    return ret;
}

MS_U8 SYS_VIF_ReadByteByVDMbox(MS_U32 u32Reg)
{
    printf("[%s]- not support\n", __func__);
    return 0;
}

void SYS_VIF_WriteByteByVDMbox(MS_U32 u32Reg, MS_U8 u8Val)
{
    printf("[%s]- not support\n", __func__);
}

void SYS_VIF_WriteByteMaskByVDMbox(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Mask)
{
    printf("[%s]- not support\n", __func__);
}

void SYS_VIF_WriteRegBitByVDMbox(MS_U32 u32Reg, MS_U8 bEnable, MS_U8 u8Mask)
{
    printf("[%s]- not support\n", __func__);
}

MS_U16 SYS_VIF_Read2ByteByVDMbox(MS_U32 u32Reg)
{
    printf("[%s]- not support\n", __func__);
    return 0;
}

void HAL_SYS_SetEfuseIOMapBase(MS_VIRT virtBase)
{
    printf("[%s]- not support\n", __func__);
}

void HAL_SYS_SetOtpIOMapBase(MS_VIRT u32Base)
{
	u32hal_sys_baseaddr_otp = u32Base;
}

MS_BOOL HAL_SYS_Query(E_SYS_QUERY id)
{
    MS_BOOL bRet = FALSE;
    MS_U32 u32OTP_0x3D0C_shadow = 0;
    MS_U32 u32OTP_0x3D10_shadow = 0;

    u32OTP_0x3D0C_shadow = *(volatile MS_U32 *)(u32hal_sys_baseaddr_otp + 0x20000 + 0x2aac);
    u32OTP_0x3D10_shadow = *(volatile MS_U32 *)(u32hal_sys_baseaddr_otp + 0x20000 + 0x2ab0);

    switch (id)
    {
        case E_SYS_QUERY_H264_SUPPORTED:
        {
            MS_U32 u32OTPValue = 0;
            u32OTPValue = (u32OTP_0x3D0C_shadow >> 24) & 0x1;
            bRet = (u32OTPValue == 1)?FALSE:TRUE;
            break;
        }
        case E_SYS_QUERY_GINGA_NCL_SUPPORTED:
        {
            MS_U32 u32OTPValue = 0;
            u32OTPValue = (u32OTP_0x3D10_shadow >> 4) & 0x3;
            if ((u32OTPValue == 0x1) || (u32OTPValue == 0x2))
                bRet = TRUE;
            break;
        }
        case E_SYS_QUERY_GINGA_JAVA_SUPPORTED:
        {
            MS_U32 u32OTPValue = 0;
            u32OTPValue = (u32OTP_0x3D10_shadow >> 4) & 0x3;
            if (u32OTPValue == 0x2)
                bRet = TRUE;
            break;
        }
        case E_SYS_QUERY_HBBTV_SUPPORTED:
        {
            MS_U32 u32OTPValue = 0;
            u32OTPValue = (u32OTP_0x3D10_shadow >> 6) & 0x1;
            bRet = (u32OTPValue == 1)?FALSE:TRUE;
            break;
        }
        default:
            //printf ("\n Un-supported E_SYS_QUERY value...%d \n", id);//Debug
            bRet = TRUE;
            break;
    }

    return bRet;

}

MS_U32 HAL_SYS_ReadRSAKey(MS_U16 u16ReadAddr)
{
    printf("[%s]- not support\n", __func__);
    return FALSE;
}

MS_BOOL HAL_SYS_PadMuxTableSuspend(void)
{
    int i = 0;

    for (i = 0; i < REG_CHIPTOP_REG_NUM; i++)
    {
        u16PadMuxTable[i] = HAL_SYS_Read2Byte(REG_CHIPTOP_REG(i));
    }

    return TRUE;
}

MS_BOOL HAL_SYS_PadMuxTableResume(void)
{
    int i = 0;

    for (i = 0; i < REG_CHIPTOP_REG_NUM; i++)
    {
        HAL_SYS_Write2Byte(REG_CHIPTOP_REG(i), u16PadMuxTable[i]);
    }

    return TRUE;
}

MS_BOOL HAL_SYS_SetTSClockPhase(SYS_PAD_MUX_SET ePadMuxType,MS_U16 u16Val)
{
    if(ePadMuxType >= E_PAD_SET_MAX)
    {
        return FALSE;
    }

    if(u16Val > 31)
    {
        return FALSE;
    }

    if(u32hal_sys_baseaddr == 0)
    {
        return FALSE;
    }

    if(ePadMuxType == E_TS0_PAD_SET)
    {
        HAL_SYS_Write2Byte(0x100B56, u16Val << 8);
        HAL_SYS_Write2Byte(0x100B56, HAL_SYS_Read2Byte(0x100B56) | 0x3);
    }
    else if(ePadMuxType == E_TS1_PAD_SET)
    {
        HAL_SYS_Write2Byte(0x100B58, u16Val << 8);
        HAL_SYS_Write2Byte(0x100B56, HAL_SYS_Read2Byte(0x100B56) | 0x2);
        HAL_SYS_Write2Byte(0x100B58, HAL_SYS_Read2Byte(0x100B58) | 0x1);
    }
    else
    {
        return FALSE;
    }

    return TRUE;

}

MS_U32 HAL_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index)
{
    MS_U32 u32OTP_0x3D10_shadow = 0;

    u32OTP_0x3D10_shadow = *(volatile MS_U32 *)(u32hal_sys_baseaddr_otp + 0x20000 + 0x2ab0);

    switch(index)
    {
        case E_SYS_DOLBY_VERSION:
            return (MS_U32)((u32OTP_0x3D10_shadow >> 9) & (0x7F)); //0x3D10[15:9]
        case E_SYS_DOLBY_CONTROL_BIT:
            return 1; //This bit is removed from OTP table, always return true to accept hash 2.0
        case E_SYS_DOLBY_REVERSE_BIT:
            return (MS_U32)((u32OTP_0x3D10_shadow >> 22) & (0x01)); // 0x3D10 [22]
        default:
            return 0;
    }

    return 0;
}

void HAL_SYS_SetChipType(E_SYS_CHIP_TYPE Type)
{

}

E_SYS_CHIP_TYPE HAL_SYS_GetChipType(void)
{
    return E_SYS_CHIP_STB;
}

MS_U16 HAL_SYS_ReadBrickTerminatorStatus(void)
{
    MS_U16 u16Status = 0;

    u16Status = HAL_SYS_Read2Byte(0x10051E);

    return u16Status;
}

void HAL_SYS_WriteBrickTerminatorStatus(MS_U16 u16Status)
{
    HAL_SYS_Write2Byte(0x10051E, u16Status);
}

void HAL_SYS_GetEfuseDid(MS_U16 *u16efuse_did)
{
    printf("[%s]- not support\n", __func__);
}

MS_BOOL HAL_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey)
{
    printf("[%s]- not support\n", __func__);
    return FALSE;
}

//=================================================================================================
#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
static MS_S32 _s32SAR_Dvfs_Mutex;
static MSTAR_DVFS_INFO hMstarDvfsInfo =
{
    .bDvfsInitOk = 0,
    .dwVidSetting = 0,
    .dwPowerChipId = CONFIG_DVFS_CHIP_ID_UNKNOWN,
};

//#if (CONFIG_DVFS_CPU_POWER_I2C_ENABLE | CONFIG_DVFS_CORE_POWER_I2C_ENABLE)
//extern MS_BOOL MApi_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8AddrNum, MS_U8* paddr, MS_U16 u16size, MS_U8* pu8data);
//extern MS_BOOL MApi_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrcount, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
//#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsProc
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////

static pthread_t   hDvfsThread;
static void *SysDvfsThreadProc(void *pThreadInfo)
{
    int                     hFileHandle;
    int                     dwDataLength = 0;
    unsigned int            dwLoopCounter = 0;
    char                    sFileName[48] = "/proc/on_demand_ctl\0";
    MSTAR_DVFS_READ_INFO    hMstarDvfsReadInfo;
    MSTAR_DVFS_WRITE_INFO   hMstarDvfsWriteInfo;

    // the dvfs process always need to do, so doesn't need to escape
    // coverity[no_escape]
    while(1)
    {
        MsOS_DelayTaskUs(100000);

        if((g_SysIoProc.SysSwI2CReadBytes != 0) && (g_SysIoProc.SysSwI2CWriteBytes != 0))
        {
            hFileHandle = open(sFileName, (O_RDWR | O_EXCL));
            if(hFileHandle == -1)
            {
                DVFS_DEBUG("\033[35m[ERROR] Cannot Open File: %s\033[m\n", sFileName);
            }
            else
            {
                // the dvfs process always need to do, so doesn't need to escape
                // coverity[no_escape]
                while(1)
                {
                    dwDataLength = read(hFileHandle, (void *) &hMstarDvfsReadInfo, sizeof(hMstarDvfsReadInfo));

                    DVFS_DEBUG("\033[35m[INFO] Read Data Length: %d\033[m\n", dwDataLength);
                    DVFS_DEBUG("\033[35m[INFO] CPU Clock: %d\033[m\n", hMstarDvfsReadInfo.dwCpuClock);
                    DVFS_DEBUG("\033[35m[INFO] Voltage: %d\033[m\n", hMstarDvfsReadInfo.dwVoltage);
                    DVFS_DEBUG("\033[35m[INFO] Voltage Type: %d\033[m\n", hMstarDvfsReadInfo.dwVoltageType);
                    DVFS_DEBUG("\033[35m[INFO] Data Exchange Count (Read): %d\033[m\n", hMstarDvfsReadInfo.dwDataExchangeCount);

                    if(hMstarDvfsInfo.bDvfsInitOk == 0)
                    {
                        SysDvfsInit();
                        hMstarDvfsInfo.bDvfsInitOk = 1;
                    }

                    if(hMstarDvfsReadInfo.dwVoltageType == CONFIG_DVFS_CPU_POWER)
                    {
                        SysDvfsCpuPowerAdjustment(hMstarDvfsReadInfo.dwVoltage);
                    }
                    else if(hMstarDvfsReadInfo.dwVoltageType == CONFIG_DVFS_CORE_POWER)
                    {
                        SysDvfsCorePowerAdjustment(hMstarDvfsReadInfo.dwVoltage);
                    }
                    else if(hMstarDvfsReadInfo.dwVoltageType == CONFIG_DVFS_STR_INIT)
                    {
                        SysDvfsInit();
                    }

            hMstarDvfsWriteInfo.dwDataExchangeCount = hMstarDvfsReadInfo.dwDataExchangeCount;
            hMstarDvfsWriteInfo.dwDataExchangeResult = 0;
            dwDataLength = write(hFileHandle, (void *)&hMstarDvfsWriteInfo, sizeof(hMstarDvfsWriteInfo));
            DVFS_DEBUG("\033[35m[INFO] Write Data Length: %d\033[m\n", dwDataLength);
            DVFS_DEBUG("\033[35m[INFO] Data Exchange Count (Write): %d\033[m\n", hMstarDvfsWriteInfo.dwDataExchangeCount);
            DVFS_DEBUG("\033[35m[INFO] Data Exchange Result: %d\033[m\n", hMstarDvfsWriteInfo.dwDataExchangeResult);

        }

                close(hFileHandle);
            }
        }

        dwLoopCounter ++;
        if(dwLoopCounter > 50)
        {
            pthread_exit(NULL);
            break;
        }
    }

    return NULL;
}

void SysDvfsProc(void)
{
    if(*(volatile MS_U16 *)(u32hal_sys_baseaddr + (0x100500 << 1)) == CONFIG_DVFS_ENABLE_PATTERN)
    {
        if(hMstarDvfsInfo.bDvfsInitOk == 0)
        {
            int dwErrorCode = 0;
            char sDvfsThreadName[20] = "MstarDvfsThread\0";
            pthread_attr_t attr;

            _s32SAR_Dvfs_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex SAR_DVFS", MSOS_PROCESS_SHARED);
            MS_ASSERT(_s32SAR_Dvfs_Mutex >= 0);

            if (FALSE == MsOS_ObtainMutex(_s32SAR_Dvfs_Mutex, CONFIG_DVFS_MUTEX_WAIT_TIME))
            {
                DVFS_INFO("\033[37m[DVFS] Mutex Lock Fail\033[m\n");
                MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
                return;
            }

            //SysDvfsInit();

            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

            dwErrorCode = pthread_create(
                                &hDvfsThread,
                                &attr,
                                SysDvfsThreadProc,
                                (void *) sDvfsThreadName);

            if(dwErrorCode)
            {
                DVFS_DEBUG("\033[33m[ERROR] Create DVFS Thread Failed\033[m\n");
                MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
                return;
            }

            MsOS_ReleaseMutex(_s32SAR_Dvfs_Mutex);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsInit
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsInit(void)
{
    SysDvfsCpuPowerInit();
    SysDvfsCorePowerInit();

    SysDvfsCpuPowerAdjustment(CONFIG_DVFS_CPU_POWER_DEFAULT);
    SysDvfsCorePowerAdjustment(CONFIG_DVFS_CORE_POWER_DEFAULT);

    hMstarDvfsInfo.bDvfsInitOk = 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuPowerAdjustment
/// @brief \b Function  \b Description: Update Output Voltage Level in External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuPowerAdjustment(MS_U32 dwCpuPowerVoltage)
{
#if CONFIG_DVFS_CPU_POWER_I2C_ENABLE
    MS_U32  dwRegisterValue = 0;
    MS_U32  dwOriginalCpuPowerVoltage = 0;
    MS_U32  dwSourceRegisterSetting = 0;
    MS_U32  dwTargetRegisterSetting = 0;


    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    if(dwCpuPowerVoltage < CONFIG_DVFS_CPU_POWER_SHIFT_PRADO)
    {
        printf("Error- dwCpuPowerVoltage is smaller than CONFIG_DVFS_CPU_POWER_SHIFT_PRADO!!!\n");
        return;
    }
    else if(dwCpuPowerVoltage > CONFIG_DVFS_CPU_POWER_MAX)
    {
        printf("Error- dwCpuPowerVoltage is larger than CONFIG_DVFS_CPU_POWER_MAX!!!\n");
        return;
    }

    if(hMstarDvfsInfo.dwPowerChipId == CONFIG_DVFS_CHIP_ID_PRADO)
    {
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x06 << 1);
        if(g_SysIoProc.SysSwI2CReadBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 2, byTargetRegAddress, 2, byTargetData) == TRUE)
        {
            dwOriginalCpuPowerVoltage = (unsigned int) byTargetData[1] + CONFIG_DVFS_CPU_POWER_SHIFT_PRADO;
            DVFS_DEBUG("\033[33m[INFO] Orginal CPU Power: %d0 mV\033[m\n", (unsigned int) dwOriginalCpuPowerVoltage);
        }
        else
        {
             printf("I2C Read Bytes with CONFIG_DVFS_POWER_SWI2C_ADDR_CPU Fail!!!\n");
             return;
        }

        dwSourceRegisterSetting = (dwOriginalCpuPowerVoltage - CONFIG_DVFS_CPU_POWER_SHIFT_PRADO);
        dwTargetRegisterSetting = (dwCpuPowerVoltage - CONFIG_DVFS_CPU_POWER_SHIFT_PRADO);

        if(hMstarDvfsInfo.bDvfsInitOk == 1)
        {
            if(dwCpuPowerVoltage > dwOriginalCpuPowerVoltage)
            {
                for(;dwSourceRegisterSetting <= dwTargetRegisterSetting; dwSourceRegisterSetting += CONFIG_DVFS_CPU_POWER_STEP)
                {
                    //Set CPU Voltage
                    dwRegisterValue = dwSourceRegisterSetting;
                    byTargetRegAddress[0] = 0x10;
                    byTargetRegAddress[1] = (0x06 << 1);
                    byTargetRegAddress[2] = 0x10;
                    byTargetRegAddress[3] = dwRegisterValue;
                    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
                    {
                        DVFS_INFO("\033[37m[INFO] Change to Voltage: %d0 mV (0x%X)\033[m\n", (unsigned int) dwCpuPowerVoltage, (unsigned int) dwRegisterValue);
                    }
                    else
                    {
                        DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
                    }
                }
            }
            else if(dwCpuPowerVoltage < dwOriginalCpuPowerVoltage)
            {
                for(;dwSourceRegisterSetting >= dwTargetRegisterSetting; dwSourceRegisterSetting -= CONFIG_DVFS_CPU_POWER_STEP)
                {
                    //Set CPU Voltage
                    dwRegisterValue = dwSourceRegisterSetting;
                    byTargetRegAddress[0] = 0x10;
                    byTargetRegAddress[1] = (0x06 << 1);
                    byTargetRegAddress[2] = 0x10;
                    byTargetRegAddress[3] = dwRegisterValue;
                    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
                    {
                        DVFS_INFO("\033[37m[INFO] Change to Voltage: %d0 mV (0x%X)\033[m\n", (unsigned int) dwCpuPowerVoltage, (unsigned int) dwRegisterValue);
                    }
                    else
                    {
                        DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
                    }
                }
            }
            else
            {
                DVFS_INFO("\033[37m[INFO] No Need to Change CPU Power\033[m\n");
            }
        }

        if(dwSourceRegisterSetting != dwTargetRegisterSetting)
        {
            //Set CPU Voltage
            dwRegisterValue = (dwCpuPowerVoltage - CONFIG_DVFS_CPU_POWER_SHIFT_PRADO);
            byTargetRegAddress[0] = 0x10;
            byTargetRegAddress[1] = (0x06 << 1);
            byTargetRegAddress[2] = 0x10;
            byTargetRegAddress[3] = dwRegisterValue;
            if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
            {
                DVFS_INFO("\033[37m[INFO] Change to Voltage: %d0 mV (0x%X)\033[m\n", (unsigned int) dwCpuPowerVoltage, (unsigned int) dwRegisterValue);
            }
            else
            {
                DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
            }
        }
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCorePowerAdjustment
/// @brief \b Function  \b Description: Update Output Voltage Level in External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCorePowerAdjustment(MS_U32 dwCorePowerVoltage)
{
    // Kano doesn't need to adjust core power after sboot
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCpuPowerInit
/// @brief \b Function  \b Description: The Init Flow of  External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCpuPowerInit(void)
{
#if CONFIG_DVFS_CPU_POWER_I2C_ENABLE
    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    byTargetRegAddress[0] = 0x53;
    byTargetRegAddress[1] = 0x45;
    byTargetRegAddress[2] = 0x52;
    byTargetRegAddress[3] = 0x44;
    byTargetRegAddress[4] = 0x42;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 5, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_Enter_I2C Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x7F;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_USE_CFG Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x7D;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_OUT_NO_DELAY Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x50;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_AD_BYTE_EN0 Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x55;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_DA_BYTE_EN1 Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x35;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[ERROR] I2C_USE Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x10;
    byTargetRegAddress[1] = 0xc0;
    if(g_SysIoProc.SysSwI2CReadBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 2, byTargetRegAddress, 2, byTargetData) == TRUE)
    {
        DVFS_DEBUG("\033[33m[INFO] MStar Power IC Chip ID: %x%x\033[m\n", (unsigned int) byTargetData[0], (unsigned int) byTargetData[1]);
        hMstarDvfsInfo.dwPowerChipId = (unsigned int) byTargetData[1];
    }

    if(hMstarDvfsInfo.dwPowerChipId == CONFIG_DVFS_CHIP_ID_PRADO)
    {
        //Set OTP Level
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x05 << 1);
        byTargetRegAddress[2] = 0x40;
        byTargetRegAddress[3] = 0x00;
        if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) != TRUE)
        {
            DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
        }

        //Set Default CPU Voltage
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x06 << 1);
        byTargetRegAddress[2] = 0x10;
        byTargetRegAddress[3] =  CONFIG_DVFS_CPU_POWER_DEFAULT - CONFIG_DVFS_CPU_POWER_SHIFT_PRADO;
        if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
        {
            DVFS_INFO("\033[37m[INFO] Setup default voltage: %d0 mV\033[m\n", (unsigned int)(CONFIG_DVFS_CPU_POWER_DEFAULT - CONFIG_DVFS_CPU_POWER_SHIFT_PRADO));
        }
        else
        {
            DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
        }

        //Set Password
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x0C << 1);
        byTargetRegAddress[2] = 0xbe;
        byTargetRegAddress[3] = 0xaf;
        if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_CPU, 4, byTargetRegAddress, 0, byTargetData) != TRUE)
        {
            DVFS_INFO("\033[37m[ERROR] Software I2C Write Failed\033[m\n");
        }
    }

    *(volatile MS_U16 *)(u32hal_sys_baseaddr + (0x100510 << 1)) = CONFIG_DVFS_DYNAMIC_POWER_ADJUST_INIT;
#endif
#if CONFIG_DVFS_CPU_POWER_GPIO_ENABLE
#error "No Support CPU Power Adjustment by GPIO in Muji Platform"
#endif
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsCorePowerInit
/// @brief \b Function  \b Description: The Init Flow of  External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsCorePowerInit(void)
{
    // Kano doesn't need to adjust core power after sboot
}

#endif

//=================================================================================================

#ifdef CONFIG_MSTAR_DVFS_KERNEL_SUPPORT
#if (CONFIG_DVFS_GPU_POWER_I2C_ENABLE)

static MS_S32 _s32SAR_Dvfs_Gpu_Mutex;
static MSTAR_DVFS_GPU_INFO hMstarDvfsGpuInfo =
{
    .bDvfsGpuInitOk = 0,
    .dwPowerChipId = CONFIG_DVFS_CHIP_ID_UNKNOWN,
};

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsGpuProc
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////

static pthread_t   hDvfsGpuThread;
static void *SysDvfsGpuThreadProc(void *pThreadInfo)
{
    int                     hFileHandle;
    int                     dwDataLength = 0;
    unsigned int            dwLoopCounter = 0;
    char                    sFileName[48] = "/proc/on_demand_ctl_gpu\0";
    MSTAR_DVFS_GPU_READ_INFO    hMstarDvfsGpuReadInfo;
    MSTAR_DVFS_GPU_WRITE_INFO   hMstarDvfsGpuWriteInfo;

    // the dvfs process always need to do, so doesn't need to escape
    // coverity[no_escape]
    while(1)
    {
        MsOS_DelayTaskUs(100000);

        if((g_SysIoProc.SysSwI2CReadBytes != 0) && (g_SysIoProc.SysSwI2CWriteBytes != 0))
        {
            hFileHandle = open(sFileName, (O_RDWR | O_EXCL));
            if(hFileHandle == -1)
            {
                DVFS_DEBUG("\033[35m[GPU_POWER][ERROR] Cannot Open File: %s\033[m\n", sFileName);
            }
            else
            {
                // the dvfs process always need to do, so doesn't need to escape
                // coverity[no_escape]
                while(1)
                {
                    dwDataLength = read(hFileHandle, (void *) &hMstarDvfsGpuReadInfo, sizeof(hMstarDvfsGpuReadInfo));

                    DVFS_DEBUG("[GPU_POWER][INFO] Read Data Length: %d\n", dwDataLength);
                    DVFS_DEBUG("\033[35m[GPU_POWER][INFO] Voltage: %d\033[m\n", hMstarDvfsGpuReadInfo.dwVoltage);
                    DVFS_DEBUG("\033[34m[GPU_POWER][INFO] GpuClock: %d\033[m\n", hMstarDvfsGpuReadInfo.dwGpuClock);
                    DVFS_DEBUG("[GPU_POWER][INFO] Data Exchange Count (Read): %d\n", hMstarDvfsGpuReadInfo.dwDataExchangeCount);

                    if(hMstarDvfsGpuInfo.bDvfsGpuInitOk == 0)
                    {
                        SysDvfsGpuPowerInit();
                        hMstarDvfsGpuInfo.bDvfsGpuInitOk = 1;
                    }

                    SysDvfsGpuPowerAdjustment(hMstarDvfsGpuReadInfo.dwVoltage);

                    hMstarDvfsGpuWriteInfo.dwDataExchangeCount = hMstarDvfsGpuReadInfo.dwDataExchangeCount;
                    hMstarDvfsGpuWriteInfo.dwDataExchangeResult = 0;
                    dwDataLength = write(hFileHandle, (void *)&hMstarDvfsGpuWriteInfo, sizeof(hMstarDvfsGpuWriteInfo));
                    DVFS_DEBUG("[GPU_POWER][INFO] Write Data Length: %d\n", dwDataLength);
                    DVFS_DEBUG("[GPU_POWER][INFO] Data Exchange Count (Write): %d\n", hMstarDvfsGpuWriteInfo.dwDataExchangeCount);
                    DVFS_DEBUG("[GPU_POWER][INFO] Data Exchange Result: %d\n", hMstarDvfsGpuWriteInfo.dwDataExchangeResult);
                }

                close(hFileHandle);
            }
        }

        dwLoopCounter ++;
        if(dwLoopCounter > 50)
        {
            pthread_exit(NULL);
            break;
        }
    }

    return NULL;
}

void SysDvfsGpuProc(void)
{
    DVFS_INFO("\033[33m[GPU_POWER][INFO] SysDvfsGpuProc()\033[m\n");
    if(*(volatile MS_U16 *)(u32hal_sys_baseaddr + (0x100500 << 1)) == CONFIG_DVFS_ENABLE_PATTERN)
    {
        if(hMstarDvfsGpuInfo.bDvfsGpuInitOk == 0)
        {
            int dwErrorCode = 0;
            char sDvfsGpuThreadName[20] = "MstarDvfsGpuThread\0";
            pthread_attr_t attr;

            _s32SAR_Dvfs_Gpu_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, "Mutex SAR_DVFS_GPU", MSOS_PROCESS_SHARED);
            MS_ASSERT(_s32SAR_Dvfs_Gpu_Mutex >= 0);

            if (FALSE == MsOS_ObtainMutex(_s32SAR_Dvfs_Gpu_Mutex, CONFIG_DVFS_MUTEX_WAIT_TIME))
            {
                DVFS_INFO("\033[37m[GPU_POWER][ERROR] Mutex Lock Fail\033[m\n");
                MsOS_ReleaseMutex(_s32SAR_Dvfs_Gpu_Mutex);
                return;
            }

            SysDvfsGpuInit();
            pthread_attr_init(&attr);
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

            dwErrorCode = pthread_create(
                                &hDvfsGpuThread,
                                &attr,
                                SysDvfsGpuThreadProc,
                                (void *) sDvfsGpuThreadName);

            if(dwErrorCode)
            {
                DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] Create DVFS GPU Thread Failed\033[m\n");
                MsOS_ReleaseMutex(_s32SAR_Dvfs_Gpu_Mutex);
                return;
            }

            MsOS_ReleaseMutex(_s32SAR_Dvfs_Gpu_Mutex);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsGpuInit
/// @brief \b Function  \b Description: Read T-Sensor to Handle DVFS Flow
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsGpuInit(void)
{
    DVFS_DEBUG("\033[33m[GPU_POWER][INFO] SysDvfsGpuInit() \033[m\n");
    SysDvfsGpuPowerInit();
    SysDvfsGpuPowerAdjustment(CONFIG_DVFS_GPU_POWER_DEFAULT);

    hMstarDvfsGpuInfo.bDvfsGpuInitOk = 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsGpuPowerAdjustment
/// @brief \b Function  \b Description: Update Output Voltage Level in External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsGpuPowerAdjustment(MS_U32 dwGpuPowerVoltage)
{
    MS_U32  dwRegisterValue = 0;

    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    if(hMstarDvfsGpuInfo.dwPowerChipId == CONFIG_DVFS_CHIP_ID_PRADO)
    {
        //Set GPU Voltage
        dwRegisterValue = (dwGpuPowerVoltage - CONFIG_DVFS_GPU_POWER_SHIFT_PRADO);
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x06 << 1);
        byTargetRegAddress[2] = 0x10;
        byTargetRegAddress[3] = dwRegisterValue;
        if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 4, byTargetRegAddress, 0, byTargetData) == TRUE)
        {
            DVFS_INFO("\033[35m[GPU_POWER][INFO] Change to Voltage: %d mv (0x%X)\033[m\n", (unsigned int) dwGpuPowerVoltage, (unsigned int) dwRegisterValue);
        }
        else
        {
            DVFS_INFO("\033[35m[GPU_POWER][ERROR] Software I2C Write Failed\033[m\n");
        }
    }
    else
    {
        DVFS_INFO("\033[35m[GPU_POWER][ERROR] PowerChipId[0x%x]: not support GPU power\033[m\n", hMstarDvfsGpuInfo.dwPowerChipId);
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: SysDvfsGpuPowerInit
/// @brief \b Function  \b Description: The Init Flow of  External Power Chip
/// @param <IN>         \b None:
/// @param <OUT>        \b None:
/// @param <RET>        \b None:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
void SysDvfsGpuPowerInit(void)
{
    MS_U8   byTargetRegAddress[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };
    MS_U8   byTargetData[5] =
            {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF
            };

    byTargetRegAddress[0] = 0x53;
    byTargetRegAddress[1] = 0x45;
    byTargetRegAddress[2] = 0x52;
    byTargetRegAddress[3] = 0x44;
    byTargetRegAddress[4] = 0x42;

    DVFS_DEBUG("\033[33m[GPU_POWER][INFO] SysDvfsGpuPowerInit() \033[m\n");

    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 5, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] I2C_Enter_I2C Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x7F;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] I2C_USE_CFG Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x7D;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] I2C_OUT_NO_DELAY Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x50;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] I2C_AD_BYTE_EN0 Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x55;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] I2C_DA_BYTE_EN1 Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x35;
    if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 1, byTargetRegAddress, 0, byTargetData) != TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][ERROR] I2C_USE Failed\033[m\n");
    }

    byTargetRegAddress[0] = 0x10;
    byTargetRegAddress[1] = 0xc0;
    if(g_SysIoProc.SysSwI2CReadBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 2, byTargetRegAddress, 2, byTargetData) == TRUE)
    {
        DVFS_DEBUG("\033[33m[GPU_POWER][INFO] MStar Power IC Chip ID: %x%x\033[m\n", (unsigned int) byTargetData[0], (unsigned int) byTargetData[1]);
        hMstarDvfsGpuInfo.dwPowerChipId = (unsigned int) byTargetData[1];
    }

    if(hMstarDvfsGpuInfo.dwPowerChipId == CONFIG_DVFS_CHIP_ID_PRADO)
    {
        //Set OTP Level
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x05 << 1);
        byTargetRegAddress[2] = 0x40;
        byTargetRegAddress[3] = 0x00;
        if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 4, byTargetRegAddress, 0, byTargetData) != TRUE)
        {
            DVFS_INFO("\033[37m[GPU_POWER][ERROR] Software I2C Write Failed\033[m\n");
        }

        //Set Password
        byTargetRegAddress[0] = 0x10;
        byTargetRegAddress[1] = (0x0C << 1);
        byTargetRegAddress[2] = 0xbe;
        byTargetRegAddress[3] = 0xaf;
        if(g_SysIoProc.SysSwI2CWriteBytes(CONFIG_DVFS_POWER_SWI2C_ADDR_GPU, 4, byTargetRegAddress, 0, byTargetData) != TRUE)
        {
            DVFS_INFO("\033[37m[GPU_POWER][ERROR] Software I2C Write Failed\033[m\n");
        }
    }

    *(volatile MS_U16 *)(u32hal_sys_baseaddr + (0x100511 << 1)) = CONFIG_DVFS_GPU_DYNAMIC_POWER_ADJUST_INIT;

}

#endif /* CONFIG_DVFS_GPU_POWER_I2C_ENABLE */
#endif /* CONFIG_MSTAR_DVFS_KERNEL_SUPPORT */

#if 0
static void HAL_SYS_WOL_ISR(InterruptNum eIntNum)
{
	#if 0
    if((HAL_SYS_ReadByte(0x0e1c)&(BIT2))==(BIT2))//reg_wk_irq_final_status[2]: WOL
    {
        //step1: set WOL_WAKE_UP_SOC(PM_SPI_WPN) from High to Low to inform ext. MICOM
        HAL_SYS_WriteByte(0x0f2c, HAL_SYS_ReadByte(0x0f2c)&(~BIT1));
        //step2: clear WOL interrupt
        //2-1 WOL part
        HAL_SYS_WriteByte(0x121a46, HAL_SYS_ReadByte(0x121a46)&(~BIT7));
        HAL_SYS_WriteByte(0x121a46, HAL_SYS_ReadByte(0x121a46)|(BIT7));
        //2-2 PM part
        HAL_SYS_WriteByte(0x0e1c, HAL_SYS_ReadByte(0x0e1c)|(BIT2)); //PM
        HAL_SYS_WriteByte(0x2b38, HAL_SYS_ReadByte(0x2b38)|(BIT2)); //IRQ

        MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
    }
	#endif
}
#endif

void HAL_SYS_EnableWkEventWOL(void)
{
	#if 0
    //set WOL_WAKE_UP_SOC(PM_SPI_WPN) to High
    HAL_SYS_WriteByte(0x0f2c, HAL_SYS_ReadByte(0x0f2c)|(BIT1));

    //install ISR
    MsOS_AttachInterrupt(E_INT_IRQ_PMSLEEP, HAL_SYS_WOL_ISR);
    MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);

    // WOL enable interrupt
    HAL_SYS_WriteByte(0x0e10, HAL_SYS_ReadByte(0x0e10)&(~BIT2)); //WOL enable interrupt
    HAL_SYS_WriteByte(0x0e12, HAL_SYS_ReadByte(0x0e12)&(~BIT2)); //WOL irq polarity

    // WOL enable
    HAL_SYS_WriteByte(0x121A47, HAL_SYS_ReadByte(0x121A47) | BIT5); //avoid to wake-up in unexpect condition
    HAL_SYS_WriteByte(0x121A46, (BIT0)); // Enable WOL
	#endif
}

void HAL_SYS_DisableWkEventWOL(void)
{
	#if 0
    //set WOL_WAKE_UP_SOC(PM_SPI_WPN) to High
    HAL_SYS_WriteByte(0x0f2c, HAL_SYS_ReadByte(0x0f2c)|(BIT1));

    HAL_SYS_WriteByte(0x0e10, HAL_SYS_ReadByte(0x0e10)|(BIT2)); //WOL disable interrupt
    HAL_SYS_WriteByte(0x121a46, HAL_SYS_ReadByte(0x121a46)&(~BIT7)); //<<WOL INT clear>>
    HAL_SYS_WriteByte(0x121a46, HAL_SYS_ReadByte(0x121a46)|(BIT7));
    HAL_SYS_WriteByte(0x121A46, HAL_SYS_ReadByte(0x121A46) & ~BIT0); // Disable WOL
	#endif
}

void HAL_SYS_ResetStatusWOL(void)
{
	#if 0
    //step1: set WOL_WAKE_UP_SOC(PM_SPI_WPN) to High
    HAL_SYS_WriteByte(0x0f2c, HAL_SYS_ReadByte(0x0f2c)|(BIT1));

    //step2: clear WOL interrupt
    //WOL part
    HAL_SYS_WriteByte(0x121a46, HAL_SYS_ReadByte(0x121a46)&(~BIT7));
    HAL_SYS_WriteByte(0x121a46, HAL_SYS_ReadByte(0x121a46)|(BIT7));
    //PM part
    HAL_SYS_WriteByte(0x0e1c, HAL_SYS_ReadByte(0x0e1c)|(BIT2)); //PM
    HAL_SYS_WriteByte(0x2b38, HAL_SYS_ReadByte(0x2b38)|(BIT2)); //IRQ
	#endif
}

MS_BOOL HAL_SYS_GetStatusWOL(void)
{
	#if 0
    MS_BOOL bRET = 0;

    if((HAL_SYS_ReadByte(0x121A46) & BIT0) == BIT0 )
    {
        bRET = 1;
    }

    return bRET;
	#else
	return 0;
	#endif
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SYS_GetMemcConfg
/// @brief \b Function  \b Description: Query the result of config MEMC
/// @param <IN>         \b eSource:
/// @param <IN>         \b eTiming:
/// @param <OUT>        \b retEnMemc:
/// @param <RET>        \b E_SYS_ReturnValue:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
MS_U32  HAL_SYS_GetMemcConfg(MS_U32 eSource, MS_U32 eTiming, MS_BOOL *retEnMemc)
{
    return E_SYS_NOT_SUPPORT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_SYS_GetXcByPartConfg
/// @brief \b Function  \b Description: Query the result of config Bypart XC
/// @param <IN>         \b eSource:
/// @param <IN>         \b eInputTiming:
/// @param <IN>         \b eOutputTiming:
/// @param <OUT>        \b retEn:
/// @param <RET>        \b E_SYS_ReturnValue:
/// @param <GLOBAL>     \b None:
////////////////////////////////////////////////////////////////////////////////
MS_U32  HAL_SYS_GetXcByPartConfg (MS_U32 eSource, MS_U32 eInputTiming, MS_U32 eOutputTiming, MS_BOOL *retEn)
{
    return E_SYS_NOT_SUPPORT;
}
