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

#ifndef _DRVAESDMA_PRIV_H_
#define _DRVAESDMA_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvCIPHER.h"


typedef MS_U32 (*IOCTL_AESDMA_INIT)(MS_PHY, MS_PHY, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETIV)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_RESET)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETFILEINOUT)(MS_PHY, MS_U32, MS_PHY, MS_PHY);
typedef MS_U32 (*IOCTL_AESDMA_SETKEY)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_SELENG)(DrvAESDMA_CipherMode, MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_START)(MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_GETSTATUS)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_ISFINISHED)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_SETSECUREKEY)(void);
typedef MS_U32 (*IOCTL_AESDMA_SETPS)(MS_U32, MS_U32, MS_BOOL, MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_PSRELEASE)(void);
typedef MS_U32 (*IOCTL_AESDMA_GETPSMATCHEDBYTECNT)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_GETPSMATCHEDPTN)(void);
typedef MS_U32 (*IOCTL_AESDMA_NOTIFY)(DrvAESDMA_Event, P_DrvAESDMA_EvtCallback);
typedef MS_U32 (*IOCTL_AESDMA_RAND)(MS_U32 *, MS_U32);
typedef MS_U32 (*IOCTL_RSA_CALCULATE)(DrvAESDMA_RSASig *, DrvAESDMA_RSAKey *, DrvAESDMA_RSAMode);
typedef MS_U32 (*IOCTL_RSA_ISFINISHED)(void);
typedef MS_U32 (*IOCTL_RSA_OUTPUT)(DrvAESDMA_RSAMode, DrvAESDMA_RSAOut *);
typedef MS_U32 (*IOCTL_SHA_CALCULATE)(DrvAESDMA_SHAMode, MS_PHY, MS_U32, MS_PHY);
typedef MS_U32 (*IOCTL_SHA_CALCULATE_MANUAL)(DrvAESDMA_HASHCFG, DrvAESDMA_HASH_STAGE, MS_U32, MS_U8 *);
typedef MS_U32 (*IOCTL_AESDMA_SETKEY_EX)(const MS_U8 *, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETIV_EX)(const MS_U8 *, MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETDEFAULTCAVID)(MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_SETKEYINDEX)(MS_U32 , MS_BOOL);
typedef MS_U32 (*IOCTL_AESDMA_ALLOCKEYSLOT)(MS_U32 *);
typedef MS_U32 (*IOCTL_AESDMA_FREEKEYSLOT)(MS_U32);
typedef MS_U32 (*IOCTL_AESDMA_INITBYSWFLAG)(MS_BOOL);

typedef struct _AESDMA_RESOURCE_PRIVATE
{
    MS_U32 AES_Dummy;
}AESDMA_RESOURCE_PRIVATE;

typedef struct _SHA_RESOURCE_PRIVATE
{
    MS_U32 SHA_Dummy;
}SHA_RESOURCE_PRIVATE;

typedef struct _RSA_RESOURCE_PRIVATE
{
    MS_U32 RSA_Dummy;
}RSA_RESOURCE_PRIVATE;

typedef struct _DRVAESDMA_RESOURCE_PRIVATE
{
    MS_BOOL bInited_Drv;
} DRVAESDMA_RESOURCE_PRIVATE;

typedef struct _AESDMA_RESOURCE_SHARED
{
    void* pAESResource;
}AESDMA_RESOURCE_SHARED;

typedef struct _RSA_RESOURCE_SHARED
{
    void* pRSAResource;
}RSA_RESOURCE_SHARED;

typedef enum {
    E_AESDMA_RESOURCE,
} eAESDMAResourceId;

typedef struct _AESDMA_INSTANT_PRIVATE
{
    MS_U8                              InitVector[16];
    MS_U32                             u32IVLen;
    MS_U32                             u32FileinAddr;
    MS_U32                             u32FileInNum;
    MS_U32                             u32FileOutSAddr;
    MS_U32                             u32FileOutEAddr;
    MS_VIRT                            ptrKey;
    MS_U8                              CipherKey[16];
    MS_U32                             u32KeyLen;
    MS_U32                             u32KeyIndex;
    MS_BOOL                            bOddKey;
    MS_U32                             u32Mode;
    MS_BOOL                            bDescrypt;
    MS_BOOL                            bSecretKey;
    MS_U32                             u32CmdId;
    MS_U32                             u32CAVid;
    MS_U8                              u8KeySlotUsage[4];
    IOCTL_AESDMA_INIT                  fpAESDMAInit;
    IOCTL_AESDMA_SETIV                 fpAESDMASetIV;
    IOCTL_AESDMA_RESET                 fpAESDMAReset;
    IOCTL_AESDMA_SETFILEINOUT          fpAESDMASetFileInOut;
    IOCTL_AESDMA_SETKEY                fpAESDMASetKey;
    IOCTL_AESDMA_SELENG                fpAESDMASelEng;
    IOCTL_AESDMA_START                 fpAESDMAStart;
    IOCTL_AESDMA_GETSTATUS             fpAESDMAGetStatus;
    IOCTL_AESDMA_ISFINISHED            fpAESDMAIsFinished;
    IOCTL_AESDMA_SETSECUREKEY          fpAESDMASetSecureKey;
    IOCTL_AESDMA_SETPS                 fpAESDMASetPS;
    IOCTL_AESDMA_PSRELEASE             fpAESDMAPSRelease;
    IOCTL_AESDMA_GETPSMATCHEDBYTECNT   fpAESDMAGetPSMatchedByteCNT;
    IOCTL_AESDMA_GETPSMATCHEDPTN       fpAESDMAGetPSMatchedPTN;
    IOCTL_AESDMA_NOTIFY                fpAESDMANotify;
    IOCTL_AESDMA_RAND                  fpAESDMARand;
    IOCTL_RSA_CALCULATE                fpRSACalculate;
    IOCTL_RSA_ISFINISHED               fpRSAIsFinished;
    IOCTL_RSA_OUTPUT                   fpRSAOutput;
    IOCTL_SHA_CALCULATE                fpSHACalculate;
    IOCTL_SHA_CALCULATE_MANUAL         fpSHACalculateManual;
    IOCTL_AESDMA_SETKEY_EX             fpAESDMASetKey_Ex;
    IOCTL_AESDMA_SETIV_EX              fpAESDMASetIV_Ex;
    IOCTL_AESDMA_SETDEFAULTCAVID       fpAESDMASetDefaultCAVid;
    IOCTL_AESDMA_SETKEYINDEX           fpAESDMASetKeyIndex;
    IOCTL_AESDMA_ALLOCKEYSLOT          fpAESDMAAllocKeySlot;
    IOCTL_AESDMA_FREEKEYSLOT           fpAESDMAFreeKeySlot;
    IOCTL_AESDMA_INITBYSWFLAG          fpAESDMAInitBySWFlag;
}AESDMA_INSTANT_PRIVATE;

void AESDMARegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 AESDMAOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 AESDMAClose(void* pInstantTmp);
MS_U32 AESDMAIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);

DRVAESDMA_RESULT _MDrv_AESDMA_Init(MS_PHY u32miu0addr , MS_PHY u32miu1addr , MS_U32 u32miunum);
DRVAESDMA_RESULT _MDrv_AESDMA_InitBySWFlag(MS_BOOL bSWInit);
DRVAESDMA_RESULT _MDrv_AESDMA_QueryCipherMode(DrvAESDMA_CipherMode eMode);
DRVAESDMA_RESULT _MDrv_AESDMA_SelEng(DrvAESDMA_CipherMode eMode, MS_BOOL bDescrypt);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKey_Ex(const MS_U8 *pu8Key, MS_U32 u32Len);
DRVAESDMA_RESULT _MDrv_AESDMA_SetIV_Ex(const MS_U8 *pu8IV, MS_U32 u32Len);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKey(MS_U32 *pCipherKey);
DRVAESDMA_RESULT _MDrv_AESDMA_SetSecureKey(void);
DRVAESDMA_RESULT _MDrv_AESDMA_SetIV(MS_U32 *pInitVector);
DRVAESDMA_RESULT _MDrv_AESDMA_SetPS(MS_U32 u32PTN, MS_U32 u32Mask, MS_BOOL bPSin_Enable, MS_BOOL bPSout_Enable);
DRVAESDMA_RESULT _MDrv_AESDMA_PSRelease(void);
DRVAESDMA_RESULT _MDrv_AESDMA_SetFileInOut(MS_PHY u32FileinAddr, MS_U32 u32FileInNum, MS_PHY u32FileOutSAddr, MS_PHY u32FileOutEAddr);
DRVAESDMA_RESULT _MDrv_AESDMA_Start(MS_BOOL bStart);
DRVAESDMA_RESULT _MDrv_AESDMA_Reset(void);
DRVAESDMA_RESULT _MDrv_AESDMA_GetStatus(MS_U32 *u32RetVal);
DRVAESDMA_RESULT _MDrv_AESDMA_IsFinished(MS_U32 *u32RetVal);
MS_U32 _MDrv_AESDMA_GetPSMatchedByteCNT(MS_U32 *u32RetVal);
MS_U32 _MDrv_AESDMA_GetPSMatchedPTN(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Notify(DrvAESDMA_Event eEvents, P_DrvAESDMA_EvtCallback pfCallback);
DRVAESDMA_RESULT _MDrv_AESDMA_SetDbgLevel(DrvAESDMA_DbgLevel DbgLevel);
DRVAESDMA_RESULT _MDrv_AESDMA_GetLibVer(const MSIF_Version **ppVersion);
DRVAESDMA_RESULT _MDrv_AESDMA_Rand(MS_U32 *u32PABuf, MS_U32 u32Size);
DRVAESDMA_RESULT _MDrv_SHA_Calculate(DrvAESDMA_SHAMode eMode, MS_PHY u32PAInBuf, MS_U32 u32Size, MS_PHY u32PAOutBuf);
DRVAESDMA_RESULT _MDrv_SHA_CalculateManual(DrvAESDMA_HASHCFG stCfg, DrvAESDMA_HASH_STAGE eStage, MS_U32 u32DoneBytes, MS_U8 *pu8SetIV);
DRVAESDMA_RESULT _MDrv_MOBF_DmaOnly(void);
DRVAESDMA_RESULT _MDrv_MOBF_Encrypt(MS_U32 u32Key, MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_MOBF_Decrypt(MS_U32 u32Key, MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_MOBF_OneWay(void);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_MaskScrmb(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetAddedScrmbPattern(DrvAESDMA_ScrmbPattern ePattern);
MS_U8 _MDrv_AESDMA_Parser_QueryPidCount(MS_U8* retCount);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_SetPid(MS_U8 u8Index, MS_U16 u16Pid);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_BypassPid(MS_BOOL bEnable);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Encrypt(DrvAESDMA_ParserMode eMode);
DRVAESDMA_RESULT _MDrv_AESDMA_Parser_Decrypt(DrvAESDMA_ParserMode eMode);
MS_BOOL _MDrv_AESDMA_Lock(void);
MS_BOOL _MDrv_AESDMA_Unlock(void);
DRVAESDMA_RESULT _MDrv_AESDMA_SetDefaultCAVid(MS_U32 u32CAVid);
DRVAESDMA_RESULT _MDrv_AESDMA_SetKeyIndex (MS_U32 u32Index, MS_BOOL bIsOddKey);
DRVAESDMA_RESULT _MDrv_AESDMA_AllocKeySlot (MS_U32 *pu32Index);
DRVAESDMA_RESULT _MDrv_AESDMA_FreeKeySlot (MS_U32 u32Index);
DRVAESDMA_RESULT _MDrv_RSA_Calculate(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAKey *pstKey, DrvAESDMA_RSAMode eMode);
DRVAESDMA_RESULT _MDrv_RSA_Calculate_Hw_Key(DrvAESDMA_RSASig *pstSign, DrvAESDMA_RSAHwKeyMode eMode);
DRVAESDMA_RESULT _MDrv_RSA_IsFinished(void);
DRVAESDMA_RESULT _MDrv_RSA_Output(DrvAESDMA_RSAMode eMode, DrvAESDMA_RSAOut *pstRSAOut);

#ifdef __cplusplus
}
#endif
#endif // _DRVAESDMA_PRIV_H_
