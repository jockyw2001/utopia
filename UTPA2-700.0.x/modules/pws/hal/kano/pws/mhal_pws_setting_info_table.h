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
#ifndef _PWS_SETTING_INFO_TABLE_H_
#define _PWS_SETTING_INFO_TABLE_H_

#include "MsTypes.h"

#define TBL_STR_LEN     32
typedef   struct   {
    MS_U32     u32RegAddr;
    MS_U8     u8RegMask;
    /*MS_U16     u16OffOnFlag;*/
    MS_BOOL     bInvert;
    const char regName[TBL_STR_LEN];//*regName;
}PWS_TABLE_INFO;

static const PWS_TABLE_INFO pws_setting_info_table[] =
{
  //demod_atop
  {0x112818, 0x04/*, 0x0001*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_CLPBUF"},
  {0x11281E, 0x80/*, 0x0001*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_VCALBUF"},
  {0x112818, 0x01/*, 0x0001*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_ADCI"},
  {0x112818, 0x02/*, 0x0001*/ /*off=0x01, on=0x00*/, FALSE, "PWDN_ADCQ"},
  {0x112860, 0x02/*, 0x0001*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_ADC_CLK_PD"},
};

static const PWS_TABLE_INFO pws_init_setting_info_table[] =
{
  {0x11286A, 0x80/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_PD"},
  {0x101e3C, 0x01/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "PD_3P3_1"},
  {0x101e3C, 0x02/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "PD_3P3_2"},
  {0x101e3C, 0x04/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "PD_3P3_3"},
  {0x11286B, 0x20/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "PD_DMPLL_CLK"},
  {0x000e5f, 0x02/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "PD_CKBUF"},
  {0x112860, 0x04/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV2_PD"},
  {0x112860, 0x08/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV3_PD"},
  {0x112860, 0x10/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV4_PD"},
  {0x112860, 0x80/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV5_PD"},
  {0x112861, 0x01/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV7_PD"},
  {0x112860, 0x20/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV8_PD"},
  {0x112860, 0x40/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV10_PD"},
  {0x112861, 0x02/*, 0x0000*/ /*off=0x01, on=0x00*/, FALSE, "MPLL_DIV14_PD"},
};


#endif
