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

#ifndef _HAL_SAR_H_
#define _HAL_SAR_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define HAL_SAR_CH1                     0
#define HAL_SAR_CH2                     1
#define HAL_SAR_CH3                     2
#define HAL_SAR_CH4                     3
#define HAL_SAR_CH5                     4
#define HAL_SAR_CH6                     5
#define HAL_SAR_CH7                     6
#define HAL_SAR_CH8                     7
//sar channel max channel id
#define HAL_SAR_CH_MAXID                (HAL_SAR_CH8+1)
#define HAL_SAR_ADC_DATA_MAX            0xFF

#define HAL_SAR_LEVEL                   1
#define HAL_SAR_EDGE                    0
#define HAL_SAR_FREERUN                 1
#define HAL_SAR_ONESHOT                 0

#define HAL_SAR_KPD_SUPPCHS             5
#define HAL_SAR_KPD_AVLBCHS             5

//Support RGB ADC HSync Channel Switch
#define HAL_SAR_ADC_HSYNC_CH0           0
#define HAL_SAR_ADC_HSYNC_CH1           1
#define HAL_SAR_ADC_HSYNC_CH2           2
#define HAL_SAR_ADC_HSYNC_CH3           3
#define HAL_SAR_ADC_HSYNC_MAX           4
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef void(*HAL_SAR_IsrProc)(void* pData);

typedef struct
{
    MS_U8 u8UpBnd;  //upper bound
    MS_U8 u8LoBnd;  //low bound
} HAL_SAR_BndCfg;

typedef struct
{
    MS_U8 u8SARChID;
    HAL_SAR_BndCfg tSARChBnd;
    MS_U8 u8KeyLevelNum;
    MS_U8 u8KeyThreshold[8];
    MS_U8 u8KeyCode[8];
} HAL_SAR_RegCfg;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void HAL_SAR_SetIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_SAR_GetIOMapBase(void);
void HAL_SAR_SetIOMapBase_NPM(MS_VIRT virtBase);
MS_VIRT HAL_SAR_GetIOMapBase_NPM(void);

MS_U8 HAL_SAR_CfgChannelBound(MS_U8 u8Channel ,HAL_SAR_BndCfg *psarBndCfg);
void HAL_SAR_CfgSingleChannel(MS_U8 u8Channel);
void HAL_SAR_CfgTriggerMode(MS_U8 u8Mode);
void HAL_SAR_CfgSingleChannelEn(MS_U8 bEnable);
void HAL_SAR_CfgDigitOperMode(MS_U8 u8Mode);
void HAL_SAR_CfgDigitPowerdown(MS_U8 bEnable);
void HAL_SAR_CfgStart(MS_U8 bEnable);
void HAL_SAR_CfgAtopPowerdown(MS_U8 bEnable);
void HAL_SAR_CfgAtopFreeRun(MS_U8 bEnable);
void HAL_SAR_CfgSelection(MS_U8 bEnable);
void HAL_SAR_CfgHighChannel(MS_U8 bEnable);
void HAL_SAR_CfgClockSamplePeriod(MS_U8 u8ClkSmpPrd);
void HAL_SAR_CfgAanlogInputSelect(MS_U8 u8Channel, MS_BOOL bEnable);
void HAL_SAR_CfgOutputEnable(MS_U8 u8Channel, MS_BOOL bEnable);
void HAL_SAR_SetOutput(MS_U8 u8Channel, MS_BOOL bHighLow);
MS_BOOL HAL_SAR_GetInput(MS_U8 u8Channel);
void HAL_SAR_CfgIntMask(MS_U8 bEnable);
void HAL_SAR_CfgIntClear(MS_U8 bEnable);
void HAL_SAR_CfgIntForce(MS_U8 bEnable);
MS_U8 HAL_SAR_GetIntStatus(void);
MS_U8 HAL_SAR_GetChannelADC(MS_U8 u8Channel);
MS_BOOL HAL_SAR_SetAdcHSyncChannel(MS_U8 u8Channel);
MS_BOOL HAL_SAR_Interrupt_EN(MS_U8 u8Channel, MS_BOOL bEnable);
MS_BOOL HAL_SAR_Wakeup_EN(MS_BOOL bEnable);
void HAL_SAR_CLR_INT(MS_U8 u8Channel);
MS_BOOL HAL_SAR_INT_Status(MS_U8 u8Channel);
void HAL_SAR_SET_LEVEL(MS_BOOL bLevel);

#ifdef CONFIG_MSTAR_T_SENSOR_ENABLE
//T-Sensor
MS_BOOL HAL_TSensorEnable(void);
MS_BOOL HAL_TSensorIRQEnable(void);
MS_BOOL HAL_TSensorIRQDisable(void);
MS_BOOL HAL_TSensorIRQClear(void);
MS_BOOL HAL_TSensorSetBound(MS_S16 s16UpBound_C , MS_S16 s16LowBound_C);
MS_S16 HAL_TSensorGetCurTemp_C(void);
MS_BOOL HAL_TSensorINTCheck(void);
MS_S16 HAL_TSensorGetUpBound(void);
MS_S16 HAL_TSensorGetLowBound(void);
#endif

#endif // _HAL_SAR_H_

