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
////////////////////////////////////////////////////////////////////////////////

#ifndef _APIMFE_PRIV_H_
#define _APIMFE_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file apiMFE.h
/// @author MStar Semiconductor Inc.
/// @brief MFE control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drv_mfe_st.h"
#include "mfe_common.h"

typedef MS_BOOL (*IOCTL_MFE_INIT)(PVR_Info*);
typedef MS_BOOL (*IOCTL_MFE_ENCODE)(PVR_Info*);
typedef MS_BOOL (*IOCTL_MFE_GETVOL)(PVR_Info*, void*);
typedef MS_BOOL (*IOCTL_MFE_COMPRESSONEPICTURE)(PVR_Info*, MEMMAP_CUR_t, MS_BOOL);
typedef MS_BOOL (*IOCTL_MFE_DEINIT)(PVR_Info*);
typedef MS_BOOL (*IOCTL_MFE_POWEROFF)(PVR_Info*);

typedef MS_BOOL (*IOCTL_MFE_GETOBUF)(PVR_Info* mfe_Info,MEMMAP_t* obuf);

typedef MS_BOOL (*IOCTL_MFE_SETBITRATE_FRAMERATE)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_SETCOLORFORMAT)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_RESETSPSPPS)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_SETFRAMETYPE)(PVR_Info* mfe_Info,MS_S32 frametype);
typedef MS_BOOL (*IOCTL_MFE_SETISR)(PVR_Info* mfe_Info);
typedef MS_BOOL (*IOCTL_MFE_GETHWCAP)(PVR_Info* mfe_Info, MFE_HwCapV0 *pDrv, MS_U32 *len);
typedef MS_BOOL (*IOCTL_MFE_GETOUTPUT_INFO)(PVR_Info *mfe_Info, MS_S32 *frametype, MS_S32 *size);
typedef MS_BOOL (*IOCTL_MFE_SETVUI)(PVR_Info* mfe_Info, VUI_Info* vui_info);

typedef struct __attribute__((packed)) _MFE_INSTANCE_PRIVATE
{
    // instance variable
    IOCTL_MFE_INIT                    fpMFEInit;
    IOCTL_MFE_ENCODE                  fpMFEEncode;
    IOCTL_MFE_GETVOL                  fpMFEGetVOL;
    IOCTL_MFE_COMPRESSONEPICTURE      fpMFECompressOnePicture;
    IOCTL_MFE_DEINIT                  fpMFEDeInit;
    IOCTL_MFE_POWEROFF                fpMFEPowerOff;
    IOCTL_MFE_GETOBUF                 fpMFEGetOBuf;
    IOCTL_MFE_SETBITRATE_FRAMERATE    fpMFESetBitrateFramerate;
    IOCTL_MFE_SETCOLORFORMAT          fpMFESetColorFormat;
    IOCTL_MFE_RESETSPSPPS             fpMFEResetSPSPPS;
    IOCTL_MFE_SETFRAMETYPE            fpMFESetFrameType;
    IOCTL_MFE_SETISR                  fpMFESetISR;
    IOCTL_MFE_GETHWCAP                fpMFEGetHWCap;
    IOCTL_MFE_GETOUTPUT_INFO          fpMFEGetOutputInfo;
    IOCTL_MFE_SETVUI                  fpMFESetVUI;
}MFE_INSTANCE_PRIVATE;

typedef struct __attribute__((packed)) _MFE_RESOURCE_PRIVATE
{
    MS_S32 sAliveInstance;
}MFE_RESOURCE_PRIVATE;

void MFERegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 MFEOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 MFEClose(void* pInstance);
MS_U32 MFEIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);


MS_BOOL _MApi_MFE_Init(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_Encode(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_GetVOL(PVR_Info* mfe_Info, void *header_info);
MS_BOOL _MApi_MFE_CompressOnePicture(PVR_Info* mfe_Info, MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe);
MS_BOOL _MApi_MFE_DeInit(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_PowerOff(PVR_Info* mfe_Info);

MS_BOOL _MApi_MFE_GetOBuf(PVR_Info* mfe_Info,MEMMAP_t* obuf);

MS_BOOL _MApi_MFE_SetBitrateFramerate(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_SetColorFormat(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_ResetSPSPPS(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_SetFrameType(PVR_Info* mfe_Info, MS_S32 frametype);
MS_BOOL _MApi_MFE_ISREnable(PVR_Info* mfe_Info);
MS_BOOL _MApi_MFE_GetHWCap(PVR_Info* mfe_Info, MFE_HwCapV0 *pDrv, MS_U32 *len);
MS_BOOL _MApi_MFE_GetOutputInfo(PVR_Info *mfe_Info, MS_S32 *frametype, MS_S32 *size);
MS_BOOL _MApi_MFE_SetVUI(PVR_Info *mfe_Info, VUI_Info* vui_info);
#ifdef __cplusplus
}
#endif
#endif // _APIMFE_PRIV_H_





