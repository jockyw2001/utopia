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

#ifndef _MDRV_SC_IP_H_
#define _MDRV_SC_IP_H_

#include "mhal_ip.h"
#include "mhal_xc_chip_config.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define PERIOD_TOLARANCE 5
#define MAX_GET_VPERIOD_TIME 3
#define ENABLE_DEBUG_CAPTURE_N_FRAMES           0

// mode change torlance for timing monitor, also used in auto procedure
#define MD_HPERIOD_TORLANCE         5      // horizontal period torlance
#define MD_VTOTAL_TORLANCE          5      // vertical total torlance
#define MD_HDE_TORLANCE             5
#define MD_VDE_TORLANCE             5

#define ACCURATE_VFREQ_TOLERANCE 20 //Tolerance for using new accurate vfreq method: 1hz

#define ACCURATE_VFREQ_TOLERANCE_FOR_VD_SOURCE 8 //Tolerance for using new accurate vfreq method: 0.8hz
#define ACCURATE_VFREQ_TOLERANCE_FOR_VD_SOURCE_EX 2 //Tolerance for using new accurate vfreq method: 0.2hz


#define WAIT_VSYNC_TIME_MAX  50

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
/// Indecate the status of pc monitor.
typedef struct __attribute__((packed))
{
    INPUT_SOURCE_TYPE_t eCurrentSrc;    ///< indicate current input source type

    XC_PCMONITOR_STATUS eCurrentState;       ///< timing monitor state
    XC_PCMONITOR_STATUS ePreMonitorState;  ///< timing pre monitor state

    MS_U32 u32LastExecutedTime;            ///< 30ms execute timing monitor once
    MS_BOOL bInputTimingChange;         ///< indicate input timing changed or not
    MS_U8  ucInputTimingStableCounter;     ///< input timing stable counter
    MS_U8  ucTimingChangeDebounce;         ///< debounce counter when timing un-stable
    MS_BOOL bInputTimingStable;         ///< input timing stable or not
    MS_BOOL bDoModeSetting;             ///< indicate the timing is stable (either with sync or without sync) and can do mode setting

    // Analog
    MS_U16 u16Hperiod;                     ///< Hperiod got from scaler
    MS_U16 u16VSyncTime;                   ///< vsync time calculated from Hperiod & Vtotal

    MS_U16 u16Vtotal;                      ///< Vertical total
    MS_U8  u8SyncStatus;                   ///< from XC driver, including H/V sync polarity, interlaced or progressive

    // Digital
    MS_WINDOW_TYPE sDVI_HDMI_DE_Info;   ///< DVI/HDMI input DE information
    MS_U32 u32FrameRatex1K;                // Frame rate x1K

} XC_PCMONITOR_HK_INFO;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef _MDRV_SC_IP_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef enum
{
    IP_ANALOG1 = 0x00,
    IP_ANALOG2 = 0x01,
    IP_ANALOG3 = 0x02,
    IP_DVI     = 0x03,
    IP_VIDEO   = 0x04,
    IP_HDTV    = 0x05,
    IP_HDMI    = 0x07,
} EN_ISELECT;

// This can be added in the future.
// FCLK have to be changed case by case.
typedef enum
{
    EN_FCLK_NORMAL,
    EN_FCLK_FBL,
} EN_SET_FCLK_CASE;

extern MS_U16 MDrv_XC_cal_pixclk(void *pInstance, MS_U16 u16HTotal, SCALER_WIN eWindow);
extern void MDrv_XC_reset_ip(void *pInstance, SCALER_WIN eWindow);

#define MDrv_SC_de_only_en  Hal_SC_de_only_en
INTERFACE void MDrv_SC_de_only_en(void *pInstance, MS_BOOL benable, SCALER_WIN eWindow);

#define MDrv_SC_de_direct_en  Hal_SC_de_direct_en
INTERFACE void MDrv_SC_de_direct_en(void *pInstance, MS_BOOL benable, SCALER_WIN eWindow);

#define MDrv_SC_set_delayline  Hal_SC_set_delayline
INTERFACE void MDrv_SC_set_delayline(void *pInstance, MS_U8 u8DelayLines, SCALER_WIN eWindow );

#define MDrv_SC_set_Fclk  Hal_SC_set_Fclk
INTERFACE void MDrv_SC_set_Fclk(void *pInstance, EN_SET_FCLK_CASE enCase );

#define MDrv_SC_setfield Hal_SC_setfield
INTERFACE void MDrv_SC_setfield(void *pInstance, MS_U16 reg_1D, MS_U16 reg_21, MS_U16 reg_23, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_input_source Hal_SC_ip_set_input_source
INTERFACE void MDrv_SC_ip_set_input_source(void *pInstance, MS_U8 u8InputSrcSel, MS_U8 u8SyncSel, MS_U8 u8VideoSel, MS_U8 u8isYPbPr, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_image_wrap Hal_SC_ip_set_image_wrap
INTERFACE void MDrv_SC_ip_set_image_wrap(void *pInstance, MS_BOOL bHEnable, MS_BOOL bVEnable, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_input_sync_reference_edge  Hal_SC_ip_set_input_sync_reference_edge
INTERFACE void MDrv_SC_ip_set_input_sync_reference_edge(void *pInstance, MS_BOOL bHRef, MS_BOOL bVRef, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_input_vsync_delay Hal_SC_ip_set_input_vsync_delay
INTERFACE void MDrv_SC_ip_set_input_vsync_delay(void *pInstance, MS_BOOL bDelay, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_de_only_mode Hal_SC_ip_set_de_only_mode
INTERFACE void MDrv_SC_ip_set_de_only_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_coast_input Hal_SC_ip_set_coast_input
INTERFACE void MDrv_SC_ip_set_coast_input(void *pInstance, MS_BOOL bInputSel, SCALER_WIN eWindow );

#define MDrv_ip_set_input_10bit hal_ip_set_input_10bit
INTERFACE void MDrv_ip_set_input_10bit(void *pInstance, MS_BOOL bInput10Bit, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_ms_filter Hal_SC_ip_set_ms_filter
INTERFACE void MDrv_SC_ip_set_ms_filter(void *pInstance, MS_BOOL bEnable, MS_U16 u16FilterRange, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_coast_window Hal_SC_ip_set_coast_window
INTERFACE void MDrv_SC_ip_set_coast_window(void *pInstance, MS_U8 u8Start, MS_U8 u8End, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_input_sync_sample_mode Hal_SC_ip_set_input_sync_sample_mode
INTERFACE void MDrv_SC_ip_set_input_sync_sample_mode(void *pInstance, MS_BOOL bMode, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_de_bypass_mode Hal_SC_ip_set_de_bypass_mode
INTERFACE void Hal_SC_ip_set_de_bypass_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_de_lock_mode Hal_SC_ip_set_de_lock_mode
INTERFACE void Hal_SC_ip_set_de_lock_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow );

#define MDrv_SC_get_hsd_pixel_count Hal_SC_get_hsd_pixel_count
INTERFACE MS_U16 Hal_SC_get_hsd_pixel_count(void *pInstance, SCALER_WIN eWindow );

#define MDrv_SC_get_vsd_line_count Hal_SC_get_vsd_line_count
INTERFACE MS_U16 Hal_SC_get_vsd_line_count(void *pInstance, SCALER_WIN eWindow );

#define MDrv_SC_ip_set_post_glitch_removal Hal_SC_ip_set_post_glitch_removal
INTERFACE void MDrv_SC_ip_set_post_glitch_removal(void *pInstance, MS_BOOL bEnble, MS_U8 u8Range, SCALER_WIN eWindow );

#define MDrv_SC_ip_get_post_glitch_removal Hal_SC_ip_get_post_glitch_removal
INTERFACE MS_U8 MDrv_SC_ip_get_post_glitch_removal(void *pInstance, SCALER_WIN eWindow);

#define MDrv_SC_ip_set_DE_Mode_Glitch Hal_SC_ip_set_DE_Mode_Glitch
INTERFACE void MDrv_SC_ip_set_DE_Mode_Glitch(void *pInstance, MS_U8 u8Setting , SCALER_WIN eWindow );

#define MDrv_SC_ip_get_DE_mode_glitch_protect_enabled Hal_SC_ip_get_DE_mode_glitch_protect_enabled
INTERFACE MS_BOOL MDrv_SC_ip_get_DE_mode_glitch_protect_enabled(void *pInstance, SCALER_WIN eWindow);

// Htt Vtt
INTERFACE MS_U16 Hal_SC_ip_get_horizontalPeriod(void *pInstance, SCALER_WIN eWindow);
#define MDrv_SC_ip_get_horizontalPeriod  Hal_SC_ip_get_horizontalPeriod

INTERFACE MS_U16 Hal_SC_ip_get_verticaltotal(void *pInstance, SCALER_WIN eWindow);
#define MDrv_SC_ip_get_verticaltotal     Hal_SC_ip_get_verticaltotal

#define MDrv_SC_ip_set_handshaking_md Hal_SC_ip_set_handshaking_md
INTERFACE void MDrv_SC_ip_set_handshaking_md(void *pInstance, MS_U8 u8MD, SCALER_WIN eWindow);

INTERFACE void MDrv_XC_Set_FD_Mask_ByWin(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow);

#define MDrv_XC_GetCaptureWindow Hal_SC_ip_get_capture_window
INTERFACE void MDrv_XC_GetCaptureWindow(void *pInstance, MS_WINDOW_TYPE* capture_win, SCALER_WIN eWindow);

#define MDrv_XC_GenSpecificTiming Hal_SC_IPMux_Gen_SpecificTiming
INTERFACE void MDrv_XC_GenSpecificTiming(void *pInstance, XC_Internal_TimingType timingtype);

#define MDrv_XC_InitIPForInternalTiming Hal_SC_ip_Init_for_internal_timing
INTERFACE void MDrv_XC_InitIPForInternalTiming(void *pInstance, XC_Internal_TimingType timingtype, SCALER_WIN eWindow);

INTERFACE E_APIXC_ReturnValue MDrv_SC_ForceInterlaceInPMode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_GetInterlaceInPModeStatus(void *pInstance, SCALER_WIN eWindow );

INTERFACE MS_BOOL MDrv_SC_Check_IP_Gen_Timing(void *pInstance);
INTERFACE void MDrv_XC_SetCaptureWindow (void *pInstance, MS_WINDOW_TYPE* pstCaptureWin, SCALER_WIN eWindow,  MS_BOOL bFire);
INTERFACE void MDrv_XC_ip_get_sync_status(void *pInstance, XC_IP_SYNC_STATUS *sXC_Sync_Status, E_MUX_INPUTPORT enInputPort , SCALER_WIN eWindow);
INTERFACE MS_U8 MDrv_XC_wait_output_vsync(void *pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow);
INTERFACE MS_U8 MDrv_XC_wait_input_vsync(void *pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_DelayAndWaitOutVsync(void *pInstance, MS_U32 u32DelayTime, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_HWAuto_SyncSeparate(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MApi_XC_IsAutoPositionResultReady(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MApi_XC_AutoPositionEnable(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow);
INTERFACE MS_BOOL MApi_XC_AutoPhaseEnable(void *pInstance, MS_BOOL bEnable , SCALER_WIN eWindow);
INTERFACE void    MDrv_XC_SetValidDataThreshold(void *pInstance, MS_U8 u8Threshold, SCALER_WIN eWindow );
INTERFACE MS_U16  MApi_XC_SetAutoPositionVstart(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_U16  MApi_XC_SetAutoPositionHstart(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_U16  MApi_XC_SetAutoPositionVend(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_U16  MApi_XC_SetAutoPositionHend(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_BOOL MDrv_XC_GetAutoPositionForcePixelMode(void *pInstance, SCALER_WIN eWindow);
INTERFACE void    MDrv_XC_SetAutoPositionForcePixelMode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void    MApi_XC_VsyncPolarityDetectMode(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow );
INTERFACE void    MApi_XC_TimingChangeInit(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_AUTOADC_TYPE MApi_XC_GetAverageDataFromMemory(void *pInstance, MS_WINDOW_TYPE* mem_win);
INTERFACE MS_U8   MApi_XC_CalculateVsyncTime(void *pInstance, MS_U16 u16VTotal, MS_U16 u16HPeriod);
INTERFACE void    MApi_XC_EnableIPAutoCoast(MS_BOOL bEnable);
INTERFACE void    MDrv_XC_EnableIPAutoCoast(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_U8 MDrv_XC_PCMonitor_GetVSyncTime(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MApi_XC_AutoRangeEnable(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow);
INTERFACE void MApi_XC_SetAutoRangeWindow(void *pInstance, MS_U16 u16Vstart, MS_U16 u16Hstart, MS_U16 u16Vsize, MS_U16 u16Hsize, SCALER_WIN eWindow);
////===========
////get scaler de info from sc_ip
INTERFACE MS_U16 MDrv_SC_de_hend_info(void); //NO_USE
INTERFACE MS_U16 MDrv_SC_de_vend_info(void); //NO_USE

INTERFACE void MDrv_XC_init_riu_base(MS_VIRT u32riu_base, MS_VIRT u32PMriu_base);
//INTERFACE void MApi_XC_GetFrameData(XC_ApiStatus stSrcInfo, MS_RECT_INFO eRect_Info);
INTERFACE void MDrv_SC_SetFrameDataCore(void *pInstance, MS_S16 x0, MS_S16 y0, MS_S16 x1, MS_S16 y1,
                              MS_U8 *pRect,
                              MS_U32 u32RectPitch,
                              MS_U32 u32RectSize,
                              MS_U16 u16PointSize);

INTERFACE void MApi_XC_Rerout_for_Capture_VOP_Data(void *pInstance);

INTERFACE MS_U8 MDrv_XC_WaitOutputVSync(void *pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_DisableInputSource(void *pInstance, MS_BOOL bDisable, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_IsInputSourceDisabled(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SoftwareReset(void *pInstance, MS_U8 u8Reset, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_GetSyncStatus(void *pInstance, INPUT_SOURCE_TYPE_t eCurrentSrc, XC_IP_SYNC_STATUS *sXC_Sync_Status, SCALER_WIN eWindow);
INTERFACE MS_U16 MApi_XC_GetAutoGainResult(void *pInstance, AUTO_GAIN_TYPE u8Type, MS_U8 u8DelayMS, SCALER_WIN eWindow);
INTERFACE MS_U8 MApi_XC_GetAutoGainMaxValueStatus(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MApi_XC_EuroHdtvHandler(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_DETECT_EURO_HDTV_STATUS_TYPE MDrv_XC_ip_GetEuroHdtvStatus(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MApi_XC_SetFIRDownSampleDivider(void *pInstance, MS_U8 u8Enable , MS_U16 u16OverSamplingCount , SCALER_WIN eWindow );
INTERFACE void MApi_XC_SetCaptureWindowVsize(MS_U16 u16Vsize , SCALER_WIN eWindow); //API_INTERFACE
INTERFACE void MApi_XC_SetCaptureWindowHsize(MS_U16 u16Hsize , SCALER_WIN eWindow); //API_INTERFACE
INTERFACE void MApi_XC_Set_BufferData_WithoutMutex(void *pInstance, E_XC_INPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pDstRect, void *pSrcBuf, MS_WINDOW_TYPE *pSrcRect, SCALER_WIN eWindow);
INTERFACE void MApi_XC_Get_BufferData_WithoutMutex(void *pInstance, E_XC_OUTPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pRect, void *pRectBuf, SCALER_WIN eWindow);

#define MDrv_SC_IP2_PreFilter_Enable  Hal_SC_IP2_PreFilter_Enable
INTERFACE void MDrv_SC_IP2_PreFilter_Enable(void *pInstance, MS_BOOL bEnable);
INTERFACE void MApi_XC_InitRegInTimingChange_Post(void *pInstance, MS_U8 u8CurrentSyncStatus, SCALER_WIN eWindow);
INTERFACE MS_BOOL MApi_XC_GetDstInfo_IP1(void *pInstance, MS_XC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_GetDEWidthHeightInDEByPassMode(void* pInstance, MS_U16* pu16Width, MS_U16* pu16Height, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_SC_Get_DE_Bypass_Mode(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_GetSkipOPWriteOffInSetWindow(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetSkipDisableOPWriteOffInFPLL(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);

INTERFACE void MDrv_XC_SetVGASogEn(void *pInstance, MS_BOOL bVGASogEn);
INTERFACE MS_BOOL MDrv_XC_GetVGASogEn(void *pInstance);
INTERFACE void MDrv_XC_SetSogDetectStatus(void *pInstance, MS_U8  u8SogDetectStatus);
INTERFACE MS_U8  MDrv_XC_GetSogDetectStatus(void *pInstance);

INTERFACE EN_FORCEi_SUPPORT_TYPE MDrv_XC_GetForceiSupportType(void *pInstance);
INTERFACE MS_U32 MDrv_XC_CalculateVFreqx1K(void *pInstance, MS_U32 u16HFreq, MS_U16 u16VTotal);
INTERFACE MS_U16 MDrv_XC_CalculateVFreqx10(void *pInstance, MS_U16 u16HFreq, MS_U16 u16VTotal);
INTERFACE MS_U32 MDrv_XC_GetAccurateVFreqx1K(void* pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_CalculateHFreqx1K(void* pInstance, MS_U16 u16HPeriod);
INTERFACE MS_U16 MDrv_XC_CalculateHFreqx10(void* pInstance, MS_U16 u16HPeriod);
INTERFACE void MDrv_XC_SetCaptureWindowHstart(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetCaptureWindowVstart(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetCaptureWindowHstart_with_Menuload(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetCaptureWindowVstart_with_Menuload(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow);
INTERFACE void MDrv_XC_GetDEWindow(void *pInstance, MS_WINDOW_TYPE *psWin, SCALER_WIN eWindow);

INTERFACE MS_BOOL MDrv_XC_PCMonitor_Init(void *pInstance, MS_U8 u8MaxWindowNum);
INTERFACE XC_PCMONITOR_HK_INFO MDrv_XC_PCMonitor_GetStatus(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_PCMonitor_SetStatus(void *pInstance, XC_PCMONITOR_HK_INFO sPCMONITOR_HK_Info, SCALER_WIN eWindow);
INTERFACE MS_U8 MDrv_XC_PCMonitor_GetStableCounter(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8 MDrv_XC_PCMonitor_GetNoSyncCounter(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_XC_PCMonitor_Get_VFreqx10(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_XC_PCMonitor_Get_HFreqx10(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_PCMonitor_Get_HFreqx1K(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_XC_PCMonitor_Get_VFreqx1K(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 MDrv_XC_PCMonitor_Get_Vtotal(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_PCMonitor_Restart(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(void *pInstance, SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin);
INTERFACE MS_BOOL MDrv_XC_PCMonitor_SyncLoss(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_PCMonitor_InvalidTimingDetect(void *pInstance, MS_BOOL bPollingOnly, SCALER_WIN eWindow);
INTERFACE MS_U32 MDrv_SC_ip_get_verticalperiod(void *pInstance, SCALER_WIN eWindow);
//INTERFACE EN_FORCEi_SUPPORT_TYPE MDrv_XC_GetForceiSupportType(void *pInstance); //re-declaration
INTERFACE void MDrv_XC_SetHVSyncStatusCheck(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void MDrv_XC_SetNewModeInterlacedDetect(void *pInstance, MS_BOOL bEnable, MS_U8 u8Source, MS_BOOL bVttByPixelClk, MS_BOOL bVInvert, MS_BOOL bHInvert, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_IsPNLYUVOutput(void *pInstance);
INTERFACE MS_BOOL MDrv_XC_Is2K2KToFrc(void *pInstance);
INTERFACE MS_U32 MDrv_XC_PCMonitor_Get_FrameRatex1K(void* pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_OPTEE_Mux(void *pInstance,EN_XC_OPTEE_ACTION action,XC_OPTEE_MUX_DATA xc_mux_data);
INTERFACE MS_BOOL MDrv_XC_OPTEE_Control(void *pInstance,EN_XC_OPTEE_ACTION action,XC_OPTEE_HANDLER* xc_handler);
INTERFACE MS_BOOL MDrv_XC_OPTEE_Update(void *pInstance,EN_XC_OPTEE_ACTION action,XC_OPTEE_HANDLER xc_handler);

#undef INTERFACE
#endif /* _MDRV_SC_IP_H_ */
