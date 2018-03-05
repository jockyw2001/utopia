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
#include "mdrv_mfe_math.h"
#include "ms_dprintf.h"



#include "m4v_header.h"
#include "msRateCtrl.h"



#define MARKER_BIT						1

#define START_CODE_PREFIX				1
#define NUMBITS_START_CODE_PREFIX		24
//	Added for data partitioning mode By Toshiba(1998-1-16:DP+RVLC)
#define NUMBITS_START_CODE_SUFFIX		8
//	End Toshiba(1998-1-16:DP+RVLC)

// session overhead information
#define USER_DATA_START_CODE			0xB2
#define VSS_START_CODE					0xB0	// 8-bit
#define VSS_END_CODE					0xB1	// 8-bit
#define NUMBITS_VSS_PROFILE				8
#define VSO_START_CODE					0xB5	// 8-bit
#define VSO_VERID						1
#define VSO_TYPE						1
#define NUMBITS_VSO_VERID				4
#define NUMBITS_VSO_PRIORITY			3
#define NUMBITS_VSO_TYPE				4

// VO overhead information
#define NUMBITS_VO_START_CODE			3
#define VO_START_CODE					0
#define NUMBITS_VO_ID					5

#define NUMBITS_NEWPRED_ENABLE						1

// VOL overhead information
#define NUMBITS_SHORT_HEADER_START_CODE 22 // Added by KPN for short headers (1998-02-07, DS)
#define SHORT_VIDEO_START_MARKER        32 // Added by KPN for short headers (1998-02-07, DS)
#define NUMBITS_VOL_START_CODE			4
#define VOL_START_CODE					2
#define NUMBITS_VOL_ID					4
#define NUMBITS_VOL_SHAPE				2
#define NUMBITS_TIME_RESOLUTION			16
#define NUMBITS_VOL_FCODE				3
#define NUMBITS_SEP_MOTION_TEXTURE		1
#define NUMBITS_QMATRIX					8

// VOP overhead information
#define VOP_START_CODE					0xB6
#define NUMBITS_VOP_START_CODE			8
#define NUMBITS_VOP_TIMEINCR			10
#define NUMBITS_VOP_HORIZONTAL_SPA_REF	13
#define NUMBITS_VOP_VERTICAL_SPA_REF	13
#define NUMBITS_VOP_WIDTH				13
#define NUMBITS_VOP_HEIGHT				13
#define NUMBITS_VOP_PRED_TYPE			2
#define NUMBITS_VOP_QUANTIZER			5
#define NUMBITS_VOP_ALPHA_QUANTIZER		6
#define NUMBITS_VOP_FCODE				3



void codeSequenceHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, "VSS_Start_Code");
	osPutBits(pStream, VSS_START_CODE, NUMBITS_START_CODE_SUFFIX, "VSS_Start_Code");
	osPutBits(pStream, 0xF5/*m_volmd.uiProfileAndLevel*/, NUMBITS_VSS_PROFILE, "VSS_Profile_Level");
	
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, "VSO_Start_Code");
	osPutBits(pStream, VSO_START_CODE, NUMBITS_START_CODE_SUFFIX, "VSO_Start_Code");
	osPutBits(pStream, 0, 1, "VSO_IsVisualObjectIdentifier");
	osPutBits(pStream, VSO_TYPE, NUMBITS_VSO_TYPE, "VSO_Type");
	osPutBits(pStream, 0, 1, "VSO_VideoSignalType");
	
// 	UInt uiNumBits = NUMBITS_START_CODE_PREFIX + NUMBITS_START_CODE_SUFFIX + NUMBITS_VSS_PROFILE
// 		+ NUMBITS_START_CODE_PREFIX + 1 + NUMBITS_VSO_TYPE + 1;
//	m_statsVOL.nBitsHead += uiNumBits;
	
//	m_statsVOL.nBitsStuffing += m_pbitstrmOut->flush ();
	osNextStartCode(pStream);
}

void codeVOHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, "VO_Start_Code");
	osPutBits(pStream, VO_START_CODE, NUMBITS_VO_START_CODE, "VO_Start_Code");		//plus 3 bits
	osPutBits(pStream, /*m_uiVOId*/1, NUMBITS_VO_ID, "VO_Id");
// 	U32 uiNumBits = NUMBITS_START_CODE_PREFIX + NUMBITS_VO_START_CODE + NUMBITS_VO_ID;
// 	m_statsVOL.nBitsHead += uiNumBits;
}

void codeVOLHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;
	CVBRRateControl* rcCtx = &pConfig->ctxRateControl;

	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, "VOL_Start_Code_Prefix");
	osPutBits(pStream, VOL_START_CODE, NUMBITS_VOL_START_CODE, "VOL_Start_Code");		//plus 4 bits
	osPutBits(pStream, 0, NUMBITS_VOL_ID, "VOL_Id"); // by katata
//	m_statsVOL.nBitsHead+=NUMBITS_START_CODE_PREFIX+NUMBITS_VOL_START_CODE+NUMBITS_VOL_ID;

	osPutBits(pStream, (int) 0, 1, "VOL_Random_Access");		//isn't this a system level flg?
//	m_statsVOL.nBitsHead++;

	osPutBits(pStream, (int) 1, 8, "VOL_Type_Indicator"); // Set to indicate SIMPLE profile.
//	m_statsVOL.nBitsHead+=8;

	osPutBits(pStream, (int) 1, 1, "VOL_Is_Object_Layer_Identifier"); 
	osPutBits(pStream, (int) 2/*m_volmd.uiVerID*/, 4, "VOL_Verid");
	osPutBits(pStream, (int) 1, 3, "VOL_Priority");
//	m_statsVOL.nBitsHead+=8;

	osPutBits(pStream, (int) 1, 4, "aspect_ratio_info"); // square pix
//	m_statsVOL.nBitsHead+=4;
	
	osPutBits(pStream, /*m_volmd.uiVolControlParameters*/0, 1, "VOL_Control_Parameters"); //useless flag for now
//	m_statsVOL.nBitsHead++;
	
	{
		osPutBits(pStream, /*iAUsage*/0, NUMBITS_VOL_SHAPE, "VOL_Shape_Type");
		osPutBits(pStream, 1, 1, "Marker");
//		m_statsVOL.nBitsStuffing ++;

		osPutBits(pStream, pInfo->nTimeResolution/*m_volmd.iClockRate*/, NUMBITS_TIME_RESOLUTION, "VOL_Time_Increment_Resolution"); 
		osPutBits(pStream, 1, 1, "Marker");
//		m_statsVOL.nBitsStuffing ++;

		osPutBits(pStream, 0, 1, "VOL_Fixed_Vop_Rate");
//		m_statsVOL.nBitsHead += NUMBITS_VOL_SHAPE + NUMBITS_TIME_RESOLUTION + 1;

		osPutBits(pStream, MARKER_BIT, 1, "Marker_Bit");
//		m_statsVOL.nBitsStuffing ++;
		osPutBits(pStream, pConfig->nBufWidth, NUMBITS_VOP_WIDTH, "VOL_Width"); 
		osPutBits(pStream, MARKER_BIT, 1, "Marker_Bit");
//		m_statsVOL.nBitsStuffing ++;
		osPutBits(pStream, pConfig->nBufHeight, NUMBITS_VOP_HEIGHT, "VOL_Height"); 
		osPutBits(pStream, MARKER_BIT, 1, "Marker_Bit");
//		m_statsVOL.nBitsStuffing ++;
//		m_statsVOL.nBitsHead += NUMBITS_VOP_WIDTH + NUMBITS_VOP_HEIGHT;
	}
	osPutBits(pStream, pInfo->bInterlace, 1, "VOL_interlace");
//	m_statsVOL.nBitsHead++;
	osPutBits(pStream, 1/*m_volmd.bAdvPredDisable*/, 1, "VOL_OBMC_Disable");
//	m_statsVOL.nBitsHead++;

	// code sprite info
	osPutBits(pStream, 0/*m_uiSprite*/, 2, "VOL_Sprite_Usage");
//	m_statsVOL.nBitsHead += 2;

	// NBIT
	osPutBits(pStream, (int) 0/*m_volmd.bNot8Bit*/, 1, "VOL_NOT_8_BIT_VIDEO");
//	m_statsVOL.nBitsHead++;

	osPutBits(pStream, (int) pInfo->bQuantizerType/*m_volmd.fQuantizer*/, 1, "VOL_Quant_Type"); 
//	m_statsVOL.nBitsHead++;
	if (pInfo->bQuantizerType/*m_volmd.fQuantizer*/==1) {
		osPutBits(pStream, 0/*m_volmd.bLoadIntraMatrix*/, 1, "VOL_Load_Q_Matrix (intra)");
//		m_statsVOL.nBitsHead++;
		osPutBits(pStream, 0/*m_volmd.bLoadInterMatrix*/, 1, "VOL_Load_Q_Matrix (inter)");
//		m_statsVOL.nBitsHead++;
	}

	osPutBits(pStream, 0/*m_volmd.bQuarterSample*/, 1, "VOL_Quarter_Sample");
//	m_statsVOL.nBitsHead += 1;

	// START: Complexity Estimation syntax support - Marc Mongenet (EPFL) - 16 Jun 1998
	osPutBits(pStream, 1/*m_volmd.bComplexityEstimationDisable*/, 1, "complexity_estimation_disable");
//	m_statsVOL.nBitsHead += 1;

	osPutBits(pStream, (rcCtx->m_nVPMbRow<=0&&rcCtx->m_nVPSize<=0)?1:0/*m_volmd.bResyncMarkerDisable*/ , 1, "VOL_resync_marker_disable");
//	m_statsVOL.nBitsHead ++;

	osPutBits(pStream, 0/*m_volmd.bDataPartitioning*/, 1, "VOL_data_partitioning");
// 	m_statsVOL.nBitsHead ++;
// 	if( m_volmd.bDataPartitioning )
// 	{
// 		osPutBits(pStream, m_volmd.bReversibleVlc, 1, "VOL_reversible_vlc");
// 		m_statsVOL.nBitsHead ++;
// 	}

	osPutBits(pStream, 0/*m_volmd.bNewpredEnable*/, NUMBITS_NEWPRED_ENABLE, "*VOL_newpred_enable");
//	m_statsVOL.nBitsHead += NUMBITS_NEWPRED_ENABLE;
	osPutBits(pStream, 0/*m_volmd.breduced_resolution_vop_enable*/, 1, "reduced_resolution_vop_enable");
//	m_statsVOL.nBitsHead ++;

	osPutBits(pStream, 0/*m_volmd.volType == ENHN_LAYER*/, 1, "VOL_Scalability");
//	m_statsVOL.nBitsHead++;

	osNextStartCode(pStream);
}

static void codeVOPHeadInitial(MFE_CONFIG *pConfig, OutStream* pStream)
{
	int tCurrSec;
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;
	
	// Start code	
	osPutBits(pStream, START_CODE_PREFIX, NUMBITS_START_CODE_PREFIX, "VOP_Start_Code");
	osPutBits(pStream, VOP_START_CODE, NUMBITS_VOP_START_CODE, "VOP_Start_Code");
//	m_statsVOP.nBitsHead += NUMBITS_START_CODE_PREFIX + NUMBITS_VOP_START_CODE;
	
	// prediction type
	osPutBits(pStream, pConfig->vopPredType==I_VOP?0:(pConfig->vopPredType==P_VOP?1:2), NUMBITS_VOP_PRED_TYPE, "VOP_Pred_Type");
//	m_statsVOP.nBitsHead += NUMBITS_VOP_PRED_TYPE;
		
	// Time reference
	tCurrSec = (int)MFE_DIV_S64((MFE_S64)pInfo->m_t * pInfo->nFixedIncrement, pInfo->nTimeResolution) ;
	//tCurrSec = (int)((INT64)pInfo->m_t * pInfo->nFixedIncrement / pInfo->nTimeResolution);// current time in seconds

//      printk("\nm_t = %d,nFixedIncrement = %d nTimeResolution=%d\n",pInfo->m_t ,pInfo->nFixedIncrement,pInfo->nTimeResolution);
//      printk("\n tCurrSec = %d,pInfo->m_tModuloBaseDecd = %d\n",tCurrSec,pInfo->m_tModuloBaseDecd);

	pInfo->m_nBitsModuloBase = tCurrSec - ((pConfig->vopPredType != B_VOP) ?
			pInfo->m_tModuloBaseDecd : pInfo->m_tModuloBaseDisp);
	MFE_ASSERT(pInfo->m_nBitsModuloBase<=31);

	osPutBits(pStream, (int) 0xFFFFFFFE & (0xFFFFFFFF>>(32-(pInfo->m_nBitsModuloBase+1))), pInfo->m_nBitsModuloBase + 1, "VOP_Modulo_Time_Base");
//	m_statsVOP.nBitsHead += m_nBitsModuloBase + 1;

	pInfo->m_iVopTimeIncr = (int)(MFE_DIV_S64((MFE_S64)pInfo->m_t * pInfo->iClockRate * pInfo->nFixedIncrement, pInfo->nTimeResolution)
	       - (MFE_S64)tCurrSec * pInfo->iClockRate) ;
	//pInfo->m_iVopTimeIncr = (int)((INT64)pInfo->m_t * pInfo->iClockRate * pInfo->nFixedIncrement / pInfo->nTimeResolution- (INT64)tCurrSec * pInfo->iClockRate);

	//printf("codeVOPHeadInitial: m_t=%d, iClockRate=%d, nFixedIncrement=%d, nTimeResolution=%d, tCurrSec=%d\n",
	//	pInfo->m_t, pInfo->iClockRate, pInfo->nFixedIncrement, pInfo->nTimeResolution, tCurrSec);

	osPutBits(pStream, (int) 1, 1, "Marker"); // marker bit	Added for error resilient mode by Toshiba(1997-11-14)
//	m_statsVOP.nBitsStuffing ++;

	if(pInfo->nNumBitsTimeIncr!=0)
		osPutBits(pStream, pInfo->m_iVopTimeIncr, pInfo->nNumBitsTimeIncr, "VOP_Time_Incr");
//	m_statsVOP.nBitsHead += m_iNumBitsTimeIncr;// Modified for error resilient mode by Toshiba(1997-11-14)

	osPutBits(pStream, (int) 1, 1, "Marker"); // marker bit
//	m_statsVOP.nBitsStuffing ++;

	if ( pConfig->vopPredType != B_VOP)
	{	//update modulo time base
		pInfo->m_tModuloBaseDisp = pInfo->m_tModuloBaseDecd;			//of the most recently displayed I/Pvop
		pInfo->m_tModuloBaseDecd = tCurrSec;							//of the most recently decoded I/Pvop
	}

// 	if (m_tFrameInterval == 0 && pInfo->bInterlace && pInfo->vopPredType == B_VOP) {
// 		m_tFrameInterval = pInfo->m_t - pInfo->m_tPastRef;
// 	}
}

void codeNonCodedVOPHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	codeVOPHeadInitial(pConfig, pStream);

	osPutBits(pStream, 0, 1, "VOP_Coded");
//	m_statsVOP.nBitsHead++;
}


void codeVOPHead (MFE_CONFIG *pConfig, OutStream* pStream)
{
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;
	
	codeVOPHeadInitial(pConfig, pStream);

	osPutBits(pStream, 1, 1, "VOP_Coded");
//	m_statsVOP.nBitsHead++;

	if (pConfig->vopPredType == P_VOP) {
		osPutBits(pStream, pInfo->iRoundingControl, 1, "VOP_Rounding_Type");
//		m_statsVOP.nBitsHead++;
	}

	osPutBits(pStream, 0/*m_vopmd.iIntraDcSwitchThr*/, 3, "IntraDCSwitchThr");
//	m_statsVOP.nBitsHead+=3;

// INTERLACE_
	if (pInfo->bInterlace == 1) {
		osPutBits(pStream, 1/*m_vopmd.bTopFieldFirst*/, 1, "Top_Field_First");
		osPutBits(pStream, 0/*m_vopmd.bAlternateScan*/, 1, "Alternate_Scan");
//		m_statsVOP.nBitsHead += 2; 
	}

// INTERLACE	

	if (pConfig->vopPredType == I_VOP) {
		osPutBits(pStream, pInfo->intStep, 5/*m_volmd.uiQuantPrecision*/, "VOP_QUANT");
//		m_statsVOP.nBitsHead += m_volmd.uiQuantPrecision;
	}
	else if (pConfig->vopPredType == P_VOP) {
		osPutBits(pStream, pInfo->intStep, 5/*m_volmd.uiQuantPrecision*/, "VOP_QUANT");
//		m_statsVOP.nBitsHead += m_volmd.uiQuantPrecision;
		osPutBits(pStream, pInfo->iFCode/*m_vopmd.mvInfoForward.uiFCode*/, NUMBITS_VOP_FCODE, "VOP_Fcode_Forward");
//		m_statsVOP.nBitsHead += NUMBITS_VOP_FCODE;
	}
	else if (pConfig->vopPredType == B_VOP) {
		osPutBits(pStream, pInfo->intStep, 5/*m_volmd.uiQuantPrecision*/, "VOP_QUANT");
//		m_statsVOP.nBitsHead += m_volmd.uiQuantPrecision;
		osPutBits(pStream, pInfo->iFCode/*m_vopmd.mvInfoForward.uiFCode*/, NUMBITS_VOP_FCODE, "VOP_Fcode_Forward");
//		m_statsVOP.nBitsHead += NUMBITS_VOP_FCODE;
		osPutBits(pStream, pInfo->iFCode/*m_vopmd.mvInfoBackward.uiFCode*/, NUMBITS_VOP_FCODE, "VOP_Fcode_Backward");
//		m_statsVOP.nBitsHead += NUMBITS_VOP_FCODE;
	}
}

//#define FORCE_30_CLOCK
void codeVOPShortHead(MFE_CONFIG *pConfig, OutStream* pStream)
{
	int nSourceFormat;
	int iTemporalRef;
	M4VEINFO *pInfo = &pConfig->ctxM4veInfo;
	
    osPutBits(pStream, SHORT_VIDEO_START_MARKER, NUMBITS_SHORT_HEADER_START_CODE, "VOP_SH_Start_Code");
#ifdef FORCE_30_CLOCK
	{
		int tmp=1;
		if(m_t<=1)
			tmp=0;
		iTemporalRef = (int)MFE_DIV_S64((MFE_S64)pInfo->m_t *(15+tmp) *  pInfo->nFixedIncrement, pInfo->nTimeResolution) -tmp;
		//iTemporalRef = (int)((INT64)pInfo->m_t * (15+tmp) *  pInfo->nFixedIncrement / pInfo->nTimeResolution) -tmp;		
	}
#else
		iTemporalRef = (int)MFE_DIV_S64((MFE_S64)pInfo->m_t * pInfo->iClockRate *  pInfo->nFixedIncrement, pInfo->nTimeResolution);
		//iTemporalRef = (int)((INT64)pInfo->m_t * pInfo->iClockRate *  pInfo->nFixedIncrement / pInfo->nTimeResolution);

#endif
    osPutBits(pStream, iTemporalRef&0xff, 8, "temporal_reference");
    osPutBits(pStream, 1, 1, "marker_bit");
    osPutBits(pStream, 0, 1, "zero_bit");
    osPutBits(pStream, 0, 1, "split_screen_indicator");
    osPutBits(pStream, 0, 1, "document_camera_indicator");
    osPutBits(pStream, 0, 1, "full_picture_freeze_release");
//    m_statsVOP.nBitsHead += NUMBITS_SHORT_HEADER_START_CODE+8+5;

    if (pConfig->nBufWidth == 128 && pConfig->nBufHeight == 96) {
		nSourceFormat = 1;
    } else if (pConfig->nBufWidth == 176 && pConfig->nBufHeight == 144) {
		nSourceFormat = 2;
    } else if (pConfig->nBufWidth == 352 && pConfig->nBufHeight == 288) {
		nSourceFormat = 3;
    } else if (pConfig->nBufWidth == 704 && pConfig->nBufHeight == 576) {
		nSourceFormat = 4;
    } else if (pConfig->nBufWidth == 1408 && pConfig->nBufHeight == 1152) {
		nSourceFormat = 5;
    } else {
		nSourceFormat = 7;	// 111: Extended PTYPE
    }
	osPutBits(pStream, nSourceFormat, 3, "source_format");
//    m_statsVOP.nBitsHead += 3;

	if (nSourceFormat<=5) {
		osPutBits(pStream, pConfig->vopPredType==P_VOP, 1, "picture_coding_type");
		osPutBits(pStream, 0, 4, "four_reserved_zero_bits");
//		m_statsVOP.nBitsHead += 5;
	}
	else {	// PLUSTYPE
		int UFEP, OPPTYPE, MPPTYPE, CPFMT;
		// Update Full Extended PTYPE (UFEP) (3 bits)
		if (pConfig->vopPredType==I_VOP)
			UFEP = 1;	// 001
		else 
			UFEP = 0;	// 000
		osPutBits(pStream, UFEP, 3, "UFEP");
//		m_statsVOP.nBitsHead += 3;
		
		// The Optional Part of PLUSPTYPE (OPPTYPE) (18 bits)
		if (UFEP==1) {
			OPPTYPE = (6<<15)	// Source Format: "110" custom source format
				| (0 <<14)	// Optional Custom PCF
				| (0 <<13)	// Optional Unrestricted Motion Vector (UMV) mode (see Annex D)
				| (0 <<12)	// Optional Syntax-based Arithmetic Coding (SAC) mode (see Annex E)
				| (0 <<11)	// Optional Advanced Prediction (AP) mode (see Annex F)
				| (0 <<10)	// Optional Advanced INTRA Coding (AIC) mode (see Annex I)
				| (0 << 9)	// Optional Deblocking Filter (DF) mode (see Annex J)
				| (0 << 8)	// Optional Slice Structured (SS) mode (see Annex K)
				| (0 << 7)	// Optional Reference Picture Selection (RPS) mode (see Annex N)
				| (0 << 6)	// Optional Independent Segment Decoding (ISD) mode (see Annex R)
				| (0 << 5)	// Optional Alternative INTER VLC (AIV) mode (see Annex S)
				| (0 << 4)	// Optional Modified Quantization (MQ) mode (see Annex T)
				| (1 << 3);	// Equal to "1" to prevent start code emulation
			osPutBits(pStream, OPPTYPE, 18, "OPPTYPE");
//			m_statsVOP.nBitsHead += 18;
		}
		
		// The mandatory part of PLUSPTYPE when PLUSPTYPE present (MPPTYPE) (9 bits)
		MPPTYPE = ((pConfig->vopPredType==I_VOP?0:1)<<6)
			| (0 << 5)		// Optional Reference Picture Resampling (RPR) mode (see Annex P)
			| (0 << 4)		// Optional Reduced-Resolution Update (RRU) mode (see Annex Q)
			| (0 << 3)		// Rounding Type (RTYPE)
			| (1     );		// Equal to "1" to prevent start code emulation
		osPutBits(pStream, MPPTYPE, 9, "MPPTYPE");
//		m_statsVOP.nBitsHead += 9;
		
		// CPM=0, PSBI=N/A
		osPutBits(pStream, 0, 1, "CPM=0, PSBI=N/A");
//		m_statsVOP.nBitsHead += 1;
		
		if (UFEP==1) {
			// Custom Picture Format (CPFMT) (23 bits)
			CPFMT = (2 << 19)	// Pixel Aspect Ratio Code. 2: 12:11 (CIF for 4:3 picture)
				| ((pConfig->nBufWidth/4-1) << 10)	// Picture Width Indication: Range [0, ... , 511]; Number of pixels per line = (PWI + 1) * 4
				| (1 << 9)					// Equal to "1" to prevent start code emulation
				| (pConfig->nBufHeight/4);			// Picture Height Indication: Range [1, ... , 288]; Number of lines = PHI * 4
			osPutBits(pStream, CPFMT, 23, "CPFMT");
//			m_statsVOP.nBitsHead += 23;
		}
	}
	
    osPutBits(pStream, pInfo->intStep, 5, "vop_quant");
//    m_statsVOP.nBitsHead += 5;
	if (nSourceFormat<=5) {
		osPutBits(pStream, 0, 1, "zero_bit");
		osPutBits(pStream, 0, 1, "pei");
//		m_statsVOP.nBitsHead += 2;
	}
	else {
		osPutBits(pStream, 0, 1, "pei");
//		m_statsVOP.nBitsHead += 1;
	}

	// Set GOB info
	pInfo->m_iGobFrameId = (pConfig->vopPredType==P_VOP ? 1 : 0);//dec_count&0x3;
}

void codeNonCodedVOPShortHead(MFE_CONFIG *pConfig, OutStream* pStream)
{
	int i, nTotalMBs;
	
	codeVOPShortHead(pConfig, pStream);
	
	// Every MB is skipped MB
	nTotalMBs = (pConfig->nBufWidth*pConfig->nBufHeight)>>8;
	for (i=0; i<nTotalMBs; i++)
		osPutBits(pStream, 1, 1, "not_coded");
}
