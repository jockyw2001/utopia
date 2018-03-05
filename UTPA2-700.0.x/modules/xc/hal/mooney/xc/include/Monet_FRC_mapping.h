////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (; MStar; Confidential; Information; ) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
//****************************************************
//   FRC Map CodeGen Version 2.0
// FRC_mapping
// Monet
// 7/29/2013  3:34:04 PM
// 39630.43853
//****************************************************

#ifndef _MONET_FRC_MAPPING_MODE_H_
#define _MONET_FRC_MAPPING_MODE_H_

#define FRC__NUM_FRC_Mapping_mode 17

typedef enum
{
    FRC_FRC_BYPASS_FRC_Mapping_mode, //0
    FRC_FHD_60_IN_4K2K_60_OUT_FRC_Mapping_mode, //1
    FRC_4K2K_24_IN_4K2K_60_OUT_FRC_Mapping_mode, //2
    FRC_4K2K_30_IN_4K2K_60_OUT_FRC_Mapping_mode, //3
    FRC_INPUTTYPE_NUM_FRC_Mapping_mode, // 4
} FRC_MAP_INDEX_e_FRC_Mapping_mode;

#endif
typedef enum
{
FRC_R2Y_FRC_Mapping_mode,  //0
FRC_MRW_FRC_Mapping_mode,  //1
FRC_HSD_FRC_Mapping_mode,  //2
FRC_444to422_FRC_Mapping_mode,  //3
FRC_VCE_FRC_Mapping_mode,  //4
FRC_IPM_OPM_FRC_Mapping_mode,  //5
FRC_VDE_FRC_Mapping_mode,  //6
FRC_422to444_FRC_Mapping_mode,  //7
FRC_SPLT_FRC_Mapping_mode,  //8
FRC_HVSP_FRC_Mapping_mode,  //9
FRC_VIP_HLPF_FRC_Mapping_mode,  //10
FRC_VIP_HLPF_dither_FRC_Mapping_mode,  //11
FRC_VIP_Peaking_FRC_Mapping_mode,  //12
FRC_VIP_Peaking_band_FRC_Mapping_mode,  //13
FRC_VIP_Peaking_Pcoring_FRC_Mapping_mode,  //14
FRC_VIP_Peaking_gain_FRC_Mapping_mode,  //15
FRC_Y2R_FRC_Mapping_mode,  //16
 }   FRC_IPTYPE_FRC_Mapping_mode;

typedef enum
{
FRC_R2Y_OFF_FRC_Mapping_mode,
FRC_R2Y_ON_FRC_Mapping_mode,
FRC_R2Y_NUMS_FRC_Mapping_mode
} FRC_R2Y_Group_FRC_Mapping_mode;

typedef enum
{
FRC_MRW_OFF_FRC_Mapping_mode,
FRC_MRW_ON_FRC_Mapping_mode,
FRC_MRW_NUMS_FRC_Mapping_mode
} FRC_MRW_Group_FRC_Mapping_mode;

typedef enum
{
FRC_HSD_OFF_FRC_Mapping_mode,
FRC_HSD_NUMS_FRC_Mapping_mode
} FRC_HSD_Group_FRC_Mapping_mode;

typedef enum
{
FRC_444to422_OFF_FRC_Mapping_mode,
FRC_444to422_ON_FRC_Mapping_mode,
FRC_444to422_NUMS_FRC_Mapping_mode
} FRC_444to422_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VCE_OFF_FRC_Mapping_mode,
FRC_VCE_ON_FRC_Mapping_mode,
FRC_VCE_NUMS_FRC_Mapping_mode
} FRC_VCE_Group_FRC_Mapping_mode;

typedef enum
{
FRC_IPM_OPM_OFF_FRC_Mapping_mode,
FRC_IPM_OPM_4k2k_FRC_Mapping_mode,
FRC_IPM_OPM_24_60_4k2k_FRC_Mapping_mode,
FRC_IPM_OPM_30_60_4k2k_FRC_Mapping_mode,
FRC_IPM_OPM_fullHD_FRC_Mapping_mode,
FRC_IPM_OPM_24_60_fullHD_FRC_Mapping_mode,
FRC_IPM_OPM_30_60_fullHD_FRC_Mapping_mode,
FRC_IPM_OPM_NUMS_FRC_Mapping_mode
} FRC_IPM_OPM_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VDE_OFF_FRC_Mapping_mode,
FRC_VDE_ON_FRC_Mapping_mode,
FRC_VDE_NUMS_FRC_Mapping_mode
} FRC_VDE_Group_FRC_Mapping_mode;

typedef enum
{
FRC_422to444_OFF_FRC_Mapping_mode,
FRC_422to444_ON_FRC_Mapping_mode,
FRC_422to444_NUMS_FRC_Mapping_mode
} FRC_422to444_Group_FRC_Mapping_mode;

typedef enum
{
FRC_SPLT_OFF_FRC_Mapping_mode,
FRC_SPLT_ON_FRC_Mapping_mode,
FRC_SPLT_NUMS_FRC_Mapping_mode
} FRC_SPLT_Group_FRC_Mapping_mode;

typedef enum
{
FRC_HVSP_OFF_FRC_Mapping_mode,
FRC_HVSP_hsp_4k_to_4k_FRC_Mapping_mode,
FRC_HVSP_hsp_2k_to_2k_FRC_Mapping_mode,
FRC_HVSP_hsp_2k_to_4k_FRC_Mapping_mode,
FRC_HVSP_hsp_4k_to_2k_FRC_Mapping_mode,
FRC_HVSP_NUMS_FRC_Mapping_mode
} FRC_HVSP_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VIP_HLPF_OFF_FRC_Mapping_mode,
FRC_VIP_HLPF_0x1_FRC_Mapping_mode,
FRC_VIP_HLPF_0x2_FRC_Mapping_mode,
FRC_VIP_HLPF_0x3_FRC_Mapping_mode,
FRC_VIP_HLPF_0x4_FRC_Mapping_mode,
FRC_VIP_HLPF_0x5_FRC_Mapping_mode,
FRC_VIP_HLPF_0x6_FRC_Mapping_mode,
FRC_VIP_HLPF_0x7_FRC_Mapping_mode,
FRC_VIP_HLPF_NUMS_FRC_Mapping_mode
} FRC_VIP_HLPF_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VIP_HLPF_dither_OFF_FRC_Mapping_mode,
FRC_VIP_HLPF_dither_ON_FRC_Mapping_mode,
FRC_VIP_HLPF_dither_NUMS_FRC_Mapping_mode
} FRC_VIP_HLPF_dither_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VIP_Peaking_OFF_FRC_Mapping_mode,
FRC_VIP_Peaking_ON_FRC_Mapping_mode,
FRC_VIP_Peaking_NUMS_FRC_Mapping_mode
} FRC_VIP_Peaking_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VIP_Peaking_band_RF_NTSC_FRC_Mapping_mode,
FRC_VIP_Peaking_band_RF_SECAM_FRC_Mapping_mode,
FRC_VIP_Peaking_band_AV_NTSC_FRC_Mapping_mode,
FRC_VIP_Peaking_band_AV_PAL_FRC_Mapping_mode,
FRC_VIP_Peaking_band_AV_SECAM_FRC_Mapping_mode,
FRC_VIP_Peaking_band_SV_NTSC_FRC_Mapping_mode,
FRC_VIP_Peaking_band_480i_FRC_Mapping_mode,
FRC_VIP_Peaking_band_720p_FRC_Mapping_mode,
FRC_VIP_Peaking_band_1080i_FRC_Mapping_mode,
FRC_VIP_Peaking_band_H_480i_FRC_Mapping_mode,
FRC_VIP_Peaking_band_H_576i_FRC_Mapping_mode,
FRC_VIP_Peaking_band_H_720p_FRC_Mapping_mode,
FRC_VIP_Peaking_band_H_1080i_FRC_Mapping_mode,
FRC_VIP_Peaking_band_DT_MPEG2_480is_FRC_Mapping_mode,
FRC_VIP_Peaking_band_DT_MPEG2_720p_FRC_Mapping_mode,
FRC_VIP_Peaking_band_DT_MPEG2_1080i_FRC_Mapping_mode,
FRC_VIP_Peaking_band_DT_H264_480is_FRC_Mapping_mode,
FRC_VIP_Peaking_band_DT_H264_720p_FRC_Mapping_mode,
FRC_VIP_Peaking_band_PC_mode_FRC_Mapping_mode,
FRC_VIP_Peaking_band_NUMS_FRC_Mapping_mode
} FRC_VIP_Peaking_band_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VIP_Peaking_Pcoring_S00_0_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S21_1_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S30_0_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S30_1_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S31_0_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S31_1_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S20_1_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S32_0_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S32_1_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_S42_1_FRC_Mapping_mode,
FRC_VIP_Peaking_Pcoring_NUMS_FRC_Mapping_mode
} FRC_VIP_Peaking_Pcoring_Group_FRC_Mapping_mode;

typedef enum
{
FRC_VIP_Peaking_gain_0x18_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x1C_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x20_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x24_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x28_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x2C_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x30_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x34_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_0x38_FRC_Mapping_mode,
FRC_VIP_Peaking_gain_NUMS_FRC_Mapping_mode
} FRC_VIP_Peaking_gain_Group_FRC_Mapping_mode;

typedef enum
{
FRC_Y2R_OFF_FRC_Mapping_mode,
FRC_Y2R_ON_FRC_Mapping_mode,
FRC_Y2R_NUMS_FRC_Mapping_mode
} FRC_Y2R_Group_FRC_Mapping_mode;

