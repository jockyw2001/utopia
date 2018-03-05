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
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
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

//[CMODEL][FWTSP]
// When enable, interrupt will not lost, CModel will block next packet
// and FwTSP will block until interrupt status is clear by MIPS.
// (For firmware and cmodel only)
#define TSP_DBG_SAFE_MODE_ENABLE    0

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define TSP_PIDFLT_NUM                  32UL
#define TSP_PIDFLT1_NUM                 16UL
#define TSP_PVR_IF_NUM                  1UL
#define TSP_MMFI_AUDIO_FILTER_NUM       2UL
#define TSP_MMFI_V3D_FILTER_NUM         1UL
#define TSP_IF_NUM                      2UL
#define TSP_DEMOD_NUM                   1UL
#define TSP_VFIFO_NUM                   2UL
#define TSP_AFIFO_NUM                   1UL
#define TSP_TS_PAD_NUM                  2UL
#define TSP_VQ_NUM                      2UL
#define TSP_CA_FLT_NUM                  16UL
#define TSP_CA_KEY_NUM                  8UL
#define TSP_VQ_PITCH                    192UL
/***************************************************/

#define TSP_ENGINE_NUM                      (1UL)
#define TSP_PIDFLT_NUM_ALL                  (TSP_PIDFLT_NUM+ TSP_PIDFLT1_NUM)
#define TSP_SECFLT_NUM                      (TSP_PIDFLT_NUM)
#define TSP_SECBUF_NUM                      (TSP_SECFLT_NUM)
#define TSP_FILTER_DEPTH                    (16UL)

#define TSP_SECFLT_NUM_All                  (TSP_SECFLT_NUM)

#define TSP_WP_SET_NUM                      (5UL)

#define DSCMB_FLT_START_ID                  (16UL)
#define DSCMB_FLT_END_ID                    (31UL)
#define DSCMB_FLT_NUM                       (16UL)

#define DSCMB_FLT_SHAREKEY_START_ID         (0UL)
#define DSCMB_FLT_SHAREKEY_END_ID           (0UL)
#define DSCMB_FLT_SHAREKEY_NUM              (0UL)

#define DSCMB_FLT_NUM_ALL                   (DSCMB_FLT_NUM+DSCMB_FLT_SHAREKEY_NUM)


//PAD MUX definition
#define TSP_MUX_TS0                         0UL
#define TSP_MUX_TS1                         1UL      
#define TSP_MUX_INDEMOD                     7UL

//Clk source definition
#define TSP_CLK_DISABLE                     0x01UL
#define TSP_CLK_INVERSE                     0x02UL
#define TSP_CLK_TS0                         0x00UL
#define TSP_CLK_TS1                         0x04UL
#define TSP_CLK_INDEMOD                     0x1CUL
#define CLKGEN0_TSP_CLK_MASK                0x1CUL

#define TSP_FW_DEVICE_ID                    0x31UL

#define STC_SYNTH_NUM                       1UL
#define STC_SYNTH_DEFAULT                   0x28000000UL

#define DRAM_SIZE                           (0x40000000UL)
#define TSP_FW_BUF_SIZE                     (0x4000UL)
#define TSP_FW_BUF_LOW_BUD                  0UL      
#define TSP_FW_BUF_UP_BUD                   DRAM_SIZE

#define TSP_VQ_BUF_LOW_BUD                  0UL
#define TSP_VQ_BUF_UP_BUD                   DRAM_SIZE

#define TSP_SEC_BUF_LOW_BUD                 0UL
#define TSP_SEC_BUF_UP_BUD                  DRAM_SIZE
#define TSP_SEC_FLT_DEPTH                   32UL 
#define TSP_FIQ_NUM                         0UL   

//QMEM Setting
#define _TSP_QMEM_I_MASK            0xffff8000UL //total: 0x4000
#define _TSP_QMEM_I_ADDR_HIT        0x00000000UL
#define _TSP_QMEM_I_ADDR_MISS       0xffffffffUL
#define _TSP_QMEM_D_MASK            0xffff8000UL
#define _TSP_QMEM_D_ADDR_HIT        0x00000000UL
#define _TSP_QMEM_D_ADDR_MISS       0xffffffffUL
#define _TSP_QMEM_SIZE              0x1000UL // 16K bytes, 32bit aligment  //0x4000

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Software
#define REG_PIDFLT_BASE         (0x00210000UL << 1UL)                   // Fit the size of REG32
#define REG_SECFLT_BASE         (0x00211000UL << 1UL)                   // Fix the size of REG32

#define REG_CTRL_BASE           (0x2A00UL)                            // 0xBF800000+(1500/2)*4
#define REG_CTRL_BASE_TS3       (0xC1400UL)                           // 0xBF800000+(60A00/2)*4
#define REG_CTRL_MMFIBASE       (0x3900UL)                            // 0xBF800000+(1C80/2)*4 (TSP2: debug table)

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

typedef REG32                       REG_PidFlt;

// PID
#define TSP_PIDFLT_PID_MASK         0x00001FFFUL
#define TSP_PIDFLT_PID_SHFT         0UL

// Section filter Id
#define TSP_PIDFLT_SECFLT_MASK      0x001F0000UL                          // [20:16] secflt id
#define TSP_PIDFLT_SECFLT_SHFT      16UL
#define TSP_PIDFLT_SECFLT_NULL      0x1FUL                                // software usage

// AF/Sec/Video/Audio/Audio-second
#define TSP_PIDFLT_OUT_MASK         0x09e02000UL
#define TSP_PIDFLT_OUT_SECFLT_AF    0x00002000UL
#define TSP_PIDFLT_OUT_NONE         0x00000000UL
#define TSP_PIDFLT_OUT_SECFLT       0x00200000UL
#define TSP_PIDFLT_OUT_VFIFO        0x00400000UL
#define TSP_PIDFLT_OUT_AFIFO        0x00800000UL
#define TSP_PIDFLT_OUT_AFIFO2       0x01000000UL
#define TSP_PIDFLT_OUT_VFIFO3D      0x08000000UL

// File/Live
#define TSP_PIDFLT_IN_MASK          0x02000000UL
#define TSP_PIDFLT_IN_LIVE          0x00000000UL
#define TSP_PIDFLT_IN_FILE          0x02000000UL

// note, this bit is only useful for PVR pure pid
// use SEC/VIDEO/AUDIO flag is identical to PVR a certain PID
#define TSP_PIDFLT_PVR_ENABLE       0x04000000UL

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
    #define TSP_SECFLT_TYPE_EMM                     0x00000006UL
    #define TSP_SECFLT_TYPE_ECM                     0x00000007UL
    #define TSP_SECFLT_TYPE_SEC_NO_PUSI             0x01000000UL
    // for TSP_SECFLT_TYPE_PCR
    #define TSP_SECFLT_PCRRST                       0x00000010UL

    // for
    // TSP_SECFLT_TYPE_SEC
    // TSP_SECFLT_TYPE_PES
    // TSP_SECFLT_TYPE_PKT
    // TSP_SECFLT_TYPE_TTX
    // TSP_SECFLT_TYPE_OAD
    #define TSP_SECFLT_MODE_MASK                    0x00000030UL          // software implementation
    #define TSP_SECFLT_MODE_SHFT                    4UL
    #define TSP_SECFLT_MODE_CONTI                   0x0UL
    #define TSP_SECFLT_MODE_ONESHOT                 0x1UL
    #define TSP_SECFLT_MODE_CRCCHK                  0x2UL
    #define TSP_SECFLT_MODE_PESSCMCHK               0x3UL                 //Only for PES type checking SCMB status

    //[NOTE] update section filter
    // It's not suggestion user update section filter control register
    // when filter is enable. There may be race condition. Be careful.
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
    REG32                           NMatch[TSP_FILTER_DEPTH/sizeof(MS_U32)];
    REG32                           _x50[12]; // (0x210080-0x210050)/4
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
                                                                        // Index(word)  CPU(byte)     MIPS(0x1500/2+index)*4
    // only 24 bits supported in PVR address. 8 bytes address
    REG32                           TsRec_Head20;                       // 0xbf802a00   0x00     //oneway/rw protect
    #define TSP_HW_PVR_BUF_HEAD20_MASK              0xFFFF0000UL
    #define TSP_HW_PVR_BUF_HEAD20_SHFT              16UL
    REG32                           TsRec_Head21_Mid20;                 // 0xbf802a08   0x02
    #define TSP_HW_PVR_BUF_HEAD21_MASK              0x0000FFFFUL
    #define TSP_HW_PVR_BUF_HEAD21_SHFT              0UL
    #define TSP_HW_PVR_BUF_MID20_MASK               0xFFFF0000UL
    #define TSP_HW_PVR_BUF_MID20_SHFT               16UL
    REG32                           TsRec_Mid21_Tail20;                 // 0xbf802a10   0x04
    #define TSP_HW_PVR_BUF_MID21_MASK               0x0000FFFFUL
    #define TSP_HW_PVR_BUF_MID21_SHFT               0UL
    #define TSP_HW_PVR_BUF_TAIL20_MASK              0xFFFF0000UL
    #define TSP_HW_PVR_BUF_TAIL20_SHFT              16UL
    REG32                           TsRec_Tail2_Pcr1;                   // 0xbf802a18   0x06
    #define TSP_HW_PVR_BUF_TAIL21_MASK              0x0000FFFFUL
    #define TSP_HW_PVR_BUF_TAIL21_SHFT              0UL
    #define TSP_PCR1_L16_MASK                       0xFFFF0000UL
    #define TSP_PCR1_L16_SHFT                       16UL
    REG32                           Pcr1;                               // 0xbf802a20   0x08
    #define TSP_PCR64_MID32_MASK                    0xFFFFFFFFUL          //PCR64 Middle 64
    #define TSP_PCR64_MID32_SHFT                    0UL
    REG32                           Pcr64_H;                            // 0xbf802a28   0x0A
    #define TSP_PCR64_H16_MASK                      0x0000FFFFUL
    #define TSP_PCR64_H16_SHFT                      0UL
    #define TSP_MOBF_FILE_KEY0_L_MASK               0x001F0000UL          //decrypt key
    #define TSP_MOBF_FILE_KEY0_L_SHIFT              16UL

    REG32                           _xbf802a30;                         //_xbf802a30    0x0C

    REG32                           DbgInfo_Ctrl;                       //_xbf802a38    0x0E
    #define TSP_DIS_LOCKED_PKT_CNT                  0x10000000UL
    #define TSP_CLR_LOCKED_PKT_CNT                  0x20000000UL
    #define TSP_CLR_AV_PKT_CNT                      0x40000000UL

    REG32                           _xbf802a40_xbf802a78[8];            // 0xbf802a40-- 0xbf802a78 (0x10 ~ 0x1E)
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
    REG32                           Hw_Config0;                         // 0xbf802ab0   0x2c : HW_Config0~3 (0x2c~0x2d)
    #define TSP_HW_CFG0_DATA_PORT_EN                0x00000001UL
    #define TSP_HW_CFG0_TSIFO_SERL                  0x00000000UL
    #define TSP_HW_CFG0_TSIF0_PARL                  0x00000002UL
    #define TSP_HW_CFG0_TSIF0_EXTSYNC               0x00000004UL
    #define TSP_HW_CFG0_TSIF0_TS_BYPASS             0x00000008UL
    #define TSP_HW_CFG0_TSIF0_VPID_BYPASS           0x00000010UL
    #define TSP_HW_CFG0_TSIF0_APID_BYPASS           0x00000020UL
    #define TSP_HW_CFG0_WB_DMA_RESET                0x00000040UL
    
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_MASK        0x0000FF00UL
    #define TSP_HW_CFG0_PACKET_BUF_SIZE_SHIFT       8UL
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_MASK    0x00FF0000UL
    #define TSP_HW_CFG0_PACKET_PIDFLT0_SIZE_SHIFT   16UL
    #define TSP_HW_CFG0_PACKET_SIZE_MASK            0xFF000000UL
    #define TSP_HW_CFG0_PACKET_SIZE_SHFT            24UL

    REG32                           TSP_DBG_PORT;                       // 0xbf802ab8   0x2e
    #define TSP_DBG_FILTER_MATCH0_MASK              0x000000FFUL
    #define TSP_DBG_FILTER_MATCH0_SHIFT             0UL
    #define TSP_DBG_FILTER_MATCH1_MASK              0x0000FF00UL
    #define TSP_DBG_FILTER_MATCH1_SHIFT             8UL
    #define TSP_DNG_DATA_MASK                       0x00FF0000UL
    #define TSP_DNG_DATA_SHIFT                      16UL
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

    REG32                           Pkt_Info2;                         // 0xbf802ad8   0x36
    #define TSP_PKT_INFO_CC_MASK                    0x0000000FUL
    #define TSP_PKT_INFO_CC_SHFT                    0UL
    #define TSP_PKT_INFO_ADPCNTL_MASK               0x00000030UL
    #define TSP_PKT_INFO_ADPCNTL_SHFT               4UL
    #define TSP_PKT_INFO_SCMB                       0x000000C0UL
    #define TSP_PKT_INFO_SCMB_SHFT                  6UL
    #define TSP_PKT_PID_0_7_CP_MASK                 0x0000FF00UL
    #define TSP_PKT_PID_0_7_CP_SHIFT                8UL

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

    REG32                           TsDma_Addr;                         // 0xbf802ae8   0x3a         //oneway/rw protect
    // only 24 bits available for filein length
    REG32                           TsDma_Size;                         // 0xbf802af0   0x3c
    REG32                           TsDma_Ctrl_CmdQ;                    // 0xbf802af8   0x3e
    // file in control
    #define TSP_TSDMA_CTRL_START                    0x00000001UL
    #define TSP_TSDMA_RDONE                         0x00000002UL
    #define TSP_TSDMA_CTRL_INIT_TRUST_MCU           0x00000004UL
    #define TSP_TSDMA_CTRL_PESMODE_MASK             0x0000001CUL
    #define TSP_TSDMA_CTRL_VPES0                    0x00000004UL
    #define TSP_TSDMA_CTRL_APES0                    0x00000008UL
    #define TSP_TSDMA_CTRL_V3DPES0                  0x00000020UL  //not used
    #define TSP_TSDMA_STAT_ABORT                    0x00000080UL
    // CmdQ
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
        #define TSP_MCU_CMD_NMATCH_FLT_MASK                     0x000000FFUL
        #define TSP_MCU_CMD_NMATCH_FLT_SHFT                     0x00000000UL
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
    #define TSP_MCU_CMD_SET_STC_OFFSET                          0x10000000UL
        #define TSP_MCU_CMD_SET_STC_OFFSET_OPTION_MASK          0x00FF0000UL
        #define TSP_MCU_CMD_SET_STC_OFFSET_OPTION_SHIFT         16UL
    // #define TSP_MSG_FW_STC_NOSYNC                   0x00000001
    // #define TSP_MSG_FW_STC1_NOSYNC                  0x00000002          //[reserved]

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
    #define TSP_HW_CFG2_TS_DATAPORT_EN1             0x04000000UL
    #define TSP_HW_CFG2_TS_FILE_IN1                 0x08000000UL
    #define TSP_HW_CFG2_TSIF1_TS_BYPASS             0x10000000UL
    #define TSP_HW_CFG2_TSIF1_VPID_BYPASS           0x20000000UL
    #define TSP_HW_CFG2_TSIF1_APID_BYPASS           0x40000000UL
    #define TSP_PIDFLT_SEL_PID0                     0x00000000UL
    #define TSP_PIDFLT_SEL_PID1                     0x80000000UL

    REG32                           Hw_Config4;                         // 0xbf802b10   0x44
    #define TSP_HW_CFG4_PVR_PIDFLT_SEC              0x00000001UL
    #define TSP_HW_CFG4_PVR_ENABLE                  0x00000002UL
    #define TSP_HW_CFG4_PVR_ENDIAN_BIG              0x00000004UL          // 1: record TS to MIU with big endian
                                                                        // 0: record TS to MIU with little endian
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
    #define TSP_HW_BD_AUDIO_EN                      0x00002000UL
    #define TSP_HW_TSP2OUTAEON_INT_EN               0x00004000UL          // Set 1 to force interrupt to outside AEON
    #define TSP_HW_HK_INT_FORCE                     0x00008000UL          // Set 1 to force interrupt to HK_MCU
    #define TSP_HW_CFG4_BYTE_ADDR_DMA               0x000E0000UL          // prevent from byte enable bug
    #define TSP_HW_CFG4_ALT_TS_SIZE                 0x00010000UL          // enable TS packets in 204 mode
    #define TSP_HW_DMA_MODE_MASK                    0x00300000UL          // Section filter DMA mode, 2'b00: Single.2'b01: Burst 2 bytes.2'b10: Burst 4 bytes.2'b11: Burst 8 bytes.
    #define TSP_HW_DMA_MODE_SHIFT                   20UL
    #define TSP_HW_CFG4_PS_VID_EN                   0x00800000UL          // program stream video enable
    #define TSP_HW_CFG4_PS_AUD_EN                   0x01000000UL          // program stream audio enable
    #define TSP_HW_CFG4_APES_ERR_RM_E               0x04000000UL          // Set 1 to enable removing APES error packet
    #define TSP_HW_CFG4_VPES_ERR_RM_EN              0x08000000UL          // Set 1 to enable removing VPES error packet
    #define TSP_HW_CFG4_SEC_ERR_RM_EN               0x10000000UL          // Set 1 to enable removing section error packet
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
    #define TSP_V3D_FIFO_OVERFLOW                   0x00200000UL
    #define TSP_VD_FIFO_OVERFLOW                    0x08000000UL
    #define TSP_AU_FIFO_OVERFLOW                    0x20000000UL

    // only 25 bits supported in PVR address. 8 bytes address
    REG32                           TsRec_Head;                         // 0xbf802b40   0x50         //oneway/rw protect
    REG32                           TsRec_Mid;                          // 0xbf802b48   0x52
    REG32                           TsRec_Tail;                         // 0xbf802b50   0x54
    REG32                           TsRec_WPtr;                         // 0xbf802b58   0x56

    REG32                           TSP_DMAWP_BND;                      // 0xbf802b60   0x58
    #define TSP_DMAWP_BND_ALI_SHIFT                 10UL
    #define TSP_DMAWP_LBND_MASK                     0x0000FFFFUL
    #define TSP_DMAWP_LBND_SHFT                     0UL
    #define TSP_DMAWP_HBND_MASK                     0x0000FFFFUL
    #define TSP_DMAWP_HBND_SHFT                     16UL

    REG32                           reg15b4;                            // 0xbf802b68   0x5a
    #define TSP_VQ_DMAW_PROTECT_EN                  0x00000001UL
    #define TSP_DMAW_PROTECT_EN                     0x00000002UL
    #define TSP_DMAW_ERRST_CLR                      0x00000004UL          // Set 1 to clear the error status of DMA write out of bound
    #define TSP_PVR_TS_HEADER                       0x00000008UL          // Set 1 to bypass TS header in PIDFLT0 record
    #define TSP_PVR_FILEIN                          0x00000010UL          // Set 1 to enable recoding through PIDFLT0
    #define TSP_REC_ALL_TS                          0x00000020UL          // Set 1 to enable recoding TS from broadcast source in PIDFLT0
    #define TSP_REC_ALL_FILE                        0x00000040UL          // Set 1 to enable recoding TS from file input source in PIDFLT0
    #define TSP_AVFIFO_RD_EN                        0x00000080UL          // 0: AFIFO and VFIFO read are connected to MVD and MAD,  1: AFIFO and VFIFO read are controlled by registers (0x15B5[2:0])
    #define TSP_AVFIFO_RD                           0x00000100UL          // If AVFIFO_RD_EN is 1, set to 1, then set to 0 would issue a read strobe to AFIFO or VFIFO
    #define TSP_AVFIFO_SEL_VIDEO                    0x00000000UL
    #define TSP_AVFIFO_SEL_AUDIO                    0x00000200UL
    #define TSP_NMATCH_DIS                          0x00000800UL          // Set 1 to disable not match compare function
    #define TSP_REC_DATA_INV_EN                     0x00001000UL          // Set 1 to enable data payload invert for PVR record
    #define TSP_PLY_FILE_INV_EN                     0x00002000UL          // Set 1 to enable data payload invert in pidflt0 file path
    #define TSP_PLY_TS_INV_EN                       0x00004000UL          // Set 1 to enable data payload invert in pidflt0 TS path
    #define TSP_BYTE_TIMER_EN                       0x00008000UL          // Set 1 to enable byte timer in ts_if0 TS path
    #define TSP_STR2MI_MIU_PINPON_EN                0x00010000UL          // Set 1 to enable MIU addresses with pinpon mode
    #define TSP_REG_REC_PID_EN                      0x00020000UL          // Set 1 to record max 24 pid and ignore a/v/sec/adp flag.
    #define TSP_TEI_SKIPE_PKT_PID0                  0x00040000UL          // Set 1 to skip error packets in pidflt0 TS path
    #define TSP_TEI_SKIPE_PKT_FILE                  0x00080000UL          // Set 1 to skip error packets in pidflt0 file path
    #define TSP_TEI_SKIPE_PKT_PID1                  0x00100000UL          // Set 1 to skip error packets in pidflt1 TS path
    #define TSP_cnt_33b_ld                          0x01000000UL          // Set 1 to load cnt_33b
    #define TSP_force_syncbyte                      0x02000000UL          // Set 1 to force sync byte (8'h47) in ts_if0 and ts_if1 path.
    #define TSP_serial_ext_sync_1t                  0x04000000UL          // Set 1 to detect serial-in sync without 8-cycle mode
    #define TSP_burst_len_MASK                      0x18000000UL          // 00,01:    burst length = 4; 10,11: burst length = 1
    #define TSP_burst_len_SHIFT                     27UL
    #define TSP_match_pid_num_ld                    0x20000000UL          // Set 1 to load match pid number
    #define TSP_match_pid_scr_ts_ld                 0x40000000UL          // Set 1 to load match pid number with scramble information from FILE PIDFLT
    #define TSP_match_pid_scr_fi_ld                 0x80000000UL          // Set 1 to load match pid number with scramble information from TS PIDFLT

    REG32                           TSP_MATCH_PID_NUM;                  // 0xbf802b70   0x5c
    REG32                           TSP_IWB_WAIT;                       // 0xbf802b78   0x5e  // Wait count settings for IWB when TSP CPU i-cache is enabled.

    REG32                           Cpu_Base;                           // 0xbf802b80   0x60         //oneway/rw protect
    #define TSP_CPU_BASE_ADDR_MASK                  0x03FFFFFFUL
    REG32                           Qmem_Ibase;                         // 0xbf802b88   0x62
    REG32                           Qmem_Imask;                         // 0xbf802b90   0x64
    REG32                           Qmem_Dbase;                         // 0xbf802b98   0x66
    REG32                           Qmem_Dmask;                         // 0xbf802ba0   0x68

    REG32                           TSP_Debug;                          // 0xbf802ba8   0x6a
    #define TSP_DEBUG_MASK          0x00FFFFFFUL

    REG32                           TsFileIn_WPtr;                      // 0xbf802bb0   0x6c, bit0~bit24
    REG32                           TsFileIn_RPtr;                      // 0xbf802bb8   0x6e
    REG32                           TsFileIn_Timer;                     // 0xbf802bc0   0x70
    REG32                           TsFileIn_Head;                      // 0xbf802bc8   0x72, bit0~bit24
    REG32                           TsFileIn_Mid;                       // 0xbf802bd0   0x74
    REG32                           TsFileIn_Tail;                      // 0xbf802bd8   0x76

    REG32                           Dnld_Ctrl;                          // 0xbf802be0   0x78, miu address
    #define TSP_DNLD_ADDR_MASK                      0x0000FFFFUL
    #define TSP_DNLD_ADDR_SHFT                      0UL
    #define TSP_DNLD_ADDR_ALI_SHIFT                 4UL //Bit [11:4] of DMA_RADDR[19:0]
    #define TSP_DNLD_NUM_MASK                       0xFFFF0000UL
    #define TSP_DNLD_NUM_SHFT                       16UL

    REG32                           TSP_Ctrl;                           // 0xbf802be8   0x7a
    #define TSP_CTRL_CPU_EN                         0x00000001UL
    #define TSP_CTRL_SW_RST                         0x00000002UL
    #define TSP_CTRL_DNLD_START                     0x00000004UL
    #define TSP_CTRL_DNLD_DONE                      0x00000008UL          // see 0x78 for related information
    #define TSP_CTRL_TSFILE_EN                      0x00000010UL
    #define TSP_CTRL_R_PRIO                         0x00000020UL
    #define TSP_CTRL_W_PRIO                         0x00000040UL
    #define TSP_CTRL_IF0_PAD_SHIFT                  7UL
    #define TSP_CTRL_IF0_PAD0_SEL                   0x00000000UL
    #define TSP_CTRL_IF0_PAD1_SEL                   0x00000080UL
    #define TSP_CTRL_ICACHE_EN                      0x00000100UL
    #define TSP_CTRL_CPU2MI_R_PRIO                  0x00000400UL
    #define TSP_CTRL_CPU2MI_W_PRIO                  0x00000800UL
    #define TSP_CTRL_I_EL                           0x00000000UL
    #define TSP_CTRL_I_BL                           0x00001000UL
    #define TSP_CTRL_D_EL                           0x00000000UL
    #define TSP_CTRL_D_BL                           0x00002000UL

    REG32                           PKT_CNT;                            // 0xbf802bf0   0x7c
    #define TSP_PKT_CNT_MASK                        0x000000FFUL
    #define TSP_DBG_SEL_MASK                        0xFFFF0000UL
    #define TSP_DBG_SEL_SHIFT                       16UL

    REG16                           HwInt_Stat;                         // 0xbf802bf8   0x7e
    #define TSP_HWINT_STATUS_MASK                   0xFF00UL              // Tsp2hk_int enable bits.
    #define TSP_HWINT_TSP_PVR_TAIL0_STATUS          0x0100UL
    #define TSP_HWINT_TSP_PVR_MID0_STATUS           0x0200UL
    #define TSP_HWINT_TSP_PVR_TAIL1_STATUS          0x0100UL
    #define TSP_HWINT_TSP_PVR_MID1_STATUS           0x0200UL
    #define TSP_HWINT_TSP_HK_INT_FORCE_STATUS       0x0400UL
    #define TSP_HWINT_TSP_FILEIN_MID_INT_STATUS     0x0800UL
    #define TSP_HWINT_TSP_FILEIN_TAIL_INT_STATUS    0x1000UL
    #define TSP_HWINT_TSP_SW_INT_STATUS             0x2000UL
    #define TSP_HWINT_TSP_DMA_READ_DONE             0x4000UL
    #define TSP_HWINT_TSP_AV_PKT_ERR                0x8000UL

    #define TSP_HWINT_HW_PVR_MASK                   (TSP_HWINT_TSP_PVR_TAIL0_STATUS|TSP_HWINT_TSP_PVR_TAIL1_STATUS)
    #define TSP_HWINT_ALL                           (TSP_HWINT_TSP_SW_INT_STATUS|TSP_HWINT_HW_PVR_MASK)

    REG16                           TSP_Ctrl1;                          // 0xbf802bfc   0x7f
    #define TSP_CTRL1_FILEIN_TIMER_ENABLE           0x0001UL
    #define TSP_CTRL1_TSP_FILE_NON_STOP             0x0002UL              //Set 1 to enable TSP file data read without timer check
    #define TSP_CTRL1_FILEIN_PAUSE                  0x0004UL
    #define TSP_CTRL1_FILE_CHECK_WP                 0x0008UL
    #define TSP_CTRL1_FILE_CHECK_WP_8x8             0x0040UL              // Select read threshold when playing back TS file to be at least 8x8 bytes
    #define TSP_CTRL1_STANDBY                       0x0080UL
    #define TSP_CTRL1_INT2NOEA                      0x0100UL
    #define TSP_CTRL1_FILEIN_ENABLE                 0x0200UL
    #define TSP_CTRL1_FORCE_XIU_WRDY                0x0400UL
    #define TSP_CTRL1_CMDQ_RESET                    0x0800UL
    #define TSP_CTRL1_DLEND_EN                      0x1000UL              // Set 1 to enable little-endian mode in TSP CPU
    #define TSP_CTRL1_PVR_CMD_QUEUE_ENABLE          0x2000UL
    #define TSP_CTRL1_FILE_WP_LOAD                  0x4000UL
    #define TSP_CTRL1_DMA_RST                       0x8000UL

    //----------------------------------------------
    // 0xBF802C00 MIPS direct access
    //----------------------------------------------
    REG32                           MCU_Data0;                           // 0xbf802c00   0x00
    #define TSP_MCU_DATA_ALIVE                      TSP_MCU_CMD_ALIVE

    REG32                           LPcr1;                              // 0xbf802c08   0x02
    REG32                           LPcr2;                              // 0xbf802c10   0x04
    REG32                           reg160C;                            // 0xbf802c18   0x06
    #define TSP_LPCR1_WLD                           0x00000001UL          // Set 1 to load LPCR1 value
    #define TSP_LPCR1_RLD                           0x00000002UL          // Set 1 to read LPCR1 value (Default: 1)
    #define TSP_LPCR2_WLD                           0x00000004UL          // Set 1 to load LPCR2 value
    #define TSP_LPCR2_RLD                           0x00000008UL          // Set 1 to read LPCR2 value (Default: 1)
    #define TSP_RECORD192_EN                        0x00000010UL          // 160C bit(5)enable TS packets with 192 bytes on record mode
    #define TSP_FILEIN192_EN                        0x00000020UL          // 160C bit(5)enable TS packets with 192 bytes on file-in mode
    #define TSP_DOUBLE_BUF_EN                       0x00000040UL          // tsin->pinpon filein->single
    #define TSP_ORZ_DMAW_PROT_EN                    0x00000080UL          // 160C bit(7) open RISC DMA write protection
    #define TSP_CLR_PIDFLT_BYTE_CNT                 0x00000100UL          // Clear pidflt0_file byte counter
    #define TSP_WATCH_DOG_EN                        0x00000200UL          // Set 1 to count watch dog and release blocking scheme on second section interface when meeting timeout
    #define TSP_BLK_DISABLE                         0x00000400UL          // Disable blocking scheme for second section interface
    #define TSP_DOUBLE_BUF_SWITCH                   0x00000800UL          // tsin->single filein->pinpon
    #define TSP_DOUBLE_BUF_DESC                     0x00004000UL          // 160d bit(6) remove buffer limitation
    #define TSP_TIMESTAMP_RESET                     0x00008000UL          // 160d bit(7) reset timestamp
    #define TSP_DIS_MIU_RQ                          0x00100000UL          // Disable miu R/W request for reset TSP usage
    #define TSP_GDMA2WBSRAM_EN                      0x00200000UL          // Enable GDMA bridge for boot from SPI
    #define TSP_GDMA2WBSRAM_ENDIAN_BIG              0x00400000UL          // Byte order of 4-byte GDMA to QMEM.
    #define TSP_RM_DMA_GLITCH                       0x00800000UL          // Fix sec_dma overflow glitch
    #define TSP_RESET_VFIFO                         0x01000000UL          // Reset VFIFO -- ECO Done
    #define TSP_RESET_AFIFO                         0x02000000UL          // Reset AFIFO -- ECO Done
    #define TSP_RESET_VFIFO3D                       0x20000000UL
    #define TSP_REG_RESET_GDMA                      0x04000000UL          // Set 1 to reset GDMA bridge
    #define TSP_CLR_ALL_FLT_MATCH                   0x08000000UL          // Set 1 to clean all flt_match in a packet

    REG32                           PktChkSizeFilein;                   // 0xbf802c20   0x08
    #define TSP_PKT_SIZE_MASK                       0x000000FFUL
    #define TSP_PKT192_BLK_DIS_FIN                  0x00000100UL          // Set 1 to disable file-in timestamp block scheme
    #define TSP_AV_CLR                              0x00000200UL          // Clear AV FIFO overflow flag and in/out counter
    #define TSP_HW_STANDBY_MODE                     0x00000400UL          // Set 1 to disable all SRAM in TSP for low power mode automatically
    #define TSP_LIVEAB_SEL                          0x00010000UL          // switch tsif1 to filein
    #define TSP_CNT_34B_DEFF_EN                     0x00020000UL          // Switch STC DIFF Mode (Output STC+DIFF to MVD and MAD)
    #define TSP_DMA_OVERFLOW_MET_SEL                0x00040000UL
    #define TSP_SYSTIME_MODE_STC64                  0x00080000UL
    #define TSP_SEC_DMA_BURST_EN                    0x00800000UL
    #define TSP_DUP_PKT_SKIP_VD                     0x02000000UL
    #define TSP_DUP_PKT_SKIP_V3D                    0x04000000UL
    #define TSP_DUP_PKT_SKIP_AV                     0x08000000UL

    REG32                           Dnld_Ctrl2;                         // 0xbf802c28   0x0a
    #define TSP_DNLD_ADDR_MASK1                     0x001F0000UL
    #define TSP_DNLD_ADDR_SHFT1                     16UL
    #define TSP_BLK_AF_SCRMB_BIT                    0x00000400UL          // Set 1 to block update pids to scrmb when the there are only AF in the pkt
    #define TSP_TSIF0_CLK_STAMP_27_EN               0x00000100UL
    #define TSP_PVR1_CLK_STAMP_27_EN                0x00000200UL  
    #define TSP_CMQ_WORD_EN                         0x00400000UL          // Set 1 to access CMDQ related registers in word.
    #define TSP_NEW_WARB_BURST_MODE_DIS             0x00800000UL
    #define TSP_V3D_PID_BYPASS                      0x08000000UL
    #define TSP_AVPID_ST_SEL                        0x20000000UL
        #define TSP_AVPID_ST_AV                     0x20000000UL
        #define TSP_AVPID_ST_AU2V3D                 0x00000000UL
    #define TSP_PS_VID3D_EN                         0x40000000UL
    #define TSP_PREVENT_OVF_META                    0x80000000UL

    REG32                           TsPidScmbStatTsin;                  // 0xbf802c30   0x0c
    REG32                           TsPidScmbStatFile;                  // 0xbf802c38   0x0e
    REG32                           _xbf802c40_xbf802c70[7];            // 0xbf802c40-0xbf802c70  0x10-0x1C -reserved

    REG32                           DbgInfo_Ctrl1;                      //0xbf802c78     0x1E
    #define TSP_CLR_SRC_MASK                0x00070000UL
    #define TSP_CLR_SRC_SHIFT                       16UL
        #define TSP_CLR_DISCINT_SRC_CH0             0x00010000UL
        #define TSP_CLR_DISCINT_SRC_CHFILE          0x00020000UL
    #define TSP_DISCONTI_VD_CLR                     0x00080000UL
    #define TSP_DISCONTI_V3D_CLR                    0x00100000UL
    #define TSP_DISCONTI_AUD_CLR                    0x00200000UL
    #define TSP_SRAM_COLLISION_CLR                  0x02000000UL

    REG32                           VQ0_BASE;                           // 0x3a2c80       0x20
    #define TSP_VQ0_BASE_MASK                       0x03FFFFFFUL
    REG32                           VQ0_CTRL;                           // 0x3a2c88       0x22
    #define TSP_VQ0_SIZE_192PK_MASK                 0x0000FFFFUL
    #define TSP_VQ0_SIZE_192PK_SHIFT                0UL
    #define TSP_VQ0_WR_THRESHOLD_MASK               0x000F0000UL
    #define TSP_VQ0_WR_THRESHOLD_SHIFT              16UL
    #define TSP_VQ0_PRIORTY_THRESHOLD_MASK          0x00F00000UL
    #define TSP_VQ0_PRIORTY_THRESHOL_SHIFT          20UL
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_MASK          0x0F000000UL
    #define TSP_VQ0_FORCE_FIRE_CNT_1K_SHIFT         24UL
    #define TSP_VQ0_RESET                           0x10000000UL
    #define TSP_VQ0_OVERFLOW_INT_EN                 0x40000000UL  // Enable the interrupt for overflow happened on Virtual Queue path
    #define TSP_VQ0_CLR_OVERFLOW_INT                0x80000000UL  // Clear the interrupt and the overflow flag
    REG32                           VQ0_STATUS;                         // 0x3a2c90       0x24
    #define TSP_VQ0_STATUS_MASK                     0x0000FFFFUL
    #define TSP_VQ0_STATUS_SHIFT                    0UL
    #define TSP_VQ0_EN                              0x00010000UL
    #define TSP_REQ_VQ_RX_THRESHOLD_MASKE           0x00060000UL
    #define TSP_REQ_VQ_RX_THRESHOLD_SHIFT           17UL
        #define TSP_REQ_VQ_RX_THRESHOLD_LEN1        0x00000000UL
        #define TSP_REQ_VQ_RX_THRESHOLD_LEN2        0x00020000UL
        #define TSP_REQ_VQ_RX_THRESHOLD_LEN4        0x00040000UL
        #define TSP_REQ_VQ_RX_THRESHOLD_LEN8        0x00060000UL
    #define TSP_VQ_FILE_EN                          0x00100000UL
    #define TSP_VQ_PINGPONG_EN                      0x00200000UL

    REG32                           _xbf802c98_xbf802ce0[10];            // 0xbf802c98-0xbf802ce0  0x26-0x38 -reserved
    REG32                           DMAW1_1;                            // 0xbf802ce8   0x3a
    #define TSP_DMAW1_ALI_SHIFT                     10UL
    #define TSP_DMAW1_LBND_MASK1                    0x0000FFFFUL
    #define TSP_DMAW1_LBND_SHIFT1                   0UL
    #define TSP_DMAW1_UBND_MASK1                    0x0000FFFFUL
    #define TSP_DMAW1_UBND_SHIFT1                   16UL
    REG32                           DMAW2_1;                            // 0xbf802cf0   0x3c
    #define TSP_DMAW2_ALI_SHIFT                     10UL
    #define TSP_DMAW2_LBND_MASK1                    0x0000FFFFUL
    #define TSP_DMAW2_LBND_SHIFT1                   0UL
    #define TSP_DMAW2_UBND_MASK1                    0x0000FFFFUL
    #define TSP_DMAW2_UBND_SHIFT1                   16UL
    REG32                           ORZ_DMAW;                           // 0xbf802cf8   0x3e
    #define TSP_ORZ_ALI_SHIFT                       2UL
    #define TSP_ORZ_DMAW_LBND                       0x0000ffffUL
    #define TSP_ORZ_DMAW_UBND                       0xffff0000UL
    #define TSP_ORZ_DMAW_UBND_SHIFT                 16UL

    REG32_L                         CA_CTRL;                            // 0xbf802d00   0x40
    #define TSP_CA_CTRL_MASK                        0x000000ffUL
    #define TSP_CA_INPUT_TSIF0_LIVEIN               0x00000001UL
    #define TSP_CA_INPUT_TSIF0_FILEIN               0x00000002UL
    #define TSP_CA_INPUT_TSIF1                      0x00000004UL
    #define TSP_CA_AVPAUSE                          0x00000008UL
    #define TSP_CA_OUTPUT_PLAY_LIVE                 0x00000010UL
    #define TSP_CA_OUTPUT_PLAY_FILE                 0x00000020UL
    #define TSP_CA_OUTPUT_REC                       0x00000040UL

    REG32                           REG_ONEWAY;                         // 0xbf802d08   0x42
    #define TSP_ONEWAY_REC_DISABLE                  0x00000001UL          //Disable record descrambled stream
    #define TSP_ONEWAY_PVR_PORT                     0x00000002UL          //PVR buffer registers are oneway
    #define TSP_ONEWAY_FW_PORT                      0x00000004UL          //Orz fw buffer registers are oneway
    #define TSP_ONEWAY_QMEM_PORT                    0x00000008UL          //QMEM registers are oneway

    REG32                           _xbf802d10_xbf802d48[8];             // 0xbf802d10 -0xbf802d48  0x44-0x52 -reserved
    REG32                           MOBF_PVR_KEY;                        // 0xbf802d50        0x54
    #define TSP_MOBF_PVR_KEY0_MASK                  0x00FF0000UL
    #define TSP_MOBF_PVR_KEY0_SHIFT                 16UL
    #define TSP_MOBF_PVR_KEY1_MASK                  0xFF000000UL
    #define TSP_MOBF_PVR_KEY1_SHIFT                 24UL

    REG32                           VQ1_Base;                           // 0xbf802d58   0x56

    REG32                           _xbf802d60_xbf802d68[2];            // 0xbf802d60 -0xbf802d68  0x58 -0x5a -reserved

    REG32                           VQ1_Size;                           // 0xbf802d70   0x5C
    #define TSP_VQ1_SIZE_192BYTE_MASK               0xffff0000UL
    #define TSP_VQ1_SIZE_192BYTE_SHIFT              16UL

    REG32                           VQ1_Config;                         // 0xbf802d78   0x5e
    #define TSP_VQ1_WR_THRESHOLD_MASK               0x0000000FUL
    #define TSP_VQ1_WR_THRESHOLD_SHIFT              0UL
    #define TSP_VQ1_PRI_THRESHOLD_MASK              0x000000F0UL
    #define TSP_VQ1_PRI_THRESHOLD_SHIFT             4UL
    #define TSP_VQ1_FORCEFIRE_CNT_1K_MASK           0x00000F00UL
    #define TSP_VQ1_FORCEFIRE_CNT_1K_SHIFT          8UL
    #define TSP_VQ1_RESET                           0x00001000UL
    #define TSP_VQ1_OVF_INT_EN                      0x00004000UL
    #define TSP_VQ1_CLR_OVF_INT                     0x00008000UL

    REG32                           reg16C0;                            // 0xbf802d80   0x60
    #define TSP_ORZ_DMAW_LBND_LSB8                  0x000000ffUL
    #define TSP_ORZ_DMAW_UBND_LSB8                  0x0000ff00UL
    #define TSP_ORZ_DMAW_UBND_LSB8_SHIFT            8UL
    #define TSP_ORZ_DMAW_BND_ALT_SHIFT              2UL
    #define TSP_TS_WATCH_DOG_MASK                   0xFFFF0000UL
    #define TSP_TS_WATCH_DOG_SHIFT                  16UL

    REG32                           reg16C4;                            // 0xbf802d88   0x62
    #define TSP_DMAW_BND_ALI_SHIFT                  2UL
    #define TSP_DMAW_LBND_LSB8                      0x000000ffUL
    #define TSP_DMAW_UBND_LSB8                      0x0000ff00UL
    #define TSP_DMAW_UBND_LSB8_SHIFT                8UL
    REG32                           reg16C8;                            // 0xbf802d90   0x64
    #define TSP_DMAW1_BND_ALI_SHIFT                 2UL
    #define TSP_DMAW1_LBND_LSB8                     0x000000ffUL
    #define TSP_DMAW1_UBND_LSB8                     0x0000ff00UL
    #define TSP_DMAW1_UBND_LSB8_SHIFT               8UL
    REG32                           reg16CC;                            // 0xbf802d98   0x66
    #define TSP_DMAW2_BND_ALI_SHIFT                 2UL
    #define TSP_DMAW2_LBND_LSB8                     0x000000ffUL
    #define TSP_DMAW2_UBND_LSB8                     0x0000ff00UL
    #define TSP_DMAW2_UBND_LSB8_SHIFT               8UL
    REG32                           _xbf802da0_xbf802da8[2];            // 0xbf802da0 -0xbf802da8  0x68-0x6a -reserved
    REG16                           SwInt_Stat1_L;                      // 0xbf802dB0   0x6c
    #define TSP_HWINT2_EN_MASK                      0x00FFUL
    #define TSP_HWINT2_STATUS_MASK                  0xFF00UL
    #define TSP_HWINT2_STATUS_SHIFT                 8UL
    #define TSP_HWINT2_DMA_WPR_STATUS               0x0100UL
    #define TSP_HWINT2_ORZ_WPR_STATUS               0x0200UL
    #define TSP_HWINT2_VQ_OVERFLOW_STATUS           0x1000UL

    REG16                           SwInt_Stat1_M;
    REG32                           SwInt_Stat1_H;                     // 0xbf802dB8   0x6e
    #define TSP_SWINT1_H_SHFT                       0UL
    #define TSP_SWINT1_H_MASK                       0x0000FFFFUL
    
    REG32                           TimeStamp_FileIn;                  // 0xbf802dC0   0x70

    REG32                           HW2_Config3;                       // 0xbf802dC0   0x72
    #define TSP_RM_OVERFLOW_GLITCH                  0x00000008UL
    #define TSP_DUP_PKT_CNT_CLR                     0x00000040UL
    #define TSP_REC_AT_SYNC_DIS                     0x00000100UL
    #define TSP_PVR_ALIGN_EN                        0x00000200UL
    #define TSP_FORCE_SYNC_EN                       0x00000400UL
    #define TSP_DMA_FLUSH_EN                        0x00040000UL
    #define TSP_STR2MI_WP_LD                        0x00080000UL
    #define TSP_CLR_SEC_DMAW_OVERFLOW               0x10000000UL
    #define TSP_PUSI_3BYTE_MODE                     0x40000000UL         // set 1 to set pusi flag only in first 3 byte of the payload

    REG32                           DMAW3_LBND;                        // 0xbf802dC0   0x74, MIU Address 3 of the lower bound of DMA write protection when REG15B4[1] is 1
    REG32                           DMAW3_UBND;                        // 0xbf802dC0   0x76, MIU Address 3 of the upper bound of DMA write protection when REG15B4[1] is 1
        #define TSP_DMAW3_BND_ALI_SHIFT             2UL
        #define TSP_DMAW3_LBND_MASK                 0x00FFFFFFUL
        #define TSP_DMAW3_UBND_MASK                 0x00FFFFFFUL
    REG32                           DMAW4_LBND;                        // 0xbf802dC0   0x78, MIU Address 4 of the lower bound of DMA write protection when REG15B4[1] is 1
    REG32                           DMAW4_UBND;                        // 0xbf802dC0   0x7a, MIU Address 4  of the upper bound of DMA write protection when REG15B4[1] is 1
        #define TSP_DMAW4_BND_ALI_SHIFT             2UL
        #define TSP_DMAW4_LBND_MASK                 0x00FFFFFFUL
        #define TSP_DMAW4_UBND_MASK                 0x00FFFFFFUL
    REG32                           MCU_Data1;                         // 0xbf802dC0   0x7C
} REG_Ctrl;

// TSP part 2
typedef struct _REG_Ctrl2
{
    REG16                           Overflow0;                          // 0xbf803900   0x40
    #define AFIFO_EVER_OVERFLOW                     0x0020UL
    #define VFIFO_EVER_OVERFLOW                     0x0080UL
    #define V3DFIFO_EVER_OVERFLOW                   0x0100UL
    #define PVR_1_EVER_OVERFLOW                     0x0200UL
    #define VQ_TX0_EVER_OVERFLOW                    0x1000UL
    #define VQ_TX1_EVER_OVERFLOW                    0x2000UL

    REG16                           Overflow1;                          // 0xbf803904   0x41
    #define SEC_PINGPONG_EVER_OVERFLOW              0x0001UL
    #define SEC_SINGLE_EVER_OVERFLOW                0x0002UL
    #define SEC_DMAW_OVERFLOW                       0x0004UL

    REG16                           FifoStatus;                         // 0xbf803908   0x42
    #define AFIFO_STATUS_MASK                       0x000FUL
    #define AFIFO_STATUS_SHFT                       0UL
    #define VFIFO_STATUS_MASK                       0x0F00UL
    #define VFIFO_STATUS_SHFT                       8UL
    #define V3DFIFO_STATUS_MASK                     0xF000UL
    #define V3DFIFO_STATUS_SHFT                     12UL

    REG16                           PvrFifoStatus;                      // 0xbf80390C   0x43
    #define PVR_1_STATUS_MASK                       0x000FUL
    #define PVR_1_STATUS_SHFT                       0UL

    REG16                           VQTxFifoStatus;                      // 0xbf803910   0x44
    #define VQ_TX0_STATUS_MASK                      0x000FUL
    #define VQ_TX0_STATUS_SHFT                      0UL
    #define VQ_TX1_STATUS_MASK                      0x0F00UL
    #define VQ_TX1_STATUS_SHFT                      8UL

    REG16                           PktCnt_TS0;                          // 0xbf803914   0x45
    REG16                           PktCnt_TS1;                          // 0xbf803918   0x46
    REG16                           PktCnt_TS2;                          // 0xbf80391C   0x47
    REG16                           PktCnt_File;                         // 0xbf803920   0x48
    #define DISCONTI_CNT_AUDIO_MASK                 0x000FUL
    #define DISCONTI_CNT_AUDIO_SHFT                 0UL
    #define DISCONTI_CNT_VIDEO_MASK                 0x0F00UL
    #define DISCONTI_CNT_VIDEO_SHFT                 8UL
    #define DISCONTI_CNT_V3D_MASK                   0xF000UL
    #define DISCONTI_CNT_V3D_SHFT                   12UL

    #define DROP_CNT_AUDIO_MASK                     0x000FUL
    #define DROP_CNT_AUDIO_SHFT                     0UL
    #define DROP_CNT_VIDEO_MASK                     0x0F00UL
    #define DROP_CNT_VIDEO_SHFT                     8UL
    #define DROP_CNT_V3D_MASK                       0xF000UL
    #define DROP_CNT_V3D_SHFT                       12UL

    REG16                           LockPktCnt;                          // 0xbf803924   0x49
    #define TS0_LOCK_CNT_MASK                       0x000FUL
    #define TS0_LOCK_CNT_SHFT                       0UL
    #define TS1_LOCK_CNT_MASK                       0x00F0UL
    #define TS1_LOCK_CNT_SHFT                       4UL
    #define TSCB_LOCK_CNT_MASK                      0xF000UL
    #define TSCB_LOCK_CNT_SHFT                      12UL

    REG16                           AVPktCnt;                            // 0xbf803928   0x4A
    #define VIDEO_PKT_CNT_MASK                      0x000FUL
    #define VIDEO_PKT_CNT_SHFT                      0UL
    #define AUDIO_PKT_CNT_MASK                      0x00F0UL
    #define AUDIO_PKT_CNT_SHFT                      4UL


    REG16                           PktErrStatus;                        // 0xbf80392C   0x4B
    REG16                           PidMatched0;                         // 0xbf803930   0x4C
    REG16                           PidMatched1;                         // 0xbf803934   0x4D
    REG16                           PidMatched2;                         // 0xbf803938   0x4E
    REG16                           PidMatched3;                         // 0xbf80393C   0x4F
    
    REG16                           Sram_Collision;                      // 0xbf803940   0x50

    REG16                           dummy_0x51_0x6F[0x70-0x51];          // 0xbf803998   0x51 ~6F    
    
    REG32                           Qmem_Config;                         // 0xbf8039FC   0x70
    #define TSP_QMEM_DBG_MODE                       0x00000001UL
    #define TSP_TSP_SEL_SRAM                        0x00000002UL
    #define TSP_QMEM_DBG_RADDR                      0xFFFF0000UL

    REG32                           Qmem_Dbg_Rd;                         // 0xbf8039FC   0x72

    REG16                           dummy_0x74_0x76[0x77-0x74];          // 0xbf803998   0x74 ~77
    
    REG16                           HwCfg0;                              // 0x77
    #define TSP_TSIFCFG_WB_FSM_RESET                0x0001UL
    #define TSP_TSIFCFG_WB_FSM_RESET_FINISH         0x0002UL
    
    #define MASK_SCR_VID_EN                         0x0004UL
    #define MASK_SCR_VID_3D_EN                      0x0008UL
    #define MASK_SCR_AUD_EN                         0x0010UL
    #define MASK_SCR_PVR1_EN                        0x0040UL
    #define PREVENT_SRAM_COLLISION                  0x0080UL

    #define TSP_3WIRE_SERIAL_MODE_MASK              0x0300UL           //set 1 to enable 3 wire serial in mode: Combine valid and clk.Valid always 1 and gated clk when no data in
    #define TSP_3WIRE_SERIAL_TSIF0                  0x0100UL
    #define TSP_3WIRE_SERIAL_TSIF1                  0x0200UL

    REG16                           HwCfg1;                              // 0x78
    #define NEW_OVERFLOW_MODE                       0x0001UL
    #define AF_PKT_LOSS_BYTE_ECO                    0x0004UL  // reg_adp_sel_sync_byte_cnt_out_en .issue befor:use section mode get AF only pkt may loss one byte
    #define FIX_PINPON_SYNC_IN_ECO                  0x0008UL  // if enable, overflow flag will be clear after dma_abort and buffer not full. if disable, overflow flag will be clear once buffer un-full.
    #define DMA_WADDR_INC_NEW_MODE                  0x0010UL  // default 0
    #define DIS_CNT_INC_BY_PAYLOAD                  0x0040UL
    #define UPDATE_SCRAMBLE_PID_PUSI                0x0080UL
    REG16                           dummy_0x79_0x7E[0x7f-0x79];
    REG16                           HwCfg2;                               // 0xbf8039FC   0x7F
    #define HW_INFO_SRC_MODE_MASK                   0x0003UL
    #define REG_SRC_SEL                             0x0001UL
    #define REG_DROP_PKT_MODE                       0x0002UL
    #define REG_RST_CC_MODE                         0x0004UL

} REG_Ctrl2;

// TSP part 3
typedef struct _REG_Ctrl3
{
    REG32                           ReSample_Config;                     // 0xbf8C1400   0x00
    #define TSP_RESAMPLE_EN                         0x00000001UL
    #define TSP_TS_SOURCE_MASK                      0x00000006UL
    #define TSP_RETURN_STATUS                       0x0000FFF8UL    
    #define TSP_RESAMPLE_CTRL                       0xFFFF0000UL

    REG32                           Clk_Phase;                           // 0xbf8C1408   0x02
    #define TSP_CLK_PHASE                           0x000000FFUL
    #define TSP_CLK_PHASE_DIFF                      0x0000FF00UL
    #define TSP_MIN_CLK                             0x00FF0000UL
    #define TSP_MAX_CLK                             0x00FF0000UL

    REG32                           MaxMin_SyncValid;                    // 0xbf8C1410   0x04
    #define TSP_MIN_SYNC                            0x000000FFUL
    #define TSP_MAX_SYNC                            0x0000FF00UL    
    #define TSP_MIN_VALID                           0x00FF0000UL
    #define TSP_MAX_VALID                           0xFF000000UL

    REG32                           MaxMin_dat0dat1;                     // 0xbf8C1418   0x06
    #define TSP_MIN_DAT0                            0x000000FFUL
    #define TSP_MAX_DAT0                            0x0000FF00UL    
    #define TSP_MIN_DAT1                            0x00FF0000UL
    #define TSP_MAX_DAT1                            0xFF000000UL

    REG32                           MaxMin_dat2dat3;                     // 0xbf8C1420   0x08
    #define TSP_MIN_DAT2                            0x000000FFUL
    #define TSP_MAX_DAT2                            0x0000FF00UL    
    #define TSP_MIN_DAT3                            0x00FF0000UL
    #define TSP_MAX_DAT3                            0xFF000000UL
    
    REG32                           MaxMin_dat4dat5;                     // 0xbf8C1428   0x0A
    #define TSP_MIN_DAT4                            0x000000FFUL
    #define TSP_MAX_DAT4                            0x0000FF00UL    
    #define TSP_MIN_DAT5                            0x00FF0000UL
    #define TSP_MAX_DAT5                            0xFF000000UL

    REG32                           MaxMin_dat6dat7;                     // 0xbf8C1430   0x0C
    #define TSP_MIN_DAT6                            0x000000FFUL
    #define TSP_MAX_DAT6                            0x0000FF00UL    
    #define TSP_MIN_DAT7                            0x00FF0000UL
    #define TSP_MAX_DAT7                            0xFF000000UL

    REG32                           _xbf8C1438_xbf8C1478[9];             // 0xbf8C1438 - 0xbf8C1478  0x0E-0x1E -reserved

    REG32                           Hw_Semaphore0;                       // 0xbf8C1480   0x20
    #define TSP_HW_SEMAPHORE0                       0x0000FFFFUL
    #define TSP_HW_SEMAPHORE1                       0xFFFF0000UL
    
    REG32                           Hw_Semaphore1;                       // 0xbf8C1488   0x22
    #define TSP_HW_SEMAPHORE2                       0x0000FFFFUL
    #define TSP_TIMESTAMP_ECO                       0x04000000UL
    REG32                           Hw_Config;                           // 0xbf8C1490   0x24
    #define TSP_3WIRE_SERIAL_MODE                   0x00FF0000UL    
    #define TSP_PREVENT_SRAM_COLLISION              0x01000000UL
    #define TSP_INIT_TIMESTAMP_FILEIN               0x00010000UL
    #define TSP_INIT_TIMESTAMP_MMFI0                0x00020000UL
    #define TSP_INIT_TIMESTAMP_MMFI1                0x00040000UL
    REG32                           Hw_FI_Timestamp;                     // 0xbf8C1492   0x26
    REG32                           Hw_MMFI0_Timestamp;                  // 0x28
    REG32                           Hw_MMFI1_Timestamp;                  // 0x2A
    REG32                           Hw_ECO;                              // 0x2C
    #define TSP_TIMESTAMP_RING                      0x00000001UL
    #define TSP_LPCR_RING                           0x00000002UL

} REG_Ctrl3;

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

