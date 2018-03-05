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

/*!
 **************************************************************************************
 * \file
 *    parsetcommon.h
 * \brief
 *    Picture and Sequence Parameter Sets, structures common to encoder and decoder
 *
 * \date 25 November 2002
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *      - Stephan Wenger        <stewe@cs.tu-berlin.de>
 ***************************************************************************************
 */


// In the JVT syntax, frequently flags are used that indicate the presence of
// certain pieces of information in the NALU.  Here, these flags are also
// present.  In the encoder, those bits indicate that the values signaled to
// be present are meaningful and that this part of the syntax should be
// written to the NALU.  In the decoder, the flag indicates that information
// was received from the decoded NALU and should be used henceforth.
// The structure names were chosen as indicated in the JVT syntax

#ifndef _PARSETCOMMON_H_
#define _PARSETCOMMON_H_

#define MAXIMUMPARSETRBSPSIZE   1500
#define MAXIMUMPARSETNALUSIZE   1500

#define MAXIMUMVALUEOFcpb_cnt   32
typedef struct
{
	unsigned int cpb_cnt_minus1;                                   // ue(v)
	unsigned int bit_rate_scale;                                   // u(4)
	unsigned int cpb_size_scale;                                   // u(4)
    unsigned int bit_rate_value_minus1 [MAXIMUMVALUEOFcpb_cnt];  // ue(v)
    unsigned int cpb_size_value_minus1 [MAXIMUMVALUEOFcpb_cnt];  // ue(v)
    unsigned int cbr_flag              [MAXIMUMVALUEOFcpb_cnt];  // u(1)
	unsigned int initial_cpb_removal_delay_length_minus1;          // u(5)
	unsigned int cpb_removal_delay_length_minus1;                  // u(5)
	unsigned int dpb_output_delay_length_minus1;                   // u(5)
	unsigned int time_offset_length;                               // u(5)
} hrd_parameters_t;


typedef struct
{
	MFE_BOOL      aspect_ratio_info_present_flag;                   // u(1)
    unsigned int aspect_ratio_idc;                               // u(8)
	unsigned int sar_width;                                    // u(16)
	unsigned int sar_height;                                   // u(16)
	MFE_BOOL      overscan_info_present_flag;                       // u(1)
    MFE_BOOL      overscan_appropriate_flag;                      // u(1)
	MFE_BOOL      video_signal_type_present_flag;                   // u(1)
    unsigned int video_format;                                   // u(3)
    MFE_BOOL      video_full_range_flag;                          // u(1)
    MFE_BOOL      colour_description_present_flag;                // u(1)
	unsigned int colour_primaries;                             // u(8)
	unsigned int transfer_characteristics;                     // u(8)
	unsigned int matrix_coefficients;                          // u(8)
	MFE_BOOL      chroma_location_info_present_flag;                // u(1)
    unsigned int  chroma_sample_loc_type_top_field;               // ue(v)
    unsigned int  chroma_sample_loc_type_bottom_field;            // ue(v)
	MFE_BOOL      timing_info_present_flag;                         // u(1)
    unsigned int num_units_in_tick;                              // u(32)
    unsigned int time_scale;                                     // u(32)
    MFE_BOOL      fixed_frame_rate_flag;                          // u(1)
	MFE_BOOL      nal_hrd_parameters_present_flag;                  // u(1)
    hrd_parameters_t nal_hrd_parameters;                      // hrd_paramters_t
	MFE_BOOL      vcl_hrd_parameters_present_flag;                  // u(1)
    hrd_parameters_t vcl_hrd_parameters;                      // hrd_paramters_t
	// if ((nal_hrd_parameters_present_flag || (vcl_hrd_parameters_present_flag))
    MFE_BOOL      low_delay_hrd_flag;                             // u(1)
	MFE_BOOL      pic_struct_present_flag;                        // u(1)
	MFE_BOOL      bitstream_restriction_flag;                       // u(1)
    MFE_BOOL      motion_vectors_over_pic_boundaries_flag;        // u(1)
    unsigned int max_bytes_per_pic_denom;                        // ue(v)
    unsigned int max_bits_per_mb_denom;                          // ue(v)
    unsigned int log2_max_mv_length_vertical;                    // ue(v)
    unsigned int log2_max_mv_length_horizontal;                  // ue(v)
    unsigned int num_reorder_frames;                             // ue(v)
    unsigned int max_dec_frame_buffering;                        // ue(v)
} vui_seq_parameters_t;


#define MAXnum_slice_groups_minus1  8
typedef struct
{
	MFE_BOOL   Valid;                  // indicates the parameter set is valid
	unsigned int pic_parameter_set_id;                             // ue(v)
	unsigned int seq_parameter_set_id;                             // ue(v)
	MFE_BOOL   entropy_coding_mode_flag;                         // u(1)
	
	MFE_BOOL   transform_8x8_mode_flag;                          // u(1)
	
	MFE_BOOL   pic_scaling_matrix_present_flag;                  // u(1)
	int       pic_scaling_list_present_flag[12];                 // u(1)
	
	// if( pic_order_cnt_type < 2 )  in the sequence parameter set
	MFE_BOOL      pic_order_present_flag;                           // u(1)
	unsigned int num_slice_groups_minus1;                          // ue(v)
    unsigned int slice_group_map_type;                        // ue(v)
    // if( slice_group_map_type = = 0 )
	unsigned int run_length_minus1[MAXnum_slice_groups_minus1]; // ue(v)
    // else if( slice_group_map_type = = 2 )
	unsigned int top_left[MAXnum_slice_groups_minus1];         // ue(v)
	unsigned int bottom_right[MAXnum_slice_groups_minus1];     // ue(v)
    // else if( slice_group_map_type = = 3 || 4 || 5
	MFE_BOOL   slice_group_change_direction_flag;            // u(1)
	unsigned int slice_group_change_rate_minus1;               // ue(v)
    // else if( slice_group_map_type = = 6 )
	unsigned int pic_size_in_map_units_minus1;                 // ue(v)
	MFE_BYTE      *slice_group_id;                              // complete MBAmap u(v)
	
	int num_ref_idx_l0_active_minus1;                     // ue(v)
	int num_ref_idx_l1_active_minus1;                     // ue(v)
	MFE_BOOL   weighted_pred_flag;                               // u(1)
	unsigned int  weighted_bipred_idc;                              // u(2)
	int       pic_init_qp_minus26;                              // se(v)
	int       pic_init_qs_minus26;                              // se(v)
	int       chroma_qp_index_offset;                           // se(v)
	
	int       cb_qp_index_offset;                               // se(v)
	int       cr_qp_index_offset;                               // se(v)
	
	MFE_BOOL   deblocking_filter_control_present_flag;           // u(1)
	MFE_BOOL   constrained_intra_pred_flag;                      // u(1)
	MFE_BOOL   redundant_pic_cnt_present_flag;                   // u(1)
	MFE_BOOL   vui_pic_parameters_flag;                          // u(1)
} pic_parameter_set_rbsp_t;


#define MAXnum_ref_frames_in_pic_order_cnt_cycle  256
typedef struct
{
	MFE_BOOL   Valid;                  // indicates the parameter set is valid
	
	unsigned int profile_idc;                                      // u(8)
	MFE_BOOL   constrained_set0_flag;                            // u(1)
	MFE_BOOL   constrained_set1_flag;                            // u(1)
	MFE_BOOL   constrained_set2_flag;                            // u(1)
	MFE_BOOL   constrained_set3_flag;                            // u(1)
	unsigned  int level_idc;                                        // u(8)
	unsigned  int seq_parameter_set_id;                             // ue(v)
	unsigned  int chroma_format_idc;                                // ue(v)
	
	unsigned int bit_depth_luma_minus8;                            // ue(v)
	unsigned int bit_depth_chroma_minus8;                          // ue(v)
	unsigned int log2_max_frame_num_minus4;                        // ue(v)
	unsigned int pic_order_cnt_type;
	// if( pic_order_cnt_type == 0 )
	unsigned int log2_max_pic_order_cnt_lsb_minus4;                 // ue(v)
	// else if( pic_order_cnt_type == 1 )
      MFE_BOOL delta_pic_order_always_zero_flag;               // u(1)
      int     offset_for_non_ref_pic;                         // se(v)
      int     offset_for_top_to_bottom_field;                 // se(v)
      unsigned int num_ref_frames_in_pic_order_cnt_cycle;          // ue(v)
    // for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
	int   offset_for_ref_frame[MAXnum_ref_frames_in_pic_order_cnt_cycle];   // se(v)
	unsigned int num_ref_frames;                                   // ue(v)
	MFE_BOOL   gaps_in_frame_num_value_allowed_flag;             // u(1)
	unsigned int pic_width_in_mbs_minus1;                          // ue(v)
	unsigned int pic_height_in_map_units_minus1;                   // ue(v)
	MFE_BOOL   frame_mbs_only_flag;                              // u(1)
	// if( !frame_mbs_only_flag )
      MFE_BOOL   mb_adaptive_frame_field_flag;                   // u(1)
	MFE_BOOL   direct_8x8_inference_flag;                        // u(1)
	MFE_BOOL   frame_cropping_flag;                              // u(1)
      unsigned int frame_cropping_rect_left_offset;                // ue(v)
      unsigned int frame_cropping_rect_right_offset;               // ue(v)
      unsigned int frame_cropping_rect_top_offset;                 // ue(v)
      unsigned int frame_cropping_rect_bottom_offset;              // ue(v)
	MFE_BOOL   vui_parameters_present_flag;                      // u(1)
      vui_seq_parameters_t vui_seq_parameters;                  // vui_seq_parameters_t
      unsigned  separate_colour_plane_flag;                       // u(1)
} seq_parameter_set_rbsp_t;

#endif
