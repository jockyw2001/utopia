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

#define LPLL_REG_NUM    24

typedef enum
{
    E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz,          //0

    E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz,          //1
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz,          //2

    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz,          //3
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz,          //4

    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz,          //5
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz,          //6
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz,          //7

    E_PNL_SUPPORTED_LPLL_TTL_100to150MHz,          //8
    E_PNL_SUPPORTED_LPLL_TTL_50to100MHz,          //9
    E_PNL_SUPPORTED_LPLL_TTL_25to50MHz,          //10

    E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_8BIT_50to80MHz,          //11

    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_100to150MHz,          //12
    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_50to100MHz,          //13

    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_100to150MHz,          //14
    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_50to100MHz,          //15

    E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_66_67to80MHz,          //16
    E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_50to66_67MHz,          //17

    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_133_33to150MHz,          //18
    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_66_67to133_33MHz,          //19
    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_50to66_67MHz,          //20

    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_133to150MHz,          //21
    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_67to133MHz,          //22
    E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_50to67MHz,          //23

    E_PNL_SUPPORTED_LPLL_EPI34_10BIT_2PAIR_150to150MHz,          //24

    E_PNL_SUPPORTED_LPLL_EPI34_10BIT_4PAIR_80to150MHz,          //25

    E_PNL_SUPPORTED_LPLL_EPI34_10BIT_6PAIR_80to150MHz,          //26

    E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_188to300MHz,          //27
    E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_80to188MHz,          //28

    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_2PAIR_150to150MHz,          //29

    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_114to150MHz,          //30
    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_80to114MHz,          //31

    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_172to300MHz,          //32
    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_80to172MHz,          //33

    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_228to300MHz,          //34
    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_114to228MHz,          //35
    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_80to114MHz,          //36

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to340MHz,          //37

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz,          //38

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz,          //39

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_220to340MHz,          //40
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to220MHz,          //41

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz,          //42
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz,          //43

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz,          //44
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to55MHz,          //45

    E_PNL_SUPPORTED_LPLL_EPI28_8BIT_12PAIR_340to340MHz,          //46

    E_PNL_SUPPORTED_LPLL_MAX,          //47
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
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0700,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0400,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0800,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x5000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_8BIT_50to80MHz    NO.11
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_100to150MHz    NO.12
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_50to100MHz    NO.13
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_100to150MHz    NO.14
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_50to100MHz    NO.15
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x3000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0002,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_66_67to80MHz    NO.16
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_50to66_67MHz    NO.17
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_133_33to150MHz    NO.18
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_66_67to133_33MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_50to66_67MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x3000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0002,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_133to150MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_67to133MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x3000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0002,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_50to67MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x4000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0003,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_2PAIR_150to150MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0000,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0011,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_4PAIR_80to150MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0011,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_6PAIR_80to150MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0011,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_188to300MHz    NO.27
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0011,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0400,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_80to188MHz    NO.28
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0011,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0400,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_2PAIR_150to150MHz    NO.29
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0000,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x000E,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_114to150MHz    NO.30
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x000E,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_80to114MHz    NO.31
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x000E,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_172to300MHz    NO.32
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x000E,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_80to172MHz    NO.33
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x000E,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0600,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_228to300MHz    NO.34
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x2000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_114to228MHz    NO.35
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x2000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_80to114MHz    NO.36
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x2000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to340MHz    NO.37
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.38
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz    NO.39
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0200,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_220to340MHz    NO.40
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to220MHz    NO.41
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz    NO.42
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz    NO.43
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz    NO.44
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to55MHz    NO.45
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0300,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x000F},//reg_lpll_fifo_div
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x8000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

    { //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_12PAIR_340to340MHz    NO.46
      //Address,Value,Mask
        {0x03,0x0000,0x0020},//reg_lpll1_pd
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x0F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_scalar_div_first
        {0x02,0x0000,0x0F00},//reg_lpll1_scalar_div_second
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x000F},//reg_lpll_fifo_div
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x8000},//reg_lpll1_duap_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x000E,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0300,0xFF00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll1_en_scalar
        {0x04,0x0000,0x2000},//reg_lpll1_sdiv3p5_en
    },

};
MS_U16 u16LoopGain[E_PNL_SUPPORTED_LPLL_MAX]=
{
    6,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.0
    6,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.1
    3,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.2
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz    NO.3
    6,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz    NO.4
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz    NO.5
    6,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz    NO.6
    3,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz    NO.7
    3,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.8
    3,           //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.9
    3,           //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.10
    3,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_8BIT_50to80MHz    NO.11
    3,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_100to150MHz    NO.12
    3,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_50to100MHz    NO.13
    3,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_100to150MHz    NO.14
    3,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_50to100MHz    NO.15
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_66_67to80MHz    NO.16
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_50to66_67MHz    NO.17
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_133_33to150MHz    NO.18
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_66_67to133_33MHz    NO.19
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_50to66_67MHz    NO.20
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_133to150MHz    NO.21
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_67to133MHz    NO.22
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_50to67MHz    NO.23
    16,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_2PAIR_150to150MHz    NO.24
    24,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_4PAIR_80to150MHz    NO.25
    18,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_6PAIR_80to150MHz    NO.26
    48,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_188to300MHz    NO.27
    24,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_80to188MHz    NO.28
    8,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_2PAIR_150to150MHz    NO.29
    12,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_114to150MHz    NO.30
    6,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_80to114MHz    NO.31
    18,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_172to300MHz    NO.32
    9,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_80to172MHz    NO.33
    24,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_228to300MHz    NO.34
    12,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_114to228MHz    NO.35
    6,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_80to114MHz    NO.36
    2,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to340MHz    NO.37
    1,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.38
    1,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz    NO.39
    8,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_220to340MHz    NO.40
    4,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to220MHz    NO.41
    4,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz    NO.42
    2,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz    NO.43
    2,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz    NO.44
    1,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to55MHz    NO.45
    18,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_12PAIR_340to340MHz    NO.46
};
MS_U16 u16LoopDiv[E_PNL_SUPPORTED_LPLL_MAX]=
{
    7,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_50to80MHz    NO.0
    7,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.1
    7,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_25to50MHz    NO.2
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_115to150MHz    NO.3
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_50to115MHz    NO.4
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_115to150MHz    NO.5
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_57to115MHz    NO.6
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_25to57MHz    NO.7
    2,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.8
    4,           //E_PNL_SUPPORTED_LPLL_TTL_50to100MHz    NO.9
    8,           //E_PNL_SUPPORTED_LPLL_TTL_25to50MHz    NO.10
    4,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_8BIT_50to80MHz    NO.11
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_100to150MHz    NO.12
    4,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_8BIT_50to100MHz    NO.13
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_100to150MHz    NO.14
    4,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_8BIT_50to100MHz    NO.15
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_66_67to80MHz    NO.16
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1CH_3PAIR_6BIT_50to66_67MHz    NO.17
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_133_33to150MHz    NO.18
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_66_67to133_33MHz    NO.19
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_3PAIR_6BIT_50to66_67MHz    NO.20
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_133to150MHz    NO.21
    1,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_67to133MHz    NO.22
    2,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2CH_6PAIR_6BIT_50to67MHz    NO.23
    17,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_2PAIR_150to150MHz    NO.24
    17,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_4PAIR_80to150MHz    NO.25
    17,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_6PAIR_80to150MHz    NO.26
    17,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_188to300MHz    NO.27
    17,           //E_PNL_SUPPORTED_LPLL_EPI34_10BIT_8PAIR_80to188MHz    NO.28
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_2PAIR_150to150MHz    NO.29
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_114to150MHz    NO.30
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_4PAIR_80to114MHz    NO.31
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_172to300MHz    NO.32
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_6PAIR_80to172MHz    NO.33
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_228to300MHz    NO.34
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_114to228MHz    NO.35
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_8PAIR_80to114MHz    NO.36
    1,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to340MHz    NO.37
    1,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.38
    2,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_38to75MHz    NO.39
    3,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_220to340MHz    NO.40
    3,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to220MHz    NO.41
    3,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_110to150MHz    NO.42
    3,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to110MHz    NO.43
    3,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_55to75MHz    NO.44
    3,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to55MHz    NO.45
    7,           //E_PNL_SUPPORTED_LPLL_EPI28_8BIT_12PAIR_340to340MHz    NO.46
};

#endif //_LPLL_TBL_H_
