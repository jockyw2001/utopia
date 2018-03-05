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

#ifndef _REG_PWM_H_
#define _REG_PWM_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & and data type
////////////////////////////////////////////////////////////////////////////////
#define REG_ALL_PAD_IN              (0x50) //bit 15;set all pads (except SPI) as input
#define REG_PWM_OEN                 (0x03) //bit 0~4

#define REG_PWM_MODE				(0x51)
#define PAD_PWM0_OUT                (BIT0)
#define PAD_PWM1_OUT                (BIT1)
#define PAD_PWM2_OUT                (BIT2)
#define PAD_PWM3_OUT                (BIT3)
#define PAD_PWM4_OUT                (BIT4)

#define REG_PWM1_MODE				(0x55)
#define REG_PWM5_MODE				(0x59)
#define PAD_PWM5_OUT                (BIT0)

#define REG_PM_BASE                 (0x1C00)
#define REG_TOP_BASE                (0x3C00)
#define REG_PWM_BASE                (0x6400)

#define REG_PWM0_PERIOD             (0x02)  //bit0~15
#define REG_PWM1_PERIOD             (0x05)  //bit0~15
#define REG_PWM2_PERIOD             (0x08)  //bit0~15
#define REG_PWM3_PERIOD             (0x0B)  //bit0~15
#define REG_PWM4_PERIOD             (0x0E)  //bit0~15
#define REG_PWM5_PERIOD             (0x11)  //bit0~15
//#define REG_PWM6_PERIOD           (0x40)  //bit0~15
//#define REG_PWM7_PERIOD           (0x43)  //bit0~15
//#define REG_PWM8_PERIOD           (0x46)  //bit0~15

#define REG_PWM0_DUTY               (0x03)  //bit0~15
#define REG_PWM1_DUTY               (0x06)  //bit0~15
#define REG_PWM2_DUTY               (0x09)  //bit0~15
#define REG_PWM3_DUTY               (0x0C)  //bit0~15
#define REG_PWM4_DUTY               (0x0F)  //bit0~15
#define REG_PWM5_DUTY               (0x12)  //bit0~15
//#define REG_PWM6_DUTY             (0x41)  //bit0~15
//#define REG_PWM7_DUTY             (0x44)  //bit0~15
//#define REG_PWM8_DUTY             (0x47)  //bit0~15

#define REG_PWM0_DIV                (0x04)  //bit0~7
#define REG_PWM1_DIV                (0x07)  //bit0~7
#define REG_PWM2_DIV                (0x0A)  //bit0~7
#define REG_PWM3_DIV                (0x0D)  //bit0~7
#define REG_PWM4_DIV                (0x10)  //bit0~7
#define REG_PWM5_DIV                (0x13)  //bit0~7
//#define REG_PWM6_DIV              (0x42)  //bit0~7
//#define REG_PWM7_DIV              (0x45)  //bit0~7
//#define REG_PWM8_DIV              (0x48)  //bit0~7

#define REG_PWM0_PORARITY           (0x04)  //bit8
#define REG_PWM1_PORARITY           (0x07)  //bit8
#define REG_PWM2_PORARITY           (0x0A)  //bit8
#define REG_PWM3_PORARITY           (0x0D)  //bit8
#define REG_PWM4_PORARITY           (0x10)  //bit8
#define REG_PWM5_PORARITY           (0x13)  //bit8
//#define REG_PWM6_PORARITY         (0x42)  //bit8
//#define REG_PWM7_PORARITY         (0x45)  //bit8
//#define REG_PWM8_PORARITY         (0x48)  //bit8

#define REG_PWM0_VDBEN              (0x04)  //bit9
#define REG_PWM1_VDBEN              (0x07)  //bit9
#define REG_PWM2_VDBEN              (0x0A)  //bit9
#define REG_PWM3_VDBEN              (0x0D)  //bit9
#define REG_PWM4_VDBEN              (0x10)  //bit9
#define REG_PWM5_VDBEN              (0x13)  //bit9
//#define REG_PWM6_VDBEN            (0x42)  //bit9
//#define REG_PWM7_VDBEN            (0x45)  //bit9
//#define REG_PWM8_VDBEN            (0x48)  //bit9

#define REG_PWM0_RESET_EN           (0x04)  //bit10
#define REG_PWM1_RESET_EN           (0x07)  //bit10
#define REG_PWM2_RESET_EN           (0x0A)  //bit10
#define REG_PWM3_RESET_EN           (0x0D)  //bit10
#define REG_PWM4_RESET_EN           (0x10)  //bit10
#define REG_PWM5_RESET_EN           (0x13)  //bit10
//#define REG_PWM6_RESET_EN         (0x42)  //bit10
//#define REG_PWM7_RESET_EN         (0x45)  //bit10
//#define REG_PWM8_RESET_EN         (0x48)  //bit10

#define REG_PWM0_DBEN               (0x04)  //bit11
#define REG_PWM1_DBEN               (0x07)  //bit11
#define REG_PWM2_DBEN               (0x0A)  //bit11
#define REG_PWM3_DBEN               (0x0D)  //bit11
#define REG_PWM4_DBEN               (0x10)  //bit11
#define REG_PWM5_DBEN               (0x13)  //bit11
//#define REG_PWM6_DBEN             (0x42)  //bit11
//#define REG_PWM7_DBEN             (0x45)  //bit11
//#define REG_PWM8_DBEN             (0x48)  //bit11

#define REG_PWM0_IMPULSE_EN         (0x04)  //bit12
#define REG_PWM1_IMPULSE_EN         (0x07)  //bit12
#define REG_PWM2_IMPULSE_EN         (0x0A)  //bit12
#define REG_PWM3_IMPULSE_EN         (0x0D)  //bit12
#define REG_PWM4_IMPULSE_EN         (0x10)  //bit12
#define REG_PWM5_IMPULSE_EN         (0x13)  //bit12
//#define REG_PWM6_IMPULSE_EN       (0x42)  //bit12
//#define REG_PWM7_IMPULSE_EN       (0x45)  //bit12
//#define REG_PWM8_IMPULSE_EN       (0x48)  //bit12

#define REG_PWM0_ODDEVEN_SYNC       (0x04)  //bit13
#define REG_PWM1_ODDEVEN_SYNC       (0x07)  //bit13
#define REG_PWM2_ODDEVEN_SYNC       (0x0A)  //bit13
#define REG_PWM3_ODDEVEN_SYNC       (0x0D)  //bit13
#define REG_PWM4_ODDEVEN_SYNC       (0x10)  //bit13
#define REG_PWM5_ODDEVEN_SYNC       (0x13)  //bit13
//#define REG_PWM6_ODDEVEN_SYNC     (0x42)  //bit13
//#define REG_PWM7_ODDEVEN_SYNC     (0x45)  //bit13
//#define REG_PWM8_ODDEVEN_SYNC     (0x48)  //bit13

#define REG_PWM0_VDBEN_SW           (0x04)  //bit14
#define REG_PWM1_VDBEN_SW           (0x07)  //bit14
#define REG_PWM2_VDBEN_SW           (0x0A)  //bit14
#define REG_PWM3_VDBEN_SW           (0x0D)  //bit14
#define REG_PWM4_VDBEN_SW           (0x10)  //bit14
#define REG_PWM5_VDBEN_SW           (0x13)  //bit14

/* If chiptop provides the related reg, please use them at chiptop reg. */
//#define REG_PWM0_OEN              (0x04)  //bit15
//#define REG_PWM1_OEN              (0x07)  //bit15
//#define REG_PWM2_OEN              (0x0a)  //bit15
//#define REG_PWM3_OEN              (0x0d)  //bit15
//#define REG_PWM4_OEN              (0x10)  //bit15
//#define REG_PWM5_OEN              (0x13)  //bit15
//#define REG_PWM6_OEN              (0x42)  //bit15
//#define REG_PWM7_OEN              (0x45)  //bit15
//#define REG_PWM8_OEN              (0x48)  //bit15

#define REG_RST_MUX0                (0x14)  //bit15
#define REG_RST_MUX1                (0x14)  //bit7
#define REG_RST_MUX2                (0x15)  //bit15
#define REG_RST_MUX3                (0x15)  //bit7
#define REG_RST_MUX4                (0x16)  //bit15
#define REG_RST_MUX5                (0x16)  //bit7
//#define REG_RST_MUX6              (0x49)  //bit15
//#define REG_RST_MUX7              (0x49)  //bit7
//#define REG_RST_MUX8              (0x4A)  //bit15

#define REG_HS_RST_CNT0             (0x14)  //bit8~11
#define REG_HS_RST_CNT1             (0x14)  //bit0~3
#define REG_HS_RST_CNT2             (0x15)  //bit8~11
#define REG_HS_RST_CNT3             (0x15)  //bit0~3
#define REG_HS_RST_CNT4             (0x16)  //bit8~11
#define REG_HS_RST_CNT5             (0x16)  //bit0~3
//#define REG_HS_RST_CNT6           (0x49)  //bit8~11
//#define REG_HS_RST_CNT7           (0x49)  //bit0~3
//#define REG_HS_RST_CNT8           (0x4A)  //bit8~11

#define REG_PWM0_PERIOD_EXT         (0x20)  //bit0~1
#define REG_PWM1_PERIOD_EXT         (0x20)  //bit2~3
#define REG_PWM2_PERIOD_EXT         (0x20)  //bit4~5
#define REG_PWM3_PERIOD_EXT         (0x20)  //bit6~7
#define REG_PWM4_PERIOD_EXT         (0x20)  //bit8~9
#define REG_PWM5_PERIOD_EXT         (0x20)  //bit10~11
//#define REG_PWM6_PERIOD_EXT       (0x4B)  //bit0~1
//#define REG_PWM7_PERIOD_EXT       (0x4B)  //bit2~3
//#define REG_PWM8_PERIOD_EXT       (0x4B)  //bit4~5

#define REG_PWM0_DUTY_EXT           (0x21)  //bit0~1
#define REG_PWM1_DUTY_EXT           (0x21)  //bit2~3
#define REG_PWM2_DUTY_EXT           (0x21)  //bit4~5
#define REG_PWM3_DUTY_EXT           (0x21)  //bit6~7
#define REG_PWM4_DUTY_EXT           (0x21)  //bit8~9
#define REG_PWM5_DUTY_EXT           (0x21)  //bit10~11
//#define REG_PWM6_DUTY_EXT         (0x4B)  //bit8~9
//#define REG_PWM7_DUTY_EXT         (0x4B)  //bit10~11
//#define REG_PWM8_DUTY_EXT         (0x4B)  //bit12~13

#define REG_PWM0_DIV_EXT            (0x22)  //bit0~7
#define REG_PWM1_DIV_EXT            (0x22)  //bit8~15
#define REG_PWM2_DIV_EXT            (0x23)  //bit0~7
#define REG_PWM3_DIV_EXT            (0x23)  //bit8~15
#define REG_PWM4_DIV_EXT            (0x24)  //bit0~7
#define REG_PWM5_DIV_EXT            (0x24)  //bit8~15
//#define REG_PWM6_DIV_EXT          (0x4C)  //bit0~7
//#define REG_PWM7_DIV_EXT          (0x4C)  //bit8~15
//#define REG_PWM8_DIV_EXT          (0x4D)  //bit0~7

#define REG_PWM0_SHIFT_L            (0x28)  //bit0~15
#define REG_PWM0_SHIFT_H            (0x29)  //bit0~1
#define REG_PWM1_SHIFT_L            (0x2A)  //bit0~15
#define REG_PWM1_SHIFT_H            (0x2B)  //bit0~1
#define REG_PWM2_SHIFT_L            (0x2C)  //bit0~15
#define REG_PWM2_SHIFT_H            (0x2D)  //bit0~1
#define REG_PWM3_SHIFT_L            (0x2E)  //bit0~15
#define REG_PWM3_SHIFT_H            (0x2F)  //bit0~1
#define REG_PWM4_SHIFT_L            (0x30)  //bit0~15
#define REG_PWM4_SHIFT_H            (0x31)  //bit0~1
#define REG_PWM5_SHIFT_L            (0x32)  //bit0~15
#define REG_PWM5_SHIFT_H            (0x33)  //bit0~1
//#define REG_PWM6_SHIFT_L          (0x4E)  //bit0~15
//#define REG_PWM6_SHIFT_H          (0x4F)  //bit0~1
//#define REG_PWM7_SHIFT_L          (0x50)  //bit0~15
//#define REG_PWM7_SHIFT_H          (0x51)  //bit0~1
//#define REG_PWM8_SHIFT_L          (0x52)  //bit0~15
//#define REG_PWM8_SHIFT_H          (0x53)  //bit0~1

#define REG_PWM0_NVS                (0x34)  //bit0
#define REG_PWM1_NVS                (0x34)  //bit1
#define REG_PWM2_NVS                (0x34)  //bit2
#define REG_PWM3_NVS                (0x34)  //bit3
#define REG_PWM4_NVS                (0x34)  //bit4
#define REG_PWM5_NVS                (0x34)  //bit5

#define REG_PWM0_Align              (0x35)  //bit0
#define REG_PWM1_Align              (0x35)  //bit1
#define REG_PWM2_Align              (0x35)  //bit2
#define REG_PWM3_Align              (0x35)  //bit3
#define REG_PWM4_Align              (0x35)  //bit4
#define REG_PWM5_Align              (0x35)  //bit5

#define reg_pwm_as_chip_config      (0x70)  //bit0
#define REG_PM_PWM0_IS_GPIO         (0x1C)  //bit5
#define REG_PM_PWM0_PERIOD          (0x6A)  //bit0~15
#define REG_PM_PWM0_DUTY            (0x69)  //bit0~15
#define REG_PM_PWM0_DIV             (0x68)  //bit0~7
#define REG_PM_PWM0_PORARITY        (0x6B)  //bit0
#define REG_PM_PWM0_DBEN            (0x6B)  //bit1

//For Debug Use
#define REG_PWM_DUMMY2              (0x36)  //bit0~15 Default:0x0000
#define REG_PWM_DUMMY3              (0x37)  //bit0~15 Default:0xFFFF
#define REG_INV_3D_FLAG             (0x78)  //bit15, inverse 3D flag


//For PWM 3D Mode
#define REG_PWM_MULTI_DIFF          (0x38)  //bit0~15
#define PWM_MULTI_DIEF_EN	          BIT(3)
#define REG_PWM0_HIT_CNT_ST         (0x54)  //bit0~15 //3D PMW_01 shift
#define REG_PWM0_HIT_CNT_END        (0x55)  //bit0~15 //3D PWM_01 duty
#define REG_PWM1_HIT_CNT_ST         (0x56)  //bit0~15 //3D PWM_11 shift
#define REG_PWM1_HIT_CNT_END        (0x57)  //bit0~15 //3D PWM_11 duty
#define REG_PWM0_HIT_CNT_ST2        (0x68)  //bit0~15 //3D PMW_02 shift
#define REG_PWM0_HIT_CNT_END2       (0x69)  //bit0~15 //3D PWM_02 duty
#define REG_PWM1_HIT_CNT_ST2        (0x6a)  //bit0~15 //3D PWM_12 shift
#define REG_PWM1_HIT_CNT_END2       (0x6b)  //bit0~15 //3D PWM_12 duty
#define REG_PWM0_SHIFT4             (0x50)  //bit0~15 //3D PMW_03 shift
#define REG_PWM0_DUTY4              (0x51)  //bit0~15 //3D PWM_03 duty
#define REG_PWM1_SHIFT4             (0x52)  //bit0~15 //3D PMW_13 shift
#define REG_PWM1_DUTY4              (0x53)  //bit0~15 //3D PWM_13 duty

#endif // _REG_PWM_H_

