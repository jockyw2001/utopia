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
//  File name: regRASP.h
//  Description: RASP Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RASP_REG_H_
#define _RASP_REG_H_

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

//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------
#define RASP_PIDFLT_NUM             24
#define RASP_PIDFLT_DEF             0x1fff
#define RASP_EVENT_NUM              16
#define RASP_EVENT_FIFO_DEPTH       32*4

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

#define REG_RASP0_BASE           (0x111B00<<1)
#define REG_RASP0_FILE_BASE      (0x112500<<1)

#define REG_RASP1_BASE           (0x111D00<<1)
#define REG_RASP1_FILE_BASE      (0x112600<<1)

#define REG_CHIPTOP_RP_BASE      (0x101E00<<1)
#define REG_CLKGEN_RP_BASE       (0x100B00<<1)
#define REG_CLKGEN2_BASE         (0x100A00<<1)

#define RASP_PID_PKT_CORPT_EN    0x2000


#define REG_RASP_PKT_TIMER_L        0x0000
#define REG_RASP_PKT_TIMER_H        0x0001
#define REG_RASP_PKT_NUM_L          0x0002
#define REG_RASP_PKT_NUM_H          0x0003
#define REG_RASP_CORRUPTION         0x0004
#define RASP_FROMTO_MASK            0x00FF
#define RASP_TO_SHIFT               8

#define REG_RASP_CORPT_PKTSIZE      0x0005
#define RASP_CORPT_DATA             0x00FF
#define RASP_PKT_SIZE               0xFF00
#define RASP_PKT_SIZE_188           0xBB
#define RASP_PKT_SHIFT              8

#define REG_RASP_EVENTLOG_STATUS    0x0006
#define RASP_EVENT_FIFO_NUM_MASK    0x0000001F
#define RASP_EVENT_FIFO_FULL        0X00000020
#define RASP_EVENT_FIFO_EMPTY       0x00000040
#define RASP_EVENT_FIFO_RDLV_MASK   0x00000300
#define RASP_EVENT_FIFO_RDLV_SHIFT  8
    #define RASP_EVENT_FIFO_WTLV_MASK   0x00000C00
    #define RASP_EVENT_FIFO_WTLV_SHIFT  10
    #define RASP_EVENT_FIFO_RD_OVF      0x00001000
    #define RASP_EVENT_FIFO_WT_OVF      0x00002000

#define REG_RASP_CONFIG_TSIF2       0x0007

#define REG_RASP_HW_CTRL0           0x0008
    #define RASP_SW_RESET               0x00000001
    #define RASP_TSIF2_ENABLE           0x00000002
    #define RASP_TSIF2_DATA_SWP         0x00000004
    #define RASP_TSIF2_PARA_SEL         0x00000008
    #define RASP_TSIF2_EXT_SYNC         0x00000010
    #define RASP_FILEIN_EN              0x00000020
    #define RASP_RISING_SYNC_DETECT     0x00000080
    #define RASP_FALLING_VALID_DETECT   0x00000100
    #define RASP_CLR_EVENT_OVERFLOW     0x00000200
    #define RASP_REC_EVENT_FIFO_EN      0x00001000
    #define RASP_CLEAR_ECM_PKT_NUM      0x00002000

#define REG_RASP_HW_CTRL1           0x0009
    #define RASP_REC_PID                0x00000001
    #define RASP_STR2MIU_EN             0x00000002
    #define RASP_PINGPONG_EN            0x00000004
    #define RASP_ALT_TS_SIZE_EN         0x00000080
    #define RASP_STR2MIU_RST_WADDR      0x00000010
    #define RASP_SERIAL_EXT_SYNC_1T     0x00000400
//192 aligement
    #define RASP_BURST_LEN_MASK         0x00001800
    #define RASP_BURST_LEN_SHIFT        0x0000000B

    #define RASP_EVENT_FIFO_READ        0x00008000

#define REG_RASP_HW_CTRL2           0x000A
    #define RASP_INT_TIMER_EN           0x00000001
    #define RASP_INT_EVENT_EN           0x00000002

    #define RASP_INT_TIMER_MASK         0x0000000C
    #define RASP_INT_TIMER_SHIFT        2

    #define RASP_INT_EVENT_MASK         0x000001F0
    #define RASP_INT_EVENT_SHIFT        4

    #define RASP_PAYLOAD_BURST_LEN      0x00000400
    #define RASP_ECM_BURST_LEN          0x00001000

#define REG_RASP_HW_CTRL3           0x000B
    #define RASP_PAYLD2MIU_EN           0x00000001
    #define RASP_PAYLD2MIU_PINGPONE     0x00000002
    #define RASP_PAYLD2MIU_SWAP         0x00000004
    #define RASP_PAYLD2MIU_RST_WADDR    0x00000008
    #define RASP_PAYLD2_PKT_192_DIS      0x00000040
    #define RASP_PAYLD2MIU_LPCR_WT      0x00000080
    #define RASP_ECM2MIU_EN             0x00000100
    #define RASP_ECM2MIU_RST_WADDR      0x00000800
    #define RASP_ECMPKT_192_DIS          0x00004000
    #define RASP_ECM2MIU_LPCR_WT        0x00008000

#define REG_RASP_CLK90K_DIV         0x000C
#define REG_RASP_INT_CLR            0x000D
#define REG_RASP_INT_ENABLE         0x000E
#define REG_RASP_INT_STATUS         0x000F
    #define RASP_INT_EFRAME_RD_OVF      0x00000001
    #define RASP_INT_EFRAME_WT_OVF      0x00000002
    #define RASP_INT_STR2MIU            0x0000000C           // 2 BIT FOR WHAT ?
    #define RASP_INT_PAYLD2MIU          0x00000030
    #define RASP_INT_ECM2MIU            0x000000C0
    #define RASP_INT_TIME_WATER_MARK    0x00000100
    #define RASP_INT_EVENT_WATER_MARK   0x00000200
    #define RASP_INT_ECM_PKT_RDY        0x00000400
    #define RASP_INT_PVR2MIU            0x00001800

#define REG_RASP_STR2MIU_HEAD1_L    0x0010
#define REG_RASP_STR2MIU_HEAD1_H    0x0011
#define REG_RASP_STR2MIU_TAIL1_L    0x0012
#define REG_RASP_STR2MIU_TAIL1_H    0x0013
#define REG_RASP_STR2MIU_MID1_L     0x0014
#define REG_RASP_STR2MIU_MID1_H     0x0015

#define REG_RASP_STR2MIU_HEAD2_L    0x0016
#define REG_RASP_STR2MIU_HEAD2_H    0x0017
#define REG_RASP_STR2MIU_TAIL2_L    0x0018
#define REG_RASP_STR2MIU_TAIL2_H    0x0019
#define REG_RASP_STR2MIU_MID2_L     0x001A
#define REG_RASP_STR2MIU_MID2_H     0x001B


#define REG_RASP_HW_CTRL4           0x001C
    #define RASP_STREAM_192_EN          0x0001
    #define RASP_STREAM_LPCR_WLD        0x0002
    #define RASP_TS_FF_BYPASS           0x0004
    #define RASP_TS_FF_CLR_OVERFLOW     0x0008
    #define RASP_TS_FF_FULL_SEL_MASK    0x0070
    #define RASP_TS_FF_FULL_SEL_SHFT    4
    #define RASP_TS_STR2MI_WP_LD_DIS    0x0080
    #define RASP_TS_PAY2MI_WP_LD_DIS    0x0100
    #define RASP_REC_AT_SYNC_DIS        0x0400
    #define RASP_AUTO_STREAM_47_48      0x2000

#define REG_RASP_HW_CTRL5           0x001D

#define REG_RASP_PIDFLT_N(n)        (0x0020 + n)

#define REG_RASP_EVENT_DESCR_L      0x0050
#define REG_RASP_EVENT_DESCR_H      0x0051

#define REG_RASP_EVENT_PKT_NUM_L    0x0052
#define REG_RASP_EVENT_PKT_NUM_H    0x0053
#define REG_RASP_EVENT_PKT_TIMER_L  0x0054
#define REG_RASP_EVENT_PKT_TIMER_H  0x0055
#define REG_RASP_EVENT_PKT_PCR_L    0x0056
#define REG_RASP_EVENT_PKT_PCR_H    0x0057
#define REG_RASP_EVENT_PKT_PID      0x0058

#define REG_RASP_PAYLOAD2MIU_HEAD1_L    0x0060
#define REG_RASP_PAYLOAD2MIU_HEAD1_H    0x0061
#define REG_RASP_PAYLOAD2MIU_TAIL1_L    0x0062
#define REG_RASP_PAYLOAD2MIU_TAIL1_H    0x0063
#define REG_RASP_PAYLOAD2MIU_MID1_L     0x0064
#define REG_RASP_PAYLOAD2MIU_MID1_H     0x0065

#define REG_RASP_PAYLOAD2MIU_HEAD2_L    0x0066
#define REG_RASP_PAYLOAD2MIU_HEAD2_H    0x0067
#define REG_RASP_PAYLOAD2MIU_TAIL2_L    0x0068
#define REG_RASP_PAYLOAD2MIU_TAIL2_H    0x0069
#define REG_RASP_PAYLOAD2MIU_MID2_L     0x006A
#define REG_RASP_PAYLOAD2MIU_MID2_H     0x006B

#define REG_RASP_PAY_LPCR1_BUF_L    0x006C
#define REG_RASP_PAY_LPCR1_BUF_H    0x006D
#define REG_RASP_PAY_LPCR1_L        0x006E
#define REG_RASP_PAY_LPCR1_H        0x006F

#define REG_RASP_ECM2MIU_HEAD1_L    0x0070
#define REG_RASP_ECM2MIU_HEAD1_H    0x0071
#define REG_RASP_ECM2MIU_TAIL1_L    0x0072
#define REG_RASP_ECM2MIU_TAIL1_H    0x0073
#define REG_RASP_ECM2MIU_MID1_L     0x0074
#define REG_RASP_ECM2MIU_MID1_H     0x0075

#define REG_RASP_ECM_LPCR1_BUF_L    0x0076
#define REG_RASP_ECM_LPCR1_BUF_H    0x0077
#define REG_RASP_ECM_LPCR1_L        0x0078
#define REG_RASP_ECM_LPCR1_H        0x0079

#define REG_RASP_STR_LPCR1_BUF_L    0x007A
#define REG_RASP_STR_LPCR1_BUF_H    0x007B
#define REG_RASP_STR_LPCR1_L        0x007C
#define REG_RASP_STR_LPCR1_H        0x007D

#define REG_RASP_STATUS             0x007F

#define REG_RASP_ECM_PID_N(n)      (0x0080 + 0x0012 + n)

#define REG_RASP_ECM_TID           (0x0080 + 0x0016)
#define REG_RASP_ECM_LOCK_CTRL     (0x0080 + 0x0017)
#define REG_RASP_CA_INT            (0x0080 + 0x0018)
#define REG_RASP_ECM_INT_STATE     (0x0080 + 0x0019)

#define REG_RASP_ECM_PID_4         (0x0080 + 0x0030)
#define REG_RASP_ECM_PID_5         (0x0080 + 0x0031)

#define REG_RASP_ECM45_LOCK_CTRL   (0x0080 + 0x0032)

#define REG_RASP_ECM_PACKET_NUM_L  (0x0080 + 0x0034)
#define REG_RASP_ECM_PACKET_NUM_H  (0x0080 + 0x0035)

#define REG_RASP_PKT_MEET_SIZE_L   (0x0080 + 0x003A)
#define REG_RASP_PKT_MEET_SIZE_H   (0x0080 + 0x003B)
#define REG_RASP_PKT_SET		   (0x0080 + 0x003C)
	#define RASP_PKT_RESET_NUMBER		0x0100
	#define RASP_PKT_RESET_TIMER		0x0200

#define REG_RASP_PAYLOAD_MASK_N(n) (0x0080 + 0x0040 + 2*n)

#define REG_RASP_EVENT_MASK_N(n)   (0x0080 + 0x0060 + 2*n)

#define REG_RFILEIN_RESET           0x0000
#define REG_RFILEIN_RESET_ALL           0x001F

#define REG_RFILEIN_CTRL_0              0x0001
#define REG_RFILEIN_PKT_CHK_SIZE    0x00FF
#define REG_RFILEIN_PKT_CHK_SIZE_188    0x00BB
#define REG_RFILEIN_PKT_CHK_SIZE_192    0x00BF
#define RASP_RFILEIN_PORT_SEL_FILE    0x0100
#define RASP_RFILEIN_ALIGN_EN        0x0200
#define RASP_RFILEIN_TIMER_EN        0x0400
#define RASP_RFILEIN_INPUT_EN        0x0800

	#define RASP_RFILEIN_PKT192_EN			0x1000
	#define RASP_RFILEIN_PKT192_BLK_DIS		0x2000

#define REG_RFILEIN_TIMER           0x0002
#define REG_RFILEIN_CTRL_1          0x0003
#define REG_RFILEIN_LPCR2_WLD    0x0002
#define REG_RFILEIN_LPCR2_LOAD    0x0020
#define REG_RFILEIN_FLUSH_AUTO    0x0100
#define REG_RFILEIN_FLUSH        0x0200
#define REG_RFILEIN_TIMER_192FIX		0x0800	//fix 192 mode issue
#define REG_RFILEIN_RST_PKT_TSTAMP      0x1000  //reset file in stream timestamp


#define REG_RFILEIN_DTS_CONFIG_0    0x0004
#define REG_RFILEIN_DBG_SEL         0x000A

#define REG_RFILEIN_ADDR_L          0x0020
#define REG_RFILEIN_ADDR_H          0x0021
#define REG_RFILEIN_SIZE_L          0x0022
#define REG_RFILEIN_SIZE_H          0x0023
#define REG_RFILEIN_START           0x0024
#define RASP_FILEIN_START            0x0001
#define REG_RFILEIN_LPCR2_BUF_L        0x0025
#define REG_RFILEIN_LPCR2_RD_L        0x0027
#define REG_RFILEIN_STREAM_TIMESTAMP_L		0x002f
#define REG_RFILEIN_STREAM_TIMESTAMP_H		0x0030


//there are still some registers not defined....maybe type later.....

#define REG_RFILEIN_CMDQ_STATUS     0x002B
    #define RFILEIN_CMD_WR_CNT_MASK     0xF
    #define RFILEIN_CMD_FIFO_FULL       (0x1<<6)
    #define RFILEIN_CMD_FIFO_EMPTY      (0x1<<7)
    #define RFILEIN_CMD_WR_LEVEL_MASK   (0x3<<8)
    #define RFILEIN_CMD_WR_COUNT_MASK   (0x1F)

#define REG_RFILEIN_DBG_L           0x002C
#define REG_RFILEIN_DBG_H           0x002D

// Software
#define RASP0_BANK0_REG_CTRL_BASE           (0x23600)   //0x11B<<9   //bank 0x111B
#define RASP0_BANK1_REG_CTRL_BASE           (0x23800)   //0x11C<<9   //bank 0x111C

#define RASP0_BANK0_PIDFLT_BASE             (RASP0_BANK0_REG_CTRL_BASE+0x80)
#define RASP0_BANK1_EVENT_MASK_BASE         (RASP0_BANK1_REG_CTRL_BASE+0x180)
#define RASP0_BANK1_PAYLOAD_MASK_BASE       (RASP0_BANK1_REG_CTRL_BASE+0x100)
#define RASP0_BANK1_ECM_PIDFLT_BASE         (RASP0_BANK1_REG_CTRL_BASE+0x48)


#define RASP1_BANK0_REG_CTRL_BASE           (0x23A00)   //0x11D<<9   //bank 0x111D
#define RASP1_BANK1_REG_CTRL_BASE           (0x23C00)   //0x11E<<9   //bank 0x111E

#define RASP1_BANK0_PIDFLT_BASE             (RASP1_BANK0_REG_CTRL_BASE+0x80)
#define RASP1_BANK1_EVENT_MASK_BASE         (RASP1_BANK1_REG_CTRL_BASE+0x180)
#define RASP1_BANK1_PAYLOAD_MASK_BASE       (RASP1_BANK1_REG_CTRL_BASE+0x100)
#define RASP1_BANK1_ECM_PIDFLT_BASE         (RASP1_BANK1_REG_CTRL_BASE+0x48)


// Payload/Event Mask flag
#define RASP_BYPASS_MASK            0xffffffff
#define RASP_BYPASS_AFE             0x00000001      // adaptation field extension
#define RASP_BYPASS_TPD             0x00000002      // transport private data
#define RASP_BYPASS_SP              0x00000004      // splicing point
#define RASP_BYPASS_OPCR            0x00000008      // OPCR
#define RASP_BYPASS_PCR             0x00000010      // PCR
#define RASP_BYPASS_ESPI            0x00000020      // elementary stream priority indicator
#define RASP_BYPASS_RAI             0x00000040      //random access indicator
#define RASP_BYPASS_DI              0x00000080      // discontinue indicator
#define RASP_BYPASS_ESOS            0x00000100      // elementary stream not scrambled
#define RASP_BYPASS_ESES            0x00000200      // elementary stream even scrambled
#define RASP_BYPASS_ESNS            0x00000400      // elementary stream odd scrambled
#define RASP_BYPASS_PUSI            0x00000800      // payload unit start indicator
#define RASP_BYPASS_FPR             0x00001000      // first packet recorded
#define RASP_BYPASS_RASP_Tick       0x80000000      // rasp tick

//Ctrl MASK
#define RASP_CTRL_MASK              0x0000ffff

typedef struct _REG32
{
    volatile MS_U16                 L;
    volatile MS_U16                 empty_L;
    volatile MS_U16                 H;
    volatile MS_U16                 empty_H;
} REG32;

typedef struct _REG16
{
    volatile MS_U16                 data;
    volatile MS_U16                 _resv;
} REG16;

typedef enum
{
    E_NDSRASP_FILEIN = 0,
    E_NDSRASP_LIVEIN,
} FILEIN_TSIF_Mode;

typedef REG16                       REG_PidFlt;

typedef struct _REG_RASP
{
    //----------------------------------------------
    // 0xBF223600 MIPS direct access
    //----------------------------------------------
    REG32                           RASP_PktTimer;                          // 0xbf201c00   0x00
    REG32                           RASP_PktNum;                            // 0xbf201c08   0x02
    REG16                           RASP_CorptFromTo;                       // 0xbf201c10   0x04
    #define RASP_FROMTO_MASK            0x00FF
    #define RASP_TO_SHIFT               8
    REG16                           RASP_CorptData_PktSize2;                // 0xbf201c14   0x05
    #define RASP_CORPT_DATA             0x00FF
    #define RASP_PKT_SIZE               0xFF00
    #define RASP_PKT_SIZE_188           0xBB
    #define RASP_PKT_SHIFT              8
    REG16                           RASP_EventlogCtrlStatus;                // 0xbf201c18   0x06
    #define RASP_EVENT_FIFO_NUM_MASK    0x0000001F
    #define RASP_EVENT_FIFO_FULL        0X00000020
    #define RASP_EVENT_FIFO_EMPTY       0x00000040
    #define RASP_EVENT_FIFO_RDLV_MASK 0x00000300
    #define RASP_EVENT_FIFO_RDLV_SHIFT 8
    #define RASP_EVENT_FIFO_WTLV_MASK 0x00000C00
    #define RASP_EVENT_FIFO_WTLV_SHIFT 10
    #define RASP_EVENT_FIFO_RD_OVF      0x00001000
    #define RASP_EVENT_FIFO_WT_OVF      0x00002000

    REG16                           RASP_PktChkSize2_SyncByte2;             // 0xbf201c1c   0x07
    REG16                           RASP_HW_CTRL0;                          // 0xbf201c20   0x08
    #define RASP_SW_RESET               0x00000001
    #define RASP_TSIF2_ENABLE           0x00000002
    #define RASP_TSIF2_DATA_SWP         0x00000004
    #define RASP_TSIF2_PARA_SEL         0x00000008
    #define RASP_TSIF2_EXT_SYNC         0x00000010
    #define RASP_FILEIN_EN              0x00000020
    #define RASP_RISING_SYNC_DETECT     0x00000080
    #define RASP_FALLING_VALID_DETECT   0x00000100
    #define RASP_CLR_EVENT_OVERFLOW     0x00000200
    #define RASP_REC_EVENT_FIFO_EN      0x00001000
    #define RASP_CLEAR_ECM_PKT_NUM      0x00002000

    REG16                           RASP_HW_CTRL1;                          // 0xbf201c24   0x09
    #define RASP_REC_PID                0x00000001
    #define RASP_STR2MIU_EN             0x00000002
    #define RASP_PINGPONG_EN            0x00000004
    #define RASP_ALT_TS_SIZE_EN         0x00000080
    #define RASP_STR2MIU_RST_WADDR      0x00000010
    #define RASP_SERIAL_EXT_SYNC_1T     0x00000400
//192 aligement
    #define RASP_BURST_LEN_MASK         0x00001800
    #define RASP_BURST_LEN_SHIFT        0x0000000B
    #define RASP_BURST_LEN              0x00000002

    #define RASP_EVENT_FIFO_READ        0x00008000



    REG16                           RASP_HW_CTRL2;                          // 0xbf201c28   0x0a
    #define RASP_INT_TIMER_EN           0x00000001
    #define RASP_INT_EVENT_EN           0x00000002
    #define RASP_INT_TIMER_MASK         0x0000000C
    #define RASP_INT_TIMER_SHIFT        2
    #define RASP_INT_EVENT_MASK         0x000001F0
    #define RASP_INT_EVENT_SHIFT        4
    #define RASP_PAYLOAD_BURST_LEN      0x00000400
    #define RASP_ECM_BURST_LEN          0x00001000

    REG16                           RASP_HW_CTRL3;                          // 0xbf201c2c   0x0b
    #define RASP_PAYLD2MIU_EN           0x00000001
    #define RASP_PAYLD2MIU_PINGPONE     0x00000002
    #define RASP_PAYLD2MIU_SWAP         0x00000004
    #define RASP_PAYLD2MIU_RST_WADDR    0x00000008
    #define RASP_PAYLD2MIU_LPCR_WT      0x00000080
    //#define RASP_ECM2MIU_EN             0X00000100
    //#define RASP_ECM2MIU_RST_WADDR      0X00000800
    //#define RASP_ECM2MIU_LPCR_WT        0X00008000


    REG16                           RASP_C90K_Divisor;                      // 0xbf201c30   0x0c
    REG16                           RASP_Int_CLR;                           // 0xbf201c34   0x0d
    REG16                           RASP_Int_EN;                            // 0xbf201c38   0x0e
    REG16                           RASP_Int_Status;                        // 0xbf201c3c   0x0f


    REG32                           RASP_Str2miu_Head;                      // 0xbf201c40   0x10
    REG32                           RASP_Str2miu_Tail;                      // 0xbf201c48   0x12
    REG32                           RASP_Str2miu_Mid;                       // 0xbf201c50   0x14
    REG32                           RASP_Str2miu_Head2;                     // 0xbf201c58   0x16
    REG32                           RASP_Str2miu_Tail2;                     // 0xbf201c60   0x18
    REG32                           RASP_Str2miu_Mid2;                      // 0xbf201c68   0x1a
    REG16                           RASP_HW_CTRL4;                          // 0xbf201c70   0x1c
    #define RASP_STREAM_192_EN          0x0001
    #define RASP_STREAM_LPCR_WLD        0x0002
    #define RASP_TS_FF_BYPASS           0x0004
    #define RASP_TS_FF_CLR_OVERFLOW     0x0008
    #define RASP_TS_FF_FULL_SEL_MASK    0x0070
    #define RASP_TS_FF_FULL_SEL_SHFT    4
    #define RASP_TS_STR2MI_WP_LD_DIS    0x0080
    #define RASP_TS_PAY2MI_WP_LD_DIS    0x0100
    #define RASP_REC_AT_SYNC_DIS        0x0400
    #define RASP_AUTO_STREAM_47_48      0x2000

    REG16                           RASP_HW_CTRL5;                          // 0xbf201c74   0x1d

    REG16                           _bf201c78;                              // 0xbf201c78   0x1e

    REG16                           RASP_HW_Status1;                        // 0xbf201c7C   0x1f
    REG16                           RASP_Pidflt_0;                          // 0xbf201c80   0x20
    REG16                           RASP_Pidflt_1;                          // 0xbf201c84   0x21
    REG16                           RASP_Pidflt_2;                          // 0xbf201c88   0x22
    REG16                           RASP_Pidflt_3;                          // 0xbf201c8c   0x23
    REG16                           RASP_Pidflt_4;                          // 0xbf201c90   0x24
    REG16                           RASP_Pidflt_5;                          // 0xbf201c94   0x25
    REG16                           RASP_Pidflt_6;                          // 0xbf201c98   0x26
    REG16                           RASP_Pidflt_7;                          // 0xbf201c9c   0x27
    REG16                           RASP_Pidflt_8;                          // 0xbf201ca0   0x28
    REG16                           RASP_Pidflt_9;                          // 0xbf201ca4   0x29
    REG16                           RASP_Pidflt_a;                          // 0xbf201ca8   0x2a
    REG16                           RASP_Pidflt_b;                          // 0xbf201cac   0x2b
    REG16                           RASP_Pidflt_c;                          // 0xbf201cb0   0x2c
    REG16                           RASP_Pidflt_d;                          // 0xbf201cb4   0x2d
    REG16                           RASP_Pidflt_e;                          // 0xbf201cb8   0x2e
    REG16                           RASP_Pidflt_f;                          // 0xbf201cbc   0x2f
    REG16                           RASP_Pidflt_10;                         // 0xbf201cc0   0x30
    REG16                           RASP_Pidflt_11;                         // 0xbf201cc4   0x31
    REG16                           RASP_Pidflt_12;                         // 0xbf201cc8   0x32
    REG16                           RASP_Pidflt_13;                         // 0xbf201ccc   0x33
    REG16                           RASP_Pidflt_14;                         // 0xbf201cd0   0x34
    REG16                           RASP_Pidflt_15;                         // 0xbf201cd4   0x35
    REG16                           RASP_Pidflt_16;                         // 0xbf201cd8   0x36
    REG16                           RASP_Pidflt_17;                         // 0xbf201cdc   0x37
    #define RASP_PID_PKT_CORPT_EN    0x2000

    REG16                           _bf201d00[0x50-0x38];                   // 0xbf201ce0-bf201d38   0x38-0x4f

    REG32                           RASP_EventDescriptor;                   // 0xbf201d40   0x50
    REG32                           RASP_EventPktNum;                       // 0xbf201d48   0x52
    REG32                           RASP_EventPktTimer;                     // 0xbf201d50   0x54
    REG32                           RASP_EventPktPCR;                       // 0xbf201d58   0x56
    REG16                           RASP_EventPktPID;                       // 0xbf201d60   0x58

    REG16                           _bf201d68_bf201d68[0x60-0x59];          // 0xbf201d64-bf201d7c   0x59-0x5f

    REG32                           RASP_Payload2miu_Head;                  // 0xbf201d80   0x60
    REG32                           RASP_Payload2miu_Tail;                  // 0xbf201d88   0x62
    REG32                           RASP_Payload2miu_Mid;                   // 0xbf201d90   0x64
    REG32                           RASP_Payload2miu_Head2;                 // 0xbf201d98   0x66
    REG32                           RASP_Payload2miu_Tail2;                 // 0xbf201da0   0x68
    REG32                           RASP_Payload2miu_Mid2;                  // 0xbf201da8   0x6a

    REG32                           RASP_PayLPCR1Buf;                       // 0xbf201db0   0x6c
    REG32                           RASP_PayLPCR1;                          // 0xbf201db8   0x6e
    REG32                           RASP_Ecm2miu_Head;                      // 0xbf201dc0   0x70
    REG32                           RASP_Ecm2miu_Tail;                      // 0xbf201dc8   0x72
    REG32                           RASP_Ecm2miu_Mid;                       // 0xbf201dd0   0x74
    REG32                           RASP_EcmLPCR1Buf;                       // 0xbf201dd8   0x76
    REG32                           RASP_EcmLPCR1;                          // 0xbf201de0   0x78
    REG32                           RASP_StrLPCR1Buf;                       // 0xbf201de8   0x7a
    REG32                           RASP_StrLPCR1;                          // 0xbf201df0   0x7c

    REG32                           _bf201db8;                              // 0xbf201df8   0x7e-0x7f

    //----------------------------------------------
    // 0xBF223800 MIPS direct access
    //----------------------------------------------
    REG16                           _bf201e00_bf201e44[0x12-0x0];           // 0xbf201e00-bf201e44   0x00-0x11

    REG16                           RASP_EcmPidflt_0;                       // 0xbf201e48   0x12
    REG16                           RASP_EcmPidflt_1;                       // 0xbf201e4c   0x13
    REG16                           RASP_EcmPidflt_2;                       // 0xbf201e50   0x14
    REG16                           RASP_EcmPidflt_3;                       // 0xbf201e54   0x15
    REG16                           RASP_Ecm_reg_16;                        // 0xbf201e58   0x16
    REG16                           RASP_Ecm_reg_17;                        // 0xbf201e5c   0x17

    REG16                           RASP_Ecm_reg_18;                        // 0xbf201e60   0x18
    REG16                           RASP_Ecm_reg_19;                        // 0xbf201e64   0x19

    REG32                           RASP_EcmDummy1a;                        // 0xbf201e68   0x1a
    REG32                           RASP_EcmDummy1c;                        // 0xbf201e70   0x1c
    REG32                           RASP_EcmDummy1e;                        // 0xbf201e78   0x1e
    REG32                           RASP_EcmDummy20;                        // 0xbf201e80   0x20
    REG32                           RASP_EcmDummy22;                        // 0xbf201e88   0x22
    REG32                           RASP_EcmDummy24;                        // 0xbf201e90   0x24
    REG32                           RASP_EcmDummy26;                        // 0xbf201e98   0x26
    REG32                           RASP_EcmDummy28;                        // 0xbf201ea0   0x28
    REG32                           RASP_EcmDummy2a;                        // 0xbf201ea8   0x2a
    REG32                           RASP_EcmDummy2c;                        // 0xbf201eb0   0x2c
    REG32                           RASP_EcmDummy2e;                        // 0xbf201eb8   0x2e

    REG16                           RASP_EcmPidflt_4;                       // 0xbf201ec0   0x30
    REG16                           RASP_EcmPidflt_5;                       // 0xbf201ec4   0x31
    REG16                           RASP_Ecm_reg_32;                        // 0xbf201ec8   0x32
    REG16                           RASP_EcmDummy33;                        // 0xbf201ecc   0x33

    REG32                           RASP_EcmPktNum;                         // 0xbf201ed0   0x34

    REG32                           RASP_EcmDummy36;                        // 0xbf201ed8   0x36
    REG32                           RASP_EcmDummy38;                        // 0xbf201ee0   0x38
    REG32                           RASP_EcmDummy3a;                        // 0xbf201ee8   0x3a
    REG32                           RASP_EcmDummy3c;                        // 0xbf201ef0   0x3c
    REG32                           RASP_EcmDummy3e;                        // 0xbf201ef8   0x3e

    REG32                           RASP_PayLoadMask_0;                     // 0xbf201f00   0x40
    REG32                           RASP_PayLoadMask_1;                     // 0xbf201f08   0x42
    REG32                           RASP_PayLoadMask_2;                     // 0xbf201f10   0x44
    REG32                           RASP_PayLoadMask_3;                     // 0xbf201f18   0x46
    REG32                           RASP_PayLoadMask_4;                     // 0xbf201f20   0x48
    REG32                           RASP_PayLoadMask_5;                     // 0xbf201f28   0x4a
    REG32                           RASP_PayLoadMask_6;                     // 0xbf201f30   0x4c
    REG32                           RASP_PayLoadMask_7;                     // 0xbf201f38   0x4e
    REG32                           RASP_PayLoadMask_8;                     // 0xbf201f40   0x50
    REG32                           RASP_PayLoadMask_9;                     // 0xbf201f48   0x52
    REG32                           RASP_PayLoadMask_a;                     // 0xbf201f50   0x54
    REG32                           RASP_PayLoadMask_b;                     // 0xbf201f58   0x56
    REG32                           RASP_PayLoadMask_c;                     // 0xbf201f60   0x58
    REG32                           RASP_PayLoadMask_d;                     // 0xbf201f68   0x5a
    REG32                           RASP_PayLoadMask_e;                     // 0xbf201f70   0x5c
    REG32                           RASP_PayLoadMask_f;                     // 0xbf201f78   0x5e

    REG32                           RASP_EventMask_0;                       // 0xbf201f80   0x60
    REG32                           RASP_EventMask_1;                       // 0xbf201f88   0x62
    REG32                           RASP_EventMask_2;                       // 0xbf201f90   0x64
    REG32                           RASP_EventMask_3;                       // 0xbf201f98   0x66
    REG32                           RASP_EventMask_4;                       // 0xbf201f70   0x68
    REG32                           RASP_EventMask_5;                       // 0xbf201f78   0x6a
    REG32                           RASP_EventMask_6;                       // 0xbf201f80   0x6c
    REG32                           RASP_EventMask_7;                       // 0xbf201f88   0x6e
    REG32                           RASP_EventMask_8;                       // 0xbf201f90   0x70
    REG32                           RASP_EventMask_9;                       // 0xbf201f98   0x72
    REG32                           RASP_EventMask_a;                       // 0xbf201fa0   0x74
    REG32                           RASP_EventMask_b;                       // 0xbf201fa8   0x76
    REG32                           RASP_EventMask_c;                       // 0xbf201fb0   0x78
    REG32                           RASP_EventMask_d;                       // 0xbf201fb8   0x7a
    REG32                           RASP_EventMask_e;                       // 0xbf201fc0   0x7c
    REG32                           RASP_EventMask_f;                       // 0xbf201fc8   0x7e
} REG_RASP;

#endif // _RASP_REG_H_
