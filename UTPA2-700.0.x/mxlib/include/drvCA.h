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
///
/// @file   drvCA.h
/// @brief  CA Driver Interface
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DRV_CA_H__
#define __DRV_CA_H__

#include "MsTypes.h"
#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_CA_LIB_CODE              {'C','A', '_', '_'}               //Lib code
#define MSIF_CA_LIBVER                {'0','1'}                         //LIB version
#define MSIF_CA_BUILDNUM              {'1','1'}                         //Build Number
#define MSIF_CA_CHANGELIST            {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define CA_DRV_VERSION               /* Character String for DRV/API version             */  \
    MSIF_TAG,                        /* 'MSIF'                                           */  \
    MSIF_CLASS,                      /* '00'                                             */  \
    MSIF_CUS,                        /* 0x0000                                           */  \
    MSIF_MOD,                        /* 0x0000                                           */  \
    MSIF_CHIP,                                                                               \
    MSIF_CPU,                                                                                \
    MSIF_CA_LIB_CODE,                /* IP__                                             */  \
    MSIF_CA_LIBVER,                  /* 0.0 ~ Z.Z                                        */  \
    MSIF_CA_BUILDNUM,                /* 00 ~ 99                                          */  \
    MSIF_CA_CHANGELIST,              /* CL#                                              */  \
    MSIF_OS


typedef enum
{
    E_CA_DEBUG_PORT_I2C,
    E_CA_DEBUG_PORT_JTAG,
    E_CA_DEBUG_PORT_SCAN,
} CA_DEBUG_PORT;

typedef enum
{
    E_CA_DEBUG_PORT_OPEN,
    E_CA_DEBUG_PORT_PASSWD,
    E_CA_DEBUG_PORT_CLOSE
} CA_DEBUG_PORT_MODE;

typedef enum
{
    E_CA_LOCK_WRITE = 1 << 0,
    E_CA_LOCK_READ = 1 << 1
} CA_LOCK_TYPE;

typedef enum
{
    E_BOOT_LockWord_W0000_0511_Addr,
    E_BOOT_LockWord_W0512_1023_Addr,
    E_BOOT_LockWord_W1024_1535_Addr,
    E_BOOT_LockWord_W1536_2047_Addr,
    E_BOOT_LockWord_W2048_2559_Addr,
    E_BOOT_LockWord_W2560_3071_Addr,
    E_BOOT_LockWord_W3072_3583_Addr,
    E_BOOT_LockWord_W3584_3759_Addr,
    E_LockWord_v_PubOtpOID,
    E_LockWord_v_PubOtpVID,
    E_LockWord_v_pubOtpUniqueID1,
    E_LockWord_v_pubOtpUniqueID2,
    E_LockWord_CA_Reserved,
    E_LockWord_CA_MSID,

    E_BOOT_LockWord_W3072_3391_Addr,
    E_LockWord_v_pubOtpUniqueID3,
    E_LockWord_v_pubOtpUniqueID4,

    E_LockWord_CAV_SecretKey6_W=0x106,
    E_LockWord_CAV_SecretKey6_R=0x116,

    E_LockWord_RSA_1_W=0x201,

    E_LockWord_Debugport_PWD_W=0x301,
    E_LockWord_Debugport_PWD_R=0x311,

    E_LockWord_CA_reserved0=0x401,
    E_LockWord_CA_reserved1=0x402,
    E_LockWord_CA_reserved2=0x403,
    E_LockWord_CA_reserved3=0x404,
} CA_OTP_LOCK;

/*
    CA Vendor code rule
    First 8 bits: code by initial of CA Vendor
    "A": 01, "B": 02, "C": 03, "D": 04, ... , "Y": 19, "Z": 1A
    Second 4 bits: Increase if the initial of CA Vendor repeat
    Thrid 4 bits: Serial number of CA Vendor

    0x0000 reserve for NULL
    0xFFFF for unknow CA
  */
typedef enum
{
    E_CA_VENDOR_NULL,
    E_CA_VENDOR_CONAX       = 0x0300,
    E_CA_VENDOR_ECHOSTAR    = 0x0500,
    E_CA_VENDOR_IRDETO      = 0x0900,
    E_CA_VENDOR_LATENS      = 0x0C00,
    E_CA_VENDOR_NDS         = 0x0E00,
    E_CA_VENDOR_NAGRA       = 0x0E10,
    E_CA_VENDOR_SMI         = 0x1300,
    E_CA_VENDOR_VIACCESS    = 0x1600,
    E_CA_VENDOR_VERIMATRIX  = 0x1610,
    E_CA_VENDOR_UNKNOW      = 0xFFFF
} CA_VENDOR_CODE;

typedef struct
{
    MS_U32 u32FwAddr;
    MS_U32 u32FwSize;
} CA_BGC_CFG;

typedef struct
{
    MS_U32 u32BGCMode;        //0:dynamic, 1:static
    MS_U32 u32ProtectAddr;    //protected address, physicall address
    MS_U32 u32ProtectSize;    //protected size
    MS_U8  u8Golden[32];      //password is used for reset section
    MS_U8  u8ResetPassword[4];//only use for dynamic mode
    MS_U16 u16Interval;       //unit: millisecond, the frequency to check the proteced area
} CA_BGC_SEC_CFG;

typedef enum
{
    E_CA_SECRET_KEY1,
    E_CA_SECRET_KEY2,
    E_CA_SECRET_KEY3,
    E_CA_SECRET_KEY4,
    E_CA_SECRET_KEY5,
    E_CA_SECRET_KEY6,
    E_CA_SECRET_KEY7,
    E_CA_SECRET_KEY8,
} CA_SECRET_KEY;

typedef enum
{
    E_CA_RSA_KEY0,
    E_CA_RSA_KEY1,
} CA_RSA_KEY;

typedef enum
{
    E_CA_CUSTOM_SECURE_MAGIC,
} CA_CUSTOM_DATA;


DLL_PUBLIC MS_BOOL MDrv_CA_Init(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxDeviceIdSize(void);
DLL_PUBLIC MS_U32 MDrv_CA_MaxReservedSize(void);

//-------------------------------------------------------------------------------------------------
// The functions will read/write directly from/to OTP
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureBoot(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureBootEnabled(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetBlockLock(MS_U32 u32Start, MS_U32 u32End, CA_LOCK_TYPE eLockType);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetBlockLock(MS_U32 *pu32Start, MS_U32 *pu32End, CA_LOCK_TYPE *eLockType);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetRSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetTEERSAextID(MS_U32 u32Value);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_GetTEERSAextID(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_REF_VER(MS_U32 u32HASH0_REF_VER, MS_U32 u32HASH2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_REF_VER(MS_U32 *pu32HASH0_REF_VER, MS_U32 *pu32HASH2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH1_REF_VER(MS_U32 u32HASH1_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH1_REF_VER(MS_U32 *pu32HASH1_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetHASH_TEE_REF_VER(MS_U32 u32HASH_TEE0_REF_VER, MS_U32 u32HASH_TEE1_REF_VER, MS_U32 u32HASH_TEE2_REF_VER);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetHASH_TEE_REF_VER(MS_U32 *pu32HASH_TEE0_REF_VER, MS_U32 *pu32HASH_TEE1_REF_VER, MS_U32 *pu32HASH_TEE2_REF_VER);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE eMode);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortMode(CA_DEBUG_PORT eDebugPort, CA_DEBUG_PORT_MODE *eMode);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDeviceId(const MS_U8 *pu8Did, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDeviceId(MS_U8 *pu8Did, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_WriteReserved(const MS_U8 *pu8RData, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_ReadReserved(MS_U8 *pu8RData, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_EnableSecureCWMode(void);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsSecureCWMode(void);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_DumpKeyProperty(MS_U32 u32KeyNumber);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetSecretKey(CA_SECRET_KEY eKeySel, const MS_U8 *pu8SecretKey, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetSecretKey(CA_SECRET_KEY eKeySel, MS_U8 *pu8SecretKey, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetRSAKey(CA_RSA_KEY eKeySel, const MS_U8 *pu8RSAKey, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetRSAKey(CA_RSA_KEY eKeySel, MS_U8 *pu8RSAKey, MS_U32 u32Size);

DLL_PUBLIC MS_BOOL MDrv_CA_OTP_SetDebugPortPassword(CA_DEBUG_PORT eDebugPort, const MS_U8 *pu8Password, MS_U32 u32Size);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_GetDebugPortPassword(CA_DEBUG_PORT eDebugPort, MS_U8 *pu8Password, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
// The OTP read/write
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_IsBlank(MS_U32 addr);
DLL_PUBLIC MS_U32 MDrv_CA_OTP_Read(MS_U32 addr);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_Write(MS_U32 addr, MS_U32 value);

DLL_PUBLIC MS_BOOL MDrv_CA_Locked(CA_OTP_LOCK eLock);
DLL_PUBLIC MS_BOOL MDrv_CA_OTP_Lock(CA_OTP_LOCK eLock);

//-------------------------------------------------------------------------------------------------
// Hardware 16-bits Random number generator
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MDrv_CA_Random(void);

//-------------------------------------------------------------------------------------------------
// The CA Vendor
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_U32 MDrv_CA_OTP_ReadCAVendor(void);


//-------------------------------------------------------------------------------------------------
// The CA MBX between R2 and ACPU
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_MBX_Init(void);
DLL_PUBLIC MS_U32  MDrv_CA_MBX_Write(MS_U8* u8Addr, MS_U32 u32Size);
DLL_PUBLIC MS_U32  MDrv_CA_MBX_Read(MS_U8* u8Addr, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
// The CA STR PM Config
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_STR_Init(void);
DLL_PUBLIC MS_BOOL MDrv_CA_STR_SetParamAddr(MS_U8 *pu8VAddr);
DLL_PUBLIC MS_BOOL MDrv_CA_STR_SetMAC(MS_U8 *pu8MAC, MS_U32 u32MACSize);

//-------------------------------------------------------------------------------------------------
// The function will config BGC
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_Init(CA_BGC_CFG stBGCCfg);
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_SetSection(MS_U32 u32SecId, const CA_BGC_SEC_CFG* const pstSecCfg);
DLL_PUBLIC MS_BOOL MDrv_CA_BGC_ResetSection(MS_U32 u32SecId, const MS_U8* const pu8RstPSWD);

//-------------------------------------------------------------------------------------------------
// The function is used for reading/writing a 32-bit customer specific data
//-------------------------------------------------------------------------------------------------
DLL_PUBLIC MS_BOOL MDrv_CA_CustomData_Write(CA_CUSTOM_DATA eData, MS_U32 u32Data);
DLL_PUBLIC MS_BOOL MDrv_CA_CustomData_Read(CA_CUSTOM_DATA eData, MS_U32* pu32Data);


#ifdef __cplusplus
}
#endif

#endif
