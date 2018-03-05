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
#define _MDRV_MFC_FILM_C_
#include "mdrv_mfc_platform.h"
#include "mdrv_mfc.h"
#include "mdrv_mfc_fb.h"

#if(CODESIZE_SEL == CODESIZE_ALL)
#define FILM_UNMATCH_THRESHOLD 0x20
#define FILM_SAME_UPPERBOUND_L 0x80 //0x80 //0x00
#define FILM_SAME_UPPERBOUND_H 0x01 //0x81 //0x03
#define FILM_CLIP_THR 0x0c

code MST_MFC_RegUnitType_t tInitializeFilmMode[]=
{
    {0x2052, 0x01},
    #if 0
    {0x2054, 0xFF},
    {0x2055, 0x01},
    #endif
    {0x2080, 0x07},
    {0x2081, 0x01},
    {0x2082, FILM_UNMATCH_THRESHOLD},
    {0x2083, 0x01},
    {0x2084, 0x18},
    {0x2085, 0x28},
    {0x2086, FILM_SAME_UPPERBOUND_L},
    {0x2087, FILM_SAME_UPPERBOUND_H},
    {0x2088, 0x20},
    {0x2089, 0x00},
    {0x208A, 0x00},
    {0x208B, 0x18},
    {0x208C, 0x00},
    {0x208D, 0x00},
    {0x208E, 0x68},
    {0x208F, 0x00},
    {0x2090, 0x58},
    {0x2091, 0x4a},
    {0x2093, 0x01},
    {0x2094, 0x00},
    {0x2095, 0x00},

    {0x209E, 0x28},
    {0x209F, 0x18},

    {0x20A2, FILM_CLIP_THR},
    {0x20A3, 0x45},
    {0x20A4, 0x10},
    {0x20A5, 0x10},
    {0x20A6, 0x00},
    {0x20A7, 0x00},
    {0x20A8, 0x00},
    {0x20A9, 0x00},
    {0x20AA, 0x00},
    {0x20AB, 0x00},
    {0x20AC, 0x00},
    {0x20AD, 0x00},
    {0x20AE, 0x00},
    {0x20AF, 0x00},
    {0x2004, 0X36},
    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeMfc26_Common[]=
{
    {0x2616, 0x12},
    {0x2617, 0x81},
    {0x2618, 0X20},
    {0x2619, 0X80},
    {0x261B, 0X14},
    {0x261D, 0X08},
    {0x261E, 0x02},

    {0x2622, 0x00},
    {0x2625, 0X80},
    {0x2628, 0x40},
    {0x2629, 0x12},
    {0x262A, 0X00},
    {0x262B, 0X00},
    {0x262C, 0X41},
    {0x262D, 0X81},

    {0x2638, 0X00},
    {0x2639, 0X61},
    {0x263A, 0X80},
    {0x263B, 0X60},
    {0x263E, 0x80},
    {0x263F, 0x00},
    {0x2642, 0x05},
    {0x2643, 0x05},
    {0x2644, 0x42},
    {0x2645, 0x00},
    {0x2646, 0x10},
    {0x2647, 0xD0},
    {0x2648, 0x38},
    {0x2649, 0x02},
    {0x264A, 0x08},
    {0x264B, 0x00},

    {0x264C, 0X00},
    {0x264D, 0x51},
    {0x264E, 0X08},
    {0x264F, 0X00},
    {0x2659, 0X00},
    {0x2660, 0X10}, //[7]=0 090820
    {0x2661, 0X63},
    {0x2662, 0X1D},
    {0x2663, 0X9E},
    {0x2664, 0X01},
    {0x2665, 0X01},
    {0x2666, 0X70},
    {0x2667, 0X01},
    {0x2668, 0XBB},
    {0x2669, 0X26},
    {0x266A, 0X08},
    {0x266B, 0X87},
    {0x266C, 0X00},
    {0x266D, 0X10},

    {0x2674, 0X80},
    {0x2676, 0X08},
    {0x2677, 0X00},
    {0x2678, 0X00},
    {0x2679, 0X02},
    {0x267A, 0X88},
    {0x267B, 0X09},
    {0x267C, 0X3F},
    {0x267D, 0X00},
    {0x267E, 0X40},

    {0x2680, 0X32},
    {0x2681, 0X02},
    {0x2682, 0X18},
    {0x2683, 0x03},
    {0x2684, 0X18},
    {0x2685, 0X30},
    {0x2686, 0X00},
    {0x2687, 0X20},
    {0x2688, 0X10},
    {0x2689, 0X10},
    {0x268A, 0X20},
    {0x268B, 0X10},
    {0x268C, 0X98},
    {0x268D, 0x20},
    {0x268E, 0X00},
    {0x268F, 0x00},
    {0x2690, 0X08},
    {0x2691, 0X20},
    {0x2692, 0X10},
    {0x2693, 0XF3},
    {0x2694, 0XC0},
    {0x2695, 0X00},
    {0x2696, 0X40},
    {0x2697, 0X04},
    {0x2698, 0X40},
    {0x2699, 0X20},
    {0x269A, 0X3c},
    {0x269B, 0X98},
    {0x269C, 0X10},
    {0x269D, 0X10},
    {0x269E, 0X40},
    {0x269F, 0X00},

    {0x26A0, 0X00},
    {0x26A1, 0X40},
    {0x26B6, 0X02},
    {0x26B7, 0X00},
    {0x26BC, 0XFF},
    {0x26BD, 0X28},
    {0x26BF, 0X01},
    {0x26E0, 0X00},
    {0x26E1, 0X00},
    {0x26E2, 0X00},
    {0x26E3, 0X00},
    {0x26E4, 0X04},
    {0x26E5, 0X04},
    {0x26E6, 0XFF},
    {0x26E7, 0X0F},

    {0x26EA, 0X00},
    {0x26EB, 0X00},
    {0x26EC, 0X48},
    {0x26ED, 0X80},
    {0x26EE, 0XF0},
    {0x26EF, 0X01},
    {0x26F0, 0XF4},
    {0x26F1, 0X01},

    {0x26FC, 0X69},
    {0x26FD, 0XD1},
    {0x26FE, 0X80},
    {0x26FF, 0X80},
    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeMfc26_U01[]=
{
    {0x261A, 0x00},
    {0x261C, 0x00},
    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeMfc26_U02[]=
{
#if 1
    {0x261A, 0x00},
    {0x261C, 0x00},
    {0x266F, 0x01},
#else
    {0x261A, 0x02},
    {0x261C, 0x02},
    {0x266F, 0X01},
    {0x26F2, 0XFF},
    {0x26F3, 0XFF},
    {0x26F8, 0XFF},
    {0x26F9, 0XFF},
#endif
    {_END_OF_TBL_, _END_OF_TBL_},
};


#define DEFAULT_KEEP_ZERO 0x80
#define DEFAULT_MV0_PENALTY 0x0a
#define MFC_DEBUG_BLK 0    //0~7  0x290A[2:0]  --> move to
code MST_MFC_RegUnitType_t tInitializeMfc_Common[]=
{
    // MC Settings
    {0x2900, 0x00},
    {0x2901, 0x00},
    {0x2902, 0x41},
    {0x2903, 0x38},

    // MC SETTINGS
    {0x2908, 0XA0},
    {0x2909, 0X03},
    {0x290A, MFC_DEBUG_BLK},
    {0x290B, 0X08},
    {0x290C, 0X00},
    {0x290D, 0X00},
    {0x290E, 0X2F},
    {0x290F, 0X1D},
    {0x2910, 0XF7},
    {0x2911, 0X13},
    {0x2912, 0X10},
    {0x2913, 0X0C},
    {0x2914, 0X20},
    {0x2915, 0XF0},
    {0x2916, 0x10},
    {0x2917, 0x10},
    // GMV
    {0x2918, 0x38},
    {0x291A, 0x00},
    {0x291B, 0xff},
    {0x291E, 0x00},
    {0x291F, 0x00},

    {0x2920, 0x00},
    {0x2921, 0x00},
    {0x2922, 0x00},
    {0x2923, 0xe8},
    {0x2924, 0x00},
    {0x2925, 0x00},
    {0x2926, 0x00},
    {0x2927, 0x00},

    //{0x2928, 0X30},
    //{0x2929, 0x1E},

    {0x292E, 0x00},
    {0x292F, 0x08},
    {0x292D, 0x08},

    {0x2930, 0xB3},
    {0x2931, 0x00},

    {0x2932, 0xc2},
    {0x2933, 0x4c},
    {0x2934, 0x10},
    {0x2935, 0xfe},
    {0x2936, 0x08},
    {0x2937, 0x08},
    {0x2938, 0x02},
    {0x2939, 0x04},

    {0x293A, 0XFE},
    {0x293B, 0X7C},
    {0x293F, 0X06},

    {0x2940, 0x40},
    {0x2941, 0x81},
    {0x2942, 0x10},
    {0x2943, 0x40},
    {0x2944, 0x01},
    {0x2945, 0x00},
    {0x2946, 0xff},
    {0x2947, 0xff},
    {0x2948, 0xff},
    {0x2949, 0xff},
    {0x294A, 0x77},
    {0x294B, 0x60},
    {0x294C, 0x02},
    {0x294D, 0x02},
    {0x294E, 0xB3},
    {0x2950, 0xFF},
    {0x2951, 0xFF},
    {0x2953, DEFAULT_KEEP_ZERO},
    {0x2954, 0x10},
    {0x2955, 0x08},
    {0x2958, 0x0c},
    {0x2959, 0x7c},
    {0x295A, 0x15},
    {0x295B, 0x0c},
    {0x295C, 0x02},
    {0x295D, 0x80},
    {0x295E, 0x04},
    {0x295F, 0x04},

    {0x2960, 0x80},
    {0x2961, 0x66},
    {0x2962, 0x10},
    {0x2963, 0x43},
    {0x2964, 0x03},
    {0x2965, 0x05},
    {0x2966, 0x00},
    {0x2967, DEFAULT_MV0_PENALTY},
    {0x2968, 0x42},
    {0x2969, 0x42},
    {0x296C, 0x63},
    {0x296D, 0x46},
    {0x296E, 0x23},
    {0x296F, 0x03},

    {0x2970, 0X03},
    {0x2971, 0X03},
    {0x2972, 0X08},
    {0x2973, 0X08},
    {0x2974, 0X14},
    {0x2975, 0X04},
    {0x2976, 0X06},
    {0x2977, 0X00},

    {0x2978, 0X03},
    {0x2979, 0X80},
    {0x297A, 0X03},
    {0x297B, 0X00},
    {0x297C, 0X00},
    {0x297D, 0X00},
    {0x297E, 0X1F},
    {0x297F, 0X7F},

    {0x2980, 0X02},
    {0x2981, 0x00},
    {0x2982, 0XF4},
    {0x2983, 0X03},
    {0x2984, 0X80},
    {0x2985, 0X40},
    {0x2988, 0X00},
    {0x2989, 0X80},
    {0x298C, 0x25},
    {0x298D, 0x25},
    {0x298E, 0x10},

    {0x2994, 0X00},
    {0x2995, 0X00},
    {0x2996, 0x00},
    {0x2997, 0x00},
    {0x2998, 0x6A},
    {0x2999, 0X52},
    {0x299A, 0X04},
    {0x299B, 0X00},
    {0x299C, 0X40},
    {0x299D, 0XC0},
    {0x299E, 0X2C},
    {0x299F, 0X14},

    // MC2D_SKIP
    {0x29A0, 0x00},
    {0x29A1, 0xff},
    {0x29A2, 0x00},
    {0x29A3, 0xff},
    {0x29A4, 0X21},
    {0x29A5, 0XA3},
    {0x29A6, 0X00},
    {0x29A7, 0X00},
    {0x29A8, 0X00},
    {0x29A9, 0X0F},
    {0x29AA, 0X01},
    {0x29AB, 0X66},
    {0x29AC, 0X80},
    {0x29AD, 0XC2},
    {0x29AE, 0X00},
    {0x29AF, 0X38},

    {0x29B0, 0X30},
    {0x29B1, 0X8E},
    {0x29B2, 0X88},
    {0x29B3, 0XB7},
    {0x29B4, 0X13},
    {0x29B5, 0X40},
    {0x29B6, 0X09},
    {0x29B7, 0X82},
    {0x29B8, 0X19},
    {0x29B9, 0X40},
    {0x29BA, 0X00},
    {0x29BB, 0X00},
    {0x29BC, 0X00},
    {0x29BD, 0X00},
    {0x29BE, 0X00},
    {0x29BF, 0X00},


    {_END_OF_TBL_, _END_OF_TBL_},
};
code MST_MFC_RegUnitType_t tInitializeMfc_U01[]=
{
    {0x293C, 0x00},
    {0x293D, 0x00},
    {0x294F, 0xFE},
    {0x2956, 0x7c},
    {0x2957, 0x3c},
    {0x296A, 0x01},
    {0x296B, 0x01},



    {_END_OF_TBL_, _END_OF_TBL_},
};
code MST_MFC_RegUnitType_t tInitializeMfc_U02[]=
{
#if 1
    {0x293C, 0x00},
    {0x293D, 0x00},
	{0x294F, 0xFF},
    {0x2956, 0x7c},
    {0x2957, 0x3c},
    {0x296A, 0x01},
    {0x296B, 0x01},
#else
    {0x293C, 0xFF},
    {0x293D, 0x5F},
    {0x294F, 0xFF},
    {0x2956, 0x10},
    {0x2957, 0x08},
    {0x296A, 0x00},
    {0x296B, 0x00},
#endif

    {_END_OF_TBL_, _END_OF_TBL_},
};

code MST_MFC_RegUnitType_t tInitializeMFC2C[]=
{
    {0x2C12, 0x7F},
    {0x2C13, 0xFF},

    {0x2C20, 0X32},
    {0x2C21, 0X0A},
    {0x2C22, 0X18},
    {0x2C23, 0X01},
    {0x2C2C, 0X8C},
    {0x2C2D, 0X00},
    {0x2C2E, 0X18},
    {0x2C2F, 0X00},

    {0x2C30, 0X00},
    {0x2C31, 0X40},
    {0x2C32, 0X10},
    {0x2C33, 0XA3},
    {0x2C34, 0XC0},
    {0x2C35, 0X00},
    {0x2C36, 0X40},
    {0x2C37, 0X07},
    {0x2C38, 0X40},
    {0x2C39, 0X20},
    {0x2C3A, 0X30},
    {0x2C3B, 0X0C},
    {0x2C3E, 0X77},
    {0x2C3F, 0X22},
    {0x2C43, 0x66},

    {_END_OF_TBL_, _END_OF_TBL_},
};

#define TwoChip_GarBand_H 128
#define MFC_H ((LimitCheck(gmfcSysInfo.u16Width, 64)/16)/2)
void MDrv_MFC_InitializeMfc(void)
{
    // Enable MR history mode & Write history MR weighting
    MDrv_MFC_WriteByte(0x2002, 0x17 ); //[7] suchiun 090820 for OSD edge color issue

    // Pseudo Buffer Request Size and Base
    MDrv_MFC_WriteByte(0x292A, (gmfcMiuBaseAddr.u32MfcBase>>4));     // Low byte base address
    MDrv_MFC_WriteByte(0x292B, (gmfcMiuBaseAddr.u32MfcBase>>4)>>8);  // Mid byte base address
    MDrv_MFC_WriteByte(0x2928, (gmfcMiuBaseAddr.u32MfcBase>>4)>>16); // High byte base address
     #if( TwoChip_Func != TwoChip_OFF )
           MDrv_MFC_Write2Bytes(0x2904, LimitCheck(gmfcSysInfo.u16Width/2, 64)+ TwoChip_GarBand_H); //ME
     #else
        MDrv_MFC_Write2Bytes(0x2904, LimitCheck(gmfcSysInfo.u16Width, 64));
    #endif
    MDrv_MFC_Write2Bytes(0x2906, LimitCheck(gmfcSysInfo.u16Height, 16));
    if (gmfcSysInfo.u16Width > 1400 )
        MDrv_MFC_WriteByte(0x2919, 0x25);
    else
        MDrv_MFC_WriteByte(0x2919, 0x24);
    MDrv_MFC_WriteByte(0x291C, 0x10);
    MDrv_MFC_WriteByte(0x291D, (gmfcSysInfo.u16Height>>3)-0x10);
    MDrv_MFC_WriteByte(0x293E, 0x28 |(((LimitCheck(gmfcSysInfo.u16Width, 64) - gmfcSysInfo.u16Width)/16)&0x3));

    if (gmfcSysInfo.u16Width == 1920 )
        MDrv_MFC_WriteByte(0x2952, 0x4F);
    else // 1366, 1680
        MDrv_MFC_WriteByte(0x2952, 0x5F);

     #if( TwoChip_Func != TwoChip_OFF )
        MDrv_MFC_Write2Bytes(0x2990, gmfcSysInfo.u16Width/2 + TwoChip_GarBand_H); //MI
     #else
        MDrv_MFC_Write2Bytes(0x2990, gmfcSysInfo.u16Width);
    #endif
    MDrv_MFC_Write2Bytes(0x2992, gmfcSysInfo.u16Height);
    MDrv_MFC_WriteRegsTbl(0x2900, tInitializeMfc_Common);
    MDrv_MFC_WriteRegsTbl(0x2600, tInitializeMfc26_Common);
    MDrv_MFC_WriteRegsTbl(0x2C00, tInitializeMFC2C);
    if(gmfcSysInfo.u8ChipRevision==1)
    {
        MDrv_MFC_WriteRegsTbl(0x2900, tInitializeMfc_U02);
        MDrv_MFC_WriteRegsTbl(0x2600, tInitializeMfc26_U02);
    }
    else// if(gmfcSysInfo.u8ChipRevision==U01)
    {
        MDrv_MFC_WriteRegsTbl(0x2900, tInitializeMfc_U01);
        MDrv_MFC_WriteRegsTbl(0x2600, tInitializeMfc26_U01);
    }

    #if( TwoChip_Func != TwoChip_OFF ) //j090609
        MDrv_MFC_WriteByte(0x2929, MFC_H|_BIT6);
    #else
        MDrv_MFC_WriteByte(0x2929, MFC_H/2);
    #endif
}

void MDrv_MFC_InitializeFilmMode(void)
{
    MDrv_MFC_WriteRegsTbl(0x2000, tInitializeFilmMode); // initialize all of bank
    #if 0
    if(gmfcSysInfo.u8ChipRevision == 1)
    {
        MDrv_MFC_WriteByte(0x2092, 0x66);  // U02 new setting, U01 can not apply this.
    }
    #endif
    MDrv_MFC_InitializeMfc();
}
#endif


