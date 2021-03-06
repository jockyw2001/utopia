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

#include <stdio.h>
#include <string.h>
#include "MFE_chip.h"
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"

#include "parsetcommon.h"
#include "parset.h"
#include "nalu.h"
#include "vlc.h"

//seq_parameter_set_rbsp_t gSPS;
//pic_parameter_set_rbsp_t gPPS;
//seq_parameter_set_rbsp_t *active_sps;
//pic_parameter_set_rbsp_t *active_pps;

#define STATIC_SPS 0UL
#define STATIC_PPS 1UL

// Local helpers
static void _GenerateParameterSets(MFE_CONFIG* pConfig);
//static void _FreeParameterSets (MFE_CONFIG* pConfig);

static NALU_t* _GenerateSeq_parameter_set_NALU (MFE_CONFIG* pConfig);
static NALU_t* _GeneratePic_parameter_set_NALU(MFE_CONFIG *pConfig);

static MS_S32 _GenerateVUI_parameters_rbsp(seq_parameter_set_rbsp_t *sps, Bitstream *bitstream);
static MS_S32 _GenerateSeq_parameter_set_rbsp (seq_parameter_set_rbsp_t *sps, MS_U8 *rbsp);
static MS_S32 _GeneratePic_parameter_set_rbsp (MFE_CONFIG* pConfig, pic_parameter_set_rbsp_t *pps, MS_U8 *rbsp);
static void _GenerateVUIParameters(MFE_CONFIG *pConfig, seq_parameter_set_rbsp_t *sps);


void MallocSPSPPSBuf(MFE_CONFIG* pConfig)
{
    pConfig->active_sps = MfeDrvMemMalloc(sizeof(seq_parameter_set_rbsp_t), (const MS_S8*)("active_sps"));
    if (pConfig->active_sps)
    {
        memset(pConfig->active_sps, 0, sizeof(seq_parameter_set_rbsp_t));
    }
    else
    {
        ms_dprintk(DRV_L0, "Failed to allocate sps!\n");
    }

    pConfig->active_pps = MfeDrvMemMalloc(sizeof(pic_parameter_set_rbsp_t), (const MS_S8*)("active_pps"));
    if (pConfig->active_pps)
    {
        memset(pConfig->active_pps, 0, sizeof(pic_parameter_set_rbsp_t));
    }
    else
    {
        ms_dprintk(DRV_L0, "Failed to allocate pps!\n");
    }
}

void FreeSPSPPSBuf(MFE_CONFIG* pConfig)
{
    MfeDrvMemFree((void**)&pConfig->active_sps, (const MS_S8*)("active_sps"));
    MfeDrvMemFree((void**)&pConfig->active_pps, (const MS_S8*)("active_pps"));
}

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
	_GenerateSeq_parameter_set_NALU(pConfig);
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
//    active_sps = &gSPS;
//    active_pps = &gPPS;
    GenerateSequenceParameterSet(pConfig, pConfig->active_sps, 0);
    GeneratePictureParameterSet (pConfig, pConfig->active_pps, pConfig->active_sps, 0);
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

static NALU_t* _GenerateSeq_parameter_set_NALU (MFE_CONFIG* pConfig)
{
    NALU_t *n = &pConfig->NaluStruct[STATIC_SPS];
    MS_S32 RBSPlen = 0;
//    MS_S32 NALUlen;
    MS_U8 rbsp[MAXRBSPSIZE];

    n->buf = pConfig->NaluBuf[STATIC_SPS];

    RBSPlen = _GenerateSeq_parameter_set_rbsp (pConfig->active_sps, rbsp);
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
*    NALU_t* _GeneratePic_parameter_set_NALU (MS_S32 PPS_id);
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
    NALU_t *n = &pConfig->NaluStruct[STATIC_PPS];
    MS_S32 RBSPlen = 0;
//    MS_S32 NALUlen;
    MS_U8 rbsp[MAXRBSPSIZE];

    n->buf = pConfig->NaluBuf[STATIC_PPS];

    RBSPlen = _GeneratePic_parameter_set_rbsp (pConfig, pConfig->active_pps, rbsp);
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
                                   MS_S32 SPS_id                     //!< SPS ID
                                 )
{
    H264INFO *pInfo = &pConfig->ctxH264Info;
    int ntmpDispHeight;

    ms_dprintk(DRV_L2,"In GenerateSequenceParameterSet\n");
    // *************************************************************************
    // Sequence Parameter Set
    // *************************************************************************
    MS_ASSERT (sps != NULL);
    if (sps == NULL)
    {
        ms_dprintk(DRV_L4, "[GenerateSequenceParameterSetsps] sps is null\n");
        return;
    }

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

    sps->frame_mbs_only_flag = (MS_BOOL) (pConfig->bInterlace == PROGRESSIVE);

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

    ntmpDispHeight = pConfig->nDispHeight;
    if(sps->frame_mbs_only_flag == 0) //interlace
    {
        ntmpDispHeight = ntmpDispHeight*2;
    }

    if ((pConfig->nBufHeight != ntmpDispHeight) || (pConfig->nBufWidth != pConfig->nDispWidth)) {
        sps->frame_cropping_flag = TRUE;
    }
    else {
        sps->frame_cropping_flag = FALSE;
    }

    {
        MS_U32 CropUnitX,CropUnitY;
        if (sps->chroma_format_idc == 0) {
            CropUnitX = 1;
            CropUnitY = 2 - sps->frame_mbs_only_flag;
        }
        else { //only support 420.
            CropUnitX = 2;
            CropUnitY = 2 * (2 - sps->frame_mbs_only_flag);

        }
        if (pConfig->MfeCropInfo.crop_en == TRUE) {
            sps->frame_cropping_flag = TRUE;
            sps->frame_cropping_rect_left_offset = pConfig->MfeCropInfo.crop_left / CropUnitX;
            sps->frame_cropping_rect_right_offset = pConfig->MfeCropInfo.crop_right / CropUnitX;
            sps->frame_cropping_rect_top_offset = pConfig->MfeCropInfo.crop_top / CropUnitY;
            sps->frame_cropping_rect_bottom_offset = pConfig->MfeCropInfo.crop_bottom / CropUnitY;
        } else if (sps->frame_cropping_flag == TRUE) {
            sps->frame_cropping_rect_left_offset = 0;
            sps->frame_cropping_rect_top_offset = 0;
            if (pConfig->nBufWidth != pConfig->nDispWidth)
                sps->frame_cropping_rect_right_offset = (16 - pConfig->nDispWidth % 16) / CropUnitX;
            else
                sps->frame_cropping_rect_right_offset = 0;
            if (pConfig->nBufHeight != ntmpDispHeight)
                sps->frame_cropping_rect_bottom_offset = (16 - pConfig->nDispHeight % 16)  / CropUnitY;
            else
                sps->frame_cropping_rect_bottom_offset = 0;
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
                                  MS_S32 PPS_id                    //!< PPS ID
								  )
{
    H264INFO *pInfo = &pConfig->ctxH264Info;

    // *************************************************************************
    // Picture Parameter Set
    // *************************************************************************

    pps->seq_parameter_set_id = sps->seq_parameter_set_id;
    pps->pic_parameter_set_id = PPS_id;

    pps->entropy_coding_mode_flag = (pConfig->UseCABAC==1);

    // JVT-Fxxx (by Stephan Wenger, make this flag unconditional
    pps->pic_order_present_flag = pInfo->pic_order_present_flag;

    pps->num_slice_groups_minus1 = 0;

    pps->num_ref_idx_l0_active_minus1 = pInfo->num_ref_idx_l0_active_minus1;//sps->frame_mbs_only_flag ? (sps->num_ref_frames-1) : (2 * sps->num_ref_frames - 1) ;   // set defaults
    pps->num_ref_idx_l1_active_minus1 = pInfo->num_ref_idx_l0_active_minus1;//sps->frame_mbs_only_flag ? (sps->num_ref_frames-1) : (2 * sps->num_ref_frames - 1) ;   // set defaults

    pps->weighted_pred_flag = FALSE;
    pps->weighted_bipred_idc = FALSE;

    pps->pic_init_qp_minus26 = 0;         // hard coded to zero, QP lives in the slice header
    pps->pic_init_qs_minus26 = 0;

    pps->chroma_qp_index_offset = 0;

    pps->deblocking_filter_control_present_flag = (MS_BOOL) pInfo->bDeblockCtrlPresent;
    pps->constrained_intra_pred_flag = (MS_BOOL) pInfo->ieap_constraint_intra;

    // if redundant slice is in use.
    pps->redundant_pic_cnt_present_flag = 0;
}


/*!
 *************************************************************************************
 * \brief
 *    MS_S32 _GenerateSeq_parameter_set_rbsp (seq_parameter_set_rbsp_t *sps, MS_U8 *rbsp);
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
static MS_S32 _GenerateSeq_parameter_set_rbsp (seq_parameter_set_rbsp_t *sps, MS_U8 *rbsp)
{
	Bitstream stream, *bitstream = &stream;
    MS_S32 len = 0, LenInBytes;
    MS_U32 i;

    MS_ASSERT (rbsp != NULL);

    // .. and use the rbsp provided (or allocated above) for the data
    bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

    len+=u_v  (8, (MS_S8*)("SPS: profile_idc"),                             sps->profile_idc,                               bitstream);

    len+=u_1  ((MS_S8*)("SPS: constrained_set0_flag"),                      sps->constrained_set0_flag,    bitstream);
    len+=u_1  ((MS_S8*)("SPS: constrained_set1_flag"),                      sps->constrained_set1_flag,    bitstream);
    len+=u_1  ((MS_S8*)("SPS: constrained_set2_flag"),                      sps->constrained_set2_flag,    bitstream);
    len+=u_1  ((MS_S8*)("SPS: constrained_set3_flag"),                      sps->constrained_set3_flag,    bitstream);
    len+=u_v  (4, (MS_S8*)("SPS: reserved_zero_4bits"),                     0,                             bitstream);

    len+=u_v  (8, (MS_S8*)("SPS: level_idc"),                               sps->level_idc,                                 bitstream);

    len+=ue_v ((MS_S8*)("SPS: seq_parameter_set_id"),                    sps->seq_parameter_set_id,                      bitstream);

    len+=ue_v ((MS_S8*)("SPS: log2_max_frame_num_minus4"),               sps->log2_max_frame_num_minus4,                 bitstream);
    len+=ue_v ((MS_S8*)("SPS: pic_order_cnt_type"),                      sps->pic_order_cnt_type,                        bitstream);

    if (sps->pic_order_cnt_type == 0)
        len+=ue_v ((MS_S8*)("SPS: log2_max_pic_order_cnt_lsb_minus4"),     sps->log2_max_pic_order_cnt_lsb_minus4,         bitstream);
    else if (sps->pic_order_cnt_type == 1)
    {
        len+=u_1  ((MS_S8*)("SPS: delta_pic_order_always_zero_flag"),        sps->delta_pic_order_always_zero_flag,          bitstream);
        len+=se_v ((MS_S8*)("SPS: offset_for_non_ref_pic"),                  sps->offset_for_non_ref_pic,                    bitstream);
        len+=se_v ((MS_S8*)("SPS: offset_for_top_to_bottom_field"),          sps->offset_for_top_to_bottom_field,            bitstream);
        len+=ue_v ((MS_S8*)("SPS: num_ref_frames_in_pic_order_cnt_cycle"),   sps->num_ref_frames_in_pic_order_cnt_cycle,     bitstream);
        for (i=0; i<sps->num_ref_frames_in_pic_order_cnt_cycle; i++)
            len+=se_v ((MS_S8*)("SPS: offset_for_ref_frame"),                  sps->offset_for_ref_frame[i],                      bitstream);
    }
    len+=ue_v ((MS_S8*)("SPS: num_ref_frames"),                          sps->num_ref_frames,                            bitstream);
    len+=u_1  ((MS_S8*)("SPS: gaps_in_frame_num_value_allowed_flag"),    sps->gaps_in_frame_num_value_allowed_flag,      bitstream);
    len+=ue_v ((MS_S8*)("SPS: pic_width_in_mbs_minus1"),                 sps->pic_width_in_mbs_minus1,                   bitstream);
    len+=ue_v ((MS_S8*)("SPS: pic_height_in_map_units_minus1"),          sps->pic_height_in_map_units_minus1,            bitstream);
    len+=u_1  ((MS_S8*)("SPS: frame_mbs_only_flag"),                     sps->frame_mbs_only_flag,                       bitstream);
    if (!sps->frame_mbs_only_flag)
    {
        len+=u_1  ((MS_S8*)("SPS: mb_adaptive_frame_field_flag"),            sps->mb_adaptive_frame_field_flag,              bitstream);
    }
    len+=u_1  ((MS_S8*)("SPS: direct_8x8_inference_flag"),               sps->direct_8x8_inference_flag,                 bitstream);

    len+=u_1  ((MS_S8*)("SPS: frame_cropping_flag"),                      sps->frame_cropping_flag,                       bitstream);

    if (sps->frame_cropping_flag)
    {
        len+=ue_v ((MS_S8*)("SPS: frame_cropping_rect_left_offset"),          sps->frame_cropping_rect_left_offset,           bitstream);
        len+=ue_v ((MS_S8*)("SPS: frame_cropping_rect_right_offset"),         sps->frame_cropping_rect_right_offset,          bitstream);
        len+=ue_v ((MS_S8*)("SPS: frame_cropping_rect_top_offset"),           sps->frame_cropping_rect_top_offset,            bitstream);
        len+=ue_v ((MS_S8*)("SPS: frame_cropping_rect_bottom_offset"),        sps->frame_cropping_rect_bottom_offset,         bitstream);
    }

    len+=u_1  ((MS_S8*)("SPS: vui_parameters_present_flag"),             sps->vui_parameters_present_flag,               bitstream);

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
 *    MS_S32 _GeneratePic_parameter_set_rbsp (pic_parameter_set_rbsp_t *sps, MS_U8 *rbsp);
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

static MS_S32 _GeneratePic_parameter_set_rbsp (MFE_CONFIG* pConfig, pic_parameter_set_rbsp_t *pps, MS_U8 *rbsp)
{
    Bitstream stream, *bitstream = &stream;
    MS_S32 len = 0, LenInBytes;
//    MS_S32 profile_idc;

    MS_ASSERT (rbsp != NULL);

    // .. and use the rbsp provided (or allocated above) for the data
    bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

    pps->pic_order_present_flag = pConfig->ctxH264Info.pic_order_present_flag;

    len+=ue_v ((MS_S8*)("PPS: pic_parameter_set_id"),                    pps->pic_parameter_set_id,                      bitstream);
    len+=ue_v ((MS_S8*)("PPS: seq_parameter_set_id"),                    pps->seq_parameter_set_id,                      bitstream);
    len+=u_1  ((MS_S8*)("PPS: entropy_coding_mode_flag"),                pps->entropy_coding_mode_flag,                  bitstream);
    len+=u_1  ((MS_S8*)("PPS: pic_order_present_flag"),                  pps->pic_order_present_flag,                    bitstream);
    len+=ue_v ((MS_S8*)("PPS: num_slice_groups_minus1"),                 pps->num_slice_groups_minus1,                   bitstream);
    MS_ASSERT(pps->num_slice_groups_minus1 == 0);

    len+=ue_v ((MS_S8*)("PPS: num_ref_idx_l0_active_minus1"),             pps->num_ref_idx_l0_active_minus1,              bitstream);
    len+=ue_v ((MS_S8*)("PPS: num_ref_idx_l1_active_minus1"),             pps->num_ref_idx_l1_active_minus1,              bitstream);
    len+=u_1  ((MS_S8*)("PPS: weighted_pred_flag"),                       pps->weighted_pred_flag,                        bitstream);
    len+=u_v  (2, (MS_S8*)("PPS: weighted_bipred_idc"),                   pps->weighted_bipred_idc,                       bitstream);
    len+=se_v ((MS_S8*)("PPS: pic_init_qp_minus26"),                      pps->pic_init_qp_minus26,                       bitstream);
    len+=se_v ((MS_S8*)("PPS: pic_init_qs_minus26"),                      pps->pic_init_qs_minus26,                       bitstream);

//    profile_idc = pConfig->ctxH264Info.ProfileIDC;//IdentifyProfile();
    len+=se_v ((MS_S8*)("PPS: chroma_qp_index_offset"),                 pps->chroma_qp_index_offset,                    bitstream);

    len+=u_1  ((MS_S8*)("PPS: deblocking_filter_control_present_flag"),   pps->deblocking_filter_control_present_flag,    bitstream);
    len+=u_1  ((MS_S8*)("PPS: constrained_intra_pred_flag"),              pps->constrained_intra_pred_flag,               bitstream);
    len+=u_1  ((MS_S8*)("PPS: redundant_pic_cnt_present_flag"),           pps->redundant_pic_cnt_present_flag,            bitstream);

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
static MS_S32 _GenerateVUI_parameters_rbsp(seq_parameter_set_rbsp_t *sps, Bitstream *bitstream)
{
    MS_S32 len=0;
    vui_seq_parameters_t *vui_seq_parameters = &(sps->vui_seq_parameters);

    len+=u_1 ((MS_S8*)("VUI: aspect_ratio_info_present_flag"), vui_seq_parameters->aspect_ratio_info_present_flag, bitstream);
    if (vui_seq_parameters->aspect_ratio_info_present_flag)
    {
        len+=u_v (8,(MS_S8*)("VUI: aspect_ratio_idc"), vui_seq_parameters->aspect_ratio_idc, bitstream);
        if (vui_seq_parameters->aspect_ratio_idc == 255)
        {
            len+=u_v (16,(MS_S8*)("VUI: sar_width"),  vui_seq_parameters->sar_width,  bitstream);
            len+=u_v (16,(MS_S8*)("VUI: sar_height"), vui_seq_parameters->sar_height, bitstream);
        }
    }

    len+=u_1 ((MS_S8*)("VUI: overscan_info_present_flag"), vui_seq_parameters->overscan_info_present_flag, bitstream);
    if (vui_seq_parameters->overscan_info_present_flag)
    {
        len+=u_1 ((MS_S8*)("VUI: overscan_appropriate_flag"), vui_seq_parameters->overscan_appropriate_flag, bitstream);
    }

    len+=u_1 ((MS_S8*)("VUI: video_signal_type_present_flag"), vui_seq_parameters->video_signal_type_present_flag, bitstream);
    if (vui_seq_parameters->video_signal_type_present_flag)
    {
        len+=u_v (3,(MS_S8*)("VUI: video_format"), vui_seq_parameters->video_format, bitstream);
        len+=u_1 ((MS_S8*)("VUI: video_full_range_flag"), vui_seq_parameters->video_full_range_flag, bitstream);
        len+=u_1 ((MS_S8*)("VUI: colour_description_present_flag"), vui_seq_parameters->colour_description_present_flag, bitstream);
        if (vui_seq_parameters->colour_description_present_flag)
        {
            len+=u_v (8,(MS_S8*)("VUI: colour_primaries"), vui_seq_parameters->colour_primaries, bitstream);
            len+=u_v (8,(MS_S8*)("VUI: transfer_characteristics"), vui_seq_parameters->transfer_characteristics, bitstream);
            len+=u_v (8,(MS_S8*)("VUI: matrix_coefficients"), vui_seq_parameters->matrix_coefficients, bitstream);
        }
    }

    len+=u_1 ((MS_S8*)("VUI: chroma_loc_info_present_flag"), vui_seq_parameters->chroma_location_info_present_flag, bitstream);
    if (vui_seq_parameters->chroma_location_info_present_flag)
    {
        len+=ue_v ((MS_S8*)("VUI: chroma_sample_loc_type_top_field"), vui_seq_parameters->chroma_sample_loc_type_top_field, bitstream);
        len+=ue_v ((MS_S8*)("VUI: chroma_sample_loc_type_bottom_field"), vui_seq_parameters->chroma_sample_loc_type_bottom_field, bitstream);
    }

    len+=u_1 ((MS_S8*)("VUI: timing_info_present_flag"), vui_seq_parameters->timing_info_present_flag, bitstream);
    // timing parameters
    if (vui_seq_parameters->timing_info_present_flag)
    {
        len+=u_v (32,(MS_S8*)("VUI: num_units_in_tick"),  vui_seq_parameters->num_units_in_tick, bitstream);
        len+=u_v (32,(MS_S8*)("VUI: time_scale"),         vui_seq_parameters->time_scale, bitstream);
        len+=u_1 ((MS_S8*)("VUI: fixed_frame_rate_flag"), vui_seq_parameters->fixed_frame_rate_flag, bitstream);
    }
    // end of timing parameters
    // nal_hrd_parameters_present_flag
    len+=u_1 ((MS_S8*)("VUI: nal_hrd_parameters_present_flag"), vui_seq_parameters->nal_hrd_parameters_present_flag, bitstream);
    MS_ASSERT(vui_seq_parameters->nal_hrd_parameters_present_flag==0);
//     if ( vui_seq_parameters->nal_hrd_parameters_present_flag )
//     {
//         len += WriteHRDParameters(sps, bitstream);
//     }
    // vcl_hrd_parameters_present_flag
    len+=u_1 ((MS_S8*)("VUI: vcl_hrd_parameters_present_flag"), vui_seq_parameters->vcl_hrd_parameters_present_flag, bitstream);
    MS_ASSERT(vui_seq_parameters->vcl_hrd_parameters_present_flag==0);
//     if ( vui_seq_parameters->vcl_hrd_parameters_present_flag )
//     {
//         len += WriteHRDParameters(sps, bitstream);
//     }
    if ( vui_seq_parameters->nal_hrd_parameters_present_flag || vui_seq_parameters->vcl_hrd_parameters_present_flag )
    {
        len+=u_1 ((MS_S8*)("VUI: low_delay_hrd_flag"), vui_seq_parameters->low_delay_hrd_flag, bitstream );
    }
    len+=u_1 ((MS_S8*)("VUI: pic_struct_present_flag"), vui_seq_parameters->pic_struct_present_flag, bitstream);

    len+=u_1 ((MS_S8*)("VUI: bitstream_restriction_flag"), vui_seq_parameters->bitstream_restriction_flag, bitstream);
    if (vui_seq_parameters->bitstream_restriction_flag)
    {
        len+=u_1  ((MS_S8*)("VUI: motion_vectors_over_pic_boundaries_flag"), vui_seq_parameters->motion_vectors_over_pic_boundaries_flag, bitstream);
        len+=ue_v ((MS_S8*)("VUI: max_bytes_per_pic_denom"), vui_seq_parameters->max_bytes_per_pic_denom, bitstream);
        len+=ue_v ((MS_S8*)("VUI: max_bits_per_mb_denom"), vui_seq_parameters->max_bits_per_mb_denom, bitstream);
        len+=ue_v ((MS_S8*)("VUI: log2_max_mv_length_horizontal"), vui_seq_parameters->log2_max_mv_length_horizontal, bitstream);
        len+=ue_v ((MS_S8*)("VUI: log2_max_mv_length_vertical"), vui_seq_parameters->log2_max_mv_length_vertical, bitstream);
        len+=ue_v ((MS_S8*)("VUI: num_reorder_frames"), vui_seq_parameters->num_reorder_frames, bitstream);
        len+=ue_v ((MS_S8*)("VUI: max_dec_frame_buffering"), vui_seq_parameters->max_dec_frame_buffering, bitstream);
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
  MS_U32          SchedSelIdx;
  hrd_parameters_t     *nal_hrd = &(sps->vui_seq_parameters.nal_hrd_parameters);
  hrd_parameters_t     *vcl_hrd = &(sps->vui_seq_parameters.vcl_hrd_parameters);
  vui_seq_parameters_t *vui     = &(sps->vui_seq_parameters);

  vui->aspect_ratio_info_present_flag      = (MS_BOOL) (pConfig->setVUI_aspect_ratio_info_present_flag == TRUE);
  vui->aspect_ratio_idc                    = (MS_U32) 1;
  vui->sar_width                           = (MS_U32) 0;
  vui->sar_height                          = (MS_U32) 0;

  if (pConfig->setVUI_aspect_ratio_info_present_flag == TRUE)
  {
    vui->aspect_ratio_idc                  = (MS_U32) pConfig->setVUI_aspect_ratio_idc;
    vui->sar_width                         = (MS_U32) pConfig->setVUI_sar_width;
    vui->sar_height                        = (MS_U32) pConfig->setVUI_sar_height;
  }

  vui->overscan_info_present_flag          = (MS_BOOL) 0;
  vui->overscan_appropriate_flag           = (MS_BOOL) 0;
  vui->video_signal_type_present_flag      = (MS_BOOL) 0;
  vui->video_format                        = (MS_U32) 5;
  vui->video_full_range_flag               = (MS_BOOL) 0;
  vui->colour_description_present_flag     = (MS_BOOL) 0;
  vui->colour_primaries                    = (MS_U32) 2;
  vui->transfer_characteristics            = (MS_U32) 2;
  vui->matrix_coefficients                 = (MS_U32) 2;
  vui->chroma_location_info_present_flag   = (MS_BOOL) 0;
  vui->chroma_sample_loc_type_top_field    = (MS_U32) 0;
  vui->chroma_sample_loc_type_bottom_field = (MS_U32) 0;
  vui->timing_info_present_flag            = (MS_BOOL) TRUE;
  switch (pConfig->FrameRatex100) {
  case 3000:
	  vui->num_units_in_tick = (MS_U32) 1;
	  vui->time_scale        = (MS_U32) 60;
	  break;
  case 1500:
	  vui->num_units_in_tick = (MS_U32) 1;
	  vui->time_scale        = (MS_U32) 30;
	  break;
  case 2500:
	  vui->num_units_in_tick = (MS_U32) 1;
	  vui->time_scale        = (MS_U32) 50;
	  break;
  case 2997:
	  vui->num_units_in_tick = (MS_U32) 1001;
	  vui->time_scale        = (MS_U32) 60000;
	  break;
  default:
	  vui->num_units_in_tick = (MS_U32) 50;
	  vui->time_scale        = (MS_U32) pConfig->FrameRatex100;
	  //MS_ASSERT(0);
  }
  vui->fixed_frame_rate_flag               = (MS_BOOL) FALSE;

  // NAL HRD parameters
  vui->nal_hrd_parameters_present_flag             = (MS_BOOL) 0;
  nal_hrd->cpb_cnt_minus1                          = (MS_U32) 0;
  nal_hrd->bit_rate_scale                          = (MS_U32) 0;
  nal_hrd->cpb_size_scale                          = (MS_U32) 0;
  for ( SchedSelIdx = 0; SchedSelIdx <= nal_hrd->cpb_cnt_minus1; SchedSelIdx++ )
  {
    nal_hrd->bit_rate_value_minus1[SchedSelIdx]    = (MS_U32) 0;
    nal_hrd->cpb_size_value_minus1[SchedSelIdx]    = (MS_U32) 0;
    nal_hrd->cbr_flag[SchedSelIdx]             = (MS_U32) 0;
  }
  nal_hrd->initial_cpb_removal_delay_length_minus1 = (MS_U32) 23;
  nal_hrd->cpb_removal_delay_length_minus1         = (MS_U32) 23;
  nal_hrd->dpb_output_delay_length_minus1          = (MS_U32) 23;
  nal_hrd->time_offset_length                      = (MS_U32) 24;

  // VCL HRD parameters
  vui->vcl_hrd_parameters_present_flag             = (MS_BOOL) 0;
  vcl_hrd->cpb_cnt_minus1                          = (MS_U32) 0;
  vcl_hrd->bit_rate_scale                          = (MS_U32) 0;
  vcl_hrd->cpb_size_scale                          = (MS_U32) 0;
  for ( SchedSelIdx = 0; SchedSelIdx <= vcl_hrd->cpb_cnt_minus1; SchedSelIdx++ )
  {
    vcl_hrd->bit_rate_value_minus1[SchedSelIdx]    = (MS_U32) 0;
    vcl_hrd->cpb_size_value_minus1[SchedSelIdx]    = (MS_U32) 0;
    vcl_hrd->cbr_flag[SchedSelIdx]             = (MS_U32) 0;
  }
  vcl_hrd->initial_cpb_removal_delay_length_minus1 = (MS_U32) 23;
  vcl_hrd->cpb_removal_delay_length_minus1         = (MS_U32) 23;
  vcl_hrd->dpb_output_delay_length_minus1          = (MS_U32) 23;
  vcl_hrd->time_offset_length                      = (MS_U32) 24;

  vui->low_delay_hrd_flag                      = (MS_BOOL) 0;
  vui->pic_struct_present_flag                 = (MS_BOOL) 0;
  vui->bitstream_restriction_flag              = (MS_BOOL) 1;
  vui->motion_vectors_over_pic_boundaries_flag = (MS_BOOL) 1;
  vui->max_bytes_per_pic_denom                 = (MS_U32) 0;
  vui->max_bits_per_mb_denom                   = (MS_U32) 0;
  vui->log2_max_mv_length_horizontal           = (MS_U32) 7;
  vui->log2_max_mv_length_vertical             = (MS_U32) 7;
  vui->num_reorder_frames                      = (MS_U32) 0;
  vui->max_dec_frame_buffering                 = (MS_U32) pConfig->ctxH264Info.num_ref_frames;
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
static MS_S32 WriteAnnexbNALU(OutStream* pStream, NALU_t *n)
{
	MS_U32 i, leftbits;
	MS_S32 BitsWritten = 0;
	MS_ASSERT (n != NULL);
	if (n == NULL)
	{
		ms_dprintk(DRV_L4, "[WriteAnnexbNALU] n is null\n");
		return -1;
	}

	MS_ASSERT (n->forbidden_bit == 0);
	MS_ASSERT (n->startcodeprefix_len == 3 || n->startcodeprefix_len == 4);

	// printf ("WriteAnnexbNALU: writing %d bytes w/ startcode_len %d\n"), n->len+1, n->startcodeprefix_len);
	if (n->startcodeprefix_len > 3)
	{
		osPutBits(pStream, 0, 8, (MS_S8*)("start-code 0"));
		BitsWritten =+ 8;
	}
	osPutBits(pStream, 0, 8, (MS_S8*)("start-code 0"));
	osPutBits(pStream, 0, 8, (MS_S8*)("start-code 0"));
	osPutBits(pStream, 1, 8, (MS_S8*)("start-code 1"));
	BitsWritten += 24;

	n->buf[0] = (MS_U8) ((n->forbidden_bit << 7) | (n->nal_reference_idc << 5) | n->nal_unit_type);
	// printf ("First Byte %x, nal_ref_idc %x, nal_unit_type %d\n"), n->buf[0], n->nal_reference_idc, n->nal_unit_type);

	for (i=0; i<n->len/8; i++)
		osPutBits(pStream, n->buf[i], 8, (MS_S8*)("nalu-body"));
	leftbits = n->len - (i*8);
	if (leftbits>0)
		osPutBits(pStream, n->buf[i]&(0xFF>>(8-leftbits)), leftbits, (MS_S8*)("nalu-body (last-bits)"));
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
static MS_S32 get_picture_type(MFE_CONFIG* pConfig)
{
	// set this value to zero for transmission without signaling
	// that the whole picture has the same slice type
	const MS_S32 same_slicetype_for_whole_frame = 0;//5;

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
static MS_S32 ref_pic_list_reordering(MFE_CONFIG* pConfig, Bitstream *bitstream)
{
	MS_S32 len=0;

	if (pConfig->vopPredType!=I_VOP)
	{
		len += u_1 ((MS_S8*)("SH: ref_pic_list_reordering_flag_l0"), /*currSlice->ref_pic_list_reordering_flag_l0*/0, bitstream);
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
static MS_S32 dec_ref_pic_marking(MFE_CONFIG* pConfig, Bitstream *bitstream)
{
	H264INFO* pInfo = &pConfig->ctxH264Info;

	MS_S32 len=0;

	if (pInfo->idr_flag)
	{
		len += u_1((MS_S8*)("SH: no_output_of_prior_pics_flag"), /*img->no_output_of_prior_pics_flag*/0, bitstream);
		len += u_1((MS_S8*)("SH: long_term_reference_flag"), /*img->long_term_reference_flag*/0, bitstream);
	}
	else
	{
		len += u_1((MS_S8*)("SH: adaptive_ref_pic_buffering_flag"), /*img->adaptive_ref_pic_buffering_flag*/0, bitstream);
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
static MS_S32 SliceHeader(MFE_CONFIG* pConfig, MS_U8 *rbsp)
{
    Bitstream stream, *bitstream = &stream;
    H264INFO* pInfo = &pConfig->ctxH264Info;
    MS_S32 len = 0;
    MS_U32 field_pic_flag = 0, bottom_field_flag = 0;

    bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

    len  = ue_v((MS_S8*)("SH: first_mb_in_slice"), /*img->current_mb_nr*/0,   bitstream);
    len += ue_v((MS_S8*)("SH: slice_type"),        get_picture_type(pConfig),   bitstream);
    len += ue_v((MS_S8*)("SH: pic_parameter_set_id"), pConfig->active_pps->pic_parameter_set_id ,bitstream);
    len += u_v (pInfo->log2_max_frame_num_minus4 + 4, (MS_S8*)("SH: frame_num"), pInfo->frame_num, bitstream);

    if (!pConfig->active_sps->frame_mbs_only_flag)
    {
        // field_pic_flag    u(1)
        field_pic_flag = (pInfo->structure ==TOP_FIELD || pInfo->structure ==BOTTOM_FIELD)?1:0;
        //MS_ASSERT( field_pic_flag == img->fld_flag );
        len += u_1((MS_S8*)("SH: field_pic_flag"), field_pic_flag, bitstream);

        if (field_pic_flag)
        {
            //bottom_field_flag     u(1)
            bottom_field_flag = (pInfo->structure == BOTTOM_FIELD)?1:0;
            len += u_1((MS_S8*)("SH: bottom_field_flag"), bottom_field_flag ,bitstream);
        }
    }

    if (pInfo->idr_flag)
    {
        // idr_pic_id
        len += ue_v ((MS_S8*)("SH: idr_pic_id"), (pInfo->number % 2), bitstream);
    }

	if (pInfo->pic_order_cnt_type == 0)
	{
		MS_ASSERT(0);
/*
		if (active_sps->frame_mbs_only_flag)
		{
			img->pic_order_cnt_lsb = (img->toppoc & ~((((MS_U32)(-1)) << (log2_max_pic_order_cnt_lsb_minus4+4))) );
		}
		else
		{
			if (!field_pic_flag || img->structure == TOP_FIELD)
				img->pic_order_cnt_lsb = (img->toppoc & ~((((MS_U32)(-1)) << (log2_max_pic_order_cnt_lsb_minus4+4))) );
			else if ( img->structure == BOTTOM_FIELD )
				img->pic_order_cnt_lsb = (img->bottompoc & ~((((MS_U32)(-1)) << (log2_max_pic_order_cnt_lsb_minus4+4))) );
		}

		len += u_v (log2_max_pic_order_cnt_lsb_minus4+4, "SH: pic_order_cnt_lsb"), img->pic_order_cnt_lsb, bitstream);

		if (img->pic_order_present_flag && !field_pic_flag)
		{
			len += se_v ("SH: delta_pic_order_cnt_bottom"), img->delta_pic_order_cnt_bottom, bitstream);
		}
*/
	}
	if (pInfo->pic_order_cnt_type == 1 /*&& !img->delta_pic_order_always_zero_flag*/)
	{
		MS_ASSERT(0);
/*
		len += se_v ("SH: delta_pic_order_cnt[0]"), img->delta_pic_order_cnt[0], bitstream);

		if (img->pic_order_present_flag && !field_pic_flag)
		{
			len += se_v ("SH: delta_pic_order_cnt[1]"), img->delta_pic_order_cnt[1], bitstream);
		}
*/
	}

	if (pConfig->vopPredType == P_VOP)
	{
		MS_S32 override_flag;

		//this is for MFE fixed setting override_flag
		override_flag = (pInfo->num_ref_idx_l0_active != (pConfig->active_pps->num_ref_idx_l0_active_minus1 +1)) ? 1 : 0;
        ms_dprintk(DRV_L3,"override_flag = %d, num_ref_idx_l0_active = %d, num_ref_idx_l0_active_minus1 = %d\n",
            (int)override_flag, (int)pInfo->num_ref_idx_l0_active, (int)pConfig->active_pps->num_ref_idx_l0_active_minus1);
		len +=  u_1((MS_S8*)("SH: num_ref_idx_active_override_flag"), override_flag, bitstream);
		if (override_flag)
		{
			len += ue_v((MS_S8*)("SH: num_ref_idx_l0_active_minus1"), pInfo->num_ref_idx_l0_active-1, bitstream);
		}
	}
	len += ref_pic_list_reordering(pConfig, bitstream);

	if (pInfo->nal_ref_idc)
		len += dec_ref_pic_marking(pConfig, bitstream);

    if(pConfig->UseCABAC && pConfig->vopPredType != I_VOP)
    {
        len += ue_v((MS_S8*)("SH: cabac_init_idc"), 0, bitstream);
    }

	len += se_v((MS_S8*)("SH: slice_qp_delta"), (pInfo->intQP - 26 - pConfig->active_pps->pic_init_qp_minus26), bitstream);

	if (pConfig->active_pps->deblocking_filter_control_present_flag)
	{
		len += ue_v((MS_S8*)("SH: disable_deblocking_filter_idc"), pInfo->nDeblockIDC, bitstream);  // Turn loop filter on/off on slice basis
		if (pInfo->nDeblockIDC!=1)
		{
			len += se_v ((MS_S8*)("SH: slice_alpha_c0_offset_div2"), pInfo->nDeblockAlpha / 2, bitstream);
			len += se_v ((MS_S8*)("SH: slice_beta_offset_div2"), pInfo->nDeblockBeta / 2, bitstream);
		}
	}

    //The Slice header must be byte aligned iff enable CABAC.
    if(pConfig->UseCABAC && (bitstream->bits_to_go!=8)) {
        len+=bitstream->bits_to_go;
        writeVlcByteAlign(bitstream);
    }
    // Put final bits
	if (bitstream->bits_to_go!=8)
		bitstream->streamBuffer[bitstream->byte_pos] = bitstream->byte_buf;

	return len;
}

/*!
********************************************************************************************
* \brief
*    Write dummy(P) slice header, and also dummy slice data
*
* \return
*    number of bits used
********************************************************************************************
*/
static MS_S32 DummySliceHeaderAndData(MFE_CONFIG* pConfig, MS_U8 *rbsp)
{
    Bitstream stream, *bitstream = &stream;
    H264INFO* pInfo = &pConfig->ctxH264Info;
    MS_S32 len = 0;
    MS_S32 nMbCount = ((pConfig->nDispWidth+15)>>4) * ((pConfig->nDispHeight+15)>>4);

    bitstream->streamBuffer = rbsp;
    bitstream->byte_pos = 0;
    bitstream->byte_buf = 0;
    bitstream->bits_to_go = 8;
    bitstream->zerocount = 0;

    len  = ue_v((MS_S8*)("SH: first_mb_in_slice"), 0, bitstream);
    len += ue_v((MS_S8*)("SH: slice_type"), 0, bitstream); // Forced P-slice
    len += ue_v((MS_S8*)("SH: pic_parameter_set_id"), pConfig->active_pps->pic_parameter_set_id, bitstream);
    len += u_v (pInfo->log2_max_frame_num_minus4 + 4, (MS_S8*)("SH: frame_num"), pInfo->frame_num, bitstream);

    len += u_1 ((MS_S8*)("SH: num_ref_idx_active_override_flag"), 0, bitstream); // Forced to 0
    len += u_1 ((MS_S8*)("SH: ref_pic_list_reordering_flag_l0"), 0, bitstream);
    len += se_v((MS_S8*)("SH: slice_qp_delta"), (/*pInfo->intStep*/26 - 26 - pConfig->active_pps->pic_init_qp_minus26), bitstream); // Forced to 26

    if (pConfig->active_pps->deblocking_filter_control_present_flag)
    {
        len += ue_v((MS_S8*)("SH: disable_deblocking_filter_idc"), pInfo->nDeblockIDC, bitstream);  // Turn loop filter on/off on slice basis
        if (pInfo->nDeblockIDC!=1)
        {
            len += se_v ((MS_S8*)("SH: slice_alpha_c0_offset_div2"), pInfo->nDeblockAlpha / 2, bitstream);
            len += se_v ((MS_S8*)("SH: slice_beta_offset_div2"), pInfo->nDeblockBeta / 2, bitstream);
        }
    }

    // Above is slice_header()
    // Below is slice_data()
    len += ue_v((MS_S8*)("SD: mb_skip_run"), nMbCount, bitstream);

    // rbsp_trailing_bits()
    len += u_1 ((MS_S8*)("SD: rbsp_stop_one_bit"), 1, bitstream);
    if (bitstream->bits_to_go!=8)
        len += u_v(bitstream->bits_to_go, (MS_S8*)("SD: rbsp_alignment_zero_bit"), 0, bitstream);

    return len;
}

MS_U8 get_NALU_1stbyte(H264INFO *pInfo, MS_U8 vopPredType)
{
    MS_S32 nal_unit_type, nal_reference_idc, forbidden_bit;
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
    return (MS_U8) ((forbidden_bit << 7) | (nal_reference_idc << 5) | nal_unit_type);
}

void codeSPSPPS(MFE_CONFIG* pConfig,OutStream* pStream)
{
	WriteAnnexbNALU(pStream, &pConfig->NaluStruct[STATIC_SPS]);
	WriteAnnexbNALU(pStream, &pConfig->NaluStruct[STATIC_PPS]);
}

MS_S32 codeH264ConfigHeaders(MFE_CONFIG *pConfig,MS_U32* size_of_sps,MS_U32* size_of_pps)
{
	OutStream* pStream = &pConfig->m_OutStream;

	osReset(pStream);

	// SPS, PPS
	//codeSPSPPS(pStream);
	*size_of_sps = WriteAnnexbNALU(pStream, &pConfig->NaluStruct[STATIC_SPS]) / 8;
	*size_of_pps = WriteAnnexbNALU(pStream, &pConfig->NaluStruct[STATIC_PPS]) / 8;
	// Finalize
	osFlushAll(pStream);

	// The generated bytes start from pStream->m_pbFrameBuffer and with pStream->m_nByteCount bytes.
	return pStream->m_nByteCount;
}

void codeSliceHeader(MFE_CONFIG* pConfig, OutStream* pStream)
{
	NALU_t naluBuf, *nalu = &naluBuf;
	H264INFO* pInfo = &pConfig->ctxH264Info;
       MS_U8 rbsp[MAXRBSPSIZE];
	MS_U8 Byte1;

	nalu->buf = rbsp;
	nalu->len = 8 + SliceHeader(pConfig, &rbsp[1]);	// rbsp[0] is reserved for nal_unit_idc and nal_unit_type

    nalu->startcodeprefix_len = 4;

	Byte1 = get_NALU_1stbyte(pInfo, pConfig->vopPredType);
	nalu->nal_unit_type = (NaluType)(Byte1&0x1f);
	nalu->nal_reference_idc = (NalRefIdc)((Byte1>>5)&0x3);
	nalu->forbidden_bit = 0;

	WriteAnnexbNALU(pStream, nalu);
}

void codeDummySliceHeader(MFE_CONFIG* pConfig, OutStream* pStream)
{
    NALU_t naluBuf, *nalu = &naluBuf;
    MS_U8 rbsp[MAXRBSPSIZE];
    MS_U8 Byte1;

    nalu->buf = rbsp;
    nalu->len = 8 + DummySliceHeaderAndData(pConfig, &rbsp[1]); // rbsp[0] is reserved for nal_unit_idc and nal_unit_type

    nalu->startcodeprefix_len = 4;

    Byte1 = (MS_U8) ((/*forbidden_bit*/0 << 7) | (/*nal_reference_idc*/NALU_PRIORITY_DISPOSABLE << 5) | /*nal_unit_type*/NALU_TYPE_SLICE);
    nalu->nal_unit_type = Byte1&0x1f;
    nalu->nal_reference_idc = (Byte1>>5)&0x3;
    nalu->forbidden_bit = 0;

    WriteAnnexbNALU(pStream, nalu);
}