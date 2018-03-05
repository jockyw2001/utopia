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
////////////////////////////////////////////////////////////////////////////////

#ifndef _REGNSK2_H_
#define _REGNSK2_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
#define __BIT(x)    ((MS_U32)(1 << (x)))
#define __BIT0       __BIT(0)
#define __BIT1       __BIT(1)
#define __BIT2       __BIT(2)
#define __BIT3       __BIT(3)
#define __BIT4       __BIT(4)
#define __BIT5       __BIT(5)
#define __BIT6       __BIT(6)
#define __BIT7       __BIT(7)
#define __BIT8       __BIT(8)
#define __BIT9       __BIT(9)
#define __BITA       __BIT(0xA)
#define __BITB       __BIT(0xB)
#define __BITC       __BIT(0xC)
#define __BITD       __BIT(0xD)
#define __BITE       __BIT(0xE)
#define __BITF       __BIT(0xF)

#define __BIT21       __BIT(21)
#define __BIT31     __BIT(31)

//#define BMASK(_bits_)                   (__BIT(((1)?_bits_)+1)-__BIT(((0)?_bits_)))

#define BYTE0        0xFF
#define BYTE1        (0xFF<<8)
#define BYTE0_MASK   0x00FF
#define BYTE1_MASK   0xFF00


#define REG_CLKGEN0_BASE        (0x100B00<<1)
#define REG_NSK2_BASE           (0x180000<<1)
#define REG_OTP_BASE            (0x190000<<1)
#define REG_OTP_CTRL_BASE       (0x1A1300<<1)
#define REG_NI_BASE             (0x1A1800<<1)
#define REG_RSA_BASE            (0x1A1200<<1)
#define REG_MPROT_BASE          (0x1A1D00<<1)

#define REG_KEY_TABLE_BASE      (0x1A0C00<<1)
#define REG_JTAG_PWD_BASE       (0x170300<<1)	//check!!
#define REG_CIPHER_CH0_BASE     (0x1A0700<<1)

#define REG_CRYPTO_DMA_BASE     (0x1A0B00<<1)	//x32_CryptoDMA0


#define REG_NSK2_ACPU_CMD       0xFC00
    #define NSK2_ACPU_CMD           0xFF

#define REG_NSK2_ACPU_WARNING   0xFC04
    #define NSK2_ACPU_BUSY          __BIT31

#define REG_NSK2_ACPU_INT       0xFC08
    #define NSK2_INT_CMD_EXIT           __BIT0
    #define NSK2_INT_ASYNC_EVENT        __BIT1
    #define NSK2_INT_ILLEGAL_CMD        __BIT2
    #define NSK2_INT_ILLEGAL_ACCESS     __BIT3
    #define NSK2_INT_RESET              __BIT4
    #define NSK2_INT_HANG               __BIT5
    #define NSK2_INT_KTE_VALID          __BIT6
    #define NSK2_INT_MASK_CLEAR         __BIT31


#define REG_NSK2_ACPU_CTRL_BLOCK    0xFC0C


#define REG_NSK2_ACPU_ERROR     0xFC10

#define REG_NSK2_KTE_SWITCH_1   0xFC40
#define REG_NSK2_KTE_SWITCH_2   0xFC44
#define REG_NSK2_KTE_SWITCH_3   0xFC48

#define REG_NSK2_KTE_VALID      0xFC4C
    #define NSK2_KTE_VALID_TRUE         __BIT0

#define REG_NSK2_GENOUT_LOW     0xFC50

#define REG_NSK2_VEN_VERSION    0xFC70

#define REG_NSK2_GENOUT_HIGH    0xFDFC


#define REG_NI_NSK2_CTRL                0x0
    #define NI_TS2NSK_ENABLE                0x1
    #define NI_TS2NSK_RATE                  BMASK(3:1)
    #define NI_TEST_RC_FREQ_MAP             BMASK(7:4)
    #define NI_N2ROM_PD                     __BIT8
    #define NI_NO_RST_DELAY                 __BITD
    #define NI_NSK2_CLK_ENABLE              __BITE
    #define NI_NSK2_RESET_DISABLE           __BITF

#define REG_NI_NSK2_FREERUN             0x1
    #define NI_NSK2_FREERUN_ENABLE          __BIT2
    #define NI_NSK2_RANDOM_FREERUN          __BIT4
    #define NI_NSK2_RANDOM_ONEBYONE         __BIT5

#define REG_NI_SW_SET_RNG               0x5
    #define NI_SW_RNG_MASK                  BMASK(15:0)

#define REG_NI_COMMAND                  0x6
    #define NI_COMMAND_START                0x1

    #define NI_KIW_OID_MASK                 BMASK(3:3)
    #define NI_KIW_OID_SHIFT                3

    #define NI_KIW_CMD_MASK                 BMASK(15:8)
    #define NI_KIW_CMD_SHIFT                8
    #define NI_NopNop                       0x0

    #define NI_OTP_ACK_NSK2                 (1<<1)
    #define NI_ERR_INVALID_SLOT             (1<<2)
    #define NI_NSKBIComplete                (1<<NI_KIW_CMD_SHIFT)
    #define NI_DriveAck                     (2<<NI_KIW_CMD_SHIFT)
    #define NI_InvalidateCmChannel          (4<<NI_KIW_CMD_SHIFT)
    #define NI_InvalidateM2M                (5<<NI_KIW_CMD_SHIFT)
    #define NI_ConfigureCmChannel           (6<<NI_KIW_CMD_SHIFT)
    #define NI_WriteTransportKey            (8<<NI_KIW_CMD_SHIFT)
    #define NI_WriteM2MKey                  (9<<NI_KIW_CMD_SHIFT)
    #define NI_WriteMDEMKey                 (10<<NI_KIW_CMD_SHIFT)
    #define NI_WriteJTAGKey                 (12<<NI_KIW_CMD_SHIFT)
    #define NI_WriteSCPUKey                 (13<<NI_KIW_CMD_SHIFT)
    #define NI_WriteReservedKey             (14<<NI_KIW_CMD_SHIFT)
    #define NI_WriteRandomValue             (15<<NI_KIW_CMD_SHIFT)
    #define NI_WriteOTPKey                  (16<<NI_KIW_CMD_SHIFT)
    #define NI_IncrementNvCounter           (17<<NI_KIW_CMD_SHIFT)


    #define NI_KIW_PidNo_MASK               BMASK(23:16)
    #define NI_WriteTKey_PidNo_Shift        16
    #define NI_WriteTransportKey_PidNo      BMASK(23:16)
    #define NI_WriteTKey_SCB_Shift          24
    #define NI_WriteTKey_SCB_MASK           BMASK(25:24)
    #define NI_WriteTKey_FSCB_Shift         26
    #define NI_WriteTKey_FSCB_MASK          BMASK(27:26)

    #define NI_WriteESA                     0x2
    #define NI_JTAG_PasswordWriteSCPUKey    0x8
    #define NI_CearWKResp                   0xC


#define REG_NI_DSCMB_ALGO               0x7

    #define NI_KIW_LSAD_ALGO_MASK           BMASK(4:0)
    #define NI_KIW_ESA_ALGO_SHIFT           8
    #define NI_KIW_ESA_ALGO_MASK            BMASK(12:8)
    #define NI_KIW_LSAS_ALGO_SHIFT          16
    #define NI_KIW_LSAS_ALGO_MASK           BMASK(20:16)

    #define NI_WriteESA_PidNo               BMASK(7:0)
    #define NI_WriteESA_ESASubSel_Shift     16
    #define NI_WriteESA_ESASubSel_MASK      BMASK(18:16)
    #define NI_WriteESA_ESASel_Shift        19
    #define NI_WriteESA_ESASel_MASK         BMASK(22:19)

    #define NI_WriteM2MKey_Shift            16
    #define NI_WriteM2MKey_MASK             BMASK(19:16)

#define REG_NI_IV_31_00                 0x8
#define REG_NI_IV_63_31                 0x9
#define REG_NI_IV_95_64                 0xA
#define REG_NI_IV_127_96                0xB

#define REG_NI_STATUS                   0xC
    #define NI_NSK_BUSY                     __BIT0
    #define NI_SKB_BUSY                     __BIT1
    #define NI_AIW_BUSY                     __BIT2
    #define NI_KIW_BUSY                     __BIT3

#define REG_NI_KTE_STATUS               0xD
    #define NI_KTE_DEST_MASK                BMASK(2:0)
    #define NI_KTE_VALID                    __BIT3
    #define NI_NSK2_FREQ_OKAY               __BIT4
    #define NI_SLOW_CLOCK_DETECT            __BIT5
    #define NI_CLOCK_HALT_DETECT            __BIT6

#define REG_NI_COMPARE_GENOUT_L         0x17
    #define NSK2_IS_BUSY                    __BIT(21)
    #define NSK2_KTE_DEST                   BMASK(20:17)
    #define NSK2_CON_CUR                    __BIT(16)
    #define NSK2_CSA2VAR_EN                 __BIT(15)
    #define NSK2_CSA3VAR_EN                 __BIT(14)
    #define NSK2_CPNR                       __BIT(1)   //Content Protection Not Required
    #define NSK2_OTPPWD                     __BIT(0)   //OTP Program Password Accepted

#define REG_NI_COMPARE_GENOUT_H         0x18
    #define NI_GENOUT_H_MASK                BMASK(5:0)

#define REG_NI_NSK2_TRNG_VALID          0x19
    #define NI_NSK2_TRNG_VALID_MASK         BMASK(0:0)

#define REG_NI_NSK2_PWD_ON              0x1B
    #define N2_JTAGPWD0_ON                  __BIT(0)
    #define N2_JTAGPWD1_ON                  __BIT(1)
    #define N2_SCANPWD_ON                   __BIT(2)
    #define N2_MBISTPWD_ON                  __BIT(3)
    #define N2_M2MKEY_ON                    __BIT(4)
    #define N2_SCPUKEY0_ON                  __BIT(5)
    #define N2_SCPUKEY1_ON                  __BIT(6)
    #define N2_RNGVALUE0_ON                 __BIT(8)
    #define N2_RNGVALUE1_ON                 __BIT(9)


#define REG_NI_NSK2_TRNG_DATA           0x1A

#define REG_NI_NSK2_KTE_RESP            0x1C


#define REG_NI_NSK2_CLK_CSA             0x24
    #define NSK2_PUSH_SLOW_CLK               __BIT(0)
    #define NSK2_EN_CSA_VAR                  __BIT(4)

#define REG_KT_KEYS_START_FPGA          0x28
#define REG_KT_KEYS_END_FPGA            0x36


#define REG_NI_NSK2_KTE_VALID_FPGA      0x37
    #define NI_NSK2_KTE_VALID_FPGA          __BIT0

#define REG_NI_NSK21_GENIN              0x40

#define REG_NI_NSK21_CONCURR_PROT_EN    0x41

#define REG_NI_NSK21_CONCURR_SET        0x42

#define REG_NI_NSK21_GEN_SHOT           0x43

#define REG_NI_NSK2_REG_GENIN           0x44

#define REG_NI_NSK21_GET_NVCOUNTER      0x78

#define REG_RSA_CLK_ENABLE              0x1
    #define RSA_PM_NSKCLK_ENABLE            __BIT1

	//TEMP for secure range <14, Aug>
#define REG_MPROT_SECRANGE_START(_a_)	(REG_MPROT_BASE + ((_a_ * 2) + 0x0) * 4 ) //0~5
#define REG_MPROT_SECRANGE_END(_a_)		(REG_MPROT_BASE + ((_a_ * 2) + 0x1) * 4 ) //0~5
#define REG_MPROT_SECRANGE_SET			0x6
#define REG_MPROT_SECRANGE_ENABLE       0xFFFEFFFF
    #define MPROT_SECRANGE_MASK			0x0000FFFF
#define REG_MPROT_SECRANGE_MASK		    (REG_MPROT_BASE + (0x10 << 2))

#endif //_REGNSK2_H_

