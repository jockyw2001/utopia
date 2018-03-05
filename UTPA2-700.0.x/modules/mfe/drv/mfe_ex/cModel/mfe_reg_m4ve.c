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

#include "msRateCtrl.h"
#include "OutStrm.h"
#include "m4v_header.h"
#include "BufMng.h"
#include "mfe_reg.h"
#if defined(WIN32)
	#include "mdrv_mfe.h"
#elif defined(_MFE_T8_) && defined(_MIPS_PLATFORM_) &&!defined(_KERNEL_MODE_)
    #include "mdrv_mfe.h"
#endif
#include "msRateCtrl.h"
#if defined(_MFE_T8_)
#define MAX_REF_FRAME  2UL
#define SEARCH_RANGE_X 32UL
#elif defined(_MFE_M1_)
#define MAX_REF_FRAME  1UL
#define SEARCH_RANGE_X 16UL
#endif

// Sync with cModel QExp.h
#define CLOCK_GATING	// Enable clock gating
#define FME_PIPELINE_OPEN	// Enable David's FME speedup version



#define FIELD_DCT_DIFF_THR  (350UL>>2)	// Subsampled by 4



static const MS_S32 rgiDefaultIntraQMatrix [64] = {
	8,	17,	18,	19,	21,	23,	25,	27,
	17,	18,	19,	21,	23,	25,	27,	28,
	20,	21,	22,	23,	24,	26,	28,	30,
	21,	22,	23,	24,	26,	28,	30,	32,
	22,	23,	24,	26,	28,	30,	32,	35,
	23,	24,	26,	28,	30,	32,	35,	38,
	25,	26,	28,	30,	32,	35,	38,	41,
	27,	28,	30,	32,	35,	38,	41,	45
};

static const MS_S32 rgiDefaultInterQMatrix [64] = {
	16,	17,	18, 19, 20,	21,	22,	23,
	17,	18,	19,	20,	21,	22,	23,	24,
	18,	19,	20,	21,	22,	23,	24,	25,
	19,	20,	21,	22,	23,	24,	26,	27,
	20,	21,	22,	23,	25,	26,	27,	28,
	21,	22,	23,	24,	26,	27,	28,	30,
	22,	23,	24,	26,	27,	28,	30,	31,
	23,	24,	25,	27,	28,	30,	31,	33
};
#ifdef MFE_MIU_PROTECT
extern void MHal_MFE_Enable_MIU_Protection(MS_S32 MIU_TEST_MODE,MFE_CONFIG* pConfig);
#endif
void OutputSwCfg1_Mp4(MS_S32 nFrmNum, MFE_CONFIG* pConfig);

static MS_U32 CeilLog2( MS_U32 uiVal)
{
	MS_U32 uiTmp = uiVal-1;
	MS_U32 uiRet = 0;

	while( uiTmp != 0 )
	{
		uiTmp >>= 1;
		uiRet++;
	}
	return uiRet;
}

void mfeM4VE_DeInit(MFE_CONFIG *pConfig)
{
    IntraUpdateClose(pConfig);
}

void mfeM4VE_Init(MFE_CONFIG *pConfig)
{
	MS_S32 i;
	M4VEINFO* pInfo = &pConfig->ctxM4veInfo;

	memset(pInfo, 0, sizeof(M4VEINFO));

	switch (pConfig->nCodecType)
	{
	case REG_ENC_MODE_MPG4:
		pInfo->bShortHeader = 0;
             pConfig->bQuantType = 1;
		break;
	case REG_ENC_MODE_H263:
		pInfo->bShortHeader = 1;
             pConfig->bQuantType = 0;
		break;
	default:
		MS_ASSERT(0);
		pInfo->bShortHeader = 0;
	}

	switch (pConfig->FrameRatex100) {
	case 3000:
		pInfo->nTimeResolution = 30;
		pInfo->nFixedIncrement = 1;
		pInfo->iClockRate = 30;
		break;
	case 1500:
		pInfo->nTimeResolution = 15;
		pInfo->nFixedIncrement = 1;
		pInfo->iClockRate = 15;
		break;
	case 2500:
		pInfo->nTimeResolution = 25;
		pInfo->nFixedIncrement = 1;
		pInfo->iClockRate = 25;
		break;
	case 2997:
		pInfo->nTimeResolution = 30000;
		pInfo->nFixedIncrement = 1001;
		pInfo->iClockRate = 2997;
		break;
	default:
            if(pConfig->nCodecType == REG_ENC_MODE_MPG4) {
                pInfo->nTimeResolution = pConfig->TimeIncreamentRes;
                pInfo->nFixedIncrement = pConfig->VopTimeIncreament;
                 ms_dprintk(DRV_L2,"Setting from API: pInfo->nTimeResolution = %d, pInfo->nFixedIncrement = %d\n",
                    (int)pInfo->nTimeResolution, (int)pInfo->nFixedIncrement);
            pInfo->iClockRate = pInfo->nTimeResolution;
            }
            else {
                pInfo->nTimeResolution = pConfig->FrameRatex100 / 100;
                pInfo->nFixedIncrement = 1;
                pInfo->iClockRate = pInfo->nTimeResolution;
            }
             MS_ASSERT(pInfo->nTimeResolution>0);
             MS_ASSERT(pInfo->nFixedIncrement>0);
	}
	{
		MS_S32 iClockRate = pInfo->iClockRate-1;
		MS_ASSERT (iClockRate < 65536);
		//printf("mfeM4VE_Init: iClockRate=%d\n"), iClockRate);
		if(iClockRate>0)
		{
			for (pInfo->nNumBitsTimeIncr = 1; pInfo->nNumBitsTimeIncr < 16; pInfo->nNumBitsTimeIncr++)	{
				if (iClockRate == 1)
					break;
				iClockRate = (iClockRate >> 1);
			}
		}
		else
			pInfo->nNumBitsTimeIncr = 1;
		//printf("mfeM4VE_Init: nNumBitsTimeIncr=%d\n"), pInfo->nNumBitsTimeIncr);
	}

	if (pConfig->nCodecType==REG_ENC_MODE_MPG4) {
		pInfo->bInterlacedCoding = (pConfig->bInterlace != PROGRESSIVE);
		pInfo->bInterlace = pInfo->bInterlacedCoding;
		pInfo->iSearchRangeForward = SEARCH_RANGE_X;
		pInfo->bQuantizerType = pConfig->bQuantType;

		for (i=0; i<64; i++) {
			pInfo->rgiIntraQuantizerMatrix[i] = rgiDefaultIntraQMatrix[i];
			pInfo->rgiInterQuantizerMatrix[i] = rgiDefaultInterQMatrix[i];
		}

		pInfo->iRoundingControlSwitch = 1;
	}
	else {
		pInfo->iSearchRangeForward = 16;
		pInfo->bQuantizerType = 0;

		pInfo->iRoundingControlSwitch = 0;
	}

	pInfo->SEARCH_RANGE_Y = 16;
	pInfo->IME_ADAPTIVE_WINDOW = 1;
	if (pInfo->SEARCH_RANGE_Y<=16 && pInfo->iSearchRangeForward<=16)
		pInfo->iFCode = 1;
	else
		pInfo->iFCode = 2;

	pInfo->iRoundingControl = pInfo->iRoundingControlSwitch;
	pConfig->vopPredType = I_VOP;
	pInfo->g_rec_en = 1;
	pInfo->g_ref_en = 0;
	pInfo->m_nLastZZ = 63;
	pInfo->m_nFmePrec = 1;	// Default: Half-pixel

	pInfo->m_nBitsResyncMarker = 0;
	pInfo->nNumBitsVPMBnum = CeilLog2((pConfig->nBufWidth*pConfig->nBufHeight)>>8);	// number of bits for macroblock_number

	pInfo->m_tModuloBaseDecd = 0;
	pInfo->m_tModuloBaseDisp = 0;
	pInfo->m_tFutureRef = 0;
	pInfo->m_tPastRef = 0;
	pInfo->m_t = 0;
	pInfo->m_nBitsModuloBase = 0;
	pInfo->m_iVopTimeIncr = 0;

	pInfo->intQP = 0;

	MS_ASSERT((pConfig->nBufWidth&0xF)==0);
	MS_ASSERT((pConfig->nBufHeight&0xF)==0);

	pInfo->bAllowSkippedPMBs = !(pConfig->nBbetweenP>0);
	pInfo->nAllowDirectBMBs = 1;

	pInfo->bHECEnabled = 1;
	pInfo->nHECPeriod = 3;

	// H263
	if (pConfig->nCodecType==REG_ENC_MODE_H263) {
		if (pConfig->nBufHeight<=400)
			pInfo->nGobUnit = 0;
		else if (pConfig->nBufHeight<=800)
			pInfo->nGobUnit = 1;
		else if (pConfig->nBufHeight<=1152)
			pInfo->nGobUnit = 2;
		else
			MS_ASSERT(0);
		pInfo->m_iGobFrameId = 0;
	}

	// Rate control
	MfeDrvRateControlInit(pConfig);

	// FDC header initialization
	osCreate(&pConfig->m_OutStream);
	osSetWriteBuffer(&pConfig->m_OutStream, pConfig->m_FdcBuffer);

	// Intra-update initialization
	IntraUpdateInit(pConfig);
}


void mfeM4VE_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo)
{
    M4VEINFO* pInfo = &pConfig->ctxM4veInfo;
    BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
    OutStream* pStream = &pConfig->m_OutStream;
    const MS_U8 gBITMASK[8] = { 0x0, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe };
	// This is done in msAPI_MFE_EnOneFrm()
	//mfeSetVopType(pConfig, pGopInfo); // Will set pConfig->vopPredType

	// Config
	// Frame type
	pInfo->bInterlace = pInfo->bInterlacedCoding;
	if (!pInfo->bShortHeader) {
		if (pConfig->vopPredType==P_VOP) {
			pInfo->iRoundingControlSwitch ^= 1;
			pInfo->iRoundingControl = pInfo->iRoundingControlSwitch;
		}
		else if (pConfig->vopPredType==B_VOP)
			pInfo->iRoundingControl = 0;
	}
	pInfo->g_rec_en = (MS_U8)((pConfig->nPbetweenI==0 || pConfig->vopPredType==B_VOP) ? /*0*/(pGopInfo->nCodingOrder&1) : 1);	// nCodingOrder&1 is for testing.
	pInfo->g_ref_en = pConfig->vopPredType==I_VOP ? 0 : (pConfig->vopPredType==P_VOP?1:3);
	pInfo->bMvStore = (pConfig->nBbetweenP>0) & ((pConfig->vopPredType==P_VOP) | (/*IVOP_MVSTORE*/1 & (pConfig->vopPredType==I_VOP) & (pGopInfo->nCodingOrder!=0)));

#ifdef _GenSkipHeader_
    if(pConfig->VTMode && rc_CheckSkippedFrame(&pConfig->VTRateCtrl)){
        codeNonCodedVOPShortHead(pConfig, pStream);
        // Finalize
        osFlushAll(&pConfig->m_OutStream);
        return;
    }
#endif
    if(pConfig->VTMode) {
        MS_S8 chFrameType;
        if(pConfig->vopPredType==I_VOP)
            chFrameType = 'I';
        else
            chFrameType = 'P';
        {
            //update original ratecontrol struct for set reg.
            CVBRRateControl* rcCtx = &pConfig->ctxRateControl;
            RateCtrl_t* rcVTCtx = &pConfig->VTRateCtrl;
            rcCtx->m_rcGranularity=MBLEVELRC;
            rcCtx->m_nFrameQStep = rcQP2Qstep(rcCtx,rc_InitFrame(&pConfig->VTRateCtrl, chFrameType));
            rcCtx->m_nTargetMbBits = rcVTCtx->m_nTargetBits / rcVTCtx->m_nNFrame;
            rcCtx->m_nMinQP = SPEC_MIN_QP;
            rcCtx->m_nMaxQP = SPEC_MAX_QP;
            rcCtx->m_nMinQStep = SPEC_MIN_QP<<QS_SHIFT_FACTOR;
            rcCtx->m_nMaxQStep = SPEC_MAX_QP<<QS_SHIFT_FACTOR;
            rcCtx->m_nCodecType = pConfig->nCodecType;
            pInfo->intQP = rcQstep2QP(rcCtx, rcCtx->m_nFrameQStep);
            ms_dprintf(DRV_L3, "m_rcGranularity = %d, rcCtx->m_nTargetMbBits = %d",
                rcCtx->m_rcGranularity, (int)rcCtx->m_nTargetMbBits);
        }
    }
    else
        pInfo->intQP = cvbr_InitFrame(&pConfig->ctxRateControl, pConfig->vopPredType, 0);

	// Frame time
	pInfo->m_t = pGopInfo->nDispOrder;
	if (pConfig->vopPredType != B_VOP) {
		pInfo->m_tPastRef = pInfo->m_tFutureRef;
		pInfo->m_tFutureRef = pInfo->m_t;
	}
	// Handled in codeVOPHeadInitial()
	//m_tModuloBaseDecd
	//m_tModuloBaseDisp
	//m_nBitsModuloBase
	//m_iVopTimeIncr

	// VideoPacket, GOB parameters
	pInfo->m_nBitsResyncMarker = 17/*NUMBITS_VP_RESYNC_MARKER*/;
	if(pConfig->vopPredType == P_VOP)
		pInfo->m_nBitsResyncMarker += (pInfo->iFCode - 1);
	else if(pConfig->vopPredType == B_VOP) {
		// tung : VP 4.21
		pInfo->m_nBitsResyncMarker += (pInfo->iFCode - 1);
		if (pInfo->m_nBitsResyncMarker<18)
			pInfo->m_nBitsResyncMarker = 18;
		// ~tung
	}

	// H263
	pInfo->m_iGobFrameId = (pConfig->vopPredType==P_VOP ? 1 : 0);

	// Buffer management
	m4veGetBufferAddr(pConfig);

	//MODE 0: (Checking range > real range ) 	reg_mfe_s_marb_miu_bound_err = 0
	//MODE 1: (Checking range < real range ) 	reg_mfe_s_marb_miu_bound_err = 1
	//#define TEST_MIU_PROTECTION_MODE 0UL
#ifdef MFE_MIU_PROTECT
	MHal_MFE_Enable_MIU_Protection(TEST_MIU_PROTECTION_MODE,pConfig);
#endif


	// Prepare header
	osReset(&pConfig->m_OutStream);
	if (!pInfo->bShortHeader)
	{
		// VOS and VO header
		if (pGopInfo->nCodingOrder==0)
		{
			codeSequenceHead(pConfig, pStream);
			codeVOHead(pConfig, pStream);
		}
		// VOL header
#if defined(_MFE_T8_)
		if (pGopInfo->nCodingOrder==0||pConfig->vopPredType==I_VOP)
#else
		if (pGopInfo->nCodingOrder==0)
#endif
		{
			codeVOLHead(pConfig, pStream);
		}
	}

	// VOP header
	if (pConfig->m_cvbrFrameSkip>0 && pConfig->vopPredType==P_VOP) {
		if (pConfig->m_bGenSkipVopHeader) {
			if (pInfo->bShortHeader)
				codeNonCodedVOPShortHead(pConfig, pStream);
			else
				codeNonCodedVOPHead(pConfig, pStream);
		}
	}
	else {
		if (pInfo->bShortHeader)
			codeVOPShortHead(pConfig, pStream);
		else
			codeVOPHead(pConfig, pStream);
	}
	// Finalize
	osFlushAll(&pConfig->m_OutStream);
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
/*
#if DEBUG_LEVEL
	{	// DEBUG codes
		MS_S32 i;
		ms_dprintk(DRV_L3,"FDC bitcount %d ==> "), pBitsInfo->len*8+pBitsInfo->bit_len);
		for (i=0; i<pBitsInfo->len; i++) {
			ms_dprintk(DRV_L3,"%02x "), pBitsInfo->ptr[i]);
		}
		ms_dprintk(DRV_L3,"%02x(msb %d bits)\n"), pBitsInfo->bits, pBitsInfo->bit_len);
	}
#endif
*/
	// Set reg and start encoding
	OutputSwCfg1_Mp4(pGopInfo->nCodingOrder, pConfig);
	ms_dprintk(DRV_L3,"In mfeM4VE_EncodeFrame() After OutputSwCfg1_Mp4\n");
}



void mfeM4VE_UpdateFrame(MFE_CONFIG *pConfig)
{
	if (pConfig->vopPredType==I_VOP)
		IntraUpdateInit(pConfig);
	IntraUpdateFrame(pConfig);

	// Rate control
	MfeDrvRateControlUpdate(pConfig, 0);

}

// Trd must >= Trb, no checking here.
static void TrbTrdReduction(MS_S32* Trb, MS_S32* Trd)
{
	MS_S32 r;
	MS_S32 d = *Trd;
	MS_S32 b = *Trb;

	if (b==d) {
		*Trb = *Trd = 1;
		return;
	}

	// b < d
	while(1) {
		r = d%b;
		if(r==0) {
			*Trb /= b;
			*Trd /= b;
			return;
		}
		d = b;
		b = r;
	}
}

void OutputSwCfg1_Mp4(MS_S32 nFrmNum, MFE_CONFIG* pConfig)
{

	MS_S32 nTarWriteCount;
	MS_S32 nRegWriteCount;
	MS_S32 nTarFDCCount;
	MS_S32 nRegFDCCount;

	M4VEINFO* pM4veInfo = &pConfig->ctxM4veInfo;
	BufInfo* pBufInfo = &pConfig->ctxBufInfo;
	BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;
	CVBRRateControl* rcCtx = &pConfig->ctxRateControl;
    MFE_REG* mfe_reg = &pConfig->mfe_reg;

	//////////////////////////////////////////////////////////////////////////
	// Sequence-wide settings

	if (nFrmNum==0) {
		memset(mfe_reg, 0, sizeof(MFE_REG));	// Initial
		mfe_reg->reg_mfe_g_enc_mode = pM4veInfo->bShortHeader ? REG_ENC_MODE_H263 : REG_ENC_MODE_MPG4;
		mfe_reg->reg_mfe_g_pic_width = pConfig->nBufWidth;
		mfe_reg->reg_mfe_g_pic_height = pConfig->nBufHeight;
#ifdef _MFE_M1_
		mfe_reg->reg_mfe_g_jpe_buffer_mode=0;
#endif
		if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {	// MPEG-4
			// Qtable
			mfe_reg->reg_mfe_g_qmode = pM4veInfo->bQuantizerType;

			mfe_reg->reg_mfe_g_mp4_itlc = pM4veInfo->bInterlacedCoding;
			mfe_reg->reg_mfe_g_mp4_pskip_off = (pM4veInfo->bAllowSkippedPMBs==0);
			mfe_reg->reg_mfe_g_mp4_acp = (0<<1) | 1;	// disabled
			mfe_reg->reg_mfe_g_er_hec = pM4veInfo->bHECEnabled;
			mfe_reg->reg_mfe_g_er_hec_t = pM4veInfo->nHECPeriod;
			mfe_reg->reg_mfe_g_mp4_direct_en = pM4veInfo->nAllowDirectBMBs==0 ? 0 : 1;
			mfe_reg->reg_mfe_g_inter_pref = 512;	// inter-intra selection

			// ME partition type
			mfe_reg->reg_mfe_s_me_16x16_disable = 0;
			mfe_reg->reg_mfe_s_me_8x8_disable = 0;
			mfe_reg->reg_mfe_s_me_16x8_disable = 1;
			mfe_reg->reg_mfe_s_me_8x16_disable = 1;
			mfe_reg->reg_mfe_s_me_8x4_disable = 1;
			mfe_reg->reg_mfe_s_me_4x8_disable = 1;
			mfe_reg->reg_mfe_s_me_4x4_disable = 1;

			// MDC
			mfe_reg->reg_mfe_s_mdc_total_mb_bw = pM4veInfo->nNumBitsVPMBnum;
			mfe_reg->reg_mfe_s_mdc_m4vop_tinc_bw = pM4veInfo->nNumBitsTimeIncr;

			// Field DCT
			mfe_reg->reg_mfe_g_mp4_flddct_en = pM4veInfo->bInterlace ? 1 : 0;
			mfe_reg->reg_mfe_g_mp4_flddct_diff_thr = FIELD_DCT_DIFF_THR;
		}
		if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_H263) {	// H263
			// Qtable
			mfe_reg->reg_mfe_g_qmode = 0;

			mfe_reg->reg_mfe_g_er_h263_unit = pM4veInfo->nGobUnit;
			mfe_reg->reg_mfe_g_inter_pref = 512;	// inter-intra selection

			// ME partition type
			mfe_reg->reg_mfe_s_me_16x16_disable = 0;
			mfe_reg->reg_mfe_s_ime_sr16 = 1;
			mfe_reg->reg_mfe_s_ime_umv_disable = 1;
			mfe_reg->reg_mfe_s_me_8x8_disable = 1;
			mfe_reg->reg_mfe_s_me_16x8_disable = 1;
			mfe_reg->reg_mfe_s_me_8x16_disable = 1;
			mfe_reg->reg_mfe_s_me_8x4_disable = 1;
			mfe_reg->reg_mfe_s_me_4x8_disable = 1;
			mfe_reg->reg_mfe_s_me_4x4_disable = 1;
		}
#ifdef _MFE_M1_
            // pre-fetch
            mfe_reg->reg_mfe_s_prfh_cryc_en = 1;
            mfe_reg->reg_mfe_s_prfh_refy_en = 1;
            mfe_reg->reg_mfe_s_prfh_cryc_idle_cnt = 0;
            mfe_reg->reg_mfe_s_prfh_refy_idle_cnt = 0;

		if (pConfig->m_bFrameMode) {
			mfe_reg->reg_mfe_g_jpe_buffer_mode = 1;	// frame-mode
			mfe_reg->reg_mfe_g_jpe_fsvs_mode = 0;
		}
		else {
			mfe_reg->reg_mfe_g_jpe_buffer_mode = 0;	// row-mode
			mfe_reg->reg_mfe_g_jpe_multibuf_mode = 0;
			mfe_reg->reg_mfe_g_jpe_enc_mode = 0;	// 420
			mfe_reg->reg_mfe_g_jpe_qfactor = 3;
#if defined(TEST_INPUT_ROW_MODE_HWAUTO_OK) || defined(TEST_INPUT_ROW_MODE_HWAUTO_FAIL1) || defined(TEST_INPUT_ROW_MODE_HWAUTO_FAIL2) || defined(TEST_INPUT_ROW_MODE_HWAUTO_FAIL3)
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=3;
#elif defined(TEST_INPUT_ROW_MODE_HW_OK) || defined(TEST_INPUT_ROW_MODE_HW_FAIL)
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=2;
#elif defined(TEST_INPUT_ROW_MODE_SWHW_OK) || defined(TEST_INPUT_ROW_MODE_SWHW_FAIL)
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=1;
#else
			mfe_reg->reg_mfe_g_jpe_fsvs_mode=2;
#endif
		}

#ifdef SW_BUF_MODE
		mfe_reg->reg_mfe_g_sw_buffer_mode = 1;
#else
		mfe_reg->reg_mfe_g_sw_buffer_mode = 0;
#endif
#endif //_MFE_M1_
	}
	else {
		mfe_reg->reg_mfe_g_frame_start_sw = 0;
	}


	//////////////////////////////////////////////////////////////////////////
	// Frame-wide settings
#ifdef CLOCK_GATING
    mfe_reg->reg16 = 0xffff;	// clock gating
#endif

	// Input buffer address: Must be 256-byte aligned.
	MS_ASSERT((pBufInfo->m_nCurYAddr.miuAddress&0xFF)==0);
	MS_ASSERT((pBufInfo->m_nCurCAddr.miuAddress&0xFF)==0);
	MS_ASSERT((pBufInfo->m_nRefYAddr[0].miuAddress&0xFF)==0);
	MS_ASSERT((pBufInfo->m_nRefCAddr[0].miuAddress&0xFF)==0);
    if(pConfig->m_bFrameMode==0) {
	MS_ASSERT((pBufInfo->m_nRefYAddr[1].miuAddress&0xFF)==0);
	MS_ASSERT((pBufInfo->m_nRefCAddr[1].miuAddress&0xFF)==0);
    }
	MS_ASSERT((pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress&0x7)==0);
	mfe_reg->reg_mfe_g_cur_y_adr_low  = (MS_U16)((pBufInfo->m_nCurYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_cur_y_adr_high = (MS_U16)(pBufInfo->m_nCurYAddr.miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_cur_c_adr_low  = (MS_U16)((pBufInfo->m_nCurCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_cur_c_adr_high = (MS_U16)(pBufInfo->m_nCurCAddr.miuAddress>>(8+16));

	mfe_reg->reg_mfe_g_ref_y_adr0_low  = (MS_U16)((pBufInfo->m_nRefYAddr[0].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_y_adr0_high = (MS_U16)(pBufInfo->m_nRefYAddr[0].miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_ref_c_adr0_low  = (MS_U16)((pBufInfo->m_nRefCAddr[0].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_c_adr0_high = (MS_U16)(pBufInfo->m_nRefCAddr[0].miuAddress>>(8+16));
     if(pConfig->m_bFrameMode==0) {
	mfe_reg->reg_mfe_g_ref_y_adr1_low  = (MS_U16)((pBufInfo->m_nRefYAddr[1].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_y_adr1_high = (MS_U16)(pBufInfo->m_nRefYAddr[1].miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_ref_c_adr1_low  = (MS_U16)((pBufInfo->m_nRefCAddr[1].miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_ref_c_adr1_high = (MS_U16)(pBufInfo->m_nRefCAddr[1].miuAddress>>(8+16));
    }
#ifdef USE_CUR_AS_REC
	MS_ASSERT(pBufInfo->m_nRecYAddr.miuAddress==pBufInfo->m_nCurYAddr.miuAddress);
	MS_ASSERT(pBufInfo->m_nRecCAddr.miuAddress==pBufInfo->m_nCurCAddr.miuAddress);
#endif
	mfe_reg->reg_mfe_g_rec_y_adr_low  = (MS_U16)((pBufInfo->m_nRecYAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_rec_y_adr_high = (MS_U16)(pBufInfo->m_nRecYAddr.miuAddress>>(8+16));
	mfe_reg->reg_mfe_g_rec_c_adr_low  = (MS_U16)((pBufInfo->m_nRecCAddr.miuAddress>>8)&0xFFFF);
	mfe_reg->reg_mfe_g_rec_c_adr_high = (MS_U16)(pBufInfo->m_nRecCAddr.miuAddress>>(8+16));

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
#endif

	// MV store
	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {
		mfe_reg->reg_mfe_g_mp4_direct_mvstore = pBufInfo->m_bEnableMvStore;
		if (mfe_reg->reg_mfe_g_mp4_direct_mvstore) {
			mfe_reg->reg_mfe_s_mvobuf_sadr_low  = (MS_U16)((pBufInfo->m_nMvStoreAddr.miuAddress>>3)&0xFFFF);
			mfe_reg->reg_mfe_s_mvobuf_sadr_high = (MS_U16)(pBufInfo->m_nMvStoreAddr.miuAddress>>(3+16));
		}
		if (mfe_reg->reg_mfe_g_mp4_direct_en) {
			mfe_reg->reg_mfe_s_gn_mvibuf_sadr_low  = (MS_U16)((pBufInfo->m_nMvStoreAddr.miuAddress>>3)&0xFFFF);
			mfe_reg->reg_mfe_s_gn_mvibuf_sadr_high = (MS_U16)(pBufInfo->m_nMvStoreAddr.miuAddress>>(3+16));
		}
	}
#ifdef _MFE_M1_
	// IMI buffer
    if(pConfig->MfeAdvInfo.input_imi_en) {
        mfe_reg->reg_mfe_s_marb_eimi_block = 0x1;
        WriteRegMFE(0x68, mfe_reg->reg68, (MS_S8*)("[%d] reg68"), nRegWriteCount, (MS_S8*)("IMI enable"));
    }
    else
        mfe_reg->reg_mfe_s_marb_eimi_block = 0x0;

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
	mfe_reg->reg_mfe_g_frame_type = pConfig->vopPredType==I_VOP ? 0 : (pConfig->vopPredType==P_VOP?1:2);
	mfe_reg->reg_mfe_g_ref_no = pConfig->vopPredType==P_VOP ? 0 : (pConfig->vopPredType==B_VOP&&(!pM4veInfo->nAllowDirectBMBs)&&pM4veInfo->g_ref_en!=3 ? 0 : 1);

	// ME setting
	mfe_reg->reg_mfe_s_ime_mesr_max_addr = (pM4veInfo->SEARCH_RANGE_Y==16 ? 95 : 83);//0x5f;
	mfe_reg->reg_mfe_s_ime_mesr_min_addr = (pM4veInfo->SEARCH_RANGE_Y==16 ? 0 : 16);//0;
	mfe_reg->reg_mfe_s_ime_mvx_min = -(pConfig->vopPredType==I_VOP?0:pM4veInfo->iSearchRangeForward) + 32;	// Min X is -pVopMd->iSearchRangeForward
	mfe_reg->reg_mfe_s_ime_mvx_max = ((pConfig->vopPredType==I_VOP?0:pM4veInfo->iSearchRangeForward)-2) + 32;	// Max X is pVopMd->iSearchRangeForward-1
	mfe_reg->reg_mfe_s_ime_mvy_min = -pM4veInfo->SEARCH_RANGE_Y + 16;					// Min Y
	mfe_reg->reg_mfe_s_ime_mvy_max = (pM4veInfo->SEARCH_RANGE_Y==16 ? 15 : 8) + 16;		// Max Y
	mfe_reg->reg_mfe_s_ime_sr16 = (mfe_reg->reg_mfe_s_ime_mvx_min>=16 ? 1 : 0);
	mfe_reg->reg_mfe_s_mesr_adapt = pM4veInfo->IME_ADAPTIVE_WINDOW?1:0;
	mfe_reg->reg_mfe_s_me_ref_en_mode = pM4veInfo->g_ref_en;
	mfe_reg->reg_mfe_s_ime_ime_wait_fme = 1;
#ifdef FME_PIPELINE_OPEN
	mfe_reg->reg_mfe_s_fme_pipeline_on = mfe_reg->reg_mfe_s_ime_ime_wait_fme ? 1 : 0;
#else
	mfe_reg->reg_mfe_s_fme_pipeline_on = 0x0;
#endif

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



	// LastZZ
	mfe_reg->reg_mfe_s_quan_idx_last = pM4veInfo->m_nLastZZ;
	if (mfe_reg->reg_mfe_s_quan_idx_last<63)
		mfe_reg->reg_mfe_s_quan_idx_swlast = 1;
		else
		mfe_reg->reg_mfe_s_quan_idx_swlast = 0;

	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {	// MPEG-4
		mfe_reg->reg_mfe_s_fme_quarter_disable = 1;
		mfe_reg->reg_mfe_s_fme_half_disable = (pM4veInfo->m_nFmePrec==0);
		//mfe_reg->reg_mfe_s_fme_one_mode = 0;
		mfe_reg->reg_mfe_s_fme_pmv_enable = 0;
		mfe_reg->reg_mfe_s_fme_mode_no = pConfig->vopPredType==P_VOP ? 1 : 0;
		mfe_reg->reg_mfe_s_fme_mode0_refno = pConfig->vopPredType==P_VOP ? 0 : ((pConfig->vopPredType==B_VOP&&pM4veInfo->g_ref_en!=3)?0:1);
		mfe_reg->reg_mfe_s_fme_mode1_refno = 0;
		mfe_reg->reg_mfe_s_fme_mode2_refno = 0;

		mfe_reg->reg_mfe_g_mp4_rounding_ctrl = pM4veInfo->iRoundingControl;
		// codeVideoPacketHeader
		MS_ASSERT(pM4veInfo->m_nBitsResyncMarker==17||pM4veInfo->m_nBitsResyncMarker==18);
		mfe_reg->reg_mfe_s_mdc_m4vpktpzero = (pM4veInfo->m_nBitsResyncMarker-1==17) ? 1 : 0;
		mfe_reg->reg_mfe_s_mdc_m4time = pM4veInfo->m_nBitsModuloBase;
		mfe_reg->reg_mfe_s_mdc_m4vop_tinc = pM4veInfo->m_iVopTimeIncr;

		// B-direct mode
		mfe_reg->reg_mfe_g_mp4_direct_pref = 129;
		{
			MS_S32 trd = pM4veInfo->m_tFutureRef - pM4veInfo->m_tPastRef;
			MS_S32 trb = pM4veInfo->m_t - pM4veInfo->m_tPastRef;
			if (trb&trb)
				TrbTrdReduction(&trb, &trd);
			mfe_reg->reg_mfe_g_mp4_direct_trb = trb;
			mfe_reg->reg_mfe_g_mp4_direct_trd = trd;
		}
	}


	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_H263) {	// H263
		mfe_reg->reg_mfe_s_fme_quarter_disable = 1;
		mfe_reg->reg_mfe_s_fme_half_disable = (pM4veInfo->m_nFmePrec==0);
		//mfe_reg->reg_mfe_s_fme_one_mode = 1;
		mfe_reg->reg_mfe_s_fme_pmv_enable = 0;
		mfe_reg->reg_mfe_s_fme_mode_no = 0;
		mfe_reg->reg_mfe_s_fme_mode0_refno = 0;
		mfe_reg->reg_mfe_s_fme_mode1_refno = 0;
		mfe_reg->reg_mfe_s_fme_mode2_refno = 0;

		mfe_reg->reg_mfe_s_mdc_gob_frame_id = pM4veInfo->m_iGobFrameId;
	}
            ms_dprintf(DRV_L3,"m_rcGranularity = %d",rcCtx->m_rcGranularity);
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
	else if (rcCtx->m_nVPMbRow>0)
		MS_ASSERT(0);
	if (mfe_reg->reg_mfe_g_er_mode==1 || mfe_reg->reg_mfe_g_er_mode==2)
		mfe_reg->reg_mfe_g_er_bs_th = rcCtx->m_nVPSize;
	mfe_reg->reg_mfe_g_qscale = pM4veInfo->intQP;
	mfe_reg->reg_mfe_s_mbr_frame_qstep = rcCtx->m_nFrameQStep;
	mfe_reg->reg_mfe_s_mbr_tmb_bits = rcCtx->m_nTargetMbBits;
	// QP/QStep: Min, max
	mfe_reg->reg_mfe_s_mbr_qp_min = rcCtx->m_nMinQP;
	mfe_reg->reg_mfe_s_mbr_qp_max = rcCtx->m_nMaxQP;
	MS_ASSERT(rcCtx->m_nMinQStep<(2<<7));
	mfe_reg->reg_mfe_s_mbr_qstep_min = rcCtx->m_nMinQStep;
	mfe_reg->reg_mfe_s_mbr_qstep_max = rcCtx->m_nMaxQStep;

	mfe_reg->reg_mfe_g_rec_en = pM4veInfo->g_rec_en;

#ifdef HW_ECO_STARTCODE_PREVENTION
    mfe_reg->reg_eco_bsp_rdy_fix = 1;
#if defined(_MFE_EDISON_)
    //agate U02 cannot set this.
    mfe_reg->reg_eco_bsp_multi_slice_fix = 1;
#endif
#endif
	//////////////////////////////////////////////////////////////////////////
	// swcfg1 output
	nTarWriteCount = 0;
	nRegWriteCount = 0;
	nTarFDCCount = 0;
	nRegFDCCount = 0;
	if (nFrmNum==0)
		WriteQTable(mfe_reg, pM4veInfo->rgiIntraQuantizerMatrix, pM4veInfo->rgiInterQuantizerMatrix);

	nRegWriteCount = 0;
#ifdef _MFE_M1_
	//FDC
	mfe_reg->reg_mfe_s_bsp_fdc_skip = 0;
    mfe_reg->reg_mfe_s_bsp_fdc_offset = 0;
	nTarFDCCount = PutFDC(mfe_reg, pBitsInfo, 1);
#endif

	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {
		nTarWriteCount = (pBufInfo->m_bEnableMvStore?62:59) + (nTarFDCCount*3);
	}
	else if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_H263) {
		nTarWriteCount = 53 + (nTarFDCCount*3);
	}
	nTarWriteCount++;	// reg to set fdc round

#ifdef CLOCK_GATING
    nTarWriteCount++;
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
	WriteRegMFE(0x06, mfe_reg->reg06, (MS_S8*)("[%d] reg06"), nRegWriteCount++, (MS_S8*)("current luma base address"));
	WriteRegMFE(0x07, mfe_reg->reg07, (MS_S8*)("[%d] reg07"), nRegWriteCount++, (MS_S8*)("current luma base address high"));
	WriteRegMFE(0x08, mfe_reg->reg08, (MS_S8*)("[%d] reg08"), nRegWriteCount++, (MS_S8*)("current chroma base address"));
	WriteRegMFE(0x09, mfe_reg->reg09, (MS_S8*)("[%d] reg09"), nRegWriteCount++, (MS_S8*)("current chroma base address high"));
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
    nRegWriteCount += SetObufAddr(mfe_reg, (MS_U32)pBufInfo->m_nOutBufAddr, pBufInfo->m_OutBufferSize, 0, 0);
    nRegWriteCount += SetObufAddr(mfe_reg, (MS_U32)pBufInfo->m_nOutBufAddr+pBufInfo->m_OutBufferSize, pBufInfo->m_OutBufferSize, 1, 1);
#else
    nRegWriteCount += SetObufAddr(mfe_reg, (MS_U32)pBufInfo->m_nOutBufAddr[pConfig->nOBufIndex].miuAddress, pBufInfo->m_OutBufferSize, 0, 1);
#endif
#endif

	// MV-store
	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {
		WriteRegMFE(0x19, mfe_reg->reg19, (MS_S8*)("[%d] reg19"), nRegWriteCount++, (MS_S8*)("enable mv-store"));
		if (mfe_reg->reg_mfe_g_mp4_direct_mvstore) {
			WriteRegMFE(0x40, mfe_reg->reg40, (MS_S8*)("[%d] reg40"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_mvobuf_sadr_low"));
			WriteRegMFE(0x41, mfe_reg->reg41, (MS_S8*)("[%d] reg41"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_mvobuf_sadr_high"));
			//
			mfe_reg->reg_mfe_s_bspobuf_set_adr = 0;
			mfe_reg->reg_mfe_s_bspobuf_fifo_th = 0;
			mfe_reg->reg_mfe_s_mvobuf_set_adr = 1;
			mfe_reg->reg_mfe_s_mvobuf_fifo_th = 1;
			WriteRegMFE(0x3b, mfe_reg->reg3b, (MS_S8*)("[%d] reg3b"), nRegWriteCount++, (MS_S8*)("set mv-sotre addr"));
			mfe_reg->reg_mfe_s_mvobuf_set_adr = 0;	// HW is write-one-clear
		}
		// mvibuf
		WriteRegMFE(0x4e, mfe_reg->reg4e, (MS_S8*)("[%d] reg4e"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_gn_mvibuf_sadr_low"));
		WriteRegMFE(0x4f, mfe_reg->reg4f, (MS_S8*)("[%d] reg4f"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_gn_mvibuf_sadr_high"));
	}


	// GN
	WriteRegMFE(0x4c, mfe_reg->reg4c, (MS_S8*)("[%d] reg1c"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_gn_sadr_low"));
	WriteRegMFE(0x4d, mfe_reg->reg4d, (MS_S8*)("[%d] reg4d"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_gn_sadr_high"));

	WriteRegMFE(0x19, mfe_reg->reg19, (MS_S8*)("[%d] reg19"), nRegWriteCount++, (MS_S8*)("value"));
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

	if (mfe_reg->reg_mfe_g_enc_mode==REG_ENC_MODE_MPG4) {	// MPEG-4
		WriteRegMFE(0x37, mfe_reg->reg37, (MS_S8*)("[%d] reg37"), nRegWriteCount++, (MS_S8*)("MPEG4 MDC"));
		WriteRegMFE(0x38, mfe_reg->reg38, (MS_S8*)("[%d] reg38"), nRegWriteCount++, (MS_S8*)("MPEG4: vop_time_increment"));
		// B-direct
		WriteRegMFE(0x1a, mfe_reg->reg1a, (MS_S8*)("[%d] reg1a"), nRegWriteCount++, (MS_S8*)("MPEG4 BDirect"));
	}


    mfe_reg->reg_mfe_g_crc_mode = 0xC;
    mfe_reg->reg_mfe_g_debug_tcycle_chk_en = 0x1;
    mfe_reg->reg_mfe_g_debug_tcycle_chk_sel = 0x0;
	mfe_reg->reg_mfe_g_debug_en = 0; // TEST
    WriteRegMFE(0x73, mfe_reg->reg73, (MS_S8*)("[%d] reg73"), nRegWriteCount++, (MS_S8*)("crc mode"));



   	WriteRegMFE(0x2c, mfe_reg->reg2c, (MS_S8*)("[%d] reg2c"), nRegWriteCount++, (MS_S8*)("Last zigzag"));

	// Cross-format wrong reg setting prevention
	WriteRegMFE(0x18, mfe_reg->reg18, (MS_S8*)("[%d] reg18"), nRegWriteCount++, (MS_S8*)("JPE encode mode"));
	WriteRegMFE(0x1b, mfe_reg->reg1b, (MS_S8*)("[%d] reg1b"), nRegWriteCount++, (MS_S8*)("MPEG4 FieldDCT"));

#ifdef _MFE_M1_
    // Prefetch & Low bandwidth mode
    WriteRegMFE(0x68,mfe_reg->reg68,"[%d] reg68"), nRegWriteCount++, (MS_S8*)("Prefetch enable/disable"));
    // Prefetch
    WriteRegMFE(0x6d,mfe_reg->reg6d,"[%d] reg6d"), nRegWriteCount++, (MS_S8*)("Prefetch MB idle count"));
	//Low bandwidth
	WriteRegMFE(0x6b, mfe_reg->reg6b, (MS_S8*)("[%d] reg6b"), nRegWriteCount++, (MS_S8*)("Low Bandwidth: IMI addr low"));
	WriteRegMFE(0x6c, mfe_reg->reg6c, (MS_S8*)("[%d] reg6c"), nRegWriteCount++, (MS_S8*)("Low Bandwidth: IMI addr high"));

	// Reset any StopAndGo or StopAndDrop setting.
	mfe_reg->reg_mfe_s_txip_sng_mb = 0;
	WriteRegMFE(0x2d, mfe_reg->reg2d, (MS_S8*)("[%d] reg2d"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_txip_sng_mb=0"));

#endif

    //enable eco item
    WriteRegMFE(0x7d, mfe_reg->reg7d, (MS_S8*)("[%d] reg7d"), nRegWriteCount++, (MS_S8*)("reg_mfe_s_txip_eco0=1"));

	ms_dprintk(DRV_L3, "In mfe_reg_m4ve.c Before En HW..\n");
//	DumpAllReg(mfe_reg);

	// Enable HW
	mfe_reg->reg_mfe_g_frame_start_sw = 1;

	WriteRegMFE(0x00, mfe_reg->reg00, (MS_S8*)("[%d] reg00"), nRegWriteCount++, (MS_S8*)("frame start"));
    mfe_reg->reg_mfe_g_frame_start_sw = 0;	// HW is write-one-clear

	ms_dprintk(DRV_L3,"After RESET\n");
	// FDC
	nRegFDCCount = PutFDC(mfe_reg, pBitsInfo, 0);
	nRegWriteCount += nRegFDCCount;

    if(nRegFDCCount != nTarFDCCount) {
        ms_dprintk(DRV_L3, "nRegFDCCount = %d, nTarFDCCount = %d\n", (int)nRegFDCCount, (int)nTarFDCCount);
    }
    if(nRegWriteCount != nTarWriteCount) {
        ms_dprintk(DRV_L3, "nRegWriteCount = %d, nTarWriteCount = %d\n", (int)nRegWriteCount, (int)nTarWriteCount);
    }
	// Only for debug
//	MS_ASSERT(nRegFDCCount==nTarFDCCount);
//	MS_ASSERT(nRegWriteCount==nTarWriteCount);

}

MS_S32 codeM4vConfigHeaders(MFE_CONFIG *pConfig,MS_BOOL IsSkipHeader)
{
	OutStream* pStream = &pConfig->m_OutStream;
//	BitsInfo* pBitsInfo = &pConfig->ctxBitsInfo;

    if(IsSkipHeader) {
	osReset(pStream);
	codeNonCodedVOPShortHead(pConfig, pStream);
	osFlushAll(&pConfig->m_OutStream);
    } else {
	osReset(pStream);
	codeSequenceHead(pConfig, pStream);
	codeVOHead(pConfig, pStream);
	codeVOLHead(pConfig, pStream);
	// Finalize
	osFlushAll(&pConfig->m_OutStream);
    }
	// The generated bytes start from pStream->m_pbFrameBuffer and with pStream->m_nByteCount bytes.
	return pStream->m_nByteCount;
}



