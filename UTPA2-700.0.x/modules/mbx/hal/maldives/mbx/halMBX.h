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
/// @file   halMBX.h
/// @brief  MStar Mailbox HAL Driver DDI
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_MBX_H
#define _HAL_MBX_H

#ifdef _HAL_MBX_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros

//=============================================================================
//busy bit Set/Clear/Get
#define   _BUSY_S(arg)  {\
                            MS_U8 val; \
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_BUSY;\
                         }

#define   _BUSY_C(arg)  {\
                            MS_U8 val; \
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_BUSY;\
                         }

#define   _BUSY(arg)    (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_BUSY);

//////////////////////////////////////////////////////////////
//error bit Set/Clear/Get
#define   _ERR_S(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_ERROR;\
                         }

#define   _ERR_C(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_ERROR;\
                         }

#define   _ERR(arg)    (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_ERROR)

//////////////////////////////////////////////////////////////
//disabled bit Set/Clear/Get
#define   _DISABLED_S(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_DISABLED;\
                         }

#define   _DISABLED_C(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_DISABLED;\
                         }

#define   _DISABLED(arg)    (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_DISABLED)

////////////////////////////////////////////////////////////////////////
//overflow bit Set/Clear/Get
#define   _OVERFLOW_S(arg)  {\
                                MS_U8 val;\
                                val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                                REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val | MBX_STATE1_OVERFLOW;\
                              }

#define   _OVERFLOW_C(arg)  {\
                                MS_U8 val;\
                                val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                                REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~MBX_STATE1_OVERFLOW;\
                              }

#define   _OVERFLOW(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) & MBX_STATE1_OVERFLOW)

////////////////////////////////////////////////////////////////////////
//status bit clear
#define   _S1_C(arg)   {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_STATE_1);\
                            REG8_MBX_GROUP(arg, REG8_MBX_STATE_1) = val & ~(MBX_STATE1_DISABLED | MBX_STATE1_OVERFLOW | MBX_STATE1_ERROR | MBX_STATE1_BUSY);\
                        }

////////////////////////////////////////////////////////////////////////
//fire bit Set/Clear/Get
#define   _FIRE_S(arg)  {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                            REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val | MBX_CTRL_FIRE;\
                         }

#define   _FIRE_C(arg)  {\
                            MS_U8 val;\
                            val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                            REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val & ~MBX_CTRL_FIRE;\
                         }

#define   _FIRE(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_CTRL) & MBX_CTRL_FIRE)

////////////////////////////////////////////////////////////////////////
//readback bit Set/Clear/Get
#define   _READBACK_S(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val | MBX_CTRL_READBACK;\
                               }

#define   _READBACK_C(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val & ~MBX_CTRL_READBACK;\
                               }

#define   _READBACK(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_CTRL) & MBX_CTRL_READBACK)

////////////////////////////////////////////////////////////////////////
//instant bit Set/Clear/Get
#define   _INSTANT_S(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val | MBX_CTRL_INSTANT;\
                              }

#define   _INSTANT_C(arg)   {\
                                  MS_U8 val;\
                                  val = REG8_MBX_GROUP(arg, REG8_MBX_CTRL);\
                                  REG8_MBX_GROUP(arg, REG8_MBX_CTRL) = val & ~MBX_CTRL_INSTANT;\
                              }

#define   _INSTANT(arg)   (REG8_MBX_GROUP(arg, REG8_MBX_CTRL) & MBX_CTRL_INSTANT)

//=============================================================================
// Type and Structure Declaration
//=============================================================================

//=============================================================================
// Enums
/// MBX HAL Recv Status Define
typedef enum
{
    /// Recv Success
    E_MBXHAL_RECV_SUCCESS = 0,
    /// Recv Error: OverFlow
    E_MBXHAL_RECV_OVERFLOW = 1,
    /// Recv Error: Not Enabled
    E_MBXHAL_RECV_DISABLED = 2,
} MBXHAL_Recv_Status;

/// MBX HAL Fire Status Define
typedef enum
{
    /// Fire Success
    E_MBXHAL_FIRE_SUCCESS = 0,
    /// Still Firing
    E_MBXHAL_FIRE_ONGOING = 1,
    /// Fire Error: Overflow:
    E_MBXHAL_FIRE_OVERFLOW = 2,
    /// Fire Error: Not Enabled
    E_MBXHAL_FIRE_DISABLED = 3,
} MBXHAL_Fire_Status;

//=============================================================================
// Mailbox HAL Driver Function
//=============================================================================

INTERFACE MBX_Result MHAL_MBX_ClearAll (MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole);

INTERFACE MBX_Result MHAL_MBX_Init(MBX_ROLE_ID eHostRole, MS_U32 u32RIUBaseAddrMBX);
INTERFACE MBX_Result MHAL_MBX_SetConfig(MBX_ROLE_ID eHostRole);

INTERFACE MBX_Result MHAL_MBX_SetInformation(MBX_ROLE_ID eTargetRole, MS_U32 u32RIUBaseAddrMBX, MS_U8 *pU8Info, MS_U8 u8Size);
INTERFACE MBX_Result MHAL_MBX_GetInformation(MBX_ROLE_ID eTargetRole, MS_U32 u32RIUBaseAddrMBX, MS_U8 *pU8Info, MS_U8 u8Size);

INTERFACE MBX_Result MHAL_MBX_Fire(MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole);
INTERFACE MBX_Result MHAL_MBX_GetFireStatus(MBX_ROLE_ID eSrcRole, MBX_ROLE_ID eDstRole, MBXHAL_Fire_Status *pFireStatus);

INTERFACE MBX_Result MHAL_MBX_Recv(MBX_Msg* pMbxMsg, MBX_ROLE_ID eDstRole);
INTERFACE MBX_Result MHAL_MBX_RecvEnd(MBX_ROLE_ID eSrcRole, MBX_ROLE_ID eDstRole, MBXHAL_Recv_Status eRecvSatus);

#undef INTERFACE
#endif //_HAL_MBX_H

