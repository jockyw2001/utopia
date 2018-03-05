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

#ifndef _LPLL_EXT_TBL_H_
#define _LPLL_EXT_TBL_H_

#define LPLL_EXT_REG_NUM    31

typedef enum
{
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to340MHz,          //0
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to150MHz,          //1

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to150MHz,          //2
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to75MHz,          //3

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to75MHz,          //4
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to37_5MHz,          //5

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_200to340MHz,          //6
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to200MHz,          //7
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to150MHz,          //8

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_100to150MHz,          //9
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to100MHz,          //10
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to75MHz,          //11

    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_50to75MHz,          //12
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to50MHz,          //13
    E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to37_5MHz,          //14

    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_100to150MHz,          //15
    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_50to100MHz,          //16
    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to50MHz,          //17
    E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to25MHz,          //18

    E_PNL_SUPPORTED_LPLL_EXT_MAX,          //19
} E_PNL_SUPPORTED_LPLL_EXT_TYPE;

typedef struct
{
    MS_U8  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct_Ext,*pTBLStruct_Ext;

TBLStruct_Ext LPLLSettingTBL_Ext[E_PNL_SUPPORTED_LPLL_EXT_MAX][LPLL_EXT_REG_NUM]=
{
    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to340MHz    NO.0
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to150MHz    NO.1
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to150MHz    NO.2
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to75MHz    NO.3
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0000,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to75MHz    NO.4
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0020,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to37_5MHz    NO.5
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0020,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_200to340MHz    NO.6
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0000,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to200MHz    NO.7
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to150MHz    NO.8
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_100to150MHz    NO.9
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to100MHz    NO.10
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to75MHz    NO.11
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_50to75MHz    NO.12
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to50MHz    NO.13
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to37_5MHz    NO.14
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0003,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0030,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0400,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0003,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0030,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0400,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x1000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0000,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0800,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0100,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x0000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0400,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_100to150MHz    NO.15
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0000,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0000,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_50to100MHz    NO.16
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0001,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0001,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to50MHz    NO.17
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0002,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

    { //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to25MHz    NO.18
      //Address,Value,Mask
        {0x59,0x0000,0x2000},//reg_lpll1_nossc_pd
        {0x40,0x0001,0x0007},//reg_lpll_ext_ictrl
        {0x41,0x0000,0x0003},//reg_lpll_ext_input_div_fst
        {0x41,0x0020,0x0030},//reg_lpll_ext_loop_div_fst
        {0x41,0x0300,0x0F00},//reg_lpll_ext_loop_div_sec
        {0x42,0x0002,0x0003},//reg_lpll_ext_scalar_div_fst
        {0x42,0x0070,0x00F0},//reg_lpll_ext_scalar_div_sec
        {0x43,0x0002,0x0007},//reg_lpll_ext_skew_div
        {0x42,0x0000,0x0700},//reg_lpll_ext_fifo_div
        {0x40,0x0000,0x1000},//reg_lpll_ext_fifo_div5_en
        {0x5A,0x0040,0x0040},//reg_lpll_ext_en_fix_clk
        {0x40,0x0000,0x0800},//reg_lpll_ext_dual_lp_en
        {0x40,0x0000,0x0100},//reg_lpll_ext_sdiv2p5_en
        {0x40,0x2000,0x2000},//reg_lpll_ext_en_mini
        {0x40,0x0000,0x0400},//reg_lpll1_ext_en_fifo
        {0x4A,0x0000,0x8000},//reg_lpll_ext_test[15]
        {0x4B,0x0000,0x0001},//reg_lpll_ext_test[16]
        {0x4B,0x0000,0x0040},//reg_lpll_ext_test[22]
        {0x59,0x4000,0x4000},//reg_lpll2_ext_nossc_pd
        {0x58,0x0000,0x4000},//reg_lpll2_ext_ibias_ictrl
        {0x58,0x0000,0x001F},//reg_lpll2_ext_input_div_first
        {0x58,0x0000,0x0060},//reg_lpll2_ext_loop_div_first
        {0x58,0x0000,0x1F00},//reg_lpll2_ext_loop_div_second
        {0x5A,0x0000,0x000F},//reg_lpll2_ext_output_div_first
        {0x59,0x0000,0x0800},//reg_lpll2_ext_test[11]
        {0x59,0x0000,0x1000},//reg_lpll2_ext_test[12]
        {0x59,0x0000,0x0100},//reg_lpll2_ext_test[8]
        {0x5A,0x0000,0x2000},//reg_lpll_ext_2ndpll_clk_sel
        {0x40,0x0200,0x0200},//reg_lpll_ext_en_scalar
        {0x5A,0x0000,0x0200},//reg_lpll_ext_scalar2fifo_en
        {0x5A,0x0000,0x0100},//reg_lpll_scalar2fifo_div2
    },

};
MS_U16 u16EXT_LoopGain[E_PNL_SUPPORTED_LPLL_EXT_MAX]=
{
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to340MHz    NO.0
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to150MHz    NO.1
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to150MHz    NO.2
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to75MHz    NO.3
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to75MHz    NO.4
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to37_5MHz    NO.5
    64,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_200to340MHz    NO.6
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to200MHz    NO.7
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to150MHz    NO.8
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_100to150MHz    NO.9
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to100MHz    NO.10
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to75MHz    NO.11
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_50to75MHz    NO.12
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to50MHz    NO.13
    32,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to37_5MHz    NO.14
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_100to150MHz    NO.15
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_50to100MHz    NO.16
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to50MHz    NO.17
    12,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to25MHz    NO.18
};
MS_U16 u16EXT_LoopDiv[E_PNL_SUPPORTED_LPLL_EXT_MAX]=
{
    10,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to340MHz    NO.0
    10,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_4lane_150to150MHz    NO.1
    20,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to150MHz    NO.2
    20,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_2lane_75to75MHz    NO.3
    40,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to75MHz    NO.4
    40,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_10bit_1lane_37_5to37_5MHz    NO.5
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_200to340MHz    NO.6
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to200MHz    NO.7
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_4lane_150to150MHz    NO.8
    15,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_100to150MHz    NO.9
    30,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to100MHz    NO.10
    30,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_2lane_75to75MHz    NO.11
    30,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_50to75MHz    NO.12
    60,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to50MHz    NO.13
    60,           //E_PNL_SUPPORTED_LPLL_EXT_VBY1_8bit_1lane_37_5to37_5MHz    NO.14
    7,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_100to150MHz    NO.15
    14,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_50to100MHz    NO.16
    28,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to50MHz    NO.17
    28,           //E_PNL_SUPPORTED_LPLL_EXT_HS_LVDS_1ch_25to25MHz    NO.18
};

#endif //_LPLL_TBL_H_
