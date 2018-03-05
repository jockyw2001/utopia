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



#define PCMBURST_WRITE           0x0001
#define PCMBURST_READ            0x0010
#define PCMBURST_STATUS          0x0100


#define REG_COMPANION_TOP_BANK       0x101E00
    #define REG_CHIPTOP_ALLPAD_IN                   REG_COMPANION_TOP_BANK + 0xA1UL
        #define ALL_PAD_IN_MASK                     0x80UL
        #define ALL_PAD_IN_DISABLE                  0x00UL
    #define REG_CHIPTOP_PCM_EN                      REG_COMPANION_TOP_BANK + 0xA3UL
        #define PCM_ENABLE_MASK                     0x03UL
        #define PCM_ENABLE                          0x03UL
    #define REG_CHIPTOP_PCM_PE                      REG_COMPANION_TOP_BANK + 0x81UL
        #define PE_ENABLE                           0xFFUL

#define REG_COMPANION_CLKGEN0_BANK   0x100B00
    #define REG_CLKGEN0_PCM                         REG_COMPANION_CLKGEN0_BANK + 0x34UL
        #define CLK_ON                              0x00UL
        #define CLK_OFF                             0x01UL

#define REG_COMPANION_INTR_CTRL_BANK 0x101900
#define REG_COMPANION_PCM_BANK       0x103500
#define REG_COMPANION_PCM2_BANK      0x103600
#define REG_COMPANION_PCMBURST_BANK  0x103700

#define REG_COMPANION_PCMBURST_ADDR  0x800002F4 // bank 1037, addr 0x7A)
#define REG_COMPANION_PCMBURST_WFIFO 0x80000300
#define REG_COMPANION_PCMBURST_RFIFO 0x80000A00


#define MSPI_CMD_RIU_W1         0x1D
// W : | RIU_W (1byte)   | ADDR (2byte) | data ...                   
#define MSPI_CMD_RIU_RIT1       0x15
// W : | RIU_RT1 (1byte) | ADDR (2byte) | TA (turn around, 1byte) |  
#define MSPI_CMD_MIU_W          0x25
// W : | MIU_W (1byte)   | ADDR (4byte) | data ...
#define MSPI_CMD_MIU_R          0x20
// W : | MIU_R (1byte)   | ADDR (4byte) | len = 0 (1byte)
#define MSPI_CMD_MIU_ST         0x21
// W : | MIU_ST (1byte)  | 0 (1byte)
#define MSPI_CMD_CFG_W          0x05

#define MSPI_MIU_STATUS_NONE    0x00 // Initial status
#define MSPI_MIU_STATUS_WBUSY   0x01 // FIFO is not empty or MIU IF is busy
#define MSPI_MIU_STATUS_RBUSY   0x02 // FIFO is not empty or MIU IF is busy
#define MSPI_MIU_STATUS_OV      0x03 // FIFO overflow
#define MSPI_MIU_STATUS_COL     0x04 // SSPI and MIU interface collision
#define MSPI_MIU_STATUS_DONE    0x0A // RIU_W or RIU_R done



#define MAX_MSPI_BURST_WRITE_SIZE           65536
#define MAX_MSPI_BURST_READ_SIZE            32
#define MAX_PCMCIA_BURST_WRITE_SIZE         128
#define MAX_PCMCIA_BURST_READ_SIZE          128

#define MAX_MSPI_STATUS_COUNT               0x1024

#define REG_PCMCIA_BASE                     0x3500UL
#define REG_CLKGEN0_BASE                    0x0B00UL

#define REG_PCMCIA_PCM_MEM_IO_CMD           REG_COMPANION_PCM_BANK + 0x00UL
#define REG_PCMCIA_ADDR0                    REG_COMPANION_PCM_BANK + 0x02UL
#define REG_PCMCIA_ADDR1                    REG_COMPANION_PCM_BANK + 0x03UL
#define REG_PCMCIA_WRITE_DATA               REG_COMPANION_PCM_BANK + 0x04UL
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     REG_COMPANION_PCM_BANK + 0x06UL
#define REG_PCMCIA_READ_DATA                REG_COMPANION_PCM_BANK + 0x08UL
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  REG_COMPANION_PCM_BANK + 0x09UL
#define REG_PCMCIA_INT_MASK_CLEAR           REG_COMPANION_PCM_BANK + 0x0AUL
#define REG_PCMCIA_INT_MASK_CLEAR1          REG_COMPANION_PCM_BANK + 0x0BUL
#define REG_PCMCIA_STAT_INT_RAW_INT         REG_COMPANION_PCM_BANK + 0x0EUL
#define REG_PCMCIA_STAT_INT_RAW_INT1        REG_COMPANION_PCM_BANK + 0x0FUL
#define REG_PCMCIA_MODULE_VCC_OOB           REG_COMPANION_PCM_BANK + 0x10UL

#define REG_PCM_BURST_CTRL                  REG_COMPANION_PCMBURST_BANK + 0x00UL
    #define REG_PCM_BURST_SW_RST_ON         0x01UL
    #define REG_PCM_BURST_SW_RST_OFF        0x00UL
    #define REG_PCM_BURST_SW_RST_MASK       0x01UL

#define REG_PCM_BURST_STATUS_CLR            REG_COMPANION_PCMBURST_BANK + 0x8UL
    #define REG_PCM_WRITE_FINISH_CLR        0x01UL
    #define REG_PCM_READ_FINISH_CLR         0x02UL
    #define REG_PCM_STATUS_READ_FINISH_CLR  0x04UL
    
#define REG_PCM_BURST_STATUS_0              REG_COMPANION_PCMBURST_BANK + 0x14UL
    #define REG_PCM_BURST_WRITE_DONE        0x01UL
    #define REG_PCM_BURST_READ_DONE         0x02UL
    #define REG_PCM_BURST_READ_STATUS_DONE  0x04UL
             
#define REG_PCM_BURST_STATUS_1              REG_COMPANION_PCMBURST_BANK + 0x16UL
    #define REG_PCM_WFIFO_EMPTY             0x01UL
    #define REG_PCM_WFIFO_FULL              0x02UL
    #define REG_PCM_RFIFO_EMPTY             0x40UL
    #define REG_PCM_RFIFO_FULL              0x80UL
                  
#define REG_PCM_BURST_WFIFO_RMN             REG_COMPANION_PCMBURST_BANK + 0x18UL // [6:0] max 127
#define REG_PCM_BURST_RFIFO_RMN             REG_COMPANION_PCMBURST_BANK + 0x19UL // [6:0] max 127
    
#define REG_HST0_IRQ_MASK_15_0              REG_COMPANION_INTR_CTRL_BANK + 0x29UL
    #define REG_PCM_IRQ_BIT_SHFT            2UL
    #define REG_PCM_IRQ_BIT_MASK            (0x1UL << REG_PCM_IRQ_BIT_SHFT)            
    #define IRQ_MASK                        (1UL << REG_PCM_IRQ_BIT_SHFT)
    #define IRQ_UNMASK                      (0UL << REG_PCM_IRQ_BIT_SHFT)      
    
#define PCMCIA_ATTRIBMEMORY_READ            0x03UL
#define PCMCIA_ATTRIBMEMORY_WRITE           0x04UL
#define PCMCIA_IO_READ                      0x05UL
#define PCMCIA_IO_WRITE                     0x06UL

#define PCM_OOB_BIT_MASK                    0x03UL
#define PCM_OOB_BIT_SHFT                    6UL

#define PCM_OOB_CYCLE_EXTEND                0x3UL
                                            // 00:th(CE)=4T (extend 3 active cycle)
                                            // 01:th(CE)=3T (extend 2 active cycle) 
                                            // 10:th(CE)=2T (extend 1 active cycle)
                                            // 11:th(CE)=1T 

//-----------------------------------------------------------------------------
//  Type and Structure
//-----------------------------------------------------------------------------

#endif // _REG_PCMCIA_H_
