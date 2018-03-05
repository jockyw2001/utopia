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
// Copyright (c) 2006-2008 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _MHALTVENCODER_H
#define _MHALTVENCODER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
//Video Encoder Capability
#define VE_SUPPORTED            TRUE
#define ENABLE_VE_SUBTITLE      TRUE
//Feature for: ve support isr from TVS(VE Source), and HW will report current VE Write frame index
#define VE_TVS_ISR_SUPPORT      TRUE
#define VE_SUPPORT_SOURCE_SCALER_OP 0

//Feature: predownscaling before TVS
#define VE_PRESCALE_SUPPORT     FALSE

#define ENABLE_VE_FIELD_INV       0

#define VE_DE_HEND_MASK 0x1FFF //0x3B13
#define VE_DE_VEND_MASK 0xFFF  //0x3B15

#define VE_MIU_DATA_LENGTH      256
#define BYTE_PER_WORD           (VE_MIU_DATA_LENGTH/8)

//VE customized table max size
#define VE_CUSTBL_MAX_SIZE 64 //4bytes * 16sets (include end of table)

#define REG_CKG_VE_IN           0x1E38
#define CKG_VE_IN_GATED         BIT(0)
#define CKG_VE_IN_INVERT        BIT(1)
#define CKG_VE_IN_MASK          (BIT(5) | BIT(4) | BIT(3) | BIT(2))
#define CKG_VE_IN_CLK_ADC       (0 << 2)
#define CKG_VE_IN_CLK_DVI       (1 << 2)
#define CKG_VE_IN_CLK_VD        (2 << 2)
#define CKG_VE_IN_CLK_MPEG0     (3 << 2)
#define CKG_VE_IN_1             (4 << 2) //IDclk2 for IP_MAIN
#define CKG_VE_IN_CLK_EXT_DI    (5 << 2)
#define CKG_VE_IN_0             (6 << 2) //Sub MPEG0
#define CKG_VE_IN_0_            (7 << 2) //ODclk
#define CKG_VE_IN_DFT_LIVE      (8 << 2)
#define CKG_VE_IN_EDCLK         (9 << 2) //edclk
#define CKG_VE_IN_CLK_DI        (10 << 2)

#define VE_NTSC_FRAME_SIZE  ((MS_U32)(720ul * 480ul)*16/VE_MIU_DATA_LENGTH) // 720*480*16bits/256bits
#define VE_PAL_FRAME_SIZE   ((MS_U32)(720ul * 576ul)*16/VE_MIU_DATA_LENGTH) // 720*576*16bits/256bits

 //VE out
#define VE_OUT_CVBS_YCC         0x00
#define VE_OUT_CVBS_YCbCr       0x01
#define VE_OUT_CVBS_RGB         0x02
#define VE_OUT_NONE             0xFF

#define VE_V_SCALE_DOWN_RATIO(IN, OUT)  (((MS_U32)(OUT) * 2048ul / (MS_U32)(IN))+1)
#define VE_H_SCALE_DOWN_RATIO(IN, OUT)  ((MS_U32)(OUT) * 2048ul / (MS_U32)(IN))
#define VE_V_SCALE_UP_RATIO(IN, OUT)  ((MS_U32)(IN - 1) * 0x100000ul / (MS_U32)(OUT - 1))
#define VE_H_SCALE_UP_RATIO(IN, OUT)  ((MS_U32)(IN) * 0x100000ul/ (MS_U32)(OUT))

#define VE_NEW_FRC_SETTING
#define VE_SUPPORT_CROP // DE is not continous, use crop mode instead of non-de mode

typedef enum
{
    EN_VE_DEMODE         = 0x01,
    EN_VE_CCIR656_IN     = 0x02,
    EN_VE_RGB_IN         = 0x04,
    EN_VE_INVERSE_HSYNC  = 0x08,
}EN_VE_DISPLAY_STATUS;

//MV Type
typedef enum
{
    MS_VE_MV_NTSC_START = 0,
    MS_VE_MV_NTSC_TYPE1 = MS_VE_MV_NTSC_START,
    MS_VE_MV_NTSC_TYPE2,
    MS_VE_MV_NTSC_TYPE3,
    MS_VE_MV_NTSC_TEST_N01,
    MS_VE_MV_NTSC_TEST_N02,
    MS_VE_MV_NTSC_TYPE2_TTX,
    MS_VE_MV_NTSC_TEST_N01_TTX,
    MS_VE_MV_PAL_START = 0x10,
    MS_VE_MV_PAL_TYPE1 = MS_VE_MV_PAL_START,
    MS_VE_MV_PAL_TYPE2,
    MS_VE_MV_PAL_TYPE3,
    MS_VE_MV_PAL_TEST_P01,
    MS_VE_MV_PAL_TEST_P02,
    MS_VE_MV_PAL_TYPE2_TTX,
    MS_VE_MV_PAL_TEST_P01_TTX,
} MS_VE_MV_TYPE;

/*
 * This Enum type was defined base on DCS for composite (CVBS)
 */
typedef enum
{
    MS_VE_DCS_TYPE_MIN = 0,
    MS_VE_DCS_CVBS_NTSC = MS_VE_DCS_TYPE_MIN,
    MS_VE_DCS_CVBS_PAL,
    MS_VE_DCS_TYPE_MAX = MS_VE_DCS_CVBS_PAL,
    MS_VE_DCS_TYPE_NUM,
} MS_VE_DCS_TYPE;

//VE capability struct
typedef struct
{
    MS_BOOL bSupport_UpScale;
    MS_BOOL bSupport_CropMode;
    MS_BOOL bSupport_DolbyVerifier;
}MS_VE_Cap;

typedef enum
{
    MS_VE_DRAM_FORMAT_Y8C8 = 0,
    MS_VE_DRAM_FORMAT_Y8C8M4,
}MS_VE_DRAM_FORMAT;

typedef enum
{
    MS_VE_INT_TVE_FIELD = 0,
    MS_VE_INT_TVE_VSYNC_TVE_FIELD,
    MS_VE_INT_TVS_VSYNC,
}MS_VE_INT_TYPE;

typedef struct
{
    MS_U8   u8CPC[2];
    MS_U8   u8CPS[34];
}MS_VE_MV_ByteInfo;

typedef struct
{
    //Versioning
    MS_U32  u32VE_MV_BitControl_version;     ///<Version of current structure.
    MS_U32  u32VE_MV_BitControl_Length;      ///<Length of this structure, u32VE_MV_BitControl_Length=sizeof(MS_VE_MV_BitControl)
    //Data
    MS_BOOL bIsYPbPr;
    union
    {
        MS_U8              u8MV_ByteInfo[36];
        MS_VE_MV_ByteInfo  MV_ByteInfo;
    }__attribute__ ((__packed__)) ;
}MS_VE_MV_BitControl;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

void MDrv_VE_PowerOn(void);
void MDrv_VE_PowerOff(void);
void Hal_VE_set_fix_color_out(MS_BOOL ben, MS_U8 u8color_y, MS_U8 u8color_cb, MS_U8 u8color_cr);

void Hal_VE_set_wss_data(MS_BOOL ben, MS_U16 u16wssdata);
MS_U16 Hal_VE_get_wss_data(void);
void Hal_VE_set_color_convert(MS_VE_VIDEOSYS VideoSys, MS_U8 u8DACType);

void Hal_VE_init(void);
MS_BOOL Hal_VE_set_output_video_std(MS_VE_VIDEOSYS VideoSystem);
MS_VE_VIDEOSYS Hal_VE_Get_Output_Video_Std(void);

MS_BOOL Hal_VE_SetCusTable(MS_VE_VIDEOSYS VideoSystem, MS_U8* pu8Tbl, MS_U16 u16size);
void Hal_VE_EnableCusTable(MS_BOOL bEnable);

void Hal_VE_set_capture_window(MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend);
void Hal_VE_set_hsync_inverse(MS_BOOL bEn);
void Hal_VE_set_cvbs_buffer_out(MS_U16 u16chnl, MS_U16 u16mux, MS_U16 u16clamp, MS_U16 u16test);
void Hal_VE_set_frc(MS_BOOL bfulldrop, MS_U32 u32fullnum, MS_U32 u32emptynum, MS_BOOL binterlace, MS_BOOL b3FrameMode);
void Hal_VE_set_field_inverse(MS_BOOL ben);
void Hal_VE_set_ccir656_out_pal(MS_BOOL bpal);
void Hal_VE_sofeware_reset(MS_BOOL ben);
void Hal_VE_set_vbi(MS_BOOL ben);
void Hal_VE_set_reg_load(MS_BOOL ben);
void Hal_VE_set_ctrl(MS_U16 u16enval);
void Hal_VE_set_h_scaling(MS_BOOL ben, MS_U16 u16ratio);
void Hal_VE_set_v_scaling(MS_BOOL ben, MS_U16 u16ratio);
void Hal_VE_set_v_scaling_Traditional(MS_BOOL ben, MS_U16 u16ratio);
void Hal_VE_set_v_initfactor(MS_BOOL binterlace, MS_U32 u32V_Ratio);
void Hal_VE_set_out_sel(MS_U8 u8out_sel);
void Hal_VE_set_source_sync_inv(MS_U8 u8val, MS_U8 u8Mask);
void Hal_VE_set_sog(MS_BOOL ben);
void Hal_VE_set_inputsource(PMS_VE_InputSrc_Info pInputSrcInfo);
void Hal_VE_set_mux(MS_U8 u8clk, MS_U8 u8data);
void Hal_VE_set_rgb_in(MS_BOOL ben);
void Hal_VE_set_ccir656_in(MS_BOOL ben);
void Hal_VE_set_source_interlace(MS_BOOL ben);
void Hal_VE_set_clk_on_off(MS_BOOL ben);
void Hal_VE_set_ve_on_off(MS_BOOL ben);
void Hal_VE_set_blackscreen(MS_BOOL ben);
MS_BOOL Hal_VE_is_blackscreen_enabled(void);
void Hal_VE_init_riu_base(MS_VIRT virtRiu_Base);
void Hal_VE_EnableTtx(MS_BOOL bEnable);
void Hal_VE_Set_ttx_Buffer(MS_PHY u32StartAddr, MS_U32 u32Size);
void Hal_VE_Clear_ttxReadDoneStatus(void);
MS_BOOL Hal_VE_Get_ttxReadDoneStatus(void);
void Hal_VE_Set_VbiTtxActiveLines(MS_U8 u8LinePerField);
void Hal_VE_SetVbiTtxRange(MS_U16 odd_start, MS_U16 odd_end,
                         MS_U16 even_start, MS_U16 enen_end);
void Hal_VE_Set_VbiTtxActiveLinesBitmap(MS_U32 u32Bitmap);
void Hal_VE_Set_winodw_offset(MS_PHY phyoffsetaddr);
MS_U32 Hal_VE_ConvertARGB2YUV(MS_U32 u32aRGB);
VE_Result Hal_VE_SetFrameColor(MS_U32 u32aRGB);
void Hal_VE_Set_OSD(MS_BOOL bEnable);
VE_Result Hal_VE_Set_OSDLayer(MS_VE_OSD_LAYER_SEL eVideoOSDLayer);
MS_VE_OSD_LAYER_SEL Hal_VE_Get_OSDLayer(void);
VE_Result Hal_VE_Set_VideoAlpha(MS_U8 u8Val);
VE_Result Hal_VE_Get_VideoAlpha(MS_U8 *pu8Val);
void Hal_VE_EnableCcSw(MS_BOOL bEnable);
void Hal_VE_SetCcRange(MS_U16 odd_start, MS_U16 odd_end, MS_U16 even_start, MS_U16 enen_end);
void Hal_VE_SendCcData(MS_BOOL bIsOdd, MS_U16 data);
void Hal_VE_Set_TestPattern(MS_BOOL ben);

void Hal_VE_W2BYTE_MSK(MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask);
MS_U16 Hal_VE_R2BYTE_MSK(MS_U32 u32Reg, MS_U16 u16Mask);

//Dump Table
void Hal_VE_DumpTable(MS_U8 *pVeTable, MS_U8 u8TableType);

//Set MV
VE_Result Hal_VE_SetMV(MS_BOOL bEnable, MS_VE_MV_TYPE eMvType);
VE_Result Hal_VE_SetMV_BitControl(MS_BOOL bEnable, MS_VE_MV_BitControl MV_BitControl_Data);

//Set DCS
VE_Result Hal_VE_DCS_SetType(MS_BOOL bEnable, MS_VE_DCS_TYPE eDCSType);
VE_Result Hal_VE_DCS_SetActivationKey(MS_U8 *pu8ActivationKeyTbl, MS_U8 u8ActivationKeyTblSize);

MS_BOOL Hal_VE_DisableRegWrite_GetCaps(void);

// show internal color bar
// TRUE: enable color bar
// FALSE: disable color bar
void Hal_VE_ShowColorBar(MS_BOOL bEnable);
VE_Result Hal_VE_AdjustPositionBase(MS_S32 s32WAddrAdjustment, MS_S32 s32RAddrAdjustment);
void Hal_VE_set_field_size(MS_U16 u16FieldSize);
void Hal_VE_SelMIU(MS_U8 u8MIU_Seltype);
void Hal_VE_SetMemAddr(MS_PHY *phyRDAddress, MS_PHY *phyWRAddress);
VE_Result HAL_VE_SetFrameLock(MS_U32 u32IDclk, MS_U32 u32ODclk, MS_U32 u32InitPll, MS_BOOL bEnable);
VE_Result Hal_VE_GetCaps(MS_VE_Cap *cap);
void Hal_VE_set_h_upscaling(MS_BOOL ben, MS_U32 u32ratio);
void Hal_VE_set_v_upscaling(MS_BOOL ben, MS_U32 u32ratio);
MS_VE_DRAM_FORMAT Hal_VE_Get_DRAM_Format(void);
void Hal_VE_Set_DRAM_Format(MS_VE_DRAM_FORMAT fmt);
MS_U8 Hal_VE_Get_Pack_Num_Per_Line(void);
void Hal_VE_Set_Pack_Num_Per_Line(MS_U8 PackNumPerLine);
MS_U16 Hal_VE_Get_Field_Line_Number(void);
void Hal_VE_Set_Field_Line_Number(MS_U16 FieldLineNumber);
void Hal_VE_Set_Crop_OSD_Offset(MS_U16 hstart, MS_U16 vstart);
void Hal_VE_Set_VScale_Output_Line_Number(MS_U16 line);
void Hal_VE_Set_HScale_Output_Line_Number(MS_U16 line);
VE_Result Hal_VE_Adjust_FrameStart(MS_S16 pixel_offset);
MS_BOOL Hal_VE_ISRConfig(MS_VE_INT_TYPE eIntType);
VE_Result Hal_VE_set_wss525_data(MS_BOOL ben, MS_U32 u32wssdata);
MS_U32 Hal_VE_get_wss525_data(void);
void Hal_VE_OnOffWSS(MS_BOOL ben);
MS_BOOL Hal_VE_GetWSSStatus(void);
void Hal_VE_OnOffMV(MS_BOOL ben);
MS_BOOL Hal_VE_GetMVStatus(void);
void Hal_VE_OnOffDCS(MS_BOOL ben);
MS_BOOL Hal_VE_GetDCSStatus(void);
#define Hal_VE_set_dolby_verifier_addr(args...)
MS_U32 Hal_VE_get_dolby_verifier_buf_size(void);
void Hal_VE_Set_PreHSD(MS_VE_INPUT_SRC_TYPE eInputSrcType,MS_U16 *u16In_Hsize,MS_U16 *u16In_Vsize);
void Hal_VE_set_crop_window(MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend);
VE_Result Hal_VE_ReloadSetting(void);

#ifdef __cplusplus
}
#endif

#endif

