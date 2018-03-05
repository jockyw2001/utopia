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
//  File name: regSTC.h
//  Description: TSP STC/PCR Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_STC_H_
#define _REG_STC_H_

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


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct _REG_STC_64
{
    REG32       STC_L;
    REG32       STC_H;
} REG_STC_64;

typedef struct _REG_STC_ENG_Ctrl // STC (Bank:0x300E)
{
    // reg_pcr64_0_riu @ tsp0 bank h'7 ~ h'A
    // reg_pcr64_1_riu @ tsp1 bank h'10 ~ h'13
    REG_STC_64  CFG_STC_00_17[6];                                               // reg_pcr64_2_riu ~ reg_pcr64_7_riu

    REG16       CFG_STC_18;                                                     // reserved

    // @NOTE: STC0_CONFIG & STC1_CONFIG is controlled by TSP HW ,
    //        no need riu fields...
    REG32       CFG_STC_19_1A;
        #define CFG_STC_19_1A_REG_SET_BASE_STC                                  0x00000001
        #define CFG_STC_19_1A_REG_STC_ENABLE                                    0x00000002
        #define CFG_STC_19_1A_REG_STC_LD                                        0x00000004

} REG_STC_ENG_Ctrl;

// PCR (Bank:0x300E)
// [Eng    , Offset]
// HW PCR0 , h'20 ~ h'27
// HW PCR1 , h'28 ~ h'2F
// HW PCR2 , h'30 ~ h'38
// HW PCR3 , h'38 ~ h'3f
// HW PCR4 , h'40 ~ h'48
// HW PCR5 , h'48 ~ h'4F
// HW PCR6 , h'50 ~ h'58
// HW PCR7 , h'58 ~ h'5F
typedef struct _REG_PCR_ENG_Ctrl
{
    REG16       CFG_PCR_00;
        #define CFG_PCR_00_REG_PCR_SRC_MASK                                     0x000F
        #define CFG_PCR_00_REG_PCR_SRC_SHIFT                                    0
        #define CFG_PCR_00_REG_TEI_SKIP_PKT_PCR                                 0x0010
        #define CFG_PCR_00_REG_PCR_RESET                                        0x0020
        #define CFG_PCR_00_REG_PCR_READ                                         0x0040
        #define CFG_PCR_00_REG_DSS_ENABLE                                       0x0080
        #define CFG_PCR_00_REG_SKIP_PVR_RUSH_DATA                               0x0100
        #define CFG_PCR_00_REG_PCR_CONFIG_MASK                                  0xFE00
        #define CFG_PCR_00_REG_PCR_CONFIG_SHIFT                                 9

    REG32       CFG_PCR_01_02;                                                  // reg_pidflt_pcr
        #define CFG_PCR_01_02_REG_PIDFLT_PCR_PID_MASK                           0x00001FFF
        #define CFG_PCR_01_02_REG_PIDFLT_PCR_PID_SHIFT                          0
        #define CFG_PCR_01_02_REG_PIDFLT_PCR_ENPCR                              0x00008000
        #define CFG_PCR_01_02_REG_PIDFLT_PCR_SOURCE_MASK                        0x003F0000
        #define CFG_PCR_01_02_REG_PIDFLT_PCR_SOURCE_SHIFT                       16

    REG32       CFG_PCR_03_04;                                                  // reg_pcr_valid ([31:0])

    REG16       CFG_PCR_05;                                                     // reg_pcr_valid ([32]) , reg_pcr_valid_ext
        #define CFG_PCR_05_REG_PCR_VALID_BIT32_MASK                             0x0001
        #define CFG_PCR_05_REG_PCR_VALID_EXT_MASK                               0x03FE
        #define CFG_PCR_05_REG_PCR_VALID_EXT_SHIFT                              1

    REG32       CFG_PCR_06_07;                                                  // reserved

} REG_PCR_ENG_Ctrl;

#endif // _REG_STC_H_
