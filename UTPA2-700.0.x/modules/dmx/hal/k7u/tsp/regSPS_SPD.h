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
//  File name: regSPS_SPD.h
//  Description: TSP SPS / SPD Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_SPS_SPD_H_
#define _REG_SPS_SPD_H_

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

typedef struct _REG_SPS_ENG_Ctrl // SPS (Bank:0x170D)
{
    REG16       CFG_SPS_00_03[0x4 - 0x0];                                       // reg_ctr_iv_sps_init_pvr

    REG16       CFG_SPS_04;                                                     // reg_ctr_iv_sps_max_pvr
        #define CFG_SPS_04_CTR_IV_SPS_MAX_1K                                    0x0001
        #define CFG_SPS_04_CTR_IV_SPS_MAX_2K                                    0x0002
        #define CFG_SPS_04_CTR_IV_SPS_MAX_4K                                    0x0004
        #define CFG_SPS_04_CTR_IV_SPS_MAX_8K                                    0x0008
        #define CFG_SPS_04_CTR_IV_SPS_MAX_16K                                   0x0010
        #define CFG_SPS_04_CTR_IV_SPS_MAX_32K                                   0x0020
        #define CFG_SPS_04_CTR_IV_SPS_MAX_64K                                   0x0040
        #define CFG_SPS_04_CTR_IV_SPS_MAX_128K                                  0x0080

    REG16       CFG_SPS_05;
        #define CFG_SPS_05_REG_CTR_MODE_SPS_PVR                                 0x0001
        #define CFG_SPS_05_REG_UPDATE_CTR_MODE_COUNTER_IV_SPS_PVR               0x0002
        #define CFG_SPS_05_REG_LOAD_INIT_COUNTER_SPS                            0x0004
        #define CFG_SPS_05_REG_SPS_ONEWAY_CTR_MODE                              0x4000
        #define CFG_SPS_05_REG_SPS_ONEWAY_IV_MAX                                0x8000

    REG16       CFG_SPS_06_09[0xA - 0x6];                                       // reg_ctr_iv_sps_update_vld_pvr

    REG16       CFG_SPS_0A_0D[0xE - 0xA];                                       // reg_for_debug

    REG32       CFG_SPS_0E_0F;                                                  // reserved

} REG_SPS_ENG_Ctrl;

typedef struct _REG_SPD_ENG_Ctrl // SPD (Bank:0x170C)
{
    REG16       CFG_SPD_00_03[0x4 - 0x0];                                       // reg_ctr_iv_spd_init_filein

    REG16       CFG_SPD_04;                                                     // reg_ctr_iv_spd_max_filein
        #define CFG_SPD_04_CTR_IV_SPD_MAX_1K                                    0x0001
        #define CFG_SPD_04_CTR_IV_SPD_MAX_2K                                    0x0002
        #define CFG_SPD_04_CTR_IV_SPD_MAX_4K                                    0x0004
        #define CFG_SPD_04_CTR_IV_SPD_MAX_8K                                    0x0008
        #define CFG_SPD_04_CTR_IV_SPD_MAX_16K                                   0x0010
        #define CFG_SPD_04_CTR_IV_SPD_MAX_32K                                   0x0020
        #define CFG_SPD_04_CTR_IV_SPD_MAX_64K                                   0x0040
        #define CFG_SPD_04_CTR_IV_SPD_MAX_128K                                  0x0080

    REG16       CFG_SPD_05;
        #define CFG_SPD_05_REG_CTR_MODE_SPD_FILEIN                              0x0001
        #define CFG_SPD_05_REG_UPDATE_CTR_MODE_COUNTER_IV_SPD_FILEIN            0x0002
        #define CFG_SPD_05_REG_LOAD_INIT_COUNTER_SPD                            0x0004
        #define CFG_SPD_05_REG_SPD_ONEWAY_CTR_MODE                              0x4000
        #define CFG_SPD_05_REG_SPD_ONEWAY_IV_MAX                                0x8000

    REG16       CFG_SPD_06_09[0xA - 0x6];                                       // reg_ctr_iv_spd_update_vld_filein

    REG16       CFG_SPD_0A_0F[0x10 - 0xA];                                      // reserved

} REG_SPD_ENG_Ctrl;

#endif // _REG_SPS_SPD_H_
