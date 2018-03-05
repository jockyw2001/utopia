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

#ifndef _PWSTABLE_H_
#define _PWSTABLE_H_

#define PWS_TBL_VERSION  "0.0_2015/07/23"
#define PWS_First_Items 0
#define OutOfRange_PWS_Items 51

static const MS_U16 pws_table[] =
{
  0xFE3F, /*EN_MUX_RGB*/
  0xFE3F, /*PD_ICLP_RGB*/
  0xB97D, /*PD_ICLP_VDY*/
  0xB9FD, /*PD_ICLP_VDC*/
  0xFEBF, /*PD_SOG_DAC*/
  0xFEBF, /*PD_SOG_MAIN*/
  0xFEBF, /*PD_SOG_MUX*/
  0xFEBF, /*PD_SOGOFF_DAC*/
  0xFEBF, /*PD_SOGOFF_MAIN*/
  0xFEBF, /*PD_SOGOFF_MUX*/
  0x383D, /*PD_BG*/
  0x383D, /*PD_REF*/
  0xFE3D, /*PD_REF_RGB*/
  0xB97D, /*PD_REF_YUV*/
  0xB97D, /*EN_MUX_VD_Y*/
  0xB9FD, /*EN_MUX_VD_C*/
  0xFFFD, /*EN_VDLPF_C*/
  0xB97D, /*EN_VDLPF_Y*/
  0x3FFF, /*EN_IDAC1*/
  0x3FFF, /*EN_IDAC_REF*/
  0xFEFF, /*PD_HSYNC0_COMP*/
  0xB83D, /*PD_ADCPLA_REG*/
  0xB97D, /*PD_ADCPLB_REG*/
  0xFE3D, /*PD_ADCR*/
  0xFE3F, /*PD_ADCG*/
  0xFE3F, /*PD_ADCB*/
  0xB97D, /*PD_ADCY*/
  0xFF7F, /*PD_FBLANK*/
  0xFEBF, /*PD_ADCA_PHDAC_R*/
  0xFEBF, /*PD_ADCA_PHDAC_G*/
  0xFEBF, /*PD_ADCA_PHDAC_B*/
  0xFF7D, /*PD_ADCB_PHDAC_R*/
  0xFF7F, /*PD_ADCB_PHDAC_G*/
  0xFF7F, /*PD_ADCB_PHDAC_B*/
  0xF97D, /*PD_ADCB_PHDAC_Y*/
  0xB83D, /*reg_pd_ldo_lv*/
  0xB83D, /*reg_pd_ldo25_adca*/
  0xB83D, /*reg_pd_ldo25_adcb*/
  0xB83D, /*reg_pd_ldo25_ana*/
  0x03FF, /*PWDN_REF*/
  0x03FF, /*PWDN_CLPBUF*/
  0x03FF, /*PWDN_ADC*/
  0x03FF, /*DMLDO_PD(2p5/2p6/1p1x2)*/
  0x03FF, /*MPLL_PD*/
  0x03FF, /*PD_DMPLL_CLK*/
  0x03FF, /*MPLL_ADC_CLK_PD*/
  0x03FF, /*MPLL_DIV2_PD*/
  0x03FF, /*MPLL_DIV3_PD*/
  0x03FF, /*MPLL_DIV4_PD*/
  0x03FF, /*MPLL_DIV8_PD*/
  0x03FF, /*MPLL_DIV10_PD*/
};

#define PWS_Init_First_Items 0
#define OutOfRange_PWS_Init_Items 3

static const MS_U16 pws_init_table[] =
{
  0xFFFF, /*EN_IDAC2*/
  0xFFFF, /*PD_ADCA_PHDAC_Y*/
  0xFFFF, /*PWDN_VCALBUF*/
};


#endif
