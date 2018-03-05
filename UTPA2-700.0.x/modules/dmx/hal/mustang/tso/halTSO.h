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
// Copyright (c) 20011-20013 MStar Semiconductor, Inc.
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

////////////////////////////////////////////////////////////////////////////////////////////////////
// file   halTSO.h
// @brief  TS I/O (TSO) HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_TSO_H__
#define __HAL_TSO_H__

#include "MsCommon.h"

#include "regTSO.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------

#define _TsoPid                      ((REG_Pid*)(REG_PIDFLT_BASE))

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------


////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////

// TSO in mux select
#define HAL_TSOIN_MUX_TS0                       0x0000UL
#define HAL_TSOIN_MUX_TS1                       0x0001UL
#define HAL_TSOIN_MUX_TS2                       0x0002UL
#define HAL_TSOIN_MUX_TS3                       0x0003UL
#define HAL_TSOIN_MUX_TS4                       0x0004UL
#define HAL_TSOIN_MUX_TS5                       0x0005UL
#define HAL_TSOIN_MUX_TSCB                      0xFFFFUL   //not supported
#define HAL_TSOIN_MUX_TSDEMOD0                  0x0007UL
#define HAL_TSOIN_MUX_TSDEMOD1                  0xFFFFUL   //not supported
#define HAL_TSOIN_MUX_MEM                       0x0008UL
#define HAL_TSOIN_MUX_MEM1                      0x0009UL

// TSO output mux select
#define HAL_TSOOUT_MUX_TS1                      0x0001UL
#define HAL_TSOOUT_MUX_TS3                      0x0003UL


// TSO In Clk select
#define HAL_TSO_CLKIN_TS0                       0x0000UL
#define HAL_TSO_CLKIN_TS1                       0x0001UL
#define HAL_TSO_CLKIN_TS2                       0x0002UL
#define HAL_TSO_CLKIN_TS3                       0x0004UL
#define HAL_TSO_CLKIN_TSCB                      0xFFFFUL  //not supported
#define HAL_TSO_CLKIN_TSDEMOD0                  0x0003UL
#define HAL_TSO_CLKIN_TSDEMOD1                  0xFFFFUL  //not supported
#define HAL_TSO_CLKIN_TS4                       0x0005UL
#define HAL_TSO_CLKIN_TS5                       0x0006UL


// Pre TsoOut Select
#define HAL_PRE_TSO_OUT_SEL_TS0IN               0x0000UL
#define HAL_PRE_TSO_OUT_SEL_TS1IN               0x0001UL
#define HAL_PRE_TSO_OUT_SEL_TS2IN               0x0002UL
#define HAL_PRE_TSO_OUT_SEL_TS3IN               0x0003UL
#define HAL_PRE_TSO_OUT_SEL_TS4IN               0x0004UL
#define HAL_PRE_TSO_OUT_SEL_TS5IN               0x0005UL
#define HAL_PRE_TSO_OUT_SEL_TSCBIN              0xFFFFUL  //not supported
#define HAL_PRE_TSO_OUT_SEL_TSDEMOD0            0xFFFFUL  //not supported

// TSO ClkOut DivClk Src Select
#define HAL_TSO_OUT_DIV_SEL_172M_2N             0x0000UL
#define HAL_TSO_OUT_DIV_SEL_288M_2N             0x0800UL
#define HAL_TSO_OUT_DIV_SEL_216M_N              0xFFFFUL  //not supported

// TSO ClkOut Select
#define HAL_TSO_OUT_SEL_TSO_OUT_DIV2N           0x0000UL
#define HAL_TSO_OUT_SEL_TSO_OUT_62MHz           0x0400UL
#define HAL_TSO_OUT_SEL_TSO_OUT_54MHz           0x0800UL
#define HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT         0x0C00UL
#define HAL_TSO_OUT_SEL_TSO_OUT_PTSOOUT_DIV8    0x1000UL
#define HAL_TSO_OUT_SEL_TSO_OUT_27MHz           0x1400UL
#define HAL_TSO_OUT_SEL_TSO_OUT_FROM_DEMOD      0x1C00UL
#define HAL_TSO_OUT_SEL_TSO_OUT_DIV             0xFFFFUL  //not supported

//TSO Channel IF
#define HAL_TSO_TSIF_LIVE1                      1UL
#define HAL_TSO_TSIF_LIVE2                      5UL
#define HAL_TSO_TSIF_LIVE3                      6UL
#define HAL_TSO_TSIF_FILE1                      HAL_TSO_TSIF_LIVE2
#define HAL_TSO_TSIF_FILE2                      HAL_TSO_TSIF_LIVE3
#define HAL_TSO_TSIF_LIVE_MMT                   7UL


//TSO SVQ RX packet mode
#define HAL_TSO_SVQRX_MODE_OPENCABLE            0UL
#define HAL_TSO_SVQRX_MODE_CIP                  1UL
#define HAL_TSO_SVQRX_MODE_192                  2UL

//TSO timestamp base
#define HAL_TSO_TIMESTAMP_90K                   0UL
#define HAL_TSO_TIMESTAMP_27M                   1UL

#define MIU_BUS                                 4UL

//TSO PVR SRC
#define HAL_TSO_PVR_NONE                        0UL
#define HAL_TSO_PVR_SVQ                         1UL
#define HAL_TSO_PVR_MMT                         2UL

typedef struct stDrvTSOOutClk
{
    MS_U16  u16OutClk;
    MS_U16  u16OutDivSrc;
    MS_U16  u16OutDivNum;
    MS_U16  u16PreTsoOutClk;
    MS_BOOL bClkInvert;
    MS_BOOL bEnable;
}HalTSOOutClk;

//
// General API
void        HAL_TSO_SetBank(MS_VIRT virtBankAddr);
void        HAL_TSO_Init(void);
void        HAL_TSO_Reset_All(MS_U8 u8Eng);
void        HAL_TSO_Reset(MS_U8 u8Eng);
void        HAL_TSO_Reset_SubItem(MS_U8 u8Eng, MS_U16 u16RstItem);
void        HAL_TSO_HWInt_Enable(MS_U8 u8Eng, MS_BOOL bEnable, MS_U16 u16init);
void        HAL_TSO_HWInt_Clear(MS_U8 u8Eng, MS_U16 u16Int);
MS_U16      HAL_TSO_HWInt_Status(MS_U8 u8Eng);
void        HAL_TSO_PowerCtrl(MS_BOOL bOn);

void        HAL_TSO_Recover_TSOutMode(MS_U8 u8Eng);
MS_BOOL     HAL_TSO_OutPad(MS_U8 u8Eng, MS_U16* pu16OutPad, MS_BOOL bSet);

MS_BOOL     HAL_TSO_SelPad(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16InPadSel, MS_BOOL bParallel);
MS_BOOL     HAL_TSO_Set_InClk(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16 u16ClkSel, MS_BOOL bClkInvert, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_GetInputTSIF_Status(MS_U8 u8Eng, MS_U8 u8TsIf, MS_U16* pu16Pad, MS_BOOL* pbClkInvert, MS_BOOL* pbExtSync, MS_BOOL* pbParl);
MS_BOOL     HAL_TSO_OutClk_DefSelect(MS_U8 u8Eng, MS_U16 u16PadSel, MS_BOOL bSet, HalTSOOutClk* pstOutClkSet);
void        HAL_TSO_OutputClk(MS_U8 u8Eng, HalTSOOutClk* pstOutClkSet, MS_BOOL bSet);
MS_BOOL     HAL_TSO_GetOutputClk(MS_U8 u8Eng, HalTSOOutClk* pstOutClkSet);
MS_BOOL     HAL_TSO_Set_TSOOut_Phase_Tune(MS_U8 u8Eng, MS_U16 u16ClkOutPhase, MS_BOOL bPhaseEnable);
MS_BOOL     HAL_TSO_PreTsoOutClk(MS_U8 u8Eng, MS_U16* pu16PreTsoOutSel, MS_BOOL bSet);
MS_BOOL     HAL_TSO_TSOOutDiv(MS_U8 u8Eng, MS_U16* pu16ClkOutDivSrcSel, MS_U16* pu16ClkOutDivNum, MS_BOOL bSet);
MS_BOOL     HAL_TSO_OutClk(MS_U8 u8Eng, MS_U16* pu16ClkOutSel, MS_BOOL* pbClkInvert, MS_BOOL* pbEnable, MS_BOOL bSet);

void        HAL_TSO_Flt_SetPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16PID);
void        HAL_TSO_Flt_SetInputSrc(MS_U8 u8Eng, MS_U16 u16FltId, MS_U16 u16InputSrc);
MS_BOOL     HAL_TSO_ReplaceFlt_SetPktPid(MS_U8 u8Eng, MS_U16 u16FltId, MS_U8 u8TsIf, MS_U16 u16OldPid, MS_U16 u16NewPid);
MS_BOOL     HAL_TSO_ReplaceFlt_Enable(MS_U8 u8Eng, MS_U16 u16FltId, MS_BOOL bEnable);

void        HAL_TSO_Set_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng, MS_PHY phyAddr);
void        HAL_TSO_Set_Filein_ReadLen(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32len);
MS_PHY      HAL_TSO_Get_Filein_ReadAddr(MS_U8 u8Eng, MS_U8 u8FileEng);
void        HAL_TSO_Set_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16ctrl);
MS_U16      HAL_TSO_Get_Filein_Ctrl(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_Set_Filein_MOBFKey(MS_U8 u8FileEng, MS_U32 u32Key, MS_BOOL bSecured);
MS_BOOL     HAL_TSO_Filein_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_Set_Filein_MOBFKey(MS_U8 u8FileEng, MS_U32 u32Key, MS_BOOL bSecured);
void        HAL_TSO_FileinTimer_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
void        HAL_TSO_Filein_Rate(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U16 u16timer);
void        HAL_TSO_Filein_192Mode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
void        HAL_TSO_Filein_192BlockMode_Enable(MS_U8 u8Eng, MS_U8 u8FileEng, MS_BOOL bEnable);
MS_U16      HAL_TSO_CmdQ_FIFO_Get_WRCnt(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_CmdQ_FIFO_IsFull(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_CmdQ_FIFO_IsEmpty(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_U8       HAL_TSO_CmdQ_FIFO_Get_WRLevel(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_CmdQ_Reset(MS_U8 u8Eng, MS_U8 u8FileEng);

void        HAL_TSO_RW_ValidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16ValidBlockCnt);
void        HAL_TSO_RW_InvalidBlock_Count(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16InvalidBlockCnt);
void        HAL_TSO_RW_OutputPktSize(MS_U8 u8Eng, MS_BOOL bWrite, MS_U16 *pu16PktSize);

void        HAL_TSO_LPcr2_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U32 u32lpcr2);
MS_U32      HAL_TSO_LPcr2_Get(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_U32      HAL_TSO_TimeStamp_Get(MS_U8 u8Eng, MS_U8 u8FileEng);
MS_BOOL     HAL_TSO_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8If, MS_U8 u8size);
void        HAL_TSO_Filein_PktChkSize_Set(MS_U8 u8Eng, MS_U8 u8FileEng, MS_U8 u8size);

void        HAL_TSO_Cfg1_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL bEnable);
void        HAL_TSO_Cfg4_Enable(MS_U8 u8Eng, MS_U16 u16CfgItem, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_ChIf_Enable(MS_U8 u8Eng, MS_U8 u8ChIf, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16 u16Cfg, MS_BOOL bEnable);
MS_BOOL     HAL_TSO_Get_ChIf_Cfg(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Cfg, MS_BOOL *pbEnable);
MS_BOOL     HAL_TSO_SVQBuf_Set(MS_U8 u8Eng, MS_U8 u8ChIf, MS_PHY phyBufAddr, MS_U32 u32BufSize);
MS_BOOL     HAL_TSO_ChIf_ClrByteCnt(MS_U8 u8Eng, MS_U8 u8ChIf);
MS_BOOL     HAL_TSO_LocalStreamID(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U8* pu8StrID, MS_BOOL beSet);
MS_BOOL     HAL_TSO_SVQ_TX_Reset(MS_U8 u8Eng, MS_U8 u8ChIf);
MS_BOOL     HAL_TSO_Set_SVQRX_MOBFKey(MS_U8 u8Eng, MS_U32 u32Key, MS_BOOL bSecured);
MS_BOOL     HAL_TSO_Set_SVQRX_PktMode(MS_U8 u8Eng, MS_U16 u16mode);
MS_BOOL     HAL_TSO_Set_SVQRX_ArbitorMode(MS_U8 u8Eng, MS_U16 u16mode, MS_U16 *pu16SvqRxPri);
MS_BOOL     HAL_TSO_Set_SVQ_LocalSysTimestamp(MS_U8 u8Eng, MS_U32 u32systime);
MS_BOOL     HAL_TSO_Get_SVQ_Status(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U16* pu16Status);
MS_BOOL     HAL_TSO_GetDelayTime_PreHd2Output(MS_U8 u8Eng, MS_U8 u8ChIf, MS_U32 *pu32time);
MS_BOOL     HAL_TSO_Get_MaxDelta_ChId(MS_U8 u8Eng, MS_U8 *pu8ChIf);
MS_BOOL     HAL_TSO_Sel_LocalSysStampClkBase(MS_U8 u8Eng, MS_U16 u16ClkBase);
MS_U16      HAL_TSO_MMT_Cfg_Map(MS_U16 u16Cfg);
void        HAL_TSO_PVR_WaitFlush(MS_U8 u8PVRId);
void        HAL_TSO_PVR_SetBuffer(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
MS_PHY      HAL_TSO_PVR_GetBufWrite(MS_U8 u8PVRId);
void        HAL_TSO_PVR_Enable(MS_U8 u8PVRId, MS_BOOL bEnable);
void        HAL_TSO_PVR_PacketMode(MS_U8 u8PVRId, MS_BOOL bSet);
void        HAL_TSO_SetPVRTimeStamp(MS_U8 u8PVRId, MS_U32 u32Stamp);
MS_U32      HAL_TSO_GetPVRTimeStamp(MS_U8 u8PVRId);
void        HAL_TSO_SetPVRTimeStampClk(MS_U8 u8PVRId, MS_U32 u32ClkSrc);
MS_BOOL     HAL_TSO_PVR_IsEnabled(MS_U32 u32EngId);
void        HAL_TSO_PVR_Src(MS_U32 u32Src);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL     HAL_TSO_SaveRegs(void);
MS_BOOL     HAL_TSO_RestoreRegs(void);
#endif  //MSOS_TYPE_LINUX_KERNEL


#endif // #ifndef __HAL_TSO_H__


