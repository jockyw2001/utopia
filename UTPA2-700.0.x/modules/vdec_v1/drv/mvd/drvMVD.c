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
////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2008-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// ("MStar Confidential Information") by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
/// file  drvMVD.c
/// @brief MPEG-2/4 Video Decoder Driver
/// @author MStar Semiconductor Inc.
///
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef REDLION_LINUX_KERNEL_ENVI
#include "drvMVD_Common.h"
#include "drvMVD.h"
#include "regMVD.h"
#include "halMVD.h"
#include "halVPU.h"
#include "drvmvd_cc.h"      //ATSC Closed Caption
#else

// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "halCHIP.h"
#include "drvMMIO.h"
#include "drvMIU.h"
#include "drvBDMA.h"
#include "drvMVD.h"

// Internal Definition
#include "regMVD.h"
#include "halMVD.h"
#include "asmCPU.h"
#include "osalMVD.h"
#include "drvmvd_cc.h"      //ATSC Closed Caption
#endif


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////
//Different level debug message
#define MVD_DEBUGVERBAL(x)    if (_u8DbgLevel>4)  { (x); }
#define MVD_DEBUGINFO(x)      if (_u8DbgLevel>1)  { (x); }
#define MVD_DEBUGERROR(x)     if (_u8DbgLevel>0)  { (x); }
#define MVD_FUNC_ENTRY()      {}//printf(" $$$ drvMVD::%s\n", __FUNCTION__)
#define MVD_DBG_STS(x)        {}
#define _SLQTBL_DUMP_PTS      FALSE//TRUE
#define _SLQTBL_DUMP_PUSHQ    FALSE//TRUE
#define _SLQTBL_DUMP_PKT      FALSE//TRUE
#define _DUMP_FRMINFO         FALSE
#define MVD_TURBO_INIT        FALSE//TRUE
#define MVD_PROFILING         FALSE//TRUE
#define SLQ_NEW_PUSH          TRUE
#define UPDATE_NEXT_WRPTR     FALSE //SlqTbl mode: update (validWrPtr+1) to hw for mvd6
//#define _MVD_V3              //define for sub decoder verification

#define MVD_FBNUM_DEFAULT 4
#define MVD_FBNUM_MAX     5
#define MVD_FBNUM_MIN     MVD_FBNUM_DEFAULT

#define MVD_DHD_FBSIZE           0x5FA000  //Framebuffer size minimum for Dual HD: (1920*2)*1088*1.5
#define MVD_HD_FBSIZE            0x2FD000  //Framebuffer size minimum for High Definition
#define MVD4_5_FBSIZE_HDMIN      0xEF1000  //5 * 1920 * 1088 * 1.5 = 14.95MB
#define MVD4_4_FBSIZE_HDMIN      0xBF4000  //4 * 1920 * 1088 * 1.5 = 11.96MB
#define MVD4_VC1_FBSIZE_SDMIN    0x2F7600  //5 * 720 * 576 * 1.5 = 2.97MB
#define MVD4_MPEG_FBSIZE_HDMIN   0xBF4000  //4 * 1920 * 1088 * 1.5 = 11.96MB
#define MVD4_MPEG_FBSIZE_SDMIN   0x25F800  //4 * 720 * 576 * 1.5 = 2.38MB

#if (defined(CHIP_A1) || defined(CHIP_A5) || defined(CHIP_A2) || defined(CHIP_A7)\
    || defined(CHIP_A3) || defined(CHIP_AMETHYST) || defined(CHIP_EDISON) || defined(CHIP_EMERALD)\
    || defined(CHIP_AGATE) || defined(CHIP_EAGLE) || defined(CHIP_EIFFEL)) || defined(CHIP_EULER)\
    || defined(CHIP_NUGGET)
#define _MVD6
#endif

#define _MS_TO_90K(x)   (x*90)  //ms ==> 90k counter
#define _90K_TO_MS(x)   ((x!=MVD_U32_MAX)?(x/90):(x))  //90k counter ==> ms

#define _IS_VC1(x) ((x==E_MVD_CODEC_VC1_ADV) || (x==E_MVD_CODEC_VC1_MAIN))

#define _MVD_Memset(pDstAddr, u32value, u32Size)                        \
            do {                                                        \
                MS_U32 x = 0;                                           \
                for (x = 0; x < (u32Size/4); x=x+4)                     \
                {                                                       \
                    HAL_MVD_MemWrite4Byte(pDstAddr+x,u32value);         \
                }                                                       \
            } while (0)

#define _MVD_Memcpy(pDstAddr, pSrcAddr, u32Size)                        \
            do {                                                        \
                MS_U32 i = 0;                                           \
                volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
                volatile MS_U8 *src = (volatile MS_U8 *)(pSrcAddr);     \
                for (i = 0; i < (u32Size); i++)                         \
                {                                                       \
                    dst[i] = src[i];                                    \
                }                                                       \
            } while (0)

#define _MVD_INIT_FAIL_RET()                                            \
            do {                                                        \
                if (_bFwLoad)                                           \
                    _bFwLoad = FALSE;                                   \
                HAL_MVD_SetVpuFwDecoder(E_VPU_DECODER_NONE);            \
                return FALSE;                                           \
            } while (0)

#define MVD_U32_MAX                 0xffffffffUL
#define MVD_NULLPKT_PTS             MVD_U32_MAX
#define MAX_ADD_28BIT               0x0fffffff
#define MAX_ADD_27BIT               0x07ffffff

//Initial value for PTS table
#define _INIT_ADDR                  MVD_U32_MAX
#define _INIT_LEN                   0
#define _INIT_TIMESTAMP             MVD_U32_MAX
#define _INIT_ID                    MVD_U32_MAX

#define SLQ_ENTRY_MAX               1024
#define SLQ_ENTRY_LEN               8   //8-byte per entry
#define SLQ_TBL_SIZE                (SLQ_ENTRY_MAX * SLQ_ENTRY_LEN)
#define ES_TBL_SIZE                 (SLQ_ENTRY_MAX * 8) //8-byte per entry

#define MVD_FW_SLQTBL_PTS_LEN       32

#define SLQ_TBL_SAFERANGE              (40*SLQ_ENTRY_LEN)
//this should be smaller than FW's lookup range (current it's 16 entries)

#define SLQTBL_CHECKVACANCY_WATERLEVEL (44*SLQ_ENTRY_LEN)

#define DIVX_PATTERN    0x63643030
#define FLV_PATTERN     0xffff0000
#define MPEG_PATTERN_0  0xC6010000 //this SC just for mpeg2/4
#if SLQ_NEW_PUSH
#define VC1_PATTERN    0x0D010000
#define RCV_PATTERN     0xFF00A55A
#endif
#define VC1_PATTERN_0   0xff010000
#define VC1_PATTERN_1   0x0000ffff
#define VC1_PATTERN_2   0xffffff01

#define RCV_PATTERN_0   0x00000000
#define RCV_PATTERN_1   0xffffffff
#define RCV_PATTERN_2   0x00000000

#define DUMMY_PATTERN   0xBE010000
#define DUMMY_SIZE      0x2000     //8K

#define END_PATTERN_0   0xFF010000
#define END_PATTERN_1   0xDDCCBBAA
#define END_PATTERN_2   0xBBAAFFEE
#define END_PATTERN_3   0xFFEEDDCC
#define END_PATTERN_SIZE 256

#define SKIP_PATTERN_0  0xc5010000
#define SKIP_PATTERN_1  0x270608ab
#define SKIP_PATTERN_SIZE 8

#define CMD_TIMEOUT_MS  500

//Length of internal buffers
#ifdef _MVD6
#define MVD3_FW_IAP_BUF_ALIGN               (0x4000UL)   // 16k
#define MVD3_FW_DP_BUF_ALIGN                (0x8000UL)   // 32k
#define MVD3_FW_MV_BUF_ALIGN                (0x8000UL)   // 32k
#define MVD3_FW_IAP_BUF_LEN                 (0x4000UL)   // 16k
#define MVD3_FW_DP_BUF_LEN                  (0x80000UL)  //512k
#define MVD3_FW_MV_BUF_LEN                  (0x48000UL)  //288K
#else
#define MVD3_FW_IAP_BUF_ALIGN               (0x2000UL)   //  8k
#define MVD3_FW_DP_BUF_ALIGN                (0x08)       //  8  byte
#define MVD3_FW_MV_BUF_ALIGN                (0x0800UL)   //  2k
#define MVD3_FW_IAP_BUF_LEN                 (0x4000UL)   // 16k
#define MVD3_FW_DP_BUF_LEN                  (0x40000UL)  //256k (1920/16) * (1088/16)* 4 * 8 = 255KB
#define MVD3_FW_MV_BUF_LEN                  (0x40000UL)  //256K ((1920/16)*4*1.5+((1920/16)*(1088/16)*4*1.5)*4
#endif
#define MVD3_FW_VOL_INFO_BUF_LEN            (0x1000UL)   //  4K
#define MVD3_FW_FRAME_INFO_BUF_LEN          (0x1000UL)   //  4K
#define MVD3_FW_DIVX_INFO_BUF_LEN           (0x1000UL)   //  4K
#define MVD3_FW_USER_DATA_BUF_LEN           (0x1000UL)   //  4K
#define MVD3_FW_USER_DATA_BUF_BACKUP_LEN    MVD3_FW_USER_DATA_BUF_LEN // 4K
#define MVD3_FW_SLQ_TAB_BUF_LEN             (0x2000UL)   //  8K
#define MVD3_FW_SLQ_TAB_TMPBUF_LEN          (0x200UL)

#define MVD_FW_SLQTBL_PTS_BUF_LEN           (SLQ_ENTRY_MAX*MVD_FW_SLQTBL_PTS_LEN)

#define MVD_FW_DYN_SCALE_BUF_LEN            (0x1000UL)   //  4K
#define MVD_FW_SCALER_INFO_BUF_LEN          (0x100UL)    // 256bytes reserved
#define MVD_FW_DECFRM_INFO_BUF_LEN          (0x100UL)    // 256bytes reserved
#define MVD_FW_USER_DATA_HDR_LEN            (6)
#define MVD_FW_USER_DATA_PKT_LEN            (256)
#define MVD_FW_USER_DATA_EXT_HDR_LEN        (16)

#define PB_ONLY_UNMUTE_VSYNC_COUNT          30

#define MVD_UNMUTE_STC_PTS_GAP              (5500 * 90)

//Default framerate 30fps, i.e. 30 frames or 60 fields per sec.
#define MVD_FRAMERATE_DEFAULT               30000

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

//Table of frame rate code for MPEG-2
static const MS_U16 stFrameRateCode[9]=
{
    NULL,23976,24000,25000,29976,30000,50000,59947,60000
};

typedef struct _MVD_SLQ_TBL_ST
{
    MS_U32 u32StAdd;
    MS_U32 u32EndAdd;
    MS_U32 u32EntryCntMax;

    MS_U32 u32RdPtr;
    MS_U32 u32WrPtr;
    MS_U32 u32Empty;
} MVD_SLQ_TBL_ST;

typedef struct _MVD_SLQ_ES_ST
{
    MS_U32 u32StAdd;
    MS_U32 u32EndAdd;

    MS_U32 u32RdPtr;
    MS_U32 u32WrPtr;
} MVD_SLQ_ES_ST;

typedef struct _MVD_CMD_QUEUE
{
    MS_U32 u32PtsBase;
} MVD_CMD_QUEUE;
#if SLQ_NEW_PUSH
typedef struct _MVD_SLQ_STATUS
{
    MS_BOOL bSlqPicStart;
    MS_BOOL bSlqPicCollect;
    MS_BOOL bSlqPicWaitNextStart;
    MS_BOOL bSlqFireRdy;
    MS_BOOL bSlqCtrlBit;
    MS_BOOL bSlqEnLastFrameShow;
    MS_U32 u32SlqPatternAddr;
    MS_U32 u32SlqPushLength;
    MS_U32 u32VaildWptrAddr;
} MVD_SLQ_STATUS;
#endif
////////////////////////////////////////////////////////////////////////////////
// Local Global Variables
////////////////////////////////////////////////////////////////////////////////
/// Version string
static MSIF_Version _drv_mvd_version = {
    .DDI = { MVD_DRV_VERSION, },
};

static MS_U32 pu8MVDSetHeaderBufStart=NULL;
static MS_U32 pu8MVDGetVolBufStart=NULL;
static MS_U32 pu8MVDGetFrameInfoBufStart=NULL;
static MS_U32 u32MVDFWSLQTABTmpbufAdr = NULL;
static MS_U32 u32MVDFWPtsTblAddr = NULL;
static MS_U32 u32DecFrmInfoAdd = NULL;
static MS_U32 u32DynScalingAdd = NULL;
static MS_U32 u32ScalerInfoAdd = NULL;
static MS_U8  u8DynScalingDepth = 0;
static MS_U32 u32VolAdd;
static MS_U32 u32ESBuffEnd = 0; //miu offset of ES buffer end
static MS_U32 u32UsrData = NULL;
static MS_U32 u32UsrDataRd = 0;
static MS_U32 u32UsrDataWr = 0;

static FW_DIVX_INFO gdivxInfo;
#if (!defined(CHIP_T7))
static FW_VOL_INFO gvolInfo;
#endif
static MS_BOOL _bDecodeIFrame = FALSE;
static MS_BOOL _bDrvInit = FALSE;
MS_BOOL bStopped = TRUE;
static MS_BOOL _bFwLoad = FALSE;

static MS_U8   u8FBMode;
static MS_U8   u8FBNum = 0;
static MS_U32  u32FbHdSize = 0;
static MS_U32  u32FbSdSize = 0;

static MVD_TrickDec eTrickMode = E_MVD_TRICK_DEC_UNKNOWN;
static MS_BOOL bAVSyncOn = FALSE;

static MS_U8 _u8DbgLevel = 0;
static MVD_DrvInfo _stDrvInfo = {
                                    1,      //1 MVD HW
                                    0,      //Device#   fixme
                                    MVD_FW_VERSION,        //firmware version
                                    { FALSE, FALSE, FALSE } //capability
                                };

static MVD_FrameInfo stPreFrmInfo;
static MVD_CodecType curCodecType;
static MVD_SrcMode curSrcMode;
static MS_U8 curDisablePESParsing;
#if SLQ_NEW_PUSH
static MVD_SLQ_STATUS _SlqStatus;
#define SLQ_PIC_START_FLAG 0x40000000
#endif
MVD_MEMCfg stMemCfg;
MVD_FWCfg stFwCfg;
static MS_BOOL bStepDecode = FALSE;
#if (defined(CHIP_T2))||(defined(CHIP_T7))
static MS_U32 u32StepDecodeCnt = 0;
#endif
static MS_BOOL bStepDisp = FALSE;
static MS_BOOL bStep2Pts = FALSE;
static MS_BOOL bSkip2Pts = FALSE;
static MS_BOOL bDropErrFrm = FALSE;
static MS_BOOL bDropDispfrm = FALSE;
static MVD_FrcMode _eFrcMode = E_MVD_FRC_NORMAL;
static MS_BOOL bEnableLastFrmShow = FALSE;
static MS_U32 u32LastPts = MVD_NULLPKT_PTS;
static MS_U32 u32DummyPktCnt = 0;
static MS_U32 u32SlqByteCnt = 0;
static MS_BOOL bSlqTblSync = FALSE; //need to BDMA SLQ table from DrvProcBuff to BitstreamBuff
static MS_BOOL bSlqTblHasValidData = FALSE;
static MS_BOOL bSlqTblHKCtrl = FALSE;   //read/write pointer can be accessed by HK directly
static MS_U32 u32FileEndPtr;

#ifndef REDLION_LINUX_KERNEL_ENVI
static BDMA_CpyType bdmaCpyType = E_BDMA_CPYTYPE_MAX;
#endif

static MVD_SLQ_TBL_ST _drvSlqTbl;
static MS_U32 u32PreEsRd = MVD_U32_MAX;
static MS_U32 u32PreEsWr = 0;

static MVD_SLQ_ES_ST _drvEsTbl;
static MVD_SLQ_ES_ST _drvDivxTbl;

static MVD_CMD_QUEUE _mvdCmdQ;

static MVD_TIMESTAMP_TYPE _eFileSyncMode;
static MVD_AVSyncCfg stSyncCfg;
static MVD_SpeedType ePreSpeedType = E_MVD_SPEED_DEFAULT;
#if defined(CHIP_T7)
static MS_U8 u8MvdPlayMode = 0;
#endif

#ifdef MVD_ENABLE_ISR
MVD_InterruptCb pfnCallback;
static MS_U32 eEventFlag;
static MS_U32 eCurEvent;
#endif

static MVD_InternalMemCfg _stInternalMemCfg;

static MS_U32 u32SyncDoneCnt = 0;
static MS_U32 u32VSyncCnt = 0;
static MS_BOOL bIsrAttached = FALSE;
#define MST_MODE_TRUE   1
#define MST_MODE_FALSE  0
static MS_U8 u8MstMode = MST_MODE_FALSE; //MStreamer mode=1; Non-MSt mode=0.
#define MDrv_MVD_IsMStreamerMode()  (MST_MODE_TRUE == u8MstMode)
#ifdef _MVD_V3
static MS_BOOL bRdyToFireCmd = FALSE;
#endif
static MS_U32 u32HWBuffTotalSize = 0;

//MVD_HWCfg* _pMvdHWCfg;
////////////////////////////////////////////////////////////////////////////////
// Local functions
////////////////////////////////////////////////////////////////////////////////
static MS_BOOL MVD_MVDSetInternalBuffAddr(MS_U32 u32start, MS_U32 u32len);
static void MVD_InitVar(void);
static MS_BOOL MVD_Init(void);
static MS_U8 MVD_MapCodecType(MVD_CodecType type);
static MS_U8 MVD_MapSrcMode(MVD_SrcMode mode);
static void MVD_WriteDivx311Data(FW_DIVX_INFO *divxInfo);
static MVD_ErrStatus MVD_GetErrShapeStat(MS_U32 u32errStat);
static MVD_ErrStatus MVD_GetErrSpriteStat(MS_U32 u32errStat);
static MS_U8 MVD_MapFrcMode(MVD_FrcMode eFrcMode);
static E_MVD_Result MVD_RstFrmInfo(MVD_FrmInfoType eType);

static MS_BOOL MVD_SLQTblSendPacket(MVD_PacketInfo *pstVideoPKT);
#if SLQ_NEW_PUSH
static void MVD_SLQTblGetHdrPkt(MVD_PacketInfo* pHdr, MVD_PacketInfo* pData);
#else
static void MVD_SLQTblGetDivxHdrPkt(MVD_PacketInfo* pDivxHdr, MVD_PacketInfo* pDivxData);
#endif
static MS_BOOL MDrv_MVD_Stop(void);
static void MVD_SLQTblInit(void);
//static void _SLQTbl_DumpInfo(MVD_SLQ_TBL_ST* pInfo);
static MS_U32 MVD_GetMemOffset(MS_PHYADDR u32PhyAdd);
static MS_U32 MVD_Map2DrvSlqTbl(MS_U32 u32HWPtr);
static void MVD_SLQTblGetDummyPkt(MVD_PacketInfo* pDummy);
static void MVD_SLQTblGetFileEndPkt(MVD_PacketInfo* pFileEnd);

static MS_BOOL MDrv_MVD_ClearCmdFinished(MVD_HANDSHAKE_CMD eCmd);

static MS_BOOL MVD_SLQTblInsertPattern(MVD_PatternType ePattern);
#if _SLQTBL_DUMP_PTS
static void _SLQTbl_DumpPtsTbl(MS_U32 u32EntryStart, MS_U32 u32EntryEnd);
#endif

#if _DUMP_FRMINFO
static void MVD_DumpFrmInfo(MVD_FrmInfo* pInfo)
{
    if (NULL == pInfo)
    {
        printf("%s: pInfo invalid!\n", __FUNCTION__);
        return;
    }

    printf("u32LumaAddr  =0x%lx\n", pInfo->u32LumaAddr  );
    printf("u32ChromaAddr=0x%lx\n", pInfo->u32ChromaAddr);
    printf("u32TimeStamp =0x%lx\n", pInfo->u32TimeStamp );
    printf("u32ID_L      =0x%lx\n", pInfo->u32ID_L      );
    printf("u32ID_H      =0x%lx\n", pInfo->u32ID_H      );
    printf("u16Pitch     =0x%x\n", pInfo->u16Pitch      );
    printf("u16Width     =0x%x\n", pInfo->u16Width      );
    printf("u16Height    =0x%x\n", pInfo->u16Height     );
    printf("eFrmType     =0x%x\n", pInfo->eFrmType      );
    return;
}
#endif


//------------------------------------------------------------------------------
/// Get MVD driver information
/// @return -the pointer to the driver information
//------------------------------------------------------------------------------
const MVD_DrvInfo* MDrv_MVD_GetInfo(void)
{
    MDrv_MVD_GetCaps(&_stDrvInfo.stCaps);
    return (&_stDrvInfo);
}


//------------------------------------------------------------------------------
/// Get MVD driver version
/// @return -the pointer to the driver version
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
        return E_MVD_RET_FAIL;

    *ppVersion = &_drv_mvd_version;
    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Get MVD driver status
/// @return -the pointer to the driver status
//------------------------------------------------------------------------------
void MDrv_MVD_GetStatus(MVD_DrvStatus* pDrvStatus)
{
    if (!pDrvStatus)
       return;

    pDrvStatus->u32FWVer = MDrv_MVD_GetFWVer();
    if (pDrvStatus->u32FWVer == 0) //timeout to get FW version
    {
        pDrvStatus->bIsBusy = TRUE;
    }
    else
    {
        pDrvStatus->bIsBusy = FALSE;
    }
    pDrvStatus->eDecStat = MDrv_MVD_GetDecodeStatus();
    pDrvStatus->u8LastFWCmd = MDrv_MVD_GetLastCmd();
    return;
}


//------------------------------------------------------------------------------
/// Set detailed level of MVD driver debug message
/// 0: None, 1: MVD_DEBUGERROR, 2: MVD_DEBUGINFO
/// @param level  \b IN  level from 0 to 2
//------------------------------------------------------------------------------
void MDrv_MVD_SetDbgLevel(MS_U8 level)
{
    _u8DbgLevel = level;
    if (level >= 2)
    {
        HAL_MVD_SetDbgLevel(level-1);
    }
    return;
}


//------------------------------------------------------------------------------
/// Get MVD firmware version
/// @return -firmware version
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetFWVer(void)
{
    return HAL_MVD_GetFWVer();
}

//------------------------------------------------------------------------------
/// Set SLQ table buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void MDrv_MVD_SetSLQTblBufStartEnd(MS_U32 u32start, MS_U32 u32end)
{
    MVD_FUNC_ENTRY();
    MVD_DEBUGINFO(printf("%s st=0x%lx end=0x%lx\n", __FUNCTION__, u32start, u32end));
    HAL_MVD_SetSLQTblBufStartEnd(u32start, u32end);
    return;
}

//------------------------------------------------------------------------------
/// Set HW buffers' start address to MVD FW
/// Return (the end address - 1)
/// @param -u32Addr \b IN : start address (MIU offset)
//------------------------------------------------------------------------------
static MS_U32 MVD_SetHWBuffer(MS_U32 u32Add)
{
    MS_U32 tmpAdr;

    MVD_DEBUGINFO(printf("====> %s u32Add = 0x%lx\n", __FUNCTION__, u32Add));
    tmpAdr = MemAlign(u32Add, MVD3_FW_IAP_BUF_ALIGN);
    MVD_DEBUGINFO(printf("set MVD3_FW_IAP_BUF_ADR =%lx\n",tmpAdr));
    HAL_MVD_SetIAPBufferAddr(tmpAdr);
    tmpAdr += MVD3_FW_IAP_BUF_LEN;

    tmpAdr = MemAlign(tmpAdr, MVD3_FW_DP_BUF_ALIGN);
    MVD_DEBUGINFO(printf("set MVD3_FW_DP_BUF_ADR=%lx\n",tmpAdr));
    HAL_MVD_SetDPBufferAddr(tmpAdr);
    tmpAdr += MVD3_FW_DP_BUF_LEN;

    tmpAdr = MemAlign(tmpAdr, MVD3_FW_MV_BUF_ALIGN);
    MVD_DEBUGINFO(printf("set MVD3_FW_MV_BUF_ADR=%lx\n",tmpAdr));
    HAL_MVD_SetMVBufferAddr(tmpAdr);
    tmpAdr += MVD3_FW_MV_BUF_LEN;
    MVD_DEBUGINFO(printf("====> %s End of HW buffers = 0x%lx\n", __FUNCTION__, tmpAdr));

    return tmpAdr;
}
#if defined(CHIP_K2)

//------------------------------------------------------------------------------
/// Set HW buffers' offset size to MVD FW
/// @param -u32Addr \b IN : start address (MIU offset)
//------------------------------------------------------------------------------
static void MVD_SetHWBufferOffset(void)
{

#define MIU_256MB 0x10000000
#define MIU_512MB 0x20000000
    MS_BOOL bBSHighOffset=FALSE,bFBHighOffset=FALSE;


    MVD_DEBUGINFO(printf("MIU Select %d  %d\n",stMemCfg.bHWMiuSel,stMemCfg.bFWMiuSel));
    MVD_DEBUGINFO(printf("MIU Base %lx \n",stMemCfg.u32Miu1BaseAddr));
    if((stMemCfg.u32FBAddr>=(MIU_512MB + stMemCfg.u32Miu1BaseAddr)) || (stMemCfg.u32BSAddr>=(MIU_512MB + stMemCfg.u32Miu1BaseAddr)))
    {
        MVD_DEBUGERROR(printf("Unsupported buffer Addr!! (0x%lx,0x%lx)\r\n",stMemCfg.u32BSAddr,stMemCfg.u32FBAddr));
        return;
    }
    if(stMemCfg.u32FBAddr >= stMemCfg.u32Miu1BaseAddr)
    {
        if(stMemCfg.u32FBAddr >= (stMemCfg.u32Miu1BaseAddr+MIU_256MB))
        {
            bFBHighOffset = TRUE;
    }
    }
    else
    {
        if(stMemCfg.u32FBAddr>=MIU_256MB)
        {
            bFBHighOffset=TRUE;
        }
    }


    if(stMemCfg.u32BSAddr >= stMemCfg.u32Miu1BaseAddr)
    {
        if(stMemCfg.u32BSAddr >= (stMemCfg.u32Miu1BaseAddr+MIU_256MB))
        {
            bBSHighOffset = TRUE;
        }
    }
    else
    {
        if(stMemCfg.u32BSAddr >= MIU_256MB)
        {
            bBSHighOffset = TRUE;
        }
    }

    HAL_MVD_SetBufferOffset(bBSHighOffset,bFBHighOffset);
#undef MIU_256MB
#undef MIU_512MB

    return;
}
#endif

//------------------------------------------------------------------------------
/// Set the number of VC1 framebuffer
/// @param -u8FrmNum \b IN : the number of framebuffer
//------------------------------------------------------------------------------
static void MVD_SetFrameBuffNum(MS_U8 u8FrmNum)
{
    MVD_CmdArg mvdcmd;

    MVD_DEBUGINFO(printf("%s u8FrmNum = %d\n", __FUNCTION__, u8FrmNum));
    if (u8FrmNum < MVD_FBNUM_MIN)
    {
        MVD_DEBUGINFO(printf("%s set u8FrmNum as %d instead of %d!\n",
            __FUNCTION__, MVD_FBNUM_MIN, u8FrmNum));
        u8FrmNum = MVD_FBNUM_MIN;
    }
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8FrmNum;
    if (HAL_MVD_MVDCommand( CMD_FB_NUM, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FB_NUM ) );
        return;
    }
    return;
}



//------------------------------------------------------------------------------
/// Set internal buffer address to MVD
/// @param -u32addr \b IN : start address
/// @param -u32len  \b IN : length
//------------------------------------------------------------------------------
static MS_BOOL MVD_MVDSetInternalBuffAddr(MS_U32 u32start, MS_U32 u32len)
{
#if(defined(CHIP_T7))//fix me later
      return __MVDSetInternalBuffAddr(u32start,u32len);
#else
    MS_U32 tmpAdr, tmpLen;
    MS_U32 i;
    MS_BOOL bRet = TRUE;

    tmpAdr = u32start;
    tmpAdr += MVD_FW_CODE_LEN;

    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align
    MVD_DEBUGINFO(printf("set MVD3_FW_VOL_INFO_BUF_ADR 2=%lx\n",tmpAdr));
    HAL_MVD_SetVolInfoBufferAddr(tmpAdr);

    pu8MVDGetVolBufStart = tmpAdr;
    tmpAdr += MVD3_FW_VOL_INFO_BUF_LEN;
    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align

    for (i=0; i<MVD3_FW_VOL_INFO_BUF_LEN; i+=4)
    {
        HAL_MVD_MemWrite4Byte(pu8MVDGetVolBufStart+i, 0x00UL);
    }


    u32VolAdd = pu8MVDGetVolBufStart;
    if (stMemCfg.bFWMiuSel == MIU_SEL_1)
    {
        u32VolAdd += stMemCfg.u32Miu1BaseAddr;
    }
    u32VolAdd = HAL_MVD_PA2NonCacheSeg(u32VolAdd);
    MVD_DEBUGVERBAL(printf("gvolInfo = 0x%lx, volBuf=0x%lx\n", u32VolAdd, pu8MVDGetVolBufStart));


    MVD_DEBUGINFO(printf("set MVD3_FW_FRAME_INFO_BUF_ADR 3=%lx\n",tmpAdr));
    HAL_MVD_SetFrameInfoBufferAddr(tmpAdr);

    pu8MVDGetFrameInfoBufStart = tmpAdr;
    tmpAdr += MVD3_FW_FRAME_INFO_BUF_LEN;
    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align

    for (i=0; i<MVD3_FW_FRAME_INFO_BUF_LEN; i+=4)
    {
        HAL_MVD_MemWrite4Byte(pu8MVDGetFrameInfoBufStart+i, 0x00UL);
    }

    MVD_DEBUGINFO(printf("set MVD3_FW_DIVX_INFO_BUF_ADR=%lx\n",tmpAdr));
    HAL_MVD_SetHeaderBufferAddr(tmpAdr);

    pu8MVDSetHeaderBufStart = tmpAdr;
    tmpAdr += MVD3_FW_DIVX_INFO_BUF_LEN;
    //set user data
    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align

    MVD_DEBUGINFO(printf("set MVD3_FW_USER_DATA_BUF_ADR 4 =%lx\n",tmpAdr));
    HAL_MVD_SetUserDataBuf(tmpAdr, MVD3_FW_USER_DATA_BUF_LEN);
    _stInternalMemCfg.u32UserDataBuf = tmpAdr;
    u32UsrDataRd = _stInternalMemCfg.u32UserDataBuf;
    u32UsrDataWr = _stInternalMemCfg.u32UserDataBuf;

    tmpAdr += MVD3_FW_USER_DATA_BUF_LEN;

    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align
    MVD_DEBUGINFO(printf("set MVD3_FW_USER_DATA_BUF2 =%lx\n",tmpAdr));
    //Used as CC decoding buffer for MVD_SUPPORT_X4_CC
    tmpAdr += MVD3_FW_USER_DATA_BUF_BACKUP_LEN;

    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align
#ifndef _MVD_V3
    tmpAdr += 0x10000;  //sync to MM
    tmpAdr += MVD3_FW_SLQ_TAB_BUF_LEN;

    tmpAdr = (MemAlign(tmpAdr, 0x1000));//4k align
    MVD_DEBUGINFO(printf("set MVD3_FW_SLQ_TAB_TMPBUF_ADR=%lx\n",tmpAdr));
    tmpAdr += MVD3_FW_SLQ_TAB_BUF_LEN;  //sync to MM
#endif
    u32MVDFWSLQTABTmpbufAdr = tmpAdr;

    MVD_DEBUGINFO(printf("set u32MVDFWSLQTABTmpbufAdr=%lx\n",tmpAdr));

    if ((u32start + u32len) < tmpAdr)
    {
        MS_ASSERT(0);
        //return FALSE;
    }

    if((curSrcMode != E_MVD_TS_FILE_MODE)
    && (curSrcMode != E_MVD_TS_MODE))
    {
        HAL_MVD_MemWrite4Byte(tmpAdr, 0xBE010000UL);
        tmpAdr+=4;
        HAL_MVD_MemWrite4Byte(tmpAdr, 0x000000FAUL);
        tmpAdr+=4;
        for (i=8; i<MVD3_FW_SLQ_TAB_TMPBUF_LEN; i+=4)
        {
            HAL_MVD_MemWrite4Byte(tmpAdr, 0x00UL);
            tmpAdr+=4;
        }

        u32MVDFWPtsTblAddr = tmpAdr;
        HAL_MVD_SetPtsTblAddr(u32MVDFWPtsTblAddr);
        for (i=0; i<MVD_FW_SLQTBL_PTS_BUF_LEN; i+=MVD_FW_SLQTBL_PTS_LEN)
        {
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i, 0);     //byteCnt
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+4, 0);   //dummyPktCnt
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+8, 0);   //idLow
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+12, 0);  //idHigh

            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+16, MVD_NULLPKT_PTS);  //PTS
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+20, 0);  //reserved0
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+24, 0);  //reserved1
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+i+28, 0);  //reserved2
        }

        MS_ASSERT((u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_BUF_LEN)<=(u32start+u32len));
        MVD_DEBUGINFO(printf("PTS tbl start=%lx end=%lx\n",
               u32MVDFWPtsTblAddr, (u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_BUF_LEN)));
        tmpAdr += MVD_FW_SLQTBL_PTS_BUF_LEN;
    }
    else
    {
        tmpAdr += (MVD3_FW_SLQ_TAB_TMPBUF_LEN + MVD_FW_SLQTBL_PTS_BUF_LEN);
    }

    u32DynScalingAdd = tmpAdr;
    if (stMemCfg.bEnableDynScale)
    {
        HAL_MVD_SetDynamicScaleAddr(tmpAdr);
        HAL_MVD_EnableDynamicScale();
        u8DynScalingDepth = 16; //HAL_MVD_GetDynamicScaleDepth
        MVD_DEBUGINFO(printf("bEnableMIUSel    = 0x%x\n", stMemCfg.bFWMiuSel));
        MVD_DEBUGINFO(printf("u32DynScalingAddr= 0x%lx\n", u32DynScalingAdd));
        MVD_DEBUGINFO(printf("u8DynScalingDepth= 0x%x\n", u8DynScalingDepth));
    }
    MVD_DEBUGINFO(printf("DynScaling start=%lx end=%lx\n",
           u32DynScalingAdd, (u32DynScalingAdd+MVD_FW_DYN_SCALE_BUF_LEN)));

    tmpAdr += MVD_FW_DYN_SCALE_BUF_LEN;

    u32ScalerInfoAdd = tmpAdr;
    MVD_DEBUGINFO(printf("ScalerInfo start=%lx end=%lx\n",
           u32ScalerInfoAdd, (u32ScalerInfoAdd+MVD_FW_SCALER_INFO_BUF_LEN)));
    tmpAdr += MVD_FW_SCALER_INFO_BUF_LEN;

    u32DecFrmInfoAdd = tmpAdr;
    HAL_MVD_SetDecFrmInfoAddr(tmpAdr);
    MVD_RstFrmInfo(E_MVD_FRMINFO_DECODE);
    MVD_RstFrmInfo(E_MVD_FRMINFO_DISPLAY);
    MVD_DEBUGINFO(printf("DecFrmInfo start=%lx\n", u32DecFrmInfoAdd));
    tmpAdr += MVD_FW_DECFRM_INFO_BUF_LEN;

#ifdef _MVD_V3
    if (tmpAdr > MVD_FW_MPOOL_START_OFFSET)
    {
        //shared memory should not overlap with FW memory pool.
        MVD_DEBUGERROR(printf("%s err: out of memory boundary!\n", __FUNCTION__));
        return FALSE;
    }
#endif

#if (!defined(_MVD_V3) && !defined(_MVD6))
    if (stMemCfg.bFWMiuSel == stMemCfg.bHWMiuSel)
    {
        tmpAdr = MVD_SetHWBuffer(tmpAdr);
    }
#endif

#if defined(CHIP_K2)
    MVD_SetHWBufferOffset();
#endif
    HAL_MVD_MemGetMap(E_MVD_MMAP_FB, &tmpAdr, &tmpLen);
    MVD_DEBUGINFO(printf("set MVD_FRAMEBUFFER_ADR=%lx\n",tmpAdr));
    MDrv_MVD_SetFrameBuffAddr(tmpAdr);



    MVD_SetFrameBuffNum(u8FBNum);

//  If VD_MHEG5(CPU) and MVD HW engine are run on different MIU,
// IAP, DP, and MV buffers are allocated after FB.
// The reason is that these 3 buffers are used by MVD HW engine.
#if (!defined(_MVD_V3) && !defined(_MVD6))
    if (stMemCfg.bFWMiuSel != stMemCfg.bHWMiuSel)
#endif
    {
        if ((u8FBMode == MVD3_HD_MODE) || (u8FBMode == MVD3_DHD_MODE))
        {
            tmpAdr += u32FbHdSize;
            if (stMemCfg.u32FBSize < (u32FbHdSize + MVD3_FW_IAP_BUF_LEN + MVD3_FW_DP_BUF_LEN + MVD3_FW_DP_BUF_LEN))
            {
                MVD_DEBUGERROR(printf("MVD HW buffers larger than FB size!!!\n"));
            }
        }
        else if (MVD3_SD_MODE == u8FBMode)
        {
            tmpAdr += u32FbSdSize;
        }
        else
        {
            MVD_DEBUGERROR(printf("%s invalid u8FBMode(0x%x)!\n", __FUNCTION__, u8FBMode));
            return FALSE;
        }
        MVD_DEBUGINFO(printf("MVD FB boundary =0x%lx\n",tmpAdr));
        tmpAdr = MVD_SetHWBuffer(tmpAdr);
    }


    HAL_MVD_MemGetMap(E_MVD_MMAP_BS, &tmpAdr, &tmpLen);
    MVD_DEBUGINFO(printf("set MVD_BITSTREAM_ADR=%lx\n",tmpAdr));
    bRet = MDrv_MVD_SetBitStreamAddr(tmpAdr,tmpAdr+tmpLen);
    u32ESBuffEnd = tmpAdr+tmpLen;
//Set Compression mode
#if !(defined(CHIP_MARIA10) || defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER))
    if((stFwCfg.stFBReduction.LumaFBReductionMode != E_MVD_FB_REDUCTION_NONE)
    || (stFwCfg.stFBReduction.ChromaFBReductionMode  != E_MVD_FB_REDUCTION_NONE))
    {
        MVD_DEBUGERROR(printf("MVD Err: Not support FB reduction mode!!\n"));
        ////return FALSE;
    }
#else
   if((stFwCfg.stFBReduction.LumaFBReductionMode == E_MVD_FB_REDUCTION_1_4)
    || (stFwCfg.stFBReduction.ChromaFBReductionMode  == E_MVD_FB_REDUCTION_1_4))
    {
        MVD_DEBUGERROR(printf("MVD Err: Not support FB reduction 1/4 mode!!\n"));
        return FALSE;
    }
    MS_U8 u8CompressionMode = ((MS_U8)stFwCfg.stFBReduction.LumaFBReductionMode)||
        (((MS_U8)stFwCfg.stFBReduction.ChromaFBReductionMode)<<1);

    HAL_MVD_SetCompressionMode(u8CompressionMode ,stFwCfg.u16FBReduceValue, stFwCfg.stFBReduction.u8EnableAutoMode);

    if (stFwCfg.stFBReduction.u8EnableAutoMode == 1)
    {
        if ((curCodecType == E_MVD_CODEC_VC1_ADV) || (curCodecType == E_MVD_CODEC_VC1_MAIN))
        {
            MVD_CmdArg mvdcmd;

            SETUP_CMDARG(mvdcmd);
            printf("HAL_MVD_MVDCommand(CMD_FIXED_FRAME_BUFFER, 0)\n");
            mvdcmd.Arg0 = 0;
            if (HAL_MVD_MVDCommand( CMD_FIXED_FRAME_BUFFER, &mvdcmd ) == FALSE)
            {
                printf( "Command: 0x%x fail!!\r\n", CMD_FIXED_FRAME_BUFFER );
                MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FIXED_FRAME_BUFFER ) );
                return FALSE;
            }
        }
    }
#endif


    MVD_DEBUGINFO(printf("set pu8MVDGetVolBufStart=%lx\n",pu8MVDGetVolBufStart));
    MVD_DEBUGINFO(printf("set pu8MVDGetFrameInfoBufStart=%lx\n",pu8MVDGetFrameInfoBufStart));
    MVD_DEBUGINFO(printf("set pu8MVDSetHeaderBufStart=%lx\n",pu8MVDSetHeaderBufStart));

    return bRet;
#endif
}


//------------------------------------------------------------------------------
/// Initialize variables for MVD driver
//------------------------------------------------------------------------------
static void MVD_InitVar(void)
{
    pu8MVDSetHeaderBufStart=0;
    pu8MVDGetVolBufStart=0;
    pu8MVDGetFrameInfoBufStart=0;
    u32ScalerInfoAdd = NULL;
    bAVSyncOn = FALSE;
    bStepDecode = FALSE;
#if (defined(CHIP_T2))||(defined(CHIP_T7))
    u32StepDecodeCnt = 0;
#endif
    bStepDisp = FALSE;
    bStep2Pts = FALSE;
    bDropErrFrm = FALSE;
    bDropDispfrm = FALSE;
    _eFrcMode = E_MVD_FRC_NORMAL;

    bEnableLastFrmShow = FALSE;
    u32LastPts = MVD_NULLPKT_PTS;
    eTrickMode = E_MVD_TRICK_DEC_UNKNOWN;
    u32PreEsRd = MVD_U32_MAX;
    u32PreEsWr = 0;
    u32ESBuffEnd = 0;

    stPreFrmInfo.u16HorSize   = 0;
    stPreFrmInfo.u16VerSize   = 0;
    stPreFrmInfo.u8AspectRate = 0;
    stPreFrmInfo.u32FrameRate = 0;
    stPreFrmInfo.u8Interlace  = 0;

    _eFileSyncMode = E_MVD_TIMESTAMP_FREERUN;
    ePreSpeedType = E_MVD_SPEED_DEFAULT;

    stSyncCfg.bEnable = FALSE;
    stSyncCfg.u32Delay = 0;
    stSyncCfg.u16Tolerance = 0;
#ifdef _MVD_V3
    bRdyToFireCmd = FALSE;
#endif
    #if SLQ_NEW_PUSH
    _SlqStatus.u32SlqPatternAddr = 0;
    _SlqStatus.u32SlqPushLength = 0;
    _SlqStatus.bSlqPicStart = FALSE;
    _SlqStatus.bSlqPicCollect = FALSE;
    _SlqStatus.bSlqPicWaitNextStart = FALSE;
    _SlqStatus.bSlqEnLastFrameShow =FALSE;
    _SlqStatus.bSlqFireRdy = FALSE;
    _SlqStatus.bSlqCtrlBit =FALSE;
    #endif
#if(defined(CHIP_T7))
    u8MvdPlayMode = 0;
#endif

    u32SyncDoneCnt = 0;
    u32VSyncCnt = 0;

#if !defined(SEC_X4)
    bIsrAttached = FALSE;
#endif

    return;
}

#if 0
static void MVD_DumpMemCfg(MVD_MEMCfg* pCfg)
{
    if (pCfg)
    {
        printf("u32FWBinAddr          = 0x%lx\n", pCfg->u32FWBinAddr);
        printf("u32FWBinSize          = 0x%lx\n", pCfg->u32FWBinSize);
        printf("u32FWCodeAddr         = 0x%lx\n", pCfg->u32FWCodeAddr);
        printf("u32FWCodeSize         = 0x%lx\n", pCfg->u32FWCodeSize);
        printf("u32FBAddr             = 0x%lx\n", pCfg->u32FBAddr);
        printf("u32FBSize             = 0x%lx\n", pCfg->u32FBSize);
        printf("u32BSAddr             = 0x%lx\n", pCfg->u32BSAddr);
        printf("u32BSSize             = 0x%lx\n", pCfg->u32BSSize);
        printf("u32DrvBufAddr         = 0x%lx\n", pCfg->u32DrvBufAddr);
        printf("u32DrvBufSize         = 0x%lx\n", pCfg->u32DrvBufSize);
        printf("u32DynSacalingBufAddr = 0x%lx\n", pCfg->u32DynSacalingBufAddr);
        printf("u32DynSacalingBufSize = 0x%lx\n", pCfg->u32DynSacalingBufSize);
        printf("u32Miu1BaseAddr       = 0x%lx\n", pCfg->u32Miu1BaseAddr);
        printf("bFWMiuSel             = 0x%x\n", pCfg->bFWMiuSel);
        printf("bHWMiuSel             = 0x%x\n", pCfg->bHWMiuSel);
    }
}
#endif

static MS_BOOL MVD_CheckFrmBuffSizeMin(MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg)
{
    MS_BOOL ret = TRUE;

    //printf("[DBG] MVD_CheckFrmBuffSizeMin\n");
#if (defined(CHIP_MARIA10) || defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER))
    //printf("[DBG] MVD_CheckFrmBuffSizeMin force return TRUE\n");
    return ret; //  u8FBMode : MVD3_SD_MODE; u8FBNum : MVD_FBNUM_MIN;
#else
    if (_IS_VC1(fwCfg->eCodecType))
    {
        MVD_DEBUGERROR(printf("Framebuffer size(0x%lx) < (0x%x+0x%lx)!\n",
            memCfg->u32FBSize, (MVD_HD_FBSIZE*MVD_FBNUM_MIN), u32HWBuffTotalSize));
        ret = FALSE;
    }
    else if (memCfg->u32FBSize < (MVD4_MPEG_FBSIZE_SDMIN+u32HWBuffTotalSize))
    {
        MVD_DEBUGERROR(printf("Framebuffer size(0x%lx) < (0x%x+0x%lx)\n",
            memCfg->u32FBSize, MVD4_MPEG_FBSIZE_SDMIN, u32HWBuffTotalSize));
        ret = FALSE;
    }
    return ret;
#endif
}

//------------------------------------------------------------------------------
/// Determine u8FBMode & u8FBNum according to the assigned FBSize
//------------------------------------------------------------------------------
static MS_BOOL MVD_CheckFrmBuffSize(MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg)
{
    MS_BOOL ret = TRUE;
    u8FBMode = 0xff;

#if defined(MVD_SUPPORT_SD_ONLY)
    u8FBMode = MVD3_SD_MODE;
#else
    if(memCfg->bSupportSDModeOnly)
    {
        u8FBMode = MVD3_SD_MODE;
    }
#endif //MVD_SUPPORT_SD_ONLY

#if (defined(_MVD_V3) || defined(_MVD6))
    //Three HW buffers are allocated after framebuffer for mvd6, so we need to
    //consider these buffers when checking the available framebuffer size/number.
    u32HWBuffTotalSize = MVD3_FW_IAP_BUF_LEN + MVD3_FW_DP_BUF_LEN + MVD3_FW_MV_BUF_LEN;
#endif

    //For SD only cases: defined(MVD_SUPPORT_SD_ONLY) and bSupportSDModeOnly
    if (MVD3_SD_MODE == u8FBMode)
    {
        ret = MVD_CheckFrmBuffSizeMin(fwCfg, memCfg);

        //set frmBuffNum as 4
        u8FBNum = MVD_FBNUM_MIN;
        u32FbSdSize = MVD4_MPEG_FBSIZE_SDMIN;
        printf("[MVD_SD_MODE] u8FBNum=%d, FBSize=0x%lx\n", u8FBNum, memCfg->u32FBSize);
        return ret;
    }

    MS_U32 u32AvailFrmBuffSize = memCfg->u32FBSize - u32HWBuffTotalSize;
    if ((MVD_SUPPORT_DHD) && (u32AvailFrmBuffSize >= (MVD_DHD_FBSIZE*MVD_FBNUM_MAX)))
    {
        MVD_DEBUGINFO(printf("%s(%d) DHD*5\n", __FUNCTION__, __LINE__));
        //Dual HD: (1920*2)*1088 or 1920*(1088*2)
        u8FBMode = MVD3_DHD_MODE;
        u8FBNum = MVD_FBNUM_MAX;
        u32FbHdSize = (MVD_DHD_FBSIZE*MVD_FBNUM_MAX);
    }
    else if ((MVD_SUPPORT_DHD) && (u32AvailFrmBuffSize >= (MVD_DHD_FBSIZE*MVD_FBNUM_MIN)))
    {
        MVD_DEBUGINFO(printf("%s(%d) DHD*4\n", __FUNCTION__, __LINE__));
        //Dual HD: (1920*2)*1088 or 1920*(1088*2)
        u8FBMode = MVD3_DHD_MODE;
        u8FBNum = MVD_FBNUM_MIN;
        u32FbHdSize = (MVD_DHD_FBSIZE*MVD_FBNUM_MIN);
    }
    else if (u32AvailFrmBuffSize >= (MVD_HD_FBSIZE*MVD_FBNUM_MAX))
    {
        MVD_DEBUGINFO(printf("%s(%d) HD*5\n", __FUNCTION__, __LINE__));
        u8FBMode = MVD3_HD_MODE;
        u8FBNum = MVD_FBNUM_MAX;
        u32FbHdSize = (MVD_HD_FBSIZE*MVD_FBNUM_MAX);
    }
    else if (u32AvailFrmBuffSize >= (MVD_HD_FBSIZE*MVD_FBNUM_MIN))
    {
        MVD_DEBUGINFO(printf("%s(%d) HD*4\n", __FUNCTION__, __LINE__));
        u8FBMode = MVD3_HD_MODE;
        u8FBNum = MVD_FBNUM_MIN;
        u32FbHdSize = (MVD_HD_FBSIZE*MVD_FBNUM_MIN);
    }
    else
    {
        MVD_DEBUGINFO(printf("%s(%d) SD\n", __FUNCTION__, __LINE__));
        ret = MVD_CheckFrmBuffSizeMin(fwCfg, memCfg);
        if (TRUE == ret)
        {
            u8FBMode = MVD3_SD_MODE;
            u8FBNum = MVD_FBNUM_MIN;
            u32FbSdSize = MVD4_MPEG_FBSIZE_SDMIN;
            MVD_DEBUGINFO(printf("Framebuffer [SD] mode, size = 0x%lx\n", u32FbSdSize));
        }
    }

    MVD_DEBUGINFO(printf("%s u8FBMode=0x%x, u8FBNum=%d, FBSize=0x%lx\n", __FUNCTION__, u8FBMode, u8FBNum, memCfg->u32FBSize));
    return ret;
}

//------------------------------------------------------------------------------
/// Configure MVD for memory and firmware and info.
/// Notice:
///     (1) u32FWAddr & u32DrvBufAddr should be on the same MIU
///     (2) u32FBAddr & u32BSAddr should be on the same MIU
/// @param -fwCfg  \b IN : pointer to firmware configuration
/// @param -memCfg \b IN : pointer to memory configuration
/// @param -InfoCfg \b IN : pointer to mvd info configuration
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetCfg(MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg, MVD_InfoCfg* InfoCfg)
{
    MS_BOOL bDrvBuffMiuSel=FALSE;
    MVD_FUNC_ENTRY();
    if ((!fwCfg) || (!memCfg))
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    //config firmware: framebuffer mode
#if (defined(CHIP_T2) || defined(CHIP_T7))
    if ((memCfg->u32FBSize < (MVD_HD_FBSIZE*4))
    || (stMemCfg.bSupportSDModeOnly))
    {   //Set Framebuffer mode as SD mode if FB < (1920*1088*1.5*4)
        u8FBMode = MVD3_SD_MODE;    //720*576
    }
    else
    {
        u8FBMode = MVD3_HD_MODE;    //1920*1088
    }
#else
    if (TRUE != MVD_CheckFrmBuffSize(fwCfg, memCfg))
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    if (TRUE != MDrv_MVD_IsMStreamerMode())
    {
        //Keep FBNum=4 for mpeg2/4, not-mstreamer/uniplayer mode.
        if (!_IS_VC1(fwCfg->eCodecType)) //mpeg2/4
        {
            if (u8FBNum != MVD_FBNUM_DEFAULT)
            {
                u8FBNum = MVD_FBNUM_DEFAULT;
            }
        }
    }
#endif
    stMemCfg.bSupportSDModeOnly = memCfg->bSupportSDModeOnly;
    stFwCfg.eCodecType = fwCfg->eCodecType;
    stFwCfg.bNotReload = fwCfg->bNotReload;
    MVD_DEBUGINFO(printf("bNotReload=0x%x\n", stFwCfg.bNotReload));
    stFwCfg.u8FBMode = u8FBMode;
    stFwCfg.stFBReduction.LumaFBReductionMode = fwCfg->stFBReduction.LumaFBReductionMode;
    stFwCfg.stFBReduction.ChromaFBReductionMode = fwCfg->stFBReduction.ChromaFBReductionMode;
    stFwCfg.stFBReduction.u8EnableAutoMode = fwCfg->stFBReduction.u8EnableAutoMode;

    if (MDrv_MIU_IsSupportMIU1())
    {
        if (memCfg->u32Miu1BaseAddr == 0)
        {
#if MVD_ENABLE_MSOS_MIU1_BASE
            memCfg->u32Miu1BaseAddr = HAL_MIU1_BASE;
#else
            memCfg->u32Miu1BaseAddr = MVD_MIU1_BASE_ADDRESS;
#endif
        }
        memCfg->bFWMiuSel = (memCfg->u32FWCodeAddr < memCfg->u32Miu1BaseAddr)?MIU_SEL_0:MIU_SEL_1;
        memCfg->bHWMiuSel = (memCfg->u32FBAddr < memCfg->u32Miu1BaseAddr)?MIU_SEL_0:MIU_SEL_1;
        bDrvBuffMiuSel = (memCfg->u32DrvBufAddr < memCfg->u32Miu1BaseAddr)?MIU_SEL_0:MIU_SEL_1;
    }
    else
    {
        memCfg->u32Miu1BaseAddr= 0;
        memCfg->bFWMiuSel = MIU_SEL_0;
        memCfg->bHWMiuSel = MIU_SEL_0;
        bDrvBuffMiuSel = MIU_SEL_0;
    }

    MVD_DEBUGINFO(printf("MIUSEL:: FW=%x HW=%x, miu1base=0x%lx\n",
                  memCfg->bFWMiuSel, memCfg->bHWMiuSel, memCfg->u32Miu1BaseAddr));
    MVD_DEBUGINFO(printf("bDrvBuffMiuSel = %x\n", bDrvBuffMiuSel));
    MS_ASSERT(bDrvBuffMiuSel == memCfg->bFWMiuSel);

    //set these attributes before HAL_MVD_MemSetMap() and HAL_MVD_LoadCode()
    stMemCfg.u32Miu1BaseAddr = memCfg->u32Miu1BaseAddr;
    stMemCfg.bFWMiuSel = memCfg->bFWMiuSel;
    stMemCfg.bHWMiuSel = memCfg->bHWMiuSel;

    //config memory
    HAL_MVD_MemSetMap(E_MVD_MMAP_FW, memCfg->u32FWCodeAddr, memCfg->u32FWCodeSize);
    HAL_MVD_MemSetMap(E_MVD_MMAP_FB, memCfg->u32FBAddr, memCfg->u32FBSize);
    HAL_MVD_MemSetMap(E_MVD_MMAP_BS, memCfg->u32BSAddr, memCfg->u32BSSize);
    HAL_MVD_MemSetMap(E_MVD_MMAP_DRV, memCfg->u32DrvBufAddr, memCfg->u32DrvBufSize);

    //both stMemCfg & memCfg are physical addr, except field u32FWSrcVAddr
    stMemCfg.eFWSrcType = memCfg->eFWSrcType;

    if (stMemCfg.eFWSrcType == E_MVD_FW_SOURCE_DRAM)
    {
        stMemCfg.u32FWSrcVAddr = HAL_MVD_PA2NonCacheSeg((MS_U32)memCfg->u32FWBinAddr);
    }
    else
    {
        stMemCfg.u32FWSrcVAddr = NULL;
    }

    stMemCfg.u32FWBinAddr = memCfg->u32FWBinAddr;
    stMemCfg.u32FWBinSize = memCfg->u32FWBinSize;
    stMemCfg.u32FWCodeAddr = memCfg->u32FWCodeAddr;
    stMemCfg.u32FWCodeSize = memCfg->u32FWCodeSize;
    stMemCfg.u32FBAddr = memCfg->u32FBAddr;
    stMemCfg.u32FBSize = memCfg->u32FBSize;
    stMemCfg.u32BSAddr = memCfg->u32BSAddr;
    stMemCfg.u32BSSize = memCfg->u32BSSize;
    stMemCfg.u32DrvBufAddr = memCfg->u32DrvBufAddr;
    stMemCfg.u32DrvBufSize = memCfg->u32DrvBufSize;
    stMemCfg.u32DynSacalingBufAddr = memCfg->u32DynSacalingBufAddr;
    stMemCfg.u32DynSacalingBufSize = memCfg->u32DynSacalingBufSize;
    stMemCfg.bEnableDynScale = memCfg->bEnableDynScale;

    bSlqTblSync = ((stMemCfg.u32DrvBufAddr < stMemCfg.u32BSAddr) ||
                   ((stMemCfg.u32DrvBufAddr+stMemCfg.u32DrvBufSize) > (stMemCfg.u32BSAddr+stMemCfg.u32BSSize)));
    MVD_DEBUGINFO(printf("bSlqTblSync = %x\n", bSlqTblSync));
    //MVD_DumpMemCfg(&stMemCfg);

    curCodecType = InfoCfg->u8CodecType;
    curSrcMode = InfoCfg->u8BSProviderMode;
    curDisablePESParsing = InfoCfg->bDisablePESParsing;

    return E_MVD_RET_OK;
}


static MS_BOOL MVD_Init(void)
{
#ifndef REDLION_LINUX_KERNEL_ENVI
    if (!OSAL_MVD_MutexInit())
    {
        MVD_DEBUGERROR(printf("%s: fail to init mutex\n", __FUNCTION__));
        _MVD_INIT_FAIL_RET();
    }
#endif
#if MVD_PROFILING
    MS_U32 t1=0, t2=0;
    t1 = MsOS_GetSystemTime();
#endif
    //load code
    if (!HAL_MVD_LoadCode())
    {
        MVD_DEBUGERROR(printf("_MVD_Init:MDrv_MVD_LoadCode failed\n"));
        _MVD_INIT_FAIL_RET();
    }
    else
    {
        MVD_DEBUGINFO(printf("_MVD_Init:MDrv_MVD_LoadCode success\n"));
    }
#if MVD_PROFILING
    t2 = MsOS_GetSystemTime();
    printf("i000: t2=%ld, diff=%ld\n", t2, t2-t1);
    t1 = t2;
#endif
    if (!HAL_MVD_InitHW())
    {
        MVD_DEBUGERROR(printf("_MVD_Init:HAL_MVD_InitHW failed\n"));
        _MVD_INIT_FAIL_RET();
    }
    else
    {
        MVD_DEBUGINFO(printf("_MVD_Init:HAL_MVD_InitHW success\n"));
    }
#if MVD_PROFILING
    t2 = MsOS_GetSystemTime();
    printf("i001: t2=%ld, diff=%ld\n", t2, t2-t1);
    t1 = t2;
#endif

    bStopped = FALSE;
    if (!HAL_MVD_InitFW())
    {
        MVD_DEBUGERROR(printf("_MVD_Init:HAL_MVD_InitFW failed\n"));
        _MVD_INIT_FAIL_RET();
    }
    else
    {
        MVD_DEBUGINFO(printf("_MVD_Init:HAL_MVD_InitFW success\n"));
    }
#if MVD_PROFILING
    t2 = MsOS_GetSystemTime();
    printf("i002: t2=%ld, diff=%ld\n", t2, t2-t1);
    t1 = t2;
#endif

    _bFwLoad = TRUE;
    HAL_MVD_SetVpuFwDecoder(E_VPU_DECODER_MVD);
    return TRUE;
}

void MDrv_MVD_RegSetBase(MS_U32 u32RegBaseAddr)
{
    HAL_MVD_RegSetBase(u32RegBaseAddr);
}

//------------------------------------------------------------------------------
/// MVD driver initialization
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_Init(void)
{
    MS_U32 u32Addr = 0;
    MS_U32 u32Len = 0;

    MVD_DEBUGINFO(printf("MDrv_MVD_Init:start\n"));

    MVD_InitVar();
    _bDecodeIFrame = FALSE;
    _bDrvInit = FALSE;

    if (!MVD_Init())
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Init:_MVD_Init failed\n"));
        return FALSE;
    }
    else
    {
        MVD_DEBUGINFO(printf("MDrv_MVD_Init:_MVD_Init success\n"));
    }
#ifndef _MVD_V3
    HAL_MVD_MemGetMap(E_MVD_MMAP_FW, &u32Addr, &u32Len);
    if(!MVD_MVDSetInternalBuffAddr(u32Addr, u32Len))
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Init:_MVD_MVDSetInternalBuffAddr failed\n"));
        return FALSE;
    }
    else
    {
        MVD_DEBUGINFO(printf("MDrv_MVD_Init:_MVD_MVDSetInternalBuffAddr success\n"));
    }
#endif
#if 0
    if(!HAL_MVD_GetMVDEngineCfg(_pMvdHWCfg))
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Init:HAL_MVD_GetMVDEngineCfg failed\n"));
        return FALSE;
    }
    printf("[VDEC] dbg %lx %lx %lx %lx %lx %lx %x %x %x \n",
        _pMvdHWCfg->MVD_HWVER,_pMvdHWCfg->u32MvdSlqTabLens,_pMvdHWCfg->u32MvdSlqTabSizes  ,_pMvdHWCfg->u32MvdSlqTabShiftBit1,
        _pMvdHWCfg->u32MvdSlqTabShiftBit2,_pMvdHWCfg->u32MvdSlqTabShiftMask,_pMvdHWCfg->u16MvdTileModePitchLens,
        _pMvdHWCfg->u16MvdTileModePitchSize,_pMvdHWCfg->u16MvdTileModeMask);
#endif
    _bDrvInit = TRUE;

    return TRUE;
}


//------------------------------------------------------------------------------
/// MVD driver exit
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_Exit(void)
{
    MVD_DEBUGINFO(printf("MDrv_MVD_Exit:start\n"));

    //do nothing if driver is not initialized
    if (_bDrvInit != TRUE)
    {
        MVD_DEBUGINFO(printf("MDrv_MVD_Exit: not-init yet\n"));
#if (!defined(CHIP_T2))&&(!defined(CHIP_T7))
        HAL_MVD_PowerCtrl(DISABLE);
        //HAL_VPU_PowerCtrl(DISABLE);
        HAL_VPU_DeInit();
#endif
        return FALSE;
    }

    //stop and reset FW/HW
    if (MDrv_MVD_Stop() != TRUE)
    {
        MVD_DEBUGINFO(printf("MDrv_MVD_Exit: stop fail\n"));
        HAL_MVD_Delayms(1);
        if ( HAL_MVD_TimeOut() == TRUE )
        {
            MVD_DEBUGERROR(printf("*** MVD ERR: STOP TIMEOUT!!! ***\n"));
        }
    }

    bStopped = TRUE;

#ifdef MVD_ENABLE_ISR
    //dettach isr
    if (eEventFlag)
    {
        if(E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(E_MVD_EVENT_DISABLE_ALL, (MVD_InterruptCb)NULL))
        {
            MVD_DEBUGERROR(printf("MDrv_MVD_Exit: dettach isr fail\n"));
        }
    }
#endif

#if !(defined(CHIP_T2) || defined(CHIP_T7))
    //From T4, need to check MAU idle before reset VPU
    {
        MS_U32 mau_idle_cnt = 100;// ms
        while(mau_idle_cnt)
        {
            if(TRUE == HAL_VPU_MAU_IDLE())
            {
                break;
            }
            mau_idle_cnt--;
            HAL_MVD_Delayms(1);
        }

        if(mau_idle_cnt == 0)
        {
            MVD_DEBUGERROR(printf("MAU idle time out~~~~~\n"));
        }
    }

    //CPU hold
    HAL_VPU_SwRst();
#endif

    //MVD HW reset
    if (!HAL_MVD_RstHW())
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Exit:MVD4ResetHW failed\n"));
    }
    else
    {
        MVD_DEBUGINFO(printf("MDrv_MVD_Exit:MVD4ResetHW success\n"));
    }

    //reset internal variables.
    MVD_InitVar();
    _bDecodeIFrame = FALSE;
    _bDrvInit = FALSE;

#if (!defined(CHIP_T2))&&(!defined(CHIP_T7))
    HAL_MVD_PowerCtrl(DISABLE);
    //HAL_VPU_PowerCtrl(DISABLE);
    HAL_VPU_DeInit();
    HAL_VPU_SwRelseMAU();
#endif

#ifndef REDLION_LINUX_KERNEL_ENVI
    OSAL_MVD_MutexExit();
#endif

    return TRUE;
}

//------------------------------------------------------------------------------
/// Reset MVD
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_Rst( void )
{
    MVD_FUNC_ENTRY();
    if (TRUE == HAL_MVD_SoftRstHW())
    {
        //notice: T2 MVD3 does not support soft-reset HW.
        //Thus, MDrv_MVD_Rst()==MDrv_MVD_Init() for T2.
        return E_MVD_RET_OK;
    }

    //For T3&Euclid MVD4, do re-init only when SoftRst does not work.
    if (FALSE == MDrv_MVD_Init())
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Rst:_MVD_Init failed\n"));
        return E_MVD_RET_FAIL;
    }
    else
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Rst:_MVD_Init success\n"));
    }

    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Allow MVD to start decoding even without sequence header.
//------------------------------------------------------------------------------
void MDrv_MVD_EnableForcePlay(void)
{
    MVD_FUNC_ENTRY();
    if (HAL_MVD_EnableForcePlay() == FALSE)
    {
        MVD_DEBUGERROR( printf("HAL_MVD_EnableForcePlay fail!!\r\n") );
        return;
    }
    return;
}

//------------------------------------------------------------------------------
/// Issue Play command to MVD
//------------------------------------------------------------------------------
void MDrv_MVD_Play(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
#if defined(CHIP_T7)
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0= u8MvdPlayMode;
    if (HAL_MVD_MVDCommand( CMD_PLAY, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_PLAY ) );
        return;
    }
#else
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    if (HAL_MVD_MVDCommand( CMD_DIU_WIDTH_ALIGN, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DIU_WIDTH_ALIGN ) );
        return;
    }

    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_PLAY, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_PLAY ) );
        return;
    }

#if (!defined(CHIP_T2))
    if (HAL_MVD_Resume() == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: HAL_MVD_Resume fail!!\r\n" ) );
        return;
    }
#endif
#endif
    _bDecodeIFrame = FALSE;
    return;
}

//------------------------------------------------------------------------------
/// Set bit stream buffer address to MVD
/// @param -u32start \b IN : start address
/// @param -u32end \b IN : end address
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetBitStreamAddr(MS_U32 u32start, MS_U32 u32end)
{
    MVD_CmdArg mvdcmd;
    MS_ASSERT((u32start%8)==0);
    u32start >>= 3;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32start);
    mvdcmd.Arg1 = H_WORD(u32start);
    mvdcmd.Arg2 = L_DWORD(u32start);
    mvdcmd.Arg3 = H_DWORD(u32start);

    if (HAL_MVD_MVDCommand( CMD_STREAM_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_STREAM_BUF_START ) );
        return FALSE;
    }

    MS_ASSERT((u32end%8)==0);
    u32end >>= 3;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32end);
    mvdcmd.Arg1 = H_WORD(u32end);
    mvdcmd.Arg2 = L_DWORD(u32end);
    mvdcmd.Arg3 = H_DWORD(u32end);

    if (HAL_MVD_MVDCommand( CMD_STREAM_BUF_END, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_STREAM_BUF_END ) );
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set frame buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void MDrv_MVD_SetFrameBuffAddr(MS_U32 u32addr)
{
    if ((u32addr>>3) > MAX_ADD_28BIT)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_SetFrameBuffAddr: only support 28bit add!\n"));
    }
    HAL_MVD_SetFrameBuffAddr(u32addr, u8FBMode);
}


//------------------------------------------------------------------------------
/// Set MVD SLQ start & end address
/// @param -u32start \b IN : start address
/// @param -u32end \b IN : end address
//------------------------------------------------------------------------------
void MDrv_MVD_SetSLQStartEnd(MS_U32 u32start, MS_U32 u32end)
{
    MVD_CmdArg mvdcmd;

    if ((u32start > MAX_ADD_27BIT) || ((u32end+1) > MAX_ADD_27BIT))
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_SetSLQStartEnd: only support 27bit add!\n"));
    }
    MVD_FUNC_ENTRY();
    SET_CMDARG(mvdcmd, (u32end+1));
    if (HAL_MVD_MVDCommand( CMD_SLQ_END, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SLQ_END ) );
        return;
    }

    SET_CMDARG(mvdcmd, u32start);
    if (HAL_MVD_MVDCommand( CMD_SLQ_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SLQ_START ) );
        return;
    }

    return;
}

//------------------------------------------------------------------------------
/// Get SLQ available level
/// @return SLQ available level
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetSLQAvailableLevel(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_SLQ_AVAIL_LEVEL, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SLQ_AVAIL_LEVEL ) );
        return 0;
    }
    MVD_DEBUGINFO( printf( "MDrv_MVD_GetSLQAvailableLevel=%x\n", mvdcmd.Arg2 ) );

    return mvdcmd.Arg2;
}


//Map driver CodecType to firmware CodecType
static MS_U8 MVD_MapCodecType(MVD_CodecType type)
{
    MS_U8 u8type = 0xff;
    switch (type)
    {
        case E_MVD_CODEC_MPEG2:
            u8type = CODEC_MPEG2;
            break;
        case E_MVD_CODEC_MPEG4:
            u8type = CODEC_MPEG4;
            break;
        case E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER:
            u8type = CODEC_MPEG4_SHORT_VIDEO_HEADER;
            break;
        case E_MVD_CODEC_DIVX311:
            u8type = CODEC_DIVX311;
            break;

        case E_MVD_CODEC_FLV:
            u8type = 0x03;
            break;

        case E_MVD_CODEC_VC1_MAIN: //RCV
            u8type = 0x05;
            break;

        case E_MVD_CODEC_VC1_ADV:  //VC1
            u8type = 0x04;
            break;

        default:
            break;
    }

    return u8type;
}


//Map driver SrcType to firmware SrcType
static MS_U8 MVD_MapSrcMode(MVD_SrcMode mode)
{
    MS_U8 u8mode = 0xff;
    switch (mode)
    {
        case E_MVD_TS_MODE:
            u8mode = STREAM_MODE;
            break;
        case E_MVD_FILE_MODE:
            u8mode = FILE_MODE;
            break;
        case E_MVD_SLQ_MODE:
            u8mode = SLQ_MODE;
            break;
        case E_MVD_SLQ_TBL_MODE:
            u8mode = SLQ_TBL_MODE;
            break;
        case E_MVD_TS_FILE_MODE:
            u8mode = TS_FILE_MODE;
            break;

        default:
            break;
    }

    return u8mode;
}


//------------------------------------------------------------------------------
/// Set codec type.
/// @param -u8CodecType \b IN : 0: mpeg4, 1: mpeg4 with short_video_header, 2: DivX311
/// @param -u8BSProviderMode \b IN : TS live stream, file, SLQ, SLQ table link and TS file mode.
//------------------------------------------------------------------------------
void MDrv_MVD_SetCodecInfo(MVD_CodecType u8CodecType, MVD_SrcMode u8BSProviderMode, MS_U8 bDisablePESParsing)
{
   #if (defined(CHIP_T7))
       return;
   #endif

    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();
    //printf("u8CodecType=0x%x\n", u8CodecType);
    //printf("u8BSProviderMode=0x%x\n", u8BSProviderMode);
    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = MVD_MapCodecType(u8CodecType);
    stCmdArg.Arg1 = MVD_MapSrcMode(u8BSProviderMode);
    stCmdArg.Arg2 = bDisablePESParsing;
    //arg2 is only valid for STREAM_MODE and TS_FILE_MODE
    //set as 0 to enable MVD parser and parser interrupt
    stCmdArg.Arg3 = 0;
    MVD_DEBUGINFO(printf("MDrv_MVD_SetCodecInfo: Cmd: %x, Arg0: %x, Arg1: %x. Arg2: %x\n",
                  CMD_CODEC_INFO, stCmdArg.Arg0, stCmdArg.Arg1, stCmdArg.Arg2));
    if (HAL_MVD_MVDCommand(CMD_CODEC_INFO, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_CODEC_INFO ) );
        return;
    }

#ifdef _MVD_V3
    MS_U32 u32Addr, u32Len;
    MS_U32 i;
    MVD_CmdArg mvdcmd;
    //set code offset to MVD
    HAL_MVD_MemGetMap(E_MVD_MMAP_FW, &u32Addr, &u32Len);
    SETUP_CMDARG(mvdcmd);
    i = u32Addr >> 3;
    mvdcmd.Arg0 = L_WORD(i);
    mvdcmd.Arg1 = H_WORD(i);
    mvdcmd.Arg2 = L_DWORD(i);
    mvdcmd.Arg3 = H_DWORD(i);
    if (HAL_MVD_MVDCommand( CMD_CODE_OFFSET, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_CODE_OFFSET ) );
        return;
    }
#endif

    curCodecType = u8CodecType;
    curSrcMode = u8BSProviderMode;
    curDisablePESParsing = bDisablePESParsing;

    //Refer to msAPI_VDPlayer_DecodeMPEG4.c (core\kernel\api\videoplayer)
    if (u8BSProviderMode == E_MVD_SLQ_TBL_MODE)
    {
        if ((u8CodecType == E_MVD_CODEC_MPEG4) ||
            (u8CodecType == E_MVD_CODEC_DIVX311) ||
            (u8CodecType == E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER))
        {
            // Enable PackMode
            SETUP_CMDARG(stCmdArg);
            stCmdArg.Arg0 = 3;
            HAL_MVD_MVDCommand(CMD_PARSE_M4V_PACKMD, &stCmdArg);

            // Set DIU width of rounding mode (align to 8byte)
            SETUP_CMDARG(stCmdArg);
            stCmdArg.Arg0 = 1;
            HAL_MVD_MVDCommand(CMD_DIU_WIDTH_ALIGN, &stCmdArg);
        }
#ifndef _MVD_V3
        MVD_SLQTblInit();
#endif
    }

#ifdef _MVD_V3
    //set internal buffers after setting codecinfo for V3
    HAL_MVD_MemGetMap(E_MVD_MMAP_FW, &u32Addr, &u32Len);
    if (!MVD_MVDSetInternalBuffAddr(u32Addr, u32Len))
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_Init:_MVD_MVDSetInternalBuffAddr failed\n"));
    }
    else
    {
        MVD_DEBUGINFO(printf("MDrv_MVD_Init:_MVD_MVDSetInternalBuffAddr success\n"));
    }
    if (u8BSProviderMode == E_MVD_SLQ_TBL_MODE)
    {
        MVD_SLQTblInit();
    }
#endif

    return;
}

//------------------------------------------------------------------------------
/// Set DivX patch.
/// @param -u8MvAdjust \b IN : chroma adjustment
/// @param -u8IdctSel \b IN : idct algorithm selection
//------------------------------------------------------------------------------
void MDrv_MVD_SetDivXCfg(MS_U8 u8MvAdjust, MS_U8 u8IdctSel)
{
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8MvAdjust;
    if (HAL_MVD_MVDCommand(CMD_DIVX_PATCH, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DIVX_PATCH ) );
        return;
    }

    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8IdctSel;
    if (HAL_MVD_MVDCommand(CMD_IDCT_SEL, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_IDCT_SEL ) );
        return;
    }

    return;
}


//------------------------------------------------------------------------------
/// Set DivX311 stream info.
/// @param divxInfo \b IN : DivX311 stream info.
//------------------------------------------------------------------------------
static void MVD_WriteDivx311Data(FW_DIVX_INFO *divxInfo)
{
    HAL_MVD_MemWrite4Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_VOL_HANDLE_DONE,divxInfo->vol_handle_done);
    HAL_MVD_MemWrite4Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_WIDTH,divxInfo->width);
    HAL_MVD_MemWrite4Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_HEIGHT,divxInfo->height);
    HAL_MVD_MemWrite4Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_FRAME_COUNT,divxInfo->frame_count);
    HAL_MVD_MemWrite4Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_FRAME_TIME,divxInfo->frame_time);
    HAL_MVD_MemWrite2Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_PTS_INCR,divxInfo->pts_incr);
    HAL_MVD_MemWrite4Byte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_FRAME_RATE,divxInfo->frame_rate);
    HAL_MVD_MemWriteByte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_ASPECT_RATIO,divxInfo->aspect_ratio);
    HAL_MVD_MemWriteByte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_PROGRESSIVE_SEQUENCE,divxInfo->progressive_sequence);
    HAL_MVD_MemWriteByte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_MPEG1,divxInfo->mpeg1);
    HAL_MVD_MemWriteByte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_PLAY_MODE,divxInfo->play_mode);
    HAL_MVD_MemWriteByte(pu8MVDSetHeaderBufStart+OFFSET_DIVX_MPEG_FRC_MODE,divxInfo->mpeg_frc_mode);
    return;
}


//static MS_U32 writePtrLast;   //SlqTbl debug
//------------------------------------------------------------------------------
/// Get MVD decoded picture counter
/// @return -decoded picture counter
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetPicCounter( void )
{
    //printf("==>> wr=%lx rpt=%lx\n", _drvSlqTbl.u32WrPtr, MDrv_MVD_GetSLQReadPtr());
#if 0
    if (MDrv_MVD_GetVldErrCount()!=0)
    {
        printf("$$ wPtr= 0x%lx(0x%lx) rPtr=0x%lx(0x%lx) vldErr=0x%lx\n", writePtrLast, _drvSlqTbl.u32WrPtr,
        MDrv_MVD_GetSLQReadPtr(), _drvSlqTbl.u32RdPtr, MDrv_MVD_GetVldErrCount());
        _SLQTbl_DumpPtsTbl(0x530, 0x620);
        while(1);
    }
#endif
    MVD_FUNC_ENTRY();
#if (defined(CHIP_T7))
    return __MVD_GetPicCounter();
#else
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetPicCounter error: pu8MVDGetFrameInfoBufStart=NULL\n"));
            return 0;
    }

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_FRAME_COUNT);
#endif
}


//------------------------------------------------------------------------------
/// Get MVD skipped picture counter
/// @return -skipped picture counter
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetSkipPicCounter( void )
{
    MVD_FUNC_ENTRY();
    #if (defined(CHIP_T7))
    return 0;
    #else
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetSkipPicCounter error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return 0;
    }

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_SKIP_FRAME_COUNT);
    #endif
}

//------------------------------------------------------------------------------
/// Get MVD picture type
/// @return - picture type
//------------------------------------------------------------------------------
MVD_PicType MDrv_MVD_GetPicType( void )
{
    MS_U32 picType = 0xff;
    MVD_PicType ret = E_MVD_PIC_UNKNOWN;

    MVD_FUNC_ENTRY();
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetSkipPicCounter error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return E_MVD_PIC_UNKNOWN;
    }

    picType = HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_PICTURE_TYPE);
    switch (picType)
    {
        case 0:
            ret = E_MVD_PIC_I;
            break;
        case 1:
            ret = E_MVD_PIC_P;
            break;
        case 2:
            ret = E_MVD_PIC_B;
            break;
        default:
            break;
    }
    return ret;
}


//------------------------------------------------------------------------------
/// Get MVD bit rate (bits/sec)
/// @return -bit rate
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetBitsRate( void )
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetVolBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetValidStreamFlag error: pu8MVDGetVolBufStart=NULL\n"));
        return FALSE;
    }

    //Ref 13818-2, this flag is unit of 400 bits/sec
    return HAL_MVD_MemRead4Byte(pu8MVDGetVolBufStart+OFFSET_BIT_RATE)*400;
}


//------------------------------------------------------------------------------
/// Get video_range flag.
/// Supported after MVD FW release v00720390
/// Color team need this information for better color quality.
/// When video_range = 0  gives a range of Y from 16 to 235 and Cb , Cr from 16 to 240
/// When video_range = 1  gives a range of Y from  0 to 255 and Cb , Cr from  0 to 255
/// @return -video_range
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetVideoRange( void )
{
    #if defined(CHIP_T7)
    return 1;
    #endif
    MVD_FUNC_ENTRY();
    if(pu8MVDGetVolBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetVideoRange error: pu8MVDGetVolBufStart=NULL\n"));
        return FALSE;
    }

    return HAL_MVD_MemReadByte(pu8MVDGetVolBufStart+OFFSET_VIDEO_RANGE);
}

//------------------------------------------------------------------------------
/// Get LowDelay flag
/// @return -True/False
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetLowDelayFlag( void )
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetVolBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetLowDelayFlag error: pu8MVDGetVolBufStart=NULL\n"));
        return FALSE;
    }

    return HAL_MVD_MemReadByte(pu8MVDGetVolBufStart+OFFSET_LOW_DELAY);
}

//------------------------------------------------------------------------------
/// Get MPEG 3:2 pull down flag
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIs32PullDown(void)
{
    MVD_FUNC_ENTRY();
    if(pu8MVDGetVolBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetIs32PullDown error: pu8MVDGetVolBufStart=NULL\n"));
        return FALSE;
    }

    return (MS_BOOL)HAL_MVD_MemReadByte(pu8MVDGetVolBufStart+OFFSET_MPEG_FRC_MODE);
}

//------------------------------------------------------------------------------
/// Get if Dynamic Scaling is enabled (reported by firmware)
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsDynScalingEnabled(void)
{
    MVD_FUNC_ENTRY();
    MS_BOOL bEnDynScale = FALSE;
    if(pu8MVDGetVolBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetIsDSEnabled error: pu8MVDGetVolBufStart=NULL\n"));
        return FALSE;
    }
    bEnDynScale = (MS_BOOL)HAL_MVD_MemReadByte(pu8MVDGetVolBufStart+OFFSET_DS_ENABLE);
    if ((FALSE == stMemCfg.bEnableDynScale) && (bEnDynScale))
    {
        MVD_DEBUGERROR(printf("bEnDynScale err: %x\n", bEnDynScale));
    }

    return bEnDynScale;
}

//------------------------------------------------------------------------------
/// Get the difference of PTS and STC
/// @return -(PTS-STC)
//------------------------------------------------------------------------------
MS_S32 MDrv_MVD_GetPtsStcDiff(void)
{
    MVD_FUNC_ENTRY();
#if (defined(CHIP_T7) || defined(CHIP_T2))
    return 0; //to be implemented
#else
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetPtsStcDiff err: pu8MVDGetFrameInfoBufStart=NULL\n"));
            return 0;
    }
    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_PTS_STC);
#endif
}

//------------------------------------------------------------------------------
/// Get MVD SLQ read pointer
/// @return -SLQ read pointer
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetSLQReadPtr( void )
{
    MS_U32 u32RdPtr = 0;

    if (!bSlqTblHKCtrl)
    {
        MVD_CmdArg mvdcmd;

        MVD_FUNC_ENTRY();
        SETUP_CMDARG(mvdcmd);
        if (HAL_MVD_MVDCommand( CMD_SLQ_GET_TBL_RPTR, &mvdcmd ) == FALSE)
        {
            MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SLQ_GET_TBL_RPTR ) );
            return 0;
        }
        u32RdPtr = mvdcmd.Arg0 | (mvdcmd.Arg1<<8) | (mvdcmd.Arg2<<16) | (mvdcmd.Arg3<<24);
    }
    else
    {
        u32RdPtr = HAL_MVD_SlqTblProbeRdPtr();
    }

    //printf("##### MDrv_MVD_GetSLQReadPtr 0x%lx\n", readPtr);

    if(u32RdPtr == 0)//not start decode yet,MVD return 0
    {
        u32RdPtr = 0;
    }
    else
    {
        u32RdPtr = u32RdPtr << 3;
    }

    return u32RdPtr;
}

//------------------------------------------------------------------------------
/// Get MVD SLQ write pointer
/// @return -SLQ write pointer
//------------------------------------------------------------------------------
void MDrv_MVD_SetSLQWritePtr(MS_BOOL bCheckData)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32WrPtr;

#ifdef _MVD_V3
#define CMD_TIMEOUT_MS  500
    if (!bRdyToFireCmd)
    {
        //check FW is init success and thus ready to update write pointer
        MS_U32 u32TimeCnt;

        u32TimeCnt = HAL_MVD_GetTime();
        while ((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS)
        {
            if (MDrv_MVD_GetDecodeStatus() != E_MVD_STAT_IDLE)
            {
                bRdyToFireCmd = TRUE;
                (printf("time=%ld ms, ", (HAL_MVD_GetTime() - u32TimeCnt)));
                break;
            }
        }
        if (!bRdyToFireCmd)
        {
            MVD_DEBUGERROR(printf("%s: err timeout(%ld)! stat=0x%x\n", __FUNCTION__,
                    HAL_MVD_GetTime() - u32TimeCnt, MDrv_MVD_GetDecodeStatus()));
            return;
        }
        else
        {
            (printf("%s: ready to update WrPtr.\n", __FUNCTION__));
        }
    }
#endif

    MS_ASSERT(_drvSlqTbl.u32WrPtr < _drvSlqTbl.u32EndAdd);
    MS_ASSERT(_drvSlqTbl.u32WrPtr >= _drvSlqTbl.u32StAdd);
#if SLQ_NEW_PUSH
    if((!_SlqStatus.bSlqFireRdy)&&_SlqStatus.bSlqCtrlBit)
    {
        (printf("**** _SlqStatus.bSlqFireRdy is not Ready ** \n"));
        return;
    }
    _SlqStatus.bSlqFireRdy = FALSE;
#endif
    if ((bCheckData==TRUE)&&(!bSlqTblHasValidData))
    {
        MVD_DEBUGINFO(printf("**** SlqWrPtr(0x%lx) is not update!(%x, %x) ****\n",
            _drvSlqTbl.u32WrPtr, bCheckData, bSlqTblHasValidData));
        return;
    }
    bSlqTblHasValidData = FALSE;

#if UPDATE_NEXT_WRPTR
    u32WrPtr = _drvSlqTbl.u32WrPtr;
#else
    //should report (WrPtr-8)
    if (_drvSlqTbl.u32WrPtr != _drvSlqTbl.u32StAdd)
    {
        u32WrPtr = _drvSlqTbl.u32WrPtr - SLQ_ENTRY_LEN;
    }
    else
    {
        u32WrPtr = _drvSlqTbl.u32EndAdd - SLQ_ENTRY_LEN;
    }
#endif

    //printf("%s wPtr = 0x%lx rPtr=0x%lx\n", __FUNCTION__, *pu32WrPtr, MDrv_MVD_GetSLQReadPtr());

#ifndef REDLION_LINUX_KERNEL_ENVI
    //if (stMemCfg.bFWMiuSel != stMemCfg.bHWMiuSel)
    if (bSlqTblSync)
    {
        //Update SLQ table, DivX311 patterns, and dummy patterns to bitstream buffer
        MS_U32 u32SrcOffset = _drvSlqTbl.u32StAdd;
        MS_U32 u32SrcAdd = u32SrcOffset;
        MS_U32 u32DstAdd = stMemCfg.u32BSAddr;
        MS_U32 u32DstOffset = MVD_GetMemOffset(stMemCfg.u32BSAddr);
        MS_U32 u32TblWr;
        //printf("===>offset(Src=0x%lx, Dst=0x%lx)", u32SrcAdd, u32DstAdd);
        if (stMemCfg.bFWMiuSel == MIU_SEL_1)
        {
            u32SrcAdd = u32SrcOffset + stMemCfg.u32Miu1BaseAddr;
        }
        //printf(" PA(Src=0x%lx, Dst=0x%lx)", u32SrcAdd, u32DstAdd);

        HAL_MVD_CPU_Sync();

        BDMA_Result bdmaRlt;

        bdmaRlt = MDrv_BDMA_CopyHnd(u32SrcAdd, u32DstAdd, stMemCfg.u32DrvBufSize, bdmaCpyType, BDMA_OPCFG_DEF);

        if (E_BDMA_OK != bdmaRlt)
        {
            MVD_DEBUGERROR(printf("%s MDrv_BDMA_MemCopy fail ret=%x!\n", __FUNCTION__, bdmaRlt));
        }
        MVD_DEBUGINFO(printf("SlqWrPtr_BDMA src=0x%lx dst=0x%lx size=0x%lx cpyType=0x%x\n",
            u32SrcAdd, u32DstAdd, stMemCfg.u32DrvBufSize, bdmaCpyType));

        u32TblWr = u32DstOffset + (u32WrPtr - _drvSlqTbl.u32StAdd);
        //printf(" wr=0x%lx, tblWr=0x%lx\n", writePtr, u32TblWr);
        u32WrPtr = u32TblWr;
    }
    //printf("wPtr= 0x%lx(0x%lx) rPtr=0x%lx(0x%lx)\n", writePtr, _drvSlqTbl.u32WrPtr,
    //    MDrv_MVD_GetSLQReadPtr(), _drvSlqTbl.u32RdPtr);
#endif

    //writePtrLast = u32WrPtr;
    #if SLQ_NEW_PUSH
    if(!_SlqStatus.bSlqEnLastFrameShow && _SlqStatus.bSlqCtrlBit)
    {
        u32WrPtr = _SlqStatus.u32VaildWptrAddr;
    }
    #endif
    MS_ASSERT((u32WrPtr%8)==0);
    u32WrPtr >>= 3;

    if (!bSlqTblHKCtrl)
    {
        SET_CMDARG(mvdcmd, u32WrPtr);
        HAL_MVD_CPU_Sync();
        if (HAL_MVD_MVDCommand( CMD_SLQ_UPDATE_TBL_WPTR, &mvdcmd ) == FALSE)
        {
            MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SLQ_UPDATE_TBL_WPTR ) );
            return;
        }
    }
    else
    {
        HAL_MVD_SlqTblLoadWrPtr(u32WrPtr);
        if (HAL_MVD_SlqTblProbeWrPtr() != u32WrPtr)
        {
            MVD_DEBUGERROR(printf("Ooops! SlqWrPtr update fail!!! 0x%lx != 0x%lx\n", HAL_MVD_SlqTblProbeWrPtr(), u32WrPtr));
        }
    }

    return;
}

//------------------------------------------------------------------------------
/// Get MVD decoded frame index.  For debug.
/// @return -frame index of the decoded frame
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetDecodedFrameIdx ( void )
{
    MVD_FUNC_ENTRY();
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetDecodedFrameIdx error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return 0;
    }

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_FB_INDEX);
}

//------------------------------------------------------------------------------
/// Get MVD VLD error count
/// @return -VLD error count
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetVldErrCount( void )
{
    MVD_FUNC_ENTRY();
#if defined(CHIP_T7)
    return __MVD_GetVldErrCount();
#else
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetVldErrCount error!\n"));
        return 0;
    }

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_VLD_ERR_COUNT);
#endif
}


//Get ErrStatus when ErrCode==ErrShape
static MVD_ErrStatus MVD_GetErrShapeStat(MS_U32 u32errStat)
{
    MVD_ErrStatus st = E_MVD_ERR_STATUS_UNKOWN;
    switch (u32errStat)
    {
        case 0:
            st = E_MVD_ERR_SHAPE_RECTANGULAR;
            break;
        case 1:
            st = E_MVD_ERR_SHAPE_BINARY;
            break;
        case 2:
            st = E_MVD_ERR_SHAPE_BINARY_ONLY;
            break;
        case 3:
            st = E_MVD_ERR_SHAPE_GRAYSCALE;
            break;
        default:
            break;
    }
    return st;
}

//Get ErrStatus when ErrCode==ErrSprite
static MVD_ErrStatus MVD_GetErrSpriteStat(MS_U32 u32errStat)
{
    MVD_ErrStatus st = E_MVD_ERR_STATUS_UNKOWN;
    switch (u32errStat)
    {
        case 0:
            st = E_MVD_ERR_USED_SPRITE_UNUSED;
            break;
        case 1:
            st = E_MVD_ERR_USED_SPRITE_STATIC;
            break;
        case 2:
            st = E_MVD_ERR_USED_SPRITE_GMC;
            break;
        case 3:
            st = E_MVD_ERR_USED_SPRITE_RESERVED;
            break;
        default:
            break;
    }
    return st;
}


//------------------------------------------------------------------------------
/// Get MVD error info.
/// This function can be used to diagnosis when the 1st DispReady doesn't occur.
///
/// @param - errorCode \b OUT : error code
/// @param - errorStatus \b OUT : error status
//------------------------------------------------------------------------------
void MDrv_MVD_GetErrInfo(MVD_ErrCode *errCode, MVD_ErrStatus *errStatus)
{
    MS_U32 errorCode = E_MVD_ERR_UNKNOWN;
    MS_U32 errorStatus = E_MVD_ERR_STATUS_UNKOWN;

    MVD_FUNC_ENTRY();
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetErrInfo error!\n"));
        return;
    }

    errorCode = HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_ERROR_CODE);
    errorStatus = HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_ERROR_STATUS);

    switch (errorCode)
    {
        case VOL_SHAPE:
            *errCode = E_MVD_ERR_SHAPE;
            *errStatus = MVD_GetErrShapeStat(errorStatus);
            break;
        case VOL_USED_SPRITE:
            *errCode = E_MVD_ERR_USED_SPRITE;
            *errStatus = MVD_GetErrSpriteStat(errorStatus);
            break;
        case VOL_NOT_8_BIT:
            *errCode = E_MVD_ERR_NOT_8_BIT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_NERPRED_ENABLE:
            *errCode = E_MVD_ERR_NERPRED_ENABLE;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_REDUCED_RES_ENABLE:
            *errCode = E_MVD_ERR_REDUCED_RES_ENABLE;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_SCALABILITY:
            *errCode = E_MVD_ERR_SCALABILITY;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_H263_ERROR:
            *errCode = E_MVD_ERR_H263_ERROR;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_RES_NOT_SUPPORT:
            *errCode = E_MVD_ERR_RES_NOT_SUPPORT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_MPEG4_NOT_SUPPORT:
            *errCode = E_MVD_ERR_MPEG4_NOT_SUPPORT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_PROFILE_NOT_SUPPORT:
            *errCode = E_MVD_ERR_PROFILE_NOT_SUPPORT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_RCV_ERROR_OCCUR:
            *errCode = E_MVD_ERR_RCV_ERROR_OCCUR;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_OTHER:
            *errCode = E_MVD_ERR_OTHER;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;

#if (defined(CHIP_A6))
        //For A6, FW will report these error code for
        //SLQ_TBL mode and mpeg4/flv/svh/divx311/vc1.
        case VOL_VC1_NOT_SUPPORT:
            *errCode = E_MVD_ERR_VC1_NOT_SUPPORT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_UNKNOW_CODEC_NOT_SUPPORT:
            *errCode = E_MVD_ERR_UNKNOW_CODEC_NOT_SUPPORT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
        case VOL_SLQ_TBL_NOT_SUPPORT:
            *errCode = E_MVD_ERR_SLQ_TBL_NOT_SUPPORT;
            *errStatus = E_MVD_ERR_STATUS_NONE;
            break;
#endif

        default:
            *errCode = E_MVD_ERR_UNKNOWN;
            *errStatus = E_MVD_ERR_STATUS_UNKOWN;
            break;
    }

    return;
}

//------------------------------------------------------------------------------
/// Get valid MVD stream detected flag
/// For MPEG2/4,
///     1. width<16(1 macroblock), height<16, width>1920, or height>1080
///     2. next start code not found
/// the stream would be considered as invalid.
/// For VC1/RCV, not impelment, default: valid
/// @return -decoded flag
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetValidStreamFlag( void )
{
    MVD_FUNC_ENTRY();
#if defined(CHIP_T7)
    return __MVD_GetValidStreamFlag();
#else
    //printf("  validStream=%d\n\n",
    //    !HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_INVALIDSTREAM));
    return !HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_INVALIDSTREAM);
#endif
}

#if !defined(CHIP_T7)
static MS_U32 MVD_GCD(MS_U32 u32A, MS_U32 u32B)
{
   MS_S32 i;
   MS_U32 x[4]; /* need x[0], x[1], x[i-1] */

   if (u32A > u32B)
   {
     x[0] = u32A; x[1] = u32B;
   }
   else
   {
     x[0] = u32B; x[1] = u32A;
   }

   i = 1;

   #define w(x) (((x)<4)?(x):(2+((x)&1)))

   while( x[w(i)] != 0 )
   {
       x[w(i+1)] = x[w(i-1)] % x[w(i)];
       i++;
   }

   return x[w(i-1)];

   #undef w
}
#endif

//------------------------------------------------------------------------------
/// Get video frame information from MVD
/// (SourceW * ParH) : (SourceH * ParW) = DarW : DarH
/// E.g. Source PAL 704x576, Par(Pixel aspect ratio)=12/11,
///      Dar(Display AR) = (704x12):(576x11) = 4:3
/// @param -pinfo \b IN : pointer to video frame information
//------------------------------------------------------------------------------
void MDrv_MVD_GetFrameInfo(MVD_FrameInfo *pinfo)
{
#if defined (CHIP_T7)
    MVD_FUNC_ENTRY();
    __MVD_GetFrameInfo(pinfo);

#else
    MS_U32 u32DAR_Width=0,u32DAR_Height=0,u32PAR_Width=0,u32PAR_Height=0,u32GCD=0;
    MS_U32 u32Vertical_Size=0, u32Horizontal_Size=0;
    //13818-2 page 38 Table 6-3
    MS_U8 u8DARTable[5][2] = { {1,1}, {1,1}, {4,3}, {16,9}, {221,100} };

    MVD_FUNC_ENTRY();

    if (pu8MVDGetVolBufStart == NULL)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetFrameInfo error: pu8MVDGetVolBufStart=NULL\n"));
        return;
    }

//    MVD_ReadVolInfo(&gvolInfo);
    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();
    gvolInfo = *(volatile FW_VOL_INFO*)u32VolAdd;

#if 0
    MVD_DEBUGINFO(printf("vol info,vol_info=%d,sprite_usage=%d,pts_incr=%d,\n",
                  gvolInfo.vol_info,gvolInfo.sprite_usage,gvolInfo.pts_incr));
    MVD_DEBUGINFO(printf("vol info,width=%x,height=%x,frame_rate=%d,aspect_ratio=%x,\n",
                  gvolInfo.width,gvolInfo.height,gvolInfo.frame_rate,gvolInfo.aspect_ratio));
    MVD_DEBUGINFO(printf("vol info,progressive_sequence=%x,mpeg1=%x,play_mode=%x,bit_rate=%x,\n",
                  gvolInfo.progressive_sequence,gvolInfo.mpeg1,gvolInfo.play_mode,gvolInfo.bit_rate));
#endif

    pinfo->u16HorSize = (MS_U16) gvolInfo.width;
    pinfo->u16VerSize = (MS_U16) gvolInfo.height;
    pinfo->u16par_width  = (MS_U16) gvolInfo.par_width;
    pinfo->u16par_height = (MS_U16) gvolInfo.par_height;
    pinfo->u8AspectRate = gvolInfo.aspect_ratio;

    pinfo->u16CropBottom = gvolInfo.CropBottom;
    pinfo->u16CropTop = 0;
    pinfo->u16CropLeft = 0;
    if (pinfo->u16HorSize & MVD_TILE_MODE_MASK)
    {
        //Notice: Firmware and driver have to be consistent for this part,
        // otherwise the pitch will not match and video is noisy.
        pinfo->u16CropRight =MVD_TILE_MODE_PITCH_LENS - (pinfo->u16HorSize & MVD_TILE_MODE_MASK);
        pinfo->u16HorSize = ((pinfo->u16HorSize >> MVD_TILE_MODE_PITCH_SIZES) + 1) << MVD_TILE_MODE_PITCH_SIZES;
    }
    else
    {
        pinfo->u16CropRight = 0;
    }

    if ((curCodecType == E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER) ||
        (curCodecType == E_MVD_CODEC_DIVX311) || (curCodecType == E_MVD_CODEC_FLV))
    {
        pinfo->u8Interlace= FALSE; //divx311/flv/svh just has progressive mode
        MS_ASSERT(gvolInfo.progressive_sequence == 1); //FW will init it as 1
    }
    else
    {
        if (gvolInfo.progressive_sequence == 0)
        {
            pinfo->u8Interlace=1;
        }
        else
        {
            pinfo->u8Interlace=0;
        }
    }

    ///Calculate PAR info
    if ((pinfo->u16par_width == 0) || (pinfo->u16par_height == 0))
    {
        if ((pinfo->u8AspectRate > 0) && (pinfo->u8AspectRate < 5 ))
        {
            if (pinfo->u8AspectRate == 1)
            {   //SAR=1.0 square sample
                u32DAR_Width = (MS_U32)pinfo->u16HorSize;
                u32DAR_Height = (MS_U32)pinfo->u16VerSize;
                pinfo->u16par_width = 1;
                pinfo->u16par_height = 1;
            }
            else
            {
                u32DAR_Width = (MS_U32)u8DARTable[pinfo->u8AspectRate][0];
                u32DAR_Height = (MS_U32)u8DARTable[pinfo->u8AspectRate][1];
                u32Vertical_Size = (MS_U32)pinfo->u16VerSize;
                u32Horizontal_Size = (MS_U32)(pinfo->u16HorSize - pinfo->u16CropRight);
                u32PAR_Width = u32DAR_Width * u32Vertical_Size;
                u32PAR_Height = u32DAR_Height * u32Horizontal_Size;
                u32GCD = MVD_GCD(u32PAR_Width, u32PAR_Height);
                if (0 == u32GCD)
                {
                    pinfo->u16HorSize = 0xFFFF;
                    pinfo->u16VerSize = 0xFFFF;

                    return;
                }
                else
                {
                    pinfo->u16par_width = (MS_U16) (u32PAR_Width / u32GCD);
                    pinfo->u16par_height = (MS_U16) (u32PAR_Height / u32GCD);
                    MVD_DEBUGVERBAL(printf("u32PAR_Width:%ld,u32PAR_Height:%ld,GCD:%ld\n",u32PAR_Width,u32PAR_Height,u32GCD));
                }
            }
        }
        else
        {
            //set to 0 to indicate it's abnormal
            u32DAR_Width = 0;
            u32DAR_Height = 0;
            pinfo->u16par_width = 0;
            pinfo->u16par_height = 0;
        }
        MVD_DEBUGVERBAL(printf("u32DAR_Width:%ld,u32DAR_Height%ld\n",u32DAR_Width,u32DAR_Height));
    }
    MVD_DEBUGVERBAL(printf("pinfo->u16par_width:%d, pinfo->u16par_height:%d\n",pinfo->u16par_width, pinfo->u16par_height));


    if (curCodecType == E_MVD_CODEC_MPEG2)
    {
        if ((gvolInfo.frame_rate > 8 && pinfo->u8Interlace == 0))
           //||(gvolInfo.frame_rate > 5 && pinfo->u8Interlace == 1))
        {
            pinfo->u32FrameRate = 0;
        }
        else
        {
            pinfo->u32FrameRate = stFrameRateCode[gvolInfo.frame_rate];
        }
    }
    else if ((curCodecType == E_MVD_CODEC_VC1_ADV) || (curCodecType == E_MVD_CODEC_VC1_MAIN))
    {
        if ((gvolInfo.vc1_frame_rate != 0) && (gvolInfo.vc1_frame_rate != MVD_U32_MAX))
        {
            pinfo->u32FrameRate = gvolInfo.vc1_frame_rate;
        }
        else if (gdivxInfo.frame_rate != 0)
        {
            pinfo->u32FrameRate = gdivxInfo.frame_rate; //report framerate specified in MDrv_MVD_SetFrameInfo()
        }
        else
        {
            pinfo->u32FrameRate = MVD_FRAMERATE_DEFAULT;
        }

        MVD_DEBUGVERBAL(printf("MVD: vc1_frameRate=%ld\n", pinfo->u32FrameRate));
    }
    else if (curCodecType == E_MVD_CODEC_MPEG4)
    {
        if (gdivxInfo.frame_rate != 0)
        {
            pinfo->u32FrameRate = gdivxInfo.frame_rate;
            //report framerate specified in MDrv_MVD_SetFrameInfo(), which is usually obtained from container
        }
        else if (gvolInfo.frame_rate != 0)
        {
            pinfo->u32FrameRate = gvolInfo.frame_rate;  //report framerate from f/w
        }
        else if ((gvolInfo.fixed_vop_time_incr != 0) && (gvolInfo.vol_time_incr_res != 0))
        {
            pinfo->u32FrameRate = (gvolInfo.vol_time_incr_res * 1000) / gvolInfo.fixed_vop_time_incr;
            //calculate framerate according to vol header
        }
        else
        {
            pinfo->u32FrameRate = MVD_FRAMERATE_DEFAULT;
        }
        MVD_DEBUGVERBAL(printf("MVD: vol_time_incr_res=%d, fixed_vop_time_incr=%d\n", gvolInfo.vol_time_incr_res, gvolInfo.fixed_vop_time_incr));
    }
    else
    {
        if (gvolInfo.frame_rate != 0)
        {
            pinfo->u32FrameRate = gvolInfo.frame_rate;
        }
        else if (gdivxInfo.frame_rate != 0)
        {
            pinfo->u32FrameRate = gdivxInfo.frame_rate; //report framerate specified in MDrv_MVD_SetFrameInfo()
        }
        else
        {
            pinfo->u32FrameRate = MVD_FRAMERATE_DEFAULT;
        }
    }

    //printf("===> MVD: frameRate=%d  curCodecType=0x%x\n", pinfo->u32FrameRate, curCodecType);

    //for MM
    pinfo->u8MPEG1=gvolInfo.mpeg1;
    pinfo->u16PTSInterval=gvolInfo.pts_incr;
    pinfo->u8PlayMode=gvolInfo.play_mode;
    pinfo->u8FrcMode=gvolInfo.mpeg_frc_mode;

    //for dynamic scaling
    pinfo->bEnableMIUSel = stMemCfg.bFWMiuSel;
    if (stMemCfg.bEnableDynScale)
    {
        if (stMemCfg.bFWMiuSel == MIU_SEL_1)
        {
            pinfo->u32DynScalingAddr= u32DynScalingAdd + stMemCfg.u32Miu1BaseAddr;
        }
        else
        {
            pinfo->u32DynScalingAddr= u32DynScalingAdd;
        }
        pinfo->u8DynScalingDepth= u8DynScalingDepth;
        pinfo->u32DynScalingBufSize= gvolInfo.DSbufsize;
    }
    else
    {
        pinfo->u32DynScalingAddr= NULL;
        pinfo->u8DynScalingDepth= 0;
        pinfo->u32DynScalingBufSize= 0;
    }

    return;
#endif
}

//------------------------------------------------------------------------------
/// Set video frame information from MVD
/// @param -pinfo \b IN : pointer to video frame information
//------------------------------------------------------------------------------
void MDrv_MVD_SetFrameInfo(MVD_FrameInfo *pinfo )
{
    MVD_FUNC_ENTRY();
    if(pu8MVDSetHeaderBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_SetFrameInfo error: pu8MVDSetHeaderBufStart=NULL\n"));
        return;
    }

    gdivxInfo.width=pinfo->u16HorSize;
    gdivxInfo.height=pinfo->u16VerSize;
    gdivxInfo.aspect_ratio=pinfo->u8AspectRate;
    gdivxInfo.frame_rate = pinfo->u32FrameRate;

    //for MM
    gdivxInfo.mpeg1=pinfo->u8MPEG1;
    gdivxInfo.pts_incr=pinfo->u16PTSInterval;
    gdivxInfo.play_mode=pinfo->u8PlayMode;
    gdivxInfo.mpeg_frc_mode=pinfo->u8FrcMode;

    if(pinfo->u8Interlace==0)
        gdivxInfo.progressive_sequence=1;
    else
        gdivxInfo.progressive_sequence=0;

    gdivxInfo.frame_count=0;
    gdivxInfo.frame_time=0;
    gdivxInfo.vol_handle_done=0;
//    gdivxInfo.invalidstream=0;
    MVD_DEBUGINFO(printf("set vol info,pts_incr=%d,\n",gdivxInfo.pts_incr));
    MVD_DEBUGINFO(printf("set vol info,width=%x,height=%x,frame_rate=%d,aspect_ratio=%x,\n",
                  (unsigned int)gdivxInfo.width,(unsigned int)gdivxInfo.height,gdivxInfo.frame_rate,gdivxInfo.aspect_ratio));
    MVD_DEBUGINFO(printf("set vol info,progressive_sequence=%x,mpeg1=%x,play_mode=%x,\n",
                  gdivxInfo.progressive_sequence,gdivxInfo.mpeg1,gdivxInfo.play_mode));

    MVD_WriteDivx311Data(&gdivxInfo);
    return;
}


//------------------------------------------------------------------------------
/// Reset for I-frame decoding
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_RstIFrameDec( void )
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_STOP, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_STOP ) );
    }

    MDrv_MVD_Rst();
    return;
}

//take care MIU1 address
static MS_U32 MVD_GetMemOffset(MS_PHYADDR u32PhyAdd)
{
    if (u32PhyAdd >= stMemCfg.u32Miu1BaseAddr)
    {
        return (u32PhyAdd - stMemCfg.u32Miu1BaseAddr);
    }
    else
    {
        return u32PhyAdd;
    }
}

//------------------------------------------------------------------------------
/// Decode I-frame
/// @param -u32FrameBufAddr \b IN : start address of frame buffer
/// @param -u32StreamBufAddr \b IN : start address of stream buffer
/// @return -return decode I-frame success or not
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DecodeIFrame(MS_PHYADDR u32FrameBufAddr, MS_PHYADDR u32StreamBufAddr, MS_PHYADDR u32StreamBufEndAddr )
{
#if defined(CHIP_T7)
    u32FrameBufAddr     = MVD_GetMemOffset(u32FrameBufAddr);
    u32StreamBufAddr    = MVD_GetMemOffset(u32StreamBufAddr);
    u32StreamBufEndAddr = MVD_GetMemOffset(u32StreamBufEndAddr);
    _bDecodeIFrame = TRUE;
    return __MVD_DecodeIFrame( u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEndAddr);
#else
    MS_U32 u32deley = 0;
    MS_U32 u32time = 0;
    MVD_CmdArg mvdcmd;

    MVD_DEBUGINFO(printf("%s input  FBAdd=0x%lx streamStart=0x%lx streamEnd=0x%lx\n",
            __FUNCTION__, u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEndAddr));
    u32FrameBufAddr     = MVD_GetMemOffset(u32FrameBufAddr);
    u32StreamBufAddr    = MVD_GetMemOffset(u32StreamBufAddr);
    u32StreamBufEndAddr = MVD_GetMemOffset(u32StreamBufEndAddr);
    MVD_DEBUGINFO(printf("%s offset FBAdd=0x%lx streamStart=0x%lx streamEnd=0x%lx\n",
            __FUNCTION__, u32FrameBufAddr, u32StreamBufAddr, u32StreamBufEndAddr));

    MDrv_MVD_SetFrameBuffAddr( u32FrameBufAddr);
    MDrv_MVD_SetCodecInfo(E_MVD_CODEC_MPEG2, E_MVD_SLQ_MODE, DISABLE_PARSER);

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    if (HAL_MVD_MVDCommand( CMD_DISPLAY_CTL, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DISPLAY_CTL ) );
        return FALSE;
    }

    if (MDrv_MVD_StepDecode() == FALSE)
    {
        MVD_DEBUGERROR( printf( "MDrv_MVD_StepDecode fail!!\r\n") );
        return FALSE;
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    mvdcmd.Arg1 = 1;
    if (HAL_MVD_MVDCommand(CMD_FAST_SLOW, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FAST_SLOW ) );
        return FALSE;
    }

    //set data
    HAL_MVD_Delayms(2);

    HAL_MVD_CPU_Sync();
    HAL_MVD_FlushMemory();

    //wait vld init success or data may lost!
#define WAIT_INIT_SUCCESS_TIME 100 //100ms
    u32deley = HAL_MVD_GetTime();
    while ((MDrv_MVD_GetDecodeStatus()==DEC_STAT_IDLE) && (u32time<WAIT_INIT_SUCCESS_TIME))
    {
        u32time = HAL_MVD_GetTime()-u32deley;
    }
    if (u32time>=WAIT_INIT_SUCCESS_TIME)
    {
        MVD_DEBUGERROR(printf("%s: wait init_success timeout!!!\n", __FUNCTION__));
    }

    MDrv_MVD_SetSLQStartEnd(u32StreamBufAddr, u32StreamBufEndAddr);
    MVD_DEBUGINFO(printf("set MVD3_FW_SLQ_TAB_TMPBUF_ADR=%lx\n",u32MVDFWSLQTABTmpbufAdr));

    HAL_MVD_CPU_Sync();
    HAL_MVD_FlushMemory();
    MDrv_MVD_SetSLQStartEnd(u32MVDFWSLQTABTmpbufAdr, u32MVDFWSLQTABTmpbufAdr+MVD3_FW_SLQ_TAB_TMPBUF_LEN);

    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();

    // wait decode complete
#define WAIT_DECODE_DONE_TIME 33 //To decode 1 frame should take less than 33ms
    u32deley = HAL_MVD_GetTime();
    u32time = 0;
    while ((MDrv_MVD_GetPicCounter()<1) && (u32time<WAIT_DECODE_DONE_TIME))
    {
        u32time = HAL_MVD_GetTime()-u32deley;
    }
    if (u32time >= WAIT_DECODE_DONE_TIME)
    {
        MVD_DEBUGERROR(printf ("MDrv_MVD_DecodeIFrame time out(%ld, %ld)\n", u32time, u32deley));
        MVD_DEBUGERROR(printf("frmCnt=%ld state=0x%x lastCmd=0x%x\n", MDrv_MVD_GetPicCounter(), MDrv_MVD_GetDecodeStatus(), MDrv_MVD_GetLastCmd()));
        return FALSE;
    }
    MVD_DEBUGINFO(printf ("MDrv_MVD_DecodeIFrame time (%ld, %ld)\n", u32time, u32deley));
    //printf("frmCnt=%ld state=0x%x lastCmd=0x%x\n", MDrv_MVD_GetPicCounter(), MDrv_MVD_GetDecodeStatus(), MDrv_MVD_GetLastCmd());

    _bDecodeIFrame = TRUE;

    return TRUE;
#endif
}

//------------------------------------------------------------------------------
/// Query if MVD is decoding frame
/// @return -MVD is decoding frame or not
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsIFrameDecoding(void)
{
    return _bDecodeIFrame;
}

//------------------------------------------------------------------------------
/// Set bitstream buffer overflow threshold
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetOverflowTH(MS_U32 u32Threshold)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    MS_ASSERT((u32Threshold%8)==0);
    u32Threshold >>= 3;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32Threshold);
    mvdcmd.Arg1 = H_WORD(u32Threshold);
    mvdcmd.Arg2 = L_DWORD(u32Threshold);
    mvdcmd.Arg3 = H_DWORD(u32Threshold);

    if (HAL_MVD_MVDCommand( CMD_DMA_OVFTH, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DMA_OVFTH ) );
        return;
    }
    return;
}

//------------------------------------------------------------------------------
/// Set bitstream buffer underflow threshold
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetUnderflowTH(MS_U32 u32Threshold)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    MS_ASSERT((u32Threshold%8)==0);
    u32Threshold >>= 3;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32Threshold);
    mvdcmd.Arg1 = H_WORD(u32Threshold);
    mvdcmd.Arg2 = L_DWORD(u32Threshold);
    mvdcmd.Arg3 = H_DWORD(u32Threshold);

    if (HAL_MVD_MVDCommand( CMD_DMA_UNFTH, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DMA_UNFTH ) );
        return;
    }
    return;
}

//------------------------------------------------------------------------------
/// Get MVD active format
/// @return -active format
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetActiveFormat(void)
{
#if defined (CHIP_T7)
    return __MVD_GetActiveFormat();
#else
    MVD_CmdArg mvdcmd;
    static MS_U8 u8Afd = 0xff;
#if (MVD_TURBO_INIT)
    static MS_U32 u32FrmCnt = 0;

    if (u32FrmCnt == MDrv_MVD_GetPicCounter())
    {
        return u8Afd;
    }
    u32FrmCnt = MDrv_MVD_GetPicCounter();
#endif
    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_GET_AFD, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_GET_AFD  ) );
        return 0xff;
    }
    u8Afd = mvdcmd.Arg0;
    return u8Afd;
#endif
}

//------------------------------------------------------------------------------
/// Enable or disable AV synchronization.
/// Delay u32Delay ms if AVSync is enabled.
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetAVSync(MS_BOOL bEnable, MS_U32 u32Delay)
{
#if defined(CHIP_T7)
    __MVD_SetAVSync(bEnable, u32Delay);
    bAVSyncOn = bEnable;    //for IsAVSyncOn
    stSyncCfg.bEnable = bEnable;
    stSyncCfg.u32Delay = u32Delay;
    return;
#else
    MVD_CmdArg mvdcmd;

    //printf("%s bEnable=%d u32Delay=%ld\n",__FUNCTION__,bEnable,u32Delay);
    MVD_FUNC_ENTRY();

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    bAVSyncOn = bEnable;    //for IsAVSyncOn
    if (HAL_MVD_MVDCommand( CMD_SYNC_ON, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_SYNC_ON  ) );
        return;
    }

    if (bEnable && (u32Delay != 0))
    {
        SET_CMDARG(mvdcmd, _MS_TO_90K(u32Delay)); //u32Delay ms ==> 90k counter
        if (HAL_MVD_MVDCommand( CMD_SYNC_OFFSET, &mvdcmd ) == FALSE)
        {
            MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_SYNC_ON  ) );
            return;
        }
    }

    stSyncCfg.bEnable = bEnable;
    stSyncCfg.u32Delay = u32Delay;
    return;
#endif
}

//------------------------------------------------------------------------------
/// Set skip & repeat frame mode.
/// mode : 1 skip B frame 0 : skip display frame
/// @return -none
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetSkipRepeatMode(MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;
    MVD_FUNC_ENTRY();

    if (u8Mode != 1 && u8Mode != 0)
    {
        return FALSE; //invalid parameter, do nothing
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg4 = u8Mode;
    mvdcmd.Arg5 = u8Mode;
    if (HAL_MVD_MVDCommand( CMD_SYN_THRESHOLD , &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SYN_THRESHOLD ) );
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set the maximum repeat times for one frame when av is not sync.
/// E.g. Arg5=0x01 mean that frame will be repeated once if av is not sync.
///      Arg5=0xff mean that frame will be always repeated when av is not sync.
/// And customer can re-define the avsync skip behavior,
/// i.e., Arg4 = 1 for skip-b-frame only and 0 for skip display(default)
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetAVSyncThreshold(MS_U32 u32Th)
{
    MVD_CmdArg mvdcmd;
    MVD_FUNC_ENTRY();

    if (u32Th == 0x00)
    {
        return; //invalid parameter, do nothing
    }
    if (u32Th > 0xff)
    {
        u32Th = 0xff; //set to maximum
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg5 = u32Th;
    if (HAL_MVD_MVDCommand(CMD_SYN_THRESHOLD, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR(printf("Ctrl: 0x%x fail!!\n", CMD_SYN_THRESHOLD));
        return;
    }

    return;

}


//------------------------------------------------------------------------------
/// Set the threshold for VC1 avsync (live mode & file mode). Unit: 90K
/// @return -none
//------------------------------------------------------------------------------
void MDrv_MVD_SetAVSyncFreerunThreshold(MS_U32 u32Th)
{
    MVD_CmdArg mvdcmd;
    MVD_FUNC_ENTRY();

    SET_CMDARG(mvdcmd, u32Th);
    if (HAL_MVD_MVDCommand(CMD_AVSYNC_FREERUN_THRESHOLD, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR(printf("Ctrl: 0x%x fail!!\n", CMD_AVSYNC_FREERUN_THRESHOLD));
        return;
    }

    return;

}

//------------------------------------------------------------------------------
/// Get MVD AVSync delay (ms)
/// @return -avsync delay
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetAVSyncDelay(void)
{
    return (stSyncCfg.u32Delay);
}

//------------------------------------------------------------------------------
/// Get if AVSync is turned on
/// @return -TRUE for yes or FALSE for no
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsAVSyncOn(void)
{
    return bAVSyncOn;
}


//------------------------------------------------------------------------------
/// Get if firmware is repeating frame for AVSync.
/// @return -TRUE for yes or FALSE for no
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsSyncRep(void)
{
#if defined(CHIP_T7)
    return FALSE;
#else
    MS_U32 u32IntStat = 0;
    MS_BOOL bRet = FALSE;

    u32IntStat = HAL_MVD_GetIntState();
    if (u32IntStat != 0)
    {
        bRet = ((u32IntStat&INT_SYN_REP)==INT_SYN_REP) ? TRUE : FALSE;
    }
    return bRet;
#endif
}


//------------------------------------------------------------------------------
/// Get if firmware is skipping frame for AVSync.
/// @return -TRUE for yes or FALSE for no
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsSyncSkip(void)
{
#if defined(CHIP_T7)
    return FALSE;
#else
    MS_U32 u32IntStat = 0;
    MS_BOOL bRet = FALSE;

    u32IntStat = HAL_MVD_GetIntState();
    if (u32IntStat != 0)
    {
        bRet = ((u32IntStat&INT_SYN_SKIP)==INT_SYN_SKIP) ? TRUE : FALSE;
    }

    return bRet;
#endif
}

//------------------------------------------------------------------------------
/// Change PTS threshold for doing AVSync.
/// Scenario: When disable black screen earlier than normal case, use this function
/// to avoid frame skip too fast or video lag.
/// @param -bEnable \b IN : enable this configuration
/// @param -u16PTS \b IN : PTS threshold (unit: 90k counter, i.e. x/90 ms, max 728ms)
/// @return -TRUE for success or FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ChangeAVsync(MS_BOOL bEnable, MS_U16 u16PTS)
{
    #if !defined(CHIP_T7)
    MVD_CmdArg mvdcmd;

    stSyncCfg.u16Tolerance = u16PTS;

    u16PTS = _MS_TO_90K(u16PTS); //u16PTS ms ==> 90k counter
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = (MS_U8)bEnable;
    mvdcmd.Arg1 = 0;
    mvdcmd.Arg2 = (MS_U8)(u16PTS&0xff);
    mvdcmd.Arg3 = (MS_U8)((u16PTS&0xff00)>>8);

    if (HAL_MVD_MVDCommand( CMD_MVD_FAST_INT, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_MVD_FAST_INT  ) );
        return FALSE;
    }
    #endif
    return TRUE;
}


//------------------------------------------------------------------------------
/// Get size of residual bitstream.
/// @return -size of residual bitstream
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetResidualStreamSize( void )
{
#if defined(CHIP_T7)
    return __MVD_GetResidualStreamSize();
#else
    MS_U32 u32Ret = 0;
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();

    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_VBUFFER_COUNT, &mvdcmd ) == TRUE)
    {
        u32Ret = (((MS_U32)mvdcmd.Arg2) << 19) |
                 (((MS_U32)mvdcmd.Arg1) << 11) |
                 (((MS_U32)mvdcmd.Arg0) << 3);
    }
    else
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\n", CMD_VBUFFER_COUNT  ) );
    }

    return u32Ret;
#endif
}

//------------------------------------------------------------------------------
/// Get address of the first I-frame.
/// @return -address of the first I-frame
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetIsIPicFound()
{
    MVD_FUNC_ENTRY();
#if defined(CHIP_T7)
    return __MVD_GetIsIPicFound();
#else
    return HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_FIRST_I_FOUND);
#endif
}

//------------------------------------------------------------------------------
/// Get sync status to know whether sync is complete or not
/// @return - 1        : sync complete
////        - otherwise: sync not complete
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetSyncStatus(void)
{
#if defined(CHIP_T7)
    return __MVD_GetSyncStatus();
#else
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_GET_SYNC_STAT, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_GET_SYNC_STAT  ) );
        return 0xFF;
    }
    MVD_DEBUGINFO(printf("Sync On/Off %x, Sync Done %x\n", mvdcmd.Arg0, mvdcmd.Arg1));
    if(mvdcmd.Arg1 == MVD_SYNC_DONE)
        return 1;
    else
        return 0xFF;
#endif
}

//------------------------------------------------------------------------------
/// Report if decoder is freerun or not
/// @return - TRUE  : freerun
////        - FALSE : not freerun
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetIsFreerun(void)
{

    return (HAL_MVD_GetAVSyncStatus() == 0);

}

//------------------------------------------------------------------------------
/// Get PTS (Presentation Time Stamp)
/// @return -PTS (unit: ms)
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetPTS(void)
{
#if defined(CHIP_T7)
    return __MVD_GetPTS();
#else
    MS_U32 u32PTS = 0;

    MVD_FUNC_ENTRY();
#if 1
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetPTS error!\n"));
        u32PTS = 0;
    }
    u32PTS = (MS_U32)HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_DISP_PTS);
#else
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_RD_PTS, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR(printf( "Ctrl: CMD_RD_PTS fail!!\n"  ) );
        u32PTS = MVD_U32_MAX;
    }
    else
    {
        u32PTS = ((MS_U32)mvdcmd.Arg3 << 24 | (MS_U32)mvdcmd.Arg2 << 16 |
                  (MS_U32)mvdcmd.Arg1 << 8  | (MS_U32)mvdcmd.Arg0);
        MVD_DEBUGVERBAL(printf("MDrv_MVD_GetPTS:0x%lx\n", u32PTS));
    }
#endif

    u32PTS = _90K_TO_MS(u32PTS);
    return u32PTS;
#endif
}

//------------------------------------------------------------------------------
/// Get Next PTS (Presentation Time Stamp)
/// @return -Next PTS (unit: ms)
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetNextPTS(void)
{
#if defined(CHIP_T2) || defined(CHIP_T7)
    return 0;
#else
    MVD_FUNC_ENTRY();
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetNextPTS error!\n"));
        return 0;
    }
    return (MS_U32)_90K_TO_MS(HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_NEXT_PTS));
#endif
}


//------------------------------------------------------------------------------
/// Get Chroma format (MDrv_MVD_GetChromaFormat)
/// @return -Chroma format ()
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetChromaFormat(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetChromaFormat error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return FALSE;
    }

    return HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_CHROMA_FORMAT);
}

//------------------------------------------------------------------------------
/// Query if MVD is ready to display
/// @return -MS_U8  0: not ready, !0: ready.
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetDispRdy(void)
{
    MVD_FUNC_ENTRY();
#if (defined(CHIP_T7))
    return __MVD_GetDispRdy();
#else
    if ((MDrv_MVD_GetPicCounter() > 0) || MDrv_MVD_GetIsIPicFound())
    {
        return 1;
    }
    else
    {
        return 0;
    }
#endif
}

//------------------------------------------------------------------------------
/// Query if the first frame is showed after play function is called.
/// Whenever the first display frame is displayed, f/w will set this flag to 1.
/// @return VDEC_Result
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_Is1stFrmRdy(void)
{
    if (pu8MVDGetVolBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s err: pu8MVDGetVolBufStart=NULL\n", __FUNCTION__));
        return FALSE;
    }

    return HAL_MVD_MemReadByte(pu8MVDGetVolBufStart + OFFSET_FIRST_DISPLAY);
}

//------------------------------------------------------------------------------
/// Get picture count of current GOP (Group of Picture)
/// @return -picture count of GOP
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetGOPCount(void)
{
    MVD_FUNC_ENTRY();
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetPicCounter error: pu8MVDGetFrameInfoBufStart=NULL\n"));
            return 0;
    }

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_GOP_I_FCNT)
         + HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_GOP_P_FCNT)
         + HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_GOP_B_FCNT);
}

//------------------------------------------------------------------------------
/// Enable or disable dropping error frames
/// @return -command is set successfully or not
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DropErrorFrame(MS_BOOL bDrop)
{
    #if (defined(CHIP_T7))
    MVD_DEBUGINFO( printf( "This Cmd is bypass in MVD1 <%x>!!\r\n", bDrop  ) );
    #else
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg1 = (MS_U8)bDrop;
    mvdcmd.Arg2 = (MS_U8)bDropDispfrm;
    mvdcmd.Arg3 = _eFrcMode;
    if (HAL_MVD_MVDCommand( CMD_DISPLAY_CTL, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_DISPLAY_CTL  ) );
        return FALSE;
    }
    #endif
    bDropErrFrm = bDrop;
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get byte count of parser.
/// @return -byte count of parser
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetParserByteCnt(void)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32Cnt = 0;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);

    //To be accurate, it's "VLD byte count", instead of "parser byte count".
    mvdcmd.Arg0 = 0x34;
    mvdcmd.Arg1 = 0x2;
    if (HAL_MVD_MVDCommand( CMD_RD_IO, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_RD_IO  ) );
        return 0;
    }
    u32Cnt = (((MS_U32)mvdcmd.Arg2)) | ((MS_U32)mvdcmd.Arg3 << 8) |
             (((MS_U32)mvdcmd.Arg4) << 16) | (((MS_U32)mvdcmd.Arg5) << 24);


    //printf("  parser byte count = %lu byte \n", u32Cnt);
    return u32Cnt;
}


//------------------------------------------------------------------------------
/// Get the decode status.
/// @return -the decode status
//------------------------------------------------------------------------------
MVD_DecStat MDrv_MVD_GetDecodeStatus(void)
{
#if defined(CHIP_T7)
    return __MVD_GetDecodeStatus();
#else
    MVD_CmdArg mvdcmd;
    MVD_DecStat stat = E_MVD_STAT_UNKNOWN;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand(CMD_DECODE_STATUS, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DECODE_STATUS ) );
        return E_MVD_STAT_UNKNOWN;
    }

    switch(mvdcmd.Arg1)
    {
        case DEC_STAT_IDLE:
            stat = E_MVD_STAT_IDLE;
            break;
        case DEC_STAT_FIND_SC:
            stat = E_MVD_STAT_FIND_STARTCODE;
            break;
        case DEC_STAT_FIND_SPE_SC:
            stat = E_MVD_STAT_FIND_SPECIALCODE;
            break;
        case DEC_STAT_FIND_FRAMEBUFFER:
            stat = E_MVD_STAT_FIND_FRAMEBUFFER;
            break;
        case DEC_STAT_WAIT_DECODE_DONE:
            stat = E_MVD_STAT_WAIT_DECODEDONE;
            break;
        case DEC_STAT_DECODE_DONE:
            stat = E_MVD_STAT_DECODE_DONE;
            break;
        case DEC_STAT_WAIT_VDFIFO:
            stat = E_MVD_STAT_WAIT_VDFIFO;
            break;
        case DEC_STAT_INIT_SUCCESS:
            stat = E_MVD_STAT_INIT_SUCCESS;
            break;
        default:
            break;
    }

    return stat;
#endif
}


//------------------------------------------------------------------------------
/// Get the last command to firmware.
/// @return -the last command
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetLastCmd(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_DECODE_STATUS, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_DECODE_STATUS ) );
        return 0xff;
    }

    return (mvdcmd.Arg0);
}


//------------------------------------------------------------------------------
/// Skip data in ES buffer.  It is used for flushing ES buffer.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SkipData(void)
{

    #define SKIP_DATA_TIMEOUT 0x20
    MS_U32 u32TimeOut = 0;
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);

    while(MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SKIP_DATA) && (u32TimeOut < SKIP_DATA_TIMEOUT))
    {
        if (HAL_MVD_MVDCommand(CMD_SKIP_DATA, &mvdcmd) == FALSE)
        {
            MVD_DEBUGINFO( printf( "Ctrl: 0x%x fail!!\r\n", CMD_SKIP_DATA ) );
            return FALSE;
        }
        u32TimeOut++;
    }

    if(u32TimeOut >= SKIP_DATA_TIMEOUT)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x timeout(0x%x)!!\r\n", CMD_SKIP_DATA, SKIP_DATA_TIMEOUT) );
        return FALSE;
    }
    return TRUE;

}


//------------------------------------------------------------------------------
/// Skip to I frame.
/// Used for MediaCodec when input data is not continuous or in the beginning of
/// streams.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SkipToIFrame(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand(CMD_START_DEC_STRICT, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_START_DEC_STRICT  ) );
        return FALSE;
    }
    return TRUE;
}


//Map driver FRC (Frame rate conversion) mode to firmware's.
static MS_U8 MVD_MapFrcMode(MVD_FrcMode eFrcMode)
{
    MS_U8 frcMode = 0xf;
    switch (eFrcMode)
    {
        case E_MVD_FRC_NORMAL:
            frcMode = FrcNormal;
            break;
        case E_MVD_FRC_DISP_TWICE:
            frcMode = FrcDisplayTwice;
            break;
        case E_MVD_FRC_3_2_PULLDOWN:    //film 24 -> 50i
            frcMode = Frc32Pulldown;
            break;
        case E_MVD_FRC_PAL_TO_NTSC:
            frcMode = FrcPALtoNTSC;
            break;
        case E_MVD_FRC_NTSC_TO_PAL:
            frcMode = FrcNTSCtoPAL;
            break;
        case E_MVD_FRC_DISP_ONEFIELD:
            frcMode = FrcShowOneFiled;
            break;
        default:
            break;
    }
    return frcMode;
}


//------------------------------------------------------------------------------
/// Display control for decoding order, enabling drop error frame, dropping
/// display and setting FRC (frame rate conversion) mode.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DispCtrl(MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = (MS_U8)bDecOrder;
    mvdcmd.Arg1 = (MS_U8)bDropErr;
    mvdcmd.Arg2 = (MS_U8)bDropDisp;
    mvdcmd.Arg3 = MVD_MapFrcMode(eFrcMode);

    if (HAL_MVD_MVDCommand( CMD_DISPLAY_CTL, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_DISPLAY_CTL  ) );
        return FALSE;
    }

    _eFrcMode = eFrcMode;
    if (bDropErrFrm != bDropErr)
    {
        MVD_DEBUGINFO( printf("bDropErrFrm(%d) != bDropErr(%d)\n", bDropErrFrm, bDropErr));
        bDropErrFrm = bDropErr;
    }
    bDropDispfrm = bDropDisp;

    return TRUE;
}


//------------------------------------------------------------------------------
/// Enable/Disable to repeat the last field when repeat for avsync or display pause.
/// E.g. Top field first: T-B-B-B-...; Bottom field first: B-T-T-T-...
/// Usage scenario: scaler framebufferless mode to avoid frame flicker
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_DispRepeatField(MS_BOOL bEnable)
{
    #if (defined(CHIP_T7))
    MVD_DEBUGINFO( printf( "This Cmd is bypass in MVD1 <%x>!!\r\n", bEnable  ) );
    #else
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    if (HAL_MVD_MVDCommand( CMD_REPEAT_MODE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_REPEAT_MODE  ) );
        return FALSE;
    }
    #endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get RFF (Repeat First Field) flag.
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetTop1stField(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetTop1stField error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return FALSE;
    }

    if (HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_TOP_FF))
        return TRUE;
    else
        return FALSE;
}


//------------------------------------------------------------------------------
/// Get RFF (Repeat First Field) flag.
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetRepeat1stField(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetRepeat1stField error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return FALSE;
    }

    if (HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_REPEAT_FF))
        return TRUE;
    else
        return FALSE;
}

//------------------------------------------------------------------------------
/// Get tmp ref flag.
/// @return -TRUE or FALSE
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetTmpRefField(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetTmpRefField error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return FALSE;
    }

    if (HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_TMP_REF))
        return TRUE;
    else
        return FALSE;
}

//------------------------------------------------------------------------------
/// Get color format.
/// @return - value (0~8)
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetColorFormat(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s error: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return 0xff;
    }

    return (HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_VIDEO_FORMAT));

}

//------------------------------------------------------------------------------
/// Get matrix coefficients in 13818-2 sequence_display_extension().
/// @return - value (0~255)
//------------------------------------------------------------------------------
MS_U8 MDrv_MVD_GetMatrixCoef(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s error: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return 0xff;
    }

    return (HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart + OFFSET_MATRIX_COEF));

}

//------------------------------------------------------------------------------
/// Issue Pause command.
//------------------------------------------------------------------------------
void MDrv_MVD_Pause(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = DISPLAY_PAUSE_ON;
    if (HAL_MVD_MVDCommand(CMD_DISPLAY_PAUSE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_PAUSE) );
        return;
    }

    HAL_MVD_Delayms(4);

    return;
}


//------------------------------------------------------------------------------
/// Issue "Decode Pause" command.
//------------------------------------------------------------------------------
void MDrv_MVD_DecodePause(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand(CMD_PAUSE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_PAUSE) );
        return;
    }

    HAL_MVD_Delayms(4);

    return;
}


//------------------------------------------------------------------------------
/// Issue Resume command.
//------------------------------------------------------------------------------
void MDrv_MVD_Resume(void)
{
    MVD_FUNC_ENTRY();
    if (HAL_MVD_Resume() == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: HAL_MVD_Resume fail!!\r\n" ) );
    }
    return;
}


//------------------------------------------------------------------------------
/// Issue Stop command.
//------------------------------------------------------------------------------
static MS_BOOL MDrv_MVD_Stop(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand(CMD_STOP, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_STOP) );
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/// Is MVD step decode done after step decode command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsStepDecodeDone(void)
{
    MS_BOOL bRet = FALSE;

    MVD_FUNC_ENTRY();

    if (bStepDecode)
    {
        #if (defined(CHIP_T2))||(defined(CHIP_T7))
        MS_U32 u32DecodedFrameCnt = 0;
        u32DecodedFrameCnt = MDrv_MVD_GetPicCounter();
        if (u32StepDecodeCnt != u32DecodedFrameCnt)
        {
            u32StepDecodeCnt = u32DecodedFrameCnt;
            bStepDecode = FALSE;
            bRet = TRUE;
        }
        #else
        if (MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SINGLE_STEP))
        {
            printf( "MVD_HANDSHAKE_SINGLE_STEP -------: 0x%x \n", MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SINGLE_STEP));
            bStepDecode = FALSE;
            bRet = TRUE;
        }
        #endif
    }
    return bRet;
}

//------------------------------------------------------------------------------
/// Issue StepPlay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_StepDecode(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_SINGLE_STEP, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SINGLE_STEP ) );
        return FALSE;
    }

    if (HAL_MVD_Resume() == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: HAL_MVD_Resume fail!!\r\n" ) );
        return FALSE;
    }
    bStepDecode = TRUE;
    return TRUE;
}

//-----------------------------------------------------------------------------
/// Is MVD step display done after step display command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsStepDispDone(void)
{
    MVD_FUNC_ENTRY();

    if (!bStepDisp)
    {
        MVD_DEBUGERROR(printf("%s err: hasn't issue StepDisp\n", __FUNCTION__));
        return FALSE;
    }
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s err: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return FALSE;
    }

    return HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_STEP_DISP_DONE);

}

//------------------------------------------------------------------------------
/// Issue StepDisplay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_StepDisp(void)
{
    MVD_FUNC_ENTRY();
    MDrv_MVD_SetSpeed(E_MVD_SPEED_DEFAULT, 1);
    if (HAL_MVD_StepDisp() == FALSE)
    {
        MVD_DEBUGERROR( printf( "%s fail!!\n", __FUNCTION__ ) );
        return FALSE;
    }
    bStepDisp = TRUE;
    return TRUE;
}

//-----------------------------------------------------------------------------
/// Is MVD step display done after step display command.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): decoding, or user did not send step decode command.
/// @retval     -TRUE(1): decode done
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsStep2PtsDone(void)
{
    MVD_FUNC_ENTRY();

    if (!bStep2Pts)
    {
        return FALSE;
    }

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s err: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return FALSE;
    }

    return HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_STEP_TO_PTS_DONE);

}

//------------------------------------------------------------------------------
/// Step to the specific PTS (u32Pts ms)
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SeekToPTS(MS_U32 u32Pts)
{
    MVD_FUNC_ENTRY();

    MDrv_MVD_SetSpeed(E_MVD_SPEED_DEFAULT, 1);
    bStep2Pts = HAL_MVD_SeekToPTS(_MS_TO_90K(u32Pts));
    return bStep2Pts;
}

//------------------------------------------------------------------------------
/// Skip to the specific PTS (u32Pts ms)
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SkipToPTS(MS_U32 u32Pts)
{
    MVD_FUNC_ENTRY();

    bSkip2Pts = HAL_MVD_SkipToPTS(_MS_TO_90K(u32Pts));
    return bSkip2Pts;
}

//------------------------------------------------------------------------------
/// Issue TrickPlay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_TrickPlay(MVD_TrickDec trickDec, MS_U8 u8DispDuration)
{
    MVD_CmdArg mvdcmd;
    MS_U8 u8DecType;
    MVD_FUNC_ENTRY();

    switch (trickDec)
    {
        case E_MVD_TRICK_DEC_ALL:
            u8DecType = 0;
            break;
        case E_MVD_TRICK_DEC_I:
            u8DecType = 1;
            break;
        case E_MVD_TRICK_DEC_IP:
            u8DecType = 2;
            break;
        default:
            return FALSE;
            break;
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8DecType;
    mvdcmd.Arg1 = u8DispDuration;
    eTrickMode = trickDec;    //for MDrv_MVD_GetTrickMode

    if (HAL_MVD_MVDCommand(CMD_FAST_SLOW, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FAST_SLOW) );
        return FALSE;
    }
    MDrv_MVD_SetAVSync(FALSE, 0);
    return TRUE;
}


//------------------------------------------------------------------------------
/// Get the trick mode which has been set.
/// @return MVD_TrickDec
//------------------------------------------------------------------------------
MVD_TrickDec MDrv_MVD_GetTrickMode(void)
{
    return eTrickMode;
}


//------------------------------------------------------------------------------
/// Flush display buffer.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FlushDisplayBuf(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand(CMD_FLUSH_DISP_QUEUE, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FLUSH_LAST_IPFRAME) );
        return FALSE;
    }
    return TRUE;
}


//------------------------------------------------------------------------------
/// Get capabilities of MPEG Video Decoder.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetCaps(MVD_Caps* pCaps)
{
    MS_U8 caps = HAL_MVD_GetCaps();
    if (!pCaps)
        return FALSE;

    pCaps->bMPEG2 = ((caps & MVD_SUPPORT_MPEG2) == MVD_SUPPORT_MPEG2);
    pCaps->bMPEG4 = ((caps & MVD_SUPPORT_MPEG4) == MVD_SUPPORT_MPEG4);
    pCaps->bVC1   = ((caps & MVD_SUPPORT_VC1) == MVD_SUPPORT_VC1);

    //printf("MP2=%d, MP4=%d, VC1=%d\n", pCaps->bMPEG2, pCaps->bMPEG4, pCaps->bVC1);
    return TRUE;
}


//------------------------------------------------------------------------------
/// Get if MVD decoder is playing.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsPlaying(void)
{
    MS_BOOL bIsPlaying = FALSE;
    bIsPlaying = ( MDrv_MVD_GetDecodeStatus() == E_MVD_STAT_WAIT_DECODEDONE );

    return bIsPlaying;
}

//------------------------------------------------------------------------------
/// Get if MVD decoder is in idle state.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsIdle(void)
{
    return (MDrv_MVD_GetDecodeStatus() == E_MVD_STAT_IDLE);
}


////////////////////////////////////////////////////////////////////////////////
//  FIXME::  Below functions are under construction.
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
/// Enable/Disable error concealment function
/// @param -bDisable \b IN : enable/disable this function
/// @return -return E_MVD_Result success/fail to enable/disable
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_DisableErrConceal(MS_BOOL bDisable)
{
    return E_MVD_RET_FAIL;
}

//------------------------------------------------------------------------------
/// Push queue.
/// @param -u32StAddr \b IN : the start address of the queue
/// @param -u32Size \b IN : the data size to be pushed
/// @param -u32TimeStamp \b IN : the corresponding PTS
/// @return -return E_MVD_Result success/fail to push queue
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_PushQueue(MVD_PacketInfo* pInfo)
{
    E_MVD_Result eRet=E_MVD_RET_INVALID_PARAM;

    //check input parameters
    if (pInfo == NULL)
    {
        MVD_DEBUGERROR(printf("PushQueue NULL pInfo\n"));
        return E_MVD_RET_INVALID_PARAM;
    }

#if _SLQTBL_DUMP_PUSHQ
{
    static MS_U32 u32pqTimes = 0;

    printf("Push[%lx] st=0x%lx len=0x%lx pts=0x%lx\n", u32pqTimes++,
            pInfo->u32StAddr, pInfo->u32Length, pInfo->u32TimeStamp);
}
#endif
    MVD_FUNC_ENTRY();
    #if SLQ_NEW_PUSH
    if((pInfo->u32StAddr & SLQ_PIC_START_FLAG) == SLQ_PIC_START_FLAG)
    {
        pInfo->u32StAddr = pInfo->u32StAddr&~SLQ_PIC_START_FLAG;
        _SlqStatus.bSlqPicStart =TRUE;
        _SlqStatus.bSlqCtrlBit = TRUE;
    }
    else
    {
        _SlqStatus.bSlqPicStart = FALSE;
    }
#endif
    //check input parameters
    if (pInfo == NULL)
    {
        MVD_DEBUGERROR(printf("PushQueue NULL pInfo\n"));
        return E_MVD_RET_INVALID_PARAM;
    }
#if 0
    else if ((pInfo->u32TimeStamp!= 0) && (pInfo->u32Length==0))
    {
        printf("PushQueue invalid pInfo pts=0x%lx\n", pInfo->u32TimeStamp);
        eRet=E_MVD_RET_INVALID_PARAM;
    }
#endif
    else if (pInfo->u32StAddr >= stMemCfg.u32BSSize)
    {
        //since u32StAddr is offset, it shouldn't be larger than size.
        MVD_DEBUGERROR(printf("PushQueue invalid u32StAddr=0x%lx, bsSize=0x%lx\n", pInfo->u32StAddr, stMemCfg.u32BSSize));
        return E_MVD_RET_INVALID_PARAM;
    }
    else if ((pInfo->u32TimeStamp == MVD_NULLPKT_PTS) && (pInfo->u32Length==0))
    {
        // AVI NULL packet.
        u32DummyPktCnt++;
        //printf("Pos:0x%x%08x; PTS:%08d; NullPKT:%d\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp, u32DummyPktCnt);
        return E_MVD_RET_OK;
    }

    if (FALSE == MDrv_MVD_IsMStreamerMode())
    {   //Check repeat PTS for non-MStreamer mode.
        //printf(".Pos:0x%x%08x; PTS:%08d; NullPKT:%d\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp, u32DummyPktCnt);
        if (pInfo->u32TimeStamp == u32LastPts)
        {
            //printf("Repeat PTS!\n");
            if (pInfo->u32TimeStamp != MVD_NULLPKT_PTS)
                pInfo->u32TimeStamp = MVD_NULLPKT_PTS; //repeat PTS
        }
        else
        {
            u32LastPts = pInfo->u32TimeStamp;
        }
    }

    //check queue vacancy
    if (_drvSlqTbl.u32Empty >= SLQ_TBL_SAFERANGE)
    {   //put packets
#if SLQ_NEW_PUSH
        if (E_MVD_CODEC_DIVX311 == curCodecType
            ||E_MVD_CODEC_VC1_MAIN == curCodecType //rcv
            ||E_MVD_CODEC_VC1_ADV == curCodecType)
        {
            MVD_PacketInfo stHdr;
            if((_SlqStatus.bSlqCtrlBit) || (E_MVD_CODEC_DIVX311 == curCodecType))
            {
                MVD_SLQTblGetHdrPkt(&stHdr, pInfo);

                if(_SlqStatus.bSlqPicStart||(!_SlqStatus.bSlqCtrlBit))
                {
                    if (MDrv_MVD_IsMStreamerMode())
                    {   //to mark this packet's pts as unused.
                        stHdr.u32TimeStamp = MVD_NULLPKT_PTS;
                        stHdr.u32ID_H = MVD_NULLPKT_PTS;
                    }
                    MVD_SLQTblSendPacket(&stHdr);
                }
            }
        }
#else
        if (E_MVD_CODEC_DIVX311 == curCodecType)
        {
            MVD_PacketInfo stDivxHdr;
            MVD_SLQTblGetDivxHdrPkt(&stDivxHdr, pInfo);
            if (MDrv_MVD_IsMStreamerMode())
            {   //to mark this packet's pts as unused.
                stDivxHdr.u32TimeStamp = MVD_NULLPKT_PTS;
                stDivxHdr.u32ID_H = MVD_NULLPKT_PTS;
            }
            MVD_SLQTblSendPacket(&stDivxHdr);
        }
#endif
        MVD_SLQTblSendPacket(pInfo);
        eRet=E_MVD_RET_OK;
        bSlqTblHasValidData = TRUE;
    }
    else
    {
        MS_ASSERT(0); //shouldn't be here!
        MVD_DEBUGERROR(printf("PushQueue FULL!!! empty=0x%lx\n", _drvSlqTbl.u32Empty));
        //Player will only push queue when queue vacancy != 0
        eRet=E_MVD_RET_QUEUE_FULL;
    }

    if (E_MVD_RET_OK != eRet)
    {
        MVD_DEBUGERROR(printf("%s ret = %d\n", __FUNCTION__, eRet));
    }
    return eRet;
}


static E_MVD_Result MVD_FlushTSQueue(void)
{
    MS_U32 u32TimeOut = 10;
    static MS_BOOL bSetSkip = FALSE;

    if (MDrv_MVD_GetLastCmd()!=CMD_PAUSE)
    {
        MDrv_MVD_Pause();
        MDrv_MVD_DecodePause();
        MDrv_MVD_FlushDisplayBuf();
    }
    if (!bSetSkip)
    {
        MDrv_MVD_SkipData();
        bSetSkip = TRUE;
    }

    while (u32TimeOut>0)
    {
        HAL_MVD_Delayms(5);

        if ( (MDrv_MVD_GetDecodeStatus()==E_MVD_STAT_FIND_SPECIALCODE)
              && (MDrv_MVD_GetLastCmd()==CMD_PAUSE) )
        {
            break;
        }
        else if (MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SKIP_DATA))
        {
            break;
        }

        u32TimeOut--;
    }

    if (u32TimeOut==0)
    {
        MVD_DEBUGVERBAL(printf("\n***** MVD_FlushTSQueue TS flush TIMEOUT!!! *****\n\n"));
        return E_MVD_RET_FAIL;
    }

    bSetSkip = FALSE;
    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Flush PVR queue: flush ES buffer, reset SLQ TABLE, flush decoded frame, and
///              keep the display frame.
/// @return -return E_MVD_Result success/fail to flush queue
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_PVRFlushDispQueue(MS_BOOL bEnable)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MVD_FUNC_ENTRY();


    if(!bEnable)
    {
        return E_MVD_RET_FAIL;
    }
    if (MDrv_MVD_GetDecodeStatus() == E_MVD_STAT_IDLE)
    {
        return eRet;
    }

    //flush Frame buffer only
    if (MDrv_MVD_GetLastCmd()!=CMD_PAUSE)
    {
        MDrv_MVD_Pause();
        MDrv_MVD_DecodePause();
        MDrv_MVD_FlushDisplayBuf();
    }
    else
    {
        MDrv_MVD_FlushDisplayBuf();
    }

    MVD_RstFrmInfo(E_MVD_FRMINFO_DECODE);

    if (TRUE == MDrv_MVD_SkipToIFrame())
    {
       return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }
}

static MS_BOOL MVD_PatternLenIsValid(MS_U32 u32Len)
{

    MS_U32 u32ValidLen = 0;
#define MAX_VALIDLEN    0x200000    //2M

    if (E_MVD_CODEC_VC1_MAIN != curCodecType)
    {
        return TRUE;
    }
    else
    {
        //only RCV has to check this
        u32ValidLen = HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_RCV_PAYLOAD_LENGTH);
        if (u32ValidLen > MAX_VALIDLEN)
        {   //avoid the extreme large value due to error bitstream
            u32ValidLen = MAX_VALIDLEN;
        }
        MVD_DEBUGVERBAL(printf("(%x) ValidLen=0x%lx CurLen=0x%lx\n",
                       (u32Len > u32ValidLen), u32ValidLen, u32Len));
        return (u32Len > u32ValidLen);
    }
}


static E_MVD_Result MVD_FlushSlqTblQueue(void)
{
    MS_U32 u32TimeCnt, u32TimeOut;
    MS_U32 u32PatternByteCnt = 0;

    MDrv_MVD_Pause();
    MDrv_MVD_DecodePause();
    MDrv_MVD_FlushDisplayBuf();

    u32TimeCnt = HAL_MVD_GetTime();
    while (((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS) ||
           (TRUE != MVD_PatternLenIsValid(u32PatternByteCnt)))
    {
        if (MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_PAUSE))
        {
            MVD_DEBUGVERBAL(printf("\nPause finished!\n"));
            MDrv_MVD_FlushDisplayBuf();
            break;
        }

        if (_drvSlqTbl.u32Empty < SLQ_TBL_SAFERANGE)
        {
            MDrv_MVD_GetQueueVacancy(FALSE); //update _drvSlqTbl.u32Empty
            HAL_MVD_Delayms(1); //avoid busy query
        }
        //insert dummy pattern
        if (TRUE == MVD_SLQTblInsertPattern(E_MVD_PATTERN_FLUSH))
        {
            u32PatternByteCnt += DUMMY_SIZE*2; //2 dummy were inserted
        }
    }
    MVD_DEBUGVERBAL(printf("====> %s (t1=%lu t2=%lu diff=%lu)\n", __FUNCTION__,
            u32TimeCnt, HAL_MVD_GetTime(), (HAL_MVD_GetTime() - u32TimeCnt)));
    //if ((HAL_MVD_GetTime() - u32TimeCnt) >= CMD_TIMEOUT_MS)
    if (TRUE != MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_PAUSE))
    {
        MVD_DEBUGERROR(printf("\n***** MDrv_MVD_FlushQueue PAUSE TIMEOUT!!! *****\n\n"));
        MVD_DEBUGERROR(printf("ValidLen=0x%lx CurPatternLen=0x%lx\n",
                      HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+92), u32PatternByteCnt));

        return E_MVD_RET_FAIL;
    }

    //flush ES buffer & reset SLQ tbl
    if (HAL_MVD_SlqTblRst() == TRUE)
    {
        //return E_MVD_RET_OK;
    }

    u32TimeOut = 10;
    while (u32TimeOut>0)
    {
        if (MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SLQ_RST))
        {
            //printf("\nSlqRst finished! x=0x%lx\n", x);
            break;
        }
        u32TimeOut--;
        HAL_MVD_Delayms(10);
    }
    //printf("\n\n=====> SlqRst u32TimeOut =%lx\n", u32TimeOut);
    if (u32TimeOut==0)
    {
        MVD_DEBUGERROR(printf("\n***** MDrv_MVD_FlushQueue SlqRst TIMEOUT!!! *****\n\n"));
        return E_MVD_RET_FAIL;
    }

    MVD_SLQTblInit(); //reset related buffers

    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Flush PTS buffer.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FlushPTSBuf(void)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    if (HAL_MVD_MVDCommand(CMD_SEND_UNI_PTS, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SEND_UNI_PTS) );
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// Flush queue: flush ES buffer, reset SLQ TABLE, flush decoded frame, and
///              keep the display frame.
/// @return -return E_MVD_Result success/fail to flush queue
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_FlushQueue(void)
{
    E_MVD_Result eRet = E_MVD_RET_OK;

    MVD_FUNC_ENTRY();

    if (MDrv_MVD_GetDecodeStatus() == E_MVD_STAT_IDLE)
    {
        return eRet;
    }

    //flush ES buffer (and cmd queue if used)
    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
        eRet = MVD_FlushSlqTblQueue();
    }
    else if (E_MVD_TS_FILE_MODE == curSrcMode)
    {
        eRet = MVD_FlushTSQueue();
    }
    if (E_MVD_RET_OK != eRet)
    {
        return eRet;
    }

    MVD_RstFrmInfo(E_MVD_FRMINFO_DECODE);
    //flush display buffer
    if (MDrv_MVD_FlushDisplayBuf() != TRUE)
    {
        return E_MVD_RET_FAIL;
    }

    if (TRUE == MDrv_MVD_SkipToIFrame())
    {
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }
}

static MS_U32 MVD_Map2DrvSlqTbl(MS_U32 u32HWPtr)
{
    MS_U32 u32HWSt = MVD_GetMemOffset(stMemCfg.u32BSAddr);
    MS_U32 u32DrvPtr;

    if ((u32HWPtr<u32HWSt) && (u32HWPtr!=0))
    {
        MVD_DEBUGERROR(printf("Invalid u32HWPtr=0x%lx\n", u32HWPtr));
        return 0;
    }
    if ((bSlqTblSync) && (u32HWPtr!=0))
    {
        u32DrvPtr = _drvSlqTbl.u32StAdd + (u32HWPtr - u32HWSt);
        return u32DrvPtr;
    }
    return u32HWPtr;
}


//------------------------------------------------------------------------------
/// Get the queue vacancy (unit in slq entry).
/// @param -pQueueVacancy \b IN : pointer to the queue vacancy
/// @return -return E_MVD_Result success/fail to get the queue vacancy
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetQueueVacancy(MS_BOOL bCached)
{
    MS_U32 u32Empty;

#if 0
    if (MDrv_MVD_GetVldErrCount()!=0)
    {
        printf("QQQ wPtr= 0x%lx(0x%lx) rPtr=0x%lx(0x%lx) vldErr=0x%lx\n", writePtrLast, _drvSlqTbl.u32WrPtr,
        MDrv_MVD_GetSLQReadPtr(), _drvSlqTbl.u32RdPtr, MDrv_MVD_GetVldErrCount());
        printf("Previous EsRead=0x%lx EsWrite=0x%lx\n", u32PreEsRd, u32PreEsWr);
        _SLQTbl_DumpPtsTbl(0x0, 0x620);
        while(1);
    }
#endif

    u32Empty = _drvSlqTbl.u32Empty;
    if ((TRUE == bCached) && (u32Empty > SLQTBL_CHECKVACANCY_WATERLEVEL))
    {
        //To have better performance, we only query F/W read pointer
        //when queue_vacancy is lower than water_level
        u32Empty -= SLQ_TBL_SAFERANGE;
        return (u32Empty / SLQ_ENTRY_LEN);
    }

    _drvSlqTbl.u32RdPtr = MVD_Map2DrvSlqTbl(MDrv_MVD_GetSLQReadPtr());
    //printf("QV=0x%lx rd=0x%lx==>", u32Empty, _drvSlqTbl.u32RdPtr);
    if (_drvSlqTbl.u32RdPtr >= (_drvSlqTbl.u32StAdd+SLQ_ENTRY_LEN))
    {
        if (_drvSlqTbl.u32RdPtr >= _drvSlqTbl.u32EndAdd)
        {
            MVD_DEBUGERROR(printf("%s: readPtr 0x%lx out of range: too large!\n",
                           __FUNCTION__, _drvSlqTbl.u32RdPtr));
        }
        _drvSlqTbl.u32RdPtr -= SLQ_ENTRY_LEN;
    }
    else if (_drvSlqTbl.u32RdPtr == _drvSlqTbl.u32StAdd)
    {
        _drvSlqTbl.u32RdPtr = _drvSlqTbl.u32EndAdd - SLQ_ENTRY_LEN;
    }
    else
    {
        MVD_DEBUGERROR(printf("%s: readPtr 0x%lx out of range: too small!\n",
                       __FUNCTION__, _drvSlqTbl.u32RdPtr));
        _drvSlqTbl.u32RdPtr = _drvSlqTbl.u32StAdd;
    }
    //printf("0x%lx\n", _drvSlqTbl.u32RdPtr);

    if (_drvSlqTbl.u32WrPtr > _drvSlqTbl.u32RdPtr)
    {
        u32Empty = SLQ_TBL_SIZE - (_drvSlqTbl.u32WrPtr - _drvSlqTbl.u32RdPtr);
    }
    else
    {
        u32Empty = _drvSlqTbl.u32RdPtr - _drvSlqTbl.u32WrPtr;
    }

    if (u32Empty == 0)// && (_drvSlqTbl.u32WrPtr == _drvSlqTbl.u32StAdd))
    {
        u32Empty = SLQ_TBL_SIZE;
    }

    _drvSlqTbl.u32Empty = u32Empty;

    if (u32Empty < SLQ_TBL_SAFERANGE)
    {//to avoid write_pointer catch up to read_pointer
        u32Empty= 0;
    }
    else
    {
        u32Empty -= SLQ_TBL_SAFERANGE;
    }

    //printf("%s r=0x%lx w=0x%lx u32Empty=0x%lx\n", __FUNCTION__,
    //        _drvSlqTbl.u32RdPtr, _drvSlqTbl.u32WrPtr, u32Empty);
    return (u32Empty / SLQ_ENTRY_LEN);
}


#define _IsNotInStreamBuff(x)                                       \
        (((x) < stMemCfg.u32DrvBufSize) ||   \
         ((x) > stMemCfg.u32BSSize) )

//------------------------------------------------------------------------------
/// Get read pointer in ElementaryStream buffer for SLQ table mode
/// @return -the read pointer
//------------------------------------------------------------------------------
static MS_U32 MVD_GetSlqTblESReadPtr(void)
{
    MS_U32 u32Idx;
    MS_U32 u32SlqRp = 0, u32SlqRp1 = 0;//_drvSlqTbl.u32RdPtr;
    MS_U32 u32EsRp;
    MS_U32 u32EsStart;

    u32SlqRp1 = MDrv_MVD_GetSLQReadPtr();

    if (u32SlqRp1 == 0)
    {
        return MVD_U32_MAX;
    }
    else
    {
        u32SlqRp = MVD_Map2DrvSlqTbl(u32SlqRp1);
    }

    //report (readPtr-1) for HW may still use (readPtr)
    if (u32SlqRp > (_drvSlqTbl.u32StAdd))
    {
        u32Idx = ((u32SlqRp - _drvSlqTbl.u32StAdd)/SLQ_ENTRY_LEN) - 1;
    }
    else
    {
        u32Idx = SLQ_ENTRY_MAX - 1;
    }
    u32EsRp = _drvEsTbl.u32StAdd + u32Idx*8;

    u32EsStart = HAL_MVD_MemRead4Byte(u32EsRp); //report StartAdd as read_pointer
#if 0
    MS_U32 u32EsEnd;
    u32EsEnd = HAL_MVD_MemRead4Byte(u32EsRp+4);
    printf("GetESReadPtr ES[%lx] Start=0x%lx End=0x%lx u32EsRp=%lx u32SlqRp=%lx\n",
            u32Idx, HAL_MVD_MemRead4Byte(u32EsRp), u32EsEnd, u32EsRp, u32SlqRp);
#endif

    if ((_IsNotInStreamBuff(u32EsStart)) && (u32EsStart != 0))
    {   //ESRead is not in BS buffer, so this entry is a divx or dummy pattern.
        //Report the last ESRead, instead of this one.
        MVD_DEBUGINFO(printf("0x%lx Not in BS, report u32PreEsRd=0x%lx\n", u32EsStart, u32PreEsRd));
        return u32PreEsRd;
    }
    u32PreEsRd = u32EsStart;

    return u32EsStart;
}

//------------------------------------------------------------------------------
/// Get write pointer in ElementaryStream buffer for SLQ table mode
/// @return -the read pointer
//------------------------------------------------------------------------------
static MS_U32 MVD_GetSlqTblESWritePtr(void)
{
    MS_U32 u32EsWp;
    MS_U32 u32EsEnd;
    MS_U32 u32Idx;

    if (_drvSlqTbl.u32WrPtr > (_drvSlqTbl.u32StAdd))
    {
        u32Idx = ((_drvSlqTbl.u32WrPtr - _drvSlqTbl.u32StAdd)/SLQ_ENTRY_LEN) - 1;
    }
    else
    {
        u32Idx = SLQ_ENTRY_MAX - 1;
    }
    u32EsWp = _drvEsTbl.u32StAdd + u32Idx*8;

    u32EsEnd = HAL_MVD_MemRead4Byte(u32EsWp+4);
#if 0
    printf("GetESWritePtr[%lx] ES Start=0x%lx End=0x%lx u32EsWp=%lx\n",
        (u32EsWp - _drvEsTbl.u32StAdd)/8,
        HAL_MVD_MemRead4Byte(u32EsWp), u32EsEnd, u32EsWp);
#endif

    if ((_IsNotInStreamBuff(u32EsEnd)) && (u32EsEnd != 0))
    {   //ESRead is not in BS buffer, so this entry is a divx pattern.
        //Report the last ESRead, instead of this one.
        MVD_DEBUGINFO(printf("0x%lx Not in BS, report u32PreEsWr=0x%lx\n", u32EsEnd, u32PreEsWr));
        return u32PreEsWr;
    }
    u32PreEsWr = u32EsEnd;

    return u32EsEnd;
}

//------------------------------------------------------------------------------
/// Get ES read address for TS file mode.
/// @return ES read address
//------------------------------------------------------------------------------
static MS_U32 MVD_GetTsFileESReadPtr(void)
{
    MS_U32 u32Add = 0;
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 2;    //ES diff
    if (HAL_MVD_MVDCommand( CMD_PARSER_READ_POSITION, &mvdcmd ) == TRUE)
    {
        //in order to latch the newest parser status
        //u32Diff = (((MS_U32)mvdcmd.Arg3) <<24) | (((MS_U32)mvdcmd.Arg2) <<16) |
        //          (((MS_U32)mvdcmd.Arg1) << 8) | (((MS_U32)mvdcmd.Arg0));
    }
    else
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\n", CMD_PARSER_READ_POSITION) );
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    if (HAL_MVD_MVDCommand( CMD_PARSER_READ_POSITION, &mvdcmd ) == TRUE)
    {
        u32Add = (((MS_U32)mvdcmd.Arg3) <<24) |
                 (((MS_U32)mvdcmd.Arg2) <<16) |
                 (((MS_U32)mvdcmd.Arg1) << 8) |
                 (((MS_U32)mvdcmd.Arg0));
    }
    else
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\n", CMD_PARSER_READ_POSITION) );
    }

    return (u32Add*8);

}

//------------------------------------------------------------------------------
/// Get ES write address for TS file mode.
/// @return ES write address
//------------------------------------------------------------------------------
static MS_U32 MVD_GetTsFileESWritePtr(void)
{
    MS_U32 u32Diff = 0;
    MS_U32 u32WrPtr = 0;
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 2;    //ES diff
    if (HAL_MVD_MVDCommand( CMD_PARSER_READ_POSITION, &mvdcmd ) == TRUE)
    {
        u32Diff = (((MS_U32)mvdcmd.Arg3) <<24) | (((MS_U32)mvdcmd.Arg2) <<16) |
                  (((MS_U32)mvdcmd.Arg1) << 8) | (((MS_U32)mvdcmd.Arg0));
    }
    else
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\n", CMD_PARSER_READ_POSITION) );
    }

    u32WrPtr = u32Diff*8 + MVD_GetTsFileESReadPtr();
    if (u32WrPtr > u32ESBuffEnd)
    {
        MVD_DEBUGVERBAL(printf("ES wrapping Wr=0x%lx ==> ", u32WrPtr));
        u32WrPtr -= stMemCfg.u32BSSize;
        MVD_DEBUGVERBAL(printf("0x%lx\n", u32WrPtr));
    }
    return u32WrPtr;

}

//------------------------------------------------------------------------------
/// Get read pointer in ElementaryStream buffer
/// @return -the read pointer
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetESReadPtr(void)
{
    MS_U32 u32ESR = 0;
    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
        u32ESR = MVD_GetSlqTblESReadPtr();
    }
    else if (E_MVD_TS_FILE_MODE == curSrcMode || E_MVD_TS_MODE == curSrcMode)
    {
        u32ESR = MVD_GetTsFileESReadPtr();
    }
    return u32ESR;
}


//------------------------------------------------------------------------------
/// Get write pointer in ElementaryStream buffer
/// @return -the read pointer
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetESWritePtr(void)
{
    MS_U32 u32ESW = 0;
    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
        u32ESW = MVD_GetSlqTblESWritePtr();
    }
    else if (E_MVD_TS_FILE_MODE == curSrcMode || E_MVD_TS_MODE == curSrcMode)
    {
        u32ESW = MVD_GetTsFileESWritePtr();
    }
    return u32ESW;
}


//------------------------------------------------------------------------------
/// Enable/Disable driver to show the last frame
/// @param -bEnable \b IN : enable/disable this function
/// @return -return E_MVD_Result success/fail to enable/disable
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_EnableLastFrameShow(MS_BOOL bEnable)
{
    MVD_FUNC_ENTRY();

    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
    #if SLQ_NEW_PUSH
        if(_SlqStatus.bSlqCtrlBit)
        {
            _drvSlqTbl.u32WrPtr = _SlqStatus.u32VaildWptrAddr + SLQ_ENTRY_LEN;
        }
    #endif

    #if UPDATE_NEXT_WRPTR
        u32FileEndPtr = _drvSlqTbl.u32WrPtr;
    #else
        //save current writePtr
        if (_drvSlqTbl.u32WrPtr != _drvSlqTbl.u32StAdd)
        {
            u32FileEndPtr = _drvSlqTbl.u32WrPtr - SLQ_ENTRY_LEN;
        }
        else
        {
            if (_drvSlqTbl.u32WrPtr != _drvSlqTbl.u32RdPtr)
            {
                u32FileEndPtr = _drvSlqTbl.u32EndAdd - SLQ_ENTRY_LEN;
            }
            else
            {
                //R==W==ST indicates SlqTbl may just be reset
                u32FileEndPtr = _drvSlqTbl.u32StAdd;
            }
        }
    #endif
        MVD_DEBUGVERBAL(printf("fe=%lx, rd=%lx, wr=%lx\n", u32FileEndPtr,
                        _drvSlqTbl.u32RdPtr, _drvSlqTbl.u32WrPtr));
    }

    if (HAL_MVD_EnableLastFrameShow(bEnable) == FALSE)
    {
        MVD_DEBUGERROR(printf("%s fail!!\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }
    bEnableLastFrmShow = bEnable;
    return E_MVD_RET_OK;
}

#define FLAG_LAST_FRM_SHOW (HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_CMD_LAST_FRAME_SHOW))
//------------------------------------------------------------------------------
/// Get if MVD decoder finish display.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_IsDispFinish(void)
{
    MS_U32 u32TimeCnt;
    MS_U32 u32FeByteCnt = 0;

    MVD_FUNC_ENTRY();

    //printf("MDrv_MVD_IsDispFinish::");
    if (bEnableLastFrmShow != TRUE)
    {
        MVD_DEBUGINFO(printf("%s: bEnableLastFrmShow!=TRUE\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s err: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //printf("0x%x\n", FLAG_LAST_FRM_SHOW);
    if ((E_MVD_SLQ_TBL_MODE == curSrcMode) && (TRUE != FLAG_LAST_FRM_SHOW))
    {
#if (!UPDATE_NEXT_WRPTR)
        //insert pattern when each time checking IsDispFinish
        if (MVD_SLQTblInsertPattern(E_MVD_PATTERN_FILEEND))
        {
            u32FeByteCnt += END_PATTERN_SIZE;
        }
#endif
        if (u32FileEndPtr == MVD_Map2DrvSlqTbl(MDrv_MVD_GetSLQReadPtr()))
        {
            //insert padding pattern until timeout
            u32TimeCnt= HAL_MVD_GetTime();
            while ((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS)
            {
                if (TRUE == FLAG_LAST_FRM_SHOW)
                {
                    //printf("\nDisp finished!\n");
                    break;
                }
                //insert file-end pattern again
                if (MVD_SLQTblInsertPattern(E_MVD_PATTERN_FILEEND))
                {
                    u32FeByteCnt += END_PATTERN_SIZE;
                }
            }
            if ((HAL_MVD_GetTime() - u32TimeCnt) >= CMD_TIMEOUT_MS)
            {
                MVD_DEBUGERROR(printf("\n***** MDrv_MVD_IsDispFinish TIMEOUT!!! *****\n\n"));
                if (E_MVD_CODEC_VC1_MAIN == curCodecType)
                {
                    if ((HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_RCV_PAYLOAD_LENGTH) > 0x200000)
                        && (TRUE != HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_MEET_FILE_END_SC)))
                    {
                        MVD_DEBUGERROR(printf("RCV payloadLen(0x%lx) invalid!\n",
                            HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_RCV_PAYLOAD_LENGTH)));
                    }
                }
                MVD_DEBUGERROR(printf("***** fe=%lx, rd=%lx(%lx,%lx), wr=%lx, empty=%lx, u32FeByteCnt=%lx\n",
                    u32FileEndPtr, _drvSlqTbl.u32RdPtr, MVD_Map2DrvSlqTbl(MDrv_MVD_GetSLQReadPtr()),
                    MDrv_MVD_GetSLQReadPtr(), _drvSlqTbl.u32WrPtr, _drvSlqTbl.u32Empty, u32FeByteCnt));
                return E_MVD_RET_TIME_OUT;
            }
            else
            {
                return E_MVD_RET_OK;
            }
        }
        else
        {
            //just return fail if readPtr is not closed to file-end ptr
            MVD_DEBUGVERBAL(printf("fe=%lx, rd=%lx(%lx), wr=%lx, empty=%lx\n", u32FileEndPtr, _drvSlqTbl.u32RdPtr,
                            MVD_Map2DrvSlqTbl(MDrv_MVD_GetSLQReadPtr()), _drvSlqTbl.u32WrPtr, _drvSlqTbl.u32Empty));
            return E_MVD_RET_FAIL;
        }
    }

    if (FLAG_LAST_FRM_SHOW)
    {
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }

}

//------------------------------------------------------------------------------
/// Set speed.
/// @param -eSpeedType \b IN : specify the speed
/// @param -u8Multiple \b IN :
/// @return -return E_MVD_Result success/fail to set the speed
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetSpeed(MVD_SpeedType eSpeedType, MS_U8 u8Multiple)
{
    static MVD_AVSyncCfg stNFPSyncCfg;

    if ((u8Multiple > 32) || (eSpeedType==E_MVD_SPEED_TYPE_UNKNOWN))
    {
        MVD_DEBUGINFO(printf("%s: invalid para!\n", __FUNCTION__));
        return E_MVD_RET_INVALID_PARAM;
    }

    if (ePreSpeedType == E_MVD_SPEED_DEFAULT)
    {   //save avsync config when normal play for restoring it later
        stNFPSyncCfg.bEnable = stSyncCfg.bEnable;
        stNFPSyncCfg.u32Delay = stSyncCfg.u32Delay;
        stNFPSyncCfg.u16Tolerance = stSyncCfg.u16Tolerance;
        MVD_DEBUGINFO(printf("MDrv_MVD_SetSpeed save avsync: bEnable=0x%x u32Delay=0x%lx u16Tolerance=0x%x\n",
                  stNFPSyncCfg.bEnable, stNFPSyncCfg.u32Delay, stNFPSyncCfg.u16Tolerance));
    }

    if (E_MVD_SPEED_DEFAULT != eSpeedType) //fast or slow forward
    {
        //disable avsyn when fast/slow forward
        MDrv_MVD_SetAVSync(FALSE, 0);
    }

    if (HAL_MVD_SetSpeed(eSpeedType, u8Multiple) == TRUE)
    {
        if (E_MVD_SPEED_DEFAULT == eSpeedType) //Normal Play
        {
            if (ePreSpeedType != E_MVD_SPEED_DEFAULT)
            {
                //set AVSync again for firmware doesn't remember AVSync settings after FF
                MVD_DEBUGINFO(printf("MDrv_MVD_SetSpeed reset avsync: bEnable=0x%x u32Delay=0x%lx u16Tolerance=0x%x\n",
                              stNFPSyncCfg.bEnable, stNFPSyncCfg.u32Delay, stNFPSyncCfg.u16Tolerance));

                MDrv_MVD_SetAVSync(stNFPSyncCfg.bEnable, stNFPSyncCfg.u32Delay);
                if ((stNFPSyncCfg.u16Tolerance!=0) && (FALSE == MDrv_MVD_ChangeAVsync(stNFPSyncCfg.bEnable, stNFPSyncCfg.u16Tolerance)))
                {
                    return E_MVD_RET_FAIL;
                }
            }
        }

        ePreSpeedType = eSpeedType;
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
   }
}

//------------------------------------------------------------------------------
/// Set PTS base to MVD F/W
/// @param -u32pts \b IN : pts unit in 90k counter
//------------------------------------------------------------------------------
static void MVD_SetPTSBase(MS_U32 u32pts)
{
    MVD_CmdArg mvdcmd;

    SET_CMDARG(mvdcmd, u32pts);
    if (HAL_MVD_MVDCommand( CMD_PTS_BASE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_PTS_BASE ) );
    }

    return;
}

//------------------------------------------------------------------------------
/// Reset Presentation Time Stamp according to u32PtsBase
/// @param -u32PtsBase \b IN : the PTS base specified by user
/// @return -return E_MVD_Result success/fail to reset PTS base
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_ResetPTS(MS_U32 u32PtsBase)
{
    _mvdCmdQ.u32PtsBase = _MS_TO_90K(u32PtsBase);
    MVD_SetPTSBase(_mvdCmdQ.u32PtsBase);
    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Get if sequence change (width/height/framerate/interlace) occurs.
/// @return TRUE or FALSE
///     - TRUE, Yes
///     - FALSE, No
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsSeqChg(void)
{
    static MVD_FrameInfo stFrmInfo;

    if (
#ifdef MVD_ENABLE_ISR
        (eCurEvent & E_MVD_EVENT_SEQ_FOUND) ||
#endif
        (MDrv_MVD_GetDispRdy())// && (TRUE == MDrv_MVD_GetValidStreamFlag()))
        )
    {
        MDrv_MVD_GetFrameInfo(&stFrmInfo);

        if (FALSE == stMemCfg.bEnableDynScale)
        {
            if((stPreFrmInfo.u16HorSize   != stFrmInfo.u16HorSize) ||
               (stPreFrmInfo.u16VerSize   != stFrmInfo.u16VerSize) ||
               (stPreFrmInfo.u32FrameRate != stFrmInfo.u32FrameRate) ||
               (stPreFrmInfo.u8Interlace  != stFrmInfo.u8Interlace))
            {
#if 0
                (printf( "MDrv_MVD_IsSeqChg::Previous\n" ));
                (printf( "H=%u\n", stPreFrmInfo.u16HorSize ));
                (printf( "V=%u\n", stPreFrmInfo.u16VerSize ));
                (printf( "F=%lu\n", stPreFrmInfo.u32FrameRate ));
                (printf( "A=%u\n", stPreFrmInfo.u8AspectRate ));
                (printf( "I=%u\n", stPreFrmInfo.u8Interlace ));
#endif
#if 0
                (printf( "MDrv_MVD_IsSeqChg\n" ));
                (printf( "H=%u\n", stFrmInfo.u16HorSize ));
                (printf( "V=%u\n", stFrmInfo.u16VerSize ));
                (printf( "F=%lu\n", stFrmInfo.u32FrameRate ));
                (printf( "A=%u\n", stFrmInfo.u8AspectRate ));
                (printf( "I=%u\n", stFrmInfo.u8Interlace ));
#endif

                //memcpy(&stPreFrmInfo, &stFrmInfo, sizeof(MVD_FrameInfo));
                stPreFrmInfo.u16HorSize   = stFrmInfo.u16HorSize;
                stPreFrmInfo.u16VerSize   = stFrmInfo.u16VerSize;
                stPreFrmInfo.u8AspectRate = stFrmInfo.u8AspectRate;
                stPreFrmInfo.u32FrameRate = stFrmInfo.u32FrameRate;
                stPreFrmInfo.u8Interlace  = stFrmInfo.u8Interlace;
#if 0
                (printf( "MDrv_MVD_IsSeqChg===>\n" ));
                (printf( "H=%u\n", stFrmInfo.u16HorSize ));
                (printf( "V=%u\n", stFrmInfo.u16VerSize ));
                (printf( "F=%u\n", stFrmInfo.u32FrameRate ));
                (printf( "A=%u\n", stFrmInfo.u8AspectRate ));
                (printf( "I=%u\n", stFrmInfo.u8Interlace ));
#endif
                return TRUE;
            }
        }
        else
        {
            //When dynamic scaling is enabled, f/w will handle the width/height change.
            //Therefore, only report SeqChg if framerate or interlace flag changes.
            if((stPreFrmInfo.u32FrameRate != stFrmInfo.u32FrameRate) ||
               (stPreFrmInfo.u8Interlace  != stFrmInfo.u8Interlace))
            {
                stPreFrmInfo.u8AspectRate = stFrmInfo.u8AspectRate;
                stPreFrmInfo.u32FrameRate = stFrmInfo.u32FrameRate;
                stPreFrmInfo.u8Interlace  = stFrmInfo.u8Interlace;
                return TRUE;
            }
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/// Set Debug Data which will be queried by MDrv_MVD_DbgGetData()
/// @param -u32Addr \b IN : address of debug data
/// @param -u32Data \b IN : the debug data
/// @return -return E_MVD_Result success/fail to set debug data
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_DbgSetData(MS_U32 u32Addr, MS_U32 u32Data)
{
    return E_MVD_RET_FAIL;
}

//------------------------------------------------------------------------------
/// Get Debug Data
/// @param -u32Addr \b IN : address of debug data
/// @param -u32Data \b IN : pointer to the debug data
/// @return -return E_MVD_Result success/fail to get debug data
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_DbgGetData(MS_U32 u32Addr, MS_U32* u32Data)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32Val;

    if (!u32Data)
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    SET_CMDARG(mvdcmd, u32Addr);
    if (HAL_MVD_MVDCommand( CMD_RD_IO, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_RD_IO ) );
        return E_MVD_RET_FAIL;
    }

#if (defined(CHIP_T2) || defined(CHIP_T7))
    u32Val = (((MS_U32)mvdcmd.Arg0)) | ((MS_U32)mvdcmd.Arg1 << 8) |
             (((MS_U32)mvdcmd.Arg2) << 16) | (((MS_U32)mvdcmd.Arg3) << 24);
#else
    u32Val = (((MS_U32)mvdcmd.Arg2)) | ((MS_U32)mvdcmd.Arg3 << 8) |
             (((MS_U32)mvdcmd.Arg4) << 16) | (((MS_U32)mvdcmd.Arg5) << 24);
#endif
    *u32Data = u32Val;

    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Set MVD firmware command
/// @param -u8cmd \b IN : MVD command
/// @param -pstCmdArg \b IN : pointer to command argument
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_MVDCommand ( MS_U8 u8cmd, MVD_CmdArg *pstCmdArg )
{
    return HAL_MVD_MVDCommand(u8cmd, pstCmdArg);
}



#ifndef REDLION_LINUX_KERNEL_ENVI
//------------- Below functions are for ATSC Closed Caption --------------------
//------------------------------------------------------------------------------
/// Reset MVD CC.  It can be called when overflow occurrs.
/// @return -Result of Reset CC.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCRst(MVD_CCCfg* pCCParam)
{
    return MDrv_MVD_CCStartParsing(pCCParam);
}


//------------------------------------------------------------------------------
/// Start CC data parsing.
/// @return -Result of issuing command to firmware.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCStartParsing(MVD_CCCfg* pCCParam)
{
    MS_U8 u8CC608 = (MS_U8)TRUE;
    MS_U8 u8Operation = 0;

    if ((!pCCParam) || (pCCParam->eFormat==E_MVD_CC_NONE) || (pCCParam->eType==E_MVD_CC_TYPE_NONE))
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    switch (pCCParam->eType)
    {
        case E_MVD_CC_TYPE_NTSC_FIELD1:
            u8Operation = 0x01;
            break;
        case E_MVD_CC_TYPE_NTSC_FIELD2:
            u8Operation = 0x02;
            break;
        case E_MVD_CC_TYPE_NTSC_TWOFIELD:
            u8Operation = 0x03;
            break;
        case E_MVD_CC_TYPE_DTVCC:
            u8Operation = 0x04;
            u8CC608 = FALSE;
            break;
        default:
            break;
    }

    MDrv_CC_Init();
    MDrv_CC_CM_SetMVDRB_HWAddr(pCCParam->u32BufStAdd, u8CC608);//fixme: VA2PA?
    MDrv_CC_CM_SetParsingType(u8Operation, (MS_U16)pCCParam->u32BufSize, u8CC608);

    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Stop CC data parsing.
/// @return -Result of issuing command to firmware.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCStopParsing(MVD_CCFormat eCCFormat)
{
    MS_U8 u8CC608 = 0xff;

    switch (eCCFormat)
    {
        case E_MVD_CC_608:
            u8CC608 = 1;
            break;
        case E_MVD_CC_708:
            u8CC608 = 0;
            break;
        default:
            return E_MVD_RET_INVALID_PARAM;
    }
    MDrv_CC_CM_DisableParsing(u8CC608);
    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Get write pointer of CC data buffer.
/// @return -Result of the query.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCGetWritePtr(MVD_CCFormat eCCFormat, MS_U32* pWrite)
{
    MS_U8 u8CC608;
    if (!pWrite)
        return E_MVD_RET_INVALID_PARAM;

    u8CC608 = ((eCCFormat==E_MVD_CC_608)?1:0);
    *pWrite = MDrv_CC_PM_GetMVDRB_WriteAddr(u8CC608);
    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Get the read pointer of CC data buffer.
/// @return -Result of the query.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCGetReadPtr(MVD_CCFormat eCCFormat, MS_U32* pRead)
{
    MS_U8 u8CC608;
    if (!pRead)
        return E_MVD_RET_INVALID_PARAM;

    u8CC608 = ((eCCFormat==E_MVD_CC_608)?1:0);
    *pRead = MDrv_CC_PM_GetMVDRB_ReadAddr(u8CC608);
    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Update the read pointer of CC data buffer.
/// @return -Result of the update.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCUpdateReadPtr(MVD_CCFormat eCCFormat, MS_U32 u32EachPacketSize)
{
    MS_U8 u8CC608;

    u8CC608 = ((eCCFormat==E_MVD_CC_608)?1:0);
    MDrv_CC_PM_SetMVDRB_ReadAddr(u32EachPacketSize, u8CC608);

    return E_MVD_RET_OK;
}


//------------------------------------------------------------------------------
/// Get if CC data buffer is overflow.
/// @return -Result of the query.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_CCGetIsOverflow(MVD_CCFormat eCCFormat, MS_BOOL* pbOverflow)
{
    MS_U8 u8CC608;
    if (!pbOverflow)
        return E_MVD_RET_INVALID_PARAM;

    u8CC608 = ((eCCFormat==E_MVD_CC_608)?1:0);
    *pbOverflow = MDrv_CC_CM_GetOverflowStatus(u8CC608);

    return E_MVD_RET_OK;
}
#endif



//------------- Below functions are for MediaCodec SLQ Table --------------------
#if _SLQTBL_DUMP_PTS
static void _SLQTbl_DumpPtsTbl(MS_U32 u32EntryStart, MS_U32 u32EntryEnd)
{
    MS_U32 i;
    MS_U32 u32EsRp, u32EsStart, u32EsEnd;

    for (i=u32EntryStart; i<u32EntryEnd; i++)
    {
        u32EsRp = _drvEsTbl.u32StAdd + i*8;

        u32EsEnd = HAL_MVD_MemRead4Byte(u32EsRp+4);
        u32EsStart = HAL_MVD_MemRead4Byte(u32EsRp); //report StartAdd as read_pointer
        printf("ES[%lx] Start=0x%lx End=0x%lx u32EsRp=%lx\n",
            i, u32EsStart, u32EsEnd, u32EsRp);
    }

    printf("\n=======Dump PTS table========\n");
    printf("addr\t byte_cnt\t dummy_cnt\t id_low\t id_high\t time_stamp\n");
    for (i=u32EntryStart; i<u32EntryEnd; i++)
    {
        printf("0x%lx\t 0x%08lx\t 0x%08lx\t 0x%08lx\t 0x%08lx\t 0x%08lx\n", u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN,
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN),   //byteCnt
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+4), //dummyPktCnt
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+8), //idLow
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+12),//idHigh
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+16) //pts
               );
    }
    printf("=====================================\n");
}
#endif


static MS_BOOL MVD_SLQTblInsertPattern(MVD_PatternType ePattern)
{
    MS_U32 i;

    if (_drvSlqTbl.u32Empty < SLQ_TBL_SAFERANGE)
    {
        MVD_DEBUGINFO(printf("SLQTbl full!(0x%lx) Cannot insert pattern any more!\n", _drvSlqTbl.u32Empty));
        return FALSE;
    }

#if SLQ_NEW_PUSH
    if(_SlqStatus.bSlqCtrlBit)
    {
        if(_SlqStatus.bSlqPicWaitNextStart)
        {
            _drvSlqTbl.u32WrPtr = _SlqStatus.u32VaildWptrAddr;
        }
        _SlqStatus.bSlqCtrlBit = FALSE;
    }
#endif // #if SLQ_NEW_PUSH
    for (i =0; i<2; i++)
    {   //insert dummy pattern
        MVD_PacketInfo stDummyPkt;

        if (E_MVD_PATTERN_FLUSH == ePattern)
        {
            MVD_SLQTblGetDummyPkt(&stDummyPkt);
        }
        else if (E_MVD_PATTERN_FILEEND == ePattern)
        {
            MVD_SLQTblGetFileEndPkt(&stDummyPkt);
        }
        else
        {
            MVD_DEBUGERROR(printf("Invalid MVD_PatternType! Won't insert pattern!\n"));
            return FALSE;
        }
        #if SLQ_NEW_PUSH
        _SlqStatus.bSlqFireRdy = TRUE;
        #endif
        //printf("WrPtr 0x%lx ", _drvSlqTbl.u32WrPtr);
        MVD_SLQTblSendPacket(&stDummyPkt);
        //printf("==> 0x%lx\n", _drvSlqTbl.u32WrPtr);
        MDrv_MVD_SetSLQWritePtr(FALSE);
    }
    return TRUE;
}


static MS_BOOL MVD_SLQTblSendPacket(MVD_PacketInfo *pstVideoPKT)
{
    static MS_U32 u32EsLast;
    static MS_U32* u32LastEntry = &_drvSlqTbl.u32WrPtr;
    MS_U32* pu32EsNew = &_drvEsTbl.u32WrPtr;
    MS_U32 u32EntryWord = 0;
    MS_U32 u32Index = 0;
    MS_U32 u32Pts;
    MS_U32 u32ESStart=0;
    MS_U32 u32ESEnd=0;
#if _SLQTBL_DUMP_PKT
    static MS_U32 u32SendTimes = 0;

    printf("Pkt[%lx] st=0x%lx len=0x%lx pts=0x%lx id_l=0x%lx id_h=0x%lx\n", u32SendTimes++,
        pstVideoPKT->u32StAddr, pstVideoPKT->u32Length, pstVideoPKT->u32TimeStamp,
        pstVideoPKT->u32ID_L, pstVideoPKT->u32ID_H);
#endif

    MS_ASSERT(u32MVDFWPtsTblAddr != NULL);
    if (u32MVDFWPtsTblAddr)
    {
        MS_U32 u32PtsTblEntryAddr = 0;
        u32Index = (_drvSlqTbl.u32WrPtr - _drvSlqTbl.u32StAdd)/8;
        if (pstVideoPKT->u32TimeStamp != MVD_NULLPKT_PTS)
        {
            u32Pts = _MS_TO_90K(pstVideoPKT->u32TimeStamp);
        }
        else
        {
            u32Pts = MVD_NULLPKT_PTS;
        }
        u32PtsTblEntryAddr = u32MVDFWPtsTblAddr+u32Index*MVD_FW_SLQTBL_PTS_LEN;
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+4, u32DummyPktCnt);   //dummyPktCnt
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+8, pstVideoPKT->u32ID_L);  //idLow
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+12, pstVideoPKT->u32ID_H); //idHigh
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+16, u32Pts);  //PTS
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr, u32SlqByteCnt&0xffffff); //byteCnt
        //printf("PTS=0x%lx(%lx), idx=0x%lx add=0x%lx\n", pstVideoPKT->u32TimeStamp,
        //        HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+u32Index*MVD_FW_SLQTBL_PTS_LEN+4),
        //        u32Index, u32MVDFWPtsTblAddr+u32Index*MVD_FW_SLQTBL_PTS_LEN+4);

        if (MDrv_MVD_IsMStreamerMode())
        {
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+12, pstVideoPKT->u32ID_H); //idHigh
            HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+16, pstVideoPKT->u32TimeStamp);  //PTS

            MVD_DBG_STS(printf(">>> drvMVD pts,idH = %lu, %lu\n", HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+16),
                   HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+12)));
            if (((MVD_NULLPKT_PTS == pstVideoPKT->u32ID_H) && (MVD_NULLPKT_PTS == pstVideoPKT->u32TimeStamp)) == FALSE)
            {   //Only update PTS to firmware when pts field is valid.
                //Plz refer to MDrv_MVD_PushQueue if (u8MstMode == MST_MODE_TRUE) {}
                HAL_MVD_UpdatePts();    //for uniplayer
            }
        }
    }
#if _SLQTBL_DUMP_PTS
    if (u32Index == 0x177)
    {
        _SLQTbl_DumpPtsTbl(0, 0x179);
    }
#endif

    u32SlqByteCnt += pstVideoPKT->u32Length;

    //update SLQ tbl entry
    //u32EsLast = (pstVideoPKT->u32StAddr)+_drvSlqTbl.u32StAdd;
    u32EsLast = (pstVideoPKT->u32StAddr)+MVD_GetMemOffset(stMemCfg.u32BSAddr);
    HAL_MVD_MemWrite4Byte(*pu32EsNew, u32EsLast-MVD_GetMemOffset(stMemCfg.u32BSAddr));
    u32ESStart = (u32EsLast) & MVD_SLQ_TBL_ENTRY_LENS;//SLQ_TBL_ENTRY_LEN;

    u32EsLast += pstVideoPKT->u32Length; //update ES write pointer
    //Notice: This is for MVD HW, so no need to minus one.
    HAL_MVD_MemWrite4Byte((*pu32EsNew)+4, u32EsLast-MVD_GetMemOffset(stMemCfg.u32BSAddr));
    u32ESEnd   = (u32EsLast) & MVD_SLQ_TBL_ENTRY_LENS;//SLQ_TBL_ENTRY_LEN;

    *pu32EsNew += 8;
    if (*pu32EsNew >= _drvEsTbl.u32EndAdd)
    {   //wrap to the beginning of the table
        MVD_DEBUGINFO(printf("...ES wrapping to the beginning!\n"));
        *pu32EsNew = _drvEsTbl.u32StAdd;
    }

#ifdef _MVD6
    //MVD_PRINT("===>[%lx] u32ESStart=0x%lx u32ESEnd=0x%lx u32EsLast=0x%lx\n",
    //    pMVDHalContext->u32SendTimes[u8Idx]++, u32ESStart, u32ESEnd, u32EsLast);

    u32EntryWord = u32ESEnd;
    HAL_MVD_MemWrite4Byte(*u32LastEntry, u32EntryWord);
    //MVD_PRINT("===> u32EntryWord1 addr=0x%lx\n", (*u32LastEntry));
    //MVD_PRINT("===> u32EntryWord0=0x%lx\n", u32EntryWord);

    u32EntryWord = u32ESStart;
    HAL_MVD_MemWrite4Byte((*u32LastEntry)+4, u32EntryWord);
    //MVD_PRINT("===> u32EntryWord1 addr=0x%lx\n", (*u32LastEntry)+4);
    //MVD_PRINT("===> u32EntryWord1=0x%lx\n", u32EntryWord);
#else
    //printf("===>[%lx] u32ESStart=0x%lx u32ESEnd=0x%lx u32EsLast=0x%lx\n",
    //    u32SendTimes++, u32ESStart, u32ESEnd, u32EsLast);

    u32EntryWord = u32ESEnd | (u32ESStart <<  MVD_SLQ_TBL_ENTRY_SIZES);//29);
    HAL_MVD_MemWrite4Byte(*u32LastEntry, u32EntryWord);
    //printf("===> u32EntryWord1 addr=0x%lx\n", (*u32LastEntry));
    //printf("===> u32EntryWord0=0x%lx\n", u32EntryWord);

    u32EntryWord = ((pstVideoPKT->u32ID_L & MVD_SLQ_TBL_SHIFT_MASK)<<
            MVD_SLQ_TBL_SHIFT_BIT1) | (u32ESStart >> MVD_SLQ_TBL_SHIFT_BIT2);


    HAL_MVD_MemWrite4Byte((*u32LastEntry)+4, u32EntryWord);
    //printf("===> u32EntryWord1 addr=0x%lx\n", (*u32LastEntry)+4);
    //printf("===> u32EntryWord1=0x%lx\n", u32EntryWord);
#endif
    *u32LastEntry += 8;
    if (*u32LastEntry >= _drvSlqTbl.u32EndAdd)
    {   //wrap to the beginning of the table
        MVD_DEBUGINFO(printf("...wrapping to the beginning!\n"));
        *u32LastEntry = _drvSlqTbl.u32StAdd;
        //also wrap DivX311 pattern table
        _drvDivxTbl.u32WrPtr = _drvDivxTbl.u32StAdd;
    }

    if (_drvSlqTbl.u32Empty)
    {
        _drvSlqTbl.u32Empty -= SLQ_ENTRY_LEN;
    }

    return TRUE;
}
#if SLQ_NEW_PUSH
static void MVD_SLQTblGetHdrPkt(MVD_PacketInfo* pDivxHdr, MVD_PacketInfo* pDivxData)
#else
static void MVD_SLQTblGetDivxHdrPkt(MVD_PacketInfo* pDivxHdr, MVD_PacketInfo* pDivxData)
#endif
{
    MS_U32 u32DivXPattern = _drvDivxTbl.u32WrPtr;
    MS_U32 u32FrmSize = pDivxData->u32Length;
#if SLQ_NEW_PUSH
    if(_SlqStatus.bSlqCtrlBit)
    {
        if(_SlqStatus.bSlqPicWaitNextStart && _SlqStatus.bSlqPicStart)
        {
             //printf("Show KC the SlqPushLength = 0x%lx\n",_SlqStatus.u32SlqPushLength);
            if (E_MVD_CODEC_DIVX311 == curCodecType)
            {
                HAL_MVD_MemWrite4Byte(_SlqStatus.u32SlqPatternAddr +4, _SlqStatus.u32SlqPushLength);
                HAL_MVD_MemWrite4Byte(_SlqStatus.u32SlqPatternAddr , DIVX_PATTERN);
            }
            else if(E_MVD_CODEC_VC1_MAIN == curCodecType)
            {//rcv
                HAL_MVD_MemWrite4Byte(_SlqStatus.u32SlqPatternAddr +4, RCV_PATTERN);
                HAL_MVD_MemWrite4Byte(_SlqStatus.u32SlqPatternAddr , _SlqStatus.u32SlqPushLength);
            }
            else if (E_MVD_CODEC_VC1_ADV == curCodecType)
            {
                HAL_MVD_MemWrite4Byte(_SlqStatus.u32SlqPatternAddr , VC1_PATTERN);
            }
            _SlqStatus.u32SlqPushLength = 0;
            _SlqStatus.u32SlqPatternAddr  = 0;
            _SlqStatus.bSlqFireRdy =TRUE;
#if UPDATE_NEXT_WRPTR
            _SlqStatus.u32VaildWptrAddr = _drvSlqTbl.u32WrPtr + SLQ_ENTRY_LEN;
#else
            _SlqStatus.u32VaildWptrAddr = _drvSlqTbl.u32WrPtr;
#endif
            _SlqStatus.bSlqPicWaitNextStart = FALSE;
            _SlqStatus.bSlqPicCollect = FALSE;
        }

        if(_SlqStatus.bSlqPicStart)
        {
            _SlqStatus.u32SlqPatternAddr = u32DivXPattern;
            _SlqStatus.u32SlqPushLength += u32FrmSize;
            _SlqStatus.bSlqPicCollect = TRUE;
            _SlqStatus.bSlqPicWaitNextStart =TRUE;
        }
        else if(_SlqStatus.bSlqPicCollect)
        {
            _SlqStatus.u32SlqPushLength += u32FrmSize;
            _SlqStatus.bSlqPicWaitNextStart =TRUE;
        }
    }
    else
    {
        HAL_MVD_MemWrite4Byte(u32DivXPattern, DIVX_PATTERN);
        HAL_MVD_MemWrite4Byte(u32DivXPattern +4,u32FrmSize);
    }
#else
    HAL_MVD_MemWrite4Byte(u32DivXPattern, DIVX_PATTERN);
    HAL_MVD_MemWrite4Byte(u32DivXPattern+4, u32FrmSize);
#endif
    pDivxHdr->u32StAddr = u32DivXPattern - MVD_GetMemOffset(stMemCfg.u32DrvBufAddr);
    pDivxHdr->u32TimeStamp = pDivxData->u32TimeStamp; //unit: ms
    pDivxHdr->u32ID_L = pDivxData->u32ID_L;
    pDivxHdr->u32ID_H = pDivxData->u32ID_H;
    //printf("u32DivXPattern(0x%lx==>0x%lx)=0x%lx 0x%lx\n", u32DivXPattern, pDivxHdr->u32StAddr,
    //        HAL_MVD_MemRead4Byte(u32DivXPattern), HAL_MVD_MemRead4Byte(u32DivXPattern+4));
    if (E_MVD_CODEC_VC1_ADV == curCodecType)
    {
        _drvDivxTbl.u32WrPtr += 4;
        pDivxHdr->u32Length = 4;
    }
    else
    {
        _drvDivxTbl.u32WrPtr += 8;
        pDivxHdr->u32Length = 8;
    }
#if SLQ_NEW_PUSH
    if(_SlqStatus.bSlqPicStart)
    {
        if (E_MVD_CODEC_VC1_ADV == curCodecType)
        {
            _drvDivxTbl.u32WrPtr += 4;
            pDivxHdr->u32Length = 4;
        }
        else
        {
            _drvDivxTbl.u32WrPtr += 8;
            pDivxHdr->u32Length = 8;
        }
    }
#endif

#if 0   //move to SendPacket
    if (_drvDivxTbl.u32WrPtr >= _drvDivxTbl.u32EndAdd)
    {   //wrap to the beginning of the table
        printf("...wrapping to the DivXTbl beginning!\n");
        _drvDivxTbl.u32WrPtr = _drvDivxTbl.u32StAdd;
    }
#endif
}


static void MVD_SLQTblGetFileEndPkt(MVD_PacketInfo* pFileEnd)
{
    MS_U32 u32EndPattern = MVD_GetMemOffset(stMemCfg.u32DrvBufAddr+SLQ_TBL_SIZE*3);

    pFileEnd->u32StAddr = SLQ_TBL_SIZE*3;//u32EndPattern - MVD_GetMemOffset(stMemCfg.u32DrvBufAddr);
    pFileEnd->u32Length = END_PATTERN_SIZE;
    pFileEnd->u32TimeStamp = MVD_NULLPKT_PTS;
    pFileEnd->u32ID_L = MVD_U32_MAX;
    pFileEnd->u32ID_H = MVD_U32_MAX;
    MVD_DEBUGINFO(printf("u32EndPattern(0x%lx)=0x%lx 0x%lx 0x%lx 0x%lx\n", pFileEnd->u32StAddr,
            HAL_MVD_MemRead4Byte(u32EndPattern), HAL_MVD_MemRead4Byte(u32EndPattern+4),
            HAL_MVD_MemRead4Byte(u32EndPattern+8), HAL_MVD_MemRead4Byte(u32EndPattern+12)));
}


static void MVD_SLQTblGetDummyPkt(MVD_PacketInfo* pDummy)
{
    //MS_U32 u32DummyES = MVD_GetMemOffset(stMemCfg.u32DrvBufAddr+SLQ_TBL_SIZE*2);

    pDummy->u32StAddr = SLQ_TBL_SIZE*2;//u32DummyES - MVD_GetMemOffset(stMemCfg.u32DrvBufAddr);
    pDummy->u32Length = DUMMY_SIZE;
    pDummy->u32TimeStamp = MVD_NULLPKT_PTS;
    pDummy->u32ID_L = MVD_U32_MAX;
    pDummy->u32ID_H = MVD_U32_MAX;
#if 0
    printf("u32DummyES(0x%lx-->0x%lx, size=0x%lx)=0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", u32DummyES,
            pDummy->u32StAddr, pDummy->u32Length, HAL_MVD_MemRead4Byte(u32DummyES),
            HAL_MVD_MemRead4Byte(u32DummyES+4),HAL_MVD_MemRead4Byte(u32DummyES+8),HAL_MVD_MemRead4Byte(u32DummyES+12));
#endif

}


static void MVD_SLQTblInitFileEndPkt(MVD_CodecType eType)
{
    MS_U32 u32EndPattern = MVD_GetMemOffset(stMemCfg.u32DrvBufAddr+SLQ_TBL_SIZE*3);
    MS_U32 i;

    for (i=0; i<END_PATTERN_SIZE; i+=4)
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern+i, 0xffffffff);
    }
    if ((E_MVD_CODEC_FLV == eType)||(E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER == eType))
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, FLV_PATTERN);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, 0xffffffff);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_1); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_2); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+16,END_PATTERN_3); //scw
        //printf("##########FileEnd for FLV/SVH!, u32EndPattern=%lx\n",u32EndPattern);
    }
    else if (E_MVD_CODEC_DIVX311 == eType)
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, DIVX_PATTERN);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, 0xffffffff);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_1); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_2); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+16,END_PATTERN_3); //scw
        //printf("##########FileEnd for DIVX311!, u32EndPattern=%lx\n",u32EndPattern);
    }
    else if ((E_MVD_CODEC_MPEG2 == eType)||(E_MVD_CODEC_MPEG4 == eType))
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, MPEG_PATTERN_0);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, END_PATTERN_1);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_2);
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_3);
        //printf("##########FileEnd for MPEG2/4!, u32EndPattern=%lx\n",u32EndPattern);
    }
    else
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, END_PATTERN_0);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, END_PATTERN_1);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_2); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_3); //scw
        //printf("##########FileEnd for VC1!, u32EndPattern=%lx\n",u32EndPattern);
    }

    MVD_DEBUGINFO(printf("u32EndPattern(0x%lx)=0x%lx 0x%lx\n", u32EndPattern,
            HAL_MVD_MemRead4Byte(u32EndPattern), HAL_MVD_MemRead4Byte(u32EndPattern+4)));
}


static void MVD_SLQTblInitDummyPkt(MVD_CodecType eType)
{
    MS_U32 u32DummyES = MVD_GetMemOffset(stMemCfg.u32DrvBufAddr+SLQ_TBL_SIZE*2);
    MS_U32 u32DummyPattern[3];
    MS_U32 u32PatternSize;
    MS_U32 i;

    //printf("eType = 0x%x\n", eType);
    //initial content for dummy packet
    for (i=0; i<DUMMY_SIZE; i+=4)
    {
        HAL_MVD_MemWrite4Byte(u32DummyES+i, 0xffffffff);
    }

    switch (eType)
    {
        case E_MVD_CODEC_FLV:
        case E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER:
            u32DummyPattern[0] = FLV_PATTERN;
            u32PatternSize = 1;
            break;

        case E_MVD_CODEC_DIVX311:
            u32DummyPattern[0] = DIVX_PATTERN;
            u32PatternSize = 1;
            break;

        case E_MVD_CODEC_VC1_ADV: //vc1
            u32DummyPattern[0] = VC1_PATTERN_0;
            u32DummyPattern[1] = VC1_PATTERN_1;
            u32DummyPattern[2] = VC1_PATTERN_2;
            u32PatternSize = 3;
            break;

        case E_MVD_CODEC_VC1_MAIN: //rcv
            u32DummyPattern[0] = RCV_PATTERN_0;
            u32DummyPattern[1] = RCV_PATTERN_1;
            u32DummyPattern[2] = RCV_PATTERN_2;
            u32PatternSize = 3;
            break;

        default:
            u32DummyPattern[0] = DUMMY_PATTERN;
            u32PatternSize = 1;
            break;
    }
    for (i=0; i<u32PatternSize; i++)
    {
        HAL_MVD_MemWrite4Byte(u32DummyES+i*4, u32DummyPattern[i]);
    }
#if 0
    printf("u32DummyES(0x%lx)=0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", u32DummyES, HAL_MVD_MemRead4Byte(u32DummyES),
            HAL_MVD_MemRead4Byte(u32DummyES+4),HAL_MVD_MemRead4Byte(u32DummyES+8),HAL_MVD_MemRead4Byte(u32DummyES+12));
#endif

}


//------------------------------------------------------------------------------------------------------------
// Layout of drvProcBuffer
//              -----------------
// drvProcBuff | SlqTbl entries  | <- _drvSlqTbl.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             | DivX311         | <- _drvSlqTbl.u32EndAdd   <- _drvDivxTbl.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             | Flush Patterns  |... <- _drvDivxTbl.u32EndAdd   <- _drvDummy.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             | FileEnd Pattern |... <- _drvDummy.u32EndAdd   <- _drvFileEnd.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             |  ES table       |... <- _drvFileEnd.u32EndAdd   <- _drvEsTbl.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             |                 |........ End of drvProcBuff
//
//------------------------------------------------------------------------------------------------------------
static void MVD_SLQTblInit(void)
{
    //printf("%s\n", __FUNCTION__);
    MS_U32 u32Addr, u32Len, i;//, u32EsStart;

    u32DummyPktCnt = 0;//reset dummy packet counter
    u32SlqByteCnt = 0; //reset SLQ table byte counter
    bSlqTblHasValidData = FALSE;

    HAL_MVD_MemGetMap(E_MVD_MMAP_DRV, &u32Addr, &u32Len);
    //u32EsStart = u32Addr + u32Len;
    //printf("DRV_PROC@0x%lx 0x%lx es@0x%lx\n", u32Addr, u32Len, u32EsStart);

    //init SLQ table attributes
    _drvSlqTbl.u32StAdd  = u32Addr;
    _drvSlqTbl.u32EndAdd = u32Addr + SLQ_TBL_SIZE;
    _drvSlqTbl.u32EntryCntMax = SLQ_ENTRY_MAX;
    u32FileEndPtr = _drvSlqTbl.u32StAdd;

    //reset SLQ table read/write pointers
    _drvSlqTbl.u32RdPtr = _drvSlqTbl.u32StAdd;
    _drvSlqTbl.u32WrPtr = _drvSlqTbl.u32StAdd;
    #if SLQ_NEW_PUSH
    #if UPDATE_NEXT_WRPTR
    _SlqStatus.u32VaildWptrAddr = _drvSlqTbl.u32WrPtr + SLQ_ENTRY_LEN;
    #else
    _SlqStatus.u32VaildWptrAddr = _drvSlqTbl.u32WrPtr;
    #endif
    _SlqStatus.bSlqPicWaitNextStart = FALSE;
    _SlqStatus.bSlqCtrlBit = FALSE;
    _SlqStatus.u32SlqPushLength = 0;
    _SlqStatus.bSlqPicStart = FALSE;
    _SlqStatus.bSlqPicCollect = FALSE;
    _SlqStatus.bSlqEnLastFrameShow =FALSE;
    _SlqStatus.bSlqFireRdy = FALSE;
    #endif
    _drvSlqTbl.u32Empty = SLQ_TBL_SIZE;

    //_SLQTbl_DumpInfo(&_drvSlqTbl);

    {
        _mvdCmdQ.u32PtsBase = MVD_NULLPKT_PTS;
    }

#if (!MVD_TURBO_INIT)
    //reset SLQ table
    _MVD_Memset(_drvSlqTbl.u32StAdd, 0, SLQ_TBL_SIZE);
#endif

    //set SLQ table start/end to F/W
    MDrv_MVD_SetSLQTblBufStartEnd(MVD_GetMemOffset(stMemCfg.u32BSAddr), MVD_GetMemOffset(stMemCfg.u32BSAddr+SLQ_TBL_SIZE));
    //if (stMemCfg.bFWMiuSel != stMemCfg.bHWMiuSel)
    if (bSlqTblSync)
    {
        //init BDMA for SLQ table update when MDrv_MVD_SetSLQWritePtr

#ifndef REDLION_LINUX_KERNEL_ENVI
        const BDMA_Info* pBDMA;
        pBDMA = MDrv_BDMA_GetInfo();
        if ((pBDMA == NULL) || (pBDMA->bInit != TRUE))
        {
            if (E_BDMA_OK != MDrv_BDMA_Init(stMemCfg.u32Miu1BaseAddr))
            {
                printf("%s fail at MDrv_BDMA_Init!!!\n", __FUNCTION__);
            }
        }

        if (stMemCfg.bHWMiuSel == MIU_SEL_1)
        {
            if (stMemCfg.bFWMiuSel == MIU_SEL_0)
            {
                bdmaCpyType = E_BDMA_SDRAM2SDRAM1;
            }
            else if (stMemCfg.bFWMiuSel == MIU_SEL_1)
            {
                bdmaCpyType = E_BDMA_SDRAM12SDRAM1;
            }
            else
            {
                MS_ASSERT(0);
            }
        }
        else if (stMemCfg.bHWMiuSel == MIU_SEL_0)
        {
            if (stMemCfg.bFWMiuSel == MIU_SEL_0)
            {
                bdmaCpyType = E_BDMA_SDRAM2SDRAM;
            }
            else if (stMemCfg.bFWMiuSel == MIU_SEL_1)
            {
                bdmaCpyType = E_BDMA_SDRAM12SDRAM;
            }
            else
            {
                MS_ASSERT(0);
            }
        }
        else
        {
            MS_ASSERT(0);
        }
#endif
    }

    ///// init SLQ entries for DivX311
    _drvDivxTbl.u32StAdd = _drvSlqTbl.u32EndAdd;
    _drvDivxTbl.u32EndAdd= _drvDivxTbl.u32StAdd + SLQ_TBL_SIZE;
    _drvDivxTbl.u32WrPtr = _drvDivxTbl.u32StAdd;
    //_drvDivxTbl.u32RdPtr = _drvDivxTbl.u32StAdd;
#if (!MVD_TURBO_INIT)
    //reset DivX311 pattern table
    _MVD_Memset(_drvDivxTbl.u32StAdd, 0, SLQ_TBL_SIZE);
#endif

    ///// init flush pattern
    MVD_SLQTblInitDummyPkt(curCodecType);

    ///// init file-end pattern
    MVD_SLQTblInitFileEndPkt(curCodecType);

    ///// init ES table
    _drvEsTbl.u32StAdd = _drvDivxTbl.u32EndAdd + DUMMY_SIZE*2;
    _drvEsTbl.u32EndAdd= _drvEsTbl.u32StAdd + ES_TBL_SIZE;
    _drvEsTbl.u32WrPtr = _drvEsTbl.u32StAdd;
    //reset ES table
    for (i = 0; i < ES_TBL_SIZE; i+=4)
    {
        HAL_MVD_MemWrite4Byte(_drvEsTbl.u32StAdd+i, stMemCfg.u32DrvBufSize);
    }

    u32PreEsRd = MVD_U32_MAX;    //reset ES read pointer
    u32PreEsWr = 0;              //reset ES write pointer

    return;
}


#if 0
static void _SLQTbl_DumpInfo(MVD_SLQ_TBL_ST* pInfo)
{
    printf("str=0x%lx\n", pInfo->u32StAdd);
    printf("end=0x%lx\n", pInfo->u32EndAdd);
    printf("cnt=0x%lx\n", pInfo->u32EntryCntMax);
    printf("rd =0x%lx\n", pInfo->u32RdPtr);
    printf("wr =0x%lx\n", pInfo->u32WrPtr);
    return;
}
#endif

#ifdef MVD_ENABLE_ISR
static MS_BOOL MVD_IntHasUsrDataDisp(MS_U32 u32IntStat)
{

    return (((u32IntStat&INT_USER_DATA_DISP)==INT_USER_DATA_DISP) ? TRUE : FALSE);

}

static MS_BOOL MVD_IntHasUsrData(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_USER_DATA)==INT_USER_DATA) ? TRUE : FALSE);
}

static MS_BOOL MVD_IntIsDispRdy(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DISP_RDY)==INT_DISP_RDY) ? TRUE : FALSE);
}

static MS_BOOL MVD_IntHasSeqHdr(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_SEQ_FOUND)==INT_SEQ_FOUND) ? TRUE : FALSE);
}

//INT_FIRST_FRAME means "1st frame be push to display queue & ready for display"
//So, (1) in IPB or IP stream, that's I-frame
//    (2) in PB only stream, that's first P-frame
static MS_BOOL MVD_IntHas1stFrame(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_FIRST_FRAME)==INT_FIRST_FRAME) ? TRUE : FALSE);
}

#define INT_DEC_I (1<<17) // wait for release to remove
//INT_DEC_I
static MS_BOOL MVD_IntHasDecodeIframe(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DEC_I)==INT_DEC_I) ? TRUE : FALSE);
}

static MS_BOOL MVD_IntVSyncInt(MS_U32 u32IntStat)
{

    return (((u32IntStat&INT_DISP_VSYNC)==INT_DISP_VSYNC) ? TRUE : FALSE);

}

///Notice: This function only works when being called by fnHandler, which was
///registered by AP using MDrv_MVD_SetIsrEvent()
MS_U32 MDrv_MVD_GetIsrEvent(void)
{
    return eCurEvent;
}

void MVD_IsrProc(void)
{
    MS_U32 u32IntStat = 0;


    u32IntStat = HAL_MVD_GetIntState();

    if (u32IntStat != 0)
    {
        //MVD_DEBUGINFO(printf("MVD_IsrProc u32IntStat=%lx\n", u32IntStat));
        eCurEvent = E_MVD_EVENT_DISABLE_ALL;

        if ((eEventFlag & E_MVD_EVENT_USER_DATA) == E_MVD_EVENT_USER_DATA)
        {
            if (MVD_IntHasUsrData(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_USER_DATA;
                MVD_DEBUGINFO(printf("===> UsrData!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_USER_DATA_DISP) == E_MVD_EVENT_USER_DATA_DISP)
        {
            if (MVD_IntHasUsrDataDisp(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_USER_DATA_DISP;
                MVD_DEBUGINFO(printf("===> DispUsrData!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_DISP_RDY) == E_MVD_EVENT_DISP_RDY)
        {
            if (MVD_IntIsDispRdy(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_DISP_RDY;
                MVD_DEBUGINFO(printf("===> DispRdy!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_SEQ_FOUND) == E_MVD_EVENT_SEQ_FOUND)
        {
            if (MVD_IntHasSeqHdr(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_SEQ_FOUND;
                MVD_DEBUGINFO(printf("===> SeqHdrFound!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_FIRST_FRAME) == E_MVD_EVENT_FIRST_FRAME)
        {
            if (MVD_IntHas1stFrame(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_FIRST_FRAME;
                MVD_DEBUGINFO(printf("===> 1stFrame!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_DEC_I) == E_MVD_EVENT_DEC_I)
        {
            if (MVD_IntHasDecodeIframe(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_DEC_I;
                MVD_DEBUGINFO(printf("===> E_MVD_EVENT_DEC_I!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_DISP_VSYNC) == E_MVD_EVENT_DISP_VSYNC)
        {
            if (MVD_IntVSyncInt(u32IntStat))
            {
                eCurEvent |= E_MVD_EVENT_DISP_VSYNC;
                MVD_DEBUGINFO(printf("===> VSyncInt!!!\n"));
            }
        }
        if ((eEventFlag & E_MVD_EVENT_UNMUTE) == E_MVD_EVENT_UNMUTE)
        {
            MVD_DEBUGINFO(printf("E_MVD_EVENT_UNMUTE...\n"));
            if (MVD_IntVSyncInt(u32IntStat))
            {
                MVD_DEBUGINFO(printf("===> UNMUTE?!\n"));
                if (MDrv_MVD_GetIsIPicFound() != NULL)
                {
                    u32VSyncCnt++;
                    if (MDrv_MVD_GetSyncStatus()==1)
                    {
                        u32VSyncCnt = 0;
                        eCurEvent |= E_MVD_EVENT_UNMUTE;
                        MVD_DEBUGINFO(printf("UnMute Sync!!!\n"));
                        eEventFlag &= (~E_MVD_EVENT_UNMUTE);
                        MDrv_MVD_EnableInt(eEventFlag);
                    }
                    else //time out checking
                    {
                        if((MDrv_MVD_GetPtsStcDiff() > MVD_UNMUTE_STC_PTS_GAP)
                        && (u32VSyncCnt > 25))
                        {
                            u32VSyncCnt = 0;
                            eCurEvent |= E_MVD_EVENT_UNMUTE;
                            MVD_DEBUGINFO(printf("UnMute 25 Vsync!!!\n"));
                            eEventFlag &= (~E_MVD_EVENT_UNMUTE);
                            MDrv_MVD_EnableInt(eEventFlag);
                        }

                        if(u32VSyncCnt > 60)
                        {
                            u32VSyncCnt = 0;
                            eCurEvent |= E_MVD_EVENT_UNMUTE;
                            MVD_DEBUGINFO(printf("UnMute 60 Vsync!!!\n"));
                            eEventFlag &= (~E_MVD_EVENT_UNMUTE);
                            MDrv_MVD_EnableInt(eEventFlag);
                        }
                    }
                }
                else // for P/B frame only stream
                {
                    if (MDrv_MVD_GetSyncStatus() == 1) //sync done
                    {
                        u32SyncDoneCnt++;
                    }

                    if (u32SyncDoneCnt > PB_ONLY_UNMUTE_VSYNC_COUNT) //sync done
                    {
                        MVD_DEBUGINFO(printf("UnMute after %d vsync!!!\n", PB_ONLY_UNMUTE_VSYNC_COUNT));
                        eCurEvent |= E_MVD_EVENT_UNMUTE;
                        eEventFlag &= (~E_MVD_EVENT_UNMUTE);
                        MDrv_MVD_EnableInt(eEventFlag);
                        u32SyncDoneCnt = 0;
                    }
                }
            }
        }

        //Events that user registered occurred, call user's callback function
        if ((eCurEvent!=E_MVD_EVENT_DISABLE_ALL) && (pfnCallback!=NULL))
        {
            pfnCallback();
        }
    }

    //clear interrupt & events
    eCurEvent = E_MVD_EVENT_DISABLE_ALL;
    HAL_MVD_ClearIRQ();
    OSAL_MVD_IntEnable(); //enable cpu interrupt mask
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_HVD_SetISREvent()
/// @brief \b Function \b Description: Set the ISR event type sended by HVD fw.
/// @param -eEvent \b IN : event types
/// @param -fnISRHandler \b IN : function pointer to a interrupt handler.
/// @return -The result of command set ISR event.
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetIsrEvent(MS_U32 eEvent, MVD_InterruptCb fnHandler)
{
    if (_bDrvInit != TRUE)
    {
        MVD_DEBUGERROR(printf("Call %s before Init\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    if (eEvent == E_MVD_EVENT_DISABLE_ALL)
    {
        //HAL_MVD_Enable_ISR(FALSE);
        OSAL_MVD_IntDisable();
#if !defined(SEC_X4)
        if(TRUE == bIsrAttached)
        {
            OSAL_MVD_IsrDetach();
            bIsrAttached = FALSE;
        }
#endif
        pfnCallback = NULL;
        eEventFlag = E_MVD_EVENT_DISABLE_ALL;
    }
    else
    {
        if(fnHandler != NULL)
        {
            //disable int & dettach isr?

            pfnCallback = (MVD_InterruptCb)fnHandler;
            eEventFlag = eEvent;
            if (FALSE == bIsrAttached)
            {
                if (OSAL_MVD_IsrAttach((void*)MVD_IsrProc) != TRUE)
                {
                    MVD_DEBUGERROR(printf("fail to attach MVD_IsrProc!\n"));
                    return E_MVD_RET_FAIL;
                }
                bIsrAttached = TRUE;
                if (OSAL_MVD_IntEnable() != TRUE)
                {
                    MVD_DEBUGERROR(printf("fail to OSAL_MVD_IntEnable!\n"));
                    return E_MVD_RET_FAIL;
                }
            }

            MVD_DEBUGINFO(printf("MDrv_MVD_SetIsrEvent eEventFlag=0x%lx\n", eEventFlag));
            MDrv_MVD_EnableInt(eEventFlag);
            MVD_DEBUGINFO(printf("attach ISR number:%d\n" , E_INT_IRQ_MVD));
            return E_MVD_RET_OK;
        }
        else
        {
            MVD_DEBUGERROR(printf( "SetISREvent with NULL pointer. ISR type:%lu\n", (MS_U32)eEvent));
            return E_MVD_RET_INVALID_PARAM;
        }
    }
    return E_MVD_RET_OK;
}

#endif //MVD_ENABLE_ISR


//------------------------------------------------------------------------------
/// Set AVSync mode for file mode.
/// @param -eSyncMode: avsync mode for file mode
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetFileModeAVSync(MVD_TIMESTAMP_TYPE eSyncMode)
{
    _eFileSyncMode = eSyncMode;
    MVD_DEBUGINFO(printf("%s eSyncMode=%d\n", __FUNCTION__, _eFileSyncMode));
    return HAL_MVD_SetFileModeAVSync(eSyncMode);
}


//-----------------------------------------------------------------------------
/// Is MVD firmware command finished.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): not finished.
/// @retval     -TRUE(1): well done!
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_CMD eCmd)
{

    MVD_CMD_HANDSHADE_INDEX u32CmdState;
    MS_BOOL bCmdRdy = FALSE;
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s err: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return FALSE;
    }

    u32CmdState.value = HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_CMD_HANDSHAKE_INDEX);
    MVD_DEBUGINFO(printf("u32CmdState.value = 0x%x\n", u32CmdState.value));
    switch (eCmd)
    {
        case MVD_HANDSHAKE_PAUSE:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_pause);
            break;
        case MVD_HANDSHAKE_SLQ_RST:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_slq_reset);
            break;
        case MVD_HANDSHAKE_STOP:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_stop);
            break;
        case MVD_HANDSHAKE_SKIP_DATA:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_skip_data);
            break;
        case MVD_HANDSHAKE_SINGLE_STEP:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_single_step);
            break;
        case MVD_HANDSHAKE_SCALER_INFO:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_scaler_data_ready);
            break;
        case MVD_HANDSHAKE_GET_NXTDISPFRM:  //uniplayer
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_get_nextdispfrm_ready);
            break;
        case MVD_HANDSHAKE_PARSER_RST:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_parser_rst);
            break;
        case MVD_HANDSHAKE_RST_CC608:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_cc608_rst);
            break;
        case MVD_HANDSHAKE_RST_CC708:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_cc708_rst);
            break;
        default:
            bCmdRdy = FALSE;
            break;
    }
    return bCmdRdy;

}

//-----------------------------------------------------------------------------
/// Clear MVD firmware command finished bit.
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): not finished.
/// @retval     -TRUE(1): well done!
//-----------------------------------------------------------------------------
static MS_BOOL MDrv_MVD_ClearCmdFinished(MVD_HANDSHAKE_CMD eCmd)
{

    MVD_CMD_HANDSHADE_INDEX u32CmdState;
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s err: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return FALSE;
    }

    u32CmdState.value = HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_CMD_HANDSHAKE_INDEX);
    MVD_DEBUGINFO(printf("before CLR u32CmdState.value = 0x%x\n", u32CmdState.value));

    if (MVD_HANDSHAKE_SCALER_INFO == eCmd)
    {
        u32CmdState.mvdcmd_handshake_scaler_data_ready = 0;
        //write the value back: may it overwrite the value that f/w is supposed to write?
        HAL_MVD_MemWrite4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_CMD_HANDSHAKE_INDEX, u32CmdState.value);
        MVD_DEBUGINFO(printf("after CLR u32CmdState.value = 0x%lx\n",
                      HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_CMD_HANDSHAKE_INDEX)));
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/// @brief Check if all of the buffers(display, decoded, bitstream) are empty.
/// @return - TRUE / FALSE
/// @retval     -FALSE(0): Not Empty.
/// @retval     -TRUE(1): Empty.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_IsAllBufferEmpty(void)
{
    MVD_DEBUGINFO(printf("%s stat=0x%x, cmd=0x%x\n", __FUNCTION__,
                  MDrv_MVD_GetDecodeStatus(), MDrv_MVD_GetLastCmd()));
#if defined(CHIP_T2)
    return ((MDrv_MVD_GetDecodeStatus()!=E_MVD_STAT_FIND_SPECIALCODE)
         && (MDrv_MVD_GetDecodeStatus()!=E_MVD_STAT_WAIT_VDFIFO)
         && (MDrv_MVD_GetLastCmd()==CMD_PAUSE));
#else
    return MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SKIP_DATA);
#endif
}


//-----------------------------------------------------------------------------
/// @brief Generate specific pattern to support some special function.
/// @param -u32VAddr \b IN : the virtual address of specific pattern
/// @param -u32Size \b IN, OUT :
///                             IN: the input array size.
///                             OUT: the used array size.
/// @return -The result of command generate specific pattern
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GenPattern(MVD_PatternType ePattern, MS_PHYADDR u32PAddr, MS_U32* pu32Size)
{
    MS_U32* pu32DummyData = NULL;

    if ((!pu32Size) || (*pu32Size < SKIP_PATTERN_SIZE))
    {
        return FALSE;
    }

    pu32DummyData = (MS_U32 *) HAL_MVD_PA2NonCacheSeg(u32PAddr);

    switch (ePattern)
    {
        case E_MVD_PATTERN_FLUSH:
            *pu32DummyData = SKIP_PATTERN_0;
            pu32DummyData++;
            *pu32DummyData = SKIP_PATTERN_1;
            *pu32Size = SKIP_PATTERN_SIZE;
            //printf("##########FLUSH!, 0x%lx 0x%lx\n",*pu32DummyData, *(pu32DummyData-1));
            break;
        case E_MVD_PATTERN_FILEEND:
            if ((E_MVD_CODEC_FLV == curCodecType)||(E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER == curCodecType))
            {
                *pu32DummyData = FLV_PATTERN;
                pu32DummyData++;
                *pu32DummyData = 0xffffffff;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_1;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_2;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_3;
                //printf("##########FileEnd for FLV/SVH!, *pu32DummyData=%lx\n",*pu32DummyData);
            }
            else if (E_MVD_CODEC_DIVX311 == curCodecType)
            {
                *pu32DummyData = DIVX_PATTERN;
                pu32DummyData++;
                *pu32DummyData = 0xffffffff;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_1;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_2;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_3;
                //printf("##########FileEnd for DIVX311!, *pu32DummyData=%lx\n",*pu32DummyData);
            }
            else if ((E_MVD_CODEC_MPEG2 == curCodecType)||(E_MVD_CODEC_MPEG4 == curCodecType))
            {
                *pu32DummyData = MPEG_PATTERN_0;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_1;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_2;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_3;
                //printf("##########FileEnd for MPEG2/4!, *pu32DummyData=%lx\n",*pu32DummyData);
            }
            else
            {
                *pu32DummyData = END_PATTERN_0;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_1;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_2;
                pu32DummyData++;
                *pu32DummyData = END_PATTERN_3;
                //printf("##########FileEnd for VC1!, *pu32DummyData=%lx\n",*pu32DummyData);
            }
            *pu32Size = 16;
            break;
        default:
            break;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/// @brief Get driver specific data information
/// @return -the information of choosed type
//-----------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetPatternInfo(void)
{
    return MVD_U32_MAX;
}

//-----------------------------------------------------------------------------
/// @brief Pass scalar parameters to f/w
/// @return -The result of command.
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetDynScalingParam(MS_PHYADDR u32StAddr, MS_U32 u32Size)
{

    #define SCALER_INFO_TIMEOUT 0x1000
    MS_U32 u32TimeOut = 0;
    MS_U32 u32SrcAdd = NULL;
    MS_U32 i;

    if ((u32StAddr==NULL) || (u32Size==0) || (u32Size>MVD_FW_SCALER_INFO_BUF_LEN))
    {
        MVD_DEBUGERROR(printf("%s invalid para u32StAddr=0x%lx, u32Size=0x%lx\n",
                       __FUNCTION__, u32StAddr, u32Size));
        return E_MVD_RET_INVALID_PARAM;
    }
    if (TRUE != stMemCfg.bEnableDynScale)
    {
        MVD_DEBUGERROR(printf("%s !bEnableDynScale\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //copy data
    u32SrcAdd = HAL_MVD_PA2NonCacheSeg(u32StAddr);
    u32Size = ((u32Size+3)>>2)<<2;
    MVD_DEBUGINFO(printf("u32Size= 0x%lx, u32SrcAdd= 0x%lx\n", u32Size, u32SrcAdd));
    for (i=0; i<u32Size; i=i+4)
    {
        HAL_MVD_MemWrite4Byte(u32ScalerInfoAdd+i, *(volatile MS_U32*)(u32SrcAdd+i));
        MVD_DEBUGINFO(printf("0x%lx = 0x%lx\n", u32ScalerInfoAdd+i, HAL_MVD_MemRead4Byte(u32ScalerInfoAdd+i)));
    }

    //notify f/w
    if (TRUE!=HAL_MVD_SetScalerInfoAddr(u32ScalerInfoAdd)) //Set the buffer address (MIU offset) to f/w
    {
        MVD_DEBUGERROR(printf("%s fail to set ScalerInfoAdd\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //check f/w already handle the data
    while((TRUE!=MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_SCALER_INFO)) && (u32TimeOut < SCALER_INFO_TIMEOUT))
    {
        u32TimeOut++;
    }
    if(u32TimeOut >= SCALER_INFO_TIMEOUT)
    {
        MVD_DEBUGERROR(printf("%s timeout!!!\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //clear ack bit
    MDrv_MVD_ClearCmdFinished(MVD_HANDSHAKE_SCALER_INFO);

    MVD_DEBUGINFO(printf("=====> %s u32TimeOut = 0x%lx\n", __FUNCTION__, u32TimeOut));
    return E_MVD_RET_OK;

}

//------------------------------------------------------------------------------
/// Set the dynamic scale base address
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetDynamicScaleAddr(MS_U32 u32addr)
{
    MS_U32 u32offset;
    u32offset = MVD_GetMemOffset(u32addr);
    return HAL_MVD_SetDynamicScaleAddr(u32offset);
}

//------------------------------------------------------------------------------
/// Set virtual box width/height to F/W.
/// F/W will use the same w/h as scaler to calculate scaling factor.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetVirtualBox(MS_U16 u16Width, MS_U16 u16Height)
{
    MVD_DEBUGINFO(printf("%s: w=0x%x h=0x%x\n", __FUNCTION__, u16Width, u16Height));
    return HAL_MVD_SetVirtualBox(u16Width, u16Height);
}

//------------------------------------------------------------------------------
/// Set blue screen
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetBlueScreen(MS_BOOL bEn)
{
    return HAL_MVD_SetBlueScreen(bEn);
}

//------------------------------------------------------------------------------
/// Enable/Disable VSync interrupt
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_EnableInt(MS_U32 bEn)
{

    MVD_CmdArg mvdcmd;
    MS_U16 u16IntFlag = 0;

    u16IntFlag = (((bEn & E_MVD_EVENT_USER_DATA) == E_MVD_EVENT_USER_DATA) ? INT_USER_DATA : 0) |
                 (((bEn & E_MVD_EVENT_USER_DATA_DISP) == E_MVD_EVENT_USER_DATA_DISP) ? INT_USER_DATA_DISP : 0) |
                 (((bEn & E_MVD_EVENT_PIC_FOUND) == E_MVD_EVENT_PIC_FOUND) ? INT_PIC_FOUND : 0) |
                 (((bEn & E_MVD_EVENT_FIRST_FRAME) == E_MVD_EVENT_FIRST_FRAME) ? INT_FIRST_FRAME : 0) |
                 (((bEn & E_MVD_EVENT_DISP_RDY) == E_MVD_EVENT_DISP_RDY) ? INT_DISP_RDY : 0) |
                 (((bEn & E_MVD_EVENT_SEQ_FOUND) == E_MVD_EVENT_SEQ_FOUND) ? INT_SEQ_FOUND : 0)|
                 (((bEn & E_MVD_EVENT_DEC_I) == E_MVD_EVENT_DEC_I) ? INT_DEC_I : 0);
    if (((bEn & E_MVD_EVENT_DISP_VSYNC) == E_MVD_EVENT_DISP_VSYNC) ||
        ((bEn & E_MVD_EVENT_UNMUTE) == E_MVD_EVENT_UNMUTE))
    {
        u16IntFlag |= INT_DISP_VSYNC;
    }

    MVD_DEBUGINFO(printf("u16IntFlag = 0x%x\n", u16IntFlag));
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u16IntFlag & 0xff;
    mvdcmd.Arg1 = (u16IntFlag>>8) & 0xff;
    if (HAL_MVD_MVDCommand(CMD_ENABLE_INT_STAT, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_ENABLE_INT_STAT ) );
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// Enable FW only show one field.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_EnableDispOneField(MS_BOOL bEn)
{
#if defined(CHIP_T2) || defined(CHIP_T7)
    return E_MVD_RET_FAIL;
#else
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(stCmdArg);
    if(TRUE == bEn)
    {
        stCmdArg.Arg0 = 1;
    }
    if (HAL_MVD_MVDCommand(CMD_SHOW_ONE_FIELD, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_SHOW_ONE_FIELD ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
#endif
}


static E_MVD_Result MVD_RstFrmInfo(MVD_FrmInfoType eType)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    if (NULL == u32DecFrmInfoAdd)
    {
        return E_MVD_RET_FAIL;
    }

    //printf("%s u32DecFrmInfoAdd = 0x%lx\n", __FUNCTION__, u32DecFrmInfoAdd);

    if (E_MVD_FRMINFO_DECODE == eType)
    {
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_LUMAADDR), _INIT_ADDR);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_CHROMAADDR), _INIT_ADDR);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_TIMESTAMP), _INIT_TIMESTAMP);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_ID_L), _INIT_ID);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_ID_H), _INIT_ID);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_PITCH), _INIT_LEN);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_WIDTH), _INIT_LEN);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_HEIGHT), _INIT_LEN);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_FRAMETYPE), 0xff);
    }
    else if (E_MVD_FRMINFO_DISPLAY == eType)
    {
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_LUMAADDR), _INIT_ADDR);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_CHROMAADDR), _INIT_ADDR);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_TIMESTAMP), _INIT_TIMESTAMP);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_ID_L), _INIT_ID);
        HAL_MVD_MemWrite4Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_ID_H), _INIT_ID);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_PITCH), _INIT_LEN);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_WIDTH), _INIT_LEN);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_HEIGHT), _INIT_LEN);
        HAL_MVD_MemWrite2Byte((u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_FRAMETYPE), 0xff);
    }
    else
    {
        eRet = E_MVD_RET_INVALID_PARAM;
    }

    return eRet;
}


static MS_BOOL MVD_IsNextDispFrmRdy(void)
{
    #define NXT_DISP_TIMEOUT 20000//0x20
    MS_U32 u32TimeOut = 0;
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand(CMD_GET_NEXTDISPFRM, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_GET_NEXTDISPFRM) );
        return FALSE;
    }
    //HAL_MVD_Delayms(4);

    while((TRUE != MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_GET_NXTDISPFRM))
        && (u32TimeOut < NXT_DISP_TIMEOUT))
    {
        u32TimeOut++;
    }
    if(u32TimeOut >= NXT_DISP_TIMEOUT)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_GET_NEXTDISPFRM ) );
        return FALSE;
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get extension info of the display .
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetExtDispInfo(MVD_ExtDispInfo* pInfo)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    if (NULL == pInfo)
    {
        MVD_DEBUGERROR(printf("GetExtDispInfo NULL pInfo!\n"));
        return E_MVD_RET_INVALID_PARAM;
    }

    if (pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("GetExtDispInfo NULL pu8MVDGetFrameInfoBufStart!\n"));
        return E_MVD_RET_FAIL;
    }

    pInfo->u16VSize  = HAL_MVD_MemRead2Byte(pu8MVDGetFrameInfoBufStart+OFFSET_DISP_V_SIZE);
    pInfo->u16HSize  = HAL_MVD_MemRead2Byte(pu8MVDGetFrameInfoBufStart+OFFSET_DISP_H_SIZE);
    pInfo->u16VOffset = HAL_MVD_MemRead2Byte(pu8MVDGetFrameInfoBufStart+OFFSET_CENTRE_V_OFFSET);
    pInfo->u16HOffset = HAL_MVD_MemRead2Byte(pu8MVDGetFrameInfoBufStart+OFFSET_CENTRE_H_OFFSET);

    return eRet;
}

//------------------------------------------------------------------------------
/// Get info of the decoded/displaying frame.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetFrmInfo(MVD_FrmInfoType eType, MVD_FrmInfo* pInfo)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    if (NULL == pInfo)
    {
        MVD_DEBUGERROR(printf("GetFrmInfo NULL pInfo!\n"));
        return E_MVD_RET_INVALID_PARAM;
    }
    if (NULL == u32DecFrmInfoAdd)
    {
        MVD_DEBUGERROR(printf("GetFrmInfo NULL u32DecFrmInfoAdd!\n"));
        return E_MVD_RET_FAIL;
    }

    //printf("%s u32DecFrmInfoAdd = 0x%lx\n", __FUNCTION__, u32DecFrmInfoAdd);

    if (E_MVD_FRMINFO_DECODE == eType)
    {
        pInfo->u32LumaAddr  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_LUMAADDR);
        pInfo->u32ChromaAddr= HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_CHROMAADDR);
        pInfo->u32TimeStamp = _90K_TO_MS(HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_TIMESTAMP));
        pInfo->u32ID_L  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_ID_L);
        pInfo->u32ID_H  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_ID_H);
        pInfo->u16Pitch = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_PITCH);
        pInfo->u16Width = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_WIDTH);
        pInfo->u16Height= HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_HEIGHT);
        pInfo->eFrmType = (MVD_PicType)HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DEC_FRAMETYPE);
    }
    else if (E_MVD_FRMINFO_DISPLAY == eType)
    {
        pInfo->u32LumaAddr  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_LUMAADDR);
        pInfo->u32ChromaAddr= HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_CHROMAADDR);
        pInfo->u32TimeStamp = _90K_TO_MS(HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_TIMESTAMP));
        pInfo->u32ID_L  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_ID_L);
        pInfo->u32ID_H  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_ID_H);
        pInfo->u16Pitch = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_PITCH);
        pInfo->u16Width = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_WIDTH);
        pInfo->u16Height= HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_HEIGHT);
        pInfo->eFrmType = (MVD_PicType)HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_DISP_FRAMETYPE);
    }

    else if (E_MVD_FRMINFO_NEXT_DISPLAY == eType)
    {
        if (!MVD_IsNextDispFrmRdy())
        {
            MVD_DEBUGINFO(printf("NextDispFrm not ready!\n"));
            return E_MVD_RET_FAIL;
        }
        pInfo->u16FrmIdx= HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_FRAMEIDX);
        if (pInfo->u16FrmIdx  == 0xFFFF)
        {
            MVD_DEBUGINFO(printf("GetFrmInfo no available frame!\n"));
            return E_MVD_RET_FAIL;
        }
        pInfo->u32LumaAddr  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_LUMAADDR);
        pInfo->u32ChromaAddr= HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_CHROMAADDR);
        // 64BIT_PTS = (TimeStamp | (ID_H<<32)) , unit: 90K
        pInfo->u32TimeStamp = (HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_TIMESTAMP));
        pInfo->u32ID_L  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_ID_L);
        pInfo->u32ID_H  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_ID_H);
        pInfo->u16Pitch = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_PITCH);
        pInfo->u16Width = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_WIDTH);
        pInfo->u16Height= HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_HEIGHT);
        pInfo->eFrmType = (MVD_PicType)HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_FRAMETYPE);

#if 0
    printf("NxtFrm:: Idx=0x%x, ", pInfo->u16FrmIdx);
    printf("Type=0x%x, ", pInfo->eFrmType      );
    printf("Luma=0x%lx, ", pInfo->u32LumaAddr  );
    printf("Chroma=0x%lx, ", pInfo->u32ChromaAddr);
    printf("Pts=%lu, ", pInfo->u32TimeStamp );
    printf("ID_H=%lu, ", pInfo->u32ID_H      );
    printf("ID_L=0x%lx\n", pInfo->u32ID_L      );
#endif
        MVD_DBG_STS(printf("<<< drvMVD pts,idH = %lu, %lu\n", pInfo->u32TimeStamp, pInfo->u32ID_H));
        //MVD_DumpFrmInfo(pInfo);
    }
    else
    {
        eRet = E_MVD_RET_INVALID_PARAM;
    }

    if ((pInfo->u32LumaAddr  == _INIT_ADDR) || (pInfo->u32ChromaAddr== _INIT_ADDR) ||
        (pInfo->u16Pitch == _INIT_LEN) || (pInfo->u16Width == _INIT_LEN) ||
        (pInfo->u16Height== _INIT_LEN))
    {
        MVD_DEBUGINFO(printf("GetFrmInfo not ready!\n"));
        return E_MVD_RET_FAIL;
    }

    if (stMemCfg.bHWMiuSel == MIU_SEL_0)
    {
        pInfo->u32LumaAddr = pInfo->u32LumaAddr * 8;
        pInfo->u32ChromaAddr = pInfo->u32ChromaAddr * 8;
    }
    else
    {
        pInfo->u32LumaAddr = pInfo->u32LumaAddr * 8 + stMemCfg.u32Miu1BaseAddr;
        pInfo->u32ChromaAddr = pInfo->u32ChromaAddr * 8 + stMemCfg.u32Miu1BaseAddr;
    }

    //printf("===> Luma=0x%lx, Chroma=0x%lx\n", pInfo->u32LumaAddr, pInfo->u32ChromaAddr);
    return eRet;
}

//------------------------------------------------------------------------------
/// Get time_code of the decoded/displaying frame.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_GetTimeCode(MVD_FrmInfoType eType, MVD_TimeCode* pInfo)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    if (NULL == pInfo)
    {
        return E_MVD_RET_INVALID_PARAM;
    }
    if (NULL == pu8MVDGetFrameInfoBufStart)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetTimeCode error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return E_MVD_RET_FAIL;
    }

    if (E_MVD_FRMINFO_DECODE == eType)
    {
        pInfo->u8TimeCodeHr  = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_HOURS);
        pInfo->u8TimeCodeMin = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_MINUTES);
        pInfo->u8TimeCodeSec = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_SECONDS);
        pInfo->u8TimeCodePic = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_PICTURES);
        pInfo->u8DropFrmFlag = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_DROP_FRAME_FLAG);
    }
    else if (E_MVD_FRMINFO_DISPLAY == eType)
    {
        pInfo->u8TimeCodeHr  = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_HOURS_DISP);
        pInfo->u8TimeCodeMin = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_MINUTES_DISP);
        pInfo->u8TimeCodeSec = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_SECONDS_DISP);
        pInfo->u8TimeCodePic = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_TIME_CODE_PICTURES_DISP);
        pInfo->u8DropFrmFlag = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_DROP_FRAME_FLAG_DISP);
    }
    else
    {
        eRet = E_MVD_RET_INVALID_PARAM;
    }

    return eRet;
}

MS_BOOL MDrv_MVD_GetUsrDataIsAvailable(void)
{
#if defined(MVD_SUPPORT_X4_CC)
    u32UsrDataWr = MDrv_CC_CM_GetMVDRB_HWAddr(4)>>3;
#else
    u32UsrDataWr = MDrv_CC_CM_GetMVDRB_HWAddr(2)>>3;
#endif

    MVD_DEBUGINFO(printf("IsAvail:%x rd=%lx wr=%lx\n", !(u32UsrDataRd == u32UsrDataWr), u32UsrDataRd, u32UsrDataWr));
    return !(u32UsrDataRd == u32UsrDataWr);
}

//------------------------------------------------------------------------------
/// Get info of user data
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetUsrDataInfo(MVD_UsrDataInfo* pUsrInfo)
{
#if defined(MVD_SUPPORT_X4_CC)
    FW_USER_DATA_BUF_EXT stUsrDataExt;
#else
    FW_USER_DATA_BUF stUsrDataInfo;
#endif

    if ((!pUsrInfo) || (pu8MVDGetFrameInfoBufStart==0))
    {
        MVD_DEBUGERROR(printf("%s: NULL ptr.\n", __FUNCTION__));
        return FALSE;
    }

    //get write pointer
#if defined(MVD_SUPPORT_X4_CC)
    u32UsrDataWr = MDrv_CC_CM_GetMVDRB_HWAddr(4)>>3;
#else
    u32UsrDataWr = MDrv_CC_CM_GetMVDRB_HWAddr(2)>>3;
#endif
    if (u32UsrDataRd == (_stInternalMemCfg.u32UserDataBuf+MVD3_FW_USER_DATA_BUF_LEN))
    {
        u32UsrDataRd = _stInternalMemCfg.u32UserDataBuf; //wrap to BufStart
    }
    MVD_DEBUGINFO(printf("CC Rd=0x%lx Wr=0x%lx\n", u32UsrDataRd, u32UsrDataWr));

    if (u32UsrDataRd == u32UsrDataWr)
    {
        MVD_DEBUGERROR(printf("%s: no data?\n", __FUNCTION__));
        return FALSE;
    }

    //miuOffset --> physical add --> noncached add
    u32UsrData = (stMemCfg.bFWMiuSel==MIU_SEL_1)?(u32UsrDataRd+stMemCfg.u32Miu1BaseAddr):(u32UsrDataRd);
    u32UsrData = HAL_MVD_PA2NonCacheSeg(u32UsrData);

    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();

#if defined(MVD_SUPPORT_X4_CC)
    stUsrDataExt = *(volatile FW_USER_DATA_BUF_EXT*)u32UsrData;

    pUsrInfo->u32Pts = stUsrDataExt.pts;
    pUsrInfo->u8PicStruct = stUsrDataExt.PicStruct;
    pUsrInfo->u8PicType   = stUsrDataExt.picType;
    pUsrInfo->u8TopFieldFirst = stUsrDataExt.top_ff;
    pUsrInfo->u8RptFirstField = stUsrDataExt.rpt_ff;
    pUsrInfo->u16TmpRef = stUsrDataExt.tmpRef;
    pUsrInfo->u8ByteCnt = stUsrDataExt.userdatabytecnt;
    pUsrInfo->u32DataBuf = u32UsrData + MVD_FW_USER_DATA_EXT_HDR_LEN;
#else
    stUsrDataInfo = *(volatile FW_USER_DATA_BUF*)u32UsrData;

    //pUsrInfo->u8PicStruct = HAL_MVD_MemReadByte(pu8MVDGetFrameInfoBufStart+OFFSET_PICTURE_STRUCTURE);
    pUsrInfo->u8PicType   = stUsrDataInfo.picType;
    pUsrInfo->u8TopFieldFirst = stUsrDataInfo.top_ff;
    pUsrInfo->u8RptFirstField = stUsrDataInfo.rpt_ff;
    pUsrInfo->u16TmpRef = stUsrDataInfo.tmpRef;
    pUsrInfo->u8ByteCnt = stUsrDataInfo.userdatabytecnt;
    pUsrInfo->u32DataBuf = u32UsrData + MVD_FW_USER_DATA_HDR_LEN;
#endif
    //update read pointer
    u32UsrDataRd += MVD_FW_USER_DATA_PKT_LEN;
#if 0
    printf("xxInfo: ");
    printf("%d, ", pUsrInfo->u16TmpRef);
    printf("%d, ", pUsrInfo->u8PicStruct);
    printf("%d, ", pUsrInfo->u8TopFieldFirst);
    printf("0x%lx, ", pUsrInfo->u32DataBuf);
    printf("%d, ", pUsrInfo->u8ByteCnt);
    printf("%ld, ", pUsrInfo->u32Pts);
    printf("%d\n", pUsrInfo->u8PicType);
#endif
    return TRUE;
}

//------------------------------------------------------------------------------
/// Enable/Disable freezing display
/// Once this flag is set, firmware continue decode new frame
/// but not to push this frame into display queue.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetFreezeDisp(MS_BOOL bEn)
{
    MDrv_MVD_SetSpeed(E_MVD_SPEED_DEFAULT, 1);
    if (TRUE == HAL_MVD_SetFreezeDisp(bEn))
        return E_MVD_RET_OK;
    else
        return E_MVD_RET_FAIL;
}


//------------------------------------------------------------------------------
/// Get MVD displayed picture counter
/// @return - displayed picture counter
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetDispCount(void)
{
#if defined(CHIP_T2) || defined(CHIP_T7)
    return 0;
#else
    MVD_FUNC_ENTRY();
    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("MDrv_MVD_GetDispCount error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return 0;
    }

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_DISPLAYED_CNT);
#endif
}


//------------------------------------------------------------------------------
/// Set MVD fd mask delay count
/// 16bits and unit in vsync for mute the fd_mask
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetFdMaskDelayCount(MS_U16 u16Cnt)
{
#if defined(CHIP_T2) || defined(CHIP_T7)
    return E_MVD_RET_FAIL;
#else
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(stCmdArg);

    //16bits and unit in vsync for mute the fd_mask
    stCmdArg.Arg0 = u16Cnt & 0xff;
    stCmdArg.Arg1 = (u16Cnt>>8) & 0xff;

    if (HAL_MVD_MVDCommand(CMD_FD_MASK_DELAY_CNT, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FD_MASK_DELAY_CNT ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
#endif
}

//------------------------------------------------------------------------------
/// Set MVD output frame rate convert
/// @param u8FrameRate \b IN : output frame rate of 8bits and unit in vsync
/// @param u8Interlace \b IN : scan style 0:progress, 1:interlace
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetOutputFRCMode(MS_U8 u8FrameRate, MS_U8 u8Interlace)
{
#if defined(CHIP_T7)
    return E_MVD_RET_FAIL;
#else
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();

    MVD_DEBUGINFO(printf("%s u8Interlace is unused.\n", __FUNCTION__));

    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8FrameRate;
    if (HAL_MVD_MVDCommand(CMD_FRC_OUPUT, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FRC_OUPUT ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
#endif
}

//------------------------------------------------------------------------------
/// Set MVD FRC drop type.
/// @param u8DropType \b IN : drop type. 0:drop frame, 1:drop field. default:0
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetFRCDropType(MS_U8 u8DropType)
{
#if defined(CHIP_T7)
    return E_MVD_RET_FAIL;
#else
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();

    if((u8DropType != FRC_DROP_FRAME)
    && (u8DropType != FRC_DROP_FIELD))
    {
        return E_MVD_RET_FAIL;
    }

    SETUP_CMDARG(stCmdArg);

    stCmdArg.Arg0 = u8DropType;

    if (HAL_MVD_MVDCommand(CMD_FRC_DROP_BEHAVIOR, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FRC_DROP_BEHAVIOR ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
#endif
}

//------------------------------------------------------------------------------
/// Set disable resolution change.
/// @return -E_MVD_RET_OK for success; E_MVD_RET_FAIL for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetDisableSeqChange(MS_BOOL bEnable)
{
#if defined(CHIP_T7)
    return E_MVD_RET_FAIL;
#else
    MVD_CmdArg stCmdArg;
    MVD_FUNC_ENTRY();
    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = bEnable;

    if (HAL_MVD_MVDCommand(0x8F, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", 0x8F ) );
        return E_MVD_RET_FAIL;
    }
    MVD_DEBUGINFO(printf("MVD CMD_FORBID_RESOLUTION_CHANGE(0x%x) OK\n", 0x8F));

    return E_MVD_RET_OK;
#endif
}

//------------------------------------------------------------------------------
MVD_InternalMemCfg *MDrv_MVD_GetInternalMemCfg(void)
{
    return &_stInternalMemCfg;
}

//------------------------------------------------------------------------------
/// Debug function to dump useful info.
//------------------------------------------------------------------------------
void MDrv_MVD_DbgDump(void)
{
    MS_U32 u32VdCnt=0;
    static MS_U32 u32PreVdCnt=0;
    MS_U32 u32ErrCnt=0;
    static MS_U32 u32PreErrCnt=0;
    static MS_BOOL b1stDump = TRUE;

    MVD_FrmInfo stFrm = {_INIT_ADDR, _INIT_ADDR, _INIT_TIMESTAMP, _INIT_ID,
                         _INIT_ID, _INIT_LEN, _INIT_LEN, _INIT_LEN, 0xff, E_MVD_PIC_UNKNOWN};


    if (!_bDrvInit)
    {
        printf("%s: _bDrvInit false!\n", __FUNCTION__);
        return;
    }

    if (b1stDump)
    {
        printf("curDisablePESParsing = %d\n", curDisablePESParsing);
        b1stDump = FALSE;
    }

    u32VdCnt=MDrv_MVD_GetParserByteCnt();
    u32ErrCnt=MDrv_MVD_GetVldErrCount();
    printf("input: vfifo=%d(full=%d,empty=%d), vdCnt=%ld(%ld), vldErr=%ld(%ld); ",
            HAL_MVD_RegReadByte(0x1564)>>6, HAL_MVD_RegReadByte(0x1564)&0x20,
            HAL_MVD_RegReadByte(0x1564)&0x10, u32VdCnt, (u32VdCnt-u32PreVdCnt),
            u32ErrCnt, (u32ErrCnt-u32PreErrCnt));
    u32PreVdCnt = u32VdCnt;
    u32PreErrCnt = u32ErrCnt;
    printf("state: fw=0x%x, lastCmd=0x%x, pc=0x%lx\n",
            MDrv_MVD_GetDecodeStatus(), MDrv_MVD_GetLastCmd(), HAL_VPU_GetProgCnt());
    printf("seq(%d): w=%d, h=%d, i/p=%x, fps=%ld; ", MDrv_MVD_GetDispRdy(), stPreFrmInfo.u16HorSize,
            stPreFrmInfo.u16VerSize, stPreFrmInfo.u8Interlace, stPreFrmInfo.u32FrameRate);
    printf("cnt: dec=%ld, skip=%ld, drop=%ld\n", MDrv_MVD_GetPicCounter(), MDrv_MVD_GetSkipPicCounter(), 0x0UL);
    printf("avsync on=%x, delay=%ld, tolerance=%d, done=%x, skip=%x, repeat=%x, pts=%ldms\n",
            stSyncCfg.bEnable, stSyncCfg.u32Delay, stSyncCfg.u16Tolerance, (MDrv_MVD_GetSyncStatus()==1),
            MDrv_MVD_GetIsSyncSkip(), MDrv_MVD_GetIsSyncRep(), MDrv_MVD_GetPTS());
    MDrv_MVD_GetFrmInfo(E_MVD_FRMINFO_DECODE, &stFrm);
    printf("frm Dec Y=%lx UV=%lx Pitch=%x; ", stFrm.u32LumaAddr, stFrm.u32ChromaAddr, stFrm.u16Pitch);
    MDrv_MVD_GetFrmInfo(E_MVD_FRMINFO_DISPLAY, &stFrm);
    printf("Disp Y=%lx UV=%lx Pitch=%x\n", stFrm.u32LumaAddr, stFrm.u32ChromaAddr, stFrm.u16Pitch);
    if (curSrcMode == E_MVD_SLQ_TBL_MODE)
    {
        printf("fe=%lx, rd=%lx(%lx), wr=%lx, empty=%lx; ", u32FileEndPtr, _drvSlqTbl.u32RdPtr,
                MVD_Map2DrvSlqTbl(MDrv_MVD_GetSLQReadPtr()), _drvSlqTbl.u32WrPtr, _drvSlqTbl.u32Empty);
        printf("es rd=0x%lx, wr=0x%lx\n", MDrv_MVD_GetESReadPtr(), MDrv_MVD_GetESWritePtr());
    }
    else if (curSrcMode == E_MVD_TS_FILE_MODE)
    {
        printf("es rd=0x%lx, wr=0x%lx\n", MDrv_MVD_GetESReadPtr(), MDrv_MVD_GetESWritePtr());
    }
}

//------------------------------------------------------------------------------
/// Dump the bitstream to predefined buffer address.
/// Before PLAY command, set the bitstream base & bitstream length, and then
/// CPU would continue to dump bitstream at the base address.
/// @param -u32base \b IN : start address (MIU offset, e.g. 128M==>0MB)
/// @param -u32size \b IN : size (bytes)
//------------------------------------------------------------------------------
void MDrv_MVD_DbgDumpBits(MS_PHYADDR u32base, MS_U32 u32size)
{

    MVD_CmdArg mvdcmd;

    u32base = MVD_GetMemOffset(u32base);
    printf("%s base=0x%lx size=0x%lx\n", __FUNCTION__, u32base, u32size);
    MS_ASSERT((u32base%8)==0);
    u32base >>= 3;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32base);
    mvdcmd.Arg1 = H_WORD(u32base);
    mvdcmd.Arg2 = L_DWORD(u32base);
    mvdcmd.Arg3 = H_DWORD(u32base);
    if (HAL_MVD_MVDCommand( CMD_DUMP_BITSTREAM_BASE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\n", CMD_DUMP_BITSTREAM_BASE ) );
        return;
    }

    MS_ASSERT((u32size%8)==0);
    u32size >>= 3;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32size);
    mvdcmd.Arg1 = H_WORD(u32size);
    mvdcmd.Arg2 = L_DWORD(u32size);
    mvdcmd.Arg3 = H_DWORD(u32size);
    if (HAL_MVD_MVDCommand( CMD_DUMP_BITSTREAM_LENGTH, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\n", CMD_DUMP_BITSTREAM_LENGTH ) );
        return;
    }
    return;

}


#if (defined(CHIP_T7))
//-----------------------------------------------------------------------------
/// MVD set VOP Done
/// @return - TRUE/FALSE
/// @retval     -FALSE(0): not finished.
/// @retval     -TRUE(1): well done!
//-----------------------------------------------------------------------------
void MDrv_Mvd_SetVOPDone(void)
{
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = VOP_INI_DONE;
    stCmdArg.Arg1 = 0;
    stCmdArg.Arg2 = 1;
    stCmdArg.Arg3 = 0;

    if (HAL_MVD_MVDCommand(CMD_SET_PARAMETER, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", VOP_INI_DONE ) );
        return;
    }

}

/********************************************************************************/
/// Set mvd play mode
/// @return -none
/********************************************************************************/
//need check

void MDrv_Mvd_SetPlayMode(MS_U8 bFileMode, MS_U8 bDisablePESParsing)
{
    u8MvdPlayMode = (MS_U8)((bFileMode?1:0)<<2) | (MS_U8)((bDisablePESParsing?1:0)<<3);
}

void MDrv_Mvd_CallDummy(void)
{
    u32MVDFWSLQTABTmpbufAdr = NULL;
    u32MVDFWPtsTblAddr = NULL;
    u32DecFrmInfoAdd = NULL;
    u32DynScalingAdd = NULL;
    u8DynScalingDepth = 0;
    u32ScalerInfoAdd = NULL;
    u32VolAdd = 0;
}
#endif
// only for VDEC internal link patch
MS_BOOL MDrv_MVD_LinkWeakSymbolPatch(void)
{
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set firmware as MStreamer mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_SetMStreamerMode(MS_U8 u8Mode)
{
    u8MstMode = u8Mode;
    return HAL_MVD_SetMStreamerMode(u8Mode);
}

//------------------------------------------------------------------------------
/// Flip the specified frame
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FrameFlip(MS_U8 u8FrmIdx)
{
    if (TRUE != MDrv_MVD_IsMStreamerMode())
    {
        MVD_DEBUGINFO(printf("Need to SetMStreamerMode before using this function\n"));
        return FALSE;
    }
    return HAL_MVD_FrameOpt(u8FrmIdx, E_MVD_FRAME_FLIP);
}

//------------------------------------------------------------------------------
/// Release the specified frame
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FrameRelease(MS_U8 u8FrmIdx)
{
    if (TRUE != MDrv_MVD_IsMStreamerMode())
    {
        MVD_DEBUGINFO(printf("Need to SetMStreamerMode before using this function\n"));
        return FALSE;
    }
    return HAL_MVD_FrameOpt(u8FrmIdx, E_MVD_FRAME_RELEASE);
}

//------------------------------------------------------------------------------
/// Reset MVD parser .  It is used for Reset MVD Parser.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_ParserRstDone(MS_BOOL bEnable)
{
    #define PARSER_RST_TIMEOUT 0x40000
    MS_U32 u32TimeOut = 0;
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg1 = bEnable;
    if (HAL_MVD_MVDCommand(CMD_PTS_TBL_RESET, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_PTS_TBL_RESET ) );
        return FALSE;
    }
    while((!(MDrv_MVD_IsCmdFinished(MVD_HANDSHAKE_PARSER_RST))) && (u32TimeOut < PARSER_RST_TIMEOUT))
    {
        u32TimeOut++;
    }

    if(u32TimeOut >= PARSER_RST_TIMEOUT)
    {
        MVD_DEBUGERROR( printf( "Ctrl: 0x%x fail!!\r\n", CMD_PTS_TBL_RESET ) );
        return FALSE;
    }
    return TRUE;
}
//------------------------------------------------------------------------------
/// Enable/Disable to capture the specified frame
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL MDrv_MVD_FrameCapture(MS_U8 u8FrmIdx, MS_BOOL bEnable)
{
    MS_BOOL ret = FALSE;

    if (TRUE != MDrv_MVD_IsMStreamerMode())
    {
        MVD_DEBUGINFO(printf("Need to SetMStreamerMode before using this function\n"));
        return FALSE;
    }

    ret = HAL_MVD_FrameCapture(u8FrmIdx, bEnable);

    //firmware will keep this frame until we release it.
    if ((TRUE == ret) && (TRUE != bEnable))
    {
        ret = MDrv_MVD_FrameRelease(u8FrmIdx);
    }

    return ret;
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_GetFrmRateIsSupported()
/// @brief \b Function \b Description:  Get if the framerate is supported
/// @return -The result of supported or not.
//-----------------------------------------------------------------------------
MS_BOOL MDrv_MVD_GetFrmRateIsSupported(void)
{
    MVD_FrameInfo stInfo;
    MDrv_MVD_GetFrameInfo(&stInfo);
    return HAL_MVD_GetFrmRateIsSupported(stInfo.u16HorSize, stInfo.u16VerSize, stInfo.u32FrameRate);
}

//------------------------------------------------------------------------------
/// set fw auto mute
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetAutoMute(MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    if (HAL_MVD_MVDCommand(CMD_ENABLE_AUTO_MUTE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_ENABLE_AUTO_MUTE) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

E_MVD_Result MDrv_MVD_SetVSizeAlign(MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    if (HAL_MVD_MVDCommand(CMD_FORCE_ALIGN_VSIZE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_FORCE_ALIGN_VSIZE) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

// Please set it before cmd "CMD_ENABLE_LAST_FRAME_SHOW"
// 0(default): for original just assert the last_frame_show_done when push to dispQ
// 1: for strict qualify the last_frame_show_done after the last_frame been displayed by mvop, 20120309
E_MVD_Result MDrv_MVD_SetDispFinishMode(MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode;

    MVD_DEBUGINFO(printf("%s CMD_ENABLE_LAST_FRAME_QUALIFIER arg0=%x\n", __FUNCTION__, u8Mode));
    if (HAL_MVD_MVDCommand(CMD_ENABLE_LAST_FRAME_QUALIFIER, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_ENABLE_LAST_FRAME_QUALIFIER) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

// Set AVSync Qualifier
// 0(default): for compatibility to original avsync bahavior
// 1: for enhance to do avsync when "enable_avsync=1" && "(lastcommand != CMD_PLAY)" for patch avsync on particular clip, 20120314
E_MVD_Result MDrv_MVD_SetAVSyncMode(MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode;

    MVD_DEBUGINFO(printf("%s CMD_ENABLE_AVSYNC_QUALIFIER arg0=%x\n", __FUNCTION__, u8Mode));
    if (HAL_MVD_MVDCommand(CMD_ENABLE_AVSYNC_QUALIFIER, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_ENABLE_AVSYNC_QUALIFIER) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Set IDCT mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetIdctMode(MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode;

    MVD_DEBUGINFO(printf("%s CMD_IDCT_SEL arg0=%x\n", __FUNCTION__, u8Mode));
    if (HAL_MVD_MVDCommand(CMD_IDCT_SEL, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_IDCT_SEL) );
        return E_MVD_RET_FAIL;
    }

    return E_MVD_RET_OK;
}

//------------------------------------------------------------------------------
/// Get MVD DivX plus Version,
/// @return -non DivX plus  \b OUT 0
//------------------------------------------------------------------------------
MS_U32 MDrv_MVD_GetDivxVer(void)
{
    MVD_FUNC_ENTRY();

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(printf("%s error!get FrameInfo buffer start=0\n", __FUNCTION__));
        return 0;
    }
    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart + OFFSET_DIVX_VER_5X);
}


//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_SetMVDClockSpeed()
/// @brief \b Function \b Description:  set mvd clock speed
/// @return - The result of setting mvd clock speed
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_SetMVDClockSpeed(MVD_ClockSpeed eClockSpeed)
{
#if defined(CHIP_A3) || defined(CHIP_K1)|| defined(CHIP_KELTIC) || defined(CHIP_KAPPA)
    HAL_MVD_ClockSpeed eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_NONE;
    switch (eClockSpeed)
    {
        case E_MVD_CLOCK_SPEED_HIGHEST:
            eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_HIGHEST;
            break;
        case E_MVD_CLOCK_SPEED_HIGH:
            eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_HIGH;
            break;
        case E_MVD_CLOCK_SPEED_MEDIUM:
            eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_MEDIUM;
            break;
        case E_MVD_CLOCK_SPEED_LOW:
            eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_LOW;
            break;
        case E_MVD_CLOCK_SPEED_LOWEST:
            eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_LOWEST;
            break;
        case E_MVD_CLOCK_SPEED_DEFAULT:
            eMVDClockSpeed = E_HAL_MVD_CLOCK_SPEED_DEFAULT;
            break;
        default:
            MVD_DEBUGERROR(printf("mvd clock setting is wrong(%d)\n", eClockSpeed));
            return E_MVD_RET_FAIL;
            break;
    }

    return HAL_MVD_SetClockSpeed(eMVDClockSpeed);
#else
    UNUSED(eClockSpeed);
    printf("This chip does not support mvd clock presetting~\n");
    return E_MVD_RET_FAIL;
#endif
}

//-----------------------------------------------------------------------------
/// @brief \b Function \b Name: MDrv_MVD_ShowFirstFrameDirect()
/// @brief \b Function \b Description:   Enable/ Disable to push first frame to display queue directly
/// @param -bEnable \b IN :  Enable/ Disable
///                 -FALSE(0): disable this mode
///                 -TRUE(1): enable this mode
/// @return -The result of command MDrv_MVD_ShowFirstFrameDirect
//-----------------------------------------------------------------------------
E_MVD_Result MDrv_MVD_ShowFirstFrameDirect(MS_BOOL bEnable)
{
    MVD_CmdArg stCmdArg;

    MVD_FUNC_ENTRY();
    SETUP_CMDARG(stCmdArg);

    stCmdArg.Arg0 = bEnable;

    MVD_DEBUGINFO(printf("%s CMD_PUSH_FIRST_FRAME_DISP arg0=%x\n", __FUNCTION__, bEnable));
    if (HAL_MVD_MVDCommand(CMD_PUSH_FIRST_FRAME_DISP, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( printf( "Command: 0x%x fail!!\r\n", CMD_PUSH_FIRST_FRAME_DISP) );
        return E_MVD_RET_FAIL;
    }

    return E_MVD_RET_OK;
}


