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

#include "drvDMD_DTMB.h"

#include "MsTypes.h"
#if DMD_DTMB_UTOPIA_EN || DMD_DTMB_UTOPIA2_EN
#include "drvDMD_common.h"
#include "halDMD_INTERN_common.h"
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define DMD_DTMB_CHIP_NIKON         0x00
#define DMD_DTMB_CHIP_NASA          0x01
#define DMD_DTMB_CHIP_MADISON       0x02
#define DMD_DTMB_CHIP_MONACO        0x03
#define DMD_DTMB_CHIP_MUJI          0x04
#define DMD_DTMB_CHIP_MONET         0x05
#define DMD_DTMB_CHIP_MANHATTAN     0x06
#define DMD_DTMB_CHIP_MESSI         0x07
#define DMD_DTMB_CHIP_MASERATI      0x08
#define DMD_DTMB_CHIP_MACAN         0x09
#define DMD_DTMB_CHIP_MOONEY        0x0A

#if defined(nikon)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_NIKON
#elif defined(nasa)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_NASA
#elif defined(madison)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MADISON
#elif defined(monaco)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MONACO
#elif defined(muji)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MUJI
#elif defined(monet)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MONET
#elif defined(manhattan)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MANHATTAN
#elif defined(messi)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MESSI
#elif defined(maserati)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MASERATI
#elif defined(macan)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MACAN
#elif defined(mooney)
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_MOONEY
#else
 #define DMD_DTMB_CHIP_VERSION      DMD_DTMB_CHIP_NIKON
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if 0
#define _RIU_READ_BYTE(addr)        ( READ_BYTE(psDMD_DTMB_ResData->sDMD_DTMB_PriData.virtDMDBaseAddr + (addr) ) )
#define _RIU_WRITE_BYTE(addr, val)  ( WRITE_BYTE(psDMD_DTMB_ResData->sDMD_DTMB_PriData.virtDMDBaseAddr + (addr), val) )
#endif
#define HAL_INTERN_DTMB_DBINFO(y)   //y

//#define MBRegBase                   0x112600
//#define DMDMcuBase                  0x103480

#define DTMB_REG_BASE        0x2600

#define DTMB_ACI_COEF_SIZE       112

#define DMD_DTMB_CHIP_ID_NASA       0x6E
#define DMD_DTMB_CHIP_ID_WALTZ      0x9C

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

const MS_U8 INTERN_DTMB_table[] = {
    #include "DMD_INTERN_DTMB.dat"
};

const MS_U8 INTERN_DTMB_6M_table[] = {
    #include "DMD_INTERN_DTMB_6M.dat"
};

#if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
const MS_U8 INTERN_DTMB_table_Waltz[] = {
    #include "DMD_INTERN_DTMB_Waltz.dat"
};

const MS_U8 INTERN_DTMB_6M_table_Waltz[] = {
    #include "DMD_INTERN_DTMB_6M_Waltz.dat"
};
#endif

static MS_U8 _ACI_COEF_TABLE_FS24M_SR8M[DTMB_ACI_COEF_SIZE] = {
  0x80, 0x06, 0x9f, 0xf4, 0x9f, 0xe8, 0x9f, 0xf0, 0x80, 0x09, 0x80, 0x1f, 0x80, 0x1d, 0x80, 0x03, 0x9f, 0xe3, 0x9f, 0xdc, 0x9f, 0xf7, 0x80, 0x1d, 0x80, 0x2c, 0x80, 0x12, 0x9f, 0xe2,
  0x9f, 0xc9, 0x9f, 0xe2, 0x80, 0x1a, 0x80, 0x42, 0x80, 0x2f, 0x9f, 0xeb, 0x9f, 0xb2, 0x9f, 0xbe, 0x80, 0x0c, 0x80, 0x5b, 0x80, 0x5e, 0x80, 0x05, 0x9f, 0x9a, 0x9f, 0x81, 0x9f, 0xdf,
  0x80, 0x6c, 0x80, 0xa7, 0x80, 0x45, 0x9f, 0x8c, 0x9f, 0x24, 0x9f, 0x84, 0x80, 0x7d, 0x81, 0x38, 0x80, 0xe3, 0x9f, 0x7b, 0x9e, 0x0e, 0x9e, 0x1f, 0x80, 0x87, 0x84, 0xa6, 0x88, 0x8c,
  0x8a, 0x25, 0x80, 0x08, 0x80, 0x0b, 0x80, 0x0b, 0x80, 0x01, 0x9f, 0xee, 0x9f, 0xdf, 0x9f, 0xdb, 0x9f, 0xe8, 0x9f, 0xfd, 0x80, 0x0a};

static MS_U8 _ACI_COEF_TABLE_FS24M_SR6M[DTMB_ACI_COEF_SIZE] = {
  0x9F, 0xF1, 0x9F, 0xFB, 0x80, 0x09, 0x80, 0x15, 0x80, 0x17, 0x80, 0x0D, 0x9F, 0xFB, 0x9F, 0xE9, 0x9F, 0xE2, 0x9F, 0xEC, 0x80, 0x04, 0x80, 0x1D, 0x80, 0x27, 0x80, 0x19, 0x9F, 0xFA,
  0x9F, 0xD9, 0x9F, 0xCE, 0x9F, 0xE1, 0x80, 0x0C, 0x80, 0x35, 0x80, 0x42, 0x80, 0x24, 0x9F, 0xEA, 0x9F, 0xB6, 0x9F, 0xAA, 0x9F, 0xD6, 0x80, 0x26, 0x80, 0x6A, 0x80, 0x72, 0x80, 0x2E,
  0x9F, 0xBF, 0x9F, 0x66, 0x9F, 0x65, 0x9F, 0xCE, 0x80, 0x71, 0x80, 0xED, 0x80, 0xE2, 0x80, 0x35, 0x9F, 0x2B, 0x9E, 0x5C, 0x9E, 0x72, 0x9F, 0xCA, 0x82, 0x3B, 0x85, 0x13, 0x87, 0x59,
  0x88, 0x38, 0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x80, 0x02, 0x80, 0x02, 0x80, 0x00, 0x9F, 0xFC, 0x9F, 0xF6, 0x9F, 0xF0, 0x9F, 0xED};

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

extern MS_U8 u8DMD_DTMB_DMD_ID;

extern DMD_DTMB_ResData *psDMD_DTMB_ResData;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
#if 0
static MS_U8 _HAL_DMD_RIU_ReadByte(MS_U32 u32Addr)
{
    return _RIU_READ_BYTE(((u32Addr) << 1) - ((u32Addr) & 1));
}

static void _HAL_DMD_RIU_WriteByte(MS_U32 u32Addr, MS_U8 u8Value)
{
    _RIU_WRITE_BYTE(((u32Addr) << 1) - ((u32Addr) & 1), u8Value);
}
#endif
//static void _HAL_DMD_RIU_WriteByteMask(MS_U32 u32Addr, MS_U8 u8Value, MS_U8 u8Mask)
//{
//    _RIU_WRITE_BYTE((((u32Addr) <<1) - ((u32Addr) & 1)), (_RIU_READ_BYTE((((u32Addr) <<1) - ((u32Addr) & 1))) & ~(u8Mask)) | ((u8Value) & (u8Mask)));
//}

static MS_BOOL _MBX_WriteReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag;

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, (u16Addr&0xff));
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x01, (u16Addr>>8));
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x10, u8Data);
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x1E, 0x01);

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51

    for (u8CheckCount=0; u8CheckCount < 10; u8CheckCount++)
    {
        u8CheckFlag = HAL_DMD_RIU_ReadByte(MBRegBase + 0x1E);
        if ((u8CheckFlag&0x01)==0)
            break;
        MsOS_DelayTask(1);
    }

    if (u8CheckFlag&0x01)
    {
        printf("ERROR: DTMB INTERN DEMOD MBX WRITE TIME OUT!\n");
        return FALSE;
    }

    return TRUE;
}

static MS_BOOL _MBX_ReadReg(MS_U16 u16Addr, MS_U8 *u8Data)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag;

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x00, (u16Addr&0xff));
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x01, (u16Addr>>8));
    HAL_DMD_RIU_WriteByte(MBRegBase + 0x1E, 0x02);

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51

    for (u8CheckCount=0; u8CheckCount < 10; u8CheckCount++)
    {
        u8CheckFlag = HAL_DMD_RIU_ReadByte(MBRegBase + 0x1E);
        if ((u8CheckFlag&0x02)==0)
        {
            *u8Data = HAL_DMD_RIU_ReadByte(MBRegBase + 0x10);
            break;
        }
        MsOS_DelayTask(1);
    }

    if (u8CheckFlag&0x02)
    {
        printf("ERROR: DTMB INTERN DEMOD MBX READ TIME OUT!\n");
        return FALSE;
    }

    return TRUE;
}

#if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NIKON)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_NIKON--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x14);
   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);
   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4f, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f29, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f28, 0x0c);
   //HAL_DMD_RIU_WriteByte(0x111f28, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f2d, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f2c, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f2f, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f2e, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f35, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f34, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3d, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3c, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f43, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f42, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f44, 0xc4);
   HAL_DMD_RIU_WriteByte(0x111f47, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f46, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f48, 0x04);

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;

   MS_U8 u8Val = 0;

   if (pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
   {
       printf("--------------DMD_DTMB_CHIP_WALTZ--------------\n");

       u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
       HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

       HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
       HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

       HAL_DMD_RIU_WriteByte(0x103301, 0x07);
       HAL_DMD_RIU_WriteByte(0x103300, 0x11);

       HAL_DMD_RIU_WriteByte(0x103309, 0x00);
       HAL_DMD_RIU_WriteByte(0x103308, 0x00);
       HAL_DMD_RIU_WriteByte(0x103315, 0x00);
       HAL_DMD_RIU_WriteByte(0x103314, 0x00);

       HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

       HAL_DMD_RIU_WriteByte(0x111f49, 0xcc);
       HAL_DMD_RIU_WriteByte(0x111f48, 0x11);

       HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
       HAL_DMD_RIU_WriteByte(0x111f22, 0x14);
       HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f72, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f75, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f74, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f77, 0x0c);
       HAL_DMD_RIU_WriteByte(0x111f76, 0x0c);

       HAL_DMD_RIU_WriteByte(0x111f61, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f60, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f62, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f65, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f64, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f68, 0x00);

       HAL_DMD_RIU_WriteByte(0x111f6B, 0x44);
       HAL_DMD_RIU_WriteByte(0x111f6A, 0x44);
       HAL_DMD_RIU_WriteByte(0x111f7a, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f7b, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f6D, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f6C, 0xC4);

       HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f70, 0x04);

       HAL_DMD_RIU_WriteByte(0x111f79, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f78, 0x00);

       HAL_DMD_RIU_WriteByte(0x111f51, 0x04);
       HAL_DMD_RIU_WriteByte(0x111f50, 0x00);

       u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
       _HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
   }
   else
   {
       printf("--------------DMD_DTMB_CHIP_NASA--------------\n");

       u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
       HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

       HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
       HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
       HAL_DMD_RIU_WriteByte(0x103301, 0x07);
       HAL_DMD_RIU_WriteByte(0x103300, 0x14);
       HAL_DMD_RIU_WriteByte(0x103309, 0x00);
       HAL_DMD_RIU_WriteByte(0x103308, 0x00);
       HAL_DMD_RIU_WriteByte(0x103315, 0x00);
       HAL_DMD_RIU_WriteByte(0x103314, 0x00);

       HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f4f, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f29, 0x0c);
       HAL_DMD_RIU_WriteByte(0x111f28, 0x0c);
        //_HAL_DMD_RIU_WriteByte(0x111f28, 0x0c);
       HAL_DMD_RIU_WriteByte(0x111f2d, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f2c, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f2f, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f2e, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f35, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f34, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f3b, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f3d, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f3c, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f43, 0x44);
       HAL_DMD_RIU_WriteByte(0x111f42, 0x44);
       HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f44, 0xc4);
       HAL_DMD_RIU_WriteByte(0x111f47, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f46, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
       HAL_DMD_RIU_WriteByte(0x111f48, 0x04);

       u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
       HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
    }
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MADISON--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);
   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x14);
   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);
   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f15, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f14, 0x01);
   HAL_DMD_RIU_WriteByte(0x111f17, 0x01);
   HAL_DMD_RIU_WriteByte(0x111f16, 0x01);

   HAL_DMD_RIU_WriteByte(0x111f23, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4a, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4f, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f4e, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f29, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f28, 0x0c);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x04);

   //HAL_DMD_RIU_WriteByte(0x111f28, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f2d, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f2c, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f2f, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f2e, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f35, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f34, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3d, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3c, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f43, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f42, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f45, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f44, 0xc4);
   HAL_DMD_RIU_WriteByte(0x111f47, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f46, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f48, 0x04);

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MONACO--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x11);

   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);
   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f48, 0x11);
   HAL_DMD_RIU_WriteByte(0x111f49, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x14);
   HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f72, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f77, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f76, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f61, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f60, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f62, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f65, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f64, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f68, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f6B, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f6A, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f6D, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f6C, 0xC4);

   HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f70, 0x04);

   HAL_DMD_RIU_WriteByte(0x111f78, 0x00);

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MUJI--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x11);

   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);
   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);
   HAL_DMD_RIU_WriteByte(0x103302, 0x01); //MUJI add
   HAL_DMD_RIU_WriteByte(0x103302, 0x00); //MUJI add

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f48, 0x11);
   HAL_DMD_RIU_WriteByte(0x111f49, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x14);
   HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f72, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f77, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f76, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f61, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f60, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f62, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f65, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f64, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f68, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f6B, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f6A, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f6D, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f6C, 0xC4);

   HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f70, 0x04);

   HAL_DMD_RIU_WriteByte(0x111f78, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f51, 0x04); //MUJI add
   HAL_DMD_RIU_WriteByte(0x111f50, 0x00); //MuJI add

   HAL_DMD_RIU_WriteByte(0x112091, 0x2f); //SRAM power saving
   HAL_DMD_RIU_WriteByte(0x112090, 0x00); //SRAM power saving

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONET)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MONET--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x11);

   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);
   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);
   HAL_DMD_RIU_WriteByte(0x103302, 0x01);
   HAL_DMD_RIU_WriteByte(0x103302, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f49, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f48, 0x11);

   HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x14);
   HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f72, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f75, 0x00);  //monet add
   HAL_DMD_RIU_WriteByte(0x111f74, 0x00);  //monet add

   HAL_DMD_RIU_WriteByte(0x111f77, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f76, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f61, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f60, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f62, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f65, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f64, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f68, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f6B, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f6A, 0x44);

   HAL_DMD_RIU_WriteByte(0x111f7B, 0x00);  //monet add
   HAL_DMD_RIU_WriteByte(0x111f7A, 0x00);  //monet add


   HAL_DMD_RIU_WriteByte(0x111f6D, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f6C, 0xC4);

   HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f70, 0x04);

   HAL_DMD_RIU_WriteByte(0x111f79, 0x00);  //moent add
   HAL_DMD_RIU_WriteByte(0x111f78, 0x00);  //monet add

   HAL_DMD_RIU_WriteByte(0x111f51, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f50, 0x00);

   // HAL_DMD_RIU_WriteByte(0x112091, 0x10); //SRAM power saving
   // HAL_DMD_RIU_WriteByte(0x112090, 0x00); //SRAM power saving

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MANHATTAN)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MANHATTAN--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x11);

   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);
   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);
   HAL_DMD_RIU_WriteByte(0x103302, 0x01);
   HAL_DMD_RIU_WriteByte(0x103302, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f49, 0x00);//0xcc?
   HAL_DMD_RIU_WriteByte(0x111f48, 0x11);

   HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f22, 0x14);
   HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f72, 0x00);

  // HAL_DMD_RIU_WriteByte(0x111f75, 0x00);  //monet add
  // HAL_DMD_RIU_WriteByte(0x111f74, 0x00);  //monet add

   HAL_DMD_RIU_WriteByte(0x111f77, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f76, 0x0c);
   HAL_DMD_RIU_WriteByte(0x111f61, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f60, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f62, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f65, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f64, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f68, 0x00);

   //carl
   HAL_DMD_RIU_WriteByte(0x111f6B, 0x44);
   HAL_DMD_RIU_WriteByte(0x111f6A, 0x44);

 //  HAL_DMD_RIU_WriteByte(0x111f7B, 0x00);  //monet add
   HAL_DMD_RIU_WriteByte(0x111f7A, 0x00);  //monet add


   HAL_DMD_RIU_WriteByte(0x111f6D, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f6C, 0xC4);

   HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f70, 0x04);

   HAL_DMD_RIU_WriteByte(0x111f79, 0x00);  //moent add
   HAL_DMD_RIU_WriteByte(0x111f78, 0x00);  //monet add

   HAL_DMD_RIU_WriteByte(0x111f51, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f50, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f81, 0x88);// manhattan adds
   HAL_DMD_RIU_WriteByte(0x111f80, 0x88);// manhattan adds

   HAL_DMD_RIU_WriteByte(0x111f83, 0xc8);// manhattan adds
   HAL_DMD_RIU_WriteByte(0x111f82, 0x88);// manhattan adds

   HAL_DMD_RIU_WriteByte(0x111f85, 0x88);// manhattan adds
   HAL_DMD_RIU_WriteByte(0x111f84, 0x88);// manhattan adds

   HAL_DMD_RIU_WriteByte(0x111f87, 0x08);// manhattan adds
   HAL_DMD_RIU_WriteByte(0x111f86, 0x88);// manhattan adds

   HAL_DMD_RIU_WriteByte(0x111f89, 0x00);// manhattan adds
   HAL_DMD_RIU_WriteByte(0x111f88, 0x00);// manhattan adds

   HAL_DMD_RIU_WriteByte(0x111f8b, 0x00);// manhattan adds
   HAL_DMD_RIU_WriteByte(0x111f8a, 0x00);// manhattan adds


   // HAL_DMD_RIU_WriteByte(0x112091, 0x10); //SRAM power saving
   // HAL_DMD_RIU_WriteByte(0x112090, 0x00); //SRAM power saving

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MESSI)
static void _HAL_INTERN_DTMB_InitClk(void)
{
    MS_U8 u8Val = 0;

    printf("--------------DMD_DTMB_CHIP_MESSI--------------\n");

    u8Val = _HAL_DMD_RIU_ReadByte(0x101e39);
    HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

    HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
    HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

    HAL_DMD_RIU_WriteByte(0x103301, 0x07);
    HAL_DMD_RIU_WriteByte(0x103300, 0x11);

    HAL_DMD_RIU_WriteByte(0x103309, 0x00);
    HAL_DMD_RIU_WriteByte(0x103308, 0x00);
    HAL_DMD_RIU_WriteByte(0x103315, 0x00);
    HAL_DMD_RIU_WriteByte(0x103314, 0x00);
    HAL_DMD_RIU_WriteByte(0x103302, 0x01);
    HAL_DMD_RIU_WriteByte(0x103302, 0x00);

    HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

    HAL_DMD_RIU_WriteByte(0x111f49, 0x00);//0xcc?
    //_HAL_DMD_RIU_WriteByte(0x111f48, 0x11);
    HAL_DMD_RIU_WriteByte(0x111f48, 0x00);//MESSI only?

    HAL_DMD_RIU_WriteByte(0x111f23, 0x04);
    HAL_DMD_RIU_WriteByte(0x111f22, 0x14);
    HAL_DMD_RIU_WriteByte(0x111f73, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f72, 0x00);

   // _HAL_DMD_RIU_WriteByte(0x111f75, 0x00);  //monet add
   // _HAL_DMD_RIU_WriteByte(0x111f74, 0x00);  //monet add

    HAL_DMD_RIU_WriteByte(0x111f77, 0x0c);
    HAL_DMD_RIU_WriteByte(0x111f76, 0x0c);
    HAL_DMD_RIU_WriteByte(0x111f61, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f60, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f63, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f62, 0x00);

    HAL_DMD_RIU_WriteByte(0x111f65, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f64, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f69, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f68, 0x00);

    //carl
    HAL_DMD_RIU_WriteByte(0x111f6B, 0x44);
    HAL_DMD_RIU_WriteByte(0x111f6A, 0x44);

    //  HAL_DMD_RIU_WriteByte(0x111f7B, 0x00);  //monet add
    HAL_DMD_RIU_WriteByte(0x111f7A, 0x00);  //monet add


    HAL_DMD_RIU_WriteByte(0x111f6D, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f6C, 0xC4);

    HAL_DMD_RIU_WriteByte(0x111f71, 0x00);
    HAL_DMD_RIU_WriteByte(0x111f70, 0x04);

    HAL_DMD_RIU_WriteByte(0x111f79, 0x00);  //moent add
    HAL_DMD_RIU_WriteByte(0x111f78, 0x00);  //monet add

    HAL_DMD_RIU_WriteByte(0x111f51, 0x04);
    HAL_DMD_RIU_WriteByte(0x111f50, 0x00);

    HAL_DMD_RIU_WriteByte(0x111f81, 0x88);// manhattan adds
    HAL_DMD_RIU_WriteByte(0x111f80, 0x88);// manhattan adds

    HAL_DMD_RIU_WriteByte(0x111f83, 0xc8);// manhattan adds
    HAL_DMD_RIU_WriteByte(0x111f82, 0x88);// manhattan adds

    HAL_DMD_RIU_WriteByte(0x111f85, 0x88);// manhattan adds
    HAL_DMD_RIU_WriteByte(0x111f84, 0x88);// manhattan adds

    HAL_DMD_RIU_WriteByte(0x111f87, 0x08);// manhattan adds
    HAL_DMD_RIU_WriteByte(0x111f86, 0x88);// manhattan adds

    HAL_DMD_RIU_WriteByte(0x111f89, 0x00);// manhattan adds
    HAL_DMD_RIU_WriteByte(0x111f88, 0x00);// manhattan adds

    HAL_DMD_RIU_WriteByte(0x111f8b, 0x00);// manhattan adds
    HAL_DMD_RIU_WriteByte(0x111f8a, 0x00);// manhattan adds


    // HAL_DMD_RIU_WriteByte(0x112091, 0x10); //SRAM power saving
    // HAL_DMD_RIU_WriteByte(0x112090, 0x00); //SRAM power saving

    u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
    HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MASERATI_MACAN--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x1128d0, 0x01);

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x11);

   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);

   //HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   //HAL_DMD_RIU_WriteByte(0x103314, 0x00);

   HAL_DMD_RIU_WriteByte(0x103302, 0x01);
   HAL_DMD_RIU_WriteByte(0x103302, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);

   HAL_DMD_RIU_WriteByte(0x1128d0, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f3b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f69, 0xCC);
   HAL_DMD_RIU_WriteByte(0x111f68, 0x11);

   HAL_DMD_RIU_WriteByte(0x152923, 0x00);
   HAL_DMD_RIU_WriteByte(0x152922, 0x14);

   HAL_DMD_RIU_WriteByte(0x111f25, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f24, 0x00);

   HAL_DMD_RIU_WriteByte(0x152973, 0x00);
   HAL_DMD_RIU_WriteByte(0x152972, 0x00);

   HAL_DMD_RIU_WriteByte(0x152975, 0x00);
   HAL_DMD_RIU_WriteByte(0x152974, 0x00);

   HAL_DMD_RIU_WriteByte(0x152977, 0x0c);
   HAL_DMD_RIU_WriteByte(0x152976, 0x0c);

   HAL_DMD_RIU_WriteByte(0x152961, 0x00);
   HAL_DMD_RIU_WriteByte(0x152960, 0x00);

   HAL_DMD_RIU_WriteByte(0x152963, 0x00);
   HAL_DMD_RIU_WriteByte(0x152962, 0x00);

   HAL_DMD_RIU_WriteByte(0x152965, 0x00);
   HAL_DMD_RIU_WriteByte(0x152964, 0x00);

   HAL_DMD_RIU_WriteByte(0x152969, 0x00);
   HAL_DMD_RIU_WriteByte(0x152968, 0x00);

   HAL_DMD_RIU_WriteByte(0x15296B, 0x44);
   HAL_DMD_RIU_WriteByte(0x15296A, 0x44);

   HAL_DMD_RIU_WriteByte(0x15297a, 0x00);

   HAL_DMD_RIU_WriteByte(0x15296d, 0x00);
   HAL_DMD_RIU_WriteByte(0x15296c, 0xc4);

   HAL_DMD_RIU_WriteByte(0x152971, 0x00);
   HAL_DMD_RIU_WriteByte(0x152970, 0x04);

   HAL_DMD_RIU_WriteByte(0x152979, 0x00);
   HAL_DMD_RIU_WriteByte(0x152978, 0x00);

   HAL_DMD_RIU_WriteByte(0x152951, 0x04);
   HAL_DMD_RIU_WriteByte(0x152950, 0x00);

   HAL_DMD_RIU_WriteByte(0x152981, 0x88);
   HAL_DMD_RIU_WriteByte(0x152980, 0x88);

   HAL_DMD_RIU_WriteByte(0x152983, 0xc8);
   HAL_DMD_RIU_WriteByte(0x152982, 0x88);

   HAL_DMD_RIU_WriteByte(0x152985, 0x88);
   HAL_DMD_RIU_WriteByte(0x152984, 0x88);

   HAL_DMD_RIU_WriteByte(0x152987, 0x08);
   HAL_DMD_RIU_WriteByte(0x152986, 0x8c);

   HAL_DMD_RIU_WriteByte(0x111f75, 0x81);
   HAL_DMD_RIU_WriteByte(0x111f74, 0x81);

   HAL_DMD_RIU_WriteByte(0x111f77, 0x81);
   HAL_DMD_RIU_WriteByte(0x111f76, 0x88);

   HAL_DMD_RIU_WriteByte(0x15298d, 0x44);
   HAL_DMD_RIU_WriteByte(0x15298c, 0x00);

   HAL_DMD_RIU_WriteByte(0x15298f, 0x88);
   HAL_DMD_RIU_WriteByte(0x15298e, 0x88);

   HAL_DMD_RIU_WriteByte(0x152991, 0xc8);
   HAL_DMD_RIU_WriteByte(0x152990, 0x88);

   HAL_DMD_RIU_WriteByte(0x152993, 0x11);
   HAL_DMD_RIU_WriteByte(0x152992, 0x18);

   HAL_DMD_RIU_WriteByte(0x111f7b, 0x18);
   HAL_DMD_RIU_WriteByte(0x111f7a, 0x11);

   HAL_DMD_RIU_WriteByte(0x111f79, 0x11);
   HAL_DMD_RIU_WriteByte(0x111f78, 0x88);

   HAL_DMD_RIU_WriteByte(0x111f7d, 0x18);
   HAL_DMD_RIU_WriteByte(0x111f7c, 0x11);

   HAL_DMD_RIU_WriteByte(0x111f89, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f88, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f8b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f8a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f31, 0x18);

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MOONEY)
static void _HAL_INTERN_DTMB_InitClk(void)
{
   MS_U8 u8Val = 0;

   printf("--------------DMD_DTMB_CHIP_MOONEY--------------\n");

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val&(~0x03));

   HAL_DMD_RIU_WriteByte(0x10331f, 0x00);
   HAL_DMD_RIU_WriteByte(0x10331e, 0x10);

   HAL_DMD_RIU_WriteByte(0x103301, 0x07);
   HAL_DMD_RIU_WriteByte(0x103300, 0x11);

   HAL_DMD_RIU_WriteByte(0x103309, 0x00);
   HAL_DMD_RIU_WriteByte(0x103308, 0x00);

   HAL_DMD_RIU_WriteByte(0x103315, 0x00);
   HAL_DMD_RIU_WriteByte(0x103314, 0x00);

   HAL_DMD_RIU_WriteByte(0x103302, 0x01);
   HAL_DMD_RIU_WriteByte(0x103302, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f0b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f0a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f3b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f3a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f69, 0xCC);
   HAL_DMD_RIU_WriteByte(0x111f68, 0x11);

   HAL_DMD_RIU_WriteByte(0x152923, 0x00);
   HAL_DMD_RIU_WriteByte(0x152922, 0x14);

   HAL_DMD_RIU_WriteByte(0x111f25, 0x04);
   HAL_DMD_RIU_WriteByte(0x111f24, 0x00);

   HAL_DMD_RIU_WriteByte(0x152973, 0x00);
   HAL_DMD_RIU_WriteByte(0x152972, 0x00);

   HAL_DMD_RIU_WriteByte(0x152975, 0x00);
   HAL_DMD_RIU_WriteByte(0x152974, 0x00);

   HAL_DMD_RIU_WriteByte(0x152977, 0x0c);
   HAL_DMD_RIU_WriteByte(0x152976, 0x0c);

   HAL_DMD_RIU_WriteByte(0x152961, 0x00);
   HAL_DMD_RIU_WriteByte(0x152960, 0x00);

   HAL_DMD_RIU_WriteByte(0x152963, 0x00);
   HAL_DMD_RIU_WriteByte(0x152962, 0x00);

   HAL_DMD_RIU_WriteByte(0x152965, 0x00);
   HAL_DMD_RIU_WriteByte(0x152964, 0x00);

   HAL_DMD_RIU_WriteByte(0x152969, 0x00);
   HAL_DMD_RIU_WriteByte(0x152968, 0x00);

   HAL_DMD_RIU_WriteByte(0x15296B, 0x44);
   HAL_DMD_RIU_WriteByte(0x15296A, 0x44);

   HAL_DMD_RIU_WriteByte(0x15296d, 0x00);
   HAL_DMD_RIU_WriteByte(0x15296c, 0xc4);

   HAL_DMD_RIU_WriteByte(0x152971, 0x00);
   HAL_DMD_RIU_WriteByte(0x152970, 0x04);

   HAL_DMD_RIU_WriteByte(0x152979, 0x00);
   HAL_DMD_RIU_WriteByte(0x152978, 0x00);

   HAL_DMD_RIU_WriteByte(0x152951, 0x04);
   HAL_DMD_RIU_WriteByte(0x152950, 0x00);

   HAL_DMD_RIU_WriteByte(0x152981, 0x88);
   HAL_DMD_RIU_WriteByte(0x152980, 0x88);

   HAL_DMD_RIU_WriteByte(0x152983, 0xc8);
   HAL_DMD_RIU_WriteByte(0x152982, 0x88);

   HAL_DMD_RIU_WriteByte(0x152985, 0x88);
   HAL_DMD_RIU_WriteByte(0x152984, 0x88);

   HAL_DMD_RIU_WriteByte(0x152987, 0x08);
   HAL_DMD_RIU_WriteByte(0x152986, 0x8c);

   HAL_DMD_RIU_WriteByte(0x111f75, 0x81);
   HAL_DMD_RIU_WriteByte(0x111f74, 0x81);

   HAL_DMD_RIU_WriteByte(0x111f77, 0x81);
   HAL_DMD_RIU_WriteByte(0x111f76, 0x88);

   HAL_DMD_RIU_WriteByte(0x15298d, 0x44);
   HAL_DMD_RIU_WriteByte(0x15298c, 0x00);

   HAL_DMD_RIU_WriteByte(0x15298f, 0x88);
   HAL_DMD_RIU_WriteByte(0x15298e, 0x88);

   HAL_DMD_RIU_WriteByte(0x152991, 0xc8);
   HAL_DMD_RIU_WriteByte(0x152990, 0x88);

   HAL_DMD_RIU_WriteByte(0x152993, 0x11);
   HAL_DMD_RIU_WriteByte(0x152992, 0x18);

   HAL_DMD_RIU_WriteByte(0x111f7b, 0x18);
   HAL_DMD_RIU_WriteByte(0x111f7a, 0x11);

   HAL_DMD_RIU_WriteByte(0x111f79, 0x11);
   HAL_DMD_RIU_WriteByte(0x111f78, 0x88);

   HAL_DMD_RIU_WriteByte(0x111f7d, 0x18);
   HAL_DMD_RIU_WriteByte(0x111f7c, 0x11);


   HAL_DMD_RIU_WriteByte(0x111fe3, 0x08);
   HAL_DMD_RIU_WriteByte(0x111fe2, 0x11);

   HAL_DMD_RIU_WriteByte(0x111ff0, 0x08);

   HAL_DMD_RIU_WriteByte(0x111f89, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f88, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f8b, 0x00);
   HAL_DMD_RIU_WriteByte(0x111f8a, 0x00);

   HAL_DMD_RIU_WriteByte(0x111f31, 0x18);

   u8Val = HAL_DMD_RIU_ReadByte(0x101e39);
   HAL_DMD_RIU_WriteByte(0x101e39, u8Val|0x03);
}
#else
static void _HAL_INTERN_DTMB_InitClk(void)
{
    printf("--------------DMD_DTMB_CHIP_NONE--------------\n");
}
#endif

static MS_BOOL _HAL_INTERN_DTMB_Ready(void)
{
    MS_U8 udata = 0x00;

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x1E, 0x02);

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51

    MsOS_DelayTask(1);

    udata = HAL_DMD_RIU_ReadByte(MBRegBase + 0x1E);

    if (udata) return FALSE;

    return TRUE;
}

static MS_BOOL _HAL_INTERN_DTMB_Download(void)
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;
    MS_U8  udata = 0x00;
    MS_U16 i = 0;
    MS_U16 fail_cnt = 0;
    MS_U8  u8TmpData;
    MS_U16 u16AddressOffset;
    const MS_U8 *DTMB_table;
    MS_U16 u16Lib_size;

    if (pRes->sDMD_DTMB_PriData.bDownloaded)
    {
        if (_HAL_INTERN_DTMB_Ready())
        {
            HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x01); // reset VD_MCU
            HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00,  0x00);
            MsOS_DelayTask(20);
            return TRUE;
        }
    }

    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    if (pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
    {
        if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M)
        {
            DTMB_table = &INTERN_DTMB_6M_table_Waltz[0];
            u16Lib_size = sizeof(INTERN_DTMB_6M_table_Waltz);
        }
        else
        {
            DTMB_table = &INTERN_DTMB_table_Waltz[0];
            u16Lib_size = sizeof(INTERN_DTMB_table_Waltz);
        }
    }
    else
    {
        if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M)
        {
            DTMB_table = &INTERN_DTMB_6M_table[0];
            u16Lib_size = sizeof(INTERN_DTMB_6M_table);
        }
        else
        {
            DTMB_table = &INTERN_DTMB_table[0];
            u16Lib_size = sizeof(INTERN_DTMB_table);
        }
    }
    #else
    if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M)
    {
        DTMB_table = &INTERN_DTMB_6M_table[0];
        u16Lib_size = sizeof(INTERN_DTMB_6M_table);
    }
    else
    {
        DTMB_table = &INTERN_DTMB_table[0];
        u16Lib_size = sizeof(INTERN_DTMB_table);
    }
    #endif

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x00); // disable SRAM

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x00); // release MCU, madison patch

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x51); // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    ////  Load code thru VDMCU_IF ////
    HAL_INTERN_DTMB_DBINFO(printf(">Load Code...\n"));

    for (i = 0; i < u16Lib_size; i++)
    {
        HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, DTMB_table[i]); // write data to VD MCU 51 code sram
    }

    ////  Content verification ////
    HAL_INTERN_DTMB_DBINFO(printf(">Verify Code...\n"));

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    for (i = 0; i < u16Lib_size; i++)
    {
        udata = HAL_DMD_RIU_ReadByte(DMDMcuBase+0x10); // read sram data

        if (udata != DTMB_table[i])
        {
            HAL_INTERN_DTMB_DBINFO(printf(">fail add = 0x%x\n", i));
            HAL_INTERN_DTMB_DBINFO(printf(">code = 0x%x\n", DTMB_table[i]));
            HAL_INTERN_DTMB_DBINFO(printf(">data = 0x%x\n", udata));

            if (fail_cnt++ > 10)
            {
                HAL_INTERN_DTMB_DBINFO(printf(">DSP Loadcode fail!"));
                return FALSE;
            }
        }
    }

    u16AddressOffset = (DTMB_table[0x400] << 8)|DTMB_table[0x401];

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, (u16AddressOffset&0xFF)); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, (u16AddressOffset>>8));   // sram address high byte

    u8TmpData = (MS_U8)pRes->sDMD_DTMB_InitData.u16IF_KHZ;
    HAL_INTERN_DTMB_DBINFO(printf("u16IF_KHZ=%d\n",pRes->sDMD_DTMB_InitData.u16IF_KHZ));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_DTMB_InitData.u16IF_KHZ >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_DTMB_InitData.bIQSwap;
    HAL_INTERN_DTMB_DBINFO(printf("bIQSwap=%d\n",pRes->sDMD_DTMB_InitData.bIQSwap));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_DTMB_InitData.u16AGC_REFERENCE;
    HAL_INTERN_DTMB_DBINFO(printf("u16AGC_REFERENCE=%X\n",pRes->sDMD_DTMB_InitData.u16AGC_REFERENCE));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_DTMB_InitData.u16AGC_REFERENCE >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_DTMB_InitData.u32TdiStartAddr;
    HAL_INTERN_DTMB_DBINFO(printf("u32TdiStartAddr=%X\n",pRes->sDMD_DTMB_InitData.u32TdiStartAddr));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_DTMB_InitData.u32TdiStartAddr >> 8);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_DTMB_InitData.u32TdiStartAddr >> 16);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)(pRes->sDMD_DTMB_InitData.u32TdiStartAddr >> 24);
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram
    u8TmpData = (MS_U8)pRes->sDMD_DTMB_PriData.eLastType;
    HAL_INTERN_DTMB_DBINFO(printf("eLastType=%d\n",pRes->sDMD_DTMB_PriData.eLastType));
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, u8TmpData); // write data to VD MCU 51 code sram

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x00); // disable "vdmcu51_if"

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset MCU, madison patch

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x01); // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x00); // release VD_MCU

    pRes->sDMD_DTMB_PriData.bDownloaded = true;

    MsOS_DelayTask(20);

    HAL_INTERN_DTMB_DBINFO(printf(">DSP Loadcode done."));

    return TRUE;
}

static void _HAL_INTERN_DTMB_FWVERSION(void)
{
    MS_U8 data1,data2,data3;

    _MBX_ReadReg(0x20C4, &data1);
    _MBX_ReadReg(0x20C5, &data2);
    _MBX_ReadReg(0x20C6, &data3);

    HAL_INTERN_DTMB_DBINFO(printf("INTERN_DTMB_FW_VERSION:%x.%x.%x\n", data1, data2, data3));
}

static MS_BOOL _HAL_INTERN_DTMB_Exit(void)
{
    MS_U8 u8CheckCount = 0;

    HAL_DMD_RIU_WriteByte(MBRegBase + 0x1C, 0x01);

    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)|0x02);    // assert interrupt to VD MCU51
    HAL_DMD_RIU_WriteByte(DMDMcuBase + 0x03, HAL_DMD_RIU_ReadByte(DMDMcuBase + 0x03)&(~0x02)); // de-assert interrupt to VD MCU51

    while ((HAL_DMD_RIU_ReadByte(MBRegBase + 0x1C)&0x02) != 0x02)
    {
        MsOS_DelayTaskUs(10);

        if (u8CheckCount++ == 0xFF)
        {
            printf(">> DTMB Exit Fail!\n");
            return FALSE;
        }
    }

    printf(">> DTMB Exit Ok!\n");

    return TRUE;
}

static MS_BOOL _HAL_INTERN_DTMB_SoftReset(void)
{
    MS_U8 u8Data = 0;

    //Reset FSM
    if (_MBX_WriteReg(0x20C0, 0x00)==FALSE) return FALSE;

    while (u8Data!=0x02)
    {
        if (_MBX_ReadReg(0x20C1, &u8Data)==FALSE) return FALSE;
    }

    return TRUE;
}

static MS_BOOL _HAL_INTERN_DTMB_SetACICoef(void)
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;

    MS_U8  *ACI_table;
    MS_U8   i;
    MS_U16  u16AddressOffset;

    if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB)
        ACI_table = &_ACI_COEF_TABLE_FS24M_SR8M[0];
    else if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_7M)
        ACI_table = &_ACI_COEF_TABLE_FS24M_SR8M[0];
    else if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M)
        ACI_table = &_ACI_COEF_TABLE_FS24M_SR6M[0];
    else if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_5M)
        ACI_table = &_ACI_COEF_TABLE_FS24M_SR8M[0];
    else ACI_table = &_ACI_COEF_TABLE_FS24M_SR8M[0];

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset VD_MCU
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x00); // disable SRAM

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x00); // release MCU, madison patch

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // enable "vdmcu51_if"
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x51); // enable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, 0x00); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, 0x00); // sram address high byte

    //SET SR value
    u16AddressOffset = ((INTERN_DTMB_table[0x400] << 8)|INTERN_DTMB_table[0x401]) + 10;
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, (u16AddressOffset&0xFF)); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, (u16AddressOffset>>8));   // sram address high byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, (MS_U8)pRes->sDMD_DTMB_PriData.eLastType);

	//set ACI coefficient
	u16AddressOffset = ((INTERN_DTMB_table[0x40A] << 8)|INTERN_DTMB_table[0x40B]);
	u16AddressOffset = ((INTERN_DTMB_table[u16AddressOffset] << 8)|INTERN_DTMB_table[u16AddressOffset+1]);
	HAL_DMD_RIU_WriteByte(DMDMcuBase+0x04, (u16AddressOffset&0xFF)); // sram address low byte
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x05, (u16AddressOffset>>8));   // sram address high byte
    for (i = 0; i < DTMB_ACI_COEF_SIZE; i++)
    {
        HAL_DMD_RIU_WriteByte(DMDMcuBase+0x0C, ACI_table[i]); // write data to VD MCU 51 code sram
    }

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x50); // diable auto-increase
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x03, 0x00); // disable "vdmcu51_if"

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x01); // reset MCU, madison patch

    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x01, 0x01); // enable SRAM
    HAL_DMD_RIU_WriteByte(DMDMcuBase+0x00, 0x00); // release VD_MCU

    MsOS_DelayTask(20);

    return TRUE;
}

static MS_BOOL _HAL_INTERN_DTMB_SetDtmbMode(void)
{
    if (_MBX_WriteReg(0x20C2, 0x03)==FALSE) return FALSE;
    return _MBX_WriteReg(0x20C0, 0x04);
}

static MS_BOOL _HAL_INTERN_DTMB_SetModeClean(void)
{
    if (_MBX_WriteReg(0x20C2, 0x07)==FALSE) return FALSE;
    return _MBX_WriteReg(0x20C0, 0x00);
}

static MS_BOOL _HAL_INTERN_DTMB_Set_QAM_SR(void)
{
    if (_MBX_WriteReg(0x20C2, 0x01)==FALSE) return FALSE;
    return _MBX_WriteReg(0x20C0, 0x04);
}

static MS_BOOL _HAL_INTERN_DTMB_AGCLock(void)
{
    MS_U8 data = 0;

    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MOONEY)
    _MBX_ReadReg(0x2829, &data);//AGC_LOCK
    #else
    _MBX_ReadReg(0x271D, &data);//AGC_LOCK
    #endif
    if (data&0x01)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _HAL_INTERN_DTMB_PNP_Lock(void)
{
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;
    #endif

    MS_U8 data = 0;
    MS_U8 data1 = 0;

    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    if (pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
    {
        _MBX_ReadReg(0x3BBA, &data);
        _MBX_ReadReg(0x3C49, &data1);// CFO_FFT_SEC_VALID         (_REG_INNDEXT(0x24)+1)
    }
    else
    {
        _MBX_ReadReg(0x22BA, &data);
        _MBX_ReadReg(0x2349, &data1);// CFO_FFT_SEC_VALID         (_REG_INNDEXT(0x24)+1)
    }
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
    _MBX_ReadReg(0x37BA, &data);
    _MBX_ReadReg(0x3849, &data1);// CFO_FFT_SEC_VALID         (_REG_INNDEXT(0x24)+1)
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
    _MBX_ReadReg(0x11BA, &data);
    _MBX_ReadReg(0x1249, &data1);// CFO_FFT_SEC_VALID         (_REG_INNDEXT(0x24)+1)
    #else
    _MBX_ReadReg(0x3BBA, &data);
    _MBX_ReadReg(0x3C49, &data1);// CFO_FFT_SEC_VALID         (_REG_INNDEXT(0x24)+1)
    #endif
    #else
    _MBX_ReadReg(0x22BA, &data);
    _MBX_ReadReg(0x2349, &data1);// CFO_FFT_SEC_VALID         (_REG_INNDEXT(0x24)+1)
    #endif

	if (((data&0x02) == 0x02)&&((data1&0x20)==0x20))
	{
	    return TRUE;
	}
	else
	{
        return FALSE;
  }
}

static MS_BOOL _HAL_INTERN_DTMB_FEC_Lock(void)
{
    MS_U8 u8state=0;


	_MBX_ReadReg(0x20C1, &u8state);

	if ((u8state >= 0x62)&& (u8state <= 0xF0))
	{
	    return TRUE;
	}
	else
	{
        return FALSE;
    }
}

static MS_BOOL _HAL_INTERN_DTMB_GetModulation(DMD_DTMB_MODULATION_INFO *psDtmbGetModulation)
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;

    MS_U8 CM, QAM, IL, CR, SiNR;
    MS_U8 data_L = 0;
    MS_U8 data_H = 0;

    if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB ||
        pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_7M ||
        pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M ||
        pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_5M)
    {
        #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
        if(pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
        {
            _MBX_ReadReg(0x3B90, &data_L);
            _MBX_ReadReg(0x3B91, &data_H);
        }
        else
        {
            _MBX_ReadReg(0x2290, &data_L);
            _MBX_ReadReg(0x2291, &data_H);
        }
        #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
        _MBX_ReadReg(0x3790, &data_L);
        _MBX_ReadReg(0x3791, &data_H);
        #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
        #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
        _MBX_ReadReg(0x1190, &data_L);
        _MBX_ReadReg(0x1191, &data_H);
        #else
        _MBX_ReadReg(0x3B90, &data_L);
        _MBX_ReadReg(0x3B91, &data_H);
        #endif
        #else
        _MBX_ReadReg(0x2290, &data_L);
        _MBX_ReadReg(0x2291, &data_H);
        #endif

        if (data_L & 0x1)
        {
            CR   = (data_L >> 6) & 0x03;
            IL   = (data_L >> 3) & 0x01;
            QAM  = (data_L >> 4) & 0x03;
            SiNR = (data_L >> 2) & 0x01;
            CM   = (data_L >> 1) & 0x01;
        }
        else
        {
            #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
            if(pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
            {
                _MBX_ReadReg(0x3B9E, &data_L);
                _MBX_ReadReg(0x3B9F, &data_H);
            }
            else
            {
                _MBX_ReadReg(0x229E, &data_L);
                _MBX_ReadReg(0x229F, &data_H);
            }
            #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
            _MBX_ReadReg(0x379E, &data_L);
            _MBX_ReadReg(0x379F, &data_H);
            #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
            #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
            _MBX_ReadReg(0x119E, &data_L);
            _MBX_ReadReg(0x119F, &data_H);
            #else
            _MBX_ReadReg(0x3B9E, &data_L);
            _MBX_ReadReg(0x3B9F, &data_H);
            #endif
            #else
            _MBX_ReadReg(0x229E, &data_L);
            _MBX_ReadReg(0x229F, &data_H);
            #endif

            CR   = (data_H >> 4) & 0x03;
            IL   = (data_H >> 6) & 0x01;
            QAM  = (data_H >> 2) & 0x03;
            SiNR = (data_H >> 1) & 0x01;
            CM   = (data_H)      & 0x01;
        }

        #if 1//def MSOS_TYPE_LINUX_KERNEL
        if (CR == 0)
            psDtmbGetModulation->fSiCodeRate = 4;
        else if (CR == 1)
            psDtmbGetModulation->fSiCodeRate = 6;
        else if (CR == 2)
            psDtmbGetModulation->fSiCodeRate = 8;
        #else
        if (CR == 0)
            psDtmbGetModulation->fSiCodeRate = 0.4;
        else if (CR == 1)
            psDtmbGetModulation->fSiCodeRate = 0.6;
        else if (CR == 2)
            psDtmbGetModulation->fSiCodeRate = 0.8;
        #endif

        if (IL == 0)
            psDtmbGetModulation->u8SiInterLeaver = 240;
        else
            psDtmbGetModulation->u8SiInterLeaver = 720;

        if (QAM == 0)
            psDtmbGetModulation->u8SiQamMode = 4;
        else if (QAM == 1)
            psDtmbGetModulation->u8SiQamMode = 16;
        else if (QAM == 2)
            psDtmbGetModulation->u8SiQamMode = 32;
        else if (QAM == 3)
            psDtmbGetModulation->u8SiQamMode = 64;

        psDtmbGetModulation->u8SiCarrierMode = CM; // 0:Multi, 1:Single
        psDtmbGetModulation->u8SiNR = SiNR;
    }
    else
    {
    }

    return TRUE;
}

static MS_U8 _HAL_INTERN_DTMB_ReadIFAGC(void)
{
    MS_U8 data = 0;
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MOONEY)
    _MBX_ReadReg(0x280F, &data);
    #else
    _MBX_ReadReg(0x28FD, &data);
    #endif

    return data;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_DTMB_ReadFrequencyOffset(MS_S16 *pFftfirstCfo, MS_S8 *pFftSecondCfo, MS_S16 *pSr)
#else
static MS_S16 _HAL_INTERN_DTMB_ReadFrequencyOffset(void)
#endif
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;

	MS_U8 u8Data = 0;
    MS_S16 fftfirstCfo = 0;
    MS_S8 fftSecondCfo = 0;
    MS_S16 sr = 0;

    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    if (pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
    {
        _MBX_ReadReg(0x3C4D, &u8Data);
        fftfirstCfo = u8Data;
        _MBX_ReadReg(0x3C4C, &u8Data);
        fftfirstCfo = (fftfirstCfo<<8)|u8Data;

        _MBX_ReadReg(0x3C50, &u8Data);
        fftSecondCfo = u8Data;
    }
    else
    {
        _MBX_ReadReg(0x234D, &u8Data);
        fftfirstCfo = u8Data;
        _MBX_ReadReg(0x234C, &u8Data);
        fftfirstCfo = (fftfirstCfo<<8)|u8Data;

        _MBX_ReadReg(0x2350, &u8Data);
        fftSecondCfo = u8Data;
    }
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
    _MBX_ReadReg(0x384D, &u8Data);
    fftfirstCfo = u8Data;
    _MBX_ReadReg(0x384C, &u8Data);
    fftfirstCfo = (fftfirstCfo<<8)|u8Data;

    _MBX_ReadReg(0x3850, &u8Data);
    fftSecondCfo = u8Data;
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
    _MBX_ReadReg(0x124D, &u8Data);
    fftfirstCfo = u8Data;
    _MBX_ReadReg(0x124C, &u8Data);
    fftfirstCfo = (fftfirstCfo<<8)|u8Data;
    _MBX_ReadReg(0x1250, &u8Data);
    fftSecondCfo = u8Data;
    #else
    _MBX_ReadReg(0x3C4D, &u8Data);
    fftfirstCfo = u8Data;
    _MBX_ReadReg(0x3C4C, &u8Data);
    fftfirstCfo = (fftfirstCfo<<8)|u8Data;

    _MBX_ReadReg(0x3C50, &u8Data);
    fftSecondCfo = u8Data;
    #endif
    #else
    _MBX_ReadReg(0x234D, &u8Data);
    fftfirstCfo = u8Data;
    _MBX_ReadReg(0x234C, &u8Data);
    fftfirstCfo = (fftfirstCfo<<8)|u8Data;

    _MBX_ReadReg(0x2350, &u8Data);
    fftSecondCfo = u8Data;
    #endif

    if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M)
        sr = 5670;
    else sr = 7560;

    #ifdef UTPA2
    *pFftfirstCfo  = fftfirstCfo;
    *pFftSecondCfo = fftSecondCfo;
    *pSr           = sr;

    return TRUE;
    #else
    return (MS_S16)((((double)fftfirstCfo/0x10000+(double)fftSecondCfo/0x20000))*(double)sr);
    #endif
}

static MS_U8 _HAL_INTERN_DTMB_ReadSNRPercentage(void)
{
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;

    MS_U8 data  = 0;
    MS_U8 level = 0;
    MS_U32 snr  = 0;

    if (pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB ||
        pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_7M ||
        pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_6M ||
        pRes->sDMD_DTMB_PriData.eLastType == DMD_DTMB_DEMOD_DTMB_5M)
    {
        if (!_HAL_INTERN_DTMB_FEC_Lock())
            level = 0;
        else
        {
            #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
            if(pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
            {
                _MBX_ReadReg(0x3BDA, &data);
                snr = data&0x3F;
                _MBX_ReadReg(0x3BD9, &data);
                snr = (snr<<8)|data;
                _MBX_ReadReg(0x3BD8, &data);
                snr = (snr<<8)|data;
            }
            else
            {
                _MBX_ReadReg(0x22DA, &data);
                snr = data&0x3F;
                _MBX_ReadReg(0x22D9, &data);
                snr = (snr<<8)|data;
                _MBX_ReadReg(0x22D8, &data);
                snr = (snr<<8)|data;
            }
            #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
            _MBX_ReadReg(0x37DA, &data);
            snr = data&0x3F;
            _MBX_ReadReg(0x37D9, &data);
            snr = (snr<<8)|data;
            _MBX_ReadReg(0x37D8, &data);
            snr = (snr<<8)|data;
            #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
            #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
            _MBX_ReadReg(0x11DA, &data);
            snr = data&0x3F;
            _MBX_ReadReg(0x11D9, &data);
            snr = (snr<<8)|data;
            _MBX_ReadReg(0x11D8, &data);
            snr = (snr<<8)|data;
            #else
            _MBX_ReadReg(0x3BDA, &data);
            snr = data&0x3F;
            _MBX_ReadReg(0x3BD9, &data);
            snr = (snr<<8)|data;
            _MBX_ReadReg(0x3BD8, &data);
            snr = (snr<<8)|data;
            #endif
            #else
            _MBX_ReadReg(0x22DA, &data);
            snr = data&0x3F;
            _MBX_ReadReg(0x22D9, &data);
            snr = (snr<<8)|data;
            _MBX_ReadReg(0x22D8, &data);
            snr = (snr<<8)|data;
            #endif

            if (snr <= 4340   ) level = 1;       // SNR <= 0.6  dB
            else if (snr <= 4983   ) level = 2;  // SNR <= 1.2  dB
            else if (snr <= 5721   ) level = 3;  // SNR <= 1.8  dB
            else if (snr <= 6569   ) level = 4;  // SNR <= 2.4  dB
            else if (snr <= 7542   ) level = 5;  // SNR <= 3.0  dB
            else if (snr <= 8659   ) level = 6;  // SNR <= 3.6  dB
            else if (snr <= 9942   ) level = 7;  // SNR <= 4.2  dB
            else if (snr <= 11415  ) level = 8;  // SNR <= 4.8  dB
            else if (snr <= 13107  ) level = 9;  // SNR <= 5.4  dB
            else if (snr <= 15048  ) level = 10; // SNR <= 6.0  dB
            else if (snr <= 17278  ) level = 11; // SNR <= 6.6  dB
            else if (snr <= 19838  ) level = 12; // SNR <= 7.2  dB
            else if (snr <= 22777  ) level = 13; // SNR <= 7.8  dB
            else if (snr <= 26151  ) level = 14; // SNR <= 8.4  dB
            else if (snr <= 30026  ) level = 15; // SNR <= 9.0  dB
            else if (snr <= 34474  ) level = 16; // SNR <= 9.6  dB
            else if (snr <= 39581  ) level = 17; // SNR <= 10.2 dB
            else if (snr <= 45446  ) level = 18; // SNR <= 10.8 dB
            else if (snr <= 52179  ) level = 19; // SNR <= 11.4 dB
            else if (snr <= 59909  ) level = 20; // SNR <= 12.0 dB
            else if (snr <= 68785  ) level = 21; // SNR <= 12.6 dB
            else if (snr <= 78975  ) level = 22; // SNR <= 13.2 dB
            else if (snr <= 90676  ) level = 23; // SNR <= 13.8 dB
            else if (snr <= 104110 ) level = 24; // SNR <= 14.4 dB
            else if (snr <= 119534 ) level = 25; // SNR <= 15.0 dB
            else if (snr <= 137244 ) level = 26; // SNR <= 15.6 dB
            else if (snr <= 157577 ) level = 27; // SNR <= 16.2 dB
            else if (snr <= 180922 ) level = 28; // SNR <= 16.8 dB
            else if (snr <= 207726 ) level = 29; // SNR <= 17.4 dB
            else if (snr <= 238502 ) level = 30; // SNR <= 18.0 dB
            else if (snr <= 273837 ) level = 31; // SNR <= 18.6 dB
            else if (snr <= 314407 ) level = 32; // SNR <= 19.2 dB
            else if (snr <= 360987 ) level = 33; // SNR <= 19.8 dB
            else if (snr <= 414469 ) level = 34; // SNR <= 20.4 dB
            else if (snr <= 475874 ) level = 35; // SNR <= 21.0 dB
            else if (snr <= 546376 ) level = 36; // SNR <= 21.6 dB
            else if (snr <= 627324 ) level = 37; // SNR <= 22.2 dB
            else if (snr <= 720264 ) level = 38; // SNR <= 22.8 dB
            else if (snr <= 826974 ) level = 39; // SNR <= 23.4 dB
            else if (snr <= 949493 ) level = 40; // SNR <= 24.0 dB
            else if (snr <= 1090164) level = 41; // SNR <= 24.6 dB
            else if (snr <= 1251676) level = 42; // SNR <= 25.2 dB
            else if (snr <= 1437116) level = 43; // SNR <= 25.8 dB
            else if (snr <= 1650030) level = 44; // SNR <= 26.4 dB
            else if (snr <= 1894488) level = 45; // SNR <= 27.0 dB
            else if (snr <= 2175163) level = 46; // SNR <= 27.6 dB
            else if (snr <= 2497421) level = 47; // SNR <= 28.2 dB
            else if (snr <= 2867423) level = 48; // SNR <= 28.8 dB
            else if (snr <= 3292242) level = 49; // SNR <= 29.4 dB
            else if (snr  > 3292242) level = 50; // SNR <= 30.0 dB
        }
    }
    else
    {
        level = 0;
    }

    return level*2;
}

#ifdef UTPA2
static MS_BOOL _HAL_INTERN_DTMB_GetPreLdpcBer(MS_U32 *pBitErr, MS_U16 *pError_window)
#else
static MS_BOOL _HAL_INTERN_DTMB_GetPreLdpcBer(float *pber)
#endif
{
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    DMD_DTMB_ResData *pRes = psDMD_DTMB_ResData + u8DMD_DTMB_DMD_ID;
    #endif

    MS_U8   u8Data=0;
    MS_U32  BitErr;
    MS_U16  error_window;

    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    if (pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
    {
        _MBX_ReadReg(0x3F3B, &u8Data);
        BitErr = u8Data;
        _MBX_ReadReg(0x3F3A, &u8Data);
        BitErr = (BitErr << 8)|u8Data;
        _MBX_ReadReg(0x3F39, &u8Data);
        BitErr = (BitErr << 8)|u8Data;
        _MBX_ReadReg(0x3F38, &u8Data);
        BitErr = (BitErr << 8)|u8Data;
    }
    else
    {
        _MBX_ReadReg(0x263B, &u8Data);
        BitErr = u8Data;
        _MBX_ReadReg(0x263A, &u8Data);
        BitErr = (BitErr << 8)|u8Data;
        _MBX_ReadReg(0x2639, &u8Data);
        BitErr = (BitErr << 8)|u8Data;
        _MBX_ReadReg(0x2638, &u8Data);
        BitErr = (BitErr << 8)|u8Data;
    }
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
    _MBX_ReadReg(0x2D3B, &u8Data);
    BitErr = u8Data;
    _MBX_ReadReg(0x2D3A, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x2D39, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x2D38, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
    _MBX_ReadReg(0x163B, &u8Data);
    BitErr = u8Data;
    _MBX_ReadReg(0x163A, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x1639, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x1638, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    #else
    _MBX_ReadReg(0x3F3B, &u8Data);
    BitErr = u8Data;
    _MBX_ReadReg(0x3F3A, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x3F39, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x3F38, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    #endif
    #else
    _MBX_ReadReg(0x263B, &u8Data);
    BitErr = u8Data;
    _MBX_ReadReg(0x263A, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x2639, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    _MBX_ReadReg(0x2638, &u8Data);
    BitErr = (BitErr << 8)|u8Data;
    #endif

    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_NASA)
    if(pRes->sDMD_DTMB_PriData.u16ChipID == DMD_DTMB_CHIP_ID_WALTZ)
    {
        _MBX_ReadReg(0x3F2F, &u8Data);
        error_window = u8Data;
        _MBX_ReadReg(0x3F2E, &u8Data);
        error_window = (error_window << 8)|u8Data;
    }
    else
    {
        _MBX_ReadReg(0x262F, &u8Data);
        error_window = u8Data;
        _MBX_ReadReg(0x262E, &u8Data);
        error_window = (error_window << 8)|u8Data;
    }
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MADISON)
    _MBX_ReadReg(0x2D2F, &u8Data);
    error_window = u8Data;
    _MBX_ReadReg(0x2D2E, &u8Data);
    error_window = (error_window << 8)|u8Data;
    #elif (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MONACO || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MUJI || DMD_DTMB_CHIP_VERSION >= DMD_DTMB_CHIP_MONET)
    #if (DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MACAN || DMD_DTMB_CHIP_VERSION == DMD_DTMB_CHIP_MASERATI)
    _MBX_ReadReg(0x162F, &u8Data);
    error_window = u8Data;
    _MBX_ReadReg(0x162E, &u8Data);
    error_window = (error_window << 8)|u8Data;
    #else
    _MBX_ReadReg(0x3F2F, &u8Data);
    error_window = u8Data;
    _MBX_ReadReg(0x3F2E, &u8Data);
    error_window = (error_window << 8)|u8Data;
    #endif
    #else
    _MBX_ReadReg(0x262F, &u8Data);
    error_window = u8Data;
    _MBX_ReadReg(0x262E, &u8Data);
    error_window = (error_window << 8)|u8Data;
    #endif

    #ifdef UTPA2
    *pBitErr = BitErr;
    *pError_window = error_window;
    #else
    *pber=(float)BitErr/7488.0/(float)error_window;
    #endif

    return TRUE;
}

static MS_BOOL _HAL_INTERN_DTMB_GetReg(MS_U16 u16Addr, MS_U8 *pu8Data)
{
    return _MBX_ReadReg(u16Addr, pu8Data);
}

static MS_BOOL _HAL_INTERN_DTMB_SetReg(MS_U16 u16Addr, MS_U8 u8Data)
{
    return _MBX_WriteReg(u16Addr, u8Data);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_INTERN_DTMB_IOCTL_CMD(DMD_DTMB_HAL_COMMAND eCmd, void *pArgs)
{
    MS_BOOL bResult = TRUE;

    switch(eCmd)
    {
    case DMD_DTMB_HAL_CMD_Exit:
        bResult = _HAL_INTERN_DTMB_Exit();
        break;
    case DMD_DTMB_HAL_CMD_InitClk:
        _HAL_INTERN_DTMB_InitClk();
        break;
    case DMD_DTMB_HAL_CMD_Download:
        bResult = _HAL_INTERN_DTMB_Download();
        break;
    case DMD_DTMB_HAL_CMD_FWVERSION:
        _HAL_INTERN_DTMB_FWVERSION();
        break;
    case DMD_DTMB_HAL_CMD_SoftReset:
        bResult = _HAL_INTERN_DTMB_SoftReset();
        break;
    case DMD_DTMB_HAL_CMD_SetACICoef:
        bResult = _HAL_INTERN_DTMB_SetACICoef();
        break;
    case DMD_DTMB_HAL_CMD_SetDTMBMode:
        bResult = _HAL_INTERN_DTMB_SetDtmbMode();
        break;
    case DMD_DTMB_HAL_CMD_SetModeClean:
        bResult = _HAL_INTERN_DTMB_SetModeClean();
        break;
    case DMD_DTMB_HAL_CMD_Set_QAM_SR:
        bResult = _HAL_INTERN_DTMB_Set_QAM_SR();
        break;
    case DMD_DTMB_HAL_CMD_Active:
        break;
    case DMD_DTMB_HAL_CMD_AGCLock:
        bResult = _HAL_INTERN_DTMB_AGCLock();
        break;
    case DMD_DTMB_HAL_CMD_DTMB_PNP_Lock:
        bResult = _HAL_INTERN_DTMB_PNP_Lock();
        break;
    case DMD_DTMB_HAL_CMD_DTMB_FEC_Lock:
        bResult = _HAL_INTERN_DTMB_FEC_Lock();
        break;
    case DMD_DTMB_HAL_CMD_DVBC_PreLock:
        break;
    case DMD_DTMB_HAL_CMD_DVBC_Main_Lock:
        break;
    case DMD_DTMB_HAL_CMD_GetModulation:
        bResult = _HAL_INTERN_DTMB_GetModulation((DMD_DTMB_MODULATION_INFO *)pArgs);
    case DMD_DTMB_HAL_CMD_ReadIFAGC:
        *((MS_U16 *)pArgs) = _HAL_INTERN_DTMB_ReadIFAGC();
        break;
    case DMD_DTMB_HAL_CMD_ReadFrequencyOffset:
        #ifdef UTPA2
        bResult = _HAL_INTERN_DTMB_ReadFrequencyOffset(&((*((DMD_DTMB_CFO_DATA *)pArgs)).fftfirstCfo), &((*((DMD_DTMB_CFO_DATA *)pArgs)).fftSecondCfo), &((*((DMD_DTMB_CFO_DATA *)pArgs)).sr));
        #else
        *((MS_S16 *)pArgs) = _HAL_INTERN_DTMB_ReadFrequencyOffset();
        #endif
        break;
    case DMD_DTMB_HAL_CMD_ReadSNRPercentage:
        *((MS_U8 *)pArgs) = _HAL_INTERN_DTMB_ReadSNRPercentage();
        break;
    case DMD_DTMB_HAL_CMD_GetPreLdpcBer:
        #ifdef UTPA2
        bResult = _HAL_INTERN_DTMB_GetPreLdpcBer(&((*((DMD_DTMB_BER_DATA *)pArgs)).BitErr), &((*((DMD_DTMB_BER_DATA *)pArgs)).Error_window));
        #else
        bResult = _HAL_INTERN_DTMB_GetPreLdpcBer((float *)pArgs);
        #endif
        break;
    case DMD_DTMB_HAL_CMD_GetPreViterbiBer:
        break;
    case DMD_DTMB_HAL_CMD_GetPostViterbiBer:
        break;
    // case DMD_DTMB_HAL_CMD_GetSNR:
    //     break;
    case DMD_DTMB_HAL_CMD_TS_INTERFACE_CONFIG:
        break;
    case DMD_DTMB_HAL_CMD_IIC_Bypass_Mode:
        break;
    case DMD_DTMB_HAL_CMD_SSPI_TO_GPIO:
        break;
    case DMD_DTMB_HAL_CMD_GPIO_GET_LEVEL:
        break;
    case DMD_DTMB_HAL_CMD_GPIO_SET_LEVEL:
        break;
    case DMD_DTMB_HAL_CMD_GPIO_OUT_ENABLE:
        break;
    // case DMD_DTMB_HAL_CMD_DoIQSwap:
    //     break;
    case DMD_DTMB_HAL_CMD_GET_REG:
        bResult = _HAL_INTERN_DTMB_GetReg((*((DMD_DTMB_REG_DATA *)pArgs)).u16Addr, &((*((DMD_DTMB_REG_DATA *)pArgs)).u8Data));
        break;
    case DMD_DTMB_HAL_CMD_SET_REG:
        bResult = _HAL_INTERN_DTMB_SetReg((*((DMD_DTMB_REG_DATA *)pArgs)).u16Addr, (*((DMD_DTMB_REG_DATA *)pArgs)).u8Data);
        break;
    default:
        break;
    }

    return bResult;
}

MS_BOOL MDrv_DMD_DTMB_Initial_Hal_Interface(void)
{
    return TRUE;
}

