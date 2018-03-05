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
// Copyright (c) 2010-2012 MStar Semiconductor, Inc.
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
//  File name: mmfilein.h
//  Description: Multimedia File In (MMFILEIN) Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MMFILEIN_REG_H_
#define _MMFILEIN_REG_H_

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
#define MMFI_ENGINE_NUM                     (2)

#define MMFI_PIDFLT0_NUM                    (6)
#define MMFI_PIDFLT1_NUM                    (6)

#define MMFI_PIDFLT_NUM_ALL                 (MMFI_PIDFLT0_NUM+MMFI_PIDFLT1_NUM)

#define MMFI_PID_NULL                       0x1FFF

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define REG_CTRL_BASE_MMFI0                 (0x27E00UL) // Bank: 0x113F , Offset: 0x00
#define REG_CTRL_BASE_MMFI1                 (0x27FB8UL) // Bank: 0x113F , Offset: 0x6E

typedef struct _REG32_MM
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32_MM;


typedef struct _REG16_MM
{
    volatile MS_U16                data;
    volatile MS_U16                _resv;
} REG16_MM;

typedef struct _REG_Ctrl_MMFI
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
                                                                                // CPU(byte)
    REG32_MM                                PidFlt[MMFI_PIDFLT0_NUM];           // 0x00~0x0B
        #define MMFI_PIDFLT_PID_MASK                    0x00001FFF
        #define MMFI_PIDFLT_EN_MASK                     0x07FFE000
        #define MMFI_PIDFLT_AFIFO_EN                    0x00002000
        #define MMFI_PIDFLT_AFIFOB_EN                   0x00004000
        #define MMFI_PIDFLT_AFIFOC_EN                   0x00008000
        #define MMFI_PIDFLT_AFIFOD_EN                   0x00010000
        #define MMFI_PIDFLT_AFIFOE_EN                   0x00020000
        #define MMFI_PIDFLT_AFIFOF_EN                   0x00040000

        #define MMFI_PIDFLT_VFIFO_EN                    0x00080000
        #define MMFI_PIDFLT_V3DFIFO_EN                  0x00100000
        #define MMFI_PIDFLT_VFIFO3_EN                   0x00200000
        #define MMFI_PIDFLT_VFIFO4_EN                   0x00400000
        #define MMFI_PIDFLT_VFIFO5_EN                   0x00800000
        #define MMFI_PIDFLT_VFIFO6_EN                   0x01000000
        #define MMFI_PIDFLT_VFIFO7_EN                   0x02000000
        #define MMFI_PIDFLT_VFIFO8_EN                   0x04000000

    REG32_MM                                Cfg;                                // 0x0C~0x0D
//------------------------- no use , just for compatibility -------------------------//
        #define MMFI_CFG_ALT_TS_SIZE                    0x00000000
        #define MMFI_CFG_DUP_PKT_SKIP                   0x00000000
        #define MMFI_CFG_TEI_SKIP                       0x00000000
        #define MMFI_CFG_APID_BYPASS                    0x00000000
        #define MMFI_CFG_APIDB_BYPASS                   0x00000000
        #define MMFI_CFG_VPID_BYPASS                    0x00000000
        #define MMFI_CFG_VPID3D_BYPASS                  0x00000000
//-----------------------------------------------------------------------------------//
        #define MMFI_CFG_LPCR2_LD                       0x00000001
        #define MMFI_CFG_LPCR2_WLD                      0x00000002
        #define MMFI_CFG_CLR_PIDFLT_BYTE_CNT            0x00000008
        #define MMFI_CFG_INIT_TIMESTAMP_RSTART_EN       0x00000010
        #define MMFI_CFG_FIXED_TIMESTAMP_RING_BANK_EN   0x00000020
        #define MMFI_CFG_FIXED_LPCR_RING_BANK_EN        0x00000040
        #define MMFI_CFG_FIND_LOSS_SYNC_BYTE_RVU        0x00000080

        #define MMFI_CFG_MMFI_ABORT                     0x00010000
        #define MMFI_CFG_DIS_MIU_RQ                     0x00020000
        #define MMFI_CFG_RADDR_READ                     0x00040000
        #define MMFI_CFG_BYTETIMER_EN                   0x00080000
        #define MMFI_CFG_2MI_RPRIORITY                  0x00800000
        #define MMFI_CFG_PS_AUD_EN                      0x01000000
        #define MMFI_CFG_PS_AUDB_EN                     0x02000000
        #define MMFI_CFG_PS_VD_EN                       0x04000000
        #define MMFI_CFG_PS_V3D_EN                      0x08000000
        #define MMFI_CFG_MEM_TS_ORDER                   0x10000000
        #define MMFI_CFG_MEM_TS_DATA_ENDIAN             0x20000000
        #define MMFI_CFG_PKT192_EN                      0x40000000
        #define MMFI_CFG_PKT192_BLK_DISABLE             0x80000000
        #define MMFI_CFG_FILEIN_MODE_MASK               (MMFI_CFG_PS_AUD_EN|MMFI_CFG_PS_AUDB_EN|MMFI_CFG_PS_VD_EN|MMFI_CFG_PS_V3D_EN)

    REG16_MM                                CFG2;                               // 0x0E
//------------------------- no use , just for compatibility -------------------------//
        #define MMFI_CFG2_MMFI_APIDC_BYPASS             0x00000000
        #define MMFI_CFG2_MMFI_APIDD_BYPASS             0x00000000
//-----------------------------------------------------------------------------------//
        #define MMFI_CFG2_C27M_EN                       0x00000001
        #define MMFI_CFG2_MMFI_PS_VD3_EN                0x00000002
        #define MMFI_CFG2_MMFI_PS_VD4_EN                0x00000004
        #define MMFI_CFG2_MMFI_PS_VD5_EN                0x00000008
        #define MMFI_CFG2_MMFI_PS_VD6_EN                0x00000010
        #define MMFI_CFG2_MMFI_PS_VD7_EN                0x00000020
        #define MMFI_CFG2_MMFI_PS_VD8_EN                0x00000040
        #define MMFI_CFG2_MMFI_PS_AUDC_EN               0x00000080
        #define MMFI_CFG2_MMFI_PS_AUDD_EN               0x00000100
        #define MMFI_CFG2_FILEIN_PAUSE                  0x00000200
        #define MMFI_CFG2_WB_FSM_RESET                  0x00000400
        #define MMFI_CFG2_MMFI_PS_AUDE_EN               0x00000800
        #define MMFI_CFG2_MMFI_PS_AUDF_EN               0x00001000
        #define MMFI_CFG2_FILEIN_MODE_MASK              ( MMFI_CFG2_MMFI_PS_AUDC_EN | MMFI_CFG2_MMFI_PS_AUDD_EN \
                                                        | MMFI_CFG2_MMFI_PS_AUDE_EN | MMFI_CFG2_MMFI_PS_AUDF_EN \
                                                        | MMFI_CFG2_MMFI_PS_VD3_EN | MMFI_CFG2_MMFI_PS_VD4_EN   \
                                                        | MMFI_CFG2_MMFI_PS_VD5_EN | MMFI_CFG2_MMFI_PS_VD6_EN   \
                                                        | MMFI_CFG2_MMFI_PS_VD7_EN | MMFI_CFG2_MMFI_PS_VD8_EN)

    REG16_MM                                CFG3;                               // 0x0F (reserved)
        #define MMFI_CFG3_MMFI_PS_VD3_EN                0x0000  // not support
        #define MMFI_CFG3_MMFI_PS_VD4_EN                0x0000  // not support
        #define MMFI_CFG3_FILEIN_MODE_MASK              (MMFI_CFG3_MMFI_PS_VD3_EN | MMFI_CFG3_MMFI_PS_VD4_EN)

    REG32_MM                                FileIn_RAddr;                       // 0x10~0x11
    REG32_MM                                FileIn_RNum;                        // 0x12~0x13
    REG16_MM                                FileIn_Ctrl;                        // 0x14
        #define MMFI_FILEIN_CTRL_START                  0x0001
        #define MMFI_FILEIN_CTRL_DONE                   0x0002
        #define MMFI_FILEIN_CTRL_INIT_TRUST             0x0004
        #define MMFI_FILEIN_CTRL_ABORT                  0x0010
        #define MMFI_FILEIN_CTRL_MOBF_EN                0x0020                  // Not used (for compability)
        #define MMFI_FILEIN_CTRL_MASK                   0x0013

    REG16_MM                                CmdQSts;                            // 0x15
        #define MMFI_CMDQ_SIZE                          8
        #define MMFI_CMDQSTS_WRCNT_MASK                 0x000F
        #define MMFI_CMDQSTS_FIFO_FULL                  0x0040
        #define MMFI_CMDQSTS_FIFO_EMPTY                 0x0080
        #define MMFI_CMDQSTS_FIFO_WRLEVEL_MASK          0x0300
        #define MMFI_CMDQSTS_FIFO_WRLEVEL_SHIFT         8

    REG16_MM                                FileIn_Timer;                       // 0x16
        #define MMFI_FILEIN_TIMER_MASK                  0x00FF
        #define MMFI_FILEIN_TIMER_SHIFT                 0

    REG32_MM                                TsHeader;                           // 0x17~0x18
        #define MMFI_HD_CCNT_MASK                       0x0000000F
        #define MMFI_HD_AF_MASK                         0x00000030
        #define MMFI_HD_AF_SHIFT                        4
        #define MMFI_HD_SCRAMBLE_MASK                   0x000000C0
        #define MMFI_HD_SCRAMBLE_SHIFT                  6
        #define MMFI_HD_PID                             0x001FFF00
        #define MMFI_HD_PID_SHIFT                       8
        #define MMFI_HD_TS_PRIORITY_MASK                0x00200000
        #define MMFI_HD_TS_PRIORITY_SHIFT               21
        #define MMFI_HD_PAYLOAD_START_FLG_MASK          0x00400000
        #define MMFI_HD_PAYLOAD_START_FLG_SHIFT         22
        #define MMFI_HD_ERR_FLG_MASK                    0x00800000
        #define MMFI_HD_ERR_FLG_SHIFT                   23

    REG32_MM                                LPcr2_Buf;                          // 0x19~0x1A

    REG32_MM                                TimeStamp_FIn;                      // 0x1B~0x1C

    REG16_MM                                PktChkSize;                         // 0x1D
        #define MMFI_PKTCHK_SIZE_MASK                   0x00FF
        #define MMFI_SYNC_BYTE_MASK                     0xFF00
        #define MMFI_SYNC_BYTE_SHIFT                    8

    REG16_MM                                MOBFKey;                            // 0x1E
        #define MMFI_MOBFKEY_MASK                       0x001F

    REG32_MM                                RAddr;                              // 0x1F~0x20
        #define MMFI_TSP2MI_RADDR_MASK                  0x0FFFFFFF

    REG32_MM                                InitTimeStamp;                      // 0x21~0x22

    REG16_MM                                SyncBytePrivilege;                  // 0x23
        #define MMFI_SYNC_BYTE_PRIVILEGE_MASK           0x00FF
        #define MMFI_SYNC_BYTE_PRIVILEGE_SHIFT          0
        #define MMFI_REPLACE_SYNC_BYTE_PRIVILEGE_MASK   0xFF00
        #define MMFI_REPLACE_SYNC_BYTE_PRIVILEGE_SHIFT  8

    REG16_MM                                ChkPrivilegeSyncByteEn;             // 0x24

    REG16_MM                                RVU;                                // 0x25
        #define MMFI_RVU_PSI_EN                         0x0001
        #define MMFI_RVU_TEI_EN                         0x0002
        #define MMFI_RVU_ERR_CLR                        0x0004
        #define MMFI_RVU_EN                             0x0008
        #define MMFI_RVU_TIMESTAMP_EN                   0x0010
        #define MMFI_RVU_HD_0000_TO_SECTION             0x0020
        #define MMFI_RVU_HD_1100_TO_SECTION             0x0040
        #define MMFI_RVU_HD_10x0_11x0_TO_SECTION        0x0080
        #define MMFI_RVU_PAYLOAD_128_MODE               0x0100
        #define MMFI_RVU_SUPPORT_NB                     0x0200
        #define MMFI_RVU_FIND_LOSS_SYNC_BYTE            0x0400
        #define MMFI_RVU_PKT130_EVER_ERR_FLAG           0x8000

    REG16_MM                                AtsCfg;                             // 0x26 (Box NOT use)

    REG16_MM                                CFG_MM_27_2F[0x30 - 0x27];          // reserved

} REG_Ctrl_MMFI;

typedef struct _REG_Ctrl_MMFI1
{
    REG16_MM                                SWRst;                              // 0x6E
        #define MMFI_SWRST_MASK                         0x07FF
        #define MMFI_SW_RSTZ_MMFILEIN_DISABLE           0x0001  // low active
        #define MMFI_RST_WB_DMA0                        0x0002
        #define MMFI_RST_CMDQ0                          0x0004
        #define MMFI_RST_TSIF0                          0x0008
        #define MMFI_RST_WB0                            0x0010
        #define MMFI_RST_WB_DMA1                        0x0020
        #define MMFI_RST_CMDQ1                          0x0040
        #define MMFI_RST_TSIF1                          0x0080
        #define MMFI_RST_WB1                            0x0100
        #define MMFI_RST_PATH0                          0x0200
        #define MMFI_RST_PATH1                          0x0400
        #define MMFI_RST_ALL                            0x07FE
        #define MMFI_RST_CLK_STAMP                      0x2000
        #define MMFI_RST_LPCR_27M_EN_MMFI0              0x4000
        #define MMFI_RST_LPCR_27M_EN_MMFI1              0x8000

    REG16_MM                                HWInt;                              // 0x6F
//------------------------- no use , just for compatibility -------------------------//
        #define MMFI_HWINT_STS_VD3D_ERR1                0x0000
        #define MMFI_HWINT_STS_AUAUB_ERR1               0x0000
        #define MMFI_HWINT_STS_VD3D_ERR0                0x0000
        #define MMFI_HWINT_STS_AUAUB_ERR0               0x0000
//-----------------------------------------------------------------------------------//
        #define MMFI_HWINT_SRC_MASK                     0x00FF
        #define MMFI_HWINT_SRC_FILEIN_DONE1             0x0004
        #define MMFI_HWINT_SRC_FILEIN_DONE0             0x0008
        #define MMFI_HWINT_SRC_ERR_PKTS1                0x0010
        #define MMFI_HWINT_SRC_ERR_PKTS0                0x0020
        #define MMFI_HWINT_STS_MASK                     0xFF00
        #define MMFI_HWINT_STS_SHIFT                    8
        #define MMFI_HWINT_STS_FILEIN_DONE1             0x0400
        #define MMFI_HWINT_STS_FILEIN_DONE0             0x0800
        #define MMFI_HWINT_STS_ERR_PKTS0                0x1000
        #define MMFI_HWINT_STS_ERR_PKTS1                0x2000

} REG_Ctrl_MMFI1;

#endif // _MMFILEIN_REG_H_
