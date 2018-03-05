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
#ifndef __HAL_TSIO_H__
#define __HAL_TSIO_H__

#include "regTSIO.h"

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Macro of bit operations
//--------------------------------------------------------------------------------------------------

#define ANALOG_PHASE_MAX 39
#define ANALOG_PHASE_MIN 0
#define ANALOG_PHASE_NUM (ANALOG_PHASE_MAX-ANALOG_PHASE_MIN+1)
#define ANALOG_PHASE_GAP 7
#define ANALOG_PHASE_TABLE 4 //0: 200M, 1: 100M, 2:50M, 3: 25M

typedef enum
{
    HAL_SGDMAOUT_DBG_INT_COM = 0,
    HAL_SGDMAOUT_DBG_STATE = 1,
    HAL_SGDMAOUT_DBG_CURRENT_VC_ID = 2,
    HAL_SGDMAOUT_DBG_BUF_BASE_ADDR_L = 3,
    HAL_SGDMAOUT_DBG_BUF_BASE_ADDR_H = 4,
    HAL_SGDMAOUT_DBG_BUF_SIZE_L = 5,
    HAL_SGDMAOUT_DBG_BUF_SIZE_H = 6,
    HAL_SGDMAOUT_DBG_BUF_USED_SIZE_L = 7,
    HAL_SGDMAOUT_DBG_BUF_USED_SIZE_H = 8,
    HAL_SGDMAOUT_DBG_CPU_FLAG = 9,
    HAL_SGDMAOUT_DBG_NODE_FLAG = 10,
    HAL_SGDMAOUT_DBG_SG_FLAG = 11,
    HAL_SGDMAOUT_DBG_BUF_EMPTY = 12,
    HAL_SGDMAOUT_DBG_BUF_LEFT_SIZE_L = 13,
    HAL_SGDMAOUT_DBG_BUF_LEFT_SIZE_H = 14,
    HAL_SGDMAOUT_DBG_SGDMA_OUT_VC_INT_MASK_0 = 15,
    HAL_SGDMAOUT_DBG_SGDMA_OUT_VC_INT_MASK_1 = 16,
    HAL_SGDMAOUT_DBG_SGDMA_OUT_VC_INT_MASK_2 = 17,
    HAL_SGDMAOUT_DBG_SGDMA_OUT_VC_INT_MASK_3 = 18,
    HAL_SGDMAOUT_DBG_PACE_DBG_CLK_NUM_H = 19,
    HAL_SGDMAOUT_DBG_PACE_DBG_CLK_NUM_L = 20,
    HAL_SGDMAOUT_DBG_PACE_DBG_PKT_NUM = 21,
    HAL_SGDMAOUT_DBG_MAX,
}HAL_SGDMAOUT_DBG;

typedef enum
{
    HAL_SGDMAIN_DBG_VC_ACTIVE_0 = 0,
    HAL_SGDMAIN_DBG_VC_ACTIVE_1 = 1,
    HAL_SGDMAIN_DBG_VC_ACTIVE_2 = 2,
    HAL_SGDMAIN_DBG_VC_ACTIVE_3 = 3,
    HAL_SGDMAIN_DBG_VC_DONE_0 = 4,
    HAL_SGDMAIN_DBG_VC_DONE_1 = 5,
    HAL_SGDMAIN_DBG_VC_DONE_2 = 6,
    HAL_SGDMAIN_DBG_VC_DONE_3 = 7,
    HAL_SGDMAIN_DBG_M_PRIOR_0 = 8,
    HAL_SGDMAIN_DBG_M_PRIOR_1 = 9,
    HAL_SGDMAIN_DBG_M_PRIOR_2 = 10,
    HAL_SGDMAIN_DBG_M_PRIOR_3 = 11,
    HAL_SGDMAIN_DBG_L_PRIOR_0 = 12,
    HAL_SGDMAIN_DBG_L_PRIOR_1 = 13,
    HAL_SGDMAIN_DBG_L_PRIOR_2 = 14,
    HAL_SGDMAIN_DBG_L_PRIOR_3 = 15,
    HAL_SGDMAIN_DBG_CURRENT_STATUS = 16,
    HAL_SGDMAIN_DBG_FIRST_NODE_ADDR_L = 17,
    HAL_SGDMAIN_DBG_FIRST_NODE_ADDR_H = 18,
    HAL_SGDMAIN_DBG_NEXT_NODE_ADDR_L = 19,
    HAL_SGDMAIN_DBG_NEXT_NODE_ADDR_H = 20,
    HAL_SGDMAIN_DBG_BUF_BASE_ADDR_L = 21,
    HAL_SGDMAIN_DBG_BUF_BASE_ADDR_H = 22,
    HAL_SGDMAIN_DBG_BUF_SIZE_L = 23,
    HAL_SGDMAIN_DBG_BUF_SIZE_H = 24,
    HAL_SGDMAIN_DBG_BUF_USED_SIZE_L = 25,
    HAL_SGDMAIN_DBG_BUF_USED_SIZE_H = 26,
    HAL_SGDMAIN_DBG_CPU_FLAG = 27,
    HAL_SGDMAIN_DBG_NODE_FLAG = 28,
    HAL_SGDMAIN_DBG_SG_FLAG = 29,
    HAL_SGDMAIN_DBG_SG_CTRL = 30,
    HAL_SGDMAIN_DBG_BUF_LEFT_SIZE_L = 31,
    HAL_SGDMAIN_DBG_BUF_LEFT_SIZE_H = 32,
    HAL_SGDMAIN_DBG_EMPTY = 33,
    HAL_SGDMAIN_DBG_STATE = 34,
    HAL_SGDMAIN_DBG_SGDMA_IN_VC_INT_MASK_0 = 35,
    HAL_SGDMAIN_DBG_SGDMA_IN_VC_INT_MASK_1 = 36,
    HAL_SGDMAIN_DBG_SGDMA_IN_VC_INT_MASK_2 = 37,
    HAL_SGDMAIN_DBG_SGDMA_IN_VC_INT_MASK_3 = 38,
    HAL_SGDMAIN_DBG_VC_TOUCHED_0 = 39,
    HAL_SGDMAIN_DBG_VC_TOUCHED_1 = 40,
    HAL_SGDMAIN_DBG_VC_TOUCHED_2 = 41,
    HAL_SGDMAIN_DBG_VC_TOUCHED_3 = 42,
    HAL_SGDMAIN_DBG_PACE_DBG_CLK_NUM_H = 43,
    HAL_SGDMAIN_DBG_PACE_DBG_CLK_NUM_L = 44,
    HAL_SGDMAIN_DBG_PACE_DBG_PKT_NUM = 45,
    HAL_SGDMAIN_DBG_MAX,
}HAL_SGDMAIN_DBG;

typedef enum
{
    HAL_SGDMA_SRAM_BUF_USED_SIZE = 0,
    HAL_SGDMA_SRAM_FLAG = 1,
    HAL_SGDMA_SRAM_BUF_SIZE = 2,
    HAL_SGDMA_SRAM_BUF_ADDR = 3,
    HAL_SGDMA_SRAM_NEXT_NODE_ADDR = 4,
    HAL_SGDMA_SRAM_FIRST_NODE_ADDR = 5,
    HAL_SGDMA_SRAM_CPU_FLAG = 6,
    HAL_SGDMA_SRAM_MAX,
}HAL_SGDMA_SRAM;

typedef enum
{
    HAL_TSIO_CLK_200M = 0,
    HAL_TSIO_CLK_100M = 1,
    HAL_TSIO_CLK_50M = 2,
    HAL_TSIO_CLK_25M = 3,
}HAL_TSIO_CLK;

typedef enum
{
    E_HAL_TSIO_DBG_LEVEL_NONE,   // no debug message shown
    E_HAL_TSIO_DBG_LEVEL_ERR,    // only shows error message that can't be recover
    E_HAL_TSIO_DBG_LEVEL_WARN,   // error case can be recover, like retry
    E_HAL_TSIO_DBG_LEVEL_EVENT,  // event that is okay but better known, ex: timestamp ring, file circular, etc.
    E_HAL_TSIO_DBG_LEVEL_INFO,   // information for internal parameter
    E_HAL_TSIO_DBG_LEVEL_FUNC,   // Function trace and input parameter trace
    E_HAL_TSIO_DBG_LEVEL_TRACE,  // debug trace
} EN_HAL_TSIO_DBGMSG_LEVEL;

////////////////////////////////////////////////
// HAL API
////////////////////////////////////////////////


//=======================================================
// General API
//=======================================================
void HAL_TSIO_ClkOpen(MS_BOOL bEnable);
void HAL_TSIO_SetBank(MS_VIRT u32BankAddr);
void HAL_TSIO_PowerInit(void);
void HAL_TSIO_Reset(void);
void HAL_TSIO_Tx2Rx_InsertCTS(MS_BOOL bEnable);
void HAL_TSIO_Tx2Rx_Direct8(MS_BOOL bEnable);
void HAL_TSIO_Tx2Rx_Direct16(MS_BOOL bEnable);
void HAL_TSIO_Tx2Rx_Loopback(MS_BOOL bEnable);
void HAL_TSIO_StuffingSID_Set(MS_U8 u8Stuffing_sid);
void HAL_TSIO_Decrypt_Set(MS_BOOL bEnable);
void HAL_TSIO_Threshold_Set(MS_U8 u8Threshold);
void HAL_TSIO_BypassMerge_Set(MS_BOOL bEnable);
void HAL_TSIO_SIDBypassSMCMergeEnable(MS_U8 u8Sid, MS_BOOL bEnable);
void HAL_TSIO_CTS_En(MS_BOOL bEnable);
void HAL_TSIO_DbgLevel(EN_HAL_TSIO_DBGMSG_LEVEL eLevel);

//=======================================================
// SGDMA_IN API
//=======================================================
void HAL_TSIO_SGDMAIN_Reset(void);
void HAL_TSIO_SGDMAIN_LastPktBuf_Set(MS_U32 u32Head, MS_U32 u32Tail);
void HAL_TSIO_SGDMAIN_SGVQBuf_Set(MS_U32 u32BufAddr, MS_U32 u32BufSize);
void HAL_TSIO_SGDMAIN_VC_Open(MS_U32 u32Addr, MS_U8 u8Chid, MS_U8 u8Sid, MS_BOOL bTSmode, MS_BOOL bPriority, MS_BOOL bFlush, MS_BOOL bNewPkt);
void HAL_TSIO_SGDMAIN_VC_Close(MS_U8 u8Chid);
void HAL_TSIO_SGDMAIN_Global_Pacing(void);
void HAL_TSIO_SGDMAIN_VC_Pacing(MS_U8 u8Chid, MS_BOOL bTSmode, MS_U32 Pacing);
void HAL_TSIO_SGDMAIN_Start(void);
void HAL_TSIO_SGDMAIN_IntRead(MS_U8 u8Chid, MS_U16 *u16Status);
void HAL_TSIO_SGDMAIN_IntClr(MS_U8 u8Chid);
MS_U64 HAL_TSIO_SGDMAIN_IntAll(void);
void HAL_TSIO_SGDMAIN_IntClrAll(void);
void HAL_TSIO_SGDMAIN_PidFltEnable(MS_U8 u8Sid, MS_BOOL bEnable);
void HAL_TSIO_SGDMAIN_Flt_SetPid(MS_U16 u16FltId, MS_U16 u16PID);
void HAL_TSIO_SGDMAIN_Flt_SetSid(MS_U16 u16FltId, MS_U8 u8Sid);
MS_U32 HAL_TSIO_SGDMAIN_ReadSram(MS_U8 u8Chid, HAL_SGDMA_SRAM eSram);


//=======================================================
// SGDMA_OUT API
//=======================================================
void HAL_TSIO_SGDMAOUT_Init(void);
void HAL_TSIO_SGDMAOUT_VC_Open(MS_U32 u32Addr, MS_U8 u8Chid, MS_BOOL bSecurity);
void HAL_TSIO_SGDMAOUT_VC_Close(MS_U8 u8Chid);
void HAL_TSIO_SGDMAOUT_Start(void);
void HAL_TSIO_SGDMAOUT_IntRead(MS_U8 u8Chid, MS_U16 *u16Status);
void HAL_TSIO_SGDMAOUT_IntClr(MS_U8 u8Chid);
MS_U64 HAL_TSIO_SGDMAOUT_IntAll(void);
void HAL_TSIO_SGDMAOUT_IntClrAll(void);
MS_U32 HAL_TSIO_SGDMAOUT_ReadSram(MS_U8 u8Chid, HAL_SGDMA_SRAM eSram, MS_BOOL bLSB);

//=======================================================
// SVID table API
//=======================================================
void HAL_TSIO_SVIDTBL_Set(MS_U8 u8Tblid, MS_U8 u8Sid, MS_U8 u8Sourceid, MS_BOOL bLive);

//=======================================================
// CC API
//=======================================================
void HAL_TSIO_CC_RxClr(void);
void HAL_TSIO_CC_RxIntClr(void);
void HAL_TSIO_CC_RxEnable(MS_BOOL bEnable);
MS_BOOL HAL_TSIO_CC_RxInt(void);
MS_U16 HAL_TSIO_CC_RxErrStatus(void);
MS_U16 HAL_TSIO_CC_RxReadSize(void);
MS_U32 HAL_TSIO_CC_RxReadData(MS_U8 u8Addr);
MS_U32 HAL_TSIO_CC_RxReadData_Fast(MS_U8 u8Addr);
void HAL_TSIO_CC_RxReadData_FastEnable(MS_BOOL bEnable);
void HAL_TSIO_CC_TxClr(void);
void HAL_TSIO_CC_TxIntClr(void);
void HAL_TSIO_CC_TxEnable(MS_BOOL bEnable);
void HAL_TSIO_CC_TxWriteData(MS_U8 u8Addr, MS_U32 u32Data);
void HAL_TSIO_CC_TxWriteSize(MS_U16 u16Size);
MS_BOOL HAL_TSIO_CC_TxInt(void);
void HAL_TSIO_CC_TxWriteData_Fast(MS_U32 u32Data);
void HAL_TSIO_CC_TxWriteData_FastEnable(MS_BOOL bEnable);

//=======================================================
// Loc Decrypt API
//=======================================================
void HAL_TSIO_LOCDEC_Xiu_Lock(MS_U8 u8Xiu);
void HAL_TSIO_LOCDEC_Xiu_Unlock(void);
void HAL_TSIO_LOCDEC_ScbFixRule(MS_BOOL bEnable);
MS_BOOL HAL_TSIO_LOCDEC_Key_Set( MS_U8 u8Sid, MS_U8 *u8Key, MS_U16 u16KeySize, MS_BOOL bOddKey);
MS_BOOL HAL_TSIO_LOCDEC_KeySetDone(void);
void HAL_TSIO_LOCDEC_KeyEnable(MS_U8 u8Sid, MS_BOOL bEnable);
void HAL_TSIO_LOCDEC_PrivilegeSlotEnable(MS_U8 u8Sid, MS_BOOL bEnable);
void HAL_TSIO_LOCDEC_SecureSlotEnable(MS_U8 u8Sid, MS_BOOL bEnable);
void HAL_TSIO_LOCDEC_SetSWKeyProperty(MS_U32 keyProperty);

//=======================================================
// Get Infomation API
//=======================================================
MS_U16 HAL_TSIO_GetLoselockcnt(void);

//=======================================================
// Bit training API
//=======================================================
void HAL_TSIO_Bittraining_init(void);
void HAL_TSIO_Bittraining_init_dynamic(void);
MS_BOOL HAL_TSIO_Bittraining_Getremapstate(void);
void HAL_TSIO_Bittraining_Setvalue(MS_BOOL bEarly, MS_BOOL bLate);
MS_U32 HAL_TSIO_DBG_Read(MS_U16 u16Dbg);
MS_U8 HAL_TSIO_Bittraining_GetPhase(MS_U8 u8Phase);
void HAL_TSIO_Bittraining_GetElinfo(MS_BOOL bEarly);
MS_U16 HAL_TSIO_Bittraining_CheckPrbsErrcnt(void);
void HAL_TSIO_Bittraining_GetPhaseinfo(void);

//=======================================================
// Interrupt API
//=======================================================
MS_BOOL HAL_TSIO_Int_Operation_Read(void);
void HAL_TSIO_Int_Operation_Enable(MS_BOOL bEnable);
void HAL_TSIO_Int_Operation_Clear(void);
void HAL_TSIO_Int_Bittraining_Enable(MS_BOOL bEnable);
void HAL_TSIO_Int_Bittraining_Clear(void);
MS_BOOL HAL_TSIO_Int_Bittraining_Read(void);

//=======================================================
// Analog phase API
//=======================================================
void HAL_TSIO_Analogphase_Pllreset(void);
void HAL_TSIO_Analogphase_Init(HAL_TSIO_CLK eClk, MS_BOOL bLoopback);
void HAL_TSIO_Analogphase_Set(MS_BOOL bSelect, MS_U8 u8Phase, MS_U8 u8Gap);
MS_U16 HAL_TSIO_Analogphase_Read(MS_BOOL bEarly);
void HAL_TSIO_Analogphase_Start(void);
MS_U16 HAL_TSIO_Analogphase_GetPuhstatus(void);
void HAL_TSIO_Analogphase_Exit(void);

//=======================================================
// TSIO Filter API (use for bypass smc)
//=======================================================
void HAL_TSIO_Filter_Enable(MS_U16 u16FltId, MS_BOOL bEnable);
void HAL_TSIO_Filter_SetPid(MS_U16 u16FltId, MS_U16 u16Pid);
void HAL_TSIO_Filter_SetSid(MS_U16 u16FltId, MS_U8 u8Sid);

//=======================================================
// Other API
//=======================================================
void HAL_TSIO_RxRecord_Start(MS_BOOL bEnable);
void HAL_TSIO_RxRecord_Setbuf(MS_U32 u32Headaddr, MS_U32 u32Tailaddr);
void HAL_TSIO_TSP_BLK_Set(MS_BOOL bEnable);
void HAL_TSIO_STRLD_SET(MS_U8 u8index);

#endif // #ifndef __HAL_TSIO_H__

