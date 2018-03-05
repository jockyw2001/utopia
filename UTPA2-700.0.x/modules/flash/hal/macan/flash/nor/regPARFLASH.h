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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    regPARFLASH.h
/// @brief  Parallel Flash Register Definition
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_PARFLASH_H_
#define _REG_PARFLASH_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// BASEADDR & BK
#define BASEADDR_RIU                    0xBF000000  // TODO: <-@@@ CHIP SPECIFIC

#define BK_CHIPTOP                      0x3C00
#define BK_PFSH                         0xB00
#define BK_PIU							0x7800

//-------- PIU register ------------------------
#define REG_PIU_SPI_CLK_SRC         0x26
	#define PSCS_CLK_SRC_SEL_MASK       	BMASK(5:5)
    #define PSCS_CLK_SRC_SEL_XTAL           BITS(5:5, 0)
    #define PSCS_CLK_SRC_SEL_CLK            BITS(5:5, 1)
	#define PSCS_CLK_SEL_MASK           	BMASK(4:2)
    #define PSCS_CLK_SEL_XTAL               BITS(4:2, 0)
    #define PSCS_CLK_SEL_27MHZ              BITS(4:2, 1)
    #define PSCS_CLK_SEL_36MHZ              BITS(4:2, 2)
    #define PSCS_CLK_SEL_43MHZ              BITS(4:2, 3)
    #define PSCS_CLK_SEL_54MHZ              BITS(4:2, 4)
    #define PSCS_CLK_SEL_72MHZ              BITS(4:2, 5)
    #define PSCS_CLK_SEL_86MHZ              BITS(4:2, 6)
    #define PSCS_CLK_SEL_108MHZ             BITS(4:2, 7)
    #define PSCS_CLK_SRC_SEL_MASK       	BMASK(5:5)
    #define PSCS_CLK_SRC_SEL_XTAL           BITS(5:5, 0)
    #define PSCS_CLK_SRC_SEL_CLK            BITS(5:5, 1)

//-------------------------------------------

//----- Chip top -------------------------
#define REG_ALLPAD_IN                   0x50
#define REG_PCMCONFIG                   0x6E
#define REG_PF_MODE                     0x6F
#define REG_PCMISGPIO					0x70

//---- Nor flash register ------------------
#define REG_NORPF_CLOCK_LENGTH          0x00
#define REG_NORPF_FLASH_DATA_WIDTH8     0x00
#define REG_NORPF_WRITE_RUN             0x01
#define REG_NORPF_DIRECT_READ_MODE      0x01
#define REG_NORPF_CEB_START             0x02
#define REG_NORPF_CEB_END               0x02
#define REG_NORPF_OEB_START             0x02
#define REG_NORPF_OEB_END               0x02
#define REG_NORPF_WEB_START             0x03
#define REG_NORPF_WEB_END               0x03
#define REG_NORPF_DATAOEN_START         0x03
#define REG_NORPF_DATAOEN_END           0x03
#define REG_NORPF_DATA_LATCH_CNT        0x04
#define REG_NORPF_CTRL                  0x06
#define REG_NORPF_BRIDGE_CTRL           0x07
#define REG_NORPF_XIU_CTRL              0x08
#define REG_NORPF_XIU_ADDR_L            0x09
#define REG_NORPF_XIU_ADDR_H            0x0A
#define REG_NORPF_XIU_WDATA             0x0B
#define REG_NORPF_XIU_STATUS            0x0C
#define REG_NORPF_XIU_RDATA_L           0x0D
#define REG_NORPF_XIU_RDATA_H           0x0E
#define REG_NORPF_WRITE_ADDR0_L         0x10
#define REG_NORPF_WRITE_ADDR0_H         0x11
#define REG_NORPF_WRITE_ADDR1_L         0x12
#define REG_NORPF_WRITE_ADDR1_H         0x13
#define REG_NORPF_WRITE_ADDR2_L         0x14
#define REG_NORPF_WRITE_ADDR2_H         0x15
#define REG_NORPF_WRITE_ADDR3_L         0x16
#define REG_NORPF_WRITE_ADDR3_H         0x17
#define REG_NORPF_WRITE_ADDR4_L         0x18
#define REG_NORPF_WRITE_ADDR4_H         0x19
#define REG_NORPF_WRITE_ADDR5_L         0x1A
#define REG_NORPF_WRITE_ADDR5_H         0x1B
#define REG_NORPF_WRITE_ADDR6_L         0x1C
#define REG_NORPF_WRITE_ADDR6_H         0x1D
#define REG_NORPF_WRITE_ADDR7_L         0x1E
#define REG_NORPF_WRITE_ADDR7_H         0x1F
#define REG_NORPF_WRITE_DATA0           0x20
#define REG_NORPF_WRITE_DATA1           0x21
#define REG_NORPF_WRITE_DATA2           0x22
#define REG_NORPF_WRITE_DATA3           0x23
#define REG_NORPF_WRITE_DATA4           0x24
#define REG_NORPF_WRITE_DATA5           0x25
#define REG_NORPF_WRITE_DATA6           0x26
#define REG_NORPF_WRITE_DATA7           0x27


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_PARFLASH_H_
