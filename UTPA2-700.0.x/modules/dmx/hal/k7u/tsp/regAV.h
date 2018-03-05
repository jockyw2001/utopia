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
//  File name: regAV.h
//  Description: TSP Audio/Video Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_AV_H_
#define _REG_AV_H_

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

typedef struct _REG_AV_ENG_Ctrl // Audio (Bank:0x3002) , Video (Bank:0x3003)
{
    REG16       CFG_AV_00;
        #define CFG_AV_00_REG_START_READ_BYPASS_ENABLE                          0x0001
        #define CFG_AV_00_REG_DIS_NULL_PKT                                      0x0002
        #define CFG_AV_00_REG_TRACE_MARK_EN                                     0x0004
        #define CFG_AV_00_REG_TEI_SKIP_PKT                                      0x0008
        #define CFG_AV_00_REG_PUSI_THREE_BYTE_MODE                              0x0010
        #define CFG_AV_00_REG_CLR_PIDFLT_BYTE_CNT                               0x0020
        #define CFG_AV_00_REG_PID_BYPASS                                        0x0040
        #define CFG_AV_00_REG_SKIP_PES_RUSH_DATA                                0x0080
        #define CFG_AV_00_REG_PES_ERR_RM_EN                                     0x0100
        #define CFG_AV_00_REG_DUP_PKT_SKIP                                      0x0200
        #define CFG_AV_00_REG_ALT_TS_SIZE                                       0x0400
        #define CFG_AV_00_REG_REG_BD_AUD_EN                                     0x0800  //Audio-Only
        #define CFG_AV_00_REG_REG_MAIN_CHANNEL                                  0x1000  //Audio-Only
        #define CFG_AV_00_REG_MASK_SCR_EN                                       0x2000
        #define CFG_AV_00_REG_BLOCK_DISABLE                                     0x4000
        #define CFG_AV_00_REG_PS_MODE                                           0x8000

    REG16       CFG_AV_01;
        #define CFG_AV_01_REG_PKT_SIZE_MASK                                     0x00FF
        #define CFG_AV_01_REG_PKT_SIZE_SHIFT                                    0
        #define CFG_AV_01_REG_DROP_ERR_START_CODE                               0x0100
        #define CFG_AV_01_REG_DROP_TEI_ERR_START_CODE                           0x0200
        #define CFG_AV_01_REG_RESET_FIFO_PARSER                                 0x0400
        #define CFG_AV_01_REG_RESET_FILTER                                      0x0800
        #define CFG_AV_01_REG_PS_MODE_SRC_MASK                                  0xF000
        #define CFG_AV_01_REG_PS_MODE_SRC_SHIFT                                 12

    REG16       CFG_AV_02;
        #define CFG_AV_02_REG_DROP_PKT_CNT_LOAD                                 0x0001
        #define CFG_AV_02_REG_DROP_PKT_CNT_CLR                                  0x0002
        #define CFG_AV_02_REG_DIS_CNTR_LOAD                                     0x0004
        #define CFG_AV_02_REG_DIS_CNTR_CLR                                      0x0008
        #define CFG_AV_02_REG_PKT_CNT_LOAD                                      0x0010
        #define CFG_AV_02_REG_PKT_CNT_CLR                                       0x0020
        #define CFG_AV_02_REG_RST_CC_MODE                                       0x0040
        #define CFG_AV_02_REG_DIS_CNTR_INC_BY_PL                                0x0080
        #define CFG_AV_02_REG_INPUT_SRC_MASK                                    0x0F00
        #define CFG_AV_02_REG_INPUT_SRC_SHIFT                                   8
        #define CFG_AV_02_REG_RESET_TOP                                         0x0100

    REG16       CFG_AV_03;                                                      // reg_drop_pkt_cntr

    REG16       CFG_AV_04;                                                      // reg_dis_cont_cntr

    REG16       CFG_AV_05;                                                      // reg_pes_pkt_cntr

    REG16       CFG_AV_06;
        #define CFG_AV_06_REG_PES_RSEL                                          0x0001
        #define CFG_AV_06_REG_FIFO_RD                                           0x0002
        #define CFG_AV_06_REG_STATUS_CLR                                        0x0004
        #define CFG_AV_06_REG_LEAF_DROP_DATA_ENABLE                             0x0008
        #define CFG_AV_06_REG_FIFO_FULL_MASK                                    0x1000
        #define CFG_AV_06_REG_FIFO_FULL_SHIFT                                   12
        #define CFG_AV_06_REG_FIFO_EMPTY_MASK                                   0x2000
        #define CFG_AV_06_REG_FIFO_EMPTY_SHIFT                                  13
        #define CFG_AV_06_REG_FIFO_WR_LEVEL_MASK                                0xC000
        #define CFG_AV_06_REG_FIFO_WR_LEVEL_SHIFT                               14

    REG16       CFG_AV_07;
        #define CFG_AV_07_REG_DEBUG_FIFO_DATA_MASK                              0x01FF
        #define CFG_AV_07_REG_DEBUG_FIFO_DATA_SHIFT                             0
        #define CFG_AV_07_REG_PES_WR_OVERFLOW                                   0x0200
        #define CFG_AV_07_REG_FIFO_WR_LEVEL_EVER_MASK                           0x0C00
        #define CFG_AV_07_REG_FIFO_WR_LEVEL_EVER_SHIFT                          10
        #define CFG_AV_07_REG_FIFO_ERR_EVER                                     0x1000

    REG16       CFG_AV_08;
        #define CFG_AV_08_REG_PID_MATCH_MASK                                    0x1FFF
        #define CFG_AV_08_REG_PID_MATCH_SHIFT                                   0
        #define CFG_AV_08_REG_PID_CHANGE                                        0x2000

    REG16       CFG_AV_09;                                                      // Video-Only
        #define CFG_AV_09_REG_VPES_DISCONTI                                     0x0001
        #define CFG_AV_09_REG_VPES_RD_CNT_MASK                                  0x00F0
        #define CFG_AV_09_REG_VPES_RD_CNT_SHIFT                                 4

    REG16       CFG_AV_0A_0E[0x0F - 0x0A];                                      // reserved

    REG16       CFG_AV_0F;
        #define CFG_AV_0F_REG_CLK_GATING_TSP                                    0x0001
        #define CFG_AV_0F_REG_CLK_GATING_PARSER                                 0x0002

} REG_AV_ENG_Ctrl;

#endif // _REG_AV_H_
