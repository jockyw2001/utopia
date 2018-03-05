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



//==============================================================================
/// [MsAce_LIB_Group_DTV1.h]
/// Date: 20090414
/// Descriptions:
//==============================================================================


#ifndef MSACE_LIB_GROUP_DTV1_H
#define MSACE_LIB_GROUP_DTV1_H

#include "ace_hwreg_utility2.h"
#include "hwreg_ace.h"

/////////////// Chakra2 define start

#define BOOL    MS_BOOL
#define BYTE    MS_U8
#define WORD    MS_U16
#define DWORD   MS_U32
#define U8      MS_U8
#define code


#define BK_SELECT_00                BK_SCALER_BASE
#define REG_BANK_VOP                0x10
#define REG_BANK_S_VOP              0x0F
#define REG_BANK_ACE                0x18

#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define L_BK_VOP(x)                 BK_REG_L(BK_SCALER_BASE,x)
#define L_BK_ACE(x)                 BK_REG_L(BK_SCALER_BASE,x)

//////////////// Chakra2 define end


#define XDATA

#define UNAVAILABLE             0xFF

#define REG_ADDR_SC_BANK_SEL    BK_SELECT_00
#define BANK_CM_MAIN            REG_BANK_VOP
#define BANK_CM_SUB             REG_BANK_S_VOP
#define BANK_ACE                REG_BANK_ACE
#define REG_ADDR_CM_MAIN        L_BK_VOP(0x26)
#define REG_ADDR_CM_SUB         L_BK_VOP(0x1D)
#define REG_ADDR_CM_CTL_MAIN    L_BK_VOP(0x2F)
#define REG_ADDR_CM_CTL_SUB     L_BK_VOP(0x26)
#define REG_ADDR_ACE_R2Y_EQ_SEL L_BK_ACE(0x6F)
#define ENABLE_CM_CTL1          (0x35)
#define ENABLE_CM_CTL2          (0x30)
#define MASK_CM_CTL             (0x37)
#define MASK_CM_CTL_EN          (0x10)
#define MASK_CM_CTL_RB_RANGE    (0x05)

#define DECLARA_CMCTL_VARIABLE              U8 CTLVal;
#if SCALER_REGISTER_SPREAD
#define DECLARA_BANK_VARIABLE               MS_U32 u32Bank = 0;
#define BACKUP_SC_BANK                      u32Bank = u32Bank;
#define RECOVER_SC_BANK
#define ACE_WRITE_CM(id, addr, data)	        SC_W2BYTE(id, ((DWORD)addr | (u32Bank << 8)),data)
#define ACE_WRITE_CM_CTL(id, addr, data, mask)	SC_W2BYTEMSK(id, ((DWORD)addr | (u32Bank << 8)),data,mask)
#define ACE_READ_CTL(id, addr, mask)			SC_R2BYTEMSK(id, ((DWORD)addr | (u32Bank << 8)),mask)
#else
#define DECLARA_BANK_VARIABLE               U8 u8Bank;
#define BACKUP_SC_BANK                      {u8Bank = GET_SC_BANK();}
#define RECOVER_SC_BANK                     {SET_SC_BANK(u8Bank);}
#define ACE_WRITE_CM(id, addr, data)		    MDrv_Write2Byte(L_BK_VOP((addr)/2), (data))
#define ACE_WRITE_CM_CTL(id, addr, data, mask)	MDrv_WriteByteMask(addr, data, mask)
#define ACE_READ_CTL(id, addr, mask)			MDrv_ReadRegBit(addr, mask)
#endif

// For scaler bank switch
#if SCALER_REGISTER_SPREAD
#define GET_SC_BANK()
#define SET_SC_BANK(bank)       u32Bank = bank;
#else
#define GET_SC_BANK()           MDrv_ReadByte(REG_ADDR_SC_BANK_SEL)
#define SET_SC_BANK(bank)       MDrv_WriteByte(REG_ADDR_SC_BANK_SEL, bank)
#endif
// Control sin&cos function
#define USE_ACE_INTERNAL_SIN_COS	0
#define sin     clampsin
#define cos     clampcos
 
#endif
