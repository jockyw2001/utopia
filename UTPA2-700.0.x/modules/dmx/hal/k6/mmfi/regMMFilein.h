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

#define MMFI_PIDFLT0_NUM                    (4)
#define MMFI_PIDFLT1_NUM                    (4)

#define MMFI_PIDFLT_NUM_ALL                 (MMFI_PIDFLT0_NUM+MMFI_PIDFLT1_NUM)

#define MMFI_PID_NULL                       0x1FFF

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define REG_CTRL_BASE_MMFI0             (0x27E00UL)                            // 0xBF800000+(13F00/2)*4
#define REG_CTRL_BASE_MMFI1             (0x27F00UL)                            // 0xBF800000+(13F80/2)*4

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
                                                                                // Index(word)  CPU(byte)     MIPS(0x13F00/2+index)*4
    REG32_MM                               PidFlt[4];                           // 0xbf827E00   0x00
        #define MMFI_PIDFLT_PID_MASK                    0x00001FFF
        #define MMFI_PIDFLT_EN_MASK                     0x001FE000
        #define MMFI_PIDFLT_AFIFOB_EN                   0x00002000
        #define MMFI_PIDFLT_AFIFO_EN                    0x00004000
        #define MMFI_PIDFLT_VFIFO_EN                    0x00008000
        #define MMFI_PIDFLT_V3DFIFO_EN                  0x00010000
        #define MMFI_PIDFLT_AFIFOC_EN                   0x00020000
        #define MMFI_PIDFLT_AFIFOD_EN                   0x00040000
        #define MMFI_PIDFLT_VFIFO3_EN                   0x00080000
        #define MMFI_PIDFLT_VFIFO4_EN                   0x00100000
        #define MMFI_PIDFLT_AFIFOE_EN                   0x00000000  // not support
        #define MMFI_PIDFLT_AFIFOF_EN                   0x00000000  // not support
        #define MMFI_PIDFLT_VFIFO5_EN                   0x00000000  // not support
        #define MMFI_PIDFLT_VFIFO6_EN                   0x00000000  // not support
        #define MMFI_PIDFLT_VFIFO7_EN                   0x00000000  // not support
        #define MMFI_PIDFLT_VFIFO8_EN                   0x00000000  // not support

    REG32_MM                               FileIn_RAddr;                        // 0xbf803820   0x08         //byte address
    REG32_MM                               FileIn_RNum;                         // 0xbf803828   0x0a

    REG16_MM                               FileIn_Ctrl;                         // 0xbf803830   0x0c
        #define MMFI_FILEIN_CTRL_START                  0x0001
        #define MMFI_FILEIN_CTRL_MOBF_EN                0x0002
        #define MMFI_FILEIN_CTRL_ABORT                  0x0010
        #define MMFI_FILEIN_CTRL_MASK                   0x0013
        #define MMFI_FILEIN_TIMER_MASK                  0xFF00
        #define MMFI_FILEIN_TIMER_SHIFT                 8

    REG16_MM                               CmdQSts;                             // 0xbf803834   0x0d
        #define MMFI_CMDQ_SIZE                          8
        #define MMFI_CMDQSTS_WRCNT_MASK                 0x001F
        #define MMFI_CMDQSTS_FIFO_FULL                  0x0040
        #define MMFI_CMDQSTS_FIFO_EMPTY                 0x0080
        #define MMFI_CMDQSTS_FIFO_WRLEVEL_MASK          0x0300
        #define MMFI_CMDQSTS_FIFO_WRLEVEL_SHIFT         8

    REG32_MM                               Cfg;                                 // 0xbf803838   0x0e
        #define MMFI_CFG_LPCR2_LD                       0x00000001
        #define MMFI_CFG_LPCR2_WLD                      0x00000002
        #define MMFI_CFG_TEI_SKIP                       0x00000004
        #define MMFI_CFG_CLR_PIDFLT_BYTE_CNT            0x00000008
        #define MMFI_CFG_APID_BYPASS                    0x00000010
        #define MMFI_CFG_APIDB_BYPASS                   0x00000020
        #define MMFI_CFG_VPID_BYPASS                    0x00000040
        #define MMFI_CFG_VPID3D_BYPASS                  0x00000080
        #define MMFI_CFG_AUD_ERR_EN                     0x00000100
        #define MMFI_CFG_AUDB_ERR_EN                    0x00000200
        #define MMFI_CFG_VD_ERR_EN                      0x00000400
        #define MMFI_CFG_V3D_ERR_EN                     0x00000800
        #define MMFI_CFG_APES_ERR_RM_EN                 0x00001000
        #define MMFI_CFG_APESB_ERR_RM_EN                0x00002000
        #define MMFI_CFG_VPES_ERR_RM_EN                 0x00004000
        #define MMFI_CFG_VPES3D_ERR_RM_EN               0x00008000
        #define MMFI_CFG_CLR_PKT_CNT                    0x00010000
        #define MMFI_CFG_DIS_MIU_RQ                     0x00020000
        #define MMFI_CFG_RADDR_READ                     0x00040000
        #define MMFI_CFG_BYTETIMER_EN                   0x00080000
        #define MMFI_CFG_PLY_FILE_INV_EN                0x00100000
        #define MMFI_CFG_DUP_PKT_SKIP                   0x00200000
        #define MMFI_CFG_ALT_TS_SIZE                    0x00400000
        #define MMFI_CFG_2MI_RPRIORITY                  0x00800000
        #define MMFI_CFG_PS_AUD_EN                      0x01000000
        #define MMFI_CFG_PS_AUDB_EN                     0x02000000
        #define MMFI_CFG_PS_VD_EN                       0x04000000
        #define MMFI_CFG_PS_V3D_EN                      0x08000000
        #define MMFI_CFG_MEM_TS_ORDER                   0x10000000
        #define MMFI_CFG_MEM_TS_DATA_ENDIAN             0x20000000
        #define MMFI_CFG_PKT192_EN                      0x40000000
        #define MMFI_CFG_PKT192_BLK_DISABLE             0x80000000
        #define MMFI_CFG_FILEIN_MODE_MASK               (MMFI_CFG_APID_BYPASS|MMFI_CFG_APIDB_BYPASS|MMFI_CFG_VPID_BYPASS|MMFI_CFG_VPID3D_BYPASS|MMFI_CFG_PS_AUD_EN|MMFI_CFG_PS_AUDB_EN|MMFI_CFG_PS_VD_EN|MMFI_CFG_PS_V3D_EN)

    REG32_MM                               TsHeader;                            // 0xbf803840   0x10
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

    REG16_MM                               APid_Status;                         // 0xbf803848   0x12
        #define MMFI_APID_MATCHED_MASK                  0x00001FFF
        #define MMFI_APID_CHANGE                        0x00002000
    REG16_MM                               APidB_Status;                        // 0xbf80384C   0x13
        #define MMFI_APIDB_MATCHED_MASK                 0x00001FFF
        #define MMFI_APIDB_CHANGE                       0x00002000
    REG16_MM                               VPID_Status;                         // 0xbf803850   0x14
        #define MMFI_VPID_MATCHED_MASK                  0x00001FFF
        #define MMFI_VPID_CHANGE                        0x00002000
    REG16_MM                               VPID3D_Status;                       // 0xbf803854   0x15
        #define MMFI_VPID3D_MATCHED_MASK                0x00001FFF
        #define MMFI_VPID3D_CHANGE                      0x00002000

    REG32_MM                               LPcr2_Buf;                           // 0xbf803858   0x16
    REG32_MM                               TimeStamp_FIn;                       // 0xbf803860   0x18

    REG16_MM                               SWRst;                               // 0xbf803868   0x1a
        #define MMFI_SWRST_MASK                         0x07FF
        #define MMFI_SW_RSTZ_MMFILEIN_DISABLE           0x0001                  // low active
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
        #define MMFI_RST_LPCR_27M_EN_MMFI0              0x4000
        #define MMFI_RST_LPCR_27M_EN_MMFI1              0x8000

    REG16_MM                               HWInt;                               // 0xbf80386c   0x1b
        #define MMFI_HWINT_SRC_MASK                     0x00FF
        #define MMFI_HWINT_SRC_FILEIN_DONE1             0x0004
        #define MMFI_HWINT_SRC_FILEIN_DONE0             0x0008
        #define MMFI_HWINT_SRC_VD3D_ERR1                0x0010
        #define MMFI_HWINT_SRC_AUAUB_ERR1               0x0020
        #define MMFI_HWINT_SRC_VD3D_ERR0                0x0040
        #define MMFI_HWINT_SRC_AUAUB_ERR0               0x0080
        #define MMFI_HWINT_STS_MASK                     0xFF00
        #define MMFI_HWINT_STS_SHIFT                    8
        #define MMFI_HWINT_STS_FILEIN_DONE1             0x0400
        #define MMFI_HWINT_STS_FILEIN_DONE0             0x0800
        #define MMFI_HWINT_STS_VD3D_ERR1                0x1000
        #define MMFI_HWINT_STS_AUAUB_ERR1               0x2000
        #define MMFI_HWINT_STS_VD3D_ERR0                0x4000
        #define MMFI_HWINT_STS_AUAUB_ERR0               0x8000

    REG16_MM                               PktChkSize;                          // 0xbf803870   0x1c
        #define MMFI_PKTCHK_SIZE_MASK                   0x00FF
        #define MMFI_SYNC_BYTE_MASK                     0xFF00
        #define MMFI_SYNC_BYTE_SHIFT                    8

    REG16_MM                               MOBFKey;                             // 0xbf803874   0x1d
        #define MMFI_MOBFKEY_MASK                       0x001F

    REG32_MM                               RAddr;                               // 0xbf803878   0x1e
        #define MMFI_TSP2MI_RADDR_MASK                  0x0FFFFFFF
} REG_Ctrl_MMFI;

typedef struct _REG_Ctrl_MMFI1
{
    //----------------------------------------------
    // 0xBF827F00 MIPS direct access
    //----------------------------------------------
                                                                                // Index(word)  CPU(byte)     MIPS(0x13F80/2+index)*4
    REG16_MM                               CFG2[2];                             // 0xbf827F00   0x40~0x41
        #define MMFI_CFG2_MMFI_APIDC_BYPASS             0x00000001
        #define MMFI_CFG2_APESC_ERR_RM_EN               0x00000002
        #define MMFI_CFG2_MMFI_PS_AUDC_EN               0x00000004
        #define MMFI_CFG2_FILEIN_PAUSE                  0x00000008
        #define MMFI_CFG2_MMFI_APIDD_BYPASS             0x00000010
        #define MMFI_CFG2_APESD_ERR_RM_EN               0x00000020
        #define MMFI_CFG2_MMFI_PS_AUDD_EN               0x00000040
        #define MMFI_CFG2_WB_FSM_RESET                  0x00000080
        #define MMFI_CFG2_INIT_TIMESTAMP_TSIF           0x00000100
        #define MMFI_CFG2_MMFI_PS_AUDE_EN               0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_AUDF_EN               0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_VD3_EN                0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_VD4_EN                0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_VD5_EN                0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_VD6_EN                0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_VD7_EN                0x00000000  // not support
        #define MMFI_CFG2_MMFI_PS_VD8_EN                0x00000000  // not support
        #define MMFI_CFG2_FILEIN_MODE_MASK              (MMFI_CFG2_MMFI_APIDC_BYPASS | MMFI_CFG2_MMFI_APIDD_BYPASS | MMFI_CFG2_MMFI_PS_AUDC_EN | MMFI_CFG2_MMFI_PS_AUDD_EN)

    REG16_MM                               CFG3[2];                             // 0xbf827F08   0x42~0x43
        #define MMFI_CFG3_RVU_PSI_EN                    0x00000001
        #define MMFI_CFG3_RVU_TEI_EN                    0x00000002
        #define MMFI_CFG3_RVU_ERR_CLR                   0x00000004
        #define MMFI_CFG3_RVU_EN                        0x00000008
        #define MMFI_CFG3_RVU_TIMESTAMP_EN              0x00000010
        #define MMFI_CFG3_MMFI_VPID3_BYPASS             0x00000020
        #define MMFI_CFG3_MMFI_PS_VD3_EN                0x00000040
        #define MMFI_CFG3_VD3_ERR_EN                    0x00000080
        #define MMFI_CFG3_VD3_ERR_RM_EN                 0x00000100
        #define MMFI_CFG3_MMFI_VPID4_BYPASS             0x00000200
        #define MMFI_CFG3_MMFI_PS_VD4_EN                0x00000400
        #define MMFI_CFG3_VD4_ERR_EN                    0x00000800
        #define MMFI_CFG3_VD4_ERR_RM_EN                 0x00001000
        #define MMFI_CFG3_FILEIN_MODE_MASK              (MMFI_CFG3_MMFI_VPID3_BYPASS | MMFI_CFG3_MMFI_VPID4_BYPASS | MMFI_CFG3_MMFI_PS_VD3_EN | MMFI_CFG3_MMFI_PS_VD4_EN)

    REG16_MM                               VPid3_Status_MMFI0;                  // 0xbf827F10   0x44
    REG16_MM                               VPid3_Status_MMFI1;                  // 0xbf827F14   0x45
        #define MMFI_VPID3_MATCHED_MASK                 0x00001FFF
        #define MMFI_VPID3_CHANGE                       0x00002000

    REG16_MM                               VPid4_Status_MMFI0;                  // 0xbf827F18   0x46
    REG16_MM                               VPid4_Status_MMFI1;                  // 0xbf827F1C   0x47
        #define MMFI_VPID4_MATCHED_MASK                 0x00001FFF
        #define MMFI_VPID4_CHANGE                       0x00002000

    REG16_MM                               APidC_Status_MMFI0;                  // 0xbf827F20   0x48
    REG16_MM                               APidC_Status_MMFI1;                  // 0xbf827F24   0x49
        #define MMFI_APIDC_MATCHED_MASK                  0x00001FFF
        #define MMFI_APIDC_CHANGE                        0x00002000

    REG16_MM                               APidD_Status_MMFI0;                  // 0xbf827F28   0x4A
    REG16_MM                               APidD_Status_MMFI1;                  // 0xbf827F2C   0x4B
        #define MMFI_APIDD_MATCHED_MASK                 0x00001FFF
        #define MMFI_APIDD_CHANGE                       0x00002000

} REG_Ctrl_MMFI1;


#endif // _MMFILEIN_REG_H_
