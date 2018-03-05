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

#include "../../drv/mvd/drvMVD.h"
#include "../../drv/hvd/drvHVD.h"
#include "../../drv/rvd/drvRVD.h"


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
__vdec_if MS_BOOL MDrv_RVD_LinkWeakSymbolPatch(void);


//======================================================
// MVD
__vdec_if E_MVD_Result MDrv_MVD_SetCfg(MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg, MVD_InfoCfg* InfoCfg) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetFWVer(void) __vdec_weak;
__vdec_if void MDrv_MVD_SetDbgLevel(MS_U8 level) __vdec_weak;
__vdec_if const MVD_DrvInfo* MDrv_MVD_GetInfo(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetLibVer(const MSIF_Version **ppVersion) __vdec_weak;
__vdec_if void MDrv_MVD_GetStatus(MVD_DrvStatus* pDrvStatus) __vdec_weak;
__vdec_if MVD_InternalMemCfg *MDrv_MVD_GetInternalMemCfg(void) __vdec_weak;

__vdec_if void MDrv_MVD_SetCodecInfo(MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing) __vdec_weak;
__vdec_if void MDrv_MVD_SetDivXCfg(MS_U8 u8MvAdjust, MS_U8 u8IdctSel) __vdec_weak;

__vdec_if MS_BOOL MDrv_MVD_SetBitStreamAddr(MS_U32 u32start, MS_U32 u32end) __vdec_weak;
__vdec_if void MDrv_MVD_SetFrameBuffAddr(MS_U32 u32addr) __vdec_weak;
__vdec_if void MDrv_MVD_GetFrameInfo(MVD_FrameInfo *pinfo ) __vdec_weak;
__vdec_if void MDrv_MVD_SetOverflowTH (MS_U32 u32Threshold) __vdec_weak;
__vdec_if void MDrv_MVD_SetUnderflowTH (MS_U32 u32Threshold) __vdec_weak;

__vdec_if void MDrv_MVD_RstIFrameDec( void ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsIFrameDecoding( void ) __vdec_weak;

__vdec_if MS_U8 MDrv_MVD_GetSyncStatus( void ) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetIsIPicFound( void ) __vdec_weak;

//for MM
__vdec_if MS_U32 MDrv_MVD_GetResidualStreamSize( void ) __vdec_weak;

__vdec_if MS_BOOL MDrv_MVD_DecodeIFrame(MS_PHYADDR u32FrameBufAddr, MS_PHYADDR u32StreamBufAddr, MS_PHYADDR u32StreamBufEndAddr ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetValidStreamFlag( void ) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetChromaFormat(void) __vdec_weak;

//for MM
__vdec_if void MDrv_MVD_SetFrameInfo(MVD_FrameInfo *pinfo ) __vdec_weak;
__vdec_if void MDrv_MVD_SetSLQStartEnd(  MS_U32 u32start, MS_U32 u32end) __vdec_weak;
__vdec_if void MDrv_MVD_GetErrInfo(MVD_ErrCode *errCode, MVD_ErrStatus *errStatus) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetSkipPicCounter( void ) __vdec_weak;

__vdec_if MS_U8 MDrv_MVD_GetSLQAvailableLevel(void) __vdec_weak;
__vdec_if void MDrv_MVD_SetSLQWritePtr(MS_BOOL bCheckData) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetSLQReadPtr( void ) __vdec_weak;
__vdec_if void MDrv_MVD_SetSLQTblBufStartEnd( MS_U32 u32start, MS_U32 u32end ) __vdec_weak;

__vdec_if MVD_PicType MDrv_MVD_GetPicType( void ) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetBitsRate( void ) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetVideoRange(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetLowDelayFlag( void ) __vdec_weak;

__vdec_if void MDrv_MVD_Pause(void) __vdec_weak;
__vdec_if void MDrv_MVD_Resume(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_StepDisp(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsStepDispDone(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_StepDecode(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsStepDecodeDone(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SeekToPTS(MS_U32 u32Pts) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsStep2PtsDone(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SkipToPTS(MS_U32 u32Pts) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_TrickPlay(MVD_TrickDec trickDec, MS_U8 u8DispDuration) __vdec_weak;
__vdec_if void MDrv_MVD_EnableForcePlay(void) __vdec_weak;

__vdec_if void MDrv_MVD_RegSetBase(MS_U32 u32RegBaseAddr) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_Init(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_Exit(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_Rst( void ) __vdec_weak;

__vdec_if void MDrv_MVD_Play( void ) __vdec_weak;
__vdec_if void MDrv_MVD_SetAVSync(MS_BOOL bEnable, MS_U32 u32Delay) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetAVSyncDelay(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsAVSyncOn(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsSyncRep(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsSyncSkip(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_ChangeAVsync(MS_BOOL bEnable, MS_U16 u16PTS) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_DispCtrl(MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_DispRepeatField(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetTop1stField(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetRepeat1stField(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetTmpRefField(void) __vdec_weak;

__vdec_if MS_U8 MDrv_MVD_GetActiveFormat( void ) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetDispRdy( void ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_Is1stFrmRdy(void) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetGOPCount( void ) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetPicCounter(void) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetParserByteCnt(void) __vdec_weak;
__vdec_if MVD_DecStat MDrv_MVD_GetDecodeStatus(void) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetLastCmd(void) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetVldErrCount( void ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_DropErrorFrame(MS_BOOL bDrop) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_MVDCommand ( MS_U8 u8cmd, MVD_CmdArg *pstCmdArg ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SkipData(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SkipToIFrame(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetCaps(MVD_Caps* pCaps) __vdec_weak;

__vdec_if E_MVD_Result MDrv_MVD_DisableErrConceal(MS_BOOL bDisable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_PushQueue(MVD_PacketInfo* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_FlushQueue(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FlushDisplayBuf(void) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetQueueVacancy(MS_BOOL bCached) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetESReadPtr(void) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetESWritePtr(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_EnableLastFrameShow(MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_IsDispFinish(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetSpeed(MVD_SpeedType eSpeedType, MS_U8 u8Multiple) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_ResetPTS(MS_U32 u32PtsBase) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetPTS(void) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetNextPTS(void) __vdec_weak;
__vdec_if MVD_TrickDec MDrv_MVD_GetTrickMode(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsPlaying(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsIdle(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsSeqChg(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_DbgSetData(MS_U32 u32Addr, MS_U32 u32Data) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_DbgGetData(MS_U32 u32Addr, MS_U32* u32Data) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetDecodedFrameIdx ( void ) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetFileModeAVSync(MVD_TIMESTAMP_TYPE eSyncMode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_IsAllBufferEmpty(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GenPattern(MVD_PatternType ePattern, MS_PHYADDR u32PAddr, MS_U32* pu32Size) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetPatternInfo(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetDynamicScaleAddr(MS_U32 u32addr) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_EnableQDMA(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetBlueScreen(MS_BOOL bEn) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_EnableDispOneField(MS_BOOL bEn) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetFrmInfo(MVD_FrmInfoType eType, MVD_FrmInfo* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetFreezeDisp(MS_BOOL bEn) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetFdMaskDelayCount(MS_U32 u32Cnt) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetOutputFRCMode(MS_U8 u8FrameRate, MS_U8 u8Interlace) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetFRCDropType(MS_U8 u8DropType) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIs32PullDown(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsDynScalingEnabled(void) __vdec_weak;
__vdec_if MS_S32 MDrv_MVD_GetPtsStcDiff(void) __vdec_weak;
__vdec_if void MDrv_MVD_SetAVSyncThreshold(MS_U32 u32Th) __vdec_weak;
__vdec_if void MDrv_MVD_SetAVSyncFreerunThreshold(MS_U32 u32Th) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetIsFreerun(void) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetColorFormat(void) __vdec_weak;
__vdec_if MS_U8 MDrv_MVD_GetMatrixCoef(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_PVRFlushDispQueue(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FlushPTSBuf(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDynScalingParam(MS_PHYADDR u32StAddr, MS_U32 u32Size) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetExtDispInfo(MVD_ExtDispInfo* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_GetTimeCode(MVD_FrmInfoType eType, MVD_TimeCode* pInfo) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDisableSeqChange(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_SetMStreamerMode(MS_U8 u8Mode) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FrameFlip(MS_U8 u8FrmIdx) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FrameRelease(MS_U8 u8FrmIdx) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_ParserRstDone(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_FrameCapture(MS_U8 u8FrmIdx, MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetFrmRateIsSupported(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetAutoMute(MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetVSizeAlign(MS_BOOL bEnable) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetDispFinishMode(MS_U8 u8Mode) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetAVSyncMode(MS_U8 u8Mode) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetIdctMode(MS_U8 u8Mode) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetDivxVer(void) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_SetMVDClockSpeed(MVD_ClockSpeed eClockSpeed) __vdec_weak;


//ATSC Closed Caption control
__vdec_if E_MVD_Result MDrv_MVD_CCRst(MVD_CCCfg* pCCParam) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_CCStartParsing(MVD_CCCfg* pCCParam) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_CCStopParsing(MVD_CCFormat eCCFormat) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_CCGetWritePtr(MVD_CCFormat eCCFormat, MS_U32* pWrite) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_CCGetReadPtr(MVD_CCFormat eCCFormat, MS_U32* pRead) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_CCUpdateReadPtr(MVD_CCFormat eCCFormat, MS_U32 u32EachPacketSize) __vdec_weak;
__vdec_if E_MVD_Result MDrv_MVD_CCGetIsOverflow(MVD_CCFormat eCCFormat, MS_BOOL* pbOverflow) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetUsrDataIsAvailable(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_MVD_GetUsrDataInfo(MVD_UsrDataInfo* pUsrInfo) __vdec_weak;

__vdec_if void MDrv_MVD_DbgDump(void) __vdec_weak;

#ifdef MVD_ENABLE_ISR
__vdec_if E_MVD_Result MDrv_MVD_SetIsrEvent(MS_U32 eEvent, MVD_InterruptCb fnHandler) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetIsrEvent(void) __vdec_weak;
#else
#define MDrv_MVD_SetIsrEvent(x, y) (E_MVD_RET_OK)
#define MDrv_MVD_GetIsrEvent()     (MS_U32)0
#endif


//======================================================
// HVD
__vdec_if void MDrv_HVD_PowerCtrl( MS_BOOL bEnable ) __vdec_weak;
__vdec_if void MDrv_HVD_SetOSRegBase( MS_U32 u32RegBaseAddr ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_Init( HVD_MemCfg* mem , HVD_Init_Settings* InitParam ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_Rst(MS_BOOL bErrHandle) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_Play(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_Exit(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_Pause(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_Flush( MS_BOOL bShowLast) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_StepDisp( void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_StepDecode(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_PushQueue(HVD_Packet_Info* pInfo) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_PushQueue_Fire(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_DecodeIFrame(MS_PHYADDR u32SrcSt , MS_U32 u32SrcSize) __vdec_weak;

__vdec_if HVD_Result MDrv_HVD_SetDataEnd( MS_BOOL bEnd ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDispErrFrm(MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDispRepeatField(MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSkipDecMode(HVD_Skip_Decode eDecType) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDispSpeed(HVD_Drv_Disp_Speed eSpeed) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSyncActive( MS_BOOL bEnable) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDropMode( HVD_Drop_Disp eMode , MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_RstPTS(MS_U32 u32PTS) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetFrcMode(HVD_FrmRateConv_Mode eMode ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSyncTolerance(MS_U32 u32Arg ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSyncVideoDelay( MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSyncFreeRunTH(MS_U32 u32Arg ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSyncRepeatTH( MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetErrConceal(MS_BOOL u32Arg ) __vdec_weak;
__vdec_if void MDrv_HVD_SetDbgLevel( HVD_Uart_Level elevel ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SeekToPTS(MS_U32 u32PTS ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SkipToPTS(MS_U32 u32PTS ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetFreezeImg(MS_BOOL bEnable ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetBlueScreen(MS_BOOL bEnable ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDispOneField(MS_BOOL bEnable ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetISREvent(MS_U32 u32Event ,HVD_InterruptCb fnISRHandler ) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_SetEnableISR(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_SetForceISR(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_SetMVOPDone(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDynScalingParam(  void *pStAddr , MS_U32 u32Size ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetAutoRmLstZeroByte(MS_BOOL bOn) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetDispInfoTH(HVD_Disp_Info_Threshold* DispInfoTH ) __vdec_weak;
__vdec_if HVD_Result  MDrv_HVD_SetFastDisplay(MS_BOOL bFastDisplay) __vdec_weak;
__vdec_if HVD_Result  MDrv_HVD_SetIgnoreErrRef(MS_BOOL bIgnore) __vdec_weak;
__vdec_if HVD_Result  MDrv_HVD_ForceFollowDTVSpec(MS_BOOL bEnable) __vdec_weak;

__vdec_if MS_BOOL MDrv_HVD_IsISROccured(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsDispFinish(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsFrameShowed(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsStepDecodeDone(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_CheckDispInfoRdy(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsDispInfoChg(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsIdle(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsSyncStart(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsSyncReach(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsLowDelay(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsIFrmFound(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_Is1stFrmRdy(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_IsAllBufferEmpty(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_IsAlive(void) __vdec_weak;

__vdec_if MS_U32 MDrv_HVD_GetBBUVacancy(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_GetDispInfo( HVD_Disp_Info *pinfo ) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetPTS(void) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetDataErrCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetDecErrCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetESWritePtr(void) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetESReadPtr(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_GetCaps( HVD_Codec u32Type) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetErrCode(void) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetPlayMode(HVD_Get_Mode_Status eMode) __vdec_weak;
__vdec_if HVD_Get_Play_State MDrv_HVD_GetPlayState(void) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetDecodeCnt(void) __vdec_weak;
__vdec_if MS_U8 MDrv_HVD_GetActiveFormat(void) __vdec_weak;
__vdec_if const HVD_DrvInfo* MDrv_HVD_GetInfo( void ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_GetLibVer(const MSIF_Version **ppVersion) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_GetStatus( HVD_DrvStatus *pstatus) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_GetFrmInfo( HVD_Get_Frm_Info_Type eType  ,  HVD_Frame_Info *pInfo) __vdec_weak;
__vdec_if MS_BOOL MDrv_HVD_GetISRInfo( MS_U32* eType ) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_CalLumaSum( HVD_Get_Frm_Info_Type eType  ) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetUserData_Wptr( void ) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetUserData_Packet( MS_U32 u32Idx , MS_U32* u32Size ) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_GenPattern( HVD_PatternType eType ,  MS_U32 u32VAddr  ,  MS_U32* u32Size  ) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetPatternInfo(   HVD_Pattern_Info eType ) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetDynamicScalingInfo(   HVD_DynamicScaling_Info eType ) __vdec_weak;

__vdec_if MS_U32 MDrv_HVD_GetData(HVD_GData_Type eType) __vdec_weak;
__vdec_if MS_U32 MDrv_HVD_GetMem_Dbg(MS_U32 u32Addr) __vdec_weak;
__vdec_if void MDrv_HVD_DbgDumpStatus(HVD_DumpStatus eFlag) __vdec_weak;
__vdec_if void MDrv_HVD_SetMem_Dbg(MS_U32 u32Addr , MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetCmd_Dbg( MS_U32 u32Cmd , MS_U32 u32Arg) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetSettings_Pro( HVD_SSettings_Type eType , MS_U32 u32Arg) __vdec_weak;

__vdec_if MS_S64 MDrv_HVD_GetPtsStcDiff(void) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetBalanceBW(MS_U8 qp_cnt, MS_U8 db_cnt, MS_U8 upper) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetFdMaskDelayCnt(MS_U8 u8DelayCnt) __vdec_weak;
__vdec_if HVD_Result MDrv_HVD_SetOutputFRCMode(MS_U8 u8FrameRate, MS_U8 u8Interlace) __vdec_weak;
__vdec_if MS_U32 MDrv_MVD_GetDispCount(void) __vdec_weak;


//======================================================
// RVD
__vdec_if void MDrv_RVD_SetBBU_OffsetLength(MS_U32 u32Address,MS_U32 u32offset, MS_U32 u32length) __vdec_weak;
__vdec_if MS_S16 MDrv_RVD_GetQmemSwBbuVacancy(void) __vdec_weak;
__vdec_if void MDrv_RVD_GetFrameSize(MS_U16*  u16Width,MS_U16*  u16Height) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetBBUStartAddr(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetTimeStamp(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetDecodeCnt(void) __vdec_weak;
__vdec_if void MDrv_RVD_SetBBUDepth(MS_U32 u32Rptr) __vdec_weak;
__vdec_if void MDrv_RVD_SetBBUReadPtr(MS_U32 u32Rptr) __vdec_weak;
__vdec_if void MDrv_RVD_SetBBUWritePtr(MS_U32 u32Wptr) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetBBUReadPtr(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetBBUWritePtr(void) __vdec_weak;
__vdec_if MS_U16 MDrv_RVD_GetBBUDepth(void) __vdec_weak;
__vdec_if void MDrv_RVD_SetPictureSize(MS_U16 u16Width, MS_U16 u16Height) __vdec_weak;
__vdec_if void MDrv_RVD_SetDisplay(RVD_DisplayMode u32PlayMode) __vdec_weak;
__vdec_if void MDrv_RVD_PushBBU (MS_U32 u32offset, MS_U32 u32length ,RVD_Packet_ID *rvdPackerInfo) __vdec_weak;
__vdec_if void MDrv_RVD_SetFileInfo(RVD_FileInfo *pFileInfo) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_Init(RVD_FileInfo *pFileInfo,RVD_MEMCfg* memCfg) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_EnableDynamicScaling(MS_BOOL bEnable) __vdec_weak;
__vdec_if void MDrv_RVD_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetDynScalingParam(MS_U32 u32StAddr, MS_U32 u32Size) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_FlushQueue(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetESReadPtr(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_ReadSVDProgramCounter(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_ChkCmdRdy(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetCmd(RVD_User_Cmd u32Command,MS_U32 u32Argument) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetSpeed(RVD_Disp_Speed u32DispSpeed) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsDbgEnable(MS_U8 u8DbgLevel) __vdec_weak;
__vdec_if MS_U8 MDrv_RVD_GetDbgLevel(void) __vdec_weak;
__vdec_if void MDrv_RVD_SetDbgLevel(MS_U8 u8DbgLevel) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetCfg( RVD_MEMCfg* memCfg) __vdec_weak;
__vdec_if void MDrv_RVD_SetOSRegBase(MS_U32 u32RegBaseAddr ) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetFrameCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetESWritePtr(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_CheckDispInfoRdy(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_CheckCaps(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsTSPlaying(void) __vdec_weak;
__vdec_if void MDrv_RVD_SetTrickMode(RVD_TrickMode u32TrickMode) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_Play(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_AVSyncOn(MS_BOOL  bEnable,MS_U16 u16SyncDelay, MS_U16 u16SyncTolerance) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsAVSyncOn(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_Pause(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_Exit(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_Resume(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_EnableLastFrameShowed(MS_BOOL bEnabled) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_StepPlay(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsStepPlayDone(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsIdle(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsDispFinish(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsIFrameFound(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetErrCode(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_DbgGetData(MS_U32 u32Addr , MS_U32 *u32Data) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_GetLibVer(const MSIF_Version **ppVersion) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_DispSetupDone(void) __vdec_weak;
__vdec_if RVD_Result  MDrv_RVD_GetDispInfo(RVD_FrameInfo *pinfo) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_GetStatus( RVD_DrvStatus *pstatus) __vdec_weak;
__vdec_if void MDrv_RVD_SetBBU_ID(MS_U32 u32Address,MS_U32 u32Byte_Pos,MS_U32 u32ID_High, MS_U32 u32ID_Low) __vdec_weak;
const RVD_DrvInfo* MDrv_RVD_GetInfo( void ) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetIDReadPtr(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetIDWritePtr(void) __vdec_weak;
__vdec_if void MDrv_RVD_SetIDWritePtr(MS_U32 u32Wptr) __vdec_weak;
__vdec_if void MDrv_RVD_SetIDReadPtr(MS_U32 u32Wptr) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_GetFrameInfo(RVD_FrameBuff_Info *framebuffinfo ,MS_BOOL bGETDISP) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_SetForceISR(MS_BOOL bEnable) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_GetISRInfo( RVD_ISR_Event* eType ) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_SetEnableISR(MS_BOOL bEnable) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetISREvent(RVD_ISR_Event eEvent ,RVD_InterruptCb fnISRHandler ) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetDispRepeatCnt(MS_U32 u32Num) __vdec_weak;
//__vdec_if void MDrv_RVD_ResetSTCBase(MS_U32 u32time_ms) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_JumpToPTS(MS_U32 u32time_ms) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SkipToPTS(MS_U32 u32time_ms) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetTrickMode(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_IsStepDecodeDone(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_StepDecode(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_Rst(void ) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetFreezeImg(MS_BOOL bEnabled) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetBlueScreen(MS_BOOL bEnabled) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_FireDecCmd(void) __vdec_weak;
__vdec_if MS_BOOL MDrv_RVD_Is1stFrameRdy(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetDecErrCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetDataErrCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetSkipCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetDropCnt(void) __vdec_weak;
__vdec_if MS_U32 MDrv_RVD_GetDispQueNum(void) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_DbgSetCmd(RVD_User_Cmd u32Command,MS_U32 u32Argument) __vdec_weak;
__vdec_if void MDrv_RVD_DbgSetData(MS_U32 u32Addr,MS_U32 data) __vdec_weak;
__vdec_if void MDrv_RVD_Dump_Status(void) __vdec_weak;
__vdec_if void MDrv_RVD_EnableTurboFWMode(MS_BOOL bEnable) __vdec_weak;
__vdec_if RVD_Result MDrv_RVD_SetAVSyncFreerunThreshold(MS_U32 u32Th) __vdec_weak;


#endif // _API_VDEC_WEAK_H_
