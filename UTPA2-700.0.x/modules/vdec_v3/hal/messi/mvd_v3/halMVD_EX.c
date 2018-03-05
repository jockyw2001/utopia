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
////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include <string.h>

#include "MsCommon.h"
#include "drvMVD_EX.h"
#include "drvBDMA.h"

// Internal Definition
#include "regMVD_EX.h"
#include "halMVD_EX.h"
#include "halVPU_EX.h"
#include "osalMVD_EX.h"
#include "mvd4_interface.h" //firmware header
#include "asmCPU.h"
#include "controller.h"
#include "MVD_EX_Common.h"
#include "halCHIP.h"
#include "drvCMAPool.h"
#if defined(MSOS_TYPE_LINUX)
#include "msos/linux/ion_uapi.h"
#include "msos/linux/mdrv_cma_pool_st.h"
#endif

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

#include "ULog.h"
#define VPRINTF(format,args...) ULOGI("VDEC", format, ##args)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define SLQ_NEW_PUSH       1
#define VIRTUAL_CMD_TIMEOUT 50
#ifdef VDEC3
#define v3_temp 1
#define v3_thinplayer 0
#else
#define v3_temp 0
#define v3_thinplayer 0
#endif

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define MIU1_BASEADDR       pMVDHalContext->stMiuCfg.u32Miu1BaseAddr
#define _PA2Offset(x)       (((x)>=MIU1_BASEADDR)?(x-MIU1_BASEADDR):(x))

#ifndef MS_ASSERT
#ifdef MS_DEBUG
#define MS_ASSERT(expr)     do {                                                        \
                                if(!(expr))                                             \
                                ULOGF("VDEC","MVD assert fail %s %d!\n", __FILE__, __LINE__); \
                            } while(0)
#else
#define MS_ASSERT(expr)
#endif
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif
//constant
#define MVD_PollingTimes      0x40000UL

#define MVD_DEBUGVERBAL(x)    if (_u8HalDbgLevel>3)  { (x); }
#define MVD_DEBUG_FWCMD(x)    if (_u8HalDbgLevel>2)  { (x); }
#define MVD_DEBUGINFO(x)      if (_u8HalDbgLevel>1)  { (x); }
#define MVD_DEBUGERROR(x)     if (_u8HalDbgLevel>0)  { (x); }
#define MVD_ERROR(x)          x

#define RIU     ((unsigned short volatile *) (u32RiuBaseAdd))
#define RIU8    ((unsigned char  volatile *) (u32RiuBaseAdd))

#define MVDCPU_ON_MIU1     ((HAL_MVD_RegReadByte(MIU0_SEL0_H) & BIT0) == BIT0)
#define MVDHW_ON_MIU1      ((HAL_MVD_RegReadByte(MIU0_SEL2_L) & BIT4) == BIT4)

// MVD5
#define SLQ_TBL_ENTRY_LEN        0x7FFFFFFF //31-bit

#define MVD_WIDTH_ALIGN_BYTE     16
#define MVD_WIDTH_ALIGN_MASK     (MVD_WIDTH_ALIGN_BYTE - 1)
#define MVD_WIDTH_ALIGN_BITS     4

#define MVD_FBNUM_DEFAULT        4
#define MVD_FBNUM_MAX            5
#define MVD_FBNUM_MIN            MVD_FBNUM_DEFAULT

#define MVD_DHD_FBSIZE           0x5FA000  //Framebuffer size minimum for Dual HD: (1920*2)*1088*1.5
#define MVD_HD_FBSIZE            0x2FD000  //Framebuffer size minimum for High Definition
#define MVD4_VC1_FBSIZE_HDMIN    0xEF1000  //5 * 1920 * 1088 * 1.5 = 14.95MB
#define MVD4_VC1_FBSIZE_HDMIN1   0xBF4000  //4 * 1920 * 1088 * 1.5 = 11.96MB
#define MVD4_VC1_FBSIZE_SDMIN    0x2F7600  //5 * 720 * 576 * 1.5 = 2.97MB
#define MVD4_MPEG_FBSIZE_HDMIN   0xBF4000  //4 * 1920 * 1088 * 1.5 = 11.96MB
#define MVD4_MPEG_FBSIZE_SDMIN   0x25F800  //4 * 720 * 576 * 1.5 = 2.38MB

#define _IS_VC1(x) ((x==E_MVD_CODEC_VC1_ADV) || (x==E_MVD_CODEC_VC1_MAIN))

#define SLQ_ENTRY_LEN               8   //8-byte per entry

#define MVD_U32_MAX                 0xffffffffUL
#define MVD_U64_MAX                 0xffffffffffffffffULL
#define MVD_GENERAL_MAX                 ((size_t)(-1))

#define MAX_ADD_28BIT               0x0fffffffUL

#define SLQ_ADDR_LEN                SLQ_TBL_ENTRY_LEN   //Slq address length is the same as SlqTbl

//Initial value for PTS table
#define _INIT_ADDR                  MVD_U32_MAX
#define _INIT_LEN                   0
#define _INIT_TIMESTAMP             MVD_U32_MAX
#define _INIT_ID                    MVD_U32_MAX

#define _MS_TO_90K(x)   (x*90)  //ms ==> 90k counter
#define _90K_TO_MS(x)   ((x!=MVD_U32_MAX)?(x/90):(x))  //90k counter ==> ms
#define _90K_TO_MS_U64(x)   ((x!=MVD_U64_MAX)?(x/90):(x))  //90k counter ==> ms

#define MVD_DBG_STS(x)        {}
#define _SLQTBL_DUMP_PTS      FALSE//TRUE
#define _SLQTBL_DUMP_PUSHQ    FALSE//TRUE
#define _SLQTBL_DUMP_PKT      FALSE//TRUE
#define MVD_TURBO_INIT        FALSE//TRUE

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
#define SLQ_PIC_START_FLAG 0x40000000
#define VC1_PATTERN     0x0D010000
#define RCV_PATTERN     0xFF00A55A
#endif
#define VC1_PATTERN_0   0xff010000
#define VC1_PATTERN_1   0x0000ffff
#define VC1_PATTERN_2   0xffffff01
#define VC1_PATTERN_3   0x0000ffee

#define RCV_PATTERN_0   0x00000000
#define RCV_PATTERN_1   0xffffffff
#define RCV_PATTERN_2   0x00000000
#define RCV_PATTERN_3   0xeeeeeeee

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
#define SKIP_DATA_TIMEOUT_MS 15

//Length of internal buffers
#define MVD_FW_IAP_BUF_LEN                 (0x4000UL)   // 16k
#define MVD_FW_DP_BUF_LEN                  (0x80000UL)  //512k
#define MVD_FW_MV_BUF_LEN                  (0x48000UL)  //288K
//Alignment of HW buffers start address
#define MVD_FW_IAP_BUF_ALIGN               (0x4000UL)   // 16k
#define MVD_FW_DP_BUF_ALIGN                (0x8000UL)   // 32k
#define MVD_FW_MV_BUF_ALIGN                (0x8000UL)   // 32k

//Three HW buffers are allocated after framebuffer, so we need to
//consider these buffers when checking the available framebuffer size/number.
#define MVD_HW_BUF_TOTAL_LEN               (MVD_FW_IAP_BUF_LEN + MVD_FW_DP_BUF_LEN + MVD_FW_MV_BUF_LEN) //u32HWBuffTotalSize

#define MVD_FW_CODE_LEN                    (OFFSET_BASE)//refer to the define in mvd4_interface.h
#define MVD_FW_CODE_LEN_V00                (0x70000UL)  //length for old layout
#define MVD_FW_MPOOL_START_OFFSET          (0x90000UL)  //576K
#define MVD_FW_TASK_OFFSET                 (0x100000UL) //1M

#define MVD3_FW_VOL_INFO_BUF_LEN            (0x1000UL)   //  4K
#define MVD3_FW_FRAME_INFO_BUF_LEN          (0x1000UL)   //  4K
#define MVD3_FW_DIVX_INFO_BUF_LEN           (0x1000UL)   //  4K
#define MVD3_FW_USER_DATA_BUF_LEN           (0x4000UL)   //  16K
#define MVD3_FW_USER_DATA_BUF_BACKUP_LEN    MVD3_FW_USER_DATA_BUF_LEN // 16K
#define MVD3_FW_SLQ_TAB_TMPBUF_LEN          (0x200UL)
#define MVD_FW_SLQTBL_PTS_BUF_LEN           (SLQ_ENTRY_MAX*MVD_FW_SLQTBL_PTS_LEN)
#define MVD_FW_DYN_SCALE_BUF_LEN            (0x1000UL)   //  4K
#define MVD_FW_SCALER_INFO_BUF_LEN          (0x100UL)    // 256bytes reserved
#define MVD_FW_DECFRM_INFO_BUF_LEN          (0x100UL)    // 256bytes reserved
#define MVD_FW_VBBU_TABLE_LEN               (0x2000UL)   // 8K

#define MVD_FW_USER_DATA_HDR_LEN            (6)
#define MVD_FW_USER_DATA_PKT_LEN            (256)
#define MVD_FW_USER_DATA_EXT_HDR_LEN        (16)
#define FW_BUFF_ALIGN                       (0x1000)     //4k align

#define SLQ_ENTRY_MAX               1024
#define MVD_FW_SLQTBL_PTS_LEN       32
#define MVD_NULLPKT_PTS             MVD_U32_MAX


//Get the start address, and the next start address.
#define GET_FW_BUFFADD(cur, size, start)                    \
        do {                                                \
            start = cur;                                    \
            cur += size;                                    \
        } while(0)

//Get the aligned start address, and the next start address.
#define GET_FW_BUFFADD_ALIGN(cur, align, size, alignStart)  \
        do {                                                \
            cur = (MemAlign(cur, align));                   \
            GET_FW_BUFFADD(cur, size, alignStart);          \
        } while(0)

//Init command arguments
#define SETUP_CMDARG(x)     \
            do {            \
                x.Arg0 = 0; \
                x.Arg1 = 0; \
                x.Arg2 = 0; \
                x.Arg3 = 0; \
                x.Arg4 = 0; \
                x.Arg5 = 0; \
               } while(0)

//Set command arguments
#define SET_CMDARG(cmd, u32val, u8Num)      \
            do {                            \
                cmd.Arg0 = L_WORD(u32val);  \
                cmd.Arg1 = H_WORD(u32val);  \
                cmd.Arg2 = L_DWORD(u32val); \
                cmd.Arg3 = H_DWORD(u32val); \
                cmd.Arg4 = 0;               \
                cmd.Arg5 = u8Num;           \
               } while(0)

#define SET_CMD_RET_FALSE(_cmdVal, _pStcmdArg)                                  \
    do {                                                                        \
        if (HAL_MVD_MVDCommand(_cmdVal, (_pStcmdArg)) == FALSE)                 \
        {                                                                       \
            MVD_DEBUGERROR(VPRINTF("Command: 0x%x fail!!\r\n", _cmdVal));        \
            return FALSE;                                                       \
        }                                                                       \
    } while(0)

#define SET_CMD_RET_VOID(_cmdVal, _pStcmdArg)                                  \
    do {                                                                        \
        if (HAL_MVD_MVDCommand(_cmdVal, (_pStcmdArg)) == FALSE)                 \
        {                                                                       \
            MVD_DEBUGERROR(VPRINTF("Command: 0x%x fail!!\r\n", _cmdVal));        \
            return;                                                       \
        }                                                                       \
    } while(0)

#define HAL_MVD_InvalidBuffRetFalse(x)                                                              \
    do {                                                                                            \
        if ((x)==0)                                                                                 \
        {                                                                                           \
            MVD_DEBUGERROR(VPRINTF("%s(%d) error: NULL buffer address.\n", __FUNCTION__, __LINE__)); \
            return FALSE;                                                                           \
        }                                                                                           \
    } while(0)

#define _MVD_Memset(pDstAddr, u32value, u32Size)                                \
    do {                                                                        \
        MS_U32 x = 0;                                                           \
        for (x = 0; x < (u32Size/4); x=x+4)                                     \
        {                                                                       \
            HAL_MVD_Memset4Byte(pDstAddr+x,u32value);                         \
        }                                                                       \
        HAL_MVD_CPU_Sync();                                 \
        HAL_MVD_ReadMemory();                                      \
    } while (0)

#define _MVD_MemResetBit(pDstAddr, mask_num)                      \
    do{                                                         \
        *pDstAddr = (*pDstAddr) & (~(0x1<<mask_num));   \
    }while(0)                                                   \


typedef enum
{
    MVD_HKSLQ_GET_READPTR = 0,
    MVD_HKSLQ_GET_WRITEPTR = 1,
    MVD_HKSLQ_NONE = 2,
} MVD_HKSLQ_CMD;

#if SLQ_NEW_PUSH
typedef struct _MVD_SLQ_STATUS
{
    MS_BOOL bSlqPicStart;
    MS_BOOL bSlqPicCollect;
    MS_BOOL bSlqPicWaitNextStart;
    MS_BOOL bSlqFireRdy;
    MS_BOOL bSlqCtrlBit;
    MS_BOOL bSlqEnLastFrameShow;
    MS_VIRT u32SlqPatternAddr;
    MS_U32 u32SlqPushLength;
    MS_VIRT u32VaildWptrAddr;
} MVD_SLQ_STATUS;
#endif

typedef struct _MVD_SLQ_TBL_ST
{
    MS_VIRT u32StAdd;
    MS_VIRT u32EndAdd;
    MS_U32 u32EntryCntMax;

    MS_VIRT u32RdPtr;
    MS_VIRT u32WrPtr;
    MS_VIRT u32Empty;
    MS_VIRT* pu32LastEntry;
} MVD_SLQ_TBL_ST;

typedef struct _MVD_ALIVEInfo
{
    MS_U32 u32decode_count;
    MS_U32 u32searchbuf_count;
    MS_U32 u32searchcode_count;
    MS_U32 u32prebuf_count;
    MS_U32 u32vfifobuf_count;
    MS_U32 u32searchheader_count;
    MS_U32 u32flashpattern_count;
} MVD_ALIVEInfo;


typedef struct _MVD_SLQ_ES_ST
{
    MS_VIRT u32StAdd;
    MS_VIRT u32EndAdd;

    MS_VIRT u32RdPtr;
    MS_VIRT u32WrPtr;
} MVD_SLQ_ES_ST;

typedef struct _MVD_FWBuff
{
    MS_VIRT pu8MVDGetVolBufStart;
    MS_VIRT u32VolAdd;
    MS_VIRT pu8MVDGetFrameInfoBufStart;
    MS_VIRT pu8MVDSetHeaderBufStart;
    MS_VIRT u32UserDataBuf;
    MS_VIRT u32MVDFWSLQTABTmpbufAdr;
    MS_VIRT u32MVDFWPtsTblAddr;
    MS_VIRT u32DynScalingAdd;
    MS_VIRT u32ScalerInfoAdd;
    MS_VIRT u32DecFrmInfoAdd;
    MS_VIRT u32VBBUTableAdd;
} MVD_FWBuff;

typedef enum _FW_BUFF_TYPE
{
    FW_BUFF_VOLINFO,
    FW_BUFF_FRMINFO,
    FW_BUFF_HDR,
    FW_BUFF_USRDATA,
    FW_BUFF_SLQTBL,
    FW_BUFF_FWSLQTAB,
    FW_BUFF_PTSTBL,
    FW_BUFF_DS,
    FW_BUFF_XCINFO,
    FW_BUFF_DECFRMINFO,
} FW_BUFF_TYPE;

typedef struct _MVD_SLQTBLInfo
{
    MS_U32 u32LastPts;      ///< record the last PTS to handle the repeat case
    MS_U32 u32DummyPktCnt;  ///< accumulated dummy packet counter
    MS_U32 u32SlqByteCnt;   ///< accumulated byte counter
    MS_VIRT u32ESBuffEnd;    ///< miu offset of ES buffer end, used for report TS read/write ptr.
    MS_VIRT u32FileEndPtr;   ///< write ptr of file-end
    MS_VIRT u32PreEsRd;      ///< previous ES read ptr
    MS_VIRT u32PreEsWr;      ///< previous ES write ptr
    MS_BOOL bRdyToFireCmd;  ///< TRUE after FW is ready to update write pointer
    MS_BOOL bSlqTblHasValidData; ///< TRUE if the write ptr is not updated
    MS_BOOL bEnSlqTblHkCtrl;
    BDMA_CpyType bdmaCpyType;
    MVD_SLQ_STATUS* pSlqStatus;
    MVD_SLQ_TBL_ST* pDrvSlqTbl;
    MVD_SLQ_ES_ST*  pDrvEsTbl;
    MVD_SLQ_ES_ST*  pDrvDivxTbl;
} MVD_SLQTBLInfo;

//Only keep one record of MIU setting since there should be only one,
//even for multiple decoders.
typedef struct _MVDMiuCfg
{
    MS_PHY u32Miu1BaseAddr;
    MS_PHY u32Miu2BaseAddr;
    MS_PHY u32Miu3BaseAddr;
    MS_U8    u8FWMiuSel;
    MS_U8    u8HWMiuSel;
} MVDMiuCfg;


typedef enum
{
    E_MVD_DEC_0 = 0,
    E_MVD_DEC_1,
    E_MVD_DEC_MAX
} MVD_DecNum;
#ifdef VDEC3
#define MAX_DEC_NUM 4
#else
#define MAX_DEC_NUM E_MVD_DEC_MAX
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
static MS_VIRT u32RiuBaseAdd = 0;
MS_U8 _u8HalDbgLevel = 0;
#ifdef VDEC3//#ifdef VDEC3
static MS_U32 u32SharememoryBase[MAX_DEC_NUM] ={MVD_U32_MAX,MVD_U32_MAX,MVD_U32_MAX,MVD_U32_MAX};
#else
static MS_U32 u32SharememoryBase[MAX_DEC_NUM] ={MVD_U32_MAX,MVD_U32_MAX};
#endif
static MS_U8 u8SHMMiuSel=0;
#if 0
static MVD_MEMCfg   stMemCfg[MAX_DEC_NUM];
static MVD_CtrlCfg  stCtrlCfg[MAX_DEC_NUM];
static MVD_FWCfg    stFwCfg[MAX_DEC_NUM];

// For SLQ table link
static MVD_SLQTBLInfo stSlqTblInfo[MAX_DEC_NUM];
static MVD_SLQ_STATUS _SlqStatus[MAX_DEC_NUM];  //for SLQ_NEW_PUSH
static MVD_SLQ_TBL_ST _drvSlqTbl[MAX_DEC_NUM];
static MVD_SLQ_ES_ST _drvEsTbl[MAX_DEC_NUM];    //maintain to report ES read/write ptr
static MVD_SLQ_ES_ST _drvDivxTbl[MAX_DEC_NUM];  //header for divx311
static FW_DIVX_INFO gdivxInfo[MAX_DEC_NUM];

MS_BOOL bSlqTblHKCtrl = FALSE;   //read/write pointer can be accessed by HK directly
static MS_BOOL bStopped = TRUE;

static HAL_MVD_Stream _stMVDStream[MAX_DEC_NUM] = {
    {E_HAL_MVD_MAIN_STREAM0, FALSE},
    {E_HAL_MVD_SUB_STREAM0, FALSE},
    //{E_HAL_MVD_SUB_STREAM1, FALSE},
};
static MVD_FWBuff stFWBuff[MAX_DEC_NUM];
#endif

#define FRM_RATE_CODE_NUM 9

typedef struct
{
    MVD_MEMCfg    stMemCfg[MAX_DEC_NUM];
    MVD_CtrlCfg    stCtrlCfg[MAX_DEC_NUM];
    MVD_FWCfg    stFwCfg[MAX_DEC_NUM];

    // For SLQ table link
    MVD_SLQTBLInfo stSlqTblInfo[MAX_DEC_NUM];
    MVD_SLQ_STATUS _SlqStatus[MAX_DEC_NUM];    //for SLQ_NEW_PUSH
    MVD_SLQ_TBL_ST _drvSlqTbl[MAX_DEC_NUM];
    MVD_SLQ_ES_ST _drvEsTbl[MAX_DEC_NUM];    //maintain to report ES read/write ptr
    MVD_SLQ_ES_ST _drvDivxTbl[MAX_DEC_NUM];    //header for divx311
    FW_DIVX_INFO gdivxInfo[MAX_DEC_NUM];

    MS_BOOL bSlqTblHKCtrl[MAX_DEC_NUM];    //read/write pointer can be accessed by HK directly
    MS_BOOL bStopped[MAX_DEC_NUM];

    HAL_MVD_Stream _stMVDStream[MAX_DEC_NUM];

    MVD_FWBuff stFWBuff[MAX_DEC_NUM];

    #if _SLQTBL_DUMP_PKT
    MS_U32 u32SendTimes[MAX_DEC_NUM];//HAL_MVD_SLQTblSendPacket
    #endif

    MS_U32 u32pqTimes[MAX_DEC_NUM];//HAL_MVD_PushQueue
    MS_BOOL bSetSkip[MAX_DEC_NUM];//MVD_FlushTSQueue

    MS_U32 u32PreVdCnt[MAX_DEC_NUM];//HAL_MVD_DbgDump
    MS_U32 u32PreErrCnt[MAX_DEC_NUM];
    MS_BOOL b1stDump[MAX_DEC_NUM];
    //pre_set
    HAL_MVD_Pre_Ctrl gMVDPreCtrl[MAX_DEC_NUM];
    MS_BOOL bAutoInsertDummyPattern[MAX_DEC_NUM];
    MVDMiuCfg stMiuCfg;
    MS_BOOL bDropOnePTS[MAX_DEC_NUM];
    MVD_ALIVEInfo  aliveInfo[MAX_DEC_NUM];

    MS_U32 u32DmxFrameRate[MAX_DEC_NUM];
    MS_U32 u32DmxFrameRateBase[MAX_DEC_NUM];
    MS_U32 u32LastAliveTime[MAX_DEC_NUM];
    MS_BOOL bCMAUsed;

    struct CMA_Pool_Init_Param cmaInitParam; // support two MIU
    struct CMA_Pool_Free_Param cmaFreeParam[MAX_DEC_NUM];
    MS_BOOL bCMATwoMIU[MAX_DEC_NUM];
} MVD_Hal_CTX;

//global variables
MVD_Hal_CTX* pMVDHalContext = NULL;
MVD_Hal_CTX gMVDHalContext;
const MS_U16 stFrameRateCode[FRM_RATE_CODE_NUM]=
{
    NULL,23976,24000,25000,29976,30000,50000,59947,60000
};


#define GET_VOL_BUFFADD(idx)           pMVDHalContext->stFWBuff[(idx)].pu8MVDGetVolBufStart
#define GET_VOL_BUFFADD_NONCACHE(idx)  pMVDHalContext->stFWBuff[(idx)].u32VolAdd
#define GET_FRMINFO_BUFFADD(idx)       pMVDHalContext->stFWBuff[(idx)].pu8MVDGetFrameInfoBufStart
#define GET_HDR_BUFFADD(idx)           pMVDHalContext->stFWBuff[(idx)].pu8MVDSetHeaderBufStart
#define GET_USRDATA_BUFFADD(idx)       pMVDHalContext->stFWBuff[(idx)].u32UserDataBuf
#define GET_SLQ_BUFFADD(idx)           pMVDHalContext->stFWBuff[(idx)].u32MVDFWSLQTABTmpbufAdr
#define GET_PTSTBL_BUFFADD(idx)        pMVDHalContext->stFWBuff[(idx)].u32MVDFWPtsTblAddr
#define GET_DS_BUFFADD(idx)            pMVDHalContext->stFWBuff[(idx)].u32DynScalingAdd
#define GET_XCINFO_BUFFADD(idx)        pMVDHalContext->stFWBuff[(idx)].u32ScalerInfoAdd
#define GET_DECFRMINFO_BUFFADD(idx)    pMVDHalContext->stFWBuff[(idx)].u32DecFrmInfoAdd
#define GET_VBBUTABLEADD(idx)          pMVDHalContext->stFWBuff[(idx)].u32VBBUTableAdd



//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void HAL_MVD_Context_Init(void)
{
#ifdef VDEC3
    MS_U8 i;

    for (i = 0; i < MAX_DEC_NUM; i++)
    {
        pMVDHalContext->bStopped[i] = TRUE;
        pMVDHalContext->b1stDump[i] = TRUE;
        pMVDHalContext->_stMVDStream[i].eStreamId = E_HAL_MVD_N_STREAM0 + i;
    }
#else
    pMVDHalContext->bStopped[0] = TRUE;
    pMVDHalContext->bStopped[1] = TRUE;
    pMVDHalContext->_stMVDStream[0].eStreamId = E_HAL_MVD_MAIN_STREAM0;
    pMVDHalContext->_stMVDStream[1].eStreamId = E_HAL_MVD_SUB_STREAM0;

    pMVDHalContext->b1stDump[0] = TRUE;
    pMVDHalContext->b1stDump[1] = TRUE;
#endif
}



static void HAL_MVD_SetIsUsed(MS_U8 u8Idx, MS_BOOL bUsed)
{
    pMVDHalContext->_stMVDStream[u8Idx].bUsed = bUsed;
}

//Given the start address & the available size for the FW buffers,
//return the address of each buffers.
//Now only support u8Size==1, 2, 4
static MS_U32 MVD_GetFWBuffData(MS_U8 u8Idx, FW_BUFF_TYPE eBuffType, MS_U8 u8Offset, MS_U8 u8Size)
{
    MS_U32 u32Val = 0;
    MS_VIRT u32BufStart = NULL;
    switch (eBuffType)
    {
        case FW_BUFF_VOLINFO:    u32BufStart = GET_VOL_BUFFADD(u8Idx);        break;
        case FW_BUFF_FRMINFO:    u32BufStart = GET_FRMINFO_BUFFADD(u8Idx);    break;
        case FW_BUFF_HDR:        u32BufStart = GET_HDR_BUFFADD(u8Idx);        break;
        case FW_BUFF_USRDATA:    u32BufStart = GET_USRDATA_BUFFADD(u8Idx);    break;
        case FW_BUFF_FWSLQTAB:   u32BufStart = GET_SLQ_BUFFADD(u8Idx);        break;
        case FW_BUFF_PTSTBL:     u32BufStart = GET_PTSTBL_BUFFADD(u8Idx);     break;
        case FW_BUFF_DS:         u32BufStart = GET_DS_BUFFADD(u8Idx);         break;
        case FW_BUFF_XCINFO:     u32BufStart = GET_XCINFO_BUFFADD(u8Idx);     break;
        case FW_BUFF_DECFRMINFO: u32BufStart = GET_DECFRMINFO_BUFFADD(u8Idx); break;
        default:
            break;
    }
    if (NULL == u32BufStart)
    {
        MVD_DEBUGERROR(VPRINTF("%s err: u8Idx=0x%x, bufType=0x%x, offset=%d, size=%d\n",
               __FUNCTION__, u8Idx, eBuffType, u8Offset, u8Size));
        return 0;
    }

    if (u8Size == sizeof(MS_U8))
    {
        u32Val = (MS_U32)HAL_MVD_MemReadByte(u32BufStart+u8Offset);
    }
    else if (u8Size == sizeof(MS_U32))
    {
        u32Val = HAL_MVD_MemRead4Byte(u32BufStart+u8Offset);
    }
    else if (u8Size == sizeof(MS_U16))
    {
        u32Val = HAL_MVD_MemRead2Byte(u32BufStart+u8Offset);
    }

    MVD_DEBUGVERBAL(VPRINTF("%s: u32Val=%d for u8Idx=0x%x, bufType=0x%x, offset=%d, size=%d\n",
                          __FUNCTION__, u32Val, u8Idx, eBuffType, u8Offset, u8Size));
    return u32Val;
}

#ifdef VDEC3
static void MVD_SetFWBuffData(MS_U8 u8Idx, FW_BUFF_TYPE eBuffType, MS_U8 u8Offset, MS_U8 u8Size, MS_U32 u32Value)
{
    MS_VIRT u32BufStart = NULL;

    switch (eBuffType)
    {
        case FW_BUFF_VOLINFO:    u32BufStart = GET_VOL_BUFFADD(u8Idx);        break;
        case FW_BUFF_FRMINFO:    u32BufStart = GET_FRMINFO_BUFFADD(u8Idx);    break;
        case FW_BUFF_HDR:        u32BufStart = GET_HDR_BUFFADD(u8Idx);        break;
        case FW_BUFF_USRDATA:    u32BufStart = GET_USRDATA_BUFFADD(u8Idx);    break;
        case FW_BUFF_FWSLQTAB:   u32BufStart = GET_SLQ_BUFFADD(u8Idx);        break;
        case FW_BUFF_PTSTBL:     u32BufStart = GET_PTSTBL_BUFFADD(u8Idx);     break;
        case FW_BUFF_DS:         u32BufStart = GET_DS_BUFFADD(u8Idx);         break;
        case FW_BUFF_XCINFO:     u32BufStart = GET_XCINFO_BUFFADD(u8Idx);     break;
        case FW_BUFF_DECFRMINFO: u32BufStart = GET_DECFRMINFO_BUFFADD(u8Idx); break;
        default:
            break;
    }

    if (NULL == u32BufStart)
    {
        MVD_DEBUGERROR(VPRINTF("%s err: u8Idx=0x%x, bufType=0x%x, offset=%d, size=%d, value=%d\n",
               __FUNCTION__, (unsigned int)u8Idx,(unsigned int) eBuffType,(unsigned int) u8Offset,(unsigned int) u8Size,(unsigned int)u32Value));
    }

    if (pMVDHalContext->stMiuCfg.u8HWMiuSel == MIU_SEL_1)
    {
        u32BufStart += HAL_MIU1_BASE;
    }
/*    else if (pMVDHalContext->stMiuCfg.u8HWMiuSel == MIU_SEL_2)
    {
        u32BufStart += HAL_MIU2_BASE;
    }*/

    if (u8Size == sizeof(MS_U8))
    {
        MS_U8* temp = (MS_U8*)MsOS_PA2KSEG1(u32BufStart+u8Offset);
        *temp = (MS_U8)u32Value;
    }
    else if (u8Size == sizeof(MS_U32))
    {
        MS_U32* temp = (MS_U32*)MsOS_PA2KSEG1(u32BufStart+u8Offset);
        *temp = (MS_U32)u32Value;
    }
    else if (u8Size == sizeof(MS_U16))
    {
        MS_U16* temp = (MS_U16*)MsOS_PA2KSEG1(u32BufStart+u8Offset);
        *temp = (MS_U16)u32Value;
    }

    MVD_DEBUGVERBAL(VPRINTF("%s: u8Idx=0x%x, bufType=0x%x, offset=%d, size=%d, value=%d\n",
                          __FUNCTION__,  (unsigned int)u8Idx, (unsigned int)eBuffType, (unsigned int)u8Offset, (unsigned int)u8Size,(unsigned int)u32Value));
}
#endif


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void HAL_MVD_SetDbgLevel(MS_U8 level)
{
    _u8HalDbgLevel = level;
    return;
}


MS_U8 HAL_MVD_GetCaps(void)
{
    MS_U8 caps = 0;

    caps |= (MVD_SUPPORT_MPEG2|MVD_SUPPORT_MPEG4|MVD_SUPPORT_VC1);
    return caps;
}

MS_U32 HAL_MVD_GetDrvFwVer(void)
{
    return MVD_FW_VERSION;
}

#if 0
MS_U32 HAL_MVD_GetMiu1BaseAdd(void)
{
    return MIU1_BASEADDR;
}
#endif


MS_VIRT HAL_MVD_GetMiu1BaseAdd(void)
{
    return MIU1_BASEADDR;
}

MS_BOOL HAL_MVD_GetFWSelMiu1(void)
{
    return MVDCPU_ON_MIU1;
}



MS_U8 HAL_MVD_GetFWSelMiu(void)
{
    return pMVDHalContext->stMiuCfg.u8FWMiuSel;
}



void HAL_MVD_MemGetMap(MS_U8 u8Idx, MS_U8 u8type, MS_VIRT* pu32addr, MS_SIZE* pu32len)
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MS_VIRT u32StartOffset;
    MS_U8  u8MiuSel;

    switch (u8type)
    {
        case E_MVD_MMAP_ALL:
        case E_MVD_MMAP_FW:
            _phy_to_miu_offset(u8MiuSel, u32StartOffset, pstMemCfg->u32FWCodeAddr);
            *pu32addr = u32StartOffset;
            *pu32len = pstMemCfg->u32FWCodeSize;
            break;
        case E_MVD_MMAP_BS:
            _phy_to_miu_offset(u8MiuSel, u32StartOffset, pstMemCfg->u32BSAddr);
            *pu32addr = u32StartOffset;
            *pu32len = pstMemCfg->u32BSSize;
            break;
        case E_MVD_MMAP_FB:
            _phy_to_miu_offset(u8MiuSel, u32StartOffset, pstMemCfg->u32FBAddr);
            *pu32addr = u32StartOffset;
            *pu32len = pstMemCfg->u32FBSize;
            break;
        case E_MVD_MMAP_DRV:
            _phy_to_miu_offset(u8MiuSel, u32StartOffset, pstMemCfg->u32DrvBufAddr);
            *pu32addr = u32StartOffset;
            *pu32len = pstMemCfg->u32DrvBufSize;
            break;
        default:
            break;
    }

    MVD_DEBUGINFO(VPRINTF("HAL_MVD_MemGetMap[%d] add=0x%lx len=0x%lx\n",u8type,(unsigned long)*pu32addr,(unsigned long)*pu32len));
    return;
}

void HAL_MVD_RegSetBase(MS_VIRT u32Base)
{
    u32RiuBaseAdd = u32Base;
    HAL_VPU_EX_InitRegBase(u32Base);
}

void HAL_MVD_RegWriteByte(MS_VIRT u32Reg, MS_U8 u8Val)
{
    if ( __builtin_constant_p( u32Reg ) )
    {
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;
    }
    else
    {
        RIU8[(u32Reg << 1) - (u32Reg & 1)] = u8Val;
    }
}

MS_U8 HAL_MVD_RegReadByte(MS_VIRT u32Reg)
{
    return (__builtin_constant_p( u32Reg ) ?
            (((u32Reg) & 0x01) ? RIU8[(u32Reg) * 2 - 1] : RIU8[(u32Reg) * 2]) :
            (RIU8[(u32Reg << 1) - (u32Reg & 1)]));
}

void HAL_MVD_RegWriteBit(MS_VIRT u32Reg, MS_BOOL bEnable, MS_U8 u8Mask)
{
    MS_VIRT u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);
    RIU8[u32Reg8] = (bEnable) ? (RIU8[u32Reg8] |  (u8Mask)) :
                                (RIU8[u32Reg8] & ~(u8Mask));
}

void HAL_MVD_RegWriteByteMask(MS_VIRT u32Reg, MS_U8 u8Val, MS_U8 u8Msk)
{
    MS_VIRT u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);
    RIU8[u32Reg8] = (RIU8[u32Reg8] & ~(u8Msk)) | ((u8Val) & (u8Msk));
}

void HAL_MVD_RegWrite4Byte(MS_VIRT u32Reg, MS_U32 u32Val)
{
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) )
    {
        RIU[u32Reg] = (MS_U16)(u32Val);
        RIU[(u32Reg) + 2] = (MS_U16)((u32Val) >> 16);
    }
    else
    {
        if (u32Reg & 0x01)
        {
            RIU8[(u32Reg << 1) - 1] = u32Val;
            RIU[u32Reg + 1] = (u32Val >> 8);
            RIU8[((u32Reg + 3) << 1)] = (u32Val >> 24);
        }
        else
        {
            RIU[u32Reg] = u32Val;
            RIU[u32Reg + 2] = (u32Val >> 16);
        }
    }
}


MS_U32 HAL_MVD_MemRead4Byte(MS_VIRT u32Address)
{
    volatile MS_U32 u32Val;
    MS_ASSERT(!(u32Address & 0x03UL));

    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32Address,u32Address);

    u32Val = *(volatile MS_U32*) HAL_MVD_PA2NonCacheSeg(u32Address);

    //printf("mvd rd 0x%lx = 0x%lx\n", u32Address, u32Val);
    return u32Val;
}

MS_U16 HAL_MVD_MemRead2Byte(MS_VIRT u32Address)
{
    MS_VIRT u32ReadAddr;
    MS_U32 u32ReadValue;
    MS_U16 u16Value;
    MS_U8 u8Shift;
    u32ReadAddr = (u32Address >> 2) << 2;
    u8Shift = (MS_U8)((u32Address & 0x03) * 8);
    u32ReadValue = HAL_MVD_MemRead4Byte(u32ReadAddr);

    u16Value = (MS_U16)(u32ReadValue >> u8Shift);
    if(u8Shift == 24)
    {
        u32ReadValue = HAL_MVD_MemRead4Byte(u32ReadAddr+4);
        u16Value = u16Value << 8 || (MS_U16)(u32ReadValue & 0xFF);
    }
    return u16Value;
}

MS_U8 HAL_MVD_MemReadByte(MS_VIRT u32Address)
{
    MS_VIRT u32ReadAddr;
    MS_U32 u32ReadValue;
    MS_U8 u8Value;
    MS_U8 u8Shift;
    u32ReadAddr = (u32Address >> 2) << 2;
    u8Shift = (MS_U8)((u32Address & 0x03) * 8);
    u32ReadValue = HAL_MVD_MemRead4Byte(u32ReadAddr);
    u8Value = (MS_U8)(u32ReadValue >> u8Shift);

    return u8Value;
}

MS_BOOL HAL_MVD_MemWrite4Byte(MS_VIRT u32Address, MS_U32 u32Value)
{

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32Address,u32Address);

    *(volatile MS_U32 *) HAL_MVD_PA2NonCacheSeg(u32Address) = u32Value;

    HAL_MVD_CPU_Sync();
    HAL_MVD_FlushMemory();

    return TRUE;
}

MS_BOOL HAL_MVD_MemWriteByte(MS_VIRT u32Address, MS_U8 u8Value)
{
    MS_VIRT u32ReadAddr;
    MS_U32 u32ReadValue;
    MS_U8 u8Shift;

    u32ReadAddr = (u32Address >> 2) << 2;
    u8Shift = (MS_U8)((u32Address & 0x03UL) * 8);
    u32ReadValue = HAL_MVD_MemRead4Byte(u32ReadAddr);
    u32ReadValue &= ~(0xFFUL << u8Shift);
    u32ReadValue |= ((MS_U32)u8Value << u8Shift);
    HAL_MVD_MemWrite4Byte(u32ReadAddr, u32ReadValue);
    return TRUE;
}

MS_BOOL HAL_MVD_MemWrite2Byte(MS_VIRT u32Address, MS_U16 u16Value)
{
    MS_VIRT u32ReadAddr;
    MS_U32 u32ReadValue;
    MS_U8 u8Shift;

    u8Shift = (MS_U8)((u32Address & 0x03UL) * 8) ;
    if(u8Shift < 24)
    {
        u32ReadAddr = (u32Address >> 2) << 2;
        u32ReadValue = HAL_MVD_MemRead4Byte(u32ReadAddr);
        u32ReadValue &= ~(0xFFFF << u8Shift);
        u32ReadValue |= ((MS_U32)u16Value << u8Shift);
        HAL_MVD_MemWrite4Byte(u32ReadAddr, u32ReadValue);
    }
    else
    {
        HAL_MVD_MemWriteByte(u32Address, (MS_U8)(u16Value));
        HAL_MVD_MemWriteByte(u32Address+1, (MS_U8)(u16Value >> 8));
    }
    return TRUE;
}


void HAL_MVD_SetReqMask(MS_BOOL bEnMask)
{
    HAL_VPU_EX_MIU_RW_Protect(bEnMask);

    if (MVDHW_ON_MIU1)
    {
        HAL_MVD_RegWriteBit(MIU1_RQ2_MASK_L, bEnMask, BIT4);  //MVD R/W
        HAL_MVD_RegWriteBit(MIU1_RQ0_MASK_H, bEnMask, BIT4);  //MVD bbu R/W
    }
    else
    {
        HAL_MVD_RegWriteBit(MIU0_RQ2_MASK_L, bEnMask, BIT4);  //MVD R/W
        HAL_MVD_RegWriteBit(MIU0_RQ0_MASK_H, bEnMask, BIT4);  //MVD bbu R/W
    }
    HAL_MVD_Delayms(1);

    return;
}

static MS_BOOL HAL_MVD_Memset4Byte(MS_VIRT u32Address, MS_U32 u32Value)
{
    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32Address,u32Address);

    *(volatile MS_U32 *) HAL_MVD_PA2NonCacheSeg(u32Address) = u32Value;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Initialize MVD
/// @return -result of resetting MVD hardware
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_RstHW(void)
{
    MS_U32 u32Time = 0;

    OSAL_MVD_LockHwMutex();

    HAL_MVD_SetReqMask(ENABLE);

    HAL_MVD_RegWriteBit(MVD_CTRL, 1, MVD_CTRL_DISCONNECT_MIU);//disconnect MIU
    HAL_MVD_RegWriteBit(MVD_CTRL, 0, MVD_CTRL_DISCONNECT_MIU);//release reset

    HAL_MVD_RegWriteBit(MVD_CTRL, 1, MVD_CTRL_RST);//reset MVD
    HAL_MVD_RegWriteBit(MVD_CTRL, 0, MVD_CTRL_RST);//release reset

    u32Time = HAL_MVD_GetTime();
    while ( ((HAL_MVD_RegReadByte(MVD_STATUS) & MVD_STATUS_READY) == 0)
            && ((HAL_MVD_GetTime() - u32Time) < 200) );

#if 0 //bring up
    printf("====================>>>>MVD Ctrl status1.5a : 0x%x\n",HAL_MVD_RegReadByte(MVD_STATUS));
    HAL_MVD_RegWriteBit(MVD_STATUS, 1, MVD_T8_MIU_128_0);//release reset
    HAL_MVD_RegWriteBit(MVD_STATUS, 1, MVD_T8_MIU_128_1);//release reset
    MVD_DEBUGINFO(printf("MVD Ctrl status : 0x%x\n",HAL_MVD_RegReadByte(MVD_STATUS)));
    printf("====================>>>>MVD Ctrl status1.5b : 0x%x\n",HAL_MVD_RegReadByte(MVD_STATUS));
#endif

    HAL_MVD_SetReqMask(DISABLE);

    OSAL_MVD_UnlockHwMutex();

    return TRUE;
}


//------------------------------------------------------------------------------
/// Release CPU
/// @return -release CPU successfully or not
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_ReleaseFW(void)
{
    //For dual decoder, we only release VPU if it is not released yet.
    if (TRUE == HAL_VPU_EX_IsRsted())
    {
        MVD_DEBUGINFO(VPRINTF("%s VPU_IsRsted\n", __FUNCTION__));
        return TRUE;
    }

    HAL_VPU_EX_SwRstRelse();
    return TRUE;
}

MS_VIRT HAL_MVD_PA2NonCacheSeg(MS_PHY u32PhyAddr)
{
    return MS_PA2KSEG1(u32PhyAddr);
}

MS_U32 HAL_MVD_GetTime(void)
{
    return MsOS_GetSystemTime();
}

void HAL_MVD_Delayms(MS_U32 u32msecs)
{
    MsOS_DelayTask(u32msecs);
}

void HAL_MVD_CPU_Sync(void)
{
    MAsm_CPU_Sync();
}

void HAL_MVD_FlushMemory(void)
{
    MsOS_FlushMemory();
}

void HAL_MVD_ReadMemory(void)
{
    MsOS_ReadMemory();
}

//Record the memory layout from system configuration
MS_BOOL HAL_MVD_SetMEMCfg(MS_U8 u8Idx, MVD_MEMCfg* pMEMCfg)
{
    memcpy(&(pMVDHalContext->stMemCfg[u8Idx]), pMEMCfg, sizeof(MVD_MEMCfg));
    //record the MIU settings
    pMVDHalContext->stMiuCfg.u32Miu1BaseAddr = pMEMCfg->u32Miu1BaseAddr;
    pMVDHalContext->stMiuCfg.u32Miu2BaseAddr = pMEMCfg->u32Miu2BaseAddr;
    pMVDHalContext->stMiuCfg.u32Miu3BaseAddr = pMEMCfg->u32Miu3BaseAddr;
    pMVDHalContext->stMiuCfg.u8FWMiuSel = pMEMCfg->u8FWMiuSel;
    pMVDHalContext->stMiuCfg.u8HWMiuSel = pMEMCfg->u8HWMiuSel;
    return TRUE;
}

MVD_MEMCfg* HAL_MVD_GetMEMCfg(MS_U8 u8Idx)
{
    //printf("%s: u8Idx=0x%x\n", __FUNCTION__, u8Idx);
    MVD_MEMCfg* pInfo = NULL;
    pInfo = &(pMVDHalContext->stMemCfg[u8Idx]);
    return pInfo;
}

MS_BOOL HAL_MVD_SetFWCfg(MS_U8 u8Idx, MVD_FWCfg* pFWCfg)
{
    //printf("%s: u8Idx=0x%x\n", __FUNCTION__, u8Idx);
    memcpy(&(pMVDHalContext->stFwCfg[u8Idx]), pFWCfg, sizeof(MVD_FWCfg));
    memcpy(&(pMVDHalContext->stFwCfg[u8Idx].stFBReduction), &pFWCfg->stFBReduction,sizeof(MVD_FB_Reduction));
    return TRUE;
}

MVD_FWCfg* HAL_MVD_GetFWCfg(MS_U8 u8Idx)
{
    MVD_FWCfg* pInfo = NULL;
    //printf("%s: u8Idx=0x%x\n", __FUNCTION__, u8Idx);
    pInfo = &(pMVDHalContext->stFwCfg[u8Idx]);
    return pInfo;
}

MS_U8 HAL_MVD_GetFBMode(MS_U8 u8Idx)
{
    MS_U8 u8Mode = MVD3_SD_MODE;
    u8Mode = pMVDHalContext->stFwCfg[u8Idx].u8FBMode;
    MVD_DEBUGINFO(VPRINTF("FBMode=0x%x\n", u8Mode));
    return u8Mode;
}

MVD_CodecType HAL_MVD_GetCodecType(MS_U8 u8Idx)
{
    MVD_CodecType eCodecType = E_MVD_CODEC_UNKNOWN;
    eCodecType = pMVDHalContext->stFwCfg[u8Idx].eCodecType;
    return eCodecType;
}

MVD_SrcMode HAL_MVD_GetSrcMode(MS_U8 u8Idx)
{
    MVD_SrcMode eSrcMode = E_MVD_SRC_UNKNOWN;
    eSrcMode = pMVDHalContext->stFwCfg[u8Idx].eSrcMode;
    return eSrcMode;
}

MS_BOOL HAL_MVD_SetSrcMode(MS_U8 u8Idx,MVD_SrcMode mode)
{
    if(pMVDHalContext != NULL)
    {
        pMVDHalContext->stFwCfg[u8Idx].eSrcMode = mode;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


MVD_CtrlCfg* HAL_MVD_GetCtrlCfg(MS_U8 u8Idx)
{
    MVD_CtrlCfg* pInfo = NULL;
    pInfo = &(pMVDHalContext->stCtrlCfg[u8Idx]);
    return pInfo;
}

MVD_SLQTBLInfo* HAL_MVD_GetSlqTblInfo(MS_U8 u8Idx)
{
    MVD_SLQTBLInfo* pInfo = NULL;
    pInfo = &(pMVDHalContext->stSlqTblInfo[u8Idx]);
    return pInfo;
}

void HAL_MVD_PowerCtrl(MS_BOOL bOn)
{
    HAL_MVD_RegWriteByteMask(REG_CKG_MVD, CKG_MVD_160MHZ, CKG_MVD_MASK);
    HAL_MVD_RegWriteBit(REG_CKG_MVD, !bOn, CKG_MVD_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD, !bOn, CKG_MVD_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_CHROMA_A, !bOn, CKG_MVD_CHROMA_A_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_CHROMA_A, !bOn, CKG_MVD_CHROMA_A_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_CHROMA_B, !bOn, CKG_MVD_CHROMA_B_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_CHROMA_B, !bOn, CKG_MVD_CHROMA_B_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_CHROMA_C, !bOn, CKG_MVD_CHROMA_C_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_CHROMA_C, !bOn, CKG_MVD_CHROMA_C_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_LUMA_A, !bOn, CKG_MVD_LUMA_A_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_LUMA_A, !bOn, CKG_MVD_LUMA_A_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_LUMA_B, !bOn, CKG_MVD_LUMA_B_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_LUMA_B, !bOn, CKG_MVD_LUMA_B_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_LUMA_C, !bOn, CKG_MVD_LUMA_C_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_LUMA_C, !bOn, CKG_MVD_LUMA_C_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_RMEM, !bOn, CKG_MVD_RMEM_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_RMEM, !bOn, CKG_MVD_RMEM_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_RMEM1, !bOn, CKG_MVD_RMEM1_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_RMEM1, !bOn, CKG_MVD_RMEM1_INVERT);

    HAL_MVD_RegWriteBit(REG_CKG_MVD_RREFDAT, !bOn, CKG_MVD_RREFDAT_GATED);
    HAL_MVD_RegWriteBit(REG_CKG_MVD_RREFDAT, !bOn, CKG_MVD_RREFDAT_INVERT);

    return;
}

#if 0
void HAL_MVD_Sleep(MS_U32 u32us)
{
    MsOS_DelayTaskUs(u32us);
}
#endif
void HAL_MVD_ResetHandShake(MS_U8 u8Idx, MVD_HANDSHAKE_CMD cmd)
{
    MS_U32 u32BufAddr = GET_FRMINFO_BUFFADD(u8Idx);
    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();
    MVD_CMD_HANDSHADE_INDEX u32CmdState;
    u32CmdState.value= HAL_MVD_MemRead4Byte(u32BufAddr + OFFSET_CMD_HANDSHAKE_INDEX);
    switch (cmd)
    {
        case MVD_HANDSHAKE_PAUSE:
            (u32CmdState.mvdcmd_handshake_pause) = 0;
            break;
        case MVD_HANDSHAKE_SLQ_RST:
            (u32CmdState.mvdcmd_handshake_slq_reset) = 0;
            break;
        case MVD_HANDSHAKE_STOP:
            (u32CmdState.mvdcmd_handshake_stop) = 0;
            break;
        case MVD_HANDSHAKE_SKIP_DATA:
            (u32CmdState.mvdcmd_handshake_skip_data) = 0;
            break;
        case MVD_HANDSHAKE_SINGLE_STEP:
            (u32CmdState.mvdcmd_handshake_single_step) = 0;
            break;
        case MVD_HANDSHAKE_SCALER_INFO:
            (u32CmdState.mvdcmd_handshake_scaler_data_ready) = 0;
            break;
        case MVD_HANDSHAKE_GET_NXTDISPFRM:
            (u32CmdState.mvdcmd_handshake_get_nextdispfrm_ready) = 0;
            break;
        case MVD_HANDSHAKE_PARSER_RST:
            (u32CmdState.mvdcmd_handshake_parser_rst) = 0;
            break;
        case MVD_HANDSHAKE_RST_CC608:
            (u32CmdState.mvdcmd_handshake_cc608_rst) = 0;
            break;
        case MVD_HANDSHAKE_RST_CC708:
            (u32CmdState.mvdcmd_handshake_cc708_rst) = 0;
            break;
        case MVD_HANDSHAKE_VIRTUAL_COMMAND:
            (u32CmdState.mvdcmd_handshake_virtualCommand) = 0;
            break;
        case MVD_HANDSHAKE_FLUSHQUEUE_COMMAND:
            (u32CmdState.mvdcmd_handshake_flush) = 0;
            break;
        default:
            break;
    }
    HAL_MVD_MemWrite4Byte(u32BufAddr + OFFSET_CMD_HANDSHAKE_INDEX,u32CmdState.value);
    MsOS_FlushMemory();
}

//------------------------------------------------------------------------------
/// Wait MVD command ready or timeout
/// @return -MVD command ready or timeout
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_TimeOut(MS_U8 u8Idx)
{
    MS_U32 i;
    MS_U32 u32StartTime = MsOS_GetSystemTime();

    for ( i = 0; i < MVD_PollingTimes; i++ )
    {
        ///- wait until MVD command ready or timeout
        if ( ( HAL_MVD_RegReadByte(MVD_STATUS) & MVD_STATUS_READY ) == MVD_STATUS_READY )
        {
            return FALSE;
        }

        if (/*(TRUE == pMVDHalContext->bStopped[u8Idx]) ||*/ ((MsOS_GetSystemTime()-u32StartTime)>1300))
        {
            MVD_DEBUGINFO(VPRINTF("%s: bStopped(%x) or timeout(%d)\n", __FUNCTION__, pMVDHalContext->bStopped[u8Idx], MsOS_GetSystemTime()-u32StartTime));
            return TRUE;
        }

        //HAL_MVD_Sleep(5);
    }
    MVD_DEBUGERROR( VPRINTF("MVD_TimeOut=%x\n", i) );
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set MVD firmware command
/// @param -u8cmd \b IN : MVD command
/// @param -pstCmdArg \b IN : pointer to command argument
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_MVDCommand ( MS_U8 u8cmd, MVD_CmdArg *pstCmdArg )
{
    MS_BOOL bRet = TRUE;
#ifdef VDEC3
    MS_U32 u32CmdArg;
    MS_U32 u32Ret = FALSE;
    MS_U32 u32Timeout;
    MS_U8  u8HalIdx = pstCmdArg->Arg5;
    MS_U32 u32Id = MVD_GetStreamId(u8HalIdx);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8HalIdx);
    CMD_QUEUE *pShm = (CMD_QUEUE *)MsOS_PA2KSEG1(pstMemCfg->u32FWCodeAddr + VCOMMANDQ_INFO + u8VPUIdx*0x100000);
    MS_VIRT u32BufStart2 = NULL;
    MS_BOOL bResponse = FALSE;
#endif
    OSAL_MVD_LockHwMutex();

#ifdef VDEC3
    if(HAL_MVD_IsDisplayCommand(u8cmd) == TRUE)
    {
        if(HAL_MVD_IsNeedResponseCommand(u8cmd) == TRUE)
        {
            // clear handshake dram
            u32BufStart2 = GET_VOL_BUFFADD(u8HalIdx);

            _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32BufStart2,u32BufStart2);

            MS_U8* temp2 = (MS_U8*)MsOS_PA2KSEG1(u32BufStart2+OFFSET_VCHANDSHAKE);

            *temp2 = 0;

            MsOS_FlushMemory();
        }


        u32CmdArg = pstCmdArg->Arg0 | (pstCmdArg->Arg1<<8) | (pstCmdArg->Arg2 << 16) | (pstCmdArg->Arg3 << 24);

        u32Timeout = MsOS_GetSystemTime()+VIRTUAL_CMD_TIMEOUT;

        do
        {
            u32Ret = HAL_VPU_EX_DRAMStreamDispCMDQueueSend(u32Id,(void*)pShm,E_HVD_CMDQ_ARG,u32CmdArg);


            if(u32Ret == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
            {
                bRet = FALSE;
                break;
            }
            else if (MsOS_GetSystemTime() > u32Timeout && u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL )
            {
                bRet = FALSE;
                break;
            }
        }while(u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);

        if(bRet == FALSE || u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL)
        {
            bRet = FALSE;
            goto _CMD_DONE;
        }


        u32Timeout = MsOS_GetSystemTime()+VIRTUAL_CMD_TIMEOUT;

        do
        {
            u32Ret = HAL_VPU_EX_DRAMStreamDispCMDQueueSend(u32Id,(void*)pShm,E_HVD_CMDQ_CMD,u8cmd|(u8VPUIdx<<24)|(pstCmdArg->Arg4<<16));

            if(u32Ret == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
            {
                bRet = FALSE;
                break;
            }
            else if (MsOS_GetSystemTime() > u32Timeout && u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL)
            {
                bRet = FALSE;
                break;
            }
        }while(u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);

        if(bRet == FALSE || u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL)
        {
            bRet = FALSE;
            goto _CMD_DONE;
        }

        if(HAL_MVD_IsNeedResponseCommand(u8cmd) == TRUE)
        {
            u32Timeout = MsOS_GetSystemTime()+VIRTUAL_CMD_TIMEOUT;

            while(MsOS_GetSystemTime() < u32Timeout)
            {
                #if 0
                if (HAL_MVD_IsCmdFinished(u8HalIdx, MVD_HANDSHAKE_VIRTUAL_COMMAND))
                {
                    bResponse = TRUE;
                    break;
                }
                #else
                if(MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VCHANDSHAKE, sizeof(MS_U8)) == 1)
                {
                    bResponse = TRUE;
                    break;
                }
                #endif
            }

            if(bResponse == FALSE)  // fail case
            {
                bRet = FALSE;
                goto _CMD_DONE;
            }

            pstCmdArg->Arg0 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG0, sizeof(MS_U8));
            pstCmdArg->Arg1 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG1, sizeof(MS_U8));
            pstCmdArg->Arg2 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG2, sizeof(MS_U8));
            pstCmdArg->Arg3 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG3, sizeof(MS_U8));
            pstCmdArg->Arg4 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG4, sizeof(MS_U8));
        }

    }
    else if(HAL_MVD_IsNormalCommand(u8cmd) == TRUE)
    {
        if(HAL_MVD_IsNeedResponseCommand(u8cmd) == TRUE)
        {
            // clear handshake dram
            u32BufStart2 = GET_VOL_BUFFADD(u8HalIdx);

            _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32BufStart2,u32BufStart2);

            MS_U8* temp2 = (MS_U8*)MsOS_PA2KSEG1(u32BufStart2+OFFSET_VCHANDSHAKE);

            *temp2 = 0;

            MsOS_ReadMemory();
            MsOS_FlushMemory();
        }

        u32CmdArg = pstCmdArg->Arg0 | (pstCmdArg->Arg1<<8) | (pstCmdArg->Arg2 << 16) | (pstCmdArg->Arg3 << 24);

        u32Timeout = MsOS_GetSystemTime()+VIRTUAL_CMD_TIMEOUT;

        do
        {
            //u32Ret = HAL_VPU_EX_DRAMStreamCMDQueueSend_MVD(pstCmdArg->Arg5,E_HVD_CMDQ_ARG,u32CmdArg);
            u32Ret = HAL_VPU_EX_DRAMStreamCMDQueueSend(u32Id,(void*)pShm,E_HVD_CMDQ_ARG,u32CmdArg);

            if(u32Ret == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
            {
                bRet = FALSE;
                break;
            }
            else if (MsOS_GetSystemTime() > u32Timeout && u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL )
            {
                bRet = FALSE;
                break;
            }
        }while(u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);


        if(bRet == FALSE || u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL)
        {
            bRet = FALSE;
            goto _CMD_DONE;
        }


        u32Timeout = MsOS_GetSystemTime()+VIRTUAL_CMD_TIMEOUT;
        do
        {
            //u32Ret = HAL_VPU_EX_DRAMStreamCMDQueueSend_MVD(pstCmdArg->Arg5,E_HVD_CMDQ_CMD,u8cmd|(pstCmdArg->Arg5<<24)|(pstCmdArg->Arg4<<16));
            u32Ret = HAL_VPU_EX_DRAMStreamCMDQueueSend(u32Id,(void*)pShm,E_HVD_CMDQ_CMD,u8cmd|(u8VPUIdx<<24)|(pstCmdArg->Arg4<<16));

            if(u32Ret == E_HVD_COMMAND_QUEUE_NOT_INITIALED)
            {
                bRet = FALSE;
                break;
            }
            else if (MsOS_GetSystemTime() > u32Timeout && u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL)
            {
                bRet = FALSE;
                break;
            }
        }while(u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL);

        if(bRet == FALSE || u32Ret != E_HVD_COMMAND_QUEUE_SEND_SUCCESSFUL)
        {
            bRet = FALSE;
            goto _CMD_DONE;
        }

        if(HAL_MVD_IsNeedResponseCommand(u8cmd) == TRUE)
        {
            u32Timeout = MsOS_GetSystemTime()+VIRTUAL_CMD_TIMEOUT;

            while(MsOS_GetSystemTime() < u32Timeout)
            {
                #if 0
                if (HAL_MVD_IsCmdFinished(u8HalIdx, MVD_HANDSHAKE_VIRTUAL_COMMAND))
                {
                    bResponse = TRUE;
                    break;
                }
                #else
                if(MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VCHANDSHAKE, sizeof(MS_U8)) == 1)
                {
                    bResponse = TRUE;
                    break;
                }
                #endif
            }

            if(bResponse == FALSE)  // fail case
            {
                bRet = FALSE;
                goto _CMD_DONE;
            }

            pstCmdArg->Arg0 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG0, sizeof(MS_U8));
            pstCmdArg->Arg1 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG1, sizeof(MS_U8));
            pstCmdArg->Arg2 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG2, sizeof(MS_U8));
            pstCmdArg->Arg3 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG3, sizeof(MS_U8));
            pstCmdArg->Arg4 = MVD_GetFWBuffData(u8HalIdx, FW_BUFF_VOLINFO, OFFSET_VIRTUAL_COMMANDARG4, sizeof(MS_U8));
        }
    }
    else
#endif
    {
        if ( HAL_MVD_TimeOut(pstCmdArg->Arg5) == TRUE )
        {
            bRet = FALSE;
            goto _CMD_DONE;
        }

        HAL_MVD_RegWriteByte(MVD_ARG0, pstCmdArg->Arg0);
        HAL_MVD_RegWriteByte(MVD_ARG1, pstCmdArg->Arg1);
        HAL_MVD_RegWriteByte(MVD_ARG2, pstCmdArg->Arg2);
        HAL_MVD_RegWriteByte(MVD_ARG3, pstCmdArg->Arg3);
        HAL_MVD_RegWriteByte(MVD_ARG4, pstCmdArg->Arg4);
        HAL_MVD_RegWriteByte(MVD_ARG5, pstCmdArg->Arg5);
        HAL_MVD_RegWriteByte(MVD_COMMAND, u8cmd);
        if ((CMD_GET_AFD != u8cmd) && (CMD_SLQ_GET_TBL_RPTR != u8cmd) &&
            (CMD_SLQ_UPDATE_TBL_WPTR != u8cmd) && (CMD_GET_NEXTDISPFRM != u8cmd) &&
            (CMD_DECODE_STATUS != u8cmd) && (CMD_RD_PTS != u8cmd) &&
            (CMD_GET_INT_STAT != u8cmd) && (CMD_RD_IO != u8cmd))
        {
            MVD_DEBUG_FWCMD(VPRINTF("MVD_CMD: %x; %x, %x, %x, %x, %x, %x\n", u8cmd, pstCmdArg->Arg0,
            pstCmdArg->Arg1, pstCmdArg->Arg2, pstCmdArg->Arg3, pstCmdArg->Arg4, pstCmdArg->Arg5));
        }

        if ( HAL_MVD_TimeOut(pstCmdArg->Arg5) == TRUE )
        {
            bRet = FALSE;
            goto _CMD_DONE;
        }

        pstCmdArg->Arg0 = HAL_MVD_RegReadByte(MVD_ARG0);
        pstCmdArg->Arg1 = HAL_MVD_RegReadByte(MVD_ARG1);
        pstCmdArg->Arg2 = HAL_MVD_RegReadByte(MVD_ARG2);
        pstCmdArg->Arg3 = HAL_MVD_RegReadByte(MVD_ARG3);
        pstCmdArg->Arg4 = HAL_MVD_RegReadByte(MVD_ARG4);
        pstCmdArg->Arg5 = HAL_MVD_RegReadByte(MVD_ARG5);
    }

_CMD_DONE:
    OSAL_MVD_UnlockHwMutex();

    if (!bRet)
    {
        MVD_DEBUG_FWCMD(VPRINTF("%s timeout dump pc\n", __FUNCTION__));
        MS_U32 x=0;
        for (x=0; x<30; x++)
            MVD_DEBUG_FWCMD(VPRINTF("0x%x\n", HAL_VPU_EX_GetProgCnt()));
        MVD_DEBUG_FWCMD(VPRINTF("###\n"));
    }

    return bRet;
}

void HAL_MVD_SetSyncClk(MS_BOOL bEnable)
{
    MS_ASSERT(0==bEnable);//Notice: Euclid & T3 have no sync_mode; Bit4 must be 0.

    OSAL_MVD_LockHwMutex();
    HAL_MVD_RegWriteBit(MVD_CTRL, bEnable, MVD_CTRL_CLK_SYNCMODE);
    OSAL_MVD_UnlockHwMutex();

    return;
}

MS_BOOL HAL_MVD_InitHW(void)
{
    //Set MVD Clock @ reg_CHIPTOP
    HAL_MVD_PowerCtrl(ENABLE);

    //Set MVD Clock aync
    HAL_MVD_SetSyncClk(DISABLE);

    //MVD reset
    if(!HAL_MVD_RstHW())
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_MVDInit:MVD4ResetHW failed\n"));
        return FALSE;
    }
    else
    {
        MVD_DEBUGINFO(VPRINTF("MDrv_MVD_MVDInit:MVD4ResetHW success\n"));
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/// Get MVD firmware version
/// @return -firmware version
//------------------------------------------------------------------------------
MS_U32 HAL_MVD_GetFWVer(MS_U32 u32VpuSid)
{
#if 0
    return HAL_VPU_EX_GetFWVer(u32VpuSid, E_VPU_EX_FW_VER_MVD_FW);
#else
    UNUSED(u32VpuSid);
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_GET_FW_VERSION, &mvdcmd ) == FALSE)
    {
        MVD_ERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_GET_FW_VERSION ) );
        return 0;
    }
    return COMBU32(mvdcmd.Arg3,mvdcmd.Arg2,mvdcmd.Arg1,mvdcmd.Arg0);
#endif
}

//------------------------------------------------------------------------------
/// Get MVD firmware interface version
/// @return -firmware interface version
//------------------------------------------------------------------------------
MS_U32 HAL_MVD_GetFWIfVer(MS_U32 u32VpuSid)
{
#if 0
    return HAL_VPU_EX_GetFWVer(u32VpuSid, E_VPU_EX_FW_VER_MVD_IF);
#else
    UNUSED(u32VpuSid);
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    if (HAL_MVD_MVDCommand( CMD_INTERFACE_VERSION, &mvdcmd ) == FALSE)
    {
        MVD_ERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_INTERFACE_VERSION ) );
        return 0;
    }
    return COMBU32(mvdcmd.Arg3,mvdcmd.Arg2,mvdcmd.Arg1,mvdcmd.Arg0);
#endif
}

//------------------------------------------------------------------------------
/// Check MVD firmware status
/// @return -firmware is ready or not
//------------------------------------------------------------------------------

#ifdef VDEC3
static MS_BOOL _MVD_Check_FW_Rdy(MS_U32 u32VpuSid)
{
    MS_U32 u32Id = MVD_GetStreamId(u32VpuSid);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u32VpuSid);
    MS_U32 u32TimeOut = MsOS_GetSystemTime() + 500;
    unsigned int* pVersion = (unsigned int*)MsOS_PA2KSEG1(pstMemCfg->u32FWCodeAddr + OFFSET_BASE + u8VPUIdx*0x100000);
    MS_BOOL ret = FALSE;

    while(MsOS_GetSystemTime() < u32TimeOut)
    {
        if(((*pVersion) == FW_VERSION) && ((*(pVersion+4)) == INTERFACE_VERSION))
        {
            ret = TRUE;
            break;
        }
    }

    if (ret == FALSE)
    {
        MVD_ERROR(VPRINTF("MVD f/w header version is wrong,%x,%x,%x,%x\n",(*(pVersion+4)), INTERFACE_VERSION,(*pVersion), FW_VERSION));
    }

    return ret;
}

#else
static MS_BOOL _MVD_Check_FW_Rdy(MS_U32 u32VpuSid)
{
    MS_U32 u32TimeOut = 2000;

    //check firmware version consistent with header file
    while ((INTERFACE_VERSION != HAL_MVD_GetFWIfVer(u32VpuSid)) && (--u32TimeOut));
    if (u32TimeOut == 0)
    {
        MVD_ERROR(VPRINTF("MVD_FW_IF_Version=%lx inconsistent with header file(%x)!\n",
            HAL_MVD_GetFWIfVer(u32VpuSid), INTERFACE_VERSION));
        return FALSE;
    }

    if (FW_VERSION != HAL_MVD_GetFWVer(u32VpuSid))
    {
        MVD_DEBUGINFO(VPRINTF("Warning! FWBinVer(%lx) != FWHdrVer(%x)\n", HAL_MVD_GetFWVer(u32VpuSid), FW_VERSION));
    }
    MVD_DEBUGINFO(VPRINTF("MVD version Interface = %x, FW = %x\n", INTERFACE_VERSION, FW_VERSION));

    return TRUE;
}
#endif
MS_BOOL _DumpCtrl(MS_U8 u8Idx)
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);

    //VPRINTF("u32VA=0x%lx, PA=0x%x\n", u32VA, pstMemCfg->u32FWCodeAddr);
    struct _ctl_info *ctl_ptr = (struct _ctl_info *)
        HAL_MVD_PA2NonCacheSeg(pstMemCfg->u32FWCodeAddr + CTL_INFO_ADDR);
    MS_U32 u32timeout = HAL_MVD_GetTime() + 1000;//u32HVDCmdTimeout;

    HAL_MVD_ReadMemory();

    MVD_DEBUGINFO(VPRINTF("version=0x%x, statue=0x%x, last_ctl_cmd=0x%x, last_ctl_arg=0x%x\n",
        ctl_ptr->verion, ctl_ptr->statue, ctl_ptr->last_ctl_cmd, ctl_ptr->last_ctl_arg));

    while (CTL_TASK_CMDRDY != ctl_ptr->statue)
    {
        if (HAL_MVD_GetTime() > u32timeout)
        {
            MVD_DEBUGERROR(VPRINTF("CTRL timeout!!! %d\n", __LINE__));
            return FALSE;
        }
    }

    MVD_DEBUGINFO(VPRINTF("Version=0x%x, statue=0x%x, last_ctl_cmd=0x%x, last_ctl_arg=0x%x\n",
        ctl_ptr->verion, ctl_ptr->statue, ctl_ptr->last_ctl_cmd, ctl_ptr->last_ctl_arg));

    MS_U8 i;

    for (i = 0; i < 4; i++)
    {
        MVD_DEBUGINFO(VPRINTF("%s: Task %d, status=%d\n", __FUNCTION__, i, ctl_ptr->task_statue[i]));
    }
    return TRUE;
}


static VPU_EX_SourceType MVD_MapVpuSrcType(MVD_SrcMode eSrcMode)
{
    VPU_EX_SourceType eVpuSrcType = E_VPU_EX_INPUT_NONE;
    switch (eSrcMode)
    {
        case E_MVD_TS_MODE:
        case E_MVD_TS_FILE_MODE:
            eVpuSrcType = E_VPU_EX_INPUT_TSP;
            break;

        case E_MVD_SLQ_TBL_MODE:
        case E_MVD_SLQ_MODE:
        case E_MVD_FILE_MODE:
            eVpuSrcType = E_VPU_EX_INPUT_FILE;
            break;

        case E_MVD_SRC_UNKNOWN:
        default:
            break;
    }
    return eVpuSrcType;
}

static void MVD_GetTaskInfo(VPU_EX_TaskInfo* pstTaskInfo, MS_U8 u8Idx, HAL_VPU_StreamId eVpuId)
{
    pstTaskInfo->u32Id = (u8Idx << 8 | eVpuId);
    pstTaskInfo->eDecType = E_VPU_EX_DECODER_MVD;
    pstTaskInfo->eVpuId = eVpuId;
    pstTaskInfo->eSrcType = MVD_MapVpuSrcType(HAL_MVD_GetSrcMode(u8Idx));
    pstTaskInfo->u32HeapSize = MVD_DRAM_SIZE;
}


MS_BOOL HAL_MVD_CreateTask(MS_U8 u8Idx, HAL_VPU_StreamId eVpuId)
{
    MS_BOOL bRet = FALSE;
    MS_BOOL bFWdecideFB = FALSE;
    MVD_FWCfg* pstCfg = HAL_MVD_GetFWCfg(u8Idx);
    HAL_VPU_EX_SetFWReload(!(pstCfg->bNotReload));

    MVD_DEBUGINFO(VPRINTF("\n\n Create Task(%x)!!!\n", u8Idx));
    VPU_EX_TaskInfo stTaskInfo;
    MVD_GetTaskInfo(&stTaskInfo, u8Idx, eVpuId);

    VPU_EX_FWCodeCfg stVpuFWCfg;
    VPU_EX_NDecInitPara stVpuInitPara;
    stVpuInitPara.pFWCodeCfg = &stVpuFWCfg;
    stVpuInitPara.pTaskInfo = &stTaskInfo;
    stVpuInitPara.pVLCCfg   = NULL;

    MVD_MEMCfg* pMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    stVpuFWCfg.u32BinAddr = HAL_MVD_PA2NonCacheSeg(pMemCfg->u32FWBinAddr);
    stVpuFWCfg.u32BinSize = pMemCfg->u32FWBinSize;
    stVpuFWCfg.u32DstAddr = HAL_MVD_PA2NonCacheSeg(pMemCfg->u32FWCodeAddr);
    stVpuFWCfg.u32DstSize = pMemCfg->u32FWCodeSize;
    stVpuFWCfg.u8SrcType  = pMemCfg->eFWSrcType;

#ifdef VDEC3
    VPU_EX_FBCfg pFBCfg;
    stVpuInitPara.pFBCfg = &pFBCfg;

    if(pMVDHalContext->bCMAUsed == TRUE)
    {
        stVpuInitPara.pFBCfg->u32FrameBufAddr = (MS_VIRT)pMVDHalContext->cmaInitParam.heap_miu_start_offset;
        stVpuInitPara.pFBCfg->u32FrameBufSize = (MS_VIRT)pMVDHalContext->cmaInitParam.heap_length;
        bFWdecideFB = TRUE;

    }
    stTaskInfo.u8HalId = u8Idx;
    pMVDHalContext->_stMVDStream[u8Idx].u32SLQId = HAL_VPU_EX_GetBBUId(stTaskInfo.u32Id, &stTaskInfo, FALSE);
    if(pMVDHalContext->_stMVDStream[u8Idx].u32SLQId == HAL_VPU_INVALID_BBU_ID)
    {
        MVD_DEBUGINFO(VPRINTF("ERROR!!!MVD GetBBU Id Fail \n"));
    }
    else
    {
        bRet = HAL_VPU_EX_TaskCreate((MS_U32)eVpuId, (void*)&stVpuInitPara, bFWdecideFB, pMVDHalContext->_stMVDStream[u8Idx].u32SLQId);
    }

    if(bRet == FALSE)
    {
        if(!HAL_VPU_EX_FreeBBUId(stTaskInfo.u32Id, pMVDHalContext->_stMVDStream[u8Idx].u32SLQId, &stTaskInfo))
        {
            MVD_DEBUGERROR(VPRINTF("[%s][%d]ERROR!!! Free BBU Id Fail !!!\n",__FUNCTION__,__LINE__));
        }
    }
#else
    bRet = HAL_VPU_EX_TaskCreate((MS_U32)eVpuId, (void*)&stVpuInitPara);
#endif
    MVD_DEBUGINFO(VPRINTF(" Create Task!!! bRet=%x ###\n\n", bRet));
    _DumpCtrl(u8Idx);

    return bRet;
}

MS_BOOL HAL_MVD_DeleteTask(MS_U8 u8Idx, HAL_VPU_StreamId eVpuId)
{
    MS_BOOL bRet = FALSE;
    MVD_DEBUGINFO(VPRINTF("\n\n Delete Task(%x)!!!\n", u8Idx));

    VPU_EX_TaskInfo stTaskInfo;
    MVD_GetTaskInfo(&stTaskInfo, u8Idx, eVpuId);

    VPU_EX_FWCodeCfg stVpuFWCfg;
    VPU_EX_NDecInitPara stVpuInitPara;
    stVpuInitPara.pFWCodeCfg = &stVpuFWCfg;
    stVpuInitPara.pTaskInfo = &stTaskInfo;

    MVD_MEMCfg* pMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    stVpuFWCfg.u32BinAddr = HAL_MVD_PA2NonCacheSeg(pMemCfg->u32FWBinAddr);
    stVpuFWCfg.u32BinSize = pMemCfg->u32FWBinSize;
    stVpuFWCfg.u32DstAddr = HAL_MVD_PA2NonCacheSeg(pMemCfg->u32FWCodeAddr);
    stVpuFWCfg.u8SrcType  = pMemCfg->eFWSrcType;

#ifdef VDEC3
    if(!HAL_VPU_EX_FreeBBUId(stTaskInfo.u32Id, pMVDHalContext->_stMVDStream[u8Idx].u32SLQId, &stTaskInfo))
    {
        MVD_DEBUGINFO(VPRINTF("ERROR!!! Free BBU Id Fail !!!\n"));
    }
#endif
    bRet = HAL_VPU_EX_TaskDelete((MS_U32) eVpuId, &stVpuInitPara);
    MVD_DEBUGINFO(VPRINTF(" Delete Task!!! ###\n\n"));
    _DumpCtrl(u8Idx);
    return bRet;
}

MS_BOOL HAL_MVD_InitFW(MS_U32 u32VpuSid)
{
    //to fix the timing issue of getting FWIfVer on Chakra2
    HAL_MVD_Delayms(1);

    //check FW ready
    if ( !_MVD_Check_FW_Rdy(u32VpuSid))
    {
        MS_ASSERT(0);
        return FALSE;
    }

#if 0
    if(pMVDHalContext->_stMVDStream[0].bUsed == FALSE && pMVDHalContext->_stMVDStream[1].bUsed == FALSE)   // no mvd is used
    {
        MVD_CmdArg mvdcmd;
        SETUP_CMDARG(mvdcmd);
        mvdcmd.Arg0 = 1; //reset mvd engine,if have only one mvd
        SET_DECNUM(mvdcmd, ((MS_U8)u32VpuSid));
        if (HAL_MVD_MVDCommand( CMD_SW_RESET, &mvdcmd ) == FALSE)
        {
            MVD_ERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SW_RESET ) );
            return FALSE;
        }
    }
#endif
    return TRUE;
}

#define _MVD_INIT_FAIL_RET()                                            \
            do {                                                        \
                HAL_MVD_SetIsUsed(u8HalIdx, FALSE);                     \
                return FALSE;                                           \
            } while (0)


MS_BOOL HAL_MVD_Init(MS_U8 u8HalIdx,MVD_CodecType eCodecType, MS_U32 u32VpuSid)
{
    //OSAL_MVD_MutexInit();
    MS_BOOL no_use = FALSE;
    MS_BOOL ret;

    ret = MDrv_MVD_AUTH_IPCheck(eCodecType,&no_use);
    if(ret == FALSE)
    {
        return FALSE;
    }

    if (!HAL_MVD_CreateTask(u8HalIdx, (HAL_VPU_StreamId)u32VpuSid))
    {
        _MVD_INIT_FAIL_RET();
    }

    pMVDHalContext->bStopped[u8HalIdx] = FALSE;

    if (!HAL_MVD_InitFW(u8HalIdx))
    {
        MVD_DEBUGERROR(VPRINTF("%s:HAL_MVD_InitFW(%x) failed\n", __FUNCTION__, u8HalIdx));
        if (!HAL_MVD_DeleteTask(u8HalIdx, (HAL_VPU_StreamId)u32VpuSid))
        {
            MVD_DEBUGERROR(VPRINTF("%s:HAL_MVD_DeleteTask failed\n", __FUNCTION__));
        }

        _MVD_INIT_FAIL_RET();
    }
    else
    {
        MVD_DEBUGINFO(VPRINTF("%s:HAL_MVD_InitFW(%x) success\n", __FUNCTION__, u8HalIdx));
    }

    HAL_MVD_SetIsUsed(u8HalIdx, TRUE);

    return TRUE;
}

#define _MVD_CMDRDY ((HAL_MVD_RegReadByte(MVD_STATUS) & MVD_STATUS_READY) == MVD_STATUS_READY)
//------------------------------------------------------------------------------
/// Check if MVD command is ready
/// @return TRUE or FALSE
///     - TRUE, Success to process the command
///     - FALSE, Failed due to timeout
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_GetCmdRdy(void)
{
    MS_U32 timeoutTick = 2000;

    while ((!_MVD_CMDRDY) && ((timeoutTick--)!=0));

    if (0 == timeoutTick)
        return FALSE;
    else
        return TRUE;
}

MS_BOOL HAL_MVD_CheckIdle(void)
{
    MS_BOOL bIsIdle = FALSE;
    MVD_CmdArg mvdcmd;

    //issue CheckIdle command
    SETUP_CMDARG(mvdcmd);
    SET_CMD_RET_FALSE(CMD_MVD_IDLE, &mvdcmd);

    bIsIdle = (mvdcmd.Arg0 == 1);
    if (HAL_MVD_GetCmdRdy())
    {
        return bIsIdle;
    }
    else
    {
        return FALSE;
    }
}


static MS_BOOL _MVD_SoftRstHW(void)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/// Soft-reset MVD
/// Ref AP note p.12 HK2MVD Reset Flow
/// @return TRUE or FALSE
///     - TRUE, Success to soft-reset MVD
///     - FALSE, Failed. Need init MVD again.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SoftRstHW(void)
{
    MS_U32 timetick = 2000;

    if (HAL_MVD_GetCmdRdy())
    {
        //idle check
        while ((!HAL_MVD_CheckIdle()) && ((timetick--)!=0));

        //either MVD is idle or timeout, do ENG/SLQ reset
        return _MVD_SoftRstHW(); //Reset HW engine
    }
    else
    {
        return FALSE; //here means "CPU hanging"
    }
}

//------------------------------------------------------------------------------
/// Clean the IRQ bit (in interrupt handler should call this function while the
/// interrupt has been triggered.
/// @param none
/// @return none
/// @internal
//------------------------------------------------------------------------------
void HAL_MVD_ClearIRQ(void)
{
    OSAL_MVD_LockHwMutex();
    HAL_MVD_RegWriteBit(MVD_CTRL, 1, MVD_CTRL_CLR_INT);
    OSAL_MVD_UnlockHwMutex();
    return;
}

//------------------------------------------------------------------------------
/// Set display speed.
///FW use (# of B frames) / (# of decode frames) < Ratio this formula to adjustment.
///Once if the ratio is 1, that means, whenever (#Bframes / #decoded) < 1, then
///FW would drop the B frame.
///In other words, once AP need to back to normal mode, AP have to set the arg0 to 0.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetSpeed(MS_U8 u8Idx, MVD_SpeedType eSpeedType, MS_U8 u8Multiple)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);

    if (E_MVD_SPEED_FAST == eSpeedType)
        mvdcmd.Arg0 = 1; //fast forward
    else if (E_MVD_SPEED_SLOW == eSpeedType)
        mvdcmd.Arg0 = 2; //slow motion
    else
        mvdcmd.Arg0 = 0; //normal speed

    if (u8Multiple == 1)
    {
        mvdcmd.Arg0 = 0;
        //The only way to be NORMAL speed.
    }

    mvdcmd.Arg1 = u8Multiple;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_DISP_SPEED_CTRL, &mvdcmd);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set frame buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetFrameBuffAddr(MS_U8 u8Idx, MS_VIRT u32addr, MS_U8 u8fbMode)
{
    MVD_CmdArg mvdcmd;
    if(pMVDHalContext->bCMAUsed)
    {
        u8fbMode = MVD_CMA_MODE;
    }

    if ((u32addr>>3) > MAX_ADD_28BIT)// TODO: fixme, in 64bits system, address may be more than 28 bits
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_SetFrameBuffAddr: only support 28bit add!\n"));
        return;
    }

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32addr);
    mvdcmd.Arg1 = H_WORD(u32addr);
    mvdcmd.Arg2 = L_DWORD(u32addr);

    //Frame Buffer Mode Setting
    mvdcmd.Arg3 = u8fbMode | ((u32addr>>24)&0x0f);
    mvdcmd.Arg4 = u8fbMode & 0xff;
    MVD_DEBUGINFO(VPRINTF("FramebufferAdd 0x%lx, FB Mode 0x%x, arg3=0x%x\n", (unsigned long)u32addr, u8fbMode, mvdcmd.Arg3));
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_FB_BASE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FB_BASE ) );
        return;
    }

    return;
}

//------------------------------------------------------------------------------
/// Set header buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetHeaderBufferAddr (MS_U8 u8Idx, MS_VIRT u32addr )
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_HEADER_INFO_BUF, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_HEADER_INFO_BUF ) );
    }
    return;
}

//------------------------------------------------------------------------------
/// Set vol info buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetVolInfoBufferAddr (MS_U8 u8Idx, MS_VIRT u32addr )
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_VOL_INFO_BUF, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_VOL_INFO_BUF ) );
    }
    return;
}

//------------------------------------------------------------------------------
/// Set frame info buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetFrameInfoBufferAddr (MS_U8 u8Idx, MS_VIRT u32addr )
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_FRAME_INFO_BUF, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FRAME_INFO_BUF ) );
    }
    return;
}

//------------------------------------------------------------------------------
/// Set IAP buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetIAPBufferAddr (MS_U8 u8Idx, MS_VIRT u32addr )
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_CmdArg mvdcmd;
    MS_ASSERT((u32addr%8192)==0);
    u32addr >>= 13;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    mvdcmd.Arg4 = pstMemCfg->u8FBMiuSel;
    if (HAL_MVD_MVDCommand( CMD_IAP_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_IAP_BUF_START ) );
    }
    return;
}

//------------------------------------------------------------------------------
/// Set Data Partition buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetDPBufferAddr (MS_U8 u8Idx, MS_VIRT u32addr )
{
    MVD_CmdArg mvdcmd;
    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DP_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DP_BUF_START ) );
    }
    return;
}

//------------------------------------------------------------------------------
/// Set Motion Vector buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetMVBufferAddr (MS_U8 u8Idx, MS_VIRT u32addr )
{
    MVD_CmdArg mvdcmd;
    MS_ASSERT((u32addr%2048)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_MV_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_MV_BUF_START ) );
    }
    return;
}

static void _MVD_SetUserDataBufStart(MS_U8 u8Idx, MS_VIRT u32addr, MS_U8 u8arg3)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;
    MVD_DEBUGINFO(VPRINTF("%s add=0x%lx arg3=0x%x\n", __FUNCTION__, (unsigned long)u32addr, u8arg3));

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32addr);
    mvdcmd.Arg1 = H_WORD(u32addr);
    mvdcmd.Arg2 = L_DWORD(u32addr);
    mvdcmd.Arg4 = H_DWORD(u32addr);
    mvdcmd.Arg3 = u8arg3;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_USER_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_USER_BUF_START ) );
        return;
    }
    return;
}

static void _MVD_SetUserDataBufSize(MS_U8 u8Idx, MS_U32 u32size, MS_U8 u8arg3)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32size%8)==0);
    u32size >>= 3;
    MVD_DEBUGINFO(VPRINTF("%s add=0x%x arg3=0x%x\n", __FUNCTION__, u32size, u8arg3));

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32size);
    mvdcmd.Arg1 = H_WORD(u32size);
    mvdcmd.Arg2 = L_DWORD(u32size);
    mvdcmd.Arg4 = H_DWORD(u32size);
    mvdcmd.Arg3 = u8arg3;
    if(mvdcmd.Arg3 <= 1) //mean 608 and 708 in MM RVU
    {
      mvdcmd.Arg2 = 7;//ntsc1+ntsc2+atsc
      mvdcmd.Arg4 = 0;
    }
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_USER_BUF_SIZE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_USER_BUF_SIZE ) );
        return;
    }
    return;
}

//------------------------------------------------------------------------------
/// Set user data buffer address to MVD
/// @param -u32addr \b IN : start address
//------------------------------------------------------------------------------
void HAL_MVD_SetUserDataBuf(MS_U8 u8Idx, MS_VIRT u32addr, MS_U32 u32size)
{
    MS_U8 u8ccType = 0;

    MS_ASSERT((u32addr%8)==0);
    MS_ASSERT((u32size%8)==0);
#ifdef REDLION_LINUX_KERNEL_ENVI
    u8ccType = 2;
#elif defined(MVD_SUPPORT_X4_CC)
    u8ccType = 4;   //display order
#else
    u8ccType = 2;// 2 for testing 0;
#endif

#if defined(MVD_SUPPORT_X4_CC)
    //set decoding buffer address
    _MVD_SetUserDataBufStart(u8Idx, u32addr+u32size, 3);
#endif

    //set CC output buffer address
    _MVD_SetUserDataBufStart(u8Idx, u32addr, u8ccType);

#if defined(MVD_SUPPORT_X4_CC)
    //set decoding buffer size
    _MVD_SetUserDataBufSize(u8Idx, u32size, 3);
#endif

    //set CC output buffer size
    _MVD_SetUserDataBufSize(u8Idx, u32size, u8ccType);

    return;
}

void HAL_MVD_SetSLQTblBufStartEnd(MS_U8 u8Idx, MS_VIRT u32start, MS_VIRT u32end)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32val = u32end>>3;
    MVD_DEBUGINFO(VPRINTF("%s st=0x%lx end=0x%lx\n", __FUNCTION__, (unsigned long)u32start, (unsigned long)u32end));

    SET_CMDARG(mvdcmd, u32val, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_SLQ_TBL_BUF_END, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SLQ_TBL_BUF_END ) );
        return;
    }

    u32val = (u32start)>>3;
    SET_CMDARG(mvdcmd, u32val, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_SLQ_TBL_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SLQ_TBL_BUF_START ) );
        return;
    }

    MVD_DEBUGINFO(VPRINTF("%s st=0x%lx end=0x%lx OK!!!\n", __FUNCTION__, (unsigned long)u32start, (unsigned long)u32end));
    return;
}

//------------------------------------------------------------------------------
/// Issue StepDisplay command.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_StepDisp(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_STEP_DISP_DECODE_ONE, &mvdcmd);
    if (HAL_MVD_Resume(u8Idx) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: HAL_MVD_Resume fail!!\r\n" ) );
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// Get ES read address for TS file mode.
/// @return ES read address
//------------------------------------------------------------------------------
MS_VIRT HAL_MVD_GetTsFileESReadPtr(MS_U8 u8Idx)
{
    MS_VIRT u32Add = 0;
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 2;    //ES diff
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_PARSER_READ_POSITION, &mvdcmd ) == TRUE)
    {
        //in order to latch the newest parser status
        //u32Diff = (((MS_U32)mvdcmd.Arg3) <<24) | (((MS_U32)mvdcmd.Arg2) <<16) |
        //          (((MS_U32)mvdcmd.Arg1) << 8) | (((MS_U32)mvdcmd.Arg0));
    }
    else
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail!!\n", CMD_PARSER_READ_POSITION) );
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_PARSER_READ_POSITION, &mvdcmd ) == TRUE)
    {
        u32Add = (((MS_U32)mvdcmd.Arg3) <<24) |
                 (((MS_U32)mvdcmd.Arg2) <<16) |
                 (((MS_U32)mvdcmd.Arg1) << 8) |
                 (((MS_U32)mvdcmd.Arg0));
    }
    else
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail!!\n", CMD_PARSER_READ_POSITION) );
    }

    return (u32Add*8);
}

//------------------------------------------------------------------------------
/// Get ES write address for TS file mode.
/// @return ES write address
//------------------------------------------------------------------------------
MS_VIRT HAL_MVD_GetTsFileESWritePtr(MS_U8 u8Idx)
{
    MS_VIRT u32Diff = 0;
    MS_VIRT u32WrPtr = 0;
    MVD_CmdArg mvdcmd;
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 2;    //ES diff
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_PARSER_READ_POSITION, &mvdcmd ) == TRUE)
    {
        u32Diff = (((MS_U32)mvdcmd.Arg3) <<24) | (((MS_U32)mvdcmd.Arg2) <<16) |
                  (((MS_U32)mvdcmd.Arg1) << 8) | (((MS_U32)mvdcmd.Arg0));
    }
    else
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail!!\n", CMD_PARSER_READ_POSITION) );
    }

    u32WrPtr = u32Diff*8 + HAL_MVD_GetTsFileESReadPtr(u8Idx);
    if (u32WrPtr > pstSlqTblInfo->u32ESBuffEnd)
    {
        MVD_DEBUGINFO(VPRINTF("ES wrapping Wr=0x%lx ==> ", (unsigned long)u32WrPtr));
        u32WrPtr -= pstMemCfg->u32BSSize;
        MVD_DEBUGINFO(VPRINTF("0x%lx\n", (unsigned long)u32WrPtr));
    }
    return u32WrPtr;
}

//------------------------------------------------------------------------------
/// Enable/Disable firmware to show the last frame.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_EnableLastFrameShow(MS_U8 u8Idx, MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;
    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_VIRT* pu32FileEndPtr = &pstSlqTblInfo->u32FileEndPtr;

    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
    #if SLQ_NEW_PUSH
        if (pstSlqTblInfo->pSlqStatus->bSlqCtrlBit)
        {
            pstSlqTblInfo->pDrvSlqTbl->u32WrPtr = pstSlqTblInfo->pSlqStatus->u32VaildWptrAddr + SLQ_ENTRY_LEN;
        }
    #endif //
        //save current writePtr
        if (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr != pstSlqTblInfo->pDrvSlqTbl->u32EndAdd)
        {
            *pu32FileEndPtr = pstSlqTblInfo->pDrvSlqTbl->u32WrPtr;
        }
        else
        {
            *pu32FileEndPtr = pstSlqTblInfo->pDrvSlqTbl->u32StAdd;
        }
        MVD_DEBUGINFO(VPRINTF("fe=%lx, rd=%lx, wr=%lx\n", (unsigned long)*pu32FileEndPtr,
                        (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32RdPtr, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32WrPtr));
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_ENABLE_LAST_FRAME_SHOW, &mvdcmd);
    return TRUE;
}


MS_BOOL HAL_MVD_SlqTblRst(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_VC1_HW_SLQ_RESET, &mvdcmd);
    pstSlqTblInfo->bEnSlqTblHkCtrl = FALSE;
    return TRUE;
}

MS_BOOL HAL_MVD_SeekToPTS(MS_U8 u8Idx, MS_U32 u32Pts)
{
    MVD_CmdArg mvdcmd;

    SET_CMDARG(mvdcmd, u32Pts, u8Idx);
    SET_CMD_RET_FALSE(CMD_STEP_TO_PTS, &mvdcmd);

    if (HAL_MVD_Resume(u8Idx) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: HAL_MVD_Resume fail!!\r\n" ) );
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_MVD_SkipToPTS(MS_U8 u8Idx, MS_U32 u32Pts)
{
    MVD_CmdArg mvdcmd;

    SET_CMDARG(mvdcmd, u32Pts, u8Idx);
    SET_CMD_RET_FALSE(CMD_SKIP_TO_PTS, &mvdcmd);

    if (HAL_MVD_Resume(u8Idx) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: HAL_MVD_Resume fail!!\r\n" ) );
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_MVD_TrickPlay(MS_U8 u8Idx, MVD_TrickDec trickDec, MS_U8 u8DispDuration)
{
    MVD_CmdArg mvdcmd;
    MS_U8 u8DecType;

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
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FAST_SLOW, &mvdcmd);

    pMVDHalContext->stCtrlCfg[u8Idx].eTrickMode = trickDec;
    return TRUE;
}

MS_BOOL HAL_MVD_FlushDisplayBuf(MS_U8 u8Idx)
{
    #define STOP_TIMEOUT 500 //ms
    MS_U32 u32StartTime = 0;

    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_FLUSHQUEUE_COMMAND);
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FLUSH_DISP_QUEUE, &mvdcmd);

    u32StartTime = HAL_MVD_GetTime();

    while(!HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_FLUSHQUEUE_COMMAND))
    {
        if ((HAL_MVD_GetTime()-u32StartTime)>STOP_TIMEOUT)
        {
            MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail timeout!!\r\n", CMD_FLUSH_DISP_QUEUE ) );
            break;
        }
    }

    return TRUE;
}


MS_BOOL HAL_MVD_SetFileModeAVSync(MS_U8 u8Idx, MVD_TIMESTAMP_TYPE eSyncMode)
{
    MVD_CmdArg mvdcmd;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    pstCtrlCfg->eFileSyncMode = eSyncMode;
    MVD_DEBUGINFO(VPRINTF("%s eSyncMode=%d\n", __FUNCTION__, eSyncMode));
    SETUP_CMDARG(mvdcmd);
    switch (eSyncMode)
    {
        case E_MVD_TIMESTAMP_PTS:
        case E_MVD_TIMESTAMP_PTS_RVU:
            mvdcmd.Arg0 = FILE_PTS_MODE;
            break;

        case E_MVD_TIMESTAMP_DTS:
        case E_MVD_TIMESTAMP_DTS_RVU:
            mvdcmd.Arg0 = FILE_DTS_MODE;
            break;
        case E_MVD_TIMESTAMP_NEW_STS:
            mvdcmd.Arg0 = FILE_STS_MODE;
            break;
        case E_MVD_TIMESTAMP_FREERUN:
        default:
            mvdcmd.Arg0 = NONE_FILE_MODE; //Freerun
            break;
    }
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_ENABLE_FILE_SYNC, &mvdcmd);

    SETUP_CMDARG(mvdcmd);
    switch (eSyncMode) //for set RVU mode
    {
        case E_MVD_TIMESTAMP_PTS_RVU:
             mvdcmd.Arg0 = FILE_PTS_MODE;
             break;
        case E_MVD_TIMESTAMP_DTS_RVU:
             mvdcmd.Arg0 = FILE_DTS_MODE;
             break;
        case E_MVD_TIMESTAMP_FREERUN:
        default:
             mvdcmd.Arg0 = 0xFF;
             break;
    }
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_RVU_EN, &mvdcmd);
    return TRUE;
}




//------------------------------------------------------------------------------
/// Set the start address of PTS table used for SLQ table link mode.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetPtsTblAddr(MS_U8 u8Idx, MS_VIRT u32addr)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    SET_CMD_RET_FALSE(CMD_PTS_TBL_START, &mvdcmd);
    return TRUE;
}

MS_BOOL HAL_MVD_SkipToIFrame(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_START_DEC_STRICT, &mvdcmd);
    return TRUE;
}

//Map driver FRC (Frame rate conversion) mode to firmware's.
MS_U8 HAL_MVD_MapFrcMode(MVD_FrcMode eFrcMode)
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

MS_BOOL HAL_MVD_DispCtrl(MS_U8 u8Idx, MS_BOOL bDecOrder, MS_BOOL bDropErr, MS_BOOL bDropDisp, MVD_FrcMode eFrcMode)
{
    MVD_CmdArg mvdcmd;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    MS_BOOL* pbDropErrFrm = &pstCtrlCfg->bDropErrFrm;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = (MS_U8)bDecOrder;
    mvdcmd.Arg1 = (MS_U8)bDropErr;
    mvdcmd.Arg2 = (MS_U8)bDropDisp;
    mvdcmd.Arg3 = HAL_MVD_MapFrcMode(eFrcMode);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_DISPLAY_CTL, &mvdcmd);

    pstCtrlCfg->eFrcMode = eFrcMode;

    if (*pbDropErrFrm != bDropErr)
    {
        MVD_DEBUGINFO( VPRINTF("bDropErrFrm(%d) != bDropErr(%d)\n", *pbDropErrFrm, bDropErr));
        *pbDropErrFrm = bDropErr;
    }
    pstCtrlCfg->bDropDispfrm = bDropDisp;

    return TRUE;
}


MS_BOOL HAL_MVD_SkipData(MS_U8 u8Idx)
{
    MS_U32 u32TimeCnt = 0;
    MVD_CmdArg mvdcmd;

    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_SKIP_DATA);
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SKIP_DATA, &mvdcmd);

    u32TimeCnt = HAL_MVD_GetTime();
    while ((HAL_MVD_GetTime() - u32TimeCnt) < SKIP_DATA_TIMEOUT_MS)
    {
        if (HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_SKIP_DATA))
        {
            MVD_DEBUGINFO(VPRINTF("\nSkip data finished!\n"));
            break;
        }
    }
    MVD_DEBUGINFO(VPRINTF("====> %s (t1=%u t2=%u diff=%u)\n", __FUNCTION__,
        u32TimeCnt, HAL_MVD_GetTime(), (HAL_MVD_GetTime() - u32TimeCnt)));
    if (TRUE != HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_SKIP_DATA))
    {
        MVD_DEBUGINFO(VPRINTF("\n***** TS flush timeout *****\n\n"));
        return FALSE;
    }

        return TRUE;
    }

MS_BOOL HAL_MVD_DispRepeatField(MS_U8 u8Idx, MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_REPEAT_MODE, &mvdcmd);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Pause display.
/// @return -TRUE for success; FALSE for failure
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_PauseDisp(MS_U8 u8Idx)
{
    MS_BOOL bRst = TRUE;
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = DISPLAY_PAUSE_ON;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_DISPLAY_PAUSE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x(ON) fail!!\r\n", CMD_DISPLAY_PAUSE) );
        bRst = FALSE;
    }
    return bRst;
}

//------------------------------------------------------------------------------
/// Issue Pause command.
/// @return -TRUE for success; FALSE for failure
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_Resume(MS_U8 u8Idx)
{
    MS_BOOL bRst = TRUE;
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = DISPLAY_PAUSE_OFF;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_DISPLAY_PAUSE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DISPLAY_PAUSE) );
        bRst = FALSE;
    }
    return bRst;
}

MS_BOOL HAL_MVD_Play(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_DIU_WIDTH_ALIGN, &mvdcmd);

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_PLAY, &mvdcmd);

    if (HAL_MVD_Resume(u8Idx) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: HAL_MVD_Resume fail!!\r\n" ) );
        return FALSE;
    }

    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    pCtrlCfg->bDecodeIFrame = FALSE;

    return TRUE;
}

//------------------------------------------------------------------------------
/// Set base address for ScalerInfo structure to f/w
/// @param -u32addr \b IN : start address (units in byte)
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetScalerInfoAddr(MS_U8 u8Idx, MS_VIRT u32addr,MS_U8 u8Arg4)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    mvdcmd.Arg4 = u8Arg4;
    SET_CMD_RET_FALSE(CMD_SCALER_INFO_BASE, &mvdcmd);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set the dynamic scale base address
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetDynamicScaleAddr(MS_U8 u8Idx, MS_VIRT u32addr)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    SET_CMD_RET_FALSE(CMD_DYNAMIC_SCALE_BASE, &mvdcmd);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set virtual box width/height to F/W.
/// F/W will use the same w/h as scaler to calculate scaling factor.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetVirtualBox(MS_U8 u8Idx, MS_U16 u16Width, MS_U16 u16Height)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u16Width);
    mvdcmd.Arg1 = H_WORD(u16Width);
    mvdcmd.Arg2 = L_WORD(u16Height);
    mvdcmd.Arg3 = H_WORD(u16Height);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_DS_VIRTUAL_BOX, &mvdcmd);
    return TRUE;
}

//------------------------------------------------------------------------------
/// Enable VC1 dynamic scaling
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_EnableDynamicScale(MS_U8 u8Idx,MS_U8 u8NewDS)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = TRUE;
    mvdcmd.Arg1 = u8NewDS;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_ENABLE_DYNAMIC_SCALE, &mvdcmd);
    return TRUE;
}


//------------------------------------------------------------------------------
/// Set blue screen
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetBlueScreen(MS_U8 u8Idx, MS_BOOL bEn)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEn; //1 -> show MVOP frame color.  0 -> normal case.
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FORCE_BLUE_SCREEN, &mvdcmd);
    return TRUE;
}


MS_BOOL HAL_MVD_SetFreezeDisp(MS_U8 u8Idx, MS_BOOL bEn)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEn;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FREEZE_DISP, &mvdcmd);
    return TRUE;
}


//------------------------------------------------------------------------------
/// Set base address for DecFrameInfo structure to f/w
/// @param -u32addr \b IN : start address (units in byte)
//------------------------------------------------------------------------------
void HAL_MVD_SetDecFrmInfoAddr(MS_U8 u8Idx, MS_VIRT u32addr)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32addr%8)==0);
    u32addr >>= 3;

    SET_CMDARG(mvdcmd, u32addr, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DEC_FRAME_INFO_BUF, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DEC_FRAME_INFO_BUF ) );
    }
    return;
}

//Check if the task has interrupt
MS_BOOL HAL_MVD_GetHasInt(MS_U8 u8Idx)
{
    MS_BOOL bHasInt = FALSE;
    MS_U32 u32IntCnt = 0;
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    u32IntCnt = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_INT_CNT, sizeof(MS_U32));
    if (u32IntCnt != pCtrlCfg->u32IntCnt)
    {
        MVD_DEBUGINFO(VPRINTF("%s %d--> %d\n", __FUNCTION__, pCtrlCfg->u32IntCnt, u32IntCnt));
        bHasInt = TRUE;
        pCtrlCfg->u32IntCnt = u32IntCnt;
    }
    return bHasInt;
}

#ifdef VDEC3
MS_U32 HAL_MVD_GetIntState(MS_U8 u8Idx)
{
    MS_U32 u32IntStat;

    u32IntStat = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_INT_STAT, sizeof(MS_U32));

    MVD_SetFWBuffData(u8Idx,FW_BUFF_FRMINFO,OFFSET_INT_STAT,sizeof(MS_U32),0);

    return u32IntStat;
}
#else
MS_U32 HAL_MVD_GetIntState(MS_U8 u8Idx)
{
    MS_U32 u32IntStat = 0;
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_GET_INT_STAT, &mvdcmd ) == TRUE)
    {
        u32IntStat = (((MS_U32)mvdcmd.Arg4) << 16) |
                     (((MS_U32)mvdcmd.Arg3) << 8) |
                     (((MS_U32)mvdcmd.Arg2));
    }
    else
    {
        if (FALSE == pMVDHalContext->bStopped[u8Idx])
        {
            MVD_DEBUGERROR(VPRINTF("Ctrl: 0x%x fail!!\n", CMD_GET_INT_STAT));
        }
        else
        {
            MVD_DEBUGINFO(VPRINTF("Cmd 0x%x normal timeout.\n", CMD_GET_INT_STAT));
        }
    }

    return u32IntStat;
}

#endif

///// functions to check interrupt status /////
MS_BOOL MVD_IntHasUsrDataDisp(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_USER_DATA_DISP)==INT_USER_DATA_DISP) ? TRUE : FALSE);
}

MS_BOOL MVD_IntHasUsrData(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_USER_DATA)==INT_USER_DATA) ? TRUE : FALSE);
}

MS_BOOL MVD_IntIsDispRdy(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DISP_RDY)==INT_DISP_RDY) ? TRUE : FALSE);
}

MS_BOOL MVD_IntHasSeqHdr(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_SEQ_FOUND)==INT_SEQ_FOUND) ? TRUE : FALSE);
}

MS_BOOL MVD_IntHasESDataInvalid(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_VES_VALID)==INT_VES_VALID) ? TRUE : FALSE);
}

MS_BOOL MVD_IntHasDecodeErr(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DEC_ERR)==INT_DEC_ERR) ? TRUE : FALSE);
}
//INT_FIRST_FRAME means "1st frame be push to display queue & ready for display"
//So, (1) in IPB or IP stream, that's I-frame
//    (2) in PB only stream, that's first P-frame
MS_BOOL MVD_IntHas1stFrame(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_FIRST_FRAME)==INT_FIRST_FRAME) ? TRUE : FALSE);
}

//INT_XC_LOW_DELAY
MS_BOOL MVD_IntHasXcLowDelay(MS_U32 u32IntStat)
{
    //VPRINTF("MVD_IntHasXcLowDelay=%x \n ", u32IntStat);
    return (((u32IntStat&INT_XC_LOW_DELAY)==INT_XC_LOW_DELAY) ? TRUE : FALSE);
}

MS_BOOL MVD_IntHasDecodeIframe(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DEC_I)==INT_DEC_I) ? TRUE : FALSE);
}


MS_BOOL MVD_IntVSyncInt(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DISP_VSYNC)==INT_DISP_VSYNC) ? TRUE : FALSE);
}

MS_BOOL MVD_IntDecOneFrmInt(MS_U32 u32IntStat)
{
    return (((u32IntStat&INT_DEC_DONE)==INT_DEC_DONE) ? TRUE : FALSE);
}


static MS_U8 MVD_GetSyncStat(MS_U8 u8Idx, MS_U8 u8ArgIdx)
{
    MS_U8 u8Val = 0xFF;
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_GET_SYNC_STAT, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR(VPRINTF("Ctrl: 0x%x fail!!\r\n", CMD_GET_SYNC_STAT));
        return 0xFF;
    }
    MVD_DEBUGINFO(VPRINTF("Sync On/Off %x, Done %x, Stat %x, diff=%d\n",
                  mvdcmd.Arg0, mvdcmd.Arg1, mvdcmd.Arg2, (int)HAL_MVD_GetPtsStcDiff(u8Idx)));

    if (0==u8ArgIdx)
    {
        u8Val = mvdcmd.Arg0; //On/Off
    }
    else if (1==u8ArgIdx)
    {
        u8Val = mvdcmd.Arg1; //Done
    }
    else if (2==u8ArgIdx)
    {
        u8Val = mvdcmd.Arg2; //Stat
    }
    return u8Val;
}

//------------------------------------------------------------------------------
/// Report avsync status
/// avsync_status=avsync_done|(avsync_skip_picture<<1)|(avsync_repeat_picture<<2),
/// 0 for free run
//------------------------------------------------------------------------------
MS_U8 HAL_MVD_GetAVSyncStatus(MS_U8 u8Idx)
{
    return MVD_GetSyncStat(u8Idx, 2);
}

void HAL_MVD_SlqTblLoadWrPtr(MS_U8 u8Idx, MS_VIRT u32WrPtr)
{
#if 1
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MVD_CmdArg mvdcmd;
    MS_U16 u16Val = 0;

    if (FALSE == pstSlqTblInfo->bEnSlqTblHkCtrl)
    {
        SETUP_CMDARG(mvdcmd);
        mvdcmd.Arg0 = 0x10;
        mvdcmd.Arg1 = 0x01;
        SET_DECNUM(mvdcmd, u8Idx);
        if (HAL_MVD_MVDCommand( CMD_RD_IO, &mvdcmd ) == FALSE)
        {
            MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_RD_IO ) );
            return;
        }
        u16Val = (((MS_U32)mvdcmd.Arg2)) | ((MS_U32)mvdcmd.Arg3 << 8);

        SETUP_CMDARG(mvdcmd);
        mvdcmd.Arg0 = 0x10;
        mvdcmd.Arg1 = 0x01;
        mvdcmd.Arg2 = u16Val & 0xff;
        mvdcmd.Arg3 = ((u16Val>>8 ) & 0xff) | 0x80;
        SET_DECNUM(mvdcmd, u8Idx);
        if (HAL_MVD_MVDCommand(CMD_WR_IO, &mvdcmd) == FALSE)
        {
            MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_WR_IO ) );
            return;
        }

        pstSlqTblInfo->bEnSlqTblHkCtrl = TRUE;
        //VPRINTF("@@@ OPEN HK.SLQ.CTRL!\n");
    }
#endif
    OSAL_MVD_LockHwMutex();

    HAL_MVD_RegWriteByte(MVD_SLQ_WADR0, (u32WrPtr & 0xff));
    HAL_MVD_RegWriteByte(MVD_SLQ_WADR1, ((u32WrPtr>>8 ) & 0xff));
    HAL_MVD_RegWriteByte(MVD_SLQ_WADR2, ((u32WrPtr>>16) & 0xff));
    HAL_MVD_RegWriteByte(MVD_SLQ_WADR3, ((u32WrPtr>>24) & 0x01));
    HAL_MVD_RegWriteBit(MVD_SLQCTRL, 1, MVD_SLQCTRL_WADR_RELOAD);
    HAL_MVD_RegWriteBit(MVD_SLQCTRL, 0, MVD_SLQCTRL_WADR_RELOAD);

    OSAL_MVD_UnlockHwMutex();
}

static MS_VIRT HAL_MVD_SlqTblProbe(MVD_HKSLQ_CMD eCmd)
{
    MS_VIRT u32Cadr = 0;
    OSAL_MVD_LockHwMutex();

    switch (eCmd)
    {
        case MVD_HKSLQ_GET_READPTR:
            HAL_MVD_RegWriteBit(MVD_SLQCTRL, 1, MVD_SLQCTRL_RADR_PROBE);
            HAL_MVD_RegWriteBit(MVD_SLQCTRL, 0, MVD_SLQCTRL_RADR_PROBE);
            break;
        case MVD_HKSLQ_GET_WRITEPTR:
            HAL_MVD_RegWriteBit(MVD_SLQCTRL, 1, MVD_SLQCTRL_WADR_PROBE);
            HAL_MVD_RegWriteBit(MVD_SLQCTRL, 0, MVD_SLQCTRL_WADR_PROBE);
            break;
        default:
            break;
    }
    u32Cadr = HAL_MVD_RegReadByte(MVD_SLQ_CADR0) |
             (HAL_MVD_RegReadByte(MVD_SLQ_CADR1) << 8) |
             (HAL_MVD_RegReadByte(MVD_SLQ_CADR2) <<16) |
             ((HAL_MVD_RegReadByte(MVD_SLQ_CADR3) & 0x01) <<24);

    OSAL_MVD_UnlockHwMutex();
    return u32Cadr;
}

MS_VIRT HAL_MVD_SlqTblProbeWrPtr(MS_U8 u8Idx)
{
    return HAL_MVD_SlqTblProbe(MVD_HKSLQ_GET_WRITEPTR);
}

MS_VIRT HAL_MVD_SlqTblProbeRdPtr(MS_U8 u8Idx)
{
    return HAL_MVD_SlqTblProbe(MVD_HKSLQ_GET_READPTR);
}

//------------------------------------------------------------------------------
/// Set firmware as MStreamer mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetMStreamerMode(MS_U8 u8Idx, MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode; //1: enable, 0:disable MStreamer mode.
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SET_MST_MODE, &mvdcmd);

    pstCtrlCfg->u8MstMode = u8Mode;
    return TRUE;
}

//------------------------------------------------------------------------------
/// Set firmware as MStreamer mode (IP clip mode)
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_ShowDecodeOrder(MS_U8 u8Idx, MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = pstCtrlCfg->u8MstMode;
    mvdcmd.Arg1 = u8Mode;//a u8Mode; //1: clip mode, 0:normal mode.
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SET_MST_MODE, &mvdcmd);
    return TRUE;

}

MS_BOOL HAL_MVD_IsMStreamerMode(MS_U8 u8Idx)
{
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    return (MST_MODE_TRUE == pstCtrlCfg->u8MstMode);
}

MS_BOOL HAL_MVD_FrameOpt(MS_U8 u8Idx, MS_U8 u8FrmIdx, MVD_FrmOpt eOpt)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8FrmIdx;
    mvdcmd.Arg1 = eOpt; // 0 = Flip,  1 = Release.
    MVD_DEBUGINFO(VPRINTF("FLIP_RELEASE_FRAME: idx=0x%x, opt=0x%x\n", u8FrmIdx, eOpt));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FLIP_RELEASE_FRAME, &mvdcmd);

    return TRUE;
}

//------------------------------------------------------------------------------
/// Set firmware dynamic allocate FrameBuffer, now support MCU mode only
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL _MVD_SetDynamicAllocateFB(MS_U8 u8Idx,MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1-bEnable; //Arg0 :  0(enable) , 1(disable)
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_FIXED_FRAME_BUFFER, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FIXED_FRAME_BUFFER ) );
        return FALSE;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// Set firmware as MCU mode
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SetMcuMode(MS_U8 u8Idx, MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode; //1: enable, 0:disable MCU
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SET_MCU_MODE, &mvdcmd);

    pstCtrlCfg->u8McuMode = u8Mode;

    return TRUE;
}

MS_BOOL HAL_MVD_IsMcuMode(MS_U8 u8Idx)
{
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    return (TRUE == pstCtrlCfg->u8McuMode);
}

//------------------------------------------------------------------------------
/// Inform firwmare that PTS is updated.
/// @return -TRUE for success; FALSE for failure.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_UpdatePts(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SEND_UNI_PTS, &mvdcmd);

    return TRUE;
}


MS_BOOL HAL_MVD_FrameCapture(MS_U8 u8Idx, MS_U8 u8FrmIdx, MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;  //0 or 1 to enable/disable the freeze function
    mvdcmd.Arg2 = u8FrmIdx; //specify the freezed frame index
    MVD_DEBUGINFO(VPRINTF("CAPTURE_FRAME: idx=0x%x, enable=0x%x\n", u8FrmIdx, bEnable));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_ENABLE_FREEZE_PIC, &mvdcmd);

    return TRUE;
}

//------------------------------------------------------------------------------
/// Set HW buffers' start address to MVD FW
/// Return (the end address - 1)
/// @param -u32Addr \b IN : start address (MIU offset)
//------------------------------------------------------------------------------
MS_VIRT HAL_MVD_SetHWBuffer(MS_U8 u8Idx, MS_VIRT u32Add)
{
    MS_VIRT tmpAdr;

    MVD_DEBUGINFO(VPRINTF("====> %s u32Add = 0x%lx\n", __FUNCTION__, (unsigned long)u32Add));
    tmpAdr = (MemAlign(u32Add, MVD_FW_IAP_BUF_ALIGN));
    MVD_DEBUGINFO(VPRINTF("set MVD3_FW_IAP_BUF_ADR =%lx\n",(unsigned long)tmpAdr));
    HAL_MVD_SetIAPBufferAddr(u8Idx, tmpAdr);
    tmpAdr += MVD_FW_IAP_BUF_LEN;

    tmpAdr = (MemAlign(tmpAdr, MVD_FW_DP_BUF_ALIGN));
    MVD_DEBUGINFO(VPRINTF("set MVD3_FW_DP_BUF_ADR=%lx\n",(unsigned long)tmpAdr));
    HAL_MVD_SetDPBufferAddr(u8Idx, tmpAdr);
    tmpAdr += MVD_FW_DP_BUF_LEN;

    tmpAdr = (MemAlign(tmpAdr, MVD_FW_MV_BUF_ALIGN));
    MVD_DEBUGINFO(VPRINTF("set MVD3_FW_MV_BUF_ADR=%lx\n",(unsigned long)tmpAdr));
    HAL_MVD_SetMVBufferAddr(u8Idx, tmpAdr);
    tmpAdr += MVD_FW_MV_BUF_LEN;
    MVD_DEBUGINFO(VPRINTF("====> %s End of HW buffers = 0x%lx\n", __FUNCTION__, (unsigned long)tmpAdr));

    return tmpAdr;
}

//------------------------------------------------------------------------------
/// Set the number of framebuffer
/// @param -u8FrmNum \b IN : the number of framebuffer
//------------------------------------------------------------------------------
void HAL_MVD_SetFrameBuffNum(MS_U8 u8Idx, MS_U8 u8FrmNum,MS_U32 u32FBUsedSize)
{
#define MVD_FBNUM_DEFAULT 4
#define MVD_FBNUM_MIN     MVD_FBNUM_DEFAULT

    MVD_CmdArg mvdcmd;

    MVD_DEBUGINFO(VPRINTF("%s u8FrmNum = %d\n", __FUNCTION__, u8FrmNum));
    if (u8FrmNum < MVD_FBNUM_MIN)
    {
        MVD_DEBUGINFO(VPRINTF("%s set u8FrmNum as %d instead of %d!\n",
            __FUNCTION__, MVD_FBNUM_MIN, u8FrmNum));
        u8FrmNum = MVD_FBNUM_MIN;
    }
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8FrmNum;
    mvdcmd.Arg1 = u32FBUsedSize&0xff;
    mvdcmd.Arg2 = (u32FBUsedSize>>8)&0xff;
    mvdcmd.Arg3 = (u32FBUsedSize>>16)&0xff;
    mvdcmd.Arg4 = (u32FBUsedSize>>24)&0xff;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_FB_NUM, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FB_NUM ) );
        return;
    }
    return;
}

static MS_BOOL MVD_WriteFWBuffData(MS_U8 u8Idx, FW_BUFF_TYPE eBuffType, MS_U8 u8Offset, MS_U8 u8Size, MS_U32 u32Val)
{
    MS_BOOL bRet = FALSE;
    MS_VIRT u32BufStart = NULL;
    switch (eBuffType)
    {
        case FW_BUFF_VOLINFO:    u32BufStart = GET_VOL_BUFFADD(u8Idx);        break;
        case FW_BUFF_FRMINFO:    u32BufStart = GET_FRMINFO_BUFFADD(u8Idx);    break;
        case FW_BUFF_HDR:        u32BufStart = GET_HDR_BUFFADD(u8Idx);        break;
        case FW_BUFF_USRDATA:    u32BufStart = GET_USRDATA_BUFFADD(u8Idx);    break;
        case FW_BUFF_FWSLQTAB:   u32BufStart = GET_SLQ_BUFFADD(u8Idx);        break;
        case FW_BUFF_PTSTBL:     u32BufStart = GET_PTSTBL_BUFFADD(u8Idx);     break;
        case FW_BUFF_DS:         u32BufStart = GET_DS_BUFFADD(u8Idx);         break;
        case FW_BUFF_XCINFO:     u32BufStart = GET_XCINFO_BUFFADD(u8Idx);     break;
        case FW_BUFF_DECFRMINFO: u32BufStart = GET_DECFRMINFO_BUFFADD(u8Idx); break;
        default:
            break;
    }
    if (NULL == u32BufStart)
    {
       MVD_DEBUGERROR( VPRINTF("%s err: u8Idx=0x%x, bufType=0x%x, offset=%d, size=%d\n",
               __FUNCTION__, u8Idx, eBuffType, u8Offset, u8Size));
        return bRet;
    }

    if (u8Size == sizeof(MS_U8))
    {
        bRet = HAL_MVD_MemWriteByte(u32BufStart+u8Offset, (MS_U8)u32Val);
    }
    else if (u8Size == sizeof(MS_U32))
    {
        bRet = HAL_MVD_MemWrite4Byte(u32BufStart+u8Offset, u32Val);
    }

    MVD_DEBUGINFO(VPRINTF("%s: u32Val=%d for u8Idx=0x%x, bufType=0x%x, offset=%d, size=%d\n",
                          __FUNCTION__, u32Val, u8Idx, eBuffType, u8Offset, u8Size));
    return bRet;
}

//================== Vol Info ==================//
MS_U32 HAL_MVD_GetBitsRate(MS_U8 u8Idx)
{
    //Ref 13818-2, this flag is unit of 400 bits/sec
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_BIT_RATE, sizeof(MS_U32))*400;
}

MS_U8 HAL_MVD_GetVideoRange(MS_U8 u8Idx)
{
    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_VIDEO_RANGE, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_GetLowDelayFlag(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_LOW_DELAY, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_GetIs32PullDown(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_MPEG_FRC_MODE, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_GetIsDynScalingEnabled(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_DS_ENABLE, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_Is1stFrmRdy(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_FIRST_DISPLAY, sizeof(MS_U8));
}


//================== FrameInfo ==================//
MS_U32 HAL_MVD_GetPicCounter(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_FRAME_COUNT, sizeof(MS_U32));
}

MS_U32 HAL_MVD_GetSkipPicCounter(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SKIP_FRAME_COUNT, sizeof(MS_U32));
}

MVD_PicType HAL_MVD_GetPicType(MS_U8 u8Idx)
{
    MS_U32 picType = 0xff;
    MVD_PicType ret = E_MVD_PIC_UNKNOWN;

    if (GET_FRMINFO_BUFFADD(u8Idx)==0)
    {
        MVD_DEBUGERROR(VPRINTF("%s error: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return E_MVD_PIC_UNKNOWN;
    }
    picType = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PICTURE_TYPE, sizeof(MS_U32));
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

MS_S32 HAL_MVD_GetPtsStcDiff(MS_U8 u8Idx)
{
    return (MS_S32)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PTS_STC, sizeof(MS_S32));
}

MS_U8 HAL_MVD_GetDecodedFrameIdx(MS_U8 u8Idx)
{
    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_FB_INDEX, sizeof(MS_U8));
}

MS_U32 HAL_MVD_GetVldErrCount(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_VLD_ERR_COUNT, sizeof(MS_U32));
}

MS_BOOL HAL_MVD_GetValidStreamFlag(MS_U8 u8Idx)
{
    return !(MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_INVALIDSTREAM, sizeof(MS_U8));
}

MS_U8 HAL_MVD_GetIsIPicFound(MS_U8 u8Idx)
{
    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_FIRST_I_FOUND, sizeof(MS_U8));
}

//------------------------------------------------------------------------------
/// Set PTS base to MVD F/W
/// @param -u32pts \b IN : pts unit in 90k counter
//------------------------------------------------------------------------------
void HAL_MVD_SetPTSBase(MS_U8 u8Idx, MS_U32 u32pts)
{
    MVD_CmdArg mvdcmd;

    SET_CMDARG(mvdcmd, u32pts, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_PTS_BASE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_PTS_BASE ) );
    }
    return;
}

MS_U32 HAL_MVD_GetPTS(MS_U8 u8Idx)
{
    MS_U32 u32PTS = 0;

    u32PTS = (MS_U32)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISP_PTS, sizeof(MS_U32));
    u32PTS = _90K_TO_MS(u32PTS);
    return u32PTS;
}

MS_U64 HAL_MVD_GetU64PTS(MS_U8 u8Idx,MVD_PtsType eType)
{
    MS_U64 u64PTS = 0;
    MS_U64 u32_high32_PTS = 0;
    MS_U64 u32_low32_PTS = 0;

    if(eType==E_MVD_PTS_DISP)
    {
    u32_low32_PTS = (MS_U64)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISP_PTS, sizeof(MS_U32));
    u32_high32_PTS = (MS_U64)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISP_PTS_MSB, sizeof(MS_U32));
    }
    else if(eType==E_MVD_PTS_PRE_PAS)
    {
        u32_low32_PTS = (MS_U64)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_WRPTR_PTS_LOW, sizeof(MS_U32));
        u32_high32_PTS = (MS_U64)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_WRPTR_PTS_HIGH, sizeof(MS_U32));
    }

    u64PTS = (u32_high32_PTS<<32)|u32_low32_PTS;
    u64PTS = _90K_TO_MS_U64(u64PTS);
    return u64PTS;
}

MS_U32 HAL_MVD_GetNextPTS(MS_U8 u8Idx)
{
    return (MS_U32)_90K_TO_MS(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_NEXT_PTS, sizeof(MS_U32)));
}

MS_U32 HAL_MVD_GetChromaFormat(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CHROMA_FORMAT, sizeof(MS_U8));
}

MS_U32 HAL_MVD_GetGOPCount(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_GOP_I_FCNT, sizeof(MS_U32))
         + MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_GOP_P_FCNT, sizeof(MS_U32))
         + MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_GOP_B_FCNT, sizeof(MS_U32));
}

MS_U8 HAL_MVD_GetColorFormat(MS_U8 u8Idx)
{
    if (GET_FRMINFO_BUFFADD(u8Idx)==0)
    {
        MVD_DEBUGERROR(VPRINTF("%s error: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return 0xff;
    }

    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_VIDEO_FORMAT, sizeof(MS_U8));
}

MS_U8 HAL_MVD_GetMatrixCoef(MS_U8 u8Idx)
{
    if (GET_FRMINFO_BUFFADD(u8Idx)==0)
    {
        MVD_DEBUGERROR(VPRINTF("%s error: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return 0xff;
    }

    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_MATRIX_COEF, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_StepDecode(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SINGLE_STEP, &mvdcmd);

    if (HAL_MVD_Resume(u8Idx) == FALSE)
    {
        MVD_DEBUGERROR(VPRINTF("Command: HAL_MVD_Resume fail!!\r\n"));
        return FALSE;
    }

    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    if (pCtrlCfg)
    {
        pCtrlCfg->bStepDecode = TRUE;
    }

    return TRUE;
}

MS_BOOL HAL_MVD_IsStepDispDone(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_STEP_DISP_DONE, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_IsStep2PtsDone(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_STEP_TO_PTS_DONE, sizeof(MS_U8));
}

//Only for RCV.
MS_U32 HAL_MVD_GetPayloadLen(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_RCV_PAYLOAD_LENGTH, sizeof(MS_U32));
}

//Only for RCV.
MS_BOOL HAL_MVD_GotFileEndPattern(MS_U8 u8Idx)
{
    return (MS_BOOL)MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_MEET_FILE_END_SC, sizeof(MS_U8));
}

MS_BOOL HAL_MVD_IsCmdFinished(MS_U8 u8Idx, MVD_HANDSHAKE_CMD eCmd)
{
    MVD_CMD_HANDSHADE_INDEX u32CmdState;
    MS_BOOL bCmdRdy = FALSE;

    HAL_MVD_InvalidBuffRetFalse(GET_FRMINFO_BUFFADD(u8Idx));

    u32CmdState.value = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CMD_HANDSHAKE_INDEX, sizeof(MS_U32));
    MVD_DEBUGINFO(VPRINTF("u32CmdState.value = 0x%x\n", u32CmdState.value));
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
        case MVD_HANDSHAKE_GET_NXTDISPFRM:
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
        case MVD_HANDSHAKE_VIRTUAL_COMMAND:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_virtualCommand);
            break;
        case MVD_HANDSHAKE_FLUSHQUEUE_COMMAND:
            bCmdRdy = (MS_BOOL)(u32CmdState.mvdcmd_handshake_flush);
            break;
        default:
            bCmdRdy = FALSE;
            break;
    }
    return bCmdRdy;
}

MS_BOOL HAL_MVD_ClearCmdFinished(MS_U8 u8Idx, MVD_HANDSHAKE_CMD eCmd)
{
    MVD_CMD_HANDSHADE_INDEX u32CmdState;

    HAL_MVD_InvalidBuffRetFalse(GET_FRMINFO_BUFFADD(u8Idx));

    u32CmdState.value = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CMD_HANDSHAKE_INDEX, sizeof(MS_U32));
    MVD_DEBUGINFO(VPRINTF("before CLR u32CmdState.value = 0x%x\n", u32CmdState.value));
    switch (eCmd)
    {
        case MVD_HANDSHAKE_SCALER_INFO:
            u32CmdState.mvdcmd_handshake_scaler_data_ready = 0;
            break;
        default:
            break;
    }

    //write the value back: may it overwrite the value that f/w is supposed to write?
    MVD_WriteFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CMD_HANDSHAKE_INDEX, sizeof(MS_U32), u32CmdState.value);
    MVD_DEBUGINFO(VPRINTF("after CLR u32CmdState.value = 0x%x\n",
        MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CMD_HANDSHAKE_INDEX, sizeof(MS_U32))));
    return TRUE;
}


MS_BOOL HAL_MVD_GetTimeCode(MS_U8 u8Idx, MVD_FrmInfoType eType, MVD_TimeCode* pInfo)
{
    MS_BOOL bRet = FALSE;
    MS_VIRT pu8MVDGetFrameInfoBufStart = GET_FRMINFO_BUFFADD(u8Idx);
    if (NULL == pInfo)
    {
        return FALSE;
    }

    HAL_MVD_InvalidBuffRetFalse(pu8MVDGetFrameInfoBufStart);

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
        bRet = FALSE;
    }

    return bRet;
}

MS_U32 HAL_MVD_GetDispCnt(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISPLAYED_CNT, sizeof(MS_U32));
}

MS_U32 HAL_MVD_GetMinTspDataSize(MS_U8 u8HalIdx)
{
#ifdef OFFSET_CUR_MIN_TSP_DATA_SIZE
    return MVD_GetFWBuffData(u8HalIdx, FW_BUFF_FRMINFO, OFFSET_CUR_MIN_TSP_DATA_SIZE, sizeof(MS_U32));
#else
    return 0x400;
#endif
}

MS_U32 HAL_MVD_GetXcLowDelayIntState(MS_U8 u8Idx)
{
    MS_VIRT pu8MVDGetFrameInfoBufStart = GET_FRMINFO_BUFFADD(u8Idx);

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_GetXcLowDelayIntState error: pu8MVDGetFrameInfoBufStart=NULL\n"));
        return 0;
    }

    MVD_DEBUGINFO(VPRINTF("MDrv_MVD_GetXcLowDelayIntState()=%x,%x,%x,%x,%x\n",
        (HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_XC_LOW_DELAY_CNT)),
        (HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_XC_LOW_DELAY_INT_STATE)),
        (HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_XC_DIFF_FIELD_NO)),
        (HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_XC_LOW_DELAY_CNT_LATCH)),
        (HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_FRAME_COUNT))));

    return HAL_MVD_MemRead4Byte(pu8MVDGetFrameInfoBufStart+OFFSET_XC_LOW_DELAY_INT_STATE);
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

void HAL_MVD_GetErrInfo(MS_U8 u8Idx, MVD_ErrCode *errCode, MVD_ErrStatus *errStatus)
{
    MS_U32 errorCode = E_MVD_ERR_UNKNOWN;
    MS_U32 errorStatus = E_MVD_ERR_STATUS_UNKOWN;
    MS_VIRT pu8MVDGetFrameInfoBufStart = GET_FRMINFO_BUFFADD(u8Idx);

    if(pu8MVDGetFrameInfoBufStart==0)
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_GetErrInfo error!\n"));
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
        default:
            *errCode = E_MVD_ERR_UNKNOWN;
            *errStatus = E_MVD_ERR_STATUS_UNKOWN;
            break;
    }

    return;
}

//take care MIU1 address
MS_PHY HAL_MVD_GetMemOffset(MS_PHY u32PhyAdd)
{
    if (u32PhyAdd >= pMVDHalContext->stMiuCfg.u32Miu2BaseAddr)
    {
        return (u32PhyAdd - pMVDHalContext->stMiuCfg.u32Miu2BaseAddr);
    }
    else if(u32PhyAdd >= pMVDHalContext->stMiuCfg.u32Miu1BaseAddr)
    {
        return (u32PhyAdd - pMVDHalContext->stMiuCfg.u32Miu1BaseAddr);
    }
    else
    {
        return u32PhyAdd;
    }
}


MS_BOOL HAL_MVD_SLQTblSendPacket(MS_U8 u8Idx, MVD_PacketInfo *pstVideoPKT)
{
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_VIRT u32EsLast;
    MS_VIRT* u32LastEntry = &pstSlqTblInfo->pDrvSlqTbl->u32WrPtr;
    MS_VIRT* pu32EsNew = &pstSlqTblInfo->pDrvEsTbl->u32WrPtr;
    MS_U32 u32EntryWord = 0;
    MS_U32 u32Index = 0;
    MS_U32 u32Pts;
    MS_VIRT u32MVDFWPtsTblAddr = NULL;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    MS_U32 u32ESStart=0;
    MS_U32 u32ESEnd=0;
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);


#if _SLQTBL_DUMP_PKT
   // static MS_U32 u32SendTimes[MAX_DEC_NUM]= {0, 0};

    MVD_DEBUGINFO(VPRINTF("Pkt[%x] st=0x%x len=0x%x pts=0x%x id_l=0x%x id_h=0x%x\n", u32SendTimes[u8Idx]++,
        pstVideoPKT->u32StAddr, pstVideoPKT->u32Length, pstVideoPKT->u32TimeStamp,
        pstVideoPKT->u32ID_L, pstVideoPKT->u32ID_H));
#endif

#ifdef VDEC3
    if(pstCtrlCfg->u8McuMode == FALSE)
#endif
    {
    u32MVDFWPtsTblAddr = GET_PTSTBL_BUFFADD(u8Idx);
    MS_ASSERT(u32MVDFWPtsTblAddr != NULL);
    if (u32MVDFWPtsTblAddr)
    {
        MS_VIRT u32PtsTblEntryAddr = 0;
        //so far, this is the only place that driver will write f/w 1M area after init.
        u32Index = (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr - pstSlqTblInfo->pDrvSlqTbl->u32StAdd)/8;
        if(HAL_MVD_IsMcuMode(u8Idx) == FALSE)
        {
            if (pstVideoPKT->u32TimeStamp != MVD_NULLPKT_PTS)
            {
                u32Pts = _MS_TO_90K(pstVideoPKT->u32TimeStamp);
            }
            else
            {
                u32Pts = MVD_NULLPKT_PTS;
            }
        }
        else
        {
            u32Pts = pstVideoPKT->u32TimeStamp;
        }
        u32PtsTblEntryAddr = u32MVDFWPtsTblAddr+u32Index*MVD_FW_SLQTBL_PTS_LEN;
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+4, pstSlqTblInfo->u32DummyPktCnt);   //dummyPktCnt
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+8, pstVideoPKT->u32ID_L);  //idLow
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+12, pstVideoPKT->u32ID_H); //idHigh
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr+16, u32Pts);  //PTS
        HAL_MVD_MemWrite4Byte(u32PtsTblEntryAddr, (pstSlqTblInfo->u32SlqByteCnt)&0xffffff); //byteCnt
        //VPRINTF("PTS=0x%x(%x), idx=0x%x add=0x%x\n", pstVideoPKT->u32TimeStamp,
        //        HAL_MVD_MemRead4Byte(u32PtsTblEntryAddr+16),
        //        u32Index, u32PtsTblEntryAddr+16);

        if ((HAL_MVD_IsMStreamerMode(u8Idx) || HAL_MVD_IsMcuMode(u8Idx))
            && (pstCtrlCfg->eFileSyncMode == E_MVD_TIMESTAMP_STS))
        {
            if(pMVDHalContext->bDropOnePTS[u8Idx] == FALSE)
            {
                HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+12, pstVideoPKT->u32ID_H); //idHigh
                HAL_MVD_MemWrite4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+16, pstVideoPKT->u32TimeStamp);  //PTS

                MVD_DBG_STS(VPRINTF(">>> drvMVD pts,idH = %lu, %lu\n", HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+16),
                       HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_LEN+12)));
                if (((MVD_NULLPKT_PTS == pstVideoPKT->u32ID_H) && (MVD_NULLPKT_PTS == pstVideoPKT->u32TimeStamp)) == FALSE)
                {   //Only update PTS to firmware when pts field is valid.
                    //Plz refer to MDrv_MVD_PushQueue if (u8MstMode == MST_MODE_TRUE) {}
                        HAL_MVD_UpdatePts(u8Idx);    //for uniplayer or MCU mode
                }
            }

            pMVDHalContext->bDropOnePTS[u8Idx] = FALSE;
        }
    }
#if _SLQTBL_DUMP_PTS
    if (u32Index == 0x177)
    {
        _SLQTbl_DumpPtsTbl(u8Idx, 0, 0x179);
    }
#endif
    }
    pstSlqTblInfo->u32SlqByteCnt += pstVideoPKT->u32Length;

    //update SLQ tbl entry
    //u32EsLast = (pstVideoPKT->u32StAddr)+pstSlqTblInfo->pDrvSlqTbl->u32StAdd;
    u32EsLast = (pstVideoPKT->u32StAddr)+HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr);
    HAL_MVD_MemWrite4Byte(*pu32EsNew, u32EsLast-HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr));
    u32ESStart = (u32EsLast) & SLQ_TBL_ENTRY_LEN;

    u32EsLast += pstVideoPKT->u32Length; //update ES write pointer
    //Notice: This is for MVD HW, so no need to minus one.
    HAL_MVD_MemWrite4Byte((*pu32EsNew)+4, u32EsLast-HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr));
    u32ESEnd   = (u32EsLast) & SLQ_TBL_ENTRY_LEN;

    *pu32EsNew += 8;
    if (*pu32EsNew >= pstSlqTblInfo->pDrvEsTbl->u32EndAdd)
    {   //wrap to the beginning of the table
        MVD_DEBUGINFO(VPRINTF("...ES wrapping to the beginning!\n"));
        *pu32EsNew = pstSlqTblInfo->pDrvEsTbl->u32StAdd;
    }

    //VPRINTF("===>[%x] u32ESStart=0x%x u32ESEnd=0x%x u32EsLast=0x%x\n",
    //    pMVDHalContext->u32SendTimes[u8Idx]++, u32ESStart, u32ESEnd, u32EsLast);

    u32EntryWord = u32ESEnd;
    HAL_MVD_MemWrite4Byte(*u32LastEntry, u32EntryWord);
    //VPRINTF("===> u32EntryWord1 addr=0x%x\n", (*u32LastEntry));
    //VPRINTF("===> u32EntryWord0=0x%x\n", u32EntryWord);

    u32EntryWord = u32ESStart;
    HAL_MVD_MemWrite4Byte((*u32LastEntry)+4, u32EntryWord);
    //VPRINTF("===> u32EntryWord1 addr=0x%x\n", (*u32LastEntry)+4);
    //VPRINTF("===> u32EntryWord1=0x%x\n", u32EntryWord);

    *u32LastEntry += 8;
    if (*u32LastEntry >= pstSlqTblInfo->pDrvSlqTbl->u32EndAdd)
    {   //wrap to the beginning of the table
        MVD_DEBUGINFO(VPRINTF("...wrapping to the beginning!\n"));
        *u32LastEntry = pstSlqTblInfo->pDrvSlqTbl->u32StAdd;
        //also wrap DivX311 pattern table
        pstSlqTblInfo->pDrvDivxTbl->u32WrPtr = pstSlqTblInfo->pDrvDivxTbl->u32StAdd;
    }

    if (pstSlqTblInfo->pDrvSlqTbl->u32Empty)
    {
        pstSlqTblInfo->pDrvSlqTbl->u32Empty -= SLQ_ENTRY_LEN;
    }

    return TRUE;
}

//Table of frame rate code for MPEG-2
#if 0
#define FRM_RATE_CODE_NUM 9
static const MS_U16 stFrameRateCode[FRM_RATE_CODE_NUM]=
{
    NULL,23976,24000,25000,29976,30000,50000,59947,60000
};
#endif
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
void HAL_MVD_GetFrameInfo(MS_U8 u8Idx, MVD_FrameInfo *pinfo)
{
    FW_VOL_INFO gvolInfo;
    FW_DIVX_INFO* pDivxInfo = &(pMVDHalContext->gdivxInfo[u8Idx]);
    MS_VIRT temp = 0;
    MS_U32 u32DAR_Width=0,u32DAR_Height=0,u32PAR_Width=0,u32PAR_Height=0,u32GCD=0;
    MS_U32 u32Vertical_Size=0, u32Horizontal_Size=0;
    //13818-2 page 38 Table 6-3
    MS_U8 u8DARTable[5][2] = { {1,1}, {1,1}, {4,3}, {16,9}, {221,100} };
    MS_U32 pu8MVDGetVolBufStart = NULL;
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);

    pu8MVDGetVolBufStart = GET_VOL_BUFFADD(u8Idx);
    if (pu8MVDGetVolBufStart == NULL)
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_GetFrameInfo error: pu8MVDGetVolBufStart=NULL\n"));
        return;
    }

    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,GET_VOL_BUFFADD(u8Idx),temp);

    gvolInfo = (*(volatile FW_VOL_INFO*)(HAL_MVD_PA2NonCacheSeg(temp)));

    pinfo->u16HorSize = (MS_U16) gvolInfo.width;
    pinfo->u16VerSize = (MS_U16) gvolInfo.height;
    pinfo->u16par_width  = (MS_U16) gvolInfo.par_width;
    pinfo->u16par_height = (MS_U16) gvolInfo.par_height;
    pinfo->u8AspectRate = gvolInfo.aspect_ratio;

    pinfo->u16CropBottom = gvolInfo.CropBottom;
    pinfo->u16CropTop = 0;
    pinfo->u16CropLeft = 0;

    if (pinfo->u16HorSize & MVD_WIDTH_ALIGN_MASK)
    {
        //Notice: Firmware and driver have to be consistent for this part,
        // otherwise the pitch will not match and video is noisy.
        pinfo->u16CropRight = MVD_WIDTH_ALIGN_BYTE - (pinfo->u16HorSize & MVD_WIDTH_ALIGN_MASK);
        pinfo->u16HorSize = ((pinfo->u16HorSize >> MVD_WIDTH_ALIGN_BITS) + 1) << MVD_WIDTH_ALIGN_BITS;
    }
    else
    {
        pinfo->u16CropRight = 0;
    }

    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);
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
    if (pinfo->u16par_width == 0 || pinfo->u16par_height == 0)
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
                    MVD_DEBUGVERBAL(VPRINTF("u32PAR_Width:%d,u32PAR_Height:%d,GCD:%d\n",u32PAR_Width,u32PAR_Height,u32GCD));
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
        MVD_DEBUGVERBAL(VPRINTF("u32DAR_Width:%d,u32DAR_Height%d\n",u32DAR_Width,u32DAR_Height));
    }
    MVD_DEBUGVERBAL(VPRINTF("pinfo->u16par_width:%d, pinfo->u16par_height:%d\n",pinfo->u16par_width, pinfo->u16par_height));

    if (curCodecType == E_MVD_CODEC_MPEG2)
    {
        if (gvolInfo.frame_rate < FRM_RATE_CODE_NUM)
        {
            pinfo->u32FrameRate = stFrameRateCode[gvolInfo.frame_rate];
        }
        else
        {
            pinfo->u32FrameRate = 0;
        }
    }
    else if ((curCodecType == E_MVD_CODEC_VC1_ADV) || (curCodecType == E_MVD_CODEC_VC1_MAIN))
    {
        if ((gvolInfo.vc1_frame_rate != 0) && (gvolInfo.vc1_frame_rate != MVD_U32_MAX))
        {
            pinfo->u32FrameRate = gvolInfo.vc1_frame_rate;
        }
        else if(pMVDHalContext->u32DmxFrameRate[u8Idx] != 0 && pMVDHalContext->u32DmxFrameRateBase[u8Idx] != 0)
        {
            pinfo->u32FrameRate = (pMVDHalContext->u32DmxFrameRate[u8Idx] * 1000) / pMVDHalContext->u32DmxFrameRateBase[u8Idx];
        }
        else
        {
            pinfo->u32FrameRate = pDivxInfo->frame_rate; //report framerate specified in MDrv_MVD_SetFrameInfo()
        }

        MVD_DEBUGVERBAL(VPRINTF("MVD: vc1_frameRate=%d\n", pinfo->u32FrameRate));
    }
    else if (curCodecType == E_MVD_CODEC_MPEG4)
    {
        if (pDivxInfo->frame_rate != 0)
        {
            pinfo->u32FrameRate = pDivxInfo->frame_rate;
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
        else if(pMVDHalContext->u32DmxFrameRate[u8Idx] != 0 && pMVDHalContext->u32DmxFrameRateBase[u8Idx] != 0)
        {
            pinfo->u32FrameRate = (pMVDHalContext->u32DmxFrameRate[u8Idx] * 1000) / pMVDHalContext->u32DmxFrameRateBase[u8Idx];
        }
        else
        {
            pinfo->u32FrameRate = 60000; //set default frame rate according to chip capability
        }
        MVD_DEBUGVERBAL(VPRINTF("MVD: vol_time_incr_res=%d, fixed_vop_time_incr=%d\n", gvolInfo.vol_time_incr_res, gvolInfo.fixed_vop_time_incr));
    }
    else
    {
        if (gvolInfo.frame_rate != 0)
        {
            pinfo->u32FrameRate = gvolInfo.frame_rate;
        }
        else
        {
            pinfo->u32FrameRate = pDivxInfo->frame_rate; //report framerate specified in MDrv_MVD_SetFrameInfo()
        }
    }
    MVD_DEBUGVERBAL(VPRINTF("===> MVD: frameRate=%d  curCodecType=0x%x\n", pinfo->u32FrameRate, curCodecType));

    //for MM
    pinfo->u8MPEG1=gvolInfo.mpeg1;
    pinfo->u16PTSInterval=gvolInfo.pts_incr;
    pinfo->u8PlayMode=gvolInfo.play_mode;
    pinfo->u8FrcMode=gvolInfo.mpeg_frc_mode;

    //for dynamic scaling
    pinfo->bEnableMIUSel = (MS_BOOL)pMVDHalContext->stMiuCfg.u8FWMiuSel;

    if(pMVDHalContext->stMiuCfg.u8FWMiuSel >= E_CHIP_MIU_2)
        {
        MVD_DEBUGERROR(VPRINTF("u8MiuSel is %d !!, but return type is only Boolean. Please use MDrv_HVD_EX_GetDSBufMiuSelect() to get right miu select of DS buffer!\n",pMVDHalContext->stMiuCfg.u8FWMiuSel));
        }

    if (pstMemCfg->bEnableDynScale)
        {
        _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,GET_DS_BUFFADD(u8Idx),pinfo->u32DynScalingAddr);

        MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
        pinfo->u8DynScalingDepth= pCtrlCfg->u8DynScalingDepth;
        pinfo->u32DynScalingBufSize= gvolInfo.DSbufsize;
    }
    else
    {
        pinfo->u32DynScalingAddr= NULL;
        pinfo->u8DynScalingDepth= 0;
        pinfo->u32DynScalingBufSize= 0;
    }

    return;
}

E_MVD_Result MVD_RstFrmInfo(MS_U8 u8Idx, MVD_FrmInfoType eType)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MS_VIRT u32DecFrmInfoAdd = GET_DECFRMINFO_BUFFADD(u8Idx);
    if (NULL == u32DecFrmInfoAdd)
    {
        return E_MVD_RET_FAIL;
    }

    //VPRINTF("%s u32DecFrmInfoAdd = 0x%x\n", __FUNCTION__, u32DecFrmInfoAdd);
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

//Given the start address & the available size for the FW buffers,
//Calculate the start address of each buffers.
static MS_BOOL MVD_GetFWBuffAdd(MS_VIRT u32Start, MS_U32 u32AvailLen, MVD_FWBuff* pBuff)
{
    MS_VIRT tmpAdr = u32Start;

    if(pBuff == NULL)
    {
        MVD_DEBUGERROR(VPRINTF("%s err: NULL pBuff\n", __FUNCTION__));
        return FALSE;
    }
    tmpAdr += MVD_FW_CODE_LEN;
    GET_FW_BUFFADD_ALIGN(tmpAdr, FW_BUFF_ALIGN, MVD3_FW_VOL_INFO_BUF_LEN, pBuff->pu8MVDGetVolBufStart);
    GET_FW_BUFFADD_ALIGN(tmpAdr, FW_BUFF_ALIGN, MVD3_FW_FRAME_INFO_BUF_LEN, pBuff->pu8MVDGetFrameInfoBufStart);
    GET_FW_BUFFADD_ALIGN(tmpAdr, FW_BUFF_ALIGN, MVD3_FW_DIVX_INFO_BUF_LEN, pBuff->pu8MVDSetHeaderBufStart);
    GET_FW_BUFFADD_ALIGN(tmpAdr, FW_BUFF_ALIGN,
        (MVD3_FW_USER_DATA_BUF_LEN+MVD3_FW_USER_DATA_BUF_BACKUP_LEN), pBuff->u32UserDataBuf);
    //MVD3_FW_USER_DATA_BUF_BACKUP_LEN is used as CC decoding buffer for MVD_SUPPORT_X4_CC

    GET_FW_BUFFADD_ALIGN(tmpAdr, FW_BUFF_ALIGN, MVD3_FW_SLQ_TAB_TMPBUF_LEN, pBuff->u32MVDFWSLQTABTmpbufAdr);
    GET_FW_BUFFADD(tmpAdr, MVD_FW_SLQTBL_PTS_BUF_LEN, pBuff->u32MVDFWPtsTblAddr);
    GET_FW_BUFFADD(tmpAdr, MVD_FW_DYN_SCALE_BUF_LEN, pBuff->u32DynScalingAdd);
    GET_FW_BUFFADD(tmpAdr, MVD_FW_SCALER_INFO_BUF_LEN, pBuff->u32ScalerInfoAdd);
    GET_FW_BUFFADD(tmpAdr, MVD_FW_DECFRM_INFO_BUF_LEN, pBuff->u32DecFrmInfoAdd);
    GET_FW_BUFFADD(tmpAdr, MVD_FW_VBBU_TABLE_LEN, pBuff->u32VBBUTableAdd);


    MVD_DEBUGINFO(VPRINTF("set pu8MVDGetVolBufStart=%lx\n", (unsigned long)pBuff->pu8MVDGetVolBufStart));
    MVD_DEBUGINFO(VPRINTF("set pu8MVDGetFrameInfoBufStart=%lx\n", (unsigned long)pBuff->pu8MVDGetFrameInfoBufStart));
    MVD_DEBUGINFO(VPRINTF("set pu8MVDSetHeaderBufStart=%lx\n", (unsigned long)pBuff->pu8MVDSetHeaderBufStart));
    MVD_DEBUGINFO(VPRINTF("u32UserDataBuf start=%lx\n", (unsigned long)pBuff->u32UserDataBuf));
    MVD_DEBUGINFO(VPRINTF("u32MVDFWSLQTABTmpbufAdr start=%lx\n", (unsigned long)pBuff->u32MVDFWSLQTABTmpbufAdr));
    MVD_DEBUGINFO(VPRINTF("PtsTblAddr start=%lx\n", (unsigned long)pBuff->u32MVDFWPtsTblAddr));
    MVD_DEBUGINFO(VPRINTF("u32DynScalingAdd start=%lx\n", (unsigned long)pBuff->u32DynScalingAdd));
    MVD_DEBUGINFO(VPRINTF("ScalerInfo start=%lx end=%lx\n",
           (unsigned long)pBuff->u32ScalerInfoAdd, (pBuff->u32ScalerInfoAdd+MVD_FW_SCALER_INFO_BUF_LEN)));
    MVD_DEBUGINFO(VPRINTF("DecFrmInfo start=%lx end=%lx\n",
           (unsigned long)pBuff->u32DecFrmInfoAdd, (pBuff->u32DecFrmInfoAdd+MVD_FW_DECFRM_INFO_BUF_LEN)));

    MS_U32 u32ShMemBoundary = (MVD_FW_CODE_LEN==MVD_FW_CODE_LEN_V00) ? MVD_FW_MPOOL_START_OFFSET : MVD_FW_TASK_OFFSET;
#if v3_temp

    if ((pBuff->u32VBBUTableAdd+MVD_FW_VBBU_TABLE_LEN-u32Start) > u32ShMemBoundary)
    {
        //shared memory should not overlap with FW memory pool.
        MVD_DEBUGERROR(VPRINTF("%s err: 0x%lx out of memory boundary!\n", __FUNCTION__,
                       (pBuff->u32VBBUTableAdd+MVD_FW_VBBU_TABLE_LEN-u32Start)));
        return FALSE;
    }
#else
     if ((pBuff->u32DecFrmInfoAdd+MVD_FW_DECFRM_INFO_BUF_LEN-u32Start) > u32ShMemBoundary)
    {
        //shared memory should not overlap with FW memory pool.
        MVD_DEBUGERROR(VPRINTF("%s err: 0x%x out of memory boundary!\n", __FUNCTION__,
                       (pBuff->u32DecFrmInfoAdd+MVD_FW_DECFRM_INFO_BUF_LEN-u32Start)));
        return FALSE;
    }
#endif

    return TRUE;
}

//Set buffer address to f/w
//Init the memory erea if necessary
static MS_BOOL MVD_SetFWBuffAdd(MS_U8 u8Idx, MVD_FWBuff* pBuff)
{
    MS_U32 i;
    MVD_SrcMode curSrcMode = E_MVD_SRC_UNKNOWN;
    MVD_MEMCfg* pstMemCfg = NULL;
    MVD_CtrlCfg* pCtrlCfg = NULL;

    if(pBuff == NULL)
    {
        MVD_DEBUGERROR(VPRINTF("%s err: NULL pBuff\n", __FUNCTION__));
        return FALSE;
    }

    pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);

    MVD_DEBUGINFO(VPRINTF("\nMIU is (shm,hw,fw)=(%x,%x,%x)\n",u8SHMMiuSel,pstMemCfg->u8HWMiuSel,pstMemCfg->u8FWMiuSel));

    curSrcMode = HAL_MVD_GetSrcMode(u8Idx);

    HAL_MVD_SetVolInfoBufferAddr(u8Idx, pBuff->pu8MVDGetVolBufStart);
    _MVD_Memset(pBuff->pu8MVDGetVolBufStart, 0, MVD3_FW_VOL_INFO_BUF_LEN);

    pBuff->u32VolAdd = pBuff->pu8MVDGetVolBufStart;

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,pBuff->u32VolAdd,pBuff->u32VolAdd);

    pBuff->u32VolAdd = HAL_MVD_PA2NonCacheSeg(pBuff->u32VolAdd);
    MVD_DEBUGINFO(VPRINTF("gvolInfo = 0x%lx, volBuf=0x%lx\n", (unsigned long)pBuff->u32VolAdd, (unsigned long)pBuff->pu8MVDGetVolBufStart));

    HAL_MVD_SetFrameInfoBufferAddr(u8Idx, pBuff->pu8MVDGetFrameInfoBufStart);
    _MVD_Memset(pBuff->pu8MVDGetFrameInfoBufStart, 0, MVD3_FW_FRAME_INFO_BUF_LEN);

    HAL_MVD_SetHeaderBufferAddr(u8Idx, pBuff->pu8MVDSetHeaderBufStart);
    HAL_MVD_SetUserDataBuf(u8Idx, pBuff->u32UserDataBuf, MVD3_FW_USER_DATA_BUF_LEN);
    pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    pCtrlCfg->u32UsrDataRd = pBuff->u32UserDataBuf;


    if((curSrcMode != E_MVD_TS_FILE_MODE)
    && (curSrcMode != E_MVD_TS_MODE))
    {
        _MVD_Memset(pBuff->u32MVDFWSLQTABTmpbufAdr, 0, MVD3_FW_SLQ_TAB_TMPBUF_LEN);
        HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWSLQTABTmpbufAdr, 0xBE010000UL);
        HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWSLQTABTmpbufAdr+4, 0x000000FAUL);

        HAL_MVD_SetPtsTblAddr(u8Idx, pBuff->u32MVDFWPtsTblAddr);
        for (i=0; i<MVD_FW_SLQTBL_PTS_BUF_LEN; i+=MVD_FW_SLQTBL_PTS_LEN)
        {
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i, 0);     //byteCnt
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+4, 0);   //dummyPktCnt
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+8, 0);   //idLow
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+12, 0);  //idHigh

            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+16, MVD_NULLPKT_PTS);  //PTS
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+20, 0);  //reserved0
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+24, 0);  //reserved1
            HAL_MVD_MemWrite4Byte(pBuff->u32MVDFWPtsTblAddr+i+28, 0);  //reserved2
        }
        //MS_ASSERT((u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_BUF_LEN)<=(u32start+u32len));
        MVD_DEBUGINFO(VPRINTF("PTS tbl start=%lx end=%lx\n",
               (unsigned long)pBuff->u32MVDFWPtsTblAddr, (unsigned long)(pBuff->u32MVDFWPtsTblAddr+MVD_FW_SLQTBL_PTS_BUF_LEN)));

        pBuff->u32VBBUTableAdd = VBBU_TABLE_START+u8Idx*0x100000;
    }

    if (pstMemCfg->bEnableDynScale)
    {
        HAL_MVD_SetDynamicScaleAddr(u8Idx, pBuff->u32DynScalingAdd);
        HAL_MVD_EnableDynamicScale(u8Idx,0); //default old DS style
        pCtrlCfg->u8DynScalingDepth = 32; //HAL_MVD_GetDynamicScaleDepth
        MVD_DEBUGINFO(VPRINTF("u8EnableMIUSel    = 0x%x\n", pMVDHalContext->stMiuCfg.u8FWMiuSel));
        MVD_DEBUGINFO(VPRINTF("u32DynScalingAddr= 0x%lx\n", (unsigned long)pBuff->u32DynScalingAdd));
        MVD_DEBUGINFO(VPRINTF("u8DynScalingDepth= 0x%x\n", pCtrlCfg->u8DynScalingDepth));
    }
    MVD_DEBUGINFO(VPRINTF("DynScaling start=%lx end=%lx\n",
           (unsigned long)pBuff->u32DynScalingAdd, (unsigned long)(pBuff->u32DynScalingAdd+MVD_FW_DYN_SCALE_BUF_LEN)));

    HAL_MVD_SetDecFrmInfoAddr(u8Idx, pBuff->u32DecFrmInfoAdd);

    MVD_RstFrmInfo(u8Idx, E_MVD_FRMINFO_DECODE);
    MVD_RstFrmInfo(u8Idx, E_MVD_FRMINFO_DISPLAY);
    MVD_DEBUGINFO(VPRINTF("DecFrmInfo start=%lx\n", (unsigned long)pBuff->u32DecFrmInfoAdd));

    if (curSrcMode == E_MVD_SLQ_TBL_MODE)
    {
        HAL_MVD_SLQTblInit(u8Idx);
    }

    return TRUE;
}

#ifdef VDEC3
#else
static MS_BOOL MVD_IsNextDispFrmRdy(MS_U8 u8Idx)
{
    #define NXT_DISP_TIMEOUT 20000//0x20
    MS_U32 u32TimeOut = 0;
    MVD_CmdArg mvdcmd;

    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_GET_NXTDISPFRM);
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_GET_NEXTDISPFRM, &mvdcmd);

    while(!HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_GET_NXTDISPFRM) && (u32TimeOut < NXT_DISP_TIMEOUT))
    {
        u32TimeOut++;
    }
    if(u32TimeOut >= NXT_DISP_TIMEOUT)
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail!!\r\n", CMD_GET_NEXTDISPFRM ) );
        return FALSE;
    }
    return TRUE;
}
#endif

void HAL_MVD_GetExtDispInfo(MS_U8 u8Idx, MVD_ExtDispInfo* pInfo)
{
    pInfo->u16VSize  = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISP_V_SIZE, sizeof(MS_U16));
    pInfo->u16HSize  = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISP_H_SIZE, sizeof(MS_U16));
    pInfo->u16VOffset = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CENTRE_V_OFFSET, sizeof(MS_U16));
    pInfo->u16HOffset = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CENTRE_H_OFFSET, sizeof(MS_U16));
}

E_MVD_Result HAL_MVD_GetFrmInfo(MS_U8 u8Idx, MVD_FrmInfoType eType, MVD_FrmInfo* pInfo)
{
    MS_U32 u32Id = MVD_GetStreamId(u8Idx);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
    E_MVD_Result eRet = E_MVD_RET_OK;
    MS_VIRT u32DecFrmInfoAdd = GET_DECFRMINFO_BUFFADD(u8Idx);
#if v3_temp
    MVD_MEMCfg* pstMemCfg = NULL;
    pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
#endif
    if (NULL == pInfo)
    {
        MVD_DEBUGERROR(VPRINTF("GetFrmInfo NULL pInfo!\n"));
        return E_MVD_RET_INVALID_PARAM;
    }
    if (NULL == u32DecFrmInfoAdd)
    {
        MVD_DEBUGERROR(VPRINTF("GetFrmInfo NULL u32DecFrmInfoAdd!\n"));
        return E_MVD_RET_FAIL;
    }
    //VPRINTF("%s u32DecFrmInfoAdd = 0x%x\n", __FUNCTION__, u32DecFrmInfoAdd);

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
#if v3_temp
        DISPQ_IN_DRAM* pTemp = (DISPQ_IN_DRAM*)(MS_PA2KSEG1(pstMemCfg->u32FWCodeAddr+DISP_QUEUE+ u8VPUIdx*0x100000));
        if (pTemp->dispQ_rd == pTemp->dispQ_wr)
        {
            MVD_DEBUGINFO(VPRINTF("NextDispFrm not ready!\n"));
            return E_MVD_RET_FAIL;
        }
        pInfo->u16FrmIdx = pTemp->disp_info[pTemp->dispQ_rd].u16NextDispFrameIdx;

        pInfo->u32LumaAddr  = pTemp->disp_info[pTemp->dispQ_rd].u32NextDispLumaAddr;

        pInfo->u32ChromaAddr= pTemp->disp_info[pTemp->dispQ_rd].u32NextDispChromaAddr;
        // 64BIT_PTS = (TimeStamp | (ID_H<<32)) , unit: 90K
        pInfo->u32TimeStamp = pTemp->disp_info[pTemp->dispQ_rd].u32NextDispTimeStamp;

        pInfo->u32ID_H  = pTemp->disp_info[pTemp->dispQ_rd].u32NextDispID_H;

        pInfo->u16Width = pTemp->disp_info[pTemp->dispQ_rd].u16NextDispWidth;

        if(HAL_MVD_IsMcuMode(u8Idx))
        {
            //bit[19-20] Picture format,
            //bit[18-17] Disp times, 1~3
            //bit[16] Top field first , 0-> bottom first, 1-> top first
            //bit[15:8] Range reduction of luma data
            //bit[7:0] Range reduction of chroma data
            pInfo->u32ID_L = (MS_U32)pTemp->disp_info[pTemp->dispQ_rd].u8NextDispRangeRed_UV
                                             | (MS_U32)(pTemp->disp_info[pTemp->dispQ_rd].u8NextDispRangeRed_Y << 8)
                                             | (MS_U32)(pTemp->disp_info[pTemp->dispQ_rd].u16ExtData << 16)
                                             | (MS_U32)(pTemp->disp_info[pTemp->dispQ_rd].u8Progressive << 19);

            if (pInfo->u16Width & MVD_WIDTH_ALIGN_MASK)
            {
                pInfo->u16Width = ((pInfo->u16Width >> MVD_WIDTH_ALIGN_BITS) + 1) << MVD_WIDTH_ALIGN_BITS;
            }
        }
        else
        {
            pInfo->u32ID_L  = pTemp->disp_info[pTemp->dispQ_rd].u32DispID_L;
        }
        pInfo->u16Pitch = pTemp->disp_info[pTemp->dispQ_rd].u16NextDispPitch;
        pInfo->u16Height= pTemp->disp_info[pTemp->dispQ_rd].u16NextDispHeight;
        pInfo->eFrmType = pTemp->disp_info[pTemp->dispQ_rd].u16NextDispeFrameType;

        pTemp->bUsedByOutside[pInfo->u16FrmIdx]++;

        pTemp->dispQ_rd = (pTemp->dispQ_rd+1)%DISPQ_SIZE;
        MsOS_FlushMemory();
#else
        if (!MVD_IsNextDispFrmRdy(u8Idx))
        {
            MVD_DEBUGINFO(VPRINTF("NextDispFrm not ready!\n"));
            return E_MVD_RET_FAIL;
        }
        pInfo->u16FrmIdx= HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_FRAMEIDX);
        if (pInfo->u16FrmIdx  == 0xFFFF)
        {
            MVD_DEBUGINFO(VPRINTF("GetFrmInfo no available frame!\n"));
            return E_MVD_RET_FAIL;
        }
        pInfo->u32LumaAddr  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_LUMAADDR);
        pInfo->u32ChromaAddr= HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_CHROMAADDR);
        // 64BIT_PTS = (TimeStamp | (ID_H<<32)) , unit: 90K
        pInfo->u32TimeStamp = (HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_TIMESTAMP));
        pInfo->u32ID_H  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_ID_H);
        pInfo->u16Width = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_WIDTH);
        if(HAL_MVD_IsMcuMode(u8Idx))
        {
            //bit[18-17] Disp times, 1~3
            //bit[16] Top field first , 0-> bottom first, 1-> top first
            //bit[15:8] Range reduction of luma data
            //bit[7:0] Range reduction of chroma data
            pInfo->u32ID_L = (MS_U32)HAL_MVD_MemReadByte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_RANGERED_UV)
                                             | (MS_U32)(HAL_MVD_MemReadByte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_RANGERED_Y) << 8)
                                             | (MS_U32)(HAL_MVD_MemReadByte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_EXT_DATA) << 16);
            if (pInfo->u16Width & MVD_WIDTH_ALIGN_MASK)
            {
                pInfo->u16Width = ((pInfo->u16Width >> MVD_WIDTH_ALIGN_BITS) + 1) << MVD_WIDTH_ALIGN_BITS;
            }
        }
        else
        {
            pInfo->u32ID_L  = HAL_MVD_MemRead4Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_ID_L);
        }
        pInfo->u16Pitch = HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_PITCH);
        pInfo->u16Height= HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_HEIGHT);
        pInfo->eFrmType = (MVD_PicType)HAL_MVD_MemRead2Byte(u32DecFrmInfoAdd + OFFSET_DECFRAMEINFO_NEXTDISP_FRAMETYPE);
#endif
#if 0
    VPRINTF("NxtFrm:: Idx=0x%x, ", pInfo->u16FrmIdx);
    VPRINTF("Type=0x%x, ", pInfo->eFrmType      );
    VPRINTF("Luma=0x%x, ", pInfo->u32LumaAddr  );
    VPRINTF("Chroma=0x%x, ", pInfo->u32ChromaAddr);
    VPRINTF("Pts=%lu, ", pInfo->u32TimeStamp );
    VPRINTF("ID_H=%lu, ", pInfo->u32ID_H      );
    VPRINTF("ID_L=0x%x\n", pInfo->u32ID_L      );
#endif
        MVD_DBG_STS(VPRINTF("<<< drvMVD pts,idH = %lu, %lu\n", pInfo->u32TimeStamp, pInfo->u32ID_H));
    }
    else
    {
        eRet = E_MVD_RET_INVALID_PARAM;
    }

    if ((pInfo->u32LumaAddr  == _INIT_ADDR) || (pInfo->u32ChromaAddr== _INIT_ADDR) ||
        (pInfo->u16Pitch == _INIT_LEN) || (pInfo->u16Width == _INIT_LEN) ||
        (pInfo->u16Height== _INIT_LEN))
    {
        MVD_DEBUGINFO(VPRINTF("GetFrmInfo not ready!\n"));
        return E_MVD_RET_FAIL;
    }

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,(pInfo->u32LumaAddr * 8),pInfo->u32LumaAddr);
    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,(pInfo->u32ChromaAddr * 8),pInfo->u32ChromaAddr);

    //VPRINTF("===> Luma=0x%x, Chroma=0x%x\n", pInfo->u32LumaAddr, pInfo->u32ChromaAddr);
    return eRet;
}



E_MVD_Result HAL_MVD_SetDynScalingParam(MS_U8 u8Idx, MS_PHY u32StAddr, MS_SIZE u32Size)
{
    #define SCALER_INFO_TIMEOUT 0x1000
    MS_U32 u32TimeOut = 0;
    MS_VIRT u32SrcAdd = NULL;
    MS_U32 i;
    MS_VIRT u32ScalerInfoAdd = GET_XCINFO_BUFFADD(u8Idx);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);

    if ((u32StAddr==NULL) || (u32Size==0) || (u32Size>MVD_FW_SCALER_INFO_BUF_LEN))
    {
        MVD_DEBUGERROR(VPRINTF("%s invalid para u32StAddr=0x%lx, u32Size=0x%lx\n",
                       __FUNCTION__, (unsigned long)u32StAddr, (unsigned long)u32Size));
        return E_MVD_RET_INVALID_PARAM;
    }
    if (TRUE != pstMemCfg->bEnableDynScale)
    {
        MVD_DEBUGERROR(VPRINTF("%s !bEnableDynScale\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //copy data
    u32SrcAdd = HAL_MVD_PA2NonCacheSeg(u32StAddr);
    u32Size = ((u32Size+3)>>2)<<2;
    MVD_DEBUGINFO(VPRINTF("u32Size= 0x%lx, u32SrcAdd= 0x%lx\n", (unsigned long)u32Size, (unsigned long)u32SrcAdd));
    for (i=0; i<u32Size; i=i+4)
    {
        HAL_MVD_MemWrite4Byte(u32ScalerInfoAdd+i, *(volatile MS_U32*)(u32SrcAdd+i));
        MVD_DEBUGINFO(VPRINTF("0x%lx = 0x%x\n", (unsigned long)(u32ScalerInfoAdd+i), HAL_MVD_MemRead4Byte(u32ScalerInfoAdd+i)));
    }

    //notify f/w
    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_SCALER_INFO);
    if (TRUE!=HAL_MVD_SetScalerInfoAddr(u8Idx, u32ScalerInfoAdd,((MS_U8*)u32SrcAdd)[0])) //Set the buffer address (MIU offset) to f/w
    {
        MVD_DEBUGERROR(VPRINTF("%s fail to set ScalerInfoAdd\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //check f/w already handle the data
    while((TRUE!=HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_SCALER_INFO)) && (u32TimeOut < SCALER_INFO_TIMEOUT))
    {
        u32TimeOut++;
    }
    if(u32TimeOut >= SCALER_INFO_TIMEOUT)
    {
        MVD_DEBUGERROR(VPRINTF("%s timeout!!!\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //clear ack bit
    HAL_MVD_ClearCmdFinished(u8Idx, MVD_HANDSHAKE_SCALER_INFO);

    MVD_DEBUGINFO(VPRINTF("=====> %s u32TimeOut = 0x%x\n", __FUNCTION__, u32TimeOut));
    return E_MVD_RET_OK;
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


static MS_BOOL MVD_CheckFrmBuffSizeMin(MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg)
{
    MS_BOOL ret = TRUE;

    if (_IS_VC1(fwCfg->eCodecType))
    {
        MVD_DEBUGERROR(VPRINTF("Framebuffer size(0x%lx) < (0x%x+0x%lx)!\n",
            (unsigned long)memCfg->u32FBSize, (MVD_HD_FBSIZE*MVD_FBNUM_MIN), MVD_HW_BUF_TOTAL_LEN));
        ret = FALSE;
    }
    else if (memCfg->u32FBSize < (MVD4_MPEG_FBSIZE_SDMIN+MVD_HW_BUF_TOTAL_LEN))
    {
        MVD_DEBUGERROR(VPRINTF("Framebuffer size(0x%lx) < (0x%x+0x%lx)\n",
            (unsigned long)memCfg->u32FBSize, MVD4_MPEG_FBSIZE_SDMIN, MVD_HW_BUF_TOTAL_LEN));
        ret = FALSE;
    }
    return ret;
}

MS_U32 MVD_GetUsedFrmBuffSize(MS_U8 u8FBMode, MS_U8 u8FBNum)
{
    MS_U32 u32Size = 0;
    if (MVD3_DHD_MODE == u8FBMode)
    {
        u32Size = MVD_DHD_FBSIZE;
        if (MVD_FBNUM_MAX == u8FBNum)
        {
            u32Size *= MVD_FBNUM_MAX;
        }
        else if (MVD_FBNUM_MIN == u8FBNum)
        {
            u32Size *= MVD_FBNUM_MIN;
        }
    }
    else if (MVD3_HD_MODE == u8FBMode)
    {
        u32Size = MVD_HD_FBSIZE;
        if (MVD_FBNUM_MAX == u8FBNum)
        {
            u32Size *= MVD_FBNUM_MAX;
        }
        else if (MVD_FBNUM_MIN == u8FBNum)
        {
            u32Size *= MVD_FBNUM_MIN;
        }
    }
    else if (MVD3_SD_MODE == u8FBMode)
    {
        u32Size = MVD4_MPEG_FBSIZE_SDMIN;
    }
    return u32Size;
}

//------------------------------------------------------------------------------
/// Determine u8FBMode & u8FBNum according to the assigned FBSize
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_CheckFrmBuffSize(MS_U8 u8Idx, MVD_FWCfg* fwCfg, MVD_MEMCfg* memCfg)
{
    MS_BOOL ret = TRUE;
    MS_U8* pu8FBMode = &(fwCfg->u8FBMode);
    MS_U8* pu8FBNum  = &(fwCfg->u8FBNum);
    MS_U32 u32AvailFrmBuffSize = memCfg->u32FBSize - MVD_HW_BUF_TOTAL_LEN;

    ret = MDrv_MVD_AUTH_IPCheck(fwCfg->eCodecType,&(memCfg->bSupportSDModeOnly));
    if(ret == FALSE)
    {
        return FALSE;
    }

    *pu8FBMode = 0xff;

#if defined(MVD_SUPPORT_SD_ONLY)
    *pu8FBMode = MVD3_SD_MODE;
#else
    if(memCfg->bSupportSDModeOnly)
    {
        *pu8FBMode = MVD3_SD_MODE;
    }
#endif //MVD_SUPPORT_SD_ONLY

    //For SD only cases: defined(MVD_SUPPORT_SD_ONLY) and bSupportSDModeOnly
    if (MVD3_SD_MODE == *pu8FBMode)
    {
        ret = MVD_CheckFrmBuffSizeMin(fwCfg, memCfg);

        //set frmBuffNum as 4
        *pu8FBNum = MVD_FBNUM_MIN;
        MVD_DEBUGINFO(VPRINTF("[MVD_SD_MODE] u8FBNum=%d, FBSize=0x%lx\n", *pu8FBNum, (unsigned long)memCfg->u32FBSize));
        goto _GET_USED_SIZE;
    }

    if (u32AvailFrmBuffSize >= (MVD_DHD_FBSIZE*MVD_FBNUM_MAX))
    {
        MVD_DEBUGINFO(VPRINTF("%s(%d) DHD*5\n", __FUNCTION__, __LINE__));
        //Dual HD: (1920*2)*1088 or 1920*(1088*2)
        if (!_IS_VC1(fwCfg->eCodecType)) //mpeg2/4
        {
            *pu8FBMode = MVD3_DHD_MODE;
        }
        else
        {
            *pu8FBMode = MVD3_DHD_MODE_MIN;
        }
        *pu8FBNum = MVD_FBNUM_MAX;
    }
    else if (u32AvailFrmBuffSize >= (MVD_DHD_FBSIZE*MVD_FBNUM_MIN))
    {
        MVD_DEBUGINFO(VPRINTF("%s(%d) DHD*4\n", __FUNCTION__, __LINE__));
        //Dual HD: (1920*2)*1088 or 1920*(1088*2)
        if (!_IS_VC1(fwCfg->eCodecType)) //mpeg2/4
        {
            *pu8FBMode = MVD3_DHD_MODE;
        }
        else
        {
            *pu8FBMode = MVD3_DHD_MODE_MIN;
        }
        *pu8FBNum = MVD_FBNUM_MIN;
    }
    else if (u32AvailFrmBuffSize >= (MVD_HD_FBSIZE*MVD_FBNUM_MAX))
    {
        MVD_DEBUGINFO(VPRINTF("%s(%d) HD*5\n", __FUNCTION__, __LINE__));
        *pu8FBMode = MVD3_HD_MODE;
        *pu8FBNum = MVD_FBNUM_MAX;
    }
    else if (u32AvailFrmBuffSize >= (MVD_HD_FBSIZE*MVD_FBNUM_MIN))
    {
        MVD_DEBUGINFO(VPRINTF("%s(%d) HD*4\n", __FUNCTION__, __LINE__));
        *pu8FBMode = MVD3_HD_MODE;
        *pu8FBNum = MVD_FBNUM_MIN;
    }
    else
    {
        MVD_DEBUGINFO(VPRINTF("%s(%d) SD\n", __FUNCTION__, __LINE__));
        ret = MVD_CheckFrmBuffSizeMin(fwCfg, memCfg);
        if (TRUE == ret)
        {
            *pu8FBMode = MVD3_SD_MODE;
            *pu8FBNum = MVD_FBNUM_MIN;
            MVD_DEBUGINFO(VPRINTF("Framebuffer [SD] mode\n"));
        }
    }


    //Keep FBNum=4 for mpeg2/4, not-mstreamer/uniplayer/mcu mode.
    if (!_IS_VC1(fwCfg->eCodecType)) //mpeg2/4
    {
        if (*pu8FBNum != MVD_FBNUM_DEFAULT)
        {
            *pu8FBNum = MVD_FBNUM_DEFAULT;
        }
    }

_GET_USED_SIZE:
    fwCfg->u32FBUsedSize = u32AvailFrmBuffSize;

    MVD_DEBUGINFO(VPRINTF("%s u8FBMode=0x%x, u8FBNum=%d, FBSize=0x%lx, used=0x%lx\n",
                  __FUNCTION__, fwCfg->u8FBMode, fwCfg->u8FBNum, (unsigned long)memCfg->u32FBSize, (unsigned long)fwCfg->u32FBUsedSize));
    return ret;
}

MS_BOOL HAL_MVD_SetCodecInfo(MS_U8 u8Idx, MVD_CodecType eCodecType, MVD_SrcMode eSrcMode, MS_U8 bDisablePESParsing)
{
    MVD_CmdArg stCmdArg;
    //VPRINTF("u8CodecType=0x%x\n", u8CodecType);
    //VPRINTF("eSrcMode=0x%x\n", eSrcMode);
    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = MVD_MapCodecType(eCodecType);
    stCmdArg.Arg1 = MVD_MapSrcMode(eSrcMode);
    stCmdArg.Arg2 = bDisablePESParsing;
    //arg2 is only valid for STREAM_MODE and TS_FILE_MODE
    //set as 0 to enable MVD parser and parser interrupt
    stCmdArg.Arg3 = 0;
    MVD_DEBUGINFO(VPRINTF("MDrv_MVD_SetCodecInfo: Cmd: %x, Arg0: %x, Arg1: %x. Arg2: %x\n",
                  CMD_CODEC_INFO, stCmdArg.Arg0, stCmdArg.Arg1, stCmdArg.Arg2));
    SET_DECNUM(stCmdArg, u8Idx);
    SET_CMD_RET_FALSE(CMD_CODEC_INFO, &stCmdArg);

    HAL_MVD_SetSrcMode(u8Idx,eSrcMode);

    //set code offset to MVD
    MS_VIRT u32Addr, u32Len;
    HAL_MVD_MemGetMap(u8Idx, E_MVD_MMAP_FW, &u32Addr, &u32Len);

    if (u32Len==0) MVD_DEBUGERROR(VPRINTF("%s err: u32Len=0!\n", __FUNCTION__));
    MS_U32 i=0;
    i = u32Addr >> 3;
    SET_CMDARG(stCmdArg, i, u8Idx);
    SET_CMD_RET_FALSE(CMD_CODE_OFFSET, &stCmdArg);

    #define _mvdAssert(x) if(!x) MVD_DEBUGERROR(VPRINTF("%s(%d) inconsistent cfg!\n", __FUNCTION__, __LINE__));
    MVD_FWCfg* pCurFwCfg = HAL_MVD_GetFWCfg(u8Idx);
    _mvdAssert(pCurFwCfg->eCodecType == eCodecType);
    _mvdAssert(pCurFwCfg->eSrcMode == eSrcMode);
    _mvdAssert(pCurFwCfg->bDisablePESParsing == bDisablePESParsing);

    //Refer to msAPI_VDPlayer_DecodeMPEG4.c (core\kernel\api\videoplayer)
    if (eSrcMode == E_MVD_SLQ_TBL_MODE)
    {
        if ((eCodecType == E_MVD_CODEC_MPEG4) ||
            (eCodecType == E_MVD_CODEC_DIVX311) ||
            (eCodecType == E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER))
        {
            // Enable PackMode
            SETUP_CMDARG(stCmdArg);
            stCmdArg.Arg0 = 3;
            SET_DECNUM(stCmdArg, u8Idx);
            if (HAL_MVD_MVDCommand( CMD_PARSE_M4V_PACKMD, &stCmdArg ) == FALSE)
            {
                MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_PARSE_M4V_PACKMD ) );
                return FALSE;
            }

            // Set DIU width of rounding mode (align to 8byte)
            SETUP_CMDARG(stCmdArg);
            stCmdArg.Arg0 = 1;
            SET_DECNUM(stCmdArg, u8Idx);
            if (HAL_MVD_MVDCommand(CMD_DIU_WIDTH_ALIGN, &stCmdArg) == FALSE)
            {
                MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DIU_WIDTH_ALIGN ) );
                return FALSE;
            }
        }
    }

    //set internal buffers after setting codecinfo for V3
    if (!HAL_MVD_SetInternalBuffAddr(u8Idx, u32Addr, u32Len))
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_Init:_MVD_MVDSetInternalBuffAddr failed\n"));
        return FALSE;
    }
    else
    {
        MVD_DEBUGINFO(VPRINTF("MDrv_MVD_Init:_MVD_MVDSetInternalBuffAddr success\n"));
    }

    return TRUE;
}

BDMA_CpyType MVD_GetBDMAType(void)
{
    BDMA_CpyType bdmaCpyType = E_BDMA_CPYTYPE_MAX;

    if (pMVDHalContext->stMiuCfg.u8HWMiuSel == MIU_SEL_0)
    {
        if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_0)
        {
            bdmaCpyType = E_BDMA_SDRAM2SDRAM;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_1)
        {
            bdmaCpyType = E_BDMA_SDRAM2SDRAM1;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_2)
        {
            bdmaCpyType = E_BDMA_SDRAM2SDRAM2;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_3)
        {
            bdmaCpyType = E_BDMA_SDRAM2SDRAM3;
        }
        else
        {
            MS_ASSERT(0);
        }
    }
    else if (pMVDHalContext->stMiuCfg.u8HWMiuSel == MIU_SEL_1)
    {
        if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_0)
        {
            bdmaCpyType = E_BDMA_SDRAM12SDRAM;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_1)
        {
            bdmaCpyType = E_BDMA_SDRAM12SDRAM1;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_2)
        {
            bdmaCpyType = E_BDMA_SDRAM12SDRAM2;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_3)
        {
            bdmaCpyType = E_BDMA_SDRAM12SDRAM3;
        }
        else
        {
            MS_ASSERT(0);
        }
    }
    else if (pMVDHalContext->stMiuCfg.u8HWMiuSel == MIU_SEL_2)
    {
        if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_0)
        {
            bdmaCpyType = E_BDMA_SDRAM22SDRAM;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_1)
        {
            bdmaCpyType = E_BDMA_SDRAM22SDRAM1;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_2)
        {
            bdmaCpyType = E_BDMA_SDRAM22SDRAM2;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_3)
        {
            bdmaCpyType = E_BDMA_SDRAM22SDRAM3;
        }
        else
        {
            MS_ASSERT(0);
        }
    }
    else if (pMVDHalContext->stMiuCfg.u8HWMiuSel == MIU_SEL_3)
    {
        if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_0)
    {
            bdmaCpyType = E_BDMA_SDRAM32SDRAM;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_1)
        {
            bdmaCpyType = E_BDMA_SDRAM32SDRAM1;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_2)
        {
            bdmaCpyType = E_BDMA_SDRAM32SDRAM2;
        }
        else if (pMVDHalContext->stMiuCfg.u8FWMiuSel == MIU_SEL_3)
        {
            bdmaCpyType = E_BDMA_SDRAM32SDRAM3;
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

    return bdmaCpyType;
}

//Init static variables.
//Exception: stMemCfg & stMiuCfg since they are set before calling this function.
void HAL_MVD_InitVar(MS_U8 u8Idx)
{
    MVD_MEMCfg*  pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    memset(pstCtrlCfg, 0, sizeof(MVD_CtrlCfg));
    pstCtrlCfg->eTrickMode = E_MVD_TRICK_DEC_ALL;
    pstCtrlCfg->eFrcMode = E_MVD_FRC_NORMAL;
    pstCtrlCfg->ePreSpeedType = E_MVD_SPEED_DEFAULT;
    pstCtrlCfg->eFileSyncMode = E_MVD_TIMESTAMP_FREERUN;

    //determine if we need to BDMA SLQ table from DrvProcBuff to BitstreamBuff
    pstCtrlCfg->bSlqTblSync = ((pstMemCfg->u32DrvBufAddr < pstMemCfg->u32BSAddr) ||
                   ((pstMemCfg->u32DrvBufAddr+pstMemCfg->u32DrvBufSize) > (pstMemCfg->u32BSAddr+pstMemCfg->u32BSSize)));
    MVD_DEBUGINFO(VPRINTF("bSlqTblSync = %x\n", pstCtrlCfg->bSlqTblSync));
    if (pstCtrlCfg->bSlqTblSync)
    {
        const BDMA_Info* pBDMA;
        pBDMA = MDrv_BDMA_GetInfo();
        if ((pBDMA == NULL) || (pBDMA->bInit != TRUE))
        {
            if (E_BDMA_OK != MDrv_BDMA_Init(pstMemCfg->u32Miu1BaseAddr))
            {
                MVD_DEBUGERROR(VPRINTF("%s fail at MDrv_BDMA_Init!!!\n", __FUNCTION__));
            }
        }
        pstSlqTblInfo->bdmaCpyType = MVD_GetBDMAType();
    }

    memset(pstSlqTblInfo, 0, sizeof(MVD_SLQTBLInfo));
    pstSlqTblInfo->u32LastPts = MVD_NULLPKT_PTS;
    pstSlqTblInfo->u32PreEsRd = MVD_U32_MAX;
    pstSlqTblInfo->u32PreEsWr = 0;
    pstSlqTblInfo->pSlqStatus = &(pMVDHalContext->_SlqStatus[u8Idx]);
    pstSlqTblInfo->pDrvSlqTbl = &(pMVDHalContext->_drvSlqTbl[u8Idx]);
    pstSlqTblInfo->pDrvEsTbl = &(pMVDHalContext->_drvEsTbl[u8Idx]);
    pstSlqTblInfo->pDrvDivxTbl = &(pMVDHalContext->_drvDivxTbl[u8Idx]);
#if SLQ_NEW_PUSH
    pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr = 0;
    pstSlqTblInfo->pSlqStatus->u32SlqPushLength = 0;
    pstSlqTblInfo->pSlqStatus->bSlqPicStart = FALSE;
    pstSlqTblInfo->pSlqStatus->bSlqPicCollect = FALSE;
    pstSlqTblInfo->pSlqStatus->bSlqPicWaitNextStart = FALSE;
    pstSlqTblInfo->pSlqStatus->bSlqEnLastFrameShow =FALSE;
    pstSlqTblInfo->pSlqStatus->bSlqFireRdy = FALSE;
    pstSlqTblInfo->pSlqStatus->bSlqCtrlBit =FALSE;
#endif
    pMVDHalContext->u32LastAliveTime[u8Idx] = 0;
}

//------------------------------------------------------------------------------
/// Issue Stop command.
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_Stop(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    MS_BOOL bRet = TRUE;

    #define STOP_TIMEOUT 500 //ms
    MS_U32 u32StartTime = 0;
    u32StartTime = HAL_MVD_GetTime();

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_STOP);

    if (HAL_MVD_MVDCommand(CMD_STOP, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR(VPRINTF("Command: 0x%x fail!!\r\n", CMD_STOP));
        HAL_MVD_Delayms(1);
        if ( HAL_MVD_TimeOut(u8Idx) == TRUE )
        {
            MVD_DEBUGERROR(VPRINTF("*** MVD ERR: STOP TIMEOUT!!! ***\n"));
        }
    }

    while(!HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_STOP))
    {
        if ((HAL_MVD_GetTime()-u32StartTime)>STOP_TIMEOUT)
        {
            MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail timeout!!\r\n", CMD_STOP ) );
            break;
        }
    }

    pMVDHalContext->bStopped[u8Idx] = TRUE;
    return bRet;
}

MS_BOOL HAL_MVD_DeinitHW(void)
{
    //MVD HW reset
    if (!HAL_MVD_RstHW())
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_Exit:MVD4ResetHW failed\n"));
    }
    else
    {
        MVD_DEBUGINFO(VPRINTF("MDrv_MVD_Exit:MVD4ResetHW success\n"));
    }
    HAL_MVD_PowerCtrl(DISABLE);
    return TRUE;
}

MS_BOOL HAL_MVD_Exit(MS_U8 u8Idx)
{
    MVD_DEBUGINFO(VPRINTF("MDrv_MVD_Exit:start\n"));

    HAL_MVD_SetIsUsed(u8Idx, FALSE);
    pMVDHalContext->bAutoInsertDummyPattern[u8Idx] = FALSE;
    pMVDHalContext->bDropOnePTS[u8Idx] = FALSE;
    pMVDHalContext->u32DmxFrameRate[u8Idx] = 0;
    pMVDHalContext->u32DmxFrameRateBase[u8Idx] = 0;

    return TRUE;
}


//------------------------------------------------------------------------------
/// Set DivX311 stream info.
/// @param divxInfo \b IN : DivX311 stream info.
//------------------------------------------------------------------------------
static void MVD_WriteDivx311Data(MS_U8 u8Idx, FW_DIVX_INFO *divxInfo)
{
    MS_VIRT pu8MVDSetHeaderBufStart = GET_HDR_BUFFADD(u8Idx);
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

void HAL_MVD_SetFrameInfo(MS_U8 u8Idx, MVD_FrameInfo *pinfo )
{
    if (GET_HDR_BUFFADD(u8Idx)==0)
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_SetFrameInfo error: pu8MVDSetHeaderBufStart=NULL\n"));
        return;
    }

    FW_DIVX_INFO* pDivxInfo = &(pMVDHalContext->gdivxInfo[u8Idx]);
    pDivxInfo->width=pinfo->u16HorSize;
    pDivxInfo->height=pinfo->u16VerSize;
    pDivxInfo->aspect_ratio=pinfo->u8AspectRate;
    pDivxInfo->frame_rate = pinfo->u32FrameRate;

    //for MM
    pDivxInfo->mpeg1=pinfo->u8MPEG1;
    pDivxInfo->pts_incr=pinfo->u16PTSInterval;
    pDivxInfo->play_mode=pinfo->u8PlayMode;
    pDivxInfo->mpeg_frc_mode=pinfo->u8FrcMode;

    if(pinfo->u8Interlace==0)
        pDivxInfo->progressive_sequence=1;
    else
        pDivxInfo->progressive_sequence=0;

    pDivxInfo->frame_count=0;
    pDivxInfo->frame_time=0;
    pDivxInfo->vol_handle_done=0;
//    pDivxInfo->invalidstream=0;
    MVD_DEBUGINFO(VPRINTF("set vol info,pts_incr=%d,\n",pDivxInfo->pts_incr));
    MVD_DEBUGINFO(VPRINTF("set vol info,width=%x,height=%x,frame_rate=%d,aspect_ratio=%x,\n",
                  (unsigned int)pDivxInfo->width,(unsigned int)pDivxInfo->height,pDivxInfo->frame_rate,pDivxInfo->aspect_ratio));
    MVD_DEBUGINFO(VPRINTF("set vol info,progressive_sequence=%x,mpeg1=%x,play_mode=%x,\n",
                  pDivxInfo->progressive_sequence,pDivxInfo->mpeg1,pDivxInfo->play_mode));

    MVD_WriteDivx311Data(u8Idx, pDivxInfo);
    return;
}



///////////////////////////////////////////////////////////////////////////////
/// Get Hardware Pointer of MVD CC Ring Buffer
/// Return value:: The HW Pointer Address of MVD CC Ring Buffer
/// @param u8CC608 \b IN
///   - # TRUE for CC608 parser
///   - # FALSE for CC708 parser
///////////////////////////////////////////////////////////////////////////////
MS_VIRT HAL_CC_CM_GetMVDRB_HWAddr(MS_U8 u8CC608)
{
#if 1
    MVD_CmdArg mvdcmd;
    MS_VIRT u32CCWrPtr = 0;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg3 = u8CC608;
    if (HAL_MVD_MVDCommand( CMD_RD_USER_WP, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_RD_USER_WP ) );
        return MVD_U32_MAX;
    }
    u32CCWrPtr = COMBU32(mvdcmd.Arg3, mvdcmd.Arg2, mvdcmd.Arg1, mvdcmd.Arg0) << 3;
    return u32CCWrPtr;
#else
    MS_U32 u32MVDCC_Temp1 = 0;
    if (HAL_MVD_TimeOut(pstCmdArg->Arg5) == FALSE)
    {
        //HAL_MVD_ARGINIT();
        HAL_MVD_RegWriteByte(MVD_ARG5, 0);
        HAL_MVD_RegWriteByte(MVD_ARG4, 0);
        HAL_MVD_RegWriteByte(MVD_ARG3, (MS_U8)u8CC608);
        HAL_MVD_RegWriteByte(MVD_COMMAND, (MS_U8)(CMD_RD_USER_WP));//CMD_GET_CCBUF_HWADDR

        if (HAL_MVD_TimeOut(pstCmdArg->Arg5) == FALSE)
        {
            u32MVDCC_Temp1 = 0;
            u32MVDCC_Temp1 = (((MS_U32)HAL_MVD_RegReadByte(MVD_ARG0)) & 0x000000FF);
            u32MVDCC_Temp1 += ((((MS_U32)HAL_MVD_RegReadByte(MVD_ARG1)) & 0x000000FF) << 8);
            u32MVDCC_Temp1 += ((((MS_U32)HAL_MVD_RegReadByte(MVD_ARG2)) & 0x000000FF) << 16);
            u32MVDCC_Temp1 = (u32MVDCC_Temp1 << 3);
            return (u32MVDCC_Temp1);
        }
        else
        {
            MVD_DEBUGINFO(VPRINTF("\nF:GHAV"));
        }
    }
    else
    {
        MVD_DEBUGINFO(VPRINTF("\nF:GHA"));
    }

    return 0xffffffff;
#endif
}

MS_BOOL HAL_MVD_GetUsrDataIsAvailable(MS_U8 u8Idx)
{
    volatile MVD_CtrlCfg* pCtrlCfg = (volatile MVD_CtrlCfg*)HAL_MVD_GetCtrlCfg(u8Idx);
    MS_VIRT u32UsrDataWr = 0;
    MS_BOOL bIsAvail = FALSE;

#if defined(MVD_SUPPORT_X4_CC)
    u32UsrDataWr = HAL_CC_CM_GetMVDRB_HWAddr(4)>>3;
#else
    u32UsrDataWr = HAL_CC_CM_GetMVDRB_HWAddr(2)>>3;
#endif
    bIsAvail = !(pCtrlCfg->u32UsrDataRd == u32UsrDataWr);
    MVD_DEBUGINFO(VPRINTF("IsAvail:%x rd=%lx wr=%lx\n", bIsAvail,
                         (unsigned long)pCtrlCfg->u32UsrDataRd, (unsigned long)u32UsrDataWr));
    return bIsAvail;
}

//------------------------------------------------------------------------------
/// Get info of user data
//------------------------------------------------------------------------------
MS_BOOL HAL_MVD_GetUsrDataInfo(MS_U8 u8Idx, MVD_UsrDataInfo* pUsrInfo)
{
    MS_VIRT u32UsrData = NULL;
    MS_VIRT u32UsrDataWr = 0;
#if defined(MVD_SUPPORT_X4_CC)
    FW_USER_DATA_BUF_EXT stUsrDataExt;
#else
    FW_USER_DATA_BUF stUsrDataInfo;
#endif
    volatile MVD_CtrlCfg* pCtrlCfg = (volatile MVD_CtrlCfg*)HAL_MVD_GetCtrlCfg(u8Idx);

    if ((!pUsrInfo) || (GET_FRMINFO_BUFFADD(u8Idx)==0))
    {
        MVD_DEBUGERROR(VPRINTF("%s: NULL ptr.\n", __FUNCTION__));
        return FALSE;
    }

    //get write pointer
#if defined(MVD_SUPPORT_X4_CC)
    u32UsrDataWr = HAL_CC_CM_GetMVDRB_HWAddr(4)>>3;
#else
    u32UsrDataWr = HAL_CC_CM_GetMVDRB_HWAddr(2)>>3;
#endif
    if (pCtrlCfg->u32UsrDataRd == (GET_USRDATA_BUFFADD(u8Idx)+MVD3_FW_USER_DATA_BUF_LEN))
    {
        pCtrlCfg->u32UsrDataRd = GET_USRDATA_BUFFADD(u8Idx); //wrap to BufStart
    }
    MVD_DEBUGINFO(VPRINTF("CC Rd=0x%lx Wr=0x%lx\n", (unsigned long)pCtrlCfg->u32UsrDataRd, (unsigned long)u32UsrDataWr));

    if (pCtrlCfg->u32UsrDataRd == u32UsrDataWr)
    {
        MVD_DEBUGERROR(VPRINTF("%s: no data? Rd=0x%lx Wr=0x%lx\n", __FUNCTION__, (unsigned long)pCtrlCfg->u32UsrDataRd, (unsigned long)u32UsrDataWr));
        return FALSE;
    }

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,pCtrlCfg->u32UsrDataRd,u32UsrData);

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

    pUsrInfo->u8PicType   = stUsrDataInfo.picType;
    pUsrInfo->u8TopFieldFirst = stUsrDataInfo.top_ff;
    pUsrInfo->u8RptFirstField = stUsrDataInfo.rpt_ff;
    pUsrInfo->u16TmpRef = stUsrDataInfo.tmpRef;
    pUsrInfo->u8ByteCnt = stUsrDataInfo.userdatabytecnt;
    pUsrInfo->u32DataBuf = u32UsrData + MVD_FW_USER_DATA_HDR_LEN;
#endif
    //update read pointer
    pCtrlCfg->u32UsrDataRd += MVD_FW_USER_DATA_PKT_LEN;
#if 0
    VPRINTF("xxInfo: ");
    VPRINTF("%02d, ", pUsrInfo->u16TmpRef);
    VPRINTF("%d, ", pUsrInfo->u8PicStruct);
    VPRINTF("%d, ", pUsrInfo->u8TopFieldFirst);
    VPRINTF("0x%x, ", pUsrInfo->u32DataBuf);
    VPRINTF("%d, ", pUsrInfo->u8ByteCnt);
    VPRINTF("%ld, ", pUsrInfo->u32Pts);
    VPRINTF("%d\n", pUsrInfo->u8PicType);
#endif
    return TRUE;
}


MS_VIRT HAL_MVD_Map2DrvSlqTbl(MS_U8 u8Idx, MS_VIRT u32HWPtr)
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_VIRT u32HWSt = HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr);
    MS_VIRT u32DrvPtr;

    if ((u32HWPtr<u32HWSt) && (u32HWPtr!=0))
    {
        MVD_DEBUGERROR(VPRINTF("Invalid u32HWPtr=0x%lx\n", (unsigned long)u32HWPtr));
        return 0;
    }
    if ((pstCtrlCfg->bSlqTblSync) && (u32HWPtr!=0))
    {
        u32DrvPtr = pstSlqTblInfo->pDrvSlqTbl->u32StAdd + (u32HWPtr - u32HWSt);
        return u32DrvPtr;
    }
    return u32HWPtr;
}


static void MVD_SLQTblGetFileEndPkt(MS_U8 u8Idx, MVD_PacketInfo* pFileEnd)
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MS_VIRT u32EndPattern = HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr+SLQ_TBL_SIZE*3);

    pFileEnd->u32StAddr = SLQ_TBL_SIZE*3;//u32EndPattern - HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr);
    pFileEnd->u32Length = END_PATTERN_SIZE;
    pFileEnd->u32TimeStamp = MVD_NULLPKT_PTS;
    pFileEnd->u32ID_L = MVD_U32_MAX;
    pFileEnd->u32ID_H = MVD_U32_MAX;
    MVD_DEBUGINFO(VPRINTF("u32EndPattern(0x%lx)=0x%x 0x%x 0x%x 0x%x\n", (unsigned long)pFileEnd->u32StAddr,
            HAL_MVD_MemRead4Byte(u32EndPattern), HAL_MVD_MemRead4Byte(u32EndPattern+4),
            HAL_MVD_MemRead4Byte(u32EndPattern+8), HAL_MVD_MemRead4Byte(u32EndPattern+12)));
}


static void MVD_SLQTblGetDummyPkt(MVD_PacketInfo* pDummy)
{
    //MS_U32 u32DummyES = HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr+SLQ_TBL_SIZE*2);

    pDummy->u32StAddr = SLQ_TBL_SIZE*2;//u32DummyES - HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr);
    pDummy->u32Length = DUMMY_SIZE;
    pDummy->u32TimeStamp = MVD_NULLPKT_PTS;
    pDummy->u32ID_L = MVD_U32_MAX;
    pDummy->u32ID_H = MVD_U32_MAX;
#if 0
    VPRINTF("u32DummyES(0x%x-->0x%x, size=0x%x)=0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", u32DummyES,
            pDummy->u32StAddr, pDummy->u32Length, HAL_MVD_MemRead4Byte(u32DummyES),
            HAL_MVD_MemRead4Byte(u32DummyES+4),HAL_MVD_MemRead4Byte(u32DummyES+8),HAL_MVD_MemRead4Byte(u32DummyES+12));
#endif

}


MS_BOOL HAL_MVD_SLQTblInsertPattern(MS_U8 u8Idx, MVD_PatternType ePattern)
{
    MS_U32 i;
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    if (pstSlqTblInfo->pDrvSlqTbl->u32Empty < SLQ_TBL_SAFERANGE)
    {
        MVD_DEBUGINFO(VPRINTF("SLQTbl full!(0x%lx) Cannot insert pattern any more!\n", (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32Empty));
        return FALSE;
    }

#if SLQ_NEW_PUSH
    if(pstSlqTblInfo->pSlqStatus->bSlqCtrlBit)
    {
        if(pstSlqTblInfo->pSlqStatus->bSlqPicWaitNextStart)
        {
            pstSlqTblInfo->pDrvSlqTbl->u32WrPtr = pstSlqTblInfo->pSlqStatus->u32VaildWptrAddr;
        }
        pstSlqTblInfo->pSlqStatus->bSlqCtrlBit = FALSE;
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
            MVD_SLQTblGetFileEndPkt(u8Idx, &stDummyPkt);
        }
        else
        {
            MVD_DEBUGERROR(VPRINTF("Invalid MVD_PatternType! Won't insert pattern!\n"));
            return FALSE;
        }
        #if SLQ_NEW_PUSH
        pstSlqTblInfo->pSlqStatus->bSlqFireRdy = TRUE;
        #endif
        //VPRINTF("WrPtr 0x%x ", pstSlqTblInfo->pDrvSlqTbl->u32WrPtr);
        HAL_MVD_SLQTblSendPacket(u8Idx, &stDummyPkt);
        //VPRINTF("==> 0x%x\n", pstSlqTblInfo->pDrvSlqTbl->u32WrPtr);
        HAL_MVD_SetSLQWritePtr(u8Idx, FALSE);
    }
    return TRUE;
}


#define FLAG_LAST_FRM_SHOW (MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CMD_LAST_FRAME_SHOW, sizeof(MS_U32)))
E_MVD_Result HAL_MVD_IsDispFinish(MS_U8 u8Idx)
{
    MS_U32 u32TimeCnt;
    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8Idx);
    MS_U32 u32FeByteCnt = 0;
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_VIRT u32FileEndPtr = pstSlqTblInfo->u32FileEndPtr;
    MS_U32 u32Id = MVD_GetStreamId(u8Idx);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
#ifdef VDEC3
    MS_VIRT u32VBBUAddr = HAL_MVD_GetVBBUTableAddr(u8VPUIdx);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    #endif

    //VPRINTF("MDrv_MVD_IsDispFinish::");
    if (GET_FRMINFO_BUFFADD(u8Idx)==0)
    {
        MVD_DEBUGERROR(VPRINTF("%s err: pu8MVDGetFrameInfoBufStart=NULL\n", __FUNCTION__));
        return E_MVD_RET_FAIL;
    }

    //VPRINTF("0x%x\n", FLAG_LAST_FRM_SHOW);
    if ((E_MVD_SLQ_TBL_MODE == curSrcMode) && (TRUE != FLAG_LAST_FRM_SHOW))
    {
        //insert pattern when each time checking IsDispFinish
#ifdef VDEC3
        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            u32TimeCnt= HAL_MVD_GetTime();
            while ((HAL_MVD_GetTime() - u32TimeCnt) < 20)  // timeout 20 ms
            {
                if(HAL_VPU_EX_GetVBBUVacancy(u32VBBUAddr) > 0)
                {
                    HAL_VPU_EX_PacketInfo stVpuPkt;
                    MVD_PacketInfo stDummyPkt;
                    MVD_SLQTblGetFileEndPkt(u8Idx,&stDummyPkt);

                    stVpuPkt.u32Offset = stDummyPkt.u32StAddr;
                    stVpuPkt.u32Length = stDummyPkt.u32Length;
                    stVpuPkt.u64TimeStamp = (MS_U64)stDummyPkt.u32TimeStamp;
                    //If in display queue mode
                    stVpuPkt.u64TimeStamp |= ((MS_U64)stDummyPkt.u32ID_H << 32);
                    stVpuPkt.u32ID_H = stDummyPkt.u32ID_H;
                    stVpuPkt.u32ID_L = stDummyPkt.u32ID_L;

                    HAL_VPU_EX_Push2VBBU(u32Id, &stVpuPkt,u32VBBUAddr);
                }

                if (TRUE == FLAG_LAST_FRM_SHOW)
                {
                    break;
                }
            }

            if (TRUE != FLAG_LAST_FRM_SHOW)
            {
                MVD_DEBUGERROR(VPRINTF("MDrv_MVD_IsDispFinish TIMEOUT\n"));
                return E_MVD_RET_TIME_OUT;
            }
            else
            {
                return E_MVD_RET_OK;
            }
        }
        else
#endif
        {
        //insert pattern when each time checking IsDispFinish
        if (HAL_MVD_SLQTblInsertPattern(u8Idx, E_MVD_PATTERN_FILEEND))
        {
            u32FeByteCnt += END_PATTERN_SIZE;
        }

        if ((u32FileEndPtr == HAL_MVD_Map2DrvSlqTbl(u8Idx, HAL_MVD_GetSLQReadPtr(u8Idx))) &&
            (HAL_MVD_IsMcuMode(u8Idx) == FALSE) &&
            (HAL_MVD_IsMStreamerMode(u8Idx) == FALSE))
        {
            //insert padding pattern until timeout
            u32TimeCnt= HAL_MVD_GetTime();
            while ((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS)
            {
                if (TRUE == FLAG_LAST_FRM_SHOW)
                {
                    //VPRINTF("\nDisp finished!\n");
                    break;
                }
                //insert file-end pattern again
                if (HAL_MVD_SLQTblInsertPattern(u8Idx, E_MVD_PATTERN_FILEEND))
                {
                    u32FeByteCnt += END_PATTERN_SIZE;
                }
            }
            if ((HAL_MVD_GetTime() - u32TimeCnt) >= CMD_TIMEOUT_MS)
            {
                MVD_DEBUGERROR(VPRINTF("\n***** MDrv_MVD_IsDispFinish TIMEOUT!!! *****\n\n"));
                if (E_MVD_CODEC_VC1_MAIN == HAL_MVD_GetCodecType(u8Idx))
                {
                    if ((HAL_MVD_GetPayloadLen(u8Idx) > 0x200000)
                        && (TRUE != HAL_MVD_GotFileEndPattern(u8Idx)))
                    {
                        MVD_DEBUGERROR(VPRINTF("RCV payloadLen(0x%x) invalid!\n",
                            HAL_MVD_GetPayloadLen(u8Idx)));
                    }
                }
                MVD_DEBUGERROR(VPRINTF("***** fe=%lx, rd=%lx(%lx,%lx), wr=%lx, empty=%lx, u32FeByteCnt=%x\n",
                    (unsigned long)u32FileEndPtr, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32RdPtr, (unsigned long)HAL_MVD_Map2DrvSlqTbl(u8Idx, HAL_MVD_GetSLQReadPtr(u8Idx)),
                    (unsigned long)HAL_MVD_GetSLQReadPtr(u8Idx), (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32WrPtr, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32Empty, u32FeByteCnt));
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
            MVD_DEBUGINFO(VPRINTF("fe=%lx, rd=%lx(%lx), wr=%lx, empty=%lx\n", (unsigned long)u32FileEndPtr, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32RdPtr,
                            (unsigned long)HAL_MVD_Map2DrvSlqTbl(u8Idx, (unsigned long)HAL_MVD_GetSLQReadPtr(u8Idx)), (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32WrPtr, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32Empty));
            return E_MVD_RET_FAIL;
        }
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
/// Set MVD SLQ start & end address
/// @param -u32start \b IN : start address
/// @param -u32end \b IN : end address
//------------------------------------------------------------------------------
static void HAL_MVD_SetSLQStartEnd(MS_U8 u8Idx, MS_U32 u32start, MS_U32 u32end)
{
    MVD_CmdArg mvdcmd;

    if ((u32start > SLQ_ADDR_LEN) || ((u32end+1) > SLQ_ADDR_LEN))
    {
        MVD_DEBUGERROR(VPRINTF("MDrv_MVD_SetSLQStartEnd: only support 27bit add!\n"));
    }

    SET_CMDARG(mvdcmd, (u32end+1), u8Idx);
    if (HAL_MVD_MVDCommand( CMD_SLQ_END, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SLQ_END ) );
        return;
    }

    SET_CMDARG(mvdcmd, u32start, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_SLQ_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SLQ_START ) );
        return;
    }

    return;
}

MS_BOOL HAL_MVD_DecodeIFrame(MS_U8 u8Idx, MS_PHY u32FrameBufAddr, MS_PHY u32StreamBufAddr, MS_PHY u32StreamBufEndAddr )
{
    MS_U32 u32deley = 0;
    MS_U32 u32time = 0;
    MVD_CmdArg mvdcmd;
    MS_VIRT u32MVDFWSLQTABTmpbufAdr = GET_SLQ_BUFFADD(u8Idx);

    MVD_DEBUGINFO(VPRINTF("%s offset FBAdd=0x%lx streamStart=0x%lx streamEnd=0x%lx\n",
            __FUNCTION__, (unsigned long)u32FrameBufAddr, (unsigned long)u32StreamBufAddr, (unsigned long)u32StreamBufEndAddr));

    HAL_MVD_SetCodecInfo(u8Idx, E_MVD_CODEC_MPEG2, E_MVD_SLQ_MODE, DISABLE_PARSER);
    HAL_MVD_SetFrameBuffAddr(u8Idx, u32FrameBufAddr, HAL_MVD_GetFBMode(u8Idx));

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_DISPLAY_CTL, &mvdcmd);

    if (HAL_MVD_StepDecode(u8Idx) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "HAL_MVD_StepDecode fail!!\r\n") );
        return FALSE;
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = 1;
    mvdcmd.Arg1 = 1;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FAST_SLOW, &mvdcmd);

    //set data
    HAL_MVD_Delayms(2);

    HAL_MVD_CPU_Sync();
    HAL_MVD_FlushMemory();

    //wait vld init success or data may lost!
#define WAIT_INIT_SUCCESS_TIME 100 //100ms
    u32deley = HAL_MVD_GetTime();
    while ((HAL_MVD_GetDecodeStatus(u8Idx)==DEC_STAT_IDLE) && (u32time<WAIT_INIT_SUCCESS_TIME))
    {
        u32time = HAL_MVD_GetTime()-u32deley;
    }
    if (u32time>=WAIT_INIT_SUCCESS_TIME)
    {
        MVD_DEBUGERROR(VPRINTF("%s: wait init_success timeout!!!\n", __FUNCTION__));
    }
    HAL_MVD_SetSLQStartEnd(u8Idx, u32StreamBufAddr, u32StreamBufEndAddr);
    MVD_DEBUGINFO(VPRINTF("set MVD3_FW_SLQ_TAB_TMPBUF_ADR=%lx\n",(unsigned long)u32MVDFWSLQTABTmpbufAdr));

    HAL_MVD_CPU_Sync();
    HAL_MVD_FlushMemory();
    HAL_MVD_SetSLQStartEnd(u8Idx, u32MVDFWSLQTABTmpbufAdr, u32MVDFWSLQTABTmpbufAdr+MVD3_FW_SLQ_TAB_TMPBUF_LEN);

    HAL_MVD_CPU_Sync();
    HAL_MVD_ReadMemory();

    // wait decode complete
#define WAIT_DECODE_DONE_TIME 33 //To decode 1 frame should take less than 33ms
    u32deley = HAL_MVD_GetTime();
    u32time = 0;
    while (HAL_MVD_GetPicCounter(u8Idx)<1 && (u32time<WAIT_DECODE_DONE_TIME))
    {
        u32time = HAL_MVD_GetTime()-u32deley;
    }
    if (u32time >= WAIT_DECODE_DONE_TIME)
    {
        MVD_DEBUGERROR(printf ("MDrv_MVD_DecodeIFrame time out(du=%d, st=%d, now=%d)\n", u32time, u32deley, HAL_MVD_GetTime()));
        MVD_DEBUGERROR(VPRINTF("frmCnt=%d state=0x%x lastCmd=0x%x\n", HAL_MVD_GetPicCounter(u8Idx), HAL_MVD_GetDecodeStatus(u8Idx), HAL_MVD_GetLastCmd(u8Idx)));
        return FALSE;
    }
    MVD_DEBUGINFO(printf ("MDrv_MVD_DecodeIFrame time (%d, %d)\n", u32time, u32deley));
    //VPRINTF("frmCnt=%ld state=0x%x lastCmd=0x%x\n", HAL_MVD_GetPicCounter(u8Idx), HAL_MVD_GetDecodeStatus(u8Idx), HAL_MVD_GetLastCmd(u8Idx));

    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    pCtrlCfg->bDecodeIFrame = TRUE;

    return TRUE;
}


//------------------------------------------------------------------------------
/// Set bit stream buffer address to MVD
/// @param -u32start \b IN : start address
/// @param -u32end \b IN : end address
//------------------------------------------------------------------------------
static void MVD_SetBitStreamAddr(MS_U8 u8Idx, MS_VIRT u32start, MS_VIRT u32end)
{
    MVD_CmdArg mvdcmd;
    MS_ASSERT((u32start%8)==0);
    u32start >>= 3;
    SET_CMDARG(mvdcmd, u32start, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_STREAM_BUF_START, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_STREAM_BUF_START ) );
        return;
    }

    MS_ASSERT((u32end%8)==0);
    u32end >>= 3;
    SET_CMDARG(mvdcmd, u32end, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_STREAM_BUF_END, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_STREAM_BUF_END ) );
        return;
    }
    return;
}

MS_BOOL HAL_MVD_SetInternalBuffAddr(MS_U8 u8Idx, MS_VIRT u32start, MS_U32 u32len)
{
    MS_VIRT tmpAdr, tmpLen;
    MVD_FWCfg* pFwCfg = NULL;
    MVD_FWBuff* pBuff = &(pMVDHalContext->stFWBuff[u8Idx]);
    MVD_MEMCfg* pstMemCfg = NULL;
    MVD_SLQTBLInfo* pstSlqTblInfo = NULL;
    MS_U32 u32Id = MVD_GetStreamId(u8Idx);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
    MS_VIRT u32BuffStart = u32start+(MVD_FW_TASK_OFFSET*u8VPUIdx);
    MS_VIRT VPUSHMAddr = HAL_VPU_EX_GetSHMAddr();
    MS_VIRT u32StartOffset;
    MS_U8  u8MiuSel;

    if(VPUSHMAddr != 0)
    {
        _phy_to_miu_offset(u8MiuSel, u32StartOffset, VPUSHMAddr);

        u32StartOffset -= MVD_FW_CODE_LEN;

        if(u8Idx == 0)
        {
            u32BuffStart = u32StartOffset; // for main decoder
        }
        else if(u8Idx == 1)
        {
            u32BuffStart = u32StartOffset + 0x20000; // VPUSHMAddr+128K bytes
        }
    }

    MVD_DEBUGINFO(VPRINTF("MVD FW shared mem start = 0x%lx\n", (unsigned long)u32BuffStart));
    MVD_GetFWBuffAdd(u32BuffStart, u32len, pBuff);
    MVD_SetFWBuffAdd(u8Idx, pBuff);

    tmpAdr = pBuff->u32DecFrmInfoAdd + MVD_FW_DECFRM_INFO_BUF_LEN;

    HAL_MVD_MemGetMap(u8Idx, E_MVD_MMAP_FB, &tmpAdr, &tmpLen);
    MVD_DEBUGINFO(VPRINTF("set MVD_FRAMEBUFFER_ADR=%lx\n",(unsigned long)tmpAdr));
    HAL_MVD_SetFrameBuffAddr(u8Idx, tmpAdr, HAL_MVD_GetFBMode(u8Idx));
    pFwCfg = HAL_MVD_GetFWCfg(u8Idx);
    HAL_MVD_SetFrameBuffNum(u8Idx, pFwCfg->u8FBNum,pFwCfg->u32FBUsedSize);

//  If VD_MHEG5(CPU) and MVD HW engine are run on different MIU,
// IAP, DP, and MV buffers are allocated after FB.
// The reason is that these 3 buffers are used by MVD HW engine.
    if(pMVDHalContext->bCMAUsed == FALSE)
    {
        tmpAdr += pFwCfg->u32FBUsedSize;
        pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
        if (pstMemCfg->u32FBSize < (pFwCfg->u32FBUsedSize + MVD_HW_BUF_TOTAL_LEN))
        {
            MVD_DEBUGERROR(VPRINTF("MVD HW buffers larger than FB size!!!\n"));
        }
        MVD_DEBUGINFO(VPRINTF("MVD FB boundary =0x%lx\n",(unsigned long)tmpAdr));
        tmpAdr = HAL_MVD_SetHWBuffer(u8Idx, tmpAdr);
    }

    HAL_MVD_MemGetMap(u8Idx, E_MVD_MMAP_BS, &tmpAdr, &tmpLen);
    MVD_DEBUGINFO(VPRINTF("set MVD_BITSTREAM_ADR=%lx\n",(unsigned long)tmpAdr));
    MVD_SetBitStreamAddr(u8Idx, tmpAdr,tmpAdr+tmpLen);
    pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    pstSlqTblInfo->u32ESBuffEnd = tmpAdr+tmpLen;

    if((pFwCfg->stFBReduction.LumaFBReductionMode != E_MVD_FB_REDUCTION_NONE)
    || (pFwCfg->stFBReduction.ChromaFBReductionMode  != E_MVD_FB_REDUCTION_NONE))
    {
        //for chips not support reduction mode, just ignore related config.
        MVD_DEBUGERROR(VPRINTF("MVD Err: Not support FB reduction mode!!\n"));
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// Issue "Decode Pause" command.
//------------------------------------------------------------------------------
void HAL_MVD_DecodePause(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_PAUSE, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_PAUSE) );
        return;
    }

    return;
}


//------------- Below functions are for MediaCodec SLQ Table --------------------
#if _SLQTBL_DUMP_PTS
static void _SLQTbl_DumpPtsTbl(MS_U8 u8Idx, MS_U32 u32EntryStart, MS_U32 u32EntryEnd)
{
    MS_U32 i;
    MS_VIRT u32EsRp, u32EsStart, u32EsEnd;
    MS_VIRT u32MVDFWPtsTblAddr = GET_PTSTBL_BUFFADD(u8Idx);

    for (i=u32EntryStart; i<u32EntryEnd; i++)
    {
        u32EsRp = pstSlqTblInfo->pDrvEsTbl->u32StAdd + i*8;

        u32EsEnd = HAL_MVD_MemRead4Byte(u32EsRp+4);
        u32EsStart = HAL_MVD_MemRead4Byte(u32EsRp); //report StartAdd as read_pointer
        MVD_DEBUGINFO(VPRINTF("ES[%x] Start=0x%x End=0x%x u32EsRp=%x\n",
            i, u32EsStart, u32EsEnd, u32EsRp));
    }

    MVD_DEBUGINFO(VPRINTF("\n=======Dump PTS table========\n"));
    MVD_DEBUGINFO(VPRINTF("addr\t byte_cnt\t dummy_cnt\t id_low\t id_high\t time_stamp\n"));
    for (i=u32EntryStart; i<u32EntryEnd; i++)
    {
        MVD_DEBUGINFO(VPRINTF("0x%x\t 0x%08lx\t 0x%08lx\t 0x%08lx\t 0x%08lx\t 0x%08lx\n", u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN,
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN),   //byteCnt
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+4), //dummyPktCnt
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+8), //idLow
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+12),//idHigh
               HAL_MVD_MemRead4Byte(u32MVDFWPtsTblAddr+i*MVD_FW_SLQTBL_PTS_LEN+16) //pts
               ));
    }
    MVD_DEBUGINFO(VPRINTF("=====================================\n"));
}
#endif


#if SLQ_NEW_PUSH
void MVD_SLQTblGetHdrPkt(MS_U8 u8Idx, MVD_PacketInfo* pDivxHdr, MVD_PacketInfo* pDivxData)
#else
static void MVD_SLQTblGetDivxHdrPkt(MS_U8 u8Idx, MVD_PacketInfo* pDivxHdr, MVD_PacketInfo* pDivxData)
#endif
{
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_VIRT u32DivXPattern = pstSlqTblInfo->pDrvDivxTbl->u32WrPtr;
    MS_U32 u32FrmSize = pDivxData->u32Length;
    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
#ifdef VDEC3
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
#endif
#if SLQ_NEW_PUSH
    if(pstSlqTblInfo->pSlqStatus->bSlqCtrlBit && pstCtrlCfg->u8McuMode == FALSE)
    {
        if(pstSlqTblInfo->pSlqStatus->bSlqPicWaitNextStart && pstSlqTblInfo->pSlqStatus->bSlqPicStart)
        {
             //VPRINTF("Show KC the SlqPushLength = 0x%x\n",pstSlqTblInfo->pSlqStatus->u32SlqPushLength);
            if (E_MVD_CODEC_DIVX311 == curCodecType)
            {
                HAL_MVD_MemWrite4Byte(pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr +4, pstSlqTblInfo->pSlqStatus->u32SlqPushLength);
                HAL_MVD_MemWrite4Byte(pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr , DIVX_PATTERN);
            }
            else if(E_MVD_CODEC_VC1_MAIN == curCodecType)
            {//rcv
                HAL_MVD_MemWrite4Byte(pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr +4, RCV_PATTERN);
                HAL_MVD_MemWrite4Byte(pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr , pstSlqTblInfo->pSlqStatus->u32SlqPushLength);
            }
            else if (E_MVD_CODEC_VC1_ADV == curCodecType)
            {
                HAL_MVD_MemWrite4Byte(pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr , VC1_PATTERN);
            }
            pstSlqTblInfo->pSlqStatus->u32SlqPushLength = 0;
            pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr  = 0;
            pstSlqTblInfo->pSlqStatus->bSlqFireRdy =TRUE;
            pstSlqTblInfo->pSlqStatus->u32VaildWptrAddr = pstSlqTblInfo->pDrvSlqTbl->u32WrPtr;// - 16;
            pstSlqTblInfo->pSlqStatus->bSlqPicWaitNextStart = FALSE;
            pstSlqTblInfo->pSlqStatus->bSlqPicCollect = FALSE;
        }

        if(pstSlqTblInfo->pSlqStatus->bSlqPicStart)
        {
            pstSlqTblInfo->pSlqStatus->u32SlqPatternAddr = u32DivXPattern;
            pstSlqTblInfo->pSlqStatus->u32SlqPushLength += u32FrmSize;
            pstSlqTblInfo->pSlqStatus->bSlqPicCollect = TRUE;
            pstSlqTblInfo->pSlqStatus->bSlqPicWaitNextStart =TRUE;
        }
        else if(pstSlqTblInfo->pSlqStatus->bSlqPicCollect)
        {
            pstSlqTblInfo->pSlqStatus->u32SlqPushLength += u32FrmSize;
            pstSlqTblInfo->pSlqStatus->bSlqPicWaitNextStart =TRUE;
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
    pDivxHdr->u32StAddr = u32DivXPattern - HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr);
    pDivxHdr->u32TimeStamp = pDivxData->u32TimeStamp; //unit: ms
    pDivxHdr->u32ID_L = pDivxData->u32ID_L;
    pDivxHdr->u32ID_H = pDivxData->u32ID_H;
    //VPRINTF("u32DivXPattern(0x%x==>0x%x)=0x%x 0x%x\n", u32DivXPattern, pDivxHdr->u32StAddr,
    //        HAL_MVD_MemRead4Byte(u32DivXPattern), HAL_MVD_MemRead4Byte(u32DivXPattern+4));
    if (E_MVD_CODEC_VC1_ADV == curCodecType)
    {
        pstSlqTblInfo->pDrvDivxTbl->u32WrPtr += 4;
        pDivxHdr->u32Length = 4;
    }
    else
    {
        pstSlqTblInfo->pDrvDivxTbl->u32WrPtr += 8;
        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            pDivxHdr->u32Length = 64;
            if (pstSlqTblInfo->pDrvDivxTbl->u32WrPtr >= pstSlqTblInfo->pDrvDivxTbl->u32EndAdd)
            {
                pstSlqTblInfo->pDrvDivxTbl->u32WrPtr = pstSlqTblInfo->pDrvDivxTbl->u32StAdd;
            }
        }
        else
        {
            pDivxHdr->u32Length = 8;
        }
    }

#if SLQ_NEW_PUSH
    if(pstSlqTblInfo->pSlqStatus->bSlqPicStart && pstCtrlCfg->u8McuMode == FALSE)
    {
        if (E_MVD_CODEC_VC1_ADV == curCodecType)
        {
            pstSlqTblInfo->pDrvDivxTbl->u32WrPtr += 4;
            pDivxHdr->u32Length = 4;
        }
        else
        {
            pstSlqTblInfo->pDrvDivxTbl->u32WrPtr += 8;
            pDivxHdr->u32Length = 8;
        }
    }
#endif
}

static void MVD_SLQTblInitFileEndPkt(MS_U8 u8Idx, MVD_CodecType eType)
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MS_VIRT u32EndPattern = HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr+SLQ_TBL_SIZE*3);
    MS_PHY temp = 0;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

#if 0
    _MVD_Memset(u32EndPattern, 0xff, END_PATTERN_SIZE);
#else
    //_miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32EndPattern,temp);
    temp = u32EndPattern+pMVDHalContext->stMiuCfg.u8FWMiuSel*HAL_MIU1_BASE;
    memset((void*)(MS_PA2KSEG1(temp)),0xff,END_PATTERN_SIZE);
    MsOS_FlushMemory();
    MsOS_ReadMemory();
#endif

    if ((E_MVD_CODEC_FLV == eType)||(E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER == eType))
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, FLV_PATTERN);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, 0xffffffff);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_1); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_2); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+16,END_PATTERN_3); //scw
        //VPRINTF("##########FileEnd for FLV/SVH!, u32EndPattern=%x\n",u32EndPattern);

        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            HAL_MVD_MemWrite4Byte(u32EndPattern+20,FLV_PATTERN);
        }
    }
    else if (E_MVD_CODEC_DIVX311 == eType)
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, DIVX_PATTERN);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, 0xffffffff);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_1); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_2); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+16,END_PATTERN_3); //scw
        //VPRINTF("##########FileEnd for DIVX311!, u32EndPattern=%x\n",u32EndPattern);

        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            HAL_MVD_MemWrite4Byte(u32EndPattern+20,DIVX_PATTERN);
        }
    }
    else if ((E_MVD_CODEC_MPEG2 == eType)||(E_MVD_CODEC_MPEG4 == eType))
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, MPEG_PATTERN_0);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, END_PATTERN_1);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_2);
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_3);
        //VPRINTF("##########FileEnd for MPEG2/4!, u32EndPattern=%x\n",u32EndPattern);

        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            HAL_MVD_MemWrite4Byte(u32EndPattern+16,DUMMY_PATTERN);
            HAL_MVD_MemWrite4Byte(u32EndPattern+20,DUMMY_PATTERN);
        }
    }
    else
    {
        HAL_MVD_MemWrite4Byte(u32EndPattern, END_PATTERN_0);
        HAL_MVD_MemWrite4Byte(u32EndPattern+4, END_PATTERN_1);
        HAL_MVD_MemWrite4Byte(u32EndPattern+8, END_PATTERN_2); //scw
        HAL_MVD_MemWrite4Byte(u32EndPattern+12,END_PATTERN_3); //scw
        //VPRINTF("##########FileEnd for VC1!, u32EndPattern=%x\n",u32EndPattern);

        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            if(E_MVD_CODEC_VC1_ADV == eType)
            {
                HAL_MVD_MemWrite4Byte(u32EndPattern+16,VC1_PATTERN_0);
                HAL_MVD_MemWrite4Byte(u32EndPattern+20,VC1_PATTERN_3);
            }
            else if(E_MVD_CODEC_VC1_MAIN == eType)
            {
                HAL_MVD_MemWrite4Byte(u32EndPattern+16,RCV_PATTERN_3);
                HAL_MVD_MemWrite4Byte(u32EndPattern+20,RCV_PATTERN_1);
                HAL_MVD_MemWrite4Byte(u32EndPattern+24,RCV_PATTERN_3);
            }
        }
    }

    MVD_DEBUGINFO(VPRINTF("u32EndPattern(0x%lx)=0x%x 0x%x\n", (unsigned long)u32EndPattern,
            HAL_MVD_MemRead4Byte(u32EndPattern), HAL_MVD_MemRead4Byte(u32EndPattern+4)));
}


static void MVD_SLQTblInitDummyPkt(MS_U8 u8Idx, MVD_CodecType eType)
{
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MS_VIRT u32DummyES = HAL_MVD_GetMemOffset(pstMemCfg->u32DrvBufAddr+SLQ_TBL_SIZE*2);
    MS_U32 u32DummyPattern[3];
    MS_U32 u32PatternSize;
    MS_U32 i;
    MS_PHY temp = 0;

    //VPRINTF("eType = 0x%x\n", eType);
    //initial content for dummy packet
    #if 0
    _MVD_Memset(u32DummyES, 0xff, DUMMY_SIZE);
    #else
    //_miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32DummyES,temp);
    temp = u32DummyES+pMVDHalContext->stMiuCfg.u8FWMiuSel*HAL_MIU1_BASE;
    memset((void*)(MS_PA2KSEG1(temp)),0xff,DUMMY_SIZE);
    MsOS_FlushMemory();
    MsOS_ReadMemory();
    #endif

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
    VPRINTF("u32DummyES(0x%x)=0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", u32DummyES, HAL_MVD_MemRead4Byte(u32DummyES),
            HAL_MVD_MemRead4Byte(u32DummyES+4),HAL_MVD_MemRead4Byte(u32DummyES+8),HAL_MVD_MemRead4Byte(u32DummyES+12));
#endif

}

static void MVD_SLQTblInitDrvSlqTbl(MVD_SLQ_TBL_ST* pDrvSlqTbl, MS_VIRT u32Addr)
{
    MS_PHY temp = 0;
    pDrvSlqTbl->u32StAdd  = u32Addr;
    pDrvSlqTbl->u32EndAdd = u32Addr + SLQ_TBL_SIZE;
    pDrvSlqTbl->u32EntryCntMax = SLQ_ENTRY_MAX;

    //reset SLQ table read/write pointers
    pDrvSlqTbl->u32RdPtr = pDrvSlqTbl->u32StAdd;
    pDrvSlqTbl->u32WrPtr = pDrvSlqTbl->u32StAdd;
    pDrvSlqTbl->pu32LastEntry = &pDrvSlqTbl->u32WrPtr;

#if (!MVD_TURBO_INIT)
    //reset SLQ table
#if 0
    _MVD_Memset(pDrvSlqTbl->u32StAdd, 0, SLQ_TBL_SIZE);
#else
    //_miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,pDrvSlqTbl->u32StAdd,temp);
    temp = pDrvSlqTbl->u32StAdd+pMVDHalContext->stMiuCfg.u8FWMiuSel*HAL_MIU1_BASE;
    memset((void*)(MS_PA2KSEG1(temp)),0,SLQ_TBL_SIZE);
    MsOS_FlushMemory();
    MsOS_ReadMemory();
#endif
#endif
    pDrvSlqTbl->u32Empty = SLQ_TBL_SIZE;
    //_SLQTbl_DumpInfo(pDrvSlqTbl);
}

static void MVD_SLQTblInitSlqStatus(MVD_SLQ_STATUS* pSlqStatus, MS_VIRT u32WrPtr)
{
#if SLQ_NEW_PUSH
    pSlqStatus->u32VaildWptrAddr = u32WrPtr ;
    pSlqStatus->bSlqPicWaitNextStart = FALSE;
    pSlqStatus->bSlqCtrlBit = FALSE;
    pSlqStatus->u32SlqPushLength = 0;
    pSlqStatus->bSlqPicStart = FALSE;
    pSlqStatus->bSlqPicCollect = FALSE;
    pSlqStatus->bSlqEnLastFrameShow =FALSE;
    pSlqStatus->bSlqFireRdy = FALSE;
#endif
}

static void MVD_SLQTblInitDrvDivxTbl(MVD_SLQ_ES_ST* pDrvDivxTbl, MS_VIRT u32StAdd, MS_U32 u32Val)
{
    MS_PHY temp = 0;
    ///// init SLQ entries for DivX311
    pDrvDivxTbl->u32StAdd = u32StAdd;
    pDrvDivxTbl->u32EndAdd= pDrvDivxTbl->u32StAdd + SLQ_TBL_SIZE;
    pDrvDivxTbl->u32WrPtr = pDrvDivxTbl->u32StAdd;
    //pDrvDivxTbl->u32RdPtr = pDrvDivxTbl->u32StAdd;
#if (!MVD_TURBO_INIT)
    //reset DivX311 pattern table
#if 0
    _MVD_Memset(pDrvDivxTbl->u32StAdd, 0, u32Val);
#else
    //_miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,pDrvDivxTbl->u32StAdd,temp);
    temp = pDrvDivxTbl->u32StAdd+pMVDHalContext->stMiuCfg.u8FWMiuSel*HAL_MIU1_BASE;
    memset((void*)(MS_PA2KSEG1(temp)),0,u32Val);
    MsOS_FlushMemory();
    MsOS_ReadMemory();
#endif
#endif
}

static void MVD_SLQTblInitDrvEsTbl(MVD_SLQ_ES_ST* pDrvEsTbl, MS_VIRT u32StAdd, MS_U32 u32Val)
{
    UNUSED(u32Val);
    MS_PHY temp = 0;
    ///// init ES table
    pDrvEsTbl->u32StAdd = u32StAdd;
    pDrvEsTbl->u32EndAdd= pDrvEsTbl->u32StAdd + ES_TBL_SIZE;
    pDrvEsTbl->u32WrPtr = pDrvEsTbl->u32StAdd;
    //reset ES table
#if 0
    _MVD_Memset(pDrvEsTbl->u32StAdd, 0, ES_TBL_SIZE);
#else
    //_miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,pDrvEsTbl->u32StAdd,temp);
    temp = pDrvEsTbl->u32StAdd+pMVDHalContext->stMiuCfg.u8FWMiuSel*HAL_MIU1_BASE;
    memset((void*)(MS_PA2KSEG1(temp)),0,ES_TBL_SIZE);
    MsOS_FlushMemory();
    MsOS_ReadMemory();
#endif
}

//------------------------------------------------------------------------------------------------------------
// Layout of drvProcBuffer
//              -----------------
// drvProcBuff | SlqTbl entries  | <- pstSlqTblInfo->pDrvSlqTbl->u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             | DivX311         | <- pstSlqTblInfo->pDrvSlqTbl->u32EndAdd   <- pstSlqTblInfo->pDrvDivxTbl->u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             | Flush Patterns  |... <- pstSlqTblInfo->pDrvDivxTbl->u32EndAdd   <- _drvDummy.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             | FileEnd Pattern |... <- _drvDummy.u32EndAdd   <- _drvFileEnd.u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             |  ES table       |... <- _drvFileEnd.u32EndAdd   <- pstSlqTblInfo->pDrvEsTbl->u32StAdd
//             |     8K bytes    |
//             | (1024 entries)  |
//             |                 |
//             |-----------------|
//             |                 |........ End of drvProcBuff
//
//------------------------------------------------------------------------------------------------------------
MS_BOOL HAL_MVD_SLQTblInit(MS_U8 u8Idx)
{
    MS_VIRT u32Addr, u32Len;
    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    if(curCodecType == E_MVD_CODEC_UNKNOWN)
    {
        MVD_DEBUGERROR(VPRINTF("%s: unknow codec type!\n", __FUNCTION__));
        return FALSE;
    }
    pstSlqTblInfo->u32DummyPktCnt = 0;//reset dummy packet counter
    pstSlqTblInfo->u32SlqByteCnt = 0; //reset SLQ table byte counter
    pstSlqTblInfo->bSlqTblHasValidData = FALSE;

    HAL_MVD_MemGetMap(u8Idx, E_MVD_MMAP_DRV, &u32Addr, &u32Len);

    //init SLQ table attributes & reset SLQ table read/write pointers
    MVD_SLQTblInitDrvSlqTbl(pstSlqTblInfo->pDrvSlqTbl, u32Addr);
    pstSlqTblInfo->u32FileEndPtr = pstSlqTblInfo->pDrvSlqTbl->u32StAdd;

    MVD_SLQTblInitSlqStatus(pstSlqTblInfo->pSlqStatus, pstSlqTblInfo->pDrvSlqTbl->u32WrPtr);

    //set SLQ table start/end to F/W
    HAL_MVD_SetSLQTblBufStartEnd(u8Idx, HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr), HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr+SLQ_TBL_SIZE));

    ///// init SLQ entries for DivX311
    MVD_SLQTblInitDrvDivxTbl(pstSlqTblInfo->pDrvDivxTbl, pstSlqTblInfo->pDrvSlqTbl->u32EndAdd, SLQ_TBL_SIZE);

    ///// init flush pattern
    MVD_SLQTblInitDummyPkt(u8Idx, curCodecType);

    ///// init file-end pattern
    MVD_SLQTblInitFileEndPkt(u8Idx, curCodecType);

    MVD_SLQTblInitDrvEsTbl(pstSlqTblInfo->pDrvEsTbl, (pstSlqTblInfo->pDrvDivxTbl->u32EndAdd + DUMMY_SIZE*2), pstMemCfg->u32DrvBufSize);

    pstSlqTblInfo->u32PreEsRd = MVD_U32_MAX;    //reset ES read pointer
    pstSlqTblInfo->u32PreEsWr = 0;              //reset ES write pointer

    return TRUE;
}


#if 0
static void _SLQTbl_DumpInfo(MVD_SLQ_TBL_ST* pInfo)
{
    VPRINTF("str=0x%x\n", pInfo->u32StAdd);
    VPRINTF("end=0x%x\n", pInfo->u32EndAdd);
    VPRINTF("cnt=0x%x\n", pInfo->u32EntryCntMax);
    VPRINTF("rd =0x%x\n", pInfo->u32RdPtr);
    VPRINTF("wr =0x%x\n", pInfo->u32WrPtr);
    return;
}
#endif


MS_U32 HAL_MVD_GetQueueVacancy(MS_U8 u8Idx, MS_BOOL bCached)
{
    MS_U32 u32Empty;
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

#if 0
    if (MDrv_MVD_GetVldErrCount()!=0)
    {
        VPRINTF("QQQ wPtr= 0x%x(0x%x) rPtr=0x%x(0x%x) vldErr=0x%x\n", writePtrLast[u8Idx], pstSlqTblInfo->pDrvSlqTbl->u32WrPtr,
        HAL_MVD_GetSLQReadPtr(u8Idx), pstSlqTblInfo->pDrvSlqTbl->u32RdPtr, MDrv_MVD_GetVldErrCount(u8Idx));
        VPRINTF("Previous EsRead=0x%x EsWrite=0x%x\n", pstSlqTblInfo->u32PreEsRd, pstSlqTblInfo->u32PreEsWr);
        _SLQTbl_DumpPtsTbl(u8Idx, 0x0, 0x620);
        while(1);
    }
#endif

    u32Empty = pstSlqTblInfo->pDrvSlqTbl->u32Empty;
    if ((TRUE == bCached) && (u32Empty > SLQTBL_CHECKVACANCY_WATERLEVEL))
    {
        //To have better performance, we only query F/W read pointer
        //when queue_vacancy is lower than water_level
        if (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr > pstSlqTblInfo->pDrvSlqTbl->u32RdPtr)
        {
            u32Empty = SLQ_TBL_SIZE - (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr - pstSlqTblInfo->pDrvSlqTbl->u32RdPtr);
        }
        else
        {
            u32Empty = pstSlqTblInfo->pDrvSlqTbl->u32RdPtr - pstSlqTblInfo->pDrvSlqTbl->u32WrPtr;
        }

        if (u32Empty == 0)// && (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr == pstSlqTblInfo->pDrvSlqTbl->u32StAdd))
        {
            u32Empty = SLQ_TBL_SIZE;
        }
        u32Empty -= SLQ_TBL_SAFERANGE;
        return (u32Empty / SLQ_ENTRY_LEN);
    }

    pstSlqTblInfo->pDrvSlqTbl->u32RdPtr = HAL_MVD_Map2DrvSlqTbl(u8Idx, HAL_MVD_GetSLQReadPtr(u8Idx));
    //VPRINTF("QV=0x%x rd=0x%x==>", u32Empty, pstSlqTblInfo->pDrvSlqTbl->u32RdPtr);
    if (pstSlqTblInfo->pDrvSlqTbl->u32RdPtr >= (pstSlqTblInfo->pDrvSlqTbl->u32StAdd+SLQ_ENTRY_LEN))
    {
        if (pstSlqTblInfo->pDrvSlqTbl->u32RdPtr >= pstSlqTblInfo->pDrvSlqTbl->u32EndAdd)
        {
            MVD_DEBUGERROR(VPRINTF("%s: readPtr 0x%lx out of range: too large!\n",
                           __FUNCTION__, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32RdPtr));
        }
        pstSlqTblInfo->pDrvSlqTbl->u32RdPtr -= SLQ_ENTRY_LEN;
    }
    else if (pstSlqTblInfo->pDrvSlqTbl->u32RdPtr == pstSlqTblInfo->pDrvSlqTbl->u32StAdd)
    {
        pstSlqTblInfo->pDrvSlqTbl->u32RdPtr = pstSlqTblInfo->pDrvSlqTbl->u32EndAdd - SLQ_ENTRY_LEN;
    }
    else
    {
        MVD_DEBUGERROR(VPRINTF("%s: readPtr 0x%lx out of range: too small!\n",
                       __FUNCTION__, (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32RdPtr));
        pstSlqTblInfo->pDrvSlqTbl->u32RdPtr = pstSlqTblInfo->pDrvSlqTbl->u32StAdd;
    }
    //VPRINTF("0x%x\n", pstSlqTblInfo->pDrvSlqTbl->u32RdPtr);

    if (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr > pstSlqTblInfo->pDrvSlqTbl->u32RdPtr)
    {
        u32Empty = SLQ_TBL_SIZE - (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr - pstSlqTblInfo->pDrvSlqTbl->u32RdPtr);
    }
    else
    {
        u32Empty = pstSlqTblInfo->pDrvSlqTbl->u32RdPtr - pstSlqTblInfo->pDrvSlqTbl->u32WrPtr;
    }

    if (u32Empty == 0)// && (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr == pstSlqTblInfo->pDrvSlqTbl->u32StAdd))
    {
        u32Empty = SLQ_TBL_SIZE;
    }

    pstSlqTblInfo->pDrvSlqTbl->u32Empty = u32Empty;

    if (u32Empty < SLQ_TBL_SAFERANGE)
    {//to avoid write_pointer catch up to read_pointer
        u32Empty= 0;
    }
    else
    {
        u32Empty -= SLQ_TBL_SAFERANGE;
    }

    //VPRINTF("%s r=0x%x w=0x%x u32Empty=0x%x\n", __FUNCTION__,
    //        pstSlqTblInfo->pDrvSlqTbl->u32RdPtr, pstSlqTblInfo->pDrvSlqTbl->u32WrPtr, u32Empty);
    return (u32Empty / SLQ_ENTRY_LEN);
}

E_MVD_Result HAL_MVD_PushQueue(MS_U8 u8Idx, MVD_PacketInfo* pInfo)
{
    E_MVD_Result eRet=E_MVD_RET_INVALID_PARAM;
    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_U32 u32Id = MVD_GetStreamId(u8Idx);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
#ifdef VDEC3
    MS_VIRT u32VBBUAddr = HAL_MVD_GetVBBUTableAddr(u8VPUIdx);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
#endif
    if (pInfo == NULL)
    {
        MVD_DEBUGERROR(VPRINTF("PushQueue NULL pInfo\n"));
        return E_MVD_RET_INVALID_PARAM;
    }

#ifndef VDEC3
    if(pMVDHalContext->bAutoInsertDummyPattern[u8Idx] == TRUE)
#else
    if(pstCtrlCfg->u8McuMode == TRUE)
#endif
    {
        MS_U8* temp = (MS_U8*)(MS_PA2KSEG1( pInfo->u32StAddr+pstMemCfg->u32BSAddr));
        MS_U32 i;
        MS_U32 u32DummyPattern[3];
        MS_U32 u32PatternSize;

        switch (curCodecType)
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
                u32DummyPattern[1] = VC1_PATTERN_3;
                u32PatternSize = 2;
                break;
            case E_MVD_CODEC_VC1_MAIN: //rcv
                u32DummyPattern[0] = RCV_PATTERN_3;
                 u32DummyPattern[1] = RCV_PATTERN_1;
                 u32DummyPattern[2] = RCV_PATTERN_3;
                u32PatternSize = 3;
                break;

            default:
                u32DummyPattern[0] = DUMMY_PATTERN;
                u32DummyPattern[1] = DUMMY_PATTERN;
                u32PatternSize = 2;
                break;
        }


        for (i=0; i<u32PatternSize; i++)
        {
            temp[4*i+pInfo->u32Length] = ((u32DummyPattern[i])&0xff);
            temp[4*i+pInfo->u32Length+1] = ((u32DummyPattern[i]>>8)&0xff);
            temp[4*i+pInfo->u32Length+2] = ((u32DummyPattern[i]>>16)&0xff);
            temp[4*i+pInfo->u32Length+3] = ((u32DummyPattern[i]>>24)&0xff);
        }

        for(i=u32PatternSize*4;i<256;i++)
        {
            temp[pInfo->u32Length+i] = 0xFF;
        }

        pInfo->u32Length += 256;
    }


#if _SLQTBL_DUMP_PUSHQ
    {
        //static MS_U32 u32pqTimes[MAX_DEC_NUM]= {0, 0};

        MVD_DEBUGINFO(VPRINTF("Push[%x] st=0x%x len=0x%x pts=0x%x\n", pMVDHalContext->u32pqTimes[u8Idx]++,
                pInfo->u32StAddr, pInfo->u32Length, pInfo->u32TimeStamp));
    }
#endif

#if SLQ_NEW_PUSH
    if((pInfo->u32StAddr & SLQ_PIC_START_FLAG) == SLQ_PIC_START_FLAG)
    {
        pInfo->u32StAddr = pInfo->u32StAddr&~SLQ_PIC_START_FLAG;
        pstSlqTblInfo->pSlqStatus->bSlqPicStart =TRUE;
        pstSlqTblInfo->pSlqStatus->bSlqCtrlBit = TRUE;
    }
    else
    {
        pstSlqTblInfo->pSlqStatus->bSlqPicStart = FALSE;
    }
#endif
    //check input parameters
    if (pInfo->u32StAddr >= pstMemCfg->u32BSSize)
    {
        //since u32StAddr is offset, it shouldn't be larger than size.
        MVD_DEBUGERROR(VPRINTF("PushQueue invalid u32StAddr=0x%lx, bsSize=0x%lx\n", (unsigned long)pInfo->u32StAddr, (unsigned long)pstMemCfg->u32BSSize));
        return E_MVD_RET_INVALID_PARAM;
    }
    else if ((pInfo->u32TimeStamp == MVD_NULLPKT_PTS) && (pInfo->u32Length==0))
    {
        // AVI NULL packet.
        pstSlqTblInfo->u32DummyPktCnt++;
        //VPRINTF("Pos:0x%x%08x; PTS:%08d; NullPKT:%d\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp, pstSlqTblInfo->u32DummyPktCnt);
        return E_MVD_RET_OK;
    }

    if (FALSE == (HAL_MVD_IsMStreamerMode(u8Idx) ||HAL_MVD_IsMcuMode(u8Idx)))
    {   //Check repeat PTS for non-MStreamer/Mcu mode.
        //VPRINTF(".Pos:0x%x%08x; PTS:%08d; NullPKT:%d\n", pInfo->u32ID_H, pInfo->u32ID_L, pInfo->u32TimeStamp, pstSlqTblInfo->u32DummyPktCnt);
        if (pInfo->u32TimeStamp == pstSlqTblInfo->u32LastPts)
        {
            //VPRINTF("Repeat PTS!\n");
            if (pInfo->u32TimeStamp != MVD_NULLPKT_PTS)
                pInfo->u32TimeStamp = MVD_NULLPKT_PTS; //repeat PTS
        }
        else
        {
            pstSlqTblInfo->u32LastPts = pInfo->u32TimeStamp;
        }
    }

    //check queue vacancy
    if (pstSlqTblInfo->pDrvSlqTbl->u32Empty >= SLQ_TBL_SAFERANGE)
    {   //put packets
#if SLQ_NEW_PUSH
        if (E_MVD_CODEC_DIVX311 == curCodecType
            ||E_MVD_CODEC_VC1_MAIN == curCodecType //rcv
            ||E_MVD_CODEC_VC1_ADV == curCodecType)
        {
            MVD_PacketInfo stHdr;
            if((pstSlqTblInfo->pSlqStatus->bSlqCtrlBit) || (E_MVD_CODEC_DIVX311 == curCodecType))
            {
                MVD_SLQTblGetHdrPkt(u8Idx, &stHdr, pInfo);

                if(pstSlqTblInfo->pSlqStatus->bSlqPicStart||(!pstSlqTblInfo->pSlqStatus->bSlqCtrlBit))
                {
                    if (HAL_MVD_IsMStreamerMode(u8Idx) || HAL_MVD_IsMcuMode(u8Idx))
                    {   //to mark this packet's pts as unused.
                        stHdr.u32TimeStamp = MVD_NULLPKT_PTS;
                        stHdr.u32ID_H = MVD_NULLPKT_PTS;
                    }
#ifdef VDEC3
                    if(pstCtrlCfg->u8McuMode == TRUE)
                    {
                        if(HAL_VPU_EX_GetVBBUVacancy(u32VBBUAddr) > 1)
                        {
                            HAL_VPU_EX_PacketInfo stVpuPkt;

                            stVpuPkt.u32Offset = stHdr.u32StAddr;
                            stVpuPkt.u32Length = stHdr.u32Length;
                            stVpuPkt.u64TimeStamp = (MS_U64)stHdr.u32TimeStamp;
                            //If in display queue mode
                            stVpuPkt.u64TimeStamp |= ((MS_U64)stHdr.u32ID_H << 32);
                            stVpuPkt.u32ID_H = stHdr.u32ID_H;
                            stVpuPkt.u32ID_L = stHdr.u32ID_L;

                            pstSlqTblInfo->bSlqTblHasValidData = TRUE;
                            eRet = (E_MVD_Result)HAL_VPU_EX_Push2VBBU(u32Id, &stVpuPkt, u32VBBUAddr);
                        }
                        else
                        {
                            return E_MVD_RET_FAIL;
                        }
                    }
                    else
#endif
                    {
                        HAL_MVD_SLQTblSendPacket(u8Idx, &stHdr);
                    }
                }
            }
        }
 #else
        if (E_MVD_CODEC_DIVX311 == curCodecType)
        {
            MVD_PacketInfo stDivxHdr;
            MVD_SLQTblGetDivxHdrPkt(u8Idx, &stDivxHdr, pInfo);
            if (HAL_MVD_IsMStreamerMode(u8Idx) || HAL_MVD_IsMcuMode(u8Idx))
            {   //to mark this packet's pts as unused.
                stDivxHdr.u32TimeStamp = MVD_NULLPKT_PTS;
                stDivxHdr.u32ID_H = MVD_NULLPKT_PTS;
            }
            HAL_MVD_SLQTblSendPacket(u8Idx, &stDivxHdr);
        }
#endif

#ifdef VDEC3
        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            if(HAL_VPU_EX_GetVBBUVacancy(u32VBBUAddr) > 0)
            {
                HAL_VPU_EX_PacketInfo stVpuPkt;

                stVpuPkt.u32Offset = pInfo->u32StAddr;
                stVpuPkt.u32Length = pInfo->u32Length;
                stVpuPkt.u64TimeStamp = (MS_U64)pInfo->u32TimeStamp;
                 //If in display queue mode
                stVpuPkt.u64TimeStamp |= ((MS_U64)pInfo->u32ID_H << 32);
                stVpuPkt.u32ID_H = pInfo->u32ID_H;
                stVpuPkt.u32ID_L = pInfo->u32ID_L;

                pstSlqTblInfo->bSlqTblHasValidData = TRUE;
                eRet = (E_MVD_Result)HAL_VPU_EX_Push2VBBU(u32Id, &stVpuPkt, u32VBBUAddr);
                //VPRINTF("[%d]%s[%d],ret=%d,u32StAddr %x,u32Length %d,u32TimeStamp %d\n",u8Idx,__FUNCTION__,__LINE__,eRet,(unsigned int)pInfo->u32StAddr,(unsigned int)pInfo->u32Length,(unsigned int)pInfo->u32TimeStamp);
                return eRet;
            }
            else
            {
                return E_MVD_RET_FAIL;
            }
        }
        else
#endif
        {
             HAL_MVD_SLQTblSendPacket(u8Idx, pInfo);
             eRet=E_MVD_RET_OK;
             pstSlqTblInfo->bSlqTblHasValidData = TRUE;
        }
    }
    else
    {
        MS_ASSERT(0); //shouldn't be here!
        MVD_DEBUGERROR(VPRINTF("PushQueue FULL!!! empty=0x%lx\n", (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32Empty));
        //Player will only push queue when queue vacancy != 0
        eRet=E_MVD_RET_QUEUE_FULL;
    }

    if (E_MVD_RET_OK != eRet)
    {
        MVD_DEBUGERROR(VPRINTF("%s ret = %d\n", __FUNCTION__, eRet));
    }
    return eRet;
}


static E_MVD_Result MVD_FlushTSQueue(MS_U8 u8Idx)
{
    if (HAL_MVD_GetLastCmd(u8Idx)!=CMD_PAUSE)
    {
        HAL_MVD_PauseDisp(u8Idx);
        HAL_MVD_DecodePause(u8Idx);
        HAL_MVD_FlushDisplayBuf(u8Idx);
    }

    if(HAL_MVD_SkipData(u8Idx) == FALSE)
    {
        return E_MVD_RET_FAIL;
    }
    else
    {
        return E_MVD_RET_OK;
    }
}


static MS_BOOL MVD_PatternLenIsValid(MS_U8 u8Idx, MS_U32 u32Len)
{
    MS_U32 u32ValidLen = 0;
    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);
#define MAX_VALIDLEN    0x200000    //2M

    if (E_MVD_CODEC_VC1_MAIN != curCodecType)
    {
        return TRUE;
    }
    else
    {
        //only RCV has to check this
        u32ValidLen = HAL_MVD_GetPayloadLen(u8Idx);
        if (u32ValidLen > MAX_VALIDLEN)
        {   //avoid the extreme large value due to error bitstream
            u32ValidLen = MAX_VALIDLEN;
        }
        MVD_DEBUGINFO(VPRINTF("(%x) ValidLen=0x%x CurLen=0x%x\n",
                       (u32Len > u32ValidLen), u32ValidLen, u32Len));
        return (u32Len > u32ValidLen);
    }
}

static E_MVD_Result MVD_FlushSlqTblQueue(MS_U8 u8Idx)
{
    MS_U32 u32TimeCnt;
    MS_U32 u32PatternByteCnt = 0;
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_U32 u32Id = MVD_GetStreamId(u8Idx);
    MS_U8 u8VPUIdx = HAL_VPU_EX_GetTaskId(u32Id);
#ifdef VDEC3
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    MS_VIRT u32VBBUAddr = HAL_MVD_GetVBBUTableAddr(u8VPUIdx);
#endif
    HAL_MVD_ResetHandShake(u8Idx,MVD_HANDSHAKE_PAUSE);
    HAL_MVD_PauseDisp(u8Idx);
    HAL_MVD_DecodePause(u8Idx);
    HAL_MVD_FlushDisplayBuf(u8Idx);

    u32TimeCnt = HAL_MVD_GetTime();
    while (((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS) ||
           (TRUE != MVD_PatternLenIsValid(u8Idx, u32PatternByteCnt)))
    {
        if (HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_PAUSE))
        {
            MVD_DEBUGINFO(VPRINTF("\nPause finished!\n"));
            break;
        }

#ifdef VDEC3
        if(pstCtrlCfg->u8McuMode == TRUE)
        {
            if(HAL_VPU_EX_GetVBBUVacancy(u32VBBUAddr) > 0)
            {
                HAL_VPU_EX_PacketInfo stVpuPkt;
                MVD_PacketInfo stDummyPkt;
                MVD_SLQTblGetDummyPkt(&stDummyPkt);

                stVpuPkt.u32Offset = stDummyPkt.u32StAddr;
                stVpuPkt.u32Length = stDummyPkt.u32Length;
                stVpuPkt.u64TimeStamp = (MS_U64)stDummyPkt.u32TimeStamp;
                //If in display queue mode
                stVpuPkt.u64TimeStamp |= ((MS_U64)stDummyPkt.u32ID_H << 32);
                stVpuPkt.u32ID_H = stDummyPkt.u32ID_H;
                stVpuPkt.u32ID_L = stDummyPkt.u32ID_L;

                HAL_VPU_EX_Push2VBBU(u32Id, &stVpuPkt,u32VBBUAddr);
            }
        }
        else
#endif
        {
            if (pstSlqTblInfo->pDrvSlqTbl->u32Empty < SLQ_TBL_SAFERANGE)
            {
                HAL_MVD_GetQueueVacancy(u8Idx, FALSE); //update pstSlqTblInfo->pDrvSlqTbl->u32Empty
                HAL_MVD_Delayms(1);  //avoid busy query
            }
            //insert dummy pattern
            if (TRUE == HAL_MVD_SLQTblInsertPattern(u8Idx, E_MVD_PATTERN_FLUSH))
            {
                u32PatternByteCnt += DUMMY_SIZE*2; //2 dummy were inserted
            }
        }


        //Timeout for RCV
        if ((HAL_MVD_GetTime() - u32TimeCnt) > (CMD_TIMEOUT_MS*10))
        {
            MVD_DEBUGERROR(VPRINTF("RCV timeout!!! pl=%d, pbc=%d, emp=%ld\n",
                           HAL_MVD_GetPayloadLen(u8Idx), u32PatternByteCnt,
                           (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32Empty));
            break;
        }
    }
    MVD_DEBUGINFO(VPRINTF("====> %s (t1=%u t2=%u diff=%u)\n", __FUNCTION__,
            u32TimeCnt, HAL_MVD_GetTime(), (HAL_MVD_GetTime() - u32TimeCnt)));
    //if ((HAL_MVD_GetTime() - u32TimeCnt) >= CMD_TIMEOUT_MS)
    if (TRUE != HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_PAUSE))
    {
        MVD_DEBUGERROR(VPRINTF("\n***** MDrv_MVD_FlushQueue PAUSE TIMEOUT!!! *****\n\n"));
        MVD_DEBUGERROR(VPRINTF("ValidLen=0x%x CurPatternLen=0x%x\n",
                      HAL_MVD_GetPayloadLen(u8Idx), u32PatternByteCnt));

        return E_MVD_RET_FAIL;
    }

    //flush ES buffer & reset SLQ tbl
    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_SLQ_RST);
    if (HAL_MVD_SlqTblRst(u8Idx) == TRUE)
    {
        //return E_MVD_RET_OK;
    }

    u32TimeCnt = HAL_MVD_GetTime();
    while (((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS))
    {
        if (HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_SLQ_RST))
        {
            MVD_DEBUGINFO(VPRINTF("\nSlqRst finished!\n"));
            break;
        }
    }
    MVD_DEBUGINFO(VPRINTF("====> %s (t1=%u t2=%u diff=%u)\n", __FUNCTION__,
            u32TimeCnt, HAL_MVD_GetTime(), (HAL_MVD_GetTime() - u32TimeCnt)));
    if (TRUE != HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_SLQ_RST))
    {
        MVD_DEBUGERROR(VPRINTF("\n***** MDrv_MVD_FlushQueue SlqRst TIMEOUT!!! *****\n\n"));
        return E_MVD_RET_FAIL;
    }

    HAL_MVD_SLQTblInit(u8Idx); //reset related buffers

    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_FlushQueue(MS_U8 u8Idx)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8Idx);

    if (HAL_MVD_GetDecodeStatus(u8Idx) == E_MVD_STAT_IDLE)
    {
        return eRet;
    }

    //flush ES buffer (and cmd queue if used)
    if (E_MVD_SLQ_TBL_MODE == curSrcMode)
    {
        eRet = MVD_FlushSlqTblQueue(u8Idx);
    }
    else if (E_MVD_TS_FILE_MODE == curSrcMode)
    {
        eRet = MVD_FlushTSQueue(u8Idx);
    }
    if (E_MVD_RET_OK != eRet)
    {
        return eRet;
    }

    MVD_RstFrmInfo(u8Idx, E_MVD_FRMINFO_DECODE);
    //flush display buffer

    if (HAL_MVD_FlushDisplayBuf(u8Idx) != TRUE)
    {
        return E_MVD_RET_FAIL;
    }

    if (TRUE == HAL_MVD_SkipToIFrame(u8Idx))
    {
        return E_MVD_RET_OK;
    }
    else
    {
        return E_MVD_RET_FAIL;
    }
}

MS_BOOL HAL_MVD_IsAllBufferEmpty(MS_U8 u8Idx)
{
    return HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_SKIP_DATA);
}

MS_BOOL HAL_MVD_EnableInt(MS_U8 u8Idx, MS_U32 bEn)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32IntFlag = 0;

    u32IntFlag = (((bEn & E_MVD_EVENT_USER_DATA) == E_MVD_EVENT_USER_DATA) ? INT_USER_DATA : 0) |
                 (((bEn & E_MVD_EVENT_USER_DATA_DISP) == E_MVD_EVENT_USER_DATA_DISP) ? INT_USER_DATA_DISP : 0) |
                 (((bEn & E_MVD_EVENT_PIC_FOUND) == E_MVD_EVENT_PIC_FOUND) ? INT_PIC_FOUND : 0) |
                 (((bEn & E_MVD_EVENT_FIRST_FRAME) == E_MVD_EVENT_FIRST_FRAME) ? INT_FIRST_FRAME : 0) |
                 (((bEn & E_MVD_EVENT_DISP_RDY) == E_MVD_EVENT_DISP_RDY) ? INT_DISP_RDY : 0) |
                 (((bEn & E_MVD_EVENT_SEQ_FOUND) == E_MVD_EVENT_SEQ_FOUND) ? INT_SEQ_FOUND : 0)|
                 (((bEn & E_MVD_EVENT_DEC_ONE_FRAME) == E_MVD_EVENT_DEC_ONE_FRAME) ? INT_DEC_DONE : 0)|
                 (((bEn & E_MVD_EVENT_DEC_ERR) == E_MVD_EVENT_DEC_ERR) ? INT_DEC_ERR : 0)|
                 (((bEn & E_MVD_EVENT_DEC_DATA_ERR) == E_MVD_EVENT_DEC_DATA_ERR) ? INT_VES_INVALID : 0)|
                 (((bEn & E_MVD_EVENT_XC_LOW_DEALY) == E_MVD_EVENT_XC_LOW_DEALY) ? INT_XC_LOW_DELAY : 0)|
                 (((bEn & E_MVD_EVENT_DEC_I) == E_MVD_EVENT_DEC_I) ? INT_DEC_I : 0);
    if (((bEn & E_MVD_EVENT_DISP_VSYNC) == E_MVD_EVENT_DISP_VSYNC) ||
        ((bEn & E_MVD_EVENT_UNMUTE) == E_MVD_EVENT_UNMUTE))
    {
        u32IntFlag |= INT_DISP_VSYNC;
    }

    MVD_DEBUGINFO(VPRINTF("u32IntFlag = 0x%x\n", u32IntFlag));
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u32IntFlag & 0xff;
    mvdcmd.Arg1 = (u32IntFlag>>8) & 0xff;
    mvdcmd.Arg2 = (u32IntFlag>>16) & 0xff;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_ENABLE_INT_STAT, &mvdcmd);

    return TRUE;
}

E_MVD_Result HAL_MVD_EnableDispOneField(MS_U8 u8Idx, MS_BOOL bEn)
{
    MVD_CmdArg stCmdArg;

    SETUP_CMDARG(stCmdArg);
    if(TRUE == bEn)
    {
        stCmdArg.Arg0 = 1;
    }
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_SHOW_ONE_FIELD, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SHOW_ONE_FIELD ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_SetFdMaskDelayCount(MS_U8 u8Idx, MS_U16 u16Cnt)
{
    MVD_CmdArg stCmdArg;

    SETUP_CMDARG(stCmdArg);
    //16bits and unit in vsync for mute the fd_mask
    stCmdArg.Arg0 = u16Cnt & 0xff;
    stCmdArg.Arg1 = (u16Cnt>>8) & 0xff;
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_FD_MASK_DELAY_CNT, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FD_MASK_DELAY_CNT ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_SetOutputFRCMode(MS_U8 u8Idx, MS_U8 u8FrameRate, MS_U8 u8Interlace)
{
    MVD_CmdArg stCmdArg;
    if((u8Interlace != 0) && (u8Interlace != 1))
    {
        return E_MVD_RET_FAIL;
    }
    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8FrameRate;
    stCmdArg.Arg1 = u8Interlace;
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_FRC_OUPUT, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FRC_OUPUT ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}


E_MVD_Result HAL_MVD_SetFRCDropType(MS_U8 u8Idx, MS_U8 u8DropType)
{
    MVD_CmdArg stCmdArg;

    if((u8DropType != FRC_DROP_FRAME)
    && (u8DropType != FRC_DROP_FIELD))
    {
        return E_MVD_RET_FAIL;
    }

    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8DropType;
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_FRC_DROP_BEHAVIOR, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FRC_DROP_BEHAVIOR ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_SetDisableSeqChange(MS_U8 u8Idx, MS_BOOL bEnable)
{
    MVD_CmdArg stCmdArg;

    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = bEnable;
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_FORBID_RESOLUTION_CHANGE, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_FORBID_RESOLUTION_CHANGE ) );
        return E_MVD_RET_FAIL;
    }
    MVD_DEBUGINFO(VPRINTF("MVD CMD_FORBID_RESOLUTION_CHANGE(0x%x) OK\n", bEnable));

    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_DbgGetData(MS_VIRT u32Addr, MS_U32* u32Data)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32Val;

    if (!u32Data)
    {
        return E_MVD_RET_INVALID_PARAM;
    }

    SET_CMDARG(mvdcmd, u32Addr, 0);  //FIXME
    if (HAL_MVD_MVDCommand( CMD_RD_IO, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_RD_IO ) );
        return E_MVD_RET_FAIL;
    }

    u32Val = (((MS_U32)mvdcmd.Arg2)) | ((MS_U32)mvdcmd.Arg3 << 8) |
             (((MS_U32)mvdcmd.Arg4) << 16) | (((MS_U32)mvdcmd.Arg5) << 24);
    *u32Data = u32Val;

    return E_MVD_RET_OK;
}


void HAL_MVD_DbgDumpBits(MS_U8 u8Idx, MS_PHY u32base, MS_U32 u32size)
{
    MVD_CmdArg mvdcmd;

    u32base = HAL_MVD_GetMemOffset(u32base);
    MVD_DEBUGINFO(VPRINTF("%s base=0x%lx size=0x%x\n", __FUNCTION__, (unsigned long)u32base, u32size));
    MS_ASSERT((u32base%8)==0);
    u32base >>= 3;
    SET_CMDARG(mvdcmd, u32base, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DUMP_BITSTREAM_BASE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\n", CMD_DUMP_BITSTREAM_BASE ) );
        return;
    }

    MS_ASSERT((u32size%8)==0);
    u32size >>= 3;
    SET_CMDARG(mvdcmd, u32size, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DUMP_BITSTREAM_LENGTH, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\n", CMD_DUMP_BITSTREAM_LENGTH ) );
        return;
    }
    return;
}


MS_U8 HAL_MVD_GetActiveFormat(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    MS_U8 u8Afd = 0;

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_GET_AFD, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail!!\r\n", CMD_GET_AFD  ) );
        return 0xff;
    }
    u8Afd = mvdcmd.Arg0;
    return u8Afd;
}

MS_BOOL HAL_MVD_EnableAVSync(MS_U8 u8Idx, MS_BOOL bEnable)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SYNC_ON, &mvdcmd);

    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    pCtrlCfg->bAVSyncOn = bEnable;

    return TRUE;
}

MS_BOOL HAL_MVD_SetAVSyncDelay(MS_U8 u8Idx, MS_U32 u32Delay)
{
    MVD_CmdArg mvdcmd;
    SET_CMDARG(mvdcmd, _MS_TO_90K(u32Delay), u8Idx); //u32Delay ms ==> 90k counter
    SET_CMD_RET_FALSE(CMD_SYNC_OFFSET, &mvdcmd);
    return TRUE;
}

MS_BOOL HAL_MVD_SetAVSyncThreshold(MS_U8 u8Idx, MS_U32 u32Th)
{
    MVD_CmdArg mvdcmd;

    if (u32Th == 0x00)
    {
        return FALSE; //invalid parameter, do nothing
    }
    if (u32Th > 0xff)
    {
        u32Th = 0xff; //set to maximum
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg3 = u32Th;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SYN_THRESHOLD, &mvdcmd);
    return TRUE;
}

MS_BOOL HAL_MVD_SetAVSyncFreerunThreshold(MS_U8 u8Idx, MS_U32 u32Th)
{
    MVD_CmdArg mvdcmd;
    SET_CMDARG(mvdcmd, u32Th, u8Idx);
    SET_CMD_RET_FALSE(CMD_AVSYNC_FREERUN_THRESHOLD, &mvdcmd);
    return TRUE;
}

MS_BOOL HAL_MVD_ChangeAVsync(MS_U8 u8Idx, MS_BOOL bEnable, MS_U16 u16PTS)
{
    MVD_CmdArg mvdcmd;

    u16PTS = _MS_TO_90K(u16PTS); //u16PTS ms ==> 90k counter
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = (MS_U8)bEnable;
    mvdcmd.Arg1 = 0;
    mvdcmd.Arg2 = (MS_U8)(u16PTS&0xff);
    mvdcmd.Arg3 = (MS_U8)((u16PTS&0xff00)>>8);
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_MVD_FAST_INT, &mvdcmd);
    return TRUE;
}

MS_BOOL HAL_MVD_GetIsSyncRep(MS_U8 u8Idx)
{
    MS_U32 u32IntStat = 0;
    MS_BOOL bRet = FALSE;

    u32IntStat = HAL_MVD_GetIntState(u8Idx);
    if (u32IntStat != 0)
    {
        bRet = ((u32IntStat&INT_SYN_REP)==INT_SYN_REP) ? TRUE : FALSE;
    }
    return bRet;
}

MS_BOOL HAL_MVD_GetIsSyncSkip(MS_U8 u8Idx)
{
    MS_U32 u32IntStat = 0;
    MS_BOOL bRet = FALSE;

    u32IntStat = HAL_MVD_GetIntState(u8Idx);
    if (u32IntStat != 0)
    {
        bRet = ((u32IntStat&INT_SYN_SKIP)==INT_SYN_SKIP) ? TRUE : FALSE;
    }

    return bRet;
}

MS_U8 HAL_MVD_GetIsSyncReach(MS_U8 u8Idx)
{
#define MVD_SYNC_DONE    1
    if(MVD_GetSyncStat(u8Idx, 1) == MVD_SYNC_DONE)
        return 1;
    else
        return 0xFF;
}

MS_U8 HAL_MVD_GetDispRdy(MS_U8 u8Idx)
{
    if (HAL_MVD_GetPicCounter(u8Idx) > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

MVD_DecStat HAL_MVD_GetDecodeStatus(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    MVD_DecStat stat = E_MVD_STAT_UNKNOWN;

    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_DECODE_STATUS, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DECODE_STATUS ) );
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
}

MS_U8 HAL_MVD_GetLastCmd(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DECODE_STATUS, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DECODE_STATUS ) );
        return 0xff;
    }

    return (mvdcmd.Arg0);
}

MS_U32 HAL_MVD_GetParserByteCnt(MS_U8 u8Idx)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32Cnt = 0;

    SETUP_CMDARG(mvdcmd);

    //To be accurate, it's "VLD byte count", instead of "parser byte count".
    mvdcmd.Arg0 = 0x34;
    mvdcmd.Arg1 = 0x2;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_RD_IO, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail!!\r\n", CMD_RD_IO  ) );
        return 0;
    }
    u32Cnt = (((MS_U32)mvdcmd.Arg2)) | ((MS_U32)mvdcmd.Arg3 << 8) |
             (((MS_U32)mvdcmd.Arg4) << 16) | (((MS_U32)mvdcmd.Arg5) << 24);

    //VPRINTF("  parser byte count = %lu byte \n", u32Cnt);
    return u32Cnt;
}

MS_BOOL HAL_MVD_DropErrorFrame(MS_U8 u8Idx, MS_BOOL bDrop)
{
    MVD_CmdArg mvdcmd;
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg1 = (MS_U8)bDrop;
    mvdcmd.Arg2 = (MS_U8)pstCtrlCfg->bDropDispfrm;
    mvdcmd.Arg3 = (MS_U8)pstCtrlCfg->eFrcMode;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_DISPLAY_CTL, &mvdcmd);
    return TRUE;
}

void HAL_MVD_SetDivXCfg(MS_U8 u8Idx, MS_U8 u8MvAdjust, MS_U8 u8IdctSel)
{
    MVD_CmdArg stCmdArg;

    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8MvAdjust;
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_DIVX_PATCH, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DIVX_PATCH ) );
        return;
    }

    SETUP_CMDARG(stCmdArg);
    stCmdArg.Arg0 = u8IdctSel;
    SET_DECNUM(stCmdArg, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_IDCT_SEL, &stCmdArg) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_IDCT_SEL ) );
        return;
    }

    return;
}

#define _IsNotInStreamBuff(x)                                       \
        (((x) < pstMemCfg->u32DrvBufSize) ||   \
         ((x) > pstMemCfg->u32BSSize) )

//------------------------------------------------------------------------------
/// Get read pointer in ElementaryStream buffer for SLQ table mode
/// @return -the read pointer
//------------------------------------------------------------------------------
MS_VIRT MVD_GetSlqTblESReadPtr(MS_U8 u8Idx)
{
    MS_U32 u32Idx;
    MS_U32 u32SlqRp = 0, u32SlqRp1 = 0;
    MS_VIRT u32EsRp;
    MS_VIRT u32EsStart;
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);

    u32SlqRp1 = HAL_MVD_GetSLQReadPtr(u8Idx);

    if (u32SlqRp1 == 0)
    {
        return MVD_U32_MAX;
    }
    else
    {
        u32SlqRp = HAL_MVD_Map2DrvSlqTbl(u8Idx, u32SlqRp1);
    }

    pstSlqTblInfo->pDrvSlqTbl->u32RdPtr = u32SlqRp; //update pstSlqTblInfo->pDrvSlqTbl->u32RdPtr

    //report (readPtr-1) for HW may still use (readPtr)
    if (u32SlqRp > (pstSlqTblInfo->pDrvSlqTbl->u32StAdd))
    {
        u32Idx = ((u32SlqRp - pstSlqTblInfo->pDrvSlqTbl->u32StAdd)/SLQ_ENTRY_LEN) - 1;
    }
    else
    {
        u32Idx = SLQ_ENTRY_MAX - 1;
    }
    u32EsRp = pstSlqTblInfo->pDrvEsTbl->u32StAdd + u32Idx*8;

    u32EsStart = HAL_MVD_MemRead4Byte(u32EsRp); //report StartAdd as read_pointer
#if 0
    MS_U32 u32EsEnd;
    u32EsEnd = HAL_MVD_MemRead4Byte(u32EsRp+4);
    VPRINTF("GetESReadPtr ES[%x] Start=0x%x End=0x%x u32EsRp=%x u32SlqRp=%x\n",
            u32Idx, HAL_MVD_MemRead4Byte(u32EsRp), u32EsEnd, u32EsRp, u32SlqRp);
#endif

    if ((_IsNotInStreamBuff(u32EsStart)) && (u32EsStart != 0))
    {   //ESRead is not in BS buffer, so this entry is a divx or dummy pattern.
        //Report the last ESRead, instead of this one.
        if(curCodecType == E_MVD_CODEC_DIVX311)
        {
            //update last slq index es end address
            if(u32Idx == 0)
            {
                u32Idx = SLQ_ENTRY_MAX-1;
            }
            else
            {
                u32Idx = u32Idx-1;
            }
            u32EsRp = pstSlqTblInfo->pDrvEsTbl->u32StAdd + u32Idx*8;
            u32EsStart = HAL_MVD_MemRead4Byte(u32EsRp); //report StartAdd as read_pointer
            if(_IsNotInStreamBuff(u32EsStart))
            {
                return (pstSlqTblInfo->u32PreEsRd);
            }
        }
        else
        {
            MVD_DEBUGINFO(VPRINTF("0x%lx Not in BS, report u32PreEsRd=0x%lx\n", (unsigned long)u32EsStart, (unsigned long)pstSlqTblInfo->u32PreEsRd));
            return (pstSlqTblInfo->u32PreEsRd);
        }
    }
    pstSlqTblInfo->u32PreEsRd = u32EsStart;

    return u32EsStart;
}

//------------------------------------------------------------------------------
/// Get write pointer in ElementaryStream buffer for SLQ table mode
/// @return -the read pointer
//------------------------------------------------------------------------------
MS_VIRT MVD_GetSlqTblESWritePtr(MS_U8 u8Idx)
{
    MS_VIRT u32EsWp;
    MS_VIRT u32EsEnd;
    MS_U32 u32Idx;
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    if (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr > (pstSlqTblInfo->pDrvSlqTbl->u32StAdd))
    {
        u32Idx = ((pstSlqTblInfo->pDrvSlqTbl->u32WrPtr - pstSlqTblInfo->pDrvSlqTbl->u32StAdd)/SLQ_ENTRY_LEN) - 1;
    }
    else
    {
        u32Idx = SLQ_ENTRY_MAX - 1;
    }
    u32EsWp = pstSlqTblInfo->pDrvEsTbl->u32StAdd + u32Idx*8;

    u32EsEnd = HAL_MVD_MemRead4Byte(u32EsWp+4);
#if 0
    VPRINTF("GetESWritePtr[%x] ES Start=0x%x End=0x%x u32EsWp=%x\n",
        (u32EsWp - pstSlqTblInfo->pDrvEsTbl->u32StAdd)/8,
        HAL_MVD_MemRead4Byte(u32EsWp), u32EsEnd, u32EsWp);
#endif

    if ((_IsNotInStreamBuff(u32EsEnd)) && (u32EsEnd != 0))
    {   //ESRead is not in BS buffer, so this entry is a divx pattern.
        //Report the last ESRead, instead of this one.
        MVD_DEBUGINFO(VPRINTF("0x%lx Not in BS, report u32PreEsWr=0x%lx\n", (unsigned long)u32EsEnd, (unsigned long)pstSlqTblInfo->u32PreEsWr));
        return (pstSlqTblInfo->u32PreEsWr);
    }
    pstSlqTblInfo->u32PreEsWr = u32EsEnd;

    return u32EsEnd;
}


MS_VIRT HAL_MVD_GetSLQReadPtr(MS_U8 u8Idx)
{
    MS_VIRT u32RdPtr = 0;

    if (!(pMVDHalContext->bSlqTblHKCtrl[u8Idx]))
    {
        #ifndef VDEC3
        MVD_CmdArg mvdcmd;
        SETUP_CMDARG(mvdcmd);
        SET_DECNUM(mvdcmd, u8Idx);
        if (HAL_MVD_MVDCommand( CMD_SLQ_GET_TBL_RPTR, &mvdcmd ) == FALSE)
        {
            MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SLQ_GET_TBL_RPTR ) );
            return 0;
        }
        u32RdPtr = mvdcmd.Arg0 | (mvdcmd.Arg1<<8) | (mvdcmd.Arg2<<16) | (mvdcmd.Arg3<<24);
        #else
        u32RdPtr = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SLQ_TBL_RPTR, sizeof(MS_U32));
        #endif
    }
    else
    {
        u32RdPtr = HAL_MVD_SlqTblProbeRdPtr(u8Idx);
    }

    //VPRINTF("##### HAL_MVD_GetSLQReadPtr 0x%x\n", readPtr);

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


MS_BOOL HAL_MVD_SetSLQWritePtr(MS_U8 u8Idx, MS_BOOL bCheckData)
{
    MVD_CmdArg mvdcmd;
    MS_VIRT u32WrPtr;
    MVD_MEMCfg* pstMemCfg = HAL_MVD_GetMEMCfg(u8Idx);
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);

    if (FALSE == pstSlqTblInfo->bRdyToFireCmd)
    {
        //check FW is init success and thus ready to update write pointer
        MS_U32 u32TimeCnt;

        u32TimeCnt = HAL_MVD_GetTime();
        while ((HAL_MVD_GetTime() - u32TimeCnt) < CMD_TIMEOUT_MS)
        {
            if (HAL_MVD_GetDecodeStatus(u8Idx) != E_MVD_STAT_IDLE)
            {
                pstSlqTblInfo->bRdyToFireCmd = TRUE;
                MVD_DEBUGVERBAL(VPRINTF("time=0x%x ms, ", (HAL_MVD_GetTime() - u32TimeCnt)));
                break;
            }
        }
        if (FALSE == pstSlqTblInfo->bRdyToFireCmd)
        {
            MVD_DEBUGERROR(VPRINTF("%s: err timeout(%d)! stat=0x%x\n", __FUNCTION__,
                    HAL_MVD_GetTime() - u32TimeCnt, HAL_MVD_GetDecodeStatus(u8Idx)));
            return FALSE;
        }
        else
        {
            MVD_DEBUGVERBAL(VPRINTF("%s: ready to update WrPtr.\n", __FUNCTION__));
        }
    }


    MS_ASSERT(pstSlqTblInfo->pDrvSlqTbl->u32WrPtr < pstSlqTblInfo->pDrvSlqTbl->u32EndAdd);
    MS_ASSERT(pstSlqTblInfo->pDrvSlqTbl->u32WrPtr >= pstSlqTblInfo->pDrvSlqTbl->u32StAdd);
#if SLQ_NEW_PUSH
    if((!pstSlqTblInfo->pSlqStatus->bSlqFireRdy)&&pstSlqTblInfo->pSlqStatus->bSlqCtrlBit)
    {
        MVD_DEBUGINFO(VPRINTF("**** pstSlqTblInfo->pSlqStatus->bSlqFireRdy is not Ready ** \n"));
        return FALSE;
    }
    pstSlqTblInfo->pSlqStatus->bSlqFireRdy = FALSE;
#endif
    if ((bCheckData==TRUE) && (FALSE==pstSlqTblInfo->bSlqTblHasValidData))
    {
        MVD_DEBUGINFO(VPRINTF("**** SlqWrPtr(0x%lx) is not update!(%x, %x) ****\n",
            (unsigned long)pstSlqTblInfo->pDrvSlqTbl->u32WrPtr, bCheckData, pstSlqTblInfo->bSlqTblHasValidData));
        return FALSE;
    }
    pstSlqTblInfo->bSlqTblHasValidData = FALSE;

    if (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr != pstSlqTblInfo->pDrvSlqTbl->u32EndAdd)
    {
        u32WrPtr = pstSlqTblInfo->pDrvSlqTbl->u32WrPtr;
    }
    else
    {
        u32WrPtr = pstSlqTblInfo->pDrvSlqTbl->u32StAdd;
    }

    if (pstCtrlCfg->bSlqTblSync)
    {
        //Update SLQ table, DivX311 patterns, and dummy patterns to bitstream buffer
        MS_VIRT u32SrcOffset = pstSlqTblInfo->pDrvSlqTbl->u32StAdd;
        MS_VIRT u32SrcAdd = u32SrcOffset;
        MS_VIRT u32DstAdd = pstMemCfg->u32BSAddr;
        MS_VIRT u32DstOffset = HAL_MVD_GetMemOffset(pstMemCfg->u32BSAddr);
        MS_VIRT u32TblWr;

        _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32SrcOffset,u32SrcAdd);


        HAL_MVD_CPU_Sync();

        BDMA_Result bdmaRlt;

        bdmaRlt = MDrv_BDMA_CopyHnd(u32SrcAdd, u32DstAdd, pstMemCfg->u32DrvBufSize, pstSlqTblInfo->bdmaCpyType, BDMA_OPCFG_DEF);

        if (E_BDMA_OK != bdmaRlt)
        {
            MVD_DEBUGERROR(VPRINTF("%s MDrv_BDMA_MemCopy fail ret=%x!\n", __FUNCTION__, bdmaRlt));
        }
        MVD_DEBUGINFO(VPRINTF("SlqWrPtr_BDMA src=0x%lx dst=0x%lx size=0x%lx cpyType=0x%x\n",
            (unsigned long)u32SrcAdd, (unsigned long)u32DstAdd, (unsigned long)pstMemCfg->u32DrvBufSize, pstSlqTblInfo->bdmaCpyType));

        u32TblWr = u32DstOffset + (u32WrPtr - pstSlqTblInfo->pDrvSlqTbl->u32StAdd);

        u32WrPtr = u32TblWr;
    }
    //VPRINTF("wPtr= 0x%x(0x%x) rPtr=0x%x(0x%x)\n", writePtr, pstSlqTblInfo->pDrvSlqTbl->u32WrPtr,
    //    HAL_MVD_GetSLQReadPtr(u8Idx), pstSlqTblInfo->pDrvSlqTbl->u32RdPtr);


    //writePtrLast[u8Idx] = u32WrPtr;
    #if SLQ_NEW_PUSH
    if(!pstSlqTblInfo->pSlqStatus->bSlqEnLastFrameShow && pstSlqTblInfo->pSlqStatus->bSlqCtrlBit)
    {
        u32WrPtr = pstSlqTblInfo->pSlqStatus->u32VaildWptrAddr;
    }
    #endif
    MS_ASSERT((u32WrPtr%8)==0);
    u32WrPtr >>= 3;

    if (!(pMVDHalContext->bSlqTblHKCtrl[u8Idx]))
    {
        SET_CMDARG(mvdcmd, u32WrPtr, u8Idx);
        HAL_MVD_CPU_Sync();
        SET_CMD_RET_FALSE(CMD_SLQ_UPDATE_TBL_WPTR, &mvdcmd);
    }
    else
    {
        HAL_MVD_SlqTblLoadWrPtr(u8Idx, u32WrPtr);
        if (HAL_MVD_SlqTblProbeWrPtr(u8Idx) != u32WrPtr)
        {
            MVD_DEBUGERROR(VPRINTF("Ooops! SlqWrPtr update fail!!! 0x%lx != 0x%lx\n", (unsigned long)HAL_MVD_SlqTblProbeWrPtr(u8Idx), (unsigned long)u32WrPtr));
            return FALSE;
        }
    }

    return TRUE;
}

void HAL_MVD_SetOverflowTH(MS_U8 u8Idx, MS_U32 u32Threshold)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32Threshold%8)==0);
    u32Threshold >>= 3;
    SET_CMDARG(mvdcmd, u32Threshold, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DMA_OVFTH, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DMA_OVFTH ) );
        return;
    }
    return;
}

//------------------------------------------------------------------------------
/// Set bitstream buffer underflow threshold
/// @return -none
//------------------------------------------------------------------------------
void HAL_MVD_SetUnderflowTH(MS_U8 u8Idx, MS_U32 u32Threshold)
{
    MVD_CmdArg mvdcmd;

    MS_ASSERT((u32Threshold%8)==0);
    u32Threshold >>= 3;
    SET_CMDARG(mvdcmd, u32Threshold, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_DMA_UNFTH, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DMA_UNFTH ) );
        return;
    }
    return;
}

MS_BOOL HAL_MVD_GenPattern(MS_U8 u8Idx, MVD_PatternType ePattern, MS_PHY u32PAddr, MS_U32* pu32Size)
{
    MS_U8* pu8DummyData = NULL;
    MVD_CodecType curCodecType = HAL_MVD_GetCodecType(u8Idx);

    if ((!pu32Size) || (*pu32Size < SKIP_PATTERN_SIZE))
    {
        return FALSE;
    }

    pu8DummyData = (MS_U8*) HAL_MVD_PA2NonCacheSeg(u32PAddr);

    if(pu8DummyData == NULL)
    {
        MVD_DEBUGERROR(VPRINTF("%s  %d  NULL Address\n",__FILE__,__LINE__));
        return FALSE;
    }

    switch (ePattern)
    {
        case E_MVD_PATTERN_FLUSH:
            #if 0
            *pu8DummyData = SKIP_PATTERN_0;
            pu8DummyData++;
            *pu8DummyData = SKIP_PATTERN_1;
            *pu32Size = SKIP_PATTERN_SIZE;
            #else
            pu8DummyData[0] =    SKIP_PATTERN_0&0xff;
            pu8DummyData[1] = (SKIP_PATTERN_0>>8)&0xff;
            pu8DummyData[2] = (SKIP_PATTERN_0>>16)&0xff;
            pu8DummyData[3] = (SKIP_PATTERN_0>>24)&0xff;

            pu8DummyData[4] = SKIP_PATTERN_1&0xff;
            pu8DummyData[5] = (SKIP_PATTERN_1>>8)&0xff;
            pu8DummyData[6] = (SKIP_PATTERN_1>>16)&0xff;
            pu8DummyData[7] = (SKIP_PATTERN_1>>24)&0xff;

            *pu32Size = SKIP_PATTERN_SIZE;
            #endif
            break;
        case E_MVD_PATTERN_FILEEND:
            if ((E_MVD_CODEC_FLV == curCodecType)||(E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER == curCodecType))
            {
                #if 0
                *pu8DummyData = FLV_PATTERN;
                pu8DummyData++;
                *pu8DummyData = 0xffffffff;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_1;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_2;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_3;
                #else
                pu8DummyData[0] = FLV_PATTERN&0xff;
                pu8DummyData[1] = (FLV_PATTERN>>8)&0xff;
                pu8DummyData[2] = (FLV_PATTERN>>16)&0xff;
                pu8DummyData[3] = (FLV_PATTERN>>24)&0xff;

                pu8DummyData[4] = 0xffffffff&0xff;
                pu8DummyData[5] = (0xffffffff>>8)&0xff;
                pu8DummyData[6] = (0xffffffff>>16)&0xff;
                pu8DummyData[7] = (0xffffffff>>24)&0xff;

                pu8DummyData[8] = END_PATTERN_1&0xff;
                pu8DummyData[9] = (END_PATTERN_1>>8)&0xff;
                pu8DummyData[10] = (END_PATTERN_1>>16)&0xff;
                pu8DummyData[11] = (END_PATTERN_1>>24)&0xff;

                pu8DummyData[12] = END_PATTERN_2&0xff;
                pu8DummyData[13] = (END_PATTERN_2>>8)&0xff;
                pu8DummyData[14] = (END_PATTERN_2>>16)&0xff;
                pu8DummyData[15] = (END_PATTERN_2>>24)&0xff;

                pu8DummyData[16] = END_PATTERN_3&0xff;
                pu8DummyData[17] = (END_PATTERN_3>>8)&0xff;
                pu8DummyData[18] = (END_PATTERN_3>>16)&0xff;
                pu8DummyData[19] = (END_PATTERN_3>>24)&0xff;

                #endif

            }
            else if (E_MVD_CODEC_DIVX311 == curCodecType)
            {
                #if 0
                *pu8DummyData = DIVX_PATTERN;
                pu8DummyData++;
                *pu8DummyData = 0xffffffff;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_1;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_2;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_3;
                #else
                pu8DummyData[0] = DIVX_PATTERN&0xff;
                pu8DummyData[1] = (DIVX_PATTERN>>8)&0xff;
                pu8DummyData[2] = (DIVX_PATTERN>>16)&0xff;
                pu8DummyData[3] = (DIVX_PATTERN>>24)&0xff;

                pu8DummyData[4] = 0xffffffff&0xff;
                pu8DummyData[5] = (0xffffffff>>8)&0xff;
                pu8DummyData[6] = (0xffffffff>>16)&0xff;
                pu8DummyData[7] = (0xffffffff>>24)&0xff;

                pu8DummyData[8] = END_PATTERN_1&0xff;
                pu8DummyData[9] = (END_PATTERN_1>>8)&0xff;
                pu8DummyData[10] = (END_PATTERN_1>>16)&0xff;
                pu8DummyData[11] = (END_PATTERN_1>>24)&0xff;

                pu8DummyData[12] = END_PATTERN_2&0xff;
                pu8DummyData[13] = (END_PATTERN_2>>8)&0xff;
                pu8DummyData[14] = (END_PATTERN_2>>16)&0xff;
                pu8DummyData[15] = (END_PATTERN_2>>24)&0xff;

                pu8DummyData[16] = END_PATTERN_3&0xff;
                pu8DummyData[17] = (END_PATTERN_3>>8)&0xff;
                pu8DummyData[18] = (END_PATTERN_3>>16)&0xff;
                pu8DummyData[19] = (END_PATTERN_3>>24)&0xff;
                #endif
            }
            else if ((E_MVD_CODEC_MPEG2 == curCodecType)||(E_MVD_CODEC_MPEG4 == curCodecType))
            {
                #if 0
                *pu8DummyData = MPEG_PATTERN_0;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_1;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_2;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_3;
                #else
                pu8DummyData[0] = MPEG_PATTERN_0&0xff;
                pu8DummyData[1] = (MPEG_PATTERN_0>>8)&0xff;
                pu8DummyData[2] = (MPEG_PATTERN_0>>16)&0xff;
                pu8DummyData[3] = (MPEG_PATTERN_0>>24)&0xff;

                pu8DummyData[4] = END_PATTERN_1&0xff;
                pu8DummyData[5] = (END_PATTERN_1>>8)&0xff;
                pu8DummyData[6] = (END_PATTERN_1>>16)&0xff;
                pu8DummyData[7] = (END_PATTERN_1>>24)&0xff;

                pu8DummyData[8] = END_PATTERN_2&0xff;
                pu8DummyData[9] = (END_PATTERN_2>>8)&0xff;
                pu8DummyData[10] = (END_PATTERN_2>>16)&0xff;
                pu8DummyData[11] = (END_PATTERN_2>>24)&0xff;

                pu8DummyData[12] = END_PATTERN_3&0xff;
                pu8DummyData[13] = (END_PATTERN_3>>8)&0xff;
                pu8DummyData[14] = (END_PATTERN_3>>16)&0xff;
                pu8DummyData[15] = (END_PATTERN_3>>24)&0xff;
                #endif
            }
            else
            {
                #if 0
                *pu8DummyData = END_PATTERN_0;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_1;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_2;
                pu8DummyData++;
                *pu8DummyData = END_PATTERN_3;
                #else
                pu8DummyData[0] = END_PATTERN_0&0xff;
                pu8DummyData[1] = (END_PATTERN_0>>8)&0xff;
                pu8DummyData[2] = (END_PATTERN_0>>16)&0xff;
                pu8DummyData[3] = (END_PATTERN_0>>24)&0xff;

                pu8DummyData[4] = END_PATTERN_1&0xff;
                pu8DummyData[5] = (END_PATTERN_1>>8)&0xff;
                pu8DummyData[6] = (END_PATTERN_1>>16)&0xff;
                pu8DummyData[7] = (END_PATTERN_1>>24)&0xff;

                pu8DummyData[8] = END_PATTERN_2&0xff;
                pu8DummyData[9] = (END_PATTERN_2>>8)&0xff;
                pu8DummyData[10] = (END_PATTERN_2>>16)&0xff;
                pu8DummyData[11] = (END_PATTERN_2>>24)&0xff;

                pu8DummyData[12] = END_PATTERN_3&0xff;
                pu8DummyData[13] = (END_PATTERN_3>>8)&0xff;
                pu8DummyData[14] = (END_PATTERN_3>>16)&0xff;
                pu8DummyData[15] = (END_PATTERN_3>>24)&0xff;
                #endif
            }
            *pu32Size = 16;
            break;
        default:
            break;
    }

    return TRUE;
}



void HAL_MVD_DbgDump(MS_U8 u8Idx)
{
    MS_U32 u32VdCnt=0;
    //static MS_U32 u32PreVdCnt[MAX_DEC_NUM]= {0, 0};
    MS_U32 u32ErrCnt=0;
    //static MS_U32 u32PreErrCnt[MAX_DEC_NUM]= {0, 0};
    //static MS_BOOL b1stDump[MAX_DEC_NUM]= {TRUE, TRUE};
    MVD_FrmInfo stFrm = {_INIT_ADDR, _INIT_ADDR, _INIT_TIMESTAMP, _INIT_ID,
                         _INIT_ID, _INIT_LEN, _INIT_LEN, _INIT_LEN, 0xff, E_MVD_PIC_UNKNOWN};
    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8Idx);
    #ifndef VDEC3
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    #endif
    if (pMVDHalContext->b1stDump[u8Idx])
    {
        MVD_FWCfg* pFwCfg = NULL;
        pFwCfg = HAL_MVD_GetFWCfg(u8Idx);
        MVD_DEBUGINFO(VPRINTF("curDisablePESParsing = %d\n", pFwCfg->bDisablePESParsing));
        pMVDHalContext->b1stDump[u8Idx] = FALSE;
    }

    u32VdCnt = HAL_MVD_GetParserByteCnt(u8Idx);
    u32ErrCnt= HAL_MVD_GetVldErrCount(u8Idx);
    MVD_DEBUGINFO(VPRINTF("input: vfifo=%d(full=%d,empty=%d), vdCnt=%d(%d), vldErr=%d(%d); ",
            HAL_MVD_RegReadByte(0x1564)>>6, HAL_MVD_RegReadByte(0x1564)&0x20,
            HAL_MVD_RegReadByte(0x1564)&0x10, u32VdCnt, (u32VdCnt-pMVDHalContext->u32PreVdCnt[u8Idx]),
            u32ErrCnt, (u32ErrCnt-pMVDHalContext->u32PreErrCnt[u8Idx])));
    pMVDHalContext->u32PreVdCnt[u8Idx] = u32VdCnt;
    pMVDHalContext->u32PreErrCnt[u8Idx] = u32ErrCnt;

    MVD_DEBUGINFO(VPRINTF("state: fw=0x%x, lastCmd=0x%x, pc=0x%x\n",
            HAL_MVD_GetDecodeStatus(u8Idx), HAL_MVD_GetLastCmd(u8Idx), HAL_VPU_EX_GetProgCnt()));
    MVD_DEBUGINFO(VPRINTF("cnt: dec=%d, skip=%d, drop=%ld\n", HAL_MVD_GetPicCounter(u8Idx), HAL_MVD_GetSkipPicCounter(u8Idx), 0x0UL));
    HAL_MVD_GetFrmInfo(u8Idx, E_MVD_FRMINFO_DECODE, &stFrm);
    MVD_DEBUGINFO(VPRINTF("frm Dec Y=%lx UV=%lx Pitch=%x; ", (unsigned long)stFrm.u32LumaAddr, (unsigned long)stFrm.u32ChromaAddr, stFrm.u16Pitch));
    HAL_MVD_GetFrmInfo(u8Idx, E_MVD_FRMINFO_DISPLAY, &stFrm);
    MVD_DEBUGINFO(VPRINTF("Disp Y=%lx UV=%lx Pitch=%x\n", (unsigned long)stFrm.u32LumaAddr, (unsigned long)stFrm.u32ChromaAddr, stFrm.u16Pitch));
    if (curSrcMode == E_MVD_SLQ_TBL_MODE)
    {
        #ifndef VDEC3
        MVD_DEBUGINFO(VPRINTF("fe=%x, rd=%x(%x), wr=%x, empty=%x; ", pstSlqTblInfo->u32FileEndPtr, pstSlqTblInfo->pDrvSlqTbl->u32RdPtr,
                HAL_MVD_Map2DrvSlqTbl(u8Idx, HAL_MVD_GetSLQReadPtr(u8Idx)), pstSlqTblInfo->pDrvSlqTbl->u32WrPtr, pstSlqTblInfo->pDrvSlqTbl->u32Empty));

        MVD_DEBUGINFO(VPRINTF("es rd=0x%lx, wr=0x%lx\n", (unsigned long)MVD_GetSlqTblESReadPtr(u8Idx), (unsigned long)MVD_GetSlqTblESWritePtr(u8Idx)));
         #endif
    }
    else if (curSrcMode == E_MVD_TS_FILE_MODE)
    {
        MVD_DEBUGINFO(VPRINTF("es rd=0x%lx, wr=0x%lx\n", (unsigned long)HAL_MVD_GetTsFileESReadPtr(u8Idx), (unsigned long)HAL_MVD_GetTsFileESWritePtr(u8Idx)));
    }
}

#ifdef MS_DEBUG
#define _STRINGIFY_HAL_SID(x)   \
    (x==E_HAL_MVD_MAIN_STREAM0)?"E_HAL_MVD_MAIN_STREAM0": \
    ((x==E_HAL_MVD_SUB_STREAM0)?"E_HAL_MVD_SUB_STREAM0":"E_HAL_MVD_STREAM_NONE")
void MVD_DumpMVDStream(void)
{
    //if (_u8HalDbgLevel == 0) return;
    MS_U8 i = 0;
    for (i=0; i< MAX_DEC_NUM; i++)
    {
        MVD_DEBUGINFO(VPRINTF("[%d] eStreamId=0x%x(%s) bUsed=0x%x\n", i, pMVDHalContext->_stMVDStream[i].eStreamId,
                _STRINGIFY_HAL_SID(pMVDHalContext->_stMVDStream[i].eStreamId), pMVDHalContext->_stMVDStream[i].bUsed));
    }
}
#endif
MS_BOOL HAL_MVD_Init_Share_Mem(void)
{
#if (defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_LINUX_KERNEL))
#if !defined(SUPPORT_X_MODEL_FEATURE)
    MS_U32 u32ShmId;
    MS_VIRT u32Addr;
    MS_U32 u32BufSize;


    if (FALSE == MsOS_SHM_GetId( (MS_U8*)"Linux MVD HAL",
                                          sizeof(MVD_Hal_CTX),
                                          &u32ShmId,
                                          &u32Addr,
                                          &u32BufSize,
                                          MSOS_SHM_QUERY))
    {
        if (FALSE == MsOS_SHM_GetId((MS_U8*)"Linux MVD HAL",
                                             sizeof(MVD_Hal_CTX),
                                             &u32ShmId,
                                             &u32Addr,
                                             &u32BufSize,
                                             MSOS_SHM_CREATE))
        {
            MVD_DEBUGINFO(VPRINTF("[%s]SHM allocation failed!!! use global structure instead!!!\n",__FUNCTION__));
            if(pMVDHalContext == NULL)
            {
                pMVDHalContext = &gMVDHalContext;
                memset(pMVDHalContext,0,sizeof(MVD_Hal_CTX));
                HAL_MVD_Context_Init();
                MVD_DEBUGINFO(VPRINTF("[%s]Global structure init Success!!!\n",__FUNCTION__));
            }
            else
            {
                MVD_DEBUGINFO(VPRINTF("[%s]Global structure exists!!!\n",__FUNCTION__));
            }
            //return FALSE;
        }
        else
        {
            memset((MS_U8*)u32Addr,0,sizeof(MVD_Hal_CTX));
            pMVDHalContext = (MVD_Hal_CTX*)u32Addr; // for one process
            HAL_MVD_Context_Init();
        }
    }
    else
    {
        pMVDHalContext = (MVD_Hal_CTX*)u32Addr; // for another process
    }
#else
    if(pMVDHalContext == NULL)
    {
        pMVDHalContext = &gMVDHalContext;
        memset(pMVDHalContext,0,sizeof(MVD_Hal_CTX));
        HAL_MVD_Context_Init();
    }
#endif
    OSAL_MVD_MutexInit();
#else
    if(pMVDHalContext == NULL)
    {
        pMVDHalContext = &gMVDHalContext;
        memset(pMVDHalContext,0,sizeof(MVD_Hal_CTX));
        HAL_MVD_Context_Init();
}
#endif


    return TRUE;

}

//------------------------------------------------------------------------------
HAL_MVD_StreamId HAL_MVD_GetFreeStream(HAL_MVD_StreamType eStreamType)
{
    MS_U32 i = 0;

#ifdef MS_DEBUG
    MVD_DumpMVDStream();
#endif
    if (eStreamType == E_HAL_MVD_MAIN_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_MVD_MAIN_STREAM_MAX - E_HAL_MVD_MAIN_STREAM_BASE) +
              (E_HAL_MVD_SUB_STREAM_MAX - E_HAL_MVD_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_MVD_MAIN_STREAM_BASE & pMVDHalContext->_stMVDStream[i].eStreamId) && (FALSE == pMVDHalContext->_stMVDStream[i].bUsed))
            {
                return pMVDHalContext->_stMVDStream[i].eStreamId;
            }
        }
    }
    else if (eStreamType == E_HAL_MVD_SUB_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_MVD_MAIN_STREAM_MAX - E_HAL_MVD_MAIN_STREAM_BASE) +
              (E_HAL_MVD_SUB_STREAM_MAX - E_HAL_MVD_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_MVD_SUB_STREAM_BASE & pMVDHalContext->_stMVDStream[i].eStreamId) && (FALSE == pMVDHalContext->_stMVDStream[i].bUsed))
            {
                return pMVDHalContext->_stMVDStream[i].eStreamId;
            }
        }
    }
#ifdef VDEC3
    else if ((eStreamType >= E_HAL_MVD_N_STREAM) && (eStreamType < E_HAL_MVD_N_STREAM + MAX_DEC_NUM))
    {
        i = eStreamType - E_HAL_MVD_N_STREAM;
        if (!pMVDHalContext->_stMVDStream[i].bUsed)
        {
            return pMVDHalContext->_stMVDStream[i].eStreamId;
         }
    }
#endif

    return E_HAL_MVD_STREAM_NONE;
}

MS_U8 HAL_MVD_SidToIdx(HAL_MVD_StreamId eSID)
{
#ifdef VDEC3
    return eSID - E_HAL_MVD_N_STREAM_BASE;
#else
    MS_U8 u8Idx = 0;
    switch (eSID)
    {
        case E_HAL_MVD_MAIN_STREAM0:
          u8Idx = 0;
          break;
        case E_HAL_MVD_SUB_STREAM0:
          u8Idx = 1;
          break;
#if 0
        case E_HAL_MVD_SUB_STREAM1:
          u8Idx = 2;
          break;
#endif
        default:
          u8Idx = 0;
          break;
    }
    return u8Idx;
#endif
}

MS_BOOL HAL_MVD_ReleaseFdMask(MS_U8 u8Idx, MS_BOOL bRls)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bRls;  //1 to release the fd mask
    MVD_DEBUGINFO(VPRINTF("%s: release=0x%x\n", __FUNCTION__, bRls));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_UPDATE_FRAME, &mvdcmd);

    return TRUE;
}

MS_BOOL HAL_MVD_ParserRstDone(MS_U8 u8Idx, MS_BOOL bEnable)
{
    #define PARSER_RST_TIMEOUT 0x40000
    MS_U32 u32TimeOut = 0;
    MVD_CmdArg mvdcmd;

    HAL_MVD_ResetHandShake(u8Idx, MVD_HANDSHAKE_PARSER_RST);
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg1 = bEnable;
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_PTS_TBL_RESET, &mvdcmd);

    while ((TRUE != HAL_MVD_IsCmdFinished(u8Idx, MVD_HANDSHAKE_PARSER_RST)) && (u32TimeOut < PARSER_RST_TIMEOUT))
    {
        u32TimeOut++;
    }
    if (u32TimeOut >= PARSER_RST_TIMEOUT)
    {
        MVD_DEBUGERROR( VPRINTF( "Ctrl: 0x%x fail timeout!!\r\n", CMD_PTS_TBL_RESET ) );
        return FALSE;
    }
    return TRUE;
}

MS_BOOL HAL_MVD_FlushPTSBuf(MS_U8 u8Idx, MS_BOOL bEnable)
{
#ifdef VDEC3
    MVD_CtrlCfg* pstCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);

    if(pstCtrlCfg->u8McuMode == FALSE)
#endif
    {
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    if (HAL_MVD_MVDCommand( CMD_SEND_UNI_PTS, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SEND_UNI_PTS ) );
        return FALSE;
    }
    }
    return TRUE;
}


MS_U32 HAL_MVD_GetSLQNum(MS_U8 u8Idx)
{
    MVD_SLQTBLInfo* pstSlqTblInfo = HAL_MVD_GetSlqTblInfo(u8Idx);
    MS_VIRT u32RdPtr = HAL_MVD_GetSLQReadPtr(u8Idx);
    MS_VIRT u32Diff = 0;

    if (pstSlqTblInfo->pDrvSlqTbl->u32WrPtr >= u32RdPtr)
    {
        u32Diff = pstSlqTblInfo->pDrvSlqTbl->u32WrPtr - u32RdPtr;
    }
    else
    {
        u32Diff = SLQ_TBL_SIZE - (u32RdPtr - pstSlqTblInfo->pDrvSlqTbl->u32WrPtr);
    }

//    VPRINTF("slq wptr = 0x%x, rptr = 0x%x\n", pstSlqTblInfo->pDrvSlqTbl->u32WrPtr, u32RdPtr);

    return (u32Diff/SLQ_ENTRY_LEN);
}


MS_U32 HAL_MVD_GetDispQNum(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DISPQ_NUM, sizeof(MS_U8));
}
MS_BOOL HAL_MVD_SetAutoMute(MS_U8 u8Idx, MS_BOOL bEn)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEn;  //1 to enable
    MVD_DEBUGINFO(VPRINTF("%s: bEn=%x\n", __FUNCTION__, bEn));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_ENABLE_AUTO_MUTE, &mvdcmd);

    return TRUE;
}

MS_BOOL HAL_MVD_SetVSizeAlign(MS_U8 u8Idx, MS_BOOL bEn)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEn;  //1 to enable VSize alignment to 4x
    MVD_DEBUGINFO(VPRINTF("%s: bEn=%x\n", __FUNCTION__, bEn));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FORCE_ALIGN_VSIZE, &mvdcmd);

    return TRUE;
}

#define MVD_HW_MAX_PIXEL (1920*1088*61000ULL) //FullHD@60p
MS_BOOL HAL_MVD_GetFrmRateIsSupported(MS_U16 u16HSize, MS_U16 u16VSize, MS_U32 u32FrmRate)
{
    MVD_DEBUGINFO(VPRINTF("%s w:%d, h:%d, fr:%d, MAX:%lld\n", __FUNCTION__, u16HSize, u16VSize, u32FrmRate, MVD_HW_MAX_PIXEL));
    return (((MS_U64)u16HSize*(MS_U64)u16VSize*(MS_U64)u32FrmRate) <= MVD_HW_MAX_PIXEL);
}

//------------------------------------------------------------------------------
/// Wait MVD generate CRC done or timeout
/// @return -MVD generate CRC done or timeout
//------------------------------------------------------------------------------
MS_BOOL _HAL_MVD_CrcTimeOut(MS_U8 u8Idx)
{
    MS_U32 i;
    MS_U32 u32StartTime = MsOS_GetSystemTime();

    for ( i = 0; i < MVD_PollingTimes; i++ )
    {
        ///- wait until MVD generate CRC done or timeout
        if ( ( HAL_MVD_RegReadByte(MVD_CRC_CTL) & MVD_CRC_CTL_DONE ) == MVD_CRC_CTL_DONE )
        {
            return FALSE;
        }

        if ((TRUE == pMVDHalContext->bStopped[u8Idx]) || ((MsOS_GetSystemTime()-u32StartTime)>1300))
        {
            MVD_DEBUGINFO(VPRINTF("%s: bStopped(%x) or timeout(%d)\n", __FUNCTION__, pMVDHalContext->bStopped[u8Idx], MsOS_GetSystemTime()-u32StartTime));
            return TRUE;
        }

    }
    MVD_DEBUGERROR( VPRINTF("_HAL_MVD_CrcTimeOut=%x\n", i) );
    return TRUE;
}

E_MVD_Result HAL_MVD_GetCrcValue(MS_U8 u8Idx, MVD_CrcIn *pCrcIn, MVD_CrcOut *pCrcOut)
{
    E_MVD_Result eRet = E_MVD_RET_OK;
    MS_U32 u32tmp = 0;

    OSAL_MVD_LockHwMutex();

    MVD_DEBUGINFO(VPRINTF("%s width=0x%x, height=0x%x, pitch=0x%x, luma=0x%lx, chroma=0x%lx\n",
                                    __FUNCTION__, pCrcIn->u32HSize, pCrcIn->u32VSize, pCrcIn->u32Strip, (unsigned long)pCrcIn->u32YStartAddr, (unsigned long)pCrcIn->u32UVStartAddr));

    //Set generate CRC value
    u32tmp = ((pCrcIn->u32HSize + MVD_WIDTH_ALIGN_MASK) >> MVD_WIDTH_ALIGN_BITS) << MVD_WIDTH_ALIGN_BITS;
    HAL_MVD_RegWriteByte(MVD_CRC_HSIZE, u32tmp & 0xf0);
    HAL_MVD_RegWriteByte(MVD_CRC_HSIZE+1, (u32tmp >> 8) & 0x3f);

    u32tmp = ((pCrcIn->u32VSize + MVD_WIDTH_ALIGN_MASK) >> MVD_WIDTH_ALIGN_BITS) << MVD_WIDTH_ALIGN_BITS;
    HAL_MVD_RegWriteByte(MVD_CRC_VSIZE, u32tmp & 0xff);
    HAL_MVD_RegWriteByte(MVD_CRC_VSIZE+1, (u32tmp >> 8) & 0xff);

    u32tmp = (pCrcIn->u32Strip) >> 3;
    HAL_MVD_RegWriteByte(MVD_CRC_STRIP, u32tmp & 0xff);
    HAL_MVD_RegWriteByte((MVD_CRC_STRIP+1), (u32tmp >> 8) & 0xff);

    u32tmp = (pCrcIn->u32YStartAddr) >> 3;
    HAL_MVD_RegWrite4Byte(MVD_CRC_Y_START, u32tmp & MVD_CRC_Y_START_LEN);

    u32tmp = (pCrcIn->u32UVStartAddr) >> 3;
    HAL_MVD_RegWrite4Byte(MVD_CRC_UV_START, u32tmp & MVD_CRC_UV_START_LEN);

    //Fire
    HAL_MVD_RegWriteBit(MVD_CRC_CTL, 1, MVD_CRC_CTL_FIRE);

    //Check CRC done
    if ( _HAL_MVD_CrcTimeOut(u8Idx) == TRUE )
    {
        eRet = E_MVD_RET_TIME_OUT;
        goto _CRC_DONE;
    }

    //Get CRC value
    pCrcOut->u32YCrc = HAL_MVD_RegReadByte(MVD_CRC_Y_L) |
             (HAL_MVD_RegReadByte(MVD_CRC_Y_L+1) << 8) |
             (HAL_MVD_RegReadByte(MVD_CRC_Y_H) <<16) |
             (HAL_MVD_RegReadByte(MVD_CRC_Y_H+1) <<24);

    pCrcOut->u32UVCrc = HAL_MVD_RegReadByte(MVD_CRC_UV_L) |
             (HAL_MVD_RegReadByte(MVD_CRC_UV_L+1) << 8) |
             (HAL_MVD_RegReadByte(MVD_CRC_UV_H) <<16) |
             (HAL_MVD_RegReadByte(MVD_CRC_UV_H+1) <<24);

    MVD_DEBUGINFO(VPRINTF("%s Y=0x%x, UV=0x%x\n", __FUNCTION__, pCrcOut->u32YCrc, pCrcOut->u32UVCrc));

_CRC_DONE:
    OSAL_MVD_UnlockHwMutex();
    return eRet;
}


MS_BOOL HAL_MVD_SuspendDynamicScale(MS_U8 u8Idx, MS_BOOL bEn)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEn;
    MVD_DEBUGINFO(VPRINTF("%s: bEn=%x\n", __FUNCTION__, bEn));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_SUSPEND_DS, &mvdcmd);

    return TRUE;
}

MS_U8 HAL_MVD_GetSuspendDynamicScale(MS_U8 u8Idx)
{
    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_SUSPEND_DS, sizeof(MS_U8));
}

MS_U8 HAL_MVD_GetStereoType(MS_U8 u8Idx)
{
    return (MS_U8)MVD_GetFWBuffData(u8Idx, FW_BUFF_VOLINFO, OFFSET_STEREO_TYPE, sizeof(MS_U8));
}

MS_U32 HAL_MVD_GetDivxVer(MS_U8 u8Idx)
{
    return MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DIVX_VER_5X, sizeof(MS_U32));
}

MS_BOOL HAL_MVD_SetIdctMode(MS_U8 u8Idx, MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode;
    SET_DECNUM(mvdcmd, u8Idx);

    MVD_DEBUGINFO(VPRINTF("%s CMD_IDCT_SEL arg0=%x)\n", __FUNCTION__, u8Mode));
    SET_CMD_RET_FALSE(CMD_IDCT_SEL, &mvdcmd);

    return TRUE;
}

E_MVD_Result HAL_MVD_EX_SetClockSpeed(HAL_MVD_EX_ClockSpeed eClockSpeed)
{
    UNUSED(eClockSpeed);
    MVD_DEBUGINFO(VPRINTF("This chip does not support mvd clock presetting~\n"));
    return E_MVD_RET_FAIL;
}

MS_BOOL HAL_MVD_GetIsAVSyncOn(MS_U8 u8Idx)
{
    MS_BOOL bAVSyncOn = FALSE;
    MVD_CtrlCfg* pCtrlCfg = HAL_MVD_GetCtrlCfg(u8Idx);
    if (pCtrlCfg)
    {
        bAVSyncOn = pCtrlCfg->bAVSyncOn;
    }

    return bAVSyncOn;
}

MS_BOOL HAL_MVD_SetExternalDSBuff(MS_U8 u8Idx, MS_VIRT u32VPUAddr, MS_VIRT u32DrvAddr)
{
    MS_BOOL bRet = TRUE;
    MVD_FWBuff* pBuff = &(pMVDHalContext->stFWBuff[u8Idx]);

    pBuff->u32DynScalingAdd = u32DrvAddr;
    bRet = HAL_MVD_SetDynamicScaleAddr(u8Idx, u32VPUAddr);
    return bRet;
}

MS_BOOL HAL_MVD_ShowFirstFrameDirect(MS_U8 u8Idx, MS_U8 bEnable)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = bEnable;
    SET_DECNUM(mvdcmd, u8Idx);

    MVD_DEBUGINFO(VPRINTF("%s CMD_PUSH_FIRST_FRAME_DISP arg0=%x)\n", __FUNCTION__, bEnable));
    SET_CMD_RET_FALSE(CMD_PUSH_FIRST_FRAME_DISP, &mvdcmd);

    return TRUE;
}

E_MVD_Result HAL_MVD_SetXCLowDelayPara(MS_U8 u8Idx,MS_U32 u32Para)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = L_WORD(u32Para); // for set the XC diff_field_number
    mvdcmd.Arg1 = H_WORD(u32Para); // for set XC UCNR diff field no
    SET_DECNUM(mvdcmd, u8Idx);

    SET_CMD_RET_FALSE(CMD_XC_LOW_DELAY_PARA, &mvdcmd);

    return TRUE;
}

MS_U8 HAL_MVD_GetESBufferStatus(MS_U8 u8Idx)
{
    return (MS_U8)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_CURRENT_ES_BUFFER_STATUS, sizeof(MS_U8)));
}

MS_BOOL HAL_MVD_Field_Polarity_Display_One_field(MS_U8 u8Idx, MS_BOOL bEn,MS_U8 top_bottom)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg2 = bEn;
    mvdcmd.Arg3 = top_bottom;
    MVD_DEBUGINFO(VPRINTF("%s: bEn=%x,top_bottom=%x\n", __FUNCTION__, bEn,top_bottom));
    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_FP_FILTER, &mvdcmd);

    return TRUE;
}


MS_BOOL HAL_MVD_SetShareMemoryBase(MS_U8 u8Idx, MS_VIRT u32base, MS_U8 u8sel)
{
    u8SHMMiuSel = u8sel;
    u32SharememoryBase[u8Idx] = u32base - MVD_FW_CODE_LEN;
    return TRUE;
}

MS_BOOL HAL_MVD_GetShareMemoryOffset(MS_U8 u8Idx, MS_VIRT *u32base)
{
    *u32base=u32SharememoryBase[u8Idx];
    return  TRUE;
}

MS_BOOL HAL_MVD_Support2ndMVOPInterface(void)
{
    return TRUE;
}

MS_BOOL HAL_MVD_SetExternal_CC_Buffer(MS_U8 u8Idx, MS_VIRT u32base, MS_U8 u8size, MS_U8 cc_type)
{
    MVD_CmdArg mvdcmd;
    MS_U32 u32cc_size=0;
    MS_VIRT u32StartOffset;
    MS_U8  u8MiuSel;

    SETUP_CMDARG(mvdcmd);
    if (u8size !=0)
    {
        mvdcmd.Arg0 = TRUE; //enable
    }
    else
    {
        mvdcmd.Arg0 = FALSE; //disable
    }
    mvdcmd.Arg1 = cc_type; //608->1,708->0

    SET_DECNUM(mvdcmd, u8Idx);
    SET_CMD_RET_FALSE(CMD_CC_ENABLE_EXTERNAL_BUFFER, &mvdcmd);

    if (u8size ==0)
    {
        SET_CMD_RET_FALSE(CMD_CLOSE_CC, &mvdcmd);
        return E_MVD_RET_OK;
    }

    u32base *= (1<<10); //unit is bytes
    u32cc_size = u8size * (1<<10); //unit is bytes


    MS_VIRT *u32ReadPtr = (MS_VIRT *)(HAL_MVD_PA2NonCacheSeg(u32base));
    MS_VIRT *u32WritePtr = (MS_VIRT *)(HAL_MVD_PA2NonCacheSeg(u32base+sizeof(MS_VIRT)));
    MS_VIRT *u32LatchOverflow = (MS_VIRT *)(HAL_MVD_PA2NonCacheSeg(u32base+sizeof(MS_VIRT)*2));
    MS_VIRT *u32CpuBaseAddr = (MS_VIRT *)(HAL_MVD_PA2NonCacheSeg(u32base+sizeof(MS_VIRT)*3));

    //rptr,wptr,overflow,latch 16 bytes
    if(cc_type==1)
    {
        u32base+=(4*sizeof(MS_VIRT));
        u32cc_size -= (4*sizeof(MS_VIRT));
    }

    if(cc_type==0) // 708: start from 128 bytes
    {
        u32base+=128;
        u32cc_size -=128;
    }

    *u32ReadPtr = *u32WritePtr = u32base;
    *u32LatchOverflow = 0;
    *u32CpuBaseAddr =  HAL_MVD_GetMEMCfg(u8Idx)->u32FWBinAddr;

    _phy_to_miu_offset(u8MiuSel, u32StartOffset, u32base);
    _MVD_SetUserDataBufStart(u8Idx,u32StartOffset,cc_type);
    _MVD_SetUserDataBufSize(u8Idx,u32cc_size,cc_type);
    //MVD_DEBUGINFO(VPRINTF("%s: u32base=%x,u32size=%x,cc_type=%x\n", __FUNCTION__,u32base,u32cc_size,cc_type));

    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_HWBuffer_ReMappingMode(MS_U8 u8Idx,MS_BOOL bEnable)
{
   pMVDHalContext->gMVDPreCtrl[u8Idx].bHWBufferReMapping = bEnable;
    return E_MVD_RET_OK;
}

MS_BOOL HAL_MVD_SetPrebufferSize(MS_U8 u8Idx, MS_U32 size)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = size&0xff;
    mvdcmd.Arg1 = (size>>8)&0xff;
    mvdcmd.Arg2 = (size>>16)&0xff;
    mvdcmd.Arg3 = (size>>24)&0xff;
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand( CMD_PREBUFFER_SIZE, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_PREBUFFER_SIZE ) );
        return FALSE;
    }
    return TRUE;
}

MS_VIRT HAL_MVD_GetVsyncAddrOffset(void)
{
    MS_VIRT VPUSHMAddr = HAL_VPU_EX_GetSHMAddr();
    MS_VIRT VsyncBridgeOffset = 0;

    if(VPUSHMAddr != 0)  // TEE project
    {
        VsyncBridgeOffset = VSYNC_BRIGE_SHM_OFFSET;
    }
    else  // normal project
    {
        VsyncBridgeOffset = VSYNC_BRIGE_SHM_START;
    }

    return VsyncBridgeOffset;
}

MS_VIRT HAL_MVD_GetVsyncExtAddrOffset(void)
{
    MS_VIRT VsyncBridgeExtOffset = 0;
    MS_VIRT VPUSHMAddr = HAL_VPU_EX_GetSHMAddr();
    if(VPUSHMAddr != 0)  // TEE project
    {
        VsyncBridgeExtOffset = VSYNC_BRIGE_EXT_SHM_OFFSET;
    }
    else  // normal project
    {
        VsyncBridgeExtOffset = VSYNC_BRIGE_EXT_SHM_START;
    }

    return VsyncBridgeExtOffset;
}

E_MVD_Result HAL_MVD_SetTimeIncPredictParam(MS_U8 u8Idx, MS_U32 u32time)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);

    mvdcmd.Arg0 = L_WORD(u32time); // Arg0(enable), 1 for enable, default is 0...
    mvdcmd.Arg1 = H_WORD(u32time); // Arg1(vop_time_incr_predict_count), 1 for predict once, 2 for twice...0xff for always guess the vop_time_incr even with the vol_header, default is 0...
    mvdcmd.Arg2 = L_DWORD(u32time);// Arg2(vop_time_incr_follow_vol_header), // 0 for follow vol_header...1 for bypass vol_header, default is 0...
    mvdcmd.Arg3 = H_DWORD(u32time);// Arg3, reserve...
    MVD_DEBUGINFO(VPRINTF("CMD_TIME_INCR_PREDICT=%x\n",u32time));
    if (HAL_MVD_MVDCommand(CMD_TIME_INCR_PREDICT, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_TIME_INCR_PREDICT ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;

}

MS_BOOL HAL_MVD_SetDecodeTimeoutParam(MS_U8 u8Idx, MS_BOOL enable,MS_U32 u32timeout)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);

    if(enable == TRUE)
    {
        mvdcmd.Arg0 = 2; //0:disable, 2: enable decode timeout
        mvdcmd.Arg1 = u32timeout&0xff;
        mvdcmd.Arg2 = (u32timeout>>8)&0xff;
        mvdcmd.Arg3 = (u32timeout>>16)&0xff;
    }
    else
    {
        mvdcmd.Arg0 = 0; //0:disable, 2: enable decode timeout
    }

    SET_DECNUM(mvdcmd, u8Idx);

    if (HAL_MVD_MVDCommand( CMD_ENABLE_VLD_TIMEOUT, &mvdcmd ) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_ENABLE_VLD_TIMEOUT ) );
        return FALSE;
    }

    return TRUE;
}

E_MVD_Result HAL_MVD_Set_Smooth_Rewind(MS_U8 u8Idx, MS_U8 btype)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = btype;
    MVD_DEBUGINFO(VPRINTF("%s: btype=%x\n", __FUNCTION__, btype));
    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_SMOOTH_REWIND, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_SMOOTH_REWIND ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;
}

E_MVD_Result HAL_MVD_IsAlive(MS_U8 u8Idx)
{
    MVD_ALIVEInfo* pAliveInfo = &(pMVDHalContext->aliveInfo[u8Idx]);

    MVD_SrcMode curSrcMode = HAL_MVD_GetSrcMode(u8Idx);

    if (E_MVD_SLQ_TBL_MODE == curSrcMode || E_MVD_TS_FILE_MODE == curSrcMode)
    {
        if(pMVDHalContext->u32LastAliveTime[u8Idx] == 0)
        {
            pMVDHalContext->u32LastAliveTime[u8Idx] = MsOS_GetSystemTime();
            return E_MVD_RET_OK;
        }

        if((MsOS_GetSystemTime()-pMVDHalContext->u32LastAliveTime[u8Idx]) < 500)
        {
            return E_MVD_RET_OK;
        }
    }

#if 0
    printf("[%d]%d,%d,%d,%d,%d,%d,%d,%d\n",__FUNCTION__,
                                        (MS_U32)MsOS_GetSystemTime(),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DECODEDONE_COUNT, sizeof(MS_U32))),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHBUF_COUNT, sizeof(MS_U32))),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHCODE_COUNT, sizeof(MS_U32))),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PREBUF_COUNT, sizeof(MS_U32))),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_VFIFOBUF_COUNT, sizeof(MS_U32))),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHHEADER_COUNT, sizeof(MS_U32))),
                                        (MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_FLASHPATTERN_COUNT, sizeof(MS_U32))));
#endif

    //check count to vertify alive
    if((pAliveInfo->u32decode_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DECODEDONE_COUNT, sizeof(MS_U32))))||
       (pAliveInfo->u32searchbuf_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHBUF_COUNT, sizeof(MS_U32))))||
       (pAliveInfo->u32searchcode_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHCODE_COUNT, sizeof(MS_U32))))||
       (pAliveInfo->u32prebuf_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PREBUF_COUNT, sizeof(MS_U32))))||
       (pAliveInfo->u32vfifobuf_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_VFIFOBUF_COUNT, sizeof(MS_U32))))||
       (pAliveInfo->u32searchheader_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHHEADER_COUNT, sizeof(MS_U32))))||
       (pAliveInfo->u32flashpattern_count!=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_FLASHPATTERN_COUNT, sizeof(MS_U32)))))
    {
        pAliveInfo->u32decode_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_DECODEDONE_COUNT, sizeof(MS_U32)));
        pAliveInfo->u32searchbuf_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHBUF_COUNT, sizeof(MS_U32)));
        pAliveInfo->u32searchcode_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHCODE_COUNT, sizeof(MS_U32)));
        pAliveInfo->u32prebuf_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PREBUF_COUNT, sizeof(MS_U32)));
        pAliveInfo->u32vfifobuf_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_VFIFOBUF_COUNT, sizeof(MS_U32)));
        pAliveInfo->u32searchheader_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_SEARCHHEADER_COUNT, sizeof(MS_U32)));
        pAliveInfo->u32flashpattern_count=(MS_U32)(MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_FLASHPATTERN_COUNT, sizeof(MS_U32)));
        pMVDHalContext->u32LastAliveTime[u8Idx] = MsOS_GetSystemTime();
        return E_MVD_RET_OK;
    }
    else
    {
        pMVDHalContext->u32LastAliveTime[u8Idx] = MsOS_GetSystemTime();
        return E_MVD_RET_FAIL;
    }

}

E_MVD_Result HAL_MVD_Set_Err_Tolerance(MS_U8 u8Idx, MS_U16 u16Para)
{
    MVD_CmdArg mvdcmd;
    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u16Para & 0xFF; //enable or disable
    mvdcmd.Arg1 = u16Para >> 8; // err rate 0~100%
    if(mvdcmd.Arg1 >= 100)
        mvdcmd.Arg1 = 100;

    SET_DECNUM(mvdcmd, u8Idx);
    if (HAL_MVD_MVDCommand(CMD_DECODE_ERROR_TOLERANCE, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_DECODE_ERROR_TOLERANCE ) );
        return E_MVD_RET_FAIL;
    }
    return E_MVD_RET_OK;

}

void HAL_MVD_EnableAutoInsertDummyPattern(MS_U8 u8Idx, MS_BOOL bEnable)
{
    pMVDHalContext->bAutoInsertDummyPattern[u8Idx] = bEnable;
}

void HAL_MVD_Drop_One_PTS(MS_U8 u8Idx)
{
    pMVDHalContext->bDropOnePTS[u8Idx] = TRUE;
}

E_MVD_Result HAL_MVD_PVR_Seamless_mode(MS_U8 u8Idx, MS_U8 u8Arg)
{
    #define STOP_TIMEOUT 500 //ms
    MVD_CmdArg mvdcmd;
    E_MVD_Result ret = E_MVD_RET_OK;
    MS_U32 u32StartTime;
    MS_U32 u32SeamlessStatus = 0;
    MS_U32 u32BufStart = 0;
    MS_U32* temp = 0;

    if(u8Arg != 2)
    {
        // clear handshake dram
        u32BufStart = GET_FRMINFO_BUFFADD(u8Idx);
        _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,u32BufStart,u32BufStart);
        temp = (MS_U32*)MsOS_PA2KSEG1(u32BufStart+OFFSET_PVR_SEAMLESS_STATUS);
        *temp = 0;
        MsOS_FlushMemory();
    }

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Arg;
    MVD_DEBUGINFO(VPRINTF("%s: arg=%d\n", __FUNCTION__, (unsigned int)u8Arg));
    SET_DECNUM(mvdcmd, u8Idx);

    if (HAL_MVD_MVDCommand(CMD_PVR_SEAMLESS_MODE, &mvdcmd) == FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_PVR_SEAMLESS_MODE ) );
        return E_MVD_RET_FAIL;
    }


    u32StartTime = HAL_MVD_GetTime();

    if(u8Arg == 1)
    {
         pMVDHalContext->stFwCfg[u8Idx].eSrcMode = E_MVD_TS_FILE_MODE;
        while(1)
        {
            u32SeamlessStatus = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PVR_SEAMLESS_STATUS, sizeof(MS_U32));
            if(u32SeamlessStatus&1) // bit0 , pause done
            {
                break;
            }

            if((HAL_MVD_GetTime()-u32StartTime)>STOP_TIMEOUT)
            {
                MVD_DEBUGERROR(VPRINTF("PVR seamless pause timeout\n" ) );
                ret = E_MVD_RET_FAIL;
                break;
            }
        }
    }
    else if(u8Arg == 2)
    {
        while(1)
        {
            u32SeamlessStatus = MVD_GetFWBuffData(u8Idx, FW_BUFF_FRMINFO, OFFSET_PVR_SEAMLESS_STATUS, sizeof(MS_U32));
            if(u32SeamlessStatus&4) // bit2, hw reset done
            {
                break;
            }

            if((HAL_MVD_GetTime()-u32StartTime)>STOP_TIMEOUT)
            {
                MVD_DEBUGERROR(VPRINTF("PVR seamless hw reset timeout\n" ) );
                ret = E_MVD_RET_FAIL;
                break;
            }
        }
    }

    return ret;
}

E_MVD_Result HAL_MVD_SetDisplayFinishMode(MS_U8 u8Idx, MS_U8 u8Mode)
{
    MVD_CmdArg mvdcmd;

    SETUP_CMDARG(mvdcmd);
    mvdcmd.Arg0 = u8Mode;

    SET_DECNUM(mvdcmd, u8Idx);

    MVD_DEBUGINFO(VPRINTF("%s CMD_ENABLE_LAST_FRAME_QUALIFIER arg0=%x\n", __FUNCTION__, u8Mode));
    if (HAL_MVD_MVDCommand(CMD_ENABLE_LAST_FRAME_QUALIFIER, &mvdcmd)== FALSE)
    {
        MVD_DEBUGERROR( VPRINTF( "Command: 0x%x fail!!\r\n", CMD_ENABLE_LAST_FRAME_QUALIFIER) );
        return E_MVD_RET_FAIL;
    }

    return E_MVD_RET_OK;
}


MS_VIRT HAL_MVD_GetVBBUTableAddr(MS_U8 u8Idx)
{
    return pMVDHalContext->stFWBuff[(u8Idx)].u32VBBUTableAdd;
}

void HAL_MVD_SetDmxFrameRate(MS_U8 u8HalIdx,MS_U32 u32Value)
{
    pMVDHalContext->u32DmxFrameRate[u8HalIdx] = u32Value;
}

void HAL_MVD_SetDmxFrameRateBase(MS_U8 u8HalIdx,MS_U32 u32Value)
{
    pMVDHalContext->u32DmxFrameRateBase[u8HalIdx] = u32Value;
}

void HAL_MVD_SetCMAInformation(void* cmaInitParam)
{
    pMVDHalContext->bCMAUsed = TRUE;
    memcpy((void*)(&pMVDHalContext->cmaInitParam),cmaInitParam,sizeof(struct CMA_Pool_Init_Param));
}

void HAL_MVD_GetCMAInformation(MS_U8 u8HalIdx,MS_U64* u64Addr,MS_SIZE* u64Size,MS_BOOL* bAllocDone)
{
    FW_VOL_INFO gvolInfo;
    MS_U32 u32VolAddr;

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,GET_VOL_BUFFADD(u8HalIdx),u32VolAddr);

    gvolInfo = (*(volatile FW_VOL_INFO*)(MsOS_PA2KSEG1(u32VolAddr)));

    *u64Addr = (MS_U64)gvolInfo.CMA_FB_Address;
    *u64Size = (MS_SIZE)gvolInfo.CMA_FB_Size;
    *bAllocDone = gvolInfo.CMA_AllocDone;

    MsOS_ReadMemory();
    MsOS_FlushMemory();
}

void HAL_MVD_SetCMAAllocateDone(MS_U8 u8HalIdx)
{
    MS_U8* temp;
    MS_VIRT u32VolAddr;

    _miu_offset_to_phy(pMVDHalContext->stMiuCfg.u8FWMiuSel,GET_VOL_BUFFADD(u8HalIdx),u32VolAddr);

    temp = (MS_U8*)(MsOS_PA2KSEG1(u32VolAddr+OFFSET_CMA_ALLOCDONE));

    *temp = 1;

    MsOS_ReadMemory();
    MsOS_FlushMemory();
}

void HAL_MVD_SetFrameBufferMiu(MS_U8 u8HalIdx,MS_U8 u8MiuIdx)
{
    //to do....
}


#ifdef VDEC3
MS_BOOL HAL_MVD_IsDisplayCommand(MS_U8 u8Cmd)
{

#if 1 // to do.
    UNUSED(u8Cmd);
    return TRUE;
#else
    MS_BOOL ret;
    switch(u8Cmd)
    {
        case CMD_PLAY:
        case CMD_STOP:
        case CMD_DISPLAY_PAUSE:
        case CMD_PAUSE:
        case CMD_FLUSH_DISP_QUEUE:
        case CMD_VC1_HW_SLQ_RESET:
        case CMD_FAST_SLOW:
        case CMD_FLIP_RELEASE_FRAME:
        case CMD_FD_MASK_DELAY_CNT:
        case CMD_PVR_SEAMLESS_MODE:
        case CMD_SW_RESET:
        case CMD_CODEC_INFO:
        case CMD_CODE_OFFSET:
        case CMD_VOL_INFO_BUF:
        case CMD_FRAME_INFO_BUF:
        case CMD_HEADER_INFO_BUF:
        case CMD_USER_BUF_START:
        case CMD_USER_BUF_SIZE:
        case CMD_PTS_TBL_START:
        case CMD_DEC_FRAME_INFO_BUF:
        case CMD_SLQ_TBL_BUF_END:
        case CMD_SLQ_TBL_BUF_START:
        case CMD_STREAM_BUF_START:
        case CMD_STREAM_BUF_END:
        case CMD_FB_BASE:
        case CMD_IAP_BUF_START:
        case CMD_DP_BUF_START:
        case CMD_MV_BUF_START:
        case CMD_FB_NUM:
        case CMD_DISPLAY_CTL:
        case CMD_REPEAT_MODE:
        case CMD_DS_VIRTUAL_BOX:
        case CMD_ENABLE_FILE_SYNC:
        case CMD_RVU_EN:
        case CMD_START_DEC_STRICT:
        case CMD_SYNC_ON:
        case CMD_SET_MST_MODE:
        case CMD_SET_MCU_MODE:
            ret = TRUE;
            break;
        default:
            ret = FALSE;
            break;
    }

    return ret;
#endif
}

MS_BOOL HAL_MVD_IsNormalCommand(MS_U8 u8Cmd)
{
    MS_BOOL ret;

    switch(u8Cmd)
    {

        case CMD_DISP_SPEED_CTRL:
        case CMD_STEP_DISP_DECODE_ONE:
        case CMD_ENABLE_LAST_FRAME_SHOW:
        case CMD_STEP_TO_PTS:
        case CMD_SKIP_DATA:
        case CMD_DIU_WIDTH_ALIGN:
        case CMD_SCALER_INFO_BASE:
        case CMD_DYNAMIC_SCALE_BASE:
        case CMD_ENABLE_DYNAMIC_SCALE:
        case CMD_FORCE_BLUE_SCREEN:
        case CMD_FREEZE_DISP:
        case CMD_FIXED_FRAME_BUFFER:
        case CMD_SEND_UNI_PTS:
        case CMD_ENABLE_FREEZE_PIC:
        case CMD_PTS_BASE:
        case CMD_SINGLE_STEP:
        case CMD_PARSE_M4V_PACKMD:
        case CMD_SLQ_END:
        case CMD_SLQ_START:
        case CMD_ENABLE_INT_STAT:
        case CMD_SHOW_ONE_FIELD:
        case CMD_FRC_OUPUT:
        case CMD_FRC_DROP_BEHAVIOR:
        case CMD_FORBID_RESOLUTION_CHANGE:
        case CMD_DUMP_BITSTREAM_BASE:
        case CMD_DUMP_BITSTREAM_LENGTH:
        case CMD_SYNC_OFFSET:
        case CMD_SYN_THRESHOLD:
        case CMD_AVSYNC_FREERUN_THRESHOLD:
        case CMD_MVD_FAST_INT:
        case CMD_DIVX_PATCH:
        case CMD_IDCT_SEL:
        case CMD_SLQ_UPDATE_TBL_WPTR:
        case CMD_DMA_OVFTH:
        case CMD_DMA_UNFTH:
        case CMD_UPDATE_FRAME:
        case CMD_PTS_TBL_RESET:
        case CMD_ENABLE_AUTO_MUTE:
        case CMD_FORCE_ALIGN_VSIZE:
        case CMD_SUSPEND_DS:
        case CMD_PUSH_FIRST_FRAME_DISP:
        case CMD_XC_LOW_DELAY_PARA:
        case CMD_FP_FILTER:
        case CMD_CC_ENABLE_EXTERNAL_BUFFER:
        case CMD_CLOSE_CC:
        case CMD_PREBUFFER_SIZE:
        case CMD_TIME_INCR_PREDICT:
        case CMD_ENABLE_VLD_TIMEOUT:
        case CMD_SMOOTH_REWIND:
        case CMD_DECODE_ERROR_TOLERANCE:
        case CMD_MVD_IDLE:
        case CMD_PARSER_READ_POSITION:
        case CMD_SLQ_GET_TBL_RPTR:
        case CMD_DECODE_STATUS:
        case CMD_GET_AFD:
        case CMD_GET_SYNC_STAT:
        case CMD_RD_USER_WP:
        case CMD_ENABLE_LAST_FRAME_QUALIFIER:
        //case CMD_RD_IO: // no need in V3
        //case CMD_WR_IO: // no need in V3
            ret = TRUE;
            break;
        default:
            ret = FALSE;
            break;
    }

    return ret;
}

MS_BOOL HAL_MVD_IsNeedResponseCommand(MS_U8 u8Cmd)
{
    MS_BOOL ret;

    switch(u8Cmd)
    {
        case CMD_GET_AFD:
        case CMD_GET_SYNC_STAT:
        case CMD_RD_USER_WP:
        case CMD_DECODE_STATUS:
        case CMD_PARSER_READ_POSITION:
        case CMD_MVD_IDLE:
            ret = TRUE;
            break;
        default:
            ret = FALSE;
            break;
    }

    return ret;
}
#endif
#endif
