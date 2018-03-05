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

#ifndef _REG_AVD_H_
#define _REG_AVD_H_


//-------------------------------------------------------------------------------------------------
//  Hardware Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Base address should be initial.
#if defined (__aeon__)          // Non-OS
    #define BASEADDR_RIU 0xA0000000
//#elif ( OS_TYPE == linux )    // Linux
//    #define RIU_BASE u32RegOSBase    // MDrv_MIOMap_GetBASE(u32RegOSBase, puSize, MAP_NONPM_BANK)
#else                           // ecos
    #define BASEADDR_RIU 0xBF800000
#endif

#define CLKGEN0_REG_BASE            0x0B00  // 0x1E00 - 0x1EFF
#define CHIP_REG_BASE               0x1E00  // 0x1E00 - 0x1EFF
#define ADC_ATOP_REG_BASE           0x2500  // 0x2500 - 0x25FF
#define ADC_DTOP_REG_BASE           0x2600  // 0x2600 - 0x26EF
#define AFEC_REG_BASE               0x3500  // 0x3500 - 0x35FF
#define MIIC_REG_BASE               0x3400 //0x3400 - 0x34FF
#define COMB_REG_BASE               0x3600  // 0x3600 - 0x36FF
#define VBI_REG_BASE                0x3700  // 0x3700 - 0x37FF
#define SCM_REG_BASE                0x3800  // 0x3800 - 0x38FF

#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

/* Clkgen0 */
#define L_BK_CLKGEN0(x)     BK_REG_L(CLKGEN0_REG_BASE, x)
#define H_BK_CLKGEN0(x)     BK_REG_H(CLKGEN0_REG_BASE, x)

/* Chip Top */
#define L_BK_CHIPTOP(x)     BK_REG_L(CHIP_REG_BASE, x)
#define H_BK_CHIPTOP(x)     BK_REG_H(CHIP_REG_BASE, x)

/* ADC */
#define L_BK_ADC_ATOP(x)    BK_REG_L(ADC_ATOP_REG_BASE, x)
#define H_BK_ADC_ATOP(x)    BK_REG_H(ADC_ATOP_REG_BASE, x)
#define L_BK_ADC_DTOP(x)    BK_REG_L(ADC_DTOP_REG_BASE, x)
#define H_BK_ADC_DTOP(x)    BK_REG_H(ADC_DTOP_REG_BASE, x)
// Register access utility
//#define TEMP_OR(addr, val)          TEMP_WRITE(addr, TEMP_READ(addr) | (val))
//#define TEMP_AND(addr, val)         TEMP_WRITE(addr, TEMP_READ(addr) & (val))
//#define TEMP_XOR(addr, val)         TEMP_WRITE(addr, TEMP_READ(addr) ^ (val))

///////////////////////////////////////////////////////
// AFEC bank
///////////////////////////////////////////////////////
#define BK_AFEC_01  (AFEC_REG_BASE+0x01)
#define BK_AFEC_02  (AFEC_REG_BASE+0x02)
#define BK_AFEC_03  (AFEC_REG_BASE+0x03)
#define BK_AFEC_04  (AFEC_REG_BASE+0x04)
#define BK_AFEC_05  (AFEC_REG_BASE+0x05)
#define BK_AFEC_06  (AFEC_REG_BASE+0x06)
#define BK_AFEC_07  (AFEC_REG_BASE+0x07)
#define BK_AFEC_08  (AFEC_REG_BASE+0x08)
#define BK_AFEC_09  (AFEC_REG_BASE+0x09)
#define BK_AFEC_0A  (AFEC_REG_BASE+0x0A)
#define BK_AFEC_0B  (AFEC_REG_BASE+0x0B)
#define BK_AFEC_0C  (AFEC_REG_BASE+0x0C)
#define BK_AFEC_0D  (AFEC_REG_BASE+0x0D)
#define BK_AFEC_0E  (AFEC_REG_BASE+0x0E)
#define BK_AFEC_0F  (AFEC_REG_BASE+0x0F)
#define BK_AFEC_10  (AFEC_REG_BASE+0x10)
#define BK_AFEC_11  (AFEC_REG_BASE+0x11)
#define BK_AFEC_12  (AFEC_REG_BASE+0x12)
#define BK_AFEC_13  (AFEC_REG_BASE+0x13)
#define BK_AFEC_14  (AFEC_REG_BASE+0x14)
#define BK_AFEC_15  (AFEC_REG_BASE+0x15)
#define BK_AFEC_16  (AFEC_REG_BASE+0x16)
#define BK_AFEC_17  (AFEC_REG_BASE+0x17)
#define BK_AFEC_18  (AFEC_REG_BASE+0x18)
#define BK_AFEC_19  (AFEC_REG_BASE+0x19)
#define BK_AFEC_1A  (AFEC_REG_BASE+0x1A)
#define BK_AFEC_1B  (AFEC_REG_BASE+0x1B)
#define BK_AFEC_1C  (AFEC_REG_BASE+0x1C)
#define BK_AFEC_1D  (AFEC_REG_BASE+0x1D)
#define BK_AFEC_1E  (AFEC_REG_BASE+0x1E)
#define BK_AFEC_1F  (AFEC_REG_BASE+0x1F)
#define BK_AFEC_20  (AFEC_REG_BASE+0x20)
#define BK_AFEC_21  (AFEC_REG_BASE+0x21)
#define BK_AFEC_22  (AFEC_REG_BASE+0x22)
#define BK_AFEC_23  (AFEC_REG_BASE+0x23)
#define BK_AFEC_24  (AFEC_REG_BASE+0x24)
#define BK_AFEC_25  (AFEC_REG_BASE+0x25)
#define BK_AFEC_26  (AFEC_REG_BASE+0x26)
#define BK_AFEC_27  (AFEC_REG_BASE+0x27)
#define BK_AFEC_28  (AFEC_REG_BASE+0x28)
#define BK_AFEC_29  (AFEC_REG_BASE+0x29)
#define BK_AFEC_2A  (AFEC_REG_BASE+0x2A)
#define BK_AFEC_2B  (AFEC_REG_BASE+0x2B)
#define BK_AFEC_2C  (AFEC_REG_BASE+0x2C)
#define BK_AFEC_2D  (AFEC_REG_BASE+0x2D)
#define BK_AFEC_2E  (AFEC_REG_BASE+0x2E)
#define BK_AFEC_2F  (AFEC_REG_BASE+0x2F)
#define BK_AFEC_30  (AFEC_REG_BASE+0x30)
#define BK_AFEC_31  (AFEC_REG_BASE+0x31)
#define BK_AFEC_32  (AFEC_REG_BASE+0x32)
#define BK_AFEC_33  (AFEC_REG_BASE+0x33)
#define BK_AFEC_34  (AFEC_REG_BASE+0x34)
#define BK_AFEC_35  (AFEC_REG_BASE+0x35)
#define BK_AFEC_36  (AFEC_REG_BASE+0x36)
#define BK_AFEC_37  (AFEC_REG_BASE+0x37)
#define BK_AFEC_38  (AFEC_REG_BASE+0x38)
#define BK_AFEC_39  (AFEC_REG_BASE+0x39)
#define BK_AFEC_3A  (AFEC_REG_BASE+0x3A)
#define BK_AFEC_3B  (AFEC_REG_BASE+0x3B)
#define BK_AFEC_3C  (AFEC_REG_BASE+0x3C)
#define BK_AFEC_3D  (AFEC_REG_BASE+0x3D)
#define BK_AFEC_3E  (AFEC_REG_BASE+0x3E)
#define BK_AFEC_3F  (AFEC_REG_BASE+0x3F)
#define BK_AFEC_40  (AFEC_REG_BASE+0x40)
#define BK_AFEC_41  (AFEC_REG_BASE+0x41)
#define BK_AFEC_42  (AFEC_REG_BASE+0x42)
#define BK_AFEC_43  (AFEC_REG_BASE+0x43)
#define BK_AFEC_44  (AFEC_REG_BASE+0x44)
#define BK_AFEC_45  (AFEC_REG_BASE+0x45)
#define BK_AFEC_46  (AFEC_REG_BASE+0x46)
#define BK_AFEC_47  (AFEC_REG_BASE+0x47)
#define BK_AFEC_48  (AFEC_REG_BASE+0x48)
#define BK_AFEC_49  (AFEC_REG_BASE+0x49)
#define BK_AFEC_4A  (AFEC_REG_BASE+0x4A)
#define BK_AFEC_4B  (AFEC_REG_BASE+0x4B)
#define BK_AFEC_4C  (AFEC_REG_BASE+0x4C)
#define BK_AFEC_4D  (AFEC_REG_BASE+0x4D)
#define BK_AFEC_4E  (AFEC_REG_BASE+0x4E)
#define BK_AFEC_4F  (AFEC_REG_BASE+0x4F)
#define BK_AFEC_50  (AFEC_REG_BASE+0x50)
#define BK_AFEC_51  (AFEC_REG_BASE+0x51)
#define BK_AFEC_52  (AFEC_REG_BASE+0x52)
#define BK_AFEC_53  (AFEC_REG_BASE+0x53)
#define BK_AFEC_54  (AFEC_REG_BASE+0x54)
#define BK_AFEC_55  (AFEC_REG_BASE+0x55)
#define BK_AFEC_56  (AFEC_REG_BASE+0x56)
#define BK_AFEC_57  (AFEC_REG_BASE+0x57)
#define BK_AFEC_58  (AFEC_REG_BASE+0x58)
#define BK_AFEC_59  (AFEC_REG_BASE+0x59)
#define BK_AFEC_5A  (AFEC_REG_BASE+0x5A)
#define BK_AFEC_5B  (AFEC_REG_BASE+0x5B)
#define BK_AFEC_5C  (AFEC_REG_BASE+0x5C)
#define BK_AFEC_5D  (AFEC_REG_BASE+0x5D)
#define BK_AFEC_5E  (AFEC_REG_BASE+0x5E)
#define BK_AFEC_5F  (AFEC_REG_BASE+0x5F)
#define BK_AFEC_60  (AFEC_REG_BASE+0x60)
#define BK_AFEC_61  (AFEC_REG_BASE+0x61)
#define BK_AFEC_62  (AFEC_REG_BASE+0x62)
#define BK_AFEC_63  (AFEC_REG_BASE+0x63)
#define BK_AFEC_64  (AFEC_REG_BASE+0x64)
#define BK_AFEC_65  (AFEC_REG_BASE+0x65)
#define BK_AFEC_66  (AFEC_REG_BASE+0x66)
#define BK_AFEC_67  (AFEC_REG_BASE+0x67)
#define BK_AFEC_68  (AFEC_REG_BASE+0x68)
#define BK_AFEC_69  (AFEC_REG_BASE+0x69)
#define BK_AFEC_6A  (AFEC_REG_BASE+0x6A)
#define BK_AFEC_6B  (AFEC_REG_BASE+0x6B)
#define BK_AFEC_6C  (AFEC_REG_BASE+0x6C)
#define BK_AFEC_6D  (AFEC_REG_BASE+0x6D)
#define BK_AFEC_6E  (AFEC_REG_BASE+0x6E)
#define BK_AFEC_6F  (AFEC_REG_BASE+0x6F)
#define BK_AFEC_70  (AFEC_REG_BASE+0x70)
#define BK_AFEC_71  (AFEC_REG_BASE+0x71)
#define BK_AFEC_72  (AFEC_REG_BASE+0x72)
#define BK_AFEC_73  (AFEC_REG_BASE+0x73)
#define BK_AFEC_74  (AFEC_REG_BASE+0x74)
#define BK_AFEC_75  (AFEC_REG_BASE+0x75)
#define BK_AFEC_76  (AFEC_REG_BASE+0x76)
#define BK_AFEC_77  (AFEC_REG_BASE+0x77)
#define BK_AFEC_78  (AFEC_REG_BASE+0x78)
#define BK_AFEC_79  (AFEC_REG_BASE+0x79)
#define BK_AFEC_7A  (AFEC_REG_BASE+0x7A)
#define BK_AFEC_7B  (AFEC_REG_BASE+0x7B)
#define BK_AFEC_7C  (AFEC_REG_BASE+0x7C)
#define BK_AFEC_7D  (AFEC_REG_BASE+0x7D)
#define BK_AFEC_7E  (AFEC_REG_BASE+0x7E)
#define BK_AFEC_7F  (AFEC_REG_BASE+0x7F)
#define BK_AFEC_80  (AFEC_REG_BASE+0x80)
#define BK_AFEC_81  (AFEC_REG_BASE+0x81)
#define BK_AFEC_82  (AFEC_REG_BASE+0x82)
#define BK_AFEC_83  (AFEC_REG_BASE+0x83)
#define BK_AFEC_84  (AFEC_REG_BASE+0x84)
#define BK_AFEC_85  (AFEC_REG_BASE+0x85)
#define BK_AFEC_86  (AFEC_REG_BASE+0x86)
#define BK_AFEC_87  (AFEC_REG_BASE+0x87)
#define BK_AFEC_88  (AFEC_REG_BASE+0x88)
#define BK_AFEC_89  (AFEC_REG_BASE+0x89)
#define BK_AFEC_8A  (AFEC_REG_BASE+0x8A)
#define BK_AFEC_8B  (AFEC_REG_BASE+0x8B)
#define BK_AFEC_8C  (AFEC_REG_BASE+0x8C)
#define BK_AFEC_8D  (AFEC_REG_BASE+0x8D)
#define BK_AFEC_8E  (AFEC_REG_BASE+0x8E)
#define BK_AFEC_8F  (AFEC_REG_BASE+0x8F)
#define BK_AFEC_90  (AFEC_REG_BASE+0x90)
#define BK_AFEC_91  (AFEC_REG_BASE+0x91)
#define BK_AFEC_92  (AFEC_REG_BASE+0x92)
#define BK_AFEC_93  (AFEC_REG_BASE+0x93)
#define BK_AFEC_94  (AFEC_REG_BASE+0x94)
#define BK_AFEC_95  (AFEC_REG_BASE+0x95)
#define BK_AFEC_96  (AFEC_REG_BASE+0x96)
#define BK_AFEC_97  (AFEC_REG_BASE+0x97)
#define BK_AFEC_98  (AFEC_REG_BASE+0x98)
#define BK_AFEC_99  (AFEC_REG_BASE+0x99)
#define BK_AFEC_9A  (AFEC_REG_BASE+0x9A)
#define BK_AFEC_9B  (AFEC_REG_BASE+0x9B)
#define BK_AFEC_9C  (AFEC_REG_BASE+0x9C)
#define BK_AFEC_9D  (AFEC_REG_BASE+0x9D)
#define BK_AFEC_9E  (AFEC_REG_BASE+0x9E)
#define BK_AFEC_9F  (AFEC_REG_BASE+0x9F)
#define BK_AFEC_A0  (AFEC_REG_BASE+0xA0)
#define BK_AFEC_A1  (AFEC_REG_BASE+0xA1)
#define BK_AFEC_A2  (AFEC_REG_BASE+0xA2)
#define BK_AFEC_A3  (AFEC_REG_BASE+0xA3)
#define BK_AFEC_A4  (AFEC_REG_BASE+0xA4)
#define BK_AFEC_A5  (AFEC_REG_BASE+0xA5)
#define BK_AFEC_A6  (AFEC_REG_BASE+0xA6)
#define BK_AFEC_A7  (AFEC_REG_BASE+0xA7)
#define BK_AFEC_A8  (AFEC_REG_BASE+0xA8)
#define BK_AFEC_A9  (AFEC_REG_BASE+0xA9)
#define BK_AFEC_AA  (AFEC_REG_BASE+0xAA)
#define BK_AFEC_AB  (AFEC_REG_BASE+0xAB)
#define BK_AFEC_AC  (AFEC_REG_BASE+0xAC)
#define BK_AFEC_AD  (AFEC_REG_BASE+0xAD)
#define BK_AFEC_AE  (AFEC_REG_BASE+0xAE)
#define BK_AFEC_AF  (AFEC_REG_BASE+0xAF)
#define BK_AFEC_B0  (AFEC_REG_BASE+0xB0)
#define BK_AFEC_B1  (AFEC_REG_BASE+0xB1)
#define BK_AFEC_B2  (AFEC_REG_BASE+0xB2)
#define BK_AFEC_B3  (AFEC_REG_BASE+0xB3)
#define BK_AFEC_B4  (AFEC_REG_BASE+0xB4)
#define BK_AFEC_B5  (AFEC_REG_BASE+0xB5)
#define BK_AFEC_B6  (AFEC_REG_BASE+0xB6)
#define BK_AFEC_B7  (AFEC_REG_BASE+0xB7)
#define BK_AFEC_B8  (AFEC_REG_BASE+0xB8)
#define BK_AFEC_B9  (AFEC_REG_BASE+0xB9)
#define BK_AFEC_BA  (AFEC_REG_BASE+0xBA)
#define BK_AFEC_BB  (AFEC_REG_BASE+0xBB)
#define BK_AFEC_BC  (AFEC_REG_BASE+0xBC)
#define BK_AFEC_BD  (AFEC_REG_BASE+0xBD)
#define BK_AFEC_BE  (AFEC_REG_BASE+0xBE)
#define BK_AFEC_BF  (AFEC_REG_BASE+0xBF)
#define BK_AFEC_C0  (AFEC_REG_BASE+0xC0)
#define BK_AFEC_C1  (AFEC_REG_BASE+0xC1)
#define BK_AFEC_C2  (AFEC_REG_BASE+0xC2)
#define BK_AFEC_C3  (AFEC_REG_BASE+0xC3)
#define BK_AFEC_C4  (AFEC_REG_BASE+0xC4)
#define BK_AFEC_C5  (AFEC_REG_BASE+0xC5)
#define BK_AFEC_C6  (AFEC_REG_BASE+0xC6)
#define BK_AFEC_C7  (AFEC_REG_BASE+0xC7)
#define BK_AFEC_C8  (AFEC_REG_BASE+0xC8)
#define BK_AFEC_C9  (AFEC_REG_BASE+0xC9)
#define BK_AFEC_CA  (AFEC_REG_BASE+0xCA)
#define BK_AFEC_CB  (AFEC_REG_BASE+0xCB)
#define BK_AFEC_CC  (AFEC_REG_BASE+0xCC)
#define BK_AFEC_CD  (AFEC_REG_BASE+0xCD)
#define BK_AFEC_CE  (AFEC_REG_BASE+0xCE)
#define BK_AFEC_CF  (AFEC_REG_BASE+0xCF)
#define BK_AFEC_D0  (AFEC_REG_BASE+0xD0)
#define BK_AFEC_D1  (AFEC_REG_BASE+0xD1)
#define BK_AFEC_D2  (AFEC_REG_BASE+0xD2)
#define BK_AFEC_D3  (AFEC_REG_BASE+0xD3)
#define BK_AFEC_D4  (AFEC_REG_BASE+0xD4)
#define BK_AFEC_D5  (AFEC_REG_BASE+0xD5)
#define BK_AFEC_D6  (AFEC_REG_BASE+0xD6)
#define BK_AFEC_D7  (AFEC_REG_BASE+0xD7)
#define BK_AFEC_D8  (AFEC_REG_BASE+0xD8)
#define BK_AFEC_D9  (AFEC_REG_BASE+0xD9)
#define BK_AFEC_DA  (AFEC_REG_BASE+0xDA)
#define BK_AFEC_DB  (AFEC_REG_BASE+0xDB)
#define BK_AFEC_DC  (AFEC_REG_BASE+0xDC)
#define BK_AFEC_DD  (AFEC_REG_BASE+0xDD)
#define BK_AFEC_DE  (AFEC_REG_BASE+0xDE)
#define BK_AFEC_DF  (AFEC_REG_BASE+0xDF)
#define BK_AFEC_E0  (AFEC_REG_BASE+0xE0)
#define BK_AFEC_E1  (AFEC_REG_BASE+0xE1)
#define BK_AFEC_E2  (AFEC_REG_BASE+0xE2)
#define BK_AFEC_E3  (AFEC_REG_BASE+0xE3)
#define BK_AFEC_E4  (AFEC_REG_BASE+0xE4)
#define BK_AFEC_E5  (AFEC_REG_BASE+0xE5)
#define BK_AFEC_E6  (AFEC_REG_BASE+0xE6)
#define BK_AFEC_E7  (AFEC_REG_BASE+0xE7)
#define BK_AFEC_E8  (AFEC_REG_BASE+0xE8)
#define BK_AFEC_E9  (AFEC_REG_BASE+0xE9)
#define BK_AFEC_EA  (AFEC_REG_BASE+0xEA)
#define BK_AFEC_EB  (AFEC_REG_BASE+0xEB)
#define BK_AFEC_EC  (AFEC_REG_BASE+0xEC)
#define BK_AFEC_ED  (AFEC_REG_BASE+0xED)
#define BK_AFEC_EE  (AFEC_REG_BASE+0xEE)
#define BK_AFEC_EF  (AFEC_REG_BASE+0xEF)
#define BK_AFEC_F0  (AFEC_REG_BASE+0xF0)
#define BK_AFEC_F1  (AFEC_REG_BASE+0xF1)
#define BK_AFEC_F2  (AFEC_REG_BASE+0xF2)
#define BK_AFEC_F3  (AFEC_REG_BASE+0xF3)
#define BK_AFEC_F4  (AFEC_REG_BASE+0xF4)
#define BK_AFEC_F5  (AFEC_REG_BASE+0xF5)
#define BK_AFEC_F6  (AFEC_REG_BASE+0xF6)
#define BK_AFEC_F7  (AFEC_REG_BASE+0xF7)
#define BK_AFEC_F8  (AFEC_REG_BASE+0xF8)
#define BK_AFEC_F9  (AFEC_REG_BASE+0xF9)
#define BK_AFEC_FA  (AFEC_REG_BASE+0xFA)
#define BK_AFEC_FB  (AFEC_REG_BASE+0xFB)
#define BK_AFEC_FC  (AFEC_REG_BASE+0xFC)
#define BK_AFEC_FD  (AFEC_REG_BASE+0xFD)
#define BK_AFEC_FE  (AFEC_REG_BASE+0xFE)
#define BK_AFEC_FF  (AFEC_REG_BASE+0xFF)


////////////////////////////////////////////////////////////////////////////////
// Comb filter bank
////////////////////////////////////////////////////////////////////////////////

#define BK_COMB_01  (COMB_REG_BASE+0x01)
#define BK_COMB_02  (COMB_REG_BASE+0x02)
#define BK_COMB_03  (COMB_REG_BASE+0x03)
#define BK_COMB_04  (COMB_REG_BASE+0x04)
#define BK_COMB_05  (COMB_REG_BASE+0x05)
#define BK_COMB_06  (COMB_REG_BASE+0x06)
#define BK_COMB_07  (COMB_REG_BASE+0x07)
#define BK_COMB_08  (COMB_REG_BASE+0x08)
#define BK_COMB_09  (COMB_REG_BASE+0x09)
#define BK_COMB_0A  (COMB_REG_BASE+0x0A)
#define BK_COMB_0B  (COMB_REG_BASE+0x0B)
#define BK_COMB_0C  (COMB_REG_BASE+0x0C)
#define BK_COMB_0D  (COMB_REG_BASE+0x0D)
#define BK_COMB_0E  (COMB_REG_BASE+0x0E)
#define BK_COMB_0F  (COMB_REG_BASE+0x0F)
#define BK_COMB_10  (COMB_REG_BASE+0x10)
#define BK_COMB_11  (COMB_REG_BASE+0x11)
#define BK_COMB_12  (COMB_REG_BASE+0x12)
#define BK_COMB_13  (COMB_REG_BASE+0x13)
#define BK_COMB_14  (COMB_REG_BASE+0x14)
#define BK_COMB_15  (COMB_REG_BASE+0x15)
#define BK_COMB_16  (COMB_REG_BASE+0x16)
#define BK_COMB_17  (COMB_REG_BASE+0x17)
#define BK_COMB_18  (COMB_REG_BASE+0x18)
#define BK_COMB_19  (COMB_REG_BASE+0x19)
#define BK_COMB_1A  (COMB_REG_BASE+0x1A)
#define BK_COMB_1B  (COMB_REG_BASE+0x1B)
#define BK_COMB_1C  (COMB_REG_BASE+0x1C)
#define BK_COMB_1D  (COMB_REG_BASE+0x1D)
#define BK_COMB_1E  (COMB_REG_BASE+0x1E)
#define BK_COMB_1F  (COMB_REG_BASE+0x1F)
#define BK_COMB_20  (COMB_REG_BASE+0x20)
#define BK_COMB_21  (COMB_REG_BASE+0x21)
#define BK_COMB_22  (COMB_REG_BASE+0x22)
#define BK_COMB_23  (COMB_REG_BASE+0x23)
#define BK_COMB_24  (COMB_REG_BASE+0x24)
#define BK_COMB_25  (COMB_REG_BASE+0x25)
#define BK_COMB_26  (COMB_REG_BASE+0x26)
#define BK_COMB_27  (COMB_REG_BASE+0x27)
#define BK_COMB_28  (COMB_REG_BASE+0x28)
#define BK_COMB_29  (COMB_REG_BASE+0x29)
#define BK_COMB_2A  (COMB_REG_BASE+0x2A)
#define BK_COMB_2B  (COMB_REG_BASE+0x2B)
#define BK_COMB_2C  (COMB_REG_BASE+0x2C)
#define BK_COMB_2D  (COMB_REG_BASE+0x2D)
#define BK_COMB_2E  (COMB_REG_BASE+0x2E)
#define BK_COMB_2F  (COMB_REG_BASE+0x2F)
#define BK_COMB_30  (COMB_REG_BASE+0x30)
#define BK_COMB_31  (COMB_REG_BASE+0x31)
#define BK_COMB_32  (COMB_REG_BASE+0x32)
#define BK_COMB_33  (COMB_REG_BASE+0x33)
#define BK_COMB_34  (COMB_REG_BASE+0x34)
#define BK_COMB_35  (COMB_REG_BASE+0x35)
#define BK_COMB_36  (COMB_REG_BASE+0x36)
#define BK_COMB_37  (COMB_REG_BASE+0x37)
#define BK_COMB_38  (COMB_REG_BASE+0x38)
#define BK_COMB_39  (COMB_REG_BASE+0x39)
#define BK_COMB_3A  (COMB_REG_BASE+0x3A)
#define BK_COMB_3B  (COMB_REG_BASE+0x3B)
#define BK_COMB_3C  (COMB_REG_BASE+0x3C)
#define BK_COMB_3D  (COMB_REG_BASE+0x3D)
#define BK_COMB_3E  (COMB_REG_BASE+0x3E)
#define BK_COMB_3F  (COMB_REG_BASE+0x3F)
#define BK_COMB_40  (COMB_REG_BASE+0x40)
#define BK_COMB_41  (COMB_REG_BASE+0x41)
#define BK_COMB_42  (COMB_REG_BASE+0x42)
#define BK_COMB_43  (COMB_REG_BASE+0x43)
#define BK_COMB_44  (COMB_REG_BASE+0x44)
#define BK_COMB_45  (COMB_REG_BASE+0x45)
#define BK_COMB_46  (COMB_REG_BASE+0x46)
#define BK_COMB_47  (COMB_REG_BASE+0x47)
#define BK_COMB_48  (COMB_REG_BASE+0x48)
#define BK_COMB_49  (COMB_REG_BASE+0x49)
#define BK_COMB_4A  (COMB_REG_BASE+0x4A)
#define BK_COMB_4B  (COMB_REG_BASE+0x4B)
#define BK_COMB_4C  (COMB_REG_BASE+0x4C)
#define BK_COMB_4D  (COMB_REG_BASE+0x4D)
#define BK_COMB_4E  (COMB_REG_BASE+0x4E)
#define BK_COMB_4F  (COMB_REG_BASE+0x4F)
#define BK_COMB_50  (COMB_REG_BASE+0x50)
#define BK_COMB_51  (COMB_REG_BASE+0x51)
#define BK_COMB_52  (COMB_REG_BASE+0x52)
#define BK_COMB_53  (COMB_REG_BASE+0x53)
#define BK_COMB_54  (COMB_REG_BASE+0x54)
#define BK_COMB_55  (COMB_REG_BASE+0x55)
#define BK_COMB_56  (COMB_REG_BASE+0x56)
#define BK_COMB_57  (COMB_REG_BASE+0x57)
#define BK_COMB_58  (COMB_REG_BASE+0x58)
#define BK_COMB_59  (COMB_REG_BASE+0x59)
#define BK_COMB_5A  (COMB_REG_BASE+0x5A)
#define BK_COMB_5B  (COMB_REG_BASE+0x5B)
#define BK_COMB_5C  (COMB_REG_BASE+0x5C)
#define BK_COMB_5D  (COMB_REG_BASE+0x5D)
#define BK_COMB_5E  (COMB_REG_BASE+0x5E)
#define BK_COMB_5F  (COMB_REG_BASE+0x5F)
#define BK_COMB_60  (COMB_REG_BASE+0x60)
#define BK_COMB_61  (COMB_REG_BASE+0x61)
#define BK_COMB_62  (COMB_REG_BASE+0x62)
#define BK_COMB_63  (COMB_REG_BASE+0x63)
#define BK_COMB_64  (COMB_REG_BASE+0x64)
#define BK_COMB_65  (COMB_REG_BASE+0x65)
#define BK_COMB_66  (COMB_REG_BASE+0x66)
#define BK_COMB_67  (COMB_REG_BASE+0x67)
#define BK_COMB_68  (COMB_REG_BASE+0x68)
#define BK_COMB_69  (COMB_REG_BASE+0x69)
#define BK_COMB_6A  (COMB_REG_BASE+0x6A)
#define BK_COMB_6B  (COMB_REG_BASE+0x6B)
#define BK_COMB_6C  (COMB_REG_BASE+0x6C)
#define BK_COMB_6D  (COMB_REG_BASE+0x6D)
#define BK_COMB_6E  (COMB_REG_BASE+0x6E)
#define BK_COMB_6F  (COMB_REG_BASE+0x6F)
#define BK_COMB_70  (COMB_REG_BASE+0x70)
#define BK_COMB_71  (COMB_REG_BASE+0x71)
#define BK_COMB_72  (COMB_REG_BASE+0x72)
#define BK_COMB_73  (COMB_REG_BASE+0x73)
#define BK_COMB_74  (COMB_REG_BASE+0x74)
#define BK_COMB_75  (COMB_REG_BASE+0x75)
#define BK_COMB_76  (COMB_REG_BASE+0x76)
#define BK_COMB_77  (COMB_REG_BASE+0x77)
#define BK_COMB_78  (COMB_REG_BASE+0x78)
#define BK_COMB_79  (COMB_REG_BASE+0x79)
#define BK_COMB_7A  (COMB_REG_BASE+0x7A)
#define BK_COMB_7B  (COMB_REG_BASE+0x7B)
#define BK_COMB_7C  (COMB_REG_BASE+0x7C)
#define BK_COMB_7D  (COMB_REG_BASE+0x7D)
#define BK_COMB_7E  (COMB_REG_BASE+0x7E)
#define BK_COMB_7F  (COMB_REG_BASE+0x7F)
#define BK_COMB_80  (COMB_REG_BASE+0x80)
#define BK_COMB_81  (COMB_REG_BASE+0x81)
#define BK_COMB_82  (COMB_REG_BASE+0x82)
#define BK_COMB_83  (COMB_REG_BASE+0x83)
#define BK_COMB_84  (COMB_REG_BASE+0x84)
#define BK_COMB_85  (COMB_REG_BASE+0x85)
#define BK_COMB_86  (COMB_REG_BASE+0x86)
#define BK_COMB_87  (COMB_REG_BASE+0x87)
#define BK_COMB_88  (COMB_REG_BASE+0x88)
#define BK_COMB_89  (COMB_REG_BASE+0x89)
#define BK_COMB_8A  (COMB_REG_BASE+0x8A)
#define BK_COMB_8B  (COMB_REG_BASE+0x8B)
#define BK_COMB_8C  (COMB_REG_BASE+0x8C)
#define BK_COMB_8D  (COMB_REG_BASE+0x8D)
#define BK_COMB_8E  (COMB_REG_BASE+0x8E)
#define BK_COMB_8F  (COMB_REG_BASE+0x8F)
#define BK_COMB_90  (COMB_REG_BASE+0x90)
#define BK_COMB_91  (COMB_REG_BASE+0x91)
#define BK_COMB_92  (COMB_REG_BASE+0x92)
#define BK_COMB_93  (COMB_REG_BASE+0x93)
#define BK_COMB_94  (COMB_REG_BASE+0x94)
#define BK_COMB_95  (COMB_REG_BASE+0x95)
#define BK_COMB_96  (COMB_REG_BASE+0x96)
#define BK_COMB_97  (COMB_REG_BASE+0x97)
#define BK_COMB_98  (COMB_REG_BASE+0x98)
#define BK_COMB_99  (COMB_REG_BASE+0x99)
#define BK_COMB_9A  (COMB_REG_BASE+0x9A)
#define BK_COMB_9B  (COMB_REG_BASE+0x9B)
#define BK_COMB_9C  (COMB_REG_BASE+0x9C)
#define BK_COMB_9D  (COMB_REG_BASE+0x9D)
#define BK_COMB_9E  (COMB_REG_BASE+0x9E)
#define BK_COMB_9F  (COMB_REG_BASE+0x9F)
#define BK_COMB_A0  (COMB_REG_BASE+0xA0)
#define BK_COMB_A1  (COMB_REG_BASE+0xA1)
#define BK_COMB_A2  (COMB_REG_BASE+0xA2)
#define BK_COMB_A3  (COMB_REG_BASE+0xA3)
#define BK_COMB_A4  (COMB_REG_BASE+0xA4)
#define BK_COMB_A5  (COMB_REG_BASE+0xA5)
#define BK_COMB_A6  (COMB_REG_BASE+0xA6)
#define BK_COMB_A7  (COMB_REG_BASE+0xA7)
#define BK_COMB_A8  (COMB_REG_BASE+0xA8)
#define BK_COMB_A9  (COMB_REG_BASE+0xA9)
#define BK_COMB_AA  (COMB_REG_BASE+0xAA)
#define BK_COMB_AB  (COMB_REG_BASE+0xAB)
#define BK_COMB_AC  (COMB_REG_BASE+0xAC)
#define BK_COMB_AD  (COMB_REG_BASE+0xAD)
#define BK_COMB_AE  (COMB_REG_BASE+0xAE)
#define BK_COMB_AF  (COMB_REG_BASE+0xAF)
#define BK_COMB_B0  (COMB_REG_BASE+0xB0)
#define BK_COMB_B1  (COMB_REG_BASE+0xB1)
#define BK_COMB_B2  (COMB_REG_BASE+0xB2)
#define BK_COMB_B3  (COMB_REG_BASE+0xB3)
#define BK_COMB_B4  (COMB_REG_BASE+0xB4)
#define BK_COMB_B5  (COMB_REG_BASE+0xB5)
#define BK_COMB_B6  (COMB_REG_BASE+0xB6)
#define BK_COMB_B7  (COMB_REG_BASE+0xB7)
#define BK_COMB_B8  (COMB_REG_BASE+0xB8)
#define BK_COMB_B9  (COMB_REG_BASE+0xB9)
#define BK_COMB_BA  (COMB_REG_BASE+0xBA)
#define BK_COMB_BB  (COMB_REG_BASE+0xBB)
#define BK_COMB_BC  (COMB_REG_BASE+0xBC)
#define BK_COMB_BD  (COMB_REG_BASE+0xBD)
#define BK_COMB_BE  (COMB_REG_BASE+0xBE)
#define BK_COMB_BF  (COMB_REG_BASE+0xBF)
#define BK_COMB_C0  (COMB_REG_BASE+0xC0)
#define BK_COMB_C1  (COMB_REG_BASE+0xC1)
#define BK_COMB_C2  (COMB_REG_BASE+0xC2)
#define BK_COMB_C3  (COMB_REG_BASE+0xC3)
#define BK_COMB_C4  (COMB_REG_BASE+0xC4)
#define BK_COMB_C5  (COMB_REG_BASE+0xC5)
#define BK_COMB_C6  (COMB_REG_BASE+0xC6)
#define BK_COMB_C7  (COMB_REG_BASE+0xC7)
#define BK_COMB_C8  (COMB_REG_BASE+0xC8)
#define BK_COMB_C9  (COMB_REG_BASE+0xC9)
#define BK_COMB_CA  (COMB_REG_BASE+0xCA)
#define BK_COMB_CB  (COMB_REG_BASE+0xCB)
#define BK_COMB_CC  (COMB_REG_BASE+0xCC)
#define BK_COMB_CD  (COMB_REG_BASE+0xCD)
#define BK_COMB_CE  (COMB_REG_BASE+0xCE)
#define BK_COMB_CF  (COMB_REG_BASE+0xCF)
#define BK_COMB_D0  (COMB_REG_BASE+0xD0)
#define BK_COMB_D1  (COMB_REG_BASE+0xD1)
#define BK_COMB_D2  (COMB_REG_BASE+0xD2)
#define BK_COMB_D3  (COMB_REG_BASE+0xD3)
#define BK_COMB_D4  (COMB_REG_BASE+0xD4)
#define BK_COMB_D5  (COMB_REG_BASE+0xD5)
#define BK_COMB_D6  (COMB_REG_BASE+0xD6)
#define BK_COMB_D7  (COMB_REG_BASE+0xD7)
#define BK_COMB_D8  (COMB_REG_BASE+0xD8)
#define BK_COMB_D9  (COMB_REG_BASE+0xD9)
#define BK_COMB_DA  (COMB_REG_BASE+0xDA)
#define BK_COMB_DB  (COMB_REG_BASE+0xDB)
#define BK_COMB_DC  (COMB_REG_BASE+0xDC)
#define BK_COMB_DD  (COMB_REG_BASE+0xDD)
#define BK_COMB_DE  (COMB_REG_BASE+0xDE)
#define BK_COMB_DF  (COMB_REG_BASE+0xDF)
#define BK_COMB_E0  (COMB_REG_BASE+0xE0)
#define BK_COMB_E1  (COMB_REG_BASE+0xE1)
#define BK_COMB_E2  (COMB_REG_BASE+0xE2)
#define BK_COMB_E3  (COMB_REG_BASE+0xE3)
#define BK_COMB_E4  (COMB_REG_BASE+0xE4)
#define BK_COMB_E5  (COMB_REG_BASE+0xE5)
#define BK_COMB_E6  (COMB_REG_BASE+0xE6)
#define BK_COMB_E7  (COMB_REG_BASE+0xE7)
#define BK_COMB_E8  (COMB_REG_BASE+0xE8)
#define BK_COMB_E9  (COMB_REG_BASE+0xE9)
#define BK_COMB_EA  (COMB_REG_BASE+0xEA)
#define BK_COMB_EB  (COMB_REG_BASE+0xEB)
#define BK_COMB_EC  (COMB_REG_BASE+0xEC)
#define BK_COMB_ED  (COMB_REG_BASE+0xED)
#define BK_COMB_EE  (COMB_REG_BASE+0xEE)
#define BK_COMB_EF  (COMB_REG_BASE+0xEF)
#define BK_COMB_F0  (COMB_REG_BASE+0xF0)
#define BK_COMB_F1  (COMB_REG_BASE+0xF1)
#define BK_COMB_F2  (COMB_REG_BASE+0xF2)
#define BK_COMB_F3  (COMB_REG_BASE+0xF3)
#define BK_COMB_F4  (COMB_REG_BASE+0xF4)
#define BK_COMB_F5  (COMB_REG_BASE+0xF5)
#define BK_COMB_F6  (COMB_REG_BASE+0xF6)
#define BK_COMB_F7  (COMB_REG_BASE+0xF7)
#define BK_COMB_F8  (COMB_REG_BASE+0xF8)
#define BK_COMB_F9  (COMB_REG_BASE+0xF9)
#define BK_COMB_FA  (COMB_REG_BASE+0xFA)
#define BK_COMB_FB  (COMB_REG_BASE+0xFB)
#define BK_COMB_FC  (COMB_REG_BASE+0xFC)
#define BK_COMB_FD  (COMB_REG_BASE+0xFD)
#define BK_COMB_FE  (COMB_REG_BASE+0xFE)
#define BK_COMB_FF  (COMB_REG_BASE+0xFF)


////////////////////////////////////////////////////////////////////////////////
// SECAM register
////////////////////////////////////////////////////////////////////////////////
#define BK_SECAM_01  (SCM_REG_BASE+0x01)
#define BK_SECAM_02  (SCM_REG_BASE+0x02)
#define BK_SECAM_03  (SCM_REG_BASE+0x03)
#define BK_SECAM_04  (SCM_REG_BASE+0x04)
#define BK_SECAM_05  (SCM_REG_BASE+0x05)
#define BK_SECAM_06  (SCM_REG_BASE+0x06)
#define BK_SECAM_07  (SCM_REG_BASE+0x07)
#define BK_SECAM_08  (SCM_REG_BASE+0x08)
#define BK_SECAM_09  (SCM_REG_BASE+0x09)
#define BK_SECAM_0A  (SCM_REG_BASE+0x0A)
#define BK_SECAM_0B  (SCM_REG_BASE+0x0B)
#define BK_SECAM_0C  (SCM_REG_BASE+0x0C)
#define BK_SECAM_0D  (SCM_REG_BASE+0x0D)
#define BK_SECAM_0E  (SCM_REG_BASE+0x0E)
#define BK_SECAM_0F  (SCM_REG_BASE+0x0F)
#define BK_SECAM_10  (SCM_REG_BASE+0x10)
#define BK_SECAM_11  (SCM_REG_BASE+0x11)
#define BK_SECAM_12  (SCM_REG_BASE+0x12)
#define BK_SECAM_13  (SCM_REG_BASE+0x13)
#define BK_SECAM_14  (SCM_REG_BASE+0x14)
#define BK_SECAM_15  (SCM_REG_BASE+0x15)
#define BK_SECAM_16  (SCM_REG_BASE+0x16)
#define BK_SECAM_17  (SCM_REG_BASE+0x17)
#define BK_SECAM_18  (SCM_REG_BASE+0x18)
#define BK_SECAM_19  (SCM_REG_BASE+0x19)
#define BK_SECAM_1A  (SCM_REG_BASE+0x1A)
#define BK_SECAM_1B  (SCM_REG_BASE+0x1B)
#define BK_SECAM_1C  (SCM_REG_BASE+0x1C)
#define BK_SECAM_1D  (SCM_REG_BASE+0x1D)
#define BK_SECAM_1E  (SCM_REG_BASE+0x1E)
#define BK_SECAM_1F  (SCM_REG_BASE+0x1F)
#define BK_SECAM_20  (SCM_REG_BASE+0x20)
#define BK_SECAM_21  (SCM_REG_BASE+0x21)
#define BK_SECAM_22  (SCM_REG_BASE+0x22)
#define BK_SECAM_23  (SCM_REG_BASE+0x23)
#define BK_SECAM_24  (SCM_REG_BASE+0x24)
#define BK_SECAM_25  (SCM_REG_BASE+0x25)
#define BK_SECAM_26  (SCM_REG_BASE+0x26)
#define BK_SECAM_27  (SCM_REG_BASE+0x27)
#define BK_SECAM_28  (SCM_REG_BASE+0x28)
#define BK_SECAM_29  (SCM_REG_BASE+0x29)
#define BK_SECAM_2A  (SCM_REG_BASE+0x2A)
#define BK_SECAM_2B  (SCM_REG_BASE+0x2B)
#define BK_SECAM_2C  (SCM_REG_BASE+0x2C)
#define BK_SECAM_2D  (SCM_REG_BASE+0x2D)
#define BK_SECAM_2E  (SCM_REG_BASE+0x2E)
#define BK_SECAM_2F  (SCM_REG_BASE+0x2F)
#define BK_SECAM_30  (SCM_REG_BASE+0x30)
#define BK_SECAM_31  (SCM_REG_BASE+0x31)
#define BK_SECAM_32  (SCM_REG_BASE+0x32)
#define BK_SECAM_33  (SCM_REG_BASE+0x33)
#define BK_SECAM_34  (SCM_REG_BASE+0x34)
#define BK_SECAM_35  (SCM_REG_BASE+0x35)
#define BK_SECAM_36  (SCM_REG_BASE+0x36)
#define BK_SECAM_37  (SCM_REG_BASE+0x37)
#define BK_SECAM_38  (SCM_REG_BASE+0x38)
#define BK_SECAM_39  (SCM_REG_BASE+0x39)
#define BK_SECAM_3A  (SCM_REG_BASE+0x3A)
#define BK_SECAM_3B  (SCM_REG_BASE+0x3B)
#define BK_SECAM_3C  (SCM_REG_BASE+0x3C)
#define BK_SECAM_3D  (SCM_REG_BASE+0x3D)
#define BK_SECAM_3E  (SCM_REG_BASE+0x3E)
#define BK_SECAM_3F  (SCM_REG_BASE+0x3F)

////////////////////////////////////////////////////////////////////////////////
// VBI register
////////////////////////////////////////////////////////////////////////////////
#define BK_VBI_2A  (VBI_REG_BASE+0x2A)
#define BK_VBI_41  (VBI_REG_BASE+0x41)
#define BK_VBI_45  (VBI_REG_BASE+0x45)
#define BK_VBI_46  (VBI_REG_BASE+0x46)
#define BK_VBI_4A  (VBI_REG_BASE+0x4A)
#define BK_VBI_4F  (VBI_REG_BASE+0x4F)
#define BK_VBI_50  (VBI_REG_BASE+0x50)
#define BK_VBI_51  (VBI_REG_BASE+0x51)
#define BK_VBI_55  (VBI_REG_BASE+0x55)
#define BK_VBI_56  (VBI_REG_BASE+0x56)
#define BK_VBI_57  (VBI_REG_BASE+0x57)
#define BK_VBI_58  (VBI_REG_BASE+0x58)
#define BK_VBI_59  (VBI_REG_BASE+0x59)
#define BK_VBI_5A  (VBI_REG_BASE+0x5A)
#define BK_VBI_5B  (VBI_REG_BASE+0x5B)
#define BK_VBI_5C  (VBI_REG_BASE+0x5C)
#define BK_VBI_5D  (VBI_REG_BASE+0x5D)
#define BK_VBI_5E  (VBI_REG_BASE+0x5E)
#define BK_VBI_5F  (VBI_REG_BASE+0x5F)
#define BK_VBI_70  (VBI_REG_BASE+0x70)
#define BK_VBI_71  (VBI_REG_BASE+0x71)
#define BK_VBI_72  (VBI_REG_BASE+0x72)
#define BK_VBI_77  (VBI_REG_BASE+0x77)
#define BK_VBI_7C  (VBI_REG_BASE+0x7C)
#define BK_VBI_7D  (VBI_REG_BASE+0x7D)
#define BK_VBI_7E  (VBI_REG_BASE+0x7E)
#define BK_VBI_7F  (VBI_REG_BASE+0x7F)
#define BK_VBI_81  (VBI_REG_BASE+0x81)
#define BK_VBI_82  (VBI_REG_BASE+0x82)
#define BK_VBI_83  (VBI_REG_BASE+0x83)
#define BK_VBI_86  (VBI_REG_BASE+0x86)
#define BK_VBI_89  (VBI_REG_BASE+0x89)
#define BK_VBI_8A  (VBI_REG_BASE+0x8A)
#define BK_VBI_8B  (VBI_REG_BASE+0x8B)
#define BK_VBI_8D  (VBI_REG_BASE+0x8D)
#define BK_VBI_91  (VBI_REG_BASE+0x91)
#define BK_VBI_92  (VBI_REG_BASE+0x92)
#define BK_VBI_99  (VBI_REG_BASE+0x99)
#define BK_VBI_9A  (VBI_REG_BASE+0x9A)
#define BK_VBI_AD  (VBI_REG_BASE+0xAD)
#define BK_VBI_AE  (VBI_REG_BASE+0xAE)
#define BK_VBI_AF  (VBI_REG_BASE+0xAF)
#define BK_VBI_B7  (VBI_REG_BASE+0xB7)
#define BK_VBI_B8  (VBI_REG_BASE+0xB8)
#define BK_VBI_BB  (VBI_REG_BASE+0xBB)
#define BK_VBI_C4  (VBI_REG_BASE+0xC4)
#define BK_VBI_CA  (VBI_REG_BASE+0xCA)
#define BK_VBI_CB  (VBI_REG_BASE+0xCB)
#define BK_VBI_CC  (VBI_REG_BASE+0xCC)
#define BK_VBI_CD  (VBI_REG_BASE+0xCD)
#define BK_VBI_CE  (VBI_REG_BASE+0xCE)

//------------------------------------------------------------------------------
// VD MCU control register
//------------------------------------------------------------------------------
//h0000	h0000	0	0	reg_vdmcu_rst
//h0000	h0000	8	8	reg_vdmcu_sram_en
//h0000	h0000	9	9	reg_vdmcu_cache_en
//h0000	h0000	10	10	reg_vdmcu_spi_en
//h0001	h0001	7	0	reg_vdmcu_rom_bank
//h0001	h0001	8	8	reg_addr_auto_inc
//h0001	h0001	9	9	reg_hk2vd_int
//h0001	h0001	15	12	reg_key
//h0002	h0002	15	0	reg_addr
//h0006	h0006	7	0	reg_sram_wd
//h0008	h0008	7	0	reg_sram_rd
#define VD_MCU_RST  0x3460
#define VD_MCU_SRAM_EN 0x3461
#define VD_MCU_KEY 0x3463
#define VD_MCU_ADDR_AUTO_INC 0x3463
#define VD_MCU_ADDR_L 0x3464
#define VD_MCU_ADDR_H 0x3465
#define VD_MCU_SRAM_WD 0x346C
#define VD_MCU_SRAM_RD 0x3470

#define VDMCU_MIU_MAP_CMD                       0x10C0
#define VDMCU_SDRAM_CODE_MAP                    0x10C1
#define VDMCU_SDRAM_CODE_MAP_H                  0x10C2
#define VDMCU_CACHE_ACCESS                      0x10C6
#define VDMCU_CACHE_HIT                         0x10C7
#define VDMCU_TAG_A0                            0x10C8
#define VDMCU_TAG_A1                            0x10C9
#define VDMCU_TAG_A2                            0x10CA
#define VDMCU_TAG_A3                            0x10CB
#define VDMCU_TAG_B0                            0x10CC
#define VDMCU_TAG_B1                            0x10CD
#define VDMCU_TAG_B2                            0x10CE
#define VDMCU_TAG_B3                            0x10CF

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


#endif // _REG_AVD_H_

