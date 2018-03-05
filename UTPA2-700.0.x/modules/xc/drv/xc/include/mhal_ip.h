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
// (!¡±MStar Confidential Information!¡L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [mhal_ip.h]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_IP_H
#define MHAL_IP_H

#include "hwreg_sc.h"

//==============================================================================
//==============================================================================

#define MST_H_PERIOD_MASK           0x3FFF
#define MST_V_TOTAL_MASK            0x1FFF

//==============================================================================
//==============================================================================
#ifdef MHAL_IP_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//for BK01_0x03[4:3]
#define LEADING_EDGE    0
#define TAILING_EDGE    1

//for BK01_0x03[5]
#define DELAY_QUARTER_HSYNC 0
#define NO_DELAY            1

//for BK01_0x24[5]
#define PC_SOURCE           0
#define COMPONENT_VIDEO     1

//for BK01_0x26[2]
#define NORMAL_MODE     0
#define GLITCH_REMOVAL  1

//pure color ip pattern value
#define PURE_COR_IP_PATTERN_VALUE 0x1841

// Force interlace mode for HDMI by HW/SW/Auto
typedef enum
{
    ForceI_HW,    //<<< set REG_SC_BK12_05[10] = 1'b1, then HW will do it.
    ForceI_SW,    //<<< Need to adjust Vtt/Capture..
    ForceI_Auto,    //<<< don't have to do anything , HW auto do
} EN_FORCEi_SUPPORT_TYPE;

INTERFACE MS_U16 Hal_SC_ip_de_hstart_info(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_ip_de_hend_info(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_U16 Hal_SC_ip_de_vstart_info(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_ip_de_vend_info(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_U8 Hal_SC_ip_get_sync_flag(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_U8 Hal_SC_ip_get_input_vsync_polarity(void *pInstance);
INTERFACE MS_U8 Hal_SC_ip_get_input_vsync_value(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_ip_get_output_vsync_value(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_U8 Hal_SC_ip_get_sync_detect_status(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_U8 Hal_SC_ip_get_interlace_status (void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_ms_filter(void *pInstance, MS_BOOL bEnable, MS_U16 u16FilterRange, SCALER_WIN eWindow );
INTERFACE void Hal_SC_ip_set_coast_window(void *pInstance, MS_U8 u8Start, MS_U8 u8End, SCALER_WIN eWindow);

INTERFACE void Hal_SC_ip_sog_detect(void *pInstance);


INTERFACE void Hal_SC_IPMux_Gen_SpecificTiming(void *pInstance, XC_Internal_TimingType timingtype );  //scaler 480p timing setting
INTERFACE MS_U16  Hal_SC_ip_get_h_position(void); //NO_USE
INTERFACE MS_U16  Hal_SC_ip_get_v_position(void); //NO_USE
INTERFACE void Hal_SC_ip_get_capture_window(void *pInstance, MS_WINDOW_TYPE* capture_win, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_capture_v_start(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_capture_h_start(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_capture_v_start_with_Menuload(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_capture_h_start_with_Menuload(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_capture_v_size(void *pInstance, MS_U16 u16Vsize , SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_capture_h_size(void *pInstance, MS_U16 u16Hsize , SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_fir_down_sample_divider(void *pInstance, MS_U8 u8Enable, MS_U16 u16OverSampleCount,SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_XC_MApi_XC_IPAutoNoSignal_GetCaps(void); //NO_NEED
INTERFACE void Hal_SC_ip_set_IPAutoNoSignal(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow );
INTERFACE MS_BOOL Hal_SC_ip_get_IPAutoNoSignal(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_enable_turnoff_OP1_for_AutoNoSignal(void *pInstance, MS_BOOL bEnable);
INTERFACE void Hal_SC_ip_set_auto_gain_function(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_ip_is_auto_gain_result_ready(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_ip_auto_gain_min_value_status(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_ip_auto_gain_max_value_status(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_ip_is_auto_phase_result_ready(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_ip_get_auto_phase_value(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_ip_is_auto_position_result_ready(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_auto_position_function(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_valid_data_threshold( void *pInstance, MS_U8 u8Threshold, SCALER_WIN eWindow );
INTERFACE MS_U16 Hal_SC_ip_get_auto_position_v_start(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_U16 Hal_SC_ip_get_auto_position_h_start(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_U16 Hal_SC_ip_get_auto_position_v_end(void *pInstance, SCALER_WIN eWindow );
INTERFACE MS_U16 Hal_SC_ip_get_auto_position_h_end(void *pInstance, SCALER_WIN eWindow );
INTERFACE void Hal_SC_ip_set_auto_range_window(void *pInstance, MS_U16 u16Vstart, MS_U16 u16Hstart, MS_U16 u16Vsize , MS_U16 u16Hsize, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_set_auto_range_function(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow );
INTERFACE void Hal_SC_ip_set_reg_usr_vspolmd(void *pInstance, MS_U8 u8Enable, SCALER_WIN eWindow );
INTERFACE void Hal_SC_ip_init_reg_in_timingchange(void *pInstance, SCALER_WIN eWindow);

INTERFACE MS_BOOL HAL_SC_set_GOP_Enable(void *pInstance, MS_U8 MaxGOP ,MS_U8 UseNum, MS_U8 u8MuxNum, MS_BOOL bEnable);
INTERFACE void HAL_SC_ip_sel_for_gop(void *pInstance, MS_U8 u8MuxNum ,MS_XC_IPSEL_GOP ipSelGop);
INTERFACE void HAL_SC_SetVOPNBL(void *pInstance);

void Hal_SC_ip_software_reset(void *pInstance, MS_U8 u8Reset, SCALER_WIN eWindow);
void Hal_SC_ip_set_input_source(void *pInstance, MS_U8 u8InputSrcSel, MS_U8 u8SyncSel, MS_U8 u8VideoSel, MS_U8 u8isYPbPr, SCALER_WIN eWindow);
MS_BOOL Hal_SC_Check_IP_Gen_Timing(void *pInstance);
void hal_ip_set_input_10bit(void *pInstance, MS_BOOL bInput10Bit, SCALER_WIN eWindow);
void Hal_SC_ip_set_image_wrap(void *pInstance, MS_BOOL bHEnable, MS_BOOL bVEnable, SCALER_WIN eWindow);
void Hal_SC_ip_set_input_sync_reference_edge(void *pInstance, MS_BOOL bHRef, MS_BOOL bVRef, SCALER_WIN eWindow);
void Hal_SC_ip_set_input_vsync_delay(void *pInstance, MS_BOOL bDelay, SCALER_WIN eWindow);
void Hal_SC_ip_set_de_only_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void Hal_SC_ip_set_coast_input(void *pInstance, MS_BOOL bInputSel, SCALER_WIN eWindow);
void Hal_SC_ip_set_DE_Mode_Glitch(void *pInstance, MS_U8 u8Setting , SCALER_WIN eWindow );
void Hal_SC_ip_set_input_sync_sample_mode(void *pInstance, MS_BOOL bMode, SCALER_WIN eWindow);
void Hal_SC_ip_set_post_glitch_removal(void *pInstance, MS_BOOL bEnble, MS_U8 u8Range, SCALER_WIN eWindow);

MS_U8 HAL_SC_ip_get_ipmux(void *pInstance);
void HAL_SC_ip_set_ipmux(void *pInstance, MS_U8 u8Val);
MS_U8 HAL_SC_ip_get_ipclk(void *pInstance);
void HAL_SC_ip_set_ipclk(void *pInstance, MS_U8 u8Val);
MS_U8 HAL_SC_ip_get_capturesource(void *pInstance);
void HAL_SC_ip_set_capturesource(void *pInstance, MS_U8 u8Val);

void Hal_SC_set_skp_fd(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_U8 Hal_SC_ip_get_user_def_interlace_status(void *pInstance);
void Hal_SC_ip_set_user_def_interlace_status(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsInterlace,SCALER_WIN eWindow);
MS_U8 HAL_SC_ip_get_capture_method(void *pInstance);
MS_BOOL HAL_SC_ip_get_capture_format(void *pInstance);
void HAL_SC_ip_set_capture_format(void *pInstance, MS_BOOL bIsRGB);


MS_BOOL HAL_SC_ip_get_h_predown_info(void *pInstance);
MS_BOOL HAL_SC_ip_get_v_predown_info(void *pInstance);
void HAL_SC_ip_set_h_predown_info(void *pInstance, MS_BOOL bEnable);
void HAL_SC_ip_set_v_predown_info(void *pInstance, MS_BOOL bEnable);

MS_BOOL HAL_SC_ip_get_444to422_filter_mod(void *pInstance);
void HAL_SC_ip_set_444to422_filter_mod(void *pInstance, MS_BOOL bEnable);

MS_U8 HAL_SC_get_memory_fmt(void *pInstance);
MS_U8 HAL_SC_get_IPM_memory_fmt(void *pInstance);
void HAL_SC_set_IPM_memory_fmt(void *pInstance, MS_U8 u8Value);
MS_U8 HAL_SC_get_OPM_memory_fmt(void *pInstance);
void HAL_SC_set_OPM_memory_fmt(void *pInstance, MS_U8 u8Value);
void HAL_SC_set_capture_image2ip(void *pInstance, MS_BOOL bEnable);
MS_U8 HAL_SC_get_VOP_mux(void *pInstance);
void HAL_SC_set_VOP_mux(void *pInstance, MS_U8 u8Val);
MS_U8 HAL_SC_get_memory_bit_fmt(void *pInstance, SCALER_WIN eWindow);
void HAL_SC_set_memory_bit_fmt(void *pInstance, MS_BOOL bEnable_8bit, MS_BOOL bEnable_10bit);
void HAL_SC_set_IPM_capture_start(void *pInstance, MS_BOOL bEnable);
MS_U32 HAL_SC_get_IRQ_mask_0_31(void *pInstance);
void HAL_SC_set_IRQ_mask_0_31(void *pInstance, MS_U32 u32Val);

void Hal_SC_ip_set_handshaking_md(void *pInstance, MS_U8 u8MD, SCALER_WIN eWindow);
void Hal_SC_ip_Set_TestPattern(void *pInstance, MS_U16 u16Enable, MS_U32 u32Pattern_type, SCALER_WIN eWindow);
void Hal_SC_adc_Set_TestPattern(void *pInstance,MS_U8 u8EnableADCType,MS_U16 u16Ramp);
void Hal_SC_ipmux_Set_TestPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16R_CR_Data, MS_U16 u16G_Y_Data ,MS_U16 u16B_CB_Data);
void Hal_SC_op_Set_TestPattern(void *pInstance,MS_BOOL bMiuLineBuff, MS_BOOL bLineBuffHVSP);
void Hal_SC_vop_Set_TestPattern(void *pInstance,MS_U8 u8Enable);
void Hal_SC_vop2_Set_TestPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data);
void Hal_SC_mod_Set_TestPattern(void *pInstance,MS_BOOL bEnable, MS_U16 u16R_Data, MS_U16 u16G_Data ,MS_U16 u16B_Data);
void HAL_SC_ip_3DMainSub_IPSync(void *pInstance);
void HAL_SC_ip_detect_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void HAL_SC_SetVGASogEn(void *pInstance, MS_BOOL bVGASogEn);
MS_BOOL HAL_SC_GetVGASogEn(void *pInstance);

void HAL_SC_SetSogDetectStatus(void *pInstance, MS_U8  u8SogDetectStatus);
MS_U8  HAL_SC_GetSogDetectStatus(void *pInstance);

MS_U8 Hal_SC_ip_get_h_v_sync_active(void *pInstance, SCALER_WIN eWindow);
MS_U32 HAL_SC_ip_get_verticalperiod(void *pInstance, SCALER_WIN eWindow);

void HAL_SC_ip_Set_FD_Mask_ByWin(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
MS_BOOL HAL_SC_ip_Get_FD_Mask_ByWin(void *pInstance, SCALER_WIN eWindow);
MS_BOOL HAL_SC_ip_Get_FD_Mask_Status(void *pInstance, SCALER_WIN eWindow);
EN_FORCEi_SUPPORT_TYPE HAL_SC_ip_get_forceI_support_type(void *pInstance);
void HAL_SC_ip_set_hv_sync_status_check(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void HAL_SC_ip_set_new_mode_interlaced_detect(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void HAL_SC_ip_set_hv_sync_source_select(void *pInstance, MS_U8 u8Source, SCALER_WIN eWindow);
void HAL_SC_ip_set_vtotal_count_by_pixel_clock(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
void HAL_SC_ip_set_vsync_invert(void *pInstance, MS_BOOL bInvert, SCALER_WIN eWindow);
void HAL_SC_ip_set_hsync_invert(void *pInstance, MS_BOOL bInvert, SCALER_WIN eWindow);

#undef INTERFACE
#endif /* MHAL_IP_H */

