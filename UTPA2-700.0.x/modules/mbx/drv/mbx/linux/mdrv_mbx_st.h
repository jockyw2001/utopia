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
///////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//
// @file   mdrv_mbx_st.h
// @brief  Mialbox KMD Driver Interface
// @author MStar Semiconductor Inc.
//
// Driver to initialize and access mailbox.
//     - Provide functions to initialize/de-initialize mailbox
//     - Provide mailbox functional access.
//////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _MDRV_MBX_ST_H
#define _MDRV_MBX_ST_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Type and Structure Declaration
//=============================================================================
//IO Ctrl struct defines:

typedef struct  __attribute__((packed))
{
    MBX_CPU_ID eHKCPU;
    MBX_ROLE_ID eHostRole;
    MS_U32 u32TimeoutMillSecs;
    MBX_Result mbxResult;
}MS_MBX_INIT_INFO, *PMS_MBX_INIT_INFO;

typedef struct  __attribute__((packed))
{
    MS_BOOL  bInfo;
    MBX_Result mbxResult;
}MS_MBX_SET_BINFO, *PMS_MBX_SET_BINFO;

typedef struct  __attribute__((packed))
{
    MBX_Class eMsgClass;
    MS_U16 	u16MsgQueueSize;
    MBX_Result mbxResult;
}MS_MBX_REGISTER_MSG, *PMS_MBX_REGISTER_MSG;

typedef struct  __attribute__((packed))
{
    MBX_Class eMsgClass;
    MS_BOOL bForceDiscardMsgQueue;
    MBX_Result mbxResult;
}MS_MBX_UNREGISTER_MSG, *PMS_MBX_UNREGISTER_MSG;

typedef struct  __attribute__((packed))
{
    MBX_Class eMsgClass;
    MBX_Result mbxResult;
}MS_MBX_CLEAR_MSG, *PMS_MBX_CLEAR_MSG;

typedef struct  __attribute__((packed))
{
    MBX_Msg *pMsg;
    MBX_Result mbxResult;
}MS_MBX_SEND_MSG, *PMS_MBX_SEND_MSG;

typedef struct  __attribute__((packed))
{
    MBX_Class eTargetClass;
    MBX_Msg *pMsg;
    MS_U32 u32WaitMillSecs;
    MS_U32 u32Flag;
    MBX_Result mbxResult;
}MS_MBX_RECV_MSG, *PMS_MBX_RECV_MSG;

typedef struct  __attribute__((packed))
{
    MBX_Class eTargetClass;
    MBX_MSGQ_Status *pMsgQueueStatus;
    MBX_Result mbxResult;
}MS_MBX_GET_MSGQSTATUS, *PMS_MBX_GET_MSGQSTATUS;

typedef struct  __attribute__((packed))
{
    MS_BOOL bEnabled;
    MS_S32  s32RefCnt;
}MS_MBX_GET_DRVSTATUS, *PMS_MBX_GET_DRVSTATUS;

typedef struct  __attribute__((packed))
{
    MBX_ROLE_ID eTargetRole;
    MS_U8 *pU8Info;
    MS_U8 u8Size;
    MBX_Result mbxResult;
}MS_MBX_CPROSYNC_INFORMATION, *PMS_MBX_CPROSYNC_INFORMATION;

#endif //_MDRV_MBX_ST_H
