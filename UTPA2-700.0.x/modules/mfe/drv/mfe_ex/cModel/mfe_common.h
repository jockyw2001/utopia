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


#include "MFE_chip.h"
#include "mfe_reg.h"
#include "IntraUpdate.h"
#include "msRateCtrl.h"
#include "OutStrm.h"
#include "udma_share.h"
#include "mfe_h263_rc.h"
#include "parsetcommon.h"
#include "nalucommon.h"

// if not T8 and KERNEL
#if (!defined(_KERNEL_MODE_) && defined(_MFE_T8_))
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#endif

/*
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
#elif defined(__UBOOT__)
#define     MFE_ASSERT(p)
#elif defined(__MOBILE_CASE__)
#include <assert.h>
#define     MFE_ASSERT(p)  assert(p)
#elif defined(_MFE_UTOPIA_)
#define     MFE_ASSERT(p) MS_ASSERT(p)
#else
#include <assert.h>
#define     MFE_ASSERT(p) assert(p)
#endif
*/

extern MFE_REG* mfe_reg;
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
extern MFE_REG1* mfe_reg1;
#endif
#define    MIU_MB_SIZE    16UL


// Used in IME and FME
#define BLOCK16x16 1UL
#define BLOCK16x8  2UL
#define BLOCK8x16  3UL
#define BLOCK8x8   4UL
#define BLOCK8x4   5UL
#define BLOCK4x8   6UL
#define BLOCK4x4   7UL

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
    ADAPTIVE_CODING,        // Not supported!
    FRAME_MB_PAIR_CODING    // Not supported!
} CodingType;

/* Don't change the numbering! It must match cvbr_InitFrame() FieldType definition. */
typedef enum
{
    FRAME = 0,
    TOP_FIELD = 1,
    BOTTOM_FIELD = 2
} PictureStructure;           //!< New enum for field processing

typedef struct _GOPINFO {
    MS_S32 nPCount;
    MS_S32 nBCount;
    MS_U32 nCodingOrder;
    MS_U32 nDispOrder;    // Display order of current frame
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
    MS_S32 m_nOutBuffer;
    MS_S32 m_OutBufferSize;
    MEMMAP_t m_BitsOutBuffer[MAX_OUTBUFFER];
    MEMMAP_t m_nOutBufAddr[MAX_OUTBUFFER];
    // Below is for video
    MEMMAP_t  m_nGNAddr;
    // Below is only for MPEG-4
    MEMMAP_t  m_nMvStoreAddr;
    MS_U8 m_bEnableMvStore;
    // Below is only for JPEG
    MS_S32 m_nRowBufCount;
} BufInfo;

typedef struct {
    MS_U32   start_addr;
    MS_U32   end_addr;
    MS_S32            used_size;	// 0 means not used
} OutBitSBUF;

typedef struct _JpgInfo
{
    MS_S32 nFrameMode;
    MS_S32 nUseMST422;
    MS_S32 nUseYVYU;
    MS_S32 nWidth, nHeight;
    MS_S32 nLastZZ;
    MS_S32 QTable[2][64];
    MS_S32 nQFactor;
    MS_S32 fdc_mode;
} JpgInfo;

typedef struct _M4VEINFO
{
    MS_U8 bShortHeader;    // 0: MPEG-4, 1: H263
    MS_U8 bInterlacedCoding;    // Seq-wide 0: Progressive, 1: Use Field-DCT

    // Frame type
    MS_U8 bInterlace;            // Frame-wide 0: Progressive, 1: Use Field-DCT
    MS_U8 bAllowSkippedPMBs;    // If B-frame, this must be 0
    MS_U8 nAllowDirectBMBs;
    MS_U8 bMvStore;
    MS_U8 iRoundingControlSwitch, iRoundingControl;
    MS_U8 g_rec_en;
    MS_S32 g_ref_en;
    MS_S32 m_nLastZZ;
    MS_S32 intQP;

    // Frame time
    MS_S32 m_tFutureRef;
    MS_S32 m_tPastRef;
    MS_S32 m_t;
    MS_S32 m_tModuloBaseDecd;
    MS_S32 m_tModuloBaseDisp;
    // Derived
    MS_S32 m_nBitsModuloBase;
    MS_S32 m_iVopTimeIncr;

    // Motion
    MS_S32 m_nFmePrec;
    MS_S32 SEARCH_RANGE_Y;
    MS_S32 iSearchRangeForward;
    MS_U8 IME_ADAPTIVE_WINDOW;
    // Derived
    MS_S32 iFCode;

    // VideoPacket, GOB parameters
    MS_S32 m_nBitsResyncMarker;
    MS_S32 nNumBitsVPMBnum;
    MS_U8 bHECEnabled;
    MS_S32 nHECPeriod;

    // Time parameters
    MS_S32 nTimeResolution;
    MS_S32 nFixedIncrement;
    MS_S32 iClockRate;
    MS_S32 nNumBitsTimeIncr;

    // Quant
    MS_U8 bQuantizerType;    // 0: H263, 1: MPEG-4 (Q-matrices)
    MS_S32 rgiIntraQuantizerMatrix[64];
    MS_S32 rgiInterQuantizerMatrix[64];

    // H263
    MS_S32 nGobUnit;
    MS_S32 m_iGobFrameId;

    // Frame buffer
    MEMMAP_t m_FrameBuf[6][2];
} M4VEINFO;

#define MAX_BUFPOOL_COUNT 3UL //20

typedef struct _FrameBufPool {
    MS_S32 FrameBufPoolSize;
    MEMMAP_t** addr; // [cur, ref0, ref1, rec][Y, C]
    MS_S32* available;
} FrameBufPool;

typedef struct _DPB_ITEM {
    MEMMAP_t addr[2];
    MS_S8 is_used;
    PictureStructure structure; // For H264 PAFF
} DPB_ITEM;

typedef struct _H264INFO
{
    MS_S32 ProfileIDC, LevelIDC;
    CodingType PicInterlace;

    MS_S32 nMaxP8x8Count;
    MS_S32 log2_max_frame_num_minus4;

    // ME
    MS_S32 num_ref_frames;
    MS_S32    iSearchRangeForward;
    MS_S32 SEARCH_RANGE_Y;
    MS_U8 IME_ADAPTIVE_WINDOW;
    MS_S32 fme_precision;    // 0--fullpel, 1--halfpel, 2--quarterpel
    MS_S32 nMaxFmeMode;
    MS_U8 no_umv;
    MS_S32 InterSearch[2][8];
    MS_S32 num_ref_idx_l0_active_minus1;    // PPS

    // DBF
    MS_U8 bDeblockCtrlPresent;
    MS_S32 nDeblockIDC;
    MS_S32 nDeblockAlpha, nDeblockBeta;

    // Frame type
    MS_S32 pic_order_cnt_type;
    PictureStructure structure;
    MS_U8 idr_flag;
    MS_S32 nal_ref_idc;
    MS_S32 frame_num;
    MS_S32 number;
    MS_S32 num_ref_idx_l0_active;
    MS_S32 RefCount;    // 0: 1ref, 1: 2ref
    MS_S32 h264_mcc_offset[2];
    MS_S32 intQP;
    // For PAFF handling
    MS_U8 vopPredType_FirstField;

    // Slice
    MS_U8 pic_order_present_flag;
    MS_S32 delta_pic_order_cnt_bottom;

    // IEAP
    MS_S32 ieap_last_mode;
    MS_S32 ieap_constraint_intra;
    MS_S32 ieap_ccest_en;
    MS_S32 ieap_ccest_thr;
    MS_S32 ieap_drop_i16;

    // Frame buffer
    FrameBufPool BufPool;
    MS_S32 dpb_size;
    DPB_ITEM* dpb;
} H264INFO;

typedef struct BitsInfo {
    MS_U8* ptr;
    MS_S32 len;                        // Number of bytes (not including the last not-full byte, if existed)
    MS_U8 bits;    // The last not-full byte, 0 if not existed
    MS_S32 bit_len;                // Number of bits of the last not-full byte, 0 if not-existed.
} BitsInfo;

typedef enum {
    WAIT_FRAME_DONE = 0,
    WAIT_INPUT_FRAME = 1,
    WRITE_MFE_REG = 2,
    WAIT_AVAIL_BITSBUF = 3,
} MFE_STAT;

typedef struct _MFE_CONFIG
{
    // [Input]
    MS_S32 nCodecType;
    MS_S32 nDispWidth, nDispHeight;    // Display resolution
    MS_S32 nBufWidth, nBufHeight;        // Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
    MFE_INTERLACE_INFO bInterlace;
    MS_U8 bQuantType;
    // GOP
    MS_S32 nPbetweenI;
    MS_S32 nBbetweenP;
    MS_S32 nP_is_infinite;
    // Rate control
    MS_S32 FrameRatex100;
    MS_S32 nBitrate;
    // Error resilience
    MS_S32 nVPSizeInBits;
    MS_S32 nVPSizeInMbRow;
    MS_S32 g_intraupdate_enable;
    MS_S32 g_intraupdate_period;
    // Memory buffer
    MS_PHY dram_base;    // Start pointer
    MS_U32 dram_size;    // Maximal size for use

    // Buffer
    BufInfo ctxBufInfo;
    MS_U32 nOBufIndex;
    // Memory buffer
    MS_PHY dram_ptr;    // Curent pointer of available memory buffer

    // Frame stat
    MS_U32 nCodingOrder;
    MS_U8 vopPredType;            // I_VOP, P_VOP, B_VOP

    // Headers
    BitsInfo ctxBitsInfo;
    OutStream m_OutStream;
    MS_U8 m_FdcBuffer[2048];

    // Format-dependent
    JpgInfo ctxJpgInfo;
    M4VEINFO ctxM4veInfo;
    H264INFO ctxH264Info;

    MS_U32 m_quality;
    MS_U32 m_bFrameMode;
    MS_U32 m_bFDC_mode;
    // H263/MPEG4/H264 rate control
    CVBRRCInfo rcInfo;
    CVBRRateControl ctxRateControl;
    MS_S32 m_cvbrFrameSkip;
    MS_S32 m_bGenSkipVopHeader;

    // IntraUpdate
    IntraUpdateContext m_IUContext;
    //HW_MB_MAP* m_UIHwMap;//[MAX_RESOLUTION_X*MAX_RESOLUTION_Y/256];
    //SW_MB_MAP* m_UISwMap://[MAX_RESOLUTION_X*MAX_RESOLUTION_Y/256];

    MS_S32 test_case_format;
    MS_S32 test_case_num;
    MS_U8 CModel_pattern[20*8];//20 frames
    MS_S32 TimeIncreamentRes;              // [IN] frame rate
    MS_S32 VopTimeIncreament;              // [IN] frame rate
    MFE_CLK_LEVEL MFEClk;
    MFE_ADV_INFO_t MfeAdvInfo;
    MFE_CROP_INFO_t  MfeCropInfo;
    MS_U32 imi_addr;
    MS_U32 imi_size;
    MS_U32 nDropFlag;
    MS_U32 VTMode;
    RateCtrl_t VTRateCtrl;
    MFE_COLOR_FORMAT_INFO bColorFormat;
    MS_U32 SecurityMode;
    MS_U32 PollingMode;
    MS_U32 g_nUseYUV422;
    MS_U32 UseCABAC;
    MS_U32 enableReduceBW;
    MS_U32 enableFastMode;
    GOPINFO GopInfo;

    //out buffer
    OutBitSBUF outbitsbuf[MAX_OUTBUFFER];
    MS_S32 rbits_index;

    MS_U32 _MFE_U32MFEEvent;
    BITSFRAME_INFO bitsframe[MAX_OUTBUFFER];

    MFE_STAT encode_state;
    MS_BOOL bForceIframe;
    MS_U32 MFE_U32MFEMIUOffset;

    seq_parameter_set_rbsp_t *active_sps;
    pic_parameter_set_rbsp_t *active_pps;
    // Static SPS, PPS NALU's
    MS_U8 NaluBuf[2][MAXRBSPSIZE];
    NALU_t NaluStruct[2];

#ifdef MFE_SUPPORT_TLB
    MS_U8 bEnableTLB;
#endif

    MFE_REG mfe_reg;
    MFE_REG1 mfe_reg1;

#ifdef MFE_MIU_PROFILE
    MS_U32 total_r_count;
    MS_U32 total_w_count;
    MS_U32 max_r_count;
    MS_U32 max_w_count;
    MS_U32 min_w_count;
    MS_U32 min_r_count;
    MS_U32 miu_count;
#endif
    //MFE_REG* mfe_reg_comm;

    MS_U32 setLevel;
    MS_BOOL setVUI_aspect_ratio_info_present_flag;
    MS_U32 setVUI_aspect_ratio_idc;
    MS_U32 setVUI_sar_width;
    MS_U32 setVUI_sar_height;

    MS_U8 encodeDummyFrame;
} MFE_CONFIG;

typedef struct _VUI_INFO
{
    MS_U32 level;
    MS_BOOL aspect_ratio_info_present_flag;
    MS_U32 aspect_ratio_idc;
    MS_U32 sar_width;
    MS_U32 sar_height;
} VUI_Info;

// Null functions
// #define sd_output_rel(arg1, arg2, arg3)
// #define sd_output(arg1, arg2, arg3, arg4)
// #define sd_output_reg(arg1, arg2, arg3, arg4, arg5, arg6)

void ReadRegMFE(MS_U32 u32Address, MS_U16 *val);
void WriteRegMFE(MS_U32 u32Address, MS_U16 val, MS_S8 *str1, MS_S32 num, MS_S8 *str3);
#if defined(_MFE_MUJI_) || defined(_MFE_MONET_) || defined(_MFE_MESSI_) || defined(_MFE_MANHATTAN_) || defined(_MFE_MASERATI_) || defined(_MFE_MAXIM_) || defined(_MFE_KANO_) || defined(_MFE_K6_)
void WriteRegMFE_BANK1(MS_U32 u32Address, MS_U16 val, MS_S8 *str1, MS_S32 num, MS_S8 *str3);
#endif
void DumpAllReg(MFE_REG* mfe_reg);

MS_S32 PutFDC(MFE_REG* mfe_reg, void* pContext, MS_S32 mode);
void WriteQTable(MFE_REG* mfe_reg, MS_S32* Table0, MS_S32* Table1);

#if defined(_MFE_M1_)||defined(_MFE_AGATE_)
MS_S32 SetObufAddr(MFE_REG* mfe_reg, MS_U32 pAddr, MS_U32 nBufSize, MS_S32 nObufIdx, MS_S32 bSetAdrFlag);
#else
void SetObufAddr(MFE_REG* mfe_reg, MS_U16 sadr_low, MS_U16 sadr_high, MS_U16 eadr_low, MS_U16 eadr_high);
#endif
void ClearBsfFullIRQ(MFE_REG* mfe_reg);

void ClearIRQ(MFE_REG* mfe_reg, MS_S32 number);
void Enable_HW(MFE_REG* mfe_reg);
void ResetAllRegs(MFE_REG* mfe_reg);

void TestStopAtMb(MFE_REG* mfe_reg);
void TestStop(MFE_REG* mfe_reg);

void IntraUpdateInit(MFE_CONFIG *pConfig);
void IntraUpdateFrame(MFE_CONFIG *pConfig);
void IntraUpdateClose(MFE_CONFIG *pConfig);

// Rate control
void MfeDrvRateControlInit(MFE_CONFIG* pConfig);
void MfeDrvRateControlUpdate(MFE_CONFIG* pConfig, MS_S8 nFieldType);
void MfeDrvRateControlDeInit(MFE_CONFIG* pConfig);
// H263/MPEG4
void mfeM4VE_Init(MFE_CONFIG *pConfig);
void mfeM4VE_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo);
void mfeM4VE_UpdateFrame(MFE_CONFIG *pConfig);
MS_S32 codeM4vConfigHeaders(MFE_CONFIG *pConfig,MS_BOOL IsSkipHeader);

// H264
void mfeH264_Init(MFE_CONFIG *pConfig);
void mfeH264_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo);
void mfeH264_EncodeDummyFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo);
void mfeH264_UpdateFrame(MFE_CONFIG *pConfig);
MS_S32 codeH264ConfigHeaders(MFE_CONFIG *pConfig,MS_U32* size_of_sps,MS_U32* size_of_pps);
void mfeH264_DeInit(MFE_CONFIG* pConfig);
MS_S32 MfeDrvCabacStuffingReport(MFE_CONFIG* pConfig);
// JPE
void mfeJPE_Init(MFE_CONFIG *pConfig);
void mfeJPE_Set_QTABLE(MFE_CONFIG *pConfig);
void mfeJPE_EncodeFrame(MFE_CONFIG *pConfig, GOPINFO* pGopInfo);

//MEM MALLOC/FREE
void* MfeDrvMemMalloc(MS_SIZE size,const MS_S8* msg);
void MfeDrvMemFree(void** mem_ptr,const MS_S8* msg);

#endif // _MFE_COMMON_H_

