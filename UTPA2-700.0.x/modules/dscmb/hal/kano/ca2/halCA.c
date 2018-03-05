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
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halCA.c
// @brief  CA HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "MsCommon.h"
#include "regCA.h"
#include "drvCA.h"
#include "halCA.h"

#include <drvSYS.h>

// #undef MS_DEBUG_MSG
// #define MS_DEBUG_MSG(x) x

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Global Definition
//--------------------------------------------------------------------------------------------------
#define MAX_RESERVED_SIZE 128

//--------------------------------------------------------------------------------------------------
//  CA Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
static MS_VIRT _u32REGBase = 0;
static MS_VIRT _u32REGPMBase = 0;
static MS_VIRT _u32REG_OTP_Base = 0;
static MS_VIRT _u32REG_RSA_Base = 0;

#define REG16(addr) (*(volatile unsigned short *)(_u32REGBase + (addr)))
#define REG16_WRITE(addr, value, mask) do { REG16(addr) = (REG16(addr) & ~(mask)) | ((value) & (mask)); } while(0)

#define REG32(addr) (*(volatile unsigned *)(_u32REGBase + (addr)))
#define REG32_WRITE(addr, value, mask) do { REG32(addr) = (REG32(addr) & ~(mask)) | ((value) & (mask)); } while(0)

#define REG32_PM(addr) (*(volatile unsigned *)(_u32REGPMBase + (addr)))
#define REG16_PM(addr) (*(volatile unsigned short *)(_u32REGPMBase + (addr)))

#ifdef CA_NO_PRINTF
#define HAL_CA_DEBUG(fmt, args...)
#else
#define HAL_CA_DEBUG(fmt, args...) do{ \
                printf("[%s][%d]"fmt"\n", __FUNCTION__, __LINE__,  ## args);}\
                while(0)
#endif
static unsigned bitmask(unsigned hi, unsigned lo)
{
    unsigned x, s = hi - lo + 1;
    // printf("hi=%d lo=%d size=%d\n", hi, lo, s);

    if(s >= sizeof(unsigned) * 8)
        x = (unsigned)-1;
    else
        x = (1U << s) - 1;
    return x << lo;
}

struct {
    MS_U32 u32Addr;
    MS_U32 u32Hi;
    MS_U32 u32Lo;
} _HAL_CA_OTP_Addr[] = {
//Parse from Kano_otp_mapping_v0.93(20151127).xlsx
[U_OTP_CA_reserved0] = {0x003500, 255, 0},
[U_OTP_CA_reserved1] = {0x003520, 255, 0},
[U_OTP_CA_reserved2] = {0x003540, 255, 0},
[U_OTP_CA_reserved3] = {0x003560, 255, 0},
[U_OTP_CA_reserved4] = {0x003580, 255, 0},
[U_OTP_CA_reserved5] = {0x0035A0, 255, 0},
[U_OTP_CA_reserved6] = {0x0035C0, 255, 0},
[U_OTP_CA_reserved7] = {0x0035E0, 255, 0},
[U_OTP_CA_reserved8] = {0x003600, 255, 0},
[U_OTP_CA_reserved9] = {0x003620, 255, 0},
[U_OTP_CA_reserved10] = {0x003640, 255, 0},
[U_OTP_CA_reserved11] = {0x003660, 255, 0},
[U_OTP_CA_reserved12] = {0x003680, 255, 0},
[U_OTP_CA_reserved13] = {0x0036A0, 255, 0},
[U_OTP_CA_reserved14] = {0x0036C0, 255, 0},
[U_OTP_CA_reserved15] = {0x0036E0, 255, 0},
[U_OTP_Chip_Extension_ID] = {0x003700, 127, 0},
[U_OTP_PVConfiguration] = {0x003710, 23, 0},
[U_OTP_EncryptLoader_Key_sel] = {0x003710, 27, 24},
[U_OTP_EncryptLoader_KL_level] = {0x003710, 29, 28},
[U_OTP_SW_Reserved0] = {0x003710, 31, 30},
[U_OTP_EncryptLoader_ForceEnc] = {0x003714, 1, 0},
[U_OTP_SW_Reserved1] = {0x003714, 31, 2},
[U_OTP_HASH0_VER_REF] = {0x003718, 31, 0},
[U_OTP_HASH1_VER_REF] = {0x00371C, 31, 0},
[U_OTP_HASH2_VER_REF] = {0x003720, 31, 0},
[U_OTP_SW_Reserved2] = {0x003724, 31, 0},
[U_OTP_Versioning] = {0x003728, 511, 0},
[U_OTP_Preenablement_0] = {0x003768, 31, 0},
[U_OTP_Preenablement_1] = {0x00376C, 31, 0},
[U_OTP_Preenablement_2] = {0x003770, 31, 0},
[U_OTP_Preenablement_3] = {0x003774, 31, 0},
[U_OTP_Preenablement_4] = {0x003778, 31, 0},
[U_OTP_Preenablement_5] = {0x00377C, 31, 0},
[U_OTP_Preenablement_6] = {0x003780, 31, 0},
[U_OTP_Preenablement_7] = {0x003784, 31, 0},
[U_OTP_CAdata_0] = {0x003788, 31, 0},
[U_OTP_CAdata_1] = {0x00378C, 31, 0},
[U_OTP_CAdata_2] = {0x003790, 31, 0},
[U_OTP_CAdata_3] = {0x003794, 31, 0},
[U_OTP_CAdata_4] = {0x003798, 31, 0},
[U_OTP_CAdata_5] = {0x00379C, 31, 0},
[U_OTP_CAdata_6] = {0x0037A0, 31, 0},
[U_OTP_CAdata_7] = {0x0037A4, 31, 0},
[U_OTP_CAdata_8] = {0x0037A8, 31, 0},
[U_OTP_CAdata_9] = {0x0037AC, 31, 0},
[U_OTP_CAdata_10] = {0x0037B0, 31, 0},
[U_OTP_CAdata_11] = {0x0037B4, 31, 0},
[U_OTP_CAdata_12] = {0x0037B8, 31, 0},
[U_OTP_CAdata_13] = {0x0037BC, 31, 0},
[U_OTP_CAdata_14] = {0x0037C0, 31, 0},
[U_OTP_CAdata_15] = {0x0037C4, 31, 0},
[U_OTP_CAdata_16] = {0x0037C8, 31, 0},
[U_OTP_CAdata_17] = {0x0037CC, 31, 0},
[U_OTP_CAdata_18] = {0x0037D0, 31, 0},
[U_OTP_CAdata_19] = {0x0037D4, 31, 0},
[U_OTP_CAdata_20] = {0x0037D8, 31, 0},
[U_OTP_CAdata_21] = {0x0037DC, 31, 0},
[U_OTP_CAdata_22] = {0x0037E0, 31, 0},
[U_OTP_CAdata_23] = {0x0037E4, 31, 0},
[U_OTP_CAdata_24] = {0x0037E8, 31, 0},
[U_OTP_CAdata_25] = {0x0037EC, 31, 0},
[U_OTP_CAdata_26] = {0x0037F0, 31, 0},
[U_OTP_CAdata_27] = {0x0037F4, 31, 0},
[U_OTP_CAdata_28] = {0x0037F8, 31, 0},
[U_OTP_CAdata_29] = {0x0037FC, 31, 0},
[U_OTP_CAdata_30] = {0x003800, 31, 0},
[U_OTP_CAdata_31] = {0x003804, 31, 0},
[U_OTP_CAdata_32] = {0x003808, 31, 0},
[U_OTP_CAdata_33] = {0x00380C, 31, 0},
[U_OTP_CAdata_34] = {0x003810, 31, 0},
[U_OTP_CAdata_35] = {0x003814, 31, 0},
[U_OTP_CAdata_36] = {0x003818, 31, 0},
[U_OTP_CAdata_37] = {0x00381C, 31, 0},
[U_OTP_forbid_SCPU2DRAM] = {0x003820, 3, 0},
[U_OTP_forbid_SCPUQMEM_to_DRAM] = {0x003820, 7, 4},
[U_OTP_forbid_CryptoDMA_keyslot_QMEM] = {0x003820, 11, 8},
[U_OTP_forbid_SCPU2UART] = {0x003820, 15, 12},
[U_OTP_IXSC_config1] = {0x003820, 19, 16},
[U_OTP_IXSC_config2] = {0x003820, 23, 20},
[U_OTP_IXSC_config3] = {0x003820, 27, 24},
[U_OTP_IXSC_config4] = {0x003820, 31, 28},
[U_OTP_LockWord_Preenablement_0_W] = {0x003824, 1, 0},
[U_OTP_LockWord_Preenablement_1_W] = {0x003824, 3, 2},
[U_OTP_LockWord_Preenablement_2_W] = {0x003824, 5, 4},
[U_OTP_LockWord_Preenablement_3_W] = {0x003824, 7, 6},
[U_OTP_LockWord_Preenablement_4_W] = {0x003824, 9, 8},
[U_OTP_LockWord_Preenablement_5_W] = {0x003824, 11, 10},
[U_OTP_LockWord_Preenablement_6_W] = {0x003824, 13, 12},
[U_OTP_LockWord_Preenablement_7_W] = {0x003824, 15, 14},
[U_OTP_Test_Switch] = {0x003824, 23, 16},
[U_OTP_IXSC_Reserved0] = {0x003824, 31, 24},
[U_OTP_NOCS3_00] = {0x003828, 383, 0},
[U_OTP_NOCS3_01] = {0x003858, 383, 0},
[U_OTP_NOCS3_02] = {0x003888, 127, 0},
[U_OTP_NOCS3_03] = {0x003898, 127, 0},
[U_OTP_MSID] = {0x0038A8, 31, 0},
[U_OTP_CFG_STB_CA_SN] = {0x0038AC, 31, 0},
[U_OTP_RSA_0] = {0x0038B0, 2047, 0},
[U_OTP_RSA_1] = {0x0039B0, 2047, 0},
[U_OTP_RSA_0_integrity_chk] = {0x003AB0, 1, 0},
[U_OTP_RSA_1_integrity_chk] = {0x003AB0, 3, 2},
[U_OTP_ena_ACPUUseHWRSAKey] = {0x003AB0, 7, 4},
[U_OTP_forbid_ACPUReadRSAKey0] = {0x003AB0, 10, 8},
[U_OTP_forbid_ACPUReadRSAKey1] = {0x003AB0, 13, 11},
[U_OTP_forbid_ACPUWrite_Versioning] = {0x003AB0, 15, 14},
[U_OTP_forbid_ACPURead_Versioning] = {0x003AB0, 17, 16},
[U_OTP_RSA_Reserved0] = {0x003AB0, 31, 18},
[U_OTP_RSA_0_CRC] = {0x003AB4, 15, 0},
[U_OTP_RSA_1_CRC] = {0x003AB4, 31, 16},
[U_OTP_PGMask0] = {0x003AB8, 31, 0},
[U_OTP_PGMask1] = {0x003ABC, 31, 0},
[U_OTP_PGMask2] = {0x003AC0, 31, 0},
[U_OTP_PGMask3] = {0x003AC4, 31, 0},
[U_OTP_PGMask4] = {0x003AC8, 31, 0},
[U_OTP_PGMask5] = {0x003ACC, 31, 0},
[U_OTP_PGMask6] = {0x003AD0, 31, 0},
[U_OTP_PGMask7] = {0x003AD4, 31, 0},
[U_OTP_PGMask8] = {0x003AD8, 31, 0},
[U_OTP_PGMask9] = {0x003ADC, 31, 0},
[U_OTP_PGMask10] = {0x003AE0, 31, 0},
[U_OTP_PGMask11] = {0x003AE4, 31, 0},
[U_OTP_PGMask12] = {0x003AE8, 31, 0},
[U_OTP_PGMask13] = {0x003AEC, 31, 0},
[U_OTP_PGMask14] = {0x003AF0, 31, 0},
[U_OTP_PGMask15] = {0x003AF4, 31, 0},
[U_OTP_PGMask16] = {0x003AF8, 31, 0},
[U_OTP_PGMask17] = {0x003AFC, 31, 0},
[U_OTP_v_PubOtpOID] = {0x003B00, 15, 0},
[U_OTP_NDS_Reserved0] = {0x003B00, 31, 16},
[U_OTP_v_PubOtpVID] = {0x003B04, 15, 0},
[U_OTP_NDS_Reserved1] = {0x003B04, 31, 16},
[U_OTP_v_PubOtpGP] = {0x003B08, 15, 0},
[U_OTP_LockWord_GP_group_W] = {0x003B08, 31, 16},
[U_OTP_v_PubOtpBID] = {0x003B0C, 3, 0},
[U_OTP_NDS_Reserved2] = {0x003B0C, 31, 4},
[U_OTP_v_PubOtpRsaIndex] = {0x003B10, 3, 0},
[U_OTP_NDS_Reserved3] = {0x003B10, 31, 4},
[U_OTP_v_PubOtpMinConfVer] = {0x003B14, 7, 0},
[U_OTP_NDS_Reserved4] = {0x003B14, 31, 8},
[U_OTP_VenderConstSel] = {0x003B18, 2, 0},
[U_OTP_NDS_Reserved5] = {0x003B18, 31, 3},
[U_OTP_LockWord_NDS_Key1] = {0x003B1C, 2, 0},
[U_OTP_LockWord_NDS_Key2] = {0x003B1C, 5, 3},
[U_OTP_LockWord_NDS_Key3] = {0x003B1C, 8, 6},
[U_OTP_LockWord_NDS_Key4] = {0x003B1C, 11, 9},
[U_OTP_LockWord_NDS_Key5] = {0x003B1C, 14, 12},
[U_OTP_LockWord_NDS_Key6] = {0x003B1C, 17, 15},
[U_OTP_LockWord_NDS_Key7] = {0x003B1C, 20, 18},
[U_OTP_LockWord_NDS_Key8] = {0x003B1C, 23, 21},
[U_OTP_NDSKeyValid] = {0x003B1C, 31, 24},
[U_OTP_forbid_NSK_wr_sck] = {0x003B20, 7, 0},
[U_OTP_UseCheckSum] = {0x003B20, 15, 8},
[U_OTP_allow_NDSKey_BlankChk] = {0x003B20, 17, 16},
[U_OTP_allow_ProgFail_RuinNDSKey] = {0x003B20, 19, 18},
[U_OTP_allow_NDSReadKeyWait200ms] = {0x003B20, 21, 20},
[U_OTP_allow_ReadErrorRstOtp] = {0x003B20, 23, 22},
[U_OTP_allow_illegalNDSFlagChk] = {0x003B20, 27, 24},
[U_OTP_allow_Rst_NDS_SCFlag_ParityFail] = {0x003B20, 31, 28},
[U_OTP_ena_ACPUUseNSK2] = {0x003B24, 3, 0},
[U_OTP_ena_DBUSUseNSK2] = {0x003B24, 7, 4},
[U_OTP_ena_ForceOneMilSec] = {0x003B24, 11, 8},
[U_OTP_allow_SCCheck] = {0x003B24, 15, 12},
[U_OTP_ena_TestRCFreq] = {0x003B24, 19, 16},
[U_OTP_ena_SWRN] = {0x003B24, 23, 20},
[U_OTP_ena_NSKSeedPRNG] = {0x003B24, 27, 24},
[U_OTP_OTPWritePWDProtect] = {0x003B24, 31, 28},
[U_OTP_NDS_keysel] = {0x003B28, 5, 0},
[U_OTP_allow_NSK2_PWD_Mode] = {0x003B28, 7, 6},
[U_OTP_ena_EMMFilter] = {0x003B28, 11, 8},
[U_OTP_ena_TestGenIN] = {0x003B28, 15, 12},
[U_OTP_RCFreq_map] = {0x003B28, 20, 16},
[U_OTP_NDS_Reserved6] = {0x003B28, 31, 21},
[U_OTP_allow_SkipBadNVBit] = {0x003B2C, 1, 0},
[U_OTP_allow_NDSSC_ReadFail_BadPkt] = {0x003B2C, 3, 2},
[U_OTP_allow_RANDOM_keybus] = {0x003B2C, 5, 4},
[U_OTP_allow_RANDOM_byteacc] = {0x003B2C, 7, 6},
[U_OTP_allow_NDS_Rd55AA] = {0x003B2C, 9, 8},
[U_OTP_allow_NDS_Parity_chk] = {0x003B2C, 11, 10},
[U_OTP_allow_NDS_KeyRd55AA] = {0x003B2C, 13, 12},
[U_OTP_forbid_OTPBuiltInTest] = {0x003B2C, 15, 14},
[U_OTP_forbid_Kilo_ProgRepair] = {0x003B2C, 17, 16},
[U_OTP_NDS_Reserved7] = {0x003B2C, 31, 18},
[U_OTP_NVCOUNT] = {0x003B30, 1023, 0},
[U_OTP_NDS_SecretKey1] = {0x003BB0, 127, 0},
[U_OTP_NDS_SecretKey2] = {0x003BC0, 127, 0},
[U_OTP_NDS_SecretKey3] = {0x003BD0, 127, 0},
[U_OTP_NDS_SecretKey4] = {0x003BE0, 127, 0},
[U_OTP_NDS_SecretKey5] = {0x003BF0, 127, 0},
[U_OTP_NDS_SecretKey6] = {0x003C00, 127, 0},
[U_OTP_NDS_SecretKey7] = {0x003C10, 127, 0},
[U_OTP_NDS_SecretKey8] = {0x003C20, 127, 0},
[U_OTP_NDSKey1Chksum] = {0x003C30, 7, 0},
[U_OTP_NDSKey1Tag] = {0x003C30, 15, 8},
[U_OTP_NDSKey2Chksum] = {0x003C30, 23, 16},
[U_OTP_NDSKey2Tag] = {0x003C30, 31, 24},
[U_OTP_NDSKey3Chksum] = {0x003C34, 7, 0},
[U_OTP_NDSKey3Tag] = {0x003C34, 15, 8},
[U_OTP_NDSKey4Chksum] = {0x003C34, 23, 16},
[U_OTP_NDSKey4Tag] = {0x003C34, 31, 24},
[U_OTP_NDSKey5Chksum] = {0x003C38, 7, 0},
[U_OTP_NDSKey5Tag] = {0x003C38, 15, 8},
[U_OTP_NDSKey6Chksum] = {0x003C38, 23, 16},
[U_OTP_NDSKey6Tag] = {0x003C38, 31, 24},
[U_OTP_NDSKey7Chksum] = {0x003C3C, 7, 0},
[U_OTP_NDSKey7Tag] = {0x003C3C, 15, 8},
[U_OTP_NDSKey8Chksum] = {0x003C3C, 23, 16},
[U_OTP_NDSKey8Tag] = {0x003C3C, 31, 24},
[U_OTP_NDS_Key1_CRC] = {0x003C40, 15, 0},
[U_OTP_NDS_Key2_CRC] = {0x003C40, 31, 16},
[U_OTP_NDS_Key3_CRC] = {0x003C44, 15, 0},
[U_OTP_NDS_Key4_CRC] = {0x003C44, 31, 16},
[U_OTP_NDS_Key5_CRC] = {0x003C48, 15, 0},
[U_OTP_NDS_Key6_CRC] = {0x003C48, 31, 16},
[U_OTP_NDS_Key7_CRC] = {0x003C4C, 15, 0},
[U_OTP_NDS_Key8_CRC] = {0x003C4C, 31, 16},
[U_OTP_NDS_ESCK_Key1_obfuscation] = {0x003C50, 1, 0},
[U_OTP_NDS_Key1_integrity_chk] = {0x003C50, 3, 2},
[U_OTP_NDS_ESCK_Key2_obfuscation] = {0x003C50, 5, 4},
[U_OTP_NDS_Key2_integrity_chk] = {0x003C50, 7, 6},
[U_OTP_NDS_ESCK_Key3_obfuscation] = {0x003C50, 9, 8},
[U_OTP_NDS_Key3_integrity_chk] = {0x003C50, 11, 10},
[U_OTP_NDS_ESCK_Key4_obfuscation] = {0x003C50, 13, 12},
[U_OTP_NDS_Key4_integrity_chk] = {0x003C50, 15, 14},
[U_OTP_NDS_ESCK_Key5_obfuscation] = {0x003C50, 17, 16},
[U_OTP_NDS_Key5_integrity_chk] = {0x003C50, 19, 18},
[U_OTP_NDS_ESCK_Key6_obfuscation] = {0x003C50, 21, 20},
[U_OTP_NDS_Key6_integrity_chk] = {0x003C50, 23, 22},
[U_OTP_NDS_ESCK_Key7_obfuscation] = {0x003C50, 25, 24},
[U_OTP_NDS_Key7_integrity_chk] = {0x003C50, 27, 26},
[U_OTP_NDS_ESCK_Key8_obfuscation] = {0x003C50, 29, 28},
[U_OTP_NDS_Key8_integrity_chk] = {0x003C50, 31, 30},
[U_OTP_EJTAG_MODE_Parity_bit] = {0x003C54, 0, 0},
[U_OTP_I2C_MODE_Parity_bit] = {0x003C54, 1, 1},
[U_OTP_SCAN_MODE_Parity_bit] = {0x003C54, 2, 2},
[U_OTP_VenderConstSel_Parity_bit] = {0x003C54, 3, 3},
[U_OTP_v_PubOtpRsaIndex_Parity_bit] = {0x003C54, 4, 4},
[U_OTP_NDS_ESCK_Key1_obfuscation_Parity_bit] = {0x003C54, 5, 5},
[U_OTP_NDS_ESCK_Key2_obfuscation_Parity_bit] = {0x003C54, 6, 6},
[U_OTP_NDS_ESCK_Key3_obfuscation_Parity_bit] = {0x003C54, 7, 7},
[U_OTP_NDS_ESCK_Key4_obfuscation_Parity_bit] = {0x003C54, 8, 8},
[U_OTP_NDS_ESCK_Key5_obfuscation_Parity_bit] = {0x003C54, 9, 9},
[U_OTP_NDS_ESCK_Key6_obfuscation_Parity_bit] = {0x003C54, 10, 10},
[U_OTP_NDS_ESCK_Key7_obfuscation_Parity_bit] = {0x003C54, 11, 11},
[U_OTP_NDS_ESCK_Key8_obfuscation_Parity_bit] = {0x003C54, 12, 12},
[U_OTP_allow_ReadErrorRstOtp_Parity_bit] = {0x003C54, 13, 13},
[U_OTP_ena_ACPUUseNSK2_Parity_bit] = {0x003C54, 14, 14},
[U_OTP_BootMode_Parity_bit] = {0x003C54, 15, 15},
[U_OTP_ena_ACPU2KT_Parity_bit] = {0x003C54, 16, 16},
[U_OTP_ena_EMMFilter_Parity_bit] = {0x003C54, 17, 17},
[U_OTP_allow_NDS_Rd55AA_Parity_bit] = {0x003C54, 18, 18},
[U_OTP_forbid_OTPBuiltInTest_Parity_bit] = {0x003C54, 19, 19},
[U_OTP_allow_NSK_RNG_ROSC_Parity_bit] = {0x003C54, 20, 20},
[U_OTP_SecretAreaEnable_Parity_bit] = {0x003C54, 21, 21},
[U_OTP_forbid_SW_SPSD_Key_Parity_bit] = {0x003C54, 22, 22},
[U_OTP_allow_OTP_BgCRC_Parity_bit] = {0x003C54, 23, 23},
[U_OTP_forbid_USBSlaveMode_Parity_bit] = {0x003C54, 24, 24},
[U_OTP_OTPWritePWDProtect_Parity_bit] = {0x003C54, 25, 25},
[U_OTP_ena_NSK2_Parity_bit] = {0x003C54, 26, 26},
[U_OTP_NDS_Reserved8] = {0x003C54, 31, 27},
[U_OTP_ena_ESAAlgo_invalidate] = {0x003C58, 3, 0},
[U_OTP_ena_LocalAlgo_Invalidate] = {0x003C58, 7, 4},
[U_OTP_ContentProtEn] = {0x003C58, 11, 8},
[U_OTP_concurrency_configuration] = {0x003C58, 13, 12},
[U_OTP_allow_NSK_RNG_ROSC] = {0x003C58, 15, 14},
[U_OTP_nds_fc_disable] = {0x003C58, 16, 16},
[U_OTP_NDS_CPNR0_sel] = {0x003C58, 17, 17},
[U_OTP_NDS_CPNR_off] = {0x003C58, 18, 18},
[U_OTP_NDS_Reserved9] = {0x003C58, 31, 19},
[U_OTP_PM51_ST_ADDR] = {0x003C5C, 11, 0},
[U_OTP_PM51_ED_ADDR] = {0x003C5C, 23, 12},
[U_OTP_forbid_PM51ReadOTP] = {0x003C5C, 25, 24},
[U_OTP_forbid_PM51WriteOTP] = {0x003C5C, 27, 26},
[U_OTP_allow_PM51] = {0x003C5C, 29, 28},
[U_OTP_PM51_SW_R2] = {0x003C5C, 31, 30},
[U_OTP_forbid_CLK_SEED_TEST] = {0x003C60, 0, 0},
[U_OTP_MOBF_TOP_use_DES] = {0x003C60, 1, 1},
[U_OTP_OBFUSCATEVideoStream] = {0x003C60, 3, 2},
[U_OTP_forbid_EJTAG_Pinshare] = {0x003C60, 5, 4},
[U_OTP_allow_clear_sram] = {0x003C60, 6, 6},
[U_OTP_forbid_dft_md_sram] = {0x003C60, 7, 7},
[U_OTP_LINK_ISOEN2GPIO4] = {0x003C60, 8, 8},
[U_OTP_PM_SLEEP_WR_PROT_EN] = {0x003C60, 9, 9},
[U_OTP_PWR_HW_RST_MODE_EN] = {0x003C60, 10, 10},
[U_OTP_ISOC_TST_DIG_EN_GATE] = {0x003C60, 11, 11},
[U_OTP_OTHERS_Reserved0] = {0x003C60, 15, 12},
[U_OTP_Rsv_Feature] = {0x003C60, 31, 16},
[U_OTP_forbid_CA_MCM] = {0x003C64, 2, 0},
[U_OTP_forbid_DIAMOND_PMU] = {0x003C64, 5, 3},
[U_OTP_allow_force_dual_core] = {0x003C64, 6, 6},
[U_OTP_disable_jpi] = {0x003C64, 7, 7},
[U_OTP_forbid_DIG_PMU] = {0x003C64, 10, 8},
[U_OTP_forbid_WHOLECHIP_alive_when_NOCORE_alive] = {0x003C64, 13, 11},
[U_OTP_OTHERS_Reserved2] = {0x003C64, 15, 14},
[U_OTP_allow_CORE0_always_alive] = {0x003C64, 18, 16},
[U_OTP_allow_NONCORE0_default_reset] = {0x003C64, 21, 19},
[U_OTP_pwrgd_int_en] = {0x003C64, 22, 22},
[U_OTP_OTHERS_Reserved3] = {0x003C64, 31, 23},
[U_OTP_LOGICBIST_SCAN_OUT_CRC32] = {0x003C68, 31, 0},
[U_OTP_OTHERS_Reserved4] = {0x003C6C, 31, 0},
[U_OTP_RC_OPTION] = {0x003C70, 7, 0},
[U_OTP_RC_OPTION_EN] = {0x003C70, 8, 8},
[U_OTP_GCR_TRVBG] = {0x003C70, 14, 9},
[U_OTP_OTHERS_Reserved5] = {0x003C70, 15, 15},
[U_OTP_TEST_Status_TE] = {0x003C70, 23, 16},
[U_OTP_TEST_Status] = {0x003C70, 31, 24},
[U_OTP_ena_PVRNS2S] = {0x003C74, 3, 0},
[U_OTP_ena_PVRS2NS] = {0x003C74, 7, 4},
[U_OTP_ena_PlayBackRec0] = {0x003C74, 11, 8},
[U_OTP_ena_PlayBackRec1] = {0x003C74, 15, 12},
[U_OTP_ena_PlayBackRec2] = {0x003C74, 19, 16},
[U_OTP_ena_PlayBackRec3] = {0x003C74, 23, 20},
[U_OTP_ena_PlayBackRec4] = {0x003C74, 27, 24},
[U_OTP_ena_PlayBackRec5] = {0x003C74, 31, 28},
[U_OTP_ena_PVR_secure_protect_0] = {0x003C78, 3, 0},
[U_OTP_ena_PVR_secure_protect_1] = {0x003C78, 7, 4},
[U_OTP_ena_VQ_secure_protect_0] = {0x003C78, 11, 8},
[U_OTP_TSP_Reserved0] = {0x003C78, 15, 12},
[U_OTP_dis_TSO] = {0x003C78, 19, 16},
[U_OTP_ena_TS2TSO_0] = {0x003C78, 23, 20},
[U_OTP_ena_TS2TSO_1] = {0x003C78, 27, 24},
[U_OTP_ena_TSO_SecRange] = {0x003C78, 31, 28},
[U_OTP_CAVIDEnforcedTS0] = {0x003C7C, 5, 0},
[U_OTP_CAVIDEnforcedTS1] = {0x003C7C, 11, 6},
[U_OTP_CAVIDEnforcedTS2] = {0x003C7C, 17, 12},
[U_OTP_CAVIDEnforcedTS3] = {0x003C7C, 23, 18},
[U_OTP_CAVIDEnforcedTS4] = {0x003C7C, 29, 24},
[U_OTP_TSP_Reserved1] = {0x003C7C, 31, 30},
[U_OTP_CAVIDEnforcedTS5] = {0x003C80, 5, 0},
[U_OTP_ForbidAVtoSec] = {0x003C80, 6, 6},
[U_OTP_TSP_Reserved2] = {0x003C80, 7, 7},
[U_OTP_dis_clear_stream_to_PVR] = {0x003C80, 11, 8},
[U_OTP_forbid_SW_SPSD_Key] = {0x003C80, 13, 12},
[U_OTP_forbid_KL_SPSD_Key] = {0x003C80, 15, 14},
[U_OTP_allow_TSPCPUCodeProt] = {0x003C80, 17, 16},
[U_OTP_TSP_Reserved3] = {0x003C80, 31, 18},
[U_OTP_ena_KeyLadder_0_lock] = {0x003C84, 3, 0},
[U_OTP_ena_KeyLadder_1_lock] = {0x003C84, 7, 4},
[U_OTP_ena_NSK2_lock] = {0x003C84, 11, 8},
[U_OTP_ena_CERT_IP_lock] = {0x003C84, 15, 12},
[U_OTP_ena_DMA_lock] = {0x003C84, 19, 16},
[U_OTP_ena_SCPU_lock] = {0x003C84, 23, 20},
[U_OTP_Dlock_Reserved0] = {0x003C84, 31, 24},
[U_OTP_ena_ACPU2KT_lock] = {0x003C88, 3, 0},
[U_OTP_ena_NSK2KT_lock] = {0x003C88, 7, 4},
[U_OTP_ena_KL2KT_lock] = {0x003C88, 11, 8},
[U_OTP_ena_SCPU2KT_lock] = {0x003C88, 15, 12},
[U_OTP_ena_ACPU2DMA_lock] = {0x003C88, 19, 16},
[U_OTP_ena_DMA_DESBasedCipher_lock] = {0x003C88, 23, 20},
[U_OTP_Dlock_Reserved1] = {0x003C88, 31, 24},
[U_OTP_ena_LSACPCM_lock] = {0x003C8C, 3, 0},
[U_OTP_ena_AESBasedCipher_lock] = {0x003C8C, 7, 4},
[U_OTP_ena_DESBasedCipher_lock] = {0x003C8C, 11, 8},
[U_OTP_ena_3DESBasedCipher_lock] = {0x003C8C, 15, 12},
[U_OTP_ena_Multi2BasedCipher_lock] = {0x003C8C, 19, 16},
[U_OTP_ena_DVBCSA2ConfCipher_lock] = {0x003C8C, 23, 20},
[U_OTP_ena_DVBCSA2Cipher_lock] = {0x003C8C, 27, 24},
[U_OTP_ena_DVBCSA3BasedCipher_lock] = {0x003C8C, 31, 28},
[U_OTP_dis_kl0_hostkey_lock] = {0x003C90, 3, 0},
[U_OTP_dis_kl1_hostkey_lock] = {0x003C90, 7, 4},
[U_OTP_NDSKeyValid_lock] = {0x003C90, 11, 8},
[U_OTP_RC_OPTION_lock] = {0x003C90, 15, 12},
[U_OTP_Dlock_Reserved2] = {0x003C90, 31, 16},
[U_OTP_Dlock_Reserved3] = {0x003C94, 31, 0},
[U_OTP_LockWord_CA_reserved0_W] = {0x003C98, 1, 0},
[U_OTP_LockWord_CA_reserved1_W] = {0x003C98, 3, 2},
[U_OTP_LockWord_CA_reserved2_W] = {0x003C98, 5, 4},
[U_OTP_LockWord_CA_reserved3_W] = {0x003C98, 7, 6},
[U_OTP_LockWord_CA_reserved4_W] = {0x003C98, 9, 8},
[U_OTP_LockWord_CA_reserved5_W] = {0x003C98, 11, 10},
[U_OTP_LockWord_CA_reserved6_W] = {0x003C98, 13, 12},
[U_OTP_LockWord_CA_reserved7_W] = {0x003C98, 15, 14},
[U_OTP_LockWord_CA_reserved8_W] = {0x003C98, 17, 16},
[U_OTP_LockWord_CA_reserved9_W] = {0x003C98, 19, 18},
[U_OTP_LockWord_CA_reserved10_W] = {0x003C98, 21, 20},
[U_OTP_LockWord_CA_reserved11_W] = {0x003C98, 23, 22},
[U_OTP_LockWord_CA_reserved12_W] = {0x003C98, 25, 24},
[U_OTP_LockWord_CA_reserved13_W] = {0x003C98, 27, 26},
[U_OTP_LockWord_CA_reserved14_W] = {0x003C98, 29, 28},
[U_OTP_LockWord_CA_reserved15_W] = {0x003C98, 31, 30},
[U_OTP_LockWord_RSA_0_W] = {0x003C9C, 1, 0},
[U_OTP_LockWord_RSA_1_W] = {0x003C9C, 3, 2},
[U_OTP_LockWord_RSA_CTRL_W] = {0x003C9C, 5, 4},
[U_OTP_LockWord_IXSC_Config_W] = {0x003C9C, 7, 6},
[U_OTP_LockWord_v_PubOtpOID_W] = {0x003C9C, 9, 8},
[U_OTP_LockWord_v_PubOtpVID_W] = {0x003C9C, 11, 10},
[U_OTP_LockWord_v_PubOtpBID_W] = {0x003C9C, 13, 12},
[U_OTP_LockWord_v_PubOtpRsaIndex_W] = {0x003C9C, 15, 14},
[U_OTP_LockWord_v_PubOtpMinConfVer_W] = {0x003C9C, 17, 16},
[U_OTP_LockWord_VenderConstSel_W] = {0x003C9C, 19, 18},
[U_OTP_LockWord_NDS_info_W] = {0x003C9C, 21, 20},
[U_OTP_LockWord_PM51_W] = {0x003C9C, 23, 22},
[U_OTP_LockWord_OTHERS_W] = {0x003C9C, 25, 24},
[U_OTP_LockWord_Reserved0] = {0x003C9C, 31, 26},
[U_OTP_LockWord_NOCS3_MEM0_W] = {0x003CA0, 1, 0},
[U_OTP_LockWord_NOCS3_MEM1_W] = {0x003CA0, 3, 2},
[U_OTP_LockWord_NOCS3_MEM2_W] = {0x003CA0, 5, 4},
[U_OTP_LockWord_NOCS3_MEM3_W] = {0x003CA0, 7, 6},
[U_OTP_LockWord_MSID_W] = {0x003CA0, 9, 8},
[U_OTP_LockWord_CFG_STB_CA_SN_W] = {0x003CA0, 11, 10},
[U_OTP_LockWord_TSP_W] = {0x003CA0, 13, 12},
[U_OTP_LockWord_PGMask_W] = {0x003CA0, 15, 14},
[U_OTP_LockWord_ClkDet_Para_W] = {0x003CA0, 17, 16},
[U_OTP_LockWord_OTP_CTRL_W] = {0x003CA0, 19, 18},
[U_OTP_LockWord_CPU_bank_W] = {0x003CA0, 21, 20},
[U_OTP_LockWord_trimming_W] = {0x003CA0, 23, 22},
[U_OTP_LockWord_BOND0_W] = {0x003CA0, 25, 24},
[U_OTP_LockWord_BOND_Reserved_W] = {0x003CA0, 27, 26},
[U_OTP_LockWord_Reserved1] = {0x003CA0, 31, 28},
[U_OTP_LockWord_LOT_INFO_W] = {0x003CA4, 1, 0},
[U_OTP_LockWord_CA_Enable_W] = {0x003CA4, 3, 2},
[U_OTP_LockWord_KL_bank_W] = {0x003CA4, 5, 4},
[U_OTP_LockWord_KT_bank_W] = {0x003CA4, 7, 6},
[U_OTP_LockWord_DMA_bank_W] = {0x003CA4, 9, 8},
[U_OTP_LockWord_TSCE_W] = {0x003CA4, 11, 10},
[U_OTP_LockWord_HDCP_KEY_W] = {0x003CA4, 13, 12},
[U_OTP_LockWord_Multi2SysKey_W] = {0x003CA4, 15, 14},
[U_OTP_LockWord_CAV_Key1_W] = {0x003CA4, 17, 16},
[U_OTP_LockWord_CAV_Key2_W] = {0x003CA4, 19, 18},
[U_OTP_LockWord_CAV_Key3_W] = {0x003CA4, 21, 20},
[U_OTP_LockWord_CAV_Key4_W] = {0x003CA4, 23, 22},
[U_OTP_LockWord_CAV_Key5_W] = {0x003CA4, 25, 24},
[U_OTP_LockWord_CAV_Key6_W] = {0x003CA4, 27, 26},
[U_OTP_LockWord_CAV_Key7_W] = {0x003CA4, 29, 28},
[U_OTP_LockWord_CAV_Key8_W] = {0x003CA4, 31, 30},
[U_OTP_LockWord_CAVID_W] = {0x003CA8, 1, 0},
[U_OTP_LockWord_CAV_KP1_W] = {0x003CA8, 3, 2},
[U_OTP_LockWord_CAV_KP2_W] = {0x003CA8, 5, 4},
[U_OTP_LockWord_CAV_KP3_W] = {0x003CA8, 7, 6},
[U_OTP_LockWord_CAV_KP4_W] = {0x003CA8, 9, 8},
[U_OTP_LockWord_CAV_KP5_W] = {0x003CA8, 11, 10},
[U_OTP_LockWord_CAV_KP6_W] = {0x003CA8, 13, 12},
[U_OTP_LockWord_CAV_KP7_W] = {0x003CA8, 15, 14},
[U_OTP_LockWord_CAV_KP8_W] = {0x003CA8, 17, 16},
[U_OTP_LockWord_CAV_KP9_W] = {0x003CA8, 19, 18},
[U_OTP_LockWord_CAV_KP10_W] = {0x003CA8, 21, 20},
[U_OTP_LockWord_CAV_KP11_W] = {0x003CA8, 23, 22},
[U_OTP_LockWord_CAV_KP12_W] = {0x003CA8, 25, 24},
[U_OTP_LockWord_CAV_KP13_W] = {0x003CA8, 27, 26},
[U_OTP_LockWord_CAV_KP14_W] = {0x003CA8, 29, 28},
[U_OTP_LockWord_CAV_KP15_W] = {0x003CA8, 31, 30},
[U_OTP_LockWord_PubOTPUniqueID1_W] = {0x003CAC, 1, 0},
[U_OTP_LockWord_PubOTPUniqueID2_W] = {0x003CAC, 3, 2},
[U_OTP_LockWord_PubOTPUniqueID3_W] = {0x003CAC, 5, 4},
[U_OTP_LockWord_PubOTPUniqueID4_W] = {0x003CAC, 7, 6},
[U_OTP_LockWord_AES_MOD_MASK_W] = {0x003CAC, 9, 8},
[U_OTP_LockWord_ACPU_Property_W] = {0x003CAC, 11, 10},
[U_OTP_LockWord_SCPU_Property_W] = {0x003CAC, 13, 12},
[U_OTP_LockWord_CA_PWD_W] = {0x003CAC, 15, 14},
[U_OTP_LockWord_CA_PWD_CTRL_W] = {0x003CAC, 17, 16},
[U_OTP_LockWord_VtrackData_W] = {0x003CAC, 19, 18},
[U_OTP_LockWord_FAKE_W] = {0x003CAC, 21, 20},
[U_OTP_LockWord_SW0_W] = {0x003CAC, 23, 22},
[U_OTP_LockWord_Reserved2] = {0x003CAC, 31, 26},
[U_OTP_LockWord_0000_0511_W] = {0x003CB0, 1, 0},
[U_OTP_LockWord_0512_1023_W] = {0x003CB0, 3, 2},
[U_OTP_LockWord_1024_1535_W] = {0x003CB0, 5, 4},
[U_OTP_LockWord_1536_2047_W] = {0x003CB0, 7, 6},
[U_OTP_LockWord_2048_2559_W] = {0x003CB0, 9, 8},
[U_OTP_LockWord_2560_3071_W] = {0x003CB0, 11, 10},
[U_OTP_LockWord_3072_3391_W] = {0x003CB0, 13, 12},
[U_OTP_LockWord_Chip_Extension_ID] = {0x003CB0, 15, 14},
[U_OTP_LockWord_Reserved3] = {0x003CB0, 31, 16},
[U_OTP_LockWord_CAV_Key1_R] = {0x003CB4, 1, 0},
[U_OTP_LockWord_CAV_Key2_R] = {0x003CB4, 3, 2},
[U_OTP_LockWord_CAV_Key3_R] = {0x003CB4, 5, 4},
[U_OTP_LockWord_CAV_Key4_R] = {0x003CB4, 7, 6},
[U_OTP_LockWord_CAV_Key5_R] = {0x003CB4, 9, 8},
[U_OTP_LockWord_CAV_Key6_R] = {0x003CB4, 11, 10},
[U_OTP_LockWord_CAV_Key7_R] = {0x003CB4, 13, 12},
[U_OTP_LockWord_CAV_Key8_R] = {0x003CB4, 15, 14},
[U_OTP_LockWord_RSA_0_R] = {0x003CB4, 17, 16},
[U_OTP_LockWord_RSA_1_R] = {0x003CB4, 19, 18},
[U_OTP_LockWord_HDCP_KEY_R] = {0x003CB4, 21, 20},
[U_OTP_LockWord_Multi2SysKey_R] = {0x003CB4, 23, 22},
[U_OTP_LockWord_AES_MOD_MASK_R] = {0x003CB4, 25, 24},
[U_OTP_LockWord_VtrackData_R] = {0x003CB4, 27, 26},
[U_OTP_LockWord_CA_PWD_R] = {0x003CB4, 29, 28},
[U_OTP_LockWord_Reserved4] = {0x003CB4, 31, 30},
[U_OTP_ClkDet_Para] = {0x003CB8, 63, 0},
[U_OTP_forbid_ACPUWriteOTP] = {0x003CC0, 1, 0},
[U_OTP_forbid_ACPUReadOTP] = {0x003CC0, 3, 2},
[U_OTP_forbid_SCPUWriteOTP] = {0x003CC0, 5, 4},
[U_OTP_forbid_SCPUReadOTP] = {0x003CC0, 7, 6},
[U_OTP_forbid_DBBUSWriteOTP] = {0x003CC0, 9, 8},
[U_OTP_forbid_DBBUSReadOTP] = {0x003CC0, 11, 10},
[U_OTP_allow_RANDOM] = {0x003CC0, 13, 12},
[U_OTP_allow_NOISE_Rd] = {0x003CC0, 15, 14},
[U_OTP_allow_OTP_BgCRC] = {0x003CC0, 17, 16},
[U_OTP_allow_SKIP_0] = {0x003CC0, 19, 18},
[U_OTP_allow_SKIP_1] = {0x003CC0, 21, 20},
[U_OTP_allow_Mask_AES] = {0x003CC0, 23, 22},
[U_OTP_ena_Mstar_Wrapper] = {0x003CC0, 25, 24},
[U_OTP_CTRL_Reserved0] = {0x003CC0, 31, 26},
[U_OTP_allow_XOR_ROMCODE] = {0x003CC4, 6, 0},
[U_OTP_allow_XOR_FIX] = {0x003CC4, 7, 7},
[U_OTP_allow_PWD_OTP_prog] = {0x003CC4, 9, 8},
[U_OTP_CTRL_Reserved1] = {0x003CC4, 31, 10},
[U_OTP_SBoot] = {0x003CC8, 3, 0},
[U_OTP_SecretAreaEnable] = {0x003CC8, 7, 4},
[U_OTP_forbid_TestInOut] = {0x003CC8, 9, 8},
[U_OTP_allow_FlashProtect] = {0x003CC8, 11, 10},
[U_OTP_PERSO_done] = {0x003CC8, 13, 12},
[U_OTP_CERT_PERSO_done] = {0x003CC8, 15, 14},
[U_OTP_DBUG_Reserved0] = {0x003CC8, 31, 16},
[U_OTP_SCAN_MODE] = {0x003CCC, 8, 0},
[U_OTP_DBUG_Reserved1] = {0x003CCC, 9, 9},
[U_OTP_MBIST_MODE] = {0x003CCC, 15, 10},
[U_OTP_I2C_MODE] = {0x003CCC, 21, 16},
[U_OTP_DBUG_Reserved2] = {0x003CCC, 25, 22},
[U_OTP_EJTAG_MODE] = {0x003CCC, 31, 26},
[U_OTP_RMA_MODE] = {0x003CD0, 5, 0},
[U_OTP_LicRework] = {0x003CD0, 7, 6},
[U_OTP_forbid_USBSlaveMode] = {0x003CD0, 11, 8},
[U_OTP_allow_DRAM_MOBF] = {0x003CD0, 13, 12},
[U_OTP_allow_DRAMOBF_4ROUND] = {0x003CD0, 15, 14},
[U_OTP_ena_DRAMOBF_NS_MASK_ADDR] = {0x003CD0, 17, 16},
[U_OTP_forbid_clk_otp_sel] = {0x003CD0, 20, 18},
[U_OTP_forbid_acpu_access_sec_bridge] = {0x003CD0, 23, 21},
[U_OTP_forbid_dbbus_access_sec_bridge] = {0x003CD0, 26, 24},
[U_OTP_XPM_State] = {0x003CD0, 29, 27},
[U_OTP_DBUG_Reserved3] = {0x003CD0, 31, 30},
[U_OTP_DeactiveDRAMBoot] = {0x003CD4, 3, 0},
[U_OTP_ACPU_Reserved0] = {0x003CD4, 7, 4},
[U_OTP_BootMode] = {0x003CD4, 13, 8},
[U_OTP_forbid_BCKGND_CHK] = {0x003CD4, 15, 14},
[U_OTP_SCPUBootMode] = {0x003CD4, 21, 16},
[U_OTP_ACPU_Reserved1] = {0x003CD4, 23, 22},
[U_OTP_PM_MCU_RST_MOD] = {0x003CD4, 26, 24},
[U_OTP_boot_host_sel] = {0x003CD4, 29, 27},
[U_OTP_ACPU_Reserved2] = {0x003CD4, 31, 30},
[U_OTP_ACPU_Reserved3] = {0x003CD8, 11, 0},
[U_OTP_PostMskAreaRangeU] = {0x003CD8, 23, 12},
[U_OTP_ACPUasHost] = {0x003CD8, 26, 24},
[U_OTP_BONDING] = {0x003CD8, 28, 27},
[U_OTP_NON_BLANK] = {0x003CD8, 29, 29},
[U_OTP_ACPU_Reserved4] = {0x003CD8, 31, 30},
[U_OTP_PE_LOT_INFO] = {0x003CDC, 47, 0},
[U_OTP_BootDevice] = {0x003CDC, 55, 48},
[U_OTP_MAX_RDNTBLK] = {0x003CDC, 61, 56},
[U_OTP_forbid_STR] = {0x003CDC, 63, 62},
[U_OTP_Bootcode_project] = {0x003CE4, 7, 0},
[U_OTP_Bootcode_subversion] = {0x003CE4, 15, 8},
[U_OTP_Bootcode_version] = {0x003CE4, 31, 16},
[U_OTP_Analog_Usage] = {0x003CE8, 63, 0},
[U_OTP_Analog_Usage2] = {0x003CF0, 95, 0},
[U_OTP_trim_dac] = {0x003CFC, 125, 0},
[U_OTP_Trim_Reserved0] = {0x003CFC, 127, 126},
[U_OTP_BOND0] = {0x003D0C, 95, 0},
[U_OTP_BOND_Reserved] = {0x003D18, 95, 0},
[U_OTP_ena_KeyLadder_0] = {0x003D24, 3, 0},
[U_OTP_ena_KeyLadder_1] = {0x003D24, 7, 4},
[U_OTP_ena_NSK2] = {0x003D24, 11, 8},
[U_OTP_ena_CERT_IP] = {0x003D24, 13, 12},
[U_OTP_ena_DMA] = {0x003D24, 15, 14},
[U_OTP_ena_SCPU] = {0x003D24, 19, 16},
[U_OTP_CA_EN_Reserved0] = {0x003D24, 31, 20},
[U_OTP_allow_IXSC] = {0x003D28, 3, 0},
[U_OTP_forbid_IXSC] = {0x003D28, 7, 4},
[U_OTP_forbid_PAVOSC] = {0x003D28, 9, 8},
[U_OTP_cfDeactiveFuse] = {0x003D28, 10, 10},
[U_OTP_SOC_UID_SEL] = {0x003D28, 12, 11},
[U_OTP_CA_EN_Reserved2] = {0x003D28, 31, 13},
[U_OTP_dis_kl0_hostkey] = {0x003D2C, 1, 0},
[U_OTP_dis_kl1_hostkey] = {0x003D2C, 3, 2},
[U_OTP_dis_kl0_hostkey_property] = {0x003D2C, 5, 4},
[U_OTP_dis_kl1_hostkey_property] = {0x003D2C, 7, 6},
[U_OTP_kl_protect0_mode] = {0x003D2C, 9, 8},
[U_OTP_kl_protect1_mode] = {0x003D2C, 11, 10},
[U_OTP_kl_protect2_mode] = {0x003D2C, 13, 12},
[U_OTP_kl_protect3_mode] = {0x003D2C, 15, 14},
[U_OTP_dis_kl_protect0] = {0x003D2C, 17, 16},
[U_OTP_dis_kl_protect1] = {0x003D2C, 19, 18},
[U_OTP_dis_kl_protect2] = {0x003D2C, 21, 20},
[U_OTP_dis_kl_protect3] = {0x003D2C, 23, 22},
[U_OTP_dis_kl_protect4] = {0x003D2C, 25, 24},
[U_OTP_kl_TA_LUT0_mode] = {0x003D2C, 27, 26},
[U_OTP_kl_TA_LUT1_mode] = {0x003D2C, 29, 28},
[U_OTP_ena_kl_StopClkChk] = {0x003D2C, 31, 30},
[U_OTP_KDF_secret_mask] = {0x003D30, 127, 0},
[U_OTP_IMLB_ROTATION] = {0x003D40, 15, 0},
[U_OTP_IrdetoTweakedCSAv3] = {0x003D40, 17, 16},
[U_OTP_allow_SPSSPDKeyShuffle] = {0x003D40, 21, 18},
[U_OTP_kl_BISS_MODE] = {0x003D40, 23, 22},
[U_OTP_ena_kl_xor_func] = {0x003D40, 25, 24},
[U_OTP_KL_Reserved2] = {0x003D40, 31, 26},
[U_OTP_ena_ACPU2KT] = {0x003D44, 3, 0},
[U_OTP_ena_NSK2KT] = {0x003D44, 7, 4},
[U_OTP_ena_KL2KT] = {0x003D44, 11, 8},
[U_OTP_ena_SCPU2KT] = {0x003D44, 15, 12},
[U_OTP_ena_ACPUWrNSKKey2KT] = {0x003D44, 19, 16},
[U_OTP_ena_ACPUWrCFBKey2KT] = {0x003D44, 23, 20},
[U_OTP_ena_NSKCW2CryptoDMA] = {0x003D44, 27, 24},
[U_OTP_ena_ACPU2LSA] = {0x003D44, 31, 28},
[U_OTP_BISS_MODE] = {0x003D48, 3, 0},
[U_OTP_BISS_SUBMODE] = {0x003D48, 5, 4},
[U_OTP_KT_Reserved0] = {0x003D48, 31, 6},
[U_OTP_ena_ACPU2DMA] = {0x003D4C, 3, 0},
[U_OTP_ena_DMA_DESBasedCipher] = {0x003D4C, 7, 4},
[U_OTP_ena_AESMAC_MaxRate] = {0x003D4C, 9, 8},
[U_OTP_allow_DMAtDES2KeyEQChk] = {0x003D4C, 12, 10},
[U_OTP_DMA_Reserved0] = {0x003D4C, 13, 13},
[U_OTP_ena_DMA_ClearKeySlot ] = {0x003D4C, 15, 14},
[U_OTP_dis_DMA_mask_engine] = {0x003D4C, 17, 16},
[U_OTP_DMA_Reserved1] = {0x003D4C, 31, 18},
[U_OTP_ena_LSACPCM] = {0x003D50, 3, 0},
[U_OTP_ena_AESBasedCipher] = {0x003D50, 7, 4},
[U_OTP_ena_DESBasedCipher] = {0x003D50, 11, 8},
[U_OTP_ena_3DESBasedCipher] = {0x003D50, 15, 12},
[U_OTP_ena_Multi2BasedCipher] = {0x003D50, 19, 16},
[U_OTP_ena_DVBCSA2ConfCipher] = {0x003D50, 23, 20},
[U_OTP_ena_DVBCSA2Cipher] = {0x003D50, 27, 24},
[U_OTP_ena_DVBCSA3BasedCipher] = {0x003D50, 31, 28},
[U_OTP_Ch_SwitchComb] = {0x003D54, 63, 0},
[U_OTP_ena_LSAD_MDI] = {0x003D5C, 3, 0},
[U_OTP_ena_LSAD_MDD] = {0x003D5C, 7, 4},
[U_OTP_ena_LSAD_CIPLUS_AES] = {0x003D5C, 11, 8},
[U_OTP_ena_LSAD_AES_ECB_CLEAR] = {0x003D5C, 15, 12},
[U_OTP_ena_LSAD_SCTE41_SCTE52_DES] = {0x003D5C, 19, 16},
[U_OTP_TSCE_Reserved0] = {0x003D5C, 31, 20},
[U_OTP_ena_ESA_CIPLUS_AES] = {0x003D60, 3, 0},
[U_OTP_ena_ESA_CIPLUS_DES] = {0x003D60, 7, 4},
[U_OTP_ena_ESA_SCTE52_DES] = {0x003D60, 11, 8},
[U_OTP_ena_ESA_tDES_CBC_CLEAR] = {0x003D60, 15, 12},
[U_OTP_TSCE_Reserved1] = {0x003D60, 31, 16},
[U_OTP_ena_LSAS_MDI] = {0x003D64, 3, 0},
[U_OTP_ena_LSAS_MDD] = {0x003D64, 7, 4},
[U_OTP_ena_LSAS_CIPLUS_AES] = {0x003D64, 11, 8},
[U_OTP_ena_LSAS_AES_ECB_CLEAR] = {0x003D64, 15, 12},
[U_OTP_ena_LSAS_SCTE41_SCTE52_DES] = {0x003D64, 19, 16},
[U_OTP_TSCE_Reserved2] = {0x003D64, 31, 20},
[U_OTP_ena_DRMAreaEncrypt] = {0x003D68, 3, 0},
[U_OTP_ena_ReviewFailPkt] = {0x003D68, 7, 4},
[U_OTP_dis_NonSecRangeEncrypt] = {0x003D68, 11, 8},
[U_OTP_TSCE_Reserved3] = {0x003D68, 31, 12},
[U_OTP_ena_CA_PVR_secure_protect_0] = {0x003D6C, 3, 0},
[U_OTP_ena_CA_PVR_secure_protect_1] = {0x003D6C, 7, 4},
[U_OTP_ena_CA_PVR_secure_protect_2] = {0x003D6C, 11, 8},
[U_OTP_ena_CA_PVR_secure_protect_3] = {0x003D6C, 15, 12},
[U_OTP_ena_LowerPathRec] = {0x003D6C, 19, 16},
[U_OTP_ena_SWMulti2SysKey] = {0x003D6C, 23, 20},
[U_OTP_TSCE_Reserved4] = {0x003D6C, 31, 24},
[U_OTP_HDCP_KEY] = {0x003D70, 55, 0},
[U_OTP_HDCP_Reserved0] = {0x003D70, 7, 0},
[U_OTP_Multi2SysKey] = {0x003D78, 255, 0},
[U_OTP_AES_MOD_MASK] = {0x003D98, 127, 0},
[U_OTP_v_pubOtpUniqueID1] = {0x003DA8, 63, 0},
[U_OTP_v_pubOtpUniqueID2] = {0x003DB0, 63, 0},
[U_OTP_v_pubOtpUniqueID3] = {0x003DB8, 63, 0},
[U_OTP_v_pubOtpUniqueID4] = {0x003DC0, 63, 0},
[U_OTP_CAV_ESCK_KeyValid] = {0x003DC8, 7, 0},
[U_OTP_CAV_Reserved0] = {0x003DC8, 15, 8},
[U_OTP_CAV_SCK_KeyValid] = {0x003DC8, 31, 16},
[U_OTP_CAV_Reserved1] = {0x003DCC, 31, 0},
[U_OTP_CAV_Key1_SCK_mapping] = {0x003DD0, 15, 0},
[U_OTP_CAV_Key1_integrity_chk] = {0x003DD0, 17, 16},
[U_OTP_CAV_Key1_allow_ChkNum] = {0x003DD0, 19, 18},
[U_OTP_CAV_Key1_obfuscation] = {0x003DD0, 21, 20},
[U_OTP_CAV_Key1_IdSel] = {0x003DD0, 23, 22},
[U_OTP_CAV_Key1_KeySel] = {0x003DD0, 29, 24},
[U_OTP_CAV_Key1_reserved] = {0x003DD0, 31, 30},
[U_OTP_CAV_Key1_ChkNum_Gold] = {0x003DD4, 31, 0},
[U_OTP_CAV_Key2_SCK_mapping] = {0x003DD8, 15, 0},
[U_OTP_CAV_Key2_integrity_chk] = {0x003DD8, 17, 16},
[U_OTP_CAV_Key2_allow_ChkNum] = {0x003DD8, 19, 18},
[U_OTP_CAV_Key2_obfuscation] = {0x003DD8, 21, 20},
[U_OTP_CAV_Key2_IdSel] = {0x003DD8, 23, 22},
[U_OTP_CAV_Key2_KeySel] = {0x003DD8, 29, 24},
[U_OTP_CAV_Key2_reserved] = {0x003DD8, 31, 30},
[U_OTP_CAV_Key2_ChkNum_Gold] = {0x003DDC, 31, 0},
[U_OTP_CAV_Key3_SCK_mapping] = {0x003DE0, 15, 0},
[U_OTP_CAV_Key3_integrity_chk] = {0x003DE0, 17, 16},
[U_OTP_CAV_Key3_allow_ChkNum] = {0x003DE0, 19, 18},
[U_OTP_CAV_Key3_obfuscation] = {0x003DE0, 21, 20},
[U_OTP_CAV_Key3_IdSel] = {0x003DE0, 23, 22},
[U_OTP_CAV_Key3_KeySel] = {0x003DE0, 29, 24},
[U_OTP_CAV_Key3_reserved] = {0x003DE0, 31, 30},
[U_OTP_CAV_Key3_ChkNum_Gold] = {0x003DE4, 31, 0},
[U_OTP_CAV_Key4_SCK_mapping] = {0x003DE8, 15, 0},
[U_OTP_CAV_Key4_integrity_chk] = {0x003DE8, 17, 16},
[U_OTP_CAV_Key4_allow_ChkNum] = {0x003DE8, 19, 18},
[U_OTP_CAV_Key4_obfuscation] = {0x003DE8, 21, 20},
[U_OTP_CAV_Key4_IdSel] = {0x003DE8, 23, 22},
[U_OTP_CAV_Key4_KeySel] = {0x003DE8, 29, 24},
[U_OTP_CAV_Key4_reserved] = {0x003DE8, 31, 30},
[U_OTP_CAV_Key4_ChkNum_Gold] = {0x003DEC, 31, 0},
[U_OTP_CAV_Key5_SCK_mapping] = {0x003DF0, 15, 0},
[U_OTP_CAV_Key5_integrity_chk] = {0x003DF0, 17, 16},
[U_OTP_CAV_Key5_allow_ChkNum] = {0x003DF0, 19, 18},
[U_OTP_CAV_Key5_obfuscation] = {0x003DF0, 21, 20},
[U_OTP_CAV_Key5_IdSel] = {0x003DF0, 23, 22},
[U_OTP_CAV_Key5_KeySel] = {0x003DF0, 29, 24},
[U_OTP_CAV_Key5_reserved] = {0x003DF0, 31, 30},
[U_OTP_CAV_Key5_ChkNum_Gold] = {0x003DF4, 31, 0},
[U_OTP_CAV_Key6_SCK_mapping] = {0x003DF8, 15, 0},
[U_OTP_CAV_Key6_integrity_chk] = {0x003DF8, 17, 16},
[U_OTP_CAV_Key6_allow_ChkNum] = {0x003DF8, 19, 18},
[U_OTP_CAV_Key6_obfuscation] = {0x003DF8, 21, 20},
[U_OTP_CAV_Key6_IdSel] = {0x003DF8, 23, 22},
[U_OTP_CAV_Key6_KeySel] = {0x003DF8, 29, 24},
[U_OTP_CAV_Key6_reserved] = {0x003DF8, 31, 30},
[U_OTP_CAV_Key6_ChkNum_Gold] = {0x003DFC, 31, 0},
[U_OTP_CAV_Key7_SCK_mapping] = {0x003E00, 15, 0},
[U_OTP_CAV_Key7_integrity_chk] = {0x003E00, 17, 16},
[U_OTP_CAV_Key7_allow_ChkNum] = {0x003E00, 19, 18},
[U_OTP_CAV_Key7_obfuscation] = {0x003E00, 21, 20},
[U_OTP_CAV_Key7_IdSel] = {0x003E00, 23, 22},
[U_OTP_CAV_Key7_KeySel] = {0x003E00, 29, 24},
[U_OTP_CAV_Key7_reserved] = {0x003E00, 31, 30},
[U_OTP_CAV_Key7_ChkNum_Gold] = {0x003E04, 31, 0},
[U_OTP_CAV_Key8_SCK_mapping] = {0x003E08, 15, 0},
[U_OTP_CAV_Key8_integrity_chk] = {0x003E08, 17, 16},
[U_OTP_CAV_Key8_allow_ChkNum] = {0x003E08, 19, 18},
[U_OTP_CAV_Key8_obfuscation] = {0x003E08, 21, 20},
[U_OTP_CAV_Key8_IdSel] = {0x003E08, 23, 22},
[U_OTP_CAV_Key8_KeySel] = {0x003E08, 29, 24},
[U_OTP_CAV_Key8_reserved] = {0x003E08, 31, 30},
[U_OTP_CAV_Key8_ChkNum_Gold] = {0x003E0C, 31, 0},
[U_OTP_ACPU_Property] = {0x003E10, 127, 0},
[U_OTP_SCPU_Property] = {0x003E20, 127, 0},
[U_OTP_CAV_Key1_Property] = {0x003E30, 127, 0},
[U_OTP_CAV_Key2_Property] = {0x003E40, 127, 0},
[U_OTP_CAV_Key3_Property] = {0x003E50, 127, 0},
[U_OTP_CAV_Key4_Property] = {0x003E60, 127, 0},
[U_OTP_CAV_Key5_Property] = {0x003E70, 127, 0},
[U_OTP_CAV_Key6_Property] = {0x003E80, 127, 0},
[U_OTP_CAV_Key7_Property] = {0x003E90, 127, 0},
[U_OTP_CAV_Key8_Property] = {0x003EA0, 127, 0},
[U_OTP_CAV_Key9_Property] = {0x003EB0, 127, 0},
[U_OTP_CAV_Key10_Property] = {0x003EC0, 127, 0},
[U_OTP_CAV_Key11_Property] = {0x003ED0, 127, 0},
[U_OTP_CAV_Key12_Property] = {0x003EE0, 127, 0},
[U_OTP_CAV_Key13_Property] = {0x003EF0, 127, 0},
[U_OTP_CAV_Key14_Property] = {0x003F00, 127, 0},
[U_OTP_CAV_Key15_Property] = {0x003F10, 127, 0},
[U_OTP_CAV_SecretKey1] = {0x003F20, 127, 0},
[U_OTP_CAV_SecretKey2] = {0x003F30, 127, 0},
[U_OTP_CAV_SecretKey3] = {0x003F40, 127, 0},
[U_OTP_CAV_SecretKey4] = {0x003F50, 127, 0},
[U_OTP_CAV_SecretKey5] = {0x003F60, 127, 0},
[U_OTP_CAV_SecretKey6] = {0x003F70, 127, 0},
[U_OTP_CAV_SecretKey7] = {0x003F80, 127, 0},
[U_OTP_CAV_SecretKey8] = {0x003F90, 127, 0},
[U_OTP_CA_PWD_integrity_chk] = {0x003FA0, 1, 0},
[U_OTP_Multi2SysKey_integrity_chk] = {0x003FA0, 3, 2},
[U_OTP_CA_PWD_Reserved0] = {0x003FA0, 7, 4},
[U_OTP_I2C_PWD_obfuscation] = {0x003FA0, 9, 8},
[U_OTP_EJTAG_PWD_obfuscation] = {0x003FA0, 11, 10},
[U_OTP_SCAN_PWD_obfuscation] = {0x003FA0, 13, 12},
[U_OTP_MBIST_PWD_obfuscation] = {0x003FA0, 15, 14},
[U_OTP_Multi2SysKey_CRC] = {0x003FA0, 31, 16},
[U_OTP_I2C_PWD_CRC] = {0x003FA4, 15, 0},
[U_OTP_EJTAG_PWD_CRC] = {0x003FA4, 31, 16},
[U_OTP_SCAN_PWD_CRC] = {0x003FA8, 15, 0},
[U_OTP_MBIST_PWD_CRC] = {0x003FA8, 31, 16},
[U_OTP_CA_PWD_Keysel] = {0x003FAC, 5, 0},
[U_OTP_ena_TP_PWD] = {0x003FAC, 7, 6},
[U_OTP_ena_KDF_I2C_PWD] = {0x003FAC, 9, 8},
[U_OTP_ena_KDF_EJTAG_PWD] = {0x003FAC, 11, 10},
[U_OTP_ena_KDF_SCAN_PWD] = {0x003FAC, 13, 12},
[U_OTP_ena_KDF_MBIST_PWD] = {0x003FAC, 15, 14},
[U_OTP_I2C_TP_ID] = {0x003FAC, 19, 16},
[U_OTP_EJTAG_TP_ID] = {0x003FAC, 23, 20},
[U_OTP_SCAN_TP_ID] = {0x003FAC, 27, 24},
[U_OTP_MBIST_TP_ID] = {0x003FAC, 31, 28},
[U_OTP_I2C_PWD] = {0x003FB0, 127, 0},
[U_OTP_EJTAG_PWD] = {0x003FC0, 127, 0},
[U_OTP_SCAN_PWD] = {0x003FD0, 127, 0},
[U_OTP_MBIST_PWD] = {0x003FE0, 127, 0},
[U_OTP_VtrackID] = {0x003FF0, 31, 0},
[U_OTP_VtrackKey] = {0x003FF4, 31, 0},
[U_OTP_VtrackID_select] = {0x003FF8, 0, 0},
[U_OTP_VtrackKey_select] = {0x003FF8, 1, 1},
[U_OTP_Vtrack_Reserved0] = {0x003FF8, 31, 2},
[U_OTP_FAKE] = {0x003FFC, 23, 0},
[U_OTP_allow_CPU_rst_release] = {0x003FFC, 31, 24},

};


MS_BOOL _HAL_CA_OTP_GetConfig(MS_U32 u32Idx, MS_U32 *pu32Value);
MS_BOOL _HAL_CA_OTP_SetConfig(MS_U32 u32Idx, MS_U32 u32Value);

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Inline Function
//--------------------------------------------------------------------------------------------------
void HAL_CA_SetBank(MS_U32 u32RegAddr)
{
    _u32REGBase = u32RegAddr;
    _u32REGPMBase = u32RegAddr - 0x200000UL;
    _u32REG_OTP_Base = u32RegAddr + REG_OTP_BASE;
    _u32REG_RSA_Base = u32RegAddr + REG_RSA_BASE;
}

MS_BOOL HAL_CA_Init(void)
{
    return TRUE;
}

void HAL_CA_BGC_ISR(void)
{
    MS_U32 u32Excp = REG32(BGC_RESP_FAIL);
//	    MS_U32 u32SecID = u32Excp & BGC_RESP_SECID;

    if((u32Excp & BGC_RESP_RESET_AV) == BGC_RESP_RESET_AV)
    {
        //AV reset
        HAL_CA_DEBUG("BGC: unexpected modification of protected area detected. Reset AV.");
    }

    if((u32Excp & BGC_RESP_RESET_ACPU) == BGC_RESP_RESET_ACPU)
    {
        //ACPU reset
        HAL_CA_DEBUG("BGC: unexpected modification of protected area detected. Reset ACPU.");
    }

    if((u32Excp & BGC_RESP_RESET_SCPU) == BGC_RESP_RESET_SCPU)
    {
        //SCPU reset
        HAL_CA_DEBUG("BGC: unexpected modification of protected area detected. Reset SCPU.");
    }

    if((u32Excp & BGC_RESP_RESET_SYSTEM) == BGC_RESP_RESET_SYSTEM)
    {
        //If error is reset system, run reset system(only in keres)
        //System reset
        HAL_CA_DEBUG("BGC: unexpected modification of protected area detected. Reset system.");
//	        REG32_PM(REG_TOP_SW_RST) = ((REG32_PM(REG_TOP_SW_RST) & ~REG_TOP_SW_RST_MSK) |  REG_TOP_SW_RST_PASSWD);
    }


}

void HAL_CA_BGC_DisableINT(void)
{
    REG32(REG_INTR_CPUINIT_HST3) = REG32(REG_INTR_CPUINIT_HST3) & (~REG_HST3TO2_INT);
}

MS_BOOL HAL_CA_BGC_LoadFW(MS_U32 u32FwAddr, MS_U32 u32FwSize)
{
    if(u32FwSize > 0x2000)
    {   //max size is 8K
        return FALSE;
    }

    if((REG32(REG_ONEWAY_42) & SEC51_SW_RST) == 0x0)//bit0, reset sec51
    {
        return TRUE;//already enable sec51
    }

    REG32(REG_BDMA_STATUS)     = REG32(REG_BDMA_STATUS) | 0x10; // triger bdma
    REG32(REG_BDMA_CTRL)       = 0x0;    //disable
    REG32(REG_BDMA_SRC_SEL)    = 0x0940; //MIU0 to Sec_51
    REG32(REG_BDMA_SRC_ADDR_L) = u32FwAddr & 0xFFFF;
    REG32(REG_BDMA_SRC_ADDR_H) = (u32FwAddr >> 16) & 0xFFFF;
    REG32(REG_BDMA_DST_ADDR_L) = 0x0;
    REG32(REG_BDMA_DST_ADDR_H) = 0x0;
    REG32(REG_BDMA_SIZE_L)     = u32FwSize & 0xFFFF; //size
    REG32(REG_BDMA_SIZE_H)     = 0x0;
    REG32(REG_BDMA_CTRL)       = 0x1; // triger bdma
    while((REG32(REG_BDMA_CTRL) & 0x1) != 0); //wait bdma done

    REG32(REG_BDMA_STATUS)     = REG32(REG_BDMA_STATUS) | 0x10; // triger bdma

    REG32(REG_ONEWAY_42)    = REG32(REG_ONEWAY_42) & (~SEC51_SW_RST);//bit 0, release sec51

    REG32(REG_ONEWAY_52)    = REG32(REG_ONEWAY_52) | SEC51_SW_LOCK;//bit 0, oneway lock sec51

    //enable MIU cross bar
    REG32(REG_MIU_CROSSBAR_CTRL)    = REG32(REG_MIU_CROSSBAR_CTRL) | REG_MIU_CROSSBAR_EN;
    return TRUE;
}

MS_BOOL HAL_CA_BGC_Ctrl(MS_U32 u32Cmd, MS_U32 u32Data)
{
    MS_U16 u16Cmd = ((u32Data << BGC_CMD_SEC_DATA_SHT) & BGC_CMD_SEC_DATA) |
                    ((u32Cmd  << BGC_CMD_SEC_TYPE_SHT) & BGC_CMD_SEC_TYPE);

    while((REG32(BGC_CMD_CTRL) & BGC_CMD_SEC_GO) == BGC_CMD_SEC_GO);

    REG32(BGC_CMD_CTRL) = u16Cmd;
    REG32(BGC_CMD_CTRL) = u16Cmd | BGC_CMD_SEC_GO;
//	    printf("[BGC Cmd]:%04X\n", u16Cmd);

    while((REG32(BGC_CMD_CTRL) & BGC_CMD_SEC_GO) == BGC_CMD_SEC_GO);
//	    printf("[BGC Cmd]:after write:%04X\n", REG32(BGC_CMD_CTRL));

    if((REG32(BGC_CMD_CTRL) & BGC_CMD_SEC_STATUS) == BGC_CMD_SEC_STATUS)
    {
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_CA_BGC_SetSec(MS_U32 u32SecId, const CA_BGC_SEC_CFG* pstSecCfg)
{
    MS_U32 i = 0;
    MS_U32 u32Fail = 0;

    if(u32SecId >= BGC_SEC_MAX )
    {
        return FALSE;
    }

    //set section id
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SECID, u32SecId) == FALSE)
    {
        u32Fail = 1;
        goto BGC_CONFIG_FAIL;
    }

    //set BGC mode
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_MODE, pstSecCfg->u32BGCMode == 1 ? BGC_SEC_MODE_STATIC : BGC_SEC_MODE_DYNAMIC) == FALSE)
    {
        u32Fail = 2;
        goto BGC_CONFIG_FAIL;
    }

    //set protect address and input cmd length
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_LENGTH, 0x4) == FALSE)
    {
        u32Fail = 3;
        goto BGC_CONFIG_FAIL;
    }

    if((HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_ADDR, (pstSecCfg->u32ProtectAddr & 0x000000FF)>>0) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_ADDR, (pstSecCfg->u32ProtectAddr & 0x0000FF00)>>8) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_ADDR, (pstSecCfg->u32ProtectAddr & 0x00FF0000)>>16) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_ADDR, (pstSecCfg->u32ProtectAddr & 0xFF000000)>>24) == FALSE))
    {
        u32Fail = 4;
        goto BGC_CONFIG_FAIL;
    }

    //set protect size and input cmd length
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_LENGTH, 0x4) == FALSE)
    {
        u32Fail = 5;
        goto BGC_CONFIG_FAIL;
    }
    if((HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SIZE, (pstSecCfg->u32ProtectSize & 0x000000FF)>>0) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SIZE, (pstSecCfg->u32ProtectSize & 0x0000FF00)>>8) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SIZE, (pstSecCfg->u32ProtectSize & 0x00FF0000)>>16) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SIZE, (pstSecCfg->u32ProtectSize & 0xFF000000)>>24) == FALSE))
    {
        u32Fail = 6;
        goto BGC_CONFIG_FAIL;
    }

    //set interval bwteen two BGC check and input cmd length
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_LENGTH, 0x2) == FALSE)
    {
        u32Fail = 7;
        goto BGC_CONFIG_FAIL;
    }
    if((HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_INTERVAL, (pstSecCfg->u16Interval & 0x00FF)>>0) == FALSE) |
       (HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_INTERVAL, (pstSecCfg->u16Interval & 0xFF00)>>8) == FALSE))
    {
        u32Fail = 8;
        goto BGC_CONFIG_FAIL;
    }


    //set golden and incput golden length
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_LENGTH, 0x20) == FALSE)
    {
        u32Fail = 9;
        goto BGC_CONFIG_FAIL;
    }

    for(i = 0 ; i < 0x20 ; i ++)
    {
        if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_GOLDEN, pstSecCfg->u8Golden[i]) == FALSE)
        {
            u32Fail = 10;
            goto BGC_CONFIG_FAIL;
        }
    }

    //set password and incput  length
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_LENGTH, 0x4) == FALSE)
    {
        u32Fail = 11;
        goto BGC_CONFIG_FAIL;
    }

    for(i = 0 ; i < 0x4 ; i ++)
    {
        if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SET_PASSWD, pstSecCfg->u8ResetPassword[i]) == FALSE)
        {
            u32Fail = 12;
            goto BGC_CONFIG_FAIL;
        }
    }

    //set algorithm to sha256
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_ALGO, BGC_SEC_ALGO_SHA256) == FALSE)
    {
        u32Fail = 13;
        goto BGC_CONFIG_FAIL;
    }

    //set the reset mode if BGC check fail
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_RESET_MODE, BGC_SEC_RESET_INTERRUPT | BGC_SEC_RESET_SYSTEM) == FALSE)
    {
        u32Fail = 14;
        goto BGC_CONFIG_FAIL;
    }

    //enable section id
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_ENABLE, TRUE) == FALSE)
    {
        u32Fail = 15;
        goto BGC_CONFIG_FAIL;
    }

    return TRUE;

BGC_CONFIG_FAIL:
    HAL_CA_DEBUG("BGC config fail:%08X\n", u32Fail);
    return FALSE;
}

MS_BOOL HAL_CA_BGC_ResetSec(MS_U32 u32SecId, const MS_U8* pu8RstPSWD)
{
    MS_U32 i = 0;
    MS_U32 u32Fail = 0;

    if((u32SecId >= BGC_SEC_MAX) || (pu8RstPSWD == NULL))
    {
        return FALSE;
    }

   //check bgc mode, if mode = 1, return fail
    //set section id
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_SECID, u32SecId) == FALSE)
    {
        u32Fail = 1;
        goto BGC_RESET_FAIL;
    }

    //set reset password and incput  length
    if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_LENGTH, 0x4) == FALSE)
    {
        u32Fail = 2;
        goto BGC_RESET_FAIL;
    }

    for(i = 0 ; i < 0x4 ; i ++)
    {
        if(HAL_CA_BGC_Ctrl(BGC_CMD_TYPE_RESET_PASSWD, pu8RstPSWD[i]) == FALSE)
        {
            u32Fail = 3;
            goto BGC_RESET_FAIL;
        }
    }
    return TRUE;

BGC_RESET_FAIL:
    HAL_CA_DEBUG("BGC reset fail:%08X\n", u32Fail);
    return FALSE;

}

MS_BOOL HAL_CA_OTP_IsBlank(MS_U32 addr)
{
    MS_U32 value;

    if(addr >= CA_OTP_SIZE)
        return 0;

    value = *(volatile MS_U32 *)(_u32REG_OTP_Base + addr);

    return value == 0;
}


MS_U32 _HAL_CA_OTP_Read_Real(MS_U32 addr, MS_BOOL bXor)
{
    MS_U32 value;

    if(addr >= CA_OTP_SIZE)
        return 0;

    value = *(volatile MS_U32 *)(_u32REG_OTP_Base + addr);

    return value;
}


MS_U32 HAL_CA_OTP_Read(MS_U32 addr)
{
    return _HAL_CA_OTP_Read_Real(addr, FALSE);
}


MS_BOOL _HAL_CA_OTP_Write_Real(MS_U32 addr, MS_U8 value)
{
    MS_DEBUG_MSG(printf("[_HAL_CA_OTP_Write_Real] addr=0x%04lx value=0x%02lx\n", addr, (MS_U32)value);)
#if 0
    printf("[_HAL_CA_OTP_Write_Real] NO REAL WRITE!!!\n");
#else
    *(volatile MS_U8 *)(_u32REG_OTP_Base + addr) = value;
#endif

    MS_U32 v;
    v = REG32(REG_OTP_CTRL_PV) & REG_OTP_CTRL_PG_VERIFY_FAIL_FLAG;
    if(v == 0) {
        return TRUE;
    }

    HAL_CA_DEBUG("[_HAL_CA_OTP_Write_Real] PV fail (0x%08x)\n", v);
    return FALSE;
}

MS_BOOL HAL_CA_OTP_Write(MS_U32 addr, MS_U32 value)
{
    int i;
    if(addr >= CA_OTP_SIZE)
        return FALSE;

    if(addr % 4 != 0)
        return FALSE;

    REG32(REG_OTP_CTRL_0) = REG_OTP_CTRL_0_VALUE;

    MS_U32 orig = _HAL_CA_OTP_Read_Real(addr, TRUE);
    MS_U32 write = value & (orig ^ value); // only write the bit from 0 to 1

    if(write == 0)
        return TRUE;

    for(i = 0; i < 4; i++) {
        if(_HAL_CA_OTP_Write_Real(addr + i, (value >> (i * 8)) & 0xff) == FALSE)
            return FALSE;
    }

    MS_U32 result = _HAL_CA_OTP_Read_Real(addr, TRUE);
    if((result & value) == value) // only check the bit 1
    {
        return TRUE;
    }

    return FALSE;
}

MS_BOOL _HAL_CA_OTP_GetConfig(MS_U32 u32Idx, MS_U32 *pu32Value)
{
    MS_U32 u32Addr  = _HAL_CA_OTP_Addr[u32Idx].u32Addr;
    MS_U32 u32Shift = _HAL_CA_OTP_Addr[u32Idx].u32Lo;
    MS_U32 u32Len   = _HAL_CA_OTP_Addr[u32Idx].u32Hi - _HAL_CA_OTP_Addr[u32Idx].u32Lo + 1;
    MS_U32 u32Mask  = bitmask(_HAL_CA_OTP_Addr[u32Idx].u32Hi, _HAL_CA_OTP_Addr[u32Idx].u32Lo);

    if(pu32Value == NULL)
        return FALSE;

    if(u32Len > 32)
        return FALSE;

    *pu32Value = (HAL_CA_OTP_Read(u32Addr) & u32Mask) >> u32Shift;

    return TRUE;
}

MS_BOOL _HAL_CA_OTP_SetConfig(MS_U32 u32Idx, MS_U32 u32Value)
{
    //printf("[_HAL_CA_OTP_SetConfig] u32Idx=%ld u32Value=0x%08lx\n", u32Idx, u32Value);

    MS_U32 u32Addr  = _HAL_CA_OTP_Addr[u32Idx].u32Addr;
    MS_U32 u32Shift = _HAL_CA_OTP_Addr[u32Idx].u32Lo;
    MS_U32 u32Len   = _HAL_CA_OTP_Addr[u32Idx].u32Hi - _HAL_CA_OTP_Addr[u32Idx].u32Lo + 1;
    MS_U32 u32Mask  = bitmask(_HAL_CA_OTP_Addr[u32Idx].u32Hi, _HAL_CA_OTP_Addr[u32Idx].u32Lo);

    if(u32Len > 32)
        return FALSE;

    u32Value = (u32Value << u32Shift) & u32Mask;

    return HAL_CA_OTP_Write(u32Addr, u32Value);
}

MS_BOOL HAL_CA_OTP_EnableSecureBoot(void)
{
    return _HAL_CA_OTP_SetConfig(U_OTP_SBoot, 0xf);
}

MS_BOOL HAL_CA_OTP_IsSecureBootEnabled(void)
{
    MS_U32 u32Value = 0;
    MS_BOOL ret = FALSE;

    ret = _HAL_CA_OTP_GetConfig(U_OTP_SBoot, &u32Value);

    if (ret == FALSE)
    {
        return FALSE;
    }
    return u32Value != 0;
}


MS_BOOL HAL_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType)
{
    //Kano not support
    MS_DEBUG_MSG(printf("[%s][%d] do not support Kano\n", __FUNCTION__, __LINE__));
    return FALSE;
}

MS_BOOL HAL_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *peLockType)
{
    //Kano not support
    MS_DEBUG_MSG(printf("[%s][%d] do not support Kano\n", __FUNCTION__, __LINE__));
    return FALSE;
}


//=======================================================
//          RSA bank for Secure Range
//=======================================================

MS_BOOL HAL_CA_RSA_SetSecureRange(MS_U32 u32SecSet, MS_U32 u32SecStart, MS_U32 u32SecEnd)
{
    if(u32SecSet > REG_RSA_SEC_RANGE_SET  || u32SecEnd <= u32SecStart)
        return FALSE;

    *(volatile MS_U32*) (_u32REGBase + REG_RSA_SEC_RANGE_START(u32SecSet)) = REG_RSA_SEC_RANGE_ENABLE | ( u32SecStart & REG_RSA_SEC_RANGE_MASK);
    *(volatile MS_U32*) (_u32REGBase + REG_RSA_SEC_RANGE_END(u32SecSet))   = REG_RSA_SEC_RANGE_ENABLE | ( u32SecEnd   & REG_RSA_SEC_RANGE_MASK);

    return TRUE;
}

MS_BOOL HAL_CA_OTP_Lock(CA_OTP_LOCK eLock)
{
    switch(eLock)
    {
        // Boot Area
        case E_BOOT_LockWord_W0000_0511_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_0000_0511_W, 0x3);
        case E_BOOT_LockWord_W0512_1023_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_0512_1023_W, 0x3);
        case E_BOOT_LockWord_W1024_1535_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_1024_1535_W, 0x3);
        case E_BOOT_LockWord_W1536_2047_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_1536_2047_W, 0x3);
        case E_BOOT_LockWord_W2048_2559_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_2048_2559_W, 0x3);
        case E_BOOT_LockWord_W2560_3071_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_2560_3071_W, 0x3);
        case E_BOOT_LockWord_W3072_3391_Addr:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_3072_3391_W, 0x3);

        // IDs
        case E_LockWord_v_PubOtpOID:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_v_PubOtpOID_W, 0x3);
        case E_LockWord_v_PubOtpVID:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_v_PubOtpVID_W, 0x3);
        case E_LockWord_v_pubOtpUniqueID1:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_PubOTPUniqueID1_W, 0x3);
        case E_LockWord_v_pubOtpUniqueID2:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_PubOTPUniqueID2_W, 0x3);
        case E_LockWord_v_pubOtpUniqueID3:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_PubOTPUniqueID3_W, 0x3);
        case E_LockWord_v_pubOtpUniqueID4:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_PubOTPUniqueID4_W, 0x3);
        case E_LockWord_CA_MSID:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_MSID_W, 0x3);

        //CA_SecretKey
        case E_LockWord_CAV_SecretKey6_W:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CAV_Key6_W, 0x3);
        case E_LockWord_CAV_SecretKey6_R:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CAV_Key6_R, 0x3);

        case E_LockWord_RSA_1_W:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_RSA_1_W, 0x3);

        //Debug ports
        case E_LockWord_Debugport_PWD_W:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CA_PWD_W, 0x3);
        case E_LockWord_Debugport_PWD_R:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CA_PWD_R, 0x3);

    // Reserved
        case E_LockWord_CA_reserved0:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CA_reserved0_W, 0x3);
        case E_LockWord_CA_reserved1:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CA_reserved1_W, 0x3);
        case E_LockWord_CA_reserved2:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CA_reserved2_W, 0x3);
        case E_LockWord_CA_reserved3:
            return _HAL_CA_OTP_SetConfig(U_OTP_LockWord_CA_reserved3_W, 0x3);

        default:
            return FALSE;
    }
}

MS_BOOL HAL_CA_Locked(CA_OTP_LOCK eLock)
{
    MS_U32 u32Value = 0;
    MS_BOOL ret = FALSE;
    switch(eLock)
    {
        // Boot Area
        case E_BOOT_LockWord_W0000_0511_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_0000_0511_W, &u32Value);
            break;
        case E_BOOT_LockWord_W0512_1023_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_0512_1023_W, &u32Value);
            break;
        case E_BOOT_LockWord_W1024_1535_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_1024_1535_W, &u32Value);
            break;
        case E_BOOT_LockWord_W1536_2047_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_1536_2047_W, &u32Value);
            break;
        case E_BOOT_LockWord_W2048_2559_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_2048_2559_W, &u32Value);
            break;
        case E_BOOT_LockWord_W2560_3071_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_2560_3071_W, &u32Value);
            break;
        case E_BOOT_LockWord_W3072_3391_Addr:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_3072_3391_W, &u32Value);
            break;

        // IDs
        case E_LockWord_v_PubOtpOID:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_v_PubOtpOID_W, &u32Value);
            break;
        case E_LockWord_v_PubOtpVID:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_v_PubOtpVID_W, &u32Value);
            break;
        case E_LockWord_v_pubOtpUniqueID1:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_PubOTPUniqueID1_W, &u32Value);
            break;
        case E_LockWord_v_pubOtpUniqueID2:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_PubOTPUniqueID2_W, &u32Value);
            break;
        case E_LockWord_v_pubOtpUniqueID3:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_PubOTPUniqueID3_W, &u32Value);
            break;
        case E_LockWord_v_pubOtpUniqueID4:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_PubOTPUniqueID4_W, &u32Value);
            break;
        case E_LockWord_CA_MSID:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_MSID_W, &u32Value);
            break;

        //CA_SecretKey
        case E_LockWord_CAV_SecretKey6_W:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CAV_Key6_W, &u32Value);
            break;
        case E_LockWord_CAV_SecretKey6_R:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CAV_Key6_R, &u32Value);
            break;

        case E_LockWord_RSA_1_W:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_RSA_1_W, &u32Value);
            break;

        //Debug ports
        case E_LockWord_Debugport_PWD_W:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CA_PWD_W, &u32Value);
            break;
        case E_LockWord_Debugport_PWD_R:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CA_PWD_R, &u32Value);
            break;

        // Reserved
            break;
        case E_LockWord_CA_reserved0:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CA_reserved0_W, &u32Value);
            break;
        case E_LockWord_CA_reserved1:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CA_reserved1_W, &u32Value);
            break;
        case E_LockWord_CA_reserved2:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CA_reserved2_W, &u32Value);
            break;
        case E_LockWord_CA_reserved3:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_LockWord_CA_reserved3_W, &u32Value);
            break;

        default:
            ret = FALSE;
    }

    if(ret == FALSE)
        return FALSE;

    if(u32Value == 0x01) // 0x01 : unlock, xx : locked
        return FALSE;

    return TRUE;
}

MS_BOOL HAL_CA_OTP_SetRSAextID(MS_U32 u32Value)
{
    return _HAL_CA_OTP_SetConfig(U_OTP_MSID, u32Value);
}

MS_U32 HAL_CA_OTP_GetRSAextID(void)
{

    MS_U32 u32Value = 0;
    MS_BOOL ret = FALSE;

    ret = _HAL_CA_OTP_GetConfig(U_OTP_MSID, &u32Value);

    if (ret == FALSE)
    {
        return FALSE;
    }
    return u32Value;
}


MS_BOOL HAL_CA_OTP_SetHASH_REF_VER_Ex(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH1_REF_VER, MS_U32 u32HASH2_REF_VER)
{
    if(_HAL_CA_OTP_SetConfig(U_OTP_HASH0_VER_REF, u32HASH0_REF_VER) == FALSE)
    {
        return FALSE;
    }

    if(_HAL_CA_OTP_SetConfig(U_OTP_HASH1_VER_REF, u32HASH1_REF_VER) == FALSE)
    {
        return FALSE;
    }

    if(_HAL_CA_OTP_SetConfig(U_OTP_HASH2_VER_REF, u32HASH2_REF_VER) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_GetHASH_REF_VER_Ex(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH1_REF_VER, MS_U32 *pu32HASH2_REF_VER)
{
    MS_U32 u32Value = 0;

    if(pu32HASH0_REF_VER != NULL)
    {
        if(_HAL_CA_OTP_GetConfig(U_OTP_HASH0_VER_REF, &u32Value) == FALSE)
        {
            return FALSE;
        }

        *pu32HASH0_REF_VER = (u32Value);
    }

    if(pu32HASH1_REF_VER != NULL)
    {
        if(_HAL_CA_OTP_GetConfig(U_OTP_HASH1_VER_REF, &u32Value) == FALSE)
        {
            return FALSE;
        }
        *pu32HASH1_REF_VER = (u32Value);
    }

    if(pu32HASH2_REF_VER != NULL)
    {
        if(_HAL_CA_OTP_GetConfig(U_OTP_HASH2_VER_REF, &u32Value) == FALSE)
        {
            return FALSE;
        }

        *pu32HASH2_REF_VER = (u32Value);
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER)
{
    if(HAL_CA_OTP_SetHASH_REF_VER_Ex(0, u32HASH1_REF_VER, 0) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER)
{
    if(HAL_CA_OTP_GetHASH_REF_VER_Ex(NULL, pu32HASH1_REF_VER, NULL) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}


MS_BOOL HAL_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER)
{
    if(HAL_CA_OTP_SetHASH_REF_VER_Ex(u32HASH0_REF_VER, 0, u32HASH2_REF_VER) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER)
{
    if(HAL_CA_OTP_GetHASH_REF_VER_Ex(pu32HASH0_REF_VER,  NULL, pu32HASH2_REF_VER) == FALSE)
    {
        return FALSE;
    }

    return TRUE;
}


MS_U32 HAL_CA_MaxDeviceIdSize(void)
{
    return MAX_DEVICEID_SIZE;
}

MS_BOOL HAL_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size)
{
    MS_U32 did[8] = {0};
    MS_U32 i, u32Addr;
    MS_U32 Did_Size = 2;
    MS_U8 *p = (MS_U8 *)did;

    for(i = 0; i < sizeof(did) && i < u32Size; i++)
    {
        p[i] = pu8Did[i];
    }

    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID1].u32Addr;
    for(i = 0; i < Did_Size; i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,did[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID2].u32Addr;
    for(i = Did_Size; i < (2*Did_Size); i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,did[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID3].u32Addr;
    for(i = (2*Did_Size); i < (3*Did_Size); i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,did[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID4].u32Addr;
    for(i = (3*Did_Size); i < (4*Did_Size); i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,did[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size)
{
    MS_U32 did[8] = {0};
    MS_U32 Did_Size = 2;
    MS_U32 i, u32Addr;
    MS_U8 *p = (MS_U8 *)did;

    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID1].u32Addr;
    for(i = 0; i < Did_Size; i++)
    {
        did[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID2].u32Addr;
    for(i = Did_Size; i < (2*Did_Size) ; i++)
    {
        did[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID3].u32Addr;
    for(i = (2*Did_Size) ; i < (3*Did_Size) ; i++)
    {
        did[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_v_pubOtpUniqueID4].u32Addr;
    for(i = (3*Did_Size); i < (4*Did_Size) ; i++)
    {
        did[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }

    for(i = 0; i < sizeof(did) && i < u32Size; i++)
    {
        pu8Did[i] = p[i];
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode)
{
    MS_U32 idx = 0;
    MS_U32 v = 0;

    switch(eDebugPort)
    {
        case E_CA_DEBUG_PORT_I2C:
            idx = U_OTP_I2C_MODE;
            break;
        case E_CA_DEBUG_PORT_JTAG:
            idx = U_OTP_EJTAG_MODE;
            break;
        case E_CA_DEBUG_PORT_SCAN:
            idx = U_OTP_SCAN_MODE;
            break;
        default:
            return FALSE;
    }

    switch(eMode)
    {
        case E_CA_DEBUG_PORT_OPEN:
            return TRUE;
        case E_CA_DEBUG_PORT_PASSWD:
            v = 0x7;
            break;
        case E_CA_DEBUG_PORT_CLOSE:
            v = 0x3f;
            break;
        default:
            return FALSE;
    }

    return _HAL_CA_OTP_SetConfig(idx, v);

}


MS_BOOL HAL_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode)
{
    MS_U32 v = 0;
    MS_BOOL ret = FALSE;

    if(eMode == NULL)
    {
        return FALSE;
    }

    switch(eDebugPort)
    {
        case E_CA_DEBUG_PORT_I2C:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_I2C_MODE, &v);
            break;
        case E_CA_DEBUG_PORT_JTAG:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_EJTAG_MODE, &v);
            break;
        case E_CA_DEBUG_PORT_SCAN:
            ret = _HAL_CA_OTP_GetConfig(U_OTP_SCAN_MODE, &v);
            break;
        default:
            return FALSE;
    }

    if (ret == FALSE)
    {
        return FALSE;
    }

    if(v & 0x38)
    {
        *eMode = E_CA_DEBUG_PORT_CLOSE;
    }
    else if(v)
    {
        *eMode = E_CA_DEBUG_PORT_PASSWD;
    }
    else
    {
        *eMode = E_CA_DEBUG_PORT_OPEN;
    }

    return TRUE;
}


//	This function is created to used for another hal driver: NSK2
MS_BOOL HAL_CA_OTP_GetCfg(MS_U32 u32Idx,MS_U32 * pu32Value)
{
	return _HAL_CA_OTP_GetConfig(u32Idx, pu32Value);
}

MS_BOOL HAL_CA_OTP_EnableSecureCWMode(void)
{
    return _HAL_CA_OTP_SetConfig(U_OTP_ena_ACPU2KT, 0xF);
}

MS_BOOL HAL_CA_OTP_IsSecureCWMode(void)
{
    MS_U32 u32Value = 0;
    MS_BOOL ret = FALSE;

    ret = _HAL_CA_OTP_GetConfig(U_OTP_ena_ACPU2KT, &u32Value);

    if (ret == FALSE)
    {
        return FALSE;
    }

    if(u32Value == 1)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

MS_U16 _HAL_CA_Random(void)
{
    MS_U32 n;

    do{
        n = REG32(REG_RNG_TRNG);
        if( n & REG_RNG_TRNG_VALID_MASK) //check valid
        {
            n &= REG_RNG_TRNG_OUT_MASK; // get random number
            REG32(REG_RNG_TRNG) = REG_RNG_TRNG_ACK; //ack
            break;
        }
    }while(1);

    return n;
}


MS_U32 HAL_CA_Random(void)
{
    MS_U32        u32Val    = 0;

    u32Val += _HAL_CA_Random()<<16;
    u32Val += _HAL_CA_Random();

    return u32Val;

}

MS_U32 HAL_CA_OTP_ReadCAVendor(void)
{
    MS_U32 u32CAV_Key_VendorID[CA_SCK_NUM] = {0}; // Store all VendorID
    MS_U32 u32CAV_Key_VendorID_Hi = 24;  //High bit of VendorID in Key property
    MS_U32 u32CAV_Key_VendorID_Lo = 20; //Low bit of VendorID in Key property

    MS_U32 u32CAV1_VendorID = 0;
    MS_U32 u32CAV2_VendorID = 0;
    MS_U32 u32CAV_Vendor = 0;
    MS_U32 u32CAV_Vendor_Shift = 16;
    MS_U32 u32CAV_Vendor_Num = 0; // Calculate how many different VendorID

    MS_U32 u32Idx = 0;
    MS_U32 u32Addr  = 0;
    MS_U32 u32Shift = 0;
    MS_U32 u32Mask  = 0;

    // Read VendorID of U_OTP_CAV_Keyn_Property
    for(u32Idx = 0; u32Idx < CA_SCK_NUM; u32Idx++)
    {
        u32Addr  = _HAL_CA_OTP_Addr[u32Idx + U_OTP_CAV_Key1_Property].u32Addr;
        u32Shift = u32CAV_Key_VendorID_Lo;
        u32Mask  = bitmask(u32CAV_Key_VendorID_Hi, u32CAV_Key_VendorID_Lo);

        u32CAV_Key_VendorID[u32Idx] = (HAL_CA_OTP_Read(u32Addr) & u32Mask) >> u32Shift;
    }

    // Assign VendorID
    for(u32Idx = 0; u32Idx < CA_SCK_NUM; u32Idx++)
    {
        // First non NULL VendorID assign to u32CAV1_VendorID
        if( (0 == u32CAV_Vendor_Num) && (CA_OTP_VENDOR_ID_NULL != u32CAV_Key_VendorID[u32Idx]) )
        {
            u32CAV1_VendorID = u32CAV_Key_VendorID[u32Idx];
            u32CAV_Vendor_Num++;
        }
        // Second non NULL and not u32CAV1_VendorID VendorID assign to u32CAV2_VendorID
        else if( (1 == u32CAV_Vendor_Num) && (CA_OTP_VENDOR_ID_NULL != u32CAV_Key_VendorID[u32Idx]) )
        {
            if(u32CAV1_VendorID != u32CAV_Key_VendorID[u32Idx])
            {
                u32CAV2_VendorID = u32CAV_Key_VendorID[u32Idx];
                u32CAV_Vendor_Num++;
            }
        }
        // If third non NULL, not u32CAV1_VendorID and not u32CAV2_VendorID VendorID
        else if( (2 <= u32CAV_Vendor_Num) && (CA_OTP_VENDOR_ID_NULL != u32CAV_Key_VendorID[u32Idx]) )
        {
            if( (u32CAV1_VendorID != u32CAV_Key_VendorID[u32Idx]) && (u32CAV2_VendorID != u32CAV_Key_VendorID[u32Idx]) )
            {
                u32CAV1_VendorID = CA_OTP_VENDOR_ID_UNKNOW;
                u32CAV2_VendorID = CA_OTP_VENDOR_ID_UNKNOW;
                u32CAV_Vendor_Num++;
            }

        }
    }

    // Transform VendorID to Vendor code
    switch(u32CAV1_VendorID)
    {
        case CA_OTP_VENDOR_ID_NULL:
            break;
        case CA_OTP_VENDOR_ID_NDS:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_NDS;
            break;
        case CA_OTP_VENDOR_ID_NAGRA:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_NAGRA;
            break;
        case CA_OTP_VENDOR_ID_VIACCESS:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_VIACCESS;
            break;
        case CA_OTP_VENDOR_ID_IRDETO:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_IRDETO;
            break;
        case CA_OTP_VENDOR_ID_VERIMATRIX:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_VERIMATRIX;
            break;
        case CA_OTP_VENDOR_ID_SMI:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_SMI;
            break;
        case CA_OTP_VENDOR_ID_CONAX:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_CONAX;
            break;
        case CA_OTP_VENDOR_ID_LATENS:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_LATENS;
            break;
        case CA_OTP_VENDOR_ID_ECHOSTAR:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_ECHOSTAR;
            break;
        default:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_UNKNOW;
    }

    switch(u32CAV2_VendorID)
    {
        case CA_OTP_VENDOR_ID_NULL:
            break;
        case CA_OTP_VENDOR_ID_NDS:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_NDS << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_NAGRA:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_NAGRA << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_VIACCESS:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_VIACCESS << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_IRDETO:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_IRDETO << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_VERIMATRIX:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_VERIMATRIX << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_SMI:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_SMI << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_CONAX:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_CONAX << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_LATENS:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_LATENS << u32CAV_Vendor_Shift;
            break;
        case CA_OTP_VENDOR_ID_ECHOSTAR:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_ECHOSTAR << u32CAV_Vendor_Shift;
            break;
        default:
            u32CAV_Vendor = u32CAV_Vendor | E_CA_VENDOR_UNKNOW << u32CAV_Vendor_Shift;
    }

   return u32CAV_Vendor;
}

void HAL_CA_STR_SetBank(MS_U32 u32Bank)
{
    _u32REGPMBase = u32Bank;

    return;
}

void HAL_CA_STR_EnableKey(void)
{
    MS_U16 u16Tmp = 0;
    u16Tmp = REG16_PM(REG_STR_KEY);
    u16Tmp |= REG_STR_DMA_KEY_VALID;
    REG16_PM(REG_STR_KEY) = u16Tmp;

    return;
}

void HAL_CA_STR_SetMAC(MS_U8* pu8STRMAC, MS_U32 u32STRMACSize)
{
    MS_U8 i = 0;

    for(i = 0; i < (u32STRMACSize/2); i++) {
        REG16_PM(REG_STR_AUTH_RESULT + i*4) = (pu8STRMAC[i*2 + 1]<<8 |pu8STRMAC[i*2]);
    }

    return;
}

void HAL_CA_STR_SetParamAddr(MS_U8 *pu8BAddr)
{
    MS_U16 u16Tmp = 0;

    u16Tmp = ((MS_U32)pu8BAddr & 0x0000FFFF);
    REG16_PM(REG_STR_PTR) = u16Tmp;

    u16Tmp = ((MS_U32)pu8BAddr & 0xFFFF0000)>>16;
    REG16_PM(REG_STR_PTR + 4) = u16Tmp;

    return;
}

void HAL_CA_STR_EnableWarmBoot(void)
{
    //Oneway
    REG16_PM(REG_WARM_BOOT_IND) = REG_WARM_BOOT_IND_MAGIC;

    return;
}
MS_BOOL HAL_CA_OTP_SetSecretKey(CA_SECRET_KEY eKeySel, const MS_U8 *pu8SecretKey, MS_U32 u32Size)
{

    MS_U32 secret_key[4] = {0};
    MS_U32 i, j, u32Addr;
    MS_U8 *p = (MS_U8 *)secret_key;
    switch(eKeySel)
    {
    case E_CA_SECRET_KEY1:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey1].u32Addr;
        break;
    case E_CA_SECRET_KEY2:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey2].u32Addr;
        break;
    case E_CA_SECRET_KEY3:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey3].u32Addr;
        break;
    case E_CA_SECRET_KEY4:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey4].u32Addr;
        break;
    case E_CA_SECRET_KEY5:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey5].u32Addr;
        break;
    case E_CA_SECRET_KEY6:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey6].u32Addr;
        break;
    case E_CA_SECRET_KEY7:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey7].u32Addr;
         break;
    case E_CA_SECRET_KEY8:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey8].u32Addr;
        break;
    default:
        return FALSE;
    }

    if(sizeof(secret_key)>u32Size)
        {
        j = u32Size;
        }
    else
        {
        j=sizeof(secret_key);
        }
     for(i = 0; i < sizeof(secret_key) && i < u32Size; i++)
    {
        p[i] = pu8SecretKey[(j-1)-i];            // Array reverse,  for user easy viewing
    }

    for(i = 0; i < 4; i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,secret_key[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    return TRUE;
}

MS_BOOL HAL_CA_OTP_GetSecretKey(CA_SECRET_KEY eKeySel, MS_U8 *pu8SecretKey, MS_U32 u32Size)
{
    MS_U32 secret_key[4] = {0};
    MS_U32 i, j, u32Addr;
    MS_U8 *p = (MS_U8 *)secret_key;

    switch(eKeySel)
    {
    case E_CA_SECRET_KEY1:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey1].u32Addr;
        break;
    case E_CA_SECRET_KEY2:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey2].u32Addr;
        break;
    case E_CA_SECRET_KEY3:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey3].u32Addr;
        break;
    case E_CA_SECRET_KEY4:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey4].u32Addr;
        break;
    case E_CA_SECRET_KEY5:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey5].u32Addr;
        break;
    case E_CA_SECRET_KEY6:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey6].u32Addr;
        break;
    case E_CA_SECRET_KEY7:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey7].u32Addr;
         break;
    case E_CA_SECRET_KEY8:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_CAV_SecretKey8].u32Addr;
        break;
    default:
        return FALSE;
        }
    for(i = 0; i < 4; i++)
    {
        secret_key[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }

    if(sizeof(secret_key)>u32Size)
        {
        j = u32Size;
        }
    else
        {
        j=sizeof(secret_key);
        }

    for(i = 0;  i < sizeof(secret_key) && i < u32Size; i++)
    {
        pu8SecretKey[i] = p[(j-1)-i];       // Array reverse,  for user easy viewing
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_SetRSAKey(CA_RSA_KEY eKeySel, const MS_U8 *pu8RSAKey, MS_U32 u32Size)
{
    MS_U32 RSA_key[64] = {0};
    MS_U32 i, u32Addr;
    MS_U8 *p = (MS_U8 *)RSA_key;

    switch(eKeySel)
    {
        case E_CA_RSA_KEY0:
            return FALSE;
        case E_CA_RSA_KEY1:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_RSA_1].u32Addr;
            break;
        default:
            return FALSE;
    }

     for(i = 0; i < sizeof(RSA_key) && i < u32Size; i++)
    {
        p[i] = pu8RSAKey[i];
    }
    for(i = 0; i < 64; i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,RSA_key[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }

    return TRUE;
}
MS_BOOL HAL_CA_OTP_GetRSAKey(CA_RSA_KEY eKeySel, MS_U8 *pu8RSAKey, MS_U32 u32Size)
{
    MS_U32 RSA_key[64] = {0};
    MS_U32 i, u32Addr;
    MS_U8 *p = (MS_U8 *)RSA_key;

    switch(eKeySel)
    {
        case E_CA_RSA_KEY0:
            return FALSE;
            break;
        case E_CA_RSA_KEY1:
        u32Addr = _HAL_CA_OTP_Addr[U_OTP_RSA_1].u32Addr;
            break;
        default:
            return FALSE;
    }
        for(i = 0; i < 64; i++)
    {
        RSA_key[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    for(i = 0; i < sizeof(RSA_key) && i < u32Size; i++)
    {
        pu8RSAKey[i] = p[i];
    }

    return TRUE;
}


MS_BOOL HAL_CA_OTP_SetDebugPortPassword(CA_DEBUG_PORT eDebugPort, const MS_U8 *pu8Password, MS_U32 u32Size)
{

    MS_U32 pwd[4] = {0};
    MS_U32 i, j, u32Addr;
    MS_U8 *p = (MS_U8 *)pwd;

    switch(eDebugPort)
    {
        case E_CA_DEBUG_PORT_I2C:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_I2C_PWD].u32Addr;
            break;
        case E_CA_DEBUG_PORT_JTAG:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_EJTAG_PWD].u32Addr;
            break;
        case E_CA_DEBUG_PORT_SCAN:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_SCAN_PWD].u32Addr;
            break;
        default:
            return FALSE;
    }
    if(sizeof(pwd)>u32Size)
        {
        j = u32Size;
        }
    else
        {
        j=sizeof(pwd);
        }

    for(i = 0;  i < sizeof(pwd) && i < u32Size; i++)
    {
        p[i] = pu8Password[(j-1)-i];        // Array reverse,  for user easy viewing
    }


    for(i = 0; i < 4; i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,pwd[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }

    return TRUE;
}

MS_BOOL HAL_CA_OTP_GetDebugPortPassword(CA_DEBUG_PORT eDebugPort, MS_U8 *pu8Password, MS_U32 u32Size)
{
    MS_U32 pwd[4] = {0};
    MS_U32 i, j, u32Addr;
    MS_U8 *p = (MS_U8 *)pwd;

    switch(eDebugPort)
    {
        case E_CA_DEBUG_PORT_I2C:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_I2C_PWD].u32Addr;
            break;
        case E_CA_DEBUG_PORT_JTAG:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_EJTAG_PWD].u32Addr;
            break;
        case E_CA_DEBUG_PORT_SCAN:
            u32Addr = _HAL_CA_OTP_Addr[U_OTP_SCAN_PWD].u32Addr;
            break;
        default:
            return FALSE;
    }
    for(i = 0; i < 4; i++)
    {
        pwd[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    if(sizeof(pwd)>u32Size)
        {
        j = u32Size;
        }
    else
        {
        j=sizeof(pwd);
        }
    for(i = 0;  i < sizeof(pwd) &&i < u32Size; i++)
    {
        pu8Password[i] = p[(j-1)-i];      // Array reverse,  for user easy viewing
    }
    return TRUE;
}

MS_U32 HAL_CA_MaxReservedSize(void)
{
    HAL_CA_DEBUG("Not Implement\n");
    return FALSE;
}

MS_BOOL HAL_CA_OTP_WriteReserved(const MS_U8 *pu8RData, MS_U32 u32Size)
{
    MS_U32 u32rdata[32] = {0};
    MS_U32 i, u32Addr;
    MS_U32 u32ReservedSize = MAX_RESERVED_SIZE /16 ;
    MS_U8 *p = (MS_U8 *)u32rdata;

    for(i = 0; i < sizeof(u32rdata) && i < u32Size; i++)
    {
        p[i] = pu8RData[i];
    }

    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved0].u32Addr;
    for(i = 0; i <u32ReservedSize; i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,u32rdata[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }

    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved1].u32Addr;
    for(i = u32ReservedSize; i < (2*u32ReservedSize); i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,u32rdata[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved2].u32Addr;
    for(i = (2*u32ReservedSize); i < (3*u32ReservedSize); i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,u32rdata[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved3].u32Addr;
    for(i = (3*u32ReservedSize); i < (4*u32ReservedSize); i++)
    {
        if(HAL_CA_OTP_Write(u32Addr,u32rdata[i]) == FALSE)
        {
            return FALSE;
        }
        u32Addr += 4;
    }
    return TRUE;
}

MS_BOOL HAL_CA_OTP_ReadReserved(MS_U8 *pu8RData, MS_U32 u32Size)
{
    MS_U32 u32rdata[32] = {0};
    MS_U32 i, u32Addr;
    MS_U32 u32ReservedSize = MAX_RESERVED_SIZE /16 ;
    MS_U8 *p = (MS_U8 *)u32rdata;

    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved0].u32Addr;
    for(i = 0; i < u32ReservedSize; i++)
    {
        u32rdata[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved1].u32Addr;
    for(i = u32ReservedSize; i < (2*u32ReservedSize); i++)
    {
        u32rdata[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved2].u32Addr;
    for(i = (2*u32ReservedSize); i < (3*u32ReservedSize); i++)
    {
        u32rdata[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }
    u32Addr = _HAL_CA_OTP_Addr[U_OTP_CA_reserved3].u32Addr;
    for(i = (3*u32ReservedSize); i < (4*u32ReservedSize); i++)
    {
        u32rdata[i] = HAL_CA_OTP_Read(u32Addr);
        u32Addr += 4;
    }

    for(i = 0; i < sizeof(u32rdata) && i < u32Size; i++)
    {
        pu8RData[i] = p[i];
    }

    return TRUE;
}
