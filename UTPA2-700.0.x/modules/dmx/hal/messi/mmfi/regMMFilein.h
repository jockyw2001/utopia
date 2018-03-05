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
#define MMFI_AUDPIDFLT_NUM              (2UL)
#define MMFI_V3DPIDFLT_NUM              (1UL)


#define MMFI_PIDFLT_NUM_ALL             (MMFI_AUDPIDFLT_NUM+MMFI_V3DPIDFLT_NUM)

#define MMFI_PID_NULL                   0x1FFFUL

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define REG_CTRL_BASE_MMFI         (0x3800UL)                            // 0xBF800000+(1c00/2)*4
#define REG_CTRL_BASE_MMFI_V3D     (0x3880UL)

typedef struct _REG32_M
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32_M;

typedef struct _REG16_M
{
    volatile MS_U16                u16data;
    volatile MS_U16                _null;
} REG16_M;


typedef struct _REG_Ctrl_MMFI
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
                                                                          // Index(word)  CPU(byte)     MIPS(0x1500/2+index)*4
    REG32_M                           PidFlt;                             // 0xbf803800   0x00
    #define MMFI_PIDFLT_PID_MASK                0x1FFFUL
    #define MMFI_PIDFLT_EN_MASK                 0xE000UL
    #define MMFI_PIDFLT_AFIFO_EN                0x4000UL
    #define MMFI_PIDFLT_VD3D_EN                 0x8000UL
    #define MMFI_PIDFLT_A_MASK                  0x0000FFFFUL
    #define MMFI_PIDFLT_B_MASK                  0xFFFF0000UL
    #define MMFI_PIDFLT_A_SHIFT                 0UL
    #define MMFI_PIDFLT_B_SHIFT                 16UL

    REG32_M                           FileIn_RAddr;                       // 0xbf803808   0x02        //byte address
    REG32_M                           FileIn_RNum;                        // 0xbf803810   0x04

    REG32_M                           Ctrl_CmdQSts;                       // 0xbf803818   0x06
    #define MMFI_FILEIN_CTRL_MASK                   0x000000FFUL
    #define MMFI_FILEIN_RSTART                      0x00000001UL
    #define MMFI_FILEIN_DONE                        0x00000002UL
    #define MMFI_FILEIN_INIT_TRUST                  0x00000004UL
    #define MMFI_FILEIN_ABORT                       0x00000010UL
    #define MMFI_TIMER_MASK                         0x0000FF00UL
    #define MMFI_TIMER_SHIFT                        8UL

    #define MMFI_CMQ_STATUS_SHIFT                   16UL
    #define MMFI_CMDQ_SIZE                          8UL
    #define MMFI_CMQ_WR_CNT_MASK                    0x001F0000UL
    #define MMFI_CMQ_STATUS_FIFO_FULL               0x00400000UL
    #define MMFI_CMQ_STATUS_FIFO_EMPTY              0x00800000UL
    #define MMFI_CMQ_STATU_WR_LEVEL_MASK            0x03000000UL
    #define MMFI_CMQ_STATU_WR_LEVEL_SHIFT           24UL

    REG32_M                           Cfg;                               // 0xbf803820   0x08
    #define MMFI_LPCR2_LOAD                         0x00000001UL
    #define MMFI_LPCR2_WLD                          0x00000002UL
    #define MMFI_TEI_SKIP_PKTF                      0x00000004UL
    #define MMFI_CLR_PIDFLT_BYTE_CNT                0x00000008UL
    #define MMFI_PKT192_BLK_DISABLE                 0x00000010UL
    #define MMFI_PKT192_EN                          0x00000020UL
    #define MMFI_APID_BYPASS                        0x00000040UL
    #define MMFI_VPID3D_BYPASS                      MMFI_APID_BYPASS
    #define MMFI_VID3D_ERR_EN                       0x00000080UL
    #define MMFI_AUD_ERR_EN                         0x00000100UL
    #define MMFI_MEM_TSDATA_ENDIAN_V3D              MMFI_AUD_ERR_EN
    #define MMFI_AUDB_ERR_EN                        0x00000200UL
    #define MMFI_MEM_TSORDER_ENDIAN_V3D             MMFI_AUDB_ERR_EN
    #define MMFI_APES_ERR_RM_EN                     0x00000400UL
    #define MMFI_MEM_TSDATA_ENDIAN_AU               0x00001000UL
    #define MMFI_MEM_TSORDER_ENDIAN_AU              0x00002000UL
    #define MMFI_WBDMA_ECO                          0x00010000UL
    #define MMFI_DIS_MIU_RQ                         0x00020000UL
    #define MMFI_USE_AUD_PATH                       0x00040000UL
    #define MMFI_USE_VD3D_PATH                      MMFI_USE_AUD_PATH
    #define MMFI_BYTE_TIMER_EN                      0x00080000UL
    #define MMFI_BYTE_PLY_FILE_INV_EN               0x00100000UL
    #define MMFI_DUP_PKT_SKIP                       0x00200000UL
    #define MMFI_ALT_TS_SIZE                        0x00400000UL
    #define MMFI_FILEIN2MI_RPRIORITY                0x00800000UL
    #define MMFI_USE_AUDB_PATH                      MMFI_USE_AUD_PATH
    #define MMFI_RADDR_READ_EN                      0x01000000UL
    #define MMFI_CLK27M_ENABLE                      0x04000000UL
    #define MMFI_WB_FSM_RESET                       0x08000000UL
    #define MMFI_FILEIN_PAUSE                       0x10000000UL

    #define MMFI_AU_MODE_MASK                       (MMFI_APID_BYPASS)
    #define MMFI_AU_ERR_MASK                        (MMFI_AUD_ERR_EN|MMFI_AUDB_ERR_EN|MMFI_APES_ERR_RM_EN)
    #define MMFI_AU_CFG_MASK                        (MMFI_AU_MODE_MASK|MMFI_AU_ERR_MASK)
    #define MMFI_VD_MODE_MASK                       (MMFI_VPID3D_BYPASS)
    #define MMFI_VD_ERR_MASK                        (MMFI_VID3D_ERR_EN)
    #define MMFI_VD_CFG_MASK                        (MMFI_VD_MODE_MASK|MMFI_VD_ERR_MASK)

    REG32_M                           TsHeader;                           // 0xbf803828   0x0a
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

    REG32_M                           Pid_Status;                        // 0xbf803830   0x0c
    #define MMFI_PID_MATCHED_MASK                   0x00001FFFUL
    #define MMFI_PID_CHANGE                         0x00002000UL
    #define MMFI_PIFSTS_A_SHIFT                     0UL
    #define MMFI_PIFSTS_B_SHIFT                     16UL

    REG32_M                           LPcr2_Buf;                          // 0xbf803838   0x0e
    REG32_M                           TimeStamp_FIn;                      // 0xbf803840   0x10

    REG32_M                           SWRst_HWInt;                        // 0xbf803848   0x12
    #define MMFI_SWRST_MASK                         0x000007FFUL
    #define MMFI_SW_RSTZ_MMFILEIN_DISABLE           0x00000001UL              // low active
    #define MMFI_RST_WB_DMA_AU                      0x00000002UL
    #define MMFI_RST_CMDQ_AU                        0x00000004UL
    #define MMFI_RST_TSIF_AU                        0x00000008UL
    #define MMFI_RST_WB_P2_AU                       0x00000010UL
    #define MMFI_RST_WB_DMA_VD                      0x00000020UL
    #define MMFI_RST_CMDQ_VD                        0x00000040UL
    #define MMFI_RST_TSIF_VD                        0x00000080UL
    #define MMFI_RST_WB_P2_VD                       0x00000100UL
    #define MMFI_RST_AU_PATH                        0x00000200UL
    #define MMFI_RST_V3D_PATH                       0x00000400UL
    #define MMFI_RST_ALL                            0x000007FEUL

    #define MMFI_HWINT_SRC_SHIFT                    16UL
    #define MMFI_HWINT_SRC_MASK                     0x00FF0000UL
    #define MMFI_HWINT_SRC_FILEIN_DONE_VD           0x00100000UL
    #define MMFI_HWINT_SRC_FILEIN_DONE_AU           0x00200000UL
    #define MMFI_HWINT_SRC_VD3D_ERR                 0x00400000UL
    #define MMFI_HWINT_SRC_AU_ERR                   0x00800000UL
    #define MMFI_HWINT_STS_MASK                     0xFF000000UL
    #define MMFI_HWINT_STS_SHIFT                    24UL
    #define MMFI_HWINT_STS_FILEIN_DONE_VD           0x10000000UL
    #define MMFI_HWINT_STS_FILEIN_DONE_AU           0x20000000UL
    #define MMFI_HWINT_STS_VD3D_ERR                 0x40000000UL
    #define MMFI_HWINT_STS_AU_ERR                   0x80000000UL

    REG32_M                           PktChkSize;                        // 0xbf803850   0x14
    #define MMFI_PKTCHK_SIZE_MASK                   0x000000FFUL
    #define MMFI_SYNC_BYTE_MASK                     0x0000FF00UL
    #define MMFI_SYNC_BYTE_SHIFT                    8UL
    #define MMFI_MOBFKEY_MASK                       0x001F0000UL
    #define MMFI_MOBFKEY_SHIFT                      16UL
    
    REG16_M                          _xbf803854;                        // 0xbf803858   0x16                          

    REG32_M                          Tsp2mi_RAddr;                      // 0xbf80385C  0x17
    
} REG_Ctrl_MMFI;


#endif // _MMFILEIN_REG_H_
