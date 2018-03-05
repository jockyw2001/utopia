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
// @file   mdrv_gflip_io.h
// @brief  GFlip KMD Driver Interface
// @author MStar Semiconductor Inc.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_GFLIP_IO_H
#define _MDRV_GFLIP_IO_H

//=============================================================================
// Includs
//=============================================================================
#include "mdrv_gflip_st.h"

//=============================================================================
// Defines
//=============================================================================
//IO Ctrl defines:
#define MDRV_GFLIP_IOC_INIT_NR        (0)
#define MDRV_GFLIP_IOC_DEINIT_NR      (MDRV_GFLIP_IOC_INIT_NR+1)
#define MDRV_GFLIP_IOC_SETFLIPINFO_NR   (MDRV_GFLIP_IOC_DEINIT_NR+1)
#define MDRV_GFLIP_IOC_SETINPUTSIGSTATUS_NR    (MDRV_GFLIP_IOC_SETFLIPINFO_NR+1)
#define MDRV_GFLIP_IOC_GETDWININTINFO_NR    (MDRV_GFLIP_IOC_SETINPUTSIGSTATUS_NR+1)
#define MDRV_GFLIP_IOC_GETDWININTINFO2_NR   (MDRV_GFLIP_IOC_GETDWININTINFO_NR + 1)
#define MDRV_GFLIP_IOC_SETPIXELIDADDR_NR    (MDRV_GFLIP_IOC_GETDWININTINFO2_NR+1)
#define MDRV_GFLIP_IOC_SETGPIO3DPIN_NR  (MDRV_GFLIP_IOC_SETPIXELIDADDR_NR+1)
#define MDRV_GFLIP_IOC_ENABLEVECAPTURE_NR      (MDRV_GFLIP_IOC_SETGPIO3DPIN_NR+1)
#define MDRV_GFLIP_IOC_GETVECAPTURESTATE_NR    (MDRV_GFLIP_IOC_ENABLEVECAPTURE_NR+1)
#define MDRV_GFLIP_IOC_VECAPTUREWAITONFRAME_NR    (MDRV_GFLIP_IOC_GETVECAPTURESTATE_NR+1)
#define MDRV_GFLIP_IOC_CLEARFLIPQUEUE_NR     (MDRV_GFLIP_IOC_VECAPTUREWAITONFRAME_NR+1)
#define MDRV_GFLIP_IOC_SETGWININFO_NR     (MDRV_GFLIP_IOC_CLEARFLIPQUEUE_NR+1)
#define MDRV_GFLIP_IOC_DLCCHANGECURVE_NR   (MDRV_GFLIP_IOC_SETGWININFO_NR+1)
#define MDRV_GFLIP_IOC_DLCONOFFINFO_NR       (MDRV_GFLIP_IOC_DLCCHANGECURVE_NR+1)       //14
#define MDRV_GFLIP_IOC_SET3DFLIPINFO_NR       (MDRV_GFLIP_IOC_DLCONOFFINFO_NR+1)       //15
#define MDRV_GFLIP_IOC_BLECHANGEPOINT_NR      (MDRV_GFLIP_IOC_SET3DFLIPINFO_NR+1)       //16
#define MDRV_GFLIP_IOC_BLEONOFFINFO_NR        (MDRV_GFLIP_IOC_BLECHANGEPOINT_NR+1)       //17
#define MDRV_GFLIP_IOC_DLCGETHISTOGRAMINFO_NR  (MDRV_GFLIP_IOC_BLEONOFFINFO_NR+1)       //18
#define MDRV_GFLIP_IOC_SETMULTIFLIPINFO_NR  (MDRV_GFLIP_IOC_DLCGETHISTOGRAMINFO_NR+1)       //19
#define MDRV_GFLIP_IOC_GETVSYNC_NR  (MDRV_GFLIP_IOC_SETMULTIFLIPINFO_NR+1)       //20
#define MDRV_GFLIP_IOC_TLBSetTLBInfo_NR  (MDRV_GFLIP_IOC_GETVSYNC_NR+1)       //21
#define MDRV_GFLIP_IOC_TLBSET3DFLIPINFO_NR  (MDRV_GFLIP_IOC_TLBSetTLBInfo_NR+1)       //22
#define MDRV_GFLIP_IOC_SETTLBMULTIFLIPINFO_NR  (MDRV_GFLIP_IOC_TLBSET3DFLIPINFO_NR+1)       //23
#define MDRV_GFLIP_IOC_SETDLCINITINFO_NR  (MDRV_GFLIP_IOC_SETTLBMULTIFLIPINFO_NR+1)       //24

#define MDRV_GFLIP_IOC_DLCSETHDRINFO_NR  51       //51

#define MDRV_GFLIP_IOC_MAX_NR              (MDRV_GFLIP_IOC_DLCSETHDRINFO_NR+1)


// use 'm' as magic number
#define MDRV_GFLIP_IOC_MAGIC      ('2')
#define MDRV_GFLIP_IOC_INIT                     _IOR(MDRV_GFLIP_IOC_MAGIC,   MDRV_GFLIP_IOC_INIT_NR, MS_U32)
//#define MDRV_GFLIP_IOC_DEINIT     _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_DEINIT_NR, MS_U32)
#define MDRV_GFLIP_IOC_SETFLIPINFO       _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_SETFLIPINFO_NR, MS_GFLIP_INFO)
#define MDRV_GFLIP_IOC_SETINPUTSIGSTATUS    _IOR(MDRV_GFLIP_IOC_MAGIC,   MDRV_GFLIP_IOC_SETINPUTSIGSTATUS_NR, MS_BOOL)
#define MDRV_GFLIP_IOC_GETDWININTINFO       _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_GETDWININTINFO_NR, MS_GFLIP_DWININT_INFO)
#define MDRV_GFLIP_IOC_GETDWININTINFO2      _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_GETDWININTINFO2_NR, MS_GFLIP_DWININT_INFO2)
#define MDRV_GFLIP_IOC_SETPIXELIDADDR       _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_SETPIXELIDADDR_NR, MS_GFLIP_INFO)
#define MDRV_GFLIP_IOC_SETGPIO3DPIN         _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_SETGPIO3DPIN_NR, MS_GFLIP_INFO)
#define MDRV_GFLIP_IOC_ENABLEVECAPTURE      _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_ENABLEVECAPTURE_NR, MS_GFLIP_VECAPTURESTATE)
#define MDRV_GFLIP_IOC_GETVECAPTURESTATE    _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_GETVECAPTURESTATE_NR, MS_GFLIP_VECAPTURESTATE)
#define MDRV_GFLIP_IOC_VECAPTUREWAITONFRAME     _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_VECAPTUREWAITONFRAME_NR, MS_GFLIP_VECAPTURESTATE)
#define MDRV_GFLIP_IOC_CLEARFLIPQUEUE       _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_CLEARFLIPQUEUE_NR,MS_GFLIP_GOPGWINIDX)
#define MDRV_GFLIP_IOC_SETGWININFO          _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_SETGWININFO_NR,MS_GWIN_INFO)
#define MDRV_GFLIP_IOC_DLCCHANGECURVE       _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_DLCCHANGECURVE_NR,MS_DLC_INFO)
#define MDRV_GFLIP_IOC_DLCONOFFINFO          _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_DLCONOFFINFO_NR,MS_BOOL)
#define MDRV_GFLIP_IOC_SET3DFLIPINFO       _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_SET3DFLIPINFO_NR, MS_GFLIP_3DINFO)
#define MDRV_GFLIP_IOC_BLECHANGEPOINT       _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_BLECHANGEPOINT_NR,MS_BLE_INFO)
#define MDRV_GFLIP_IOC_BLEONOFFINFO          _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_BLEONOFFINFO_NR,MS_BOOL)
#define MDRV_GFLIP_IOC_DLCGETHISTOGRAMINFO     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_DLCGETHISTOGRAMINFO_NR,MS_U16)
#define MDRV_GFLIP_IOC_SETMULTIFLIPINFO     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_SETMULTIFLIPINFO_NR,MS_U16)
#define MDRV_GFLIP_IOC_GETVSYNC     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_GETVSYNC_NR,MS_U32)
#define MDRV_GFLIP_IOC_TLBSETFLIPINFO     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_TLBSetTLBInfo_NR,MS_TLB_GFLIP_INFO)
#define MDRV_GFLIP_IOC_TLBSET3DFLIPINFO       _IOWR(MDRV_GFLIP_IOC_MAGIC, MDRV_GFLIP_IOC_TLBSET3DFLIPINFO_NR, MS_TLB_GFLIP_3DINFO)
#define MDRV_GFLIP_IOC_SETTLBMULTIFLIPINFO     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_SETTLBMULTIFLIPINFO_NR,MS_U16)
#define MDRV_GFLIP_IOC_SETDLCINITINFO     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_SETDLCINITINFO_NR,MS_DLC_INIT_INFO)
#define MDRV_GFLIP_IOC_DLCSETHDRINFO     _IOWR(MDRV_GFLIP_IOC_MAGIC,MDRV_GFLIP_IOC_DLCSETHDRINFO_NR,MS_HDR_INFO)

#endif //_MDRV_GFLIP_IO_H
