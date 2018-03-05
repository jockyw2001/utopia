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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  File name: regAESDMA.h
//  Description: AESDMA Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RSA_REG_MCU_H_
#define _RSA_REG_MCU_H_


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
#define REG_RSACTRL_BASE            (0xA1200 * 2)

#define RSA_E_BASE_ADDR             (0x00)
#define RSA_N_BASE_ADDR             (0x40)
#define RSA_A_BASE_ADDR             (0x80)
#define RSA_Z_BASE_ADDR             (0xC0)  /// 256-64

#define RSA_MAX_SECURE_RANGE_SIZE   (0x6)

typedef struct _REG32
{
    volatile MS_U32                u32Reg;
} REG32;

typedef struct _REG_SecureRange
{
	REG32 Rsa_Sec_Range_Start;
	REG32 Rsa_Sec_Range_End;

}REG_SecureRange;


typedef struct _REG_RSACtrl
{
    REG32               Rsa_Ind32_Start;                        //(REG_X32_RSA_BASE + 4 * 0)
        #define RSA_EXP_START                       0x00000001  ///exp_start
        #define RSA_INDIRECT_START                  0x00000002  /// ram start
        #define RSA_IND32_CTRL_DIRECTION_WRITE      0x00000004  /// ram_wdata
        #define RSA_INT_CLR                         0x00000008  /// int_clr
        #define RSA_BLOCK_RSA_KEY                   0x00000010  /// block_rsa_key
        #define RSA_ROOTKEY_SEL_PUB_NO              0x00000020  /// if public key, 0: select 1st key; 1 select 2nd key
        #define RSA_ROOTKEY_SEL_TYPE                0x00000040  /// 1: OTP RSA key is public; 0: OTP RSA is private
        //#define RSA_ROOTKEY_SEL_MASK                0x00000060 /// rootkey_sel:2
    REG32               Rsa_Dummy;
    REG32               Rsa_Ctrl;                               //(REG_X32_RSA_BASE + 4 * 2)
        #define RSA_CTRL_RSA_RST                    0x00000001  //RSA_SW_RESET_t sw_reset:1;
        #define RSA_CTRL_SEL_HW_KEY                 0x00000002  //RSA_HW_KEY_t hw_key:1;
        #define RSA_CTRL_SEL_PUBLIC_KEY             0x00000004  //RSA_PUB_KEY_t pub_key:1;
        #define RSA_CTRL_NMI                        0x00000010  ///nmi:1;
        #define RSA_CTRL_NLEN_ADJ                   0x00000020  ///nlen_adj:1
        #define RSA_CTRL_SRAM_EN                    0x00000080  ///sram_en:1
        #define RSA_CTRL_KEY_LENGTH_MASK            0x00003F00  ///nlen:6
        #define RSA_RAM_DIR                         0x00020000  ///RAM_DIR_t
        #define RSA_IND32_CTRL_ADDR_AUTO_INC        0x00040000
        #define RSA_IND32_CTRL_ACCESS_AUTO_START    0x00080000
        #define RSA_RAM_MSB_FIRST                   0x00100000
    REG32               Rsa_Ind32_Addr;                         //(REG_X32_RSA_BASE + 4 * 3)  REG_RSA_RAM_ADDR, REG_RSA_STATUS
        #define RSA_STATUS_RSA_BUSY                 0x01000000
        #define RSA_STATUS_MASK                     0x01000000
    REG32               Rsa_Ind32_Data;                         //(REG_X32_RSA_BASE + 4 * 4)          REG_RSA_RAM_DATA

    REG_SecureRange Rsa_Sec_Range[RSA_MAX_SECURE_RANGE_SIZE];
        #define REG_RSA_SEC_RANGE_MASK              0xFFFF0000
        #define REG_RSA_SEC_RANGE_SHIFT              16

    REG32               Rsa_CA_Spare_Oneway0;                   //(REG_X32_RSA_BASE + 4 * 11)
    REG32               Rsa_CA_Spare_Oneway1;                   //(REG_X32_RSA_BASE + 4 * 12)
    REG32               Rsa_Miu_Address_Mask;                   //(REG_X32_RSA_BASE + 4 * 13)
    REG32               Rsa_Xiu_Id;                             //(REG_X32_RSA_BASE + 4 * 14)
    REG32               Rsa_Release_Access;                     //(REG_X32_RSA_BASE + 4 * 15)
    REG32               Rsa_Hwkey_Switch;                       //(REG_X32_RSA_BASE + 4 * 16)
        #define RSA_READ_HWKEY_SWITCH               0x00000001
        #define RSA_HWKEY_SWITCH                    0x00000100

}REG_RSACtrl;


#endif // #ifndef _RSA_REG_MCU_H_
