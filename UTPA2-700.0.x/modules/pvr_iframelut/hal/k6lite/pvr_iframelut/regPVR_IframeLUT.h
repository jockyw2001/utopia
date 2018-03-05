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
//  File name: regPVR_IframeLUT.h
//  Description: PVR Iframe LUT Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PVR_IframeLUT_REG_H_
#define _PVR_IframeLUT_REG_H_

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

typedef struct _REG32PVR_IframeLUT
{
    volatile MS_U16                 low;
    volatile MS_U16                 _null_l;
    volatile MS_U16                 high;
    volatile MS_U16                 _null_h;
} REG32PVR_IframeLUT;

typedef struct _REG16PVR_IframeLUT
{
    volatile MS_U16                 data;
    volatile MS_U16                 _null;
} REG16PVR_IframeLUT;

typedef struct _PVR_IframeLUT32
{
    volatile MS_U32                 REG32PVR_IframeLUT;
} PVR_IframeLUT32;

typedef struct _REG_Ctrl0 // PVR_IframeLUT LUT Bank:0x1736
{
    REG16PVR_IframeLUT   CFG0_00;
        #define CFG0_00_REG_SW_RSTB                                             0x0001
        #define CFG0_00_REG_TABLE_ID_AUTO_SET                                   0x0002  // choose table_id golden. 0: riu, 1:auto
        #define CFG0_00_REG_WB_ACK_LOC                                          0x0004
        #define CFG0_00_REG_CHANNEL_EN0                                         0x0100
        #define CFG0_00_REG_CHANNEL_EN1                                         0x0200
        #define CFG0_00_REG_CHANNEL_EN2                                         0x0400
        #define CFG0_00_REG_CHANNEL_EN3                                         0x0800
    REG16PVR_IframeLUT   CFG0_01;
        #define GOLDEN_TABLE_ID0_MASK                                           0x001F
        #define GOLDEN_TABLE_ID1_MASK                                           0x1F00
    REG16PVR_IframeLUT   CFG0_02;
        #define GOLDEN_TABLE_ID2_MASK                                           0x001F
        #define GOLDEN_TABLE_ID3_MASK                                           0x1F00
    REG32PVR_IframeLUT   CFG0_03_04;                                                      // set golden pes start code prefix value 0
    REG32PVR_IframeLUT   CFG0_05_06;                                                      // set golden pes start code prefix value 1
    REG32PVR_IframeLUT   CFG0_07_08;                                                      // set golden pes start code prefix value 2
    REG32PVR_IframeLUT   CFG0_09_0A;                                                      // set golden pes start code prefix value 3
    REG16PVR_IframeLUT   CFG0_0B;
        #define CFG0_0B_REG_LUT2MI_EN0                                          0x0001
        #define CFG0_0B_REG_LUT2MI_EN1                                          0x0002
        #define CFG0_0B_REG_LUT2MI_EN2                                          0x0004
        #define CFG0_0B_REG_LUT2MI_RST_WADR                                     0x0008  // reset lut2mi wadr
        #define CFG0_0B_REG_LUT2MI_DSWAP                                        0x0010  // swap lut2mi_data_in bit by bit
        #define CFG0_0B_REG_LUT2MI_DSWAP_BT_ORDER                               0x0020  // swap lut2mi_data_in byte by byte
        #define CFG0_0B_REG_LUT2MI_PAUSE                                        0x0040  // lut2mi pause data input
        #define CFG0_0B_REG_LUT2MI_WADR_LD                                      0x0080  // lut2mi wadr load to cpu
        #define CFG0_0B_REG_LUT2MI_INT_EN                                       0x0100  // interrupt enable
        #define CFG0_0B_REG_LUT2MI_INT_PTS_EN                                   0x0200  // PTS interrupt enable
    REG16PVR_IframeLUT   CFG0_0C;
        #define CFG0_0C_REG_LUT2MI_INT                                          0x0001  // lut2mi interrupt
        #define CFG0_0C_REG_LUT2MI_PTS_INT                                      0x0002  // pts interrupt

        #define REG_LUT2MI_ADDR_MASKMASK                                        0x0FFFFFFF
    REG16PVR_IframeLUT   CFG0_0D_0F[3];

    REG32PVR_IframeLUT   CFG0_10_11;                                                      // lut2mi dram start waddr for pvr0
    REG32PVR_IframeLUT   CFG0_12_13;                                                      // lut2mi dram mid waddr for pvr0
    REG32PVR_IframeLUT   CFG0_14_15;                                                      // lut2mi dram end waddr for pvr0

    REG32PVR_IframeLUT   CFG0_16_17;                                                      // lut2mi dram start waddr for pvr1
    REG32PVR_IframeLUT   CFG0_18_19;                                                      // lut2mi dram mid waddr for pvr1
    REG32PVR_IframeLUT   CFG0_1A_1B;                                                      // lut2mi dram end waddr for pvr1

    REG32PVR_IframeLUT   CFG0_1C_1D;                                                      // lut2mi dram start waddr for pvr2
    REG32PVR_IframeLUT   CFG0_1E_1F;                                                      // lut2mi dram mid waddr for pvr2
    REG32PVR_IframeLUT   CFG0_20_21;                                                      // lut2mi dram end waddr for pvr2

    REG32PVR_IframeLUT   CFG0_22_23;                                                      // lut2mi dram start waddr for pvr3
    REG32PVR_IframeLUT   CFG0_24_25;                                                      // lut2mi dram mid waddr for pvr3
    REG32PVR_IframeLUT   CFG0_26_27;                                                      // lut2mi dram end waddr for pvr3


    REG32PVR_IframeLUT   CFG0_28_29;                                                      // read current lut2mi_wadr
        #define REG_LUT2MI_WADR_R_MASKMASK                                      0x0FFFFFFF
    REG16PVR_IframeLUT   CFG0_2A;
        #define CFG0_2A_REG_SW_RSTZ_MEET_TAIL0                                  0x0001  // sw resets "meet tail0" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_MID0                                   0x0002  // sw resets "meet mid0" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_TAIL1                                  0x0004  // sw resets "meet tail1" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_MID1                                   0x0008  // sw resets "meet mid1" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_TAIL2                                  0x0010  // sw resets "meet tail2" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_MID2                                   0x0020  // sw resets "meet mid2" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_TAIL3                                  0x0040  // sw resets "meet tail3" status
        #define CFG0_2A_REG_SW_RSTZ_MEET_MID3                                   0x0080  // sw resets "meet mid3" status
    REG16PVR_IframeLUT   CFG0_2B;
        #define CFG0_2B_REG_MEET_TAIL0                                          0x0001  // meet tail0 status
        #define CFG0_2B_REG_MEET_MID0                                           0x0002  // meet mid1 status
        #define CFG0_2B_REG_MEET_TAIL1                                          0x0004  // meet tail1 status
        #define CFG0_2B_REG_MEET_MID1                                           0x0008  // meet mid1 status
        #define CFG0_2B_REG_MEET_TAIL2                                          0x0010  // meet tail2 status
        #define CFG0_2B_REG_MEET_MID2                                           0x0020  // meet mid2 status
        #define CFG0_2B_REG_MEET_TAIL3                                          0x0040  // meet tail3 status
        #define CFG0_2B_REG_MEET_MID3                                           0x0080  // meet mid3 status
    REG16PVR_IframeLUT   CFG0_2C_2F[4];
    REG32PVR_IframeLUT   CFG0_30_31_PTS0_L;                                               // PTS in channel0        // PTS0_low
    REG16PVR_IframeLUT   CFG0_32;
        #define CFG0_32_REG_PES2TS_PTS0_H                                       0x0001  // PTS in channel0        // PTS0_high
        #define CFG0_32_REG_PES2TS_PTS0_RSTB                                    0x0002  // reset PTS in channel0

    REG32PVR_IframeLUT   CFG0_33_34_PTS1_L;                                               // PTS in channel1        // PTS1_low
    REG16PVR_IframeLUT   CFG0_35;
        #define CFG0_35_REG_PES2TS_PTS1_H                                       0x0001  // PTS in channel1        // PTS1_high
        #define CFG0_35_REG_PES2TS_PTS1_RSTB                                    0x0002  // reset PTS in channe1l

    REG32PVR_IframeLUT   CFG0_36_37_PTS2_L;                                               // PTS in channel2        // PTS2_low
    REG16PVR_IframeLUT   CFG0_38;
        #define CFG0_38_REG_PES2TS_PTS2_H                                       0x0001  // PTS in channel2        // PTS2_high
        #define CFG0_38_REG_PES2TS_PTS2_RSTB                                    0x0002  // reset PTS in channel2

    REG32PVR_IframeLUT   CFG0_39_3A_PTS3_L;                                               // PTS in channel3        // PTS3_low
    REG16PVR_IframeLUT   CFG0_3B;
        #define CFG0_3B_REG_PES2TS_PTS3_H                                       0x0001  // PTS in channel3        // PTS3_high
        #define CFG0_3B_REG_PES2TS_PTS3_RSTB                                    0x0002  // reset PTS in channel3
    REG16PVR_IframeLUT   CFG0_3C_3E[3];
    REG16PVR_IframeLUT   CFG0_3F;                                                         // register scan number: h5cf3

} REG_Ctrl0;

typedef struct _REG_Ctrl1 // pvr_fsc Bank:0x1737 (PVR 1~4)
{
    REG16PVR_IframeLUT   CFG1_00;
        #define CFG1_00_REG_PVR_CHK_ID_MATCH_ON                                 0x0001  // When packet_header=0, get next packet only when stream_id matches.
        #define CFG1_00_REG_PVR_ID_CHK_QUALIFY_OFF                              0x0002  // 0: pes parser will not qualify stream_id with state machine
                                                                                        // 1: parser will take stream_id to be leagal only at ID_CHK state and PKT_DATA state when pkt_cnt >5 to avoid start code emulation.
        #define CFG1_00_REG_PVR_PKT_LEN_EN                                      0x0004  // 0: ignore reading packet length from PES stream. The packet length is taken as 0
                                                                                        // 1: read packet length from PES stream.
        #define CFG1_00_REG_PVR_IGNORE_ERR                                      0x0008  // Enable ignoring PES header parsing error
        #define CFG1_00_REG_PVR_TS_PUSI_DIS                                     0x0010  // Disable PES start code identification with PUSI flag
        #define CFG1_00_REG_PVR_SER_ERROR                                       0x0100  // write 1 to clear PES header parsing error flag
    REG16PVR_IframeLUT   CFG1_01;                                                         // PES stream ID
        #define CFG1_01_REG_PVR_STREAM_ID_MASK                                  0x00FF
        #define CFG1_01_REG_PVR_STREAM_ID_SHIFT                                 0
        #define CFG1_01_REG_PVR_STREAM_ID_MASK_MASK                             0xFF00
        #define CFG1_01_REG_PVR_STREAM_ID_MASK_SHIFT                            8
    REG16PVR_IframeLUT   CFG1_02;                                                         // Last detected PES stream ID
        #define CFG1_02_REG_PVR_PES_LAST_STREAM_ID_MASK                         0x00FF
        #define CFG1_02_REG_PVR_PES_LAST_STREAM_ID_SHIFT                        0
    REG16PVR_IframeLUT   CFG1_03;
        #define CFG1_03_REG_PVR_CODEC_MPGE                                      0x0001
        #define CFG1_03_REG_PVR_CODEC_H264                                      0x0002
        #define CFG1_03_REG_PVR_CODEC_HEVC                                      0x0003
        #define CFG1_03_REG_PVR_CODEC_AVS                                       0x0004
        #define CFG1_03_REG_PVR_SPS_TAG_EN                                      0x0010  // Enable SPS tag

    REG32PVR_IframeLUT   CFG1_04_05;                                                      // Start code prefix pattern
        #define REG_PVR_SC_PREFIX                                               0x00FFFFFF
    REG32PVR_IframeLUT   CFG1_06_07;                                                      // Start code prefix pattern mask
        #define REG_PVR_SC_PREFIX_MASK                                          0x00FFFFFF

    REG32PVR_IframeLUT   CFG1_08_09;                                                      // HEVC VCL NAL enable flag
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_TRAIL_N                          0x00000001
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_TRAIL_R                          0x00000002
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_TSA_N                            0x00000004
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_TSA_R                            0x00000008
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_STSA_N                           0x00000010
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_STSA_R                           0x00000020
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RADL_N                           0x00000040
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RADL_R                           0x00000080
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RASL_N                           0x00000100
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RASL_R                           0x00000200
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_VCL_N10                      0x00000400
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_VCL_R11                      0x00000800
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_VCL_N12                      0x00001000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_VCL_R13                      0x00002000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_VCL_N14                      0x00004000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_VCL_R15                      0x00008000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_BLA_W_LP                         0x00010000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_BLA_W_RADL                       0x00020000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_BLA_N_LP                         0x00040000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_IDR_W_RADL                       0x00080000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_IDR_N_LP                         0x00100000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_CRA_NUT                          0x00200000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_IRAP_VCL22                   0x00400000
        #define CFG1_08_09_REG_PVR_HEVC_NAL_EN_RSV_IRAP_VCL23                   0x00800000
    REG32PVR_IframeLUT   CFG1_0A_0B;                                                       // HEVC VCL IDR NAL flag
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_TRAIL_N                         0x00000001
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_TRAIL_R                         0x00000002
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_TSA_N                           0x00000004
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_TSA_R                           0x00000008
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_STSA_N                          0x00000010
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_STSA_R                          0x00000020
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RADL_N                          0x00000040
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RADL_R                          0x00000080
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RASL_N                          0x00000100
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RASL_R                          0x00000200
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_VCL_N10                     0x00000400
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_VCL_R11                     0x00000800
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_VCL_N12                     0x00001000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_VCL_R13                     0x00002000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_VCL_N14                     0x00004000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_VCL_R15                     0x00008000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_BLA_W_LP                        0x00010000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_BLA_W_RADL                      0x00020000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_BLA_N_LP                        0x00040000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_IDR_W_RADL                      0x00080000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_IDR_N_LP                        0x00100000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_CRA_NUT                         0x00200000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_IRAP_VCL22                  0x00400000
        #define CFG1_0A_0B_REG_PVR_HEVC_NAL_IDR_RSV_IRAP_VCL23                  0x00800000
    REG16PVR_IframeLUT   CFG1_0C_1F[20];

} REG_Ctrl1;

#endif // _PVR_IframeLUT_REG_H_
