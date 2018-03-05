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
/// @file   _apiVDEC_weak.h
/// @brief  VDEC element API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_VDEC_WEAK_H_
#define _API_VDEC_WEAK_H_

#include "drvMVD_EX.h"
#include "drvHVD_EX.h"

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define __vdec_weak      __attribute__((weak))
#define __vdec_if            extern

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//======================================================
// strong symbol ( necessary!!! can not remove for elf compile envi)
__vdec_if MS_BOOL MDrv_MVD_LinkWeakSymbolPatch(void);
__vdec_if MS_BOOL MDrv_HVD_LinkWeakSymbolPatch(void);



//======================================================
// MVD
__vdec_if E_MVD_Result MDrv_MVD_SetCfg(MS_U32 u32Id, MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetFWVer(MS_U32 u32Id) __vdec_weak;
__vdec_if void MDrv_MVD_SetDbgLevel(MS_U8 level) __vdec_weak;
__vdec_if const MVD_DrvInfo* MDrv_MVD_GetInfo(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetLibVer(const MSIF_Version **ppVersion) __vdec_weak;

__vdec_if void MDrv_MVD_SetCodecInfo(MS_U32 u32Id, MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing) __vdec_weak;
__vdec_if void MDrv_MVD_SetDivXCfg(MS_U32 u32Id, MS_U8 u8MvAdjust, MS_U8 u8IdctSel) __vdec_weak;

__vdec_if void MDrv_MVD_SetFrameBuffAddr(MS_U32 u32Id, MS_VIRT u32addr) __vdec_weak;
__vdec_if void MDrv_MVD_GetFrameInfo(MS_U32 u32Id, MVD_FrameInfo *pinfo ) __vdec_weak;
__vdec_if void MDrv_MVD_SetOverflowTH (MS_U32 u32Id, MS_U32 u32Threshold) __vdec_weak;
__vdec_if void MDrv_MVD_SetUnderflowTH (MS_U32 u32Id, MS_U32 u32Threshold) __vdec_weak;

__vdec_if void MDrv_MVD_RstIFrameDec(MS_U32 u32Id,MVD_CodecType eCodecType) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsIFrameDecoding(MS_U32 u32Id) __vdec_weak;

__vdec_if MS_U8 MDrv_MVD_GetSyncStatus(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetIsIPicFound(MS_U32 u32Id) __vdec_weak;

//for MM
__vdec_if MS_U32 MDrv_MVD_GetResidualStreamSize(MS_U32 u32Id) __vdec_weak;

__vdec_if MS_BOOL MDrv_MVD_DecodeIFrame(MS_U32 u32Id, MS_PHY u32FrameBufAddr, MS_PHY u32StreamBufAddr, MS_PHY u32StreamBufEndAddr ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetValidStreamFlag(MS_U32 u32Id) __vdec_weak;

//for MM
__vdec_if void MDrv_MVD_SetFrameInfo(MS_U32 u32Id, MVD_FrameInfo *pinfo ) __vdec_weak;
__vdec_if void MDrv_MVD_GetErrInfo(MS_U32 u32Id, MVD_ErrCode *errCode, MVD_ErrStatus *errStatus) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetSkipPicCounter(MS_U32 u32Id) __vdec_weak;

__vdec_if MS_U8 MDrv_MVD_GetSLQAvailableLevel(MS_U32 u32Id) __vdec_weak;
__vdec_if void MDrv_MVD_SetSLQWritePtr(MS_U32 u32Id, MS_BOOL bCheckData) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetSLQReadPtr(MS_U32 u32Id) __vdec_weak;

__vdec_if MVD_PicType MDrv_MVD_GetPicType(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetBitsRate(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetVideoRange(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetLowDelayFlag(MS_U32 u32Id) __vdec_weak;

__vdec_if void MDrv_MVD_Pause(MS_U32 u32Id) __vdec_weak;
__vdec_if void MDrv_MVD_Resume(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_StepDisp(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsStepDispDone(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_StepDecode(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsStepDecodeDone(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SeekToPTS(MS_U32 u32Id, MS_U32 u32Pts) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsStep2PtsDone(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SkipToPTS(MS_U32 u32Id, MS_U32 u32Pts) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_TrickPlay(MS_U32 u32Id, MVD_TrickDec trickDec, MS_U8 u8DispDuration) __vdec_weak;
__vdec_if void MDrv_MVD_EnableForcePlay(MS_U32 u32Id) __vdec_weak;

__vdec_if void MDrv_MVD_RegSetBase(MS_VIRT u32RegBaseAddr) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_Init(MS_U32 u32Id,MVD_CodecType eCodecType) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_Exit(MS_U32 u32Id) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_Rst(MS_U32 u32Id,MVD_CodecType eCodecType) __vdec_weak;

__vdec_if void MDrv_MVD_Play(MS_U32 u32Id) __vdec_weak;
__vdec_if void MDrv_MVD_SetAVSync(MS_U32 u32Id, MS_BOOL bEnable, MS_U32 u32Delay) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetAVSyncDelay(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsAVSyncOn(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsSyncRep(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsSyncSkip(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_ChangeAVsync(MS_U32 u32Id, MS_BOOL bEnable, MS_U16 u16PTS) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_DispCtrl(MS_U32 u32Id, MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_DispRepeatField(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;

__vdec_if MS_U8 MDrv_MVD_GetActiveFormat(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetDispRdy(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_Is1stFrmRdy(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetGOPCount(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetPicCounter(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetParserByteCnt(MS_U32 u32Id) __vdec_weak;
__vdec_if MVD_DecStat MDrv_MVD_GetDecodeStatus(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetLastCmd(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetVldErrCount(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_DropErrorFrame(MS_U32 u32Id, MS_BOOL bDrop) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_MVDCommand ( MS_U8 u8cmd, MVD_CmdArg *pstCmdArg ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SkipData(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SkipToIFrame(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetCaps(MVD_Caps* pCaps) __vdec_weak;

__vdec_if E_MVD_Result MDrv_MVD_DisableErrConceal(MS_U32 u32Id, MS_BOOL bDisable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_PushQueue(MS_U32 u32Id, MVD_PacketInfo* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_FlushQueue(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FlushDisplayBuf(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetQueueVacancy(MS_U32 u32Id, MS_BOOL bCached) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetESReadPtr(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetESWritePtr(MS_U32 u32Id) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_EnableLastFrameShow(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_IsDispFinish(MS_U32 u32Id) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetSpeed(MS_U32 u32Id, MVD_SpeedType eSpeedType, MS_U8 u8Multiple) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_ResetPTS(MS_U32 u32Id, MS_U32 u32PtsBase) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetPTS(MS_U32 u32Id) __vdec_weak;
__vdec_if MVD_TrickDec MDrv_MVD_GetTrickMode(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsPlaying(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsIdle(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsSeqChg(MS_U32 u32Id) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_DbgSetData(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32 u32Data) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_DbgGetData(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32* u32Data) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetDecodedFrameIdx (MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetFileModeAVSync(MS_U32 u32Id, MVD_TIMESTAMP_TYPE eSyncMode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsAllBufferEmpty(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GenPattern(MS_U32 u32Id, MVD_PatternType ePattern, MS_PHY u32PAddr, MS_U32* pu32Size) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetPatternInfo(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetVirtualBox(MS_U32 u32Id, MS_U16 u16Width, MS_U16 u16Height) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetBlueScreen(MS_U32 u32Id, MS_BOOL bEn) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_EnableDispOneField(MS_U32 u32Id, MS_BOOL bEn) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetFrmInfo(MS_U32 u32Id, MVD_FrmInfoType eType, MVD_FrmInfo* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetFreezeDisp(MS_U32 u32Id, MS_BOOL bEn) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetVSizeAlign(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if void MDrv_MVD_DbgDump(MS_U32 u32Id) __vdec_weak;
__vdec_if void MDrv_MVD_SetAVSyncThreshold(MS_U32 u32Id, MS_U32 u32Th) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetFdMaskDelayCount(MS_U32 u32Id, MS_U16 u16Cnt) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetOutputFRCMode(MS_U32 u32Id, MS_U8 u8FrameRate, MS_U8 u8Interlace) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetFRCDropType(MS_U32 u32Id, MS_U8 u8DropType) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDisableSeqChange(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_ParserRstDone(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetChromaFormat(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetMcuMode(MS_U32 u32Id, MS_U8 u8Mode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_ReleaseFdMask(MS_U32 u32Id, MS_BOOL bRls) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_ShowDecodeOrder(MS_U32 u32Id, MS_U8 u8Mode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SuspendDynamicScale(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetAutoMute(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDispFinishMode(MS_U32 u32Id,MS_U8 u8Mode) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetAVSyncMode(MS_U8 u8Mode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetIdctMode(MS_U32 u32Id, MS_U8 u8Mode) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_EX_SetMVDClockSpeed(MVD_EX_ClockSpeed eClockSpeed) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDbgMode(MS_U32 u32Id, MVD_DbgMode enDbgMode, MS_BOOL bEn) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FlushPTSBuf(MS_U32 u32Id,MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetMatrixCoef(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsDynScalingEnabled(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetSingleDecodeMode(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_U64 MDrv_MVD_GetU64PTS(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIs32PullDown(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_S32 MDrv_MVD_GetPtsStcDiff(MS_U32 u32Id) __vdec_weak;
__vdec_if void MDrv_MVD_SetAVSyncFreerunThreshold(MS_U32 u32Id, MS_U32 u32Th) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsFreerun(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetNextPTS(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetColorFormat(MS_U32 u32Id) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDynScalingParam(MS_U32 u32Id, MS_PHY u32StAddr, MS_VIRT u32Size) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetExtDispInfo(MS_U32 u32Id, MVD_ExtDispInfo* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetTimeCode(MS_U32 u32Id, MVD_FrmInfoType eType, MVD_TimeCode* pInfo) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetUsrDataIsAvailable(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetUsrDataInfo(MS_U32 u32Id, MVD_UsrDataInfo* pUsrInfo) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetDispCount(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetMStreamerMode(MS_U32 u32Id, MS_U8 u8Mode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FrameFlip(MS_U32 u32Id, MS_U8 u8FrmIdx) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FrameRelease(MS_U32 u32Id, MS_U8 u8FrmIdx) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FrameCapture(MS_U32 u32Id, MS_U8 u8FrmIdx, MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_Init_Share_Mem(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetFreeStream(MS_U32 *pu32Id, MVD_DRV_StreamType eStreamType) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetSLQNum(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetFrmRateIsSupported(MS_U32 u32Id) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetCrcValue(MS_U32 u32Id, MVD_CrcIn *pCrcIn, MVD_CrcOut *pCrcOut) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetDispQNum(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetDivxVer(MS_U32 u32Id) __vdec_weak;

#ifdef MVD_ENABLE_ISR
__vdec_if E_MVD_Result MDrv_MVD_SetIsrEvent(MS_U32 u32Id, MS_U32 eEvent, MVD_InterruptCb fnHandler) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetIsrEvent(MS_U32 u32Id) __vdec_weak;
#else
#define MDrv_MVD_SetIsrEvent(x, y, z) (E_MVD_RET_OK)
#define MDrv_MVD_GetIsrEvent(x)     (MVD_Event)0
#endif


//======================================================
// HVD
__vdec_if void MDrv_HVD_EX_PowerCtrl(MS_BOOL bEnable) __vdec_weak;
__vdec_if void MDrv_HVD_EX_SetOSRegBase(MS_U32 u32RegBaseAddr) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_Init(MS_U32 u32Id, HVD_EX_MemCfg *mem, HVD_EX_InitSettings *InitParam) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_Rst(MS_U32 u32Id, MS_BOOL bErrHandle) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_Play(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_Exit(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_Pause(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_Flush(MS_U32 u32Id, MS_BOOL bShowLast) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_StepDisp(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_StepDecode(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_PushQueue(MS_U32 u32Id, HVD_EX_PacketInfo *pInfo) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_PushQueue_Fire(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_DecodeIFrame(MS_U32 u32Id, MS_PHY u32SrcSt, MS_VIRT u32SrcSize) __vdec_weak;

__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDataEnd(MS_U32 u32Id, MS_BOOL bEnd) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDispErrFrm(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDispRepeatField(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSkipDecMode(MS_U32 u32Id, HVD_EX_SkipDecode eDecType) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDispSpeed(MS_U32 u32Id, HVD_EX_DispSpeed eSpeed) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSyncActive(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDropMode(MS_U32 u32Id, HVD_EX_DropDisp eMode , MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_RstPTS(MS_U32 u32Id, MS_U32 u32PTS) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetFrcMode(MS_U32 u32Id, HVD_EX_FrmRateConvMode eMode) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSyncTolerance(MS_U32 u32Id, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSyncVideoDelay(MS_U32 u32Id, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSyncFreeRunTH(MS_U32 u32Id, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSyncRepeatTH(MS_U32 u32Id, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetErrConceal(MS_U32 u32Id, MS_BOOL u32Arg) __vdec_weak;
__vdec_if void MDrv_HVD_EX_SetDbgLevel(HVD_EX_UartLevel elevel) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SeekToPTS(MS_U32 u32Id, MS_U32 u32PTS) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SkipToPTS(MS_U32 u32Id, MS_U32 u32PTS) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetFreezeImg(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetBlueScreen(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDispOneField(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetISREvent(MS_U32 u32Id, MS_U32 u32Event ,HVD_InterruptCb fnISRHandler) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_SetEnableISR(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_SetForceISR(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_SetMVOPDone(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetVirtualBox(MS_U32 u32Id, MS_U16 u16Width, MS_U16 u16Height) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDynScalingParam(MS_U32 u32Id, void *pStAddr, MS_U32 u32Size) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetAutoRmLstZeroByte(MS_U32 u32Id, MS_BOOL bOn) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetDispInfoTH(MS_U32 u32Id, HVD_EX_DispInfoThreshold* DispInfoTH ) __vdec_weak;

__vdec_if MS_BOOL MDrv_HVD_EX_IsISROccured(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsDispFinish(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsFrameShowed(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsStepDecodeDone(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_CheckDispInfoRdy(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsDispInfoChg(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsIdle(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsSyncStart(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsSyncReach(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsLowDelay(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsIFrmFound(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_Is1stFrmRdy(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_IsAllBufferEmpty(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_IsAlive(MS_U32 u32Id) __vdec_weak;

__vdec_if MS_U32 MDrv_HVD_EX_GetBBUVacancy(MS_U32 u32Id) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_GetDispInfo(MS_U32 u32Id, HVD_EX_DispInfo *pinfo) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetPTS(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetDataErrCnt(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetDecErrCnt(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetESWritePtr(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetESReadPtr(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetESQuantity(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_GetCaps(HVD_EX_Codec u32Type) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetErrCode(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetPlayMode(MS_U32 u32Id, HVD_EX_GetModeStatus eMode) __vdec_weak;
__vdec_if HVD_EX_GetPlayState MDrv_HVD_EX_GetPlayState(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetDecodeCnt(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U8 MDrv_HVD_EX_GetActiveFormat(MS_U32 u32Id) __vdec_weak;
__vdec_if const HVD_EX_DrvInfo* MDrv_HVD_EX_GetInfo( void ) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_GetLibVer(const MSIF_Version **ppVersion) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_GetStatus(MS_U32 u32Id, HVD_EX_DrvStatus *pstatus) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_GetFrmInfo(MS_U32 u32Id, HVD_EX_GetFrmInfoType eType, HVD_EX_FrameInfo *pInfo) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_EX_GetISRInfo(MS_U32 u32Id, MS_U32* eType) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_CalLumaSum(MS_U32 u32Id, HVD_EX_GetFrmInfoType eType) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetUserData_Wptr(MS_U32 u32Id) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetUserData_Packet(MS_U32 u32Id, MS_U32 u32Idx , MS_U32* u32Size) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_GenPattern(MS_U32 u32Id, HVD_EX_PatternType eType, MS_U32 u32VAddr, MS_U32 *u32Size) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetPatternInfo(MS_U32 u32Id, HVD_EX_PatternInfo eType) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetDynamicScalingInfo(MS_U32 u32Id, HVD_EX_DynamicScalingInfo eType) __vdec_weak;

__vdec_if MS_U32 MDrv_HVD_EX_GetData(MS_U32 u32Id, HVD_EX_GDataType eType) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_EX_GetMem_Dbg(MS_U32 u32Id, MS_VIRT u32Addr) __vdec_weak;
__vdec_if void MDrv_HVD_EX_DbgDumpStatus(MS_U32 u32Id, HVD_EX_DumpStatus eFlag) __vdec_weak;
__vdec_if void MDrv_HVD_EX_SetMem_Dbg(MS_U32 u32Id, MS_VIRT u32Addr, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetCmd_Dbg(MS_U32 u32Id, MS_U32 u32Cmd, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_SetSettings_Pro(MS_U32 u32Id, HVD_EX_SSettingsType eType, MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_EX_Result MDrv_HVD_EX_DispOutsideMode(MS_U32 u32Id, MS_BOOL bEnable) __vdec_weak;
#endif
#endif // _API_VDEC_WEAK_H_
