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
// (¡uMStar Confidential Information¡v) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"

#if defined(__MOBILE_CASE__)
#include "drv_clkgen_cmu.h"
#endif
#if defined(_MIPS_PLATFORM_)&&defined(_MFE_T8_)&&defined(_KERNEL_MODE_)
#elif defined(__UBOOT__)
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "mhal_mfe.h"
#include "mfe_reg.h"

MS_BOOL MHal_MFE_GetHWCap(MS_U16 *width, MS_U16 *height)
{
#ifdef _MFE_EINSTEIN_
    *width = 1920;
    *height = 1088;
#else
    *width = 1280;
    *height = 720;
#endif
    return TRUE;
}

#if (defined(_MFE_T8_)||defined(_MFE_M1_))&& !defined(_KERNEL_MODE_)
unsigned int u32MFERegOSBase;

void MHAL_MFE_InitRegBase(MFE_U32 U32RegBase)
{
    ms_dprintk(HAL_L1,"HAL_MFE_InitRegBase 0x%X\n", (unsigned int)U32RegBase);
    u32MFERegOSBase = U32RegBase;
}

void MHAL_MFE_CreateRegMap(MFE_REG* mfe_reg, MFE_REG1* mfe_reg1)
{
    //mfe_reg = malloc(sizeof(MFE_REG));
    memset(mfe_reg, 0, sizeof(MFE_REG));
    //mfe_reg1 = malloc(sizeof(MFE_REG1));
    memset(mfe_reg1, 0, sizeof(MFE_REG1));

}

void MHAL_MFE_DelRegMap(MFE_REG* mfe_reg, MFE_REG1* mfe_reg1)
{
    //free(mfe_reg);
    //free(mfe_reg1);
}

#endif


void MHal_MFE_PowerOff(MFE_U32 is_off,MFE_CLK_LEVEL clock_level)
{
#ifndef WIN32

#if defined(__MOBILE_CASE__)
//#define CMU_MMP_ASIC_CLK_MFE           170 //clk_occmux_sel  CMU_CLK_OFF, CMU_CLK_32K, CMU_CLK_12M, CMU_CLK_24M, CMU_CLK_64M, CMU_CLK_85P3M, CMU_CLK_109P6M, CMU_CLK_128M, CMU_CLK_153P6M, CMU_CLK_170P6M
#ifndef CMU_CLK_24M
#define CMU_CLK_24M       CMU_MMP_ASIC_CLK_24M     
#define CMU_CLK_64M       CMU_MMP_ASIC_CLK_64M
#define CMU_CLK_128M      CMU_MMP_ASIC_CLK_128M
#define CMU_CLK_170P6M    CMU_MMP_ASIC_CLK_170P6M
#define CMU_CLK_170P6M    CMU_MMP_ASIC_CLK_170P6M
#endif

    int ret;
    if (is_off) {
        ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_OFF);
        if(ret < 0)
            ms_dprintk(HAL_L1,"CMU_MMP_ASIC_CLK_MFE CMU_CLK_OFF fail\n");
        ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MIU_MFE, CMU_CLK_OFF);
        if(ret < 0)
            ms_dprintk(HAL_L1,"CMU_MMP_ASIC_CLK_MIU_MFE CMU_CLK_OFF fail\n");
    } else {
        ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MIU_MFE, CMU_CLK_ON);
        if(ret < 0)
            ms_dprintk(HAL_L1,"CMU_MMP_ASIC_CLK_MIU_MFE CMU_CLK_ON fail\n");
        ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_ON);
        if(ret < 0)
            ms_dprintk(HAL_L1,"CMU_MMP_ASIC_CLK_MFE CMU_CLK_ON fail\n");

        ms_dprintk(HAL_L1,"clk level = %d\n",clock_level);
        switch (clock_level)
        {
            case MFE_CLK_VERY_SLOW:
                ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_24M); break;
            case MFE_CLK_SLOW:
                ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_64M); break;
            case MFE_CLK_MEDIUM:
                ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_128M); break;
            case MFE_CLK_FAST:
                ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_170P6M); break;
            default:
                ret = DrvClkgenSetClk(CMU_MMP_ASIC_CLK_MFE, CMU_CLK_170P6M); break;
        }
        if(ret < 0)
            ms_dprintk(HAL_L1,"CMU_MMP_ASIC_CLK_MFE Switch fail\n");
    }

#elif defined(_MFE_T8_) &&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_)
    if (is_off) {
        *(unsigned short *)(0xbf206600+(0x18)*4) = 1;//disable MFE clock
    } else {
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 0; // 4'b0000 123
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 4; // 4'b0100 144
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 8; // 4'b1000 172
        //*(unsigned short *)(0xbf206600+(0x18)*4) = 12; // 4'b1100 192

        if((clock_level >>2) == 0 )
            *(unsigned short *)(0xbf206600+(0x18)*4) = clock_level*4;
        else
            *(unsigned short *)(0xbf206600+(0x18)*4) = 8;
    }
    //MFE clock;
    //*(unsigned short *)(0xbf200000+(0x1980+0x18)*4) = 2<<2; //2<<2
#elif defined(_MFE_T8_) &&defined(_MIPS_PLATFORM_)&&defined(_MFE_UTOPIA_)
    if (is_off) {
        *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 1;//disable MFE clock
    } else {
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 0; // 4'b0000 123
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 4; // 4'b0100 144
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 8; // 4'b1000 172
        //*(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 12; // 4'b1100 192
        switch (clock_level)
        {
            case MFE_CLK_VERY_SLOW:
                *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 0; break;
            case MFE_CLK_SLOW:
                *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 4; break;
            case MFE_CLK_MEDIUM:
                *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 8; break;
            case MFE_CLK_FAST:
                *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 12; break;
            default:
                *(unsigned short *)(T8_RIU_BASE+(0x1980+0x18)*4) = 12; break;
        }
    }
#endif

#endif // _FPGA_
}

void MHal_MFE_SWReset(MFE_REG* mfe_reg)
{
    MFE_U16 temp;
    mfe_reg->reg_mfe_g_soft_rstz = 1;
    WriteRegMFE(0x0, mfe_reg->reg00, "", 0, "");
    ReadRegMFE(0x0, &temp);
    ms_dprintk(HAL_L1, "[HAL] SWReset reg00: %04x\n", (unsigned int)temp);
}

MFE_U32 MHal_MFE_GetBitstreamEncodedLen(void)
{
    MFE_U16 reg_mfe_s_bsp_bit_cnt_high=0;
    MFE_U16 reg_mfe_s_bsp_bit_cnt_low=0;
    MFE_U32 nHwBytes;

	// Read bit count
    ReadRegMFE(0x42, &reg_mfe_s_bsp_bit_cnt_low);
    ReadRegMFE(0x43, &reg_mfe_s_bsp_bit_cnt_high);
	// Convert into byte count
    nHwBytes = (((MFE_U32)reg_mfe_s_bsp_bit_cnt_high<<16) + reg_mfe_s_bsp_bit_cnt_low ) >> 3;
	return nHwBytes;
}


void MHal_MFE_ClearIRQ(MFE_U16 irq_bits)
{
    irq_bits = irq_bits&0x7f;
    WriteRegMFE(0x1d, irq_bits, "", 0, "");
}

void MHal_MFE_GetIRQ(MFE_U16 *irq_bits)
{
    ReadRegMFE(0x1e, irq_bits);
    ms_dprintk(HAL_L1, "[HAL] GetIRQ reg1e: 0x%x\n", (int)(*irq_bits));
}

MFE_U32 MHal_MFE_CycleReport(void)
{
    MFE_U16 tmp_reg,tmp_reg1;
    MFE_U32 tCycles = 0;
    ReadRegMFE(0x73, &tmp_reg);
    tmp_reg = tmp_reg | 0x200; //enable total time;
    WriteRegMFE(0x73, tmp_reg, "", 0, "");

    ReadRegMFE(0x76, &tmp_reg);
    ReadRegMFE(0x77, &tmp_reg1);

    tCycles = ((tmp_reg1 & 0xff)<<16) | tmp_reg;
    return tCycles;
}

void MHal_MFE_set_outbitsbuf(MFE_REG* mfe_reg, OutBitSBUF *bitsbuf,int outbufsize)
{
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
	SetObufAddr(mfe_reg, bitsbuf->start_addr, outbufsize, 0, 1);
#else
	MFE_WORD sadr_low, sadr_high, eadr_low, eadr_high;

    sadr_low = LOWORD(bitsbuf->start_addr>>MIU_SHIFT);
    sadr_high = HIWORD(bitsbuf->start_addr>>MIU_SHIFT);
    eadr_low = LOWORD(((bitsbuf->end_addr)>>MIU_SHIFT)-1);
    eadr_high = HIWORD(((bitsbuf->end_addr)>>MIU_SHIFT)-1);
	SetObufAddr(mfe_reg, sadr_low, sadr_high, eadr_low, eadr_high);
#endif
}

void MHal_MFE_SetCLKCTL(MFE_REG* mfe_reg)
{
   WriteRegMFE(0x0a, (MFE_U16)mfe_reg->reg0a, "", 0, "");
}

void MHal_MFE_ResetReg(MFE_REG* mfe_reg)
{
    memset(mfe_reg, 0, sizeof(MFE_REG));
}

void MHal_MFE_GetCRC(MFE_U8 checksum_HW[8])
{
	int i;
	MFE_U16 u16Reg1;

	ReadRegMFE(0x73, &u16Reg1);
	u16Reg1 = u16Reg1 & 0xFDFF;
	WriteRegMFE(0x73, u16Reg1, "", 0, "");

	//call HW CRC64
	for(i=0;i<4;i++){
		ReadRegMFE(0x76+i, &u16Reg1);
		checksum_HW[2*i] = (MFE_U8)(u16Reg1&0xFF);
		checksum_HW[2*i+1] = u16Reg1>>8;
	}

	ReadRegMFE(0x73, &u16Reg1);
	u16Reg1 = u16Reg1 | 0x0100;
	WriteRegMFE(0x73, u16Reg1, "", 0, "");
}


