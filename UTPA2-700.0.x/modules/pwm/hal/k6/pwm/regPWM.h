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

#ifndef _REG_PWM_H_
#define _REG_PWM_H_

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & and data type
////////////////////////////////////////////////////////////////////////////////
#define REG_ALL_PAD_IN              0x1EA1 //bit 7;set all pads (except SPI) as input
#define REG_PWM_IS_GPIO             0x1EA2 //bit 0~3
#define REG_PWM_OEN                 0x1E06 //bit 0~4

#define REG_PM_BASE                 (0x1C00)
#define REG_PWM_BASE                0x3200

#define REG_PWM_GRP0_CLK_GATE_EN    (REG_PWM_BASE + 0x01*2)  //bit0
#define REG_PWM_GRP1_CLK_GATE_EN    (REG_PWM_BASE + 0x01*2)  //bit1
#define REG_PWM_GRP2_CLK_GATE_EN    (REG_PWM_BASE + 0x01*2)  //bit2

#define REG_PWM0_PERIOD             (REG_PWM_BASE + 0x02*2)  //bit0~15
#define REG_PWM1_PERIOD             (REG_PWM_BASE + 0x05*2)  //bit0~15
#define REG_PWM2_PERIOD             (REG_PWM_BASE + 0x08*2)  //bit0~15
#define REG_PWM3_PERIOD             (REG_PWM_BASE + 0x0B*2)  //bit0~15
#define REG_PWM4_PERIOD             (REG_PWM_BASE + 0x0E*2)  //bit0~15
#define REG_PWM5_PERIOD             (REG_PWM_BASE + 0x11*2)  //bit0~15
#define REG_PWM6_PERIOD             (REG_PWM_BASE + 0x40*2)  //bit0~15
#define REG_PWM7_PERIOD             (REG_PWM_BASE + 0x43*2)  //bit0~15
#define REG_PWM8_PERIOD             (REG_PWM_BASE + 0x46*2)  //bit0~15

#define REG_PWM0_DUTY               (REG_PWM_BASE + 0x03*2)  //bit0~15
#define REG_PWM1_DUTY               (REG_PWM_BASE + 0x06*2)  //bit0~15
#define REG_PWM2_DUTY               (REG_PWM_BASE + 0x09*2)  //bit0~15
#define REG_PWM3_DUTY               (REG_PWM_BASE + 0x0C*2)  //bit0~15
#define REG_PWM4_DUTY               (REG_PWM_BASE + 0x0F*2)  //bit0~15
#define REG_PWM5_DUTY               (REG_PWM_BASE + 0x12*2)  //bit0~15
#define REG_PWM6_DUTY               (REG_PWM_BASE + 0x41*2)  //bit0~15
#define REG_PWM7_DUTY               (REG_PWM_BASE + 0x44*2)  //bit0~15
#define REG_PWM8_DUTY               (REG_PWM_BASE + 0x47*2)  //bit0~15

#define REG_PWM0_DIV                (REG_PWM_BASE + 0x04*2)  //bit0~7
#define REG_PWM1_DIV                (REG_PWM_BASE + 0x07*2)  //bit0~7
#define REG_PWM2_DIV                (REG_PWM_BASE + 0x0A*2)  //bit0~7
#define REG_PWM3_DIV                (REG_PWM_BASE + 0x0D*2)  //bit0~7
#define REG_PWM4_DIV                (REG_PWM_BASE + 0x10*2)  //bit0~7
#define REG_PWM5_DIV                (REG_PWM_BASE + 0x13*2)  //bit0~7
#define REG_PWM6_DIV                (REG_PWM_BASE + 0x42*2)  //bit0~7
#define REG_PWM7_DIV                (REG_PWM_BASE + 0x45*2)  //bit0~7
#define REG_PWM8_DIV                (REG_PWM_BASE + 0x48*2)  //bit0~7

#define REG_PWM0_PORARITY           (REG_PWM_BASE + 0x04*2)  //bit8
#define REG_PWM1_PORARITY           (REG_PWM_BASE + 0x07*2)  //bit8
#define REG_PWM2_PORARITY           (REG_PWM_BASE + 0x0A*2)  //bit8
#define REG_PWM3_PORARITY           (REG_PWM_BASE + 0x0D*2)  //bit8
#define REG_PWM4_PORARITY           (REG_PWM_BASE + 0x10*2)  //bit8
#define REG_PWM5_PORARITY           (REG_PWM_BASE + 0x13*2)  //bit8
#define REG_PWM6_PORARITY           (REG_PWM_BASE + 0x42*2)  //bit8
#define REG_PWM7_PORARITY           (REG_PWM_BASE + 0x45*2)  //bit8
#define REG_PWM8_PORARITY           (REG_PWM_BASE + 0x48*2)  //bit8

#define REG_PWM0_VDBEN              (REG_PWM_BASE + 0x04*2)  //bit9
#define REG_PWM1_VDBEN              (REG_PWM_BASE + 0x07*2)  //bit9
#define REG_PWM2_VDBEN              (REG_PWM_BASE + 0x0A*2)  //bit9
#define REG_PWM3_VDBEN              (REG_PWM_BASE + 0x0D*2)  //bit9
#define REG_PWM4_VDBEN              (REG_PWM_BASE + 0x10*2)  //bit9
#define REG_PWM5_VDBEN              (REG_PWM_BASE + 0x13*2)  //bit9
#define REG_PWM6_VDBEN              (REG_PWM_BASE + 0x42*2)  //bit9
#define REG_PWM7_VDBEN              (REG_PWM_BASE + 0x45*2)  //bit9
#define REG_PWM8_VDBEN              (REG_PWM_BASE + 0x48*2)  //bit9

#define REG_PWM0_RESET_EN           (REG_PWM_BASE + 0x04*2)  //bit10
#define REG_PWM1_RESET_EN           (REG_PWM_BASE + 0x07*2)  //bit10
#define REG_PWM2_RESET_EN           (REG_PWM_BASE + 0x0A*2)  //bit10
#define REG_PWM3_RESET_EN           (REG_PWM_BASE + 0x0D*2)  //bit10
#define REG_PWM4_RESET_EN           (REG_PWM_BASE + 0x10*2)  //bit10
#define REG_PWM5_RESET_EN           (REG_PWM_BASE + 0x13*2)  //bit10
#define REG_PWM6_RESET_EN           (REG_PWM_BASE + 0x42*2)  //bit10
#define REG_PWM7_RESET_EN           (REG_PWM_BASE + 0x45*2)  //bit10
#define REG_PWM8_RESET_EN           (REG_PWM_BASE + 0x48*2)  //bit10

#define REG_PWM0_DBEN               (REG_PWM_BASE + 0x04*2)  //bit11
#define REG_PWM1_DBEN               (REG_PWM_BASE + 0x07*2)  //bit11
#define REG_PWM2_DBEN               (REG_PWM_BASE + 0x0A*2)  //bit11
#define REG_PWM3_DBEN               (REG_PWM_BASE + 0x0D*2)  //bit11
#define REG_PWM4_DBEN               (REG_PWM_BASE + 0x10*2)  //bit11
#define REG_PWM5_DBEN               (REG_PWM_BASE + 0x13*2)  //bit11
#define REG_PWM6_DBEN               (REG_PWM_BASE + 0x42*2)  //bit11
#define REG_PWM7_DBEN               (REG_PWM_BASE + 0x45*2)  //bit11
#define REG_PWM8_DBEN               (REG_PWM_BASE + 0x48*2)  //bit11

#define REG_PWM0_VDBEN_SW           (0x04)  //bit14
#define REG_PWM1_VDBEN_SW           (0x07)  //bit14
#define REG_PWM2_VDBEN_SW           (0x0A)  //bit14
#define REG_PWM3_VDBEN_SW           (0x0D)  //bit14
#define REG_PWM4_VDBEN_SW           (0x10)  //bit14

#define REG_PWM0_IMPULSE_EN         (0x04)  //bit12
#define REG_PWM1_IMPULSE_EN         (0x07)  //bit12
#define REG_PWM2_IMPULSE_EN         (0x0A)  //bit12
#define REG_PWM3_IMPULSE_EN         (0x0D)  //bit12
#define REG_PWM4_IMPULSE_EN         (0x10)  //bit12
#define REG_PWM5_IMPULSE_EN         (0x13)  //bit12
#define REG_PWM6_IMPULSE_EN         (0x42)  //bit12
#define REG_PWM7_IMPULSE_EN         (0x45)  //bit12
#define REG_PWM8_IMPULSE_EN         (0x48)  //bit12

#define REG_PWM0_ODDEVEN_SYNC       (0x04)  //bit13
#define REG_PWM1_ODDEVEN_SYNC       (0x07)  //bit13
#define REG_PWM2_ODDEVEN_SYNC       (0x0A)  //bit13
#define REG_PWM3_ODDEVEN_SYNC       (0x0D)  //bit13
#define REG_PWM4_ODDEVEN_SYNC       (0x10)  //bit13
#define REG_PWM5_ODDEVEN_SYNC       (0x13)  //bit13
#define REG_PWM6_ODDEVEN_SYNC       (0x42)  //bit13
#define REG_PWM7_ODDEVEN_SYNC       (0x45)  //bit13
#define REG_PWM8_ODDEVEN_SYNC       (0x48)  //bit13

/* If chiptop provides the related reg, please use them at chiptop reg. */
#define REG_PWM0_OEN                (REG_PWM_BASE + 0x04*2)  //bit15
#define REG_PWM1_OEN                (REG_PWM_BASE + 0x07*2)  //bit15
#define REG_PWM2_OEN                (REG_PWM_BASE + 0x0a*2)  //bit15
#define REG_PWM3_OEN                (REG_PWM_BASE + 0x0d*2)  //bit15
#define REG_PWM4_OEN                (REG_PWM_BASE + 0x10*2)  //bit15
#define REG_PWM5_OEN                (REG_PWM_BASE + 0x13*2)  //bit15
#define REG_PWM6_OEN                (REG_PWM_BASE + 0x42*2)  //bit15
#define REG_PWM7_OEN                (REG_PWM_BASE + 0x45*2)  //bit15
#define REG_PWM8_OEN                (REG_PWM_BASE + 0x48*2)  //bit15

#define REG_RST_MUX0                (REG_PWM_BASE + 0x14*2)  //bit15
#define REG_RST_MUX1                (REG_PWM_BASE + 0x14*2)  //bit7
#define REG_RST_MUX2                (REG_PWM_BASE + 0x15*2)  //bit15
#define REG_RST_MUX3                (REG_PWM_BASE + 0x15*2)  //bit7
#define REG_RST_MUX4                (REG_PWM_BASE + 0x16*2)  //bit15
#define REG_RST_MUX5                (REG_PWM_BASE + 0x16*2)  //bit7
#define REG_RST_MUX6                (REG_PWM_BASE + 0x49*2)  //bit15
#define REG_RST_MUX7                (REG_PWM_BASE + 0x49*2)  //bit7
#define REG_RST_MUX8                (REG_PWM_BASE + 0x4A*2)  //bit15

#define REG_HS_RST_CNT0             (REG_PWM_BASE + 0x14*2)  //bit8~11
#define REG_HS_RST_CNT1             (REG_PWM_BASE + 0x14*2)  //bit0~3
#define REG_HS_RST_CNT2             (REG_PWM_BASE + 0x15*2)  //bit8~11
#define REG_HS_RST_CNT3             (REG_PWM_BASE + 0x15*2)  //bit0~3
#define REG_HS_RST_CNT4             (REG_PWM_BASE + 0x16*2)  //bit8~11
#define REG_HS_RST_CNT5             (REG_PWM_BASE + 0x16*2)  //bit0~3
#define REG_HS_RST_CNT6             (REG_PWM_BASE + 0x49*2)  //bit8~11
#define REG_HS_RST_CNT7             (REG_PWM_BASE + 0x49*2)  //bit0~3
#define REG_HS_RST_CNT8             (REG_PWM_BASE + 0x4A*2)  //bit8~11

#define REG_PWM0_PERIOD_EXT         (REG_PWM_BASE + 0x20*2)  //bit0~1
#define REG_PWM1_PERIOD_EXT         (REG_PWM_BASE + 0x20*2)  //bit2~3
#define REG_PWM2_PERIOD_EXT         (REG_PWM_BASE + 0x20*2)  //bit4~5
#define REG_PWM3_PERIOD_EXT         (REG_PWM_BASE + 0x20*2)  //bit6~7
#define REG_PWM4_PERIOD_EXT         (REG_PWM_BASE + 0x20*2)  //bit8~9
#define REG_PWM5_PERIOD_EXT         (REG_PWM_BASE + 0x20*2)  //bit10~11
#define REG_PWM6_PERIOD_EXT         (REG_PWM_BASE + 0x4B*2)  //bit0~1
#define REG_PWM7_PERIOD_EXT         (REG_PWM_BASE + 0x4B*2)  //bit2~3
#define REG_PWM8_PERIOD_EXT         (REG_PWM_BASE + 0x4B*2)  //bit4~5

#define REG_PWM0_DUTY_EXT           (REG_PWM_BASE + 0x21*2)  //bit0~1
#define REG_PWM1_DUTY_EXT           (REG_PWM_BASE + 0x21*2)  //bit2~3
#define REG_PWM2_DUTY_EXT           (REG_PWM_BASE + 0x21*2)  //bit4~5
#define REG_PWM3_DUTY_EXT           (REG_PWM_BASE + 0x21*2)  //bit6~7
#define REG_PWM4_DUTY_EXT           (REG_PWM_BASE + 0x21*2)  //bit8~9
#define REG_PWM5_DUTY_EXT           (REG_PWM_BASE + 0x21*2)  //bit10~11
#define REG_PWM6_DUTY_EXT           (REG_PWM_BASE + 0x4B*2)  //bit8~9
#define REG_PWM7_DUTY_EXT           (REG_PWM_BASE + 0x4B*2)  //bit10~11
#define REG_PWM8_DUTY_EXT           (REG_PWM_BASE + 0x4B*2)  //bit12~13

#define REG_PWM0_DIV_EXT            (REG_PWM_BASE + 0x22*2)  //bit0~7
#define REG_PWM1_DIV_EXT            (REG_PWM_BASE + 0x22*2)  //bit8~15
#define REG_PWM2_DIV_EXT            (REG_PWM_BASE + 0x23*2)  //bit0~7
#define REG_PWM3_DIV_EXT            (REG_PWM_BASE + 0x23*2)  //bit8~15
#define REG_PWM4_DIV_EXT            (REG_PWM_BASE + 0x24*2)  //bit0~7
#define REG_PWM5_DIV_EXT            (REG_PWM_BASE + 0x24*2)  //bit8~15
#define REG_PWM6_DIV_EXT            (REG_PWM_BASE + 0x4C*2)  //bit0~7
#define REG_PWM7_DIV_EXT            (REG_PWM_BASE + 0x4C*2)  //bit8~15
#define REG_PWM8_DIV_EXT            (REG_PWM_BASE + 0x4D*2)  //bit0~7

#define REG_PWM0_SHIFT_L            (REG_PWM_BASE + 0x28*2)  //bit0~15
#define REG_PWM0_SHIFT_H            (REG_PWM_BASE + 0x29*2)  //bit0~1
#define REG_PWM1_SHIFT_L            (REG_PWM_BASE + 0x2A*2)  //bit0~15
#define REG_PWM1_SHIFT_H            (REG_PWM_BASE + 0x2B*2)  //bit0~1
#define REG_PWM2_SHIFT_L            (REG_PWM_BASE + 0x2C*2)  //bit0~15
#define REG_PWM2_SHIFT_H            (REG_PWM_BASE + 0x2D*2)  //bit0~1
#define REG_PWM3_SHIFT_L            (REG_PWM_BASE + 0x2E*2)  //bit0~15
#define REG_PWM3_SHIFT_H            (REG_PWM_BASE + 0x2F*2)  //bit0~1
#define REG_PWM4_SHIFT_L            (REG_PWM_BASE + 0x30*2)  //bit0~15
#define REG_PWM4_SHIFT_H            (REG_PWM_BASE + 0x31*2)  //bit0~1
#define REG_PWM5_SHIFT_L            (REG_PWM_BASE + 0x32*2)  //bit0~15
#define REG_PWM5_SHIFT_H            (REG_PWM_BASE + 0x33*2)  //bit0~1
#define REG_PWM6_SHIFT_L            (REG_PWM_BASE + 0x4E*2)  //bit0~15
#define REG_PWM6_SHIFT_H            (REG_PWM_BASE + 0x4F*2)  //bit0~1
#define REG_PWM7_SHIFT_L            (REG_PWM_BASE + 0x50*2)  //bit0~15
#define REG_PWM7_SHIFT_H            (REG_PWM_BASE + 0x51*2)  //bit0~1
#define REG_PWM8_SHIFT_L            (REG_PWM_BASE + 0x52*2)  //bit0~15
#define REG_PWM8_SHIFT_H            (REG_PWM_BASE + 0x53*2)  //bit0~1

//For Debug Use
#define REG_PWM_DUMMY0              (REG_PWM_BASE + 0x34*2)  //bit0~15
#define REG_PWM_DUMMY1              (REG_PWM_BASE + 0x35*2)  //bit0~15
#define REG_PWM_DUMMY2              (REG_PWM_BASE + 0x36*2)  //bit0~15
#define REG_PWM_DUMMY3              (REG_PWM_BASE + 0x37*2)  //bit0~15

#define REG_INV_3D_FLAG             (REG_PWM_BASE + 0x78*2+1)  //bit15, inverse 3D flag

//For PWM 3D Mode
#define REG_PWM_MULTI_DIFF          (REG_PWM_BASE + 0x38*2)  //bit0~15
#define PWM_MULTI_DIEF_EN		BIT(3)
#define REG_PWM0_HIT_CNT_ST         (REG_PWM_BASE + 0x54*2)  //bit0~15 //3D PMW_01 shift
#define REG_PWM0_HIT_CNT_END        (REG_PWM_BASE + 0x55*2)  //bit0~15 //3D PWM_01 duty
#define REG_PWM1_HIT_CNT_ST         (REG_PWM_BASE + 0x56*2)  //bit0~15 //3D PWM_11 shift
#define REG_PWM1_HIT_CNT_END        (REG_PWM_BASE + 0x57*2)  //bit0~15 //3D PWM_11 duty
#define REG_PWM0_HIT_CNT_ST2        (REG_PWM_BASE + 0x68*2)  //bit0~15 //3D PMW_02 shift
#define REG_PWM0_HIT_CNT_END2       (REG_PWM_BASE + 0x69*2)  //bit0~15 //3D PWM_02 duty
#define REG_PWM1_HIT_CNT_ST2        (REG_PWM_BASE + 0x6a*2)  //bit0~15 //3D PWM_12 shift
#define REG_PWM1_HIT_CNT_END2       (REG_PWM_BASE + 0x6b*2)  //bit0~15 //3D PWM_12 duty
#define REG_PWM0_SHIFT4             (REG_PWM_BASE + 0x50*2)  //bit0~15 //3D PMW_03 shift
#define REG_PWM0_DUTY4              (REG_PWM_BASE + 0x51*2)  //bit0~15 //3D PWM_03 duty
#define REG_PWM1_SHIFT4             (REG_PWM_BASE + 0x52*2)  //bit0~15 //3D PMW_13 shift
#define REG_PWM1_DUTY4              (REG_PWM_BASE + 0x53*2)  //bit0~15 //3D PWM_13 duty

#endif // _REG_PWM_H_

