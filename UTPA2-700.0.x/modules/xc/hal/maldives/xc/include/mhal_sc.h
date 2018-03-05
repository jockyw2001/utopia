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
// [mhal_sc.h]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#ifndef MHAL_SC_H
#define MHAL_SC_H

#include "hwreg_sc.h"
#include "hwreg_mod.h"

//==============================================================================
// Scaling Ratio Macro

// H_PreScalingDownRatio() was refined to reduce the calculation error.
// Use round up (x+y/2)/y might reduce the scaling ratio and induce right vertical garbage line.
// So use un-conditional add by 1 (x+y)/y.
#define H_PreScalingDownRatio(Input, Output)                ( (((MS_U32)(Output)) * 1048576ul)/ (Input) + 1 )
#define H_PreScalingDownRatioAdv(Input, Output)             ( (((MS_U32)(Input)-1) * 1048576ul)/ ((Output)-1) ) //Advance scaling
#define H_PreScalingDownRatioAdv_No_Minus1(Input, Output)   ( (((MS_U32)(Input)) * 1048576ul)/ ((Output)) )     //Advance scaling without minus 1
#define V_PreScalingDownRatio(Input, Output)                ( (((MS_U32)(Output)) * 1048576ul)/ (Input) + 1 ) // CB mode
#define V_PreScalingDownRatioBilinear(Input, Output)        ( (((MS_U32)(Input)-1) * 1048576ul)/ ((Output) -1)) // Bilinear

#define H_PostScalingRatio(Input, Output)        ( ((MS_U32)(Input)) * 1048576ul / (Output) + 1 )
//#define H_PostScalingRatioAdv(Input, Output)     ( ((MS_U32)(Input)-1) * 1048576ul / ((Output)-1) + 1 )
#define V_PostScalingRatio(Input, Output)        ( ((MS_U32)(Input)-1) * 1048576ul / ((Output)-1) + 1 )
#define V_PostScalingRatio_P2I(Input, Output)        ( ((MS_U32)(Input)) * 1048576ul / (Output))
#define HW_CLK_CTRL FALSE

// In MDrv_SC_3D_Adjust_PreHorDstSize(),
// do a rough check after all, for 2 frame case.
#define ENABLE_2_FRAME_SIZE_PROTECTION  TRUE

typedef struct
{
    MS_U32 u32IPMBase0;
    MS_U32 u32IPMBase1;
    MS_U32 u32IPMBase2;
    MS_U16 u16IPMOffset;
    MS_U16 u16IPMFetch;
    MS_U16 u16VLength;
    MS_U32 u32WriteLimitBase;

    MS_BOOL bLinearAddrMode;
    MS_BOOL bYCSeparate;

    MS_BOOL bMemFormat422;
    MS_BOOL bInterlace;
    MS_U8 u8BitPerPixel;
    MS_U8 u8FBNum;
} SC_FRAMEBUF_INFO_t;

typedef struct
{
    MS_U16 u16MiuG0Mask;
    MS_U16 u16MiuG1Mask;
    MS_U16 u16MiuG2Mask;
    MS_U16 u16MiuG3Mask;
} SC_MIUMASK_t;

typedef struct
{
    MS_U8 u8MainFBSel;
    MS_U8 u8SubFBSel;
} SC_MIUSEL_t;

typedef enum
{
    E_XC_FPLL_DIR_UNKNOWN,
    E_XC_FPLL_DIR_UP,
    E_XC_FPLL_DIR_DOWN,
} MS_XC_FPLL_DIRECTION;

typedef enum
{
    E_XC_FPLL_RES_WAITING,
    E_XC_FPLL_RES_TIMEOUT,
    E_XC_FPLL_RES_FINISHED,
} MS_XC_FPLL_RESULT;

typedef struct
{
    MS_U8  u8Debounce;
    MS_U16 u16PhaseDiff;
    MS_XC_FPLL_DIRECTION eFpllDir;
    MS_XC_FPLL_RESULT eFpllResult;
} MS_XC_GET_FPLL_PHASEDIFF;

typedef enum
{
    E_XC_OP1_PATTERN_TYPE_PURE = 0,
    E_XC_OP1_PATTERN_TYPE_FADE,
    E_XC_OP1_PATTERN_TYPE_LINEDOT,
    E_XC_OP1_PATTERN_TYPE_CALIBRATION,
    E_XC_OP1_PATTERN_TYPE_DOTFADE,
} E_XC_OP1_PATTERN_DTAT_TYPE;

typedef enum
{
    E_HAL_SC_3D_LRCHGMODE_OFF   = 0,
    E_HAL_SC_3D_LRCHGMODE_FRAME = 1,
    E_HAL_SC_3D_LRCHGMODE_BLOCK = 2,
    E_HAL_SC_3D_LRCHGMODE_LINE  = 3,
} HAL_SC_3D_LRCHGMODE;

typedef struct
{
    void* pInstance;
    MS_BOOL bEnable;
    SCALER_WIN eWindow;
}SC_SET_FORCE_CURRENT_READ_BANK_t;

//==============================================================================
//==============================================================================
#ifdef MHAL_SC_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE void Hal_SC_setfield(void *pInstance, MS_U16 reg_1D, MS_U16 reg_21, MS_U16 reg_23, SCALER_WIN eWindow);

INTERFACE void Hal_SC_set_ficlk(void *pInstance, MS_BOOL bPreDown, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_shiftline(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_422_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow);
INTERFACE void Hal_XC_Set_FreeFRCMD(void *pInstance, MS_BOOL bEnable);
INTERFACE void Hal_SC_force3fb(MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_force4fb(MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_force8fb(MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE XC_FRAME_STORE_NUMBER Hal_SC_GetFrameStoreMode(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_csc(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_wr_bank_mapping(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_wr_bank_mapping_num(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_Get_WR_Bank_Mapping(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_Get_WR_Bank_Mapping_Num(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_delayline(void *pInstance, MS_U8 u8DelayLines, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_write_limit(void *pInstance, MS_U32 u32WritelimitAddrBase, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_U32 u32OPWlimitAddr, SCALER_WIN eWindow);
INTERFACE void Hal_SC_sw_db(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void Hal_SC_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void Hal_SC_dual_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pMainDBreg, P_SC_SWDB_INFO pSubDBreg);
INTERFACE E_APIXC_ReturnValue Hal_SC_VIP_Peaking_Setting(void *pInstance, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_support_source_to_ve(void *pInstance, MS_U16* OutputCapability);
INTERFACE E_APIXC_ReturnValue Hal_SC_set_output_capture_enable(void *pInstance, MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE);
INTERFACE void Hal_SC_set_de_window(void *pInstance, XC_PANEL_INFO *pPanel_Info);
INTERFACE void Hal_SC_get_disp_de_window(void *pInstance, MS_WINDOW_TYPE *pWin);
INTERFACE MS_U32 Hal_SC_Get_DNRBase0(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_Get_DNRBase1(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_Get_OPMBase0(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_Get_OPMBase1(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_Get_OPMBase2(SCALER_WIN eWindow);
INTERFACE MS_U8  Hal_SC_Get_LBOffset(SCALER_WIN eWindow);
INTERFACE void   Hal_SC_Set_LBOffset(MS_U8 u8LBOffset, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_DNRBase0(void *pInstance, MS_U32 u32DNRBase0, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_DNRBase1(void *pInstance, MS_U32 u32DNRBase1, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_DNRBase2(void *pInstance, MS_U32 u32DNRBase2, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_OPMBase0(void *pInstance, MS_U32 u32OPMBase0, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_OPMBase1(void *pInstance, MS_U32 u32OPMBase1, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_OPMBase2(void *pInstance, MS_U32 u32OPMBase2, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_memoryaddress(void *pInstance, MS_U32 u32DNRBase0, MS_U32 u32DNRBase1, MS_U32 u32DNRBase2, MS_U32 u32OPMBase0, MS_U32 u32OPMBase1, MS_U32 u32OPMBase2, SCALER_WIN eWindow);
INTERFACE void Hal_SC_Enable_MiuMask(void *pInstance);
INTERFACE void Hal_SC_Disable_MiuMask(void *pInstance);
INTERFACE void Hal_SC_set_miusel(void *pInstance,MS_U8 u8MIUSel);

INTERFACE void Hal_SC_set_blsk(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_SC_set_blsk_burst(void *pInstance,MS_BOOL bEn);

INTERFACE void Hal_SC_set_main_black_screen_burst(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_SC_set_main_sub_black_screen_burst(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_SC_set_sub_blue_screen_burst(void *pInstance,MS_BOOL bEn, MS_BOOL bBlue);
INTERFACE void Hal_SC_set_main_black_screen(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_SC_set_main_sub_black_screen(void *pInstance,MS_BOOL bEn);
INTERFACE void Hal_SC_set_sub_blue_screen(void *pInstance,MS_BOOL bEn, MS_BOOL bBlue);
INTERFACE E_APIXC_ReturnValue Hal_SC_IP_Memory_Write_Request(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_IP_Memory_Read_Request(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_OP_Memory_Write_Request(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_disable_inputsource_burst(void *pInstance,MS_BOOL bDisable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_disable_inputsource(void *pInstance,MS_BOOL bDisable, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_Is_InputSource_Disable(void *pInstance,SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_nosignal_color(void *pInstance,MS_U8 u8Color,SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_fbl(void *pInstance,MS_BOOL bEn);
INTERFACE MS_BOOL Hal_SC_get_fbl(void *pInstance);
INTERFACE void HAL_SC_Set_FB_Num(void *pInstance,SCALER_WIN eWindow, XC_FRAME_STORE_NUMBER eBestFBNum, MS_BOOL bInterlace);
INTERFACE void Hal_SC_set_freezeimg_burst(void *pInstance,MS_BOOL bEn, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_freezeimg(void *pInstance,MS_BOOL bEn, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_get_freezeimg(void *pInstance,SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_Init_for_internal_timing(void *pInstance,XC_Internal_TimingType timingtype, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_rgb_average_info(void *pInstance,MS_U16 u16mask, SCALER_WIN eWindow);
INTERFACE void Hal_SC_autogain_enable(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_autogain_status(void *pInstance,SCALER_WIN eWindow);
INTERFACE void Hal_SC_init_riu_base(MS_U32 u32riu_base, MS_U32 u32PMriu_base);
INTERFACE void Hal_SC_set_mirror(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_GetAVDStatus(void);
INTERFACE void Hal_SC_set_rep_window(void *pInstance, MS_BOOL bEnable,MS_U16 x,MS_U16 y,MS_U16 w,MS_U16 h,MS_U8 u8Color);
INTERFACE void Hal_SC_set_disp_window(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin);
INTERFACE void Hal_SC_get_disp_window(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin);
INTERFACE void Hal_SC_set_Fclk(void *pInstance, EN_SET_FCLK_CASE enCase);
INTERFACE void Hal_SC_get_framebuf_Info(void *pInstance, SC_FRAMEBUF_INFO_t *pFrameBufInfo, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_framebuf_Info(void *pInstance, SC_FRAMEBUF_INFO_t pFrameBufInfo);
INTERFACE MS_U8 HAL_SC_get_dnr_setting(void *pInstance, SCALER_WIN eWindow);
INTERFACE void HAL_SC_enable_dnr(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
INTERFACE void HAL_SC_enable_field_avg_y(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void HAL_SC_enable_field_avg_c(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_get_cs_det_cnt(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_linearmem_mode(MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_cs_det_cnt(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_get_plus_width(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_opm_fetch(void *pInstance, SCALER_WIN eWindow, MS_U16 u16OPMFetch);
INTERFACE MS_U16 Hal_SC_get_opm_fetch(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_SWDS_AddCmd(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void Hal_SC_SWDS_Fire(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);

INTERFACE void HAL_SC_VOP_Set_Contrast_En(void *pInstance, MS_BOOL bEenable, SCALER_WIN eWindow);
INTERFACE void HAL_SC_VOP_Set_Contrast_Value(void *pInstance, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow);
INTERFACE void HAL_SC_VOP_Set_Brightness_En(void *pInstance, MS_BOOL bEenable, SCALER_WIN eWindow);
INTERFACE void HAL_SC_VOP_Set_Brightness_Value(void *pInstance, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow);

INTERFACE MS_BOOL HAL_SC_Set_DynamicScaling(void *pInstance,MS_U32 u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On);
INTERFACE MS_BOOL HAL_SC_Get_DynamicScaling_Status(void);
INTERFACE void HAL_SC_Enable_VInitFactor(void *pInstance,MS_BOOL bEnable,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_VInitFactorOne(void *pInstance,MS_U32 u32Value,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_VInitFactorTwo(void *pInstance,MS_U32 u32Value,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_vsd_input_line_count(void *pInstance,MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_vsd_output_line_count(void *pInstance,MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_CheckSubWinPreScaling(MS_U16 u16ScaleDst,MS_BOOL bInterlace);
INTERFACE void HAL_SC_EnableFPLL(void);
INTERFACE MS_BOOL HAL_SC_WaitFPLLDone(void *pInstance);
INTERFACE MS_U16 HAL_SC_GetOutputVFreqX100(MS_U32 u32XTAL_Clock);
INTERFACE MS_BOOL Hal_SC_get_csc(void *pInstance,SCALER_WIN eWindow);

//Add for T3
INTERFACE void HAL_SC_FilmMode_Patch1(void *pInstance);

INTERFACE void HAL_SC_SetMainLineBufferOffset(void *pInstance, MS_U16 u16Linebuffer);
INTERFACE void HAL_SC_SetSubLineBufferOffset(void *pInstance, MS_U16 u16Linebuffer);
INTERFACE void HAL_SC_SetDisplay_LineBuffer_Mode(void *pInstance, MS_BOOL bEnable);
INTERFACE void HAL_SC_SetDisplay_Start_Mode(void *pInstance, MS_BOOL bEnable);
INTERFACE void HAL_SC_FillLineBuffer(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_enable_window(void *pInstance, MS_BOOL bEn,SCALER_WIN eWindow);
INTERFACE void Hal_SC_enable_window_burst(void *pInstance, MS_BOOL bEn,SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_trigger_signal(void);
INTERFACE void Hal_SC_Set_extra_fetch_line(void *pInstance, MS_U8 u8val);
INTERFACE void Hal_SC_Set_extra_adv_line(void *pInstance, MS_U8 u8val);
INTERFACE void Hal_SC_SetOPWriteOff(void *pInstance, MS_BOOL bEna);
INTERFACE MS_BOOL Hal_SC_GetOPWriteOff(void *pInstance);
INTERFACE MS_BOOL Hal_SC_get_pixel_rgb(void *pInstance, XC_Get_Pixel_RGB *pData);

// FPLL
void HAL_SC_Set_FPLL_Limit(void *pInstance, MS_U32 *u32PllSet, MS_U32 u32LowBound, MS_U32 u32UpBound, MS_BOOL _bInFPLLDbgMode, MS_U32 _U32LimitD5D6D7);

//Add for U4
INTERFACE void Hal_SC_Set_OSD2VE(void *pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX);
//Add for T4/Janus
INTERFACE MS_BOOL Hal_SC_IsOPMFetchPatch_Enable(void);
INTERFACE MS_BOOL Hal_SC_Check_HNonLinearScaling(void *pInstance);

//Add for T8 H3D reg handle
INTERFACE void Hal_XC_H3D_Enable(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_XC_H3D_Input3DType(void *pInstance, E_XC_3D_INPUT_MODE e3DType);
INTERFACE void Hal_XC_H3D_Breakline_Enable(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_XC_H3D_HDE(void *pInstance, MS_U16 u16Hde);
INTERFACE void Hal_XC_H3D_LR_Toggle_Enable(void *pInstance, MS_BOOL bEn, MS_BOOL b2DTo3D, MS_BOOL bSkipDefaultLRFlag);
INTERFACE void HAL_XC_H3D_OPM_SBYS_PIP_Enable(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_XC_H3D_HDE(void *pInstance,MS_U16 u16Hde);
INTERFACE void Hal_XC_H3D_VDE_F0(void *pInstance,MS_U16 u16Vde);
INTERFACE void Hal_XC_H3D_VDE_F2(void *pInstance,MS_U16 u16Vde);
INTERFACE void Hal_XC_H3D_HBLANK(void *pInstance,MS_U16 u16Hblank);
INTERFACE void Hal_XC_H3D_INIT_VBLANK(void *pInstance,MS_U8 u8Vblank);
INTERFACE void Hal_XC_H3D_VBLANK0(void *pInstance,MS_U8 u8Vblank);
INTERFACE void Hal_XC_H3D_VBLANK1(void *pInstance,MS_U8 u8Vblank);
INTERFACE void Hal_XC_H3D_VBLANK2(void *pInstance,MS_U8 u8Vblank);
INTERFACE void Hal_XC_H3D_VSYNC_WIDTH(void *pInstance,MS_U8 u8Width);
INTERFACE void Hal_XC_H3D_VSYNC_POSITION(void *pInstance,MS_U16 u16Position);

INTERFACE MS_BOOL Hal_SC_Detect_RequestFBL_Mode(void *pInstance);
INTERFACE void Hal_XC_SetFrameColor(void *pInstance, MS_U32 u32aRGB);
INTERFACE MS_BOOL Hal_SC_is_extra_req_en(void *pInstance, MS_U16 *pu16MainHStart, MS_U16 *pu16MainHEnd, MS_U16 *pu16SubHStart,  MS_U16 *pu16SubHEnd);
INTERFACE MS_U8 Hal_SC_getVSyncWidth(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_enable_cursor_report(void *pInstance, MS_BOOL bEn);

//Add for K1, just a dummy function at this chip
INTERFACE E_APIXC_ReturnValue Hal_SC_Set_OSDLayer(void *pInstance, E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow);
INTERFACE E_VOP_OSD_LAYER_SEL Hal_SC_Get_OSDLayer(void *pInstance, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_Set_VideoAlpha(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_Get_VideoAlpha(void *pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow);
INTERFACE void Hal_XC_ClearScalingFactorForInternalCalib(void *pInstance );
INTERFACE MS_BOOL MHal_XC_SetFRCMBufAddress(void *pInstance, MS_U32 u32BaseAddr, SCALER_WIN eWindow);

INTERFACE MS_BOOL MHal_XC_SetFRCMBufSize(void *pInstance, MS_U32 u32BufSize, SCALER_WIN eWindow);

INTERFACE MS_U8 MHal_SC_cal_usedgain_to_reggain(MS_U32 u16UsedGain_x32);
INTERFACE E_APIXC_ReturnValue Hal_SC_OP2VOPDESel(void *pInstance,E_OP2VOP_DE_SEL eVopDESel);
INTERFACE MS_BOOL Hal_XC_SVOutput_GetCaps(void *pInstance);

#define _MHal_SC_Flock_Caculate_LPLLSet(args...)
#define _MHal_SC_Set_LPLL_Limit(args...)
#define _MHal_SC_Flock_Set_IGainPGain(args...)
#define _MHal_SC_Flock_Set_LPLL_Enable(args...)

#define MHal_CLKGEN_FRC_Bypass_Enable(args...)

#define Hal_XC_IsForcePrescaling_3D Hal_XC_IsForcePrescaling
INTERFACE E_APIXC_ReturnValue Hal_XC_IsForcePrescaling(void *pInstance,const XC_InternalStatus *pSrcInfo, MS_BOOL *pbForceV, MS_BOOL *pbForceH, SCALER_WIN eWindow);
INTERFACE MS_U32 MHal_SC_Get_LpllSet_Factor(void *pInstance,MS_U8 u8LPLL_Mode,MS_U8 u8LPLL_Type, MS_U32 u32DefaultDClk);
INTERFACE MS_U32 MHal_SC_Get_LpllSet_Factor_denominator(void *pInstance,MS_U8 u8LPLL_Mode,MS_U8 u8LPLL_Type, MS_U32 u32DefaultDClk);
INTERFACE E_APIXC_ReturnValue Hal_SC_Enable_AVMute(void *pInstance,SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_XC_ReportPixelInfo(void *pInstance,MS_XC_REPORT_PIXELINFO *pstRepPixInfo);

INTERFACE void MHal_XC_Calc_IGainPGain(void *pInstance, MS_U8 *u8GainI, MS_U8 *u8GainP, MS_U32 u32XTAL_Clock, MS_U8 u8LGain, MS_U8 u8Vco, MS_U16 u16HTotal, MS_U16 u16VTotal, MS_U8 u8FRC_Out);

#define Hal_SC_set_mrq_miusel(args...)
#define Hal_SC_set_mcdi_memoryaddress(args...)
#define Hal_SC_set_mcdi_write_limit(args...)
#define Hal_SC_enable_mcdi(args...);

INTERFACE void Hal_SC_set_bws_mode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_sw_lcnt_en(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_sw_lcnt(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_ipmw_lcnt_inv(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_ipmr_lcnt_inv(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_opm_lcnt_inv(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_opm_lcnt_inv_pre(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_3D_Adjust_PreHVscaling_SaveBW(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling,MS_BOOL *pb3DPreVScaling,MS_BOOL bForcePreHScalingDown,MS_BOOL bForcePreVScalingDown,SCALER_WIN eWindow);


#define MHAL_SC_set_osdc_tgen_hsync_start(args...)
#define MHAL_SC_set_osdc_tgen_hsync_end(args...)
#define MHAL_SC_set_osdc_tgen_hframe_de_start(args...)
#define MHAL_SC_set_osdc_tgen_hframe_de_end(args...)
#define MHAL_SC_set_osdc_tgen_htotal(args...)
#define MHAL_SC_set_osdc_tgen_vtotal(args...)
#define MHAL_SC_set_osdc_tgen_vframe_de_start(args...)
#define MHAL_SC_set_osdc_tgen_vframe_de_end(args...)
#define MHAL_SC_set_osdc_tgen_vsync_start(args...)
#define MHAL_SC_set_osdc_tgen_vsync_end(args...)
#define MHAL_SC_set_osdc_tgen_reset_enable(args...)
#define MHAL_SC_set_osdc_swreset_enable(args...)
#define MHAL_SC_set_osdc_mixer_bypass_enable(args...)
#define MHAL_SC_set_osdc_mixer_inv_alpha_enable(args...)
#define MHAL_SC_set_osdc_mixer_hs_n_vfde_enable(args...)
#define MHAL_SC_set_osdc_mixer_hfde_n_vfde_enable(args...)
#define MHAL_SC_set_osdc_clk_mux(args...)
#define MHAL_SC_enable_osdc(args...)

#define MHAL_SC_get_osdc_tgen_hsync_start(args...) 0
#define MHAL_SC_get_osdc_tgen_hsync_end(args...) 0
#define MHAL_SC_get_osdc_tgen_hframe_de_start(args...) 0
#define MHAL_SC_get_osdc_tgen_hframe_de_end(args...) 0
#define MHAL_SC_get_osdc_tgen_htotal(args...) 0
#define MHAL_SC_get_osdc_tgen_vtotal(args...) 0
#define MHAL_SC_get_osdc_tgen_vframe_de_start(args...) 0
#define MHAL_SC_get_osdc_tgen_vframe_de_end(args...) 0
#define MHAL_SC_get_osdc_tgen_vsync_start(args...) 0
#define MHAL_SC_get_osdc_tgen_vsync_end(args...) 0
#define MHAL_SC_get_osdc_mixer_bypass_status(args...) 0
#define MHAL_SC_get_osdc_mixer_inv_alpha_status(args...) 0
#define MHAL_SC_get_osdc_mixer_hs_n_vfde_status(args...) 0
#define MHAL_SC_get_osdc_mixer_hfde_n_vfde_status(args...) 0
#define MHAL_SC_get_osdc_onoff_status(args...) 0

#define MHal_XC_ByPass_SetOSDVsyncPos(args...)
#define MHal_XC_Bypass_SetinputSrc(args...)
#define MHal_XC_BYPASS_Setinputclk(args...)
#define MHal_XC_BYPASS_SetCSC(args...)

INTERFACE void MHal_XC_SetForceReadBank(void *pInstance, MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow);
INTERFACE void MHal_XC_SetForceCurrentReadBank(void *pInstance,MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_SetPX2MemFormat(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_IsPX2MemFormat(void *pInstance, SCALER_WIN eWindow);
//#define MHal_XC_Set_DSIndexSourceSelect(args...)
//#define MHal_XC_Set_DSForceIndex(args...)
INTERFACE MS_BOOL MHal_XC_SetDNRBufAddress(void *pInstance, MS_U32 u32DNRBaseAddr, SCALER_WIN eWindow);
INTERFACE MS_U32 MHal_XC_GetDNRBufAddress(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MHal_XC_SetDNRBufSize(void *pInstance, MS_U32 u32DNRBufSize, SCALER_WIN eWindow);
INTERFACE MS_U32 MHal_XC_GetDNRBufSize(void *pInstance, SCALER_WIN eWindow);
#define MHal_XC_Init_Patch(args...) 0
INTERFACE E_APIXC_ReturnValue Hal_SC_SetOSDDetect(void *pInstance, MS_BOOL bEnable, MS_U32 Threhold);
INTERFACE E_APIXC_ReturnValue Hal_SC_GetOSDDetect(void *pInstance, MS_BOOL *pbOSD);

//Vtrack
#define MHal_XC_Vtrack_SetPayloadData(args...) 2
#define MHal_XC_Vtrack_SetUserDefindedSetting(args...)2
#define MHal_XC_Vtrack_Enable(args...) 2

// scaling without mute
INTERFACE void MHAL_SC_enable_vsd_shift(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL MHal_XC_IsPNLYUVOutput(void *pInstance);
INTERFACE void MHal_SC_set_manual_rbank_switch_cnt(void *pInstance, MS_BOOL bEnable, MS_U16 u16SwitchCnt, SCALER_WIN eWindow);
#define HAL_SC_Set_LB_MergeAddress(args...)
INTERFACE MS_BOOL Hal_SC_Init(void *pInstance);
INTERFACE MS_U32 Hal_SC_Get_Device_Offset(MS_U8 deviceIdx);

INTERFACE void Hal_SC_set_ficlk2(void *pInstance, MS_U8 u8Clk_Mux, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void HAL_SC_EnableLegacyMode(void *pInstance, MS_BOOL bEnable);
INTERFACE void HAL_SC_SwitchIPMWriteBank(void *pInstance, MS_BOOL bEnable, E_XC_IPM_CONNECT_ID eIPMConnect);
INTERFACE void HAL_SC_SwitchFRCMWriteBank(void *pInstance, MS_BOOL bEnable, E_XC_FRCMW_CONNECT_ID eFRCMConnect);
INTERFACE void HAL_SC_Enable_FEOP1(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);

INTERFACE void Hal_SC_set_fmclk(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_BOOL Hal_SC_3D_Is_LR_Sbs2Line(void *pInstance);
INTERFACE void Hal_SC_3D_SetLRChgMode(void *pInstance, HAL_SC_3D_LRCHGMODE eLRChgMode);
INTERFACE void Hal_SC_3D_SetInitialLRIndex(void *pInstance, MS_BOOL bRFirst);
INTERFACE void Hal_SC_3D_SetSplitHalf(void *pInstance, MS_BOOL bEnable);
INTERFACE void HAL_SC_Set_vsd_3D_autofactor_reset(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_CheckMuteStatusByRegister(void *pInstance, SCALER_WIN eWindow);
#define Hal_SC_set_frcm_freezeimg(args...)
#define Hal_SC_set_frcm_memoryaddress(args...)
#define Hal_SC_set_frcm_write_limit(args...)
#define Hal_SC_set_frcm_mirror(args...)
#define Hal_SC_set_edclk(args...)
#define Hal_SC_set_T3D_setting(args...)
#define Hal_SC_set_T3D_H_size(args...)
#define MHAL_SC_set_2p_mode(args...) 0

#define Hal_SC_set_frcm_to_FD_mask(args...)
#define Hal_SC_set_frcm_to_freeze(args...)

#define Hal_SC_3D_set_top_win_6Tap(args...)

#ifdef ENABLE_SPREADMODE
//not implemnet
#define MHal_XC_IsSupportSpreadMode(args...) FALSE
#endif

INTERFACE MS_BOOL Hal_SC_get_ip2_csc(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_get_miusel(void *pInstance, SC_MIUSEL_t* stMIUSel);

// HW internal pattern
INTERFACE void MHal_SC_OP1_Pattern_backup_setting(void);
INTERFACE void MHal_SC_OP1_Pattern_enable(MS_BOOL bEnable);
INTERFACE void MHal_SC_OP1_Pattern_init_setting(void);
INTERFACE void MHal_SC_OP1_Pattern_restore_setting(void);
INTERFACE void MHal_SC_OP1_Pattern_set_jump_threshold(MS_U16 u16Threshold);
INTERFACE void MHal_SC_OP1_Pattern_set_wraddr(MS_U16 u16Addr);
INTERFACE void MHal_SC_OP1_Pattern_sram_write_enable(MS_BOOL bEn);
INTERFACE void MHal_SC_OP1_Pattern_sram_write_fastmode(MS_BOOL bEn);
INTERFACE void MHal_SC_OP1_Pattern_sram_writestart(void);
INTERFACE void MHal_SC_OP1_Pattern_sram_writedone(void);
INTERFACE MS_BOOL MHal_SC_OP1_Pattern_set_patgen_mode(EN_XC_OP1_PATGEN_MODE mode);
INTERFACE void MHal_SC_OP1_PatGen_adjust_frame_size(MS_U16 u16HSize, MS_U16 u16VSize);
INTERFACE void MHal_SC_OP1_Pattern_set_frame_size(MS_U16 u16HSize, MS_U16 u16VSize);
INTERFACE void MHal_SC_OP1_Pattern_set_dotfade_value(MS_U8 RVal, MS_U8 GVal, MS_U8 BVal);
INTERFACE void MHal_SC_OP1_Pattern_set_dotfade_overfollow_protection(MS_BOOL REn, MS_BOOL GEn, MS_BOOL BEn);
INTERFACE void MHal_SC_OP1_Pattern_pure_color(MS_U8 u8Color);
INTERFACE void MHal_SC_OP1_Pattern_set_sram(MS_U16 u16Tbl0, MS_U16 u16Tbl1, MS_U16 u16Tbl2, MS_U16 u16Tbl3);
INTERFACE void MHal_SC_OP1_Pattern_define_tile(MS_U16 hTile, MS_U16 vTile);
INTERFACE void MHal_SC_OP1_Pattern_enable_border(MS_BOOL bEnable, MS_U16 RVal, MS_U16 GVal, MS_U16 BVal);
INTERFACE void MHal_SC_OP1_Pattern_customize_blcok(MS_U8 block_Size, MS_U8 repeat_Num, MS_BOOL isLineStart, MS_BOOL isLineEnd, MS_U16 RVal, MS_U16 GVal, MS_U16 BVal, E_XC_OP1_PATTERN_DTAT_TYPE eType);
INTERFACE void MHal_SC_OP1_Pattern_set_border_sram(XC_OP1_PatGen_Border tile1, XC_OP1_PatGen_Border tile2, XC_OP1_PatGen_Border tile3, XC_OP1_PatGen_Border tile4, XC_OP1_PatGen_Border tile5);
INTERFACE void MHAL_SC_set_r2y_en(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void MHAL_SC_set_VOP(void *pInstance, MS_XC_VOP_Data *pstVOPData);

#undef INTERFACE
#endif /* MHAL_SC_H */

