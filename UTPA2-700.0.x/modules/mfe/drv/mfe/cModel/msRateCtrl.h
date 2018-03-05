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

#ifndef _RATECTRL_H_
#define _RATECTRL_H_

#include "mfe_type.h"

/************************************************************************/
/* Configuration                                                        */
/************************************************************************/

// Define this to support MB-level rate control
#define _SUPPORT_MBLEVEL_RC_

// Define this to declare RC variables outside RC kernel
// NOTE: MFE_MPEG4_CMODEL-- define this.
//#define _USE_EXT_RCDATA_

// The maximal allowable MB width
#define MAX_MB_WIDTH (960>>4)

/* ~Configuration *******************************************************/

#ifndef MSRC_MAX
#define  MSRC_MAX(a,b)              (((a) > (b)) ? (a) : (b))
#endif
#ifndef MSRC_MIN
#define  MSRC_MIN(a,b)              (((a) < (b)) ? (a) : (b))
#endif

#ifdef _SUPPORT_MBLEVEL_RC_
	#define TOP_QP_DIFF_LIMIT          3 // Must >= SPEC_QP_DIFF_LIMIT
	#define LEFT_QP_DIFF_LIMIT         3 // Must >= SPEC_QP_DIFF_LIMIT
#endif

typedef enum {
	CONST_QUALITY = 1,								//!< Constant qscale: Use specified QP.
	CONST_BITRATE = 2,								//!< Constant bitrate. 
	VARIABLE_BITRATE = 3,							//!< Variable bitrate: instant bitrate is unrestricted.
	CONSTRAINED_VARIABLE_BITRATE = 4,	//!< Variable bitrate: instant maximal bitrate is restricted.
} RC_METHOD;

typedef enum {
	FRAMELEVELRC = 1,   //!< All MB's in one frame uses the same qscale.
	MBLEVELRC = 2       //!< Allow qscale changing within frame, for better bitrate achievement.
} RC_GRANULARITY;

// For CONSTRAINED_VARIABLE_BITRATE
#define MAX_GAUGE_SIZE  60

/* Input for rate control */
typedef struct {
	int nCodecType;	// 0/1/2/3: MPEG4/H263/H264/JPEG
	int nWidth, nHeight;
	MFE_BOOL fTargetFrameRate_is_float;
	float fTargetFrameRate;
	int int_fTargetFrameRate;
	int nBitrate;     // Meaningful only when rcMethod!=CONST_QUALITY (bits)
										// If rcMethod=CONSTRAINED_VARIABLE_BITRATE, 0 means internally-decided.
	int nMaxBitrate;  // Meaningful only when rcMethod=CONSTRAINED_VARIABLE_BITRATE (Bits)
										// For others, this should be 0.
	int nConstQP;			// Meaningful only when rcMethod=CONST_QUALITY
	// 
	// For coded frame slicing: GOB (H.263), video packet (MPEG4), or slice (H.264)
	int nVPSize;	// Video packet size in bits.
	int nVPMbRow;   // Video packet size in number of MB rows. If H.263, this must be multiple of "MBrow per GOB".
	//
	MFE_BOOL bFixedFrameRate;	// If FALSE, rate control can signal frame-skipping.
	int nPCount;			// Number of P-frames between I-frames
	int nBCount;			// Number of B-frames between P-frames // Suggest: 0, 1, 2
	RC_GRANULARITY rcGranularity;
	RC_METHOD rcMethod;
} CVBRRCInfo;

typedef struct {
	/* Input parameters */

	int m_nCodecType;
	int m_nWidth, m_nHeight;
	int m_nTargetFrameRateNum, m_nTargetFrameRateDeNum, m_nTargetFrameRateInt;	// The target rate is Num/Denum
	int m_nBitrate;
	int m_nMaxBitrate; // Only when CONSTRAINED_VARIABLE_BITRATE
	int m_nConstQP;   // Only when CONST_QUALITY
	int m_nVPSize, m_nVPMbRow;
	MFE_BOOL m_bFixedFrameRate;
	int m_nPCount, m_nBCount;
	RC_GRANULARITY m_rcGranularity;
	RC_METHOD m_rcMethod;
	// Derived variables
	int m_nAvgBitsPerFrame;
	int m_nBitsPerFrame[3];	// I, P, B

	/* rate control variables */

	int m_nFrameCount;  // How many frame coded
	int m_nBufFullness;  // Rate control buffer
	MFE_S64 m_nTotalBits;
	// Last-frame status
	MFE_BOOL m_bIsBotField;
	int m_nLastFrameBits, m_nLastFrameAvgQP[2];
	int m_nLastTargetBits;

	/* Bitrate usage compensation */
	int m_nTargetFullness;
	int m_nDeputyCount, m_nMinDeputyCount;

	/* Variable bitrate */
	int m_nLongTermQP64;

	// Model parameters

	int m_nTargetBits;   // target number of bits of current frame
	int m_nTotalMB;			// number of macroblocks in a frame
	int m_nMBN;					// Accumulated handled MB count (within one frame)
	int m_nFrameQP;
	int m_nFrameType;

	/* Only for CONSTRAINED_VARIABLE_BITRATE */
	int m_nMaxOffset;
	int m_BitrateGauge[MAX_GAUGE_SIZE];
	int m_nGaugeCount, m_nGaugeIndex, m_nGaugeBitrate;
	int m_nMaxFrozenFrame;

	// QP, QStep: Min, Max 
	int m_nMinQP, m_nMaxQP;
	int m_nMinQStep, m_nMaxQStep;
	int m_nFrameSkipThrQP;

	/* MB-level rate control */
#ifdef _SUPPORT_MBLEVEL_RC_
	int m_nTargetMbBits;
	int m_nTargetUsedBits;
	int m_nUsedBits[3];	// For HW pipeline delay
	int m_nSumAct;
	int m_nMbWidth, m_nPrevQP, m_nLeftQP, m_nPrevTopQP[MAX_MB_WIDTH];
	int m_nLastTargetQP;	// For sig_dump
	// For coded frame slicing
	int m_nNewPacket;	// New GOB, video packet, slice... Simulating HW er_en
	int m_nLastVPBits;
#endif

	int* m_pMBBitsArray;	// Only for DEBUG usage.
} CVBRRateControl;

// Global
void cvbr_InitRateControl(CVBRRateControl* ct, CVBRRCInfo* pRCInfo);
void cvbr_CloseRateControl(CVBRRateControl* ct);
// Each frame
int cvbr_InitFrame(CVBRRateControl* ct, int nFrameType, char FieldType);    // Return the initial frame qp
int cvbr_UpdateFrame(CVBRRateControl* ct, int totalUsedBits, char bDummyFrame, char FieldType);
// Each macroblock
int cvbr_InitMB(CVBRRateControl* ct, int nVar, const int nPrevQP, const int nBits, int IsIntra, int IsP4MV, int BPredType, int nResetDQ);
void output_MBR_reg(void* hd, CVBRRateControl* ct);

#endif
