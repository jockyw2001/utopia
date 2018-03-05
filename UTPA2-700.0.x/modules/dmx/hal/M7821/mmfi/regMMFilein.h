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
#define MMFI_ENGINE_NUM                     (2UL)

#define MMFI_PIDFLT_GROUP0                   (4UL) // filters reside in upper half of bank MMFI
#define MMFI_PIDFLT_GROUP1                   (2UL) // filters reside in bottom half of bank MMFI

#define MMFI_PIDFLT0_NUM                    (MMFI_PIDFLT_GROUP0 + MMFI_PIDFLT_GROUP1)
#define MMFI_PIDFLT1_NUM                    (MMFI_PIDFLT0_NUM)

#define MMFI_PIDFLT_NUM_ALL                 (MMFI_PIDFLT0_NUM+MMFI_PIDFLT1_NUM)

#define MMFI_PID_NULL                       0x1FFFUL

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define REG_CTRL_BASE_MMFI0             (0x3800UL)                            // 0xBF800000+(1c00/2)*4
#define REG_CTRL_BASE_MMFI1             (0x3880UL)
#define REG_CTRL2                       (0x3900UL)                            // MMFI part 2

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

typedef struct _REG_Ctrl_MMFI
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
                                                                            // Index(word)  CPU(byte)     MIPS(0x1500/2+index)*4
    REG32                               PidFlt[4];                          // 0xbf803800   0x00
    #define MMFI_PIDFLT_PID_MASK                    0x00001FFFUL
    #define MMFI_PIDFLT_EN_MASK                     0x0007E000UL
    #define MMFI_PIDFLT_AFIFOB_EN                   0x00002000UL
    #define MMFI_PIDFLT_AFIFO_EN                    0x00004000UL
    #define MMFI_PIDFLT_VFIFO_EN                    0x00008000UL
    #define MMFI_PIDFLT_V3DFIFO_EN                  0x00010000UL
    #define MMFI_PIDFLT_AFIFOC_EN                   0x00020000UL
    #define MMFI_PIDFLT_AFIFOD_EN                   0x00040000UL

    REG32                               FileIn_RAddr;                       // 0xbf803820  0x08         //byte address
    REG32                               FileIn_RNum;                        // 0xbf803828  0x0a

    REG16                               FileIn_Ctrl;                        // 0xbf803830   0x0c
    #define MMFI_FILEIN_CTRL_START                  0x0001UL
    #define MMFI_FILEIN_CTRL_DONE                   0x0002UL
    #define MMFI_FILEIN_CTRL_ABORT                  0x0010UL
    #define MMFI_FILEIN_CTRL_MASK                   0x0013UL
    #define MMFI_FILEIN_TIMER_MASK                  0xFF00UL
    #define MMFI_FILEIN_TIMER_SHIFT                 8UL

    REG16                               CmdQSts;                            // 0xbf803834   0x0d
    #define MMFI_CMDQ_SIZE                          8UL
    #define MMFI_CMDQSTS_WRCNT_MASK                 0x001FUL
    #define MMFI_CMDQSTS_FIFO_FULL                  0x0040UL
    #define MMFI_CMDQSTS_FIFO_EMPTY                 0x0080UL
    #define MMFI_CMDQSTS_FIFO_WRLEVEL_MASK          0x0300UL
    #define MMFI_CMDQSTS_FIFO_WRLEVEL_SHIFT         8UL

    REG32                               Cfg;                                // 0xbf803838   0x0e
    #define MMFI_CFG_LPCR2_LD                       0x00000001UL
    #define MMFI_CFG_LPCR2_WLD                      0x00000002UL
    #define MMFI_CFG_TEI_SKIP                       0x00000004UL
    #define MMFI_CFG_CLR_PIDFLT_BYTE_CNT            0x00000008UL
    #define MMFI_CFG_APID_BYPASS                    0x00000010UL
    #define MMFI_CFG_APIDB_BYPASS                   0x00000020UL
    #define MMFI_CFG_VPID_BYPASS                    0x00000040UL
    #define MMFI_CFG_VPID3D_BYPASS                  0x00000080UL
    #define MMFI_CFG_AUD_ERR_EN                     0x00000100UL
    #define MMFI_CFG_AUDB_ERR_EN                    0x00000200UL
    #define MMFI_CFG_VD_ERR_EN                      0x00000400UL
    #define MMFI_CFG_V3D_ERR_EN                     0x00000800UL
    #define MMFI_CFG_APES_ERR_RM_EN                 0x00001000UL
    #define MMFI_CFG_APESB_ERR_RM_EN                0x00002000UL
    #define MMFI_CFG_VPES_ERR_RM_EN                 0x00004000UL
    #define MMFI_CFG_VPES3D_ERR_RM_EN               0x00008000UL
    #define MMFI_CFG_CLR_PKT_CNT                    0x00010000UL
    #define MMFI_CFG_DIS_MIU_RQ                     0x00020000UL
    #define MMFI_CFG_RADDR_READ                     0x00040000UL
    #define MMFI_CFG_BYTETIMER_EN                   0x00080000UL
    #define MMFI_CFG_PLY_FILE_INV_EN                0x00100000UL
    #define MMFI_CFG_DUP_PKT_SKIP                   0x00200000UL
    #define MMFI_CFG_ALT_TS_SIZE                    0x00400000UL
    #define MMFI_CFG_2MI_RPRIORITY                  0x00800000UL
    #define MMFI_CFG_PS_AUD_EN                      0x01000000UL
    #define MMFI_CFG_PS_AUDB_EN                     0x02000000UL
    #define MMFI_CFG_PS_VD_EN                       0x04000000UL
    #define MMFI_CFG_PS_V3D_EN                      0x08000000UL
    #define MMFI_CFG_MEM_TS_ORDER                   0x10000000UL
    #define MMFI_CFG_MEM_TS_DATA_ENDIAN             0x20000000UL
    #define MMFI_CFG_PKT192_EN                      0x40000000UL
    #define MMFI_CFG_PKT192_BLK_DISABLE             0x80000000UL
    #define MMFI_CFG_FILEIN_MODE_MASK               (MMFI_CFG_APID_BYPASS|MMFI_CFG_APIDB_BYPASS|MMFI_CFG_VPID_BYPASS   \
                                                    |MMFI_CFG_VPID3D_BYPASS|MMFI_CFG_PS_AUD_EN|MMFI_CFG_PS_AUDB_EN     \
                                                    |MMFI_CFG_PS_VD_EN|MMFI_CFG_PS_V3D_EN)

    REG32                               TsHeader;                           // 0xbf803840  0x10
    #define MMFI_HD_CCNT_MASK                       0x0000000FUL
    #define MMFI_HD_AF_MASK                         0x00000030UL
    #define MMFI_HD_AF_SHIFT                        4UL
    #define MMFI_HD_SCRAMBLE_MASK                   0x000000C0UL
    #define MMFI_HD_SCRAMBLE_SHIFT                  6UL
    #define MMFI_HD_PID                             0x001FFF00UL
    #define MMFI_HD_PID_SHIFT                       8UL
    #define MMFI_HD_TS_PRIORITY_MASK                0x00200000UL
    #define MMFI_HD_TS_PRIORITY_SHIFT               21UL
    #define MMFI_HD_PAYLOAD_START_FLG_MASK          0x00400000UL
    #define MMFI_HD_PAYLOAD_START_FLG_SHIFT         22UL
    #define MMFI_HD_ERR_FLG_MASK                    0x00800000UL
    #define MMFI_HD_ERR_FLG_SHIFT                   23UL

    REG16                               APid_Status;                        // 0xbf803848   0x12
    #define MMFI_APID_MATCHED_MASK                  0x00001FFFUL
    #define MMFI_APID_CHANGE                        0x00002000UL
    REG16                               APidB_Status;                       // 0xbf803848   0x13
    #define MMFI_APIDB_MATCHED_MASK                 0x00001FFFUL
    #define MMFI_APIDB_CHANGE                       0x00002000UL
    REG16                               VPID_Status;                        // 0xbf803850   0x14
    #define MMFI_VPID_MATCHED_MASK                  0x00001FFFUL
    #define MMFI_VPID_CHANGE                        0x00002000UL
    REG16                               VPID3D_Status;                      // 0xbf803854   0x15
    #define MMFI_VPID3D_MATCHED_MASK                0x00001FFFUL
    #define MMFI_VPID3D_CHANGE                      0x00002000UL

    REG32                               LPcr2_Buf;                          // 0xbf803858   0x16
    REG32                               TimeStamp_FIn;                      // 0xbf803860   0x18

    REG16                               SWRst;                              // 0xbf803868   0x1a
    #define MMFI_SWRST_MASK                         0x07FFUL
    #define MMFI_SW_RSTZ_MMFILEIN_DISABLE           0x0001UL                  // low active
    #define MMFI_RST_WB_DMA0                        0x0002UL
    #define MMFI_RST_CMDQ0                          0x0004UL
    #define MMFI_RST_TSIF0                          0x0008UL
    #define MMFI_RST_WB0                            0x0010UL
    #define MMFI_RST_WB_DMA1                        0x0020UL
    #define MMFI_RST_CMDQ1                          0x0040UL
    #define MMFI_RST_TSIF1                          0x0080UL
    #define MMFI_RST_WB1                            0x0100UL
    #define MMFI_RST_PATH0                          0x0200UL
    #define MMFI_RST_PATH1                          0x0400UL
    #define MMFI_RST_MOBF_MMFI0                     0x0800UL
    #define MMFI_RST_MOBF_MMFI1                     0x1000UL
    #define MMFI_RST_ALL                            0x1FFEUL

    REG16                               HWInt;                             // 0xbf80386c   0x1b
    #define MMFI_HWINT_SRC_MASK                     0x00FFUL
    #define MMFI_HWINT_SRC_FILEIN_DONE1             0x0004UL
    #define MMFI_HWINT_SRC_FILEIN_DONE0             0x0008UL
    #define MMFI_HWINT_SRC_VD3D_ERR1                0x0010UL
    #define MMFI_HWINT_SRC_AUAUB_ERR1               0x0020UL
    #define MMFI_HWINT_SRC_VD3D_ERR0                0x0040UL
    #define MMFI_HWINT_SRC_AUAUB_ERR0               0x0080UL
    #define MMFI_HWINT_STS_MASK                     0xFF00UL
    #define MMFI_HWINT_STS_SHIFT                    8UL
    #define MMFI_HWINT_STS_FILEIN_DONE1             0x0400UL
    #define MMFI_HWINT_STS_FILEIN_DONE0             0x0800UL
    #define MMFI_HWINT_STS_VD3D_ERR1                0x1000UL
    #define MMFI_HWINT_STS_AUAUB_ERR1               0x2000UL
    #define MMFI_HWINT_STS_VD3D_ERR0                0x4000UL
    #define MMFI_HWINT_STS_AUAUB_ERR0               0x8000UL

    REG16                               PktChkSize;                         // 0xbf803870   0x1c
    #define MMFI_PKTCHK_SIZE_MASK                   0x00FFUL
    #define MMFI_SYNC_BYTE_MASK                     0xFF00UL
    #define MMFI_SYNC_BYTE_SHIFT                    8UL

    REG16                               MOBFKey;                            // 0xbf803874   0x1d
    #define MMFI_MOBFKEY_MASK                       0x001FUL
    #define MMFI_FILEIN_CTRL_MOBF_EN                0                       //not used

    REG32                               RAddr;                              // 0xbf803878   0x1e
    #define MMFI_TSP2MI_RADDR_MASK                  0x07FFFFFFUL
} REG_Ctrl_MMFI;

// MMFI part 2
typedef struct _REG_Ctrl_MMFI2
{
    REG16                               RVU_config[2];                          // 0x40- 0x41
    #define MMFI_RVU_PSI_EN                         0x0001UL
    #define MMFI_RVU_TEI_EN                         0x0002UL
    #define MMFI_RVU_ERR_CLR                        0x0004UL
    #define MMFI_RVU_EN                             0x0008UL
    #define MMFI_RVU_TIMESTAMP_EN                   0x0010UL
    REG16                               dummy[14];            					// 0x42 ~ 0x4F
    REG16                               Cfg2[2];                             	// 0x50~ 0x51
    #define MMFI_CFG2_MMFI_27M_EN                   0x0001UL
    #define MMFI_CFG2_MMFI_APIDC_BYPASS             0x0020UL
    #define MMFI_CFG2_MMFI_APIDD_BYPASS             0x0040UL
    #define MMFI_CFG2_MMFI_PS_AUDC_EN               0x0080UL
    #define MMFI_CFG2_MMFI_PS_AUDD_EN               0x0100UL
    #define MMFI_CFG2_TSP_FILEIN_PAUSE_EN           0x0200UL
    #define MMFI_CFG2_WB_FSRM_RST                   0x0400UL

    #define MMFI_CFG2_FILEIN_MODE_MASK          (MMFI_CFG2_MMFI_APIDC_BYPASS|MMFI_CFG2_MMFI_APIDD_BYPASS    \
                                                |MMFI_CFG2_MMFI_PS_AUDC_EN|MMFI_CFG2_MMFI_PS_AUDD_EN)

    REG16                               MMFI0_APidC_Status;                  					// 0xbf803948   0x52
    REG16                               MMFI0_APidD_Status;                  					// 0xbf80394C   0x53
    REG16                               MMFI1_APidC_Status;                  					// 0xbf803950   0x54
    REG16                               MMFI1_APidD_Status;                  					// 0xbf803954   0x55
    REG32                               PidFlt[MMFI_ENGINE_NUM][MMFI_PIDFLT_GROUP1];          	// 0xbf803958   0x56 , MMFI0/1 filter 4~5
    REG16                               MMFI_ats_config[2];                  					// 0x66~0x67
    #define MMFI_ATS_MODE                        0x0001UL
    #define MMFI_ATS_OFFSET_EN                   0x0002UL
    #define MMFI_ATS_OFFSET_MASK                 0x1F00UL
    #define MMFI_ATS_OFFSET_SHIFT                8UL
} REG_Ctrl_MMFI2;
#endif // _MMFILEIN_REG_H_

