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
/// @file   halDMS.h
/// @brief  DMS HAL Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_DMS_H_
#define _HAL_DMS_H_
#include "UFO.h"
#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
#include "apiDMS_Type.h"
#include "apiXC.h"
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#if defined(HAL_DMS_C)
#define DMS_HAL_INTERFACE
#else
#define DMS_HAL_INTERFACE extern
#endif

#define DMS_HAL_DIP_Align           16
#define DMS_HAL_DIPR_Align_YUV420   32
#define DMS_DIP_Support_HVSP 1
#define DMS_DIP_Support_DI   0
#define DMS_DIP_Support_3DDI 1
#define DMS_DIP_NWindow_Support_2P5DDI 0
#define DMS_DIP_Support_IMI 0

#define DMS_DIPWindow_NORMAL 0
#define DMS_DIPWindow_CAPTURE 1

#define DMS_MFDEC_CAPTURE 2
#define DMS_MFDEC_NORMAL 1

// DMS_MAIN_WINDOW_FLOW_VERSION
// 0: MAIN_WINDOW_FLOW : don't need mainQ, diQ, DI by XC
// 1: MAIN_WINDOW_FLOW : need mainQ, diQ, DI by DIP
#define DMS_MAIN_WINDOW_FLOW_VERSION 0
#define SKIP_DMS_MAIN_WINDOW_FLOW(windowId) if((DMS_MAIN_WINDOW_FLOW_VERSION == 0) && (windowId == MVOP_WINDOW))    \
                                            {                                \
                                                continue;                    \
                                            }
#define DMS_DONT_USE_CMA 0
//All Hard code Register
#define REG_SC_BK12_0F_L (0x1200|(0x0F<<1))
#define REG_SC_BK12_17_L (0x1200|(0x17<<1))
#define REG_SC_BK36_03_L (0x3600|(0x03<<1))

typedef enum
{
    DS_IP,
    DS_OP,
} DMS_ds_reg_ip_op_sel;

typedef enum
{
    DMS_DS_XC,
    DMS_DS_MVOP,
} DMS_ds_reg_source_sel;

typedef struct
{
    MS_U16 u16CMDCNT_IPM;
    MS_U16 u16CMDCNT_IPS;
    MS_U16 u16CMDCNT_OPM;
    MS_U16 u16CMDCNT_OPS;
} DMS_DS_CMDCNT;

typedef struct
{
    MS_WINDOW_TYPE stMVOPCropWindow;
    MS_PHY phyMVOP_LumaAddr;
    MS_PHY phyMVOP_ChromaAddr;
    MS_U32 u32HPostRatio;
    MS_U32 u32VPostRatio;
    MS_U32 u32FlowFlag;
    MS_U16 u16H_Start;
    MS_U16 u16H_End;
    MS_U16 u16V_Start;
    MS_U16 u16V_End;
    MS_U16 u16MVOP_Pitch;
    MS_BOOL bMVOP_422Pack;
} DMS_DS_SWDB;

DMS_HAL_INTERFACE MS_BOOL Hal_DMS_XC_SetMute(void* pInstance, MS_BOOL bEnable);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_XC_DSSetConfig(void* pInstance);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_XC_DSSetWin(void* pInstance, MS_WINDOW_TYPE* stMVOPCropWindow, const XC_SETWIN_INFO* stXCSetWinInfo, MS_U32 u32FlowFlag);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_XC_DSSetMVOPPitch(void* pInstance, MS_U16 u16Pitch);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_XC_DSSetMVOPAddr(void* pInstance, MS_PHY phyLumaAddr, MS_PHY phyChromaAddr, MS_BOOL b422Pack);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_XC_DSFire(void* pInstance);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_XC_GetCaps(void* pInstance, EN_DMS_CAPABILITY *peCapability);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_Release_CMA(void* pInstance, EN_DMS_MEMORY_TYPE enMemType, MS_U32 u32CMAHeapID);
DMS_HAL_INTERFACE MS_BOOL HAL_DMS_Setup_CMA(void* pInstance, EN_DMS_MEMORY_TYPE enMemType, MS_U32 u32CMAHeapID);


#ifdef __cplusplus
}
#endif

#endif


