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

#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"
#include "mdrv_mfe_math.h"
#include "mhal_mfe.h"

#ifdef WIN32
#include "UDMAapi.h"
#endif

#if defined(CHECK_FDC_DONE)&&defined(_MFE_UTOPIA_)
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"
#endif
#define CEILING_ALIGN(value, align) (((MS_U32)(value)+((align)-1UL)) & ~((align)-1UL))
#ifdef __MOBILE_CASE__
#include <stdio.h>
#include <string.h>
#include "MsOS.h"
#endif
#if (defined(_MFE_T8_)||defined(_MFE_M1_))&&defined(_MIPS_PLATFORM_)&&!defined(_KERNEL_MODE_)
extern MS_U32 u32MFERegOSBase;
#endif
void WriteRegMFE(MS_U32 u32Address, MS_U16 val, MS_S8 *str1, MS_S32 num, MS_S8 *str3)
{

#if defined(__MFE_G2__)
    //ms_dprintk(DRV_L5, (MS_S8*)("write: %02x=%04x\n"),u32Address,val);
    __MFE_REG(u32Address) = (MS_U32) val;
    //MsOS_DelayTask(1);
#elif defined(WIN32)
    UDMA_RIUWrite16(REG_BANK_MFE+u32Address, val);
#else
    __MFE_REG(u32Address) = val;
#endif
}

// void WriteRegMFE_Bank1(U32 u32Address, U16 val, MS_S8 *str1, MS_S32 num, MS_S8 *str3)
// {
//     sd_output_reg(sd_sw_cfg3, u32Address, val, str1, num, str3);
//     UDMA_RIUWrite16(REG_BANK_MFE1+u32Address, val);
// }
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
void WriteRegMFE_BANK1(MS_U32 u32Address, MS_U16 val, MS_S8 *str1, MS_S32 num, MS_S8 *str3)
{
#if defined(WIN32)
	UDMA_RIUWrite16(REG_BANK_MFE1+u32Address, val);
#else
    __MFE_REG1(u32Address) = val;
#endif
}
#endif

void ReadRegMFE(MS_U32 u32Address, MS_U16 *val)
{

#if defined(__MFE_G2__)
    *val = (MS_U16) __MFE_REG(u32Address);
#elif defined(WIN32)
    UDMA_RIURead16(REG_BANK_MFE+u32Address, val);
#else
    *val = __MFE_REG(u32Address);
#endif
}
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
void ReadRegMFE_BANK1(MS_U32 u32Address, MS_U16 *val)
{

#if defined(__MFE_G2__)
    *val = (MS_U16) __MFE_REG1(u32Address);
#elif defined(WIN32)
    UDMA_RIURead16(REG_BANK_MFE1+u32Address, val);
#else
    *val = __MFE_REG1(u32Address);
#endif
}
#endif
void DumpAllReg(MFE_REG* mfe_reg)
{
#if (DRV_L5==0)
    return;
#elif defined(__MOBILE_CASE__)
    MS_S32 i,j;
    MS_U16 tmp[8];
    for (i = 0; i < 0x80; i+=8) {
        for(j=0;j<8;j++)
            ReadRegMFE(i+j, (tmp+j));

        ms_dprintk(DRV_L5,"%02X=%04X %02X=%04X %02X=%04X %02X=%04X %02X=%04X %02X=%04X %02X=%04X %02X=%04X\n"),
            i, tmp[0],i+1, tmp[1],i+2, tmp[2],i+3, tmp[3],i+4, tmp[4],i+5, tmp[5],i+6, tmp[6],i+7, tmp[7]);
    }

#else
    MS_S32 i;
    MS_U16 tmp[8] = { 0 };
    for (i = 0; i < 0x80; i += 0x8) {
        ReadRegMFE(i    , &tmp[0]);
        ReadRegMFE(i + 1, &tmp[1]);
        ReadRegMFE(i + 2, &tmp[2]);
        ReadRegMFE(i + 3, &tmp[3]);
        ReadRegMFE(i + 4, &tmp[4]);
        ReadRegMFE(i + 5, &tmp[5]);
        ReadRegMFE(i + 6, &tmp[6]);
        ReadRegMFE(i + 7, &tmp[7]);
        ms_dprintk(DRV_L5, "0x%02X | 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x\n",
            (unsigned int)i,
            (unsigned int)tmp[0], (unsigned int)tmp[1], (unsigned int)tmp[2], (unsigned int)tmp[3],
            (unsigned int)tmp[4], (unsigned int)tmp[5], (unsigned int)tmp[6], (unsigned int)tmp[7]);
    }
#endif
}

#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
/*
 * pAddr: The buffer address (can be s0 or s1)
 * nBufSize: The size of the buffer
 * nObufIdx: Must be 0 or 1 (means to set s0/e0 or s1/e1)
 * bSetAdrFlag: if 1, set reg_jpe_s_bspobuf_set_adr. This is for (1) before-fs buffer setting, or (2) in-frame sw-multi-obuf switching.
 *              Note for in-frame hw-double-buffer changing, this must be 0.
 */
MS_S32 SetObufAddr(MFE_REG* mfe_reg, MS_U32 pAddr, MS_U32 nBufSize, MS_S32 nObufIdx, MS_S32 bSetAdrFlag)
{
    MS_S32 nRegWriteCount = 0;
    MS_U32 pOutBufAddr = pAddr;

    if (nObufIdx==0) {
        // For safety
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 0;
        MS_ASSERT(mfe_reg->reg3e==0x0);
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        //
        mfe_reg->reg_mfe_s_bspobuf_low = (pOutBufAddr>>3)&0xFFFF;
        mfe_reg->reg_mfe_s_bspobuf_high = pOutBufAddr>>(3+16);
        mfe_reg->reg_mfe_s_obuf_id = 0;  // s0
        WriteRegMFE(0x3c, mfe_reg->reg3c, (MS_S8*)("[%d] reg3c"), nRegWriteCount++, (MS_S8*)("bsp obuf address low"));
        WriteRegMFE(0x3d, mfe_reg->reg3d, (MS_S8*)("[%d] reg3d"), nRegWriteCount++, (MS_S8*)("bsp obuf address high"));
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 1;
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));

        // For safety
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 0;
        MS_ASSERT(mfe_reg->reg3e==0x0);
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        //
        mfe_reg->reg_mfe_s_bspobuf_low = ((pOutBufAddr+nBufSize-8)>>3)&0xFFFF;
        mfe_reg->reg_mfe_s_bspobuf_high = (pOutBufAddr+nBufSize-8)>>(3+16);
        mfe_reg->reg_mfe_s_obuf_id = 1;  // e0
        WriteRegMFE(0x3c, mfe_reg->reg3c, (MS_S8*)("[%d] reg3c"), nRegWriteCount++, (MS_S8*)("bsp obuf address low"));
        WriteRegMFE(0x3d, mfe_reg->reg3d, (MS_S8*)("[%d] reg3d"), nRegWriteCount++, (MS_S8*)("bsp obuf address high"));
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 1;
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
    }

#if defined(USE_HW_DBL_OBUF)
    if (nObufIdx==1) {
        MS_ASSERT(OBUF_NUM==2||OBUF_NUM==4);
        MS_ASSERT((pOutBufAddr&0x7)==0);

        // For safety
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 0;
        MS_ASSERT(mfe_reg->reg3e==0x0);
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        //
        mfe_reg->reg_mfe_s_bspobuf_low = (pOutBufAddr>>3)&0xFFFF;
        mfe_reg->reg_mfe_s_bspobuf_high = pOutBufAddr>>(3+16);
        mfe_reg->reg_mfe_s_obuf_id = 2;  // s1
        WriteRegMFE(0x3c, mfe_reg->reg3c, (MS_S8*)("[%d] reg3c"), nRegWriteCount++, (MS_S8*)("bsp obuf address low"));
        WriteRegMFE(0x3d, mfe_reg->reg3d, (MS_S8*)("[%d] reg3d"), nRegWriteCount++, (MS_S8*)("bsp obuf address high"));
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 1;
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));

        // For safety
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 0;
        MS_ASSERT(mfe_reg->reg3e==0x0);
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
        //
        mfe_reg->reg_mfe_s_bspobuf_low = ((pOutBufAddr+nBufSize-8)>>3)&0xFFFF;
        mfe_reg->reg_mfe_s_bspobuf_high = (pOutBufAddr+nBufSize-8)>>(3+16);
        mfe_reg->reg_mfe_s_obuf_id = 3;  // e1
        WriteRegMFE(0x3c, mfe_reg->reg3c, (MS_S8*)("[%d] reg3c"), nRegWriteCount++, (MS_S8*)("bsp obuf address low"));
        WriteRegMFE(0x3d, mfe_reg->reg3d, (MS_S8*)("[%d] reg3d"), nRegWriteCount++, (MS_S8*)("bsp obuf address high"));
        mfe_reg->reg_mfe_s_obuf_write_id_adr = 1;
        WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("write_id_adr"));
    }    // nObufIdx==1
#endif // USE_HW_DBL_OBUF

    if (bSetAdrFlag) {
        mfe_reg->reg_mfe_s_bspobuf_set_adr = 1;
        WriteRegMFE(0x3b, mfe_reg->reg3b, (MS_S8*)("[%d] reg3b"), nRegWriteCount++, (MS_S8*)("set bsp obuf"));
        mfe_reg->reg_mfe_s_bspobuf_set_adr = 0;    // HW is write-one-clear
    }
/*
#if defined(USE_HW_DBL_OBUF)
    // DEBUG codes
    {
        MFE_REG tmp_reg;
        tmp_reg.reg6a = 0;
        UDMA_RIURead16(REG_BANK_MFE+0x6a, (MS_U16*)&tmp_reg.reg6a);
        printf("[SetObufAddr]\n\treg_mfe_s_obuf0_status = %d\n\treg_mfe_s_obuf1_status = %d\n\treg_mfe_s_bspobuf_idx=%d\n\tobufadr_update_cnt=%d\n"),
            tmp_reg.reg_mfe_s_obuf0_status,
            tmp_reg.reg_mfe_s_obuf1_status,
            tmp_reg.reg_mfe_s_bspobuf_idx,
            tmp_reg.obufadr_update_cnt);
      // Below is read back buffer address to check
      //#define CHECK_OBUF_ADDRESS
      #if defined(CHECK_OBUF_ADDRESS)
        UDMA_RIURead16(REG_BANK_MFE+0x3f, (MS_U16*)&tmp_reg.reg3f);
        tmp_reg.reg_mfe_s_bspobuf_adr_rchk_en = 1;
        for (MS_S32 i=0; i<4; i++) {
            tmp_reg.reg_mfe_s_bspobuf_adr_rchk_sel = i;
            UDMA_RIUWrite16(REG_BANK_MFE+0x3f, tmp_reg.reg3f);
            UDMA_RIURead16(REG_BANK_MFE+0x44, (MS_U16*)&tmp_reg.reg44);
            UDMA_RIURead16(REG_BANK_MFE+0x45, (MS_U16*)&tmp_reg.reg45);
            printf("\tObuf%d %d(s=0,e=1) = 0x%08x\n"),
                i>>1, i&1, ((tmp_reg.reg_mfe_s_bspobuf_wptr_high<<16)|tmp_reg.reg_mfe_s_bspobuf_wptr_low)<<3);
        }
        tmp_reg.reg_mfe_s_bspobuf_adr_rchk_en = 0;
        UDMA_RIUWrite16(REG_BANK_MFE+0x3f, tmp_reg.reg3f);
      #endif
    }
#endif
*/
    return nRegWriteCount;
}

#else
void SetObufAddr(MFE_REG* mfe_reg, MS_U16 sadr_low, MS_U16 sadr_high, MS_U16 eadr_low, MS_U16 eadr_high)
{
    mfe_reg->reg_mfe_s_bspobuf_sadr_low = sadr_low;
    mfe_reg->reg_mfe_s_bspobuf_sadr_high = sadr_high;
    mfe_reg->reg_mfe_s_bspobuf_eadr_low = eadr_low;
    mfe_reg->reg_mfe_s_bspobuf_eadr_high = eadr_high;

    WriteRegMFE(0x3c, mfe_reg->reg3c, (MS_S8*)(""), 0, (MS_S8*)(""));
    WriteRegMFE(0x3d, mfe_reg->reg3d, (MS_S8*)(""), 0, (MS_S8*)(""));
    WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)(""), 0, (MS_S8*)(""));
    WriteRegMFE(0x3f, mfe_reg->reg3f, (MS_S8*)(""), 0, (MS_S8*)(""));

    //printf("sadr_high = 0x%x, sadr_low = 0x%x, eadr_high = 0x%x, eadr_low = 0x%x\n"), sadr_high, sadr_low, eadr_high, eadr_low);
    MS_ASSERT(mfe_reg->reg_mfe_s_mvobuf_set_adr==0);
    mfe_reg->reg_mfe_s_bspobuf_set_adr = 1;
    WriteRegMFE(0x3b, mfe_reg->reg3b, (MS_S8*)(""), 0, (MS_S8*)(""));
    mfe_reg->reg_mfe_s_bspobuf_set_adr = 0;    // HW is write-one-clear
}
#endif

void ClearBsfFullIRQ(MFE_REG* mfe_reg)
{
    mfe_reg->reg_mfe_g_irq_clr1 = 1;
    WriteRegMFE(0x1d, mfe_reg->reg1d, (MS_S8*)(""), 0, (MS_S8*)(""));
    mfe_reg->reg_mfe_g_irq_clr1 = 0;    // HW is write-one-clear
}

void ClearIRQ(MFE_REG* mfe_reg,MS_S32 number)
{
    ReadRegMFE(0x1d, &mfe_reg->reg1d);
    switch (number) {
        case 0: mfe_reg->reg_mfe_g_irq_clr0 = 1; break;
        case 1: mfe_reg->reg_mfe_g_irq_clr1 = 1; break;
        case 2: mfe_reg->reg_mfe_g_irq_clr2 = 1; break;
        case 3: mfe_reg->reg_mfe_g_irq_clr3 = 1; break;
        case 4: mfe_reg->reg_mfe_g_irq_clr4 = 1; break;
        case 5: mfe_reg->reg_mfe_g_irq_clr5 = 1; break;
        case 6: mfe_reg->reg_mfe_g_irq_clr6 = 1; break;
        case 7: mfe_reg->reg_mfe_g_irq_clr7 = 1; break;
    }
    WriteRegMFE(0x1d, mfe_reg->reg1d, (MS_S8*)(""), 0, (MS_S8*)(""));
    switch (number) {    // HW is write-one-clear
    case 0: mfe_reg->reg_mfe_g_irq_clr0 = 0; break;
    case 1: mfe_reg->reg_mfe_g_irq_clr1 = 0; break;
    case 2: mfe_reg->reg_mfe_g_irq_clr2 = 0; break;
    case 3: mfe_reg->reg_mfe_g_irq_clr3 = 0; break;
    case 4: mfe_reg->reg_mfe_g_irq_clr4 = 0; break;
    case 5: mfe_reg->reg_mfe_g_irq_clr5 = 0; break;
    case 6: mfe_reg->reg_mfe_g_irq_clr6 = 0; break;
    case 7: mfe_reg->reg_mfe_g_irq_clr7 = 0; break;
    }
}


void Enable_HW(MFE_REG* mfe_reg)
{
    mfe_reg->reg_mfe_g_frame_start_sw = 1;
    //WriteRegMFE(0x00, mfe_reg->reg00, (MS_S8*)("[%d] reg00"), nRegWriteCount++, (MS_S8*)("frame start"));
    WriteRegMFE(0x00, mfe_reg->reg00, (MS_S8*)(""), 0, (MS_S8*)(""));
    mfe_reg->reg_mfe_g_frame_start_sw = 0;    // HW is write-one-clear
}

void ResetAllRegs(MFE_REG* mfe_reg)
{
    MS_S32 nRegWriteCount = 0;

    memset(mfe_reg, 0, sizeof(MFE_REG));    // Initial
    mfe_reg->reg_mfe_g_soft_rstz = 1;
    WriteRegMFE(0x0, mfe_reg->reg00, (MS_S8*)("[%d] reg00"), nRegWriteCount++, (MS_S8*)("SW reset 1"));

    WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("tbc_mode=0"));
    WriteRegMFE(0x4, mfe_reg->reg04, (MS_S8*)("[%d] reg04"), nRegWriteCount++, (MS_S8*)("er_bs mode threshold"));
    mfe_reg->reg_mfe_g_inter_pref = 0x200;
    WriteRegMFE(0x5, mfe_reg->reg05, (MS_S8*)("[%d] reg05"), nRegWriteCount++, (MS_S8*)("inter prediction preference"));

    WriteRegMFE(0x16, mfe_reg->reg16, (MS_S8*)("[%d] reg16"), nRegWriteCount++, (MS_S8*)("clock gating=0"));

    // [JPEG]
    mfe_reg->reg_mfe_g_jpe_qfactor = 0x3;
    mfe_reg->reg_mfe_g_viu_soft_rstz = 1;
    WriteRegMFE(0x18, mfe_reg->reg18, (MS_S8*)("[%d] reg18"), nRegWriteCount++, (MS_S8*)("JPE encode mode"));

    // [MPEG4/H263]
    WriteRegMFE(0x19, mfe_reg->reg19, (MS_S8*)("[%d] reg19"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x1a, mfe_reg->reg1a, (MS_S8*)("[%d] reg1a"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x1b, mfe_reg->reg1b, (MS_S8*)("[%d] reg1b"), nRegWriteCount++, (MS_S8*)("value"));

    // ME setting
    mfe_reg->reg_mfe_s_me_ref_en_mode = 0x3;
    WriteRegMFE(0x20, mfe_reg->reg20, (MS_S8*)("[%d] reg20"), nRegWriteCount++, (MS_S8*)("ME partition setting"));

    // [IME PIPELINE]
    WriteRegMFE(0x21, mfe_reg->reg21, (MS_S8*)("[%d] reg21"), nRegWriteCount++, (MS_S8*)("value"));
    mfe_reg->reg_mfe_s_ime_mesr_max_addr = 0x5d;
    WriteRegMFE(0x22, mfe_reg->reg22, (MS_S8*)("[%d] reg22"), nRegWriteCount++, (MS_S8*)("me search range max depth"));
    mfe_reg->reg_mfe_s_ime_mvx_max = 0x3e;
    WriteRegMFE(0x23, mfe_reg->reg23, (MS_S8*)("[%d] reg23"), nRegWriteCount++, (MS_S8*)("me mvx"));
    mfe_reg->reg_mfe_s_ime_mvy_max = 0x1f;
    WriteRegMFE(0x24, mfe_reg->reg24, (MS_S8*)("[%d] reg24"), nRegWriteCount++, (MS_S8*)("me mvy"));

    // [FME PIPELINE]
    mfe_reg->reg_mfe_s_fme_pipeline_on = 0x0;    // This is hw default value
    WriteRegMFE(0x25, mfe_reg->reg25, (MS_S8*)("[%d] reg25"), nRegWriteCount++, (MS_S8*)("FME"));

    // MBR
    WriteRegMFE(0x26, mfe_reg->reg26, (MS_S8*)("[%d] reg26"), nRegWriteCount++, (MS_S8*)("MBR: mbbits"));
    WriteRegMFE(0x27, mfe_reg->reg27, (MS_S8*)("[%d] reg27"), nRegWriteCount++, (MS_S8*)("MBR: frame qstep"));
    WriteRegMFE(0x29, mfe_reg->reg29, (MS_S8*)("[%d] reg29"), nRegWriteCount++, (MS_S8*)("264 qp-offset"));
    mfe_reg->reg_mfe_s_mbr_qp_min = 0x1;
    mfe_reg->reg_mfe_s_mbr_qp_max = 0x1d;
    WriteRegMFE(0x2a, mfe_reg->reg2a, (MS_S8*)("[%d] reg2a"), nRegWriteCount++, (MS_S8*)("QP min/max"));
    mfe_reg->reg_mfe_s_mbr_qstep_min = 0x1f;
    WriteRegMFE(0x6e, mfe_reg->reg6e, (MS_S8*)("[%d] reg6e"), nRegWriteCount++, (MS_S8*)("QStep min"));
    mfe_reg->reg_mfe_s_mbr_qstep_max = 0x3a0;
    WriteRegMFE(0x6f, mfe_reg->reg6f, (MS_S8*)("[%d] reg6f"), nRegWriteCount++, (MS_S8*)("QStep max"));

    // IEAP
    mfe_reg->reg_mfe_s_ieap_last_mode = 8;
    mfe_reg->reg_mfe_s_ieap_ccest_en = 1;
    mfe_reg->reg_mfe_s_ieap_ccest_thr = 3;
    WriteRegMFE(0x2b, mfe_reg->reg2b, (MS_S8*)("[%d] reg2b"), nRegWriteCount++, (MS_S8*)("ieap"));

    // QUAN
    WriteRegMFE(0x2c, mfe_reg->reg2c, (MS_S8*)("[%d] reg2c"), nRegWriteCount++, (MS_S8*)("Last zigzag"));

    // TXIP control & debug
    WriteRegMFE(0x2d, mfe_reg->reg2d, (MS_S8*)("[%d] reg2d"), nRegWriteCount, (MS_S8*)(""));
    WriteRegMFE(0x2e, mfe_reg->reg2e, (MS_S8*)("[%d] reg2e"), nRegWriteCount, (MS_S8*)(""));
    WriteRegMFE(0x2f, mfe_reg->reg2f, (MS_S8*)("[%d] reg2f"), nRegWriteCount, (MS_S8*)(""));
    WriteRegMFE(0x30, mfe_reg->reg30, (MS_S8*)("[%d] reg30"), nRegWriteCount, (MS_S8*)(""));
    WriteRegMFE(0x31, mfe_reg->reg31, (MS_S8*)("[%d] reg31"), nRegWriteCount, (MS_S8*)(""));
    mfe_reg->reg_mfe_s_txip_wait_mode = 1;
    WriteRegMFE(0x32, mfe_reg->reg32, (MS_S8*)("[%d] reg32"), nRegWriteCount, (MS_S8*)(""));
    WriteRegMFE(0x33, mfe_reg->reg33, (MS_S8*)("[%d] reg33"), nRegWriteCount, (MS_S8*)(""));
    WriteRegMFE(0x34, mfe_reg->reg34, (MS_S8*)("[%d] reg34"), nRegWriteCount, (MS_S8*)(""));

    // MDC
    WriteRegMFE(0x37, mfe_reg->reg37, (MS_S8*)("[%d] reg37"), nRegWriteCount++, (MS_S8*)("MPEG4 MDC"));
    WriteRegMFE(0x38, mfe_reg->reg38, (MS_S8*)("[%d] reg38"), nRegWriteCount++, (MS_S8*)("MPEG4: vop_time_increment"));
    WriteRegMFE(0x39, mfe_reg->reg39, (MS_S8*)("[%d] reg39"), nRegWriteCount++, (MS_S8*)("value"));
    mfe_reg->reg_mfe_s_mdc_h264_disable_dbf_idc = 2;
    WriteRegMFE(0x3a, mfe_reg->reg3a, (MS_S8*)("[%d] reg3a"), nRegWriteCount++, (MS_S8*)("value"));

    // FDC
    WriteRegMFE(0x46, mfe_reg->reg46, (MS_S8*)("[FDC %d] reg46"), nRegWriteCount++, (MS_S8*)("fdc bs"));
    WriteRegMFE(0x47, mfe_reg->reg47, (MS_S8*)("[FDC %d] reg47"), nRegWriteCount++, (MS_S8*)("fdc len"));
    WriteRegMFE(0x48, mfe_reg->reg48, (MS_S8*)("[FDC %d] reg48"), nRegWriteCount++, (MS_S8*)("fdc vld"));
    // [Table Control]
    WriteRegMFE(0x49, mfe_reg->reg49, (MS_S8*)("[Table %d] reg49"), nRegWriteCount++, (MS_S8*)("table address"));
    WriteRegMFE(0x4a, mfe_reg->reg4a, (MS_S8*)("[Table %d] reg4a"), nRegWriteCount++, (MS_S8*)("table write data"));

    // [Debug]
    WriteRegMFE(0x70, mfe_reg->reg70, (MS_S8*)("[%d] reg70"), nRegWriteCount++, (MS_S8*)(""));
    WriteRegMFE(0x71, mfe_reg->reg71, (MS_S8*)("[%d] reg71"), nRegWriteCount++, (MS_S8*)(""));
    WriteRegMFE(0x72, mfe_reg->reg72, (MS_S8*)("[%d] reg72"), nRegWriteCount++, (MS_S8*)(""));
    WriteRegMFE(0x73, mfe_reg->reg73, (MS_S8*)("[%d] reg73"), nRegWriteCount++, (MS_S8*)(""));
}

void TestStopAtMb(MFE_REG* mfe_reg)
{
    mfe_reg->reg_mfe_g_debug_trig_mbx = STOP_MBX;
    WriteRegMFE(0x71, mfe_reg->reg71, (MS_S8*)("[%d] reg71"), 0, (MS_S8*)("reg_mfe_g_debug_trig_mbx"));
    mfe_reg->reg_mfe_g_debug_trig_mby = STOP_MBY;
    WriteRegMFE(0x72, mfe_reg->reg72, (MS_S8*)("[%d] reg72"), 0, (MS_S8*)("reg_mfe_g_debug_trig_mby"));
    mfe_reg->reg_mfe_s_txip_sng_mb = 1;
    WriteRegMFE(0x2d, mfe_reg->reg2d, (MS_S8*)("[%d] reg2d"), 0, (MS_S8*)("reg_mfe_s_txip_sng_mb=1"));
    mfe_reg->reg_mfe_g_debug_trig_mode = 1;
    mfe_reg->reg_mfe_g_debug_en = 1;
    WriteRegMFE(0x73, mfe_reg->reg73, (MS_S8*)("[%d] reg73"), 0, (MS_S8*)("reg_mfe_g_debug_trig_mode=1"));
    mfe_reg->reg_mfe_g_debug_trig_cycle = 0;
    WriteRegMFE(0x70, mfe_reg->reg70, (MS_S8*)("[%d] reg70"), 0, (MS_S8*)("reg_mfe_g_debug_trig_cycle=0"));
}

void TestStop(MFE_REG* mfe_reg)
{
    mfe_reg->reg_mfe_s_txip_sng_set = 1;
    WriteRegMFE(0x2d, mfe_reg->reg2d, (MS_S8*)("[%d] reg2d"), 0, (MS_S8*)("reg_mfe_s_txip_sng_set=1"));
}


void WriteQTable(MFE_REG* mfe_reg, MS_S32* Table0, MS_S32* Table1)
{
    MS_S32 i, nTarWriteCount, nRegWriteCount=0;

    if (mfe_reg->reg_mfe_g_qmode==1) {    // Q table
        nTarWriteCount = 2 + ((64>>1)*3)*2;

        // Switch to sw mode
        mfe_reg->reg_mfe_g_tbc_mode = 0;
        WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg3"), nRegWriteCount++, (MS_S8*)("tbc_mode=0"));

        mfe_reg->reg48 = 0;
        for (i=0; i<64; i+=2) {
            mfe_reg->reg_mfe_s_tbc_wdata =Table0[((i&0x7)<<3)|(i>>3)]
                | (Table0[(((i+1)&0x7)<<3)|((i+1)>>3)]<<8);

            WriteRegMFE(0x4a, mfe_reg->reg4a, (MS_S8*)("[%d] reg4a"), nRegWriteCount++, (MS_S8*)("table write data"));
            mfe_reg->reg_mfe_s_tbc_rw = 1;
            mfe_reg->reg_mfe_s_tbc_en = 1;
            mfe_reg->reg_mfe_s_tbc_adr = i>>1;
            WriteRegMFE(0x49, mfe_reg->reg49, (MS_S8*)("[%d] reg49"), nRegWriteCount++, (MS_S8*)("table address"));
            WriteRegMFE(0x48, mfe_reg->reg48, (MS_S8*)("[%d] reg48"), nRegWriteCount++, (MS_S8*)("table write enable"));
            mfe_reg->reg_mfe_s_tbc_en = 0;    // HW is write-one-clear

        }
        for (i=0; i<64; i+=2) {
            mfe_reg->reg_mfe_s_tbc_wdata = Table1[((i&0x7)<<3)|(i>>3)]
                | (Table1[(((i+1)&0x7)<<3)|((i+1)>>3)]<<8);

            WriteRegMFE(0x4a, mfe_reg->reg4a, (MS_S8*)("[%d] reg4a"), nRegWriteCount++, (MS_S8*)("table write data (inter)"));
            mfe_reg->reg_mfe_s_tbc_rw = 1;
            mfe_reg->reg_mfe_s_tbc_en = 1;
            mfe_reg->reg_mfe_s_tbc_adr = (64+i)>>1;
            WriteRegMFE(0x49, mfe_reg->reg49, (MS_S8*)("[%d] reg49"), nRegWriteCount++, (MS_S8*)("table address"));
            WriteRegMFE(0x48, mfe_reg->reg48, (MS_S8*)("[%d] reg48"), nRegWriteCount++, (MS_S8*)("table write enable"));
            mfe_reg->reg_mfe_s_tbc_en = 0;    // HW is write-one-clear

        }

        // Switch to hw mode
        mfe_reg->reg_mfe_g_tbc_mode = 1;
        WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("tbc_mode=1"));
    } else
    {
        nTarWriteCount = 2;
        // Switch to sw mode
        mfe_reg->reg_mfe_g_tbc_mode = 0;
        WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("tbc_mode=0"));
        // Switch to hw mode
        mfe_reg->reg_mfe_g_tbc_mode = 1;
        WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("tbc_mode=1"));
    }
    MS_ASSERT(nRegWriteCount==nTarWriteCount);

}

// Mode 0: Actual FDC putting.
// Mode 1: Get number of FDC data round (each round is 16bit).

MS_S32 PutFDC(MFE_REG* mfe_reg, void* pContext, MS_S32 mode)
{
    MS_S32 len, bit_len;
    MS_U32 val=0;
    MS_U8* ptr;
    MS_U8 bits;
    MS_S32 nRegWriteNum = 0;
    MS_S32 nTotalRound = 0;


    BitsInfo* pBitsInfo = (BitsInfo*)pContext;
    ptr = pBitsInfo->ptr;
    len = pBitsInfo->len;
    bits = pBitsInfo->bits;
    bit_len = pBitsInfo->bit_len;
    bit_len += len*8;

    // Calculate total round.
    if (bit_len==0)
        nTotalRound = 1;
    else
        nTotalRound = (bit_len+15)>>4;
    if (mode==1)
        return nTotalRound;

    ms_dprintk(DRV_L3, "FDC: %d bits\n", (int)bit_len);
    mfe_reg->reg46 = 0;
    mfe_reg->reg47 = 0;
    mfe_reg->reg48 = 0;
    // Fill the total round reg
    mfe_reg->reg_mfe_s_fdc_bs_count = nTotalRound-1;    // Count from 0
    WriteRegMFE(0x47, mfe_reg->reg47, (MS_S8*)("[FDC %d] reg47"), nRegWriteNum++, (MS_S8*)("fdc round count"));

    while (bit_len>16) {
        val = (ptr[0]<<8) | ptr[1];
        ptr += 2;
        bit_len -= 16;
        len -= 2;

        mfe_reg->reg_mfe_s_fdc_bs = val;
        mfe_reg->reg_mfe_s_fdc_bs_len = 15;
        mfe_reg->reg_mfe_s_fdc_bs_vld = 1;
        WriteRegMFE(0x46, mfe_reg->reg46, (MS_S8*)("[FDC %d] reg46"), nRegWriteNum++, (MS_S8*)("fdc bs"));
        WriteRegMFE(0x47, mfe_reg->reg47, (MS_S8*)("[FDC %d] reg47"), nRegWriteNum++, (MS_S8*)("fdc len"));
        WriteRegMFE(0x48, mfe_reg->reg48, (MS_S8*)("[FDC %d] reg48"), nRegWriteNum++, (MS_S8*)("fdc vld"));
        mfe_reg->reg_mfe_s_fdc_bs_vld = 0;    // HW is write-one-clear
    }
    if (len==0) {
        val = bits<<8;
    } else if (len==1) {
        val = (ptr[0]<<8) | bits;
    } else if (len==2) {
        val = (ptr[0]<<8) | ptr[1];
    }

    mfe_reg->reg_mfe_s_fdc_bs = val;
    mfe_reg->reg_mfe_s_fdc_bs_len = bit_len;
    mfe_reg->reg_mfe_s_fdc_bs_vld = 1;
    WriteRegMFE(0x46, mfe_reg->reg46, (MS_S8*)("[FDC %d] reg46"), nRegWriteNum++, (MS_S8*)("fdc bs"));
    WriteRegMFE(0x47, mfe_reg->reg47, (MS_S8*)("[FDC %d] reg47"), nRegWriteNum++, (MS_S8*)("fdc len"));
    WriteRegMFE(0x48, mfe_reg->reg48, (MS_S8*)("[FDC %d] reg48"), nRegWriteNum++, (MS_S8*)("fdc vld"));
    mfe_reg->reg_mfe_s_fdc_bs_vld = 0;    // HW is write-one-clear

    return nRegWriteNum;
}

//////////////////////////////////////////////////////////////////////////
// Intra Update
//        These are for settings and buffer allocation.
//        The actual algorithm is implemented in IntraUpdate.c
//////////////////////////////////////////////////////////////////////////

void IntraUpdateInit(MFE_CONFIG *pConfig)
{
    IntraUpdateContext *ctx = &pConfig->m_IUContext;

    ctx->nWidth = pConfig->nBufWidth;
    MS_ASSERT(ctx->nWidth%16==0);
    ctx->nHeight = pConfig->nBufHeight;
    MS_ASSERT(ctx->nHeight%16==0);
    ctx->nTotalMb = (ctx->nWidth*ctx->nHeight)>>8;
    ctx->pHwMbMap = MfeDrvMemMalloc(sizeof(HW_MB_MAP)*ctx->nTotalMb, (const MS_S8*)("pHwMbMap"));//pConfig->m_UIHwMap;
    ctx->pSwMbMap = MfeDrvMemMalloc(sizeof(SW_MB_MAP)*ctx->nTotalMb, (const MS_S8*)("pSwMbMap"));//pConfig->m_UISwMap;

    if (pConfig->g_intraupdate_enable) {
        ctx->bCIR = 1;
        ctx->CIR_COUNT = (ctx->nTotalMb/pConfig->g_intraupdate_period)+1;
    }
    else {
        ctx->bCIR = 0;
    }

    ctx->bFIR = 0;
    ctx->FIR_THR = 132;

    ctx->bAIR = 0;
    ctx->AIR_COUNT = 10;
    ctx->AIR_ROUND = 1;

    IntraUpdate_Init(ctx);
}

void IntraUpdateFrame(MFE_CONFIG *pConfig)
{
    IntraUpdate_Frame(&pConfig->m_IUContext);
}

void IntraUpdateClose(MFE_CONFIG *pConfig)
{
    IntraUpdateContext *ctx = &pConfig->m_IUContext;

    if (ctx->pHwMbMap) {
        MfeDrvMemFree((void**)&ctx->pHwMbMap, (const MS_S8*)("pHwMbMap"));
    }
    if (ctx->pSwMbMap) {
        MfeDrvMemFree((void**)&ctx->pSwMbMap, (const MS_S8*)("pSwMbMap"));
    }
}

//////////////////////////////////////////////////////////////////////////
// Rate Control
//////////////////////////////////////////////////////////////////////////


static void _sw263v_ReinitRateCtrl(RateCtrl_t* rcctx,MFE_CONFIG* pConfig)
{
    H263RCInfo rcInfo;
    rcInfo.nWidth = pConfig->nBufWidth;
    rcInfo.nHeight= pConfig->nBufHeight;
    rcInfo.fTargetFrameRate = (MS_FLOAT) (pConfig->FrameRatex100 / 100);
    rcInfo.fMinFrameRate = 5;
    rcInfo.m_nBitrate = pConfig->nBitrate;
    rcInfo.nPCount= pConfig->nPbetweenI;
    rcInfo.rcGranularity = FRAMELEVELRC;
    rcInfo.rcMethod = STRICT_CONST_BITRATE;
    rc_Init(rcctx,&rcInfo);

}

void MfeDrvRateControlInit(MFE_CONFIG* pConfig)
{

    if(pConfig->VTMode){
        rc_Create(&pConfig->VTRateCtrl);
        _sw263v_ReinitRateCtrl(&pConfig->VTRateCtrl,pConfig);
        return;
    }

    pConfig->rcInfo.nCodecType = pConfig->nCodecType;
    pConfig->rcInfo.nWidth = pConfig->nBufWidth;
    pConfig->rcInfo.nHeight = pConfig->nBufHeight;

//beacuse LG GP3 env no support float div
//this version only support 29.97fps and integer fps
    if(pConfig->FrameRatex100==2997){
        pConfig->rcInfo.fTargetFrameRate_is_float = 1;
        pConfig->rcInfo.fTargetFrameRate = 29.97f;
    }
    else{
        pConfig->rcInfo.fTargetFrameRate_is_float = 0;
        pConfig->rcInfo.fTargetFrameRate = 0;
        pConfig->rcInfo.int_fTargetFrameRate = pConfig->FrameRatex100/100;
       //pConfig->rcInfo.fTargetFrameRate = (MS_FLOAT)(pConfig->FrameRatex100/100);
    }

    pConfig->rcInfo.nBitrate = pConfig->nBitrate;
    pConfig->rcInfo.nMaxBitrate = 0;   // Let RC decide
    pConfig->rcInfo.nVPSize = pConfig->nVPSizeInBits > 0 ? pConfig->nVPSizeInBits : 0;    // 0 means not used.
    pConfig->rcInfo.nVPMbRow = pConfig->nVPSizeInMbRow;    // 0 means not used.
    pConfig->rcInfo.bFixedFrameRate = 1;
    pConfig->rcInfo.nPCount = pConfig->nPbetweenI;    // Number of P-frames between I-frames
    pConfig->rcInfo.nBCount = pConfig->nBbetweenP;    // Number of B-frames between P-frames
    pConfig->rcInfo.rcMethod = CONSTRAINED_VARIABLE_BITRATE;
    pConfig->rcInfo.rcGranularity = MBLEVELRC;//FRAMELEVELRC;
    if (pConfig->rcInfo.nBitrate <=0) {
        pConfig->rcInfo.rcGranularity = FRAMELEVELRC;
        pConfig->rcInfo.rcMethod = CONST_QUALITY;
        if (pConfig->rcInfo.nBitrate<0) {
            pConfig->rcInfo.nConstQP = -pConfig->rcInfo.nBitrate;
            if (pConfig->rcInfo.nConstQP>29)
                pConfig->rcInfo.nConstQP = 29;
        }
        else
            pConfig->rcInfo.nConstQP = 8;
    }
    ms_dprintk(DRV_L1,"RC method: %d\n", pConfig->rcInfo.rcMethod);
#if 0 // Test const qp
    pConfig->rcInfo.rcMethod = CONST_QUALITY;
    pConfig->rcInfo.rcGranularity = FRAMELEVELRC;
    pConfig->rcInfo.nConstQP = 12;
#endif
    cvbr_InitRateControl(&pConfig->ctxRateControl, &pConfig->rcInfo);
    pConfig->m_cvbrFrameSkip = 0;
    pConfig->m_bGenSkipVopHeader = 1;
}

void MfeDrvRateControlUpdate(MFE_CONFIG* pConfig, MS_S8 nFieldType)
{
    MS_S32 nLastFrmAvgQStep, nLastFrmBits;
    CVBRRateControl* pRC = &pConfig->ctxRateControl;
    RateControl_REG reg;
    // Rate control
    // Read reg_mfe_s_mbr_last_frm_avg_qp and assign to rate control context.
    reg.reg28 = 0;
    reg.reg29 = 0;
    ReadRegMFE(0x28, &reg.reg28);
    ReadRegMFE(0x29, &reg.reg29);
    nLastFrmAvgQStep = ((MS_S32)reg.reg_mfe_s_mbr_last_frm_avg_qp_high << 16) + reg.reg_mfe_s_mbr_last_frm_avg_qp_low;
    reg.reg42 = 0;
    reg.reg43 = 0;
    ReadRegMFE(0x42, &reg.reg42);
    ReadRegMFE(0x43, &reg.reg43);
    nLastFrmBits = ((MS_S32)reg.reg_mfe_s_bsp_bit_cnt_high<<16) + reg.reg_mfe_s_bsp_bit_cnt_low;

    if(pConfig->VTMode){
        if(rc_CheckSkippedFrame(&pConfig->VTRateCtrl))
            rc_UpdateFrame(&pConfig->VTRateCtrl, 0,rcQstep2QP(pRC,nLastFrmAvgQStep/pConfig->VTRateCtrl.m_nNFrame),TRUE);
        else
            rc_UpdateFrame(&pConfig->VTRateCtrl, nLastFrmBits,rcQstep2QP(pRC,nLastFrmAvgQStep/pConfig->VTRateCtrl.m_nNFrame),FALSE);
        return;
    }
    if (nFieldType==0 || nFieldType==1) {
        pRC->m_nLastFrameAvgQStep[0] = nLastFrmAvgQStep;
        pRC->m_nLastFrameAvgQStep[1] = 0;
    }
    else {    // nFieldType=2
        pRC->m_nLastFrameAvgQStep[1] = nLastFrmAvgQStep;
    }
    // Read
    if (pConfig->m_cvbrFrameSkip>0 && pConfig->vopPredType==P_VOP) {
        nLastFrmBits = pConfig->m_OutStream.m_nByteCount*8;
        cvbr_UpdateFrame(pRC, nLastFrmBits, 1, nFieldType);
        pConfig->m_cvbrFrameSkip--;
    }
    else {
        pConfig->m_cvbrFrameSkip = cvbr_UpdateFrame(pRC, nLastFrmBits, 0, nFieldType);
    }

}

void MfeDrvRateControlDeInit(MFE_CONFIG* pConfig)
{
    if(pConfig->VTMode){
        rc_Finish(&pConfig->VTRateCtrl);
        rc_Destroy(&pConfig->VTRateCtrl);
    }
    else {
        cvbr_CloseRateControl(&pConfig->ctxRateControl);
    }
}

//report cabac_bit_count and bin count
MS_S32 MfeDrvCabacStuffingReport(MFE_CONFIG* pConfig)
{
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    MS_U16 tmp_low = 0;
    MS_U16 tmp_high = 0;
    MS_U32 bytes_in_picture;
    MS_U32 bin_count,bit_count;

    MS_U32 min_num_bytes=0;
    MS_S32 stuffing_bytes=0;
    MS_S32 RawMbBits;
    MS_S32 PicSizeInMbs = pConfig->nBufWidth*pConfig->nBufHeight/16/16;
    //load reg_mfe_s_cabac_bin_count
    ReadRegMFE_BANK1(0x30,&tmp_low);
    ReadRegMFE_BANK1(0x31,&tmp_high);
    bin_count = (((MS_U32)tmp_high)<<16) | ((MS_U32)tmp_low);

    //load reg_mfe_s_cabac_bit_count
    ReadRegMFE_BANK1(0x32,&tmp_low);
    ReadRegMFE_BANK1(0x33,&tmp_high);
    bit_count = (((MS_U32)tmp_high)<<16) | ((MS_U32)tmp_low);

    bytes_in_picture = (CEILING_ALIGN(bit_count,0x8) / 8);

    RawMbBits = 256 * /*img->bitdepth_luma*/8 + 2 * /*MbWidthC[active_sps->chroma_format_idc]*/8 * /*MbHeightC[active_sps->chroma_format_idc]*/8 * /*img->bitdepth_chroma*/8;
    min_num_bytes = ((96 * bin_count) - (RawMbBits * PicSizeInMbs *3) + 1023) / 1024;
    if ((MS_U32)min_num_bytes > bytes_in_picture)
    {
        stuffing_bytes = min_num_bytes - bytes_in_picture;
        stuffing_bytes = 3*((stuffing_bytes+2)/3);
        ms_dprintk(DRV_L3, "CABAC stuffing bytes = %6d (according to Clause 7.4.2.10)\n", (int)stuffing_bytes);
    }

    return stuffing_bytes;
#else
    ms_dprintk(DRV_L4,"Not Support CABAC in this CHIP\n");
    return 0;
#endif
}


void* MfeDrvMemMalloc(MS_SIZE size,const MS_S8* msg)
{
    void* ret_ptr = malloc(size);
    ms_dprintk(DRV_L2, "[MFE MALLOC][%s] PTR: %p, SIZE: 0x%x\n", msg, ret_ptr, (unsigned int)size);
    return ret_ptr;
}


void MfeDrvMemFree(void** mem_ptr, const MS_S8* msg)
{
    if (mem_ptr && *mem_ptr)
    {
        ms_dprintk(DRV_L2,"[MFE FREE][%s] PTR: %p\n", msg, mem_ptr);
        free(*mem_ptr);
        *mem_ptr = NULL;
    }
}
