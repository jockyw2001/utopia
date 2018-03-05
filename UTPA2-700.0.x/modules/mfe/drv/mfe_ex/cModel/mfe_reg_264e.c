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
#include "mfe_type.h"
#include "mfe_common.h"
#include "ms_dprintf.h"
#include "mhal_mfe.h"
#ifdef __MOBILE_CASE__
#include <stdio.h>
#include <string.h>
#endif

#include "parset.h"
#include "msRateCtrl.h"
#include "BufMng.h"

#include "mfe_reg.h"
// Sync with cModel QExp.h
#define CLOCK_GATING	// Enable clock gating
#define FME_PIPELINE_OPEN	// Enable David's FME speedup version
#define CHECK_P8x8_BOUND_RECT

// Memory bound addr units is 8-byte.
#define EIGHT_BYTE_ADDR(addr)  ((addr) >> 3)
#define LO_WORD(val)           ((val) & 0xFFFF)
#define HI_WORD(val)           (((val) >> 16) & 0xFFFF)
#define MEM_BOUND_LO(addr)     (LO_WORD(EIGHT_BYTE_ADDR(addr)))
#define MEM_BOUND_HI(addr)     (HI_WORD(EIGHT_BYTE_ADDR(addr)))

#if defined(_MFE_T8_)
#define MAX_REF_FRAME  2UL
#define SEARCH_RANGE_X 32UL
#define BLOCK_TYPE_4x4 1UL
#define BLOCK_TYPE_4x8 1UL
#define BLOCK_TYPE_8x4 1UL
#elif defined(_MFE_M1_)
#define MAX_REF_FRAME  1UL
#define SEARCH_RANGE_X 16UL
#define BLOCK_TYPE_4x4 0UL
#define BLOCK_TYPE_4x8 0UL
#define BLOCK_TYPE_8x4 0UL
#endif

#if (DEBUG_LEVEL & DRV_L6)
#include "api_mfe_performance.h"
extern mfe_performance_t mfe_enc;
extern mfe_performance_t mfe_hw_enc;
extern mfe_performance_t mfe_duration_enc;
#endif

#ifdef MFE_MIU_PROTECT
extern void MHal_MFE_Enable_MIU_Protection(MS_S32 MIU_TEST_MODE,MFE_CONFIG* pConfig);
#endif
void OutputSwCfg1_H264(MS_S32 nFrmNum, MFE_CONFIG* pConfig);

void mfeH264_DeInit(MFE_CONFIG* pConfig)
{
    MS_S32 i;
    H264INFO* pInfo = &pConfig->ctxH264Info;
    FreeSPSPPSBuf(pConfig);
    if(pInfo->dpb)
        MfeDrvMemFree((void**)&pInfo->dpb, (const MS_S8*)("pInfo->dpb"));


    for(i=0;i<pInfo->BufPool.FrameBufPoolSize;i++) {
        if(pInfo->BufPool.addr[i])
            MfeDrvMemFree((void**)&(pInfo->BufPool.addr[i]), (const MS_S8*)("BufPool.addr[i]"));
    }
    if(pInfo->BufPool.addr)
        MfeDrvMemFree((void**)&pInfo->BufPool.addr, (const MS_S8*)("BufPool.addr"));
    if(pInfo->BufPool.available)
        MfeDrvMemFree((void**)&pInfo->BufPool.available, (const MS_S8*)("BufPool.available"));
    IntraUpdateClose(pConfig);

}
void mfeH264_Init(MFE_CONFIG *pConfig)
{
    MS_S32 i;
    H264INFO* pInfo = &pConfig->ctxH264Info;
    MS_S32 FrameSizeInMbs = (pConfig->nBufWidth>>4) * (pConfig->nBufHeight>>4);

    memset(pInfo, 0, sizeof(H264INFO));

    if(pConfig->UseCABAC) {
        pInfo->ProfileIDC = 77;	// Baseline
        pInfo->LevelIDC = 40;	// Up to 720x576x25 or 720x480x30
    } else {
        pInfo->ProfileIDC = 66;	// Baseline
        pInfo->LevelIDC = 30;	// Up to 720x576x25 or 720x480x30
        if(pConfig->nDispHeight> 576)
            pInfo->LevelIDC = 31;
    }
    pInfo->PicInterlace = (pConfig->bInterlace == PROGRESSIVE)? FRAME_CODING : FIELD_CODING;
    if (pInfo->PicInterlace==FIELD_CODING) {
    	pInfo->ProfileIDC = 77;	// Forced to be main profile
    	MS_ASSERT((pConfig->nBufHeight&0x1F)==0);
    }

    if (pConfig->setLevel != 0) {
        pInfo->LevelIDC = pConfig->setLevel;
    }

    if (pConfig->enableReduceBW) {
        pInfo->nMaxP8x8Count = FrameSizeInMbs / 2;
    } else {
        pInfo->nMaxP8x8Count = FrameSizeInMbs;
    }
    pInfo->log2_max_frame_num_minus4 = 1;

    // ME
    pInfo->num_ref_frames = MAX_REF_FRAME;        // sequence-level

    if (pInfo->PicInterlace==FIELD_CODING)
        pInfo->num_ref_frames = MAX_REF_FRAME -1;		// means: 2 field pictures
    else
        pInfo->num_ref_frames = MAX_REF_FRAME;		// means: 2 frame

    if((pConfig->nBufHeight*pConfig->nBufWidth) >= (1280*720))
        pInfo->num_ref_frames = 1;
#ifdef MFE_MIU_PROFILE
    pInfo->num_ref_frames = 1;
#endif
    pInfo->iSearchRangeForward = SEARCH_RANGE_X;
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
    if (pConfig->enableReduceBW) {
        pInfo->InterSearch[0][BLOCK8x4] = 0;
        pInfo->InterSearch[0][BLOCK4x8] = 0;
        pInfo->InterSearch[0][BLOCK4x4] = 0;
    } else {
        pInfo->InterSearch[0][BLOCK8x4] = BLOCK_TYPE_8x4;
        pInfo->InterSearch[0][BLOCK4x8] = BLOCK_TYPE_4x8;
        pInfo->InterSearch[0][BLOCK4x4] = BLOCK_TYPE_4x4;
    }
    for (i=0; i<=BLOCK4x4; i++)
    	pInfo->InterSearch[1][i] = 0;

    if (pInfo->PicInterlace==FIELD_CODING)
        pInfo->num_ref_idx_l0_active_minus1 = 2*pInfo->num_ref_frames-1;	// PPS info
    else
        pInfo->num_ref_idx_l0_active_minus1 = pInfo->num_ref_frames-1;	// PPS info

    // DBF
    pInfo->bDeblockCtrlPresent = 0;
    pInfo->nDeblockIDC = 2;	// 0--all edges; 1--no edges; 2--edges inside slice boundary
    						// NOTE: MFE does not support er_en enabled with DeblockIDC=0
    pInfo->nDeblockAlpha = 0;
    pInfo->nDeblockBeta = 0;

    // Frame type
    pInfo->pic_order_cnt_type = 2;	// HW only support 2
    pInfo->structure = FRAME;       // If H264 PAFF, override for each input field in MDrv_MFE_GetDispOrder()
    pInfo->idr_flag = 1;
    pInfo->nal_ref_idc = 1;
    pInfo->frame_num = 0;
    pInfo->number = 0;
    pInfo->num_ref_idx_l0_active = 0;
    pInfo->RefCount = 0;	// frame-level 0: 1ref, 1: 2ref
    pInfo->h264_mcc_offset[0] = pInfo->h264_mcc_offset[1] = 0;
    pInfo->intQP = 0;

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

    if (pConfig->enableFastMode) {
        pInfo->ieap_last_mode = 2;
        pInfo->ieap_drop_i16 = 1;
        pInfo->fme_precision = 0;
    }

    // Rate control
    MfeDrvRateControlInit(pConfig);
    // FDC header initialization
    osCreate(&pConfig->m_OutStream);

    osSetWriteBuffer(&pConfig->m_OutStream, pConfig->m_FdcBuffer);

    // Intra-update initialization
    IntraUpdateInit(pConfig);

    // Prepare SPS, PPS
    MallocSPSPPSBuf(pConfig);
    GenerateSPSPPS(pConfig);

	// DPB initialization
    if (pInfo->PicInterlace==FIELD_CODING)
        pInfo->dpb_size = pInfo->num_ref_frames + 1;
    else
        pInfo->dpb_size = pInfo->num_ref_frames;
    pInfo->dpb = MfeDrvMemMalloc(sizeof(DPB_ITEM)*pInfo->dpb_size, (const MS_S8*)("dpb"));
    for (i=0; i<pInfo->dpb_size; i++)
    	pInfo->dpb[i].is_used = 0;


    //FrameBufPool initialization
    pInfo->BufPool.FrameBufPoolSize = pInfo->dpb_size+1;
    pInfo->BufPool.addr = (MEMMAP_t**)MfeDrvMemMalloc(sizeof(MEMMAP_t*)*(pInfo->BufPool.FrameBufPoolSize), (const MS_S8*)("BufPool.addr"));
    pInfo->BufPool.available = MfeDrvMemMalloc(sizeof(MS_S32)*pInfo->BufPool.FrameBufPoolSize, (const MS_S8*)("BufPool.available"));

    if (pInfo->BufPool.addr) {
        for (i = 0; i < pInfo->BufPool.FrameBufPoolSize; i++) {
            pInfo->BufPool.addr[i] = (MEMMAP_t*)MfeDrvMemMalloc(sizeof(MEMMAP_t) * 2, (const MS_S8*)("BufPool.addr[i]"));
        }
    }
}


#ifdef DRV_MFE_TIME_MEASURE
extern void MFE_MsOS_START_TIMER (void);
#endif

void mfeH264_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{
    H264INFO* pInfo = &pConfig->ctxH264Info;
    BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
    OutStream* pStream = &pConfig->m_OutStream;
        const MS_U8 gBITMASK[8] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };
    // This is done in msAPI_MFE_EnOneFrm()
    //mfeSetVopType(pConfig, pGopInfo); // Will set pConfig->vopPredType
#ifdef SPS_PPS_IN_EACH_I
    	pInfo->idr_flag = pConfig->vopPredType==I_VOP ? 1 : 0;
//#elif defined(NO_SPS_PPS)
//    pInfo->idr_flag = 0;
#else
/*
      // Frame type
    if (pGopInfo->nCodingOrder==0)
    	pInfo->idr_flag = pConfig->vopPredType==I_VOP ? 1 : 0;
    else
    	pInfo->idr_flag = 0;
*/
    pInfo->idr_flag = pConfig->vopPredType==I_VOP ? 1 : 0;
#endif
    pInfo->nal_ref_idc = (get_NALU_1stbyte(pInfo, pConfig->vopPredType)>>5)&0x3;
    if (pInfo->idr_flag) {
        //pInfo->number = 0;
    	pInfo->frame_num = 0;
    }
    //pInfo->frame_num, pInfo->number
    if (pInfo->idr_flag)//(pConfig->vopPredType==I_VOP)
    	pInfo->num_ref_idx_l0_active = 0;
    else {
    	if (pInfo->num_ref_idx_l0_active<pInfo->num_ref_idx_l0_active_minus1+1)
           pInfo->num_ref_idx_l0_active++;
    }

    if(pInfo->PicInterlace==FIELD_CODING){
        if(pInfo->structure==BOTTOM_FIELD){
            //always set bottom field to override.
            if (pInfo->num_ref_idx_l0_active==(pInfo->num_ref_idx_l0_active_minus1+1))
                pInfo->num_ref_idx_l0_active -= 1;
        }
    }

    pInfo->RefCount = pConfig->vopPredType==I_VOP ? 0 : pInfo->num_ref_idx_l0_active;
    pInfo->h264_mcc_offset[0] = pInfo->h264_mcc_offset[1] = 0;  // If PAFF, will possibly override in h264GetBufferAddr()

    if(pConfig->VTMode) {
        MS_S8 chFrameType;
        if(pConfig->vopPredType==I_VOP)
            chFrameType = 'I';
        else
            chFrameType = 'P';
        pInfo->intQP = rc_InitFrame(&pConfig->VTRateCtrl, chFrameType);
    }
    else
        pInfo->intQP = cvbr_InitFrame(&pConfig->ctxRateControl, pConfig->vopPredType, pInfo->structure);

    // Buffer management
    h264GetBufferAddr(pConfig);

    // Prepare header
    osReset(pStream);
#if 0//ndef _MFE_M1_
    // SPS, PPS
    if (pGopInfo->nCodingOrder==0||pInfo->idr_flag) {
    	codeSPSPPS(pConfig,pStream);
    }
#endif
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




    //MODE 0: (Checking range > real range ) 	reg_mfe_s_marb_miu_bound_err = 0
    //MODE 1: (Checking range < real range ) 	reg_mfe_s_marb_miu_bound_err = 1
    //#define TEST_MIU_PROTECTION_MODE 0UL
#ifdef MFE_MIU_PROTECT
    MHal_MFE_Enable_MIU_Protection(TEST_MIU_PROTECTION_MODE,pConfig);
#endif
    /*
    {	// DEBUG codes
    	MS_S32 i;
    	ms_dprintk(DRV_L3,"FDC bitcount %d ==> "), pBitsInfo->len*8+pBitsInfo->bit_len);
    	for (i=0; i<pBitsInfo->len; i++) {
    		ms_dprintk(DRV_L3,"%02x "), pBitsInfo->ptr[i]);
    	}
    	ms_dprintk(DRV_L3,"%02x(msb %d bits)\n"), pBitsInfo->bits, pBitsInfo->bit_len);
    }
    */
#if (DEBUG_LEVEL & DRV_L6)
        mfe_enc.mfe_starttime = MsOS_GetSystemTime();
#endif
    // Set reg and start encoding
     //       DumpAllReg(mfe_reg);
    OutputSwCfg1_H264(pGopInfo->nCodingOrder, pConfig);

    //	printf("DumpAllReg After delay OutputSwCfg1_H264\n"));
    //		DumpAllReg(mfe_reg);


}

void mfeH264_EncodeDummyFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{
    H264INFO* pInfo = &pConfig->ctxH264Info;
    BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
    OutStream* pStream = &pConfig->m_OutStream;
    const MS_U8 gBITMASK[8] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };

    pInfo->RefCount = pInfo->num_ref_idx_l0_active; // frame-level 0: 1ref, 1: 2ref
    pInfo->h264_mcc_offset[0] = pInfo->h264_mcc_offset[1] = 0;

    // Prepare header
    osReset(pStream);
    // First slice header
    codeDummySliceHeader(pConfig, pStream);
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
}

void mfeH264_UpdateFrame(MFE_CONFIG *pConfig)
{
    H264INFO* pInfo = &pConfig->ctxH264Info;
    MS_S32 max_frame_num = 1 << (pInfo->log2_max_frame_num_minus4 + 4);
    //if (pConfig->vopPredType==I_VOP)
    //	IntraUpdateInit(pConfig);
    IntraUpdateFrame(pConfig);

    // Rate control
    MfeDrvRateControlUpdate(pConfig, pInfo->structure);

    h264DpbHandling(pConfig);

    if (pInfo->structure!=TOP_FIELD)
    {
        pInfo->frame_num++;
        pInfo->frame_num %= max_frame_num;
    }
    pInfo->number++;
    //pInfo->number++;

    // If PAFF, always assume top-field first.
    // A trick here: restore to top type for correct mfeSetVopType()
    if (pInfo->structure==BOTTOM_FIELD)
        pConfig->vopPredType = pInfo->vopPredType_FirstField;

}

void OutputSwCfg1_H264(MS_S32 nFrmNum, MFE_CONFIG* pConfig)
{
    MS_S32 nTarWriteCount;
    MS_S32 nRegWriteCount;
    MS_S32 nTarFDCCount;
    MS_S32 nRegFDCCount;

    H264INFO* pInfo = &pConfig->ctxH264Info;
    BufInfo* pBufInfo = &pConfig->ctxBufInfo;
    BitsInfo *pBitsInfo = &pConfig->ctxBitsInfo;
    CVBRRateControl* rcCtx = &pConfig->ctxRateControl;
    MFE_REG* mfe_reg = &pConfig->mfe_reg;
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    MFE_REG1* mfe_reg1 = &pConfig->mfe_reg1;
#endif
    void* pContext = (void*)pBitsInfo;

	//////////////////////////////////////////////////////////////////////////
	// Sequence-wide settings

	if (nFrmNum==0) {
        memset(mfe_reg, 0, sizeof(MFE_REG));    // Initial
        mfe_reg->reg_mfe_g_enc_mode = REG_ENC_MODE_H264;
        mfe_reg->reg_mfe_g_pic_width = pConfig->nBufWidth;
        mfe_reg->reg_mfe_g_pic_height = (pInfo->PicInterlace==FIELD_CODING) ? (pConfig->nBufHeight>>1) : pConfig->nBufHeight;

		// Qtable
        mfe_reg->reg_mfe_g_qmode = 0;
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
        if(0) {
            mfe_reg->reg_mfe_g_mb_pitch_en = 1;
            //pirch for x-axle
            mfe_reg->reg_mfe_g_mb_pitch = 0;//g_nMBPitch;//mfe_reg.reg_mfe_g_pic_width;
        }
        else {
            mfe_reg->reg_mfe_g_mb_pitch_en = 0;
            //pirch for x-axle
            mfe_reg->reg_mfe_g_mb_pitch = mfe_reg->reg_mfe_g_pic_width/16;
        }
#endif
		//Field Coding
		if (pInfo->PicInterlace==FIELD_CODING) {
                mfe_reg->reg_mfe_g_fldpic_en = 1;
#if defined(_MFE_AGATE_)
                mfe_reg->reg_mfe_g_mstar_tile_field_split = (pConfig->bInterlace ==INTERLACE_SPLITED)? 1:0 ;
                mfe_reg->reg_mfe_g_fldpic_idx = 0; //top
#endif
		}
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
		//Entropy coding mode
        if(pConfig->UseCABAC==1)
			mfe_reg->reg_mfe_g_cabac_en = 1;
		else
			mfe_reg->reg_mfe_g_cabac_en = 0;
#endif
#if defined(_MFE_AGATE_)
        if(pConfig->bColorFormat != YUVTILE)
            mfe_reg->reg_mfe_g_mstar_tile = 1;
        else
            mfe_reg->reg_mfe_g_mstar_tile = 0;
#endif

		// ME partition type
        mfe_reg->reg_mfe_s_me_16x16_disable = !pInfo->InterSearch[0][BLOCK16x16];
        mfe_reg->reg_mfe_s_me_8x8_disable = !pInfo->InterSearch[0][BLOCK8x8];
        mfe_reg->reg_mfe_s_me_16x8_disable = !pInfo->InterSearch[0][BLOCK16x8];
        mfe_reg->reg_mfe_s_me_8x16_disable = !pInfo->InterSearch[0][BLOCK8x16];
        mfe_reg->reg_mfe_s_me_8x4_disable  = !pInfo->InterSearch[0][BLOCK8x4];
        mfe_reg->reg_mfe_s_me_4x8_disable  = !pInfo->InterSearch[0][BLOCK4x8];
        mfe_reg->reg_mfe_s_me_4x4_disable  = !pInfo->InterSearch[0][BLOCK4x4];

		// MDC
        mfe_reg->reg_mfe_s_mbr_qp_cidx_offset = 0;//active_pps->chroma_qp_index_offset;
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
            // pre-fetch
            mfe_reg->reg_mfe_s_prfh_cryc_en = 1;
            mfe_reg->reg_mfe_s_prfh_refy_en = 1;
            mfe_reg->reg_mfe_s_prfh_cryc_idle_cnt = 0;
            mfe_reg->reg_mfe_s_prfh_refy_idle_cnt = 0;
#ifdef SW_BUF_MODE
            mfe_reg->reg_mfe_g_sw_buffer_mode = 1;
#else
            mfe_reg->reg_mfe_g_sw_buffer_mode = 0;
#endif

            mfe_reg->reg_mfe_g_jpe_buffer_mode = 1;    // frame-mode
            mfe_reg->reg_mfe_g_jpe_fsvs_mode = 0;
#endif
	}
	else {
        mfe_reg->reg_mfe_g_frame_start_sw = 0;
#if defined(_MFE_AGATE_)
        mfe_reg->reg_mfe_g_fldpic_idx = ~mfe_reg->reg_mfe_g_fldpic_idx; //change top and bottom.
#endif
	}

#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
        mfe_reg1->reg_mfe_g_jpd_hsk_en = 0;
        if(pConfig->g_nUseYUV422 != 0) {
            mfe_reg1->reg_mfe_s_packed422_en = 1;
        }
        else {
            mfe_reg1->reg_mfe_s_packed422_en = 0;
        }

        if(pConfig->g_nUseYUV422 == 1){
            mfe_reg1->reg_mfe_s_packed422_uv_swap = 0;
            mfe_reg1->reg_mfe_s_packed422_yc_swap = 0;
        }
        else if(pConfig->g_nUseYUV422 == 2) {
            mfe_reg1->reg_mfe_s_packed422_uv_swap = 1;
            mfe_reg1->reg_mfe_s_packed422_yc_swap = 0;
        }
        else if(pConfig->g_nUseYUV422 == 3) {
            mfe_reg1->reg_mfe_s_packed422_uv_swap = 0;
            mfe_reg1->reg_mfe_s_packed422_yc_swap = 1;
        }
        else if(pConfig->g_nUseYUV422 == 4) {
            mfe_reg1->reg_mfe_s_packed422_uv_swap = 1;
            mfe_reg1->reg_mfe_s_packed422_yc_swap = 1;
        }
#endif

#if defined(MFE_YUV_LOADER)
    mfe_reg1->reg0b = 0;
    mfe_reg1->reg56 = 0;

    if (YUVTILE != pConfig->bColorFormat) {
        mfe_reg1->reg_mfe_g_yuvldr_en = 1;
        if (MFE_EVDTILE == pConfig->bColorFormat) {
            mfe_reg1->reg_mfe_g_hevd_tile = 1;
        }
        else if (MFE_32x32TILE == pConfig->bColorFormat) {
            mfe_reg1->reg_mfe_g_hevd_tile32 = 1;
        }
        else if (MFE_NV12 == pConfig->bColorFormat) {
            mfe_reg1->reg_mfe_g_yuv420_semi = 1;
        }
        else if (MFE_NV21 == pConfig->bColorFormat) {
            mfe_reg1->reg_mfe_g_yuv420_semi = 1;
            mfe_reg1->reg_mfe_g_yuv420_semi_uv_swap = 1;
        }
        else if (pConfig->g_nUseYUV422 != 0) {
            mfe_reg1->reg_mfe_g_mb_pitch = pConfig->nBufWidth / 16;
        }
    }
#endif

	//////////////////////////////////////////////////////////////////////////
	// Frame-wide settings
#ifdef CLOCK_GATING
     mfe_reg->reg16 = 0xffff;    // clock gating
#endif
     ms_dprintk(DRV_L3,"   RecY:0x%x, RecC:0x%x\n", (unsigned int)pBufInfo->m_nRecYAddr.miuAddress, (unsigned int)pBufInfo->m_nRecCAddr.miuAddress);
     ms_dprintk(DRV_L3,"[0]RefY:0x%x, RefC:0x%x\n", (unsigned int)pBufInfo->m_nRefYAddr[0].miuAddress, (unsigned int)pBufInfo->m_nRefCAddr[0].miuAddress);
     if(pInfo->num_ref_frames==2)
         ms_dprintk(DRV_L3,"[1]RefY:0x%x, RefC:0x%x\n", (unsigned int)pBufInfo->m_nRefYAddr[1].miuAddress, (unsigned int)pBufInfo->m_nRefCAddr[1].miuAddress);

    if(pConfig->g_nUseYUV422 != 0) {
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
        MS_ASSERT((pBufInfo->m_nCurYAddr.miuAddress&0xF)==0);
        mfe_reg1->reg_mfe_g_enc_cury_offset = 0;
        mfe_reg1->reg_mfe_g_enc_cury_adr_low = (MS_U16)((pBufInfo->m_nCurYAddr.miuAddress>>3)& 0x1FFF); //13bit
        mfe_reg1->reg_mfe_g_enc_cury_adr_high = (MS_U16)(pBufInfo->m_nCurYAddr.miuAddress >> (13+3));
#else
        MS_ASSERT(0); // 422 mode didnot supported at old chip.
#endif
    } else {
	    // Input buffer address: Must be 256-byte aligned.
	    MS_ASSERT((pBufInfo->m_nCurYAddr.miuAddress&0xFF)==0);
	    MS_ASSERT((pBufInfo->m_nCurCAddr.miuAddress&0xFF)==0);
        mfe_reg->reg_mfe_g_cur_y_adr_low  = (MS_U16)((pBufInfo->m_nCurYAddr.miuAddress>>8)&0xFFFF);
        mfe_reg->reg_mfe_g_cur_y_adr_high = (MS_U16)(pBufInfo->m_nCurYAddr.miuAddress>>(8+16));
        mfe_reg->reg_mfe_g_cur_c_adr_low  = (MS_U16)((pBufInfo->m_nCurCAddr.miuAddress>>8)&0xFFFF);
        mfe_reg->reg_mfe_g_cur_c_adr_high = (MS_U16)(pBufInfo->m_nCurCAddr.miuAddress>>(8+16));
    }
	MS_ASSERT((pBufInfo->m_nRefYAddr[0].miuAddress&0xFF)==0);
    MS_ASSERT((pBufInfo->m_nRefCAddr[0].miuAddress&0xFF)==0);
#ifndef _MFE_M1_
	MS_ASSERT((pBufInfo->m_nRefYAddr[1].miuAddress&0xFF)==0);
	MS_ASSERT((pBufInfo->m_nRefCAddr[1].miuAddress&0xFF)==0);
#endif
    if(!pConfig->SecurityMode) {
	MS_ASSERT((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress&0x7)==0);
    }

    mfe_reg->reg_mfe_g_ref_y_adr0_low = (MS_U16)((pBufInfo->m_nRefYAddr[0].miuAddress>>8)&0xFFFF);
    mfe_reg->reg_mfe_g_ref_y_adr0_high = (MS_U16)(pBufInfo->m_nRefYAddr[0].miuAddress>>(8+16));
#ifndef _MFE_M1_
    mfe_reg->reg_mfe_g_ref_y_adr1_low  = (MS_U16)((pBufInfo->m_nRefYAddr[1].miuAddress>>8)&0xFFFF);
    mfe_reg->reg_mfe_g_ref_y_adr1_high = (MS_U16)(pBufInfo->m_nRefYAddr[1].miuAddress>>(8+16));
#endif
    mfe_reg->reg_mfe_g_ref_c_adr0_low  = (MS_U16)((pBufInfo->m_nRefCAddr[0].miuAddress>>8)&0xFFFF);
    mfe_reg->reg_mfe_g_ref_c_adr0_high = (MS_U16)(pBufInfo->m_nRefCAddr[0].miuAddress>>(8+16));
#ifndef _MFE_M1_
    mfe_reg->reg_mfe_g_ref_c_adr1_low  = (MS_U16)((pBufInfo->m_nRefCAddr[1].miuAddress>>8)&0xFFFF);
    mfe_reg->reg_mfe_g_ref_c_adr1_high = (MS_U16)(pBufInfo->m_nRefCAddr[1].miuAddress>>(8+16));
#endif

    mfe_reg->reg_mfe_g_rec_y_adr_low  = (MS_U16)((pBufInfo->m_nRecYAddr.miuAddress>>8)&0xFFFF);
    mfe_reg->reg_mfe_g_rec_y_adr_high = (MS_U16)(pBufInfo->m_nRecYAddr.miuAddress>>(8+16));
    mfe_reg->reg_mfe_g_rec_c_adr_low  = (MS_U16)((pBufInfo->m_nRecCAddr.miuAddress>>8)&0xFFFF);
    mfe_reg->reg_mfe_g_rec_c_adr_high = (MS_U16)(pBufInfo->m_nRecCAddr.miuAddress>>(8+16));

    ms_dprintk(DRV_L3, "field = %d, Rec = 0x%x, Ref0 = 0x%x, Ref1 = 0x%x\n",
        pInfo->structure, (unsigned int)pBufInfo->m_nRecYAddr.miuAddress,
        (unsigned int)pBufInfo->m_nRefCAddr[0].miuAddress, (unsigned int)pBufInfo->m_nRefCAddr[1].miuAddress);

	// Output buffers: Must be 8-byte aligned.
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
	mfe_reg->reg_mfe_s_bspobuf_hw_en = 0;
#if defined(USE_HW_DBL_OBUF)
	mfe_reg->reg_mfe_s_bspobuf_hw_en = 1;
#endif
#else
	mfe_reg->reg_mfe_s_bspobuf_sadr_low  = (MS_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>3)&0xFFFF);
	mfe_reg->reg_mfe_s_bspobuf_sadr_high = (MS_U16)(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress>>(3+16));
	mfe_reg->reg_mfe_s_bspobuf_eadr_low  = (MS_U16)(((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>3)&0xFFFF);
	mfe_reg->reg_mfe_s_bspobuf_eadr_high = (MS_U16)((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress+pBufInfo->m_OutBufferSize-8)>>(3+16));
#endif

	// GN
    mfe_reg->reg_mfe_s_gn_sadr_low  = (MS_U16)((pBufInfo->m_nGNAddr.miuAddress>>3)&0xFFFF);
    mfe_reg->reg_mfe_s_gn_sadr_high = (MS_U16)(pBufInfo->m_nGNAddr.miuAddress>>(3+16));
#ifdef _MFE_M1_
       mfe_reg->reg_mfe_s_gn_sadr_mode =
#if defined(GN_WHOLE_FRAME)
        1;
#else
        0;
#endif

        // IMI buffer
        mfe_reg->reg_mfe_s_marb_eimi_block = 0;
        mfe_reg->reg_mfe_s_marb_lbwd_mode = 0;
        mfe_reg->reg_mfe_s_marb_imi_sadr_low = 0;
        mfe_reg->reg_mfe_s_marb_imi_sadr_high = 0;

    if (pConfig->vopPredType!=I_VOP && pConfig->MfeAdvInfo.low_bandwidth_en && pConfig->imi_size>0) {
        mfe_reg->reg_mfe_s_marb_eimi_block = 1;
        mfe_reg->reg_mfe_s_marb_lbwd_mode = 1;
        mfe_reg->reg_mfe_s_marb_imi_sadr_low = (pConfig->imi_addr>>3)&0xFFFF;
        mfe_reg->reg_mfe_s_marb_imi_sadr_high = pConfig->imi_addr>>(3+16);
        switch (pConfig->imi_size) {
            // 0: 64kB, 1:32kB, 2:16kB, 3:8kB
        case 0x10000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 0;
            break;
        case 0x8000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 1;
            break;
        case 0x4000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 2;
            break;
        case 0x2000:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 3;
            break;
        default:
            mfe_reg->reg_mfe_s_marb_imi_cache_size = 0;
            MS_ASSERT(0);
        }
    }

#endif

#if defined(MFE_DBF_PACKED_MODE)
    mfe_reg->reg_mfe_s_gn_bwr_mode |= 0x2;
#endif

    mfe_reg->reg_mfe_g_frame_type = pConfig->vopPredType==I_VOP?0:(pConfig->vopPredType==P_VOP?1:2);

    mfe_reg->reg_mfe_g_ref_no = pInfo->RefCount==2 ? 1 : 0;
/*
    if(pInfo->PicInterlace==FIELD_CODING){
        if(pInfo->structure==BOTTOM_FIELD){
            mfe_reg->reg_mfe_g_ref_no = 0;
        }
    }
*/
    // ME setting
    mfe_reg->reg_mfe_s_ime_mesr_max_addr = (pInfo->SEARCH_RANGE_Y==16 ? 95 : /*83*/85);//0x5f;
    mfe_reg->reg_mfe_s_ime_mesr_min_addr =(pInfo->SEARCH_RANGE_Y==16 ? 0 : /*16*/10);//0;

#if defined(MFE_SUPPORT_720P)
    // If force encoding frame size bigger than 720p on 720p/30fps chip,
    // reduce ime search range to improve encoding speed.
    if (pConfig->nBufWidth * pConfig->nBufHeight > 1280 * 736)
    {
        mfe_reg->reg_mfe_s_ime_mvx_min = 0x10;
        mfe_reg->reg_mfe_s_ime_mvx_max = 0x2e;
    }
    else
#endif
    {
        mfe_reg->reg_mfe_s_ime_mvx_min = -(pInfo->iSearchRangeForward) + 32;    // Min X is -pVopMd->iSearchRangeForward
        mfe_reg->reg_mfe_s_ime_mvx_max = (pInfo->iSearchRangeForward-2) + 32;   // Max X is pVopMd->iSearchRangeForward-1
    }

    mfe_reg->reg_mfe_s_ime_mvy_min = -pInfo->SEARCH_RANGE_Y + 16;               // Min Y
    mfe_reg->reg_mfe_s_ime_mvy_max = (pInfo->SEARCH_RANGE_Y==16 ? 15 : 8) + 16; // Max Y
    mfe_reg->reg_mfe_s_ime_sr16 = (mfe_reg->reg_mfe_s_ime_mvx_min>=16 ? 1 : 0);
    mfe_reg->reg_mfe_s_ime_umv_disable = pInfo->no_umv;

    mfe_reg->reg_mfe_s_ime_ime_wait_fme = 1;
#ifdef FME_PIPELINE_OPEN
    mfe_reg->reg_mfe_s_fme_pipeline_on = mfe_reg->reg_mfe_s_ime_ime_wait_fme ? 1 : 0;
#else
    mfe_reg->reg_mfe_s_fme_pipeline_on = 0x0;
#endif

#ifdef CHECK_P8x8_BOUND_RECT
    mfe_reg->reg_mfe_s_ime_boundrect_en = (pInfo->ProfileIDC==66 && pInfo->LevelIDC<=30) ? 1 : 0;
#endif
    mfe_reg->reg_mfe_s_ime_h264_p8x8_ctrl_en = pInfo->nMaxP8x8Count<(mfe_reg->reg_mfe_g_pic_width>>4)*(mfe_reg->reg_mfe_g_pic_height>>4) ? 1 : 0;
    if (mfe_reg->reg_mfe_s_ime_h264_p8x8_ctrl_en)
        mfe_reg->reg_mfe_s_ime_h264_p8x8_max = pInfo->nMaxP8x8Count>>4;
    mfe_reg->reg_mfe_s_mesr_adapt = pInfo->IME_ADAPTIVE_WINDOW?1:0;
    mfe_reg->reg_mfe_s_me_ref_en_mode = pInfo->RefCount==2 ? 0x3 : (pInfo->RefCount==1 ? 0x1 : 0);

    mfe_reg->reg_mfe_s_fme_quarter_disable = (pInfo->fme_precision!=2);
    mfe_reg->reg_mfe_s_fme_half_disable = (pInfo->fme_precision==0);
    //mfe_reg->reg_mfe_s_fme_one_mode = 1;
    mfe_reg->reg_mfe_s_fme_pmv_enable = (/*(PSKIP_PREFERRED==3) &&*/ pInfo->InterSearch[0][0]);
    mfe_reg->reg_mfe_s_fme_mode_no = pInfo->nMaxFmeMode-1;

    mfe_reg->reg_mfe_s_fme_mode0_refno = 1;
    mfe_reg->reg_mfe_s_fme_mode1_refno = 1;
    mfe_reg->reg_mfe_s_fme_mode2_refno = 1;
/*
    if(pInfo->PicInterlace==FIELD_CODING){
        if(pInfo->structure==BOTTOM_FIELD){
            mfe_reg->reg_mfe_s_fme_mode0_refno = 0;
            mfe_reg->reg_mfe_s_fme_mode1_refno = 0;
            mfe_reg->reg_mfe_s_fme_mode2_refno = 0;
        }
    }
*/
	// Intra update
    mfe_reg->reg_mfe_s_txip_irfsh_en = 0;

    if(mfe_reg->reg_mfe_s_txip_irfsh_en)
    {
        MS_S32 i;
        MS_S32 count, prv_intra;
        MS_S32 start[2], end[2];
        IntraUpdateContext* ctx = &pConfig->m_IUContext;

        count = 0;
        prv_intra = 0;
        for (i=0; i<ctx->nTotalMb; i++) {
        	if (ctx->pHwMbMap[i].intra) {
                if (prv_intra==0) {
                    count++;
                    if (count>2) {
                        MS_ASSERT(0);
                    }
                    start[count-1] = end[count-1] = i;
                }
                else
                    end[count-1] = i;
        	}
        	prv_intra = ctx->pHwMbMap[i].intra;
        }
        if (count>0) {
            mfe_reg->reg_mfe_s_txip_irfsh_en |= 1;
            mfe_reg->reg_mfe_s_txip_irfsh_mb_s0 = start[0];
            mfe_reg->reg_mfe_s_txip_irfsh_mb_e0 = end[0];
        }
        if (count>1) {
        	mfe_reg->reg_mfe_s_txip_irfsh_en |= 2;
        	mfe_reg->reg_mfe_s_txip_irfsh_mb_s1 = start[1];
        	mfe_reg->reg_mfe_s_txip_irfsh_mb_e1 = end[1];
        }
    }

    mfe_reg->reg_mfe_s_mdc_h264_nal_ref_idc = pInfo->nal_ref_idc;
    mfe_reg->reg_mfe_s_mdc_h264_nal_unit_type = pInfo->idr_flag ? 1 : 0;
    mfe_reg->reg_mfe_s_mdc_h264_fnum_bits = pInfo->log2_max_frame_num_minus4+4-5;
    mfe_reg->reg_mfe_s_mdc_h264_dbf_control = pInfo->bDeblockCtrlPresent;
    mfe_reg->reg_mfe_s_mdc_h264_fnum_value = pInfo->frame_num;//img->frame_num;
    mfe_reg->reg_mfe_s_mdc_h264_idr_pic_id = (pInfo->number/*img->number*/ % 2);
    mfe_reg->reg_mfe_s_mdc_h264_disable_dbf_idc = pInfo->nDeblockIDC;//img->LFDisableIdc==1?1:2;
    mfe_reg->reg_mfe_s_mdc_h264_alpha = pInfo->nDeblockAlpha;//img->LFAlphaC0Offset/2;
    mfe_reg->reg_mfe_s_mdc_h264_beta = pInfo->nDeblockBeta;//img->LFBetaOffset/2;
    mfe_reg->reg_mfe_s_mdc_h264_ridx_aor_flag = ((pInfo->num_ref_idx_l0_active != (pInfo->num_ref_idx_l0_active_minus1 +1)) ? 1 : 0);

    mfe_reg->reg_mfe_s_quan_idx_last = 63;
    if (mfe_reg->reg_mfe_s_quan_idx_last<63)
        mfe_reg->reg_mfe_s_quan_idx_swlast = 1;
    else
        mfe_reg->reg_mfe_s_quan_idx_swlast = 0;

    // MBR
    mfe_reg->reg_mfe_g_mbr_en = rcCtx->m_rcGranularity==MBLEVELRC ? 1 : 0;
    mfe_reg->reg_mfe_s_mbr_pqp_dlimit = LEFT_QP_DIFF_LIMIT;
    mfe_reg->reg_mfe_s_mbr_uqp_dlimit = TOP_QP_DIFF_LIMIT;

	// er_en
	if (rcCtx->m_nVPMbRow>0 && rcCtx->m_nVPSize<=0)
        mfe_reg->reg_mfe_g_er_mode = 0;
	else if (rcCtx->m_nVPMbRow<=0 && rcCtx->m_nVPSize>0)
        mfe_reg->reg_mfe_g_er_mode = 1;
	else if (rcCtx->m_nVPMbRow>0 && rcCtx->m_nVPSize>0)
        mfe_reg->reg_mfe_g_er_mode = 2;
	else
        mfe_reg->reg_mfe_g_er_mode = 3;
	if (rcCtx->m_nVPMbRow==0 || rcCtx->m_nVPMbRow==1)
        mfe_reg->reg_mfe_g_er_mby = 0;
	else if (rcCtx->m_nVPMbRow==2)
        mfe_reg->reg_mfe_g_er_mby = 1;
	else if (rcCtx->m_nVPMbRow==4)
        mfe_reg->reg_mfe_g_er_mby = 2;
	else if (rcCtx->m_nVPMbRow==8)
        mfe_reg->reg_mfe_g_er_mby = 3;
    else if (rcCtx->m_nVPMbRow>0) {
		MS_ASSERT(0);
        }
    if (mfe_reg->reg_mfe_g_er_mode==1 || mfe_reg->reg_mfe_g_er_mode==2)
        mfe_reg->reg_mfe_g_er_bs_th = rcCtx->m_nVPSize;
    mfe_reg->reg_mfe_g_qscale = pInfo->intQP;
    mfe_reg->reg_mfe_s_mbr_frame_qstep = rcCtx->m_nFrameQStep;
    mfe_reg->reg_mfe_s_mbr_tmb_bits = rcCtx->m_nTargetMbBits;
	// QP/QStep: Min, max
    mfe_reg->reg_mfe_s_mbr_qp_min = rcCtx->m_nMinQP;
    mfe_reg->reg_mfe_s_mbr_qp_max = rcCtx->m_nMaxQP;
	MS_ASSERT(rcCtx->m_nMinQStep<(2<<7));
    mfe_reg->reg_mfe_s_mbr_qstep_min = rcCtx->m_nMinQStep;
    mfe_reg->reg_mfe_s_mbr_qstep_max = rcCtx->m_nMaxQStep;

    mfe_reg->reg_mfe_g_rec_en = 1;

	// IEAP
    mfe_reg->reg_mfe_s_ieap_last_mode = pInfo->ieap_last_mode;
    mfe_reg->reg_mfe_s_ieap_constraint_intra = pInfo->ieap_constraint_intra;
    mfe_reg->reg_mfe_s_ieap_ccest_en = pInfo->ieap_ccest_en;
    mfe_reg->reg_mfe_s_ieap_ccest_thr = pInfo->ieap_ccest_thr;
    mfe_reg->reg_mfe_s_ieap_drop_i16 = pInfo->ieap_drop_i16;
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    mfe_reg->reg_mfe_s_ieap_early_termination = 1;
#endif
	// Field coding
	switch (pInfo->h264_mcc_offset[0]) {
		case 0:
            mfe_reg->reg_mfe_s_mvdctl_ref0_offset = 0;
			break;
		case 2:
            mfe_reg->reg_mfe_s_mvdctl_ref0_offset = 1;
			break;
		case -2:
            mfe_reg->reg_mfe_s_mvdctl_ref0_offset = 2;
			break;
	}
	switch (pInfo->h264_mcc_offset[1]) {
		case 0:
            mfe_reg->reg_mfe_s_mvdctl_ref1_offset = 0;
			break;
		case 2:
            mfe_reg->reg_mfe_s_mvdctl_ref1_offset = 1;
			break;
		case -2:
            mfe_reg->reg_mfe_s_mvdctl_ref1_offset = 2;
			break;
	}

#if defined(MFE_MIU_WRITE_PROTECTION)
    {
        MS_U32 gn_buf_size = (pConfig->nBufWidth / 16) * 128;
        mfe_reg->reg_mfe_s_marb_lbound_3_low   = MEM_BOUND_LO(pBufInfo->m_nGNAddr.miuAddress);
        mfe_reg->reg_mfe_s_marb_lbound_3_high  = MEM_BOUND_HI(pBufInfo->m_nGNAddr.miuAddress);
        mfe_reg->reg_mfe_s_marb_ubound_3_low   = MEM_BOUND_LO(pBufInfo->m_nGNAddr.miuAddress + gn_buf_size);
        mfe_reg->reg_mfe_s_marb_ubound_3_high  = MEM_BOUND_HI(pBufInfo->m_nGNAddr.miuAddress + gn_buf_size);

        // write ports 2 and 3 share the same buffer
        mfe_reg->reg_mfe_s_marb_lbound_2_low   = mfe_reg->reg_mfe_s_marb_lbound_3_low;
        mfe_reg->reg_mfe_s_marb_lbound_2_high  = mfe_reg->reg_mfe_s_marb_lbound_3_high;
        mfe_reg->reg_mfe_s_marb_ubound_2_low   = mfe_reg->reg_mfe_s_marb_ubound_3_low;
        mfe_reg->reg_mfe_s_marb_ubound_2_high  = mfe_reg->reg_mfe_s_marb_ubound_3_high;

        // bsp obuf
        mfe_reg->reg_mfe_s_marb_lbound_0_low   = MEM_BOUND_LO(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress);
        mfe_reg->reg_mfe_s_marb_lbound_0_high  = MEM_BOUND_HI(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress);
        mfe_reg->reg_mfe_s_marb_ubound_0_low   = MEM_BOUND_LO(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress + pBufInfo->m_OutBufferSize);
        mfe_reg->reg_mfe_s_marb_ubound_0_high  = MEM_BOUND_HI(pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress + pBufInfo->m_OutBufferSize);

        // recon frames (between gn buf and bsp obuf)
        mfe_reg->reg_mfe_s_marb_lbound_1_low   = mfe_reg->reg_mfe_s_marb_ubound_3_low;
        mfe_reg->reg_mfe_s_marb_lbound_1_high  = mfe_reg->reg_mfe_s_marb_ubound_3_high;
        mfe_reg->reg_mfe_s_marb_ubound_1_low   = mfe_reg->reg_mfe_s_marb_lbound_0_low;
        mfe_reg->reg_mfe_s_marb_ubound_1_high  = mfe_reg->reg_mfe_s_marb_lbound_0_high;

        mfe_reg->reg_mfe_s_marb_miu_bound_en_0 = 1;
        mfe_reg->reg_mfe_s_marb_miu_bound_en_1 = 1;
        mfe_reg->reg_mfe_s_marb_miu_bound_en_2 = 1;
        mfe_reg->reg_mfe_s_marb_miu_bound_en_3 = 1;
    }
#endif

#ifdef HW_ECO_STARTCODE_PREVENTION
    mfe_reg->reg_eco_bsp_rdy_fix = 1;
#if defined(_MFE_EDISON_)
    //agate U02 cannot set this.
    mfe_reg->reg_eco_bsp_multi_slice_fix = 1;
#endif
#endif
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    mfe_reg->reg_eco_bsp_stuffing = 1;
#endif
	//////////////////////////////////////////////////////////////////////////
	// swcfg1 output
	nTarWriteCount = 0;
	nRegWriteCount = 0;
	nTarFDCCount = 0;
	nRegFDCCount = 0;
	if (nFrmNum==0) {
		nTarWriteCount = 2;
		// Switch to sw mode
        mfe_reg->reg_mfe_g_tbc_mode = 0;
        WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("tbc_mode=0"));
		// Switch to hw mode
        mfe_reg->reg_mfe_g_tbc_mode = 1;
        WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("tbc_mode=1"));
		MS_ASSERT(nRegWriteCount==nTarWriteCount);
	}

	nRegWriteCount = 0;
	nTarFDCCount = PutFDC(mfe_reg, pContext, 1);
	nTarWriteCount = 54+(nTarFDCCount*3);

	nTarWriteCount++;	// reg to set fdc round
    nTarWriteCount++;   // reset 0/1: MPEG4 enable/disable p skip mode

#ifdef CLOCK_GATING
	nTarWriteCount++;
#endif
#ifdef MFE_SUPPORT_TLB
    nTarWriteCount++;   // mfe_reg1.reg36
#endif
#ifdef MFE_YUV_LOADER
    nTarWriteCount += 2;    // mfe_reg1.reg56, mfe_reg1.reg0b
#endif
#ifdef MFE_DBF_PACKED_MODE
    nTarWriteCount++;   // mfe_reg.reg4f
#endif
#if defined(MFE_MIU_WRITE_PROTECTION)
    nTarWriteCount += 16;   // mfe_reg.reg58 ~ mfe_reg.reg67
#endif

	nTarFDCCount *= 3;
	nTarFDCCount++;	// reg to set fdc round

	// SW reset
    mfe_reg->reg_mfe_g_soft_rstz = 0;
    WriteRegMFE(0x0, mfe_reg->reg00, (MS_S8*)("[%d] reg00"), nRegWriteCount++, (MS_S8*)("SW reset 0"));
    mfe_reg->reg_mfe_g_soft_rstz = 1;
    WriteRegMFE(0x0, mfe_reg->reg00, (MS_S8*)("[%d] reg00"), nRegWriteCount++, (MS_S8*)("SW reset 1"));
    WriteRegMFE(0x1, mfe_reg->reg01, (MS_S8*)("[%d] reg01"), nRegWriteCount++, (MS_S8*)("picture width"));
    WriteRegMFE(0x2, mfe_reg->reg02, (MS_S8*)("[%d] reg02"), nRegWriteCount++, (MS_S8*)("picture height"));
    WriteRegMFE(0x3, mfe_reg->reg03, (MS_S8*)("[%d] reg03"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x4, mfe_reg->reg04, (MS_S8*)("[%d] reg04"), nRegWriteCount++, (MS_S8*)("er_bs mode threshold"));
    WriteRegMFE(0x5, mfe_reg->reg05, (MS_S8*)("[%d] reg05"), nRegWriteCount++, (MS_S8*)("inter prediction preference"));

    WriteRegMFE(0x20, mfe_reg->reg20, (MS_S8*)("[%d] reg20"), nRegWriteCount++, (MS_S8*)("ME partition setting"));
    WriteRegMFE(0x21, mfe_reg->reg21, (MS_S8*)("[%d] reg21"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x22, mfe_reg->reg22, (MS_S8*)("[%d] reg22"), nRegWriteCount++, (MS_S8*)("me search range max depth"));
    WriteRegMFE(0x23, mfe_reg->reg23, (MS_S8*)("[%d] reg23"), nRegWriteCount++, (MS_S8*)("me mvx"));
    WriteRegMFE(0x24, mfe_reg->reg24, (MS_S8*)("[%d] reg24"), nRegWriteCount++, (MS_S8*)("me mvy"));
    WriteRegMFE(0x25, mfe_reg->reg25, (MS_S8*)("[%d] reg25"), nRegWriteCount++, (MS_S8*)("FME"));

#ifdef CLOCK_GATING
    WriteRegMFE(0x16, mfe_reg->reg16, (MS_S8*)("[%d] reg16"), nRegWriteCount++, (MS_S8*)("Clock gating"));
#endif

	// Input buffers
	//
    if(pConfig->g_nUseYUV422 == 0) {
        WriteRegMFE(0x06, mfe_reg->reg06, (MS_S8*)("[%d] reg06"), nRegWriteCount++, (MS_S8*)("current luma base address"));
        WriteRegMFE(0x07, mfe_reg->reg07, (MS_S8*)("[%d] reg07"), nRegWriteCount++, (MS_S8*)("current luma base address high"));
        WriteRegMFE(0x08, mfe_reg->reg08, (MS_S8*)("[%d] reg08"), nRegWriteCount++, (MS_S8*)("current chroma base address"));
        WriteRegMFE(0x09, mfe_reg->reg09, (MS_S8*)("[%d] reg09"), nRegWriteCount++, (MS_S8*)("current chroma base address high"));
    }
    WriteRegMFE(0x0a, mfe_reg->reg0a, (MS_S8*)("[%d] reg0a"), nRegWriteCount++, (MS_S8*)("reference luma base address0"));
    WriteRegMFE(0x0b, mfe_reg->reg0b, (MS_S8*)("[%d] reg0b"), nRegWriteCount++, (MS_S8*)("reference luma base address0 high"));
    WriteRegMFE(0x0c, mfe_reg->reg0c, (MS_S8*)("[%d] reg0c"), nRegWriteCount++, (MS_S8*)("reference luma base address1"));
    WriteRegMFE(0x0d, mfe_reg->reg0d, (MS_S8*)("[%d] reg0d"), nRegWriteCount++, (MS_S8*)("reference luma base address1 high"));
    WriteRegMFE(0x0e, mfe_reg->reg0e, (MS_S8*)("[%d] reg0e"), nRegWriteCount++, (MS_S8*)("reference chroma base address0"));
    WriteRegMFE(0x0f, mfe_reg->reg0f, (MS_S8*)("[%d] reg0f"), nRegWriteCount++, (MS_S8*)("reference chroma base address0 high"));
    WriteRegMFE(0x10, mfe_reg->reg10, (MS_S8*)("[%d] reg10"), nRegWriteCount++, (MS_S8*)("reference chroma base address1"));
    WriteRegMFE(0x11, mfe_reg->reg11, (MS_S8*)("[%d] reg11"), nRegWriteCount++, (MS_S8*)("reference chroma base address1 high"));
    WriteRegMFE(0x12, mfe_reg->reg12, (MS_S8*)("[%d] reg12"), nRegWriteCount++, (MS_S8*)("reconstructed luma base address:"));
    WriteRegMFE(0x13, mfe_reg->reg13, (MS_S8*)("[%d] reg13"), nRegWriteCount++, (MS_S8*)("reconstructed luma base address high"));
    WriteRegMFE(0x14, mfe_reg->reg14, (MS_S8*)("[%d] reg14"), nRegWriteCount++, (MS_S8*)("reconstructed chroma base address:"));
    WriteRegMFE(0x15, mfe_reg->reg15, (MS_S8*)("[%d] reg15"), nRegWriteCount++, (MS_S8*)("reconstructed chroma base address: high"));

#if defined(_MFE_T8_)&&!defined(_MFE_AGATE_)
	// Output buffer
    WriteRegMFE(0x3c, mfe_reg->reg3c, (MS_S8*)("[%d] reg3c"), nRegWriteCount++, (MS_S8*)("bsp obuf start address: "));
    WriteRegMFE(0x3d, mfe_reg->reg3d, (MS_S8*)("[%d] reg3d"), nRegWriteCount++, (MS_S8*)("bsp obuf start address high"));
    WriteRegMFE(0x3e, mfe_reg->reg3e, (MS_S8*)("[%d] reg3e"), nRegWriteCount++, (MS_S8*)("bsp obuf end address: "));
    WriteRegMFE(0x3f, mfe_reg->reg3f, (MS_S8*)("[%d] reg3f"), nRegWriteCount++, (MS_S8*)("bsp obuf end address high"));
    mfe_reg->reg_mfe_s_bspobuf_set_adr = 1;
#endif
	//
    mfe_reg->reg_mfe_s_bspobuf_fifo_th = 1;
    mfe_reg->reg_mfe_s_mvobuf_set_adr = 0;
    mfe_reg->reg_mfe_s_mvobuf_fifo_th = 0;

#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    if(!pConfig->SecurityMode)
#endif
    {
#if defined(_MFE_T8_)&&!defined(_MFE_AGATE_)
    WriteRegMFE(0x3b, mfe_reg->reg3b, (MS_S8*)("[%d] reg3b"), nRegWriteCount++, (MS_S8*)("set bsp obuf"));
       mfe_reg->reg_mfe_s_bspobuf_set_adr = 0;    // HW is write-one-clear
#elif defined(_MFE_M1_)||defined(_MFE_AGATE_)
    // Enable set-obuf
    mfe_reg->reg_mfe_s_bspobuf_update_adr = 1;
    WriteRegMFE(0x3f, mfe_reg->reg3f, (MS_S8*)("[%d] reg3f"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_bspobuf_update_adr"));
    mfe_reg->reg_mfe_s_bspobuf_update_adr = 0;  // write-one-clear
#if defined(WIN32)
    mfe_reg->enable_obufadr_update = 0;
    UDMA_RIURead16(REG_BANK_MFE+0x6a, (MS_U16*)&mfe_reg->reg6a);
    while (mfe_reg->enable_obufadr_update!=1) {
        UDMA_RIURead16(REG_BANK_MFE+0x6a, (MS_U16*)&mfe_reg->reg6a);
        printf("Wait for enable_obufadr_update=1!\n"));
    }
#endif
#if defined(USE_HW_DBL_OBUF)
    nRegWriteCount += SetObufAddr((MS_U32)pBufInfo->m_nOutBufAddr, pBufInfo->m_OutBufferSize, 0, 0);
    nRegWriteCount += SetObufAddr((MS_U32)pBufInfo->m_nOutBufAddr+pBufInfo->m_OutBufferSize, pBufInfo->m_OutBufferSize, 1, 1);
#else
/*
#define CEILING_ALIGN(value, align) (((MEF_U32)(value)+((align)-1UL)) & ~((align)-1UL))
#define IN_SIZE (CEILING_ALIGN(720UL,32UL)*1280UL+CEILING_ALIGN(720UL,64UL)*1280UL/2UL+0x100UL)

    pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress = 0x000ABEA000 + IN_SIZE;
    pBufInfo->m_OutBufferSize = 1024*1024;
*/
    nRegWriteCount += SetObufAddr(mfe_reg, (MS_U32)pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress, pBufInfo->m_OutBufferSize, 0, 1);
#endif
#endif
    }
	// GN
    WriteRegMFE(0x4c, mfe_reg->reg4c, (MS_S8*)("[%d] reg4c"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_gn_sadr_low"));
    WriteRegMFE(0x4d, mfe_reg->reg4d, (MS_S8*)("[%d] reg4d"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_gn_sadr_high"));

	// MBR
    WriteRegMFE(0x26, mfe_reg->reg26, (MS_S8*)("[%d] reg26"), nRegWriteCount++, (MS_S8*)("MBR: mbbits"));
    WriteRegMFE(0x27, mfe_reg->reg27, (MS_S8*)("[%d] reg27"), nRegWriteCount++, (MS_S8*)("MBR: frame qstep"));
    WriteRegMFE(0x29, mfe_reg->reg29, (MS_S8*)("[%d] reg29"), nRegWriteCount++, (MS_S8*)("264 qp-offset"));
    WriteRegMFE(0x2a, mfe_reg->reg2a, (MS_S8*)("[%d] reg2a"), nRegWriteCount++, (MS_S8*)("QP min/max"));
    WriteRegMFE(0x6e, mfe_reg->reg6e, (MS_S8*)("[%d] reg6e"), nRegWriteCount++, (MS_S8*)("QStep min"));
    WriteRegMFE(0x6f, mfe_reg->reg6f, (MS_S8*)("[%d] reg6f"), nRegWriteCount++, (MS_S8*)("QStep max"));

	// MDC
    WriteRegMFE(0x39, mfe_reg->reg39, (MS_S8*)("[%d] reg39"), nRegWriteCount++, (MS_S8*)("value"));

	// Intra Update
    WriteRegMFE(0x2f, mfe_reg->reg2f, (MS_S8*)("[%d] reg2f"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x30, mfe_reg->reg30, (MS_S8*)("[%d] reg30"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x31, mfe_reg->reg31, (MS_S8*)("[%d] reg31"), nRegWriteCount++, (MS_S8*)("value"));
    WriteRegMFE(0x32, mfe_reg->reg32, (MS_S8*)("[%d] reg32"), nRegWriteCount++, (MS_S8*)("value"));

	// DBF
    WriteRegMFE(0x3a, mfe_reg->reg3a, (MS_S8*)("[%d] reg3a"), nRegWriteCount++, (MS_S8*)("value"));

    mfe_reg->reg_mfe_g_crc_mode = 0xC;
    mfe_reg->reg_mfe_g_debug_tcycle_chk_en = 0x1;
//#ifdef TEST_CRC_MODE
    mfe_reg->reg_mfe_g_debug_tcycle_chk_sel = 0x0;
//#endif

    mfe_reg->reg_mfe_g_debug_en = 0; // TEST
    WriteRegMFE(0x73, mfe_reg->reg73, (MS_S8*)("[%d] reg73"), nRegWriteCount++, (MS_S8*)("crc mode"));

    //mfe_reg->reg_mfe_s_txip_idle_cnt = 160;
    //WriteRegMFE(0x33, mfe_reg->reg33, (MS_S8*)("[%d] reg33"), nRegWriteCount++, (MS_S8*)("txip_idle_cnt"));
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    mfe_reg->reg_mfe_g_ieap_sram_4x2_swap = 1;
#endif
    WriteRegMFE(0x2c, mfe_reg->reg2c, (MS_S8*)("[%d] reg2c"), nRegWriteCount++, (MS_S8*)("Last zigzag"));

	// IEAP
    WriteRegMFE(0x2b, mfe_reg->reg2b, (MS_S8*)("[%d] reg2b"), nRegWriteCount++, (MS_S8*)("ieap"));

	// Cross-format wrong reg setting prevention
    mfe_reg->reg_mfe_g_viu_soft_rstz = 1;
    WriteRegMFE(0x18, mfe_reg->reg18, (MS_S8*)("[%d] reg18"), nRegWriteCount++, (MS_S8*)("JPE encode mode"));
    WriteRegMFE(0x1b, mfe_reg->reg1b, (MS_S8*)("[%d] reg1b"), nRegWriteCount++, (MS_S8*)("MPEG4 FieldDCT"));
    MS_ASSERT(mfe_reg->reg19==0);
	WriteRegMFE(0x19,0, (MS_S8*)("[%d] reg19"), nRegWriteCount++, (MS_S8*)("0/1: MPEG4 enable/disable p skip mode"));
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
    WriteRegMFE_BANK1(0x52, mfe_reg1->reg52, (MS_S8*)("[%d] reg52"), nRegWriteCount++, (MS_S8*)("YUV422 and handshake mode"));

    if(pConfig->g_nUseYUV422 != 0) {
        WriteRegMFE_BANK1(0x12, mfe_reg1->reg12, (MS_S8*)("[%d] reg12"), nRegWriteCount++, (MS_S8*)("enc curr_y addr"));
        WriteRegMFE_BANK1(0x13, mfe_reg1->reg13, (MS_S8*)("[%d] reg13"), nRegWriteCount++, (MS_S8*)("enc curr_y addr"));
    }
#endif

#if defined(MFE_YUV_LOADER)
    WriteRegMFE_BANK1(0x0b, mfe_reg1->reg0b, (MS_S8*)("[%d] reg0b"), nRegWriteCount++, (MS_S8*)("YUV422 pitch"));
    WriteRegMFE_BANK1(0x56, mfe_reg1->reg56, (MS_S8*)("[%d] reg56"), nRegWriteCount++, (MS_S8*)("YUV Loader"));
#endif

#ifdef _MFE_T8_
    if (mfe_reg->reg_mfe_g_enc_mode !=REG_ENC_MODE_MPG4) {    // MPEG-4
        WriteRegMFE(0x37, mfe_reg->reg37=0, (MS_S8*)("[%d] reg37"), nRegWriteCount, (MS_S8*)("MPEG4 MDC"));
        WriteRegMFE(0x38, mfe_reg->reg38=0, (MS_S8*)("[%d] reg38"), nRegWriteCount, (MS_S8*)("MPEG4: vop_time_increment"));
		// B-direct
        WriteRegMFE(0x1a, mfe_reg->reg1a=0, (MS_S8*)("[%d] reg1a"), nRegWriteCount, (MS_S8*)("MPEG4 BDirect"));
	}
#endif
#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
    // Prefetch & Low bandwidth mode
    WriteRegMFE(0x68,mfe_reg->reg68, (MS_S8*)("[%d] reg68"), nRegWriteCount++, (MS_S8*)("Prefetch & Low bandwidth mode"));
    // Prefetch
    WriteRegMFE(0x6d,mfe_reg->reg6d, (MS_S8*)("[%d] reg6d"), nRegWriteCount++, (MS_S8*)("Prefetch MB idle count"));
    //Low BandWidth
    WriteRegMFE(0x6b, mfe_reg->reg6b, (MS_S8*)("[%d] reg6b"), nRegWriteCount++, (MS_S8*)("Low Bandwidth: IMI addr low"));
    WriteRegMFE(0x6c, mfe_reg->reg6c, (MS_S8*)("[%d] reg6c"), nRegWriteCount++, (MS_S8*)("Low Bandwidth: IMI addr high"));

#if defined(MFE_DBF_PACKED_MODE)
    WriteRegMFE(0x4f, mfe_reg->reg4f, (MS_S8*)("[%d] reg4f"), nRegWriteCount++, (MS_S8*)("DBF packed mode"));
#endif

    // Reset any StopAndGo or StopAndDrop setting.
    mfe_reg->reg_mfe_s_txip_sng_mb = 0;
    WriteRegMFE(0x2d, mfe_reg->reg2d, (MS_S8*)("[%d] reg2d"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_txip_sng_mb=0"));

#endif
    //hw bug, disable it.
    mfe_reg->reg_mfe_s_mcc_prldr_mode = 0;
    WriteRegMFE(0x7f, mfe_reg->reg7f, (MS_S8*)("[%d] reg7f"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_mcc_prldr_mode=0"));
    mfe_reg->reg_mfe_s_marb_rp0_promote = 0x0;
    mfe_reg->reg_mfe_s_marb_rp1_promote = 0x0;
    mfe_reg->reg_mfe_s_marb_rp2_promote = 0x0;
    mfe_reg->reg_mfe_s_marb_rp3_promote = 0x0;
    mfe_reg->reg_mfe_s_marb_mrpriority_thd = 0xf;
    mfe_reg->reg_mfe_s_marb_mwpriority_thd = 0xf;
    mfe_reg->reg_mfe_s_marb_rp4_occupy = 0x0;
    mfe_reg->reg_mfe_s_marb_rp4_promote = 0x0;
    WriteRegMFE(0x51, mfe_reg->reg51, (MS_S8*)("[%d] reg51"), nRegWriteCount++, (MS_S8*)("marb_rp_promote"));
    WriteRegMFE(0x55, mfe_reg->reg55, (MS_S8*)("[%d] reg55"), nRegWriteCount++, (MS_S8*)("marb_rp4_promote"));

    //enable eco item
    WriteRegMFE(0x7d, mfe_reg->reg7d, (MS_S8*)("[%d] reg7d"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_txip_eco0=1"));

#if defined(TEST_MB_STOPANDGO) || defined(TEST_MB_STOPANDDROP)
	if (nFrmNum==STOP_FRAME)
	TestStopAtMb();
#endif
#ifdef MFE_MIU_PROFILE
    mfe_reg1->reg_mfe_s_rhist_en = 1;
    WriteRegMFE_BANK1(0x45, mfe_reg1->reg45, (MS_S8*)("[%d] reg45"), nRegWriteCount++, (MS_S8*)("Read histogram enable"));
    mfe_reg1->reg_mfe_s_whist_en = 1;
    WriteRegMFE_BANK1(0x40, mfe_reg1->reg40, (MS_S8*)("[%d] reg40"), nRegWriteCount++, (MS_S8*)("Write histogram enable"));
#endif

#ifdef MFE_SUPPORT_TLB
    mfe_reg1->reg_mfe_tlb = pConfig->bEnableTLB ? 1 : 0;
    WriteRegMFE_BANK1(0x36, mfe_reg1->reg36, (MS_S8*)("[%d] reg36"), nRegWriteCount++, (MS_S8*)("Enable TLB"));
#endif

#if defined(MFE_MIU_WRITE_PROTECTION)
    WriteRegMFE(0x58, mfe_reg->reg58, (MS_S8*)("[%d] reg58"), nRegWriteCount++, (MS_S8*)("MIU upper bound 0"));
    WriteRegMFE(0x59, mfe_reg->reg59, (MS_S8*)("[%d] reg59"), nRegWriteCount++, (MS_S8*)("MIU upper bound 0"));
    WriteRegMFE(0x5a, mfe_reg->reg5a, (MS_S8*)("[%d] reg5a"), nRegWriteCount++, (MS_S8*)("MIU lower bound 0"));
    WriteRegMFE(0x5b, mfe_reg->reg5b, (MS_S8*)("[%d] reg5b"), nRegWriteCount++, (MS_S8*)("MIU lower bound 0"));
    WriteRegMFE(0x5c, mfe_reg->reg5c, (MS_S8*)("[%d] reg5c"), nRegWriteCount++, (MS_S8*)("MIU upper bound 1"));
    WriteRegMFE(0x5d, mfe_reg->reg5d, (MS_S8*)("[%d] reg5d"), nRegWriteCount++, (MS_S8*)("MIU upper bound 1"));
    WriteRegMFE(0x5e, mfe_reg->reg5e, (MS_S8*)("[%d] reg5e"), nRegWriteCount++, (MS_S8*)("MIU lower bound 1"));
    WriteRegMFE(0x5f, mfe_reg->reg5f, (MS_S8*)("[%d] reg5f"), nRegWriteCount++, (MS_S8*)("MIU lower bound 1"));
    WriteRegMFE(0x60, mfe_reg->reg60, (MS_S8*)("[%d] reg60"), nRegWriteCount++, (MS_S8*)("MIU upper bound 2"));
    WriteRegMFE(0x61, mfe_reg->reg61, (MS_S8*)("[%d] reg61"), nRegWriteCount++, (MS_S8*)("MIU upper bound 2"));
    WriteRegMFE(0x62, mfe_reg->reg62, (MS_S8*)("[%d] reg62"), nRegWriteCount++, (MS_S8*)("MIU lower bound 2"));
    WriteRegMFE(0x63, mfe_reg->reg63, (MS_S8*)("[%d] reg63"), nRegWriteCount++, (MS_S8*)("MIU lower bound 2"));
    WriteRegMFE(0x64, mfe_reg->reg64, (MS_S8*)("[%d] reg64"), nRegWriteCount++, (MS_S8*)("MIU upper bound 3"));
    WriteRegMFE(0x65, mfe_reg->reg65, (MS_S8*)("[%d] reg65"), nRegWriteCount++, (MS_S8*)("MIU upper bound 3"));
    WriteRegMFE(0x66, mfe_reg->reg66, (MS_S8*)("[%d] reg66"), nRegWriteCount++, (MS_S8*)("MIU lower bound 3"));
    WriteRegMFE(0x67, mfe_reg->reg67, (MS_S8*)("[%d] reg67"), nRegWriteCount++, (MS_S8*)("MIU lower bound 3"));
#endif

    //DumpAllReg();
    // Enable HW
    mfe_reg->reg_mfe_g_frame_start_sw = 1;
    WriteRegMFE(0x00, mfe_reg->reg00, (MS_S8*)("[%d] reg00"), nRegWriteCount++, (MS_S8*)("frame start"));
    mfe_reg->reg_mfe_g_frame_start_sw = 0;    // HW is write-one-clear


	nRegFDCCount = PutFDC(mfe_reg, pContext, 0);
	nRegWriteCount += nRegFDCCount;

#if defined(TEST_STOPANDGO) || defined(TEST_STOPANDDROP)
	if (nFrmNum==STOP_FRAME)
	TestStop();
#endif

    if(nRegFDCCount != nTarFDCCount) {
        ms_dprintk(DRV_L3, "nRegFDCCount = %d, nTarFDCCount = %d\n", (int)nRegFDCCount, (int)nTarFDCCount);
    }
    if(nRegWriteCount != nTarWriteCount) {
        ms_dprintk(DRV_L3, "nRegWriteCount = %d, nTarWriteCount = %d\n", (int)nRegWriteCount, (int)nTarWriteCount);
    }
	// Only for debug
    //MS_ASSERT(nRegFDCCount==nTarFDCCount);
    //MS_ASSERT(nRegWriteCount==nTarWriteCount);
}


