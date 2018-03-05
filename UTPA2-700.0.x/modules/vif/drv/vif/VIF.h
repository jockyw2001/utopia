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


#ifndef _VIF_H_
#define _VIF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "drvVIF_v2.h"

//****************************************************************************
// Public attributes.
//****************************************************************************

#ifdef _VIF_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef struct
{
    BYTE Vif_Clamp_L;
    BYTE Vif_Clamp_H;
    BYTE Vif_Clamp_Gain_L;
    BYTE Vif_Clamp_Gain_H;

    BYTE Vif_Cr_Rate_1;
    BYTE Vif_Cr_Rate_2;
    BYTE Vif_Cr_Rate_3;
    BYTE Vif_Cr_Rate_Inv;

    BYTE Vif_N_A1_C0_L;
    BYTE Vif_N_A1_C0_H;
    BYTE Vif_N_A1_C1_L;
    BYTE Vif_N_A1_C1_H;
    BYTE Vif_N_A1_C2_L;
    BYTE Vif_N_A1_C2_H;
    BYTE Vif_N_A2_C0_L;
    BYTE Vif_N_A2_C0_H;
    BYTE Vif_N_A2_C1_L;
    BYTE Vif_N_A2_C1_H;
    BYTE Vif_N_A2_C2_L;
    BYTE Vif_N_A2_C2_H;

    BYTE Vif_SOS_11_C0_L;
    BYTE Vif_SOS_11_C0_H;
    BYTE Vif_SOS_11_C1_L;
    BYTE Vif_SOS_11_C1_H;
    BYTE Vif_SOS_11_C2_L;
    BYTE Vif_SOS_11_C2_H;
    BYTE Vif_SOS_11_C3_L;
    BYTE Vif_SOS_11_C3_H;
    BYTE Vif_SOS_11_C4_L;
    BYTE Vif_SOS_11_C4_H;
    BYTE Vif_SOS_12_C0_L;
    BYTE Vif_SOS_12_C0_H;
    BYTE Vif_SOS_12_C1_L;
    BYTE Vif_SOS_12_C1_H;
    BYTE Vif_SOS_12_C2_L;
    BYTE Vif_SOS_12_C2_H;
    BYTE Vif_SOS_12_C3_L;
    BYTE Vif_SOS_12_C3_H;
    BYTE Vif_SOS_12_C4_L;
    BYTE Vif_SOS_12_C4_H;

    BYTE VifSos21FilterC0_L;
    BYTE VifSos21FilterC0_H;
    BYTE VifSos21FilterC1_L;
    BYTE VifSos21FilterC1_H;
    BYTE VifSos21FilterC2_L;
    BYTE VifSos21FilterC2_H;
    BYTE VifSos21FilterC3_L;
    BYTE VifSos21FilterC3_H;
    BYTE VifSos21FilterC4_L;
    BYTE VifSos21FilterC4_H;
    BYTE VifSos22FilterC0_L;
    BYTE VifSos22FilterC0_H;
    BYTE VifSos22FilterC1_L;
    BYTE VifSos22FilterC1_H;
    BYTE VifSos22FilterC2_L;
    BYTE VifSos22FilterC2_H;
    BYTE VifSos22FilterC3_L;
    BYTE VifSos22FilterC3_H;
    BYTE VifSos22FilterC4_L;
    BYTE VifSos22FilterC4_H;
    BYTE VifSos31FilterC0_L;
    BYTE VifSos31FilterC0_H;
    BYTE VifSos31FilterC1_L;
    BYTE VifSos31FilterC1_H;
    BYTE VifSos31FilterC2_L;
    BYTE VifSos31FilterC2_H;
    BYTE VifSos31FilterC3_L;
    BYTE VifSos31FilterC3_H;
    BYTE VifSos31FilterC4_L;
    BYTE VifSos31FilterC4_H;
    BYTE VifSos32FilterC0_L;
    BYTE VifSos32FilterC0_H;
    BYTE VifSos32FilterC1_L;
    BYTE VifSos32FilterC1_H;
    BYTE VifSos32FilterC2_L;
    BYTE VifSos32FilterC2_H;
    BYTE VifSos32FilterC3_L;
    BYTE VifSos32FilterC3_H;
    BYTE VifSos32FilterC4_L;
    BYTE VifSos32FilterC4_H;

    BYTE Vif_Agc_Ref;
    BYTE Vif_CR_K_Sel;
    BYTE Vif_CR_KpKi;
}VIFSTRParameter;

//****************************************************************************
// Public functions.
//****************************************************************************
INTERFACE void VIF_Version(void);
INTERFACE void VIF_SetClock(BOOL bEnable);
INTERFACE void VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen);
INTERFACE void VIF_Exit(void);
INTERFACE void VIF_Reset(void);
INTERFACE void VIF_Handler(BOOL bAutoScan);
INTERFACE void VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem);
INTERFACE void VIF_SetIfFreq(IfFrequencyType u16IfFreq);
INTERFACE U8 VIF_Read_CR_FOE(void);
INTERFACE U8 VIF_Read_CR_LOCK_STATUS(void);
INTERFACE void VIF_BypassDBBAudioFilter(BOOL bEnable);
INTERFACE void VIF_SetFreqBand(FrequencyBand u8FreqBand);
INTERFACE BOOL VIF_GetInputLevelIndicator(void);
INTERFACE BOOL VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen);
INTERFACE void VIF_ShiftClk(BYTE VifShiftClk);
INTERFACE MS_U32 VIF_SetPowerState(EN_POWER_MODE u16PowerState);
INTERFACE void VIF_WriteByte(U32 u32Reg, U8 u8Val );
INTERFACE U8 VIF_ReadByte(U32 u32Reg );


//-------------------------------- functions ------------------------------------------------------------

typedef void (*IOCTL_VIF_Version)(void);
typedef void (*IOCTL_VIF_SetClock)(BOOL);
typedef void (*IOCTL_VIF_Init)(VIFInitialIn *, DWORD);
typedef void (*IOCTL_VIF_Exit)(void);
typedef void (*IOCTL_VIF_Reset)(void);
typedef void (*IOCTL_VIF_Handler)(BOOL);
typedef void (*IOCTL_VIF_SetSoundSystem)(VIFSoundSystem);
typedef void (*IOCTL_VIF_SetIfFreq)(IfFrequencyType);
typedef U8 (*IOCTL_VIF_CR_FOE)(void);
typedef U8 (*IOCTL_VIF_LOCK_STATUS)(void);
typedef void (*IOCTL_VIF_Bypass_DBBAudioFilter)(BOOL);
typedef void (*IOCTL_VIF_SetFreqBand)(FrequencyBand);
typedef BOOL (*IOCTL_VIF_GetInputLevelIndicator)(void);
typedef BOOL (*IOCTL_VIF_SetParameter)(VIF_PARA_GROUP, void *, DWORD);
typedef void (*IOCTL_VIF_ShiftClk)(BYTE);
typedef MS_U32 (*IOCTL_VIF_SetPowerState)(EN_POWER_MODE);
typedef void (*IOCTL_VIF_WriteByte)(U32, U8);
typedef U8 (*IOCTL_VIF_ReadByte)(U32);


typedef struct _VIF_INSTANT_PRIVATE
{
    IOCTL_VIF_Version                                  fpVIFVersion;
    IOCTL_VIF_SetClock                                fpVIFSetClock;
    IOCTL_VIF_Init                                        fpVIFInit;
    IOCTL_VIF_Exit                                        fpVIFExit;
    IOCTL_VIF_Reset                                     fpVIFReset;
    IOCTL_VIF_Handler                                  fpVIFHandler;
    IOCTL_VIF_SetSoundSystem                     fpVIFSetSoundSystem;
    IOCTL_VIF_SetIfFreq                                fpVIFSetIfFreq;
    IOCTL_VIF_CR_FOE                                  fpVIFCRFOE;
    IOCTL_VIF_LOCK_STATUS                        fpVIFLOCKSTATUS;
    IOCTL_VIF_Bypass_DBBAudioFilter            fpVIFBypassDBBAudioFilter;
    IOCTL_VIF_SetFreqBand                           fpVIFSetFreqBand;
    IOCTL_VIF_GetInputLevelIndicator             fpVIFGetInputLevelIndicator;
    IOCTL_VIF_SetParameter                          fpVIFSetParameter;
    IOCTL_VIF_ShiftClk                                  fpVIFShiftClk;
    IOCTL_VIF_SetPowerState                        fpVIFSetPowerState;
    IOCTL_VIF_WriteByte                               fpVIFWriteByte;
    IOCTL_VIF_ReadByte                               fpVIFReadBtye;
    
}VIF_INSTANT_PRIVATE;


void VIFRegisterToUtopia(void);
MS_U32 VIFOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 VIFClose(void* pInstantTmp);
MS_U32 VIFIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
MS_U32 VIFStr(MS_U32 u32PowerState, void* pModule);


#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif //_VIF_H_

