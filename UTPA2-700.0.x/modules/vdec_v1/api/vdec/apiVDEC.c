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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiVDEC.c
/// @brief  VDEC API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "apiVDEC.h"
#include "apiMJPEG.h"

#include "drvMMIO.h"
#include "drvSYS.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

//#if (VDEC_UTOPIA20)
#include "utopia.h"
#include "apiVDEC_v2.h"
//#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------
#define VDEC_ENABLE_LINK_WEAK_SYMBOL    1

    #define VDEC_ENABLE_MVD     1

#if !(defined(CHIP_T7) || defined(CHIP_A6))
    #define VDEC_ENABLE_HVD     1
#else
    #define VDEC_ENABLE_HVD     0
#endif

#define _VDEC_PTS_TABLE         0

#if 1  //temparary for bring up
#define _VDEC_AUTH_IPCheck_TEST_CODE_
#endif

#ifndef _VDEC_AUTH_IPCheck_TEST_CODE_
#if defined(VESTEL_FEATURE)
    #define VESTEL_FEATURE_ENABLE 1
#else
    #define VESTEL_FEATURE_ENABLE 0
#endif
#endif

#if VDEC_ENABLE_LINK_WEAK_SYMBOL
    #include "_apiVDEC_weak.h"
#else
    #include "../../drv/mvd/drvMVD.h"
    #include "../../drv/hvd/drvHVD.h"
    #include "../../drv/rvd/drvRVD.h"
#endif

#if defined(MSOS_TYPE_LINUX) || defined(MSOS_TYPE_ECOS)
    #define VDEC_ISR_MONITOR
#endif

#ifdef _VDEC_AUTH_IPCheck_TEST_CODE_
#define IPAUTH_VIDEO_MPEG2          (19)
#define IPAUTH_VIDEO_MPEG2_HD       (20)
#define IPAUTH_VIDEO_MPEG4          (21)
#define IPAUTH_VIDEO_MPEG4_SD       (22)
#define IPAUTH_VIDEO_MPEG4_HD       (23)
#define IPAUTH_VIDEO_DIVX_1080PHD   (24)
#define IPAUTH_VIDEO_DIVX_DRM       (25)
#define IPAUTH_VIDEO_DIVX_PLUS      (26)
#define IPAUTH_VIDEO_H264           (27)
#define IPAUTH_VIDEO_RM             (28)
#define IPAUTH_VIDEO_VC1            (29)
#define IPAUTH_VIDEO_WMV            (30)
#define IPAUTH_VIDEO_WMDRM_PD       (31)
#define IPAUTH_VIDEO_WMDRM_ND       (32)
#define IPAUTH_VIDEO_AVS            (33)
#define IPAUTH_VIDEO_FLV            (34)
#define IPAUTH_VIDEO_DIVX_QMOBILE   (35)
#define IPAUTH_VIDEO_DIVX_MOBILE    (36)
#define IPAUTH_VIDEO_DIVX_HT        (37)
#define IPAUTH_VIDEO_DIVX_720PHD    (38)
#define IPAUTH_VIDEO_MVC            (40)
MS_BOOL bModeSelectHD;
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);            //FROM #include "drvIPAUTH.h"
#endif

MS_BOOL bIsSupportDivxPlus;
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define E_VDEC_CB_MAX_NUM   (E_VDEC_CB_SUB + 1)

#define MVD_SRCMODE_MAX 5

#if 0
#define _VDEC_Memset(pDstAddr, u8value, u32Size)                \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = (u8value);                                 \
        }                                                       \
    } while (0)

#define _VDEC_Memcpy(pDstAddr, pSrcAddr, u32Size)               \
    do {                                                        \
        MS_U32 i = 0;                                           \
        volatile MS_U8 *dst = (volatile MS_U8 *)(pDstAddr);     \
        volatile MS_U8 *src = (volatile MS_U8 *)(pSrcAddr);     \
        for (i = 0; i < (u32Size); i++)                         \
        {                                                       \
            dst[i] = src[i];                                    \
        }                                                       \
    } while (0)
#else
#define _VDEC_Memcpy(pDstAddr, pSrcAddr, u32Size)   memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define _VDEC_Memset(pDstAddr, u8value, u32Size)    memset((pDstAddr), (u8value), (u32Size))
#endif

#define _MVD_RET_HANDLE(x)          \
    do                              \
    {                               \
        if (E_MVD_RET_OK != (x))    \
        {                           \
            return E_VDEC_FAIL;     \
        }                           \
    } while (0)

#define _HVD_RET_HANDLE(x)      \
    do                          \
    {                           \
        if (E_HVD_OK != (x))    \
        {                       \
            return E_VDEC_FAIL; \
        }                       \
    } while (0)

#define _MJPEG_RET_HANDLE(x)                \
    do                                      \
    {                                       \
        if (E_MJPEG_RET_SUCCESS != (x))     \
        {                                   \
            return E_VDEC_FAIL;             \
        }                                   \
    } while (0)

#define _RVD_RET_HANDLE(x)              \
    do                                  \
    {                                   \
        if (E_RVD_RET_SUCCESS != (x))   \
        {                               \
            return E_VDEC_FAIL;         \
        }                               \
    } while (0)

#define _BOOL_TO_VDEC_RESULT(rst, b)    \
    do                                  \
    {                                   \
        if (TRUE != (b))                \
        {                               \
            rst = E_VDEC_FAIL;          \
        }                               \
        else                            \
        {                               \
            rst = E_VDEC_OK;            \
        }                               \
    } while (0)

#define _BOOL_TO_MJPEG_RESULT(rst, b)   \
    do                                  \
    {                                   \
        if (TRUE != (b))                \
        {                               \
            rst = E_VDEC_FAIL;          \
        }                               \
        else                            \
        {                               \
            rst = E_VDEC_OK;            \
        }                               \
    } while (0)

#if VDEC_ENABLE_LINK_WEAK_SYMBOL

#define _RET_VDEC_HVD_LINT_CHECK()              \
    do                                          \
    {                                           \
        if (!MDrv_HVD_Init)                     \
        {                                       \
            return E_VDEC_RET_ILLEGAL_ACCESS;   \
        }                                       \
    } while (0)

#define _RET_VDEC_MVD_LINT_CHECK()              \
    do                                          \
    {                                           \
        if (!MDrv_MVD_Init)                     \
        {                                       \
            return E_VDEC_RET_ILLEGAL_ACCESS;   \
        }                                       \
    } while (0)

#define _RET_VDEC_RVD_LINT_CHECK()              \
    do                                          \
    {                                           \
        if (!MDrv_RVD_Init)                     \
        {                                       \
            return E_VDEC_RET_ILLEGAL_ACCESS;   \
        }                                       \
    } while (0)

#define _BOOL_VDEC_HVD_LINT_CHECK() \
    do                              \
    {                               \
        if (!MDrv_HVD_Init)         \
        {                           \
            return FALSE;           \
        }                           \
    } while (0)

#define _BOOL_VDEC_MVD_LINT_CHECK() \
    do                              \
    {                               \
        if (!MDrv_MVD_Init)         \
        {                           \
            return FALSE;           \
        }                           \
    } while (0)

#define _BOOL_VDEC_RVD_LINT_CHECK() \
    do                              \
    {                               \
        if (!MDrv_RVD_Init)         \
        {                           \
            return FALSE;           \
        }                           \
    } while (0)
#else // VDEC_ENABLE_LINK_WEAK_SYMBOL

#define _RET_VDEC_MVD_LINT_CHECK()
#define _RET_VDEC_HVD_LINT_CHECK()
#define _RET_VDEC_RVD_LINT_CHECK()
#define _BOOL_VDEC_MVD_LINT_CHECK()
#define _BOOL_VDEC_HVD_LINT_CHECK()
#define _BOOL_VDEC_RVD_LINT_CHECK()

#endif //VDEC_ENABLE_LINK_WEAK_SYMBOL

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#if defined(VDEC_ISR_MONITOR)
#define E_VDEC_EVENT_ISR_EVENT_CHANGE   0x80000000
#endif

#define VDEC_U32_MAX            0xffffffffUL
#define MAX_VDEC_DQNUM          32
#define VDEC_DQ_EMPTY           0xff
#define VDEC_INFO(x)    if (_Attr.eDbgMsgLevel >= E_VDEC_DBG_LEVEL_INFO) { (x); }

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_VDEC_DECODER_MVD = 0,
    E_VDEC_DECODER_HVD,
    E_VDEC_DECODER_MJPEG,
    E_VDEC_DECODER_RVD,
    E_VDEC_DECODER_NONE,
} VDEC_Decoder;

typedef enum
{
    E_VDEC_STREAM_0 = 0,
    E_VDEC_STREAM_NUM_MAX,
} VDEC_STREAM_NUM;

typedef struct
{
    MS_U32          u32EventBitMap;
    VDEC_EventCb    pVdecEventCb;
    void*           pParam;
} VDEC_EventInfo;

typedef struct
{
    MS_BOOL bInit;
    VDEC_DbgLevel   eDbgMsgLevel;
    VDEC_CodecType  eCodecType;
    VDEC_Decoder    eDecoder;
    VDEC_SrcMode    eSrcMode;
    VDEC_ErrCode    eErrCode;
    VDEC_CCFormat   eCCFormat;
    VDEC_EventInfo VdecEventInfo[E_VDEC_CB_MAX_NUM];
} VDEC_Attr;

typedef struct
{
    //******** One Pending Buffer ********//
    MS_BOOL  bOnePendingBuffer;
    //******** HW Buffer ReMapping ********//
    MS_BOOL  bHWBufferReMapping;
    //******** Frame Rate Handling ********//
    MS_BOOL  bFrameRateHandling;
    MS_U32   u32PreSetFrameRate;
} VDEC_Pre_Ctrl;

//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------
static void             _VDEC_InitLocalVar(void);
static VDEC_Decoder     _VDEC_GetDecoderByCodecType(VDEC_CodecType eCodecType);
static MVD_CodecType    _VDEC_Map2MVDCodecType(VDEC_CodecType vdecCodecType);
static MVD_SrcMode      _VDEC_Map2MVDSrcMode(VDEC_SrcMode vdecSrcMode);
static void             _VDEC_MVDCfgDivX(VDEC_CodecType vdecCodecType);
static MS_BOOL          _VDEC_IsIdle(void);
static void             _VDEC_IsrProc(void);
static MS_U32           _VDEC_GetESBuffVacancy(void);

#if defined(VDEC_ISR_MONITOR)
static MS_BOOL          _VDEC_SYS_Init(void);
static void             _VDEC_MonitorISR_Proc(void);
#endif
#if 0
static MS_BOOL          _VDEC_SYS_Exit(void);
#endif

static HVD_ClockSpeed _VDEC_Map2HVDClockSpeed(VDEC_ClockSpeed ClockSpeed);
static MVD_ClockSpeed _VDEC_Map2MVDClockSpeed(VDEC_ClockSpeed ClockSpeed);
static VDEC_Result _VDEC_SetVPUClockSpeed(VDEC_ClockSpeed ClockSpeed);
static VDEC_Result _VDEC_SetHVDClockSpeed(VDEC_ClockSpeed ClockSpeed);
static VDEC_Result _VDEC_SetMVDClockSpeed(VDEC_ClockSpeed ClockSpeed);
static VDEC_Result _MApi_VDEC_GetFlushPatternEntryNum(MS_U32* param);

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MSIF_Version _api_vdec_version =
{
    .DDI = { VDEC_API_VERSION, },
};

static VDEC_Info        _api_vdec_info;

void* pu32VDEC_Inst = NULL;




static VDEC_Attr        _Attr;
static VDEC_Pre_Ctrl    _Pre_Ctrl = {0};
static MS_BOOL          bDropErrFrm = FALSE;
static MS_BOOL          bTurboMode = FALSE;
static MS_BOOL          bEsBufMgmt = FALSE;
static VDEC_DispFrame   VDEC_DispQ[E_VDEC_STREAM_NUM_MAX][MAX_VDEC_DQNUM];

static MS_S32           _s32VdecMutexId = -1;
static MS_U8            _u8VdecMutex[] = {"VDEC_Mutex"};
#if defined(VDEC_ISR_MONITOR)
#define VDEC_STACK_SIZE 4096
static MS_U8            _VdecStack[VDEC_STACK_SIZE];
static MS_BOOL          _bVdecIsrMntr = TRUE; //Coverity:for infinite loop
static MS_BOOL          _IsSysEnable = FALSE;
static MS_S32           _s32VdecEventId = -1;
static MS_U8            _u8VdecEvent[] = {"VDEC_Event"};
static MS_S32           _s32VdecTaskId = -1;
static MS_U8            _u8VdecTask[] = {"VDEC_Task"};
static MS_U32           _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
static MS_U32           _u32PreVdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
#else
static MS_U32           _u32VdecIsrEventFlag = 0;
#endif

static MS_U32 u32VdecInitParamVersion = 0;
static VDEC_InitParam       _vdecInitParam;
static VDEC_InitParam_EX1   _vdecInitParam_ex1;

static MS_S32 _s32DqMutexId = -1;
static MS_U8  _u8DqMutex[] = {"VDEC_DQ_Mutex"};

static HVD_FrmPackingSEI _stFrmPacking;
EN_POWER_MODE _prev_u16PowerState;

//Configure MVD parser will be disabled/enabled when bitstream buffer overflow
static const MS_U8 bMvdParserDisable[MVD_SRCMODE_MAX] =
{
    1, //E_MVD_FILE_MODE   : disable
    1, //E_MVD_SLQ_MODE    : disable
    0, //E_MVD_TS_MODE     : enable
    1, //E_MVD_SLQ_TBL_MODE: disable
    0  //E_MVD_TS_FILE_MODE: DISABLE_PKT_LEN
};

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#if _VDEC_PTS_TABLE
#define STS_SIZE        1024
#define STS_NULL_PTS    0
MS_U64 u64PtsTable[STS_SIZE];
MS_U32 u32StsNum = 0;

MS_BOOL _VDEC_PtsTableIsFull(void)
{
    return (u32StsNum >= (STS_SIZE-32));
}

void _VDEC_PtsTableInit(void)
{
    MS_U32 i = 0;
    for (i=0; i<STS_SIZE; i++)
    {
        u64PtsTable[i] = STS_NULL_PTS;
    }
    u32StsNum = 0;
}

//Sort and put, always put the bigger one at u64PtsTable[0]
void _VDEC_PtsTablePut(MS_U64 u64Pts)
{
    MS_U32 i = 0;
    MS_U32 u32ResortIdx = 0xffffffffUL;
    //MS_U64 u64Tmp;

    for (i=0; i<STS_SIZE; i++)
    {
        if (u64Pts >= u64PtsTable[i])
        {
            //printf("Put [%llu] in idx = %ld\n", u64Pts, i);
            u32ResortIdx = i;
            break;
        }
    }

    if (u32ResortIdx == 0xffffffffUL)
    {
        printf("Err. Cannot find place to put [%llu](%llu)\n", u64Pts, u64PtsTable[0]);
        return;
    }
    u32StsNum++; //since we're gonna add u64Pts

    if (u32StsNum > STS_SIZE)
    {
        printf("Err. Overflow!!! DecFrmCnt=%ld\n", MApi_VDEC_GetFrameCnt());
        return;
    }

    //move those smaller than u64Pts to reserve u64PtsTable[u32ResortIdx] for it
    for (i=(u32StsNum-1); i>u32ResortIdx; i--)
    {
        u64PtsTable[i] = u64PtsTable[i-1];
    }

    u64PtsTable[u32ResortIdx] = u64Pts;
    //printf("_______ptsPut[%ld]=%llu @ %ld\n", u32StsNum, u64Pts, u32ResortIdx);
}

MS_U64 _VDEC_PtsTableGet(void)
{
    MS_U64 u64PtsMin = 0;

    if (u32StsNum>=1)
    {
        u32StsNum--;
        u64PtsMin = u64PtsTable[u32StsNum];
    }
    //printf("_______ptsGet[%ld]=%llu \n", u32StsNum, u64PtsMin);

    return u64PtsMin;
}
#endif


VDEC_Result MApi_VDEC_DbgCmd(MS_U32 u8Cmd, MS_U32 u32CmdArg)
{
    VDEC_Result retVal;
    MVD_CmdArg stCmdArg;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _VDEC_Memset(&stCmdArg, 0x00, sizeof(MVD_CmdArg));

            if (u32CmdArg)
            {
                stCmdArg.Arg0 = (MS_U8) (u32CmdArg & 0x000000ff);
                stCmdArg.Arg1 = (MS_U8) ((u32CmdArg & 0x0000ff00) >> 8);
                stCmdArg.Arg2 = (MS_U8) ((u32CmdArg & 0x00ff0000) >> 16);
                stCmdArg.Arg3 = (MS_U8) ((u32CmdArg & 0xff000000) >> 24);
            }
            if (MDrv_MVD_MVDCommand(u8Cmd, &stCmdArg))
            {
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_RET_TIMEOUT;
            }
            break;

        case E_VDEC_DECODER_HVD:
            if (E_HVD_OK == MDrv_HVD_SetCmd_Dbg((MS_U32) u8Cmd,
                                                         u32CmdArg))
            {
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_RET_TIMEOUT;
            }
            break;

        case E_VDEC_DECODER_RVD:
            if (E_RVD_RET_SUCCESS == MDrv_RVD_DbgSetCmd((RVD_User_Cmd) u8Cmd,
                                                         u32CmdArg))
            {
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_RET_TIMEOUT;
            }
            break;
        case E_VDEC_DECODER_MJPEG:
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_DbgSetData(MS_U32 u32Addr, MS_U32 u32Data)
{
    VDEC_Result retVal  = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;

        case E_VDEC_DECODER_HVD:
            MDrv_HVD_SetMem_Dbg(u32Addr, u32Data);
            break;

        case E_VDEC_DECODER_MJPEG:
            {
                MJPEG_Result ret = MApi_MJPEG_DbgSetData(u32Addr, u32Data);

                if (E_MJPEG_RET_SUCCESS != ret)
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        case E_VDEC_DECODER_RVD:
            MDrv_RVD_DbgSetData(u32Addr, u32Data);
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_DbgGetData(MS_U32 u32Addr, MS_U32 *u32Data)
{
    VDEC_Result retVal  = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_DbgGetData(u32Addr, u32Data);

                if (E_MVD_RET_OK != ret)
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        case E_VDEC_DECODER_HVD:
            *u32Data = MDrv_HVD_GetMem_Dbg(u32Addr);

            break;

        case E_VDEC_DECODER_MJPEG:
            {
                MJPEG_Result ret = MApi_MJPEG_DbgGetData(u32Addr, u32Data);

                if (E_MJPEG_RET_SUCCESS != ret)
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        case E_VDEC_DECODER_RVD:
            {
                RVD_Result ret = MDrv_RVD_DbgGetData(u32Addr, u32Data);

                if (E_RVD_RET_SUCCESS != ret)
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;

            break;
    }

    return retVal;
}


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_BOOL _VDEC_DqMutexLock(void)
{
    if (_s32DqMutexId != -1)
    {
        return MsOS_ObtainMutex(_s32DqMutexId, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _VDEC_DqMutexUnlock(void)
{
    if (_s32DqMutexId != -1)
    {
        return MsOS_ReleaseMutex(_s32DqMutexId);
    }
    else
    {
        return FALSE;
    }
}

static void _VDEC_InitDq(MS_U8 u8StreamNum)
{
    MS_U32 i=0;

    //Create Mutex
    if(_s32DqMutexId < 0)
    {
        _s32DqMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)_u8DqMutex, MSOS_PROCESS_SHARED);
    }
    if(_s32DqMutexId < 0)
    {
        printf("%s MsOS_CreateMutex failed!!\n", __FUNCTION__);
        return;
    }

    _VDEC_DqMutexLock();
    for (i=0; i<MAX_VDEC_DQNUM; i++)
    {
        VDEC_DispQ[u8StreamNum][i].u32Idx = VDEC_DQ_EMPTY;
    }
    _VDEC_DqMutexUnlock();
}

static void _VDEC_DqMutexExit(void)
{
    if(_s32DqMutexId != -1)
    {
        MsOS_DeleteMutex(_s32DqMutexId);
        _s32DqMutexId = -1;
    }
}

static VDEC_DispFrame* _VDEC_MallocDq(MS_U8 u8StreamNum)
{
    VDEC_DispFrame* pDisp = NULL;
    MS_U32 i=0;

    if (u8StreamNum >= E_VDEC_STREAM_NUM_MAX)
    {
        return NULL;
    }
    _VDEC_DqMutexLock();
    for (i=0; i<MAX_VDEC_DQNUM; i++)
    {
        if (VDEC_DispQ[u8StreamNum][i].u32Idx == VDEC_DQ_EMPTY)
        {
            VDEC_DispQ[u8StreamNum][i].u32Idx = i;
            pDisp = &VDEC_DispQ[u8StreamNum][i];
            //printf("_VDEC_MallocDq %ld\n", i);
            break;
        }
    }
    _VDEC_DqMutexUnlock();
    return pDisp;
}

static void _VDEC_FreeDq(MS_U8 u8StreamNum, VDEC_DispFrame* pDq)
{
    if (pDq && (VDEC_DQ_EMPTY != pDq->u32Idx))
    {
        _VDEC_DqMutexLock();
        VDEC_DispQ[u8StreamNum][pDq->u32Idx].u32Idx = VDEC_DQ_EMPTY;
        _VDEC_DqMutexUnlock();
    }
}

static MS_BOOL _VDEC_IsDqValid(VDEC_DispFrame* pDq)
{
    MS_BOOL bRet = FALSE;
    if (pDq)
    {
        _VDEC_DqMutexLock();
        bRet = (pDq->u32Idx == VDEC_DQ_EMPTY);
        _VDEC_DqMutexUnlock();
    }
    return bRet;
}

#if VDEC_ENABLE_LINK_WEAK_SYMBOL
static void __attribute__((unused)) _VDEC_LinkWeakSymbolPatch(void)
{
#if VDEC_ENABLE_MVD
    MDrv_MVD_LinkWeakSymbolPatch();
#endif
#if VDEC_ENABLE_HVD
    MDrv_HVD_LinkWeakSymbolPatch();
#endif
}
#endif

static void _VDEC_InitLocalVar(void)
{
    MS_U32 u32Tmp = 0;

    _Attr.bInit = FALSE;
    _Attr.eDbgMsgLevel = VDEC_DEFAULT_DBG_MSG_LEVEL;
    _Attr.eCodecType = E_VDEC_CODEC_TYPE_NONE;
    _Attr.eDecoder = E_VDEC_DECODER_NONE;
    _Attr.eErrCode = E_VDEC_ERR_CODE_BASE;

#if defined(VDEC_ISR_MONITOR)
    MsOS_ClearEvent(_s32VdecEventId, VDEC_U32_MAX);
    _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
    _u32PreVdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
    MsOS_SetEvent(_s32VdecEventId, E_VDEC_EVENT_ISR_EVENT_CHANGE);
#else
    _u32VdecIsrEventFlag = 0;
#endif
    _prev_u16PowerState = E_POWER_MECHANICAL;

    for (u32Tmp = 0; u32Tmp < E_VDEC_CB_MAX_NUM; u32Tmp++)
    {
        _Attr.VdecEventInfo[u32Tmp].u32EventBitMap = 0;
        _Attr.VdecEventInfo[u32Tmp].pVdecEventCb = NULL;
        _Attr.VdecEventInfo[u32Tmp].pParam = NULL;
    }
#if 0
    if(u32VdecInitParamVersion == 0)
    {
        _VDEC_Memset(&_vdecInitParam, 0, sizeof(VDEC_InitParam));
    }
    else if(u32VdecInitParamVersion == 1)
    {
        _VDEC_Memset(&_vdecInitParam_ex1, 0, sizeof(VDEC_InitParam_EX1));
    }
#endif

    _VDEC_InitDq(E_VDEC_STREAM_0);
#if _VDEC_PTS_TABLE
    _VDEC_PtsTableInit();
#endif
}


#if defined(VDEC_ISR_MONITOR)
static MS_BOOL _VDEC_SYS_Init(void)
{
    //Create Mutex
    if(_s32VdecMutexId < 0)
    {
        _s32VdecMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)_u8VdecMutex, MSOS_PROCESS_SHARED);
    }
    if(_s32VdecMutexId < 0)
    {
        printf("MsOS_CreateMutex failed!!\n");
        return FALSE;
    }

    //Create Event Group
    if(_s32VdecEventId < 0)
    {
        _s32VdecEventId = MsOS_CreateEventGroup((char *)_u8VdecEvent);
    }
    if(_s32VdecEventId < 0)
    {
        printf("MsOS_CreateEventGroup failed!!\n");
        MsOS_DeleteMutex(_s32VdecMutexId);
        return FALSE;
    }

    //Create Vdec event monitor Task
    _bVdecIsrMntr = TRUE;
    if(_s32VdecTaskId < 0)
    {
        _s32VdecTaskId = MsOS_CreateTask((TaskEntry)_VDEC_MonitorISR_Proc,
                                        (MS_U32)NULL,
                                        E_TASK_PRI_MEDIUM,
                                        TRUE,
                                        (void *)_VdecStack,
                                        VDEC_STACK_SIZE,
                                        (char *)_u8VdecTask);
    }
    if(_s32VdecTaskId < 0)
    {
        printf("MsOS_CreateTask failed!!\n");
        MsOS_DeleteEventGroup(_s32VdecEventId);
        MsOS_DeleteMutex(_s32VdecMutexId);
        return FALSE;
    }
    return TRUE;
}
#else
static MS_BOOL _VDEC_MutexInit(void)
{
    if(_s32VdecMutexId < 0)
    {
        _s32VdecMutexId = MsOS_CreateMutex(E_MSOS_FIFO, (char*)_u8VdecMutex, MSOS_PROCESS_SHARED);
    }
    return (_s32VdecMutexId != -1);
}

static MS_BOOL _VDEC_MutexExit(void)
{
    MS_BOOL bRet = TRUE;
    if(_s32VdecMutexId != -1)
    {
        MsOS_DeleteMutex(_s32VdecMutexId);
        _s32VdecMutexId = -1;
    }
    return bRet;
}

#endif

#if 0
static MS_BOOL _VDEC_SYS_Exit(void)
{
    //Create Mutex
    if(_s32VdecMutexId != -1)
    {
        MsOS_DeleteMutex(_s32VdecMutexId);
        _s32VdecMutexId = -1;
    }

    if (_s32VdecEventId != -1)
    {
        MsOS_DeleteEventGroup(_s32VdecEventId);
        _s32VdecEventId = -1;
    }

    if (_s32VdecTaskId != -1)
    {
        MsOS_DeleteTask(_s32VdecTaskId);
        _s32VdecTaskId = -1;
    }
    return TRUE;
}
#endif

// Mutex function for VDEC API
static MS_BOOL _VDEC_MutexLock(void)
{
    if (_s32VdecMutexId != -1)
    {
        return MsOS_ObtainMutex(_s32VdecMutexId, MSOS_WAIT_FOREVER);
    }
    else
    {
        return FALSE;
    }
}

static MS_BOOL _VDEC_MutexUnlock(void)
{
    if (_s32VdecMutexId != -1)
    {
        return MsOS_ReleaseMutex(_s32VdecMutexId);
    }
    else
    {
        return FALSE;
    }
}

#ifdef _VDEC_AUTH_IPCheck_TEST_CODE_
static VDEC_Decoder _VDEC_GetDecoderByCodecType(VDEC_CodecType eCodecType)
{
    VDEC_Decoder eDecoder;

    bModeSelectHD = FALSE;
    bIsSupportDivxPlus = FALSE;

    switch (eCodecType)
    {
        case E_VDEC_CODEC_TYPE_MPEG2:
            if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2_HD) == TRUE))
            {
                if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG2_HD) == TRUE)
                    bModeSelectHD =TRUE;
                else
                    bModeSelectHD =FALSE;
                eDecoder = E_VDEC_DECODER_MVD;
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;
        case E_VDEC_CODEC_TYPE_H263:
        case E_VDEC_CODEC_TYPE_MPEG4:
            if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_SD) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_HD) == TRUE))
            {
                if(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_MPEG4_HD) == TRUE)
                    bModeSelectHD =TRUE;
                else
                    bModeSelectHD =FALSE;
                eDecoder = E_VDEC_DECODER_MVD;

                if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE)
                {
                    bIsSupportDivxPlus = TRUE;
                }
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;
        case E_VDEC_CODEC_TYPE_DIVX311:
        case E_VDEC_CODEC_TYPE_DIVX412:
            if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_1080PHD) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_DRM) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_QMOBILE) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_MOBILE) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HT) == TRUE))
            {
                if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_1080PHD) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_DRM) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_PLUS) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_DIVX_HT) == TRUE))
                    bModeSelectHD =TRUE;
                else
                    bModeSelectHD =FALSE;
                eDecoder = E_VDEC_DECODER_MVD;
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;

        case E_VDEC_CODEC_TYPE_FLV:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_FLV) == TRUE)
            {
                bModeSelectHD = TRUE;
                eDecoder = E_VDEC_DECODER_MVD;
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;

        case E_VDEC_CODEC_TYPE_VC1_ADV:
        case E_VDEC_CODEC_TYPE_VC1_MAIN:
            if ((MDrv_AUTH_IPCheck(IPAUTH_VIDEO_VC1) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMV) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMDRM_PD) == TRUE)
                ||(MDrv_AUTH_IPCheck(IPAUTH_VIDEO_WMDRM_ND) == TRUE))
            {

                bModeSelectHD =TRUE;
                eDecoder = E_VDEC_DECODER_MVD;
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;

        case E_VDEC_CODEC_TYPE_RV8:
        case E_VDEC_CODEC_TYPE_RV9:

        #if VDEC_ENABLE_HVD
            eDecoder = E_VDEC_DECODER_HVD;
        #else
            eDecoder = E_VDEC_DECODER_NONE;
        #endif

            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_RM) == TRUE)
                bModeSelectHD =TRUE;
            else
                eDecoder = E_VDEC_DECODER_NONE;

            break;
        case E_VDEC_CODEC_TYPE_H264:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_H264) == TRUE)
            {
                bModeSelectHD =TRUE;
                eDecoder = E_VDEC_DECODER_HVD;
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;
        case E_VDEC_CODEC_TYPE_AVS:
            if (MDrv_AUTH_IPCheck(IPAUTH_VIDEO_AVS) == TRUE)
            {
                bModeSelectHD =TRUE;
                eDecoder = E_VDEC_DECODER_HVD;
            }
            else
            {
                eDecoder = E_VDEC_DECODER_NONE;
            }
            break;
        case E_VDEC_CODEC_TYPE_MJPEG:
            eDecoder = E_VDEC_DECODER_MJPEG;

            break;

        default:
            eDecoder = E_VDEC_DECODER_NONE;

            break;
    }

    return eDecoder;
}
#else
static VDEC_Decoder _VDEC_GetDecoderByCodecType(VDEC_CodecType eCodecType)
{
    VDEC_Decoder eDecoder;
    switch (eCodecType)
    {
        case E_VDEC_CODEC_TYPE_MPEG2:
        case E_VDEC_CODEC_TYPE_H263:
        case E_VDEC_CODEC_TYPE_MPEG4:
        #if (!VESTEL_FEATURE_ENABLE)
        case E_VDEC_CODEC_TYPE_DIVX311:
        case E_VDEC_CODEC_TYPE_DIVX412:
        case E_VDEC_CODEC_TYPE_FLV:
        case E_VDEC_CODEC_TYPE_VC1_ADV:
        case E_VDEC_CODEC_TYPE_VC1_MAIN:
        #endif
            eDecoder = E_VDEC_DECODER_MVD;
            bIsSupportDivxPlus = TRUE;
            break;

        case E_VDEC_CODEC_TYPE_RV8:
        case E_VDEC_CODEC_TYPE_RV9:

        #if VDEC_ENABLE_HVD
            eDecoder = E_VDEC_DECODER_HVD;
        #else
            eDecoder = E_VDEC_DECODER_NONE;
        #endif
            break;
        #if (!VESTEL_FEATURE_ENABLE)
        case E_VDEC_CODEC_TYPE_H264:
        case E_VDEC_CODEC_TYPE_AVS:
            eDecoder = E_VDEC_DECODER_HVD;

            break;
        #endif
        case E_VDEC_CODEC_TYPE_MJPEG:
            eDecoder = E_VDEC_DECODER_MJPEG;

            break;

        default:
            eDecoder = E_VDEC_DECODER_NONE;

            break;
    }

    return eDecoder;
}
#endif

static MVD_CodecType _VDEC_Map2MVDCodecType(VDEC_CodecType vdecCodecType)
{
    MVD_CodecType mvdCodecType = E_MVD_CODEC_UNKNOWN;

    switch (vdecCodecType)
    {
        case E_VDEC_CODEC_TYPE_MPEG2:
            mvdCodecType = E_MVD_CODEC_MPEG2;
            break;

        case E_VDEC_CODEC_TYPE_H263:
            mvdCodecType = E_MVD_CODEC_MPEG4_SHORT_VIDEO_HEADER;
            break;

        case E_VDEC_CODEC_TYPE_MPEG4:
        case E_VDEC_CODEC_TYPE_DIVX412:
            mvdCodecType = E_MVD_CODEC_MPEG4;
            break;

        case E_VDEC_CODEC_TYPE_DIVX311:
            mvdCodecType = E_MVD_CODEC_DIVX311;
            break;

        case E_VDEC_CODEC_TYPE_FLV:
            mvdCodecType = E_MVD_CODEC_FLV;
            break;

        case E_VDEC_CODEC_TYPE_VC1_ADV:
            mvdCodecType = E_MVD_CODEC_VC1_ADV;
            break;

        case E_VDEC_CODEC_TYPE_VC1_MAIN:
            mvdCodecType = E_MVD_CODEC_VC1_MAIN;
            break;

        default:
            mvdCodecType = E_MVD_CODEC_UNKNOWN;
            break;
    }

    return mvdCodecType;
}

static MVD_SrcMode _VDEC_Map2MVDSrcMode(VDEC_SrcMode vdecSrcMode)
{
    MVD_SrcMode mvdSrcMode = E_MVD_TS_MODE;

    switch (vdecSrcMode)
    {
        case E_VDEC_SRC_MODE_DTV:
            mvdSrcMode = E_MVD_TS_MODE;
            break;

        case E_VDEC_SRC_MODE_TS_FILE:
            mvdSrcMode = E_MVD_TS_FILE_MODE;
            break;

        case E_VDEC_SRC_MODE_FILE:
            mvdSrcMode = E_MVD_SLQ_TBL_MODE;
            break;

        default:
            mvdSrcMode = E_MVD_TS_MODE;
            break;
    }

    return mvdSrcMode;
}

static VDEC_FrameType _VDEC_MapFrmType2MVD(MVD_PicType vdecFrmType)
{
    VDEC_FrameType mvdFrmType = E_VDEC_FRM_TYPE_OTHER;

    switch (vdecFrmType)
    {
        case E_MVD_PIC_I:
            mvdFrmType = E_VDEC_FRM_TYPE_I;
            break;

        case E_MVD_PIC_P:
            mvdFrmType = E_VDEC_FRM_TYPE_P;
            break;

        case E_MVD_PIC_B:
            mvdFrmType = E_VDEC_FRM_TYPE_B;
            break;

        default:
            mvdFrmType = E_VDEC_FRM_TYPE_OTHER;
            break;
    }

    return mvdFrmType;
}

static VDEC_FrameType _VDEC_MapFrmType2HVD(HVD_Frm_Type vdecFrmType)
{
    VDEC_FrameType hvdFrmType = E_VDEC_FRM_TYPE_OTHER;

    switch (vdecFrmType)
    {
        case E_HVD_FRM_TYPE_I:
            hvdFrmType = E_VDEC_FRM_TYPE_I;
            break;

        case E_HVD_FRM_TYPE_P:
            hvdFrmType = E_VDEC_FRM_TYPE_P;
            break;

        case E_HVD_FRM_TYPE_B:
            hvdFrmType = E_VDEC_FRM_TYPE_B;
            break;

        default:
            hvdFrmType = E_VDEC_FRM_TYPE_OTHER;
            break;
    }

    return hvdFrmType;
}

static VDEC_FrameType _VDEC_MapFrmType2MJPEG(MJPEG_FrameType vdecFrmType)
{
    VDEC_FrameType mjpegFrmType = E_VDEC_FRM_TYPE_OTHER;

    switch (vdecFrmType)
    {
        case E_MJPEG_FRM_TYPE_I:
            mjpegFrmType = E_VDEC_FRM_TYPE_I;
            break;

        case E_MJPEG_FRM_TYPE_OTHER:
        default:
            mjpegFrmType = E_VDEC_FRM_TYPE_OTHER;
            break;
    }

    return mjpegFrmType;
}

static void _VDEC_MVDCfgDivX(VDEC_CodecType vdecCodecType)
{
    switch (vdecCodecType)
    {
        case E_VDEC_CODEC_TYPE_MPEG4:
            MDrv_MVD_SetDivXCfg(0x01, 0x01);
            break;

        case E_VDEC_CODEC_TYPE_DIVX412:
            MDrv_MVD_SetDivXCfg(0x11, 0x01);
            break;

        case E_VDEC_CODEC_TYPE_DIVX311:
            MDrv_MVD_SetDivXCfg(0x00, 0x00);
            break;

        default:
            break;
    }

    return;
}

MS_BOOL _VDEC_IsIdle(void)
{
    MS_BOOL bRet;

    if (!_Attr.bInit)
    {
        return FALSE;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            bRet = MDrv_MVD_IsIdle();
            break;

        case E_VDEC_DECODER_HVD:
            bRet = MDrv_HVD_IsIdle();
            break;

        case E_VDEC_DECODER_MJPEG:
            bRet = MApi_MJPEG_IsIdle();
            break;

        case E_VDEC_DECODER_RVD:
            bRet = MDrv_RVD_IsIdle();
            break;

        default:
            bRet = FALSE;
            break;
    }

    return bRet;
}

#if defined(VDEC_ISR_MONITOR)
//------------------------------------------------------------------------------
/// The task for monitoring vdec isr
/// @return -None
//------------------------------------------------------------------------------
static void _VDEC_MonitorISR_Proc(void)
{
    MS_U32 u32VdecEventFlag = 0;
    MS_U32 cb_idx = 0;

    while (_bVdecIsrMntr == TRUE)
    {
        if(_u32PreVdecIsrEventFlag != _u32VdecIsrEventFlag)
        {
            MsOS_ClearEvent(_s32VdecEventId, VDEC_U32_MAX);
            _u32PreVdecIsrEventFlag = _u32VdecIsrEventFlag;
        }

        MsOS_WaitEvent(_s32VdecEventId,
            _u32PreVdecIsrEventFlag,
            &u32VdecEventFlag,
            E_OR_CLEAR,
            MSOS_WAIT_FOREVER);

        if ((u32VdecEventFlag & ~E_VDEC_EVENT_ISR_EVENT_CHANGE) & _u32VdecIsrEventFlag)
        {
            for (cb_idx = 0; cb_idx < E_VDEC_CB_MAX_NUM; cb_idx++)
            {
                MS_U32 u32Event = u32VdecEventFlag &_Attr.VdecEventInfo[cb_idx].u32EventBitMap;

                if ((u32Event) &&
                    (_Attr.VdecEventInfo[cb_idx].pVdecEventCb))
                {
                    _Attr.VdecEventInfo[cb_idx].pVdecEventCb(u32Event,
                                                        _Attr.VdecEventInfo[cb_idx].pParam);
                }
            }
        }
    }
}
#endif

//------------------------------------------------------------------------------
/// Set decoder event and register it's callback function
/// @param pfn \b In : callback function
/// @return -VDEC_Result
//------------------------------------------------------------------------------
static void _VDEC_IsrProc(void)
{
#if !defined(VDEC_ISR_MONITOR)
    MS_U32 i = 0;
#endif

    _VDEC_MutexLock();

#if defined(VDEC_ISR_MONITOR)
    if (_u32VdecIsrEventFlag & ~E_VDEC_EVENT_ISR_EVENT_CHANGE)
#else
    if (_u32VdecIsrEventFlag)
#endif
    {
        MS_U32 eEventFlag = E_VDEC_EVENT_OFF;

        switch (_Attr.eDecoder)
        {
            case E_VDEC_DECODER_MVD:
            {
                MS_U32 u32MVDIsrEvent = MDrv_MVD_GetIsrEvent();
                if (u32MVDIsrEvent != E_MVD_EVENT_DISABLE_ALL)
                {
#ifdef _MVD_EVENT_DBG
                    if (u32MVDIsrEvent & E_MVD_EVENT_DISP_VSYNC)
                    {
                        eEventFlag |= E_VDEC_EVENT_DISP_ONE;
                    }
#endif
                    if (u32MVDIsrEvent & E_MVD_EVENT_DISP_RDY)
                    {
                        eEventFlag |= E_VDEC_EVENT_DISP_INFO_RDY;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_SEQ_FOUND)
                    {
                        eEventFlag |= E_VDEC_EVENT_SEQ_HDR_FOUND;

                        if (MDrv_MVD_IsSeqChg())
                        {
                            eEventFlag |= E_VDEC_EVENT_DISP_INFO_CHG;
                        }
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_USER_DATA)
                    {
#if !defined(MVD_SUPPORT_X4_CC)
                        //exclude this for X4_CC for this is UserData in "decoding" order
                        eEventFlag |= E_VDEC_EVENT_USER_DATA_FOUND;
#endif
                        MS_U8 u8Afd = MDrv_MVD_GetActiveFormat();

                        if ((u8Afd != 0) && (u8Afd != 0xff))
                        {
                            eEventFlag |= E_VDEC_EVENT_AFD_FOUND;
                        }
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_USER_DATA_DISP)
                    {
                        //this is UserData in "display" order
                        eEventFlag |= E_VDEC_EVENT_USER_DATA_FOUND;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_UNMUTE)
                    {
                        eEventFlag |= E_VDEC_EVENT_VIDEO_UNMUTE;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_FIRST_FRAME)
                    {
                        eEventFlag |= E_VDEC_EVENT_FIRST_FRAME;
                        eEventFlag |= E_VDEC_EVENT_DISP_INFO_CHG;
                    }
                    if (u32MVDIsrEvent & E_MVD_EVENT_DEC_I)
                    {
                        eEventFlag |= E_VDEC_EVENT_DEC_I;
                    }

#if defined(VDEC_ISR_MONITOR)
                    MsOS_SetEvent(_s32VdecEventId, eEventFlag);
#else
                    for (i = 0; i < E_VDEC_CB_MAX_NUM; i++)
                    {
                        MS_U32 u32Event = eEventFlag &_Attr.VdecEventInfo[i].u32EventBitMap;

                        if ((u32Event) &&
                            (_Attr.VdecEventInfo[i].pVdecEventCb))
                        {
                            _VDEC_MutexUnlock();
                            _Attr.VdecEventInfo[i].pVdecEventCb(u32Event,
                                                                _Attr.VdecEventInfo[i].pParam);
                            _VDEC_MutexLock();
                        }
                    }
#endif
                }
                break;
            }
            case E_VDEC_DECODER_HVD:
            {
                MS_U32 u32HVDIsrEvent = E_HVD_ISR_NONE;
                if (MDrv_HVD_GetISRInfo(&u32HVDIsrEvent))
                {
                    if (u32HVDIsrEvent & E_HVD_ISR_DISP_ONE)
                    {
                        eEventFlag |= E_VDEC_EVENT_DISP_ONE;
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DISP_REPEAT)
                    {
                        eEventFlag |= E_VDEC_EVENT_DISP_REPEAT;
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DISP_WITH_CC)
                    {
                        eEventFlag |= (E_VDEC_EVENT_DISP_WITH_CC |
                                       E_VDEC_EVENT_USER_DATA_FOUND);
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DISP_FIRST_FRM)
                    {
                        eEventFlag |= E_VDEC_EVENT_VIDEO_UNMUTE;
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DEC_ONE)
                    {
                        eEventFlag |= E_VDEC_EVENT_DEC_ONE;
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DEC_I)
                    {
                        eEventFlag |= E_VDEC_EVENT_DEC_I;
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DEC_HW_ERR)
                    {
                        eEventFlag |= E_VDEC_EVENT_DEC_ERR;
                    }
#if 1 //Currently, for HVD, E_VDEC_EVENT_USER_DATA_FOUND is triggerred by E_HVD_ISR_DISP_WITH_CC
                    if (u32HVDIsrEvent &E_HVD_ISR_DEC_CC_FOUND )
                    {
                        eEventFlag |= E_VDEC_EVENT_USER_DATA_FOUND;
                    }
#endif
                    if (u32HVDIsrEvent & E_HVD_ISR_DEC_DISP_INFO_CHANGE)
                    {
                        MS_U8 u8Afd = MDrv_HVD_GetActiveFormat();

                        if (u8Afd != 0)
                        {
                            eEventFlag |= E_VDEC_EVENT_AFD_FOUND;
                        }

                        eEventFlag |= (E_VDEC_EVENT_DISP_INFO_CHG | E_VDEC_EVENT_DISP_INFO_RDY);
                    }
                    if (u32HVDIsrEvent & E_HVD_ISR_DEC_FIRST_FRM)
                    {
                        eEventFlag |= E_VDEC_EVENT_FIRST_FRAME;
                    }
#if defined(VDEC_ISR_MONITOR)
                    MsOS_SetEvent(_s32VdecEventId, eEventFlag);
#else
                    for (i = 0; i < E_VDEC_CB_MAX_NUM; i++)
                    {
                        MS_U32 u32Event = eEventFlag & _Attr.VdecEventInfo[i].u32EventBitMap;

                        if ((u32Event) &&
                            (_Attr.VdecEventInfo[i].pVdecEventCb))
                        {
                            _VDEC_MutexUnlock();
                            _Attr.VdecEventInfo[i].pVdecEventCb(u32Event, _Attr.VdecEventInfo[i].pParam);
                            _VDEC_MutexLock();
                        }
                    }
#endif
                }
                break;
            }
            case E_VDEC_DECODER_MJPEG:
                break;
            case E_VDEC_DECODER_RVD:
                break;
            default:
                break;
        }
    }

    _VDEC_MutexUnlock();
}

static MS_U8 _VDEC_Map2MVDSpeed(VDEC_DispSpeed eSpeed)
{
    MS_U8 u8Speed = 0;

    switch (eSpeed)
    {
        case E_VDEC_DISP_SPEED_32X:
            u8Speed = 32;
            break;

        case E_VDEC_DISP_SPEED_16X:
            u8Speed = 16;
            break;

        case E_VDEC_DISP_SPEED_8X:
            u8Speed = 8;
            break;

        case E_VDEC_DISP_SPEED_4X:
            u8Speed = 4;
            break;

        case E_VDEC_DISP_SPEED_2X:
            u8Speed = 2;
            break;

        case E_VDEC_DISP_SPEED_1X:
            u8Speed = 1;
            break;

        default:
            u8Speed = (MS_U8) eSpeed;
            break;
    }

    return u8Speed;
}

static MS_U8 _VDEC_Map2HVDSpeed(VDEC_DispSpeed eSpeed)
{
    return _VDEC_Map2MVDSpeed(eSpeed);
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display repeat last field
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_RepeatLastField(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_DispRepeatField(bEnable));
            break;
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetDispRepeatField(bEnable));
            ret = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set the maximum repeat times for one frame when av is not sync.
/// @param -u32Th \b IN : repeat times. 0x01 ~ 0xFF
///                0xff - repeat current frame until STC catch up PTS.
/// @return -The result of command set sync repeat threashold
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetAVSyncRepeatThreshold(MS_U32 u32Th)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            MDrv_MVD_SetAVSyncThreshold(u32Th);
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetSyncRepeatTH(u32Th));
            ret = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Enable FW only show one field.
/// @param bEnable \b IN : TRUE - show one field; FALSE - disable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_DispOneField(MS_U32 u32Arg)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_EnableDispOneField((MS_BOOL)u32Arg));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetDispOneField((MS_BOOL)u32Arg));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set fd mask delay count.
/// @param u32Arg \b IN : unit is in vsync base for muting the fd_mask
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetFdMaskDelayCnt(MS_U32 u32Arg)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetFdMaskDelayCount((MS_U16)u32Arg));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetFdMaskDelayCnt((MS_U8)u32Arg));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set frame rate convert output mode
/// @param pFRC \b IN : the address of VDEC_FRC_OutputParam
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_FrameRateConvert(VDEC_FRC_OutputParam *pFRC)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetOutputFRCMode((MS_U8)(pFRC->u32OutputFrameRate), pFRC->u8Interlace));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetOutputFRCMode((MS_U8)(pFRC->u32OutputFrameRate), pFRC->u8Interlace));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set ignore error reference
/// @param bIgnore \b IN : turn on / off.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetIgnoreErrRef(MS_BOOL bIgnore)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetIgnoreErrRef(bIgnore));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set ignore error reference
/// @param bIgnore \b IN : turn on / off.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_ForceFollowDTVSpec(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_ForceFollowDTVSpec(bEnable));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set fast display, not caring av-sync
/// @param bFastDisplay \b IN : turn on / off.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetFastDisplay(MS_BOOL bFastDisplay)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetFastDisplay(bFastDisplay));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set frame rate convert drop type
/// @param u32DropType \b IN : FRC drop type.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetFRCDropType(MS_U32 u32DropType)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetFRCDropType((MS_U8)u32DropType));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetFRCDropType((MS_U8)u32DropType));
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set disable resolution change
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetDisableSeqChange(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetDisableSeqChange(bEnable));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_HVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set the theshold of H264 frame gap, 0xFFFFFFFF don't care frame gap
/// @param u32FrmGap \b IN : frame gap
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetMinFrameGap(MS_U32 u32FrmGap)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetSettings_Pro(E_HVD_SSET_MIN_FRAME_GAP, u32FrmGap));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MVD:
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set disable Anti-Video Deadlock function
/// @param u32FrmGap \b IN : none
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetDisableAntiVdead(MS_U32 u32Arg)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetSettings_Pro(E_HVD_SSET_DISABLE_ANTI_VDEAD, u32Arg));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MVD:
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Reset MVD parser
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_ResetMvdParser(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_ParserRstDone(bEnable));
            break;
        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set Flush Frame buffer
/// @param bEnable \b IN : turn on / off.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_DoFlushFrmBuffer(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_PVRFlushDispQueue(bEnable));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetSettings_Pro(E_HVD_SSET_FLUSH_FRM_BUF,!bEnable));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Set DTV User Data packet mode
/// @param u32Mode \b IN : User Data Packet Mode, 0: DVB normal, 1: ATSC DirectTV. default:0.
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_SetDTVUserDataMode(MS_U32 u32Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetDTVUserDataMode((MS_U8)u32Mode));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Force into interlace mode
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_ForceInterlaceMode(MS_U32 u32Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_ForceInterlaceMode((MS_U8)u32Mode));
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Show Decode Order
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_ShowDecodeOrder(MS_U32 u32Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_ShowDecodeOrder((MS_U8)u32Mode));
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Force set FW to mute if resolution change
/// @param u32Mode \b IN : 0: Disable(default), 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_ForceAutoMute(MS_BOOL u32Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetAutoMute(u32Mode));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_SetVSizeAlign(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetVSizeAlign(bEnable));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EnableVSizeAlign(bEnable));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_Disp_Ignore_Crop(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_Disp_Ignore_Crop(bEnable));
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_SetDispFinishMode(MS_U8 u8Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetDispFinishMode(u8Mode));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_SetAVSyncMode(MS_U8 u8Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetAVSyncMode(u8Mode));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_SetCcInfoEnhanceMode(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = (MDrv_CC_InfoEnhanceMode(bEnable)) ? E_VDEC_OK : E_VDEC_FAIL ;
            break;

        case E_VDEC_DECODER_HVD:
            ret = (MDrv_HVD_CC_InfoEnhanceMode(bEnable)) ? E_VDEC_OK : E_VDEC_FAIL ;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_SetIdctMode(MS_U8 u8Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetIdctMode(u8Mode));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}


static HVD_ClockSpeed _VDEC_Map2HVDClockSpeed(VDEC_ClockSpeed ClockSpeed)
{
    HVD_ClockSpeed hvdClockSpeed = E_HVD_CLOCK_SPEED_NONE;

    switch(ClockSpeed)
    {
        case E_VDEC_CLOCK_SPEED_HIGHEST:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_HIGHEST;
            break;
        case E_VDEC_CLOCK_SPEED_HIGH:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_HIGH;
            break;
        case E_VDEC_CLOCK_SPEED_MEDIUM:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_MEDIUM;
            break;
        case E_VDEC_CLOCK_SPEED_LOW:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_LOW;
            break;
        case E_VDEC_CLOCK_SPEED_LOWEST:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_LOWEST;
            break;
        case E_VDEC_CLOCK_SPEED_DEFAULT:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_DEFAULT;
            break;
        default:
            hvdClockSpeed = E_HVD_CLOCK_SPEED_NONE;
            break;
    }

    return hvdClockSpeed;
}

static MVD_ClockSpeed _VDEC_Map2MVDClockSpeed(VDEC_ClockSpeed ClockSpeed)
{
    MVD_ClockSpeed mvdClockSpeed = E_MVD_CLOCK_SPEED_NONE;

    switch(ClockSpeed)
    {
        case E_VDEC_CLOCK_SPEED_HIGHEST:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_HIGHEST;
            break;
        case E_VDEC_CLOCK_SPEED_HIGH:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_HIGH;
            break;
        case E_VDEC_CLOCK_SPEED_MEDIUM:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_MEDIUM;
            break;
        case E_VDEC_CLOCK_SPEED_LOW:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_LOW;
            break;
        case E_VDEC_CLOCK_SPEED_LOWEST:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_LOWEST;
            break;
        case E_VDEC_CLOCK_SPEED_DEFAULT:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_DEFAULT;
            break;
        default:
            mvdClockSpeed = E_MVD_CLOCK_SPEED_NONE;
            break;
    }

    return mvdClockSpeed;
}


static VDEC_Result _VDEC_SetVPUClockSpeed(VDEC_ClockSpeed ClockSpeed)
{
    _HVD_RET_HANDLE(MDrv_HVD_SetVPUClockSpeed(_VDEC_Map2HVDClockSpeed(ClockSpeed)));

    return E_VDEC_OK;
}


static VDEC_Result _VDEC_SetHVDClockSpeed(VDEC_ClockSpeed ClockSpeed)
{
    _HVD_RET_HANDLE(MDrv_HVD_SetHVDClockSpeed(_VDEC_Map2HVDClockSpeed(ClockSpeed)));

    return E_VDEC_OK;
}


static VDEC_Result _VDEC_SetMVDClockSpeed(VDEC_ClockSpeed ClockSpeed)
{
    _MVD_RET_HANDLE(MDrv_MVD_SetMVDClockSpeed(_VDEC_Map2MVDClockSpeed(ClockSpeed)));

    return E_VDEC_OK;
}


static MS_U32 _VDEC_GetESBuffVacancy(void)
{
    MS_U32 u32ESWtPtr = MApi_VDEC_GetESWritePtr();
    MS_U32 u32ESRdPtr = MApi_VDEC_GetESReadPtr();

    if (u32VdecInitParamVersion == 0)
    {
        if (u32ESWtPtr == u32ESRdPtr)
        {
            return _vdecInitParam.SysConfig.u32BitstreamBufSize;
        }
        else if (u32ESWtPtr > u32ESRdPtr)
        {
            return _vdecInitParam.SysConfig.u32BitstreamBufSize - u32ESWtPtr + u32ESRdPtr;
        }
    }
    else if (u32VdecInitParamVersion == 1)
    {
        if (u32ESWtPtr == u32ESRdPtr)
        {
            return _vdecInitParam_ex1.SysConfig.u32BitstreamBufSize;
        }
        else if (u32ESWtPtr > u32ESRdPtr)
        {
            return _vdecInitParam_ex1.SysConfig.u32BitstreamBufSize - u32ESWtPtr + u32ESRdPtr;
        }
    }

    return u32ESRdPtr - u32ESWtPtr;
}

static VDEC_Result _VDEC_DropErrFrame(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = (MDrv_MVD_DropErrorFrame(bEnable)) ? E_VDEC_OK : E_VDEC_FAIL ;
            break;

        case E_VDEC_DECODER_HVD:
            ret = (MDrv_HVD_SetDispErrFrm(!bEnable)) ? E_VDEC_OK : E_VDEC_FAIL ;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Push Disp Q with Ref Number
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_PushDispQWithRefNum(MS_U32 u32Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if ((u32Mode != 0) && (u32Mode != 1))
        return ret;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_PushDispQWithRefNum((MS_BOOL)u32Mode));
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Push First
/// @param u32Mode \b IN : 0: Disable, 1: Enable
/// @return VDEC_Result
//------------------------------------------------------------------------------
static VDEC_Result _VDEC_ShowFirstFrameDirect(MS_U32 u32Mode)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if ((u32Mode != 0) && (u32Mode != 1))
        return ret;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
             _HVD_RET_HANDLE(MDrv_HVD_ShowFirstFrameDirect((MS_BOOL)u32Mode));
             ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MVD:
             _MVD_RET_HANDLE(MDrv_MVD_ShowFirstFrameDirect((MS_BOOL)u32Mode));
             ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_SetAutoExhaustESMode(MS_U32 u32ESBound)
{
    VDEC_Result ret = E_VDEC_FAIL;


    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_HVD:

            if(_Attr.eCodecType != E_VDEC_CODEC_TYPE_H264)
            {
                return E_VDEC_RET_UNSUPPORTED;
            }
            _HVD_RET_HANDLE(MDrv_HVD_AutoExhaustESMode(u32ESBound));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}

static VDEC_Result _VDEC_AVCSupportRefNumOverMaxDBPSize(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
            
        case E_VDEC_DECODER_HVD:
            if(_Attr.eCodecType != E_VDEC_CODEC_TYPE_H264)
            {
                return E_VDEC_RET_UNSUPPORTED;
            }
            _HVD_RET_HANDLE(MDrv_HVD_AVCSupportRefNumOverMaxDBPSize( bEnable));
            ret = E_VDEC_OK;
            break;
            
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
            
        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            break;
    }

    return ret;
}


static VDEC_Result _VDEC_SetMinTspDataSize(MS_U32 u32Size)
{
    VDEC_Result ret = E_VDEC_FAIL;


    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_HVD:

            if(_Attr.eCodecType != E_VDEC_CODEC_TYPE_H264)
            {
                return E_VDEC_RET_UNSUPPORTED;
            }
            _HVD_RET_HANDLE(MDrv_HVD_SetMinTspDataSize(u32Size));
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            break;
    }

    return ret;
}


static VDEC_Result _MApi_VDEC_GetFlushPatternEntryNum(MS_U32* param)
{
	VDEC_Result ret = E_VDEC_FAIL;

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
		{
            *param = (MS_U32)1;
            ret = E_VDEC_OK;
            break;
		}
        case E_VDEC_DECODER_HVD:
        {
            *param = (MS_U32)2;
            ret = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}



//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Get VDEC version
/// @return -the pointer to the VDEC version
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetLibVer(const MSIF_Version **ppVersion)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;


    IO_arg.param[0] = (void *)ppVersion;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETLIBVER, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}
VDEC_Result MApi_VDEC_V2_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_VDEC_FAIL;
    }

    *ppVersion = &_api_vdec_version;

    return E_VDEC_OK;
}

//-----------------------------------------------------------------------------
/// Get VDEC info
/// @brief \b Function \b Description:  Get information of VDEC API
/// @return - the pointer to the VDEC information
//-----------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetInfo(const VDEC_Info **ppInfo)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;


    IO_arg.param[0] = (void *)ppInfo;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETINFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}
VDEC_Result MApi_VDEC_V2_GetInfo(const VDEC_Info **ppInfo)
{
    if (!ppInfo)
    {
        return E_VDEC_FAIL;
    }

    *ppInfo = &_api_vdec_info;

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Get decoder status
/// @param pStatus \b OUT : VDEC_Status
/// @return TRUE or FALSE
///     - TRUE: Useful status got
///     - FALSE: No status because of not init yet
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_GetStatus(VDEC_Status *pStatus)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;


    IO_arg.param[0] = (void *)pStatus;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETSTATUS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}
MS_BOOL MApi_VDEC_V2_GetStatus(VDEC_Status *pStatus)
{
    pStatus->bInit = _Attr.bInit;

    if (!_Attr.bInit)
    {
        pStatus->bIdle = TRUE;
        pStatus->eStage = E_VDEC_STAGE_STOP;
        return FALSE;
    }
    else
    {
        pStatus->bIdle = _VDEC_IsIdle();
        // FIXME
        //pStatus->eStage =
        return TRUE;
    }
}

//------------------------------------------------------------------------------
/// Check if codec type is supported or not.
/// @param eCodecType \b IN : codec type
/// @return TRUE or FALSE
///     - TRUE: Success
///     - FALSE: Failed
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_CheckCaps(VDEC_CodecType eCodecType)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;


    IO_arg.param[0] = (void *)&eCodecType;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CHECKCAPS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

MS_BOOL MApi_VDEC_V2_CheckCaps(VDEC_CodecType eCodecType)
{
    VDEC_Decoder eDecoder;

    eDecoder = _VDEC_GetDecoderByCodecType(eCodecType);

    switch (eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        _BOOL_VDEC_MVD_LINT_CHECK();
        {
            MVD_Caps caps;

            _VDEC_Memset(&caps, 0, sizeof(MVD_Caps));

            if (MDrv_MVD_GetCaps(&caps))
            {
                if (E_VDEC_CODEC_TYPE_MPEG2 == eCodecType)
                {
                    return caps.bMPEG2;
                }
                else if (E_VDEC_CODEC_TYPE_MPEG4 == eCodecType)
                {
                    return caps.bMPEG4;
                }
                else if (E_VDEC_CODEC_TYPE_VC1_ADV == eCodecType || E_VDEC_CODEC_TYPE_VC1_MAIN == eCodecType)
                {
                    return caps.bVC1;
                }
            }
            else
            {
                return FALSE;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        _BOOL_VDEC_HVD_LINT_CHECK();
        {
            if (E_VDEC_CODEC_TYPE_H264 == eCodecType)
            {
                return MDrv_HVD_GetCaps(E_HVD_AVC);
            }
            else if (E_VDEC_CODEC_TYPE_AVS == eCodecType)
            {
                return MDrv_HVD_GetCaps(E_HVD_AVS);
            }
            else if (E_VDEC_CODEC_TYPE_RV8 == eCodecType || E_VDEC_CODEC_TYPE_RV9 == eCodecType)
            {
                return MDrv_HVD_GetCaps(E_HVD_RM);
            }
            else
            {
                return FALSE;
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
            // unnecessary
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_VDEC_RVD_LINT_CHECK();
            return MDrv_RVD_CheckCaps();

        default:
            break;
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/// Enable turbo mode for VDEC.
/// @param bTurbo \b IN : TRUE for enable or FALSE for disable
/// @return VDEC_Result
///     - E_VDEC_OK: success
///     - E_VDEC_FAIL: failed
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_EnableTurboMode(MS_BOOL bEnable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;


    IO_arg.param[0] = (void *)&bEnable;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_ENABLETUEBOMODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_EnableTurboMode(MS_BOOL bEnable)
{
    bTurboMode = bEnable;
    return E_VDEC_OK;
}

#ifdef _MVD_EVENT_DBG
MS_U32 u32Para = 111;
extern MS_U8 HAL_MVD_GetAVSyncStatus(void);
void VDEC_TestVSync(MS_U32 eFlag, void *param)
{
    if ((E_VDEC_EVENT_DISP_ONE & eFlag) == E_VDEC_EVENT_DISP_ONE)
    {
        printf("SyncStat=0x%x, diff=%ld\n", HAL_MVD_GetAVSyncStatus(), MDrv_MVD_GetPtsStcDiff());
        MDrv_MVD_GetIsFreerun();
    }
}
void VDEC_TestCC(MS_U32 eFlag, void *param)
{
    static MS_U16 u16TempRef = 0xffff;
    //printf("testCb eFlag=0x%lx, param=%ld\n", eFlag, *(MS_U32*)param);

    if ((E_VDEC_EVENT_USER_DATA_FOUND & eFlag) == E_VDEC_EVENT_USER_DATA_FOUND)
    {
        VDEC_CC_Info ccInfo;
        ccInfo.u32Version = 0;
        while (MApi_VDEC_IsCCAvailable())
        {
            if (E_VDEC_OK == MApi_VDEC_GetCCInfo((void*)&ccInfo, sizeof(VDEC_CC_Info)))
            {
                //if ((u16TempRef+1) != ccInfo.u16TempRef)
                {
                    printf("ccInfo: ");
                    printf("%d, ", ccInfo.u16TempRef);
                    printf("%d, ", ccInfo.u8PicStructure);
                    printf("%d, ", ccInfo.u8TopFieldFirst);
                    printf("0x%lx, ", ccInfo.u32UserDataBuf);
                    printf("%ld, ", ccInfo.u32UserDataSize);
                    printf("%ld\n", ccInfo.u32Pts);
                }
                u16TempRef = ccInfo.u16TempRef;
            }
        }
    }

    return;
}

void _VDEC_MVDEventDbg(void)
{
#if 1
    //Get info at every VSync:
    MApi_VDEC_SetEvent(E_VDEC_EVENT_DISP_ONE, VDEC_TestVSync, (void *)&u32Para);
#else
    //Test DisplayOrder user data
    MApi_VDEC_SetEvent(E_VDEC_EVENT_USER_DATA_FOUND, VDEC_TestCC, (void *)&u32Para);
#endif
}
#endif

VDEC_Result _MApi_VDEC_IsChroma420(MS_U32* param)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            if (MDrv_MVD_GetChromaFormat() <= 1)
            {
                *param = TRUE;
            }
            else
            {
                *param = FALSE;
            }
                ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            *param = TRUE;
            ret = E_VDEC_OK;
            break;

        default:
            *param = FALSE;
            break;
    }

    return ret;
}

VDEC_Result _MApi_VDEC_GetRealFrameRate(MS_U32* param)
{
    VDEC_Result ret = E_VDEC_FAIL;
    VDEC_DispInfo Dispinfo;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    if ( MApi_VDEC_CheckDispInfoRdy() != E_VDEC_OK )
    {  // DispInfo isn't ready
        return E_VDEC_RET_NOT_READY;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
            *param = MDrv_HVD_GetData(E_HVD_GDATA_TYPE_REAL_FRAMERATE);
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            if ( MApi_VDEC_GetDispInfo(&Dispinfo) == E_VDEC_OK )
            {
                *param = Dispinfo.u32FrameRate;
                ret = E_VDEC_OK;
            }
            break;
    }

    return ret;
}

VDEC_Result _MApi_VDEC_GetColorMatrixCoef(MS_U32* param)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            *param = MDrv_MVD_GetMatrixCoef();
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            *param = 0xff;
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

VDEC_Result _MApi_VDEC_GetIsDynScalingEnabled(MS_U32* param)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!param)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }
    else
    {
        *param = FALSE;
    }

    if (!_Attr.bInit)
    {
        VDEC_INFO(printf("%s NOT_INIT\n", __FUNCTION__));
        return E_VDEC_RET_NOT_INIT;
    }

    if ( MApi_VDEC_CheckDispInfoRdy() != E_VDEC_OK )
    {  // DispInfo isn't ready
        VDEC_INFO(printf("%s NOT_RDY\n", __FUNCTION__));
        return E_VDEC_RET_NOT_READY;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
            *param = (MS_BOOL) MDrv_HVD_GetDynamicScalingInfo(E_HVD_DS_IS_ENABLED);
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MVD:
            *param = MDrv_MVD_GetIsDynScalingEnabled();
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    VDEC_INFO(printf("%s = %x\n", __FUNCTION__, (MS_BOOL)*param));
    return ret;
}

VDEC_Result _MApi_VDEC_GetIsOriInterlaceMode(MS_U32* param)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(MVD_FrameInfo));
            MDrv_MVD_GetFrameInfo(&info);
            *param = (MS_U32)info.u8Interlace;
            ret = E_VDEC_OK;

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if(MDrv_HVD_CheckDispInfoRdy() == E_HVD_OK)
            {
                *param = MDrv_HVD_GetData(E_HVD_GDATA_TYPE_IS_ORI_INTERLACE_MODE);
                ret = E_VDEC_OK;
            }
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        {
            *param = 0;
            ret = E_VDEC_OK;
            break;
        }
        default:
            *param = 0;
            break;
    }

    return ret;
}

static MS_U8 _VDEC_Map2MVDCCFmt(VDEC_CCFormat eFmt)
{
    MS_U8 u8Fmt = 0xff;
    u8Fmt = (E_VDEC_CC_608==eFmt)?E_CC_MVD_TYPE_608:E_CC_MVD_TYPE_708;
    return u8Fmt;
}

static MS_U8 _VDEC_Map2MVDCCOpt(VDEC_CCType eType)
{
    MS_U8 u8Operation = 0;
    switch (eType)
    {
        case E_VDEC_CC_TYPE_NTSC_FIELD1:
            u8Operation = 0x01;
            break;
        case E_VDEC_CC_TYPE_NTSC_FIELD2:
            u8Operation = 0x02;
            break;
        case E_VDEC_CC_TYPE_NTSC_TWOFIELD:
            u8Operation = 0x03;
            break;
        case E_VDEC_CC_TYPE_DTVCC:
        default:
            u8Operation = 0x04;
            break;
    }
    return u8Operation;
}


static MS_U8 _VDEC_Map2HVDCCFmt(VDEC_CCFormat eFmt)
{
    MS_U8 u8Fmt = 0xff;
    u8Fmt = (E_VDEC_CC_608==eFmt)?1:0;
    return u8Fmt;
}

static MS_U8 _VDEC_Map2HVDCCOpt(VDEC_CCType eType)
{
    MS_U8 u8Operation = 0;
    switch (eType)
    {
        case E_VDEC_CC_TYPE_NTSC_FIELD1:
            u8Operation = 0x01;
            break;
        case E_VDEC_CC_TYPE_NTSC_FIELD2:
            u8Operation = 0x02;
            break;
        case E_VDEC_CC_TYPE_NTSC_TWOFIELD:
            u8Operation = 0x03;
            break;
        case E_VDEC_CC_TYPE_DTVCC:
        default:
            u8Operation = 0x04;
            break;
    }
    return u8Operation;
}


static VDEC_Result _MApi_VDEC_GetFrmPackingArrSEI(void *param)
{
    VDEC_Result ret = E_VDEC_OK;
    VDEC_Frame_packing_SEI *pFPASEIInfo = NULL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    if( param == NULL)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }
    else
    {
        pFPASEIInfo = (VDEC_Frame_packing_SEI *)param;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_GetFrmPackingArrSEI(&_stFrmPacking));

            pFPASEIInfo->u8Frm_packing_arr_cnl_flag = _stFrmPacking.u8Frm_packing_arr_cnl_flag;
            pFPASEIInfo->u8Frm_packing_arr_type = _stFrmPacking.u8Frm_packing_arr_type;
            pFPASEIInfo->u8content_interpretation_type = _stFrmPacking.u8content_interpretation_type;
            pFPASEIInfo->u1Quincunx_sampling_flag = _stFrmPacking.u1Quincunx_sampling_flag;

            pFPASEIInfo->u1Spatial_flipping_flag = _stFrmPacking.u1Spatial_flipping_flag;
            pFPASEIInfo->u1Frame0_flipping_flag = _stFrmPacking.u1Frame0_flipping_flag;
            pFPASEIInfo->u1Field_views_flag = _stFrmPacking.u1Field_views_flag;
            pFPASEIInfo->u1Current_frame_is_frame0_flag = _stFrmPacking.u1Current_frame_is_frame0_flag;

            pFPASEIInfo->u1Frame0_self_contained_flag = _stFrmPacking.u1Frame0_self_contained_flag;
            pFPASEIInfo->u1Frame1_self_contained_flag = _stFrmPacking.u1Frame1_self_contained_flag;
            pFPASEIInfo->u4Frame0_grid_position_x = _stFrmPacking.u4Frame0_grid_position_x;
            pFPASEIInfo->u4Frame0_grid_position_y = _stFrmPacking.u4Frame0_grid_position_y;

            pFPASEIInfo->u4Frame1_grid_position_x = _stFrmPacking.u4Frame1_grid_position_x;
            pFPASEIInfo->u4Frame1_grid_position_y = _stFrmPacking.u4Frame1_grid_position_y;

            break;
        case E_VDEC_DECODER_MJPEG:
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}

VDEC_Result _MApi_VDEC_GetFBUsageMem(MS_U32* param)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    if ( MApi_VDEC_CheckDispInfoRdy() != E_VDEC_OK )
    {  // DispInfo isn't ready
        return E_VDEC_RET_NOT_READY;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_HVD:
            *param = MDrv_HVD_GetData(E_HVD_GDATA_TYPE_FB_USAGE_MEM);
            ret = E_VDEC_OK;
            break;
        case E_VDEC_DECODER_MVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            *param = 0xffff;
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Initial VDEC.
/// @param pInitParam \b IN : pointer to initial parameter
/// @return VDEC_Result
///     - E_VDEC_OK: success
///     - E_VDEC_FAIL: failed
///     - E_VDEC_RET_NOT_EXIT: not ext after last initialization
///     - E_VDEC_RET_INVALID_PARAM: input parameter is invalid
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Init(VDEC_InitParam *pInitParam)
{
    if (pu32VDEC_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC, &pu32VDEC_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Utopia Open VDEC failed\n");
            return E_VDEC_FAIL;
        }
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)pInitParam;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_INIT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Initial VDEC.
/// @param pInitParam \b IN : pointer to initial parameter
/// @return VDEC_Result
///     - E_VDEC_OK: success
///     - E_VDEC_FAIL: failed
///     - E_VDEC_RET_NOT_EXIT: not ext after last initialization
///     - E_VDEC_RET_INVALID_PARAM: input parameter is invalid
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Init(VDEC_InitParam *pInitParam)
{
    MS_BOOL bNotReloadFW = FALSE;

    if (_Attr.bInit== TRUE)
    {
        printf("VDEC Warn: re-init VDEC Driver\n");
#if defined( MS_DEBUG )
        return E_VDEC_RET_NOT_EXIT;
#endif
    }

    u32VdecInitParamVersion = 0;

    _VDEC_InitLocalVar();

    _VDEC_Memcpy(&_vdecInitParam, pInitParam, sizeof(VDEC_InitParam));

    _Attr.eCodecType    = pInitParam->eCodecType;
    _Attr.eDecoder      = _VDEC_GetDecoderByCodecType(pInitParam->eCodecType);
    _Attr.eSrcMode      = pInitParam->VideoInfo.eSrcMode;
    _Attr.eDbgMsgLevel  = pInitParam->SysConfig.eDbgMsgLevel;

#if (defined(CHIP_A5)) || defined(CHIP_A5P)
#define _CHIP_U02 0x01
    if (pInitParam->EnableDynaScale)
    {
        MS_U8 u8ChipRev = MDrv_SYS_GetChipRev();
        if (u8ChipRev < _CHIP_U02)
        {
            printf("VDEC Warn: DS(%x) unsupported! Force EnableDynaScale as FALSE.\n", u8ChipRev);
            pInitParam->EnableDynaScale = FALSE;
        }
    }
#endif

    // init debug message
    MApi_VDEC_SetDbgLevel(_Attr.eDbgMsgLevel);

#if VDEC_ENABLE_LINK_WEAK_SYMBOL
    // it is critical very any nos compile envi
    _VDEC_LinkWeakSymbolPatch();
#endif

    if (bTurboMode)
    {
        bNotReloadFW = TRUE;
    }
    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        _RET_VDEC_MVD_LINT_CHECK();
        {
            if (_Pre_Ctrl.bHWBufferReMapping)
            {
                //////MDrv_MVD_HWBuffer_ReMappingMode(_Pre_Ctrl.bHWBufferReMapping);
            }
            
            VDEC_SysCfg cfg = pInitParam->SysConfig;
            MVD_FWCfg fwCfg;
            MVD_MEMCfg memCfg;
            MVD_InfoCfg infoCfg;
            MVD_CodecType eMvdCodecType = E_MVD_CODEC_UNKNOWN;
            MVD_SrcMode   eMvdSrcMode;
            MVD_TIMESTAMP_TYPE eSyncType = E_MVD_TIMESTAMP_FREERUN;

            // check codec type
            eMvdCodecType = _VDEC_Map2MVDCodecType(_Attr.eCodecType);
            if (eMvdCodecType == E_MVD_CODEC_UNKNOWN)
            {
                //printf("%s(%d): Unknown eMvdCodecType!\n", __FUNCTION__, __LINE__);
                return E_VDEC_FAIL;
            }

            // check source type
            eMvdSrcMode = _VDEC_Map2MVDSrcMode(_Attr.eSrcMode);
            if ((eMvdSrcMode == 0xff) || (eMvdSrcMode >= MVD_SRCMODE_MAX))
            {
                //printf("%s(%d): Invalid eMvdSrcMode!\n", __FUNCTION__, __LINE__);
                return E_VDEC_FAIL;
            }

            _VDEC_Memset(&fwCfg, 0, sizeof(MVD_FWCfg));
            _VDEC_Memset(&memCfg, 0, sizeof(MVD_MEMCfg));

            fwCfg.eCodecType = eMvdCodecType;
            fwCfg.bNotReload = bNotReloadFW; //TRUE for not load fw more than once
            memCfg.eFWSrcType = (MVD_FWSrcType)cfg.eFWSourceType;
            memCfg.u32FWBinAddr = cfg.u32FWBinaryAddr;
            memCfg.u32FWBinSize = cfg.u32FWBinarySize;
            memCfg.u32FWCodeAddr = cfg.u32CodeBufAddr;
            memCfg.u32FWCodeSize = cfg.u32CodeBufSize;
            memCfg.u32FBAddr = cfg.u32FrameBufAddr;
            memCfg.u32FBSize = cfg.u32FrameBufSize;
            memCfg.u32BSAddr = cfg.u32BitstreamBufAddr;
            memCfg.u32BSSize = cfg.u32BitstreamBufSize;
            memCfg.u32DrvBufAddr = cfg.u32DrvProcBufAddr;
            memCfg.u32DrvBufSize = cfg.u32DrvProcBufSize;
            memCfg.bEnableDynScale = pInitParam->EnableDynaScale;
            #ifdef _VDEC_AUTH_IPCheck_TEST_CODE_
            memCfg.bSupportSDModeOnly = !bModeSelectHD;
            #endif

            infoCfg.u8CodecType = eMvdCodecType;
            infoCfg.u8BSProviderMode = eMvdSrcMode;
            infoCfg.bDisablePESParsing = bMvdParserDisable[eMvdSrcMode];

            _MVD_RET_HANDLE(MDrv_MVD_SetCfg(&fwCfg, &memCfg, &infoCfg));

            //the very first step: Get register base addr
            //notice: must do this first before accessing any register.
            MS_U32 u32NonPMBankSize;
            MS_U32 u32NonPMBankAddr = 0;
            if (!MDrv_MMIO_GetBASE(&u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_MVD))
            {
                printf("_MVD_Init: IOMap failure\n");
                return E_VDEC_FAIL;
            }
            else
            {
                MDrv_MVD_RegSetBase(u32NonPMBankAddr);
                //printf("_MVD_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr);
            }

            if (!MDrv_MVD_Init())
            {
                return E_VDEC_FAIL;
            }

            if (eMvdSrcMode == E_MVD_TS_FILE_MODE)
            {
                MS_U32 u32bsBufUnderflowTH = ((((cfg.u32BitstreamBufSize*15)/16)>>3)<<3);
                MS_U32 u32bsBufOverflowTH  = (((cfg.u32BitstreamBufSize-0x4000)>>3)<<3); //16K
                MDrv_MVD_SetUnderflowTH(u32bsBufUnderflowTH);
                MDrv_MVD_SetOverflowTH(u32bsBufOverflowTH);
            }

            MDrv_MVD_SetCodecInfo(eMvdCodecType, eMvdSrcMode, bMvdParserDisable[eMvdSrcMode]);

            if (FALSE == MDrv_MVD_DropErrorFrame(!pInitParam->bDisableDropErrFrame))
            {
                return E_VDEC_FAIL;
            }
            bDropErrFrm = (!pInitParam->bDisableDropErrFrame);

            if (FALSE == MDrv_MVD_DispRepeatField(pInitParam->bRepeatLastField))
            {
                return E_VDEC_FAIL;
            }

            MDrv_MVD_DisableErrConceal(!pInitParam->bDisableErrConceal);
            /*
            E_MVD_Result ret = MDrv_MVD_DisableErrConceal(pInitParam->bDisableErrConceal);


            if (E_MVD_RET_OK != ret)
            {
                return E_VDEC_FAIL;
            }
            */

            MDrv_MVD_SetVirtualBox(pInitParam->u32DSVirtualBoxWidth, pInitParam->u32DSVirtualBoxHeight);

            if (eMvdSrcMode == E_MVD_SLQ_TBL_MODE)
            {
                if (E_VDEC_TIME_STAMP_DTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_DTS;
                }
                else if (E_VDEC_TIME_STAMP_PTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_PTS;
                }
                else if (E_VDEC_TIME_STAMP_NONE == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_FREERUN;
                }
                else if (E_VDEC_TIME_STAMP_STS == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_STS;
                }
                else if (E_VDEC_TIME_STAMP_DTS_MPEG_DIRECTV_SD == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_DTS_RVU;
                }
                else if (E_VDEC_TIME_STAMP_PTS_MPEG_DIRECTV_SD == pInitParam->VideoInfo.eTimeStampType)
                {
                    eSyncType = E_MVD_TIMESTAMP_PTS_RVU;
                }

                else
                {
                    printf("Invalid sync type %d for MVD\n", pInitParam->VideoInfo.eTimeStampType);
                    return E_VDEC_FAIL;
                }
                if ((eSyncType != E_MVD_TIMESTAMP_STS) && (FALSE == MDrv_MVD_SetFileModeAVSync(eSyncType)))
                {
                    return E_VDEC_FAIL;
                }
            }

            if ((eMvdSrcMode == E_MVD_TS_FILE_MODE)||(eMvdSrcMode == E_MVD_SLQ_TBL_MODE))
            {  //set up MVD for MediaCodec playback
                _VDEC_MVDCfgDivX(_Attr.eCodecType);

                if (eMvdCodecType == E_MVD_CODEC_DIVX311)
                {
                    MVD_FrameInfo stFrminfo;
                    _VDEC_Memset(&stFrminfo, 0, sizeof(MVD_FrameInfo));
                    stFrminfo.u16HorSize = pInitParam->VideoInfo.u16Width[0];
                    stFrminfo.u16VerSize = pInitParam->VideoInfo.u16Height[0];
#ifdef MSOS_TYPE_LINUX_KERNEL
                    MS_U64 u64Tmp = (MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000;
                    do_div(u64Tmp, pInitParam->VideoInfo.u32FrameRateBase);
                    stFrminfo.u32FrameRate = (MS_U32)u64Tmp;
#else
                    stFrminfo.u32FrameRate =
                        (MS_U32)(((MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000) / pInitParam->VideoInfo.u32FrameRateBase);
#endif
                    #if 0
                    printf("==>DivX311 w=0x%x h=0x%x fr=0x%lx,0x%lx\n",
                        pInitParam->VideoInfo.u16Width[0],pInitParam->VideoInfo.u16Height[0],
                        pInitParam->VideoInfo.u32FrameRate, pInitParam->VideoInfo.u32FrameRateBase);
                    printf("==>DivX311 w=0x%x h=0x%x fr=%ld\n",
                        stFrminfo.u16HorSize, stFrminfo.u16VerSize, stFrminfo.u32FrameRate);
                    #endif
                    MDrv_MVD_SetFrameInfo(&stFrminfo);
                }
                else if (eMvdCodecType != E_MVD_CODEC_MPEG2)
                {
                    MVD_FrameInfo stFrminfo1;
                    _VDEC_Memset(&stFrminfo1, 0, sizeof(MVD_FrameInfo));
                    if(pInitParam->VideoInfo.u32FrameRateBase == 0)
                    {
                        printf("Invalid FrameRate base\n");
                        pInitParam->VideoInfo.u32FrameRateBase = 1;
                    }
#ifdef MSOS_TYPE_LINUX_KERNEL
                    MS_U64 u64Tmp = (MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000;
                    do_div(u64Tmp, pInitParam->VideoInfo.u32FrameRateBase);
                    stFrminfo1.u32FrameRate = (MS_U32)u64Tmp;
#else
                    stFrminfo1.u32FrameRate =
                        (MS_U32)(((MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000) / pInitParam->VideoInfo.u32FrameRateBase);
#endif
                    MDrv_MVD_SetFrameInfo(&stFrminfo1);
                }

                if (MDrv_MVD_SkipToIFrame() != TRUE)
                {
                    return E_VDEC_FAIL;
                }
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        _RET_VDEC_HVD_LINT_CHECK();
        {
            if (_Pre_Ctrl.bOnePendingBuffer)
            {
                MDrv_HVD_OnePendingBufferMode(_Pre_Ctrl.bOnePendingBuffer);
            }

            if (_Pre_Ctrl.bFrameRateHandling)
            {
                MDrv_HVD_FrameRateHandling(_Pre_Ctrl.bFrameRateHandling, _Pre_Ctrl.u32PreSetFrameRate);
            }
            
            VDEC_SysCfg cfg = pInitParam->SysConfig;
            HVD_MemCfg stMemCfg;
            HVD_Init_Settings stInitSettings;
            RV_Info stRVInfo;

            _VDEC_Memset(&stMemCfg, 0, sizeof(HVD_MemCfg));
            _VDEC_Memset(&stInitSettings, 0, sizeof(HVD_Init_Settings));
            _VDEC_Memset(&stRVInfo, 0, sizeof(RV_Info));

            stInitSettings.pRVFileInfo = &stRVInfo;

            stMemCfg.eFWSourceType = (HVD_FWSourceType) cfg.eFWSourceType;
            stMemCfg.u32FWBinaryAddr = (MS_PHYADDR) cfg.u32FWBinaryAddr;

            if (stMemCfg.eFWSourceType == E_HVD_FW_SOURCE_DRAM)
            {
                stMemCfg.u32FWBinaryVAddr = MS_PA2KSEG1((MS_U32) cfg.u32FWBinaryAddr);
            }

            stMemCfg.u32FWBinarySize = cfg.u32FWBinarySize;

            stMemCfg.u32VLCBinaryAddr = (MS_PHYADDR) cfg.u32VlcBinarySrcAddr;

            if (stMemCfg.eFWSourceType == E_HVD_FW_SOURCE_DRAM)
            {
                stMemCfg.u32VLCBinaryVAddr = MS_PA2KSEG1((MS_U32) cfg.u32VlcBinarySrcAddr);
            }

            stMemCfg.u32VLCBinarySize = cfg.u32VlcTabBinarySize;

            stMemCfg.u32CodeBufAddr  = (MS_PHYADDR) cfg.u32CodeBufAddr;
            stMemCfg.u32CodeBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32CodeBufAddr);
            stMemCfg.u32CodeBufSize  = cfg.u32CodeBufSize;
            stMemCfg.u32FrameBufAddr = (MS_PHYADDR) cfg.u32FrameBufAddr;
            stMemCfg.u32FrameBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32FrameBufAddr);
            stMemCfg.u32FrameBufSize = cfg.u32FrameBufSize;
            stMemCfg.u32BitstreamBufAddr = (MS_PHYADDR) cfg.u32BitstreamBufAddr;
            stMemCfg.u32BitstreamBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32BitstreamBufAddr);
            stMemCfg.u32BitstreamBufSize = cfg.u32BitstreamBufSize;

            stMemCfg.u32DrvProcessBufAddr = (MS_PHYADDR) cfg.u32DrvProcBufAddr;
            stMemCfg.u32DrvProcessBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32DrvProcBufAddr);
            stMemCfg.u32DrvProcessBufSize = cfg.u32DrvProcBufSize;

            // set codec flag
            if (E_VDEC_CODEC_TYPE_H264 == _Attr.eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_AVC;
            }
            else if (E_VDEC_CODEC_TYPE_AVS == _Attr.eCodecType)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_AVS;
            }
            else if ((E_VDEC_CODEC_TYPE_RV8 == _Attr.eCodecType)
                  || (E_VDEC_CODEC_TYPE_RV9 == _Attr.eCodecType))
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_HW_RM;
            }

            // set input source mode
            if (E_VDEC_SRC_MODE_DTV == _Attr.eSrcMode)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_LIVE_STREAM;
                stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_TSP;
                stInitSettings.u8SyncType = E_HVD_SYNC_ATS;
            }
            else if (E_VDEC_SRC_MODE_TS_FILE == _Attr.eSrcMode)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_FILE_TS;
                stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_TSP;
                stInitSettings.u8SyncType = E_HVD_SYNC_ATS;
            }
            else if (E_VDEC_SRC_MODE_FILE == _Attr.eSrcMode)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_FILE_RAW;
                stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_DRV;

                if (E_VDEC_TIME_STAMP_STS == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_SYNC_STS;
                }
                else if (E_VDEC_TIME_STAMP_NONE == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_SYNC_ATS;
                }
                else if (E_VDEC_TIME_STAMP_DTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_SYNC_DTS;
                }
                else if (E_VDEC_TIME_STAMP_PTS == pInitParam->VideoInfo.eTimeStampType)
                {
                    stInitSettings.u8SyncType = E_HVD_SYNC_PTS;
                }
                else
                {
                    printf("Invalid sync type %d\n", pInitParam->VideoInfo.eTimeStampType);

                    return E_VDEC_FAIL;
                }
            }

            // set start code exist
            if (pInitParam->VideoInfo.bWithoutNalStCode)
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_START_CODE_REMOVED;
            }
            else
            {
                stInitSettings.u32ModeFlag |= HVD_INIT_START_CODE_REMAINED;
            }

            // pro settings
            stInitSettings.u32FrameRate = pInitParam->VideoInfo.u32FrameRate;
            stInitSettings.u32FrameRateBase = pInitParam->VideoInfo.u32FrameRateBase;

            if (E_VDEC_SRC_MODE_FILE == _Attr.eSrcMode)
            {
                stInitSettings.u8MinFrmGap = 2; // default set
            }
            else
            {
                stInitSettings.u8MinFrmGap = 0; // default set
            }

            stInitSettings.bAutoPowerSaving = TRUE;
            stInitSettings.bFastDisplay = TRUE;

            stInitSettings.u8TimeUnit = 1;    // ms
            stInitSettings.bDynamicScaling = pInitParam->EnableDynaScale;

            if (bNotReloadFW)
            {
                stInitSettings.u8TurboInit |= E_HVD_TURBOINIT_FW_RELOAD; //set for not load fw more than once
            }

            // for RM
            if (E_VDEC_CODEC_TYPE_RV8 == pInitParam->eCodecType)
            {
                stRVInfo.RV_Version = 0;
            }
            else if (E_VDEC_CODEC_TYPE_RV9 == pInitParam->eCodecType)
            {
                stRVInfo.RV_Version = 1;
            }

            stRVInfo.ulNumSizes = pInitParam->VideoInfo.u16NumSizes;
            _VDEC_Memcpy(stRVInfo.ulPicSizes_w, pInitParam->VideoInfo.u16Width, sizeof(pInitParam->VideoInfo.u16Width));
            _VDEC_Memcpy(stRVInfo.ulPicSizes_h, pInitParam->VideoInfo.u16Height, sizeof(pInitParam->VideoInfo.u16Height));

            {
                MS_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0;
                if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
                {
                    printf("VDEC HVD Init Err: MMIO_GetBASE failure\n");
                    return E_VDEC_FAIL;
                }
                else
                {
                    //printf("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
                    MDrv_HVD_SetOSRegBase(u32RiuBaseAdd);
                }
            }

            _HVD_RET_HANDLE(MDrv_HVD_Init(&stMemCfg, &stInitSettings));

            // SPS filter
            if (E_VDEC_SRC_MODE_DTV == _Attr.eSrcMode)
            {
                HVD_Disp_Info_Threshold stDispInfoTH;

                stDispInfoTH.u32FrmrateLowBound = 0;
                stDispInfoTH.u32FrmrateUpBound  = 0;
                //stDispInfoTH.u32FrmrateLowBound = 10000;
                //stDispInfoTH.u32FrmrateUpBound  = 170000;
                stDispInfoTH.u32MvopLowBound    = 16000000;
                stDispInfoTH.u32MvopUpBound     = 0;

                MDrv_HVD_SetDispInfoTH(&stDispInfoTH);
            }

            if (pInitParam->EnableDynaScale)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetVirtualBox(pInitParam->u32DSVirtualBoxWidth,
                                pInitParam->u32DSVirtualBoxHeight));
            }

            _HVD_RET_HANDLE(MDrv_HVD_SetDispErrFrm(pInitParam->bDisableDropErrFrame));

            _HVD_RET_HANDLE(MDrv_HVD_SetErrConceal(!pInitParam->bDisableErrConceal));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            VDEC_SysCfg cfg = pInitParam->SysConfig;
            MJPEG_INIT_PARAM initParam;

            _VDEC_Memset(&initParam, 0, sizeof(MJPEG_INIT_PARAM));

            initParam.u32FwBinAddr = cfg.u32FWBinaryAddr;
            initParam.u32FwBinSize = cfg.u32FWBinarySize;
            initParam.u32FwAddr = cfg.u32CodeBufAddr;
            initParam.u32FwSize = cfg.u32CodeBufSize;
            initParam.u32MRCBufAddr = cfg.u32BitstreamBufAddr;
            initParam.u32MRCBufSize = cfg.u32BitstreamBufSize;
            initParam.u32MWCBufAddr = cfg.u32FrameBufAddr;
            initParam.u32MWCBufSize = cfg.u32FrameBufSize;
            initParam.u32InternalBufAddr = cfg.u32DrvProcBufAddr;
            initParam.u32InternalBufSize = cfg.u32DrvProcBufSize;
            initParam.u32FrameRate = pInitParam->VideoInfo.u32FrameRate;
            initParam.u32FrameRateBase= pInitParam->VideoInfo.u32FrameRateBase;
            initParam.eFwSrcType = (MJPEG_FWSourceType)cfg.eFWSourceType;
            //For M10. frame buffer size is controlled by width and height
            initParam.u32Width = (MS_U32)pInitParam->VideoInfo.u16Width[0];
            initParam.u32Height = (MS_U32)pInitParam->VideoInfo.u16Height[0];

            MApi_MJPEG_EnableTurboFWMode(bNotReloadFW);

            _MJPEG_RET_HANDLE(MApi_MJPEG_Init(&initParam));

            break;
        }
        case E_VDEC_DECODER_RVD:
        _RET_VDEC_RVD_LINT_CHECK();
        {
            VDEC_SysCfg cfg = pInitParam->SysConfig;
            RVD_FileInfo info;
            RVD_MEMCfg memCfg;

            _VDEC_Memset(&info, 0, sizeof(RVD_FileInfo));
            _VDEC_Memset(&memCfg, 0, sizeof(RVD_MEMCfg));


            if (E_VDEC_CODEC_TYPE_RV8 == pInitParam->eCodecType)
            {
                info.RV_Version = 0;
            }
            else if (E_VDEC_CODEC_TYPE_RV9 == pInitParam->eCodecType)
            {
                info.RV_Version = 1;
            }

            info.ulNumSizes = pInitParam->VideoInfo.u16NumSizes;
            _VDEC_Memcpy(info.ulPicSizes_w, pInitParam->VideoInfo.u16Width, sizeof(pInitParam->VideoInfo.u16Width));
            _VDEC_Memcpy(info.ulPicSizes_h, pInitParam->VideoInfo.u16Height, sizeof(pInitParam->VideoInfo.u16Height));
            memCfg.eFWSourceType = (RVD_FWSourceType)cfg.eFWSourceType;
            memCfg.u32FWAddr = cfg.u32CodeBufAddr;
            memCfg.u32FWSize = cfg.u32CodeBufSize;
            memCfg.u32FBAddr = cfg.u32FrameBufAddr;
            memCfg.u32FBSize = cfg.u32FrameBufSize;
            memCfg.u32BSAddr = cfg.u32BitstreamBufAddr;
            memCfg.u32BSSize = cfg.u32BitstreamBufSize;
            memCfg.u32PBAddr = cfg.u32DrvProcBufAddr;
            memCfg.u32PBSize = cfg.u32DrvProcBufSize;
            memCfg.u32VLCBinAddr = cfg.u32VlcBinarySrcAddr;
            memCfg.u32VLCBinSize = cfg.u32VlcTabBinarySize;
            memCfg.u32FWBinAddr = cfg.u32FWBinaryAddr;
            memCfg.u32FWBinSize = cfg.u32FWBinarySize;
            {
                MS_U32 u32NonPMBankSize=0,u32NonPMBankAddr=0;
                if( !MDrv_MMIO_GetBASE( &u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_RVD))
                {
                    printf("_MDrv_RVD_Init: IOMap failure\n");
                    return E_VDEC_FAIL;
                }
                else
                {
                    MDrv_RVD_SetOSRegBase(u32NonPMBankAddr);
                    //RVD_MSG_INF("_MDrv_RVD_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr);
                }
            }
#if 0 //RVD reload fw mechanism need to be check
            MDrv_RVD_EnableTurboFWMode(bNotReloadFW);
#endif
            _RVD_RET_HANDLE(MDrv_RVD_Init(&info, &memCfg));
            if (pInitParam->EnableDynaScale)
            {
                MDrv_RVD_SetVirtualBox(pInitParam->u32DSVirtualBoxWidth, pInitParam->u32DSVirtualBoxHeight);
                MDrv_RVD_EnableDynamicScaling(TRUE);
            }
            break;
        }

        default:
        {
            if(_Attr.eDbgMsgLevel >= E_VDEC_DBG_LEVEL_ERR)
            {
                printf("VDEC Err: Decoder is not supported!!\n");
            }
            return E_VDEC_RET_INVALID_PARAM;
        }
    }

#if defined(VDEC_ISR_MONITOR)
    if(FALSE == _IsSysEnable)
    {
        if(FALSE == _VDEC_SYS_Init())
        {
            return E_VDEC_FAIL;
        }
        _IsSysEnable = TRUE;
    }
#else
    _VDEC_MutexInit();
#endif

    _Attr.bInit = TRUE;

#ifdef _MVD_EVENT_DBG
    _VDEC_MVDEventDbg();
#endif
    return E_VDEC_OK;
}

VDEC_Result MApi_VDEC_Init_EX(void *pInfo, MS_U32 u32Size)
{
    if (pu32VDEC_Inst == NULL)
    {
        if(UtopiaOpen(MODULE_VDEC, &pu32VDEC_Inst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Utopia Open VDEC failed\n");
            return E_VDEC_FAIL;
        }
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)pInfo;
    IO_arg.param[1] = (void*)&u32Size;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_INIT_EX, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_Init_EX(void *pInfo, MS_U32 u32Size)
{
    MS_BOOL bNotReloadFW = FALSE;
    MS_U32 u32Version = *((MS_U32 *)pInfo);

    if (_Attr.bInit)
    {
        printf("VDEC Warn: re-init VDEC Driver\n");
#if defined( MS_DEBUG )
        return E_VDEC_RET_NOT_EXIT;
#endif
    }

    if ((u32Version == 1) && (u32Size == sizeof(VDEC_InitParam_EX1)))
    {
        VDEC_InitParam_EX1 *pInitParam = (VDEC_InitParam_EX1 *)pInfo;

        u32VdecInitParamVersion = 1;

        _VDEC_InitLocalVar();

        _VDEC_Memcpy(&_vdecInitParam_ex1, pInitParam, sizeof(VDEC_InitParam_EX1));

        _Attr.eCodecType    = pInitParam->eCodecType;
        _Attr.eDecoder      = _VDEC_GetDecoderByCodecType(pInitParam->eCodecType);
        _Attr.eSrcMode      = pInitParam->VideoInfo.eSrcMode;
        _Attr.eDbgMsgLevel  = pInitParam->SysConfig.eDbgMsgLevel;

        // init debug message
        MApi_VDEC_SetDbgLevel(_Attr.eDbgMsgLevel);

#if VDEC_ENABLE_LINK_WEAK_SYMBOL
        // it is critical very any nos compile envi
        _VDEC_LinkWeakSymbolPatch();
#endif

        if (bTurboMode)
        {
            bNotReloadFW = TRUE;
        }
        switch (_Attr.eDecoder)
        {
            case E_VDEC_DECODER_MVD:
            _RET_VDEC_MVD_LINT_CHECK();
            {
                VDEC_SysCfg cfg = pInitParam->SysConfig;
                MVD_FWCfg fwCfg;
                MVD_MEMCfg memCfg;
                MVD_InfoCfg infoCfg;
                MVD_CodecType eMvdCodecType = E_MVD_CODEC_UNKNOWN;
                MVD_SrcMode   eMvdSrcMode;
                MVD_TIMESTAMP_TYPE eSyncType = E_MVD_TIMESTAMP_FREERUN;

                // check codec type
                eMvdCodecType = _VDEC_Map2MVDCodecType(_Attr.eCodecType);
                if (eMvdCodecType == E_MVD_CODEC_UNKNOWN)
                {
                    //printf("%s(%d): Unknown eMvdCodecType!\n", __FUNCTION__, __LINE__);
                    return E_VDEC_FAIL;
                }

                // check source type
                eMvdSrcMode = _VDEC_Map2MVDSrcMode(_Attr.eSrcMode);
                if ((eMvdSrcMode == 0xff) || (eMvdSrcMode >= MVD_SRCMODE_MAX))
                {
                    //printf("%s(%d): Invalid eMvdSrcMode!\n", __FUNCTION__, __LINE__);
                    return E_VDEC_FAIL;
                }

                _VDEC_Memset(&fwCfg, 0, sizeof(MVD_FWCfg));
                _VDEC_Memset(&memCfg, 0, sizeof(MVD_MEMCfg));

                fwCfg.eCodecType = eMvdCodecType;
                fwCfg.bNotReload = bNotReloadFW; //TRUE for not load fw more than once
                memCfg.eFWSrcType = (MVD_FWSrcType)cfg.eFWSourceType;
                memCfg.u32FWBinAddr = cfg.u32FWBinaryAddr;
                memCfg.u32FWBinSize = cfg.u32FWBinarySize;
                memCfg.u32FWCodeAddr = cfg.u32CodeBufAddr;
                memCfg.u32FWCodeSize = cfg.u32CodeBufSize;
                memCfg.u32FBAddr = cfg.u32FrameBufAddr;
                memCfg.u32FBSize = cfg.u32FrameBufSize;
                memCfg.u32BSAddr = cfg.u32BitstreamBufAddr;
                memCfg.u32BSSize = cfg.u32BitstreamBufSize;
                memCfg.u32DrvBufAddr = cfg.u32DrvProcBufAddr;
                memCfg.u32DrvBufSize = cfg.u32DrvProcBufSize;
                memCfg.bEnableDynScale = pInitParam->EnableDynaScale;
                #ifdef _VDEC_AUTH_IPCheck_TEST_CODE_
                memCfg.bSupportSDModeOnly = !bModeSelectHD;
                #endif

                infoCfg.u8CodecType = eMvdCodecType;
                infoCfg.u8BSProviderMode = eMvdSrcMode;
                infoCfg.bDisablePESParsing = bMvdParserDisable[eMvdSrcMode];


                fwCfg.stFBReduction.LumaFBReductionMode = (MVD_FB_Reduction_Type)pInitParam->stFBReduction.eLumaFBReduction;
                fwCfg.stFBReduction.ChromaFBReductionMode = (MVD_FB_Reduction_Type)pInitParam->stFBReduction.eChromaFBReduction;
                fwCfg.stFBReduction.u8EnableAutoMode = (MS_U8)pInitParam->stFBReduction.bEnableAutoMode;

                _MVD_RET_HANDLE(MDrv_MVD_SetCfg(&fwCfg, &memCfg, &infoCfg));

                //the very first step: Get register base addr
                //notice: must do this first before accessing any register.
                MS_U32 u32NonPMBankSize;
                MS_U32 u32NonPMBankAddr = 0;
                if (!MDrv_MMIO_GetBASE(&u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_MVD))
                {
                    printf("_MVD_Init: IOMap failure\n");
                    return E_VDEC_FAIL;
                }
                else
                {
                    MDrv_MVD_RegSetBase(u32NonPMBankAddr);
                    //printf("_MVD_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr);
                }

                if (!MDrv_MVD_Init())
                {
                    return E_VDEC_FAIL;
                }

                if (eMvdSrcMode == E_MVD_TS_FILE_MODE)
                {
                    MS_U32 u32bsBufUnderflowTH = ((((cfg.u32BitstreamBufSize*15)/16)>>3)<<3);
                    MS_U32 u32bsBufOverflowTH  = (((cfg.u32BitstreamBufSize-0x4000)>>3)<<3); //16K
                    MDrv_MVD_SetUnderflowTH(u32bsBufUnderflowTH);
                    MDrv_MVD_SetOverflowTH(u32bsBufOverflowTH);
                }

                MDrv_MVD_SetCodecInfo(eMvdCodecType, eMvdSrcMode, bMvdParserDisable[eMvdSrcMode]);

                if (FALSE == MDrv_MVD_DropErrorFrame(!pInitParam->bDisableDropErrFrame))
                {
                    return E_VDEC_FAIL;
                }
                bDropErrFrm = (!pInitParam->bDisableDropErrFrame);

                if (FALSE == MDrv_MVD_DispRepeatField(pInitParam->bRepeatLastField))
                {
                    return E_VDEC_FAIL;
                }

                MDrv_MVD_DisableErrConceal(!pInitParam->bDisableErrConceal);
                /*
                E_MVD_Result ret = MDrv_MVD_DisableErrConceal(pInitParam->bDisableErrConceal);


                if (E_MVD_RET_OK != ret)
                {
                    return E_VDEC_FAIL;
                }
                */

                MDrv_MVD_SetVirtualBox(pInitParam->u32DSVirtualBoxWidth, pInitParam->u32DSVirtualBoxHeight);

                if (eMvdSrcMode == E_MVD_SLQ_TBL_MODE)
                {
                    if (E_VDEC_TIME_STAMP_DTS == pInitParam->VideoInfo.eTimeStampType)
                    {
                        eSyncType = E_MVD_TIMESTAMP_DTS;
                    }
                    else if (E_VDEC_TIME_STAMP_PTS == pInitParam->VideoInfo.eTimeStampType)
                    {
                        eSyncType = E_MVD_TIMESTAMP_PTS;
                    }
                    else if (E_VDEC_TIME_STAMP_NONE == pInitParam->VideoInfo.eTimeStampType)
                    {
                        eSyncType = E_MVD_TIMESTAMP_FREERUN;
                    }
                    else if (E_VDEC_TIME_STAMP_STS == pInitParam->VideoInfo.eTimeStampType)
                    {
                        eSyncType = E_MVD_TIMESTAMP_STS;
                    }
                    else if (E_VDEC_TIME_STAMP_DTS_MPEG_DIRECTV_SD == pInitParam->VideoInfo.eTimeStampType)
                    {
                        eSyncType = E_MVD_TIMESTAMP_DTS_RVU;
                    }
                    else if (E_VDEC_TIME_STAMP_PTS_MPEG_DIRECTV_SD == pInitParam->VideoInfo.eTimeStampType)
                    {
                        eSyncType = E_MVD_TIMESTAMP_PTS_RVU;
                    }

                    else
                    {
                        printf("Invalid sync type %d for MVD\n", pInitParam->VideoInfo.eTimeStampType);
                        return E_VDEC_FAIL;
                    }
                    if ((eSyncType != E_MVD_TIMESTAMP_STS) && (FALSE == MDrv_MVD_SetFileModeAVSync(eSyncType)))
                    {
                        return E_VDEC_FAIL;
                    }
                }

                if ((eMvdSrcMode == E_MVD_TS_FILE_MODE)||(eMvdSrcMode == E_MVD_SLQ_TBL_MODE))
                {  //set up MVD for MediaCodec playback
                    _VDEC_MVDCfgDivX(_Attr.eCodecType);

                    if (eMvdCodecType == E_MVD_CODEC_DIVX311)
                    {
                        MVD_FrameInfo stFrminfo;
                        _VDEC_Memset(&stFrminfo, 0, sizeof(MVD_FrameInfo));
                        stFrminfo.u16HorSize = pInitParam->VideoInfo.u16Width[0];
                        stFrminfo.u16VerSize = pInitParam->VideoInfo.u16Height[0];
#ifdef MSOS_TYPE_LINUX_KERNEL
                        MS_U64 u64Tmp = (MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000;
                        do_div(u64Tmp, pInitParam->VideoInfo.u32FrameRateBase);
                        stFrminfo.u32FrameRate = (MS_U32)u64Tmp;
#else
                        stFrminfo.u32FrameRate =
                            (MS_U32)(((MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000) / pInitParam->VideoInfo.u32FrameRateBase);
#endif
                        #if 0
                        printf("==>DivX311 w=0x%x h=0x%x fr=0x%lx,0x%lx\n",
                            pInitParam->VideoInfo.u16Width[0],pInitParam->VideoInfo.u16Height[0],
                            pInitParam->VideoInfo.u32FrameRate, pInitParam->VideoInfo.u32FrameRateBase);
                        printf("==>DivX311 w=0x%x h=0x%x fr=%ld\n",
                            stFrminfo.u16HorSize, stFrminfo.u16VerSize, stFrminfo.u32FrameRate);
                        #endif
                        MDrv_MVD_SetFrameInfo(&stFrminfo);
                    }
                    else if (eMvdCodecType != E_MVD_CODEC_MPEG2)
                    {
                        MVD_FrameInfo stFrminfo1;
                        _VDEC_Memset(&stFrminfo1, 0, sizeof(MVD_FrameInfo));
                        if(pInitParam->VideoInfo.u32FrameRateBase == 0)
                        {
                            printf("Invalid FrameRate base\n");
                            pInitParam->VideoInfo.u32FrameRateBase = 1;
                        }
#ifdef MSOS_TYPE_LINUX_KERNEL
                        MS_U64 u64Tmp = (MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000;
                        do_div(u64Tmp, pInitParam->VideoInfo.u32FrameRateBase);
                        stFrminfo1.u32FrameRate = (MS_U32)u64Tmp;
#else
                        stFrminfo1.u32FrameRate =
                            (MS_U32)(((MS_U64)pInitParam->VideoInfo.u32FrameRate * 1000) / pInitParam->VideoInfo.u32FrameRateBase);
#endif
                        MDrv_MVD_SetFrameInfo(&stFrminfo1);
                    }

                    if (MDrv_MVD_SkipToIFrame() != TRUE)
                    {
                        return E_VDEC_FAIL;
                    }
                }
                break;
            }
            case E_VDEC_DECODER_HVD:
            _RET_VDEC_HVD_LINT_CHECK();
            {
                VDEC_SysCfg cfg = pInitParam->SysConfig;
                HVD_MemCfg stMemCfg;
                HVD_Init_Settings stInitSettings;
                RV_Info stRVInfo;

                _VDEC_Memset(&stMemCfg, 0, sizeof(HVD_MemCfg));
                _VDEC_Memset(&stInitSettings, 0, sizeof(HVD_Init_Settings));
                _VDEC_Memset(&stRVInfo, 0, sizeof(RV_Info));

                stInitSettings.pRVFileInfo = &stRVInfo;

                stMemCfg.eFWSourceType = (HVD_FWSourceType) cfg.eFWSourceType;
                stMemCfg.u32FWBinaryAddr = (MS_PHYADDR) cfg.u32FWBinaryAddr;

                if (stMemCfg.eFWSourceType == E_HVD_FW_SOURCE_DRAM)
                {
                    stMemCfg.u32FWBinaryVAddr = MS_PA2KSEG1((MS_U32) cfg.u32FWBinaryAddr);
                }

                stMemCfg.u32FWBinarySize = cfg.u32FWBinarySize;

                stMemCfg.u32VLCBinaryAddr = (MS_PHYADDR) cfg.u32VlcBinarySrcAddr;

                if (stMemCfg.eFWSourceType == E_HVD_FW_SOURCE_DRAM)
                {
                    stMemCfg.u32VLCBinaryVAddr = MS_PA2KSEG1((MS_U32) cfg.u32VlcBinarySrcAddr);
                }

                stMemCfg.u32VLCBinarySize = cfg.u32VlcTabBinarySize;

                stMemCfg.u32CodeBufAddr  = (MS_PHYADDR) cfg.u32CodeBufAddr;
                stMemCfg.u32CodeBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32CodeBufAddr);
                stMemCfg.u32CodeBufSize  = cfg.u32CodeBufSize;
                stMemCfg.u32FrameBufAddr = (MS_PHYADDR) cfg.u32FrameBufAddr;
                stMemCfg.u32FrameBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32FrameBufAddr);
                stMemCfg.u32FrameBufSize = cfg.u32FrameBufSize;
                stMemCfg.u32BitstreamBufAddr = (MS_PHYADDR) cfg.u32BitstreamBufAddr;
                stMemCfg.u32BitstreamBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32BitstreamBufAddr);
                stMemCfg.u32BitstreamBufSize = cfg.u32BitstreamBufSize;

                stMemCfg.u32DrvProcessBufAddr = (MS_PHYADDR) cfg.u32DrvProcBufAddr;
                stMemCfg.u32DrvProcessBufVAddr = MS_PA2KSEG1((MS_U32) cfg.u32DrvProcBufAddr);
                stMemCfg.u32DrvProcessBufSize = cfg.u32DrvProcBufSize;

                // set codec flag
                if (E_VDEC_CODEC_TYPE_H264 == _Attr.eCodecType)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_HW_AVC;
                }
                else if (E_VDEC_CODEC_TYPE_AVS == _Attr.eCodecType)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_HW_AVS;
                }
                else if ((E_VDEC_CODEC_TYPE_RV8 == _Attr.eCodecType)
                      || (E_VDEC_CODEC_TYPE_RV9 == _Attr.eCodecType))
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_HW_RM;
                }

                // set input source mode
                if (E_VDEC_SRC_MODE_DTV == _Attr.eSrcMode)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_LIVE_STREAM;
                    stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_TSP;
                    stInitSettings.u8SyncType = E_HVD_SYNC_ATS;
                }
                else if (E_VDEC_SRC_MODE_TS_FILE == _Attr.eSrcMode)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_FILE_TS;
                    stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_TSP;
                    stInitSettings.u8SyncType = E_HVD_SYNC_ATS;
                }
                else if (E_VDEC_SRC_MODE_FILE == _Attr.eSrcMode)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_MAIN_FILE_RAW;
                    stInitSettings.u32ModeFlag |= HVD_INIT_INPUT_DRV;

                    if (E_VDEC_TIME_STAMP_STS == pInitParam->VideoInfo.eTimeStampType)
                    {
                        stInitSettings.u8SyncType = E_HVD_SYNC_STS;
                    }
                    else if (E_VDEC_TIME_STAMP_NONE == pInitParam->VideoInfo.eTimeStampType)
                    {
                        stInitSettings.u8SyncType = E_HVD_SYNC_ATS;
                    }
                    else if (E_VDEC_TIME_STAMP_DTS == pInitParam->VideoInfo.eTimeStampType)
                    {
                        stInitSettings.u8SyncType = E_HVD_SYNC_DTS;
                    }
                    else if (E_VDEC_TIME_STAMP_PTS == pInitParam->VideoInfo.eTimeStampType)
                    {
                        stInitSettings.u8SyncType = E_HVD_SYNC_PTS;
                    }
                    else
                    {
                        printf("Invalid sync type %d\n", pInitParam->VideoInfo.eTimeStampType);

                        return E_VDEC_FAIL;
                    }
                }

                // set start code exist
                if (pInitParam->VideoInfo.bWithoutNalStCode)
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_START_CODE_REMOVED;
                }
                else
                {
                    stInitSettings.u32ModeFlag |= HVD_INIT_START_CODE_REMAINED;
                }

                // pro settings
                stInitSettings.u32FrameRate = pInitParam->VideoInfo.u32FrameRate;
                stInitSettings.u32FrameRateBase = pInitParam->VideoInfo.u32FrameRateBase;

                if (E_VDEC_SRC_MODE_FILE == _Attr.eSrcMode)
                {
                    stInitSettings.u8MinFrmGap = 2; // default set
                }
                else
                {
                    stInitSettings.u8MinFrmGap = 0; // default set
                }

                stInitSettings.bAutoPowerSaving = TRUE;
#if defined(SEC_X4)
                stInitSettings.bFastDisplay = FALSE;
#else
                stInitSettings.bFastDisplay = TRUE;
#endif
                stInitSettings.u8TimeUnit = 1;    // ms
                stInitSettings.bDynamicScaling = pInitParam->EnableDynaScale;

                if (bNotReloadFW)
                {
                    stInitSettings.u8TurboInit |= E_HVD_TURBOINIT_FW_RELOAD; //set for not load fw more than once
                }

                // for RM
                if (E_VDEC_CODEC_TYPE_RV8 == pInitParam->eCodecType)
                {
                    stRVInfo.RV_Version = 0;
                }
                else if (E_VDEC_CODEC_TYPE_RV9 == pInitParam->eCodecType)
                {
                    stRVInfo.RV_Version = 1;
                }

                stRVInfo.ulNumSizes = pInitParam->VideoInfo.u16NumSizes;
                _VDEC_Memcpy(stRVInfo.ulPicSizes_w, pInitParam->VideoInfo.u16Width, sizeof(pInitParam->VideoInfo.u16Width));
                _VDEC_Memcpy(stRVInfo.ulPicSizes_h, pInitParam->VideoInfo.u16Height, sizeof(pInitParam->VideoInfo.u16Height));

                {
                    MS_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0;
                    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
                    {
                        printf("VDEC HVD Init Err: MMIO_GetBASE failure\n");
                        return E_VDEC_FAIL;
                    }
                    else
                    {
                        //printf("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
                        MDrv_HVD_SetOSRegBase(u32RiuBaseAdd);
                    }
                }

                stInitSettings.stFBReduction.LumaFBReductionMode = (HVD_FB_Reduction_Type)pInitParam->stFBReduction.eLumaFBReduction;
                stInitSettings.stFBReduction.ChromaFBReductionMode = (HVD_FB_Reduction_Type)pInitParam->stFBReduction.eChromaFBReduction;
                stInitSettings.stFBReduction.u8EnableAutoMode = (MS_U8)pInitParam->stFBReduction.bEnableAutoMode;

                _HVD_RET_HANDLE(MDrv_HVD_Init(&stMemCfg, &stInitSettings));

                // SPS filter
                if (E_VDEC_SRC_MODE_DTV == _Attr.eSrcMode)
                {
                    HVD_Disp_Info_Threshold stDispInfoTH;

                    stDispInfoTH.u32FrmrateLowBound = 10000;
                    stDispInfoTH.u32FrmrateUpBound  = 0;
                    //stDispInfoTH.u32FrmrateUpBound  = 170000;
                    stDispInfoTH.u32MvopLowBound    = 16000000;
                    stDispInfoTH.u32MvopUpBound     = 0;

                    MDrv_HVD_SetDispInfoTH(&stDispInfoTH);
                }

                if (pInitParam->EnableDynaScale)
                {
                    _HVD_RET_HANDLE(MDrv_HVD_SetVirtualBox(pInitParam->u32DSVirtualBoxWidth,
                                    pInitParam->u32DSVirtualBoxHeight));
                }

                _HVD_RET_HANDLE(MDrv_HVD_SetDispErrFrm(pInitParam->bDisableDropErrFrame));

                _HVD_RET_HANDLE(MDrv_HVD_SetErrConceal(!pInitParam->bDisableErrConceal));

                break;
            }
            case E_VDEC_DECODER_MJPEG:
            {
                VDEC_SysCfg cfg = pInitParam->SysConfig;
                MJPEG_INIT_PARAM initParam;

                _VDEC_Memset(&initParam, 0, sizeof(MJPEG_INIT_PARAM));

                initParam.u32FwBinAddr = cfg.u32FWBinaryAddr;
                initParam.u32FwBinSize = cfg.u32FWBinarySize;
                initParam.u32FwAddr = cfg.u32CodeBufAddr;
                initParam.u32FwSize = cfg.u32CodeBufSize;
                initParam.u32MRCBufAddr = cfg.u32BitstreamBufAddr;
                initParam.u32MRCBufSize = cfg.u32BitstreamBufSize;
                initParam.u32MWCBufAddr = cfg.u32FrameBufAddr;
                initParam.u32MWCBufSize = cfg.u32FrameBufSize;
                initParam.u32InternalBufAddr = cfg.u32DrvProcBufAddr;
                initParam.u32InternalBufSize = cfg.u32DrvProcBufSize;
                initParam.u32FrameRate = pInitParam->VideoInfo.u32FrameRate;
                initParam.u32FrameRateBase= pInitParam->VideoInfo.u32FrameRateBase;
                initParam.eFwSrcType = (MJPEG_FWSourceType)cfg.eFWSourceType;
                MApi_MJPEG_EnableTurboFWMode(bNotReloadFW);

                _MJPEG_RET_HANDLE(MApi_MJPEG_Init(&initParam));

                break;
            }
            case E_VDEC_DECODER_RVD:
            _RET_VDEC_RVD_LINT_CHECK();
            {
                VDEC_SysCfg cfg = pInitParam->SysConfig;
                RVD_FileInfo info;
                RVD_MEMCfg memCfg;

                _VDEC_Memset(&info, 0, sizeof(RVD_FileInfo));
                _VDEC_Memset(&memCfg, 0, sizeof(RVD_MEMCfg));

                if (E_VDEC_CODEC_TYPE_RV8 == pInitParam->eCodecType)
                {
                    info.RV_Version = 0;
                }
                else if (E_VDEC_CODEC_TYPE_RV9 == pInitParam->eCodecType)
                {
                    info.RV_Version = 1;
                }

                info.ulNumSizes = pInitParam->VideoInfo.u16NumSizes;
                _VDEC_Memcpy(info.ulPicSizes_w, pInitParam->VideoInfo.u16Width, sizeof(pInitParam->VideoInfo.u16Width));
                _VDEC_Memcpy(info.ulPicSizes_h, pInitParam->VideoInfo.u16Height, sizeof(pInitParam->VideoInfo.u16Height));
                memCfg.eFWSourceType = (RVD_FWSourceType)cfg.eFWSourceType;
                memCfg.u32FWAddr = cfg.u32CodeBufAddr;
                memCfg.u32FWSize = cfg.u32CodeBufSize;
                memCfg.u32FBAddr = cfg.u32FrameBufAddr;
                memCfg.u32FBSize = cfg.u32FrameBufSize;
                memCfg.u32BSAddr = cfg.u32BitstreamBufAddr;
                memCfg.u32BSSize = cfg.u32BitstreamBufSize;
                memCfg.u32PBAddr = cfg.u32DrvProcBufAddr;
                memCfg.u32PBSize = cfg.u32DrvProcBufSize;
                memCfg.u32VLCBinAddr = cfg.u32VlcBinarySrcAddr;
                memCfg.u32VLCBinSize = cfg.u32VlcTabBinarySize;
                memCfg.u32FWBinAddr = cfg.u32FWBinaryAddr;
                memCfg.u32FWBinSize = cfg.u32FWBinarySize;
                {
                    MS_U32 u32NonPMBankSize=0,u32NonPMBankAddr=0;
                    if( !MDrv_MMIO_GetBASE( &u32NonPMBankAddr, &u32NonPMBankSize, MS_MODULE_RVD))
                    {
                        printf("_MDrv_RVD_Init: IOMap failure\n");
                        return E_VDEC_FAIL;
                    }
                    else
                    {
                        MDrv_RVD_SetOSRegBase(u32NonPMBankAddr);
                        //RVD_MSG_INF("_MDrv_RVD_Init: u32RiuBaseAdd = %lx\n", u32NonPMBankAddr);
                    }
                }
#if 0 //RVD reload fw mechanism need to be check
                MDrv_RVD_EnableTurboFWMode(bNotReloadFW);
#endif
                _RVD_RET_HANDLE(MDrv_RVD_Init(&info, &memCfg));
                if (pInitParam->EnableDynaScale)
                {
                    MDrv_RVD_SetVirtualBox(pInitParam->u32DSVirtualBoxWidth, pInitParam->u32DSVirtualBoxHeight);
                    MDrv_RVD_EnableDynamicScaling(TRUE);
                }
                break;
            }

            default:
            {
                if(_Attr.eDbgMsgLevel >= E_VDEC_DBG_LEVEL_ERR)
                {
                    printf("VDEC Err: Decoder is not supported!!\n");
                }
                return E_VDEC_RET_INVALID_PARAM;
            }
        }

#if defined(VDEC_ISR_MONITOR)
        if(FALSE == _IsSysEnable)
        {
            if(FALSE == _VDEC_SYS_Init())
            {
                return E_VDEC_FAIL;
            }
            _IsSysEnable = TRUE;
        }
#else
        _VDEC_MutexInit();
#endif

        _Attr.bInit = TRUE;

#ifdef _MVD_EVENT_DBG
        _VDEC_MVDEventDbg();
#endif
        return E_VDEC_OK;
    }
    else
    {
        MS_ASSERT(FALSE);
        return E_VDEC_FAIL;
    }
}


//------------------------------------------------------------------------------
/// Reset decoder.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Rst(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;

    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_RST, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Reset decoder.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Rst(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            _MVD_RET_HANDLE(MDrv_MVD_Rst());

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_Rst(FALSE));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_Rst());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_Rst());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Stop playback
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Exit(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;

    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_EXIT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {

        if(UtopiaClose(pu32VDEC_Inst) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Utopia Close VDEC failed\n");
            pu32VDEC_Inst = NULL;
            return E_VDEC_FAIL;
        }
        pu32VDEC_Inst = NULL;


        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Stop playback
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Exit(MS_BOOL suspend_flag)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    //Disable VDEC ISR when exit.
#if defined(VDEC_ISR_MONITOR)
    _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
    MsOS_SetEvent(_s32VdecEventId, E_VDEC_EVENT_ISR_EVENT_CHANGE);
#else
    _u32VdecIsrEventFlag = 0;
#endif

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_Exit())
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_Exit());

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_Stop());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_Exit());

            break;
        }
        default:
            break;
    }
    
    if(suspend_flag == FALSE)
    {
        _VDEC_Memset((void*) &_Pre_Ctrl, 0, sizeof(VDEC_Pre_Ctrl));
        _Attr.bInit = FALSE;


#if defined(VDEC_ISR_MONITOR)
#if 0
        _VDEC_SYS_Exit();
#endif
#else
        _VDEC_MutexExit();
#endif
        _VDEC_DqMutexExit();
    }
    return E_VDEC_OK;
}

VDEC_Result MApi_VDEC_V2_SetPowerState(EN_POWER_MODE u16PowerState)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (u16PowerState == E_POWER_SUSPEND)
    {
        if(_Attr.bInit == TRUE)
        {
            MApi_VDEC_V2_Exit(TRUE);
        }

        _prev_u16PowerState = u16PowerState;
        ret = E_VDEC_OK;
    }
    else if (u16PowerState == E_POWER_RESUME)
    {
        if (_prev_u16PowerState == E_POWER_SUSPEND)
        {
            if(_Attr.bInit == TRUE)
            {
                MApi_VDEC_V2_Init(&(_vdecInitParam));
                MApi_VDEC_V2_Play();
            }
            _prev_u16PowerState = u16PowerState;
            ret = E_VDEC_OK;
         }
         else
         {
             printf("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
             ret = E_VDEC_FAIL;
         }
    }
    else
    {
        printf("[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
        ret = E_VDEC_FAIL;
    }
    return ret;
}

//------------------------------------------------------------------------------
/// Check if information for display setting is ready or not
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CheckDispInfoRdy(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;

    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CHECK_DISP_INFO_RDY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}

//------------------------------------------------------------------------------
/// Check if information for display setting is ready or not
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_CheckDispInfoRdy(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            if (0 == MDrv_MVD_GetDispRdy())
            {
                return E_VDEC_RET_NOT_READY;
            }

            break;

        case E_VDEC_DECODER_HVD:
        {
            HVD_Result ret = MDrv_HVD_CheckDispInfoRdy();

            if (E_HVD_OK != ret)
            {
                if (E_HVD_RET_NOTREADY == ret)
                {
                    return E_VDEC_RET_NOT_READY;
                }
                else if( ret == E_HVD_RET_UNSUPPORTED)
                {
                    return E_VDEC_RET_UNSUPPORTED;
                }
                else
                {
                    return E_VDEC_FAIL;
                }
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_CheckDispInfoRdy());

            break;
        }
        case E_VDEC_DECODER_RVD:
            if (!MDrv_RVD_CheckDispInfoRdy())
            {
                return E_VDEC_FAIL;
            }

            break;

        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Set up frame rate conversion mode
/// @param eFrcMode \b IN : frame rate conversion mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetFrcMode(VDEC_FrcMode eFrcMode)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;

    IO_arg.param[0] = (void*)&eFrcMode;
    IO_arg.pRet = (void*)&eRet;



    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_FRC_MODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}


//------------------------------------------------------------------------------
/// Set up frame rate conversion mode
/// @param eFrcMode \b IN : frame rate conversion mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetFrcMode(VDEC_FrcMode eFrcMode)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (E_VDEC_FRC_NORMAL == eFrcMode)
            {
                if (FALSE == MDrv_MVD_DispCtrl(FALSE, bDropErrFrm, FALSE, E_MVD_FRC_NORMAL))
                {
                    return E_VDEC_FAIL;
                }
            }
            else if (E_VDEC_FRC_DISP_TWICE == eFrcMode)
            {
                if (FALSE == MDrv_MVD_DispCtrl(FALSE, bDropErrFrm, FALSE, E_MVD_FRC_DISP_TWICE))
                {
                    return E_VDEC_FAIL;
                }
            }
            else if (E_VDEC_FRC_3_2_PULLDOWN == eFrcMode)
            {
                if (FALSE == MDrv_MVD_DispCtrl(FALSE, bDropErrFrm, FALSE, E_MVD_FRC_3_2_PULLDOWN))
                {
                    return E_VDEC_FAIL;
                }
            }
            else if ((E_VDEC_FRC_PAL_TO_NTSC == eFrcMode) || (E_VDEC_FRC_MODE_50P_60P == eFrcMode))
            {
                if (FALSE == MDrv_MVD_DispCtrl(FALSE, bDropErrFrm, FALSE, E_MVD_FRC_PAL_TO_NTSC))
                {
                    return E_VDEC_FAIL;
                }
            }
            else if ((E_VDEC_FRC_NTSC_TO_PAL == eFrcMode) || (E_VDEC_FRC_MODE_60P_50P == eFrcMode))
            {
                if (FALSE == MDrv_MVD_DispCtrl(FALSE, bDropErrFrm, FALSE, E_MVD_FRC_NTSC_TO_PAL))
                {
                    return E_VDEC_FAIL;
                }
            }
            else
            {
                return E_VDEC_RET_INVALID_PARAM;
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if (E_VDEC_FRC_NORMAL == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_NORMAL));
            }
            else if (E_VDEC_FRC_DISP_TWICE == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_DISP_2X));
            }
            else if (E_VDEC_FRC_3_2_PULLDOWN == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_32PULLDOWN));
            }
            else if (E_VDEC_FRC_PAL_TO_NTSC == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_PAL2NTSC));
            }
            else if (E_VDEC_FRC_NTSC_TO_PAL == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_NTSC2PAL));
            }
            else if (E_VDEC_FRC_MODE_50P_60P == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_50P_60P));
            }
            else if (E_VDEC_FRC_MODE_60P_50P == eFrcMode)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetFrcMode(E_HVD_FRC_MODE_60P_50P));
            }
            else
            {
                return E_VDEC_RET_INVALID_PARAM;
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            return E_VDEC_RET_UNSUPPORTED;

        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Set up some parameters about dynamic scaling in FW
/// @param u32Addr \b IN : the physical start address of parameter set
/// @param u32Size \b IN : the size of parameter set
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetDynScalingParams( MS_PHYADDR u32Addr , MS_U32 u32Size)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;

    IO_arg.param[0] = (void*)&u32Addr;
    IO_arg.param[1] = (void*)&u32Size;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_DYNSCALING_PARAMS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }


}


//------------------------------------------------------------------------------
/// Set up some parameters about dynamic scaling in FW
/// @param u32Addr \b IN : the physical start address of parameter set
/// @param u32Size \b IN : the size of parameter set
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetDynScalingParams( MS_PHYADDR u32Addr , MS_U32 u32Size)
{
    VDEC_Result ret = E_VDEC_OK;
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetDynScalingParam(u32Addr, u32Size));
            break;
        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_SetDynScalingParam(  (void*)MS_PA2KSEG1(u32Addr)  ,  u32Size ) );
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_RVD:
            _RVD_RET_HANDLE(MDrv_RVD_SetDynScalingParam(MS_PA2KSEG1(u32Addr), u32Size));
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}


//------------------------------------------------------------------------------
/// Start playback
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Play(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }


    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_PLAY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Start playback
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Play(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MDrv_MVD_Play();

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_Play());

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_Play());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_Play());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Pause decode and display
/// @return return VDEC_Result
//-----------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Pause(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_PAUSE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Pause decode and display
/// @return return VDEC_Result
//-----------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Pause(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MDrv_MVD_Pause();

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_Pause());

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_Pause());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_Pause());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Resume decode and display
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Resume(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_RESUME, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Resume decode and display
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Resume(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MDrv_MVD_Resume();
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_Play());

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_Resume());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_Resume());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Trigger to display one frame (background may still decoding)
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_StepDisp(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_STEP_DISP, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Trigger to display one frame (background may still decoding)
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_StepDisp(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_StepDisp())
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_StepDisp());

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_StepPlay());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_StepPlay());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Check if step play done or not
/// @return VDEC_Result
///     - E_VDEC_OK: step display done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsStepDispDone(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;;
    IO_arg.pRet = (void*)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_STEP_DISP_DONE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Check if step play done or not
/// @return VDEC_Result
///     - E_VDEC_OK: step display done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsStepDispDone(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_IsStepDispDone())
            {
                ret = E_VDEC_FAIL;
            }
            else
            {
                ret = E_VDEC_OK;
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsFrameShowed());
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret, MApi_MJPEG_IsStepPlayDone());
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_RVD_IsStepPlayDone());
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Decoder will start decode with full  speed until target PTS is reached (equal or larger) then pause.
/// @param u32PTS \b IN : target PTS
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SeekToPTS(MS_U32 u32PTS)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)&u32PTS;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SEEK_TO_PTS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Decoder will start decode with full  speed until target PTS is reached (equal or larger) then pause.
/// @param u32PTS \b IN : target PTS
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SeekToPTS(MS_U32 u32PTS)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_SeekToPTS(u32PTS))
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SeekToPTS(u32PTS));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_SeekToPTS(u32PTS));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_JumpToPTS(u32PTS));

            break;
        }
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Decoder will skip all frames before target PTS, and start decode with full  speed after target PTS is reached.
/// @param u32PTS \b IN : target PTS
///         - 0: turn off this mode.
///         - any not zero: enable this mode.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SkipToPTS(MS_U32 u32PTS)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)&u32PTS;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SKIP_TO_PTS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Decoder will skip all frames before target PTS, and start decode with full  speed after target PTS is reached.
/// @param u32PTS \b IN : target PTS
///         - 0: turn off this mode.
///         - any not zero: enable this mode.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SkipToPTS(MS_U32 u32PTS)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_SkipToPTS(u32PTS))
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SkipToPTS(u32PTS));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_SkipToPTS(u32PTS));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_SkipToPTS(u32PTS));

            break;
        }
        default:
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Trigger decoder to decode one frame.
/// Decoder seeks to an I frame, decodes and displays it.
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_StepDecode(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_STEP_DECODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Trigger decoder to decode one frame.
/// Decoder seeks to an I frame, decodes and displays it.
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_StepDecode(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_StepDecode())
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_StepDecode());

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            // Now we always return success
            _MJPEG_RET_HANDLE(MApi_MJPEG_StepDecode());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_StepDecode());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Check if step decode done or not
/// @return VDEC_Result
///     - E_VDEC_OK: step decode done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsStepDecodeDone(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_STEP_DECODE_DONE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Check if step decode done or not
/// @return VDEC_Result
///     - E_VDEC_OK: step decode done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsStepDecodeDone(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (FALSE == MDrv_MVD_IsStepDecodeDone())
            {
                return E_VDEC_FAIL;
            }
            else
            {
                return E_VDEC_OK;
            }
            break;
        }

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsStepDecodeDone());
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret, MApi_MJPEG_IsStepDecodeDone());
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret,  MDrv_RVD_IsStepDecodeDone());
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set up trick decode mode for decode I, decode IP and decode all.
/// @param eTrickDec \b IN : trick decode mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetTrickMode(VDEC_TrickDec eTrickDec)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)&eTrickDec;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_TRICK_MODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Set up trick decode mode for decode I, decode IP and decode all.
/// @param eTrickDec \b IN : trick decode mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetTrickMode(VDEC_TrickDec eTrickDec)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (E_VDEC_TRICK_DEC_ALL == eTrickDec)
            {
                if (FALSE == MDrv_MVD_TrickPlay(E_MVD_TRICK_DEC_ALL, 1))
                {
                    return E_VDEC_FAIL;
                }
            }
            else if (E_VDEC_TRICK_DEC_IP == eTrickDec)
            {
                if (FALSE == MDrv_MVD_TrickPlay(E_MVD_TRICK_DEC_IP, 1))
                {
                    return E_VDEC_FAIL;
                }
            }
            else if (E_VDEC_TRICK_DEC_I == eTrickDec)
            {
                if (FALSE == MDrv_MVD_TrickPlay(E_MVD_TRICK_DEC_I, 1))
                {
                    return E_VDEC_FAIL;
                }
            }
            else
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if (E_VDEC_TRICK_DEC_ALL == eTrickDec)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSkipDecMode(E_HVD_SKIP_DECODE_ALL));
            }
            else if (E_VDEC_TRICK_DEC_IP == eTrickDec)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSkipDecMode(E_HVD_SKIP_DECODE_IP));
            }
            else if (E_VDEC_TRICK_DEC_I == eTrickDec)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSkipDecMode(E_HVD_SKIP_DECODE_I));
            }
            else
            {
                return E_VDEC_RET_INVALID_PARAM;
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
            return E_VDEC_RET_UNSUPPORTED;

        case E_VDEC_DECODER_RVD:
        {
            if (E_VDEC_TRICK_DEC_ALL == eTrickDec)
            {
                MDrv_RVD_SetTrickMode(E_RVD_TRICKMODE_DECODE_ALL);
            }
            else if (E_VDEC_TRICK_DEC_IP == eTrickDec)
            {
                MDrv_RVD_SetTrickMode(E_RVD_TRICKMODE_DECODE_IP);
            }
            else if (E_VDEC_TRICK_DEC_I == eTrickDec)
            {
                MDrv_RVD_SetTrickMode(E_RVD_TRICKMODE_DECODE_I);
            }

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Push decode command queue
/// @param pCmd \b IN : the pointer to decode command
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_PushDecQ(VDEC_DecCmd *pCmd)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)pCmd;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_PUSH_DECQ, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}
//------------------------------------------------------------------------------
/// Push decode command queue
/// @param pCmd \b IN : the pointer to decode command
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_PushDecQ(VDEC_DecCmd *pCmd)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    if (bEsBufMgmt)
    {
        //minus bsStartAdd automatically since GetESBuff returns PA but PushDecQ use offset
        if(u32VdecInitParamVersion == 0)
        {
            pCmd->u32StAddr = pCmd->u32StAddr - _vdecInitParam.SysConfig.u32BitstreamBufAddr;
        }
        else if(u32VdecInitParamVersion == 1)
        {
            pCmd->u32StAddr = pCmd->u32StAddr - _vdecInitParam_ex1.SysConfig.u32BitstreamBufAddr;
        }
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_PacketInfo stMvdPktInfo;

            _VDEC_Memset(&stMvdPktInfo, 0, sizeof(MVD_PacketInfo));

            stMvdPktInfo.u32StAddr    = pCmd->u32StAddr;
            stMvdPktInfo.u32Length    = pCmd->u32Size;
            stMvdPktInfo.u32TimeStamp = pCmd->u32Timestamp;
            stMvdPktInfo.u32ID_H      = pCmd->u32ID_H;
            stMvdPktInfo.u32ID_L      = pCmd->u32ID_L;

            _MVD_RET_HANDLE(MDrv_MVD_PushQueue(&stMvdPktInfo));

            break;
        }
        case E_VDEC_DECODER_HVD:
        { // File mode
            HVD_Packet_Info packetInfo;

            _VDEC_Memset(&packetInfo, 0, sizeof(HVD_Packet_Info));

            packetInfo.u32Staddr    = pCmd->u32StAddr;
            packetInfo.u32Length    = pCmd->u32Size;
            packetInfo.u32TimeStamp = pCmd->u32Timestamp;
            packetInfo.u32ID_H      = pCmd->u32ID_H;
            packetInfo.u32ID_L      = pCmd->u32ID_L;

            _HVD_RET_HANDLE(MDrv_HVD_PushQueue(&packetInfo));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            MJPEG_Packet_Info packetInfo;

            _VDEC_Memset(&packetInfo, 0, sizeof(MJPEG_Packet_Info));

            packetInfo.u32StAddr    = pCmd->u32StAddr;
            packetInfo.u32Size      = pCmd->u32Size;
            packetInfo.u32TimeStamp = pCmd->u32Timestamp;
            packetInfo.u32ID_H      = pCmd->u32ID_H;
            packetInfo.u32ID_L      = pCmd->u32ID_L;

            _MJPEG_RET_HANDLE(MApi_MJPEG_PushQueue(&packetInfo));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            RVD_Packet_ID packetID;

            packetID.u32PacketID_HI = pCmd->u32ID_H;
            packetID.u32PacketID_LO = pCmd->u32ID_L;

            MDrv_RVD_PushBBU(pCmd->u32StAddr, pCmd->u32Size, &packetID);

            break;
        }
        default:
            break;
    }

#if _VDEC_PTS_TABLE
    _VDEC_PtsTablePut(pCmd->u32Timestamp);
#endif

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Fire the decode command which be pushed since last fired
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_FireDecCmd(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_FIRE_DEC_CMD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Fire the decode command which be pushed since last fired
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_FireDecCmd(void)
{
     VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            MDrv_MVD_SetSLQWritePtr(TRUE);
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_HVD:
            MDrv_HVD_PushQueue_Fire();
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret,  MDrv_RVD_FireDecCmd());
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get current queue vacancy
/// @return vacancy number
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetDecQVacancy(void)
{


    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Vacancy = 0;
    IO_arg.pRet = (void *)&u32Vacancy;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DECQ_VACANCY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Vacancy;
    }
}

//------------------------------------------------------------------------------
/// Get current queue vacancy
/// @return vacancy number
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetDecQVacancy(void)
{
    MS_U32 u32Vacancy = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Vacancy = MDrv_MVD_GetQueueVacancy(TRUE);

            break;

        case E_VDEC_DECODER_HVD:
            u32Vacancy = MDrv_HVD_GetBBUVacancy();

            break;

        case E_VDEC_DECODER_MJPEG:
            u32Vacancy = MApi_MJPEG_GetQueueVacancy();

            break;

        case E_VDEC_DECODER_RVD:
            u32Vacancy = MDrv_RVD_GetQmemSwBbuVacancy();

            break;

        default:
            u32Vacancy = 0;
            break;
    }

    return u32Vacancy;
}

MS_BOOL MApi_VDEC_IsCCAvailable(void)
{


    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_BOOL ret = FALSE;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_CC_AVAILABLE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        return ret;
    }
}

MS_BOOL MApi_VDEC_V2_IsCCAvailable(void)
{
    MS_BOOL ret = FALSE;
    if (!_Attr.bInit)
    {
        return FALSE;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            ret = MDrv_MVD_GetUsrDataIsAvailable();

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            ret = MDrv_HVD_GetUsrDataIsAvailable();
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            break;
        }
        default:
            break;
    }

    return ret;
}

VDEC_Result MApi_VDEC_GetCCInfo(void *pInfo, MS_U32 u32Size)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }


    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = pInfo;
    IO_arg.param[1] = (void*)(&u32Size);
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_CC_INFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}

VDEC_Result MApi_VDEC_V2_GetCCInfo(void *pInfo, MS_U32 u32Size)
{
    VDEC_Result ret = E_VDEC_OK;
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    MS_U32 u32Version = *((MS_U32 *) pInfo);

    if (u32Version == 0 && u32Size == sizeof(VDEC_CC_Info))
    {
        VDEC_CC_Info *pCCInfo = (VDEC_CC_Info *) pInfo;

        switch (_Attr.eDecoder)
        {
            case E_VDEC_DECODER_MVD:
            {
                MVD_UsrDataInfo stUsrInfo;
                _VDEC_Memset(&stUsrInfo, 0, sizeof(MVD_UsrDataInfo));

                _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_GetUsrDataInfo(&stUsrInfo));
                pCCInfo->u8PicStructure     = (VDEC_PicStructure)stUsrInfo.u8PicStruct;
                pCCInfo->u8TopFieldFirst    = stUsrInfo.u8TopFieldFirst;
                pCCInfo->u16TempRef         = stUsrInfo.u16TmpRef;
                pCCInfo->u32Pts             = stUsrInfo.u32Pts;
                pCCInfo->u32UserDataBuf     = stUsrInfo.u32DataBuf;
                pCCInfo->u32UserDataSize    = stUsrInfo.u8ByteCnt;

                break;
            }
            case E_VDEC_DECODER_HVD:
            {
                HVD_UserData_Info stUsrInfo;
                _VDEC_Memset(&stUsrInfo, 0, sizeof(HVD_UserData_Info));

                _HVD_RET_HANDLE(MDrv_HVD_GetUserDataInfo(&stUsrInfo));
                pCCInfo->u8PicStructure     = (VDEC_PicStructure)stUsrInfo.u8PicStruct;
                pCCInfo->u8TopFieldFirst    = stUsrInfo.u8TopFieldFirst;
                pCCInfo->u16TempRef         = stUsrInfo.u16TmpRef;
                pCCInfo->u32Pts             = stUsrInfo.u32Pts;
                pCCInfo->u32UserDataBuf     = stUsrInfo.u32DataBuf;
                pCCInfo->u32UserDataSize    = stUsrInfo.u8ByteCnt;

                break;
            }
            case E_VDEC_DECODER_MJPEG:
            {
                ret = E_VDEC_RET_UNSUPPORTED;
                break;
            }
            case E_VDEC_DECODER_RVD:
            {
                ret = E_VDEC_RET_UNSUPPORTED;
                break;
            }
            default:
                break;
        }
    }

    return ret;

}

//------------------------------------------------------------------------------
/// Flush command queue and internal data of decoder
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Flush(VDEC_FreezePicSelect eFreezePic)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)(&eFreezePic);
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_FLUSH, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}
//------------------------------------------------------------------------------
/// Flush command queue and internal data of decoder
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_Flush(VDEC_FreezePicSelect eFreezePic)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            _MVD_RET_HANDLE(MDrv_MVD_FlushQueue());

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if (eFreezePic == E_VDEC_FREEZE_AT_LAST_PIC)
            {
                _HVD_RET_HANDLE(MDrv_HVD_Flush(TRUE));
            }
            else
            {
                _HVD_RET_HANDLE(MDrv_HVD_Flush(FALSE));
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_FlushQueue());

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_FlushQueue());

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Get ES buffer write pointer
/// @return write pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetESWritePtr(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ptr = 0;
    IO_arg.pRet = (void*)(&u32Ptr);

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_ES_WRITE_PTR, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ptr;
    }
}

//------------------------------------------------------------------------------
/// Get ES buffer write pointer
/// @return write pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetESWritePtr(void)
{
    MS_U32 u32Ptr = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Ptr = (MS_U32) MDrv_MVD_GetESWritePtr();

            break;

        case E_VDEC_DECODER_HVD:
            u32Ptr = MDrv_HVD_GetESWritePtr();

            break;

        case E_VDEC_DECODER_MJPEG:
            u32Ptr = MApi_MJPEG_GetESWritePtr();

            break;

        case E_VDEC_DECODER_RVD:
            u32Ptr = MDrv_RVD_GetESWritePtr();

            break;

        default:
            u32Ptr = 0;
            break;
    }

    return u32Ptr;
}


//------------------------------------------------------------------------------
/// Get decoder ES buffer read pointer
/// @return read pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetESReadPtr(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ptr = 0;
    IO_arg.pRet = (void*)(&u32Ptr);

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_ES_READ_PTR, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ptr;
    }
}

//------------------------------------------------------------------------------
/// Get decoder ES buffer read pointer
/// @return read pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetESReadPtr(void)
{
    MS_U32 u32Ptr = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Ptr = (MS_U32) MDrv_MVD_GetESReadPtr();

            break;

        case E_VDEC_DECODER_HVD:
            u32Ptr = MDrv_HVD_GetESReadPtr();

            break;

        case E_VDEC_DECODER_MJPEG:
            u32Ptr = MApi_MJPEG_GetESReadPtr();

            break;

        case E_VDEC_DECODER_RVD:
            u32Ptr = MDrv_RVD_GetESReadPtr();

            break;

        default:
            u32Ptr = 0;
            break;
    }

    return u32Ptr;
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display all decoded frame when out of input data
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_EnableLastFrameShow(MS_BOOL bEnable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }


    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void*)(&bEnable);
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_ENABLE_LAST_FRAME_SHOW, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display all decoded frame when out of input data
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_EnableLastFrameShow(MS_BOOL bEnable)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            _MVD_RET_HANDLE(MDrv_MVD_EnableLastFrameShow(bEnable));

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetDataEnd(bEnable));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_EnableLastFrameShow(bEnable));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_EnableLastFrameShowed(bEnable));

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Check if all decoded frames are displayed when input stream is run out
/// @return VDEC_Result
///     - E_VDEC_OK: finished
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsDispFinish(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }


    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_DISP_FINISH, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Check if all decoded frames are displayed when input stream is run out
/// @return VDEC_Result
///     - E_VDEC_OK: finished
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsDispFinish(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            if (MDrv_MVD_IsDispFinish() == E_MVD_RET_OK)
                ret = E_VDEC_OK;
            else
                ret = E_VDEC_FAIL;
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsDispFinish());
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret,  MApi_MJPEG_IsDispFinish());
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret,  MDrv_RVD_IsDispFinish());
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set up display speed
/// @param eSpeedType \b IN : display speed type
/// @param eSpeed \b IN : multiple
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetSpeed(VDEC_SpeedType eSpeedType, VDEC_DispSpeed eSpeed)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }


    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0]= (void *)&eSpeedType;
    IO_arg.param[1] = (void *)&eSpeed;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_SPEED, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}



//------------------------------------------------------------------------------
/// Set up display speed
/// @param eSpeedType \b IN : display speed type
/// @param eSpeed \b IN : multiple
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetSpeed(VDEC_SpeedType eSpeedType, VDEC_DispSpeed eSpeed)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MS_U8 u8MvdSpeed = _VDEC_Map2MVDSpeed(eSpeed);

            if (u8MvdSpeed == 0)
            {
                return E_VDEC_RET_INVALID_PARAM;
            }

            if (E_VDEC_SPEED_FAST == eSpeedType)
            {
                _MVD_RET_HANDLE(MDrv_MVD_SetSpeed(E_MVD_SPEED_FAST, u8MvdSpeed));
            }
            else if (E_VDEC_SPEED_SLOW == eSpeedType)
            {
                _MVD_RET_HANDLE(MDrv_MVD_SetSpeed(E_MVD_SPEED_SLOW, u8MvdSpeed));
            }
            else
            {
                MS_ASSERT(u8MvdSpeed == 1);
                _MVD_RET_HANDLE(MDrv_MVD_SetSpeed(E_MVD_SPEED_DEFAULT, 1));
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            MS_U8 u8Speed = _VDEC_Map2HVDSpeed(eSpeed);

            if (E_VDEC_SPEED_FAST == eSpeedType)
            {
                switch (eSpeed)
                {
                    case E_VDEC_DISP_SPEED_32X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_FF_32X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_16X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_FF_16X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_8X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_FF_8X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_4X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_FF_4X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_2X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_FF_2X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_1X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_NORMAL_1X));

                        break;
                    }
                    default:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed((HVD_Drv_Disp_Speed) u8Speed));

                        break;
                    }
                }
            }
            else if (E_VDEC_SPEED_SLOW == eSpeedType)
            {
                switch (eSpeed)
                {
                    case E_VDEC_DISP_SPEED_32X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_SF_32X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_16X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_SF_16X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_8X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_SF_8X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_4X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_SF_4X));

                        break;
                    }
                    case E_VDEC_DISP_SPEED_2X:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_SF_2X));

                        break;
                    }
                    default:
                    {
                        _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed((HVD_Drv_Disp_Speed) -u8Speed));

                        break;
                    }
                }
            }
            else
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetDispSpeed(E_HVD_DISP_SPEED_NORMAL_1X));
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_SetSpeed((MJPEG_SpeedType) eSpeedType, (MJPEG_DispSpeed) eSpeed));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            if (E_VDEC_SPEED_FAST == eSpeedType)
            {
                switch (eSpeed)
                {
                    case E_VDEC_DISP_SPEED_32X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_FF_32X));

                        break;

                    case E_VDEC_DISP_SPEED_16X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_FF_16X));

                        break;

                    case E_VDEC_DISP_SPEED_8X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_FF_8X));

                        break;

                    case E_VDEC_DISP_SPEED_4X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_FF_4X));

                        break;

                    case E_VDEC_DISP_SPEED_2X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_FF_2X));

                        break;

                    case E_VDEC_DISP_SPEED_1X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_NORMAL_1X));

                        break;

                    default:
                        break;
                }
            }
            else if (E_VDEC_SPEED_SLOW == eSpeedType)
            {
                switch (eSpeed)
                {
                    case E_VDEC_DISP_SPEED_32X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_SF_32X));

                        break;

                    case E_VDEC_DISP_SPEED_16X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_SF_16X));

                        break;

                    case E_VDEC_DISP_SPEED_8X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_SF_8X));

                        break;

                    case E_VDEC_DISP_SPEED_4X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_SF_4X));

                        break;

                    case E_VDEC_DISP_SPEED_2X:
                        _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_SF_2X));

                        break;

                    default:
                        break;
                }
            }
            else
            {
                _RVD_RET_HANDLE(MDrv_RVD_SetSpeed(E_RVD_DISP_SPEED_NORMAL_1X));
            }

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Check if the first frame is showed after play function is called
/// @return VDEC_Result
///     - E_VDEC_OK: ready
///     - E_VDEC_FAIL: not ready
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsFrameRdy(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_FRAME_RDY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Check if the first frame is showed after play function is called
/// @return VDEC_Result
///     - E_VDEC_OK: ready
///     - E_VDEC_FAIL: not ready
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsFrameRdy(void)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_Is1stFrmRdy());
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_Is1stFrmRdy());
            break;
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_OK;
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_RVD_Is1stFrameRdy());
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Let FW stop updating frames when vsync, but decoding process is still going.
/// @param bEnable \b IN : Enable/Disable
/// @return -The result of command freeze image.
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetFreezeDisp(MS_BOOL bEnable)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&bEnable;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_FREEZE_DISP, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }


}


//------------------------------------------------------------------------------
/// Let FW stop updating frames when vsync, but decoding process is still going.
/// @param bEnable \b IN : Enable/Disable
/// @return -The result of command freeze image.
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetFreezeDisp(MS_BOOL bEnable)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _MVD_RET_HANDLE(MDrv_MVD_SetFreezeDisp(bEnable));
            break;

        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetFreezeImg(bEnable));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_SetFreezeDisp(bEnable));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_SetFreezeImg(bEnable));

            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Set blue screen on/off
/// @param bOn \b IN : turn on / off blue screen
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetBlueScreen(MS_BOOL bOn)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&bOn;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_BLUE_SCREEN, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Set blue screen on/off
/// @param bOn \b IN : turn on / off blue screen
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetBlueScreen(MS_BOOL bOn)
{
    VDEC_Result rst = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(rst, MDrv_MVD_SetBlueScreen(bOn));
            break;

        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetBlueScreen(bOn));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_SetBlueScreen(bOn));

            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            _RVD_RET_HANDLE(MDrv_RVD_SetBlueScreen(bOn));

            break;
        }
        default:
            break;
    }

    return rst;
}

//------------------------------------------------------------------------------
/// Reset PTS
/// @param u32PtsBase \b IN : new PTS base value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_ResetPTS(MS_U32 u32PtsBase)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&u32PtsBase;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_RESET_PTS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Reset PTS
/// @param u32PtsBase \b IN : new PTS base value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_ResetPTS(MS_U32 u32PtsBase)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            _MVD_RET_HANDLE(MDrv_MVD_ResetPTS(u32PtsBase));

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_RstPTS(u32PtsBase));

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            _MJPEG_RET_HANDLE(MApi_MJPEG_ResetPTS(u32PtsBase));

            break;
        }
        case E_VDEC_DECODER_RVD:
            break;

        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Switch AV sync on/off
/// @param bOn \b IN : on/off
/// @param u32SyncDelay \b IN : delay value
/// @param u16SyncTolerance \b IN : tolerance value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_AVSyncOn(MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&bOn;
    IO_arg.param[1] = (void *)&u32SyncDelay;
    IO_arg.param[2] = (void *)&u16SyncTolerance;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_AVSYNC_ON, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Switch AV sync on/off
/// @param bOn \b IN : on/off
/// @param u32SyncDelay \b IN : delay value
/// @param u16SyncTolerance \b IN : tolerance value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_AVSyncOn(MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MDrv_MVD_SetAVSync(bOn, u32SyncDelay);

            if ((u16SyncTolerance!=0) && (FALSE == MDrv_MVD_ChangeAVsync(bOn, u16SyncTolerance)))
            {
                return E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if (bOn)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSyncActive(TRUE));
            }
            else
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSyncActive(FALSE));
            }

            if (bOn)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSyncVideoDelay(u32SyncDelay));
            }

            if (bOn)
            {
                _HVD_RET_HANDLE(MDrv_HVD_SetSyncTolerance(u16SyncTolerance));
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
            _MJPEG_RET_HANDLE(MApi_MJPEG_AVSyncOn(bOn, u32SyncDelay, u16SyncTolerance));

            break;

        case E_VDEC_DECODER_RVD:
            _RVD_RET_HANDLE(MDrv_RVD_AVSyncOn(bOn, u32SyncDelay, u16SyncTolerance));

            break;

        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Switch AV sync free run threshold
/// @param u32Threshold \b IN : threshold value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetAVSyncFreerunThreshold(MS_U32 u32Threshold )
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&u32Threshold;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_AVSYNC_FREERUN_THRESHOLD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Switch AV sync free run threshold
/// @param u32Threshold \b IN : threshold value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetAVSyncFreerunThreshold(MS_U32 u32Threshold )
{
    VDEC_Result ret=E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MDrv_MVD_SetAVSyncFreerunThreshold( u32Threshold );
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE( MDrv_HVD_SetSyncFreeRunTH( u32Threshold ) );
            break;
        }
        case E_VDEC_DECODER_MJPEG:
            break;

        case E_VDEC_DECODER_RVD:
            _RVD_RET_HANDLE(MDrv_RVD_SetAVSyncFreerunThreshold(u32Threshold));
            break;

        default:
            ret=E_VDEC_FAIL;
            break;
    }

    return ret;
}



//------------------------------------------------------------------------------
/// Check if AV sync on or not
/// @return VDEC_Result
///     - E_VDEC_OK: AV sync on
///     - E_VDEC_FAIL: freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsAVSyncOn(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_AVSYNC_ON, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Check if AV sync on or not
/// @return VDEC_Result
///     - E_VDEC_OK: AV sync on
///     - E_VDEC_FAIL: freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsAVSyncOn(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_GetIsAVSyncOn());
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, (MS_BOOL) MDrv_HVD_GetPlayMode(E_HVD_GMODE_IS_SYNC_ON));
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret, MApi_MJPEG_IsAVSyncOn());
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_RVD_IsAVSyncOn());
            break;

        default:
            ret =  E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the PTS of current displayed frame
/// @return PTS
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetPTS(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Pts = 0;

    IO_arg.pRet = (void *)&u32Pts;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_PTS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Pts;
    }


}




//------------------------------------------------------------------------------
/// Get the PTS of current displayed frame
/// @return PTS
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetPTS(void)
{
    MS_U32 u32Pts = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Pts = MDrv_MVD_GetPTS()+MDrv_MVD_GetAVSyncDelay();

            break;

        case E_VDEC_DECODER_HVD:
            u32Pts = MDrv_HVD_GetPTS();

            break;

        case E_VDEC_DECODER_MJPEG:
            u32Pts = MApi_MJPEG_GetPTS();

            break;

        case E_VDEC_DECODER_RVD:
            u32Pts = MDrv_RVD_GetTimeStamp();

            break;

        default:
            u32Pts = 0;
            break;
    }

    return u32Pts;
}


//------------------------------------------------------------------------------
/// Get the PTS of next displayed frame
/// @return next PTS (unit:ms)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetNextPTS(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Pts = 0;

    IO_arg.pRet = (void *)&u32Pts;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_NEXT_PTS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Pts;
    }
}


//------------------------------------------------------------------------------
/// Get the PTS of next displayed frame
/// @return next PTS (unit:ms)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetNextPTS(void)
{
    MS_U32 u32Pts = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Pts = MDrv_MVD_GetNextPTS();
            break;

        case E_VDEC_DECODER_HVD:
            u32Pts = MDrv_HVD_GetNextPTS();
            break;

        case E_VDEC_DECODER_MJPEG:
            u32Pts = 0;
            break;

        case E_VDEC_DECODER_RVD:
            u32Pts = 0;
            break;

        default:
            u32Pts = 0;
            break;
    }

    return u32Pts;
}

//------------------------------------------------------------------------------
/// Check if decoder is doing AV sync now
/// @return VDEC_Result
///     - E_VDEC_OK: doing AV sync
///     - E_VDEC_FAIL: still freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsStartSync(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_START_SYNC, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}


//------------------------------------------------------------------------------
/// Check if decoder is doing AV sync now
/// @return VDEC_Result
///     - E_VDEC_OK: doing AV sync
///     - E_VDEC_FAIL: still freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsStartSync(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            if(MDrv_MVD_GetIsSyncSkip() || MDrv_MVD_GetIsSyncRep())
            {
                ret = E_VDEC_OK;
            }
            else
            {
                ret = E_VDEC_FAIL;
            }
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsSyncStart());
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if the distance of Video time stamp and STC is closed enough.
/// @return VDEC_Result
///     - E_VDEC_OK: sync complete
///     - E_VDEC_FAIL: sync is incomplete
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsReachSync(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_REACH_SYNC, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}


//------------------------------------------------------------------------------
/// Check if the distance of Video time stamp and STC is closed enough.
/// @return VDEC_Result
///     - E_VDEC_OK: sync complete
///     - E_VDEC_FAIL: sync is incomplete
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsReachSync(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret,  (MS_BOOL) (MDrv_MVD_GetSyncStatus()==1));
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsSyncReach());
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret, MApi_MJPEG_IsReachSync());
            break;

        case E_VDEC_DECODER_RVD:
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Check if decoder is avsync freerun
/// @return VDEC_Result
///     - E_VDEC_OK: freerun
///     - E_VDEC_FAIL: not freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsFreerun(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_FREERUN, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }


}


//------------------------------------------------------------------------------
/// Check if decoder is avsync freerun
/// @return VDEC_Result
///     - E_VDEC_OK: freerun
///     - E_VDEC_FAIL: not freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsFreerun(void)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret,  MDrv_MVD_GetIsFreerun());
            break;

        case E_VDEC_DECODER_HVD:
            break;

        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, decode one I frame
/// @param u32FrameBufAddr \b IN : output buffer address
/// @param u32SrcSt \b IN : source start address
/// @param u32SrcEnd \b IN : source end address
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_MHEG_DecodeIFrame(MS_PHYADDR u32FrameBufAddr,
                                        MS_PHYADDR u32SrcSt,
                                        MS_PHYADDR u32SrcEnd)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)(&u32FrameBufAddr);
    IO_arg.param[1] = (void *)(&u32SrcSt);
    IO_arg.param[2] = (void *)(&u32SrcEnd);
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_MHEG_DECODE_I_FRAME, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }


}


//------------------------------------------------------------------------------
/// For MHEG5 MW, decode one I frame
/// @param u32FrameBufAddr \b IN : output buffer address
/// @param u32SrcSt \b IN : source start address
/// @param u32SrcEnd \b IN : source end address
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_MHEG_DecodeIFrame(MS_PHYADDR u32FrameBufAddr,
                                        MS_PHYADDR u32SrcSt,
                                        MS_PHYADDR u32SrcEnd)
{
    VDEC_Result retVal = E_VDEC_OK;;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (MDrv_MVD_DecodeIFrame(u32FrameBufAddr, u32SrcSt, u32SrcEnd))
            {
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if(u32VdecInitParamVersion == 0)
            {
                if (E_VDEC_SRC_MODE_DTV == _vdecInitParam.VideoInfo.eSrcMode ||
                    E_VDEC_SRC_MODE_TS_FILE == _vdecInitParam.VideoInfo.eSrcMode)
                {
                    VDEC_InitParam stVdecInitParam;
                    _VDEC_Memcpy(&stVdecInitParam, &_vdecInitParam, sizeof(VDEC_InitParam));

                    // change to file mode
                    stVdecInitParam.VideoInfo.eSrcMode = E_VDEC_SRC_MODE_FILE;

                    MApi_VDEC_Exit();

                    // re-init VDEC
                    MApi_VDEC_Init(&stVdecInitParam);
                }
            }
            else if(u32VdecInitParamVersion == 1)
            {
                if (E_VDEC_SRC_MODE_DTV == _vdecInitParam_ex1.VideoInfo.eSrcMode ||
                    E_VDEC_SRC_MODE_TS_FILE == _vdecInitParam_ex1.VideoInfo.eSrcMode)
                {
                    VDEC_InitParam_EX1 stVdecInitParam_ex1;
                    _VDEC_Memcpy(&stVdecInitParam_ex1, &_vdecInitParam_ex1, sizeof(VDEC_InitParam_EX1));

                    // change to file mode
                    stVdecInitParam_ex1.VideoInfo.eSrcMode = E_VDEC_SRC_MODE_FILE;

                    MApi_VDEC_Exit();

                    // re-init VDEC
                    MApi_VDEC_Init_EX((void *)&stVdecInitParam_ex1, sizeof(VDEC_InitParam_EX1));
                }
            }

            if (MDrv_HVD_DecodeIFrame(u32SrcSt, (u32SrcEnd - u32SrcSt)) == E_HVD_OK)
            {
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_FAIL;
            }
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            retVal = E_VDEC_RET_UNSUPPORTED;

            break;
    }

    return retVal;
}


//------------------------------------------------------------------------------
/// For MHEG5 MW, check if I frame decode done
/// @return VDEC_Result
///     - E_VDEC_OK: decode done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_MHEG_IsIFrameDecoding(void)
{


    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_MHEG_IS_I_FRAME_DECODING, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, check if I frame decode done
/// @return VDEC_Result
///     - E_VDEC_OK: decode done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_MHEG_IsIFrameDecoding(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_GetIsIFrameDecoding());
            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// For MHEG5 MW, reset decoder after I frame decode done
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_MHEG_RstIFrameDec(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_MHEG_RST_I_FRAME_DEC, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// For MHEG5 MW, reset decoder after I frame decode done
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_MHEG_RstIFrameDec(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            MDrv_MVD_RstIFrameDec();

            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            return E_VDEC_RET_UNSUPPORTED;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Get error code
/// @return error code
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetErrCode(void)
{


    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.pRet = (void *)&u32Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_ERR_CODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ret;
    }

}


//------------------------------------------------------------------------------
/// Get error code
/// @return error code
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetErrCode(void)
{
    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MVD_ErrCode errCode=E_MVD_ERR_UNKNOWN;
                MVD_ErrStatus errStatus=E_MVD_ERR_STATUS_UNKOWN;

                MDrv_MVD_GetErrInfo(&errCode, &errStatus);

                _Attr.eErrCode = (VDEC_ErrCode) (errCode + (MS_U32) E_VDEC_MVD_ERR_CODE_BASE);

                //Check if framerate is out of HW spec
                if ((E_MVD_ERR_UNKNOWN == errCode) && (MApi_VDEC_CheckDispInfoRdy() == E_VDEC_OK))
                {
                    //printf("%s(%d) check MVD FrmRate eErrCode=0x%x\n", __FUNCTION__, __LINE__, _Attr.eErrCode);
                    // only check when DispInfo is ready
                    if (FALSE == MDrv_MVD_GetFrmRateIsSupported())
                    {
                        _Attr.eErrCode = E_VDEC_ERR_CODE_FRMRATE_NOT_SUPPORT;
                        //printf("%s(%d) NS!!! eErrCode=0x%x\n", __FUNCTION__, __LINE__, _Attr.eErrCode);
                    }

                    if ( (bIsSupportDivxPlus == FALSE) && (MDrv_MVD_GetDivxVer() != 0) )
                    {
                        _Attr.eErrCode = E_VDEC_ERR_CODE_DIVX_PLUS_UNSUPPORTED;
                    }
                }
            }

            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U32 HVDerrCode=0;
                HVDerrCode = MDrv_HVD_GetErrCode();
                _Attr.eErrCode = (VDEC_ErrCode) ( HVDerrCode + (MS_U32) E_VDEC_HVD_ERR_CODE_BASE);

                //Check if framerate is out of HW spec
                if ((HVDerrCode == 0) && (MApi_VDEC_CheckDispInfoRdy() == E_VDEC_OK))
                {
                    //printf("%s(%d) check HVD FrmRate eErrCode=0x%x\n", __FUNCTION__, __LINE__, _Attr.eErrCode);
                    // only check when DispInfo is ready
                    if (FALSE == MDrv_HVD_GetFrmRateIsSupported())
                    {
                        _Attr.eErrCode = E_VDEC_ERR_CODE_FRMRATE_NOT_SUPPORT;
                        //printf("%s(%d) NS!!! eErrCode=0x%x\n", __FUNCTION__, __LINE__, _Attr.eErrCode);
                    }
                }
            }
            break;

        case E_VDEC_DECODER_MJPEG:
            _Attr.eErrCode = (VDEC_ErrCode) (MApi_MJPEG_GetErrCode() + (MS_U32) E_VDEC_MJPEG_ERR_CODE_BASE);

            break;

        case E_VDEC_DECODER_RVD:
            _Attr.eErrCode = (VDEC_ErrCode) (MDrv_RVD_GetErrCode() + (MS_U32) E_VDEC_RVD_ERR_CODE_BASE);

            break;

        default:
            _Attr.eErrCode=(VDEC_ErrCode)(0);
            break;
    }
    if( ( (MS_U32)(_Attr.eErrCode) & 0x00ffffff) == 0 )
    {
        _Attr.eErrCode=(VDEC_ErrCode)(0);
    }
    return _Attr.eErrCode;
}

//------------------------------------------------------------------------------
/// Get accumulated error counter
/// @return error counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetErrCnt(void)
{


    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.pRet = (void *)&u32Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_ERR_CNT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ret;
    }


}



//------------------------------------------------------------------------------
/// Get accumulated error counter
/// @return error counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetErrCnt(void)
{
    MS_U32 u32Cnt = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Cnt = MDrv_MVD_GetVldErrCount();
            break;

        case E_VDEC_DECODER_HVD:
            u32Cnt = MDrv_HVD_GetDecErrCnt()+MDrv_HVD_GetDataErrCnt();

            break;

        case E_VDEC_DECODER_MJPEG:
            u32Cnt = MApi_MJPEG_GetErrCnt();

            break;

        case E_VDEC_DECODER_RVD:
            u32Cnt = MDrv_RVD_GetDecErrCnt() + MDrv_RVD_GetDataErrCnt();

            break;

        default:
            u32Cnt = 0;
            break;
    }

    return u32Cnt;
}


//------------------------------------------------------------------------------
/// Get codec type which be initialed
/// @return VDEC_CodecType
//------------------------------------------------------------------------------
VDEC_CodecType MApi_VDEC_GetActiveCodecType(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_CODEC_TYPE_NONE;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_CodecType eRet = E_VDEC_CODEC_TYPE_NONE;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_ACTIVE_CODEC_TYPE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Get codec type which be initialed
/// @return VDEC_CodecType
//------------------------------------------------------------------------------
VDEC_CodecType MApi_VDEC_V2_GetActiveCodecType(void)
{
    return _Attr.eCodecType;
}


//------------------------------------------------------------------------------
/// Get bitrate which decoder retrieved from stream
/// @return bitrate
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetBitsRate(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.pRet = (void *)&u32Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_BITS_RATE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ret;
    }
}

//------------------------------------------------------------------------------
/// Get bitrate which decoder retrieved from stream
/// @return bitrate
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetBitsRate(void)
{
    MS_U32 u32Bitrate = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            return MDrv_MVD_GetBitsRate();

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG: // unnecessary
        case E_VDEC_DECODER_RVD:
        default:
            u32Bitrate = 0;
            break;
    }

    return u32Bitrate;
}

//------------------------------------------------------------------------------
/// Get 3:2 pull down flag which decoder retrieved from stream
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_Is32PullDown(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_BOOL bRet = FALSE;

    IO_arg.pRet = (void *)&bRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_32PULLDOWN, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    else
    {
        return bRet;
    }
}



//------------------------------------------------------------------------------
/// Get 3:2 pull down flag which decoder retrieved from stream
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_V2_Is32PullDown(void)
{
    MS_BOOL bIs32PullDown = 0;

    if (!_Attr.bInit)
    {
        return FALSE;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            return MDrv_MVD_GetIs32PullDown();

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            bIs32PullDown = FALSE;
            break;
    }

    return bIs32PullDown;
}

//------------------------------------------------------------------------------
/// Check whether the status of decoder is running or not.
/// @return VDEC_Result: E_VDEC_OK/E_VDEC_RET_NOT_RUNNING/E_VDEC_RET_NOT_INIT
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsAlive(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_ALIVE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Check whether the status of decoder is running or not.
/// @return VDEC_Result: E_VDEC_OK/E_VDEC_RET_NOT_RUNNING/E_VDEC_RET_NOT_INIT
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsAlive(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;
        case E_VDEC_DECODER_HVD:
            if(E_HVD_RET_NOT_RUNNING == MDrv_HVD_IsAlive())
            {
                return E_VDEC_RET_NOT_RUNNING;
            }
            break;
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Get the value of PTS - STC for video
/// @return the value of PTS - STC for video
//------------------------------------------------------------------------------
MS_S64 MApi_VDEC_GetVideoPtsStcDelta(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_S64 s64Ret = 0;

    IO_arg.pRet = (void *)&s64Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_VIDEO_PTS_STC_DELTA, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return s64Ret;
    }

}

//------------------------------------------------------------------------------
/// Get the value of PTS - STC for video
/// @return the value of PTS - STC for video
//------------------------------------------------------------------------------
MS_S64 MApi_VDEC_V2_GetVideoPtsStcDelta(void)
{
    MS_S64 s64Pts = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            s64Pts = MDrv_MVD_GetPtsStcDiff();
            //printf("s64Pts=%lld, s32=%ld\n", s64Pts, MDrv_MVD_GetPtsStcDiff());
            break;

        case E_VDEC_DECODER_HVD:
            s64Pts = MDrv_HVD_GetPtsStcDiff();


            break;

        case E_VDEC_DECODER_MJPEG:


            break;

        case E_VDEC_DECODER_RVD:


            break;

        default:
            s64Pts = 0;
            break;
    }

    return s64Pts;
}

//------------------------------------------------------------------------------
/// Check if decoder got valid stream to decode
/// @return TRUE or FALSE
///     - TRUE: find valid stream
///     - FALSE: not yet
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsWithValidStream(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_WITH_VALID_STREAM, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}


//------------------------------------------------------------------------------
/// Check if decoder got valid stream to decode
/// @return TRUE or FALSE
///     - TRUE: find valid stream
///     - FALSE: not yet
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsWithValidStream(void)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (MDrv_MVD_GetValidStreamFlag())
            {
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_FAIL;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Check if decoder found I frame after reset or flush queue
/// @return VDEC_Result
///     - E_VDEC_OK: found
///     - E_VDEC_FAIL: not found
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsIFrameFound(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_I_FRAME_FOUND, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Check if decoder found I frame after reset or flush queue
/// @return VDEC_Result
///     - E_VDEC_OK: found
///     - E_VDEC_FAIL: not found
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsIFrameFound(void)
{
    VDEC_Result ret = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_GetIsIPicFound());
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsIFrmFound());
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret, MApi_MJPEG_IsIFrameFound());
            break;

        case E_VDEC_DECODER_RVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_RVD_IsIFrameFound());
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Check if input stream is with low delay flag
/// @return VDEC_Result
///     - E_VDEC_OK: with low delay
///     - E_VDEC_FAIL: without low delay
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsWithLowDelay(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_WITH_LOW_DELAY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Check if input stream is with low delay flag
/// @return VDEC_Result
///     - E_VDEC_OK: with low delay
///     - E_VDEC_FAIL: without low delay
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsWithLowDelay(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_GetLowDelayFlag());
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsLowDelay());
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_VDEC_RESULT(ret, MApi_MJPEG_IsWithLowDelay());
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_FAIL;
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Check if all date buffers are empty or not
/// @return VDEC_Result
///     - E_VDEC_OK: All of the buffers are empty
///     - E_VDEC_FAIL: Some of the buffers are not empty
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsAllBufferEmpty(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_ALL_BUFFER_EMPTY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }

}


//------------------------------------------------------------------------------
/// Check if all date buffers are empty or not
/// @return VDEC_Result
///     - E_VDEC_OK: All of the buffers are empty
///     - E_VDEC_FAIL: Some of the buffers are not empty
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsAllBufferEmpty(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_IsAllBufferEmpty());
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(  ret  ,    MDrv_HVD_IsAllBufferEmpty() );
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_FAIL;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_FAIL;
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Get picture counter of current GOP
/// @return picture counter
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_GetGOPCnt(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U8 u8Ret = 0;

    IO_arg.pRet = (void *)&u8Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_GOP_CNT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u8Ret;
    }
}

//------------------------------------------------------------------------------
/// Get picture counter of current GOP
/// @return picture counter
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_V2_GetGOPCnt(void)
{
    MS_U8 u8Cnt = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u8Cnt = MDrv_MVD_GetGOPCount();
            break;

        case E_VDEC_DECODER_HVD:
            // not supported in HVD
            break;

        case E_VDEC_DECODER_MJPEG:
            // not supported in MJPEG
            break;

        case E_VDEC_DECODER_RVD:
            break;

        default:
            break;
    }

    return u8Cnt;
}

//-----------------------------------------------------------------------------
/// Get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetFrameCnt(void)
{


    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.pRet = (void *)&u32Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_FRAME_CNT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ret;
    }

}


//-----------------------------------------------------------------------------
/// Get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetFrameCnt(void)
{
    MS_U32 u32Cnt = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Cnt = MDrv_MVD_GetPicCounter() + MDrv_MVD_GetSkipPicCounter();

            break;

        case E_VDEC_DECODER_HVD:
            u32Cnt = MDrv_HVD_GetDecodeCnt() + MDrv_HVD_GetData(E_HVD_GDATA_TYPE_SKIP_CNT);

            break;

        case E_VDEC_DECODER_MJPEG:
            u32Cnt = MApi_MJPEG_GetFrameCnt();

            break;

        case E_VDEC_DECODER_RVD:
            u32Cnt = MDrv_RVD_GetFrameCnt() + MDrv_RVD_GetSkipCnt();

            break;

        default:
            u32Cnt = 0;
            break;
    }

    return u32Cnt;
}


//------------------------------------------------------------------------------
/// Get skipped counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetSkipCnt(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.pRet = (void *)&u32Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_SKIP_CNT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ret;
    }
}

//------------------------------------------------------------------------------
/// Get skipped counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetSkipCnt(void)
{
    MS_U32 u32Cnt = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Cnt = MDrv_MVD_GetSkipPicCounter();
            break;

        case E_VDEC_DECODER_HVD:
            u32Cnt = MDrv_HVD_GetData(E_HVD_GDATA_TYPE_SKIP_CNT);
            break;

        case E_VDEC_DECODER_MJPEG:
            break;

        case E_VDEC_DECODER_RVD:
            u32Cnt = MDrv_RVD_GetSkipCnt();
            break;

        default:
            break;
    }

    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get dropped frame counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetDropCnt(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.pRet = (void *)&u32Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DROP_CNT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0;
    }
    else
    {
        return u32Ret;
    }
}


//------------------------------------------------------------------------------
/// Get dropped frame counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_V2_GetDropCnt(void)
{
    MS_U32 u32Cnt = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Cnt = 0;
            break;

        case E_VDEC_DECODER_HVD:
            u32Cnt = MDrv_HVD_GetData(E_HVD_GDATA_TYPE_DROP_CNT);
            break;

        case E_VDEC_DECODER_MJPEG:
            break;

        case E_VDEC_DECODER_RVD:
            u32Cnt = MDrv_RVD_GetDropCnt();
            break;

        default:
            break;
    }

    return u32Cnt;
}


//------------------------------------------------------------------------------
/// Get display information
/// @param pDispinfo \b OUT : pointer to display setting information
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDispInfo(VDEC_DispInfo *pDispinfo)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)(pDispinfo);
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DISP_INFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Get display information
/// @param pDispinfo \b OUT : pointer to display setting information
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_GetDispInfo(VDEC_DispInfo *pDispinfo)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(MVD_FrameInfo));

            MDrv_MVD_GetFrameInfo(&info);

            if ((0xFFFF == info.u16HorSize) &&
                (0xFFFF == info.u16VerSize))
            {
                return E_VDEC_FAIL;
            }

            pDispinfo->u16HorSize   = info.u16HorSize;
            pDispinfo->u16VerSize   = info.u16VerSize;
            pDispinfo->u32FrameRate = info.u32FrameRate;
            pDispinfo->u8AspectRate = info.u8AspectRate;
            pDispinfo->u8Interlace  = info.u8Interlace;
            pDispinfo->u8AFD        = info.u8AFD;
            ///VDEC SAR actually means PAR(Pixel Aspect Ratio)
            pDispinfo->u16SarWidth  = info.u16par_width;
            pDispinfo->u16SarHeight = info.u16par_height;
            if ((pDispinfo->u16SarWidth > 1) && (pDispinfo->u16SarHeight > 1)
            && (pDispinfo->u16HorSize > 1 ) && (pDispinfo->u16VerSize > 1))
            {
                pDispinfo->u32AspectWidth = (MS_U32)pDispinfo->u16SarWidth * (MS_U32)pDispinfo->u16HorSize;
                pDispinfo->u32AspectHeight = (MS_U32)pDispinfo->u16SarHeight* (MS_U32)pDispinfo->u16VerSize;
            }
            else
            {
                pDispinfo->u32AspectWidth = pDispinfo->u16HorSize;
                pDispinfo->u32AspectHeight = pDispinfo->u16VerSize;
            }
            pDispinfo->u16CropRight = info.u16CropRight;
            pDispinfo->u16CropLeft  = info.u16CropLeft;
            pDispinfo->u16CropBottom = info.u16CropBottom;
            pDispinfo->u16CropTop   = info.u16CropTop;
            pDispinfo->u16Pitch     = info.u16Pitch;
            pDispinfo->u16PTSInterval = info.u16PTSInterval;
            pDispinfo->u8MPEG1      = info.u8MPEG1;
            pDispinfo->u8PlayMode   = info.u8PlayMode;
            pDispinfo->u8FrcMode    = info.u8FrcMode;
            pDispinfo->bWithChroma  = TRUE;
            pDispinfo->bEnableMIUSel = info.bEnableMIUSel;
            pDispinfo->u32DynScalingAddr = info.u32DynScalingAddr;
            pDispinfo->u8DynScalingDepth = info.u8DynScalingDepth;
            pDispinfo->u32DynScalingSize = info.u32DynScalingBufSize;

            if (MDrv_MVD_GetVideoRange() == 1)
            {
                pDispinfo->bColorInXVYCC = TRUE;
            }
            else
            {
                pDispinfo->bColorInXVYCC = FALSE;
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            HVD_Disp_Info info;

            _VDEC_Memset(&info, 0, sizeof(HVD_Disp_Info));

            _HVD_RET_HANDLE(MDrv_HVD_GetDispInfo(&info));

            pDispinfo->u16HorSize   = info.u16HorSize;
            pDispinfo->u16VerSize   = info.u16VerSize;
            pDispinfo->u32FrameRate = info.u32FrameRate;
            pDispinfo->u8AspectRate = info.u8AspectRate;
            pDispinfo->u8Interlace  = info.u8Interlace;
            pDispinfo->u8AFD        = info.u8AFD;
            pDispinfo->u16SarWidth  = info.u16SarWidth;
            pDispinfo->u16SarHeight = info.u16SarHeight;
            if(  (pDispinfo->u16SarWidth >= 1) &&  (pDispinfo->u16SarHeight >= 1)  &&
                  (pDispinfo->u16HorSize > 1 )  &&  (pDispinfo->u16VerSize > 1) &&
                  (( info.u16CropRight + info.u16CropLeft) < pDispinfo->u16HorSize) &&
                  ((info.u16CropTop + info.u16CropBottom) <pDispinfo->u16VerSize))
            {
                pDispinfo->u32AspectWidth = (MS_U32)pDispinfo->u16SarWidth * (MS_U32)(pDispinfo->u16HorSize- ( info.u16CropRight + info.u16CropLeft));
                pDispinfo->u32AspectHeight = (MS_U32)pDispinfo->u16SarHeight * (MS_U32)(pDispinfo->u16VerSize - ( info.u16CropTop + info.u16CropBottom));
            }
            else
            {
                pDispinfo->u32AspectWidth = pDispinfo->u16HorSize;
                pDispinfo->u32AspectHeight = pDispinfo->u16VerSize;
            }
            pDispinfo->u16CropRight = info.u16CropRight;
            pDispinfo->u16CropLeft  = info.u16CropLeft;
            pDispinfo->u16CropBottom = info.u16CropBottom;
            pDispinfo->u16CropTop   = info.u16CropTop;
            pDispinfo->u16Pitch = info.u16Pitch;
            pDispinfo->bWithChroma  = !(info.bChroma_idc_Mono);
            pDispinfo->bColorInXVYCC = TRUE;
            pDispinfo->bEnableMIUSel = MDrv_HVD_GetDynamicScalingInfo(E_HVD_DS_BUF_MIUSEL);
            pDispinfo->u32DynScalingAddr= MDrv_HVD_GetDynamicScalingInfo(E_HVD_DS_BUF_ADDR);
            pDispinfo->u8DynScalingDepth= MDrv_HVD_GetDynamicScalingInfo(E_HVD_DS_VECTOR_DEPTH);
            pDispinfo->u32DynScalingSize= MDrv_HVD_GetDynamicScalingInfo(E_HVD_DS_BUF_SIZE);
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            MJPEG_DISP_INFO info;

            _VDEC_Memset(&info, 0, sizeof(MJPEG_DISP_INFO));

            _MJPEG_RET_HANDLE(MApi_MJPEG_GetDispInfo(&info));

            pDispinfo->u16HorSize = info.u16Width;
            pDispinfo->u16VerSize = info.u16Height;
            pDispinfo->u32FrameRate = info.u32FrameRate;
            pDispinfo->u8Interlace  = 0; // no interlace
            pDispinfo->u8AFD = 0; // not used
            pDispinfo->u16SarWidth = 1;
            pDispinfo->u16SarHeight = 1;
            pDispinfo->u32AspectWidth = pDispinfo->u16HorSize;
            pDispinfo->u32AspectHeight = pDispinfo->u16VerSize;
            pDispinfo->u16CropRight = info.u16CropRight;
            pDispinfo->u16CropLeft  = info.u16CropLeft;
            pDispinfo->u16CropBottom = info.u16CropBottom;
            pDispinfo->u16CropTop   = info.u16CropTop;
            pDispinfo->u16Pitch = info.u16Pitch;
            pDispinfo->u16PTSInterval = 0; //not used
            pDispinfo->u8MPEG1 = 0; // not used
            pDispinfo->u8PlayMode = 0; // not used
            pDispinfo->u8FrcMode = 0; // not used
            pDispinfo->u8AspectRate = 0; // not used
            pDispinfo->bWithChroma = TRUE;
            pDispinfo->bColorInXVYCC = TRUE;
            pDispinfo->u32DynScalingAddr = 0; // not used
            pDispinfo->u8DynScalingDepth = 0; // not used
            pDispinfo->u32DynScalingSize = 0; // not used
            pDispinfo->bEnableMIUSel = FALSE; // not used
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            RVD_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(RVD_FrameInfo));

            _RVD_RET_HANDLE(MDrv_RVD_GetDispInfo(&info));

            pDispinfo->u16HorSize   = info.u16HorSize;
            pDispinfo->u16VerSize   = info.u16VerSize;
            pDispinfo->u32FrameRate = info.u16FrameRate;
            pDispinfo->u16SarWidth  = info.u16Sar_width;
            pDispinfo->u16SarHeight = info.u16Sar_height;
            pDispinfo->u16CropRight = info.u16CropRight;
            pDispinfo->u16CropLeft  = info.u16CropLeft;
            pDispinfo->u16CropBottom = info.u16CropBottom;
            pDispinfo->u16CropTop   = info.u16CropTop;
            pDispinfo->bWithChroma  = TRUE;
            pDispinfo->bColorInXVYCC = TRUE;
            pDispinfo->u8Interlace  = 0;
            pDispinfo->bEnableMIUSel = info.bEnableMIUSel;
            pDispinfo->u32DynScalingAddr= info.u32DynScalingAddr;
            pDispinfo->u8DynScalingDepth= info.u8DynScalingDepth;
            pDispinfo->u32DynScalingSize= (MS_U32)pDispinfo->u8DynScalingDepth*16;
            pDispinfo->u32AspectHeight = info.u32AspectHeight;
            pDispinfo->u32AspectWidth = info.u32AspectWidth;
            break;
        }
        default:
            break;
    }

    return E_VDEC_OK;
}

//------------------------------------------------------------------------------
/// Get current trick decode mode of decoder
/// @return VDEC_TrickDec
//------------------------------------------------------------------------------
VDEC_TrickDec MApi_VDEC_GetTrickMode(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_TRICK_DEC_ALL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_TrickDec eRet = E_VDEC_TRICK_DEC_ALL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_TRICK_MODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_TRICK_DEC_ALL;
    }
    else
    {
        return eRet;
    }

}


//------------------------------------------------------------------------------
/// Get current trick decode mode of decoder
/// @return VDEC_TrickDec
//------------------------------------------------------------------------------
VDEC_TrickDec MApi_VDEC_V2_GetTrickMode(void)
{
    VDEC_TrickDec eTrick = E_VDEC_TRICK_DEC_ALL;

    if (!_Attr.bInit)
    {
        // FIXME
        return eTrick;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_TrickDec dec = MDrv_MVD_GetTrickMode();

            if (E_MVD_TRICK_DEC_ALL == dec)
            {
                return E_VDEC_TRICK_DEC_ALL;
            }
            else if (E_MVD_TRICK_DEC_IP == dec)
            {
                return E_VDEC_TRICK_DEC_IP;
            }
            else if (E_MVD_TRICK_DEC_I == dec)
            {
                return E_VDEC_TRICK_DEC_I;
            }
            else
            {
                return eTrick;
            }

            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            HVD_Skip_Decode dec = (HVD_Skip_Decode) MDrv_HVD_GetPlayMode(E_HVD_GMODE_SKIP_MODE);

            if (E_HVD_SKIP_DECODE_ALL == dec)
            {
                return E_VDEC_TRICK_DEC_ALL;
            }
            else if (E_HVD_SKIP_DECODE_I == dec)
            {
                return E_VDEC_TRICK_DEC_I;
            }
            else if (E_HVD_SKIP_DECODE_IP == dec)
            {
                return E_VDEC_TRICK_DEC_IP;
            }
            else
            {
                return eTrick;
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
            break;

        case E_VDEC_DECODER_RVD:
        {
            RVD_TrickMode dec = (RVD_TrickMode) MDrv_RVD_GetTrickMode();

            if (E_RVD_TRICKMODE_DECODE_ALL == dec)
            {
                return E_VDEC_TRICK_DEC_ALL;
            }
            else if (E_RVD_TRICKMODE_DECODE_IP == dec)
            {
                return E_VDEC_TRICK_DEC_IP;
            }
            else if (E_RVD_TRICKMODE_DECODE_I == dec)
            {
                return E_VDEC_TRICK_DEC_I;
            }
            else
            {
                return eTrick;
            }

            break;
        }
        default:
            break;
    }

    return eTrick;
}

//------------------------------------------------------------------------------
/// Get stream active format which decoder retrieved
/// @return active format ID
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_GetActiveFormat(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U8 u8Ret = 0xFF;

    IO_arg.pRet = (void *)&u8Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_ACTIVE_FORMAT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0xFF;
    }
    else
    {
        return u8Ret;
    }
}


//------------------------------------------------------------------------------
/// Get stream active format which decoder retrieved
/// @return active format ID
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_V2_GetActiveFormat(void)
{
    MS_U8 u8ActFmt;

    if (!_Attr.bInit)
    {
        return 0xFF;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u8ActFmt = MDrv_MVD_GetActiveFormat();

            break;

        case E_VDEC_DECODER_HVD:
            u8ActFmt = MDrv_HVD_GetActiveFormat();

            break;

        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            u8ActFmt = 0xFF;
            break;
    }

    return u8ActFmt;
}


//------------------------------------------------------------------------------
/// Get stream colour primaries
/// @return stream colour primaries
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_GetColourPrimaries(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U8 u8Ret = 0xFF;

    IO_arg.pRet = (void *)&u8Ret;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_COLOUR_PRIMARIES, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return 0xFF;
    }
    else
    {
        return u8Ret;
    }
}

//------------------------------------------------------------------------------
/// Get stream colour primaries
/// @return stream colour primaries
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_V2_GetColourPrimaries(void)
{
    MS_U8 u8ColourPrimaries;

    if (!_Attr.bInit)
    {
        return 0xFF;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u8ColourPrimaries = MDrv_MVD_GetColorFormat();

            break;

        case E_VDEC_DECODER_HVD:
        {
            HVD_Disp_Info info;

            _VDEC_Memset(&info, 0, sizeof(HVD_Disp_Info));

            if (E_HVD_OK != MDrv_HVD_GetDispInfo(&info))
            {
                u8ColourPrimaries = 0xFF;
            }
            else
            {
                u8ColourPrimaries = info.u8ColourPrimaries;
            }

            break;
        }
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            u8ColourPrimaries = 0xFF;
            break;
    }

    return u8ColourPrimaries;
}

VDEC_Result MApi_VDEC_GetHWKey(MS_U8 *pu8Key)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pu8Key;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_HW_KEY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


VDEC_Result MApi_VDEC_V2_GetHWKey(MS_U8 *pu8Key)
{
    VDEC_Result retVal = E_VDEC_RET_UNSUPPORTED;
    UNUSED(pu8Key);

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Check if sequence header is different from previous one
/// @return VDEC_Result
///     - E_VDEC_OK: changed
///     - E_VDEC_FAIL: not changed
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsSeqChg(void)
{

    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_IS_SEQ_CHG, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Check if sequence header is different from previous one
/// @return VDEC_Result
///     - E_VDEC_OK: changed
///     - E_VDEC_FAIL: not changed
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_IsSeqChg(void)
{
    VDEC_Result ret;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_IsSeqChg());
            break;

        case E_VDEC_DECODER_HVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_HVD_IsDispInfoChg());
            break;

        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_FAIL;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_FAIL;
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}


//------------------------------------------------------------------------------
/// Set up debug message level
/// @param eDbgLevel \b IN : message level
/// @return VDEC_Result
///     - E_VDEC_OK: Success
///     - E_VDEC_FAIL: Failed
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetDbgLevel(VDEC_DbgLevel eDbgLevel)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eDbgLevel;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SET_DBG_LEVEL, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Set up debug message level
/// @param eDbgLevel \b IN : message level
/// @return VDEC_Result
///     - E_VDEC_OK: Success
///     - E_VDEC_FAIL: Failed
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_SetDbgLevel(VDEC_DbgLevel eDbgLevel)
{
    if(E_VDEC_DBG_LEVEL_FW == eDbgLevel)
    {
        MS_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0;
        _RET_VDEC_HVD_LINT_CHECK();
        if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
        {
            printf("VDEC HVD MApi_VDEC_SetDbgLevel Err: MMIO_GetBASE failure\n");
            return E_VDEC_FAIL;
        }
        else
        {
            //printf("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
            MDrv_HVD_SetOSRegBase(u32RiuBaseAdd);
        }
        MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_FW);
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _RET_VDEC_MVD_LINT_CHECK();
            MDrv_MVD_SetDbgLevel(eDbgLevel);

            break;

        case E_VDEC_DECODER_HVD:
            _RET_VDEC_HVD_LINT_CHECK();
            if (E_VDEC_DBG_LEVEL_NONE == eDbgLevel)
            {
                MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_NONE);
            }
            if (E_VDEC_DBG_LEVEL_ERR == eDbgLevel)
            {
                MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_ERR);
            }
            else if (E_VDEC_DBG_LEVEL_INFO == eDbgLevel)
            {
                MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_INFO);
            }
            else if (E_VDEC_DBG_LEVEL_DBG == eDbgLevel)
            {
                MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_DBG);
            }
            else if (E_VDEC_DBG_LEVEL_TRACE == eDbgLevel)
            {
                MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_TRACE);
            }
            else if (E_VDEC_DBG_LEVEL_FW == eDbgLevel)
            {
                MDrv_HVD_SetDbgLevel(E_HVD_UART_LEVEL_FW);
            }

            break;

        case E_VDEC_DECODER_MJPEG:
            MApi_MJPEG_DbgSetMsgLevel((MJPEG_DbgLevel)eDbgLevel);

            break;

        case E_VDEC_DECODER_RVD:
            _RET_VDEC_RVD_LINT_CHECK();
            if (E_VDEC_DBG_LEVEL_ERR == eDbgLevel)
            {
                MDrv_RVD_SetDbgLevel(E_RVD_DEBUG_ERR);
            }
            else if (E_VDEC_DBG_LEVEL_INFO == eDbgLevel)
            {
                MDrv_RVD_SetDbgLevel(E_RVD_DEBUG_INF);
            }
            else if (E_VDEC_DBG_LEVEL_DBG == eDbgLevel)
            {
                MDrv_RVD_SetDbgLevel(E_RVD_DEBUG_INF | E_RVD_DEBUG_ERR);
            }
            else if (E_VDEC_DBG_LEVEL_FW == eDbgLevel)
            {
                MDrv_RVD_SetDbgLevel(E_RVD_DEBUG_INF | E_RVD_DEBUG_ERR | E_RVD_DEBUG_DRV | E_RVD_DEBUG_HAL);
            }

            break;

        default:
            break;
    }

    return E_VDEC_OK;
}


//------------------------------------------------------------------------------
/// Get the information of the latest decoded frame.
/// @param pFrmInfo \b IN : the information of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDecFrameInfo(VDEC_FrameInfo* pFrmInfo)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pFrmInfo;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DEC_FRAME_INFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Get the information of the latest decoded frame.
/// @param pFrmInfo \b IN : the information of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_GetDecFrameInfo(VDEC_FrameInfo* pFrmInfo)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_FrmInfo stFrm;
            _VDEC_Memset(&stFrm, 0, sizeof(MVD_FrmInfo));
            _MVD_RET_HANDLE(MDrv_MVD_GetFrmInfo(E_MVD_FRMINFO_DECODE, &stFrm));
            pFrmInfo->eFrameType    = _VDEC_MapFrmType2MVD(stFrm.eFrmType);
            pFrmInfo->u16Height     = stFrm.u16Height;
            pFrmInfo->u16Width      = stFrm.u16Width;
            pFrmInfo->u16Pitch      = stFrm.u16Pitch;
            pFrmInfo->u32ChromaAddr = stFrm.u32ChromaAddr;
            pFrmInfo->u32ID_H       = stFrm.u32ID_H;
            pFrmInfo->u32ID_L       = stFrm.u32ID_L;
            pFrmInfo->u32LumaAddr   = stFrm.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = stFrm.u32TimeStamp;
            pFrmInfo->eFieldType=  E_VDEC_FIELDTYPE_BOTH;
            retVal = E_VDEC_OK;
            break;
        }

        case E_VDEC_DECODER_HVD:
        {
            HVD_Frame_Info info;

            _VDEC_Memset(&info, 0, sizeof(HVD_Frame_Info));

            _HVD_RET_HANDLE(MDrv_HVD_GetFrmInfo(E_HVD_GFRMINFO_DECODE, &info));

            pFrmInfo->eFrameType    = _VDEC_MapFrmType2HVD(info.eFrmType);
            pFrmInfo->u16Height     = info.u16Height;
            pFrmInfo->u16Width      = info.u16Width;
            pFrmInfo->u16Pitch      = info.u16Pitch;
            pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfo->u32ID_H       = info.u32ID_H;
            pFrmInfo->u32ID_L       = info.u32ID_L;
            pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pFrmInfo->eFieldType=  (VDEC_FieldType)info.eFieldType;

            retVal = E_VDEC_OK;

            break;
        }

        case E_VDEC_DECODER_MJPEG:
        {
            MJPEG_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(MJPEG_FrameInfo));

            _MJPEG_RET_HANDLE(MApi_MJPEG_GetDecFrameInfo(&info));

            pFrmInfo->eFrameType    = _VDEC_MapFrmType2MJPEG(info.eFrameType);
            pFrmInfo->u16Height     = info.u16Height;
            pFrmInfo->u16Width      = info.u16Width;
            pFrmInfo->u16Pitch      = info.u16Pitch;
            pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfo->u32ID_H       = info.u32ID_H;
            pFrmInfo->u32ID_L       = info.u32ID_L;
            pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pFrmInfo->eFieldType=  E_VDEC_FIELDTYPE_BOTH;

            retVal = E_VDEC_OK;

            break;
        }

        case E_VDEC_DECODER_RVD:
        {
            RVD_FrameBuff_Info info;

            _VDEC_Memset(&info, 0, sizeof(RVD_FrameBuff_Info));

            _RVD_RET_HANDLE(MDrv_RVD_GetFrameInfo(&info , 0));

            pFrmInfo->eFrameType    = (VDEC_FrameType) (info.u16FrmType);
            pFrmInfo->u16Height     = info.u16Height;
            pFrmInfo->u16Width      = info.u16Width;
            pFrmInfo->u16Pitch      = info.u16Pitch;
            pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfo->u32ID_H       = info.u32ID_H;
            pFrmInfo->u32ID_L       = info.u32ID_L;
            pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pFrmInfo->eFieldType=  E_VDEC_FIELDTYPE_BOTH;

            retVal = E_VDEC_OK;

            break;
        }
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}



//------------------------------------------------------------------------------
/// Get the information of current displaying frame.
/// @param pFrmInfo \b IN : the information of displaying frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDispFrameInfo(VDEC_FrameInfo *pFrmInfo)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pFrmInfo;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DISP_FRAME_INFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Get the information of current displaying frame.
/// @param pFrmInfo \b IN : the information of displaying frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_GetDispFrameInfo(VDEC_FrameInfo *pFrmInfo)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_FrmInfo stFrm;
            _VDEC_Memset(&stFrm, 0, sizeof(MVD_FrmInfo));
            _MVD_RET_HANDLE(MDrv_MVD_GetFrmInfo(E_MVD_FRMINFO_DISPLAY, &stFrm));
            pFrmInfo->eFrameType    = _VDEC_MapFrmType2MVD(stFrm.eFrmType);
            pFrmInfo->u16Height     = stFrm.u16Height;
            pFrmInfo->u16Width      = stFrm.u16Width;
            pFrmInfo->u16Pitch      = stFrm.u16Pitch;
            pFrmInfo->u32ChromaAddr = stFrm.u32ChromaAddr;
            pFrmInfo->u32ID_H       = stFrm.u32ID_H;
            pFrmInfo->u32ID_L       = stFrm.u32ID_L;
            pFrmInfo->u32LumaAddr   = stFrm.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = stFrm.u32TimeStamp;
            pFrmInfo->eFieldType=  E_VDEC_FIELDTYPE_BOTH;
            retVal = E_VDEC_OK;
            break;
        }

        case E_VDEC_DECODER_HVD:
        {
            HVD_Frame_Info info;

            _VDEC_Memset(&info, 0, sizeof(HVD_Frame_Info));

            _HVD_RET_HANDLE(MDrv_HVD_GetFrmInfo(E_HVD_GFRMINFO_DISPLAY, &info));

            pFrmInfo->eFrameType    = _VDEC_MapFrmType2HVD(info.eFrmType);
            pFrmInfo->u16Height     = info.u16Height;
            pFrmInfo->u16Width      = info.u16Width;
            pFrmInfo->u16Pitch      = info.u16Pitch;
            pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfo->u32ID_H       = info.u32ID_H;
            pFrmInfo->u32ID_L       = info.u32ID_L;
            pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pFrmInfo->eFieldType =  (VDEC_FieldType)info.eFieldType;
            retVal = E_VDEC_OK;

            break;
        }

        case E_VDEC_DECODER_MJPEG:
        {
            MJPEG_FrameInfo info;

            _VDEC_Memset(&info, 0, sizeof(MJPEG_FrameInfo));

            _MJPEG_RET_HANDLE(MApi_MJPEG_GetDispFrameInfo(&info));

            pFrmInfo->eFrameType    = _VDEC_MapFrmType2MJPEG(info.eFrameType);
            pFrmInfo->u16Height     = info.u16Height;
            pFrmInfo->u16Width      = info.u16Width;
            pFrmInfo->u16Pitch      = info.u16Pitch;
            pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfo->u32ID_H       = info.u32ID_H;
            pFrmInfo->u32ID_L       = info.u32ID_L;
            pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pFrmInfo->eFieldType=  E_VDEC_FIELDTYPE_BOTH;
            retVal = E_VDEC_OK;

            break;
        }

        case E_VDEC_DECODER_RVD:
        {
            RVD_FrameBuff_Info info;

            _VDEC_Memset(&info, 0, sizeof(RVD_FrameBuff_Info));

            _RVD_RET_HANDLE(MDrv_RVD_GetFrameInfo(&info , 1));

            pFrmInfo->eFrameType    = (VDEC_FrameType) (info.u16FrmType);
            pFrmInfo->u16Height     = info.u16Height;
            pFrmInfo->u16Width      = info.u16Width;
            pFrmInfo->u16Pitch      = info.u16Pitch;
            pFrmInfo->u32ChromaAddr = info.u32ChromaAddr;
            pFrmInfo->u32ID_H       = info.u32ID_H;
            pFrmInfo->u32ID_L       = info.u32ID_L;
            pFrmInfo->u32LumaAddr   = info.u32LumaAddr;
            pFrmInfo->u32TimeStamp  = info.u32TimeStamp;
            pFrmInfo->eFieldType=  E_VDEC_FIELDTYPE_BOTH;
            retVal = E_VDEC_OK;

            break;
        }
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }


    return retVal;
}

//------------------------------------------------------------------------------
/// Get the extension information of decoded frame.
/// @param pExtDispinfo \b IN : the extension information of decoded frame.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetExtDispInfo(VDEC_ExtDispInfo *pExtDispinfo)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pExtDispinfo;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_EXT_DISP_INFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Get the extension information of decoded frame.
/// @param pExtDispinfo \b IN : the extension information of decoded frame.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_GetExtDispInfo(VDEC_ExtDispInfo *pExtDispinfo)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_ExtDispInfo stExtDispInfo;
            _VDEC_Memset(&stExtDispInfo, 0, sizeof(MVD_ExtDispInfo));
            _MVD_RET_HANDLE(MDrv_MVD_GetExtDispInfo(&stExtDispInfo));

            pExtDispinfo->u16VSize   = stExtDispInfo.u16VSize;
            pExtDispinfo->u16HSize   = stExtDispInfo.u16HSize;
            pExtDispinfo->s16VOffset = (MS_S16)stExtDispInfo.u16VOffset;
            pExtDispinfo->s16HOffset = (MS_S16)stExtDispInfo.u16HOffset;
            retVal = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }
    return retVal;
}


//------------------------------------------------------------------------------
/// Get the time_code of the latest decoded frame.
/// @param pTimeCode \b IN : the time_code of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDecTimeCode(VDEC_TimeCode* pTimeCode)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pTimeCode;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DEC_TIME_CODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest decoded frame.
/// @param pTimeCode \b IN : the time_code of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_GetDecTimeCode(VDEC_TimeCode* pTimeCode)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_TimeCode stTimeCode;
            _VDEC_Memset(&stTimeCode, 0, sizeof(MVD_TimeCode));
            _MVD_RET_HANDLE(MDrv_MVD_GetTimeCode(E_MVD_FRMINFO_DECODE, &stTimeCode));

            pTimeCode->u8TimeCodeHr  = stTimeCode.u8TimeCodeHr ;
            pTimeCode->u8TimeCodeMin = stTimeCode.u8TimeCodeMin;
            pTimeCode->u8TimeCodeSec = stTimeCode.u8TimeCodeSec;
            pTimeCode->u8TimeCodePic = stTimeCode.u8TimeCodePic;
            pTimeCode->u8DropFrmFlag = stTimeCode.u8DropFrmFlag;

            retVal = E_VDEC_OK;
            break;
        }

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest displayed frame.
/// @param pTimeCode \b IN : the time_code of the latest displayed frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDispTimeCode(VDEC_TimeCode* pTimeCode)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pTimeCode;
    IO_arg.pRet = (void *)&eRet;


    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GET_DISP_TIME_CODE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}


//------------------------------------------------------------------------------
/// Get the time_code of the latest displayed frame.
/// @param pTimeCode \b IN : the time_code of the latest displayed frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_V2_GetDispTimeCode(VDEC_TimeCode* pTimeCode)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_TimeCode stTimeCode;
            _VDEC_Memset(&stTimeCode, 0, sizeof(MVD_TimeCode));
            _MVD_RET_HANDLE(MDrv_MVD_GetTimeCode(E_MVD_FRMINFO_DISPLAY, &stTimeCode));

            pTimeCode->u8TimeCodeHr  = stTimeCode.u8TimeCodeHr ;
            pTimeCode->u8TimeCodeMin = stTimeCode.u8TimeCodeMin;
            pTimeCode->u8TimeCodeSec = stTimeCode.u8TimeCodeSec;
            pTimeCode->u8TimeCodePic = stTimeCode.u8TimeCodePic;
            pTimeCode->u8DropFrmFlag = stTimeCode.u8DropFrmFlag;

            retVal = E_VDEC_OK;
            break;
        }

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}


//------------------------------------------------------------------------------
/// Start CC data parsing.
/// @param pCCParam \b IN : CC configuration
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_StartParsing(VDEC_CCCfg* pCCParam)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)(pCCParam);
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_STARTPARSING, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }

}

VDEC_Result MApi_VDEC_V2_CC_StartParsing(VDEC_CCCfg* pCCParam)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_CCStartParsing((MVD_CCCfg*)pCCParam);

                if(ret == E_MVD_RET_OK)
                {
                    _Attr.eCCFormat = pCCParam->eFormat;
                    retVal = E_VDEC_OK;
                }
                else
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Stop CC data parsing.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_StopParsing(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
         return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_STOPPARSING, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }

}

VDEC_Result MApi_VDEC_V2_CC_StopParsing(void)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_CCStopParsing((MVD_CCFormat)_Attr.eCCFormat);

                if(ret == E_MVD_RET_OK)
                {
                    retVal = E_VDEC_OK;
                }
                else
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Get write pointer of CC data buffer.
/// @param pu32Write \b OUT : current write pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_GetWritePtr(MS_U32* pu32Write)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
         return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pu32Write;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETWRITEPTR, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }

}

VDEC_Result MApi_VDEC_V2_CC_GetWritePtr(MS_U32* pu32Write)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_CCGetWritePtr((MVD_CCFormat)_Attr.eCCFormat, pu32Write);

                if(ret == E_MVD_RET_OK)
                {
                    retVal = E_VDEC_OK;
                }
                else
                {
                    retVal = E_VDEC_FAIL;
                }
            }

            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Get the read pointer of CC data buffer.
/// @param pu32Read \b OUT : current read pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_GetReadPtr(MS_U32* pu32Read)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
         return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pu32Read;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETREADPTR, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_GetReadPtr(MS_U32* pu32Read)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_CCGetReadPtr((MVD_CCFormat)_Attr.eCCFormat, pu32Read);

                if(ret == E_MVD_RET_OK)
                {
                    retVal = E_VDEC_OK;
                }
                else
                {
                    retVal = E_VDEC_FAIL;
                }
            }
            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Update the read pointer of CC data buffer.
/// @param u32EachPacketSize \b IN : new read pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_UpdateReadPtr(MS_U32 u32EachPacketSize)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
         return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&u32EachPacketSize;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_UPDATEREADPTR, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_UpdateReadPtr(MS_U32 u32EachPacketSize)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_CCUpdateReadPtr((MVD_CCFormat)_Attr.eCCFormat, u32EachPacketSize);

                if(ret == E_MVD_RET_OK)
                {
                    retVal = E_VDEC_OK;
                }
                else
                {
                    retVal = E_VDEC_FAIL;
                }
            }
            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Get if CC data buffer is overflow.
/// @param pbOverflow \b OUT : overflow flag
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_GetIsOverflow(MS_BOOL* pbOverflow)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
         return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pbOverflow;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETISOVERFLOW, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_GetIsOverflow(MS_BOOL* pbOverflow)
{
    VDEC_Result retVal;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                E_MVD_Result ret = MDrv_MVD_CCGetIsOverflow((MVD_CCFormat)_Attr.eCCFormat, pbOverflow);

                if(ret == E_MVD_RET_OK)
                {
                    retVal = E_VDEC_OK;
                }
                else
                {
                    retVal = E_VDEC_FAIL;
                }
            }
            break;

        case E_VDEC_DECODER_HVD:
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_SetEvent(MS_U32 u32EnableEvent, VDEC_EventCb pfn, void *param)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&u32EnableEvent;
    IO_arg.param[1] = (void *)&pfn;
    IO_arg.param[2] = (void *)param;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SETEVENT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_SetEvent(MS_U32 u32EnableEvent, VDEC_EventCb pfn, void *param)
{
    VDEC_Result retVal = E_VDEC_FAIL;
    MS_U32 i = 0;

    _VDEC_MutexLock();

    if (!_Attr.bInit)
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_NOT_INIT;
    }

    if ((u32EnableEvent != E_VDEC_EVENT_OFF) && (pfn == NULL))
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_INVALID_PARAM;
    }

    _Attr.VdecEventInfo[E_VDEC_CB_MAIN].u32EventBitMap = u32EnableEvent;
    _Attr.VdecEventInfo[E_VDEC_CB_MAIN].pVdecEventCb = pfn;
    _Attr.VdecEventInfo[E_VDEC_CB_MAIN].pParam = param;

#if defined(VDEC_ISR_MONITOR)
    _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
#else
    _u32VdecIsrEventFlag = 0; //reset event flag;
#endif
    for(i = 0; i < E_VDEC_CB_MAX_NUM; i++)
    {
        _u32VdecIsrEventFlag |= _Attr.VdecEventInfo[i].u32EventBitMap;
    }

#if defined(VDEC_ISR_MONITOR)
    MsOS_SetEvent(_s32VdecEventId, E_VDEC_EVENT_ISR_EVENT_CHANGE);
#endif

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MS_U32 u32Event = 0;

            //Set Event

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_ONE)
            {
                u32Event |= E_MVD_EVENT_DISP_VSYNC;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                u32Event |= E_MVD_EVENT_DISP_RDY;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_SEQ_HDR_FOUND)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
#if defined(MVD_SUPPORT_X4_CC)
                //subscribe event "CC data found in display order"
                u32Event |= E_MVD_EVENT_USER_DATA_DISP;
#else
                u32Event |= E_MVD_EVENT_USER_DATA;
#endif
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                u32Event |= E_MVD_EVENT_USER_DATA;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                u32Event |= E_MVD_EVENT_FIRST_FRAME;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_PIC_FOUND)
            {
                u32Event |= E_MVD_EVENT_PIC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                u32Event |= E_MVD_EVENT_UNMUTE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                u32Event |= E_MVD_EVENT_DEC_I;
            }

            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexUnlock();
            }
            if(E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Event,(MVD_InterruptCb)_VDEC_IsrProc))
            {
                retVal = E_VDEC_FAIL;
            }
            else
            {
                retVal = E_VDEC_OK;
            }
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexLock();
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            MS_U32 eEvent = E_HVD_ISR_NONE;

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_ONE)
            {
                eEvent |= E_HVD_ISR_DISP_ONE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_REPEAT)
            {
                eEvent |= E_HVD_ISR_DISP_REPEAT;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_WITH_CC)
            {
                eEvent |= E_HVD_ISR_DISP_WITH_CC;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ONE)
            {
                eEvent |= E_HVD_ISR_DEC_ONE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                eEvent |= E_HVD_ISR_DEC_I;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ERR)
            {
                eEvent |= E_HVD_ISR_DEC_HW_ERR;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                eEvent |= E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
                eEvent |= E_HVD_ISR_DEC_CC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                eEvent |= E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                eEvent |= E_HVD_ISR_DEC_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                eEvent |= E_HVD_ISR_DISP_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                eEvent |= E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (eEvent != E_HVD_ISR_NONE)
            {
                if(E_HVD_OK != MDrv_HVD_SetISREvent(eEvent, (HVD_InterruptCb) _VDEC_IsrProc))
                {
                    _VDEC_MutexUnlock();
                    return E_VDEC_FAIL;
                }
            }
            retVal = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
#if 1
            if (_u32VdecIsrEventFlag == E_VDEC_EVENT_OFF)
            {
                MDrv_RVD_SetISREvent(E_RVD_ISR_NONE,
                                     (RVD_InterruptCb) _VDEC_IsrProc);
            }
            retVal = E_VDEC_OK;

#else
            retVal = E_VDEC_RET_UNSUPPORTED;
#endif
            break;
        }
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    _VDEC_MutexUnlock();

    return retVal;
}

VDEC_Result MApi_VDEC_UnsetEvent(MS_U32 u32EnableEvent)
{
      if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&u32EnableEvent;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_UNSETEVENT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_UnsetEvent(MS_U32 u32EnableEvent)
{
    VDEC_Result retVal = E_VDEC_FAIL;
    MS_U32 i = 0;

    _VDEC_MutexLock();

    if (!_Attr.bInit)
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_NOT_INIT;
    }

    if(u32EnableEvent == E_VDEC_EVENT_OFF)
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_INVALID_PARAM;
    }

    _Attr.VdecEventInfo[E_VDEC_CB_MAIN].u32EventBitMap &= ~u32EnableEvent;

    if(E_VDEC_EVENT_OFF == _Attr.VdecEventInfo[E_VDEC_CB_MAIN].u32EventBitMap)
    {
        _Attr.VdecEventInfo[E_VDEC_CB_MAIN].pVdecEventCb = NULL;
        _Attr.VdecEventInfo[E_VDEC_CB_MAIN].pParam = NULL;
    }

#if defined(VDEC_ISR_MONITOR) //clear ISR Event flag ,
    _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE; //reset event flag;
#else
    _u32VdecIsrEventFlag = 0; //reset event flag;
#endif
    for(i = 0; i < E_VDEC_CB_MAX_NUM; i++)
    {
        _u32VdecIsrEventFlag |= _Attr.VdecEventInfo[i].u32EventBitMap;
    }

#if defined(VDEC_ISR_MONITOR)
    MsOS_SetEvent(_s32VdecEventId, E_VDEC_EVENT_ISR_EVENT_CHANGE);
#endif

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MS_U32 u32Event = 0;

            //Set Event
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                u32Event |= E_MVD_EVENT_DISP_RDY;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_SEQ_HDR_FOUND)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
#if defined(MVD_SUPPORT_X4_CC)
                u32Event |= E_MVD_EVENT_USER_DATA_DISP;
#else
                u32Event |= E_MVD_EVENT_USER_DATA;
#endif
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                u32Event |= E_MVD_EVENT_USER_DATA;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                u32Event |= E_MVD_EVENT_FIRST_FRAME;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_PIC_FOUND)
            {
                u32Event |= E_MVD_EVENT_PIC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                u32Event |= E_MVD_EVENT_UNMUTE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                u32Event |= E_MVD_EVENT_DEC_I;
            }


            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexUnlock();
            }
            if(E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Event, (MVD_InterruptCb) _VDEC_IsrProc))
            {
                retVal = E_VDEC_FAIL;
            }
            else
            {
                retVal = E_VDEC_OK;
            }
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexLock();
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            HVD_ISR_Event eEvent = E_HVD_ISR_NONE;

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_ONE)
            {
                eEvent|=E_HVD_ISR_DISP_ONE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_REPEAT)
            {
                eEvent|=E_HVD_ISR_DISP_REPEAT;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_WITH_CC)
            {
                eEvent|=E_HVD_ISR_DISP_WITH_CC;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ONE)
            {
                eEvent|=E_HVD_ISR_DEC_ONE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                eEvent|=E_HVD_ISR_DEC_I;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ERR)
            {
                eEvent|=E_HVD_ISR_DEC_HW_ERR;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
                eEvent|=E_HVD_ISR_DEC_CC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                eEvent|=E_HVD_ISR_DEC_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                eEvent|=E_HVD_ISR_DISP_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if( eEvent != E_HVD_ISR_NONE )
            {
                if(E_HVD_OK !=MDrv_HVD_SetISREvent(eEvent, (HVD_InterruptCb) _VDEC_IsrProc))
                {
                    _VDEC_MutexUnlock();
                    return E_VDEC_FAIL;
                }
            }
            retVal = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
#if 1
            if (_u32VdecIsrEventFlag == E_VDEC_EVENT_OFF)
            {
                MDrv_RVD_SetISREvent(E_RVD_ISR_NONE, (RVD_InterruptCb) _VDEC_IsrProc);
            }
            retVal = E_VDEC_OK;

#else
            retVal = E_VDEC_RET_UNSUPPORTED;
#endif
            break;
        }
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    _VDEC_MutexUnlock();

    return retVal;
}

//------------------------------------------------------------------------------
/// set ISR event CB
/// @param cb_type \b IN : The CB type for VDEC interrupt
/// @param u32EnableEvent \b IN : The event flag for user data CB function
/// @param pfn \b IN : user data CB function
/// @param param \b IN : the param of/for user data CB function
/// @return status
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent, VDEC_EventCb pfn, void* param)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&cb_type;
    IO_arg.param[1] = (void *)&u32EnableEvent;
    IO_arg.param[2] = (void *)&pfn;
    IO_arg.param[3] = (void *)param;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SETENVENT_EX, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_SetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent, VDEC_EventCb pfn, void* param)
{
    VDEC_Result retVal = E_VDEC_FAIL;
    MS_U32 i = 0;

    _VDEC_MutexLock();

    if (!_Attr.bInit)
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_NOT_INIT;
    }

    if( (u32EnableEvent != E_VDEC_EVENT_OFF)  && (pfn == NULL) )
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_INVALID_PARAM;
    }

    _Attr.VdecEventInfo[cb_type].u32EventBitMap = u32EnableEvent;
    _Attr.VdecEventInfo[cb_type].pVdecEventCb = pfn;
    _Attr.VdecEventInfo[cb_type].pParam = param;

#if defined(VDEC_ISR_MONITOR)
    _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE;
#else
    _u32VdecIsrEventFlag = 0; //reset event flag;
#endif
    for(i = 0; i < E_VDEC_CB_MAX_NUM; i++)
    {
        _u32VdecIsrEventFlag |= _Attr.VdecEventInfo[cb_type].u32EventBitMap;
    }

#if defined(VDEC_ISR_MONITOR)
    MsOS_SetEvent(_s32VdecEventId, E_VDEC_EVENT_ISR_EVENT_CHANGE);
#endif

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MS_U32 u32Event = 0;

            //Set Event
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                u32Event |= E_MVD_EVENT_DISP_RDY;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_SEQ_HDR_FOUND)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
#if defined(MVD_SUPPORT_X4_CC)
                u32Event |= E_MVD_EVENT_USER_DATA_DISP;
#else
                u32Event |= E_MVD_EVENT_USER_DATA;
#endif
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                u32Event |= E_MVD_EVENT_USER_DATA;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                u32Event |= E_MVD_EVENT_FIRST_FRAME;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_PIC_FOUND)
            {
                u32Event |= E_MVD_EVENT_PIC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                u32Event |= E_MVD_EVENT_UNMUTE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                u32Event |= E_MVD_EVENT_DEC_I;
            }

            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexUnlock();
            }
            if(E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Event,(MVD_InterruptCb)_VDEC_IsrProc))
            {
                retVal =  E_VDEC_FAIL;
            }
            else
            {
                retVal = E_VDEC_OK;
            }
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexLock();
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            MS_U32 eEvent = E_HVD_ISR_NONE;

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_ONE)
            {
                eEvent|=E_HVD_ISR_DISP_ONE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_REPEAT)
            {
                eEvent|=E_HVD_ISR_DISP_REPEAT;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_WITH_CC)
            {
                eEvent|=E_HVD_ISR_DISP_WITH_CC;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ONE)
            {
                eEvent|=E_HVD_ISR_DEC_ONE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                eEvent|=E_HVD_ISR_DEC_I;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ERR)
            {
                eEvent|=E_HVD_ISR_DEC_HW_ERR;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
                eEvent|=E_HVD_ISR_DEC_CC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                eEvent|=E_HVD_ISR_DEC_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                eEvent|=E_HVD_ISR_DISP_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if( eEvent != E_HVD_ISR_NONE )
            {
                if(E_HVD_OK != MDrv_HVD_SetISREvent(eEvent, (HVD_InterruptCb) _VDEC_IsrProc))
                {
                    _VDEC_MutexUnlock();
                    return E_VDEC_FAIL;
                }

            }
            retVal = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
#if 1
            if (_u32VdecIsrEventFlag == E_VDEC_EVENT_OFF)
            {
                MDrv_RVD_SetISREvent(E_RVD_ISR_NONE, (RVD_InterruptCb) _VDEC_IsrProc);
            }
            retVal = E_VDEC_OK;

#else
            retVal = E_VDEC_RET_UNSUPPORTED;
#endif
            break;
        }
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    _VDEC_MutexUnlock();

    return retVal;
}

//------------------------------------------------------------------------------
/// Unset ISR event CB
/// @param cb_type \b IN : The CB type for VDEC interrupt
/// @param u32EnableEvent \b IN : The event flag for user data CB function
/// @return status
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_UnsetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&cb_type;
    IO_arg.param[1] = (void *)&u32EnableEvent;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_UNSETEVENT_EX, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_UnsetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent)
{
    VDEC_Result retVal = E_VDEC_FAIL;
    MS_U32 i = 0;

    _VDEC_MutexLock();

    if (!_Attr.bInit)
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_NOT_INIT;
    }

    if(u32EnableEvent == E_VDEC_EVENT_OFF)
    {
        _VDEC_MutexUnlock();
        return E_VDEC_RET_INVALID_PARAM;
    }

    _Attr.VdecEventInfo[cb_type].u32EventBitMap &= ~u32EnableEvent;

    if(E_VDEC_EVENT_OFF == _Attr.VdecEventInfo[cb_type].u32EventBitMap)
    {
        _Attr.VdecEventInfo[cb_type].pVdecEventCb = NULL;
        _Attr.VdecEventInfo[cb_type].pParam = NULL;
    }

#if defined(VDEC_ISR_MONITOR)
    _u32VdecIsrEventFlag = E_VDEC_EVENT_ISR_EVENT_CHANGE; //reset event flag;
#else
    _u32VdecIsrEventFlag = 0; //reset event flag;
#endif
    for(i = 0; i < E_VDEC_CB_MAX_NUM; i++)
    {
        _u32VdecIsrEventFlag |= _Attr.VdecEventInfo[i].u32EventBitMap;
    }

#if defined(VDEC_ISR_MONITOR)
    MsOS_SetEvent(_s32VdecEventId, E_VDEC_EVENT_ISR_EVENT_CHANGE);
#endif

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MS_U32 u32Event = 0;

            //Set Event
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                u32Event |= E_MVD_EVENT_DISP_RDY;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_SEQ_HDR_FOUND)
            {
                u32Event |= E_MVD_EVENT_SEQ_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
#if defined(MVD_SUPPORT_X4_CC)
                u32Event |= E_MVD_EVENT_USER_DATA_DISP;
#else
                u32Event |= E_MVD_EVENT_USER_DATA;
#endif
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                u32Event |= E_MVD_EVENT_USER_DATA;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                u32Event |= E_MVD_EVENT_FIRST_FRAME;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_PIC_FOUND)
            {
                u32Event |= E_MVD_EVENT_PIC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                u32Event |= E_MVD_EVENT_UNMUTE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                u32Event |= E_MVD_EVENT_DEC_I;
            }


            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexUnlock();
            }
            if(E_MVD_RET_OK != MDrv_MVD_SetIsrEvent(u32Event, (MVD_InterruptCb) _VDEC_IsrProc))
            {
                retVal = E_VDEC_FAIL;
            }
            else
            {
                retVal = E_VDEC_OK;
            }
            if (u32Event == E_MVD_EVENT_DISABLE_ALL)
            {
                _VDEC_MutexLock();
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            HVD_ISR_Event eEvent = E_HVD_ISR_NONE;

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_ONE)
            {
                eEvent|=E_HVD_ISR_DISP_ONE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_REPEAT)
            {
                eEvent|=E_HVD_ISR_DISP_REPEAT;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_WITH_CC)
            {
                eEvent|=E_HVD_ISR_DISP_WITH_CC;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ONE)
            {
                eEvent|=E_HVD_ISR_DEC_ONE;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_I)
            {
                eEvent|=E_HVD_ISR_DEC_I;
            }
            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DEC_ERR)
            {
                eEvent|=E_HVD_ISR_DEC_HW_ERR;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_CHG)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_USER_DATA_FOUND)
            {
                eEvent|=E_HVD_ISR_DEC_CC_FOUND;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_DISP_INFO_RDY)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_FIRST_FRAME)
            {
                eEvent|=E_HVD_ISR_DEC_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_VIDEO_UNMUTE)
            {
                eEvent|=E_HVD_ISR_DISP_FIRST_FRM;
            }

            if (_u32VdecIsrEventFlag & E_VDEC_EVENT_AFD_FOUND)
            {
                eEvent|=E_HVD_ISR_DEC_DISP_INFO_CHANGE;
            }

            if( eEvent != E_HVD_ISR_NONE )
            {
                if(E_HVD_OK !=MDrv_HVD_SetISREvent(eEvent, (HVD_InterruptCb) _VDEC_IsrProc))
                {
                    _VDEC_MutexUnlock();
                    return E_VDEC_FAIL;
                }
            }
            retVal = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
#if 1
            if (_u32VdecIsrEventFlag == E_VDEC_EVENT_OFF)
            {
                MDrv_RVD_SetISREvent(E_RVD_ISR_NONE, (RVD_InterruptCb) _VDEC_IsrProc);
            }
            retVal = E_VDEC_OK;

#else
            retVal = E_VDEC_RET_UNSUPPORTED;
#endif
            break;
        }
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    _VDEC_MutexUnlock();

    return retVal;
}

//------------------------------------------------------------------------------
/// Get ISR event status
/// @return status
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetEventInfo(MS_U32* u32EventFlag)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)u32EventFlag;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETEVENTINFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_GetEventInfo(MS_U32* u32EventFlag)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            retVal = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            MS_U32 u32Event = E_HVD_ISR_NONE;
            if( MDrv_HVD_GetISRInfo(&u32Event) )
            {
                *u32EventFlag = E_VDEC_EVENT_OFF;
                if (u32Event & E_HVD_ISR_DISP_ONE)
                {
                    *u32EventFlag|=E_VDEC_EVENT_DISP_ONE;
                }
                if (u32Event & E_HVD_ISR_DISP_REPEAT)
                {
                    *u32EventFlag|=E_VDEC_EVENT_DISP_REPEAT;
                }
                if (u32Event & E_HVD_ISR_DISP_WITH_CC)
                {
                    *u32EventFlag|=(E_VDEC_EVENT_DISP_WITH_CC | E_VDEC_EVENT_USER_DATA_FOUND);
                }
                if (u32Event & E_HVD_ISR_DISP_FIRST_FRM)
                {
                    *u32EventFlag|=E_VDEC_EVENT_VIDEO_UNMUTE;
                }
                if (u32Event & E_HVD_ISR_DEC_ONE)
                {
                    *u32EventFlag|=E_VDEC_EVENT_DEC_ONE;
                }
                if (u32Event & E_HVD_ISR_DEC_I)
                {
                    *u32EventFlag|=E_VDEC_EVENT_DEC_I;
                }
                if (u32Event & E_HVD_ISR_DEC_HW_ERR)
                {
                    *u32EventFlag|=E_VDEC_EVENT_DEC_ERR;
                }
                if (u32Event & E_HVD_ISR_DEC_CC_FOUND)
                {
                    *u32EventFlag|=E_VDEC_EVENT_USER_DATA_FOUND;
                }
                if (u32Event & E_HVD_ISR_DEC_DISP_INFO_CHANGE)
                {
                    MS_U8 u8Afd = MDrv_HVD_GetActiveFormat();
                    if (u8Afd != 0)
                    {
                        *u32EventFlag|=E_VDEC_EVENT_AFD_FOUND;
                    }
                    *u32EventFlag|=(E_VDEC_EVENT_DISP_INFO_CHG | E_VDEC_EVENT_DISP_INFO_RDY);
                }
                if (u32Event & E_HVD_ISR_DEC_FIRST_FRM)
                {
                    *u32EventFlag|=E_VDEC_EVENT_FIRST_FRAME;
                }
                retVal = E_VDEC_OK;
            }
            else
            {
                retVal = E_VDEC_FAIL;
            }
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }
    return retVal;
}

//------------------------------------------------------------------------------
/// Generate a special pattern to work with specific functions
/// @param ePatternType \b IN : Pattern type
/// @param u32Addr \b IN : Pattern buffer physical start address
/// @param u32Size \b IN,OUT : Pattern buffer size
///                             IN: Pattern buffer size
///                             OUT: Used Pattern buffer size
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MDrv_VDEC_GenPattern(VDEC_PatternType ePatternType ,  MS_PHYADDR u32Addr , MS_U32* u32Size)
{
    printf("VDEC Warn: MDrv_VDEC_GenPattern() will be removed. please use new API:MApi_VDEC_GenPattern()\n ");
    return MApi_VDEC_GenPattern( ePatternType   ,  u32Addr  ,  u32Size  );
}

VDEC_Result MApi_VDEC_GenPattern(VDEC_PatternType ePatternType,  MS_PHYADDR u32Addr, MS_U32 *pu32Size)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.param[0] = (void *)&ePatternType;
    IO_arg.param[1] = (void *)&u32Addr;
    IO_arg.param[2] = (void *)pu32Size;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GENPATTERN, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_GenPattern(VDEC_PatternType ePatternType,  MS_PHYADDR u32Addr, MS_U32 *pu32Size)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            if (ePatternType == E_VDEC_PATTERN_FLUSH)
            {
                _MVD_RET_HANDLE(MDrv_MVD_GenPattern(E_MVD_PATTERN_FLUSH, u32Addr, pu32Size));
            }
            else if (ePatternType == E_VDEC_PATTERN_FILEEND)
            {
                _MVD_RET_HANDLE(MDrv_MVD_GenPattern(E_MVD_PATTERN_FILEEND, u32Addr, pu32Size));
            }
            else
            {
                ret = E_VDEC_RET_UNSUPPORTED;
            }
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            if (ePatternType == E_VDEC_PATTERN_FLUSH)
            {
                _HVD_RET_HANDLE(MDrv_HVD_GenPattern(E_HVD_PATTERN_FLUSH, MS_PA2KSEG1(u32Addr), pu32Size));
            }
            else if (ePatternType == E_VDEC_PATTERN_FILEEND)
            {
                _HVD_RET_HANDLE(MDrv_HVD_GenPattern(E_HVD_PATTERN_FILEEND, MS_PA2KSEG1(u32Addr), pu32Size));
            }
            else
            {
                ret = E_VDEC_RET_UNSUPPORTED;
            }
            break;
        }
        case E_VDEC_DECODER_MJPEG:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        case E_VDEC_DECODER_RVD:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Get the information of the least length of pattern used for specific function
/// @param pFrmInfo \b IN : the information of the least length of pattern used for specific function
/// @return -the length (Bytes)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetPatternLeastLength(VDEC_PatternType ePatternType )
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Ret = 0;

    IO_arg.param[0] = (void *)&ePatternType;
    IO_arg.pRet = (void *)&u32Ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GENPATTERNLEASTLENGTH, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return u32Ret;
    }
}

MS_U32 MApi_VDEC_V2_GetPatternLeastLength(VDEC_PatternType ePatternType )
{
    MS_U32 u32Ret = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Ret = MDrv_MVD_GetPatternInfo();

            break;

        case E_VDEC_DECODER_HVD:
            if(  ePatternType == E_VDEC_PATTERN_FLUSH)
            {
                u32Ret = MDrv_HVD_GetPatternInfo( E_HVD_FLUSH_PATTERN_SIZE );
            }
            else if(  ePatternType == E_VDEC_PATTERN_FILEEND)
            {
                u32Ret = MDrv_HVD_GetPatternInfo( E_HVD_DUMMY_HW_FIFO );
            }
            break;

        case E_VDEC_DECODER_MJPEG:
            break;

        case E_VDEC_DECODER_RVD:
            break;

        default:
            break;
    }

    return u32Ret;
}

//------------------------------------------------------------------------------
/// Set deblocking process on/off
/// @param bOn \b IN : turn on / off deblocking process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_DisableDeblocking(MS_BOOL bDisable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result rst = E_VDEC_OK;

    IO_arg.param[0] = (void *)&bDisable;
    IO_arg.pRet = (void *)&rst;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_DISABLEDEBLOCKING, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return rst;
    }
}

VDEC_Result MApi_VDEC_V2_DisableDeblocking(MS_BOOL bDisable)
{
    VDEC_Result rst = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;

        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetSettings_Pro(E_HVD_SSET_DISABLE_DEBLOCKING  , bDisable ));
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            break;
        }
        default:
            break;
    }

    return rst;
}

//------------------------------------------------------------------------------
/// Set Quarter Pixel process on/off
/// @param bOn \b IN : turn on / off Quarter Pixel process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_DisableQuarterPixel(MS_BOOL bDisable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result rst = E_VDEC_OK;

    IO_arg.param[0] = (void *)&bDisable;
    IO_arg.pRet = (void *)&rst;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_DISABLEQUARTERPIXEL, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return rst;
    }

}

VDEC_Result MApi_VDEC_V2_DisableQuarterPixel(MS_BOOL bDisable)
{
    VDEC_Result rst = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;

        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetSettings_Pro(  E_HVD_SSET_DISABLE_QUARTER_PIXEL ,bDisable));
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            break;
        }
        default:
            break;
    }

    return rst;
}

//------------------------------------------------------------------------------
/// Turn on/off Auto Remove Last Zero Byte process
/// @param bOn \b IN : turn on / off Auto Remove Last Zero Byte process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetAutoRmLstZeroByte(MS_BOOL bOn)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result rst = E_VDEC_OK;

    IO_arg.param[0] = (void *)&bOn;
    IO_arg.pRet = (void *)&rst;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SETAUTORMLSTZEROBYTE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return rst;
    }

}

VDEC_Result MApi_VDEC_V2_SetAutoRmLstZeroByte(MS_BOOL bOn)
{
    VDEC_Result rst = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;

        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetAutoRmLstZeroByte(bOn));
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            break;
        }
        default:
            break;
    }

    return rst;
}

//------------------------------------------------------------------------------
/// Set the parameters of Balancing BW
/// @param u8QuarPixelTH \b IN : set Quarter pixel threshold
/// @param u8DeBlockingTH \b IN : set deblocking threshold
/// @param u8UpperBound \b IN : set upper boundary of BW control counter
/// @param u8SafeCoef \b IN : set safety coeffecient of average MB tick.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetBalanceBW(MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result rst = E_VDEC_OK;

    IO_arg.param[0] = (void *)&u8QuarPixelTH;
    IO_arg.param[1] = (void *)&u8DeBlockingTH;
    IO_arg.param[2] = (void *)&u8UpperBound;
    IO_arg.param[3] = (void *)&u8SafeCoef;
    IO_arg.pRet = (void *)&rst;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SETBALANCEBW, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return rst;
    }

}

VDEC_Result MApi_VDEC_V2_SetBalanceBW(MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef)
{
    VDEC_Result rst = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            break;

        case E_VDEC_DECODER_HVD:
        {
            _HVD_RET_HANDLE(MDrv_HVD_SetBalanceBW(u8QuarPixelTH, u8DeBlockingTH, u8UpperBound));
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            break;
        }
        default:
            break;
    }

    return rst;
}

VDEC_Result MApi_VDEC_DbgDumpStatus(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_OK;

    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_DBGDUMPSTATUS, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_DbgDumpStatus(void)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_FAIL;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MDrv_MVD_DbgDump();
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            MDrv_HVD_DbgDumpStatus( E_HVD_DUMP_STATUS_FW );
            break;
        }
        case E_VDEC_DECODER_MJPEG:
        {
            break;
        }
        case E_VDEC_DECODER_RVD:
        {
            MDrv_RVD_Dump_Status();
            break;
        }
        default:
            break;
    }
    return E_VDEC_OK;
}

extern void HAL_VPU_InitRegBase(MS_U32 u32RegBase) __attribute__((weak));
extern MS_U32 HAL_VPU_GetProgCnt(void) __attribute__((weak));

MS_U32 MApi_VDEC_DbgGetProgCnt(void)
{
    MS_U32 u32NonPMBankSize=0,u32RiuBaseAdd=0;
    if( !MDrv_MMIO_GetBASE( &u32RiuBaseAdd, &u32NonPMBankSize, MS_MODULE_HW))
    {
        printf("VDEC HVD MApi_VDEC_DbgGetProgramCnt Err: MMIO_GetBASE failure\n");
        return 0;
    }
    else
    {
        //printf("HVD:1 u32RiuBaseAdd = %lx\n", u32RiuBaseAdd);
        if( HAL_VPU_InitRegBase )
        {
            HAL_VPU_InitRegBase(u32RiuBaseAdd);
        }
        else
        {
            return 0;
        }
    }
    if( HAL_VPU_GetProgCnt )
    {
        return HAL_VPU_GetProgCnt();
    }
    else
    {
        return 0;
    }
}

VDEC_Result MApi_VDEC_SetControl(VDEC_User_Cmd cmd_id, MS_U32 param)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&cmd_id;
    IO_arg.param[1] = (void *)&param;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SETCONTROL, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_SetControl(VDEC_User_Cmd cmd_id, MS_U32 param)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (cmd_id)
    {
        case VDEC_USER_CMD_REPEAT_LAST_FIELD:
            return _VDEC_RepeatLastField((MS_BOOL)param);
            break;
        case VDEC_USER_CMD_AVSYNC_REPEAT_TH:
            return _VDEC_SetAVSyncRepeatThreshold(param);
            break;
        case VDEC_USER_CMD_DISP_ONE_FIELD:
            return _VDEC_DispOneField(param);
            break;
        case VDEC_USER_CMD_FD_MASK_DELAY_COUNT:
            return _VDEC_SetFdMaskDelayCnt(param);
            break;
        case VDEC_USER_CMD_FRC_OUTPUT:
            return _VDEC_FrameRateConvert((VDEC_FRC_OutputParam *)param);
            break;
        case VDEC_USER_CMD_FRC_DROP_TYPE:
            return _VDEC_SetFRCDropType(param);
            break;
        case VDEC_USER_CMD_FAST_DISPLAY:
            return _VDEC_SetFastDisplay((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_IGNORE_ERR_REF:
            return _VDEC_SetIgnoreErrRef((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_FORCE_FOLLOW_DTV_SPEC:
            return _VDEC_ForceFollowDTVSpec((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_AVC_MIN_FRM_GAP:
            return _VDEC_SetMinFrameGap(param);
            break;
        case VDEC_USER_CMD_DISABLE_SEQ_CHG:
            return _VDEC_SetDisableSeqChange((MS_BOOL) param);
        case VDEC_USER_CMD_AVC_DISABLE_ANTI_VDEAD:
            return _VDEC_SetDisableAntiVdead(param);
            break;
        case VDEC_USER_CMD_DTV_RESET_MVD_PARSER:
            return _VDEC_ResetMvdParser((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_PVR_FLUSH_FRAME_BUFFER://PVR used,only used in S company
            return _VDEC_DoFlushFrmBuffer((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_SET_DTV_USER_DATA_MODE:
            return _VDEC_SetDTVUserDataMode(param);
            break;
        case VDEC_USER_CMD_FORCE_INTERLACE_MODE:
            return _VDEC_ForceInterlaceMode(param);
            break;
        case VDEC_USER_CMD_SHOW_DECODE_ORDER:
            return _VDEC_ShowDecodeOrder(param);
            break;
        case VDEC_USER_CMD_FORCE_AUTO_MUTE:
            return _VDEC_ForceAutoMute((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_SET_VSIZE_ALIGN:
            return _VDEC_SetVSizeAlign((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_AVC_DISP_IGNORE_CROP:
            return _VDEC_Disp_Ignore_Crop((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_SET_DISP_FINISH_MODE:
            return _VDEC_SetDispFinishMode((MS_U8) param);
            break;
        case VDEC_USER_CMD_SET_AVSYNC_MODE:
            return _VDEC_SetAVSyncMode((MS_U8) param);
            break;
        case VDEC_USER_CMD_SET_CC608_INFO_ENHANCE_MODE:
            return _VDEC_SetCcInfoEnhanceMode((MS_BOOL) param);
            break;
        case VDEC_USER_CMD_SET_IDCT_MODE:
            return _VDEC_SetIdctMode((MS_U8) param);
            break;
        case VDEC_USER_CMD_DROP_ERR_FRAME:
            return _VDEC_DropErrFrame((MS_BOOL) param);
        case VDEC_USER_CMD_PUSH_DISPQ_WITH_REF_NUM:
            return _VDEC_PushDispQWithRefNum((MS_BOOL) param);
        case VDEC_USER_CMD_SHOW_FIRST_FRAME_DIRECT:
            return  _VDEC_ShowFirstFrameDirect(param);
        case VDEC_USER_CMD_AUTO_EXHAUST_ES_MODE:
            return  _VDEC_SetAutoExhaustESMode(param);
            break;
        case VDEC_USER_CMD_SET_MIN_TSP_DATA_SIZE:
            return  _VDEC_SetMinTspDataSize(param);
            break;
        case VDEC_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE:
            return  _VDEC_AVCSupportRefNumOverMaxDBPSize((MS_BOOL)param);
            break;
        default:
            return E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return E_VDEC_OK;
}

VDEC_Result MApi_VDEC_GetControl(VDEC_User_Cmd cmd_id, MS_U32 *param)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&cmd_id;
    IO_arg.param[1] = (void *)&param;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETCONTROL, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_GetControl(VDEC_User_Cmd cmd_id, MS_U32 *param)
{
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    if (param == NULL)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (cmd_id)
    {
        case VDEC_USER_CMD_GET_CHROMA_TYPE:
            return _MApi_VDEC_IsChroma420(param);
            break;
        case VDEC_USER_CMD_GET_REAL_FRAMERATE:
            return _MApi_VDEC_GetRealFrameRate(param);
            break;
        case VDEC_USER_CMD_GET_COLOR_MATRIX:
            return _MApi_VDEC_GetColorMatrixCoef(param);
            break;
        case VDEC_USER_CMD_GET_DYNSCALE_ENABLED:
            return _MApi_VDEC_GetIsDynScalingEnabled(param);
            break;
        case VDEC_USER_CMD_GET_ORI_INTERLACE_MODE:
            return _MApi_VDEC_GetIsOriInterlaceMode(param);
            break;
        case VDEC_USER_CMD_GET_FPA_SEI:
            return _MApi_VDEC_GetFrmPackingArrSEI(param);
            break;
        case VDEC_USER_CMD_GET_FB_USAGE_MEM:
            return _MApi_VDEC_GetFBUsageMem(param);
            break;
        case VDEC_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM:
            return _MApi_VDEC_GetFlushPatternEntryNum(param);
            break;

        default:
            return E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return E_VDEC_OK;
}

MS_U32 MApi_VDEC_GetDispCnt(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32Cnt = 0;

    IO_arg.pRet = (void *)&u32Cnt;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETDISPCNT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return u32Cnt;
    }
}

MS_U32 MApi_VDEC_V2_GetDispCnt(void)
{
    MS_U32 u32Cnt = 0;

    if (!_Attr.bInit)
    {
        return 0;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            u32Cnt = MDrv_MVD_GetDispCount();
            break;

        case E_VDEC_DECODER_HVD:
            u32Cnt = MDrv_HVD_GetData(E_HVD_GDATA_TYPE_DISP_CNT);
            break;

        case E_VDEC_DECODER_MJPEG:
            break;

        case E_VDEC_DECODER_RVD:
            break;

        default:
            u32Cnt = 0;
            break;
    }

    return u32Cnt;
}

MS_U32 MApi_VDEC_GetFwVersion(VDEC_FwType eFwType)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32FWVer = 0;

    IO_arg.param[0] = (void *)&eFwType;
    IO_arg.pRet = (void *)&u32FWVer;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETFWVERSION, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return u32FWVer;
    }
}

MS_U32 MApi_VDEC_V2_GetFwVersion(VDEC_FwType eFwType)
{
    const MVD_DrvInfo* pMvdDrvInfo = NULL;
    MS_U32 u32FWVer = 0;

    switch (eFwType)
    {
        case VDEC_FW_TYPE_MVD:
            pMvdDrvInfo = MDrv_MVD_GetInfo();
            u32FWVer = pMvdDrvInfo->u32FWVersion;
            if ((TRUE == _Attr.bInit) && (E_VDEC_DECODER_MVD == _Attr.eDecoder))
            {
                if (MDrv_MVD_GetFWVer() != 0)
                {
                    u32FWVer = MDrv_MVD_GetFWVer();
                }
            }
            break;

        case VDEC_FW_TYPE_HVD:
            u32FWVer = MDrv_HVD_GetDrvFwVer();
            if ((TRUE == _Attr.bInit) && (E_VDEC_DECODER_HVD == _Attr.eDecoder))
            {
                u32FWVer = MDrv_HVD_GetFwVer();
            }
            break;
        default:
            break;
    }

    return u32FWVer;
}

VDEC_Result MApi_VDEC_SetBlockDisplay(MS_BOOL bEnable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.param[0] = (void *)&bEnable;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SETBLOCKDISPLAY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_SetBlockDisplay(MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_SetMStreamerMode(bEnable));
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_EnableDispQue(bEnable));
            break;

        case E_VDEC_DECODER_RVD:
            break;

        case E_VDEC_DECODER_MJPEG:
            _BOOL_TO_MJPEG_RESULT(ret, MApi_MJPEG_EnableDispCmdQ(bEnable));
            break;
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}

VDEC_Result MApi_VDEC_EnableESBuffMalloc(MS_BOOL bEnable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&bEnable;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_ENABLEESBUFFMALLOC, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_EnableESBuffMalloc(MS_BOOL bEnable)
{
    //check vdec state & config
    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if ((E_VDEC_DECODER_MVD != _Attr.eDecoder) &&
        (E_VDEC_DECODER_HVD != _Attr.eDecoder) &&
        (E_VDEC_DECODER_MJPEG != _Attr.eDecoder) &&
        (E_VDEC_DECODER_RVD != _Attr.eDecoder))
    {
        return E_VDEC_RET_UNSUPPORTED;
    }

    bEsBufMgmt = bEnable;
    return E_VDEC_OK;
}

MS_U32 MApi_VDEC_GetESBuffVacancy(void* pData)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_U32 u32BuffVacancy = 0;

    IO_arg.param[0] = (void *)pData;
    IO_arg.pRet = (void *)&u32BuffVacancy;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETESBUFFVACANCY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return u32BuffVacancy;
    }
}

MS_U32 MApi_VDEC_V2_GetESBuffVacancy(void* pData)
{
    UNUSED(pData);
    return _VDEC_GetESBuffVacancy();
}

VDEC_Result MApi_VDEC_GetESBuff(MS_U32 u32ReqSize, MS_U32* u32AvailSize, MS_PHYADDR* u32Addr)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&u32ReqSize;
    IO_arg.param[1] = (void *)u32AvailSize;
    IO_arg.param[2] = (void *)u32Addr;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETESBUFFVACANCY, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_GetESBuff(MS_U32 u32ReqSize, MS_U32* u32AvailSize, MS_PHYADDR* u32Addr)
{
#define ES_SAFE_SIZE    (0x20)
    MS_U32 u32ReadPtr = 0;
    MS_U32 u32WritePtr = 0;
    MS_U32 u32WritePtrNew = 0;
    MS_U32 u32BuffSt = 0;
    MS_U32 u32BuffEnd = 0;
    VDEC_SysCfg* pCfg = NULL;

    if (!bEsBufMgmt)
    {
        //printf("Please EnableESBuffMalloc before use GetESBuff\n");
        return E_VDEC_RET_ILLEGAL_ACCESS;
    }
    //check input parameters
    if ((NULL==u32AvailSize) || (NULL==u32Addr))
    {
        //printf("NULL pointer for output\n");
        return E_VDEC_RET_INVALID_PARAM;
    }

    if (_VDEC_GetESBuffVacancy() < u32ReqSize)
    {
        VDEC_INFO(printf("ESVaca(0x%lx) < ReqSize(0x%lx)\n", _VDEC_GetESBuffVacancy(), u32ReqSize));
        return E_VDEC_FAIL;
    }

    //check DecCmdQ/BBU vacancy
    if (MApi_VDEC_GetDecQVacancy() == 0)
    {
        VDEC_INFO(printf("DecQ full\n"));
        goto _NO_ES_BUFF;
    }

#if 0//_VDEC_PTS_TABLE
    if (_VDEC_PtsTableIsFull())
    {
        VDEC_INFO(printf("PtsTable full\n"));
        goto _NO_ES_BUFF;
    }
#endif
    //check ES read/write pointer
    pCfg = &_vdecInitParam.SysConfig;

    if(u32VdecInitParamVersion == 1)
    {
        pCfg = &_vdecInitParam_ex1.SysConfig;
    }
    u32ReadPtr = MApi_VDEC_GetESReadPtr();
    u32WritePtr = MApi_VDEC_GetESWritePtr();

    u32BuffSt = pCfg->u32DrvProcBufSize;
    u32BuffEnd = pCfg->u32BitstreamBufSize;

    if (u32WritePtr == 0)
    {
        u32WritePtr = u32BuffSt;
    }

    u32ReqSize += ES_SAFE_SIZE; //for safety
    u32WritePtrNew = u32WritePtr + u32ReqSize;
    if (u32ReadPtr <= u32WritePtr)
    {
        if (u32WritePtrNew >= u32BuffEnd)
        {
            u32WritePtrNew = u32BuffSt + u32ReqSize;

            if (u32WritePtrNew > u32ReadPtr)
            {
                VDEC_INFO(printf("xRd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%lx\n", u32ReadPtr, u32WritePtr,
                    u32WritePtrNew, pCfg->u32BitstreamBufAddr, u32ReqSize));
                goto _NO_ES_BUFF;
            }
            u32WritePtr = u32BuffSt;
        }
    }
    else
    {
        if (u32WritePtrNew > u32ReadPtr)
        {
            VDEC_INFO(printf("xRd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%lx\n", u32ReadPtr, u32WritePtr,
                u32WritePtrNew, pCfg->u32BitstreamBufAddr, u32ReqSize));
            goto _NO_ES_BUFF;
        }
    }

    *u32AvailSize = u32ReqSize;
    *u32Addr = u32WritePtr + pCfg->u32BitstreamBufAddr;
    VDEC_INFO(printf("Rd=0x%lx, Wr=0x%lx, WrNew=0x%lx, BsBuff=0x%lx, ReqSize=0x%lx\n", u32ReadPtr, u32WritePtr,
           u32WritePtrNew, pCfg->u32BitstreamBufAddr, u32ReqSize));

    return E_VDEC_OK;

_NO_ES_BUFF:
    *u32AvailSize = 0;
    *u32Addr = (MS_PHYADDR)NULL;
    return E_VDEC_FAIL;
}

VDEC_Result MApi_VDEC_GetNextDispFrame(VDEC_DispFrame** ppDispFrm)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.param[0] = (void *)ppDispFrm;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_GETNEXTDISPFRAME, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_GetNextDispFrame(VDEC_DispFrame** ppDispFrm)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        //printf("VDEC not_init %s\n", __FUNCTION__);
        return E_VDEC_RET_NOT_INIT;
    }
    if (!ppDispFrm)
    {
        //printf("NULL pointer for %s\n", __FUNCTION__);
        return E_VDEC_RET_INVALID_PARAM;
    }
    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
        {
            MVD_FrmInfo stFrm;

            _VDEC_Memset(&stFrm, 0, sizeof(MVD_FrmInfo));
            _MVD_RET_HANDLE(MDrv_MVD_GetFrmInfo(E_MVD_FRMINFO_NEXT_DISPLAY, &stFrm));

            VDEC_DispFrame* pDispFrm = _VDEC_MallocDq(E_VDEC_STREAM_0);

            if (!pDispFrm)
            {
                //printf("No available DispQ!!!\n");
                ret = E_VDEC_FAIL;
                break;
            }

            pDispFrm->stFrmInfo.eFrameType    = _VDEC_MapFrmType2MVD(stFrm.eFrmType);
            pDispFrm->stFrmInfo.u16Height     = stFrm.u16Height;
            pDispFrm->stFrmInfo.u16Width      = stFrm.u16Width;
            pDispFrm->stFrmInfo.u16Pitch      = stFrm.u16Pitch;
            pDispFrm->stFrmInfo.u32ChromaAddr = stFrm.u32ChromaAddr;
            pDispFrm->stFrmInfo.u32ID_H       = stFrm.u32ID_H;
            pDispFrm->stFrmInfo.u32ID_L       = stFrm.u32ID_L;
            pDispFrm->stFrmInfo.u32LumaAddr   = stFrm.u32LumaAddr;
            pDispFrm->stFrmInfo.u32TimeStamp  = stFrm.u32TimeStamp;
            pDispFrm->stFrmInfo.eFieldType    = E_VDEC_FIELDTYPE_BOTH;
            *ppDispFrm = pDispFrm;
            pDispFrm->u32PriData = stFrm.u16FrmIdx;
            //printf("apiVDEC_MVD pts=%ld, frmIdx=%d\n", pDispFrm->stFrmInfo.u32TimeStamp, stFrm.u16FrmIdx);
#if _VDEC_PTS_TABLE
            pDispFrm->stFrmInfo.u32TimeStamp  = _VDEC_PtsTableGet();
#endif
            ret = E_VDEC_OK;
            break;
        }
        case E_VDEC_DECODER_HVD:
        {
            HVD_Frame_Info info;

            _VDEC_Memset(&info, 0, sizeof(HVD_Frame_Info));
            _HVD_RET_HANDLE(MDrv_HVD_GetFrmInfo(E_HVD_GFRMINFO_NEXT_DISPLAY, &info));
            VDEC_DispFrame* pDispFrm = _VDEC_MallocDq(E_VDEC_STREAM_0);

            if (!pDispFrm)
            {
                //printf("No available DispQ!!!\n");
                ret = E_VDEC_FAIL;
                break;
            }

            pDispFrm->stFrmInfo.eFrameType    = _VDEC_MapFrmType2HVD(info.eFrmType);
            pDispFrm->stFrmInfo.u16Height     = info.u16Height;
            pDispFrm->stFrmInfo.u16Width      = info.u16Width;
            pDispFrm->stFrmInfo.u16Pitch      = info.u16Pitch;
            pDispFrm->stFrmInfo.u32ChromaAddr = info.u32ChromaAddr;
            pDispFrm->stFrmInfo.u32ID_H       = info.u32ID_H;
            pDispFrm->stFrmInfo.u32ID_L       = info.u32ID_L;
            pDispFrm->stFrmInfo.u32LumaAddr   = info.u32LumaAddr;
            pDispFrm->stFrmInfo.u32TimeStamp  = info.u32TimeStamp;
            pDispFrm->stFrmInfo.eFieldType    = (VDEC_FieldType) info.eFieldType;
            *ppDispFrm = pDispFrm;

            pDispFrm->u32PriData = MDrv_HVD_GetNextDispQPtr();
            //printf("apiVDEC pts=%ld pri=0x%lx\n", pDispFrm->stFrmInfo.u32TimeStamp, pDispFrm->u32PriData);

            ret = E_VDEC_OK;

            break;
        }
        case E_VDEC_DECODER_RVD:
            break;

        case E_VDEC_DECODER_MJPEG:
        {
            MJPEG_FrameInfo info;
            _VDEC_Memset(&info, 0, sizeof(MJPEG_FrameInfo));
            _MJPEG_RET_HANDLE(MApi_MJPEG_GetNextDispFrame(&info));
            VDEC_DispFrame* pDispFrm = _VDEC_MallocDq(E_VDEC_STREAM_0);

            if (!pDispFrm)
            {
                //printf("No available DispQ!!!\n");
                ret = E_VDEC_FAIL;
                break;
            }
            pDispFrm->stFrmInfo.eFrameType    = E_VDEC_FRM_TYPE_I;
            pDispFrm->stFrmInfo.u16Height     = info.u16Height;
            pDispFrm->stFrmInfo.u16Width      = info.u16Width;
            pDispFrm->stFrmInfo.u16Pitch      = info.u16Pitch;
            pDispFrm->stFrmInfo.u32ChromaAddr = info.u32ChromaAddr;
            pDispFrm->stFrmInfo.u32ID_H       = info.u32ID_H;
            pDispFrm->stFrmInfo.u32ID_L       = info.u32ID_L;
            pDispFrm->stFrmInfo.u32LumaAddr   = info.u32LumaAddr;
            pDispFrm->stFrmInfo.u32TimeStamp  = info.u32TimeStamp;
            pDispFrm->stFrmInfo.eFieldType    = E_VDEC_FIELDTYPE_BOTH;
            *ppDispFrm = pDispFrm;

            pDispFrm->u32PriData = MApi_MJPEG_GetNextDispQPtr();
            //printf("apiVDEC pts=%ld\n", pDispFrm->stFrmInfo.u32TimeStamp);

            ret = E_VDEC_OK;

            break;
        }
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}

//------------------------------------------------------------------------------
/// Notify FW to display the specified frame
/// @param pDispFrm \b IN : the pointer of the frame to be displayed
/// @return VDEC_Result
/// @retval -E_VDEC_RET_NOT_INIT: VDEC is not initialized.
/// @retval -E_VDEC_RET_INVALID_PARAM: the pointer is NULL.
/// @retval -E_VDEC_RET_ILLEGAL_ACCESS: the pointer is invalid.
///          There might be multiple frame display/release.
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_DisplayFrame(VDEC_DispFrame *pDispFrm)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.param[0] = (void *)pDispFrm;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_DISPLAYFRAME, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_DisplayFrame(VDEC_DispFrame *pDispFrm)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if (!pDispFrm)
    {
        //printf("NULL pointer for %s\n", __FUNCTION__);
        return E_VDEC_RET_INVALID_PARAM;
    }

    if (_VDEC_IsDqValid(pDispFrm))
    {
        VDEC_INFO(printf("%s invalid pDispFrm 0x%lx\n", __FUNCTION__, pDispFrm->u32PriData));
        return E_VDEC_RET_ILLEGAL_ACCESS;
    }

   switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_FrameFlip(pDispFrm->u32PriData));
            _VDEC_FreeDq(E_VDEC_STREAM_0, pDispFrm);
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_DispFrame(pDispFrm->u32PriData));
            _VDEC_FreeDq(E_VDEC_STREAM_0, pDispFrm);
            break;

        case E_VDEC_DECODER_RVD:
            break;

        case E_VDEC_DECODER_MJPEG:
            _MJPEG_RET_HANDLE(MApi_MJPEG_DispFrame(pDispFrm->u32PriData));
            _VDEC_FreeDq(E_VDEC_STREAM_0, pDispFrm);
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}

VDEC_Result MApi_VDEC_ReleaseFrame(VDEC_DispFrame *pDispFrm)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.param[0] = (void *)pDispFrm;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_RELEASEFRAME, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_ReleaseFrame(VDEC_DispFrame *pDispFrm)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    if (!pDispFrm)
    {
        //printf("NULL pointer for %s\n", __FUNCTION__);
        return E_VDEC_RET_INVALID_PARAM;
    }

    if (_VDEC_IsDqValid(pDispFrm))
    {
        VDEC_INFO(printf("%s invalid pDispFrm 0x%lx\n", __FUNCTION__, pDispFrm->u32PriData));
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_FrameRelease(pDispFrm->u32PriData));
            _VDEC_FreeDq(E_VDEC_STREAM_0, pDispFrm);
            break;

        case E_VDEC_DECODER_HVD:
            _HVD_RET_HANDLE(MDrv_HVD_FreeFrame(pDispFrm->u32PriData));
            _VDEC_FreeDq(E_VDEC_STREAM_0, pDispFrm);
            break;

        case E_VDEC_DECODER_RVD:
            break;

        case E_VDEC_DECODER_MJPEG:
            _MJPEG_RET_HANDLE(MApi_MJPEG_FreeFrame(pDispFrm->u32PriData));
            _VDEC_FreeDq(E_VDEC_STREAM_0, pDispFrm);
            break;

        default:
            ret = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }
    return ret;
}

VDEC_Result MApi_VDEC_FlushPTSBuf(void)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_FLUSHPTSBUF, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_FlushPTSBuf(void)
{
    VDEC_Result ret = E_VDEC_OK;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_FlushPTSBuf());
            break;

        case E_VDEC_DECODER_HVD:
             break;

        case E_VDEC_DECODER_RVD:
            break;

        case E_VDEC_DECODER_MJPEG:
            break;
        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }
    return ret;
}

VDEC_Result MApi_VDEC_CaptureFrame(MS_U32 u32FrmPriData, MS_BOOL bEnable)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;

    IO_arg.param[0] = (void *)&u32FrmPriData;
    IO_arg.param[1] = (void *)&bEnable;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CAPTUREFRAME, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_CaptureFrame(MS_U32 u32FrmPriData, MS_BOOL bEnable)
{
    VDEC_Result ret = E_VDEC_OK;
    static MS_U32 u32PriData = VDEC_U32_MAX;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if (u32FrmPriData == VDEC_U32_MAX)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    //check the disable frame is valid
    if ((FALSE == bEnable) && (u32PriData != u32FrmPriData))
    {
        printf("%s: Release an uncaptured frame(0x%lx != 0x%lx)!\n",
            __FUNCTION__, u32FrmPriData, u32PriData);
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            _BOOL_TO_VDEC_RESULT(ret, MDrv_MVD_FrameCapture(u32FrmPriData, bEnable));
            break;

        case E_VDEC_DECODER_HVD:
            break;

        case E_VDEC_DECODER_MJPEG:
            break;

        default:
            ret = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    if (E_VDEC_OK == ret)
    {
        if(FALSE == bEnable)
        {
            u32PriData = VDEC_U32_MAX;
        }
        else
        {   //record the captured frame idx
            u32PriData = u32FrmPriData;
        }
    }

    return ret;
}

VDEC_Result MApi_VDEC_CC_Init(MS_U32 *pIn)
{
     if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)pIn;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_INIT, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_Init(MS_U32 *pIn)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MDrv_CC_Init();
                retVal = E_VDEC_OK;
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                retVal = MDrv_HVD_CC_Init();
            }
            break;

        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_SetCfg(VDEC_CCFormat eFmt, VDEC_CCType eType, MS_U32 *pIn)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.param[1] = (void *)&eType;
    IO_arg.param[2] = (void *)pIn;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_SETCFG, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_SetCfg(VDEC_CCFormat eFmt, VDEC_CCType eType, MS_U32 *pIn)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if (!pIn)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MS_U8 u8Opt = _VDEC_Map2MVDCCOpt(eType);
                MS_U16 u16BufferSize = 0;
                VDEC_CC_InputPara* pInput = (VDEC_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    printf("%s not supported ver %ld\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_FAIL;
                }
                u16BufferSize = (MS_U16)pInput->u32Val;
                MDrv_CC_CM_SetParsingType(u8Opt, u16BufferSize, u8Type);
                retVal = E_VDEC_OK;
            }
            break;
        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                MS_U8 u8Opt = _VDEC_Map2HVDCCOpt(eType);
                MS_U16 u16BufferSize = 0;
                VDEC_CC_InputPara* pInput = (VDEC_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    printf("%s not supported ver %ld\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_FAIL;
                }
                u16BufferSize = (MS_U16)pInput->u32Val;
                retVal = MDrv_HVD_CC_SetCfg(u8Opt, u16BufferSize, u8Type);
            }
            break;
        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_SetBuffStartAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.param[1] = (void *)pIn;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_SETBUFFSTARTADD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_SetBuffStartAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if (!pIn)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MS_U32 u32StartAdd = 0;
                VDEC_CC_InputPara* pInput = (VDEC_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    printf("%s not supported ver %ld\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_FAIL;
                }
                u32StartAdd = pInput->u32Val;
                MDrv_CC_CM_SetMVDRB_HWAddr(u32StartAdd, u8Type);
                retVal = E_VDEC_OK;
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                MS_U32 u32StartAdd = 0;
                VDEC_CC_InputPara* pInput = (VDEC_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    printf("%s not supported ver %ld\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_FAIL;
                }
                u32StartAdd = pInput->u32Val;
                retVal = MDrv_HVD_CC_Set_RB_StartAddr(u32StartAdd, u8Type);
            }
            break;

        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_UpdateWriteAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.param[1] = (void *)pIn;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_UPDATEWRITEADD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_UpdateWriteAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MDrv_CC_CM_SyncMVDRB_SWAddr2HWAddr(u8Type);
                retVal = E_VDEC_OK;
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                retVal = MDrv_HVD_CC_SyncRB_RdAddr2WrAddr(u8Type);
            }
            break;

        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_UpdateReadAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
      if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.param[1] = (void *)pIn;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_UPDATEREADADD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_UpdateReadAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if (!pIn)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MS_U32 u32EachPktSize = 0;
                VDEC_CC_InputPara* pInput = (VDEC_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    printf("%s not supported ver %ld\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_FAIL;
                }
                u32EachPktSize = pInput->u32Val;
                MDrv_CC_PM_SetMVDRB_ReadAddr(u32EachPktSize, u8Type);
                retVal = E_VDEC_OK;
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                MS_U32 u32EachPktSize = 0;
                VDEC_CC_InputPara* pInput = (VDEC_CC_InputPara*)pIn;
                if (pInput->u32Ver != 0)
                {
                    printf("%s not supported ver %ld\n", __FUNCTION__, pInput->u32Ver);
                    return E_VDEC_FAIL;
                }
                u32EachPktSize = pInput->u32Val;
                retVal = MDrv_HVD_CC_Adv_RB_ReadAddr(u32EachPktSize, u8Type);
            }
            break;

        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_DisableParsing(VDEC_CCFormat eFmt)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_DISABLEPARSING, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_DisableParsing(VDEC_CCFormat eFmt)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                MDrv_CC_CM_DisableParsing(u8Type);
                retVal = E_VDEC_OK;
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                retVal = MDrv_HVD_CC_DisableParsing(u8Type);
            }
            break;

        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_GetInfo(VDEC_CCFormat eFmt, VDEC_CCInfoCmd eCmd, MS_U32 *pOut)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.param[1] = (void *)&eCmd;
    IO_arg.param[2] = (void *)pOut;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETINFO, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_GetInfo(VDEC_CCFormat eFmt, VDEC_CCInfoCmd eCmd, MS_U32 *pOut)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }
    if (!pOut)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                EN_CC_MVD_INFO eCCInfo = CC_SELECTOR_CCMVD_RINGBUFFER;
                MS_U32 u32Val1=0, u32Val2=0;
                if (E_VDEC_CC_GET_708_ENABLE == eCmd)
                {
                    eCCInfo = CC_SELECTOR_708_SW;
                }
                MDrv_CC_CM_GetInfo(eCCInfo, u8Type, &u32Val1, &u32Val2);
                *pOut = u32Val1;
                if (E_VDEC_CC_GET_BUFF_SIZE == eCmd)
                {
                    *pOut = u32Val2;
                }
                retVal = E_VDEC_OK;
            }
            break;

         case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                EN_CC_MVD_INFO eCCInfo = HVD_CC_SELECTOR_RINGBUFFER;
                MS_U32 u32Val1=0, u32Val2=0;
                if (E_VDEC_CC_GET_708_ENABLE == eCmd)
                {
                    eCCInfo = HVD_CC_SELECTOR_708_SW;
                }
                retVal = MDrv_HVD_CC_GetInfo(eCCInfo, u8Type, &u32Val1, &u32Val2);
                *pOut = u32Val1;
                if (E_VDEC_CC_GET_BUFF_SIZE == eCmd)
                {
                    *pOut = u32Val2;
                }
            }
            break;

        default:
            retVal = E_VDEC_RET_UNSUPPORTED;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_GetIsRstDone(VDEC_CCFormat eFmt)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;


    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETISRSTDONE, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_GetIsRstDone(VDEC_CCFormat eFmt)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(retVal, MDrv_CC_CM_IsMvdRstDone(u8Type));
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(retVal, MDrv_HVD_CC_IsHvdRstDone(u8Type));
            }
            break;

        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

VDEC_Result MApi_VDEC_CC_GetIsBuffOverflow(VDEC_CCFormat eFmt)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result retVal = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.pRet = (void *)&retVal;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETISBUFFOVERFLOW, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return retVal;
    }
}

VDEC_Result MApi_VDEC_V2_CC_GetIsBuffOverflow(VDEC_CCFormat eFmt)
{
    VDEC_Result retVal = E_VDEC_FAIL;

    if (!_Attr.bInit)
    {
        return E_VDEC_RET_NOT_INIT;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(retVal, MDrv_CC_CM_GetOverflowStatus(u8Type));
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                _BOOL_TO_VDEC_RESULT(retVal, MDrv_HVD_CC_GetOverflowStatus(u8Type));
            }
            break;

        case E_VDEC_DECODER_MJPEG:
        case E_VDEC_DECODER_RVD:
        default:
            retVal = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return retVal;
}

//------------------------------------------------------------------------------
/// Report the physical address of write pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHYADDR
//------------------------------------------------------------------------------
MS_PHYADDR MApi_VDEC_CC_GetWriteAdd(VDEC_CCFormat eFmt)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_PHYADDR u32CCWrPtr = VDEC_U32_MAX;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.pRet = (void *)&u32CCWrPtr;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETWRITEADD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return u32CCWrPtr;
    }
}

MS_PHYADDR MApi_VDEC_V2_CC_GetWriteAdd(VDEC_CCFormat eFmt)
{
    MS_PHYADDR u32CCWrPtr = VDEC_U32_MAX;

    if (!_Attr.bInit)
    {
        return u32CCWrPtr;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                u32CCWrPtr = MDrv_CC_PM_GetMVDRB_WriteAddr(u8Type);
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                u32CCWrPtr = MDrv_HVD_CC_Get_RB_WriteAddr(u8Type);
            }
            break;

        default:
            break;
    }

    return u32CCWrPtr;
}

//------------------------------------------------------------------------------
/// Report the physical address of read pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHYADDR
//------------------------------------------------------------------------------
MS_PHYADDR MApi_VDEC_CC_GetReadAdd(VDEC_CCFormat eFmt)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    MS_PHYADDR u32CCRdPtr = VDEC_U32_MAX;

    IO_arg.param[0] = (void *)&eFmt;
    IO_arg.pRet = (void *)&u32CCRdPtr;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_CC_GETREADADD, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return u32CCRdPtr;
    }
}

MS_PHYADDR MApi_VDEC_V2_CC_GetReadAdd(VDEC_CCFormat eFmt)
{
    MS_PHYADDR u32CCRdPtr = VDEC_U32_MAX;

    if (!_Attr.bInit)
    {
        return u32CCRdPtr;
    }

    switch (_Attr.eDecoder)
    {
        case E_VDEC_DECODER_MVD:
            {
                MS_U8 u8Type = _VDEC_Map2MVDCCFmt(eFmt);
                u32CCRdPtr = MDrv_CC_PM_GetMVDRB_ReadAddr(u8Type);
            }
            break;

        case E_VDEC_DECODER_HVD:
            {
                MS_U8 u8Type = _VDEC_Map2HVDCCFmt(eFmt);
                u32CCRdPtr = MDrv_HVD_CC_Get_RB_ReadAddr(u8Type);
            }
            break;

        default:
            break;
    }

    return u32CCRdPtr;
}

VDEC_Result MApi_VDEC_SystemPreSetControl(VDEC_User_Cmd cmd_id, void *pParam)
{
     if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result eRet = E_VDEC_FAIL;

    IO_arg.param[0] = (void *)&cmd_id;
    IO_arg.param[0] = (void *)pParam;
    IO_arg.pRet = (void *)&eRet;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_SYSTEMPRESETCONTROL, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return eRet;
    }
}

VDEC_Result MApi_VDEC_V2_SystemPreSetControl(VDEC_User_Cmd cmd_id, void *pParam)
{
    MS_U32 *pu32Param = (MS_U32 *)pParam;

    switch (cmd_id)
    {
        case VDEC_USER_CMD_SYSTEM_PRESET_VPU_CLOCK:
            return _VDEC_SetVPUClockSpeed((VDEC_ClockSpeed)(*pu32Param));
            break;

        case VDEC_USER_CMD_SYSTEM_PRESET_HVD_CLOCK:
            return _VDEC_SetHVDClockSpeed((VDEC_ClockSpeed)(*pu32Param));
            break;

        case VDEC_USER_CMD_SYSTEM_PRESET_MVD_CLOCK:
            return _VDEC_SetMVDClockSpeed((VDEC_ClockSpeed)(*pu32Param));
            break;

        default:
            return E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return E_VDEC_OK;
}

VDEC_Result MApi_VDEC_PreSetControl(VDEC_User_Cmd cmd_id, MS_U32 param)
{
    if (pu32VDEC_Inst == NULL)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }

    VDEC_V2_IO_Param IO_arg;
    VDEC_Result ret = E_VDEC_OK;


    IO_arg.param[0] = (void *)&cmd_id;
    IO_arg.param[1] = (void *)&param;
    IO_arg.pRet = (void *)&ret;

    if(UtopiaIoctl(pu32VDEC_Inst,E_VDEC_V2_CMD_PRESETCONTROL, (void*)&IO_arg) != UTOPIA_STATUS_SUCCESS)
    {
        printf("[Fail][%s,%d]\n",__FUNCTION__,__LINE__);
        return E_VDEC_FAIL;
    }
    else
    {
        return ret;
    }
}

VDEC_Result MApi_VDEC_V2_PreSetControl(VDEC_User_Cmd cmd_id, MS_U32 param)
{
    VDEC_Result ret = E_VDEC_OK;

    if (_Attr.bInit)
    {
        printf("VDEC Warn: PreSetControl should be called before VDEC Init\n");
        return E_VDEC_RET_ILLEGAL_ACCESS;
    }

    switch (cmd_id)
    {
        case VDEC_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE:
            _Pre_Ctrl.bOnePendingBuffer = (MS_BOOL)param;
            break;
        case VDEC_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE:
            _Pre_Ctrl.bHWBufferReMapping = (MS_BOOL)param;
            break;
        case VDEC_USER_CMD_FRAMERATE_HANDLING:
            _Pre_Ctrl.bFrameRateHandling = TRUE;
            _Pre_Ctrl.u32PreSetFrameRate = param;
            break;
        default:
            ret = E_VDEC_RET_ILLEGAL_ACCESS;
            break;
    }

    return ret;
}

