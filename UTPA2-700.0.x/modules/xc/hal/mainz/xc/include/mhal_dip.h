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
//==============================================================================
// [mhal_dip.h]
// Date: 20120208
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_DIP_H
#define MHAL_DIP_H

#include "hwreg_sc.h"

//==============================================================================
// Scaling Ratio Macro

// H_PreScalingDownRatio() was refined to reduce the calculation error.
// Use round up (x+y/2)/y might reduce the scaling ratio and induce right vertical garbage line.
// So use un-conditional add by 1 (x+y)/y.
#define H_PreScalingDownRatioDIP(Input, Output)                ( (((MS_U32)(Output)) * 1048576ul)/ (Input) + 1 )
#define V_PreScalingDownRatioDIP(Input, Output)                ( (((MS_U32)(Output)) * 1048576ul)/ (Input) + 1 )  // CB mode

#define DIP_CHIP_CAP(eWindow,_ret)     do{    \
                                            switch(eWindow){                \
                                                case DIP_WINDOW:            \
                                                    _ret = DIP_CAP_EXIST|   \
                                                           DIP_CAP_420TILE| \
                                                           DIP_CAP_MIRROR|  \
                                                           DIP_CAP_R2Y|     \
                                                           DIP_CAP_OP1_CAPTURE_V2|\
                                                           DIP_CAP_SCALING_DOWN; \
                                                    break;                  \
                                                default:                    \
                                                    _ret = 0;               \
                                                    break;                  \
                                            }                               \
                                        }while(0)

#define GOPD_DIP

typedef enum
{
	E_XC_DIP_SOURCE_TYPE_SUB2,
	E_XC_DIP_SOURCE_TYPE_SUB,
	E_XC_DIP_SOURCE_TYPE_MAIN,
	E_XC_DIP_SOURCE_TYPE_OP,
} MS_XC_DIP_SOURCE_TYPE;


//==============================================================================

// Debug Log
#include "ULog.h"

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define DIP_H_INFO(x, args...) ULOGI("DIP HAL", x, ##args)
// Warning, illegal paramter but can be self fixed in functions
#define DIP_H_WARN(x, args...) ULOGW("DIP HAL", x, ##args)
//  Need debug, illegal paramter.
#define DIP_H_DBUG(x, args...) ULOGD("DIP HAL", x, ##args)
// Error, function will be terminated but system not crash
#define DIP_H_ERR(x, args...) ULOGE("DIP HAL", x, ##args)
// Critical, system crash. (ex. assert)
#define DIP_H_FATAL(x, args...) ULOGF("DIP HAL", x, ##args)

//==============================================================================
#ifdef MHAL_DIP_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#define Hal_SC_DWIN_set_ficlk(args...)
#define Hal_SC_DWIN_set_wr_bank_mapping(args...)
#define Hal_SC_DWIN_set_csc(args...)
#define Hal_SC_DWIN_set_delayline(args...)
#define Hal_SC_DWIN_sw_db_burst(args...)
#define Hal_SC_DWIN_Set_FB_Num(args...)
#define Hal_SC_DWIN_get_memory_bit_fmt(args...) 0
#define Hal_SC_DWIN_set_capture_v_start(args...)
#define Hal_SC_DWIN_set_capture_h_start(args...)
#define Hal_SC_DWIN_set_capture_v_size(args...)
#define Hal_SC_DWIN_set_capture_h_size(args...)
#define Hal_SC_DWIN_get_capture_window(args...)
#define Hal_SC_DWIN_set_miusel(args...)
#define Hal_SC_DWIN_set_memoryaddress(args...)
#define Hal_SC_DWIN_DisableInputSource(args...)
#define Hal_SC_DWIN_SetInputSource(args...)
#define Hal_SC_DWIN_software_reset(args...)
#define Hal_SC_DWIN_setfield(args...)
#define Hal_SC_DWIN_set_image_wrap(args...)
#define Hal_SC_DWIN_set_input_sync_reference_edge(args...)
#define Hal_SC_DWIN_set_input_vsync_delay(args...)
#define Hal_SC_DWIN_set_de_only_mode(args...)
#define Hal_SC_DWIN_set_input_10bit(args...)
#define Hal_SC_DWIN_set_fir_down_sample_divider(args...)
#define Hal_SC_DWIN_set_coast_input(args...)
#define Hal_SC_DWIN_set_coast_window(args...)
#define Hal_SC_DWIN_set_input_sync_sample_mode(args...)
#define Hal_SC_DWIN_set_post_glitch_removal(args...)
#define Hal_SC_DWIN_set_DE_Mode_Glitch(args...)
#define Hal_SC_DWIN_set_de_bypass_mode(args...)
#define Hal_SC_DWIN_set_ms_filter(args...)
#define Hal_SC_DWIN_Disable_IPM_ReadWriteRequest(args...)
#define Hal_SC_DWIN_get_de_window(args...)
#define Hal_SC_DWIN_EnableIPAutoCoast(args...)
INTERFACE void Hal_SC_DWIN_set_422_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_DIP_WIN eWindow);
INTERFACE XC_FRAME_STORE_NUMBER Hal_SC_DWIN_GetFrameStoreMode(void *pInstance, SCALER_DIP_WIN eWindow,MS_BOOL bInterlace);
INTERFACE void Hal_SC_DWIN_EnableR2YCSC(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_Set_vsd_output_line_count(void *pInstance, MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_Set_vsd_input_line_count(void *pInstance, MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_sw_db(void *pInstance, P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow);
INTERFACE void Hal_SC_DWIN_set_input_vsync_inverse(void *pInstance, MS_BOOL bEnable, SCALER_DIP_WIN eWindow);
//==============Set===================
INTERFACE MS_U16 HAL_XC_DIP_GetBusSize(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SWReset(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Init(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetBase0(void *pInstance, MS_PHY u64BufStart,MS_PHY u64BufEnd,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetBase1(void *pInstance, MS_PHY u64BufStart,MS_PHY u64BufEnd,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetWinProperty(void *pInstance, MS_U8 u8BufCnt,MS_U16 u16Width,MS_U16 u16LineOft,MS_U16 u16Height,MS_PHY u64OffSet, SCALER_DIP_SOURCE_TYPE eSource,MS_BOOL bPIP,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetWinProperty1(void *pInstance, MS_PHY u64OffSet,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetMiuSel(void *pInstance, MS_U8 u8MIUSel,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_CpatureOneFrame(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_CpatureOneFrame2(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_ClearIntr(void *pInstance, MS_U16 u16mask,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_EnableCaptureStream(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_EnableIntr(void *pInstance, MS_U16 u8mask, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDataFmt(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fmt,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SelectSourceScanType(void *pInstance, EN_XC_DWIN_SCAN_TYPE enScan,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetAlphaValue(void *pInstance, MS_U8 u8AlphaVal,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetUVSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetInterlaceWrite(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetY2R(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetYCSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetRGBSwap(void *pInstance, MS_BOOL bEnable,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetOutputCapture(void *pInstance, MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Set444to422(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fmt,MS_BOOL bSrcYUVFmt,MS_BOOL bSrcFmt422,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetFRC(void *pInstance, MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetMirror(void *pInstance, MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_SetDIPRProperty(void *pInstance, ST_XC_DIPR_PROPERTY *pstDIPRProperty, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_InterruptAttach(void *pInstance, InterruptCb pIntCb,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_InterruptDetach(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_MuxDispatch(void *pInstance, SCALER_DIP_SOURCE_TYPE eSource,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Rotation(void *pInstance, MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,MS_PHY u64StartAddr,SCALER_DIP_WIN eTmpWindow);
INTERFACE void HAL_XC_DIP_SetPinpon(void *pInstance, MS_BOOL bPinpon,MS_PHY u64PinponAddr,MS_PHY u64OffSet,SCALER_DIP_WIN eWindow);
INTERFACE SCALER_DIP_WIN HAL_XC_DIP_GetHVSP(void* pInstance);
INTERFACE MS_BOOL HAL_XC_DIP_SetHVSP(void* pInstance,MS_BOOL bSelect, SCALER_DIP_WIN eWindow);
INTERFACE MS_BOOL HAL_XC_DIP_Set420TileBlock(void* pInstance,EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_Enable(void *pInstance, MS_BOOL bEnable);
//==============Get===================
INTERFACE MS_U8 HAL_XC_DIP_GetBufCnt(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE MS_U16 HAL_XC_DIP_GetBPP(void *pInstance, EN_DRV_XC_DWIN_DATA_FMT fbFmt,SCALER_DIP_WIN eWindow);
INTERFACE MS_U16 HAL_XC_DIP_WidthAlignCheck(void *pInstance, MS_U16 u16Width,MS_U16 u16Bpp,SCALER_DIP_WIN eWindow);
INTERFACE MS_U16 HAL_XC_DIP_GetIntrStatus(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE MS_BOOL HAL_XC_DIP_GetInterlaceWrite(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE EN_DRV_XC_DWIN_DATA_FMT HAL_XC_DIP_GetDataFmt(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE EN_XC_DWIN_SCAN_TYPE HAL_XC_DIP_GetSourceScanType(void *pInstance, SCALER_DIP_WIN eWindow);
INTERFACE void HAL_XC_DIP_2P_Width_Check(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_DIP_WIN eWindow);

#undef INTERFACE
#endif /* MHAL_DIP_H */
