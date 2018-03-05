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

#ifndef SC_DIP_H
#define SC_DIP_H
////===========================>>SW DB Buffer
#ifdef DRV_SC_DIPSCALING_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define ALIGN_CHECK(value,factor) ((value + factor-1) & (~(factor-1)))
#define DIP_BYTE_PER_WORD       16UL
#define DIP_HVD_TILE_BLOCK_H    16UL
#define DIP_HVD_TILE_BLOCK_V    32UL
#define DWIN_W_LIMITE_OFT       31UL
#define DWIN_BYTE_PER_WORD      32UL

typedef struct
{
    MS_U16 u16H_CapStart;
    MS_U16 u16H_CapSize;
    MS_U16 u16V_CapStart;
    MS_U16 u16V_CapSize;

    MS_U32 u32H_PreScalingRatio;
    MS_U32 u32V_PreScalingRatio;

    MS_U16 u16DNROffset;
    MS_U16 u16DNRFetch;
}SC_DIP_SWDB_INFO, *P_SC_DIP_SWDB_INFO;

/// DWIN scan mode
typedef enum
{
    GOPDWIN_SCAN_MODE_AUTO = 0,
    GOPDWIN_SCAN_MODE_PROGRESSIVE = GOPDWIN_SCAN_MODE_AUTO,  ///< Progressive scan
    GOPDWIN_SCAN_MODE_INTERLACE,  ///< Interlace scan
    GOPDWIN_SCAN_MODE_MAX
} EN_XC_DWIN_SCAN_TYPE;

/// DWIN source data format
typedef enum
{
    /// YCrYCb.
    XC_DWIN_DATA_FMT_YUV422 = 0,
    /// RGB domain
    XC_DWIN_DATA_FMT_RGB565,
    /// RGB domain
    XC_DWIN_DATA_FMT_ARGB8888,
    /// YUV420 HVD tile fmt
    XC_DWIN_DATA_FMT_YUV420,
    /// YC separate 422
    XC_DWIN_DATA_FMT_YC422,
    /// YUV420 H265 tile fmt
    XC_DWIN_DATA_FMT_YUV420_H265,
    /// YUV420 H265 10 bits tile fmt
    XC_DWIN_DATA_FMT_YUV420_H265_10BITS,
    /// YUV420 planer
    XC_DWIN_DATA_FMT_YUV420_PLANER,
    /// YUV420 semi planer
    XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER,
    XC_DWIN_DATA_FMT_MAX
} EN_DRV_XC_DWIN_DATA_FMT;

/// DIP OP-TEE Operation mode
typedef enum
{
    /// unLock_output.
    XC_DIP_OPMODE_UNLOCK_OUTPUT = 0,
    /// unLock_output.
    XC_DIP_OPMODE_LOCK_OUTPUT = 1,
    /// Enable.
    XC_DIP_OPMODE_ENABLE = 2,
    /// Disable.
    XC_DIP_OPMODE_DISABLE = 3,
} EN_DRV_XC_DIP_OPERATIONMODE;

typedef struct
{
    MS_U32 u32PipeID;
    MS_U32 u32SecureDMA;
    MS_U32 u32OperationMode;
    MS_U32 u32ReturnValue;
}DIP_CONFIG_PIPE;

INTERFACE SC_DIP_SWDB_INFO stDIP_DBreg[MAX_DIP_WINDOW];         ///< SW double buffer

void MDrv_XC_DIP_SWReset(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_Init(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CMDQ_SetAction(void* pInstance,EN_XC_DIP_CMDQ_ACTION eAction, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CMDQ_SetOperations(void* pInstance, cmd_mload_utopia_interface *pOps, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);

void MDrv_XC_DWIN_init_swdb(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_sw_db(void* pInstance,P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_SetInputSource(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_GetInputSource(void* pInstance,INPUT_SOURCE_TYPE_t *enInputSourceType,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_DisableInputSource(void* pInstance,MS_BOOL bDisable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_SetFrameBufferAddress(void* pInstance,MS_PHY u64FBAddress, MS_PHY u64FBSize, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_v_start(void* pInstance,MS_U16 u16Vstart , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_h_start(void* pInstance,MS_U16 u16Vstart , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_v_size(void* pInstance,MS_U16 u16Vsize , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_h_size(void* pInstance,MS_U16 u16Hsize , SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_capture_window(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_get_capture_window(void* pInstance,MS_WINDOW_TYPE* capture_win,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_get_DE_Window(void* pInstance,MS_WINDOW_TYPE* psWin,SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_EnableR2YCSC(void* pInstance,MS_BOOL benable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_delayline(void* pInstance,MS_U8 u8DelayLines, SCALER_DIP_WIN eWindow );
void MDrv_XC_DWIN_set_prescaling_ratio(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_set_storeFrameNUM(void* pInstance,SCALER_DIP_WIN eWindow,MS_U8 u8FrameNum);
void MDrv_XC_DWIN_set_fetch_number_limit(void* pInstance,XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_EnableIPAutoCoast(void* pInstance,MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_Scaler_Field_Detect(void* pInstance,INPUT_SOURCE_TYPE_t enInputSrcType, EN_XC_DWIN_SCAN_TYPE enScanType, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_Disable_IPM_ReadWriteRequest(void* pInstance,MS_BOOL bDisable, SCALER_DIP_WIN eWindow);
void MDrv_XC_DWIN_SetMode(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow);
//==============Set===================
void MDrv_XC_DIP_SetWindow(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetFRC(void* pInstance,MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetWinProperty(void* pInstance,ST_XC_DIP_WINPROPERTY *pstDIPWinProperty, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CaptureOneFrame(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_CaptureOneFrame2(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_EnableIntr(void* pInstance,MS_U16 u8mask, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_ClearIntr(void* pInstance,MS_U16 u16mask,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_EnableCaptureStream(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SelectSourceScanType(void* pInstance,EN_XC_DWIN_SCAN_TYPE enScan,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetInterlaceWrite(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetDataFmt(void* pInstance,EN_DRV_XC_DWIN_DATA_FMT fmt,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetAlphaValue(void* pInstance,MS_U8 u8AlphaVal,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetY2R(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetUVSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetYCSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetRGBSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetOutputCapture(void* pInstance,MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetMirror(void* pInstance,MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetDIPRProperty(void* pInstance,ST_XC_DIPR_PROPERTY *pstDIPRProperty, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetDIPRProperty_EX(void* pInstance,ST_XC_DIPR_PROPERTY_EX *pstDIPRProperty_ex, SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_InterruptAttach(void* pInstance,InterruptCb pIntCb,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_InterruptDetach(void* pInstance,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetRotation(void* pInstance,MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,SCALER_DIP_WIN eTmpWindow);
void MDrv_XC_DIP_SetPinpon(void* pInstance,MS_BOOL bPinpon,MS_PHY u64PinponAddr,SCALER_DIP_WIN eWindow);
MS_BOOL MDrv_XC_DIP_SetHVSP(void* pInstance,MS_BOOL bSelect, SCALER_DIP_WIN eWindow);
MS_BOOL MDrv_XC_DIP_Set420TileBlock(void* pInstance,EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow);
void MDrv_XC_DIP_SetBdgLevel(EN_XC_DIP_DEBUG_LEVEL level);
#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
void MDrv_XC_DIP_Mdb_Cmdline(MS_U64* u64ReqHdl,MS_U32 u32CmdSize,char *pcCmdLine);
void MDrv_XC_DIP_Mdb_GetInfo(MS_U64* u64ReqHdl);
#endif
//==============Get===================
MS_U16 MDrv_XC_DIP_GetIntrStatus(void* pInstance,SCALER_DIP_WIN eWindow);
BUFFER_INFO MDrv_XC_DIP_GetBufferInfo(void* pInstance,SCALER_DIP_WIN eWindow);
//==============OP TEE===================
MS_U32 MDrv_XC_DIP_ConfigPipe_U2(void* pInstance,MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
#undef INTERFACE
#endif /* DRV_SC_SCALING_H */

