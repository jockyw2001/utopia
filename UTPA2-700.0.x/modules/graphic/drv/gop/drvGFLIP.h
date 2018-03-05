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
/// @file   drvGFLIP.h
/// @brief  MStar gflip Interface header file
/// @author MStar Semiconductor Inc.
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_GFLIP_H
#define _MDRV_GFLIP_H

#ifdef _MDRV_GFLIP_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if defined(__cplusplus)
extern "C" {
#endif
#include "drvGOP.h"
#include "halGOP.h"
#include "mdrv_gflip_st.h"


#define MAX_GOP_GWIN             MAX_GOP0_GWIN
#define MAX_FLIP_ADDR_FIFO      (0x10UL)

//=============================================================================
// Defines & Macros
//=============================================================================

//=============================================================================
// Type and Structure Declaration
//=============================================================================
typedef struct DLL_PACKED
{
    MS_PHY u64Addr; //in
    MS_U32 u32TagId; //in
}GFLIP_INFO, *PGFLIP_INFO;

typedef struct __attribute__((packed))
{
  MS_GOP_CTX_SHARED apiCtxShared;
  GOP_CTX_HAL_SHARED halCtxShared;
}GOP_CTX_DRV_SHARED;
typedef struct DLL_PACKED
{
    MS_GOP_CTX_LOCAL  apiCtxLocal;
    GOP_CTX_DRV_SHARED *pDrvCtxShared;//pointer to shared context paramemetrs
    GOP_CTX_HAL_LOCAL    halCtxLocal;
    MS_U8 *pu8REGMAP_Base[MAX_GOP_SUPPORT];
    MS_U16 u16REGMAP_Offset[MAX_GOP_SUPPORT];
    MS_U32 u32REGMAP_Len[MAX_GOP_SUPPORT];
    //GFLIP parameters
    MS_BOOL bEnableVsyncIntFlip[MAX_GOP_SUPPORT];
#ifdef MSOS_TYPE_LINUX
    MS_S32 s32FdGFlip;
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#else
    GFLIP_INFO GFlipInfo[MAX_GOP_SUPPORT][MAX_GOP_GWIN][MAX_FLIP_ADDR_FIFO];
    MS_U32 u32GFlipInfoReadPtr[MAX_GOP_SUPPORT][MAX_GOP_GWIN];
    MS_U32 u32GFlipInfoWritePtr[MAX_GOP_SUPPORT][MAX_GOP_GWIN];
    MS_U16 u16GOPRefCnt[MAX_GOP_SUPPORT];
    MS_BOOL bGFlipInVsyncLimitation[MAX_GOP_SUPPORT];
    GFLIP_DWININT_INFO gflipDWinIntInfo;
#endif
    MS_BOOL gop_gwin_frwr;
    MS_BOOL gop_dwin_frwr;
    MS_BOOL bGOPBankFwr[SHARED_GOP_MAX_COUNT];
    MS_BOOL b16BPPTileMode[SHARED_GOP_MAX_COUNT];
    MS_BOOL b32BPPTileMode[SHARED_GOP_MAX_COUNT];

    MS_U8 current_gop;
}GOP_CTX_DRV_LOCAL;

//=============================================================================
// Function
//=============================================================================

//Init related:
INTERFACE MS_BOOL MDrv_GFLIP_Init(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_BOOL bEnableVsyncIntFlip);
INTERFACE MS_BOOL MDrv_GFLIP_DeInit(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx);

//Drv Interface related(drv interface):
INTERFACE MS_BOOL MDrv_GFLIP_SetMultiFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx,MS_GFLIP_MULTIINFO* pMultiinfo);
INTERFACE MS_BOOL MDrv_GFLIP_SetTLBMultiFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx,MS_TLB_GFLIP_MULTIINFO* pTLBMultiinfo);
INTERFACE MS_BOOL MDrv_GFLIP_SetFlipInfo(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY u64MainAddress, MS_PHY u64SubAddress, MS_U32 u32TagId, MS_U32 * u32QEntry);
INTERFACE MS_BOOL MDrv_GFLIP_SetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopIdx, MS_U8 u8GwinIdx,DRV_GOP_GWIN_INFO* pinfo);
INTERFACE MS_BOOL MDrv_GFLIP_RestoreFromVsyncLimitation(MS_GOP_CTX_LOCAL *pGOPCtx);
INTERFACE MS_BOOL MDrv_GFLIP_GetDWinIntInfo(MS_GOP_CTX_LOCAL *pGOPCtx, GFLIP_DWININT_INFO *pGFlipDWinIntInfo, MS_U32 u32Timeout);
INTERFACE MS_BOOL MDrv_GFLIP_GetVECaptureState(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
INTERFACE MS_BOOL MDrv_GFLIP_VECaptureWaitOnFrame(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
INTERFACE MS_BOOL MDrv_GFLIP_EnaVECapture(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
INTERFACE MS_BOOL MDrv_GFLIP_ClearFlipQueue(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_U32 u32GwinIdx);
INTERFACE MS_BOOL MDrv_GFLIP_SetGPIO3DPin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GPIO3DPin);

#if defined(__cplusplus)
}
#endif

#undef INTERFACE

#endif //_MDRV_GFLIP_H

