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
#define _PANEL_C_
#include "mdrv_mfc_platform.h"
#include "mdrv_mfc.h"
#include "mdrv_mfc_fb.h"

#if(CODESIZE_SEL == CODESIZE_ALL)

extern code U8 tInitializeColorMatrix[];
extern U8 tInitializeColorMatrix_count;

extern code MST_MFC_RegUnitType_t tInitializeTcon23b[];
extern code  MST_MFC_RegUnitType_t tInitializeScTop2[];
extern code U8 tOverDrive[];
#if 0
//_RSDS
code MST_MFC_RegUnitType_t tInitializeTcon22a[]=
{
	{0x22A0, 0xb2},
	{0x22A2, 0xd4},
	{0x22A6, 0x05},
	{0x22A8, 0x05},
	{0x22AA, 0x02},
	{0x22AC, 0x11},
	{0x22AE, 0x5a},

{_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeTcon23a[]=
{
	{0x2302, 0x7b},
	{0x2304, 0x30},
	{0x2306, 0x06},
	{0x2308, 0x80},
	{0x230A, 0x0c},
	{0x230E, 0x50},

	{0x2310, PANEL_WIDTH},
	{0x2311, PANEL_WIDTH>>8},
	{0x2314, PANEL_HEIGHT},
	{0x2315, PANEL_HEIGHT>>8},
	{0x2318, PANEL_HTOTAL},
	{0x2319, PANEL_HTOTAL>>8},
	{0x231C, PANEL_VTOTAL},
	{0x231D, PANEL_VTOTAL>>8},
	{0x231E, 0x80},

	{0x2320, TP_START},
	{0x2321, TP_START>>8},
	{0x2322, TP_WIDTH},
	{0x2323, TP_WIDTH>>8},
	{0x2324, 0x30},
	{0x2326, STV_START},
	{0x2327, STV_START>>8},

	{0x232A, CPV_START},
	{0x232B, CPV_START>>8},
	{0x232E, CPV_WIDTH},
	{0x232F, CPV_WIDTH>>8},

	{0x2332, OE_START},
	{0x2333, OE_START>>8},
	{0x2334, OE_WIDTH},
	{0x2335, OE_WIDTH>>8},
	{0x2336, 0x00},
	{0x2338, POL_START},
	{0x2339, POL_START>>8},
#if (PANEL_CHANNEL==_SINGLE)
	{0x233C, PANEL_WIDTH},
	{0x233D, PANEL_WIDTH>>8},
#elif (PANEL_CHANNEL==_DUAL)
	{0x233C, PANEL_WIDTH>>1},
	{0x233D, PANEL_WIDTH>>9},
#else
	{0x233C, PANEL_WIDTH>>2},
	{0x233D, PANEL_WIDTH>>10},
#endif
	{0x233E, 0xf0},

	{0x2340, 0x00}, //
	{0x2341, 0x00}, //
	{0x2342, 0xe2}, //
	{0x2343, 0x02}, //
	{0x2344, 0x82}, //
	{0x2345, 0x01}, //
	{0x2348, 0xd4}, //
	{0x2349, 0x00}, //
	{0x234A, 0x20}, //
	{0x234B, 0x02}, //
	{0x234C, 0x21}, //
	{0x234E, 0x80}, //

	{0x2360, 0x1a},
	{0x2361, 0x02},
	{0x2362, 0x1a},
	{0x2363, 0x02},
	{0x2364, 0xc7},
	{0x2365, 0x00},
	{0x2368, 0x00},
	{0x2369, 0x00},
	{0x236A, 0xff},
	{0x236B, 0x02},
	{0x236C, 0x31},
	{0x236E, 0x05},

	{0x2382, 0xe2}, //
	{0x2383, 0x02}, //
	{0x2384, 0x82}, //
	{0x2385, 0x01}, //
	{0x2388, 0xd4}, //
	{0x2389, 0x00}, //
	{0x238A, 0x22}, //
	{0x238B, 0x02}, //
	{0x238C, 0x00}, //
	{0x238D, 0xff}, //
	{0x238E, 0x05}, //
	{0x238F, 0x80}, //

	{0x2392, 0xd4}, //
	{0x2393, 0x00}, //
	{0x2394, 0xd4}, //
	{0x2395, 0x00}, //
	{0x2396, 0x08}, //
	{0x2398, 0xd4}, //
	{0x2399, 0x00}, //

	{0x23A0, 0x80}, //
	{0x23A2, 0x24}, //
	{0x23A3, 0x02}, //
	{0x23A4, 0x26}, //
	{0x23A5, 0x02}, //
	{0x23A6, 0x28}, //
	{0x23A7, 0x02}, //

	{0x23E2, 0x00},
	{0x23E3, 0x00},

{_END_OF_TBL_, _END_OF_TBL_},
};
#endif

//_MINI_LVDS
code MST_MFC_RegUnitType_t tInitializeTcon22b_Comm[]=
{
	{0x22A0, 0x01}, //DEOUT delay
	{0x22A1, 0x00},
	{0x22A2, 0x00}, //DEOUT_AHEAD delay
	{0x22A3, 0x11},
{_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeTcon23b_Comm[]=
 {
    //common
    //{0x2301, 0x00},
	{0x2302, 0x7c},  // {8'h0, skew_reg, ivmd_on, bist_hw_set, swap_fs, fsmode, dskew} = { - 01111, 3'h4};
	{0x2303, 0x00},
	{0x2304, 0x50},  // {8'h0, f2line, fcol, f1a2line, age_8bit, 4'h0} = {- 0001 - }
	{0x2305, 0x00},
	{0x230A, 0x2d},
	/*{0x2310, 0xc0},  // {5'h0, hres} = {- 11'h556 }
	{0x2311, 0x03},
	{0x2314, 0x38},  // {5'h0, vres } = {- 11'h300 }
	{0x2315, 0x04},
	{0x2318, 0x98},  // {4'h0, htot} = {- 12'h698 }
	{0x2319, 0x08},*/
	{0x231A, 0x20},  // {10'h0, cout_type, inmod_pad_sel, 4'h0} =  {- 00 -}
	{0x231B, 0x00},
	///{0x231C, 0x65},  // {5'h0, vtot } = {- 12'h326 }
	///{0x231D, 0x04},
	{0x231E, 0xc0},  // {8'h0, tp_drv, sth_drv, 1'b0, pol_newtype, 4'h0} = {- 11 - 0 -};
	{0x231F, 0x00},
///////Tcon setting by panel start/////////
    {0x23E0, 0xa6}, //2370
    {0x23E1, 0x03},
	{0x23E4, 0x00}, //[15]en_minilvds [14:13]bit_flag [12:9]mini_channel_max
	{0x23E5, 0xac},
    {0x23E8, 0x20},//2374
	{0x23E9, 0x5b},
	{0x23F4, 0x00},
	{0x23F5, 0x40}, //GOE Mask
	{0x23F6, 0x08},
	{0x23F7, 0x00},
{_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeTcon23b_42[]=
{
	{0x233C, 0xe0}, // {5'h0, bank1_len[10:0]}
	{0x233D, 0x01},
	{0x2326, 0xd0}, //2313(GSP)
	{0x2327, 0x01},
	{0x2320, 0xdd},//2310(SOE)
	{0x2321, 0x03},
	{0x2322, 0x46},//2311
	{0x2323, 0x00},
	{0x2332, 0x14},//2319(GOE)
	{0x2333, 0x03},
	{0x2334, 0xD2},//231A
	{0x2335, 0x00},
	{0x232A, 0xa0},//2315(GSC)
	{0x232B, 0x03},
	{0x232E, 0x26},//2317
	{0x232F, 0x02},
	{0x2338, 0xC4},//231c(POL)
	{0x2339, 0x03},
	{0x23EC, 0x9F},//2376(FLK)
    {0x23ED, 0x02},
    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeTcon23b_47[]=
{
	{0x233C, 0xe0},  // {5'h0, bank1_len[10:0]}
	{0x233D, 0x01},
	{0x2326, 0xd0},//2313
	{0x2327, 0x01},
    {0x2320, 0xd9},//2310
	{0x2321, 0x03},
	{0x2322, 0x4f},//2311
	{0x2323, 0x00},
	{0x2332, 0x21},//2319
	{0x2333, 0x03},
	{0x2334, 0x04},//231A
	{0x2335, 0x01},
	{0x232A, 0xa6},//2315
	{0x232B, 0x03},
	{0x232E, 0x26}, //2317  //0x73
	{0x232F, 0x02},
	{0x2338, 0xC4},//231c
	{0x2339, 0x03},
	{0x23EC, 0x85},//2376
    {0x23ED, 0x02},
    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeTcon23b_55[]=
{
	{0x233C, 0xe0},  // {5'h0, bank1_len[10:0]}
	{0x233D, 0x01},
	{0x2326, 0xd0},//2313
	{0x2327, 0x01},
    {0x2320, 0xFF},//2310
	{0x2321, 0x03},
	{0x2322, 0x46},//2311
	{0x2323, 0x00},
	{0x2332, 0xA6},//2319
	{0x2333, 0x03},
	{0x2334, 0x52},//231A
	{0x2335, 0x00},
	{0x232A, 0xd9},//2315
	{0x232B, 0x03},
	{0x232E, 0x73}, //2317  //0x73
	{0x232F, 0x02},
	{0x2338, 0xC4},//231c
	{0x2339, 0x03},
	{0x23EC, 0xFF},//2376
    {0x23ED, 0x01},
    {_END_OF_TBL_, _END_OF_TBL_},
};

//_MINI_LVDS_GIP
code MST_MFC_RegUnitType_t tInitializeTcon22c[]=
{
	{0x22A0, 0x01}, //DEOUT delay
	{0x22A1, 0x00},
	{0x22A2, 0x00}, //DEOUT_AHEAD delay
	{0x22A3, 0x11},
{_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeTcon23c[]=
{
    //common
    //{0x2301, 0x00},
	{0x2302, 0x7c},  // {8'h0, skew_reg, ivmd_on, bist_hw_set, swap_fs, fsmode, dskew} = { - 01111, 3'h4};
	{0x2303, 0x00},
	{0x2304, 0x50},  // {8'h0, f2line, fcol, f1a2line, age_8bit, 4'h0} = {- 0001 - }
	{0x2305, 0x00},
	{0x230A, 0x2d},
	{0x2310, 0xc0},  // {5'h0, hres} = {- 11'h556 }
	{0x2311, 0x03},
	{0x2314, 0x38},  // {5'h0, vres } = {- 11'h300 }
	{0x2315, 0x04},
	{0x2318, 0x98},  // {4'h0, htot} = {- 12'h698 }
	{0x2319, 0x08},
	{0x231A, 0x20},  // {10'h0, cout_type, inmod_pad_sel, 4'h0} =  {- 00 -}
	{0x231B, 0x00},
	{0x231C, 0x65},  // {5'h0, vtot } = {- 12'h326 }
	{0x231D, 0x04},
	{0x231E, 0xc0},  // {8'h0, tp_drv, sth_drv, 1'b0, pol_newtype, 4'h0} = {- 11 - 0 -};
	{0x231F, 0x00},
	{0x2320, 0xdd},//0xd9,  // {4'h0, tpst } = {- 12'h2b5 }
	{0x2321, 0x03},
	{0x2322, 0x46},//0x44,  // {8'h0, tppw} = {- 8'h63 }
	{0x2323, 0x00},
	{0x2326, 0xd0},  // {4'h0, stvst} = {- 12'h1c0 }
	{0x2327, 0x01},
	{0x232A, 0xa9},  // {4'h0, cpvst} = {- 12'h2e2 }
	{0x232B, 0x03},
	{0x232E, 0x73},  // {4'h0, cpvpw} = {- 12'h1a5 }
	{0x232F, 0x02},
	{0x2332, 0x52},  // {4'h0, oest} = {- 12'h28b }
	{0x2333, 0x03},
	{0x2334, 0xd2},  // {6'h0, oepw} = {- 10'h95 }
	{0x2335, 0x00},
	{0x2338, 0xC4},//0x00,  // {4'h0, poltg} = {- 12'h256} ;
	{0x2339, 0x03},//0x04,
	{0x233C, 0xe0},  // {5'h0, bank1_len[10:0]} = {- 11'h2ab}
	{0x233D, 0x01},
    {0x23E0, 0xa6},
    {0x23E1, 0x03},
	{0x23E4, 0x00}, //[15]en_minilvds [14:13]bit_flag [12:9]mini_channel_max
	{0x23E5, 0xac},
	{0x23E8, 0x20}, //HEMAN
	{0x23E9, 0x5B},
    {0x23EC, 0xff},
    {0x23ED, 0x01},
	{0x23F5, 0x40},
	{0x23F6, 0x08},
	{0x23F7, 0x00},
    //gip
	//gpo0 //GCLK1
	{0x234E, 0x20},//
	{0x234F, 0x00},
	{0x2348, 0x13},//[15]frame_tog_md,[14]line_tog_md,[13]first_2h_mode,[12]wave_polarity,[11:0]vst
    //{0x2349, 0x40},
	{0x234A, 0x4b}, //[11:0]vend
	{0x234B, 0x04},
	{0x2340, 0x4b},//[15:12]n_frame_tog_l4[11:0]hst2
	{0x2341, 0x04},
	{0x2342, 0x4b},//[15:12]n_frame_tog_h4[11:0]hst1
	{0x2343, 0x04},
	{0x2344, 0x1f}, //[15:12]n_line_tog_l4[11:0]hpw1
	{0x2345, 0x23},
	{0x2346, 0x1f}, //[15:12]n_line_tog_h4[11:0]hpw2
	{0x2347, 0x03},
	{0x23A8, 0x4B},//hst3
	{0x23A9, 0x04},
	{0x23AA, 0x1f}, //hpw3
	{0x23AB, 0x03},
	//gpo1 //GCLK2
	{0x235E, 0x20},//
	{0x235F, 0x00},
	{0x2358, 0x14},//[15]frame_tog_md,[14]line_tog_md,[13]first_2h_mode,[12]wave_polarity,[11:0]vst
	{0x2359, 0x40},
	{0x235A, 0x4c}, //[11:0]vend
	{0x235B, 0x04},
	{0x2350, 0x4B},//[15:12]n_frame_tog_l4[11:0]hst2
	{0x2351, 0x04},
	{0x2352, 0x4B},//[15:12]n_frame_tog_h4[11:0]hst1
	{0x2353, 0x04},
	{0x2354, 0x1f}, //[15:12]n_line_tog_l4[11:0]hpw1
	{0x2355, 0x23},
	{0x2356, 0x1f}, //[15:12]n_line_tog_h4[11:0]hpw2
	{0x2357, 0x03},
	{0x23AC, 0x4B},//hst3
	{0x23AD, 0x04},
	{0x23AE, 0x1f}, //hpw3
	{0x23AF, 0x03},
	//gpo2 //GCLK3
	{0x236E, 0x20},//
	{0x236F, 0x00},
	{0x2368, 0x15},//[15]frame_tog_md,[14]line_tog_md,[13]first_2h_mode,[12]wave_polarity,[11:0]vst
	{0x2369, 0x40},
	{0x236A, 0x4d}, //[11:0]vend
	{0x236B, 0x04},
	{0x2360, 0x4b},//[15:12]n_frame_tog_l4[11:0]hst2
	{0x2361, 0x04},
	{0x2362, 0x4b},//[15:12]n_frame_tog_h4[11:0]hst1
	{0x2363, 0x04},
	{0x2364, 0x1f}, //[15:12]n_line_tog_l4[11:0]hpw1
	{0x2365, 0x23},
	{0x2366, 0x1f}, //[15:12]n_line_tog_h4[11:0]hpw2
	{0x2367, 0x03},
	{0x23B0, 0x4b},//hst3
	{0x23B1, 0x04},
	{0x23B2, 0x1f}, //hpw3
	{0x23B3, 0x03},
	//gpo3 //GCLK4
	{0x237E, 0x20},//
	{0x237F, 0x00},
    //{0x2378, 0x10},//[15]frame_tog_md,[14]line_tog_md,[13]first_2h_mode,[12]wave_polarity,[11:0]vst
	{0x2379, 0x40},
	{0x237A, 0x4e}, //[11:0]vend
	{0x237B, 0x04},
	{0x2370, 0x4b},//[15:12]n_frame_tog_l4[11:0]hst2
	{0x2371, 0x04},
	{0x2372, 0x4b},//[15:12]n_frame_tog_h4[11:0]hst1
	{0x2373, 0x04},
	{0x2374, 0x1f}, //[15:12]n_line_tog_l4[11:0]hpw1
	{0x2375, 0x23},
	{0x2376, 0x1f}, //[15:12]n_line_tog_h4[11:0]hpw2
	{0x2377, 0x03},
	{0x23B4, 0x4b},//hst3
	{0x23B5, 0x04},
	{0x23B6, 0x1f}, //hpw3
	{0x23B7, 0x03},
	//gpo4 //GCLK5
	{0x238C, 0x55},// Output Tristate of 4 ,5 ,6 ,7
	{0x238D, 0x00},
	{0x238E, 0x00},//
	{0x238F, 0x00},
    //{0x2388, 0x11},//[15]frame_tog_md,[14]line_tog_md,[13]first_2h_mode,[12]wave_polarity,[11:0]vst
	{0x2389, 0x40},
	{0x238A, 0x49}, //[11:0]vend
	{0x238B, 0x04},
	{0x2380, 0x4b},//[15:12]n_frame_tog_l4[11:0]hst2
	{0x2381, 0x04},
	{0x2382, 0x4b},//[15:12]n_frame_tog_h4[11:0]hst1
	{0x2383, 0x04},
	{0x2384, 0x1f}, //[15:12]n_line_tog_l4[11:0]hpw1
	{0x2385, 0x23},
	{0x2386, 0x1f}, //[15:12]n_line_tog_h4[11:0]hpw2
	{0x2387, 0x03},
	{0x23B8, 0x4b},//hst3
	{0x23B9, 0x04},
	{0x23BA, 0x1f}, //hpw3
	{0x23BB, 0x03},
	//gpo5 //GCLK6
    //{0x2392, 0x12},//[15]frame_tog_md,[14]line_tog_md,[13]first_2h_mode,[12]wave_polarity,[11:0]vst
	{0x2393, 0x40},
	{0x23A2, 0x4a}, //[11:0]vend
	{0x23A3, 0x04},
	{0x23BC, 0x4b},//[15:12]n_frame_tog_l4[11:0]hst1
	{0x23BD, 0x04},
	{0x23BE, 0x1f}, //[15:12]n_frame_tog_h4[11:0]hst2
	{0x23BF, 0x03},
	{0x23D4, 0x4b},//[15:12]n_line_tog_l4[11:0]hpw1
	{0x23D5, 0x24},
	{0x23D6, 0x1f}, //[15:12]n_line_tog_h4[11:0]hpw2
	{0x23D7, 0x03},
	{0x23D8, 0x4b},//hst3
	{0x23D9, 0x04},
	{0x23DA, 0x1f}, //hpw3
	{0x23DB, 0x03},
	//gpo6 //VST
    //{0x2394, 0x0F},//[14]line_tog_md,[12]wave_polarity,[11:0]vst
	{0x2395, 0x40},
    //{0x23A4, 0x10},//[11:0]vend
    //{0x23A5, 0x40},
	{0x23DC, 0x23},//[15:12]n_line_tog_l4[11:0]hpw1
	{0x23DD, 0x10},
	{0x23DE, 0x39},//[15:12]n_line_tog_h4[11:0]hst1
	{0x23DF, 0x03},
	//gpo5b //VDD_EVEN
	{0x23C8, 0x10},
	{0x23C9, 0x00},
	{0x23C4, 0x00}, //[15]frame_tog_md,[12]wave_polarity,[11:0]vst
	{0x23C5, 0x80},
	{0x23C6, 0x00},
	{0x23C7, 0x00},
	//0x23a5 0x0000 ; //[11:0]vend
	{0x23C0, 0x80},//[15:12]n_frame_tog_l4[11:0]hpw1
	{0x23C1, 0x80},
	{0x23C2, 0x20}, //[15:12]n_frame_tog_h4[11:0]hst1
	{0x23C3, 0xD0},
	//gpo6b //VDD_ODD
	{0x23D2, 0x10},
	{0x23D3, 0x00},
	{0x23CE, 0x00}, //[15]frame_tog_md,[12]wave_polarity,[11:0]vst
	{0x23CF, 0x90},
	{0x23D0, 0x00},
	{0x23D1, 0x00},
	//0x23a5 0x0000 ; //[11:0]vend
	{0x23CA, 0x20},//[15:12]n_frame_tog_l4[11:0]hpw1
	{0x23CB, 0x80},
	{0x23CC, 0x80},//[15:12]n_frame_tog_h4[11:0]hst1
	{0x23CD, 0xD0},
	{0x2312, 0xC0}, //En
	{0x2300, 0xF8},
	{0x2338, 0xC4}, //poltg
	{0x2339, 0x03},
	{0x2320, 0xdd}, //tpst
	{0x2321, 0x03},
	{0x2322, 0x78},//tppw
//Gip setting //don't change process////George recommnat //j081031
   	{0x23C8, 0x00},
	{0x23D2, 0x00},
    {0x23F4, 0x00},
{_END_OF_TBL_, _END_OF_TBL_},
};


void msInitializeColorMatrix(void)
{
	U8 i;

	MDrv_MFC_WriteBit(0x20C0,  gmfcSysInfo.u8PanelCSC, _BIT0); // [0]:CSC [1]:dither [2]:round
	MDrv_MFC_WriteByte(0x3074, 0x00); // disable dither 6bit enable
	MDrv_MFC_WriteByte(0x3075, 0x2d);
	for (i=0; i<tInitializeColorMatrix_count/*sizeof(tInitializeColorMatrix)*/; i++) // 0x3002 ~ 0x301d
	{
	    if((0x3002+i)==0x3017)
        	MDrv_MFC_WriteByteMask(0x3002+i, tInitializeColorMatrix[i], 0x7F);
	    else
        	MDrv_MFC_WriteByte(0x3002+i, tInitializeColorMatrix[i]);
    }

    //printf("\r\nmsInitializeColorMatrix()");
}

void msInitializeTcon(void)
{
    //if(gmfcSysInfo.u8PanelType == _MINI_LVDS_GIP)
    //    MDrv_MFC_WriteByte(0x1E0F, 0x10); //HEMAN

	//MDrv_MFC_WriteBit(0x2330, 1, _BIT4);//GOE polarity swap-----I-Chang 0829
	//MDrv_MFC_WriteByte(0x23F0, 0xC0);//reg_ctrl_low_sel and reg_de_delay-----I-Chang 0909
	MDrv_MFC_WriteBit(0x23F0, 1, _BIT3);//reg_ctrl_low_sel-----I-Chang 0909
	MDrv_MFC_WriteBit(0x23F0, 1, _BIT2);//reg_de_delay-----I-Chang 0909
	MDrv_MFC_WriteBit(0x23F0, 0, _BIT1);//reg_de_delay-----I-Chang 0909
	MDrv_MFC_WriteBit(0x23F0, 0, _BIT0);//reg_de_delay-----I-Chang 0909
	MDrv_MFC_WriteBit(0x2313, 1, _BIT7);//reg_tp_md_sel-----I-Chang 0909
	MDrv_MFC_WriteBit(0x230F, 1, _BIT3);//Request by Bryan, control load-----I-Chang 0901

    if(gmfcSysInfo.u8PanelType != _MINI_LVDS_GIP && gmfcSysInfo.u8PanelType != _MINI_LVDS_GIP_V5)
		MDrv_MFC_WriteBit(0x238C, 0, _BIT0); //Output Enable FLK //j081031

	MDrv_MFC_WriteByte(0x2398, 0x00);
	MDrv_MFC_WriteByte(0x2399, 0x00);
	MDrv_MFC_WriteByte(0x23A6, 0xFF);
	MDrv_MFC_WriteByte(0x23A7, 0x00);
	MDrv_MFC_WriteByte(0x23E0, 0xFF);
	MDrv_MFC_WriteByte(0x23E1, 0x00);
	MDrv_MFC_WriteByte(0x23EC, 0xFF);
	MDrv_MFC_WriteByte(0x23ED, 0x00);
	//msWriteBit(0x2330, 0, _BIT4);//GOE polarity swap-----I-Chang 0829
	MDrv_MFC_WriteBit(0x2052, 0, _BIT0);//Only for visit; PC mode on-----I-Chang 0829
	//printf("\r\nmsInitializeTcon()");
	MDrv_MFC_WriteBit(0x2300, gmfcSysInfo.u8PanelBlankCPVC, _BIT6);
	MDrv_MFC_WriteBit(0x2300, gmfcSysInfo.u8PanelBlankOEC, _BIT5);
	MDrv_MFC_WriteBit(0x2300, gmfcSysInfo.u8PanelBlankTPC, _BIT4);
	MDrv_MFC_WriteBit(0x2300, gmfcSysInfo.u8PanelBlankSTHC, _BIT3);

	/* if(gmfcSysInfo.u8PanelType==_RSDS)
	{
		MDrv_MFC_WriteRegsTbl(0x2200, tInitializeTcon22a); // initialize all of bank
		MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23a); // initialize all of bank
	}
	else */if(gmfcSysInfo.u8PanelType==_MINI_LVDS)
	{
		MDrv_MFC_WriteRegsTbl(0x2200, tInitializeTcon22b_Comm); // initialize all of bank
		MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23b_Comm); // initialize all of bank

		MDrv_MFC_Write2Bytes(0x2310, gmfcSysInfo.u16Width/2);
		MDrv_MFC_Write2Bytes(0x2314, gmfcSysInfo.u16Height);
		MDrv_MFC_Write2Bytes(0x2318, gmfcSysInfo.u16HTotal);
		MDrv_MFC_Write2Bytes(0x231C, gmfcSysInfo.u16VTotal);


		#if(CODEBASE_SEL == CODEBASE_51)
	    	MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23b); //each panel
	    #elif(PANEL_TYPE_SEL == PNL_LCEAll)
			if(MDrv_MFC_ReadByte(0x1E48) == 1)
			{
				MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23b_42);
			}
			else if(MDrv_MFC_ReadByte(0x1E48) == 2)
			{
				MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23b_47);
			}
        #else//55" need to refine the code
            MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23b_55);
		#endif
	}
    else if(gmfcSysInfo.u8PanelType==_MINI_LVDS_GIP ||gmfcSysInfo.u8PanelType==_MINI_LVDS_GIP_V5)
	{
		MDrv_MFC_WriteRegsTbl(0x2200, tInitializeTcon22c); // initialize all of bank
		MDrv_MFC_WriteRegsTbl(0x2300, tInitializeTcon23c); // initialize all of bank

        if(gmfcSysInfo.u8PanelType==_MINI_LVDS_GIP)//V4
        {
            MDrv_MFC_WriteByte(0x2349, 0x60);
            MDrv_MFC_WriteByte(0x2378, 0x16);
            MDrv_MFC_WriteByte(0x2388, 0x17);
            MDrv_MFC_WriteByte(0x2392, 0x18);
            MDrv_MFC_WriteByte(0x2394, 0x14);
            MDrv_MFC_WriteByte(0x23A4, 0x15);
            MDrv_MFC_WriteByte(0x23A5, 0x00);
        }
        else//V5
        {
            MDrv_MFC_WriteByte(0x2349, 0x40);
            MDrv_MFC_WriteByte(0x2378, 0x10);
            MDrv_MFC_WriteByte(0x2388, 0x11);
            MDrv_MFC_WriteByte(0x2392, 0x12);
            MDrv_MFC_WriteByte(0x2394, 0x0F);
            MDrv_MFC_WriteByte(0x23A4, 0x10);
            MDrv_MFC_WriteByte(0x23A5, 0x40);

            // Fitch T cont V5 setting
            // 20090810
            MDrv_MFC_WriteBit(0x3240,  1, _BIT7);
            // 20090812
            //MDrv_MFC_WriteByte(0x3253, 0xC0);
            //MDrv_MFC_WriteByte(0x3252, 0xC0);
            MDrv_MFC_WriteByte(0x3276, 0xFC);
            MDrv_MFC_WriteByte(0x1E3F, 0x60);
            MDrv_MFC_WriteByte(0x1E40, 0xFC);
            MDrv_MFC_WriteByte(0x2540, 0x05);
            // 20090813
            MDrv_MFC_WriteByte(0x23DC, 0x00);
            MDrv_MFC_WriteByte(0x23DD, 0x00);
            MDrv_MFC_WriteByte(0x23DE, 0x7E);
            MDrv_MFC_WriteByte(0x23DF, 0x03);
            MDrv_MFC_WriteByte(0x23C0, 0x01);
            MDrv_MFC_WriteByte(0x23C1, 0x80);
            MDrv_MFC_WriteByte(0x23C2, 0xE4);
            MDrv_MFC_WriteByte(0x23C3, 0xD0);
            MDrv_MFC_WriteByte(0x23CA, 0x64);
            MDrv_MFC_WriteByte(0x23CB, 0x80);

            MDrv_MFC_WriteByte(0x3250, (0x4C|MOD_POWER_ON_AFTER_INIT));
            MDrv_MFC_WriteByte(0x324E, 0x0F);
            MDrv_MFC_WriteByte(0x324F, 0x31);

            MDrv_MFC_WriteByte(0x2505, 0x0D);
            MDrv_MFC_WriteByte(0x2504, 0x3F);
            MDrv_MFC_WriteByte(0x2507, 0x86);
            MDrv_MFC_WriteByte(0x2506, 0x2F);
            MDrv_MFC_WriteByte(0x2509, 0x04);
            MDrv_MFC_WriteByte(0x2508, 0x00);
            MDrv_MFC_WriteByte(0x250B, 0x0D);
            MDrv_MFC_WriteByte(0x250A, 0x3F);
            MDrv_MFC_WriteByte(0x250D, 0x49);
            MDrv_MFC_WriteByte(0x250C, 0x7E);
            MDrv_MFC_WriteByte(0x250F, 0x04);
            MDrv_MFC_WriteByte(0x250E, 0x00);
            MDrv_MFC_WriteByte(0x2541, 0x00);
            MDrv_MFC_WriteByte(0x2540, 0x0F);
            MDrv_MFC_WriteByte(0x2543, 0x00);
            MDrv_MFC_WriteByte(0x2542, 0x0D);
            MDrv_MFC_WriteByte(0x2555, 0x86);
            MDrv_MFC_WriteByte(0x2554, 0x9F);
            MDrv_MFC_WriteByte(0x2557, 0x00);
            MDrv_MFC_WriteByte(0x2556, 0x01);
        }

		//*****HEMAN*******//
		//Gip setting //don't change process////Bryan recommnat //j081031
		MDrv_MFC_WriteByte(0x3270, 0x90);
        mfcSleepMs(20);
		MDrv_MFC_WriteByte(0x238C, 0x51);
        mfcSleepMs(25);
		MDrv_MFC_WriteByte(0x238C, 0x00);
		//MDrv_MFC_WriteByte(0x3230, 0x00);
		//MDrv_MFC_WriteByte(0x3231, 0x00);
		//MDrv_MFC_WriteByte(0x3232, 0x00);
		//MDrv_MFC_WriteByte(0x3233, 0x00);
		MDrv_MFC_WriteByte(0x234E, 0x00);
		MDrv_MFC_WriteByte(0x235E, 0x00);
		MDrv_MFC_WriteByte(0x236E, 0x00);
		MDrv_MFC_WriteByte(0x237E, 0x00);
		//MDrv_MFC_WriteByte(0x238C, 0x15);
		//MDrv_MFC_WriteByte(0x238C, 0x05);

        //MDrv_MFC_WriteByte(0x1E0E, 0x00);
		//MDrv_MFC_WriteByte(0x1E0F, 0x00);
	}
    if(S7M==0) MDrv_MFC_WriteByte(0x2301, 0x00); //not ttl

	MDrv_MFC_WriteByte(0x23F9, 0x20);
	MDrv_MFC_WriteBit(0x1E0F, 0, _BIT4);
}

#if (OD_MODE_SEL != OD_MODE_OFF)
void MDrv_MFC_InitializeOD(U8* pODTbl)
{

	U8 ucVal;
	U32 wCount;
    U8 ucTARGET;

	// od_top clock enable
	MDrv_MFC_WriteByte(0x2802, 0x0e); // sram io enable
	MDrv_MFC_WriteByte(0x2803, 0x00); // sram io enable

    // Uncompressed mode
    ucTARGET=*(pODTbl+9);// 10th
    for (wCount=0; wCount<272; wCount++)
	{
		MDrv_MFC_WriteByte(0x2806, (wCount == 9)?ucTARGET:(ucTARGET ^ *(pODTbl+wCount)));
		MDrv_MFC_Write2Bytes(0x2804, wCount|0x8000);
		while(_bit7_(MDrv_MFC_ReadByte(0x2805)));

		//MDrv_MFC_Write2Bytes(0x2804, wCount|0x4000);
		//printf(" ,-[%x]", MDrv_MFC_ReadByte(0x2808));
		// while(_bit6_(MDrv_MFC_ReadByte(0x2805)));
    }

    ucTARGET=*(pODTbl+272+19);// 20th
    for (wCount=0; wCount<272; wCount++)
    {
        MDrv_MFC_WriteByte(0x280C, (wCount == 19)?ucTARGET:(ucTARGET ^ *(pODTbl+272+wCount)));
        MDrv_MFC_Write2Bytes(0x280A, wCount|0x8000);
    	while(_bit7_(MDrv_MFC_ReadByte(0x280B)));
    }

    ucTARGET=*(pODTbl+272*2+29);// 30th
    for (wCount=0; wCount<256; wCount++)
	{
        MDrv_MFC_WriteByte(0x2812, (wCount == 29)?ucTARGET:(ucTARGET ^ *(pODTbl+272*2+wCount)));
        MDrv_MFC_Write2Bytes(0x2810, wCount|0x8000);
	    	while(_bit7_(MDrv_MFC_ReadByte(0x2811)));
	}

    ucTARGET=*(pODTbl+272*2+256+39);// 40th
    for (wCount=0; wCount<256; wCount++)
    {
        MDrv_MFC_WriteByte(0x2818, (wCount == 39)?ucTARGET:(ucTARGET ^ *(pODTbl+272*2+256+wCount)));
        MDrv_MFC_Write2Bytes(0x2816, wCount|0x8000);
        while(_bit7_(MDrv_MFC_ReadByte(0x2817)));
	}

	MDrv_MFC_WriteByte(0x2802, 0x00); // sram io disable
	MDrv_MFC_WriteByte(0x2803, 0x00); // sram io disable
	MDrv_MFC_WriteByte(0x2823, 0x5f); //[3:0] od_user_weight, [7:4] b_weight
	MDrv_MFC_WriteByte(0x2824, 0x0c); // [7:0] od active threshold
	// [7:0] Even request base address low byte
	MDrv_MFC_WriteByte(0x282A, (U8)(gmfcMiuBaseAddr.u32OdBaseEven>>4));
	// [7:0] Even request base address med byte
	MDrv_MFC_WriteByte(0x282B, (U8)((gmfcMiuBaseAddr.u32OdBaseEven>>4)>>8));
	// [7:0] Even request base address high byte
	MDrv_MFC_WriteByte(0x282C, (U8)((gmfcMiuBaseAddr.u32OdBaseEven>>4)>>16));

	// [7:0] request limit address low byte
	MDrv_MFC_WriteByte(0x282E, (U8)(gmfcMiuBaseAddr.u32OdLimitEven>>4));
	// [7:0] request limit address med byte
	MDrv_MFC_WriteByte(0x282F, (U8)((gmfcMiuBaseAddr.u32OdLimitEven>>4)>>8));
	// [7:0] request limit address high byte
	MDrv_MFC_WriteByte(0x2830, (U8)((gmfcMiuBaseAddr.u32OdLimitEven>>4)>>16));

	// [7:0] reg_od_wadr_max_limit low byte
	MDrv_MFC_WriteByte(0x2872, (U8)(gmfcMiuBaseAddr.u32OdSizehalf>>4));
	// [7:0] reg_od_wadr_max_limit med byte
	MDrv_MFC_WriteByte(0x2873, (U8)((gmfcMiuBaseAddr.u32OdSizehalf>>4)>>8));
	// [7:0] reg_od_wadr_max_limit high byte
	MDrv_MFC_WriteByte(0x2874, (U8)((gmfcMiuBaseAddr.u32OdSizehalf>>4)>>16));

	// [7:0] reg_od_radr_max_limit low byte
	MDrv_MFC_WriteByte(0x2876, (U8)(gmfcMiuBaseAddr.u32OdSizehalf>>4));
	// [7:0] reg_od_radr_max_limit med byte
	MDrv_MFC_WriteByte(0x2877, (U8)((gmfcMiuBaseAddr.u32OdSizehalf>>4)>>8));
	// [7:0] reg_od_radr_max_limit high byte
	MDrv_MFC_WriteByte(0x2878, (U8)((gmfcMiuBaseAddr.u32OdSizehalf>>4)>>16));

	// [7:0] Odd request base address low byte
	MDrv_MFC_WriteByte(0x288E, (U8)(gmfcMiuBaseAddr.u32OdBaseOdd>>4));
	// [7:0] Odd request base address med byte
	MDrv_MFC_WriteByte(0x288F, (U8)((gmfcMiuBaseAddr.u32OdBaseOdd>>4)>>8));
	// [7:0] Odd request base address high byte
	MDrv_MFC_WriteByte(0x2890, (U8)((gmfcMiuBaseAddr.u32OdBaseOdd>>4)>>16));

	// [7:0] request limit address low byte
	MDrv_MFC_WriteByte(0x2891, (U8)(gmfcMiuBaseAddr.u32OdLimitOdd>>4));
	// [7:0] request limit address med byte
	MDrv_MFC_WriteByte(0x2892, (U8)((gmfcMiuBaseAddr.u32OdLimitOdd>>4)>>8));
	// [7:0] request limit address high byte
	MDrv_MFC_WriteByte(0x2893, (U8)((gmfcMiuBaseAddr.u32OdLimitOdd>>4)>>16));

	MDrv_MFC_WriteByte(0x2832, 0x30); // [7:0] reg_od_r_thrd
	MDrv_MFC_WriteByte(0x2833, 0x7e); // [7:0] reg_od_wff_ack_thrd
	MDrv_MFC_WriteByte(0x2834, 0x20); // [7:0] reg_od_r_thrd2
	MDrv_MFC_WriteByte(0x2835, 0x50); // [7:0] reg_od_r_hpri
	MDrv_MFC_WriteByte(0x2836, 0x30); // [7:0] reg_od_w_thrd
	MDrv_MFC_WriteByte(0x2837, 0x04); // [7:0] reg_od_wlast_fire_thrd
	MDrv_MFC_WriteByte(0x2838, 0x20); // [7:0] reg_od_w_thrd2
	MDrv_MFC_WriteByte(0x2839, 0x50); // [7:0] reg_od_w_hpri
	MDrv_MFC_WriteByte(0x2841, 0x00); // od request space stop cnt
	MDrv_MFC_WriteByte(0x285C, 0x80); // [7:0] reg_patchTh0
	MDrv_MFC_WriteByte(0x285D, 0x00); // [5:0] reg_patchTh1 : bias offset
    // [6] reg_patchTh1 : patch enable
	MDrv_MFC_WriteByte(0x285E, 0x88); // [3:0] reg_patchTh2
    // [7:4] reg_patchTh3
	MDrv_MFC_WriteByte(0x2866, 0x10); // [7:0] reg_min3x3Length
	MDrv_MFC_WriteByte(0x2867, 0x40); // [7:0] reg_max3x3Length

	// [7:0] reg_od_mem_adr_limit low byte
	MDrv_MFC_WriteByte(0x2859, (U8)(gmfcMiuBaseAddr.u32OdSize>>4));
	// [7:0] reg_od_mem_adr_limit med byte
	MDrv_MFC_WriteByte(0x285A, (U8)((gmfcMiuBaseAddr.u32OdSize>>4)>>8));
	// [7:0] reg_od_mem_adr_limit high byte
	MDrv_MFC_WriteByte(0x285B, (U8)((gmfcMiuBaseAddr.u32OdSize>>4)>>16));

	// lsb request base address low byte
	MDrv_MFC_WriteByte(0x289E, (U8)(gmfcMiuBaseAddr.u32OdLsbBase>>4));
	// lsb request base address med byte
	MDrv_MFC_WriteByte(0x289F, (U8)((gmfcMiuBaseAddr.u32OdLsbBase>>4)>>8));
	// lsb request base address high byte
	MDrv_MFC_WriteByte(0x28A0, (U8)((gmfcMiuBaseAddr.u32OdLsbBase>>4)>>16));

	// lsb request limit address low byte
	MDrv_MFC_WriteByte(0x28A1, (U8)(gmfcMiuBaseAddr.u32OdLsbLimit>>4));
	// lsb request limit address med byte
	MDrv_MFC_WriteByte(0x28A2, (U8)((gmfcMiuBaseAddr.u32OdLsbLimit>>4)>>8));
	// lsb request limit address high byte
	MDrv_MFC_WriteByte(0x28A3, (U8)((gmfcMiuBaseAddr.u32OdLsbLimit>>4)>>16));

	MDrv_MFC_WriteByte(0x28A4, 0x20); // [7:0] reg_od_r_thrd_lsb
	MDrv_MFC_WriteByte(0x28A5, 0x30); // [7:0] reg_od_r_thrd2_lsb
	MDrv_MFC_WriteByte(0x28A6, 0x50); // [7:0] reg_od_r_hpri_lsb
	MDrv_MFC_WriteByte(0x28A7, 0x50); // [7:0] reg_od_w_hpri_lsb
	MDrv_MFC_WriteByte(0x28A8, 0x20); // [7:0] reg_od_w_thrd_lsb
	MDrv_MFC_WriteByte(0x28A9, 0x30); // [7:0] reg_od_w_thrd2_lsb
	MDrv_MFC_WriteByte(0x28AB, 0x14); // [3:0] reg_vsync_start_delay
    // [5:4] reg_vsync_width_delay    // [7:6] reg_vfend_delay
	if (gmfcMiuBaseAddr.u8OdMode==OD_MODE_666_COMPRESS)
		MDrv_MFC_WriteByte(0x289B, 0x25);
	else if (gmfcMiuBaseAddr.u8OdMode==OD_MODE_555_COMPRESS)
		MDrv_MFC_WriteByte(0x289B, 0x15);
	else
		MDrv_MFC_WriteByte(0x289B, 0x05);

	// [0]   reg_last_data_ctrl_en
    // [1]   reg_od1_last_dummy_pix_sel
    // [2]   reg_od1_last_rdy_sel
    // [6:4] reg_od_compress_mode
    // [7]   reg_od_lsb_wlast_force_req_disable

#if 0
	MDrv_MFC_WriteByte(0x2824, 0x0c);  // od active threshold
	MDrv_MFC_WriteByte(0x2825, 0x00);  // od active threshold
	MDrv_MFC_WriteByte(0x2841, 0x00);  // od request space stop cnt
	MDrv_MFC_WriteByte(0x28AB, 0x14);  // od self generate vsync
	MDrv_MFC_WriteByte(0x286C, 0x07);  // [7:6] reg_od_status_sel
	// [5]   reg_od_read_over_disable
	// [4]   reg_od_read_over_sel
	// [3]   reg_od_status_rst
	// [2]   reg_od_rq_over_under_mask_en
	// [1]   reg_od_next_frame_en
	// [0]	 reg_od_active_sel

	MDrv_MFC_WriteByte(0x286D, 0x82);
	// [7] reg_od1_write_data_over_sel, compress bypaa mode need to set 0
	// [6:4] reg_od1_overflow_thrd,  compress mode check line buffer overflow threshold sel
	// [3:0] reg_od1_underflow_thrd, compress mode check line buffer underflow threshold

	MDrv_MFC_WriteByte(0x284B, 0x80);
	// [7] reg_od1_read_data_under_sel
	// [6] reg_od1_linebuf_bypass_en

	MDrv_MFC_WriteByte(0x2885, 0x80);  // [7:4] reg_od1_read_under_act_thrd
	MDrv_MFC_WriteByte(0x285F, 0xff);  // [7:0] reg_od1_rbuf_thrd
#endif
	ucVal = OD_MODE_SEL;
	ucVal &= 0x0F;
	MDrv_MFC_WriteByte(0x2820, 0x20|ucVal);
	// [0]   od_en
	// [3:1] od_mode , 000{444}, 001{565}, 010{y-8}, 011{333}, 100{666}, 101{compress}, 110{555}, 111{888}
	// [4]   reserved
	// [5]   reg_od_user_weight_sel
	// [6]   od_h_range_en
	// [7]   od_v_range_en

    //printf("MDrv_MFC_InitializeOD()\n");
}
#endif

void MDrv_MFC_InitializePanel(void)
{
	msInitializeColorMatrix();

    if (gmfcSysInfo.u8PanelType == _MINI_LVDS || gmfcSysInfo.u8PanelType == _RSDS || gmfcSysInfo.u8PanelType == _MINI_LVDS_GIP || gmfcSysInfo.u8PanelType == _MINI_LVDS_GIP_V5)
		msInitializeTcon();

	#if (OD_MODE_SEL != OD_MODE_OFF)
	if (gmfcMiuBaseAddr.u8OdMode != OD_MODE_OFF)
	{
		#if (PANEL_TYPE_SEL == PNL_LCEAll)
			if( MDrv_MFC_ReadByte(0x1E48)>0 && MDrv_MFC_ReadByte(0x1E48)<=3 )
			{
				if(MDrv_MFC_ReadByte(0x1E48) == 1)//42"
					MDrv_MFC_InitializeOD(tOD42);
				else if(MDrv_MFC_ReadByte(0x1E48) == 2)//47"
					MDrv_MFC_InitializeOD(tOD47);
				else if(MDrv_MFC_ReadByte(0x1E48) == 3)//GIP 37"
					MDrv_MFC_InitializeOD(tOD37);
			}
        #else
                //MDrv_MFC_InitializeOD(tOverDrive);  // calvin
                MDrv_MFC_InitializeOD(tOD42);
		#endif
	}
	#endif
	//if(gmfcSysInfo.u8MirrorMode)
		//MDrv_MFC_Write2Bytes(0x2F2C, (gmfcSysInfo.u16Height-50)&0x07FF); // for flip memc issue
}

void MDrv_MFC_InitializeScTop2_Bypanel(void)
{
	MDrv_MFC_WriteRegsTbl(0x3200, tInitializeScTop2);
}

#if(CODEBASE_SEL == CODEBASE_LINUX)
void MDrv_MFC_InitializeBypass(void)
{
	U8 i;
	//for bypass use
	//IP CSC off: 20C0[0] = 0
	MDrv_MFC_WriteBit(0x20C0,  0, _BIT0); // [0]:CSC [1]:dither [2]:round

	//IP 121 horizontal and vertical disable: 2052[1:0] = 0
	MDrv_MFC_WriteByteMask(0x2052, 0, _BIT0|_BIT1);

	//MFC off: 290E[3:0] = 0.
	MDrv_MFC_WriteByteMask(0x290E, 0x00, 0x0F);

	//Color matrixes disable: 3002[3] = 0, 3003~3015 set to 0.
        MDrv_MFC_WriteBit(0x3002, 0, _BIT3);
        for (i=0; i<0x13; i++) // 0x3003 ~ 0x3015
            MDrv_MFC_WriteByte(0x3003+i, 0);

	//Brightness disable: 3016[1] = 0, 3018~301B set to 0.
	MDrv_MFC_WriteBit(0x3016, 0, _BIT1);
        for (i=0; i<0x4; i++) // 0x3018 ~ 0x301B
            MDrv_MFC_WriteByte(0x3018+i, 0);
}
#endif

#endif
