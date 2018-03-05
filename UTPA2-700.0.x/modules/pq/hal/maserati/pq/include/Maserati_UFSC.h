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
//   Quality Map CodeGen Version 4.0
// 1920
// Maserati
// 8/30/2016  4:01:27 PM
// 42612.6676736
//****************************************************

#ifndef _MASERATI_UFSC_H_
#define _MASERATI_UFSC_H_

#define PQ_IP_NUM_UFSC 111
#define PQ_IP_VIP_ICC_CRD_SRAM_SIZE_UFSC 2178

typedef enum
{
    #if PQ_QM_HMDI
    QM_4K2K_RGB444_UFSC, //0
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV444_UFSC, //1
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV422_UFSC, //2
    #endif
    #if PQ_QM_HMDI
    QM_FHD_RGB444_UFSC, //3
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV444_UFSC, //4
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV422_UFSC, //5
    #endif
    #if PQ_QM_HMDI
    QM_FSC_3D_UFSC, //6
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_RGB444_DS_UFSC, //7
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV444_DS_UFSC, //8
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_YUV422_DS_UFSC, //9
    #endif
    #if PQ_QM_HMDI
    QM_FHD_RGB444_DS_UFSC, //10
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV444_DS_UFSC, //11
    #endif
    #if PQ_QM_HMDI
    QM_FHD_YUV422_DS_UFSC, //12
    #endif
    #if PQ_QM_HMDI
    QM_FSC_3D_DS_UFSC, //13
    #endif
    #if PQ_QM_HMDI
    QM_4K2K_PQbypass_UFSC, //14
    #endif
    #if PQ_QM_HMDI
    QM_FHD_PQBypass_UFSC, //15
    #endif
    QM_INPUTTYPE_NUM_UFSC, // 16
} SC_QUALITY_MAP_INDEX_e_UFSC;

typedef enum
{
PQ_IP_SC_Init_UFSC,  //0
PQ_IP_UC_CTL_UFSC,  //1
PQ_IP_SPF_UFSC,  //2
PQ_IP_DMS_UFSC,  //3
PQ_IP_DMS_H_UFSC,  //4
PQ_IP_DMS_V_UFSC,  //5
PQ_IP_DMS_Cplx_UFSC,  //6
PQ_IP_V12L_UFSC,  //7
PQ_IP_AA_UFSC,  //8
PQ_IP_AA_H_UFSC,  //9
PQ_IP_AA_V_UFSC,  //10
PQ_IP_BNF_UFSC,  //11
PQ_IP_PreEnhance_UFSC,  //12
PQ_IP_PreEnhance_PrePK_UFSC,  //13
PQ_IP_PreEnhance_LDE3x3_UFSC,  //14
PQ_IP_PreEnhance_PK3x3_UFSC,  //15
PQ_IP_PreEnhance_BLTI_UFSC,  //16
PQ_IP_SPF_PrePK_Kernel_UFSC,  //17
PQ_IP_ANN_UFSC,  //18
PQ_IP_WeakDetail_Detect_UFSC,  //19
PQ_IP_SDHD_Detect_UFSC,  //20
PQ_IP_ERC_UFSC,  //21
PQ_IP_ERCL1_UFSC,  //22
PQ_IP_ERCL2_UFSC,  //23
PQ_IP_ERC_Edge_UFSC,  //24
PQ_IP_ERC_Modify_MTISL_UFSC,  //25
PQ_IP_ERC_Modify_DSMINSTD_UFSC,  //26
PQ_IP_ERC_Modify_Others_UFSC,  //27
PQ_IP_P2M_UFSC,  //28
PQ_IP_INIT_FAC_UFSC,  //29
PQ_IP_DBI_UFSC,  //30
PQ_IP_DS_UFSC,  //31
PQ_IP_DS_180_AD_UFSC,  //32
PQ_IP_DS_180_BC_UFSC,  //33
PQ_IP_DS_180_R_UFSC,  //34
PQ_IP_DS_11_AD_UFSC,  //35
PQ_IP_DS_11_BC_UFSC,  //36
PQ_IP_DS_11_R_UFSC,  //37
PQ_IP_DS_12_AD_UFSC,  //38
PQ_IP_DS_12_BC_UFSC,  //39
PQ_IP_DS_12_R_UFSC,  //40
PQ_IP_DS_13_AD_UFSC,  //41
PQ_IP_DS_13_BC_UFSC,  //42
PQ_IP_DS_13_R_UFSC,  //43
PQ_IP_VSP_Y_UFSC,  //44
PQ_IP_VSP_C_UFSC,  //45
PQ_IP_VSP_CoRing_UFSC,  //46
PQ_IP_VSP_DeRing_UFSC,  //47
PQ_IP_VSP_Dither_UFSC,  //48
PQ_IP_VSP_PreVBound_UFSC,  //49
PQ_IP_PreCTI_UFSC,  //50
PQ_IP_HSP_Y_UFSC,  //51
PQ_IP_HSP_C_UFSC,  //52
PQ_IP_HSP_CoRing_UFSC,  //53
PQ_IP_HSP_DeRing_UFSC,  //54
PQ_IP_HSP_Dither_UFSC,  //55
PQ_IP_HnonLinear_UFSC,  //56
PQ_IP_VSP_Y_COEFF_UFSC,  //57
PQ_IP_VSP_C_COEFF_UFSC,  //58
PQ_IP_HSP_Y_COEFF_UFSC,  //59
PQ_IP_HSP_C_COEFF_UFSC,  //60
PQ_IP_SemiDictionarySR_UFSC,  //61
PQ_IP_SemiDictionarySR_filter_UFSC,  //62
PQ_IP_NLM_UFSC,  //63
PQ_IP_NLM_DC_UFSC,  //64
PQ_IP_VIP_UFSC,  //65
PQ_IP_VIP_pseudo_UFSC,  //66
PQ_IP_VIP_LineBuffer_UFSC,  //67
PQ_IP_VIP_BGD_UFSC,  //68
PQ_IP_VIP_Post3x3PK_UFSC,  //69
PQ_IP_VIP_Peaking_UFSC,  //70
PQ_IP_VIP_Peaking_band_UFSC,  //71
PQ_IP_VIP_Peaking_SecAC_UFSC,  //72
PQ_IP_VIP_Peaking_Coef_UFSC,  //73
PQ_IP_VIP_Peaking_adptive_UFSC,  //74
PQ_IP_VIP_Peaking_dering_UFSC,  //75
PQ_IP_VIP_Peaking_V_dering_UFSC,  //76
PQ_IP_VIP_Peaking_V_dering_VDR1_UFSC,  //77
PQ_IP_VIP_Peaking_V_dering_VDR2_UFSC,  //78
PQ_IP_VIP_Peaking_Pcoring_UFSC,  //79
PQ_IP_VIP_Peaking_Pcoring_ad_C_UFSC,  //80
PQ_IP_VIP_Peaking_Pcoring_ad_Y_UFSC,  //81
PQ_IP_VIP_Peaking_gain_UFSC,  //82
PQ_IP_VIP_Peaking_gain_ad_C_UFSC,  //83
PQ_IP_VIP_Peaking_gain_ad_Y_UFSC,  //84
PQ_IP_VIP_Peaking_SDHD_Blending_UFSC,  //85
PQ_IP_VIP_Peaking_Patch_UFSC,  //86
PQ_IP_VIP_ICC_CRD_SRAM_UFSC,  //87
PQ_IP_VIP_YC_gain_offset_UFSC,  //88
PQ_IP_VIP_LCE_UFSC,  //89
PQ_IP_VIP_LCE_dither_UFSC,  //90
PQ_IP_VIP_LCE_dc_UFSC,  //91
PQ_IP_VIP_LCE_setting_UFSC,  //92
PQ_IP_VIP_LCE_ad_C_UFSC,  //93
PQ_IP_VIP_LCE_curve_UFSC,  //94
PQ_IP_VIP_DLC_UFSC,  //95
PQ_IP_VIP_DLC_ad_C_UFSC,  //96
PQ_IP_VIP_DLC_skin_protect_UFSC,  //97
PQ_IP_VIP_DLC_dither_UFSC,  //98
PQ_IP_VIP_DLC_His_range_UFSC,  //99
PQ_IP_VIP_DLC_His_rangeH_UFSC,  //100
PQ_IP_VIP_DLC_His_rangeV_UFSC,  //101
PQ_IP_VIP_DLC_PC_UFSC,  //102
PQ_IP_VIP_BWLE_UFSC,  //103
PQ_IP_VIP_BLE_UFSC,  //104
PQ_IP_VIP_WLE_UFSC,  //105
PQ_IP_VIP_BWLE_dither_UFSC,  //106
PQ_IP_VIP_UVC_UFSC,  //107
PQ_IP_VIP_YCbCr_Clip_UFSC,  //108
PQ_IP_SWDriver_UFSC,  //109
PQ_IP_SC_End_UFSC,  //110
 }   PQ_IPTYPE_UFSC;

typedef enum
{
PQ_IP_SC_Init_COM_UFSC,  //0
PQ_IP_UC_CTL_COM_UFSC,  //1
PQ_IP_SPF_COM_UFSC,  //2
PQ_IP_DMS_COM_UFSC,  //3
PQ_IP_DMS_H_COM_UFSC,  //4
PQ_IP_DMS_V_COM_UFSC,  //5
PQ_IP_DMS_Cplx_COM_UFSC,  //6
PQ_IP_V12L_COM_UFSC,  //7
PQ_IP_AA_COM_UFSC,  //8
PQ_IP_AA_H_COM_UFSC,  //9
PQ_IP_AA_V_COM_UFSC,  //10
PQ_IP_BNF_COM_UFSC,  //11
PQ_IP_PreEnhance_COM_UFSC,  //12
PQ_IP_PreEnhance_PrePK_COM_UFSC,  //13
PQ_IP_PreEnhance_LDE3x3_COM_UFSC,  //14
PQ_IP_PreEnhance_PK3x3_COM_UFSC,  //15
PQ_IP_PreEnhance_BLTI_COM_UFSC,  //16
PQ_IP_SPF_PrePK_Kernel_COM_UFSC,  //17
PQ_IP_ANN_COM_UFSC,  //18
PQ_IP_WeakDetail_Detect_COM_UFSC,  //19
PQ_IP_SDHD_Detect_COM_UFSC,  //20
PQ_IP_ERC_COM_UFSC,  //21
PQ_IP_ERCL1_COM_UFSC,  //22
PQ_IP_ERCL2_COM_UFSC,  //23
PQ_IP_ERC_Edge_COM_UFSC,  //24
PQ_IP_ERC_Modify_MTISL_COM_UFSC,  //25
PQ_IP_ERC_Modify_DSMINSTD_COM_UFSC,  //26
PQ_IP_ERC_Modify_Others_COM_UFSC,  //27
PQ_IP_P2M_COM_UFSC,  //28
PQ_IP_INIT_FAC_COM_UFSC,  //29
PQ_IP_DBI_COM_UFSC,  //30
PQ_IP_DS_COM_UFSC,  //31
PQ_IP_DS_180_AD_COM_UFSC,  //32
PQ_IP_DS_180_BC_COM_UFSC,  //33
PQ_IP_DS_180_R_COM_UFSC,  //34
PQ_IP_DS_11_AD_COM_UFSC,  //35
PQ_IP_DS_11_BC_COM_UFSC,  //36
PQ_IP_DS_11_R_COM_UFSC,  //37
PQ_IP_DS_12_AD_COM_UFSC,  //38
PQ_IP_DS_12_BC_COM_UFSC,  //39
PQ_IP_DS_12_R_COM_UFSC,  //40
PQ_IP_DS_13_AD_COM_UFSC,  //41
PQ_IP_DS_13_BC_COM_UFSC,  //42
PQ_IP_DS_13_R_COM_UFSC,  //43
PQ_IP_VSP_Y_COM_UFSC,  //44
PQ_IP_VSP_C_COM_UFSC,  //45
PQ_IP_VSP_CoRing_COM_UFSC,  //46
PQ_IP_VSP_DeRing_COM_UFSC,  //47
PQ_IP_VSP_Dither_COM_UFSC,  //48
PQ_IP_VSP_PreVBound_COM_UFSC,  //49
PQ_IP_PreCTI_COM_UFSC,  //50
PQ_IP_HSP_Y_COM_UFSC,  //51
PQ_IP_HSP_C_COM_UFSC,  //52
PQ_IP_HSP_CoRing_COM_UFSC,  //53
PQ_IP_HSP_DeRing_COM_UFSC,  //54
PQ_IP_HSP_Dither_COM_UFSC,  //55
PQ_IP_HnonLinear_COM_UFSC,  //56
PQ_IP_VSP_Y_COEFF_COM_UFSC,  //57
PQ_IP_VSP_C_COEFF_COM_UFSC,  //58
PQ_IP_HSP_Y_COEFF_COM_UFSC,  //59
PQ_IP_HSP_C_COEFF_COM_UFSC,  //60
PQ_IP_SemiDictionarySR_COM_UFSC,  //61
PQ_IP_SemiDictionarySR_filter_COM_UFSC,  //62
PQ_IP_NLM_COM_UFSC,  //63
PQ_IP_NLM_DC_COM_UFSC,  //64
PQ_IP_VIP_COM_UFSC,  //65
PQ_IP_VIP_pseudo_COM_UFSC,  //66
PQ_IP_VIP_LineBuffer_COM_UFSC,  //67
PQ_IP_VIP_BGD_COM_UFSC,  //68
PQ_IP_VIP_Post3x3PK_COM_UFSC,  //69
PQ_IP_VIP_Peaking_COM_UFSC,  //70
PQ_IP_VIP_Peaking_band_COM_UFSC,  //71
PQ_IP_VIP_Peaking_SecAC_COM_UFSC,  //72
PQ_IP_VIP_Peaking_Coef_COM_UFSC,  //73
PQ_IP_VIP_Peaking_adptive_COM_UFSC,  //74
PQ_IP_VIP_Peaking_dering_COM_UFSC,  //75
PQ_IP_VIP_Peaking_V_dering_COM_UFSC,  //76
PQ_IP_VIP_Peaking_V_dering_VDR1_COM_UFSC,  //77
PQ_IP_VIP_Peaking_V_dering_VDR2_COM_UFSC,  //78
PQ_IP_VIP_Peaking_Pcoring_COM_UFSC,  //79
PQ_IP_VIP_Peaking_Pcoring_ad_C_COM_UFSC,  //80
PQ_IP_VIP_Peaking_Pcoring_ad_Y_COM_UFSC,  //81
PQ_IP_VIP_Peaking_gain_COM_UFSC,  //82
PQ_IP_VIP_Peaking_gain_ad_C_COM_UFSC,  //83
PQ_IP_VIP_Peaking_gain_ad_Y_COM_UFSC,  //84
PQ_IP_VIP_Peaking_SDHD_Blending_COM_UFSC,  //85
PQ_IP_VIP_Peaking_Patch_COM_UFSC,  //86
PQ_IP_VIP_ICC_CRD_SRAM_COM_UFSC,  //87
PQ_IP_VIP_YC_gain_offset_COM_UFSC,  //88
PQ_IP_VIP_LCE_COM_UFSC,  //89
PQ_IP_VIP_LCE_dither_COM_UFSC,  //90
PQ_IP_VIP_LCE_dc_COM_UFSC,  //91
PQ_IP_VIP_LCE_setting_COM_UFSC,  //92
PQ_IP_VIP_LCE_ad_C_COM_UFSC,  //93
PQ_IP_VIP_LCE_curve_COM_UFSC,  //94
PQ_IP_VIP_DLC_COM_UFSC,  //95
PQ_IP_VIP_DLC_ad_C_COM_UFSC,  //96
PQ_IP_VIP_DLC_skin_protect_COM_UFSC,  //97
PQ_IP_VIP_DLC_dither_COM_UFSC,  //98
PQ_IP_VIP_DLC_His_range_COM_UFSC,  //99
PQ_IP_VIP_DLC_His_rangeH_COM_UFSC,  //100
PQ_IP_VIP_DLC_His_rangeV_COM_UFSC,  //101
PQ_IP_VIP_DLC_PC_COM_UFSC,  //102
PQ_IP_VIP_BWLE_COM_UFSC,  //103
PQ_IP_VIP_BLE_COM_UFSC,  //104
PQ_IP_VIP_WLE_COM_UFSC,  //105
PQ_IP_VIP_BWLE_dither_COM_UFSC,  //106
PQ_IP_VIP_UVC_COM_UFSC,  //107
PQ_IP_VIP_YCbCr_Clip_COM_UFSC,  //108
PQ_IP_SWDriver_COM_UFSC,  //109
PQ_IP_SC_End_COM_UFSC,  //110
PQ_IP_COM_NUMS_UFSC
} PQ_IP_COM_Group_UFSC;

typedef enum
{
PQ_IP_SC_Init_Init_UFSC,
PQ_IP_SC_Init_Frange_UFSC,
PQ_IP_SC_Init_Lrange_UFSC,
PQ_IP_SC_Init_0915_UFSC,
PQ_IP_SC_Init_NUMS_UFSC
} PQ_IP_SC_Init_Group_UFSC;

typedef enum
{
PQ_IP_UC_CTL_OFF_UFSC,
PQ_IP_UC_CTL_Y8C6M4_UFSC,
PQ_IP_UC_CTL_Y8C8M4_UFSC,
PQ_IP_UC_CTL_Y8M4_UFSC,
PQ_IP_UC_CTL_Y8_UFSC,
PQ_IP_UC_CTL_NUMS_UFSC
} PQ_IP_UC_CTL_Group_UFSC;

typedef enum
{
PQ_IP_SPF_OFF_UFSC,
PQ_IP_SPF_ON_UFSC,
PQ_IP_SPF_NUMS_UFSC
} PQ_IP_SPF_Group_UFSC;

typedef enum
{
PQ_IP_DMS_OFF_UFSC,
PQ_IP_DMS_ON_UFSC,
PQ_IP_DMS_L0716_UFSC,
PQ_IP_DMS_NUMS_UFSC
} PQ_IP_DMS_Group_UFSC;

typedef enum
{
PQ_IP_DMS_H_OFF_UFSC,
PQ_IP_DMS_H_S1_UFSC,
PQ_IP_DMS_H_S2_UFSC,
PQ_IP_DMS_H_S3_UFSC,
PQ_IP_DMS_H_DTV_S1_UFSC,
PQ_IP_DMS_H_DTV_S2_UFSC,
PQ_IP_DMS_H_DTV_S3_UFSC,
PQ_IP_DMS_H_S4_N_UFSC,
PQ_IP_DMS_H_S5_P_UFSC,
PQ_IP_DMS_H_S_TCL_UFSC,
PQ_IP_DMS_H_L0716_UFSC,
PQ_IP_DMS_H_NUMS_UFSC
} PQ_IP_DMS_H_Group_UFSC;

typedef enum
{
PQ_IP_DMS_V_OFF_UFSC,
PQ_IP_DMS_V_S1_UFSC,
PQ_IP_DMS_V_S2_UFSC,
PQ_IP_DMS_V_S3_UFSC,
PQ_IP_DMS_V_HD_UFSC,
PQ_IP_DMS_V_DTV_S1_UFSC,
PQ_IP_DMS_V_DTV_S2_UFSC,
PQ_IP_DMS_V_DTV_S3_UFSC,
PQ_IP_DMS_V_S4_N_UFSC,
PQ_IP_DMS_V_S5_P_UFSC,
PQ_IP_DMS_V_S_TCL_UFSC,
PQ_IP_DMS_V_L0716_UFSC,
PQ_IP_DMS_V_NUMS_UFSC
} PQ_IP_DMS_V_Group_UFSC;

typedef enum
{
PQ_IP_DMS_Cplx_OFF_UFSC,
PQ_IP_DMS_Cplx_cplx_th5A_UFSC,
PQ_IP_DMS_Cplx_L0716_UFSC,
PQ_IP_DMS_Cplx_L1117_UFSC,
PQ_IP_DMS_Cplx_NUMS_UFSC
} PQ_IP_DMS_Cplx_Group_UFSC;

typedef enum
{
PQ_IP_V12L_OFF_UFSC,
PQ_IP_V12L_ON_UFSC,
PQ_IP_V12L_L0716_UFSC,
PQ_IP_V12L_NUMS_UFSC
} PQ_IP_V12L_Group_UFSC;

typedef enum
{
PQ_IP_AA_OFF_UFSC,
PQ_IP_AA_S1_UFSC,
PQ_IP_AA_S2_UFSC,
PQ_IP_AA_S3_UFSC,
PQ_IP_AA_S4_UFSC,
PQ_IP_AA_S5_UFSC,
PQ_IP_AA_L0716_UFSC,
PQ_IP_AA_NUMS_UFSC
} PQ_IP_AA_Group_UFSC;

typedef enum
{
PQ_IP_AA_H_OFF_UFSC,
PQ_IP_AA_H_S1_UFSC,
PQ_IP_AA_H_S2_UFSC,
PQ_IP_AA_H_S3_UFSC,
PQ_IP_AA_H_S4_UFSC,
PQ_IP_AA_H_S5_UFSC,
PQ_IP_AA_H_L0716_UFSC,
PQ_IP_AA_H_NUMS_UFSC
} PQ_IP_AA_H_Group_UFSC;

typedef enum
{
PQ_IP_AA_V_OFF_UFSC,
PQ_IP_AA_V_S1_UFSC,
PQ_IP_AA_V_S2_UFSC,
PQ_IP_AA_V_S3_UFSC,
PQ_IP_AA_V_S4_UFSC,
PQ_IP_AA_V_S5_UFSC,
PQ_IP_AA_V_L0716_UFSC,
PQ_IP_AA_V_NUMS_UFSC
} PQ_IP_AA_V_Group_UFSC;

typedef enum
{
PQ_IP_BNF_OFF_UFSC,
PQ_IP_BNF_ON_UFSC,
PQ_IP_BNF_bypass_UFSC,
PQ_IP_BNF_NUMS_UFSC
} PQ_IP_BNF_Group_UFSC;

typedef enum
{
PQ_IP_PreEnhance_OFF_UFSC,
PQ_IP_PreEnhance_ON_UFSC,
PQ_IP_PreEnhance_NUMS_UFSC
} PQ_IP_PreEnhance_Group_UFSC;

typedef enum
{
PQ_IP_PreEnhance_PrePK_OFF_UFSC,
PQ_IP_PreEnhance_PrePK_ON_UFSC,
PQ_IP_PreEnhance_PrePK_NUMS_UFSC
} PQ_IP_PreEnhance_PrePK_Group_UFSC;

typedef enum
{
PQ_IP_PreEnhance_LDE3x3_OFF_UFSC,
PQ_IP_PreEnhance_LDE3x3_ON_UFSC,
PQ_IP_PreEnhance_LDE3x3_UHD_UFSC,
PQ_IP_PreEnhance_LDE3x3_NUMS_UFSC
} PQ_IP_PreEnhance_LDE3x3_Group_UFSC;

typedef enum
{
PQ_IP_PreEnhance_PK3x3_OFF_UFSC,
PQ_IP_PreEnhance_PK3x3_ON_UFSC,
PQ_IP_PreEnhance_PK3x3_SR_1_UFSC,
PQ_IP_PreEnhance_PK3x3_SR_2_UFSC,
PQ_IP_PreEnhance_PK3x3_NUMS_UFSC
} PQ_IP_PreEnhance_PK3x3_Group_UFSC;

typedef enum
{
PQ_IP_PreEnhance_BLTI_OFF_UFSC,
PQ_IP_PreEnhance_BLTI_ON_UFSC,
PQ_IP_PreEnhance_BLTI_NUMS_UFSC
} PQ_IP_PreEnhance_BLTI_Group_UFSC;

typedef enum
{
PQ_IP_SPF_PrePK_Kernel_OFF_UFSC,
PQ_IP_SPF_PrePK_Kernel_ON_UFSC,
PQ_IP_SPF_PrePK_Kernel_LG_Cmodel_UFSC,
PQ_IP_SPF_PrePK_Kernel_NUMS_UFSC
} PQ_IP_SPF_PrePK_Kernel_Group_UFSC;

typedef enum
{
PQ_IP_ANN_OFF_UFSC,
PQ_IP_ANN_ON_UFSC,
PQ_IP_ANN_S1_UFSC,
PQ_IP_ANN_NUMS_UFSC
} PQ_IP_ANN_Group_UFSC;

typedef enum
{
PQ_IP_WeakDetail_Detect_OFF_UFSC,
PQ_IP_WeakDetail_Detect_ON_UFSC,
PQ_IP_WeakDetail_Detect_M1127_UFSC,
PQ_IP_WeakDetail_Detect_Frange_UFSC,
PQ_IP_WeakDetail_Detect_Lrange_UFSC,
PQ_IP_WeakDetail_Detect_NUMS_UFSC
} PQ_IP_WeakDetail_Detect_Group_UFSC;

typedef enum
{
PQ_IP_SDHD_Detect_OFF_UFSC,
PQ_IP_SDHD_Detect_ON_UFSC,
PQ_IP_SDHD_Detect_M1127_UFSC,
PQ_IP_SDHD_Detect_NUMS_UFSC
} PQ_IP_SDHD_Detect_Group_UFSC;

typedef enum
{
PQ_IP_ERC_OFF_UFSC,
PQ_IP_ERC_S1_UFSC,
PQ_IP_ERC_S2_UFSC,
PQ_IP_ERC_S3_UFSC,
PQ_IP_ERC_SD_W_UFSC,
PQ_IP_ERC_0915_UFSC,
PQ_IP_ERC_SmallAC_UFSC,
PQ_IP_ERC_NUMS_UFSC
} PQ_IP_ERC_Group_UFSC;

typedef enum
{
PQ_IP_ERCL1_OFF_UFSC,
PQ_IP_ERCL1_S1_UFSC,
PQ_IP_ERCL1_SmallAC_UFSC,
PQ_IP_ERCL1_NUMS_UFSC
} PQ_IP_ERCL1_Group_UFSC;

typedef enum
{
PQ_IP_ERCL2_OFF_UFSC,
PQ_IP_ERCL2_S1_UFSC,
PQ_IP_ERCL2_SmallAC_UFSC,
PQ_IP_ERCL2_NUMS_UFSC
} PQ_IP_ERCL2_Group_UFSC;

typedef enum
{
PQ_IP_ERC_Edge_OFF_UFSC,
PQ_IP_ERC_Edge_S1_UFSC,
PQ_IP_ERC_Edge_M1127_UFSC,
PQ_IP_ERC_Edge_NUMS_UFSC
} PQ_IP_ERC_Edge_Group_UFSC;

typedef enum
{
PQ_IP_ERC_Modify_MTISL_OFF_UFSC,
PQ_IP_ERC_Modify_MTISL_ON_UFSC,
PQ_IP_ERC_Modify_MTISL_NUMS_UFSC
} PQ_IP_ERC_Modify_MTISL_Group_UFSC;

typedef enum
{
PQ_IP_ERC_Modify_DSMINSTD_OFF_UFSC,
PQ_IP_ERC_Modify_DSMINSTD_ON_UFSC,
PQ_IP_ERC_Modify_DSMINSTD_NUMS_UFSC
} PQ_IP_ERC_Modify_DSMINSTD_Group_UFSC;

typedef enum
{
PQ_IP_ERC_Modify_Others_OFF_UFSC,
PQ_IP_ERC_Modify_Others_ON_UFSC,
PQ_IP_ERC_Modify_Others_S1_UFSC,
PQ_IP_ERC_Modify_Others_NUMS_UFSC
} PQ_IP_ERC_Modify_Others_Group_UFSC;

typedef enum
{
PQ_IP_P2M_OFF_UFSC,
PQ_IP_P2M_LG_Cmodel_UFSC,
PQ_IP_P2M_W0_UFSC,
PQ_IP_P2M_W1_UFSC,
PQ_IP_P2M_SR1122_UFSC,
PQ_IP_P2M_SR_UFSC,
PQ_IP_P2M_NUMS_UFSC
} PQ_IP_P2M_Group_UFSC;

typedef enum
{
PQ_IP_INIT_FAC_Type0_UFSC,
PQ_IP_INIT_FAC_Type1_UFSC,
PQ_IP_INIT_FAC_Type2_UFSC,
PQ_IP_INIT_FAC_NUMS_UFSC
} PQ_IP_INIT_FAC_Group_UFSC;

typedef enum
{
PQ_IP_DBI_OFF_UFSC,
PQ_IP_DBI_W0_UFSC,
PQ_IP_DBI_W1_UFSC,
PQ_IP_DBI_W2_UFSC,
PQ_IP_DBI_NUMS_UFSC
} PQ_IP_DBI_Group_UFSC;

typedef enum
{
PQ_IP_DS_OFF_UFSC,
PQ_IP_DS_W0_UFSC,
PQ_IP_DS_W1_UFSC,
PQ_IP_DS_W2_UFSC,
PQ_IP_DS_NUMS_UFSC
} PQ_IP_DS_Group_UFSC;

typedef enum
{
PQ_IP_DS_180_AD_C3_UFSC,
PQ_IP_DS_180_AD_C4_UFSC,
PQ_IP_DS_180_AD_C5_UFSC,
PQ_IP_DS_180_AD_C6_UFSC,
PQ_IP_DS_180_AD_C7_UFSC,
PQ_IP_DS_180_AD_C8_UFSC,
PQ_IP_DS_180_AD_C9_UFSC,
PQ_IP_DS_180_AD_C10_UFSC,
PQ_IP_DS_180_AD_C11_UFSC,
PQ_IP_DS_180_AD_C12_UFSC,
PQ_IP_DS_180_AD_C13_UFSC,
PQ_IP_DS_180_AD_C14_UFSC,
PQ_IP_DS_180_AD_NUMS_UFSC
} PQ_IP_DS_180_AD_Group_UFSC;

typedef enum
{
PQ_IP_DS_180_BC_C3_UFSC,
PQ_IP_DS_180_BC_C4_UFSC,
PQ_IP_DS_180_BC_C5_UFSC,
PQ_IP_DS_180_BC_C6_UFSC,
PQ_IP_DS_180_BC_C7_UFSC,
PQ_IP_DS_180_BC_C8_UFSC,
PQ_IP_DS_180_BC_C9_UFSC,
PQ_IP_DS_180_BC_C10_UFSC,
PQ_IP_DS_180_BC_C11_UFSC,
PQ_IP_DS_180_BC_C12_UFSC,
PQ_IP_DS_180_BC_C13_UFSC,
PQ_IP_DS_180_BC_C14_UFSC,
PQ_IP_DS_180_BC_NUMS_UFSC
} PQ_IP_DS_180_BC_Group_UFSC;

typedef enum
{
PQ_IP_DS_180_R_C3_UFSC,
PQ_IP_DS_180_R_C4_UFSC,
PQ_IP_DS_180_R_C5_UFSC,
PQ_IP_DS_180_R_C6_UFSC,
PQ_IP_DS_180_R_C7_UFSC,
PQ_IP_DS_180_R_C8_UFSC,
PQ_IP_DS_180_R_C9_UFSC,
PQ_IP_DS_180_R_C10_UFSC,
PQ_IP_DS_180_R_C11_UFSC,
PQ_IP_DS_180_R_C12_UFSC,
PQ_IP_DS_180_R_C13_UFSC,
PQ_IP_DS_180_R_C14_UFSC,
PQ_IP_DS_180_R_NUMS_UFSC
} PQ_IP_DS_180_R_Group_UFSC;

typedef enum
{
PQ_IP_DS_11_AD_C3_UFSC,
PQ_IP_DS_11_AD_C4_UFSC,
PQ_IP_DS_11_AD_C5_UFSC,
PQ_IP_DS_11_AD_C6_UFSC,
PQ_IP_DS_11_AD_C7_UFSC,
PQ_IP_DS_11_AD_C8_UFSC,
PQ_IP_DS_11_AD_C9_UFSC,
PQ_IP_DS_11_AD_C10_UFSC,
PQ_IP_DS_11_AD_C11_UFSC,
PQ_IP_DS_11_AD_C12_UFSC,
PQ_IP_DS_11_AD_C13_UFSC,
PQ_IP_DS_11_AD_C14_UFSC,
PQ_IP_DS_11_AD_NUMS_UFSC
} PQ_IP_DS_11_AD_Group_UFSC;

typedef enum
{
PQ_IP_DS_11_BC_C3_UFSC,
PQ_IP_DS_11_BC_C4_UFSC,
PQ_IP_DS_11_BC_C5_UFSC,
PQ_IP_DS_11_BC_C6_UFSC,
PQ_IP_DS_11_BC_C7_UFSC,
PQ_IP_DS_11_BC_C8_UFSC,
PQ_IP_DS_11_BC_C9_UFSC,
PQ_IP_DS_11_BC_C10_UFSC,
PQ_IP_DS_11_BC_C11_UFSC,
PQ_IP_DS_11_BC_C12_UFSC,
PQ_IP_DS_11_BC_C13_UFSC,
PQ_IP_DS_11_BC_C14_UFSC,
PQ_IP_DS_11_BC_NUMS_UFSC
} PQ_IP_DS_11_BC_Group_UFSC;

typedef enum
{
PQ_IP_DS_11_R_C3_UFSC,
PQ_IP_DS_11_R_C4_UFSC,
PQ_IP_DS_11_R_C5_UFSC,
PQ_IP_DS_11_R_C6_UFSC,
PQ_IP_DS_11_R_C7_UFSC,
PQ_IP_DS_11_R_C8_UFSC,
PQ_IP_DS_11_R_C9_UFSC,
PQ_IP_DS_11_R_C10_UFSC,
PQ_IP_DS_11_R_C11_UFSC,
PQ_IP_DS_11_R_C12_UFSC,
PQ_IP_DS_11_R_C13_UFSC,
PQ_IP_DS_11_R_C14_UFSC,
PQ_IP_DS_11_R_NUMS_UFSC
} PQ_IP_DS_11_R_Group_UFSC;

typedef enum
{
PQ_IP_DS_12_AD_C3_UFSC,
PQ_IP_DS_12_AD_C4_UFSC,
PQ_IP_DS_12_AD_C5_UFSC,
PQ_IP_DS_12_AD_C6_UFSC,
PQ_IP_DS_12_AD_C7_UFSC,
PQ_IP_DS_12_AD_C8_UFSC,
PQ_IP_DS_12_AD_C9_UFSC,
PQ_IP_DS_12_AD_C10_UFSC,
PQ_IP_DS_12_AD_C11_UFSC,
PQ_IP_DS_12_AD_C12_UFSC,
PQ_IP_DS_12_AD_C13_UFSC,
PQ_IP_DS_12_AD_C14_UFSC,
PQ_IP_DS_12_AD_NUMS_UFSC
} PQ_IP_DS_12_AD_Group_UFSC;

typedef enum
{
PQ_IP_DS_12_BC_C3_UFSC,
PQ_IP_DS_12_BC_C4_UFSC,
PQ_IP_DS_12_BC_C5_UFSC,
PQ_IP_DS_12_BC_C6_UFSC,
PQ_IP_DS_12_BC_C7_UFSC,
PQ_IP_DS_12_BC_C8_UFSC,
PQ_IP_DS_12_BC_C9_UFSC,
PQ_IP_DS_12_BC_C10_UFSC,
PQ_IP_DS_12_BC_C11_UFSC,
PQ_IP_DS_12_BC_C12_UFSC,
PQ_IP_DS_12_BC_C13_UFSC,
PQ_IP_DS_12_BC_C14_UFSC,
PQ_IP_DS_12_BC_NUMS_UFSC
} PQ_IP_DS_12_BC_Group_UFSC;

typedef enum
{
PQ_IP_DS_12_R_C3_UFSC,
PQ_IP_DS_12_R_C4_UFSC,
PQ_IP_DS_12_R_C5_UFSC,
PQ_IP_DS_12_R_C6_UFSC,
PQ_IP_DS_12_R_C7_UFSC,
PQ_IP_DS_12_R_C8_UFSC,
PQ_IP_DS_12_R_C9_UFSC,
PQ_IP_DS_12_R_C10_UFSC,
PQ_IP_DS_12_R_C11_UFSC,
PQ_IP_DS_12_R_C12_UFSC,
PQ_IP_DS_12_R_C13_UFSC,
PQ_IP_DS_12_R_C14_UFSC,
PQ_IP_DS_12_R_NUMS_UFSC
} PQ_IP_DS_12_R_Group_UFSC;

typedef enum
{
PQ_IP_DS_13_AD_C3_UFSC,
PQ_IP_DS_13_AD_C4_UFSC,
PQ_IP_DS_13_AD_C5_UFSC,
PQ_IP_DS_13_AD_C6_UFSC,
PQ_IP_DS_13_AD_C7_UFSC,
PQ_IP_DS_13_AD_C8_UFSC,
PQ_IP_DS_13_AD_C9_UFSC,
PQ_IP_DS_13_AD_C10_UFSC,
PQ_IP_DS_13_AD_C11_UFSC,
PQ_IP_DS_13_AD_C12_UFSC,
PQ_IP_DS_13_AD_C13_UFSC,
PQ_IP_DS_13_AD_C14_UFSC,
PQ_IP_DS_13_AD_NUMS_UFSC
} PQ_IP_DS_13_AD_Group_UFSC;

typedef enum
{
PQ_IP_DS_13_BC_C3_UFSC,
PQ_IP_DS_13_BC_C4_UFSC,
PQ_IP_DS_13_BC_C5_UFSC,
PQ_IP_DS_13_BC_C6_UFSC,
PQ_IP_DS_13_BC_C7_UFSC,
PQ_IP_DS_13_BC_C8_UFSC,
PQ_IP_DS_13_BC_C9_UFSC,
PQ_IP_DS_13_BC_C10_UFSC,
PQ_IP_DS_13_BC_C11_UFSC,
PQ_IP_DS_13_BC_C12_UFSC,
PQ_IP_DS_13_BC_C13_UFSC,
PQ_IP_DS_13_BC_C14_UFSC,
PQ_IP_DS_13_BC_NUMS_UFSC
} PQ_IP_DS_13_BC_Group_UFSC;

typedef enum
{
PQ_IP_DS_13_R_C3_UFSC,
PQ_IP_DS_13_R_C4_UFSC,
PQ_IP_DS_13_R_C5_UFSC,
PQ_IP_DS_13_R_C6_UFSC,
PQ_IP_DS_13_R_C7_UFSC,
PQ_IP_DS_13_R_C8_UFSC,
PQ_IP_DS_13_R_C9_UFSC,
PQ_IP_DS_13_R_C10_UFSC,
PQ_IP_DS_13_R_C11_UFSC,
PQ_IP_DS_13_R_C12_UFSC,
PQ_IP_DS_13_R_C13_UFSC,
PQ_IP_DS_13_R_C14_UFSC,
PQ_IP_DS_13_R_NUMS_UFSC
} PQ_IP_DS_13_R_Group_UFSC;

typedef enum
{
PQ_IP_VSP_Y_Bypass_UFSC,
PQ_IP_VSP_Y_Bilinear_UFSC,
PQ_IP_VSP_Y_Y_4Tap_UFSC,
PQ_IP_VSP_Y_Y_6Tap_UFSC,
PQ_IP_VSP_Y_NUMS_UFSC
} PQ_IP_VSP_Y_Group_UFSC;

typedef enum
{
PQ_IP_VSP_C_Bypass_UFSC,
PQ_IP_VSP_C_Bilinear_UFSC,
PQ_IP_VSP_C_Y_4Tap_UFSC,
PQ_IP_VSP_C_C_4Tap_UFSC,
PQ_IP_VSP_C_NUMS_UFSC
} PQ_IP_VSP_C_Group_UFSC;

typedef enum
{
PQ_IP_VSP_CoRing_OFF_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_1_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_2_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_3_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_4_UFSC,
PQ_IP_VSP_CoRing_Y_Coring_5_UFSC,
PQ_IP_VSP_CoRing_NUMS_UFSC
} PQ_IP_VSP_CoRing_Group_UFSC;

typedef enum
{
PQ_IP_VSP_DeRing_OFF_UFSC,
PQ_IP_VSP_DeRing_DR1_UFSC,
PQ_IP_VSP_DeRing_DR2_UFSC,
PQ_IP_VSP_DeRing_NUMS_UFSC
} PQ_IP_VSP_DeRing_Group_UFSC;

typedef enum
{
PQ_IP_VSP_Dither_OFF_UFSC,
PQ_IP_VSP_Dither_ON_UFSC,
PQ_IP_VSP_Dither_NUMS_UFSC
} PQ_IP_VSP_Dither_Group_UFSC;

typedef enum
{
PQ_IP_VSP_PreVBound_OFF_UFSC,
PQ_IP_VSP_PreVBound_ON_UFSC,
PQ_IP_VSP_PreVBound_NUMS_UFSC
} PQ_IP_VSP_PreVBound_Group_UFSC;

typedef enum
{
PQ_IP_PreCTI_OFF_UFSC,
PQ_IP_PreCTI_CTI_N_0_UFSC,
PQ_IP_PreCTI_CTI_P_0_UFSC,
PQ_IP_PreCTI_CTI_P_S5_UFSC,
PQ_IP_PreCTI_CTI_N_S5_UFSC,
PQ_IP_PreCTI_CTI_P_S3_MS5_UFSC,
PQ_IP_PreCTI_CTI_N_S2_MS5_UFSC,
PQ_IP_PreCTI_CTI_S0_MS5_YREF04_UFSC,
PQ_IP_PreCTI_CTI_S0_MS5_YREF08_UFSC,
PQ_IP_PreCTI_CTI_S0_MS5_YREF0C_UFSC,
PQ_IP_PreCTI_CTI_MS3_CLF_low_UFSC,
PQ_IP_PreCTI_CTI_MS3_CLF_mid_UFSC,
PQ_IP_PreCTI_CTI_MS3_CLF_high_UFSC,
PQ_IP_PreCTI_NUMS_UFSC
} PQ_IP_PreCTI_Group_UFSC;

typedef enum
{
PQ_IP_HSP_Y_Bypass_UFSC,
PQ_IP_HSP_Y_Bilinear_UFSC,
PQ_IP_HSP_Y_Y_4Tap_UFSC,
PQ_IP_HSP_Y_Y_6Tap_UFSC,
PQ_IP_HSP_Y_NUMS_UFSC
} PQ_IP_HSP_Y_Group_UFSC;

typedef enum
{
PQ_IP_HSP_C_Bypass_UFSC,
PQ_IP_HSP_C_Bilinear_UFSC,
PQ_IP_HSP_C_C_4Tap_UFSC,
PQ_IP_HSP_C_Y_4Tap_UFSC,
PQ_IP_HSP_C_NUMS_UFSC
} PQ_IP_HSP_C_Group_UFSC;

typedef enum
{
PQ_IP_HSP_CoRing_OFF_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_1_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_2_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_3_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_4_UFSC,
PQ_IP_HSP_CoRing_Y_Coring_5_UFSC,
PQ_IP_HSP_CoRing_NUMS_UFSC
} PQ_IP_HSP_CoRing_Group_UFSC;

typedef enum
{
PQ_IP_HSP_DeRing_OFF_UFSC,
PQ_IP_HSP_DeRing_DR1_UFSC,
PQ_IP_HSP_DeRing_NUMS_UFSC
} PQ_IP_HSP_DeRing_Group_UFSC;

typedef enum
{
PQ_IP_HSP_Dither_OFF_UFSC,
PQ_IP_HSP_Dither_ON_UFSC,
PQ_IP_HSP_Dither_NUMS_UFSC
} PQ_IP_HSP_Dither_Group_UFSC;

typedef enum
{
PQ_IP_HnonLinear_OFF_UFSC,
PQ_IP_HnonLinear_H_1366_0_UFSC,
PQ_IP_HnonLinear_H_1366_1_UFSC,
PQ_IP_HnonLinear_H_1366_2_UFSC,
PQ_IP_HnonLinear_H_1440_UFSC,
PQ_IP_HnonLinear_H_1680_UFSC,
PQ_IP_HnonLinear_H_1920_0_UFSC,
PQ_IP_HnonLinear_H_1920_1_UFSC,
PQ_IP_HnonLinear_H_1920_2_UFSC,
PQ_IP_HnonLinear_NUMS_UFSC
} PQ_IP_HnonLinear_Group_UFSC;

typedef enum
{
PQ_IP_VSP_Y_COEFF_OFF_UFSC,
PQ_IP_VSP_Y_COEFF_InvSinc4Tc4p4Fc95Apass0001Astop40_UFSC,
PQ_IP_VSP_Y_COEFF_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC,
PQ_IP_VSP_Y_COEFF_InvSinc4Tc4p4Fc85Apass0001Astop40_UFSC,
PQ_IP_VSP_Y_COEFF_NUMS_UFSC
} PQ_IP_VSP_Y_COEFF_Group_UFSC;

typedef enum
{
PQ_IP_VSP_C_COEFF_OFF_UFSC,
PQ_IP_VSP_C_COEFF_InvSinc4Tc4p4Fc85Apass0001Astop40_UFSC,
PQ_IP_VSP_C_COEFF_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC,
PQ_IP_VSP_C_COEFF_NUMS_UFSC
} PQ_IP_VSP_C_COEFF_Group_UFSC;

typedef enum
{
PQ_IP_HSP_Y_COEFF_OFF_UFSC,
PQ_IP_HSP_Y_COEFF_InvSinc4Tc4p4Fc95Apass0001Astop40_UFSC,
PQ_IP_HSP_Y_COEFF_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC,
PQ_IP_HSP_Y_COEFF_InvSinc4Tc4p4Fc85Apass0001Astop40_UFSC,
PQ_IP_HSP_Y_COEFF_NUMS_UFSC
} PQ_IP_HSP_Y_COEFF_Group_UFSC;

typedef enum
{
PQ_IP_HSP_C_COEFF_OFF_UFSC,
PQ_IP_HSP_C_COEFF_InvSinc4Tc4p4Fc85Apass0001Astop40_UFSC,
PQ_IP_HSP_C_COEFF_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC,
PQ_IP_HSP_C_COEFF_NUMS_UFSC
} PQ_IP_HSP_C_COEFF_Group_UFSC;

typedef enum
{
PQ_IP_SemiDictionarySR_OFF_UFSC,
PQ_IP_SemiDictionarySR_ON_UFSC,
PQ_IP_SemiDictionarySR_HONO_UFSC,
PQ_IP_SemiDictionarySR_NUMS_UFSC
} PQ_IP_SemiDictionarySR_Group_UFSC;

typedef enum
{
PQ_IP_SemiDictionarySR_filter_ON_UFSC,
PQ_IP_SemiDictionarySR_filter_filter_13_UFSC,
PQ_IP_SemiDictionarySR_filter_filter_14_UFSC,
PQ_IP_SemiDictionarySR_filter_filter_15_UFSC,
PQ_IP_SemiDictionarySR_filter_filter_16_UFSC,
PQ_IP_SemiDictionarySR_filter_NUMS_UFSC
} PQ_IP_SemiDictionarySR_filter_Group_UFSC;

typedef enum
{
PQ_IP_NLM_OFF_UFSC,
PQ_IP_NLM_S1_UFSC,
PQ_IP_NLM_UHD_UFSC,
PQ_IP_NLM_0915_UFSC,
PQ_IP_NLM_NUMS_UFSC
} PQ_IP_NLM_Group_UFSC;

typedef enum
{
PQ_IP_NLM_DC_OFF_UFSC,
PQ_IP_NLM_DC_DC1_UFSC,
PQ_IP_NLM_DC_DC2_UFSC,
PQ_IP_NLM_DC_NUMS_UFSC
} PQ_IP_NLM_DC_Group_UFSC;

typedef enum
{
PQ_IP_VIP_OFF_UFSC,
PQ_IP_VIP_ON_UFSC,
PQ_IP_VIP_NUMS_UFSC
} PQ_IP_VIP_Group_UFSC;

typedef enum
{
PQ_IP_VIP_pseudo_OFF_UFSC,
PQ_IP_VIP_pseudo_ON_UFSC,
PQ_IP_VIP_pseudo_NUMS_UFSC
} PQ_IP_VIP_pseudo_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LineBuffer_OFF_UFSC,
PQ_IP_VIP_LineBuffer_ON_UFSC,
PQ_IP_VIP_LineBuffer_NUMS_UFSC
} PQ_IP_VIP_LineBuffer_Group_UFSC;

typedef enum
{
PQ_IP_VIP_BGD_OFF_UFSC,
PQ_IP_VIP_BGD_ON_UFSC,
PQ_IP_VIP_BGD_1204_UFSC,
PQ_IP_VIP_BGD_1209_UFSC,
PQ_IP_VIP_BGD_NUMS_UFSC
} PQ_IP_VIP_BGD_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Post3x3PK_OFF_UFSC,
PQ_IP_VIP_Post3x3PK_S1_UFSC,
PQ_IP_VIP_Post3x3PK_1120_UFSC,
PQ_IP_VIP_Post3x3PK_SR_L_UFSC,
PQ_IP_VIP_Post3x3PK_SR_H_UFSC,
PQ_IP_VIP_Post3x3PK_NUMS_UFSC
} PQ_IP_VIP_Post3x3PK_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_OFF_UFSC,
PQ_IP_VIP_Peaking_ON_UFSC,
PQ_IP_VIP_Peaking_NUMS_UFSC
} PQ_IP_VIP_Peaking_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_band_Gain_0C_UFSC,
PQ_IP_VIP_Peaking_band_0915_UFSC,
PQ_IP_VIP_Peaking_band_M1127_UFSC,
PQ_IP_VIP_Peaking_band_M1208_UFSC,
PQ_IP_VIP_Peaking_band_M0205_UFSC,
PQ_IP_VIP_Peaking_band_M0216_UFSC,
PQ_IP_VIP_Peaking_band_NUMS_UFSC
} PQ_IP_VIP_Peaking_band_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_SecAC_OFF_UFSC,
PQ_IP_VIP_Peaking_SecAC_ON_UFSC,
PQ_IP_VIP_Peaking_SecAC_M1127_UFSC,
PQ_IP_VIP_Peaking_SecAC_M1208_UFSC,
PQ_IP_VIP_Peaking_SecAC_NUMS_UFSC
} PQ_IP_VIP_Peaking_SecAC_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_Coef_OFF_UFSC,
PQ_IP_VIP_Peaking_Coef_ON_UFSC,
PQ_IP_VIP_Peaking_Coef_FS_UFSC,
PQ_IP_VIP_Peaking_Coef_1204_UFSC,
PQ_IP_VIP_Peaking_Coef_NUMS_UFSC
} PQ_IP_VIP_Peaking_Coef_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_adptive_OFF_UFSC,
PQ_IP_VIP_Peaking_adptive_ON_UFSC,
PQ_IP_VIP_Peaking_adptive_M1208_UFSC,
PQ_IP_VIP_Peaking_adptive_Lrange_UFSC,
PQ_IP_VIP_Peaking_adptive_NUMS_UFSC
} PQ_IP_VIP_Peaking_adptive_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_dering_OFF_UFSC,
PQ_IP_VIP_Peaking_dering_W0_R0_UFSC,
PQ_IP_VIP_Peaking_dering_W1_R0_UFSC,
PQ_IP_VIP_Peaking_dering_W0_R0_SA_UFSC,
PQ_IP_VIP_Peaking_dering_0915_UFSC,
PQ_IP_VIP_Peaking_dering_NUMS_UFSC
} PQ_IP_VIP_Peaking_dering_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_V_dering_OFF_UFSC,
PQ_IP_VIP_Peaking_V_dering_0x07_UFSC,
PQ_IP_VIP_Peaking_V_dering_0915_UFSC,
PQ_IP_VIP_Peaking_V_dering_NUMS_UFSC
} PQ_IP_VIP_Peaking_V_dering_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_V_dering_VDR1_OFF_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR1_VDR1_1_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR1_VDR1_2_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR1_L_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR1_NUMS_UFSC
} PQ_IP_VIP_Peaking_V_dering_VDR1_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_V_dering_VDR2_OFF_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR2_VDR2_1_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR2_VDR2_2_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR2_0915_UFSC,
PQ_IP_VIP_Peaking_V_dering_VDR2_NUMS_UFSC
} PQ_IP_VIP_Peaking_V_dering_VDR2_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_Pcoring_S00_0_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S21_0_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S21_1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S30_0_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S30_1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S31_0_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S31_1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S20_1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S32_0_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S32_1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_S42_1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_SEC1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_SEC2_UFSC,
PQ_IP_VIP_Peaking_Pcoring_SEC3_UFSC,
PQ_IP_VIP_Peaking_Pcoring_0915_UFSC,
PQ_IP_VIP_Peaking_Pcoring_NUMS_UFSC
} PQ_IP_VIP_Peaking_Pcoring_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_Pcoring_ad_C_OFF_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T1_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T2_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T3_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T4_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T5_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_T6_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_C_NUMS_UFSC
} PQ_IP_VIP_Peaking_Pcoring_ad_C_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_Pcoring_ad_Y_OFF_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_Y_T2_UFSC,
PQ_IP_VIP_Peaking_Pcoring_ad_Y_NUMS_UFSC
} PQ_IP_VIP_Peaking_Pcoring_ad_Y_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_gain_0x18_UFSC,
PQ_IP_VIP_Peaking_gain_0x1C_UFSC,
PQ_IP_VIP_Peaking_gain_0x20_UFSC,
PQ_IP_VIP_Peaking_gain_0x24_UFSC,
PQ_IP_VIP_Peaking_gain_0x28_UFSC,
PQ_IP_VIP_Peaking_gain_0x2C_UFSC,
PQ_IP_VIP_Peaking_gain_0x30_UFSC,
PQ_IP_VIP_Peaking_gain_0x34_UFSC,
PQ_IP_VIP_Peaking_gain_0x38_UFSC,
PQ_IP_VIP_Peaking_gain_M1127_UFSC,
PQ_IP_VIP_Peaking_gain_0216_UFSC,
PQ_IP_VIP_Peaking_gain_NUMS_UFSC
} PQ_IP_VIP_Peaking_gain_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_gain_ad_C_OFF_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_T1_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_T2_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_T3_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_T4_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_T5_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_T6_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_BGBrown0A_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_SECX10_UFSC,
PQ_IP_VIP_Peaking_gain_ad_C_NUMS_UFSC
} PQ_IP_VIP_Peaking_gain_ad_C_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_gain_ad_Y_OFF_UFSC,
PQ_IP_VIP_Peaking_gain_ad_Y_T2_UFSC,
PQ_IP_VIP_Peaking_gain_ad_Y_T3_UFSC,
PQ_IP_VIP_Peaking_gain_ad_Y_NUMS_UFSC
} PQ_IP_VIP_Peaking_gain_ad_Y_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_SDHD_Blending_OFF_UFSC,
PQ_IP_VIP_Peaking_SDHD_Blending_ON_UFSC,
PQ_IP_VIP_Peaking_SDHD_Blending_M1127_UFSC,
PQ_IP_VIP_Peaking_SDHD_Blending_NUMS_UFSC
} PQ_IP_VIP_Peaking_SDHD_Blending_Group_UFSC;

typedef enum
{
PQ_IP_VIP_Peaking_Patch_OFF_UFSC,
PQ_IP_VIP_Peaking_Patch_ON_UFSC,
PQ_IP_VIP_Peaking_Patch_E_Strong_UFSC,
PQ_IP_VIP_Peaking_Patch_NUMS_UFSC
} PQ_IP_VIP_Peaking_Patch_Group_UFSC;

typedef enum
{
PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC,
PQ_IP_VIP_ICC_CRD_SRAM_NUMS_UFSC
} PQ_IP_VIP_ICC_CRD_SRAM_Group_UFSC;

typedef enum
{
PQ_IP_VIP_YC_gain_offset_OFF_UFSC,
PQ_IP_VIP_YC_gain_offset_compress_1023_to_940_UFSC,
PQ_IP_VIP_YC_gain_offset_extend_940_to_1023_limitout_UFSC,
PQ_IP_VIP_YC_gain_offset_extend_940_to_1023_fullout_UFSC,
PQ_IP_VIP_YC_gain_offset_NUMS_UFSC
} PQ_IP_VIP_YC_gain_offset_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LCE_OFF_UFSC,
PQ_IP_VIP_LCE_ON_cascade_UFSC,
PQ_IP_VIP_LCE_ON_parallel_UFSC,
PQ_IP_VIP_LCE_NUMS_UFSC
} PQ_IP_VIP_LCE_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LCE_dither_OFF_UFSC,
PQ_IP_VIP_LCE_dither_ON_UFSC,
PQ_IP_VIP_LCE_dither_NUMS_UFSC
} PQ_IP_VIP_LCE_dither_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LCE_dc_OFF_UFSC,
PQ_IP_VIP_LCE_dc_ON_UFSC,
PQ_IP_VIP_LCE_dc_NUMS_UFSC
} PQ_IP_VIP_LCE_dc_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LCE_setting_S0_UFSC,
PQ_IP_VIP_LCE_setting_S1_UFSC,
PQ_IP_VIP_LCE_setting_S2_UFSC,
PQ_IP_VIP_LCE_setting_S3_UFSC,
PQ_IP_VIP_LCE_setting_3CV_S3_UFSC,
PQ_IP_VIP_LCE_setting_3CV_T12_UFSC,
PQ_IP_VIP_LCE_setting_NUMS_UFSC
} PQ_IP_VIP_LCE_setting_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LCE_ad_C_OFF_UFSC,
PQ_IP_VIP_LCE_ad_C_R5_UFSC,
PQ_IP_VIP_LCE_ad_C_NUMS_UFSC
} PQ_IP_VIP_LCE_ad_C_Group_UFSC;

typedef enum
{
PQ_IP_VIP_LCE_curve_CV1_UFSC,
PQ_IP_VIP_LCE_curve_linear_UFSC,
PQ_IP_VIP_LCE_curve_3CV_T1_UFSC,
PQ_IP_VIP_LCE_curve_3CV_T12_UFSC,
PQ_IP_VIP_LCE_curve_NUMS_UFSC
} PQ_IP_VIP_LCE_curve_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_OFF_UFSC,
PQ_IP_VIP_DLC_ON_UFSC,
PQ_IP_VIP_DLC_NUMS_UFSC
} PQ_IP_VIP_DLC_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_ad_C_OFF_UFSC,
PQ_IP_VIP_DLC_ad_C_T0_UFSC,
PQ_IP_VIP_DLC_ad_C_T1_UFSC,
PQ_IP_VIP_DLC_ad_C_T2_UFSC,
PQ_IP_VIP_DLC_ad_C_T3_UFSC,
PQ_IP_VIP_DLC_ad_C_demo_UFSC,
PQ_IP_VIP_DLC_ad_C_HD_UFSC,
PQ_IP_VIP_DLC_ad_C_R5G7BBrown5G9_UFSC,
PQ_IP_VIP_DLC_ad_C_NUMS_UFSC
} PQ_IP_VIP_DLC_ad_C_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_skin_protect_OFF_UFSC,
PQ_IP_VIP_DLC_skin_protect_NUMS_UFSC
} PQ_IP_VIP_DLC_skin_protect_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_dither_OFF_UFSC,
PQ_IP_VIP_DLC_dither_ON_UFSC,
PQ_IP_VIP_DLC_dither_NUMS_UFSC
} PQ_IP_VIP_DLC_dither_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_His_range_OFF_UFSC,
PQ_IP_VIP_DLC_His_range_ON_UFSC,
PQ_IP_VIP_DLC_His_range_NUMS_UFSC
} PQ_IP_VIP_DLC_His_range_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_His_rangeH_90pa_1366_UFSC,
PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC,
PQ_IP_VIP_DLC_His_rangeH_NUMS_UFSC
} PQ_IP_VIP_DLC_His_rangeH_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_His_rangeV_90pa_1366_UFSC,
PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC,
PQ_IP_VIP_DLC_His_rangeV_NUMS_UFSC
} PQ_IP_VIP_DLC_His_rangeV_Group_UFSC;

typedef enum
{
PQ_IP_VIP_DLC_PC_OFF_UFSC,
PQ_IP_VIP_DLC_PC_ON_UFSC,
PQ_IP_VIP_DLC_PC_NUMS_UFSC
} PQ_IP_VIP_DLC_PC_Group_UFSC;

typedef enum
{
PQ_IP_VIP_BWLE_OFF_UFSC,
PQ_IP_VIP_BWLE_ON_UFSC,
PQ_IP_VIP_BWLE_NUMS_UFSC
} PQ_IP_VIP_BWLE_Group_UFSC;

typedef enum
{
PQ_IP_VIP_BLE_OFF_UFSC,
PQ_IP_VIP_BLE_0x82_0x40_UFSC,
PQ_IP_VIP_BLE_0x82_0x50_UFSC,
PQ_IP_VIP_BLE_0x82_0x60_UFSC,
PQ_IP_VIP_BLE_0x84_0x40_UFSC,
PQ_IP_VIP_BLE_0x86_0x40_UFSC,
PQ_IP_VIP_BLE_0x88_0x40_UFSC,
PQ_IP_VIP_BLE_0x88_0x60_UFSC,
PQ_IP_VIP_BLE_0x8A_0x40_UFSC,
PQ_IP_VIP_BLE_NUMS_UFSC
} PQ_IP_VIP_BLE_Group_UFSC;

typedef enum
{
PQ_IP_VIP_WLE_OFF_UFSC,
PQ_IP_VIP_WLE_0x78_0x20_UFSC,
PQ_IP_VIP_WLE_NUMS_UFSC
} PQ_IP_VIP_WLE_Group_UFSC;

typedef enum
{
PQ_IP_VIP_BWLE_dither_OFF_UFSC,
PQ_IP_VIP_BWLE_dither_ON_UFSC,
PQ_IP_VIP_BWLE_dither_NUMS_UFSC
} PQ_IP_VIP_BWLE_dither_Group_UFSC;

typedef enum
{
PQ_IP_VIP_UVC_OFF_UFSC,
PQ_IP_VIP_UVC_ON_UFSC,
PQ_IP_VIP_UVC_ON_6CC_UFSC,
PQ_IP_VIP_UVC_ON_RGB_UFSC,
PQ_IP_VIP_UVC_ON_HDR_UFSC,
PQ_IP_VIP_UVC_NUMS_UFSC
} PQ_IP_VIP_UVC_Group_UFSC;

typedef enum
{
PQ_IP_VIP_YCbCr_Clip_OFF_UFSC,
PQ_IP_VIP_YCbCr_Clip_NUMS_UFSC
} PQ_IP_VIP_YCbCr_Clip_Group_UFSC;

typedef enum
{
PQ_IP_SWDriver_OFF_UFSC,
PQ_IP_SWDriver_ON_UFSC,
PQ_IP_SWDriver_SD_FRC_UFSC,
PQ_IP_SWDriver_HD_FRC_UFSC,
PQ_IP_SWDriver_0915_UFSC,
PQ_IP_SWDriver_Frange_UFSC,
PQ_IP_SWDriver_Lrange_UFSC,
PQ_IP_SWDriver_Lrange_0205_UFSC,
PQ_IP_SWDriver_Lrange_0216_UFSC,
PQ_IP_SWDriver_Lrange_0323_UFSC,
PQ_IP_SWDriver_HDR_SROFF_UFSC,
PQ_IP_SWDriver_NUMS_UFSC
} PQ_IP_SWDriver_Group_UFSC;

typedef enum
{
PQ_IP_SC_End_End_UFSC,
PQ_IP_SC_End_NUMS_UFSC
} PQ_IP_SC_End_Group_UFSC;

extern code U8 MST_SkipRule_IP_UFSC[PQ_IP_NUM_UFSC];
extern code EN_IPTAB_INFO PQ_IPTAB_INFO_UFSC[];
extern code U8 QMAP_1920_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC];
extern code U8 QMAP_1366_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC];
extern code U8 QMAP_4K_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC];

#endif
