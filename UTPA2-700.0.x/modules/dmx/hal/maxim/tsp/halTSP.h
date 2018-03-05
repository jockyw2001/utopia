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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
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
// file   halTSP.h
// @brief  Transport Stream Processer (TSP) HAL
// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __HAL_TSP_H__
#define __HAL_TSP_H__

#include "MsCommon.h"

#include "regTSP.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------
#define TSP_HWPCR_BY_HK                 0UL    //Tuning STC by driver side
#define TSP_AUDIO3_AUDIO4_SUPPORT       1UL    //Support AUDIO3 & AUDIO4 FIFO

//--------------------------------------------------------------------------------------------------
//  TSP Hardware Abstraction Layer
//--------------------------------------------------------------------------------------------------
// TSP Register
#define _TspPid                      ((REG_Pid*)(REG_PIDFLT_L_BASE))
#define _TspPid_H                    ((REG_Pid*)(REG_PIDFLT_H_BASE))

#define _TspPid_Ext                  ((REG_Pid*)(REG_PIDFLT_L_EXT_BASE))
#define _TspPid_Ext_H                ((REG_Pid*)(REG_PIDFLT_H_EXT_BASE))

#define _TspSec1                     ((REG_Sec*)(REG_SECFLT_BASE1))
#define _TspSec2                     ((REG_Sec*)(REG_SECFLT_BASE2))

#define TSP_HW_CFG_0   0x00UL
#define TSP_HW_CFG_1   0x01UL
#define TSP_HW_CFG_2   0x10UL
#define TSP_HW_CFG_3   0x11UL

/// TSP debug mode type
typedef enum
{
    TSP_DEBUG_MODE_DIS_CONT,         ///< Select dis-continue packet count mode
    TSP_DEBUG_MODE_DROP_CONT,        ///< Select drop packet count mode
} TSP_DEBUG_MODE;

typedef enum
{
    TSP_DEBUG_SRC_TS0,               ///< TSP input from TS0 interface
    TSP_DEBUG_SRC_TS1,               ///< TSP input from TS1 interface
    TSP_DEBUG_SRC_TS2,               ///< TSP input from TS2 interface
    TSP_DEBUG_SRC_FILE,              ///< TSP input from filein
} TSP_DEBUG_SRC;

typedef enum
{
    TSP_DEBUG_FIFO_VIDEO,            ///< TSP output to Video FIFO
    TSP_DEBUG_FIFO_AUDIO,            ///< TSP output to Audio FIFO
    TSP_DEBUG_FIFO_VIDEO3D,          ///< TSP output to Video3D FIFO
    TSP_DEBUG_FIFO_AUDIOB,           ///< TSP output to AudioB FIFO
    TSP_DEBUG_FIFO_AUDIOC,           ///< TSP output to AudioC FIFO
    TSP_DEBUG_FIFO_AUDIOD,           ///< TSP output to AudioD FIFO
} TSP_DEBUG_FIFO;

typedef enum
{
    TSP_DEBUG_PKT_DEMUX_0,
    TSP_DEBUG_PKT_DEMUX_0_FILE,
    TSP_DEBUG_PKT_DEMUX_1,
    TSP_DEBUG_PKT_DEMUX_2,
    TSP_DEBUG_MMFI0,
    TSP_DEBUG_MMFI1,
} TSP_DEBUG_FIFO_SRC;

typedef enum
{
    TSP_DEBUG_TSIF0,            ///< TSP output to Video FIFO
    TSP_DEBUG_TSIF1,            ///< TSP output to Audio FIFO
    TSP_DEBUG_TSIF2,
    TSP_DEBUG_TSIFFI,
} TSP_DEBUG_TSIF;

typedef enum
{
    TSP_DEBUG_CMD_NONE,              ///< TSP debug table cmd: do nothing
    TSP_DEBUG_CMD_CLEAR,             ///< TSP debug table cmd: clear
    TSP_DEBUG_CMD_ENABLE,            ///< TSP debug table cmd: enable
    TSP_DEBUG_CMD_DISABLE,           ///< TSP debug table cmd: disable
} TSP_DEBUG_CMD;

typedef enum
{
    TSP_CLR_SRC_PIDFLT_0    = 0x1,            ///< TSP debug table clear source: pidflt 0
    TSP_CLR_SRC_PIDFLT_FILE = 0x2,         ///< TSP debug table clear source: pidflt file
} TSP_DEBUG_CLR_SRC;

//----------------------------------
/// DMX debug table information structure
//----------------------------------
typedef struct
{
    TSP_DEBUG_CMD               TspCmd;
    TSP_DEBUG_SRC               TspSrc;
    TSP_DEBUG_FIFO              TspFifo;
} TSP_DisconPktCnt_Info, TSP_DropPktCnt_Info;

typedef struct
{
    TSP_DEBUG_CMD               TspCmd;
    TSP_DEBUG_TSIF              TspTsif;
} TSP_LockPktCnt_info;

typedef struct
{
    TSP_DEBUG_CMD               TspCmd;
    TSP_DEBUG_FIFO              TspFifo;
    TSP_DEBUG_FIFO_SRC          TspFifoSrc;
} TSP_AVPktCnt_info;

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------
#define HAS_FLAG(flag, bit)        ((flag) & (bit))
#define SET_FLAG(flag, bit)        ((flag)|= (bit))
#define RESET_FLAG(flag, bit)      ((flag)&= (~(bit)))
#define SET_FLAG1(flag, bit)       ((flag)|  (bit))
#define RESET_FLAG1(flag, bit)     ((flag)&  (~(bit)))

// define NULL function
#define HAL_TSP_SecFlt_SelSecBuf(u32EngId, u32SecFltId, u32BufId, bEnable)
#define HAL_TSP_SecFlt_SetEcmIdx(u32EngId, u32SecFltId, u32EcmIdx)
#define HAL_TSP_SecFlt_ResetEmmIdx(u32EngId, u32SecFltId)
#define HAL_TSP_SelAudOut(u32EngId)

//AV FIFO Enum
#define TSP_FIFO_AU                         0UL
#define TSP_FIFO_AUB                        1UL
#define TSP_FIFO_V3D                        2UL
#define TSP_FIFO_VD                         3UL
#define TSP_FIFO_AUC                        4UL
#define TSP_FIFO_AUD                        5UL

//TSP IF Source Enum
#define TSP_SRC_FROM_TSIF0_LIVE             0x00000001UL
#define TSP_SRC_FROM_TSIF0_FILE             0x00000002UL
#define TSP_SRC_FROM_TSIF1                  0x00000003UL
#define TSP_SRC_FROM_TSIF2                  0x00000004UL
#define TSP_SRC_FROM_MMFI0                  0x00000006UL
#define TSP_SRC_FROM_MMFI1                  0x00000007UL

//TSP Packet Demux Enum
#define TSP_PKTDMX_NONE                     0x00000000UL
#define TSP_PKTDMX0_LIVE                    0x00000002UL
#define TSP_PKTDMX0_FILE                    0x00000004UL
#define TSP_PKTDMX1                         0x00000008UL
#define TSP_PKTDMX2                         0x00000010UL


////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////

//enum of Capcbility item
#define HAL_TSP_CAP_PID_FILTER_NUM          0x00000000UL
#define HAL_TSP_CAP_SEC_FILTER_NUM          0x00000001UL
#define HAL_TSP_CAP_SEC_BUF_NUM             0x00000002UL
#define HAL_TSP_CAP_PVR_ENG_NUM             0x00000003UL
#define HAL_TSP_CAP_PVR_FILTER_NUM          0x00000004UL
#define HAL_TSP_CAP_PVR1_FILTER_NUM         0x00000005UL
#define HAL_TSP_CAP_MMFI0_FILTER_NUM        0x00000006UL
#define HAL_TSP_CAP_MMFI1_FILTER_NUM        0x00000007UL
#define HAL_TSP_CAP_TSIF_NUM                0x00000008UL
#define HAL_TSP_CAP_DEMOD_NUM               0x00000009UL
#define HAL_TSP_CAP_TS_PAD_NUM              0x0000000aUL
#define HAL_TSP_CAP_VQ_NUM                  0x0000000bUL
#define HAL_TSP_CAP_CA_FLT_NUM              0x0000000cUL
#define HAL_TSP_CAP_CA_KEY_NUM              0x0000000dUL
#define HAL_TSP_CAP_FW_ALIGN                0x0000000eUL
#define HAL_TSP_CAP_VQ_ALIGN                0x0000000fUL
#define HAL_TSP_CAP_VQ_PITCH                0x00000010UL
#define HAL_TSP_CAP_SEC_BUF_ALIGN           0x00000011UL
#define HAL_TSP_CAP_PVR_ALIGN               0x00000012UL
#define HAL_TSP_CAP_PVRCA_PATH_NUM          0x00000013UL
#define HAL_TSP_CAP_SHAREKEY_FLT_RANGE      0x00000014UL
#define HAL_TSP_CAP_CA0_FLT_RANGE           0x00000015UL
#define HAL_TSP_CAP_CA1_FLT_RANGE           0x00000016UL
#define HAL_TSP_CAP_CA2_FLT_RANGE           0x00000017UL
#define HAL_TSP_CAP_SHAREKEY_FLT1_RANGE     0x00000018UL
#define HAL_TSP_CAP_SHAREKEY_FLT2_RANGE     0x00000019UL
#define HAL_TSP_CAP_HW_TYPE                 0x0000001aUL
#define HAL_TSP_CAP_VFIFO_NUM               0x0000001cUL
#define HAL_TSP_CAP_AFIFO_NUM               0x0000001dUL
#define HAL_TSP_CAP_HWPCR_SUPPORT           0x0000001eUL
#define HAL_TSP_CAP_PCRFLT_START_IDX        0x0000001fUL
#define HAL_TSP_CAP_HWWP_SET_NUM            0x00000020UL
#define HAL_TSP_CAP_DSCMB_ENG_NUM           0x00000021UL
#define HAL_TSP_CAP_MERGESTR_NUM            0x00000022UL
#define HAL_TSP_CAP_MAX_SEC_FLT_DEPTH       0x00000023UL
#define HAL_TSP_CAP_FW_BUF_SIZE             0x00000024UL
#define HAL_TSP_CAP_FW_BUF_RANGE            0x00000025UL
#define HAL_TSP_CAP_VQ_BUF_RANGE            0x00000026UL
#define HAL_TSP_CAP_SEC_BUF_RANGE           0x00000027UL
#define HAL_TSP_CAP_FIQ_NUM                 0x00000028UL

// Output pad mode
#define HAL_TSP_OUTPAD_DMD                  0x00000000UL
#define HAL_TSP_OUTPAD_S2P                  0x10000000UL
#define HAL_TSP_OUTPAD_TSO                  0x20000000UL
#define HAL_TSP_OUTPAD_S2P1                 0x40000000UL
#define HAL_TSP_OUTPAD_TSO1                 0x80000000UL

//STC update Control Parameters define
#define HAL_TSP_STC_UPDATE_FW               0x00UL
#define HAL_TSP_STC_UPDATE_HK               0x01UL
#define HAL_TSP_STC_UPDATE_UPDATEONCE       0x02UL

//S2P Clock Option
#define HAL_S2P_CLK_OPT_NONE                0x00000000UL
#define HAL_S2P_CLK_OPT_INVERT              0x00000001UL
#define HAL_S2P_CLK_OPT_NON_INVERT          0x00000002UL

//[LEGACY] //[OBSOLETE]
extern MS_BOOL _bIsHK;
//[LEGACY] //[OBSOLETE]

MS_BOOL HAL_TSP_GetCap(MS_U32 u32Cap, void* pData);
MS_BOOL HAL_TSP_Alive(void);
void    HAL_TSP_HW_Lock_Init(void);
void    HAL_TSP_HW_Lock_Release(void);
void    HAL_TSP_SetPKTSize(MS_U32 u32PKTSize);
void    HAL_TSP_SetOwner(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_BOOL bOwner);
MS_BOOL HAL_TSP_TTX_IsAccess(MS_U32 u32Try);
void    HAL_TSP_TTX_UnlockAccess(void);

void    HAL_ResetAll(void);
void    HAL_TSP_PowerCtrl(MS_BOOL bOn);
void    HAL_TSP_SaveFltState(void);
void    HAL_TSP_RestoreFltState(void);
void    HAL_TSP_Enable_ValidSync_Dectect(void);
void    HAL_Reset_WB(void);
MS_BOOL HAL_TSP_WriteProtect_Enable(MS_BOOL bEnable, MS_PHY* pphyStartAddr, MS_PHY* pphyEndAddr);
void    HAL_TSP_OrzWriteProtect_Enable(MS_BOOL bEnable, MS_PHY phyStartAddr, MS_PHY phyEndAddr);

void    HAL_TSP_ORAcess_Optimize(MS_BOOL bEnable);
void    HAL_TSP_Reset(MS_U32 u32EngId);
void    HAL_TSP_SetBank(MS_VIRT virtBankAddr, MS_VIRT virtPMBankAddr);
void    HAL_TSP_WbDmaEnable(MS_BOOL bEnable);
void    HAL_TSP_HwPatch(void);
void    HAL_TSP_CPU_SetBase(MS_PHY phyAddr, MS_U32 u32Size);
void    HAL_TSP_SetCtrlMode(MS_U32 u32EngId, MS_U32 u32Mode, MS_U32 u32TsIfId);
MS_U32  HAL_TSP_GetCtrlMode(MS_U32 u32EngId);

void    HAL_TSP_Ind_Enable(void);
MS_U32  HAL_TSP_Scmb_Status(MS_U32 u32TSSrc, MS_U32 u32GroupId, MS_U32 u32PidFltId);

void    HAL_TSP_RemoveDupAVPkt(MS_BOOL bEnable);
void    HAL_TSP_RemoveDupAVFifoPkt(MS_U32 u32StreamId, MS_BOOL bEnable);
void    HAL_TSP_TEI_RemoveErrorPkt(MS_U32 u32PktType, MS_BOOL bEnable);
MS_BOOL HAL_TSP_DupPktCnt_Clear(void);
MS_BOOL HAL_TSP_AU_BD_Mode_Enable(MS_BOOL bEnable);

void    HAL_TSP_Int_ClearSw(void);
void    HAL_TSP_Int_ClearHw(MS_U32 u32Mask);
void    HAL_TSP_Int_ClearHw2(MS_U32 u32Mask);
void    HAL_TSP_Int_Enable(MS_U32 u32Mask);
void    HAL_TSP_Int_Disable(MS_U32 u32Mask);
void    HAL_TSP_Int2_Enable(MS_U32 u32Mask);
void    HAL_TSP_Int2_Disable(MS_U32 u32Mask);
MS_U32  HAL_TSP_HW_INT_STATUS(void);
MS_U32  HAL_TSP_HW_INT2_STATUS(void);
MS_U32  HAL_TSP_SW_INT_STATUS(void);

MS_BOOL HAL_TSP_TsOutPadCfg(MS_U32 u32OutPad, MS_U32 u32OutPadMode, MS_U32 u32InPad, MS_BOOL bInParallel);
void    HAL_TSP_SelPad(MS_U32 u32EngId, MS_U32 u32Flow, MS_U32 u32Pad, MS_BOOL bParl);
void    HAL_TSP_SelPad_ClkInv(MS_U32 u32EngId, MS_U32 u32Flow, MS_BOOL bClkInv);
void    HAL_TSP_SelPad_ExtSync(MS_U32 u32EngId, MS_BOOL bExtSync, MS_U32 u32Flow);
void    HAL_TSP_SelPad_Parl(MS_U32 u32EngId, MS_BOOL bParl, MS_U32 u32Flow);
MS_BOOL HAL_TSP_BlockTSOIn_En(MS_U32 u32EngId, MS_U32 u32TSIf, MS_BOOL bBlockMode);
MS_BOOL HAL_TSP_TsOuOutClockPhase(MS_U16 u16OutPad, MS_U16 u16Val, MS_BOOL bEnable, MS_U32 u32S2pOpt);
void    HAL_TSP_TSOut_En(MS_BOOL bEnable);
MS_BOOL HAL_TSP_GetTSIF_Status(MS_U8 u8TsIfId, MS_U16* pu16Pad, MS_U16* pu16Clk, MS_BOOL* pbExtSync, MS_BOOL* pbParl);
void    HAL_TSP_Parl_BitOrderSwap(MS_U32 u32EngId, MS_U32 u32Flow, MS_BOOL bInvert);
void    HAL_TSP_Flush_AV_FIFO(MS_U32 u32StreamId, MS_BOOL bFlush);
MS_U32  HAL_TSP_Get_AVFifoLevel(MS_U32 u32StreamId);
void    HAL_TSP_AVFIFO_Src_Select(MS_U32 u32Fifo, MS_U32 u32Src);
void    HAL_TSP_AVFIFO_Block_Disable(MS_U32 u32Fifo, MS_BOOL bDisable);
MS_BOOL HAL_TSP_TSIF_Enable(MS_U8 u8_tsif, MS_BOOL bEnable);
void    HAL_TSP_SelMatchPidSrc(MS_U32 u32Src);
MS_BOOL HAL_TSP_PidFlt_Src_Select(MS_U32 u32Src);


void    HAL_TSP_CmdQ_SetSTC(MS_U32 u32EngId, MS_U32 u32STC);
void    HAL_TSP_CmdQ_SetSTC_32(MS_U32 u32EngId, MS_U32 u32STC_32);
MS_U32  HAL_TSP_CmdQ_GetSTC(MS_U32 u32EngId);
MS_U32  HAL_TSP_CmdQ_GetSTC_32(MS_U32 u32EngId);
MS_U32  HAL_TSP_CmdQ_CmdCount(void);
MS_BOOL HAL_TSP_CmdQ_Reset(void);
void    HAL_TSP_CmdQ_TsDma_SetAddr(MS_PHY phyStreamAddr);
MS_BOOL HAL_TSP_CmdQ_TsDma_SetSize(MS_U32 u32StreamSize);
void    HAL_TSP_CmdQ_TsDma_Start(MS_U32 u32TsDmaCtrl);
MS_U32  HAL_TSP_CmdQ_TsDma_GetState(void);
MS_U32  HAL_TSP_CmdQ_EmptyCount(void);
void    HAL_TSP_CmdQ_TsDma_Reset(void);
MS_BOOL HAL_TSP_CmdQ_IsEmpty(void);
void    HAL_TSP_TsDma_SetDelay(MS_U32 u32Delay);
void    HAL_TSP_TsDma_Pause(void);
void    HAL_TSP_TsDma_Resume(void);
MS_U8   HAL_TSP_Get_CmdQFifoLevel(void);

void    HAL_TSP_FileIn_192BlockScheme_En(MS_BOOL bEnable);
void    HAL_TSP_filein_enable(MS_BOOL b_enable);
void    HAL_TSP_FileIn_Set(MS_BOOL bset);
void    HAL_TSP_ResetTimeStamp(void);
MS_U32  HAL_TSP_GetPlayBackTimeStamp(void);
void    HAL_TSP_SetPlayBackTimeStamp(MS_U32 u32Stamp);
void    HAL_TSP_SetPlayBackTimeStampClk(MS_U8 u8Id, MS_U32 u32ClkSrc);
MS_U32  HAL_TSP_GetFileInTimeStamp(void);
MS_BOOL HAL_TSP_GetFilinReadAddr(MS_PHY* pphyReadAddr);
void    HAL_TSP_SetDMABurstLen(MS_U32 u32Len);

void    HAL_TSP_STC64_Mode_En(MS_BOOL bEnable);  //T12 new
void    HAL_TSP_SetSTC(MS_U32 u32EngId, MS_U32 u32STC, MS_U32 u32STC_32);
void    HAL_TSP_Stc_ctrl(MS_U32 u32EngId, MS_U32 u32Sync);
MS_U32  HAL_TSP_GetSTCSynth(MS_U32 u32EngId);
void HAL_TSP_SetSTCSynth(MS_U32 u32EngId, MS_U32 u32Sync);
//void    HAL_TSP_SetSTC_32(MS_U32 u32EngId, MS_U32 u32STC_32);
void    HAL_TSP_STC_Update_Disable(MS_U32 u32EngId, MS_BOOL bDisable);
MS_U32  HAL_TSP_GetSTC(MS_U32 u32EngId);
MS_U32  HAL_TSP_GetSTC_32(MS_U32 u32EngId);
MS_BOOL HAL_TSP_SelectSTCEng(MS_U32 u32FltSrc, MS_U32 u32Eng);
MS_BOOL HAL_TSP_GetPcr(MS_U32 u32EngId, MS_U32 *pu32Pcr_32, MS_U32 *pu32Pcr);
MS_BOOL HAL_TSP_Check_FIFO_Overflow(MS_U32 u32StreamId);
void    HAL_TSP_HWPcr_SetSrcId(MS_U32 u32EngId, MS_U32 u32SrcId);
void    HAL_TSP_HWPcr_SelSrc(MS_U32 u32EngId, MS_U32 u32Src);
void    HAL_TSP_HWPcr_Reset(MS_U32 u32EngId, MS_BOOL bReset);
void    HAL_TSP_HWPcr_Int_Enable(MS_U32 u32EngId, MS_BOOL bEnable);
void    HAL_TSP_HWPcr_Read(MS_U32 u32EngId, MS_U32 *pu32Pcr, MS_U32 *pu32Pcr_32);

//MS_U32 HAL_TSP_PidFltId(MS_U32 u32PidType);
void    HAL_TSP_PidFlt_SetPid(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32PID);
void    HAL_TSP_PidFlt_SelFltOutput(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32FltOutput);
void    HAL_TSP_PidFlt_SelSecFlt(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32SecFltId);
MS_U32  HAL_TSP_PidFlt_GetSecFlt(MS_U32 u32EngId, MS_U32 u32PidFltId);
MS_U32  HAL_TSP_PidFlt_GetPid(MS_U32 u32EngId, MS_U32 u32PidFltId);
MS_U32  HAL_TSP_PidFlt_GetFltOutput(MS_U32 u32EngId, MS_U32 u32PidFltId);
void    HAL_TSP_PidFlt_SelFltSource(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32FltSource);
void    HAL_TSP_PidFlt_SetFltSrcStreamID(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_U32 u32SrcStrId);
void    HAL_TSP_PidFlt_SetFltRushPass(MS_U32 u32EngId, MS_U32 u32PidFltId, MS_BOOL bEnable);

void    HAL_TSP_PidFlt_SetHWPcrPid(MS_U32 u32EngId, MS_U32 u32PID);
MS_U32  HAL_TSP_PidFlt_GetHWPcrPid(MS_U32 u32EngId);
void    HAL_TSP_PidFlt_HWPcrFlt_Enable(MS_U32 u32EngId, MS_BOOL bEnable);

void    HAL_TSP_SecBuf_ResetBuffer(MS_U32 u32EngId, MS_U32 u32SecBufId);
void    HAL_TSP_SecFlt_ResetState(MS_U32 u32EngId, MS_U32 u32SecFltId);
void    HAL_TSP_SecFlt_SetType(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32FltType);
void    HAL_TSP_SecFlt_SetRmnCount(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32RmnCount);
void    HAL_TSP_SecFlt_ClrCtrl(MS_U32 u32EngId, MS_U32 u32SecFltId);
void    HAL_TSP_SecFlt_SetMask(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Mask);
void    HAL_TSP_SecFlt_SetNMask(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8NMask);
void    HAL_TSP_SecFlt_SetMatch(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U8 *pu8Match);
void    HAL_TSP_SecFlt_SetReqCount(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32ReqCount);
MS_U32  HAL_TSP_SecFlt_GetMode(MS_U32 u32EngId, MS_U32 u32SecFltId);
void    HAL_TSP_SecFlt_PcrReset(MS_U32 u32EngId, MS_U32 u32SecFltId);
void    HAL_TSP_SecFlt_VerReset(MS_U32 u32SecFltId);
void    HAL_TSP_SecFlt_SetDataAddr(MS_PHY phyDataAddr);

MS_PHY  HAL_TSP_SecBuf_GetBufRead(MS_U32 u32EngId, MS_U32 u32SecBufId);
MS_PHY  HAL_TSP_SecBuf_GetBufWrite(MS_U32 u32EngId, MS_U32 u32SecBufId);
void    HAL_TSP_SecBuf_SetBuffer(MS_U32 u32EngId, MS_U32 u32SecBufId, MS_PHY phyStartAddr, MS_U32 u32BufSize);
void    HAL_TSP_SecBuf_SetBufRead(MS_U32 u32EngId, MS_U32 u32SecBufId, MS_PHY phyReadAddr);
MS_PHY  HAL_TSP_SecBuf_GetBufStart(MS_U32 u32EngId, MS_U32 u32SecBufId);
MS_PHY  HAL_TSP_SecBuf_GetBufEnd(MS_U32 u32EngId, MS_U32 u32SecBufId);
MS_PHY  HAL_TSP_SecBuf_GetBufCur(MS_U32 u32EngId, MS_U32 u32SecBufId);
void    HAL_TSP_SecFlt_SetMode(MS_U32 u32EngId, MS_U32 u32SecFltId, MS_U32 u32SecFltMode);
MS_U32  HAL_TSP_SecFlt_GetCRC32(MS_U32 u32EngId, MS_U32 u32SecFltId);
MS_U32  HAL_TSP_SecFlt_GetState(MS_U32 u32EngId, MS_U32 u32SecFltId);
void    HAL_TSP_SecBuf_SetBufRead_tmp(MS_U32 u32EngId, MS_U32 u32SecBufId, MS_PHY phyReadAddr);
MS_PHY  HAL_TSP_SecBuf_GetBufWrite_tmp(MS_U32 u32EngId, MS_U32 u32SecBufId);

void    HAL_TSP_PVR_SetBuffer(MS_U8 u8PVRId, MS_PHY phyBufStart0, MS_PHY phyBufStart1, MS_U32 u32BufSize0, MS_U32 u32BufSize1);
void    HAL_TSP_PVR_Enable(MS_U8 u8PVRId, MS_BOOL bEnable);
void    HAL_TSP_PVR_Reset(MS_U8 u8PVRId);
void    HAL_TSP_PVR_All(MS_U8 u8PVRId, MS_BOOL bPvrAll, MS_BOOL bWithNull, MS_BOOL bOldMode);
MS_PHY  HAL_TSP_PVR_GetBufWrite(MS_U8 u8PVRId);
void    HAL_TSP_PVR_WaitFlush(MS_U8 u8PVRId);
void    HAL_TSP_PVR_BypassHeader_En(MS_U8 u8PVRId, MS_BOOL bBypassHD);
void    HAL_TSP_PVR_Src_Select(MS_U8 u8PVRId, MS_U32 u32Src);
MS_U32  HAL_TSP_PVR_StartingEngs_Get(MS_U32 u32PktDmxSrc);
MS_BOOL HAL_TSP_PVR_IsEnabled(MS_U32 u32EngId);
MS_BOOL HAL_TSP_PVR_Alignment_Enable(MS_U8 u8PVRId, MS_BOOL bEnable);
MS_BOOL HAL_TSP_PVR_ForceSync_Enable(MS_U8 u8PVRId, MS_BOOL bEnable);
void    HAL_TSP_PVR_PacketMode(MS_U8 u8PVRId, MS_BOOL bSet);
MS_BOOL HAL_TSP_PVR_Fifo_Block_Disable(MS_U8 u8PVRId, MS_BOOL bDisable);
MS_U32  HAL_TSP_GetPVRTimeStamp(MS_U8 u8PVRId);
void    HAL_TSP_SetPVRTimeStamp(MS_U8 u8PVRId, MS_U32 u32Stamp);
void    HAL_TSP_SetPVRTimeStampClk(MS_U8 u8PVRId, MS_U32 u32ClkSrc);
MS_BOOL HAL_TSP_Read_DropPktCnt(MS_U16* pu16ADropCnt, MS_U16* pu16VDropCnt);
void    HAL_TSP_TSIF0_Enable(MS_BOOL bEnable);
void    HAL_TSP_TSIF1_Enable(MS_BOOL bEnable);
void    HAL_TSP_TSIFFI_SrcSelect(MS_BOOL bFileMode);

MS_U32  HAL_REG32_IndR(REG32 *reg);
void    HAL_REG32_IndW(REG32 *reg, MS_U32 value);

// void HAL_TSP_SetFwMsg(MS_U32 u32Mode);
// MS_U32 HAL_TSP_GetFwMsg(void);
void    HAL_TSP_ISR_SAVE_ALL(void);
void    HAL_TSP_ISR_RESTORE_ALL(void);
void    HAL_TSP_PS_Path_Disable(void);
void    HAL_TSP_PS_Path_Enable(MS_U32 u32TsDmaCtrl);
void    HAL_TSP_CSA_Set_ScrmPath(MS_U8 u8EngId, MS_U32 u32ScrmPath);
MS_U32  HAL_TSP_CSA_Get_ScrmPath(MS_U8 u8EngId);
MS_BOOL HAL_TSP_CSA_Set_CACtrl(MS_U8 u8EngId, MS_U8 u8SrcTSIF, MS_U32 u32Dst);
MS_BOOL HAL_TSP_CSA_Get_CACtrl(MS_U8 u8EngId, MS_U8* pu8SrcTSIF, MS_U32* pu32Dst);

MS_U32  HAL_TSP_Get_FW_VER(void);
MS_BOOL HAL_TSP_Check_FW_VER(void);
MS_BOOL HAL_TSP_SetFwDbgMem(MS_PHY phyAddr, MS_U32 u32Size);
MS_BOOL HAL_TSP_SetFwDbgWord(MS_U32 u32Word);
MS_U32  HAL_TSP_GetDBGPortInfo(MS_U32 u32dbgsel);
MS_U8   HAL_TSP_Get_PesScmb_Sts(MS_U8 u8FltId);
MS_U8   HAL_TSP_Get_TsScmb_Sts(MS_U8 u8FltId);

//------- VQ Funcions -----------------------------------------------------------
void    HAL_TSP_SetVQBuffer(MS_U8 u8VQId, MS_PHY phyBaseAddr, MS_U32 u32BufLen);
void    HAL_TSP_VQueue_Enable(MS_BOOL bEnable);
void    HAL_TSP_VQueue_Reset(MS_U8 u8VQId);
void    HAL_TSP_VQueue_OverflowInt_En(MS_U8 u8VQId, MS_BOOL bEnable);
void    HAL_TSP_VQueue_Clr_OverflowInt(MS_U8 u8VQId);
void    HAL_TSP_Set_Req_VQ_RX_Threshold(MS_U8 u8req_len);
MS_U32  HAL_TSP_Get_VQStatus(void);
void    HAL_TSP_VQBlock_Disable(MS_U8 u8VQId, MS_BOOL bDisable);
//-----------------------------------------------------------------------------

//------------------ MOBF Functions ---------------
//--- decrypt address must be the same as encrypt address ------
void    HAL_TSP_MOBF_Select(MS_U8 u8Model, MS_U8 u8MobfIndex0, MS_U8 u8MobfIndex1);
//---------------------------------------------------------

// -------------------------------------------------------------
//  Common api for mode setting
//  u32Cmd[31]: 0 -> public cmd, 1 -> private cmd
// -------------------------------------------------------------
// HAL_CMD_ONEWAY                    : Oneway record enable, 1-> Rec scrmable stream disable, 2 -> PVR oneway, 4 -> LoadFW oneway
// -------------------------------------------------------------
MS_BOOL HAL_TSP_CMD_Run(MS_U32 u32Cmd, MS_U32 u32Config0, MS_U32 u32Config1, MS_U32* pData);
#define HAL_CMD_ONEWAY                   0x80000001UL //u32Config0: 1-> Rec SCM stream disable , 2->OnewayPVRPort, 4->OnewayFW; u32Config1,pData: Don't care
#define HAL_CMD_SET_KRSTR_MODE           0x80000002UL  //u32Config0: 0 -> Disable, 1 -> Suspend, 2 -> Resume
#define HAL_CMD_SET_LIB_MODE             0x80000003UL  //u32Config0: 0 ->  user mode, 1 ->  Kernel mode with user cb
#define HAL_CMD_PVR_PES_MODE             0x00000030UL  //u32Config0: PVR EngineID, u32Config1=1, *pData = 1 is enable, pData = 0 is disable

MS_BOOL HAL_TSP_STC_UpdateCtrl(MS_U8 u8Eng, MS_U8 u8Opt);
MS_BOOL HAL_TSP_SetSTCOffset(MS_U32 u32EngId, MS_U32 u32Offset, MS_BOOL bAdd);


MS_BOOL HAL_FQ_SetMuxSwitch(MS_U32 u32FQEng, MS_U32 u32FQSrc);
MS_U32 HAL_FQ_GetMuxSwitch(MS_U32 u32FQEng);

// -------------------------------------------------------------
// Debug table
// -------------------------------------------------------------
MS_U32 HAL_TSP_Get_DisContiCnt(TSP_DisconPktCnt_Info* TspDisconPktCntInfo);
MS_U32 HAL_TSP_Get_DropPktCnt(TSP_DropPktCnt_Info* TspDropCntInfo);
MS_U32 HAL_TSP_Get_LockPktCnt(TSP_LockPktCnt_info* TspLockCntInfo);
MS_U32 HAL_TSP_GetAVPktCnt(TSP_AVPktCnt_info* TspAVCntInfo);

MS_U32  HAL_TSP_Get_SecTEI_PktCount(MS_U32 u32PktSrc);
MS_BOOL HAL_TSP_Reset_SecTEI_PktCount(MS_U32 u32PktSrc);
MS_U32  HAL_TSP_Get_SecDisCont_PktCount(MS_U32 u32FltId);
MS_BOOL HAL_TSP_Reset_SecDisCont_PktCount(MS_U32 u32FltId);

// -------------------------------------------------------------
// Merge Stream
// -------------------------------------------------------------
MS_BOOL HAL_TSP_Set_Sync_Byte(MS_U8 u8Path, MS_U8 u8Id, MS_U8 *pu8SyncByte, MS_BOOL bSet);
MS_BOOL HAL_TSP_Set_Src_Id(MS_U8 u8Path, MS_U8 u8Id, MS_U8 *pu8SrcId, MS_BOOL bSet);

// ATS Calibration API
MS_BOOL HAL_TSP_Set_ATS_AdjPeriod(MS_U16 u16Value);
MS_BOOL HAL_TSP_Set_ATS_AdjEnable(MS_BOOL bEnable);
MS_BOOL HAL_TSP_Set_ATS_AdjOffset(MS_BOOL bIncreased, MS_U16 u16Offset);
MS_BOOL HAL_TSP_DropScmbPkt(MS_U32 u32StreamId,MS_BOOL bEnable);

#ifdef MSOS_TYPE_LINUX_KERNEL
MS_BOOL HAL_TSP_SaveRegs(void);
MS_BOOL HAL_TSP_RestoreRegs(void);
#endif  //MSOS_TYPE_LINUX_KERNEL


#endif // #ifndef __HAL_TSP_H__
