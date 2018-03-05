//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2014 - 2016 MStar Semiconductor, Inc. All rights reserved.
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
// Copyright (c) 2014-2016 MStar Semiconductor, Inc.
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
//
//  File name: apiDMX_tsio_v2.h
//  Description: Demux (dmx) api header file
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DMX_TSIO_V2_H_
#define _DMX_TSIO_V2_H_

#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct DLL_PACKED _DMX_TSIO_Service_SetDMAoutVC_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_DMAOUT_SET*    eSet;
}DMX_TSIO_Service_SetDMAoutVC_PARAM,*PDMX_TSIO_Service_SetDMAoutVC_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_DMAout_WriteGet_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_PHY*             pphyWrite;
}DMX_TSIO_Service_DMAout_WriteGet_PARAM, *PDMX_TSIO_Service_DMAout_WriteGet_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_SetDestination_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_SERVICEDEST    eDest;
}DMX_TSIO_Service_SetDestination_PARAM,*PDMX_TSIO_Service_SetDestination_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_DMAin_PidOpen_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_U16              u16Pid;
    MS_U16*             pu16FltId;
}DMX_TSIO_Service_DMAin_PidOpen_PARAM,*PDMX_TSIO_Service_DMAin_PidOpen_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_DMAin_PidClose_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_U16              u16FltId;
}DMX_TSIO_Service_DMAin_PidClose_PARAM,*PDMX_TSIO_Service_DMAin_PidClose_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_SetDMAinVC_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_DMAIN_SET*     eSet;
}DMX_TSIO_Service_SetDMAinVC_PARAM,*PDMX_TSIO_Service_SetDMAinVC_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_DMAinVC_Start_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_U32              u32Pacing;
}DMX_TSIO_Service_DMAinVC_Start_PARAM,*PDMX_TSIO_Service_DMAinVC_Start_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_SetLocdecKey_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    TSIO_LOCKEY_SET*    eSet;
}DMX_TSIO_Service_SetLocdecKey_PARAM,*PDMX_TSIO_Service_SetLocdecKey_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_LocdecKeyEnable_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_BOOL             bEnable;
}DMX_TSIO_Service_LocdecKeyEnable_PARAM,*PDMX_TSIO_Service_LocdecKeyEnable_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_PidOpen_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    DMX_TSIF            eTSOInSrc;
    MS_U16              u16Pid;
    MS_U16*             pu16DmxId;
}DMX_TSIO_Service_PidOpen_PARAM,*PDMX_TSIO_Service_PidOpen_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_PidClose_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
    MS_U16              u16DmxId;
}DMX_TSIO_Service_PidClose_PARAM,*PDMX_TSIO_Service_PidClose_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_Alloc_PARAM
{
    MS_U32              u32Res;
    MS_U8               u8Sid;
    MS_U16*             u16Handle;
}DMX_TSIO_Service_Alloc_PARAM,*PDMX_TSIO_Service_Alloc_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Service_Free_PARAM
{
    MS_U32              u32Res;
    MS_U16              u16Handle;
}DMX_TSIO_Service_Free_PARAM,*PDMX_TSIO_Service_Free_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_RegisterIntCb_PARAM
{
    MS_U32              u32Res;
    TSIOIntTypeCB       efn;
}DMX_TSIO_RegisterIntCb_PARAM,*PDMX_TSIO_RegisterIntCb_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Init_PARAM
{
    MS_U32              u32Res;
    TSIO_Init_Set*      eParm;
}DMX_TSIO_Init_PARAM,*PDMX_TSIO_Init_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Open_PARAM
{
    MS_U32              u32Res;
    TSIO_CLK            eClk;
}DMX_TSIO_Open_PARAM, *PDMX_TSIO_Open_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_Cmd_PARAM
{
    MS_U32              u32Res;
    TSIO_CMD            eCmd;
    MS_U32              u32Value;
}DMX_TSIO_Cmd_PARAM,*PDMX_TSIO_Cmd_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_CC_PARAM
{
    MS_U32              u32Res;
    TSIO_CC_Set*        Parm;
    TSIO_CC_RESULT*     pResult;
}DMX_TSIO_CC_PARAM,*PDMX_TSIO_CC_PARAM;

typedef struct DLL_PACKED _DMX_TSIO_GetInfo_PARAM
{
    MS_U32              u32Res;
    TSIO_INFO           eInfo;
    MS_U32              u32Arg;
    MS_U32*             u32Retinfo;
}DMX_TSIO_GetInfo_PARAM,*PDMX_TSIO_GetInfo_PARAM;

#ifdef __cplusplus
}
#endif


#endif //_DMX_TSIO_V2_H_
