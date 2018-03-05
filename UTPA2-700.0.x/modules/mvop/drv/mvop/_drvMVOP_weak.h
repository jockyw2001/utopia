//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2spt_stMVOP_SUB_ENABLE010 - 2012 MStar Semiconductor, Inc. All rights reserved.
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
/// @file   _drvMVOP_weak.h
/// @brief  MVOP weak symbols declaration.
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MVOP_WEAK_H_
#define _DRV_MVOP_WEAK_H_

#include "halMVOP.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#if defined(MSOS_TYPE_LINUX_KERNEL)
#define __mvop_weak      __attribute__((weak))
#define __mvop_if        extern
#define __mvop_weak_0      {return 0;}
#define __mvop_if_0
#define __mvop_weak_v      {}
#define __mvop_if_v
#else
#define __mvop_weak      __attribute__((weak))
#define __mvop_if        extern
#define __mvop_weak_0    __attribute__((weak))
#define __mvop_if_0      extern
#define __mvop_weak_v    __attribute__((weak))
#define __mvop_if_v      extern
#endif
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//======================================================
// MAIN MVOP
__mvop_if void HAL_MVOP_SetVerticalMirrorMode(MS_BOOL bEnable) __mvop_weak;
__mvop_if void HAL_MVOP_SetHorizontallMirrorMode(MS_BOOL bEnable) __mvop_weak;
__mvop_if void HAL_MVOP_SetYUVBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack) __mvop_weak;
__mvop_if void HAL_MVOP_SetRepeatField(MVOP_RptFldMode eMode) __mvop_weak;
__mvop_if void HAL_MVOP_SetVSyncMode(MS_U8 u8Mode) __mvop_weak;
__mvop_if MS_PHY HAL_MVOP_GetYBaseAdd(void) __mvop_weak;
__mvop_if MS_PHY HAL_MVOP_GetUVBaseAdd(void) __mvop_weak;
__mvop_if_0 MS_BOOL HAL_MVOP_Set3DLRInsVBlank(MS_U16 u16InsLines) __mvop_weak_0;
__mvop_if MS_BOOL HAL_MVOP_Set3DLRAltOutput(MS_BOOL bEnable) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_Set3DLRAltOutput_VHalfScaling(MS_BOOL bEnable) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_Get3DLRAltOutput(void) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_Get3DLRAltSBSOutput(void) __mvop_weak;
__mvop_if EN_MVOP_Output_3D_TYPE HAL_MVOP_GetOutput3DType(void) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_Set3DLR2ndCfg(MS_BOOL bEnable) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_Get3DLR2ndCfg(void) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetRgbFormat(MVOP_RgbFormat eRgbFmt) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetSupportRgbFormat(MVOP_RgbFormat eRgbFmt) __mvop_weak;
__mvop_if MVOP_DrvMirror HAL_MVOP_GetMirrorMode(MVOP_DevID eID) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetVerDup(MS_BOOL bEnable) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetVerDup(void) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetVerx4Dup(MS_BOOL bEnable) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetVerx4Dup(void) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetHorx4Dup(MS_BOOL bEnable) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetHorx4Dup(void) __mvop_weak;
__mvop_if HALMVOPFREQUENCY HAL_MVOP_GetMaximumClk(void) __mvop_weak;
__mvop_if HALMVOPFREQUENCY HAL_MVOP_GetCurrentClk(void) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetYUVBaseAddMultiView(MVOP_BaseAddInput *stBaseAddInfo) __mvop_weak;
__mvop_if MS_PHY HAL_MVOP_GetYBaseAddMultiView(MVOP_3DView eView) __mvop_weak;
__mvop_if MS_PHY HAL_MVOP_GetUVBaseAddMultiView(MVOP_3DView eView) __mvop_weak;
__mvop_if MS_U16 HAL_MVOP_GetTopVStart(MVOP_DevID eID) __mvop_weak;
__mvop_if MS_U16 HAL_MVOP_GetBottomVStart(MVOP_DevID eID) __mvop_weak;
__mvop_if MS_U16 HAL_MVOP_GetVCount(MVOP_DevID eID) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetVC1RangeMap(MVOP_DevID eID, MVOP_VC1RangeMapInfo *stVC1RangeMapInfo) __mvop_weak;
__mvop_if void HAL_MVOP_SetEnable4k2kClk(MS_BOOL bEnable)__mvop_weak;
__mvop_if HALMVOPFREQUENCY HAL_MVOP_Get4k2kClk(void)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_Set420BWSaveMode(MS_BOOL bEnable)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SetEVDYUVBaseAdd(MVOP_EVDBaseAddInput *stEVDBaseAddInfo)__mvop_weak;
__mvop_if void HAL_MVOP_SetEVDFeature(MVOP_DevID eID, MVOP_EVDFeature* stEVDInput)__mvop_weak;
__mvop_if void HAL_MVOP_SetRptPreVsyncFrame(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak;
__mvop_if MVOP_HSMode HAL_MVOP_GetHandShakeMode(MVOP_DevID eID)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_CheckSTCCW(void)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetIsMiuIPControl(HALMVOPMIUCLIENTINFO stInfo)__mvop_weak;
__mvop_if void HAL_MVOP_SelMIU(MVOP_DevID eDevID, HALMVOPMIUSEL eMiuMSB0, HALMVOPMIUSEL eMiuMSB1, HALMVOPMIUSEL eMiuLSB0, HALMVOPMIUSEL eMiuLSB1)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetIsOnlyMiuIPControl(void)__mvop_weak;
__mvop_if void HAL_MVOP_SetDCSRAMClk(MS_U8 clkNum, MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SetEnable4k2k2P(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_GetMaxFramerate(MVOP_DevID eDevID, MVOP_GetMaxFps* stStreamInfo)__mvop_weak;
__mvop_if void HAL_MVOP_SetHandShakeMode(MVOP_DevID eDevID, MS_BOOL bEnable, MS_U8 u8Framerate)__mvop_weak;
__mvop_if void HAL_MVOP_SetCropforXC(MVOP_DevID eID, MVOP_XCGetCrop* stXCCrop, MS_U16 u16Width, MS_U16 u16Height)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SupportFRCOutputFPS(MVOP_DevID eID)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetIsCurrentHSK(MVOP_DevID eID)__mvop_weak;
__mvop_if_0 MS_BOOL HAL_MVOP_AutoTimingHDS(MVOP_DevID eID)__mvop_weak_0;
#ifdef UFO_MVOP_GET_IS_SW_CROP
__mvop_if_0 void HAL_MVOP_GetCropAddbySW(MVOP_DevID eID, MVOP_XCGetCropMirAdd* stCropAdd)__mvop_weak_0;
__mvop_if_0 MS_BOOL HAL_MVOP_GetIsSWCrop(void)__mvop_weak_0;
#endif
#ifdef UFO_MVOP_RESET_SETTING
__mvop_if void HAL_MVOP_ResetMVOPSetting(MVOP_DevID eDevID)__mvop_weak;
#endif
__mvop_if void HAL_MVOP_SetInterlaceType(MVOP_DevID eDevID, MS_U16 u16ECONumber, MS_U8 u8Interlace)__mvop_weak;
#ifdef UFO_MVOP_DOLBY_HDR
__mvop_if void HAL_MVOP_Enable2DCTimingSync(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_EnableHDRSetting(MS_BOOL bEnable)__mvop_weak;
#endif
__mvop_if void HAL_MVOP_SetSramPower(MVOP_DevID eID ,MS_BOOL bEnable)__mvop_weak;
#ifdef UFO_MVOP_FB_DEC_CLK
__mvop_if void HAL_MVOP_SetFBDecClk(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak;
#endif
#if defined(STB_DC_MODE)||(STB_DC)
__mvop_if HALMVOPFREQUENCY HAL_MVOP_SetDefaultClk(MVOP_DevID eDevID)__mvop_weak;
__mvop_if void HAL_MVOP_SetEnableFixClk(MS_BOOL bEnable)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_GetIsCurrentXCGenTiming(MVOP_DevID eID)__mvop_weak;
__mvop_if void HAL_MVOP_SetTimingFromXC(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_MaskDBRegCtrl(MVOP_DevID eID, MS_BOOL bEnable ,MVOPMSAKDBREGCTRL eModeCtrl)__mvop_weak;
#endif
#ifdef UFO_MVOP_GET_IS_MVOP_AUTO_GEN_BLACK
__mvop_if MS_BOOL HAL_MVOP_GetIsMVOPSupportBLKBackground(MVOP_DevID eID)__mvop_weak;
#endif
__mvop_if void HAL_MVOP_Exit(MVOP_DevID eID)__mvop_weak;
__mvop_if void HAL_MVOP_UVSwapEnable(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SetECONumber(MS_U16 u16ECOVersion)__mvop_weak;

//======================================================
// SUB MVOP
__mvop_if void HAL_MVOP_SubRegSetBase(MS_VIRT u32Base)__mvop_weak;
__mvop_if void HAL_MVOP_SubInit(void)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetFieldInverse(MS_BOOL b2MVD, MS_BOOL b2IP)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetChromaWeighting(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubLoadReg(void)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetMIUReqMask(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubRst(void)__mvop_weak;
__mvop_if void HAL_MVOP_SubEnable(MS_BOOL bEnable, MS_U8 u8Framerate)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubGetEnableState(void)__mvop_weak;
__mvop_if HALMVOPFREQUENCY HAL_MVOP_SubGetMaxFreerunClk(void)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetFrequency(HALMVOPFREQUENCY enFrequency)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetOutputInterlace(MS_BOOL bEnable, MS_U16 u16ECOVersion)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetPattern(MVOP_Pattern enMVOPPattern)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubSetTileFormat(MVOP_TileFormat eTileFmt)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubSetRgbFormat(MVOP_RgbFormat eRgbFmt) __mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubEnable3DLR(MS_BOOL bEnable)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubGet3DLRMode(void)__mvop_weak;
__mvop_if_0 MS_BOOL HAL_MVOP_SubSet3DLRInsVBlank(MS_U16 u16InsLines) __mvop_weak_0;
__mvop_if void HAL_MVOP_SubSetBlackBG(void)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetCropWindow(MVOP_InputCfg *pparam)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetInputMode( VOPINPUTMODE mode, MVOP_InputCfg *pparam, MS_U16 u16ECOVersion  )__mvop_weak;
__mvop_if void HAL_MVOP_SubEnableUVShift(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetEnable60P(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetOutputTiming( MVOP_Timing *ptiming )__mvop_weak;
__mvop_if void HAL_MVOP_SubSetDCClk(MS_U8 clkNum, MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetDCSRAMClk(MS_U8 clkNum, MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetSynClk(MVOP_Timing *ptiming)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetMonoMode(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetH264HardwireMode(MS_U16 u16ECOVersion)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetRMHardwireMode(MS_U16 u16ECOVersion)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetJpegHardwireMode(MS_U16 u16ECOVersion)__mvop_weak;
__mvop_if void HAL_MVOP_SubEnableMVDInterface(MS_BOOL bEnable)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubGetTimingInfoFromRegisters(MVOP_TimingInfo_FromRegisters *pMvopTimingInfo)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetHorizontallMirrorMode(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetVerticalMirrorMode(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubEnableFreerunMode(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetYUVBaseAdd(MS_PHY u32YOffset, MS_PHY u32UVOffset, MS_BOOL bProgressive, MS_BOOL b422pack)__mvop_weak;
__mvop_if EN_MVOP_Output_3D_TYPE HAL_MVOP_SubGetOutput3DType(void) __mvop_weak;
__mvop_if MS_PHY HAL_MVOP_SubGetYBaseAdd(void) __mvop_weak;
__mvop_if MS_PHY HAL_MVOP_SubGetUVBaseAdd(void) __mvop_weak;
//__mvop_if MS_BOOL HAL_MVOP_SubSetVC1RangeMap(MVOP_VC1RangeMapInfo *stVC1RangeMapInfo) __mvop_weak;
__mvop_if void HAL_MVOP_SubSetEnable4k2kClk(MS_BOOL bEnable)__mvop_weak;
__mvop_if HALMVOPFREQUENCY HAL_MVOP_SubGet4k2kClk(void)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubSet420BWSaveMode(MS_BOOL bEnable, MS_U16 u16ECOVersion)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubSetEVDYUVBaseAdd(MVOP_EVDBaseAddInput *stEVDBaseAddInfo)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubCheckSTCCW(void)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetEnable4k2k2P(MS_BOOL bEnable)__mvop_weak;
__mvop_if void HAL_MVOP_SubSetVSyncMode(MS_U8 u8Mode)__mvop_weak;
__mvop_if MS_BOOL HAL_MVOP_SubSet3DLRAltOutput_VHalfScaling(MS_BOOL bEnable)__mvop_weak;
#ifdef UFO_MVOP_DOLBY_HDR
__mvop_if void HAL_MVOP_SubEnableHDRSetting(MS_BOOL bEnable)__mvop_weak;
#endif
//======================================================
// EX MVOP:: Extended HAL functions for MVOP modules
__mvop_if_v void HAL_MVOP_EX_RegSetBase(MS_VIRT u32Base)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_InitMirrorMode(MVOP_DevID eID, MS_BOOL bMir)__mvop_weak_v;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_Init(MVOP_DevID eID)__mvop_weak_0;
__mvop_if_v void HAL_MVOP_EX_SetVerticalMirrorMode(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetHorizontallMirrorMode(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetFieldInverse(MVOP_DevID eID, MS_BOOL b2MVD, MS_BOOL b2IP)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetChromaWeighting(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_LoadReg(MVOP_DevID eID)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetMIUReqMask(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_Rst(MVOP_DevID eID)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_Enable(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_GetEnableState(MVOP_DevID eID)__mvop_weak_0;
__mvop_if_0 HALMVOPFREQUENCY HAL_MVOP_EX_GetMaxFreerunClk(MVOP_DevID eID)__mvop_weak_0;
__mvop_if_0 HALMVOPFREQUENCY HAL_MVOP_EX_GetClk(MS_U32 u32PixClk)__mvop_weak_0;
__mvop_if_v void HAL_MVOP_EX_SetFrequency(MVOP_DevID eID, HALMVOPFREQUENCY enFrequency)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetOutputInterlace(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetPattern(MVOP_DevID eID, MVOP_Pattern enMVOPPattern)__mvop_weak_v;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_SetTileFormat(MVOP_DevID eID, MVOP_TileFormat eTileFmt)__mvop_weak_0;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_SetRgbFormat(MVOP_DevID eID, MVOP_RgbFormat eRgbFmt)__mvop_weak_0;
__mvop_if_v void HAL_MVOP_EX_SetBlackBG(MVOP_DevID eID)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetCropWindow(MVOP_DevID eID, MVOP_InputCfg *pparam)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetInputMode(MVOP_DevID eID, VOPINPUTMODE mode, MVOP_InputCfg *pparam )__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_EnableUVShift(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetEnable60P(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_0 MVOP_Result HAL_MVOP_EX_EnableFreerunMode(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_0;
__mvop_if_v void HAL_MVOP_EX_SetOutputTiming(MVOP_DevID eID, MVOP_Timing *ptiming )__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetDCClk(MVOP_DevID eID, MS_U8 clkNum, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetSynClk(MVOP_DevID eID, MVOP_Timing *ptiming, HALMVOPFREQUENCY eFreq)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetMonoMode(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetH264HardwireMode(MVOP_DevID eID)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_EnableMVDInterface(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetRMHardwireMode(MVOP_DevID eID)__mvop_weak_v;
__mvop_if_v void HAL_MVOP_EX_SetJpegHardwireMode(MVOP_DevID eID)__mvop_weak_v;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_Enable3DLR(MVOP_DevID eID, MS_BOOL bEnable)__mvop_weak_0;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_Get3DLRMode(MVOP_DevID eID)__mvop_weak_0;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_Set3DLRInsVBlank(MS_U16 u16InsLines) __mvop_weak_0;
__mvop_if_0 MS_BOOL HAL_MVOP_EX_GetTimingInfoFromRegisters(MVOP_DevID eID, MVOP_TimingInfo_FromRegisters *pMvopTimingInfo)__mvop_weak_0;
__mvop_if_0 EN_MVOP_Output_3D_TYPE HAL_MVOP_EX_GetOutput3DType(MVOP_DevID eID)__mvop_weak_0;

#if (defined(STB_DC_MODE)||(STB_DC))&&(MVOP_SUPPORT_SUB)
__mvop_if void HAL_MVOP_MaskDBRegCtrl(MVOP_DevID eID, MS_BOOL bEnable ,MVOPMSAKDBREGCTRL eModeCtrl)__mvop_weak;
#endif
__mvop_if MS_BOOL HAL_MVOP_IntEnableMask(MVOP_DevID eID, MS_U8 eIntType) __mvop_weak;
__mvop_if MS_U8 HAL_MVOP_IntGetStatus(MVOP_DevID eID) __mvop_weak;

#endif // _DRV_MVOP_WEAK_H_
