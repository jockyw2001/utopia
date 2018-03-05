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
// file   halCA.h
// @brief  CA HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_CA_H__
#define __HAL_CA_H__

#include "regCA.h"
//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define CA_OTP_SIZE           0x4000UL
#define CA_RETRY_TIMES        1UL
#define BGC_IRQ    E_INT_FIQ_SECURE51_TO_BEON
#define CA_SCK_NUM            15UL

//Vendor ID
#define CA_OTP_VENDOR_ID_NULL       0x00UL
#define CA_OTP_VENDOR_ID_NDS        0x01UL
#define CA_OTP_VENDOR_ID_NAGRA      0x02UL
#define CA_OTP_VENDOR_ID_VIACCESS   0x03UL
#define CA_OTP_VENDOR_ID_IRDETO     0x04UL
#define CA_OTP_VENDOR_ID_VERIMATRIX 0x05UL
#define CA_OTP_VENDOR_ID_SMI        0x06UL
#define CA_OTP_VENDOR_ID_CONAX      0x07UL
#define CA_OTP_VENDOR_ID_LATENS     0x08UL
#define CA_OTP_VENDOR_ID_ECHOSTAR   0x09UL
#define CA_OTP_VENDOR_ID_UNKNOW     0xFFUL

//--------------------------------------------------------------------------------------------------
//  CA Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////
void HAL_CA_SetBank(MS_U32 u32RegAddr);
MS_BOOL HAL_CA_Init(void);

MS_BOOL HAL_CA_BGC_LoadFW(MS_U32 u32FwAddr, MS_U32 u32FwSize);
MS_BOOL HAL_CA_BGC_SetSec(MS_U32 u32SecId, const CA_BGC_SEC_CFG* pstSecCfg);
MS_BOOL HAL_CA_BGC_ResetSec(MS_U32 u32SecId, const MS_U8* pu8RstPSWD);
void    HAL_CA_BGC_ISR(void);
void    HAL_CA_BGC_DisableINT(void);

MS_U32 HAL_CA_MaxDeviceIdSize(void);
MS_U32 HAL_CA_MaxReservedSize(void);

MS_BOOL HAL_CA_OTP_EnableSecureBoot(void);
MS_BOOL HAL_CA_OTP_IsSecureBootEnabled(void);

MS_BOOL HAL_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType);
MS_BOOL HAL_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *peLockType);

MS_BOOL HAL_CA_OTP_IsBlank(MS_U32 addr);
MS_U32 HAL_CA_OTP_Read(MS_U32 addr);
MS_BOOL HAL_CA_OTP_Write(MS_U32 addr, MS_U32 value);

MS_BOOL HAL_CA_OTP_Lock(CA_OTP_LOCK eLock);
MS_BOOL HAL_CA_Locked(CA_OTP_LOCK eLock);

MS_BOOL HAL_CA_OTP_SetRSAextID(MS_U32 u32Value);
MS_U32 HAL_CA_OTP_GetRSAextID(void);

MS_BOOL HAL_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER);
MS_BOOL HAL_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER);
MS_BOOL HAL_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER);
MS_BOOL HAL_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER);

MS_BOOL HAL_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode);
MS_BOOL HAL_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode);

MS_BOOL HAL_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_SetSecretKey(CA_SECRET_KEY eKeySel, const MS_U8 *pu8SecretKey, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_GetSecretKey(CA_SECRET_KEY eKeySel, MS_U8 *pu8SecretKey, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_SetRSAKey(CA_RSA_KEY eKeySel, const MS_U8 *pu8RSAKey, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_GetRSAKey(CA_RSA_KEY eKeySel, MS_U8 *pu8RSAKey, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_SetDebugPortPassword(CA_DEBUG_PORT eDebugPort, const MS_U8 *pu8Password, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_GetDebugPortPassword(CA_DEBUG_PORT eDebugPort, MS_U8 *pu8Password, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_WriteReserved(const MS_U8 *pu8RData, MS_U32 u32Size);
MS_BOOL HAL_CA_OTP_ReadReserved(MS_U8 *pu8RData, MS_U32 u32Size);

MS_BOOL HAL_CA_OTP_GetCfg(MS_U32 u32Idx,MS_U32 * pu32Value);

MS_BOOL HAL_CA_OTP_EnableSecureCWMode(void);
MS_BOOL HAL_CA_OTP_IsSecureCWMode(void);

MS_U32 HAL_CA_Random(void);
MS_U32 HAL_CA_OTP_ReadCAVendor(void);

void HAL_CA_STR_SetBank(MS_U32 u32Bank);
void HAL_CA_STR_EnableKey(void);
void HAL_CA_STR_SetParamAddr(MS_U8 *pu8BAddr);
void HAL_CA_STR_SetMAC(MS_U8* pu8STRMAC, MS_U32 u32STRMACSize);
void HAL_CA_STR_EnableWarmBoot(void);

////////////////////////////////////////////////
// OTP configuratoins
////////////////////////////////////////////////
//
// CAV1: for CAVid = 4'd1
// CAV4: for CAVid = 4'd4
// CAV10: for CAVid = 4'd10
//
typedef enum {
    U_OTP_CA_reserved0,
    U_OTP_CA_reserved1,
    U_OTP_CA_reserved2,
    U_OTP_CA_reserved3,
    U_OTP_CA_reserved4,
    U_OTP_CA_reserved5,
    U_OTP_CA_reserved6,
    U_OTP_CA_reserved7,
    U_OTP_CA_reserved8,
    U_OTP_CA_reserved9,
    U_OTP_CA_reserved10,
    U_OTP_CA_reserved11,
    U_OTP_CA_reserved12,
    U_OTP_CA_reserved13,
    U_OTP_CA_reserved14,
    U_OTP_CA_reserved15,
    U_OTP_Chip_Extension_ID,
    U_OTP_PVConfiguration,
    U_OTP_EncryptLoader_Key_sel,
    U_OTP_EncryptLoader_KL_level,
    U_OTP_SW_Reserved0,
    U_OTP_EncryptLoader_ForceEnc,
    U_OTP_SW_Reserved1,
    U_OTP_HASH0_VER_REF,
    U_OTP_HASH1_VER_REF,
    U_OTP_HASH2_VER_REF,
    U_OTP_SW_Reserved2,
    U_OTP_Versioning,
    U_OTP_Preenablement_0,
    U_OTP_Preenablement_1,
    U_OTP_Preenablement_2,
    U_OTP_Preenablement_3,
    U_OTP_Preenablement_4,
    U_OTP_Preenablement_5,
    U_OTP_Preenablement_6,
    U_OTP_Preenablement_7,
    U_OTP_CAdata_0,
    U_OTP_CAdata_1,
    U_OTP_CAdata_2,
    U_OTP_CAdata_3,
    U_OTP_CAdata_4,
    U_OTP_CAdata_5,
    U_OTP_CAdata_6,
    U_OTP_CAdata_7,
    U_OTP_CAdata_8,
    U_OTP_CAdata_9,
    U_OTP_CAdata_10,
    U_OTP_CAdata_11,
    U_OTP_CAdata_12,
    U_OTP_CAdata_13,
    U_OTP_CAdata_14,
    U_OTP_CAdata_15,
    U_OTP_CAdata_16,
    U_OTP_CAdata_17,
    U_OTP_CAdata_18,
    U_OTP_CAdata_19,
    U_OTP_CAdata_20,
    U_OTP_CAdata_21,
    U_OTP_CAdata_22,
    U_OTP_CAdata_23,
    U_OTP_CAdata_24,
    U_OTP_CAdata_25,
    U_OTP_CAdata_26,
    U_OTP_CAdata_27,
    U_OTP_CAdata_28,
    U_OTP_CAdata_29,
    U_OTP_CAdata_30,
    U_OTP_CAdata_31,
    U_OTP_CAdata_32,
    U_OTP_CAdata_33,
    U_OTP_CAdata_34,
    U_OTP_CAdata_35,
    U_OTP_CAdata_36,
    U_OTP_CAdata_37,
    U_OTP_forbid_SCPU2DRAM,
    U_OTP_forbid_SCPUQMEM_to_DRAM,
    U_OTP_forbid_CryptoDMA_keyslot_QMEM,
    U_OTP_forbid_SCPU2UART,
    U_OTP_IXSC_config1,
    U_OTP_IXSC_config2,
    U_OTP_IXSC_config3,
    U_OTP_IXSC_config4,
    U_OTP_LockWord_Preenablement_0_W,
    U_OTP_LockWord_Preenablement_1_W,
    U_OTP_LockWord_Preenablement_2_W,
    U_OTP_LockWord_Preenablement_3_W,
    U_OTP_LockWord_Preenablement_4_W,
    U_OTP_LockWord_Preenablement_5_W,
    U_OTP_LockWord_Preenablement_6_W,
    U_OTP_LockWord_Preenablement_7_W,
    U_OTP_Test_Switch,
    U_OTP_IXSC_Reserved0,
    U_OTP_NOCS3_00,
    U_OTP_NOCS3_01,
    U_OTP_NOCS3_02,
    U_OTP_NOCS3_03,
    U_OTP_MSID,
    U_OTP_CFG_STB_CA_SN,
    U_OTP_RSA_0,
    U_OTP_RSA_1,
    U_OTP_RSA_0_integrity_chk,
    U_OTP_RSA_1_integrity_chk,
    U_OTP_ena_ACPUUseHWRSAKey,
    U_OTP_forbid_ACPUReadRSAKey0,
    U_OTP_forbid_ACPUReadRSAKey1,
    U_OTP_forbid_ACPUWrite_Versioning,
    U_OTP_forbid_ACPURead_Versioning,
    U_OTP_RSA_Reserved0,
    U_OTP_RSA_0_CRC,
    U_OTP_RSA_1_CRC,
    U_OTP_PGMask0,
    U_OTP_PGMask1,
    U_OTP_PGMask2,
    U_OTP_PGMask3,
    U_OTP_PGMask4,
    U_OTP_PGMask5,
    U_OTP_PGMask6,
    U_OTP_PGMask7,
    U_OTP_PGMask8,
    U_OTP_PGMask9,
    U_OTP_PGMask10,
    U_OTP_PGMask11,
    U_OTP_PGMask12,
    U_OTP_PGMask13,
    U_OTP_PGMask14,
    U_OTP_PGMask15,
    U_OTP_PGMask16,
    U_OTP_PGMask17,
    U_OTP_v_PubOtpOID,
    U_OTP_NDS_Reserved0,
    U_OTP_v_PubOtpVID,
    U_OTP_NDS_Reserved1,
    U_OTP_v_PubOtpGP,
    U_OTP_LockWord_GP_group_W,
    U_OTP_v_PubOtpBID,
    U_OTP_NDS_Reserved2,
    U_OTP_v_PubOtpRsaIndex,
    U_OTP_NDS_Reserved3,
    U_OTP_v_PubOtpMinConfVer,
    U_OTP_NDS_Reserved4,
    U_OTP_VenderConstSel,
    U_OTP_NDS_Reserved5,
    U_OTP_LockWord_NDS_Key1,
    U_OTP_LockWord_NDS_Key2,
    U_OTP_LockWord_NDS_Key3,
    U_OTP_LockWord_NDS_Key4,
    U_OTP_LockWord_NDS_Key5,
    U_OTP_LockWord_NDS_Key6,
    U_OTP_LockWord_NDS_Key7,
    U_OTP_LockWord_NDS_Key8,
    U_OTP_NDSKeyValid,
    U_OTP_forbid_NSK_wr_sck,
    U_OTP_UseCheckSum,
    U_OTP_allow_NDSKey_BlankChk,
    U_OTP_allow_ProgFail_RuinNDSKey,
    U_OTP_allow_NDSReadKeyWait200ms,
    U_OTP_allow_ReadErrorRstOtp,
    U_OTP_allow_illegalNDSFlagChk,
    U_OTP_allow_Rst_NDS_SCFlag_ParityFail,
    U_OTP_ena_ACPUUseNSK2,
    U_OTP_ena_DBUSUseNSK2,
    U_OTP_ena_ForceOneMilSec,
    U_OTP_allow_SCCheck,
    U_OTP_ena_TestRCFreq,
    U_OTP_ena_SWRN,
    U_OTP_ena_NSKSeedPRNG,
    U_OTP_OTPWritePWDProtect,
    U_OTP_NDS_keysel,
    U_OTP_allow_NSK2_PWD_Mode,
    U_OTP_ena_EMMFilter,
    U_OTP_ena_TestGenIN,
    U_OTP_RCFreq_map,
    U_OTP_NDS_Reserved6,
    U_OTP_allow_SkipBadNVBit,
    U_OTP_allow_NDSSC_ReadFail_BadPkt,
    U_OTP_allow_RANDOM_keybus,
    U_OTP_allow_RANDOM_byteacc,
    U_OTP_allow_NDS_Rd55AA,
    U_OTP_allow_NDS_Parity_chk,
    U_OTP_allow_NDS_KeyRd55AA,
    U_OTP_forbid_OTPBuiltInTest,
    U_OTP_forbid_Kilo_ProgRepair,
    U_OTP_NDS_Reserved7,
    U_OTP_NVCOUNT,
    U_OTP_NDS_SecretKey1,
    U_OTP_NDS_SecretKey2,
    U_OTP_NDS_SecretKey3,
    U_OTP_NDS_SecretKey4,
    U_OTP_NDS_SecretKey5,
    U_OTP_NDS_SecretKey6,
    U_OTP_NDS_SecretKey7,
    U_OTP_NDS_SecretKey8,
    U_OTP_NDSKey1Chksum,
    U_OTP_NDSKey1Tag,
    U_OTP_NDSKey2Chksum,
    U_OTP_NDSKey2Tag,
    U_OTP_NDSKey3Chksum,
    U_OTP_NDSKey3Tag,
    U_OTP_NDSKey4Chksum,
    U_OTP_NDSKey4Tag,
    U_OTP_NDSKey5Chksum,
    U_OTP_NDSKey5Tag,
    U_OTP_NDSKey6Chksum,
    U_OTP_NDSKey6Tag,
    U_OTP_NDSKey7Chksum,
    U_OTP_NDSKey7Tag,
    U_OTP_NDSKey8Chksum,
    U_OTP_NDSKey8Tag,
    U_OTP_NDS_Key1_CRC,
    U_OTP_NDS_Key2_CRC,
    U_OTP_NDS_Key3_CRC,
    U_OTP_NDS_Key4_CRC,
    U_OTP_NDS_Key5_CRC,
    U_OTP_NDS_Key6_CRC,
    U_OTP_NDS_Key7_CRC,
    U_OTP_NDS_Key8_CRC,
    U_OTP_NDS_ESCK_Key1_obfuscation,
    U_OTP_NDS_Key1_integrity_chk,
    U_OTP_NDS_ESCK_Key2_obfuscation,
    U_OTP_NDS_Key2_integrity_chk,
    U_OTP_NDS_ESCK_Key3_obfuscation,
    U_OTP_NDS_Key3_integrity_chk,
    U_OTP_NDS_ESCK_Key4_obfuscation,
    U_OTP_NDS_Key4_integrity_chk,
    U_OTP_NDS_ESCK_Key5_obfuscation,
    U_OTP_NDS_Key5_integrity_chk,
    U_OTP_NDS_ESCK_Key6_obfuscation,
    U_OTP_NDS_Key6_integrity_chk,
    U_OTP_NDS_ESCK_Key7_obfuscation,
    U_OTP_NDS_Key7_integrity_chk,
    U_OTP_NDS_ESCK_Key8_obfuscation,
    U_OTP_NDS_Key8_integrity_chk,
    U_OTP_EJTAG_MODE_Parity_bit,
    U_OTP_I2C_MODE_Parity_bit,
    U_OTP_SCAN_MODE_Parity_bit,
    U_OTP_VenderConstSel_Parity_bit,
    U_OTP_v_PubOtpRsaIndex_Parity_bit,
    U_OTP_NDS_ESCK_Key1_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key2_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key3_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key4_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key5_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key6_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key7_obfuscation_Parity_bit,
    U_OTP_NDS_ESCK_Key8_obfuscation_Parity_bit,
    U_OTP_allow_ReadErrorRstOtp_Parity_bit,
    U_OTP_ena_ACPUUseNSK2_Parity_bit,
    U_OTP_BootMode_Parity_bit,
    U_OTP_ena_ACPU2KT_Parity_bit,
    U_OTP_ena_EMMFilter_Parity_bit,
    U_OTP_allow_NDS_Rd55AA_Parity_bit,
    U_OTP_forbid_OTPBuiltInTest_Parity_bit,
    U_OTP_allow_NSK_RNG_ROSC_Parity_bit,
    U_OTP_SecretAreaEnable_Parity_bit,
    U_OTP_forbid_SW_SPSD_Key_Parity_bit,
    U_OTP_allow_OTP_BgCRC_Parity_bit,
    U_OTP_forbid_USBSlaveMode_Parity_bit,
    U_OTP_OTPWritePWDProtect_Parity_bit,
    U_OTP_ena_NSK2_Parity_bit,
    U_OTP_NDS_Reserved8,
    U_OTP_ena_ESAAlgo_invalidate,
    U_OTP_ena_LocalAlgo_Invalidate,
    U_OTP_ContentProtEn,
    U_OTP_concurrency_configuration,
    U_OTP_allow_NSK_RNG_ROSC,
    U_OTP_nds_fc_disable,
    U_OTP_NDS_CPNR0_sel,
    U_OTP_NDS_CPNR_off,
    U_OTP_NDS_Reserved9,
    U_OTP_PM51_ST_ADDR,
    U_OTP_PM51_ED_ADDR,
    U_OTP_forbid_PM51ReadOTP,
    U_OTP_forbid_PM51WriteOTP,
    U_OTP_allow_PM51,
    U_OTP_PM51_SW_R2,
    U_OTP_forbid_CLK_SEED_TEST,
    U_OTP_MOBF_TOP_use_DES,
    U_OTP_OBFUSCATEVideoStream,
    U_OTP_forbid_EJTAG_Pinshare,
    U_OTP_allow_clear_sram,
    U_OTP_forbid_dft_md_sram,
    U_OTP_LINK_ISOEN2GPIO4,
    U_OTP_PM_SLEEP_WR_PROT_EN,
    U_OTP_PWR_HW_RST_MODE_EN,
    U_OTP_ISOC_TST_DIG_EN_GATE,
    U_OTP_OTHERS_Reserved0,
    U_OTP_Rsv_Feature,
    U_OTP_forbid_CA_MCM,
    U_OTP_forbid_DIAMOND_PMU,
    U_OTP_allow_force_dual_core,
    U_OTP_disable_jpi,
    U_OTP_forbid_DIG_PMU,
    U_OTP_forbid_WHOLECHIP_alive_when_NOCORE_alive,
    U_OTP_OTHERS_Reserved2,
    U_OTP_allow_CORE0_always_alive,
    U_OTP_allow_NONCORE0_default_reset,
    U_OTP_pwrgd_int_en,
    U_OTP_OTHERS_Reserved3,
    U_OTP_LOGICBIST_SCAN_OUT_CRC32,
    U_OTP_OTHERS_Reserved4,
    U_OTP_RC_OPTION,
    U_OTP_RC_OPTION_EN,
    U_OTP_GCR_TRVBG,
    U_OTP_OTHERS_Reserved5,
    U_OTP_TEST_Status_TE,
    U_OTP_TEST_Status,
    U_OTP_ena_PVRNS2S,
    U_OTP_ena_PVRS2NS,
    U_OTP_ena_PlayBackRec0,
    U_OTP_ena_PlayBackRec1,
    U_OTP_ena_PlayBackRec2,
    U_OTP_ena_PlayBackRec3,
    U_OTP_ena_PlayBackRec4,
    U_OTP_ena_PlayBackRec5,
    U_OTP_ena_PVR_secure_protect_0,
    U_OTP_ena_PVR_secure_protect_1,
    U_OTP_ena_VQ_secure_protect_0,
    U_OTP_TSP_Reserved0,
    U_OTP_dis_TSO,
    U_OTP_ena_TS2TSO_0,
    U_OTP_ena_TS2TSO_1,
    U_OTP_ena_TSO_SecRange,
    U_OTP_CAVIDEnforcedTS0,
    U_OTP_CAVIDEnforcedTS1,
    U_OTP_CAVIDEnforcedTS2,
    U_OTP_CAVIDEnforcedTS3,
    U_OTP_CAVIDEnforcedTS4,
    U_OTP_TSP_Reserved1,
    U_OTP_CAVIDEnforcedTS5,
    U_OTP_ForbidAVtoSec,
    U_OTP_TSP_Reserved2,
    U_OTP_dis_clear_stream_to_PVR,
    U_OTP_forbid_SW_SPSD_Key,
    U_OTP_forbid_KL_SPSD_Key,
    U_OTP_allow_TSPCPUCodeProt,
    U_OTP_TSP_Reserved3,
    U_OTP_ena_KeyLadder_0_lock,
    U_OTP_ena_KeyLadder_1_lock,
    U_OTP_ena_NSK2_lock,
    U_OTP_ena_CERT_IP_lock,
    U_OTP_ena_DMA_lock,
    U_OTP_ena_SCPU_lock,
    U_OTP_Dlock_Reserved0,
    U_OTP_ena_ACPU2KT_lock,
    U_OTP_ena_NSK2KT_lock,
    U_OTP_ena_KL2KT_lock,
    U_OTP_ena_SCPU2KT_lock,
    U_OTP_ena_ACPU2DMA_lock,
    U_OTP_ena_DMA_DESBasedCipher_lock,
    U_OTP_Dlock_Reserved1,
    U_OTP_ena_LSACPCM_lock,
    U_OTP_ena_AESBasedCipher_lock,
    U_OTP_ena_DESBasedCipher_lock,
    U_OTP_ena_3DESBasedCipher_lock,
    U_OTP_ena_Multi2BasedCipher_lock,
    U_OTP_ena_DVBCSA2ConfCipher_lock,
    U_OTP_ena_DVBCSA2Cipher_lock,
    U_OTP_ena_DVBCSA3BasedCipher_lock,
    U_OTP_dis_kl0_hostkey_lock,
    U_OTP_dis_kl1_hostkey_lock,
    U_OTP_NDSKeyValid_lock,
    U_OTP_RC_OPTION_lock,
    U_OTP_Dlock_Reserved2,
    U_OTP_Dlock_Reserved3,
    U_OTP_LockWord_CA_reserved0_W,
    U_OTP_LockWord_CA_reserved1_W,
    U_OTP_LockWord_CA_reserved2_W,
    U_OTP_LockWord_CA_reserved3_W,
    U_OTP_LockWord_CA_reserved4_W,
    U_OTP_LockWord_CA_reserved5_W,
    U_OTP_LockWord_CA_reserved6_W,
    U_OTP_LockWord_CA_reserved7_W,
    U_OTP_LockWord_CA_reserved8_W,
    U_OTP_LockWord_CA_reserved9_W,
    U_OTP_LockWord_CA_reserved10_W,
    U_OTP_LockWord_CA_reserved11_W,
    U_OTP_LockWord_CA_reserved12_W,
    U_OTP_LockWord_CA_reserved13_W,
    U_OTP_LockWord_CA_reserved14_W,
    U_OTP_LockWord_CA_reserved15_W,
    U_OTP_LockWord_RSA_0_W,
    U_OTP_LockWord_RSA_1_W,
    U_OTP_LockWord_RSA_CTRL_W,
    U_OTP_LockWord_IXSC_Config_W,
    U_OTP_LockWord_v_PubOtpOID_W,
    U_OTP_LockWord_v_PubOtpVID_W,
    U_OTP_LockWord_v_PubOtpBID_W,
    U_OTP_LockWord_v_PubOtpRsaIndex_W,
    U_OTP_LockWord_v_PubOtpMinConfVer_W,
    U_OTP_LockWord_VenderConstSel_W,
    U_OTP_LockWord_NDS_info_W,
    U_OTP_LockWord_PM51_W,
    U_OTP_LockWord_OTHERS_W,
    U_OTP_LockWord_Reserved0,
    U_OTP_LockWord_NOCS3_MEM0_W,
    U_OTP_LockWord_NOCS3_MEM1_W,
    U_OTP_LockWord_NOCS3_MEM2_W,
    U_OTP_LockWord_NOCS3_MEM3_W,
    U_OTP_LockWord_MSID_W,
    U_OTP_LockWord_CFG_STB_CA_SN_W,
    U_OTP_LockWord_TSP_W,
    U_OTP_LockWord_PGMask_W,
    U_OTP_LockWord_ClkDet_Para_W,
    U_OTP_LockWord_OTP_CTRL_W,
    U_OTP_LockWord_CPU_bank_W,
    U_OTP_LockWord_trimming_W,
    U_OTP_LockWord_BOND0_W,
    U_OTP_LockWord_BOND_Reserved_W,
    U_OTP_LockWord_Reserved1,
    U_OTP_LockWord_LOT_INFO_W,
    U_OTP_LockWord_CA_Enable_W,
    U_OTP_LockWord_KL_bank_W,
    U_OTP_LockWord_KT_bank_W,
    U_OTP_LockWord_DMA_bank_W,
    U_OTP_LockWord_TSCE_W,
    U_OTP_LockWord_HDCP_KEY_W,
    U_OTP_LockWord_Multi2SysKey_W,
    U_OTP_LockWord_CAV_Key1_W,
    U_OTP_LockWord_CAV_Key2_W,
    U_OTP_LockWord_CAV_Key3_W,
    U_OTP_LockWord_CAV_Key4_W,
    U_OTP_LockWord_CAV_Key5_W,
    U_OTP_LockWord_CAV_Key6_W,
    U_OTP_LockWord_CAV_Key7_W,
    U_OTP_LockWord_CAV_Key8_W,
    U_OTP_LockWord_CAVID_W,
    U_OTP_LockWord_CAV_KP1_W,
    U_OTP_LockWord_CAV_KP2_W,
    U_OTP_LockWord_CAV_KP3_W,
    U_OTP_LockWord_CAV_KP4_W,
    U_OTP_LockWord_CAV_KP5_W,
    U_OTP_LockWord_CAV_KP6_W,
    U_OTP_LockWord_CAV_KP7_W,
    U_OTP_LockWord_CAV_KP8_W,
    U_OTP_LockWord_CAV_KP9_W,
    U_OTP_LockWord_CAV_KP10_W,
    U_OTP_LockWord_CAV_KP11_W,
    U_OTP_LockWord_CAV_KP12_W,
    U_OTP_LockWord_CAV_KP13_W,
    U_OTP_LockWord_CAV_KP14_W,
    U_OTP_LockWord_CAV_KP15_W,
    U_OTP_LockWord_PubOTPUniqueID1_W,
    U_OTP_LockWord_PubOTPUniqueID2_W,
    U_OTP_LockWord_PubOTPUniqueID3_W,
    U_OTP_LockWord_PubOTPUniqueID4_W,
    U_OTP_LockWord_AES_MOD_MASK_W,
    U_OTP_LockWord_ACPU_Property_W,
    U_OTP_LockWord_SCPU_Property_W,
    U_OTP_LockWord_CA_PWD_W,
    U_OTP_LockWord_CA_PWD_CTRL_W,
    U_OTP_LockWord_VtrackData_W,
    U_OTP_LockWord_FAKE_W,
    U_OTP_LockWord_SW0_W,
    U_OTP_LockWord_Reserved2,
    U_OTP_LockWord_0000_0511_W,
    U_OTP_LockWord_0512_1023_W,
    U_OTP_LockWord_1024_1535_W,
    U_OTP_LockWord_1536_2047_W,
    U_OTP_LockWord_2048_2559_W,
    U_OTP_LockWord_2560_3071_W,
    U_OTP_LockWord_3072_3391_W,
    U_OTP_LockWord_Chip_Extension_ID,
    U_OTP_LockWord_Reserved3,
    U_OTP_LockWord_CAV_Key1_R,
    U_OTP_LockWord_CAV_Key2_R,
    U_OTP_LockWord_CAV_Key3_R,
    U_OTP_LockWord_CAV_Key4_R,
    U_OTP_LockWord_CAV_Key5_R,
    U_OTP_LockWord_CAV_Key6_R,
    U_OTP_LockWord_CAV_Key7_R,
    U_OTP_LockWord_CAV_Key8_R,
    U_OTP_LockWord_RSA_0_R,
    U_OTP_LockWord_RSA_1_R,
    U_OTP_LockWord_HDCP_KEY_R,
    U_OTP_LockWord_Multi2SysKey_R,
    U_OTP_LockWord_AES_MOD_MASK_R,
    U_OTP_LockWord_VtrackData_R,
    U_OTP_LockWord_CA_PWD_R,
    U_OTP_LockWord_Reserved4,
    U_OTP_ClkDet_Para,
    U_OTP_forbid_ACPUWriteOTP,
    U_OTP_forbid_ACPUReadOTP,
    U_OTP_forbid_SCPUWriteOTP,
    U_OTP_forbid_SCPUReadOTP,
    U_OTP_forbid_DBBUSWriteOTP,
    U_OTP_forbid_DBBUSReadOTP,
    U_OTP_allow_RANDOM,
    U_OTP_allow_NOISE_Rd,
    U_OTP_allow_OTP_BgCRC,
    U_OTP_allow_SKIP_0,
    U_OTP_allow_SKIP_1,
    U_OTP_allow_Mask_AES,
    U_OTP_ena_Mstar_Wrapper,
    U_OTP_CTRL_Reserved0,
    U_OTP_allow_XOR_ROMCODE,
    U_OTP_allow_XOR_FIX,
    U_OTP_allow_PWD_OTP_prog,
    U_OTP_CTRL_Reserved1,
    U_OTP_SBoot,
    U_OTP_SecretAreaEnable,
    U_OTP_forbid_TestInOut,
    U_OTP_allow_FlashProtect,
    U_OTP_PERSO_done,
    U_OTP_CERT_PERSO_done,
    U_OTP_DBUG_Reserved0,
    U_OTP_SCAN_MODE,
    U_OTP_DBUG_Reserved1,
    U_OTP_MBIST_MODE,
    U_OTP_I2C_MODE,
    U_OTP_DBUG_Reserved2,
    U_OTP_EJTAG_MODE,
    U_OTP_RMA_MODE,
    U_OTP_LicRework,
    U_OTP_forbid_USBSlaveMode,
    U_OTP_allow_DRAM_MOBF,
    U_OTP_allow_DRAMOBF_4ROUND,
    U_OTP_ena_DRAMOBF_NS_MASK_ADDR,
    U_OTP_forbid_clk_otp_sel,
    U_OTP_forbid_acpu_access_sec_bridge,
    U_OTP_forbid_dbbus_access_sec_bridge,
    U_OTP_XPM_State,
    U_OTP_DBUG_Reserved3,
    U_OTP_DeactiveDRAMBoot,
    U_OTP_ACPU_Reserved0,
    U_OTP_BootMode,
    U_OTP_forbid_BCKGND_CHK,
    U_OTP_SCPUBootMode,
    U_OTP_ACPU_Reserved1,
    U_OTP_PM_MCU_RST_MOD,
    U_OTP_boot_host_sel,
    U_OTP_ACPU_Reserved2,
    U_OTP_ACPU_Reserved3,
    U_OTP_PostMskAreaRangeU,
    U_OTP_ACPUasHost,
    U_OTP_BONDING,
    U_OTP_NON_BLANK,
    U_OTP_ACPU_Reserved4,
    U_OTP_PE_LOT_INFO,
    U_OTP_BootDevice,
    U_OTP_MAX_RDNTBLK,
    U_OTP_forbid_STR,
    U_OTP_Bootcode_project,
    U_OTP_Bootcode_subversion,
    U_OTP_Bootcode_version,
    U_OTP_Analog_Usage,
    U_OTP_Analog_Usage2,
    U_OTP_trim_dac,
    U_OTP_Trim_Reserved0,
    U_OTP_BOND0,
    U_OTP_BOND_Reserved,
    U_OTP_ena_KeyLadder_0,
    U_OTP_ena_KeyLadder_1,
    U_OTP_ena_NSK2,
    U_OTP_ena_CERT_IP,
    U_OTP_ena_DMA,
    U_OTP_ena_SCPU,
    U_OTP_CA_EN_Reserved0,
    U_OTP_allow_IXSC,
    U_OTP_forbid_IXSC,
    U_OTP_forbid_PAVOSC,
    U_OTP_cfDeactiveFuse,
    U_OTP_SOC_UID_SEL,
    U_OTP_CA_EN_Reserved2,
    U_OTP_dis_kl0_hostkey,
    U_OTP_dis_kl1_hostkey,
    U_OTP_dis_kl0_hostkey_property,
    U_OTP_dis_kl1_hostkey_property,
    U_OTP_kl_protect0_mode,
    U_OTP_kl_protect1_mode,
    U_OTP_kl_protect2_mode,
    U_OTP_kl_protect3_mode,
    U_OTP_dis_kl_protect0,
    U_OTP_dis_kl_protect1,
    U_OTP_dis_kl_protect2,
    U_OTP_dis_kl_protect3,
    U_OTP_dis_kl_protect4,
    U_OTP_kl_TA_LUT0_mode,
    U_OTP_kl_TA_LUT1_mode,
    U_OTP_ena_kl_StopClkChk,
    U_OTP_KDF_secret_mask,
    U_OTP_IMLB_ROTATION,
    U_OTP_IrdetoTweakedCSAv3,
    U_OTP_allow_SPSSPDKeyShuffle,
    U_OTP_kl_BISS_MODE,
    U_OTP_ena_kl_xor_func,
    U_OTP_KL_Reserved2,
    U_OTP_ena_ACPU2KT,
    U_OTP_ena_NSK2KT,
    U_OTP_ena_KL2KT,
    U_OTP_ena_SCPU2KT,
    U_OTP_ena_ACPUWrNSKKey2KT,
    U_OTP_ena_ACPUWrCFBKey2KT,
    U_OTP_ena_NSKCW2CryptoDMA,
    U_OTP_ena_ACPU2LSA,
    U_OTP_BISS_MODE,
    U_OTP_BISS_SUBMODE,
    U_OTP_KT_Reserved0,
    U_OTP_ena_ACPU2DMA,
    U_OTP_ena_DMA_DESBasedCipher,
    U_OTP_ena_AESMAC_MaxRate,
    U_OTP_allow_DMAtDES2KeyEQChk,
    U_OTP_DMA_Reserved0,
    U_OTP_ena_DMA_ClearKeySlot,
    U_OTP_dis_DMA_mask_engine,
    U_OTP_DMA_Reserved1,
    U_OTP_ena_LSACPCM,
    U_OTP_ena_AESBasedCipher,
    U_OTP_ena_DESBasedCipher,
    U_OTP_ena_3DESBasedCipher,
    U_OTP_ena_Multi2BasedCipher,
    U_OTP_ena_DVBCSA2ConfCipher,
    U_OTP_ena_DVBCSA2Cipher,
    U_OTP_ena_DVBCSA3BasedCipher,
    U_OTP_Ch_SwitchComb,
    U_OTP_ena_LSAD_MDI,
    U_OTP_ena_LSAD_MDD,
    U_OTP_ena_LSAD_CIPLUS_AES,
    U_OTP_ena_LSAD_AES_ECB_CLEAR,
    U_OTP_ena_LSAD_SCTE41_SCTE52_DES,
    U_OTP_TSCE_Reserved0,
    U_OTP_ena_ESA_CIPLUS_AES,
    U_OTP_ena_ESA_CIPLUS_DES,
    U_OTP_ena_ESA_SCTE52_DES,
    U_OTP_ena_ESA_tDES_CBC_CLEAR,
    U_OTP_TSCE_Reserved1,
    U_OTP_ena_LSAS_MDI,
    U_OTP_ena_LSAS_MDD,
    U_OTP_ena_LSAS_CIPLUS_AES,
    U_OTP_ena_LSAS_AES_ECB_CLEAR,
    U_OTP_ena_LSAS_SCTE41_SCTE52_DES,
    U_OTP_TSCE_Reserved2,
    U_OTP_ena_DRMAreaEncrypt,
    U_OTP_ena_ReviewFailPkt,
    U_OTP_dis_NonSecRangeEncrypt,
    U_OTP_TSCE_Reserved3,
    U_OTP_ena_CA_PVR_secure_protect_0,
    U_OTP_ena_CA_PVR_secure_protect_1,
    U_OTP_ena_CA_PVR_secure_protect_2,
    U_OTP_ena_CA_PVR_secure_protect_3,
    U_OTP_ena_LowerPathRec,
    U_OTP_ena_SWMulti2SysKey,
    U_OTP_TSCE_Reserved4,
    U_OTP_HDCP_KEY,
    U_OTP_HDCP_Reserved0,
    U_OTP_Multi2SysKey,
    U_OTP_AES_MOD_MASK,
    U_OTP_v_pubOtpUniqueID1,
    U_OTP_v_pubOtpUniqueID2,
    U_OTP_v_pubOtpUniqueID3,
    U_OTP_v_pubOtpUniqueID4,
    U_OTP_CAV_ESCK_KeyValid,
    U_OTP_CAV_Reserved0,
    U_OTP_CAV_SCK_KeyValid,
    U_OTP_CAV_Reserved1,
    U_OTP_CAV_Key1_SCK_mapping,
    U_OTP_CAV_Key1_integrity_chk,
    U_OTP_CAV_Key1_allow_ChkNum,
    U_OTP_CAV_Key1_obfuscation,
    U_OTP_CAV_Key1_IdSel,
    U_OTP_CAV_Key1_KeySel,
    U_OTP_CAV_Key1_reserved,
    U_OTP_CAV_Key1_ChkNum_Gold,
    U_OTP_CAV_Key2_SCK_mapping,
    U_OTP_CAV_Key2_integrity_chk,
    U_OTP_CAV_Key2_allow_ChkNum,
    U_OTP_CAV_Key2_obfuscation,
    U_OTP_CAV_Key2_IdSel,
    U_OTP_CAV_Key2_KeySel,
    U_OTP_CAV_Key2_reserved,
    U_OTP_CAV_Key2_ChkNum_Gold,
    U_OTP_CAV_Key3_SCK_mapping,
    U_OTP_CAV_Key3_integrity_chk,
    U_OTP_CAV_Key3_allow_ChkNum,
    U_OTP_CAV_Key3_obfuscation,
    U_OTP_CAV_Key3_IdSel,
    U_OTP_CAV_Key3_KeySel,
    U_OTP_CAV_Key3_reserved,
    U_OTP_CAV_Key3_ChkNum_Gold,
    U_OTP_CAV_Key4_SCK_mapping,
    U_OTP_CAV_Key4_integrity_chk,
    U_OTP_CAV_Key4_allow_ChkNum,
    U_OTP_CAV_Key4_obfuscation,
    U_OTP_CAV_Key4_IdSel,
    U_OTP_CAV_Key4_KeySel,
    U_OTP_CAV_Key4_reserved,
    U_OTP_CAV_Key4_ChkNum_Gold,
    U_OTP_CAV_Key5_SCK_mapping,
    U_OTP_CAV_Key5_integrity_chk,
    U_OTP_CAV_Key5_allow_ChkNum,
    U_OTP_CAV_Key5_obfuscation,
    U_OTP_CAV_Key5_IdSel,
    U_OTP_CAV_Key5_KeySel,
    U_OTP_CAV_Key5_reserved,
    U_OTP_CAV_Key5_ChkNum_Gold,
    U_OTP_CAV_Key6_SCK_mapping,
    U_OTP_CAV_Key6_integrity_chk,
    U_OTP_CAV_Key6_allow_ChkNum,
    U_OTP_CAV_Key6_obfuscation,
    U_OTP_CAV_Key6_IdSel,
    U_OTP_CAV_Key6_KeySel,
    U_OTP_CAV_Key6_reserved,
    U_OTP_CAV_Key6_ChkNum_Gold,
    U_OTP_CAV_Key7_SCK_mapping,
    U_OTP_CAV_Key7_integrity_chk,
    U_OTP_CAV_Key7_allow_ChkNum,
    U_OTP_CAV_Key7_obfuscation,
    U_OTP_CAV_Key7_IdSel,
    U_OTP_CAV_Key7_KeySel,
    U_OTP_CAV_Key7_reserved,
    U_OTP_CAV_Key7_ChkNum_Gold,
    U_OTP_CAV_Key8_SCK_mapping,
    U_OTP_CAV_Key8_integrity_chk,
    U_OTP_CAV_Key8_allow_ChkNum,
    U_OTP_CAV_Key8_obfuscation,
    U_OTP_CAV_Key8_IdSel,
    U_OTP_CAV_Key8_KeySel,
    U_OTP_CAV_Key8_reserved,
    U_OTP_CAV_Key8_ChkNum_Gold,
    U_OTP_ACPU_Property,
    U_OTP_SCPU_Property,
    U_OTP_CAV_Key1_Property,
    U_OTP_CAV_Key2_Property,
    U_OTP_CAV_Key3_Property,
    U_OTP_CAV_Key4_Property,
    U_OTP_CAV_Key5_Property,
    U_OTP_CAV_Key6_Property,
    U_OTP_CAV_Key7_Property,
    U_OTP_CAV_Key8_Property,
    U_OTP_CAV_Key9_Property,
    U_OTP_CAV_Key10_Property,
    U_OTP_CAV_Key11_Property,
    U_OTP_CAV_Key12_Property,
    U_OTP_CAV_Key13_Property,
    U_OTP_CAV_Key14_Property,
    U_OTP_CAV_Key15_Property,
    U_OTP_CAV_SecretKey1,
    U_OTP_CAV_SecretKey2,
    U_OTP_CAV_SecretKey3,
    U_OTP_CAV_SecretKey4,
    U_OTP_CAV_SecretKey5,
    U_OTP_CAV_SecretKey6,
    U_OTP_CAV_SecretKey7,
    U_OTP_CAV_SecretKey8,
    U_OTP_CA_PWD_integrity_chk,
    U_OTP_Multi2SysKey_integrity_chk,
    U_OTP_CA_PWD_Reserved0,
    U_OTP_I2C_PWD_obfuscation,
    U_OTP_EJTAG_PWD_obfuscation,
    U_OTP_SCAN_PWD_obfuscation,
    U_OTP_MBIST_PWD_obfuscation,
    U_OTP_Multi2SysKey_CRC,
    U_OTP_I2C_PWD_CRC,
    U_OTP_EJTAG_PWD_CRC,
    U_OTP_SCAN_PWD_CRC,
    U_OTP_MBIST_PWD_CRC,
    U_OTP_CA_PWD_Keysel,
    U_OTP_ena_TP_PWD,
    U_OTP_ena_KDF_I2C_PWD,
    U_OTP_ena_KDF_EJTAG_PWD,
    U_OTP_ena_KDF_SCAN_PWD,
    U_OTP_ena_KDF_MBIST_PWD,
    U_OTP_I2C_TP_ID,
    U_OTP_EJTAG_TP_ID,
    U_OTP_SCAN_TP_ID,
    U_OTP_MBIST_TP_ID,
    U_OTP_I2C_PWD,
    U_OTP_EJTAG_PWD,
    U_OTP_SCAN_PWD,
    U_OTP_MBIST_PWD,
    U_OTP_VtrackID,
    U_OTP_VtrackKey,
    U_OTP_VtrackID_select,
    U_OTP_VtrackKey_select,
    U_OTP_Vtrack_Reserved0,
    U_OTP_FAKE,
    U_OTP_allow_CPU_rst_release,


} OTP_Conf;

#endif // #ifndef __HAL_CA_H__
