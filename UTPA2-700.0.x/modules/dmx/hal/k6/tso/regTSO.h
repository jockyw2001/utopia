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
//  File name: regTSO.h
//  Description: TS I/O Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TSO_REG_H_
#define _TSO_REG_H_

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

//@TODO  check ENG PIDFLT TSIF number

#define TSO_ENGINE_NUM                      (1)
#define TSO_PIDFLT_NUM                      (256)
#define TSO_REP_PIDFLT_NUM                  (16)
#define TSO_FILE_IF_NUM                     (2)
#define TSO_TSIF_NUM                        (6)

#define TSO_PIDFLT_NUM_ALL                  TSO_PIDFLT_NUM

#define TSO_PID_NULL                        (0x1FFF)
#define TSO_MIU_BUS                         (4)
#define TSO_SVQ_UNIT_SIZE                   (208)

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define TSO_CLKIN_TS0                       0x00
#define TSO_CLKIN_TS1                       0x01
#define TSO_CLKIN_TS2                       0x02
#define TSO_CLKIN_TS3                       0x03
#define TSO_CLKIN_TS4                       0x04
#define TSO_CLKIN_TS5                       0x05
#define TSO_CLKIN_TS6                       0x06
#define TSO_CLKIN_TSO0_OUT_P                0x07
#define TSO_CLKIN_DMD                       0xFFFF //not supported

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_PIDFLT_BASE                     (0x210000UL)
#define TSO_PIDFLT_PID_MASK                 (0x1FFF)
#define TSO_PIDFLT_IN_MASK                  (0x7)
#define TSO_PIDFLT_IN_SHIFT                 (13)

#define REG_CTRL_BASE_TSO                   (0xE0C00UL)                            // 0x1706
#define REG_CTRL_BASE_TSO1                  (0xC2400UL)                            // 0x1612

typedef struct _REG32_TSO
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32_TSO;

typedef struct _REG16_TSO
{
    volatile MS_U16                data;
    volatile MS_U16                _resv;
} REG16_TSO;

//TSO0
typedef struct _REG_Ctrl_TSO
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
                                                                    // Index(word)  CPU(byte)     MIPS(0x13A00/2+index)*4
    REG16_TSO                             SW_RSTZ;                  //00
    #define TSO_SW_RSTZ                                             0x0001
    #define TSO_SW_RST_CLK_STAMP                                    0x0002
    #define TSO_SW_RST_CMDQ1                                        0x0100
    #define TSO_SW_RST_WB1                                          0x0200
    #define TSO_SW_RST_WB_DMA1                                      0x0400
    #define TSO_SW_RST_TS_FIN1                                      0x0800
    #define TSO_SW_RST_CMDQ                                         0x1000
    #define TSO_SW_RST_WB                                           0x2000
    #define TSO_SW_RST_WB_DMA                                       0x4000
    #define TSO_SW_RST_FIN                                          0x8000
    #define TSO_SW_RST_ALL                                          0xF002
    #define TSO_SW_RST_ALL1                                         0x0F02


    REG16_TSO                             SW_RSTZ1;                 //01
    #define TSO_SW_RST_CHANNEL_IF1                                  0x0001
    #define TSO_SW_RST_CHANNEL_IF2                                  0x0002
    #define TSO_SW_RST_CHANNEL_IF3                                  0x0004
    #define TSO_SW_RST_CHANNEL_IF4                                  0x0008
    #define TSO_SW_RST_CHANNEL_IF5                                  0x0010
    #define TSO_SW_RST_CHANNEL_IF6                                  0x0020

    REG16_TSO                             CFG_TSO_02_03[2];

    REG16_TSO                             CHANNEL0_IF1_CONFIG0;     //04
    #define TSO_CHANNEL0_IF1_CONFIG0_PKT_SIZE_CHK_LIVE_MASK         0x00FF //for internal sync
    #define TSO_CHANNEL0_IF1_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT        0
    #define TSO_CHANNEL0_IF1_CONFIG0_PIDFLT_PKT_SIZE_MASK           0xFF00
    #define TSO_CHANNEL0_IF1_CONFIG0_PIDFLT_PKT_SIZE_SHIFT          8

    REG16_TSO                             CHANNEL0_IF1_CONFIG1;     //05
    #define TSO_CHANNEL0_IF1_CONFIG1_SYNC_BYTE_MASK                 0x00FF
    #define TSO_CHANNEL0_IF1_CONFIG1_SYNC_BYTE_SHIFT                0
    #define TSO_CHANNEL0_IF1_CONFIG1_PKT_INPUT_MODE_MASK            0x0700
    #define TSO_CHANNEL0_IF1_CONFIG1_PKT_INPUT_MODE_SHIFT           8
    #define TSO_CHANNEL0_IF1_CONFIG1_PKT_HEADER_LEN_MASK            0xF800
    #define TSO_CHANNEL0_IF1_CONFIG1_PKT_HEADER_LEN_SHIFT           11

    REG16_TSO                             CHANNEL0_IF1_CONFIG2;     //06
    //----- for TV comaptibility -----//
    #define TSO_CHCFG_P_SEL                                         0x0001
    #define TSO_CHCFG_EXT_SYNC_SEL                                  0x0002
    #define TSO_CHCFG_TS_SIN_C0                                     0x0004
    #define TSO_CHCFG_TS_SIN_C1                                     0x0008
    #define TSO_CHCFG_PIDFLT_REC_ALL                                0x0010
    #define TSO_CHCFG_PIDFLT_REC_NULL                               0x0020
    #define TSO_CHCFG_PIDFLT_OVF_INT_EN                             0x0040
    #define TSO_CHCFG_PIDFLT_OVF_CLR                                0x0080
    #define TSO_CHCFG_FORCE_SYNC_BYTE                               0x0100
    #define TSO_CHCFG_SKIP_TEI_PKT                                  0x0200
    #define TSO_CHCFG_DIS_LOCKED_PKT_CNT                            0x0400
    #define TSO_CHCFG_CLR_LOCKED_PKT_CNT                            0x0800
    #define TSO_CHCFG_TRC_CLK_LD_DIS                                0x1000
    #define TSO_CHCFG_TRC_CLK_CLR                                   0x2000
    //--------------------------------//

    REG16_TSO                             CHANNEL0_IF1_CONFIG3;     //07    reserved

    REG16_TSO                             CHANNEL0_IF2_CONFIG0;     //08
    #define TSO_CHANNEL0_IF2_CONFIG0_PKT_SIZE_CHK_LIVE_MASK         0x00FF
    #define TSO_CHANNEL0_IF2_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT        0
    #define TSO_CHANNEL0_IF2_CONFIG0_PIDFLT_PKT_SIZE_MASK           0xFF00
    #define TSO_CHANNEL0_IF2_CONFIG0_PIDFLT_PKT_SIZE_SHIFT          8

    REG16_TSO                             CHANNEL0_IF2_CONFIG1;     //09
    #define TSO_CHANNEL0_IF2_CONFIG1_SYNC_BYTE_MASK                 0x00FF
    #define TSO_CHANNEL0_IF2_CONFIG1_SYNC_BYTE_SHIFT                0
    #define TSO_CHANNEL0_IF2_CONFIG1_PKT_INPUT_MODE_MASK            0x0700
    #define TSO_CHANNEL0_IF2_CONFIG1_PKT_INPUT_MODE_SHIFT           8
    #define TSO_CHANNEL0_IF2_CONFIG1_PKT_HEADER_LEN_MASK            0xF800
    #define TSO_CHANNEL0_IF2_CONFIG1_PKT_HEADER_LEN_SHIFT           11

    REG16_TSO                             CHANNEL0_IF2_CONFIG2;     //0a
    #define TSO_CHANNEL0_IF2_CONFIG2_P_SEL                          0x0001
    #define TSO_CHANNEL0_IF2_CONFIG2_EXT_SYNC_SEL                   0x0002
    #define TSO_CHANNEL0_IF2_CONFIG2_TS_SIN_C0                      0x0004
    #define TSO_CHANNEL0_IF2_CONFIG2_TS_SIN_C1                      0x0008
    #define TSO_CHANNEL0_IF2_CONFIG2_PIDFLT_REC_ALL                 0x0010
    #define TSO_CHANNEL0_IF2_CONFIG2_PIDFLT_REC_NULL                0x0020
    #define TSO_CHANNEL0_IF2_CONFIG2_PIDFLT_OVERFLOW_INT_EN         0x0040
    #define TSO_CHANNEL0_IF2_CONFIG2_PIDFLT_OVERFLOW_CLR            0x0080
    #define TSO_CHANNEL0_IF2_CONFIG2_FORCE_SYNC_BYTE                0x0100
    #define TSO_CHANNEL0_IF2_CONFIG2_SKIP_TEI_PKT                   0x0200
    #define TSO_CHANNEL0_IF2_CONFIG2_DIS_LOCKED_PKT_CNT             0x0400
    #define TSO_CHANNEL0_IF2_CONFIG2_CLR_LOCKED_PKT_CNT             0x0800
    #define TSO_CHANNEL0_IF2_CONFIG2_TRACING_CLOCK_LD_DIS           0x1000
    #define TSO_CHANNEL0_IF2_CONFIG2_TRACING_CLOCK_CLR              0x2000

    REG16_TSO                             CHANNEL0_IF2_CONFIG3;     //0b    reserved

    REG16_TSO                             CHANNEL0_IF3_CONFIG0;     //0c
    #define TSO_CHANNEL0_IF3_CONFIG0_PKT_SIZE_CHK_LIVE_MASK         0x00FF
    #define TSO_CHANNEL0_IF3_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT        0
    #define TSO_CHANNEL0_IF3_CONFIG0_PIDFLT_PKT_SIZE_MASK           0xFF00
    #define TSO_CHANNEL0_IF3_CONFIG0_PIDFLT_PKT_SIZE_SHIFT          8

    REG16_TSO                             CHANNEL0_IF3_CONFIG1;     //0d
    #define TSO_CHANNEL0_IF3_CONFIG1_SYNC_BYTE_MASK                 0x00FF
    #define TSO_CHANNEL0_IF3_CONFIG1_SYNC_BYTE_SHIFT                0
    #define TSO_CHANNEL0_IF3_CONFIG1_PKT_INPUT_MODE_MASK            0x0700
    #define TSO_CHANNEL0_IF3_CONFIG1_PKT_INPUT_MODE_SHIFT           8
    #define TSO_CHANNEL0_IF3_CONFIG1_PKT_HEADER_LEN_MASK            0xF800
    #define TSO_CHANNEL0_IF3_CONFIG1_PKT_HEADER_LEN_SHIFT           11

    REG16_TSO                             CHANNEL0_IF3_CONFIG2;     //0e
    #define TSO_CHANNEL0_IF3_CONFIG2_P_SEL                          0x0001
    #define TSO_CHANNEL0_IF3_CONFIG2_EXT_SYNC_SEL                   0x0002
    #define TSO_CHANNEL0_IF3_CONFIG2_TS_SIN_C0                      0x0004
    #define TSO_CHANNEL0_IF3_CONFIG2_TS_SIN_C1                      0x0008
    #define TSO_CHANNEL0_IF3_CONFIG2_PIDFLT_REC_ALL                 0x0010
    #define TSO_CHANNEL0_IF3_CONFIG2_PIDFLT_REC_NULL                0x0020
    #define TSO_CHANNEL0_IF3_CONFIG2_PIDFLT_OVERFLOW_INT_EN         0x0040
    #define TSO_CHANNEL0_IF3_CONFIG2_PIDFLT_OVERFLOW_CLR            0x0080
    #define TSO_CHANNEL0_IF3_CONFIG2_FORCE_SYNC_BYTE                0x0100
    #define TSO_CHANNEL0_IF3_CONFIG2_SKIP_TEI_PKT                   0x0200
    #define TSO_CHANNEL0_IF3_CONFIG2_DIS_LOCKED_PKT_CNT             0x0400
    #define TSO_CHANNEL0_IF3_CONFIG2_CLR_LOCKED_PKT_CNT             0x0800
    #define TSO_CHANNEL0_IF3_CONFIG2_TRACING_CLOCK_LD_DIS           0x1000
    #define TSO_CHANNEL0_IF3_CONFIG2_TRACING_CLOCK_CLR              0x2000

    REG16_TSO                             CHANNEL0_IF3_CONFIG3;     //0f    reserved

    REG16_TSO                             CHANNEL0_IF4_CONFIG0;     //10
    #define TSO_CHANNEL0_IF4_CONFIG0_PKT_SIZE_CHK_LIVE_MASK         0x00FF
    #define TSO_CHANNEL0_IF4_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT        0
    #define TSO_CHANNEL0_IF4_CONFIG0_PIDFLT_PKT_SIZE_MASK           0xFF00
    #define TSO_CHANNEL0_IF4_CONFIG0_PIDFLT_PKT_SIZE_SHIFT          8

    REG16_TSO                             CHANNEL0_IF4_CONFIG1;     //11
    #define TSO_CHANNEL0_IF4_CONFIG1_SYNC_BYTE_MASK                 0x00FF
    #define TSO_CHANNEL0_IF4_CONFIG1_SYNC_BYTE_SHIFT                0
    #define TSO_CHANNEL0_IF4_CONFIG1_PKT_INPUT_MODE_MASK            0x0700
    #define TSO_CHANNEL0_IF4_CONFIG1_PKT_INPUT_MODE_SHIFT           8
    #define TSO_CHANNEL0_IF4_CONFIG1_PKT_HEADER_LEN_MASK            0xF800
    #define TSO_CHANNEL0_IF4_CONFIG1_PKT_HEADER_LEN_SHIFT           11

    REG16_TSO                             CHANNEL0_IF4_CONFIG2;     //12
    #define TSO_CHANNEL0_IF4_CONFIG2_P_SEL                          0x0001
    #define TSO_CHANNEL0_IF4_CONFIG2_EXT_SYNC_SEL                   0x0002
    #define TSO_CHANNEL0_IF4_CONFIG2_TS_SIN_C0                      0x0004
    #define TSO_CHANNEL0_IF4_CONFIG2_TS_SIN_C1                      0x0008
    #define TSO_CHANNEL0_IF4_CONFIG2_PIDFLT_REC_ALL                 0x0010
    #define TSO_CHANNEL0_IF4_CONFIG2_PIDFLT_REC_NULL                0x0020
    #define TSO_CHANNEL0_IF4_CONFIG2_PIDFLT_OVERFLOW_INT_EN         0x0040
    #define TSO_CHANNEL0_IF4_CONFIG2_PIDFLT_OVERFLOW_CLR            0x0080
    #define TSO_CHANNEL0_IF4_CONFIG2_FORCE_SYNC_BYTE                0x0100
    #define TSO_CHANNEL0_IF4_CONFIG2_SKIP_TEI_PKT                   0x0200
    #define TSO_CHANNEL0_IF4_CONFIG2_DIS_LOCKED_PKT_CNT             0x0400
    #define TSO_CHANNEL0_IF4_CONFIG2_CLR_LOCKED_PKT_CNT             0x0800
    #define TSO_CHANNEL0_IF4_CONFIG2_TRACING_CLOCK_LD_DIS           0x1000
    #define TSO_CHANNEL0_IF4_CONFIG2_TRACING_CLOCK_CLR              0x2000

    REG16_TSO                             CHANNEL0_IF4_CONFIG3;     //13 reserved

    REG16_TSO                             CHANNEL0_IF5_CONFIG0;     //14
    #define TSO_CHANNEL0_IF5_CONFIG0_PKT_SIZE_CHK_LIVE_MASK         0x00FF
    #define TSO_CHANNEL0_IF5_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT        0
    #define TSO_CHANNEL0_IF5_CONFIG0_PIDFLT_PKT_SIZE_MASK           0xFF00
    #define TSO_CHANNEL0_IF5_CONFIG0_PIDFLT_PKT_SIZE_SHIFT          8

    REG16_TSO                             CHANNEL0_IF5_CONFIG1;     //15
    #define TSO_CHANNEL0_IF5_CONFIG1_SYNC_BYTE_MASK                 0x00FF
    #define TSO_CHANNEL0_IF5_CONFIG1_SYNC_BYTE_SHIFT                0
    #define TSO_CHANNEL0_IF5_CONFIG1_PKT_INPUT_MODE_MASK            0x0700
    #define TSO_CHANNEL0_IF5_CONFIG1_PKT_INPUT_MODE_SHIFT           8
    #define TSO_CHANNEL0_IF5_CONFIG1_PKT_HEADER_LEN_MASK            0xF800
    #define TSO_CHANNEL0_IF5_CONFIG1_PKT_HEADER_LEN_SHIFT           11

    REG16_TSO                             CHANNEL0_IF5_CONFIG2;     //16
    #define TSO_CHANNEL0_IF5_CONFIG2_P_SEL                          0x0001
    #define TSO_CHANNEL0_IF5_CONFIG2_EXT_SYNC_SEL                   0x0002
    #define TSO_CHANNEL0_IF5_CONFIG2_TS_SIN_C0                      0x0004
    #define TSO_CHANNEL0_IF5_CONFIG2_TS_SIN_C1                      0x0008
    #define TSO_CHANNEL0_IF5_CONFIG2_PIDFLT_REC_ALL                 0x0010
    #define TSO_CHANNEL0_IF5_CONFIG2_PIDFLT_REC_NULL                0x0020
    #define TSO_CHANNEL0_IF5_CONFIG2_PIDFLT_OVERFLOW_INT_EN         0x0040
    #define TSO_CHANNEL0_IF5_CONFIG2_PIDFLT_OVERFLOW_CLR            0x0080
    #define TSO_CHANNEL0_IF5_CONFIG2_FORCE_SYNC_BYTE                0x0100
    #define TSO_CHANNEL0_IF5_CONFIG2_SKIP_TEI_PKT                   0x0200
    #define TSO_CHANNEL0_IF5_CONFIG2_DIS_LOCKED_PKT_CNT             0x0400
    #define TSO_CHANNEL0_IF5_CONFIG2_CLR_LOCKED_PKT_CNT             0x0800
    #define TSO_CHANNEL0_IF5_CONFIG2_TRACING_CLOCK_LD_DIS           0x1000
    #define TSO_CHANNEL0_IF5_CONFIG2_TRACING_CLOCK_CLR              0x2000

    REG16_TSO                             CHANNEL0_IF5_CONFIG3;     //17 reserved

    REG16_TSO                             CHANNEL0_IF6_CONFIG0;     //18
    #define TSO_CHANNEL0_IF6_CONFIG0_PKT_SIZE_CHK_LIVE_MASK         0x00FF
    #define TSO_CHANNEL0_IF6_CONFIG0_PKT_SIZE_CHK_LIVE_SHIFT        0
    #define TSO_CHANNEL0_IF6_CONFIG0_PIDFLT_PKT_SIZE_MASK           0xFF00
    #define TSO_CHANNEL0_IF6_CONFIG0_PIDFLT_PKT_SIZE_SHIFT          8

    REG16_TSO                             CHANNEL0_IF6_CONFIG1;     //19
    #define TSO_CHANNEL0_IF6_CONFIG1_SYNC_BYTE_MASK                 0x00FF
    #define TSO_CHANNEL0_IF6_CONFIG1_SYNC_BYTE_SHIFT                0
    #define TSO_CHANNEL0_IF6_CONFIG1_PKT_INPUT_MODE_MASK            0x0700
    #define TSO_CHANNEL0_IF6_CONFIG1_PKT_INPUT_MODE_SHIFT           8
    #define TSO_CHANNEL0_IF6_CONFIG1_PKT_HEADER_LEN_MASK            0xF800
    #define TSO_CHANNEL0_IF6_CONFIG1_PKT_HEADER_LEN_SHIFT           11

    REG16_TSO                             CHANNEL0_IF6_CONFIG2;     //1a
    #define TSO_CHANNEL0_IF6_CONFIG2_P_SEL                          0x0001
    #define TSO_CHANNEL0_IF6_CONFIG2_EXT_SYNC_SEL                   0x0002
    #define TSO_CHANNEL0_IF6_CONFIG2_TS_SIN_C0                      0x0004
    #define TSO_CHANNEL0_IF6_CONFIG2_TS_SIN_C1                      0x0008
    #define TSO_CHANNEL0_IF6_CONFIG2_PIDFLT_REC_ALL                 0x0010
    #define TSO_CHANNEL0_IF6_CONFIG2_PIDFLT_REC_NULL                0x0020
    #define TSO_CHANNEL0_IF6_CONFIG2_PIDFLT_OVERFLOW_INT_EN         0x0040
    #define TSO_CHANNEL0_IF6_CONFIG2_PIDFLT_OVERFLOW_CLR            0x0080
    #define TSO_CHANNEL0_IF6_CONFIG2_FORCE_SYNC_BYTE                0x0100
    #define TSO_CHANNEL0_IF6_CONFIG2_SKIP_TEI_PKT                   0x0200
    #define TSO_CHANNEL0_IF6_CONFIG2_DIS_LOCKED_PKT_CNT             0x0400
    #define TSO_CHANNEL0_IF6_CONFIG2_CLR_LOCKED_PKT_CNT             0x0800
    #define TSO_CHANNEL0_IF6_CONFIG2_TRACING_CLOCK_LD_DIS           0x1000
    #define TSO_CHANNEL0_IF6_CONFIG2_TRACING_CLOCK_CLR              0x2000

    REG16_TSO                             CHANNEL0_IF6_CONFIG3;     //1b reserved

    REG16_TSO                             TSO_CONFIG0;              //1c
    #define TSO_CONFIG0_S2P_EN                                      0x0001
    #define TSO_CONFIG0_S2P_TS_SIN_C0                               0x0002
    #define TSO_CONFIG0_S2P_TS_SIN_C1                               0x0004
    #define TSO_CONFIG0_S2P_3WIRE_MODE                              0x0008
    #define TSO_CONFIG0_BYPASS_S2P                                  0x0010
    #define TSO_CONFIG0_S2P1_EN                                     0x0100
    #define TSO_CONFIG0_S2P1_TS_SIN_C0                              0x0200
    #define TSO_CONFIG0_S2P1_TS_SIN_C1                              0x0400
    #define TSO_CONFIG0_S2P1_3WIRE_MODE                             0x0800
    #define TSO_CONFIG0_BYPASS_S2P1                                 0x1000

    REG16_TSO                             TSO_CONFIG1;              //1d
    //----- for TV comaptibility -----//
    #define TSO_CFG1_TSO_OUT_EN                                     0x0001
    #define TSO_CFG1_TSO_TSIF1_EN                                   0x0002
    #define TSO_CFG1_TSO_TSIF2_EN                                   0x0004
    #define TSO_CFG1_TSO_TSIF3_EN                                   0x0008
    #define TSO_CFG1_TSO_TSIF4_EN                                   0x0010
    #define TSO_CFG1_TSO_TSIF5_EN                                   0x0020
    #define TSO_CFG1_TSO_TSIF6_EN                                   0x0040
    //--------------------------------//
    #define TSO_CONFIG1_PAUSE_OPIF                                  0x0080
    #define TSO_CONFIG1_TURN_OFF_MCM                                0x0100
    #define TSO_CONFIG1_CLOCK_TRACING_SEL_MASK                      0x0E00
    #define TSO_CONFIG1_CLOCK_TRACING_SEL_SHIFT                     9
    #define TSO_CONFIG1_SERIAL_OUT_EN                               0x1000
    #define TSO_CONFIG1_PKT_LOCK_CLR                                0x2000
    #define TSO_CONFIG1_PKT_NULL_EN                                 0x4000
    //----- for TV comaptibility -----//
    #define TSO_CFG1_PKT_PARAM_LD                                   0x8000
    //--------------------------------//

    REG16_TSO                             TSO_CONFIG2;              //1e
    #define TSO_CONFIG2_VALID_BYTE_CNT_MASK                         0x00FF
    #define TSO_CONFIG2_VALID_BYTE_CNT_SHIFT                        0
    #define TSO_CONFIG2_INVALID_BYTE_CNT_MASK                       0xFF00
    #define TSO_CONFIG2_INVALID_BYTE_CNT_SHIFT                      8

    REG16_TSO                             TSO_CONFIG3;              //1f
    #define TSO_CONFIG3_OPIF_PKT_SIZE_MASK                          0xFFFF

    REG32_TSO                             PIDFLTS[16];              //20~3e  PID00~0F
    //FOR ALL PID
    #define TSO_PID_ORIGINAL_PID_MASK                               0x00001FFF
    #define TSO_PID_ORIGINAL_PID_SHIFT                              0
    #define TSO_PID_SOURCE_SEL_MASK                                 0x0000E000
    #define TSO_PID_SOURCE_SEL_SHIFT                                13
    #define TSO_PID_NEW_PID_MASK                                    0x1FFF0000
    #define TSO_PID_NEW_PID_SHIFT                                   16
    #define TSO_PID_REPLACE_EN                                      0x80000000

    REG16_TSO                             CLR_BYTE_CNT;             //40
    #define TSO_CLR_BYTE_CNT_1                                      0x0001
    #define TSO_CLR_BYTE_CNT_2                                      0x0002
    #define TSO_CLR_BYTE_CNT_3                                      0x0004
    #define TSO_CLR_BYTE_CNT_4                                      0x0008
    #define TSO_CLR_BYTE_CNT_5                                      0x0010
    #define TSO_CLR_BYTE_CNT_6                                      0x0020

    REG16_TSO                             CFG_TSO_41_42[2];         //41~42

    REG16_TSO                             TSO_CONFIG4;              //43
    #define TSO_CFG4_LOCK_RETURN_SYSTEM_TIMESTAMP                   0x0001
    #define TSO_CFG4_ENABLE_SYS_TIMESTAMP                           0x0002
    #define TSO_CFG4_SET_SYS_TIMESTAMP_TO_HW                        0x0004
    #define TSO_CFG4_TIMESTAMP_BASE                                 0x0008//0:90k 1:27m
    #define TSO_CFG4_PDTABLE_SRAM_SD_EN                             0x0010
    #define TSO_CFG4_NULL_PKT_ID_MASK                               0xFF00
    #define TSO_CFG4_NULL_PKT_ID_SHIFT                              8

    REG16_TSO                             TSO_CONFIG5;              //44
    #define TSO_CONFIG5_3_WIRE_EN_1                                 0x0001
    #define TSO_CONFIG5_3_WIRE_EN_2                                 0x0002
    #define TSO_CONFIG5_3_WIRE_EN_3                                 0x0004
    #define TSO_CONFIG5_3_WIRE_EN_4                                 0x0008
    #define TSO_CONFIG5_3_WIRE_EN_5                                 0x0010
    #define TSO_CONFIG5_3_WIRE_EN_6                                 0x0020
    #define TSO_CONFIG5_DIS_MIU_RQ                                  0x0040
	#define TSO_CONFIG5_FIXED_MIU_REG_FLUSH                         0x0080
  	#define TSO_CONFIG5_TSIO_MODE                                   0x0400
    #define TSO_CONFIG5_TSIO2OPIF                                   0x0800

    REG16_TSO                             PDTABLE_ADDR_L;           //45 ind R/W of L addr to pdtable
    REG16_TSO                             PDTABLE_ADDR_H;           //46 ind R/W of H addr to pdtable

    REG16_TSO                             PDTABLE_WDATA_L;          //47 ind R/W of L addr to pdtable
    REG16_TSO                             PDTABLE_WDATA_H;          //48 ind R/W of L addr to pdtable

    REG16_TSO                             PDTABLE_RDATA;            //49 ind of Rdata from pdtable

    REG16_TSO                             PDTABLE_EN;               //4a
    #define TSO_PDTABLE_W_EN                                        0x0001//Ind W flag to pdtable
    #define TSO_PDTABLE_R_EN                                        0x0002//Ind R flag to pdtable

    REG16_TSO                             TSO_STATUS;               //4b
    #define TSO_STATUS_SVQ_MASK                                     0x7F00
    #define TSO_STATUS_SVQ_SHIFT                                    8
    #define TSO_STATUS_PDFLT                                        0x8000

    REG16_TSO                             FILE_TIMER[2];            //4c ~ 4d

    REG16_TSO                             TSO_STATUS1;              //4e
    #define TSO_STATUS1_EVEROVERFLOW_TSIF_1                         0x0001
    #define TSO_STATUS1_EVEROVERFLOW_TSIF_2                         0x0002
    #define TSO_STATUS1_EVEROVERFLOW_TSIF_3                         0x0004
    #define TSO_STATUS1_EVEROVERFLOW_TSIF_4                         0x0008
    #define TSO_STATUS1_EVEROVERFLOW_TSIF_5                         0x0010
    #define TSO_STATUS1_EVEROVERFLOW_TSIF_6                         0x0020

    REG16_TSO                             CFG_TSO_4F_5A[12];        //4f~5a

    REG16_TSO                             TSO_TRACING_HIGH;         //5b
    REG16_TSO                             TSO_TRACING_LOW;          //5c
    REG16_TSO                             TSO_TRACING_1T;           //5d
    REG16_TSO                             TSO_BLOCK_SIZE_DB;        //5e
    REG16_TSO                             TSO_OPT_SZIE_DB;          //5f

    REG32_TSO                             CFG_TSO_60_63[2];         //60~63
    REG16_TSO                             TSO_Filein_Ctrl;          //64
    REG32_TSO                             CFG_TSO_65_68[2];         //65~68
    REG16_TSO                             TSO_Filein_Ctrl1;         //69
    #define TSO_FILEIN_CTRL_MASK                                    0x0007
    #define TSO_FILEIN_RSTART                                       0x0001
    #define TSO_FILEIN_ABORT                                        0x0002
    #define TSO_FILEIN_TRUST                                        0x0004

    REG16_TSO                             PKT_CNT_SEL;              //6a
    #define TSO_PKT_CNT_RETURN_SEL_MASK                             0x000F
    #define TSO_PKT_CNT_RETURN_SEL_SHIFT                            0
    #define TSO_PKT_CNT_DBG_LOCKED_PKT_CNT_MASK                     0x00F0
    #define TSO_PKT_CNT_DBG_LOCKED_PKT_CNT_SHIFT                    4
    #define TSO_PKT_CNT_DBG_PKT_CNTT_DBG_MASK                       0xFF00
    #define TSO_PKT_CNT_DBG_PKT_CNTT_DBG_SHIFT                      8

    REG16_TSO                             PKT_CHK_SIZE_FIN;         //6b
    #define TSO_PKT_CHK_SIZE_FIN_MASK                               0x00FF
    #define TSO_PKT_CHK_SIZE_FIN_SHIFT                              0
    #define TSO_PKT_CHK_SIZE_FIN1_MASK                              0xFF00
    #define TSO_PKT_CHK_SIZE_FIN1_SHIFT                             8

    REG32_TSO                             LPCR2_BUF;                //6c~6d
    REG32_TSO                             LPCR2_BUF1;               //6e~6f

    REG32_TSO                             TIMESTAMP;                //70~71
    REG32_TSO                             TIMESTAMP1;               //72~73

    REG32_TSO                             TSO2MI_RADDR;             //74~75
    REG32_TSO                             TSO2MI_RADDR1;            //76~77

    REG16_TSO                             CMD_QUEUE_STATUS;         //78
    #define TSO_CMDQ_SIZE                                           16
    #define TSO_CMD_QUEUE_STATUS_CMD_WR_COUNT_MASK                  0x000F
    #define TSO_CMD_QUEUE_STATUS_CMD_WR_COUNT_SHIFT                 0
    #define TSO_CMD_QUEUE_STATUS_CMD_WR_LEVEL_MASK                  0x0030
    #define TSO_CMD_QUEUE_STATUS_CMD_WR_LEVEL_SHIFT                 4
    #define TSO_CMD_QUEUE_STATUS_CMD_FIFO_FULL                      0x0040
    #define TSO_CMD_QUEUE_STATUS_CMD_FIFO_EMPTY                     0x0080
    #define TSO_CMD_QUEUE_STATUS1_CMD_WR_COUNT_MASK                 0x0F00
    #define TSO_CMD_QUEUE_STATUS1_CMD_WR_COUNT_SHIFT                8
    #define TSO_CMD_QUEUE_STATUS1_CMD_WR_LEVEL_MASK                 0x3000
    #define TSO_CMD_QUEUE_STATUS1_CMD_WR_LEVEL_SHIFT                12
    #define TSO_CMD_QUEUE_STATUS1_CMD_FIFO_FULL                     0x4000
    #define TSO_CMD_QUEUE_STATUS1_CMD_FIFO_EMPTY                    0x8000

    REG16_TSO                             TSO_FILE_CONFIG;          //79
    #define TSO_FILE_CONFIG_TSO2MI_RPRIORITY                        0x0001
    #define TSO_FILE_CONFIG_MEM_TS_DATA_ENDIAN                      0x0002
    #define TSO_FILE_CONFIG_MEM_TS_W_ORDER                          0x0004
    #define TSO_FILE_CONFIG_LPCR2_WLD                               0x0008
    #define TSO_FILE_CONFIG_LPCR2_LOAD                              0x0010
    #define TSO_FILE_CONFIG_DIS_MIU_RQ                              0x0020
    #define TSO_FILE_CONFIG_TSO_RADDR_READ                          0x0040
    #define TSO_FILE_CONFIG_TS_DATA_PORT_SEL                        0x0080
    #define TSO_FILE_CONFIG_TSO_FILE_IN                             0x0100
    #define TSO_FILE_CONFIG_TIMER_EN                                0x0200
    #define TSO_FILE_CONFIG_PKT_192_BLK_DISABLE                     0x0400
    #define TSO_FILE_CONFIG_PKT_192_EN                              0x0800
    #define TSO_FILE_CONFIG_TSP_FILE_SEGMENT                        0x1000
    #define TSO_FILE_CONFIG_CLK_STAMP_27_EN                         0x2000

    REG16_TSO                             TSO_FILE_CONFIG1;         //7a
    #define TSO_FILE_CONFIG1_TSO2MI_RPRIORITY                       0x0001
    #define TSO_FILE_CONFIG1_MEM_TS_DATA_ENDIAN                     0x0002
    #define TSO_FILE_CONFIG1_MEM_TS_W_ORDER                         0x0004
    #define TSO_FILE_CONFIG1_LPCR2_WLD                              0x0008
    #define TSO_FILE_CONFIG1_LPCR2_LOAD                             0x0010
    #define TSO_FILE_CONFIG1_DIS_MIU_RQ                             0x0020
    #define TSO_FILE_CONFIG1_TSO_RADDR_READ                         0x0040
    #define TSO_FILE_CONFIG1_TS_DATA_PORT_SEL                       0x0080
    #define TSO_FILE_CONFIG1_TSO_FILE_IN                            0x0100
    #define TSO_FILE_CONFIG1_TIMER_EN                               0x0200
    #define TSO_FILE_CONFIG1_PKT_192_BLK_DISABLE                    0x0400
    #define TSO_FILE_CONFIG1_PKT_192_EN                             0x0800
    #define TSO_FILE_CONFIG1_TSP_FILE_SEGMENT                       0x1000
    #define TSO_FILE_CONFIG1_CLK_STAMP_27_EN                        0x2000

    REG16_TSO                             INTERRUPT;                //7b
    #define TSO_INT_SRC_MASK                                        0x00FF
    #define TSO_INT_STS_MASK                                        0xFF00
    //----- for TV comaptibility -----//
    #define TSO_INT_DMA_DONE                                        0x0001
    #define TSO_INT_DMA_DONE1                                       0x0002
    //--------------------------------//
    #define TSO_INT_SRC_TRAC_CLK_UPDATE                             0x0004
    #define TSO_INT_STS_DMA_DONE                                    0x0100
    #define TSO_INT_STS_DMA_DONE1                                   0x0200
    #define TSO_INT_STS_TRAC_CLK_UPDATE                             0x0400

    REG16_TSO                             INTERRUPT1;               //7c
    #define TSO_INT_SRC_PIDFLT1_OVERFLOW                            0x0001
    #define TSO_INT_SRC_PIDFLT2_OVERFLOW                            0x0002
    #define TSO_INT_SRC_PIDFLT3_OVERFLOW                            0x0004
    #define TSO_INT_SRC_PIDFLT4_OVERFLOW                            0x0008
    #define TSO_INT_SRC_PIDFLT5_OVERFLOW                            0x0010
    #define TSO_INT_SRC_PIDFLT6_OVERFLOW                            0x0020

    #define TSO_INT_STS_PIDFLT1_OVERFLOW                            0x0100
    #define TSO_INT_STS_PIDFLT2_OVERFLOW                            0x0200
    #define TSO_INT_STS_PIDFLT3_OVERFLOW                            0x0400
    #define TSO_INT_STS_PIDFLT4_OVERFLOW                            0x0800
    #define TSO_INT_STS_PIDFLT5_OVERFLOW                            0x1000
    #define TSO_INT_STS_PIDFLT6_OVERFLOW                            0x2000

    REG32_TSO                             TSO_DEBUG;                //7d~7e

    REG16_TSO                             DBG_SEL;                  //7f

} REG_Ctrl_TSO;


//TSO1
typedef struct _REG_Ctrl_TSO1
{
    //----------------------------------------------
    // 0xBF802C00 MIPS direct access
    //----------------------------------------------

    REG16_TSO                             REG_PRE_HEADER_1_CONFIG_0;    //00
    #define TSO1_REG_PRE_HEADER_1_CONFIG_0_LOCAL_STREAMID_MASK      0x00FF
    #define TSO1_REG_PRE_HEADER_1_CONFIG_0_LOCAL_STREAMID_SHIFT     0

    REG16_TSO                             REG_PRE_HEADER_1_CONFIG_1;    //01
    REG16_TSO                             REG_PRE_HEADER_1_CONFIG_2;    //02
    REG16_TSO                             REG_PRE_HEADER_1_CONFIG_3;    //03

    REG16_TSO                             REG_PRE_HEADER_2_CONFIG_0;    //04
    #define TSO1_REG_PRE_HEADER_2_CONFIG_0_LOCAL_STREAMID_MASK      0x00FF
    #define TSO1_REG_PRE_HEADER_2_CONFIG_0_LOCAL_STREAMID_SHIFT     0

    REG16_TSO                             REG_PRE_HEADER_2_CONFIG_1;    //05
    REG16_TSO                             REG_PRE_HEADER_2_CONFIG_2;    //06
    REG16_TSO                             REG_PRE_HEADER_2_CONFIG_3;    //07

    REG16_TSO                             REG_PRE_HEADER_3_CONFIG_0;    //08
    #define TSO1_REG_PRE_HEADER_3_CONFIG_0_LOCAL_STREAMID_MASK      0x00FF
    #define TSO1_REG_PRE_HEADER_3_CONFIG_0_LOCAL_STREAMID_SHIFT     0

    REG16_TSO                             REG_PRE_HEADER_3_CONFIG_1;    //09
    REG16_TSO                             REG_PRE_HEADER_3_CONFIG_2;    //0a
    REG16_TSO                             REG_PRE_HEADER_3_CONFIG_3;    //0b

    REG16_TSO                             REG_PRE_HEADER_4_CONFIG_0;    //0c
    #define TSO1_REG_PRE_HEADER_4_CONFIG_0_LOCAL_STREAMID_MASK      0x00FF
    #define TSO1_REG_PRE_HEADER_4_CONFIG_0_LOCAL_STREAMID_SHIFT     0

    REG16_TSO                             REG_PRE_HEADER_4_CONFIG_1;    //0d
    REG16_TSO                             REG_PRE_HEADER_4_CONFIG_2;    //0e
    REG16_TSO                             REG_PRE_HEADER_4_CONFIG_3;    //0f

    REG16_TSO                             REG_PRE_HEADER_5_CONFIG_0;    //10
    #define TSO1_REG_PRE_HEADER_5_CONFIG_0_LOCAL_STREAMID_MASK      0x00FF
    #define TSO1_REG_PRE_HEADER_5_CONFIG_0_LOCAL_STREAMID_SHIFT     0

    REG16_TSO                             REG_PRE_HEADER_5_CONFIG_1;    //11
    REG16_TSO                             REG_PRE_HEADER_5_CONFIG_2;    //12
    REG16_TSO                             REG_PRE_HEADER_5_CONFIG_3;    //13

    REG16_TSO                             REG_PRE_HEADER_6_CONFIG_0;    //14
    #define TSO1_REG_PRE_HEADER_6_CONFIG_0_LOCAL_STREAMID_MASK      0x00FF
    #define TSO1_REG_PRE_HEADER_6_CONFIG_0_LOCAL_STREAMID_SHIFT     0

    REG16_TSO                             REG_PRE_HEADER_6_CONFIG_1;    //15
    REG16_TSO                             REG_PRE_HEADER_6_CONFIG_2;    //16
    REG16_TSO                             REG_PRE_HEADER_6_CONFIG_3;    //17

    REG32_TSO                             SVQ1_BASE;                    //18~19
    #define TSO1_SVQ1_BASE_MASK                                     0x0FFFFFFF
    #define TSO1_SVQ1_BASE_SHIFT                                    0

    REG16_TSO                             SVQ1_SIZE_200BYTE;            //1a
    #define TSO1_SVQ1_SIZE_200BYTE_SVQ_SIZE_MASK                    0xFFFF
    #define TSO1_SVQ1_SIZE_200BYTE_SVQ_SIZE_SHIFT                   0

    REG16_TSO                             SVQ1_TX_CONFIG;               //1b
    #define TSO1_SVQ1_TX_CONFIG_WR_THRESHOLD_MASK                   0x000F
    #define TSO1_SVQ1_TX_CONFIG_WR_THRESHOLD_SHIFT                  0
    #define TSO1_SVQ1_TX_CONFIG_PRIORITY_THRESHOLD_MASK             0x00F0
    #define TSO1_SVQ1_TX_CONFIG_PRIORITY_THRESHOLD_SHIFT            4
    #define TSO1_SVQ1_TX_CONFIG_FORCEFIRE_CNT_MASK                  0x0F00
    #define TSO1_SVQ1_TX_CONFIG_FORCEFIRE_CNT_SHIFT                 8
    #define TSO1_SVQ1_TX_CONFIG_TX_RESET                            0x1000
    #define TSO1_SVQ1_TX_CONFIG_OVERFLOW_INT_EN                     0x2000
    #define TSO1_SVQ1_TX_CONFIG_OVERFLOW_CLR                        0x4000
    #define TSO1_SVQ1_TX_CONFIG_SVQ_TX_ENABLE                       0x8000
    REG32_TSO                             SVQ2_BASE;                    //1C~1D
    REG16_TSO                             SVQ2_SIZE_200BYTE;            //1E
    REG16_TSO                             SVQ2_TX_CONFIG;               //1F
    REG32_TSO                             SVQ3_BASE;                    //20~21
    REG16_TSO                             SVQ3_SIZE_200BYTE;            //22
    REG16_TSO                             SVQ3_TX_CONFIG;               //23
    REG32_TSO                             SVQ4_BASE;                    //24~25
    REG16_TSO                             SVQ4_SIZE_200BYTE;            //26
    REG16_TSO                             SVQ4_TX_CONFIG;               //27
    REG32_TSO                             SVQ5_BASE;                    //28~29
    REG16_TSO                             SVQ5_SIZE_200BYTE;            //2a
    REG16_TSO                             SVQ5_TX_CONFIG;               //2b
    REG32_TSO                             SVQ6_BASE;                    //2C~2D
    REG16_TSO                             SVQ6_SIZE_200BYTE;            //2E
    REG16_TSO                             SVQ6_TX_CONFIG;               //2F

    REG16_TSO                             SVQ_RX_CONFIG;                //30
    #define TSO1_SVQ_RX_CONFIG_MODE_MASK                            0x0003 //00=open cable 01=CI+ 10=192 mode
    #define TSO1_SVQ_RX_CONFIG_MODE_SHIT                            0
    //----- for TV comaptibility -----//
    #define TSO_SVQ_RX_CFG_MODE_OPENCBL                             0x0000
    #define TSO_SVQ_RX_CFG_MODE_CIPL                                0x0001
    #define TSO_SVQ_RX_CFG_MODE_192PKT                              0x0002
    //--------------------------------//
    #define TSO1_SVQ_RX_CONFIG_RD_THRESHOLD_MASK                    0x001C //000=1/6 empty 001=1/8 empty 010=1/4 empty 011=1/2 empty  else empty
    #define TSO1_SVQ_RX_CONFIG_RD_THRESHOLD_SHIT                    2
    #define TSO1_SVQ_RX_CONFIG_ARBITOR_MODE_MASK                    0x0060 //00=Run-Robin. 01=fix priority by REG 0x31~33 mode 10=dynamic priority
    #define TSO1_SVQ_RX_CONFIG_ARBITOR_MODE_SHIT                    5
    //----- for TV comaptibility -----//
    #define TSO_SVQ_RX_CFG_ARBMODE_RUNROBIN                         0x0000
    #define TSO_SVQ_RX_CFG_ARBMODE_FIXPRI                           0x0001
    #define TSO_SVQ_RX_CFG_ARBMODE_DYMPRI                           0x0002
    //--------------------------------//
    #define TSO1_SVQ_RX_CONFIG_SRAM_SD_EN                           0x0080
    #define TSO1_SVQ_RX_CONFIG_SVQ_FORCE_RESET                      0x0100
    #define TSO1_SVQ_RX_CONFIG_SVQ_MIU_NS                           0x0200
    #define TSO1_SVQ_RX_CONFIG_SVQ_MOBF_INDEX_MASK                  0x7C00
    #define TSO1_SVQ_RX_CONFIG_SVQ_MOBF_INDEX_SHIFT                 10
    #define TSO1_SVQ_RX_CONFIG_SVQ_DYNAMIC_PRI                      0x8000

    REG16_TSO                             SVQ_RX_1_2_PRIORITY;          //31
    #define TSO1_SVQ_RX1_PRIORITY_MASK                              0x003F
    #define TSO1_SVQ_RX1_PRIORITY_SHIFT                             0
    #define TSO1_SVQ_RX2_PRIORITY_MASK                              0x3F00
    #define TSO1_SVQ_RX2_PRIORITY_SHIFT                             8

    REG16_TSO                             SVQ_RX_3_4_PRIORITY;          //32
    #define TSO1_SVQ_RX3_PRIORITY_MASK                              0x003F
    #define TSO1_SVQ_RX3_PRIORITY_SHIFT                             0
    #define TSO1_SVQ_RX4_PRIORITY_MASK                              0x3F00
    #define TSO1_SVQ_RX4_PRIORITY_SHIFT                             8

    REG16_TSO                             SVQ_RX_5_6_PRIORITY;          //33
    #define TSO1_SVQ_RX5_PRIORITY_MASK                              0x003F
    #define TSO1_SVQ_RX5_PRIORITY_SHIFT                             0
    #define TSO1_SVQ_RX6_PRIORITY_MASK                              0x3F00
    #define TSO1_SVQ_RX6_PRIORITY_SHIFT                             8

    REG32_TSO                             SVQ_STATUS;                   //34~35
    //----- for TV comaptibility -----//
    #define TSO_SVQ_STS_MASK                                        0x000F
    #define TSO_SVQ1_STS_SHIFT                                      0
    #define TSO_SVQ2_STS_SHIFT                                      4
    #define TSO_SVQ3_STS_SHIFT                                      8
    #define TSO_SVQ4_STS_SHIFT                                      12
    #define TSO_SVQ5_STS_SHIFT                                      16
    #define TSO_SVQ6_STS_SHIFT                                      20
    #define TSO_SVQ_STS_EVER_FULL                                   0x0001
    #define TSO_SVQ_STS_EVER_OVF                                    0x0002
    #define TSO_SVQ_STS_EMPTY                                       0x0004
    #define TSO_SVQ_STS_BUSY                                        0x0008
    //--------------------------------//
    #define TSO1_SVQ1_OVERFLOW_INT                                  0x01000000
    #define TSO1_SVQ2_OVERFLOW_INT                                  0x02000000
    #define TSO1_SVQ3_OVERFLOW_INT                                  0x04000000
    #define TSO1_SVQ4_OVERFLOW_INT                                  0x08000000
    #define TSO1_SVQ5_OVERFLOW_INT                                  0x10000000
    #define TSO1_SVQ6_OVERFLOW_INT                                  0x20000000

    REG32_TSO                             SVQ_STATUS2;                  //36~37
    #define TSO1_SVQ1_TX_WATER_LEVEL_MASK                           0x00000003
    #define TSO1_SVQ1_TX_WATER_LEVEL_SHIFT                          0
    #define TSO1_SVQ1_TX_FULL_MASK                                  0x00000004
    #define TSO1_SVQ1_TX_FULL_SHIFT                                 2
    #define TSO1_SVQ1_TX_EMPTY_MASK                                 0x00000008
    #define TSO1_SVQ1_TX_EMPTY_SHIFT                                3
    #define TSO1_SVQ2_TX_WATER_LEVEL_MASK                           0x00000030
    #define TSO1_SVQ2_TX_WATER_LEVEL_SHIFT                          4
    #define TSO1_SVQ2_TX_FULL_MASK                                  0x00000040
    #define TSO1_SVQ2_TX_FULL_SHIFT                                 6
    #define TSO1_SVQ2_TX_EMPTY_MASK                                 0x00000080
    #define TSO1_SVQ2_TX_EMPTY_SHIFT                                7
    #define TSO1_SVQ3_TX_WATER_LEVEL_MASK                           0x00000300
    #define TSO1_SVQ3_TX_WATER_LEVEL_SHIFT                          8
    #define TSO1_SVQ3_TX_FULL_MASK                                  0x00000400
    #define TSO1_SVQ3_TX_FULL_SHIFT                                 10
    #define TSO1_SVQ3_TX_EMPTY_MASK                                 0x00000800
    #define TSO1_SVQ3_TX_EMPTY_SHIFT                                11
    #define TSO1_SVQ4_TX_WATER_LEVEL_MASK                           0x00003000
    #define TSO1_SVQ4_TX_WATER_LEVEL_SHIFT                          12
    #define TSO1_SVQ4_TX_FULL_MASK                                  0x00004000
    #define TSO1_SVQ4_TX_FULL_SHIFT                                 14
    #define TSO1_SVQ4_TX_EMPTY_MASK                                 0x00008000
    #define TSO1_SVQ4_TX_EMPTY_SHIFT                                15
    #define TSO1_SVQ5_TX_WATER_LEVEL_MASK                           0x00030000
    #define TSO1_SVQ5_TX_WATER_LEVEL_SHIFT                          16
    #define TSO1_SVQ5_TX_FULL_MASK                                  0x00040000
    #define TSO1_SVQ5_TX_FULL_SHIFT                                 18
    #define TSO1_SVQ5_TX_EMPTY_MASK                                 0x00080000
    #define TSO1_SVQ5_TX_EMPTY_SHIFT                                19
    #define TSO1_SVQ6_TX_WATER_LEVEL_MASK                           0x00300000
    #define TSO1_SVQ6_TX_WATER_LEVEL_SHIFT                          20
    #define TSO1_SVQ6_TX_FULL_MASK                                  0x00400000
    #define TSO1_SVQ6_TX_FULL_SHIFT                                 22
    #define TSO1_SVQ6_TX_EMPTY_MASK                                 0x00800000
    #define TSO1_SVQ6_TX_EMPTY_SHIFT                                23

    REG32_TSO                             DELTA;                        //38~39

    REG16_TSO                             DELTA_CONFIG;                 //3a
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_MASK                      0x0007
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_SHIFT                     0
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_1                         1
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_2                         2
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_3                         3
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_4                         4
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_5                         5
    #define TSO1_DELTA_CONFIG_SEL_CHANNEL_6                         6
    #define TSO1_DELTA_CONFIG_DELTA_CLR                             0x0008
    #define TSO1_DELTA_CONFIG_MAX_ID_MASK                           0x0070
    #define TSO1_DELTA_CONFIG_MAX_ID_SHIFT                          8

    REG16_TSO                             REG_TSO1_CFG3B_52[24];        //3b~52
    REG16_TSO                             REG_TSO_MIU_SEL_1;            //53
    #define REG_MIU_SEL_SVQTX1_MASK                                 0x0003
    #define REG_MIU_SEL_SVQTX1_SHIFT                                0
    #define REG_MIU_SEL_SVQTX2_MASK                                 0x000C
    #define REG_MIU_SEL_SVQTX2_SHIFT                                2
    #define REG_MIU_SEL_SVQTX3_MASK                                 0x0030
    #define REG_MIU_SEL_SVQTX3_SHIFT                                4
    #define REG_MIU_SEL_SVQTX4_MASK                                 0x00C0
    #define REG_MIU_SEL_SVQTX4_SHIFT                                6
    #define REG_MIU_SEL_SVQTX5_MASK                                 0x0300
    #define REG_MIU_SEL_SVQTX5_SHIFT                                8
    #define REG_MIU_SEL_SVQTX6_MASK                                 0x0C00
    #define REG_MIU_SEL_SVQTX6_SHIFT                                10
    #define REG_MIU_SEL_SVQRX_MASK                                  0x300
    #define REG_MIU_SEL_SVQRX_SHIFT                                 12
    #define REG_MIU_SEL_CH5FILEIN_MASK                              0xC000
    #define REG_MIU_SEL_CH5FILEIN_SHIFT                             14
    REG16_TSO                             REG_TSO_MIU_SEL_2;            //54
    #define REG_MIU_SEL_CH6FILEIN_MASK                              0x0003
    #define REG_MIU_SEL_CH6FILEIN_SHIFT                             0
    REG16_TSO                             REG_TSO1_CFG55_66[18];        //55~66
    REG16_TSO                             REG_TSO_MIU_ABT_CONFIG_1;
    #define REG_MIU_ABT_CONFIG_1_CHECK2MI_RDY                       0x0040 //U02
    #define REG_MIU_ABT_CONFIG_1_MIU_FIXED_LAST_WD_EN_DONE_Z        0x0080 //U02
} REG_Ctrl_TSO1;

#endif // _TSO_REG_H_
