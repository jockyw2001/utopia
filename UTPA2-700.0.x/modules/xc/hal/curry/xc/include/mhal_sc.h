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

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#endif


#include "hwreg_sc.h"
#include "hwreg_mod.h"
#include "mhal_xc_chip_config.h"
#include "apiXC_VMark.h"

//==============================================================================
// Scaling Ratio Macro
// Scaling Ratio Macro
#ifdef MSOS_TYPE_LINUX_KERNEL
#define H_PreScalingDownRatio(Input, Output)                { u64_result = ((MS_U64)(Output))    * 1048576ul;do_div(u64_result,(Input));}
#define H_PreScalingDownRatioAdv(Input, Output)             { u64_result = ((MS_U64)(Input)-1)   * 2097152ul;do_div(u64_result,(Output-1));  u64_result +=1;do_div(u64_result,2);} //Advance scaling
#define H_PreScalingDownRatioAdv_No_Minus1(Input, Output)   { u64_result = ((MS_U64)(Input))     * 2097152ul;do_div(u64_result,((Output)) *2);}   //Advance scaling without minus 1
#define V_PreScalingDownRatio(Input, Output)                { u64_result = ((MS_U64)(Output))    * 1048576ul;do_div(u64_result,(Input));     u64_result +=1;} // CB mode
#define V_PreScalingDownRatioBilinear(Input, Output)        { u64_result = ((MS_U64)(Input)-1)   * 1048576ul;do_div(u64_result,((Output) -1));} // Bilinear

#define H_PostScalingRatio(Input, Output)                   { u64_result = ((MS_U64)(Input))     * 2097152ul;do_div(u64_result,(Output));    u64_result +=1;do_div(u64_result,2);}
#define V_PostScalingRatio(Input, Output)                   { u64_result = ((MS_U64)(Input)-1)   * 2097152ul;do_div(u64_result,(Output-1));  u64_result +=1;do_div(u64_result,2);}
#define V_PostScalingRatio_P2I(Input, Output)               { u64_result = ((MS_U64)(Input))     * 1048576ul;do_div(u64_result,(Output));}
#else
// H_PreScalingDownRatio() was refined to reduce the calculation error.
// Use round up (x+y/2)/y might reduce the scaling ratio and induce right vertical garbage line.
// So use un-conditional add by 1 (x+y)/y.
#define H_PreScalingDownRatio(Input, Output)                ((MS_U32)( (((MS_U64)(Output)) * 1048576ul)/ (Input)))
#define H_PreScalingDownRatioAdv(Input, Output)             ((MS_U32)( (((MS_U64)(Input)-1) * 2097152ul)/ (((Output)-1) +1)/2)) //Advance scaling
#define H_PreScalingDownRatioAdv_No_Minus1(Input, Output)   ((MS_U32)( (((MS_U64)(Input)) * 2097152ul)/ ((Output) /2)))    //Advance scaling without minus 1
#define V_PreScalingDownRatio(Input, Output)                ((MS_U32)( (((MS_U64)(Output)) * 1048576ul)/ (Input) + 1 )) // CB mode
#define V_PreScalingDownRatioBilinear(Input, Output)        ((MS_U32)( (((MS_U64)(Input)-1) * 1048576ul)/ ((Output) -1) )) // Bilinear

#define H_PostScalingRatio(Input, Output)          ((MS_U32)( ((MS_U64)(Input)) * 2097152ul / (Output) + 1 )/2)
//#define H_PostScalingRatioAdv(Input, Output)     (((MS_U32)(Input)-1) * 1048576ul / ((Output)-1) + 1 )
#define V_PostScalingRatio(Input, Output)          ((MS_U32)( ((MS_U64)(Input) * 2097152ul) / Output + 1) / 2)
#define V_PostScalingRatio_P2I(Input, Output)    ((MS_U32)( ((MS_U64)(Input)) * 1048576ul / (Output) ))

#define V_PostScalingUPInInitialPhase(Input, Output)          ((MS_U32)(1048576ul + (Input*1048576ul)/(Output) + 1) / 2)
#define V_PostScalingDOWNInitialPhase(Input, Output)          ((MS_U32)(((MS_U64)Input*1048576ul)/(Output) - 1048576ul + 1) / 2)
#endif
// In MDrv_SC_3D_Adjust_PreHorDstSize(),
// do a rough check after all, for 2 frame case.
#define ENABLE_2_FRAME_SIZE_PROTECTION  FALSE

typedef struct
{
    MS_PHY u32IPMBase0;
    MS_PHY u32IPMBase1;
    MS_PHY u32IPMBase2;
    MS_U16 u16IPMOffset;
    MS_U16 u16IPMFetch;
    MS_U16 u16VLength;
    MS_PHY u32WriteLimitBase;

    MS_BOOL bLinearAddrMode;
    MS_BOOL bYCSeparate;

    MS_BOOL bMemFormat422;
    MS_BOOL bInterlace;
    MS_U8 u8BitPerPixel;
    MS_U8 u8FBNum;
} SC_FRAMEBUF_INFO_t;

typedef struct __attribute__((packed))
{
    MS_U16 u16MiuG0Mask;
    MS_U16 u16MiuG1Mask;
    MS_U16 u16MiuG2Mask;
    MS_U16 u16MiuG3Mask;
    MS_U16 u16MiuG4Mask;
    MS_U16 u16MiuG5Mask;
    MS_U16 u16MiuG6Mask;
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
    E_HAL_SC_3D_LRCHGMODE_OFF   = 0,
    E_HAL_SC_3D_LRCHGMODE_FRAME = 1,
    E_HAL_SC_3D_LRCHGMODE_BLOCK = 2,
    E_HAL_SC_3D_LRCHGMODE_LINE  = 3,
} HAL_SC_3D_LRCHGMODE;

typedef enum
{
    E_HAL_SC_BYPASSMODE_INVALID   = 0,
    E_HAL_SC_BYPASSMODE_NORMAL    = 1,
    E_HAL_SC_BYPASSMODE_BYPASSIP  = 2,
    E_HAL_SC_BYPASSMODE_BYPASSDI  = 3,
} E_HAL_SC_BYPASSMODE;

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
#define Hal_SC_force3fb(args...)
#define Hal_SC_force4fb(args...)
#define Hal_SC_force8fb(args...)
INTERFACE XC_FRAME_STORE_NUMBER Hal_SC_GetFrameStoreMode(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_csc(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_wr_bank_mapping(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_wr_bank_mapping_num(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_Get_WR_Bank_Mapping(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_Get_WR_Bank_Mapping_Num(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_delayline(void *pInstance, MS_U8 u8DelayLines, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_write_limit(void *pInstance, MS_PHY u32WritelimitAddrBase, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow);
INTERFACE void Hal_SC_sw_db(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void Hal_SC_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void Hal_SC_dual_sw_db_burst(void *pInstance, P_SC_SWDB_INFO pMainDBreg, P_SC_SWDB_INFO pSubDBreg);
INTERFACE E_APIXC_ReturnValue Hal_SC_VIP_Peaking_Setting(void *pInstance, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_support_source_to_ve(void *pInstance, MS_U16* OutputCapability);
INTERFACE E_APIXC_ReturnValue Hal_SC_set_output_capture_enable(void *pInstance, MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE);
INTERFACE void Hal_SC_set_de_window(void *pInstance, XC_PANEL_INFO *pPanel_Info);
INTERFACE void Hal_SC_get_disp_de_window(void *pInstance, MS_WINDOW_TYPE *pWin);
INTERFACE MS_PHY Hal_SC_Get_DNRBase0(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_PHY Hal_SC_Get_DNRBase1(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_PHY Hal_SC_Get_OPMBase0(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_PHY Hal_SC_Get_OPMBase1(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_PHY Hal_SC_Get_OPMBase2(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_U8  Hal_SC_Get_LBOffset(void *pInstance, SCALER_WIN eWindow);
INTERFACE void   Hal_SC_Set_LBOffset(void *pInstance, MS_U8 u8LBOffset, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_DNRBase0(void *pInstance, MS_PHY u32DNRBase0, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_DNRBase1(void *pInstance, MS_PHY u32DNRBase1, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_DNRBase2(void *pInstance, MS_PHY u32DNRBase2, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_OPMBase0(void *pInstance, MS_PHY u32OPMBase0, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_OPMBase1(void *pInstance, MS_PHY u32OPMBase1, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_OPMBase2(void *pInstance, MS_PHY u32OPMBase2, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_memoryaddress(void *pInstance, MS_PHY u32DNRBase0, MS_PHY u32DNRBase1, MS_PHY u32DNRBase2, MS_PHY u32OPMBase0, MS_PHY u32OPMBase1, MS_PHY u32OPMBase2, SCALER_WIN eWindow);
INTERFACE void Hal_SC_Enable_MiuMask(void *pInstance);
INTERFACE void Hal_SC_Disable_MiuMask(void *pInstance);
INTERFACE void Hal_SC_set_miusel(void *pInstance, MS_U8 u8MIUSel);
INTERFACE void Hal_SC_get_miusel(void *pInstance, SC_MIUSEL_t* stMIUSel);
INTERFACE void Hal_SC_set_blsk(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_SC_set_blsk_burst(void *pInstance, MS_BOOL bEn);

INTERFACE void Hal_SC_set_main_black_screen_burst(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_SC_set_main_sub_black_screen_burst(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_SC_set_sub_blue_screen_burst(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue);
INTERFACE void Hal_SC_set_main_black_screen(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_SC_set_main_sub_black_screen(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_SC_set_sub_blue_screen(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue);
INTERFACE E_APIXC_ReturnValue Hal_SC_OP_Memory_Write_Request(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_IP_Memory_Write_Request(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_IP_Memory_Read_Request(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_disable_inputsource_burst(void *pInstance, MS_BOOL bDisable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_disable_inputsource(void *pInstance, MS_BOOL bDisable, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_Is_InputSource_Disable(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_nosignal_color(void *pInstance, MS_U8 u8Color,SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_fbl(void *pInstance, MS_BOOL bEn);
INTERFACE MS_BOOL Hal_SC_get_fbl(void *pInstance);
INTERFACE void HAL_SC_Set_FB_Num(void *pInstance, SCALER_WIN eWindow, XC_FRAME_STORE_NUMBER eBestFBNum, MS_BOOL bInterlace);
INTERFACE void Hal_SC_set_freezeimg_burst(void *pInstance, MS_BOOL bEn, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_freezeimg(void *pInstance, MS_BOOL bEn, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_get_freezeimg(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_ip_Init_for_internal_timing(void *pInstance, XC_Internal_TimingType timingtype, SCALER_WIN eWindow);
INTERFACE MS_U16 Hal_SC_rgb_average_info(void *pInstance, MS_U16 u16mask, SCALER_WIN eWindow);
INTERFACE void Hal_SC_autogain_enable(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_autogain_status(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_init_riu_base(MS_VIRT u32riu_base, MS_VIRT u32PMriu_base);
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
INTERFACE void Hal_SC_set_cs_det_cnt(void *pInstance, MS_U8 u8val, SCALER_WIN eWindow);
INTERFACE MS_U8 Hal_SC_get_plus_width(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_opm_fetch(void *pInstance, SCALER_WIN eWindow, MS_U16 u16OPMFetch);
INTERFACE MS_U16 Hal_SC_get_opm_fetch(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_linearmem_mode(MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_SWDS_AddCmd(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow);
INTERFACE void Hal_SC_SWDS_Fire(void *pInstance, SCALER_WIN eWindow);
#define Hal_SC_Set_2pmode(args...);

INTERFACE void HAL_SC_VOP_Set_Contrast_En(void *pInstance, MS_BOOL bEenable, SCALER_WIN eWindow);
INTERFACE void HAL_SC_VOP_Set_Contrast_Value(void *pInstance, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow);
INTERFACE void HAL_SC_VOP_Set_Brightness_En(void *pInstance, MS_BOOL bEenable, SCALER_WIN eWindow);
INTERFACE void HAL_SC_VOP_Set_Brightness_Value(void *pInstance, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val, SCALER_WIN eWindow);

INTERFACE void HAL_SC_Enable_VInitFactor(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_VInitFactorOne(void *pInstance, MS_U32 u32Value,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_VInitFactorTwo(void *pInstance, MS_U32 u32Value,SCALER_WIN eWindow);
void HAL_SC_Enable_SUBONEMode(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_vsd_input_line_count(void *pInstance, MS_BOOL bEnable,MS_BOOL bUserMode,MS_U32 u32UserLineCount,SCALER_WIN eWindow);
INTERFACE void HAL_SC_Set_vsd_output_line_count(void *pInstance, MS_BOOL bEnable,MS_U32 u32LineCount,SCALER_WIN eWindow);
INTERFACE MS_U32 Hal_SC_CheckSubWinPreScaling(MS_U16 u16ScaleDst,MS_BOOL bInterlace);
INTERFACE void HAL_SC_EnableFPLL(void);
INTERFACE MS_BOOL HAL_SC_WaitFPLLDone(void *pInstance);
INTERFACE MS_U16 HAL_SC_GetOutputVFreqX100(MS_U32 u32XTAL_Clock);
INTERFACE MS_BOOL Hal_SC_get_ip2_csc(void *pInstance, SCALER_WIN eWindow);
//INTERFACE void Hal_SC_SetBypassMode(void *pInstance, E_HAL_SC_BYPASSMODE eByPassMode);
//INTERFACE E_HAL_SC_BYPASSMODE Hal_SC_GetBypassMode(void *pInstance);

//Add for T3
INTERFACE void HAL_SC_FilmMode_Patch1(void *pInstance);

INTERFACE void HAL_SC_SetMainLineBufferOffset(void *pInstance, MS_U16 u16Linebuffer);
INTERFACE void HAL_SC_SetSubLineBufferOffset(void *pInstance, MS_U16 u16Linebuffer);
INTERFACE void HAL_SC_SetDisplay_LineBuffer_Mode(void *pInstance, MS_BOOL bEnable);
INTERFACE void HAL_SC_SetDisplay_Start_Mode(void *pInstance, MS_BOOL bEnable);
INTERFACE void HAL_SC_FillLineBuffer(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow);
INTERFACE void Hal_SC_enable_window(void *pInstance, MS_BOOL bEn,SCALER_WIN eWindow);
INTERFACE void Hal_SC_enable_window_burst(void *pInstance, MS_BOOL bEn,SCALER_WIN eWindow);
INTERFACE void Hal_SC_set_trigger_signal(void *pInstance);
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

INTERFACE MS_BOOL Hal_SC_Detect_RequestFBL_Mode(void *pInstance);
INTERFACE MS_BOOL Hal_SC_Set_RequestFBL_Mode(void *pInstance, MS_BOOL bEn);
INTERFACE void Hal_XC_SetFrameColor(void *pInstance, MS_U32 u32aRGB);
INTERFACE MS_BOOL Hal_SC_is_extra_req_en(void *pInstance, MS_U16 *pu16MainHStart, MS_U16 *pu16MainHEnd, MS_U16 *pu16SubHStart,  MS_U16 *pu16SubHEnd);
INTERFACE MS_U8 Hal_SC_getVSyncWidth(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_enable_cursor_report(void *pInstance, MS_BOOL bEn);

//Add for K1
INTERFACE E_APIXC_ReturnValue Hal_SC_Set_OSDLayer(void *pInstance, E_VOP_OSD_LAYER_SEL  eVOPOSDLayer, SCALER_WIN eWindow);
INTERFACE E_VOP_OSD_LAYER_SEL Hal_SC_Get_OSDLayer(void *pInstance, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_Set_VideoAlpha(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_Get_VideoAlpha(void *pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue Hal_SC_SetOSDBlendingFormula(void *pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow);
INTERFACE void Hal_XC_ClearScalingFactorForInternalCalib( void *pInstance);

INTERFACE MS_U8 MHal_SC_cal_usedgain_to_reggain(MS_U32 u16UsedGain_x32);

INTERFACE E_APIXC_ReturnValue Hal_SC_OP2VOPDESel(void *pInstance, E_OP2VOP_DE_SEL eVopDESel);
INTERFACE MS_BOOL Hal_XC_SVOutput_GetCaps(void *pInstance);

#define _MHal_SC_Flock_Caculate_LPLLSet(args...)
#define _MHal_SC_Set_LPLL_Limit(args...)
#define _MHal_SC_Flock_Set_IGainPGain(args...)
#define _MHal_SC_Flock_Set_LPLL_Enable(args...)

#define MHal_LPLL_TGen_Src_Select(args...)
#define MHal_CLKGEN_FRC_Init(args...)
#define MHal_CLKGEN_FRC_Bypass_Enable(args...)
#define Hal_XC_ReportPixelInfo(args...)  FALSE

#define Hal_XC_IsForcePrescaling_3D Hal_XC_IsForcePrescaling
INTERFACE E_APIXC_ReturnValue Hal_XC_IsForcePrescaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pbForceV, MS_BOOL *pbForceH, SCALER_WIN eWindow);
INTERFACE MS_U32 MHal_SC_Get_LpllSet_Factor(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 u8LPLL_Type, MS_U32 u32DefaultDClk);
#define MHal_SC_Get_LpllSet_Div(args...) 1

INTERFACE E_APIXC_ReturnValue Hal_SC_Enable_AVMute(void *pInstance, SCALER_WIN eWindow);

INTERFACE void MHal_XC_Calc_IGainPGain(void *pInstance, MS_U8 *u8GainI, MS_U8 *u8GainP, MS_U32 u32XTAL_Clock, MS_U8 u8LGain, MS_U8 u8Vco, MS_U16 u16HTotal, MS_U16 u16VTotal, MS_U8 u8FRC_Out);

#define Hal_SC_set_mrq_miusel(args...)
#define Hal_SC_get_mcdi_memoryaddressfromreg(args...) 0
#define Hal_SC_set_mcdi_memoryaddress(args...)
#define Hal_SC_set_mcdi_write_limit(args...)
#define Hal_SC_enable_mcdi(args...)

#define Hal_SC_set_bws_mode(args...)
#define Hal_SC_sw_lcnt_en(args...)
#define Hal_SC_set_sw_lcnt(args...)
#define Hal_SC_set_ipmw_lcnt_inv(args...)
#define Hal_SC_set_ipmr_lcnt_inv(args...)
#define Hal_SC_set_opm_lcnt_inv(args...)

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
#define MHal_XC_Init_Patch(args...) FALSE
INTERFACE E_APIXC_ReturnValue Hal_SC_SetOSDDetect(void *pInstance, MS_BOOL bEnable, MS_U32 Threhold);
INTERFACE E_APIXC_ReturnValue Hal_SC_GetOSDDetect(void *pInstance, MS_BOOL *pbOSD);

INTERFACE void MHal_XC_SetForceReadBank(void *pInstance, MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow);
#define MHal_XC_SetForceCurrentReadBank(args...)

INTERFACE MS_BOOL MHal_XC_SetDNRBufAddress(void *pInstance, MS_PHY u32DNRBaseAddr, SCALER_WIN eWindow);
INTERFACE MS_PHY MHal_XC_GetDNRBufAddress(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MHal_XC_SetDNRBufSize(void *pInstance, MS_PHY u32DNRBufSize, SCALER_WIN eWindow);
INTERFACE MS_PHY MHal_XC_GetDNRBufSize(void *pInstance, SCALER_WIN eWindow);
INTERFACE void Hal_SC_SetPX2MemFormat(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL Hal_SC_IsPX2MemFormat(void *pInstance, SCALER_WIN eWindow);

//Vtrack
INTERFACE E_APIXC_ReturnValue MHal_XC_Vtrack_SetPayloadData(void *pInstance, MS_U16 u16Timecode, MS_U8 u8OperatorID);
INTERFACE E_APIXC_ReturnValue MHal_XC_Vtrack_SetUserDefindedSetting(void *pInstance, MS_BOOL bUserDefinded, MS_U8 *pu8Setting);
INTERFACE E_APIXC_ReturnValue MHal_XC_Vtrack_Enable(void *pInstance, MS_U8 u8FrameRate, MS_BOOL bEnable);
INTERFACE void HAL_SC_Set_LB_MergeAddress(void *pInstance);
INTERFACE MS_BOOL MHal_XC_IsPNLYUVOutput(void *pInstance);

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
INTERFACE void MHal_XC_CMAPatch(void *pInstance, SCALER_WIN eWindow);
INTERFACE void MHal_XC_CMAPatchClose(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MHal_XC_Get_CMA_UsingCondition(void *pInstance, MS_BOOL bIsGetCMABuff[], MS_U32 au32CMAMemSCMSize[],MS_U32 au32CMAMemFRCMSize[], MS_U32* pu32DualMiuMemSize, MS_U32 u32DataSize, SCALER_WIN eWindow);
INTERFACE MS_BOOL MHal_XC_Release_CMA(void *pInstance, XC_CMA_CLIENT enCmaClient, SCALER_WIN eWindow);
INTERFACE MS_U64 MHal_XC_Get_CMA_Addr(void *pInstance, XC_CMA_CLIENT enCmaClient, MS_U32 u32GetCMASize, SCALER_WIN eWindow);
#endif
#endif

INTERFACE MS_BOOL Hal_SC_Init(void *pInstance);
INTERFACE MS_U32 Hal_SC_Get_Device_Offset(MS_U8 deviceIdx);
INTERFACE MS_BOOL Hal_SC_3D_Is_LR_Sbs2Line(void *pInstance);

INTERFACE void Hal_SC_3D_SetLRChgMode(void *pInstance, HAL_SC_3D_LRCHGMODE eLRChgMode);
INTERFACE void Hal_SC_3D_SetInitialLRIndex(void *pInstance, MS_BOOL bRFirst);
INTERFACE void Hal_SC_3D_SetSplitHalf(void *pInstance, MS_BOOL bEnable);
INTERFACE void Hal_SC_3D_SetActiveVideoHeight(void *pInstance, MS_U16 u16VideoSize);
INTERFACE void Hal_SC_3D_SetActiveBlankSize0(void *pInstance, MS_U16 u16BlankSize);
INTERFACE void Hal_SC_3D_SetActiveBlankSize1(void *pInstance, MS_U16 u16BlankSize);
INTERFACE void Hal_SC_3D_SetActiveVideoWidth(void *pInstance, MS_U16 u16VideoSize);
INTERFACE void Hal_SC_3D_SetActiveVideoHeightAfterVSD(void *pInstance, MS_U16 u16VideoSize);
INTERFACE void Hal_SC_3D_SetFRCMActiveVideoHeightAfterVSD(void *pInstance, MS_U16 u16VideoSize);
INTERFACE void Hal_SC_3D_SetPixelSeparationWidth(void *pInstance, MS_U16 u16VideoSize);
INTERFACE void Hal_SC_3D_enable_FALLRR_out(void *pInstance, MS_BOOL bEnable);
INTERFACE void Hal_SC_3D_Adjust_FP_PreHscaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling);
INTERFACE void Hal_SC_3D_Adjust_FA_PreHscaling(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling);
INTERFACE void Hal_SC_3D_Adjust_PreHVscaling_SaveBW(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL *pb3DPreHScaling,MS_BOOL *pb3DPreVScaling,MS_BOOL bForcePreHScalingDown,MS_BOOL bForcePreVScalingDown,SCALER_WIN eWindow);
INTERFACE void Hal_SC_3D_InvertMemsyncInterlaceMode(void *pInstance);

#define HAL_SC_Set_vsd_3D_autofactor_reset(args...)
#define MHal_SC_set_manual_rbank_switch_cnt(args...)
#define Hal_SC_set_frcm_freezeimg(args...)
#define Hal_SC_set_frcm_memoryaddress(args...)
#define Hal_SC_set_frcm_write_limit(args...)
#define Hal_SC_set_frcm_mirror(args...)
#define Hal_SC_set_edclk(args...)
#define Hal_SC_set_ficlk2(args...)
#define Hal_SC_set_fmclk(args...)
#define Hal_SC_set_T3D_setting(args...)
#define Hal_SC_set_T3D_H_size(args...)
#define HAL_SC_EnableLegacyMode(args...)

INTERFACE void Hal_SC_SetHDMI_Spliter(void *pInstance,XC_IP_SYNC_STATUS *sXC_Sync_Status,E_MUX_INPUTPORT enInputPort ,SCALER_WIN eWindow);

// Temp function for SC1 to load MADi settings
INTERFACE MS_U8 Hal_SC1_LoadMADi(void *pInstance, SCALER_WIN eWindow, MS_U8 *pu8BitPerPixel);
#ifdef UFO_XC_SETBLACKVIDEOBYMODE
INTERFACE E_APIXC_ReturnValue Hal_SC_set_main_black_screen_by_mode_burst(void *pInstance, MS_BOOL bEn, EN_XC_BLACK_VIDEO_MODE eMode);
INTERFACE E_APIXC_ReturnValue Hal_SC_set_sub_blue_screen_by_mode_burst(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue, EN_XC_BLACK_VIDEO_MODE eMode);
INTERFACE E_APIXC_ReturnValue Hal_SC_set_main_black_screen_by_mode(void *pInstance, MS_BOOL bEn, EN_XC_BLACK_VIDEO_MODE eMode);
INTERFACE E_APIXC_ReturnValue Hal_SC_set_sub_blue_screen_by_mode(void *pInstance, MS_BOOL bEn, MS_BOOL bBlue, EN_XC_BLACK_VIDEO_MODE eMode);
#endif
#define Hal_SC_get_pre_align_pixel(args...) FALSE
INTERFACE MS_BOOL Hal_SC_CheckMuteStatusByRegister(void *pInstance, SCALER_WIN eWindow);

//OTT
INTERFACE void Hal_SC_add_reg_to_shm(void *pInstance,SCALER_WIN eWindow,MS_U32 u32_bk,MS_U16 u16_value,MS_U16 u16_mask);
INTERFACE void Hal_SC_add_dram_to_shm(void *pInstance,SCALER_WIN eWindow,MS_PHY u32address,MS_U32 u32length);
INTERFACE MS_BOOL Hal_SC_secure_lock(void *pInstance, SCALER_WIN eWindow, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
INTERFACE void Hal_SC_update_to_shm(void *pInstance, SCALER_WIN eWindow);

#ifdef ENABLE_SPREADMODE
//not implemnet
#define MHal_XC_IsSupportSpreadMode(args...) FALSE
#endif

#ifdef UFO_XC_AUTO_DOWNLOAD
INTERFACE E_APIXC_ReturnValue MHal_XC_AutoDownload_Config(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_PHY phyBaseAddr, EN_XC_AUTODOWNLOAD_MODE enMode);
INTERFACE E_APIXC_ReturnValue MHal_XC_AutoDownload_Write(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_U8* pu8Data, MS_U32 u32Size, void* pParam);
INTERFACE E_APIXC_ReturnValue MHal_XC_AutoDownload_Fire(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient);
INTERFACE E_APIXC_ReturnValue MHal_XC_GetAutoDownloadCaps(EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_BOOL *pbSupported);
#endif

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
INTERFACE E_APIXC_ReturnValue MHal_XC_HDR_Control(void* pInstance, EN_XC_HDR_CTRL_TYPE enCtrlType, void *pParam);
INTERFACE E_APIXC_ReturnValue MHal_XC_HDR_GetCaps(void *pInstance, XC_HDR_SUPPORTED_CAPS *pstHDRCaps);
#endif
#endif

//VMark
INTERFACE E_APIXC_ReturnValue MHal_XC_VMark_SetParameters(void* pInstance, MS_U8 u8ServiceIndex, XC_VMARK_PARAMETER *pstXC_VMarkParameters);

#ifdef CONFIG_MSTAR_SRAMPD
INTERFACE MS_BOOL Hal_SC_Power_OnOff(void *pInstance, MS_BOOL bEnable);
#endif

INTERFACE E_XC_FB_LEVEL MHal_XC_Get_FB_Level(void* pInstance, MS_WINDOW_TYPE* pstCropWin, MS_WINDOW_TYPE* pstDispWin, MS_BOOL bInterlace, MS_U16* pu16HSize, MS_U16* pu16VSize);
INTERFACE MS_U32 Hal_SC_get_HDMIpolicy(void *pInstance);

#undef INTERFACE
#endif /* MHAL_SC_H */
