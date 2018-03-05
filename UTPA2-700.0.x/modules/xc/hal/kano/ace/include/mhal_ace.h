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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
#ifndef MHAL_ACE_H
#define MHAL_ACE_H

/// Define XC ACE device number
typedef enum
{
    XC_ACE_DEVICE0 = 0,
    XC_ACE_DEVICE1 = 1,
    XC_ACE_MAX_DEVICE_NUM
} XC_ACE_DEVICE_NUM;

#define XC_ACE_SUPPORT_DEVICE_NUM XC_ACE_MAX_DEVICE_NUM
#define ACE_NOT_IN_KERNEL

typedef enum
{
    E_HALACE_DEVICE0_XC_BANK_OFFSET    = 0,
    E_HALACE_DEVICE1_XC_BANK_OFFSET    = 0x80,
} ACE_HAL_DEVICE_XC_BANK_OFFSET;

void Hal_ACE_DMS( void* pInstance, MS_BOOL bScalerWin, MS_BOOL bisATV );
void Hal_ACE_GetColorMatrix( void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix);
void Hal_ACE_SetColorMatrix( void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix);
MS_BOOL Hal_ACE_SetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val);
MS_BOOL Hal_ACE_GetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8* pu8Val);
MS_BOOL Hal_ACE_SetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val);
MS_BOOL Hal_ACE_GetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eIHC, MS_U8* pu8Val);
MS_BOOL Hal_ACE_SetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val);
MS_BOOL Hal_ACE_GetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIHC, MS_U8* pu8Val);

void Hal_ACE_PatchDTGColorChecker(void* pInstance, MS_U8 u8Mode);
void Hal_ACE_SetSlopValue( void* pInstance, MS_BOOL bScalerWin, MS_U8 u8SlopValue);
void Hal_ACE_SetFCC_En( void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Reg, MS_BOOL bEn);
void Hal_ACE_SetFCC_Cb(void* pInstance, MS_U8 u8Reg, MS_U8 u8value);
void Hal_ACE_init_riu_base(MS_VIRT ptr_riu_base);
void Hal_ACE_SetSharpness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Sharpness);
void Hal_ACE_SetSkipWaitVsync( void* pInstance, MS_BOOL bIsMainSkipWaitVsyn,MS_BOOL bIsSubSkipWaitVsyn);
MS_BOOL Hal_ACE_GetSkipWaitVsync( void* pInstance, MS_BOOL bScalerWin);

void Hal_ACE_SetBrightness( void* pInstance, MS_BOOL bScalerWin, MS_U8 u8RedBrightness, MS_U8 u8GreenBrightness, MS_U8 u8BlueBrightness );
void Hal_ACE_SetBrightnessPrecise(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedBrightness, MS_U16 u16GreenBrightness, MS_U16 u16BlueBrightness);
void Hal_ACE_GetBrightness(void* pInstance, MS_BOOL bScalerWin, MS_U8* u8RedBrightness, MS_U8* u8GreenBrightness, MS_U8* u8BlueBrightness);
void Hal_ACE_SetPostRGBGain(void* pInstance, MS_BOOL bScalerWin, MS_U16 u8RedGain, MS_U16 u8GreenGain, MS_U16 u8BlueGain);
void Hal_ACE_SetPostRGBOffset(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset);
void Hal_ACE_SetPostRGBGainOffsetTogether(void* pInstance, MS_BOOL bScalerWin,
                                          MS_U16 u16RedGain,   MS_U16 u16GreenGain,   MS_U16 u16BlueGain,
                                          MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset);
void Hal_ACE_Set_IHC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt);
void Hal_ACE_Set_ICC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16Cnt);
void Hal_ACE_Write_Color_Matrix_Burst( void* pInstance, MS_BOOL bWindow, MS_U16* psMatrix );
MS_BOOL Hal_ACE_Is_Support_MLoad( void* pInstance );

void Hal_ACE_MWESetWin(void* pInstance, MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend);
void Hal_ACE_MWEEnable(void* pInstance, MS_BOOL ben, MS_BOOL bLoadFromTable);
void Hal_ACE_MWESetBorder( void* pInstance, MS_BOOL bScalerWin, MS_U16 u16color, MS_U16 u16_l, MS_U16 u16_r, MS_U16 u16_t, MS_U16 u16_d);
void Hal_ACE_MWECloneVisualEffect(void* pInstance);
void Hal_ACE_3DClonePQMap(void* pInstance, XC_ACE_WEAVETYPE enWeaveType);

MS_U8 Hal_ACE_DNR_GetMotion(void* pInstance);
MS_U8 Hal_ACE_DNR_GetCoringThreshold(void* pInstance, MS_BOOL bScalerWin);
MS_U8 Hal_ACE_DNR_GetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin);
MS_U8 Hal_ACE_DNR_GetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin);


void Hal_ACE_DNR_SetCoringThreshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNM_V(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGNR_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGNR_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetCP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetDP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNM_H_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNM_H_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGray_Ground_Gain(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetGray_Ground_En(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetSC_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetSpikeNR_0(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetSpikeNR_1(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetSNR_NM(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetBank_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNRTbl_Y(void* pInstance, MS_U8 u8Idx, MS_U16 u16val);
void Hal_ACE_DNR_SetNRTbl_C(void* pInstance, MS_U8 u8Idx, MS_U16 u16val);
void HAL_ACE_Set_RBChannelOffset(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable);
void Hal_ACE_Set_Device_Bank_Offset(void);

// Function in mdrv_xc_menuload.c
typedef enum
{
    E_MLOAD_UNSUPPORTED = 0,
    E_MLOAD_DISABLED    = 1,
    E_MLOAD_ENABLED     = 2,
}MLOAD_TYPE;

typedef enum
{
    E_MLG_UNSUPPORTED = 0,
    E_MLG_DISABLED    = 1,
    E_MLG_ENABLED     = 2,
}MLG_TYPE;

/// Define mute type
typedef enum
{
    E_MAIN_WINDOW_MUTE =0,          ///< main window mute
    E_SUB_WINDOW_MUTE =1,           ///< sub window mute
    E_PANEL_MUTE = 2            ///< panel mute
}EN_MUTE_TYPE;

extern MS_BOOL MApi_XC_MLoad_Fire(void* pInstance, MS_BOOL bImmediate);
extern MS_BOOL MApi_XC_MLoad_WriteCmd(void* pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire(void* pInstance, MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt);
extern MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire(void* pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MLOAD_TYPE MApi_XC_MLoad_GetStatus_U2(void *pInstance);
extern MS_BOOL MApi_XC_GetForceWrite( EN_MUTE_TYPE enMuteType);


extern MS_S32 _MLOAD_MUTEX;

#define MLOAD_MUTEX
#define MLOAD_MUTEX_DBG 0

#ifdef MLOAD_MUTEX
    #if(MLOAD_MUTEX_DBG)
        #define _MLOAD_ENTRY()                                                         \
                printf("1,==========================\n");                              \
                printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
                if(!MsOS_ObtainMutex(_MLOAD_MUTEX, MSOS_WAIT_FOREVER))                 \
                {                                                                       \
                    printf("==========================\n");                             \
                    printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
                }
        #define _MLOAD_RETURN()                                                       \
                printf("0,==========================\n");                              \
                printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
                MsOS_ReleaseMutex(_MLOAD_MUTEX);
                                    //return _ret;
    #else
        #define _MLOAD_ENTRY()                                               \
                if(!MsOS_ObtainMutex(_MLOAD_MUTEX, MSOS_WAIT_FOREVER))      \
                {                                                            \
                }
        #define _MLOAD_RETURN()    MsOS_ReleaseMutex(_MLOAD_MUTEX);
                                    //return _ret;
    #endif
#else // #if not def MLOAD_MUTEX
    #define _MLOAD_ENTRY()         while(0)
    #define _MLOAD_RETURN()    while(0)
#endif // #ifdef _MLOAD_MUTEX

#endif


