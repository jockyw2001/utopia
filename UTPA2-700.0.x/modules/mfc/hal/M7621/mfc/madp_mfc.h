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
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MADP_MFC_H_
#define _MADP_MFC_H_

//#include "mdrv_types.h"
//#include "mdrv_mfc_io.h"
//#include "mdrv_mfc_st.h"
#include "mdrv_mfc.h"

//------------------------------------------------------------------------------
//  Global veriable
//------------------------------------------------------------------------------
// extern MST_MFC_SYS_INFO_t *mfc_sysinfo;

//------------------------------------------------------------------------------
//  Structure
//------------------------------------------------------------------------------
/*
typedef enum
{
    MFC_I2cUseBusN       = 0x34,
    MFC_I2cUseBus        = 0x35,
    MFC_CpuWaitN         = 0x36,
    MFC_CpuWait          = 0x37,
    MFC_SerialDebugEnter = 0x44,
    MFC_SerialDebugExit  = 0x45,
} MFC_SERIAL_DEBUG_CMD_e;
*/
//------------------------------------------------------------------------------
//  Functions prototype
//------------------------------------------------------------------------------
void MAdp_MFC_SpiLoadCode(void);
void MAdp_MFC_Init(PMST_MFC_SYS_INFO_t mfcSysInfo, U8 u8IsS7M);
void MAdp_MFC_OnOff(BOOL bOnOff);
void MAdp_MFC_Compensation(U8 u8Blur, U8 u8Judder, U8 u8MFC);
void MAdp_MFC_True_Motion_Demo(U8 u8TrueMotionDemo);
void MAdp_MFC_Video_Block(U8 u8Ctrl, BOOL bCtrl);
void MAdp_MFC_LVDSOnOff(BOOL bOnOff);
void MAdp_MFC_BypassWindow(BOOL bOnOff, U8 u8WinId, U16 u16Hstart, U16 u16Vstart, U16 u16Hsize, U16 u16Vsize);
U16 MAdp_MFC_GetFirmwareVersion(void);
U16 MAdp_MFC_GetFirmwareVersionFromBin(void);
void MAdp_MFC_SetSpreadSpectrum(U16 u16KHz, U16 u16Percent, BOOL bEnable, BOOL bMiuLVDS);
void MAdp_MFC_OnOffDebugBlock(BOOL bOnOff);
void MAdp_MFC_DemoBarControl(BOOL bOnOFF, BOOL bDirection, U8 u8Width, U8 u8Color);
U8 MAdp_MFC_GetModelType(void);
void MAdp_MFC_ReadHVTotal(U16* pHTotal, U16* pVTotal);
U8 MAdp_MFC_IsStable(void);
void MAdp_MFC_SetLVDSVesaJeida(BOOL bVesaJeida);
void MAdp_MFC_SetFrameRate(U8 u8frameRate);
void MAdp_MFC_SetSlowFrameLock(BOOL bEnable);
void MAdp_MFC_ControlFrameLockMode(U8 u8type);
void MAdp_MFC_RESET(void);
void MAdp_MFC_UpdateSW(void);
void MAdp_MFC_SetReverseControl(U8 u8Mode);
void MAdp_MFC_SetLVDSBitNum(U8 u8LVDSBitNum);
void MAdp_MFC_SetODCTable(void);
void MAdp_MFC_SetVFreq(U16 u16Inputfreq, BOOL enableFPLL);

#endif /* _MADP_MFC_H_ */
