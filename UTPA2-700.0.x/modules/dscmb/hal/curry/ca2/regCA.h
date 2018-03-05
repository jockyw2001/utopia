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
//  File name: regCA.h
//  Description: CA Register Definition
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _REG_CA_H_
#define _REG_CA_H_


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
#define MAX_DEVICEID_SIZE 16UL


//--------------------------------------------------------------------------------------------------
//  Compliation Option
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Harware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#define REG_OTP_BASE        (0x90000UL * 2)
#define REG_OTP_CTRL_BASE   (0xA1300UL * 2)

#define REG_OTP_CTRL_0                      (REG_OTP_CTRL_BASE + 0x00 * 4)
#define REG_OTP_CTRL_0_VALUE                (0x1303003cUL)
#define REG_OTP_CTRL_PV                     (REG_OTP_CTRL_BASE + 0x08 * 4)
#define REG_OTP_CTRL_PG_VERIFY_FAIL_FLAG    (1UL << 4)

#define REG_RNG_BASE    (0xA1C00UL * 2)
#define REG_RNG_SCPU    (REG_RNG_BASE + 0x12 * 4)
#define REG_RNG_ACPU    (REG_RNG_BASE + 0x13 * 4)
#if defined (MCU_AEON)
#define REG_RNG_TRNG    (REG_RNG_SCPU)
#else
#define REG_RNG_TRNG    (REG_RNG_ACPU)
#endif
#define REG_RNG_TRNG_ACK 0x80000000UL
#define REG_RNG_TRNG_VALID_MASK 0x00010000UL
#define REG_RNG_TRNG_OUT_MASK 0x0000FFFFUL


#define REG_RSA_BASE                        (0xA1200UL * 2) //bank: 1A12
    #define REG_RSA_SEC_RANGE_SET           0x6UL
    #define REG_RSA_SEC_RANGE_ENABLE        0x00010000UL
    #define REG_RSA_SEC_RANGE_MASK          0x0000FFFFUL
    #define REG_RSA_SEC_RANGE_START(_a_)    (REG_RSA_BASE + (0x05 + (_a_)* 2) * 4 ) //0~5
    #define REG_RSA_SEC_RANGE_END(_a_)      (REG_RSA_BASE + (0x06 + (_a_)* 2) * 4 ) //0~5

//BDMA for move BGC FW to SEC51
#define REG_BDMA_BASE           (0x00900UL * 2)
    #define REG_BDMA_CTRL       (REG_BDMA_BASE + 0x0)
    #define REG_BDMA_STATUS     (REG_BDMA_BASE + 0x1*4)
    #define REG_BDMA_SRC_SEL    (REG_BDMA_BASE + 0x2*4)
    #define REG_BDMA_SRC_ADDR_L (REG_BDMA_BASE + 0x4*4)
    #define REG_BDMA_SRC_ADDR_H (REG_BDMA_BASE + 0x5*4)
    #define REG_BDMA_DST_ADDR_L (REG_BDMA_BASE + 0x6*4)
    #define REG_BDMA_DST_ADDR_H (REG_BDMA_BASE + 0x7*4)
    #define REG_BDMA_SIZE_L     (REG_BDMA_BASE + 0x8*4)
    #define REG_BDMA_SIZE_H     (REG_BDMA_BASE + 0x9*4)

//SEC51
#define REG_ONEWAY_BASE       (0x60F00UL * 2)
    #define REG_ONEWAY_42       (REG_ONEWAY_BASE + 0x42*4)
        #define SEC51_SW_RST     0x0001UL
    #define REG_ONEWAY_52       (REG_ONEWAY_BASE + 0x52*4)
        #define SEC51_SW_LOCK    0x0001UL

#define REG_MIU_CROSSBAR      (0x61300UL * 2)
    #define REG_MIU_CROSSBAR_CTRL (REG_MIU_CROSSBAR + 0x0*4)
        #define REG_MIU_CROSSBAR_EN   0x0008UL

#define REG_MBX_BASE       (0x03300UL * 2)

#define BGC_SEC_MAX          0x8UL

//define a BGC command field
#define BGC_CMD_CTRL                       (REG_MBX_BASE + 0x60*4)
#define BGC_CMD_SEC_GO                     0x0001UL //ACPU: write 1 to trigger, Sec51 write 0 to finish a command
#define BGC_CMD_SEC_STATUS                 0x0002UL //ACPU: read only, 0: cmd ok, 1:cmd fail
#define BGC_CMD_SEC_TYPE                   0x003CUL
#define BGC_CMD_SEC_TYPE_SHT               2UL
    #define BGC_CMD_TYPE_SECID             0x0UL
    #define BGC_CMD_TYPE_LENGTH            0x1UL
    #define BGC_CMD_TYPE_ADDR              0x2UL
    #define BGC_CMD_TYPE_SIZE              0x3UL
    #define BGC_CMD_TYPE_GOLDEN            0x4UL
    #define BGC_CMD_TYPE_SET_PASSWD        0x5UL
    #define BGC_CMD_TYPE_RESET_PASSWD      0x6UL
    #define BGC_CMD_TYPE_INTERVAL          0x7UL
    #define BGC_CMD_TYPE_ALGO              0x8UL
    #define BGC_SEC_ALGO_SHA256            0x0UL
    #define BGC_CMD_TYPE_MODE              0x9UL
    #define BGC_SEC_MODE_STATIC            0x1UL
    #define BGC_SEC_MODE_DYNAMIC           0x0UL
    #define BGC_CMD_TYPE_RESET_MODE        0xAUL
    #define BGC_SEC_RESET_AV               0x01UL
    #define BGC_SEC_RESET_ACPU             0x02UL
    #define BGC_SEC_RESET_SCPU             0x04UL
    #define BGC_SEC_RESET_SYSTEM           0x08UL
    #define BGC_SEC_RESET_INTERRUPT        0x10UL

    #define BGC_CMD_TYPE_ENABLE            0xFUL
    //0xB for acpu forbid
    //0xC for Read BGC_CMD_SEC_RESET_STATUS                     //RO
    //0xD for Read BGC_CMD_SEC_INTEGRITY_CHECK_STATUS  //RO
    #define BGC_CMD_SEC_DATA               0xFF00UL // DATA or Length
    #define BGC_CMD_SEC_DATA_SHT           8UL

//define a BGC response when BGC run fail
#define BGC_RESP_FAIL       (REG_MBX_BASE + 0x61*4)
    #define BGC_RESP_SECID         0x000FUL
    #define BGC_RESP_RESET_AV      0x0100UL
    #define BGC_RESP_RESET_ACPU    0x0200UL
    #define BGC_RESP_RESET_SCPU    0x0400UL
    #define BGC_RESP_RESET_SYSTEM  0x0800UL
    #define BGC_RESP_HASH_FAIL     0x8000UL

#define REG_INTR_CPUINIT_BASE      (0x00500UL * 2)//1005
    #define REG_INTR_CPUINIT_HST3      (REG_INTR_CPUINIT_BASE + 0x26 * 4)
        #define REG_HST3TO2_INT           0x0004UL

#define REG_PM_SLEEP_BASE          (0x00E00UL * 2)//000E
    #define REG_TOP_SW_RST         (REG_PM_SLEEP_BASE + 0x2E*4)
        #define REG_TOP_SW_RST_MSK         0x00FFUL
        #define REG_TOP_SW_RST_PASSWD  0x79UL

//ONEWAY PM
#define REG_PMONEWAY_BASE        (0x02500UL * 2) //0025
#define REG_STR_AUTH_RESULT      (REG_PMONEWAY_BASE + (0x00 << 2))
#define REG_STR_PTR              (REG_PMONEWAY_BASE + (0x08 << 2))
#define REG_WARM_BOOT_IND        (REG_PMONEWAY_BASE + (0x10 << 2))
    #define REG_WARM_BOOT_IND_MAGIC 0x7A2D
#define REG_STR_KEY              (REG_PMONEWAY_BASE + (0x21 << 2))
    #define REG_STR_DMA_KEY_VALID      0x02
    #define REG_STR_SPS_KEY_VALID      0x04


#endif // #ifndef __REG_CA_H_

