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
//  File name: regPVR.h
//  Description: TSP PVR Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_PVR_H_
#define _REG_PVR_H_

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

typedef struct _REG_PVR_ENG_Ctrl // PVR 1~8 (Bank:0x300A ~ 0x300D) , PVR 9~10 (Bank:0x162B)
{
    REG16       CFG_PVR_00;
        #define CFG_PVR_00_REG_PVR_PINGPONG_EN                                  0x0001
        #define CFG_PVR_00_REG_PVR_STR2MI_EN                                    0x0002
        #define CFG_PVR_00_REG_PVR_STR2MI_RST_WADR                              0x0004
        #define CFG_PVR_00_REG_PVR_STR2MI_PAUSE                                 0x0008
        #define CFG_PVR_00_REG_PVR_PKT192_EN                                    0x0010
        #define CFG_PVR_00_REG_PVR_DMA_FLUSH_EN                                 0x0020
        #define CFG_PVR_00_REG_PVR_BURST_LEN_MASK                               0x00C0
        #define CFG_PVR_00_REG_PVR_BURST_LEN_SHIFT                              6
        #define CFG_PVR_00_REG_PVR_LPCR1_WLD                                    0x0100
        #define CFG_PVR_00_REG_PVR_ALIGN_EN                                     0x0200
        #define CFG_PVR_00_REG_PVR_STR2MI_DSWAP                                 0x0400
        #define CFG_PVR_00_REG_PVR_STR2MI_BT_ORDER                              0x0800
        #define CFG_PVR_00_REG_REC_DATA_INV_EN                                  0x1000
        #define CFG_PVR_00_REG_REC_ALL_OLD                                      0x4000
        #define CFG_PVR_00_REG_PVR_LPCR1_RLD                                    0x8000

    REG32       CFG_PVR_01_02;                                                  // reg_pvr_str2mi_head

    REG32       CFG_PVR_03_04;                                                  // reg_pvr_str2mi_mid

    REG32       CFG_PVR_05_06;                                                  // reg_pvr_str2mi_tail

    REG32       CFG_PVR_07_08;                                                  // reg_pvr_str2mi_head2

    REG32       CFG_PVR_09_0A;                                                  // reg_pvr_str2mi_mid2

    REG32       CFG_PVR_0B_0C;                                                  // reg_pvr_str2mi_tail2

    REG16       CFG_PVR_0D;
        #define CFG_PVR_0D_REG_STR2MI_WP_LD                                     0x0001
        #define CFG_PVR_0D_REG_STR2MI_90K_27M_SEL                               0x0002
        #define CFG_PVR_0D_REG_PVR_TIMESTAMP_SRC_SEL                            0x0004
        #define CFG_PVR_0D_REG_SPS_PVR_FIFO_SRC_SEL                             0x0008
        #define CFG_PVR_0D_REG_BYTE_ENABLE_TIE_1_SPS                            0x0010
        #define CFG_PVR_0D_REG_CLR_PVR_OVERFLOW                                 0x0020
        #define CFG_PVR_0D_REG_FORCE_SYNC_EN                                    0x0040
        #define CFG_PVR_0D_REG_RECORD_AT_SYNC_DIS                               0x0080
        #define CFG_PVR_0D_REG_MIU_HIGHPRI                                      0x0100
        #define CFG_PVR_0D_REG_PVR_WRITE_POINTER_TO_NEXT_ADDRESS_EN             0x0200
        #define CFG_PVR_0D_REG_FLUSH_PVR_DATA                                   0x0400
        #define CFG_PVR_0D_REG_FLUSH_DATA_PVR_STATUS                            0x0800
        #define CFG_PVR_0D_REG_PVR_DMAW_PROTECT_EN                              0x1000
        #define CFG_PVR_0D_REG_ONEWAY_REC_CA_UPPER_PATH                         0x2000
        #define CFG_PVR_0D_REG_MIU_HIGHPRI_THOLD_MASK                           0xC000
        #define CFG_PVR_0D_REG_MIU_HIGHPRI_THOLD_SHIFT                          14

    REG32       CFG_PVR_0E_0F;                                                  // reg_pvr_pkt_meet_size

    REG16       CFG_PVR_10;
        #define CFG_PVR_10_REG_PVR_STR2MI_CNT_CLEAR                             0x0001
        #define CFG_PVR_10_REG_PVR_CNT_INT_MODE                                 0x0002
        #define CFG_PVR_10_REG_PVR_SYNC_INT_MODE                                0x0004
        #define CFG_PVR_10_REG_REC_CA_UPPER_PATH                                0x0008
        #define CFG_PVR_10_REG_INPUT_SRC_MASK                                   0x00F0
        #define CFG_PVR_10_REG_INPUT_SRC_SHIFT                                  4
        #define CFG_PVR_10_REG_PVR_MEET_SIZE_CNT_R_MASK                         0xFF00
        #define CFG_PVR_10_REG_PVR_MEET_SIZE_CNT_R_SHIFT                        8

    REG32       CFG_PVR_11_12;                                                  // reserved

    REG32       CFG_PVR_13_14;                                                  // reg_pvr_str2mi_wadr_r

    REG32       CFG_PVR_15_16;                                                  // reg_pvr_lpcr1_buf

    REG32       CFG_PVR_17_18;                                                  // reg_pvr_lpcr1

    REG16       CFG_PVR_19;                                                     // reg_pvr_fifo_status

    REG16       CFG_PVR_1A;
        #define CFG_PVR_1A_REG_PVR_EVER_OVERFLOW                                0x0001
        #define CFG_PVR_1A_REG_CHANGE_PRIVILEGE_SYNC_BYTE_ENABLE_PVR            0x0002
        #define CFG_PVR_1A_REG_CLR_PVR_NO_HIT_DMAW                              0x0004
        #define CFG_PVR_1A_REG_SYNC_BYTE_PRIVILEGE_PVR_MASK                     0xFF00
        #define CFG_PVR_1A_REG_SYNC_BYTE_PRIVILEGE_PVR_SHIFT                    8

    REG16       CFG_PVR_1B;
        #define CFG_PVR_1B_REG_PKT_192_SPS_EN                                   0x0001
        #define CFG_PVR_1B_REG_LOAD_SPS_KEY                                     0x0002
        #define CFG_PVR_1B_REG_AES_DOUT_BT_ORDER                                0x0004
        #define CFG_PVR_1B_REG_AES_DIN_BT_ORDER                                 0x0008
        #define CFG_PVR_1B_REG_ONEWAY_PKT192_SPS_EN                             0x0010

    REG32       CFG_PVR_1C_1D;                                                  // reg_pvr_dmaw_lbnd

    REG32       CFG_PVR_1E_1F;                                                  // reg_pvr_dmaw_ubnd

    REG16       CFG_PVR_20;
        #define CFG_PVR_20_REG_START_READ_BYPASS_EN                             0x0001
        #define CFG_PVR_20_REG_CLR_PIDFLT_BYTE_CNT                              0x0002
        #define CFG_PVR_20_REG_PVR_ERR_RM_EN                                    0x0004
        #define CFG_PVR_20_REG_MASK_SCR_PVR_EN                                  0x0008
        #define CFG_PVR_20_REG_DIS_NULL_PKT                                     0x0010
        #define CFG_PVR_20_REG_TEI_SKIP_PKT                                     0x0020
        #define CFG_PVR_20_REG_RECORD_TS                                        0x0040
        #define CFG_PVR_20_REG_RECORD_ALL                                       0x0080
        #define CFG_PVR_20_REG_SKIP_PVR_RUSH_DATA                               0x0100
        #define CFG_PVR_20_REG_ALT_TS_SIZE                                      0x0200
        #define CFG_PVR_20_REG_PVR_BLOCK_DISABLE                                0x0400
        #define CFG_PVR_20_REG_PVR_PES_DIRECTV_130_MODE                         0x0800
        #define CFG_PVR_20_REG_PVR_PES_DIRECTV_134_MODE                         0x1000
        #define CFG_PVR_20_REG_SUPPORT_DIRECTV_ALIGN_MODE                       0x2000  //K7U not support
        #define CFG_PVR_20_REG_RESET_FILTER                                     0x4000
        #define CFG_PVR_20_REG_ONEWAY_PVR                                       0x8000

    REG16       CFG_PVR_21;
        #define CFG_PVR_21_REG_PKT_SIZE_MASK                                    0x00FF
        #define CFG_PVR_21_REG_PKT_SIZE_SHIFT                                   0

    REG32       CFG_PVR_22_23;                                                  // reg_pause_time

    REG32       CFG_PVR_24_25;                                                  // reg_pvr_gap

    REG16       CFG_PVR_26;
        #define CFG_PVR_26_REG_RESET_FIRST_SYNC_PVR                             0x0001
        #define CFG_PVR_26_REG_MINUS_BASE_TIME_PVR                              0x0002
        #define CFG_PVR_26_REG_BYPASS_TIMESTAMP_SEL                             0x0004
        #define CFG_PVR_26_REG_PVR_GAP_LD                                       0x0008
        #define CFG_PVR_26_REG_RESET_TIMESTAMP_SEL                              0x0010

    REG16       CFG_PVR_27;                                                     // reg_config_by_tee_pvr
        #define CFG_PVR_27_REG_CHK_PRIVILEGE_FLAG                               0x0001
        #define CFG_PVR_27_REG_CHK_TEE_FILEIN                                   0x0002

    REG16       CFG_PVR_28;
        #define CFG_PVR_28_REG_PVR_MCM_DISABLE                                  0x0001
        #define CFG_PVR_28_REG_RESET_TOP                                        0x0002

    REG16       CFG_PVR_29;
        #define CFG_PVR_29_REG_PVR_INT_EN_MASK                                  0x00FF
        #define CFG_PVR_29_REG_PVR_INT_EN_PVR_MEET_TAIL                         0x0001
        #define CFG_PVR_29_REG_PVR_INT_EN_PVR_MEET_MID                          0x0002
        #define CFG_PVR_29_REG_PVR_INT_STATUS_MASK                              0xFF00
        #define CFG_PVR_29_INT_STATUS_PVR_MEET_TAIL                             0x0100
        #define CFG_PVR_29_INT_STATUS_PVR_MEET_MID                              0x0200

    REG16       CFG_PVR_2A_3E[0x3F - 0x2A];                                     // reserved

    REG16       CFG_PVR_3F;
        #define CFG_PVR_3F_REG_CLK_GATING_TSP_PVR                               0x0001
        #define CFG_PVR_3F_REG_CLK_GATING_MIU_PVR                               0x0002

} REG_PVR_ENG_Ctrl;

#endif // _REG_PVR_H_
