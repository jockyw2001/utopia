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
// comply with all applicable laws and regulations
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
//    or otherwise
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

#ifndef _API_VDEC_V2_PRIV_H_
#define _API_VDEC_V2_PRIV_H_

#include "apiVDEC.h"

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif




////////////////////////////////////////////////////////////////////////////////
// Enum for VDEC layer
////////////////////////////////////////////////////////////////////////////////
/*
enum
{
    E_VDEC_POOL_ID_INTERNAL = 0,
    E_VDEC_POOL_ID_MAX,
} E_VDEC_POOL_ID;
*/
////////////////////////////////////////////////////////////////////////////////
// Structure for VDEC layer
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    MS_U8 u8DecoderInUse;
}VDEC_RESOURCE_PRIVATE;





////////////////////////////////////////////////////////////////////////////////
// Function pointer for VDEC layer
////////////////////////////////////////////////////////////////////////////////
typedef VDEC_Result      (*IOCTL_VDEC_GetLibVer)                    (const MSIF_Version **);    
typedef VDEC_Result      (*IOCTL_VDEC_GetInfo)                      (const VDEC_Info **);    
typedef VDEC_Result      (*IOCTL_VDEC_GetStatus)                    (VDEC_Status *);      
typedef VDEC_Result      (*IOCTL_VDEC_CheckCaps)                    (VDEC_CodecType);     
typedef VDEC_Result      (*IOCTL_VDEC_EnableTurboMode)              (MS_BOOL);    
typedef VDEC_Result      (*IOCTL_VDEC_Init)                         (VDEC_InitParam*);
typedef VDEC_Result      (*IOCTL_VDEC_Init_EX)                      (void *, MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_Rst)                          (void);
typedef VDEC_Result      (*IOCTL_VDEC_Exit)                         (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_CheckDispInfoRdy)             (void);
typedef VDEC_Result      (*IOCTL_VDEC_SetFrcMode)                   (VDEC_FrcMode);
typedef VDEC_Result      (*IOCTL_VDEC_SetDynScalingParams)          (MS_PHYADDR, MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_Play)                         (void);
typedef VDEC_Result      (*IOCTL_VDEC_Pause)                        (void);
typedef VDEC_Result      (*IOCTL_VDEC_Resume)                       (void);
typedef VDEC_Result      (*IOCTL_VDEC_StepDisp)                     (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsStepDispDone)               (void);
typedef VDEC_Result      (*IOCTL_VDEC_SeekToPTS)                    (MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_SkipToPTS)                    (MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_StepDecode)                   (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsStepDecodeDone)             (void);
typedef VDEC_Result      (*IOCTL_VDEC_SetTrickMode)                 (VDEC_TrickDec);
typedef VDEC_Result      (*IOCTL_VDEC_PushDecQ)                     (VDEC_DecCmd*);
typedef VDEC_Result      (*IOCTL_VDEC_FireDecCmd)                   (void);
typedef MS_U32           (*IOCTL_VDEC_GetDecQVacancy)               (void);
typedef MS_BOOL          (*IOCTL_VDEC_IsCCAvailable)                (void);
typedef VDEC_Result      (*IOCTL_VDEC_GetCCInfo)                    (void *, MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_Flush)                        (VDEC_FreezePicSelect);
typedef MS_U32           (*IOCTL_VDEC_GetESWritePtr)                (void);
typedef MS_U32           (*IOCTL_VDEC_GetESReadPtr)                 (void);
typedef VDEC_Result      (*IOCTL_VDEC_EnableLastFrameShow)          (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_IsDispFinish)                 (void);
typedef VDEC_Result      (*IOCTL_VDEC_SetSpeed)                     (VDEC_SpeedType, VDEC_DispSpeed);
typedef VDEC_Result      (*IOCTL_VDEC_IsFrameRdy)                   (void);
typedef VDEC_Result      (*IOCTL_VDEC_SetFreezeDisp)                (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_SetBlueScreen)                (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_ResetPTS)                     (MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_AVSyncOn)                     (MS_BOOL, MS_U32, MS_U16);
typedef VDEC_Result      (*IOCTL_VDEC_SetAVSyncFreerunThreshold)    (MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_IsAVSyncOn)                   (void);
typedef MS_U32           (*IOCTL_VDEC_GetPTS)                       (void);
typedef MS_U32           (*IOCTL_VDEC_GetNextPTS)                   (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsStartSync)                  (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsReachSync)                  (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsFreerun)                    (void);
typedef VDEC_Result      (*IOCTL_VDEC_MhegDecodeIFrame)             (MS_PHYADDR,MS_PHYADDR,MS_PHYADDR);
typedef VDEC_Result      (*IOCTL_VDEC_MhegIsIFrameDecoding)         (void);
typedef VDEC_Result      (*IOCTL_VDEC_MhegRstIFrameDec)             (void);
typedef MS_U32           (*IOCTL_VDEC_GetErrCode)                   (void);
typedef MS_U32           (*IOCTL_VDEC_GetErrCnt)                    (void);
typedef VDEC_CodecType   (*IOCTL_VDEC_GetActiveCodecType)           (void);
typedef MS_U32           (*IOCTL_VDEC_GetBitsRate)                  (void);
typedef MS_BOOL          (*IOCTL_VDEC_Is32PullDown)                 (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsAlive)                      (void);
typedef MS_S64           (*IOCTL_VDEC_GetVideoPtsStcDelta)          (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsWithValidStream)            (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsIFrameFound)                (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsWithLowDelay)               (void);
typedef VDEC_Result      (*IOCTL_VDEC_IsAllBufferEmpty)             (void);
typedef MS_U8            (*IOCTL_VDEC_GetGOPCnt)                    (void);
typedef MS_U32           (*IOCTL_VDEC_GetFrameCnt)                  (void);
typedef MS_U32           (*IOCTL_VDEC_GetSkipCnt)                   (void);
typedef MS_U32           (*IOCTL_VDEC_GetDropCnt)                   (void);
typedef VDEC_Result      (*IOCTL_VDEC_GetDispInfo)                  (VDEC_DispInfo *);
typedef VDEC_TrickDec    (*IOCTL_VDEC_GetTrickMode)                 (void);
typedef MS_U8            (*IOCTL_VDEC_GetActiveFormat)              (void);
typedef MS_U8            (*IOCTL_VDEC_GetColourPrimaries)           (void);
typedef VDEC_Result      (*IOCTL_VDEC_GetHWKey)                     (MS_U8 *);
typedef VDEC_Result      (*IOCTL_VDEC_IsSeqChg)                     (void);
typedef VDEC_Result      (*IOCTL_VDEC_SetDbgLevel)                  (VDEC_DbgLevel);
typedef VDEC_Result      (*IOCTL_VDEC_GetDecFrameInfo)              (VDEC_FrameInfo *);
typedef VDEC_Result      (*IOCTL_VDEC_GetDispFrameInfo)             (VDEC_FrameInfo *);
typedef VDEC_Result      (*IOCTL_VDEC_GetExtDispInfo)               (VDEC_ExtDispInfo *);
typedef VDEC_Result      (*IOCTL_VDEC_GetDecTimeCode)               (VDEC_TimeCode *);
typedef VDEC_Result      (*IOCTL_VDEC_GetDispTimeCode)              (VDEC_TimeCode *);
typedef VDEC_Result      (*IOCTL_VDEC_CC_StartParsing)              (VDEC_CCCfg* );
typedef VDEC_Result      (*IOCTL_VDEC_CC_StopParsing)               (VDEC_CCCfg* );
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetWritePtr)               (MS_U32* );
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetReadPtr)                (MS_U32* );
typedef VDEC_Result      (*IOCTL_VDEC_CC_UpdateReadPtr)             (MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetIsOverflow)             (MS_BOOL*);
typedef VDEC_Result      (*IOCTL_VDEC_SetEvent)                     (MS_U32, VDEC_EventCb, void *);
typedef VDEC_Result      (*IOCTL_VDEC_UnsetEvent)                   (MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_SetEvent_EX)                  (VDEC_CB_TYPE, MS_U32, VDEC_EventCb, void*);
typedef VDEC_Result      (*IOCTL_VDEC_UnsetEvent_EX)                (VDEC_CB_TYPE, MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_GetEventInfo)                 (MS_U32*);
typedef VDEC_Result      (*IOCTL_VDEC_GenPattern)                   (VDEC_PatternType,  MS_PHYADDR, MS_U32 *);
typedef VDEC_Result      (*IOCTL_VDEC_GetPatternLeastLength)        (VDEC_PatternType);
typedef VDEC_Result      (*IOCTL_VDEC_DisableDeblocking)            (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_DisableQuarterPixel)          (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_SetAutoRmLstZeroByte)         (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_SetBalanceBW)                 (MS_U8, MS_U8, MS_U8, MS_U8);
typedef VDEC_Result      (*IOCTL_VDEC_DbgDumpStatus)                (void);
typedef VDEC_Result      (*IOCTL_VDEC_SetControl)                   (VDEC_User_Cmd, MS_U32);
typedef VDEC_Result      (*IOCTL_VDEC_GetControl)                   (VDEC_User_Cmd, MS_U32);
typedef MS_U32           (*IOCTL_VDEC_GetDispCnt)                   (void);
typedef MS_U32           (*IOCTL_VDEC_GetFwVersion)                 (VDEC_FwType);
typedef VDEC_Result      (*IOCTL_VDEC_SetBlockDisplay)              (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_EnableESBuffMalloc)           (MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_GetESBuffVacancy)             (void*);
typedef VDEC_Result      (*IOCTL_VDEC_GetESBuff)                    (MS_U32, MS_U32*, MS_PHYADDR* );
typedef VDEC_Result      (*IOCTL_VDEC_GetNextDispFrame)             (VDEC_DispFrame** );
typedef VDEC_Result      (*IOCTL_VDEC_DisplayFrame)                 (VDEC_DispFrame* );
typedef VDEC_Result      (*IOCTL_VDEC_ReleaseFrame)                 (VDEC_DispFrame* );
typedef VDEC_Result      (*IOCTL_VDEC_FlushPTSBuf)                  (void);
typedef VDEC_Result      (*IOCTL_VDEC_CaptureFrame)                 (MS_U32, MS_BOOL);
typedef VDEC_Result      (*IOCTL_VDEC_CC_Init)                      (MS_U32*);
typedef VDEC_Result      (*IOCTL_VDEC_CC_SetCfg)                    (VDEC_CCFormat, VDEC_CCType, MS_U32*);
typedef VDEC_Result      (*IOCTL_VDEC_CC_SetBuffStartAdd)           (VDEC_CCFormat, MS_U32*);
typedef VDEC_Result      (*IOCTL_VDEC_CC_UpdateWriteAdd)            (VDEC_CCFormat, MS_U32*);
typedef VDEC_Result      (*IOCTL_VDEC_CC_UpdateReadAdd)             (VDEC_CCFormat, MS_U32*);
typedef VDEC_Result      (*IOCTL_VDEC_CC_DisableParsing)            (VDEC_CCFormat);
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetInfo)                   (VDEC_CCFormat, VDEC_CCInfoCmd, MS_U32 *);
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetIsRstDone)              (VDEC_CCFormat);
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetIsBuffOverflow)         (VDEC_CCFormat);
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetWriteAdd)               (VDEC_CCFormat);
typedef VDEC_Result      (*IOCTL_VDEC_CC_GetReadAdd)                (VDEC_CCFormat);
typedef VDEC_Result      (*IOCTL_VDEC_CC_SystemPreSetControl)       (VDEC_User_Cmd, void *);
typedef VDEC_Result      (*IOCTL_VDEC_CC_PreSetControl)             (VDEC_User_Cmd, MS_U32 **);

typedef struct
{
    // instance variable
    //MS_U8                                Index_of_decoder;
    // Function pointer
    //IOCTL_VDEC_GetFreeStream          fpVDEC_EX_GetFreeStream;
    
    IOCTL_VDEC_GetLibVer                    fpVDEC_GetLibVer;
    IOCTL_VDEC_GetInfo                      fpVDEC_GetInfo;
    IOCTL_VDEC_GetStatus                    fpVDEC_GetStatus;
    IOCTL_VDEC_CheckCaps                    fpVDEC_CheckCaps;
    IOCTL_VDEC_EnableTurboMode              fpVDEC_EnableTurboMode;
    IOCTL_VDEC_Init                         fpVDEC_Init;
    IOCTL_VDEC_Init_EX                      fpVDEC_Init_EX;
    IOCTL_VDEC_Rst                          fpVDEC_Rst;
    IOCTL_VDEC_Exit                         fpVDEC_Exit;
    IOCTL_VDEC_CheckDispInfoRdy             fpVDEC_CheckDispInfoRdy;
    IOCTL_VDEC_SetFrcMode                   fpVDEC_SetFrcMode;
    IOCTL_VDEC_SetDynScalingParams          fpVDEC_SetDynScalingParams;
    IOCTL_VDEC_Play                         fpVDEC_Play;
    IOCTL_VDEC_Pause                        fpVDEC_Pause;
    IOCTL_VDEC_Resume                       fpVDEC_Resume;
    IOCTL_VDEC_StepDisp                     fpVDEC_StepDisp;
    IOCTL_VDEC_IsStepDispDone               fpVDEC_IsStepDispDone;
    IOCTL_VDEC_SeekToPTS                    fpVDEC_SeekToPTS;
    IOCTL_VDEC_SkipToPTS                    fpVDEC_SkipToPTS;
    IOCTL_VDEC_StepDecode                   fpVDEC_StepDecode;
    IOCTL_VDEC_IsStepDecodeDone             fpVDEC_IsStepDecodeDone;
    IOCTL_VDEC_SetTrickMode                 fpVDEC_SetTrickMode;
    IOCTL_VDEC_PushDecQ                     fpVDEC_PushDecQ;
    IOCTL_VDEC_FireDecCmd                   fpVDEC_FireDecCmd;
    IOCTL_VDEC_GetDecQVacancy               fpVDEC_GetDecQVacancy;
    IOCTL_VDEC_IsCCAvailable                fpVDEC_IsCCAvailable;
    IOCTL_VDEC_GetCCInfo                    fpVDEC_GetCCInfo;
    IOCTL_VDEC_Flush                        fpVDEC_Flush;
    IOCTL_VDEC_GetESWritePtr                fpVDEC_GetESWritePtr;
    IOCTL_VDEC_GetESReadPtr                 fpVDEC_GetESReadPtr;
    IOCTL_VDEC_EnableLastFrameShow          fpVDEC_EnableLastFrameShow;
    IOCTL_VDEC_IsDispFinish                 fpVDEC_IsDispFinish;
    IOCTL_VDEC_SetSpeed                     fpVDEC_SetSpeed;
    IOCTL_VDEC_IsFrameRdy                   fpVDEC_IsFrameRdy;
    IOCTL_VDEC_SetFreezeDisp                fpVDEC_SetFreezeDisp;
    IOCTL_VDEC_SetBlueScreen                fpVDEC_SetBlueScreen;
    IOCTL_VDEC_ResetPTS                     fpVDEC_ResetPTS;
    IOCTL_VDEC_AVSyncOn                     fpVDEC_AVSyncOn;
    IOCTL_VDEC_SetAVSyncFreerunThreshold    fpVDEC_SetAVSyncFreerunThreshold;
    IOCTL_VDEC_IsAVSyncOn                   fpVDEC_IsAVSyncOn;
    IOCTL_VDEC_GetPTS                       fpVDEC_GetPTS;
    IOCTL_VDEC_GetNextPTS                   fpVDEC_GetNextPTS;
    IOCTL_VDEC_IsStartSync                  fpVDEC_IsStartSync;
    IOCTL_VDEC_IsReachSync                  fpVDEC_IsReachSync;
    IOCTL_VDEC_IsFreerun                    fpVDEC_IsFreerun;
    IOCTL_VDEC_MhegDecodeIFrame             fpVDEC_MhegDecodeIFrame;
    IOCTL_VDEC_MhegIsIFrameDecoding         fpVDEC_MhegIsIFrameDecoding;
    IOCTL_VDEC_MhegRstIFrameDec             fpVDEC_MhegRstIFrameDec;
    IOCTL_VDEC_GetErrCode                   fpVDEC_GetErrCode;
    IOCTL_VDEC_GetErrCnt                    fpVDEC_GetErrCnt;
    IOCTL_VDEC_GetActiveCodecType           fpVDEC_GetActiveCodecType;
    IOCTL_VDEC_GetBitsRate                  fpVDEC_GetBitsRate;
    IOCTL_VDEC_Is32PullDown                 fpVDEC_Is32PullDown;
    IOCTL_VDEC_IsAlive                      fpVDEC_IsAlive;
    IOCTL_VDEC_GetVideoPtsStcDelta          fpVDEC_GetVideoPtsStcDelta;
    IOCTL_VDEC_IsWithValidStream            fpVDEC_IsWithValidStream;
    IOCTL_VDEC_IsIFrameFound                fpVDEC_IsIFrameFound;
    IOCTL_VDEC_IsWithLowDelay               fpVDEC_IsWithLowDelay;
    IOCTL_VDEC_IsAllBufferEmpty             fpVDEC_IsAllBufferEmpty;
    IOCTL_VDEC_GetGOPCnt                    fpVDEC_GetGOPCnt;
    IOCTL_VDEC_GetFrameCnt                  fpVDEC_GetFrameCnt;
    IOCTL_VDEC_GetSkipCnt                   fpVDEC_GetSkipCnt;
    IOCTL_VDEC_GetDropCnt                   fpVDEC_GetDropCnt;
    IOCTL_VDEC_GetDispInfo                  fpVDEC_GetDispInfo;
    IOCTL_VDEC_GetTrickMode                 fpVDEC_GetTrickMode;
    IOCTL_VDEC_GetActiveFormat              fpVDEC_GetActiveFormat;
    IOCTL_VDEC_GetColourPrimaries           fpVDEC_GetColourPrimaries;
    IOCTL_VDEC_GetHWKey                     fpVDEC_GetHWKey;
    IOCTL_VDEC_IsSeqChg                     fpVDEC_IsSeqChg;
    IOCTL_VDEC_SetDbgLevel                  fpVDEC_SetDbgLevel;
    IOCTL_VDEC_GetDecFrameInfo              fpVDEC_GetDecFrameInfo;
    IOCTL_VDEC_GetDispFrameInfo             fpVDEC_GetDispFrameInfo;
    IOCTL_VDEC_GetExtDispInfo               fpVDEC_GetExtDispInfo;
    IOCTL_VDEC_GetDecTimeCode               fpVDEC_GetDecTimeCode;
    IOCTL_VDEC_GetDispTimeCode              fpVDEC_GetDispTimeCode;
    IOCTL_VDEC_CC_StartParsing              fpVDEC_CC_StartParsing;
    IOCTL_VDEC_CC_StopParsing               fpVDEC_CC_StopParsing;
    IOCTL_VDEC_CC_GetWritePtr               fpVDEC_CC_GetWritePtr;
    IOCTL_VDEC_CC_GetReadPtr                fpVDEC_CC_GetReadPtr;
    IOCTL_VDEC_CC_UpdateReadPtr             fpVDEC_CC_UpdateReadPtr;
    IOCTL_VDEC_CC_GetIsOverflow             fpVDEC_CC_GetIsOverflow;
    IOCTL_VDEC_SetEvent                     fpVDEC_SetEvent;
    IOCTL_VDEC_UnsetEvent                   fpVDEC_UnsetEvent;
    IOCTL_VDEC_SetEvent_EX                  fpVDEC_SetEvent_EX;
    IOCTL_VDEC_UnsetEvent_EX                fpVDEC_UnsetEvent_EX;
    IOCTL_VDEC_GetEventInfo                 fpVDEC_GetEventInfo;
    IOCTL_VDEC_GenPattern                   fpVDEC_GenPattern;
    IOCTL_VDEC_GetPatternLeastLength        fpVDEC_GetPatternLeastLength;
    IOCTL_VDEC_DisableDeblocking            fpVDEC_DisableDeblocking;
    IOCTL_VDEC_DisableQuarterPixel          fpVDEC_DisableQuarterPixel;
    IOCTL_VDEC_SetAutoRmLstZeroByte         fpVDEC_SetAutoRmLstZeroByte;
    IOCTL_VDEC_SetBalanceBW                 fpVDEC_SetBalanceBW;
    IOCTL_VDEC_DbgDumpStatus                fpVDEC_DbgDumpStatus;
    IOCTL_VDEC_SetControl                   fpVDEC_SetControl;
    IOCTL_VDEC_GetControl                   fpVDEC_GetControl;
    IOCTL_VDEC_GetDispCnt                   fpVDEC_GetDispCnt;
    IOCTL_VDEC_GetFwVersion                 fpVDEC_GetFwVersion;
    IOCTL_VDEC_SetBlockDisplay              fpVDEC_SetBlockDisplay;
    IOCTL_VDEC_EnableESBuffMalloc           fpVDEC_EnableESBuffMalloc;
    IOCTL_VDEC_GetESBuffVacancy             fpVDEC_GetESBuffVacancy;
    IOCTL_VDEC_GetESBuff                    fpVDEC_GetESBuff;
    IOCTL_VDEC_GetNextDispFrame             fpVDEC_GetNextDispFrame;
    IOCTL_VDEC_DisplayFrame                 fpVDEC_DisplayFrame;
    IOCTL_VDEC_ReleaseFrame                 fpVDEC_ReleaseFrame;
    IOCTL_VDEC_FlushPTSBuf                  fpVDEC_FlushPTSBuf;
    IOCTL_VDEC_CaptureFrame                 fpVDEC_CaptureFrame;
    IOCTL_VDEC_CC_Init                      fpVDEC_CC_Init;
    IOCTL_VDEC_CC_SetCfg                    fpVDEC_CC_SetCfg;
    IOCTL_VDEC_CC_SetBuffStartAdd           fpVDEC_CC_SetBuffStartAdd;
    IOCTL_VDEC_CC_UpdateWriteAdd            fpVDEC_CC_UpdateWriteAdd;
    IOCTL_VDEC_CC_UpdateReadAdd             fpVDEC_CC_UpdateReadAdd;
    IOCTL_VDEC_CC_DisableParsing            fpVDEC_CC_DisableParsing;
    IOCTL_VDEC_CC_GetInfo                   fpVDEC_CC_GetInfo;
    IOCTL_VDEC_CC_GetIsRstDone              fpVDEC_CC_GetIsRstDone;
    IOCTL_VDEC_CC_GetIsBuffOverflow         fpVDEC_CC_GetIsBuffOverflow;
    IOCTL_VDEC_CC_GetWriteAdd               fpVDEC_CC_GetWriteAdd;
    IOCTL_VDEC_CC_GetReadAdd                fpVDEC_CC_GetReadAdd;
    IOCTL_VDEC_CC_SystemPreSetControl       fpVDEC_SystemPreSetControl;
    IOCTL_VDEC_CC_PreSetControl             fpVDEC_PreSetControl;
}VDEC_INSTANCE_PRIVATE;

#ifdef __cplusplus
}
#endif

#undef _API_VDEC_V2_PRIV_H_
#endif // _API_VDEC_V2_PRIV_H_
