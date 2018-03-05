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
//////////////////////////////////////////////////////////////////////////
// MStar simple rate control
//////////////////////////////////////////////////////////////////////////

#include "MFE_chip.h"
#include "mfe_common.h"
#include "mfe_type.h"
#include "ms_dprintf.h"

#include "mdrv_mfe_math.h"

#include "mfe_reg.h"
#include "msRateCtrl.h"
#if defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&!defined(_KERNEL_MODE_)
#include <stdlib.h>
#include <stdio.h>
#endif



// H264
    // Spec limitation is [1, 51]
	#define MSRC_MAX_QP_H264  48
	#define MSRC_MIN_QP_H264  5
	#define FRAMESKIP_THR_QP_H264   40
// MPEG-4, H263
    // Spec limitation is [1, 31]
	#define MSRC_MAX_QP_MPEG4  28
	#define MSRC_MIN_QP_MPEG4  1
	#define FRAMESKIP_THR_QP_MPEG4   20

//! How many seconds of frames are responsible for compensation of bitrate usage.
#define CBR_DEPUTY_SECOND     1
#define CVBR_DEPUTY_SECOND    3
#define VBR_DEPUTY_SECOND     10

#define MIN_DEPUTY_FACTOR 10

//! Default I-frame weighting over inter-frame
// #define IFRAME_WEIGHT   2
// #define PFRAME_WEIGHT   1
#define IFRAME_WEIGHT   8
#define PFRAME_WEIGHT   4
#define BFRAME_WEIGHT	3

#define QS_SHIFT_FACTOR	5

#ifdef _SUPPORT_MBLEVEL_RC_
	#define MBACT_NORMALIZATION_FACTOR 2
	//#define FLAT_AREA_VAR_THR          5
	#define SPEC_QP_DIFF_LIMIT         2 // Spec limitation
	#define USE_INT_PREC
	// Below are used when USE_INT_PREC
	#define SCALE_BITS                 6
	#define SCALE_ACT                  6
#endif

//////////////////////////////////////////////////////////////////////////
// global variables
//////////////////////////////////////////////////////////////////////////
#ifndef _USE_EXT_RCDATA_
	CVBRRateControl g_cvbrContext;
	CVBRRCInfo g_cvbrInfo;
	int g_cvbrFrameSkip;
#endif

//private:
int cvbr_ComputeFrameQP(CVBRRateControl* ct, int nFrameType);	// Return the target 'QStep'
int cvbr_ComputeMbQP(CVBRRateControl* ct, int mb_idx);	// Return the target 'QP'

int rcQP2Qstep(CVBRRateControl* ct, int QP);
int rcQstep2QP(CVBRRateControl* ct, int QstepX32);

/*
void output_MBR_reg(void* hd, CVBRRateControl* ct)
{
    if (ct->m_nVPMbRow>0 && ct->m_nVPSize<=0)
        sd_output(hd, 0, 2, "reg_mfe_g_er_mode");
    else if (ct->m_nVPMbRow<=0 && ct->m_nVPSize>0)
        sd_output(hd, 1, 2, "reg_mfe_g_er_mode");
    else if (ct->m_nVPMbRow>0 && ct->m_nVPSize>0)
        sd_output(hd, 2, 2, "reg_mfe_g_er_mode");
    else
        sd_output(hd, 3, 2, "reg_mfe_g_er_mode");
    if (ct->m_nVPMbRow==1)
        sd_output(hd, 0, 2, "reg_mfe_g_er_mby");
    else if (ct->m_nVPMbRow==2)
        sd_output(hd, 1, 2, "reg_mfe_g_er_mby");
    else if (ct->m_nVPMbRow==4)
        sd_output(hd, 2, 2, "reg_mfe_g_er_mby");
    else if (ct->m_nVPMbRow==8)
        sd_output(hd, 3, 2, "reg_mfe_g_er_mby");
    else
        sd_output(hd, 0, 2, "reg_mfe_g_er_mby");
}
*/

void cvbr_InitRateControl(CVBRRateControl* ct, CVBRRCInfo* pRCInfo)
{
	int i, j;

	ct->m_nFrameCount = 0;

	MFE_ASSERT(pRCInfo->nWidth<=MAX_MB_WIDTH*16);
	if (pRCInfo->rcMethod==CONST_QUALITY)
		MFE_ASSERT(pRCInfo->rcGranularity==FRAMELEVELRC);
	// Copy parameters, no error checking yet.
	ct->m_nCodecType = pRCInfo->nCodecType;
	ct->m_nWidth = pRCInfo->nWidth;
	ct->m_nHeight = pRCInfo->nHeight;
/*	
	if (pRCInfo->fTargetFrameRate - (int)pRCInfo->fTargetFrameRate != 0) {
		switch ((int)pRCInfo->fTargetFrameRate) {
		case 14:	// 14.985
			ct->m_nTargetFrameRateNum = 15000;
			ct->m_nTargetFrameRateDeNum = 1001;
			break;
		case 23:	// 23.976
			ct->m_nTargetFrameRateNum = 24000;
			ct->m_nTargetFrameRateDeNum = 1001;
			break;
		case 29:	// 29.97
			ct->m_nTargetFrameRateNum = 30000;
			ct->m_nTargetFrameRateDeNum = 1001;
			break;
		default:
			MFE_ASSERT(0);
		}
	}
*/
	//2010/01/21 because LG GP3 no support float operation, so only support 29.97 now.
	//if fps is 29.97, fTargetFrameRate_is_float =1  
	if (pRCInfo->fTargetFrameRate_is_float==1) {
			ct->m_nTargetFrameRateNum = 30000;
			ct->m_nTargetFrameRateDeNum = 1001;
	}	
	else {
		ct->m_nTargetFrameRateNum = pRCInfo->int_fTargetFrameRate;
		ct->m_nTargetFrameRateDeNum = 1;
	}

	ct->m_nTargetFrameRateInt = ct->m_nTargetFrameRateNum / ct->m_nTargetFrameRateDeNum;
	ct->m_nBitrate = pRCInfo->nBitrate;
	ct->m_nMaxBitrate = pRCInfo->nMaxBitrate;
	ct->m_nConstQP = pRCInfo->nConstQP;
	ct->m_nVPSize = pRCInfo->nVPSize;
	ct->m_nVPMbRow = pRCInfo->nVPMbRow;
	ct->m_bFixedFrameRate = pRCInfo->bFixedFrameRate;
	ct->m_nPCount = pRCInfo->nPCount;
	ct->m_nBCount = pRCInfo->nBCount;
	ct->m_rcGranularity = pRCInfo->rcGranularity;
	ct->m_rcMethod = pRCInfo->rcMethod;
	// More bitrate checking
	if (ct->m_rcMethod==CONSTRAINED_VARIABLE_BITRATE) {
		if (ct->m_nBitrate==0)
			ct->m_nBitrate = (int)(ct->m_nMaxBitrate *6/10);
		if (ct->m_nMaxBitrate==0)
			ct->m_nMaxBitrate = (int)(ct->m_nBitrate * 14/10);
	ct->m_nMaxOffset = (int)MFE_DIV_S64((MFE_S64)(ct->m_nMaxBitrate - ct->m_nBitrate)*ct->m_nTargetFrameRateDeNum, ct->m_nTargetFrameRateNum) ;	
//	ct->m_nMaxOffset = (int)((MFE_S64)(ct->m_nMaxBitrate - ct->m_nBitrate)*ct->m_nTargetFrameRateDeNum / ct->m_nTargetFrameRateNum);

		if (ct->m_rcGranularity==FRAMELEVELRC)
			ct->m_nMaxOffset = (ct->m_nMaxOffset) >> 2;
	}
	else {
		ct->m_nMaxBitrate = 0;  // Don't care
	}

	// Derived
	ct->m_nAvgBitsPerFrame = (int)MFE_DIV_S64((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum, ct->m_nTargetFrameRateNum) ;
	//ct->m_nAvgBitsPerFrame = (int)((MFE_S64)ct->m_nBitrate *ct->m_nTargetFrameRateDeNum / ct->m_nTargetFrameRateNum);

	i = 1+ct->m_nPCount*(1+ct->m_nBCount);
	j = IFRAME_WEIGHT + PFRAME_WEIGHT*ct->m_nPCount + BFRAME_WEIGHT*(ct->m_nPCount*ct->m_nBCount);
	ct->m_nBitsPerFrame[0] = (int)MFE_DIV_S64((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum*IFRAME_WEIGHT*i, j*ct->m_nTargetFrameRateNum);
	ct->m_nBitsPerFrame[1] = (int)MFE_DIV_S64((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum*PFRAME_WEIGHT*i,j*ct->m_nTargetFrameRateNum);
	ct->m_nBitsPerFrame[2] = (int)MFE_DIV_S64((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum*BFRAME_WEIGHT*i,j*ct->m_nTargetFrameRateNum);
#ifdef _SUPPORT_MBLEVEL_RC_
	ct->m_nMbWidth = (ct->m_nWidth+15)>>4;
#endif

	// QP, QStep: min, max
	if (ct->m_nCodecType==REG_ENC_MODE_MPG4 || ct->m_nCodecType==REG_ENC_MODE_H263) {
		ct->m_nMinQP = MSRC_MIN_QP_MPEG4;
		ct->m_nMaxQP = MSRC_MAX_QP_MPEG4;
		ct->m_nMinQStep = rcQP2Qstep(ct, MSRC_MIN_QP_MPEG4)-1;
		ct->m_nMaxQStep = rcQP2Qstep(ct, MSRC_MAX_QP_MPEG4);
		ct->m_nFrameSkipThrQP = FRAMESKIP_THR_QP_MPEG4;
	}
	else {
		ct->m_nMinQP = MSRC_MIN_QP_H264;
		ct->m_nMaxQP = MSRC_MAX_QP_H264;
		ct->m_nMinQStep = rcQP2Qstep(ct, MSRC_MIN_QP_H264)-1;
		ct->m_nMaxQStep = rcQP2Qstep(ct, MSRC_MAX_QP_H264);
		ct->m_nFrameSkipThrQP = FRAMESKIP_THR_QP_H264;
		MFE_ASSERT(ct->m_nCodecType==REG_ENC_MODE_H264);
	}

	// Bitrate usage monitoring
	ct->m_nMinDeputyCount = (ct->m_nTargetFrameRateNum*CBR_DEPUTY_SECOND)/ct->m_nTargetFrameRateDeNum;
	ct->m_nTargetFullness = ct->m_nBitrate >> 1;
	ct->m_nBufFullness = ct->m_nTargetFullness;
	switch (ct->m_rcMethod) {
		case VARIABLE_BITRATE:
			ct->m_nDeputyCount = (ct->m_nTargetFrameRateNum*VBR_DEPUTY_SECOND)/ct->m_nTargetFrameRateDeNum;
			break;
		case CONSTRAINED_VARIABLE_BITRATE:
			ct->m_nDeputyCount = (ct->m_nTargetFrameRateNum*CVBR_DEPUTY_SECOND)/ct->m_nTargetFrameRateDeNum;
			ct->m_BitrateGauge[0] = ct->m_nBitsPerFrame[0];
			for (i=1; i<MAX_GAUGE_SIZE; i++) {
				ct->m_BitrateGauge[i] = ct->m_nBitsPerFrame[1];
				for (j=0; j<ct->m_nBCount; j++, i++) {
					if (i==MAX_GAUGE_SIZE) break;
					ct->m_BitrateGauge[i] = ct->m_nBitsPerFrame[2];
				}
			}
			ct->m_nGaugeCount = ct->m_nTargetFrameRateInt;
			ct->m_nGaugeIndex = 0;
			ct->m_nGaugeBitrate = (int)MFE_DIV_S64((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum*ct->m_nGaugeCount, ct->m_nTargetFrameRateNum) ;				
			//ct->m_nGaugeBitrate = (int)((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum*ct->m_nGaugeCount) / ct->m_nTargetFrameRateNum;
			break;
		case CONST_BITRATE:
		default:
	ct->m_nDeputyCount = (int)MFE_DIV_S64((MFE_S64)ct->m_nTargetFrameRateNum*CBR_DEPUTY_SECOND,ct->m_nTargetFrameRateDeNum) ;			
	//ct->m_nDeputyCount = (int)((MFE_S64)ct->m_nTargetFrameRateNum*CBR_DEPUTY_SECOND)/ct->m_nTargetFrameRateDeNum;
			break;
	}

	ct->m_nFrameCount = 0;
	ct->m_nTotalBits = 0;
	ct->m_nLastFrameAvgQP[0] = ct->m_nLastFrameAvgQP[1] = ct->m_nLastFrameBits = 0;
	ct->m_nLongTermQP64 = 0;

	ct->m_nTotalMB = (ct->m_nWidth>>4)*(ct->m_nHeight>>4);
#ifndef _USE_EXT_RCDATA_
	g_cvbrFrameSkip = 0;
#endif


//	ms_dprintk(DRV_L3,"[CVBRRC] InitRateControl: w=%d, h=%d, FPS=%2.3f, FixedFPS=%d, Bitrate=%d\n",
//		ct->m_nWidth, ct->m_nHeight, (float)ct->m_nTargetFrameRateNum/ct->m_nTargetFrameRateDeNum,
//		ct->m_bFixedFrameRate, ct->m_nBitrate);




}


void cvbr_CloseRateControl(CVBRRateControl* ct)
{
}

// FieldType: 0--Progressive, 1--First field, 2--second field.
int cvbr_InitFrame(CVBRRateControl* ct, int nFrameType, char FieldType)
{
	int nDeputyCount = 0;
	int delta = 0, nRtnQP;

	if (FieldType==0||FieldType==1)
		ct->m_bIsBotField = 0;
	else
		ct->m_bIsBotField = 1;	// FieldType is 2

	ct->m_nFrameType = nFrameType;
	ct->m_nLastVPBits = 0;
	ct->m_nNewPacket = 0;

	// Target frame bitcount 
	if (FieldType!=2) 
	{
		if (ct->m_nFrameCount>0) {
			// 1. Determine the number of future frame to compensate for current bitrate mismatch.
			if (ct->m_nFrameCount>ct->m_nDeputyCount*MIN_DEPUTY_FACTOR)
				nDeputyCount = ct->m_nDeputyCount;
			else if (ct->m_nFrameCount<ct->m_nMinDeputyCount)
				nDeputyCount = ct->m_nMinDeputyCount;
			else
				nDeputyCount = ct->m_nMinDeputyCount + 
				(ct->m_nFrameCount-ct->m_nMinDeputyCount)*(ct->m_nDeputyCount-ct->m_nMinDeputyCount)/(ct->m_nDeputyCount*MIN_DEPUTY_FACTOR-ct->m_nMinDeputyCount);
			// 2. Calculate the bitcount that this frame should be compensate for.
			if (ct->m_rcMethod==CONST_BITRATE) {
			delta = (int)MFE_DIV_S64((MFE_S64)(ct->m_nBufFullness-ct->m_nTargetFullness)*ct->m_nTargetFrameRateDeNum, ct->m_nTargetFrameRateNum) ;
		       //delta = (int)((MFE_S64)(ct->m_nBufFullness-ct->m_nTargetFullness)*ct->m_nTargetFrameRateDeNum)/ct->m_nTargetFrameRateNum;


// 			delta = (ct->m_nBufFullness>ct->m_nTargetFullness) ? 
// 				(int)((ct->m_nBufFullness-ct->m_nTargetFullness)/ct->m_fTargetFrameRate) : ct->m_nBufFullness-ct->m_nTargetFullness;
			}
			else if (ct->m_rcMethod==CONSTRAINED_VARIABLE_BITRATE) {
				delta = (ct->m_nBufFullness-ct->m_nTargetFullness) / nDeputyCount;
				if (delta<-ct->m_nMaxOffset)
					delta = -ct->m_nMaxOffset;
			}
			else if (ct->m_rcMethod==VARIABLE_BITRATE) {
				delta = (ct->m_nBufFullness-ct->m_nTargetFullness) / nDeputyCount;
				if (delta>0 && ((ct->m_nLastFrameAvgQP[0]<<6)>ct->m_nLongTermQP64))
					delta = delta>>1;  // Make it more variable bitrate to allow better quality
			}
			// 3. Finally, calculate the target bitcount.
			if (ct->m_nPCount==0) {
				ct->m_nTargetBits = ct->m_nBitsPerFrame[0] - delta;
			}
			else {
				switch (nFrameType) {
			case I_VOP:
				ct->m_nTargetBits = ct->m_nBitsPerFrame[0] - delta;
				break;
			case P_VOP:
				ct->m_nTargetBits = ct->m_nBitsPerFrame[1] - delta;
				break;
			case B_VOP:
				ct->m_nTargetBits = ct->m_nBitsPerFrame[2] - delta;
				break;
				}
			}
			if (ct->m_nTargetBits<=(ct->m_nAvgBitsPerFrame>>3))
				ct->m_nTargetBits = (ct->m_nAvgBitsPerFrame>>3);  // Target bitcount must>0 for ComputeFrameQP()
		}
		else {
			ct->m_nTargetBits = ct->m_nBitsPerFrame[0];    // Must be I-frame
		}

		/* Return initial frame QP */

		ct->m_nFrameQP = cvbr_ComputeFrameQP(ct, nFrameType);

		ct->m_nLastFrameAvgQP[0] = ct->m_nLastFrameAvgQP[1] = 0;
		ct->m_nLastFrameBits = 0;

#ifdef _SUPPORT_MBLEVEL_RC_
		ct->m_nTargetMbBits = ct->m_nTargetBits / ct->m_nTotalMB;
#endif
	}
	else {	// bottom field
		int nTargetBits;
		ct->m_nFrameQP = ct->m_nLastFrameAvgQP[0] / ((ct->m_nTotalMB>>1)-1);
#ifdef _SUPPORT_MBLEVEL_RC_
		nTargetBits = ct->m_nTargetBits - ct->m_nLastFrameBits;
		if (nTargetBits<ct->m_nTargetBits>>3)
			nTargetBits = ct->m_nTargetBits>>3;
		ct->m_nTargetMbBits = nTargetBits / (ct->m_nTotalMB>>1);
#endif
	}

#ifdef _SUPPORT_MBLEVEL_RC_
	ct->m_nMBN = 0;
	if (ct->m_nTargetMbBits<1)
		ct->m_nTargetMbBits = 1;
#endif

	nRtnQP = rcQstep2QP(ct, ct->m_nFrameQP);
	//ct->m_nFrameQP = rcQP2Qstep(ct, nRtnQP);

//	ms_dprintk(DRV_L3,"[CVBRRC] %5d%s[%s] InitFrame: TargetBits %7d InitQP %2d Buffer %7d Deputy=%d\n",
//		ct->m_nFrameCount, FieldType==0?"F":(FieldType==1?"T":"B"), nFrameType==I_VOP?"I":(nFrameType==P_VOP?"P":"B"), 
//		(int)(ct->m_nTargetBits), nRtnQP, ct->m_nBufFullness, nDeputyCount);

	return nRtnQP;
}

int cvbr_UpdateFrame(CVBRRateControl* ct, int totalUsedBits, char bDummyFrame, char FieldType)
{
	int frameskip = 0;

	// Update counter
	ct->m_nTotalBits += totalUsedBits;
	ct->m_nLastFrameBits += totalUsedBits;
	ct->m_nBufFullness += totalUsedBits;
	if (FieldType!=1) {
		ct->m_nFrameCount++;
		ct->m_nLastTargetBits = ct->m_nTargetBits;
		
		if (!bDummyFrame) {
			ct->m_nLastFrameAvgQP[0] = (ct->m_nLastFrameAvgQP[0]+ct->m_nLastFrameAvgQP[1]) / (FieldType==0 ? ct->m_nTotalMB-1 : ct->m_nTotalMB-2);
			// Variable bitrate
			if (ct->m_rcMethod==VARIABLE_BITRATE)
				ct->m_nLongTermQP64 += ((ct->m_nLastFrameAvgQP[0]<<6)-ct->m_nLongTermQP64) / ct->m_nFrameCount;
			else if (ct->m_rcMethod==CONSTRAINED_VARIABLE_BITRATE) {
				if (ct->m_nFrameCount==1)
					ct->m_nLongTermQP64 = ct->m_nLastFrameAvgQP[0]<<6;
				else
					ct->m_nLongTermQP64 = (ct->m_nLongTermQP64*(ct->m_nDeputyCount-1) + (ct->m_nLastFrameAvgQP[0]<<6)) / ct->m_nDeputyCount;
			}
		}
		else {
			ct->m_nLastFrameAvgQP[0] = ct->m_nFrameQP;
		}
		
		if (ct->m_rcMethod==CONSTRAINED_VARIABLE_BITRATE) {
			ct->m_nGaugeBitrate -= ct->m_BitrateGauge[ct->m_nGaugeIndex];
			ct->m_nGaugeBitrate += totalUsedBits;
			ct->m_BitrateGauge[ct->m_nGaugeIndex] = totalUsedBits;
			ct->m_nGaugeIndex++;
			if (ct->m_nGaugeIndex==ct->m_nGaugeCount)
				ct->m_nGaugeIndex = 0;
		}
		
		// Update buffer status
		ct->m_nBufFullness -= ct->m_nAvgBitsPerFrame;
	}


	// Check if next skipped frame(s) needed
	if (FieldType!=1 && !ct->m_bFixedFrameRate && !bDummyFrame)
	{
		if (ct->m_rcMethod==CONSTRAINED_VARIABLE_BITRATE || ct->m_rcMethod==VARIABLE_BITRATE) {
			if (   ct->m_nLongTermQP64>(rcQP2Qstep(ct, ct->m_nFrameSkipThrQP)<<6) 
				&& ct->m_nLastFrameBits >= (ct->m_nLastTargetBits<<1) ) {
					//frameskip = (ct->m_nLastFrameBits / ct->m_nLastTargetBits)-1;
					frameskip = (int)((ct->m_nLastFrameBits - ct->m_nLastTargetBits) / ct->m_nAvgBitsPerFrame - 1);
					if (frameskip<0)
						frameskip = 0;
					else if (frameskip>ct->m_nMaxFrozenFrame)
						frameskip = ct->m_nMaxFrozenFrame;
			}
		}
		else if (ct->m_rcMethod==CONST_BITRATE) {
			if (ct->m_nLastFrameAvgQP[0]>rcQP2Qstep(ct, ct->m_nFrameSkipThrQP)) {
				// Actual fullness is updated after encoding dummy-P frame
				int nBufFullness = ct->m_nBufFullness;
				while (nBufFullness > ct->m_nTargetFullness) {
					nBufFullness = (int)(nBufFullness - ct->m_nAvgBitsPerFrame);
					frameskip += 1;
				}
			}
		}
	}

#ifdef DEBUG
	if (frameskip > 255)
	{
		fprintf (stderr, "Warning: frameskip > 255\n");
	}
	if (FieldType!=1){
	    MFE_S64 tmp_bitrate ;
	    tmp_bitrate= (int)MFE_DIV_S64((MFE_S64)ct->m_nTotalBits*ct->m_nTargetFrameRateNum, ct->m_nFrameCount*ct->m_nTargetFrameRateDeNum) ;

//	    ms_dprintk(DRV_L3,"[CVBRRC] UpdateFrame(%7d bits, %3d%%) AvgQ=%2d LTQ=%2d Bitrate=%8d frameskip=%2d\n",
//		ct->m_nLastFrameBits, (ct->m_nLastFrameBits-ct->m_nTargetBits)*100/ct->m_nTargetBits, rcQstep2QP(ct, ct->m_nLastFrameAvgQP[0]), 
//		(int)(ct->m_nLongTermQP64+32)>>6,(int)tmp_bitrate, frameskip);
	}

//	DEBUG_RC(("[CVBRRC]       UpdateFrame(%7d bits, %3d%%) AvgQ=%2d LTQ=%2d Bitrate=%8d frameskip=%2d\n",
//	totalUsedBits, (totalUsedBits-ct->m_nTargetBits)*100/ct->m_nTargetBits, ct->m_nLastFrameAvgQP[0], 
//	(int)ct->m_fLongTermQP, ct->m_nGaugeBitrate, frameskip));
#endif

	return frameskip;
}

#define SMOOTH_PERIOD	1
#define INIT_QP_FACTOR	720
#define MIN_INIT_QP		1
#define MAX_INIT_QP		15

/* Return target QPStep */
int cvbr_ComputeFrameQP(CVBRRateControl* ct, int nFrameType)
{
	int newQPStep=0;
	MFE_S64 buf_rest;
	int buf_rest_pic;
	int frames_left;
	int nAdjust;

	int bitrate = ct->m_nBitrate;
	int targetFPS_DeNum =  ct->m_nTargetFrameRateDeNum;
	int targetFPS_Num =  ct->m_nTargetFrameRateNum;
	int TotalMB = ct->m_nTotalMB;
	int frame_count = ct->m_nFrameCount;
	
	// For the very first frame, guess one qp!
	if (ct->m_nFrameCount==0) {
		int nbpMb, newQP;
		if (ct->m_rcMethod==CONST_QUALITY) {
			newQP = ct->m_nConstQP;
			newQPStep = rcQP2Qstep(ct, newQP);	// So that frame qp will be exactly ct->m_nConstQP
		}
		else {
	              nbpMb= (int)MFE_DIV_S64((MFE_S64)bitrate*targetFPS_DeNum, TotalMB*targetFPS_Num) +1 ;
	          //nbpMb = (int)((MFE_S64)ct->m_nBitrate*ct->m_nTargetFrameRateDeNum)/(ct->m_nTotalMB*ct->m_nTargetFrameRateNum) + 1;
			newQP = INIT_QP_FACTOR / nbpMb;
			if (newQP<MIN_INIT_QP)
				newQP = MIN_INIT_QP;
			else if (newQP>MAX_INIT_QP)
				newQP = MAX_INIT_QP;
			newQPStep = newQP<<QS_SHIFT_FACTOR;
		}
		
		ct->m_nMaxFrozenFrame = newQP>>1;
		if (ct->m_nMaxFrozenFrame>15)
			ct->m_nMaxFrozenFrame=15;
		return newQPStep;
	}

	if (ct->m_rcMethod==CONST_QUALITY)
		return rcQP2Qstep(ct, ct->m_nConstQP);

	if (ct->m_rcMethod==CONST_BITRATE) {
	       buf_rest= MFE_DIV_S64((MFE_S64)frame_count*targetFPS_DeNum*bitrate, targetFPS_Num) ;
          //buf_rest = (((MFE_S64)ct->m_nFrameCount*ct->m_nTargetFrameRateDeNum*ct->m_nBitrate)/ct->m_nTargetFrameRateNum);
		
		buf_rest += (SMOOTH_PERIOD*ct->m_nBitrate) - ct->m_nTotalBits;

		newQPStep = ct->m_nLastFrameAvgQP[0];
		frames_left = (SMOOTH_PERIOD * ct->m_nTargetFrameRateNum) / ct->m_nTargetFrameRateDeNum;
		//if (frames_left > 0)
		{
			int dQP;
			buf_rest_pic = (int)MFE_DIV_S64(buf_rest , frames_left);
			dQP = ct->m_nLastFrameAvgQP[0]>>3;
			if (ct->m_nLastFrameBits > (buf_rest_pic*9)>>3) {
				newQPStep = ct->m_nLastFrameAvgQP[0]+dQP;
			} 
			else if (ct->m_nLastFrameBits < (buf_rest_pic*7)>>3) {
				newQPStep = ct->m_nLastFrameAvgQP[0]-dQP;
			}
		}
	}

	else if (ct->m_rcMethod==CONSTRAINED_VARIABLE_BITRATE) {    
 		int nLowBound, nHighBound;
		nAdjust = ct->m_nLongTermQP64>>2;
		nLowBound = (ct->m_nLongTermQP64 - nAdjust) >> 6;
		nHighBound = (ct->m_nLongTermQP64 + nAdjust) >> 6;
		if (ct->m_nPCount>0 && nFrameType==I_VOP) {
			newQPStep = ct->m_nLastFrameAvgQP[0];
			if (ct->m_nGaugeBitrate<ct->m_nBitrate)
				newQPStep = newQPStep-(1<<QS_SHIFT_FACTOR);
			newQPStep = MSRC_MAX((1<<QS_SHIFT_FACTOR), newQPStep);
			newQPStep = MSRC_MIN((12<<QS_SHIFT_FACTOR), newQPStep);
		}
		else {
			MFE_S64 tmp;
	              tmp= MFE_DIV_S64((MFE_S64)ct->m_nLongTermQP64 * MFE_DIV_S64(ct->m_nTotalBits,ct->m_nFrameCount), ct->m_nTargetBits) ;
                 //tmp = ct->m_nLongTermQP64 * (ct->m_nTotalBits/ct->m_nFrameCount) /ct->m_nTargetBits);		
			MFE_ASSERT((tmp >> 6) < (1<<31));
	              newQPStep=  (int)(
				  	MFE_DIV_S64((MFE_S64)ct->m_nLongTermQP64 * MFE_DIV_S64(ct->m_nTotalBits,ct->m_nFrameCount),
				  	             ct->m_nTargetBits)  >>6) ;
	//newQPStep = (int)((ct->m_nLongTermQP64 * (ct->m_nTotalBits/ct->m_nFrameCount) / ct->m_nTargetBits) >> 6);
			if (ct->m_nLastFrameBits>ct->m_nLastTargetBits) {
				nAdjust = ((ct->m_nLastFrameBits-ct->m_nLastTargetBits)/ct->m_nMaxOffset) + (1<<QS_SHIFT_FACTOR);
				if (nAdjust>(3<<QS_SHIFT_FACTOR)) nAdjust=(3<<QS_SHIFT_FACTOR);
				if ((ct->m_nLastFrameAvgQP[0]<<6) > ct->m_nLongTermQP64) {  // Danger! Make it more aggressive
					nHighBound = ct->m_nLastFrameAvgQP[0]+nAdjust;
					newQPStep = ct->m_nLastFrameAvgQP[0]+nAdjust;
				}
				else {
					nHighBound += nAdjust;
					newQPStep += nAdjust;
				}
			}
			else if (ct->m_nGaugeBitrate>ct->m_nBitrate) {
				if (newQPStep < ct->m_nLastFrameAvgQP[0])
					newQPStep = ct->m_nLastFrameAvgQP[0];
				if ((newQPStep<<6) < ct->m_nLongTermQP64)
					newQPStep = ct->m_nLongTermQP64>>6;
			}
			else {
				if ((newQPStep<<6) >= ct->m_nLongTermQP64)
					newQPStep = (ct->m_nLongTermQP64>>6)-1;
				if (ct->m_nTargetFullness>ct->m_nBufFullness) {
					nAdjust = (ct->m_nTargetFullness-ct->m_nBufFullness) / (int)ct->m_nBitrate;
					newQPStep -= nAdjust;
				}
			}
		}
		newQPStep = MSRC_MIN(nHighBound, newQPStep);
		newQPStep = MSRC_MAX(nLowBound, newQPStep);
	}
	else if (ct->m_rcMethod==VARIABLE_BITRATE) {
		int nLowBound, nHighBound;
		if (ct->m_nPCount>0 && nFrameType==I_VOP) {
			newQPStep = ct->m_nLastFrameAvgQP[0];
			if ((ct->m_nLastFrameAvgQP[0]<<6) > ct->m_nLongTermQP64)
				newQPStep = newQPStep-(1<<QS_SHIFT_FACTOR);
		}
		else {
			int nAdjLTQ;
			MFE_S64 tmp;
			if (ct->m_nFrameCount>=ct->m_nTargetFrameRateInt || ct->m_nPCount==0) {
	              tmp=  MFE_DIV_S64(
				  	(MFE_S64)ct->m_nLongTermQP64 * MFE_DIV_S64(ct->m_nTotalBits,ct->m_nFrameCount),
				  	ct->m_nAvgBitsPerFrame) ;
                 //tmp = ct->m_nLongTermQP64 * (ct->m_nTotalBits/ct->m_nFrameCount) / ct->m_nAvgBitsPerFrame;
				MFE_ASSERT((tmp >> 6) < (1<<31));

	              nAdjLTQ=  (int)(MFE_DIV_S64(
				  	(MFE_S64)ct->m_nLongTermQP64 * MFE_DIV_S64(ct->m_nTotalBits,ct->m_nFrameCount),
				  	ct->m_nAvgBitsPerFrame)   >>6) ;
	//nAdjLTQ = (int)((ct->m_nLongTermQP64 * (ct->m_nTotalBits/ct->m_nFrameCount) / ct->m_nAvgBitsPerFrame) >> 6);

			}
			else {
				nAdjLTQ = ct->m_nLongTermQP64>>6;   // Wait for stabilization
			}
			MFE_ASSERT(ct->m_nTargetBits>0);
			newQPStep = (nAdjLTQ * ct->m_nAvgBitsPerFrame) / ct->m_nTargetBits;

			nAdjust = MSRC_MAX((2<<QS_SHIFT_FACTOR), (int)(nAdjLTQ)>>2);
			nLowBound = (int)(nAdjLTQ) - nAdjust;
			nHighBound = (int)(nAdjLTQ) + nAdjust;
			if (ct->m_nLastFrameBits>ct->m_nLastTargetBits) {
				nAdjust = (int)(ct->m_nLastFrameBits/ct->m_nLastTargetBits);
				if (nAdjust>2) nAdjust=2;
				nHighBound += nAdjust;
			}

			if (ct->m_nAvgBitsPerFrame>ct->m_nTargetBits) {
				newQPStep = MSRC_MIN(nHighBound, newQPStep);
			}
			else {
				newQPStep = MSRC_MAX(nLowBound, newQPStep);
			}
		}
	}
	
	return newQPStep;
}

// int QP2QSTEP32[52] = {	// QP2Qstep(QP) * 32
// 	20, 
// 	22, 26, 28, 32, 36, 40, 44, 52, 56, 64,
// 	72, 80, 88, 104, 112, 128, 144, 160, 176, 208,
// 	224, 256, 288, 320, 352, 416, 448, 512, 576, 640,
// 	704, 832, 896, 1024, 1152, 1280, 1408, 1664, 1792, 2048,
// 	2304, 2560, 2816, 3328, 3584, 4096, 4608, 5120, 5632, 6656,
// 	7168,
// };
int rcQP2Qstep(CVBRRateControl* ct,  int QP)
{
    if (ct->m_nCodecType==REG_ENC_MODE_H264)
    {
        int i;
        int Qstep;
        static const int QP2QSTEP[6] = { 20, 22, 26, 28, 32, 36 };
        
        Qstep = QP2QSTEP[QP % 6];
        for( i=0; i<(QP/6); i++)
            Qstep *= 2;
        
        return Qstep;
    }
    else
    {
        return QP<<QS_SHIFT_FACTOR;
    }

    return 0;   // Should never entering this.
}

int rcQstep2QP(CVBRRateControl* ct, int QstepX32)
{
    if (ct->m_nCodecType==REG_ENC_MODE_H264)
    {
        int q_per = 0, q_rem = 0;
        
        //  MFE_ASSERT( QstepX32 >= QP2QSTEP[0] && QstepX32 <= QP2QSTEP[51]);
        if( QstepX32 <= ct->m_nMinQStep)
            return ct->m_nMinQP;
        else if (QstepX32 > ct->m_nMaxQStep)
            return ct->m_nMaxQP;
        
        while( QstepX32 > rcQP2Qstep(ct, 5) )
        {
            QstepX32 >>= 1;
            q_per += 1;
        }
        
        if (QstepX32 <= 21)
        {
            QstepX32 = 20;
            q_rem = 0;
        }
        else if (QstepX32 <= 24)
        {
            QstepX32 = 22;
            q_rem = 1;
        }
        else if (QstepX32 <= 27)
        {
            QstepX32 = 26;
            q_rem = 2;
        }
        else if (QstepX32 <= 30)
        {
            QstepX32 = 28;
            q_rem = 3;
        }
        else if (QstepX32 <= 34)
        {
            QstepX32 = 32;
            q_rem = 4;
        }
        else
        {
            QstepX32 = 36;
            q_rem = 5;
        }
        
        return (q_per * 6 + q_rem);
    }
    else if (ct->m_nCodecType==REG_ENC_MODE_MPG4 || ct->m_nCodecType==REG_ENC_MODE_H263)
    {
        if( QstepX32 <= ct->m_nMinQStep)
            return ct->m_nMinQP;
        else if (QstepX32 > ct->m_nMaxQStep)
            return ct->m_nMaxQP;
        
        return QstepX32>>QS_SHIFT_FACTOR;
    }

    return 0;   // Should never entering this.
}

#ifdef _SUPPORT_MBLEVEL_RC_

#define DUMP_FRAME_NO -1 //140
// Each MB must call this routine once
int cvbr_InitMB(CVBRRateControl* ct, int nVar, const int nPrevQP, const int nBits, int IsIntra, int IsP4MV, int BPredType, int nResetDQ)
{
#ifdef USE_INT_PREC
	int nAvgAct, nActj, nNBits=0;
#else 
	float fAvgAct, fActj, fNBits;
#endif
	int QStep, RtnQP, nVariance;
//        int TgtQP;
	MFE_BOOL bChangeQP = 0;
	int nMBX, nMBY;

#ifdef RAND_INPUT 
	nVariance = nVar = rand()%128;
	//IsIntra = (rand()%4)==0 ? 1 : 0;
	//Is4MV = (rand()%4)==0 ? 1 : 0; 
#else
	nVariance = nVar;
#endif

//     if (ct->m_nFrameCount == DUMP_FRAME_NO)
//         printf("ct->m_nMBN = %d (MBX, MBY) = (%2d, %2d), nBits = %d\n", ct->m_nMBN, ct->m_nMBN%ct->m_nMbWidth, ct->m_nMBN/ct->m_nMbWidth, nBits);

// 	if (ct->m_rcGranularity==FRAMELEVELRC) {
// 		return rcQstep2QP(ct->m_nFrameQP);
// 	}
	
	if (ct->m_nMBN==0) { // Initialization
		ct->m_nSumAct = 0;
		ct->m_nTargetUsedBits = 0;
	}

	if (ct->m_nMBN>0)
	{ // Previous MB updating: Last MB is not counted!
		// Update QP
		ct->m_nPrevTopQP[(ct->m_nMBN-1)%ct->m_nMbWidth] = nPrevQP;
		ct->m_nLastFrameAvgQP[ct->m_bIsBotField] += rcQP2Qstep(ct, nPrevQP);

		// SW simulation hw pipeline: Update bit usage history
		if (ct->m_nMBN<3)
			ct->m_nUsedBits[ct->m_nMBN-1] = nBits;
		else {
			ct->m_nUsedBits[0] = ct->m_nUsedBits[1];
			ct->m_nUsedBits[1] = nBits;
		}
	}

	if (ct->m_nMBN>1)	// HW pipeline: See below nNBits (or fNBits) calculation
		ct->m_nTargetUsedBits += ct->m_nTargetMbBits;

	// Update history
	if (nVariance==0) nVariance = 1;
	ct->m_nSumAct += nVariance;

	// QP calculation
	ct->m_nNewPacket = 0; // Default
	if (ct->m_nMBN<2) {
		RtnQP = rcQstep2QP(ct, ct->m_nFrameQP);
	}
	else 
	{
		if (ct->m_nCodecType==REG_ENC_MODE_MPG4 && (!IsIntra) && ((ct->m_nFrameType==P_VOP&&IsP4MV)||(ct->m_nFrameType==B_VOP&&BPredType==0))) {
			RtnQP = nPrevQP;	// Not allowing DQ!=0
		}
		else {
#ifdef USE_INT_PREC
			nNBits = (ct->m_nUsedBits[0]<<SCALE_BITS) / ct->m_nTargetUsedBits;
			//MFE_ASSERT(nNBits<(1<<(SCALE_BITS+6)));
			if (nNBits>=(1<<(SCALE_BITS+6)))
				nNBits = (1<<(SCALE_BITS+6)) - 1;
			nAvgAct = ct->m_nSumAct / (ct->m_nMBN+1);
			nActj = (((nVariance<<1) + nAvgAct) << SCALE_ACT) / (nVariance + (nAvgAct<<1));
			// Target QStep
			if (ct->m_nFrameType==I_VOP)
				QStep = (int)(((MFE_S64)ct->m_nFrameQP * nActj) >> SCALE_ACT);
			else
				QStep = (int)(((MFE_S64)ct->m_nFrameQP * nActj * nNBits) >> (SCALE_ACT+SCALE_BITS));
#else
			fNBits = (float)ct->m_nUsedBits[0] / ct->m_nTargetUsedBits;
			fAvgAct = (float)ct->m_nSumAct / (ct->m_nMBN+1);
			fActj = ((nVariance*MBACT_NORMALIZATION_FACTOR) + fAvgAct) / (nVariance + (fAvgAct*MBACT_NORMALIZATION_FACTOR));
			// Target QStep
			if (ct->m_nFrameType==I_VOP)
				QStep = (int)(ct->m_nFrameQP * fActj);
			else
				QStep = (int)(ct->m_nFrameQP * fActj * fNBits);
#endif
			// 		if (QStep<(1<<QS_SHIFT_FACTOR))
			// 			QStep = (1<<QS_SHIFT_FACTOR);
			MFE_ASSERT(QStep>=0 && QStep<(1<<20));

			// Target QP
			RtnQP = rcQstep2QP(ct, QStep);
			// Refinement: Phase 1, top and left MB-QP limitation
			if (ct->m_nMBN >= ct->m_nMbWidth) {
				if (RtnQP > ct->m_nPrevTopQP[ct->m_nMBN%ct->m_nMbWidth] + TOP_QP_DIFF_LIMIT) {
					RtnQP = ct->m_nPrevTopQP[ct->m_nMBN%ct->m_nMbWidth] + SPEC_QP_DIFF_LIMIT;
					bChangeQP = 1;
				}
				else if (RtnQP < ct->m_nPrevTopQP[ct->m_nMBN%ct->m_nMbWidth] - TOP_QP_DIFF_LIMIT) {
					RtnQP = ct->m_nPrevTopQP[ct->m_nMBN%ct->m_nMbWidth] - SPEC_QP_DIFF_LIMIT;
					bChangeQP = 1;
				}
			}
			if (RtnQP > nPrevQP + LEFT_QP_DIFF_LIMIT) {
				RtnQP = nPrevQP + SPEC_QP_DIFF_LIMIT;
				bChangeQP = 1;
			}
			else if (RtnQP < nPrevQP - LEFT_QP_DIFF_LIMIT) {
				RtnQP = nPrevQP - SPEC_QP_DIFF_LIMIT;
				bChangeQP = 1;
			}
			// Refinement: Phase 2, spec QP range limitation
			RtnQP = MSRC_MAX(ct->m_nMinQP, RtnQP);
			RtnQP = MSRC_MIN(ct->m_nMaxQP, RtnQP);
#if 0	// Don't need this because QP is bounded by rcQstep2QP(),
			// and if TOP_QP_DIFF_LIMIT and BOTTOM_QP_DIFF_LIMIT >=2, there is no way to have +-1.
			// Refinement: Phase 3, MPEG-4 B-frame limitation
			if (ct->m_nCodecType==REG_ENC_MODE_MPG4 && ct->m_nFrameType==B_VOP /*&& RtnQP!=nPrevQP*/) {
				if (RtnQP-nPrevQP==-1 || RtnQP-nPrevQP==1)
					RtnQP = nPrevQP;
			}
#endif
			// Avoid frequent QP changing
			if (!bChangeQP)
				RtnQP = nPrevQP; // Reset
		}

		//DEBUG_RC(("  [CVBRRC] %d : NBits=%d, AvgAct=%d\n", ct->m_nMBN, (int)fNBits, (int)fAvgAct ));

		//////////////////////////////////////////////////////////////////////////
		// er_en 
		if (ct->m_nVPSize) {
			if (ct->m_nUsedBits[0]-ct->m_nLastVPBits>=ct->m_nVPSize) {
				if (ct->m_nVPMbRow) {
					nMBX = ct->m_nMBN%ct->m_nMbWidth;
					nMBY = ct->m_nMBN/ct->m_nMbWidth;
					if (nMBX==0 && nMBY>0 && (nMBY%ct->m_nVPMbRow)==0) {
						ct->m_nNewPacket = 1;
						ct->m_nLastVPBits = ct->m_nUsedBits[0];
					}
				}
				else {
					ct->m_nNewPacket = 1;
					ct->m_nLastVPBits = ct->m_nUsedBits[0];
				}
			}
		}
		else if (ct->m_nVPMbRow) {
			nMBX = ct->m_nMBN%ct->m_nMbWidth;
			nMBY = ct->m_nMBN/ct->m_nMbWidth;
			if (nMBX==0 && nMBY>0 && (nMBY%ct->m_nVPMbRow)==0) {
				ct->m_nNewPacket = 1;
				ct->m_nLastVPBits = ct->m_nUsedBits[0];
			}
		}
	}

	if (ct->m_rcGranularity==FRAMELEVELRC) {
		RtnQP = rcQstep2QP(ct, ct->m_nFrameQP);
	}

	ct->m_nMBN++;
	if (ct->m_rcMethod==CONST_QUALITY)
		return ct->m_nConstQP;
	MFE_ASSERT(RtnQP<=ct->m_nMaxQP && RtnQP>=ct->m_nMinQP);
	return RtnQP;
}

#else	// _SUPPORT_MBLEVEL_RC_ not defined

int cvbr_InitMB(CVBRRateControl* ct, int nVar, const int nPrevQP, const int nBits, int IsIntra, int IsP4MV, int BPredType, int nResetDQ)
{
	if (ct->m_nMBN>0)
		ct->m_nLastFrameAvgQP[ct->m_bIsBotField] += rcQP2Qstep(nPrevQP);
	if (ct->m_rcMethod==CONST_QUALITY)
		return ct->m_nConstQP;
	return rcQstep2QP(ct->m_nFrameQP);
}

#endif // _SUPPORT_MBLEVEL_RC_
