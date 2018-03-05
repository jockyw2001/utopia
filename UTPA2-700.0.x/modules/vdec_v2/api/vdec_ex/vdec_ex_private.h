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
// comply with all applicable laws and regulations
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
//    or otherwise
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

#ifndef _API_VDEC_EX_V2_PRIV_H_
#define _API_VDEC_EX_V2_PRIV_H_

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)
////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "apiVDEC_EX.h"
#include "apiVDEC_EX_v2.h"
////////////////////////////////////////////////////////////////////////////////
// Define for VDEC layer
////////////////////////////////////////////////////////////////////////////////

#define SUPPORT_DECODER_NUM_MAX     2
#define VDEC_EX_V2_BIT(_bit_)      (1 << (_bit_))
#define MAX_NUM_OF_STORE_USER_SET_CMD  100
#define MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG 4
//#define MAX_NUM_OF_STORE_USER_SET_CMD_ARG MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG+1
////////////////////////////////////////////////////////////////////////////////
// Enum for VDEC layer
////////////////////////////////////////////////////////////////////////////////

enum
{
    E_VDEC_EX_POOL_ID_INTERNAL = 0, 
    E_VDEC_EX_POOL_ID_MAX,
} E_VDEC_EX_POOL_ID;


////////////////////////////////////////////////////////////////////////////////
// Function pointer for VDEC layer
////////////////////////////////////////////////////////////////////////////////
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_GetFreeStream)        (void*,MS_U32,VDEC_EX_Stream,VDEC_EX_CodecType);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_Init)                 (VDEC_StreamId*,VDEC_EX_InitParam*);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_SetControl)           (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**,MS_BOOL);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_GetControl)           (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_PreSetControl)        (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_PostSetControl)       (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);

////////////////////////////////////////////////////////////////////////////////
// Structure for VDEC layer
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    MS_U8 u8DecoderInUse[SUPPORT_DECODER_NUM_MAX];
}VDEC_EX_RESOURCE_PRIVATE;


typedef struct
{
    // instance variable
    MS_U8                                Index_of_decoder;                             
    // Function pointer
    IOCTL_VDEC_EX_GetFreeStream          fpVDEC_EX_GetFreeStream;
    IOCTL_VDEC_EX_Init                   fpVDEC_EX_Init;
    IOCTL_VDEC_EX_SetControl             fpVDEC_EX_SetControl;
    IOCTL_VDEC_EX_GetControl             fpVDEC_EX_GetControl;
    IOCTL_VDEC_EX_PreSetControl          fpVDEC_EX_PreSetControl;
    IOCTL_VDEC_EX_PostSetControl         fpVDEC_EX_PostSetControl;
}VDEC_EX_INSTANCE_PRIVATE;


typedef struct
{
    VDEC_EX_V2_User_Cmd cmd;
    MS_VIRT              u32Arg[MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG];
    //void*               pStructArg;
}VDEC_EX_RECORD_SET_CMD;

typedef struct
{
    MS_U8 u8Idx;
    VDEC_EX_V2_User_Cmd cmd_id;
    void** param;
    MS_U32  u32Arg_num;
    MS_BOOL structArg_num;  
    //MS_U32 struct_size;
    MS_U8  position[MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG]; // index 0~4 for U32ARG position, index 5 for structARG position
}VDEC_EX_RESUME_SET_CMD;



////////////////////////////////////////////////////////////////////////////////
// Function for VDEC layer
////////////////////////////////////////////////////////////////////////////////

VDEC_EX_Result MApi_VDEC_EX_V2_GetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
VDEC_EX_Result MApi_VDEC_EX_V2_SetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**,MS_BOOL);
VDEC_EX_Result MApi_VDEC_EX_V2_GetFreeStream(void*,MS_U32,VDEC_EX_Stream,VDEC_EX_CodecType);
VDEC_EX_Result MApi_VDEC_EX_V2_Init(VDEC_StreamId*,VDEC_EX_InitParam*);
VDEC_EX_Result MApi_VDEC_EX_V2_PreSetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
VDEC_EX_Result MApi_VDEC_EX_V2_PostSetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);

#ifdef __cplusplus
}
#endif

#endif
#undef _API_VDEC_EX_V2_PRIV_H_
#endif // _API_VDEC_EX_V2_PRIV_H_
