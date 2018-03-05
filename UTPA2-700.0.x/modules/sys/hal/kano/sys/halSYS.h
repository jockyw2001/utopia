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

#ifndef _HAL_SYS_H_
#define _HAL_SYS_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define UNUSED( var )       (void)((var) = (var))

#define CHIPREV_U01     0x00UL
#define CHIPREV_U02     0x01UL

#define EFUSE_ADDRESS_MASK            0x007FUL
#define EFUSE_OFFSET_HDCP_FSM0        0x0010UL
#define EFUSE_OFFSET_HDCP_FSM1        0x0038UL
#define EFUSE_OFFSET_HASH_FSM0        0x0002UL // bank 2

//efuse FSM0
#define REG_EFUSE_FSM0_TRIG           0x2000UL
#define REG_EFUSE_FSM0_CTRL           0x2002UL
#define REG_EFUSE_FSM0_RDATA_15_0     0x2060UL
#define REG_EFUSE_FSM0_RDATA_31_16    0x2062UL
#define REG_EFUSE_FSM0_RDATA_47_32    0x2064UL
#define REG_EFUSE_FSM0_RDATA_63_48    0x2066UL

//efuse FSM1
#define REG_EFUSE_FSM1_TRIG           0x204CUL
#define REG_EFUSE_FSM1_CTRL           0x204EUL
#define REG_EFUSE_FSM1_RDATA_15_0     0x2080UL
#define REG_EFUSE_FSM1_RDATA_31_16    0x2082UL
#define REG_EFUSE_FSM1_RDATA_47_32    0x2084UL
#define REG_EFUSE_FSM1_RDATA_63_48    0x2086UL

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32  u32ChipTopBase;
    MS_U32  u32PmTopBase;      // REG_PM_BASE

} hal_sys_base_t;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void            SYS_Init(MS_PHY phy64baseaddr);
MS_U8           SYS_GetChipRev(void);
MS_U16          SYS_GetChipID(void);
const SYS_Info* SYS_GetInfo(void);
void            SYS_ResetCPU(void);
MS_U8           SYS_VIF_ReadByteByVDMbox(MS_U32 u32Reg);
void            SYS_VIF_WriteByteByVDMbox(MS_U32 u32Reg, MS_U8 u8Val);
void            SYS_VIF_WriteByteMaskByVDMbox(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Mask);
void            SYS_VIF_WriteRegBitByVDMbox(MS_U32 u32Reg, MS_U8 bEnable, MS_U8 u8Mask);
MS_U16          SYS_VIF_Read2ByteByVDMbox(MS_U32 u32Reg);
void            HAL_SYS_RFAGC_Tristate(MS_BOOL bEnable);
void            HAL_SYS_IFAGC_Tristate(MS_BOOL bEnable);
void            HAL_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux);
MS_BOOL         HAL_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel);
MS_BOOL         HAL_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode);
MS_BOOL         HAL_SYS_PadMuxTableSuspend(void);
MS_BOOL         HAL_SYS_PadMuxTableResume(void);
MS_BOOL         HAL_SYS_DisableDebugPort(void);
MS_BOOL         HAL_SYS_EnableDebugPort(void);
MS_BOOL         HAL_SYS_SetTSOutClockPhase(MS_U16 u16Val);
MS_BOOL         HAL_SYS_SetTSClockPhase(SYS_PAD_MUX_SET ePadMuxType,MS_U16 u16Val);

void            HAL_SYS_SetEfuseIOMapBase(MS_VIRT virtBase);
MS_U16          HAL_SYS_EfuseRead2Byte(MS_U32 u32RegAddr);
MS_U16          HAL_SYS_EfuseWrite2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
void            HAL_SYS_SetOtpIOMapBase(MS_VIRT u32Base);
MS_U32          HAL_SYS_ReadRSAKey(MS_U16 u16ReadAddr);
MS_BOOL         HAL_SYS_Query(E_SYS_QUERY id);
MS_U32          HAL_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index);
E_SYS_CHIP_TYPE HAL_SYS_GetChipType(void);
void HAL_SYS_SetChipType(E_SYS_CHIP_TYPE Type);
MS_U16          HAL_SYS_ReadBrickTerminatorStatus(void);
void            HAL_SYS_WriteBrickTerminatorStatus(MS_U16 u16Status);
void            HAL_SYS_GetEfuseDid(MS_U16 *u16efuse_did);
MS_BOOL         HAL_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey);
MS_U32          HAL_SYS_GetMemcConfg(MS_U32 eSource, MS_U32 eTiming, MS_BOOL *retEnMemc);
MS_U32          HAL_SYS_GetXcByPartConfg (MS_U32 eSource, MS_U32 eInputTiming, MS_U32 eOutputTiming, MS_BOOL *retEn);
void            HAL_SYS_EnableWkEventWOL(void);
void            HAL_SYS_DisableWkEventWOL(void);
void            HAL_SYS_ResetStatusWOL(void);
MS_BOOL         HAL_SYS_GetStatusWOL(void);

#endif // _HAL_SYS_H_
