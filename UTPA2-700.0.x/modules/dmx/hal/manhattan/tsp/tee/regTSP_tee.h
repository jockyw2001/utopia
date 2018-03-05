//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2015 - 2020 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2015-2020 MStar Semiconductor, Inc.
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
//  File name: regTSP_tee.h
//  Description: Transport Stream Processor (TSP) Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _TSP_TEE_REG_H_
#define _TSP_TEE_REG_H_

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
#define MIU_BUS                                 4
#define TSP_VQ_PITCH                            208
#define TSP_VQ_NUM                              4    //VQ0, VQ_file, VQ1, VQ_2
#define TSP_PVR_ENG_NUM                         2

#define TSP_QMEM_SIZES                          0x1000 // 16K bytes, 32bit aligment  //0x4000

//------------------------------------------------
// TS0 Bank
//------------------------------------------------
#define REG_TSP0_FW_DMA_ADDR_L                0x78
    #define TSP_FW_DMA_ADDR_MASK              0xFFFFFFUL
    #define TSP_DNLD_ADDR_ALI_SHIFT           4
#define REG_TSP1_FW_DMA_ADDR_H                0x0A
    #define TSP_FW_DMA_ADDR_H_MASK            0xFF
#define REG_TSP0_FW_DMA_NUM                   0x79

#define REG_TSP1_ONEWAY                       0x42
    #define TSP_FW_ONEWAY                     0x0008

#define REG_TSP0_PVR_HEAD1_L                  0x50
#define REG_TSP0_PVR_HEAD1_H                  0x51
#define REG_TSP0_PVR_MID1_L                   0x52
#define REG_TSP0_PVR_MID1_H                   0x53
#define REG_TSP0_PVR_TAIL1_L                  0x54
#define REG_TSP0_PVR_TAIL1_H                  0x55

#define REG_TSP0_PVR_HEAD2_L                  0x01
#define REG_TSP0_PVR_HEAD2_H                  0x02
#define REG_TSP0_PVR_MID2_L                   0x03
#define REG_TSP0_PVR_MID2_H                   0x04
#define REG_TSP0_PVR_TAIL2_L                  0x05
#define REG_TSP0_PVR_TAIL2_H                  0x06

#define REG_TSP0_PVR1_HEAD1_L                 0x12
#define REG_TSP0_PVR1_HEAD1_H                 0x13
#define REG_TSP0_PVR1_MID1_L                  0x14
#define REG_TSP0_PVR1_MID1_H                  0x15
#define REG_TSP0_PVR1_TAIL1_L                 0x16
#define REG_TSP0_PVR1_TAIL1_H                 0x17

#define REG_TSP0_PVR1_HEAD2_L                 0x18
#define REG_TSP0_PVR1_HEAD2_H                 0x19
#define REG_TSP0_PVR1_MID2_L                  0x1A
#define REG_TSP0_PVR1_MID2_H                  0x1B
#define REG_TSP0_PVR1_TAIL2_L                 0x1C
#define REG_TSP0_PVR1_TAIL2_H                 0x1D

#define REG_TSP0_FILE_ADDR_L                  0x3A
#define REG_TSP0_FILE_ADDR_H                  0x3B
#define REG_TSP0_FILE_SIZE_L                  0x3C
#define REG_TSP0_FILE_SIZE_H                  0x3D

//------------------------------------------------
// TS1 Bank
//------------------------------------------------
#define REG_TSP1_VQ0_BASE_L                   0x20
#define REG_TSP1_VQ0_BASE_H                   0x21
#define REG_TSP1_VQ0_SIZE                     0x22
#define REG_TSP1_VQ1_BASE_L                   0x56
#define REG_TSP1_VQ1_BASE_H                   0x57
#define REG_TSP1_PVR_CFG                      0x5A
    #define REG_TSP1_CH_BW_WP_LD              0x0100
#define REG_TSP1_VQ1_SIZE                     0x5C
#define REG_TSP1_VQ2_BASE_L                   0x5E
#define REG_TSP1_VQ2_BASE_H                   0x5F
#define REG_TSP1_VQ2_SIZE                     0x64
#define REG_TSP1_VQ3_BASE_L                   0x74
#define REG_TSP1_VQ3_BASE_H                   0x75
#define REG_TSP1_VQ3_SIZE                     0x76

//------------------------------------------------
// MMFI Bank
//------------------------------------------------
#define REG_MMFI_FILE_ADDR_L                  0x08
#define REG_MMFI_FILE_ADDR_H                  0x09
#define REG_MMFI_FILE_SIZE_L                  0x0A
#define REG_MMFI_FILE_SIZE_H                  0x0B

#define REG_MMFI1_FILE_ADDR_L                 0x28
#define REG_MMFI1_FILE_ADDR_H                 0x29
#define REG_MMFI1_FILE_SIZE_L                 0x2A
#define REG_MMFI1_FILE_SIZE_H                 0x2B

#endif // _TSP_TEE_REG_H_
