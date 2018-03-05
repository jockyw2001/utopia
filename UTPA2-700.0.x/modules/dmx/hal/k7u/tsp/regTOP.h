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
//  File name: regTOP.h
//  Description: TSP Miu/Clk Gating Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_TOP_H_
#define _REG_TOP_H_

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
#define TSP_MIU_SEL_BITS_LEN        2

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct _REG_TOP_ProtectCtrl
{
    REG32       L_BND;
    REG32       U_BND;
        #define REG_TOP_ProtectCtrl_BND_MASK                                    0x0FFFFFFF
        #define REG_TOP_ProtectCtrl_ChkEn                                       0x80000000
} REG_TOP_ProtectCtrl;

typedef struct _REG_TOP_Ctrl // TOP (Bank:0x1703)
{
    REG16       CFG_TOP_00;
        #define CFG_TOP_00_REG_MIU_MERGE_ABT_EN                                 0x0001

    REG16       CFG_TOP_01;
        #define CFG_TOP_01_REG_MIU_RR_PRI_ABT_EN                                0x0001

    REG16       CFG_TOP_02;
        #define CFG_TOP_02_REG_DIS_MIU_RQ_ABT                                   0x0001

    REG16       CFG_TOP_03;
        #define CFG_TOP_03_REG_RLAST_MASK_ABT_EN                                0x0001

    REG16       CFG_TOP_04;
        #define CFG_TOP_04_REG_SEL_ABT_STATUS_NUM_MASK                          0x000F
        #define CFG_TOP_04_REG_SEL_ABT_STATUS_NUM_SHIFT                         0
        #define CFG_TOP_04_REG_ABT_STATUS_MASK                                  0x03F0
        #define CFG_TOP_04_REG_ABT_STATUS_SHIFT                                 4

    REG32       CFG_TOP_05_06;                                                  // PVR 1~10
        #define CFG_TOP_05_06_REG_MIU_SEL_PVR_MASK                              0x00000003
        #define CFG_TOP_05_06_REG_MIU_SEL_PVR_SHIFT                             0

    REG16       CFG_TOP_07;
        #define CFG_TOP_07_REG_MIU_SEL_FIQ_MASK                                 0x0003
        #define CFG_TOP_07_REG_MIU_SEL_FIQ_SHIFT                                0

    REG16       CFG_TOP_08;
        #define CFG_TOP_08_REG_MIU_SEL_FILEIN_MASK                              0x0003
        #define CFG_TOP_08_REG_MIU_SEL_FILEIN_SHIFT                             0

    REG16       CFG_TOP_09;
        #define CFG_TOP_09_REG_MIU_SEL_VQ_MASK                                  0x0003
        #define CFG_TOP_09_REG_MIU_SEL_VQ_SHIFT                                 0
        #define CFG_TOP_09_REG_MIU_SEL_SEC_MASK                                 0x000C
        #define CFG_TOP_09_REG_MIU_SEL_SEC_SHIFT                                2
        #define CFG_TOP_09_REG_MIU_SEL_ORZ_MASK                                 0x0030
        #define CFG_TOP_09_REG_MIU_SEL_ORZ_SHIFT                                4
        #define CFG_TOP_09_REG_MIU_SEL_MMFI_MASK                                0x00C0 //MMFI 0 ~ 1
        #define CFG_TOP_09_REG_MIU_SEL_MMFI_SHIFT                               6
        #define CFG_TOP_09_REG_MIU_SEL_FIQ_MUX_MASK                             0x0C00 //FIQ_MUX 0 ~ 2
        #define CFG_TOP_09_REG_MIU_SEL_FIQ_MUX_SHIFT                            10

    REG16       CFG_TOP_0A;
        #define CFG_TOP_0A_REG_FORCE_PRI_ABT_EN                                 0x0001

    REG16       CFG_TOP_0B;
        #define CFG_TOP_0B_REG_RRB_PLUS_PRI_EN_ABT                              0x0001

    REG16       CFG_TOP_0C;
        #define CFG_TOP_0C_REG_MIU_LAT_CYCLE_CNT_EN                             0x0001
        #define CFG_TOP_0C_REG_MIU_LAT_CYCLE_CNT_CLR                            0x0002
        #define CFG_TOP_0C_REG_MIU_LAT_LEVEL_EVER_EN                            0x0004
        #define CFG_TOP_0C_REG_MIU_LAT_LEVEL_EVER_CLR                           0x0008
        #define CFG_TOP_0C_REG_MIU_LAT_LEVEL_EVER_ABT_SEL_MASK                  0x00F0
        #define CFG_TOP_0C_REG_MIU_LAT_LEVEL_EVER_ABT_SEL_SHIFT                 4

    REG32       CFG_TOP_0D_0E;                                                  // reg_miu_lat_level_ever
        #define CFG_TOP_REG_MIU_LAT_THOLD_A_ABT_MASK                            0x0000000F
        #define CFG_TOP_REG_MIU_LAT_THOLD_A_ABT_SHIFT                           0
        #define CFG_TOP_REG_MIU_LAT_THOLD_B_ABT_MASK                            0x000000F0
        #define CFG_TOP_REG_MIU_LAT_THOLD_B_ABT_SHIFT                           4
        #define CFG_TOP_REG_MIU_LAT_THOLD_C_ABT_MASK                            0x00000F00
        #define CFG_TOP_REG_MIU_LAT_THOLD_C_ABT_SHIFT                           8
        #define CFG_TOP_REG_MIU_LAT_THOLD_D_ABT_MASK                            0x0000F000
        #define CFG_TOP_REG_MIU_LAT_THOLD_D_ABT_SHIFT                           12
        #define CFG_TOP_REG_MIU_LAT_THOLD_E_ABT_MASK                            0x000F0000
        #define CFG_TOP_REG_MIU_LAT_THOLD_E_ABT_SHIFT                           16
        #define CFG_TOP_REG_MIU_LAT_THOLD_F_ABT_MASK                            0x00F00000
        #define CFG_TOP_REG_MIU_LAT_THOLD_F_ABT_SHIFT                           20
        #define CFG_TOP_REG_MIU_LAT_THOLD_G_ABT_MASK                            0x0F000000
        #define CFG_TOP_REG_MIU_LAT_THOLD_G_ABT_SHIFT                           24
        #define CFG_TOP_REG_MIU_LAT_THOLD_H_ABT_MASK                            0xF0000000
        #define CFG_TOP_REG_MIU_LAT_THOLD_H_ABT_SHIFT                           28

    REG16       CFG_TOP_0F;                                                     // reg_miu_lat_cycle_per_tick

    REG16       CFG_TOP_10_23[0x24 - 0x10];                                     // reg_miu_lat_thold_abt 0 ~ 9

    REG16       CFG_TOP_24;                                                     // reg_miu_fixed_last_wd_en_done_z
        #define CFG_TOP_24_REG_MIU_FIXED_LAST_WD_EN_DONE_Z_ABT                  0x0001
        #define CFG_TOP_24_REG_MIU_FIXED_LAST_WD_EN_DONE_Z_ABT_ALL              0x03FF

    REG16       CFG_TOP_25;                                                     // reg_check_mi2rdy
        #define CFG_TOP_25_REG_CHECK_MI2RDY_ABT                                 0x0001
        #define CFG_TOP_25_REG_CHECK_MI2RDY_ABT_ALL                             0x03FF

    REG16       CFG_TOP_26_2C[0x2D - 0x26];                                     // reserved

    REG16       CFG_TOP_2D;
        #define CFG_TOP_2D_REG_PVR_RASP_REC_EVENT_ENABLE                        0x0001  // RASP 1 ~ 10

    REG16       CFG_TOP_2E;
        #define CFG_TOP_2E_REG_PVR_LUT_REC_EVENT_ENABLE                         0x0001  // LUT 1 ~ 10

    REG16       CFG_TOP_2F;
        #define CFG_TOP_2F_REG_PVR_EVENT_ENABLE                                 0x0001  // PVR 1 ~ 10

    REG_TOP_ProtectCtrl     CFG_TOP_30_4B[7];                                   // FILEIN 0 ~ 6 bnd

    REG16       CFG_TOP_4C_4F[0x50 - 0x4C];                                     // reserved

    REG_TOP_ProtectCtrl     CFG_TOP_50_57[2];                                   // MMFI 0 ~ 1 bnd

    REG16       CFG_TOP_58_5F[0x60 - 0x58];                                     // reserved

    REG16       CFG_TOP_60_6C[0x6D - 0x60];                                     // reg_bist_fail_status

    REG32       CFG_TOP_6D_6E;                                                  // reserved

    REG16       CFG_TOP_6F;                                                     // reg_sync_reg_ctrl
        #define CFG_TOP_6F_REG_EN_ALWAYS_ON                                     0x0001
        #define CFG_TOP_6F_REG_EN_TIMER_POSTPONE_MASK                           0x00F0
        #define CFG_TOP_6F_REG_EN_TIMER_POSTPONE_SHIFT                          4

    REG16       CFG_TOP_70;                                                     // reg_clk_gating
        #define CFG_TOP_70_REG_CLK_GATING_TSP_ENG                               0x0001
        #define CFG_TOP_70_REG_CLK_GATING_MMFI                                  0x0002  // MMFI 0 ~ 1
        #define CFG_TOP_70_REG_MIU_CLK_GATING_TSP_ENG                           0x0100
        #define CFG_TOP_70_REG_MIU_CLK_GATING_MMFI                              0x0200  // MMFI 0 ~ 1

    REG16       CFG_TOP_71_77[0x78 - 0x71];                                     // reserved

    REG16       CFG_TOP_78;                                                     // reg_reset_ctrl0

    REG16       CFG_TOP_79;                                                     // reg_reset_ctrl1

    REG16       CFG_TOP_7A;                                                     // reg_reset_ctrl2

    REG16       CFG_TOP_7B;                                                     // reg_reset_ctrl3

    REG16       CFG_TOP_7C;                                                     // reg_reset_ctrl4

    REG16       CFG_TOP_7D;                                                     // reg_reset_ctrl5
        #define CFG_TOP_7D_REG_RESET_OTV                                        0x0100
        #define CFG_TOP_7D_REG_RESET_DEBUG_TABLE                                0x0200
        #define CFG_TOP_7D_REG_RESET_DMA_ENG                                    0x0400
        #define CFG_TOP_7D_REG_RESET_SEC_CMP                                    0x0800
        #define CFG_TOP_7D_REG_RESET_SECFLT_REG                                 0x1000
        #define CFG_TOP_7D_REG_RESET_SEC                                        0x2000
        #define CFG_TOP_7D_REG_RESET_PID_TABLE                                  0x4000

    REG16       CFG_TOP_7E;                                                     // reg_reset_ctrl6

} REG_TOP_Ctrl;

#endif // _REG_TOP_H_
