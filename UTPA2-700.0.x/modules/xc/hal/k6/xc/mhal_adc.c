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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#define MHAL_ADC_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/delay.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#if 0
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"

#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#endif
void MDrv_XC_ADC_Set_Freerun(void *pInstance, MS_BOOL bEnable)
{
    //do nothing
    return;
}

void Hal_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch)
{
}

#if 0

// Internal Definition
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_adc.h"
#include "mhal_ip.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8  u8L_BkAtop_00;
    MS_U8  u8L_BkAtop_01;
    MS_U8  u8L_BkAtop_03;
    MS_U8  u8L_BkAtop_0C;
    MS_U8  u8L_BkAtop_2C;
    MS_U8  u8L_BkAtop_1F;
    MS_U8  u8H_BkAtop_2D;
    MS_U8  u8L_BkDtop_06;
    MS_U8  u8H_BkChipTop_1F;
    MS_U8  u8L_BkChipTop_55;
    MS_U8  u8L_BkIpMux_1;
    MS_U8  u8L_SC_BK1_21;
    MS_U8  u8L_SC_BK10_19;
    MS_U16 u16L_BkAtop_05;
    MS_U16 u16L_BkAtop_5E;
    MS_U16 u16BkAtop_1C;
    MS_U16 u16BkAtop_05;
    MS_U16 u16BkAtop_06;
    MS_U16 u16BkDtop_01;
    MS_U16 u16BkDtop_02;
    MS_U16 u16SC_BK1_02;
    MS_U16 u16SC_BK1_03;
    MS_U16 u16SC_BK1_04;
    MS_U16 u16SC_BK1_05;
    MS_U16 u16SC_BK1_06;
    MS_U16 u16SC_BK1_07;
    MS_U16 u16SC_BK1_0E;
    MS_U16 u16SC_BK12_01;
    MS_U16 u16SC_BK12_03;
    MS_U16 u16SC_BK12_04;
    MS_U16 u16SC_BK12_0E;
    MS_U16 u16SC_BK12_0F;
    MS_U16 u16SC_BK12_16;
    MS_U16 u16SC_BK12_17;
    MS_U32 u32SC_BK12_10;
    MS_U32 u32SC_BK12_12;
    MS_U32 u32SC_BK12_14;
    MS_U16 u16SC_BK12_07;
    MS_U32 u32SC_BK12_08;
    MS_U32 u32SC_BK12_0A;
    MS_U32 u32SC_BK12_0C;
    MS_U16 u16SC_BK02_04;
    MS_U16 u16SC_BK02_05;
    MS_U16 u16SC_BK02_08;
    MS_U16 u16SC_BK02_09;
    MS_U16 u16SC_BK23_07;
    MS_U16 u16SC_BK23_08;
    MS_U16 u16SC_BK23_09;
    MS_U16 u16SC_BK23_0A;
    MS_U16 u16SC_BK12_1A;
    MS_U16 u16SC_BK12_1B;
    MS_U16 u16SC_BK06_01;
    MS_U16 u16SC_BK06_21;
    MS_U16 u16SC_BK12_44;
    MS_U16 u16SC_BK12_47;
} XC_Adc_BackupSetting;


typedef struct
{
    MS_U8 u8ADC_Power_04L;
    MS_U8 u8ADC_Power_04H;
    MS_U8 u8ADC_Power_05L;
    MS_U8 u8ADC_Power_06L;
    MS_U8 u8ADC_Power_06H;
    MS_U8 u8ADC_Power_60L;
    MS_U8 u8ADC_Power_60H;
    MS_U8 u8ADC_Power_69L;
    MS_U8 u8ADC_Power_69H;
    MS_U8 u8ADC_Power_40L;
}ADC_ATOP_POWERON_TBL_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

MS_U8 MST_ADCSetModeRGB_SOG_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_DATA_SIZE]=
{                 // Reg           Mask Ignore Value
 { DRV_ADC_REG(REG_ADC_ATOP_34_L), 0x0F, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x05/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x06/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x07/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x09/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x0A/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x0A/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x0B/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x0B/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x0C/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x0C/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x0D/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x0E/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_34_L), 0xF0, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x50/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x60/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x70/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x90/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0xA0/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0xA0/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0xB0/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0xB0/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0xC0/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0xC0/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0xD0/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0xE0/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_34_H), 0x0F, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x05/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x06/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x07/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x09/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x0A/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x0A/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x0B/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x0B/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x0C/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x0C/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x0D/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x0E/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_34_H), 0xF0, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x50/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x60/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x70/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x90/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0xA0/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0xA0/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0xB0/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0xB0/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0xC0/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0xC0/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0xD0/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0xE0/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_35_L), 0x0F, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x05/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x06/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x07/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x09/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x0A/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x0A/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x0B/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x0B/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x0C/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x0C/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x0D/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x0E/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_35_L), 0xF0, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x50/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x60/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x70/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x90/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0xA0/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0xA0/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0xB0/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0xB0/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0xC0/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0xC0/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0xD0/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0xE0/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_DTOP_17_H), 0x0F, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_DTOP_17_L), 0xFF, 0x00, 0xF0/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0xF0/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0xF0/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0xD0/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0xD0/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0xD0/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x95/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x95/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x88/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x88/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x60/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x30/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x30/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_DTOP_18_L), 0xFF, 0x00, 0x80/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x80/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x30/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x30/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_DTOP_19_H), 0x60, 0x00, 0x40/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x40/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_0D_L), 0x10, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_0C_L), 0x07, 0x00, 0x01/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_09_H), 0x18, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x08/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x10/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x18/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x18/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x18/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_0A_L), 0x04, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_61_H), 0x60, 0x00, 0x00/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x00/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x20/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_ADC_ATOP_09_H), 0x07, 0x00, 0x02/*ADC_TABLE_FREQ_SECTION1*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION2*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION3*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION4*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION5*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION6*/,
                                         0x00, 0x01/*ADC_TABLE_FREQ_SECTION7*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION8*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION9*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION10*/,
                                         0x00, 0x02/*ADC_TABLE_FREQ_SECTION11*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION12*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION13*/,
                                         0x00, 0x04/*ADC_TABLE_FREQ_SECTION14*/,
                                         0x00, 0x06/*ADC_TABLE_FREQ_SECTION15*/,
                                         0x00, 0x06/*ADC_TABLE_FREQ_SECTION16*/,
                                         0x00, 0x06/*ADC_TABLE_FREQ_SECTION17*/,},
 { DRV_ADC_REG(REG_TABLE_END), 0x00, 0x00, 0x00 }
};




//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static XC_Adc_BackupSetting _stAutoAdcSetting;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
///ADC soft reset
/******************************************************************************/
void Hal_ADC_reset(MS_U16 u16Reset)
{

    // Remove ADC reset after T3
    /*
        W2BYTE(REG_ADC_ATOP_07_L, u16Reset);
        W2BYTE(REG_ADC_ATOP_07_L, 0x0000);
    */
}

/******************************************************************************/
///This function will set ADC registers for different port
///@param enInputSourceType \b IN: source type
///@param u8InputClock \b IN: pixel clock
/******************************************************************************/
//=========================================================//
// Function : Hal_ADC_ext_clk_en
// Description:
//=========================================================//
void Hal_ADC_ext_clk_en(MS_BOOL benable)
{
    W2BYTEMSK(REG_ADC_ATOP_58_L, (benable ? BIT(7):0), BIT(7));
}

//=========================================================//
// Function : Hal_ADC_hdmi_vco_ctrl
// Description:
//=========================================================//
void Hal_ADC_hdmi_vco_ctrl(MS_U16 u16InputClock)
{
    if (u16InputClock > 108)
    {
        W2BYTEMSK(REG_ADC_ATOP_4C_L, (0x02<<8), HBMASK);
    }
    else
    {
        W2BYTEMSK(REG_ADC_ATOP_4C_L, (0x0E<<8), HBMASK);
    }

}
//=========================================================//
// Function : Hal_ADC_vco_ctrl
// Description:
//=========================================================//
void Hal_ADC_vco_ctrl(MS_U16 u16InputClock)
{
    MS_U8 u8Value;

    // VCO range/Settling time
    if (u16InputClock > 150)
    {
        // high
        u8Value = 0x02;
    }
    else if (u16InputClock > 20)
    {
        // middle
        u8Value = 0x01;
    }
    else
    {
        // low
        u8Value = 0x00;
     }
    // set multiplier of ADC PLL clock
    W2BYTEMSK(REG_ADC_ATOP_0C_L, u8Value, (BIT(2)|BIT(1)|BIT(0)));
}

void Hal_ADC_set_vco_ctrl(MS_BOOL bIsDVIPort, MS_U16 u16InputClock)
{
    // enable external clock
    Hal_ADC_ext_clk_en(ENABLE);

    if ( bIsDVIPort )
    {
        Hal_ADC_hdmi_vco_ctrl(u16InputClock);
    }
    else
    {
        Hal_ADC_vco_ctrl(u16InputClock);
    }

    // disable external clock
    Hal_ADC_ext_clk_en(DISABLE);
}

/******************************************************************************/
///This function sets PLL clock divider ratio
///@param u16Value \b IN: PLL clock divider ratio
/******************************************************************************/
void Hal_ADC_dtop_clk_setting ( MS_U16 u16Value )
{
    // limit set ADC PLL
    if((u16Value > 3) && (u16Value < ADC_MAX_CLK))
    {
        u16Value -= 3; // actual - 3
        W2BYTE(REG_ADC_DTOP_00_L, u16Value);
    }
}

/******************************************************************************/
///This function return phase steps of current chip
///@param u8Value \b IN: phase steps
/******************************************************************************/
MS_U16 Hal_ADC_get_phase_range(void)
{
	return 128;
}

MS_U8 Hal_ADC_get_phase(void)
{
   return ( (MS_U8) R2BYTEMSK(REG_ADC_DTOP_03_L,LBMASK) ) ;
}

MS_U16 Hal_ADC_get_phaseEx(void)
{
   return ( (MS_U16) R2BYTEMSK(REG_ADC_DTOP_03_L,LBMASK) ) ;
}

/******************************************************************************/
///This function sets PLL phase
///@param u8Value \b IN: PLL phase divider ratio
/******************************************************************************/
void Hal_ADC_set_phase( MS_U8 u8Value )
{
    W2BYTEMSK(REG_ADC_DTOP_03_L, u8Value, LBMASK);
}

void Hal_ADC_set_phaseEx( MS_U16 u16Value )
{
    W2BYTEMSK(REG_ADC_DTOP_03_L, u16Value, LBMASK);
}

/******************************************************************************/
///This function sets ADC offset
///@param *pstADCSetting \b IN: pointer to ADC settings
/******************************************************************************/
void Hal_ADC_offset_setting ( XC_AdcGainOffsetSetting *pstADCSetting  )
{
    W2BYTEMSK(REG_ADC_DTOP_08_L, (~(pstADCSetting->u16RedOffset))<<8, HBMASK);
    W2BYTEMSK(REG_ADC_DTOP_09_L, (~(pstADCSetting->u16GreenOffset))<<8, HBMASK);
    W2BYTEMSK(REG_ADC_DTOP_0A_L, (~(pstADCSetting->u16BlueOffset))<<8, HBMASK);
}

/******************************************************************************/
///This function sets ADC gain
///@param *pstADCSetting \b IN: pointer to ADC settings
/******************************************************************************/
void Hal_ADC_gain_setting ( XC_AdcGainOffsetSetting *pstADCSetting  )
{
    W2BYTEMSK(REG_ADC_DTOP_08_L, ~(pstADCSetting->u16RedGain), LBMASK);
    W2BYTEMSK(REG_ADC_DTOP_09_L, ~(pstADCSetting->u16GreenGain), LBMASK);
    W2BYTEMSK(REG_ADC_DTOP_0A_L, ~(pstADCSetting->u16BlueGain), LBMASK);
}

/******************************************************************************/
///This function enable/disable output double buffer
///@param bEnable \b IN:
///- Enable: Turn on ADC double buffer
///- Disable: Turn off ADC double buffer
/******************************************************************************/
void Hal_ADC_doublebuffer_setting(MS_BOOL bEnable)
{
    W2BYTEMSK(REG_ADC_DTOP_07_L, (bEnable ? BIT(0):0), BIT(0));
}

/******************************************************************************/
///This function recalibrates ADC offset. This function should be called
///after mode changed.
///@param bFlag \b IN:
///- 0: Turn on
///- 1: Turn off
/******************************************************************************/
void Hal_ADC_dtop_calibration_target_setting(MS_BOOL bIsYPbPrFlag)
{
    if(bIsYPbPrFlag)
    {
        // Use code 16 as offset CAL target
        W2BYTEMSK(REG_ADC_DTOP_10_L, BIT(15), BIT(15));
    }
    else
    {
        // Use code 0 as offset CAL target
        W2BYTEMSK(REG_ADC_DTOP_10_L, 0, BIT(15));
    }
}
void Hal_ADC_dtop_sw_mode_setting(MS_BOOL bEnable, MS_BOOL bIsAutoSWMode)
{
    if(bEnable)
    {
        if (bIsAutoSWMode)      //normal procedure
        {
            W2BYTEMSK(REG_ADC_DTOP_10_L, 0x00, 0xF3);
        }
        else
        {
            W2BYTEMSK(REG_ADC_DTOP_10_L, 0x50, 0xF3);
        }

    }
    else            //used when doing YPbPr calibration with software mode
    {
        W2BYTEMSK(REG_ADC_DTOP_10_L, 0x00, 0xF3);
    }
}

void Hal_ADC_Set_Source_Calibration(ADC_INPUTSOURCE_TYPE enADC_SourceType)
{
    return;
}

/******************************************************************************/
///This function sets clamp placement
///@param u8Value \b IN:
/******************************************************************************/
void Hal_ADC_clamp_placement_setting(MS_U16 u16InputClockMHz)
{
    if(u16InputClockMHz>= 40)
    {
        /* Vclamp_dly */
        W2BYTEMSK(REG_ADC_DTOP_0B_L, 0x38, LBMASK);
    }
    else
    {
        W2BYTEMSK(REG_ADC_DTOP_0B_L, 0x08, LBMASK);
    }
}


void Hal_XC_ADC_Set_VClamp_level(ADC_VClamp_Level_Type type)
{
    MS_U16 VClampSetting=E_ADC_VClamp_0_85V;
    switch(type)
    {
        case E_ADC_VClamp_0_85V:
            VClampSetting = 0x2;
            break;
        case E_ADC_VClamp_0_9V:
            VClampSetting = 0x3;
            break;
        case E_ADC_VClamp_0_95V:
            VClampSetting = 0x4;
            break;
        case E_ADC_VClamp_1_0V:
            VClampSetting = 0x5;
            break;
        case E_ADC_VClamp_1_05V:
            VClampSetting = 0x6;
            break;
        case E_ADC_VClamp_1_2V:
            VClampSetting = 0x1;
            break;
        case E_ADC_VClamp_1_5V:
            VClampSetting = 0x8;
            break;
    }
    W2BYTEMSK(REG_ADC_ATOP_2D_L, VClampSetting << 8, HBMASK);
}

/******************************************************************************/
///This function sets input HSync polarity
///@param u8Value \b IN:
/******************************************************************************/
void Hal_ADC_hpolarity_setting(MS_BOOL bHightActive)
{
    W2BYTEMSK(REG_ADC_DTOP_07_L, (bHightActive ? BIT(7):0), BIT(7));
}

/******************************************************************************/
///This function power off ADC
/******************************************************************************/
void Hal_ADC_poweroff(void)
{
//    W2BYTE(REG_ADC_ATOP_04_L, 0xFFFE);         // Bit-0 is relative to DRAM.
//    W2BYTEMSK(REG_ADC_ATOP_05_L, 0xFF, LBMASK);
    W2BYTE(REG_ADC_ATOP_06_L, 0xFFFF);
//    W2BYTE(REG_ADC_ATOP_60_L, 0xFFFF);         // Bit-4 is relative to DRAM
//    W2BYTE(REG_ADC_ATOP_69_L, 0xFFFF);         // Bit-4 is relative to DRAM
    W2BYTEMSK(REG_ADC_ATOP_70_L, 0x0F, LBMASK); // reg_cvbso1_pd

    W2BYTEMSK(REG_ADC_ATOP_4C_L, BIT(5), BIT(5));
//    W2BYTEMSK(REG_ADC_ATOP_40_L, BIT(6), BIT(6));
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Hal_ADC_dtop_internaldc_setting(ADC_Internal_Voltage InternalVoltage)
{
    MS_U16 u16regvalue = 0;

    switch ( InternalVoltage )
    {
        case E_ADC_Internal_0V:
             u16regvalue = 0x00;
        break;
        case E_ADC_Internal_0_1V:
             u16regvalue = 0x20;
        break;
        case E_ADC_Internal_0_6V:
             u16regvalue = 0x30; //switch internal to 0.6V
        break;
        default:
              u16regvalue = 0x00;
        break;
    }

    W2BYTEMSK(REG_ADC_DTOP_13_L, InternalVoltage, 0x30);
}

void Hal_ADC_ExitExternalCalibration(ADC_INPUTSOURCE_TYPE eADC_Source,XC_AdcGainOffsetSetting* InitialGainOffset)
{
    if ( eADC_Source == ADC_INPUTSOURCE_ONLY_RGB)
    {

    }
	else if ( eADC_Source == ADC_INPUTSOURCE_ONLY_YPBPR)
	{


	}
    else if (eADC_Source == ADC_INPUTSOURCE_ONLY_SCART )
    {
		W2BYTEMSK(REG_ADC_ATOP_42_L, BIT(5) , BIT(5));
    }
    else
    {
        // Undefined.
    }

}

#endif

// If define this function to false, driver layer will cause dead_error_condition (if condition always be false)
// So we need implement this function
MS_BOOL Hal_ADC_InitExternalCalibration(void *pInstance, MS_U32 enAdcSource)
{
    return FALSE;
}

#if 0
void Hal_ADC_InitInternalCalibration(SCALER_WIN eWindow)
{
    XC_AdcGainOffsetSetting gain_offset_setting;

    MS_U8 u8Bank;
    u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == SUB_WINDOW)
    {
        // 444 format
        MDrv_WriteByte( BK_SELECT_00, REG_BANK_SCMI);
        MDrv_WriteRegBit(L_BK_SCMI(0x41), TRUE, BIT(5));
        MDrv_WriteRegBit(L_BK_SCMI(0x43), FALSE, BIT(4));
        //MDrv_WriteRegBit(H_BK_SCMI(0x43), 0x0, (BIT(4)|BIT(5)));//disable mirror

        // 10-bit
        MDrv_WriteByte( BK_SELECT_00, REG_BANK_SCMI);
        MDrv_WriteByteMask(H_BK_SCMI(0x41), 0x01, BIT(0)|BIT(1)|BIT(2));
        MDrv_WriteByteMask(H_BK_SCMI(0x41), 0x00, BIT(4)|BIT(5)|BIT(6));
        MDrv_WriteRegBit(L_BK_SCMI(0x44), TRUE, BIT(1));

        MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F1 );
        MDrv_WriteByte(L_BK_IP1F1(0x0E), 0x11); // enable auto gain function

        // Disable NR
        //SC_W2BYTEMSK(REG_SC_BK06_21_L, 0, BIT(1)|BIT(0));
        //SC_W2BYTEMSK(REG_SC_BK06_01_L, 0, BIT(1)|BIT(0));

        //framebuffer number
        //SC_W2BYTEMSK(REG_SC_BK12_04_L, 0, BIT(6)|BIT(7));
        //SC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(13));
        SC_W2BYTEMSK(REG_SC_BK12_44_L, 0, BIT(6)|BIT(7));
        SC_W2BYTEMSK(REG_SC_BK12_47_L, 0, BIT(13));
    }
    else
    {
        // 444 format
        MDrv_WriteByte( BK_SELECT_00, REG_BANK_SCMI);
        MDrv_WriteRegBit(L_BK_SCMI(0x01), TRUE, BIT(5));
        MDrv_WriteRegBit(L_BK_SCMI(0x03), FALSE, BIT(4));
        //MDrv_WriteRegBit(H_BK_SCMI(0x03), 0x0, (BIT(4)|BIT(5)));//disable mirror

        // 10-bit
        MDrv_WriteByte( BK_SELECT_00, REG_BANK_SCMI);
        MDrv_WriteByteMask(H_BK_SCMI(0x01), 0x01, BIT(0)|BIT(1)|BIT(2));
        MDrv_WriteByteMask(H_BK_SCMI(0x01), 0x00, BIT(4)|BIT(5)|BIT(6));
        MDrv_WriteRegBit(L_BK_SCMI(0x04), TRUE, BIT(1));

        MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F2 );
        MDrv_WriteByte(L_BK_IP1F2(0x0E), 0x11); // enable auto gain function

        // Disable NR
        SC_W2BYTEMSK(REG_SC_BK06_21_L, 0, BIT(1)|BIT(0));
        SC_W2BYTEMSK(REG_SC_BK06_01_L, 0, BIT(1)|BIT(0));

        //framebuffer number
        SC_W2BYTEMSK(REG_SC_BK12_04_L, 0, BIT(6)|BIT(7));
        SC_W2BYTEMSK(REG_SC_BK12_07_L, 0, BIT(13));
        //SC_W2BYTEMSK(REG_SC_BK12_44_L, 0, BIT(6)|BIT(7));
        //SC_W2BYTEMSK(REG_SC_BK12_47_L, 0, BIT(13));
    }


    // Set gain offset as middle core.
    gain_offset_setting.u16BlueGain = gain_offset_setting.u16GreenGain =
    gain_offset_setting.u16RedGain = 0x80;

    gain_offset_setting.u16BlueOffset = gain_offset_setting.u16GreenOffset =
    gain_offset_setting.u16RedOffset = 0x80;

    Hal_ADC_offset_setting(&gain_offset_setting);
    Hal_ADC_gain_setting(&gain_offset_setting);


    MDrv_Write2Byte(REG_ADC_ATOP_00_L ,0x0001 );
    MDrv_Write2Byte(REG_ADC_ATOP_04_L ,0xFE00 );
    MDrv_Write2Byte(REG_ADC_ATOP_05_L ,0x009B );
    MDrv_Write2Byte(REG_ADC_ATOP_06_L ,0xEBF0);

    W2BYTEMSK( L_BK_IPMUX(0x01) , 0xF0, 0xF0 );  //select pattern generator source
    if(eADC_Source == ADC_INPUTSOURCE_ONLY_SCART )
    {
        MDrv_Write2Byte(REG_ADC_ATOP_03_L ,0x0000 );
        MDrv_Write2Byte(REG_ADC_ATOP_5E_L ,0x0200);
        MDrv_WriteByte(L_BK_CHIPTOP(0x55),0x00);
    }

    MDrv_WriteByte(REG_ADC_ATOP_1C_H, 0xF8);   // Turn on SOG input low bandwidth filter

    MDrv_WriteByte(BK_SELECT_00, u8Bank);

}


void Hal_ADC_clk_gen_setting(ADC_Gen_Clock_Type clocktype)
{

    W2BYTEMSK(REG_ADC_ATOP_01_L, 0x0f, 0x0f );

    W2BYTEMSK(REG_ADC_ATOP_1C_L, BIT(5), BIT(5) );  /// turn off ADC a SoG comparator
    W2BYTEMSK(REG_ADC_ATOP_1F_L, BIT(5), BIT(5) );  /// turn off ADC a SoG comparator


    switch(clocktype)
    {
    case E_ADC_Gen_480P_Clk:
    default:
        W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x00, 0x07 );
        W2BYTE(REG_ADC_DTOP_01_L, 0x0040);
        W2BYTE(REG_ADC_DTOP_02_L, 0x0000);
        break;
    case E_ADC_Gen_720P_Clk:
        W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x01, 0x07 );
        W2BYTE(REG_ADC_DTOP_01_L, 0xB82E);
        W2BYTE(REG_ADC_DTOP_02_L, 0x0052);
        break;
    case E_ADC_Gen_1080P_Clk:
        W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x10, 0x07 );
        W2BYTE(REG_ADC_DTOP_01_L, 0x0723);
        W2BYTE(REG_ADC_DTOP_02_L, 0x0086);
        break;
    }

    W2BYTEMSK(REG_ADC_DTOP_06_L, 0x80, 0x80);
}

//----------------------------------------------------------------------
//  RGB Gain setting
//----------------------------------------------------------------------
void Hal_ADC_dtop_gain_r_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_08_L, u16value, LBMASK);
}
void Hal_ADC_dtop_gain_g_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_09_L, u16value, LBMASK);
}
void Hal_ADC_dtop_gain_b_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_0A_L, u16value, LBMASK);
}
//----------------------------------------------------------------------
//  RGB Offset setting
//----------------------------------------------------------------------
//    MDrv_WriteByte( H_BK_ADC_DTOP(0x08),DcOffset_R);
//    MDrv_WriteByte( H_BK_ADC_DTOP(0x09),DcOffset_G);
//    MDrv_WriteByte( H_BK_ADC_DTOP(0x0A),DcOffset_B);

void Hal_ADC_dtop_offset_r_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_08_L, u16value<<8, HBMASK);
}
void Hal_ADC_dtop_offset_g_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_09_L, u16value<<8, HBMASK);
}
void Hal_ADC_dtop_offset_b_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_0A_L, u16value<<8, HBMASK);
}

#endif

/******************************************************************************/
/// Power
/******************************************************************************/
/******************************************************************************/
///Initialize ADC
/******************************************************************************/
void Hal_ADC_init(void *pInstance, MS_U16 u16XTAL_CLK,MS_BOOL IsShareGrd,  MS_U16 eScartIDPortSelection)
{
    return;

/*
    MS_U8 u8MPLL_LOOP_2nd_DIVIDER;

    // ShareGrd setting only for T3.
    UNUSED(IsShareGrd);
    UNUSED(eScartIDPortSelection);

    W2BYTEMSK(REG_ADC_ATOP_04_L, 0x00, HBMASK);    //
    W2BYTEMSK(REG_ADC_ATOP_05_L, 0x00, LBMASK);   //
    W2BYTEMSK(REG_ADC_ATOP_0B_L, 0x00, LBMASK);

    // PLL
    W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x01, LBMASK);    // VCO
    W2BYTEMSK(REG_ADC_ATOP_12_L, 0x01, LBMASK);    // ADC_B VCO S3 may not need to programming this because S3 only has one ADC

    W2BYTEMSK(REG_ADC_ATOP_1C_L, 0x10, LBMASK);    // SOG trigger lenel

    // enhance SOG performance
    W2BYTEMSK(REG_ADC_ATOP_21_L, 0x40<<8, HBMASK);    // To increse hysteresis
    W2BYTEMSK(REG_ADC_ATOP_22_L, 0x30, LBMASK);    // To increse SOG clamping ability

    W2BYTEMSK(REG_ADC_ATOP_46_L, 0x80, LBMASK);    // DAC gain, 0x20
    W2BYTEMSK(REG_ADC_ATOP_46_L, 0x10<<8, HBMASK);    // LVDS/RSDS/TTL output logic regulator voltage contrl
    W2BYTEMSK(REG_ADC_ATOP_2E_L, 0x00, LBMASK);    // I-clamp setting

    W2BYTEMSK(REG_ADC_ATOP_60_L, 0x00, LBMASK);    //DVI  //HDMI Port A/B
    W2BYTEMSK(REG_ADC_ATOP_60_L, 0x00, HBMASK);
    W2BYTEMSK(REG_ADC_ATOP_69_L, 0x00, LBMASK);    //DVI2 //HDMI Port C
    W2BYTEMSK(REG_ADC_ATOP_69_L, 0x00, HBMASK);

    W2BYTEMSK(REG_ADC_ATOP_0B_L, 0x00, LBMASK);

    W2BYTEMSK(REG_ADC_DTOP_01_L, 0x81, LBMASK);    // PLL loop filer control
    W2BYTEMSK(REG_ADC_DTOP_01_L, 0x09<<8, HBMASK);    // PLL loop filer control
    W2BYTEMSK(REG_ADC_DTOP_02_L, 0x03, LBMASK);    // PLL loop filer control
    W2BYTEMSK(REG_ADC_DTOP_04_L, 0x05, LBMASK);    // setting time
    W2BYTEMSK(REG_ADC_DTOP_04_L, 0x95<<8, HBMASK);    // PLL control for composite sync input
    W2BYTEMSK(REG_ADC_DTOP_0B_L, 0x10, LBMASK);    // clamp placement
    W2BYTEMSK(REG_ADC_DTOP_0B_L, 0x08<<8, HBMASK);    // clamp duration


    u8MPLL_LOOP_2nd_DIVIDER = (MS_U8)((215000UL*2 + (u16XTAL_CLK/2)) / u16XTAL_CLK );
    W2BYTEMSK(REG_ADC_ATOP_0A_L, u8MPLL_LOOP_2nd_DIVIDER, LBMASK);
    */
}

#if 0
void Hal_XC_ADC_poweron_source(ADC_INPUTSOURCE_TYPE enADC_SourceType)
{
    ADC_ATOP_POWERON_TBL_t adc_tbl;
    MS_ADC_POWER_ON_TYPE enADCPowerType = MS_ADC_POWER_ALL_OFF;
    MS_U8 u8Src_En = 0;

    switch (enADC_SourceType)
    {
        case ADC_INPUTSOURCE_ONLY_DVI:

            u8Src_En |= En_DVI;
            enADCPowerType = MS_DVI_POWER_ON;
            break;
        case ADC_INPUTSOURCE_ONLY_RGB:
        case ADC_INPUTSOURCE_ONLY_YPBPR:
            u8Src_En |= (En_ADC_A | En_ADC_AMUX);
            enADCPowerType = MS_ADC_A_POWER_ON;
            break;
        case ADC_INPUTSOURCE_ONLY_SCART:
            u8Src_En |= (En_VD|En_FB_RGB|EN_ADC_FB);
            enADCPowerType = MS_VDA_FBLANK_POWER_ON;
            break;
        case ADC_INPUTSOURCE_ONLY_SVIDEO:
            u8Src_En |= En_VD|En_VD_YC|En_FB_RGB;
            u8Src_En &= ~En_ADC_AMUX;
            enADCPowerType = MS_VDA_SV_POWER_ON;
            break;
        case ADC_INPUTSOURCE_ONLY_CVBS:
        case ADC_INPUTSOURCE_ONLY_MVOP:
            u8Src_En |= En_VD;
            u8Src_En &= ~En_ADC_AMUX;
            enADCPowerType = MS_VDA_CVBS_POWER_ON;
            break;
        default:
            u8Src_En |= En_VD;
            u8Src_En &= ~En_ADC_AMUX;
            enADCPowerType = MS_VDA_CVBS_POWER_ON;
            break;
    }

    // Enable ADC source
    W2BYTEMSK(REG_ADC_ATOP_00_L, u8Src_En, 0x00FF);

    // Source Power On
    adc_tbl.u8ADC_Power_04L = (BIT(7) | BIT(6) | BIT(0));
    adc_tbl.u8ADC_Power_04H = (BIT(0));
    adc_tbl.u8ADC_Power_05L = (BIT(6) | BIT(5) | BIT(2));
    adc_tbl.u8ADC_Power_06L = (BIT(0));
    adc_tbl.u8ADC_Power_06H = (BIT(4) | BIT(2));
    adc_tbl.u8ADC_Power_60L = (BIT(6) | BIT(4));
    adc_tbl.u8ADC_Power_60H = (0x00);
    adc_tbl.u8ADC_Power_69L = (BIT(6) | BIT(4));
    adc_tbl.u8ADC_Power_69H = (0x00);
    adc_tbl.u8ADC_Power_40L = BIT(6);

    switch (enADCPowerType)
    {
    case MS_ADC_A_POWER_ON:
        adc_tbl.u8ADC_Power_04L |= (BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1));
        adc_tbl.u8ADC_Power_06L |= (BIT(3)|BIT(2)|BIT(1));
        break;

    case MS_VDA_CVBS_POWER_ON:
        adc_tbl.u8ADC_Power_04H |= (BIT(7)|BIT(4)|BIT(1));
        adc_tbl.u8ADC_Power_05L |= (BIT(4)|BIT(3)|BIT(1));
        adc_tbl.u8ADC_Power_06L |= (BIT(7)|BIT(6));
        break;

    case MS_VDA_SV_POWER_ON:
        adc_tbl.u8ADC_Power_04L |= (BIT(5));
        adc_tbl.u8ADC_Power_04H |= (BIT(7)|BIT(6)|BIT(4)|BIT(1));
        adc_tbl.u8ADC_Power_05L |= (BIT(4)|BIT(3)|BIT(1)|BIT(0));
        adc_tbl.u8ADC_Power_06L |= (BIT(7)|BIT(6));
        break;

    case MS_VDA_FBLANK_POWER_ON:
        adc_tbl.u8ADC_Power_04L |= (BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1));
        adc_tbl.u8ADC_Power_04H |= (BIT(7)|BIT(4)|BIT(1));
        adc_tbl.u8ADC_Power_05L |= (BIT(4)|BIT(3)|BIT(1));
        adc_tbl.u8ADC_Power_06L |= (BIT(7)|BIT(6)|BIT(3)|BIT(2)|BIT(1));
        adc_tbl.u8ADC_Power_06H |= (BIT(6));
        adc_tbl.u8ADC_Power_40L = 0;
        break;

    case MS_DVI_POWER_ON:
        adc_tbl.u8ADC_Power_06H |= (BIT(7)|BIT(5)|BIT(3)|BIT(1)|BIT(0));
        adc_tbl.u8ADC_Power_60L |= (BIT(7)|BIT(5)|BIT(3)|BIT(2)|BIT(1)|BIT(0));
        adc_tbl.u8ADC_Power_60H |= (BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0));
        adc_tbl.u8ADC_Power_69L |= (BIT(7)|BIT(5)|BIT(3)|BIT(2)|BIT(1)|BIT(0));
        adc_tbl.u8ADC_Power_69H |= (BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0));
        break;
    /*
    case MS_ADC_VD_BLEND_POWER_ON:
        adc_tbl.u8ADC_Power_04L |= (BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1));
        adc_tbl.u8ADC_Power_04H |= (BIT(7)|BIT(4)|BIT(1));
        adc_tbl.u8ADC_Power_05L |= (BIT(4)|BIT(3)|BIT(1));
        adc_tbl.u8ADC_Power_06L |= (BIT(7)|BIT(6)|BIT(3)|BIT(2)|BIT(1));
        adc_tbl.u8ADC_Power_06H |= (BIT(6));
        adc_tbl.u8ADC_Power_40L = 0;
        break;
    */
     default:
        break;
    }

    // Always turn DVI/HDCP clk on for all source.
    // Blue-ray BD1400 will stop playing if power off DVI/HDCP clk and HDMI cable plug-in.
    // This will cause component video still.
    adc_tbl.u8ADC_Power_06H |= (BIT(1));
    adc_tbl.u8ADC_Power_60L |= (BIT(7)|BIT(5));
    //////////////////////////////////////////////////////

    adc_tbl.u8ADC_Power_04L &= ~(BIT(5) | BIT(4) | BIT(3));
    adc_tbl.u8ADC_Power_04L |= ~(R2BYTEMSK(REG_ADC_ATOP_04_L, LBMASK));
    adc_tbl.u8ADC_Power_04H &= ~BIT(4);
    adc_tbl.u8ADC_Power_04H |= ~(R2BYTEMSK(REG_ADC_ATOP_04_L, HBMASK));
    adc_tbl.u8ADC_Power_05L &= ~(BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0));
    adc_tbl.u8ADC_Power_05L |= ~(R2BYTEMSK(REG_ADC_ATOP_05_L, LBMASK));
    //adc_tbl.u8ADC_Power_40L = (R2BYTEMSK(REG_ADC_ATOP_40_L, LBMASK) & 0x40;

    //W2BYTEMSK(REG_ADC_ATOP_04_L, (0xFF & ~(adc_tbl.u8ADC_Power_04L)), LBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_04_L, (0xFF & ~(adc_tbl.u8ADC_Power_04H))<<8, HBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_05_L, (0xFF & ~(adc_tbl.u8ADC_Power_05L)), LBMASK);
    W2BYTEMSK(REG_ADC_ATOP_06_L, (0xFF & ~(adc_tbl.u8ADC_Power_06L)), LBMASK);
    W2BYTEMSK(REG_ADC_ATOP_06_L, (0xFF & ~(adc_tbl.u8ADC_Power_06H))<<8, HBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_60_L, (0xFF & ~(adc_tbl.u8ADC_Power_60L)), LBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_60_L, (0xFF & ~(adc_tbl.u8ADC_Power_60H))<<8, HBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_69_L, (0xFF & ~(adc_tbl.u8ADC_Power_69L)), LBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_69_L, (0xFF & ~(adc_tbl.u8ADC_Power_69H))<<8, HBMASK);
    //W2BYTEMSK(REG_ADC_ATOP_40_L, adc_tbl.u8ADC_Power_40L  , BIT(6));
}

void Hal_ADC_Set_Source(ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount)
{
    switch(enADC_SourceType)
    {
        case ADC_INPUTSOURCE_ONLY_RGB:

            /* Vclamp */
            W2BYTEMSK(REG_ADC_ATOP_2C_L, 0x00, (BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))); // G clamp to VP3
            /* clamp placement */ /* clamp duration */
            W2BYTE(REG_ADC_DTOP_0B_L, 0x810);

            /* PLL multiplier */
            W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x01, (BIT(2) | BIT(1) | BIT(0)));
            /* PLL phase setting time */
            W2BYTEMSK(REG_ADC_DTOP_04_L, 0x05, (BIT(3) | BIT(2) | BIT(1) | BIT(0)));
            /* SOG level */
            W2BYTEMSK(REG_ADC_ATOP_1C_L, 0x25, (BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0)));
            /* ADC GenCtrl */
            W2BYTEMSK(REG_ADC_DTOP_07_L, 0x02, LBMASK);
            break;

        case ADC_INPUTSOURCE_ONLY_YPBPR:

            /* New mid-clamp */
            W2BYTEMSK(REG_ADC_ATOP_2C_L, 0x77, (BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))); //0x77 2007-12-4
            /* clamp placement */ /* clamp duration */
            W2BYTE(REG_ADC_DTOP_0B_L, 0x530);

            /* PLL multiplier */
            W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x00, (BIT(2) | BIT(1) | BIT(0)));
            /* PLL phase setting time */
            W2BYTEMSK(REG_ADC_DTOP_04_L, 0x08, (BIT(3) | BIT(2) | BIT(1) | BIT(0)));
            /* SOG level */
            W2BYTEMSK(REG_ADC_ATOP_1C_L, 0x05, (BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0)));
            /* ADC GenCtrl */
            W2BYTEMSK(REG_ADC_DTOP_07_L, 0x6A, LBMASK);
            break;

         case ADC_INPUTSOURCE_ONLY_DVI:

             W2BYTEMSK(REG_ADC_ATOP_2C_L, 0x00, (BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))); // G clamp to VP3
             Hal_ADC_reset(REST_ADC|REST_DVI|REST_HDCP);
             break;

         case ADC_INPUTSOURCE_ONLY_SVIDEO:
         case ADC_INPUTSOURCE_ONLY_SCART:
         case ADC_INPUTSOURCE_ONLY_CVBS:

            /* Vclamp */
            W2BYTEMSK(REG_ADC_ATOP_2C_L, 0x00, (BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0))); // G clamp to VP3
            /* PLL multiplier */
            W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x01, LBMASK);
            /* PLL phase setting time */
            W2BYTEMSK(REG_ADC_DTOP_04_L, 0x05, LBMASK);
            /* SOG level */
            W2BYTEMSK(REG_ADC_ATOP_1C_L, 0x05, (BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0)));
            break;
         default:
            break;
    }

    //U3 first version need ADC to turn off its jitter-finetune function for VD case.
    if(enADC_SourceType &
        ( ADC_INPUTSOURCE_ONLY_CVBS   | ADC_INPUTSOURCE_ONLY_SCART
        | ADC_INPUTSOURCE_ONLY_SVIDEO | ADC_INPUTSOURCE_ONLY_ATV ))
    {
         W2BYTEMSK(REG_ADC_ATOP_13_L, BIT(6), BIT(6));
    }
    else
    {
         W2BYTEMSK(REG_ADC_ATOP_13_L, 0, BIT(6));
    }

#ifdef MSOS_TYPE_LINUX_KERNEL
    mdelay(2);
#else
    MsOS_DelayTask(2);
#endif

}

// Only support input port is CVBS or DAC


//MS_BOOL bEnable,MS_BOOL bIsSvideoSource, MS_BOOL bIsDACSource, E_MUX_INPUTPORT* enPorts, MS_U8 u8Port_Count)

void Hal_ADC_set_cvbs_out(E_ADC_CVBSOUT_TYPE e_cvbs_out_type)
{
    MS_U16 u16channel = 0xF , u16clamp = 0 , u16test = 0 ;

    if ( e_cvbs_out_type != ADC_CVBSOUT_DISABLE_1 || e_cvbs_out_type != ADC_CVBSOUT_DISABLE_2)
    {
        // Need refine.
        // It should not read mux from hardware and set.
        if ( e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_1 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_2 ||
             e_cvbs_out_type == ADC_CVBSOUT_VIF_VIF_1 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VIF_2)
        {
            u16channel = 0x5C;
        }
        else if ( e_cvbs_out_type == ADC_CVBSOUT_SV_ON_1 || e_cvbs_out_type == ADC_CVBSOUT_SV_OFF_1 ||
                  e_cvbs_out_type == ADC_CVBSOUT_SV_ON_2 || e_cvbs_out_type == ADC_CVBSOUT_SV_OFF_2
                )
        {
            u16channel = 0x5C;
        }
        else if ( e_cvbs_out_type == ADC_CVBSOUT_CVBS_ON_1 || e_cvbs_out_type == ADC_CVBSOUT_CVBS_OFF_1 ||
                  e_cvbs_out_type == ADC_CVBSOUT_CVBS_ON_2 || e_cvbs_out_type == ADC_CVBSOUT_CVBS_OFF_2
                )
        {
            u16channel = 0x14;
            u16clamp   = 0x03;
            u16test    = 0x0A;
        }
        else
        {
            // Undefined
        }
    }
    else
    {
        // Turn off cvbs out.
        u16channel = 0x0F;
    }

    if ( e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_1 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_2)
    {
        MDrv_WriteRegBit(REG_ADC_ATOP_46_L,DISABLE,BIT(6));
    }
    else
    {
        MDrv_WriteRegBit(REG_ADC_ATOP_46_L,ENABLE,BIT(6));
    }

    // For channel 1
    if ( e_cvbs_out_type >= ADC_CVBSOUT_DISABLE_1 &&  e_cvbs_out_type <= ADC_CVBSOUT_VIF_VIF_1 )
    {

        if (e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_1 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VIF_1 )
        {
            W2BYTE(REG_ADC_ATOP_39_L, 0x0B0B );
        }

        W2BYTEMSK(REG_ADC_ATOP_38_L, u16channel & 0xFF, LBMASK);
        W2BYTEMSK(REG_ADC_ATOP_3A_L, u16clamp & 0xFF, LBMASK);
        W2BYTEMSK(REG_ADC_ATOP_3B_L, u16test<<8, HBMASK);
    }
    else // For channel 2
    {

        if (e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_2 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VIF_2 )
        {
            W2BYTE(REG_ADC_ATOP_3D_L, 0x0B0B );
        }
        W2BYTEMSK(REG_ADC_ATOP_3C_L, u16channel & 0xFF, LBMASK);
        W2BYTEMSK(REG_ADC_ATOP_3E_L, u16clamp & 0xFF, LBMASK);
        W2BYTEMSK(REG_ADC_ATOP_3F_L, u16test<<8, HBMASK);
    }

}

/*
// Define the ADC filter BW table
// Table rule :
// Bandwidth frequency from small to large
*/
MS_U16 tAdcFilterBW[17][2] = // {MHz, Reg.Value},
{
    {  7, 0x0F},
    { 15, 0x0E},
    { 16, 0x0D},
    { 17, 0x0C},
    { 18, 0x0B},
    { 20, 0x0A},
    { 22, 0x09},
    { 24, 0x08},
    { 26, 0x07},
    { 28, 0x06},
    { 30, 0x05},
    { 60, 0x04},
    {125, 0x03},
    {150, 0x02},
    {190, 0x01},
    {250, 0x00},
    {0xFFFF, 0x00},
};

void Hal_ADC_set_mode(ADC_INPUTSOURCE_TYPE enADCInput, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio)
{
    MS_U8 u8Loop;

    u16PixelClockPerSecond = ADC_FILTER_BW_DCLK(((enADCInput & ADC_INPUTSOURCE_ONLY_RGB)!= 0)?TRUE:FALSE, u16PixelClockPerSecond);

    //MDrv_ADC_pll_setting
    for(u8Loop = 0 ; u8Loop < sizeof(tAdcFilterBW)/(sizeof(MS_U16)*2); u8Loop++)
    {
        if(tAdcFilterBW[u8Loop][0] > u16PixelClockPerSecond)
        {
            W2BYTEMSK(REG_ADC_ATOP_1C_L, (((tAdcFilterBW[u8Loop][1])<<4)<<8), 0xF000);
            break;
        }
    }

    /* MDrv_ADC_sog_filter_setting */
    if (u16PixelClockPerSecond > ADC_SOG_FILTER_THRSHLD)
    {
        W2BYTEMSK(REG_ADC_ATOP_1C_L, 0 , BIT(11));
    }
    else
    {
        W2BYTEMSK(REG_ADC_ATOP_1C_L, BIT(11) , BIT(11));
    }

    Hal_ADC_dtop_clk_setting (u16HorizontalTotal * u16SamplingRatio);

    //Hal_ADC_clamp_duration_setting( u16HorizontalTotal/50 );
    W2BYTEMSK(REG_ADC_DTOP_0B_L, (u16HorizontalTotal/50)<<8, HBMASK);

}

void Hal_ADC_get_default_gain_offset(ADC_INPUTSOURCE_TYPE adc_src,XC_AdcGainOffsetSetting* ADCSetting)
{
    switch(adc_src)
    {
        case ADC_INPUTSOURCE_ONLY_RGB:      // RGB source
        case ADC_INPUTSOURCE_ONLY_YPBPR:
        case ADC_INPUTSOURCE_ONLY_SCART:
        default:
            ADCSetting->u16RedGain = ADCSetting->u16GreenGain = ADCSetting->u16BlueGain = 0x80;
            ADCSetting->u16RedOffset = ADCSetting->u16GreenOffset = ADCSetting->u16BlueOffset = 0x80;
            break;
    }

}


MS_U16 Hal_ADC_get_center_gain(void)
{
    return 0x0080;
}

MS_U16 Hal_ADC_get_center_offset(void)
{
    return 0x0080;
}

MS_U8 Hal_ADC_get_offset_bit_cnt(void)
{
    return 8;
}

MS_U8 Hal_ADC_get_gain_bit_cnt(void)
{
    return 8;
}

void Hal_ADC_auto_adc_backup(SCALER_WIN eWindow)
{
    MS_U8 u8Bank;

    u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == SUB_WINDOW)
    {
        MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
    }
    else
    {
        MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    }
    _stAutoAdcSetting.u8L_BkAtop_00    = MDrv_ReadByte(REG_ADC_ATOP_00_L );
    _stAutoAdcSetting.u8L_BkAtop_01    = MDrv_ReadByte(REG_ADC_ATOP_01_L );
    _stAutoAdcSetting.u8L_BkAtop_0C    = MDrv_ReadByte(REG_ADC_ATOP_0C_L );
    _stAutoAdcSetting.u8L_BkAtop_2C    = MDrv_ReadByte(REG_ADC_ATOP_2C_L );
    _stAutoAdcSetting.u8L_BkAtop_1F    = MDrv_ReadByte(REG_ADC_ATOP_1F_L );
    _stAutoAdcSetting.u8H_BkAtop_2D    = MDrv_ReadByte(REG_ADC_ATOP_2D_H );
    _stAutoAdcSetting.u8L_BkDtop_06    = MDrv_ReadByte(REG_ADC_DTOP_06_L );
    _stAutoAdcSetting.u8L_BkAtop_03    = MDrv_ReadByte(REG_ADC_ATOP_03_L );
    _stAutoAdcSetting.u16L_BkAtop_05   = MDrv_Read2Byte(REG_ADC_ATOP_05_L );
    _stAutoAdcSetting.u16L_BkAtop_5E   = MDrv_Read2Byte(REG_ADC_ATOP_5E_L );
    _stAutoAdcSetting.u8H_BkChipTop_1F = MDrv_ReadByte(H_BK_CHIPTOP(0x1f));
    _stAutoAdcSetting.u8L_BkChipTop_55 = MDrv_ReadByte(L_BK_CHIPTOP(0x55));
    _stAutoAdcSetting.u8L_BkIpMux_1    = MDrv_ReadByte(L_BK_IPMUX(0x01) );
    if(eWindow == SUB_WINDOW)
    {
        _stAutoAdcSetting.u8L_SC_BK1_21    = MDrv_ReadByte(L_BK_IP1F1(0x21) );
    }
    else
    {
        _stAutoAdcSetting.u8L_SC_BK1_21    = MDrv_ReadByte(L_BK_IP1F2(0x21) );
    }

    _stAutoAdcSetting.u16BkAtop_1C     = MDrv_Read2Byte(REG_ADC_ATOP_1C_L );
    _stAutoAdcSetting.u16BkAtop_05     = MDrv_Read2Byte(REG_ADC_ATOP_05_L);
    _stAutoAdcSetting.u16BkAtop_06     = MDrv_Read2Byte(REG_ADC_ATOP_06_L );
    _stAutoAdcSetting.u16BkDtop_01     = MDrv_Read2Byte(REG_ADC_DTOP_01_L );
    _stAutoAdcSetting.u16BkDtop_02     = MDrv_Read2Byte(REG_ADC_DTOP_02_L );

    if(eWindow == SUB_WINDOW)
    {
        _stAutoAdcSetting.u16SC_BK1_02     = SC_R2BYTE(REG_SC_BK03_02_L);
        _stAutoAdcSetting.u16SC_BK1_03     = SC_R2BYTE(REG_SC_BK03_03_L);
        _stAutoAdcSetting.u16SC_BK1_04     = SC_R2BYTE(REG_SC_BK03_04_L);
        _stAutoAdcSetting.u16SC_BK1_05     = SC_R2BYTE(REG_SC_BK03_05_L);
        _stAutoAdcSetting.u16SC_BK1_06     = SC_R2BYTE(REG_SC_BK03_06_L);
        _stAutoAdcSetting.u16SC_BK1_07     = SC_R2BYTE(REG_SC_BK03_07_L);
        _stAutoAdcSetting.u16SC_BK1_0E     = SC_R2BYTE(REG_SC_BK03_0E_L);

        _stAutoAdcSetting.u16SC_BK12_01    = SC_R2BYTE(REG_SC_BK12_41_L);
        _stAutoAdcSetting.u16SC_BK12_03    = SC_R2BYTE(REG_SC_BK12_43_L);
        _stAutoAdcSetting.u16SC_BK12_04    = SC_R2BYTE(REG_SC_BK12_44_L);
        _stAutoAdcSetting.u16SC_BK12_0E    = SC_R2BYTE(REG_SC_BK12_4E_L);
        _stAutoAdcSetting.u16SC_BK12_0F    = SC_R2BYTE(REG_SC_BK12_4F_L);
        _stAutoAdcSetting.u16SC_BK12_16    = SC_R2BYTE(REG_SC_BK12_56_L);
        _stAutoAdcSetting.u16SC_BK12_17    = SC_R2BYTE(REG_SC_BK12_57_L);

        //scaling
        _stAutoAdcSetting.u16SC_BK02_04    = SC_R2BYTE(REG_SC_BK04_04_L);
        _stAutoAdcSetting.u16SC_BK02_05    = SC_R2BYTE(REG_SC_BK04_05_L);
        _stAutoAdcSetting.u16SC_BK02_08    = SC_R2BYTE(REG_SC_BK04_08_L);
        _stAutoAdcSetting.u16SC_BK02_09    = SC_R2BYTE(REG_SC_BK04_09_L);
        _stAutoAdcSetting.u16SC_BK23_07    = SC_R2BYTE(REG_SC_BK23_27_L);
        _stAutoAdcSetting.u16SC_BK23_08    = SC_R2BYTE(REG_SC_BK23_28_L);
        _stAutoAdcSetting.u16SC_BK23_09    = SC_R2BYTE(REG_SC_BK23_29_L);
        _stAutoAdcSetting.u16SC_BK23_0A    = SC_R2BYTE(REG_SC_BK23_2A_L);

        //DNR base
        _stAutoAdcSetting.u32SC_BK12_08    = SC_R4BYTE(REG_SC_BK12_48_L);
        _stAutoAdcSetting.u32SC_BK12_0A    = SC_R4BYTE(REG_SC_BK12_4A_L);
        _stAutoAdcSetting.u32SC_BK12_0C    = SC_R4BYTE(REG_SC_BK12_4C_L);
        //OPM Base
        _stAutoAdcSetting.u32SC_BK12_10    = SC_R4BYTE(REG_SC_BK12_50_L);
        _stAutoAdcSetting.u32SC_BK12_12    = SC_R4BYTE(REG_SC_BK12_52_L);
        _stAutoAdcSetting.u32SC_BK12_14    = SC_R4BYTE(REG_SC_BK12_54_L);

        _stAutoAdcSetting.u16SC_BK06_01    = SC_R2BYTE(REG_SC_BK06_01_L);
        _stAutoAdcSetting.u16SC_BK06_21    = SC_R2BYTE(REG_SC_BK06_21_L);
        _stAutoAdcSetting.u16SC_BK12_07    = SC_R2BYTE(REG_SC_BK12_07_L);
        _stAutoAdcSetting.u16SC_BK12_44    = SC_R2BYTE(REG_SC_BK12_44_L);
        _stAutoAdcSetting.u16SC_BK12_47    = SC_R2BYTE(REG_SC_BK12_47_L);
        _stAutoAdcSetting.u16SC_BK12_1A     = SC_R2BYTE(REG_SC_BK12_5A_L);
        _stAutoAdcSetting.u16SC_BK12_1B     = SC_R2BYTE(REG_SC_BK12_5B_L);
    }
    else
    {
        _stAutoAdcSetting.u16SC_BK1_02     = SC_R2BYTE(REG_SC_BK01_02_L);
        _stAutoAdcSetting.u16SC_BK1_03     = SC_R2BYTE(REG_SC_BK01_03_L);
        _stAutoAdcSetting.u16SC_BK1_04     = SC_R2BYTE(REG_SC_BK01_04_L);
        _stAutoAdcSetting.u16SC_BK1_05     = SC_R2BYTE(REG_SC_BK01_05_L);
        _stAutoAdcSetting.u16SC_BK1_06     = SC_R2BYTE(REG_SC_BK01_06_L);
        _stAutoAdcSetting.u16SC_BK1_07     = SC_R2BYTE(REG_SC_BK01_07_L);
        _stAutoAdcSetting.u16SC_BK1_0E     = SC_R2BYTE(REG_SC_BK01_0E_L);

        _stAutoAdcSetting.u16SC_BK12_01    = SC_R2BYTE(REG_SC_BK12_01_L);
        _stAutoAdcSetting.u16SC_BK12_03    = SC_R2BYTE(REG_SC_BK12_03_L);
        _stAutoAdcSetting.u16SC_BK12_04    = SC_R2BYTE(REG_SC_BK12_04_L);
        _stAutoAdcSetting.u16SC_BK12_0E    = SC_R2BYTE(REG_SC_BK12_0E_L);
        _stAutoAdcSetting.u16SC_BK12_0F    = SC_R2BYTE(REG_SC_BK12_0F_L);
        _stAutoAdcSetting.u16SC_BK12_16    = SC_R2BYTE(REG_SC_BK12_16_L);
        _stAutoAdcSetting.u16SC_BK12_17    = SC_R2BYTE(REG_SC_BK12_17_L);

        //scaling
        _stAutoAdcSetting.u16SC_BK02_04    = SC_R2BYTE(REG_SC_BK02_04_L);
        _stAutoAdcSetting.u16SC_BK02_05    = SC_R2BYTE(REG_SC_BK02_05_L);
        _stAutoAdcSetting.u16SC_BK02_08    = SC_R2BYTE(REG_SC_BK02_08_L);
        _stAutoAdcSetting.u16SC_BK02_09    = SC_R2BYTE(REG_SC_BK02_09_L);
        _stAutoAdcSetting.u16SC_BK23_07    = SC_R2BYTE(REG_SC_BK23_07_L);
        _stAutoAdcSetting.u16SC_BK23_08    = SC_R2BYTE(REG_SC_BK23_08_L);
        _stAutoAdcSetting.u16SC_BK23_09    = SC_R2BYTE(REG_SC_BK23_09_L);
        _stAutoAdcSetting.u16SC_BK23_0A    = SC_R2BYTE(REG_SC_BK23_0A_L);

        //DNR base
        _stAutoAdcSetting.u32SC_BK12_08    = SC_R4BYTE(REG_SC_BK12_08_L);
        _stAutoAdcSetting.u32SC_BK12_0A    = SC_R4BYTE(REG_SC_BK12_0A_L);
        _stAutoAdcSetting.u32SC_BK12_0C    = SC_R4BYTE(REG_SC_BK12_0C_L);
        //OPM Base
        _stAutoAdcSetting.u32SC_BK12_10    = SC_R4BYTE(REG_SC_BK12_10_L);
        _stAutoAdcSetting.u32SC_BK12_12    = SC_R4BYTE(REG_SC_BK12_12_L);
        _stAutoAdcSetting.u32SC_BK12_14    = SC_R4BYTE(REG_SC_BK12_14_L);

        _stAutoAdcSetting.u16SC_BK06_01    = SC_R2BYTE(REG_SC_BK06_01_L);
        _stAutoAdcSetting.u16SC_BK06_21    = SC_R2BYTE(REG_SC_BK06_21_L);
        _stAutoAdcSetting.u16SC_BK12_07    = SC_R2BYTE(REG_SC_BK12_07_L);
        _stAutoAdcSetting.u16SC_BK12_44    = SC_R2BYTE(REG_SC_BK12_44_L);
        _stAutoAdcSetting.u16SC_BK12_47    = SC_R2BYTE(REG_SC_BK12_47_L);
        _stAutoAdcSetting.u16SC_BK12_1A     = SC_R2BYTE(REG_SC_BK12_1A_L);
        _stAutoAdcSetting.u16SC_BK12_1B     = SC_R2BYTE(REG_SC_BK12_1B_L);
    }

    MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    _stAutoAdcSetting.u8L_SC_BK10_19   = MDrv_ReadByte(L_BK_VOP(0x19) );

    MDrv_WriteByteMask(REG_ADC_ATOP_5C_L, 0x30,0x30);  //ldo

    MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

void Hal_ADC_auto_adc_restore(void)
{
    MS_U8 u8Bank;

    u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == SUB_WINDOW)
    {
        MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
    }
    else
    {
        MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    }
    MDrv_WriteByte(REG_ADC_ATOP_00_L, _stAutoAdcSetting.u8L_BkAtop_00);
    MDrv_WriteByte(REG_ADC_ATOP_01_L, _stAutoAdcSetting.u8L_BkAtop_01);
    MDrv_WriteByte(REG_ADC_ATOP_0C_L, _stAutoAdcSetting.u8L_BkAtop_0C);
    MDrv_WriteByte(REG_ADC_ATOP_2C_L, _stAutoAdcSetting.u8L_BkAtop_2C);
    MDrv_WriteByte(REG_ADC_ATOP_1F_L, _stAutoAdcSetting.u8L_BkAtop_1F);
    MDrv_WriteByte(REG_ADC_ATOP_2D_H, _stAutoAdcSetting.u8H_BkAtop_2D);
    MDrv_WriteByte(REG_ADC_ATOP_03_L, _stAutoAdcSetting.u8L_BkAtop_03);
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, _stAutoAdcSetting.u16L_BkAtop_05);
    MDrv_Write2Byte(REG_ADC_ATOP_5E_L, _stAutoAdcSetting.u16L_BkAtop_5E);
    MDrv_WriteByte(REG_ADC_DTOP_06_L, _stAutoAdcSetting.u8L_BkDtop_06);
    MDrv_WriteByte(H_BK_CHIPTOP(0x1f), _stAutoAdcSetting.u8H_BkChipTop_1F);
    MDrv_WriteByte(L_BK_CHIPTOP(0x55), _stAutoAdcSetting.u8L_BkChipTop_55);
    MDrv_WriteByte(L_BK_IPMUX(0x01), _stAutoAdcSetting.u8L_BkIpMux_1);

    if(eWindow == SUB_WINDOW)
    {
        MDrv_WriteByte(L_BK_IP1F1(0x21), _stAutoAdcSetting.u8L_SC_BK1_21);
    }
    else
    {
        MDrv_WriteByte(L_BK_IP1F2(0x21), _stAutoAdcSetting.u8L_SC_BK1_21);
    }

    MDrv_Write2Byte(REG_ADC_ATOP_1C_L, _stAutoAdcSetting.u16BkAtop_1C);
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, _stAutoAdcSetting.u16BkAtop_05);
    MDrv_Write2Byte(REG_ADC_ATOP_06_L, _stAutoAdcSetting.u16BkAtop_06);
    MDrv_Write2Byte(REG_ADC_DTOP_01_L, _stAutoAdcSetting.u16BkDtop_01);
    MDrv_Write2Byte(REG_ADC_DTOP_02_L, _stAutoAdcSetting.u16BkDtop_02);

    if(eWindow == SUB_WINDOW)
    {
        MDrv_Write2Byte(L_BK_IP1F1(0x02), _stAutoAdcSetting.u16SC_BK1_02);
        MDrv_Write2Byte(L_BK_IP1F1(0x03), _stAutoAdcSetting.u16SC_BK1_03);
        MDrv_Write2Byte(L_BK_IP1F1(0x04), _stAutoAdcSetting.u16SC_BK1_04);
        MDrv_Write2Byte(L_BK_IP1F1(0x05), _stAutoAdcSetting.u16SC_BK1_05);
        MDrv_Write2Byte(L_BK_IP1F1(0x06), _stAutoAdcSetting.u16SC_BK1_06);
        MDrv_Write2Byte(L_BK_IP1Fr(0x07), _stAutoAdcSetting.u16SC_BK1_07);
        MDrv_Write2Byte(L_BK_IP1F1(0x0E), _stAutoAdcSetting.u16SC_BK1_0E);

        MDrv_WriteByte(BK_SELECT_00, REG_BANK_SCMI);
        MDrv_Write2Byte(L_BK_SCMI(0x41), _stAutoAdcSetting.u16SC_BK12_01);
        MDrv_Write2Byte(L_BK_SCMI(0x43), _stAutoAdcSetting.u16SC_BK12_03);
        MDrv_Write2Byte(L_BK_SCMI(0x44), _stAutoAdcSetting.u16SC_BK12_04);
        MDrv_Write2Byte(L_BK_SCMI(0x4E), _stAutoAdcSetting.u16SC_BK12_0E);
        MDrv_Write2Byte(L_BK_SCMI(0x4F), _stAutoAdcSetting.u16SC_BK12_0F);
        MDrv_Write2Byte(L_BK_SCMI(0x56), _stAutoAdcSetting.u16SC_BK12_16);
        MDrv_Write2Byte(L_BK_SCMI(0x57), _stAutoAdcSetting.u16SC_BK12_17);

        //scaling
        SC_W2BYTE(REG_SC_BK04_04_L, _stAutoAdcSetting.u16SC_BK02_04);
        SC_W2BYTE(REG_SC_BK04_05_L, _stAutoAdcSetting.u16SC_BK02_05);
        SC_W2BYTE(REG_SC_BK04_08_L, _stAutoAdcSetting.u16SC_BK02_08);
        SC_W2BYTE(REG_SC_BK04_09_L, _stAutoAdcSetting.u16SC_BK02_09);
        SC_W2BYTE(REG_SC_BK23_27_L, _stAutoAdcSetting.u16SC_BK23_07);
        SC_W2BYTE(REG_SC_BK23_28_L, _stAutoAdcSetting.u16SC_BK23_08);
        SC_W2BYTE(REG_SC_BK23_29_L, _stAutoAdcSetting.u16SC_BK23_09);
        SC_W2BYTE(REG_SC_BK23_2A_L, _stAutoAdcSetting.u16SC_BK23_0A);

        //DNR,OPM Base
        SC_W4BYTE(REG_SC_BK12_48_L, _stAutoAdcSetting.u32SC_BK12_08);
        SC_W4BYTE(REG_SC_BK12_4A_L, _stAutoAdcSetting.u32SC_BK12_0A);
        SC_W4BYTE(REG_SC_BK12_4C_L, _stAutoAdcSetting.u32SC_BK12_0C);
        SC_W4BYTE(REG_SC_BK12_50_L, _stAutoAdcSetting.u32SC_BK12_10);
        SC_W4BYTE(REG_SC_BK12_52_L, _stAutoAdcSetting.u32SC_BK12_12);
        SC_W4BYTE(REG_SC_BK12_54_L, _stAutoAdcSetting.u32SC_BK12_14);
        SC_W2BYTE(REG_SC_BK12_5A_L, _stAutoAdcSetting.u16SC_BK12_1A);
        SC_W2BYTE(REG_SC_BK12_5B_L, _stAutoAdcSetting.u16SC_BK12_1B);

        SC_W2BYTE(REG_SC_BK06_01_L, _stAutoAdcSetting.u16SC_BK06_01);
        SC_W2BYTE(REG_SC_BK06_21_L, _stAutoAdcSetting.u16SC_BK06_21);
        SC_W2BYTE(REG_SC_BK12_07_L, _stAutoAdcSetting.u16SC_BK12_07);
        SC_W2BYTE(REG_SC_BK12_44_L, _stAutoAdcSetting.u16SC_BK12_44);
        SC_W2BYTE(REG_SC_BK12_47_L, _stAutoAdcSetting.u16SC_BK12_47);
    }
    else
    {
        MDrv_Write2Byte(L_BK_IP1F2(0x02), _stAutoAdcSetting.u16SC_BK1_02);
        MDrv_Write2Byte(L_BK_IP1F2(0x03), _stAutoAdcSetting.u16SC_BK1_03);
        MDrv_Write2Byte(L_BK_IP1F2(0x04), _stAutoAdcSetting.u16SC_BK1_04);
        MDrv_Write2Byte(L_BK_IP1F2(0x05), _stAutoAdcSetting.u16SC_BK1_05);
        MDrv_Write2Byte(L_BK_IP1F2(0x06), _stAutoAdcSetting.u16SC_BK1_06);
        MDrv_Write2Byte(L_BK_IP1F2(0x07), _stAutoAdcSetting.u16SC_BK1_07);
        MDrv_Write2Byte(L_BK_IP1F2(0x0E), _stAutoAdcSetting.u16SC_BK1_0E);

        MDrv_WriteByte(BK_SELECT_00, REG_BANK_SCMI);
        MDrv_Write2Byte(L_BK_SCMI(0x01), _stAutoAdcSetting.u16SC_BK12_01);
        MDrv_Write2Byte(L_BK_SCMI(0x03), _stAutoAdcSetting.u16SC_BK12_03);
        MDrv_Write2Byte(L_BK_SCMI(0x04), _stAutoAdcSetting.u16SC_BK12_04);
        MDrv_Write2Byte(L_BK_SCMI(0x0E), _stAutoAdcSetting.u16SC_BK12_0E);
        MDrv_Write2Byte(L_BK_SCMI(0x0F), _stAutoAdcSetting.u16SC_BK12_0F);
        MDrv_Write2Byte(L_BK_SCMI(0x16), _stAutoAdcSetting.u16SC_BK12_16);
        MDrv_Write2Byte(L_BK_SCMI(0x17), _stAutoAdcSetting.u16SC_BK12_17);

        //scaling
        SC_W2BYTE(REG_SC_BK02_04_L, _stAutoAdcSetting.u16SC_BK02_04);
        SC_W2BYTE(REG_SC_BK02_05_L, _stAutoAdcSetting.u16SC_BK02_05);
        SC_W2BYTE(REG_SC_BK02_08_L, _stAutoAdcSetting.u16SC_BK02_08);
        SC_W2BYTE(REG_SC_BK02_09_L, _stAutoAdcSetting.u16SC_BK02_09);
        SC_W2BYTE(REG_SC_BK23_07_L, _stAutoAdcSetting.u16SC_BK23_07);
        SC_W2BYTE(REG_SC_BK23_08_L, _stAutoAdcSetting.u16SC_BK23_08);
        SC_W2BYTE(REG_SC_BK23_09_L, _stAutoAdcSetting.u16SC_BK23_09);
        SC_W2BYTE(REG_SC_BK23_0A_L, _stAutoAdcSetting.u16SC_BK23_0A);

        //DNR,OPM Base
        SC_W4BYTE(REG_SC_BK12_08_L, _stAutoAdcSetting.u32SC_BK12_08);
        SC_W4BYTE(REG_SC_BK12_0A_L, _stAutoAdcSetting.u32SC_BK12_0A);
        SC_W4BYTE(REG_SC_BK12_0C_L, _stAutoAdcSetting.u32SC_BK12_0C);
        SC_W4BYTE(REG_SC_BK12_10_L, _stAutoAdcSetting.u32SC_BK12_10);
        SC_W4BYTE(REG_SC_BK12_12_L, _stAutoAdcSetting.u32SC_BK12_12);
        SC_W4BYTE(REG_SC_BK12_14_L, _stAutoAdcSetting.u32SC_BK12_14);
        SC_W2BYTE(REG_SC_BK12_1A_L, _stAutoAdcSetting.u16SC_BK12_1A);
        SC_W2BYTE(REG_SC_BK12_1B_L, _stAutoAdcSetting.u16SC_BK12_1B);

        SC_W2BYTE(REG_SC_BK06_01_L, _stAutoAdcSetting.u16SC_BK06_01);
        SC_W2BYTE(REG_SC_BK06_21_L, _stAutoAdcSetting.u16SC_BK06_21);
        SC_W2BYTE(REG_SC_BK12_07_L, _stAutoAdcSetting.u16SC_BK12_07);
        SC_W2BYTE(REG_SC_BK12_44_L, _stAutoAdcSetting.u16SC_BK12_44);
        SC_W2BYTE(REG_SC_BK12_47_L, _stAutoAdcSetting.u16SC_BK12_47);
    }

    MDrv_WriteByteMask(REG_ADC_ATOP_5C_L, 0x00,0x70);

    MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    MDrv_WriteByte(L_BK_VOP(0x19),_stAutoAdcSetting.u8L_SC_BK10_19  );

    MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

MS_BOOL Hal_ADC_is_scart_rgb(void)
{
    MS_BOOL bRGB;
    MS_U8 u8Flag;

    u8Flag = MDrv_ReadByte(REG_ADC_ATOP_45_H);

    if((u8Flag & 0x50) == 0x10)
        bRGB = TRUE;
    else
        bRGB = FALSE;

    MDrv_WriteByteMask(REG_ADC_ATOP_45_H, 0x20, 0x20);

    return bRGB;
}

MS_U16 Hal_ADC_get_clk (void)
{
    //u16Value -= 3; // actual - 3
 //ADC PLL divider ratio (htotal-3), (write sequence LSB -> MSB)
    return (R2BYTEMSK(REG_ADC_DTOP_00_L, 0xFFFF)+3);//(REG_ADC_DTOP_00_L, u16Value);
}

MS_BOOL Hal_ADC_set_SoG_Calibration(void)
{
    // Reset SoG Online/Offline calibration depend on chip
    return TRUE;
}

/******************************************************************************/
///This function return SOG level range
///@param u32Min \b OUT: min of SOG level
///@param u32Max \b OUT: max of SOG level
///@param u32Recommend_value \b OUT: recommend value
/******************************************************************************/
void Hal_ADC_get_SoG_LevelRange(MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value)
{
    *u32Min = 0;
    *u32Max = 0;
    *u32Recommend_value = 0;
}

/******************************************************************************/
///This function Set SOG Level
///@param u32Value \b IN: set SOG value
/******************************************************************************/
void Hal_ADC_set_SoG_Level(MS_U32 u32Value)
{
    UNUSED(u32Value);
}

/******************************************************************************/
///select RGB input pipe delay, this reg will decide the H start of SCART RGB
///@param u32Value \b IN: set PIPE Delay value
/******************************************************************************/
void Hal_ADC_set_RGB_PIPE_Delay(MS_U8 u8Value)
{
    W2BYTEMSK(REG_ADC_ATOP_43_L, (u8Value<<8), 0x7F00);
}

/******************************************************************************/
///This function set Scart RGB Sync on Green clamp delay.
///@param u16Value \b IN: set clamp delay value
/******************************************************************************/
void Hal_ADC_set_ScartRGB_SOG_ClampDelay(MS_U16 u16Clpdly, MS_U16 u16Caldur)
{
    return;
    W2BYTEMSK(REG_ADC_DTOP_17_L, u16Clpdly, 0x0FFF);
    W2BYTEMSK(REG_ADC_DTOP_18_L, u16Caldur, 0x00FF);
}

/******************************************************************************/
///This function set YPbPr Loose LPF.
///@param benable \b IN: enable or disable
/******************************************************************************/
void Hal_ADC_set_YPbPrLooseLPF(MS_BOOL benable)
{
    //Obsolate in u4
}

/******************************************************************************/
///This function set SOG BW
///@param u16value \b IN: value of SOG BW
/******************************************************************************/
void Hal_ADC_Set_SOGBW(MS_U16 u16value)
{
    u16value = u16value;
}

/******************************************************************************/
///Set negative clamping duration..
///@param u16Value \b IN: set clamp delay value
/******************************************************************************/
void Hal_ADC_dtop_iClampDuration_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_18_L, u16value, LBMASK);
}

/******************************************************************************/
///Set postive clamping duration..
///@param u16Value \b IN: set clamp delay value
/******************************************************************************/
void Hal_ADC_dtop_vClampDuration_setting(MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_08_L, u16value, LBMASK);
}
#endif

#undef MHAL_ADC_C

