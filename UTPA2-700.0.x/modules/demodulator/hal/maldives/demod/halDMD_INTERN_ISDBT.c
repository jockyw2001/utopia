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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <math.h>
#endif

#include "drvDMD_ISDBT.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define DMD_ISDBT_CHIP_EULER        0x00
#define DMD_ISDBT_CHIP_NUGGET       0x01
#define DMD_ISDBT_CHIP_KAPPA        0x02
#define DMD_ISDBT_CHIP_EINSTEIN     0x03
#define DMD_ISDBT_CHIP_NAPOLI       0x04
#define DMD_ISDBT_CHIP_MONACO       0x05
#define DMD_ISDBT_CHIP_MIAMI        0x06
#define DMD_ISDBT_CHIP_MUJI         0x07
#define DMD_ISDBT_CHIP_MUNICH       0x08
#define DMD_ISDBT_CHIP_MANHATTAN    0x09
#define DMD_ISDBT_CHIP_MULAN        0x0A
#define DMD_ISDBT_CHIP_MESSI        0x0B
#define DMD_ISDBT_CHIP_MASERATI     0x0C
#define DMD_ISDBT_CHIP_KIWI         0x0D
#define DMD_ISDBT_CHIP_MACAN        0x0E
#define DMD_ISDBT_CHIP_MUSTANG      0x0F
#define DMD_ISDBT_CHIP_MAXIM        0x10
#if defined(CHIP_EULER)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_EULER
#elif defined(CHIP_NUGGET)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_NUGGET
#elif defined(CHIP_KAPPA)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_KAPPA
#elif defined(CHIP_EINSTEIN)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_EINSTEIN
#elif defined(CHIP_NAPOLI)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_NAPOLI
#elif defined(CHIP_MIAMI)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MIAMI
#elif defined(CHIP_MUJI)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MUJI
#elif defined(CHIP_MUNICH)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MUNICH
#elif defined(CHIP_MANHATTAN)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MANHATTAN 
#elif defined(CHIP_MULAN)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MULAN  
#elif defined(CHIP_MESSI)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MESSI 
#elif defined(CHIP_MASERATI)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MASERATI 
#elif defined(CHIP_KIWI)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_KIWI 
#elif defined(CHIP_MACAN)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MACAN 
#elif defined(CHIP_MUSTANG)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MUSTANG 
#elif defined(CHIP_MAXIM)
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_MAXIM  
#else
 #define DMD_ISDBT_CHIP_VERSION     DMD_ISDBT_CHIP_EULER
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if (DMD_ISDBT_CHIP_VERSION >= DMD_ISDBT_CHIP_MULAN)
#define DMD_ISDBT_TBVA_EN		1
#else
#define DMD_ISDBT_TBVA_EN		0
#endif
#define _RIU_READ_BYTE(addr)        ( READ_BYTE(psDMD_ISDBT_ResData->sDMD_ISDBT_PriData.virtDMDBaseAddr + (addr) ) )
#define _RIU_WRITE_BYTE(addr, val)  ( WRITE_BYTE(psDMD_ISDBT_ResData->sDMD_ISDBT_PriData.virtDMDBaseAddr + (addr), val) )

#define HAL_INTERN_ISDBT_DBINFO(y)   //y
#ifndef MBRegBase
#define MBRegBase               0x112600UL
#endif
#ifndef MBRegBase_DMD1
#define MBRegBase_DMD1          0x112400UL
#endif
#ifndef DMDMcuBase
#define DMDMcuBase              0x103480UL
#endif

#if (DMD_ISDBT_CHIP_VERSION >= DMD_ISDBT_CHIP_MULAN) && (DMD_ISDBT_CHIP_VERSION != DMD_ISDBT_CHIP_MESSI) && (DMD_ISDBT_CHIP_VERSION != DMD_ISDBT_CHIP_KIWI)
#define REG_ISDBT_LOCK_STATUS   0x11F5
#define ISDBT_TDP_REG_BASE      0x1400
#define ISDBT_FDP_REG_BASE      0x1500
#define ISDBT_FDPEXT_REG_BASE   0x1600
#define ISDBT_OUTER_REG_BASE    0x1700
#else
#define REG_ISDBT_LOCK_STATUS   0x36F5
#define ISDBT_TDP_REG_BASE      0x3700
#define ISDBT_FDP_REG_BASE      0x3800
#define ISDBT_FDPEXT_REG_BASE   0x3900
#define ISDBT_OUTER_REG_BASE    0x3A00
#endif

#if (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_EULER) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_NUGGET) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUNICH)
#define ISDBT_MIU_CLIENTW_ADDR      0xF5
#define ISDBT_MIU_CLIENTR_ADDR      0xF5
#define ISDBT_MIU_CLIENTW_MASK      0x87
#define ISDBT_MIU_CLIENTR_MASK      0x87
#define ISDBT_MIU_CLIENTW_BIT_MASK  0x01
#define ISDBT_MIU_CLIENTR_BIT_MASK  0x02
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_EINSTEIN) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_NAPOLI) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MONACO) || \
      (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MIAMI) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUJI) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MANHATTAN) || \
      (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MESSI)
#define ISDBT_MIU_CLIENTW_ADDR      0xF2
#define ISDBT_MIU_CLIENTR_ADDR      0xF2
#define ISDBT_MIU_CLIENTW_MASK      0x66
#define ISDBT_MIU_CLIENTR_MASK      0x66
#define ISDBT_MIU_CLIENTW_BIT_MASK  0x02 
#define ISDBT_MIU_CLIENTR_BIT_MASK  0x04
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_KAPPA)
#define ISDBT_MIU_CLIENTW_ADDR      0xF1
#define ISDBT_MIU_CLIENTR_ADDR      0xF0
#define ISDBT_MIU_CLIENTW_MASK      0x47
#define ISDBT_MIU_CLIENTR_MASK      0x46
#define ISDBT_MIU_CLIENTW_BIT_MASK  0x02  
#define ISDBT_MIU_CLIENTR_BIT_MASK  0x20
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_KIWI)
#define ISDBT_MIU_CLIENTW_ADDR      0xF1
#define ISDBT_MIU_CLIENTR_ADDR      0xF0
#define ISDBT_MIU_CLIENTW_MASK      0x47
#define ISDBT_MIU_CLIENTR_MASK      0x46
#define ISDBT_MIU_CLIENTW_BIT_MASK  0x04 
#define ISDBT_MIU_CLIENTR_BIT_MASK  0x20
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

const MS_U8 INTERN_ISDBT_table[] = {
    #include "DMD_INTERN_ISDBT.dat"
};

#ifndef UTPA2
static const float _LogApproxTableX[80] =
{ 1.00, 1.30, 1.69, 2.20, 2.86, 3.71, 4.83, 6.27, 8.16, 10.60, 13.79,
  17.92, 23.30, 30.29, 39.37, 51.19, 66.54, 86.50, 112.46, 146.19,
  190.05, 247.06, 321.18, 417.54, 542.80, 705.64, 917.33, 1192.53,
  1550.29, 2015.38, 2620.00, 3405.99, 4427.79, 5756.13, 7482.97,
  9727.86, 12646.22, 16440.08, 21372.11, 27783.74, 36118.86,
  46954.52, 61040.88, 79353.15, 103159.09, 134106.82, 174338.86,
  226640.52, 294632.68, 383022.48, 497929.22, 647307.99, 841500.39, 1093950.50,
  1422135.65, 1848776.35, 2403409.25, 3124432.03, 4061761.64, 5280290.13,
  6864377.17, 8923690.32, 11600797.42, 15081036.65, 19605347.64, 25486951.94,
  33133037.52, 43072948.77, 55994833.40, 72793283.42, 94631268.45,
  123020648.99, 159926843.68, 207904896.79, 270276365.82, 351359275.57,
  456767058.24, 593797175.72, 771936328.43, 1003517226.96
};

static const float _LogApproxTableY[80] =
{ 0.00, 0.11, 0.23, 0.34, 0.46, 0.57, 0.68, 0.80, 0.91, 1.03, 1.14, 1.25,
  1.37, 1.48, 1.60, 1.71, 1.82, 1.94, 2.05, 2.16, 2.28, 2.39, 2.51, 2.62,
  2.73, 2.85, 2.96, 3.08, 3.19, 3.30, 3.42, 3.53, 3.65, 3.76, 3.87, 3.99,
  4.10, 4.22, 4.33, 4.44, 4.56, 4.67, 4.79, 4.90, 5.01, 5.13, 5.24, 5.36,
  5.47, 5.58, 5.70, 5.81, 5.93, 6.04, 6.15, 6.27, 6.04, 6.15, 6.27, 6.38,
  6.49, 6.61, 6.72, 6.84, 6.95, 7.06, 7.18, 7.29, 7.41, 7.52, 7.63, 7.75,
  7.86, 7.98, 8.09, 8.20, 8.32, 8.43, 8.55, 8.66
};
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern MS_U8 u8DMD_ISDBT_DMD_ID;

extern DMD_ISDBT_ResData *psDMD_ISDBT_ResData;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#ifndef UTPA2

#ifndef MSOS_TYPE_LINUX
static float Log10Approx(float flt_x)
{
    MS_U8  indx = 0;

    do {
        if (flt_x < _LogApproxTableX[indx])
            break;
        indx++;
    }while (indx < 79);   //stop at indx = 80

    return _LogApproxTableY[indx];
}
#endif

static MS_U16 _CALCULATE_SQI(float fber)
{
    float flog_ber;
    MS_U16 u16SQI;
    
    #ifdef MSOS_TYPE_LINUX
    flog_ber = (float)log10((double)fber);
    #else
    if (fber != 0.0)
        flog_ber = (float)(-1.0*Log10Approx((double)(1.0 / fber))); 
    else
        flog_ber = -8.0;//when fber=0 means u16SQI=100    
    #endif
    
    //printf("dan fber = %f\n", fber);
    //printf("dan flog_ber = %f\n", flog_ber);
    // Part 2: transfer ber value to u16SQI value.
    if (flog_ber <= ( - 7.0))
    {
        u16SQI = 100;    //*quality = 100;
    }
    else if (flog_ber < -6.0)
    {
        u16SQI = (90+((( - 6.0) - flog_ber) / (( - 6.0) - ( - 7.0))*(100-90)));
    }
    else if (flog_ber < -5.5)
    {
        u16SQI = (80+((( - 5.5) - flog_ber) / (( - 5.5) - ( - 6.0))*(90-80)));
    }
    else if (flog_ber < -5.0)
    {
        u16SQI = (70+((( - 5.0) - flog_ber) / (( - 5.0) - ( - 5.5))*(80-70)));
    }
    else if (flog_ber < -4.5)
    {
        u16SQI = (60+((( - 4.5) - flog_ber) / (( -4.5) - ( - 5.0))*(70-50)));
    }
    else if (flog_ber < -4.0)
    {
        u16SQI = (50+((( - 4.0) - flog_ber) / (( - 4.0) - ( - 45))*(60-50)));
    }
    else if (flog_ber < -3.5)
    {
        u16SQI = (40+((( - 3.5) - flog_ber) / (( - 3.5) - ( - 4.0))*(50-40)));
    }
    else if (flog_ber < -3.0)
    {
        u16SQI = (30+((( - 3.0) - flog_ber) / (( - 3.0) - ( - 3.5))*(40-30)));
    }
    else if (flog_ber < -2.5)
    {
        u16SQI = (20+((( - 2.5) - flog_ber) / (( - 2.5) - ( -3.0))*(30-20)));
    }
    else if (flog_ber < -2.0)
    {
        u16SQI = (0+((( - 2.0) - flog_ber) / (( - 2.0) - ( - 2.5))*(20-0)));
    }
    else
    {
        u16SQI = 0;
    }
    
    return u16SQI;
}
#endif

static MS_U8 _HAL_DMD_RIU_ReadByte(MS_U32 u32Addr)
{
    return _RIU_READ_BYTE(((u32Addr) << 1) - ((u32Addr) & 1));
}

static void _HAL_DMD_RIU_WriteByte(MS_U32 u32Addr, MS_U8 u8Value)
{
    _RIU_WRITE_BYTE(((u32Addr) << 1) - ((u32Addr) & 1), u8Value);
}

static void _HAL_DMD_RIU_WriteByteMask(MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask)
{
    _RIU_WRITE_BYTE((((u32Addr) <<1) - ((u32Addr) & 1)), (_RIU_READ_BYTE((((u32Addr) <<1) - ((u32Addr) & 1))) & ~(u8Mask)) | ((u8Value) & (u8Mask)));
}

static MS_BOOL _MBX_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag = 0xFF;
    MS_U32 u32MBRegBase = MBRegBase;
    
    if (u8DMD_ISDBT_DMD_ID == 0)
        u32MBRegBase = MBRegBase;
    else if (u8DMD_ISDBT_DMD_ID == 1)
        u32MBRegBase = MBRegBase_DMD1;
    
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x00, (u16Addr&0xff));
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x01, (u16Addr>>8));
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x10, u8Data);
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x1E, 0x01);

    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51

    for (u8CheckCount=0; u8CheckCount < 10; u8CheckCount++)
    {
        u8CheckFlag = _HAL_DMD_RIU_ReadByte(u32MBRegBase + 0x1E);
        if ((u8CheckFlag&0x01)==0)
            break;
        MsOS_DelayTask(1);
    }
    
    if (u8CheckFlag&0x01)
    {
        printf("ERROR: ATSC INTERN DEMOD MBX WRITE TIME OUT!\n");
        return FALSE;
    }
    
    return TRUE;
}

static MS_BOOL _MBX_ReadReg(MS_U16 u16Addr, MS_U8 *u8Data)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag = 0xFF;
    MS_U32 u32MBRegBase = MBRegBase;
    
    if (u8DMD_ISDBT_DMD_ID == 0)
        u32MBRegBase = MBRegBase;
    else if (u8DMD_ISDBT_DMD_ID == 1)
        u32MBRegBase = MBRegBase_DMD1;
    
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x00, (u16Addr&0xff));
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x01, (u16Addr>>8));
    _HAL_DMD_RIU_WriteByte(u32MBRegBase + 0x1E, 0x02);
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51

    for (u8CheckCount=0; u8CheckCount < 10; u8CheckCount++)
    {
        u8CheckFlag = _HAL_DMD_RIU_ReadByte(u32MBRegBase + 0x1E);
        if ((u8CheckFlag&0x02)==0)
        {
           *u8Data = _HAL_DMD_RIU_ReadByte(u32MBRegBase + 0x10);
            break;
        }
        MsOS_DelayTask(1);
    }
    
    if (u8CheckFlag&0x02)
    {
        printf("ERROR: ATSC INTERN DEMOD MBX READ TIME OUT!\n");
        return FALSE;
    }
    
    return TRUE;
}



#if (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_EULER)
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_EULER--------------\n"));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_NUGGET)
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    MS_U8 u8Val = 0;
   
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_NUGGET--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x03);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 

    u8Val = _HAL_DMD_RIU_ReadByte(0x1006F5);
    _HAL_DMD_RIU_WriteByte(0x1006F5, (u8Val & ~0x03));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_KAPPA)
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_KAPPA--------------\n"));
   
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_EINSTEIN)
static void _HAL_INTERN_ISDBT_InitClk(void)
{  
    MS_U8 u8Val = 0;
    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_EINSTEIN--------------\n"));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    u8Val = _HAL_DMD_RIU_ReadByte(0x11208E);    //dan add to clear bit 0
    u8Val &= ~0x01; 
    _HAL_DMD_RIU_WriteByte(0x11208E, u8Val);

    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x03);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_NAPOLI)
static MS_BOOL _HAL_INTERN_ISDBT_InitClk(void) /* Ok */
{    
    MS_U8 u8Val = 0;
    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_NAPOLI--------------\n"));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    u8Val = _HAL_DMD_RIU_ReadByte(0x11208E);    //dan add to clear bit 0
    u8Val &= ~0x01; 
    _HAL_DMD_RIU_WriteByte(0x11208E, u8Val);

    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x03);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MONACO)
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    MS_U8 u8Val = 0;
    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MONACO--------------\n"));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // Init by HKMCU
    u8Val = _HAL_DMD_RIU_ReadByte(0x11208E);    //dan add to clear bit 0
    u8Val &= ~0x01; 
    _HAL_DMD_RIU_WriteByte(0x11208E, u8Val);

    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x14);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x41);
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x00);       
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MIAMI)
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MIAMI--------------\n"));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);
    
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
     
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f4d, 0x00); //inner clock
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4e, 0x00); //outer clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f51, 0x00); //cci lms clock  
    _HAL_DMD_RIU_WriteByte(0x111f50, 0x88); //cci lms clock   
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);    
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUJI)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MUJI--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f29, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x10); 
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4d, 0x00); //inner clock
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x40);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x14);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x41);     
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x00);   
    _HAL_DMD_RIU_WriteByte(0x111f4f, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x112091, 0x46);
    _HAL_DMD_RIU_WriteByte(0x112090, 0x00);
  
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUNICH)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MUNICH--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);
    
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f4d, 0x00); //inner clock
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4e, 0x00); //outer clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f51, 0x00); //cci lms clock  
    _HAL_DMD_RIU_WriteByte(0x111f50, 0x88); //cci lms clock   
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MANHATTAN)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MANHATTAN--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f29, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x10); 
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4d, 0x00); //inner clock
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x40);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x14);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x41);     
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x00);   
    _HAL_DMD_RIU_WriteByte(0x111f4f, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f81, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f80, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f83, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f82, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f85, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f84, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f87, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f86, 0x44);
  
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MULAN)
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MULAN--------------\n"));
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);
    
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01); //reset ts divider

    _HAL_DMD_RIU_WriteByte(0x111f28, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x04); 
    _HAL_DMD_RIU_WriteByte(0x111f31, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f30, 0x01); 
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x80);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f24, 0x05);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f62, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f65, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f64, 0x44);    
    _HAL_DMD_RIU_WriteByte(0x111f66, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f69, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f68, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f6b, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f6a, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6d, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f6c, 0x40);
    _HAL_DMD_RIU_WriteByte(0x111f6f, 0x01); 
    _HAL_DMD_RIU_WriteByte(0x111f6e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f72, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f75, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f74, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f79, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f78, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7b, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7a, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7d, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7c, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f7f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111fe1, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111fe0, 0x88);
    _HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe2, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111feb, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111fea, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fef, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fee, 0x88);
    
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00); //reset ts divider 
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);    
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MESSI)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MESSI--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f29, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x10); 
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);     
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f4d, 0x00); //inner clock
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x40);    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x04);  
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44); 
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x14);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x41);     
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x00);   
    _HAL_DMD_RIU_WriteByte(0x111f4f, 0x0C);
    _HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f51, 0x48);
    _HAL_DMD_RIU_WriteByte(0x111f50, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f81, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f80, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f83, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f82, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f85, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f84, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f87, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f86, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f89, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f88, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f8b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f8a, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f8d, 0x18);
    _HAL_DMD_RIU_WriteByte(0x111f8c, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f8f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f8e, 0x44);
  
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}

#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MASERATI)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MASERATI--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);
    
    _HAL_DMD_RIU_WriteByte(0x1128d0, 0x01);
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
 
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1128d0, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x04); 
    _HAL_DMD_RIU_WriteByte(0x111f31, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f30, 0x01);   
    _HAL_DMD_RIU_WriteByte(0x111f3b, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);      
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x80);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f63, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f62, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f65, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f64, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f66, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f68, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6a, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6c, 0x40);
    _HAL_DMD_RIU_WriteByte(0x111f6f, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f6e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f73, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f72, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f75, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f74, 0x44);      
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x44);   
    _HAL_DMD_RIU_WriteByte(0x111f79, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f78, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7b, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7a, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f7d, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7c, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111fe1, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111fe0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111ff0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe2, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111feb, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111fea, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fef, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fee, 0x88);
    _HAL_DMD_RIU_WriteByte(0x15298f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x15298e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152991, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152990, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152992, 0x04);
    _HAL_DMD_RIU_WriteByte(0x1529e5, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1529e4, 0x04);
    _HAL_DMD_RIU_WriteByte(0x152971, 0x10);
    _HAL_DMD_RIU_WriteByte(0x152970, 0x01);
  
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MACAN)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MACAN--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);
    
    _HAL_DMD_RIU_WriteByte(0x1128d0, 0x01);
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
 
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1128d0, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x04); 
    _HAL_DMD_RIU_WriteByte(0x111f31, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f30, 0x01);   
    _HAL_DMD_RIU_WriteByte(0x111f3b, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);      
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x80);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f63, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f62, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f65, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f64, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f66, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f68, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6a, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6c, 0x40);
    _HAL_DMD_RIU_WriteByte(0x111f6f, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f6e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f73, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f72, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f75, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f74, 0x44);      
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x44);   
    _HAL_DMD_RIU_WriteByte(0x111f79, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f78, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7b, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7a, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f7d, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7c, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111fe1, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111fe0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111ff0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe2, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111feb, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111fea, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fef, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fee, 0x88);
    _HAL_DMD_RIU_WriteByte(0x15298f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x15298e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152991, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152990, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152992, 0x04);
    _HAL_DMD_RIU_WriteByte(0x1529e5, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1529e4, 0x04);
    _HAL_DMD_RIU_WriteByte(0x152971, 0x10);
    _HAL_DMD_RIU_WriteByte(0x152970, 0x01);
  
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUSTANG)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MUSTANG--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);
    
    _HAL_DMD_RIU_WriteByte(0x1128d0, 0x01);
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
 
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);    
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1128d0, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x04); 
    _HAL_DMD_RIU_WriteByte(0x111f31, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f30, 0x01);   
    _HAL_DMD_RIU_WriteByte(0x111f3b, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);      
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x80);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f63, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f62, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f65, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f64, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f66, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f68, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6a, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6c, 0x40);
    _HAL_DMD_RIU_WriteByte(0x111f6f, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f6e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f73, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f72, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f75, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f74, 0x44);      
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x44);   
    _HAL_DMD_RIU_WriteByte(0x111f79, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f78, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7b, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7a, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f7d, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7c, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111fe1, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111fe0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111ff0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe2, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111feb, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111fea, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fef, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fee, 0x88);
    _HAL_DMD_RIU_WriteByte(0x15298f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x15298e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152991, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152990, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152992, 0x04);
    _HAL_DMD_RIU_WriteByte(0x1529e5, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1529e4, 0x04);
    _HAL_DMD_RIU_WriteByte(0x152971, 0x10);
    _HAL_DMD_RIU_WriteByte(0x152970, 0x01);
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MAXIM)
static void _HAL_INTERN_ISDBT_InitClk(void)
{    
    HAL_INTERN_ISDBT_DBINFO(printf("--------------DMD_ISDBT_CHIP_MAXIM--------------\n"));

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    _HAL_DMD_RIU_WriteByte(0x1128e5, 0x01); // DMD_ANA_ADC_SYNC CLK_W
    _HAL_DMD_RIU_WriteByte(0x1128e4, 0x01); // DMD_ANA_ADC_SYNC CLK_W
        
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x06); //ts clock = 7.2M 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x0b);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    //_HAL_DMD_RIU_WriteByte(0x103315, 0x00); //ADC SYNC FLOW
    //_HAL_DMD_RIU_WriteByte(0x103314, 0x00); //ADC SYNC FLOW
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);

    //_HAL_DMD_RIU_WriteByte(0x103321, 0x00); //Add in MAXIM //ADC SYNC FLOW
    //_HAL_DMD_RIU_WriteByte(0x103320, 0x00); //Add in MAXIM //ADC SYNC FLOW	
    
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);  

    _HAL_DMD_RIU_WriteByte(0x103321, 0x00); //ADC_SYNC CLK_W / DMD_ANA_ADC_SYNC CLK_R
    _HAL_DMD_RIU_WriteByte(0x103320, 0x00); //ADC_SYNC CLK_W / DMD_ANA_ADC_SYNC CLK_R

    _HAL_DMD_RIU_WriteByte(0x1128e4, 0x00); // DMD_ANA_ADC_SYNC CLK_W
          
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x04); 
    _HAL_DMD_RIU_WriteByte(0x111f31, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f30, 0x01);   
    _HAL_DMD_RIU_WriteByte(0x111f3b, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);      
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x80);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f63, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f62, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f65, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f64, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f66, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f68, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6a, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f6c, 0x40);
    _HAL_DMD_RIU_WriteByte(0x111f6f, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f6e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f73, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f72, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f75, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f74, 0x44);      
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x44);   
    _HAL_DMD_RIU_WriteByte(0x111f79, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f78, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7b, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7a, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f7d, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7c, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f7e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111fe1, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111fe0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111ff0, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111fe2, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111feb, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111fea, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fef, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111fee, 0x88);
    _HAL_DMD_RIU_WriteByte(0x15298f, 0x44);
    _HAL_DMD_RIU_WriteByte(0x15298e, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152991, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152990, 0x44);
    _HAL_DMD_RIU_WriteByte(0x152992, 0x04);
    _HAL_DMD_RIU_WriteByte(0x1529e5, 0x00);
    _HAL_DMD_RIU_WriteByte(0x1529e4, 0x04);
    _HAL_DMD_RIU_WriteByte(0x152971, 0x10);
    _HAL_DMD_RIU_WriteByte(0x152970, 0x01);
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
    //_HAL_DMD_RIU_WriteByteMask(0x103480, 0x00, 0x02);
}
#else
static void _HAL_INTERN_ISDBT_InitClk(void)
{
    printf("--------------DMD_ISDBT_CHIP_NONE--------------\n");
}
#endif

static MS_BOOL _HAL_INTERN_ISDBT_Ready(void)
{
    MS_U8 udata = 0x00;
    
    _HAL_DMD_RIU_WriteByte(MBRegBase + 0x1E, 0x02);
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51
    
    MsOS_DelayTask(1);
    
    udata = _HAL_DMD_RIU_ReadByte(MBRegBase + 0x1E);
    
    if (udata) return FALSE;
    
    return TRUE;
}

static MS_BOOL _HAL_INTERN_ISDBT_Download(void)
{
    DMD_ISDBT_ResData *pRes = psDMD_ISDBT_ResData + u8DMD_ISDBT_DMD_ID;
    
    MS_U8  udata = 0x00;
    MS_U16 i = 0;
    MS_U16 fail_cnt = 0;
    MS_U8  u8TmpData;
    MS_U16 u16AddressOffset;
    const MS_U8 *ISDBT_table;
    MS_U16 u16Lib_size;
    
    if (pRes->sDMD_ISDBT_PriData.bDownloaded)
    {
        if (_HAL_INTERN_ISDBT_Ready())
        {
            #if DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUSTANG
            _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x02, 0x02); // reset RIU remapping
            #endif
            _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x01, 0x01); // reset VD_MCU
            _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x00, 0x03); 
            
            MsOS_DelayTask(20);
            return TRUE;
        }
    }
    
    ISDBT_table = &INTERN_ISDBT_table[0];
    u16Lib_size = sizeof(INTERN_ISDBT_table);
    
    #if DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MUSTANG
    _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x02, 0x02); // reset RIU remapping
    #endif
    _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x01, 0x01); // reset VD_MCU
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x00); // disable SRAM
    
    _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x00, 0x01); // release MCU, madison patch
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // enable "vdmcu51_if"
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x51); // enable auto-increase
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    ////  Load code thru VDMCU_IF ////
    HAL_INTERN_ISDBT_DBINFO(printf(">Load Code...\n"));

    for (i = 0; i < u16Lib_size; i++)
    {
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, ISDBT_table[i]); // write data to VD MCU 51 code sram
    }

    ////  Content verification ////
    HAL_INTERN_ISDBT_DBINFO(printf(">Verify Code...\n"));

    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    for (i = 0; i < u16Lib_size; i++)
    {
        udata = _HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10); // read sram data

        if (udata != ISDBT_table[i])
        {
            HAL_INTERN_ISDBT_DBINFO(printf(">fail add = 0x%x\n", i));
            HAL_INTERN_ISDBT_DBINFO(printf(">code = 0x%x\n", INTERN_ISDBT_table[i]));
            HAL_INTERN_ISDBT_DBINFO(printf(">data = 0x%x\n", udata));

            if (fail_cnt++ > 10)
            {
                HAL_INTERN_ISDBT_DBINFO(printf(">DSP Loadcode fail!"));
                return FALSE;
            }
        }
    }
    
    u16AddressOffset = (ISDBT_table[0x400] << 8)|ISDBT_table[0x401];
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, (u16AddressOffset&0xFF)); // sram address low byte
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, (u16AddressOffset>>8));   // sram address high byte
     
    u8TmpData = (MS_U8)pRes->sDMD_ISDBT_InitData.u16IF_KHZ;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_ISDBT_InitData.u16IF_KHZ >> 8);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_ISDBT_InitData.bIQSwap;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_ISDBT_InitData.u16AgcReferenceValue;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_ISDBT_InitData.u16AgcReferenceValue >> 8);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_ISDBT_InitData.u32TdiStartAddr;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram 
    u8TmpData = (MS_U8)(pRes->sDMD_ISDBT_InitData.u32TdiStartAddr >> 8);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_ISDBT_InitData.u32TdiStartAddr >> 16);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_ISDBT_InitData.u32TdiStartAddr >> 24);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // diable auto-increase
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x00); // disable "vdmcu51_if"
    
    _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x01, 0x01); // reset MCU, madison patch
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x01); // enable SRAM
    _HAL_DMD_RIU_WriteByteMask(DMDMcuBase+0x00, 0x00, 0x03); // release VD_MCU
    
    pRes->sDMD_ISDBT_PriData.bDownloaded = true;
    
    MsOS_DelayTask(20);
    
    HAL_INTERN_ISDBT_DBINFO(printf(">DSP Loadcode done."));
    
    return TRUE;
}

static void _HAL_INTERN_ISDBT_FWVERSION(void)
{
    MS_U8 data1 = 0; 
    MS_U8 data2 = 0;
    MS_U8 data3 = 0;
    
    _MBX_ReadReg(0x20C4, &data1);
    _MBX_ReadReg(0x20C5, &data2);
    _MBX_ReadReg(0x20C6, &data3);

    printf("INTERN_ISDBT_FW_VERSION:%x.%x.%x\n", data1, data2, data3);
}

static MS_BOOL _HAL_INTERN_ISDBT_Exit(void)
{
    MS_U8 u8CheckCount = 0;
    #if (DMD_ISDBT_CHIP_VERSION < DMD_ISDBT_CHIP_MULAN) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MESSI) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_KIWI)
    MS_U8 u8RegValTmp = 0;
    
    u8RegValTmp = _HAL_DMD_RIU_ReadByte(0x101200+ISDBT_MIU_CLIENTW_ADDR);
    if (u8RegValTmp & ISDBT_MIU_CLIENTW_BIT_MASK)
    {
       _HAL_DMD_RIU_WriteByteMask(0x100600+ISDBT_MIU_CLIENTW_MASK, ISDBT_MIU_CLIENTW_BIT_MASK, ISDBT_MIU_CLIENTW_BIT_MASK);
       _HAL_DMD_RIU_WriteByteMask(0x100600+ISDBT_MIU_CLIENTR_MASK, ISDBT_MIU_CLIENTR_BIT_MASK, ISDBT_MIU_CLIENTR_BIT_MASK);
    }
    else
    {
       _HAL_DMD_RIU_WriteByteMask(0x101200+ISDBT_MIU_CLIENTW_MASK, ISDBT_MIU_CLIENTW_BIT_MASK, ISDBT_MIU_CLIENTW_BIT_MASK);
       _HAL_DMD_RIU_WriteByteMask(0x101200+ISDBT_MIU_CLIENTR_MASK, ISDBT_MIU_CLIENTR_BIT_MASK, ISDBT_MIU_CLIENTR_BIT_MASK);
    }
    #endif
    _HAL_DMD_RIU_WriteByte(MBRegBase + 0x1C, 0x01);

    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51
    
    while ((_HAL_DMD_RIU_ReadByte(MBRegBase + 0x1C)&0x02) != 0x02)
    {
        MsOS_DelayTaskUs(10);

        if (u8CheckCount++ == 0xFF)
        {
            printf(">> ISDBT Exit Fail!\n");
            return FALSE;
        }
    }
    
    printf(">> ISDBT Exit Ok!\n");
    
    return TRUE;
}

static MS_BOOL _HAL_INTERN_ISDBT_SoftReset(void)
{
    MS_U8 u8Data = 0;

    #if (DMD_ISDBT_CHIP_VERSION < DMD_ISDBT_CHIP_MULAN) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MESSI) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_KIWI)
    MS_U8 u8RegValTmp = 0;
    
    u8RegValTmp = _HAL_DMD_RIU_ReadByte(0x101200+ISDBT_MIU_CLIENTW_ADDR);
    if (u8RegValTmp & ISDBT_MIU_CLIENTW_BIT_MASK)
    {
       _HAL_DMD_RIU_WriteByteMask(0x100600+ISDBT_MIU_CLIENTW_MASK, ISDBT_MIU_CLIENTW_BIT_MASK, ISDBT_MIU_CLIENTW_BIT_MASK);
       _HAL_DMD_RIU_WriteByteMask(0x100600+ISDBT_MIU_CLIENTR_MASK, ISDBT_MIU_CLIENTR_BIT_MASK, ISDBT_MIU_CLIENTR_BIT_MASK);
    }
    else
    {
       _HAL_DMD_RIU_WriteByteMask(0x101200+ISDBT_MIU_CLIENTW_MASK, ISDBT_MIU_CLIENTW_BIT_MASK, ISDBT_MIU_CLIENTW_BIT_MASK);
       _HAL_DMD_RIU_WriteByteMask(0x101200+ISDBT_MIU_CLIENTR_MASK, ISDBT_MIU_CLIENTR_BIT_MASK, ISDBT_MIU_CLIENTR_BIT_MASK);
    }
    #endif
    
    //Reset FSM
    if (_MBX_WriteReg(0x20C0, 0x00)==FALSE) return FALSE;
    
    while (u8Data!=0x02)
    {
        if (_MBX_ReadReg(0x20C1, &u8Data)==FALSE) return FALSE;
    }
    
    return TRUE;
}

static MS_BOOL _HAL_INTERN_ISDBT_SetACICoef(void)
{
    return TRUE;
}

static MS_BOOL _HAL_INTERN_ISDBT_SetIsdbtMode(void)
{
    #if (DMD_ISDBT_CHIP_VERSION < DMD_ISDBT_CHIP_MULAN) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_MESSI) || (DMD_ISDBT_CHIP_VERSION == DMD_ISDBT_CHIP_KIWI)
    MS_U8 u8RegValTmp = 0;
    
    u8RegValTmp = _HAL_DMD_RIU_ReadByte(0x101200+ISDBT_MIU_CLIENTW_ADDR);
    if (u8RegValTmp & ISDBT_MIU_CLIENTW_BIT_MASK)
    {
       _HAL_DMD_RIU_WriteByteMask(0x100600+ISDBT_MIU_CLIENTW_MASK, 0, ISDBT_MIU_CLIENTW_BIT_MASK);
       _HAL_DMD_RIU_WriteByteMask(0x100600+ISDBT_MIU_CLIENTR_MASK, 0, ISDBT_MIU_CLIENTR_BIT_MASK);
    }
    else
    {
       _HAL_DMD_RIU_WriteByteMask(0x101200+ISDBT_MIU_CLIENTW_MASK, 0, ISDBT_MIU_CLIENTW_BIT_MASK);
       _HAL_DMD_RIU_WriteByteMask(0x101200+ISDBT_MIU_CLIENTR_MASK, 0, ISDBT_MIU_CLIENTR_BIT_MASK);
    }
    #endif
    
    if (_MBX_WriteReg(0x20C2, 0x04)==FALSE) return FALSE;
    return _MBX_WriteReg(0x20C0, 0x04);
}

static MS_BOOL _HAL_INTERN_ISDBT_SetModeClean(void)
{
    if (_MBX_WriteReg(0x20C2, 0x07)==FALSE) return FALSE;
    return _MBX_WriteReg(0x20C0, 0x00);
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_FEC_Lock(void)
{
    MS_BOOL bCheckPass = FALSE;
    MS_U8   u8Data = 0;       

    _MBX_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    
    if ((u8Data & 0x02) != 0x00) // Check FEC Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_FSA_TRACK_Lock(void)
{
    MS_BOOL bCheckPass = FALSE;
    MS_U8   u8Data = 0;       

    _MBX_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    
    if ((u8Data & 0x01) != 0x00) // Check FSA Track Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_PSYNC_Lock(void)
{
    MS_BOOL bCheckPass = FALSE;
    MS_U8   u8Data = 0;       

    _MBX_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    
    if ((u8Data & 0x04) != 0x00) // Check Psync Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_Check_ICFO_CH_EXIST_Lock(void)
{
    MS_BOOL bCheckPass = FALSE;
    MS_U8   u8Data = 0;    

    _MBX_ReadReg(REG_ISDBT_LOCK_STATUS, &u8Data);
    
    if ((u8Data & 0x80) != 0x00) // Check Psync Lock Flag
        bCheckPass = TRUE;

    return bCheckPass;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalCodeRate(EN_ISDBT_Layer eLayerIndex, EN_ISDBT_CODE_RATE *peIsdbtCodeRate)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data = 0;
    MS_U8 u8CodeRate = 0;
    
    switch (eLayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [10:8] reg_tmcc_cur_convolution_code_rate_a
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x04*2+1, &u8Data);
            u8CodeRate = u8Data & 0x07;
            break;
        case E_ISDBT_Layer_B:
            // [10:8] reg_tmcc_cur_convolution_code_rate_b
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x05*2+1, &u8Data);
            u8CodeRate = u8Data & 0x07;
            break;
       case E_ISDBT_Layer_C:
            // [10:8] reg_tmcc_cur_convolution_code_rate_c
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x06*2+1, &u8Data);
            u8CodeRate = u8Data & 0x07;
            break;
       default:
            u8CodeRate = 15;
            break;
    }

    switch (u8CodeRate)
    {
        case 0:
            *peIsdbtCodeRate = E_ISDBT_CODERATE_1_2;
            break;
        case 1:
            *peIsdbtCodeRate = E_ISDBT_CODERATE_2_3;
            break;
        case 2:
            *peIsdbtCodeRate = E_ISDBT_CODERATE_3_4;
            break;
        case 3:
            *peIsdbtCodeRate = E_ISDBT_CODERATE_5_6;
            break;
        case 4:
            *peIsdbtCodeRate = E_ISDBT_CODERATE_7_8;
            break;
        default:
            *peIsdbtCodeRate = E_ISDBT_CODERATE_INVALID;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalGuardInterval(EN_ISDBT_GUARD_INTERVAL *peIsdbtGI)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data = 0;
    MS_U8 u8CP = 0;

    // [7:6] reg_mcd_out_cp
    // output cp -> 00: 1/4
    //                    01: 1/8
    //                    10: 1/16
    //                    11: 1/32
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE+0x34*2, &u8Data);
    
    u8CP  = (u8Data >> 6) & 0x03;
    
    switch (u8CP)
    {
        case 0:
            *peIsdbtGI = E_ISDBT_GUARD_INTERVAL_1_4;
            break;
        case 1:
            *peIsdbtGI = E_ISDBT_GUARD_INTERVAL_1_8;
            break;
        case 2:
            *peIsdbtGI = E_ISDBT_GUARD_INTERVAL_1_16;
            break;
        case 3:
            *peIsdbtGI = E_ISDBT_GUARD_INTERVAL_1_32;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalTimeInterleaving(EN_ISDBT_Layer eLayerIndex, EN_ISDBT_TIME_INTERLEAVING *peIsdbtTDI)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data = 0;
    MS_U8 u8Mode = 0;
    MS_U8 u8Tdi = 0;
    
    // [5:4] reg_mcd_out_mode
    // output mode  -> 00: 2k
    //                         01: 4k
    //                         10: 8k
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE+0x34*2, &u8Data);
    
    u8Mode  = (u8Data >> 4) & 0x03;

    switch (eLayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [14:12] reg_tmcc_cur_interleaving_length_a
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x04*2+1, &u8Data);
            u8Tdi = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_B:
            // [14:12] reg_tmcc_cur_interleaving_length_b
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x05*2+1, &u8Data);
            u8Tdi = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_C:
            // [14:12] reg_tmcc_cur_interleaving_length_c
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x06*2+1, &u8Data);
            u8Tdi = (u8Data >> 4) & 0x07;
            break;
       default:
            u8Tdi = 15;
            break;
    }

    // u8Tdi+u8Mode*4
    // => 0~3: 2K
    // => 4~7: 4K
    // => 8~11:8K
    switch (u8Tdi+u8Mode*4)
    {
        case 0:
            *peIsdbtTDI = E_ISDBT_2K_TDI_0;
            break;
        case 1:
            *peIsdbtTDI = E_ISDBT_2K_TDI_4;
            break;
        case 2:
            *peIsdbtTDI = E_ISDBT_2K_TDI_8;
            break;
        case 3:
            *peIsdbtTDI = E_ISDBT_2K_TDI_16;
            break;
        case 4:
            *peIsdbtTDI = E_ISDBT_4K_TDI_0;
            break;
        case 5:
            *peIsdbtTDI = E_ISDBT_4K_TDI_2;
            break;
        case 6:
            *peIsdbtTDI = E_ISDBT_4K_TDI_4;
            break;
        case 7:
            *peIsdbtTDI = E_ISDBT_4K_TDI_8;
            break;
        case 8:
            *peIsdbtTDI = E_ISDBT_8K_TDI_0;
            break;
        case 9:
            *peIsdbtTDI = E_ISDBT_8K_TDI_1;
            break;
        case 10:
            *peIsdbtTDI = E_ISDBT_8K_TDI_2;
            break;
        case 11:
            *peIsdbtTDI = E_ISDBT_8K_TDI_4;
            break;
        default:
            *peIsdbtTDI = E_ISDBT_TDI_INVALID;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalFFTValue(EN_ISDBT_FFT_VAL *peIsdbtFFT)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data = 0;
    MS_U8 u8Mode = 0;
    
    // [5:4]  reg_mcd_out_mode
    // output mode  -> 00: 2k
    //                         01: 4k
    //                         10: 8k
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE+0x34*2, &u8Data);
    
    u8Mode  = (u8Data >> 4) & 0x03;

    switch (u8Mode)
    {
        case 0:
            *peIsdbtFFT = E_ISDBT_FFT_2K;
            break;
        case 1:
            *peIsdbtFFT = E_ISDBT_FFT_4K;
            break;
        case 2:
            *peIsdbtFFT = E_ISDBT_FFT_8K;
            break;
        default:
            *peIsdbtFFT = E_ISDBT_FFT_INVALID;
            break;
    }

    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_GetSignalModulation(EN_ISDBT_Layer eLayerIndex, EN_ISDBT_CONSTEL_TYPE *peIsdbtConstellation)
{
    MS_BOOL bRet = TRUE;
    MS_U8 u8Data = 0;
    MS_U8 u8QAM = 0;

    switch(eLayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [6:4] reg_tmcc_cur_carrier_modulation_a
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x04*2, &u8Data);
            u8QAM = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_B:
            // [6:4] reg_tmcc_cur_carrier_modulation_b
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x05*2, &u8Data);
            u8QAM = (u8Data >> 4) & 0x07;
            break;
        case E_ISDBT_Layer_C:
            // [6:4] reg_tmcc_cur_carrier_modulation_c
            bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE+0x06*2, &u8Data);
            u8QAM = (u8Data >> 4) & 0x07;
            break;
        default:
            u8QAM = 15;
            break;
    }

    switch(u8QAM)
    {
        case 0:
            *peIsdbtConstellation = E_ISDBT_DQPSK;
            break;
        case 1:
            *peIsdbtConstellation = E_ISDBT_QPSK;
            break;
        case 2:
            *peIsdbtConstellation = E_ISDBT_16QAM;
            break;
        case 3:
            *peIsdbtConstellation = E_ISDBT_64QAM;
            break;
        default:
            *peIsdbtConstellation = E_ISDBT_QAM_INVALID;
            break;
    }

    return bRet;
}

static MS_U8 _HAL_INTERN_ISDBT_ReadIFAGC(void)
{
    MS_U8 data = 0;
    
    _MBX_ReadReg(0x28FD, &data);

    return data;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ISDBT_GetFreqOffset(MS_U8 *pFFT_Mode, MS_S32 *pTdCfoRegValue, MS_S32 *pFdCfoRegValue, MS_S16 *pIcfoRegValue)
#else
static MS_BOOL _HAL_INTERN_ISDBT_GetFreqOffset(float *pFreqOff)
#endif
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8Data = 0;
    MS_S32  s32TdCfoRegValue = 0;    
    MS_S32  s32FdCfoRegValue = 0;    
    MS_S16  s16IcfoRegValue = 0;
    #ifndef UTPA2
    float   fTdCfoFreq = 0.0;
    float   fICfoFreq = 0.0;
    float   fFdCfoFreq = 0.0;
    #endif
    
    //Get TD CFO
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x04, &u8Data);   //0x02 * 2
    bRet &= _MBX_WriteReg(ISDBT_TDP_REG_BASE + 0x04, (u8Data|0x01));

    //read td_freq_error
    //Read <29,38>
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x8A, &u8Data);   //0x45 * 2
    s32TdCfoRegValue = u8Data;
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x8B, &u8Data);   //0x45 * 2 + 1
    s32TdCfoRegValue |= u8Data << 8;
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x8C, &u8Data);   //0x46 * 2
    s32TdCfoRegValue = u8Data << 16;
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x8D, &u8Data);   //0x46 * 2 + 1
    s32TdCfoRegValue |= u8Data << 24;

    if (u8Data >= 0x10)
        s32TdCfoRegValue = 0xE0000000 | s32TdCfoRegValue;
    
    s32TdCfoRegValue >>=4;
    
    //TD_cfo_Hz = RegCfoTd * fb
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x04, &u8Data);   //0x02 * 2
    bRet &= _MBX_WriteReg(ISDBT_TDP_REG_BASE + 0x04, (u8Data&~0x01));

    #ifndef UTPA2
    fTdCfoFreq = ((float)s32TdCfoRegValue) / 17179869184.0; //<25,34>
    fTdCfoFreq = fTdCfoFreq * 8126980.0;
    #endif
    
    //Get FD CFO
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data|0x01));
    //load
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    //read CFO_KI
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0x5E, &u8Data);   //0x2F * 2
    s32FdCfoRegValue = u8Data;
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0x5F, &u8Data);   //0x2F * 2 + 1
    s32FdCfoRegValue |= u8Data << 8;
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0x60, &u8Data);   //0x30 * 2
    s32FdCfoRegValue |= u8Data << 16;
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0x61, &u8Data);   //0x30 * 2
    s32FdCfoRegValue |= u8Data << 24;
    
    if(u8Data >= 0x01)
        s32FdCfoRegValue = 0xFE000000 | s32FdCfoRegValue;
    
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data&~0x01));
    //load
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    #ifndef UTPA2
    fFdCfoFreq = ((float)s32FdCfoRegValue) / 17179869184.0;
    fFdCfoFreq = fFdCfoFreq * 8126980.0;
    #endif
    
    //Get ICFO
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0x5C, &u8Data);   //0x2E * 2
    s16IcfoRegValue = u8Data;
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0x5D, &u8Data);   //0x2E * 2 + 1
    s16IcfoRegValue |= u8Data << 8;
    s16IcfoRegValue = (s16IcfoRegValue >> 4) & 0x07FF;
    
    if(s16IcfoRegValue >= 0x400)
        s16IcfoRegValue = s16IcfoRegValue | 0xFFFFF800;
    
    bRet &= _MBX_ReadReg(ISDBT_TDP_REG_BASE + 0x68, &u8Data);   //0x34 * 2

    #ifdef UTPA2
    *pFFT_Mode = u8Data;
    *pTdCfoRegValue = s32TdCfoRegValue;
    *pFdCfoRegValue = s32TdCfoRegValue;
    *pIcfoRegValue = s16IcfoRegValue;
    #else
    if((u8Data & 0x30) == 0x0000) // 2k
        fICfoFreq = (float)s16IcfoRegValue*250000.0/63.0;
    else if((u8Data & 0x0030) == 0x0010)	// 4k
        fICfoFreq = (float)s16IcfoRegValue*125000.0/63.0;
    else //if(u16data & 0x0030 == 0x0020) // 8k
        fICfoFreq = (float)s16IcfoRegValue*125000.0/126.0;

    *pFreqOff = fTdCfoFreq + fFdCfoFreq + fICfoFreq;

    HAL_INTERN_ISDBT_DBINFO(printf("Total CFO value = %f\n", *pFreqOff));
    #endif
    
    return bRet;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer eLayerIndex, MS_U32 *pBerValue, MS_U16 *pBerPeriod )
#else
static MS_BOOL _HAL_INTERN_ISDBT_GetPreViterbiBer(EN_ISDBT_Layer eLayerIndex, float *pfber)
#endif
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8Data = 0;
    MS_U16  u16BerValue = 0;
    MS_U32  u32BerPeriod = 0;
    
    // reg_rd_freezeber
    bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x60, &u8Data);
    bRet &= _MBX_WriteReg(ISDBT_OUTER_REG_BASE + 0x60, u8Data|0x08);

    if (eLayerIndex == E_ISDBT_Layer_A)
    {
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x90, &u8Data);  //0x48 * 2
        u16BerValue=u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x91, &u8Data);  //0x48 * 2+1
        u16BerValue |= (u8Data << 8);
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x76, &u8Data); //0x3b * 2
        u32BerPeriod = (u8Data&0x3F);
        u32BerPeriod <<= 16;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x70, &u8Data); //0x38 * 2
        u32BerPeriod |= u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x70, &u8Data); //0x38 * 2 +1
        u32BerPeriod |= (u8Data << 8);
    }
    else if (eLayerIndex == E_ISDBT_Layer_B)
    {
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x92, &u8Data);  //0x49 * 2
        u16BerValue=u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x93, &u8Data);  //0x49 * 2+1
        u16BerValue |= (u8Data << 8);
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x77, &u8Data); //0x3b * 2 + 1
        u32BerPeriod = (u8Data&0x3F);
        u32BerPeriod <<= 16;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x72, &u8Data); //0x39 * 2
        u32BerPeriod |= u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x73, &u8Data); //0x39 * 2 +1
        u32BerPeriod |= (u8Data << 8);
    }
    else if (eLayerIndex == E_ISDBT_Layer_C)
    {
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x94, &u8Data);  //0x4A * 2
        u16BerValue=u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x95, &u8Data);  //0x4A * 2+1
        u16BerValue |= (u8Data << 8);
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x78, &u8Data); //0x3C
        u32BerPeriod = (u8Data&0x003F);
        u32BerPeriod <<= 16;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x74, &u8Data); //0x3A * 2
        u32BerPeriod |= u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x75, &u8Data); //0x3A * 2 +1
        u32BerPeriod |= (u8Data << 8);
    }
    else
    {
        HAL_INTERN_ISDBT_DBINFO(printf("Please select correct Layer\n"));
        bRet = FALSE;
    }

    // reg_rd_freezeber
    bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x60, &u8Data);
    bRet &= _MBX_WriteReg(ISDBT_OUTER_REG_BASE + 0x60, (u8Data&~0x08));

    u32BerPeriod <<= 8; // *256
    
    if(u32BerPeriod == 0) u32BerPeriod = 1;
    
    #ifdef UTPA2
    *pBerPeriod = u32BerPeriod;
    *pBerValue = u16BerValue;
    #else
    *pfber = (float)u16BerValue/u32BerPeriod;      
    HAL_INTERN_ISDBT_DBINFO(printf("Layer: 0x%x, Pre-Ber = %e\n", eLayerIndex, *pfber));
    #endif
    
    return bRet;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer eLayerIndex, MS_U32 *pBerValue, MS_U16 *pBerPeriod )
#else
static MS_BOOL _HAL_INTERN_ISDBT_GetPostViterbiBer(EN_ISDBT_Layer eLayerIndex, float *pfber)
#endif
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8Data = 0;
    MS_U8   u8FrzData = 0;
    MS_U32  u32BerValue = 0;
    MS_U16  u16BerPeriod = 0;
    
    // reg_rd_freezeber
    bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x01*2+1, &u8FrzData);
    u8Data = u8FrzData | 0x01;
    bRet &= _MBX_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8Data);
    
    if (eLayerIndex == E_ISDBT_Layer_A)
    {
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x14, &u8Data);  //0x0A * 2
        u32BerValue = u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x15, &u8Data);  //0x0A * 2+1
        u32BerValue |= u8Data << 8;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x16, &u8Data);  //0x0B * 2
        u32BerValue |= u8Data << 16;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x17, &u8Data);  //0x0B * 2+1
        u32BerValue |= u8Data << 24;

        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x0A, &u8Data);  //0x05 * 2
        u16BerPeriod = u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x0B, &u8Data);  //0x05 * 2+1
        u16BerPeriod |= u8Data << 8;
    }
    else if (eLayerIndex == E_ISDBT_Layer_B)
    {
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x46, &u8Data);  //0x23 * 2
        u32BerValue = u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x47, &u8Data);  //0x23 * 2+1
        u32BerValue |= u8Data << 8;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x48, &u8Data);  //0x24 * 2
        u32BerValue |= u8Data << 16;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x49, &u8Data);  //0x24 * 2+1
        u32BerValue |= u8Data << 24;

        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x3A, &u8Data);  //0x1d * 2
        u16BerPeriod = u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x3B, &u8Data);  //0x1d * 2+1
        u16BerPeriod |= u8Data << 8;
    }
    else if (eLayerIndex == E_ISDBT_Layer_C)
    {
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x88, &u8Data);  //0x44 * 2
        u32BerValue = u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x89, &u8Data);  //0x44 * 2+1
        u32BerValue |= u8Data << 8;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x8A, &u8Data);  //0x45 * 2
        u32BerValue |= u8Data << 16;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x8B, &u8Data);  //0x45 * 2+1
        u32BerValue |= u8Data << 24;

        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x3E, &u8Data);  //0x1f * 2
        u16BerPeriod = u8Data;
        bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE + 0x3F, &u8Data);  //0x1d * 2+1
        u16BerPeriod |= u8Data << 8;
    }
    else
    {
        HAL_INTERN_ISDBT_DBINFO(printf("Please select correct Layer\n"));
        bRet = FALSE;
    }

    // reg_rd_freezeber
    bRet &= _MBX_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8FrzData);

    if(u16BerPeriod == 0) u16BerPeriod = 1;

    #ifdef UTPA2
    *pBerPeriod = u16BerPeriod;
    *pBerValue = u32BerValue;
    #else
    *pfber = (float)u32BerValue/u16BerPeriod/(128.0*188.0*8.0);       
    HAL_INTERN_ISDBT_DBINFO(printf("Layer: 0x%x, Post-Ber = %e\n", eLayerIndex, *pfber));
    #endif
    return bRet;
}

#ifndef UTPA2
static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA(void)
{
    float fber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer eLayerIndex;
    MS_U16 u16SQI;

    // Tmp solution
    eLayerIndex = E_ISDBT_Layer_A;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {
        //printf("Dan Demod unlock!!!\n");
        u16SQI = 0; 
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(eLayerIndex, &fber);
        
        u16SQI = _CALCULATE_SQI(fber);        
    }
    
    //printf("dan SQI = %d\n", SQI);
    return u16SQI;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB(void)
{
    float fber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer eLayerIndex;
    MS_U16 u16SQI;

    // Tmp solution
    eLayerIndex = E_ISDBT_Layer_B;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {  
        //printf("Dan Demod unlock!!!\n");
        u16SQI = 0; 
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(eLayerIndex, &fber);
        
        u16SQI = _CALCULATE_SQI(fber);    
    }
    
    //printf("dan SQI = %d\n", SQI);
    return u16SQI;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC(void)
{
    float fber;
    MS_BOOL bRet = TRUE;
    EN_ISDBT_Layer eLayerIndex;
    MS_U16 u16SQI;

    // Tmp solution
    eLayerIndex = E_ISDBT_Layer_C;

    if(_HAL_INTERN_ISDBT_Check_FEC_Lock() == FALSE)
    {  
        //printf("Dan Demod unlock!!!\n");
        u16SQI = 0; 
    }
    else
    {
        // Part 1: get ber value from demod.
        bRet &= _HAL_INTERN_ISDBT_GetPostViterbiBer(eLayerIndex, &fber);
        
        u16SQI = _CALCULATE_SQI(fber);    
    }
    
    //printf("dan SQI = %d\n", SQI);
    return u16SQI;
}

static MS_U16 _HAL_INTERN_ISDBT_GetSignalQualityOfLayerCombine(void)
{
    MS_S8  s8LayerAValue = 0, s8LayerBValue = 0, s8LayerCValue = 0;
    MS_U16 u16SQI;
    EN_ISDBT_Layer eLayerIndex; 
    EN_ISDBT_CONSTEL_TYPE eIsdbtConstellationA, eIsdbtConstellationB, eIsdbtConstellationC;
    
    //Get modulation of each layer
    eLayerIndex = E_ISDBT_Layer_A;
    _HAL_INTERN_ISDBT_GetSignalModulation(eLayerIndex, &eIsdbtConstellationA);	
    eLayerIndex = E_ISDBT_Layer_B;
    _HAL_INTERN_ISDBT_GetSignalModulation(eLayerIndex, &eIsdbtConstellationB);
    eLayerIndex = E_ISDBT_Layer_C;
    _HAL_INTERN_ISDBT_GetSignalModulation(eLayerIndex, &eIsdbtConstellationC);
    
    if (eIsdbtConstellationA != E_ISDBT_QAM_INVALID)
        s8LayerAValue = (MS_S8)eIsdbtConstellationA;
    else
        s8LayerAValue = -1;
    	       
    if (eIsdbtConstellationB != E_ISDBT_QAM_INVALID)
        s8LayerBValue = (MS_S8)eIsdbtConstellationB;
    else
        s8LayerBValue = -1;
        
    if (eIsdbtConstellationC != E_ISDBT_QAM_INVALID)
        s8LayerCValue = (MS_S8)eIsdbtConstellationC;
    else
        s8LayerCValue = -1;
    	   
    //printf("Layer info A:%d, B:%d, C:%d\n", s8LayerAValue, s8LayerBValue, s8LayerCValue);	   
    if (s8LayerAValue >= s8LayerBValue)
    {
        if (s8LayerCValue >= s8LayerAValue)
        {
            //Get Layer C u16SQI	
            u16SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC();
            //printf("dan u16SQI Layer C1: %d\n", u16SQI);
        }	
        else  //A>C
        {
            //Get Layer A u16SQI
            u16SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA();
            //printf("dan u16SQI Layer A: %d\n", u16SQI);		
        }		
    }
    else  // B >= A
    {
        if (s8LayerCValue >= s8LayerBValue)
        {
            //Get Layer C u16SQI	
            u16SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC();
            //printf("dan u16SQI Layer C2: %d\n", u16SQI);
        }	
        else  //B>C
        {
            //Get Layer B u16SQI
            u16SQI = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB();
            //printf("dan u16SQI Layer B: %d\n", u16SQI);
        }			
    }
    
    return u16SQI;
}
#endif

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ISDBT_GetSNR(MS_U32 *pRegSNR, MS_U16 *pRegSnrObsNum)
#else
static MS_BOOL _HAL_INTERN_ISDBT_GetSNR(float *pf_snr)
#endif
{
    MS_BOOL bRet = TRUE;
    MS_U8   u8Data = 0;
    MS_U32  u32RegSNR = 0;
    MS_U16  u16RegSnrObsNum = 0;
    #ifndef UTPA2
    float   fSNRAvg = 0.0;
    #endif

    //set freeze
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data|0x01));
    //load
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    // ==============Average SNR===============//
    // [26:0] reg_snr_accu
    bRet &= _MBX_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2d*2+1, &u8Data);
    u32RegSNR = u8Data&0x07;
    bRet &= _MBX_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2d*2, &u8Data);
    u32RegSNR = (u32RegSNR<<8) | u8Data;
    bRet &= _MBX_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2c*2+1, &u8Data);
    u32RegSNR = (u32RegSNR<<8) | u8Data;
    bRet &= _MBX_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2c*2, &u8Data);
    u32RegSNR = (u32RegSNR<<8) | u8Data;

    // [12:0] reg_snr_observe_sum_num
    bRet &= _MBX_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2a*2+1, &u8Data);
    u16RegSnrObsNum = u8Data&0x1f;
    bRet &= _MBX_ReadReg(ISDBT_FDPEXT_REG_BASE+0x2a*2, &u8Data);
    u16RegSnrObsNum = (u16RegSnrObsNum<<8) | u8Data;

    //release freeze
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFE, &u8Data);   //0x7f * 2
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFE, (u8Data&~0x01));
    //load
    bRet &= _MBX_ReadReg(ISDBT_FDP_REG_BASE + 0xFF, &u8Data);   //0x7f * 2 + 1
    bRet &= _MBX_WriteReg(ISDBT_FDP_REG_BASE + 0xFF, (u8Data|0x01));

    if (u16RegSnrObsNum == 0)
        u16RegSnrObsNum = 1;


    #ifdef UTPA2
     *pRegSNR = u32RegSNR;
     *pRegSnrObsNum = u16RegSnrObsNum;
    #else
     fSNRAvg = (float)u32RegSNR/u16RegSnrObsNum;
     if (fSNRAvg == 0)                 //protect value 0
         fSNRAvg = 0.01;

     #ifdef MSOS_TYPE_LINUX
     *pf_snr = 10.0f*(float)log10f((double)fSNRAvg/2);
     #else
     *pf_snr = 10.0f*(float)Log10Approx((double)fSNRAvg/2);
     #endif
     HAL_INTERN_ISDBT_DBINFO(printf("SNR value = %f\n", *pf_snr));
    #endif
        
    return bRet;
}

static MS_BOOL _HAL_INTERN_ISDBT_Read_PKT_ERR(EN_ISDBT_Layer eLayerIndex, MS_U16 *pu16PacketErr)
{
    MS_U8 bRet = true;
    MS_U8 u8Data = 0;
    MS_U8 u8FrzData = 0;
    MS_U16 u16PacketErrA = 0xFFFF, u16PacketErrB = 0xFFFF, u16PacketErrC = 0xFFFF;
    #if DMD_ISDBT_TBVA_EN
    MS_U8 bTbvaBypass = 0;
    MS_U8 u8TbvaLayer = 0;
    #endif
    // Read packet errors of three layers
    // OUTER_FUNCTION_ENABLE
    // [8] reg_biterr_num_pcktprd_freeze
    // Freeze Packet error
    bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x01*2+1, &u8FrzData);
    u8Data = u8FrzData | 0x01;
    bRet &= _MBX_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8Data);
#if DMD_ISDBT_TBVA_EN
    bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x10*2, &u8Data);
    bTbvaBypass = u8Data & 0x01;
    bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x11*2, &u8Data);
    u8TbvaLayer = u8Data & 0x03;
    switch(eLayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_A
            if (!bTbvaBypass && u8TbvaLayer == 0)
            {
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x17*2+1, &u8Data);
                u16PacketErrA = u8Data << 8;
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x17*2, &u8Data);
                u16PacketErrA = u16PacketErrA | u8Data;
                *pu16PacketErr = u16PacketErrA;
            }
            else
            {
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x08*2+1, &u8Data);
                u16PacketErrA = u8Data << 8;
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x08*2, &u8Data);
                u16PacketErrA = u16PacketErrA | u8Data;
                *pu16PacketErr = u16PacketErrA;
            }    
            break;
        case E_ISDBT_Layer_B:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_B
            if (!bTbvaBypass && u8TbvaLayer == 1)
            {
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x17*2+1, &u8Data);
                u16PacketErrB = u8Data << 8;
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x17*2, &u8Data);
                u16PacketErrB = u16PacketErrB | u8Data;
                *pu16PacketErr = u16PacketErrB;
            }
            else
            {
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x21*2+1, &u8Data);
                u16PacketErrB = u8Data << 8;
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x21*2, &u8Data);
                u16PacketErrB = u16PacketErrB | u8Data;
                *pu16PacketErr = u16PacketErrB;
            }
            break;
        case E_ISDBT_Layer_C:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_C
            if (!bTbvaBypass && u8TbvaLayer == 2)
            {
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x17*2+1, &u8Data);
                u16PacketErrC = u8Data << 8;
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x17*2, &u8Data);
                u16PacketErrC = u16PacketErrC | u8Data;
                *pu16PacketErr = u16PacketErrC;
            }
            else
            {            
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x42*2+1, &u8Data);
                u16PacketErrC = u8Data << 8;
                bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x42*2, &u8Data);
                u16PacketErrC = u16PacketErrC | u8Data;
                *pu16PacketErr = u16PacketErrC;
            }    
            break;
        default:
            *pu16PacketErr = 0xFFFF;
            break;
    }
#else
    switch(eLayerIndex)
    {
        case E_ISDBT_Layer_A:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_A
            bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x08*2+1, &u8Data);
            u16PacketErrA = u8Data << 8;
            bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x08*2, &u8Data);
            u16PacketErrA = u16PacketErrA | u8Data;
            *pu16PacketErr = u16PacketErrA;
            break;
        case E_ISDBT_Layer_B:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_B
            bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x21*2+1, &u8Data);
            u16PacketErrB = u8Data << 8;
            bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x21*2, &u8Data);
            u16PacketErrB = u16PacketErrB | u8Data;
            *pu16PacketErr = u16PacketErrB;
            break;
        case E_ISDBT_Layer_C:
            // [15:0] OUTER_UNCRT_PKT_NUM_PCKTPRD_C
            bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x42*2+1, &u8Data);
            u16PacketErrC = u8Data << 8;
            bRet &= _MBX_ReadReg(ISDBT_OUTER_REG_BASE+0x42*2, &u8Data);
            u16PacketErrC = u16PacketErrC | u8Data;
            *pu16PacketErr = u16PacketErrC;
            break;
        default:
            *pu16PacketErr = 0xFFFF;
            break;
    }
#endif
    // Unfreeze Packet error
    bRet &= _MBX_WriteReg(ISDBT_OUTER_REG_BASE+0x01*2+1, u8FrzData);

    return bRet;  
}

static MS_BOOL _HAL_INTERN_ISDBT_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return _MBX_ReadReg(u16Addr, pu8Data);
}

static MS_BOOL _HAL_INTERN_ISDBT_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return _MBX_WriteReg(u16Addr, u8Data);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_INTERN_ISDBT_IOCTL_CMD(DMD_ISDBT_HAL_COMMAND eCmd, void *pArgs)
{
    MS_BOOL bResult = TRUE;
    
    switch(eCmd)
    {
    case DMD_ISDBT_HAL_CMD_Exit:
        bResult = _HAL_INTERN_ISDBT_Exit();
        break;
    case DMD_ISDBT_HAL_CMD_InitClk:
        _HAL_INTERN_ISDBT_InitClk();
        break;
    case DMD_ISDBT_HAL_CMD_Download:
        bResult = _HAL_INTERN_ISDBT_Download();
        break;
    case DMD_ISDBT_HAL_CMD_FWVERSION:
        _HAL_INTERN_ISDBT_FWVERSION();
        break;
    case DMD_ISDBT_HAL_CMD_SoftReset:
        bResult = _HAL_INTERN_ISDBT_SoftReset();
        break;
    case DMD_ISDBT_HAL_CMD_SetACICoef:
        bResult = _HAL_INTERN_ISDBT_SetACICoef();
        break;
    case DMD_ISDBT_HAL_CMD_SetISDBTMode:
        bResult = _HAL_INTERN_ISDBT_SetIsdbtMode();
        break;
    case DMD_ISDBT_HAL_CMD_SetModeClean:
        bResult = _HAL_INTERN_ISDBT_SetModeClean();
        break;
    case DMD_ISDBT_HAL_CMD_Active:
        break;        
    case DMD_ISDBT_HAL_CMD_Check_FEC_Lock:
        bResult = _HAL_INTERN_ISDBT_Check_FEC_Lock();
        break;
    case DMD_ISDBT_HAL_CMD_Check_FSA_TRACK_Lock:
        bResult = _HAL_INTERN_ISDBT_Check_FSA_TRACK_Lock();
        break;
    case DMD_ISDBT_HAL_CMD_Check_PSYNC_Lock:
        bResult = _HAL_INTERN_ISDBT_Check_PSYNC_Lock();
        break;
    case DMD_ISDBT_HAL_CMD_Check_ICFO_CH_EXIST_Lock:
        bResult = _HAL_INTERN_ISDBT_Check_ICFO_CH_EXIST_Lock();
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalCodeRate:
        bResult = _HAL_INTERN_ISDBT_GetSignalCodeRate((*((DMD_ISDBT_GET_CodeRate*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_CodeRate*)pArgs)).eCodeRate));
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalGuardInterval:
        bResult = _HAL_INTERN_ISDBT_GetSignalGuardInterval((EN_ISDBT_GUARD_INTERVAL *)pArgs);
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalTimeInterleaving:
        bResult = _HAL_INTERN_ISDBT_GetSignalTimeInterleaving((*((DMD_ISDBT_GET_TimeInterleaving*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_TimeInterleaving*)pArgs)).eTimeInterleaving));
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalFFTValue:
        bResult = _HAL_INTERN_ISDBT_GetSignalFFTValue((EN_ISDBT_FFT_VAL *)pArgs);
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalModulation:
        bResult = _HAL_INTERN_ISDBT_GetSignalModulation((*((DMD_ISDBT_GET_MODULATION*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_MODULATION*)pArgs)).eConstellation));
        break;
    case DMD_ISDBT_HAL_CMD_ReadIFAGC:
        *((MS_U16 *)pArgs) = _HAL_INTERN_ISDBT_ReadIFAGC();
        break;
    case DMD_ISDBT_HAL_CMD_GetFreqOffset:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ISDBT_GetFreqOffset(&((*((DMD_ISDBT_CFO_DATA*)pArgs)).FFT_Mode), &((*((DMD_ISDBT_CFO_DATA*)pArgs)).TdCfoRegValue), &((*((DMD_ISDBT_CFO_DATA*)pArgs)).FdCfoRegValue), &((*((DMD_ISDBT_CFO_DATA*)pArgs)).IcfoRegValue));
        #else
        bResult = _HAL_INTERN_ISDBT_GetFreqOffset((float *)pArgs);
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalQuality:
    case DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerA:
        #ifndef UTPA2
        *((MS_U16*)pArgs) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerA();
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerB:
        #ifndef UTPA2
        *((MS_U16*)pArgs) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerB();
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalQualityOfLayerC:
        #ifndef UTPA2
        *((MS_U16*)pArgs) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerC();
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetSignalQualityCombine:
        #ifndef UTPA2
        *((MS_U16*)pArgs) = _HAL_INTERN_ISDBT_GetSignalQualityOfLayerCombine();
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetSNR:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ISDBT_GetSNR(&((*((DMD_ISDBT_SNR_DATA*)pArgs)).RegSNR), &((*((DMD_ISDBT_SNR_DATA*)pArgs)).RegSnrObsNum));
        #else
        bResult = _HAL_INTERN_ISDBT_GetSNR((float *)pArgs);  
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetPreViterbiBer:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ISDBT_GetPreViterbiBer((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).BerValue), &((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).BerPeriod));
        #else
        bResult = _HAL_INTERN_ISDBT_GetPreViterbiBer((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).fBerValue)); 
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_GetPostViterbiBer:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ISDBT_GetPostViterbiBer((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).BerValue), &((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).BerPeriod));
        #else
        bResult = _HAL_INTERN_ISDBT_GetPostViterbiBer((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_BER_VALUE*)pArgs)).fBerValue));  
        #endif
        break;
    case DMD_ISDBT_HAL_CMD_Read_PKT_ERR:
        bResult = _HAL_INTERN_ISDBT_Read_PKT_ERR((*((DMD_ISDBT_GET_PKT_ERR*)pArgs)).eIsdbtLayer, &((*((DMD_ISDBT_GET_PKT_ERR*)pArgs)).u16PacketErr));
        break;
    case DMD_ISDBT_HAL_CMD_TS_INTERFACE_CONFIG:
        break; 
    case DMD_ISDBT_HAL_CMD_IIC_Bypass_Mode:
        break;
    case DMD_ISDBT_HAL_CMD_SSPI_TO_GPIO:
        break;
    case DMD_ISDBT_HAL_CMD_GPIO_GET_LEVEL:        
        break;
    case DMD_ISDBT_HAL_CMD_GPIO_SET_LEVEL:
        break;
    case DMD_ISDBT_HAL_CMD_GPIO_OUT_ENABLE:
        break;
    case DMD_ISDBT_HAL_CMD_GET_REG:
        bResult = _HAL_INTERN_ISDBT_GetReg((*((DMD_ISDBT_REG_DATA *)pArgs)).u16Addr, &((*((DMD_ISDBT_REG_DATA *)pArgs)).u8Data));
        break;
    case DMD_ISDBT_HAL_CMD_SET_REG:
        bResult = _HAL_INTERN_ISDBT_SetReg((*((DMD_ISDBT_REG_DATA *)pArgs)).u16Addr, (*((DMD_ISDBT_REG_DATA *)pArgs)).u8Data);
        break;
    default:
        break;
    }
    
    return bResult;
}

MS_BOOL MDrv_DMD_ISDBT_Initial_Hal_Interface(void)
{
    return TRUE;
}

