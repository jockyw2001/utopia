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
#include "halCHIP.h"
#include "drvSYS.h"
#include "halSYS.h"
#include "drvMMIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define REG_EFUSE_OUTA_15_00    0x2008
#define REG_EFUSE_OUTA_31_16    0x200A
#define REG_EFUSE_OUTA_47_32    0x200C
#define REG_EFUSE_OUTA_63_48    0x200E
#define REG_EFUSE_OUTB_15_00    0x2010
#define REG_EFUSE_OUTB_31_16    0x2012
#define REG_EFUSE_OUTB_47_32    0x2014
#define REG_EFUSE_OUTB_63_48    0x2016
#define REG_EFUSE_OUTC_15_00    0x202C
#define REG_EFUSE_OUTC_31_16    0x202E
#define REG_EFUSE_OUTC_47_32    0x2030
#define REG_EFUSE_OUTC_63_48    0x2032
#define REG_EFUSE_OUTD_15_00    0x2034
#define REG_EFUSE_OUTD_31_16    0x2036
#define REG_EFUSE_OUTD_47_32    0x2038
#define REG_EFUSE_OUTD_63_48    0x203A

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define BIT0                    (0x0001)
#define BIT1                    (0x0002)
#define BIT2                    (0x0004)
#define BIT3                    (0x0008)
#define BIT4                    (0x0010)
#define BIT5                    (0x0020)
#define BIT6                    (0x0040)
#define BIT7                    (0x0080)
#define BIT8                    (0x0100)
#define BIT9                    (0x0200)
#define BIT10                   (0x0400)
#define BIT11                   (0x0800)
#define BIT12                   (0x1000)
#define BIT13                   (0x2000)
#define BIT14                   (0x4000)
#define BIT15                   (0x8000)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_U32 _gMIO_efuse_MapBase = 0;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SYS_Info       sysInfo;

static MS_U32 u32hal_sys_baseaddr=0;
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
    MsOS_DisableInterrupt(eIntNum);
    printf("XIU Time Out Occurred!\n");
    printf("Address is 0x%x%x\n", HAL_SYS_Read2Byte(0x100113), HAL_SYS_Read2Byte(0x100112));
    printf("Address is 0x%x%x\n", HAL_SYS_Read2Byte(0x101276), HAL_SYS_Read2Byte(0x101274));
    MsOS_EnableInterrupt(eIntNum);
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void SYS_Init(MS_U32 u32baseaddr)
{
    u32hal_sys_baseaddr=u32baseaddr;

    sysInfo.Chip.DeviceId = HAL_SYS_Read2Byte(0x001E00);
    sysInfo.Chip.Version  = (HAL_SYS_Read2Byte(0x001E02) & CHIP_VERSION_MASK)  >> CHIP_VERSION_SHFT;
    sysInfo.Chip.Revision = (HAL_SYS_Read2Byte(0x001E02) & CHIP_REVISION_MASK) >> CHIP_REVISION_SHFT;
    sysInfo.Chip.MIU1Base = HAL_MIU1_BASE;
    sysInfo.Chip.MIU1BusBase = HAL_MIU1_BUS_BASE;

    //HAL_SYS_SetTSOutClockPhase(0);

#if 0
    // Enable XIU timeout
    HAL_SYS_Write2Byte(0x100100, 0x0001);
    HAL_SYS_Write2Byte(0x100128, 0xFFFF);
    HAL_SYS_Write2Byte(0x10012A, 0xFFFF);
    MsOS_AttachInterrupt(E_INT_FIQ_XIU_TIMEOUT, (InterruptCb)XIU_TimeOutINTHandler);
    MsOS_EnableInterrupt(E_INT_FIQ_XIU_TIMEOUT);
#endif
    // 40nm MCP demod issue
    HAL_SYS_Write2Byte(0x11280e, (HAL_SYS_Read2Byte(0x11280e) | (BIT4)));
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
/*
    // Switch CPU to XTAL clk
    TOP_REG(REG_TOP_MCU_USB_STC0) = (TOP_REG(REG_TOP_MCU_USB_STC0) & ~(TOP_MCU_CLK_MASK)) | TOP_MCU_CLK_DFT;

    // Reset CPU
    TOP_REG(REG_TOP_RESET_CPU0) = 0x029F;
    TOP_REG(REG_TOP_RESET_CPU0) = 0x029F | TOP_RESET_CPU0;
*/
}

void HAL_SYS_RFAGC_Tristate(MS_BOOL bEnable)
{
    MS_BOOL     bDmdAccessPreEnabled = TRUE;
    MS_U16      wReadRegisterData = 0;

    wReadRegisterData = HAL_SYS_Read2Byte(0x101e38);

    if((wReadRegisterData & (BIT9 | BIT8)) != 0)
    {
        HAL_SYS_Write2Byte(0x101e38, (wReadRegisterData & ~(BIT9 | BIT8)));
        bDmdAccessPreEnabled = FALSE;
    }

    if(bEnable == TRUE)
    {
        HAL_SYS_Write2Byte(0x11286c, (HAL_SYS_Read2Byte(0x11286c) | (BIT0)));
    }
    else
    {
        HAL_SYS_Write2Byte(0x11286c, (HAL_SYS_Read2Byte(0x11286c) & ~(BIT0)));
    }

    if(bDmdAccessPreEnabled == FALSE)
    {
        HAL_SYS_Write2Byte(0x101e38, wReadRegisterData);
    }
}

void HAL_SYS_IFAGC_Tristate(MS_BOOL bEnable)
{
    MS_BOOL     bDmdAccessPreEnabled = TRUE;
    MS_U16      wReadRegisterData = 0;

    wReadRegisterData = HAL_SYS_Read2Byte(0x101e38);

    if((wReadRegisterData & (BIT9 | BIT8)) != 0)
    {
        HAL_SYS_Write2Byte(0x101e38, (wReadRegisterData & ~(BIT9 | BIT8)));
        bDmdAccessPreEnabled = FALSE;
    }

    if(bEnable == TRUE)
    {
        HAL_SYS_Write2Byte(0x11286c, (HAL_SYS_Read2Byte(0x11286c) | (BIT4)));
    }
    else
    {
        HAL_SYS_Write2Byte(0x11286c, (HAL_SYS_Read2Byte(0x11286c) & ~(BIT4)));
    }

    if(bDmdAccessPreEnabled == FALSE)
    {
        HAL_SYS_Write2Byte(0x101e38, wReadRegisterData);
    }
}

void HAL_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux)
{
    MS_BOOL     bDmdAccessPreEnabled = TRUE;
    MS_U16      wReadRegisterData = 0;

    wReadRegisterData = HAL_SYS_Read2Byte(0x101e38);

    if((wReadRegisterData & (BIT9 | BIT8)) != 0)
    {
        HAL_SYS_Write2Byte(0x101e38, (wReadRegisterData & ~(BIT9 | BIT8)));
        bDmdAccessPreEnabled = FALSE;
    }

    if(eAgcPadMux == E_SYS_ATV_AGC_PAD_SET)         // ATV Mode
    {
        HAL_SYS_RFAGC_Tristate(TRUE);
        HAL_SYS_IFAGC_Tristate(TRUE);
    }
    else if(eAgcPadMux == E_SYS_DTV_AGC_PAD_SET)    // DTV Mode
    {
        HAL_SYS_RFAGC_Tristate(TRUE);
        HAL_SYS_IFAGC_Tristate(FALSE);
    }
    else if(eAgcPadMux == E_SYS_DTV_AGC_PAD_SET_ALL_OFF)    // DTV Mode
    {
        HAL_SYS_RFAGC_Tristate(TRUE);
        HAL_SYS_IFAGC_Tristate(TRUE);
    }
    else
    {
        printf("[ERROR] HAL_SYS_SetAGCPadMux: Invalid AGC Pad Selection\r\n");
    }

    if(bDmdAccessPreEnabled == FALSE)
    {
        HAL_SYS_Write2Byte(0x101e38, wReadRegisterData);
    }

}

MS_BOOL HAL_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode)
{
    MS_BOOL ret = TRUE;

    if(ePCMCDMode == E_PCM_CD_SINGLE)
    {
        HAL_SYS_Write2Byte(0x102a04, (HAL_SYS_Read2Byte(0x102a04) & ~(BIT13)));
    }
    else if(ePCMCDMode == E_PCM_CD_OR)
    {
        HAL_SYS_Write2Byte(0x102a04, (HAL_SYS_Read2Byte(0x102a04) | BIT13));
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

MS_BOOL HAL_SYS_DisableDebugPort(void)
{
    return FALSE;
}

MS_BOOL HAL_SYS_EnableDebugPort(void)
{
    return FALSE;
}

MS_BOOL HAL_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel)
{
    MS_BOOL ret = TRUE;
    MS_U16  u16data=0;

    if((ePadMuxType >= E_PAD_SET_MAX) || (ePadSel >= E_PAD_SEL_MAX))
    {
        ret = FALSE;
        return ret;
    }

    if(ePadMuxType == E_TS0_PAD_SET)
    {
        u16data =  HAL_SYS_Read2Byte(0x101EA2) & ~(BIT10 | BIT9);

        if(ePadSel == E_PARALLEL_IN)
        {
            u16data |= BIT9;
            HAL_SYS_Write2Byte(0x101EA2, u16data);
        }
        else if(ePadSel == E_SERIAL_IN)
        {
            u16data |= BIT10;
            HAL_SYS_Write2Byte(0x101EA2, u16data);
        }
        else
        {
           ret = FALSE;
        }
    }
    else if(ePadMuxType == E_TS1_PAD_SET)
    {
        u16data =  HAL_SYS_Read2Byte(0x101EA2) & ~(BIT13 | BIT12 | BIT11);

        if(ePadSel == E_PARALLEL_IN)
        {
            u16data |= BIT11;
            HAL_SYS_Write2Byte(0x101EA2, u16data);
        }
        else if(ePadSel == E_PARALLEL_OUT)
        {
            u16data |= BIT12;
            HAL_SYS_Write2Byte(0x101EA2, u16data);
        }
        else if(ePadSel == E_SERIAL_IN)
        {
            u16data |= (BIT12|BIT11);
            HAL_SYS_Write2Byte(0x101EA2, u16data);
        }
        else
        {
           ret = FALSE;
        }
    }
    else if(ePadMuxType == E_TS2_PAD_SET)
    {
        u16data =  HAL_SYS_Read2Byte(0x101EA8) & ~(BIT5 | BIT4);

        if(ePadSel == E_PARALLEL_IN)
        {
            u16data |= BIT4;
            HAL_SYS_Write2Byte(0x101EA8, u16data);
        }
        else if(ePadSel == E_SERIAL_IN)
        {
            u16data |= BIT5;
            HAL_SYS_Write2Byte(0x101EA8, u16data);
        }
        else if(ePadSel == E_MSPI_PAD_ON)
        {
            u16data |= (BIT4|BIT5);
            HAL_SYS_Write2Byte(0x101EA8,u16data);
        }
        else
        {
           ret = FALSE;
        }
    }
    else if (ePadMuxType == E_CA_CI_PAD_SET)
    {
        if (ePadSel == E_CA_CI_PAD_CA)
        {
            HAL_SYS_Write2Byte(0x101E16, (HAL_SYS_Read2Byte(0x101E16) | (BIT10|BIT11))); // pad to CA, reg_sm_config
            HAL_SYS_Write2Byte(0x101E9E, (HAL_SYS_Read2Byte(0x101E9E) & ~(BIT14|BIT15))); // disable pad to CI, reg_pcmctrlconfig/reg_pcmadconfig
            HAL_SYS_Write2Byte(0x101EC8, (HAL_SYS_Read2Byte(0x101EC8) & ~(BIT1))); // disable pad to CI, reg_cictrlconfig

        }
        else if (ePadSel == E_CA_CI_PAD_CI)
        {
            HAL_SYS_Write2Byte(0x101E9E, (HAL_SYS_Read2Byte(0x101E9E) | (BIT15))); // pad to CI, reg_pcmctrlconfig
            HAL_SYS_Write2Byte(0x101E16, (HAL_SYS_Read2Byte(0x101E16) & ~(BIT10|BIT11))); // disable pad to CA, reg_sm_config
        }
        else if (ePadSel == E_CA_CI_PAD_NONE)
        {
            HAL_SYS_Write2Byte(0x101E9E, (HAL_SYS_Read2Byte(0x101E9E) & ~(BIT15))); // disable CI pad
            HAL_SYS_Write2Byte(0x101E16, (HAL_SYS_Read2Byte(0x101E16) & ~(BIT10|BIT11))); // disable pad to CA, reg_sm_config
        }
        else
        {
            ret = FALSE;
        }
    }
    else if(ePadSel == E_MSPI_PAD_ON)
    {
        u16data = HAL_SYS_Read2Byte(0x101EA2) & ~(0x3800); // TS0_MODE = 0
        HAL_SYS_Write2Byte(0x101EA2, u16data);
        u16data = HAL_SYS_Read2Byte(0x101EA8); // MSPI_MODE = 1
        u16data |= 0x30;
        HAL_SYS_Write2Byte(0x101EA8, u16data);
    }
    else
    {
        ret = FALSE;
    }

    return ret;
}

MS_BOOL HAL_SYS_SetTSOutClockPhase(MS_U16 u16Val)
{
    if(u16Val > 32)
    {
        return FALSE;
    }

    if(u32hal_sys_baseaddr == 0)
    {
        return FALSE;
    }

    //Enable Demod TS output clock phase tuning
    HAL_SYS_Write2Byte(0x103300, HAL_SYS_Read2Byte(0x103300) | BIT12);

    //Demod TS output clock phase tuning number.
    HAL_SYS_Write2Byte(0x10330a, HAL_SYS_Read2Byte(0x10330a) & ~(BIT12 | BIT11 | BIT10 | BIT9 | BIT8));
    HAL_SYS_Write2Byte(0x10330a, HAL_SYS_Read2Byte(0x10330a) | (u16Val * 0x100));

    return TRUE;
}

MS_U8 SYS_VIF_ReadByteByVDMbox(MS_U32 u32Reg)
{
    printf("%s(0x%08X) not support!!!\n", __FUNCTION__, (int)u32Reg);
    return 0;
}

void SYS_VIF_WriteByteByVDMbox(MS_U32 u32Reg, MS_U8 u8Val)
{
    printf("%s(0x%08X, 0x%02X) not support!!!\n", __FUNCTION__, (int)u32Reg, u8Val);
}

void SYS_VIF_WriteByteMaskByVDMbox(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Mask)
{
    printf("%s(0x%08X, 0x%02X, 0x%02X) not support!!!\n", __FUNCTION__, (int)u32Reg, u8Val, u8Mask);
}

void SYS_VIF_WriteRegBitByVDMbox(MS_U32 u32Reg, MS_U8 bEnable, MS_U8 u8Mask)
{
    printf("%s(0x%08X, 0x%02X, 0x%02X) not support!!!\n", __FUNCTION__, (int)u32Reg, bEnable, u8Mask);
}

MS_U16 SYS_VIF_Read2ByteByVDMbox(MS_U32 u32Reg)
{
    printf("%s(0x%08X) not support!!!\n", __FUNCTION__, (int)u32Reg);
    return 0;
}

void HAL_SYS_SetEfuseIOMapBase(MS_VIRT u32Base)
{
    _gMIO_efuse_MapBase = u32Base;
}

MS_U16 HAL_SYS_EfuseRead2Byte(MS_U32 u32RegAddr)
{
    return ((volatile MS_U16*)(_gMIO_efuse_MapBase))[u32RegAddr];
}

MS_U16 HAL_SYS_EfuseWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    return ((volatile MS_U16*)(_gMIO_efuse_MapBase))[u32RegAddr] = u16Val;
}

void HAL_SYS_SetOtpIOMapBase(MS_U32 u32Base)
{
    // Not Implemented
}

MS_BOOL HAL_SYS_Query(E_SYS_QUERY id)
{
    MS_BOOL bRet = FALSE;

    //
    // get efuse settings
    //

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_U32  dwEfuseIoBaseAddress = 0;
        MS_U32  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return FALSE;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);
    }

    //
    // determine
    //
    switch (id)
    {
        case E_SYS_QUERY_TTS_SUPPORTED:
        {
                //=============================
                // vmmm-xxxx-xxxx-xxxx
                //   Valid0 : reg_efuse_outb[19]
                //   Mode0 : reg_efuse_outb[18~16]
                // xxxx-xxxx-xxxx-vmmm
                //   Valid1 : reg_efuse_outb[23]
                //   Mode1 : reg_efuse_outb[22~20]
                //=============================
                MS_U32 u32timeout_count = 0;
                MS_U16 u16EfuseOutb31_16 = 0;
                MS_U16 u16EfuseOutb47_32 = 0;
                MS_BOOL bTTSValid0=0, bTTSValid1=0;
                MS_U8 u8TTSMode0=0, u8TTSMode1=0;

                //switch to bank a,b,c,d (reg_sel_read_256=0)
                HAL_SYS_EfuseWrite2Byte(REG_EFUSE_SEL_READ_256 , HAL_SYS_EfuseRead2Byte(REG_EFUSE_SEL_READ_256)&(~0x0100));
                //trigger addr=0x0002 for read bank b
                HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM_SET_0 , 0x0002);
                HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM_TRIGGER_0, 0x0001);
                //wait done status for trigger
                while ((HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM_TRIGGER_0) & FLAG_EFUSE_DATA_BUSY) != 0)
                {
                    if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
                    {
                        printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
                        return 0;
                    }
                    MsOS_DelayTaskUs(10);
                }
                //read bank b
                u16EfuseOutb31_16 = HAL_SYS_EfuseRead2Byte(REG_EFUSE_OUTB_31_16);
                u16EfuseOutb47_32 = HAL_SYS_EfuseRead2Byte(REG_EFUSE_OUTB_47_32);
                //get Set0 & Set1
                bTTSValid0 = (((u16EfuseOutb31_16>>0) & 0x08)!=0) ? TRUE : FALSE;
                u8TTSMode0 = (u16EfuseOutb31_16>>0) & 0x07;
                bTTSValid1 = (((u16EfuseOutb31_16>>4) & 0x08)!=0) ? TRUE : FALSE;
                u8TTSMode1 = (u16EfuseOutb31_16>>4) & 0x07;
                bRet =(MS_BOOL)((bTTSValid1) ?  u8TTSMode1 : ((bTTSValid0)? u8TTSMode0 : 0));
            }
            break;
        case E_SYS_QUERY_SECURED_IC_SUPPORTED:
            bRet = (HAL_SYS_Read2Byte(0x0038E0) & (BIT0)) == 0x0 ? FALSE : TRUE;
            break;
        default:
            printf("[SYS] Unknown query!\n");
            return TRUE;
    }

    return bRet;
}

MS_U32 HAL_SYS_ReadRSAKey(MS_U16 u16ReadAddr)
{
    //not implement yet
    return FALSE;
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
        HAL_SYS_Write2Byte(0x110B00, (HAL_SYS_Read2Byte(0x110B00) & ~0x7F) | (u16Val|BIT5));
    }
    else if(ePadMuxType == E_TS1_PAD_SET)
    {
        HAL_SYS_Write2Byte(0x110B02, (HAL_SYS_Read2Byte(0x110B02) & ~0x7F) | (u16Val|BIT5));
    }
    else if(ePadMuxType == E_TS2_PAD_SET)
    {
        HAL_SYS_Write2Byte(0x110B04, (HAL_SYS_Read2Byte(0x110B04) & ~0x7F) | (u16Val|BIT5));
    }
    else if(ePadMuxType == E_TSO_PAD_SET)
    {
        HAL_SYS_Write2Byte(0x110B20, (HAL_SYS_Read2Byte(0x110B20) & ~0x7F) | (u16Val|BIT5));
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

MS_U32 HAL_SYS_QueryCBUSInfo(E_SYS_CBUS_INFO index)
{

    MS_U16 hash_rdata_lo = 0;
    MS_U16 hash_rdata_hi = 0;
    MS_U32 u32Ret = 0;
    MS_U32 u32timeout_count = 0;

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_VIRT  dwEfuseIoBaseAddress = 0;
        MS_PHY  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return 0;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);
    }

    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_CTRL, 0x26 );
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_TRIG, 0x01 );
    while ((HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_TRIG) & FLAG_EFUSE_DATA_BUSY) != 0)
    {
        if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
        {
            printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        MsOS_DelayTaskUs(10);
    }

    hash_rdata_lo = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_47_32);
    hash_rdata_hi = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_63_48);

    switch(index)
    {
    case PAD_HOTPLUGD:
        //Bank 0x42 [12:8],
        u32Ret = (MS_U32)(((hash_rdata_lo >> 8) & (0x1F)));
        break;

    case PAD_HOTPLUGC:
        //Bank 0x42[15:13], 0x43[1:0]
        u32Ret = (MS_U32)((hash_rdata_lo >> 13) & (0x07)); //0x42[15:13],
        u32Ret |=  (MS_U32)(((hash_rdata_hi & 0x03) << 3));
        break;

    default:
        printf ("[Error] %s(%d) Unknown Cmd: 0x%x\n", __FUNCTION__, __LINE__, index);
        break;
    }

    return u32Ret;
}

MS_U32 HAL_SYS_QueryDDRAOTPInfo(E_SYS_DDR_AOTP_INFO index)
{

    MS_U16 hash_rdata_lo = 0;
    MS_U32 u32Ret = 0;
    MS_U32 u32timeout_count = 0;

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_VIRT  dwEfuseIoBaseAddress = 0;
        MS_PHY  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return 0;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);
    }

    //Bank 2, 0x29 = 2008
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_CTRL, 0x24 );
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_TRIG, 0x01 );
    while ((HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_TRIG) & FLAG_EFUSE_DATA_BUSY) != 0)
    {
        if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
        {
            printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        MsOS_DelayTaskUs(10);
    }

    hash_rdata_lo = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_31_16);

    switch(index)
    {
    case SEL_ZQ_B:
        //Bank 0x41 [15:8]
        u32Ret = (MS_U32)((hash_rdata_lo >> 8) & (0xFF));
        break;

    default:
        printf ("[Error] %s(%d) Unknown Cmd: 0x%x\n", __FUNCTION__, __LINE__, index);
        break;
    }

    return u32Ret;
}

MS_U32 HAL_SYS_QueryVIDEOATOPInfo(E_SYS_VIDEO_ATOP_INFO index)
{

    MS_U16 hash_rdata_lo = 0;
    MS_U16 hash_rdata_hi = 0;
    MS_U32 u32Ret = 0;
    MS_U32 u32timeout_count = 0;

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_VIRT  dwEfuseIoBaseAddress = 0;
        MS_PHY  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return 0;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);
    }

    //Bank 2, 0x29 = 2008
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_CTRL, 0x24 );
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_TRIG, 0x01);

    while ((HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_TRIG) & FLAG_EFUSE_DATA_BUSY) != 0)
    {
        if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
        {
            printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        MsOS_DelayTaskUs(10);
    }

    hash_rdata_lo = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_15_0);
    hash_rdata_hi = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_31_16);

    switch(index)
    {
    case GC_REF_TRIM_SEL:
        //Bank 2 [12:10], [3:0],
        u32Ret = (MS_U32)((hash_rdata_lo) & (0x1F));
        break;

    case ActivateVIDEOefuse:
        //Bank 2 [13]
        u32Ret = (MS_U32)((hash_rdata_lo >> 7) & (0x01)); //Bank 2 [13]
        break;

    case GC_IDAC1_TRIM:
        //Bank 2 [14]
        u32Ret = (MS_U32)((hash_rdata_lo >> 8) & (0xFF)); // Bank 2 [14]
        break;

    case GC_IDAC2_TRIM:
        //Bank 2 [14]
        u32Ret = (MS_U32)((hash_rdata_hi) & (0xFF)); // Bank 2 [14]
        break;

    case GC_IDAC3_TRIM:
        //Bank 2 [14]
        u32Ret = (MS_U32)((hash_rdata_hi >> 8) & (0xFF)); // Bank 2 [14]
        break;

    default:
        printf ("[Error] %s(%d) Unknown Cmd: 0x%x\n", __FUNCTION__, __LINE__, index);
        break;
    }

    return u32Ret;
}

MS_U32 HAL_SYS_QueryETHATOPInfo(E_SYS_ETH_ATOP_INFO index)
{

    MS_U16 hash_rdata_lo = 0;
    MS_U16 hash_rdata_hi = 0;
    MS_U32 u32Ret = 0;
    MS_U32 u32timeout_count = 0;

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_VIRT  dwEfuseIoBaseAddress = 0;
        MS_PHY  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return 0;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);
    }

    //Bank 0x5D, 0x27 = 0x26
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_CTRL, 0x24);
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_TRIG, 0x01);

    while ((HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_TRIG) & FLAG_EFUSE_DATA_BUSY) != 0)
    {
        if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
        {
            printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        MsOS_DelayTaskUs(10);
    }

    hash_rdata_lo = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_47_32);
    hash_rdata_hi = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_63_48);

    switch(index)
    {
    case GCR_TX_TRIM_100T:
        //Bank 20 [4:0]
        u32Ret = (MS_U32)((hash_rdata_lo) & (0x1F));
        break;

    case GCR_TX_TRIM_10T:
        //Bank 20 [9:5]
        u32Ret = (MS_U32)((hash_rdata_lo >> 4) & (0x1F));
        break;

    case GCR_TX_RO_SEL:
        //Bank 20 [13:10]
        u32Ret = (MS_U32)((hash_rdata_lo >> 10) & (0x0F));
        break;
    case GCR_LPF_ZIN:
        u32Ret = (MS_U32)((hash_rdata_lo >> 14) & (0x03));
        u32Ret |= (MS_U32)(((hash_rdata_hi) & (0x03))<< 3);
        break;

    default:
        printf ("[Error] %s(%d) Unknown Cmd: 0x%x\n", __FUNCTION__, __LINE__, index);
        break;
    }

    return u32Ret;
}


MS_U32 HAL_SYS_QueryPMSARATOPInfo(E_SYS_PM_SAR_ATOP_INFO index)
{

    MS_U16 hash_rdata_lo = 0;
    MS_U32 u32Ret = 0;
    MS_U32 u32timeout_count = 0;

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_VIRT  dwEfuseIoBaseAddress = 0;
        MS_PHY  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return 0;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);
    }

    //Bank 2, 0x29 = 2008
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_CTRL, 0x26);
    HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM1_TRIG, 0x01);

    while ((HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_TRIG) & FLAG_EFUSE_DATA_BUSY) != 0)
    {
        if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
        {
            printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        MsOS_DelayTaskUs(10);
    }

    hash_rdata_lo = HAL_SYS_EfuseRead2Byte(REG_EFUSE_FSM1_RDATA_31_16);

    switch(index)
    {
    case GCR_TRVBG:
        u32Ret = (MS_U32)(((hash_rdata_lo) & (0x1F)));
        break;

    case DATO_SAR_DATA:
        u32Ret = (MS_U32)((hash_rdata_lo >> 5) & (0x1F));
        break;

    default:
        printf ("[Error] %s(%d) Unknown Cmd: 0x%x\n", __FUNCTION__, __LINE__, index);
        break;
    }

    return u32Ret;
}

MS_U32 HAL_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index)
{

    printf("%s(0x%08X) not support!!!\n", __FUNCTION__, (int)index);
    return 0;
}

void HAL_SYS_SetChipType(E_SYS_CHIP_TYPE Type)
{

}

E_SYS_CHIP_TYPE HAL_SYS_GetChipType(void)
{
    return E_SYS_CHIP_TV;
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
    // efuse_outc [47:0]
    u16efuse_did[0] = HAL_SYS_Read2Byte(0x003800);
    u16efuse_did[1] = HAL_SYS_Read2Byte(0x003802);
    u16efuse_did[2] = HAL_SYS_Read2Byte(0x003804);

    // efuse_outd [39:0]
    u16efuse_did[3] = HAL_SYS_Read2Byte(0x003808);
    u16efuse_did[4] = HAL_SYS_Read2Byte(0x00380A);
    u16efuse_did[5] = HAL_SYS_Read2Byte(0x00380C);
}

MS_BOOL HAL_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey)
{

    MS_U16 hdcp_rdata_lo = 0;
    MS_U16 hdcp_rdata_hi = 0;
    MS_U32 u32timeout_count = 0;
    MS_U16 bank_low ;
    MS_U16 bank_higt ;
    MS_U16 trigger ;

    if(_gMIO_efuse_MapBase == 0)
    {
        MS_U32  dwEfuseIoBaseAddress = 0;
        MS_U32  dwEfuseIoBaseSize = 0;

        if(MDrv_MMIO_GetBASE(&dwEfuseIoBaseAddress, &dwEfuseIoBaseSize, MS_MODULE_PM) == FALSE)
        {
            printf("[ERROR][SYS] Get IO Base Address Failed\n");
            MS_ASSERT(0);
            return FALSE;
        }

        HAL_SYS_SetEfuseIOMapBase(dwEfuseIoBaseAddress);

    }

    if(u16ReadAddr > 75)
    {
        printf("[ERROR][SYS] Read Address overflow\n");
        MS_ASSERT(0);
        return FALSE;
    }

    MS_U32 shift_bank = (u16ReadAddr % 2 == 0) ? 0 : 1;

    // FSM0
    if(u16ReadAddr < 40)
    {
        HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM_SET_0 , (u16ReadAddr & 0xFFFE) + 0x0010	); // puls 10
        HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM_TRIGGER_0, 0x01);
        trigger = REG_EFUSE_FSM_TRIGGER_0;
        if(shift_bank == 0)
        {
        	bank_low = REG_READ_FSM_0_0;
        	bank_higt = REG_READ_FSM_0_1;
        }else
        {
        	bank_low = REG_READ_FSM_0_2;
        	bank_higt = REG_READ_FSM_0_3;
        }
    }
    else // FSM1
    {
        u16ReadAddr = u16ReadAddr - 40;
        HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM_SET_1    , (u16ReadAddr & 0xFFFE) );
        HAL_SYS_EfuseWrite2Byte(REG_EFUSE_FSM_TRIGGER_1, 0x01);
        trigger = REG_EFUSE_FSM_TRIGGER_1;
        if(shift_bank == 0)
        {
        	bank_low = REG_READ_FSM_1_0;
        	bank_higt = REG_READ_FSM_1_1;
        }else
        {
        	bank_low = REG_READ_FSM_1_2;
        	bank_higt = REG_READ_FSM_1_3;
        }
    }


    while ((HAL_SYS_EfuseRead2Byte(trigger) & FLAG_EFUSE_DATA_BUSY) != 0)
    {
        if (u32timeout_count++ > MAX_TIMEOUT_COUNT)
        {
            printf ("[Error] %s(%d) Read time out!!\n", __FUNCTION__, __LINE__);
            return 0;
        }
        MsOS_DelayTaskUs(10);
    }

    hdcp_rdata_lo = HAL_SYS_EfuseRead2Byte(bank_low);
    hdcp_rdata_hi = HAL_SYS_EfuseRead2Byte(bank_higt);

    *u32HDCPKey = (MS_U32)((hdcp_rdata_hi<<16)|hdcp_rdata_lo);

    return TRUE;

}
