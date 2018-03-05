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

#include "drvDMD_ATSC.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define DMD_ATSC_CHIP_T3_T10        0x01
#define DMD_ATSC_CHIP_T7            0x02
#define DMD_ATSC_CHIP_T8_T9         0x03
#define DMD_ATSC_CHIP_A1            0x04
#define DMD_ATSC_CHIP_A3            0x05
#define DMD_ATSC_CHIP_A5            0x06
#define DMD_ATSC_CHIP_A7            0x07
#define DMD_ATSC_CHIP_A7P           0x08
#define DMD_ATSC_CHIP_AGATE         0x09
#define DMD_ATSC_CHIP_EDISON        0x0A
#define DMD_ATSC_CHIP_EINSTEIN      0x0B
#define DMD_ATSC_CHIP_EMERALD       0x0C
#define DMD_ATSC_CHIP_EIFFEL        0x0D
#define DMD_ATSC_CHIP_EDEN          0x0E
#define DMD_ATSC_CHIP_EINSTEIN3     0x0F
#define DMD_ATSC_CHIP_MONACO        0x10
#define DMD_ATSC_CHIP_MIAMI         0x11
#define DMD_ATSC_CHIP_MUJI          0x12
#define DMD_ATSC_CHIP_MUNICH        0x13
#define DMD_ATSC_CHIP_MAYA          0x14
#define DMD_ATSC_CHIP_MANHATTAN     0x15
#define DMD_ATSC_CHIP_WHISKY        0x16
#define DMD_ATSC_CHIP_MASERATI      0x17
#define DMD_ATSC_CHIP_MACAN         0x18
#define DMD_ATSC_CHIP_MUSTANG       0x19
#define DMD_ATSC_CHIP_MAXIM         0x1A
#define DMD_ATSC_CHIP_K3            0x80 //UTOF start from 0x80
#define DMD_ATSC_CHIP_KELTIC        0x81
#define DMD_ATSC_CHIP_KERES         0x82
#define DMD_ATSC_CHIP_KIRIN         0x83

#if defined(a1) 
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_A1
#elif defined(a3)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_A3
#elif defined(a5)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_A5
#elif defined(a7)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_A7
#elif defined(amethyst)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_A7P
#elif defined(agate)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_AGATE
#elif defined(edison)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EDISON
#elif defined(einstein)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EINSTEIN
#elif defined(einstein3)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EINSTEIN3
#elif defined(monaco)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_MONACO
#elif defined(emerald)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EMERALD
#elif defined(eiffel)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EIFFEL
#elif defined(kaiser)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_K3
#elif defined(keltic)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_KELTIC
#elif defined(eden)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EDEN
#elif defined(miami)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_MIAMI
#elif defined(keres)
#define DMD_ATSC_CHIP_VERSION       DMD_ATSC_CHIP_KERES
#elif defined(muji)
#define DMD_ATSC_CHIP_VERSION       DMD_ATSC_CHIP_MUJI
#elif defined(munich)
#define DMD_ATSC_CHIP_VERSION       DMD_ATSC_CHIP_MUNICH
#elif defined(mustang)
#define DMD_ATSC_CHIP_VERSION       DMD_ATSC_CHIP_MUSTANG
#elif defined(maxim)
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_MAXIM
#else
 #define DMD_ATSC_CHIP_VERSION      DMD_ATSC_CHIP_EMERALD
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define _RIU_READ_BYTE(addr)        ( READ_BYTE(psDMD_ATSC_ResData->sDMD_ATSC_PriData.virtDMDBaseAddr + (addr) ) )
#define _RIU_WRITE_BYTE(addr, val)  ( WRITE_BYTE(psDMD_ATSC_ResData->sDMD_ATSC_PriData.virtDMDBaseAddr + (addr), val) )

#define HAL_INTERN_ATSC_DBINFO(y)   //y

#if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
 #ifndef MBRegBase
  #define MBRegBase                 0x112600UL
 #endif
 #ifndef MBRegBase_DMD1
  #define MBRegBase_DMD1            0x112400UL
 #endif
#else
 #ifndef MBRegBase
  #define MBRegBase                 0x110500UL
 #endif
#endif

#if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T3_T10)
 #ifndef DMDMcuBase
  #define DMDMcuBase                0x103460UL
 #endif
#else
 #ifndef DMDMcuBase
  #define DMDMcuBase                0x103480UL
 #endif
#endif

#if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
 #define INTERN_ATSC_OUTER_STATE          0xF0
#else
 #define INTERN_ATSC_OUTER_STATE          0x80
#endif
#define INTERN_ATSC_VSB_TRAIN_SNR_LIMIT   0x05//0xBE//14.5dB
#define INTERN_ATSC_FEC_ENABLE            0x1F

#define VSB_ATSC           0x04
#define QAM256_ATSC        0x02

#define QAM16_J83ABC       0x00
#define QAM32_J83ABC       0x01
#define QAM64_J83ABC       0x02
#define QAM128_J83ABC      0x03
#define QAM256_J83ABC      0x04

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

const MS_U8 INTERN_ATSC_table[] = {
    #include "DMD_INTERN_ATSC.dat"
};

static MS_U16 u16Lib_size = sizeof(INTERN_ATSC_table);

#if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)

#if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T7)
static MS_U8 Demod_Flow_register[17] = {0x52, 0x72, 0x52, 0x72, 0x5C, 0x5C, 0xA3, 0xEC, 0xEA,
                                        0x05, 0x74, 0x1E, 0x38, 0x3A, 0x08, 0x70, 0x68};
#else
static MS_U8 Demod_Flow_register[21] = {0x52, 0x72, 0x52, 0x72, 0x5C, 0x5C, 0xA3, 0xEC, 0xEA,
                                        0x05, 0x74, 0x1E, 0x38, 0x3A, 0x00, 0x00, 0x00, 0x00,
                                        0x00, 0x00, 0x00};
#endif

#endif

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

extern MS_U8 u8DMD_ATSC_DMD_ID;

extern DMD_ATSC_ResData *psDMD_ATSC_ResData;

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

    if (u8DMD_ATSC_DMD_ID == 0)
        u32MBRegBase = MBRegBase;
    else if (u8DMD_ATSC_DMD_ID == 1)
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

    if (u8DMD_ATSC_DMD_ID == 0)
        u32MBRegBase = MBRegBase;
    else if (u8DMD_ATSC_DMD_ID == 1)
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

#if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_K3)
static MS_BOOL _SEL_DMD(void)
{
    MS_U8 u8data = 0;
    
    u8data = _HAL_DMD_RIU_ReadByte(0x101e3c);
    
    if (u8DMD_ATSC_DMD_ID == 0) //select DMD0
        u8data &= (~0x10);
    else if (u8DMD_ATSC_DMD_ID == 1) //sel DMD1
        u8data |= 0x10;
    
    _HAL_DMD_RIU_WriteByte(0x101e3c, u8data);
    
    return TRUE;
}
#endif

#if ((DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1) && (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3))
static void _initTable(void)
{
    DMD_ATSC_ResData *pRes = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    if (pRes->sDMD_ATSC_InitData.bTunerGainInvert)
        Demod_Flow_register[12]=1;
    else Demod_Flow_register[12]=0;
    
    if (pRes->sDMD_ATSC_InitData.bIQSwap)
        Demod_Flow_register[14] = 1;
    else Demod_Flow_register[14] = 0;
        
    Demod_Flow_register[15] =  pRes->sDMD_ATSC_InitData.u16IF_KHZ&0xFF;
    Demod_Flow_register[16] = (pRes->sDMD_ATSC_InitData.u16IF_KHZ)>>8;
    
    printf("\n#### IF_KHz  = [%d]\n", pRes->sDMD_ATSC_InitData.u16IF_KHZ);
    printf("\n#### IQ_SWAP = [%d]\n", pRes->sDMD_ATSC_InitData.bIQSwap);
    printf("\n#### Tuner Gain Invert = [%d]\n", pRes->sDMD_ATSC_InitData.bTunerGainInvert);  
}
#endif

#if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T3_T10)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    printf("--------------DMD_ATSC_CHIP_T3_T10--------------\n");
    
    // MailBox
    _HAL_DMD_RIU_WriteByte(0x100b44, 0x00); //clk mail box0 =xtal  <<hk51 <--mail box 0--> aeon
    _HAL_DMD_RIU_WriteByte(0x100b45, 0x00); //clk mail box0 =xtal  <<hk51 <--mail box 1--> aeon
    
    // Enable DMD MCU clock (108MHz)
    if (_HAL_DMD_RIU_ReadByte(0x001ecf) == 0x00)
        _HAL_DMD_RIU_WriteByte(0x100b42, 0x10);
    else  //after t3_u02
        _HAL_DMD_RIU_WriteByte(0x100b42, 0x0D);

    _HAL_DMD_RIU_WriteByte(0x100b43, 0x01); // Disable VD200 clock

    _HAL_DMD_RIU_WriteByte(0x103314, 0x01); // Disable ADC clock
    
    _HAL_DMD_RIU_WriteByte(0x103315, 0x01); // Disable DVB INNER clock

    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103303, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103304, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103305, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103306, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103307, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10330a, 0x08);

    // Enable DVB INNERx1&2 clock
    _HAL_DMD_RIU_WriteByte(0x10330c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10330d, 0x00);
    
    // Enable DVB SRAM0~SRAM3 clock
    _HAL_DMD_RIU_WriteByte(0x103318, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103319, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00); // Enable ATSC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x01); // Disable DVB TS clock
    
    _HAL_DMD_RIU_WriteByte(0x103300, 0x18); // Set DMD clock div
    _HAL_DMD_RIU_WriteByte(0x103301, 0x04); // Enable DMD clock

    MsOS_DelayTaskUs(1);
    
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00); // Enable ADC clock
    
    _HAL_DMD_RIU_WriteByte(0x101e22, 0x02); // Set TS PAD
    _HAL_DMD_RIU_WriteByte(0x101e23, 0x00);

    _HAL_DMD_RIU_WriteByte(0x100b50, 0x08); // Enable TS0&1 clock
    _HAL_DMD_RIU_WriteByte(0x100b51, 0x08);

    if (bRFAGCTristateEnable)
        _HAL_DMD_RIU_WriteByteMask(0x101e05, 0x20, 0x30); // Set IF&RF AGC output mode
    else
        _HAL_DMD_RIU_WriteByteMask(0x101e05, 0x00, 0x30); // Set IF&RF AGC output mode
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T7)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    printf("--------------DMD_ATSC_CHIP_T7--------------\n");
    
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); // Enable DMD MCU clock (108MHz)

    _HAL_DMD_RIU_WriteByte(0x103314, 0x01); // Disable ADC clock

    _HAL_DMD_RIU_WriteByte(0x103315, 0x01); // Disable DVB INNER clock

    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103303, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103304, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103305, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103306, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103307, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10330a, 0x08);
    
    // Enable DVB INNERx1&2&4 clock
    _HAL_DMD_RIU_WriteByte(0x10330c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10330d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10330e, 0x00);
    
    // Enable DVB OUTERx1&2&2_c clock
    _HAL_DMD_RIU_WriteByte(0x103310, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103311, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103312, 0x00);

    // Enable DVB EQx1&8c clock
    _HAL_DMD_RIU_WriteByte(0x103316, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103317, 0x00);

    // Enable DVB SRAM0~SRAM3 clock
    _HAL_DMD_RIU_WriteByte(0x103318, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103319, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00); // Enable ATSC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x01); // Disable DVB TS clock

    _HAL_DMD_RIU_WriteByte(0x103300, 0x18); // Set DMD clock div
    _HAL_DMD_RIU_WriteByte(0x103301, 0x04); // Enable DMD clock

    MsOS_DelayTaskUs(1);
    
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00); // Enable ADC clock
    
    _HAL_DMD_RIU_WriteByte(0x101e22, 0x02); // Set TS PAD
    _HAL_DMD_RIU_WriteByte(0x101e23, 0x00);

    _HAL_DMD_RIU_WriteByte(0x100b50, 0x08); // Enable TS0&1 clock
    _HAL_DMD_RIU_WriteByte(0x100b51, 0x08);
    
    _HAL_DMD_RIU_WriteByteMask(0x101e9e, 0x00, 0xCF); // Set IF&RF AGC PAD and PWM AGC mode 

    if (bRFAGCTristateEnable)
        _HAL_DMD_RIU_WriteByteMask(0x101e05, 0x20, 0x30); // Set IF&RF AGC output mode
    else
        _HAL_DMD_RIU_WriteByteMask(0x101e05, 0x00, 0x30); // Set IF&RF AGC output mode
    
    _HAL_DMD_RIU_WriteByteMask(0x101ea1, 0x00, 0x80); // Set all pads (except SPI) as output
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x01);
    _HAL_DMD_RIU_WriteByteMask(0x112003, 0x20, 0x20); // Release Ana misc resest
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x01, 0x01);
    
    // Set DMD ANA
    _HAL_DMD_RIU_WriteByte(0x112864, 0x00); // Set VCO first and second div
    _HAL_DMD_RIU_WriteByte(0x112865, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x11286C, 0x20); // Disable T&RF-AGC
    _HAL_DMD_RIU_WriteByte(0x11286D, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x112868, 0x00);
    _HAL_DMD_RIU_WriteByte(0x112869, 0x80);
    
    _HAL_DMD_RIU_WriteByte(0x112862, 0x00); // Set PLL first and second div
    _HAL_DMD_RIU_WriteByte(0x112863, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x112818, 0x03); // ADC I&Q pown down
    _HAL_DMD_RIU_WriteByte(0x112819, 0x00);
    
    MsOS_DelayTaskUs(2);
    
    _HAL_DMD_RIU_WriteByte(0x11286A, 0x86); // Initial MPLL procedure
    _HAL_DMD_RIU_WriteByte(0x11286B, 0x1E);
    MsOS_DelayTaskUs(2);
    _HAL_DMD_RIU_WriteByte(0x11286A, 0x06);
    _HAL_DMD_RIU_WriteByte(0x11286B, 0x1E);
    MsOS_DelayTaskUs(2);
    _HAL_DMD_RIU_WriteByte(0x11286A, 0x06);
    _HAL_DMD_RIU_WriteByte(0x11286B, 0x06);
    
    MsOS_DelayTaskUs(2);
    
    _HAL_DMD_RIU_WriteByte(0x112866, 0x01); // Set MPLL first and second div
    _HAL_DMD_RIU_WriteByte(0x112867, 0x1d);
    
    _HAL_DMD_RIU_WriteByte(0x112860, 0x00); // MPLL power up
    _HAL_DMD_RIU_WriteByte(0x112861, 0x1c); // Set ADC output div
    
    _HAL_DMD_RIU_WriteByte(0x112802, 0x40); // Set ADC I&Q
    _HAL_DMD_RIU_WriteByte(0x112803, 0x04);
    
    _HAL_DMD_RIU_WriteByte(0x112816, 0x05); // set PGA gain
    _HAL_DMD_RIU_WriteByte(0x112817, 0x05);
        
    _HAL_DMD_RIU_WriteByte(0x112818, 0x00); // ADC I&Q pown up
    _HAL_DMD_RIU_WriteByte(0x112819, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x112840, 0x00); // Disable SIF&VIF
    _HAL_DMD_RIU_WriteByte(0x112841, 0x00);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T8_T9)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    printf("--------------DMD_ATSC_CHIP_T8_T9--------------\n");
    
    _HAL_DMD_RIU_WriteByte(0x10331E, 0x10); // Enable DMD MCU clock (108MHz)
     
    _HAL_DMD_RIU_WriteByte(0x103314, 0x01); // Disable ADC clock
    
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    
    // Disable DVB INNERx1&2&4 clock
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x01);
    
    // Disable DVB OUTERx1&2&2_c clock
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x01);
    
    // Disable DVB INNER clock
    _HAL_DMD_RIU_WriteByte(0x111f15, 0x01);
    
    // Disable DVB EQx1&8c clock
    _HAL_DMD_RIU_WriteByte(0x111f16, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f17, 0x01);
    
    // Enable DVB SRAM0~SRAM3 clock
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);

    _HAL_DMD_RIU_WriteByte(0x103300, 0x11); // Set DMD clock div
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05); // Enable DMD clock

    _HAL_DMD_RIU_WriteByte(0x103308, 0x00); // Enable ATSC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x01); // Disable DVB TS clock

    // Disable VIF clock
    _HAL_DMD_RIU_WriteByte(0x111f1c, 0x01);
    _HAL_DMD_RIU_WriteByte(0x111f1d, 0x01);
    _HAL_DMD_RIU_WriteByte(0x10331a, 0x01);
    _HAL_DMD_RIU_WriteByte(0x10331b, 0x01);

    MsOS_DelayTaskUs(1);

    _HAL_DMD_RIU_WriteByte(0x103314, 0x00); // Enable ADC clock
    
    _HAL_DMD_RIU_WriteByteMask(0x101eaf, 0x10, 0x18); // Set TS PAD

    if (bRFAGCTristateEnable)
        _HAL_DMD_RIU_WriteByteMask(0x101e05, 0x20, 0x30); // Set IF&RF AGC output mode
    else
        _HAL_DMD_RIU_WriteByteMask(0x101e05, 0x00, 0x30); // Set IF&RF AGC output mode
    
    _HAL_DMD_RIU_WriteByteMask(0x101e9e, 0x00, 0xCF); // Set IF&RF AGC PAD and PWM AGC mode

    _HAL_DMD_RIU_WriteByteMask(0x101ea0, 0x00, 0x03); // PWM2 uses PAD_PWM2 and PWM3 uses PAD_PWM3
    _HAL_DMD_RIU_WriteByteMask(0x101ea1, 0x00, 0x80); // Set all pads (except SPI) as output
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_A1)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{    
    printf("--------------DMD_ATSC_CHIP_A1--------------\n");
    
    //Set register at CLKGEN1
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); // Denny: change 0x10!! 108M
    
    _HAL_DMD_RIU_WriteByte(0x103314, 0x01); // Disable ADC clock
    
    // set parallet ts clock
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);

    // enable atsc, DVBTC ts clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    
    // enable dvbc adc clock
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    
    // enable vif DAC clock
    _HAL_DMD_RIU_WriteByte(0x10331b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331a, 0x00);
    
    // Set register at CLKGEN_DMD
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);

    // enable clk_atsc_adcd_sync
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);

    // enable dvbt inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);

    // enable dvbt inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);

    // enable dvbt inner clock
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);

    // enable dvbc outer clock
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);

    // enable dvbc inner-c clock
    _HAL_DMD_RIU_WriteByte(0x111f15, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f14, 0x00);

    // enable dvbc eq clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);

    // enable vif clock
    _HAL_DMD_RIU_WriteByte(0x111f1d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f1c, 0x00);

    // For ADC DMA Dump
    _HAL_DMD_RIU_WriteByte(0x111f21, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f20, 0x00);

    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);

    MsOS_DelayTaskUs(1);

    _HAL_DMD_RIU_WriteByte(0x103314, 0x00); // Enable ADC clock
    
    //  Turn TSP
    _HAL_DMD_RIU_WriteByte(0x100b55, 0x00);
    _HAL_DMD_RIU_WriteByte(0x100b54, 0x00);
        
    // set the ts0_clk from demod
    _HAL_DMD_RIU_WriteByte(0x100b51, 0x00);
    _HAL_DMD_RIU_WriteByte(0x100b50, 0x0C);
    _HAL_DMD_RIU_WriteByte(0x101e22, 0x02);
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_A7)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    printf("--------------DMD_ATSC_CHIP_A7--------------\n");
    
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    
    _HAL_DMD_RIU_WriteByte(0x103314, 0x01); // Disable ADC clock
    
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f24, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    
    MsOS_DelayTaskUs(1);

    _HAL_DMD_RIU_WriteByte(0x103314, 0x00); // Enable ADC clock
    
    _HAL_DMD_RIU_WriteByteMask(0x000e13, 0x00, 0x04);
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_K3)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    DMD_ATSC_ResData *pRes  = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    printf("--------------DMD_ATSC_CHIP_K3--------------\n");
    
    if (pRes->sDMD_ATSC_InitData.u8IS_DUAL)
    {
        _HAL_DMD_RIU_WriteByte(0x101e39, 0x00);
        _HAL_DMD_RIU_WriteByte(0x101e3d, 0x00);
        
        /****************DMD0****************/
    
        //set CLK_DMDMCU as 108M Hz
        _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
    
        // set parallet ts clock
        _HAL_DMD_RIU_WriteByte(0x103301, 0x07);
        _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    
        // enable DVBTC ts clock
        _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    
        // enable dvbc adc clock
        _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
        _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    
        // enable clk_atsc_adcd_sync
        _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
        _HAL_DMD_RIU_WriteByte(0x111f0a, 0x04);
    
        // enable dvbt inner clock
        _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    
        // enable dvbt outer clock
        _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
        
        // enable dvbc outer clock
        _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
        _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);
    
        // enable dvbc inner-c clock
        _HAL_DMD_RIU_WriteByte(0x111f15, 0x04);
    
        // enable dvbc eq clock
        _HAL_DMD_RIU_WriteByte(0x111f17, 0x00);
        _HAL_DMD_RIU_WriteByte(0x111f16, 0x00);
    
        // For ADC DMA Dump
        _HAL_DMD_RIU_WriteByte(0x111f22, 0x04);
        
        //  Turn TSP
        //_HAL_DMD_RIU_WriteByte(0x000e13, 0x01);
    
        //set reg_allpad_in
        _HAL_DMD_RIU_WriteByte(0x101ea1, 0x00);
        _HAL_DMD_RIU_WriteByte(0x101e04, 0x02);
        _HAL_DMD_RIU_WriteByte(0x101e76, 0x03);   
    
        /****************DMD1****************/
        _HAL_DMD_RIU_WriteByte(0x10331f, 0x10);
        
        _HAL_DMD_RIU_WriteByte(0x103321, 0x07);
        _HAL_DMD_RIU_WriteByte(0x103320, 0x11);
        
        _HAL_DMD_RIU_WriteByte(0x103323, 0x00);
        _HAL_DMD_RIU_WriteByte(0x103322, 0x00);
        
        _HAL_DMD_RIU_WriteByte(0x11220b, 0x00);
        _HAL_DMD_RIU_WriteByte(0x11220a, 0x04);
        
        _HAL_DMD_RIU_WriteByte(0x11220c, 0x00);
        _HAL_DMD_RIU_WriteByte(0x112211, 0x00);
        
        _HAL_DMD_RIU_WriteByte(0x112213, 0x00);
        _HAL_DMD_RIU_WriteByte(0x112212, 0x00);
        
        _HAL_DMD_RIU_WriteByte(0x112215, 0x04);
        
        _HAL_DMD_RIU_WriteByte(0x112217, 0x00);
        _HAL_DMD_RIU_WriteByte(0x112216, 0x00);
        
        _HAL_DMD_RIU_WriteByte(0x112222, 0x04);
        
        _HAL_DMD_RIU_WriteByte(0x101e39, 0x03); //force ANA MISC controlled by DMD0
        _HAL_DMD_RIU_WriteByte(0x101e3d, 0x01);
    }
    else
    {
        _HAL_DMD_RIU_WriteByte(0x101e39, 0x00);

        _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);  
        _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 

        _HAL_DMD_RIU_WriteByte(0x103301, 0x07); 
        _HAL_DMD_RIU_WriteByte(0x103300, 0x11); 

        _HAL_DMD_RIU_WriteByte(0x103309, 0x00);

        _HAL_DMD_RIU_WriteByte(0x103315, 0x00); 
        _HAL_DMD_RIU_WriteByte(0x103314, 0x00);

        _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
        _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

        _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);

        _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);

        _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
        _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);

        _HAL_DMD_RIU_WriteByte(0x111f15, 0x00);
         
        _HAL_DMD_RIU_WriteByte(0x111f17, 0x00);
        _HAL_DMD_RIU_WriteByte(0x111f16, 0x00);

        _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);

        _HAL_DMD_RIU_WriteByte(0x101ea1, 0x00);
        _HAL_DMD_RIU_WriteByte(0x101e04, 0x02);
        _HAL_DMD_RIU_WriteByte(0x101e76, 0x03);

        _HAL_DMD_RIU_WriteByte(0x101e39, 0x03); //force ANA MISC controlled by DMD0
        _HAL_DMD_RIU_WriteByte(0x101e3d, 0x01);
    }
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_KELTIC)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{ 
    printf("--------------DMD_ATSC_CHIP_KELTIC--------------\n");
    
    _HAL_DMD_RIU_WriteByte(0x101e39, 0x00);

    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 

    _HAL_DMD_RIU_WriteByte(0x103301, 0x07); 
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11); 

    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);

    _HAL_DMD_RIU_WriteByte(0x103315, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f15, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f17, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f16, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x1120bc, 0x00);

    _HAL_DMD_RIU_WriteByte(0x101ea1, 0x00);
    _HAL_DMD_RIU_WriteByte(0x101e04, 0x02);
    
    _HAL_DMD_RIU_WriteByte(0x101e39, 0x03); 
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_KERES)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val=0x00;
    
    printf("--------------DMD_ATSC_CHIP_KERES--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    _HAL_DMD_RIU_WriteByte(0x10331f,0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
    _HAL_DMD_RIU_WriteByte(0x103301,0x07);
    _HAL_DMD_RIU_WriteByte(0x103300,0x11);
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f11,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f15,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f17,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f16,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f2b,0x00);  //enable clk_rs
    _HAL_DMD_RIU_WriteByte(0x111f2a,0x10);
    //_HAL_DMD_RIU_WriteByte(0x000e13,0x01); // No need, it cause uart issue.
    _HAL_DMD_RIU_WriteByte(0x101ea1,0x00);

    _HAL_DMD_RIU_WriteByte(0x101e04,0x02);
    _HAL_DMD_RIU_WriteByte(0x101e76,0x03);

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_EDEN)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0x00;
    
    printf("--------------DMD_ATSC_CHIP_EDEN--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
    _HAL_DMD_RIU_WriteByte(0x103301,0x04);
    _HAL_DMD_RIU_WriteByte(0x103300,0x0B);
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103308,0x00);
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x04);

    _HAL_DMD_RIU_WriteByte(0x111f03,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x08);

    _HAL_DMD_RIU_WriteByte(0x111f0d,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e,0x00);

    _HAL_DMD_RIU_WriteByte(0x111f11,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f19,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f23,0x40);
    _HAL_DMD_RIU_WriteByte(0x111f22,0x00);

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, (u8Val|0x03));
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_EMERALD)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0x00;
    
    printf("--------------DMD_ATSC_CHIP_EMERALD--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    _HAL_DMD_RIU_WriteByte(0x10331f,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
    _HAL_DMD_RIU_WriteByte(0x103301,0x05);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x103300,0x11);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103308,0x00);
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x00);//Different with EDEN!

    _HAL_DMD_RIU_WriteByte(0x111f03,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x08);

    _HAL_DMD_RIU_WriteByte(0x111f0d,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e,0x00);

    _HAL_DMD_RIU_WriteByte(0x111f11,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f19,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f23,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x111f22,0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f25,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x111f24,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x111f1E,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x111f09,0x00);//Different with EDEN!

    u8Val = _HAL_DMD_RIU_ReadByte(0x000e13);
    _HAL_DMD_RIU_WriteByte(0x000e13, u8Val&0xFB);

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03); 
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_EINSTEIN)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{ 
    MS_U8 u8Val = 0;
    
    printf("--------------DMD_ATSC_CHIP_EINSTEIN--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39); 
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); //Denny: change 0x10!! 108M
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05); //addy update 0809 MAdp_Demod_WriteReg(0x103301, 0x06);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11); //addy update 0809 MAdp_Demod_WriteReg(0x103300, 0x0B);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08); // note enable clk_atsc_adcd_sync=25.41
    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08); //0406 update 0->8
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);  //dan add for nugget
    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);//0x08); VT found some err.

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);    
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_EINSTEIN3)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{ 
    MS_U8 u8Val = 0;
    
    printf("--------------DMD_ATSC_CHIP_EINSTEIN3--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);  
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10);// Denny: change 0x10!! 108M
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);//addy update 0809 MAdp_Demod_WriteReg(0x103301, 0x06);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);//addy update 0809 MAdp_Demod_WriteReg(0x103300, 0x0B);
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  // note enable clk_atsc_adcd_sync=25.41
    
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);//0406 update 0->8
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);

    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f45, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f44, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f46, 0x01);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);  //dan add for nugget
    _HAL_DMD_RIU_WriteByte(0x111f4c, 0x00);  //dan add for nugget
    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);//0x08); VT found some err.

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);    
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MONACO)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;

    printf("--------------DMD_ATSC_CHIP_MONACO--------------\n");

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    // DMDMCU 108M
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    // [11] : reg_ckg_demod_test_in_en = 0
    //        0: select internal ADC CLK
    //        1: select external test-in clock 
    // [10] : reg_ckg_dvbtm_ts_out_mode = 1
    //        0: select gated clock 
    //        1: select free-run clock
    // [9]  : reg_ckg_atsc_dvbtc_ts_inv = 0
    //        0: normal phase to pad
    //        1: invert phase to pad
    // [8]  : reg_ckg_atsc_dvb_div_sel = 1
    //        0: select clk_dmplldiv5
    //        1: select clk_dmplldiv3
    // [4:0]: reg_ckg_dvbtm_ts_divnum = 17
    //        => TS clock = (864/3)/(2*(17+1)) = 8MHz
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Enable VIF DAC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x111f29, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); 
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // Enable clk_atsc_adcd_sync = 25.41
    // [3:0]: reg_ckg_atsc_adcd_sync
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: clk_dmdadc_sync
    //        01: clk_atsc50_p
    //            ^^^^^^^^^^^^
    //        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7(49.7 MHz)
    //                   else               => clk_dmplldiv17(50.82 MHz)
    //        10: clk_atsc25_p
    //            ^^^^^^^^^^^^
    //        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div2(24.85 MHz)
    //                   else			            => clk_dmplldiv17_div2(25.41 MHz)
    //        11: 1'b0                
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0008);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0008);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBT inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    // Enable DVBT outer clock
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // Enable ISDBT SRAM share clock and symbol rate clock
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00); 
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // [3:0]  : reg_ckg_dtmb_eq2x_inner2x_12x
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk288_buf(256 MHz)
    //        01: dtmb_eq_sram_clk36_buf(32 MHz)
    //        10: dtmb_eq_sram_clk216_buf(192 MHz)
    //        11: 1'b0
    // [7:4]  : reg_ckg_dtmb_inner1x_dvbc_eq1x => CCI LMS 1x
    //                                            ^^^^^^^^^^
    // [0] : disable clock
    // [1] : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk18_buf(16 MHz)	    => DTMB
    //        01: clk_dmplldiv3_div16(18 MHz)  => DVBC,ISDBT(>= (24/2=12))
    //        10: clk_dmplldiv10_div8(10.8 MHz)=> DVBT
    //        11: clk_cci_lms_1x_atsc_p_buf    => ATSC
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^
    //        if(reg_atsc_eq_sel_mplldiv2) => clk_dmplldiv2_div2_inv_div8(21.75 MHz)
    //             else                         => clk_dmplldiv5_inv_div8(21.6 MHz)
    // [11:8] : reg_ckg_dtmb_inner4x_dvbc_eq4x => CCI LMS 4x
    //                                            ^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk72_buf(64 MHz)	    => DTMB
    //        01: clk_dmplldiv3_div4(72 MHz)   => DVBC,ISDBT(>= 48)
    //        10: clk_dmplldiv10_div2(43.2 MHz)=> DVBT
    //        11: clk_cci_lms_4x_atsc_p_buf    => ATSC
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^
    //            if(reg_atsc_eq_sel_mplldiv2) => clk_dmplldiv2_div2_inv_div2(87 MHz)
    //            else                         => clk_dmplldiv5_inv_div2(86.4 MHz)
    // [15:12]: reg_ckg_dtmb_sram_dump
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk18_buf(16 MHz)	    
    //        01: dtmb_sram_dump_clk144_buf(128 MHz)   
    //        10: dtmb_sram_dump_clk216_buf(192 MHz)
    //        11: dtmb_sram_dump_dmplldiv5_buf(153.6 MHz)
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h38, 2'b11, 16'h1cc1);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h38, 2'b11, 16'h1cc1);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x1C);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0xC1);
    // [4:0]  : reg_ckg_dtmb_inner4x_sr1x => symbol rate FFT 1x
    //                                       ^^^^^^^^^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]: Select clock source 
    //        000: adc_clk_buf
    //        001: clk_atsc25_p
    //             ^^^^^^^^^^^^
    //             if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div2(24.85 MHz)
    //                              else			      => clk_dmplldiv17_div2(25.41 MHz)
    //        010: clk_atsc_eq25_p
    //             ^^^^^^^^^^^^^^^
    //		        case({reg_eq25_sel_mplldiv3,reg_atsc_eq_sel_mplldiv2})
    //		           2'b00: clk_dmplldiv5_inv_div8	(21.6 MHz)
    //		           2'b01: clk_dmplldiv2_div2_inv_div8	(21.75 MHz)
    //		           2'b10: clk_dmplldiv3_div16		(18 MHz)
    //		           2'b11: 1'b0
    //                      endcase
    //        011: dtmb_clk72_buf(72 MHz)
    //        100: dtmb_clk18_buf(18 MHz)
    //        101: 1'b0
    //        110: 1'b0
    //        111: 1'b0
    // [12:8] : reg_ckg_dtmb_inner2x_sr0p5x => symbol rate FFT 0.5x
    //                                         ^^^^^^^^^^^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]: Select clock source 
    //        000: clk_adc_div2_buf
    //        001: clk_frontend_d2_p0
    //             ^^^^^^^^^^^^^^^^^^
    //             if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div4(12.43 MHz)
    //             else                          => clk_dmplldiv17_div4(12.705 MHz)
    //        010: clk_atsc_eq25_div2_p
    //             ^^^^^^^^^^^^^^^^^^^^
    //	       case({reg_eq25_sel_mplldiv3,reg_atsc_eq_sel_mplldiv2})
    //		           2'b00: clk_dmplldiv5_inv_div8_div2	    (10.8 MHz)
    //		           2'b01: clk_dmplldiv2_div2_inv_div8_div2  (10.875 MHz)
    //		           2'b10: clk_dmplldiv3_div32		    (9 MHz)
    //		           2'b11: 1'b0
    //                      endcase
    //        011: dtmb_clk36_buf(36 MHz)
    //        100: dtmb_clk9_buf(9 MHz)
    //        101: 1'b0
    //        110: 1'b0
    //        111: 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0404);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0404);
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x04);
    
    //Enable SRAM power saving
    _HAL_DMD_RIU_WriteByte(0x112091, 0x44);
    _HAL_DMD_RIU_WriteByte(0x112090, 0x00);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);  
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_EDISON)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0x00;
    
    printf("--------------DMD_ATSC_CHIP_EDISON--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    _HAL_DMD_RIU_WriteByte(0x10331f,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
    _HAL_DMD_RIU_WriteByte(0x103301,0x06);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x103300,0x0B);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103308,0x00);
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x00);//Different with EDEN!

    _HAL_DMD_RIU_WriteByte(0x111f03,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0d,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e,0x00);

    _HAL_DMD_RIU_WriteByte(0x111f11,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f19,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f23,0x00);//Different with EDEN!
    _HAL_DMD_RIU_WriteByte(0x111f22,0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f25,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f24,0x00);   

    _HAL_DMD_RIU_WriteByte(0x111F1E,0x00);
    _HAL_DMD_RIU_WriteByte(0x111F09,0x00);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x000e13);
    _HAL_DMD_RIU_WriteByte(0x000e13, u8Val&0xFB);

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03); 
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_EIFFEL)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0x00;
    
    printf("--------------DMD_ATSC_CHIP_EIFFEL--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    _HAL_DMD_RIU_WriteByte(0x101e39 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x10331f ,0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e ,0x10);
    _HAL_DMD_RIU_WriteByte(0x103301 ,0x05);
    _HAL_DMD_RIU_WriteByte(0x103300 ,0x11);
    _HAL_DMD_RIU_WriteByte(0x103309 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x103308 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x103315 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f28 ,0x00);
 // _HAL_DMD_RIU_WriteByte(0x112028 ,0x03);
    _HAL_DMD_RIU_WriteByte(0x111f03 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0b ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a ,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f0d ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f11 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12 ,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f19 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f23 ,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22 ,0x00);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x000e61);
    _HAL_DMD_RIU_WriteByte(0x000e61, u8Val&0xFE);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MIAMI)
stativ void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;
    
    printf("--------------DMD_ATSC_CHIP_MIAMI--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    // DMDMCU 108M
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Select MPLLDIV17
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); 
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // enable clk_atsc_adcd_sync
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBT inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    // Enable DVBT outer clock
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // enable clk_dvbtc_sram4_isdbt_inner4x & clk_adc1x_eq1x clock
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00); 
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // enable CCI LMS clock
    _HAL_DMD_RIU_WriteByte(0x111f51, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f50, 0xCC);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03); 
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUJI)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    printf("--------------DMD_ATSC_CHIP_MUJI--------------\n");

    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x00, 0x03);

    // DMDMCU 108M
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    // [11] : reg_ckg_demod_test_in_en = 0
    //        0: select internal ADC CLK
    //        1: select external test-in clock 
    // [10] : reg_ckg_dvbtm_ts_out_mode = 1
    //        0: select gated clock 
    //        1: select free-run clock
    // [9]  : reg_ckg_atsc_dvbtc_ts_inv = 0
    //        0: normal phase to pad
    //        1: invert phase to pad
    // [8]  : reg_ckg_atsc_dvb_div_sel = 1
    //        0: select clk_dmplldiv5
    //        1: select clk_dmplldiv3
    // [4:0]: reg_ckg_dvbtm_ts_divnum = 17
    //        => TS clock = (864/3)/(2*(17+1)) = 8MHz
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Reset TS divider
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);    
    // Enable VIF DAC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x111f29, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); 
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // Enable clk_atsc_adcd_sync = 25.41
    // [3:0]: reg_ckg_atsc_adcd_sync
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: clk_dmdadc_sync
    //        01: clk_atsc50_p
    //            ^^^^^^^^^^^^
    //        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7(49.7 MHz)
    //                   else               => clk_dmplldiv17(50.82 MHz)
    //        10: clk_atsc25_p
    //            ^^^^^^^^^^^^
    //        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div2(24.85 MHz)
    //                   else			            => clk_dmplldiv17_div2(25.41 MHz)
    //        11: 1'b0                
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0008);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0008);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBT inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    // Enable DVBT outer clock
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // Enable ISDBT SRAM share clock and symbol rate clock
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00); 
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // [3:0]  : reg_ckg_dtmb_eq2x_inner2x_12x
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk288_buf(256 MHz)
    //        01: dtmb_eq_sram_clk36_buf(32 MHz)
    //        10: dtmb_eq_sram_clk216_buf(192 MHz)
    //        11: 1'b0
    // [7:4]  : reg_ckg_dtmb_inner1x_dvbc_eq1x => CCI LMS 1x
    //                                            ^^^^^^^^^^
    // [0] : disable clock
    // [1] : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk18_buf(16 MHz)	    => DTMB
    //        01: clk_dmplldiv3_div16(18 MHz)  => DVBC,ISDBT(>= (24/2=12))
    //        10: clk_dmplldiv10_div8(10.8 MHz)=> DVBT
    //        11: clk_cci_lms_1x_atsc_p_buf    => ATSC
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^
    //        if(reg_atsc_eq_sel_mplldiv2) => clk_dmplldiv2_div2_inv_div8(21.75 MHz)
    //             else                         => clk_dmplldiv5_inv_div8(21.6 MHz)
    // [11:8] : reg_ckg_dtmb_inner4x_dvbc_eq4x => CCI LMS 4x
    //                                            ^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk72_buf(64 MHz)	    => DTMB
    //        01: clk_dmplldiv3_div4(72 MHz)   => DVBC,ISDBT(>= 48)
    //        10: clk_dmplldiv10_div2(43.2 MHz)=> DVBT
    //        11: clk_cci_lms_4x_atsc_p_buf    => ATSC
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^
    //            if(reg_atsc_eq_sel_mplldiv2) => clk_dmplldiv2_div2_inv_div2(87 MHz)
    //            else                         => clk_dmplldiv5_inv_div2(86.4 MHz)
    // [15:12]: reg_ckg_dtmb_sram_dump
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk18_buf(16 MHz)	    
    //        01: dtmb_sram_dump_clk144_buf(128 MHz)   
    //        10: dtmb_sram_dump_clk216_buf(192 MHz)
    //        11: dtmb_sram_dump_dmplldiv5_buf(153.6 MHz)
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h38, 2'b11, 16'h1cc1);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h38, 2'b11, 16'h1cc1);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x1C);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0xC1);
    // [4:0]  : reg_ckg_dtmb_inner4x_sr1x => symbol rate FFT 1x
    //                                       ^^^^^^^^^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]: Select clock source 
    //        000: adc_clk_buf
    //        001: clk_atsc25_p
    //             ^^^^^^^^^^^^
    //             if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div2(24.85 MHz)
    //                              else			      => clk_dmplldiv17_div2(25.41 MHz)
    //        010: clk_atsc_eq25_p
    //             ^^^^^^^^^^^^^^^
    //		        case({reg_eq25_sel_mplldiv3,reg_atsc_eq_sel_mplldiv2})
    //		           2'b00: clk_dmplldiv5_inv_div8	(21.6 MHz)
    //		           2'b01: clk_dmplldiv2_div2_inv_div8	(21.75 MHz)
    //		           2'b10: clk_dmplldiv3_div16		(18 MHz)
    //		           2'b11: 1'b0
    //                      endcase
    //        011: dtmb_clk72_buf(72 MHz)
    //        100: dtmb_clk18_buf(18 MHz)
    //        101: 1'b0
    //        110: 1'b0
    //        111: 1'b0
    // [12:8] : reg_ckg_dtmb_inner2x_sr0p5x => symbol rate FFT 0.5x
    //                                         ^^^^^^^^^^^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]: Select clock source 
    //        000: clk_adc_div2_buf
    //        001: clk_frontend_d2_p0
    //             ^^^^^^^^^^^^^^^^^^
    //             if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div4(12.43 MHz)
    //             else                          => clk_dmplldiv17_div4(12.705 MHz)
    //        010: clk_atsc_eq25_div2_p
    //             ^^^^^^^^^^^^^^^^^^^^
    //	       case({reg_eq25_sel_mplldiv3,reg_atsc_eq_sel_mplldiv2})
    //		           2'b00: clk_dmplldiv5_inv_div8_div2	    (10.8 MHz)
    //		           2'b01: clk_dmplldiv2_div2_inv_div8_div2  (10.875 MHz)
    //		           2'b10: clk_dmplldiv3_div32		    (9 MHz)
    //		           2'b11: 1'b0
    //                      endcase
    //        011: dtmb_clk36_buf(36 MHz)
    //        100: dtmb_clk9_buf(9 MHz)
    //        101: 1'b0
    //        110: 1'b0
    //        111: 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0404);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0404);
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x04);
    
    // Muji
    // [1:0]  : reg_ckg_isdbt_outer1x_dvbt_outer1x
    //          [0]  : disable clock
    //          [1]  : invert clock
    //          [3:2]: Select clock source 
    //                 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //                 sel[0]= (reg_demod_isdbt_on & reg_ckg_isdbt_outer1x[2])
    //                 sel[1]= (~reg_demod_isdbt_on)                          
    //                 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //                 00: isdbt_clk6_lat(6 MHz)
    //                 01: isdbt_clk8_lat(8 MHz)
    //                 10: clk_dmplldiv10_div2(43.2 MHz)
    //                 11: 1'b0
    // [6:4]  : reg_ckg_miu_dvbtc_outer2x
    //          [0]  : disable clock
    //          [1]  : invert clock
    //          [2]  : Select clock source 
    //                 0: clk_miu_p
    //                 1: clk_dmplldiv10(86.4 MHz)
    // [12:8] : reg_ckg_dvbtc_rs
    //          [0]  : disable clock
    //          [1]  : invert clock
    //          [4:2]: Select clock source 
    //                 000: clk_dmplldiv10(86.4 MHz)
    //                 001: clk_dmplldiv10_div2(43.2 MHz)
    //                 010: clk_atsc50_p
    //                      ^^^^^^^^^^^^
    //		        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7(49.7 MHz)
    //                      else                          => clk_dmplldiv17(50.82 MHz)
    //                 011: clk_dvbtc_rs_216_buf(216 MHz)
    //                 100: clk_dvbtc_rs_172_buf(172 MHz)
    //                 101: clk_dvbtc_rs_144_buf(144 MHz)
    //                 110: 1'b0
    //                 111: 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h27, 2'b10, 16'h0800);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h27, 2'b10, 16'h0800);
    _HAL_DMD_RIU_WriteByte(0x111f4f, 0x08);

    //Enable SRAM power saving
    _HAL_DMD_RIU_WriteByte(0x112091, 0x44);
    _HAL_DMD_RIU_WriteByte(0x112090, 0x00);
    
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);  
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUNICH)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;
    
    printf("--------------DMD_ATSC_CHIP_MUNICH--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    // DMDMCU 108M
    _HAL_DMD_RIU_WriteByte(0x10331f, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Select MPLLDIV17
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); 
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // enable clk_atsc_adcd_sync
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBT inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    // Enable DVBT outer clock
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // enable clk_dvbtc_sram4_isdbt_inner4x & clk_adc1x_eq1x clock
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x00);  
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00); 
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // enable CCI LMS clock
    _HAL_DMD_RIU_WriteByte(0x111f51, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f50, 0xCC);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03); 
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_KIRIN)
void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val=0x00;
    
    printf("--------------DMD_ATSC_CHIP_KIRIN--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    _HAL_DMD_RIU_WriteByte(0x10331f,0x00);
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
    _HAL_DMD_RIU_WriteByte(0x103301,0x07);
    _HAL_DMD_RIU_WriteByte(0x103300,0x11);
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x00);

    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f11,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f13,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f15,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f17,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f16,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f2b,0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f2a,0x10);
    _HAL_DMD_RIU_WriteByte(0x101ea1,0x00);

    _HAL_DMD_RIU_WriteByte(0x101e04,0x02);
    _HAL_DMD_RIU_WriteByte(0x101e76,0x03);

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39,(u8Val| 0x03));
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAYA)
void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val=0x00;
    
    printf("--------------DMD_ATSC_CHIP_MAYA--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    // DMDMCU 108M 
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Reset TS divider
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
    // ADC select MPLLDIV17 & EQ select MPLLDIV5
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    // configure reg_ckg_atsc50, reg_ckg_atsc25, reg_ckg_atsc_eq25 and reg_ckg_atsc_ce25    
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // enable clk_atsc_adcd_sync
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // enable CCI LMS clock
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x00);
    // set symbol rate
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x04);
    // reg_ckg_adc1x_eq1x
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x04);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39,(u8Val| 0x03));
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MANHATTAN)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;

    printf("--------------DMD_ATSC_CHIP_MANHATTAN--------------\n");

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    // DMDMCU 108M
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    // [11] : reg_ckg_demod_test_in_en = 0
    //        0: select internal ADC CLK
    //        1: select external test-in clock 
    // [10] : reg_ckg_dvbtm_ts_out_mode = 1
    //        0: select gated clock 
    //        1: select free-run clock
    // [9]  : reg_ckg_atsc_dvbtc_ts_inv = 0
    //        0: normal phase to pad
    //        1: invert phase to pad
    // [8]  : reg_ckg_atsc_dvb_div_sel = 1
    //        0: select clk_dmplldiv5
    //        1: select clk_dmplldiv3
    // [4:0]: reg_ckg_dvbtm_ts_divnum = 17
    //        => TS clock = (864/3)/(2*(17+1)) = 8MHz
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    // `RIU_W((`RIUBASE_CLKGEN1>>1)+7'h00, 2'b11, 16'h0511);
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Reset TS divider
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);    
    // Enable VIF DAC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x111f29, 0x00); 
    _HAL_DMD_RIU_WriteByte(0x111f28, 0x00); 
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f05, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // Enable clk_atsc_adcd_sync = 25.41
    // [3:0]: reg_ckg_atsc_adcd_sync
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: clk_dmdadc_sync
    //        01: clk_atsc50_p
    //            ^^^^^^^^^^^^
    //        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7(49.7 MHz)
    //                   else               => clk_dmplldiv17(50.82 MHz)
    //        10: clk_atsc25_p
    //            ^^^^^^^^^^^^
    //        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div2(24.85 MHz)
    //                   else			            => clk_dmplldiv17_div2(25.41 MHz)
    //        11: 1'b0                
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0008);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h05, 2'b11, 16'h0008);
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBT inner clock
    _HAL_DMD_RIU_WriteByte(0x111f0d, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0c, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0f, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0e, 0x00);
    // Enable DVBT outer clock
    _HAL_DMD_RIU_WriteByte(0x111f11, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f10, 0x00);
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f13, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x08);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // Enable ISDBT SRAM share clock and symbol rate clock
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x44);  
    _HAL_DMD_RIU_WriteByte(0x111f48, 0x00); 
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // [3:0]  : reg_ckg_dtmb_eq2x_inner2x_12x
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk288_buf(256 MHz)
    //        01: dtmb_eq_sram_clk36_buf(32 MHz)
    //        10: dtmb_eq_sram_clk216_buf(192 MHz)
    //        11: 1'b0
    // [7:4]  : reg_ckg_dtmb_inner1x_dvbc_eq1x => CCI LMS 1x
    //                                            ^^^^^^^^^^
    // [0] : disable clock
    // [1] : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk18_buf(16 MHz)	    => DTMB
    //        01: clk_dmplldiv3_div16(18 MHz)  => DVBC,ISDBT(>= (24/2=12))
    //        10: clk_dmplldiv10_div8(10.8 MHz)=> DVBT
    //        11: clk_cci_lms_1x_atsc_p_buf    => ATSC
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^
    //        if(reg_atsc_eq_sel_mplldiv2) => clk_dmplldiv2_div2_inv_div8(21.75 MHz)
    //             else                         => clk_dmplldiv5_inv_div8(21.6 MHz)
    // [11:8] : reg_ckg_dtmb_inner4x_dvbc_eq4x => CCI LMS 4x
    //                                            ^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk72_buf(64 MHz)	    => DTMB
    //        01: clk_dmplldiv3_div4(72 MHz)   => DVBC,ISDBT(>= 48)
    //        10: clk_dmplldiv10_div2(43.2 MHz)=> DVBT
    //        11: clk_cci_lms_4x_atsc_p_buf    => ATSC
    //            ^^^^^^^^^^^^^^^^^^^^^^^^^
    //            if(reg_atsc_eq_sel_mplldiv2) => clk_dmplldiv2_div2_inv_div2(87 MHz)
    //            else                         => clk_dmplldiv5_inv_div2(86.4 MHz)
    // [15:12]: reg_ckg_dtmb_sram_dump
    // [0]  : disable clock
    // [1]  : invert clock
    // [3:2]: Select clock source 
    //        00: dtmb_clk18_buf(16 MHz)	    
    //        01: dtmb_sram_dump_clk144_buf(128 MHz)   
    //        10: dtmb_sram_dump_clk216_buf(192 MHz)
    //        11: dtmb_sram_dump_dmplldiv5_buf(153.6 MHz)
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h38, 2'b11, 16'h1cc1);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h38, 2'b11, 16'h1cc1);
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x1C);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0xC1);
    // [4:0]  : reg_ckg_dtmb_inner4x_sr1x => symbol rate FFT 1x
    //                                       ^^^^^^^^^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]: Select clock source 
    //        000: adc_clk_buf
    //        001: clk_atsc25_p
    //             ^^^^^^^^^^^^
    //             if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div2(24.85 MHz)
    //                              else			      => clk_dmplldiv17_div2(25.41 MHz)
    //        010: clk_atsc_eq25_p
    //             ^^^^^^^^^^^^^^^
    //		        case({reg_eq25_sel_mplldiv3,reg_atsc_eq_sel_mplldiv2})
    //		           2'b00: clk_dmplldiv5_inv_div8	(21.6 MHz)
    //		           2'b01: clk_dmplldiv2_div2_inv_div8	(21.75 MHz)
    //		           2'b10: clk_dmplldiv3_div16		(18 MHz)
    //		           2'b11: 1'b0
    //                      endcase
    //        011: dtmb_clk72_buf(72 MHz)
    //        100: dtmb_clk18_buf(18 MHz)
    //        101: 1'b0
    //        110: 1'b0
    //        111: 1'b0
    // [12:8] : reg_ckg_dtmb_inner2x_sr0p5x => symbol rate FFT 0.5x
    //                                         ^^^^^^^^^^^^^^^^^^^^
    // [0]  : disable clock
    // [1]  : invert clock
    // [4:2]: Select clock source 
    //        000: clk_adc_div2_buf
    //        001: clk_frontend_d2_p0
    //             ^^^^^^^^^^^^^^^^^^
    //             if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7_div4(12.43 MHz)
    //             else                          => clk_dmplldiv17_div4(12.705 MHz)
    //        010: clk_atsc_eq25_div2_p
    //             ^^^^^^^^^^^^^^^^^^^^
    //	       case({reg_eq25_sel_mplldiv3,reg_atsc_eq_sel_mplldiv2})
    //		           2'b00: clk_dmplldiv5_inv_div8_div2	    (10.8 MHz)
    //		           2'b01: clk_dmplldiv2_div2_inv_div8_div2  (10.875 MHz)
    //		           2'b10: clk_dmplldiv3_div32		    (9 MHz)
    //		           2'b11: 1'b0
    //                      endcase
    //        011: dtmb_clk36_buf(36 MHz)
    //        100: dtmb_clk9_buf(9 MHz)
    //        101: 1'b0
    //        110: 1'b0
    //        111: 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0404);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h3b, 2'b11, 16'h0404);
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x04);
    
    // Muji
    // [1:0]  : reg_ckg_isdbt_outer1x_dvbt_outer1x
    //          [0]  : disable clock
    //          [1]  : invert clock
    //          [3:2]: Select clock source 
    //                 vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //                 sel[0]= (reg_demod_isdbt_on & reg_ckg_isdbt_outer1x[2])
    //                 sel[1]= (~reg_demod_isdbt_on)                          
    //                 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //                 00: isdbt_clk6_lat(6 MHz)
    //                 01: isdbt_clk8_lat(8 MHz)
    //                 10: clk_dmplldiv10_div2(43.2 MHz)
    //                 11: 1'b0
    // [6:4]  : reg_ckg_miu_dvbtc_outer2x
    //          [0]  : disable clock
    //          [1]  : invert clock
    //          [2]  : Select clock source 
    //                 0: clk_miu_p
    //                 1: clk_dmplldiv10(86.4 MHz)
    // [12:8] : reg_ckg_dvbtc_rs
    //          [0]  : disable clock
    //          [1]  : invert clock
    //          [4:2]: Select clock source 
    //                 000: clk_dmplldiv10(86.4 MHz)
    //                 001: clk_dmplldiv10_div2(43.2 MHz)
    //                 010: clk_atsc50_p
    //                      ^^^^^^^^^^^^
    //		        if(reg_atsc_adc_sel_mplldiv2) => clk_dmplldiv2_div7(49.7 MHz)
    //                      else                          => clk_dmplldiv17(50.82 MHz)
    //                 011: clk_dvbtc_rs_216_buf(216 MHz)
    //                 100: clk_dvbtc_rs_172_buf(172 MHz)
    //                 101: clk_dvbtc_rs_144_buf(144 MHz)
    //                 110: 1'b0
    //                 111: 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h27, 2'b10, 16'h0800);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h27, 2'b10, 16'h0800);
    _HAL_DMD_RIU_WriteByte(0x111f4f, 0x08);

    // ================================================================
    //  ISDBT SRAM pool clock enable
    // ================================================================
    // [3:0]    : reg_ckg_share_dtmb_inner12x_isdbt_inner2x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dmplldiv10_div2	\_________________> clk_isdbt_inner2x_dvbt_inner2x,	*sel[0] = (reg_demod_isdbt_on) | TEST_CLK_EN
    //			    1 : clk_isdbt_inner2x_p	/
    //			    2 : clk_dtmb_inner12x_4x_dvbtc_rs_mux --------> clk_dtmb_inner12x_4x_dvbtc_rs
    //			    3 : 1'b0
    // [7:4]    : reg_ckg_share_dtmb_inner8x_rs_isdbt_inner2x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dmplldiv10_div2	\_________________> clk_isdbt_inner2x_dvbt_inner2x,	*sel[0] = (reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk144_buf		\_________________> clk_dtmb_inner8x_dvbtc_rs,		*sel[0] = reg_ckg_dtmb_inner8x_dvbtc_rs[2] | TEST_CLK_EN
    //			:   3 : clk_dvbtc_rs_mux	/
    // [11:8]   : reg_ckg_share_dtmb_inner8x_isdbt_inner2x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dmplldiv10_div2	\_________________> clk_isdbt_inner2x_dvbt_inner2x,	*sel[0] = (reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk144_buf		------------------> clk_dtmb_inner8x
    //			:   3 : 1'b0
    // [15:12]  : reg_ckg_share_dtmb_outer2x_isdbt_inner2x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dmplldiv10_div2	\_________________> clk_isdbt_inner2x_dvbt_inner2x,	*sel[0] = (reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_eq_clk36_buf	------------------> clk_dtmb_outer2x
    //			:   3 : 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h40, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h40, 2'b11, 16'h0000);
    _HAL_DMD_RIU_WriteByte(0x111f81, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f80, 0x00);


    // [3:0]    : reg_ckg_share_dtmb_inner2x_isdbt_inner2x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dmplldiv10_div2	\_________________> clk_isdbt_inner2x_dvbt_inner2x,	*sel[0] = (reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk36_buf		------------------> clk_dtmb_inner2x
    //			:   3 : 1'b0
    // [7:4]    : reg_ckg_share_dtmb_inner12x_isdbt_inner4x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram4_p	\_________________> clk_dvbtc_sram4_isdbt_inner4x,	*sel[0] = reg_demod_isdbt_on | TEST_CLK_EN;
    //			:   1 : clk_isdbt_inner4x_p	/
    //			:   2 : clk_dtmb_inner12x_4x_dvbtc_rs_mux --------> clk_dtmb_inner12x_4x_dvbtc_rs,
    //			:   3 : 1'b0
    // [11:8]   : reg_ckg_share_dtmb_eq2x_isdbt_outer6x
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : isdbt_clk36_lat		\_________________> clk_isdbt_outer6x,			*sel[0] = reg_ckg_isdbt_outer6x[2] | TEST_CLK_EN
    //			:   1 : isdbt_clk48_lat		/
    //			:   2 : clk_dtmb_eq2x_inner12x_mux ---------------> clk_dtmb_eq2x_inner12x
    //			:   3 : 1'b0
    // [15:12]  : reg_ckg_share_dtmb_eq0p5x_isdbt_sram0
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram0_p	\_________________> clk_sram0,				*sel[0] = reg_demod_isdbt_on     |  TEST_CLK_EN
    //			:   1 : clk_isdbt_inner4x_p	/
    //			:   2 : dtmb_eq_clk72_buf	\_________________> clk_dtmb_eq0p5x_inner4x		*sel[0] = reg_ckg_dtmb_eq0p5x_inner4x[2] | extmd
    //			:   3 : dtmb_clk72_buf		/
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h41, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h41, 2'b11, 16'h0000);
    _HAL_DMD_RIU_WriteByte(0x111f83, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f82, 0x00);


    // [3:0]    : reg_ckg_share_dtmb_eq2x_isdbt_sram1
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram1_p	\_________________> clk_sram1,				*sel[0] = reg_demod_isdbt_on     |  TEST_CLK_EN
    //			:   1 : clk_isdbt_inner4x_p	/
    //			:   2 : clk_dtmb_eq2x_inner2x_12x_mux ------------> clk_dtmb_eq2x_inner2x_12x
    //			:   3 : 1'b0
    // [7:4]    : reg_ckg_share_dtmb_inner6x_isdbt_sram3
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram3_p	\_________________> clk_sram3,				*sel[0] = ((~reg_fed_srd_on) & reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk108_buf		------------------> clk_dtmb_inner6x
    //			:   3 : 1'b0
    // [11:8]   : reg_ckg_share_dtmb_eq2x_isdbt_sram3
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram3_p	\_________________> clk_sram3,				*sel[0] = ((~reg_fed_srd_on) & reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : clk_dtmb_eq2x_inner12x_mux ---------------> clk_dtmb_eq2x_inner12x
    //			:   3 : 1'b0
    // [15:12]  : reg_ckg_share_dtmb_inner12x_isdbt_sram4
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram4_p	\_________________> clk_sram4,				*sel[0] = ((~reg_fed_srd_on) & reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk216_buf		------------------> clk_dtmb_inner12x
    //			:   3 : 1'b0
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h42, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h42, 2'b11, 16'h0000);
    _HAL_DMD_RIU_WriteByte(0x111f85, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f84, 0x00);


    // [3:0]    : reg_ckg_share_dtmb_inner12x_eq0p5x_isdbt_sram4
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram4_p	\_________________> clk_sram4,				*sel[0] = ((~reg_fed_srd_on) & reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk216_buf		\_________________> clk_dtmb_inner12x_eq0p5x,		*sel[0] = reg_ckg_dtmb_inner12x_eq0p5x[2] | extmd
    //			:   3 : dtmb_eq_clk72_buf	/
    // [7:4]    : reg_ckg_share_dtmb_eq0p25x_isdbt_sram4
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram4_p	\_________________> clk_sram4,				*sel[0] = ((~reg_fed_srd_on) & reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_eq_clk36_buf	------------------> clk_dtmb_eq0p25x
    //			:   3 : 1'b0
    // [11:8]   : reg_ckg_share_dtmb_inner2x_isdbt_sram4
    //		[0]	: disable clock
    //		[1]	: invert clock
    //		[3:2]	:   0 : clk_dvbtc_sram4_p	\_________________> clk_sram4,				*sel[0] = ((~reg_fed_srd_on) & reg_demod_isdbt_on) | TEST_CLK_EN
    //			:   1 : clk_isdbt_inner2x_p	/
    //			:   2 : dtmb_clk36_buf		\_________________> clk_dtmb_inner2x_dvbtc_rs,		*sel[0] = reg_ckg_dtmb_inner2x_dvbtc_rs[2] | TEST_CLK_EN
    //			:   3 : clk_dvbtc_rs_mux	/
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h43, 2'b11, 16'h0000);
    // `RIU_W((`RIUBASE_CLKGEN_DMD>>1)+7'h43, 2'b11, 16'h0000);
    _HAL_DMD_RIU_WriteByte(0x111f87, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f86, 0x00);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);  
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_WHISKY)
void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val=0x00;
    
    printf("--------------DMD_ATSC_CHIP_WHISKY--------------\n");
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
    
    // DMDMCU 108M
    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    // Set parallel TS clock
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    // Enable ATSC, DVBTC TS clock
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    // Enable ADC clock in clkgen_demod
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    // Reset TS divider
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);
    // ADC select MPLLDIV17 & EQ select MPLLDIV5
    _HAL_DMD_RIU_WriteByte(0x111f43, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f42, 0x00);
    // Enable ATSC clock
    _HAL_DMD_RIU_WriteByte(0x111f03, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f02, 0x00);
    // configure reg_ckg_atsc50, reg_ckg_atsc25, reg_ckg_atsc_eq25 and reg_ckg_atsc_ce25    
    _HAL_DMD_RIU_WriteByte(0x111f04, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f07, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f06, 0x00);
    // enable clk_atsc_adcd_sync
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    // Enable DVBC outer clock
    _HAL_DMD_RIU_WriteByte(0x111f12, 0x00);
    // Enable SRAM clock
    _HAL_DMD_RIU_WriteByte(0x111f19, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f18, 0x00);
    // select clock
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
    // enable CCI LMS clock
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x00);
    // set symbol rate
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x04);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x04);
    // reg_ckg_adc1x_eq1x
    _HAL_DMD_RIU_WriteByte(0x111f49, 0x04);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39,(u8Val| 0x03));
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN)
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;

    #if DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI
    printf("--------------DMD_ATSC_CHIP_MASERATI--------------\n");
    #else
    printf("--------------DMD_ATSC_CHIP_MACAN--------------\n");
    #endif

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    _HAL_DMD_RIU_WriteByte(0x10331e, 0x10); 
    
    _HAL_DMD_RIU_WriteByte(0x103301, 0x05);
    _HAL_DMD_RIU_WriteByte(0x103300, 0x11);
    
    _HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    _HAL_DMD_RIU_WriteByte(0x103314, 0x08);
    
    _HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    _HAL_DMD_RIU_WriteByte(0x103302, 0x00);    
    
    _HAL_DMD_RIU_WriteByte(0x152928, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x152903, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152902, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x152905, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152904, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x152907, 0x00);                                                                  
    _HAL_DMD_RIU_WriteByte(0x152906, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a, 0x08);  
    
    _HAL_DMD_RIU_WriteByte(0x111f21, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f20, 0x40);
    
    _HAL_DMD_RIU_WriteByte(0x111f23, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111f22, 0x44);
    
    _HAL_DMD_RIU_WriteByte(0x111f3b, 0x08);  
    _HAL_DMD_RIU_WriteByte(0x111f3a, 0x08);
    
    _HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f70, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
    _HAL_DMD_RIU_WriteByte(0x111f72, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f79, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111f78, 0x18);
    
    _HAL_DMD_RIU_WriteByte(0x152991, 0x88);
    _HAL_DMD_RIU_WriteByte(0x152990, 0x88);
    
    _HAL_DMD_RIU_WriteByte(0x111f69, 0x44);
    _HAL_DMD_RIU_WriteByte(0x111f68, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f75, 0x81);
    _HAL_DMD_RIU_WriteByte(0x111f74, 0x11);
    
    _HAL_DMD_RIU_WriteByte(0x111f77, 0x81);
    _HAL_DMD_RIU_WriteByte(0x111f76, 0x88);
    
    _HAL_DMD_RIU_WriteByte(0x15298f, 0x11);
    _HAL_DMD_RIU_WriteByte(0x15298e, 0x88);
    
    _HAL_DMD_RIU_WriteByte(0x152923, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152922, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x111f25, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111f24, 0x11);
                                
    _HAL_DMD_RIU_WriteByte(0x152971, 0x1c);
    _HAL_DMD_RIU_WriteByte(0x152970, 0xc1);
                                
    _HAL_DMD_RIU_WriteByte(0x152977, 0x04);
    _HAL_DMD_RIU_WriteByte(0x152976, 0x04);
                                 
    _HAL_DMD_RIU_WriteByte(0x111f6f, 0x11);
    _HAL_DMD_RIU_WriteByte(0x111f6e, 0x00);
                                 
    _HAL_DMD_RIU_WriteByte(0x111feb, 0x18);
                                 
    _HAL_DMD_RIU_WriteByte(0x111f7f, 0x10);
    _HAL_DMD_RIU_WriteByte(0x111f7e, 0x11);
                                 
    _HAL_DMD_RIU_WriteByte(0x111f31, 0x14);
                                 
    _HAL_DMD_RIU_WriteByte(0x152981, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152980, 0x00);
                        
    _HAL_DMD_RIU_WriteByte(0x152983, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152982, 0x00);
                        
    _HAL_DMD_RIU_WriteByte(0x152985, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152984, 0x00);
                        
    _HAL_DMD_RIU_WriteByte(0x152987, 0x00);
    _HAL_DMD_RIU_WriteByte(0x152986, 0x00);
    
    _HAL_DMD_RIU_WriteByte(0x152979, 0x11);
    _HAL_DMD_RIU_WriteByte(0x152978, 0x14);
    
    _HAL_DMD_RIU_WriteByte(0x15298d, 0x81);
    _HAL_DMD_RIU_WriteByte(0x15298c, 0x44);
    
    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);  
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG )
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;

    printf("--------------DMD_ATSC_CHIP_MUSTANG--------------\n");

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   
    _HAL_DMD_RIU_WriteByte(0x101e39,0x00);
    
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x103301,0x05);
    _HAL_DMD_RIU_WriteByte(0x103300,0x11);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103308,0x00);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x08);
                                  
    _HAL_DMD_RIU_WriteByte(0x103302,0x01);
    _HAL_DMD_RIU_WriteByte(0x103302,0x00);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152928,0x00);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152903,0x00);
    _HAL_DMD_RIU_WriteByte(0x152902,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x152905,0x00);
    _HAL_DMD_RIU_WriteByte(0x152904,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x152907,0x00);
    _HAL_DMD_RIU_WriteByte(0x152906,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x08);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f21,0x44);
    _HAL_DMD_RIU_WriteByte(0x111f20,0x40);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x111f23,0x10);
    _HAL_DMD_RIU_WriteByte(0x111f22,0x44);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f3b,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f3a,0x08);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f71,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f70,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f73,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f72,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f79,0x11);
    _HAL_DMD_RIU_WriteByte(0x111f78,0x18);
                                  
    _HAL_DMD_RIU_WriteByte(0x152991,0x88);
    _HAL_DMD_RIU_WriteByte(0x152990,0x88);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x111f69,0x44);
    _HAL_DMD_RIU_WriteByte(0x111f68,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f75,0x81);
    _HAL_DMD_RIU_WriteByte(0x111f74,0x11);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x111f77,0x81);
    _HAL_DMD_RIU_WriteByte(0x111f76,0x88);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x15298f,0x11);
    _HAL_DMD_RIU_WriteByte(0x15298e,0x88);
                                  
    _HAL_DMD_RIU_WriteByte(0x152923,0x00);
    _HAL_DMD_RIU_WriteByte(0x152922,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f25,0x10);
    _HAL_DMD_RIU_WriteByte(0x111f24,0x11);
                                  
    _HAL_DMD_RIU_WriteByte(0x152971,0x1c);
    _HAL_DMD_RIU_WriteByte(0x152970,0xc1);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152977,0x04);
    _HAL_DMD_RIU_WriteByte(0x152976,0x04);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x111f6f,0x11);
    _HAL_DMD_RIU_WriteByte(0x111f6e,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x111feb,0x18);
                                  
    _HAL_DMD_RIU_WriteByte(0x111f31,0x14);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152981,0x00);
    _HAL_DMD_RIU_WriteByte(0x152980,0x00);
                                  
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152983,0x00);
    _HAL_DMD_RIU_WriteByte(0x152982,0x00);
                                  
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152985,0x00);
    _HAL_DMD_RIU_WriteByte(0x152984,0x00);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x152987,0x00);
    _HAL_DMD_RIU_WriteByte(0x152986,0x00);
                                  
    _HAL_DMD_RIU_WriteByte(0x152979,0x11);
    _HAL_DMD_RIU_WriteByte(0x152978,0x14);
                                  
                                  
    _HAL_DMD_RIU_WriteByte(0x15298d,0x81);
    _HAL_DMD_RIU_WriteByte(0x15298c,0x44);
                              
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}
#elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM )
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    MS_U8 u8Val = 0;

    printf("--------------DMD_ATSC_CHIP_MAXIM--------------\n");

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    _HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));
              
    _HAL_DMD_RIU_WriteByte(0x10331e,0x10);
                            
    _HAL_DMD_RIU_WriteByte(0x103301,0x05);
    _HAL_DMD_RIU_WriteByte(0x103300,0x11);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x103309,0x00);
    _HAL_DMD_RIU_WriteByte(0x103308,0x00);
                            
    _HAL_DMD_RIU_WriteByte(0x103315,0x00);
    _HAL_DMD_RIU_WriteByte(0x103314,0x08);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x103302,0x01);
                            
    _HAL_DMD_RIU_WriteByte(0x103302,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x103321,0x00);
    _HAL_DMD_RIU_WriteByte(0x103320,0x08);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152928,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152903,0x00);
    _HAL_DMD_RIU_WriteByte(0x152902,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152905,0x00);
    _HAL_DMD_RIU_WriteByte(0x152904,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152907,0x00);
    _HAL_DMD_RIU_WriteByte(0x152906,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x111f0b,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f0a,0x08);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x111f21,0x44);
    _HAL_DMD_RIU_WriteByte(0x111f20,0x40);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x111f23,0x10);
    _HAL_DMD_RIU_WriteByte(0x111f22,0x44);
                            
    _HAL_DMD_RIU_WriteByte(0x111f3b,0x08);
    _HAL_DMD_RIU_WriteByte(0x111f3a,0x08);
                            
    _HAL_DMD_RIU_WriteByte(0x111f71,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f70,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f73,0x00);
    _HAL_DMD_RIU_WriteByte(0x111f72,0x00);
                            
    _HAL_DMD_RIU_WriteByte(0x111f79,0x11);
    _HAL_DMD_RIU_WriteByte(0x111f78,0x18);
                            
    _HAL_DMD_RIU_WriteByte(0x152991,0x88);
    _HAL_DMD_RIU_WriteByte(0x152990,0x88);
                            
    _HAL_DMD_RIU_WriteByte(0x111f69,0x44);
    _HAL_DMD_RIU_WriteByte(0x111f68,0x00);
                            
    _HAL_DMD_RIU_WriteByte(0x111f75,0x81);
    _HAL_DMD_RIU_WriteByte(0x111f74,0x11);
                            
    _HAL_DMD_RIU_WriteByte(0x111f77,0x81);
    _HAL_DMD_RIU_WriteByte(0x111f76,0x88);
                            
    _HAL_DMD_RIU_WriteByte(0x15298f,0x11);
    _HAL_DMD_RIU_WriteByte(0x15298e,0x88);
                            
    _HAL_DMD_RIU_WriteByte(0x152923,0x00);
    _HAL_DMD_RIU_WriteByte(0x152922,0x00);
                            
    _HAL_DMD_RIU_WriteByte(0x111f25,0x10);
    _HAL_DMD_RIU_WriteByte(0x111f24,0x11);
                            
    _HAL_DMD_RIU_WriteByte(0x152971,0x1c);
    _HAL_DMD_RIU_WriteByte(0x152970,0xc1);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152977,0x04);
    _HAL_DMD_RIU_WriteByte(0x152976,0x04);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x111f6f,0x11);
    _HAL_DMD_RIU_WriteByte(0x111f6e,0x00);
                            
    _HAL_DMD_RIU_WriteByte(0x111feb,0x18);
                            
    _HAL_DMD_RIU_WriteByte(0x111f7f,0x10);
    _HAL_DMD_RIU_WriteByte(0x111f7e,0x11);
                            
    _HAL_DMD_RIU_WriteByte(0x111f31,0x14);
                            
    _HAL_DMD_RIU_WriteByte(0x152981,0x00);
    _HAL_DMD_RIU_WriteByte(0x152980,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152983,0x00);
    _HAL_DMD_RIU_WriteByte(0x152982,0x00);
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152985,0x00);
    _HAL_DMD_RIU_WriteByte(0x152984,0x00);
                            
                            
                            
    _HAL_DMD_RIU_WriteByte(0x152987,0x00);
    _HAL_DMD_RIU_WriteByte(0x152986,0x00);
                            
    _HAL_DMD_RIU_WriteByte(0x152979,0x11);
    _HAL_DMD_RIU_WriteByte(0x152978,0x14);
                            
    _HAL_DMD_RIU_WriteByte(0x15298d,0x81);
    _HAL_DMD_RIU_WriteByte(0x15298c,0x44);
                            
    _HAL_DMD_RIU_WriteByteMask(0x101e39, 0x03, 0x03);
}

#else
static void _HAL_INTERN_ATSC_InitClk(MS_BOOL bRFAGCTristateEnable)
{
    printf("--------------DMD_ATSC_CHIP_NONE--------------\n");
}
#endif

static MS_BOOL _HAL_INTERN_ATSC_Download(void)
{
    DMD_ATSC_ResData *pRes  = psDMD_ATSC_ResData + u8DMD_ATSC_DMD_ID;
    
    MS_U8 udata = 0x00;
    MS_U16 i=0;
    MS_U16 fail_cnt=0;
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    MS_U8  u8TmpData;
    MS_U16 u16AddressOffset;
    #endif
    
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T3_T10)
    if (_HAL_DMD_RIU_ReadByte(0x101E3E) != 0x08) HAL_PWS_Stop_VDMCU();
    else
    {
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x01); // reset VD_MCU
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x00);
        MsOS_DelayTask(20);
        return TRUE;
    }
    #else
    if (pRes->sDMD_ATSC_PriData.bDownloaded)
    {
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
        udata=_HAL_DMD_RIU_ReadByte(DMDMcuBase+0x00);
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, (udata|0x02)); // reset RIU remapping reset
        udata=_HAL_DMD_RIU_ReadByte(DMDMcuBase+0x00);
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  (udata|0x01)); // reset VD_MCU
        #else
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset MCU, madison patch
        #endif
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x00);
        MsOS_DelayTask(20);
        return TRUE;
    }
    #endif
    
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
    udata=_HAL_DMD_RIU_ReadByte(DMDMcuBase+0x00);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, (udata|0x02)); // reset RIU remapping reset
    udata=_HAL_DMD_RIU_ReadByte(DMDMcuBase+0x00);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  (udata|0x01)); // reset VD_MCU
    #else
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset MCU, madison patch
    #endif
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x00); // disable SRAM
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
    udata=_HAL_DMD_RIU_ReadByte(DMDMcuBase+0x00);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, (udata&(~0x01))); // release MCU, madison patch
    #else
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x00);// release MCU, madison patchs
    #endif
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // enable "vdmcu51_if"
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x51); // enable auto-increase
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    ////  Load code thru VDMCU_IF ////
    HAL_INTERN_ATSC_DBINFO(printf(">Load Code...\n"));

    for (i = 0; i < u16Lib_size; i++)
    {
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, INTERN_ATSC_table[i]); // write data to VD MCU 51 code sram
    }

    ////  Content verification ////
    HAL_INTERN_ATSC_DBINFO(printf(">Verify Code...\n"));

    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    for (i = 0; i < u16Lib_size; i++)
    {
        udata = _HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10); // read sram data

        if (udata != INTERN_ATSC_table[i])
        {
            HAL_INTERN_ATSC_DBINFO(printf(">fail add = 0x%x\n", i));
            HAL_INTERN_ATSC_DBINFO(printf(">code = 0x%x\n", INTERN_ATSC_table[i]));
            HAL_INTERN_ATSC_DBINFO(printf(">data = 0x%x\n", udata));

            if (fail_cnt++ > 10)
            {
                HAL_INTERN_ATSC_DBINFO(printf(">DSP Loadcode fail!"));
                return FALSE;
            }
        }
    }

    #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    _initTable();
    #endif
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x80); // sram address low byte
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T8_T9 || DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x6B); // sram address high byte
    #else
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x5B); // sram address high byte
    #endif
    
    for (i = 0; i < sizeof(Demod_Flow_register); i++)
    {
        _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, Demod_Flow_register[i]);
    }
    #else // #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)
    u16AddressOffset = (INTERN_ATSC_table[0x400] << 8)|INTERN_ATSC_table[0x401];
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, (u16AddressOffset&0xFF)); // sram address low byte
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, (u16AddressOffset>>8));   // sram address high byte
     
    u8TmpData = (MS_U8)pRes->sDMD_ATSC_InitData.u16IF_KHZ;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_ATSC_InitData.u16IF_KHZ >> 8);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_ATSC_InitData.bIQSwap;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_ATSC_InitData.u16AGC_REFERENCE;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_ATSC_InitData.u16AGC_REFERENCE >> 8);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_ATSC_InitData.u8IS_DUAL;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)u8DMD_ATSC_DMD_ID;
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    #endif // #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)
    
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // diable auto-increase
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x00); // disable "vdmcu51_if"

    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
    udata=_HAL_DMD_RIU_ReadByte(DMDMcuBase+0x00);
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  (udata|0x01)); // reset VD_MCU
    #else
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset MCU, madison patch
    #endif
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x01); // enable SRAM
    _HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x00); // release VD_MCU
    
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T3_T10)
    _HAL_DMD_RIU_WriteByte(0x101E3E, 0x08);     // ATSC = BIT3 -> 0x08
    _HAL_DMD_RIU_WriteByte(0x11051C, 0x00);
    #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_T7)
    _HAL_DMD_RIU_WriteByte(0x11261C, 0x00);
    pRes->sDMD_ATSC_PriData.bDownloaded = true;
    #else
    pRes->sDMD_ATSC_PriData.bDownloaded = true;
    #endif
    
    MsOS_DelayTask(20);
    
    HAL_INTERN_ATSC_DBINFO(printf(">DSP Loadcode done.\n"));
    
    return TRUE;
}

static void _HAL_INTERN_ATSC_FWVERSION(void)
{
    MS_U8 data1,data2,data3;
    
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x20C4, &data1);
    _MBX_ReadReg(0x20C5, &data2);
    _MBX_ReadReg(0x20C6, &data3);
	#else
    _MBX_ReadReg(0x20C4, &data1);
    _MBX_ReadReg(0x20CF, &data2);
    _MBX_ReadReg(0x20D0, &data3);
	#endif

    HAL_INTERN_ATSC_DBINFO(printf("INTERN_ATSC_FW_VERSION:%x.%x.%x\n", data1, data2, data3));
}

static MS_BOOL _HAL_INTERN_ATSC_Exit(void)
{
    MS_U8 u8CheckCount = 0;

    _HAL_DMD_RIU_WriteByte(MBRegBase + 0x1C, 0x01);

    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    _HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, _HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51
    
    while ((_HAL_DMD_RIU_ReadByte(MBRegBase + 0x1C)&0x02) != 0x02)
    {
        MsOS_DelayTaskUs(10);

        if (u8CheckCount++ == 0xFF)
        {
            printf(">> ATSC Exit Fail!\n");
            return FALSE;
        }
    }
    
    printf(">> ATSC Exit Ok!\n");
    
    return TRUE;
}

static MS_BOOL _HAL_INTERN_ATSC_SoftReset(void)
{
    MS_U8 u8Data = 0xFF;
    
    //Reset FSM
    if (_MBX_WriteReg(0x20C0, 0x00)==FALSE) return FALSE;
    
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    while (u8Data != 0x02)
    #else
    while (u8Data != 0x00)
    #endif
    {
        if (_MBX_ReadReg(0x20C1, &u8Data)==FALSE) return FALSE;
    }
    
    #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)
    //Execute demod top reset
    _MBX_ReadReg(0x2002, &u8Data);
    _MBX_WriteReg(0x2002, (u8Data|0x10));
    return _MBX_WriteReg(0x2002, (u8Data&(~0x10)));
	#else
	return TRUE;
	#endif
}

static MS_BOOL _HAL_INTERN_ATSC_SetVsbMode(void)
{
    return _MBX_WriteReg(0x20C0, 0x08); 
}

static MS_BOOL _HAL_INTERN_ATSC_Set64QamMode(void)
{
    #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)
    if (_MBX_WriteReg(0x20C3, 0x00)==FALSE) return FALSE;
    #endif
    return _MBX_WriteReg(0x20C0, 0x04);
}

static MS_BOOL _HAL_INTERN_ATSC_Set256QamMode(void)
{
    #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_K3)
    if (_MBX_WriteReg(0x20C3, 0x01)==FALSE) return FALSE;
    #endif
    return _MBX_WriteReg(0x20C0, 0x04);
}

static MS_BOOL _HAL_INTERN_ATSC_SetModeClean(void)
{
    return _MBX_WriteReg(0x20C0, 0x00);
}

static DMD_ATSC_DEMOD_TYPE _HAL_INTERN_ATSC_Check8VSB64_256QAM(void)
{
    MS_U8 mode = 0;

    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x2A02, &mode); //EQ mode check
    
    mode &= 0x07;
    
    if (mode == QAM16_J83ABC) return DMD_ATSC_DEMOD_ATSC_16QAM;
    else if (mode == QAM32_J83ABC) return DMD_ATSC_DEMOD_ATSC_32QAM;
    else if (mode == QAM64_J83ABC) return DMD_ATSC_DEMOD_ATSC_64QAM;
    else if (mode == QAM128_J83ABC) return DMD_ATSC_DEMOD_ATSC_128QAM;
    else if (mode == QAM256_J83ABC) return DMD_ATSC_DEMOD_ATSC_256QAM;
    else return DMD_ATSC_DEMOD_ATSC_256QAM;
    #else
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1700, &mode); //mode check
    #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
    _MBX_ReadReg(0x0400, &mode); //mode check  (atsc_dmd)
    #else
    _MBX_ReadReg(0x2900, &mode); //mode check
    #endif

    if ((mode&VSB_ATSC) == VSB_ATSC) return DMD_ATSC_DEMOD_ATSC_VSB;
    else if ((mode & QAM256_ATSC) == QAM256_ATSC) return DMD_ATSC_DEMOD_ATSC_256QAM;
    else return DMD_ATSC_DEMOD_ATSC_64QAM;
    #endif
}

static MS_BOOL _HAL_INTERN_ATSC_Vsb_QAM_AGCLock(void)
{
    MS_U8 data1 = 0;

    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x2829, &data1); //AGC_LOCK
    #else
    _MBX_ReadReg(0x271D, &data1); //AGC_LOCK
    #endif
    #else
    _MBX_ReadReg(0x293A, &data1); //AGC_LOCK
    #endif
    
    if(data1&0x01)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_INTERN_ATSC_Vsb_PreLock(void)
{
    MS_U8 data1 = 0;
    MS_U8 data2 = 0;
    MS_U16 checkValue;

    _MBX_ReadReg(0x20C2, &data1); //<0>TR_LOCK, <1>PTK_LOCK
    
    if ((data1&0x02) == 0x02)
    {
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
            || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
        _MBX_ReadReg(0x18EA, &data1);
        _MBX_ReadReg(0x18EB, &data2);
        #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
        _MBX_ReadReg(0x05EA, &data1); //atsc_dmdext
        _MBX_ReadReg(0x05EB, &data2); //atsc_dmdext
        #else
        _MBX_ReadReg(0x2AEA, &data1);
        _MBX_ReadReg(0x2AEB, &data2);
        #endif
        
        checkValue  = data1 << 8;
        checkValue |= data2;

        HAL_INTERN_ATSC_DBINFO(printf("Internal Pre Locking time :[%d]ms\n",checkValue));

        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nPreLock - FALSE"));
        
        return FALSE;
    }
}

static MS_BOOL _HAL_INTERN_ATSC_Vsb_FSync_Lock(void)
{
    MS_U8 data1 = 0;
    MS_U8 data2 = 0;
    MS_U16 checkValue;
 
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1824, &data1); //<4>1:Field Sync lock = Fsync lock
    #else
    _MBX_ReadReg(0x2A24, &data1); //<4>1:Field Sync lock = Fsync lock
    #endif

    if ((data1&0x10) == 0x10)
    {
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
            || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
        _MBX_ReadReg(0x18EE, &data1);
        _MBX_ReadReg(0x18EF, &data2);
        #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
        _MBX_ReadReg(0x05EE, &data1); //atsc_dmdext
        _MBX_ReadReg(0x05EF, &data2); //atsc_dmdext
        #else
        _MBX_ReadReg(0x2AEE, &data1);
        _MBX_ReadReg(0x2AEF, &data2);
        #endif
        
        checkValue  = data1 << 8;
        checkValue |= data2;

        HAL_INTERN_ATSC_DBINFO(printf("Internal Fsync Locking time :[%d]ms\n",checkValue));

        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nFsync Lock - FALSE"));
        
        return FALSE;
    }
}

static MS_BOOL _HAL_INTERN_ATSC_Vsb_CE_Lock(void)
{
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    return TRUE;
    #else
    MS_U8 data1 = 0;
    
    _MBX_ReadReg(0x20C2, &data1); //<4>1:CE Search Fail

    if((data1&0x10) == 0)
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nCE Lock"));
        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nCE unLock"));
        return FALSE;
    }
    #endif
}

static MS_BOOL _HAL_INTERN_ATSC_Vsb_FEC_Lock(void)
{
    MS_U8 data1=0, data2=0, data3=0, data4=0, data5=0;
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    MS_U8 data6 =0, data7 = 0;
    #endif

    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    _MBX_ReadReg(0x20C1, &data1);
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1A17, &data2);//AD_NOISE_PWR_TRAIN1
    #else
    _MBX_ReadReg(0x2C17, &data2);//AD_NOISE_PWR_TRAIN1
    #endif
    _MBX_ReadReg(0x20C2, &data3);//<0>TR_LOCK, <1>PTK_LOCK
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1901, &data4);//FEC_EN_CTL
    _MBX_ReadReg(0x1C67, &data5);//addy
    #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
    _MBX_ReadReg(0x0601, &data4);//FEC_EN_CTL (atsc_fec)
    _MBX_ReadReg(0x2D67, &data5);//addy  (atsc_eqext)
    #else
    _MBX_ReadReg(0x2B01, &data4);//FEC_EN_CTL
    _MBX_ReadReg(0x2D67, &data5);//addy
    #endif
    _MBX_ReadReg(0x1F01, &data6);
    _MBX_ReadReg(0x1F40, &data7);

    //Driver update 0426 :suggestion for field claim
    if (data1==INTERN_ATSC_OUTER_STATE &&
        ((data2<=INTERN_ATSC_VSB_TRAIN_SNR_LIMIT) || (data5 <= INTERN_ATSC_VSB_TRAIN_SNR_LIMIT)) &&
        ((data3&0x02)==0x02) &&
        ((data4&INTERN_ATSC_FEC_ENABLE)==INTERN_ATSC_FEC_ENABLE) &&
        ((data6&0x10) == 0x10) && ((data7&0x01) == 0x01))
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nFEC Lock"));
        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nFEC unLock"));
        return FALSE;
    }
    #else
    _MBX_ReadReg(0x20C1, &data1);
    _MBX_ReadReg(0x2C17, &data2); //AD_NOISE_PWR_TRAIN1 (DFS)
    _MBX_ReadReg(0x20C2, &data3); //<0>TR_LOCK, <1>PTK_LOCK
    _MBX_ReadReg(0x2B01, &data4); //FEC_EN_CTL
    _MBX_ReadReg(0x2C15, &data5); //AD_NOISE_PWR_TRAIN1 (DSS)

    if ((data1 == INTERN_ATSC_OUTER_STATE) &&
        ((data2 <= INTERN_ATSC_VSB_TRAIN_SNR_LIMIT) || (data5 <= INTERN_ATSC_VSB_TRAIN_SNR_LIMIT)) &&
        ((data3&0x02)==0x02) &&
        ((data4&INTERN_ATSC_FEC_ENABLE) == INTERN_ATSC_FEC_ENABLE))
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nFEC Lock"));
        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("\nFEC unLock"));
        return FALSE;
    }
    #endif
}

static MS_BOOL _HAL_INTERN_ATSC_QAM_PreLock(void)
{
    MS_U8 data1 = 0;
    
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x2950, &data1); //TR_LOCK
    #elif (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1B15, &data1); //TR_LOCK
    #else
    _MBX_ReadReg(0x2615, &data1); //TR_LOCK
    #endif
    #else
    _MBX_ReadReg(0x2921, &data1); //TR_LOCK
    #endif
    
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    if (data1&0x01)
    {
        HAL_INTERN_ATSC_DBINFO(printf("    QAM preLock OK  \n"));
        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("    QAM preLock NOT OK   \n"));
        return FALSE;
    }
    #else
    if((data1&0x10) == 0x10)
    {
        HAL_INTERN_ATSC_DBINFO(printf("    QAM preLock OK  \n"));
        return TRUE;
    }
    else
    {
        HAL_INTERN_ATSC_DBINFO(printf("    QAM preLock NOT OK   \n"));
        return FALSE;
    }
    #endif
}

static MS_BOOL _HAL_INTERN_ATSC_QAM_Main_Lock(void)
{
    #if DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1
    MS_U8 data1=0, data2=0, data3=0, data4=0, data5=0, data6=0;
    #else
    MS_U8 data1=0, data4=0, data5=0, data6=0;
    #endif

    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x20C1, &data1);
    _MBX_ReadReg(0x2B18, &data2); //boundary detected
    _MBX_ReadReg(0x2950, &data3); //TR_LOCK
    _MBX_ReadReg(0x2B01, &data4); //FEC_EN_CTL
    _MBX_ReadReg(0x2101, &data5); //RS_backend
    _MBX_ReadReg(0x2140, &data6); //RS_backend

    if (data1==INTERN_ATSC_OUTER_STATE && (data2&0x01)==0x01 &&
        data4==INTERN_ATSC_FEC_ENABLE && (data3&0x01) ==0x01 &&
        ((data5&0x10) == 0x10) && ((data6&0x01) == 0x01))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }    
    #elif (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    _MBX_ReadReg(0x20C1, &data1);
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1918, &data2); //boundary detected
    _MBX_ReadReg(0x1B15, &data3); //TR_LOCK
    _MBX_ReadReg(0x1901, &data4); //FEC_EN_CTL
    #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
    _MBX_ReadReg(0x0618, &data2); //boundary detected (atsc_fec)
    _MBX_ReadReg(0x2615, &data3); //TR_LOCK    (dmd_tr)
    _MBX_ReadReg(0x0601, &data4); //FEC_EN_CTL  (atsc_fec)
    #else
    _MBX_ReadReg(0x2B18, &data2); //boundary detected
    _MBX_ReadReg(0x2615, &data3); //TR_LOCK
    _MBX_ReadReg(0x2B01, &data4); //FEC_EN_CTL
    #endif
    _MBX_ReadReg(0x1F01, &data5);
    _MBX_ReadReg(0x1F40, &data6);
        
    if (data1==INTERN_ATSC_OUTER_STATE && (data2&0x01)==0x01 &&
        data4==INTERN_ATSC_FEC_ENABLE && (data3&0x10)==0x10 &&
        ((data5&0x10) == 0x10) && ((data6&0x01) == 0x01))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    #else
    _MBX_ReadReg(0x2B18, &data4); //boundary detected
    _MBX_ReadReg(0x2B01, &data5); //FEC_EN_CTL
    _MBX_ReadReg(0x2921, &data6); //TR_LOCK
    _MBX_ReadReg(0x20C1, &data1);

    if (data1==INTERN_ATSC_OUTER_STATE && (data4&0x01) == 0x01 &&
        (data5&INTERN_ATSC_FEC_ENABLE) == INTERN_ATSC_FEC_ENABLE &&
        (data6&0x10) == 0x10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    #endif
}

static MS_U8 _HAL_INTERN_ATSC_ReadIFAGC(void)
{
    MS_U16 data = 0;
    
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x2726, ((MS_U8*)(&data))+1); //reg_frontend
    _MBX_ReadReg(0x2727,  (MS_U8*)(&data));
    #elif (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_A1)
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG|| DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x280E, ((MS_U8*)(&data))+1);
    _MBX_ReadReg(0x280F, (MS_U8*)(&data));
    #else
    _MBX_ReadReg(0x2944, ((MS_U8*)(&data))+1);
    _MBX_ReadReg(0x2945,  (MS_U8*)(&data));
    #endif
    #endif
    
    return data;
}

static void _HAL_INTERN_ATSC_CheckSignalCondition(DMD_ATSC_SIGNAL_CONDITION* pstatus)
{
    DMD_ATSC_DEMOD_TYPE eMode;
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    MS_U8 u8NoisePowerH = 0, u8NoisePowerL = 0;
    static MS_U8 u8NoisePowerL_Last = 0xff;
    #else
    MS_U8 u8NoisePowerH=0;
    #endif
    static MS_U8 u8NoisePowerH_Last = 0xff;
    
    eMode = _HAL_INTERN_ATSC_Check8VSB64_256QAM();

    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x2ABE, &u8NoisePowerL); //DVBC_EQ
    _MBX_ReadReg(0x2ABF, &u8NoisePowerH); 
    #else
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1A15, &u8NoisePowerH);
    #else
    _MBX_ReadReg(0x2C15, &u8NoisePowerH); //(atsc_eq)
    #endif
    #endif
    
    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB) //VSB mode//SNR=10*log10((1344<<10)/noisepower)
    {
        if (!_HAL_INTERN_ATSC_Vsb_FEC_Lock()) u8NoisePowerH=0xFF;
        else if (abs(u8NoisePowerH_Last-u8NoisePowerH) > 5)
            u8NoisePowerH_Last = u8NoisePowerH;
        else u8NoisePowerH = u8NoisePowerH_Last;

        if (u8NoisePowerH > 0xBE) //SNR<14.5
            *pstatus=DMD_ATSC_SIGNAL_NO;
        else if (u8NoisePowerH > 0x4D) //SNR<18.4
            *pstatus=DMD_ATSC_SIGNAL_WEAK;
        else if (u8NoisePowerH > 0x23) //SNR<21.8
            *pstatus=DMD_ATSC_SIGNAL_MODERATE;
        else if (u8NoisePowerH > 0x0A) //SNR<26.9
            *pstatus=DMD_ATSC_SIGNAL_STRONG;
        else
            *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
    }
    else //QAM MODE
    {
        #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
        if (!_HAL_INTERN_ATSC_QAM_Main_Lock() || u8NoisePowerH) u8NoisePowerL=0xFF;
        else if (abs(u8NoisePowerL_Last-u8NoisePowerL) > 5)
            u8NoisePowerL_Last = u8NoisePowerL;
        else u8NoisePowerL = u8NoisePowerL_Last;
        
        //SNR=10*log10(65536/noisepower)
        if (eMode == DMD_ATSC_DEMOD_ATSC_256QAM)
        {
            if (u8NoisePowerL > 0x71) //SNR<27.6
                *pstatus=DMD_ATSC_SIGNAL_NO;
            else if (u8NoisePowerL > 0x31) //SNR<31.2
                *pstatus=DMD_ATSC_SIGNAL_WEAK;
            else if (u8NoisePowerL > 0x25) //SNR<32.4
                *pstatus=DMD_ATSC_SIGNAL_MODERATE;
            else if (u8NoisePowerL > 0x17) //SNR<34.4
                *pstatus=DMD_ATSC_SIGNAL_STRONG;
            else
                *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
        }
        else
        {
            if (u8NoisePowerL > 0x1D) //SNR<21.5
                *pstatus=DMD_ATSC_SIGNAL_NO;
            else if (u8NoisePowerL > 0x14) //SNR<25.4
                *pstatus=DMD_ATSC_SIGNAL_WEAK;
            else if (u8NoisePowerL > 0x0F) //SNR<27.8
                *pstatus=DMD_ATSC_SIGNAL_MODERATE;
            else if (u8NoisePowerL > 0x0B) //SNR<31.4
                *pstatus=DMD_ATSC_SIGNAL_STRONG;
            else
                *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
        }
        #else
        if (!_HAL_INTERN_ATSC_QAM_Main_Lock()) u8NoisePowerH=0xFF;
        else if (abs(u8NoisePowerH_Last-u8NoisePowerH) > 5)
            u8NoisePowerH_Last = u8NoisePowerH;
        else u8NoisePowerH = u8NoisePowerH_Last;
        
        if (eMode == DMD_ATSC_DEMOD_ATSC_256QAM) //256QAM//SNR=10*log10((2720<<10)/noisepower)
        {
            if (u8NoisePowerH > 0x13) //SNR<27.5
                *pstatus=DMD_ATSC_SIGNAL_NO;
            else if (u8NoisePowerH > 0x08) //SNR<31.2
                *pstatus=DMD_ATSC_SIGNAL_WEAK;
            else if (u8NoisePowerH > 0x06) //SNR<32.4
                *pstatus=DMD_ATSC_SIGNAL_MODERATE;
            else if (u8NoisePowerH > 0x04) //SNR<34.2
                *pstatus=DMD_ATSC_SIGNAL_STRONG;
            else
                *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
        }
        else //64QAM//SNR=10*log10((2688<<10)/noisepower)
        {
            if (u8NoisePowerH > 0x4C) //SNR<21.5
                *pstatus=DMD_ATSC_SIGNAL_NO;
            else if (u8NoisePowerH > 0x1F) //SNR<25.4
                *pstatus=DMD_ATSC_SIGNAL_WEAK;
            else if (u8NoisePowerH > 0x11) //SNR<27.8
                *pstatus=DMD_ATSC_SIGNAL_MODERATE;
            else if (u8NoisePowerH > 0x07) //SNR<31.4
                *pstatus=DMD_ATSC_SIGNAL_STRONG;
            else
                *pstatus=DMD_ATSC_SIGNAL_VERY_STRONG;
        }
        #endif
    }
}

static MS_U8 _HAL_INTERN_ATSC_ReadSNRPercentage(void)
{
    DMD_ATSC_DEMOD_TYPE eMode;
    MS_U8 u8NoisePowerH = 0, u8NoisePowerL = 0;
    MS_U16 u16NoisePower;

    eMode = _HAL_INTERN_ATSC_Check8VSB64_256QAM();
    
    #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    _MBX_ReadReg(0x2ABE, &u8NoisePowerL); //DVBC_EQ
    _MBX_ReadReg(0x2ABF, &u8NoisePowerH); 
    #else
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
        || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
    _MBX_ReadReg(0x1A14, &u8NoisePowerL);
    _MBX_ReadReg(0x1A15, &u8NoisePowerH);
    #else
    _MBX_ReadReg(0x2C14, &u8NoisePowerL);  //atsc_eq
    _MBX_ReadReg(0x2C15, &u8NoisePowerH);
    #endif
    #endif
    
    u16NoisePower = (u8NoisePowerH<<8) | u8NoisePowerL;

    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB) //VSB mode//SNR=10*log10((1344<<10)/noisepower)
    {
        if (!_HAL_INTERN_ATSC_Vsb_FEC_Lock())
            return 0;//SNR=0;
        else if (u16NoisePower<=0x008A)//SNR>=40dB
            return 100;//SNR=MAX_SNR;
        else if (u16NoisePower<=0x0097)//SNR>=39.6dB
            return 99;//
        else if (u16NoisePower<=0x00A5)//SNR>=39.2dB
            return 98;//
        else if (u16NoisePower<=0x00B5)//SNR>=38.8dB
            return 97;//
        else if (u16NoisePower<=0x00C7)//SNR>=38.4dB
            return 96;//
        else if (u16NoisePower<=0x00DA)//SNR>=38.0dB
            return 95;//
        else if (u16NoisePower<=0x00EF)//SNR>=37.6dB
            return 94;//
        else if (u16NoisePower<=0x0106)//SNR>=37.2dB
            return 93;//
        else if (u16NoisePower<=0x0120)//SNR>=36.8dB
            return 92;//
        else if (u16NoisePower<=0x013B)//SNR>=36.4dB
            return 91;//
        else if (u16NoisePower<=0x015A)//SNR>=36.0dB
            return 90;//
        else if (u16NoisePower<=0x017B)//SNR>=35.6dB
            return 89;//
        else if (u16NoisePower<=0x01A0)//SNR>=35.2dB
            return 88;//
        else if (u16NoisePower<=0x01C8)//SNR>=34.8dB
            return 87;//
        else if (u16NoisePower<=0x01F4)//SNR>=34.4dB
            return 86;//
        else if (u16NoisePower<=0x0224)//SNR>=34.0dB
            return 85;//
        else if (u16NoisePower<=0x0259)//SNR>=33.6dB
            return 84;//
        else if (u16NoisePower<=0x0293)//SNR>=33.2dB
            return 83;//
        else if (u16NoisePower<=0x02D2)//SNR>=32.8dB
            return 82;//
        else if (u16NoisePower<=0x0318)//SNR>=32.4dB
            return 81;//
        else if (u16NoisePower<=0x0364)//SNR>=32.0dB
            return 80;//
        else if (u16NoisePower<=0x03B8)//SNR>=31.6dB
            return 79;//
        else if (u16NoisePower<=0x0414)//SNR>=31.2dB
            return 78;//
        else if (u16NoisePower<=0x0479)//SNR>=30.8dB
            return 77;//
        else if (u16NoisePower<=0x04E7)//SNR>=30.4dB
            return 76;//
        else if (u16NoisePower<=0x0560)//SNR>=30.0dB
            return 75;//
        else if (u16NoisePower<=0x05E5)//SNR>=29.6dB
            return 74;//
        else if (u16NoisePower<=0x0677)//SNR>=29.2dB
            return 73;//
        else if (u16NoisePower<=0x0716)//SNR>=28.8dB
            return 72;//
        else if (u16NoisePower<=0x07C5)//SNR>=28.4dB
            return 71;//
        else if (u16NoisePower<=0x0885)//SNR>=28.0dB
            return 70;//
        else if (u16NoisePower<=0x0958)//SNR>=27.6dB
            return 69;//
        else if (u16NoisePower<=0x0A3E)//SNR>=27.2dB
            return 68;//
        else if (u16NoisePower<=0x0B3B)//SNR>=26.8dB
            return 67;//
        else if (u16NoisePower<=0x0C51)//SNR>=26.4dB
            return 66;//
        else if (u16NoisePower<=0x0D81)//SNR>=26.0dB
            return 65;//
        else if (u16NoisePower<=0x0ECF)//SNR>=25.6dB
            return 64;//
        else if (u16NoisePower<=0x103C)//SNR>=25.2dB
            return 63;//
        else if (u16NoisePower<=0x11CD)//SNR>=24.8dB
            return 62;//
        else if (u16NoisePower<=0x1385)//SNR>=24.4dB
            return 61;//
        else if (u16NoisePower<=0x1567)//SNR>=24.0dB
            return 60;//
        else if (u16NoisePower<=0x1778)//SNR>=23.6dB
            return 59;//
        else if (u16NoisePower<=0x19BB)//SNR>=23.2dB
            return 58;//
        else if (u16NoisePower<=0x1C37)//SNR>=22.8dB
            return 57;//
        else if (u16NoisePower<=0x1EF0)//SNR>=22.4dB
            return 56;//
        else if (u16NoisePower<=0x21EC)//SNR>=22.0dB
            return 55;//
        else if (u16NoisePower<=0x2531)//SNR>=21.6dB
            return 54;//
        else if (u16NoisePower<=0x28C8)//SNR>=21.2dB
            return 53;//
        else if (u16NoisePower<=0x2CB7)//SNR>=20.8dB
            return 52;//
        else if (u16NoisePower<=0x3108)//SNR>=20.4dB
            return 51;//
        else if (u16NoisePower<=0x35C3)//SNR>=20.0dB
            return 50;//
        else if (u16NoisePower<=0x3AF2)//SNR>=19.6dB
            return 49;//
        else if (u16NoisePower<=0x40A2)//SNR>=19.2dB
            return 48;//
        else if (u16NoisePower<=0x46DF)//SNR>=18.8dB
            return 47;//
        else if (u16NoisePower<=0x4DB5)//SNR>=18.4dB
            return 46;//
        else if (u16NoisePower<=0x5534)//SNR>=18.0dB
            return 45;//
        else if (u16NoisePower<=0x5D6D)//SNR>=17.6dB
            return 44;//
        else if (u16NoisePower<=0x6670)//SNR>=17.2dB
            return 43;//
        else if (u16NoisePower<=0x7052)//SNR>=16.8dB
            return 42;//
        else if (u16NoisePower<=0x7B28)//SNR>=16.4dB
            return 41;//
        else if (u16NoisePower<=0x870A)//SNR>=16.0dB
            return 40;//
        else if (u16NoisePower<=0x9411)//SNR>=15.6dB
            return 39;//
        else if (u16NoisePower<=0xA25A)//SNR>=15.2dB
            return 38;//
        else if (u16NoisePower<=0xB204)//SNR>=14.8dB
            return 37;//
        else if (u16NoisePower<=0xC331)//SNR>=14.4dB
            return 36;//
        else if (u16NoisePower<=0xD606)//SNR>=14.0dB
            return 35;//
        else if (u16NoisePower<=0xEAAC)//SNR>=13.6dB
            return 34;//
        else// if (u16NoisePower>=0xEAAC)//SNR<13.6dB
            return 33;//
    }
    else //QAM MODE
    {
        if( eMode == DMD_ATSC_DEMOD_ATSC_256QAM ) //256QAM//SNR=10*log10((2720<<10)/noisepower)
        {
            if (!_HAL_INTERN_ATSC_QAM_Main_Lock())
                return 0;//SNR=0;
            else if (u16NoisePower<=0x0117)//SNR>=40dB
                return 100;//
            else if (u16NoisePower<=0x0131)//SNR>=39.6dB
                return 99;//
            else if (u16NoisePower<=0x014F)//SNR>=39.2dB
                return 98;//
            else if (u16NoisePower<=0x016F)//SNR>=38.8dB
                return 97;//
            else if (u16NoisePower<=0x0193)//SNR>=38.4dB
                return 96;//
            else if (u16NoisePower<=0x01B9)//SNR>=38.0dB
                return 95;//
            else if (u16NoisePower<=0x01E4)//SNR>=37.6dB
                return 94;//
            else if (u16NoisePower<=0x0213)//SNR>=37.2dB
                return 93;//
            else if (u16NoisePower<=0x0246)//SNR>=36.8dB
                return 92;//
            else if (u16NoisePower<=0x027E)//SNR>=36.4dB
                return 91;//
            else if (u16NoisePower<=0x02BC)//SNR>=36.0dB
                return 90;//
            else if (u16NoisePower<=0x02FF)//SNR>=35.6dB
                return 89;//
            else if (u16NoisePower<=0x0349)//SNR>=35.2dB
                return 88;//
            else if (u16NoisePower<=0x039A)//SNR>=34.8dB
                return 87;//
            else if (u16NoisePower<=0x03F3)//SNR>=34.4dB
                return 86;//
            else if (u16NoisePower<=0x0455)//SNR>=34.0dB
                return 85;//
            else if (u16NoisePower<=0x04C0)//SNR>=33.6dB
                return 84;//
            else if (u16NoisePower<=0x0535)//SNR>=33.2dB
                return 83;//
            else if (u16NoisePower<=0x05B6)//SNR>=32.8dB
                return 82;//
            else if (u16NoisePower<=0x0643)//SNR>=32.4dB
                return 81;//
            else if (u16NoisePower<=0x06DD)//SNR>=32.0dB
                return 80;//
            else if (u16NoisePower<=0x0787)//SNR>=31.6dB
                return 79;//
            else if (u16NoisePower<=0x0841)//SNR>=31.2dB
                return 78;//
            else if (u16NoisePower<=0x090D)//SNR>=30.8dB
                return 77;//
            else if (u16NoisePower<=0x09EC)//SNR>=30.4dB
                return 76;//
            else if (u16NoisePower<=0x0AE1)//SNR>=30.0dB
                return 75;//
            else if (u16NoisePower<=0x0BEE)//SNR>=29.6dB
                return 74;//
            else if (u16NoisePower<=0x0D15)//SNR>=29.2dB
                return 73;//
            else if (u16NoisePower<=0x0E58)//SNR>=28.8dB
                return 72;//
            else if (u16NoisePower<=0x0FBA)//SNR>=28.4dB
                return 71;//
            else if (u16NoisePower<=0x113E)//SNR>=28.0dB
                return 70;//
            else if (u16NoisePower<=0x12E8)//SNR>=27.6dB
                return 69;//
            else if (u16NoisePower<=0x14BB)//SNR>=27.2dB
                return 68;//
            else if (u16NoisePower<=0x16BB)//SNR>=26.8dB
                return 67;//
            else if (u16NoisePower<=0x18ED)//SNR>=26.4dB
                return 66;//
            else if (u16NoisePower<=0x1B54)//SNR>=26.0dB
                return 65;//
            else if (u16NoisePower<=0x1DF7)//SNR>=25.6dB
                return 64;//
            else if (u16NoisePower<=0x20DB)//SNR>=25.2dB
                return 63;//
            else if (u16NoisePower<=0x2407)//SNR>=24.8dB
                return 62;//
            else if (u16NoisePower<=0x2781)//SNR>=24.4dB
                return 61;//
            else if (u16NoisePower<=0x2B50)//SNR>=24.0dB
                return 60;//
            else if (u16NoisePower<=0x2F7E)//SNR>=23.6dB
                return 59;//
            else if (u16NoisePower<=0x3413)//SNR>=23.2dB
                return 58;//
            else if (u16NoisePower<=0x3919)//SNR>=22.8dB
                return 57;//
            else if (u16NoisePower<=0x3E9C)//SNR>=22.4dB
                return 56;//
            else if (u16NoisePower<=0x44A6)//SNR>=22.0dB
                return 55;//
            else if (u16NoisePower<=0x4B45)//SNR>=21.6dB
                return 54;//
            else if (u16NoisePower<=0x5289)//SNR>=21.2dB
                return 53;//
            else if (u16NoisePower<=0x5A7F)//SNR>=20.8dB
                return 52;//
            else if (u16NoisePower<=0x633A)//SNR>=20.4dB
                return 51;//
            else if (u16NoisePower<=0x6CCD)//SNR>=20.0dB
                return 50;//
            else if (u16NoisePower<=0x774C)//SNR>=19.6dB
                return 49;//
            else if (u16NoisePower<=0x82CE)//SNR>=19.2dB
                return 48;//
            else if (u16NoisePower<=0x8F6D)//SNR>=18.8dB
                return 47;//
            else if (u16NoisePower<=0x9D44)//SNR>=18.4dB
                return 46;//
            else if (u16NoisePower<=0xAC70)//SNR>=18.0dB
                return 45;//
            else if (u16NoisePower<=0xBD13)//SNR>=17.6dB
                return 44;//
            else if (u16NoisePower<=0xCF50)//SNR>=17.2dB
                return 43;//
            else if (u16NoisePower<=0xE351)//SNR>=16.8dB
                return 42;//
            else if (u16NoisePower<=0xF93F)//SNR>=16.4dB
                return 41;//
            else// if (u16NoisePower>=0xF93F)//SNR<16.4dB
                return 40;//
        }
        else //64QAM//SNR=10*log10((2688<<10)/noisepower)
        {
            if (!_HAL_INTERN_ATSC_QAM_Main_Lock())
                return 0;//SNR=0;
            else if (u16NoisePower<=0x0113)//SNR>=40dB
                return 100;//
            else if (u16NoisePower<=0x012E)//SNR>=39.6dB
                return 99;//
            else if (u16NoisePower<=0x014B)//SNR>=39.2dB
                return 98;//
            else if (u16NoisePower<=0x016B)//SNR>=38.8dB
                return 97;//
            else if (u16NoisePower<=0x018E)//SNR>=38.4dB
                return 96;//
            else if (u16NoisePower<=0x01B4)//SNR>=38.0dB
                return 95;//
            else if (u16NoisePower<=0x01DE)//SNR>=37.6dB
                return 94;//
            else if (u16NoisePower<=0x020C)//SNR>=37.2dB
                return 93;//
            else if (u16NoisePower<=0x023F)//SNR>=36.8dB
                return 92;//
            else if (u16NoisePower<=0x0277)//SNR>=36.4dB
                return 91;//
            else if (u16NoisePower<=0x02B3)//SNR>=36.0dB
                return 90;//
            else if (u16NoisePower<=0x02F6)//SNR>=35.6dB
                return 89;//
            else if (u16NoisePower<=0x033F)//SNR>=35.2dB
                return 88;//
            else if (u16NoisePower<=0x038F)//SNR>=34.8dB
                return 87;//
            else if (u16NoisePower<=0x03E7)//SNR>=34.4dB
                return 86;//
            else if (u16NoisePower<=0x0448)//SNR>=34.0dB
                return 85;//
            else if (u16NoisePower<=0x04B2)//SNR>=33.6dB
                return 84;//
            else if (u16NoisePower<=0x0525)//SNR>=33.2dB
                return 83;//
            else if (u16NoisePower<=0x05A5)//SNR>=32.8dB
                return 82;//
            else if (u16NoisePower<=0x0630)//SNR>=32.4dB
                return 81;//
            else if (u16NoisePower<=0x06C9)//SNR>=32.0dB
                return 80;//
            else if (u16NoisePower<=0x0770)//SNR>=31.6dB
                return 79;//
            else if (u16NoisePower<=0x0828)//SNR>=31.2dB
                return 78;//
            else if (u16NoisePower<=0x08F1)//SNR>=30.8dB
                return 77;//
            else if (u16NoisePower<=0x09CE)//SNR>=30.4dB
                return 76;//
            else if (u16NoisePower<=0x0AC1)//SNR>=30.0dB
                return 75;//
            else if (u16NoisePower<=0x0BCA)//SNR>=29.6dB
                return 74;//
            else if (u16NoisePower<=0x0CED)//SNR>=29.2dB
                return 73;//
            else if (u16NoisePower<=0x0E2D)//SNR>=28.8dB
                return 72;//
            else if (u16NoisePower<=0x0F8B)//SNR>=28.4dB
                return 71;//
            else if (u16NoisePower<=0x110A)//SNR>=28.0dB
                return 70;//
            else if (u16NoisePower<=0x12AF)//SNR>=27.6dB
                return 69;//
            else if (u16NoisePower<=0x147D)//SNR>=27.2dB
                return 68;//
            else if (u16NoisePower<=0x1677)//SNR>=26.8dB
                return 67;//
            else if (u16NoisePower<=0x18A2)//SNR>=26.4dB
                return 66;//
            else if (u16NoisePower<=0x1B02)//SNR>=26.0dB
                return 65;//
            else if (u16NoisePower<=0x1D9D)//SNR>=25.6dB
                return 64;//
            else if (u16NoisePower<=0x2078)//SNR>=25.2dB
                return 63;//
            else if (u16NoisePower<=0x239A)//SNR>=24.8dB
                return 62;//
            else if (u16NoisePower<=0x270A)//SNR>=24.4dB
                return 61;//
            else if (u16NoisePower<=0x2ACE)//SNR>=24.0dB
                return 60;//
            else if (u16NoisePower<=0x2EEF)//SNR>=23.6dB
                return 59;//
            else if (u16NoisePower<=0x3376)//SNR>=23.2dB
                return 58;//
            else if (u16NoisePower<=0x386D)//SNR>=22.8dB
                return 57;//
            else if (u16NoisePower<=0x3DDF)//SNR>=22.4dB
                return 56;//
            else if (u16NoisePower<=0x43D7)//SNR>=22.0dB
                return 55;//
            else if (u16NoisePower<=0x4A63)//SNR>=21.6dB
                return 54;//
            else if (u16NoisePower<=0x5190)//SNR>=21.2dB
                return 53;//
            else if (u16NoisePower<=0x596E)//SNR>=20.8dB
                return 52;//
            else if (u16NoisePower<=0x620F)//SNR>=20.4dB
                return 51;//
            else if (u16NoisePower<=0x6B85)//SNR>=20.0dB
                return 50;//
            else if (u16NoisePower<=0x75E5)//SNR>=19.6dB
                return 49;//
            else if (u16NoisePower<=0x8144)//SNR>=19.2dB
                return 48;//
            else if (u16NoisePower<=0x8DBD)//SNR>=18.8dB
                return 47;//
            else if (u16NoisePower<=0x9B6A)//SNR>=18.4dB
                return 46;//
            else if (u16NoisePower<=0xAA68)//SNR>=18.0dB
                return 45;//
            else if (u16NoisePower<=0xBAD9)//SNR>=17.6dB
                return 44;//
            else if (u16NoisePower<=0xCCE0)//SNR>=17.2dB
                return 43;//
            else if (u16NoisePower<=0xE0A4)//SNR>=16.8dB
                return 42;//
            else if (u16NoisePower<=0xF650)//SNR>=16.4dB
                return 41;//
            else// if (u16NoisePower>=0xF650)//SNR<16.4dB
                return 40;//
        }
    }
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ATSC_GET_QAM_SNR(MS_U16 *pNoisepower, MS_U32 *pSym_num)
#else
static MS_BOOL _HAL_INTERN_ATSC_GET_QAM_SNR(float *f_snr)
#endif
{
    MS_U8 u8Data = 0;
    MS_U16 noisepower = 0;

    if (_HAL_INTERN_ATSC_QAM_Main_Lock())
    {
        // latch
        _MBX_WriteReg(0x2905, 0x80);
        // read noise power
        _MBX_ReadReg(0x2A45, &u8Data);
        noisepower = u8Data;
        _MBX_ReadReg(0x2A44, &u8Data);
        noisepower = (noisepower<<8)|u8Data;
        // unlatch
        _MBX_WriteReg(0x2905, 0x00);

        if (noisepower == 0x0000)
            noisepower = 0x0001;

        #ifdef UTPA2
         *pNoisepower = noisepower;
         *pSym_num = 65536;
        #else
        #ifdef MSOS_TYPE_LINUX
        *f_snr = 10.0f*log10f(65536.0f/(float)noisepower);
        #else
        *f_snr = 10.0f*Log10Approx(65536.0f/(float)noisepower);
        #endif
        #endif
    }
    else
    {
        #ifdef UTPA2
        *pNoisepower = 0;
        *pSym_num = 0;
        #else
        *f_snr = 0.0f;
        #endif
    }
    
    return TRUE;
}

static MS_U16 _HAL_INTERN_ATSC_ReadPKTERR(void)
{
    MS_U16 data = 0;
    MS_U8 reg = 0;
    DMD_ATSC_DEMOD_TYPE eMode;

    eMode = _HAL_INTERN_ATSC_Check8VSB64_256QAM();
    
    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB)
    {
        if (!_HAL_INTERN_ATSC_Vsb_FEC_Lock()) data = 0;
        else
        {
            #if (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_A1)
            _MBX_ReadReg(0x2B20, &reg);
            data = reg;
            _MBX_ReadReg(0x2B21, &reg);
            data = (data << 8)|reg;
            #else
            _MBX_ReadReg(0x1F67, &reg);
            data = reg;
            _MBX_ReadReg(0x1F66, &reg);
            data = (data << 8)|reg;
            #endif
        }
    }
    else
    {
        if (!_HAL_INTERN_ATSC_QAM_Main_Lock()) data = 0;
        else
        {
            #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
            _MBX_ReadReg(0x2166, &reg);
            data = reg;
            _MBX_ReadReg(0x2167, &reg);
            data = (data << 8)|reg;
            #elif (DMD_ATSC_CHIP_VERSION < DMD_ATSC_CHIP_A1)
            _MBX_ReadReg(0x2B20, &reg);
            data = reg;
            _MBX_ReadReg(0x2B21, &reg);
            data = (data << 8)|reg;
            #else
            _MBX_ReadReg(0x1F66, &reg);
            data = reg;
            _MBX_ReadReg(0x1F67, &reg);
            data = (data << 8)|reg;
            #endif
        }
    }
    
    return data;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ATSC_ReadBER(MS_U32 *pBitErr, MS_U16 *pError_window, MS_U32 *pWin_unit)
#else
static MS_BOOL _HAL_INTERN_ATSC_ReadBER(float *pBer)
#endif
{
    MS_BOOL status = true;
    MS_U8 reg = 0, reg_frz = 0;
    MS_U16 BitErrPeriod;
    MS_U32 BitErr;
    DMD_ATSC_DEMOD_TYPE eMode;

    eMode = _HAL_INTERN_ATSC_Check8VSB64_256QAM();

    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB)
    {
        if (!_HAL_INTERN_ATSC_Vsb_FEC_Lock())
        {
            #ifdef UTPA2
            *pBitErr = 0;
            *pError_window = 0;
            *pWin_unit = 0;
            #else
            *pBer = 0;
            #endif
        }
        else
        {
            _MBX_ReadReg(0x1F03, &reg_frz);
            _MBX_WriteReg(0x1F03, reg_frz|0x03);
            
            _MBX_ReadReg(0x1F47, &reg);
            BitErrPeriod = reg;
            _MBX_ReadReg(0x1F46, &reg);
            BitErrPeriod = (BitErrPeriod << 8)|reg;
            
            status &= _MBX_ReadReg(0x1F6d, &reg);
            BitErr = reg;
            status &= _MBX_ReadReg(0x1F6c, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _MBX_ReadReg(0x1F6b, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _MBX_ReadReg(0x1F6a, &reg);
            BitErr = (BitErr << 8)|reg;
            
            reg_frz=reg_frz&(~0x03);
            _MBX_WriteReg(0x1F03, reg_frz);
            
            if (BitErrPeriod == 0 )    //protect 0
                BitErrPeriod = 1;
            
            #ifdef UTPA2
            *pBitErr = BitErr;
            *pError_window = BitErrPeriod;
            *pWin_unit = 8*187*128;
            #else
            if (BitErr <=0 )
                *pBer = 0.5f / ((float)BitErrPeriod*8*187*128);
            else
                *pBer = (float)BitErr / ((float)BitErrPeriod*8*187*128);
            #endif
        }
    }
    else
    {
        if (!_HAL_INTERN_ATSC_QAM_Main_Lock())
        {
            #ifdef UTPA2
            *pBitErr = 0;
            *pError_window = 0;
            *pWin_unit = 0;
            #else
            *pBer = 0;
            #endif
        }
        else
        {
            #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
            _MBX_ReadReg(0x2103, &reg_frz);
            _MBX_WriteReg(0x2103, reg_frz|0x03);
            
            _MBX_ReadReg(0x2147, &reg);
            BitErrPeriod = reg;
            _MBX_ReadReg(0x2146, &reg);
            BitErrPeriod = (BitErrPeriod << 8)|reg;
            
            status &= _MBX_ReadReg(0x216d, &reg);
            BitErr = reg;
            status &= _MBX_ReadReg(0x216c, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _MBX_ReadReg(0x216b, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _MBX_ReadReg(0x216a, &reg);
            BitErr = (BitErr << 8)|reg;
            
            reg_frz=reg_frz&(~0x03);
            _MBX_WriteReg(0x2103, reg_frz);
            
            if (BitErrPeriod == 0)    //protect 0
                BitErrPeriod = 1;
            
            #ifdef UTPA2
            *pBitErr = BitErr;
            *pError_window = BitErrPeriod;
            *pWin_unit = 8*188*128;
            #else
            if (BitErr <=0)
                *pBer = 0.5f / ((float)BitErrPeriod*8*188*128);
            else
                *pBer = (float)BitErr / ((float)BitErrPeriod*8*188*128);
            #endif
            #else // #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
            _MBX_ReadReg(0x1F03, &reg_frz);
            _MBX_WriteReg(0x1F03, reg_frz|0x03);
            
            _MBX_ReadReg(0x1F47, &reg);
            BitErrPeriod = reg;
            _MBX_ReadReg(0x1F46, &reg);
            BitErrPeriod = (BitErrPeriod << 8)|reg;
            
            BitErr = reg;
            status &= _MBX_ReadReg(0x1F6c, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _MBX_ReadReg(0x1F6b, &reg);
            BitErr = (BitErr << 8)|reg;
            status &= _MBX_ReadReg(0x1F6a, &reg);
            BitErr = (BitErr << 8)|reg;
            
            reg_frz=reg_frz&(~0x03);
            _MBX_WriteReg(0x1F03, reg_frz);
            
            if (BitErrPeriod == 0 )    //protect 0
                BitErrPeriod = 1;
            
            #ifdef UTPA2
            *pBitErr = BitErr;
            *pError_window = BitErrPeriod;
            *pWin_unit = 7*122*128;
            #else
            if (BitErr <=0 )
                *pBer = 0.5f / ((float)BitErrPeriod*7*122*128);
            else
                *pBer = (float)BitErr / ((float)BitErrPeriod*7*122*128);
            #endif
            #endif // #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
        }
    }
    
    return status;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_ATSC_ReadFrequencyOffset(MS_U8 *pMode, MS_S16 *pFF, MS_S16 *pRate)
#else
static MS_S16 _HAL_INTERN_ATSC_ReadFrequencyOffset(void)
#endif
{
    DMD_ATSC_DEMOD_TYPE eMode;
    MS_U8 u8PTK_LOOP_FF_R3=0, u8PTK_LOOP_FF_R2=0;
    MS_U8 u8PTK_RATE_2=0;
    MS_U8 u8AD_CRL_LOOP_VALUE0=0, u8AD_CRL_LOOP_VALUE1=0;
    MS_U8 u8MIX_RATE_0=0, u8MIX_RATE_1=0, u8MIX_RATE_2=0;
    MS_S16 PTK_LOOP_FF;
    MS_S16 AD_CRL_LOOP_VALUE;
    MS_S16 MIX_RATE;
    #ifndef UTPA2
    MS_S16 FreqOffset = 0; //kHz
    #endif
    
    eMode = _HAL_INTERN_ATSC_Check8VSB64_256QAM();
    
    #ifdef UTPA2
    *pMode = eMode;
    #endif
    
    if (eMode == DMD_ATSC_DEMOD_ATSC_VSB) //VSB mode//
    {
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
            || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
        _MBX_WriteReg(0x177E, 0x01);
        _MBX_WriteReg(0x17E6, 0xff);
        _MBX_ReadReg(0x177C, &u8PTK_LOOP_FF_R2);
        _MBX_ReadReg(0x177D, &u8PTK_LOOP_FF_R3);
        _MBX_WriteReg(0x177E, 0x00);
        _MBX_WriteReg(0x17E6, 0xff);
        #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
        _MBX_WriteReg(0x047E, 0x01);   //atsc_dmd
        _MBX_WriteReg(0x04E6, 0xff);
        _MBX_ReadReg(0x047C, &u8PTK_LOOP_FF_R2);
        _MBX_ReadReg(0x047D, &u8PTK_LOOP_FF_R3);
        _MBX_WriteReg(0x047E, 0x00);
        _MBX_WriteReg(0x04E6, 0xff);
        #else
        _MBX_WriteReg(0x297E, 0x01);
        _MBX_WriteReg(0x29E6, 0xff);
        _MBX_ReadReg(0x297C, &u8PTK_LOOP_FF_R2);
        _MBX_ReadReg(0x297D, &u8PTK_LOOP_FF_R3);
        _MBX_WriteReg(0x297E, 0x00);
        _MBX_WriteReg(0x29E6, 0xff);
        #endif
        
        PTK_LOOP_FF = (u8PTK_LOOP_FF_R3<<8) | u8PTK_LOOP_FF_R2;
        #ifdef UTPA2
        *pFF = PTK_LOOP_FF;
        #else
        FreqOffset  = (float)(-PTK_LOOP_FF*0.04768);
        #endif
        
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
            || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
        _MBX_ReadReg(0x1782, &u8PTK_RATE_2);
        #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
        _MBX_ReadReg(0x0482, &u8PTK_RATE_2);   //atsc_dmd
        #else
        _MBX_ReadReg(0x2982, &u8PTK_RATE_2);
        #endif
        
        #ifdef UTPA2
        *pRate = u8PTK_RATE_2;
        #else
        if (u8PTK_RATE_2 == 0x07)
            FreqOffset = FreqOffset-100;
        else if (u8PTK_RATE_2 == 0x08)
            FreqOffset = FreqOffset-500;
        #endif
    }
    else //QAM MODE
    {
        #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
        _MBX_ReadReg(0x2A40, &u8AD_CRL_LOOP_VALUE0);
        _MBX_ReadReg(0x2A41, &u8AD_CRL_LOOP_VALUE1);
        _MBX_ReadReg(0x2758, &u8MIX_RATE_0);
        _MBX_ReadReg(0x2759, &u8MIX_RATE_1);
        _MBX_ReadReg(0x275A, &u8MIX_RATE_2);
        
        AD_CRL_LOOP_VALUE = (u8AD_CRL_LOOP_VALUE1 << 8) | u8AD_CRL_LOOP_VALUE0;
        MIX_RATE = ((u8MIX_RATE_2 << 16) | (u8MIX_RATE_1 << 8) | u8MIX_RATE_0) >> 4;
        
        #ifdef UTPA2
        *pMode |= 0x80;
        *pFF = AD_CRL_LOOP_VALUE;
        *pRate = MIX_RATE;
        #else
        if (eMode == DMD_ATSC_DEMOD_ATSC_256QAM) //256QAM//
        {
            FreqOffset = (float)(AD_CRL_LOOP_VALUE*0.0000199); //5.360537E6/2^28*1000
        }
        else if (eMode == DMD_ATSC_DEMOD_ATSC_64QAM)//64QAM//
        {
            FreqOffset = (float)(AD_CRL_LOOP_VALUE*0.0000188); //5.056941E6/2^21*1000
        }
        
        FreqOffset = FreqOffset+(float)(MIX_RATE-0x3A07)/330.13018; //(0.001/25.41*2^27/16)???
        #endif
        #else // #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
            || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
        _MBX_ReadReg(0x1A04, &u8AD_CRL_LOOP_VALUE0);
        _MBX_ReadReg(0x1A05, &u8AD_CRL_LOOP_VALUE1);
        #else
        _MBX_ReadReg(0x2C04, &u8AD_CRL_LOOP_VALUE0);
        _MBX_ReadReg(0x2C05, &u8AD_CRL_LOOP_VALUE1);
        #endif
        
        AD_CRL_LOOP_VALUE = (u8AD_CRL_LOOP_VALUE1<<8) | u8AD_CRL_LOOP_VALUE0;
        
        #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MASERATI || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MACAN \
            || DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MAXIM)
        _MBX_ReadReg(0x1704, &u8MIX_RATE_0);
        _MBX_ReadReg(0x1705, &u8MIX_RATE_1);
        _MBX_ReadReg(0x1706, &u8MIX_RATE_2);
        #elif (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_MUSTANG)
        _MBX_ReadReg(0x0404, &u8MIX_RATE_0);  //atsc_dmd
        _MBX_ReadReg(0x0405, &u8MIX_RATE_1);
        _MBX_ReadReg(0x0406, &u8MIX_RATE_2);
        #else
        _MBX_ReadReg(0x2904, &u8MIX_RATE_0);
        _MBX_ReadReg(0x2905, &u8MIX_RATE_1);
        _MBX_ReadReg(0x2906, &u8MIX_RATE_2);
        #endif
        
        MIX_RATE = (u8MIX_RATE_2<<12)|(u8MIX_RATE_1<<4)|(u8MIX_RATE_0>>4);
        
        #ifdef UTPA2
        *pFF = AD_CRL_LOOP_VALUE;
        *pRate = MIX_RATE;
        #else
        if (eMode == DMD_ATSC_DEMOD_ATSC_256QAM) //256QAM//
        {
            FreqOffset = (float)(AD_CRL_LOOP_VALUE*0.0025561); //5.360537E6/2^21*1000
        }
        else if (eMode == DMD_ATSC_DEMOD_ATSC_64QAM)//64QAM//
        {
            FreqOffset = (float)(AD_CRL_LOOP_VALUE*0.00241134); //5.056941E6/2^21*1000
        }
        
        FreqOffset = FreqOffset+(float)(MIX_RATE-0x3D70)/2.62144; //(0.001/25*2^20/16)
        #endif
        #endif // #if (DMD_ATSC_CHIP_VERSION >= DMD_ATSC_CHIP_K3)
    }
    
    #ifdef UTPA2
    return TRUE;
    #else
    return FreqOffset;
    #endif
}

static MS_BOOL _HAL_INTERN_ATSC_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return _MBX_ReadReg(u16Addr, pu8Data);
}

static MS_BOOL _HAL_INTERN_ATSC_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return _MBX_WriteReg(u16Addr, u8Data);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_INTERN_ATSC_IOCTL_CMD(DMD_ATSC_HAL_COMMAND eCmd, void *pArgs)
{
    MS_BOOL bResult = TRUE;
    
    #if (DMD_ATSC_CHIP_VERSION == DMD_ATSC_CHIP_K3)
    _SEL_DMD();
    #endif
    
    switch(eCmd)
    {
    case DMD_ATSC_HAL_CMD_Exit:
        bResult = _HAL_INTERN_ATSC_Exit();
        break;
    case DMD_ATSC_HAL_CMD_InitClk:
        _HAL_INTERN_ATSC_InitClk(false);
        break;
    case DMD_ATSC_HAL_CMD_Download:
        bResult = _HAL_INTERN_ATSC_Download();
        break;
    case DMD_ATSC_HAL_CMD_FWVERSION:
        _HAL_INTERN_ATSC_FWVERSION();
        break;
    case DMD_ATSC_HAL_CMD_SoftReset:
        bResult = _HAL_INTERN_ATSC_SoftReset();
        break;
    case DMD_ATSC_HAL_CMD_SetVsbMode:
        bResult = _HAL_INTERN_ATSC_SetVsbMode();
        break;
    case DMD_ATSC_HAL_CMD_Set64QamMode:
        bResult = _HAL_INTERN_ATSC_Set64QamMode();
        break;
    case DMD_ATSC_HAL_CMD_Set256QamMode:
        bResult = _HAL_INTERN_ATSC_Set256QamMode();
        break;
    case DMD_ATSC_HAL_CMD_SetModeClean:
        bResult = _HAL_INTERN_ATSC_SetModeClean();
        break;
    case DMD_ATSC_HAL_CMD_Set_QAM_SR:
        break;
    case DMD_ATSC_HAL_CMD_Active:
        break;
    case DMD_ATSC_HAL_CMD_Check8VSB64_256QAM:
        *((DMD_ATSC_DEMOD_TYPE *)pArgs) = _HAL_INTERN_ATSC_Check8VSB64_256QAM();
        break;
    case DMD_ATSC_HAL_CMD_AGCLock:
        bResult = _HAL_INTERN_ATSC_Vsb_QAM_AGCLock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_PreLock:
        bResult = _HAL_INTERN_ATSC_Vsb_PreLock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_FSync_Lock:
        bResult = _HAL_INTERN_ATSC_Vsb_FSync_Lock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_CE_Lock:
        bResult = _HAL_INTERN_ATSC_Vsb_CE_Lock();
        break;
    case DMD_ATSC_HAL_CMD_Vsb_FEC_Lock:
        bResult = _HAL_INTERN_ATSC_Vsb_FEC_Lock();
        break;
    case DMD_ATSC_HAL_CMD_QAM_PreLock:
        bResult = _HAL_INTERN_ATSC_QAM_PreLock();
        break;
    case DMD_ATSC_HAL_CMD_QAM_Main_Lock:
        bResult = _HAL_INTERN_ATSC_QAM_Main_Lock();
        break;
    case DMD_ATSC_HAL_CMD_ReadIFAGC:
        *((MS_U16 *)pArgs) = _HAL_INTERN_ATSC_ReadIFAGC();
        break;
    case DMD_ATSC_HAL_CMD_CheckSignalCondition:
        _HAL_INTERN_ATSC_CheckSignalCondition((DMD_ATSC_SIGNAL_CONDITION *)pArgs);
        break;
    case DMD_ATSC_HAL_CMD_ReadSNRPercentage:
        *((MS_U8 *)pArgs) = _HAL_INTERN_ATSC_ReadSNRPercentage();
        break;
    case DMD_ATSC_HAL_CMD_GET_QAM_SNR:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ATSC_GET_QAM_SNR(&((*((DMD_ATSC_SNR_DATA *)pArgs)).noisepower), &((*((DMD_ATSC_SNR_DATA *)pArgs)).sym_num));
        #else
        bResult = _HAL_INTERN_ATSC_GET_QAM_SNR((float *)pArgs);
        #endif
        break;
    case DMD_ATSC_HAL_CMD_ReadPKTERR:
        *((MS_U16 *)pArgs) = _HAL_INTERN_ATSC_ReadPKTERR();
        break;
    case DMD_ATSC_HAL_CMD_GetPreViterbiBer:
        break;
    case DMD_ATSC_HAL_CMD_GetPostViterbiBer:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ATSC_ReadBER(&((*((DMD_ATSC_BER_DATA *)pArgs)).BitErr), &((*((DMD_ATSC_BER_DATA *)pArgs)).Error_window), &((*((DMD_ATSC_BER_DATA *)pArgs)).Win_unit));
        #else
        bResult = _HAL_INTERN_ATSC_ReadBER((float *)pArgs);
        #endif
        break;
    case DMD_ATSC_HAL_CMD_ReadFrequencyOffset:
        #ifdef UTPA2
        bResult = _HAL_INTERN_ATSC_ReadFrequencyOffset(&((*((DMD_ATSC_CFO_DATA *)pArgs)).Mode), &((*((DMD_ATSC_CFO_DATA *)pArgs)).FF), &((*((DMD_ATSC_CFO_DATA *)pArgs)).Rate));
        #else
        *((MS_S16 *)pArgs) = _HAL_INTERN_ATSC_ReadFrequencyOffset();
        #endif
        break;
    case DMD_ATSC_HAL_CMD_TS_INTERFACE_CONFIG:
        break;
    case DMD_ATSC_HAL_CMD_IIC_Bypass_Mode:
        break;
    case DMD_ATSC_HAL_CMD_SSPI_TO_GPIO:
        break;
    case DMD_ATSC_HAL_CMD_GPIO_GET_LEVEL:
        break;
    case DMD_ATSC_HAL_CMD_GPIO_SET_LEVEL:
        break;
    case DMD_ATSC_HAL_CMD_GPIO_OUT_ENABLE:
        break;
    case DMD_ATSC_HAL_CMD_GET_REG:
        bResult = _HAL_INTERN_ATSC_GetReg((*((DMD_ATSC_REG_DATA *)pArgs)).u16Addr, &((*((DMD_ATSC_REG_DATA *)pArgs)).u8Data));
        break;
    case DMD_ATSC_HAL_CMD_SET_REG:
        bResult = _HAL_INTERN_ATSC_SetReg((*((DMD_ATSC_REG_DATA *)pArgs)).u16Addr, (*((DMD_ATSC_REG_DATA *)pArgs)).u8Data);
        break;
    default:
        break;
    }
    
    return bResult;
}

MS_BOOL MDrv_DMD_ATSC_Initial_Hal_Interface(void)
{
    return TRUE;
}
