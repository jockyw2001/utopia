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
//  File name: regFILE.h
//  Description: TSP File-in Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_FILE_H_
#define _REG_FILE_H_

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

typedef struct _REG_FILE_ENG_Ctrl // FILE (Bank:0x1611 , 0x1627)
{
    REG16       CFG_FILE_00;
        #define CFG_FILE_00_REG_TSP_FILE_IN                                     0x0001
        #define CFG_FILE_00_REG_MEM_TS_DATA_ENDIAN                              0x0002
        #define CFG_FILE_00_REG_TSP_FILE_SEGMENT                                0x0004
        #define CFG_FILE_00_REG_FILEIN_RADDR_READ                               0x0008
        #define CFG_FILE_00_REG_MEM_TS_W_ORDER                                  0x0010
        #define CFG_FILE_00_REG_DIS_MIU_RQ                                      0x0020
        #define CFG_FILE_00_REG_RST_TS_FIN                                      0x0040
        #define CFG_FILE_00_REG_RST_FILEIN_TSIF                                 0x0080
        #define CFG_FILE_00_REG_RST_CMDQ_FILEIN                                 0x0100
        #define CFG_FILE_00_REG_WB_RST_FILEIN                                   0x0200
        #define CFG_FILE_00_REG_RST_WB_DMA_FILEIN                               0x0400
        #define CFG_FILE_00_REG_FILE2MI_PRI                                     0x0800
        #define CFG_FILE_00_REG_RST_READ_DMA                                    0x1000
        #define CFG_FILE_00_REG_LPCR2_LOAD                                      0x2000
        #define CFG_FILE_00_REG_WB_FSM_RESET                                    0x4000

    REG16       CFG_FILE_01;
        #define CFG_FILE_01_REG_TIMER_EN                                        0x0002
        #define CFG_FILE_01_REG_PKT192_EN                                       0x0004
        #define CFG_FILE_01_REG_PKT192_BLK_DISABLE                              0x0008
        #define CFG_FILE_01_REG_LPCR2_WLD                                       0x0010
        #define CFG_FILE_01_REG_TS_DATA_PORT_SEL                                0x0020
        #define CFG_FILE_01_REG_LPCR_FREG_27M_90K                               0x0040
        #define CFG_FILE_01_REG_TSP_FILEIN_ABORT                                0x0080
        #define CFG_FILE_01_REG_DISABLE_FILEIN_ADDR_LEN_BY_TEE                  0x0100
        #define CFG_FILE_01_REG_PS_MODE_BLOCK                                   0x0200
        #define CFG_FILE_01_REG_TSIF_PAUSE                                      0x0400
        #define CFG_FILE_01_REG_DISABLE_STREAM_ID_CHK_FOR_NAGRA_DONGLE          0x0800
        #define CFG_FILE_01_REG_FILTER_STREAM_ID_0_TO_1F_FOR_NAGRA_DONGLE       0x1000
        #define CFG_FILE_01_REG_INCR_PERFORMANCE_ENABLE                         0x2000

    REG32       CFG_FILE_02_03;                                                 // reg_tsp_filein_raddr_tsif

    REG32       CFG_FILE_04_05;                                                 // reg_tsp_filein_rnum_tsif

    REG16       CFG_FILE_06;                                                    // reg_tsp_filein_ctrl_tsif
        #define CFG_FILE_06_REG_FILEIN_RSTART                                   0x0001
        #define CFG_FILE_06_REG_FILEIN_DONE                                     0x0002
        #define CFG_FILE_06_REG_FILEIN_INIT_TRUST                               0x0004
        #define CFG_FILE_06_REG_FILEIN_ABORT                                    0x0010

    REG16       CFG_FILE_07;
        #define CFG_FILE_07_CMD_WR_CNT_TSIF1_MASK                               0x001F
        #define CFG_FILE_07_CMD_WR_CNT_TSIF1_SHIFT                              0
        #define CFG_FILE_07_CMD_FIFO_FULL_TSIF1                                 0x0040
        #define CFG_FILE_07_CMD_FIFO_EMPTY_TSIF1                                0x0080
        #define CFG_FILE_07_CMD_WR_LEVEL_TSIF1_MASK                             0x0300
        #define CFG_FILE_07_CMD_WR_LEVEL_TSIF1_SHIFT                            8

    REG16       CFG_FILE_08;
        #define CFG_FILE_08_REG_CHK_PKT_SIZE_MASK                               0x00FF
        #define CFG_FILE_08_REG_CHK_PKT_SIZE_SHIFT                              0
        #define CFG_FILE_08_REG_MATCH_CNT_THRESHOLD_MASK                        0x0F00
        #define CFG_FILE_08_REG_MATCH_CNT_THRESHOLD_SHIFT                       8
        #define CFG_FILE_08_REG_MATCH_CNT_INIT_VALUE_MASK                       0xF000
        #define CFG_FILE_08_REG_MATCH_CNT_INIT_VALUE_SHIFT                      12

    REG16       CFG_FILE_09;                                                    // reg_tsp_file_timer

    REG16       CFG_FILE_0A;
        #define CFG_FILE_0A_REG_FIX_192_TIMER_0_EN                              0x0001
        #define CFG_FILE_0A_REG_INIT_TIMESTAMP                                  0x0002
        #define CFG_FILE_0A_REG_FIXED_TIMESTAMP_RING_BACK_EN                    0x0004
        #define CFG_FILE_0A_REG_FIX_LPCR_RING_BACK_EN                           0x0008
        #define CFG_FILE_0A_REG_MATCH_CNT_FILEIN_EN                             0x0010
        #define CFG_FILE_0A_REG_INIT_STAMP_RSTART_EN                            0x0020
        #define CFG_FILE_0A_REG_CHK_PRIVILEGE_SYNC_BYTE_EN                      0x0040
        #define CFG_FILE_0A_REG_INIT_TRUST_SYNC_CNT_VALUE_MASK                  0xFF00
        #define CFG_FILE_0A_REG_INIT_TRUST_SYNC_CNT_VALUE_SHIFT                 8

    REG32       CFG_FILE_0B_0C;                                                 // reg_init_timestamp_vld

    REG16       CFG_FILE_0D;
        #define CFG_FILE_0D_REG_SYNC_BYTE_PRIVILEGE_MASK                        0x00FF
        #define CFG_FILE_0D_REG_SYNC_BYTE_PRIVILEGE_SHIFT                       0
        #define CFG_FILE_0D_REG_REPLACE_PRIVILEGE_SYNC_BYTE_MASK                0xFF00
        #define CFG_FILE_0D_REG_REPLACE_PRIVILEGE_SYNC_BYTE_SHIFT               8

    REG16       CFG_FILE_0E;
        #define CFG_FILE_0E_REG_RVU_PSI_EN                                      0x0001
        #define CFG_FILE_0E_REG_RVU_TEI_EN                                      0x0002
        #define CFG_FILE_0E_REG_RVU_ERR_CLR                                     0x0004
        #define CFG_FILE_0E_REG_RVU_EN                                          0x0008
        #define CFG_FILE_0E_REG_RVU_TIMESTAMP_EN                                0x0010
        #define CFG_FILE_0E_REG_RVU_ERR_EVER                                    0x0020
        #define CFG_FILE_0E_REG_HD_0000_TO_SECTION                              0x0040
        #define CFG_FILE_0E_REG_HD_1100_TO_SECTION                              0x0080
        #define CFG_FILE_0E_REG_HD_10X0_11X0_TO_SECTION                         0x0100
        #define CFG_FILE_0E_REG_PAYLOAD_128_MODE                                0x0200
        #define CFG_FILE_0E_REG_SUPPORT_NB                                      0x0400
        #define CFG_FILE_0E_REG_FIND_RVU_SYNC_PID                               0x0800

    REG16       CFG_FILE_0F;
        #define CFG_FILE_0F_REG_TSIF_SPD_RST                                    0x0001
        #define CFG_FILE_0F_REG_SPD_TSIF_BYPASS                                 0x0002
        #define CFG_FILE_0F_REG_LOAD_SPD_KEY                                    0x0004

    REG32       CFG_FILE_10_11;                                                 // reg_lpcr_buf

    REG32       CFG_FILE_12_13;                                                 // reg_lpcr_tsif

    REG32       CFG_FILE_14_15;                                                 // reg_timestamp

    REG32       CFG_FILE_16_17;                                                 // reg_tsp2mi_raddr_tsif
        #define CFG_FILE_16_17_REG_TSP2MI_RADDR_TSIF_MASK                       0x0FFFFFFF

    REG16       CFG_FILE_18;
        #define CFG_FILE_18_REG_NAGRA_DONGLE_SYNC_BYTE_F0_MASK                  0x00FF
        #define CFG_FILE_18_REG_NAGRA_DONGLE_SYNC_BYTE_F0_SHIFT                 0
        #define CFG_FILE_18_REG_NAGRA_DONGLE_SYNC_BYTE_F1_MASK                  0xFF00
        #define CFG_FILE_18_REG_NAGRA_DONGLE_SYNC_BYTE_F1_SHIFT                 8

    REG16       CFG_FILE_19_1F[0x20 - 0x19];                                    // reserved

} REG_FILE_ENG_Ctrl;

#endif // _REG_FILE_H_
