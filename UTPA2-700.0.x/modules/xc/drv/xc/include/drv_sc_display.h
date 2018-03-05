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

#ifndef MDRV_SC_DISPLAY_H
#define MDRV_SC_DISPLAY_H

typedef struct
{
    MS_U16 u16Htt_out;
    MS_U16 u16Vtt_out;
}XC_DISPLAYOUT_INFO;

//-----------------------------------------------------------------------------
//  below definitions are for FSM FPLL
//-----------------------------------------------------------------------------
typedef enum
{
    FPLL_IDLE_STATE,
    FPLL_INIT_STATE,
    FPLL_ATV_INIT_STATE,
    FPLL_WAIT_PRD_LOCK_STATE,
    FPLL_WAIT_PHASE_STABLE_STATE,
    FPLL_CHECK_FRAME_LOCK_STATE,
    FPLL_LOCKED_STATE,
    FPLL_VD_LOCKED_STATE,
    FPLL_WAIT_LPLL_OFS_STATE,
    FPLL_WAIT_IP_STABLE_STATE,
    FPLL_DEATTACH_ISR_STATE,
    FPLL_FREQ_ONLY_LOCK_STATE,
    FPLL_LOCK_ATV_ADJUST_IVS_STATE,
    FPLL_LOCK_ATV_IVS_OVS_STATE,
    FPLL_FREQ_ONLY_LOCK_WAIT_PHASE_STABLE_STATE,
} EN_FPLL_FSM;

typedef enum
{
    FPLL_THRESH_MODE_IDLE_STATE,
    FPLL_THRESH_MODE_LOCKED_STATE,
    FPLL_THRESH_MODE_DEATTACH_ISR_STATE,
} EN_FPLL_THRESH_MODE;

#ifdef UFO_XC_AUTO_DOWNLOAD
typedef struct
{
    MS_PHY phyBaseAddr;                 /// baseaddr
    MS_U32 u32Size;                     /// size
    MS_BOOL bEnable;                    /// enable/disable the client
    EN_XC_AUTODOWNLOAD_MODE enMode;     /// work mode
    MS_PHY u32StartAddr;                /// sram start addr
    MS_U32 u32Depth;                    /// data length
} XC_AUTODOWNLOAD_CLIENT_INFO;
#endif

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
typedef enum
{
    E_XC_INPUT_SOURCE_TYPE_NONE,
    E_XC_INPUT_SOURCE_TYPE_HDMI,
    E_XC_INPUT_SOURCE_TYPE_OTT,
    E_XC_INPUT_SOURCE_TYPE_MAX,
} EN_XC_INPUT_SOURCE_TYPE;
#endif
#endif

#ifdef ENABLE_SPREADMODE
typedef enum
{
    FPLL_SPREAD_MODE_NONE,
    FPLL_SPREAD_MODE_1_2,
    FPLL_SPREAD_MODE_2_5,
    FPLL_SPREAD_MODE_3_5,
    FPLL_SPREAD_MODE_4_5,
    FPLL_SPREAD_MODE_5_6,
    FPLL_SPREAD_MODE_5_12,
    FPLL_SPREAD_MODE_12_25,
    FPLL_SPREAD_MODE_24_25,

} EN_FPLL_SPREAD_MODE;
#endif

MS_U16 MDrv_SC_set_frc(MS_U16 u16InputVFreqX10); //NO_USE
void MDrv_SC_init_lpll(XC_PANEL_INFO* pstPanel_Info /* in */); //NO_USE
void MDrv_SC_set_fpll(void *pInstance, MS_BOOL bEnable, MS_BOOL bEnablePhaseLock, MS_U8 u8Lpll_bank);
void MDrv_SC_clear_lpll(void *pInstance);
void MDrv_SC_monitor_lpll(void *pInstance);
void MDrv_SC_set_fastlpll(void *pInstance, MS_BOOL bEn);
MS_BOOL MDrv_SC_get_fastlpll(void *pInstance);
MS_BOOL MDrv_Scaler_GetFPLLDoneStatus(void *pInstance);

void MDrv_SC_set_freerun_window(void *pInstance);
void MDrv_XC_SetFreeRunTiming (void *pInstance);
void MDrv_SC_GenerateBlackVideo(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void MDrv_SC_GenerateBlackVideoForBothWin(void *pInstance, MS_BOOL bEnable);
void MDrv_SC_Set_BLSK(void *pInstance, MS_BOOL bEnable);

void MDrv_SC_set_output_dclk(void *pInstance, MS_U16 u16Htt, MS_U16 u16Vtt, MS_U16 u16VFreq, MS_U32 u32HighAccurateVFreq, SCALER_WIN eWindow);
void MDrv_SC_set_video_dark(MS_BOOL bEnable, MS_U16 u16VideoDarkLevel); //NO_USE
void MDrv_Scaler_set_de_window(XC_PANEL_INFO *pPanel_Info); //NO_USE
void MDrv_SC_set_csc(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow );
void MDrv_SC_rep_window(void *pInstance, MS_BOOL bEnable,MS_WINDOW_TYPE Win,MS_U8 u8Color);
void MDrv_XC_set_dispwin_to_reg(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin);
void MDrv_XC_get_dispwin_from_reg(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin);
void MDrv_XC_UpdateWindow(void *pInstance, MS_WINDOW_TYPE *pDEWin, MS_WINDOW_TYPE *pMainWin, MS_WINDOW_TYPE *pSubWin, MS_WINDOW_TYPE *pReportWin, MS_WINDOW_TYPE *pNewOSDWin);
MS_U16 MDrv_SC_GetOutputVFreqX100(void *pInstance);
MS_U16 MDrv_SC_GetOP1OutputVFreqX100(void *pInstance);

void MDrv_SC_SetDisplay_LineBuffer_Mode(void *pInstance, MS_BOOL bEnable);
void MDrv_SC_SetDisplay_Start_Mode(void *pInstance, MS_BOOL bEnable);
void MDrv_XC_SetOSD2VEmode(void *pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX);

void MDrv_XC_FPLLCusReset(void *pInstance);

void MDrv_XC_GetDefaultHVSyncInfo(void *pInstance);
void MDrv_SC_GetCurrentFRCTable(SC_FRC_SETTING* pstFRCTable);
MS_BOOL MDrv_XC_FPLL_IsSupportLockFreqOnly(void *pInstance);

MS_BOOL MDrv_XC_Get_OPWriteOffEnable(void *pInstance, SCALER_WIN eWindow);
void MDrv_XC_SetMWEStatus(void *pInstance, MS_BOOL bMWEStatus);
MS_BOOL MDrv_XC_GetMWEStatus(void *pInstance);
void MDrv_XC_DisableSubWindow(void); //NO_USE
void MDrv_XC_EnableCLK_for_SUB(void *pInstance, MS_BOOL bEnable);
void MDrv_XC_EnableWindow(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);

E_APIXC_ReturnValue MDrv_XC_SetOSDLayer(void *pInstance, E_VOP_OSD_LAYER_SEL eVOPOSDLayer, SCALER_WIN eWindow);
E_VOP_OSD_LAYER_SEL MDrv_XC_GetOSDLayer(void *pInstance, SCALER_WIN eWindow);
E_APIXC_ReturnValue MDrv_XC_SetVideoAlpha(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
E_APIXC_ReturnValue MDrv_XC_GetVideoAlpha(void *pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow);

E_APIXC_ReturnValue MDrv_SC_ShiftDisplayWindowDirectly(void *pInstance, MS_S8 s8H, MS_S8 s8V, MS_S8 s8preH, MS_S8 s8preV);
E_APIXC_ReturnValue MDrv_XC_Combine_MLoadEn(void *pInstance, MS_BOOL bEn);
#ifdef UFO_XC_ZORDER
E_APIXC_ReturnValue MDrv_SC_GetVideoOnOSD(void *pInstance, E_VIDEO_ON_OSD_LAYER *penLayer, SCALER_WIN eWindow);
#endif
E_APIXC_ReturnValue MDrv_SC_SetVideoOnOSD(void *pInstance, E_VIDEO_ON_OSD_LAYER enlayer, SCALER_WIN eWindow);
E_APIXC_ReturnValue MDrv_SC_SetOSDLayerBlending(void *pInstance, MS_U8 u8Layer, MS_BOOL bEnable, SCALER_WIN eWindow);
E_APIXC_ReturnValue MDrv_SC_SetOSDLayerAlpha(void *pInstance, MS_U8 u8Layer, MS_U8 u8Alpha);
E_APIXC_ReturnValue MDrv_SC_SetOSDBlendingFormula(void *pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow);

void MDrv_SC_SetMCDIBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, E_XC_MCDI_TYPE eType);
void MDrv_SC_EnableMCDI(void *pInstance, MS_BOOL bEnable, E_XC_MCDI_TYPE eType);
void MDrv_XC_3D_AdjustOSDVDE(void *pInstance, MS_BOOL bEnable, MS_U16 VStart, MS_U16 VEnd, MS_U16 HStart, MS_U16 HEnd);

E_APIXC_ReturnValue MDrv_SC_ForceReadFrame(void *pInstance, MS_BOOL bEnable, MS_U16 u16FrameIndex);
MS_BOOL MDrv_SC_IsForceFreerun(void *pInstance);
MS_BOOL MDrv_XC_SetExPanelInfo(void *pInstance, MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx);

#define MDrv_XC_Is_SubWindowEanble Hal_SC_Is_subwindow_enable
MS_BOOL MDrv_XC_Is_SubWindowEanble(void *pInstance);
void MDrv_XC_SetFrameColor(void *pInstance, MS_U32 u32aRGB );
void MDrv_XC_SetDispWindowColor(void *pInstance, MS_U8 u8Color, SCALER_WIN eWindow);
void MDrv_XC_Enable_Extra_Request(void *pInstance, MS_BOOL bEnable);
void MDrv_SC_EnableReadBankSwitchControl(void *pInstance, MS_BOOL bEnable, MS_U16 u16SwitchCnt, SCALER_WIN eWindow);
void MDrv_SC_Set_LB_MergeAddress(void *pInstance);
E_APIXC_ReturnValue MDrv_XC_SetPixelShift(void* pInstance, MS_S8 s8H, MS_S8 s8V);

void MDrv_SC_Get_GOPOffset(MS_U16 *u16OffsetH, MS_U16 *u16OffsetV);

#ifdef UFO_XC_AUTO_DOWNLOAD
E_APIXC_ReturnValue MDrv_XC_AutoDownload_Config(void* pInstance, XC_AUTODOWNLOAD_CONFIG_INFO *pstConfigInfo);
E_APIXC_ReturnValue MDrv_XC_AutoDownload_Write(void* pInstance, XC_AUTODOWNLOAD_DATA_INFO *pstDataInfo);
E_APIXC_ReturnValue MDrv_XC_AutoDownload_Fire(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient);
E_APIXC_ReturnValue MDrv_XC_GetAutoDownloadCaps(EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_BOOL *pbSupported);
#endif

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
E_APIXC_ReturnValue MDrv_XC_HDR_Control(void* pInstance, EN_XC_HDR_CTRL_TYPE enCtrlType, void *pParam);
E_APIXC_ReturnValue MDrv_XC_HDR_SetInputSource(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType);
E_APIXC_ReturnValue MDrv_XC_HDR_SetWindow(void* pInstance, MS_WINDOW_TYPE stCapWin);
E_APIXC_ReturnValue MDrv_XC_HDR_GetCaps(void *pInstance, XC_HDR_SUPPORTED_CAPS *pstHDRCaps);
#endif
#endif

#ifdef UFO_XC_SETBLACKVIDEOBYMODE
E_APIXC_ReturnValue MDrv_SC_GenerateBlackVideoByMode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow, EN_XC_BLACK_VIDEO_MODE eMode);
#endif

MS_BOOL MDrv_XC_SetSWDRInfo(void* pInstance, XC_SWDR_INFO *pSWDR_INFO);
MS_BOOL MDrv_XC_GetSWDRInfo(void* pInstance, XC_SWDR_INFO *pSWDR_INFO);
#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
MS_U32 MDrv_SC_GetHDMIPolicy(void *pInstance);
#endif
#undef INTERFACE
#endif /* MDRV_SC_DISPLAY_H */
