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

#ifndef CEC_ANALOG_REG_H
#define CEC_ANALOG_REG_H

#include "cec_hwreg.h"
/******************************************************************************/
/*                     Macro                                                  */
/******************************************************************************/

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

///////////////////////////////////////////////////////
// Scaler Bank
///////////////////////////////////////////////////////

#define REG_BANK_RESET              0x00UL
#define REG_BANK_GOPINT             0x00UL
#define REG_BANK_IP1F2              0x01UL
#define REG_BANK_IP2F2              0x02UL
#define REG_BANK_IP1F1              0x03UL
#define REG_BANK_IP2F1              0x04UL
#define REG_BANK_OPM                0x05UL
#define REG_BANK_DNR                0x06UL
#define REG_BANK_OP1                0x07UL
#define REG_BANK_OP1PIP             0x08UL
#define REG_BANK_OP1HVSP            0x09UL
#define REG_BANK_FILM               0x0AUL    //T2
#define REG_BANK_ELA                0x0BUL
#define REG_BANK_ACE                0x0CUL
#define REG_BANK_HD                 0x0EUL
#define REG_BANK_S_VOP              0x0FUL
#define REG_BANK_VOP                0x10UL
#define REG_BANK_TCON               0x11UL
#define REG_BANK_SCMI               0x12UL  //T2
#define REG_BANK_OD                 0x16UL
#define REG_BANK_SRAM               0x17UL
#define REG_BANK_VIP                0x18UL
#define REG_BANK_VPS                0x19UL
#define REG_BANK_DLC                0x1AUL
#define REG_BANK_OP1PIPEXT          0x1BUL
#define REG_BANK_SmoothFilter       0x1CUL
#define REG_BANK_MFC                0x1DUL
#define REG_BANK_PIP                0x20UL
#define REG_BANK_EODI               0x21UL
#define REG_BANK_MADI               0x22UL
#define REG_BANK_HVSP               0x23UL
#define REG_BANK_DMS                0x26UL
#define REG_BANK_PRED               0x28UL

#define BK_SELECT_00                REG_SCALER_BASE

#define HIPART( u32x )              (((u32x) >> 16) & 0xFFFF)
#define LOPART( u32x )              ((u32x) & 0xFFFF)

#define BK_REG_L( x, y )            ((x) | (((y) << 1)))
#define BK_REG_H( x, y )            (((x) | (((y) << 1))) + 1)

/* Scaler */
#define L_BK_GOPINT(x)      BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_GOPINT(x)      BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_SWRESET(x)     BK_REG_L(REG_SCALER_BASE,x)
#define L_BK_IP1F2(x)       BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_IP1F2(x)       BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_IP2F2(x)       BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_IP2F2(x)       BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_IP1F1(x)       BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_IP1F1(x)       BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_IP2F1(x)       BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_IP2F1(x)       BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_OPM(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_OPM(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_DNR(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_DNR(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_OP1(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_OP1(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_OP1HVSP(x)     BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_OP1HVSP(x)     BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_ELA(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_ELA(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_ACE(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_ACE(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_HD(x)          BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_HD(x)          BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_S_VOP(x)       BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_S_VOP(x)       BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_VOP(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_VOP(x)         BK_REG_H(REG_SCALER_BASE,x)

#define L_BK_SCMI(x)        BK_REG_L(REG_SCALER_BASE,x)      //SUPPORT_CHIP==T2
#define H_BK_SCMI(x)        BK_REG_H(REG_SCALER_BASE,x)      //SUPPORT_CHIP==T2
#define L_BK_OD(x)          BK_REG_L(REG_SCALER_BASE,x)      //SUPPORT_CHIP==T2
#define H_BK_OD(x)          BK_REG_H(REG_SCALER_BASE,x)      //SUPPORT_CHIP==T2

#define L_BK_OP1ZZ(x)       BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_OP1ZZ(x)       BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_VIP(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_VIP(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_VPS(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_VPS(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_DLC(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_DLC(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_OP1PIPEXT(x)   BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_OP1PIPEXT(x)   BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_SmoothFilter(x)  BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_SmoothFilter(x)  BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_MFC(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_MFC(x)         BK_REG_H(REG_SCALER_BASE,x)

#define L_BK_EODI(x)        BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_EODI(x)        BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_MADI(x)        BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_MADI(x)        BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_PIP(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_PIP(x)         BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_HVSP(x)        BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_HVSP(x)        BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_PRED(x)        BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_PRED(x)        BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_FILM(x)        BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_FILM(x)        BK_REG_H(REG_SCALER_BASE,x)
#define L_BK_DMS(x)         BK_REG_L(REG_SCALER_BASE,x)
#define H_BK_DMS(x)         BK_REG_H(REG_SCALER_BASE,x)

/* VE */
#define L_BK_VE_ENC(x)      BK_REG_L(REG_BASE_VE_ENCODER, x)
#define H_BK_VE_ENC(x)      BK_REG_H(REG_BASE_VE_ENCODER, x)
#define L_BK_VE_SRC(x)      BK_REG_L(REG_BASE_VE_SOURCE, x)
#define H_BK_VE_SRC(x)      BK_REG_H(REG_BASE_VE_SOURCE, x)


/* LPLL */
#define L_BK_LPLL(x)        BK_REG_L(REG_LPLL_BASE, x)
#define H_BK_LPLL(x)        BK_REG_H(REG_LPLL_BASE, x)

/* IP Mux */
#define L_BK_IPMUX(x)       BK_REG_L(REG_IPMUX_BASE, x)
#define H_BK_IPMUX(x)       BK_REG_H(REG_IPMUX_BASE, x)

/* Chip Top */
#define L_BK_CHIPTOP(x)     BK_REG_L(REG_CHIPTOP_BASE, x)
#define H_BK_CHIPTOP(x)     BK_REG_H(REG_CHIPTOP_BASE, x)

/* ADC */
#define L_BK_ADC_ATOP(x)    BK_REG_L(REG_ADC_ATOP_BASE, x)
#define H_BK_ADC_ATOP(x)    BK_REG_H(REG_ADC_ATOP_BASE, x)
#define L_BK_ADC_DTOP(x)    BK_REG_L(REG_ADC_DTOP_BASE, x)
#define H_BK_ADC_DTOP(x)    BK_REG_H(REG_ADC_DTOP_BASE, x)

/* MOD */
#define L_BK_MOD(x)         BK_REG_L(REG_MOD_BASE, x)
#define H_BK_MOD(x)         BK_REG_H(REG_MOD_BASE, x)

/* CEC */
#define L_BK_CEC(x)         BK_REG_L(REG_CEC_BASE, x)
#define H_BK_CEC(x)         BK_REG_H(REG_CEC_BASE, x)

/* PM SLP */
#define L_BK_PMSLP(x)         BK_REG_L(REG_PM_SLP_BASE, x)
#define H_BK_PMSLP(x)         BK_REG_H(REG_PM_SLP_BASE, x)

/* PM MCU */
#define L_BK_PMMCU(x)         BK_REG_L(REG_PM_MCU_BASE, x)
#define H_BK_PMMCU(x)         BK_REG_H(REG_PM_MCU_BASE, x)

//------------------------------------------------------------------------------
// Input source select

// BK_IP1F2_02 [2:0]
#define IP_INSSEL_ANALOG1   0x0000UL
#define IP_INSSEL_ANALOG2   0x0001UL
#define IP_INSSEL_ANALOG3   0x0002UL
#define IP_INSSEL_DVI       0x0003UL
#define IP_INSSEL_VIDEO     0x0004UL
#define IP_INSSEL_HDTV      0x0005UL
#define IP_INSSEL_HDMI      0x0007UL

#define IP_INSSEL_MASK      BITMASK(2:0)
#define IP_VDOSEL_MASK      BITMASK(1:0)

typedef enum
{
    IP_CCIR656_A,
    IP_MST_VD_A,
    IP_CCIR601,
    IP_MST_VD_B, // Don't use; RD's suggestion.
    IP_CCIR656_B=0x20
} VDOSEL;


typedef enum
{
    VE_IPMUX_ADC_A      = 0,            ///< ADC A
    VE_IPMUX_HDMI_DVI   = 1,            ///< DVI
    VE_IPMUX_VD         = 2,            ///< VD
    VE_IPMUX_MVOP       = 3,            ///< MPEG/DC0
    VE_IPMUX_SC_IP1     = 4,            ///< Scaler IP1 output
    VE_IPMUX_EXT_VD     = 5,            ///< External VD
    VE_IPMUX_ADC_B      = 6,            ///< ADC B
} VE_IPMUX_TYPE;

typedef enum
{
    AUTO_DETECT     =0x00,
    HV_SEPARATED    =0x01,
    COMPOSITE_SYNC  =0x02,
    SYNC_ON_GREEN   =0x03
}STYPE;

typedef enum
{
    CSYNC   = 0,
    SOG     = 1
}COMP;

//------------------------------------------------------------------------------
// ADC

#define ADC_AMUXA_MASK      BITMASK(1:0)
#define ADC_YMUX_MASK       BITMASK(3:0)
#define ADC_CMUX_MASK       BITMASK(7:4)


//------------------------------------------------------------------------------
// MUX

typedef enum
{
    ADC_RGB1,
    ADC_RGB2,
    ADC_RGB3,
}AMUX_SEL;

typedef enum // For PC/YPbPr input mux
{
    ANALOG_RGB0 = ADC_RGB1,
    ANALOG_RGB1 = ADC_RGB2,
    ANALOG_RGB2 = ADC_RGB3,
    ANALOG_RGB_DUMMY,
}ANALOG_RGB;

typedef enum
{
    MSVD_YMUX_CVBS0,
    MSVD_YMUX_CVBS1,
    MSVD_YMUX_CVBS2,
    MSVD_YMUX_CVBS3,

    MSVD_YMUX_Y0,
    MSVD_YMUX_Y1,
    MSVD_YMUX_C0,
    MSVD_YMUX_C1,

    MSVD_YMUX_SOG0 = 8,
    MSVD_YMUX_SOG1,
    MSVD_YMUX_SOG2,

    MSVD_YMUX_G0 = 11,
    MSVD_YMUX_G1 = 12,
    MSVD_YMUX_G2 = 13,
    MSVD_YMUX_CVBS4 = MSVD_YMUX_Y0,
    MSVD_YMUX_CVBS5 = MSVD_YMUX_Y1,
    MSVD_YMUX_CVBS6 = MSVD_YMUX_C0,
    MSVD_YMUX_CVBS7 = MSVD_YMUX_C1,
    MSVD_YMUX_NONE = 0xF,

    MSVD_YMUX_DUMMY,
}MS_VD_YMUX;

typedef enum
{
    MSVD_CMUX_CVBS0,
    MSVD_CMUX_CVBS1,
    MSVD_CMUX_CVBS2,
    MSVD_CMUX_CVBS3,

    MSVD_CMUX_Y0 = 4,
    MSVD_CMUX_Y1,
    MSVD_CMUX_C0,
    MSVD_CMUX_C1,

    MSVD_CMUX_SOG0 = 8,
    MSVD_CMUX_SOG1,
    MSVD_CMUX_SOG2,

    MSVD_CMUX_R0 = 11,
    MSVD_CMUX_R1 = 12,
    MSVD_CMUX_R2 = 13,
    MSVD_CMUX_CVBS4 = MSVD_CMUX_Y0,
    MSVD_CMUX_CVBS5 = MSVD_CMUX_C0,
    MSVD_CMUX_CVBS6 = MSVD_CMUX_Y1,
    MSVD_CMUX_CVBS7 = MSVD_CMUX_C1,
    MSVD_CMUX_NONE = 0xF,

    MSVD_CMUX_DUMMY,
}MS_VD_CMUX;

//------------------------------------------------------------------------------
// SCART

typedef enum // For specify scart RGB input
{
    SCART_RGB0 = 0x00,
    SCART_RGB1,
    SCART_RGB2,
    SCART_RGB_DUMMY,
}SCART_RGB;

typedef enum    // 0x2580[5:4]
{
    SCART_FB_NONE = 0x00,
    SCART_FB0,
    SCART_FB1,
    SCART_FB2,
}SCART_FB;

#define SCART_RGB_NONE  0xFF


///////////////////////////////////////////////////////
// MDHI bank
///////////////////////////////////////////////////////

#define L(x)                    ((x) + 0)
#define H(x)                    ((x) + 1)
#define __REG_HDMI(idx)         (REG_HDMI_BASE + (idx) * 2)

#define REG_HDMI_SYSCONFIG      __REG_HDMI(0x00)
#define REG_HDMI_ST1            __REG_HDMI(0x01)
#define REG_HDMI_ST2            __REG_HDMI(0x02)
#define REG_HDMI_ERR1           __REG_HDMI(0x04)
#define REG_HDMI_CONFIG1        __REG_HDMI(0x06)
#define REG_HDMI_CONFIG2        __REG_HDMI(0x07)
#define REG_HDMI_CONFIG3        __REG_HDMI(0x08)
#define REG_HDMI_CONFIG4        __REG_HDMI(0x09)
#define REG_HDMI_CLK_CFG        __REG_HDMI(0x0A)
#define REG_HDMI_TMCTRL         __REG_HDMI(0x0B)
#define REG_HDMI_FREQ_CMPVAL1   __REG_HDMI(0x0C)
#define REG_HDMI_FREQ_CMPVAL2   __REG_HDMI(0x0D)
#define REG_HDMI_FREQ_CMPVAL3   __REG_HDMI(0x0E)
#define REG_HDMI_PKT_TYPE       __REG_HDMI(0x10)
#define REG_HDMI_PCLK_FREQ      __REG_HDMI(0x11)
#define REG_HDMI_AUDIO_CLK0     __REG_HDMI(0x12)
#define REG_HDMI_AUDIO_CLK1     __REG_HDMI(0x13)
#define REG_HDMI_AUDIO_CLK2     __REG_HDMI(0x14)
#define REG_HDMI_GCONTROL       __REG_HDMI(0x15)
#define REG_HDMI_ACP_HB1        __REG_HDMI(0x16)
#define REG_HDMI_ACP_DATA0      __REG_HDMI(0x17)
#define REG_HDMI_ACP_DATA1      __REG_HDMI(0x18)
#define REG_HDMI_ACP_DATA2      __REG_HDMI(0x19)
#define REG_HDMI_ACP_DATA3      __REG_HDMI(0x1A)
#define REG_HDMI_ACP_DATA4      __REG_HDMI(0x1B)
#define REG_HDMI_ACP_DATA5      __REG_HDMI(0x1C)
#define REG_HDMI_ACP_DATA6      __REG_HDMI(0x1D)
#define REG_HDMI_ACP_DATA7      __REG_HDMI(0x1E)
#define REG_HDMI_ISRC_HB1       __REG_HDMI(0x1F)
#define REG_HDMI_ISRC_DATA0     __REG_HDMI(0x20)
#define REG_HDMI_ISRC_DATA1     __REG_HDMI(0x21)
#define REG_HDMI_ISRC_DATA2     __REG_HDMI(0x22)
#define REG_HDMI_ISRC_DATA3     __REG_HDMI(0x23)
#define REG_HDMI_ISRC_DATA4     __REG_HDMI(0x24)
#define REG_HDMI_ISRC_DATA5     __REG_HDMI(0x25)
#define REG_HDMI_ISRC_DATA6     __REG_HDMI(0x26)
#define REG_HDMI_ISRC_DATA7     __REG_HDMI(0x27)
#define REG_HDMI_ISRC_DATA8     __REG_HDMI(0x28)
#define REG_HDMI_ISRC_DATA9     __REG_HDMI(0x29)
#define REG_HDMI_ISRC_DATA10    __REG_HDMI(0x2A)
#define REG_HDMI_ISRC_DATA11    __REG_HDMI(0x2B)
#define REG_HDMI_ISRC_DATA12    __REG_HDMI(0x2C)
#define REG_HDMI_ISRC_DATA13    __REG_HDMI(0x2D)
#define REG_HDMI_ISRC_DATA14    __REG_HDMI(0x2E)
#define REG_HDMI_ISRC_DATA15    __REG_HDMI(0x2F)
#define REG_HDMI_VS_HDR0        __REG_HDMI(0x30)
#define REG_HDMI_VS_HDR1        __REG_HDMI(0x31)
#define REG_HDMI_VS_IF0         __REG_HDMI(0x32)
#define REG_HDMI_VS_IF1         __REG_HDMI(0x33)
#define REG_HDMI_VS_IF2         __REG_HDMI(0x34)
#define REG_HDMI_VS_IF3         __REG_HDMI(0x35)
#define REG_HDMI_VS_IF4         __REG_HDMI(0x36)
#define REG_HDMI_VS_IF5         __REG_HDMI(0x37)
#define REG_HDMI_VS_IF6         __REG_HDMI(0x38)
#define REG_HDMI_VS_IF7         __REG_HDMI(0x39)
#define REG_HDMI_VS_IF8         __REG_HDMI(0x3A)
#define REG_HDMI_VS_IF9         __REG_HDMI(0x3B)
#define REG_HDMI_VS_IF10        __REG_HDMI(0x3C)
#define REG_HDMI_VS_IF11        __REG_HDMI(0x3D)
#define REG_HDMI_VS_IF12        __REG_HDMI(0x3E)
#define REG_HDMI_VS_IF13        __REG_HDMI(0x3F)
#define REG_HDMI_AVI_IF0        __REG_HDMI(0x40)
#define REG_HDMI_AVI_IF1        __REG_HDMI(0x41)
#define REG_HDMI_AVI_IF2        __REG_HDMI(0x42)
#define REG_HDMI_AVI_IF3        __REG_HDMI(0x43)
#define REG_HDMI_AVI_IF4        __REG_HDMI(0x44)
#define REG_HDMI_AVI_IF5        __REG_HDMI(0x45)
#define REG_HDMI_AVI_IF6        __REG_HDMI(0x46)
#define REG_HDMI_SPD_IF0        __REG_HDMI(0x47)
#define REG_HDMI_SPD_IF1        __REG_HDMI(0x48)
#define REG_HDMI_SPD_IF2        __REG_HDMI(0x49)
#define REG_HDMI_SPD_IF3        __REG_HDMI(0x4A)
#define REG_HDMI_SPD_IF4        __REG_HDMI(0x4B)
#define REG_HDMI_SPD_IF5        __REG_HDMI(0x4C)
#define REG_HDMI_SPD_IF6        __REG_HDMI(0x4D)
#define REG_HDMI_SPD_IF7        __REG_HDMI(0x4E)
#define REG_HDMI_SPD_IF8        __REG_HDMI(0x4F)
#define REG_HDMI_SPD_IF9        __REG_HDMI(0x50)
#define REG_HDMI_SPD_IF10       __REG_HDMI(0x51)
#define REG_HDMI_SPD_IF11       __REG_HDMI(0x52)
#define REG_HDMI_SPD_IF12       __REG_HDMI(0x53)
#define REG_HDMI_AUDIO_IF0      __REG_HDMI(0x54)
#define REG_HDMI_AUDIO_IF1      __REG_HDMI(0x55)
#define REG_HDMI_AUDIO_IF2      __REG_HDMI(0x56)
#define REG_HDMI_MPEG_IF0       __REG_HDMI(0x57)
#define REG_HDMI_MPEG_IF1       __REG_HDMI(0x58)
#define REG_HDMI_MPEG_IF2       __REG_HDMI(0x59)
#define REG_HDMI_CS0            __REG_HDMI(0x5A)
#define REG_HDMI_CS1            __REG_HDMI(0x5B)
#define REG_HDMI_CS2            __REG_HDMI(0x5C)
#define REG_HDMI_PLL_CTRL1      __REG_HDMI(0x5D)
#define REG_HDMI_PLL_CTRL2      __REG_HDMI(0x5E)
#define REG_HDMI_PLL_CTRL3      __REG_HDMI(0x5F)
#define REG_HDMI_INT_MASK       __REG_HDMI(0x60)
#define REG_HDMI_INT_STATUS     __REG_HDMI(0x61)
#define REG_HDMI_INT_FORCE      __REG_HDMI(0x62)
#define REG_HDMI_INT_CLEAR      __REG_HDMI(0x63)
#define REG_HDMI_RESET_PACKET   __REG_HDMI(0x64)
#define REG_HDMI_AUTO_MODE      __REG_HDMI(0x65)
#define REG_HDMI_FRAME_RP_VAL   __REG_HDMI(0x66)
#define REG_HDMI_CEC_CONFIG1    __REG_HDMI(0x67)
#define REG_HDMI_CEC_CONFIG2    __REG_HDMI(0x68)
#define REG_HDMI_CEC_CONFIG3    __REG_HDMI(0x69)
#define REG_HDMI_CEC_CONFIG4    __REG_HDMI(0x6A)
#define REG_HDMI_CEC_STATUS1    __REG_HDMI(0x6C)
#define REG_HDMI_CEC_TX_DATA0    __REG_HDMI(0x70)
#define REG_HDMI_CEC_TX_DATA1    __REG_HDMI(0x71)
#define REG_HDMI_CEC_RX_DATA0    __REG_HDMI(0x78)
#define REG_HDMI_CEC_RX_DATA1    __REG_HDMI(0x79)


#define __REG_HDCP(idx)         (REG_HDCP_BASE + (idx) * 2)

#define REG_HDCP_00             __REG_HDCP(0x00)
#define REG_HDCP_STATUS         __REG_HDCP(0x01)
#define REG_HDCP_02             __REG_HDCP(0x02)
#define REG_HDCP_03             __REG_HDCP(0x03)

#define REG_BANK_MACE           0x05UL
#define REG_BANK_COMB           0x06UL    // VD Comb bank

//------------------------------------------------------------------------------
// HDMI

#define MS_HDMI_ACP_PKT         BIT11
#define MS_HDMI_ISRC1_PKT       BIT10
#define MS_HDMI_ISRC2_PKT       BIT9
#define MS_HDMI_NULL_PKT        BIT8
#define MS_HDMI_VS_PKT          BIT7
#define MS_HDMI_ACR_PKT         BIT6
#define MS_HDMI_ASAMPLE_PKT     BIT5
#define MS_HDMI_GC_PKT          BIT4
#define MS_HDMI_AVI_PKT         BIT3
#define MS_HDMI_SPD_PKT         BIT2
#define MS_HDMI_AUI_PKT         BIT1
#define MS_HDMI_MPEG_PKT        BIT0

//------------------------------------------------------------------------------
// DVI

typedef enum
{
    DVI_SW_A,
    DVI_SW_B,
}DVI_CH_SEL;


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
// MACE bank
////////////////////////////////////////////////////////////////////////////////
#define BK_MACE_01  0x01UL
#define BK_MACE_02  0x02UL
#define BK_MACE_03  0x03UL
#define BK_MACE_04  0x04UL
#define BK_MACE_05  0x05UL
#define BK_MACE_06  0x06UL
#define BK_MACE_07  0x07UL
#define BK_MACE_08  0x08UL
#define BK_MACE_09  0x09UL
#define BK_MACE_0A  0x0AUL
#define BK_MACE_0B  0x0BUL
#define BK_MACE_0C  0x0CUL
#define BK_MACE_0D  0x0DUL
#define BK_MACE_0E  0x0EUL
#define BK_MACE_0F  0x0FUL
#define BK_MACE_10  0x10UL
#define BK_MACE_11  0x11UL
#define BK_MACE_12  0x12UL
#define BK_MACE_13  0x13UL
#define BK_MACE_14  0x14UL
#define BK_MACE_15  0x15UL
#define BK_MACE_16  0x16UL
#define BK_MACE_17  0x17UL
#define BK_MACE_18  0x18UL
#define BK_MACE_19  0x19UL
#define BK_MACE_1A  0x1AUL
#define BK_MACE_1B  0x1BUL
#define BK_MACE_1C  0x1CUL
#define BK_MACE_1D  0x1DUL
#define BK_MACE_1E  0x1EUL
#define BK_MACE_1F  0x1FUL
#define BK_MACE_20  0x20UL
#define BK_MACE_21  0x21UL
#define BK_MACE_22  0x22UL
#define BK_MACE_23  0x23UL
#define BK_MACE_24  0x24UL
#define BK_MACE_25  0x25UL
#define BK_MACE_26  0x26UL
#define BK_MACE_27  0x27UL
#define BK_MACE_28  0x28UL
#define BK_MACE_29  0x29UL
#define BK_MACE_2A  0x2AUL
#define BK_MACE_2B  0x2BUL
#define BK_MACE_2C  0x2CUL
#define BK_MACE_2D  0x2DUL
#define BK_MACE_2E  0x2EUL
#define BK_MACE_2F  0x2FUL
#define BK_MACE_30  0x30UL
#define BK_MACE_31  0x31UL
#define BK_MACE_32  0x32UL
#define BK_MACE_33  0x33UL
#define BK_MACE_34  0x34UL
#define BK_MACE_35  0x35UL
#define BK_MACE_36  0x36UL
#define BK_MACE_37  0x37UL
#define BK_MACE_38  0x38UL
#define BK_MACE_39  0x39UL
#define BK_MACE_3A  0x3AUL
#define BK_MACE_3B  0x3BUL
#define BK_MACE_3C  0x3CUL
#define BK_MACE_3D  0x3DUL
#define BK_MACE_3E  0x3EUL
#define BK_MACE_3F  0x3FUL
#define BK_MACE_40  0x40UL
#define BK_MACE_41  0x41UL
#define BK_MACE_42  0x42UL
#define BK_MACE_43  0x43UL
#define BK_MACE_44  0x44UL
#define BK_MACE_45  0x45UL
#define BK_MACE_46  0x46UL
#define BK_MACE_47  0x47UL
#define BK_MACE_48  0x48UL
#define BK_MACE_49  0x49UL
#define BK_MACE_4A  0x4AUL
#define BK_MACE_4B  0x4BUL
#define BK_MACE_4C  0x4CUL
#define BK_MACE_4D  0x4DUL
#define BK_MACE_4E  0x4EUL
#define BK_MACE_4F  0x4FUL
#define BK_MACE_50  0x50UL
#define BK_MACE_51  0x51UL
#define BK_MACE_52  0x52UL
#define BK_MACE_53  0x53UL
#define BK_MACE_54  0x54UL
#define BK_MACE_55  0x55UL
#define BK_MACE_56  0x56UL
#define BK_MACE_57  0x57UL
#define BK_MACE_58  0x58UL
#define BK_MACE_59  0x59UL
#define BK_MACE_5A  0x5AUL
#define BK_MACE_5B  0x5BUL
#define BK_MACE_5C  0x5CUL
#define BK_MACE_5D  0x5DUL
#define BK_MACE_5E  0x5EUL
#define BK_MACE_5F  0x5FUL
#define BK_MACE_60  0x60UL
#define BK_MACE_61  0x61UL
#define BK_MACE_62  0x62UL
#define BK_MACE_63  0x63UL
#define BK_MACE_64  0x64UL
#define BK_MACE_65  0x65UL
#define BK_MACE_66  0x66UL
#define BK_MACE_67  0x67UL
#define BK_MACE_68  0x68UL
#define BK_MACE_69  0x69UL
#define BK_MACE_6A  0x6AUL
#define BK_MACE_6B  0x6BUL
#define BK_MACE_6C  0x6CUL
#define BK_MACE_6D  0x6DUL
#define BK_MACE_6E  0x6EUL
#define BK_MACE_6F  0x6FUL
#define BK_MACE_70  0x70UL
#define BK_MACE_71  0x71UL
#define BK_MACE_72  0x72UL
#define BK_MACE_73  0x73UL
#define BK_MACE_74  0x74UL
#define BK_MACE_75  0x75UL
#define BK_MACE_76  0x76UL
#define BK_MACE_77  0x77UL
#define BK_MACE_78  0x78UL
#define BK_MACE_79  0x79UL
#define BK_MACE_7A  0x7AUL
#define BK_MACE_7B  0x7BUL
#define BK_MACE_7C  0x7CUL
#define BK_MACE_7D  0x7DUL
#define BK_MACE_7E  0x7EUL
#define BK_MACE_7F  0x7FUL
#define BK_MACE_80  0x80UL
#define BK_MACE_81  0x81UL
#define BK_MACE_82  0x82UL
#define BK_MACE_83  0x83UL
#define BK_MACE_84  0x84UL
#define BK_MACE_85  0x85UL
#define BK_MACE_86  0x86UL
#define BK_MACE_87  0x87UL
#define BK_MACE_88  0x88UL
#define BK_MACE_89  0x89UL
#define BK_MACE_8A  0x8AUL
#define BK_MACE_8B  0x8BUL
#define BK_MACE_8C  0x8CUL
#define BK_MACE_8D  0x8DUL
#define BK_MACE_8E  0x8EUL
#define BK_MACE_8F  0x8FUL
#define BK_MACE_90  0x90UL
#define BK_MACE_91  0x91UL
#define BK_MACE_92  0x92UL
#define BK_MACE_93  0x93UL
#define BK_MACE_94  0x94UL
#define BK_MACE_95  0x95UL
#define BK_MACE_96  0x96UL
#define BK_MACE_97  0x97UL
#define BK_MACE_98  0x98UL
#define BK_MACE_99  0x99UL
#define BK_MACE_9A  0x9AUL
#define BK_MACE_9B  0x9BUL
#define BK_MACE_9C  0x9CUL
#define BK_MACE_9D  0x9DUL
#define BK_MACE_9E  0x9EUL
#define BK_MACE_9F  0x9FUL
#define BK_MACE_A0  0xA0UL
#define BK_MACE_A1  0xA1UL
#define BK_MACE_A2  0xA2UL
#define BK_MACE_A3  0xA3UL
#define BK_MACE_A4  0xA4UL
#define BK_MACE_A5  0xA5UL
#define BK_MACE_A6  0xA6UL
#define BK_MACE_A7  0xA7UL
#define BK_MACE_A8  0xA8UL
#define BK_MACE_A9  0xA9UL
#define BK_MACE_AA  0xAAUL
#define BK_MACE_AB  0xABUL
#define BK_MACE_AC  0xACUL
#define BK_MACE_AD  0xADUL
#define BK_MACE_AE  0xAEUL
#define BK_MACE_AF  0xAFUL
#define BK_MACE_B0  0xB0UL
#define BK_MACE_B1  0xB1UL
#define BK_MACE_B2  0xB2UL
#define BK_MACE_B3  0xB3UL
#define BK_MACE_B4  0xB4UL
#define BK_MACE_B5  0xB5UL
#define BK_MACE_B6  0xB6UL
#define BK_MACE_B7  0xB7UL
#define BK_MACE_B8  0xB8UL
#define BK_MACE_B9  0xB9UL
#define BK_MACE_BA  0xBAUL
#define BK_MACE_BB  0xBBUL
#define BK_MACE_BC  0xBCUL
#define BK_MACE_BD  0xBDUL
#define BK_MACE_BE  0xBEUL
#define BK_MACE_BF  0xBFUL
#define BK_MACE_C0  0xC0UL
#define BK_MACE_C1  0xC1UL
#define BK_MACE_C2  0xC2UL
#define BK_MACE_C3  0xC3UL
#define BK_MACE_C4  0xC4UL
#define BK_MACE_C5  0xC5UL
#define BK_MACE_C6  0xC6UL
#define BK_MACE_C7  0xC7UL
#define BK_MACE_C8  0xC8UL
#define BK_MACE_C9  0xC9UL
#define BK_MACE_CA  0xCAUL
#define BK_MACE_CB  0xCBUL
#define BK_MACE_CC  0xCCUL
#define BK_MACE_CD  0xCDUL
#define BK_MACE_CE  0xCEUL
#define BK_MACE_CF  0xCFUL
#define BK_MACE_D0  0xD0UL
#define BK_MACE_D1  0xD1UL
#define BK_MACE_D2  0xD2UL
#define BK_MACE_D3  0xD3UL
#define BK_MACE_D4  0xD4UL
#define BK_MACE_D5  0xD5UL
#define BK_MACE_D6  0xD6UL
#define BK_MACE_D7  0xD7UL
#define BK_MACE_D8  0xD8UL
#define BK_MACE_D9  0xD9UL
#define BK_MACE_DA  0xDAUL
#define BK_MACE_DB  0xDBUL
#define BK_MACE_DC  0xDCUL
#define BK_MACE_DD  0xDDUL
#define BK_MACE_DE  0xDEUL
#define BK_MACE_DF  0xDFUL
#define BK_MACE_E0  0xE0UL
#define BK_MACE_E1  0xE1UL
#define BK_MACE_E2  0xE2UL
#define BK_MACE_E3  0xE3UL
#define BK_MACE_E4  0xE4UL
#define BK_MACE_E5  0xE5UL
#define BK_MACE_E6  0xE6UL
#define BK_MACE_E7  0xE7UL
#define BK_MACE_E8  0xE8UL
#define BK_MACE_E9  0xE9UL
#define BK_MACE_EA  0xEAUL
#define BK_MACE_EB  0xEBUL
#define BK_MACE_EC  0xECUL
#define BK_MACE_ED  0xEDUL
#define BK_MACE_EE  0xEEUL
#define BK_MACE_EF  0xEFUL
#define BK_MACE_F0  0xF0UL
#define BK_MACE_F1  0xF1UL
#define BK_MACE_F2  0xF2UL
#define BK_MACE_F3  0xF3UL
#define BK_MACE_F4  0xF4UL
#define BK_MACE_F5  0xF5UL
#define BK_MACE_F6  0xF6UL
#define BK_MACE_F7  0xF7UL
#define BK_MACE_F8  0xF8UL
#define BK_MACE_F9  0xF9UL
#define BK_MACE_FA  0xFAUL
#define BK_MACE_FB  0xFBUL
#define BK_MACE_FC  0xFCUL
#define BK_MACE_FD  0xFDUL
#define BK_MACE_FE  0xFEUL
#define BK_MACE_FF  0xFFUL


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

#endif // ANALOG_REG_H
