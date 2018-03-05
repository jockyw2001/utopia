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

#ifndef _DEV_MVD_H_
#define _DEV_MVD_H_
#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

#include "halVPU_EX.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef __MDRV_TYPES_H__
#define BIT0    BIT(0)
#define BIT1    BIT(1)
#define BIT2    BIT(2)
#define BIT3    BIT(3)
#define BIT4    BIT(4)
#define BIT5    BIT(5)
#define BIT6    BIT(6)
#define BIT7    BIT(7)
#endif

#define H_DWORD(x)            (MS_U8)(((x)>>24)&0xff)
#define L_DWORD(x)            (MS_U8)(((x)>>16)&0xff)
#define H_WORD(x)             (MS_U8)(((x)>>8 )&0xff)
#define L_WORD(x)             (MS_U8)((x)&0xff)
#define COMBM16(hi,lo)        ((((MS_U16)(hi))<<8) | ((MS_U16)(lo)))
#define COMBU32(hh,hl,lh,ll)  ((((MS_U32)(hh))<<24) | (((MS_U32)(hl))<<16) | (((MS_U32)(lh))<<8) | ((MS_U32)(ll)))
#ifndef MemAlign
#define MemAlign(n, unit)     ( (((n)+(unit)-1)/(unit)) * (unit) )
#endif

//Specify which decoder is used
#define SET_DECNUM(cmd, u8Num)              \
            do {                            \
                cmd.Arg5 = u8Num;           \
               } while(0)

void HAL_MVD_RegSetBase(MS_VIRT u32Base);
MS_U8 HAL_MVD_RegReadByte(MS_VIRT u32Reg);
void HAL_MVD_RegWriteByte(MS_VIRT u32Reg, MS_U8 u8Val);
void HAL_MVD_RegWriteBit(MS_VIRT u32Reg, MS_BOOL bEnable, MS_U8 u8Mask);
void HAL_MVD_RegWriteByteMask(MS_VIRT u32Reg, MS_U8 u8Val, MS_U8 u8Msk);
void HAL_MVD_RegWrite4Byte(MS_VIRT u32Reg, MS_U32 u32Val);

// Mutex function for HI interface
#define HAL_MVD_LockHiIfMutex()
#define HAL_MVD_UnlockHiIfMutex()

#define MVD_SUPPORT_MPEG2 0x01
#define MVD_SUPPORT_MPEG4 0x02
#define MVD_SUPPORT_VC1   0x04

#define MVD_FW_VERSION   FW_VERSION

#define MVD_TURBO_INIT        FALSE//TRUE

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_MVD_MMAP_FW  = 0, //firmware
    E_MVD_MMAP_BS  = 1, //bitstream buffer
    E_MVD_MMAP_FB  = 2, //framebuffer
    E_MVD_MMAP_ALL = 3,
    E_MVD_MMAP_DRV = 4  //driver processing buffer
} MVD_MMAP_Type;


typedef struct _MVD_DrvCfg
{
    MS_U32 u32fmVerNum;//firmware version number
    MS_U16 u16fmSrcID; //firmware bin source id
    MS_U8  u8fbMode;   //framebuffer mode: HD or SD
} MVD_DrvCfg;

typedef enum
{
    E_HAL_MVD_MAIN_STREAM,  //Support TSP/TS/File mode
    E_HAL_MVD_SUB_STREAM,   //Only support file mode
#ifdef VDEC3
    E_HAL_MVD_N_STREAM,
#endif
} HAL_MVD_StreamType;

typedef enum
{
    E_HAL_MVD_STREAM_NONE = 0x0,

    //Support TSP/TS/File mode
    E_HAL_MVD_MAIN_STREAM_BASE = 0x10,
    E_HAL_MVD_MAIN_STREAM0 = E_HAL_MVD_MAIN_STREAM_BASE,
    E_HAL_MVD_MAIN_STREAM_MAX,

    //Only support file mode
    E_HAL_MVD_SUB_STREAM_BASE = 0x20,
    E_HAL_MVD_SUB_STREAM0 = E_HAL_MVD_SUB_STREAM_BASE,
    //E_HAL_MVD_SUB_STREAM1,
    E_HAL_MVD_SUB_STREAM_MAX,

#ifdef VDEC3
    E_HAL_MVD_N_STREAM_BASE = 0x40,
    E_HAL_MVD_N_STREAM0 = E_HAL_MVD_N_STREAM_BASE,
    E_HAL_MVD_N_STREAM_MAX = E_HAL_MVD_N_STREAM0 + VPU_MAX_DEC_NUM,
#endif
} HAL_MVD_StreamId;

/// MVD clock speed
typedef enum
{
    E_HAL_MVD_EX_CLOCK_SPEED_NONE = 0,
    E_HAL_MVD_EX_CLOCK_SPEED_HIGHEST,
    E_HAL_MVD_EX_CLOCK_SPEED_HIGH,
    E_HAL_MVD_EX_CLOCK_SPEED_MEDIUM,
    E_HAL_MVD_EX_CLOCK_SPEED_LOW,
    E_HAL_MVD_EX_CLOCK_SPEED_LOWEST,
    E_HAL_MVD_EX_CLOCK_SPEED_DEFAULT,
} HAL_MVD_EX_ClockSpeed;

typedef struct
{
    HAL_MVD_StreamId eStreamId;
    MS_BOOL bUsed;
    #ifdef VDEC3
    MS_U32 u32SLQId;
    #endif
} HAL_MVD_Stream;

typedef struct
{
    MS_BOOL bHWBufferReMapping;
    MS_U32 bHWReMappingPaddingAddr;
} HAL_MVD_Pre_Ctrl;

typedef enum
{
    MVD_DISP_QUEUE_NOT_EMPTY = 0,
    MVD_DISP_QUEUE_EMPTY = 1,
    MVD_DISP_QUEUE_INVALID_ADDR = 2,
} E_MVD_DISP_QUEUE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
HAL_MVD_StreamId HAL_MVD_GetFreeStream(HAL_MVD_StreamType eStreamType);
MS_U8 HAL_MVD_GetCaps(void);
MS_U64 HAL_MVD_GetMaxPixel(MS_U32 u32Id);
MS_BOOL HAL_MVD_GetFrmRateIsSupported(MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate);
MS_U32 HAL_MVD_GetDrvFwVer(void);
void HAL_MVD_MemGetMap(MS_U8 u8Idx, MS_U8 u8type, MS_VIRT* pu32addr, MS_SIZE* pu32len);
MS_U32 HAL_MVD_MemRead4Byte(MS_VIRT u32Address);
MS_U16 HAL_MVD_MemRead2Byte(MS_VIRT u32Address);
MS_U8 HAL_MVD_MemReadByte(MS_VIRT u32Address);
MS_BOOL HAL_MVD_MemWrite4Byte(MS_VIRT u32Address, MS_U32 u32Value);
MS_BOOL HAL_MVD_MemWrite2Byte(MS_VIRT u32Address, MS_U16 u16Value);
MS_BOOL HAL_MVD_MemWriteByte(MS_VIRT u32Address, MS_U8 u8Value);
void HAL_MVD_SetReqMask(MS_U32 u32Id, MS_BOOL bEnMask);
MS_VIRT HAL_MVD_GetMiu1BaseAdd(void);
MS_U8 HAL_MVD_GetFWSelMiu(void);
MS_BOOL HAL_MVD_GetFWSelMiu1(void);

MS_VIRT  HAL_MVD_PA2NonCacheSeg(MS_PHY u32PhyAddr);
MS_U32      HAL_MVD_GetTime(void);
void        HAL_MVD_Delayms(MS_U32 u32msecs);
void        HAL_MVD_CPU_Sync(void);
void        HAL_MVD_FlushMemory(void);
void        HAL_MVD_ReadMemory(void);
MS_PHY      HAL_MVD_GetMemOffset(MS_PHY u32PhyAdd);

MS_BOOL HAL_MVD_Init(MS_U8 u8HalIdx,MVD_CodecType eCodecType, MS_U32 u32VpuSid);
MS_BOOL HAL_MVD_RstHW(MS_U32 u32Id);
MS_BOOL HAL_MVD_ReleaseFW(MS_U32 u32Id);
MS_BOOL HAL_MVD_LoadCode(MS_U8 u8Idx);

void HAL_MVD_PowerCtrl(MS_BOOL bOn);
MS_BOOL HAL_MVD_TimeOut(MS_U8 u8Idx);
MS_BOOL HAL_MVD_MVDCommand(MS_U8 u8cmd, MVD_CmdArg *pstCmdArg);
MS_BOOL HAL_MVD_InitHW(MS_U32 u32Id, VPU_EX_SourceType SourceType, VPU_EX_DecoderType eDecType);
MS_BOOL HAL_MVD_InitFW(MS_U32 u32VpuSid);
MS_U32 HAL_MVD_GetFWVer(MS_U32 u32VpuSid);
MS_BOOL HAL_MVD_SoftRstHW(void);
void HAL_MVD_ClearIRQ(void);
MS_BOOL HAL_MVD_CreateTask(MS_U8 u8Idx, HAL_VPU_StreamId eVpuId);
MS_BOOL HAL_MVD_DeleteTask(MS_U8 u8Idx, HAL_VPU_StreamId eVpuId);

MS_BOOL HAL_MVD_SetSpeed(MS_U8 u8Idx, MVD_SpeedType eSpeedType, MS_U8 u8Multiple);
void HAL_MVD_SetFrameBuffAddr(MS_U8 u8Idx, MS_VIRT u32addr, MS_U8 u8fbMode);
MS_BOOL HAL_MVD_StepDisp(MS_U8 u8Idx);
MS_BOOL HAL_MVD_EnableLastFrameShow(MS_U8 u8Idx, MS_BOOL bEnable);
MS_BOOL HAL_MVD_SeekToPTS(MS_U8 u8Idx, MS_U32 u32Pts);
MS_BOOL HAL_MVD_SkipToPTS(MS_U8 u8Idx, MS_U32 u32Pts);
MS_BOOL HAL_MVD_SetFileModeAVSync(MS_U8 u8Idx, MVD_TIMESTAMP_TYPE eSyncMode);
MS_BOOL HAL_MVD_SetVirtualBox(MS_U8 u8Idx, MS_U16 u16Width, MS_U16 u16Height);
MS_BOOL HAL_MVD_SetBlueScreen(MS_U8 u8Idx, MS_BOOL bEn);
MS_BOOL HAL_MVD_Resume(MS_U8 u8Idx);
MS_BOOL HAL_MVD_Play(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetFreezeDisp(MS_U8 u8Idx, MS_BOOL bEn);

MS_BOOL HAL_MVD_GetHasInt(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetIntState(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetAVSyncStatus(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetMStreamerMode(MS_U8 u8Idx, MS_U8 u8Mode);
MS_BOOL HAL_MVD_IsMStreamerMode(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetMcuMode(MS_U8 u8Idx, MS_U8 u8Mode);
MS_BOOL HAL_MVD_IsMcuMode(MS_U8 u8Idx);
MS_BOOL HAL_MVD_FrameOpt(MS_U8 u8Idx, MS_U8 u8FrmIdx, MVD_FrmOpt eOpt);
MS_BOOL HAL_MVD_UpdatePts(MS_U8 u8Idx);
MS_BOOL HAL_MVD_FrameCapture(MS_U8 u8Idx, MS_U8 u8FrmIdx, MS_BOOL bEnable);

MS_VIRT HAL_MVD_SetHWBuffer(MS_U8 u8Idx, MS_VIRT u32Add);
void HAL_MVD_SetFrameBuffNum(MS_U8 u8Idx, MS_U8 u8FrmNum,MS_U32 u32FBUsedSize);

MS_U32 HAL_MVD_GetBitsRate(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetVideoRange(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetLowDelayFlag(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetIs32PullDown(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetIsDynScalingEnabled(MS_U8 u8Idx);
MS_BOOL HAL_MVD_Is1stFrmRdy(MS_U8 u8Idx);

MS_U32 HAL_MVD_GetPicCounter(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetSkipPicCounter(MS_U8 u8Idx);
MVD_PicType HAL_MVD_GetPicType(MS_U8 u8Idx);
MS_S32 HAL_MVD_GetPtsStcDiff(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetDecodedFrameIdx(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetVldErrCount(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetValidStreamFlag(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetIsIPicFound(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetNextPTS(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetChromaFormat(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetGOPCount(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetColorFormat(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetMatrixCoef(MS_U8 u8Idx);
MS_BOOL HAL_MVD_IsStepDispDone(MS_U8 u8Idx);
MS_BOOL HAL_MVD_IsStep2PtsDone(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetPayloadLen(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GotFileEndPattern(MS_U8 u8Idx);
MS_BOOL HAL_MVD_IsCmdFinished(MS_U8 u8Idx, MVD_HANDSHAKE_CMD eCmd);
MS_BOOL HAL_MVD_ClearCmdFinished(MS_U8 u8Idx, MVD_HANDSHAKE_CMD eCmd);
MS_BOOL HAL_MVD_GetTimeCode(MS_U8 u8Idx, MVD_FrmInfoType eType, MVD_TimeCode* pInfo);
MS_U32 HAL_MVD_GetDispCnt(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetXcLowDelayIntState(MS_U8 u8Idx);
void HAL_MVD_GetErrInfo(MS_U8 u8Idx, MVD_ErrCode *errCode, MVD_ErrStatus *errStatus);

MS_BOOL HAL_MVD_SetInternalBuffAddr(MS_U8 u8Idx, MS_VIRT u32start, MS_U32 u32len);
void HAL_MVD_InitVar(MS_U8 u8Idx);
MS_BOOL HAL_MVD_CheckFrmBuffSize(MS_U8 u8Idx, MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg);
MS_BOOL HAL_MVD_SetCodecInfo(MS_U8 u8Idx, MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing, MS_U8 u8ChipECORev);
MS_BOOL HAL_MVD_Exit(MS_U8 u8Idx);

void HAL_MVD_GetFrameInfo(MS_U8 u8Idx, MVD_FrameInfo *pinfo);
void HAL_MVD_SetFrameInfo(MS_U8 u8Idx, MVD_FrameInfo *pinfo );
MS_VIRT MVD_GetSlqTblESReadPtr(MS_U8 u8Idx);
MS_VIRT MVD_GetSlqTblESWritePtr(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetQueueVacancy(MS_U8 u8Idx, MS_BOOL bCached);
E_MVD_Result HAL_MVD_PushQueue(MS_U8 u8Idx, MVD_PacketInfo* pInfo);
E_MVD_Result HAL_MVD_FlushQueue(MS_U8 u8Idx);
MS_BOOL HAL_MVD_IsAllBufferEmpty(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SLQTblInit(MS_U8 u8Idx);

E_MVD_Result HAL_MVD_IsDispFinish(MS_U8 u8Idx);
E_MVD_Result HAL_MVD_SetDynScalingParam(MS_U8 u8Idx, MS_PHY u32StAddr, MS_SIZE u32Size);
void HAL_MVD_GetExtDispInfo(MS_U8 u8Idx, MVD_ExtDispInfo* pInfo);
E_MVD_Result HAL_MVD_GetFrmInfo(MS_U8 u8Idx, MVD_FrmInfoType eType, MVD_FrmInfo* pInfo);

MS_BOOL HAL_MVD_GetUsrDataIsAvailable(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetUsrDataInfo(MS_U8 u8Idx, MVD_UsrDataInfo* pUsrInfo);

MS_BOOL HAL_MVD_DecodeIFrame(MS_U8 u8Idx, MS_PHY u32FrameBufAddr, MS_PHY u32StreamBufAddr, MS_PHY u32StreamBufEndAddr, MS_U8 u8ChipECORev);

MS_U8 HAL_MVD_GetActiveFormat(MS_U8 u8Idx);
MS_BOOL HAL_MVD_EnableAVSync(MS_U8 u8Idx, MS_BOOL bEnable);
MS_BOOL HAL_MVD_SetAVSyncDelay(MS_U8 u8Idx, MS_U32 u32Delay);
MS_BOOL HAL_MVD_SetAVSyncThreshold(MS_U8 u8Idx, MS_U32 u32Th);
MS_BOOL HAL_MVD_SetAVSyncFreerunThreshold(MS_U8 u8Idx, MS_U32 u32Th);
MS_BOOL HAL_MVD_ChangeAVsync(MS_U8 u8Idx, MS_BOOL bEnable, MS_U16 u16PTS);

MS_BOOL HAL_MVD_GetIsSyncRep(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetIsSyncSkip(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetIsSyncReach(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetDispRdy(MS_U8 u8Idx);

MVD_DecStat HAL_MVD_GetDecodeStatus(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetParserByteCnt(MS_U8 u8Idx);

MS_BOOL MVD_IntHasUsrDataDisp(MS_U32 u32IntStat);
MS_BOOL MVD_IntHasUsrData(MS_U32 u32IntStat);
MS_BOOL MVD_IntIsDispRdy(MS_U32 u32IntStat);
MS_BOOL MVD_IntHasSeqHdr(MS_U32 u32IntStat);
MS_BOOL MVD_IntHas1stFrame(MS_U32 u32IntStat);
MS_BOOL MVD_IntHasXcLowDelay(MS_U32 u32IntStat);
MS_BOOL MVD_IntHasDecodeIframe(MS_U32 u32IntStat);
MS_BOOL MVD_IntVSyncInt(MS_U32 u32IntStat);
MS_BOOL MVD_IntDecOneFrmInt(MS_U32 u32IntStat);
MS_BOOL HAL_MVD_EnableInt(MS_U8 u8Idx, MS_U32 bEn);
MS_BOOL MVD_IntHasESDataInvalid(MS_U32 u32IntStat);
MS_BOOL MVD_IntHasDecodeErr(MS_U32 u32IntStat);
E_MVD_Result HAL_MVD_EnableDispOneField(MS_U8 u8Idx, MS_BOOL bEn);
E_MVD_Result HAL_MVD_SetFdMaskDelayCount(MS_U8 u8Idx, MS_U16 u16Cnt);
E_MVD_Result HAL_MVD_SetOutputFRCMode(MS_U8 u8Idx, MS_U8 u8FrameRate, MS_U8 u8Interlace);
E_MVD_Result HAL_MVD_SetFRCDropType(MS_U8 u8Idx, MS_U8 u8DropType);
E_MVD_Result HAL_MVD_SetDisableSeqChange(MS_U8 u8Idx, MS_BOOL bEnable);
MS_BOOL HAL_MVD_DropErrorFrame(MS_U8 u8Idx, MS_BOOL bDrop);

void HAL_MVD_SetPTSBase(MS_U8 u8Idx, MS_U32 u32pts);
MS_U32 HAL_MVD_GetPTS(MS_U8 u8Idx);
MS_U64 HAL_MVD_GetU64PTS(MS_U8 u8Idx,MVD_PtsType eType);
MS_BOOL HAL_MVD_SkipData(MS_U8 u8Idx);
MS_BOOL HAL_MVD_DispRepeatField(MS_U8 u8Idx, MS_BOOL bEnable);
MS_BOOL HAL_MVD_PauseDisp(MS_U8 u8Idx);
MS_BOOL HAL_MVD_FlushDisplayBuf(MS_U8 u8Idx);
MS_VIRT HAL_MVD_GetTsFileESReadPtr(MS_U8 u8Idx);
MS_VIRT HAL_MVD_GetTsFileESWritePtr(MS_U8 u8Idx);
void HAL_MVD_SetDivXCfg(MS_U8 u8Idx, MS_U8 u8MvAdjust, MS_U8 u8IdctSel);
MS_VIRT HAL_MVD_GetSLQReadPtr(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetSLQWritePtr(MS_U8 u8Idx, MS_BOOL bCheckData);

void HAL_MVD_SetOverflowTH(MS_U8 u8Idx, MS_U32 u32Threshold);
void HAL_MVD_SetUnderflowTH(MS_U8 u8Idx, MS_U32 u32Threshold);
MS_BOOL HAL_MVD_Stop(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetLastCmd(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SkipToIFrame(MS_U8 u8Idx);
MS_BOOL HAL_MVD_DispCtrl(MS_U8 u8Idx, MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode);
MS_BOOL HAL_MVD_StepDecode(MS_U8 u8Idx);
MS_BOOL HAL_MVD_TrickPlay(MS_U8 u8Idx, MVD_TrickDec trickDec, MS_U8 u8DispDuration);
MS_BOOL HAL_MVD_GenPattern(MS_U8 u8Idx, MVD_PatternType ePattern, MS_PHY u32PAddr, MS_U32* pu32Size);

MVD_CodecType HAL_MVD_GetCodecType(MS_U8 u8Idx);
MVD_SrcMode HAL_MVD_GetSrcMode(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetFWCfg(MS_U8 u8Idx, MVD_FWCfg* pFWCfg);
MS_U8 HAL_MVD_GetFBMode(MS_U8 u8Idx);
MVD_CtrlCfg* HAL_MVD_GetCtrlCfg(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetMEMCfg(MS_U8 u8Idx, MVD_MEMCfg* pMEMCfg);
MVD_MEMCfg* HAL_MVD_GetMEMCfg(MS_U8 u8Idx);

E_MVD_Result HAL_MVD_DbgGetData(MS_VIRT u32Addr, MS_U32* u32Data);
void HAL_MVD_DbgDumpBits(MS_U8 u8Idx, MS_PHY u32base, MS_U32 u32size);
void HAL_MVD_DbgDump(MS_U8 u8Idx);
void HAL_MVD_SetDbgLevel(MS_U8 level);
MS_U32 HAL_MVD_CPUGetPC(void);
MS_U8 HAL_MVD_SidToIdx(HAL_MVD_StreamId eSID);
MS_BOOL HAL_MVD_ReleaseFdMask(MS_U8 u8Idx, MS_BOOL bRls);
MS_BOOL HAL_MVD_ParserRstDone(MS_U8 u8Idx, MS_BOOL bEnable);
MS_BOOL HAL_MVD_FlushPTSBuf(MS_U8 u8Idx,MS_BOOL bEnable);
MS_BOOL HAL_MVD_ShowDecodeOrder(MS_U8 u8Idx, MS_U8 u8Mode);
MS_U32 HAL_MVD_GetSLQNum(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetDispQNum(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetAutoMute(MS_U8 u8Idx, MS_BOOL bEn);
MS_BOOL HAL_MVD_Init_Share_Mem(void);
MS_BOOL HAL_MVD_SetVSizeAlign(MS_U8 u8Idx, MS_BOOL bEn);
E_MVD_Result HAL_MVD_GetCrcValue(MS_U8 u8Idx, MVD_CrcIn *pCrcIn, MVD_CrcOut *pCrcOut);
E_MVD_Result HAL_MVD_EnablePTSDetector(MS_U8 u8Idx, MS_BOOL bEn);
E_MVD_Result HAL_MVD_DisablePBFrameMode(MS_U8 u8Idx, MS_BOOL bEn);

MS_BOOL HAL_MVD_SuspendDynamicScale(MS_U8 u8Idx, MS_BOOL bEn);
MS_U8 HAL_MVD_GetSuspendDynamicScale(MS_U8 u8Idx);
MS_U8 HAL_MVD_GetStereoType(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetDivxVer(MS_U8 u8Idx);
MS_BOOL HAL_MVD_SetIdctMode(MS_U8 u8Idx, MS_U8 u8Mode);
E_MVD_Result HAL_MVD_EX_SetClockSpeed(HAL_MVD_EX_ClockSpeed eClockSpeed);
MS_BOOL HAL_MVD_GetIsAVSyncOn(MS_U8 u8Idx);
MS_BOOL HAL_MVD_ShowFirstFrameDirect(MS_U8 u8Idx, MS_U8 bEnable);
E_MVD_Result HAL_MVD_SetXCLowDelayPara(MS_U8 u8Idx,MS_U32 u32Para);
MS_BOOL HAL_MVD_EnableDynamicScale(MS_U8 u8Idx,MS_U8 u8NewDS);
MS_U8 HAL_MVD_GetESBufferStatus(MS_U8 u8Idx);
MS_BOOL HAL_MVD_Field_Polarity_Display_One_field(MS_U8 u8Idx, MS_BOOL bEn,MS_U8 top_bottom);
MS_BOOL HAL_MVD_SetExternalDSBuff(MS_U8 u8Idx, MS_VIRT u32VPUAddr, MS_VIRT u32DrvAddr);
MS_BOOL HAL_MVD_SetShareMemoryBase(MS_U8 u8Idx, MS_VIRT u32base, MS_BOOL bsel);
MS_BOOL HAL_MVD_GetShareMemoryOffset(MS_U8 u8Idx, MS_VIRT *u32base);
MS_BOOL HAL_MVD_Support2ndMVOPInterface(void);
MS_BOOL HAL_MVD_SetExternal_CC_Buffer(MS_U8 u8Idx, MS_VIRT u32base, MS_U8 u8size, MS_U8 cc_type);
MS_BOOL HAL_MVD_GetPVRSeamlessInfo(MS_U8 u8Idx,void* param);
MS_VIRT HAL_MVD_GetVsyncBridgeAddr(MS_U8 u8Idx);
E_MVD_Result HAL_MVD_HWBuffer_ReMappingMode(MS_U8 u8Idx,MS_BOOL bEnable);
MS_BOOL HAL_MVD_SetPrebufferSize(MS_U8 u8Idx, MS_U32 size);
MS_VIRT HAL_MVD_GetVsyncAddrOffset(void);
MS_VIRT HAL_MVD_GetVsyncExtAddrOffset(void);
E_MVD_Result HAL_MVD_SetTimeIncPredictParam(MS_U8 u8Idx, MS_U32 u32time);
MS_BOOL HAL_MVD_SetDecodeTimeoutParam(MS_U8 u8Idx, MS_BOOL enable,MS_U32 u32timeout);
MS_BOOL HAL_MVD_SetSrcMode(MS_U8 u8Idx,MVD_SrcMode mode);
MS_BOOL _MVD_SetDynamicAllocateFB(MS_U8 u8Idx,MS_BOOL bEnable);
E_MVD_Result HAL_MVD_Set_Smooth_Rewind(MS_U8 u8Idx, MS_U8 btype);
E_MVD_Result HAL_MVD_IsAlive(MS_U8 u8Idx);
E_MVD_Result HAL_MVD_Set_Err_Tolerance(MS_U8 u8Idx, MS_U16 u16Para);
void HAL_MVD_EnableAutoInsertDummyPattern(MS_U8 u8Idx, MS_BOOL bEnable);
void HAL_MVD_Drop_One_PTS(MS_U8 u8Idx);
E_MVD_Result HAL_MVD_PVR_Seamless_mode(MS_U8 u8Idx, MS_U8 u8Arg);
E_MVD_Result HAL_MVD_SetDisplayFinishMode(MS_U8 u8Idx, MS_U8 u8Mode);
MS_VIRT HAL_MVD_GetVBBUTableAddr(MS_U8 u8Idx);
MS_BOOL HAL_MVD_GetColorInfo(MS_U8 u8Idx, MVD_Color_Info* pstColorInfo);
#ifdef VDEC3
MS_BOOL HAL_MVD_IsDisplayCommand(MS_U8 u8Cmd);
MS_BOOL HAL_MVD_IsNormalCommand(MS_U8 u8Cmd);
MS_BOOL HAL_MVD_IsNeedResponseCommand(MS_U8 u8Cmd);
#endif
void HAL_MVD_ResetHandShake(MS_U8 u8Idx, MVD_HANDSHAKE_CMD cmd);
MS_BOOL HAL_MVD_FRC_OnlyShowTopField(MS_U8 u8Idx, MS_BOOL bEnable);

void HAL_MVD_SetDmxFrameRate(MS_U8 u8HalIdx,MS_U32 u32Value);
void HAL_MVD_SetDmxFrameRateBase(MS_U8 u8HalIdx,MS_U32 u32Value);
void HAL_MVD_TrickPlay2xAVSync(MS_U8 u8Idx,MS_BOOL bEnable);
void HAL_MVD_SetFrameBufferMiu(MS_U8 u8HalIdx,MS_U8 u8MiuIdx);
void HAL_MVD_SetCMAAllocateDone(MS_U8 u8HalIdx);
void HAL_MVD_GetCMAInformation(MS_U8 u8HalIdx,MS_U64* u64Addr,MS_SIZE* u64Size,MS_BOOL* bAllocDone);
void HAL_MVD_SetCMAInformation(void* cmaInitParam);
E_MVD_Result HAL_MVD_Set_SlowSyncParam(MS_U8 u8Idx, MS_U8 u8RepeatPeriod,MS_U8 u8DropPeriod);
MS_BOOL HAL_MVD_VariableFrameRate(MS_U8 u8Idx);
E_MVD_DISP_QUEUE HAL_MVD_IsDispQueueEmpty(MS_U8 u8Idx);
MS_U32 HAL_MVD_GetMinTspDataSize(MS_U8 u8HalIdx);
#endif

#endif // _DEV_MVD_H_
