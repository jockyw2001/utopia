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

#include "mvd4_interface.h" //firmware header
#include "halVPU.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//#define MVD_ENABLE_ISR    //unmark this to enable MVD ISR

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

//Init command arguments
#define SETUP_CMDARG(x)     \
            do {            \
                x.Arg0 = 0; \
                x.Arg1 = 0; \
                x.Arg2 = 0; \
                x.Arg3 = 0; \
                x.Arg4 = 0; \
                x.Arg5 = 0; \
               } while(0)

//Set command arguments
#define SET_CMDARG(cmd, u32val)             \
            do {                            \
                cmd.Arg0 = L_WORD(u32val);  \
                cmd.Arg1 = H_WORD(u32val);  \
                cmd.Arg2 = L_DWORD(u32val); \
                cmd.Arg3 = H_DWORD(u32val); \
                cmd.Arg4 = 0;               \
                cmd.Arg5 = 0;               \
               } while(0)

void HAL_MVD_RegSetBase(MS_U32 u32Base);
MS_U8 HAL_MVD_RegReadByte(MS_U32 u32Reg);
void HAL_MVD_RegWriteByte(MS_U32 u32Reg, MS_U8 u8Val);
void HAL_MVD_RegWriteBit(MS_U32 u32Reg, MS_BOOL bEnable, MS_U8 u8Mask);
void HAL_MVD_RegWriteByteMask(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Msk);
void HAL_MVD_RegWrite4Byte(MS_U32 u32Reg, MS_U32 u32Val);

// Mutex function for HI interface
#define HAL_MVD_LockHiIfMutex()
#define HAL_MVD_UnlockHiIfMutex()

#define HAL_MVD_SetVpuFwDecoder(x) HAL_VPU_SetFWDecoder(x)

#define MVD_SUPPORT_MPEG2 0x01
#define MVD_SUPPORT_MPEG4 0x02
#define MVD_SUPPORT_VC1   0x04

#define MVD_FW_VERSION   FW_VERSION
#define MVD_FW_CODE_LEN                    (0x40000UL)  //256K
#define MVD_SYNC_DONE    1

typedef enum
{
    E_MVD_VER_ERR = 0x0,
    E_MVD_VER_4  = 0x1,
    E_MVD_VER_5  = 0x10,
    E_MVD_VER_6  = 0x100,
    E_MVD_VER_MAX =0x11111111
} MVD_VER_Type;
//------------------------------------------------------------------------------//
//MVD Engine family description (from mvd4 ~ mvd ??)
//*****
//MVD 4
//==>T3 ,U3, JANUS,T4 ,U4 , K1, M10
//
//#define MVD_HW_VER E_MVD_VER_4
//#define MVD_SLQ_TBL_ENTRY_LENS 0x0FFFFFFF
//#define MVD_SLQ_TBL_ENTRY_SIZES 28
//#define MVD_SLQ_TBL_SHIFT_MASK 0xff
//#define MVD_SLQ_TBL_SHIFT_BIT1 24
//#define MVD_SLQ_TBL_SHIFT_BIT2  4
//#define MVD_TILE_MODE_MASK 0x0007
//#define MVD_TILE_MODE_PITCH_LENS 8
//#define MVD_TILE_MODE_PITCH_SIZES  3
//
//MVD 5
//==>T8 ,T9, J2 ,T11 ,T12 ,T13
//
//#define MVD_HW_VER (E_MVD_VER_5|E_MVD_VER_4)
//#define MVD_SLQ_TBL_ENTRY_LENS 0x1FFFFFFF
//#define MVD_SLQ_TBL_ENTRY_SIZES 29
//#define MVD_SLQ_TBL_SHIFT_MASK 0x3f
//#define MVD_SLQ_TBL_SHIFT_BIT1 26
//#define MVD_SLQ_TBL_SHIFT_BIT2 3
//#define MVD_TILE_MODE_MASK 0x000f
//#define MVD_TILE_MODE_PITCH_LENS 16
//#define MVD_TILE_MODE_PITCH_SIZES 4
//
//MVD 6
//==>A1 ,A2 ,A5
//
//#define MVD_HW_(E_MVD_VER_6|E_MVD_VER_5|E_MVD_VER_4)
//#define MVD_SLQ_TBL_ENTRY_LENS 0x1FFFFFFF
//#define MVD_SLQ_TBL_ENTRY_SIZES 29
//#define MVD_SLQ_TBL_SHIFT_MASK 0x3f
//#define MVD_SLQ_TBL_SHIFT_BIT1 26
//#define MVD_SLQ_TBL_SHIFT_BIT2 3
//#define MVD_TILE_MODE_MASK 0x000f
//#define MVD_TILE_MODE_PITCH_LENS 16
//#define MVD_TILE_MODE_PITCH_SIZES 4
//
//------------------------------------------------------------------------------//
#if defined(CHIP_MUJI)
#define MVD_HW_VER (E_MVD_VER_6|E_MVD_VER_5|E_MVD_VER_4)
#define MVD_SLQ_TBL_ENTRY_LENS 0x7FFFFFFF
#define MVD_SLQ_TBL_ENTRY_SIZES 31
#define MVD_SLQ_TBL_SHIFT_MASK 0x0
#define MVD_SLQ_TBL_SHIFT_BIT1 0
#define MVD_SLQ_TBL_SHIFT_BIT2 0
#define MVD_TILE_MODE_MASK 0x000f
#define MVD_TILE_MODE_PITCH_LENS 16
#define MVD_TILE_MODE_PITCH_SIZES 4
#define MVD_SUPPORT_DHD 1
#else
    #error "The CHIP version not verify yet ,Plz contact MVD owner !!"
#endif

#ifdef REDLION_LINUX_KERNEL_ENVI
#if defined(CHIP_T3)
#define MVD_MIU1_BASE_ADDRESS   0x08000000UL//0x10000000UL
#else
#define MVD_MIU1_BASE_ADDRESS   0x08000000UL
#endif
#endif

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

#if 0
typedef struct _MVD_HWCfg
{
    MS_U32 MVD_HWVER;
    MS_U32 u32MvdSlqTabLens;
    MS_U32 u32MvdSlqTabSizes;
    MS_U32 u32MvdSlqTabShiftMask;
    MS_U32 u32MvdSlqTabShiftBit1;
    MS_U32 u32MvdSlqTabShiftBit2;

    MS_U16 u16MvdTileModeMask;
    MS_U16 u16MvdTileModePitchLens;
    MS_U16 u16MvdTileModePitchSize;

} MVD_HWCfg;
#endif

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_U8 HAL_MVD_GetCaps(void);
void HAL_MVD_MemSetMap(MS_U8 u8type, MS_U32 u32addr, MS_U32 u32len);
void HAL_MVD_MemGetMap(MS_U8 u8type, MS_U32* pu32addr, MS_U32* pu32len);
MS_U32 HAL_MVD_MemRead4Byte(MS_U32 u32Address);
MS_U16 HAL_MVD_MemRead2Byte(MS_U32 u32Address);
MS_U8 HAL_MVD_MemReadByte(MS_U32 u32Address);
MS_BOOL HAL_MVD_MemWrite4Byte(MS_U32 u32Address, MS_U32 u32Value);
MS_BOOL HAL_MVD_MemWrite2Byte(MS_U32 u32Address, MS_U16 u16Value);
MS_BOOL HAL_MVD_MemWriteByte(MS_U32 u32Address, MS_U8 u8Value);
void HAL_MVD_SetReqMask(MS_BOOL bEnMask);

MS_PHYADDR  HAL_MVD_PA2NonCacheSeg(MS_PHYADDR u32PhyAddr);
MS_U32      HAL_MVD_GetTime(void);
void        HAL_MVD_Delayms(MS_U32 u32msecs);
void        HAL_MVD_CPU_Sync(void);
void        HAL_MVD_FlushMemory(void);
void        HAL_MVD_ReadMemory(void);

MS_BOOL HAL_MVD_RstHW(void);
MS_BOOL HAL_MVD_ReleaseFW(void);
MS_BOOL HAL_MVD_LoadCode(void);
void HAL_MVD_SetFWBinID(MS_U32 u32SrcID);

MS_U8 HAL_MVD_GetChipMajorID(void);

void HAL_MVD_PowerCtrl(MS_BOOL bOn);
MS_BOOL HAL_MVD_TimeOut(void);
MS_BOOL HAL_MVD_MVDCommand(MS_U8 u8cmd, MVD_CmdArg *pstCmdArg);
MS_BOOL HAL_MVD_InitHW(void);
MS_BOOL HAL_MVD_InitFW(void);
MS_U32 HAL_MVD_GetFWVer(void);
MS_BOOL HAL_MVD_SoftRstHW(void);
void HAL_MVD_ClearIRQ(void);

MS_BOOL HAL_MVD_SetSpeed(MVD_SpeedType eSpeedType, MS_U8 u8Multiple);
MS_BOOL HAL_MVD_EnableForcePlay(void);
void HAL_MVD_SetFrameBuffAddr(MS_U32 u32addr, MS_U8 u8fbMode);
void HAL_MVD_SetHeaderBufferAddr(MS_U32 u32addr);
void HAL_MVD_SetVolInfoBufferAddr(MS_U32 u32addr);
void HAL_MVD_SetFrameInfoBufferAddr(MS_U32 u32addr);
void HAL_MVD_SetIAPBufferAddr(MS_U32 u32addr);
void HAL_MVD_SetDPBufferAddr(MS_U32 u32addr);
void HAL_MVD_SetMVBufferAddr(MS_U32 u32addr);
void HAL_MVD_SetUserDataBuf(MS_U32 u32addr, MS_U32 u32size);
void HAL_MVD_SetSLQTblBufStartEnd(MS_U32 u32start, MS_U32 u32end);
MS_BOOL HAL_MVD_StepDisp(void);
MS_BOOL HAL_MVD_EnableLastFrameShow(MS_BOOL bEnable);
MS_BOOL HAL_MVD_SlqTblRst(void);
MS_BOOL HAL_MVD_SeekToPTS(MS_U32 u32Pts);
MS_BOOL HAL_MVD_SkipToPTS(MS_U32 u32Pts);
MS_BOOL HAL_MVD_SetFileModeAVSync(MVD_TIMESTAMP_TYPE eSyncMode);
MS_BOOL HAL_MVD_SetPtsTblAddr(MS_U32 u32addr);
MS_BOOL HAL_MVD_SetScalerInfoAddr(MS_U32 u32addr);
MS_BOOL HAL_MVD_SetDynamicScaleAddr(MS_U32 u32addr);
MS_BOOL HAL_MVD_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height);
MS_BOOL HAL_MVD_EnableDynamicScale(void);
MS_BOOL HAL_MVD_SetBlueScreen(MS_BOOL bEn);
MS_BOOL HAL_MVD_Resume(void);
MS_BOOL HAL_MVD_SetFreezeDisp(MS_BOOL bEn);
void HAL_MVD_SetDecFrmInfoAddr(MS_U32 u32addr);
MS_U32 HAL_MVD_GetIntState(void);
MS_U8 HAL_MVD_GetAVSyncStatus(void);
void HAL_MVD_SlqTblLoadWrPtr(MS_U32 u32WrPtr);
MS_U32 HAL_MVD_SlqTblProbeWrPtr(void);
MS_U32 HAL_MVD_SlqTblProbeRdPtr(void);
MS_BOOL HAL_MVD_SetMStreamerMode(MS_U8 u8Mode);
MS_BOOL HAL_MVD_FrameOpt(MS_U8 u8FrmIdx, MVD_FrmOpt eOpt);
MS_BOOL HAL_MVD_UpdatePts(void);
MS_BOOL HAL_MVD_FrameCapture(MS_U8 u8FrmIdx, MS_BOOL bEnable);
MS_BOOL HAL_MVD_GetFrmRateIsSupported(MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate);

void HAL_MVD_SetDbgLevel(MS_U8 level);
//MS_BOOL HAL_MVD_GetMVDEngineCfg(MVD_HWCfg* MvdHWCfg);

#endif // _DEV_MVD_H_
