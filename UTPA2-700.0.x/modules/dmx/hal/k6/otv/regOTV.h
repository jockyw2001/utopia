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
//  File name: regOTV.h
//  Description: OTV Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OTV_REG_H_
#define _OTV_REG_H_

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

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

// Software

#define OTV_BANK0_REG_CTRL_BASE           (0xE1400)    //0x70A<<9   //bank 0x170A
#define OTV_BANK1_REG_CTRL_BASE           (0xE1600)	   //0x70B<<9   //bank 0x170B

#define OTV0_REG_CTRL_BASE                OTV_BANK0_REG_CTRL_BASE
#define OTV1_REG_CTRL_BASE                (OTV_BANK0_REG_CTRL_BASE+0x100)
#define OTV2_REG_CTRL_BASE                OTV_BANK1_REG_CTRL_BASE
#define OTV3_REG_CTRL_BASE                (OTV_BANK1_REG_CTRL_BASE+0x100)

#define OTV0_PIDFLT_BASE                  (OTV0_REG_CTRL_BASE+0x2C)
#define OTV1_PIDFLT_BASE                  (OTV1_REG_CTRL_BASE+0x2C)
#define OTV2_PIDFLT_BASE                  (OTV2_REG_CTRL_BASE+0x2C)
#define OTV3_PIDFLT_BASE                  (OTV3_REG_CTRL_BASE+0x2C)

#define OTV0_EVENT_MASK_BASE              (OTV0_REG_CTRL_BASE+0xC0)
#define OTV1_EVENT_MASK_BASE              (OTV1_REG_CTRL_BASE+0xC0)
#define OTV2_EVENT_MASK_BASE              (OTV2_REG_CTRL_BASE+0xC0)
#define OTV3_EVENT_MASK_BASE              (OTV3_REG_CTRL_BASE+0xC0)

//#define OTV0_BANK0_PIDFLT_BASE             (OTV0_BANK0_REG_CTRL_BASE+0x80)
//#define OTV0_BANK1_EVENT_MASK_BASE         (OTV0_BANK1_REG_CTRL_BASE+0x180)
//#define OTV0_BANK1_PAYLOAD_MASK_BASE       (OTV0_BANK1_REG_CTRL_BASE+0x100)

//#define OTV1_BANK0_REG_CTRL_BASE           (0x23A00)
//#define OTV1_BANK1_REG_CTRL_BASE           (0x23C00)

//#define OTV1_BANK0_PIDFLT_BASE             (OTV1_BANK0_REG_CTRL_BASE+0x80)
//#define OTV1_BANK1_EVENT_MASK_BASE         (OTV1_BANK1_REG_CTRL_BASE+0x180)
//#define OTV1_BANK1_PAYLOAD_MASK_BASE       (OTV1_BANK1_REG_CTRL_BASE+0x100)

// Payload/Event Mask flag
#define OTV_EVENT_MASK            0xffffffff
#define OTV_EVENT_AFE             0x00000001      // Adaptation Field Extension flag
#define OTV_EVENT_TPD             0x00000002      // Transport Private Data flag
#define OTV_EVENT_SP              0x00000004      // Splicing Point flag
#define OTV_EVENT_OPCR            0x00000008      // OPCR flag
#define OTV_EVENT_PCR             0x00000010      // PCR flag
#define OTV_EVENT_ESPI            0x00000020      // Elementary Stream Priority Indicator
#define OTV_EVENT_RAI             0x00000040      // Random Access Indicator
#define OTV_EVENT_DI              0x00000080      // Discontinuity Indicator
#define OTV_EVENT_CESOS           0x00000100      // Change of Elementary Stream to Odd  Scrambled
#define OTV_EVENT_CESES           0x00000200      // Change of Elementary Stream to Even Scrambled
#define OTV_EVENT_CESNS           0x00000400      // Change of Elementary Stream to Not  Scrambled
#define OTV_EVENT_PUSI            0x00000800      // Payload Unit Start Indicator
#define OTV_EVENT_FPR             0x00001000      // First Packet Recorded


typedef struct _OTV_REG32
{
    volatile MS_U16                 L;
    volatile MS_U16                 empty_L;
    volatile MS_U16                 H;
    volatile MS_U16                 empty_H;
} OTV_REG32;

typedef struct _OTV_REG16
{
    volatile MS_U16                 data;
    volatile MS_U16                 _resv;
} OTV_REG16;

//typedef OTV_REG16                       REG_PidFlt;  //bruce

typedef struct _REG_OTV
{
    //----------------------------------------------
    // 0xBF245000 MIPS direct access
    //----------------------------------------------
    OTV_REG32                           OTV_PktTimer;                          // 0xbf245000   0x00
    OTV_REG32                           OTV_PktNum;                            // 0xbf245008   0x02
    OTV_REG16                           _bf245010;                             // 0xbf245010   0x04

    OTV_REG16                           OTV_PktSize2;                          // 0xbf245014   0x05
    #define OTV_PKT_SIZE_MASK              0xFF00
    #define OTV_PKT_SIZE_188                 0xBB
    #define OTV_PKT_SHIFT                       8

    OTV_REG16                           OTV_EventLogCtrlStatus;                // 0xbf245018   0x06
    #define OTV_EVENT_FIFO_NUM_MASK        0x001F
    #define OTV_EVENT_FIFO_FULL            0X0020
    #define OTV_EVENT_FIFO_EMPTY           0x0040
    #define OTV_EVENT_FIFO_RDLV_MASK       0x0300
    #define OTV_EVENT_FIFO_RDLV_SHIFT           8
    #define OTV_EVENT_FIFO_WTLV_MASK       0x0C00
    #define OTV_EVENT_FIFO_WTLV_SHIFT          10
    #define OTV_EVENT_FIFO_RD_OVF          0x1000
    #define OTV_EVENT_FIFO_WT_OVF          0x2000

    OTV_REG16                           OTV_PktChkSize2;                       // 0xbf24501c   0x07
    OTV_REG16                           OTV_HW_CTRL0;                          // 0xbf245020   0x08
    #define OTV_SW_RESET                   0x0001
    #define OTV_TEI_SKIP_PKT               0x0040
    #define OTV_CLR_EVENT_OVERFLOW         0x0200
    #define OTV_MASK_MD                    0x0800
    #define OTV_REC_EFRAME_EN              0x1000
    #define OTV_RM_PIPE                    0x4000                              // OTV RASP auto flush

    OTV_REG16                           OTV_HW_CTRL1;                          // 0xbf245024   0x09
    #define OTV_ALT_TS_SIZE                0x0080
    #define OTV_TICK_TIME_MASK             0x6000
    #define OTV_EFRAME_RD                  0x8000                              // read eframe fifo

    OTV_REG16                           OTV_HW_CTRL2;                          // 0xbf245028   0x0a
    #define OTV_INT_TIMER_EN               0x0001
    #define OTV_INT_EVENT_EN               0x0002
    #define OTV_INT_TIME_WATER_MASK        0x000C
    #define OTV_INT_TIME_WATER_SHIFT            2
    #define OTV_INT_EVENT_WATER_MASK       0x01F0
    #define OTV_INT_EVENT_WATER_SHIFT           4
    #define OTV_PKT_NUM_TIMER_SEL          0x2000                              // The recording packet timestamp: default 0:replace with pkt timer, 1: pkt number
    #define OTV_TIMESTAMP_OTV_EN           0x4000                              // The recording packet timestamp sel: default 0: no OTV timestamp replace, with PVR timestamp replace;  1: with OTV timestamp replace
    #define OTV_TIMESTAMP_PVR_TO_OTV_EN    0x8000                              // The OTV packet timestamp source select : default 0: match OTV local timstamp  1: match PVR timstamp

    OTV_REG16                           OTV_Pidflt_0;                          // 0xbf24502c   0x0b
    OTV_REG16                           OTV_Pidflt_1;                          // 0xbf245030   0x0c
    OTV_REG16                           OTV_Pidflt_2;                          // 0xbf245034   0x0d
    OTV_REG16                           OTV_Pidflt_3;                          // 0xbf245038   0x0e

    OTV_REG16                           OTV_HW_CTRL3;                          // 0xbf24503c   0x0f
    #define OTV_PKT_SW_RST                 0x0001                              // If this bit set to 1, pkt_num & pkt timestamp will be reset
    #define OTV_PKT_NUM_SRC_SEL            0x0002                              // 0:count by OTV pid hit, pkt_num index start from 0,1,2,...etc
                                                                               // 1:count by pkt dmx hit, pkt_num index start from 1,2,3,...etc

    #define OTV_PKT_NUM_TIMER_LOCK         0x0004                              // pkt num/timer will be locked for reading if this bit is set to 1
    #define OTV_FIRST_PKT_TIMER_BASE_EN    0x0008                              // If this bit set to 1, timer will be (orginal timer - first pkt timer)
    #define OTV_EVENT_FLT_RST              0x0010                              // If this bit set to 1, OTV event flt will reset (The bit only work after Kirin U02, including U02)

    OTV_REG32                           OTV_EventDescriptor;                   // 0xbf201d40   0x10
    OTV_REG32                           OTV_EventPktNum;                       // 0xbf201d48   0x12
    OTV_REG32                           OTV_EventPktTimer;                     // 0xbf201d50   0x14
    OTV_REG32                           OTV_EventPktPCR;                       // 0xbf201d58   0x16
    OTV_REG16                           OTV_EventPktPID;                       // 0xbf201d60   0x18

    OTV_REG16                           OTV_PushCnt;                           // 0xbf201d64   0x19
    #define OTV_PUSH_CNT_MASK              0xFF00                              // OTV auto flush push counter

    OTV_REG32                           OTV_Debug;                             // 0xbf245068   0x1a

    OTV_REG16                           OTV_INT_Flag;                          // 0xbf201d70   0x1c
    #define OTV_INT_FLAG_MASK              0x00FF
    #define OTV_INT_FROM_OTV0_TIMER        0x0001                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV0_EVENT        0x0002                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV1_TIMER        0x0004                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV1_EVENT        0x0008                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV2_TIMER        0x0010                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV2_EVENT        0x0020                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV3_TIMER        0x0040                              // write 0 to clear(only valid at bank OTV0)
    #define OTV_INT_FROM_OTV3_EVENT        0x0080                              // write 0 to clear(only valid at bank OTV0)

    OTV_REG16                           OTV_HW_CTRL4;                          // 0xbf245074   0x1d
    OTV_REG16                           OTV_HW_CTRL5;                          // 0xbf245078   0x1e
    OTV_REG16                           OTV_HW_CTRL6;                          // 0xbf24507c   0x1f

    OTV_REG32                           OTV_Pid_PayLoadMask_0;                 // 0xbf245080   0x20
    OTV_REG32                           OTV_Pid_PayLoadMask_1;                 // 0xbf245088   0x22
    OTV_REG32                           OTV_Pid_PayLoadMask_2;                 // 0xbf245090   0x24
    OTV_REG32                           OTV_Pid_PayLoadMask_3;                 // 0xbf245098   0x26
    OTV_REG32                           OTV_Pid_PayLoadMask_4;                 // 0xbf2450a0   0x28

    OTV_REG16                           _bf2450a8_bf2450bc[0x30-0x2a];         // 0xbf2450a8-bf2450bc   0x2a-0x2f

    OTV_REG32                           OTV_Pid_EventMask_0;                   // 0xbf2450c0   0x30
    OTV_REG32                           OTV_Pid_EventMask_1;                   // 0xbf2450c8   0x32
    OTV_REG32                           OTV_Pid_EventMask_2;                   // 0xbf2450d0   0x34
    OTV_REG32                           OTV_Pid_EventMask_3;                   // 0xbf2450d8   0x36
    OTV_REG32                           OTV_Pid_EventMask_4;                   // 0xbf2450e0   0x38

} REG_OTV;

#endif // _OTV_REG_H_
