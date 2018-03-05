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

#ifndef _MFE_COMMON_H_
#define _MFE_COMMON_H_

#include "mfe_reg.h"
#include "IntraUpdate.h"
#include "msRateCtrl.h"
#include "OutStrm.h"
#include "udma_share.h"
#include "MFE_chip.h"

// if not T8 and KERNEL
#if (!defined(_KERNEL_MODE_) || !defined(_MFE_T8_))
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#endif


#if (defined(_MFE_T8_)&&defined(_MIPS_PLATFORM_)&&defined(_KERNEL_MODE_))
		#ifndef _MFE_ASSERT_
			#define _MFE_ASSERT_
                    #define MFE_ASSERT(p)   do {\
                        if (!(p)) {\
                            printk(KERN_CRIT "MFE BUG at %s:%d assert(%s)\n",\
                                    __FILE__, __LINE__, #p);\
                            BUG();\
                        }\
                    } while (0)
		#endif
#include <linux/string.h>
#include "mdrv_types.h"
#else
#include <assert.h>
#define 	MFE_ASSERT(p) assert(p)
#endif

extern MFE_REG mfe_reg;

#define	MIU_MB_SIZE	16

#define MAX_RESOLUTION_X	960
#define MAX_RESOLUTION_Y	576

// Used in IME and FME
#define BLOCK16x16 1
#define BLOCK16x8  2
#define BLOCK8x16  3
#define BLOCK8x8   4
#define BLOCK8x4   5
#define BLOCK4x8   6
#define BLOCK4x4   7

typedef enum {
	M4VE_FRM_CUR_ORI = 0,
	M4VE_FRM_REF0_ORI,
	M4VE_FRM_REF1_ORI,
	M4VE_FRM_CUR_REC,
	M4VE_FRM_REF0_REC,
	M4VE_FRM_REF1_REC,
} M4VE_FRM_NAME;

typedef enum
{
	FRAME_CODING = 0,
	FIELD_CODING,
	ADAPTIVE_CODING,		// Not supported!
	FRAME_MB_PAIR_CODING	// Not supported!
} CodingType;

typedef enum
{
	FRAME = 0,
	TOP_FIELD,
	BOTTOM_FIELD
} PictureStructure;           //!< New enum for field processing

typedef struct _GOPINFO {
	int nPCount;
	int nBCount;
	MFE_U32 nCodingOrder;
	MFE_U32 nDispOrder;	// Display order of current frame
} GOPINFO;

// Mstar: for swcfg1
typedef struct BufInfo
{
	MEMMAP_t m_nCurYAddr;
	MEMMAP_t m_nCurCAddr;
	MEMMAP_t m_nRefYAddr[2];
	MEMMAP_t m_nRefCAddr[2];
	MEMMAP_t m_nRecYAddr;
	MEMMAP_t m_nRecCAddr;
       int m_nOutBuffer;
       int m_OutBufferSize;
	MEMMAP_t m_BitsOutBuffer[MAX_OUTBUFFER];
	MEMMAP_t m_nOutBufAddr[MAX_OUTBUFFER];
	// Below is for video
	MEMMAP_t  m_nGNAddr;
	// Below is only for MPEG-4
	MEMMAP_t  m_nMvStoreAddr;
	MFE_U8 m_bEnableMvStore;
	// Below is only for JPEG
	int m_nRowBufCount;
} BufInfo;

typedef struct _JpgInfo
{
	int nFrameMode;
	int nUseYVYU;
	int nWidth, nHeight;
	int nLastZZ;
	int QTable[2][64];
	int nQFactor;
} JpgInfo;

typedef struct _M4VEINFO
{
	MFE_U8 bShortHeader;	// 0: MPEG-4, 1: H263
	MFE_U8 bInterlacedCoding;	// Seq-wide 0: Progressive, 1: Use Field-DCT

	// Frame type
	MFE_U8 bInterlace;			// Frame-wide 0: Progressive, 1: Use Field-DCT
	MFE_U8 bAllowSkippedPMBs;	// If B-frame, this must be 0
	MFE_U8 nAllowDirectBMBs;
	MFE_U8 bMvStore;
	MFE_U8 iRoundingControlSwitch, iRoundingControl;
	MFE_U8 g_rec_en;
	int g_ref_en;
	int m_nLastZZ;
	int intStep;

	// Frame time
	int m_tFutureRef;
	int m_tPastRef;
	int m_t;
	int m_tModuloBaseDecd;
	int m_tModuloBaseDisp;
	// Derived
	int m_nBitsModuloBase;
	int m_iVopTimeIncr;

	// Motion
	int m_nFmePrec;
	int SEARCH_RANGE_Y;
	int iSearchRangeForward;
	MFE_U8 IME_ADAPTIVE_WINDOW;
	// Derived
	int iFCode;

	// VideoPacket, GOB parameters
	int m_nBitsResyncMarker;
	int nNumBitsVPMBnum;
	MFE_U8 bHECEnabled;
	int nHECPeriod;

	// Time parameters
	int nTimeResolution;
	int nFixedIncrement;
	int iClockRate;
	int nNumBitsTimeIncr;

	// Quant
	MFE_U8 bQuantizerType;	// 0: H263, 1: MPEG-4 (Q-matrices)
	int rgiIntraQuantizerMatrix[64];
	int rgiInterQuantizerMatrix[64];

	// H263
	int nGobUnit;
	int m_iGobFrameId;

	// Frame buffer
	MEMMAP_t m_FrameBuf[6][2];
} M4VEINFO;

#define MAX_BUFPOOL_COUNT 12//20
typedef struct _FrameBufPool {
	MEMMAP_t addr[MAX_BUFPOOL_COUNT][2];	// [cur, ref0, ref1, rec][Y, C]
    char available[MAX_BUFPOOL_COUNT];
} FrameBufPool;

#define DPB_SIZE 3
typedef struct _DPB_ITEM {
	MEMMAP_t addr[2];
	char is_used;
} DPB_ITEM;

typedef struct _H264INFO
{
	int ProfileIDC, LevelIDC;
	CodingType PicInterlace;

	int nMaxP8x8Count;
	int log2_max_frame_num_minus4;

    // ME
    int num_ref_frames;
    int	iSearchRangeForward;
    int SEARCH_RANGE_Y;
    MFE_U8 IME_ADAPTIVE_WINDOW;
    int fme_precision;	// 0--fullpel, 1--halfpel, 2--quarterpel
    int nMaxFmeMode;
    MFE_U8 no_umv;
    int InterSearch[2][8];
    int num_ref_idx_l0_active_minus1;	// PPS

    // DBF
    MFE_U8 bDeblockCtrlPresent;
    int nDeblockIDC;
    int nDeblockAlpha, nDeblockBeta;

    // Frame type
	int pic_order_cnt_type;
	PictureStructure structure;
    MFE_U8 idr_flag;
    int nal_ref_idc;
    int frame_num;
    int number;
    int num_ref_idx_l0_active;
    int RefCount;	// 0: 1ref, 1: 2ref
    int h264_mcc_offset[2];
    int intStep;

	// Slice
	MFE_U8 pic_order_present_flag;
	int delta_pic_order_cnt_bottom;

    // IEAP
	int ieap_last_mode;
	int ieap_constraint_intra;
	int ieap_ccest_en;
	int ieap_ccest_thr;
	int ieap_drop_i16;

	// Frame buffer
	FrameBufPool BufPool;
	DPB_ITEM dpb[DPB_SIZE];
} H264INFO;

typedef struct BitsInfo {
	unsigned char* ptr;
	int len;						// Number of bytes (not including the last not-full byte, if existed)
	unsigned char bits;	// The last not-full byte, 0 if not existed
	int bit_len;				// Number of bits of the last not-full byte, 0 if not-existed.
} BitsInfo;

typedef struct _MFE_CONFIG
{
	// [Input]
	int nCodecType;
	int nDispWidth, nDispHeight;	// Display resolution
	int nBufWidth, nBufHeight;		// Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
	MFE_U8 bInterlace;
	MFE_U8 bQuantType;
	// GOP
	int nPbetweenI;
	int nBbetweenP;
       int nP_is_infinite;
	// Rate control
	int FrameRatex100;
	int nBitrate;
	// Error resilience
	int nVPSizeInBits;
	int nVPSizeInMbRow;
	int g_intraupdate_enable;
	int g_intraupdate_period;
	// Memory buffer
	MFE_U8* dram_base;	// Start pointer
	MFE_U32 dram_size;	// Maximal size for use

	// Buffer
	BufInfo ctxBufInfo;
	MFE_U32     nOBufIndex;
	// Memory buffer
	MFE_U8* dram_ptr;	// Curent pointer of available memory buffer

	// Frame stat
	MFE_U32 nCodingOrder;
	MFE_U8 vopPredType;			// I_VOP, P_VOP, B_VOP

	// Headers
	BitsInfo ctxBitsInfo;
	OutStream m_OutStream;
	MFE_U8 m_FdcBuffer[1024];

	// Format-dependent
	union {
		JpgInfo ctxJpgInfo;
		M4VEINFO ctxM4veInfo;
		H264INFO ctxH264Info;
	};

	// H263/MPEG4/H264 rate control
	CVBRRCInfo rcInfo;
	CVBRRateControl ctxRateControl;
	int m_cvbrFrameSkip;
	int m_bGenSkipVopHeader;

	// IntraUpdate
	IntraUpdateContext m_IUContext;
	HW_MB_MAP m_UIHwMap[MAX_RESOLUTION_X*MAX_RESOLUTION_Y/256];
	SW_MB_MAP m_UISwMap[MAX_RESOLUTION_X*MAX_RESOLUTION_Y/256];

	int test_case_format;
	int test_case_num;
	unsigned char CModel_pattern[20*8];//20 frames

} MFE_CONFIG;

// Null functions
// #define sd_output_rel(arg1, arg2, arg3)
// #define sd_output(arg1, arg2, arg3, arg4)
// #define sd_output_reg(arg1, arg2, arg3, arg4, arg5, arg6)

void ReadRegMFE(MFE_U32 u32Address, MFE_U16 *val);
void WriteRegMFE(MFE_U32 u32Address, MFE_U16 val, char *str1, int num, char *str3);
void WriteRegMFE_Bank1(MFE_U32 u32Address, MFE_U16 val, char *str1, int num, char *str3);
void DumpAllReg(void);

int PutFDC(void* pContext, int mode);
void WriteQTable(int* Table0, int* Table1);

void SetObufAddr(MFE_U16 sadr_low, MFE_U16 sadr_high, MFE_U16 eadr_low, MFE_U16 eadr_high);
void ClearBsfFullIRQ(void);

void ClearIRQ(int number);
void Enable_HW(void);
void ResetAllRegs(void);

void TestStopAtMb(void);
void TestStop(void);

void IntraUpdateInit(MFE_CONFIG *pConfig);
void IntraUpdateFrame(MFE_CONFIG *pConfig);
void IntraUpdateClose(MFE_CONFIG *pConfig);

// Rate control
void MfeDrvRateControlInit(MFE_CONFIG* pConfig);
void MfeDrvRateControlUpdate(MFE_CONFIG* pConfig, char nFieldType);

// H263/MPEG4
void mfeM4VE_Init(MFE_CONFIG *pConfig);
void mfeM4VE_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo);
void mfeM4VE_UpdateFrame(MFE_CONFIG *pConfig);
int codeM4vConfigHeaders(MFE_CONFIG *pConfig);

// H264
void mfeH264_Init(MFE_CONFIG *pConfig);
void mfeH264_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo);
void mfeH264_UpdateFrame(MFE_CONFIG *pConfig);
int codeH264ConfigHeaders(MFE_CONFIG *pConfig);

#endif // _MFE_COMMON_H_

