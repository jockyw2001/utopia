#include "hwreg_frc_map.h" 
#include "Maserati_ACT_4K0_5K.h"

// ACT_4K0_5K
// 3D_4K0_5K_ACT_NO_FRC
void MFC_3D_4K0_5K_ACT_3D_4K0_5K_ACT_NO_FRC(void)
{
// FSC_TOP
    MDrv_WriteByteMask( REG_FSC_BK20_A2, 0x80, 0xff); // reg_splt_gb_in_size_r
    MDrv_WriteByteMask( REG_FSC_BK20_A3, 0x00, 0x01); // reg_splt_gb_in_size_r
    MDrv_WriteByteMask( REG_FSC_BK20_A4, 0x00, 0xff); // reg_splt_h_size
    MDrv_WriteByteMask( REG_FSC_BK20_A5, 0x0f, 0x1f); // reg_splt_h_size
    MDrv_WriteByteMask( REG_FSC_BK20_A6, 0x80, 0xff); // reg_splt_h_size_l
    MDrv_WriteByteMask( REG_FSC_BK20_A7, 0x07, 0x0f); // reg_splt_h_size_l
    MDrv_WriteByteMask( REG_FSC_BK20_A8, 0x80, 0x80); // reg_mcm_lr_en
    MDrv_WriteByteMask( REG_FSC_BK20_49, 0x08, 0x0f); // reg_vertical_limit_cnt
    MDrv_WriteByteMask( REG_FSC_BK20_48, 0x70, 0xff); // reg_vertical_limit_cnt
    MDrv_WriteByteMask( REG_FSC_BK20_1D, 0x00, 0x1f); // reg_hde_st
    MDrv_WriteByteMask( REG_FSC_BK20_1C, 0x00, 0xff); // reg_hde_st
    MDrv_WriteByteMask( REG_FSC_BK20_1F, 0x00, 0x0f); // reg_vde_st
    MDrv_WriteByteMask( REG_FSC_BK20_1E, 0x00, 0xff); // reg_vde_st
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x04, 0x04); // reg_de_h_mask_en
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x08, 0x08); // reg_de_v_mask_en
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x10); // reg_hvmask_en
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x20); // reg_mask_hcnt_sel
    MDrv_WriteByteMask( REG_FSC_BK20_C1, 0x00, 0x40); // reg_mask_vcnt_sel
    MDrv_WriteByteMask( REG_FSC_BK20_CA, 0x00, 0x01); // reg_path_selection
    MDrv_WriteByteMask( REG_FSC_BK20_05, 0x10, 0x10); // reg_ckg_ficlk
    MDrv_WriteByteMask( REG_FSC_BK20_04, 0x0C, 0x1f); // reg_ckg_odclk
// CSC
// DD
    MDrv_WriteByteMask( REG_FSC_BK1D_02, 0x00, 0xff); // reg_dd_pixel_num (H)
    MDrv_WriteByteMask( REG_FSC_BK1D_03, 0x08, 0x0f); // reg_dd_pixel_num
    MDrv_WriteByteMask( REG_FSC_BK1D_04, 0x70, 0xff); // reg_dd_line_num (V)
    MDrv_WriteByteMask( REG_FSC_BK1D_05, 0x08, 0x0f); // reg_dd_line_num
    MDrv_WriteByteMask( REG_FSC_BK1D_0A, 0x40, 0x7f); // reg_flh_last_ctr
    MDrv_WriteByteMask( REG_FSC_BK1D_0B, 0x40, 0x7f); // reg_flh_req_ctr
    MDrv_WriteByteMask( REG_FSC_BK1D_0D, 0x00, 0x40); // reg_blk_md
// MCNR
    MDrv_WriteByteMask( REG_FSC_BK20_20, 0x00, 0xff); // reg_fetch_num
    MDrv_WriteByteMask( REG_FSC_BK20_21, 0x08, 0xff); // reg_fetch_num
    MDrv_WriteByteMask( REG_FSC_BK20_22, 0x70, 0xff); // reg_vlen
    MDrv_WriteByteMask( REG_FSC_BK20_23, 0x08, 0xff); // reg_vlen
// MCM
    MDrv_WriteByteMask( REG_FSC_BK12_1C, 0x80, 0xff); // reg_mcm_offset
    MDrv_WriteByteMask( REG_FSC_BK12_1D, 0x07, 0x1f); // reg_mcm_offset
    MDrv_WriteByteMask( REG_FSC_BK12_1E, 0x80, 0xff); // reg_mcm_fetch
    MDrv_WriteByteMask( REG_FSC_BK12_1F, 0x07, 0x1f); // reg_mcm_fetch
    MDrv_WriteByteMask( REG_FSC_BK12_60, 0x00, 0x0C); // reg_mcm_fi
    MDrv_WriteByteMask( REG_FSC_BK12_2F, 0x80, 0x80); // reg_ipm_cut_en
// VSU
    MDrv_WriteByteMask( REG_FSC_BK23_12, 0x00, 0xff); // vsp_scl_fac0
    MDrv_WriteByteMask( REG_FSC_BK23_13, 0x00, 0xff); // vsp_scl_fac1
    MDrv_WriteByteMask( REG_FSC_BK23_14, 0x10, 0xff); // vsp_scl_fac2
    MDrv_WriteByteMask( REG_FSC_BK23_15, 0x01, 0xff); // vsp_scl_en
    MDrv_WriteByteMask( REG_FSC_BK23_15, 0x00, 0x02); // vsp_shift_mode_en
    MDrv_WriteByteMask( REG_FSC_BK23_0A, 0x1C, 0xff); // vsp_vsize_in0
    MDrv_WriteByteMask( REG_FSC_BK23_0B, 0x02, 0xff); // vsp_vsize_in1
    MDrv_WriteByteMask( REG_FSC_BK23_0C, 0x1C, 0xff); // vsp_vsize_out0
    MDrv_WriteByteMask( REG_FSC_BK23_0D, 0x02, 0xff); // vsp_vsize_out1
// HSU
    MDrv_WriteByteMask( REG_FSC_BK23_0E, 0x00, 0xff); // hsp_scl_fac0
    MDrv_WriteByteMask( REG_FSC_BK23_0F, 0x00, 0xff); // hsp_scl_fac1
    MDrv_WriteByteMask( REG_FSC_BK23_10, 0x10, 0xff); // hsp_scl_fac2
    MDrv_WriteByteMask( REG_FSC_BK23_11, 0x01, 0xff); // hsp_scl_en
    MDrv_WriteByteMask( REG_FSC_BK23_11, 0x00, 0x02); // hsp_shift_mode_en
    MDrv_WriteByteMask( REG_FSC_BK23_34, 0x80, 0xff); // hsp_hsize_in0
    MDrv_WriteByteMask( REG_FSC_BK23_35, 0x07, 0xff); // hsp_hsize_in1
    MDrv_WriteByteMask( REG_FSC_BK23_36, 0x80, 0xff); // hsp_hsize_out0
    MDrv_WriteByteMask( REG_FSC_BK23_37, 0x07, 0xff); // hsp_hsize_out1
    MDrv_WriteByteMask( REG_FSC_BK23_38, 0x80, 0xff); // gb_in_size
    MDrv_WriteByteMask( REG_FSC_BK23_39, 0x00, 0x0f); // gb_in_size
    MDrv_WriteByteMask( REG_FSC_BK23_39, 0x10, 0x10); // gb_in_en
    MDrv_WriteByteMask( REG_FSC_BK23_3A, 0x80, 0xff); // gb_out_size
    MDrv_WriteByteMask( REG_FSC_BK23_3B, 0x00, 0x0f); // gb_out_size
    MDrv_WriteByteMask( REG_FSC_BK23_3B, 0x10, 0x10); // gb_out_en
    MDrv_WriteByteMask( REG_FSC_BK23_3E, 0x04, 0x04); // hsize_half_en
    MDrv_WriteByteMask( REG_FSC_BK23_3C, 0x00, 0xff); // hsp_mask_size
    MDrv_WriteByteMask( REG_FSC_BK23_3D, 0x00, 0x01); // hsp_mask_size
// VIP
    MDrv_WriteByteMask( REG_FSC_BK1B_1C, 0x00, 0xFF); // reg_vip_horizontal_num_lsb
    MDrv_WriteByteMask( REG_FSC_BK1B_1D, 0x08, 0x0F); // reg_vip_horizontal_num_msb
    MDrv_WriteByteMask( REG_FSC_BK1B_1E, 0x1C, 0xFF); // reg_vip_vertical_num_lsb
    MDrv_WriteByteMask( REG_FSC_BK1B_1F, 0x02, 0x1F); // reg_vip_vertical_num_msb
    MDrv_WriteByteMask( REG_FSC_BK1B_1F, 0x00, 0x80); // reg_vip_tb_3d_en
// VIP_444to422
    MDrv_WriteByteMask( REG_FSC_BK1B_10, 0x41, 0x01); // reg_main_444to422_en
    MDrv_WriteByteMask( REG_FSC_BK1B_10, 0x00, 0x02); // reg_main_h_mirror_en
    MDrv_WriteByteMask( REG_FSC_BK1B_10, 0x41, 0x40); // reg_main_444to422_filter
    MDrv_WriteByteMask( REG_FSC_BK1B_11, 0x41, 0x01); // reg_sub_444to422_en
    MDrv_WriteByteMask( REG_FSC_BK1B_11, 0x00, 0x02); // reg_sub_h_mirror_en
    MDrv_WriteByteMask( REG_FSC_BK1B_11, 0x41, 0x40); // reg_sub_444to422_en
// SPTF_D2LR
    MDrv_WriteByteMask( REG_FRC_BK33A_60  , 0x00, 0xff); // gb_cut_st_l
    MDrv_WriteByteMask( REG_FRC_BK33A_61  , 0x00, 0xff); // gb_cut_st_l
    MDrv_WriteByteMask( REG_FRC_BK33A_62  , 0x80, 0xff); // gb_cut_end_l
    MDrv_WriteByteMask( REG_FRC_BK33A_63  , 0x07, 0xff); // gb_cut_end_l
    MDrv_WriteByteMask( REG_FRC_BK33A_64  , 0x80, 0xff); // gb_cut_st_r
    MDrv_WriteByteMask( REG_FRC_BK33A_65  , 0x00, 0xff); // gb_cut_st_r
    MDrv_WriteByteMask( REG_FRC_BK33A_66  , 0xff, 0xff); // gb_cut_end_r
    MDrv_WriteByteMask( REG_FRC_BK33A_67  , 0x1f, 0xff); // gb_cut_end_r
    MDrv_WriteByteMask( REG_FRC_BK33A_50  , 0x00, 0x01); // d2lr_bypass
    MDrv_WriteByteMask( REG_FRC_BK33A_50  , 0x02, 0x02); // d2lr_eo
    MDrv_WriteByteMask( REG_FRC_BK33A_50  , 0x00, 0x04); // d2lr_lr
    MDrv_WriteByteMask( REG_FRC_BK33A_52  , 0x80, 0xff); // d2lr_output_h
    MDrv_WriteByteMask( REG_FRC_BK33A_53  , 0x08, 0xff); // d2lr_output_h
    MDrv_WriteByteMask( REG_FRC_BK33A_40  , 0x00, 0xff); // d2lr_w0_st
    MDrv_WriteByteMask( REG_FRC_BK33A_41  , 0x00, 0xff); // d2lr_w0_st
    MDrv_WriteByteMask( REG_FRC_BK33A_42  , 0xff, 0xff); // d2lr_w0_end
    MDrv_WriteByteMask( REG_FRC_BK33A_43  , 0x0e, 0xff); // d2lr_w0_end
    MDrv_WriteByteMask( REG_FRC_BK33A_44  , 0x00, 0xff); // d2lr_w1_st
    MDrv_WriteByteMask( REG_FRC_BK33A_45  , 0x00, 0xff); // d2lr_w1_st
    MDrv_WriteByteMask( REG_FRC_BK33A_46  , 0xff, 0xff); // d2lr_w1_end
    MDrv_WriteByteMask( REG_FRC_BK33A_47  , 0x0e, 0xff); // d2lr_w1_end
    MDrv_WriteByteMask( REG_FRC_BK33A_48  , 0x00, 0xff); // d2lr_r0_st
    MDrv_WriteByteMask( REG_FRC_BK33A_49  , 0x00, 0xff); // d2lr_r0_st
    MDrv_WriteByteMask( REG_FRC_BK33A_4A  , 0x7f, 0xff); // d2lr_r0_end
    MDrv_WriteByteMask( REG_FRC_BK33A_4B  , 0x08, 0xff); // d2lr_r0_end
    MDrv_WriteByteMask( REG_FRC_BK33A_4C  , 0x80, 0xff); // d2lr_r1_st
    MDrv_WriteByteMask( REG_FRC_BK33A_4D  , 0x06, 0xff); // d2lr_r1_st
    MDrv_WriteByteMask( REG_FRC_BK33A_4E  , 0xff, 0xff); // d2lr_r1_end
    MDrv_WriteByteMask( REG_FRC_BK33A_4F  , 0x0e, 0xff); // d2lr_r1_end
// FSC_FBL
    MDrv_WriteByteMask( REG_FSC_BK20_0E, 0x00, 0x01); // fbl_en
    MDrv_WriteByteMask( REG_FSC_BK20_0C, 0x00, 0x03); // reg_ipm_lr_en
    MDrv_WriteByteMask( REG_FSC_BK20_60, 0x00, 0x04); // 3d_sbs_en
    MDrv_WriteByteMask( REG_FSC_BK20_60, 0x00, 0x01); // 3d_top_bot_en
// IPM_OPM
    MDrv_WriteByteMask( REG_FRC_BK134_1C  , 0x80, 0xff); // ipm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK134_1D  , 0x07, 0xff); // ipm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK134_1E  , 0x80, 0xff); // ipm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK134_1F  , 0x07, 0xff); // ipm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK134_04  , 0x00, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FRC_BK134_05  , 0x01, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_04  , 0x03, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_05  , 0x31, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_84  , 0x03, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_85  , 0x31, 0xff); // ipm_mem_config_f2
    MDrv_WriteByteMask( REG_FRC_BK134_9C  , 0x80, 0xff); // ipm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK134_9D  , 0x07, 0xff); // ipm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK134_9E  , 0x80, 0xff); // ipm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK134_9F  , 0x07, 0xff); // ipm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK134_84  , 0x00, 0xff); // ipm_mem_config_f1
    MDrv_WriteByteMask( REG_FRC_BK134_85  , 0x01, 0xff); // ipm_mem_config_f1
    MDrv_WriteByteMask( REG_FRC_BK134_2C  , 0x80, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK134_2D  , 0x07, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK134_2E  , 0x80, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK134_2F  , 0x07, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK134_30  , 0x38, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK134_31  , 0x04, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK136_2C  , 0x80, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK136_2D  , 0x07, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK136_2E  , 0x80, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK136_2F  , 0x07, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK136_30  , 0x38, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK136_31  , 0x04, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_2C  , 0x80, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_2D  , 0x07, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_2E  , 0x80, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_2F  , 0x07, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_30  , 0x38, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_31  , 0x04, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13B_2C  , 0x80, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK13B_2D  , 0x07, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK13B_2E  , 0x80, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK13B_2F  , 0x07, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK13B_30  , 0x38, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13B_31  , 0x04, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13C_2C  , 0x80, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK13C_2D  , 0x07, 0xff); // opm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK13C_2E  , 0x80, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK13C_2F  , 0x07, 0xff); // opm_fetch_f2
    MDrv_WriteByteMask( REG_FRC_BK13C_30  , 0x38, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13C_31  , 0x04, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13D_2C  , 0x80, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK13D_2D  , 0x07, 0xff); // opm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK13D_2E  , 0x80, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK13D_2F  , 0x07, 0xff); // opm_fetch_f1
    MDrv_WriteByteMask( REG_FRC_BK13D_30  , 0x38, 0xff); // reg_ipm_vcnt_limit_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13D_31  , 0x04, 0xff); // reg_ipm_vcnt_limit_num_f2
// IPM_OPM_vlen
    MDrv_WriteByteMask( REG_FRC_BK134_CD  , 0x80, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FRC_BK136_CD  , 0x80, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FRC_BK13A_CD  , 0x80, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FRC_BK13B_CD  , 0x80, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FRC_BK13C_CD  , 0x80, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FRC_BK13D_CD  , 0x80, 0x80); // reg_opm_vlen_sel
    MDrv_WriteByteMask( REG_FRC_BK134_62  , 0x38, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK134_63  , 0x04, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK136_62  , 0x38, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK136_63  , 0x04, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13A_62  , 0x38, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13A_63  , 0x04, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13B_62  , 0x38, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13B_63  , 0x04, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13C_62  , 0x38, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13C_63  , 0x04, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13D_62  , 0x38, 0xff); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK13D_63  , 0x04, 0x1f); // reg_opm_vlen
    MDrv_WriteByteMask( REG_FRC_BK134_CC  , 0x70, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK134_CD  , 0x08, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK136_CC  , 0x70, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK136_CD  , 0x08, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13A_CC  , 0x70, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13A_CD  , 0x08, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13B_CC  , 0x70, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13B_CD  , 0x08, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13C_CC  , 0x70, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13C_CD  , 0x08, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13D_CC  , 0x70, 0xff); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13D_CD  , 0x08, 0x1f); // reg_opm_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK134_BA  , 0x1c, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK134_BB  , 0x02, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK136_BA  , 0x1c, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK136_BB  , 0x02, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13A_BA  , 0x1c, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13A_BB  , 0x02, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13B_BA  , 0x1c, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13B_BB  , 0x02, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13C_BA  , 0x1c, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13C_BB  , 0x02, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13D_BA  , 0x1c, 0xff); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK13D_BB  , 0x02, 0x1f); // reg_opm_meds_vlen
    MDrv_WriteByteMask( REG_FRC_BK134_C8  , 0x38, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK134_C9  , 0x04, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK136_C8  , 0x38, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK136_C9  , 0x04, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13A_C8  , 0x38, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13A_C9  , 0x04, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13B_C8  , 0x38, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13B_C9  , 0x04, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13C_C8  , 0x38, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13C_C9  , 0x04, 0x1f); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13D_C8  , 0x38, 0xff); // reg_opm_meds_vlen_new
    MDrv_WriteByteMask( REG_FRC_BK13D_C9  , 0x04, 0x1f); // reg_opm_meds_vlen_new
// IPM_OPM_DSmode
    MDrv_WriteByteMask( REG_FRC_BK13A_1C  , 0xc0, 0xff); // reg_ipm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_1D  , 0x03, 0xff); // reg_ipm_offset_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_1E  , 0xc0, 0xff); // reg_ipm_fetch_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_1F  , 0x03, 0xff); // reg_ipm_fetch_num_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_9C  , 0xc0, 0xff); // reg_ipm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_9D  , 0x03, 0xff); // reg_ipm_offset_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_9E  , 0xc0, 0xff); // reg_ipm_fetch_num_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_9F  , 0x03, 0xff); // reg_ipm_fetch_num_f1
    MDrv_WriteByteMask( REG_FRC_BK134_BC  , 0xc0, 0xff); // reg_opm_meds_offset
    MDrv_WriteByteMask( REG_FRC_BK134_BD  , 0x03, 0xff); // reg_opm_meds_offset
    MDrv_WriteByteMask( REG_FRC_BK134_BE  , 0xc0, 0xff); // reg_opm_meds_fetch_num
    MDrv_WriteByteMask( REG_FRC_BK134_BF  , 0x03, 0xff); // reg_opm_meds_fetch_num
// IPM_3D
    MDrv_WriteByteMask( REG_FRC_BK135_1C  , 0x38, 0xff); // reg_v_toggle_value
    MDrv_WriteByteMask( REG_FRC_BK135_1D  , 0x04, 0x0f); // reg_v_toggle_value
    MDrv_WriteByteMask( REG_FRC_BK135_04  , 0x20, 0x30); // reg_v_toggle_en
    MDrv_WriteByteMask( REG_FRC_BK134_27  , 0x80, 0x80); // reg_ipm_ud_en
    MDrv_WriteByteMask( REG_FRC_BK13A_27  , 0x80, 0x80); // reg_ipm_meds_ud_en
    MDrv_WriteByteMask( REG_FRC_BK134_26  , 0x1c, 0xff); // reg_ipm_turn_back_line
    MDrv_WriteByteMask( REG_FRC_BK134_27  , 0x02, 0x1f); // reg_ipm_turn_back_line
    MDrv_WriteByteMask( REG_FRC_BK13A_26  , 0x0e, 0xff); // reg_ipm_turn_back_line_meds
    MDrv_WriteByteMask( REG_FRC_BK13A_27  , 0x01, 0x1f); // reg_ipm_turn_back_line_meds
// OPM_3D
    MDrv_WriteByteMask( REG_FRC_BK134_60  , 0x01, 0x01); // reg_ipm_3d_en_f2
    MDrv_WriteByteMask( REG_FRC_BK134_E0  , 0x01, 0x01); // reg_ipm_3d_en_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_60  , 0x01, 0x01); // reg_ipm_3d_en_mef3f4_f2
    MDrv_WriteByteMask( REG_FRC_BK13A_E0  , 0x01, 0x01); // reg_ipm_3d_en_mef3f4_f1
    MDrv_WriteByteMask( REG_FRC_BK134_66  , 0x80, 0x80); // reg_opm_3d_en_f2
    MDrv_WriteByteMask( REG_FRC_BK136_66  , 0x80, 0x80); // reg_opm_3d_en_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_66  , 0x80, 0x80); // reg_opm_3d_en_mef3f4_f2
    MDrv_WriteByteMask( REG_FRC_BK13B_66  , 0x80, 0x80); // reg_opm_3d_en_mef3f4_f1
    MDrv_WriteByteMask( REG_FRC_BK13C_66  , 0x80, 0x80); // reg_opm_3d_en_mif3_f2
    MDrv_WriteByteMask( REG_FRC_BK13D_66  , 0x80, 0x80); // reg_opm_3d_en_mif3_f1
    MDrv_WriteByteMask( REG_FRC_BK134_67  , 0x00, 0x04); // reg_opm_passive_en_f2
    MDrv_WriteByteMask( REG_FRC_BK136_67  , 0x00, 0x04); // reg_opm_passive_en_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_67  , 0x00, 0x04); // reg_opm_passive_en_f2
    MDrv_WriteByteMask( REG_FRC_BK13B_67  , 0x00, 0x04); // reg_opm_passive_en_f1
    MDrv_WriteByteMask( REG_FRC_BK13C_67  , 0x00, 0x04); // reg_opm_passive_en_f2
    MDrv_WriteByteMask( REG_FRC_BK13D_67  , 0x00, 0x04); // reg_opm_passive_en_f1
    MDrv_WriteByteMask( REG_FRC_BK134_2B  , 0x00, 0x20); // reg_opm_line_repeat_en_f2
    MDrv_WriteByteMask( REG_FRC_BK136_2B  , 0x00, 0x20); // reg_opm_line_repeat_en_f1
    MDrv_WriteByteMask( REG_FRC_BK13A_2B  , 0x00, 0x20); // reg_opm_line_repeat_en_f2
    MDrv_WriteByteMask( REG_FRC_BK13B_2B  , 0x00, 0x20); // reg_opm_line_repeat_en_f1
    MDrv_WriteByteMask( REG_FRC_BK13C_2B  , 0x00, 0x20); // reg_opm_line_repeat_en_f2
    MDrv_WriteByteMask( REG_FRC_BK13D_2B  , 0x00, 0x20); // reg_opm_line_repeat_en_f1
// OPMRM
    MDrv_WriteByteMask( REG_FRC_BK134_E8  , 0x00, 0x01); // reg_opm_ml_en
    MDrv_WriteByteMask( REG_FRC_BK134_E9  , 0x00, 0x08); // reg_rbk_free
    MDrv_WriteByteMask( REG_FRC_BK134_E9  , 0x00, 0x70); // reg_rbk_free_diff
    MDrv_WriteByteMask( REG_FRC_BK134_32  , 0x00, 0x80); // reg_opm_2f_md
    MDrv_WriteByteMask( REG_FRC_BK136_32  , 0x00, 0x80); // reg_opm_2f_md
    MDrv_WriteByteMask( REG_FRC_BK134_33  , 0x01, 0x01); // reg_opm_3f_md
    MDrv_WriteByteMask( REG_FRC_BK136_33  , 0x01, 0x01); // reg_opm_3f_md
    MDrv_WriteByteMask( REG_FRC_BK134_CE  , 0x05, 0x0f); // reg_opm_memc_md
    MDrv_WriteByteMask( REG_FRC_BK134_CF  , 0x04, 0x07); // reg_opm_memc_md
// HSD_MEDS
    MDrv_WriteByteMask( REG_FRC_BK320_C2  , 0x33, 0xff); // reg_frc_ipm_hvsd_la_mode
    MDrv_WriteByteMask( REG_FRC_BK320_C3  , 0x8E, 0xff); // reg_frc_ipm_hvsd_drop_mode
// MEMC_FSC
    MDrv_WriteByteMask( REG_FRC_BK320_A4  , 0x00, 0xff); // reg_frc_xxxx
    MDrv_WriteByteMask( REG_FRC_BK320_A5  , 0x0F, 0xff); // reg_frc_xxxx
    MDrv_WriteByteMask( REG_FRC_BK320_A6  , 0x80, 0xff); // reg_frc_xxxx
    MDrv_WriteByteMask( REG_FRC_BK320_A7  , 0x07, 0xff); // reg_frc_xxxx
// MLB
    MDrv_WriteByteMask( REG_FRC_BK226_F4  , 0x01, 0xff); // reg_sadmvRangeL
    MDrv_WriteByteMask( REG_FRC_BK226_F5  , 0x00, 0x03); // reg_sadmvRangeL
    MDrv_WriteByteMask( REG_FRC_BK226_F8  , 0x01, 0xff); // reg_sadmvRangeU
    MDrv_WriteByteMask( REG_FRC_BK226_F9  , 0x00, 0x01); // reg_sadmvRangeU
    MDrv_WriteByteMask( REG_FRC_BK226_F6  , 0xff, 0xff); // reg_sadmvRangeR
    MDrv_WriteByteMask( REG_FRC_BK226_F7  , 0x03, 0x03); // reg_sadmvRangeR
    MDrv_WriteByteMask( REG_FRC_BK226_F2  , 0xff, 0xff); // reg_sadmvRangeD
    MDrv_WriteByteMask( REG_FRC_BK226_F3  , 0x00, 0x01); // reg_sadmvRangeD
    MDrv_WriteByteMask( REG_FRC_BK232_02  , 0x00, 0x02); // reg_pass3d_la
    MDrv_WriteByteMask( REG_FRC_BK232_11  , 0x00, 0xc0); // reg_mask_en
// MV_PREPROC
    MDrv_WriteByteMask( REG_FRC_BK22C_92  , 0x00, 0x03); // reg_mv_preprocess
// MFC_pipectrl
    MDrv_WriteByteMask( REG_FRC_BK233_3E  , 0x00, 0xff); // reg_ppctr_h_pixl_num_me
    MDrv_WriteByteMask( REG_FRC_BK233_3F  , 0x0f, 0x1f); // reg_ppctr_h_pixl_num_me
    MDrv_WriteByteMask( REG_FRC_BK233_40  , 0x1c, 0xff); // reg_ppctr_v_line_num_me
    MDrv_WriteByteMask( REG_FRC_BK233_41  , 0x02, 0x1f); // reg_ppctr_v_line_num_me
    MDrv_WriteByteMask( REG_FRC_BK233_46  , 0x00, 0xff); // reg_me_h_pixel_num_mi
    MDrv_WriteByteMask( REG_FRC_BK233_47  , 0x0f, 0x1f); // reg_me_h_pixel_num_mi
    MDrv_WriteByteMask( REG_FRC_BK233_48  , 0x38, 0xff); // reg_me_v_pixel_num_mi
    MDrv_WriteByteMask( REG_FRC_BK233_49  , 0x04, 0x1f); // reg_me_v_pixel_num_mi
    MDrv_WriteByteMask( REG_FRC_BK233_F8  , 0x00, 0xff); // reg_h_pixel_num_mlb
    MDrv_WriteByteMask( REG_FRC_BK233_F9  , 0x0f, 0x1f); // reg_h_pixel_num_mlb
    MDrv_WriteByteMask( REG_FRC_BK233_FA  , 0x38, 0xff); // reg_v_pixel_num_mlb
    MDrv_WriteByteMask( REG_FRC_BK233_FB  , 0x04, 0x1f); // reg_v_pixel_num_mlb
    MDrv_WriteByteMask( REG_FRC_BK233_3A  , 0x00, 0xff); // reg_time_gen_sw_h_width
    MDrv_WriteByteMask( REG_FRC_BK233_3B  , 0x0f, 0x1f); // reg_time_gen_sw_h_width
    MDrv_WriteByteMask( REG_FRC_BK233_3C  , 0x0d, 0xff); // reg_time_gen_sw_v_width
    MDrv_WriteByteMask( REG_FRC_BK233_3D  , 0x01, 0x1f); // reg_time_gen_sw_v_width
    MDrv_WriteByteMask( REG_FRC_BK233_42  , 0x80, 0xff); // reg_mlb_disp_pixel_latch
    MDrv_WriteByteMask( REG_FRC_BK233_43  , 0x07, 0x1f); // reg_mlb_disp_pixel_latch
    MDrv_WriteByteMask( REG_FRC_BK233_A8  , 0x80, 0xff); // reg_gmv_vertical_active_window_height
    MDrv_WriteByteMask( REG_FRC_BK233_A9  , 0x00, 0x01); // reg_gmv_vertical_active_window_height
// MFC_VDUP
    MDrv_WriteByteMask( REG_FRC_BK20A_04  , 0xC0, 0xFF); // reg_VDUP_width
    MDrv_WriteByteMask( REG_FRC_BK20A_05  , 0x03, 0xFF); // reg_VDUP_width
    MDrv_WriteByteMask( REG_FRC_BK20A_06  , 0x70, 0xFF); // reg_VDUP_height
    MDrv_WriteByteMask( REG_FRC_BK20A_07  , 0x08, 0xFF); // reg_VDUP_height
// MFC
    MDrv_WriteByteMask( REG_FRC_BK226_61  , 0x73, 0xff); // reg_MFC_enable
    MDrv_WriteByteMask( REG_FRC_BK226_70  , 0x20, 0x20); // 
    MDrv_WriteByteMask( REG_FRC_BK226_73  , 0x0f, 0x1f); // reg_h_pix_num_3D
    MDrv_WriteByteMask( REG_FRC_BK226_72  , 0x00, 0xff); // reg_h_pix_num_3D
    MDrv_WriteByteMask( REG_FRC_BK226_75  , 0x02, 0x1f); // reg_v_lin_num_3D
    MDrv_WriteByteMask( REG_FRC_BK226_74  , 0x1c, 0xff); // reg_v_lin_num_3D
    MDrv_WriteByteMask( REG_FRC_BK22C_80  , 0x08, 0x08); // 
    MDrv_WriteByteMask( REG_FRC_BK229_05  , 0x0f, 0xFF); // reg_h_pix_num_ME
    MDrv_WriteByteMask( REG_FRC_BK229_04  , 0x00, 0xFF); // reg_h_pix_num_ME
    MDrv_WriteByteMask( REG_FRC_BK229_07  , 0x04, 0x0F); // reg_v_lin_num_ME
    MDrv_WriteByteMask( REG_FRC_BK229_06  , 0x40, 0xFF); // reg_v_lin_num_ME
    MDrv_WriteByteMask( REG_FRC_BK229_91  , 0x0f, 0xFF); // reg_h_pix_num_MI
    MDrv_WriteByteMask( REG_FRC_BK229_90  , 0x00, 0xFF); // reg_h_pix_num_MI
    MDrv_WriteByteMask( REG_FRC_BK229_93  , 0x04, 0xFF); // reg_v_lin_num_MI
    MDrv_WriteByteMask( REG_FRC_BK229_92  , 0x38, 0xFF); // reg_v_lin_num_MI
    MDrv_WriteByteMask( REG_FRC_BK226_67  , 0x01, 0x01); // reg_422to444_en
    MDrv_WriteByteMask( REG_FRC_BK232_D0  , 0x00, 0x02); // reg_rgb_bypass
    MDrv_WriteByteMask( REG_FRC_BK226_60  , 0x80, 0x80); // reg_c_drop
    MDrv_WriteByteMask( REG_FRC_BK229_9C  , 0x80, 0x80); // reg_422_avgmode
    MDrv_WriteByteMask( REG_FRC_BK22C_80  , 0x01, 0x01); // reg_pipectrl_bypass
    MDrv_WriteByteMask( REG_FRC_BK233_31  , 0x00, 0x10); // reg_mlb_mi_half_sr
    MDrv_WriteByteMask( REG_FRC_BK233_31  , 0x00, 0x20); // reg_mlb_me_half_sr
// MFC_Halo
    MDrv_WriteByteMask( REG_FRC_BK22C_93  , 0x00, 0x80); // reg_halo_buf_frame_end_en
    MDrv_WriteByteMask( REG_FRC_BK22C_A7  , 0x02, 0x02); // reg_halo_buf23_force_ready
// MFC_GMV
    MDrv_WriteByteMask( REG_FRC_BK232_2A  , 0x01, 0x01); // reg_gmv_in_ud_mode
// SNR
    MDrv_WriteByteMask( REG_FRC_BK2E_E0  , 0x00, 0x08); // reg_snr_bypass_en
    MDrv_WriteByteMask( REG_FRC_BK2E_E0  , 0x04, 0x04); // reg_snr_la_en
    MDrv_WriteByteMask( REG_FRC_BK2E_E2  , 0x00, 0xff); // reg_snr_pix_num_LSB
    MDrv_WriteByteMask( REG_FRC_BK2E_E3  , 0x0f, 0x1f); // reg_snr_pix_num_MSB
    MDrv_WriteByteMask( REG_FRC_BK2E_E8  , 0x38, 0xff); // reg_snr_line_num_LSB
    MDrv_WriteByteMask( REG_FRC_BK2E_E9  , 0x04, 0x0f); // reg_snr_line_num_MSB
// SNR_3D
    MDrv_WriteByteMask( REG_FRC_BK2E_65  , 0x00, 0x20); // reg_line_switch
// SNR_VSU2X
    MDrv_WriteByteMask( REG_FRC_BK2E_E0  , 0x10, 0x10); // reg_lbi_vsu2x_en
// T3D
    MDrv_WriteByteMask( REG_SC_BKC6_C0, 0x31, 0xff); // reg_pipe_guard_cycle_pb
    MDrv_WriteByteMask( REG_SC_BKC6_C2, 0x00, 0xff); // reg_ln_width
    MDrv_WriteByteMask( REG_SC_BKC6_C3, 0x0f, 0x3f); // reg_ln_width
    MDrv_WriteByteMask( REG_SC_BKC6_C4, 0x70, 0xff); // reg_col_height
    MDrv_WriteByteMask( REG_SC_BKC6_C5, 0x08, 0x1f); // reg_col_height
    MDrv_WriteByteMask( REG_SC_BKC6_C5, 0x00, 0x60); // reg_lb_mode_control
    MDrv_WriteByteMask( REG_SC_BKC6_C6, 0x01, 0x01); // reg_srclb_en
    MDrv_WriteByteMask( REG_SC_BKC6_C6, 0x02, 0x02); // reg_depthlb_en
    MDrv_WriteByteMask( REG_SC_BKC6_D0, 0x00, 0x01); // reg_render_control
    MDrv_WriteByteMask( REG_SC_BKC6_D0, 0x00, 0x02); // reg_lr_control_l
    MDrv_WriteByteMask( REG_SC_BKC6_D0, 0x04, 0x04); // reg_lr_control_r
    MDrv_WriteByteMask( REG_SC_BKC6_10, 0x10, 0x10); // reg_mtv_bypass_en
    MDrv_WriteByteMask( REG_SC_BKC6_C5, 0x00, 0x40); // reg_lineswap
    MDrv_WriteByteMask( REG_SC_BKC7_02, 0x00, 0xff); // reg_hsu_size_1
    MDrv_WriteByteMask( REG_SC_BKC7_03, 0x0f, 0x0f); // reg_hsu_size_1
    MDrv_WriteByteMask( REG_SC_BKC7_12, 0x00, 0xff); // reg_hsu_size_2
    MDrv_WriteByteMask( REG_SC_BKC7_13, 0x0f, 0x0f); // reg_hsu_size_2
// FO_HSU
    MDrv_WriteByteMask( REG_FRC_BK115_30  , 0x00, 0xff); // hfac_smd0
    MDrv_WriteByteMask( REG_FRC_BK115_31  , 0x00, 0xff); // hfac_smd1
    MDrv_WriteByteMask( REG_FRC_BK115_32  , 0x00, 0x3f); // hfac_smd2
    MDrv_WriteByteMask( REG_FRC_BK115_33  , 0x01, 0x01); // hsp_bypass_en
    MDrv_WriteByteMask( REG_FRC_BK115_33  , 0x00, 0x02); // hsp_bypass2_en
    MDrv_WriteByteMask( REG_FRC_BK115_0C  , 0x00, 0xff); // hsp_size_in0
    MDrv_WriteByteMask( REG_FRC_BK115_0D  , 0x0f, 0x1f); // hsp_size_in1
    MDrv_WriteByteMask( REG_FRC_BK115_0E  , 0x00, 0xff); // hsp_size_out0
    MDrv_WriteByteMask( REG_FRC_BK115_0F  , 0x0f, 0x1f); // hsp_size_out1
// SPTP
    MDrv_WriteByteMask( REG_FRC_BK3E_80  , 0xff, 0xff); // reg_sptp_mfc_dc_m1
    MDrv_WriteByteMask( REG_FRC_BK3E_81  , 0x0e, 0x0f); // reg_sptp_mfc_dc_m1
    MDrv_WriteByteMask( REG_FRC_BK3E_82  , 0x00, 0x80); // reg_sptp_fbl_en
    MDrv_WriteByteMask( REG_FRC_BK3E_84  , 0x01, 0x01); // reg_sptp_usr_en
    MDrv_WriteByteMask( REG_FRC_BK3E_88  , 0x00, 0xff); // reg_sptp_f0_st
    MDrv_WriteByteMask( REG_FRC_BK3E_89  , 0x00, 0x01); // reg_sptp_f0_st
    MDrv_WriteByteMask( REG_FRC_BK3E_8A  , 0xf1, 0xff); // reg_sptp_f0_end
    MDrv_WriteByteMask( REG_FRC_BK3E_8B  , 0x00, 0x01); // reg_sptp_f0_end
    MDrv_WriteByteMask( REG_FRC_BK3E_8C  , 0xee, 0xff); // reg_sptp_f1_st
    MDrv_WriteByteMask( REG_FRC_BK3E_8D  , 0x00, 0x01); // reg_sptp_f1_st
    MDrv_WriteByteMask( REG_FRC_BK3E_8E  , 0xdf, 0xff); // reg_sptp_f1_end
    MDrv_WriteByteMask( REG_FRC_BK3E_8F  , 0x01, 0x01); // reg_sptp_f1_end
    MDrv_WriteByteMask( REG_FRC_BK3E_86  , 0x02, 0xff); // reg_sptp_gb_en
// CSC
}



/********************************************/
