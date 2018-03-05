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

#define LPLL_REG_NUM    31

typedef enum
{
    E_PNL_SUPPORTED_LPLL_TTL_100to150MHz,          //0
    E_PNL_SUPPORTED_LPLL_TTL_75to100MHz,          //1
    E_PNL_SUPPORTED_LPLL_TTL_75to75MHz,          //2

    E_PNL_SUPPORTED_LPLL_LVDS_1CH_55to80MHz,          //3
    E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to55MHz,          //4
    E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to40MHz,          //5

    E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz,          //6
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to50MHz,          //7
    E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to37_5MHz,          //8

    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz,          //9
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to100MHz,          //10
    E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to75MHz,          //11

    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz,          //12
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to100MHz,          //13
    E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to75MHz,          //14

    E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to80MHz,          //15
    E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to40MHz,          //16

    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to150MHz,          //17
    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to75MHz,          //18

    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to150MHz,          //19
    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to75MHz,          //20

    E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_66_67to80MHz,          //21
    E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to66_67MHz,          //22
    E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to40MHz,          //23

    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_133_33to150MHz,          //24
    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to133_33MHz,          //25
    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to75MHz,          //26

    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_133_33to150MHz,          //27
    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to133_33MHz,          //28
    E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to75MHz,          //29

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_115to150MHz,          //30
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to115MHz,          //31
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to75MHz,          //32

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_115to150MHz,          //33
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to115MHz,          //34
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to75MHz,          //35

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to300MHz,          //36
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to150MHz,          //37

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_230to300MHz,          //38
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to230MHz,          //39
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to150MHz,          //40

    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to300MHz,          //41
    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to150MHz,          //42

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_200to300MHz,          //43
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to200MHz,          //44
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to150MHz,          //45

    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to300MHz,          //46
    E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to150MHz,          //47

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to300MHz,          //48
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to150MHz,          //49

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_240to300MHz,          //50
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to240MHz,          //51
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to150MHz,          //52

    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_180to300MHz,          //53
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to180MHz,          //54
    E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to150MHz,          //55

    E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to300MHz,          //56
    E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to150MHz,          //57

    E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to300MHz,          //58
    E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to150MHz,          //59

    E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to300MHz,          //60
    E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to150MHz,          //61

    E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to300MHz,          //62
    E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to150MHz,          //63

    E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to300MHz,          //64
    E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to150MHz,          //65

    E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_200to300MHz,          //66
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to200MHz,          //67
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to150MHz,          //68

    E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz,          //69
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz,          //70

    E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to300MHz,          //71
    E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to150MHz,          //72

    E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_199_2to300MHz,          //73
    E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to199_2MHz,          //74
    E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to150MHz,          //75

    E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_249to300MHz,          //76
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to249MHz,          //77
    E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to150MHz,          //78

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to300MHz,          //79
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to150MHz,          //80

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz,          //81
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz,          //82

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz,          //83
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz,          //84

    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_200to300MHz,          //85
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to200MHz,          //86
    E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to150MHz,          //87

    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz,          //88
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz,          //89
    E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz,          //90

    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz,          //91
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz,          //92
    E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz,          //93

    E_PNL_SUPPORTED_LPLL_MAX,          //94
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
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_75to100MHz    NO.1
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_TTL_75to75MHz    NO.2
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x7000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_55to80MHz    NO.3
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to55MHz    NO.4
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to40MHz    NO.5
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.6
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to50MHz    NO.7
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to37_5MHz    NO.8
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz    NO.9
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to100MHz    NO.10
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to75MHz    NO.11
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz    NO.12
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to100MHz    NO.13
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to75MHz    NO.14
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0700,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to80MHz    NO.15
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0800,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to40MHz    NO.16
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0800,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to150MHz    NO.17
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to75MHz    NO.18
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to150MHz    NO.19
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to75MHz    NO.20
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0400,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_66_67to80MHz    NO.21
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to66_67MHz    NO.22
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to40MHz    NO.23
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_133_33to150MHz    NO.24
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to133_33MHz    NO.25
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to75MHz    NO.26
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_133_33to150MHz    NO.27
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x2000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0001,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to133_33MHz    NO.28
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x3000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0002,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to75MHz    NO.29
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0300,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x3000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0002,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x0000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_115to150MHz    NO.30
      //Address,Value,Mask
        {0x03,0x0014,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to115MHz    NO.31
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to75MHz    NO.32
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_115to150MHz    NO.33
      //Address,Value,Mask
        {0x03,0x0014,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to115MHz    NO.34
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to75MHz    NO.35
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0002,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to300MHz    NO.36
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to150MHz    NO.37
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_230to300MHz    NO.38
      //Address,Value,Mask
        {0x03,0x0014,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to230MHz    NO.39
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to150MHz    NO.40
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to300MHz    NO.41
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to150MHz    NO.42
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_200to300MHz    NO.43
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to200MHz    NO.44
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to150MHz    NO.45
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to300MHz    NO.46
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to150MHz    NO.47
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to300MHz    NO.48
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to150MHz    NO.49
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_240to300MHz    NO.50
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to240MHz    NO.51
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to150MHz    NO.52
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_180to300MHz    NO.53
      //Address,Value,Mask
        {0x03,0x0014,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to180MHz    NO.54
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to150MHz    NO.55
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0700,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to300MHz    NO.56
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to150MHz    NO.57
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to300MHz    NO.58
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to150MHz    NO.59
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0000,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to300MHz    NO.60
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0500,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0005,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0001,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0900,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to150MHz    NO.61
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0500,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0004,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0005,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0001,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0900,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to300MHz    NO.62
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to150MHz    NO.63
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0500,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to300MHz    NO.64
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0005,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to150MHz    NO.65
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0005,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_200to300MHz    NO.66
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to200MHz    NO.67
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to150MHz    NO.68
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0002,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz    NO.69
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0200,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0100,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz    NO.70
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0001,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0200,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0100,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to300MHz    NO.71
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0005,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to150MHz    NO.72
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0005,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_199_2to300MHz    NO.73
      //Address,Value,Mask
        {0x03,0x0010,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0F00,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x000E,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to199_2MHz    NO.74
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0F00,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x000E,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to150MHz    NO.75
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0F00,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x000E,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_249to300MHz    NO.76
      //Address,Value,Mask
        {0x03,0x0008,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0008,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0800,0x0800},//reg_lpll2_test[11]
        {0x39,0x1000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to249MHz    NO.77
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x000E,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to150MHz    NO.78
      //Address,Value,Mask
        {0x03,0x0004,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0001,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0900,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0000,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x0000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0000,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x4000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x8000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0000,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0003,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x000E,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0100,0x0100},//reg_lpll2_test[8]
        {0x35,0x0020,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to300MHz    NO.79
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to150MHz    NO.80
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0000,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.81
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.82
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz    NO.83
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz    NO.84
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0200,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_200to300MHz    NO.85
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x0000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to200MHz    NO.86
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to150MHz    NO.87
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz    NO.88
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x1000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz    NO.89
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.90
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz    NO.91
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x2000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x0000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0000,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz    NO.92
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

    { //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz    NO.93
      //Address,Value,Mask
        {0x03,0x000C,0x001C},//reg_lpll1_ibias_ictrl
        {0x15,0x0000,0x0003},//reg_lpll1_input_div_first
        {0x01,0x0003,0x0003},//reg_lpll1_loop_div_first
        {0x01,0x0400,0x1F00},//reg_lpll1_loop_div_second
        {0x02,0x3000,0x3000},//reg_lpll1_output_div_second[13:12]
        {0x02,0x0300,0x0F00},//reg_lpll1_output_div_second[11:8]
        {0x35,0x1000,0x7000},//reg_lpll1_skew_div
        {0x2E,0x0004,0x0007},//reg_lpll1_fifo_div
        {0x03,0x0800,0x0800},//reg_lpll1_fifo_div5_en
        {0x2E,0x1000,0x1000},//reg_lpll1_en_fix_clk
        {0x2E,0x8000,0x8000},//reg_lpll1_dual_lp_en
        {0x03,0x0400,0x0400},//reg_lpll1_sdiv2p5_en
        {0x2E,0x0000,0x4000},//reg_lpll1_en_mini
        {0x2E,0x0040,0x0040},//reg_lpll1_en_fifo
        {0x36,0x0000,0x8000},//reg_lpll1__test[15]
        {0x37,0x0000,0x0001},//reg_lpll1__test[16]
        {0x37,0x0000,0x0040},//reg_lpll1__test[22]
        {0x33,0x0020,0x0020},//reg_lpll2_pd
        {0x33,0x0000,0x0004},//reg_lpll2_ibias_ictrl
        {0x30,0x0000,0x001F},//reg_lpll2_input_div_first
        {0x31,0x0000,0x0003},//reg_lpll2_loop_div_first
        {0x31,0x0000,0x1F00},//reg_lpll2_loop_div_second
        {0x32,0x0000,0x000F},//reg_lpll2_output_div_first
        {0x39,0x0000,0x0800},//reg_lpll2_test[11]
        {0x39,0x0000,0x1000},//reg_lpll2_test[12]
        {0x39,0x0000,0x0100},//reg_lpll2_test[8]
        {0x35,0x0000,0x0020},//reg_lpll_2ndpll_clk_sel
        {0x2E,0x0010,0x0010},//reg_lpll_en_scalar
        {0x38,0x0000,0x0200},//reg_lpll1_scalar2fifo_en
        {0x38,0x0000,0x0100},//reg_lpll1_scalar2fifo_div2
        {0x36,0x0000,0x0004},//reg_lpll1__test[2]
    },

};
MS_U16 u16LoopGain[E_PNL_SUPPORTED_LPLL_MAX]=
{
    12,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.0
    12,           //E_PNL_SUPPORTED_LPLL_TTL_75to100MHz    NO.1
    12,           //E_PNL_SUPPORTED_LPLL_TTL_75to75MHz    NO.2
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_55to80MHz    NO.3
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to55MHz    NO.4
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to40MHz    NO.5
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.6
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to50MHz    NO.7
    12,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to37_5MHz    NO.8
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz    NO.9
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to100MHz    NO.10
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to75MHz    NO.11
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz    NO.12
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to100MHz    NO.13
    12,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to75MHz    NO.14
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to80MHz    NO.15
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to40MHz    NO.16
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to150MHz    NO.17
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to75MHz    NO.18
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to150MHz    NO.19
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to75MHz    NO.20
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_66_67to80MHz    NO.21
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to66_67MHz    NO.22
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to40MHz    NO.23
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_133_33to150MHz    NO.24
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to133_33MHz    NO.25
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to75MHz    NO.26
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_133_33to150MHz    NO.27
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to133_33MHz    NO.28
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to75MHz    NO.29
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_115to150MHz    NO.30
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to115MHz    NO.31
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to75MHz    NO.32
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_115to150MHz    NO.33
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to115MHz    NO.34
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to75MHz    NO.35
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to300MHz    NO.36
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to150MHz    NO.37
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_230to300MHz    NO.38
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to230MHz    NO.39
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to150MHz    NO.40
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to300MHz    NO.41
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to150MHz    NO.42
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_200to300MHz    NO.43
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to200MHz    NO.44
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to150MHz    NO.45
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to300MHz    NO.46
    8,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to150MHz    NO.47
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to300MHz    NO.48
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to150MHz    NO.49
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_240to300MHz    NO.50
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to240MHz    NO.51
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to150MHz    NO.52
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_180to300MHz    NO.53
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to180MHz    NO.54
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to150MHz    NO.55
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to300MHz    NO.56
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to150MHz    NO.57
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to300MHz    NO.58
    8,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to150MHz    NO.59
    72,           //E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to300MHz    NO.60
    72,           //E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to150MHz    NO.61
    8,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to300MHz    NO.62
    8,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to150MHz    NO.63
    8,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to300MHz    NO.64
    8,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to150MHz    NO.65
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_200to300MHz    NO.66
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to200MHz    NO.67
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to150MHz    NO.68
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz    NO.69
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz    NO.70
    8,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to300MHz    NO.71
    8,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to150MHz    NO.72
    16,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_199_2to300MHz    NO.73
    16,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to199_2MHz    NO.74
    16,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to150MHz    NO.75
    8,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_249to300MHz    NO.76
    16,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to249MHz    NO.77
    16,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to150MHz    NO.78
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to300MHz    NO.79
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to150MHz    NO.80
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.81
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.82
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz    NO.83
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz    NO.84
    64,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_200to300MHz    NO.85
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to200MHz    NO.86
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to150MHz    NO.87
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz    NO.88
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz    NO.89
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.90
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz    NO.91
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz    NO.92
    32,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz    NO.93
};
MS_U16 u16LoopDiv[E_PNL_SUPPORTED_LPLL_MAX]=
{
    8,           //E_PNL_SUPPORTED_LPLL_TTL_100to150MHz    NO.0
    16,           //E_PNL_SUPPORTED_LPLL_TTL_75to100MHz    NO.1
    16,           //E_PNL_SUPPORTED_LPLL_TTL_75to75MHz    NO.2
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_55to80MHz    NO.3
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to55MHz    NO.4
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_1CH_40to40MHz    NO.5
    14,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_50to75MHz    NO.6
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to50MHz    NO.7
    28,           //E_PNL_SUPPORTED_LPLL_LVDS_2CH_37_5to37_5MHz    NO.8
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_100to150MHz    NO.9
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to100MHz    NO.10
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_1CH_75to75MHz    NO.11
    7,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_100to150MHz    NO.12
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to100MHz    NO.13
    14,           //E_PNL_SUPPORTED_LPLL_HS_LVDS_2CH_75to75MHz    NO.14
    16,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to80MHz    NO.15
    16,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_8BIT_40to40MHz    NO.16
    8,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to150MHz    NO.17
    8,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_8BIT_75to75MHz    NO.18
    8,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to150MHz    NO.19
    8,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_8BIT_75to75MHz    NO.20
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_66_67to80MHz    NO.21
    24,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to66_67MHz    NO.22
    24,           //E_PNL_SUPPORTED_LPLL_MINILVDS_1BK_3PAIR_6BIT_40to40MHz    NO.23
    6,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_133_33to150MHz    NO.24
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to133_33MHz    NO.25
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_3PAIR_6BIT_75to75MHz    NO.26
    6,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_133_33to150MHz    NO.27
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to133_33MHz    NO.28
    12,           //E_PNL_SUPPORTED_LPLL_MINILVDS_2BK_6PAIR_6BIT_75to75MHz    NO.29
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_115to150MHz    NO.30
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to115MHz    NO.31
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_2PAIR_FHD_75to75MHz    NO.32
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_115to150MHz    NO.33
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to115MHz    NO.34
    8,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_4PAIR_FHD_75to75MHz    NO.35
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to300MHz    NO.36
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_FHD_150to150MHz    NO.37
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_230to300MHz    NO.38
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to230MHz    NO.39
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_FHD_150to150MHz    NO.40
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to300MHz    NO.41
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_16PAIR_UHD_150to150MHz    NO.42
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_200to300MHz    NO.43
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to200MHz    NO.44
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_16PAIR_UHD_150to150MHz    NO.45
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to300MHz    NO.46
    4,           //E_PNL_SUPPORTED_LPLL_EPI_24_10BIT_12PAIR_UHD_150to150MHz    NO.47
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to300MHz    NO.48
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_12PAIR_UHD_150to150MHz    NO.49
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_240to300MHz    NO.50
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to240MHz    NO.51
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_8PAIR_EPI3G_UHD_150to150MHz    NO.52
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_180to300MHz    NO.53
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to180MHz    NO.54
    4,           //E_PNL_SUPPORTED_LPLL_EPI_28_8BIT_6PAIR_EPI3G_V17_150to150MHz    NO.55
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to300MHz    NO.56
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_24_10BIT_12PAIR_150to150MHz    NO.57
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to300MHz    NO.58
    4,           //E_PNL_SUPPORTED_LPLL_CMPI_27_8BIT_12PAIR_150to150MHz    NO.59
    25,           //E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to300MHz    NO.60
    25,           //E_PNL_SUPPORTED_LPLL_USI_T_10BIT_12PAIR_150to150MHz    NO.61
    4,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to300MHz    NO.62
    4,           //E_PNL_SUPPORTED_LPLL_USI_T_8BIT_12PAIR_150to150MHz    NO.63
    5,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to300MHz    NO.64
    5,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_6PAIR_DUAL_150to150MHz    NO.65
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_200to300MHz    NO.66
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to200MHz    NO.67
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_6PAIR_DUAL_150to150MHz    NO.68
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to300MHz    NO.69
    4,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_12PAIR_150to150MHz    NO.70
    5,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to300MHz    NO.71
    5,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_12PAIR_150to150MHz    NO.72
    9,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_199_2to300MHz    NO.73
    9,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to199_2MHz    NO.74
    9,           //E_PNL_SUPPORTED_LPLL_ISP_10BIT_8PAIR_150to150MHz    NO.75
    3,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_249to300MHz    NO.76
    9,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to249MHz    NO.77
    9,           //E_PNL_SUPPORTED_LPLL_ISP_8BIT_8PAIR_150to150MHz    NO.78
    10,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to300MHz    NO.79
    10,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_10BIT_150to150MHz    NO.80
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to150MHz    NO.81
    20,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_10BIT_75to75MHz    NO.82
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to75MHz    NO.83
    40,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_10BIT_37_5to37_5MHz    NO.84
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_200to300MHz    NO.85
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to200MHz    NO.86
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_8CH_8BIT_150to150MHz    NO.87
    15,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_100to150MHz    NO.88
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to100MHz    NO.89
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_4CH_8BIT_75to75MHz    NO.90
    30,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_50to75MHz    NO.91
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to50MHz    NO.92
    60,           //E_PNL_SUPPORTED_LPLL_VBY1_2CH_8BIT_37_5to37_5MHz    NO.93
};

#endif //_LPLL_TBL_H_
