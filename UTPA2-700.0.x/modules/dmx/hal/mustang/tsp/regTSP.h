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
#define TS_PACKET_SIZE              188UL


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------
#define TSP_LIVE_AV_BLOCK_EN            //for maserati , live path should enable AV fifo block, since dscmb behavior change


//[CMODEL][FWTSP]
// When enable, interrupt will not lost, CModel will block next packet
// and FwTSP will block until interrupt status is clear by MIPS.
// (For firmware and cmodel only)
#define TSP_DBG_SAFE_MODE_ENABLE    0UL

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define TSP_PIDFLT_NUM                  128UL

#define TSP_PVR_IF_NUM                  2UL
#define TSP_MMFI0_FILTER_NUM            4UL
#define TSP_MMFI1_FILTER_NUM            4UL
#define TSP_IF_NUM                      4UL
#define TSP_DEMOD_NUM                   2UL
#define TSP_VFIFO_NUM                   2UL
#define TSP_AFIFO_NUM                   4UL
#define TSP_TS_PAD_NUM                  3UL  // 2P in + 1P in&out
#define TSP_VQ_NUM                      4UL  //VQ0, VQ_file, VQ1, VQ_2
#define TSP_VQ_PITCH                    208UL
#define TSP_CA_ENGINE_NUM               3UL
#define TSP_CA_KEY_NUM                  8UL
#define TSP_CA0_FLT_NUM                 128UL
#define TSP_CA1_FLT_NUM                 128UL
#define TSP_CA_FLT_NUM                  128UL
#define TSP_MERGESTR_MUM                8UL
#define TSP_ENGINE_NUM                  1UL
#define TSP_SECFLT_NUM                  128UL
#define TSP_PCRFLT_NUM                  4UL
#define TSP_STC_NUM                     4UL

#ifdef HWPCR_ENABLE
#define TSP_PIDFLT_NUM_ALL              (TSP_PIDFLT_NUM+TSP_PCRFLT_NUM)
#else
#define TSP_PIDFLT_NUM_ALL              (TSP_PIDFLT_NUM)
#endif

#define TSP_SECBUF_NUM                  TSP_SECFLT_NUM
#define TSP_FILTER_DEPTH                16UL

#define TSP_WP_SET_NUM                  4UL

#define DSCMB_FLT_START_ID              16UL
#define DSCMB_FLT_END_ID                31UL
#define DSCMB_FLT_NUM                   16UL

#define DSCMB1_FLT_START_ID             32UL
#define DSCMB1_FLT_END_ID               47UL
#define DSCMB1_FLT_NUM                  16UL

#define DSCMB2_FLT_START_ID             48UL
#define DSCMB2_FLT_END_ID               63UL
#define DSCMB2_FLT_NUM                  16UL

#define DSCMB_FLT_SHAREKEY_START_ID     64UL
#define DSCMB_FLT_SHAREKEY_END_ID       127UL
#define DSCMB_FLT_SHAREKEY_NUM          128UL

#define DSCMB_FLT_SHAREKEY1_START_ID    64UL
#define DSCMB_FLT_SHAREKEY1_END_ID      127UL
#define DSCMB_FLT_SHAREKEY1_NUM         128UL

#define DSCMB_FLT_SHAREKEY2_START_ID    64UL
#define DSCMB_FLT_SHAREKEY2_END_ID      127UL
#define DSCMB_FLT_SHAREKEY2_NUM         128UL

#define TSP_NMATCH_FLTID                17UL


//PAD MUX definition
#define TSP_MUX_TS0                     0UL
#define TSP_MUX_TS1                     1UL
#define TSP_MUX_TS2                     2UL
#define TSP_MUX_TSO                     6UL
#define TSP_MUX_INDEMOD                 7UL
#define TSP_MUX_3WIRE_MASK              0x80UL
#define TSP_MUX_TSCB                    0xFFUL //not support
#define TSP_MUX_NONE                    0xFF

//Clk source definition
#define TSP_CLK_DISABLE                 0x01UL
#define TSP_CLK_INVERSE                 0x02UL
#define TSP_CLK_TS0                     0x00UL
#define TSP_CLK_TS1                     0x04UL
#define TSP_CLK_TS2                     0x08UL
#define TSP_CLK_TSOOUT                  0x18UL
#define TSP_CLK_INDEMOD                 0x1CUL
#define CLKGEN0_TSP_CLK_MASK            0x1CUL
#define TSP_CLK_TSCB                    0xFFUL  //not support

//PIDFLT1,2 source definition
#define TSP_PIDFLT1_USE_TSIF1           0UL
#define TSP_PIDFLT2_USE_TSIF2           1UL
#define TSP_PIDFLT1_USE_TSIF_MMFI0      2UL
#define TSP_PIDFLT2_USE_TSIF_MMFI1      3UL


#define TSP_FW_DEVICE_ID                0x67UL

#define STC_SYNTH_DEFAULT               0x28000000UL

#define DRAM_SIZE                       (0x80000000UL)
#define TSP_FW_BUF_SIZE                 (0x4000UL)
#define TSP_FW_BUF_LOW_BUD              0UL
#define TSP_FW_BUF_UP_BUD               DRAM_SIZE

#define TSP_VQ_BUF_LOW_BUD              0UL
#define TSP_VQ_BUF_UP_BUD               (0xFFFFFFFFUL)

#define TSP_SEC_BUF_LOW_BUD             0UL
#define TSP_SEC_BUF_UP_BUD              (0xFFFFFFFFUL)
#define TSP_SEC_FLT_DEPTH               32UL
#define TSP_FIQ_NUM                     1UL

//QMEM Setting
#define _TSP_QMEM_I_MASK                0xffff8000UL //total: 0x4000
#define _TSP_QMEM_I_ADDR_HIT            0x00000000UL
#define _TSP_QMEM_I_ADDR_MISS           0xffffffffUL
#define _TSP_QMEM_D_MASK                0xffff8000UL
#define _TSP_QMEM_D_ADDR_HIT            0x00000000UL
#define _TSP_QMEM_D_ADDR_MISS           0xffffffffUL
#define _TSP_QMEM_SIZE                  0x1000UL // 16K bytes, 32bit aligment  //0x4000

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Software
#define REG_PIDFLT_L_BASE                (0x00210000UL << 1UL)                   // Fit the size of REG32, 0~127
#define REG_PIDFLT_H_BASE                (0x00210800UL << 1UL)                   // Fit the size of REG32, 0~127

#define REG_PIDFLT_L_EXT_BASE            (0x00210400UL << 1UL)                   // Fit the size of REG32, 128~143
#define REG_PIDFLT_H_EXT_BASE            (0x00210C00UL << 1UL)                   // Fit the size of REG32, 128~143

#define REG_SECFLT_BASE1                 (0x00211000UL << 1UL)                   // Fix the size of REG32
#define REG_SECFLT_BASE2                 (0x00215000UL << 1UL)                   // Fix the size of REG32

#define REG_CTRL_BASE                    (0x2A00UL)                              // 0xBF800000+(1500/2)*4
#define REG_CTRL_MMFIBASE                (0x39C0UL)                              // 0xBF800000+(3800/2)*4 (TSP2: debug table), from 0x70
#define REG_CTRL_TSP3                    (0xC1440UL)                             // 0xBF800000+(60a20/2)*4
#define REG_CTRL_TSP4                    (0xC2E00UL)                             // 0xBF800000+(61700/2)*4
#define REG_CTRL_TSP5                    (0xC7600UL)                             // 0xBF800000+(63b00/2)*4
#define REG_CTRL_TSP6                    (0xC3E00UL)                             // 0xBF800000+(61f00/2)*4
#define REG_CTRL_TS_SAMPLE               (0x21400UL)                             // 0xBF800000+(10A00/2)*4

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
#define TSP_PIDFLT_PID_MASK             0x00001FFFUL
#define TSP_PIDFLT_PID_SHFT             0UL

// PIDFLT SRC
#define TSP_PIDFLT_IN_MASK              0x0000E000UL
#define TSP_PIDFLT_IN_NONE              0x00000000UL
#define TSP_PIDFLT_IN_PIDFLT0           0x00002000UL
#define TSP_PIDFLT_IN_PIDFLT_FILE       0x00004000UL
#define TSP_PIDFLT_IN_PIDFLT1           0x00006000UL
#define TSP_PIDFLT_IN_PIDFLT2           0x00008000UL
#define TSP_PIDFLT_IN_PIDFLT_CB         0UL                                   //not support
#define TSP_PIDFLT_IN_SHIFT             13UL

// Section filter Id (0~128)
#define TSP_PIDFLT_SECFLT_MASK          0x000007F0UL                          // [38:32] secflt id
#define TSP_PIDFLT_SECFLT_SHFT          4UL

// Stream source ID
#define TSP_PIDFLT_IN_SRC_MASK          0x0000000FUL                         // [42:39] stream source id
#define TSP_PIDFLT_IN_SRC_SHFT          0UL

// AF/Sec/Video/V3D/Audio/Audio-second/PVR1/PVR2
#define TSP_PIDFLT_OUT_MASK             0xFFE00000UL
#define TSP_PIDFLT_OUT_NONE             0x00000000UL
#define TSP_PIDFLT_OUT_AFIFO4           0x00200000UL
#define TSP_PIDFLT_OUT_AFIFO3           0x00400000UL
#define TSP_PIDFLT_OUT_SECFLT_AF        0x01000000UL
#define TSP_PIDFLT_OUT_SECFLT           0x02000000UL
#define TSP_PIDFLT_OUT_VFIFO            0x04000000UL
#define TSP_PIDFLT_OUT_VFIFO3D          0x08000000UL
#define TSP_PIDFLT_OUT_AFIFO            0x10000000UL
#define TSP_PIDFLT_OUT_AFIFO2           0x20000000UL
#define TSP_PIDFLT_OUT_PVR1             0x80000000UL
#define TSP_PIDFLT_OUT_PVR2             0x40000000UL

#define TSP_PIDFLT_SECFLT_NULL          0x7FUL                                // software usage clean selected section filter
//******************** PIDFLT DEFINE END ********************//

typedef struct _REG_SecFlt
{
    REG32                           Ctrl;
    // SW flag
    #define TSP_SECFLT_TYPE_MASK                    0x01000007UL
    #define TSP_SECFLT_TYPE_SHFT                    0UL
    #define TSP_SECFLT_TYPE_SEC                     0x00000000UL
    #define TSP_SECFLT_TYPE_PES                     0x00000001UL
    #define TSP_SECFLT_TYPE_PKT                     0x00000002UL
    #define TSP_SECFLT_TYPE_PCR                     0x00000003UL
    #define TSP_SECFLT_TYPE_TTX                     0x00000004UL
    #define TSP_SECFLT_TYPE_VER                     0x00000005UL
    #ifdef SEC_ADF_TYPE_SUPPORT
    #define TSP_SECFLT_TYPE_ADF                     0x00000006UL          //for af_descriptor
    #endif
    //#define TSP_SECFLT_TYPE_EMM                     0x00000006UL
    //#define TSP_SECFLT_TYPE_ECM                     0x00000007UL
    #define TSP_SECFLT_TYPE_SEC_NO_PUSI             0x01000000UL

    #define TSP_SECFLT_PCRRST                       0x00000010UL          // for TSP_SECFLT_TYPE_PCR

    #define TSP_SECFLT_MODE_MASK                    0x00000030UL          // software implementation
    #define TSP_SECFLT_MODE_SHFT                    4UL
    #define TSP_SECFLT_MODE_CONTI                   0x0UL
    #define TSP_SECFLT_MODE_ONESHOT                 0x1UL
    #define TSP_SECFLT_MODE_CRCCHK                  0x2UL
    #define TSP_SECFLT_MODE_PESSCMCHK               0x3UL                 //Only for PES type checking SCMB status

    #define TSP_SECFLT_STATE_MASK                   0x000000C0UL          // software implementation
    #define TSP_SECFLT_STATE_SHFT                   6UL
    #define TSP_SECFLT_STATE_OVERFLOW               0x1UL
    #define TSP_SECFLT_STATE_DISABLE                0x2UL

    REG32                           Match[TSP_FILTER_DEPTH/sizeof(MS_U32)];

    REG32                           Mask[TSP_FILTER_DEPTH/sizeof(MS_U32)];

    REG32                           BufStart;
    #define TSP_SECFLT_BUFSTART_MASK                0xFFFFFFFFUL

    REG32                           BufEnd;

    REG32                           BufRead;

    REG32                           BufWrite;

    REG32                           BufCur;

    REG32                           RmnReqCnt;
    #define TSP_SECFLT_OWNER_MASK                   0x80000000UL
    #define TSP_SECFLT_OWNER_SHFT                   31UL
    #define TSP_SECFLT_REQCNT_MASK                  0x7FFF0000UL
    #define TSP_SECFLT_REQCNT_SHFT                  16UL
    #define TSP_SECFLT_RMNCNT_MASK                  0x0000FFFFUL
    #define TSP_SECFLT_RMNCNT_SHFT                  0UL

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
    REG_PidFlt                      Flt[TSP_PIDFLT_NUM];
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
    #define TSP_HW_PVR_BUF_HEAD20_MASK              0xFFFF0000UL
    #define TSP_HW_PVR_BUF_HEAD20_SHFT              16UL

    REG32                           TsRec_Head21_Mid20_Wptr;            // 0xbf802a08   0x02 ,wptr & mid share same register
    #define TSP_HW_PVR_BUF_HEAD21_MASK              0x00000FFFUL
    #define TSP_HW_PVR_BUF_HEAD21_SHFT              0UL
    #define TSP_HW_PVR_BUF_MID20_MASK               0xFFFF0000UL
    #define TSP_HW_PVR_BUF_MID20_SHFT               16UL

    REG32                           TsRec_Mid21_Tail20;                 // 0xbf802a10   0x04
    #define TSP_HW_PVR_BUF_MID21_MASK               0x00000FFFUL
    #define TSP_HW_PVR_BUF_MID21_SHFT               0UL
    #define TSP_HW_PVR_BUF_TAIL20_MASK              0xFFFF0000UL
    #define TSP_HW_PVR_BUF_TAIL20_SHFT              16UL

    REG32                           TsRec_Tail2_Pcr1;                   // 0xbf802a18   0x06
    #define TSP_HW_PVR_BUF_TAIL21_MASK              0x00000FFFUL
    #define TSP_HW_PVR_BUF_TAIL21_SHFT              0UL                   // PCR64 L16
    #define TSP_PCR64_L16_MASK                      0xFFFF0000UL
    #define TSP_PCR64_L16_SHFT                      16UL

    REG32                           Pcr1;                               // 0xbf802a20   0x08
    #define TSP_PCR64_MID32_MASK                    0xFFFFFFFFUL          // PCR64 Middle 64
    #define TSP_PCR64_MID32_SHFT                    0UL

    REG32                           Pcr64_H;                            // 0xbf802a28   0x0a
    #define TSP_PCR64_H16_MASK                      0x0000FFFFUL
    #define TSP_PCR64_H16_SHFT                      0UL
    #define TSP_MOBF_FILE_INDEX_MASK                0x001F0000UL        // MOBF file index
    #define TSP_MOBF_FILE_INDEX_SHIFT               16UL

    REG16                           _xbf202a30;                         // 0xbf802a30   0x0c

    REG16                           SW_Mail_Box0;                       // 0xbf802a34   0x0d

    REG32                           PVR2_Config;                        // 0xbf802a38   0x0e
    #define TSP_PVR2_LPCR1_WLD                      0x00000001UL
    #define TSP_PVR2_LPCR1_RLD                      0x00000002UL
    #define TSP_PVR2_STR2MIU_DSWAP                  0x00000004UL
    #define TSP_PVR2_STR2MIU_EN                     0x00000008UL
    #define TSP_PVR2_STR2MIU_RST_WADR               0x00000010UL
    #define TSP_PVR2_STR2MIU_BT_ORDER               0x00000020UL
    #define TSP_PVR2_STR2MIU_PAUSE                  0x00000040UL
    #define TSP_PVR2_REG_PINGPONG_EN                0x00000080UL
    #define TSP_PVR2_PVR_ALIGN_EN                   0x00000100UL
    #define TSP_PVR2_DMA_FLUSH_EN                   0x00000200UL
    #define TSP_PVR2_PKT192_EN                      0x00000400UL
    #define TSP_PVR2_BURST_LEN_MASK                 0x00001800UL
    #define TSP_PVR2_BURST_LEN_4                    0x00000800UL
    #define TSP_REC_DATA2_INV                       0x00002000UL
    #define TSP_V_BLOCK_DIS                         0x00004000UL
    #define TSP_V3D_BLOCK_DIS                       0x00008000UL
    #define TSP_AUD_BLOCK_DIS                       0x00010000UL
    #define TSP_AUDB_BLOCK_DIS                      0x00020000UL
    #define TSP_PVR1_BLOCK_DIS                      0x00040000UL
    #define TSP_PVR2_BLOCK_DIS                      0x00080000UL
    #define TSP_TSIF2_ENABLE                        0x00100000UL
    #define TSP_TSIF2_DATASWAP                      0x00200000UL
    #define TSP_TSIF2_SERL                          0x00000000UL
    #define TSP_TSIF2_PARL                          0x00400000UL
    #define TSP_TSIF2_EXTSYNC                       0x00800000UL
    #define TSP_TSIF2_BYPASS                        0x01000000UL
    #define TSP_TEI_SKIP_PKT2                       0x02000000UL
    #define TSP_AUDC_BLOCK_DIS                      0x04000000UL
    #define TSP_AUDD_BLOCK_DIS                      0x08000000UL
    #define TSP_DIS_LOCKED_PKT_CNT                  0x10000000UL
    #define TSP_CLR_LOCKED_PKT_CNT                  0x20000000UL
    #define TSP_CLR_AV_PKT_CNT                      0x40000000UL
    #define TSP_CLR_PVR_OVERFLOW                    0x80000000UL

    REG32                           PVR2_LPCR1;                         // 0xbf802a40   0x10

    #define TSP_STR2MI2_ADDR_MASK  0x0FFFFFFFUL
    REG32                           Str2mi_head1_pvr2;                  // 0xbf802a48   0x12
    REG32                           Str2mi_mid1_wptr_pvr2;              // 0xbf802a50   0x14
    REG32                           Str2mi_tail1_pvr2;                  // 0xbf802a58   0x16
    REG32                           Str2mi_head2_pvr2;                  // 0xbf802a60   0x18
    REG32                           Str2mi_mid2_pvr2;                   // 0xbf802a68   0x1a, PVR2 mid address & write point
    REG32                           Str2mi_tail2_pvr2;                  // 0xbf802a70   0x1c
    REG32                           SyncByte2_ChkSize;                  // 0xbf802a78   0x1e
    #define TSP_SYNC_BYTE2_MASK     0x000000FFUL
    #define TSP_PKT_SIZE2_MASK      0x0000FF00UL
    #define TSP_PKT_SIZE2_SHIFT     8UL
    #define TSP_PKT_CHK_SIZE2_MASK  0x00FF0000UL
    #define TSP_PKT_CHK_SIZE2_SHIFT 16UL
    REG32                           Pkt_CacheW0;                        // 0xbf802a80   0x20

    REG32                           Pkt_CacheW1;                        // 0xbf802a88   0x22

    REG32                           Pkt_CacheW2;                        // 0xbf802a90   0x24

    REG32                           Pkt_CacheW3;                        // 0xbf802a98   0x26

    REG32_L                         Pkt_CacheIdx;                       // 0xbf802aa0   0x28

    REG32                           Pkt_DMA;                            // 0xbf802aa8   0x2a
    #define TSP_SEC_DMAFIL_NUM_MASK                 0x000000FFUL
    #define TSP_SEC_DMAFIL_NUM_SHIFT                0UL
    #define TSP_SEC_DMASRC_OFFSET_MASK              0x0000FF00UL
    #define TSP_SEC_DMASRC_OFFSET_SHIFT             8UL
    #define TSP_SEC_DMASRC_OFFSET_MASK              0x0000FF00UL
    #define TSP_SEC_DMADES_LEN_MASK                 0x00FF0000UL
    #define TSP_SEC_DMADES_LEN_SHIFT                16UL

    REG32                           Hw_Config0;                         // 0xbf802ab0   0x2c
    #define TSP_HW_CFG0_DATA_PORT_EN                0x00000001UL
    #define TSP_HW_CFG0_TSIFO_SERL                  0x00000000UL
    #define TSP_HW_CFG0_TSIF0_PARL                  0x00000002UL
    #define TSP_HW_CFG0_TSIF0_EXTSYNC               0x00000004UL
    #define TSP_HW_CFG0_TSIF0_TS_BYPASS             0x00000008UL
    #define TSP_HW_CFG0_TSIF0_VPID_BYPASS           0x00000010UL
    #define TSP_HW_CFG0_TSIF0_APID_BYPASS           0x00000020UL
    #define TSP_HW_CFG0_WB_DMA_RESET                0x00000040UL
    #define TSP_HW_CFG0_TSIF0_APID_B_BYPASS         0x00000080UL
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_MASK        0x0000FF00UL
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_SHIFT       8UL
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_MASK    0x00FF0000UL
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_SHIFT   16UL
    #define TSP_HW_CFG0_PACKET_CHK_SIZE_MASK        0xFF000000UL
    #define TSP_HW_CFG0_PACKET_CHK_SIZE_SHFT        24UL

    REG32                           TSP_DBG_PORT;                       // 0xbf802ab8   0x2e
    #define TSP_PCR64_3_SET                         0x00000001UL
    #define TSP_PCR64_3_EN                          0x00000002UL
    #define TSP_PCR64_3_LD                          0x00000004UL
    #define TSP_PCR64_4_SET                         0x00000010UL
    #define TSP_PCR64_4_EN                          0x00000020UL
    #define TSP_PCR64_4_LD                          0x00000040UL
    #define TSP_DNG_DATA_PORT_MASK                  0x00FF0000UL
    #define TSP_DNG_DATA_PORT_SHIFT                 16UL

    REG_Stc                         Pcr;                                // 0xbf802ac0   0x30 & 0x32

    REG32                           Pkt_Info;                           // 0xbf802ad0   0x34
    #define TSP_APID_L_MASK                         0x000000FFUL
    #define TSP_APID_L_SHIFT                        0UL
    #define TSP_APID_H_MASK                         0x00001F00UL
    #define TSP_APID_H_SHIFT                        8UL
    #define TSP_PKT_PID_8_12_CP_MASK                0x001F0000UL
    #define TSP_PKT_PID_8_12_CP_SHIFT               16UL
    #define TSP_PKT_PRI_MASK                        0x00200000UL
    #define TSP_PKT_PRI_SHIFT                       21UL
    #define TSP_PKT_PLST_MASK                       0x00400000UL
    #define TSP_PKT_PLST_SHIFT                      22UL
    #define TSP_PKT_ERR                             0x00800000UL
    #define TSP_PKT_ERR_SHIFT                       23UL
    #define TSP_DMAW_NO_HIT_INT                     0x0F000000UL
    #define TSP_DMAW_NO_HIT_INT_SHIFT               24UL

    REG32                           Pkt_Info2;                          // 0xbf802ad8   0x36
    #define TSP_PKT_INFO_CC_MASK                    0x0000000FUL
    #define TSP_PKT_INFO_CC_SHFT                    0UL
    #define TSP_PKT_INFO_ADPCNTL_MASK               0x00000030UL
    #define TSP_PKT_INFO_ADPCNTL_SHFT               4UL
    #define TSP_PKT_INFO_SCMB                       0x000000C0UL
    #define TSP_PKT_INFO_SCMB_SHFT                  6UL
    #define TSP_PKT_PID_0_7_CP_MASK                 0x0000FF00UL
    #define TSP_PKT_PID_0_7_CP_SHIFT                8UL
    #define TSP_VFIFO3D_STATUS                      0x000F0000UL
    #define TSP_VFIFO3D_STATUS_SHFT                 16UL
    #define TSP_VFIFO_STATUS                        0x00F00000UL
    #define TSP_VFIFO_STATUS_SHFT                   20UL
    #define TSP_AFIFO_STATUS                        0x0F000000UL
    #define TSP_AFIFO_STATUS_SHFT                   24UL
    #define TSP_AFIFOB_STATUS                       0xF0000000UL
    #define TSP_AFIFOB_STATUS_SHFT                  28UL

    REG32                           SwInt_Stat;                         // 0xbf802ae0   0x38
    #define TSP_SWINT_INFO_SEC_MASK                 0x000000FFUL
    #define TSP_SWINT_INFO_SEC_SHFT                 0UL
    #define TSP_SWINT_INFO_ENG_MASK                 0x0000FF00UL
    #define TSP_SWINT_INFO_ENG_SHFT                 8UL
    #define TSP_SWINT_STATUS_CMD_MASK               0x7FFF0000UL
    #define TSP_SWINT_STATUS_CMD_SHFT               16UL
    #define TSP_SWINT_STATUS_SEC_RDY                0x0001UL
    #define TSP_SWINT_STATUS_REQ_RDY                0x0002UL
    #define TSP_SWINT_STATUS_BUF_OVFLOW             0x0006UL
    #define TSP_SWINT_STATUS_SEC_CRCERR             0x0007UL
    #define TSP_SWINT_STATUS_SEC_ERROR              0x0008UL
    #define TSP_SWINT_STATUS_SYNC_LOST              0x0010UL
    #define TSP_SWINT_STATUS_PKT_OVRUN              0x0020UL
    #define TSP_SWINT_STATUS_DEBUG                  0x0030UL
    #define TSP_SWINT_CMD_DMA_PAUSE                 0x0100UL
    #define TSP_SWINT_CMD_DMA_RESUME                0x0200UL
    #define TSP_SWINT_STATUS_SEC_GROUP              0x000FUL
    #define TSP_SWINT_STATUS_GROUP                  0x00FFUL
    #define TSP_SWINT_CMD_GROUP                     0x7F00UL
    #define TSP_SWINT_CMD_STC_UPD                   0x0400UL
    #define TSP_SWINT_CTRL_FIRE                     0x80000000UL

    REG32                           TsDma_Addr;                         // 0xbf802ae8   0x3a

    REG32                           TsDma_Size;                         // 0xbf802af0   0x3c

    REG32                           TsDma_Ctrl_CmdQ;                    // 0xbf802af8   0x3e

    #define TSP_TSDMA_CTRL_VPES0                    0x00000004UL
    #define TSP_TSDMA_CTRL_APES0                    0x00000008UL
    #define TSP_TSDMA_CTRL_A2PES0                   0x00000010UL
    #define TSP_TSDMA_CTRL_V3DPES0                  0x00000020UL
    #define TSP_TSDMA_CTRL_A3PES0                   0x00000040UL
    #define TSP_TSDMA_CTRL_A4PES0                   0x00000080UL

    #define TSP_TSDMA_CTRL_START                    0x00000001UL
    #define TSP_TSDMA_CTRL_DONE                     0x00000002UL
    #define TSP_TSDMA_STAT_ABORT                    0x00000080UL
    #define TSP_CMDQ_CNT_MASK                       0x001F0000UL
    #define TSP_CMDQ_CNT_SHFT                       16UL
    #define TSP_CMDQ_FULL                           0x00400000UL
    #define TSP_CMDQ_EMPTY                          0x00800000UL
    #define TSP_CMDQ_SIZE                           16UL
    #define TSP_CMDQ_WR_LEVEL_MASK                  0x03000000UL
    #define TSP_CMDQ_WR_LEVEL_SHFT                  24UL

    REG32                           MCU_Cmd;                            // 0xbf802b00   0x40
    #define TSP_MCU_CMD_MASK                                    0xFF000000UL
    #define TSP_MCU_CMD_NULL                                    0x00000000UL
    #define TSP_MCU_CMD_ALIVE                                   0x01000000UL
    #define TSP_MCU_CMD_NMATCH                                  0x02000000UL
    #define TSP_MCU_CMD_NMATCH_FLT_MASK                         0x000000FFUL
    #define TSP_MCU_CMD_NMATCH_FLT_SHFT                         0x00000000UL
    #define TSP_MCU_CMD_PCR_GET                                 0x03000000UL
    #define TSP_MCU_CMD_VER_RESET                               0x04000000UL
        #define TSP_MCU_CMD_VER_RESET_FLT_MASK                  0x000000FFUL
        #define TSP_MCU_CMD_VER_RESET_FLT_SHFT                  0x00000000UL
    #define TSP_MCU_CMD_MEM_HIGH_ADDR                           0x05000000UL
    #define TSP_MCU_CMD_MEM_LOW_ADDR                            0x06000000UL
        #define TSP_MCU_CMD_MEM_ADDR_SHFT                       0x00000000UL
        #define TSP_MCU_CMD_MEM_ADDR_MASK                       0x0000FFFFUL
    #define TSP_MCU_CMD_VERSION_GET                             0x07000000UL
    #define TSP_MCU_CMD_DBG_MEM                                 0x08000000UL
    #define TSP_MCU_CMD_DBG_WORD                                0x09000000UL
    #define TSP_MCU_CMD_HWPCR_REG_SET                           0x0A000000UL
    #define TSP_MCU_CMD_SCMSTS_GET                              0x0B000000UL
    #define TSP_MCU_CMD_CTRL_STC_UPDATE                         0x0C000000UL
    #define TSP_MCU_CMD_CTRL_STC1_UPDATE                        0x0D000000UL
        #define TSP_MCU_CMD_CTRL_STC_UPDATE_OPTION_MASK         0x00FF0000UL
        #define TSP_MCU_CMD_CTRL_STC_UPDATE_ONCE                0x00010000UL
    #define TSP_MCU_CMD_TEI_COUNT_GET                           0x0E000000UL
        #define TSP_MCU_CMD_TEI_COUNT_SRC_MASK                  0x0000FFFFUL
            #define TSP_MCU_CMD_TEI_COUNT_SRC_LIVE              0x00000000UL
            #define TSP_MCU_CMD_TEI_COUNT_SRC_FILE              0x00000001UL
        #define TSP_MCU_CMD_TEI_COUNT_OPTION_MASK               0x00FF0000UL
            #define TSP_MCU_CMD_TEI_COUNT_OPTION_RESET          0x00800000UL
    #define TSP_MCU_CMD_DISCONT_COUNT_GET                       0x0F000000UL
        #define TSP_MCU_CMD_DISCONT_COUNT_FLT_MASK              0x0000FFFFUL
            #define TSP_MCU_CMD_DISCONT_COUNT_OPTION_MASK       0x00FF0000UL
        #define TSP_MCU_CMD_DISCONT_COUNT_OPTION_RESET          0x00800000UL
    #define TSP_MCU_CMD_SEL_STC_ENG                             0x20000000UL
        #define TSP_MCU_SEL_STC_ENG_ID_MASK                     0x000000FFUL
        #define TSP_MCU_SEL_STC_ENG_ID_SHIFT                    0UL
        #define TSP_MCU_CMD_SEL_STC_ENG_FLTSRC_MASK             0x0000FF00UL
        #define TSP_MCU_CMD_SEL_STC_ENG_FLTSRC_SHIFT            8UL

    REG32                           Hw_Config2;                         // 0xbf802b08   0x42
    #define TSP_HW_CFG2_PACKET_CHK_SIZE1_MASK       0x000000FFUL
    #define TSP_HW_CFG2_PACKET_CHK_SIZE1_SHFT       0UL
    #define TSP_HW_CFG2_PACKET_SYNCBYTE1_MASK       0x0000FF00UL
    #define TSP_HW_CFG2_PACKET_SYNCBYTE1_SHFT       8UL
    #define TSP_HW_CFG2_PACKET_SIZE1_MASK           0x00FF0000UL
    #define TSP_HW_CFG2_PACKET_SIZE1_SHFT           16UL
    #define TSP_HW_CFG2_TSIF1_SERL                  0x00000000UL
    #define TSP_HW_CFG2_TSIF1_PARL                  0x01000000UL
    #define TSP_HW_CFG2_TSIF1_EXTSYNC               0x02000000UL
    #define TSP_HW_CFG2_PIDFLT1_SOURCE_TSIF_MMFI0   0x20000000UL          // Switch source of PIDFLT1 to MMFI0
    #define TSP_HW_CFG2_PIDFLT2_SOURCE_TSIF_MMFI1   0x40000000UL          // Switch source of PIDFLT2 to MMFI1

    REG32                           Hw_Config4;                         // 0xbf802b10   0x44
    #define TSP_HW_CFG4_PVR_ENABLE                  0x00000002UL
    #define TSP_HW_CFG4_PVR_ENDIAN_BIG              0x00000004UL          // 1: record TS to MIU with big endian, 0: record TS to MIU with little endian
    #define TSP_HW_CFG4_TSIF1_ENABLE                0x00000008UL          // 1: enable ts interface 1 and vice versa
    #define TSP_HW_CFG4_PVR_FLUSH                   0x00000010UL          // 1: str2mi_wadr <- str2mi_miu_head
    #define TSP_HW_CFG4_PVRBUF_BYTEORDER_BIG        0x00000020UL          // Byte order of 8-byte recoding buffer to MIU.
    #define TSP_HW_CFG4_PVR_PAUSE                   0x00000040UL
    #define TSP_HW_CFG4_MEMTSDATA_ENDIAN_BIG        0x00000080UL          // 32-bit data byte order read from 8x64 FIFO when playing file.
    #define TSP_HW_CFG4_TSIF0_ENABLE                0x00000100UL          // 1: enable ts interface 0 and vice versa
    #define TSP_VALID_FALLING_DETECT                0x00000200UL          // Reset bit count when data valid signal of TS interface is low.
    #define TSP_SYNC_RISING_DETECT                  0x00000400UL          // Reset bit count on the rising sync signal of TS interface.
    #define TSP_HW_CFG4_TS_DATA0_SWAP               0x00000800UL          // Set 1 to swap the bit order of TS0 DATA bus
    #define TSP_HW_CFG4_TS_DATA1_SWAP               0x00001000UL          // Set 1 to swap the bit order of TS1 DATA bus
    #define TSP_HW_TSP2OUTAEON_INT_EN               0x00004000UL          // Set 1 to force interrupt to outside AEON
    #define TSP_HW_HK_INT_FORCE                     0x00008000UL          // Set 1 to force interrupt to HK_MCU
    #define TSP_HW_CFG4_BYTE_ADDR_DMA               0x000F0000UL          // prevent from byte enable bug, bit1~3 must enable togather
    #define TSP_HW_CFG4_ALT_TS_SIZE                 0x00010000UL          // enable TS packets in 204 mode
    #define TSP_HW_DMA_MODE_MASK                    0x00300000UL          // Section filter DMA mode, 2'b00: Single.2'b01: Burst 2 bytes.2'b10: Burst 4 bytes.2'b11: Burst 8 bytes.
    #define TSP_HW_DMA_MODE_SHIFT                   20UL
    #define TSP_HW_CFG4_WSTAT_CH_EN                 0x00400000UL
    #define TSP_HW_CFG4_PS_VID_EN                   0x00800000UL          // program stream video enable
    #define TSP_HW_CFG4_PS_AUD_EN                   0x01000000UL          // program stream audio enable
    #define TSP_HW_CFG4_PS_AUD2_EN                  0x02000000UL          // program stream audioB enable
    #define TSP_HW_CFG4_APES_ERR_RM_EN              0x04000000UL          // Set 1 to enable removing APES error packet
    #define TSP_HW_CFG4_VPES_ERR_RM_EN              0x08000000UL          // Set 1 to enable removing VPES error packet
    #define TSP_HW_CFG4_SEC_ERR_RM_EN               0x10000000UL          // Set 1 to enable removing section error packet
    #define TSP_HW_CFG4_VID_ERR                     0x20000000UL          // Set 1 to mask the error packet interrupt
    #define TSP_HW_CFG4_AUD_ERR                     0x40000000UL          // Set 1 to mask the error packet interrupt
    #define TSP_HW_CFG4_ISYNC_PATCH_EN              0x80000000UL          // Set 1 to enable the patch of internal sync in "tsif"

    REG32                           NOEA_PC;                            // 0xbf802b18   0x46

    REG32                           Idr_Ctrl_Addr0;                     // 0xbf802b20   0x48
    #define TSP_IDR_START                           0x00000001UL
    #define TSP_IDR_READ                            0x00000000UL
    #define TSP_IDR_WRITE                           0x00000002UL
    #define TSP_IDR_WR_ENDIAN_BIG                   0x00000004UL
    #define TSP_IDR_WR_ADDR_AUTO_INC                0x00000008UL          // Set 1 to enable address auto-increment after finishing read/write
    #define TSP_IDR_WDAT0_TRIG_EN                   0x00000010UL          // WDAT0_TRIG_EN
    #define TSP_IDR_MCUWAIT                         0x00000020UL
    #define TSP_IDR_SOFT_RST                        0x00000080UL          // Set 1 to soft-reset the IND32 module
    #define TSP_IDR_AUTO_INC_VAL_MASK               0x00000F00UL
    #define TSP_IDR_AUTO_INC_VAL_SHIFT              8UL
    #define TSP_IDR_ADDR_MASK0                      0xFFFF0000UL
    #define TSP_IDR_ADDR_SHFT0                      16UL

    REG32                           Idr_Addr1_Write0;                   // 0xbf802b28   0x4a
    #define TSP_IDR_ADDR_MASK1                      0x0000FFFFUL
    #define TSP_IDR_ADDR_SHFT1                      0UL
    #define TSP_IDR_WRITE_MASK0                     0xFFFF0000UL
    #define TSP_IDR_WRITE_SHFT0                     16UL

    REG32                           Idr_Write1_Read0;                   // 0xbf802b30   0x4c
    #define TSP_IDR_WRITE_MASK1                     0x0000FFFFUL
    #define TSP_IDR_WRITE_SHFT1                     0UL
    #define TSP_IDR_READ_MASK0                      0xFFFF0000UL
    #define TSP_IDR_READ_SHFT0                      16UL

    REG32                           Idr_Read1;                          // 0xbf802b38   0x4e
    #define TSP_IDR_READ_MASK1                      0x0000FFFFUL
    #define TSP_IDR_READ_SHFT1                      0UL
    #define TSP_V3D_FIFO_DISCON                     0x00100000UL
    #define TSP_V3D_FIFO_OVERFLOW                   0x00200000UL
    #define TSP_VD_FIFO_DISCON                      0x02000000UL
    #define TSP_VD_FIFO_OVERFLOW                    0x08000000UL
    #define TSP_AUB_FIFO_OVERFLOW                   0x10000000UL
    #define TSP_AU_FIFO_OVERFLOW                    0x20000000UL
    #define TSP_AUD_FIFO_OVERFLOW                   0x40000000UL
    #define TSP_AUC_FIFO_OVERFLOW                   0x80000000UL

    // only 25 bits supported in PVR address. 8 bytes address
    REG32                           TsRec_Head;                         // 0xbf802b40   0x50
    REG32                           TsRec_Mid_PVR1_WPTR;                // 0xbf802b48   0x52, PVR1 mid address & write point
    REG32                           TsRec_Tail;                         // 0xbf802b50   0x54

    REG16                           SW_Mail_Box1;                       // 0xbf802b58   0x56
    REG16                           SW_Mail_Box2;                       // 0xbf802b5C   0x57
    REG32                           _xbf802b60;                         // 0xbf802b60 ~ 0xbf802b64   0x58~0x59

    REG32                           reg15b4;                            // 0xbf802b68   0x5a
    #define TSP_SEC_DMAW_PROTECT_EN                 0x00000001UL
    #define TSP_PVR1_DAMW_PROTECT_EN                0x00000002UL
    #define TSP_PVR2_DAMW_PROTECT_EN                0x00000004UL
    #define TSP_PVR_PID_BYPASS                      0x00000008UL          // Set 1 to bypass PID in record
    #define TSP_PVR_PID_BYPASS2                     0x00000010UL          // Set 1 to bypass PID in record2
    #define TSP_BD_AUD_EN                           0x00000020UL          // Set 1 to enable the BD audio stream recognization ( core /extend audio stream) to Audio A/B
    #define TSP_BD_AUD_EN2                          0x00000040UL          // Set 1 to enable the BD audio stream recognization ( core /extend audio stream) to Audio C/D
    #define TSP_AVFIFO_RD_EN                        0x00000080UL          // 0: AFIFO and VFIFO read are connected to MVD and MAD,  1: AFIFO and VFIFO read are controlled by registers (0x15B5[2:0])
    #define TSP_AVFIFO_RD                           0x00000100UL          // If AVFIFO_RD_EN is 1, set to 1, then set to 0 would issue a read strobe to AFIFO or VFIFO
    #define TSP_AVFIFO_SEL_VIDEO                    0x00000000UL
    #define TSP_AVFIFO_SEL_AUDIO                    0x00000200UL
    #define TSP_AVFIFO_SEL_AUDIOB                   0x00000400UL
    #define TSP_AVFIFO_SEL_V3D                      0x00000600UL
    #define TSP_PVR_INVERT                          0x00001000UL          // Set 1 to enable data payload invert for PVR record
    #define TSP_PLY_FILE_INV_EN                     0x00002000UL          // Set 1 to enable data payload invert in pidflt0 file path
    #define TSP_PLY_TS_INV_EN                       0x00004000UL          // Set 1 to enable data payload invert in pidflt0 TS path
    #define TSP_FILEIN_BYTETIMER_ENABLE             0x00008000UL          // Set 1 to enable byte timer in ts_if0 TS path
    #define TSP_PVR1_PINGPONG                       0x00010000UL          // Set 1 to enable MIU addresses with pinpon mode
    #define TSP_TEI_SKIPE_PKT_PID0                  0x00040000UL          // Set 1 to skip error packets in pidflt0 TS path
    #define TSP_TEI_SKIPE_PKT_FILE                  0x00080000UL          // Set 1 to skip error packets in pidflt0 file path
    #define TSP_TEI_SKIPE_PKT_PID1                  0x00100000UL          // Set 1 to skip error packets in pidflt1 TS path
    #define TSP_DUP_PKT_SKIP                        0x00400000UL
    #define TSP_64bit_PCR2_ld                       0x00800000UL          // Set 1 to load CNT_64B_2 (the second STC)
    #define TSP_cnt_33b_ld                          0x01000000UL          // Set 1 to load cnt_33b
    #define TSP_FORCE_SYNCBYTE                      0x02000000UL          // Set 1 to force sync byte (8'h47) in ts_if0 and ts_if1 path.
    #define TSP_SERIAL_EXT_SYNC_LT                  0x04000000UL          // Set 1 to detect serial-in sync without 8-cycle mode
    #define TSP_BURST_LEN_MASK                      0x18000000UL          // 00,01:    burst length = 4; 10,11: burst length = 1
    #define TSP_BURST_LEN_4                         0x08000000UL
    #define TSP_BURST_LEN_SHIFT                     27UL
    #define TSP_MATCH_PID_SRC_MASK                  0x60000000UL          // Select the source of pid filter number with hit pid and match pid number with scramble information, 00 : from pkt_demux0, 01 : from pkt_demux_file, 10 : from pkt_demux1, 11 : from pkt_demux2
    #define TSP_MATCH_PID_SRC_SHIFT                 29UL
        #define TSP_MATCH_PID_SRC_PKTDMX0           0UL
        #define TSP_MATCH_PID_SRC_PKTDMXFL          1UL
        #define TSP_MATCH_PID_SRC_PKTDMX1           2UL
        #define TSP_MATCH_PID_SRC_PKTDMX2           3UL
    #define TSP_MATCH_PID_LD                        0x80000000UL

    REG32                           TSP_MATCH_PID_NUM;                  // 0xbf802b70   0x5c

    REG32                           TSP_IWB_WAIT;                       // 0xbf802b78   0x5e  // Wait count settings for IWB when TSP CPU i-cache is enabled.

    REG32                           Cpu_Base;                           // 0xbf802b80   0x60
    #define TSP_CPU_BASE_ADDR_MASK                  0x01FFFFFFUL

    REG32                           Qmem_Ibase;                         // 0xbf802b88   0x62

    REG32                           Qmem_Imask;                         // 0xbf802b90   0x64

    REG32                           Qmem_Dbase;                         // 0xbf802b98   0x66

    REG32                           Qmem_Dmask;                         // 0xbf802ba0   0x68

    REG32                           TSP_Debug;                          // 0xbf802ba8   0x6a
    #define TSP_DEBUG_MASK                          0x00FFFFFFUL

    REG32                           _xbf802bb0;                         // 0xbf802bb0   0x6c

    REG32                           TsFileIn_RPtr;                      // 0xbf802bb8   0x6e

    REG32                           TsFileIn_Timer;                     // 0xbf802bc0   0x70
    #define TSP_FILE_TIMER_MASK                     0x00FFFFFFUL
    REG32                           TsFileIn_Head;                      // 0xbf802bc8   0x72
    #define TSP_FILE_ADDR_MASK                      0x07FFFFFFUL
    REG32                           TsFileIn_Mid;                       // 0xbf802bd0   0x74

    REG32                           TsFileIn_Tail;                      // 0xbf802bd8   0x76

    REG32                           Dnld_Ctrl;                          // 0xbf802be0   0x78
    #define TSP_DNLD_ADDR_MASK                      0x0000FFFFUL
    #define TSP_DNLD_ADDR_SHFT                      0UL
    #define TSP_DNLD_ADDR_ALI_SHIFT                 4UL                   // Bit [11:4] of DMA_RADDR[19:0]
    #define TSP_DNLD_NUM_MASK                       0xFFFF0000UL
    #define TSP_DNLD_NUM_SHFT                       16UL

    REG32                           TSP_Ctrl;                           // 0xbf802be8   0x7a
    #define TSP_CTRL_CPU_EN                         0x00000001UL
    #define TSP_CTRL_SW_RST                         0x00000002UL
    #define TSP_CTRL_DNLD_START                     0x00000004UL
    #define TSP_CTRL_DNLD_DONE                      0x00000008UL          // See 0x78 for related information
    #define TSP_CTRL_TSFILE_EN                      0x00000010UL
    #define TSP_CTRL_R_PRIO                         0x00000020UL
    #define TSP_CTRL_W_PRIO                         0x00000040UL
    #define TSP_CTRL_ICACHE_EN                      0x00000100UL
    #define TSP_CTRL_CPU2MI_R_PRIO                  0x00000400UL
    #define TSP_CTRL_CPU2MI_W_PRIO                  0x00000800UL
    #define TSP_CTRL_I_EL                           0x00000000UL
    #define TSP_CTRL_I_BL                           0x00001000UL
    #define TSP_CTRL_D_EL                           0x00000000UL
    #define TSP_CTRL_D_BL                           0x00002000UL
    #define TSP_CTRL_NOEA_QMEM_ACK_DIS              0x00004000UL
    #define TSP_CTRL_MEM_TS_WORDER                  0x00008000UL
    #define TSP_SYNC_BYTE_MASK                      0x00FF0000UL
    #define TSP_SYNC_BYTE_SHIFT                     16UL

    REG32                           PKT_CNT;                            // 0xbf802bf0   0x7c
    #define TSP_PKT_CNT_MASK                        0x000000FFUL
    #define TSP_DBG_SEL_MASK                        0xFFFF0000UL
    #define TSP_DBG_SEL_SHIFT                       16UL

    REG16                           HwInt_Stat;                         // 0xbf802bf8   0x7e
    #define TSP_HWINT_STATUS_MASK                   0xFF00UL              // Tsp2hk_int enable bits.
    #define TSP_HWINT_TSP_PVR_TAIL0_STATUS          0x0100UL
    #define TSP_HWINT_TSP_PVR_MID0_STATUS           0x0200UL
    #define TSP_HWINT_TSP_HK_INT_FORCE_STATUS       0x0400UL
    #define TSP_HWINT_TSP_FILEIN_MID_INT_STATUS     0x0800UL
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_STATUS    0x1000UL
    #define TSP_HWINT_TSP_SW_INT_STATUS             0x2000UL
    #define TSP_HWINT_TSP_DMA_READ_DONE             0x4000UL
    #define TSP_HWINT_TSP_AV_PKT_ERR                0x8000UL

    #define TSP_HWINT_HW_PVR1_MASK                  (TSP_HWINT_TSP_PVR_TAIL0_STATUS | TSP_HWINT_TSP_PVR_MID0_STATUS)
    #define TSP_HWINT_ALL                           (TSP_HWINT_HW_PVR1_MASK | TSP_HWINT_TSP_SW_INT_STATUS)

    // 0x7f: TSP_CTRL1: hidden in HwInt_Stat
    REG16                           TSP_Ctrl1;                          // 0xbf802bfc   0x7f
    #define TSP_CTRL1_FILEIN_TIMER_ENABLE           0x0001UL
    #define TSP_CTRL1_TSP_FILE_NON_STOP             0x0002UL              //Set 1 to enable TSP file data read without timer check
    #define TSP_CTRL1_FILEIN_PAUSE                  0x0004UL
    #define TSP_CTRL1_STANDBY                       0x0080UL
    #define TSP_CTRL1_INT2NOEA                      0x0100UL
    #define TSP_CTRL1_INT2NOEA_FORCE                0x0200UL
    #define TSP_CTRL1_FORCE_XIU_WRDY                0x0400UL
    #define TSP_CTRL1_CMDQ_RESET                    0x0800UL
    #define TSP_CTRL1_DLEND_EN                      0x1000UL              // Set 1 to enable little-endian mode in TSP CPU
    #define TSP_CTRL1_PVR_CMD_QUEUE_ENABLE          0x2000UL
    #define TSP_CTRL1_DMA_RST                       0x8000UL

    //----------------------------------------------
    // 0xBF802C00 MIPS direct access
    //----------------------------------------------
    REG32                           MCU_Data0;                          // 0xbf802c00   0x00
    #define TSP_MCU_DATA_ALIVE                      TSP_MCU_CMD_ALIVE

    REG32                           PVR1_LPcr1;                         // 0xbf802c08   0x02

    REG32                           LPcr2;                              // 0xbf802c10   0x04

    REG32                           reg160C;                            // 0xbf802c18   0x06
    #define TSP_PVR1_LPCR1_WLD                      0x00000001UL          // Set 1 to load LPCR1 value
    #define TSP_PVR1_LPCR1_RLD                      0x00000002UL          // Set 1 to read LPCR1 value (Default: 1)
    #define TSP_LPCR2_WLD                           0x00000004UL          // Set 1 to load LPCR2 value
    #define TSP_LPCR2_RLD                           0x00000008UL          // Set 1 to read LPCR2 value (Default: 1)
    #define TSP_RECORD192_EN                        0x00000010UL          // 160C bit(5)enable TS packets with 192 bytes on record mode
    #define TSP_FILEIN192_EN                        0x00000020UL          // 160C bit(5)enable TS packets with 192 bytes on file-in mode
    #define TSP_RVU_TIMESTAMP_EN                    0x00000040UL
    #define TSP_ORZ_DMAW_PROT_EN                    0x00000080UL          // 160C bit(7) open RISC DMA write protection
    #define TSP_CLR_PIDFLT_BYTE_CNT                 0x00000100UL          // Clear pidflt0_file byte counter
    #define TSP_DOUBLE_BUF_DESC                     0x00004000UL          // 160d bit(6) remove buffer limitation, Force pinpong buffer to flush
    #define TSP_TIMESTAMP_RESET                     0x00008000UL          // 160d bit(7) reset timestamp, reset all file in path
    #define TSP_VQTX0_BLOCK_DIS                     0x00010000UL
    #define TSP_VQTX1_BLOCK_DIS                     0x00020000UL
    #define TSP_VQTX2_BLOCK_DIS                     0x00040000UL
    #define TSP_VQTX3_BLOCK_DIS                     0x00080000UL
    #define TSP_DIS_MIU_RQ                          0x00100000UL          // Disable miu R/W request for reset TSP usage
    #define TSP_RM_DMA_GLITCH                       0x00800000UL          // Fix sec_dma overflow glitch
    #define TSP_RESET_VFIFO                         0x01000000UL          // Reset VFIFO -- ECO Done
    #define TSP_RESET_AFIFO                         0x02000000UL          // Reset AFIFO -- ECO Done
    #define TSP_RESET_GDMA                          0x04000000UL          // Set 1 to reset GDMA bridge
    #define TSP_CLR_ALL_FLT_MATCH                   0x08000000UL          // Set 1 to clean all flt_match in a packet
    #define TSP_RESET_AFIFO2                        0x10000000UL
    #define TSP_RESET_VFIFO3D                       0x20000000UL
    #define TSP_PVR_WPRI_HIGH                       0x20000000UL
    #define TSP_OPT_ORACESS_TIMING                  0x80000000UL

    REG32                           PktChkSizeFilein;                   // 0xbf802c20   0x08
    #define TSP_PKT_SIZE_MASK                       0x000000ffUL
    #define TSP_PKT192_BLK_DIS_FIN                  0x00000100UL          // Set 1 to disable file-in timestamp block scheme
    #define TSP_AV_CLR                              0x00000200UL          // Clear AV FIFO overflow flag and in/out counter
    #define TSP_HW_STANDBY_MODE                     0x00000400UL          // Set 1 to disable all SRAM in TSP for low power mode automatically
    #define TSP_CNT_34B_DEFF_EN                     0x00020000UL          // Switch STC DIFF Mode (Output STC+DIFF to MVD and MAD)
    #define TSP_SYSTIME_MODE_STC64                  0x00080000UL          // Switch normal STC or STC diff
    #define TSP_SEC_DMA_BURST_EN                    0x00800000UL          // ECO bit for section DMA burst mode
    #define TSP_REMOVE_DUP_VIDEO_PKT                0x02000000UL          // Set 1 to remove duplicate video packet
    #define TSP_REMOVE_DUP_VIDEO3D_PKT              0x04000000UL          // Set 1 to remove duplicate video 3D packet
    #define TSP_REMOVE_DUP_AUDIO_PKT                0x08000000UL          // Set 1 to remove duplicate audio packet
    #define TSP_REMOVE_DUP_AUDIOB_PKT               0x10000000UL          // Set 1 to remove duplicate audio description packet
    #define TSP_REMOVE_DUP_AUDIOC_PKT               0x20000000UL          // Set 1 to remove duplicate third audio fifo packet
    #define TSP_REMOVE_DUP_AUDIOD_PKT               0x40000000UL          // Set 1 to remove duplicate fourth audio fifo packet

    #define TSP_REMOVE_DUP_AV_PKT (TSP_REMOVE_DUP_VIDEO_PKT   | \
                                   TSP_REMOVE_DUP_VIDEO3D_PKT | \
                                   TSP_REMOVE_DUP_AUDIO_PKT   | \
                                   TSP_REMOVE_DUP_AUDIOB_PKT  )

    REG32                           Dnld_Ctrl2;                         // 0xbf802c28   0x0a
    #define TSP_DMA_RADDR_MSB_MASK                  0x000000FFUL
    #define TSP_DMA_RADDR_MSB_SHIFT                 0UL
    //#define TSP_CMQ_WORD_EN                         0x00400000UL          // Set 1 to access CMDQ related registers in word.
    //#define TSP_RESET_PVR_MOBF                      0x04000000UL
    //#define TSP_RESET_FILEIN_MOBF                   0x08000000UL
    #define TSP_TSIF0_VPID_3D_BYPASS                0x08000000UL          // bypass TS for matched video 3D pid
    #define TSP_VPID_3D_ERR_RM_EN                   0x10000000UL          // enable removing v3d err pkt
    #define TSP_PS_VID3D_EN                         0x40000000UL

    REG32                           TsPidScmbStatTsin;                  // 0xbf802c30   0x0c

    REG32                           _xbf802c38;                         // 0xbf802c38   0x0e

    REG32                           PCR64_2_L;                          // 0xbf802c40   0x10

    REG32                           PCR64_2_H;                          // 0xbf802c48   0x12

    #define TSP_DMAW_BND_MASK                       0xFFFFFFFFFUL
    REG32                           DMAW_LBND0;                         // 0xbf802c50   0x14

    REG32                           DMAW_UBND0;                         // 0xbf802c58   0x16

    REG32                           DMAW_LBND1;                         // 0xbf802c60   0x18

    REG32                           DMAW_UBND1;                         // 0xbf802c68   0x1A

    REG32                           DMAW_ERR_WADDR_SRC_SEL;             // 0xbf802c70   0x1C
    #define TSP_CLR_NO_HIT_INT                      0x00000001UL         // set 1 clear all dma write function not hit interrupt
    #define DMAW_ERR_WADDR_SRC_SEL_MASK             0x0000001EUL
    #define DMAW_ERR_WADDR_SRC_SEL_SHIFT            1UL
        #define TSP_PVR1_DWMA_WADDR_ERR                 0x0UL
        #define TSP_SEC_DWMA_WADDR_ERR                  0x1UL
        #define TSP_PVR_CB_DWMA_WADDR_ERR               0x2UL
        #define TSP_VQTX0_DWMA_WADDR_ERR                0x3UL
        #define TSP_VQTX1_DWMA_WADDR_ERR                0x4UL
        #define TSP_ORZ_DWMA_WADDR_ERR                  0x5UL
        #define TSP_VQTX2_DWMA_WADDR_ERR                0x6UL
        #define TSP_VQTX3_DWMA_WADDR_ERR                0x7UL
        #define TSP_PVR2_DWMA_WADDR_ERR                 0x8UL
    #define TSP_CLR_SEC_DMAW_OVERFLOW               0x00000040UL
    #define TSP_APES_B_ERR_RM_EN                    0x00000080UL
    #define TSP_BLK_AF_SCRMB_BIT                    0x00000400UL

    REG32                           reg163C;                            // 0xbf802c78   0x1e
    #define TSP_AUDC_SRC_MASK                       0x00000007UL
    #define TSP_AUDC_SRC_SHIFT                      0UL
    #define TSP_AUDD_SRC_MASK                       0x00000038UL
    #define TSP_AUDD_SRC_SHIFT                      3UL
    #define TSP_CLR_SRC_MASK                        0x00070000UL
    #define TSP_CLR_SRC_SHIFT                       16UL
    #define TSP_DISCONTI_VD_CLR                     0x00080000UL  //Set 1 to clear video discontinuity count
    #define TSP_DISCONTI_V3D_CLR                    0x00100000UL  //Set 1 to clear v3D discontinuity count
    #define TSP_DISCONTI_AUD_CLR                    0x00200000UL  //Set 1 to clear audio discontinuity count
    #define TSP_DISCONTI_AUDB_CLR                   0x00400000UL  //Set 1 to clear videoB discontinuity count
    #define TSL_CLR_SRAM_COLLISION                  0x02000000UL
    #define TSP_TS_OUT_EN                           0x04000000UL  //set 1 to enable ts_out

    #define TSP_ALL_VALID_EN                        0x08000000UL
    #define TSP_PKT130_PUSI_EN                      0x10000000UL
    #define TSP_PKT130_TEI_EN                       0x20000000UL
    #define TSP_PKT130_ERR_CLR                      0x40000000UL
    #define TSP_PKT130_EN                           0x80000000UL // file in only

    REG32                           VQ0_BASE;                           // 0xbf802c80   0x20
    REG32                           VQ0_CTRL;                           // 0xbf802c88   0x22
    #define TSP_VQ0_SIZE_208PK_MASK                 0x0000FFFFUL
    #define TSP_VQ0_SIZE_208PK_SHIFT                0UL
    #define TSP_VQ0_WR_THRESHOLD_MASK               0x000F0000UL
    #define TSP_VQ0_WR_THRESHOLD_SHIFT              16UL
    #define TSP_VQ0_PRIORTY_THRESHOLD_MASK          0x00F00000UL
    #define TSP_VQ0_PRIORTY_THRESHOL_SHIFT          20UL
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_MASK          0x0F000000UL
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT         24UL
    #define TSP_VQ0_RESET                           0x10000000UL
    #define TSP_VQ0_OVERFLOW_INT_EN                 0x40000000UL          // Enable the interrupt for overflow happened on Virtual Queue path
    #define TSP_VQ0_CLR_OVERFLOW_INT                0x80000000UL         // Clear the interrupt and the overflow flag

    REG32                           VQ_PIDFLT_CTRL;                    // 0xbf802c90   0x24
    #define TSP_REQ_VQ_RX_THRESHOLD_MASKE           0x000E0000UL
    #define TSP_REQ_VQ_RX_THRESHOLD_SHIFT           17UL
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN1            0x00000000UL
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN2            0x00020000UL
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN4            0x00040000UL
    #define TSP_REQ_VQ_RX_THRESHOLD_LEN8            0x00060000UL
    #define TSP_PIDFLT0_OVF_INT_EN                  0x00400000UL
    #define TSP_PIDFLT0_CLR_OVF_INT                 0x00800000UL
    #define TSP_PIDFLT0_FILE_OVF_INT_EN             0x01000000UL
    #define TSP_PIDFLT0_FILE_CLR_OVF_INT            0x02000000UL
    #define TSP_PIDFLT1_OVF_INT_EN                  0x04000000UL
    #define TSP_PIDFLT1_CLR_OVF_INT                 0x08000000UL
    #define TSP_PIDFLT2_OVF_INT_EN                  0x10000000UL
    #define TSP_PIDFLT2_CLR_OVF_INT                 0x20000000UL

    REG32                           MOBF_PVR1_Index;                    // 0xbf3a2c98   0x26
    #define TSP_MOBF_PVR1_INDEX0_MASK               0x0000000FUL
    #define TSP_MOBF_PVR1_INDEX0_SHIFT              0UL
    #define TSP_MOBF_PVR1_INDEX1_MASK               0x000F0000UL
    #define TSP_MOBF_PVR1_INDEX1_SHIFT              16UL

    REG32                           MOBF_PVR2_Index;                    // 0xbf3a2cA0   0x28
    #define TSP_MOBF_PVR2_INDEX0_MASK               0x0000000FUL
    #define TSP_MOBF_PVR2_INDEX0_SHIFT              0UL
    #define TSP_MOBF_PVR2_INDEX1_MASK               0x000F0000UL
    #define TSP_MOBF_PVR2_INDEX1_SHIFT              16UL

    REG32                           DMAW_LBND2;                         // 0xbf802ca8   0x2a

    REG32                           DMAW_UBND2;                         // 0xbf802cb0   0x2c

    REG32                           DMAW_LBND3;                         // 0xbf802cb8   0x2e          //reserved

    REG32                           DMAW_UBND3;                         // 0xbf802cc0   0x30          //reserved

    REG32                           DMAW_LBND4;                         // 0xbf802cc8   0x32

    REG32                           DMAW_UBND4;                         // 0xbf802cd0   0x34

    REG32                           ORZ_DMAW_LBND;                      // 0xbf802cd8   0x36
    #define TSP_ORZ_DMAW_LBND_MASK                  0xffffffffUL
    REG32                           ORZ_DMAW_UBND;                      // 0xbf802ce0   0x38
    #define TSP_ORZ_DMAW_UBND_MASK                  0xffffffffUL
    REG32                           _xbf802ce8_xbf802cec;               // 0xbf802ce8_0xbf802cec  0x3a~0x3b

    REG32                           HWPCR0_L;                           // 0xbf802cf0   0x3c
    REG32                           HWPCR0_H;                           // 0xbf802cf8   0x3e

    REG32                           CA_CTRL;                            // 0xbf802d00   0x40
    #define TSP_CA_CTRL_MASK                        0xffffffffUL
    #define TSP_CA0_CTRL_MASK                       0x00007077UL
    #define TSP_CA0_INPUT_TSIF0_LIVEIN              0x00000001UL
    #define TSP_CA0_INPUT_TSIF0_FILEIN              0x00000002UL
    #define TSP_CA0_INPUT_TSIF1                     0x00000004UL
    #define TSP_CA0_OUTPUT_PKTDMX0_LIVE             0x00000010UL
    #define TSP_CA0_OUTPUT_PKTDMX0_FILE             0x00000020UL
    #define TSP_CA0_OUTPUT_PKTDMX1                  0x00000040UL
    #define TSP_CA0_INPUT_TSIF2                     0x00001000UL
    #define TSP_CA0_OUTPUT_PKTDMX2                  0x00002000UL

    #define TSP_CA1_CTRL_MASK                       0x77300000UL
    #define TSP_CA1_INPUT_TSIF2                     0x00100000UL
    #define TSP_CA1_OUTPUT_PKTDMX2                  0x00200000UL
    
    #define TSP_CA2_CTRL_MASK_L                     0x00C00000UL 
    #define TSP_CA2_INPUT_TSIF2                     0x00400000UL
    #define TSP_CA2_OUTPUT_PKTDMX2                  0x00800000UL
        
    #define TSP_CA1_INPUT_TSIF0_LIVEIN              0x01000000UL
    #define TSP_CA1_INPUT_TSIF0_FILEIN              0x02000000UL
    #define TSP_CA1_INPUT_TSIF1                     0x04000000UL
    #define TSP_CA1_OUTPUT_PKTDMX0_LIVE             0x10000000UL
    #define TSP_CA1_OUTPUT_PKTDMX0_FILE             0x20000000UL
    #define TSP_CA1_OUTPUT_PKTDMX1                  0x40000000UL

    REG32                           REG_ONEWAY;                         // 0xbf802d08   0x42
    #define TSP_ONEWAY_PVR1_PORT                    0x00000002UL          // Oneway for PVR1 buffer
    #define TSP_ONEWAY_PVR2_PORT                    0x00000004UL          // Oneway for PVR2 buffer
    #define TSP_ONEWAY_LOAD_FW_PORT                 0x00000008UL          // Oneway for f/w load address
    #define TSP_ONEWAY_QMEM                         0x00000010UL
    #define TSP_ONEWAY_AV_NOT_TO_SEC                0x00000020UL          // Oneway for block av packet to section
    
    #define TSP_CA2_CTRL_MASK_H                     0x00770000UL
    #define TSP_CA2_CTRL_SHIFT_H                    16UL
    #define TSP_CA2_INPUT_TSIF0_LIVEIN              0x00000001UL
    #define TSP_CA2_INPUT_TSIF0_FILEIN              0x00000002UL
    #define TSP_CA2_INPUT_TSIF1                     0x00000004UL
    #define TSP_CA2_OUTPUT_PKTDMX0_LIVE             0x00000010UL
    #define TSP_CA2_OUTPUT_PKTDMX0_FILE             0x00000020UL
    #define TSP_CA2_OUTPUT_PKTDMX1                  0x00000040UL

    #define TSP_CA3_CTRL_MASK                       0x7F800000UL
    #define TSP_CA3_INPUT_TSIF0_LIVEIN              0x00800000UL
    #define TSP_CA3_INPUT_TSIF0_FILEIN              0x01000000UL
    #define TSP_CA3_INPUT_TSIF1                     0x02000000UL
    #define TSP_CA3_INPUT_TSIF2                     0x04000000UL
    #define TSP_CA3_OUTPUT_PKTDMX0_LIVE             0x08000000UL
    #define TSP_CA3_OUTPUT_PKTDMX0_FILE             0x10000000UL
    #define TSP_CA3_OUTPUT_PKTDMX1                  0x20000000UL 
    #define TSP_CA3_OUTPUT_PKTDMX2                  0x40000000UL 
    
    
    REG32                           HWPCR1_L;                           // 0xbf802d10   0x44
    REG32                           HWPCR1_H;                           // 0xbf802d18   0x46

    REG32                           _xbf802d20[4];                         // 0xbf802d20~0xbf802d3c   0x48~0x4f   //LPCR_CB

    REG32                           FIFO_Src;                           // 0xbf802d40   0x50
    #define TSP_AUD_SRC_MASK                        0x00000007UL
    #define TSP_AUD_SRC_SHIFT                       0UL
        #define TSP_SRC_FROM_PKTDMX0                0x00000001UL
        #define TSP_SRC_FROM_PKTDMXFL               0x00000002UL
        #define TSP_SRC_FROM_PKTDMX1                0x00000003UL
        #define TSP_SRC_FROM_PKTDMX2                0x00000004UL
        #define TSP_SRC_FROM_MMFI0                  0x00000006UL
        #define TSP_SRC_FROM_MMFI1                  0x00000007UL
    #define TSP_AUDB_SRC_MASK                       0x00000038UL
    #define TSP_AUDB_SRC_SHIFT                      3UL
    #define TSP_VID_SRC_MASK                        0x000001C0UL
    #define TSP_VID_SRC_SHIFT                       6UL
    #define TSP_VID3D_SRC_MASK                      0x00000E00UL
    #define TSP_VID3D_SRC_SHIFT                     9UL
    #define TSP_PVR1_SRC_MASK                       0x00007000UL
    #define TSP_PVR1_SRC_SHIFT                      12UL
    #define TSP_PCR0_SRC_MASK                       0x001C0000UL
    #define TSP_PCR0_SRC_SHIFT                      18UL
    #define TSP_PCR1_SRC_MASK                       0x00E00000UL
    #define TSP_PCR1_SRC_SHIFT                      21UL
    #define TSP_TEI_SKIP_PKT_PCR0                   0x01000000UL
    #define TSP_PCR0_RESET                          0x02000000UL
    #define TSP_PCR0_READ                           0x08000000UL
    #define TSP_TEI_SKIP_PKT_PCR1                   0x10000000UL
    #define TSP_PCR1_RESET                          0x20000000UL
    #define TSP_PCR1_READ                           0x80000000UL

    REG32                           STC_DIFF_BUF;                       // 0xbf802d48   0x52

    REG32                           STC_DIFF_BUF_H;                     // 0xbf802d50   0x54
    #define TSP_STC_DIFF_BUF_H_MASK                 0x0000007FUL
    #define TSP_STC_DIFF_BUF_H_AHIFT                0UL
    #define TSP_PVR2_SRC_MASK                       0x00070000UL
    #define TSP_PVR2_SRC_SHIFT                      16UL


    REG32                           VQ1_Base;                           // 0xbf802d58   0x56

    REG32                           _rbf802d60;                         // 0xbf802d60   0x58

    REG32                           CH_BW_CTRL;                         // 0xbf802d68   0x5a
    #define TSP_CH_BW_WP_LD                         0x00000100UL

    REG32                           VQ1_Config;                         // 0xbf802d70   0x5C
    #define TSP_VQ1_SIZE_208BYTE_MASK               0x0000ffffUL
    #define TSP_VQ1_SIZE_208BYTE_SHIFT              0UL
    #define TSP_VQ1_WR_THRESHOLD_MASK               0x000F0000UL
    #define TSP_VQ1_WR_THRESHOLD_SHIFT              16UL
    #define TSP_VQ1_PRI_THRESHOLD_MASK              0x00F00000UL
    #define TSP_VQ1_PRI_THRESHOLD_SHIFT             20UL
    #define TSP_VQ1_FORCEFIRE_CNT_1K_MASK           0x0F000000UL
    #define TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT          24UL
    #define TSP_VQ1_RESET                           0x10000000UL
    #define TSP_VQ1_OVF_INT_EN                      0x40000000UL
    #define TSP_VQ1_CLR_OVF_INT                     0x80000000UL

    REG32                           VQ2_Base;                           // 0xbf802d78   0x5E

    REG32                           Pkt_Info3;                          // 0xbf802d80   0x60
    #define TSP_AFIFOC_STATUS                       0x0000000FUL
    #define TSP_AFIFOC_STATUS_SHFT                  0UL
    #define TSP_AFIFOD_STATUS                       0x000000F0UL
    #define TSP_AFIFOD_STATUS_SHFT                  4UL

    REG32                           Bist_Fail;                          // 0xbf802d88   0x62
    #define TSP_BIST_FAIL_STATUS_MASK               0x00FF0000UL
    #define TSP_BIST_FAIL_STATUS_SRAM1P192x8_MASK   0x00070000UL
    #define TSP_BIST_FAIL_STATUS_SRAM2P512x32w8     0x00080000UL
    #define TSP_BIST_FAIL_STATUS_SRAM2P16x128_MASK  0x00600000UL
    #define TSP_BIST_FAIL_STATUS_SRAM1P2048x32w8    0x00800000UL
    #define TSP_BIST_FAIL_STATUS_SRAM1P1024x32w8    0x01000000UL
    #define TSP_BIST_FAIL_STATUS_SRAM1P512x20       0x00200000UL

    REG32                           VQ2_Config;                         // 0xbf802d90   0x64
    #define TSP_VQ2_SIZE_208BYTE_MASK               0x0000ffffUL
    #define TSP_VQ2_SIZE_208BYTE_SHIFT              0UL
    #define TSP_VQ2_WR_THRESHOLD_MASK               0x000F0000UL
    #define TSP_VQ2_WR_THRESHOLD_SHIFT              16UL
    #define TSP_VQ2_PRI_THRESHOLD_MASK              0x00F00000UL
    #define TSP_VQ2_PRI_THRESHOLD_SHIFT             20UL
    #define TSP_VQ2_FORCEFIRE_CNT_1K_MASK           0x0F000000UL
    #define TSP_VQ2_FORCEFIRE_CNT_1K_SHIFT          24UL
    #define TSP_VQ2_RESET                           0x10000000UL
    #define TSP_VQ2_OVF_INT_EN                      0x40000000UL
    #define TSP_VQ2_CLR_OVF_INT                     0x80000000UL

    REG32                           VQ_STATUS;                          // 0xbf802d98   0x66
    #define TSP_VQ_STATUS_MASK                      0xFFFFFFFFUL
    #define TSP_VQ_STATUS_SHIFT                     0UL
    #define TSP_VQ0_STATUS_READ_EVER_FULL           0x00001000UL
    #define TSP_VQ0_STATUS_READ_EVER_OVERFLOW       0x00002000UL
    #define TSP_VQ0_STATUS_EMPTY                    0x00004000UL
    #define TSP_VQ0_STATUS_READ_BUSY                0x00008000UL
    #define TSP_VQ1_STATUS_READ_EVER_FULL           0x00010000UL
    #define TSP_VQ1_STATUS_READ_EVER_OVERFLOW       0x00020000UL
    #define TSP_VQ1_STATUS_EMPTY                    0x00040000UL
    #define TSP_VQ1_STATUS_READ_BUSY                0x00080000UL
    #define TSP_VQ2_STATUS_READ_EVER_FULL           0x00100000UL
    #define TSP_VQ2_STATUS_READ_EVER_OVERFLOW       0x00200000UL
    #define TSP_VQ2_STATUS_EMPTY                    0x00400000UL
    #define TSP_VQ2_STATUS_READ_BUSY                0x00800000UL
    #define TSP_VQ3_STATUS_READ_EVER_FULL           0x01000000UL
    #define TSP_VQ3_STATUS_READ_EVER_OVERFLOW       0x02000000UL
    #define TSP_VQ3_STATUS_EMPTY                    0x04000000UL
    #define TSP_VQ3_STATUS_READ_BUSY                0x08000000UL
    #define TSP_VQ0_STATUS_TX_OVERFLOW              0x10000000UL
    #define TSP_VQ1_STATUS_TX_OVERFLOW              0x20000000UL
    #define TSP_VQ2_STATUS_TX_OVERFLOW              0x40000000UL
    #define TSP_VQ3_STATUS_TX_OVERFLOW              0x80000000UL

    REG32                           DM2MI_WAddr_Err;                    // 0xbf802da0   0x68  , DM2MI_WADDR_ERR0

    REG32                           ORZ_DMAW_WAddr_Err;                 // 0xbf802da8   0x6a  , ORZ_WADDR_ERR0

    REG16                           SwInt_Stat1_L;                      // 0xbf802dB0   0x6c
    #define TSP_HWINT2_EN_MASK                      0x00FFUL
    #define TSP_HWINT2_EN_SHIFT                     0UL
    #define TSP_HWINT2_STATUS_MASK                  0xFF00UL
    #define TSP_HWINT2_STATUS_SHIFT                 8UL
    #define TSP_HWINT2_PCR1_UPDATE_END              0x0400UL
    #define TSP_HWINT2_PCR0_UPDATE_END              0x0800UL
    #define TSP_HWINT2_PVRCB_MEET_MID_TAIL          0x1000UL
    #define TSP_HWINT2_ALL_DMA_WADDR_NOT_IN_PROCT_Z 0x2000UL
    #define TSP_HWINT2_VQ0_VQ1_VQ2_VQ3_OVERFLOW     0x4000UL
    #define TSP_HWINT2_PVR2_MID_TAIL_STATUS         0x8000UL

    #define TSP_HWINT_HW_PVRCB_MASK                 TSP_HWINT2_PVRCB_MEET_MID_TAIL
    #define TSP_HWINT_HW_PVR2_MASK                  TSP_HWINT2_PVR2_MID_TAIL_STATUS
    #define TSP_HWINT2_ALL                          (TSP_HWINT_HW_PVRCB_MASK|TSP_HWINT_HW_PVR2_MASK|TSP_HWINT2_PCR0_UPDATE_END|TSP_HWINT2_PCR1_UPDATE_END)

    #define TSP_SWINT1_L_SHFT                       16UL
    #define TSP_SWINT1_L_MASK                       0xFFFF0000UL

    REG16                           SwInt_Stat1_M;
    REG32                           SwInt_Stat1_H;                     // 0xbf802dB8   0x6e
    #define TSP_SWINT1_H_SHFT       0UL
    #define TSP_SWINT1_H_MASK       0x0000FFFFUL

    REG32                           TimeStamp_FileIn;                   // 0xbf802dC0   0x70

    REG32                           HW2_Config3;                        // 0xbf802dC0   0x72
    #define TSP_RM_OVF_GLITCH                       0x00000008UL
    #define TSP_FILEIN_RADDR_READ                   0x00000010UL
    #define TSP_DUP_PKT_CNT_CLR                     0x00000040UL
    #define TSP_REC_AT_SYNC_DIS                     0x00000100UL
    #define TSP_PVR1_ALIGN_EN                       0x00000200UL
    #define TSP_REC_FORCE_SYNC_EN                   0x00000400UL
    #define TSP_RM_PKT_DEMUX_PIPE                   0x00000800UL
    #define TSP_VQ_EN                               0x00004000UL
    #define TSP_VQ2PINGPONG_EN                      0x00008000UL
    #define TSP_PVR1_REC_ALL_EN                     0x00010000UL
    #define TSP_PVR2_REC_ALL_EN                     0x00020000UL
    #define TSP_DMA_FLUSH_EN                        0x00040000UL        //PVR1, PVR2 dma flush
    #define TSP_REC_ALL_OLD                         0x00080000UL
    #define TSP_RESET_AFIFO3                        0x00400000UL
    #define TSP_RESET_AFIFO4                        0x00800000UL
    #define TSP_TSIF0_CLK_STAMP_27_EN               0x01000000UL
    #define TSP_PVR1_CLK_STAMP_27_EN                0x02000000UL
    #define TSP_PVR2_CLK_STAMP_27_EN                0x04000000UL
    #define TSP_HW_CFG3_PS_AUDC_EN                  0x10000000UL
    #define TSP_HW_CFG3_PS_AUDD_EN                  0x20000000UL
    #define TSP_REC_NULL                            0x40000000UL        // No used


    REG32                           VQ3_BASE;                           // 0xbf802dC0   0x74

    REG32                           VQ3_Config;                         // 0xbf802dC0   0x76
    #define TSP_VQ3_SIZE_208BYTE_MASK               0x0000ffffUL
    #define TSP_VQ3_SIZE_208BYTE_SHIFT              0UL
    #define TSP_VQ3_WR_THRESHOLD_MASK               0x000F0000UL
    #define TSP_VQ3_WR_THRESHOLD_SHIFT              16UL
    #define TSP_VQ3_PRI_THRESHOLD_MASK              0x00F00000UL
    #define TSP_VQ3_PRI_THRESHOLD_SHIFT             20UL
    #define TSP_VQ3_FORCEFIRE_CNT_1K_MASK           0x0F000000UL
    #define TSP_VQ3_FORCEFIRE_CNT_1K_SHIFT          24UL
    #define TSP_VQ3_RESET                           0x10000000UL
    #define TSP_VQ3_OVF_INT_EN                      0x40000000UL
    #define TSP_VQ3_CLR_OVF_INT                     0x80000000UL

    REG32                           VQ_RX_Status;                       // 0xbf802dC0   0x78
    #define VQ_RX_ARBITER_MODE_MASK                 0x0000000FUL
    #define VQ_RX_ARBITER_MODE_SHIFT                0UL
    #define VQ_RX0_PRI_MASK                         0x000000F0UL
    #define VQ_RX0_PRI_SHIFT                        4UL
    #define VQ_RX1_PRI_MASK                         0x00000F00UL
    #define VQ_RX1_PRI_SHIFT                        8UL
    #define VQ_RX2_PRI_MASK                         0x0000F000UL
    #define VQ_RX2_PRI_SHIFT                        12UL
    #define VQ_RX3_PRI_MASK                         0x000F0000UL
    #define VQ_RX3_PRI_SHIFT                        16UL

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
    REG16                           PktConverterCfg[4];         // 0x10~13
    #define INPUT_MODE_MASK                                     0x0007UL
    #define INPUT_MODE_SHIF                                     0UL
    // input mode of pkt_converter0(need to turn on reg_eco_fiq_input(TSP5,0c[7]) when flow through FIQ)
    // 0: normal 188
    // 1:CI+1.4 188
    // 2.Opencable
    // 3:192 mode
    // 4:MxL mode   (192 / 196 / 200 bytes, but flexible by pkt_header_len)
    // 3. reg_filter_null_pkt(TSP5,06[2])
    // 5:Nagra Dongle mode (192 bytes, but flexible by pkt_header_len & sync_byte_position)
    #define FORCE_SYNC_0X47                                     0x0008UL
    #define BYPASS_PKT_CONVERTER                                0x0010UL
    #define BYPASS_SRC_ID_PARSER                                0x0020UL
    #define SRC_ID_FLT_EN                                       0x0040UL
    #define MXL_TS_HEADER_LEN_MASK                              0x0F80UL
    #define MXL_TS_HEADER_LEN_SHFT                              0x7UL
    // 4 : Mxl 192 or Nagra Dongle 192
    // 8 : Mxl 196
    // 12 : Mxl 200
    #define SYNC_BYTE_POS_MASK                                  0xF000UL
    #define SYNC_BYTE_POS_SHFT                                  0x12UL
    // 1 : Nagra Dongle 192

    REG16                           HW3_Cfg0;                   //0x14
    #define PREVENT_SRAM_COLLISION                              0x0001UL
    #define PUSI_THREE_BYTE_MODE                                0x0002UL
    #define PVR1_TIMESTAMP_SRC                                  0x0004UL // 1: FIQ  0: LPCR
    #define PVR2_TIMESTAMP_SRC                                  0x0004UL
    #define PCR0_SRC_MASK                                       0x0F00UL
    #define PCR0_SRC_SHIFT                                      8UL
    #define PCR1_SRC_MASK                                       0xF000UL
    #define PCR1_SRC_SHIFT                                      12UL

    REG16                           HW3_Cfg1;                   //0x15
    #define MASK_SCR_VID_EN                                     0x0001UL
    #define MASK_SCR_VID_3D_EN                                  0x0002UL
    #define MASK_SCR_AUD_EN                                     0x0004UL
    #define MASK_SCR_AUD_B_EN                                   0x0008UL
    #define MASK_SCR_AUD_C_EN                                   0x0010UL
    #define MASK_SCR_AUD_D_EN                                   0x0020UL
    #define MASK_SCR_PVR1_EN                                    0x0040UL
    #define MASK_SCR_PVR2_EN                                    0x0080UL
    #define RST_CC_MODE                                         0x0100UL
    #define DIS_CNTR_INC_BY_PL                                  0x0200UL
    #define BYPASS_TIMESTAMP_SEL0                               0x0400UL
    #define BYPASS_TIMESTAMP_SEL1                               0x0800UL
    #define APID_C_BYPASS                                       0x1000UL
    #define APID_D_BYPASS                                       0x2000UL
    REG32                          PauseTime[2];                // 0x16~17, 0x18~19
    REG32                          PIDFLR_PCR[2];
    #define TSP_PIDFLT_PCR_PID_MASK                             0x00001fffUL
    #define TSP_PIDFLT_PCR_EN                                   0x00008000UL
    #define TSP_PIDFLT_PCR_SOURCE_MASK                          0x000F0000UL
    #define TSP_PIDFLT_PCR_SOURCE_SHIFT                         16UL
    REG32                          Reserve;                     // 0x1e
    REG16                          HW_Semaphore0;               // 0x20
    REG16                          HW_Semaphore1;               // 0x21
    REG16                          HW_Semaphore2;               // 0x22

    REG16                          HWeco0;                      // 0x23
    #define                        HW_ECO_RVU                   0x0001UL   //RVU, reg_start_read_bypass_en, set 1 to fix start_read hang when unexpected writes
    #define                        HW_ECO_NEW_SYNCP_IN_ECO      0x0002UL   // fixed_rm_pinpong_limation_en
    #define                        HW_ECO_SEC_DMA_BURST_NEWMODE 0x000CUL   // fixed bust length 2 /4 issue
    #define                        HW_ECO_FIQ_REVERSE_DEADLOCK  0x0010UL   // fix FIQ will be deadlock when reverse block
    #define                        HW_ECO_FIX_SEC_NULLPKT_ERR   0x0020UL   // fix section can't receive pid 1ffb pkt
    #define                        HW_ECO_INIT_TIMESTAMP        0x0400UL   // set 1 to init timestamp when filein start
    #define                        HW_ECO_FIXED_VQ_MIUREQ_FLUSH 0x0800UL

    REG16                          HWeco1;                      // 0x24
    REG16                          ModeCfg;                     // 0x25
    #define TSP_3WIRE_SERIAL_MODE_MASK                          0x001FUL           //set 1 to enable 3 wire serial in mode: Combine valid and clk.Valid always 1 and gated clk when no data in
    #define TSP_3WIRE_SERIAL_TSIF0                              0x0001UL
    #define TSP_3WIRE_SERIAL_TSIF1                              0x0002UL
    #define TSP_3WIRE_SERIAL_TSIF2                              0x0004UL
    #define TSP_3WIRE_SERIAL_TSIFFI                             0x0010UL
    #define TSP_NEW_OVERFLOW_MODE                               0x0100UL            // 1: new dma_overflow 0:old dma_overflow
    #define TSP_NON_188_CNT_MODE                                0x0200UL
    #define TSP_STREAMID_CHK_DISABLE                            0x0400UL    // 1 : for nagra dongle, sync byte = 0x00, 0x80 or 0x81
    #define TSP_FILTER_STREAMID_0_TO_1F                         0x0800UL

    REG16                          NAGRA_DONGLE_SYNCBYTE;       // 0x26    
    #define SYNC_BYTE0_MASK                                     0x00FFUL
    #define SYNC_BYTE0_SHFT                                     0UL
    #define SYNC_BYTE1_MASK                                     0xFF00UL
    #define SYNC_BYTE1_SHFT                                     8UL

    REG16                          dummy;                       // 0x27

    REG16                          SyncByte_tsif0[4];           // 0x28~2b
    #define TSP_SYNC_BYTE0_MAASK0                               0x00FFUL
    #define TSP_SYNC_BYTE0_MAASK1                               0xFF00UL
    REG16                          SourceId_tsif0[2];           // 0x2c~2d
    #define TSP_SRCID_MASK0                                     0x000FUL
    #define TSP_SRCID_MASK1                                     0x00F0UL
    #define TSP_SRCID_MASK2                                     0x0F00UL
    #define TSP_SRCID_MASK3                                     0xF000UL
    REG16                          SyncByte_file[4];            // 0x2e~31
    REG16                          SourceId_file[2];            // 0x32~33
    REG16                          SyncByte_tsif1[4];           // 0x34~37
    REG16                          SourceId_tsif1[2];           // 0x38~39
    REG16                          SyncByte_tsif2[4];           // 0x3a~3d
    REG16                          SourceId_tsif2[2];           // 0x3e~3f
} REG_Ctrl3;

// TSP part 4
typedef struct _REG_Ctrl4
{
    REG16                               Overflow0;                          // 0xbf803900   0x00
    #define PID_HIT_0_EVER_OVERFLOW                 0x0001UL
    #define PID_HIT_1_EVER_OVERFLOW                 0x0002UL
    #define PID_HIT_2_EVER_OVERFLOW                 0x0004UL
    #define PID_HIT_FILE_EVER_OVERFLOW              0x0008UL
    #define PID_HIT_CB_EVER_OVERFLOW                0x0010UL
    #define AFIFO_EVER_OVERFLOW                     0x0020UL
    #define AFIFOB_EVER_OVERFLOW                    0x0040UL
    #define VFIFO_EVER_OVERFLOW                     0x0080UL
    #define V3DFIFO_EVER_OVERFLOW                   0x0100UL
    #define PVR_1_EVER_OVERFLOW                     0x0200UL
    #define PVR_2_EVER_OVERFLOW                     0x0400UL
    #define VQ_TX0_EVER_OVERFLOW                    0x1000UL
    #define VQ_TX1_EVER_OVERFLOW                    0x2000UL
    #define VQ_TX2_EVER_OVERFLOW                    0x4000UL
    #define VQ_TX3_EVER_OVERFLOW                    0x8000UL

    REG16                               Overflow1;                          // 0xbf803904   0x01
    #define AFIFOD_EVER_OVERFLOW                    0x0010UL
    #define AFIFOC_EVER_OVERFLOW                    0x0008UL
    #define SEC_DMAW_OVERFLOW                       0x0004UL
    #define SEC_SINGLE_EVER_OVERFLOW                0x0002UL
    #define SEC_PINGPONG_EVER_OVERFLOW              0x0001UL

    REG16                               FifoStatus;                         // 0xbf803908   0x02
    #define AFIFO_STATUS_MASK                       0x000FUL
    #define AFIFO_STATUS_SHFT                       0UL
    #define AFIFOC_STATUS_MASK                      0x000FUL
    #define AFIFOC_STATUS_SHFT                      0UL
    #define AFIFOB_STATUS_MASK                      0x00F0UL
    #define AFIFOB_STATUS_SHFT                      4UL
    #define AFIFOD_STATUS_MASK                      0x00F0UL
    #define AFIFOD_STATUS_SHFT                      4UL
    #define VFIFO_STATUS_MASK                       0x0F00UL
    #define VFIFO_STATUS_SHFT                       8UL
    #define V3DFIFO_STATUS_MASK                     0xF000UL
    #define V3DFIFO_STATUS_SHFT                     12UL

    REG16                               PvrFifoStatus;                       // 0xbf80390C   0x03
    #define PVR_1_STATUS_MASK                       0x000FUL
    #define PVR_1_STATUS_SHFT                       0UL

    REG16                               VQTxFifoStatus;                      // 0xbf803910   0x04
    #define VQ_TX0_STATUS_MASK                      0x000FUL
    #define VQ_TX0_STATUS_SHFT                      0UL
    #define VQ_TX1_STATUS_MASK                      0x0F00UL
    #define VQ_TX1_STATUS_SHFT                      8UL

    REG16                               PktCnt_video;                        // 0xbf803914  0x05
    REG16                               PktCnt_v3d;                          // 0xbf803918  0x06
    REG16                               PktCnt_aud;                          // 0xbf80391C  0x07
    REG16                               PktCnt_audB;                         // 0xbf803920  0x08
    REG16                               PktCnt_audC;                         // 0xbf803924  0x09
    REG16                               PktCnt_audD;                         // 0xbf803928  0x0a

    REG32                               _bf803924[1];                        // 0xbf80392C~0xbf803930  0x0b~0x0c

    REG16                               LockedPktCnt;                        // 0x0d
    REG16                               AVPktCnt;                            // 0x0e

    REG16                               PktErrStatus;                        // 0xbf80392C   0x0x0f
    REG16                               PidMatched0;                         // 0xbf803930   0x10
    REG16                               PidMatched1;                         // 0xbf803934   0x11
    REG16                               PidMatched2;                         // 0xbf803938   0x12
    REG16                               PidMatched3;                         // 0xbf80393C   0x13
    REG16                               dummy[2];                            // 0x14~0x15
    REG16                               Sram2p_collision;                    // 0x16
    #define SRAM_COLLISION_BY_SW        0x1000UL
    #define SRAM_COLLISION_BY_HW        0x2000UL
    #define SECFLT_SRAM1_EVER_COLLISION 0x4000UL
    #define SECFLT_SRAM0_EVER_COLLISION 0x8000UL
    REG16                               AVPktCnt1;                          //for vid_3d/audb                0x17
    REG16                               ErrPktCnt;                          //use reg_err_pkt_src_sel      0x18
    REG16                               AVPktCnt2;                          //for audc/audd                  0x19

    REG16                               EverUnlockStatus;                   // 0x1a
    #define EVER_UNLOCK_TS0             0x0001UL      // set 1 mean there are unlock pkts
    #define EVER_UNLOCK_TS1             0x0002UL
    #define EVER_UNLOCK_TS2             0x0004UL
    #define EVER_UNLOCK_TS_FI           0x0008UL

    REG16                               Overflow2;                          // 0xbf803904   0x1b
    #define PC_EVER_OVERFLOW_0          0x0001UL
    #define PC_EVER_OVERFLOW_FILE       0x0002UL
    #define PC_EVER_OVERFLOW_1          0x0004UL
    #define PC_EVER_OVERFLOW_2          0x0008UL

    REG16                               dummy1[0x70-0x1c];                  //0x1C~0x6f
    REG16                               ErrPktSrcSel;                       //select source of ErrPktCnt  0x70
    #define ERR_PKT_SRC_TS0             0x0001UL
    #define ERR_PKT_SRC_FILE            0x0002UL
    #define ERR_PKT_SRC_TS1             0x0003UL
    #define ERR_PKT_SRC_TS2             0x0004UL
    #define ERR_PKT_SRC_MMFI0           0x0005UL
    #define ERR_PKT_SRC_MMFI1           0x0006UL

    REG16                               ErrPktCntLoad;                      // 0x71
    #define ERR_PKT_CNT_0_LOAD          0x0001UL
    #define ERR_PKT_CNT_FILE_LOAD       0x0002UL
    #define ERR_PKT_CNT_1_LOAD          0x0004UL
    #define ERR_PKT_CNT_2_LOAD          0x0008UL
    #define ERR_PKT_CNT_MMFI0_LOAD      0x0010UL
    #define ERR_PKT_CNT_MMFI1_LOAD      0x0020UL

    REG16                               ErrPktCntClr;                       // 0x72
    #define ERR_PKT_CNT_0_CLR           0x0001UL
    #define ERR_PKT_CNT_FILE_CLR        0x0002UL
    #define ERR_PKT_CNT_1_CLR           0x0004UL
    #define ERR_PKT_CNT_2_CLR           0x0008UL
    #define ERR_PKT_CNT_MMFI0_CLR       0x0010UL
    #define ERR_PKT_CNT_MMFI1_CLR       0x0020UL

    REG16                               dummy2[0x78-0x73];                  // 0x73~0x78

    REG16                               PktCntSrc2;                         // 0x78
    #define AUDC_SRC_MASK               0x0007UL
    #define AUDC_SRC_SHIFT              0UL
    #define AUDD_SRC_MASK               0x0038UL
    #define AUDD_SRC_SHIFT              3UL

    REG16                               dummy3;                             // 0x79
    REG16                               PktCntLoad;                         // 0x7a
    #define LOCK_PKT_CNT_0_LOAD         0x0001UL
    #define LOCK_PKT_CNT_1_LOAD         0x0002UL
    #define LOCK_PKT_CNT_2_LOAD         0x0004UL
    #define LOCK_PKT_CNT_FI_LOAD        0x0010UL
    #define V_PKT_CNT_LOAD              0x0100UL
    #define V3D_PKT_CNT_LOAD            0x0200UL
    #define AUD_PKT_CNT_LOAD            0x0400UL
    #define AUDB_PKT_CNT_LOAD           0x0800UL
    #define AUDC_PKT_CNT_LOAD           0x1000UL
    #define AUDD_PKT_CNT_LOAD           0x2000UL

    REG16                               PktCntLoad1;                        // 0x7b
    #define V_DROP_PKT_CNT_LOAD          0x0001UL
    #define V3D_DROP_PKT_CNT_LOAD        0x0002UL
    #define AUD_DROP_PKT_CNT_LOAD        0x0004UL
    #define AUDB_DROP_PKT_CNT_LOAD       0x0008UL
    #define AUDC_DROP_PKT_CNT_LOAD       0x0010UL
    #define AUDD_DROP_PKT_CNT_LOAD       0x0020UL
    #define V_DIS_CNTR_PKT_CNT_LOAD      0x0100UL
    #define V3D_DIS_CNTR_PKT_CNT_LOAD    0x0200UL
    #define AUD_DIS_CNTR_PKT_CNT_LOAD    0x0400UL
    #define AUDB_DIS_CNTR_PKT_CNT_LOAD   0x0800UL
    #define AUDC_DIS_CNTR_PKT_CNT_LOAD   0x1000UL
    #define AUDD_DIS_CNTR_PKT_CNT_LOAD   0x2000UL

    REG16                               PktCntClr;                          // 0x7c
    #define LOCK_PKT_CNT_0_CLR           0x0001UL
    #define LOCK_PKT_CNT_1_CLR           0x0002UL
    #define LOCK_PKT_CNT_2_CLR           0x0004UL
    #define LOCK_PKT_CNT_FI_CLR          0x0010UL
    #define V_PKT_CNT_CLR                0x0100UL
    #define V3D_PKT_CNT_CLR              0x0200UL
    #define AUD_PKT_CNT_CLR              0x0400UL
    #define AUDB_PKT_CNT_CLR             0x0800UL
    #define AUDC_PKT_CNT_CLR             0x1000UL
    #define AUDD_PKT_CNT_CLR             0x2000UL

    REG16                               PktCntClr1;                         // 0x7d
    #define V_DROP_PKT_CNT_CLR           0x0001UL
    #define V3D_DROP_PKT_CNT_CLR         0x0002UL
    #define AUD_DROP_PKT_CNT_CLR         0x0004UL
    #define AUDB_DROP_PKT_CNT_CLR        0x0008UL
    #define AUDC_DROP_PKT_CNT_CLR        0x0010UL
    #define AUDD_DROP_PKT_CNT_CLR        0x0020UL
    #define V_DIS_CNTR_PKT_CNT_CLR       0x0100UL
    #define V3D_DIS_CNTR_PKT_CNT_CLR     0x0200UL
    #define AUD_DIS_CNTR_PKT_CNT_CLR     0x0400UL
    #define AUDB_DIS_CNTR_PKT_CNT_CLR    0x0800UL
    #define AUDC_DIS_CNTR_PKT_CNT_CLR    0x1000UL
    #define AUDD_DIS_CNTR_PKT_CNT_CLR    0x2000UL

    REG16                               PktCntSrc;                          // 0x7e
    #define VID_SRC_MASK                0x0007UL
    #define VID_SRC_SHIFT               0UL
    #define V3D_SRC_MASK                0x0031UL
    #define V3D_SRC_SHIFT               3UL
    #define AUD_SRC_MASK                0x01C0UL
    #define AUD_SRC_SHIFT               6UL
    #define AUDB_SRC_MASK               0x0E00UL
    #define AUDB_SRC_SHIFT              9UL

    REG16                               DebugSrcSel;                        // 0x7f
    #define SRC_SEL_MASK                0x0001UL
    #define DROP_PKT_MODE_MASK          0x0002UL
    #define PIDFLT_SRC_SEL_MASK         0x001CUL
    #define TSIF_SRC_SEL_MASK           0x00E0UL
    #define TSIF_SRC_SEL_SHIFT          5UL
        #define TSIF_SRC_SEL_TSIF0      0x000UL
        #define TSIF_SRC_SEL_TSIF1      0x001UL
        #define TSIF_SRC_SEL_TSIF2      0x002UL
        #define TSIF_SRC_SEL_TSIF_FI    0x004UL
    #define AV_PKT_SRC_SEL              0x0100UL
    #define AV_PKT_SRC_SEL_MASK         0x0100UL
    #define AV_PKT_SRC_SEL_SHIFT        8UL
        #define AV_PKT_SRC_VID          0x0
        #define AV_PKT_SRC_AUD          0x1
        #define AV_PKT_SRC_V3D          0x0
        #define AV_PKT_SRC_AUDB         0x1
        #define AV_PKT_SRC_AUDC         0x0
        #define AV_PKT_SRC_AUDD         0x1
    #define CLR_SRC_MASK                0x0E00UL
    #define CLR_SRC_SHIFT               9UL
    #define CLR_SRC_TSIF0               0x0200UL
        #define CLR_SRC_TSIFFI          0x0400UL
        #define CLR_SRC_TSIF1           0x0600UL
        #define CLR_SRC_TSIF2           0x0800UL
        #define CLR_SRC_MMFI0           0x0C00UL
        #define CLR_SRC_MMFI1           0x0E00UL

}REG_Ctrl4;

// TSP part 4
typedef struct _REG_Ctrl5
{
    REG16                               ATS_Adj_Period;                             // 0x00
    #define TSP_ATS_ADJ_PERIOD_MASK                     0x000FUL

    REG16                               AtsCfg;                                     // 0x01
    #define TSP_ATS_MODE_FI_ENABLE                      0x0001UL
    #define TSP_ATS_OFFSET_FI_ENABLE                    0x0002UL
    #define TSP_ATS_OFFSET_FI_SHIFT                     8UL
    #define TSP_ATS_OFFSET_FI_MASK                      0x0F00UL
    #define TSP_ATS_OFFSET_FI_POSITIVE                  0x0000UL
    #define TSP_ATS_OFFSET_FI_NEGATIVE                  0x1000UL

    REG16                               Ts_If_Fi_Cfg;                               // 0x02
    #define TSP_FIIF_EN                                 0x0001UL
    #define TSP_FIIF_DATA_SWAP                          0x0002UL
    #define TSP_FIIF_P_SEL                              0x0004UL
    #define TSP_FIIF_EXT_SYNC_SEL                       0x0008UL
    #define TSP_FIIF_MUX_MASK                           0x0010UL
        #define TSP_FIIF_MUX_FILE_PATH                  0x0000UL
        #define TSP_FIIF_MUX_LIVE_PATH                  0x0010UL
    #define TSP_PKT_CHK_SIZE_FI_MASK                    0xFF00UL
    #define TSP_PKT_CHK_SIZE_FI_SHIFT                   8UL

    REG16                               MatchPidSel;                                //  0x03
    #define TSP_MATCH_PID_SEL_MASK                      0x000FUL
    #define TSP_MATCH_PID_SEL_SHIFT                     0UL

    REG16                               TsifCfg;                                    //  0x04
    #define TSP_TSIFCFG_TSIF0_TSOBLK_EN                 0x0100UL
    #define TSP_TSIFCFG_TSIF1_TSOBLK_EN                 0x0200UL
    #define TSP_TSIFCFG_TSIF2_TSOBLK_EN                 0x0400UL
    #define TSP_TSIFCFG_TSIFFI_TSOBLK_EN                0x0800UL
    #define TSP_TSIFCFG_WB_FSM_RESET                    0x1000UL
    #define TSP_TSIFCFG_WB_FSM_RESET_FINISH             0x2000UL

    REG16                               TraceMarkCfg;                               //  0x05
    #define TSP_TRACE_MARK_VID_EN                       0x0001UL
    #define TSP_TRACE_MARK_V3D_EN                       0x0002UL
    #define TSP_TRACE_MARK_AUD_EN                       0x0004UL
    #define TSP_TRACE_MARK_AUDB_EN                      0x0008UL
    #define TSP_TRACE_MARK_AUDC_EN                      0x0010UL
    #define TSP_TRACE_MARK_AUDD_EN                      0x0020UL

    REG16                               HwCfg0;                                     //  0x06
    #define TSP_FIX_192_TIMER_0_EN                      0x0001UL
    #define TSP_VQ_CLR                                  0x0002UL
    #define TSP_FILTER_NULL_PKT0                        0x0004UL
    #define TSP_FILTER_NULL_PKT1                        0x0008UL
    #define TSP_FILTER_NULL_PKT2                        0x0010UL
    #define TSP_FILTER_NULL_PKT_FILE                    0x0020UL
    #define TSP_FLUSH_PVR1_DATA                         0x0100UL
    #define TSP_FLUSH_PVR2_DATA                         0x0200UL

    REG16                               InitTimestamp;                              //  0x07
    #define TSP_INIT_TIMESTAMP_FILEIN                   0x0001UL
    #define TSP_INIT_TIMESTAMP_MMFI0                    0x0002UL
    #define TSP_INIT_TIMESTAMP_MMFI1                    0x0004UL
    #define TSP_MATCH_CNT_FILEIN                        0x0008UL    // set 1 to enable match cnt function for filein
    #define TSP_MATCH_CNT_THRESHOLD_MASK                0x00F0UL    // file in will lost lock when match cnt <= threshold
    #define TSP_MATCH_CNT_THRESHOLD_SHFT                4UL
    #define TSP_INIT_TRUST_SYNC_CNT_MASK                0xFF00UL    // reg_init_trust_sync_cnt_value for Filein
    #define TSP_INIT_TRUST_SYNC_CNT_SHFT                8UL         // 188 : normal 192 mode (+4 is sync byte)
                                                                    // 189 : nagra dongle 192 mode (+3 is sync byte)

    REG16                               MiuSelCtrl0;                                //  0x08
    #define TSP_MIU_SEL_FILEIN_MASK                     0x0003UL
    #define TSP_MIU_SEL_FILEIN_SHIFT                    0UL
    #define TSP_MIU_SEL_SECTION_MASK                    0x000CUL
    #define TSP_MIU_SEL_SECTION_SHIFT                   2UL
    #define TSP_MIU_SEL_MMFI0_MASK                      0x0030UL
    #define TSP_MIU_SEL_MMFI0_SHIFT                     4UL
    #define TSP_MIU_SEL_MMFI1_MASK                      0x00C0UL
    #define TSP_MIU_SEL_MMFI1_SHIFT                     6UL
    #define TSP_MIU_SEL_VQ_RW_MASK                      0x0300UL
    #define TSP_MIU_SEL_VQ_RW_SHIFT                     8UL
    #define TSP_MIU_SEL_OR_RW_MASK                      0x0C00UL
    #define TSP_MIU_SEL_OR_RW_SHIFT                     10UL
    #define TSP_MIU_SEL_PVRCB_RW_MASK                   0x3000UL
    #define TSP_MIU_SEL_PVRCB_RW_SHIFT                  12UL

    REG16                               MiuSelCtrl1;                                //  0x09
    #define TSP_MIU_SEL_PVR1_MASK                       0x0003UL
    #define TSP_MIU_SEL_PVR1_SHIFT                      0UL
    #define TSP_MIU_SEL_PVR2_MASK                       0x000CUL
    #define TSP_MIU_SEL_PVR2_SHIFT                      2UL
    #define TSP_MIU_SEL_FIQ0_RW_MASK                    0x0300UL
    #define TSP_MIU_SEL_FIQ0_RW_SHIFT                   8UL
    #define TSP_MIU_SEL_FIQ1_RW_MASK                    0x0C00UL
    #define TSP_MIU_SEL_FIQ1_RW_SHIFT                   10UL

    REG16                               MiuRrPri;                                   //  0x0A
    #define TSP_MIU_RR_PRI_ABT0                         0x0001UL
    #define TSP_MIU_RR_PRI_ABT1                         0x0002UL
    #define TSP_MIU_RR_PRI_ABT2                         0x0004UL
    #define TSP_MIU_RR_PRI_ABT3                         0x0008UL
    #define TSP_MIU_RR_PRI_ABT4                         0x0010UL

    REG16                               FIQ_MUX_CFG;                                // 0xB
    #define FIQ_MUX_CFG_MASK                            0x0007UL
    #define FIQ_MUX_CFG_SHFT                            0UL
        #define FIQ_MUX_CFG_TS0                         0x0000UL
        #define FIQ_MUX_CFG_FILE                        0x0001UL
        #define FIQ_MUX_CFG_TS1                         0x0002UL
        #define FIQ_MUX_CFG_TS2                         0x0003UL

    REG16                               HWeco2;                                     // 0xC
    #define HW_ECO_TIMESTAMP_RING_BACK                  0x0001UL // set 1 to fix time stamp ring back from 32'hffff_ffff
    #define HW_ECO_LPCR_RING_BACK                       0x0002UL // set 1 to fix lpcr ring back from 32'hffff_ffff
    #define NMATCH_DISABLE                              0x0008UL // set 1 to disable secflt_not_match
    #define SCRAMB_BIT_AFTER_CA                         0x0010UL // set 1 to see match pid scramble status after ca on TSP1,0c~0d
    #define HW_ECO_TS_SYNC_OUT_DELAY                    0x0020UL // set 1 to fix MxL FIQ no timestamp issue
    #define HW_ECO_TS_SYNC_OUT_REVERSE_BLK              0x0040UL // set 1 to fix MxL FIQ no timestamp issue & reverse block
    #define HW_ECO_FIQ_INPUT                            0x0080UL // set 1 to fix MxL FIQ sync early issue
    #define SECFLT_CTRL_DMA_DISABLE                     0x0100UL // set 1 to disable sec_dma update section sram table
    #define PKT_CONVERTER_FIRST_SYNC_VLD_MASK           0x0200UL // set 1 to enable first sync valid mask for pkt_converter

    REG16                               dummy0[0x10-0xD];                           // 0xD~0xF

    REG16                               TS_MUX_CFG0;                                // 0x10
    #define TS_MUX_CFG_TS0_MUX_MASK                     0x000FUL
    #define TS_MUX_CFG_TS0_MUX_SHIFT                    0UL
    #define TS_MUX_CFG_TS1_MUX_MASK                     0x00F0UL
    #define TS_MUX_CFG_TS1_MUX_SHIFT                    4UL
    #define TS_MUX_CFG_TS2_MUX_MASK                     0x0F00UL
    #define TS_MUX_CFG_TS2_MUX_SHIFT                    8UL
    #define TS_MUX_CFG_TSFI_MUX_MASK                    0xF000UL
    #define TS_MUX_CFG_TSFI_MUX_SHIFT                   12UL
        #define TS_MUX_CFG_TS_MUX_TS0                   0x0000UL
        #define TS_MUX_CFG_TS_MUX_TS1                   0x0001UL
        #define TS_MUX_CFG_TS_MUX_TS2                   0x0002UL
        #define TS_MUX_CFG_TS_MUX_TSO                   0x0006UL
        #define TS_MUX_CFG_TS_MUX_DMD                   0x0007UL
    REG16                               TS_MUX_CFG1;                                // 0x11

    REG16                               TS_MUX_CFG_S2P;                             // 0x12
    #define TS_MUX_CFG_S2P0_MUX_MASK                    0x000FUL
    #define TS_MUX_CFG_S2P0_MUX_SHIFT                   0
        #define TS_MUX_CFG_S2P_MUX_TS0                  0x0000UL
        #define TS_MUX_CFG_S2P_MUX_TS1                  0x0001UL
        #define TS_MUX_CFG_S2P_MUX_TS2                  0x0002UL

    REG16                               TS_MUX_CFG0_TSOIN;                          // 0x13
    #define TS_MUX_CFG_TSOIN0_MUX_MASK                  0x000FUL
    #define TS_MUX_CFG_TSOIN0_MUX_SHIFT                 0UL
    #define TS_MUX_CFG_TSOIN1_MUX_MASK                  0x00F0UL
    #define TS_MUX_CFG_TSOIN1_MUX_SHIFT                 4UL
    #define TS_MUX_CFG_TSOIN2_MUX_MASK                  0x0F00UL
    #define TS_MUX_CFG_TSOIN2_MUX_SHIFT                 8UL
        #define TS_MUX_CFG_TSO_MUX_TS0                  0x0000UL
        #define TS_MUX_CFG_TSO_MUX_TS1                  0x0001UL
        #define TS_MUX_CFG_TSO_MUX_TS2                  0x0002UL
        #define TS_MUX_CFG_TSO_MUX_DMD                  0x0007UL

    REG16                               TSP5_Reserve_14;                            // 0x14

    REG16                               TS_MUX_CFG_TSOOUT;                          // 0x15
    #define TS_MUX_CFG_TSOOUT_MASK                      0x000FUL
        #define TS_MUX_CFG_TSOOUT_FROM_TSO              0x0000UL
        #define TS_MUX_CFG_TSOOUT_FROM_S2P              0x0001UL

    REG16                               TS_MMT_MUX_CFG;                             // 0x16
    #define TS_MMT_MUX_CFG_MASK                         0x000FUL
        #define TS_MMT_MUX_CFG_TS_MUX_TS0               0x0000UL
        #define TS_MMT_MUX_CFG_TS_MUX_TS1               0x0001UL
        #define TS_MMT_MUX_CFG_TS_MUX_TS2               0x0002UL
        #define TS_MMT_MUX_CFG_TS_MUX_TSO               0x0006UL
    REG16                               dummy1[0x20-0x17];                          // 0x17~0x1F

    REG32                               FileIn_Dmar_LBnd;                           // 0x20
    #define TS_FILEIN_DMAR_LBND_MASK                    0x0FFFFFFFUL

    REG32                               FileIn_Dmar_UBnd;                           // 0x22
    #define TS_FILEIN_DMAR_UBND_MASK                    0x0FFFFFFFUL

    REG32                               MMFileIn0_Dmar_LBnd;                        // 0x24
    #define TS_MMFILEIN0_DMAR_LBND_MASK                 0x0FFFFFFFUL

    REG32                               MMFileIn0_Dmar_UBnd;                        // 0x26
    #define TS_MMFILEIN0_DMAR_UBND_MASK                 0x0FFFFFFFUL

    REG32                               MMFileIn1_Dmar_LBnd;                        // 0x28
    #define TS_MMFILEIN1_DMAR_LBND_MASK                 0x0FFFFFFFUL

    REG32                               MMFileIn1_Dmar_UBnd;                        // 0x2A
    #define TS_MMFILEIN1_DMAR_UBND_MASK                 0x0FFFFFFFUL

    REG32                               Orz_Dmar_LBnd;                              // 0x2C
    #define TS_ORZ_DMAR_LBND_MASK                       0x0FFFFFFFUL

    REG32                               Orz_Dmar_UBnd;                              // 0x2E
    #define TS_ORZ_DMAR_UBND_MASK                       0x0FFFFFFFUL

    REG32                               VQTX0_Dmar_LBnd;                            // 0x30
    #define TS_VQTX0_DMAR_LBND_MASK                     0x0FFFFFFFUL

    REG32                               VQTX0_Dmar_UBnd;                            // 0x32
    #define TS_VQTX0_DMAR_UBND_MASK                     0x0FFFFFFFUL

    REG32                               VQTX1_Dmar_LBnd;                            // 0x34
    #define TS_VQTX1_DMAR_LBND_MASK                     0x0FFFFFFFUL

    REG32                               VQTX1_Dmar_UBnd;                            // 0x36
    #define TS_VQTX1_DMAR_UBND_MASK                     0x0FFFFFFFUL

    REG32                               VQTX2_Dmar_LBnd;                            // 0x38
    #define TS_VQTX2_DMAR_LBND_MASK                     0x0FFFFFFFUL

    REG16                               dummy_3A_3F[6];                             // 0x3A~0x3F

    REG32                               VQTX2_Dmar_UBnd;                            // 0x40
    #define TS_VQTX2_DMAR_UBND_MASK                     0x0FFFFFFFUL

    REG32                               VQTX3_Dmar_LBnd;                            // 0x42
    #define TS_VQTX3_DMAR_LBND_MASK                     0x0FFFFFFFUL

    REG32                               VQTX3_Dmar_UBnd;                            // 0x44
    #define TS_VQTX3_DMAR_UBND_MASK                     0x0FFFFFFFUL

    REG32                               VQRX_Dmar_LBnd;                             // 0x46
    #define TS_VQRX_DMAR_LBND_MASK                      0x0FFFFFFFUL

    REG32                               VQRX_Dmar_UBnd;                             // 0x48
    #define TS_VQRX_DMAR_UBND_MASK                      0x0FFFFFFFUL

    REG32                               Fiq0_Dmar_LBnd;                             // 0x4A
    #define TS_Fiq0_DMAR_LBND_MASK                      0x0FFFFFFFUL

    REG32                               Fiq0_Dmar_UBnd;                             // 0x4C
    #define TS_Fiq0_DMAR_UBND_MASK                      0x0FFFFFFFUL

    REG32                               Fiq1_Dmar_LBnd;                             // 0x4E
    #define TS_Fiq1_DMAR_LBND_MASK                      0x0FFFFFFFUL

    REG32                               Fiq1_Dmar_UBnd;                             // 0x50
    #define TS_Fiq1_DMAR_UBND_MASK                      0x0FFFFFFFUL

    REG16                               dummy2[0x5D-0x52];                          // 0x52~0x5C

    REG16                               SyncByte_Privilege_FILE;                    // 0x5D
    #define FILE_PRIVILEGE_SYNC_BYTE                    0x00FFUL
    #define FILE_PRIVILEGE_SYNC_BYTE_REPLACE            0xFF00UL
    REG16                               SyncByte_Privilege_PVR;                     // 0x5E
    #define PVR1_PRIVILEGE_SYNC_BYTE                    0x00FFUL
    #define PVR2_PRIVILEGE_SYNC_BYTE                    0xFF00UL

    REG16                               CPU_Secure_Cfg;                             // 0x5F
    #define FW_LOAD_ONCE_BY_TEE                         0x0001UL
    #define FW_SECURE_STATUS                            0x0002UL
    #define FW_FIX_DMX_RST_ONEWAY                       0x0004UL
    #define LEAF_DROP_ENABLE                            0x0008UL

    REG16                               Dma_Ns_Cfg;                                 // 0x60
    #define TS_DMA_NS_CTRL_FILEIN                       0x0001UL
    #define TS_DMA_NS_CTRL_MMFI0                        0x0002UL
    #define TS_DMA_NS_CTRL_MMFI1                        0x0004UL
    #define TS_DMA_NS_CTRL_PVR1                         0x0008UL
    #define TS_DMA_NS_CTRL_PVR2                         0x0010UL
    #define TS_DMA_NS_CTRL_VQ                           0x0020UL
    #define TS_DMA_NS_CTRL_ORZ                          0x0040UL
    #define TS_DMA_NS_CTRL_SEC                          0x0080UL
    #define TS_DMA_NS_CTRL_FIQ0                         0x0100UL
    #define TS_DMA_NS_CTRL_FIQ1                         0x0200UL

    REG16                               Dma_Be_Cfg;                                 // 0x61
    #define TS_DMA_BE_CTRL_FILEIN                       0x0001UL
    #define TS_DMA_BE_CTRL_MMFI0                        0x0002UL
    #define TS_DMA_BE_CTRL_MMFI1                        0x0004UL
    #define TS_DMA_BE_CTRL_PVR1                         0x0008UL
    #define TS_DMA_BE_CTRL_PVR2                         0x0010UL
    #define TS_DMA_BE_CTRL_VQ                           0x0020UL
    #define TS_DMA_BE_CTRL_ORZ                          0x0040UL
    #define TS_DMA_BE_CTRL_SEC                          0x0080UL
    #define TS_DMA_BE_CTRL_FIQ0                         0x0100UL
    #define TS_DMA_BE_CTRL_FIQ1                         0x0200UL

    REG16                               MIU_NsUseTee_Cfg;                           // 0x62
    #define TS_MIU_NS_USE_TEE_WP_RP_FILEIN              0x0001UL
    #define TS_MIU_NS_USE_TEE_WP_RP_MMFI0               0x0002UL
    #define TS_MIU_NS_USE_TEE_WP_RP_MMFI1               0x0004UL

    REG32                               INIT_TIMESTAMP_FILE;                        // 0x63
    REG32                               INIT_TIMESTAMP_MMFI0;                        // 0x65
    REG32                               INIT_TIMESTAMP_MMFI1;                       // 0x67
    REG16                               Reserve[0x71-0x69];                         // 0x69-0x70
    REG32                               PVR1_Int_Cfg;                               // 0x71
    REG32                               PVR2_Int_Cfg;                               // 0x73
    REG16                               PVR_MetSize;                                // 0x75

    REG32                               DMAW_LBND5;                                 // 0x76
    REG32                               DMAW_UBND5;                                 // 0x78
    REG32                               DMAW_LBND6;                                 // 0x7A
    REG32                               DMAW_UBND6;                                 // 0x7C

    REG16                               VQ_Idle_Cnt;                                // 0x7E
    REG16                               Miu_MCM_Cfg;                                // 0x7F
    #define MCM_TURN_OFF_ALL                            0x0001UL
    #define TS_OR_WRITE_FIX                             0x8000UL
}REG_Ctrl5;

typedef struct _REG_Ctrl6
{
    REG32                               PCR64_3_L;                                   // 0x00
    REG32                               PCR64_3_H;                                   // 0x02
    REG32                               PCR64_4_L;                                   // 0x04
    REG32                               PCR64_4_H;                                   // 0x06
}REG_Ctrl6;

// TSP: ts sample part
typedef struct _REG_TS_Sample
{
    REG16                               TS0_Clk_Sample;                             // 0x00
    #define TS0_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TS0_PHASE_ADJUST_EN                         0x0020UL
    #define TS0_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL

    REG16                               TS1_Clk_Sample;                             // 0x01
    #define TS1_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TS1_PHASE_ADJUST_EN                         0x0020UL
    #define TS1_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL

    REG16                               TS2_Clk_Sample;                             // 0x02
    #define TS2_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TS2_PHASE_ADJUST_EN                         0x0020UL
    #define TS2_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL

    REG16                               TS3_Clk_Sample;                             // 0x03
    #define TS3_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TS3_PHASE_ADJUST_EN                         0x0020UL
    #define TS3_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL

    REG16                               TS4_Clk_Sample;                             // 0x04
    #define TS4_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TS4_PHASE_ADJUST_EN                         0x0020UL
    #define TS4_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL

    REG16                               TS5_Clk_Sample;                             // 0x05
    #define TS5_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TS5_PHASE_ADJUST_EN                         0x0020UL
    #define TS5_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL

    REG16                               TsSample_Reserved0[0x10-0x6];               // 0x06 - 0x0F

    REG16                               TSO_Clk_Sample;                             // 0x10
    #define TSO_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define TSO_PHASE_ADJUST_EN                         0x0020UL
    #define TSO_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL
    #define TSO_CLK_INVERT                              0x0080UL

    REG16                               TsSample_Reserved1[0x20-0x11];              // 0x11 - 0x1F

    REG16                               TS_Out_Clk_Sample;                          // 0x20 (for old path: TSIF2 out)
    #define TS_OUT_PHASE_ADJUST_COUNT_MASK              0x001FUL
    #define TS_OUT_PHASE_ADJUST_EN                      0x0020UL
    #define TS_OUT_RESAMPLE_VOTE_ADJUST_EN              0x0040UL
    #define TS_OUT_CLK_INVERT                           0x0080UL

    REG16                               S2P_Out_Clk_Sample;                         // 0x21
    #define S2P_PHASE_ADJUST_COUNT_MASK                 0x001FUL
    #define S2P_PHASE_ADJUST_EN                         0x0020UL
    #define S2P_RESAMPLE_VOTE_ADJUST_EN                 0x0040UL
    #define S2P_CLK_INVERT                              0x0080UL

    REG16                               S2P1_Out_Clk_Sample;                        // 0x22
    #define S2P1_PHASE_ADJUST_COUNT_MASK                0x001FUL
    #define S2P1_PHASE_ADJUST_EN                        0x0020UL
    #define S2P1_RESAMPLE_VOTE_ADJUST_EN                0x0040UL
    #define S2P1_CLK_INVERT                             0x0080UL

}REG_TS_Sample;

// Firmware status
#define TSP_FW_STATE_MASK           0xFFFF0000UL
#define TSP_FW_STATE_LOAD           0x00010000UL
#define TSP_FW_STATE_ENG_OVRUN      0x00020000UL
#define TSP_FW_STATE_ENG1_OVRUN     0x00040000UL                          //[reserved]
#define TSP_FW_STATE_IC_ENABLE      0x01000000UL
#define TSP_FW_STATE_DC_ENABLE      0x02000000UL
#define TSP_FW_STATE_IS_ENABLE      0x04000000UL
#define TSP_FW_STATE_DS_ENABLE      0x08000000UL


// TSP AEON specific IP address
#define OPENRISC_IP_1_ADDR 0x00200000UL
#define OPENRISC_IP_1_SIZE 0x00020000UL
#define OPENRISC_IP_2_ADDR 0x90000000UL
#define OPENRISC_IP_2_SIZE 0x00010000UL
#define OPENRISC_IP_3_ADDR 0x40080000UL
#define OPENRISC_IP_3_SIZE 0x00020000UL
#define OPENRISC_QMEM_ADDR 0x00000000UL
#define OPENRISC_QMEM_SIZE 0x00003000UL
#endif // _TSP_REG_H_

