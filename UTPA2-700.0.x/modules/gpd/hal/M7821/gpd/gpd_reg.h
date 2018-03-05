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
#ifndef _GPD_REG_H_
#define _GPD_REG_H_

#include "gpd.h"

#define GPD_SCALING_SUPPORT

typedef enum {
reg_gif_go_le,
reg_act_chk_le,
reg_gpd_bsaddr_go_le,
reg_gpd_rst_le,
reg_png_go_le,
reg_png_blk_go_le,
reg_gif_ltbl_size      ,
reg_gif_local_tbl      ,
reg_gif_done           ,
reg_ofifo_abort        ,
reg_ififo_full         ,
reg_ififo_empty        ,
reg_bsadr_full         ,
reg_ififo_diff         ,
reg_bitpos             ,
reg_png_dtbl_size      ,
reg_gif_state          ,
reg_gpd_pitch          ,
reg_gpd_iwidth         ,
reg_gpd_iheight        ,
reg_gpd_bstart         ,
reg_gpd_bend           ,
reg_gpd_istart         ,
reg_spare1             ,
reg_gpd_boffset        ,
reg_iofifo_state       ,
reg_gpd_roi_hstart     ,
reg_gpd_roi_vstart     ,
reg_gpd_roi_width      ,
reg_gpd_roi_height     ,
reg_gpd_roi_en         ,
reg_gpd_default_alpha  ,
reg_spare2_rst0        ,
reg_png_trans_r        ,
reg_png_trans_g        ,
reg_png_blk_done       ,
reg_png_mincode1       ,
reg_png_mincode2       ,
reg_png_mincode3       ,
reg_png_mincode4       ,
reg_png_mincode5       ,
reg_png_mincode6       ,
reg_png_mincode7       ,
reg_gpd_ipm_en         ,
reg_gpd_ipm_size       ,
reg_gpd_ocolor         ,
reg_gpd_en,
reg_ofifo_done         ,
reg_miu_domain_empty   ,
reg_mreq_always_active ,
reg_eng_always_active  ,
reg_png_ltbl_size      ,
reg_gpd_act_chk        ,
reg_png_sca            ,
reg_png_eob            ,
reg_png_mincode8       ,
reg_png_mincode9       ,
reg_png_mincode10      ,
reg_png_mincode11      ,
reg_png_mincode12      ,
reg_png_mincode13      ,
reg_png_mincode14      ,
reg_png_mincode15      ,
reg_png2_mincode1      ,
reg_png2_mincode2      ,
reg_png2_mincode3      ,
reg_png2_mincode4      ,
reg_png2_mincode5      ,
reg_png2_mincode6      ,
reg_png2_mincode7      ,
reg_png2_mincode8      ,
reg_png2_mincode9      ,
reg_png2_mincode10     ,
reg_png2_mincode11     ,
reg_png2_mincode12     ,
reg_png2_mincode13     ,
reg_png2_mincode14     ,
reg_png2_mincode15     ,
reg_png_lbase2         ,
reg_png_lbase3         ,
reg_png_lbase4         ,
reg_png_lbase5         ,
reg_png_lbase6         ,
reg_png_lbase7         ,
reg_png_lbase8         ,
reg_png_lbase9         ,
reg_png_lbase10        ,
reg_png_lbase11        ,
reg_png_lbase12        ,
reg_png_lbase13        ,
reg_png_lbase14        ,
reg_png_lbase15        ,
reg_png_dbase2         ,
reg_png_dbase3         ,
reg_png_dbase4         ,
reg_png_dbase5         ,
reg_png_dbase6         ,
reg_png_dbase7         ,
reg_png_dbase8         ,
reg_png_dbase9         ,
reg_png_dbase10        ,
reg_png_dbase11        ,
reg_png_dbase12        ,
reg_png_dbase13        ,
reg_png_dbase14        ,
reg_png_dbase15        ,
reg_png_scline0_width  ,
reg_png_scline1_width  ,
reg_png_scline2_width  ,
reg_png_scline3_width  ,
reg_png_scline4_width  ,
reg_png_scline5_width  ,
reg_png_scline6_width  ,
reg_png_scline0_height ,
reg_png_scline1_height ,
reg_png_scline2_height ,
reg_png_scline3_height ,
reg_png_scline4_height ,
reg_png_scline5_height ,
reg_png_scline6_height ,
reg_png_mincode_valid  ,
reg_png2_mincode_valid ,
reg_gpd_only_decom_en  ,
reg_png_done           ,
reg_png_color_type     ,
reg_gpd_interlace      ,
reg_spare6             ,
reg_png_compress_type  ,
reg_png_color_depth    ,
reg_miu_act_chk        ,
reg_png_trans_en       ,
reg_png_trans_b        ,
reg_png_state          ,
reg_ififo_cnt          ,
reg_hipri              ,
reg_gpd_premult_alpha_en,
reg_gpd_gif_alpha_mask_en,
reg_frun_cnt           ,
reg_png_burst_en       ,
reg_gif_mask           ,
reg_deflt_fast_on      ,
reg_ififo_radr         ,
reg_gpd_time_out       ,
reg_gif_code_size_err  ,
reg_gif_err            ,
reg_gpd_pgend          ,
reg_miu64              ,
reg_gpd2mi_adr         ,
reg_wait_last_done     ,
reg_miu_wait_cyc       ,
reg_fixed_pri          ,
reg_last_done_md       ,
reg_gpd_read_data      ,
reg_io_read_gpd        ,
reg_cbuf_bas           ,
reg_zbuf_bas           ,
reg_bist_fail_lz_psram ,
reg_bist_fail_bst_psram,
reg_bist_fail_lit_psram,
reg_bist_fail_flt_psram,
reg_bist_fail_lmem     ,
reg_bist_fail_cmem     ,
reg_bist_fail_omem     ,
reg_bist_fail_imem     ,
reg_bist_fail_dc0_cmem ,
reg_bist_fail_dc1_cmem ,
reg_bist_fail_stk_psram,
reg_bist_fail_dma1     ,
reg_bist_fail_dma0     ,
reg_bist_fail_cmd      ,
reg_bist_fail_data     ,
reg_bist_fail_cc_cmem  ,
reg_bist_fail_cc_dmem  ,
reg_bist_fail_cc_rdmem ,
reg_bist_fail_cc_wdmem ,
reg_bist_fail_zc_cmem  ,
reg_bist_fail_zc_dmem  ,
reg_bist_fail_zc_rdmem ,
reg_bist_fail_zc_wdmem ,
reg_int_mask           ,
reg_int_rst            ,
reg_debug_mux          ,
reg_dram_imi           ,
reg_gpd_istr_8b        ,
reg_gif_act_clr        ,
reg_int_status         ,
reg_int_sw_force       ,
reg_cache_hit_cmp      ,
reg_scale_en           ,
reg_scale_md           ,
reg_lb_addr            ,
reg_ub_addr            ,
reg_gpd_xiu_byte_sel   ,
reg_gpd_read_bits      ,
reg_gpd_reserved1      ,
reg_gpd_cmem_wdata,
reg_png_ltbl_wdata,
reg_png_dtbl_wdata,
reg_debug2_mux         ,
reg_wbe_bypass_go_chk,
reg_gpd_sram_sd_en,
reg_gpd_debug          ,
reg_gpd_debug2,
reg_gpd_version,
reg_gpd_tlb
} gpd_reg_index;


#define reg_gif_go          reg_gif_go_le
#define reg_act_chk         reg_act_chk_le
#define reg_gpd_bsaddr_go   reg_gpd_bsaddr_go_le
#define reg_gpd_rst         reg_gpd_rst_le
#define reg_png_go          reg_png_go_le
#define reg_png_blk_go      reg_png_blk_go_le
#define reg_png_en          reg_gpd_en
#define reg_gpd_cmem_wd     reg_gpd_cmem_wdata
#define reg_png_ltbl_wd     reg_png_ltbl_wdata
#define reg_png_dtbl_wd     reg_png_dtbl_wdata


MS_U32 GPD_GET_MS_U32REG_dbg(MS_U32 index);
MS_U32 GPD_GET_MS_U32REG(MS_U32 index);
void GPD_SET_MS_U32REG(MS_U32 index, MS_U32 value);
MS_U32 hal_gpd_get_clkbase(void);
MS_U32 hal_gpd_get_clkoffset(void);
void hal_gpd_reg_base(MS_U32* u32RIUBase, MS_U32* u32XIUBase);
void hal_gpd_miu_client(MS_U8* u8Offset, MS_U16* u16BitMask);
void hal_gpd_init_outside_reg(MS_VIRT BankBaseAddr);
void hal_gpd_power_on(MS_VIRT BankBaseAddr);
void hal_gpd_power_off(MS_VIRT BankBaseAddr);
void hal_gpd_init_chip_specific_reg(void);
void hal_gpd_SetMIUProtectMask(MS_VIRT BankBaseAddr, MS_U8 bEnable);


#endif
