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
/// file    apiVDEC_wrap.c
/// @brief  wrapper to wrap VDEC_EX to VDEC API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "apiVDEC.h"
#include "apiVDEC_EX.h"

#include "drvMMIO.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define _VDEC_Memcpy(pDstAddr, pSrcAddr, u32Size)   memcpy((pDstAddr), (pSrcAddr), (u32Size))
#define _VDEC_Memset(pDstAddr, u8value, u32Size)    memset((pDstAddr), (u8value), (u32Size))

#define _VDEC_WRAP_NOT_INIT_RET()                            \
      do {                                                   \
            if (VDEC_U32_MAX == stStreamId.u32Id)            \
            {                                                \
                return E_VDEC_RET_NOT_INIT;                  \
            }                                                \
      } while(0)

#define _VDEC_WRAP_NOT_INIT_RET_ZERO()                       \
      do {                                                   \
            if (VDEC_U32_MAX == stStreamId.u32Id)            \
            {                                                \
                return 0;                                    \
            }                                                \
      } while(0)

#define VDEC_U32_MAX            0xffffffffUL

#if ((defined(CHIP_A1) || defined(CHIP_A7)) && defined (__aeon__))
#define VDEC_INFO(x)
#else
#define VDEC_INFO(x)    if (_eDbgMsgLevel >= E_VDEC_DBG_LEVEL_INFO) { (x); }
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

#ifndef MSOS_TYPE_NUTTX

//Use EX_MAIN stream for the original VDEC interface
static VDEC_EX_Stream eStType = E_VDEC_EX_MAIN_STREAM;

//StreamID for VDEC_EX interface
static VDEC_StreamId stStreamId = {
                                      0,            //u32Version
                                      VDEC_U32_MAX  //u32Id
                                  };

static VDEC_Info        _api_vdec_info;
#if !((defined(CHIP_A1) || defined(CHIP_A7)) && defined (__aeon__))
static VDEC_DbgLevel    _eDbgMsgLevel = E_VDEC_DBG_LEVEL_ERR;
#endif
static VDEC_InitParam   _vdecInitParam;

static VDEC_Result _VDEC_MapResult(VDEC_EX_Result eEXRet)
{
    VDEC_Result eRet = E_VDEC_FAIL;
    const VDEC_Result eRetMapTable[E_VDEC_EX_RET_NUM] =
    {
        E_VDEC_FAIL,
        E_VDEC_OK,
        E_VDEC_RET_INVALID_PARAM,
        E_VDEC_RET_ILLEGAL_ACCESS,
        E_VDEC_RET_HARDWARE_BREAKDOWN,
        E_VDEC_RET_UNSUPPORTED,
        E_VDEC_RET_TIMEOUT,
        E_VDEC_RET_NOT_READY,
        E_VDEC_RET_NOT_INIT,
        E_VDEC_RET_NOT_EXIT,
        E_VDEC_RET_NOT_RUNNING
    };

    if (eEXRet < E_VDEC_EX_RET_NUM)
    {
        eRet = eRetMapTable[eEXRet];
    }
    return eRet;
}

static void _VDEC_MapDispInfo(VDEC_EX_DispInfo* pExDisp, VDEC_DispInfo* pDisp)
{
    pDisp->u16HorSize        = pExDisp->u16HorSize;
    pDisp->u16VerSize        = pExDisp->u16VerSize;
    pDisp->u32FrameRate      = pExDisp->u32FrameRate;
    pDisp->u8Interlace       = pExDisp->u8Interlace;
    pDisp->u8AFD             = pExDisp->u8AFD;
    pDisp->u16SarWidth       = pExDisp->u16SarWidth;
    pDisp->u16SarHeight      = pExDisp->u16SarHeight;
    pDisp->u16CropRight      = pExDisp->u16CropRight;
    pDisp->u16CropLeft       = pExDisp->u16CropLeft;
    pDisp->u16CropBottom     = pExDisp->u16CropBottom;
    pDisp->u16CropTop        = pExDisp->u16CropTop;
    pDisp->u16Pitch          = pExDisp->u16Pitch;
    pDisp->u16PTSInterval    = pExDisp->u16PTSInterval;
    pDisp->u8MPEG1           = pExDisp->u8MPEG1;
    pDisp->u8PlayMode        = pExDisp->u8PlayMode;
    pDisp->u8FrcMode         = pExDisp->u8FrcMode;
    pDisp->u8AspectRate      = pExDisp->u8AspectRate;
    pDisp->bWithChroma       = pExDisp->bWithChroma;
    pDisp->bColorInXVYCC     = pExDisp->bColorInXVYCC;
    pDisp->u32DynScalingAddr = pExDisp->u32DynScalingAddr;
    pDisp->u32DynScalingSize = pExDisp->u32DynScalingSize;
    pDisp->u8DynScalingDepth = pExDisp->u8DynScalingDepth;
    pDisp->bEnableMIUSel   = pExDisp->bEnableMIUSel;
    pDisp->u32AspectWidth  = pExDisp->u32AspectWidth;
    pDisp->u32AspectHeight = pExDisp->u32AspectHeight;
}

static void _VDEC_MapExDecCmd(VDEC_DecCmd *pCmd, VDEC_EX_DecCmd *pExCmd)
{
    pExCmd->u32ID_H      = pCmd->u32ID_H;
    pExCmd->u32ID_L      = pCmd->u32ID_L;
    pExCmd->u32StAddr    = pCmd->u32StAddr;
    pExCmd->u32Size      = pCmd->u32Size;
    pExCmd->u32Timestamp = pCmd->u32Timestamp;
}

static VDEC_FrameType _VDEC_MapFrameType(VDEC_EX_FrameType eExFrameType)
{
    const VDEC_FrameType eFrmTypeMapTable[E_VDEC_EX_FRM_TYPE_NUM] =
    {
        E_VDEC_FRM_TYPE_I,
        E_VDEC_FRM_TYPE_P,
        E_VDEC_FRM_TYPE_B,
        E_VDEC_FRM_TYPE_OTHER
    };
    VDEC_FrameType eFrameType = eFrmTypeMapTable[eExFrameType];
    MS_ASSERT(eExFrameType < E_VDEC_EX_FRM_TYPE_NUM);
    return eFrameType;
}

static VDEC_FieldType _VDEC_MapFieldType(VDEC_EX_FieldType eExFieldType)
{
    const VDEC_FieldType eFieldTypeMapTable[E_VDEC_EX_FIELDTYPE_NUM] =
    {
        E_VDEC_FIELDTYPE_NONE,
        E_VDEC_FIELDTYPE_TOP,
        E_VDEC_FIELDTYPE_BOTTOM,
        E_VDEC_FIELDTYPE_BOTH
    };
    VDEC_FieldType eFieldType = eFieldTypeMapTable[eExFieldType];
    MS_ASSERT(eExFieldType < E_VDEC_EX_FIELDTYPE_NUM);
    return eFieldType;
}

static void _VDEC_MapFrameInfo(VDEC_EX_FrameInfo* pExFrmInfo, VDEC_FrameInfo* pFrmInfo)
{
    pFrmInfo->u32LumaAddr   = pExFrmInfo->u32LumaAddr  ;
    pFrmInfo->u32ChromaAddr = pExFrmInfo->u32ChromaAddr;
    pFrmInfo->u32TimeStamp  = pExFrmInfo->u32TimeStamp ;
    pFrmInfo->u32ID_L       = pExFrmInfo->u32ID_L;
    pFrmInfo->u32ID_H       = pExFrmInfo->u32ID_H;
    pFrmInfo->u16Pitch      = pExFrmInfo->u16Pitch;
    pFrmInfo->u16Width      = pExFrmInfo->u16Width;
    pFrmInfo->u16Height     = pExFrmInfo->u16Height;
    pFrmInfo->eFrameType    = _VDEC_MapFrameType(pExFrmInfo->eFrameType);
    pFrmInfo->eFieldType    = _VDEC_MapFieldType(pExFrmInfo->eFieldType);
}

static void _VDEC_MapExtDispInfo(VDEC_EX_ExtDispInfo* pExFrmInfo, VDEC_ExtDispInfo* pFrmInfo)
{
    pFrmInfo->u16VSize = pExFrmInfo->u16VSize;
    pFrmInfo->u16HSize = pExFrmInfo->u16HSize;
    pFrmInfo->s16HOffset= pExFrmInfo->s16HOffset;
    pFrmInfo->s16VOffset= pExFrmInfo->s16VOffset;
}

static VDEC_TrickDec _VDEC_MapTrickDecMode(VDEC_EX_TrickDec eExTrickDec)
{
    const VDEC_TrickDec eTrickDecMapTable[E_VDEC_EX_TRICK_DEC_NUM] =
    {
        E_VDEC_TRICK_DEC_ALL,
        E_VDEC_TRICK_DEC_IP,
        E_VDEC_TRICK_DEC_I
    };
    VDEC_TrickDec eTrickDec = eTrickDecMapTable[eExTrickDec];
    MS_ASSERT(eExTrickDec < E_VDEC_EX_TRICK_DEC_NUM);
    return eTrickDec;
}

static VDEC_EX_TrickDec _VDEC_MapExTrickDecMode(VDEC_TrickDec eTrickDec)
{
#if 1
    VDEC_EX_TrickDec eExTrickDec = E_VDEC_EX_TRICK_DEC_ALL;

    switch (eTrickDec)
    {
        case E_VDEC_TRICK_DEC_ALL:
            eExTrickDec = E_VDEC_EX_TRICK_DEC_ALL;
            break;
        case E_VDEC_TRICK_DEC_IP:
            eExTrickDec = E_VDEC_EX_TRICK_DEC_IP;
            break;
        case E_VDEC_TRICK_DEC_I:
            eExTrickDec = E_VDEC_EX_TRICK_DEC_I;
            break;
        default:
            eExTrickDec = E_VDEC_EX_TRICK_DEC_ALL;
            MS_ASSERT(0);
            break;
    }

    return eExTrickDec;
#else
    const VDEC_EX_TrickDec eExTrickDecMapTable[E_VDEC_EX_TRICK_DEC_NUM] =
    {
        E_VDEC_EX_TRICK_DEC_ALL,
        E_VDEC_EX_TRICK_DEC_IP,
        E_VDEC_EX_TRICK_DEC_I
    };
    VDEC_EX_TrickDec eExTrickDec = eExTrickDecMapTable[eTrickDec];
    MS_ASSERT(eTrickDec < E_VDEC_EX_TRICK_DEC_NUM);
    return eExTrickDec;
#endif
}

static VDEC_EX_FreezePicSelect _VDEC_MapExFreezePicSel(VDEC_FreezePicSelect ePicSel)
{
    VDEC_EX_FreezePicSelect eExPicSel = E_VDEC_EX_FREEZE_AT_CUR_PIC;

    switch (ePicSel)
    {
        case E_VDEC_FREEZE_AT_CUR_PIC:
            eExPicSel = E_VDEC_EX_FREEZE_AT_CUR_PIC;
            break;

        case E_VDEC_FREEZE_AT_LAST_PIC:
            eExPicSel = E_VDEC_EX_FREEZE_AT_LAST_PIC;
            break;
        default:
            eExPicSel = E_VDEC_EX_FREEZE_AT_LAST_PIC;
            MS_ASSERT(0);
            break;
    }
    return eExPicSel;
}

static VDEC_EX_SpeedType _VDEC_MapExSpeedType(VDEC_SpeedType eSpeedType)
{
    VDEC_EX_SpeedType eExSpeedType = E_VDEC_EX_SPEED_DEFAULT;

    switch (eSpeedType)
    {
        case E_VDEC_SPEED_DEFAULT:
            eExSpeedType = E_VDEC_EX_SPEED_DEFAULT;
            break;
        case E_VDEC_SPEED_FAST:
            eExSpeedType = E_VDEC_EX_SPEED_FAST;
            break;
        case E_VDEC_SPEED_SLOW:
            eExSpeedType = E_VDEC_EX_SPEED_SLOW;
            break;
        default:
            eExSpeedType = E_VDEC_EX_SPEED_DEFAULT;
            MS_ASSERT(0);
            break;
    }

    return eExSpeedType;
}

static VDEC_EX_DispSpeed _VDEC_MapExDispSpeed(VDEC_DispSpeed eSpeed)
{
    VDEC_EX_DispSpeed eExSpeed = E_VDEC_EX_DISP_SPEED_1X;

    switch (eSpeed)
    {
        case E_VDEC_DISP_SPEED_1X:
            eExSpeed = E_VDEC_EX_DISP_SPEED_1X;
            break;
        case E_VDEC_DISP_SPEED_2X:
            eExSpeed = E_VDEC_EX_DISP_SPEED_2X;
            break;
        case E_VDEC_DISP_SPEED_4X :
            eExSpeed = E_VDEC_EX_DISP_SPEED_4X;
            break;
        case E_VDEC_DISP_SPEED_8X :
            eExSpeed = E_VDEC_EX_DISP_SPEED_8X;
            break;
        case E_VDEC_DISP_SPEED_16X:
            eExSpeed = E_VDEC_EX_DISP_SPEED_16X;
            break;
        case E_VDEC_DISP_SPEED_32X:
            eExSpeed = E_VDEC_EX_DISP_SPEED_32X;
            break;
        default:
            eExSpeed = E_VDEC_EX_DISP_SPEED_1X;
            MS_ASSERT(0);
            break;
    }

    return eExSpeed;
}

static void _VDEC_MapTimeCode(VDEC_EX_TimeCode* pstExTimeCode, VDEC_TimeCode* pstTimeCode)
{
    pstTimeCode->u8TimeCodeHr  = pstExTimeCode->u8TimeCodeHr;
    pstTimeCode->u8TimeCodeMin = pstExTimeCode->u8TimeCodeMin;
    pstTimeCode->u8TimeCodeSec = pstExTimeCode->u8TimeCodeSec;
    pstTimeCode->u8TimeCodePic = pstExTimeCode->u8TimeCodePic;
    pstTimeCode->u8DropFrmFlag = pstExTimeCode->u8DropFrmFlag;
}

static VDEC_EX_PatternType _VDEC_MapExPatternType(VDEC_PatternType ePatternType)
{
    VDEC_EX_PatternType eExPatternType = E_VDEC_EX_PATTERN_FLUSH;

    if (ePatternType == E_VDEC_PATTERN_FLUSH)
    {
        eExPatternType = E_VDEC_EX_PATTERN_FLUSH;
    }
    else if (ePatternType == E_VDEC_PATTERN_FILEEND)
    {
        eExPatternType = E_VDEC_EX_PATTERN_FILEEND;
    }
    else
    {
        MS_ASSERT(0);
    }

    return eExPatternType;
}

static void _VDEC_MapStatus(VDEC_EX_Status* pExStatus, VDEC_Status* pStatus)
{
    pStatus->bInit  = pExStatus->bInit;
    pStatus->bIdle  = pExStatus->bIdle;
    switch (pExStatus->eStage)
    {
        case E_VDEC_EX_STAGE_STOP:  pStatus->eStage = E_VDEC_STAGE_STOP; break;
        case E_VDEC_EX_STAGE_INIT:  pStatus->eStage = E_VDEC_STAGE_INIT; break;
        case E_VDEC_EX_STAGE_PLAY:  pStatus->eStage = E_VDEC_STAGE_PLAY; break;
        case E_VDEC_EX_STAGE_PAUSE: pStatus->eStage = E_VDEC_STAGE_PAUSE; break;
        default:    pStatus->eStage = E_VDEC_STAGE_STOP; break;
    }
    return;
}

static VDEC_CodecType _VDEC_MapCodecType(VDEC_EX_CodecType eExCodecType)
{
    VDEC_CodecType eCodecType = E_VDEC_CODEC_TYPE_NONE;

    const VDEC_CodecType eCodecTypeMapTable[E_VDEC_EX_CODEC_TYPE_NUM] =
    {
        E_VDEC_CODEC_TYPE_NONE,
        E_VDEC_CODEC_TYPE_MPEG2,
        E_VDEC_CODEC_TYPE_H263,
        E_VDEC_CODEC_TYPE_MPEG4,
        E_VDEC_CODEC_TYPE_DIVX311,
        E_VDEC_CODEC_TYPE_DIVX412,
        E_VDEC_CODEC_TYPE_FLV,
        E_VDEC_CODEC_TYPE_VC1_ADV,
        E_VDEC_CODEC_TYPE_VC1_MAIN,
        E_VDEC_CODEC_TYPE_RV8,
        E_VDEC_CODEC_TYPE_RV9,
        E_VDEC_CODEC_TYPE_H264,
        E_VDEC_CODEC_TYPE_AVS,
        E_VDEC_CODEC_TYPE_MJPEG,
        E_VDEC_CODEC_TYPE_MVC,
        E_VDEC_CODEC_TYPE_VP8,
        E_VDEC_CODEC_TYPE_HEVC,
    };

    if (eExCodecType < E_VDEC_EX_CODEC_TYPE_NUM)
    {
        eCodecType = eCodecTypeMapTable[eExCodecType];
    }

    return eCodecType;
}

static VDEC_EX_CodecType _VDEC_MapExCodecType(VDEC_CodecType eCodecType)
{
    VDEC_EX_CodecType eExCodecType = E_VDEC_EX_CODEC_TYPE_NONE;

    const VDEC_EX_CodecType eExCodecTypeMapTable[E_VDEC_EX_CODEC_TYPE_NUM] =
    {
        E_VDEC_EX_CODEC_TYPE_NONE,
        E_VDEC_EX_CODEC_TYPE_MPEG2,
        E_VDEC_EX_CODEC_TYPE_H263,
        E_VDEC_EX_CODEC_TYPE_MPEG4,
        E_VDEC_EX_CODEC_TYPE_DIVX311,
        E_VDEC_EX_CODEC_TYPE_DIVX412,
        E_VDEC_EX_CODEC_TYPE_FLV,
        E_VDEC_EX_CODEC_TYPE_VC1_ADV,
        E_VDEC_EX_CODEC_TYPE_VC1_MAIN,
        E_VDEC_EX_CODEC_TYPE_RV8,
        E_VDEC_EX_CODEC_TYPE_RV9,
        E_VDEC_EX_CODEC_TYPE_H264,
        E_VDEC_EX_CODEC_TYPE_AVS,
        E_VDEC_EX_CODEC_TYPE_MJPEG,
        E_VDEC_EX_CODEC_TYPE_MVC,
        E_VDEC_EX_CODEC_TYPE_VP8,
        E_VDEC_EX_CODEC_TYPE_HEVC,
    };

    if (eExCodecType < E_VDEC_EX_CODEC_TYPE_NUM)
    {
        eExCodecType = eExCodecTypeMapTable[eCodecType];
    }

    return eExCodecType;
}

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_DbgCmd(MS_U32 u8Cmd, MS_U32 u32CmdArg)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_DbgCmd(&stStreamId, u8Cmd, u32CmdArg);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_DbgSetData(MS_VIRT u32Addr, MS_U32 u32Data)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_DbgSetData(&stStreamId, u32Addr, u32Data);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_DbgGetData(MS_VIRT u32Addr, MS_U32 *u32Data)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_DbgGetData(&stStreamId, u32Addr, u32Data);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Get VDEC version
/// @return -the pointer to the VDEC version
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetLibVer(const MSIF_Version **ppVersion)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    if (!ppVersion)
    {
        return eRetVal;
    }

    eEXRetVal = MApi_VDEC_EX_GetLibVer(ppVersion);
    eRetVal = _VDEC_MapResult(eEXRetVal);
    return eRetVal;
}

//-----------------------------------------------------------------------------
/// Get VDEC info
/// @brief \b Function \b Description:  Get information of VDEC API
/// @return - the pointer to the VDEC information
//-----------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetInfo(const VDEC_Info **ppInfo)
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
    MS_BOOL bRet = FALSE;
    VDEC_EX_Status stExStatus;

    if (VDEC_U32_MAX == stStreamId.u32Id)
    {
        return FALSE;
    }

    if (pStatus)
    {
        bRet = MApi_VDEC_EX_GetStatus(&stStreamId, &stExStatus);
        _VDEC_MapStatus(&stExStatus, pStatus);
    }

    return bRet;
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
    VDEC_EX_CodecType eExCodecType = _VDEC_MapExCodecType(eCodecType);

    if (VDEC_U32_MAX == stStreamId.u32Id)
    {
        return FALSE;
    }

    return MApi_VDEC_EX_CheckCaps(&stStreamId, eExCodecType);
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
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;

    eEXRetVal = MApi_VDEC_EX_EnableTurboMode(bEnable);
    eRetVal = _VDEC_MapResult(eEXRetVal);
    return eRetVal;
}

#ifdef _MVD_EVENT_DBG
MS_U32 u32Para = 111;
extern MS_U8 HAL_MVD_GetAVSyncStatus(MS_U8 u8Idx);
void VDEC_TestVSync(MS_U32 eFlag, void *param)
{
    if ((E_VDEC_EVENT_DISP_ONE & eFlag) == E_VDEC_EVENT_DISP_ONE)
    {
        printf("SyncStat=0x%x, diff=%lld\n", HAL_MVD_GetAVSyncStatus(0), MApi_VDEC_GetVideoPtsStcDelta());
        //MApi_VDEC_IsFreerun();
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
                    printf("%02d, ", ccInfo.u16TempRef);
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

static VDEC_EX_FWSourceType _VDEC_MapFWSourceType(VDEC_FWSourceType eFWSrc)
{
    VDEC_EX_FWSourceType eExFWSrc = E_VDEC_EX_FW_SOURCE_NONE;

    switch (eFWSrc)
    {
        case E_VDEC_FW_SOURCE_DRAM:
            eExFWSrc = E_VDEC_EX_FW_SOURCE_DRAM;
            break;
        case E_VDEC_FW_SOURCE_FLASH:
            eExFWSrc = E_VDEC_EX_FW_SOURCE_FLASH;
            break;
        case E_VDEC_FW_SOURCE_NONE:
        default:
            eExFWSrc = E_VDEC_EX_FW_SOURCE_NONE;
            break;
    }

    return eExFWSrc;
}

static VDEC_EX_CCFormat _VDEC_MapCCFormat(VDEC_CCFormat eCCFmt)
{
    VDEC_EX_CCFormat eExCCFmt = E_VDEC_EX_CC_NONE;

    switch (eCCFmt)
    {
        case E_VDEC_CC_608:
            eExCCFmt = E_VDEC_EX_CC_608;
            break;
        case E_VDEC_CC_708:
            eExCCFmt = E_VDEC_EX_CC_708;
            break;
        case E_VDEC_CC_UNPACKED:
            eExCCFmt = E_VDEC_EX_CC_608;
            break;
        case E_VDEC_CC_NONE:
        default:
            eExCCFmt = E_VDEC_EX_CC_NONE;
            break;
    }

    return eExCCFmt;
}

static VDEC_EX_CCType _VDEC_MapCCType(VDEC_CCType eCCType)
{
    VDEC_EX_CCType eExCCType = E_VDEC_EX_CC_TYPE_NONE;

    switch (eCCType)
    {
        case E_VDEC_CC_TYPE_NTSC_FIELD1:
            eExCCType = E_VDEC_EX_CC_TYPE_NTSC_FIELD1;
            break;
        case E_VDEC_CC_TYPE_NTSC_FIELD2:
            eExCCType = E_VDEC_EX_CC_TYPE_NTSC_FIELD2;
            break;
        case E_VDEC_CC_TYPE_DTVCC:
            eExCCType = E_VDEC_EX_CC_TYPE_DTVCC;
            break;
        case E_VDEC_CC_TYPE_NTSC_TWOFIELD:
            eExCCType = E_VDEC_EX_CC_TYPE_NTSC_TWOFIELD;
            break;
        case E_VDEC_CC_TYPE_NONE:
        default:
            eExCCType = E_VDEC_EX_CC_TYPE_NONE;
            break;
    }

    return eExCCType;
}

static VDEC_EX_CCInfoCmd _VDEC_MapCCInfoCmd(VDEC_CCInfoCmd eCCInfoCmd)
{
    VDEC_EX_CCInfoCmd eExCCInfoCmd = E_VDEC_EX_CC_GET_BUFF_START;

    switch (eCCInfoCmd)
    {
        case E_VDEC_CC_GET_BUFF_START:
            eExCCInfoCmd = E_VDEC_EX_CC_GET_BUFF_START;
            break;
        case E_VDEC_CC_GET_BUFF_SIZE:
            eExCCInfoCmd = E_VDEC_EX_CC_GET_BUFF_SIZE;
            break;
        case E_VDEC_CC_GET_708_ENABLE:
        default:
            eExCCInfoCmd = E_VDEC_EX_CC_GET_708_ENABLE;
            break;
    }

    return eExCCInfoCmd;
}

static VDEC_EX_DbgLevel _VDEC_MapDbgLevel(VDEC_DbgLevel eDbg)
{
    VDEC_EX_DbgLevel  eExDbg = E_VDEC_EX_DBG_LEVEL_NONE;

    switch (eDbg)
    {
        case E_VDEC_DBG_LEVEL_ERR:
            eExDbg = E_VDEC_EX_DBG_LEVEL_ERR;
            break;
        case E_VDEC_DBG_LEVEL_INFO:
            eExDbg = E_VDEC_EX_DBG_LEVEL_INFO;
            break;
        case E_VDEC_DBG_LEVEL_DBG:
            eExDbg = E_VDEC_EX_DBG_LEVEL_DBG;
            break;
        case E_VDEC_DBG_LEVEL_TRACE:
            eExDbg = E_VDEC_EX_DBG_LEVEL_TRACE;
            break;
        case E_VDEC_DBG_LEVEL_FW:
            eExDbg = E_VDEC_EX_DBG_LEVEL_FW;
            break;
        case E_VDEC_DBG_LEVEL_NONE:
        default:
            eExDbg = E_VDEC_EX_DBG_LEVEL_NONE;
            break;
    }
    return eExDbg;
}

static void _VDEC_MapSysCfg(VDEC_SysCfg* pstSys, VDEC_EX_SysCfg* pstExSys)
{
    pstExSys->u32FWBinaryAddr     = pstSys->u32FWBinaryAddr;
    pstExSys->u32FWBinarySize     = pstSys->u32FWBinarySize;
    pstExSys->u32CodeBufAddr      = pstSys->u32CodeBufAddr;
    pstExSys->u32CodeBufSize      = pstSys->u32CodeBufSize;
    pstExSys->u32FrameBufAddr     = pstSys->u32FrameBufAddr;
    pstExSys->u32FrameBufSize     = pstSys->u32FrameBufSize;
    pstExSys->u32BitstreamBufAddr = pstSys->u32BitstreamBufAddr;
    pstExSys->u32BitstreamBufSize = pstSys->u32BitstreamBufSize;
    pstExSys->u32DrvProcBufAddr   = pstSys->u32DrvProcBufAddr;
    pstExSys->u32DrvProcBufSize   = pstSys->u32DrvProcBufSize;
    pstExSys->u32VlcBinarySrcAddr = pstSys->u32VlcBinarySrcAddr;
    pstExSys->u32VlcTabBinarySize = pstSys->u32VlcTabBinarySize;
    pstExSys->eDbgMsgLevel        = _VDEC_MapDbgLevel(pstSys->eDbgMsgLevel);
    pstExSys->eFWSourceType       = _VDEC_MapFWSourceType(pstSys->eFWSourceType);
}

static VDEC_EX_SrcMode _VDEC_MapSrcMode(VDEC_SrcMode eSrcMode)
{
    VDEC_EX_SrcMode eExSrcMode = E_VDEC_EX_SRC_MODE_DTV;

    switch (eSrcMode)
    {
        case E_VDEC_SRC_MODE_DTV:
            eExSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            break;
        case E_VDEC_SRC_MODE_TS_FILE:
            eExSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE;
            break;
        case E_VDEC_SRC_MODE_FILE:
            eExSrcMode = E_VDEC_EX_SRC_MODE_FILE;
            break;
        case E_VDEC_SRC_MODE_TS_FILE_DUAL_ES:
            eExSrcMode = E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES;
            break;
        case E_VDEC_SRC_MODE_FILE_DUAL_ES:
            eExSrcMode = E_VDEC_EX_SRC_MODE_FILE_DUAL_ES;
            break;
        default:
            VDEC_INFO(printf("input eSrcMode 0x%x out of range!\n", eSrcMode));
            eExSrcMode = E_VDEC_EX_SRC_MODE_DTV;
            break;
    }
    return eExSrcMode;
}

static VDEC_EX_TimeStampType _VDEC_MapTimeStampType(VDEC_TimeStampType eTimeStamp)
{
    VDEC_EX_TimeStampType eExTimeStamp = E_VDEC_EX_TIME_STAMP_NONE;

    switch (eTimeStamp)
    {
        case E_VDEC_TIME_STAMP_PTS:
            eExTimeStamp = E_VDEC_EX_TIME_STAMP_PTS;
            break;
        case E_VDEC_TIME_STAMP_DTS:
            eExTimeStamp = E_VDEC_EX_TIME_STAMP_DTS;
            break;
        case E_VDEC_TIME_STAMP_STS:
            eExTimeStamp = E_VDEC_EX_TIME_STAMP_STS;
            break;
        case E_VDEC_TIME_STAMP_NONE:
        default:
           eExTimeStamp = E_VDEC_EX_TIME_STAMP_NONE;
            break;
    }
    return eExTimeStamp;
}

static VDEC_EX_MJpegScaleFactor _VDEC_MapMjpegScaleFactor(VDEC_MJpegScaleFactor eMjScaleFactor)
{
    VDEC_EX_MJpegScaleFactor eExMjScaleFactor = E_VDEC_EX_MJPEG_SCALE_1to1;

    switch (eMjScaleFactor)
    {
        case E_VDEC_MJPEG_SCALE_1to1:
            eExMjScaleFactor = E_VDEC_EX_MJPEG_SCALE_1to1;
            break;
        case E_VDEC_MJPEG_SCALE_2to1:
            eExMjScaleFactor = E_VDEC_EX_MJPEG_SCALE_2to1;
            break;
        case E_VDEC_MJPEG_SCALE_4to1:
            eExMjScaleFactor = E_VDEC_EX_MJPEG_SCALE_4to1;
            break;
        case E_VDEC_MJPEG_SCALE_8to1:
            eExMjScaleFactor = E_VDEC_EX_MJPEG_SCALE_8to1;
            break;
        default:
            eExMjScaleFactor = E_VDEC_EX_MJPEG_SCALE_1to1;
            break;
    }
    return eExMjScaleFactor;
}

static void _VDEC_MapVideoInfo(VDEC_VideoInfo* pstVidInfo, VDEC_EX_VideoInfo* pstExVidInfo)
{
    MS_U8 i = 0;

    pstExVidInfo->eSrcMode           = _VDEC_MapSrcMode(pstVidInfo->eSrcMode);
    pstExVidInfo->eTimeStampType     = _VDEC_MapTimeStampType(pstVidInfo->eTimeStampType);
    pstExVidInfo->eMJpegScaleFactor  = _VDEC_MapMjpegScaleFactor(pstVidInfo->eMJpegScaleFactor);
    pstExVidInfo->bWithoutNalStCode  = pstVidInfo->bWithoutNalStCode;
    pstExVidInfo->u32FrameRate       = pstVidInfo->u32FrameRate;
    pstExVidInfo->u32FrameRateBase   = pstVidInfo->u32FrameRateBase;
    pstExVidInfo->u16NumSizes        = pstVidInfo->u16NumSizes;

    for (i = 0; i < 8; i++)
    {
        pstExVidInfo->u16Width[i]   = pstVidInfo->u16Width[i];
        pstExVidInfo->u16Height[i]  = pstVidInfo->u16Height[i];
    }
}

static MS_BOOL _VDEC_MapInitParam(VDEC_InitParam* pstInitPara, VDEC_EX_InitParam* pstExInitPara)
{
    pstExInitPara->u32Version = 0;
    pstExInitPara->eCodecType = _VDEC_MapExCodecType(pstInitPara->eCodecType);
    _VDEC_MapSysCfg(&pstInitPara->SysConfig, &pstExInitPara->SysConfig);
    _VDEC_MapVideoInfo(&pstInitPara->VideoInfo, &pstExInitPara->VideoInfo);
    pstExInitPara->EnableDynaScale       = pstInitPara->EnableDynaScale;
    pstExInitPara->bDisableDropErrFrame  = pstInitPara->bDisableDropErrFrame;
    pstExInitPara->bDisableErrConceal    = pstInitPara->bDisableErrConceal;
    pstExInitPara->bRepeatLastField      = pstInitPara->bRepeatLastField;
    pstExInitPara->u8ErrThreshold        = pstInitPara->u8ErrThreshold;
    pstExInitPara->u32DSVirtualBoxWidth  = pstInitPara->u32DSVirtualBoxWidth;
    pstExInitPara->u32DSVirtualBoxHeight = pstInitPara->u32DSVirtualBoxHeight ;
    pstExInitPara->stFBReduction.eLumaFBReduction = VDEC_EX_FB_REDUCTION_NONE;
    pstExInitPara->stFBReduction.eChromaFBReduction = VDEC_EX_FB_REDUCTION_NONE;
    pstExInitPara->stFBReduction.bEnableAutoMode = FALSE;
    return TRUE;
}

static VDEC_EX_FBReductionType _VDEC_MapFBReductionType(VDEC_FBReductionType eRedType)
{
    VDEC_EX_FBReductionType eExRedType = VDEC_EX_FB_REDUCTION_NONE;
    switch (eRedType)
    {
        case VDEC_FB_REDUCTION_1_2:
            eExRedType = VDEC_EX_FB_REDUCTION_1_2;
            break;
        case VDEC_FB_REDUCTION_1_4:
            eExRedType = VDEC_EX_FB_REDUCTION_1_4;
            break;
        case VDEC_FB_REDUCTION_NONE:
        default:
            eExRedType = VDEC_EX_FB_REDUCTION_NONE;
            break;
    }
    return eExRedType;
}

static MS_BOOL _VDEC_MapInitParam_EX1(VDEC_InitParam_EX1* pstInitPara, VDEC_EX_InitParam* pstExInitPara)
{
    pstExInitPara->u32Version = 0;
    pstExInitPara->eCodecType = _VDEC_MapExCodecType(pstInitPara->eCodecType);
    _VDEC_MapSysCfg(&pstInitPara->SysConfig, &pstExInitPara->SysConfig);
    _VDEC_MapVideoInfo(&pstInitPara->VideoInfo, &pstExInitPara->VideoInfo);
    pstExInitPara->EnableDynaScale       = pstInitPara->EnableDynaScale;
    pstExInitPara->bDisableDropErrFrame  = pstInitPara->bDisableDropErrFrame;
    pstExInitPara->bDisableErrConceal    = pstInitPara->bDisableErrConceal;
    pstExInitPara->bRepeatLastField      = pstInitPara->bRepeatLastField;
    pstExInitPara->u8ErrThreshold        = pstInitPara->u8ErrThreshold;
    pstExInitPara->u32DSVirtualBoxWidth  = pstInitPara->u32DSVirtualBoxWidth;
    pstExInitPara->u32DSVirtualBoxHeight = pstInitPara->u32DSVirtualBoxHeight ;
    pstExInitPara->stFBReduction.eLumaFBReduction = _VDEC_MapFBReductionType(pstInitPara->stFBReduction.eLumaFBReduction);
    pstExInitPara->stFBReduction.eChromaFBReduction = _VDEC_MapFBReductionType(pstInitPara->stFBReduction.eChromaFBReduction);
    pstExInitPara->stFBReduction.bEnableAutoMode = pstInitPara->stFBReduction.bEnableAutoMode;
    return TRUE;
}

static VDEC_EX_User_Cmd _VDEC_MapUserCmd(VDEC_User_Cmd eUserCmd)
{
    VDEC_EX_User_Cmd eEXUserCmd = E_VDEC_EX_USER_CMD_SET_CONTROL_BASE;
    switch (eUserCmd)
    {
        case VDEC_USER_CMD_SET_CONTROL_BASE             : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_CONTROL_BASE; break;
        case VDEC_USER_CMD_REPEAT_LAST_FIELD            : eEXUserCmd = E_VDEC_EX_USER_CMD_REPEAT_LAST_FIELD; break;
        case VDEC_USER_CMD_AVSYNC_REPEAT_TH             : eEXUserCmd = E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH; break;
        case VDEC_USER_CMD_DISP_ONE_FIELD               : eEXUserCmd = E_VDEC_EX_USER_CMD_DISP_ONE_FIELD; break;
        case VDEC_USER_CMD_FD_MASK_DELAY_COUNT          : eEXUserCmd = E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT; break;
        case VDEC_USER_CMD_FRC_OUTPUT                   : eEXUserCmd = E_VDEC_EX_USER_CMD_FRC_OUTPUT; break;
        case VDEC_USER_CMD_FRC_DROP_TYPE                : eEXUserCmd = E_VDEC_EX_USER_CMD_FRC_DROP_TYPE; break;
        case VDEC_USER_CMD_FAST_DISPLAY                 : eEXUserCmd = E_VDEC_EX_USER_CMD_FAST_DISPLAY; break;
        case VDEC_USER_CMD_IGNORE_ERR_REF               : eEXUserCmd = E_VDEC_EX_USER_CMD_IGNORE_ERR_REF; break;
        case VDEC_USER_CMD_FORCE_FOLLOW_DTV_SPEC        : eEXUserCmd = E_VDEC_EX_USER_CMD_FORCE_FOLLOW_DTV_SPEC; break;
        case VDEC_USER_CMD_AVC_MIN_FRM_GAP              : eEXUserCmd = E_VDEC_EX_USER_CMD_AVC_MIN_FRM_GAP; break;
        case VDEC_USER_CMD_DISABLE_SEQ_CHG              : eEXUserCmd = E_VDEC_EX_USER_CMD_DISABLE_SEQ_CHG; break;
        case VDEC_USER_CMD_AVC_DISABLE_ANTI_VDEAD       : eEXUserCmd = E_VDEC_EX_USER_CMD_AVC_DISABLE_ANTI_VDEAD; break;
        case VDEC_USER_CMD_DTV_RESET_MVD_PARSER         : eEXUserCmd = E_VDEC_EX_USER_CMD_DTV_RESET_MVD_PARSER; break;
        case VDEC_USER_CMD_PVR_FLUSH_FRAME_BUFFER       : eEXUserCmd = E_VDEC_EX_USER_CMD_PVR_FLUSH_FRAME_BUFFER; break;
        case VDEC_USER_CMD_SET_DTV_USER_DATA_MODE       : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_DTV_USER_DATA_MODE; break;
        case VDEC_USER_CMD_SET_SINGLE_TASK_MODE         : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_SINGLE_TASK_MODE; break;
        case VDEC_USER_CMD_FORCE_INTERLACE_MODE         : eEXUserCmd = E_VDEC_EX_USER_CMD_FORCE_INTERLACE_MODE; break;
        case VDEC_USER_CMD_RELEASE_FD_MASK              : eEXUserCmd = E_VDEC_EX_USER_CMD_RELEASE_FD_MASK; break;
        case VDEC_USER_CMD_SET_DECODE_MODE              : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_DECODE_MODE; break;
        case VDEC_USER_CMD_SHOW_DECODE_ORDER            : eEXUserCmd = E_VDEC_EX_USER_CMD_SHOW_DECODE_ORDER; break;
        case VDEC_USER_CMD_SUPPORT_AVC_TO_MVC           : eEXUserCmd = E_VDEC_EX_USER_CMD_SUPPORT_AVC_TO_MVC; break;
        case VDEC_USER_CMD_3DLR_VIEW_EXCHANGE           : eEXUserCmd = E_VDEC_EX_USER_CMD_3DLR_VIEW_EXCHANGE; break;
        case VDEC_USER_CMD_FORCE_AUTO_MUTE              : eEXUserCmd = E_VDEC_EX_USER_CMD_FORCE_AUTO_MUTE; break;
        case VDEC_USER_CMD_SET_VSIZE_ALIGN              : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_VSIZE_ALIGN; break;
        case VDEC_USER_CMD_AVC_DISP_IGNORE_CROP         : eEXUserCmd = E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP; break;
        case VDEC_USER_CMD_SET_DISP_FINISH_MODE         : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_DISP_FINISH_MODE; break;
        case VDEC_USER_CMD_SET_AVSYNC_MODE              : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_AVSYNC_MODE; break;
        case VDEC_USER_CMD_SET_CC608_INFO_ENHANCE_MODE  : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_CC608_INFO_ENHANCE_MODE; break;
        case VDEC_USER_CMD_PUSH_DISPQ_WITH_REF_NUM      : eEXUserCmd = E_VDEC_EX_USER_CMD_PUSH_DISPQ_WITH_REF_NUM; break;
        case VDEC_USER_CMD_SET_IDCT_MODE                : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_IDCT_MODE; break;
        case VDEC_USER_CMD_SHOW_FIRST_FRAME_DIRECT      : eEXUserCmd = E_VDEC_EX_USER_CMD_SHOW_FIRST_FRAME_DIRECT; break;
        case VDEC_USER_CMD_SET_XC_LOW_DELAY_PARA        : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_XC_LOW_DELAY_PARA; break;
        case VDEC_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE  : eEXUserCmd = E_VDEC_EX_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE; break;
        case VDEC_USER_CMD_FRAMEBUFFER_AUTO_MODE        : eEXUserCmd = E_VDEC_EX_USER_CMD_FRAMEBUFFER_AUTO_MODE; break;
        case VDEC_USER_CMD_AUTO_DROP_DISPLAY_QUEUE      : eEXUserCmd = E_VDEC_EX_USER_CMD_AUTO_DROP_DISPLAY_QUEUE; break;
        case VDEC_USER_CMD_SET_PTS_US_MODE              : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_PTS_US_MODE; break;
        case VDEC_USER_CMD_FORCE_PROGRESSIVE_MODE       : eEXUserCmd = E_VDEC_EX_USER_CMD_FORCE_PROGRESSIVE_MODE; break;

        case VDEC_USER_CMD_MVC_BBU2_PUSH_PACKET         : eEXUserCmd = E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET; break;
        case VDEC_USER_CMD_MVC_BBU2_FIRE_DECCMD         : eEXUserCmd = E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD; break;

        //Group2:Get Control command
        case VDEC_USER_CMD_GET_CONTROL_BASE             : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_CONTROL_BASE; break;
        case VDEC_USER_CMD_GET_CHROMA_TYPE              : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_CHROMA_TYPE; break;
        case VDEC_USER_CMD_GET_REAL_FRAMERATE           : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_REAL_FRAMERATE; break;
        case VDEC_USER_CMD_GET_COLOR_MATRIX             : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_COLOR_MATRIX; break;
        case VDEC_USER_CMD_GET_DYNSCALE_ENABLED         : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_DYNSCALE_ENABLED; break;
        case VDEC_USER_CMD_GET_ORI_INTERLACE_MODE       : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_ORI_INTERLACE_MODE; break;
        case VDEC_USER_CMD_GET_FPA_SEI                  : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_FPA_SEI; break;
        case VDEC_USER_CMD_GET_U64PTS                   : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_U64PTS; break;
        //case VDEC_USER_CMD_GET_FB_USAGE_MEM             : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_FB_USAGE_MEM; break;
        case VDEC_USER_CMD_GET_FW_STATUS_FLAG           : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_FW_STATUS_FLAG; break;
        case VDEC_USER_CMD_GET_DS_BUF_MIU_SEL           : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_DS_BUF_MIU_SEL; break;

        case VDEC_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO  : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO; break;
        case VDEC_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY    : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY; break;
        case VDEC_USER_CMD_GET_MVC_ES2_READ_PTR         : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_MVC_ES2_READ_PTR; break;
        case VDEC_USER_CMD_GET_MVC_ES2_WRITE_PTR        : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_MVC_ES2_WRITE_PTR; break;
        case VDEC_USER_CMD_GET_ES_QUANTITY              : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_ES_QUANTITY; break;
        case VDEC_USER_CMD_GET_ES2_QUANTITY             : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_ES2_QUANTITY; break;
        case VDEC_USER_CMD_DROP_ERR_FRAME               : eEXUserCmd = E_VDEC_EX_USER_CMD_DROP_ERR_FRAME; break;
        case VDEC_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM   : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM ; break;
        case VDEC_USER_CMD_SET_MIN_TSP_DATA_SIZE        : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_MIN_TSP_DATA_SIZE; break;

        //Group3:System Preset Control command======================
        //Group3-1:Common system Preset Control command
        case VDEC_USER_CMD_SYSTEM_PRESET_CONTROL_BASE   : eEXUserCmd = E_VDEC_EX_USER_CMD_SYSTEM_PRESET_CONTROL_BASE; break;
        case VDEC_USER_CMD_SYSTEM_PRESET_VPU_CLOCK      : eEXUserCmd = E_VDEC_EX_USER_CMD_SYSTEM_PRESET_VPU_CLOCK; break;

        //Group3-2:HVD System Preset Control command
        case VDEC_USER_CMD_SYSTEM_PRESET_HVD_BASE       : eEXUserCmd = E_VDEC_EX_USER_CMD_SYSTEM_PRESET_HVD_BASE; break;
        case VDEC_USER_CMD_SYSTEM_PRESET_HVD_CLOCK      : eEXUserCmd = E_VDEC_EX_USER_CMD_SYSTEM_PRESET_HVD_CLOCK; break;
        case VDEC_USER_CMD_GET_XC_LOW_DELAY_INT_STATE   : eEXUserCmd = E_VDEC_EX_USER_CMD_GET_XC_LOW_DELAY_INT_STATE; break;

        //Group3-3:MVD System Preset Control command
        case VDEC_USER_CMD_SYSTEM_PRESET_MVD_BASE       : eEXUserCmd = E_VDEC_EX_USER_CMD_SYSTEM_PRESET_MVD_BASE; break;
        case VDEC_USER_CMD_SYSTEM_PRESET_MVD_CLOCK      : eEXUserCmd = E_VDEC_EX_USER_CMD_SYSTEM_PRESET_MVD_CLOCK; break;
        case VDEC_USER_CMD_VPU_SECURITY_MODE            : eEXUserCmd = E_VDEC_EX_USER_CMD_VPU_SECURITY_MODE; break;

        //Group3-4:Preset Control command=============================
        case VDEC_USER_CMD_PRESET_CONTROL_BASE          : eEXUserCmd = E_VDEC_EX_USER_CMD_PRESET_CONTROL_BASE; break;
        case VDEC_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE  : eEXUserCmd = E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE; break;
        case VDEC_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE  : eEXUserCmd = E_VDEC_EX_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE; break;
        case VDEC_USER_CMD_FRAMERATE_HANDLING           : eEXUserCmd = E_VDEC_EX_USER_CMD_FRAMERATE_HANDLING; break;
        case VDEC_USER_CMD_SET_SHAREMEMORY_BASE         : eEXUserCmd = E_VDEC_EX_USER_CMD_SET_SHAREMEMORY_BASE; break;
        case VDEC_USER_CMD_HVD_COL_BBU_MODE             : eEXUserCmd = E_VDEC_EX_USER_CMD_HVD_COL_BBU_MODE; break;
        case VDEC_USER_CMD_HVD_IAPGN_BUF_SHARE_BW_MODE  : eEXUserCmd = E_VDEC_EX_USER_CMD_HVD_IAPGN_BUF_SHARE_BW_MODE; break;
        case VDEC_USER_CMD_DTV_DEBUG_MODE               : eEXUserCmd = E_VDEC_EX_USER_CMD_DTV_DEBUG_MODE; break;
        case VDEC_USER_CMD_HVD_TS_IN_BBU_MODE           : eEXUserCmd = E_VDEC_EX_USER_CMD_HVD_TS_IN_BBU_MODE; break;
        case VDEC_USER_CMD_AUTO_ARRANGE_FRAMEBUFFER_USAGE           : eEXUserCmd = E_VDEC_EX_USER_CMD_AUTO_ARRANGE_FRAMEBUFFER_USAGE; break;
        case VDEC_USER_CMD_THUMBNAIL_MODE               : eEXUserCmd = E_VDEC_EX_USER_CMD_THUMBNAIL_MODE; break;
        case VDEC_USER_CMD_FORCE_8BIT_DEC_MODE           : eEXUserCmd = E_VDEC_EX_USER_CMD_FORCE_8BIT_DEC_MODE; break;
        

        default:
            break;
    }
    return eEXUserCmd;
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
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_InitParam stInitPara;

    if (!pInitParam)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    if (!_VDEC_MapInitParam(pInitParam, &stInitPara))
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetFreeStream(&stStreamId, sizeof(VDEC_StreamId), eStType, stInitPara.eCodecType);
    if (E_VDEC_EX_OK != eEXRetVal)
    {
        goto _init_ret;
    }

    if (E_VDEC_EX_CODEC_TYPE_MVC == stInitPara.eCodecType)
    {
        MApi_VDEC_EX_SetSingleDecode(TRUE);
    }

    eEXRetVal = MApi_VDEC_EX_Init(&stStreamId, &stInitPara);

_init_ret:
    eRetVal = _VDEC_MapResult(eEXRetVal);

    if (E_VDEC_EX_OK == eEXRetVal)
    {
        //printf("%s: Update _vdecInitParam\n", __FUNCTION__);
        _VDEC_Memcpy(&_vdecInitParam, pInitParam, sizeof(VDEC_InitParam));
    }

#ifdef _MVD_EVENT_DBG
    _VDEC_MVDEventDbg();
#endif

    return eRetVal;
}

VDEC_Result MApi_VDEC_Init_EX(void *pInfo, MS_U32 u32Size)
{
    MS_U32 u32Version = *((MS_U32 *)pInfo);
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_InitParam stInitPara;

    if (FALSE == ((u32Version == 1) && (u32Size == sizeof(VDEC_InitParam_EX1))))
    {
        MS_ASSERT(FALSE);
        return E_VDEC_FAIL;
    }

    VDEC_InitParam_EX1 *pInitParam = (VDEC_InitParam_EX1 *)pInfo;

    if (!_VDEC_MapInitParam_EX1(pInitParam, &stInitPara))
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetFreeStream(&stStreamId, sizeof(VDEC_StreamId), eStType, stInitPara.eCodecType);
    if (E_VDEC_EX_OK != eEXRetVal)
    {
        goto _init_ret;
    }

    eEXRetVal = MApi_VDEC_EX_Init(&stStreamId, &stInitPara);

_init_ret:
    eRetVal = _VDEC_MapResult(eEXRetVal);
    return eRetVal;
}

//------------------------------------------------------------------------------
/// Reset decoder.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Rst(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_Rst(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Stop playback
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Exit(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;

    eEXRetVal = MApi_VDEC_EX_Exit(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    stStreamId.u32Version = 0;
    stStreamId.u32Id = VDEC_U32_MAX;

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Check if information for display setting is ready or not
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CheckDispInfoRdy(void)
{
    _VDEC_WRAP_NOT_INIT_RET();
    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_CheckDispInfoRdy(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Set up frame rate conversion mode
/// @param eFrcMode \b IN : frame rate conversion mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetFrcMode(VDEC_FrcMode eFrcMode)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetFrcMode(&stStreamId, (VDEC_EX_FrcMode)eFrcMode);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Set up some parameters about dynamic scaling in FW
/// @param u32Addr \b IN : the physical start address of parameter set
/// @param u32Size \b IN : the size of parameter set
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetDynScalingParams(MS_PHY u32Addr , MS_U32 u32Size)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetDynScalingParams(&stStreamId, u32Addr, u32Size);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Start playback
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Play(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_Play(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Pause decode and display
/// @return return VDEC_Result
//-----------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Pause(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_Pause(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Resume decode and display
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Resume(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_Resume(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Trigger to display one frame (background may still decoding)
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_StepDisp(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_StepDisp(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsStepDispDone(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Decoder will start decode with full  speed until target PTS is reached (equal or larger) then pause.
/// @param u32PTS \b IN : target PTS
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SeekToPTS(MS_U32 u32PTS)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SeekToPTS(&stStreamId, u32PTS);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SkipToPTS(&stStreamId, u32PTS);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Trigger decoder to decode one frame.
/// Decoder seeks to an I frame, decodes and displays it.
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_StepDecode(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_StepDecode(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsStepDecodeDone(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Set up trick decode mode for decode I, decode IP and decode all.
/// @param eTrickDec \b IN : trick decode mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetTrickMode(VDEC_TrickDec eTrickDec)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetTrickMode(&stStreamId, _VDEC_MapExTrickDecMode(eTrickDec));
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Push decode command queue
/// @param pCmd \b IN : the pointer to decode command
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_PushDecQ(VDEC_DecCmd *pCmd)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_DecCmd stExDecCmd;
    _VDEC_MapExDecCmd(pCmd, &stExDecCmd);
    eEXRetVal = MApi_VDEC_EX_PushDecQ(&stStreamId, &stExDecCmd);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Fire the decode command which be pushed since last fired
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_FireDecCmd(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_FireDecCmd(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get current queue vacancy
/// @return vacancy number
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetDecQVacancy(void)
{
    MS_U32 u32DecQVac = 0;
    u32DecQVac = MApi_VDEC_EX_GetDecQVacancy(&stStreamId);
    return u32DecQVac;
}

MS_BOOL MApi_VDEC_IsCCAvailable(void)
{
    MS_BOOL bIsCCAvail = FALSE;
    bIsCCAvail = MApi_VDEC_EX_IsCCAvailable(&stStreamId);
    return bIsCCAvail;
}

VDEC_Result MApi_VDEC_GetCCInfo(void *pInfo, MS_U32 u32Size)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;

    MS_U32 u32Version = *((MS_U32 *) pInfo);

    if (u32Version == 0 && u32Size == sizeof(VDEC_CC_Info))
    {
        VDEC_EX_CC_Info stExCCInfo;

        _VDEC_Memset(&stExCCInfo, 0, sizeof(VDEC_EX_CC_Info));

        eEXRetVal = MApi_VDEC_EX_GetCCInfo(&stStreamId, &stExCCInfo, sizeof(VDEC_EX_CC_Info));

        ((VDEC_CC_Info *)pInfo)->u8PicStructure = stExCCInfo.u8PicStructure;
        ((VDEC_CC_Info *)pInfo)->u8TopFieldFirst = stExCCInfo.u8TopFieldFirst;
        ((VDEC_CC_Info *)pInfo)->u16TempRef = stExCCInfo.u16TempRef;
        ((VDEC_CC_Info *)pInfo)->u32Pts = stExCCInfo.u32Pts;
        ((VDEC_CC_Info *)pInfo)->u32UserDataBuf = stExCCInfo.u32UserDataBuf;
        ((VDEC_CC_Info *)pInfo)->u32UserDataSize = stExCCInfo.u32UserDataSize;

        eRetVal = _VDEC_MapResult(eEXRetVal);
    }
    else
    {
        VDEC_INFO(printf("%s err: wrong pInfo\n", __FUNCTION__));
        eRetVal = E_VDEC_RET_INVALID_PARAM;
    }

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Flush command queue and internal data of decoder
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_Flush(VDEC_FreezePicSelect eFreezePic)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    VDEC_EX_FreezePicSelect eExFreezePic = _VDEC_MapExFreezePicSel(eFreezePic);
    eEXRetVal = MApi_VDEC_EX_Flush(&stStreamId, eExFreezePic);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get ES buffer write pointer
/// @return write pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetESWritePtr(void)
{
    MS_U32 u32WrPtr = 0;
    u32WrPtr = MApi_VDEC_EX_GetESWritePtr(&stStreamId);
    return u32WrPtr;
}

//------------------------------------------------------------------------------
/// Get decoder ES buffer read pointer
/// @return read pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetESReadPtr(void)
{
    MS_U32 u32RdPtr = 0;
    u32RdPtr = MApi_VDEC_EX_GetESReadPtr(&stStreamId);
    return u32RdPtr;
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display all decoded frame when out of input data
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_EnableLastFrameShow(MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_EnableLastFrameShow(&stStreamId, bEnable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsDispFinish(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Set up display speed
/// @param eSpeedType \b IN : display speed type
/// @param eSpeed \b IN : multiple
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetSpeed(VDEC_SpeedType eSpeedType, VDEC_DispSpeed eSpeed)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_SpeedType eExSpeedType = _VDEC_MapExSpeedType(eSpeedType);
    VDEC_EX_DispSpeed eExSpeed = _VDEC_MapExDispSpeed(eSpeed);
    eEXRetVal = MApi_VDEC_EX_SetSpeed(&stStreamId, eExSpeedType, eExSpeed);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsFrameRdy(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Let FW stop updating frames when vsync, but decoding process is still going.
/// @param bEnable \b IN : Enable/Disable
/// @return -The result of command freeze image.
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetFreezeDisp(MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetFreezeDisp(&stStreamId, bEnable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Set blue screen on/off
/// @param bOn \b IN : turn on / off blue screen
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetBlueScreen(MS_BOOL bOn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetBlueScreen(&stStreamId, bOn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Reset PTS
/// @param u32PtsBase \b IN : new PTS base value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_ResetPTS(MS_U32 u32PtsBase)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_ResetPTS(&stStreamId, u32PtsBase);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_AVSyncOn(&stStreamId, bOn, u32SyncDelay, u16SyncTolerance);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Switch AV sync free run threshold
/// @param u32Threshold \b IN : threshold value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetAVSyncFreerunThreshold(MS_U32 u32Threshold)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetAVSyncFreerunThreshold(&stStreamId, u32Threshold);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsAVSyncOn(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the PTS of current displayed frame
/// @return PTS
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetPTS(void)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO();

    MS_U32 u32Pts = VDEC_U32_MAX;
    u32Pts = MApi_VDEC_EX_GetPTS(&stStreamId);
    return u32Pts;
}

//------------------------------------------------------------------------------
/// Get the PTS of next displayed frame
/// @return next PTS (unit:ms)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetNextPTS(void)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO();

    MS_U32 u32NextPts = VDEC_U32_MAX;
    u32NextPts = MApi_VDEC_EX_GetNextPTS(&stStreamId);
    return u32NextPts;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsStartSync(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsReachSync(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsFreerun(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, decode one I frame
/// @param u32FrameBufAddr \b IN : output buffer address
/// @param u32SrcSt \b IN : source start address
/// @param u32SrcEnd \b IN : source end address
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_MHEG_DecodeIFrame(MS_PHY u32FrameBufAddr,
                                        MS_PHY u32SrcSt,
                                        MS_PHY u32SrcEnd)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_MHEG_DecodeIFrame(&stStreamId, u32FrameBufAddr, u32SrcSt, u32SrcEnd);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_MHEG_IsIFrameDecoding(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, reset decoder after I frame decode done
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_MHEG_RstIFrameDec(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    eRetVal = MApi_VDEC_Exit();
    if (E_VDEC_OK != eRetVal)
    {
        printf("%s fail to exit 0x%x\n", __FUNCTION__, eRetVal);
    }

    eRetVal = MApi_VDEC_Init(&_vdecInitParam);
    if (E_VDEC_OK != eRetVal)
    {
        printf("%s fail to init 0x%x\n", __FUNCTION__, eRetVal);
    }

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get error code
/// @return error code
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetErrCode(void)
{
    MS_U32 u32ErrCode = 0;
    u32ErrCode = MApi_VDEC_EX_GetErrCode(&stStreamId);
    return u32ErrCode;
}

//------------------------------------------------------------------------------
/// Get accumulated error counter
/// @return error counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetErrCnt(void)
{
    MS_U32 u32Cnt = 0;
    u32Cnt = MApi_VDEC_EX_GetErrCnt(&stStreamId);
    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get codec type which be initialed
/// @return VDEC_CodecType
//------------------------------------------------------------------------------
VDEC_CodecType MApi_VDEC_GetActiveCodecType(void)
{
    VDEC_CodecType eCodecType = E_VDEC_CODEC_TYPE_NONE;
    VDEC_EX_CodecType eExCodecType =  MApi_VDEC_EX_GetActiveCodecType(&stStreamId);
    eCodecType = _VDEC_MapCodecType(eExCodecType);

    return eCodecType;
}

//------------------------------------------------------------------------------
/// Get bitrate which decoder retrieved from stream
/// @return bitrate
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetBitsRate(void)
{
    MS_U32 u32Bitrate = 0;
    u32Bitrate = MApi_VDEC_EX_GetBitsRate(&stStreamId);
    return u32Bitrate;
}


//------------------------------------------------------------------------------
/// Get 3:2 pull down flag which decoder retrieved from stream
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_Is32PullDown(void)
{
    MS_BOOL bIs32PullDown = FALSE;
    bIs32PullDown = MApi_VDEC_EX_Is32PullDown(&stStreamId);
    return bIs32PullDown;
}


//------------------------------------------------------------------------------
/// Check whether the status of decoder is running or not.
/// @return VDEC_Result: E_VDEC_OK/E_VDEC_RET_NOT_RUNNING/E_VDEC_RET_NOT_INIT
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_IsAlive(void)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsAlive(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the value of PTS - STC for video
/// @return the value of PTS - STC for video
//------------------------------------------------------------------------------
MS_S64 MApi_VDEC_GetVideoPtsStcDelta(void)
{
    MS_S64 s64Pts = 0;
    s64Pts = MApi_VDEC_EX_GetVideoPtsStcDelta(&stStreamId);
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
    _VDEC_WRAP_NOT_INIT_RET();
    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsWithValidStream(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsIFrameFound(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsWithLowDelay(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsAllBufferEmpty(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get picture counter of current GOP
/// @return picture counter
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_GetGOPCnt(void)
{
    MS_U8 u8Cnt = 0;
    u8Cnt = MApi_VDEC_EX_GetGOPCnt(&stStreamId);
    return u8Cnt;
}

//-----------------------------------------------------------------------------
/// Get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetFrameCnt(void)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO();
    MS_U32 u32Cnt = 0;
    u32Cnt = MApi_VDEC_EX_GetFrameCnt(&stStreamId);
    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get skipped counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetSkipCnt(void)
{
    MS_U32 u32Cnt = 0;
    u32Cnt = MApi_VDEC_EX_GetSkipCnt(&stStreamId);
    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get dropped frame counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetDropCnt(void)
{
    MS_U32 u32Cnt = 0;
    u32Cnt = MApi_VDEC_EX_GetDropCnt(&stStreamId);
    return u32Cnt;
}

//------------------------------------------------------------------------------
/// Get display information
/// @param pDispinfo \b OUT : pointer to display setting information
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDispInfo(VDEC_DispInfo *pDispinfo)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_DispInfo stExDispinfo;

    if (pDispinfo)
    {
        eEXRetVal =  MApi_VDEC_EX_GetDispInfo(&stStreamId, &stExDispinfo);
        if (eEXRetVal == E_VDEC_EX_OK)
        {
            _VDEC_MapDispInfo(&stExDispinfo, pDispinfo);
        }
        else
        {
            _VDEC_Memset(pDispinfo, 0, sizeof(VDEC_DispInfo));
        }
        eRetVal = _VDEC_MapResult(eEXRetVal);
    }
    else
    {
        VDEC_INFO(printf("%s err: null input\n", __FUNCTION__));
        eRetVal = E_VDEC_RET_INVALID_PARAM;
    }
    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get current trick decode mode of decoder
/// @return VDEC_TrickDec
//------------------------------------------------------------------------------
VDEC_TrickDec MApi_VDEC_GetTrickMode(void)
{
    VDEC_TrickDec eTrick = E_VDEC_TRICK_DEC_ALL;
    VDEC_EX_TrickDec eExTrick = E_VDEC_EX_TRICK_DEC_ALL;
    eExTrick = MApi_VDEC_EX_GetTrickMode(&stStreamId);
    eTrick = _VDEC_MapTrickDecMode(eExTrick);
    return eTrick;
}

//------------------------------------------------------------------------------
/// Get stream active format which decoder retrieved
/// @return active format ID
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_GetActiveFormat(void)
{
    MS_U8 u8ActFmt = 0xFF;
    u8ActFmt = MApi_VDEC_EX_GetActiveFormat(&stStreamId);
    return u8ActFmt;
}

//------------------------------------------------------------------------------
/// Get stream colour primaries
/// @return stream colour primaries
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_GetColourPrimaries(void)
{
    MS_U8 u8ColourPrimaries = 0xFF;
    u8ColourPrimaries = MApi_VDEC_EX_GetColourPrimaries(&stStreamId);
    return u8ColourPrimaries;
}

VDEC_Result MApi_VDEC_GetHWKey(MS_U8 *pu8Key)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_GetHWKey(&stStreamId, pu8Key);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_IsSeqChg(&stStreamId);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;
    eEXRetVal = MApi_VDEC_EX_SetDbgLevel(&stStreamId, _VDEC_MapDbgLevel(eDbgLevel));
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the information of the latest decoded frame.
/// @param pFrmInfo \b IN : the information of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDecFrameInfo(VDEC_FrameInfo* pFrmInfo)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_FrameInfo stExFrmInfo;

    if (!pFrmInfo)
    {
        VDEC_INFO(printf("%s err: null pFrmInfo\n", __FUNCTION__));
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetDecFrameInfo(&stStreamId, &stExFrmInfo);
    if (eEXRetVal == E_VDEC_EX_OK)
    {
        _VDEC_MapFrameInfo(&stExFrmInfo, pFrmInfo);
    }
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the information of current displaying frame.
/// @param pFrmInfo \b IN : the information of displaying frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDispFrameInfo(VDEC_FrameInfo *pFrmInfo)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_FrameInfo stExFrmInfo;

    if (!pFrmInfo)
    {
        VDEC_INFO(printf("%s err: null pFrmInfo\n", __FUNCTION__));
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetDispFrameInfo(&stStreamId, &stExFrmInfo);
    if (eEXRetVal == E_VDEC_EX_OK)
    {
        _VDEC_MapFrameInfo(&stExFrmInfo, pFrmInfo);
    }
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the extension information of decoded frame.
/// @param pExtDispinfo \b IN : the extension information of decoded frame.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetExtDispInfo(VDEC_ExtDispInfo *pExtDispinfo)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_ExtDispInfo stExExtDisp;

    if (!pExtDispinfo)
    {
        VDEC_INFO(printf("%s err: null pExtDispinfo\n", __FUNCTION__));
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetExtDispInfo(&stStreamId, &stExExtDisp);
    if (eEXRetVal == E_VDEC_EX_OK)
    {
        _VDEC_MapExtDispInfo(&stExExtDisp, pExtDispinfo);
    }
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest decoded frame.
/// @param pTimeCode \b IN : the time_code of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDecTimeCode(VDEC_TimeCode* pTimeCode)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_TimeCode stTimeCode = {0,0,0,0,0,{0,0,0}};

    if (!pTimeCode)
    {
        VDEC_INFO(printf("%s err: null pTimeCode\n", __FUNCTION__));
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetDecTimeCode(&stStreamId, &stTimeCode);

    if (eEXRetVal == E_VDEC_EX_OK)
    {
        _VDEC_MapTimeCode(&stTimeCode, pTimeCode);
    }
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest displayed frame.
/// @param pTimeCode \b IN : the time_code of the latest displayed frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetDispTimeCode(VDEC_TimeCode* pTimeCode)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_TimeCode stTimeCode = {0,0,0,0,0,{0,0,0}};
    if (!pTimeCode)
    {
        return E_VDEC_RET_INVALID_PARAM;
    }

    eEXRetVal = MApi_VDEC_EX_GetDispTimeCode(&stStreamId, &stTimeCode);
    if (eEXRetVal == E_VDEC_EX_OK)
    {
        _VDEC_MapTimeCode(&stTimeCode, pTimeCode);
    }
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// [Obsolete] Start CC data parsing.
/// @param pCCParam \b IN : CC configuration
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_StartParsing(VDEC_CCCfg* pCCParam)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pCCParam);
    return E_VDEC_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Stop CC data parsing.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_StopParsing(void)
{
    printf("%s is obsolete\n", __FUNCTION__);
    return E_VDEC_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get write pointer of CC data buffer.
/// @param pu32Write \b OUT : current write pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_GetWritePtr(MS_U32* pu32Write)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pu32Write);
    return E_VDEC_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get the read pointer of CC data buffer.
/// @param pu32Read \b OUT : current read pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_GetReadPtr(MS_U32* pu32Read)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pu32Read);
    return E_VDEC_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Update the read pointer of CC data buffer.
/// @param u32EachPacketSize \b IN : new read pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_UpdateReadPtr(MS_U32 u32EachPacketSize)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(u32EachPacketSize);
    return E_VDEC_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get if CC data buffer is overflow.
/// @param pbOverflow \b OUT : overflow flag
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_CC_GetIsOverflow(MS_BOOL* pbOverflow)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pbOverflow);
    return E_VDEC_RET_UNSUPPORTED;
}

VDEC_Result MApi_VDEC_SetEvent(MS_U32 u32EnableEvent, VDEC_EventCb pfn, void *param)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetEvent(&stStreamId, u32EnableEvent, pfn, param);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_UnsetEvent(MS_U32 u32EnableEvent)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_UnsetEvent(&stStreamId, u32EnableEvent);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetEvent_MultiCallback(&stStreamId, (VDEC_EX_CB_TYPE)cb_type, u32EnableEvent, pfn, param);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Unset ISR event CB
/// @param cb_type \b IN : The CB type for VDEC interrupt
/// @param u32EnableEvent \b IN : The event flag for user data CB function
/// @return status
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_UnsetEvent_EX(VDEC_CB_TYPE cb_type, MS_U32 u32EnableEvent)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_UnsetEvent_MultiCallback(&stStreamId, (VDEC_EX_CB_TYPE)cb_type, u32EnableEvent);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get ISR event status
/// @return status
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_GetEventInfo(MS_U32* u32EventFlag)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;

    if (u32EventFlag)
    {
        eEXRetVal = MApi_VDEC_EX_GetEventInfo(&stStreamId, u32EventFlag);
        eRetVal = _VDEC_MapResult(eEXRetVal);
    }

    return eRetVal;
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
VDEC_Result MDrv_VDEC_GenPattern(VDEC_PatternType ePatternType ,  MS_PHY u32Addr , MS_U32* u32Size)
{
    printf("VDEC Warn: MDrv_VDEC_GenPattern() will be removed. please use new API:MApi_VDEC_GenPattern()\n ");
    return MApi_VDEC_GenPattern(ePatternType, u32Addr, u32Size);
}

VDEC_Result MApi_VDEC_GenPattern(VDEC_PatternType ePatternType,  MS_PHY u32Addr, MS_U32 *pu32Size)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    VDEC_EX_PatternType eExPtnType = _VDEC_MapExPatternType(ePatternType);
    eEXRetVal = MApi_VDEC_EX_GenPattern(&stStreamId, eExPtnType, u32Addr, pu32Size);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Get the information of the least length of pattern used for specific function
/// @param pFrmInfo \b IN : the information of the least length of pattern used for specific function
/// @return -the length (Bytes)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_GetPatternLeastLength(VDEC_PatternType ePatternType)
{
    MS_U32 u32Ret = 0;
    VDEC_EX_PatternType eExPtnType = _VDEC_MapExPatternType(ePatternType);
    u32Ret = MApi_VDEC_EX_GetPatternLeastLength(&stStreamId, eExPtnType);
    return u32Ret;
}

//------------------------------------------------------------------------------
/// Set deblocking process on/off
/// @param bOn \b IN : turn on / off deblocking process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_DisableDeblocking(MS_BOOL bDisable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_DisableDeblocking(&stStreamId, bDisable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Set Quarter Pixel process on/off
/// @param bOn \b IN : turn on / off Quarter Pixel process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_DisableQuarterPixel(MS_BOOL bDisable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_DisableQuarterPixel(&stStreamId, bDisable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Turn on/off Auto Remove Last Zero Byte process
/// @param bOn \b IN : turn on / off Auto Remove Last Zero Byte process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_Result MApi_VDEC_SetAutoRmLstZeroByte(MS_BOOL bOn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetAutoRmLstZeroByte(&stStreamId, bOn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetBalanceBW(&stStreamId, u8QuarPixelTH, u8DeBlockingTH, u8UpperBound, u8SafeCoef);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_DbgDumpStatus(void)
{
    MApi_VDEC_EX_DbgDumpStatus(&stStreamId);
    return E_VDEC_EX_OK;
}

extern MS_U32 MApi_VDEC_EX_DbgGetProgCnt(void);
MS_U32 MApi_VDEC_DbgGetProgCnt(void)
{
    return MApi_VDEC_EX_DbgGetProgCnt();
}

VDEC_Result MApi_VDEC_SetControl(VDEC_User_Cmd cmd_id, MS_U32 param)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetControl(&stStreamId, _VDEC_MapUserCmd(cmd_id), param);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_PreSetControl(VDEC_User_Cmd cmd_id, MS_U32 param)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_PreSetControl(&stStreamId, _VDEC_MapUserCmd(cmd_id), param);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_GetControl(VDEC_User_Cmd cmd_id, MS_U32 *param)
{
    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_OK;

    if (VDEC_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO == cmd_id)
    {
        VDEC_FrameInfo* pstFrmInfo = (VDEC_FrameInfo*)param;
        VDEC_EX_FrameInfo stEXFrmInfo;
        memset(&stEXFrmInfo,0,sizeof(VDEC_EX_FrameInfo));
        eEXRetVal = MApi_VDEC_EX_GetControl(&stStreamId, E_VDEC_EX_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO, (MS_U32*)&stEXFrmInfo);
        if (E_VDEC_EX_OK == eEXRetVal)
        {
            _VDEC_MapFrameInfo(&stEXFrmInfo, pstFrmInfo);
        }
    }
    else if(VDEC_USER_CMD_GET_MAIN_STREAM_ID == cmd_id)
    {
        *param = stStreamId.u32Id;
    }
    else
    {
        eEXRetVal = MApi_VDEC_EX_GetControl(&stStreamId, _VDEC_MapUserCmd(cmd_id), param);
    }
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

MS_U32 MApi_VDEC_GetDispCnt(void)
{
    MS_U32 u32Cnt = 0;
    u32Cnt = MApi_VDEC_EX_GetDispCnt(&stStreamId);
    return u32Cnt;
}

MS_U32 MApi_VDEC_GetFwVersion(VDEC_FwType eFwType)
{
    MS_U32 u32FWVer = 0;

    u32FWVer = MApi_VDEC_EX_GetFwVersion(&stStreamId, (VDEC_EX_FwType)eFwType);

    return u32FWVer;
}

VDEC_Result MApi_VDEC_SetBlockDisplay(MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SetBlockDisplay(&stStreamId, bEnable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_EnableESBuffMalloc(MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_EnableESBuffMalloc(&stStreamId, bEnable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

MS_U32 MApi_VDEC_GetESBuffVacancy(void* pData)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO();
    return MApi_VDEC_EX_GetESBuffVacancy(&stStreamId, pData);
}

VDEC_Result MApi_VDEC_GetESBuff(MS_U32 u32ReqSize, MS_U32* u32AvailSize, MS_PHY* u32Addr)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_GetESBuff(&stStreamId, u32ReqSize, u32AvailSize, u32Addr);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_GetNextDispFrame(VDEC_DispFrame** ppDispFrm)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;

    //MS_ASSERT(sizeof(VDEC_EX_DispFrame) == sizeof(VDEC_DispFrame));
    //MS_ASSERT(sizeof(VDEC_EX_FrameInfo) == sizeof(VDEC_FrameInfo));

    eEXRetVal = MApi_VDEC_EX_GetNextDispFrame(&stStreamId, (VDEC_EX_DispFrame **)ppDispFrm);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
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
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_DisplayFrame(&stStreamId, (VDEC_EX_DispFrame*)pDispFrm);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_ReleaseFrame(VDEC_DispFrame *pDispFrm)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_ReleaseFrame(&stStreamId, (VDEC_EX_DispFrame*)pDispFrm);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CaptureFrame(MS_U32 u32FrmPriData, MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CaptureFrame(&stStreamId, u32FrmPriData, bEnable);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_Init(MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_Init(&stStreamId, pIn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_SetCfg(VDEC_CCFormat eFmt, VDEC_CCType eType, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_SetCfg(&stStreamId, _VDEC_MapCCFormat(eFmt), _VDEC_MapCCType(eType), pIn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_SetBuffStartAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_SetBuffStartAdd(&stStreamId, _VDEC_MapCCFormat(eFmt), pIn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_UpdateWriteAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_UpdateWriteAdd(&stStreamId, _VDEC_MapCCFormat(eFmt), pIn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_UpdateReadAdd(VDEC_CCFormat eFmt, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_UpdateReadAdd(&stStreamId, _VDEC_MapCCFormat(eFmt), pIn);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_DisableParsing(VDEC_CCFormat eFmt)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_DisableParsing(&stStreamId, _VDEC_MapCCFormat(eFmt));
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_GetInfo(VDEC_CCFormat eFmt, VDEC_CCInfoCmd eCmd, MS_U32 *pOut)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_GetInfo(&stStreamId, _VDEC_MapCCFormat(eFmt), _VDEC_MapCCInfoCmd(eCmd), pOut);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_GetIsRstDone(VDEC_CCFormat eFmt)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_GetIsRstDone(&stStreamId, _VDEC_MapCCFormat(eFmt));
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

VDEC_Result MApi_VDEC_CC_GetIsBuffOverflow(VDEC_CCFormat eFmt)
{
    _VDEC_WRAP_NOT_INIT_RET();

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_CC_GetIsBuffOverflow(&stStreamId, _VDEC_MapCCFormat(eFmt));
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}

//------------------------------------------------------------------------------
/// Report the physical address of write pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHY
//------------------------------------------------------------------------------
MS_PHY MApi_VDEC_CC_GetWriteAdd(VDEC_CCFormat eFmt)
{
    MS_PHY u32CCWrPtr = VDEC_U32_MAX;
    u32CCWrPtr = MApi_VDEC_EX_CC_GetWriteAdd(&stStreamId, _VDEC_MapCCFormat(eFmt));
    return u32CCWrPtr;
}

//------------------------------------------------------------------------------
/// Report the physical address of read pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHY
//------------------------------------------------------------------------------
MS_PHY MApi_VDEC_CC_GetReadAdd(VDEC_CCFormat eFmt)
{
    MS_PHY u32CCRdPtr = VDEC_U32_MAX;
    u32CCRdPtr = MApi_VDEC_EX_CC_GetReadAdd(&stStreamId, _VDEC_MapCCFormat(eFmt));
    return u32CCRdPtr;
}


VDEC_Result MApi_VDEC_SystemPreSetControl(VDEC_User_Cmd cmd_id, void *pParam)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    eEXRetVal = MApi_VDEC_EX_SystemPreSetControl(_VDEC_MapUserCmd(cmd_id), pParam);
    eRetVal = _VDEC_MapResult(eEXRetVal);

    return eRetVal;
}
#endif
