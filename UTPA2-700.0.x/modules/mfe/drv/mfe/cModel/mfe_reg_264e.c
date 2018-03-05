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



#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"


#include "parsetcommon.h"
#include "parset.h"
#include "msRateCtrl.h"
#include "BufMng.h"
#if defined(WIN32)
	#include "sigdump/sigdump_api.h"
#endif
#include "mfe_reg.h"
// Sync with cModel QExp.h
#define CLOCK_GATING	// Enable clock gating
#define FME_PIPELINE_OPEN	// Enable David's FME speedup version
#define CHECK_P8x8_BOUND_RECT

extern void MHal_MFE_Enable_MIU_Protection(int MIU_TEST_MODE,MFE_CONFIG* pConfig);
void OutputSwCfg1_H264(int nFrmNum, MFE_CONFIG* pConfig);
extern void MHal_MFE_Enable_MIU_Protection(int MIU_TEST_MODE,MFE_CONFIG* pConfig);

void mfeH264_Init(MFE_CONFIG *pConfig)
{
	int i;
	H264INFO* pInfo = &pConfig->ctxH264Info;
	int FrameSizeInMbs = (pConfig->nBufWidth>>4) * (pConfig->nBufHeight>>4);

	memset(pInfo, 0, sizeof(H264INFO));

	pInfo->ProfileIDC = 66;	// Baseline
	pInfo->LevelIDC = 30;	// Up to 720x576x25 or 720x480x30
	pInfo->PicInterlace = !pConfig->bInterlace ? FRAME_CODING : FIELD_CODING;
	if (pInfo->PicInterlace==FIELD_CODING) {
		pInfo->ProfileIDC = 77;	// Forced to be main profile
		MFE_ASSERT((pConfig->nBufHeight&0x1F)==0);
	}

	pInfo->nMaxP8x8Count = FrameSizeInMbs;
	pInfo->log2_max_frame_num_minus4 = 1;

    // ME
    pInfo->num_ref_frames = 2;		// sequence-level
    pInfo->iSearchRangeForward = 32;
    pInfo->SEARCH_RANGE_Y = 16;
    pInfo->IME_ADAPTIVE_WINDOW = 1;
    pInfo->fme_precision = 2;	// 0--fullpel, 1--halfpel, 2--quarterpel
    pInfo->nMaxFmeMode = (FrameSizeInMbs>396) ? 1 : 2;
    pInfo->no_umv = 0;
    pInfo->InterSearch[0][0] = 1;	// PSKIP
    pInfo->InterSearch[0][BLOCK16x16] = 1;
    pInfo->InterSearch[0][BLOCK16x8] = 1;
    pInfo->InterSearch[0][BLOCK8x16] = 1;
    pInfo->InterSearch[0][BLOCK8x8] = 1;
    pInfo->InterSearch[0][BLOCK8x4] = 1;
    pInfo->InterSearch[0][BLOCK4x8] = 1;
    pInfo->InterSearch[0][BLOCK4x4] = 1;
	for (i=0; i<=BLOCK4x4; i++)
		pInfo->InterSearch[1][i] = 0;
    pInfo->num_ref_idx_l0_active_minus1 = pInfo->num_ref_frames-1;	// PPS info

    // DBF
    pInfo->bDeblockCtrlPresent = 0;
    pInfo->nDeblockIDC = 2;	// 0--all edges; 1--no edges; 2--edges inside slice boundary
							// NOTE: MFE does not support er_en enabled with DeblockIDC=0
	pInfo->nDeblockAlpha = 0;
	pInfo->nDeblockBeta = 0;

    // Frame type
	pInfo->pic_order_cnt_type = 2;	// HW only support 2
	pInfo->structure = FRAME;
    pInfo->idr_flag = 1;
    pInfo->nal_ref_idc = 1;
    pInfo->frame_num = 0;
    pInfo->number = 0;
    pInfo->num_ref_idx_l0_active = 0;
    pInfo->RefCount = 0;	// frame-level 0: 1ref, 1: 2ref
	pInfo->h264_mcc_offset[0] = pInfo->h264_mcc_offset[1] = 0;
    pInfo->intStep = 0;

	// Slice header
	if ((pInfo->PicInterlace==FRAME_CODING) /*&& (pInfo->MbInterlace==FRAME_CODING)*/)
	{
		pInfo->pic_order_present_flag = 0;
		pInfo->delta_pic_order_cnt_bottom = 0;
	}
	else
	{
		pInfo->pic_order_present_flag = 1;
		pInfo->delta_pic_order_cnt_bottom = 1;
	}

    // IEAP
	pInfo->ieap_last_mode = 8;
	pInfo->ieap_constraint_intra = 0;
	pInfo->ieap_ccest_en = !(pInfo->PicInterlace==FIELD_CODING) ? 1 : 0;
	pInfo->ieap_ccest_thr = pInfo->ieap_ccest_en ? (4-1) : 0;
	pInfo->ieap_drop_i16 = 0;

	// Rate control
	MfeDrvRateControlInit(pConfig);

	// FDC header initialization
	osCreate(&pConfig->m_OutStream);
	osSetWriteBuffer(&pConfig->m_OutStream, pConfig->m_FdcBuffer);

	// Intra-update initialization
	IntraUpdateInit(pConfig);

	// Prepare SPS, PPS
	GenerateSPSPPS(pConfig);

	// DPB initialization
	for (i=0; i<DPB_SIZE; i++)
		pInfo->dpb[i].is_used = 0;
}


#ifdef DRV_MFE_TIME_MEASURE
extern void MFE_MsOS_START_TIMER (void);
#endif
extern const unsigned char gBITMASK[8];
void mfeH264_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{
    H264INFO* pInfo = &pConfig->ctxH264Info;
    BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
    OutStream* pStream = &pConfig->m_OutStream;

    // This is done in msAPI_MFE_EnOneFrm()
    //mfeSetVopType(pConfig, pGopInfo); // Will set pConfig->vopPredType
#ifdef SPS_PPS_IN_EACH_I
    	pInfo->idr_flag = pConfig->vopPredType==I_VOP ? 1 : 0;
#else
      // Frame type
    if (pGopInfo->nCodingOrder==0)
    	pInfo->idr_flag = pConfig->vopPredType==I_VOP ? 1 : 0;
    else
    	pInfo->idr_flag = 0;
#endif
    pInfo->nal_ref_idc = (get_NALU_1stbyte(pInfo, pConfig->vopPredType)>>5)&0x3;
    if (pInfo->idr_flag)
    	pInfo->frame_num = 0;
    //pInfo->frame_num, pInfo->number
    if (pInfo->idr_flag)//(pConfig->vopPredType==I_VOP)
    	pInfo->num_ref_idx_l0_active = 0;
    else {
    	if (pInfo->num_ref_idx_l0_active<pInfo->num_ref_frames)
    		pInfo->num_ref_idx_l0_active++;
    }
    pInfo->RefCount = pConfig->vopPredType==I_VOP ? 0 : pInfo->num_ref_idx_l0_active;	// frame-level 0: 1ref, 1: 2ref
    pInfo->h264_mcc_offset[0] = pInfo->h264_mcc_offset[1] = 0;
    pInfo->intStep = cvbr_InitFrame(&pConfig->ctxRateControl, pConfig->vopPredType, 0);

    // Buffer management
    h264GetBufferAddr(pConfig);

    // Prepare header
    osReset(pStream);
    // SPS, PPS
    if (pGopInfo->nCodingOrder==0||pInfo->idr_flag) {
    	codeSPSPPS(pStream);
    }
    // First slice header
    codeSliceHeader(pConfig, pStream);
    // Finalize
    osFlushAll(pStream);
    pBitsInfo->ptr = pStream->m_pbFrameBuffer;
    pBitsInfo->len = pStream->m_nByteCount;
    if ((pStream->BC_nCumulativeBits&7)==0) {
    	pBitsInfo->bit_len = 0;
    	pBitsInfo->bits = 0;
    }
    else {
    	pBitsInfo->len--;
    	pBitsInfo->bit_len = pStream->BC_nCumulativeBits&7;
    	pBitsInfo->bits = pStream->m_pbFrameBuffer[pStream->BC_nCumulativeBits>>3] & gBITMASK[pBitsInfo->bit_len];
    }



#ifdef DRV_MFE_TIME_MEASURE
    //        MFE_MsOS_START_TIMER();
    //    timetick_start = jiffies; //MDrv_SYS_GetSyetemTime();
    //    ms_dprintk(DRV_L6, "[MFE_TIME_STR] timetick_str = %ld\n",timetick_start);
    //ftime(&tp1);
#endif

    //MODE 0: (Checking range > real range ) 	reg_mfe_s_marb_miu_bound_err = 0
    //MODE 1: (Checking range < real range ) 	reg_mfe_s_marb_miu_bound_err = 1
    //#define TEST_MIU_PROTECTION_MODE 0
    MHal_MFE_Enable_MIU_Protection(TEST_MIU_PROTECTION_MODE,pConfig);

    /*
    {	// DEBUG codes
    	int i;
    	ms_dprintk(DRV_L4,"FDC bitcount %d ==> ", pBitsInfo->len*8+pBitsInfo->bit_len);
    	for (i=0; i<pBitsInfo->len; i++) {
    		ms_dprintk(DRV_L4,"%02x ", pBitsInfo->ptr[i]);
    	}
    	ms_dprintk(DRV_L4,"%02x(msb %d bits)\n", pBitsInfo->bits, pBitsInfo->bit_len);
    }
    */
    //	printf("DumpAllReg before OutputSwCfg1_H264\n");
    // Set reg and start encoding
    //		DumpAllReg();
    OutputSwCfg1_H264(pGopInfo->nCodingOrder, pConfig);

    //	printf("DumpAllReg After delay OutputSwCfg1_H264\n");
    //		DumpAllReg();


}

void mfeH264_UpdateFrame(MFE_CONFIG *pConfig)
{
	H264INFO* pInfo = &pConfig->ctxH264Info;
	int max_frame_num = 1 << (pInfo->log2_max_frame_num_minus4 + 4);

	if (pConfig->vopPredType==I_VOP)
		IntraUpdateInit(pConfig);
	IntraUpdateFrame(pConfig);

	// Rate control
	MfeDrvRateControlUpdate(pConfig, 0);

	h264DpbHandling(pConfig);

	pInfo->frame_num++;
	pInfo->frame_num %= max_frame_num;
	pInfo->number++;
}

void OutputSwCfg1_H264(int nFrmNum, MFE_CONFIG* pConfig)
{
	int nTarWriteCount;
	int nRegWriteCount;
	int nTarFDCCount;
	int nRegFDCCount;

	H264INFO* pInfo = &pConfig->ctxH264Info;
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;
	BitsInfo *pBitsInfo = &pConfig->ctxBitsInfo;
	CVBRRateControl* rcCtx = &pConfig->ctxRateControl;

	void* pContext = (void*)pBitsInfo;

	//////////////////////////////////////////////////////////////////////////
	// Sequence-wide settings

	if (nFrmNum==0) {
		memset(&mfe_reg, 0, sizeof(MFE_REG));	// Initial
		mfe_reg.reg_mfe_g_enc_mode = REG_ENC_MODE_H264;
		mfe_reg.reg_mfe_g_pic_width = pConfig->nBufWidth;
		mfe_reg.reg_mfe_g_pic_height = (pInfo->PicInterlace==FIELD_CODING) ? (pConfig->nBufHeight>>1) : pConfig->nBufHeight;

		// Qtable
		mfe_reg.reg_mfe_g_qmode = 0;

		//Field Coding
		if (pInfo->PicInterlace==FIELD_CODING) {
			mfe_reg.reg_mfe_g_fldpic_en = 1;
		}

		// ME partition type
		mfe_reg.reg_mfe_s_me_16x16_disable = !pInfo->InterSearch[0][BLOCK16x16];
		mfe_reg.reg_mfe_s_me_8x8_disable = !pInfo->InterSearch[0][BLOCK8x8];
		mfe_reg.reg_mfe_s_me_16x8_disable = !pInfo->InterSearch[0][BLOCK16x8];
		mfe_reg.reg_mfe_s_me_8x16_disable = !pInfo->InterSearch[0][BLOCK8x16];
		mfe_reg.reg_mfe_s_me_8x4_disable  = !pInfo->InterSearch[0][BLOCK8x4];
		mfe_reg.reg_mfe_s_me_4x8_disable  = !pInfo->InterSearch[0][BLOCK4x8];
		mfe_reg.reg_mfe_s_me_4x4_disable  = !pInfo->InterSearch[0][BLOCK4x4];

		// MDC
		mfe_reg.reg_mfe_s_mbr_qp_cidx_offset = 0;//active_pps->chroma_qp_index_offset;
	}
	else {
		mfe_reg.reg_mfe_g_frame_start_sw = 0;
	}

	//////////////////////////////////////////////////////////////////////////
	// Frame-wide settings
#ifdef CLOCK_GATING
	 mfe_reg.reg16 = 0xffff;	// clock gating
#endif

	// Input buffer address: Must be 256-byte aligned.
	MFE_ASSERT((pBufInfo->m_nCurYAddr.miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nCurCAddr.miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefYAddr[0].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefYAddr[1].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefCAddr[0].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nRefCAddr[1].miuAddress&0xFF)==0);
	MFE_ASSERT((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress&0x7)==0);
	mfe_reg.reg_mfe_g_cur_y_adr_low  = (MFE_U16)((pBufInfo->m_nCurYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_cur_y_adr_high = (MFE_U16)(pBufInfo->m_nCurYAddr.miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_cur_c_adr_low  = (MFE_U16)((pBufInfo->m_nCurCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_cur_c_adr_high = (MFE_U16)(pBufInfo->m_nCurCAddr.miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_ref_y_adr0_low = (MFE_U16)((pBufInfo->m_nRefYAddr[0].miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_ref_y_adr0_high = (MFE_U16)(pBufInfo->m_nRefYAddr[0].miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_ref_y_adr1_low  = (MFE_U16)((pBufInfo->m_nRefYAddr[1].miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_ref_y_adr1_high = (MFE_U16)(pBufInfo->m_nRefYAddr[1].miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_ref_c_adr0_low  = (MFE_U16)((pBufInfo->m_nRefCAddr[0].miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_ref_c_adr0_high = (MFE_U16)(pBufInfo->m_nRefCAddr[0].miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_ref_c_adr1_low  = (MFE_U16)((pBufInfo->m_nRefCAddr[1].miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_ref_c_adr1_high = (MFE_U16)(pBufInfo->m_nRefCAddr[1].miuAddress>>(8+16));
#ifdef USE_CUR_AS_REC
	MFE_ASSERT(pBufInfo->m_nRecYAddr==pBufInfo->m_nCurYAddr);
	MFE_ASSERT(pBufInfo->m_nRecCAddr==pBufInfo->m_nCurCAddr);
#endif
	mfe_reg.reg_mfe_g_rec_y_adr_low  = (MFE_U16)((pBufInfo->m_nRecYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_rec_y_adr_high = (MFE_U16)(pBufInfo->m_nRecYAddr.miuAddress>>(8+16));
	mfe_reg.reg_mfe_g_rec_c_adr_low  = (MFE_U16)((pBufInfo->m_nRecCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg.reg_mfe_g_rec_c_adr_high = (MFE_U16)(pBufInfo->m_nRecCAddr.miuAddress>>(8+16));

	// Output buffers: Must be 8-byte aligned.
	mfe_reg.reg_mfe_s_bspobuf_sadr_low  = (MFE_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>3)&0xFFFF);
	mfe_reg.reg_mfe_s_bspobuf_sadr_high = (MFE_U16)(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>(3+16));
	mfe_reg.reg_mfe_s_bspobuf_eadr_low  = (MFE_U16)(((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>3)&0xFFFF);
	mfe_reg.reg_mfe_s_bspobuf_eadr_high = (MFE_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>(3+16));

	// GN
	mfe_reg.reg_mfe_s_gn_sadr_low  = (MFE_U16)((pBufInfo->m_nGNAddr.miuAddress>>3)&0xFFFF);
	mfe_reg.reg_mfe_s_gn_sadr_high = (MFE_U16)(pBufInfo->m_nGNAddr.miuAddress>>(3+16));

	mfe_reg.reg_mfe_g_frame_type = pConfig->vopPredType==I_VOP?0:(pConfig->vopPredType==P_VOP?1:2);
	mfe_reg.reg_mfe_g_ref_no = pInfo->RefCount==2 ? 1 : 0;

	// ME setting
	mfe_reg.reg_mfe_s_ime_mesr_max_addr = (pInfo->SEARCH_RANGE_Y==16 ? 95 : /*83*/85);//0x5f;
	mfe_reg.reg_mfe_s_ime_mesr_min_addr =(pInfo->SEARCH_RANGE_Y==16 ? 0 : /*16*/10);//0;
	mfe_reg.reg_mfe_s_ime_mvx_min = -(pInfo->iSearchRangeForward) + 32;	// Min X is -pVopMd->iSearchRangeForward
	mfe_reg.reg_mfe_s_ime_mvx_max = (pInfo->iSearchRangeForward-2) + 32;	// Max X is pVopMd->iSearchRangeForward-1
	mfe_reg.reg_mfe_s_ime_mvy_min = -pInfo->SEARCH_RANGE_Y + 16;					// Min Y
	mfe_reg.reg_mfe_s_ime_mvy_max = (pInfo->SEARCH_RANGE_Y==16 ? 15 : 8) + 16;		// Max Y
	mfe_reg.reg_mfe_s_ime_sr16 = (mfe_reg.reg_mfe_s_ime_mvx_min>=16 ? 1 : 0);
	mfe_reg.reg_mfe_s_ime_umv_disable = pInfo->no_umv;

	mfe_reg.reg_mfe_s_ime_ime_wait_fme = 1;
#ifdef FME_PIPELINE_OPEN
	mfe_reg.reg_mfe_s_fme_pipeline_on = mfe_reg.reg_mfe_s_ime_ime_wait_fme ? 1 : 0;
#else
	mfe_reg.reg_mfe_s_fme_pipeline_on = 0x0;
#endif

#ifdef CHECK_P8x8_BOUND_RECT
	mfe_reg.reg_mfe_s_ime_boundrect_en = (pInfo->ProfileIDC==66 && pInfo->LevelIDC<=30) ? 1 : 0;
#endif
	mfe_reg.reg_mfe_s_ime_h264_p8x8_ctrl_en = pInfo->nMaxP8x8Count<(mfe_reg.reg_mfe_g_pic_width>>4)*(mfe_reg.reg_mfe_g_pic_height>>4) ? 1 : 0;
	if (mfe_reg.reg_mfe_s_ime_h264_p8x8_ctrl_en)
		mfe_reg.reg_mfe_s_ime_h264_p8x8_max = pInfo->nMaxP8x8Count>>4;
	mfe_reg.reg_mfe_s_mesr_adapt = pInfo->IME_ADAPTIVE_WINDOW?1:0;
	mfe_reg.reg_mfe_s_me_ref_en_mode = pInfo->RefCount==2 ? 0x3 : (pInfo->RefCount==1 ? 0x1 : 0);

	mfe_reg.reg_mfe_s_fme_quarter_disable = (pInfo->fme_precision!=2);
	mfe_reg.reg_mfe_s_fme_half_disable = (pInfo->fme_precision==0);
	//mfe_reg.reg_mfe_s_fme_one_mode = 1;
	mfe_reg.reg_mfe_s_fme_pmv_enable = (/*(PSKIP_PREFERRED==3) &&*/ pInfo->InterSearch[0][0]);
	mfe_reg.reg_mfe_s_fme_mode_no = pInfo->nMaxFmeMode-1;
	mfe_reg.reg_mfe_s_fme_mode0_refno = 1;
	mfe_reg.reg_mfe_s_fme_mode1_refno = 1;
	mfe_reg.reg_mfe_s_fme_mode2_refno = 1;

	// Intra update
	mfe_reg.reg_mfe_s_txip_irfsh_en = 0;
	{
		int i;
		int count, prv_intra;
		int start[2], end[2];
		IntraUpdateContext* ctx = &pConfig->m_IUContext;

		count = 0;
		prv_intra = 0;
		for (i=0; i<ctx->nTotalMb; i++) {
			if (ctx->pHwMbMap[i].intra) {
				if (prv_intra==0) {
					count++;
					if (count>2)
						MFE_ASSERT(0);
					start[count-1] = end[count-1] = i;
				}
				else
					end[count-1] = i;
			}
			prv_intra = ctx->pHwMbMap[i].intra;
		}
		if (count>0) {
			mfe_reg.reg_mfe_s_txip_irfsh_en |= 1;
			mfe_reg.reg_mfe_s_txip_irfsh_mb_s0 = start[0];
			mfe_reg.reg_mfe_s_txip_irfsh_mb_e0 = end[0];
		}
		if (count>1) {
			mfe_reg.reg_mfe_s_txip_irfsh_en |= 2;
			mfe_reg.reg_mfe_s_txip_irfsh_mb_s1 = start[1];
			mfe_reg.reg_mfe_s_txip_irfsh_mb_e1 = end[1];
		}
	}

	mfe_reg.reg_mfe_s_mdc_h264_nal_ref_idc = pInfo->nal_ref_idc;
	mfe_reg.reg_mfe_s_mdc_h264_nal_unit_type = pInfo->idr_flag ? 1 : 0;
	mfe_reg.reg_mfe_s_mdc_h264_fnum_bits = pInfo->log2_max_frame_num_minus4+4-5;
	mfe_reg.reg_mfe_s_mdc_h264_dbf_control = pInfo->bDeblockCtrlPresent;
	mfe_reg.reg_mfe_s_mdc_h264_fnum_value = pInfo->frame_num;//img->frame_num;
	mfe_reg.reg_mfe_s_mdc_h264_idr_pic_id = (pInfo->number/*img->number*/ % 2);
	mfe_reg.reg_mfe_s_mdc_h264_disable_dbf_idc = pInfo->nDeblockIDC;//img->LFDisableIdc==1?1:2;
	mfe_reg.reg_mfe_s_mdc_h264_alpha = pInfo->nDeblockAlpha;//img->LFAlphaC0Offset/2;
	mfe_reg.reg_mfe_s_mdc_h264_beta = pInfo->nDeblockBeta;//img->LFBetaOffset/2;
	mfe_reg.reg_mfe_s_mdc_h264_ridx_aor_flag = ((pInfo->num_ref_idx_l0_active != (pInfo->num_ref_idx_l0_active_minus1 +1)) ? 1 : 0);

	mfe_reg.reg_mfe_s_quan_idx_last = 63;
	if (mfe_reg.reg_mfe_s_quan_idx_last<63)
		mfe_reg.reg_mfe_s_quan_idx_swlast = 1;
	else
		mfe_reg.reg_mfe_s_quan_idx_swlast = 0;

	// MBR
	mfe_reg.reg_mfe_g_mbr_en = rcCtx->m_rcGranularity==MBLEVELRC ? 1 : 0;
	mfe_reg.reg_mfe_s_mbr_pqp_dlimit = LEFT_QP_DIFF_LIMIT;
	mfe_reg.reg_mfe_s_mbr_uqp_dlimit = TOP_QP_DIFF_LIMIT;
	// er_en
	if (rcCtx->m_nVPMbRow>0 && rcCtx->m_nVPSize<=0)
		mfe_reg.reg_mfe_g_er_mode = 0;
	else if (rcCtx->m_nVPMbRow<=0 && rcCtx->m_nVPSize>0)
		mfe_reg.reg_mfe_g_er_mode = 1;
	else if (rcCtx->m_nVPMbRow>0 && rcCtx->m_nVPSize>0)
		mfe_reg.reg_mfe_g_er_mode = 2;
	else
		mfe_reg.reg_mfe_g_er_mode = 3;
	if (rcCtx->m_nVPMbRow==0 || rcCtx->m_nVPMbRow==1)
		mfe_reg.reg_mfe_g_er_mby = 0;
	else if (rcCtx->m_nVPMbRow==2)
		mfe_reg.reg_mfe_g_er_mby = 1;
	else if (rcCtx->m_nVPMbRow==4)
		mfe_reg.reg_mfe_g_er_mby = 2;
	else if (rcCtx->m_nVPMbRow==8)
		mfe_reg.reg_mfe_g_er_mby = 3;
	else if (rcCtx->m_nVPMbRow>0)
		MFE_ASSERT(0);
	if (mfe_reg.reg_mfe_g_er_mode==1 || mfe_reg.reg_mfe_g_er_mode==2)
		mfe_reg.reg_mfe_g_er_bs_th = rcCtx->m_nVPSize;
	mfe_reg.reg_mfe_g_qscale = pInfo->intStep;
	mfe_reg.reg_mfe_s_mbr_frame_qstep = rcCtx->m_nFrameQP;
	mfe_reg.reg_mfe_s_mbr_tmb_bits = rcCtx->m_nTargetMbBits;
	// QP/QStep: Min, max
	mfe_reg.reg_mfe_s_mbr_qp_min = rcCtx->m_nMinQP;
	mfe_reg.reg_mfe_s_mbr_qp_max = rcCtx->m_nMaxQP;
	MFE_ASSERT(rcCtx->m_nMinQStep<(2<<7));
	mfe_reg.reg_mfe_s_mbr_qstep_min = rcCtx->m_nMinQStep;
	mfe_reg.reg_mfe_s_mbr_qstep_max = rcCtx->m_nMaxQStep;

	mfe_reg.reg_mfe_g_rec_en = 1;

	// IEAP
	mfe_reg.reg_mfe_s_ieap_last_mode = pInfo->ieap_last_mode;
	mfe_reg.reg_mfe_s_ieap_constraint_intra = pInfo->ieap_constraint_intra;
	mfe_reg.reg_mfe_s_ieap_ccest_en = pInfo->ieap_ccest_en;
	mfe_reg.reg_mfe_s_ieap_ccest_thr = pInfo->ieap_ccest_thr;
	mfe_reg.reg_mfe_s_ieap_drop_i16 = pInfo->ieap_drop_i16;

	// Field coding
	switch (pInfo->h264_mcc_offset[0]) {
		case 0:
			mfe_reg.reg_mfe_s_mvdctl_ref0_offset = 0;
			break;
		case 2:
			mfe_reg.reg_mfe_s_mvdctl_ref0_offset = 1;
			break;
		case -2:
			mfe_reg.reg_mfe_s_mvdctl_ref0_offset = 2;
			break;
	}
	switch (pInfo->h264_mcc_offset[1]) {
		case 0:
			mfe_reg.reg_mfe_s_mvdctl_ref1_offset = 0;
			break;
		case 2:
			mfe_reg.reg_mfe_s_mvdctl_ref1_offset = 1;
			break;
		case -2:
			mfe_reg.reg_mfe_s_mvdctl_ref1_offset = 2;
			break;
	}

	//////////////////////////////////////////////////////////////////////////
	// swcfg1 output
	nTarWriteCount = 0;
	nRegWriteCount = 0;
	nTarFDCCount = 0;
	nRegFDCCount = 0;
	if (nFrmNum==0) {
		nTarWriteCount = 2;
#if defined(WIN32)
		sd_output(sd_sw_cfg1, nTarWriteCount, 10, "NO_OF_REG_WRITE");
#endif
		// Switch to sw mode
		mfe_reg.reg_mfe_g_tbc_mode = 0;
		WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "tbc_mode=0");
		// Switch to hw mode
		mfe_reg.reg_mfe_g_tbc_mode = 1;
		WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "tbc_mode=1");
		MFE_ASSERT(nRegWriteCount==nTarWriteCount);
	}

	nRegWriteCount = 0;
#ifdef WIN32
	sd_output_rel(sd_sw_cfg1, "#frame %X", nFrmNum);
	sd_output_rel(sd_sw_cfg3, "#frame %X", nFrmNum);
#endif

	nTarFDCCount = PutFDC(pContext, 1);
	nTarWriteCount = 54+(nTarFDCCount*3);
	nTarWriteCount++;	// reg to set fdc round
       nTarWriteCount++;   // reset 0/1: MPEG4 enable/disable p skip mode

#ifdef CLOCK_GATING
	nTarWriteCount++;
#endif
#ifdef WIN32
	sd_output(sd_sw_cfg1, nTarWriteCount, 10, "NO_OF_REG_WRITE");
	sd_output(sd_sw_cfg1, nTarFDCCount, 10, "NO_OF_FDC_WRITE");
#endif
	nTarFDCCount *= 3;
	nTarFDCCount++;	// reg to set fdc round

	// SW reset
	mfe_reg.reg_mfe_g_soft_rstz = 0;
	WriteRegMFE(0x0, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "SW reset 0");
	mfe_reg.reg_mfe_g_soft_rstz = 1;
	WriteRegMFE(0x0, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "SW reset 1");
	WriteRegMFE(0x1, mfe_reg.reg01, "[%d] reg01", nRegWriteCount++, "picture width");
	WriteRegMFE(0x2, mfe_reg.reg02, "[%d] reg02", nRegWriteCount++, "picture height");
	WriteRegMFE(0x3, mfe_reg.reg03, "[%d] reg03", nRegWriteCount++, "value");
	WriteRegMFE(0x4, mfe_reg.reg04, "[%d] reg04", nRegWriteCount++, "er_bs mode threshold");
	WriteRegMFE(0x5, mfe_reg.reg05, "[%d] reg05", nRegWriteCount++, "inter prediction preference");

	WriteRegMFE(0x20, mfe_reg.reg20, "[%d] reg20", nRegWriteCount++, "ME partition setting");
	WriteRegMFE(0x21, mfe_reg.reg21, "[%d] reg21", nRegWriteCount++, "value");
	WriteRegMFE(0x22, mfe_reg.reg22, "[%d] reg22", nRegWriteCount++, "me search range max depth");
	WriteRegMFE(0x23, mfe_reg.reg23, "[%d] reg23", nRegWriteCount++, "me mvx");
	WriteRegMFE(0x24, mfe_reg.reg24, "[%d] reg24", nRegWriteCount++, "me mvy");
	WriteRegMFE(0x25, mfe_reg.reg25, "[%d] reg25", nRegWriteCount++, "FME");

#ifdef CLOCK_GATING
	WriteRegMFE(0x16, mfe_reg.reg16, "[%d] reg16", nRegWriteCount++, "Clock gating");
#endif

	// Input buffers
	//
	WriteRegMFE(0x06, mfe_reg.reg06, "[%d] reg06", nRegWriteCount++, "current luma base address");
	WriteRegMFE(0x07, mfe_reg.reg07, "[%d] reg07", nRegWriteCount++, "current luma base address high");
	WriteRegMFE(0x08, mfe_reg.reg08, "[%d] reg08", nRegWriteCount++, "current chroma base address");
	WriteRegMFE(0x09, mfe_reg.reg09, "[%d] reg09", nRegWriteCount++, "current chroma base address high");
	WriteRegMFE(0x0a, mfe_reg.reg0a, "[%d] reg0a", nRegWriteCount++, "reference luma base address0");
	WriteRegMFE(0x0b, mfe_reg.reg0b, "[%d] reg0b", nRegWriteCount++, "reference luma base address0 high");
	WriteRegMFE(0x0c, mfe_reg.reg0c, "[%d] reg0c", nRegWriteCount++, "reference luma base address1");
	WriteRegMFE(0x0d, mfe_reg.reg0d, "[%d] reg0d", nRegWriteCount++, "reference luma base address1 high");
	WriteRegMFE(0x0e, mfe_reg.reg0e, "[%d] reg0e", nRegWriteCount++, "reference chroma base address0");
	WriteRegMFE(0x0f, mfe_reg.reg0f, "[%d] reg0f", nRegWriteCount++, "reference chroma base address0 high");
	WriteRegMFE(0x10, mfe_reg.reg10, "[%d] reg10", nRegWriteCount++, "reference chroma base address1");
	WriteRegMFE(0x11, mfe_reg.reg11, "[%d] reg11", nRegWriteCount++, "reference chroma base address1 high");
	WriteRegMFE(0x12, mfe_reg.reg12, "[%d] reg12", nRegWriteCount++, "reconstructed luma base address:");
	WriteRegMFE(0x13, mfe_reg.reg13, "[%d] reg13", nRegWriteCount++, "reconstructed luma base address high");
	WriteRegMFE(0x14, mfe_reg.reg14, "[%d] reg14", nRegWriteCount++, "reconstructed chroma base address:");
	WriteRegMFE(0x15, mfe_reg.reg15, "[%d] reg15", nRegWriteCount++, "reconstructed chroma base address: high");

	// Output buffer
	WriteRegMFE(0x3c, mfe_reg.reg3c, "[%d] reg3c", nRegWriteCount++, "bsp obuf start address: ");
	WriteRegMFE(0x3d, mfe_reg.reg3d, "[%d] reg3d", nRegWriteCount++, "bsp obuf start address high");
	WriteRegMFE(0x3e, mfe_reg.reg3e, "[%d] reg3e", nRegWriteCount++, "bsp obuf end address: ");
	WriteRegMFE(0x3f, mfe_reg.reg3f, "[%d] reg3f", nRegWriteCount++, "bsp obuf end address high");
	//
	mfe_reg.reg_mfe_s_bspobuf_set_adr = 1;
	mfe_reg.reg_mfe_s_bspobuf_fifo_th = 1;
	mfe_reg.reg_mfe_s_mvobuf_set_adr = 0;
	mfe_reg.reg_mfe_s_mvobuf_fifo_th = 0;
	WriteRegMFE(0x3b, mfe_reg.reg3b, "[%d] reg3b", nRegWriteCount++, "set bsp obuf");
       mfe_reg.reg_mfe_s_bspobuf_set_adr = 0;	// HW is write-one-clear

	// GN
	WriteRegMFE(0x4c, mfe_reg.reg4c, "[%d] reg4c", nRegWriteCount++, "reg_mfe_s_gn_sadr_low");
	WriteRegMFE(0x4d, mfe_reg.reg4d, "[%d] reg4d", nRegWriteCount++, "reg_mfe_s_gn_sadr_high");

	// MBR
	WriteRegMFE(0x26, mfe_reg.reg26, "[%d] reg26", nRegWriteCount++, "MBR: mbbits");
	WriteRegMFE(0x27, mfe_reg.reg27, "[%d] reg27", nRegWriteCount++, "MBR: frame qstep");
	WriteRegMFE(0x29, mfe_reg.reg29, "[%d] reg29", nRegWriteCount++, "264 qp-offset");
	WriteRegMFE(0x2a, mfe_reg.reg2a, "[%d] reg2a", nRegWriteCount++, "QP min/max");
	WriteRegMFE(0x6e, mfe_reg.reg6e, "[%d] reg6e", nRegWriteCount++, "QStep min");
	WriteRegMFE(0x6f, mfe_reg.reg6f, "[%d] reg6f", nRegWriteCount++, "QStep max");

	// MDC
	WriteRegMFE(0x39, mfe_reg.reg39, "[%d] reg39", nRegWriteCount++, "value");

	// Intra Update
	WriteRegMFE(0x2f, mfe_reg.reg2f, "[%d] reg2f", nRegWriteCount++, "value");
	WriteRegMFE(0x30, mfe_reg.reg30, "[%d] reg30", nRegWriteCount++, "value");
	WriteRegMFE(0x31, mfe_reg.reg31, "[%d] reg31", nRegWriteCount++, "value");
	WriteRegMFE(0x32, mfe_reg.reg32, "[%d] reg32", nRegWriteCount++, "value");

	// DBF
	WriteRegMFE(0x3a, mfe_reg.reg3a, "[%d] reg3a", nRegWriteCount++, "value");

	mfe_reg.reg_mfe_g_crc_mode = 0xC;
       mfe_reg.reg_mfe_g_debug_tcycle_chk_en = 0x1;
#ifdef TEST_CRC_MODE
       mfe_reg.reg_mfe_g_debug_tcycle_chk_sel = 0x1;
#endif
       WriteRegMFE(0x73, mfe_reg.reg73, "[%d] reg73", nRegWriteCount++, "crc mode");

	//mfe_reg.reg_mfe_s_txip_idle_cnt = 160;
	//WriteRegMFE(0x33, mfe_reg.reg33, "[%d] reg33", nRegWriteCount++, "txip_idle_cnt");

	WriteRegMFE(0x2c, mfe_reg.reg2c, "[%d] reg2c", nRegWriteCount++, "Last zigzag");

	// IEAP
#ifdef WIN32
	sd_output(sd_sw_cfg3, 0, 10, "NO_OF_REG_WRITE");
#endif
	WriteRegMFE(0x2b, mfe_reg.reg2b, "[%d] reg2b", nRegWriteCount++, "ieap");

	// Cross-format wrong reg setting prevention
	WriteRegMFE(0x18, mfe_reg.reg18, "[%d] reg18", nRegWriteCount++, "JPE encode mode");
	WriteRegMFE(0x1b, mfe_reg.reg1b, "[%d] reg1b", nRegWriteCount++, "MPEG4 FieldDCT");
	MFE_ASSERT(mfe_reg.reg19==0);
	WriteRegMFE(0x19,0, "[%d] reg19", nRegWriteCount++, "0/1: MPEG4 enable/disable p skip mode");


	if (mfe_reg.reg_mfe_g_enc_mode !=REG_ENC_MODE_MPG4) {	// MPEG-4
		WriteRegMFE(0x37, mfe_reg.reg37=0, "[%d] reg37", nRegWriteCount, "MPEG4 MDC");
		WriteRegMFE(0x38, mfe_reg.reg38=0, "[%d] reg38", nRegWriteCount, "MPEG4: vop_time_increment");
		// B-direct
		WriteRegMFE(0x1a, mfe_reg.reg1a=0, "[%d] reg1a", nRegWriteCount, "MPEG4 BDirect");
	}

//	ReadRegMFE(0x70, & mfe_reg.reg70);
//	mfe_reg.reg_mfe_g_debug_mode = 0xe;
//	WriteRegMFE(0x70, mfe_reg.reg70, "", 1, "");

	//2009/12/18 fixed pending bug, it must = 7, the old version default=8, will halt at P frame.
//	ReadRegMFE(0x56, & mfe_reg.reg56);
//	mfe_reg.reg_mfe_s_marb_mr_pending = 7;
//	WriteRegMFE(0x56, mfe_reg.reg56, "", 1, "");

#if defined(TEST_MB_STOPANDGO) || defined(TEST_MB_STOPANDDROP)
	if (nFrmNum==STOP_FRAME)
	TestStopAtMb();
#endif

//	printf("OutputSwCfg1_H264 Before Enable HW,\n");
	DumpAllReg();


	// Enable HW
	mfe_reg.reg_mfe_g_frame_start_sw = 1;
	WriteRegMFE(0x00, mfe_reg.reg00, "[%d] reg00", nRegWriteCount++, "frame start");
    mfe_reg.reg_mfe_g_frame_start_sw = 0;	// HW is write-one-clear

	// FDC
	nRegFDCCount = PutFDC(pContext, 0);
	nRegWriteCount += nRegFDCCount;
#if defined(TEST_STOPANDGO) || defined(TEST_STOPANDDROP)
	if (nFrmNum==STOP_FRAME)
	TestStop();
#endif

	// Only for debug
	MFE_ASSERT(nRegFDCCount==nTarFDCCount);
	MFE_ASSERT(nRegWriteCount==nTarWriteCount);
}


