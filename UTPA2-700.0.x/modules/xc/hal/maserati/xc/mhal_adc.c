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
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Internal Definition
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_dvi_atop.h"//alex_tung
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
#include "hwreg_adc_dtopb.h"
#include "hwreg_hdmi.h"
#include "hwreg_ipmux.h"
#include "apiXC.h"
#include "apiXC_v2.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "hwreg_sc.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"

// include ADC table.
#include "mhal_adctbl.h"
#include "mhal_adctbl.c"
#include "mhal_adc.h"
#include "mhal_ip.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define MHAL_ADC_DBG(x)  //x
#define MHAL_ADC_EFUSE_RETRY_THRESHOLD 50
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

static MS_U16 u16ADC_DTOP_24_Buffer = 0;
static MS_U16 u16ADC_DTOP_2E_Buffer = 0;
static MS_U16 u16ADC_DTOP_38_Buffer = 0;

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

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL Hal_ADC_GetBandgapTrimData(void *pInstance, MS_U8 *pu8BandgapTrimData);
static MS_BOOL Hal_ADC_GetIDacTrimData(void *pInstance, MS_U16 u16mode, MS_U8 *pu8iDacTrimData);
static MS_BOOL Hal_ADC_LoadIDacTrimData(void *pInstance, MS_U16 u16mode);
static MS_BOOL Hal_ADC_External_eFuse(void);

/******************************************************************************/
/// Table dump area
/******************************************************************************/
static void Hal_ADC_LoadTable(void *pInstance, TAB_Info* pTab_info)
{
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U8 u8DoNotSet;
    MS_U16 i;

    if (pTab_info->pTable == NULL || pTab_info->u8TabRows == 0 || pTab_info->u8TabRows == 1)
        return;

    if (REG_ADDR_SIZE+REG_MASK_SIZE+pTab_info->u8TabIdx*REG_DATA_SIZE >= pTab_info->u8TabCols)
    {
        MHAL_ADC_DBG(printf("Tab_info error\n"));
        return;
    }

    for (i=0; i<pTab_info->u8TabRows-1; i++)
    {
         u32Addr =  (MS_U32)( (pTab_info->pTable[0]<<8) + pTab_info->pTable[1] );

         // DRV_ADC_REG(REG_ADC_DTOPB_FE_L)

         if ( pTab_info->pTable[0] == 0x25 ) // ATOP
            u32Addr = u32Addr | ( REG_ADC_ATOP_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x26 ) // DTOP
            u32Addr = u32Addr | ( REG_ADC_DTOP_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x12 ) // DTOPB
            u32Addr = u32Addr | ( REG_ADC_DTOPB_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x3D ) // ATOPB
            u32Addr = u32Addr | ( REG_ADC_ATOPB_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x35 ) // AFEC
            u32Addr = u32Addr | ( REG_AFEC_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x36 ) // COMB
            u32Addr = u32Addr | ( REG_COMB_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x1E ) // CHIPTOP
            u32Addr = u32Addr | ( REG_ADC_CHIPTOP_BASE & 0xFFFF0000 );
         else if ( pTab_info->pTable[0] == 0x0B ) // CLKGEN0
            u32Addr = u32Addr | ( REG_CLKGEN0_BASE & 0xFFFF0000 );

         if(u32Addr == REG_ADC_DTOPB_FE_L)
         {
            // delay only, skip to write next register
            u8Value = pTab_info->pTable[REG_ADDR_SIZE+REG_MASK_SIZE+pTab_info->u8TabIdx*REG_DATA_SIZE + 1];
            MHAL_ADC_DBG(printf("ADC tbl delay (%d) ms \n",u8Value));
            OS_DELAY_TASK(u8Value);
            goto NEXT;
         }

         u8Mask  = pTab_info->pTable[2];
         u8DoNotSet = pTab_info->pTable[REG_ADDR_SIZE+REG_MASK_SIZE+pTab_info->u8TabIdx*REG_DATA_SIZE];
         u8Value = pTab_info->pTable[REG_ADDR_SIZE+REG_MASK_SIZE+pTab_info->u8TabIdx*REG_DATA_SIZE + 1];

         //printf("[addr=%06lx, msk=%02x, enb=%02x val=%02x]\n", u32Addr, u8Mask,u8DoNotSet, u8Value);

         if ( !u8DoNotSet )
         {
             if (u32Addr&0x1)
                W2BYTEMSK((MS_U32)(u32Addr &0xFFFFFE), (MS_U16)u8Value<<8, (MS_U16)u8Mask<<8);
             else
                W2BYTEMSK((MS_U32)u32Addr, (MS_U16)u8Value, (MS_U16)u8Mask);
         }

NEXT:
         pTab_info->pTable+=pTab_info->u8TabCols; // next
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/******************************************************************************/
///ADC soft reset
/******************************************************************************/
void Hal_ADC_reset(void *pInstance, MS_U16 u16Reset)
{
    //[4]: Soft-reset ATOP
    //[3]: Soft-reset PLLB
    //[2]: Soft-reset ADCB
    //[1]: Soft-reset PLLA
    //[0]: Soft-reset ADCA

    // Remove ADC reset after T3
    /*
        W2BYTE(REG_DVI_ATOP_07_L, u16Reset);
        OS_DELAY_TASK(2);
        W2BYTE(REG_DVI_ATOP_07_L, 0x0000);
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
void Hal_ADC_ext_clk_en(void *pInstance, MS_BOOL benable)
{
    W2BYTEMSK(REG_ADC_ATOP_60_L, (benable ? BIT(7):0), BIT(7));
}

//=========================================================//
// Function : Hal_ADC_hdmi_vco_ctrl
// Description:
//=========================================================//
void Hal_ADC_hdmi_vco_ctrl(void *pInstance, MS_U16 u16InputClock)
{
    // Obsolate in T3
}
//=========================================================//
// Function : Hal_ADC_vco_ctrl
// Description:
//=========================================================//
void Hal_ADC_vco_ctrl(void *pInstance, MS_U16 u16InputClock)
{
    //Obsolate in T3
}

void Hal_ADC_set_vco_ctrl(void *pInstance, MS_BOOL bIsDVIPort, MS_U16 u16InputClock)
{
    //Obsolete
    UNUSED(bIsDVIPort);
    UNUSED(u16InputClock);
}


/******************************************************************************/
///This function sets PLL clock divider ratio
///@param u16Value \b IN: PLL clock divider ratio
/******************************************************************************/
void Hal_ADC_dtop_clk_setting ( void *pInstance, MS_U16 u16Value )
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
MS_U16 Hal_ADC_get_phase_range(void *pInstance)
{
    // Get phase range according to reg_adc_pll_mod
    // reg_adc_pll_mod == 0 ? 128 :
    // reg_adc_pll_mod == 1 ? 256 :
    // reg_adc_pll_mod == 2 ? 512 : 1024;
    return 128 << (R2BYTEMSK(REG_ADC_ATOP_09_L,BMASK(12:11)) >> 11);
}

MS_U8 Hal_ADC_get_phase(void *pInstance)
{
    //FIXME: Change API
   return ( (MS_U8) R2BYTEMSK(REG_ADC_ATOP_15_L, BMASK(9:0)));
}

MS_U16 Hal_ADC_get_phaseEx(void *pInstance)
{
    MS_U16 u16ADCPllMod = R2BYTEMSK(REG_ADC_ATOP_09_L,BMASK(12:11)) >> 11;

   return R2BYTEMSK(REG_ADC_ATOP_15_L,BMASK((6+u16ADCPllMod):0));
}

/******************************************************************************/
///This function sets PLL phase
///@param u8Value \b IN: PLL phase divider ratio
/******************************************************************************/
void Hal_ADC_set_phase( void *pInstance, MS_U8 u8Value )
{
    //FIXME: Change API
    W2BYTEMSK(REG_ADC_ATOP_15_L, u8Value, BMASK(9:0));
    W2BYTEMSK(REG_ADC_ATOP_15_L, u8Value+1, BMASK(9:0));
    W2BYTEMSK(REG_ADC_ATOP_15_L, u8Value, BMASK(9:0));
}

void Hal_ADC_set_phaseEx( void *pInstance, MS_U16 u16Value )
{
    MS_U16 u16ADCPllMod = R2BYTEMSK(REG_ADC_ATOP_09_L,BMASK(12:11)) >> 11;
    MS_U16 u16Mask = BMASK((6+u16ADCPllMod):0) ;

    W2BYTEMSK(REG_ADC_ATOP_15_L, u16Value, u16Mask);
    W2BYTEMSK(REG_ADC_ATOP_15_L, u16Value+1, u16Mask);
    W2BYTEMSK(REG_ADC_ATOP_15_L, u16Value, u16Mask);
}

/******************************************************************************/
///This function sets ADC offset
///@param *pstADCSetting \b IN: pointer to ADC settings
/******************************************************************************/
void Hal_ADC_offset_setting ( void *pInstance, XC_AdcGainOffsetSetting *pstADCSetting  )
{
    Hal_ADC_SetOffsetR(pInstance, pstADCSetting->u16RedOffset);
    Hal_ADC_SetOffsetG(pInstance, pstADCSetting->u16GreenOffset);
    Hal_ADC_SetOffsetB(pInstance, pstADCSetting->u16BlueOffset);
}

/******************************************************************************/
///This function sets ADC gain
///@param *pstADCSetting \b IN: pointer to ADC settings
/******************************************************************************/
void Hal_ADC_gain_setting ( void *pInstance, XC_AdcGainOffsetSetting *pstADCSetting  )
{
    Hal_ADC_SetGainR(pInstance, pstADCSetting->u16RedGain);
    Hal_ADC_SetGainG(pInstance, pstADCSetting->u16GreenGain);
    Hal_ADC_SetGainB(pInstance, pstADCSetting->u16BlueGain);
}

/******************************************************************************/
///This function enable/disable output double buffer
///@param bEnable \b IN:
///- Enable: Turn on ADC double buffer
///- Disable: Turn off ADC double buffer
/******************************************************************************/
void Hal_ADC_doublebuffer_setting(void *pInstance, MS_BOOL bEnable)
{
    //Obsolate in T3
}

/******************************************************************************/
///This function recalibrates ADC offset. This function should be called
///after mode changed.
///@param bFlag \b IN:
///- 0: Turn on
///- 1: Turn off
/******************************************************************************/
void Hal_ADC_dtop_calibration_target_setting(void *pInstance, MS_BOOL bIsYPbPrFlag)
{
    //FIXME: T3 has new calibration algorithm

}
void Hal_ADC_dtop_sw_mode_setting(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsAutoSWMode)
{
#if 0
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCAdcCal_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_AdcCal_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;

    MHAL_ADC_DBG(printf("ADC Tbl:auto offset\n");)
    Hal_ADC_LoadTable(&Tab_info);
    return;
#endif
}


static void _Hal_ADC_Set_RGBYPbPr_Calibration(void *pInstance)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    TAB_Info Tab_info;
    if (pXCResourcePrivate->sthal_ADC._bEnableHWCalibration == ENABLE)
    {
        Tab_info.pTable = (void*)MST_ADCAdcCal_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_AdcCal_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_TBL)/Tab_info.u8TabCols;
        if(pXCResourcePrivate->sthal_ADC._bUseBandgap)
            Tab_info.u8TabIdx = ADC_TABLE_AdcCal_BG_Fix_UG;
        else
            Tab_info.u8TabIdx = ADC_TABLE_AdcCal_Fix_UG;
    }
    else
    {
        Tab_info.pTable = (void*)MST_ADCAdcCal_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_AdcCal_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_TBL)/Tab_info.u8TabCols;
        Tab_info.u8TabIdx = ADC_TABLE_AdcCal_SW_UG;
    }

    if(pXCResourcePrivate->sthal_ADC._bUseBandgap)
    {
        MS_U8 efuseBandgap = 0;
        if(Hal_ADC_GetBandgapTrimData(pInstance, &efuseBandgap))
        {
            //printf("%s: efuseBandgap = 0x%x\n", __FUNCTION__, efuseBandgap & 0x1F);
            // Write to ATOP register
            W2BYTEMSK(REG_ADC_ATOP_5B_L , BIT(13), BIT(13));

            if(Hal_ADC_External_eFuse())
            {
                MHAL_ADC_DBG(printf("ADC eFuse: Just let bandgap be overwrite by eFuse, no change bandgap efuse value which be wrote by mboot\n"););
            }
            else
            {
                W2BYTEMSK(REG_ADC_ATOP_5B_L ,((MS_U16)efuseBandgap & 0x1F) << 8, BMASK(12:8) );
            }
        }
        else
        {
             printf("[ERROR] Invalid bandgap value in eFuse !\n");
        }
    }
    else
    {
        // Use LDO
        W2BYTEMSK(REG_ADC_ATOP_5B_L , 0, BIT(13));
    }

    Hal_ADC_LoadTable(pInstance, &Tab_info);

    return;
}

static void _Hal_ADC_Set_AV_Calibration(void *pInstance)
{
    MS_U16 reg = 0;

    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCAdcCal_AV_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_AdcCal_AV_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_AV_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;

    // Pre-charge 1.5V capacitor by short CLPZ
    reg = R2BYTEMSK(REG_AFEC_BASE+0x94 , 0xFFFF );
    W2BYTEMSK(REG_AFEC_BASE+0x94 , reg | BIT(5) | BIT(6) , 0xFFFF );
    // Patch for Napoli
    MHAL_ADC_DBG(printf("ADC Tbl: Set AV Cal - Set reg_ref_vclp_vd_sel  0.8V (for AV calibration)\n"););
    W2BYTEMSK(REG_ADC_ATOP_36_L , 0x0000, 0xE000 );

    Hal_ADC_LoadTable(pInstance, &Tab_info);

    // Patch for Napoli
    MHAL_ADC_DBG(printf("ADC Tbl: Set AV Cal - Set reg_ref_vclp_vd_sel  1.5V (VD Mode)\n"););
    W2BYTEMSK(REG_ADC_ATOP_36_L , 0x8000, 0xE000 );
    // Put CLPZ to Open
    reg = R2BYTEMSK(REG_AFEC_BASE+0x94 , ~(BIT(5) | BIT(6)) );
    W2BYTEMSK(REG_AFEC_BASE+0x94 , reg , 0xFFFF );

    return;
}

static void _Hal_ADC_Set_SV_Calibration(void *pInstance)
{
    MS_U16 reg = 0;

    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCAdcCal_SV_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_AdcCal_SV_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_SV_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;

    // Pre-charge 1.5V capacitor by short CLPZ
    reg = R2BYTEMSK(REG_AFEC_BASE+0x94 , 0xFFFF );
    W2BYTEMSK(REG_AFEC_BASE+0x94 , reg | BIT(5) | BIT(6) , 0xFFFF );
    // Patch for Napoli
    MHAL_ADC_DBG(printf("ADC Tbl: Set SV Cal - Set reg_ref_vclp_vd_sel  0.8V (for SV calibration)\n"););
    W2BYTEMSK(REG_ADC_ATOP_36_L , 0x0000, 0xE000 );

    Hal_ADC_LoadTable(pInstance, &Tab_info);

    // Patch for Napoli
    MHAL_ADC_DBG(printf("ADC Tbl: Set SV Cal - Set reg_ref_vclp_vd_sel  1.5V (VD Mode)\n"););
    W2BYTEMSK(REG_ADC_ATOP_36_L , 0x8000, 0xE000 );
    // Put CLPZ to Open
    reg = R2BYTEMSK(REG_AFEC_BASE+0x94 , ~(BIT(5) | BIT(6)) );
    W2BYTEMSK(REG_AFEC_BASE+0x94 , reg , 0xFFFF );

    return;
}


static ADC_SOURCE_TYPE _Hal_XC_ADC_InputToSrcType(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput)
{
    ADC_SOURCE_TYPE enADCSrcType;

    switch(enADCInput)
    {
        case ADC_INPUTSOURCE_ONLY_RGB:
            enADCSrcType = ADC_TABLE_SOURCE_RGB;
            break;

        case ADC_INPUTSOURCE_ONLY_YPBPR:
            enADCSrcType = ADC_TABLE_SOURCE_YUV;
            break;

        case ADC_INPUTSOURCE_ONLY_MVOP:
            enADCSrcType = ADC_TABLE_SOURCE_MVOP;
            break;

        case ADC_INPUTSOURCE_ONLY_DVI:
            enADCSrcType = ADC_TABLE_SOURCE_DVI;
            break;

        case ADC_INPUTSOURCE_ONLY_ATV:
            enADCSrcType = ADC_TABLE_SOURCE_ATV;
            break;

        case ADC_INPUTSOURCE_ONLY_SVIDEO:
            enADCSrcType = ADC_TABLE_SOURCE_SVIDEO;
            break;

        case ADC_INPUTSOURCE_ONLY_SCART:
            enADCSrcType = ADC_TABLE_SOURCE_SCART;
            break;

        case ADC_INPUTSOURCE_ONLY_CVBS:
            enADCSrcType = ADC_TABLE_SOURCE_CVBS;
            break;

        default:
            enADCSrcType = ADC_TABLE_SOURCE_NUMS;
            break;
    }

    return enADCSrcType;
}

typedef enum {
    HAL_ADC_SCART_PATCH_SWITCH_RGB,
    HAL_ADC_SCART_PATCH_SWITCH_SV,
} HAL_ADC_SCART_PATCH_SWITCH;

static void _Hal_ADC_SCART_SV_RGB_switch(void *pInstance, HAL_ADC_SCART_PATCH_SWITCH type)
{
    switch(type)
    {
        case HAL_ADC_SCART_PATCH_SWITCH_RGB:
            W2BYTEMSK(REG_ADC_ATOP_00_L, 0, BIT(4)); // reg_vd_rgb_en
            W2BYTEMSK(REG_ADC_ATOP_01_L, BIT(14), BIT(14)); // reg_mux_rgb_en
            W2BYTEMSK(REG_ADC_ATOP_01_L, 0, (BIT(4)|BIT(5))); // reg_mux_rsel
            W2BYTEMSK(REG_ADC_ATOP_02_L, 0, BIT(9)); // reg_mux_c_en
            W2BYTEMSK(REG_ADC_ATOP_04_L, BIT(12), BIT(12)); // reg_pdn_iclp_vdc
            W2BYTEMSK(REG_ADC_ATOP_38_L, 0, BIT(1)); // reg_vlpf_c_en
            break;
        case HAL_ADC_SCART_PATCH_SWITCH_SV:
            W2BYTEMSK(REG_ADC_ATOP_00_L, BIT(4), BIT(4)); // reg_vd_rgb_en
            W2BYTEMSK(REG_ADC_ATOP_01_L, 0, BIT(14)); // reg_mux_rgb_en
            W2BYTEMSK(REG_ADC_ATOP_01_L, BIT(4), (BIT(4)|BIT(5))); // reg_mux_rsel
            W2BYTEMSK(REG_ADC_ATOP_02_L, BIT(9), BIT(9)); // reg_mux_c_en
            W2BYTEMSK(REG_ADC_ATOP_04_L, 0, BIT(12)); // reg_pdn_iclp_vdc
            W2BYTEMSK(REG_ADC_ATOP_38_L, BIT(1), BIT(1)); // reg_vlpf_c_en
            break;
        default:
            MHAL_ADC_DBG(printf("_Hal_ADC_SCART_SV_RGB_switch: Unknow type\n"););
    }
}

void Hal_ADC_Set_Source_Calibration(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    ADC_SOURCE_TYPE enADCSrcType = _Hal_XC_ADC_InputToSrcType(pInstance, enADC_SourceType);

    switch (enADCSrcType)
    {
        case ADC_TABLE_SOURCE_RGB:
        case ADC_TABLE_SOURCE_YUV:
        case ADC_TABLE_SOURCE_RGB_DVI:
        case ADC_TABLE_SOURCE_RGB_MV:
        case ADC_TABLE_SOURCE_YUV_DVI:
        case ADC_TABLE_SOURCE_YUV_MV:
            _Hal_ADC_Set_RGBYPbPr_Calibration(pInstance);
            break;
        case ADC_TABLE_SOURCE_ATV:
        case ADC_TABLE_SOURCE_CVBS:
        case ADC_TABLE_SOURCE_MVOP_AV:
        case ADC_TABLE_SOURCE_DVI_CVBS:
            _Hal_ADC_Set_AV_Calibration(pInstance);
            break;
        case ADC_TABLE_SOURCE_SVIDEO:
        case ADC_TABLE_SOURCE_MVOP_SV:
        case ADC_TABLE_SOURCE_DVI_SV:
            _Hal_ADC_Set_AV_Calibration(pInstance);
            _Hal_ADC_Set_SV_Calibration(pInstance);
            break;
        case ADC_TABLE_SOURCE_SCART:
            // stop ISOG reset to prevent garbage on G channel
            Hal_ADC_ISOG_SetResetWidth(pInstance, 0);

            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_ADC_AUTO_SCART_SV)
            {
                if(Hal_ADC_is_scart_rgb(pInstance))
                {
                    MHAL_ADC_DBG(printf("============ SCART RGB Calibration =========\n");)
                        _Hal_ADC_SCART_SV_RGB_switch(pInstance, HAL_ADC_SCART_PATCH_SWITCH_RGB);
                    // stop ISOG reset to prevent garbage on G channel
                    //Hal_ADC_ISOG_SetResetWidth(pInstance, 0);
                    _Hal_ADC_Set_RGBYPbPr_Calibration(pInstance);
                    _Hal_ADC_Set_AV_Calibration(pInstance);
                }
                else
                {
                    MHAL_ADC_DBG(printf("============ SCART SV/AV Calibration =========\n");)
                        _Hal_ADC_SCART_SV_RGB_switch(pInstance, HAL_ADC_SCART_PATCH_SWITCH_SV);

                    _Hal_ADC_Set_AV_Calibration(pInstance);
                    _Hal_ADC_Set_SV_Calibration(pInstance);
                }
            }
            else
            {
                _Hal_ADC_Set_RGBYPbPr_Calibration(pInstance);
                _Hal_ADC_Set_AV_Calibration(pInstance);
            }
            break;
        case ADC_TABLE_SOURCE_RGB_SC:
        case ADC_TABLE_SOURCE_RGB_AV:
        case ADC_TABLE_SOURCE_YUV_SC:
        case ADC_TABLE_SOURCE_YUV_AV:
        case ADC_TABLE_SOURCE_MVOP_SC:
        case ADC_TABLE_SOURCE_DVI_SC:
            _Hal_ADC_Set_RGBYPbPr_Calibration(pInstance);
            _Hal_ADC_Set_AV_Calibration(pInstance);
            break;
        default:
        // ADC_TABLE_SOURCE_DVI
        // ADC_TABLE_SOURCE_MVOP
        // ADC_TABLE_SOURCE_MVOP_DVI
        // ADC_TABLE_SOURCE_NUMS
            break;
    }
}


/******************************************************************************/
///This function sets clamp placement
///@param u8Value \b IN:
/******************************************************************************/
void Hal_ADC_clamp_placement_setting(void *pInstance, MS_U16 u16InputClockMHz)
{
    /* Vclamp_dly */
    if(u16InputClockMHz>= 40)  //~= HD resolution likes 1280*720*50/10^6
    {
        W2BYTEMSK(REG_ADC_DTOP_0B_L, 0x38, LBMASK);
    }
    else
    {
        W2BYTEMSK(REG_ADC_DTOP_0B_L, 0x08, LBMASK);
    }
}

void Hal_XC_ADC_Set_VClamp_level(void *pInstance, ADC_VClamp_Level_Type type)
{
    //FIXME: T3 has new calibration algorithm
    return;
}

/******************************************************************************/
///This function sets input HSync polarity
///@param u8Value \b IN:
/******************************************************************************/
void Hal_ADC_hpolarity_setting(void *pInstance, MS_BOOL bHightActive)
{
    W2BYTEMSK(REG_ADC_DTOP_07_L, (bHightActive ? BIT(7):0), BIT(7));
}

/******************************************************************************/
///This function power off ADC
/******************************************************************************/
void Hal_ADC_poweroff(void *pInstance)
{
    //Obsolate in T3
}
//----------------------------------------------------------------------
//  Set RGB Gain setting
//----------------------------------------------------------------------
void Hal_ADC_SetGainR(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_51_L, u16value, 0x3FFF);
}
void Hal_ADC_SetGainG(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_56_L, u16value, 0x3FFF);
}
void Hal_ADC_SetGainB(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_5B_L, u16value, 0x3FFF);
}
//----------------------------------------------------------------------
//  Set RGB Offset setting
//----------------------------------------------------------------------

void Hal_ADC_SetOffsetR(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_52_L, u16value, 0x1FFF);
}
void Hal_ADC_SetOffsetG(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_57_L, u16value, 0x1FFF);
}
void Hal_ADC_SetOffsetB(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_5C_L, u16value, 0x1FFF);
}

//----------------------------------------------------------------------
//  Get RGB Gain setting
//----------------------------------------------------------------------
MS_U16 Hal_ADC_GetGainR(void *pInstance)
{
    MS_U16 u16Value = R2BYTEMSK(REG_ADC_DTOP_51_L, ((1 << ADC_GAIN_BIT_CNT) - 1));
    return u16Value;
}
MS_U16 Hal_ADC_GetGainG(void *pInstance)
{
    MS_U16 u16Value = R2BYTEMSK(REG_ADC_DTOP_56_L, ((1 << ADC_GAIN_BIT_CNT) - 1));
    return u16Value;
}
MS_U16 Hal_ADC_GetGainB(void *pInstance)
{
    MS_U16 u16Value = R2BYTEMSK(REG_ADC_DTOP_5B_L, ((1 << ADC_GAIN_BIT_CNT) - 1));
    return u16Value;
}

//----------------------------------------------------------------------
//  Get RGB Offset setting
//----------------------------------------------------------------------

MS_U16 Hal_ADC_GetOffsetR(void *pInstance)
{
    MS_U16 u16Value = R2BYTEMSK(REG_ADC_DTOP_52_L, ((1 << ADC_OFFSET_BIT_CNT) - 1));
    return u16Value;
}
MS_U16 Hal_ADC_GetOffsetG(void *pInstance)
{
    MS_U16 u16Value = R2BYTEMSK(REG_ADC_DTOP_57_L, ((1 << ADC_OFFSET_BIT_CNT) - 1));
    return u16Value;
}
MS_U16 Hal_ADC_GetOffsetB(void *pInstance)
{
    MS_U16 u16Value = R2BYTEMSK(REG_ADC_DTOP_5C_L, ((1 << ADC_OFFSET_BIT_CNT) - 1));
    return u16Value;
}

//----------------------------------------------------------------------
// Internal calibration related.
//----------------------------------------------------------------------
void Hal_ADC_dtop_internaldc_setting(void *pInstance, ADC_Internal_Voltage InternalVoltage)
{
    MS_U16 u16regvalue = 0;
#if 0 // Ref
    switch ( InternalVoltage )
    {

        case E_ADC_Internal_0_1V: //!! Mapping to T3 0.55V
             u16regvalue = 0xB00;
        break;

        case E_ADC_Internal_0_6V: //!! Mapping to T3 1.05V
             u16regvalue = 0xF00;
        break;

        case E_ADC_Internal_0V:  // We do not need to tune offset in T3 ( using iclamping )
        case E_ADC_Internal_None:
        default:
            W2BYTEMSK(REG_ADC_DTOPB_04_L, 0x00 , BMASK(12:10) );
            //Ref VCal disable
            W2BYTEMSK(REG_ADC_DTOPB_05_L, 0x00 , BIT(12) );
            return;
        break;
    }

    W2BYTEMSK(REG_ADC_DTOPB_04_L, BMASK(12:10) , BMASK(12:10) );
    //Ref VCal enable
    W2BYTEMSK(REG_ADC_DTOPB_05_L, BIT(12) , BIT(12) );

    //Ref Voltage - ADCA VCAL force value
    W2BYTEMSK(REG_ADC_DTOPB_05_L, u16regvalue , BMASK(11:8) );
#else // LDO

    switch ( InternalVoltage )
    {

        case E_ADC_Internal_0_1V: //!! Mapping to T3 0.55V
             u16regvalue = 0x2;
        break;

        case E_ADC_Internal_0_6V: //!! Mapping to T3 1.05V
             u16regvalue = 0x3;
        break;

        case E_ADC_Internal_0V:  // We do not need to tune offset in T3 ( using iclamping )
        case E_ADC_Internal_None:
        default:
            W2BYTEMSK(REG_ADC_ATOP_5A_L, 0x00 , BIT(0) );
            // LDO VCal disable
            W2BYTEMSK(REG_ADC_DTOPB_05_L, 0x00 , BMASK(3:0) );

            W2BYTEMSK(REG_ADC_DTOPB_04_L, 0x00 , BMASK(12:10) );

            W2BYTEMSK(REG_ADC_DTOPB_05_L, 0x00 , BMASK(12:8) );
            return;
        break;
    }

    W2BYTEMSK(REG_ADC_DTOPB_04_L, BMASK(12:10) , BMASK(12:10) );

    W2BYTEMSK(REG_ADC_DTOPB_05_L, BIT(8) | BIT(12) , BIT(8) | BIT(12) );

    W2BYTEMSK(REG_ADC_ATOP_5A_L, BIT(0) , BIT(0) );

    // LDO VCal enable
    W2BYTEMSK(REG_ADC_DTOPB_05_L, BMASK(3:2) , BMASK(3:2) );

    //Ref Voltage - ADCA VCAL force value
    W2BYTEMSK(REG_ADC_DTOPB_05_L, u16regvalue , BMASK(1:0) );

#endif
}
static MS_U8 Hal_ADC_SearchFreqSetTableIndex(void *pInstance, MS_U16 u16OriginalPixClk)
{
    MS_U8  u8ClkIndex;
    for(u8ClkIndex=0; u8ClkIndex<sizeof(MST_ADC_FreqRange_TBL)/sizeof(ADC_FREQ_RANGE); u8ClkIndex++)
    {
        if((u16OriginalPixClk < MST_ADC_FreqRange_TBL[u8ClkIndex].FreqHLimit) &&
            (u16OriginalPixClk >= MST_ADC_FreqRange_TBL[u8ClkIndex].FreqLLimit))
            break;
    }
    return u8ClkIndex;
}

static void Hal_ADC_SetADCBwFilter(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput,MS_U16 u16PixelClk)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    TAB_Info Tab_info;
    if ( enADCInput & ADC_INPUTSOURCE_ONLY_YPBPR ) // YPbPr
    {
        Tab_info.pTable = (void*)MST_ADCSetModeYUV_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCSetModeYUV_TBL)/Tab_info.u8TabCols;

    }
    else if ((HAL_SC_GetVGASogEn(pInstance))
             && ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L) & 0x70)==0x70)
             && (enADCInput == ADC_INPUTSOURCE_ONLY_RGB))
    {
        //SOG
        Tab_info.pTable = (void*)MST_ADCSetModeRGB_SOG_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCSetModeRGB_SOG_TBL)/Tab_info.u8TabCols;
    }
    else // RGB
    {
        Tab_info.pTable = (void*)MST_ADCSetModeRGB_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCSetModeRGB_TBL)/Tab_info.u8TabCols;
    }

    Tab_info.u8TabIdx = Hal_ADC_SearchFreqSetTableIndex(pInstance, u16PixelClk);

    MHAL_ADC_DBG(printf("ADC Tbl:BwFilter PixelClk %d, TabIdx %d\n", u16PixelClk, Tab_info.u8TabIdx);)
    Hal_ADC_LoadTable(pInstance, &Tab_info);

    if (pXCResourcePrivate->sthal_ADC.bIsYPbPrLooseLPF && (enADCInput & ADC_INPUTSOURCE_ONLY_YPBPR))
    {
        Tab_info.pTable = (void*)MST_ADCSetModeYUV_Y_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_SetMode_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCSetModeYUV_Y_TBL)/Tab_info.u8TabCols;

        MHAL_ADC_DBG(printf("ADC Tbl: Enable YPbPr Loose LPF\n");)
        Hal_ADC_LoadTable(pInstance, &Tab_info);
    }

}

void Hal_ADC_ExitExternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE eADC_Source,XC_AdcGainOffsetSetting* InitialGainOffset)
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

MS_BOOL Hal_ADC_InitExternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE enAdcSource)
{
    MS_BOOL bRet = FALSE;
    if ( enAdcSource == ADC_INPUTSOURCE_ONLY_RGB )
    {
        bRet = TRUE;
    }
    else if ( enAdcSource == ADC_INPUTSOURCE_ONLY_YPBPR )
    {
        bRet = TRUE;
    }
    else if (enAdcSource == ADC_INPUTSOURCE_ONLY_SCART )
    {
        // Not YUV source. Clamping to 0v
        Hal_ADC_dtop_calibration_target_setting(pInstance, FALSE);
        Hal_ADC_dtop_sw_mode_setting(pInstance, TRUE, FALSE);
        W2BYTEMSK(REG_ADC_ATOP_42_L, 0x00 , BIT(5));
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong ADC input source %u!!\n", enAdcSource);
        bRet = FALSE;
    }
    return bRet;
}

void Hal_ADC_InitInternalCalibration(void *pInstance, ADC_INPUTSOURCE_TYPE enAdcSource, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == SUB_WINDOW)
    {
        //MS_U8 u8Bank;
        //u8Bank = MDrv_ReadByte(BK_SELECT_00);
        // Calibration in YUV or RGB 444 10 bit mode
        //MDrv_WriteByte( BK_SELECT_00, REG_BANK_SCMI);
        //MDrv_WriteByteMask(L_BK_SCMI(0x01), BIT(5), BIT(4) | BIT(5));
        //MDrv_WriteRegBit(L_BK_SCMI(0x03), FALSE, BIT(4));
        //MDrv_WriteRegBit(H_BK_SCMI(0x03), 0x0, (BIT(4)|BIT(5)));//disable mirror

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, BIT(5), BIT(4) | BIT(5));
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, FALSE, BIT(4));

        // P mode
        //MDrv_WriteByteMask(H_BK_SCMI(0x01), 0x01, BIT(0)|BIT(1)|BIT(2));
        //MDrv_WriteByteMask(H_BK_SCMI(0x01), 0x00, BIT(4)|BIT(5)|BIT(6));
        //MDrv_WriteRegBit(L_BK_SCMI(0x04), TRUE, BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, 0x0100, BIT(8)|BIT(9)|BIT(10));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, 0x0000, BIT(12)|BIT(13)|BIT(14));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, 0x02, BIT(1));

        // Disable user memory format
        //MDrv_WriteByteMask(H_BK_SCMI(0x02), 0x00, BIT(0)|BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_42_L, 0x0000, BIT(8)|BIT(9));

        //MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F2 );
        //MDrv_WriteByte(L_BK_IP1F2(0x0E), 0x11); // enable auto gain function
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, 0x0011); // enable auto gain function

        MDrv_Write2Byte(REG_ADC_ATOP_00_L ,0x0001 );

        // Disable NR
#if 0
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L, 0, BIT(1)|BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L, 0, BIT(1)|BIT(0));
#endif

        //framebuffer number
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, 0, BIT(6)|BIT(7));
        //    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(13));
        // Two frame mode
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L, 0x02,  BIT(3)|BIT(2)|BIT(1)|BIT(0));
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, 0, BIT(6)|BIT(7));
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(13));

        // init power
        MDrv_Write2Byte(REG_ADC_ATOP_04_L ,0xF800 );
        MDrv_Write2Byte(REG_ADC_ATOP_05_L ,0x0003 );
        MDrv_Write2Byte(REG_ADC_ATOP_06_L ,0xFB00 );

        // divider number of the post divider
        MDrv_WriteByteMask(REG_ADC_ATOP_09_H, 0x00 ,0x18 );

        // Set offset
        Hal_ADC_SetOffsetR(pInstance, 0x800);
        Hal_ADC_SetOffsetG(pInstance, 0x800);
        Hal_ADC_SetOffsetB(pInstance, 0x800);

        // Set black level
        W2BYTEMSK(REG_ADC_DTOP_50_L, 0x800, 0x0FFF);
        W2BYTEMSK(REG_ADC_DTOP_55_L, 0x800, 0x0FFF);
        W2BYTEMSK(REG_ADC_DTOP_5A_L, 0x800, 0x0FFF);

        //Set ADC input LPF to low bandwidth.
        W2BYTE(REG_ADC_ATOP_34_L,0xFFFF);
        W2BYTE(REG_ADC_ATOP_35_L,0x00FF);

        W2BYTEMSK( REG_IPMUX_01_L, 0xF0, 0xF0 );  //select pattern generator source
        if(enAdcSource == ADC_INPUTSOURCE_ONLY_SCART )
        {
            MDrv_Write2Byte(REG_ADC_ATOP_03_L ,0x0000 );
            MDrv_Write2Byte(REG_ADC_ATOP_5E_L ,0x0200);
            MDrv_WriteByte(L_BK_CHIPTOP(0x55),0x00);
        }
        //MDrv_WriteByte(H_BK_ADC_ATOP(0x1C), 0xF8);   // Turn on SOG input low bandwidth filter

        //MDrv_WriteByte(BK_SELECT_00, u8Bank);
    }
    else
    {
        //MS_U8 u8Bank;
        //u8Bank = MDrv_ReadByte(BK_SELECT_00);
        // Calibration in YUV or RGB 444 10 bit mode
        //MDrv_WriteByte( BK_SELECT_00, REG_BANK_SCMI);
        //MDrv_WriteByteMask(L_BK_SCMI(0x01), BIT(5), BIT(4) | BIT(5));
        //MDrv_WriteRegBit(L_BK_SCMI(0x03), FALSE, BIT(4));
        //MDrv_WriteRegBit(H_BK_SCMI(0x03), 0x0, (BIT(4)|BIT(5)));//disable mirror

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, BIT(5), BIT(4) | BIT(5));
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, FALSE, BIT(4));

        // P mode
        //MDrv_WriteByteMask(H_BK_SCMI(0x01), 0x01, BIT(0)|BIT(1)|BIT(2));
        //MDrv_WriteByteMask(H_BK_SCMI(0x01), 0x00, BIT(4)|BIT(5)|BIT(6));
        //MDrv_WriteRegBit(L_BK_SCMI(0x04), TRUE, BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, 0x0100, BIT(8)|BIT(9)|BIT(10));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, 0x0000, BIT(12)|BIT(13)|BIT(14));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, 0x02, BIT(1));

        // Disable user memory format
        //MDrv_WriteByteMask(H_BK_SCMI(0x02), 0x00, BIT(0)|BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0x0000, BIT(8)|BIT(9));

        //MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F2 );
        //MDrv_WriteByte(L_BK_IP1F2(0x0E), 0x11); // enable auto gain function
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, 0x0011); // enable auto gain function

        MDrv_Write2Byte(REG_ADC_ATOP_00_L ,0x0001 );

        // Disable NR
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L, 0, BIT(1)|BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L, 0, BIT(1)|BIT(0));

        //framebuffer number
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, 0, BIT(6)|BIT(7));
        //    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(13));
        // Two frame mode
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L, 0x02,  BIT(3)|BIT(2)|BIT(1)|BIT(0));
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, 0, BIT(6)|BIT(7));
        //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(13));

        // init power
        MDrv_Write2Byte(REG_ADC_ATOP_04_L ,0xF800 );
        MDrv_Write2Byte(REG_ADC_ATOP_05_L ,0x0003 );
        MDrv_Write2Byte(REG_ADC_ATOP_06_L ,0xFB00 );

        // divider number of the post divider
        MDrv_WriteByteMask(REG_ADC_ATOP_09_H, 0x00 ,0x18 );

        // Set offset
        Hal_ADC_SetOffsetR(pInstance, 0x800);
        Hal_ADC_SetOffsetG(pInstance, 0x800);
        Hal_ADC_SetOffsetB(pInstance, 0x800);

        // Set black level
        W2BYTEMSK(REG_ADC_DTOP_50_L, 0x800, 0x0FFF);
        W2BYTEMSK(REG_ADC_DTOP_55_L, 0x800, 0x0FFF);
        W2BYTEMSK(REG_ADC_DTOP_5A_L, 0x800, 0x0FFF);

        //Set ADC input LPF to low bandwidth.
        W2BYTE(REG_ADC_ATOP_34_L,0xFFFF);
        W2BYTE(REG_ADC_ATOP_35_L,0x00FF);

        W2BYTEMSK( REG_IPMUX_01_L, 0xF0, 0xF0 );  //select pattern generator source
        if(enAdcSource == ADC_INPUTSOURCE_ONLY_SCART )
        {
            MDrv_Write2Byte(REG_ADC_ATOP_03_L ,0x0000 );
            MDrv_Write2Byte(REG_ADC_ATOP_5E_L ,0x0200);
            MDrv_WriteByte(L_BK_CHIPTOP(0x55),0x00);
        }
        //MDrv_WriteByte(H_BK_ADC_ATOP(0x1C), 0xF8);   // Turn on SOG input low bandwidth filter

        //MDrv_WriteByte(BK_SELECT_00, u8Bank);

    }
}

void Hal_ADC_clk_gen_setting(void *pInstance, ADC_Gen_Clock_Type clocktype)
{

    W2BYTEMSK(REG_ADC_ATOP_01_L, 0x0f, 0x0f );

    W2BYTEMSK(REG_ADC_ATOP_1C_L, BIT(5), BIT(5) );  /// turn off ADC a SoG comparator
    W2BYTEMSK(REG_ADC_ATOP_1F_L, BIT(5), BIT(5) );  /// turn off ADC a SoG comparator

    W2BYTEMSK(REG_ADC_ATOP_0C_L, 0x00, 0x07 );
    switch(clocktype)
    {
    case E_ADC_Gen_480P_Clk:
    default:

        W2BYTEMSK(REG_ADC_DTOP_09_L, 0x1400,0x1F00);

        W2BYTE(REG_ADC_DTOP_01_L, 0x6656);
        W2BYTE(REG_ADC_DTOP_02_L, 0x0066);

        break;
    case E_ADC_Gen_720P_Clk:
        W2BYTEMSK(REG_ADC_DTOP_09_L, 0x0A00,0x1F00);

        W2BYTE(REG_ADC_DTOP_01_L, 0x8B2E);
        W2BYTE(REG_ADC_DTOP_02_L, 0x00A2);

        break;
    case E_ADC_Gen_1080P_Clk:
        W2BYTEMSK(REG_ADC_DTOP_09_L, 0x0100,0x1F00);

        W2BYTE(REG_ADC_DTOP_01_L, 0x4517);
        W2BYTE(REG_ADC_DTOP_02_L, 0x00D1);
        break;
    }

    W2BYTEMSK(REG_ADC_DTOP_06_L, 0x80, 0x80);

}



/******************************************************************************/
/// Power
/******************************************************************************/
// This will become table dumping function.
void Hal_ADC_init(void *pInstance, MS_U16 u16XTAL_CLK,MS_BOOL IsShareGrd,  MS_U16 eScartIDPortSelection)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
	if(psXCInstPri->u32DeviceID!=0)
		return;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    TAB_Info Tab_info;
    MS_U16 ScartIDPort = 0;
    //MS_U8 u8MPLL_LOOP_2nd_DIVIDER;

    Tab_info.pTable = (void*)MST_ADCINIT_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_INIT_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCINIT_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;

    MHAL_ADC_DBG(printf("ADC Tbl:init\n");)
    Hal_ADC_LoadTable(pInstance, &Tab_info);

    // Set share ground bit
    W2BYTEMSK(REG_ADC_ATOP_32_L, IsShareGrd?BIT(9):0 , BIT(9) );

    // Set Scart ID port selection
    if (eScartIDPortSelection & E_XC_SCARTID_TO_HSYNC0)
        ScartIDPort |= BIT(12);
    if (eScartIDPortSelection & E_XC_SCARTID_TO_HSYNC1)
        ScartIDPort |= BIT(13);
    if (eScartIDPortSelection & E_XC_SCARTID_TO_HSYNC2)
        ScartIDPort |= BIT(14);
    if (eScartIDPortSelection & E_XC_SCARTID_TO_SOG2)
        ScartIDPort |= BIT(15);

    //E_XC_SCARTID_TO_SOG0 and E_XC_SCARTID_TO_SOG1 are undefined.
    W2BYTEMSK(REG_ADC_ATOP_32_L, ScartIDPort , BIT(12)|BIT(13)|BIT(14)|BIT(15) );
    // Disable bIsYPbPrLooseLPF
    pXCResourcePrivate->sthal_ADC.bIsYPbPrLooseLPF = FALSE;

    // Enable iternal SOG
    #ifdef CONFIG_PUMABOOT
    Hal_ADC_ISOG_Enable(pInstance, FALSE);
    #else
    Hal_ADC_ISOG_Enable(pInstance, TRUE);
    #endif

    //  (1) Set full current mode for China package.
    //  (2) Read idac trim data from efuse with corresponding current mode
    //      & fill register.
    Hal_ADC_SetIdacCurrentMode(pInstance, ADC_IDAC_CURRENT_MODE_FULL);

    if(!Hal_ADC_SetCalibrationRefSource(pInstance, ADC_REFERENCE_VOLATAGE_BANDGAP)) printf("Set ADC Cal Ref to BG fail!!\n");
}


static void Hal_ADC_SetSourceAndPowerOn(void *pInstance, ADC_SOURCE_TYPE inputsrc_type)
{
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCSOURCE_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_SOURCE_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCSOURCE_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = inputsrc_type;

    MHAL_ADC_DBG(printf("ADC Tbl:set src %d \n", inputsrc_type);)
    Hal_ADC_LoadTable(pInstance, &Tab_info);
}

static void Hal_ADC_SetMux(void *pInstance, ADC_MUX_TYPE ipmux_type)
{
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCMUX_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_MUX_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCMUX_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = ipmux_type;

    MHAL_ADC_DBG(printf("ADC Tbl:set mux %d \n",ipmux_type);)
    Hal_ADC_LoadTable(pInstance, &Tab_info);

}

#ifdef ADC_ADCX_ENABLE
static void Hal_ADC_SetSOGX_calibration(void *pInstance)
{
    MS_U16 u16timeout = 20;
    W2BYTEMSK(REG_ADC_ATOP_03_L, 0xC000, 0xF000);
    W2BYTEMSK(REG_ADC_ATOPB_44_L, 0x0030, 0x00FF);
    W2BYTEMSK(REG_ADC_ATOPB_45_L, 0x0040, 0x00FF);
    W2BYTEMSK(REG_ADC_ATOPB_46_L, BIT(10), BIT(10));
    while ((R2BYTEMSK(REG_ADC_ATOPB_4D_L, BIT(8))) && (u16timeout>0))
    {
        MsOS_DelayTask(1);
        u16timeout--;
    }
    W2BYTEMSK(REG_ADC_ATOPB_46_L, R2BYTEMSK(REG_ADC_ATOPB_4D_L, 0xFF), 0x00FF);
    W2BYTEMSK(REG_ADC_ATOPB_46_L, 0x00, BIT(10));
    /// 2. MUX: MST_ADCMUX_TBL
    Hal_ADC_SetMux(pInstance, ADC_TABLE_MUX_CVBSX0);
}

static void Hal_ADC_SetCVBSO_ADCX(void *pInstance)
{
    TAB_Info Tab_info;
    /// init ADC flow:
    /// 1. CVBSOX: MST_ADCCVBSOX_TBL
    /// 2. MUX: MST_ADCMUX_TBL
    /// 3. AdcCal_AV_OFF: MST_ADCCVBSOX_TBL
    /// 4. SOGX_calibration
    /// 1. CVBSOX: MST_ADCCVBSOX_TBL
    Tab_info.pTable = (void*)MST_ADCCVBSOX_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSOX_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCCVBSOX_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = ADC_TABLE_CVBSOX_ADCX_ON;
    Hal_ADC_LoadTable(pInstance,&Tab_info);
    /// 2. MUX: MST_ADCMUX_TBL
    Hal_ADC_SetMux(pInstance, ADC_TABLE_MUX_CVBSX0);

    /// 3. AdcCal_AV_OFF: MST_ADCCVBSOX_TBL
    Tab_info.pTable = (void*)MST_ADCAdcCal_AV_OFF_TBL;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_AdcCal_AV_OFF_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCAdcCal_AV_OFF_TBL)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = 0;
    Hal_ADC_LoadTable(pInstance,&Tab_info);
    /// 4. SOGX_calibration
    Hal_ADC_SetSOGX_calibration(pInstance);

}
#endif

static void Hal_ADC_SetCVBSO_Ch1(void *pInstance, ADC_CVBSO_TYPE cvbso_type)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCCVBSO_TBL1;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSO_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCCVBSO_TBL1)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = cvbso_type;

    MHAL_ADC_DBG(printf("ADC Tbl:set cvbs0 ch1\n");)

#ifdef ADC_ADCX_ENABLE
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_S_DISPLAY_INITMISC_A_ENABLE_CVBSOX_ADCX))
    {
        if ((cvbso_type== ADC_TABLE_CVBSO_CVBS_ON) && (!IsSrcTypeATV(gSrcInfo[MAIN_WINDOW].enInputSourceType)))
        {
            Hal_ADC_SetCVBSO_ADCX(pInstance);
        }
        else
        {
            Hal_ADC_LoadTable(pInstance,&Tab_info);
        }
        if (cvbso_type== ADC_TABLE_CVBSO_OFF)
        {
            Tab_info.pTable = (void*)MST_ADCCVBSOX_TBL;
            Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSOX_NUMS*REG_DATA_SIZE;
            Tab_info.u8TabRows = sizeof(MST_ADCCVBSOX_TBL)/Tab_info.u8TabCols;
            Tab_info.u8TabIdx = ADC_TABLE_CVBSOX_ADCX_OFF;
            Hal_ADC_LoadTable(pInstance,&Tab_info);
        }
    }
    else
#endif
    {
        Hal_ADC_LoadTable(pInstance,&Tab_info);
    }
}

static void Hal_ADC_SetCVBSO_Ch2(void *pInstance, ADC_CVBSO_TYPE cvbso_type)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCCVBSO_TBL2;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSO_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCCVBSO_TBL2)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = cvbso_type;

    MHAL_ADC_DBG(printf("ADC Tbl:set cvbs0 ch2\n");)
#ifdef ADC_ADCX_ENABLE
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_S_DISPLAY_INITMISC_A_ENABLE_CVBSOX_ADCX))
    {
        if ((cvbso_type== ADC_TABLE_CVBSO_CVBS_ON) && (!IsSrcTypeATV(gSrcInfo[MAIN_WINDOW].enInputSourceType)))
        {
            Hal_ADC_SetCVBSO_ADCX(pInstance);
        }
        else
        {
            Hal_ADC_LoadTable(pInstance,&Tab_info);
        }
        if (cvbso_type== ADC_TABLE_CVBSO_OFF)
        {
            Tab_info.pTable = (void*)MST_ADCCVBSOX_TBL;
            Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSOX_NUMS*REG_DATA_SIZE;
            Tab_info.u8TabRows = sizeof(MST_ADCCVBSOX_TBL)/Tab_info.u8TabCols;
            Tab_info.u8TabIdx = ADC_TABLE_CVBSOX_ADCX_OFF;
            Hal_ADC_LoadTable(pInstance,&Tab_info);
        }
    }
    else
#endif
    {
        Hal_ADC_LoadTable(pInstance,&Tab_info);
    }

}

static void Hal_ADC_SetCVBSOMux_Ch1(void *pInstance, ADC_CVBSO_MUX_TYPE cvbso_MuxType)
{
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCCVBSO_MUX_TBL1;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSO_MUX_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCCVBSO_MUX_TBL1)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = cvbso_MuxType;

    MHAL_ADC_DBG(printf("ADC Tbl:set CVBSO mux ch1\n");)
    Hal_ADC_LoadTable(pInstance, &Tab_info);
}

static void Hal_ADC_SetCVBSOMux_Ch2(void *pInstance, ADC_CVBSO_MUX_TYPE cvbso_MuxType)
{
    TAB_Info Tab_info;
    Tab_info.pTable = (void*)MST_ADCCVBSO_MUX_TBL2;
    Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_CVBSO_MUX_NUMS*REG_DATA_SIZE;
    Tab_info.u8TabRows = sizeof(MST_ADCCVBSO_MUX_TBL2)/Tab_info.u8TabCols;
    Tab_info.u8TabIdx = cvbso_MuxType;

    MHAL_ADC_DBG(printf("ADC Tbl:set CVBSO mux ch2\n");)
    Hal_ADC_LoadTable(pInstance, &Tab_info);
}


void Hal_XC_ADC_poweron_source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType)
{
    // Obsolete in T3. Power control is merged into 'Hal_ADC_Set_Source'
}

ADC_MUX_TYPE Hal_ADC_port_mapping(void *pInstance, E_MUX_INPUTPORT enInputPortType)
{
    ADC_MUX_TYPE stADCIPMux;

    switch(enInputPortType)
    {
    case INPUT_PORT_ANALOG0:
        stADCIPMux = ADC_TABLE_MUX_RGB0_Data;
        break;
    case INPUT_PORT_ANALOG1:
        stADCIPMux = ADC_TABLE_MUX_RGB1_Data;
        break;
    case INPUT_PORT_ANALOG2:
        stADCIPMux = ADC_TABLE_MUX_RGB2_Data;
        break;

    case INPUT_PORT_ANALOG0_SYNC:
        stADCIPMux = ADC_TABLE_MUX_RGB0_Sync;
        break;
    case INPUT_PORT_ANALOG1_SYNC:
        stADCIPMux = ADC_TABLE_MUX_RGB1_Sync;
        break;
    case INPUT_PORT_ANALOG2_SYNC:
        stADCIPMux = ADC_TABLE_MUX_RGB2_Sync;
        break;

    case INPUT_PORT_YMUX_CVBS0:
        stADCIPMux = ADC_TABLE_MUX_CVBSY0;
        break;
    case INPUT_PORT_YMUX_CVBS1:
        stADCIPMux = ADC_TABLE_MUX_CVBSY1;
        break;
    case INPUT_PORT_YMUX_CVBS2:
        stADCIPMux = ADC_TABLE_MUX_CVBSY2;
        break;
    case INPUT_PORT_YMUX_CVBS3:
        stADCIPMux = ADC_TABLE_MUX_CVBSY3;
        break;
#if 0 // Einstein only has CVBS 0~3
    case INPUT_PORT_YMUX_CVBS4:
        stADCIPMux = ADC_TABLE_MUX_CVBSY4;
        break;
    case INPUT_PORT_YMUX_CVBS5:
        stADCIPMux = ADC_TABLE_MUX_CVBSY5;
        break;
    case INPUT_PORT_YMUX_CVBS6:
        stADCIPMux = ADC_TABLE_MUX_CVBSY6;
        break;
    case INPUT_PORT_YMUX_CVBS7:
        stADCIPMux = ADC_TABLE_MUX_CVBSY7;
        break;
#endif
    case INPUT_PORT_YMUX_G0:
        stADCIPMux = ADC_TABLE_MUX_G0;
        break;
    case INPUT_PORT_YMUX_G1:
        stADCIPMux = ADC_TABLE_MUX_G1;
        break;
    case INPUT_PORT_YMUX_G2:
        stADCIPMux = ADC_TABLE_MUX_G2;
        break;
    case INPUT_PORT_CMUX_CVBS0:
        stADCIPMux = ADC_TABLE_MUX_CVBSC0;
        break;
    case INPUT_PORT_CMUX_CVBS1:
        stADCIPMux = ADC_TABLE_MUX_CVBSC1;
        break;
    case INPUT_PORT_CMUX_CVBS2:
        stADCIPMux = ADC_TABLE_MUX_CVBSC2;
        break;
    case INPUT_PORT_CMUX_CVBS3:
        stADCIPMux = ADC_TABLE_MUX_CVBSC3;
        break;
#if 0 // Einstein only has CVBS 0~3
    case INPUT_PORT_CMUX_CVBS4:
        stADCIPMux = ADC_TABLE_MUX_CVBSC4;
        break;
    case INPUT_PORT_CMUX_CVBS5:
        stADCIPMux = ADC_TABLE_MUX_CVBSC5;
        break;
    case INPUT_PORT_CMUX_CVBS6:
        stADCIPMux = ADC_TABLE_MUX_CVBSC6;
        break;
    case INPUT_PORT_CMUX_CVBS7:
        stADCIPMux = ADC_TABLE_MUX_CVBSC7;
        break;
#endif
    case INPUT_PORT_CMUX_R0:
        stADCIPMux = ADC_TABLE_MUX_R0;
        break;
    case INPUT_PORT_CMUX_R1:
        stADCIPMux = ADC_TABLE_MUX_R1;
        break;
    case INPUT_PORT_CMUX_R2:
        stADCIPMux = ADC_TABLE_MUX_R2;
        break;
    case INPUT_PORT_DVI0:
        stADCIPMux = ADC_TABLE_MUX_DVI0;
        break;
    case INPUT_PORT_DVI1:
        stADCIPMux = ADC_TABLE_MUX_DVI1;
        break;
    case INPUT_PORT_DVI2:
        stADCIPMux = ADC_TABLE_MUX_DVI2;
        break;
    case INPUT_PORT_DVI3:
        stADCIPMux = ADC_TABLE_MUX_DVI3;
        break;

    case INPUT_PORT_MVOP:
    default:
        printf("Hal_ADC_port_mapping: unknown type %u\n",enInputPortType);
        stADCIPMux = ADC_TABLE_MUX_NUMS;
        break;
    }

    return stADCIPMux;
}

ADC_CVBSO_MUX_TYPE Hal_ADC_CVBSOMux_mapping(void *pInstance, E_MUX_INPUTPORT enInputPortType)
{
  ADC_CVBSO_MUX_TYPE stCVBSOMuxType;

    switch(enInputPortType)
    {
    case INPUT_PORT_YMUX_CVBS0:
    case INPUT_PORT_CMUX_CVBS0:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS0;
        break;
    case INPUT_PORT_YMUX_CVBS1:
    case INPUT_PORT_CMUX_CVBS1:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS1;
        break;
    case INPUT_PORT_YMUX_CVBS2:
    case INPUT_PORT_CMUX_CVBS2:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS2;
        break;
    case INPUT_PORT_YMUX_CVBS3:
    case INPUT_PORT_CMUX_CVBS3:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS3;
        break;
#if 0  // Einstein only has CVBS 0~3
    case INPUT_PORT_YMUX_CVBS4:
    case INPUT_PORT_CMUX_CVBS4:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS4;
        break;
    case INPUT_PORT_YMUX_CVBS5:
    case INPUT_PORT_CMUX_CVBS5:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS5;
        break;
    case INPUT_PORT_YMUX_CVBS6:
    case INPUT_PORT_CMUX_CVBS6:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS6;
        break;
    case INPUT_PORT_YMUX_CVBS7:
    case INPUT_PORT_CMUX_CVBS7:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_CVBS7;
        break;
#endif
    case INPUT_PORT_YMUX_G0:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_G0;
        break;
    case INPUT_PORT_YMUX_G1:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_G1;
        break;
    case INPUT_PORT_YMUX_G2:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_G2;
        break;
    case INPUT_PORT_CMUX_R0:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_R0;
        break;
    case INPUT_PORT_CMUX_R1:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_R1;
        break;
    case INPUT_PORT_CMUX_R2:
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_R2;
        break;
    default:
        printf("Hal_ADC_CVBSOMux_mapping: unknown type %u\n",enInputPortType);
        stCVBSOMuxType = ADC_TABLE_CVBSO_MUX_NUMS;
        break;
    }

    return stCVBSOMuxType;
}
void Hal_ADC_set_mux(void *pInstance, E_MUX_INPUTPORT port)
{
    Hal_ADC_SetMux(pInstance, Hal_ADC_port_mapping(pInstance, port));
}

void Hal_ADC_set_CVBSOMux(void *pInstance, ADC_CVBSOPortNumber CVBSOPortNumber, E_MUX_INPUTPORT port)
{
    if (CVBSOPortNumber == E_ADC_CVBSO_PORT_MAX)
    {
        printf("CVBSO port is not exist\n");
        return;
    }

    if (CVBSOPortNumber == E_ADC_CVBSO_PORT_1)
    {
        Hal_ADC_SetCVBSOMux_Ch1(pInstance, Hal_ADC_CVBSOMux_mapping(pInstance, port));
    }
    else if (CVBSOPortNumber == E_ADC_CVBSO_PORT_2)
    {
        Hal_ADC_SetCVBSOMux_Ch2(pInstance, Hal_ADC_CVBSOMux_mapping(pInstance, port));
    }
}

static void _Hal_ADC_set_freerun(void *pInstance, MS_BOOL bEnable)
{
    TAB_Info Tab_info;

    if (bEnable)
    {
        Tab_info.pTable = (void*)MST_ADCFreeRunEn_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_FreeRunEn_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCFreeRunEn_TBL)/Tab_info.u8TabCols;
        Tab_info.u8TabIdx = 0;
    }
    else
    {
        Tab_info.pTable = (void*)MST_ADCFreeRunDis_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_FreeRunDis_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCFreeRunDis_TBL)/Tab_info.u8TabCols;
        Tab_info.u8TabIdx = 0;
    }

    Hal_ADC_LoadTable(pInstance, &Tab_info);
}

static void _Hal_ADC_porst(void *pInstance, MS_BOOL bEnable)
{
    TAB_Info Tab_info;

    if (bEnable)
    {
        Tab_info.pTable = (void*)MST_ADCPorstEn_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_PorstEn_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCPorstEn_TBL)/Tab_info.u8TabCols;
        Tab_info.u8TabIdx = 0;
    }
    else
    {
        Tab_info.pTable = (void*)MST_ADCPorstDis_TBL;
        Tab_info.u8TabCols = REG_ADDR_SIZE+REG_MASK_SIZE+ADC_TABLE_PorstDis_NUMS*REG_DATA_SIZE;
        Tab_info.u8TabRows = sizeof(MST_ADCPorstDis_TBL)/Tab_info.u8TabCols;
        Tab_info.u8TabIdx = 0;
    }

    Hal_ADC_LoadTable(pInstance, &Tab_info);
}

void MDrv_XC_ADC_Set_Freerun(void *pInstance, MS_BOOL bEnable)
{

    _Hal_ADC_set_freerun(pInstance, bEnable);

    if (bEnable)
    {
        _Hal_ADC_porst(pInstance, TRUE);
        MsOS_DelayTaskUs(10);
        _Hal_ADC_porst(pInstance, FALSE);
    }
}

void Hal_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_ADC._bSourceSwitched = bSwitch;
}

void Hal_ADC_set_mode(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16PixClk, u16HTotal, u16TimeOutCnt = 32;

    if((pXCResourcePrivate->sthal_ADC.enLastSourceType != enADCInput) ||
       (pXCResourcePrivate->sthal_ADC.u16LastHtotal != u16HorizontalTotal) ||
       (pXCResourcePrivate->sthal_ADC.u16LastPixelClk != u16PixelClockPerSecond) ||
       (pXCResourcePrivate->sthal_ADC.u16LastSamplingRatio != u16SamplingRatio) ||
       pXCResourcePrivate->sthal_ADC._bSourceSwitched)
    {
        // update new setting
        pXCResourcePrivate->sthal_ADC.enLastSourceType = enADCInput;
        pXCResourcePrivate->sthal_ADC.u16LastHtotal = u16HorizontalTotal;
        pXCResourcePrivate->sthal_ADC.u16LastPixelClk = u16PixelClockPerSecond;
        pXCResourcePrivate->sthal_ADC.u16LastSamplingRatio = u16SamplingRatio;

        pXCResourcePrivate->sthal_ADC._bSourceSwitched = 0; //Must clear this flag once the init has been done one time, or the screen will be blinking.
    }
    else
    {
        u16PixClk = u16PixelClockPerSecond * u16SamplingRatio;
        u16HTotal = u16HorizontalTotal * u16SamplingRatio;

        // If same timing, dump register only. Do not reset again.
        Hal_ADC_SetADCBwFilter(pInstance, enADCInput,u16PixClk);

        // Adc set mode already complete if reach here.
        _Hal_ADC_set_freerun(pInstance, FALSE);

        ///////////////////////////
        // Protection
        do
        {
            while (R2BYTEMSK(REG_ADC_ATOP_0F_L, BIT(12)))
            {
                _Hal_ADC_set_freerun(pInstance, TRUE);

                _Hal_ADC_porst(pInstance, TRUE);
                Hal_ADC_SetADCBwFilter(pInstance, enADCInput,u16PixClk);
                _Hal_ADC_porst(pInstance, FALSE);

                MsOS_DelayTask(5);
                Hal_ADC_dtop_clk_setting (pInstance, u16HTotal);
                _Hal_ADC_set_freerun(pInstance, FALSE);
                if(u16TimeOutCnt == 0)
                    break;
                else
                    --u16TimeOutCnt;
            }
            if(u16TimeOutCnt == 0)
                break;
            else
                --u16TimeOutCnt;

            MsOS_DelayTask(2);
        } while(!R2BYTEMSK(REG_ADC_DTOP_05_L, (BIT(7) | BIT(6)) ));

        if( (enADCInput & ADC_INPUTSOURCE_ONLY_YPBPR) || (enADCInput & ADC_INPUTSOURCE_ONLY_RGB) )
        {
            W2BYTEMSK(REG_ADC_ATOP_07_L, BIT(6), BIT(6));
            MsOS_DelayTask(2);
            W2BYTEMSK(REG_ADC_ATOP_07_L, 0, BIT(6));
        }
        return;
    }

    // There is no table for Scart
    if (enADCInput == ADC_INPUTSOURCE_ONLY_SCART)
    {
        //SCART iclamp setting
        Hal_ADC_SCART_RGB_setMode(pInstance, TRUE);
        MsOS_DelayTask(5);
        Hal_ADC_Set_Source_Calibration(pInstance, enADCInput);
        return;
    }
    else
    {
        Hal_ADC_SCART_RGB_setMode(pInstance, FALSE);
    }

    u16PixClk = u16PixelClockPerSecond * u16SamplingRatio;
    u16HTotal = u16HorizontalTotal * u16SamplingRatio;

    _Hal_ADC_set_freerun(pInstance, TRUE);
    _Hal_ADC_porst(pInstance, TRUE);
    Hal_ADC_SetADCBwFilter(pInstance, enADCInput,u16PixClk);
    _Hal_ADC_porst(pInstance, FALSE);
    MsOS_DelayTask(5);
    Hal_ADC_dtop_clk_setting (pInstance, u16HTotal);
    // disable freerun after set ATOP_09[12:11], BK_ATOP_0C[2:0], DTOP_00[12:0]
    _Hal_ADC_set_freerun(pInstance, FALSE);

    do
    {
        while (R2BYTEMSK(REG_ADC_ATOP_0F_L, BIT(12)))
        {
            _Hal_ADC_set_freerun(pInstance, TRUE);
            _Hal_ADC_porst(pInstance, TRUE);
            Hal_ADC_SetADCBwFilter(pInstance, enADCInput,u16PixClk);
            _Hal_ADC_porst(pInstance, FALSE);
            MsOS_DelayTask(5);
            Hal_ADC_dtop_clk_setting (pInstance, u16HTotal);
            _Hal_ADC_set_freerun(pInstance, FALSE);
            if(u16TimeOutCnt == 0)
                break;
            else
                --u16TimeOutCnt;
        }
        if(u16TimeOutCnt == 0)
            break;
        else
            --u16TimeOutCnt;
        MsOS_DelayTask(2);
    } while(!R2BYTEMSK(REG_ADC_DTOP_05_L, (BIT(7) | BIT(6)) ));

    Hal_ADC_Set_Source_Calibration(pInstance, enADCInput);

    //Hal_ADC_clamp_duration_setting( u16HorizontalTotal/50 );
    W2BYTEMSK(REG_ADC_DTOP_08_L, (u16HorizontalTotal/50)<<8, HBMASK);
}

void Hal_ADC_set_cvbs_out(void *pInstance, E_ADC_CVBSOUT_TYPE e_cvbs_out_type)
{
    ADC_CVBSO_TYPE stCVBSOType=ADC_TABLE_CVBSO_OFF;

    switch(e_cvbs_out_type)
    {
        default:
        case ADC_CVBSOUT_DISABLE_1:
        case ADC_CVBSOUT_DISABLE_2:
            stCVBSOType =  ADC_TABLE_CVBSO_OFF;
            break;

        case ADC_CVBSOUT_SV_ON_1:
            stCVBSOType =  ADC_TABLE_CVBSO_SV_ON;
            break;
        case ADC_CVBSOUT_SV_OFF_1:
            stCVBSOType =  ADC_TABLE_CVBSO_SV_OFF;
            break;
        case ADC_CVBSOUT_CVBS_ON_1:
            stCVBSOType =  ADC_TABLE_CVBSO_CVBS_ON;
            break;
        case ADC_CVBSOUT_CVBS_OFF_1:
            stCVBSOType =  ADC_TABLE_CVBSO_SV_OFF;
            break;

        case ADC_CVBSOUT_VIF_VE_1:
            stCVBSOType =  ADC_TABLE_CVBSO_VIF_DAC;
            break;

        case ADC_CVBSOUT_VIF_VIF_1:
            stCVBSOType =  ADC_TABLE_CVBSO_VIF_VIF;
            break;

        case ADC_CVBSOUT_SV_ON_2:
            stCVBSOType =  ADC_TABLE_CVBSO_SV_ON;
            break;

        case ADC_CVBSOUT_SV_OFF_2:
            stCVBSOType =  ADC_TABLE_CVBSO_SV_OFF;
            break;

        case ADC_CVBSOUT_CVBS_ON_2:
            stCVBSOType =  ADC_TABLE_CVBSO_CVBS_ON;
            break;

        case ADC_CVBSOUT_CVBS_OFF_2:
            stCVBSOType =  ADC_TABLE_CVBSO_CVBS_OFF;
            break;

        case ADC_CVBSOUT_VIF_VE_2:
            stCVBSOType =  ADC_TABLE_CVBSO_VIF_DAC;
            break;

        case ADC_CVBSOUT_VIF_VIF_2:
            stCVBSOType =  ADC_TABLE_CVBSO_VIF_VIF;
            break;


    }

    // e_cvbs_out_type must larger then ADC_CVBSOUT_DISABLE_1 and less or equal then ADC_CVBSOUT_VIF_VIF_1
    if ( e_cvbs_out_type <= ADC_CVBSOUT_VIF_VIF_1 )
    {
        if ( e_cvbs_out_type == ADC_CVBSOUT_DISABLE_1 )
        {
            Hal_ADC_SetCVBSO_Ch1(pInstance, stCVBSOType);

            // Set mux to a undefined mux for forcing send out black level.
            MDrv_WriteByteMask(REG_ADC_ATOP_51_L, 0xE0 , 0xF0 ); // Change mux to null mux.

            // Make sure Y mux is turn off ( Keep C mux enabled )
            MDrv_WriteByteMask(REG_ADC_ATOP_51_H, BIT(2) , BIT(2) | BIT(3)); // Turn on C mux

            // DISABLE DACA2 (Disable clock of DAC out ch1)
            MDrv_WriteRegBit(REG_CKG_DACA2, ENABLE, CKG_DACA2_GATED);

            // Waiting stable
            MsOS_DelayTask(30);

            // Turn off CVBS out power
            MDrv_WriteByteMask(REG_ADC_ATOP_50_L, BIT(0) , BIT(0) );
        }
        else
        {
            // Turn on CVBS out power
            MDrv_WriteByteMask(REG_ADC_ATOP_50_L, 0x00  , BIT(0) );

            // DISABLE DACA2 (Disable clock of DAC out ch1)
            MDrv_WriteRegBit(REG_CKG_DACA2, DISABLE, CKG_DACA2_GATED);

            // Reset C mux
            MDrv_WriteByteMask(REG_ADC_ATOP_51_L, 0x00 , 0xF0 );

            Hal_ADC_SetCVBSO_Ch1(pInstance, stCVBSOType);
        }

    }
    else
    {
        if ( e_cvbs_out_type == ADC_CVBSOUT_DISABLE_2 )
        {
            Hal_ADC_SetCVBSO_Ch2(pInstance, stCVBSOType);

            // Set mux to a undefined mux for forcing send out black level.
            MDrv_WriteByteMask(REG_ADC_ATOP_53_L, 0xE0 , 0xF0 ); // Change mux to null mux.

            // Make sure Y mux is turn off ( Keep C mux enabled )
            MDrv_WriteByteMask(REG_ADC_ATOP_53_H, BIT(2) , BIT(2) | BIT(3)); // Turn on C mux

            // DISABLE DACB2 (Disable clock of DAC out ch2)
            MDrv_WriteRegBit(REG_CKG_DACB2, ENABLE, CKG_DACB2_GATED);

            // Waiting stable
            MsOS_DelayTask(30);

            // Turn off CVBS out power
            MDrv_WriteByteMask(REG_ADC_ATOP_52_L, BIT(0) , BIT(0) );

        }
        else
        {
            // Turn on CVBS out power
            MDrv_WriteByteMask(REG_ADC_ATOP_52_L, 0x00 , BIT(0) );

            // DISABLE DACB2 (Disable clock of DAC out ch2)
            MDrv_WriteRegBit(REG_CKG_DACB2, DISABLE, CKG_DACB2_GATED);
            // Reset C mux
            MDrv_WriteByteMask(REG_ADC_ATOP_53_L, 0x00 , 0xF0 );

            Hal_ADC_SetCVBSO_Ch2(pInstance, stCVBSOType);
        }
    }

    if (e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_1 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VIF_1 )
    {
        // Select Mux to DAC, otherwise setting from Mux driver.
        MDrv_WriteByte(REG_ADC_ATOP_51_L,0x0F);
    }
    else if ( e_cvbs_out_type == ADC_CVBSOUT_VIF_VE_2 || e_cvbs_out_type == ADC_CVBSOUT_VIF_VIF_2 )
    {
        // Select Mux to DAC, otherwise setting from Mux driver.
        MDrv_WriteByte(REG_ADC_ATOP_53_L,0x0F);
    }

}

MS_BOOL Hal_ADC_is_cvbs_out_enabled(void *pInstance, E_MUX_OUTPUTPORT outputPort)
{
    if (outputPort == OUTPUT_PORT_CVBS1)
    {
        if ( MDrv_ReadByte(REG_ADC_ATOP_50_L ) & BIT(0) )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else if (outputPort == OUTPUT_PORT_CVBS2)
    {
        if ( MDrv_ReadByte(REG_ADC_ATOP_52_L ) & BIT(0) )
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }

    }
    else
    {
        return FALSE;
    }
}

void Hal_ADC_Set_Source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount)
{
    ADC_SOURCE_TYPE stADCSrc;

    UNUSED(enInputPortType);
    UNUSED(u8PortCount);

    switch(enADC_SourceType)
    {
    case ADC_INPUTSOURCE_ONLY_RGB:
        stADCSrc = ADC_TABLE_SOURCE_RGB;
        break;

    case ADC_INPUTSOURCE_ONLY_YPBPR:
        stADCSrc = ADC_TABLE_SOURCE_YUV;
        break;

    case ADC_INPUTSOURCE_ONLY_MVOP:
        stADCSrc = ADC_TABLE_SOURCE_MVOP;
        break;

    case ADC_INPUTSOURCE_ONLY_DVI:
        stADCSrc = ADC_TABLE_SOURCE_DVI;
        break;

    case ADC_INPUTSOURCE_ONLY_ATV:
        stADCSrc = ADC_TABLE_SOURCE_ATV;
        break;

    case ADC_INPUTSOURCE_ONLY_SVIDEO:
        stADCSrc = ADC_TABLE_SOURCE_SVIDEO;
        break;

    case ADC_INPUTSOURCE_ONLY_SCART:
        stADCSrc = ADC_TABLE_SOURCE_SCART;
        break;

    case ADC_INPUTSOURCE_ONLY_CVBS:
        stADCSrc = ADC_TABLE_SOURCE_CVBS;
        break;

    case ADC_INPUTSOURCE_MULTI_RGB_MVOP:
        stADCSrc = ADC_TABLE_SOURCE_RGB_MV;
        break;

    case ADC_INPUTSOURCE_MULTI_RGB_DVI:
        stADCSrc = ADC_TABLE_SOURCE_RGB_DVI;
        break;

    case ADC_INPUTSOURCE_MULTI_RGB_SCART:
        stADCSrc = ADC_TABLE_SOURCE_RGB_SC;
        break;

    case ADC_INPUTSOURCE_MULTI_RGB_ATV:
    case ADC_INPUTSOURCE_MULTI_RGB_CVBS:
        stADCSrc = ADC_TABLE_SOURCE_RGB_AV;
        break;

    case ADC_INPUTSOURCE_MULTI_YPBPR_MVOP:
        stADCSrc = ADC_TABLE_SOURCE_YUV_MV;
        break;

    case ADC_INPUTSOURCE_MULTI_YPBPR_DVI:
        stADCSrc = ADC_TABLE_SOURCE_YUV_DVI;
        break;


    case ADC_INPUTSOURCE_MULTI_YPBPR_SCART:
        stADCSrc = ADC_TABLE_SOURCE_YUV_SC;
        break;

    case ADC_INPUTSOURCE_MULTI_YPBPR_ATV:
    case ADC_INPUTSOURCE_MULTI_YPBPR_CVBS:
        stADCSrc = ADC_TABLE_SOURCE_YUV_AV;
        break;


    case ADC_INPUTSOURCE_MULTI_MVOP_SVIDEO:
        stADCSrc = ADC_TABLE_SOURCE_MVOP_SV;
        break;

    case ADC_INPUTSOURCE_MULTI_MVOP_SCART:
        stADCSrc = ADC_TABLE_SOURCE_MVOP_SC;
        break;

    case ADC_INPUTSOURCE_MULTI_MVOP_CVBS:
        stADCSrc = ADC_TABLE_SOURCE_MVOP_AV;
        break;

    case ADC_INPUTSOURCE_MULTI_DVI_SCART:
        stADCSrc = ADC_TABLE_SOURCE_DVI_SC;
        break;

    case ADC_INPUTSOURCE_MULTI_DVI_SVIDEO:
        stADCSrc = ADC_TABLE_SOURCE_DVI_SV;
        break;

    case ADC_INPUTSOURCE_MULTI_DVI_ATV:
    case ADC_INPUTSOURCE_MULTI_DVI_CVBS:
        stADCSrc = ADC_TABLE_SOURCE_DVI_CVBS;
        break;

    // Unsupport in T3
    case ADC_INPUTSOURCE_MULTI_YPBPR_SVIDEO:
    case ADC_INPUTSOURCE_MULTI_RGB_SVIDEO:
    case ADC_INPUTSOURCE_MULTI_MVOP_DVI:
    default:
        printf("Unknown type =%u\n",enADC_SourceType);
        stADCSrc = ADC_TABLE_SOURCE_NUMS;
        break;
    }

    if(stADCSrc != ADC_TABLE_SOURCE_NUMS)
    {
        Hal_ADC_SetSourceAndPowerOn(pInstance, stADCSrc);
    }

    MsOS_DelayTask(2);

}

void Hal_ADC_auto_adc_backup(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    //FIXME: T3 need rewrite this function.
    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_00    = MDrv_ReadByte(REG_ADC_ATOP_00_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_01    = MDrv_ReadByte(REG_ADC_ATOP_01_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_0C    = MDrv_ReadByte(REG_ADC_ATOP_0C_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_2C    = MDrv_ReadByte(REG_ADC_ATOP_2C_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_1F    = MDrv_ReadByte(REG_ADC_ATOP_1F_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8H_BkAtop_2D    = MDrv_ReadByte(REG_ADC_ATOP_2D_H );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_03    = MDrv_ReadByte(REG_ADC_ATOP_03_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16L_BkAtop_05   = MDrv_Read2Byte(REG_ADC_ATOP_05_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16L_BkAtop_5E   = MDrv_Read2Byte(REG_ADC_ATOP_5E_L );

    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8H_BkChipTop_1F = MDrv_ReadByte(H_BK_CHIPTOP(0x1f));
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkChipTop_55 = MDrv_ReadByte(L_BK_CHIPTOP(0x55));
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkIpMux_1    = MDrv_ReadByte(REG_IPMUX_01_L );
    //pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_SC_BK1_21    = MDrv_ReadByte(L_BK_IP1F2(0x21) );
    if(eWindow == SUB_WINDOW)
    {
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_21    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L);
    }
    else
    {
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_21    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L);
    }

    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.R_UserOffset =    MDrv_Read2Byte(REG_ADC_DTOP_52_L) & 0x1FFF;
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.G_UserOffset =    MDrv_Read2Byte(REG_ADC_DTOP_57_L) & 0x1FFF;
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.B_UserOffset =    MDrv_Read2Byte(REG_ADC_DTOP_5C_L) & 0x1FFF;

    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.R_BlankLevel =    MDrv_Read2Byte(REG_ADC_DTOP_50_L) & 0x0FFF;
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.G_BlankLevel =    MDrv_Read2Byte(REG_ADC_DTOP_55_L) & 0x0FFF;
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.B_BlankLevel =    MDrv_Read2Byte(REG_ADC_DTOP_5A_L) & 0x0FFF;

    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_1C     = MDrv_Read2Byte(REG_ADC_ATOP_1C_L );

    // backup power setting
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_04     = MDrv_Read2Byte(REG_ADC_ATOP_04_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_05     = MDrv_Read2Byte(REG_ADC_ATOP_05_L);
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_06     = MDrv_Read2Byte(REG_ADC_ATOP_06_L );

    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkDtop_01     = MDrv_Read2Byte(REG_ADC_DTOP_01_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkDtop_02     = MDrv_Read2Byte(REG_ADC_DTOP_02_L );
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkDtop_06    = MDrv_ReadByte(REG_ADC_DTOP_06_L );

    if(eWindow == SUB_WINDOW)
    {
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_02     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_03     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_03_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_04     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_05     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_06     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_07     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_0E     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L);

        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_01    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_03    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_04    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0E    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4E_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0F    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4F_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_16    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_17    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L);

        //scaling
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_04    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_04_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_05    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_08    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_08_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_09    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_09_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_07    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_27_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_08    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_28_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_09    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_0A    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_2A_L);

        //DNR base
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_08    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_48_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0A    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4A_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0C    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4C_L);
        //OPM Base
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_10    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_12    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_52_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_14    = 0;//SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L);

        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_01    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_21    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_07    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_44    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_47    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1A     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_5A_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1B     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_5B_L);

        //CSC
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_2B     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_2B_L);
    }
    else
    {
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_02     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_03     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_04     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_05     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_06     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_07     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_0E     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L);

        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_01    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_03    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_04    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0E    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0F    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_16    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_17    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L);

        //scaling
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_04    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_05    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_08    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_09    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_07    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_07_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_08    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_09    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_0A    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L);

        //DNR base
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_08    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0A    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0A_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0C    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0C_L);
        //OPM Base
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_10    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_10_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_12    = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_12_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_14    = 0;//SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L);

        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_01    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_21    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_07    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_44    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_47    = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1A     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_1A_L);
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1B     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_1B_L);

        //CSC
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_2B     = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_2B_L);
    }

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK10_19   = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L);
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if (IsVBY1_16LANE((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type))
    {
        pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BKCB_17  = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_17_L);
    }
#endif

    MDrv_WriteByteMask(REG_ADC_ATOP_5C_L, 0x30,0x30);  //ldo

    if(eWindow == SUB_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_42_L, BIT(15), BIT(15));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, BIT(15), BIT(15));
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

void Hal_ADC_disable_csc(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //disable CSC for internal auto calibration if CSC is enable
    if(eWindow == SUB_WINDOW)
    {
        if(0 != SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_2B_L))
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_2B_L, 0);
        }
    }
    else
    {
        if(0 != SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_2B_L))
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_2B_L, 0);
        }
    }
}

void Hal_ADC_auto_adc_restore(void *pInstance, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
   // u8Bank = MDrv_ReadByte(BK_SELECT_00);
   // MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    MDrv_WriteByte(REG_ADC_ATOP_00_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_00);
    MDrv_WriteByte(REG_ADC_ATOP_01_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_01);
    MDrv_WriteByte(REG_ADC_ATOP_0C_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_0C);
    MDrv_WriteByte(REG_ADC_ATOP_2C_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_2C);
    MDrv_WriteByte(REG_ADC_ATOP_1F_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_1F);
    MDrv_WriteByte(REG_ADC_ATOP_2D_H, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8H_BkAtop_2D);
    MDrv_WriteByte(REG_ADC_ATOP_03_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkAtop_03);
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16L_BkAtop_05);
    MDrv_Write2Byte(REG_ADC_ATOP_5E_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16L_BkAtop_5E);

    // ADC clock gen
    MDrv_Write2Byte(REG_ADC_DTOP_01_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkDtop_01);
    MDrv_Write2Byte(REG_ADC_DTOP_02_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkDtop_02);
    MDrv_WriteByte(REG_ADC_DTOP_06_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkDtop_06);

    // Rstore Offset
    W2BYTEMSK(REG_ADC_DTOP_52_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.R_UserOffset, 0x1FFF);
    W2BYTEMSK(REG_ADC_DTOP_57_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.G_UserOffset, 0x1FFF);
    W2BYTEMSK(REG_ADC_DTOP_5C_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.B_UserOffset, 0x1FFF);

    // Restore black level
    W2BYTEMSK(REG_ADC_DTOP_50_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.R_BlankLevel, 0x0FFF);
    W2BYTEMSK(REG_ADC_DTOP_55_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.G_BlankLevel, 0x0FFF);
    W2BYTEMSK(REG_ADC_DTOP_5A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.B_BlankLevel, 0x0FFF);

    MDrv_WriteByte(H_BK_CHIPTOP(0x1f), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8H_BkChipTop_1F);
    MDrv_WriteByte(L_BK_CHIPTOP(0x55), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkChipTop_55);
    MDrv_WriteByte(REG_IPMUX_01_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_BkIpMux_1);
    //MDrv_WriteByte(L_BK_IP1F2(0x21), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_SC_BK1_21);

    if(eWindow == SUB_WINDOW)
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_21);
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_21);
    }

    MDrv_Write2Byte(REG_ADC_ATOP_1C_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_1C);

    // Restore Power
    MDrv_Write2Byte(REG_ADC_ATOP_04_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_04);
    MDrv_Write2Byte(REG_ADC_ATOP_05_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_05);
    MDrv_Write2Byte(REG_ADC_ATOP_06_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16BkAtop_06);

    if(eWindow == SUB_WINDOW)
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_02);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_03_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_03);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_04_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_04);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_05_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_05);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_06_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_06);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_07);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_0E_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_0E);
        /*
           MDrv_Write2Byte(L_BK_IP1F2(0x02), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_02);
           MDrv_Write2Byte(L_BK_IP1F2(0x03), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_03);
           MDrv_Write2Byte(L_BK_IP1F2(0x04), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_04);
           MDrv_Write2Byte(L_BK_IP1F2(0x05), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_05);
           MDrv_Write2Byte(L_BK_IP1F2(0x06), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_06);
           MDrv_Write2Byte(L_BK_IP1F2(0x07), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_07);
           MDrv_Write2Byte(L_BK_IP1F2(0x0E), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_0E);
           */
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_SCMI);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_41_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_01);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_03);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_04);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4E_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0E);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4F_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0F);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_56_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_16);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_57_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_17);

        //scaling
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_04_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_04);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_05);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_08_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_08);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_09_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_09);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_27_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_07);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_28_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_08);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_09);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_2A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_0A);

        //DNR,OPM Base
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_48_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_08);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0A);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_4C_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0C);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_50_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_10);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_52_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_12);
        //    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_14);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_5A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1A);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_5B_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1B);

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_01);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_21);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_07);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_44);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_47);
        /*
           MDrv_Write2Byte(L_BK_SCMI(0x01), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_01);
           MDrv_Write2Byte(L_BK_SCMI(0x03), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_03);
           MDrv_Write2Byte(L_BK_SCMI(0x04), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_04);
           MDrv_Write2Byte(L_BK_SCMI(0x0E), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0E);
           MDrv_Write2Byte(L_BK_SCMI(0x0F), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0F);
           MDrv_Write2Byte(L_BK_SCMI(0x16), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_16);
           MDrv_Write2Byte(L_BK_SCMI(0x17), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_17);
           */

        //CSC
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK04_2B_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_2B);
    }
    else
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_02);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_03);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_04_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_04);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_05_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_05);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_06);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_07);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0E_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_0E);
        /*
           MDrv_Write2Byte(L_BK_IP1F2(0x02), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_02);
           MDrv_Write2Byte(L_BK_IP1F2(0x03), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_03);
           MDrv_Write2Byte(L_BK_IP1F2(0x04), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_04);
           MDrv_Write2Byte(L_BK_IP1F2(0x05), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_05);
           MDrv_Write2Byte(L_BK_IP1F2(0x06), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_06);
           MDrv_Write2Byte(L_BK_IP1F2(0x07), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_07);
           MDrv_Write2Byte(L_BK_IP1F2(0x0E), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK1_0E);
           */
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_SCMI);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_01_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_01);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_03);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_04);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0E_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0E);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0F_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0F);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_16_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_16);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_17_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_17);

        //scaling
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_04_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_04);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_05);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_08_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_08);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_09);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_07_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_07);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_08);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_09);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK23_0A);

        //DNR,OPM Base
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_08_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_08);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0A);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_0C_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_0C);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_10_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_10);
        SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_12_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_12);
        //    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_14_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u32SC_BK12_14);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_1A_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1A);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_1B_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_1B);

        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_01_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_01);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK06_21_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK06_21);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_07);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_44_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_44);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_47);
        /*
           MDrv_Write2Byte(L_BK_SCMI(0x01), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_01);
           MDrv_Write2Byte(L_BK_SCMI(0x03), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_03);
           MDrv_Write2Byte(L_BK_SCMI(0x04), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_04);
           MDrv_Write2Byte(L_BK_SCMI(0x0E), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0E);
           MDrv_Write2Byte(L_BK_SCMI(0x0F), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_0F);
           MDrv_Write2Byte(L_BK_SCMI(0x16), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_16);
           MDrv_Write2Byte(L_BK_SCMI(0x17), pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK12_17);
           */

        //CSC
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_2B_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK02_2B);
    }

    MDrv_WriteByteMask(REG_ADC_ATOP_5C_L, 0x00,0x70);

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if (IsVBY1_16LANE((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type))
    {
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_17_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BKCB_17);
    }
#endif

    if(eWindow == SUB_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_42_L, 0x00, BIT(15));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0x00, BIT(15));
    }

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteByte(L_BK_VOP(0x19),pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u8L_SC_BK10_19  );
    //need delay before unmute,the delay time is a experiential value
    MsOS_DelayTask(250);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L, pXCResourcePrivate->sthal_ADC._stAutoAdcSetting.u16SC_BK10_19);

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

MS_BOOL Hal_ADC_is_scart_rgb(void *pInstance)
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

MS_U16 Hal_ADC_get_clk (void *pInstance)
{
    //u16Value -= 3; // actual - 3
 //ADC PLL divider ratio (htotal-3), (write sequence LSB -> MSB)
    return (R2BYTEMSK(REG_ADC_DTOP_00_L, 0xFFFF)+3);//(REG_ADC_DTOP_00_L, u16Value);
}

MS_BOOL Hal_ADC_set_SoG_Calibration(void *pInstance)
{
    W2BYTEMSK(REG_ADC_ATOP_39_L, 0, BIT(13) );
    W2BYTEMSK(REG_ADC_ATOP_3C_L, 0, BIT(13) );

    OS_DELAY_TASK(1);

    W2BYTEMSK(REG_ADC_ATOP_39_L, BIT(13), BIT(13) );
    W2BYTEMSK(REG_ADC_ATOP_3C_L, BIT(13), BIT(13) );

    return TRUE;
}
/******************************************************************************/
///This function return SOG level range
///@param u32Min \b OUT: min of SOG level
///@param u32Max \b OUT: max of SOG level
///@param u32Recommend_value \b OUT: recommend value
/******************************************************************************/
void Hal_ADC_get_SoG_LevelRange(void *pInstance, MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value)
{
    *u32Min = 0;
    *u32Max = 255;
    *u32Recommend_value = 0x68;
}

/******************************************************************************/
///This function Set SOG Level
///@param u32Value \b IN: set SOG value
/******************************************************************************/
void Hal_ADC_set_SoG_Level(void *pInstance, MS_U32 u32Value)
{
    W2BYTEMSK(REG_ADC_ATOP_3B_L, (MS_U16)u32Value,LBMASK);
}

/******************************************************************************/
///select RGB input pipe delay, this reg will decide the H start of SCART RGB
///@param u32Value \b IN: set PIPE Delay value
/******************************************************************************/
void Hal_ADC_set_RGB_PIPE_Delay(void *pInstance, MS_U8 u8Value)
{
    W2BYTEMSK(REG_ADC_ATOP_43_L, (u8Value<<8), 0x7F00);
}

/******************************************************************************/
///This function set Scart RGB Sync on Green clamp delay.
///@param u16Value \b IN: set clamp delay value
/******************************************************************************/
void Hal_ADC_set_ScartRGB_SOG_ClampDelay(void *pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur)
{
    W2BYTEMSK(REG_ADC_DTOP_17_L, u16Clpdly, 0x0FFF);
    W2BYTEMSK(REG_ADC_DTOP_18_L, u16Caldur, 0x00FF);
}

/******************************************************************************/
///This function set YPbPr Loose LPF.
///@param benable \b IN: enable or disable
/******************************************************************************/
void Hal_ADC_set_YPbPrLooseLPF(void *pInstance, MS_BOOL benable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_ADC.bIsYPbPrLooseLPF = benable;
}

/******************************************************************************/
///This function set SOG BW
///@param u16value \b IN: value of SOG BW
/******************************************************************************/
void Hal_ADC_Set_SOGBW(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_ATOP_39_L, u16value, 0x1F);
}

/******************************************************************************/
///Set negative clamping duration..
///@param u16Value \b IN: set clamp delay value
/******************************************************************************/
void Hal_ADC_dtop_iClampDuration_setting(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_18_L, u16value, LBMASK);
}

/******************************************************************************/
///Set postive clamping duration..
///@param u16Value \b IN: set clamp delay value
/******************************************************************************/
void Hal_ADC_dtop_vClampDuration_setting(void *pInstance, MS_U16 u16value)
{
    W2BYTEMSK(REG_ADC_DTOP_08_L, u16value, LBMASK);
}

/******************************************************************************/
///This function enable HW mode calibration
///@param bEnable \b IN: enable:select HW adc table, disable:not select HW adc table
/******************************************************************************/
void Hal_ADC_Enable_HWCalibration(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->sthal_ADC._bEnableHWCalibration = bEnable;
}

static MS_BOOL Hal_ADC_GetBandgapTrimData(void *pInstance, MS_U8 *pu8BandgapTrimData)
{

    if(Hal_ADC_External_eFuse())
    {
        MHAL_ADC_DBG(printf("[ADC eFuse] MBOOT fill ADC efuse\n"));
        *pu8BandgapTrimData = 0;
        return TRUE;
    }
    else
    {
        //MS_U8 u8RetryCount = 0;
        // Read trimed value form eFuse
        MDrv_WriteByteMask(0x002050, 0x30, 0xFF);
        MDrv_WriteByteMask(0x002051, 0x21, 0xFF);
        MHAL_ADC_DBG(printf("[ADC eFuse] bandgap trim data init = %x\n",(*pu8BandgapTrimData)));

#if 0
        while( MDrv_ReadByte(0x002051) & BIT(5) )//waiting data  ready
        {
            if(u8RetryCount >= MHAL_ADC_EFUSE_RETRY_THRESHOLD)
            {
                *pu8BandgapTrimData = 0;
                printf("%s[ERROR] read data from efuse fail!!!\n", __FUNCTION__);
                return FALSE;
            }
            u8RetryCount++;
        }
#else
        MsOS_DelayTask(1); //wait for 1.3us
#endif

        *pu8BandgapTrimData = (MS_U8)MDrv_ReadByte(0x002058);

        if(*pu8BandgapTrimData & 0x80)
        {
            MHAL_ADC_DBG(printf("[ADC eFuse] bandgap get trim data = %x\n",(*pu8BandgapTrimData)));
            MHAL_ADC_DBG(printf("[ADC eFuse] bandgap trim data enable \n"));
            return TRUE;
        }
        else
        {
            *pu8BandgapTrimData = 0;
            MHAL_ADC_DBG(printf("[ADC eFuse] bandgap get trim data = %x\n",(*pu8BandgapTrimData)));
            MHAL_ADC_DBG(printf("[ADC eFuse] bandgap trim data disable \n"));
            return FALSE;
        }
    }

}

void Hal_ADC_SetBandgapTrimData(void *pInstance, MS_BOOL bUseLDO, MS_U8 u8EfuseBandgap)
{
	if(bUseLDO)
	{
		W2BYTEMSK(REG_ADC_ATOP_5B_L, 0 , BIT(13));
	}
	else
	{
		W2BYTEMSK(REG_ADC_ATOP_5B_L, BIT(13) , BIT(13));
		W2BYTEMSK(REG_ADC_ATOP_5B_L, ((MS_U16)u8EfuseBandgap & 0x1F) << 8 , 0x1F00);
	}
}

/******************************************************************************/
///This function set ADC calibration reference voltage soure type
///@param benable \b IN: enable or disable
/******************************************************************************/
MS_BOOL Hal_ADC_SetCalibrationRefSource(void *pInstance, MS_U16 type)
{
    MS_BOOL bDone = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch(type)
    {
        case ADC_REFERENCE_VOLATAGE_LDO:
            pXCResourcePrivate->sthal_ADC._bUseBandgap = FALSE;
            bDone  = TRUE;
            break;
        case ADC_REFERENCE_VOLATAGE_BANDGAP:
            {
                MS_U8 efuseBandgap = 0;

                if(Hal_ADC_GetBandgapTrimData(pInstance, &efuseBandgap))
                {
                    pXCResourcePrivate->sthal_ADC._bUseBandgap = TRUE;
                    bDone  = TRUE;
                    //printf("%s: efuseBandgap = 0x%x\n", __FUNCTION__, efuseBandgap & 0x1F);

                    // Write to ATOP register
                    W2BYTEMSK(REG_ADC_ATOP_5B_L , BIT(13), BIT(13));

                    if(Hal_ADC_External_eFuse())
                    {
                        MHAL_ADC_DBG(printf("ADC eFuse: Just let bandgap be overwrite by eFuse, no change bandgap efuse value which be wrote by mboot\n"););
                    }
                    else
                    {
                        W2BYTEMSK(REG_ADC_ATOP_5B_L ,((MS_U16)efuseBandgap & 0x1F) << 8, BMASK(12:8) );
                    }
                }
                else
                {
                    // Use LDO
                    W2BYTEMSK(REG_ADC_ATOP_5B_L , 0, BIT(13));
                }
            }
            break;
        default:
            break;
    }
    return bDone;
}

MS_U16 Hal_ADC_GetCalibrationRefSource(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->sthal_ADC._bUseBandgap)
        return ADC_REFERENCE_VOLATAGE_BANDGAP;
    else
        return ADC_REFERENCE_VOLATAGE_LDO;
}

/******************************************************************************/
///This function enable Internal SOG. (Use ADC G Pad instead of SOG_Ini Pad as SOG source)
///@param bEnable \b IN: enable: select internal SOG path, disable: select external SOG path.
/******************************************************************************/
void Hal_ADC_ISOG_Enable(void *pInstance, MS_BOOL bEnable)
{
    W2BYTEMSK(REG_ADC_ATOP_6B_L, (R2BYTE(REG_ADC_ATOP_6B_L) & 0x7FFF) | ( (bEnable?1:0) << 15), 0x8000);
    // Per ADC analog RD's request & make the following change:
    //   Change ISOG clock discharge lctrl counter period & width
    //     from
    //          lctrl duty = 2:200 -> 0x20C8 (default)
    //     to
    //          lctrl duty = 1:100 -> 0x1064
    W2BYTEMSK(REG_ADC_ATOP_6D_L, 0x1064, 0xFFFF);
}

/******************************************************************************/
///This function checks if internal SOG function enabled.
/******************************************************************************/
MS_BOOL Hal_ADC_ISOG_CheckEnabled(void *pInstance)
{
    return R2BYTEMSK(REG_ADC_ATOP_6B_L, 0x8000)? TRUE : FALSE;

}

/******************************************************************************/
///This function set internal SOG reset signal pulse width.
///@param u8value \b set reset signal pulse width.
/******************************************************************************/
void Hal_ADC_ISOG_SetResetWidth(void *pInstance, MS_U8 u8value)
{
    W2BYTEMSK(REG_ADC_ATOP_6C_L, (u8value & 0x0F) << 12, 0xF000);
}

/******************************************************************************/
///This function set CVBS Out IDAC current mode.
///@param u16mode \b IDAC current mode.
/******************************************************************************/
MS_BOOL Hal_ADC_SetIdacCurrentMode(void *pInstance, MS_U16 u16mode)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    MS_BOOL bReturn = TRUE;
    MS_U8 u8value = 0;

    switch(u16mode)
    {
        case ADC_IDAC_CURRENT_MODE_FULL:
            u8value = 0x00;
            break;
        case ADC_IDAC_CURRENT_MODE_HALF:
            u8value = 0x02;
            break;
        case ADC_IDAC_CURRENT_MODE_QUART:
            u8value = 0x0A;
            break;
        default:
            bReturn = FALSE;
            u8value = 0x00;
            break;
    }
    if(psXCInstPri->u32DeviceID == 0)
    {
        if(Hal_ADC_External_eFuse())
        {
            MHAL_ADC_DBG(printf("ADC eFuse: no change iDac efuse value\n"););
        }
        else
        {
            W2BYTEMSK(REG_ADC_ATOPB_08_L, (MS_U16)(u8value & 0x0F) << 12, 0xF000);
            // read idac trim data from efuse & write to register
            Hal_ADC_LoadIDacTrimData(pInstance, u16mode);
        }
    }
    pXCResourcePrivate->sthal_ADC._u16IdacCurrrenMode = u16mode;

    return bReturn;
}

/******************************************************************************/
///This function get CVBS Out IDAC current mode.
///@return IDAC current mode.
/******************************************************************************/
MS_U16 Hal_ADC_GetIdacCurrentMode(void *pInstance)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    return pXCResourcePrivate->sthal_ADC._u16IdacCurrrenMode;
}

/******************************************************************************/
///This function read IDAC trim data from efuse.
///@param  u16mode \b IDAC current mode.
///@param  pu8iDacTrimData \b IDAC trim data with flag (bit7: valid efuse data)
///@return Success or not.
/******************************************************************************/
static MS_BOOL Hal_ADC_GetIDacTrimData(void *pInstance, MS_U16 u16mode, MS_U8 *pu8iDacTrimData)
{
    MS_BOOL bReturn = TRUE;
    MS_U8 u8offset = 0;

    if(Hal_ADC_External_eFuse())
    {
        MHAL_ADC_DBG(printf("[ADC eFuse] MBOOT fill ADC iDac efuse\n"));
        *pu8iDacTrimData = 0;
        return TRUE;
    }
    else
    {
        //MS_U8 u8RetryCount = 0;
        MHAL_ADC_DBG(printf("[ADC eFuse] IDac trim data init = %x\n",(*pu8iDacTrimData)));
        switch(u16mode)
        {
            case ADC_IDAC_CURRENT_MODE_FULL:
                u8offset = 0x59;
                break;
            case ADC_IDAC_CURRENT_MODE_HALF:
                u8offset = 0x5A;
                break;
            case ADC_IDAC_CURRENT_MODE_QUART:
                u8offset = 0x5B;
                break;
            default:
                bReturn = FALSE;
                u8offset = 0x59;
                break;
        }
        // Read trimed value form eFuse
        MDrv_WriteByteMask(0x002050, 0x30, 0xFF);
        MDrv_WriteByteMask(0x002051, 0x21, 0xFF);

#if 0
        while( MDrv_ReadByte(0x002051) & BIT(5) )//waiting data  ready
        {
            if(u8RetryCount >= MHAL_ADC_EFUSE_RETRY_THRESHOLD)
            {
                *pu8iDacTrimData = 0;
                printf("%s[ERROR] read data from efuse fail!!!\n", __FUNCTION__);
                return FALSE;
            }
            u8RetryCount++;
        }
#else
        MsOS_DelayTask(1); //wait for 1.3us
#endif

        *pu8iDacTrimData = (MS_U8)MDrv_ReadByte(0x002000+u8offset);

        if(bReturn)
        {
            if(*pu8iDacTrimData & 0x80)
            {
                bReturn = TRUE;
            }
            else
            {
                bReturn = FALSE;
            }
        }

        if(!bReturn)
        {
           *pu8iDacTrimData = 0;
        }

        MHAL_ADC_DBG(printf("[ADC eFuse] IDac get trim data = %x\n",(*pu8iDacTrimData)));
        MHAL_ADC_DBG(printf("[ADC eFuse] IDac trim data enable = %d\n",bReturn));

        return bReturn;
    }
}

/******************************************************************************/
///This function read IDAC trim data from efuse, then fill the value to register.
///@param  u16mode \b IDAC current mode.
///@return Success or not.
/******************************************************************************/
static MS_BOOL Hal_ADC_LoadIDacTrimData(void *pInstance, MS_U16 u16mode)
{
    MS_U8 u8iDacTrimData = 0;

    if(Hal_ADC_GetIDacTrimData(pInstance, u16mode, &u8iDacTrimData))
    {
        if(Hal_ADC_External_eFuse())
        {
            MHAL_ADC_DBG(printf("ADC eFuse: no change iDac efuse value which be wrote by mboot\n"););
        }
        else
        {
            W2BYTEMSK((MS_U32)(REG_ADC_ATOPB_BASE+0x10), (MS_U16)u8iDacTrimData, 0x007F);
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL Hal_ADC_External_eFuse(void)
{
#ifdef ADC_EFUSE_IN_MBOOT
    return TRUE;
#else
    return FALSE;
#endif
}

void Hal_ADC_SCART_RGB_setMode(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
        //restore initial value
        if( (u16ADC_DTOP_24_Buffer==0) && (u16ADC_DTOP_2E_Buffer==0) && (u16ADC_DTOP_38_Buffer==0) )
        {
            u16ADC_DTOP_24_Buffer = R2BYTE(REG_ADC_DTOP_24_L);
            u16ADC_DTOP_2E_Buffer = R2BYTE(REG_ADC_DTOP_2E_L);
            u16ADC_DTOP_38_Buffer = R2BYTE(REG_ADC_DTOP_38_L);
        }
        // DTOP 17 [11..0]:reg_iclamp_clpdly, register setting for delay(to start of analog clamp)
        W2BYTEMSK(REG_ADC_DTOP_17_L, 0x0030 , 0x0FFF);
        // DTOP 18 [7..0]:reg_iclamp_caldur, register setting for duration, only for 2^N (of analog clamp)
        W2BYTEMSK(REG_ADC_DTOP_18_L, 0x0040 , 0x00FF);
        // DTOP 19 [14..13]:reg_iclamp_clk_rate, 00: x1 Analog ICLAMP, 01: x2, 10: x4, 11: x4
        W2BYTEMSK(REG_ADC_DTOP_19_L, 0x4000 , 0x6000);
        // DTOP 24 [6..0]:reg_iclamp_r_k1_ini, k1 = 2^(CLMP_K1[6:3]-8-2) * (1 + CLMP_K1[2:0]/8). for Initial
        W2BYTEMSK(REG_ADC_DTOP_24_L, 0x005B , 0x007F);
        // DTOP 2E [6..0]:reg_iclamp_g_k1_ini, k1 = 2^(CLMP_K1[6:3]-8-2) * (1 + CLMP_K1[2:0]/8). for Initial
        W2BYTEMSK(REG_ADC_DTOP_2E_L, 0x005B , 0x007F);
        // DTOP 38 [6..0]:reg_iclamp_b_k1_ini, k1 = 2^(CLMP_K1[6:3]-8-2) * (1 + CLMP_K1[2:0]/8). for Initial
        W2BYTEMSK(REG_ADC_DTOP_38_L, 0x005B , 0x007F);
    }
    else
    {
        if( (u16ADC_DTOP_24_Buffer!=0) && (u16ADC_DTOP_2E_Buffer!=0) && (u16ADC_DTOP_38_Buffer!=0) )
        {
            // DTOP 24 [6..0]:reg_iclamp_r_k1_ini, k1 = 2^(CLMP_K1[6:3]-8-2) * (1 + CLMP_K1[2:0]/8). for Initial
            W2BYTEMSK(REG_ADC_DTOP_24_L, u16ADC_DTOP_24_Buffer , 0x007F);
            // DTOP 2E [6..0]:reg_iclamp_g_k1_ini, k1 = 2^(CLMP_K1[6:3]-8-2) * (1 + CLMP_K1[2:0]/8). for Initial
            W2BYTEMSK(REG_ADC_DTOP_2E_L, u16ADC_DTOP_2E_Buffer , 0x007F);
            // DTOP 38 [6..0]:reg_iclamp_b_k1_ini, k1 = 2^(CLMP_K1[6:3]-8-2) * (1 + CLMP_K1[2:0]/8). for Initial
            W2BYTEMSK(REG_ADC_DTOP_38_L, u16ADC_DTOP_38_Buffer , 0x007F);
        }
    }

}

#undef MHAL_ADC_C

