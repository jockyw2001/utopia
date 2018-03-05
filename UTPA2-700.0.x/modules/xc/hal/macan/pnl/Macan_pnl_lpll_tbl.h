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

#define LPLL_REG_NUM    41

typedef enum
{
    E_PNL_SUPPORTED_LPLL_TTL_100to150MHz,          //0
    E_PNL_SUPPORTED_LPLL_TTL_50to100MHz,          //1
    E_PNL_SUPPORTED_LPLL_TTL_25to50MHz,          //2
    E_PNL_SUPPORTED_LPLL_TTL_25to25MHz,          //3

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to300MHz,          //4
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz,          //5

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz,          //6
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz,          //7

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz,          //8
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz,          //9

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to300MHz,          //10
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz,          //11
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz,          //12

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz,          //13
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz,          //14
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz,          //15

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz,          //16
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz,          //17
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz,          //18

    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to300MHz,          //19
    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to150MHz,          //20

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to300MHz,          //21
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to150MHz,          //22

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_240to300MHz,          //23
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to240MHz,          //24
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to150MHz,          //25

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_180to300MHz,          //26
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to180MHz,          //27
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to150MHz,          //28

    E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to300MHz,          //29
    E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to150MHz,          //30

    E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to300MHz,          //31
    E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to150MHz,          //32

    E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to300MHz,          //33
    E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to150MHz,          //34

    E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz,          //35
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz,          //36

    E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to300MHz,          //37
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to150MHz,          //38

    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz,          //39
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to100MHz,          //40
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz,          //41

    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz,          //42
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_50to100MHz,          //43
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to50MHz,          //44
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz,          //45

    E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz,          //46
    E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz,          //47

    E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz,          //48
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz,          //49
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz,          //50

    E_PNL_SUPPORTED_LPLL_MAX,          //51
} E_PNL_SUPPORTED_LPLL_TYPE;

typedef struct
{
    MS_U8  address;
    MS_U16 value;
    MS_U16 mask;
}TBLStruct,*pTBLStruct;

TBLStruct LPLLSettingTBL[E_PNL_SUPPORTED_LPLL_MAX][LPLL_REG_NUM]=
{
    { //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.0
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0800,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0800,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to300MHz    NO.4
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz    NO.5
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.6
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.7
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz    NO.8
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz    NO.9
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to300MHz    NO.10
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz    NO.11
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz    NO.12
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz    NO.13
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz    NO.14
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.15
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz    NO.16
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz    NO.17
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz    NO.18
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to300MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to150MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to300MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to150MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_240to300MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to240MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to150MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_180to300MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0014,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to180MHz    NO.27
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to150MHz    NO.28
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0700,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to300MHz    NO.29
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to150MHz    NO.30
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to300MHz    NO.31
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to150MHz    NO.32
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to300MHz    NO.33
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to150MHz    NO.34
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz    NO.35
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0200,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0100,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz    NO.36
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0200,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0100,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to300MHz    NO.37
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0200,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to150MHz    NO.38
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0900,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0200,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz    NO.39
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to100MHz    NO.40
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz    NO.41
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz    NO.42
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_50to100MHz    NO.43
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to50MHz    NO.44
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz    NO.45
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.46
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz    NO.47
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.48
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.49
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz    NO.50
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[5:4]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[3:0]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1_test[15]
        {0x37,0x0000,0x0001},//reg_lpll1_test[16]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x37,0x0000,0x0040},//reg_lpll1_test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0xFF,0x0032,0xFFFF},//wait_time(micro_second)
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1_test[2]
    },

};
MS_U16 u16LoopGain[E_PNL_SUPPORTED_LPLL_MAX]=
{
    12,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.0
    12,           //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.1
    12,           //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.2
    12,           //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.3
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to300MHz    NO.4
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz    NO.5
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.6
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.7
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz    NO.8
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz    NO.9
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to300MHz    NO.10
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz    NO.11
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz    NO.12
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz    NO.13
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz    NO.14
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.15
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz    NO.16
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz    NO.17
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz    NO.18
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to300MHz    NO.19
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to150MHz    NO.20
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to300MHz    NO.21
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to150MHz    NO.22
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_240to300MHz    NO.23
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to240MHz    NO.24
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to150MHz    NO.25
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_180to300MHz    NO.26
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to180MHz    NO.27
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to150MHz    NO.28
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to300MHz    NO.29
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to150MHz    NO.30
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to300MHz    NO.31
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to150MHz    NO.32
    8,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to300MHz    NO.33
    8,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to150MHz    NO.34
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz    NO.35
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz    NO.36
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to300MHz    NO.37
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to150MHz    NO.38
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz    NO.39
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to100MHz    NO.40
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz    NO.41
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz    NO.42
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_50to100MHz    NO.43
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to50MHz    NO.44
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz    NO.45
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.46
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz    NO.47
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.48
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.49
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz    NO.50
};
MS_U16 u16LoopDiv[E_PNL_SUPPORTED_LPLL_MAX]=
{
    8,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.0
    16,           //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.1
    32,           //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.2
    32,           //E_PNL_SUPPORTED_LPLL_TTL_25to25MHz    NO.3
    10,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to300MHz    NO.4
    10,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_4K_150to150MHz    NO.5
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.6
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.7
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz    NO.8
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz    NO.9
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_200to300MHz    NO.10
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to200MHz    NO.11
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_4K_150to150MHz    NO.12
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz    NO.13
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz    NO.14
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.15
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz    NO.16
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz    NO.17
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz    NO.18
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to300MHz    NO.19
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_4K_CASE1_150to150MHz    NO.20
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to300MHz    NO.21
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_4K_150to150MHz    NO.22
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_240to300MHz    NO.23
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to240MHz    NO.24
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_4K_EPI3G_FOR_TRY_150to150MHz    NO.25
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_180to300MHz    NO.26
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to180MHz    NO.27
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_4K_EPI3G_FOR_TRY_150to150MHz    NO.28
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to300MHz    NO.29
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_X1_150to150MHz    NO.30
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to300MHz    NO.31
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_X1_150to150MHz    NO.32
    4,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to300MHz    NO.33
    4,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_4K_150to150MHz    NO.34
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz    NO.35
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz    NO.36
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to300MHz    NO.37
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_X1_DUAL_150to150MHz    NO.38
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz    NO.39
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to100MHz    NO.40
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to50MHz    NO.41
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz    NO.42
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_50to100MHz    NO.43
    28,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to50MHz    NO.44
    28,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to25MHz    NO.45
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.46
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to50MHz    NO.47
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.48
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.49
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to25MHz    NO.50
};

#endif //_LPLL_TBL_H_
