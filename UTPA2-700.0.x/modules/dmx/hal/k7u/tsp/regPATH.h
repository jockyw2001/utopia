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
//  File name: regPATH.h
//  Description: TSP Live-in Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_PATH_H_
#define _REG_PATH_H_

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

typedef struct _REG_PATH_ENG_Ctrl // LIVE-IN (Bank:0x1610)
{
    REG16       CFG_PATH_00;
        #define CFG_PATH_00_REG_TS_IF_EN                                        0x0001
        #define CFG_PATH_00_REG_TS_DATA_SWAP                                    0x0002
        #define CFG_PATH_00_REG_P_SEL                                           0x0004
        #define CFG_PATH_00_REG_EXT_SYNC_SEL                                    0x0008
        #define CFG_PATH_00_REG_TS_SIN_C0                                       0x0010
        #define CFG_PATH_00_REG_TS_SIN_C1                                       0x0020
        #define CFG_PATH_00_REG_SERIAL_EXT_SYNC_1T                              0x0040
        #define CFG_PATH_00_REG_FORCE_SYNC_BYTE                                 0x0080
        #define CFG_PATH_00_REG_DATA_CHK_2T                                     0x0100
        #define CFG_PATH_00_REG_MERGED_MATCH_SYNC_BYTE_EN                       0x0400
        #define CFG_PATH_00_REG_TS_FIFO_BYPASS                                  0x0800
        #define CFG_PATH_00_REG_TS_FIFO_EVER_OVERFLOW_CLR                       0x1000
        #define CFG_PATH_00_REG_TS_FIFO_EVER_FULL_CLR                           0x2000
        #define CFG_PATH_00_REG_TS_FIFO_EVER_OVERFLOW                           0x4000
        #define CFG_PATH_00_REG_TS_FIFO_EVER_FULL                               0x8000

    REG16       CFG_PATH_01;
        #define CFG_PATH_01_REG_PKT_CHK_SIZE_MASK                               0x00FF
        #define CFG_PATH_01_REG_PKT_CHK_SIZE_SHIFT                              0

    REG16       CFG_PATH_02;
        #define CFG_PATH_02_REG_LOCKED_PKT_CNT_CLR                              0x0001
        #define CFG_PATH_02_REG_LOCKED_PKT_CNT_LOAD                             0x0002
        #define CFG_PATH_02_REG_CLR_OVERFLOW                                    0x0004
        #define CFG_PATH_02_REG_UNLOCKED_PKT_CNT_MODE                           0x0008
        #define CFG_PATH_02_REG_EVER_UNLOCK_STATUS                              0x0010
        #define CFG_PATH_02_REG_TSIF_EVER_OVERFLOW                              0x0020
        #define CFG_PATH_02_REG_TSIF_TSO0_BLOCK_EN                              0x0100
        #define CFG_PATH_02_REG_TSIF_TSO1_BLOCK_EN                              0x0200
        #define CFG_PATH_02_REG_TSIF_TSIO_BLOCK_EN                              0x0400

    REG16       CFG_PATH_03;                                                    // reg_locked_pkt_cnt

    REG16       CFG_PATH_04;                                                    // directv_130_188
        #define CFG_PATH_04_REG_DIRECTV_EN                                      0x0001
        #define CFG_PATH_04_REG_DIRECTV_PSI_EN                                  0x0002
        #define CFG_PATH_04_REG_DIRECTV_TEI_EN                                  0x0004
        #define CFG_PATH_04_REG_DIRECTV_ERR_CLR                                 0x0008
        #define CFG_PATH_04_REG_DIRECTV_ERR_STATUS                              0x0010

    REG16       CFG_PATH_05;                                                    // reg_pkt_converter_config0
        #define CFG_PATH_05_PKT_CONVERTER_MODE_MASK                             0x0007
        #define CFG_PATH_05_PKT_CONVERTER_MODE_SHIFT                            0
            #define CFG_PATH_05_PKT_CONVERTER_NORMAL_MODE                       0
            #define CFG_PATH_05_PKT_CONVERTER_CIPLUS_MODE                       1
            #define CFG_PATH_05_PKT_CONVERTER_OPENCABLE_MODE                    2
            #define CFG_PATH_05_PKT_CONVERTER_ATS_MODE                          3
            #define CFG_PATH_05_PKT_CONVERTER_MXL_MODE                          4
            #define CFG_PATH_05_PKT_CONVERTER_ND_MODE                           5
        #define CFG_PATH_05_PKT_CONVERTER_FORCE_SYNCBYTE                        0x0008
        #define CFG_PATH_05_BYPASS_PKT_CONVERTER                                0x0010
        #define CFG_PATH_05_BYPASS_SRC_ID_PARSER                                0x0020
        #define CFG_PATH_05_SRC_ID_FLT_EN                                       0x0040
        #define CFG_PATH_05_MXL_PKT_HEADER_MASK                                 0x0F80
        #define CFG_PATH_05_MXL_PKT_HEADER_SHIFT                                7
        #define CFG_PATH_05_SYNC_BYTE_POSITION_MASK                             0xF000
        #define CFG_PATH_05_SYNC_BYTE_POSITION_SHIFT                            12

    REG16       CFG_PATH_06;
        // reg_pkt_converter_config1
        #define CFG_PATH_06_REG_ECO_TS_SYNC_OUT_DELAY                           0x0001
        #define CFG_PATH_06_REG_ECO_TS_SYNC_OUT_REVERSE_BLOCK                   0x0002
        #define CFG_PATH_06_REG_PKT_CONVERTER_FIRST_SYNC_VALID_MASK_EN          0x0004
        #define CFG_PATH_06_REG_DISABLE_STREAM_ID_CHK_FOR_NAGRA_DONGLE          0x0008
        #define CFG_PATH_06_REG_DISABLE_PATH_ID_CHK_IN_PKT_CONVERTER            0x0010
        #define CFG_PATH_06_REG_FILTER_STREAM_ID_0_TO_1F_FOR_NAGRA_DONGLE       0x0080
        // filter null pkt
        #define CFG_PATH_06_REG_FILTER_NULL_PKT                                 0x0100
        #define CFG_PATH_06_REG_FIX_FILTER_NULL_PKT                             0x0200

    REG16       CFG_PATH_07;
        #define CFG_PATH_07_REG_CAVID_MASK                                      0x001F
        #define CFG_PATH_07_REG_CAVID_SHIFT                                     0
        #define CFG_PATH_07_REG_BYPASS_CA_PATH                                  0x0020
        #define CFG_PATH_07_REG_USE_PID_SLOT_MAP                                0x0040
        #define CFG_PATH_07_REG_DUP_PKT_SKIP                                    0x0080
        #define CFG_PATH_07_REG_ADP_DUP                                         0x0100
        #define CFG_PATH_07_REG_NDS_TEST_MODE                                   0x0200
        #define CFG_PATH_07_REG_DIS_DUP_FOR_NSK21                               0x0400
        #define CFG_PATH_07_REG_CA_DEST_GEN_EN                                  0x0800
        #define CFG_PATH_07_REG_CLEAR_REPLACE_EN_MASK                           0xF000
        #define CFG_PATH_07_REG_CLEAR_REPLACE_EN_SHIFT                          12

    REG16       CFG_PATH_08;
        #define CFG_PATH_08_REG_TEI_SKIP_PKT                                    0x0001
        #define CFG_PATH_08_REG_BLK_AF_SCRMB_BIT_TSP                            0x0002
        #define CFG_PATH_08_REG_UPDATE_SCRMB_BIT_BY_PUSI                        0x0004
        #define CFG_PATH_08_REG_PDFLT_OVERFLOW_INT_ENABLE                       0x0008
        #define CFG_PATH_08_REG_PDFLT_CLR_OVERFLOW_INT                          0x0010
        #define CFG_PATH_08_REG_REC_NULL_PKT                                    0x0020
        #define CFG_PATH_08_REG_ONEWAY_TO_LOCK_BYPASS_CA                        0x0040
        #define CFG_PATH_08_REG_PDFLT_PACKET_LENGTH_MASK                        0xFF00
        #define CFG_PATH_08_REG_PDFLT_PACKET_LENGTH_SHIFT                       8

    REG16       CFG_PATH_09;
        #define CFG_PATH_09_REG_CA_CHANNEL_ID_MASK                              0x000F
        #define CFG_PATH_09_REG_CA_CHANNEL_ID_SHIFT                             0
        #define CFG_PATH_09_REG_PKT_CNT_INIT                                    0x0010
        #define CFG_PATH_09_REG_TIMESTAMP_PKT_CNT_SEL                           0x0020
        #define CFG_PATH_09_REG_FIQ_MUX_OUT_PATH_SRC_MASK                       0xC000
        #define CFG_PATH_09_REG_FIQ_MUX_OUT_PATH_SRC_SHIFT                      14

    REG16       CFG_PATH_0A;
        #define CFG_PATH_0A_REG_DIS_NULL_PKT                                    0x0001
        #define CFG_PATH_0A_REG_START_READ_BYPASS_ENABLE                        0x0002
        #define CFG_PATH_0A_REG_MASK_PRIVILEGE_EN                               0x0004
        #define CFG_PATH_0A_REG_ONEWAY_AV_NOT_TO_SEC                            0x0008
        #define CFG_PATH_0A_REG_SKIP_SEC_RUSH_DATA                              0x0010
        #define CFG_PATH_0A_REG_SKIP_ADP_RUSH_DATA                              0x0020
        #define CFG_PATH_0A_REG_SEC_ERR_RM_EN                                   0x0040
        #define CFG_PATH_0A_REG_ALT_TS_SIZE                                     0x0080
        #define CFG_PATH_0A_REG_PKT_SIZE_SECTION_MASK                           0xFF00
        #define CFG_PATH_0A_REG_PKT_SIZE_SECTION_SHIFT                          8

    REG16       CFG_PATH_0B;                                                    // reg_err_pkt_cntr

    REG16       CFG_PATH_0C;
        #define CFG_PATH_0C_REG_CLR_BYTE_CNT                                    0x0001
        #define CFG_PATH_0C_REG_ERR_PKT_CNTR_CLR                                0x0002
        #define CFG_PATH_0C_REG_ERR_PKT_CNTR_LOAD                               0x0004

    REG16       CFG_PATH_0D;
        #define CFG_PATH_0D_REG_CHECK_TIMEOUT_CNT_TS_FIFO_MASK                  0x00FF
        #define CFG_PATH_0D_REG_CHECK_TIMEOUT_CNT_TS_FIFO_SHIFT                 0
        #define CFG_PATH_0D_REG_CHECK_TIMEOUT_CNT_PDFLT_MASK                    0xFF00
        #define CFG_PATH_0D_REG_CHECK_TIMEOUT_CNT_PDFLT_SHIFT                   8

    REG16       CFG_PATH_0E;
        #define CFG_PATH_0E_REG_RESET_PATH                                      0x0001
        #define CFG_PATH_0E_REG_RESET_TSIF                                      0x0002
        #define CFG_PATH_0E_REG_RESET_DIRECTV_130_188                           0x0004
        #define CFG_PATH_0E_REG_RESET_PKT_CONVERTER                             0x0008
        #define CFG_PATH_0E_REG_RESET_TS_DSS_CONVERTER                          0x0010
        #define CFG_PATH_0E_REG_RESET_FILTER_NULL_PKT                           0x0020
        #define CFG_PATH_0E_REG_RESET_SRC_ID_PARSER                             0x0040
        #define CFG_PATH_0E_REG_RESET_PDFLT                                     0x0080
        #define CFG_PATH_0E_REG_RESET_RBF_PDFLT                                 0x0100
        #define CFG_PATH_0E_REG_RESET_SCR_FILTER                                0x0200

    REG16       CFG_PATH_0F;
        #define CFG_PATH_0F_REG_CLK_GATING_TSP_PATH                             0x0001
        #define CFG_PATH_0F_REG_CLK_GATING_MIU_PATH                             0x0002
        #define CFG_PATH_0F_REG_CLK_GATING_PATH_LIVEIN                          0x0004
        #define CFG_PATH_0F_REG_CLK_GATING_PATH_FILEIN                          0x0008
        #define CFG_PATH_0F_REG_CHECK_TIMEOUT_ENABLE_0                          0x0010
        #define CFG_PATH_0F_REG_CHECK_TIMEOUT_ENABLE_1                          0x0020
        #define CFG_PATH_0F_REG_CHECK_TIMEOUT_ENABLE_2                          0x0040

} REG_PATH_ENG_Ctrl;

#endif // _REG_PATH_H_
