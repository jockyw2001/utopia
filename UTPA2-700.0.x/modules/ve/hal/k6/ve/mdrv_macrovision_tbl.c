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

//****************************************************
// MACROVISION Drive Chip           : Uranus4_Macrovision_Driver
// MACROVISION Excel CodeGen Version: 1.04
// MACROVISION Excel SW      Version: 1.1
// MACROVISION Excel update date    : 2013/9/17 11:39
//****************************************************

#ifndef _DRVMACROVISION_TBL_C_
#define _DRVMACROVISION_TBL_C_

#include "MsCommon.h"
#include "mdrv_macrovision_tbl.h"

//****************************************************
// MACROVISION  MACROVISION_NTSC_TYPE1
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TYPE1_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TYPE1_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x36/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x25/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x38/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x09/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x85/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xD4/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x1F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_NTSC_TYPE2
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TYPE2_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TYPE2_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x25/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x38/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x09/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x85/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xD4/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x1F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_NTSC_TYPE3
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TYPE3_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TYPE3_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x17/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x21/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x09/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x85/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xD4/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x1F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_NTSC_TEST_N01
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TEST_N01_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TEST_N01_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x17/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x21/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x19/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x7E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x0E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x91/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x85/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xEC/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x14/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x1E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x78/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x18/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_NTSC_TEST_N02
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TEST_N02_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TEST_N02_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x2F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x0A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x1A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x0D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x20/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x24/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x25/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xB8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x36/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0xCF/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x6D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x23/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x13/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x70/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x94/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xF7/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x06/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x78/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_NTSC_TYPE2_TTX
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TYPE2_TTX_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TYPE2_TTX_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x25/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x38/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x1B/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x09/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x04/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x85/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xD4/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0xF8/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x1F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_NTSC_TEST_N01_TTX
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_NTSC_TEST_N01_TTX_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_NTSC_TEST_N01_TTX_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x17/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x21/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x40/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x28/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x19/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x0C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x7E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x0F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x0E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x91/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x85/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xEC/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x14/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x1E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x78/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x18/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_PAL_TYPE1_2_3
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_PAL_TYPE1_2_3_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_PAL_TYPE1_2_3_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x36/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x2A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x10/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x3D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xFE/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x03/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x54/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0xFE/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x7E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x9D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xDC/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x20/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x50/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x15/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_PAL_TEST_P01
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_PAL_TEST_P01_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_PAL_TEST_P01_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x2A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x10/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xE0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x3D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xFE/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x03/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x54/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0xFE/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x7E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x9D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xDC/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x20/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x50/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x15/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_PAL_TEST_P02
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_PAL_TEST_P02_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_PAL_TEST_P02_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 //{ DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x2A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x10/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x23/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x12/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0xC0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x0A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0xC6/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x78/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x43/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x1F/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0x53/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0xA3/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0xF0/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x8E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xF1/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x18/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x38/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x14/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_PAL_TYPE1_2_3_TTX
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_PAL_TYPE1_2_3_TTX_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_PAL_TYPE1_2_3_TTX_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x36/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x2A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x10/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x3D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x03/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x54/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0xFE/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x7E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x9D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xDC/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x20/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x50/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x15/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

//****************************************************
// MACROVISION  MACROVISION_PAL_TEST_P01_TTX
//****************************************************
MS_U8 MST_MACROVISION_MACROVISION_PAL_TEST_P01_TTX_MACROVISION_TBL[][REG_ADDR_SIZE+REG_MASK_SIZE+VE_TAB_MACROVISION_PAL_TEST_P01_TTX_MACROVISION_NUMS]=
{                 // Reg           Mask  Value
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0xFF, 0x3E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7E_L), 0x01, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0x3F, 0x1A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_01_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0x3F, 0x2A/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_L), 0xC0, 0x80/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_02_H), 0x0F, 0x08/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x07, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0x38, 0x10/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_03_L), 0xE0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_L), 0x00, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_78_H), 0x3F, 0x1C/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0x3F, 0x3D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_L), 0xC0, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_79_H), 0x0F, 0x05/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_L), 0xFF, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7A_H), 0x7F, 0x03/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_L), 0xFF, 0x54/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7B_H), 0x7F, 0x01/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_L), 0xFF, 0xFE/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7C_H), 0xFF, 0x7E/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_L), 0xFF, 0x60/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_L), 0xFF, 0x9D/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7F_H), 0xFF, 0xDC/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x7F, 0x20/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_L), 0x80, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_00_H), 0x3F, 0x00/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0x07, 0x07/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_L), 0xF8, 0x50/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC2_04_H), 0x1F, 0x15/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TC_VE_ENC1_7D_H), 0x02, 0x02/*ALL*/, },
 { DRV_MACROVISION_REG(REG_TABLE_END)   , 0x00, 0x00,  }
};

MACROVISION_TAB_INFO MACROVISIONMAP_Main[MACROVISION_TYPE_NUMS]=
{
{
*MST_MACROVISION_MACROVISION_NTSC_TYPE1_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_NTSC_TYPE2_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_NTSC_TYPE3_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_NTSC_TEST_N01_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_NTSC_TEST_N02_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_NTSC_TYPE2_TTX_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_NTSC_TEST_N01_TTX_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_PAL_TYPE1_2_3_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_PAL_TEST_P01_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_PAL_TEST_P02_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_PAL_TYPE1_2_3_TTX_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
{
*MST_MACROVISION_MACROVISION_PAL_TEST_P01_TTX_MACROVISION_TBL, MACROVISION_TABTYPE_MACROVISION, 
},
};

#endif
