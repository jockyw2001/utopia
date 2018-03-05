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
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"

#include "mdrv_mfe_math.h"
#include "mhal_mfe.h"

#ifdef _FPGA_
    #include "fpga_def.h"
#endif
#if defined(WIN32)
    #include "sigdump/sigdump_api.h"
#endif

#if defined(CHECK_FDC_DONE)&&defined(_MFE_UTOPIA_)
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"
#endif

#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&!defined(_KERNEL_MODE_)
extern MFE_REG mfe_reg;
extern MFE_U32 u32MFERegOSBase;
#endif
void WriteRegMFE(MFE_U32 u32Address, MFE_U16 val, char *str1, int num, char *str3)
{
#if defined(WIN32)
    sd_output_reg(sd_sw_cfg1, u32Address, val, str1, num, str3);
#endif

#if defined(_FPGA_) || defined(_UDMA_) || defined(_HIF_)
    FPGA_RIUWrite16((U16)(REG_BANK_MFE+u32Address), val);
#else
    //temp = __M4VE_REG(u32Address);
    //ms_dprintk(DRV_L2, "%04X %04X\n", (unsigned int)u32Address, (unsigned int)val);
    __MFE_REG(u32Address) = val;
#endif
}

// void WriteRegMFE_Bank1(U32 u32Address, U16 val, char *str1, int num, char *str3)
// {
//     sd_output_reg(sd_sw_cfg3, u32Address, val, str1, num, str3);
//     UDMA_RIUWrite16(REG_BANK_MFE1+u32Address, val);
// }

void ReadRegMFE(MFE_U32 u32Address, MFE_U16 *val)
{
#if defined(_FPGA_) || defined(_UDMA_) || defined(_HIF_)
    FPGA_RIURead16((MFE_U16)(REG_BANK_MFE+u32Address), val);
#else
    *val = __MFE_REG(u32Address);
#endif
}

void DumpAllReg()
{
#if (DRV_L5==0)
    return;
#else
    int i;
    unsigned short tmp = 0;
    for (i = 0; i < 0x80; i++) {
        ReadRegMFE(i, &tmp);
        ms_dprintk(DRV_L5,"%02X=%04X ", i, tmp);
        if (((i+1)%8)==0)
            ms_dprintk(DRV_L5, " \n\r");
    }
#endif
}

void SetObufAddr(MFE_U16 sadr_low, MFE_U16 sadr_high, MFE_U16 eadr_low, MFE_U16 eadr_high)
{
    mfe_reg.reg_mfe_s_bspobuf_sadr_low = sadr_low;
    mfe_reg.reg_mfe_s_bspobuf_sadr_high = sadr_high;
    mfe_reg.reg_mfe_s_bspobuf_eadr_low = eadr_low;
    mfe_reg.reg_mfe_s_bspobuf_eadr_high = eadr_high;

    WriteRegMFE(0x3c, mfe_reg.reg3c, "", 0, "");
    WriteRegMFE(0x3d, mfe_reg.reg3d, "", 0, "");
    WriteRegMFE(0x3e, mfe_reg.reg3e, "", 0, "");
    WriteRegMFE(0x3f, mfe_reg.reg3f, "", 0, "");

    //printf("sadr_high = 0x%x, sadr_low = 0x%x, eadr_high = 0x%x, eadr_low = 0x%x\n", sadr_high, sadr_low, eadr_high, eadr_low);
    MFE_ASSERT(mfe_reg.reg_mfe_s_mvobuf_set_adr==0);
    mfe_reg.reg_mfe_s_bspobuf_set_adr = 1;
    WriteRegMFE(0x3b, mfe_reg.reg3b, "", 0, "");
    mfe_reg.reg_mfe_s_bspobuf_set_adr = 0;    // HW is write-one-clear
}

void ClearBsfFullIRQ()
{
    mfe_reg.reg_mfe_g_irq_clr1 = 1;
    WriteRegMFE(0x1d, mfe_reg.reg1d, "", 0, "");
    mfe_reg.reg_mfe_g_irq_clr1 = 0;    // HW is write-one-clear
}

void ClearIRQ(int number)
{
    ReadRegMFE(0x1d, &mfe_reg.reg1d);
    switch (number) {
        case 0: mfe_reg.reg_mfe_g_irq_clr0 = 1; break;
        case 1: mfe_reg.reg_mfe_g_irq_clr1 = 1; break;
        case 2: mfe_reg.reg_mfe_g_irq_clr2 = 1; break;
        case 3: mfe_reg.reg_mfe_g_irq_clr3 = 1; break;
        case 4: mfe_reg.reg_mfe_g_irq_clr4 = 1; break;
        case 5: mfe_reg.reg_mfe_g_irq_clr5 = 1; break;
        case 6: mfe_reg.reg_mfe_g_irq_clr6 = 1; break;
        case 7: mfe_reg.reg_mfe_g_irq_clr7 = 1; break;
    }
    WriteRegMFE(0x1d, mfe_reg.reg1d, "", 0, "");
    switch (number) {    // HW is write-one-clear
    case 0: mfe_reg.reg_mfe_g_irq_clr0 = 0; break;
    case 1: mfe_reg.reg_mfe_g_irq_clr1 = 0; break;
    case 2: mfe_reg.reg_mfe_g_irq_clr2 = 0; break;
    case 3: mfe_reg.reg_mfe_g_irq_clr3 = 0; break;
    case 4: mfe_reg.reg_mfe_g_irq_clr4 = 0; break;
    case 5: mfe_reg.reg_mfe_g_irq_clr5 = 0; break;
    case 6: mfe_reg.reg_mfe_g_irq_clr6 = 0; break;
    case 7: mfe_reg.reg_mfe_g_irq_clr7 = 0; break;
    }
}


void Enable_HW()
{
    mfe_reg.reg_mfe_g_frame_start_sw = 1;
    //WriteRegMFE(0x00, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "frame start");
    WriteRegMFE(0x00, mfe_reg.reg00, "", 0, "");
    mfe_reg.reg_mfe_g_frame_start_sw = 0;    // HW is write-one-clear
}

void ResetAllRegs()
{
    int nRegWriteCount = 0;

    memset(&mfe_reg, 0, sizeof(MFE_REG));    // Initial
    mfe_reg.reg_mfe_g_soft_rstz = 1;
    WriteRegMFE(0x0, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "SW reset 1");

    WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "tbc_mode=0");
    WriteRegMFE(0x4, mfe_reg.reg04, "[%d] reg04", nRegWriteCount++, "er_bs mode threshold");
    mfe_reg.reg_mfe_g_inter_pref = 0x200;
    WriteRegMFE(0x5, mfe_reg.reg05, "[%d] reg05", nRegWriteCount++, "inter prediction preference");

    WriteRegMFE(0x16, mfe_reg.reg16, "[%d] reg16", nRegWriteCount++, "clock gating=0");

    // [JPEG]
    mfe_reg.reg_mfe_g_jpe_qfactor = 0x3;
    mfe_reg.reg_mfe_g_viu_soft_rstz = 1;
    WriteRegMFE(0x18, mfe_reg.reg18, "[%d] reg18", nRegWriteCount++, "JPE encode mode");

    // [MPEG4/H263]
    WriteRegMFE(0x19, mfe_reg.reg19, "[%d] reg19", nRegWriteCount++, "value");
    WriteRegMFE(0x1a, mfe_reg.reg1a, "[%d] reg1a", nRegWriteCount++, "value");
    WriteRegMFE(0x1b, mfe_reg.reg1b, "[%d] reg1b", nRegWriteCount++, "value");

    // ME setting
    mfe_reg.reg_mfe_s_me_ref_en_mode = 0x3;
    WriteRegMFE(0x20, mfe_reg.reg20, "[%d] reg20", nRegWriteCount++, "ME partition setting");

    // [IME PIPELINE]
    WriteRegMFE(0x21, mfe_reg.reg21, "[%d] reg21", nRegWriteCount++, "value");
    mfe_reg.reg_mfe_s_ime_mesr_max_addr = 0x5d;
    WriteRegMFE(0x22, mfe_reg.reg22, "[%d] reg22", nRegWriteCount++, "me search range max depth");
    mfe_reg.reg_mfe_s_ime_mvx_max = 0x3e;
    WriteRegMFE(0x23, mfe_reg.reg23, "[%d] reg23", nRegWriteCount++, "me mvx");
    mfe_reg.reg_mfe_s_ime_mvy_max = 0x1f;
    WriteRegMFE(0x24, mfe_reg.reg24, "[%d] reg24", nRegWriteCount++, "me mvy");

    // [FME PIPELINE]
    mfe_reg.reg_mfe_s_fme_pipeline_on = 0x0;    // This is hw default value
    WriteRegMFE(0x25, mfe_reg.reg25, "[%d] reg25", nRegWriteCount++, "FME");

    // MBR
    WriteRegMFE(0x26, mfe_reg.reg26, "[%d] reg26", nRegWriteCount++, "MBR: mbbits");
    WriteRegMFE(0x27, mfe_reg.reg27, "[%d] reg27", nRegWriteCount++, "MBR: frame qstep");
    WriteRegMFE(0x29, mfe_reg.reg29, "[%d] reg29", nRegWriteCount++, "264 qp-offset");
    mfe_reg.reg_mfe_s_mbr_qp_min = 0x1;
    mfe_reg.reg_mfe_s_mbr_qp_max = 0x1d;
    WriteRegMFE(0x2a, mfe_reg.reg2a, "[%d] reg2a", nRegWriteCount++, "QP min/max");
    mfe_reg.reg_mfe_s_mbr_qstep_min = 0x1f;
    WriteRegMFE(0x6e, mfe_reg.reg6e, "[%d] reg6e", nRegWriteCount++, "QStep min");
    mfe_reg.reg_mfe_s_mbr_qstep_max = 0x3a0;
    WriteRegMFE(0x6f, mfe_reg.reg6f, "[%d] reg6f", nRegWriteCount++, "QStep max");

    // IEAP
    mfe_reg.reg_mfe_s_ieap_last_mode = 8;
    mfe_reg.reg_mfe_s_ieap_ccest_en = 1;
    mfe_reg.reg_mfe_s_ieap_ccest_thr = 3;
    WriteRegMFE(0x2b, mfe_reg.reg2b, "[%d] reg2b", nRegWriteCount++, "ieap");

    // QUAN
    WriteRegMFE(0x2c, mfe_reg.reg2c, "[%d] reg2c", nRegWriteCount++, "Last zigzag");

    // TXIP control & debug
    WriteRegMFE(0x2d, mfe_reg.reg2d, "[%d] reg2d", nRegWriteCount, "");
    WriteRegMFE(0x2e, mfe_reg.reg2e, "[%d] reg2e", nRegWriteCount, "");
    WriteRegMFE(0x2f, mfe_reg.reg2f, "[%d] reg2f", nRegWriteCount, "");
    WriteRegMFE(0x30, mfe_reg.reg30, "[%d] reg30", nRegWriteCount, "");
    WriteRegMFE(0x31, mfe_reg.reg31, "[%d] reg31", nRegWriteCount, "");
    mfe_reg.reg_mfe_s_txip_wait_mode = 1;
    WriteRegMFE(0x32, mfe_reg.reg32, "[%d] reg32", nRegWriteCount, "");
    WriteRegMFE(0x33, mfe_reg.reg33, "[%d] reg33", nRegWriteCount, "");
    WriteRegMFE(0x34, mfe_reg.reg34, "[%d] reg34", nRegWriteCount, "");

    // MDC
    WriteRegMFE(0x37, mfe_reg.reg37, "[%d] reg37", nRegWriteCount++, "MPEG4 MDC");
    WriteRegMFE(0x38, mfe_reg.reg38, "[%d] reg38", nRegWriteCount++, "MPEG4: vop_time_increment");
    WriteRegMFE(0x39, mfe_reg.reg39, "[%d] reg39", nRegWriteCount++, "value");
    mfe_reg.reg_mfe_s_mdc_h264_disable_dbf_idc = 2;
    WriteRegMFE(0x3a, mfe_reg.reg3a, "[%d] reg3a", nRegWriteCount++, "value");

    // FDC
    WriteRegMFE(0x46, mfe_reg.reg46, "[FDC %d] reg46", nRegWriteCount++, "fdc bs");
    WriteRegMFE(0x47, mfe_reg.reg47, "[FDC %d] reg47", nRegWriteCount++, "fdc len");
    WriteRegMFE(0x48, mfe_reg.reg48, "[FDC %d] reg48", nRegWriteCount++, "fdc vld");
    // [Table Control]
    WriteRegMFE(0x49, mfe_reg.reg49, "[Table %d] reg49", nRegWriteCount++, "table address");
    WriteRegMFE(0x4a, mfe_reg.reg4a, "[Table %d] reg4a", nRegWriteCount++, "table write data");

    // [Debug]
    WriteRegMFE(0x70, mfe_reg.reg70, "[%d] reg70", nRegWriteCount++, "");
    WriteRegMFE(0x71, mfe_reg.reg71, "[%d] reg71", nRegWriteCount++, "");
    WriteRegMFE(0x72, mfe_reg.reg72, "[%d] reg72", nRegWriteCount++, "");
    WriteRegMFE(0x73, mfe_reg.reg73, "[%d] reg73", nRegWriteCount++, "");
}

void TestStopAtMb()
{
    mfe_reg.reg_mfe_g_debug_trig_mbx = STOP_MBX;
    WriteRegMFE(0x71, mfe_reg.reg71, "[%d] reg71", 0, "reg_mfe_g_debug_trig_mbx");
    mfe_reg.reg_mfe_g_debug_trig_mby = STOP_MBY;
    WriteRegMFE(0x72, mfe_reg.reg72, "[%d] reg72", 0, "reg_mfe_g_debug_trig_mby");
    mfe_reg.reg_mfe_s_txip_sng_mb = 1;
    WriteRegMFE(0x2d, mfe_reg.reg2d, "[%d] reg2d", 0, "reg_mfe_s_txip_sng_mb=1");
    mfe_reg.reg_mfe_g_debug_trig_mode = 1;
    mfe_reg.reg_mfe_g_debug_en = 1;
    WriteRegMFE(0x73, mfe_reg.reg73, "[%d] reg73", 0, "reg_mfe_g_debug_trig_mode=1");
    mfe_reg.reg_mfe_g_debug_trig_cycle = 0;
    WriteRegMFE(0x70, mfe_reg.reg70, "[%d] reg70", 0, "reg_mfe_g_debug_trig_cycle=0");
}

void TestStop()
{
    mfe_reg.reg_mfe_s_txip_sng_set = 1;
    WriteRegMFE(0x2d, mfe_reg.reg2d, "[%d] reg2d", 0, "reg_mfe_s_txip_sng_set=1");
}


void WriteQTable(int* Table0, int* Table1)
{
    int i, nTarWriteCount, nRegWriteCount=0;

#ifdef CHECK_WriteQTable_DONE
        RegWQTDone RegWQT;
#endif

#ifdef QTABLE_READBACK_CHECK
        RegWQTDone RegWQT_RB;    //used for set status (Reg49)
        RegQT_RB_CHECK RegQT_RB;    //used for read table(Reg4b)
        RegQT_RB_CHECK_REG03 RegQT_RB_REG03; //used for switch to sw/hw mode
        unsigned int ErrorCount = 0;
#endif

    if (mfe_reg.reg_mfe_g_qmode==1) {    // Q table
        nTarWriteCount = 2 + ((64>>1)*3)*2;
#ifdef WIN32
        sd_output(sd_sw_cfg1, nTarWriteCount, 10, "NO_OF_REG_WRITE");
#endif
        // Switch to sw mode
        mfe_reg.reg_mfe_g_tbc_mode = 0;
        WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg3", nRegWriteCount++, "tbc_mode=0");

        mfe_reg.reg48 = 0;
        for (i=0; i<64; i+=2) {
            mfe_reg.reg_mfe_s_tbc_wdata =Table0[((i&0x7)<<3)|(i>>3)]
                | (Table0[(((i+1)&0x7)<<3)|((i+1)>>3)]<<8);

            WriteRegMFE(0x4a, mfe_reg.reg4a, "[%d] reg4a", nRegWriteCount++, "table write data");
            mfe_reg.reg_mfe_s_tbc_rw = 1;
            mfe_reg.reg_mfe_s_tbc_en = 1;
            mfe_reg.reg_mfe_s_tbc_adr = i>>1;
            WriteRegMFE(0x49, mfe_reg.reg49, "[%d] reg49", nRegWriteCount++, "table address");
            WriteRegMFE(0x48, mfe_reg.reg48, "[%d] reg48", nRegWriteCount++, "table write enable");

//Check Table0
#ifdef CHECK_WriteQTable_DONE
            ms_dprintk(DRV_L4, "[TABLE0] CHECK_WriteQTable_DONE\n");
            RegWQT.reg49 = 0;
            // Verify hw receives the bits
            while (1) {
                ReadRegMFE(0x49, &RegWQT.reg49);
                if (RegWQT.reg_mfe_s_tbc_done==1)
                    break;
            }

            // Reset hw status
            RegWQT.reg_mfe_s_tbc_done_clr = 1;
            WriteRegMFE(0x49, RegWQT.reg49,"",0,"");
            RegWQT.reg_mfe_s_tbc_done_clr = 0;    // HW is write-one-clear
            while (1) {
                ReadRegMFE(0x49, &RegWQT.reg49);
                if (RegWQT.reg_mfe_s_tbc_done==0)
                    break;
            }

#endif
        }

        for (i=0; i<64; i+=2) {
            mfe_reg.reg_mfe_s_tbc_wdata = Table1[((i&0x7)<<3)|(i>>3)]
                | (Table1[(((i+1)&0x7)<<3)|((i+1)>>3)]<<8);

            WriteRegMFE(0x4a, mfe_reg.reg4a, "[%d] reg4a", nRegWriteCount++, "table write data (inter)");
            mfe_reg.reg_mfe_s_tbc_rw = 1;
            mfe_reg.reg_mfe_s_tbc_en = 1;
            mfe_reg.reg_mfe_s_tbc_adr = (64+i)>>1;
            WriteRegMFE(0x49, mfe_reg.reg49, "[%d] reg49", nRegWriteCount++, "table address");
            WriteRegMFE(0x48, mfe_reg.reg48, "[%d] reg48", nRegWriteCount++, "table write enable");
            mfe_reg.reg_mfe_s_tbc_en = 0;    // HW is write-one-clear

//Check Table1
#ifdef CHECK_WriteQTable_DONE
            ms_dprintk(DRV_L4, "[TABLE1] CHECK_WriteQTable_DONE\n");
            RegWQT.reg49 = 0;
            // Verify hw receives the bits
            while (1) {
                ReadRegMFE(0x49, &RegWQT.reg49);
                if (RegWQT.reg_mfe_s_tbc_done==1)
                    break;
            }

            // Reset hw status
            RegWQT.reg_mfe_s_tbc_done_clr = 1;
            WriteRegMFE(0x49, RegWQT.reg49,"",0,"");
            RegWQT.reg_mfe_s_tbc_done_clr = 0;    // HW is write-one-clear
            while (1) {
                ReadRegMFE(0x49, &RegWQT.reg49);
                if (RegWQT.reg_mfe_s_tbc_done==0)
                    break;
            }
#endif
        }

        // Switch to hw mode
        mfe_reg.reg_mfe_g_tbc_mode = 1;
        WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "tbc_mode=1");
    } else
    {
        nTarWriteCount = 2;
#ifdef WIN32
        sd_output(sd_sw_cfg1, nTarWriteCount, 10, "NO_OF_REG_WRITE");
#endif
        // Switch to sw mode
        mfe_reg.reg_mfe_g_tbc_mode = 0;
        WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "tbc_mode=0");
        // Switch to hw mode
        mfe_reg.reg_mfe_g_tbc_mode = 1;
        WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "tbc_mode=1");
    }
    MFE_ASSERT(nRegWriteCount==nTarWriteCount);

#ifdef QTABLE_READBACK_CHECK

    if (mfe_reg.reg_mfe_g_qmode==1) {    // Q table

        // Switch to SW mode
        RegQT_RB_REG03.reg03 = 0;
        ReadRegMFE(0x03, &RegQT_RB_REG03.reg03);
        RegQT_RB_REG03.reg_mfe_g_tbc_mode = 0;
        WriteRegMFE(0x03, RegQT_RB_REG03.reg03,"",0,"");

        // Reset hw status
        RegWQT_RB.reg49 = 0;
        ReadRegMFE(0x49, &RegWQT_RB.reg49);
        RegWQT_RB.reg_mfe_s_tbc_done_clr = 1;
        RegWQT_RB.reg_mfe_s_tbc_rw = 0;
        WriteRegMFE(0x49, RegWQT_RB.reg49,"",0,"");
        RegWQT.reg_mfe_s_tbc_done_clr = 0;    // HW is write-one-clear
        while (1) {
            ReadRegMFE(0x49, &RegWQT_RB.reg49);
            if (RegWQT_RB.reg_mfe_s_tbc_done==0)
                break;
        }

        //check table 0
        for (i=0; i<64; i+=2) {

            RegWQT_RB.reg49 = 0;
            ReadRegMFE(0x49, &RegWQT_RB.reg49);
            RegWQT_RB.reg_mfe_s_tbc_adr = i>>1;    //reg49
            WriteRegMFE(0x49, RegWQT_RB.reg49, "", 0, "");

            mfe_reg.reg_mfe_s_tbc_en = 1;
            WriteRegMFE(0x48, mfe_reg.reg48, "[%d] reg48", nRegWriteCount++, "table write enable");
            //polling reg_mfe_s_tbc_done
            RegWQT_RB.reg49 = 0;
            while (1) {
                ReadRegMFE(0x49, &RegWQT_RB.reg49);
                if (RegWQT_RB.reg_mfe_s_tbc_done==1)
                    break;

            }

            ReadRegMFE(0x4b, &RegQT_RB.reg4b);    //read data

            if(((int) RegQT_RB.reg4b) !=( Table0[((i&0x7)<<3)|(i>>3)] | (Table0[(((i+1)&0x7)<<3)|((i+1)>>3)]<<8))){
//                ms_dprintk(DRV_L4, "error,table 0 no match!,i = %d,address = %d",i,i>>1);
                ErrorCount++;
            }

        }
        ms_dprintk(DRV_L4, "Table 0 error : %d\n",ErrorCount);
        ErrorCount = 0;

        //check table 1
        for (i=0; i<64; i+=2) {

            RegWQT_RB.reg49 = 0;
            ReadRegMFE(0x49, &RegWQT_RB.reg49);
            RegWQT_RB.reg_mfe_s_tbc_adr = (64+i)>>1;    //reg49
            WriteRegMFE(0x49, RegWQT_RB.reg49,"",0,"");

            mfe_reg.reg_mfe_s_tbc_en = 1;
            WriteRegMFE(0x48, mfe_reg.reg48, "[%d] reg48", nRegWriteCount++, "table write enable");
            mfe_reg.reg_mfe_s_tbc_en = 0;    // HW is write-one-clear
            //polling reg_mfe_s_tbc_done
            RegWQT_RB.reg49 = 0;
            while (1) {
                ReadRegMFE(0x49, &RegWQT_RB.reg49);
                if (RegWQT_RB.reg_mfe_s_tbc_done==1)
                    break;
            }

            ReadRegMFE(0x4b, &RegQT_RB.reg4b);    //read data

            if(((int) RegQT_RB.reg4b) !=( Table1[((i&0x7)<<3)|(i>>3)] | (Table1[(((i+1)&0x7)<<3)|((i+1)>>3)]<<8))){
//                ms_dprintk(DRV_L4, "error,table 1 no match!,i = %d,address = %d",i,(64+i)>>1);
                ErrorCount++;
            }

        }

        ms_dprintk(DRV_L4, "Table 1 error : %d\n",ErrorCount);


        // Switch to HW mode
        RegQT_RB_REG03.reg03 = 0;
        ReadRegMFE(0x03, &RegQT_RB_REG03.reg03);
        RegQT_RB_REG03.reg_mfe_g_tbc_mode = 1;
        WriteRegMFE(0x03, RegQT_RB_REG03.reg03,"",0,"");
    }

#endif    //QTABLE_READBACK_CHECK

}

// Mode 0: Actual FDC putting.
// Mode 1: Get number of FDC data round (each round is 16bit).

int PutFDC(void* pContext, int mode)
{
    int len, bit_len;
    unsigned int val=0;
    unsigned char* ptr;
    unsigned char bits;
    int nRegWriteNum = 0;
    int nTotalRound = 0;
#ifdef CHECK_FDC_DONE
    RegFdcDone RegFdc;
#endif

    BitsInfo* pBitsInfo = (BitsInfo*)pContext;
    ptr = pBitsInfo->ptr;
    len = pBitsInfo->len;
    bits = pBitsInfo->bits;
    bit_len = pBitsInfo->bit_len;
    bit_len += len*8;

#ifdef DISABLE_PutFDC
    bit_len = len = 0;
#endif

    // Calculate total round.
    if (bit_len==0)
        nTotalRound = 1;
    else
        nTotalRound = (bit_len+15)>>4;
    if (mode==1)
        return nTotalRound;

    ms_dprintk(DRV_L4, "FDC: %d bits\n", bit_len);
    mfe_reg.reg46 = 0;
    mfe_reg.reg47 = 0;
    mfe_reg.reg48 = 0;
    // Fill the total round reg
    mfe_reg.reg_mfe_s_fdc_bs_count = nTotalRound-1;    // Count from 0
    WriteRegMFE(0x47, mfe_reg.reg47, "[FDC %d] reg47", nRegWriteNum++, "fdc round count");

    while (bit_len>16) {
        val = (ptr[0]<<8) | ptr[1];
        ptr += 2;
        bit_len -= 16;
        len -= 2;

        mfe_reg.reg_mfe_s_fdc_bs = val;
        mfe_reg.reg_mfe_s_fdc_bs_len = 15;
        mfe_reg.reg_mfe_s_fdc_bs_vld = 1;
        WriteRegMFE(0x46, mfe_reg.reg46, "[FDC %d] reg46", nRegWriteNum++, "fdc bs");
        WriteRegMFE(0x47, mfe_reg.reg47, "[FDC %d] reg47", nRegWriteNum++, "fdc len");
        WriteRegMFE(0x48, mfe_reg.reg48, "[FDC %d] reg48", nRegWriteNum++, "fdc vld");
        mfe_reg.reg_mfe_s_fdc_bs_vld = 0;    // HW is write-one-clear

#ifdef CHECK_FDC_DONE
        RegFdc.reg48 = 0;
        // Verify hw receives the bits
        while (1) {
            ReadRegMFE(0x48, &RegFdc.reg48);
            if (RegFdc.reg_mfe_s_fdc_done==1) {
                ms_dprintk(DRV_L4, "reg_mfe_s_fdc_done\n");
                break;
            }
            else
                ms_dprintk(DRV_L4, "reg_mfe_s_fdc_done != 1\n");
            MsOS_DelayTask(10);
        }
        // Reset hw status
        RegFdc.reg_mfe_s_fdc_done_clr = 1;
        WriteRegMFE(0x48, RegFdc.reg48,"reg48",nRegWriteNum,"");
        RegFdc.reg_mfe_s_fdc_done_clr = 0;    // HW is write-one-clear
        while (1) {
            ReadRegMFE(0x48, &RegFdc.reg48);
            if (RegFdc.reg_mfe_s_fdc_done==0)
                break;
            else
                ms_dprintk(DRV_L4, "reg_mfe_s_fdc_done != 0\n");
            MsOS_DelayTask(10);
        }
#endif
    }
    if (len==0) {
        val = bits<<8;
    } else if (len==1) {
        val = (ptr[0]<<8) | bits;
    } else if (len==2) {
        val = (ptr[0]<<8) | ptr[1];
    }
    mfe_reg.reg_mfe_s_fdc_bs = val;
    mfe_reg.reg_mfe_s_fdc_bs_len = bit_len;
    mfe_reg.reg_mfe_s_fdc_bs_vld = 1;
    WriteRegMFE(0x46, mfe_reg.reg46, "[FDC %d] reg46", nRegWriteNum++, "fdc bs");
    WriteRegMFE(0x47, mfe_reg.reg47, "[FDC %d] reg47", nRegWriteNum++, "fdc len");
    WriteRegMFE(0x48, mfe_reg.reg48, "[FDC %d] reg48", nRegWriteNum++, "fdc vld");
    mfe_reg.reg_mfe_s_fdc_bs_vld = 0;    // HW is write-one-clear

#ifdef CHECK_FDC_DONE
    RegFdc.reg48 = 0;
    // Verify hw receives the bits
    while (1) {
        ReadRegMFE(0x48, &RegFdc.reg48);
        if (RegFdc.reg_mfe_s_fdc_done==1) {
            ms_dprintk(DRV_L4, "reg_mfe_s_fdc_done\n");
            break;
        }
        else
            ms_dprintk(DRV_L4, "reg_mfe_s_fdc_done != 1\n");
        MsOS_DelayTask(10);
    }
    // Reset hw status
    RegFdc.reg_mfe_s_fdc_done_clr = 1;
    WriteRegMFE(0x48, RegFdc.reg48,"reg48",nRegWriteNum,"");
    RegFdc.reg_mfe_s_fdc_done_clr = 0;    // HW is write-one-clear
    while (1) {
        ReadRegMFE(0x48, &RegFdc.reg48);
        if (RegFdc.reg_mfe_s_fdc_done==0)
            break;
        else
            ms_dprintk(DRV_L4, "reg_mfe_s_fdc_done != 0\n");
        MsOS_DelayTask(10);
    }
#endif
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
    MFE_ASSERT(ctx->nWidth%16==0);
    ctx->nHeight = pConfig->nBufHeight;
    MFE_ASSERT(ctx->nHeight%16==0);
    ctx->nTotalMb = (ctx->nWidth*ctx->nHeight)>>8;
    if (!ctx->pHwMbMap)
        ctx->pHwMbMap = pConfig->m_UIHwMap;//new HW_MB_MAP[ctx->nTotalMb];
    if (!ctx->pSwMbMap)
        ctx->pSwMbMap = pConfig->m_UISwMap;//new SW_MB_MAP[ctx->nTotalMb];

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
        //delete[] ctx->pHwMbMap;
        ctx->pHwMbMap = NULL;
    }
    if (ctx->pSwMbMap) {
        //delete[] ctx->pSwMbMap;
        ctx->pSwMbMap = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////
// Rate Control
//////////////////////////////////////////////////////////////////////////

void MfeDrvRateControlInit(MFE_CONFIG* pConfig)
{

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
       //pConfig->rcInfo.fTargetFrameRate = (float)(pConfig->FrameRatex100/100);
    }
    pConfig->rcInfo.nBitrate = pConfig->nBitrate;
    pConfig->rcInfo.nMaxBitrate = 0;   // Let RC decide
    pConfig->rcInfo.nVPSize = pConfig->nVPSizeInBits > 0 ? pConfig->nVPSizeInBits : 0;    // 0 means not used.
    pConfig->rcInfo.nVPMbRow = pConfig->nVPSizeInMbRow;    // 0 means not used.
    pConfig->rcInfo.bFixedFrameRate = 1;
    pConfig->rcInfo.nPCount = pConfig->nPbetweenI;    // Number of P-frames between I-frames
    pConfig->rcInfo.nBCount = pConfig->nBbetweenP;    // Number of B-frames between P-frames
    pConfig->rcInfo.rcMethod = CONST_BITRATE;//CONSTRAINED_VARIABLE_BITRATE;//VARIABLE_BITRATE;
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
#if 0 // Test const qp
    pConfig->rcInfo.rcMethod = CONST_QUALITY;
    pConfig->rcInfo.rcGranularity = FRAMELEVELRC;
    pConfig->rcInfo.nConstQP = 12;
#endif
    cvbr_InitRateControl(&pConfig->ctxRateControl, &pConfig->rcInfo);
    pConfig->m_cvbrFrameSkip = 0;
    pConfig->m_bGenSkipVopHeader = 1;
}

void MfeDrvRateControlUpdate(MFE_CONFIG* pConfig, char nFieldType)
{
    int nLastFrmAvgQP, nLastFrmBits;
    CVBRRateControl* pRC = &pConfig->ctxRateControl;
    MFE_REG reg;

    // Rate control
    // Read reg_mfe_s_mbr_last_frm_avg_qp and assign to rate control context.
    reg.reg28 = 0;
    reg.reg29 = 0;
    ReadRegMFE(0x28, &reg.reg28);
    ReadRegMFE(0x29, &reg.reg29);
    nLastFrmAvgQP = ((int)reg.reg_mfe_s_mbr_last_frm_avg_qp_high << 16) + reg.reg_mfe_s_mbr_last_frm_avg_qp_low;
    if (nFieldType==0 || nFieldType==1) {
        pRC->m_nLastFrameAvgQP[0] = nLastFrmAvgQP;
        pRC->m_nLastFrameAvgQP[1] = 0;
    }
    else {    // nFieldType=2
        pRC->m_nLastFrameAvgQP[1] = nLastFrmAvgQP;
    }
    // Read
    if (pConfig->m_cvbrFrameSkip>0 && pConfig->vopPredType==P_VOP) {
        nLastFrmBits = pConfig->m_OutStream.m_nByteCount*8;
        cvbr_UpdateFrame(pRC, nLastFrmBits, 1, nFieldType);
        pConfig->m_cvbrFrameSkip--;
    }
    else {
        reg.reg42 = 0;
        reg.reg43 = 0;
        ReadRegMFE(0x42, &reg.reg42);
        ReadRegMFE(0x43, &reg.reg43);
        nLastFrmBits = ((int)reg.reg_mfe_s_bsp_bit_cnt_high<<16) + reg.reg_mfe_s_bsp_bit_cnt_low;
        pConfig->m_cvbrFrameSkip = cvbr_UpdateFrame(pRC, nLastFrmBits, 0, nFieldType);
    }

}


