
code U8 QMAP_4K_UFSC[QM_INPUTTYPE_NUM_UFSC][PQ_IP_NUM_UFSC]=
{
    #if PQ_QM_HMDI
    {//4K 4K2K_RGB444, 0
    PQ_IP_SC_Init_Init_UFSC, PQ_IP_UC_CTL_OFF_UFSC, PQ_IP_SPF_OFF_UFSC, PQ_IP_DMS_OFF_UFSC, PQ_IP_DMS_H_S_TCL_UFSC, 
    PQ_IP_DMS_V_S_TCL_UFSC, PQ_IP_DMS_Cplx_OFF_UFSC, PQ_IP_V12L_OFF_UFSC, PQ_IP_AA_OFF_UFSC, PQ_IP_AA_H_OFF_UFSC, 
    PQ_IP_AA_V_OFF_UFSC, PQ_IP_BNF_OFF_UFSC, PQ_IP_PreEnhance_OFF_UFSC, PQ_IP_PreEnhance_PrePK_OFF_UFSC, PQ_IP_PreEnhance_LDE3x3_OFF_UFSC, 
    PQ_IP_PreEnhance_PK3x3_OFF_UFSC, PQ_IP_PreEnhance_BLTI_OFF_UFSC, PQ_IP_SPF_PrePK_Kernel_OFF_UFSC, PQ_IP_ANN_OFF_UFSC, PQ_IP_ERC_OFF_UFSC, 
    PQ_IP_ERCL0_OFF_UFSC, PQ_IP_ERCL1_OFF_UFSC, PQ_IP_ERCL2_OFF_UFSC, PQ_IP_ERC_Modify_MTISL_OFF_UFSC, PQ_IP_ERC_Modify_DSMINSTD_OFF_UFSC, 
    PQ_IP_ERC_Modify_Others_OFF_UFSC, PQ_IP_P2M_OFF_UFSC, PQ_IP_INIT_FAC_Type0_UFSC, PQ_IP_DBI_OFF_UFSC, PQ_IP_DS_OFF_UFSC, 
    PQ_IP_DS_180_AD_C12_UFSC, PQ_IP_DS_180_BC_C13_UFSC, PQ_IP_DS_180_R_C5_UFSC, PQ_IP_DS_11_AD_C7_UFSC, PQ_IP_DS_11_BC_C8_UFSC, 
    PQ_IP_DS_11_R_C4_UFSC, PQ_IP_DS_12_AD_C10_UFSC, PQ_IP_DS_12_BC_C11_UFSC, PQ_IP_DS_12_R_C4_UFSC, PQ_IP_DS_13_AD_C10_UFSC, 
    PQ_IP_DS_13_BC_C11_UFSC, PQ_IP_DS_13_R_C4_UFSC, PQ_IP_VSP_Y_Bypass_UFSC, PQ_IP_VSP_C_Bypass_UFSC, PQ_IP_VSP_CoRing_OFF_UFSC, 
    PQ_IP_VSP_DeRing_OFF_UFSC, PQ_IP_VSP_Dither_OFF_UFSC, PQ_IP_VSP_PreVBound_OFF_UFSC, PQ_IP_PreCTI_OFF_UFSC, PQ_IP_HSP_Y_Bypass_UFSC, 
    PQ_IP_HSP_C_Bypass_UFSC, PQ_IP_HSP_CoRing_OFF_UFSC, PQ_IP_HSP_DeRing_OFF_UFSC, PQ_IP_HSP_Dither_OFF_UFSC, PQ_IP_HnonLinear_OFF_UFSC, 
    PQ_IP_SRAM1_InvSinc6Tc4p4Fc87Apass01Astop50_UFSC, PQ_IP_SRAM2_InvSinc8Tc4p4Fc85Apass0001Astop60_UFSC, PQ_IP_C_SRAM1_C121_UFSC, PQ_IP_C_SRAM2_C2121_UFSC, PQ_IP_SemiDictionarySR_OFF_UFSC, 
    PQ_IP_SemiDictionarySR_filter_filter_15_UFSC, PQ_IP_NLM_OFF_UFSC, PQ_IP_VIP_OFF_UFSC, PQ_IP_VIP_pseudo_OFF_UFSC, PQ_IP_VIP_LineBuffer_OFF_UFSC, 
    PQ_IP_VIP_BGD_OFF_UFSC, PQ_IP_VIP_Post3x3PK_OFF_UFSC, PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC, PQ_IP_VIP_YC_gain_offset_OFF_UFSC, PQ_IP_VIP_LCE_OFF_UFSC, 
    PQ_IP_VIP_LCE_dither_OFF_UFSC, PQ_IP_VIP_LCE_setting_S3_UFSC, PQ_IP_VIP_LCE_ad_C_OFF_UFSC, PQ_IP_VIP_LCE_curve_CV1_UFSC, PQ_IP_VIP_DLC_OFF_UFSC, 
    PQ_IP_VIP_DLC_ad_C_OFF_UFSC, PQ_IP_VIP_DLC_skin_protect_OFF_UFSC, PQ_IP_VIP_DLC_dither_OFF_UFSC, PQ_IP_VIP_DLC_His_range_ON_UFSC, PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC, 
    PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC, PQ_IP_VIP_DLC_PC_OFF_UFSC, PQ_IP_VIP_BWLE_OFF_UFSC, PQ_IP_VIP_BLE_OFF_UFSC, PQ_IP_VIP_WLE_OFF_UFSC, 
    PQ_IP_VIP_BWLE_dither_OFF_UFSC, PQ_IP_VIP_UVC_OFF_UFSC, PQ_IP_VIP_YCbCr_Clip_OFF_UFSC, PQ_IP_SWDriver_OFF_UFSC, PQ_IP_SC_End_End_UFSC, 
    
    },
    #endif
    #if PQ_QM_HMDI
    {//4K 4K2K_YUV444, 1
    PQ_IP_SC_Init_Init_UFSC, PQ_IP_UC_CTL_Y8M4_UFSC, PQ_IP_SPF_ON_UFSC, PQ_IP_DMS_ON_UFSC, PQ_IP_DMS_H_S_TCL_UFSC, 
    PQ_IP_DMS_V_S_TCL_UFSC, PQ_IP_DMS_Cplx_cplx_th5A_UFSC, PQ_IP_V12L_OFF_UFSC, PQ_IP_AA_S2_UFSC, PQ_IP_AA_H_S2_UFSC, 
    PQ_IP_AA_V_OFF_UFSC, PQ_IP_BNF_ON_UFSC, PQ_IP_PreEnhance_ON_UFSC, PQ_IP_PreEnhance_PrePK_OFF_UFSC, PQ_IP_PreEnhance_LDE3x3_ON_UFSC, 
    PQ_IP_PreEnhance_PK3x3_SR_2_UFSC, PQ_IP_PreEnhance_BLTI_OFF_UFSC, PQ_IP_SPF_PrePK_Kernel_ON_UFSC, PQ_IP_ANN_OFF_UFSC, PQ_IP_ERC_OFF_UFSC, 
    PQ_IP_ERCL0_S1_UFSC, PQ_IP_ERCL1_S1_UFSC, PQ_IP_ERCL2_S1_UFSC, PQ_IP_ERC_Modify_MTISL_ON_UFSC, PQ_IP_ERC_Modify_DSMINSTD_ON_UFSC, 
    PQ_IP_ERC_Modify_Others_S1_UFSC, PQ_IP_P2M_W1_UFSC, PQ_IP_INIT_FAC_Type0_UFSC, PQ_IP_DBI_W0_UFSC, PQ_IP_DS_OFF_UFSC, 
    PQ_IP_DS_180_AD_C12_UFSC, PQ_IP_DS_180_BC_C13_UFSC, PQ_IP_DS_180_R_C5_UFSC, PQ_IP_DS_11_AD_C7_UFSC, PQ_IP_DS_11_BC_C8_UFSC, 
    PQ_IP_DS_11_R_C4_UFSC, PQ_IP_DS_12_AD_C10_UFSC, PQ_IP_DS_12_BC_C11_UFSC, PQ_IP_DS_12_R_C4_UFSC, PQ_IP_DS_13_AD_C10_UFSC, 
    PQ_IP_DS_13_BC_C11_UFSC, PQ_IP_DS_13_R_C4_UFSC, PQ_IP_VSP_Y_Bypass_UFSC, PQ_IP_VSP_C_Bypass_UFSC, PQ_IP_VSP_CoRing_OFF_UFSC, 
    PQ_IP_VSP_DeRing_OFF_UFSC, PQ_IP_VSP_Dither_OFF_UFSC, PQ_IP_VSP_PreVBound_OFF_UFSC, PQ_IP_PreCTI_OFF_UFSC, PQ_IP_HSP_Y_Bypass_UFSC, 
    PQ_IP_HSP_C_Bypass_UFSC, PQ_IP_HSP_CoRing_OFF_UFSC, PQ_IP_HSP_DeRing_OFF_UFSC, PQ_IP_HSP_Dither_OFF_UFSC, PQ_IP_HnonLinear_OFF_UFSC, 
    PQ_IP_SRAM1_InvSinc6Tc4p4Fc87Apass01Astop50_UFSC, PQ_IP_SRAM2_InvSinc8Tc4p4Fc85Apass0001Astop60_UFSC, PQ_IP_C_SRAM1_C121_UFSC, PQ_IP_C_SRAM2_C2121_UFSC, PQ_IP_SemiDictionarySR_S1210_UFSC, 
    PQ_IP_SemiDictionarySR_filter_filter_15_UFSC, PQ_IP_NLM_UHD_UFSC, PQ_IP_VIP_ON_UFSC, PQ_IP_VIP_pseudo_OFF_UFSC, PQ_IP_VIP_LineBuffer_OFF_UFSC, 
    PQ_IP_VIP_BGD_ON_UFSC, PQ_IP_VIP_Post3x3PK_1120_UFSC, PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC, PQ_IP_VIP_YC_gain_offset_OFF_UFSC, PQ_IP_VIP_LCE_ON_parallel_UFSC, 
    PQ_IP_VIP_LCE_dither_ON_UFSC, PQ_IP_VIP_LCE_setting_S3_UFSC, PQ_IP_VIP_LCE_ad_C_R5_UFSC, PQ_IP_VIP_LCE_curve_CV1_UFSC, PQ_IP_VIP_DLC_ON_UFSC, 
    PQ_IP_VIP_DLC_ad_C_HD_UFSC, PQ_IP_VIP_DLC_skin_protect_OFF_UFSC, PQ_IP_VIP_DLC_dither_ON_UFSC, PQ_IP_VIP_DLC_His_range_ON_UFSC, PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC, 
    PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC, PQ_IP_VIP_DLC_PC_OFF_UFSC, PQ_IP_VIP_BWLE_ON_UFSC, PQ_IP_VIP_BLE_OFF_UFSC, PQ_IP_VIP_WLE_OFF_UFSC, 
    PQ_IP_VIP_BWLE_dither_ON_UFSC, PQ_IP_VIP_UVC_OFF_UFSC, PQ_IP_VIP_YCbCr_Clip_OFF_UFSC, PQ_IP_SWDriver_OFF_UFSC, PQ_IP_SC_End_End_UFSC, 
    
    },
    #endif
    #if PQ_QM_HMDI
    {//4K 4K2K_YUV422, 2
    PQ_IP_SC_Init_Init_UFSC, PQ_IP_UC_CTL_Y8M4_UFSC, PQ_IP_SPF_ON_UFSC, PQ_IP_DMS_ON_UFSC, PQ_IP_DMS_H_S_TCL_UFSC, 
    PQ_IP_DMS_V_S_TCL_UFSC, PQ_IP_DMS_Cplx_cplx_th5A_UFSC, PQ_IP_V12L_OFF_UFSC, PQ_IP_AA_S2_UFSC, PQ_IP_AA_H_S2_UFSC, 
    PQ_IP_AA_V_OFF_UFSC, PQ_IP_BNF_ON_UFSC, PQ_IP_PreEnhance_ON_UFSC, PQ_IP_PreEnhance_PrePK_OFF_UFSC, PQ_IP_PreEnhance_LDE3x3_ON_UFSC, 
    PQ_IP_PreEnhance_PK3x3_SR_2_UFSC, PQ_IP_PreEnhance_BLTI_OFF_UFSC, PQ_IP_SPF_PrePK_Kernel_ON_UFSC, PQ_IP_ANN_OFF_UFSC, PQ_IP_ERC_OFF_UFSC, 
    PQ_IP_ERCL0_S1_UFSC, PQ_IP_ERCL1_S1_UFSC, PQ_IP_ERCL2_S1_UFSC, PQ_IP_ERC_Modify_MTISL_ON_UFSC, PQ_IP_ERC_Modify_DSMINSTD_ON_UFSC, 
    PQ_IP_ERC_Modify_Others_S1_UFSC, PQ_IP_P2M_W1_UFSC, PQ_IP_INIT_FAC_Type0_UFSC, PQ_IP_DBI_W0_UFSC, PQ_IP_DS_OFF_UFSC, 
    PQ_IP_DS_180_AD_C12_UFSC, PQ_IP_DS_180_BC_C13_UFSC, PQ_IP_DS_180_R_C5_UFSC, PQ_IP_DS_11_AD_C7_UFSC, PQ_IP_DS_11_BC_C8_UFSC, 
    PQ_IP_DS_11_R_C4_UFSC, PQ_IP_DS_12_AD_C10_UFSC, PQ_IP_DS_12_BC_C11_UFSC, PQ_IP_DS_12_R_C4_UFSC, PQ_IP_DS_13_AD_C10_UFSC, 
    PQ_IP_DS_13_BC_C11_UFSC, PQ_IP_DS_13_R_C4_UFSC, PQ_IP_VSP_Y_Bypass_UFSC, PQ_IP_VSP_C_Bypass_UFSC, PQ_IP_VSP_CoRing_OFF_UFSC, 
    PQ_IP_VSP_DeRing_OFF_UFSC, PQ_IP_VSP_Dither_OFF_UFSC, PQ_IP_VSP_PreVBound_OFF_UFSC, PQ_IP_PreCTI_OFF_UFSC, PQ_IP_HSP_Y_Bypass_UFSC, 
    PQ_IP_HSP_C_Bypass_UFSC, PQ_IP_HSP_CoRing_OFF_UFSC, PQ_IP_HSP_DeRing_OFF_UFSC, PQ_IP_HSP_Dither_OFF_UFSC, PQ_IP_HnonLinear_OFF_UFSC, 
    PQ_IP_SRAM1_InvSinc6Tc4p4Fc87Apass01Astop50_UFSC, PQ_IP_SRAM2_InvSinc8Tc4p4Fc85Apass0001Astop60_UFSC, PQ_IP_C_SRAM1_C121_UFSC, PQ_IP_C_SRAM2_C2121_UFSC, PQ_IP_SemiDictionarySR_S1210_UFSC, 
    PQ_IP_SemiDictionarySR_filter_filter_15_UFSC, PQ_IP_NLM_UHD_UFSC, PQ_IP_VIP_ON_UFSC, PQ_IP_VIP_pseudo_OFF_UFSC, PQ_IP_VIP_LineBuffer_OFF_UFSC, 
    PQ_IP_VIP_BGD_ON_UFSC, PQ_IP_VIP_Post3x3PK_1120_UFSC, PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC, PQ_IP_VIP_YC_gain_offset_OFF_UFSC, PQ_IP_VIP_LCE_ON_parallel_UFSC, 
    PQ_IP_VIP_LCE_dither_ON_UFSC, PQ_IP_VIP_LCE_setting_S3_UFSC, PQ_IP_VIP_LCE_ad_C_R5_UFSC, PQ_IP_VIP_LCE_curve_CV1_UFSC, PQ_IP_VIP_DLC_ON_UFSC, 
    PQ_IP_VIP_DLC_ad_C_HD_UFSC, PQ_IP_VIP_DLC_skin_protect_OFF_UFSC, PQ_IP_VIP_DLC_dither_ON_UFSC, PQ_IP_VIP_DLC_His_range_ON_UFSC, PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC, 
    PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC, PQ_IP_VIP_DLC_PC_OFF_UFSC, PQ_IP_VIP_BWLE_ON_UFSC, PQ_IP_VIP_BLE_OFF_UFSC, PQ_IP_VIP_WLE_OFF_UFSC, 
    PQ_IP_VIP_BWLE_dither_ON_UFSC, PQ_IP_VIP_UVC_OFF_UFSC, PQ_IP_VIP_YCbCr_Clip_OFF_UFSC, PQ_IP_SWDriver_OFF_UFSC, PQ_IP_SC_End_End_UFSC, 
    
    },
    #endif
    #if PQ_QM_HMDI
    {//4K FHD_RGB444, 3
    PQ_IP_SC_Init_Init_UFSC, PQ_IP_UC_CTL_OFF_UFSC, PQ_IP_SPF_OFF_UFSC, PQ_IP_DMS_OFF_UFSC, PQ_IP_DMS_H_L0716_UFSC, 
    PQ_IP_DMS_V_L0716_UFSC, PQ_IP_DMS_Cplx_OFF_UFSC, PQ_IP_V12L_OFF_UFSC, PQ_IP_AA_OFF_UFSC, PQ_IP_AA_H_OFF_UFSC, 
    PQ_IP_AA_V_OFF_UFSC, PQ_IP_BNF_OFF_UFSC, PQ_IP_PreEnhance_OFF_UFSC, PQ_IP_PreEnhance_PrePK_OFF_UFSC, PQ_IP_PreEnhance_LDE3x3_OFF_UFSC, 
    PQ_IP_PreEnhance_PK3x3_OFF_UFSC, PQ_IP_PreEnhance_BLTI_OFF_UFSC, PQ_IP_SPF_PrePK_Kernel_OFF_UFSC, PQ_IP_ANN_OFF_UFSC, PQ_IP_ERC_OFF_UFSC, 
    PQ_IP_ERCL0_OFF_UFSC, PQ_IP_ERCL1_OFF_UFSC, PQ_IP_ERCL2_OFF_UFSC, PQ_IP_ERC_Modify_MTISL_OFF_UFSC, PQ_IP_ERC_Modify_DSMINSTD_OFF_UFSC, 
    PQ_IP_ERC_Modify_Others_OFF_UFSC, PQ_IP_P2M_OFF_UFSC, PQ_IP_INIT_FAC_Type1_UFSC, PQ_IP_DBI_OFF_UFSC, PQ_IP_DS_OFF_UFSC, 
    PQ_IP_DS_180_AD_C12_UFSC, PQ_IP_DS_180_BC_C13_UFSC, PQ_IP_DS_180_R_C5_UFSC, PQ_IP_DS_11_AD_C7_UFSC, PQ_IP_DS_11_BC_C8_UFSC, 
    PQ_IP_DS_11_R_C4_UFSC, PQ_IP_DS_12_AD_C10_UFSC, PQ_IP_DS_12_BC_C11_UFSC, PQ_IP_DS_12_R_C4_UFSC, PQ_IP_DS_13_AD_C10_UFSC, 
    PQ_IP_DS_13_BC_C11_UFSC, PQ_IP_DS_13_R_C4_UFSC, PQ_IP_VSP_Y_SRAM_1_4Tap_UFSC, PQ_IP_VSP_C_SRAM_1_4Tap_UFSC, PQ_IP_VSP_CoRing_OFF_UFSC, 
    PQ_IP_VSP_DeRing_OFF_UFSC, PQ_IP_VSP_Dither_OFF_UFSC, PQ_IP_VSP_PreVBound_OFF_UFSC, PQ_IP_PreCTI_OFF_UFSC, PQ_IP_HSP_Y_SRAM_2_4Tap_UFSC, 
    PQ_IP_HSP_C_SRAM_2_4Tap_UFSC, PQ_IP_HSP_CoRing_OFF_UFSC, PQ_IP_HSP_DeRing_OFF_UFSC, PQ_IP_HSP_Dither_OFF_UFSC, PQ_IP_HnonLinear_OFF_UFSC, 
    PQ_IP_SRAM1_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC, PQ_IP_SRAM2_InvSinc4Tc4p4Fc75Apass0001Astop40_UFSC, PQ_IP_C_SRAM1_C121_UFSC, PQ_IP_C_SRAM2_C2121_UFSC, PQ_IP_SemiDictionarySR_OFF_UFSC, 
    PQ_IP_SemiDictionarySR_filter_filter_15_UFSC, PQ_IP_NLM_OFF_UFSC, PQ_IP_VIP_OFF_UFSC, PQ_IP_VIP_pseudo_OFF_UFSC, PQ_IP_VIP_LineBuffer_OFF_UFSC, 
    PQ_IP_VIP_BGD_OFF_UFSC, PQ_IP_VIP_Post3x3PK_OFF_UFSC, PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC, PQ_IP_VIP_YC_gain_offset_OFF_UFSC, PQ_IP_VIP_LCE_OFF_UFSC, 
    PQ_IP_VIP_LCE_dither_OFF_UFSC, PQ_IP_VIP_LCE_setting_S3_UFSC, PQ_IP_VIP_LCE_ad_C_OFF_UFSC, PQ_IP_VIP_LCE_curve_CV1_UFSC, PQ_IP_VIP_DLC_OFF_UFSC, 
    PQ_IP_VIP_DLC_ad_C_OFF_UFSC, PQ_IP_VIP_DLC_skin_protect_OFF_UFSC, PQ_IP_VIP_DLC_dither_OFF_UFSC, PQ_IP_VIP_DLC_His_range_ON_UFSC, PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC, 
    PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC, PQ_IP_VIP_DLC_PC_OFF_UFSC, PQ_IP_VIP_BWLE_OFF_UFSC, PQ_IP_VIP_BLE_OFF_UFSC, PQ_IP_VIP_WLE_OFF_UFSC, 
    PQ_IP_VIP_BWLE_dither_OFF_UFSC, PQ_IP_VIP_UVC_OFF_UFSC, PQ_IP_VIP_YCbCr_Clip_OFF_UFSC, PQ_IP_SWDriver_OFF_UFSC, PQ_IP_SC_End_End_UFSC, 
    
    },
    #endif
    #if PQ_QM_HMDI
    {//4K FHD_YUV444, 4
    PQ_IP_SC_Init_Init_UFSC, PQ_IP_UC_CTL_Y8M4_UFSC, PQ_IP_SPF_ON_UFSC, PQ_IP_DMS_L0716_UFSC, PQ_IP_DMS_H_L0716_UFSC, 
    PQ_IP_DMS_V_L0716_UFSC, PQ_IP_DMS_Cplx_L1117_UFSC, PQ_IP_V12L_ON_UFSC, PQ_IP_AA_S2_UFSC, PQ_IP_AA_H_S2_UFSC, 
    PQ_IP_AA_V_S2_UFSC, PQ_IP_BNF_ON_UFSC, PQ_IP_PreEnhance_ON_UFSC, PQ_IP_PreEnhance_PrePK_OFF_UFSC, PQ_IP_PreEnhance_LDE3x3_ON_UFSC, 
    PQ_IP_PreEnhance_PK3x3_ON_UFSC, PQ_IP_PreEnhance_BLTI_ON_UFSC, PQ_IP_SPF_PrePK_Kernel_ON_UFSC, PQ_IP_ANN_ON_UFSC, PQ_IP_ERC_S1_UFSC, 
    PQ_IP_ERCL0_S1_UFSC, PQ_IP_ERCL1_S1_UFSC, PQ_IP_ERCL2_S1_UFSC, PQ_IP_ERC_Modify_MTISL_ON_UFSC, PQ_IP_ERC_Modify_DSMINSTD_ON_UFSC, 
    PQ_IP_ERC_Modify_Others_S1_UFSC, PQ_IP_P2M_SR_UFSC, PQ_IP_INIT_FAC_Type1_UFSC, PQ_IP_DBI_OFF_UFSC, PQ_IP_DS_OFF_UFSC, 
    PQ_IP_DS_180_AD_C12_UFSC, PQ_IP_DS_180_BC_C13_UFSC, PQ_IP_DS_180_R_C5_UFSC, PQ_IP_DS_11_AD_C7_UFSC, PQ_IP_DS_11_BC_C8_UFSC, 
    PQ_IP_DS_11_R_C4_UFSC, PQ_IP_DS_12_AD_C10_UFSC, PQ_IP_DS_12_BC_C11_UFSC, PQ_IP_DS_12_R_C4_UFSC, PQ_IP_DS_13_AD_C10_UFSC, 
    PQ_IP_DS_13_BC_C11_UFSC, PQ_IP_DS_13_R_C4_UFSC, PQ_IP_VSP_Y_SRAM_1_4Tap_UFSC, PQ_IP_VSP_C_C_SRAM_1_UFSC, PQ_IP_VSP_CoRing_OFF_UFSC, 
    PQ_IP_VSP_DeRing_DR1_UFSC, PQ_IP_VSP_Dither_OFF_UFSC, PQ_IP_VSP_PreVBound_OFF_UFSC, PQ_IP_PreCTI_OFF_UFSC, PQ_IP_HSP_Y_SRAM_2_4Tap_UFSC, 
    PQ_IP_HSP_C_C_SRAM_1_UFSC, PQ_IP_HSP_CoRing_OFF_UFSC, PQ_IP_HSP_DeRing_DR1_UFSC, PQ_IP_HSP_Dither_OFF_UFSC, PQ_IP_HnonLinear_OFF_UFSC, 
    PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Apass01Astop50_UFSC, PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Apass01Astop50_UFSC, PQ_IP_C_SRAM1_C121_UFSC, PQ_IP_C_SRAM2_C2121_UFSC, PQ_IP_SemiDictionarySR_S1210_UFSC, 
    PQ_IP_SemiDictionarySR_filter_filter_15_UFSC, PQ_IP_NLM_S1_UFSC, PQ_IP_VIP_ON_UFSC, PQ_IP_VIP_pseudo_OFF_UFSC, PQ_IP_VIP_LineBuffer_OFF_UFSC, 
    PQ_IP_VIP_BGD_ON_UFSC, PQ_IP_VIP_Post3x3PK_1120_UFSC, PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC, PQ_IP_VIP_YC_gain_offset_OFF_UFSC, PQ_IP_VIP_LCE_OFF_UFSC, 
    PQ_IP_VIP_LCE_dither_ON_UFSC, PQ_IP_VIP_LCE_setting_S3_UFSC, PQ_IP_VIP_LCE_ad_C_OFF_UFSC, PQ_IP_VIP_LCE_curve_CV1_UFSC, PQ_IP_VIP_DLC_ON_UFSC, 
    PQ_IP_VIP_DLC_ad_C_HD_UFSC, PQ_IP_VIP_DLC_skin_protect_OFF_UFSC, PQ_IP_VIP_DLC_dither_ON_UFSC, PQ_IP_VIP_DLC_His_range_ON_UFSC, PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC, 
    PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC, PQ_IP_VIP_DLC_PC_OFF_UFSC, PQ_IP_VIP_BWLE_ON_UFSC, PQ_IP_VIP_BLE_OFF_UFSC, PQ_IP_VIP_WLE_OFF_UFSC, 
    PQ_IP_VIP_BWLE_dither_ON_UFSC, PQ_IP_VIP_UVC_OFF_UFSC, PQ_IP_VIP_YCbCr_Clip_OFF_UFSC, PQ_IP_SWDriver_ON_UFSC, PQ_IP_SC_End_End_UFSC, 
    
    },
    #endif
    #if PQ_QM_HMDI
    {//4K FHD_YUV422, 5
    PQ_IP_SC_Init_Init_UFSC, PQ_IP_UC_CTL_Y8M4_UFSC, PQ_IP_SPF_ON_UFSC, PQ_IP_DMS_L0716_UFSC, PQ_IP_DMS_H_L0716_UFSC, 
    PQ_IP_DMS_V_L0716_UFSC, PQ_IP_DMS_Cplx_L1117_UFSC, PQ_IP_V12L_ON_UFSC, PQ_IP_AA_S2_UFSC, PQ_IP_AA_H_S2_UFSC, 
    PQ_IP_AA_V_S2_UFSC, PQ_IP_BNF_ON_UFSC, PQ_IP_PreEnhance_ON_UFSC, PQ_IP_PreEnhance_PrePK_OFF_UFSC, PQ_IP_PreEnhance_LDE3x3_ON_UFSC, 
    PQ_IP_PreEnhance_PK3x3_ON_UFSC, PQ_IP_PreEnhance_BLTI_ON_UFSC, PQ_IP_SPF_PrePK_Kernel_ON_UFSC, PQ_IP_ANN_ON_UFSC, PQ_IP_ERC_S1_UFSC, 
    PQ_IP_ERCL0_S1_UFSC, PQ_IP_ERCL1_S1_UFSC, PQ_IP_ERCL2_S1_UFSC, PQ_IP_ERC_Modify_MTISL_ON_UFSC, PQ_IP_ERC_Modify_DSMINSTD_ON_UFSC, 
    PQ_IP_ERC_Modify_Others_S1_UFSC, PQ_IP_P2M_SR_UFSC, PQ_IP_INIT_FAC_Type1_UFSC, PQ_IP_DBI_OFF_UFSC, PQ_IP_DS_OFF_UFSC, 
    PQ_IP_DS_180_AD_C12_UFSC, PQ_IP_DS_180_BC_C13_UFSC, PQ_IP_DS_180_R_C5_UFSC, PQ_IP_DS_11_AD_C7_UFSC, PQ_IP_DS_11_BC_C8_UFSC, 
    PQ_IP_DS_11_R_C4_UFSC, PQ_IP_DS_12_AD_C10_UFSC, PQ_IP_DS_12_BC_C11_UFSC, PQ_IP_DS_12_R_C4_UFSC, PQ_IP_DS_13_AD_C10_UFSC, 
    PQ_IP_DS_13_BC_C11_UFSC, PQ_IP_DS_13_R_C4_UFSC, PQ_IP_VSP_Y_SRAM_1_4Tap_UFSC, PQ_IP_VSP_C_C_SRAM_1_UFSC, PQ_IP_VSP_CoRing_OFF_UFSC, 
    PQ_IP_VSP_DeRing_DR1_UFSC, PQ_IP_VSP_Dither_OFF_UFSC, PQ_IP_VSP_PreVBound_OFF_UFSC, PQ_IP_PreCTI_OFF_UFSC, PQ_IP_HSP_Y_SRAM_2_4Tap_UFSC, 
    PQ_IP_HSP_C_C_SRAM_1_UFSC, PQ_IP_HSP_CoRing_OFF_UFSC, PQ_IP_HSP_DeRing_DR1_UFSC, PQ_IP_HSP_Dither_OFF_UFSC, PQ_IP_HnonLinear_OFF_UFSC, 
    PQ_IP_SRAM1_InvSinc4Tc4p4Fc85Apass01Astop50_UFSC, PQ_IP_SRAM2_InvSinc4Tc4p4Fc85Apass01Astop50_UFSC, PQ_IP_C_SRAM1_C121_UFSC, PQ_IP_C_SRAM2_C2121_UFSC, PQ_IP_SemiDictionarySR_S1210_UFSC, 
    PQ_IP_SemiDictionarySR_filter_filter_15_UFSC, PQ_IP_NLM_S1_UFSC, PQ_IP_VIP_ON_UFSC, PQ_IP_VIP_pseudo_OFF_UFSC, PQ_IP_VIP_LineBuffer_ON_UFSC, 
    PQ_IP_VIP_BGD_ON_UFSC, PQ_IP_VIP_Post3x3PK_1120_UFSC, PQ_IP_VIP_ICC_CRD_SRAM_15wins3_UFSC, PQ_IP_VIP_YC_gain_offset_OFF_UFSC, PQ_IP_VIP_LCE_OFF_UFSC, 
    PQ_IP_VIP_LCE_dither_ON_UFSC, PQ_IP_VIP_LCE_setting_S3_UFSC, PQ_IP_VIP_LCE_ad_C_OFF_UFSC, PQ_IP_VIP_LCE_curve_CV1_UFSC, PQ_IP_VIP_DLC_ON_UFSC, 
    PQ_IP_VIP_DLC_ad_C_HD_UFSC, PQ_IP_VIP_DLC_skin_protect_OFF_UFSC, PQ_IP_VIP_DLC_dither_ON_UFSC, PQ_IP_VIP_DLC_His_range_ON_UFSC, PQ_IP_VIP_DLC_His_rangeH_90pa_1920_UFSC, 
    PQ_IP_VIP_DLC_His_rangeV_90pa_1920_UFSC, PQ_IP_VIP_DLC_PC_OFF_UFSC, PQ_IP_VIP_BWLE_ON_UFSC, PQ_IP_VIP_BLE_OFF_UFSC, PQ_IP_VIP_WLE_OFF_UFSC, 
    PQ_IP_VIP_BWLE_dither_ON_UFSC, PQ_IP_VIP_UVC_OFF_UFSC, PQ_IP_VIP_YCbCr_Clip_OFF_UFSC, PQ_IP_SWDriver_ON_UFSC, PQ_IP_SC_End_End_UFSC, 
    
    },
    #endif
};

