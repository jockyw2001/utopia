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

#ifndef _DMS_PRIV_H_
#define _DMS_PRIV_H_
#include "UFO.h"
////////////////////////////////////////////////////////////////////////////////
/// @file drvDMS.h
/// @author MStar Semiconductor Inc.
/// @brief DMS control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

typedef EN_DMS_RESULT                    (*IOCTL_DMS_INIT)                    (void*, ST_DMS_INITDATA*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETMEMTYPE)            (void*, ST_DMS_SET_MEMORY_TYPE*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_CREATEWINDOW)          (void*, ST_DMS_WINDOW*, ST_DMS_CREATE_WIN_INFO*, MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETDIGIDECINFO)       (void*, MS_U32, ST_DMS_DISPFRAMEFORMAT*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_CLEARDIGIDECINFO)    (void*, MS_U32);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_DESTORYWINDOW)        (void*, MS_U32);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETWINDOW)             (void*, MS_U32, ST_DMS_SETWIN_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETWINDOWINFO)        (void*, MS_U32, ST_DMS_WINDOW_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VIDEOFLIP)             (void*, MS_U32, ST_DMS_DISPFRAMEFORMAT*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETZORDER)             (void*, MS_U32, ST_DMS_ZORDER_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VIDEOMUTE)             (void*, MS_U32, ST_DMS_MUTE_WINDOW_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETMUTECOLOR)         (void*, MS_U32, ST_DMS_MUTE_COLOR_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_VIDEOFREEZE)          (void*, MS_U32, ST_DMS_FREEZE_WINDOW_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETSTATUS)             (void*, EN_DMS_STATUS*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETCAPS)               (void*, EN_DMS_CAPABILITY*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETEXISTWIN)          (void*, MS_U32*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETOUTPUTLAYER)      (void*, ST_DMS_WINDOW*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SET3DMODE)             (void*, MS_U32, ST_DMS_3D_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_SETCAPTUREINFO)      (void*, MS_U32, ST_DMS_SET_CAPTURE_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_GETCAPTUREBUF)        (void*, MS_U32, ST_DMS_CAPTURE_INFO*);
typedef EN_DMS_RESULT                    (*IOCTL_DMS_RELEASECAPTUREBUF)   (void*, MS_U32, ST_DMS_CAPTURE_INFO*);

typedef enum
{
    E_DMS_POOL_ID_INTERNAL_VARIABLE = 0,
    E_DMS_POOL_ID_MAX,
} E_DMS_POOL_ID;

typedef struct
{
    MS_PHY phyDSAddr;
    MS_U8 u8DSIndex;
    DMS_DS_SWDB stDMS_DSSWDB;
} ST_HAL_DMS;

typedef struct
{
#ifdef DMS_VERSION_DIPGOP
    DMS_DIPMEM_INFO _stXC_DIPMemInfo[E_DMS_DIP_CLIENT_MAX];
    DMS_DIPWIN_INFO _stXC_DIPWinInfo[DMS_MAX_WINDOW_NUM];
    DMS_DIPWIN_INFO _stXC_DIPWinInfo_MVOPDoubleBuffer;
    ST_DMS_WINDOW _stXC_PreDispWinInfo[DMS_MAX_WINDOW_NUM];
    DMS_DISP_BUFFER_FLAG _stXC_DispBuf_Flag[E_DMS_DIP_MULTIVIEW_BUF_ID_MAX][DMS_MAX_WINDOW_NUM];

    DMS_DIPWriteReadPointer _stXC_DIPWriteReadPointer[DMS_MAX_WINDOW_NUM];
    MS_U32 u32MaxOutputFrameRate;
    MS_U32 u32VsyncCount;
    MS_U32 u32GOPDispCount;
    MS_U8 u8DIPReadytoCaptureID[DMS_MAX_WINDOW_NUM];
    MS_U8 u8DIP_Window[E_DMS_DIP_MAX];

    MS_PHY u32DIPAddress_pa[E_DMS_DIP_CLIENT_MAX];
    MS_PHY u32GEAddress_pa;
    MS_U8 u8GOPGWinNum;
    MS_BOOL bIsGOPInit;
    MS_BOOL bDIPWriteFirstAddr;
    MS_BOOL bDIPMVOPWriteFirstAddr;
    MS_U32 u32WindowFrameRate[DMS_MAX_WINDOW_NUM];
    MS_U8 u8EnableWindowCount;
    MS_U8 u8UseDIPWindowCount;
    MS_BOOL bNeedGEstretch;

    E_DMS_MVOP_FLOW_CONTROL eDMS_MVOPFlowCtrl;
    MS_BOOL bDMS_MVOPFlowFromDi;
    E_DMS_MVOP_FLOW_CONTROL eDMS_PreMVOPFlowCtrl;
    MS_U8 gDMS_MVOPShowBufferID;
    MS_U8 gDMS_MVOPRemoveBufferID;
    MS_BOOL bMVOPMarkRemoveBuffer;
    MS_BOOL bIsWindowMute[DMS_MAX_WINDOW_NUM];

    MS_U32 gDMS_ScalingCondition;
    MS_U32 gu8DMS_SetXC_Status[DMS_MAX_WINDOW_NUM];
    MS_U8 gu8DMS_Seq_Change_ID;
    ST_DMS_WINDOW _stXC_XCDispWinInfo;
    ST_DMS_WINDOW _stXC_XCCropWinInfo;
    ST_DMS_WINDOW _stXC_XCDispWinInfo_DoubleBuffer;
    ST_DMS_WINDOW _stXC_XCCropWinInfo_DoubleBuffer;

    // For DS regen crop
    ST_DMS_WINDOW _stXC_XCCropWinInfo_raw;
    MS_U16 u16FirstSrcWidth;
    MS_U16 u16FirstSrcHeight;

    MS_PHY u32MVOPFreezeAddress_pa;
    MS_BOOL bDMS_Freeze_DoubleBuffer;
    MS_BOOL _bForceSwitchBufferFlag;

    MS_U32 u32OutputRate;
    MS_U32 bStartCountFRC[DMS_MAX_WINDOW_NUM];
    MS_U32 u32AccumuleInputRate[DMS_MAX_WINDOW_NUM];
#endif

    DMS_VDECFRAME_INFO _stXC_DIPPushInfo[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U8 _stXC_LocalFrameRefCount[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U8 WindowZOrder[DMS_MAX_WINDOW_NUM];
    MS_U8 WindowScore[DMS_MAX_WINDOW_NUM];
    ST_DMS_WINDOW stOutputLayer;

#ifdef DMS_VERSION_VSYNCBRDIGE
    // vsyncbridge
    MS_PHY phyVsyncbridgeAddr[DMS_MAX_WINDOW_NUM];
    MS_PHY phyVsyncbridgeExtAddr[DMS_MAX_WINDOW_NUM];
    MS_U8 u8MirrorMode;
    MS_BOOL bWinChanged[DMS_MAX_WINDOW_NUM];
    MS_BOOL bWinUsed[DMS_MAX_WINDOW_NUM];
    MS_BOOL bWindowInfoReady[DMS_MAX_WINDOW_NUM];
    ST_DMS_OUTPUT_WIN_INFO stOutputWinInfo[DMS_MAX_WINDOW_NUM];
    ST_DMS_WINDOW stDisplayWin[DMS_MAX_WINDOW_NUM];
#endif
#ifdef DMS_VERSION_DIPGOP
    // For mvop release
    MS_U32  u32ReleaseState[DMS_DIP_QUEUEDEPTH];
    MS_BOOL bFirstPlay;
    MS_BOOL bDisableBlackScreen;
    MS_U32 u32MuteCounter;
    MS_BOOL bXCEnableBob;
#endif
} ST_DRV_DMS;

#ifdef DMS_VERSION_DIPGOP
typedef struct
{
    DMS_VDECFRAME_INFO _stXC_DIPDIInfo[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    DMS_DIPWriteReadPointer _stXC_DIPWriteReadPointerInterlace[DMS_MAX_WINDOW_NUM];
    MS_U8 _u8DMS_DiSetEventBufferID;
    MS_U8 _u8DMS_DiReleaseVDECFrameBufferID;
    MS_U8 u8DIPReadytoCaptureIDInterlace[DMS_MAX_WINDOW_NUM];
    MS_U8 _stXC_LocalFrameRefCountInterlace[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U8 u8DIRingBufferID;
    MS_U8 _bDiNowShowRingBufID;
    MS_U8 _bDiReleaseShowRingBufID;
    MS_BOOL _bSetwinToDiTask_DoubleBuffer;

    MS_PHY u32DIPDIAddress_pa;
    MS_BOOL bFlipToDiTask;               //For set event can not use, temp solution for push di task
} ST_DRV_DMS_DI;

typedef struct
{
    DMS_DIPCAPTUREWIN_INFO _stXC_DIPCaptureWinInfo[DMS_MAX_WINDOW_NUM];
    DMS_DIPWriteReadPointer _stXC_DIPCapWin_WriteReadPointer[DMS_MAX_WINDOW_NUM];
    DMS_DIPCaptureWin_WritePointer _stXC_DIPCaptureWin_RingBufferPointer[DMS_MAX_WINDOW_NUM];
    MS_U8 _stXC_CaptureRingBufRefCount[DMS_MAX_WINDOW_NUM][DMS_CAPTUREWIN_RING_BUFFER];
    MS_U8 _stXC_LocalFrameRefCountCaptureMode[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U8 u8CapturedWinNum;
    MS_U8 u8frameRepeatCnt[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U32 bIsFrameValid[DMS_MAX_WINDOW_NUM][DMS_DIP_QUEUEDEPTH];
    MS_U32 bIsRingBufAccessed[DMS_CAPTUREWIN_RING_BUFFER];
#if DMS_DIP_Support_IMI
    DMS_DIPCAPTURE_IMI_INFO _stXC_IMICaptureInfo[DMS_MAX_WINDOW_NUM];
#endif
} ST_DRV_DMS_CAPTUREMODE;

#endif

typedef struct __attribute__((__packed__))
{
    // flow control related
    MS_BOOL bResourceRegistered;
    MS_BOOL bDrvInit;
    ST_HAL_DMS stHalDMS;
    ST_DRV_DMS stDrvDMS;
    #ifdef DMS_VERSION_DIPGOP
    ST_DRV_DMS_DI stDrvDMSDi;
    ST_DRV_DMS_CAPTUREMODE stDrvDMSCapMode;
    #endif
} DMS_RESOURCE_PRIVATE;

typedef struct _DAC_INSTANT_PRIVATE
{
    IOCTL_DMS_INIT                  fpDMSInit;
    IOCTL_DMS_SETMEMTYPE            fpDMSSetMemType;
    IOCTL_DMS_CREATEWINDOW          fpDMSCreateWin;
    IOCTL_DMS_SETDIGIDECINFO        fpDMSSetDigiDecInfo;
    IOCTL_DMS_CLEARDIGIDECINFO      fpDMSClearDigiDecInfo;
    IOCTL_DMS_DESTORYWINDOW         fpDMSDestoryWin;
    IOCTL_DMS_SETWINDOW             fpDMSSetWin;
    IOCTL_DMS_GETWINDOWINFO         fpDMSGetWinInfo;
    IOCTL_DMS_VIDEOFLIP             fpDMSFlip;
    IOCTL_DMS_SETZORDER             fpDMSZOrder;
    IOCTL_DMS_VIDEOMUTE             fpDMSMute;
    IOCTL_DMS_SETMUTECOLOR          fpDMSSetMuteColor;
    IOCTL_DMS_VIDEOFREEZE           fpDMSFreeze;
    IOCTL_DMS_GETSTATUS             fpDMSGetStatus;
    IOCTL_DMS_GETCAPS               fpDMSGetCaps;
    IOCTL_DMS_GETEXISTWIN           fpDMSGetExistWin;
    IOCTL_DMS_SETOUTPUTLAYER        fpDMSSetOutputLayer;
    IOCTL_DMS_SET3DMODE             fpDMSSet3DMode;
    IOCTL_DMS_SETCAPTUREINFO        fpDMSSetCaptureInfo;
    IOCTL_DMS_GETCAPTUREBUF         fpDMSGetCaptureBuf;
    IOCTL_DMS_RELEASECAPTUREBUF     fpDMSReleaseCaptureBuf;
}DMS_INSTANT_PRIVATE;

EN_DMS_RESULT MApi_DMS_Init_U2(void* pInstance, ST_DMS_INITDATA *pstDMS_InitData);
EN_DMS_RESULT MApi_DMS_SetMemory_U2(void* pInstance, ST_DMS_SET_MEMORY_TYPE *pstDMS_SetMemType);
EN_DMS_RESULT MApi_DMS_CreateWindow_U2(void* pInstance, ST_DMS_WINDOW *pstOutputWin, ST_DMS_CREATE_WIN_INFO *pstCreateWin_Info, MS_U32 *pu32WindowID);
EN_DMS_RESULT MApi_DMS_SetDigitalDecodeSignalInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT *pstDispFrameFormat);
EN_DMS_RESULT MApi_DMS_ClearDigitalDecodeSignalInfo_U2(void* pInstance, MS_U32 u32WindowID);
EN_DMS_RESULT MApi_DMS_DestroyWindow_U2(void* pInstance, MS_U32 u32WindowID);
EN_DMS_RESULT MApi_DMS_SetWindow_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_SETWIN_INFO *pstDMS_SetWin_Info);
EN_DMS_RESULT MApi_DMS_GetWindowInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_WINDOW_INFO *pstDMS_GetWin_Info);
EN_DMS_RESULT MApi_DMS_Video_Flip_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_DISPFRAMEFORMAT* pstDispFrameFormat);
EN_DMS_RESULT MApi_DMS_SetZOrder_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_ZORDER_INFO *pstZorderInfo);
EN_DMS_RESULT MApi_DMS_Video_Freeze_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_FREEZE_WINDOW_INFO *pstWindowFreeze);
EN_DMS_RESULT MApi_DMS_Video_Mute_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_MUTE_WINDOW_INFO *pstWindowMute);
EN_DMS_RESULT MApi_DMS_Set_MuteColor_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_MUTE_COLOR_INFO *pstMuteColor);
EN_DMS_RESULT MApi_DMS_GetStatus_U2(void* pInstance, EN_DMS_STATUS *peStatus);
EN_DMS_RESULT MApi_DMS_GetCapability_U2(void* pInstance, EN_DMS_CAPABILITY *peCapability);
EN_DMS_RESULT MApi_DMS_GetExistedWindows_U2(void* pInstance, MS_U32 *pu32WindowNums);
EN_DMS_RESULT MApi_DMS_SetOutputLayer_U2(void* pInstance, ST_DMS_WINDOW *pstLayer);
EN_DMS_RESULT MApi_DMS_Set_3D_Mode_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_3D_INFO *pst3DInfo);
EN_DMS_RESULT MApi_DMS_Set_CaptureInfo_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_SET_CAPTURE_INFO *pstSetCaptureInfo);
EN_DMS_RESULT MApi_DMS_Get_CaptureBuffer_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);
EN_DMS_RESULT MApi_DMS_Release_CaptureBuffer_U2(void* pInstance, MS_U32 u32WindowID, ST_DMS_CAPTURE_INFO *pstCaptureInfo);

void DMSRegisterToUtopia(void);
MS_U32 DMSOpen(void** ppInstance, const void* const pAttribute);
MS_U32 DMSClose(void* pInstance);
MS_U32 DMSIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DMS_PRIV_H_
