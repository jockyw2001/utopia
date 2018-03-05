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
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////

#ifndef _REG_PCMCIA_H_
#define _REG_PCMCIA_H_

//-----------------------------------------------------------------------------
//  Hardware Capability
//-----------------------------------------------------------------------------
// Base address should be initial.
#define PCMCIA_RIU_MAP u32PCMCIA_RIU_BaseAdd  //obtain in init

//-----------------------------------------------------------------------------
//  Macro and Define
//-----------------------------------------------------------------------------
//hardware spec
#define REG_PCMCIA_PCM_MEM_IO_CMD           0x00UL
#define REG_PCMCIA_ADDR0                    0x02UL
#define REG_PCMCIA_ADDR1                    0x03UL
#define REG_PCMCIA_WRITE_DATA               0x04UL
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     0x06UL
#define REG_PCMCIA_READ_DATA                0x08UL
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  0x09UL
#define REG_PCMCIA_INT_MASK_CLEAR           0x0AUL
#define REG_PCMCIA_INT_MASK_CLEAR1          0x0BUL
#define REG_PCMCIA_STAT_INT_RAW_INT         0x0EUL
#define REG_PCMCIA_STAT_INT_RAW_INT1        0x0FUL
#define REG_PCMCIA_MODULE_VCC_OOB           0x10UL

#define PCMCIA_ATTRIBMEMORY_READ            0x03UL
#define PCMCIA_ATTRIBMEMORY_WRITE           0x04UL
#define PCMCIA_IO_READ                      0x05UL
#define PCMCIA_IO_WRITE                     0x06UL

#define REG_PCMCIA_BASE                     0x3440UL
#define PCMCIA_BASE_ADDRESS	                ( PCMCIA_RIU_MAP + ( REG_PCMCIA_BASE * 2 ) )

#if defined ( MCU_AEON )
#define PCMCIA_RIU_DEFAULT_BASE_ADDR        0xA0000000UL;
#elif defined ( MCU_MIPS )
#define PCMCIA_RIU_DEFAULT_BASE_ADDR        0xBF200000UL;
#endif

#define PCM_OOB_BIT_MASK                    0x03UL
#define PCM_OOB_BIT_SHFT                    6UL

#define PCM_OOB_CYCLE_EXTEND                0x3UL
                                            // 00:th(CE)=4T (extend 3 active cycle)
                                            // 01:th(CE)=3T (extend 2 active cycle) 
                                            // 10:th(CE)=2T (extend 1 active cycle)
                                            // 11:th(CE)=1T 
#define PCM_CLKGEN0_REG(addr)               (*((volatile MS_U16*)(PCMCIA_RIU_MAP + 0x1600UL + ((addr)<<2UL))))
    #define REG_CLKGEN0_PCM_CLK             0x001AUL
        #define REG_CLKGEN0_PCM_CLK_MASK    0x000FUL
        #define REG_CLKGEN0_PCM_CLK_DIS     0x0001UL
//-----------------------------------------------------------------------------
//  Type and Structure
//-----------------------------------------------------------------------------

#endif // _REG_PCMCIA_H_
