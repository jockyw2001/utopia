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
#define TSO_ENGINE_NUM                          (1)
#define TSO_PIDFLT_NUM                          (64)
#define TSO_REP_PIDFLT_NUM                      (16)
#define TSO_TSIF_NUM                            (2)
#define TSO_FILE_IF_NUM                         (1)
#define TSO_SVQ_UNIT_SIZE                       (208)

#define TSO_PIDFLT_NUM_ALL                      TSO_PIDFLT_NUM

#define TSO_PID_NULL                            0x1FFF

#define TSO_MIU_BUS                             4

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

#define TSO_IN_MUX_TS0                          0x0
#define TSO_IN_MUX_TS1                          0x1
#define TSO_IN_MUX_TS2                          0x2
#define TSO_IN_MUX_TSDEMOD                      0x7
#define TSO_IN_MUX_MEM                          0x8

#define TSO_CLKIN_TS0                           0x00
#define TSO_CLKIN_TS1                           0x04
#define TSO_CLKIN_TS2                           0x08
#define TSO_CLKIN_DMD                           0x1C

//--------------- u16ClkOutDivSrcSel   -------------
#define TSO_OUT_DIV_DMPLLDIV5                   0x0000      //  dmplldiv5  = 844/5 = 172.8MHz
#define TSO_OUT_DIV_DMPLLDIV3                   0x0001      //  dmplldiv3  = 844/3 = 288MHz

// Note:
// DVB-T    dmplldiv5 / 2 (11+1) = 7.2 MHz
// DVB-C    dmplldiv5 / 2 (11+1) = 7.2 MHz
// ATSC      dmplldiv5 / 2 (11+1) = 7.2 MHz
// ISDB-T   dmplldiv_3 / 2 (17+1) = 8 MHz

//---------------- u16ClkOutSel ---------------
#define TSO_OUT_DIV2                            0x0000      // Must also select div src and set div num
#define TSO_OUT_62MHz                           0x0400
#define TSO_OUT_54MHz                           0x0800
#define TSO_OUT_PTSO_OUT                        0x0C00 //live-in
#define TSO_OUT_PTSO_OUT_DIV8                   0x1000 //live-in
#define TSO_OUT_27MHz                           0x1400
#define TSO_OUT_DEMOD_P                         0x1C00 //live-in

//--------------- u16PreTsoOutSel   -------------
#define TSO_PRE_OUT_TS0IN                       0x0000
#define TSO_PRE_OUT_TS1IN                       0x0001
#define TSO_PRE_OUT_TS2IN                       0x0002
#define TSO_PRE_OUT_DEMDOIN                     0x0003

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define REG_PIDFLT_BASE                     (0x00210000 << 1)                   // Fit the size of REG32

#define REG_CTRL_BASE_TSO                   (0x27400)                            // 0x113A
#define REG_CTRL_BASE_TSO1                  (0x47A00)                            // 0x123D


typedef struct _REG32
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32;

typedef struct _REG16
{
    volatile MS_U16                data;
    volatile MS_U16                _resv;
} REG16;

typedef REG32                           REG_PidFlt;

// PID
#define TSO_PIDFLT_PID_MASK         0x00001FFF
#define TSO_PIDFLT_PID_SHFT         0

// Channel source
#define TSO_PIDFLT_IN_SHIFT         13
#define TSO_PIDFLT_IN_MASK          0x0000E000
#define TSO_PIDFLT_IN_CH0           0x00002000
#define TSO_PIDFLT_IN_CH5           0x0000A000
#define TSO_PIDFLT_IN_CH6           0x0000C000

typedef struct _REG_Pid
{                                                                       // Index(word)  CPU(byte)       Default
    REG_PidFlt                      Flt[TSO_PIDFLT_NUM];
} REG_Pid;


typedef struct _REG_Ctrl_TSO
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
                                                                       // Index(word)  CPU(byte)     MIPS(0x13A00/2+index)*4

    REG16                             SW_RSTZ;                         // 0xbf827400   0x00
    #define TSO_SW_RSTZ_DISABLE                     0x0001
    #define TSO_SW_RSTZ_CLK_STAMP                   0x0002
    #define TSO_SW_RSTZ_WB1                         0x0200
    #define TSO_SW_RSTZ_WB_DMA1                     0x0400
    #define TSO_SW_RSTZ_CMDQ                        0x1000
    #define TSO_SW_RSTZ_WB                          0x2000
    #define TSO_SW_RSTZ_WB_DMA                      0x4000
    #define TSO_SW_RSTZ_TS_FIN                      0x8000
    #define TSO_SW_RSTZ_ALL                         0x00FE

    REG16                             SW_RSTZ1;                         // 0xbf827404   0x01
    #define TSO_SW_RSTZ1_CH_IF1                     0x0001
    #define TSO_SW_RSTZ1_CH_IF5                     0x0010
    #define TSO_SW_RSTZ1_ALL                        0x0031

    REG32                            _xbf827408_740c;                   // 0xbf827408~0xbf82740c  0x02~03

    REG16                             TSO_CH0_IF1_CFG0;                 // 0xbf827410   0x04
    #define TSO_PKT_SIZE_CHK_LIVE_MASK              0x00FF
    #define TSO_PIDFLT_PKT_SIZE_MASK                0xFF00
    #define TSO_PIDFLT_PKT_SIZE_SHIFT               8

    REG16                             TSO_CH0_IF1_CFG1;                 // 0xbf827414   0x05
    REG16                             TSO_CH0_IF1_CFG2;                 // 0xbf827418   0x06
    #define TSO_CHCFG_P_SEL                         0x0001
    #define TSO_CHCFG_EXT_SYNC_SEL                  0x0002
    #define TSO_CHCFG_TS_SIN_C0                     0x0004
    #define TSO_CHCFG_TS_SIN_C1                     0x0008
    #define TSO_CHCFG_PIDFLT_REC_ALL                0x0010              // bypass all packets
    #define TSO_CHCFG_PIDFLT_REC_NULL               0x0020              // bypass NULL packets
    #define TSO_CHCFG_PIDFLT_OVF_INT_EN             0x0040
    #define TSO_CHCFG_PIDFLT_OVF_CLR                0x0080
    #define TSO_CHCFG_FORCE_SYNC_BYTE               0x0100
    #define TSO_CHCFG_SKIP_TEI_PKT                  0x0200
    #define TSO_CHCFG_DIS_LOCKED_PKT_CNT            0x0400
    #define TSO_CHCFG_CLR_LOCKED_PKT_CNT            0x0800
    #define TSO_CHCFG_TRC_CLK_LD_DIS                0x1000
    #define TSO_CHCFG_TRC_CLK_CLR                   0x2000
    REG16                             TSO_CH0_IF1_CFG3;                 // 0xbf82741c   0x07

    REG16                             _xbf827420_744c[12];              // 0xbf827420~0xbf82744c  0x08~13

    REG16                             TSO_CH0_IF5_CFG0;                 // 0xbf827450   0x14
    REG16                             TSO_CH0_IF5_CFG1;                 // 0xbf827454   0x15
    REG16                             TSO_CH0_IF5_CFG2;                 // 0xbf827458   0x16
    REG16                             TSO_CH0_IF5_CFG3;                 // 0xbf82745c   0x17

    REG16                             _xbf827460_746c[4];              // 0xbf827460~0xbf82746c   0x18~0x1b

    REG16                             TSO_CFG0;                         // 0xbf827470   0x1c  //s2p_Cfg
    REG16                             TSO_CFG1;                         // 0xbf827474   0x1d
    #define TSO_CFG1_TSO_OUT_EN                     0x0001
    #define TSO_CFG1_TSO_TSIF1_EN                   0x0002
    #define TSO_CFG1_TSO_TSIF5_EN                   0x0020
    #define TSO_CFG1_CLK_TRC_SEL_MASK               0x0E00
    #define TSO_CFG1_PKT_LOCK_CLR                   0x2000
    #define TSO_CFG1_NULL_EN                        0x4000
    #define TSO_CFG1_PKT_PARAM_LD                   0x8000

    REG16                             TSO_CFG2;                         // 0xbf827478   0x1e
    #define TSO_CFG2_VALID_BYTECNT_MASK             0x00FF
    #define TSO_CFG2_INVALID_BYTECNT_MASK           0xFF00
    #define TSO_CFG2_VALID_BYTECNT_SHIFT            0
    #define TSO_CFG2_INVALID_BYTECNT_SHIFT          8

    REG16                             TSO_CFG3;                         // 0xbf82747c   0x1f

    REG32                             REP_PidFlt[16];                   // 0xbf827480~0xbf8274F8   0x20~0x3e
    #define REP_PIDFLT_ORG_PID_MASK                 0x00001FFF
    #define REP_PIDFLT_SRC_MASK                     0x0000E000
    #define REP_PIDFLT_SRC_SHIFT                    13
        #define REP_PIDFLT_SRC_CH1                  0x00002000
        #define REP_PIDFLT_SRC_CH5                  0x0000A000
        #define REP_PIDFLT_SRC_CH6                  0x0000C000
    #define REP_PIDFLT_NEW_PID_MASK                 0x01FFF000
    #define REP_PIDFLT_NEW_PID_SHIFT                16
    #define REP_PIDFLT_REPLACE_EN                   0x80000000

    REG16                             TSO_CLR_BYTE_CNT;                // 0xbf827500   0x40
    #define TSO_CLR_BYTE_CNT_1                      0x0000
    #define TSO_CLR_BYTE_CNT_5                      0x0004

    REG32                             TSO_SYSTIMESTAMP;                // 0xbf827504~0xbf827508   0x41~42

    REG16                             TSO_CFG4;                        // 0xbf82750c   0x43
    #define TSO_CFG4_LOCK_RET_SYS_TIMESTAMP         0x0001
    #define TSO_CFG4_ENABLE_SYS_TIMESTAMP           0x0002
    #define TSO_CFG4_SET_SYS_TIMESTAMP              0x0004
    #define TSO_CFG4_SET_TIMESTAMP_BASE_MASK        0x0008
        #define TSO_CFG4_SET_TIMESTAMP_90K          0x0000
        #define TSO_CFG4_SET_TIMESTAMP_27M          0x0008
    #define TSO_CFG4_PIDTABLE_SRAM_SD_EN            0x0010
    #define TSO_CFG4_NULL_PKT_ID_MASK               0xF000

    REG16                             TSO_CFG5;                        // 0xbf82750c   0x44
    #define TSO_CFG5_WIRE_MODE_EN_1                 0x0001
    #define TSO_CFG5_WIRE_MODE_EN_5                 0x0010

    REG32                             TSO_INDR_ADDR;                   // 0xbf82750c~0xbf827510   0x45~0x46
    REG32                             TSO_INDR_WDATA;                  // 0xbf827514~0xbf827518   0x47~0x48
    REG16                             TSO_INDR_RDATA;                  // 0xbf82751c   0x49
    REG16                             TSO_INDR_CTRL ;                  // 0xbf827520   0x4a
    #define TSO_INDIR_W_ENABLE                      0x0001
    #define TSO_INDIR_R_ENABLE                      0x0002

    REG16                             TSO_STATUS;                      // 0xbf827524   0x4b

    REG16                             TSO_FI_TIMER[1];                 // 0xbf827528  0x4c

    REG16                            _xbf82752c;                       // 0xbf82752c  0x4d

    REG16                             TSO_STATUS1;                     // 0xbf827530   0x4e
    #define TSO_PIDFLT_OVF_EVER_TSIF0               0x0001
    #define TSO_PIDFLT_OVF_EVER_TSIF5               0x0010

    REG16                            _xbf827534_7568[12];              // 0xbf827534~0xbf827568  0x4f~0x5a

    REG16                             TSO_TRACE_HIGH;                  // 0xbf82756c   0x5b
    REG16                             TSO_TRACE_LOW;                   // 0xbf827570   0x5c
    REG16                             TSO_TRACE_1t;                    // 0xbf827574   0x5d

    REG16                             TSO_BLOCK_SIZE_DB;               // 0xbf827578   0x5e
    REG16                             TSO_BLOCK_OPT_DB;                // 0xbf82757c   0x5f

    REG32                             TSO_Filein_raddr;                // 0xbf827580~0xbf827584      0x60-0x61
    REG32                             TSO_Filein_rNum;                 // 0xbf827588~0xbf82758c      0x62-0x63
    REG16                             TSO_Filein_Ctrl;                 // 0xbf827590   0x64
    #define TSO_FILEIN_CTRL_MASK                    0x0003
    #define TSO_FILEIN_RSTART                       0x0001
    #define TSO_FILEIN_ABORT                        0x0002
    #define TSO_FILEIN_MOBF_IDX_MASK                0x1F00
    #define TSO_FILEIN_MOBF_IDX_SHIFT               8
    #define TSO_FILEIN_RIU_TSO_NS                   0x2000

    REG16                            _xbf827594_75a4[5];               // 0xbf827594~0xbf8275a4  0x65-0x69

    REG16                             TSO_PKT_CNT_SEL;                 // 0xbf8275a8   0x6a
    #define TSO_PKT_CNT_SEL_MASK                    0x000F
    #define TSO_PKT_CNT_LOCKED_CNT_MASK             0x00F0
    #define TSO_PKT_CNT_DBG_MASK                    0xFF00

    REG16                             TSO_PKT_CHKSIZE_FI;              // 0xbf8275ac   0x6b
    #define TSO_PKT_CHKSIZE_FI_MASK                 0x00FF

    REG32                             TSO_LPCR2[1];                    // 0xbf8275b0~ 0xbf8275b4  0x6c~0x6d

    REG32                             _xbf8275b8_75bc;                 // 0xbf8275b8~ 0xbf8275bc  0x6e~0x6f

    REG32                             TSO_TIMESTAMP[1];                // 0xbf8275c0~ 0xbf8275c4  0x70~0x71

    REG32                             _xbf8275c8_75cc;                 // 0xbf8275c8~ 0xbf8275cc  0x72~0x73

    REG32                             TSO_TSO2MI_RADDR[1];             // 0xbf8275d0~ 0xbf8275d4  0x74~0x75

    REG32                             _xbf8275d8_75dc;                 // 0xbf8275d8~ 0xbf8275dc  0x76~0x77

    REG16                             TSO_CMDQ_STATUS;                 // 0xbf8275e0   0x78
    #define TSO_CMDQ_SIZE                           8
    #define TSO_CMDQ_STS_WCNT_MASK                  0x000F
    #define TSO_CMDQ_STS_WLEVEL_MASK                0x0030
    #define TSO_CMDQ_STS_FIFO_FULL                  0x0040
    #define TSO_CMDQ_STS_FIFO_EMPTY                 0x0080

    REG16                             TSO_FILE_CFG[1];                 // 0xbf8275e4  0x79
    #define TSO_FICFG_TSO2MI_RPRI                   0x0001
    #define TSO_FICFG_MEM_TSDATA_ENDIAN             0x0002
    #define TSO_FICFG_MEM_TS_W_ORDER                0x0004
    #define TSO_FICFG_LPCR2_WLD                     0x0008
    #define TSO_FICFG_LPCR2_LD                      0x0010
    #define TSO_FICFG_DIS_MIU_RQ                    0x0020
    #define TSO_FICFG_RADDR_READ                    0x0040
    #define TSO_FICFG_TS_DATAPORT_SEL               0x0080
    #define TSO_FICFG_TSO_FILEIN                    0x0100
    #define TSO_FICFG_TIMER_ENABLE                  0x0200
    #define TSO_FICFG_PKT192_BLK_DISABLE            0x0400
    #define TSO_FICFG_PKT192_ENABLE                 0x0800
    #define TSO_FICFG_FILE_SEGMENT                  0x1000
    #define TSO_FICFG_CLK_TIMESTAMP_SEL_MASK        0x2000
    #define TSO_FICFG_CLK_TIMESTAMP_27M             0x2000
    #define TSO_FICFG_CLK_TIMESTAMP_90K             0x0000

    REG16                             _xbf8275e8;                       // 0xbf8275e8   x7a

    REG16                             TSO_Interrupt;                    // 0xbf8275ec   0x7b
    #define TSO_INT_ENABLE_MASK                     0x00FF
    #define TSO_INT_STATUS_MASK                     0xFF00
    #define TSO_INT_DMA_DONE                        0x0001
    #define TSO_INT_DMA_DONE1                       0x0002
    #define TSO_INT_TRCCLK_UPDATE                   0x0004

    REG16                             TSO_Interrupt1;                   // 0xbf8275f0   0x7c
    #define TSO_INT1_ENABLE_MASK                    0x00FF
    #define TSO_INT1_STATUS_MASK                    0xFF00
    #define TSO_INT1_PIDFLT1_OVF                    0x0001
    #define TSO_INT1_PIDFLT5_OVF                    0x0010

    REG32                             TSO_DBG;                         // 0xbf8275f4~0xbf8275f8   0x7d~0x7e
    REG16                             TSO_DBG_SEL;                     // 0xbf8275fc   0x7f

} REG_Ctrl_TSO;

typedef struct _REG_Ctrl_TSO1
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
                                                                       // Index(word)  CPU(byte)     MIPS(0x13A00/2+index)*4

    REG16                             TSO_PRE_HEADER1_CFG0;            // 0xbf847A00   0x00
    #define TSO_PRE_HD1_CFG0_LOCAL_STRID_MASK       0x00FF

    REG16                             TSO_PRE_HEADER1_CFG1;            // 0xbf847A04   0x01
    REG16                             TSO_PRE_HEADER1_CFG2;            // 0xbf847A08   0x02
    REG16                             TSO_PRE_HEADER1_CFG3;            // 0xbf847A0c   0x03

    REG16                            _xbf827a10_7a3c[12];              // 0xbf847A10~0xbf847A3c  0x04~0x0f

    REG16                            TSO_PRE_HEADER5_CFG0;             // 0xbf847A40   0x10
    REG16                            TSO_PRE_HEADER5_CFG1;             // 0xbf847A44   0x11
    REG16                            TSO_PRE_HEADER5_CFG2;             // 0xbf847A48   0x12
    REG16                            TSO_PRE_HEADER5_CFG3;             // 0xbf847A4c   0x13

    REG16                            _xbf827a50_7a5c[4];               // 0xbf847A50~0xbf847A5c  0x14~0x17

    REG32                            TSO_SVQ1_BASE;                    // 0xbf847A50~0xbf847A54   0x18~0x19
    REG16                            TSO_SVQ1_SIZE;                    // 0xbf847A58   0x1a  //unit:200byte/pkt
    REG16                            TSO_SVQ1_TX_CFG;                  // 0xbf847A5c   0x1b
    #define TSO_SVQ_TX_CFG_WR_THRESHOLD_MASK        0x000F
    #define TSO_SVQ_TX_CFG_PRI_THRESHOLD_MASK       0x00F0
    #define TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_MASK      0x0F00
    #define TSO_SVQ_TX_CFG_FORCE_FIRE_CNT_SHIFT     8UL
    #define TSO_SVQ_TX_CFG_TX_RESET                 0x1000
    #define TSO_SVQ_TX_CFG_OVF_INT_EN               0x2000
    #define TSO_SVQ_TX_CFG_OVF_CLR                  0x4000
    #define TSO_SVQ_TX_CFG_SVQ_EN                   0x8000

    REG16                            _xbf827a60_7a9c[12];              // 0xbf847A60~0xbf847A9c  0x1c~0x27

    REG32                            TSO_SVQ5_BASE;                    // 0xbf847Aa0~0xbf847Aa4   0x28~0x29
    REG16                            TSO_SVQ5_SIZE;                    // 0xbf847Aa8   0x2a  //unit:200byte/pkt
    REG16                            TSO_SVQ5_TX_CFG;                  // 0xbf847Aac   0x2b

    REG16                            _xbf827ab0_7abc[4];              // 0xbf847Ab0~0xbfbc7Abc  0x2c~0x2f

    REG16                            TSO_SVQ_RX_CFG;                   // 0xbf847Ac0   0x30
    #define TSO_SVQ_RX_CFG_MODE_MASK                0x0003
    #define TSO_SVQ_RX_CFG_MODE_OPENCBL             0x0000
    #define TSO_SVQ_RX_CFG_MODE_CIPL                0x0001
    #define TSO_SVQ_RX_CFG_MODE_192PKT              0x0002
    #define TSO_SVQ_RX_CFG_RD_THRESHOLD_MASK        0x001C
    #define TSO_SVQ_RX_CFG_ARBMODE_MASK             0x0060
    #define TSO_SVQ_RX_CFG_ARBMODE_RUNROBIN         0x0000
    #define TSO_SVQ_RX_CFG_ARBMODE_FIXPRI           0x0020
    #define TSO_SVQ_RX_CFG_ARBMODE_DYMPRI           0x0040
    #define TSO_SVQ_RX_CFG_DRAM_SD_ENABLE           0x0080
    #define TSO_SVQ_RX_CFG_SVQ_FORCE_RESET          0x0100
    #define TSO_SVQ_RX_CFG_SVQ_MIU_NS               0x0200
    #define TSO_SVQ_RX_CFG_SVQ_MOBF_IDX_MASK        0x7C00
    #define TSO_SVQ_RX_CFG_SVQ_MOBF_IDX_SHIFT       10
    #define TSO_SVQ_RX_CFG_SVQ_DYN_PRI              0x8000

    REG16                            TSO_SVQ_RX_PRI[2];                // 0xbf847Ac4~0xbf847Acc   0x31~0x32
    #define TSO_SVQ_RX_NUM                          6
    #define TSO_SVQ_RX_PRI_MASK                     0xFF
    #define TSO_SVQ_RX_PRI_SHIFT                    8

    REG16                            _xbf827acc;                        // 0xbf847Acc  0x33

    REG32                            TSO_SVQ_STATUS;                   // 0xbf847Ad0~0xbf847Ad4   0x34~0x35
    #define TSO_SVQ_STS_MASK                        0x000F
    #define TSO_SVQ1_STS_SHIFT                      0
    #define TSO_SVQ5_STS_SHIFT                      16
    #define TSO_SVQ_STS_EVER_FULL                   0x0001
    #define TSO_SVQ_STS_EVER_OVF                    0x0002
    #define TSO_SVQ_STS_EMPTY                       0x0004
    #define TSO_SVQ_STS_BUSY                        0x0008

    REG32                            TSO_SVQ_STATUS2;                  // 0xbf847Ad8~0xbf847Adc   0x36~0x37
    #define TSO_SVQ_STS2_MASK                       0x000F
    #define TSO_SVQ1_STS2_SHIFT                     0
    #define TSO_SVQ5_STS2_SHIFT                     16
    #define TSO_SVQ_STS2_TXFIFO_WLEVEL_MASK         0x000C
    #define TSO_SVQ_STS2_TXFIFO_FULL                0x0002
    #define TSO_SVQ_STS2_TXFIFO_EMPTY               0x0001

    REG32                            TSO_DELTA;                       // 0xbf847Ae0~0xbf847Ae4   0x38~0x39

    REG16                            TSO_DELTA_CFG;                   // 0xbf847Ae8   0x3a
    #define TSO_DELTA_CFG_SEL_CH_MASK               0x0007
    #define TSO_DELTA_CFG_DELTA_CLR                 0x0008
    #define TSO_DELTA_CFG_MAX_ID_MASK               0x0700
    #define TSO_DELTA_CFG_MAX_ID_SHIFT              8

} REG_Ctrl_TSO1;


#endif // _TSO_REG_H_

