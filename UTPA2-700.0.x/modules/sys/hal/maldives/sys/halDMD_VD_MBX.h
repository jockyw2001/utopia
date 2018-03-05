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

#ifndef _HAL_SYS_DMD_VD_MBX_H_
#define _HAL_SYS_DMD_VD_MBX_H_


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DMD_VD_MBX_CHIP_U3          0
#define DMD_VD_MBX_CHIP_T3          1
#define DMD_VD_MBX_CHIP_T7_T8_T9    2
#define DMD_VD_MBX_CHIP_T12_T13     3
#define DMD_VD_MBX_CHIP_VERSION     DMD_VD_MBX_CHIP_T12_T13

#if (DMD_VD_MBX_CHIP_VERSION >= DMD_VD_MBX_CHIP_T7_T8_T9)
#define MBRegBase                   0x012600
#define DMDMcuBase                  0x003480
#elif (DMD_VD_MBX_CHIP_VERSION == DMD_VD_MBX_CHIP_T3)
#define MBRegBase                   0x010500
#define DMDMcuBase                  0x003460
#else
#define MBRegBase                   0x0
#define DMDMcuBase                  0x0
#endif

#if (DMD_VD_MBX_CHIP_VERSION == DMD_VD_MBX_CHIP_T3)
#define DemodCmdWrReg               0x01     // write register.
#define DemodCmdRdReg               0x02     // read register.
#define DemodCmd                    0x010500L//0x110500L
#define DemodAdrL                   0x010501L//0x110501L
#define DemodAdrH                   0x010502L//0x110502L
#define DemodData                   0x010503L//0x110503L
#endif

#define BIN_FOR_ATV                 BIT_(0)
#define BIN_FOR_DVBT                BIT_(1)
#define BIN_FOR_DVBC                BIT_(2)
#define BIN_FOR_ATSC                BIT_(3)
#define BIN_FOR_ISDBT               BIT_(4)
#define BIN_FOR_DTMB                BIT_(5)
#define BIN_FOR_DVBS        BIT_(6)
#define BIN_FOR_DVBT2				BIT_(7)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_SYS_DMD_VD_MBX_Init(void);
MS_U8 HAL_SYS_DMD_VD_MBX_GetType(void);
void HAL_SYS_DMD_VD_MBX_SetType(MS_U8 u8Value);
// ATV
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WaitReady(void);
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_ReadByte(MS_U32 u32Reg, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WriteByte(MS_U32 u32Reg, MS_U8 u8Val);
// DVBT & DVBC
void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadByte(MS_U16 u16Addr, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteByte(MS_U32 u16Addr, MS_U8 u8Data);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadDspReg(MS_U32 u16Addr, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteDspReg(MS_U32 u16Addr, MS_U8 u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_ReadReg(MS_U16 u16Addr, MS_U8 *u8Value);
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_WriteReg(MS_U32 u16Addr, MS_U8 u8Data);
// ATSC
void HAL_SYS_DMD_VD_MBX_ATSC_WriteByte(MS_U16 u16Addr, MS_U8 u8Data);
MS_U8 HAL_SYS_DMD_VD_MBX_ATSC_ReadByte(MS_U16 u16Addr);

#endif // _HAL_SYS_DMD_VD_MBX_H_

