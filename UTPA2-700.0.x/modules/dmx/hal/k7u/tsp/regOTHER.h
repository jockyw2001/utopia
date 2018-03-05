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
// Copyright (c) 2011-2013 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regOTHER.h
//  Description: TSP Resample / Misc. Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_OTHER_H_
#define _REG_OTHER_H_

//--------------------------------------------------------------------------------------------------
//  Abbreviation
//--------------------------------------------------------------------------------------------------
// Addr                             Address
// Buf                              Buffer
// Clr                              Clear
// CmdQ                             Command queue
// Cnt                              Count
// Ctrl                             Control
// Flt                              Filter
// Hw                               Hardware
// Int                              Interrupt
// Len                              Length
// Ovfw                             Overflow
// Pkt                              Packet
// Rec                              Record
// Recv                             Receive
// Rmn                              Remain
// Reg                              Register
// Req                              Request
// Rst                              Reset
// Scmb                             Scramble
// Sec                              Section
// Stat                             Status
// Sw                               Software
// Ts                               Transport Stream
// MMFI                             Multi Media File In

//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct _REG_OTHER_VQ_TX
{
    REG32       reg_vq_base;
    REG16       reg_vq_size_208byte;
    REG16       reg_vq_ctrl;
        #define REG_OTHER_VQ_TX_REG_VQ_PRIORITY_THRESHOLD_MASK                  0x001F
        #define REG_OTHER_VQ_TX_REG_VQ_PRIORITY_THRESHOLD_SHIFT                 0
        #define REG_OTHER_VQ_TX_REG_VQ_RESET                                    0x0100
        #define REG_OTHER_VQ_TX_REG_VQ_OVERFLOW_INT_EN                          0x0200
        #define REG_OTHER_VQ_TX_REG_VQ_CLR_OVERFLOW_INT                         0x0400
        #define REG_OTHER_VQ_TX_REG_READ_VQ_EVER_FULL                           0x1000
        #define REG_OTHER_VQ_TX_REG_READ_VQ_EVER_OVERFLOW                       0x2000
        #define REG_OTHER_VQ_TX_REG_VQ_EMPTY                                    0x4000
        #define REG_OTHER_VQ_TX_REG_READ_VQ_BUSY                                0x8000
    REG16       reg_vq_status;
        #define REG_OTHER_VQ_TX_REG_VQ_STATUS_EMPTY                             0x0001
        #define REG_OTHER_VQ_TX_REG_VQ_STATUS_FULL                              0x0002
        #define REG_OTHER_VQ_TX_REG_VQ_STATUS_WR_EVER_LEVEL_MASK                0x000C
        #define REG_OTHER_VQ_TX_REG_VQ_STATUS_WR_EVER_LEVEL_SHIFT               2

    REG16       reg_vq_config0;

    REG32       reg_vq_reserved;

} REG_OTHER_VQ_TX;

typedef struct _REG_OTHER_Ctrl // OTHER (Bank:0x1702)
{
    REG16       CFG_OTHER_00_0F[0x10 - 0x0];                                    // reserved (for Resample)

    REG16       CFG_OTHER_10_12[0x13 - 0x10];                                   // reg_hw_semaphore

    REG16       CFG_OTHER_13;
        #define CFG_OTHER_13_REG_3WIRE_SERIAL_MODE_EN                           0x0001
        #define CFG_OHTER_13_REG_TSP2MI_REQ_MCM_DISABLE                         0x0100
        #define CFG_OHTER_13_REG_TSP2MI_MASK_MCM_FILEIN                         0x0200

    REG16       CFG_OTHER_14;
        #define CFG_OHTER_14_REG_OR_WRITE_FIX_FOR_NEW_MIU_ARBITER_DISABLE       0x0001
        #define CFG_OHTER_14_REG_CPU_LOAD_CODE_ONLY_ONE_TIME_BY_TEE             0x0002
        #define CFG_OHTER_14_REG_CPU_SECURE_STATUS                              0x0004
        #define CFG_OHTER_14_REG_SCR_BIT_AFTER_CA                               0x0008

    REG16       CFG_OTHER_15;                                                   // reg_filein_int
        #define CFG_OTHER_15_REG_FILEIN_INT_EN_MASK                             0x00FF
        #define CFG_OTHER_15_REG_FILEIN_INT_EN_SHIFT                            0
        #define CFG_OTHER_15_FILEIN0_RDONE_INT_EN                               0x0001
        #define CFG_OTHER_15_FILEIN1_RDONE_INT_EN                               0x0002
        #define CFG_OTHER_15_FILEIN2_RDONE_INT_EN                               0x0004
        #define CFG_OTHER_15_FILEIN3_RDONE_INT_EN                               0x0008
        #define CFG_OTHER_15_FILEIN4_RDONE_INT_EN                               0x0010
        #define CFG_OTHER_15_FILEIN5_RDONE_INT_EN                               0x0020
        #define CFG_OTHER_15_FILEIN6_RDONE_INT_EN                               0x0040

        #define CFG_OTHER_15_REG_FILEIN_INT_STATUS_MASK                         0xFF00
        #define CFG_OTHER_15_REG_FILEIN_INT_STATUS_SHIFT                        8
        #define CFG_OTHER_15_FILEIN0_RDONE_STATUS                               0x0100
        #define CFG_OTHER_15_FILEIN1_RDONE_STATUS                               0x0200
        #define CFG_OTHER_15_FILEIN2_RDONE_STATUS                               0x0400
        #define CFG_OTHER_15_FILEIN3_RDONE_STATUS                               0x0800
        #define CFG_OTHER_15_FILEIN4_RDONE_STATUS                               0x1000
        #define CFG_OTHER_15_FILEIN5_RDONE_STATUS                               0x2000
        #define CFG_OTHER_15_FILEIN6_RDONE_STATUS                               0x4000

    REG16       CFG_OTHER_16;
        #define CFG_OHTER_16_REG_CLR_SRAM_COLLISION                             0x0001
        #define CFG_OHTER_16_REG_PREVENT_SRAM_COLLISION                         0x0002
        #define CFG_OHTER_16_REG_RW_CONDITION_0                                 0x0004
        #define CFG_OHTER_16_REG_RW_CONDITION_1                                 0x0008
        #define CFG_OHTER_16_REG_BURST4_NEW_MODE_0                              0x0010
        #define CFG_OHTER_16_REG_BURST4_NEW_MODE_1                              0x0020
        #define CFG_OHTER_16_REG_MASK_DMA_OVERFLOW                              0x0040
        #define CFG_OHTER_16_REG_FIX_PINPON_SYNCP_IN                            0x0080
        #define CFG_OHTER_16_REG_FIXED_DMA_WADDR_NEXT_OVERFLOW                  0x0100

    REG16       CFG_OTHER_17;                                                   // reserved

    REG16       CFG_OTHER_18;
        #define CFG_OTHER_18_REG_PVR1_DMAW_PROTECT_EN                           0x0001
        #define CFG_OTHER_18_REG_MMFI0_DMAR_PROTECT_EN                          0x0100
        #define CFG_OTHER_18_REG_MMFI0_ILLEGAL_ADDR_0                           0x0400
        #define CFG_OTHER_18_REG_MMFI0_ILLEGAL_MIU_NS_EN                        0x1000
        #define CFG_OTHER_18_REG_DISABLE_MMFI0_ADDR_LEN_BY_TEE                  0x4000

    REG16       CFG_OTHER_19;
        #define CFG_OTHER_19_REG_FILEIN0_DMAR_PROTECT_EN                        0x0001
        #define CFG_OTHER_19_REG_FILEIN0_ILLEGAL_ADDR_0                         0x0100

    REG16       CFG_OTHER_1A;
        #define CFG_OTHER_1A_REG_FILEIN0_ILLEGAL_MIU_NS_EN                      0x0001
        #define CFG_OTHER_1A_REG_DISABLE_FILEIN0_ADDR_LEN_BY_TEE                0x0100

    REG16       CFG_OTHER_1B_1E[0x1F - 0x1B];                                   // reserved

    REG16       CFG_OTHER_1F;
        #define CFG_OTHER_1F_REG_SRC_AES_PVR_KEY_MASK                           0x000F
        #define CFG_OTHER_1F_REG_SRC_AES_PVR_KEY_SHIFT                          0
        #define CFG_OTHER_1F_REG_SRC_AES_FILEIN_KEY_MASK                        0x00F0
        #define CFG_OTHER_1F_REG_SRC_AES_FILEIN_KEY_SHIFT                       4

    REG16       CFG_OTHER_20_27[0x28 - 0x20];                                   // reg_aes_key_pvr

    REG16       CFG_OTHER_28_2F[0x30 - 0x28];                                   // reg_aes_key_filein

    REG_OTHER_VQ_TX     CFG_OTHER_30_67[7];                                     // vqtx 0 ~ 6

    REG16       CFG_OTHER_68_6F[0x70 - 0x68];                                   // reserved

    REG16       CFG_OTHER_70;
        #define CFG_OTHER_70_REG_VQ_FORCEFIRE_CNT_1K_MASK                       0x003F
        #define CFG_OTHER_70_REG_VQ_FORCEFIRE_CNT_1K_SHIFT                      0
        #define CFG_OTHER_70_REG_VQ_STATUS_CLR                                  0x0040
        #define CFG_OTHER_70_REG_VQ_IDLE_CNT_DISABLE                            0x0080
        #define CFG_OTHER_70_REG_VQ_WR_THRESHOLD_MASK                           0x1F00
        #define CFG_OTHER_70_REG_VQ_WR_THRESHOLD_SHIFT                          8
        #define CFG_OTHER_70_REG_VQ_RX_ARBITER_MODE_MASK                        0xC000
        #define CFG_OTHER_70_REG_VQ_RX_ARBITER_MODE_SHIFT                       14

    REG16       CFG_OTHER_71_74[0x75 - 0x71];                                   // reg_vq_rx_priority
        #define CFG_OTHER_VQ_RX_PRIORITY_EVEN_MASK                              0x007F
        #define CFG_OTHER_VQ_RX_PRIORITY_EVEN_SHIFT                             0
        #define CFG_OTHER_VQ_RX_PRIORITY_ODD_MASK                               0x7F00
        #define CFG_OTHER_VQ_RX_PRIORITY_ODD_SHIFT                              8

    REG16       CFG_OTHER_75;
        #define CFG_OTHER_75_REG_VQ_TX_BLOCK_DISABLE                            0x0001
        #define CFG_OTHER_75_REG_FIXED_MIU_REQ_FLUSH                            0x0100
        #define CFG_OTHER_75_REG_CHECK_BURST_LEN                                0x0200
        #define CFG_OTHER_75_REG_CLR_NO_HIT_INT                                 0x0400
        #define CFG_OTHER_75_REG_VQ_RX_THRESHOLD_MASK                           0x3800
        #define CFG_OTHER_75_REG_VQ_RX_THRESHOLD_SHIFT                          11

    REG16       CFG_OTHER_76_7D[0x7E - 0x76];                                   // reserved

    REG16       CFG_OTHER_7E;                                                   // reg_reset_ctrl
        #define CFG_OTHER_7E_REG_VQ_TOP_RESET                                   0x0001
        #define CFG_OTHER_7E_REG_VQ_RX_RESET                                    0x0002
        #define CFG_OTHER_7E_REG_VQ_TX_RESET                                    0x0004

    REG16       CFG_OTHER_7F;
        #define CFG_OTHER_7E_REG_CLK_GATING_VQ_TX_TSP                           0x0001
        #define CFG_OTHER_7E_REG_CLK_GATING_VQ_TX_MIU                           0x0100

} REG_OTHER_Ctrl;

#endif // _REG_OTHER_H_
