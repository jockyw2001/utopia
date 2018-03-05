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
 *    parset.c
 * \brief
 *    Picture and Sequence Parameter set generation and handling
 *  \date 25 November 2002
 * \author
 *    Main contributors (see contributors.h for copyright, address and affiliation details)
 *      - Stephan Wenger        <stewe@cs.tu-berlin.de>
 *
 **************************************************************************************
 */


#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"

#include "parsetcommon.h"
#include "parset.h"
#include "nalu.h"
#include "vlc.h"

seq_parameter_set_rbsp_t gSPS;
pic_parameter_set_rbsp_t gPPS;
seq_parameter_set_rbsp_t *active_sps;
pic_parameter_set_rbsp_t *active_pps;

// Static SPS, PPS NALU's
#define STATIC_SPS 0
#define STATIC_PPS 1
MFE_BYTE gNaluBuf[2][MAXRBSPSIZE];
NALU_t gNaluStruct[2];

// Local helpers
static void _GenerateParameterSets(MFE_CONFIG* pConfig);
//static void _FreeParameterSets (MFE_CONFIG* pConfig);

static NALU_t* _GenerateSeq_parameter_set_NALU (void);
static NALU_t* _GeneratePic_parameter_set_NALU(MFE_CONFIG *pConfig);

static int _GenerateVUI_parameters_rbsp(seq_parameter_set_rbsp_t *sps, Bitstream *bitstream);
static int _GenerateSeq_parameter_set_rbsp (seq_parameter_set_rbsp_t *sps, MFE_BYTE *rbsp);
static int _GeneratePic_parameter_set_rbsp (MFE_CONFIG* pConfig, pic_parameter_set_rbsp_t *pps, MFE_BYTE *rbsp);
static void _GenerateVUIParameters(MFE_CONFIG *pConfig, seq_parameter_set_rbsp_t *sps);

/*!
*************************************************************************************
* \brief
*    Prepare SPS and PPS NALU's for later usage.
*
* \return
*    A NALU containing the Sequence ParameterSet
*
*************************************************************************************
*/
void GenerateSPSPPS(MFE_CONFIG* pConfig)
{
	_GenerateParameterSets(pConfig);
	_GenerateSeq_parameter_set_NALU();
	_GeneratePic_parameter_set_NALU(pConfig);
}

/*!
 *************************************************************************************
 * \brief
 *    generates a sequence and picture parameter set and stores these in global
 *    active_sps and active_pps
 *
 * \return
 *    A NALU containing the Sequence ParameterSet
 *
 *************************************************************************************
*/
static void _GenerateParameterSets(MFE_CONFIG* pConfig)
{
    active_sps = &gSPS;
    active_pps = &gPPS;

    GenerateSequenceParameterSet(pConfig, active_sps, 0);
    GeneratePictureParameterSet (pConfig, active_pps, active_sps, 0);
}

/*!
*************************************************************************************
* \brief
*    frees global parameter sets active_sps and active_pps
*
* \return
*    A NALU containing the Sequence ParameterSet
*
*************************************************************************************
*/
/*
static void _FreeParameterSets (MFE_CONFIG* pConfig)
{
    active_pps = NULL;
    active_pps = NULL;
}
*/
/*!
*************************************************************************************
* \brief
*   NALU_t* _GenerateSeq_parameter_set_NALU (void);
*
* \note
*    Uses the global variables through GenerateSequenceParameterSet()
*    and GeneratePictureParameterSet
*
* \return
*    A NALU containing the Sequence ParameterSet
*
*************************************************************************************
*/

static NALU_t* _GenerateSeq_parameter_set_NALU (void)
{
    NALU_t *n = &gNaluStruct[STATIC_SPS];
    int RBSPlen = 0;
//    int NALUlen;
    MFE_BYTE rbsp[MAXRBSPSIZE];

    n->buf = gNaluBuf[STATIC_SPS];

    RBSPlen = _GenerateSeq_parameter_set_rbsp (active_sps, rbsp);
//    NALUlen = RBSPtoNALU (rbsp, n, RBSPlen, NALU_TYPE_SPS, NALU_PRIORITY_HIGHEST, 0, 1);
    RBSPtoNALU (rbsp, n, RBSPlen, NALU_TYPE_SPS, NALU_PRIORITY_HIGHEST, 0, 1);
/* RefSW has this originally, ken marked
  n->startcodeprefix_len = 4;
*/
    return n;
}


/*!
*************************************************************************************
* \brief
*    NALU_t* _GeneratePic_parameter_set_NALU (int PPS_id);
*
* \note
*    Uses the global variables through GenerateSequenceParameterSet()
*    and GeneratePictureParameterSet
*
* \return
*    A NALU containing the Picture Parameter Set
*
*************************************************************************************
*/

static NALU_t* _GeneratePic_parameter_set_NALU(MFE_CONFIG *pConfig)
{
    NALU_t *n = &gNaluStruct[STATIC_PPS];
    int RBSPlen = 0;
//    int NALUlen;
    MFE_BYTE rbsp[MAXRBSPSIZE];

    n->buf = gNaluBuf[STATIC_PPS];

    RBSPlen = _GeneratePic_parameter_set_rbsp (pConfig, active_pps, rbsp);
//    NALUlen = RBSPtoNALU (rbsp, n, RBSPlen, NALU_TYPE_PPS, NALU_PRIORITY_HIGHEST, 0, 1);
    RBSPtoNALU (rbsp, n, RBSPlen, NALU_TYPE_PPS, NALU_PRIORITY_HIGHEST, 0, 1);
//RefSW set n->startcodeprefix_len = 4, ken marked
//  n->startcodeprefix_len = 4;

    return n;
}


/*!
 ************************************************************************
 * \brief
 *    GenerateSequenceParameterSet: extracts info from global variables and
 *    generates sequence parameter set structure
 *
 * \par
 *    Function reads all kinds of values from several global variables,
 *    including input-> and image-> and fills in the sps.  Many
 *    values are current hard-coded to defaults.
 *
 ************************************************************************
 */

void GenerateSequenceParameterSet( MFE_CONFIG *pConfig,
								   seq_parameter_set_rbsp_t *sps, //!< Sequence Parameter Set to be filled
                                   int SPS_id                     //!< SPS ID
                                 )
{
    H264INFO *pInfo = &pConfig->ctxH264Info;

    // *************************************************************************
    // Sequence Parameter Set
    // *************************************************************************
    MFE_ASSERT (sps != NULL);
    // Profile and Level should be calculated using the info from the config
    // file.  Calculation is hidden in IndetifyProfile() and IdentifyLevel()
    sps->profile_idc = pInfo->ProfileIDC;
    sps->level_idc = pInfo->LevelIDC;

    // needs to be set according to profile
    sps->constrained_set0_flag = FALSE;
	sps->constrained_set1_flag = TRUE;	// Constrained Baseline or Main
    sps->constrained_set2_flag = FALSE;

    if ( (sps->level_idc == 9) ) // Level 1.b
    {
        sps->constrained_set3_flag = TRUE;
        sps->level_idc = 11;
    }
    else
    {
        sps->constrained_set3_flag = FALSE;
    }

    // Parameter Set ID hard coded to zero
    sps->seq_parameter_set_id = 0;

    // Fidelity Range Extensions stuff
    sps->bit_depth_luma_minus8   = 0;
    sps->bit_depth_chroma_minus8 = 0;

    //! POC stuff:
    //! The following values are hard-coded in init_poc().  Apparently,
    //! the poc implementation covers only a subset of the poc functionality.
    //! Here, the same subset is implemented.  Changes in the POC stuff have
    //! also to be reflected here
    sps->log2_max_frame_num_minus4 = 1;//log2_max_frame_num_minus4;
    sps->log2_max_pic_order_cnt_lsb_minus4 = 0;	// Not used

    sps->pic_order_cnt_type = 2; //This is for MFE
    sps->num_ref_frames_in_pic_order_cnt_cycle = 1;	// Not used
    sps->delta_pic_order_always_zero_flag = FALSE;	// Not used
    sps->offset_for_non_ref_pic = 0;	// Not used
    sps->offset_for_top_to_bottom_field = 1;

/*
    for (i=0; i<img->num_ref_frames_in_pic_order_cnt_cycle; i++)
    {
        sps->offset_for_ref_frame[i] = img->offset_for_ref_frame[i];
    }
*/
    // End of POC stuff

    // Number of Reference Frames
    sps->num_ref_frames = pInfo->num_ref_frames;

    //required_frame_num_update_behaviour_flag hardcoded to zero
    sps->gaps_in_frame_num_value_allowed_flag = FALSE;    // double check

    sps->frame_mbs_only_flag = (MFE_BOOL) !pConfig->bInterlace;

    // Picture size, finally a simple one :-)
    sps->pic_width_in_mbs_minus1 = (pConfig->nBufWidth/16) -1;
    sps->pic_height_in_map_units_minus1 = ((pConfig->nBufHeight/16)/ (2 - sps->frame_mbs_only_flag)) - 1;

    // a couple of flags, simple
    sps->mb_adaptive_frame_field_flag = FALSE;
    sps->direct_8x8_inference_flag = TRUE;

    // Sequence VUI not implemented, signalled as not present
    sps->vui_parameters_present_flag = TRUE;

    sps->chroma_format_idc = 1;
    sps->separate_colour_plane_flag = 0;

    if ( sps->vui_parameters_present_flag )
        _GenerateVUIParameters(pConfig, sps);

    if((pConfig->nBufHeight!=pConfig->nDispHeight)||(pConfig->nBufWidth!=pConfig->nDispWidth)) {
        sps->frame_cropping_flag = TRUE;
    }
    else {
        sps->frame_cropping_flag = FALSE;
    }

    {
        MFE_U32 CropUnitX,CropUnitY;
        if(sps->chroma_format_idc == 0) {
            CropUnitX = 1;
            CropUnitY = 2 - sps->frame_mbs_only_flag;
        }
        else { //only support 420.
            CropUnitX = 2;
            CropUnitY = 2*(2-sps->frame_mbs_only_flag);

        }
        if(sps->frame_cropping_flag == TRUE){
            sps->frame_cropping_rect_left_offset = 0;
            sps->frame_cropping_rect_top_offset = 0;
            sps->frame_cropping_rect_right_offset = (16 - pConfig->nDispWidth%16) / CropUnitX;
            sps->frame_cropping_rect_bottom_offset = (16 - pConfig->nDispHeight%16)  / CropUnitY;
        }
        else {
            sps->frame_cropping_rect_left_offset = 0;
            sps->frame_cropping_rect_top_offset = 0;
            sps->frame_cropping_rect_right_offset = 0;
            sps->frame_cropping_rect_bottom_offset = 0;
        }
    }
}

/*!
 ************************************************************************
 * \brief
 *    GeneratePictureParameterSet:
 *    Generates a Picture Parameter Set structure
 *
 * \par
 *    Regarding the QP
 *    The previous software versions coded the absolute QP only in the
 *    slice header.  This is kept, and the offset in the PPS is coded
 *    even if we could save bits by intelligently using this field.
 *
 ************************************************************************
 */

void GeneratePictureParameterSet( MFE_CONFIG* pConfig,
								  pic_parameter_set_rbsp_t *pps, //!< Picture Parameter Set to be filled
                                  seq_parameter_set_rbsp_t *sps, //!< used Sequence Parameter Set
                                  int PPS_id                    //!< PPS ID
								  )
{
    H264INFO *pInfo = &pConfig->ctxH264Info;

    // *************************************************************************
    // Picture Parameter Set
    // *************************************************************************

    pps->seq_parameter_set_id = sps->seq_parameter_set_id;
    pps->pic_parameter_set_id = PPS_id;
    pps->entropy_coding_mode_flag = FALSE;

    // JVT-Fxxx (by Stephan Wenger, make this flag unconditional
    pps->pic_order_present_flag = pInfo->pic_order_present_flag;

    pps->num_slice_groups_minus1 = 0;

    pps->num_ref_idx_l0_active_minus1 = sps->frame_mbs_only_flag ? (sps->num_ref_frames-1) : (2 * sps->num_ref_frames - 1) ;   // set defaults
    pps->num_ref_idx_l1_active_minus1 = sps->frame_mbs_only_flag ? (sps->num_ref_frames-1) : (2 * sps->num_ref_frames - 1) ;   // set defaults

    pps->weighted_pred_flag = FALSE;
    pps->weighted_bipred_idc = FALSE;

    pps->pic_init_qp_minus26 = 0;         // hard coded to zero, QP lives in the slice header
    pps->pic_init_qs_minus26 = 0;

    pps->chroma_qp_index_offset = 0;

    pps->deblocking_filter_control_present_flag = (MFE_BOOL) pInfo->bDeblockCtrlPresent;
    pps->constrained_intra_pred_flag = (MFE_BOOL) pInfo->ieap_constraint_intra;

    // if redundant slice is in use.
    pps->redundant_pic_cnt_present_flag = 0;
}


/*!
 *************************************************************************************
 * \brief
 *    int _GenerateSeq_parameter_set_rbsp (seq_parameter_set_rbsp_t *sps, char *rbsp);
 *
 * \param sps
 *    sequence parameter structure
 * \param rbsp
 *    buffer to be filled with the rbsp, size should be at least MAXIMUMPARSETRBSPSIZE
 *
 * \return
 *    size of the RBSP in bytes
 *
 * \note
 *    Sequence Parameter VUI function is called, but the function implements
 *    an exit (-1)
 *************************************************************************************
 */
static int _GenerateSeq_parameter_set_rbsp (seq_parameter_set_rbsp_t *sps, MFE_BYTE *rbsp)
{
	Bitstream stream, *bitstream = &stream;
    int len = 0, LenInBytes;
    unsigned i;

    MFE_ASSERT (rbsp != NULL);

    // .. and use the rbsp provided (or allocated above) for the data
    bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

    len+=u_v  (8, "SPS: profile_idc",                             sps->profile_idc,                               bitstream);

    len+=u_1  ("SPS: constrained_set0_flag",                      sps->constrained_set0_flag,    bitstream);
    len+=u_1  ("SPS: constrained_set1_flag",                      sps->constrained_set1_flag,    bitstream);
    len+=u_1  ("SPS: constrained_set2_flag",                      sps->constrained_set2_flag,    bitstream);
    len+=u_1  ("SPS: constrained_set3_flag",                      sps->constrained_set3_flag,    bitstream);
    len+=u_v  (4, "SPS: reserved_zero_4bits",                     0,                             bitstream);

    len+=u_v  (8, "SPS: level_idc",                               sps->level_idc,                                 bitstream);

    len+=ue_v ("SPS: seq_parameter_set_id",                    sps->seq_parameter_set_id,                      bitstream);

    len+=ue_v ("SPS: log2_max_frame_num_minus4",               sps->log2_max_frame_num_minus4,                 bitstream);
    len+=ue_v ("SPS: pic_order_cnt_type",                      sps->pic_order_cnt_type,                        bitstream);

    if (sps->pic_order_cnt_type == 0)
        len+=ue_v ("SPS: log2_max_pic_order_cnt_lsb_minus4",     sps->log2_max_pic_order_cnt_lsb_minus4,         bitstream);
    else if (sps->pic_order_cnt_type == 1)
    {
        len+=u_1  ("SPS: delta_pic_order_always_zero_flag",        sps->delta_pic_order_always_zero_flag,          bitstream);
        len+=se_v ("SPS: offset_for_non_ref_pic",                  sps->offset_for_non_ref_pic,                    bitstream);
        len+=se_v ("SPS: offset_for_top_to_bottom_field",          sps->offset_for_top_to_bottom_field,            bitstream);
        len+=ue_v ("SPS: num_ref_frames_in_pic_order_cnt_cycle",   sps->num_ref_frames_in_pic_order_cnt_cycle,     bitstream);
        for (i=0; i<sps->num_ref_frames_in_pic_order_cnt_cycle; i++)
            len+=se_v ("SPS: offset_for_ref_frame",                  sps->offset_for_ref_frame[i],                      bitstream);
    }
    len+=ue_v ("SPS: num_ref_frames",                          sps->num_ref_frames,                            bitstream);
    len+=u_1  ("SPS: gaps_in_frame_num_value_allowed_flag",    sps->gaps_in_frame_num_value_allowed_flag,      bitstream);
    len+=ue_v ("SPS: pic_width_in_mbs_minus1",                 sps->pic_width_in_mbs_minus1,                   bitstream);
    len+=ue_v ("SPS: pic_height_in_map_units_minus1",          sps->pic_height_in_map_units_minus1,            bitstream);
    len+=u_1  ("SPS: frame_mbs_only_flag",                     sps->frame_mbs_only_flag,                       bitstream);
    if (!sps->frame_mbs_only_flag)
    {
        len+=u_1  ("SPS: mb_adaptive_frame_field_flag",            sps->mb_adaptive_frame_field_flag,              bitstream);
    }
    len+=u_1  ("SPS: direct_8x8_inference_flag",               sps->direct_8x8_inference_flag,                 bitstream);

    len+=u_1  ("SPS: frame_cropping_flag",                      sps->frame_cropping_flag,                       bitstream);

    if (sps->frame_cropping_flag)
    {
        len+=ue_v ("SPS: frame_cropping_rect_left_offset",          sps->frame_cropping_rect_left_offset,           bitstream);
        len+=ue_v ("SPS: frame_cropping_rect_right_offset",         sps->frame_cropping_rect_right_offset,          bitstream);
        len+=ue_v ("SPS: frame_cropping_rect_top_offset",           sps->frame_cropping_rect_top_offset,            bitstream);
        len+=ue_v ("SPS: frame_cropping_rect_bottom_offset",        sps->frame_cropping_rect_bottom_offset,         bitstream);
    }

    len+=u_1  ("SPS: vui_parameters_present_flag",             sps->vui_parameters_present_flag,               bitstream);

    if (sps->vui_parameters_present_flag)
        len+=_GenerateVUI_parameters_rbsp(sps, bitstream);    // currently a dummy, asserting

    SODBtoRBSP(bitstream);     // copies the last couple of bits into the byte buffer

    LenInBytes=bitstream->byte_pos;

    //free (bitstream);

    return LenInBytes;
}


/*!
 ***********************************************************************************************
 * \brief
 *    int _GeneratePic_parameter_set_rbsp (pic_parameter_set_rbsp_t *sps, char *rbsp);
 *
 * \param pps
 *    picture parameter structure
 * \param rbsp
 *    buffer to be filled with the rbsp, size should be at least MAXIMUMPARSETRBSPSIZE
 *
 * \return
 *    size of the RBSP in bytes, negative in case of an error
 *
 * \note
 *    Picture Parameter VUI function is called, but the function implements
 *    an exit (-1)
 ************************************************************************************************
 */

static int _GeneratePic_parameter_set_rbsp (MFE_CONFIG* pConfig, pic_parameter_set_rbsp_t *pps, MFE_BYTE *rbsp)
{
    Bitstream stream, *bitstream = &stream;
    int len = 0, LenInBytes;
//    int profile_idc;

    MFE_ASSERT (rbsp != NULL);

    // .. and use the rbsp provided (or allocated above) for the data
    bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

    pps->pic_order_present_flag = pConfig->ctxH264Info.pic_order_present_flag;

    len+=ue_v ("PPS: pic_parameter_set_id",                    pps->pic_parameter_set_id,                      bitstream);
    len+=ue_v ("PPS: seq_parameter_set_id",                    pps->seq_parameter_set_id,                      bitstream);
    len+=u_1  ("PPS: entropy_coding_mode_flag",                pps->entropy_coding_mode_flag,                  bitstream);
    len+=u_1  ("PPS: pic_order_present_flag",                  pps->pic_order_present_flag,                    bitstream);
    len+=ue_v ("PPS: num_slice_groups_minus1",                 pps->num_slice_groups_minus1,                   bitstream);
    MFE_ASSERT(pps->num_slice_groups_minus1 == 0);

    len+=ue_v ("PPS: num_ref_idx_l0_active_minus1",             pps->num_ref_idx_l0_active_minus1,              bitstream);
    len+=ue_v ("PPS: num_ref_idx_l1_active_minus1",             pps->num_ref_idx_l1_active_minus1,              bitstream);
    len+=u_1  ("PPS: weighted_pred_flag",                       pps->weighted_pred_flag,                        bitstream);
    len+=u_v  (2, "PPS: weighted_bipred_idc",                   pps->weighted_bipred_idc,                       bitstream);
    len+=se_v ("PPS: pic_init_qp_minus26",                      pps->pic_init_qp_minus26,                       bitstream);
    len+=se_v ("PPS: pic_init_qs_minus26",                      pps->pic_init_qs_minus26,                       bitstream);

//    profile_idc = pConfig->ctxH264Info.ProfileIDC;//IdentifyProfile();
    len+=se_v ("PPS: chroma_qp_index_offset",                 pps->chroma_qp_index_offset,                    bitstream);

    len+=u_1  ("PPS: deblocking_filter_control_present_flag",   pps->deblocking_filter_control_present_flag,    bitstream);
    len+=u_1  ("PPS: constrained_intra_pred_flag",              pps->constrained_intra_pred_flag,               bitstream);
    len+=u_1  ("PPS: redundant_pic_cnt_present_flag",           pps->redundant_pic_cnt_present_flag,            bitstream);

    SODBtoRBSP(bitstream);     // copies the last couple of bits into the byte buffer

    LenInBytes=bitstream->byte_pos;

    // Get rid of the helper structures
    //free (bitstream);

    return LenInBytes;
}


/*!
 *************************************************************************************
 * \brief
 *    Function body for VUI Parameter generation (to be done)
 *
 * \return
 *    exits with error message
 *************************************************************************************
 */
static int _GenerateVUI_parameters_rbsp(seq_parameter_set_rbsp_t *sps, Bitstream *bitstream)
{
    int len=0;
    vui_seq_parameters_t *vui_seq_parameters = &(sps->vui_seq_parameters);

    len+=u_1 ("VUI: aspect_ratio_info_present_flag", vui_seq_parameters->aspect_ratio_info_present_flag, bitstream);
    if (vui_seq_parameters->aspect_ratio_info_present_flag)
    {
        len+=u_v (8,"VUI: aspect_ratio_idc", vui_seq_parameters->aspect_ratio_idc, bitstream);
        if (vui_seq_parameters->aspect_ratio_idc == 255)
        {
            len+=u_v (16,"VUI: sar_width",  vui_seq_parameters->sar_width,  bitstream);
            len+=u_v (16,"VUI: sar_height", vui_seq_parameters->sar_height, bitstream);
        }
    }

    len+=u_1 ("VUI: overscan_info_present_flag", vui_seq_parameters->overscan_info_present_flag, bitstream);
    if (vui_seq_parameters->overscan_info_present_flag)
    {
        len+=u_1 ("VUI: overscan_appropriate_flag", vui_seq_parameters->overscan_appropriate_flag, bitstream);
    }

    len+=u_1 ("VUI: video_signal_type_present_flag", vui_seq_parameters->video_signal_type_present_flag, bitstream);
    if (vui_seq_parameters->video_signal_type_present_flag)
    {
        len+=u_v (3,"VUI: video_format", vui_seq_parameters->video_format, bitstream);
        len+=u_1 ("VUI: video_full_range_flag", vui_seq_parameters->video_full_range_flag, bitstream);
        len+=u_1 ("VUI: colour_description_present_flag", vui_seq_parameters->colour_description_present_flag, bitstream);
        if (vui_seq_parameters->colour_description_present_flag)
        {
            len+=u_v (8,"VUI: colour_primaries", vui_seq_parameters->colour_primaries, bitstream);
            len+=u_v (8,"VUI: transfer_characteristics", vui_seq_parameters->transfer_characteristics, bitstream);
            len+=u_v (8,"VUI: matrix_coefficients", vui_seq_parameters->matrix_coefficients, bitstream);
        }
    }

    len+=u_1 ("VUI: chroma_loc_info_present_flag", vui_seq_parameters->chroma_location_info_present_flag, bitstream);
    if (vui_seq_parameters->chroma_location_info_present_flag)
    {
        len+=ue_v ("VUI: chroma_sample_loc_type_top_field", vui_seq_parameters->chroma_sample_loc_type_top_field, bitstream);
        len+=ue_v ("VUI: chroma_sample_loc_type_bottom_field", vui_seq_parameters->chroma_sample_loc_type_bottom_field, bitstream);
    }

    len+=u_1 ("VUI: timing_info_present_flag", vui_seq_parameters->timing_info_present_flag, bitstream);
    // timing parameters
    if (vui_seq_parameters->timing_info_present_flag)
    {
        len+=u_v (32,"VUI: num_units_in_tick",  vui_seq_parameters->num_units_in_tick, bitstream);
        len+=u_v (32,"VUI: time_scale",         vui_seq_parameters->time_scale, bitstream);
        len+=u_1 ("VUI: fixed_frame_rate_flag", vui_seq_parameters->fixed_frame_rate_flag, bitstream);
    }
    // end of timing parameters
    // nal_hrd_parameters_present_flag
    len+=u_1 ("VUI: nal_hrd_parameters_present_flag", vui_seq_parameters->nal_hrd_parameters_present_flag, bitstream);
    MFE_ASSERT(vui_seq_parameters->nal_hrd_parameters_present_flag==0);
//     if ( vui_seq_parameters->nal_hrd_parameters_present_flag )
//     {
//         len += WriteHRDParameters(sps, bitstream);
//     }
    // vcl_hrd_parameters_present_flag
    len+=u_1 ("VUI: vcl_hrd_parameters_present_flag", vui_seq_parameters->vcl_hrd_parameters_present_flag, bitstream);
    MFE_ASSERT(vui_seq_parameters->vcl_hrd_parameters_present_flag==0);
//     if ( vui_seq_parameters->vcl_hrd_parameters_present_flag )
//     {
//         len += WriteHRDParameters(sps, bitstream);
//     }
    if ( vui_seq_parameters->nal_hrd_parameters_present_flag || vui_seq_parameters->vcl_hrd_parameters_present_flag )
    {
        len+=u_1 ("VUI: low_delay_hrd_flag", vui_seq_parameters->low_delay_hrd_flag, bitstream );
    }
    len+=u_1 ("VUI: pic_struct_present_flag", vui_seq_parameters->pic_struct_present_flag, bitstream);

    len+=u_1 ("VUI: bitstream_restriction_flag", vui_seq_parameters->bitstream_restriction_flag, bitstream);
    if (vui_seq_parameters->bitstream_restriction_flag)
    {
        len+=u_1  ("VUI: motion_vectors_over_pic_boundaries_flag", vui_seq_parameters->motion_vectors_over_pic_boundaries_flag, bitstream);
        len+=ue_v ("VUI: max_bytes_per_pic_denom", vui_seq_parameters->max_bytes_per_pic_denom, bitstream);
        len+=ue_v ("VUI: max_bits_per_mb_denom", vui_seq_parameters->max_bits_per_mb_denom, bitstream);
        len+=ue_v ("VUI: log2_max_mv_length_horizontal", vui_seq_parameters->log2_max_mv_length_horizontal, bitstream);
        len+=ue_v ("VUI: log2_max_mv_length_vertical", vui_seq_parameters->log2_max_mv_length_vertical, bitstream);
        len+=ue_v ("VUI: num_reorder_frames", vui_seq_parameters->num_reorder_frames, bitstream);
        len+=ue_v ("VUI: max_dec_frame_buffering", vui_seq_parameters->max_dec_frame_buffering, bitstream);
    }

    return len;
}



/*!
 *************************************************************************************
 * \brief
 *    void _GenerateVUIParameters(seq_parameter_set_rbsp_t *sps)
 *
 *
 * \return
 *    none
 *
 * \note
 *************************************************************************************
 */
static void _GenerateVUIParameters(MFE_CONFIG *pConfig, seq_parameter_set_rbsp_t *sps)
{
  unsigned int          SchedSelIdx;
  hrd_parameters_t     *nal_hrd = &(sps->vui_seq_parameters.nal_hrd_parameters);
  hrd_parameters_t     *vcl_hrd = &(sps->vui_seq_parameters.vcl_hrd_parameters);
  vui_seq_parameters_t *vui     = &(sps->vui_seq_parameters);

  vui->aspect_ratio_info_present_flag      = (MFE_BOOL) 0;
  vui->aspect_ratio_idc                    = (unsigned int) 1;
  vui->sar_width                           = (unsigned int) 0;
  vui->sar_height                          = (unsigned int) 0;
  vui->overscan_info_present_flag          = (MFE_BOOL) 0;
  vui->overscan_appropriate_flag           = (MFE_BOOL) 0;
  vui->video_signal_type_present_flag      = (MFE_BOOL) 0;
  vui->video_format                        = (unsigned int) 5;
  vui->video_full_range_flag               = (MFE_BOOL) 0;
  vui->colour_description_present_flag     = (MFE_BOOL) 0;
  vui->colour_primaries                    = (unsigned int) 2;
  vui->transfer_characteristics            = (unsigned int) 2;
  vui->matrix_coefficients                 = (unsigned int) 2;
  vui->chroma_location_info_present_flag   = (MFE_BOOL) 0;
  vui->chroma_sample_loc_type_top_field    = (unsigned int) 0;
  vui->chroma_sample_loc_type_bottom_field = (unsigned int) 0;
  vui->timing_info_present_flag            = (MFE_BOOL) TRUE;
  switch (pConfig->FrameRatex100) {
  case 3000:
	  vui->num_units_in_tick = (unsigned int) 1;
	  vui->time_scale        = (unsigned int) 60;
	  break;
  case 1500:
	  vui->num_units_in_tick = (unsigned int) 1;
	  vui->time_scale        = (unsigned int) 30;
	  break;
  case 2500:
	  vui->num_units_in_tick = (unsigned int) 1;
	  vui->time_scale        = (unsigned int) 50;
	  break;
  case 2997:
	  vui->num_units_in_tick = (unsigned int) 1001;
	  vui->time_scale        = (unsigned int) 60000;
	  break;
  default:
	  vui->num_units_in_tick = (unsigned int) 1;
	  vui->time_scale        = (unsigned int) pConfig->FrameRatex100*2/100;
	  MFE_ASSERT(0);
  }
  vui->fixed_frame_rate_flag               = (MFE_BOOL) FALSE;

  // NAL HRD parameters
  vui->nal_hrd_parameters_present_flag             = (MFE_BOOL) 0;
  nal_hrd->cpb_cnt_minus1                          = (unsigned int) 0;
  nal_hrd->bit_rate_scale                          = (unsigned int) 0;
  nal_hrd->cpb_size_scale                          = (unsigned int) 0;
  for ( SchedSelIdx = 0; SchedSelIdx <= nal_hrd->cpb_cnt_minus1; SchedSelIdx++ )
  {
    nal_hrd->bit_rate_value_minus1[SchedSelIdx]    = (unsigned int) 0;
    nal_hrd->cpb_size_value_minus1[SchedSelIdx]    = (unsigned int) 0;
    nal_hrd->cbr_flag[SchedSelIdx]             = (unsigned int) 0;
  }
  nal_hrd->initial_cpb_removal_delay_length_minus1 = (unsigned int) 23;
  nal_hrd->cpb_removal_delay_length_minus1         = (unsigned int) 23;
  nal_hrd->dpb_output_delay_length_minus1          = (unsigned int) 23;
  nal_hrd->time_offset_length                      = (unsigned int) 24;

  // VCL HRD parameters
  vui->vcl_hrd_parameters_present_flag             = (MFE_BOOL) 0;
  vcl_hrd->cpb_cnt_minus1                          = (unsigned int) 0;
  vcl_hrd->bit_rate_scale                          = (unsigned int) 0;
  vcl_hrd->cpb_size_scale                          = (unsigned int) 0;
  for ( SchedSelIdx = 0; SchedSelIdx <= vcl_hrd->cpb_cnt_minus1; SchedSelIdx++ )
  {
    vcl_hrd->bit_rate_value_minus1[SchedSelIdx]    = (unsigned int) 0;
    vcl_hrd->cpb_size_value_minus1[SchedSelIdx]    = (unsigned int) 0;
    vcl_hrd->cbr_flag[SchedSelIdx]             = (unsigned int) 0;
  }
  vcl_hrd->initial_cpb_removal_delay_length_minus1 = (unsigned int) 23;
  vcl_hrd->cpb_removal_delay_length_minus1         = (unsigned int) 23;
  vcl_hrd->dpb_output_delay_length_minus1          = (unsigned int) 23;
  vcl_hrd->time_offset_length                      = (unsigned int) 24;

  vui->low_delay_hrd_flag                      = (MFE_BOOL) 0;
  vui->pic_struct_present_flag                 = (MFE_BOOL) 0;
  vui->bitstream_restriction_flag              = (MFE_BOOL) 1;
  vui->motion_vectors_over_pic_boundaries_flag = (MFE_BOOL) 1;
  vui->max_bytes_per_pic_denom                 = (unsigned int) 0;
  vui->max_bits_per_mb_denom                   = (unsigned int) 0;
  vui->log2_max_mv_length_horizontal           = (unsigned int) 7;
  vui->log2_max_mv_length_vertical             = (unsigned int) 7;
  vui->num_reorder_frames                      = (unsigned int) 0;
  vui->max_dec_frame_buffering                 = (unsigned int) 2;
}


/*!
********************************************************************************************
* \brief
*    Writes a NALU to the Annex B Byte Stream
*
* \return
*    number of bits written
*
********************************************************************************************
*/
static int WriteAnnexbNALU(OutStream* pStream, NALU_t *n)
{
	unsigned int i, leftbits;
	int BitsWritten = 0;
	MFE_ASSERT (n != NULL);
	MFE_ASSERT (n->forbidden_bit == 0);
	MFE_ASSERT (n->startcodeprefix_len == 3 || n->startcodeprefix_len == 4);

	// printf ("WriteAnnexbNALU: writing %d bytes w/ startcode_len %d\n", n->len+1, n->startcodeprefix_len);
	if (n->startcodeprefix_len > 3)
	{
		osPutBits(pStream, 0, 8, "start-code 0");
		BitsWritten =+ 8;
	}
	osPutBits(pStream, 0, 8, "start-code 0");
	osPutBits(pStream, 0, 8, "start-code 0");
	osPutBits(pStream, 1, 8, "start-code 1");
	BitsWritten += 24;

	n->buf[0] = (unsigned char) ((n->forbidden_bit << 7) | (n->nal_reference_idc << 5) | n->nal_unit_type);
	// printf ("First Byte %x, nal_ref_idc %x, nal_unit_type %d\n", n->buf[0], n->nal_reference_idc, n->nal_unit_type);

	for (i=0; i<n->len/8; i++)
		osPutBits(pStream, n->buf[i], 8, "nalu-body");
	leftbits = n->len - (i*8);
	if (leftbits>0)
		osPutBits(pStream, n->buf[i]&(0xFF>>(8-leftbits)), leftbits, "nalu-body (last-bits)");
	BitsWritten += n->len;

	return BitsWritten;
}

/*!
************************************************************************
* \brief
*    Selects picture type and codes it to symbol
*
* \return
*    symbol value for picture type
************************************************************************
*/
static int get_picture_type(MFE_CONFIG* pConfig)
{
	// set this value to zero for transmission without signaling
	// that the whole picture has the same slice type
	const int same_slicetype_for_whole_frame = 0;//5;

	switch (pConfig->vopPredType)
	{
	case I_VOP:
		return 2 + same_slicetype_for_whole_frame;
		break;
	case P_VOP:
		return 0 + same_slicetype_for_whole_frame;
		break;
	case B_VOP:
		return 1 + same_slicetype_for_whole_frame;
		break;
	}

	return 0;
}

/*!
********************************************************************************************
* \brief
*    writes the ref_pic_list_reordering syntax
*    based on content of according fields in img structure
*
* \return
*    number of bits used
********************************************************************************************
*/
static int ref_pic_list_reordering(MFE_CONFIG* pConfig, Bitstream *bitstream)
{
	int len=0;

	if (pConfig->vopPredType!=I_VOP)
	{
		len += u_1 ("SH: ref_pic_list_reordering_flag_l0", /*currSlice->ref_pic_list_reordering_flag_l0*/0, bitstream);
	}

	return len;
}

/*!
************************************************************************
* \brief
*    write the memory management control operations
*
* \return
*    number of bits used
************************************************************************
*/
static int dec_ref_pic_marking(MFE_CONFIG* pConfig, Bitstream *bitstream)
{
	H264INFO* pInfo = &pConfig->ctxH264Info;

	int len=0;

	if (pInfo->idr_flag)
	{
		len += u_1("SH: no_output_of_prior_pics_flag", /*img->no_output_of_prior_pics_flag*/0, bitstream);
		len += u_1("SH: long_term_reference_flag", /*img->long_term_reference_flag*/0, bitstream);
	}
	else
	{
		len += u_1("SH: adaptive_ref_pic_buffering_flag", /*img->adaptive_ref_pic_buffering_flag*/0, bitstream);
	}
	return len;
}

/*!
 ********************************************************************************************
 * \brief
 *    Write a slice header
 *
 * \return
 *    number of bits used
 ********************************************************************************************
*/
static int SliceHeader(MFE_CONFIG* pConfig, MFE_BYTE *rbsp)
{
	Bitstream stream, *bitstream = &stream;
	H264INFO* pInfo = &pConfig->ctxH264Info;
	int len = 0;
	unsigned int field_pic_flag = 0, bottom_field_flag = 0;

	bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

	len  = ue_v("SH: first_mb_in_slice", /*img->current_mb_nr*/0,   bitstream);
	len += ue_v("SH: slice_type",        get_picture_type(pConfig),   bitstream);
	len += ue_v("SH: pic_parameter_set_id" , active_pps->pic_parameter_set_id ,bitstream);
	len += u_v (pInfo->log2_max_frame_num_minus4 + 4,"SH: frame_num", pInfo->frame_num, bitstream);

	if (!active_sps->frame_mbs_only_flag)
	{
		// field_pic_flag    u(1)
		field_pic_flag = (pInfo->structure ==TOP_FIELD || pInfo->structure ==BOTTOM_FIELD)?1:0;
		//MFE_ASSERT( field_pic_flag == img->fld_flag );
		len += u_1("SH: field_pic_flag", field_pic_flag, bitstream);

		if (field_pic_flag)
		{
			//bottom_field_flag     u(1)
			bottom_field_flag = (pInfo->structure == BOTTOM_FIELD)?1:0;
			len += u_1("SH: bottom_field_flag" , bottom_field_flag ,bitstream);
		}
	}

	if (pInfo->idr_flag)
	{
		// idr_pic_id
		len += ue_v ("SH: idr_pic_id", (pInfo->number % 2), bitstream);
	}

	if (pInfo->pic_order_cnt_type == 0)
	{
		MFE_ASSERT(0);
/*
		if (active_sps->frame_mbs_only_flag)
		{
			img->pic_order_cnt_lsb = (img->toppoc & ~((((unsigned int)(-1)) << (log2_max_pic_order_cnt_lsb_minus4+4))) );
		}
		else
		{
			if (!field_pic_flag || img->structure == TOP_FIELD)
				img->pic_order_cnt_lsb = (img->toppoc & ~((((unsigned int)(-1)) << (log2_max_pic_order_cnt_lsb_minus4+4))) );
			else if ( img->structure == BOTTOM_FIELD )
				img->pic_order_cnt_lsb = (img->bottompoc & ~((((unsigned int)(-1)) << (log2_max_pic_order_cnt_lsb_minus4+4))) );
		}

		len += u_v (log2_max_pic_order_cnt_lsb_minus4+4, "SH: pic_order_cnt_lsb", img->pic_order_cnt_lsb, bitstream);

		if (img->pic_order_present_flag && !field_pic_flag)
		{
			len += se_v ("SH: delta_pic_order_cnt_bottom", img->delta_pic_order_cnt_bottom, bitstream);
		}
*/
	}
	if (pInfo->pic_order_cnt_type == 1 /*&& !img->delta_pic_order_always_zero_flag*/)
	{
		MFE_ASSERT(0);
/*
		len += se_v ("SH: delta_pic_order_cnt[0]", img->delta_pic_order_cnt[0], bitstream);

		if (img->pic_order_present_flag && !field_pic_flag)
		{
			len += se_v ("SH: delta_pic_order_cnt[1]", img->delta_pic_order_cnt[1], bitstream);
		}
*/
	}

	if (pConfig->vopPredType == P_VOP)
	{
		int override_flag;

		//this is for MFE fixed setting override_flag
		override_flag = (pInfo->num_ref_idx_l0_active != (active_pps->num_ref_idx_l0_active_minus1 +1)) ? 1 : 0;
		len +=  u_1 ("SH: num_ref_idx_active_override_flag", override_flag, bitstream);
		if (override_flag)
		{
			len += ue_v ("SH: num_ref_idx_l0_active_minus1", pInfo->num_ref_idx_l0_active-1, bitstream);
		}
	}
	len += ref_pic_list_reordering(pConfig, bitstream);

	if (pInfo->nal_ref_idc)
		len += dec_ref_pic_marking(pConfig, bitstream);

	len += se_v("SH: slice_qp_delta", (pInfo->intStep - 26 - active_pps->pic_init_qp_minus26), bitstream);

	if (active_pps->deblocking_filter_control_present_flag)
	{
		len += ue_v("SH: disable_deblocking_filter_idc", pInfo->nDeblockIDC, bitstream);  // Turn loop filter on/off on slice basis
		if (pInfo->nDeblockIDC!=1)
		{
			len += se_v ("SH: slice_alpha_c0_offset_div2", pInfo->nDeblockAlpha / 2, bitstream);
			len += se_v ("SH: slice_beta_offset_div2", pInfo->nDeblockBeta / 2, bitstream);
		}
	}

	// Put final bits
	if (bitstream->bits_to_go!=8)
		bitstream->streamBuffer[bitstream->byte_pos] = bitstream->byte_buf;

	return len;
}

MFE_U8 get_NALU_1stbyte(H264INFO *pInfo, MFE_U8 vopPredType)
{
    int nal_unit_type, nal_reference_idc, forbidden_bit;
    if (pInfo->idr_flag)
    {
        nal_unit_type = NALU_TYPE_IDR;
        nal_reference_idc = NALU_PRIORITY_HIGHEST;
    }
    else if (vopPredType == B_VOP)
    {
        nal_unit_type = NALU_TYPE_SLICE;
        if (pInfo->nal_ref_idc !=0)
        {
            nal_reference_idc = NALU_PRIORITY_HIGH;
        }
        else
        {
            nal_reference_idc = NALU_PRIORITY_DISPOSABLE;
        }
    }
    else   // non-b frame, non IDR slice
    {
        nal_unit_type = NALU_TYPE_SLICE;
        if (pInfo->nal_ref_idc !=0)
        {
            nal_reference_idc = NALU_PRIORITY_HIGH;
        }
        else
        {
            nal_reference_idc = NALU_PRIORITY_DISPOSABLE;
        }
    }
    forbidden_bit = 0;
    return (MFE_U8) ((forbidden_bit << 7) | (nal_reference_idc << 5) | nal_unit_type);
}

void codeSPSPPS(OutStream* pStream)
{
	WriteAnnexbNALU(pStream, &gNaluStruct[STATIC_SPS]);
	WriteAnnexbNALU(pStream, &gNaluStruct[STATIC_PPS]);
}

int codeH264ConfigHeaders(MFE_CONFIG *pConfig)
{
	OutStream* pStream = &pConfig->m_OutStream;

	osReset(pStream);
	// SPS, PPS
	codeSPSPPS(pStream);
	// Finalize
	osFlushAll(pStream);

	// The generated bytes start from pStream->m_pbFrameBuffer and with pStream->m_nByteCount bytes.
	return pStream->m_nByteCount;
}

void codeSliceHeader(MFE_CONFIG* pConfig, OutStream* pStream)
{
	NALU_t naluBuf, *nalu = &naluBuf;
	H264INFO* pInfo = &pConfig->ctxH264Info;
       MFE_BYTE rbsp[MAXRBSPSIZE];
	MFE_U8 Byte1;

	nalu->buf = rbsp;
	nalu->len = 8 + SliceHeader(pConfig, &rbsp[1]);	// rbsp[0] is reserved for nal_unit_idc and nal_unit_type

    nalu->startcodeprefix_len = 4;

	Byte1 = get_NALU_1stbyte(pInfo, pConfig->vopPredType);
	nalu->nal_unit_type = (NaluType)(Byte1&0x1f);
	nalu->nal_reference_idc = (NalRefIdc)((Byte1>>5)&0x3);
	nalu->forbidden_bit = 0;

	WriteAnnexbNALU(pStream, nalu);
}

