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

#ifndef _LPLL_TBL_H_
#define _LPLL_TBL_H_

#define LPLL_REG_NUM    29

typedef enum
{
    E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz,          //0
    E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz,          //1

    E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to80MHz,          //2
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz,          //3
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz,          //4

    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz,          //5
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz,          //6
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz,          //7

    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz,          //8
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz,          //9
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz,          //10
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz,          //11

    E_PNL_SUPPORTED_LPLL_TTL_100to150MHz,          //12
    E_PNL_SUPPORTED_LPLL_TTL_50to100MHz,          //13
    E_PNL_SUPPORTED_LPLL_TTL_25to50MHz,          //14
    E_PNL_SUPPORTED_LPLL_TTL_25to25MHz,          //15

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to340MHz,          //16
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz,          //17

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz,          //18
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz,          //19

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz,          //20
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to38MHz,          //21

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to340MHz,          //22
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz,          //23
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz,          //24

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz,          //25
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz,          //26
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz,          //27

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz,          //28
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to55MHz,          //29
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to38MHz,          //30

    E_PNL_SUPPORTED_LPLL_EPI_34_10BIT_12PAIR_4K_CASE2_V15_330to330MHz,          //31

    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_V15_330to330MHz,          //32

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_V15_330to330MHz,          //33

    E_PNL_SUPPORTED_LPLL_MAX,          //34
} E_PNL_SUPPORTED_LPLL_TYPE;

typedef struct
{
    MS_U8  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct,*pTBLStruct;

TBLStruct LPLLSettingTBL[E_PNL_SUPPORTED_LPLL_MAX][LPLL_REG_NUM]=
{
    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.0
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to80MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz    NO.11
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.12
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.13
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.14
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0800,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.15
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0800,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to340MHz    NO.16
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz    NO.17
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.18
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to38MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to340MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.27
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz    NO.28
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to55MHz    NO.29
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to38MHz    NO.30
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_34_10BIT_12PAIR_4K_CASE2_V15_330to330MHz    NO.31
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0100,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0001,0x0001},//reg_lpll1_test[16]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_V15_330to330MHz    NO.32
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_V15_330to330MHz    NO.33
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x37,0x0000,0x0004},//reg_lpll1_test[18]
    },

};
MS_U16 u16LoopGain[E_PNL_SUPPORTED_LPLL_MAX]=
{
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.0
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz    NO.1
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to80MHz    NO.2
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.3
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz    NO.4
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz    NO.5
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz    NO.6
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz    NO.7
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz    NO.8
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz    NO.9
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz    NO.10
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz    NO.11
    12,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.12
    12,           //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.13
    12,           //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.14
    12,           //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.15
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to340MHz    NO.16
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz    NO.17
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.18
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.19
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz    NO.20
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to38MHz    NO.21
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to340MHz    NO.22
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz    NO.23
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz    NO.24
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz    NO.25
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz    NO.26
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.27
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz    NO.28
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to55MHz    NO.29
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to38MHz    NO.30
    8,           //E_PNL_SUPPORTED_LPLL_EPI_34_10BIT_12PAIR_4K_CASE2_V15_330to330MHz    NO.31
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_V15_330to330MHz    NO.32
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_V15_330to330MHz    NO.33
};
MS_U16 u16LoopDiv[E_PNL_SUPPORTED_LPLL_MAX]=
{
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.0
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz    NO.1
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to80MHz    NO.2
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.3
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz    NO.4
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz    NO.5
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz    NO.6
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz    NO.7
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz    NO.8
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz    NO.9
    28,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz    NO.10
    28,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz    NO.11
    8,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.12
    16,           //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.13
    32,           //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.14
    32,           //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.15
    10,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to340MHz    NO.16
    10,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz    NO.17
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.18
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.19
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz    NO.20
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to38MHz    NO.21
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to340MHz    NO.22
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz    NO.23
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz    NO.24
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz    NO.25
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz    NO.26
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.27
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz    NO.28
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to55MHz    NO.29
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_38to38MHz    NO.30
    4,           //E_PNL_SUPPORTED_LPLL_EPI_34_10BIT_12PAIR_4K_CASE2_V15_330to330MHz    NO.31
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_V15_330to330MHz    NO.32
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_V15_330to330MHz    NO.33
};

#endif //_LPLL_TBL_H_
