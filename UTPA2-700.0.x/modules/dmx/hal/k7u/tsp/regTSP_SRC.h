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
//  File name: regTSP_SRC.h
//  Description: TSP Source Mux Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_TSP_SRC_H_
#define _REG_TSP_SRC_H_

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

typedef struct _REG_TSP_SRC_Ctrl // TSP-SRC (Bank:0x3014)
{
    REG16       CFG_TSP_SRC_00_03[0x4 - 0x0];                                   // reg_ts_mux
        #define CFG_TSP_SRC_MUX_EVEN_MASK                                       0x00FF
        #define CFG_TSP_SRC_MUX_EVEN_SHIFT                                      0
        #define CFG_TSP_SRC_MUX_ODD_MASK                                        0xFF00
        #define CFG_TSP_SRC_MUX_ODD_SHIFT                                       8

            #define CFG_TSP_SRC_MUX_PAD_TS0                                     0x0000
            #define CFG_TSP_SRC_MUX_PAD_TS1                                     0x0001
            #define CFG_TSP_SRC_MUX_PAD_TS2                                     0x0002
            #define CFG_TSP_SRC_MUX_PAD_TS3                                     0x0003
            #define CFG_TSP_SRC_MUX_PAD_TS4                                     0x0004
            #define CFG_TSP_SRC_MUX_PAD_TS5                                     0x0005
            #define CFG_TSP_SRC_MUX_PAD_TS6                                     0x0006
            #define CFG_TSP_SRC_MUX_PAD_TS7                                     0x0007
            #define CFG_TSP_SRC_MUX_PAD_TS8                                     0x0008
            #define CFG_TSP_SRC_MUX_PAD_TSO0                                    0x0009
            #define CFG_TSP_SRC_MUX_PAD_TSO1                                    0x000A
            #define CFG_TSP_SRC_MUX_PAD_TSIO                                    0x000B
            #define CFG_TSP_SRC_MUX_PAD_CILINK                                  0x000C
            #define CFG_TSP_SRC_MUX_PAD_DMD0                                    0x000E // Not support
            #define CFG_TSP_SRC_MUX_PAD_DMD1                                    0x000F // Not support

    REG16       CFG_TSP_SRC_04_06[0x7 - 0x4];                                   // reg_tso_mux (TSO #0)

    REG16       CFG_TSP_SRC_07;                                                 // reserved

    REG16       CFG_TSP_SRC_08_0A[0xB - 0x8];                                   // reg_tso_mux (TSO #1)

    REG16       CFG_TSP_SRC_0B;                                                 // reserved

    REG16       CFG_TSP_SRC_0C;                                                 // reg_s2p_mux

    REG16       CFG_TSP_SRC_0D_0F[0x10 - 0xD];                                  // reserved

    REG16       CFG_TSP_SRC_10;                                                 // reg_rasp_mux

    REG16       CFG_TSP_SRC_11;                                                 // reg_emmflt_mux

    REG16       CFG_TSP_SRC_12;                                                 // reg_cilink_mux

    REG16       CFG_TSP_SRC_13_1F[0x20 - 0x13];                                 // reserved

    REG16       CFG_TSP_SRC_20;
        #define CFG_TSP_SRC_20_REG_TS0_MUX_LOCK                                 0x0001
        #define CFG_TSP_SRC_20_REG_TS1_MUX_LOCK                                 0x0002
        #define CFG_TSP_SRC_20_REG_TS2_MUX_LOCK                                 0x0004
        #define CFG_TSP_SRC_20_REG_TS3_MUX_LOCK                                 0x0008
        #define CFG_TSP_SRC_20_REG_TS4_MUX_LOCK                                 0x0010
        #define CFG_TSP_SRC_20_REG_TS5_MUX_LOCK                                 0x0020
        #define CFG_TSP_SRC_20_REG_TS6_MUX_LOCK                                 0x0040
        #define CFG_TSP_SRC_20_REG_TSO0_MUX_LOCK                                0x0100
        #define CFG_TSP_SRC_20_REG_TSO1_MUX_LOCK                                0x0200
        #define CFG_TSP_SRC_20_REG_TSO2_MUX_LOCK                                0x0400
        #define CFG_TSP_SRC_20_REG_TSO3_MUX_LOCK                                0x0800
        #define CFG_TSP_SRC_20_REG_TSO4_MUX_LOCK                                0x1000
        #define CFG_TSP_SRC_20_REG_TSO5_MUX_LOCK                                0x2000

    REG16       CFG_TSP_SRC_21;
        #define CFG_TSP_SRC_21_REG_TSO10_MUX_LOCK                               0x0001
        #define CFG_TSP_SRC_21_REG_TSO11_MUX_LOCK                               0x0002
        #define CFG_TSP_SRC_21_REG_TSO12_MUX_LOCK                               0x0004
        #define CFG_TSP_SRC_21_REG_TSO13_MUX_LOCK                               0x0008
        #define CFG_TSP_SRC_21_REG_TSO14_MUX_LOCK                               0x0010
        #define CFG_TSP_SRC_21_REG_TSO15_MUX_LOCK                               0x0020
        #define CFG_TSP_SRC_21_REG_S2P0_MUX_LOCK                                0x0100
        #define CFG_TSP_SRC_21_REG_S2P1_MUX_LOCK                                0x0200
        #define CFG_TSP_SRC_21_REG_CILINK1_MUX_LOCK                             0x0400
        #define CFG_TSP_SRC_21_REG_CILINK2_MUX_LOCK                             0x0800

    REG16       CFG_TSP_SRC_22;
        #define CFG_TSP_SRC_22_REG_RASP0_MUX_LOCK                               0x0001
        #define CFG_TSP_SRC_22_REG_RASP1_MUX_LOCK                               0x0002
        #define CFG_TSP_SRC_22_REG_EMMFLT0_MUX_LOCK                             0x0004
        #define CFG_TSP_SRC_22_REG_EMMFLT1_MUX_LOCK                             0x0008

} REG_TSP_SRC_Ctrl;

#endif // _REG_TSP_SRC_H_
