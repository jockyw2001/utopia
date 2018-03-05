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

#ifndef _NSK2OTPFIELD_H_
#define _NSK2OTPFIELD_H_

////////////////////////////////////////////////////////////////////////////////
/// @file nsk2otpfield.h
/// @author MStar Semiconductor Inc.
/// @brief
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    ContentProtectionEnable = 0x40,
    NDS_OTP_TagBegin = ContentProtectionEnable,
    U_OTP_SBoot_0 = 0x41,
    U_OTP_ena_NSK2 = 0x42,
    U_OTP_ena_ACPUUseNSK2 = 0x43,
    U_OTP_ena_DBUSUseNSK2 = 0x44,
    U_OTP_ena_ForceTenSec = 0x45,
    U_OTP_ena_SCCheck = 0x46,
    U_OTP_ena_SWRN = 0x47,
    U_OTP_ena_TestRCFreq = 0x48,
    U_OTP_VenderConstSel = 0x49,

    U_OTP_RANDOM_0 = 0x4A,
    U_OTP_NOISE_0 = 0x4B,
    U_OTP_NDS_Parity_ChkEn_0 = 0x4C,
    U_OTP_OTPWritePWDProtect_0 = 0x4D,

    U_OTP_concurrency_configuration = 0x4E,
    U_OTP_nds_fc_disable = 0x4F,

    U_OTP_ena_EMMFilter = 0x52,
    U_OTP_allow_NSKCW2CryptoDMA = 0x53,
    U_OTP_ena_ACPU2DMA = 0x54,
    U_OTP_ena_DMA_DESBasedCipher = 0x55,
    U_OTP_ena_DMA_GetKeyFromKL = 0x56,
    U_OTP_allow_ACPU2KT = 0x57,


    U_OTP_allow_ACPUWrNSKKey2KT = 0x58,
    U_OTP_allow_NSK2KT = 0x59,
    U_OTP_ESAAlgo_invalidate = 0x5A,
    U_OTP_NDS_ContentProtect_sel = 0x5B,
    U_OTP_allow_CSA2Var = 0x5C,
    U_OTP_allow_CSA2Var_nonNSK = 0x5D,
    U_OTP_allow_CSA3NonNskVariant = 0x5E,
    U_OTP_allow_DVBCSA2ComfCipher = 0x5F,
    U_OTP_allow_DVBCSA3Var = 0x60,
    U_OTP_allow_Multi2BasedCipher = 0x61,
    U_OTP_Multi2SysKey = 0x62,
    U_OTP_SWMulti2SysKey = 0x63,
    U_OTP_ModifyXRC_Override = 0x64,
    U_OTP_Permutation_Override = 0x65,
    U_OTP_ena_TS2TSO = 0x66,
    U_OTP_ena_TS2TSO_0 = 0x67,
    U_OTP_ena_TS2TSO_1 = 0x68,
    U_OTP_ContentProEn_all1 = 0x69,
    U_OTP_ena_LSACPCM = 0x6A,
    U_OTP_EJTAG_MODE = 0x6B,
    U_OTP_DIG_JTAGMode = 0x6C,
    U_OTP_SBJTAGMode = 0x6D,
    U_OTP_SBI2CMode = 0x6E,
    U_OTP_I2CMode = 0x6F,	//previous: 7f. changed 2014.June.04
    U_OTP_SCANMode = 0x70,


    U_OTP_ena_USBSlaveMode = 0x71,
    U_OTP_Disable_TestInOut = 0x72,
    U_OTP_ena_NDS_JTAG_PWD = 0x73,//???
    U_OTP_SBoot_1 = 0x74,
    NDS_OTP_TagEnd,
}NSK2_OTPField;

#endif//_NSK2OTP_H_

