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
//  File name: regTSP.h
//  Description: Transport Stream Processor (TSP) Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TSP_REG_H_
#define _TSP_REG_H_

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


//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------
#define TS_PACKET_SIZE              188


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------

//[CMODEL][FWTSP]
// When enable, interrupt will not lost, CModel will block next packet
// and FwTSP will block until interrupt status is clear by MIPS.
// (For firmware and cmodel only)
#define TSP_DBG_SAFE_MODE_ENABLE    0

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define TSP_PIDFLT_NUM                  64
#define TSP_PIDFLT_EXT_NUM              8

#define TSP_PVR_IF_NUM                  2
#define TSP_MMFI0_FILTER_NUM            4
#define TSP_MMFI1_FILTER_NUM            4
#define TSP_IF_NUM                      3
#define TSP_DEMOD_NUM                   2
#define TSP_VFIFO_NUM                   2
#define TSP_AFIFO_NUM                   2
#define TSP_TS_PAD_NUM                  3  // 3p
#define TSP_VQ_NUM                      3  // VQ0, VQ_file, VQ1
#define TSP_VQ_PITCH                    208
#define TSP_CA_ENGINE_NUM               2
#define TSP_CA_KEY_NUM                  8
#define TSP_CA0_FLT_NUM                 72
#define TSP_CA_FLT_NUM                  72
#define TSP_MERGESTR_MUM                8
#define TSP_ENGINE_NUM                  1
#define TSP_SECFLT_NUM                  64
#define TSP_PCRFLT_NUM                  1
#define TSP_STC_NUM                     1

#ifdef HWPCR_ENABLE
#define TSP_PIDFLT_NUM_ALL              (TSP_PIDFLT_NUM+TSP_PIDFLT_EXT_NUM+TSP_PCRFLT_NUM)
#else
#define TSP_PIDFLT_NUM_ALL              (TSP_PIDFLT_NUM+TSP_PIDFLT_EXT_NUM)
#endif

#define TSP_SECBUF_NUM                  TSP_SECFLT_NUM
#define TSP_FILTER_DEPTH                16

#define TSP_WP_SET_NUM                  4

#define DSCMB_FLT_START_ID              16
#define DSCMB_FLT_END_ID                31
#define DSCMB_FLT_NUM                   16

#define DSCMB1_FLT_START_ID             32
#define DSCMB1_FLT_END_ID               47
#define DSCMB1_FLT_NUM                  16


#define DSCMB_FLT_SHAREKEY_START_ID     48
#define DSCMB_FLT_SHAREKEY_END_ID       64
#define DSCMB_FLT_SHAREKEY_NUM          16

#define DSCMB_FLT_SHAREKEY1_START_ID    48
#define DSCMB_FLT_SHAREKEY1_END_ID      64
#define DSCMB_FLT_SHAREKEY1_NUM         16


#define TSP_NMATCH_FLTID                17

//PAD MUX definition
#define TSP_MUX_TS0                     0
#define TSP_MUX_TS1                     1
#define TSP_MUX_TS2                     2
#define TSP_MUX_TSO                     6
#define TSP_MUX_INDEMOD                 7
#define TSP_MUX_TSCB                    0xFF //not support
#define TSP_MUX_NONE                    0xFF

//Clk source definition
#define TSP_CLK_DISABLE                 0x01
#define TSP_CLK_INVERSE                 0x02
#define TSP_CLK_TS0                     0x00
#define TSP_CLK_TS1                     0x04
#define TSP_CLK_TS2                     0x08
#define TSP_CLK_TSOOUT                  0x18
#define TSP_CLK_INDEMOD                 0x1C
#define CLKGEN0_TSP_CLK_MASK            0x1C
#define TSP_CLK_TSCB                    0xFF    //not support

//PIDFLT1,2 source definition
#define TSP_PIDFLT1_USE_TSIF1           0
#define TSP_PIDFLT2_USE_TSIF2           1
#define TSP_PIDFLT1_USE_TSIF_MMFI0      2
#define TSP_PIDFLT2_USE_TSIF_MMFI1      3


#define TSP_FW_DEVICE_ID                0x67

#define STC_SYNTH_DEFAULT               0x28000000

#define DRAM_SIZE                       (0x20000000)
#define TSP_FW_BUF_SIZE                 (0x4000UL)
#define TSP_FW_BUF_LOW_BUD              0
#define TSP_FW_BUF_UP_BUD               DRAM_SIZE

#define TSP_VQ_BUF_LOW_BUD              0
#define TSP_VQ_BUF_UP_BUD               DRAM_SIZE

#define TSP_SEC_BUF_LOW_BUD             0
#define TSP_SEC_BUF_UP_BUD              DRAM_SIZE
#define TSP_SEC_FLT_DEPTH               32
#define TSP_FIQ_NUM                     0

//QMEM definition
#define _TSP_QMEM_I_MASK            0xffff8000 //total: 0x4000
#define _TSP_QMEM_I_ADDR_HIT        0x00000000
#define _TSP_QMEM_I_ADDR_MISS       0xffffffff
#define _TSP_QMEM_D_MASK            0xffff8000
#define _TSP_QMEM_D_ADDR_HIT        0x00000000
#define _TSP_QMEM_D_ADDR_MISS       0xffffffff
#define _TSP_QMEM_SIZE              0x1000 // 16K bytes, 32bit aligment  //0x4000

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Software
#define REG_PIDFLT_L_BASE                (0x00210000 << 1)                   // Fit the size of REG32, 0~63
#define REG_PIDFLT_H_BASE                (0x00210800 << 1)                   // Fit the size of REG32, 0~63

#define REG_SECFLT_BASE1                 (0x00211000 << 1)                   // Fix the size of REG32

#define REG_CTRL_BASE                   (0x2A00)                            // 0xBF800000+(1500/2)*4
#define REG_CTRL_MMFIBASE               (0x39C0)                            // 0xBF800000+(3800/2)*4 (TSP2: debug table), from 0x70
#define REG_CTRL_TSP3                   (0xC1440)
#define REG_CTRL_TSP4                   (0xC2E00)
#define REG_CTRL_TSP5                   (0xC7600)
#define REG_CTRL_TS_SAMPLE              (0x21600)

typedef struct _REG32
{
    volatile MS_U16                L;
    volatile MS_U16                empty_L;
    volatile MS_U16                H;
    volatile MS_U16                empty_H;
} REG32;

typedef struct _REG32_L
{
    volatile MS_U32                data;
    volatile MS_U32                _resv;
} REG32_L;

typedef struct _REG16
{
    volatile MS_U16                 u16data;
    volatile MS_U16                 _null;
} REG16;

typedef REG32                           REG_PidFlt;

//******************** PIDFLT DEFINE START ********************//
// PID
#define TSP_PIDFLT_PID_MASK             0x00001FFF
#define TSP_PIDFLT_PID_SHFT             0

// PIDFLT SRC
#define TSP_PIDFLT_IN_MASK              0x0000E000
#define TSP_PIDFLT_IN_NONE              0x00000000
#define TSP_PIDFLT_IN_PIDFLT0           0x00002000
#define TSP_PIDFLT_IN_PIDFLT_FILE       0x00004000
#define TSP_PIDFLT_IN_PIDFLT1           0x00006000
#define TSP_PIDFLT_IN_PIDFLT2           0x00008000
#define TSP_PIDFLT_IN_PIDFLT_CB         0                                   //not support
#define TSP_PIDFLT_IN_SHIFT             13

// Section filter Id (0~64)
#define TSP_PIDFLT_SECFLT_MASK          0x000007f0                          // [36:42] secflt id
#define TSP_PIDFLT_SECFLT_SHFT          4

// Stream source ID
#define TSP_PIDFLT_IN_SRC_MASK          0x0000000F                         // [35:32] stream source id
#define TSP_PIDFLT_IN_SRC_SHFT          0

// AF/Sec/Video/V3D/Audio/Audio-second/PVR1/PVR2
#define TSP_PIDFLT_OUT_MASK             0xFFE00000
#define TSP_PIDFLT_OUT_NONE             0x00000000
#define TSP_PIDFLT_OUT_AFIFO4           0x00200000
#define TSP_PIDFLT_OUT_AFIFO3           0x00400000
#define TSP_PIDFLT_OUT_SECFLT_AF        0x01000000
#define TSP_PIDFLT_OUT_SECFLT           0x02000000
#define TSP_PIDFLT_OUT_VFIFO            0x04000000
#define TSP_PIDFLT_OUT_VFIFO3D          0x08000000
#define TSP_PIDFLT_OUT_AFIFO            0x10000000
#define TSP_PIDFLT_OUT_AFIFO2           0x20000000
#define TSP_PIDFLT_OUT_PVR1             0x80000000
#define TSP_PIDFLT_OUT_PVR2             0x40000000

#define TSP_PIDFLT_SECFLT_NULL          0x3F                                // software usage clean selected section filter
//******************** PIDFLT DEFINE END ********************//

typedef struct _REG_SecFlt
{
    REG32                           Ctrl;
    // SW flag
    #define TSP_SECFLT_TYPE_MASK                    0x01000007
    #define TSP_SECFLT_TYPE_SHFT                    0
    #define TSP_SECFLT_TYPE_SEC                     0x00000000
    #define TSP_SECFLT_TYPE_PES                     0x00000001
    #define TSP_SECFLT_TYPE_PKT                     0x00000002
    #define TSP_SECFLT_TYPE_PCR                     0x00000003
    #define TSP_SECFLT_TYPE_TTX                     0x00000004
    #define TSP_SECFLT_TYPE_VER                     0x00000005
    #ifdef SEC_ADF_TYPE_SUPPORT
    #define TSP_SECFLT_TYPE_ADF                     0x00000006          //for af_descriptor
    #endif
    //#define TSP_SECFLT_TYPE_EMM                     0x00000006
    //#define TSP_SECFLT_TYPE_ECM                     0x00000007
    #define TSP_SECFLT_TYPE_SEC_NO_PUSI             0x01000000


    #define TSP_SECFLT_PCRRST                       0x00000010          // for TSP_SECFLT_TYPE_PCR

    #define TSP_SECFLT_MODE_MASK                    0x00000030          // software implementation
    #define TSP_SECFLT_MODE_SHFT                    4
    #define TSP_SECFLT_MODE_CONTI                   0x0
    #define TSP_SECFLT_MODE_ONESHOT                 0x1
    #define TSP_SECFLT_MODE_CRCCHK                  0x2
    #define TSP_SECFLT_MODE_PESSCMCHK               0x3                 //Only for PES type checking SCMB status

    #define TSP_SECFLT_STATE_MASK                   0x000000C0          // software implementation
    #define TSP_SECFLT_STATE_SHFT                   6
    #define TSP_SECFLT_STATE_OVERFLOW               0x1
    #define TSP_SECFLT_STATE_DISABLE                0x2

    REG32                           Match[TSP_FILTER_DEPTH/sizeof(MS_U32)];

    REG32                           Mask[TSP_FILTER_DEPTH/sizeof(MS_U32)];

    REG32                           BufStart;
    #define TSP_SECFLT_BUFSTART_MASK                0xFFFFFFFF

    REG32                           BufEnd;

    REG32                           BufRead;

    REG32                           BufWrite;

    REG32                           BufCur;

    REG32                           RmnReqCnt;
    #define TSP_SECFLT_OWNER_MASK                   0x80000000
    #define TSP_SECFLT_OWNER_SHFT                   31
    #define TSP_SECFLT_REQCNT_MASK                  0x7FFF0000
    #define TSP_SECFLT_REQCNT_SHFT                  16
    #define TSP_SECFLT_RMNCNT_MASK                  0x0000FFFF
    #define TSP_SECFLT_RMNCNT_SHFT                  0

    REG32                           CRC32;

    REG32                           _x50[16];       // (0x210080-0x210050)/4
} REG_SecFlt;


typedef struct _REG_Stc
{
    REG32                           ML;
    REG32_L                         H32;
} REG_Stc;

typedef struct _REG_Pid
{                                                                       // Index(word)  CPU(byte)       Default
    REG_PidFlt                      Flt[TSP_PIDFLT_NUM_ALL];
} REG_Pid;

typedef struct _REG_Sec
{                                                                       // Index(word)  CPU(byte)       Default
    REG_SecFlt                      Flt[TSP_SECFLT_NUM];
} REG_Sec;

typedef struct _REG_Ctrl
{
    //----------------------------------------------
    // 0xBF802A00 MIPS direct access
    //----------------------------------------------
    // Type                         Name                                Index(word)     CPU(byte)     MIPS(0x1500/2+index)*4
    REG32                           TsRec_Head20;                       // 0xbf802a00   0x00
    #define TSP_HW_PVR_BUF_HEAD20_MASK              0xFFFF0000
    #define TSP_HW_PVR_BUF_HEAD20_SHFT              16

    REG32                           TsRec_Head21_Mid20_Wptr;            // 0xbf802a08   0x02 ,wptr & mid share same register
    #define TSP_HW_PVR_BUF_HEAD21_MASK              0x000007FF
    #define TSP_HW_PVR_BUF_HEAD21_SHFT              0
    #define TSP_HW_PVR_BUF_MID20_MASK               0xFFFF0000
    #define TSP_HW_PVR_BUF_MID20_SHFT               16

    REG32                           TsRec_Mid21_Tail20;                 // 0xbf802a10   0x04
    #define TSP_HW_PVR_BUF_MID21_MASK               0x000007FF
    #define TSP_HW_PVR_BUF_MID21_SHFT               0
    #define TSP_HW_PVR_BUF_TAIL20_MASK              0xFFFF0000
    #define TSP_HW_PVR_BUF_TAIL20_SHFT              16

    REG32                           TsRec_Tail2_Pcr1;                   // 0xbf802a18   0x06
    #define TSP_HW_PVR_BUF_TAIL21_MASK              0x000007FF
    #define TSP_HW_PVR_BUF_TAIL21_SHFT              0                   // PCR64 L16
    #define TSP_PCR64_L16_MASK                      0xFFFF0000
    #define TSP_PCR64_L16_SHFT                      16

    REG32                           Pcr1;                               // 0xbf802a20   0x08
    #define TSP_PCR64_MID32_MASK                    0xFFFFFFFF          // PCR64 Middle 64
    #define TSP_PCR64_MID32_SHFT                    0

    REG32                           Pcr64_H;                            // 0xbf802a28   0x0a
    #define TSP_PCR64_H16_MASK                      0x0000FFFF
    #define TSP_PCR64_H16_SHFT                      0
    #define TSP_MOBF_FILE_INDEX_MASK                0x001F0000          // MOBF file index
    #define TSP_MOBF_FILE_INDEX_SHIFT               16

    REG16                           _xbf202a30;                         // 0xbf802a30   0x0c
    REG16                           sw_mail_box0;                       // 0xbf802a34   0x0d

    REG32                           PVR2_Config;                        // 0xbf802a38   0x0e
    #define TSP_PVR2_LPCR1_WLD                      0x00000001
    #define TSP_PVR2_LPCR1_RLD                      0x00000002
    #define TSP_PVR2_STR2MIU_DSWAP                  0x00000004
    #define TSP_PVR2_STR2MIU_EN                     0x00000008
    #define TSP_PVR2_STR2MIU_RST_WADR               0x00000010
    #define TSP_PVR2_STR2MIU_BT_ORDER               0x00000020
    #define TSP_PVR2_STR2MIU_PAUSE                  0x00000040
    #define TSP_PVR2_REG_PINGPONG_EN                0x00000080
    #define TSP_PVR2_PVR_ALIGN_EN                   0x00000100
    #define TSP_PVR2_DMA_FLUSH_EN                   0x00000200
    #define TSP_PVR2_PKT192_EN                      0x00000400
    #define TSP_PVR2_BURST_LEN_MASK                 0x00001800
    #define TSP_PVR2_BURST_LEN_4                    0x00000800
    #define TSP_REC_DATA2_INV                       0x00002000
    #define TSP_V_BLOCK_DIS                         0x00004000
    #define TSP_V3D_BLOCK_DIS                       0x00008000
    #define TSP_AUD_BLOCK_DIS                       0x00010000
    #define TSP_AUDB_BLOCK_DIS                      0x00020000
    #define TSP_PVR1_BLOCK_DIS                      0x00040000
    #define TSP_PVR2_BLOCK_DIS                      0x00080000
    #define TSP_TSIF2_ENABLE                        0x00100000
    #define TSP_TSIF2_DATASWAP                      0x00200000
    #define TSP_TSIF2_SERL                          0x00000000
    #define TSP_TSIF2_PARL                          0x00400000
    #define TSP_TSIF2_EXTSYNC                       0x00800000
    #define TSP_TSIF2_BYPASS                        0x01000000
    #define TSP_TEI_SKIP_PKT2                       0x02000000
    #define TSP_CLR_LOCKED_PKT_CNT                  0x20000000
    #define TSP_CLR_AV_PKT_CNT                      0x40000000
    #define TSP_CLR_PVR_OVERFLOW                    0x80000000

    REG32                           PVR2_LPCR1;                         // 0xbf802a40   0x10

    #define TSP_STR2MI2_ADDR_MASK  0x07FFFFFF
    REG32                           Str2mi_head1_pvr2;                  // 0xbf802a48   0x12
    REG32                           Str2mi_mid1_wptr_pvr2;              // 0xbf802a50   0x14
    REG32                           Str2mi_tail1_pvr2;                  // 0xbf802a58   0x16
    REG32                           Str2mi_head2_pvr2;                  // 0xbf802a60   0x18
    REG32                           Str2mi_mid2_pvr2;                   // 0xbf802a68   0x1a, PVR2 mid address & write point
    REG32                           Str2mi_tail2_pvr2;                  // 0xbf802a70   0x1c
    REG32                           _xbf802a70;                         // 0xbf802a78   0x1e
    REG32                           Pkt_CacheW0;                        // 0xbf802a80   0x20

    REG32                           Pkt_CacheW1;                        // 0xbf802a88   0x22

    REG32                           Pkt_CacheW2;                        // 0xbf802a90   0x24

    REG32                           Pkt_CacheW3;                        // 0xbf802a98   0x26

    REG32_L                         Pkt_CacheIdx;                       // 0xbf802aa0   0x28

    REG32                           Pkt_DMA;                            // 0xbf802aa8   0x2a
    #define TSP_SEC_DMAFIL_NUM_MASK                 0x000000FF
    #define TSP_SEC_DMAFIL_NUM_SHIFT                0
    #define TSP_SEC_DMASRC_OFFSET_MASK              0x0000FF00
    #define TSP_SEC_DMASRC_OFFSET_SHIFT             8
    #define TSP_SEC_DMASRC_OFFSET_MASK              0x0000FF00
    #define TSP_SEC_DMADES_LEN_MASK                 0x00FF0000
    #define TSP_SEC_DMADES_LEN_SHIFT                16

    REG32                           Hw_Config0;                         // 0xbf802ab0   0x2c
    #define TSP_HW_CFG0_DATA_PORT_EN                0x00000001
    #define TSP_HW_CFG0_TSIFO_SERL                  0x00000000
    #define TSP_HW_CFG0_TSIF0_PARL                  0x00000002
    #define TSP_HW_CFG0_TSIF0_EXTSYNC               0x00000004
    #define TSP_HW_CFG0_TSIF0_TS_BYPASS             0x00000008
    #define TSP_HW_CFG0_TSIF0_VPID_BYPASS           0x00000010
    #define TSP_HW_CFG0_TSIF0_APID_BYPASS           0x00000020
    #define TSP_HW_CFG0_WB_DMA_RESET                0x00000040
    #define TSP_HW_CFG0_TSIF0_APID_B_BYPASS         0x00000080
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_MASK        0x0000FF00
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_SHIFT       8
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_MASK    0x00FF0000
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_SHIFT   16
    #define TSP_HW_CFG0_PACKET_CHK_SIZE_MASK        0xFF000000
    #define TSP_HW_CFG0_PACKET_CHK_SIZE_SHFT        24

    REG32                           TSP_DBG_PORT;                       // 0xbf802ab8   0x2e
    #define TSP_DBG_FILTER_MATCH0_MASK              0x000000FF
    #define TSP_DBG_FILTER_MATCH0_SHIFT             0
    #define TSP_DBG_FILTER_MATCH1_MASK              0x0000FF00
    #define TSP_DBG_FILTER_MATCH1_SHIFT             8
    #define TSP_DNG_DATA_PORT_MASK                  0x00FF0000
    #define TSP_DNG_DATA_PORT_SHIFT                 16

    REG_Stc                         Pcr;                                // 0xbf802ac0   0x30 & 0x32

    REG32                           Pkt_Info;                           // 0xbf802ad0   0x34
    #define TSP_APID_L_MASK                         0x000000FF
    #define TSP_APID_L_SHIFT                        0
    #define TSP_APID_H_MASK                         0x00001F00
    #define TSP_APID_H_SHIFT                        8
    #define TSP_PKT_PID_8_12_CP_MASK                0x001F0000
    #define TSP_PKT_PID_8_12_CP_SHIFT               16
    #define TSP_PKT_PRI_MASK                        0x00200000
    #define TSP_PKT_PRI_SHIFT                       21
    #define TSP_PKT_PLST_MASK                       0x00400000
    #define TSP_PKT_PLST_SHIFT                      22
    #define TSP_PKT_ERR                             0x00800000
    #define TSP_PKT_ERR_SHIFT                       23
    #define TSP_DMAW_NO_HIT_INT                     0x0F000000
    #define TSP_DMAW_NO_HIT_INT_SHIFT               24

    REG32                           Pkt_Info2;                          // 0xbf802ad8   0x36
    #define TSP_PKT_INFO_CC_MASK                    0x0000000F
    #define TSP_PKT_INFO_CC_SHFT                    0
    #define TSP_PKT_INFO_ADPCNTL_MASK               0x00000030
    #define TSP_PKT_INFO_ADPCNTL_SHFT               4
    #define TSP_PKT_INFO_SCMB                       0x000000C0
    #define TSP_PKT_INFO_SCMB_SHFT                  6
    #define TSP_PKT_PID_0_7_CP_MASK                 0x0000FF00
    #define TSP_PKT_PID_0_7_CP_SHIFT                8
    #define TSP_VFIFO3D_STATUS                      0x000F0000
    #define TSP_VFIFO3D_STATUS_SHFT                 16
    #define TSP_VFIFO_STATUS                        0x00F00000
    #define TSP_VFIFO_STATUS_SHFT                   20
    #define TSP_AFIFO_STATUS                        0x0F000000
    #define TSP_AFIFO_STATUS_SHFT                   24
    #define TSP_AFIFOB_STATUS                       0xF0000000
    #define TSP_AFIFOB_STATUS_SHFT                  28

    REG32                           SwInt_Stat;                         // 0xbf802ae0   0x38
    #define TSP_SWINT_INFO_SEC_MASK                 0x000000FF
    #define TSP_SWINT_INFO_SEC_SHFT                 0
    #define TSP_SWINT_INFO_ENG_MASK                 0x0000FF00
    #define TSP_SWINT_INFO_ENG_SHFT                 8
    #define TSP_SWINT_STATUS_CMD_MASK               0x7FFF0000
    #define TSP_SWINT_STATUS_CMD_SHFT               16
    #define TSP_SWINT_STATUS_SEC_RDY                0x0001
    #define TSP_SWINT_STATUS_REQ_RDY                0x0002
    #define TSP_SWINT_STATUS_BUF_OVFLOW             0x0006
    #define TSP_SWINT_STATUS_SEC_CRCERR             0x0007
    #define TSP_SWINT_STATUS_SEC_ERROR              0x0008
    #define TSP_SWINT_STATUS_SYNC_LOST              0x0010
    #define TSP_SWINT_STATUS_PKT_OVRUN              0x0020
    #define TSP_SWINT_STATUS_DEBUG                  0x0030
    #define TSP_SWINT_CMD_DMA_PAUSE                 0x0100
    #define TSP_SWINT_CMD_DMA_RESUME                0x0200
    #define TSP_SWINT_STATUS_SEC_GROUP              0x000F
    #define TSP_SWINT_STATUS_GROUP                  0x00FF
    #define TSP_SWINT_CMD_GROUP                     0x7F00
    #define TSP_SWINT_CMD_STC_UPD                   0x0400
    #define TSP_SWINT_CTRL_FIRE                     0x80000000

    REG32                           TsDma_Addr;                         // 0xbf802ae8   0x3a

    REG32                           TsDma_Size;                         // 0xbf802af0   0x3c

    REG32                           TsDma_Ctrl_CmdQ;                    // 0xbf802af8   0x3e

    #define TSP_TSDMA_CTRL_VPES0                    0x00000004
    #define TSP_TSDMA_CTRL_APES0                    0x00000008
    #define TSP_TSDMA_CTRL_A2PES0                   0x00000010
    #define TSP_TSDMA_CTRL_V3DPES0                  0x00000020  //not used
    #define TSP_TSDMA_CTRL_A3PES0                   0x00000040  //not used
    #define TSP_TSDMA_CTRL_A4PES0                   0x00000080  //not used

    #define TSP_TSDMA_CTRL_START                    0x00000001
    #define TSP_TSDMA_CTRL_DONE                     0x00000002
    #define TSP_TSDMA_CTRL_INIT_TRUST               0x00000004
    #define TSP_TSDMA_STAT_ABORT                    0x00000080
    #define TSP_CMDQ_CNT_MASK                       0x001F0000
    #define TSP_CMDQ_CNT_SHFT                       16
    #define TSP_CMDQ_FULL                           0x00400000
    #define TSP_CMDQ_EMPTY                          0x00800000
    #define TSP_CMDQ_SIZE                           16
    #define TSP_CMDQ_WR_LEVEL_MASK                  0x03000000
    #define TSP_CMDQ_WR_LEVEL_SHFT                  24

    REG32                           MCU_Cmd;                            // 0xbf802b00   0x40
    #define TSP_MCU_CMD_MASK                                    0xFF000000
    #define TSP_MCU_CMD_NULL                                    0x00000000
    #define TSP_MCU_CMD_ALIVE                                   0x01000000
    #define TSP_MCU_CMD_NMATCH                                  0x02000000
    #define TSP_MCU_CMD_NMATCH_FLT_MASK                         0x000000FF
    #define TSP_MCU_CMD_NMATCH_FLT_SHFT                         0x00000000
    #define TSP_MCU_CMD_PCR_GET                                 0x03000000
    #define TSP_MCU_CMD_VER_RESET                               0x04000000
        #define TSP_MCU_CMD_VER_RESET_FLT_MASK                  0x000000FF
        #define TSP_MCU_CMD_VER_RESET_FLT_SHFT                  0x00000000
    #define TSP_MCU_CMD_MEM_HIGH_ADDR                           0x05000000
    #define TSP_MCU_CMD_MEM_LOW_ADDR                            0x06000000
        #define TSP_MCU_CMD_MEM_ADDR_SHFT                       0x00000000
        #define TSP_MCU_CMD_MEM_ADDR_MASK                       0x0000FFFF
    #define TSP_MCU_CMD_VERSION_GET                             0x07000000
    #define TSP_MCU_CMD_DBG_MEM                                 0x08000000
    #define TSP_MCU_CMD_DBG_WORD                                0x09000000
    #define TSP_MCU_CMD_HWPCR_REG_SET                           0x0A000000
    #define TSP_MCU_CMD_SCMSTS_GET                              0x0B000000
    #define TSP_MCU_CMD_CTRL_STC_UPDATE                         0x0C000000
    #define TSP_MCU_CMD_CTRL_STC1_UPDATE                        0x0D000000
        #define TSP_MCU_CMD_CTRL_STC_UPDATE_OPTION_MASK         0x00FF0000
        #define TSP_MCU_CMD_CTRL_STC_UPDATE_ONCE                0x00010000
    #define TSP_MCU_CMD_TEI_COUNT_GET                           0x0E000000
        #define TSP_MCU_CMD_TEI_COUNT_SRC_MASK                  0x0000FFFF
            #define TSP_MCU_CMD_TEI_COUNT_SRC_LIVE              0x00000000
            #define TSP_MCU_CMD_TEI_COUNT_SRC_FILE              0x00000001
        #define TSP_MCU_CMD_TEI_COUNT_OPTION_MASK               0x00FF0000
            #define TSP_MCU_CMD_TEI_COUNT_OPTION_RESET          0x00800000
    #define TSP_MCU_CMD_DISCONT_COUNT_GET                       0x0F000000
        #define TSP_MCU_CMD_DISCONT_COUNT_FLT_MASK              0x0000FFFF
            #define TSP_MCU_CMD_DISCONT_COUNT_OPTION_MASK       0x00FF0000
        #define TSP_MCU_CMD_DISCONT_COUNT_OPTION_RESET          0x00800000
    #define TSP_MCU_CMD_SET_STC_OFFSET                          0x10000000
        #define TSP_MCU_CMD_SET_STC_OFFSET_OPTION_MASK          0x00FF0000
        #define TSP_MCU_CMD_SET_STC_OFFSET_OPTION_SHIFT         16
    #define TSP_MCU_CMD_SEL_STC_ENG                             0x20000000
        #define TSP_MCU_SEL_STC_ENG_ID_MASK                     0x000000FF
        #define TSP_MCU_SEL_STC_ENG_ID_SHIFT                    0
        #define TSP_MCU_CMD_SEL_STC_ENG_FLTSRC_MASK             0x0000FF00
        #define TSP_MCU_CMD_SEL_STC_ENG_FLTSRC_SHIFT            8

    REG32                           Hw_Config2;                         // 0xbf802b08   0x42
    #define TSP_HW_CFG2_PACKET_CHK_SIZE1_MASK       0x000000FF
    #define TSP_HW_CFG2_PACKET_CHK_SIZE1_SHFT       0
    #define TSP_HW_CFG2_PACKET_SYNCBYTE1_MASK       0x0000FF00
    #define TSP_HW_CFG2_PACKET_SYNCBYTE1_SHFT       8
    #define TSP_HW_CFG2_PACKET_SIZE1_MASK           0x00FF0000
    #define TSP_HW_CFG2_PACKET_SIZE1_SHFT           16
    #define TSP_HW_CFG2_TSIF1_SERL                  0x00000000
    #define TSP_HW_CFG2_TSIF1_PARL                  0x01000000
    #define TSP_HW_CFG2_TSIF1_EXTSYNC               0x02000000
    #define TSP_HW_CFG2_TS_DATAPORT_EN1             0x04000000
    #define TSP_HW_CFG2_PIDFLT1_SOURCE_TSIF_MMFI0   0x20000000          // Switch source of PIDFLT1 to MMFI0
    #define TSP_HW_CFG2_PIDFLT2_SOURCE_TSIF_MMFI1   0x40000000          // Switch source of PIDFLT2 to MMFI1

    REG32                           Hw_Config4;                         // 0xbf802b10   0x44
    #define TSP_HW_CFG4_PVR_ENABLE                  0x00000002
    #define TSP_HW_CFG4_PVR_ENDIAN_BIG              0x00000004          // 1: record TS to MIU with big endian, 0: record TS to MIU with little endian
    #define TSP_HW_CFG4_TSIF1_ENABLE                0x00000008          // 1: enable ts interface 1 and vice versa
    #define TSP_HW_CFG4_PVR_FLUSH                   0x00000010          // 1: str2mi_wadr <- str2mi_miu_head
    #define TSP_HW_CFG4_PVRBUF_BYTEORDER_BIG        0x00000020          // Byte order of 8-byte recoding buffer to MIU.
    #define TSP_HW_CFG4_PVR_PAUSE                   0x00000040
    #define TSP_HW_CFG4_MEMTSDATA_ENDIAN_BIG        0x00000080          // 32-bit data byte order read from 8x64 FIFO when playing file.
    #define TSP_HW_CFG4_TSIF0_ENABLE                0x00000100          // 1: enable ts interface 0 and vice versa
    #define TSP_VALID_FALLING_DETECT                0x00000200          // Reset bit count when data valid signal of TS interface is low.
    #define TSP_SYNC_RISING_DETECT                  0x00000400          // Reset bit count on the rising sync signal of TS interface.
    #define TSP_HW_CFG4_TS_DATA0_SWAP               0x00000800          // Set 1 to swap the bit order of TS0 DATA bus
    #define TSP_HW_CFG4_TS_DATA1_SWAP               0x00001000          // Set 1 to swap the bit order of TS1 DATA bus
    #define TSP_HW_TSP2OUTAEON_INT_EN               0x00004000          // Set 1 to force interrupt to outside AEON
    #define TSP_HW_HK_INT_FORCE                     0x00008000          // Set 1 to force interrupt to HK_MCU
    #define TSP_HW_CFG4_BYTE_ADDR_DMA               0x000F0000          // prevent from byte enable bug, bit1~3 must enable togather
    #define TSP_HW_CFG4_ALT_TS_SIZE                 0x00010000          // enable TS packets in 204 mode
    #define TSP_HW_DMA_MODE_MASK                    0x00300000          // Section filter DMA mode, 2'b00: Single.2'b01: Burst 2 bytes.2'b10: Burst 4 bytes.2'b11: Burst 8 bytes.
    #define TSP_HW_DMA_MODE_SHIFT                   20
    #define TSP_HW_CFG4_WSTAT_CH_EN                 0x00400000
    #define TSP_HW_CFG4_PS_VID_EN                   0x00800000          // program stream video enable
    #define TSP_HW_CFG4_PS_AUD_EN                   0x01000000          // program stream audio enable
    #define TSP_HW_CFG4_PS_AUD2_EN                  0x02000000          // program stream audioB enable
    #define TSP_HW_CFG4_APES_ERR_RM_EN              0x04000000          // Set 1 to enable removing APES error packet
    #define TSP_HW_CFG4_VPES_ERR_RM_EN              0x08000000          // Set 1 to enable removing VPES error packet
    #define TSP_HW_CFG4_SEC_ERR_RM_EN               0x10000000          // Set 1 to enable removing section error packet
    #define TSP_HW_CFG4_VID_ERR                     0x20000000          // Set 1 to mask the error packet interrupt
    #define TSP_HW_CFG4_AUD_ERR                     0x40000000          // Set 1 to mask the error packet interrupt
    #define TSP_HW_CFG4_ISYNC_PATCH_EN              0x80000000          // Set 1 to enable the patch of internal sync in "tsif"

    REG32                           NOEA_PC;                            // 0xbf802b18   0x46

    REG32                           Idr_Ctrl_Addr0;                     // 0xbf802b20   0x48
    #define TSP_IDR_START                           0x00000001
    #define TSP_IDR_READ                            0x00000000
    #define TSP_IDR_WRITE                           0x00000002
    #define TSP_IDR_WR_ENDIAN_BIG                   0x00000004
    #define TSP_IDR_WR_ADDR_AUTO_INC                0x00000008          // Set 1 to enable address auto-increment after finishing read/write
    #define TSP_IDR_WDAT0_TRIG_EN                   0x00000010          // WDAT0_TRIG_EN
    #define TSP_IDR_MCUWAIT                         0x00000020
    #define TSP_IDR_SOFT_RST                        0x00000080          // Set 1 to soft-reset the IND32 module
    #define TSP_IDR_AUTO_INC_VAL_MASK               0x00000F00
    #define TSP_IDR_AUTO_INC_VAL_SHIFT              8
    #define TSP_IDR_ADDR_MASK0                      0xFFFF0000
    #define TSP_IDR_ADDR_SHFT0                      16

    REG32                           Idr_Addr1_Write0;                   // 0xbf802b28   0x4a
    #define TSP_IDR_ADDR_MASK1                      0x0000FFFF
    #define TSP_IDR_ADDR_SHFT1                      0
    #define TSP_IDR_WRITE_MASK0                     0xFFFF0000
    #define TSP_IDR_WRITE_SHFT0                     16

    REG32                           Idr_Write1_Read0;                   // 0xbf802b30   0x4c
    #define TSP_IDR_WRITE_MASK1                     0x0000FFFF
    #define TSP_IDR_WRITE_SHFT1                     0
    #define TSP_IDR_READ_MASK0                      0xFFFF0000
    #define TSP_IDR_READ_SHFT0                      16

    REG32                           Idr_Read1;                          // 0xbf802b38   0x4e
    #define TSP_IDR_READ_MASK1                      0x0000FFFF
    #define TSP_IDR_READ_SHFT1                      0
    #define TSP_V3D_FIFO_DISCON                     0x00100000
    #define TSP_V3D_FIFO_OVERFLOW                   0x00200000
    #define TSP_VD_FIFO_DISCON                      0x02000000
    #define TSP_VD_FIFO_OVERFLOW                    0x08000000
    #define TSP_AUB_FIFO_OVERFLOW                   0x10000000
    #define TSP_AU_FIFO_OVERFLOW                    0x20000000

    // only 25 bits supported in PVR address. 8 bytes address
    #define TSP_STR2MI2_ADDR_MASK                   0x07FFFFFF
    REG32                           TsRec_Head;                         // 0xbf802b40   0x50
    REG32                           TsRec_Mid_PVR1_WPTR;                // 0xbf802b48   0x52, PVR1 mid address & write point
    REG32                           TsRec_Tail;                         // 0xbf802b50   0x54

    REG16                           sw_mail_b0x1;                       // 0xbf802b58 0x56
    REG16                           sw_mail_b0x2;                       // 0xbf802b5c 0x57
    REG32                           _xbf802b58;                         // 0xbf802b60 ~ 0xbf802b64   0x58~0x59

    REG32                           reg15b4;                            // 0xbf802b68   0x5a
    #define TSP_SEC_CB_PVR2_DAMW_PROTECT_EN         0x00000002
    #define TSP_PVR_PID_BYPASS                      0x00000008          // Set 1 to bypass PID in record
    #define TSP_PVR_PID_BYPASS2                     0x00000010          // Set 1 to bypass PID in record2
    #define TSP_BD_AUD_EN                           0x00000020          // Set 1 to enable the BD audio stream recognization ( core /extend audio stream)
    #define TSP_AVFIFO_RD_EN                        0x00000080          // 0: AFIFO and VFIFO read are connected to MVD and MAD,  1: AFIFO and VFIFO read are controlled by registers (0x15B5[2:0])
    #define TSP_AVFIFO_RD                           0x00000100          // If AVFIFO_RD_EN is 1, set to 1, then set to 0 would issue a read strobe to AFIFO or VFIFO
    #define TSP_AVFIFO_SEL_VIDEO                    0x00000000
    #define TSP_AVFIFO_SEL_AUDIO                    0x00000200
    #define TSP_PVR_INVERT                          0x00001000          // Set 1 to enable data payload invert for PVR record
    #define TSP_PLY_FILE_INV_EN                     0x00002000          // Set 1 to enable data payload invert in pidflt0 file path
    #define TSP_PLY_TS_INV_EN                       0x00004000          // Set 1 to enable data payload invert in pidflt0 TS path
    #define TSP_FILEIN_BYTETIMER_ENABLE             0x00008000          // Set 1 to enable byte timer in ts_if0 TS path
    #define TSP_PVR1_PINGPONG                       0x00010000          // Set 1 to enable MIU addresses with pinpon mode
    #define TSP_TEI_SKIPE_PKT_PID0                  0x00040000          // Set 1 to skip error packets in pidflt0 TS path
    #define TSP_TEI_SKIPE_PKT_FILE                  0x00080000          // Set 1 to skip error packets in pidflt0 file path
    #define TSP_TEI_SKIPE_PKT_PID1                  0x00100000          // Set 1 to skip error packets in pidflt1 TS path
    #define TSP_64bit_PCR2_ld                       0x00800000          // Set 1 to load CNT_64B_2 (the second STC)
    #define TSP_cnt_33b_ld                          0x01000000          // Set 1 to load cnt_33b
    #define TSP_FORCE_SYNCBYTE                      0x02000000          // Set 1 to force sync byte (8'h47) in ts_if0 and ts_if1 path.
    #define TSP_SERIAL_EXT_SYNC_LT                  0x04000000          // Set 1 to detect serial-in sync without 8-cycle mode
    #define TSP_BURST_LEN_MASK                      0x18000000          // 00,01:    burst length = 4; 10,11: burst length = 1
    #define TSP_BURST_LEN_4                         0x08000000
    #define TSP_BURST_LEN_SHIFT                     27
    #define TSP_MATCH_PID_SRC_MASK                  0x60000000          // Select the source of pid filter number with hit pid and match pid number with scramble information, 00 : from pkt_demux0, 01 : from pkt_demux_file, 10 : from pkt_demux1, 11 : from pkt_demux2
    #define TSP_MATCH_PID_SRC_SHIFT                 29
        #define TSP_MATCH_PID_SRC_PKTDMX0           0
        #define TSP_MATCH_PID_SRC_PKTDMXFL          1
        #define TSP_MATCH_PID_SRC_PKTDMX1           2
    #define TSP_MATCH_PID_LD                        0x80000000          // Set 1 to load match pid number with scramble information from FILE PIDFLT

    REG32                           TSP_MATCH_PID_NUM;                  // 0xbf802b70   0x5c

    REG32                           TSP_IWB_WAIT;                       // 0xbf802b78   0x5e  // Wait count settings for IWB when TSP CPU i-cache is enabled.

    REG32                           Cpu_Base;                           // 0xbf802b80   0x60
    #define TSP_CPU_BASE_ADDR_MASK                  0x01FFFFFF

    REG32                           Qmem_Ibase;                         // 0xbf802b88   0x62

    REG32                           Qmem_Imask;                         // 0xbf802b90   0x64

    REG32                           Qmem_Dbase;                         // 0xbf802b98   0x66

    REG32                           Qmem_Dmask;                         // 0xbf802ba0   0x68

    REG32                           TSP_Debug;                          // 0xbf802ba8   0x6a
    #define TSP_DEBUG_MASK                          0x00FFFFFF

    REG32                           _xbf802bb0;                         // 0xbf802bb0   0x6c

    REG32                           TsFileIn_RPtr;                      // 0xbf802bb8   0x6e

    REG32                           TsFileIn_Timer;                     // 0xbf802bc0   0x70
    #define TSP_FILE_TIMER_MASK                     0x00FFFFFF
    REG32                           TsFileIn_Head;                      // 0xbf802bc8   0x72
    #define TSP_FILE_ADDR_MASK                      0x07FFFFFF
    REG32                           TsFileIn_Mid;                       // 0xbf802bd0   0x74

    REG32                           TsFileIn_Tail;                      // 0xbf802bd8   0x76

    REG32                           Dnld_Ctrl;                          // 0xbf802be0   0x78
    #define TSP_DNLD_ADDR_MASK                      0x0000FFFF
    #define TSP_DNLD_ADDR_SHFT                      0
    #define TSP_DNLD_ADDR_ALI_SHIFT                 4                   // Bit [11:4] of DMA_RADDR[19:0]
    #define TSP_DNLD_NUM_MASK                       0xFFFF0000
    #define TSP_DNLD_NUM_SHFT                       16

    REG32                           TSP_Ctrl;                           // 0xbf802be8   0x7a
    #define TSP_CTRL_CPU_EN                         0x00000001
    #define TSP_CTRL_SW_RST                         0x00000002
    #define TSP_CTRL_DNLD_START                     0x00000004
    #define TSP_CTRL_DNLD_DONE                      0x00000008          // See 0x78 for related information
    #define TSP_CTRL_TSFILE_EN                      0x00000010
    #define TSP_CTRL_R_PRIO                         0x00000020
    #define TSP_CTRL_W_PRIO                         0x00000040
    #define TSP_CTRL_ICACHE_EN                      0x00000100
    #define TSP_CTRL_CPU2MI_R_PRIO                  0x00000400
    #define TSP_CTRL_CPU2MI_W_PRIO                  0x00000800
    #define TSP_CTRL_I_EL                           0x00000000
    #define TSP_CTRL_I_BL                           0x00001000
    #define TSP_CTRL_D_EL                           0x00000000
    #define TSP_CTRL_D_BL                           0x00002000
    #define TSP_CTRL_NOEA_QMEM_ACK_DIS              0x00004000
    #define TSP_CTRL_MEM_TS_WORDER                  0x00008000
    #define TSP_SYNC_BYTE_MASK                      0x00FF0000
    #define TSP_SYNC_BYTE_SHIFT                     16

    REG32                           PKT_CNT;                            // 0xbf802bf0   0x7c
    #define TSP_PKT_CNT_MASK                        0x000000FF
    #define TSP_DBG_SEL_MASK                        0xFFFF0000
    #define TSP_DBG_SEL_SHIFT                       16

    REG16                           HwInt_Stat;                         // 0xbf802bf8   0x7e
    #define TSP_HWINT_STATUS_MASK                   0xFF00              // Tsp2hk_int enable bits.
    #define TSP_HWINT_TSP_PVR_TAIL0_STATUS          0x0100
    #define TSP_HWINT_TSP_PVR_MID0_STATUS           0x0200
    #define TSP_HWINT_TSP_HK_INT_FORCE_STATUS       0x0400
    #define TSP_HWINT_TSP_FILEIN_MID_INT_STATUS     0x0800
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_STATUS    0x1000
    #define TSP_HWINT_TSP_SW_INT_STATUS             0x2000
    #define TSP_HWINT_TSP_DMA_READ_DONE             0x4000
    #define TSP_HWINT_TSP_AV_PKT_ERR                0x8000

    #define TSP_HWINT_HW_PVR1_MASK                  (TSP_HWINT_TSP_PVR_TAIL0_STATUS | TSP_HWINT_TSP_PVR_MID0_STATUS)
    #define TSP_HWINT_ALL                           (TSP_HWINT_HW_PVR1_MASK | TSP_HWINT_TSP_SW_INT_STATUS)

    // 0x7f: TSP_CTRL1: hidden in HwInt_Stat
    REG16                           TSP_Ctrl1;                          // 0xbf802bfc   0x7f
    #define TSP_CTRL1_FILEIN_TIMER_ENABLE           0x0001
    #define TSP_CTRL1_TSP_FILE_NON_STOP             0x0002              //Set 1 to enable TSP file data read without timer check
    #define TSP_CTRL1_FILEIN_PAUSE                  0x0004
    #define TSP_CTRL1_STANDBY                       0x0080
    #define TSP_CTRL1_INT2NOEA                      0x0100
    #define TSP_CTRL1_INT2NOEA_FORCE                0x0200
    #define TSP_CTRL1_FORCE_XIU_WRDY                0x0400
    #define TSP_CTRL1_CMDQ_RESET                    0x0800
    #define TSP_CTRL1_DLEND_EN                      0x1000              // Set 1 to enable little-endian mode in TSP CPU
    #define TSP_CTRL1_PVR_CMD_QUEUE_ENABLE          0x2000
    #define TSP_CTRL1_DMA_RST                       0x8000

    //----------------------------------------------
    // 0xBF802C00 MIPS direct access
    //----------------------------------------------
    REG32                           MCU_Data0;                          // 0xbf802c00   0x00
    #define TSP_MCU_DATA_ALIVE                      TSP_MCU_CMD_ALIVE

    REG32                           PVR1_LPcr1;                         // 0xbf802c08   0x02

    REG32                           LPcr2;                              // 0xbf802c10   0x04

    REG32                           reg160C;                            // 0xbf802c18   0x06
    #define TSP_PVR1_LPCR1_WLD                      0x00000001          // Set 1 to load LPCR1 value
    #define TSP_PVR1_LPCR1_RLD                      0x00000002          // Set 1 to read LPCR1 value (Default: 1)
    #define TSP_LPCR2_WLD                           0x00000004          // Set 1 to load LPCR2 value
    #define TSP_LPCR2_RLD                           0x00000008          // Set 1 to read LPCR2 value (Default: 1)
    #define TSP_RECORD192_EN                        0x00000010          // 160C bit(5)enable TS packets with 192 bytes on record mode
    #define TSP_FILEIN192_EN                        0x00000020          // 160C bit(5)enable TS packets with 192 bytes on file-in mode
    #define TSP_RVU_TIMESTAMP_EN                    0x00000040
    #define TSP_ORZ_DMAW_PROT_EN                    0x00000080          // 160C bit(7) open RISC DMA write protection
    #define TSP_CLR_PIDFLT_BYTE_CNT                 0x00000100          // Clear pidflt0_file byte counter
    #define TSP_DOUBLE_BUF_DESC                     0x00004000          // 160d bit(6) remove buffer limitation, Force pinpong buffer to flush
    #define TSP_TIMESTAMP_RESET                     0x00008000          // 160d bit(7) reset timestamp, reset all file in path
    #define TSP_VQTX0_BLOCK_DIS                     0x00010000
    #define TSP_VQTX1_BLOCK_DIS                     0x00020000
    #define TSP_VQTX2_BLOCK_DIS                     0x00040000
    #define TSP_DIS_MIU_RQ                          0x00100000          // Disable miu R/W request for reset TSP usage
    #define TSP_RM_DMA_GLITCH                       0x00800000          // Fix sec_dma overflow glitch
    #define TSP_RESET_VFIFO                         0x01000000          // Reset VFIFO -- ECO Done
    #define TSP_RESET_AFIFO                         0x02000000          // Reset AFIFO -- ECO Done
    #define TSP_RESET_GDMA                          0x04000000          // Set 1 to reset GDMA bridge
    #define TSP_CLR_ALL_FLT_MATCH                   0x08000000          // Set 1 to clean all flt_match in a packet
    #define TSP_RESET_AFIFO2                        0x10000000
    #define TSP_RESET_VFIFO3D                       0x20000000
    #define TSP_PVR_WPRI_HIGH                       0x20000000
    #define TSP_OPT_ORACESS_TIMING                  0x80000000

    REG32                           PktChkSizeFilein;                   // 0xbf802c20   0x08
    #define TSP_PKT_SIZE_MASK                       0x000000ff
    #define TSP_PKT192_BLK_DIS_FIN                  0x00000100          // Set 1 to disable file-in timestamp block scheme
    #define TSP_AV_CLR                              0x00000200          // Clear AV FIFO overflow flag and in/out counter
    #define TSP_HW_STANDBY_MODE                     0x00000400          // Set 1 to disable all SRAM in TSP for low power mode automatically
    #define TSP_CNT_34B_DEFF_EN                     0x00020000          // Switch STC DIFF Mode (Output STC+DIFF to MVD and MAD)
    #define TSP_SYSTIME_MODE_STC64                  0x00080000          // Switch normal STC or STC diff
    #define TSP_SEC_DMA_BURST_EN                    0x00800000          // ECO bit for section DMA burst mode
    #define TSP_REMOVE_DUP_VIDEO_PKT                0x02000000          // Set 1 to remove duplicate video packet
    #define TSP_REMOVE_DUP_VIDEO3D_PKT              0x04000000          // Set 1 to remove duplicate video 3D packet
    #define TSP_REMOVE_DUP_AUDIO_PKT                0x08000000          // Set 1 to remove duplicate audio packet
    #define TSP_REMOVE_DUP_AUDIOB_PKT               0x10000000          // Set 1 to remove duplicate audio description packet

    #define TSP_REMOVE_DUP_AV_PKT                   (TSP_REMOVE_DUP_VIDEO_PKT|TSP_REMOVE_DUP_AUDIO_PKT|TSP_REMOVE_DUP_AUDIOB_PKT | TSP_REMOVE_DUP_VIDEO3D_PKT)

    REG32                           Dnld_Ctrl2;                         // 0xbf802c28   0x0a
    #define TSP_DMA_RADDR_MSB_MASK                  0x000000FF
    #define TSP_DMA_RADDR_MSB_SHIFT                 0
    //#define TSP_CMQ_WORD_EN                         0x00400000          // Set 1 to access CMDQ related registers in word.
    //#define TSP_RESET_PVR_MOBF                      0x04000000
    //#define TSP_RESET_FILEIN_MOBF                   0x08000000
    #define TSP_TSIF0_VPID_3D_BYPASS                0x0F000000          // bypass TS for matched video 3D pid
    #define TSP_VPID_3D_ERR_RM_EN                   0x10000000          // enable removing v3d err pkt
    #define TSP_PS_VID3D_EN                         0x40000000
    #define TSP_PREVENT_OVF_META                    0x80000000

    REG32                           TsPidScmbStatTsin;                  // 0xbf802c30   0x0c

    REG32                           _xbf802c38;                         // 0xbf802c38   0x0e

    REG32                           PCR64_2_L;                          // 0xbf802c40   0x10

    REG32                           PCR64_2_H;                          // 0xbf802c48   0x12

    #define TSP_DMAW_BND_MASK                       0xFFFFFFFFF
    REG32                           DMAW_LBND0;                         // 0xbf802c50   0x14

    REG32                           DMAW_UBND0;                         // 0xbf802c58   0x16

    REG32                           DMAW_LBND1;                         // 0xbf802c60   0x18

    REG32                           DMAW_UBND1;                         // 0xbf802c68   0x1A

    REG32                           DMAW_ERR_WADDR_SRC_SEL;                   //   0x1C ~ 0x1D
    #define TSP_CLR_NO_HIT_INT                       0x00000001         // set 1 clear all dma write function not hit interrupt
    #define DMAW_ERR_WADDR_SRC_SEL_MASK              0x0000001E
    #define DMAW_ERR_WADDR_SRC_SEL_SHIFT             1
    #define TSP_PVR1_DWMA_WADDR_ERR                  0x0
    #define TSP_SEC_DWMA_WADDR_ERR                   0x1
    #define TSP_PVR_CB_DWMA_WADDR_ERR                0x2
    #define TSP_VQTX0_DWMA_WADDR_ERR                 0x3
    #define TSP_VQTX1_DWMA_WADDR_ERR                 0x4
    #define TSP_ORZ_DWMA_WADDR_ERR                   0x5
    #define TSP_VQTX2_DWMA_WADDR_ERR                 0x6
    #define TSP_PVR2_DWMA_WADDR_ERR                  0x8
    #define TSP_CLR_SEC_DMAW_OVERFLOW                0x00000040
    #define TSP_APES_B_ERR_RM_EN                     0x00000080
    #define TSP_BLK_AF_SCRMB_BIT                     0x00000400

    REG32                           reg163C;                            // 0xbf802c78   0x1e

    #define TSP_CLR_SRC_MASK                        0x00070000
    #define TSP_CLR_SRC_SHIFT                       16
        #define TSP_CLR_SRC_CH_0                    1
        #define TSP_CLR_SRC_CH_FI                   2
        #define TSP_CLR_SRC_CH_1                    3
        #define TSP_CLR_SRC_CH_MMFI0                6
        #define TSP_CLR_SRC_CH_MMFI1                7
    #define TSP_DISCONTI_VD_CLR                     0x00080000  //Set 1 to clear video discontinuity count
    #define TSP_DISCONTI_AUD_CLR                    0x00200000  //Set 1 to clear audio discontinuity count
    #define TSP_DISCONTI_AUDB_CLR                   0x00400000  //Set 1 to clear videoB discontinuity count
    #define TSL_CLR_SRAM_COLLISION                  0x02000000
    #define TSP_TS_OUT_EN                           0x04000000  //set 1 to enable ts_out

    #define TSP_ALL_VALID_EN                        0x08000000
    #define TSP_PKT130_PUSI_EN                      0x10000000
    #define TSP_PKT130_TEI_EN                       0x20000000
    #define TSP_PKT130_ERR_CLR                      0x40000000
    #define TSP_PKT130_EN                           0x80000000 // file in only

    REG32                           VQ0_BASE;                           // 0xbf802c80   0x20
    REG32                           VQ0_CTRL;                           // 0xbf802c88   0x22
    #define TSP_VQ0_SIZE_208PK_MASK                 0x0000FFFF
    #define TSP_VQ0_SIZE_208PK_SHIFT                0
    #define TSP_VQ0_WR_THRESHOLD_MASK               0x000F0000
    #define TSP_VQ0_WR_THRESHOLD_SHIFT              16
    #define TSP_VQ0_PRIORTY_THRESHOLD_MASK          0x00F00000
    #define TSP_VQ0_PRIORTY_THRESHOL_SHIFT          20
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_MASK          0x0F000000
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT         24
    #define TSP_VQ0_RESET                           0x10000000
    #define TSP_VQ0_OVERFLOW_INT_EN                 0x40000000          // Enable the interrupt for overflow happened on Virtual Queue path
    #define TSP_VQ0_CLR_OVERFLOW_INT                0x80000000         // Clear the interrupt and the overflow flag

    REG32                           VQ_PIDFLT_CTRL;                    // 0xbf802c90   0x24
    #define TSP_REQ_VQ_RX_THRESHOLD_MASKE           0x000E0000
    #define TSP_REQ_VQ_RX_THRESHOLD_SHIFT           17
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN1            0x00000000
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN2            0x00020000
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN4            0x00040000
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN8            0x00060000
    #define TSP_PIDFLT0_OVF_INT_EN                  0x00400000
    #define TSP_PIDFLT0_CLR_OVF_INT                 0x00800000
    #define TSP_PIDFLT0_FILE_OVF_INT_EN             0x01000000
    #define TSP_PIDFLT0_FILE_CLR_OVF_INT            0x02000000
    #define TSP_PIDFLT1_OVF_INT_EN                  0x04000000
    #define TSP_PIDFLT1_CLR_OVF_INT                 0x08000000
    #define TSP_PIDFLT2_OVF_INT_EN                  0x10000000
    #define TSP_PIDFLT2_CLR_OVF_INT                 0x20000000
    #define TSP_PIDFLT_CB_OVF_INT_EN                0x40000000
    #define TSP_PIDFLT_CB_CLR_OVF_INT               0x80000000

    REG32                           MOBF_PVR1_Index;                    // 0xbf3a2c98   0x26
    #define TSP_MOBF_PVR1_INDEX0_MASK               0x0000000F
    #define TSP_MOBF_PVR1_INDEX0_SHIFT              0
    #define TSP_MOBF_PVR1_INDEX1_MASK               0x000F0000
    #define TSP_MOBF_PVR1_INDEX1_SHIFT              16

    REG32                           MOBF_PVR2_Index;                    // 0xbf3a2cA0   0x28
    #define TSP_MOBF_PVR2_INDEX0_MASK               0x0000000F
    #define TSP_MOBF_PVR2_INDEX0_SHIFT              0
    #define TSP_MOBF_PVR2_INDEX1_MASK               0x000F0000
    #define TSP_MOBF_PVR2_INDEX1_SHIFT              16

    REG32                           DMAW_LBND2;                         // 0xbf802ca8   0x2a

    REG32                           DMAW_UBND2;                         // 0xbf802cb0   0x2c

    REG32                           DMAW_LBND3;                         // 0xbf802cb8   0x2e          //reserved

    REG32                           DMAW_UBND3;                         // 0xbf802cc0   0x30          //reserved

    REG32                           DMAW_LBND4;                         // 0xbf802cc8   0x32

    REG32                           DMAW_UBND4;                         // 0xbf802cd0   0x34

    REG32                           ORZ_DMAW_LBND;                      // 0xbf802cd8   0x36
    #define TSP_ORZ_DMAW_LBND_MASK                  0xffffffff
    REG32                           ORZ_DMAW_UBND;                      // 0xbf802ce0   0x38
    #define TSP_ORZ_DMAW_UBND_MASK                  0xffffffff
    REG32                           _xbf802ce8_xbf802cec;               // 0xbf802ce8_0xbf802cec  0x3a~0x3b

    REG32                           HWPCR0_L;                           // 0xbf802cf0   0x3c
    REG32                           HWPCR0_H;                           // 0xbf802cf8   0x3e

    REG32                           CA_CTRL;                            // 0xbf802d00   0x40
    #define TSP_CA_CTRL_MASK                        0xffffffff
    #define TSP_CA0_CTRL_MASK                       0x00003077
    #define TSP_CA0_INPUT_TSIF0_LIVEIN              0x00000001
    #define TSP_CA0_INPUT_TSIF0_FILEIN              0x00000002
    #define TSP_CA0_INPUT_TSIF1                     0x00000004
    #define TSP_CA0_OUTPUT_PKTDMX0_LIVE             0x00000010
    #define TSP_CA0_OUTPUT_PKTDMX0_FILE             0x00000020
    #define TSP_CA0_OUTPUT_PKTDMX1                  0x00000040
    #define TSP_CA0_INPUT_TSIF2                     0x00001000
    #define TSP_CA0_OUTPUT_PKTDMX2                  0x00002000
    #define TSP_CA0_OUTPUT_CA2                      0x00004000

    #define TSP_CA1_CTRL_MASK                       0x77308000
    #define TSP_CA1_OUTPUT_CA2                      0x00008000
    #define TSP_CA1_INPUT_TSIF2                     0x00100000
    #define TSP_CA1_OUTPUT_PKTDMX2                  0x00200000

    #define TSP_CA1_INPUT_TSIF0_LIVEIN              0x01000000
    #define TSP_CA1_INPUT_TSIF0_FILEIN              0x02000000
    #define TSP_CA1_INPUT_TSIF1                     0x04000000
    #define TSP_CA1_OUTPUT_PKTDMX0_LIVE             0x10000000
    #define TSP_CA1_OUTPUT_PKTDMX0_FILE             0x20000000
    #define TSP_CA1_OUTPUT_PKTDMX1                  0x40000000

    REG32                           REG_ONEWAY;                         // 0xbf802d08   0x42
    #define TSP_ONEWAY_REC_DISABLE                  0x00000001          // Disable PVR
    #define TSP_ONEWAY_PVR_PORT                     0x00000002          // Oneway for PVR buffer
    #define TSP_ONEWAY_LOAD_FW_PORT                 0x00000004          // Oneway for f/w load address

    REG32                           HWPCR1_L;                           // 0xbf802d10   0x44
    REG32                           HWPCR1_H;                           // 0xbf802d18   0x46

    REG32                           LPCR_CB;                            // 0xbf802d20   0x48

    REG32                           CHBW_BUF_HEAD;                      // 0xbf802d28   0x4a, channel browser

    REG32                           CHBW_BUF_MID_Wptr;                  // 0xbf802d30   0x4C, channel browser, Mid & Wptr share same register

    REG32                           CHBW_BUF_TAIL;                      // 0xbf802d38   0x4E, channel browser

    REG32                           FIFO_Src;                           // 0xbf802d40   0x50
    #define TSP_AUD_SRC_MASK                        0x00000007
    #define TSP_AUD_SRC_SHIFT                       0
        #define TSP_SRC_FROM_PKTDMX0                0x00000001
        #define TSP_SRC_FROM_PKTDMXFL               0x00000002
        #define TSP_SRC_FROM_PKTDMX1                0x00000003
        #define TSP_SRC_FROM_PKTDMX2                0x00000004
        #define TSP_SRC_FROM_MMFI0                  0x00000006
        #define TSP_SRC_FROM_MMFI1                  0x00000007
    #define TSP_AUDB_SRC_MASK                       0x00000038
    #define TSP_AUDB_SRC_SHIFT                      3
    #define TSP_VID_SRC_MASK                        0x000001C0
    #define TSP_VID_SRC_SHIFT                       6
    #define TSP_VID3D_SRC_MASK                      0x00000E00
    #define TSP_VID3D_SRC_SHIFT                     9
    #define TSP_PVR1_SRC_MASK                       0x00007000
    #define TSP_PVR1_SRC_SHIFT                      12
    #define TSP_PVR2_SRC_MASK                       0x00038000
    #define TSP_PVR2_SRC_SHIFT                      15
    #define TSP_PCR0_SRC_MASK                       0x001C0000
    #define TSP_PCR0_SRC_SHIFT                      18
    #define TSP_TEI_SKIP_PKT_PCR0                   0x01000000
    #define TSP_PCR0_RESET                          0x02000000
    #define TSP_PCR0_INT_CLR                        0x04000000
    #define TSP_PCR0_READ                           0x08000000

    REG32                           STC_DIFF_BUF;                       // 0xbf802d48   0x52

    REG32                           STC_DIFF_BUF_H;                     // 0xbf802d50   0x54
    #define TSP_STC_DIFF_BUF_H_MASK                 0x0000007F
    #define TSP_STC_DIFF_BUF_H_AHIFT                0

    REG32                           VQ1_Base;                           // 0xbf802d58   0x56

    REG32                           Hw_Config5;                         // 0xbf802d60   0x58

    REG32                           CH_BW_CTRL;                         // 0xbf802d68   0x5a
    #define TSP_CH_BW_WP_LD                         0x00000100

    REG32                           VQ1_Config;                         // 0xbf802d70   0x5C
    #define TSP_VQ1_SIZE_208BYTE_MASK               0x0000ffff
    #define TSP_VQ1_SIZE_208BYTE_SHIFT              0
    #define TSP_VQ1_WR_THRESHOLD_MASK               0x000F0000
    #define TSP_VQ1_WR_THRESHOLD_SHIFT              16
    #define TSP_VQ1_PRI_THRESHOLD_MASK              0x00F00000
    #define TSP_VQ1_PRI_THRESHOLD_SHIFT             20
    #define TSP_VQ1_FORCEFIRE_CNT_1K_MASK           0x0F000000
    #define TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT          24
    #define TSP_VQ1_RESET                           0x10000000
    #define TSP_VQ1_OVF_INT_EN                      0x40000000
    #define TSP_VQ1_CLR_OVF_INT                     0x80000000

    REG32                           VQ2_Base;                           // 0xbf802d78   0x5E

    REG32                           Pkt_Info3;                          // 0xbf802d80   0x60
    #define TSP_AFIFOC_STATUS                       0x0000000F
    #define TSP_AFIFOC_STATUS_SHFT                  0
    #define TSP_AFIFOD_STATUS                       0x000000F0
    #define TSP_AFIFOD_STATUS_SHFT                  4

    REG32                           Bist_Fail;                          // 0xbf802d88   0x62
    #define TSP_BIST_FAIL_STATUS_MASK               0x00FF0000
    #define TSP_BIST_FAIL_STATUS_SRAM1P192x8_MASK   0x00070000
    #define TSP_BIST_FAIL_STATUS_SRAM2P512x32w8     0x00080000
    #define TSP_BIST_FAIL_STATUS_SRAM2P16x128_MASK  0x00600000
    #define TSP_BIST_FAIL_STATUS_SRAM1P2048x32w8    0x00800000
    #define TSP_BIST_FAIL_STATUS_SRAM1P1024x32w8    0x01000000
    #define TSP_BIST_FAIL_STATUS_SRAM1P512x20       0x00200000

    REG32                           VQ2_Config;                         // 0xbf802d90   0x64
    #define TSP_VQ2_SIZE_208BYTE_MASK               0x0000ffff
    #define TSP_VQ2_SIZE_208BYTE_SHIFT              0
    #define TSP_VQ2_WR_THRESHOLD_MASK               0x000F0000
    #define TSP_VQ2_WR_THRESHOLD_SHIFT              16
    #define TSP_VQ2_PRI_THRESHOLD_MASK              0x00F00000
    #define TSP_VQ2_PRI_THRESHOLD_SHIFT             20
    #define TSP_VQ2_FORCEFIRE_CNT_1K_MASK           0x0F000000
    #define TSP_VQ2_FORCEFIRE_CNT_1K_SHIFT          24
    #define TSP_VQ2_RESET                           0x10000000
    #define TSP_VQ2_OVF_INT_EN                      0x40000000
    #define TSP_VQ2_CLR_OVF_INT                     0x80000000

    REG32                           VQ_STATUS;                          // 0xbf802d98   0x66
    #define TSP_VQ_STATUS_MASK                      0xFFFFFFFF
    #define TSP_VQ_STATUS_SHIFT                     0
    #define TSP_VQ0_STATUS_READ_EVER_FULL           0x00001000
    #define TSP_VQ0_STATUS_READ_EVER_OVERFLOW       0x00002000
    #define TSP_VQ0_STATUS_EMPTY                    0x00004000
    #define TSP_VQ0_STATUS_READ_BUSY                0x00008000
    #define TSP_VQ1_STATUS_READ_EVER_FULL           0x00010000
    #define TSP_VQ1_STATUS_READ_EVER_OVERFLOW       0x00020000
    #define TSP_VQ1_STATUS_EMPTY                    0x00040000
    #define TSP_VQ1_STATUS_READ_BUSY                0x00080000
    #define TSP_VQ2_STATUS_READ_EVER_FULL           0x00100000
    #define TSP_VQ2_STATUS_READ_EVER_OVERFLOW       0x00200000
    #define TSP_VQ2_STATUS_EMPTY                    0x00400000
    #define TSP_VQ2_STATUS_READ_BUSY                0x00800000
    #define TSP_VQ3_STATUS_READ_EVER_FULL           0x01000000
    #define TSP_VQ3_STATUS_READ_EVER_OVERFLOW       0x02000000
    #define TSP_VQ3_STATUS_EMPTY                    0x04000000
    #define TSP_VQ3_STATUS_READ_BUSY                0x08000000
    #define TSP_VQ0_STATUS_TX_OVERFLOW              0x10000000
    #define TSP_VQ1_STATUS_TX_OVERFLOW              0x20000000
    #define TSP_VQ2_STATUS_TX_OVERFLOW              0x40000000
    #define TSP_VQ3_STATUS_TX_OVERFLOW              0x80000000

    REG32                           DM2MI_WAddr_Err;                    // 0xbf802da0   0x68  , DM2MI_WADDR_ERR0

    REG32                           ORZ_DMAW_WAddr_Err;                 // 0xbf802da8   0x6a  , ORZ_WADDR_ERR0

    REG16                           SwInt_Stat1_L;                      // 0xbf802dB0   0x6c
    #define TSP_HWINT2_EN_MASK                      0x00FF
    #define TSP_HWINT2_EN_SHIFT                     0
    #define TSP_HWINT2_STATUS_MASK                  0xFF00
    #define TSP_HWINT2_STATUS_SHIFT                 8
    #define TSP_HWINT2_PCR1_UPDATE_END              0x0400
    #define TSP_HWINT2_PCR0_UPDATE_END              0x0800
    #define TSP_HWINT2_PVRCB_MEET_MID_TAIL          0x1000
    #define TSP_HWINT2_ALL_DMA_WADDR_NOT_IN_PROCT_Z 0x2000
    #define TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW     0x4000
    #define TSP_HWINT2_PVR2_MID_TAIL_STATUS         0x8000

    #define TSP_HWINT_HW_PVRCB_MASK                 TSP_HWINT2_PVRCB_MEET_MID_TAIL
    #define TSP_HWINT_HW_PVR2_MASK                  TSP_HWINT2_PVR2_MID_TAIL_STATUS
    #define TSP_HWINT2_ALL                          (TSP_HWINT_HW_PVRCB_MASK|TSP_HWINT_HW_PVR2_MASK|TSP_HWINT2_PCR0_UPDATE_END|TSP_HWINT2_PCR1_UPDATE_END)

    #define TSP_SWINT1_L_SHFT                       16
    #define TSP_SWINT1_L_MASK                       0xFFFF0000

    REG16                           SwInt_Stat1_M;
    REG32                           SwInt_Stat1_H;                     // 0xbf802dB8   0x6e
    #define TSP_SWINT1_H_SHFT       0
    #define TSP_SWINT1_H_MASK       0x0000FFFF

    REG32                           TimeStamp_FileIn;                   // 0xbf802dC0   0x70

    REG32                           HW2_Config3;                        // 0xbf802dC0   0x72
    #define TSP_WADDR_ERR_SRC_SEL_MASK              0x00000006
    #define TSP_WADDR_ERR_SRC_SEL_SHIFT             1
    #define TSP_WADDR_ERR_SRC_PVR                   0x00000000
    #define TSP_WADDR_ERR_SRC_VQ                    0x00000002
    #define TSP_WADDR_ERR_SRC_SEC_CB                0x00000004
    #define TSP_RM_OVF_GLITCH                       0x00000008
    #define TSP_FILEIN_RADDR_READ                   0x00000010
    #define TSP_DUP_PKT_CNT_CLR                     0x00000040
    #define TSP_REC_AT_SYNC_DIS                     0x00000100
    #define TSP_PVR1_ALIGN_EN                       0x00000200
    #define TSP_REC_FORCE_SYNC_EN                   0x00000400
    #define TSP_RM_PKT_DEMUX_PIPE                   0x00000800
    #define TSP_VQ_EN                               0x00004000
    #define TSP_VQ2PINGPONG_EN                      0x00008000
    #define TSP_PVR1_REC_ALL_EN                     0x00010000
    #define TSP_PVR2_REC_ALL_EN                     0x00020000
    #define TSP_DMA_FLUSH_EN                        0x00040000          //PVR1, PVR2 dma flush
    #define TSP_REC_ALL_OLD                         0x00080000
    #define TSP_RESET_AFIFO3                        0x00400000
    #define TSP_RESET_AFIFO4                        0x00800000
    #define TSP_TSIF0_CLK_STAMP_27_EN               0x01000000
    #define TSP_PVR1_CLK_STAMP_27_EN                0x02000000
    #define TSP_PVR2_CLK_STAMP_27_EN                0x04000000
    #define TSP_REC_NULL                            0x40000000          // No used

    REG32                           VQ3_BASE;                           // 0xbf802dC0   0x74

    REG32                           VQ3_Config;                         // 0xbf802dC0   0x76

    REG32                           VQ_RX_Status;                       // 0xbf802dC0   0x78
    #define VQ_RX_ARBITER_MODE_MASK                 0x0000000F
    #define VQ_RX_ARBITER_MODE_SHIFT                0
    #define VQ_RX0_PRI_MASK                         0x000000F0
    #define VQ_RX0_PRI_SHIFT                        4
    #define VQ_RX1_PRI_MASK                         0x00000F00
    #define VQ_RX1_PRI_SHIFT                        8
    #define VQ_RX2_PRI_MASK                         0x0000F000
    #define VQ_RX2_PRI_SHIFT                        12

    REG32                           _xbf802dC0;                         // 0xbf802dC0   0x7a

    REG32                           MCU_Data1;                          // 0xbf802dC0   0x7c
} REG_Ctrl;

// TSP part 2
typedef struct _REG_Ctrl2
{
    REG16                           Qmem_Dbg;                          // 0xbf803ac0   0x70
    #define QMEM_DBG_MODE                           0x0001
    #define QMEM_DBG_TSP_SEL_SRAM                   0x0002
    REG16                           Qmem_Dbg_RAddr;                    // 0xbf803ac4   0x71
    #define QMEM_DBG_RADDR_MASK                     0xFFFF
    REG32                           Qmem_Dbg_RD ;                      // 0xbf803ac8~0xbf803acc   0x72~0x73

} REG_Ctrl2;

typedef struct _REG_Ctrl3
{
    REG16                           PktConverterCfg[3];         // 0x10~12
    #define INPUT_MODE_MASK                                     0x0007
    #define INPUT_MODE_SHIF                                     0
    #define FORCE_SYNC_0X47                                     0x0008
    #define BYPASS_PKT_CONVERTER                                0x0010
    #define BYPASS_SRC_ID_PARSER                                0x0020

    REG16                           _reserved_TSP3_13;          // 0x13

    REG16                           HW3_Cfg0;                   // 0x14
    #define PREVENT_SRAM_COLLISION                              0x0001
    #define PUSI_THREE_BYTE_MODE                                0x0002
    #define PCR0_SRC_MASK                                       0x0F00
    #define PCR0_SRC_SHIFT                                      8

    REG16                           HW3_Cfg1;                   // 0x15
    #define MASK_SCR_VID_EN                                     0x0001
    #define MASK_SCR_VID_3D_EN                                  0x0002
    #define MASK_SCR_AUD_EN                                     0x0004
    #define MASK_SCR_AUD_B_EN                                   0x0008
    #define MASK_SCR_PVR1_EN                                    0x0040
    #define MASK_SCR_PVR2_EN                                    0x0080
    #define RST_CC_MODE                                         0x0100
    #define DIS_CNTR_INC_BY_PL                                  0x0200
    #define BYPASS_TIMESTAMP_SEL0                               0x0400
    #define BYPASS_TIMESTAMP_SEL1                               0x0800

    REG32                          _reserved_TSP3_16_19[2];     // 0x16~17, 0x18~19

    REG32                          PIDFLR_PCR[1];               // 0x1a-0x1b
    #define TSP_PIDFLT_PCR_PID_MASK                             0x00001fff
    #define TSP_PIDFLT_PCR_EN                                   0x00008000
    #define TSP_PIDFLT_PCR_SOURCE_MASK                          0x000F0000
    #define TSP_PIDFLT_PCR_SOURCE_SHIFT                         16

    REG32                          _reserved_TSP3_1c_1f[2];     // 0x1c~0x1f

    REG16                          HW_Semaphore0;               // 0x20
    REG16                          HW_Semaphore1;               // 0x21
    REG16                          HW_Semaphore2;               // 0x22

    REG16                          HWeco0;                      // 0x23
    #define                        HW_ECO_RVU                   0x0001
    #define                        HW_ECO_NEW_SYNCP_IN_ECO      0x0002
    #define                        HW_ECO_BURST_NEW_MODE0       0x0004
    #define                        HW_ECO_BURST_NEW_MODE1       0x0008
    #define                        HW_ECO_FIX_FIQ_RESDEADLOCK   0x0010
    #define                        HW_ECO_FIX_SEC_NULLPKT_ERR   0x0020

    REG16                          HWeco1;                      // 0x24
    REG16                          ModeCfg;                     // 0x25
    #define TSP_3WIRE_SERIAL_MODE_MASK                          0x001F           //set 1 to enable 3 wire serial in mode: Combine valid and clk.Valid always 1 and gated clk when no data in
    #define TSP_3WIRE_SERIAL_TSIF0                              0x0001
    #define TSP_3WIRE_SERIAL_TSIF1                              0x0002
    #define TSP_3WIRE_SERIAL_TSIFFI                             0x0010
    #define TSP_NEW_OVERFLOW_MODE                               0x0100            // 1: new dma_overflow 0:old dma_overflow
    #define TSP_NON_188_CNT_MODE                                0x0200

    REG16                          _reserved_TSP3_26_27[2];      // 0x26~27

    REG16                          SyncByte_tsif0[4];           // 0x28~2b
    #define TSP_SYNC_BYTE0_MAASK0                               0x00FF
    #define TSP_SYNC_BYTE0_MAASK1                               0xFF00
    REG16                          SourceId_tsif0[2];           // 0x2c~2d
    #define TSP_SRCID_MASK0                                     0x000F
    #define TSP_SRCID_MASK1                                     0x00F0
    #define TSP_SRCID_MASK2                                     0x0F00
    #define TSP_SRCID_MASK3                                     0xF000
    REG16                          SyncByte_file[4];
    REG16                          SourceId_file[2];
    REG16                          SyncByte_tsif1[4];
    REG16                          SourceId_tsif1[2];
} REG_Ctrl3;

// TSP part 4
typedef struct _REG_Ctrl4
{
    REG16                               Overflow0;                          // 0xbf803900   0x00
    #define PID_HIT_0_EVER_OVERFLOW                 0x0001
    #define PID_HIT_1_EVER_OVERFLOW                 0x0002
    #define PID_HIT_FILE_EVER_OVERFLOW              0x0008
    #define AFIFO_EVER_OVERFLOW                     0x0020
    #define AFIFOB_EVER_OVERFLOW                    0x0040
    #define VFIFO_EVER_OVERFLOW                     0x0080
    #define V3DFIFO_EVER_OVERFLOW                   0x0100
    #define PVR_1_EVER_OVERFLOW                     0x0200
    #define PVR_2_EVER_OVERFLOW                     0x0400
    #define VQ_TX0_EVER_OVERFLOW                    0x1000
    #define VQ_TX1_EVER_OVERFLOW                    0x2000
    #define VQ_TX2_EVER_OVERFLOW                    0x4000

    REG16                               Overflow1;                          // 0xbf803904   0x01
    #define SEC_DMAW_OVERFLOW                       0x0004
    #define SEC_SINGLE_EVER_OVERFLOW                0x0002
    #define SEC_PINGPONG_EVER_OVERFLOW              0x0001

    REG16                               FifoStatus;                         // 0xbf803908   0x02
    #define AFIFO_STATUS_MASK                       0x000F
    #define AFIFO_STATUS_SHFT                       0
    #define AFIFOB_STATUS_MASK                      0x00F0
    #define AFIFOB_STATUS_SHFT                      4
    #define VFIFO_STATUS_MASK                       0x0F00
    #define VFIFO_STATUS_SHFT                       8
    #define V3DFIFO_STATUS_MASK                     0xF000
    #define V3DFIFO_STATUS_SHFT                     12

    REG16                               PvrFifoStatus;                       // 0xbf80390C   0x03
    #define PVR_1_STATUS_MASK                       0x000F
    #define PVR_1_STATUS_SHFT                       0
    #define PVR_2_STATUS_MASK                       0x00F0
    #define PVR_2_STATUS_SHFT                       4

    REG16                               VQTxFifoStatus;                      // 0xbf803910   0x04
    #define VQ_TX0_STATUS_MASK                      0x000F
    #define VQ_TX0_STATUS_SHFT                      0
    #define VQ_TX1_STATUS_MASK                      0x0F00
    #define VQ_TX1_STATUS_SHFT                      8

    REG16                               PktCnt_video;                        // 0x05
    REG16                               PktCnt_v3d;                          // 0x06
    REG16                               PktCnt_aud;                          // 0x07
    REG16                               PktCnt_audB;                         // 0x08
    REG16                               PktCnt_audC;                         // 0x09
    REG16                               PktCnt_audD;                         // 0x0a
    REG16                               Reserved[0xd-0xb];
    REG16                               LockedPktCnt;                        // 0x0d
    REG16                               AVPktCnt;                            // 0x0e

    REG16                               PktErrStatus;                        // 0xbf80392C   0x0x0f
    REG16                               PidMatched0;                         // 0xbf803930   0x10
    REG16                               PidMatched1;                         // 0xbf803934   0x11
    REG16                               PidMatched2;                         // 0xbf803938   0x12
    REG16                               PidMatched3;                         // 0xbf80393C   0x13
    REG16                               dummy[2];                            // 0x14~0x15
    REG16                               Sram2p_collision;                    // 0x16
    #define SRAM_COLLISION_BY_SW                0x1000
    #define SRAM_COLLISION_BY_HW                0x2000
    #define SECFLT_SRAM1_EVER_COLLISION         0x4000
    #define SECFLT_SRAM0_EVER_COLLISION         0x8000
    REG16                               AVPktCnt1;                          //for vid_3d/audb                0x17
    REG16                               ErrPktCnt;                          //use reg_err_pkt_src_sel      0x18
    REG16                               AVPktCnt2;                          //for audc/audd                  0x19

    REG16                               EverUnlockStatus;                   // 0x1a
    #define EVER_UNLOCK_TS0                     0x0001      // set 1 mean there are unlock pkts
    #define EVER_UNLOCK_TS1                     0x0002
    #define EVER_UNLOCK_TS2                     0x0004

    REG16                               Overflow2;                          // 0xbf803904   0x1b
    #define PC_EVER_OVERFLOW_0                  0x0001
    #define PC_EVER_OVERFLOW_FILE               0x0002
    #define PC_EVER_OVERFLOW_1                  0x0004
    #define PC_EVER_OVERFLOW_2                  0x0008

    REG16                               dummy1[0x70-0x1c];
    REG16                               ErrPktSrcSel;                       //select source of ErrPktCnt  0x70
    #define ERR_PKT_SRC_TS0                     0x0001
    #define ERR_PKT_SRC_FILE                    0x0002
    #define ERR_PKT_SRC_TS1                     0x0003
    #define ERR_PKT_SRC_TS2                     0x0004
    #define ERR_PKT_SRC_MMFI0                   0x0005
    #define ERR_PKT_SRC_MMFI1                   0x0006

    REG16                               ErrPktCntLoad;                      // 0x71
    #define ERR_PKT_CNT_0_LOAD                  0x0001
    #define ERR_PKT_CNT_FILE_LOAD               0x0002
    #define ERR_PKT_CNT_1_LOAD                  0x0004
    #define ERR_PKT_CNT_2_LOAD                  0x0008
    #define ERR_PKT_CNT_MMFI0_LOAD              0x0010
    #define ERR_PKT_CNT_MMFI1_LOAD              0x0020

    REG16                               ErrPktCntClr;                       // 0x72
    #define ERR_PKT_CNT_0_CLR                   0x0001
    #define ERR_PKT_CNT_FILE_CLR                0x0002
    #define ERR_PKT_CNT_1_CLR                   0x0004
    #define ERR_PKT_CNT_2_CLR                   0x0008
    #define ERR_PKT_CNT_MMFI0_CLR               0x0010
    #define ERR_PKT_CNT_MMFI1_CLR               0x0020

    REG16                               dummy2[0x78-0x73];                    // 0x73~0x77
    REG16                               AudCSrc;        // ??                                   //0x78
    REG16                               dummy3;
    REG16                               PktCntLoad;                             // 0x7a
    #define LOCK_PKT_CNT_0_LOAD                 0x0001
    #define LOCK_PKT_CNT_1_LOAD                 0x0002
    #define LOCK_PKT_CNT_2_LOAD                 0x0004
    #define LOCK_PKT_CNT_CB_LOAD                0x0008
    #define LOCK_PKT_CNT_FI_LOAD                0x0010

    #define V_PKT_CNT_LOAD                      0x0100
    #define V3D_PKT_CNT_LOAD                    0x0200
    #define AUD_PKT_CNT_LOAD                    0x0400
    #define AUDB_PKT_CNT_LOAD                   0x0800
    #define AUDC_PKT_CNT_LOAD                   0x1000
    #define AUDD_PKT_CNT_LOAD                   0x2000

    REG16                               PktCntLoad1;                             // 0x7b
    #define V_DROP_PKT_CNT_LOAD                 0x0001
    #define V3D_DROP_PKT_CNT_LOAD               0x0002
    #define AUD_DROP_PKT_CNT_LOAD               0x0004
    #define AUDB_DROP_PKT_CNT_LOAD              0x0008
    #define AUDC_DROP_PKT_CNT_LOAD              0x0010
    #define AUDD_DROP_PKT_CNT_LOAD              0x0020

    #define V_DIS_CNTR_PKT_CNT_LOAD             0x0100
    #define V3D_DIS_CNTR_PKT_CNT_LOAD           0x0200
    #define AUD_DIS_CNTR_PKT_CNT_LOAD           0x0400
    #define AUDB_DIS_CNTR_PKT_CNT_LOAD          0x0800
    #define AUDC_DIS_CNTR_PKT_CNT_LOAD          0x1000
    #define AUDD_DIS_CNTR_PKT_CNT_LOAD          0x2000


    REG16                               PktCntClr;                             // 0x7c
    #define LOCK_PKT_CNT_0_CLR                  0x0001
    #define LOCK_PKT_CNT_1_CLR                  0x0002
    #define LOCK_PKT_CNT_2_CLR                  0x0004
    #define LOCK_PKT_CNT_CB_CLR                 0x0008
    #define LOCK_PKT_CNT_FI_CLR                 0x0010

    #define V_PKT_CNT_CLR                       0x0100
    #define V3D_PKT_CNT_CLR        0x0200
    #define AUD_PKT_CNT_CLR                     0x0400
    #define AUDB_PKT_CNT_CLR                    0x0800

    REG16                               PktCntClr1;                             // 0x7d
    #define V_DROP_PKT_CNT_CLR                  0x0001
    #define V3D_DROP_PKT_CNT_CLR        0x0002
    #define AUD_DROP_PKT_CNT_CLR                0x0004
    #define AUDB_DROP_PKT_CNT_CLR               0x0008

    #define V_DIS_CNTR_PKT_CNT_CLR              0x0100
    #define V3D_DIS_CNTR_PKT_CNT_CLR        0x0200
    #define AUD_DIS_CNTR_PKT_CNT_CLR            0x0400
    #define AUDB_DIS_CNTR_PKT_CNT_CLR           0x0800

    REG16                               PktCntSrc;                             // 0x7e
    #define VID_SRC_MASK                        0x0007
    #define VID_SRC_SHIFT                       0
    #define V3D_SRC_MASK    0x0031
    #define V3D_SRC_SHIFT   3
    #define AUD_SRC_MASK                        0x01C0
    #define AUD_SRC_SHIFT                       6
    #define AUDB_SRC_MASK                       0x0E00
    #define AUDB_SRC_SHIFT                      9

    REG16                               DebugSrcSel;                            // 0x7f
    #define SRC_SEL_MASK                        0x0001
    #define DROP_PKT_MODE_MASK                  0x0002
    #define PIDFLT_SRC_SEL_MASK                 0x001C
    #define TSIF_SRC_SEL_MASK                   0x00E0
    #define TSIF_SRC_SEL_SHIFT                  5
    #define TSIF_SRC_SEL_TSIF0                  0x000
    #define TSIF_SRC_SEL_TSIF1                  0x001
    #define TSIF_SRC_SEL_TSIF_FI                0x004

    #define AV_PKT_SRC_SEL                      0x0100
    #define AV_PKT_SRC_SEL_MASK                 0x0100
    #define AV_PKT_SRC_SEL_SHIFT                8
    #define AV_PKT_SRC_VID                      0x0
    #define AV_PKT_SRC_AUD                      0x1
    #define AV_PKT_SRC_V3D          0x0
    #define AV_PKT_SRC_AUDB                     0x1
    #define CLR_SRC_MASK                        0x0E00

}REG_Ctrl4;

// TSP part 4
typedef struct _REG_Ctrl5
{
    REG16                               ATS_Adj_Period;                             // 0x00
    #define TSP_ATS_ADJ_PERIOD_MASK                     0x000F

    REG16                               AtsCfg;                                     // 0x01
    #define TSP_ATS_MODE_FI_ENABLE                      0x0001
    #define TSP_ATS_OFFSET_FI_ENABLE                    0x0002
    #define TSP_ATS_OFFSET_FI_SHIFT                     8
    #define TSP_ATS_OFFSET_FI_MASK                      0x0F00
    #define TSP_ATS_OFFSET_FI_POSITIVE                  0x0000
    #define TSP_ATS_OFFSET_FI_NEGATIVE                  0x1000

    REG16                               Ts_If_Fi_Cfg;                               // 0x02
    #define TSP_FIIF_EN                                 0x0001
    #define TSP_FIIF_DATA_SWAP                          0x0002
    #define TSP_FIIF_P_SEL                              0x0004
    #define TSP_FIIF_EXT_SYNC_SEL                       0x0008
    #define TSP_FIIF_MUX_MASK                           0x0010
        #define TSP_FIIF_MUX_FILE_PATH                  0x0000
        #define TSP_FIIF_MUX_LIVE_PATH                  0x0010
    #define TSP_PKT_CHK_SIZE_FI_MASK                    0xFF00
    #define TSP_PKT_CHK_SIZE_FI_SHIFT                   8

    REG16                               S2PCfg;                                     //  0x03
    #define TSP_MATCH_PID_SEL_MASK                      0x000F                      // 0: #0~#31, 1: #32~#63, 2: #64~#95, 3: #96~#127
    #define TSP_MATCH_PID_SEL_SHIFT                     0

    REG16                               S2PCfg1;                                    //  0x04
    #define TSP_S2PCFG1_TSIF_0_TSO_BLK_EN               0x0100
    #define TSP_S2PCFG1_TSIF_1_TSO_BLK_EN               0x0200
    #define TSP_S2PCFG1_TSIF_FI_TSO_BLK_EN              0x0800
    #define TSP_S2PCFG1_WB_FSM_RST                      0x1000
    #define TSP_S2PCFG1_WB_FSM_RST_FINISHED             0x2000

    REG16                               TSP5_Reserve_5;                           // 0x05
    REG16                               TSP5_Eco;                                 // 0x06
    #define TSP_192_TIMER_0_EN                          0x0001
    REG16                               TSP5_Reserve[9];                           // 0x07~0x0F


    REG16                               TS_MUX_CFG0;                                // 0x10
    #define TS_MUX_CFG_TS0_MUX_MASK                     0x000F
    #define TS_MUX_CFG_TS0_MUX_SHIFT                    0
    #define TS_MUX_CFG_TS1_MUX_MASK                     0x00F0
    #define TS_MUX_CFG_TS1_MUX_SHIFT                    4
    #define TS_MUX_CFG_TSFI_MUX_MASK                    0xF000
    #define TS_MUX_CFG_TSFI_MUX_SHIFT                   12
        #define TS_MUX_CFG_TS_MUX_TS0                   0x0000
        #define TS_MUX_CFG_TS_MUX_TS1                   0x0001
        #define TS_MUX_CFG_TS_MUX_TS2                   0x0002
        #define TS_MUX_CFG_TS_MUX_TSO                   0x0006
        #define TS_MUX_CFG_TS_MUX_DMD                   0x0007
    REG16                               TS_MUX_CFG1;                                // 0x11

    REG16                               TS_MUX_CFG_S2P;                             // 0x12
    #define TS_MUX_CFG_S2P0_MUX_MASK                    0x000F
        #define TS_MUX_CFG_S2P_MUX_TS0                  0x0000
        #define TS_MUX_CFG_S2P_MUX_TS1                  0x0001
        #define TS_MUX_CFG_S2P_MUX_TS2                  0x0002

    REG16                               TS_MUX_CFG0_TSOIN;                          // 0x13
        #define TS_MUX_CFG_TSOIN0_MUX_MASK              0x000F
        #define TS_MUX_CFG_TSOIN0_MUX_SHIFT             0
        #define TS_MUX_CFG_TSOIN1_MUX_MASK              0x00F0
        #define TS_MUX_CFG_TSOIN1_MUX_SHIFT             4

    REG16                               TSP5_Reserve_14;                            // 0x14

    REG16                               TS_MUX_CFG_TSOOUT;                          // 0x15
    #define TS_MUX_CFG_TSOOUT_MASK                      0x000F
        #define TS_MUX_CFG_TSOOUT_FROM_TSO              0x0000
        #define TS_MUX_CFG_TSOOUT_FROM_S2P              0x0001
}REG_Ctrl5;

// TSP: ts sample part
typedef struct _REG_TS_Sample
{
    REG16                               TS0_Clk_Sample;                             // 0x00
    #define TS0_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TS0_PHASE_ADJUST_EN                         0x0020
    #define TS0_RESAMPLE_VOTE_ADJUST_EN                 0x0040

    REG16                               TS1_Clk_Sample;                             // 0x01
    #define TS1_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TS1_PHASE_ADJUST_EN                         0x0020
    #define TS1_RESAMPLE_VOTE_ADJUST_EN                 0x0040

    REG16                               TS2_Clk_Sample;                             // 0x02
    #define TS2_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TS2_PHASE_ADJUST_EN                         0x0020
    #define TS2_RESAMPLE_VOTE_ADJUST_EN                 0x0040

    REG16                               TS3_Clk_Sample;                             // 0x03
    #define TS3_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TS3_PHASE_ADJUST_EN                         0x0020
    #define TS3_RESAMPLE_VOTE_ADJUST_EN                 0x0040

    REG16                               TS4_Clk_Sample;                             // 0x04
    #define TS4_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TS4_PHASE_ADJUST_EN                         0x0020
    #define TS4_RESAMPLE_VOTE_ADJUST_EN                 0x0040

    REG16                               TS5_Clk_Sample;                             // 0x05
    #define TS5_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TS5_PHASE_ADJUST_EN                         0x0020
    #define TS5_RESAMPLE_VOTE_ADJUST_EN                 0x0040

    REG16                               TsSample_Reserved0[0x10-0x6];               // 0x06 - 0x0F

    REG16                               TSO_Clk_Sample;                             // 0x10
    #define TSO_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define TSO_PHASE_ADJUST_EN                         0x0020
    #define TSO_RESAMPLE_VOTE_ADJUST_EN                 0x0040
    #define TSO_CLK_INVERT                              0x0080

    REG16                               TsSample_Reserved1[0x20-0x11];              // 0x11 - 0x1F

    REG16                               TS_Out_Clk_Sample;                          // 0x20 (for old path: TSIF2 out)
    #define TS_OUT_PHASE_ADJUST_COUNT_MASK              0x001F
    #define TS_OUT_PHASE_ADJUST_EN                      0x0020
    #define TS_OUT_RESAMPLE_VOTE_ADJUST_EN              0x0040
    #define TS_OUT_CLK_INVERT                           0x0080

    REG16                               S2P_Out_Clk_Sample;                         // 0x21
    #define S2P_PHASE_ADJUST_COUNT_MASK                 0x001F
    #define S2P_PHASE_ADJUST_EN                         0x0020
    #define S2P_RESAMPLE_VOTE_ADJUST_EN                 0x0040
    #define S2P_CLK_INVERT                              0x0080

    REG16                               TsSample_Reserved2[0x30-0x23];              // 0x22 - 0x30

    REG16                               TSO_Out_Clk_Sel;                            // 0x30
    #define TSO_0_sel_MASK                              0x0003
    #define TSO_0_sel_TSO                               0x0000
    #define TSO_0_sel_S2P0                              0x0001
    #define TSO_0_sel_S2P1                              0x0002

    #define TSO_1_sel_MASK                              0x0030
    #define TSO_1_sel_TSO                               0x0000
    #define TSO_1_sel_S2P0                              0x0010
    #define TSO_1_sel_S2P1                              0x0020

}REG_TS_Sample;

// Firmware status
#define TSP_FW_STATE_MASK           0xFFFF0000
#define TSP_FW_STATE_LOAD           0x00010000
#define TSP_FW_STATE_ENG_OVRUN      0x00020000
#define TSP_FW_STATE_ENG1_OVRUN     0x00040000                          //[reserved]
#define TSP_FW_STATE_IC_ENABLE      0x01000000
#define TSP_FW_STATE_DC_ENABLE      0x02000000
#define TSP_FW_STATE_IS_ENABLE      0x04000000
#define TSP_FW_STATE_DS_ENABLE      0x08000000


// TSP AEON specific IP address
#define OPENRISC_IP_1_ADDR 0x00200000
#define OPENRISC_IP_1_SIZE 0x00020000
#define OPENRISC_IP_2_ADDR 0x90000000
#define OPENRISC_IP_2_SIZE 0x00010000
#define OPENRISC_IP_3_ADDR 0x40080000
#define OPENRISC_IP_3_SIZE 0x00020000
#define OPENRISC_QMEM_ADDR 0x00000000
#define OPENRISC_QMEM_SIZE 0x00003000
#endif // _TSP_REG_H_
