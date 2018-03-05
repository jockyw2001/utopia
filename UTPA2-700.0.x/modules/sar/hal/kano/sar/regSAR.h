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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
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

#ifndef _REG_SAR_H
#define _REG_SAR_H


//------------------------------------------------------------------------------
// SAR Reg
//------------------------------------------------------------------------------
// base address
#define SAR_REG_BASE                            (0x0A00UL*2) //0x1400 + (0x000000)

// register definition
#define REG_SAR_CTRL0                           ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x00UL*2))
    #define _SAR_SINGLE_CH_MSK                  (BIT2|BIT1|BIT0)//[2:0] //select channel for single channel modech0~ch7
    #define _SAR_LEVEL_TRIGGER                  (BIT3) //keypad level trigger enable. 0:  use edge trigger, 1:  use level trigger
    #define _SAR_SINGLE_CH_EN                   (BIT4) //enable single channel mode. 0: disable1: enable
    #define _SAR_MODE                           (BIT5) //select sar digital operation mode. 0: one-shot, 1: freerun
    #define _SAR_PD                             (BIT6) //sar digital power down
    #define _SAR_START                          (BIT7) //sar start signal

#define REG_SAR_CTRL1                           ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x00UL*2)+1)
    #define _SAR_ADC_PD                         (BIT0)
    #define _SAR_FREERUN                        (BIT1)
    #define _SAR_SEL                            (BIT2)
    #define _SAR_NCH_EN                         (BIT3)
    #define _SAR_LOAD_EN                        (BIT6)

#define REG_SAR_CKSAMP_PRD                      ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x01UL*2))
    #define _SAR_CKSAMP_PRD_MSK                 (0xFFUL)

#define REG_SAR_PM_DMY                          ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x10UL*2))

#define REG_SAR_CH1_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x20UL*2))
#define REG_SAR_CH1_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x30UL*2))
#define REG_SAR_CH2_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x21UL*2))
#define REG_SAR_CH2_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x31UL*2))
#define REG_SAR_CH3_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x22UL*2))
#define REG_SAR_CH3_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x32UL*2))
#define REG_SAR_CH4_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x23UL*2))
#define REG_SAR_CH4_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x33UL*2))
#define REG_SAR_CH5_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x24UL*2))
#define REG_SAR_CH5_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x34UL*2))
#define REG_SAR_CH6_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x25UL*2))
#define REG_SAR_CH6_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x35UL*2))
#define REG_SAR_CH7_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x26UL*2))
#define REG_SAR_CH7_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x36UL*2))
#define REG_SAR_CH8_UPD                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x27UL*2))
#define REG_SAR_CH8_LOB                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x37UL*2))
    #define _SAR_CHN_UPB_MSK                    (0xFFUL)
    #define _SAR_CHN_LOB_MSK                    (0xFFUL)

#define REG_SAR_ADC_CH1_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x40UL*2))
#define REG_SAR_ADC_CH2_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x41UL*2))
#define REG_SAR_ADC_CH3_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x42UL*2))
#define REG_SAR_ADC_CH4_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x43UL*2))
#define REG_SAR_ADC_CH5_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x44UL*2))
#define REG_SAR_ADC_CH6_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x45UL*2))
#define REG_SAR_ADC_CH7_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x46UL*2))
#define REG_SAR_ADC_CH8_DATA                    ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x47UL*2))
    #define _SAR_ADC_OUT_10BITMSK               (0x3FFUL)
    #define _SAR_ADC_OUT_8BITMSK                (0x3FFUL)

#define REG_SAR_AISEL                           ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x11UL*2))
    #define _SAR_AISEL                          (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_AISEL_CH1                      (BIT0)
    #define _SAR_AISEL_CH2                      (BIT1)
    #define _SAR_AISEL_CH3                      (BIT2)
    #define _SAR_AISEL_CH4                      (BIT3)
    #define _SAR_AISEL_CH5                      (BIT4)

#define REG_SAR_OEN_GPIO                        ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x11UL*2)+ 1)
    #define _SAR_OEN_GPIO_MSK                   (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_OEN_GPIO_CH1                   (BIT0)
    #define _SAR_OEN_GPIO_CH2                   (BIT1)
    #define _SAR_OEN_GPIO_CH3                   (BIT2)
    #define _SAR_OEN_GPIO_CH4                   (BIT3)
    #define _SAR_OEN_GPIO_CH5                   (BIT4)

#define REG_SAR_I_GPIO                          ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x12UL*2))
    #define _SAR_I_GPIO_MSK                     (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_I_GPIO_CH1                     (BIT0)
    #define _SAR_I_GPIO_CH2                     (BIT1)
    #define _SAR_I_GPIO_CH3                     (BIT2)
    #define _SAR_I_GPIO_CH4                     (BIT3)
    #define _SAR_I_GPIO_CH5                     (BIT4)

#define REG_SAR_C_GPIO                          ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x12UL*2)+ 1)
    #define _SAR_C_GPIO_MSK                     (BIT4|BIT3|BIT2|BIT1|BIT0)
    #define _SAR_C_GPIO_CH1                     (BIT0)
    #define _SAR_C_GPIO_CH2                     (BIT1)
    #define _SAR_C_GPIO_CH3                     (BIT2)
    #define _SAR_C_GPIO_CH4                     (BIT3)
    #define _SAR_C_GPIO_CH5                     (BIT4)

#define REG_SAR_TEST0                           ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x13UL*2))
#define REG_SAR_TEST1                           ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x13UL*2) +1)

#define REG_SAR_STATUS0                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x14UL*2))
    #define _SAR_INT_MASK                       (BIT0)
    #define _SAR_INT_CLR                        (BIT1)
    #define _SAR_INT_FORCE                      (BIT2)
    #define _SAR_INT_STATUS                     (BIT3)

#define REG_SAR_STATUS2                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x14UL*2) +1)
    #define _SAR_RDY                            (BIT7)

#define REG_SAR_INT                             ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x14UL*2))
    #define _SAR0_INT_MASK                      (BIT0)
    #define _SAR1_INT_MASK                      (BIT1)
    #define _SAR2_INT_MASK                      (BIT2)
    #define _SAR3_INT_MASK                      (BIT3)
        
#define REG_SAR_CLR_INT                         ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x15UL*2))
    #define _SAR0_CLR_INT                       (BIT0)
    #define _SAR1_CLR_INT                       (BIT1)
    #define _SAR2_CLR_INT                       (BIT2)
    #define _SAR3_CLR_INT                       (BIT3)
        
#define REG_SAR_FORCE_INT                       ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x16UL*2))
    #define _SAR0_FORCE_INT                     (BIT0)
    #define _SAR1_FORCE_INT                     (BIT1)
    #define _SAR2_FORCE_INT                     (BIT2)
    #define _SAR3_FORCE_INT                     (BIT3)
        
#define REG_SAR_INT_STATUS                      ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x17UL*2))
#define REG_SAR__REF_V_SEL                      ((0x00UL*0x10000UL)+ (SAR_REG_BASE + 0x19UL*2))
	#define _SAR0_REF_V                         (BIT0)
	#define _SAR1_REF_V                         (BIT1)
	#define _SAR2_REF_V                         (BIT2)
	#define _SAR3_REF_V                         (BIT3)
	#define _SAR4_REF_V                         (BIT4)
	#define _SAR5_REF_V                         (BIT5)
	#define _SAR6_REF_V                         (BIT6)
	#define _SAR7_REF_V                         (BIT7)
        
//specially SAR Wake Up
#define REG_PM_SLP                              (0x0e00UL)
#define REG_WK_IRQ                              (REG_PM_SLP+0x08UL*2)
    #define _W_SAR                              (BIT1)

//specially defined for atop
#define REG_ADC_ATOP                            (0x2500UL)
#define REG_ADC_ATOP_SAR2                       (REG_ADC_ATOP+0x32UL*2+1)
    #define _SAR2_HSYNC_CH0                     (BIT4)
    #define _SAR2_HSYNC_CH1                     (BIT5)
    #define _SAR2_HSYNC_CH2                     (BIT6)
    #define _SAR2_HSYNC_CH3                     (BIT7)
    #define _SAR2_HSYNC_MSK                     (BIT7|BIT6|BIT5|BIT4)

//T-Sensor
#define BIT_PM_SAR_CHANNEL8 (0x1 << 7)

#define OTP_REG_BASE                            (0x171500)
#define REG_VBE_CODE_FT                         (OTP_REG_BASE + (0x28*2))

#define REG_PM_TR                               (REG_PM_SLP+0x19*2+1)
#define REG_PM_TSENSOR_L                        (REG_PM_SLP+0x1a*2)
    #define _SEL_SAR_TSEN                       (BIT6)
    #define _SEL_TSEN_REF                       (BIT7)
#define REG_PM_TSENSOR_H                        (REG_PM_SLP+0x1a*2+1)
    #define _EN_TSEN                            (BIT0)

#define REG_SAR_ADC_REFV_SEL                    ((0x00*0x10000)+ (SAR_REG_BASE + 0x019*2))
    #define _SAR_ADC_CH1_REFV_SEL               (BIT0)
    #define _SAR_ADC_CH2_REFV_SEL               (BIT1)
    #define _SAR_ADC_CH3_REFV_SEL               (BIT2)
    #define _SAR_ADC_CH4_REFV_SEL               (BIT3)
    #define _SAR_ADC_CH5_REFV_SEL               (BIT4)
    #define _SAR_ADC_CH6_REFV_SEL               (BIT5)
    #define _SAR_ADC_CH7_REFV_SEL               (BIT6)
    #define _SAR_ADC_CH8_REFV_SEL               (BIT7)

#endif  // _REG_SAR_H

