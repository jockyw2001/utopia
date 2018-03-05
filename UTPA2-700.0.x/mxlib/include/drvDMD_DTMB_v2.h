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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvDMD_DTMB_v2.h
/// @brief  DMD DTMB Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DTMB_V2_H_
#define _DRV_DTMB_V2_H_

#include "drvDMD_DTMB.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef DLL_PUBLIC 
#define DLL_PUBLIC 
#endif

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef enum {
    DMD_DTMB_DRV_CMD_SetDbgLevel,
    DMD_DTMB_DRV_CMD_GetInfo,
    DMD_DTMB_DRV_CMD_GetLibVer,
    DMD_DTMB_DRV_CMD_Init,
    DMD_DTMB_DRV_CMD_Exit,
    DMD_DTMB_DRV_CMD_GetConfig,  
    DMD_DTMB_DRV_CMD_SetConfig,
    DMD_DTMB_DRV_CMD_SetReset,
    DMD_DTMB_DRV_CMD_Set_QAM_SR,
    DMD_DTMB_DRV_CMD_SetActive,
    DMD_DTMB_DRV_CMD_SetPowerState,
    DMD_DTMB_DRV_CMD_GetLock,
    DMD_DTMB_DRV_CMD_GetModulationMode,
    DMD_DTMB_DRV_CMD_GetSignalStrength,
    DMD_DTMB_DRV_CMD_ReadFrequencyOffset,
    DMD_DTMB_DRV_CMD_GetSignalQuality,
    DMD_DTMB_DRV_CMD_GetPreLdpcBer,
    DMD_DTMB_DRV_CMD_GetPreViterbiBer,
    DMD_DTMB_DRV_CMD_GetPostViterbiBer,
    DMD_DTMB_DRV_CMD_GetSNR,
    DMD_DTMB_DRV_CMD_SetSerialControl,
    DMD_DTMB_DRV_CMD_IIC_BYPASS_MODE,
    DMD_DTMB_DRV_CMD_SWITCH_SSPI_GPIO,
    DMD_DTMB_DRV_CMD_GPIO_GET_LEVEL,
    DMD_DTMB_DRV_CMD_GPIO_SET_LEVEL,
    DMD_DTMB_DRV_CMD_GPIO_OUT_ENABLE,
    DMD_DTMB_DRV_CMD_DoIQSwap,
    DMD_DTMB_DRV_CMD_GetReg,
    DMD_DTMB_DRV_CMD_SetReg,
    DMD_DTMB_DRV_CMD_MD_Init,
    DMD_DTMB_DRV_CMD_MD_Exit,
    DMD_DTMB_DRV_CMD_MD_GetConfig,   
    DMD_DTMB_DRV_CMD_MD_SetConfig,
    DMD_DTMB_DRV_CMD_MD_SetReset,
    DMD_DTMB_DRV_CMD_MD_Set_QAM_SR,
    DMD_DTMB_DRV_CMD_MD_SetActive,
    DMD_DTMB_DRV_CMD_MD_SetPowerState,
    DMD_DTMB_DRV_CMD_MD_GetLock,
    DMD_DTMB_DRV_CMD_MD_GetModulationMode,
    DMD_DTMB_DRV_CMD_MD_GetSignalStrength,
    DMD_DTMB_DRV_CMD_MD_ReadFrequencyOffset,
    DMD_DTMB_DRV_CMD_MD_GetSignalQuality,
    DMD_DTMB_DRV_CMD_MD_GetPreLdpcBer,
    DMD_DTMB_DRV_CMD_MD_GetPreViterbiBer,
    DMD_DTMB_DRV_CMD_MD_GetPostViterbiBer,
    DMD_DTMB_DRV_CMD_MD_GetSNR,
    DMD_DTMB_DRV_CMD_MD_SetSerialControl,
    DMD_DTMB_DRV_CMD_MD_IIC_BYPASS_MODE,
    DMD_DTMB_DRV_CMD_MD_SWITCH_SSPI_GPIO,
    DMD_DTMB_DRV_CMD_MD_GPIO_GET_LEVEL,
    DMD_DTMB_DRV_CMD_MD_GPIO_SET_LEVEL,
    DMD_DTMB_DRV_CMD_MD_GPIO_OUT_ENABLE,
    DMD_DTMB_DRV_CMD_MD_DoIQSwap,
    DMD_DTMB_DRV_CMD_MD_GetReg,
    DMD_DTMB_DRV_CMD_MD_SetReg
} DMD_DTMB_DRV_COMMAND;

typedef struct DLL_PACKED _DTMB_DBG_LEVEL_PARAM
{
    DMD_DTMB_DbgLv u8DbgLevel;
} DTMB_DBG_LEVEL_PARAM, *PDTMB_DBG_LEVEL_PARAM;

typedef struct DLL_PACKED _DTMB_GET_INFO_PARAM
{
    DMD_DTMB_Info* pInfo;
} DTMB_GET_INFO_PARAM, *PDTMB_GET_INFO_PARAM;

typedef struct DLL_PACKED _DTMB_GET_LIB_VER_PARAM
{
    const MSIF_Version **ppVersion;
} DTMB_GET_LIB_VER_PARAM, *PDTMB_GET_LIB_VER_PARAM;

typedef struct DLL_PACKED _DTMB_INIT_PARAM
{
    MS_U8 id;
    DMD_DTMB_InitData *pDMD_DTMB_InitData;
    MS_U32             u32InitDataLen;
} DTMB_INIT_PARAM, *PDTMB_INIT_PARAM;

typedef struct DLL_PACKED _DTMB_ID_PARAM
{
    MS_U8 id;
} DTMB_ID_PARAM, *PDTMB_ID_PARAM;

typedef struct DLL_PACKED _DTMB_SET_CONFIG_PARAM
{
    MS_U8 id;
    DMD_DTMB_DEMOD_TYPE eType;
    MS_BOOL             bEnable;
} DTMB_SET_CONFIG_PARAM, *PDTMB_SET_CONFIG_PARAM;

typedef struct DLL_PACKED _DTMB_SET_QAM_SR_PARAM
{
    MS_U8 id;
    DMD_DTMB_DEMOD_TYPE eType;
    MS_U16              symbol_rate;
} DTMB_SET_QAM_SR_PARAM, *PDTMB_SET_QAM_SR_PARAM;

typedef struct DLL_PACKED _DTMB_SET_ACTIVE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} DTMB_SET_ACTIVE_PARAM, *PDTMB_SET_ACTIVE_PARAM;

typedef struct DLL_PACKED _DTMB_SET_POWER_STATE_PARAM
{
    MS_U8 id;
    EN_POWER_MODE u16PowerState;
} DTMB_SET_POWER_STATE_PARAM, *PDTMB_SET_POWER_STATE_PARAM;

typedef struct DLL_PACKED _DTMB_GET_LOCK_PARAM
{
    MS_U8 id;
    DMD_DTMB_GETLOCK_TYPE eType;
    DMD_DTMB_LOCK_STATUS  status;
} DTMB_GET_LOCK_PARAM, *PDTMB_GET_LOCK_PARAM;

typedef struct DLL_PACKED _DTMB_GET_MODULATION_MODE_PARAM
{
    MS_U8 id;
    DMD_DTMB_MODULATION_INFO info;
} DTMB_GET_MODULATION_MODE_PARAM, *PDTMB_GET_MODULATION_MODE_PARAM;

typedef struct DLL_PACKED _DTMB_GET_SIGNAL_STRENGTH_PARAM
{
    MS_U8 id;
    MS_U16 u16Strength;
} DTMB_GET_SIGNAL_STRENGTH_PARAM, *PDTMB_GET_SIGNAL_STRENGTH_PARAM;

typedef struct DLL_PACKED _DTMB_READ_FREQ_OFFSET_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_DTMB_CFO_DATA cfo;
    #else
    MS_S16 cfo;
    #endif
} DTMB_READ_FREQ_OFFSET_PARAM, *PDTMB_READ_FREQ_OFFSET_PARAM;

typedef struct DLL_PACKED _DTMB_GET_SIGNAL_QUALITY_PARAM
{
    MS_U8 id;
    MS_U8 u8Percentage;
} DTMB_GET_SIGNAL_QUALITY_PARAM, *PDTMB_GET_SIGNAL_QUALITY_PARAM;

typedef struct DLL_PACKED _DTMB_GET_BER_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_DTMB_BER_DATA ber;
    #else
    float ber;
    #endif
} DTMB_GET_BER_PARAM, *PDTMB_GET_BER_PARAM;

typedef struct DLL_PACKED _DTMB_GET_SNR_PARAM
{
    MS_U8 id;
    #ifdef UTPA2
    DMD_DTMB_SNR_DATA snr;
    #else
    float snr;
    #endif
} DTMB_GET_SNR_PARAM, *PDTMB_GET_SNR_PARAM;

typedef struct DLL_PACKED _DTMB_SET_SERIAL_CONTROL_PARAM
{
    MS_U8 id;
    MS_U8 u8TsConfigData;
} DTMB_SET_SERIAL_CONTROL_PARAM, *PDTMB_SET_SERIAL_CONTROL_PARAM;

typedef struct DLL_PACKED _DTMB_IIC_BYPASS_MODE_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} DTMB_IIC_BYPASS_MODE_PARAM, *PDTMB_IIC_BYPASS_MODE_PARAM;

typedef struct DLL_PACKED _DTMB_SWITCH_SSPI_GPIO_PARAM
{
    MS_U8 id;
    MS_BOOL bEnable;
} DTMB_SWITCH_SSPI_GPIO_PARAM, *PDTMB_SWITCH_SSPI_GPIO_PARAM;

typedef struct DLL_PACKED _DTMB_GPIO_LEVEL_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bLevel;
} DTMB_GPIO_LEVEL_PARAM, *PDTMB_GPIO_LEVEL_PARAM;

typedef struct DLL_PACKED _DTMB_DO_IQ_SWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} DTMB_DO_IQ_SWAP_PARAM, *PDTMB_DO_IQ_SWAP_PARAM;

typedef struct DLL_PACKED _DTMB_GPIO_OUT_ENABLE_PARAM
{
    MS_U8 id;
    MS_U8 u8Pin;
    MS_BOOL bEnableOut;
} DTMB_GPIO_OUT_ENABLE_PARAM, *PDTMB_GPIO_OUT_ENABLE_PARAM;

typedef struct DLL_PACKED _DTMB_DO_IQSWAP_PARAM
{
    MS_U8 id;
    MS_BOOL bIsQPad;
} DTMB_DO_IQSWAP_PARAM, *PDTMB_DO_IQSWAP_PARAM;

typedef struct DLL_PACKED _DTMB_REG_PARAM
{
    MS_U8 id;
    MS_U16 u16Addr;
    MS_U8 u8Data;
} DTMB_REG_PARAM, *PDTMB_REG_PARAM;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

DLL_PUBLIC extern void DTMBRegisterToUtopia(FUtopiaOpen ModuleType);
DLL_PUBLIC extern MS_U32 DTMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
DLL_PUBLIC extern MS_U32 DTMBClose(void* pInstance);
DLL_PUBLIC extern MS_U32 DTMBIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif

#endif // _DRV_ATSC_V2_H_

