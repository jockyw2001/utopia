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
/// file    apiVDEC_EX_wrap.c
/// @brief  wrapper to wrap VDEC_EX to VDEC API
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "MsVersion.h"
#include "../vdec_ex/apiVDEC_EX.h"
#include "apiVDEC.h"

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

#define _VDEC_WRAP_NOT_INIT_RET(StreamID)                            \
      do {                                                   \
            if (VDEC_MAIN_ID != StreamID)            \
            {                                                \
                return E_VDEC_EX_RET_NOT_INIT;                  \
            }                                                \
      } while(0)

#define _VDEC_WRAP_NOT_INIT_RET_ZERO(StreamID)                       \
      do {                                                   \
            if (VDEC_MAIN_ID != StreamID)            \
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
#define VDEC_MAIN_ID 0x1111

//StreamID for VDEC_EX interface
static VDEC_StreamId stStreamId = {
                                      0,            //u32Version
                                      VDEC_U32_MAX  //u32Id
                                  };

static VDEC_EX_Info        _api_vdec_info;
#if !((defined(CHIP_A1) || defined(CHIP_A7)) && defined (__aeon__))
static VDEC_DbgLevel    _eDbgMsgLevel = E_VDEC_DBG_LEVEL_ERR;
#endif
static VDEC_EX_InitParam   _ExVdecInitParam;

static VDEC_EX_Result _VDEC_MapExResult(VDEC_Result eRet)
{
    VDEC_EX_Result eExRet = E_VDEC_EX_FAIL;
    const VDEC_EX_Result eRetMapTable[E_VDEC_EX_RET_NUM] =
    {
        E_VDEC_EX_FAIL,
        E_VDEC_EX_OK,
        E_VDEC_EX_RET_INVALID_PARAM,
        E_VDEC_EX_RET_ILLEGAL_ACCESS,
        E_VDEC_EX_RET_HARDWARE_BREAKDOWN,
        E_VDEC_EX_RET_UNSUPPORTED,
        E_VDEC_EX_RET_TIMEOUT,
        E_VDEC_EX_RET_NOT_READY,
        E_VDEC_EX_RET_NOT_INIT,
        E_VDEC_EX_RET_NOT_EXIT,
        E_VDEC_EX_RET_NOT_RUNNING
    };

    //MS_ASSERT(eRet < E_VDEC_EX_RET_NUM);
    eExRet = eRetMapTable[eRet];
    return eExRet;
}

static void _VDEC_MapExDispInfo(VDEC_DispInfo* pDisp, VDEC_EX_DispInfo* pExDisp)
{
    pExDisp->u16HorSize        = pDisp->u16HorSize;
    pExDisp->u16VerSize        = pDisp->u16VerSize;
    pExDisp->u32FrameRate      = pDisp->u32FrameRate;
    pExDisp->u8Interlace       = pDisp->u8Interlace;
    pExDisp->u8AFD             = pDisp->u8AFD;
    pExDisp->u16SarWidth       = pDisp->u16SarWidth;
    pExDisp->u16SarHeight      = pDisp->u16SarHeight;
    pExDisp->u16CropRight      = pDisp->u16CropRight;
    pExDisp->u16CropLeft       = pDisp->u16CropLeft;
    pExDisp->u16CropBottom     = pDisp->u16CropBottom;
    pExDisp->u16CropTop        = pDisp->u16CropTop;
    pExDisp->u16Pitch          = pDisp->u16Pitch;
    pExDisp->u16PTSInterval    = pDisp->u16PTSInterval;
    pExDisp->u8MPEG1           = pDisp->u8MPEG1;
    pExDisp->u8PlayMode        = pDisp->u8PlayMode;
    pExDisp->u8FrcMode         = pDisp->u8FrcMode;
    pExDisp->u8AspectRate      = pDisp->u8AspectRate;
    pExDisp->bWithChroma       = pDisp->bWithChroma;
    pExDisp->bColorInXVYCC     = pDisp->bColorInXVYCC;
    pExDisp->u32DynScalingAddr = pDisp->u32DynScalingAddr;
    pExDisp->u32DynScalingSize = pDisp->u32DynScalingSize;
    pExDisp->u8DynScalingDepth = pDisp->u8DynScalingDepth;
    pExDisp->bEnableMIUSel   = pDisp->bEnableMIUSel;
    pExDisp->u32AspectWidth  = pDisp->u32AspectWidth;
    pExDisp->u32AspectHeight = pDisp->u32AspectHeight;
}

static void _VDEC_MapDecCmd(VDEC_EX_DecCmd *pExCmd, VDEC_DecCmd *pCmd)
{
    pCmd->u32ID_H      = pExCmd->u32ID_H;
    pCmd->u32ID_L      = pExCmd->u32ID_L;
    pCmd->u32StAddr    = pExCmd->u32StAddr;
    pCmd->u32Size      = pExCmd->u32Size;
    pCmd->u32Timestamp = pExCmd->u32Timestamp;
}

static VDEC_EX_FrameType _VDEC_MapExFrameType(VDEC_FrameType eFrameType)
{
    VDEC_EX_FrameType eExFrameType = E_VDEC_EX_FRM_TYPE_OTHER;

    switch(eFrameType)
    {
        case E_VDEC_EX_FRM_TYPE_I:
            eExFrameType = E_VDEC_EX_FRM_TYPE_I;
            break;
        case E_VDEC_EX_FRM_TYPE_P:
            eExFrameType = E_VDEC_EX_FRM_TYPE_P;
            break;
        case E_VDEC_EX_FRM_TYPE_B:
            eExFrameType = E_VDEC_EX_FRM_TYPE_B;
            break;
        case E_VDEC_EX_FRM_TYPE_OTHER:
        default:
            eExFrameType = E_VDEC_EX_FRM_TYPE_OTHER;
            break;
    }

    return eExFrameType;
}

static VDEC_EX_FieldType _VDEC_MapExFieldType(VDEC_FieldType eFieldType)
{
    VDEC_EX_FieldType eExFieldType = E_VDEC_EX_FIELDTYPE_NONE;

    switch(eFieldType)
    {
        case E_VDEC_FIELDTYPE_NONE:
            eExFieldType = E_VDEC_EX_FIELDTYPE_NONE;
            break;
        case E_VDEC_FIELDTYPE_TOP:
            eExFieldType = E_VDEC_EX_FIELDTYPE_TOP;
            break;
        case E_VDEC_FIELDTYPE_BOTTOM:
            eExFieldType = E_VDEC_EX_FIELDTYPE_BOTTOM;
            break;
        case E_VDEC_FIELDTYPE_BOTH:
            eExFieldType = E_VDEC_EX_FIELDTYPE_BOTH;
            break;
        default:
            eExFieldType = E_VDEC_EX_FIELDTYPE_NONE;
            MS_ASSERT(0);
            break;
    }

    return eExFieldType;
}

static void _VDEC_MapExFrameInfo(VDEC_FrameInfo* pFrmInfo, VDEC_EX_FrameInfo* pExFrmInfo)
{
    pExFrmInfo->u32LumaAddr   = pFrmInfo->u32LumaAddr  ;
    pExFrmInfo->u32ChromaAddr = pFrmInfo->u32ChromaAddr;
    pExFrmInfo->u32TimeStamp  = pFrmInfo->u32TimeStamp ;
    pExFrmInfo->u32ID_L       = pFrmInfo->u32ID_L;
    pExFrmInfo->u32ID_H       = pFrmInfo->u32ID_H;
    pExFrmInfo->u16Pitch      = pFrmInfo->u16Pitch;
    pExFrmInfo->u16Width      = pFrmInfo->u16Width;
    pExFrmInfo->u16Height     = pFrmInfo->u16Height;
    pExFrmInfo->eFrameType    = _VDEC_MapExFrameType(pFrmInfo->eFrameType);
    pExFrmInfo->eFieldType    = _VDEC_MapExFieldType(pFrmInfo->eFieldType);
}

static void _VDEC_MapExtDispInfo(VDEC_ExtDispInfo* pFrmInfo, VDEC_EX_ExtDispInfo* pExFrmInfo)
{
    pExFrmInfo->u16VSize = pFrmInfo->u16VSize;
    pExFrmInfo->u16HSize = pFrmInfo->u16HSize;
    pExFrmInfo->s16HOffset= pFrmInfo->s16HOffset;
    pExFrmInfo->s16VOffset= pFrmInfo->s16VOffset;
}

static VDEC_TrickDec _VDEC_MapTrickDecMode(VDEC_EX_TrickDec eExTrickDec)
{
    VDEC_TrickDec eTrickDec = E_VDEC_TRICK_DEC_ALL;

    switch(eExTrickDec)
    {
        case E_VDEC_EX_TRICK_DEC_ALL:
            eTrickDec = E_VDEC_TRICK_DEC_ALL;
            break;
        case E_VDEC_EX_TRICK_DEC_IP:
            eTrickDec = E_VDEC_TRICK_DEC_IP;
            break;
        case E_VDEC_EX_TRICK_DEC_I:
            eTrickDec = E_VDEC_TRICK_DEC_I;
            break;
        default:
            eTrickDec = E_VDEC_TRICK_DEC_ALL;
            MS_ASSERT(0);
            break;
    }

    return eTrickDec;
}

static VDEC_EX_TrickDec _VDEC_MapExTrickDecMode(VDEC_TrickDec eTrickDec)
{
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
}

static VDEC_FreezePicSelect _VDEC_MapFreezePicSel(VDEC_EX_FreezePicSelect eExPicSel)
{
    VDEC_FreezePicSelect ePicSel = E_VDEC_FREEZE_AT_CUR_PIC;

    switch (eExPicSel)
    {
        case E_VDEC_EX_FREEZE_AT_CUR_PIC:
            ePicSel = E_VDEC_FREEZE_AT_CUR_PIC;
            break;
        case E_VDEC_EX_FREEZE_AT_LAST_PIC:
            ePicSel = E_VDEC_FREEZE_AT_LAST_PIC;
            break;
        default:
            ePicSel = E_VDEC_FREEZE_AT_LAST_PIC;
            MS_ASSERT(0);
            break;
    }
    return ePicSel;
}

static VDEC_SpeedType _VDEC_MapSpeedType(VDEC_EX_SpeedType eExSpeedType)
{
    VDEC_SpeedType eSpeedType = E_VDEC_SPEED_DEFAULT;

    switch (eExSpeedType)
    {
        case E_VDEC_EX_SPEED_DEFAULT:
            eSpeedType = E_VDEC_SPEED_DEFAULT;
            break;
        case E_VDEC_EX_SPEED_FAST:
            eSpeedType = E_VDEC_SPEED_FAST;
            break;
        case E_VDEC_EX_SPEED_SLOW:
            eSpeedType = E_VDEC_SPEED_SLOW;
            break;
        default:
            eSpeedType = E_VDEC_SPEED_DEFAULT;
            MS_ASSERT(0);
            break;
    }

    return eSpeedType;
}

static VDEC_DispSpeed _VDEC_MapDispSpeed(VDEC_EX_DispSpeed eExSpeed)
{
    VDEC_DispSpeed eSpeed = E_VDEC_DISP_SPEED_1X;

    switch (eExSpeed)
    {
        case E_VDEC_EX_DISP_SPEED_1X:
            eSpeed = E_VDEC_DISP_SPEED_1X;
            break;
        case E_VDEC_EX_DISP_SPEED_2X:
            eSpeed = E_VDEC_DISP_SPEED_2X;
            break;
        case E_VDEC_EX_DISP_SPEED_4X :
            eSpeed = E_VDEC_DISP_SPEED_4X;
            break;
        case E_VDEC_EX_DISP_SPEED_8X :
            eSpeed = E_VDEC_DISP_SPEED_8X;
            break;
        case E_VDEC_EX_DISP_SPEED_16X:
            eSpeed = E_VDEC_DISP_SPEED_16X;
            break;
        case E_VDEC_EX_DISP_SPEED_32X:
            eSpeed = E_VDEC_DISP_SPEED_32X;
            break;
        default:
            eSpeed = E_VDEC_DISP_SPEED_1X;
            MS_ASSERT(0);
            break;
    }

    return eSpeed;
}

static void _VDEC_MapExTimeCode(VDEC_TimeCode* pstTimeCode, VDEC_EX_TimeCode* pstExTimeCode)
{
    pstExTimeCode->u8TimeCodeHr  = pstTimeCode->u8TimeCodeHr;
    pstExTimeCode->u8TimeCodeMin = pstTimeCode->u8TimeCodeMin;
    pstExTimeCode->u8TimeCodeSec = pstTimeCode->u8TimeCodeSec;
    pstExTimeCode->u8TimeCodePic = pstTimeCode->u8TimeCodePic;
    pstExTimeCode->u8DropFrmFlag = pstTimeCode->u8DropFrmFlag;
}

static VDEC_PatternType _VDEC_MapPatternType(VDEC_EX_PatternType eExPatternType)
{
    VDEC_PatternType ePatternType = E_VDEC_PATTERN_FLUSH;

    if (eExPatternType == E_VDEC_EX_PATTERN_FLUSH)
    {
        ePatternType = E_VDEC_PATTERN_FLUSH;
    }
    else if (eExPatternType == E_VDEC_EX_PATTERN_FILEEND)
    {
        ePatternType = E_VDEC_PATTERN_FILEEND;
    }
    else
    {
        MS_ASSERT(0);
    }

    return ePatternType;
}

static void _VDEC_MapExStatus(VDEC_Status* pStatus, VDEC_EX_Status* pExStatus)
{
    pExStatus->bInit  = pStatus->bInit;
    pExStatus->bIdle  = pStatus->bIdle;
    switch (pStatus->eStage)
    {
        case E_VDEC_STAGE_STOP:  pExStatus->eStage = E_VDEC_EX_STAGE_STOP; break;
        case E_VDEC_STAGE_INIT:  pExStatus->eStage = E_VDEC_EX_STAGE_INIT; break;
        case E_VDEC_STAGE_PLAY:  pExStatus->eStage = E_VDEC_EX_STAGE_PLAY; break;
        case E_VDEC_STAGE_PAUSE: pExStatus->eStage = E_VDEC_EX_STAGE_PAUSE; break;
        default:    pExStatus->eStage = E_VDEC_EX_STAGE_STOP; break;
    }
    return;
}

static VDEC_CodecType _VDEC_MapCodecType(VDEC_EX_CodecType eExCodecType)
{
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
        E_VDEC_CODEC_TYPE_VP8
    };
    VDEC_CodecType eCodecType = eCodecTypeMapTable[eExCodecType];
    MS_ASSERT(eExCodecType < E_VDEC_EX_CODEC_TYPE_NUM);
    return eCodecType;
}

static VDEC_EX_CodecType _VDEC_MapExCodecType(VDEC_CodecType eCodecType)
{
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
        E_VDEC_EX_CODEC_TYPE_VP8
    };
    //MS_ASSERT((MS_U32)eCodecType <= (MS_U32)E_VDEC_EX_CODEC_TYPE_NUM); //VDEC only has (E_VDEC_EX_CODEC_TYPE_NUM-1) CodecTypes
    VDEC_EX_CodecType eExCodecType = eExCodecTypeMapTable[eCodecType];
    return eExCodecType;
}
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_DbgCmd(VDEC_StreamId *pStreamId, MS_U32 u8Cmd, MS_U32 u32CmdArg)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_DbgCmd(u8Cmd, u32CmdArg));
}

VDEC_EX_Result MApi_VDEC_EX_DbgSetData(VDEC_StreamId *pStreamId, MS_U32 u32Addr, MS_U32 u32Data)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_DbgSetData(u32Addr, u32Data));
}

VDEC_EX_Result MApi_VDEC_EX_DbgGetData(VDEC_StreamId *pStreamId, MS_U32 u32Addr, MS_U32 *u32Data)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_DbgGetData(u32Addr, u32Data);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
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
VDEC_EX_Result MApi_VDEC_EX_GetLibVer(const MSIF_Version **ppVersion)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;
    if (!ppVersion)
    {
        return eEXRetVal;
    }

    eRetVal = MApi_VDEC_GetLibVer(ppVersion);
    eEXRetVal = _VDEC_MapExResult(eRetVal);
    return eEXRetVal;
}

//-----------------------------------------------------------------------------
/// Get VDEC info
/// @brief \b Function \b Description:  Get information of VDEC API
/// @return - the pointer to the VDEC information
//-----------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetInfo(const VDEC_EX_Info **ppInfo)
{
    if (!ppInfo)
    {
        return E_VDEC_EX_FAIL;
    }

    *ppInfo = &_api_vdec_info;

    return E_VDEC_EX_OK;
}

//------------------------------------------------------------------------------
/// Get decoder status
/// @param pStatus \b OUT : VDEC_Status
/// @return TRUE or FALSE
///     - TRUE: Useful status got
///     - FALSE: No status because of not init yet
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_GetStatus(VDEC_StreamId *pStreamId, VDEC_EX_Status *pExStatus)
{
    MS_BOOL bRet = FALSE;
    VDEC_Status stStatus;

    if (pExStatus)
    {
        bRet = MApi_VDEC_GetStatus(&stStatus);
        _VDEC_MapExStatus(&stStatus, pExStatus);
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
MS_BOOL MApi_VDEC_EX_CheckCaps(VDEC_StreamId *pStreamId, VDEC_EX_CodecType eExCodecType)
{
    VDEC_CodecType eCodecType = _VDEC_MapCodecType(eExCodecType);

    return MApi_VDEC_CheckCaps(eCodecType);
}

//------------------------------------------------------------------------------
/// Enable turbo mode for VDEC.
/// @param bTurbo \b IN : TRUE for enable or FALSE for disable
/// @return VDEC_Result
///     - E_VDEC_OK: success
///     - E_VDEC_FAIL: failed
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_EnableTurboMode(MS_BOOL bEnable)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eEXRetVal = E_VDEC_EX_FAIL;

    eRetVal = MApi_VDEC_EnableTurboMode(bEnable);
    eEXRetVal = _VDEC_MapExResult(eRetVal);
    return eEXRetVal;
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

static VDEC_FWSourceType _VDEC_MapFWSourceType(VDEC_EX_FWSourceType eExFWSrc)
{
    VDEC_FWSourceType eFWSrc = E_VDEC_FW_SOURCE_NONE;

    switch (eExFWSrc)
    {
        case E_VDEC_EX_FW_SOURCE_DRAM:
            eFWSrc = E_VDEC_FW_SOURCE_DRAM;
            break;
        case E_VDEC_EX_FW_SOURCE_FLASH:
            eFWSrc = E_VDEC_FW_SOURCE_FLASH;
            break;
        case E_VDEC_EX_FW_SOURCE_NONE:
        default:
            eFWSrc = E_VDEC_FW_SOURCE_NONE;
            break;
    }

    return eFWSrc;
}

static VDEC_CCFormat _VDEC_MapCCFormat(VDEC_EX_CCFormat eExCCFmt)
{
    VDEC_CCFormat eCCFmt = E_VDEC_CC_NONE;

    switch (eExCCFmt)
    {
        case E_VDEC_EX_CC_608:
            eCCFmt = E_VDEC_CC_608;
            break;
        case E_VDEC_EX_CC_708:
            eCCFmt = E_VDEC_CC_708;
            break;
        case E_VDEC_EX_CC_UNPACKED:
            eCCFmt = E_VDEC_CC_UNPACKED;
            break;
        case E_VDEC_EX_CC_NONE:
        default:
            eCCFmt = E_VDEC_CC_NONE;
            break;
    }

    return eCCFmt;
}

static VDEC_CCType _VDEC_MapCCType(VDEC_EX_CCType eExCCType)
{
    VDEC_CCType eCCType = E_VDEC_CC_TYPE_NONE;

    switch (eExCCType)
    {
        case E_VDEC_EX_CC_TYPE_NTSC_FIELD1:
            eCCType = E_VDEC_CC_TYPE_NTSC_FIELD1;
            break;
        case E_VDEC_EX_CC_TYPE_NTSC_FIELD2:
            eCCType = E_VDEC_CC_TYPE_NTSC_FIELD2;
            break;
        case E_VDEC_EX_CC_TYPE_DTVCC:
            eCCType = E_VDEC_CC_TYPE_DTVCC;
            break;
        case E_VDEC_EX_CC_TYPE_NTSC_TWOFIELD:
            eCCType = E_VDEC_CC_TYPE_NTSC_TWOFIELD;
            break;
        case E_VDEC_EX_CC_TYPE_NONE:
        default:
            eCCType = E_VDEC_CC_TYPE_NONE;
            break;
    }

    return eCCType;
}

static VDEC_CCInfoCmd _VDEC_MapCCInfoCmd(VDEC_EX_CCInfoCmd eExCCInfoCmd)
{
    VDEC_CCInfoCmd eCCInfoCmd = E_VDEC_CC_GET_BUFF_START;

    switch (eExCCInfoCmd)
    {
        case E_VDEC_EX_CC_GET_BUFF_START:
            eCCInfoCmd = E_VDEC_CC_GET_BUFF_START;
            break;
        case E_VDEC_EX_CC_GET_BUFF_SIZE:
            eCCInfoCmd = E_VDEC_CC_GET_BUFF_SIZE;
            break;
        case E_VDEC_EX_CC_GET_708_ENABLE:
        default:
            eCCInfoCmd = E_VDEC_CC_GET_708_ENABLE;
            break;
    }

    return eCCInfoCmd;
}

static VDEC_DbgLevel _VDEC_MapDbgLevel(VDEC_EX_DbgLevel eExDbg)
{
    VDEC_DbgLevel  eDbg = E_VDEC_DBG_LEVEL_NONE;

    switch (eExDbg)
    {
        case E_VDEC_EX_DBG_LEVEL_ERR:
            eDbg = E_VDEC_DBG_LEVEL_ERR;
            break;
        case E_VDEC_EX_DBG_LEVEL_INFO:
            eDbg = E_VDEC_DBG_LEVEL_INFO;
            break;
        case E_VDEC_EX_DBG_LEVEL_DBG:
            eDbg = E_VDEC_DBG_LEVEL_DBG;
            break;
        case E_VDEC_EX_DBG_LEVEL_TRACE:
            eDbg = E_VDEC_DBG_LEVEL_TRACE;
            break;
        case E_VDEC_EX_DBG_LEVEL_FW:
            eDbg = E_VDEC_DBG_LEVEL_FW;
            break;
        case E_VDEC_EX_DBG_LEVEL_NONE:
        default:
            eDbg = E_VDEC_DBG_LEVEL_NONE;
            break;
    }
    return eDbg;
}

static void _VDEC_MapSysCfg(VDEC_EX_SysCfg* pstExSys, VDEC_SysCfg* pstSys)
{
    pstSys->u32FWBinaryAddr     = pstExSys->u32FWBinaryAddr;
    pstSys->u32FWBinarySize     = pstExSys->u32FWBinarySize;
    pstSys->u32CodeBufAddr      = pstExSys->u32CodeBufAddr;
    pstSys->u32CodeBufSize      = pstExSys->u32CodeBufSize;
    pstSys->u32FrameBufAddr     = pstExSys->u32FrameBufAddr;
    pstSys->u32FrameBufSize     = pstExSys->u32FrameBufSize;
    pstSys->u32BitstreamBufAddr = pstExSys->u32BitstreamBufAddr;
    pstSys->u32BitstreamBufSize = pstExSys->u32BitstreamBufSize;
    pstSys->u32DrvProcBufAddr   = pstExSys->u32DrvProcBufAddr;
    pstSys->u32DrvProcBufSize   = pstExSys->u32DrvProcBufSize;
    pstSys->u32VlcBinarySrcAddr = pstExSys->u32VlcBinarySrcAddr;
    pstSys->u32VlcTabBinarySize = pstExSys->u32VlcTabBinarySize;
    pstSys->eDbgMsgLevel        = _VDEC_MapDbgLevel(pstExSys->eDbgMsgLevel);
    pstSys->eFWSourceType       = _VDEC_MapFWSourceType(pstExSys->eFWSourceType);
}

static VDEC_SrcMode _VDEC_MapSrcMode(VDEC_EX_SrcMode eExSrcMode)
{
    VDEC_SrcMode eSrcMode = E_VDEC_SRC_MODE_DTV;

    switch (eExSrcMode)
    {
        case E_VDEC_EX_SRC_MODE_DTV:
            eSrcMode = E_VDEC_SRC_MODE_DTV;
            break;
        case E_VDEC_EX_SRC_MODE_TS_FILE:
            eSrcMode = E_VDEC_SRC_MODE_TS_FILE;
            break;
        case E_VDEC_EX_SRC_MODE_FILE:
            eSrcMode = E_VDEC_SRC_MODE_FILE;
            break;
        case E_VDEC_EX_SRC_MODE_TS_FILE_DUAL_ES:
            eSrcMode = E_VDEC_SRC_MODE_TS_FILE_DUAL_ES;
            break;
        case E_VDEC_EX_SRC_MODE_FILE_DUAL_ES:
            eSrcMode = E_VDEC_SRC_MODE_FILE_DUAL_ES;
            break;
        default:
            VDEC_INFO(printf("input eExSrcMode 0x%x out of range!\n", eExSrcMode));
            eSrcMode = E_VDEC_SRC_MODE_DTV;
            break;
    }
    return eSrcMode;
}

static VDEC_TimeStampType _VDEC_MapTimeStampType(VDEC_EX_TimeStampType eExTimeStamp)
{
    VDEC_TimeStampType eTimeStamp = E_VDEC_TIME_STAMP_NONE;

    switch (eExTimeStamp)
    {
        case E_VDEC_EX_TIME_STAMP_PTS:
            eTimeStamp = E_VDEC_TIME_STAMP_PTS;
            break;
        case E_VDEC_EX_TIME_STAMP_DTS:
            eTimeStamp = E_VDEC_TIME_STAMP_DTS;
            break;
        case E_VDEC_EX_TIME_STAMP_STS:
            eTimeStamp = E_VDEC_TIME_STAMP_STS;
            break;
        case E_VDEC_EX_TIME_STAMP_NONE:
        default:
           eTimeStamp = E_VDEC_TIME_STAMP_NONE;
            break;
    }
    return eTimeStamp;
}

static VDEC_MJpegScaleFactor _VDEC_MapMjpegScaleFactor(VDEC_EX_MJpegScaleFactor eExMjScaleFactor)
{
    VDEC_MJpegScaleFactor eMjScaleFactor = E_VDEC_MJPEG_SCALE_1to1;

    switch (eExMjScaleFactor)
    {
        case E_VDEC_EX_MJPEG_SCALE_1to1:
            eMjScaleFactor = E_VDEC_MJPEG_SCALE_1to1;
            break;
        case E_VDEC_EX_MJPEG_SCALE_2to1:
            eMjScaleFactor = E_VDEC_MJPEG_SCALE_2to1;
            break;
        case E_VDEC_EX_MJPEG_SCALE_4to1:
            eMjScaleFactor = E_VDEC_MJPEG_SCALE_4to1;
            break;
        case E_VDEC_EX_MJPEG_SCALE_8to1:
            eMjScaleFactor = E_VDEC_MJPEG_SCALE_8to1;
            break;
        default:
            eMjScaleFactor = E_VDEC_MJPEG_SCALE_1to1;
            break;
    }
    return eMjScaleFactor;
}

static VDEC_FBReductionType _VDEC_MapFBReductionType(VDEC_EX_FBReductionType eExFBReductionType)
{
    VDEC_FBReductionType eFBReductionType = VDEC_FB_REDUCTION_NONE;

    switch (eExFBReductionType)
    {
        case VDEC_EX_FB_REDUCTION_1_2:
            eFBReductionType = VDEC_FB_REDUCTION_1_2;
            break;
        case VDEC_EX_FB_REDUCTION_1_4:
            eFBReductionType = VDEC_FB_REDUCTION_1_4;
            break;
        case VDEC_EX_FB_REDUCTION_NONE:
        default:
            eFBReductionType = VDEC_FB_REDUCTION_NONE;
            break;
    }
    return eFBReductionType;
}

static void _VDEC_MapFBReduction(VDEC_EX_FBReduction* pstExFBReduction, VDEC_FBReduction* pstFBReduction)
{
    pstFBReduction->bEnableAutoMode = pstExFBReduction->bEnableAutoMode;
    pstFBReduction->eLumaFBReduction = _VDEC_MapFBReductionType(pstExFBReduction->eLumaFBReduction);
    pstFBReduction->eChromaFBReduction = _VDEC_MapFBReductionType(pstExFBReduction->eChromaFBReduction);
}

static void _VDEC_MapVideoInfo(VDEC_EX_VideoInfo* pstExVidInfo, VDEC_VideoInfo* pstVidInfo)
{
    MS_U8 i = 0;

    pstVidInfo->eSrcMode           = _VDEC_MapSrcMode(pstExVidInfo->eSrcMode);
    pstVidInfo->eTimeStampType     = _VDEC_MapTimeStampType(pstExVidInfo->eTimeStampType);
    pstVidInfo->eMJpegScaleFactor  = _VDEC_MapMjpegScaleFactor(pstExVidInfo->eMJpegScaleFactor);
    pstVidInfo->bWithoutNalStCode  = pstExVidInfo->bWithoutNalStCode;
    pstVidInfo->u32FrameRate       = pstExVidInfo->u32FrameRate;
    pstVidInfo->u32FrameRateBase   = pstExVidInfo->u32FrameRateBase;
    pstVidInfo->u16NumSizes        = pstExVidInfo->u16NumSizes;

    for (i = 0; i < 8; i++)
    {
        pstVidInfo->u16Width[i]   = pstExVidInfo->u16Width[i];
        pstVidInfo->u16Height[i]  = pstExVidInfo->u16Height[i];
    }
}

static MS_BOOL _VDEC_MapInitParam(VDEC_EX_InitParam* pstExInitPara, VDEC_InitParam* pstInitPara)
{
    pstInitPara->eCodecType = _VDEC_MapCodecType(pstExInitPara->eCodecType);
    _VDEC_MapSysCfg(&pstExInitPara->SysConfig, &pstInitPara->SysConfig);
    _VDEC_MapVideoInfo(&pstExInitPara->VideoInfo, &pstInitPara->VideoInfo);
    pstInitPara->EnableDynaScale       = pstExInitPara->EnableDynaScale;
    pstInitPara->bDisableDropErrFrame  = pstExInitPara->bDisableDropErrFrame;
    pstInitPara->bDisableErrConceal    = pstExInitPara->bDisableErrConceal;
    pstInitPara->bRepeatLastField      = pstExInitPara->bRepeatLastField;
    pstInitPara->u8ErrThreshold        = pstExInitPara->u8ErrThreshold;
    pstInitPara->u32DSVirtualBoxWidth  = pstExInitPara->u32DSVirtualBoxWidth;
    pstInitPara->u32DSVirtualBoxHeight = pstExInitPara->u32DSVirtualBoxHeight ;
    return TRUE;
}

static MS_BOOL _VDEC_MapInitExParam(VDEC_EX_InitParam* pstExInitPara, VDEC_InitParam_EX1* pstInitPara)
{
    pstInitPara->u32Version = pstExInitPara->u32Version;
    pstInitPara->eCodecType = _VDEC_MapCodecType(pstExInitPara->eCodecType);
    _VDEC_MapSysCfg(&pstExInitPara->SysConfig, &pstInitPara->SysConfig);
    _VDEC_MapVideoInfo(&pstExInitPara->VideoInfo, &pstInitPara->VideoInfo);
    pstInitPara->EnableDynaScale       = pstExInitPara->EnableDynaScale;
    pstInitPara->bDisableDropErrFrame  = pstExInitPara->bDisableDropErrFrame;
    pstInitPara->bDisableErrConceal    = pstExInitPara->bDisableErrConceal;
    pstInitPara->bRepeatLastField      = pstExInitPara->bRepeatLastField;
    pstInitPara->u8ErrThreshold        = pstExInitPara->u8ErrThreshold;
    pstInitPara->u32DSVirtualBoxWidth  = pstExInitPara->u32DSVirtualBoxWidth;
    pstInitPara->u32DSVirtualBoxHeight = pstExInitPara->u32DSVirtualBoxHeight ;

    _VDEC_MapFBReduction(&pstExInitPara->stFBReduction, &pstInitPara->stFBReduction);
    return TRUE;
}


static VDEC_User_Cmd _VDEC_MapUserCmd(VDEC_EX_User_Cmd eExUserCmd)
{
    VDEC_User_Cmd eUserCmd = VDEC_USER_CMD_SET_CONTROL_BASE;
    switch (eExUserCmd)
    {
        case E_VDEC_EX_USER_CMD_SET_CONTROL_BASE             : eUserCmd = VDEC_USER_CMD_SET_CONTROL_BASE; break;
        case E_VDEC_EX_USER_CMD_REPEAT_LAST_FIELD            : eUserCmd = VDEC_USER_CMD_REPEAT_LAST_FIELD; break;
        case E_VDEC_EX_USER_CMD_AVSYNC_REPEAT_TH             : eUserCmd = VDEC_USER_CMD_AVSYNC_REPEAT_TH; break;
        case E_VDEC_EX_USER_CMD_DISP_ONE_FIELD               : eUserCmd = VDEC_USER_CMD_DISP_ONE_FIELD; break;
        case E_VDEC_EX_USER_CMD_FD_MASK_DELAY_COUNT          : eUserCmd = VDEC_USER_CMD_FD_MASK_DELAY_COUNT; break;
        case E_VDEC_EX_USER_CMD_FRC_OUTPUT                   : eUserCmd = VDEC_USER_CMD_FRC_OUTPUT; break;
        case E_VDEC_EX_USER_CMD_FRC_DROP_TYPE                : eUserCmd = VDEC_USER_CMD_FRC_DROP_TYPE; break;
        case E_VDEC_EX_USER_CMD_FAST_DISPLAY                 : eUserCmd = VDEC_USER_CMD_FAST_DISPLAY; break;
        case E_VDEC_EX_USER_CMD_IGNORE_ERR_REF               : eUserCmd = VDEC_USER_CMD_IGNORE_ERR_REF; break;
        case E_VDEC_EX_USER_CMD_FORCE_FOLLOW_DTV_SPEC        : eUserCmd = VDEC_USER_CMD_FORCE_FOLLOW_DTV_SPEC; break;
        case E_VDEC_EX_USER_CMD_AVC_MIN_FRM_GAP              : eUserCmd = VDEC_USER_CMD_AVC_MIN_FRM_GAP; break;
        case E_VDEC_EX_USER_CMD_DISABLE_SEQ_CHG              : eUserCmd = VDEC_USER_CMD_DISABLE_SEQ_CHG; break;
        //case E_VDEC_EX_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE   : eUserCmd = VDEC_USER_CMD_SET_DISP_OUTSIDE_CTRL_MODE; break;  /// Org no define
        case E_VDEC_EX_USER_CMD_SET_DTV_USER_DATA_MODE       : eUserCmd = VDEC_USER_CMD_SET_DTV_USER_DATA_MODE; break;
        case E_VDEC_EX_USER_CMD_SET_SINGLE_TASK_MODE         : eUserCmd = VDEC_USER_CMD_SET_SINGLE_TASK_MODE; break;
        case E_VDEC_EX_USER_CMD_AVC_DISABLE_ANTI_VDEAD       : eUserCmd = VDEC_USER_CMD_AVC_DISABLE_ANTI_VDEAD; break;
        case E_VDEC_EX_USER_CMD_DTV_RESET_MVD_PARSER         : eUserCmd = VDEC_USER_CMD_DTV_RESET_MVD_PARSER; break;
        case E_VDEC_EX_USER_CMD_PVR_FLUSH_FRAME_BUFFER       : eUserCmd = VDEC_USER_CMD_PVR_FLUSH_FRAME_BUFFER; break;
        case E_VDEC_EX_USER_CMD_FORCE_INTERLACE_MODE         : eUserCmd = VDEC_USER_CMD_FORCE_INTERLACE_MODE; break;
        case E_VDEC_EX_USER_CMD_RELEASE_FD_MASK              : eUserCmd = VDEC_USER_CMD_RELEASE_FD_MASK; break;
        case E_VDEC_EX_USER_CMD_SET_DECODE_MODE              : eUserCmd = VDEC_USER_CMD_SET_DECODE_MODE; break;
        case E_VDEC_EX_USER_CMD_SUPPORT_AVC_TO_MVC           : eUserCmd = VDEC_USER_CMD_SUPPORT_AVC_TO_MVC; break;
        case E_VDEC_EX_USER_CMD_3DLR_VIEW_EXCHANGE           : eUserCmd = VDEC_USER_CMD_3DLR_VIEW_EXCHANGE; break;
        case E_VDEC_EX_USER_CMD_SET_VSIZE_ALIGN              : eUserCmd = VDEC_USER_CMD_SET_VSIZE_ALIGN; break;
        case E_VDEC_EX_USER_CMD_SHOW_DECODE_ORDER            : eUserCmd = VDEC_USER_CMD_SHOW_DECODE_ORDER; break;
        case E_VDEC_EX_USER_CMD_AVC_DISP_IGNORE_CROP         : eUserCmd = VDEC_USER_CMD_AVC_DISP_IGNORE_CROP; break;
        case E_VDEC_EX_USER_CMD_SET_DISP_FINISH_MODE         : eUserCmd = VDEC_USER_CMD_SET_DISP_FINISH_MODE; break;
        case E_VDEC_EX_USER_CMD_SET_AVSYNC_MODE              : eUserCmd = VDEC_USER_CMD_SET_AVSYNC_MODE; break;
        //case E_VDEC_EX_USER_CMD_SUSPEND_DYNAMIC_SCALE        : eUserCmd = VDEC_USER_CMD_SUSPEND_DYNAMIC_SCALE; break;
        case E_VDEC_EX_USER_CMD_FORCE_AUTO_MUTE              : eUserCmd = VDEC_USER_CMD_FORCE_AUTO_MUTE; break;
        case E_VDEC_EX_USER_CMD_PUSH_DISPQ_WITH_REF_NUM      : eUserCmd = VDEC_USER_CMD_PUSH_DISPQ_WITH_REF_NUM; break;
        case E_VDEC_EX_USER_CMD_SET_IDCT_MODE                : eUserCmd = VDEC_USER_CMD_SET_IDCT_MODE; break;
        case E_VDEC_EX_USER_CMD_SET_XC_LOW_DELAY_PARA        : eUserCmd = VDEC_USER_CMD_SET_XC_LOW_DELAY_PARA; break;
        case E_VDEC_EX_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE  : eUserCmd = VDEC_USER_CMD_AVC_SUPPORT_REF_NUM_OVER_MAX_DPB_SIZE; break;
        case E_VDEC_EX_USER_CMD_AUTO_DROP_DISPLAY_QUEUE      : eUserCmd = VDEC_USER_CMD_AUTO_DROP_DISPLAY_QUEUE; break;
        case E_VDEC_EX_USER_CMD_SET_DV_XC_SHM_ADDR          : eUserCmd = VDEC_USER_CMD_SET_DV_XC_SHM_ADDR ;break;
        case E_VDEC_EX_USER_CMD_SET_ENABLE_HDR              : eUserCmd = VDEC_USER_CMD_SET_ENABLE_HDR ;break;

        case E_VDEC_EX_USER_CMD_MVC_BBU2_PUSH_PACKET         : eUserCmd = VDEC_USER_CMD_MVC_BBU2_PUSH_PACKET; break;
        case E_VDEC_EX_USER_CMD_MVC_BBU2_FIRE_DECCMD         : eUserCmd = VDEC_USER_CMD_MVC_BBU2_FIRE_DECCMD; break;

        //Group2:Get Control command
        case E_VDEC_EX_USER_CMD_GET_CONTROL_BASE             : eUserCmd = VDEC_USER_CMD_GET_CONTROL_BASE; break;
        case E_VDEC_EX_USER_CMD_GET_CHROMA_TYPE              : eUserCmd = VDEC_USER_CMD_GET_CHROMA_TYPE; break;
        case E_VDEC_EX_USER_CMD_GET_REAL_FRAMERATE           : eUserCmd = VDEC_USER_CMD_GET_REAL_FRAMERATE; break;
        case E_VDEC_EX_USER_CMD_GET_COLOR_MATRIX             : eUserCmd = VDEC_USER_CMD_GET_COLOR_MATRIX; break;
        case E_VDEC_EX_USER_CMD_GET_DYNSCALE_ENABLED         : eUserCmd = VDEC_USER_CMD_GET_DYNSCALE_ENABLED; break;
        case E_VDEC_EX_USER_CMD_GET_FPA_SEI                  : eUserCmd = VDEC_USER_CMD_GET_FPA_SEI; break;
        case E_VDEC_EX_USER_CMD_GET_U64PTS                   : eUserCmd = VDEC_USER_CMD_GET_U64PTS; break;
        case E_VDEC_EX_USER_CMD_GET_ORI_INTERLACE_MODE       : eUserCmd = VDEC_USER_CMD_GET_ORI_INTERLACE_MODE; break;
        //case E_VDEC_EX_USER_CMD_GET_MBS_ONLY_FLAG            : eUserCmd = VDEC_USER_CMD_GET_MBS_ONLY_FLAG; break;
        //case E_VDEC_EX_USER_CMD_GET_CRC_VALUE                : eUserCmd = VDEC_USER_CMD_GET_CRC_VALUE; break;
        case E_VDEC_EX_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM   : eUserCmd = VDEC_USER_CMD_GET_FLUSH_PATTEN_ENTRY_NUM; break;
        case E_VDEC_EX_USER_CMD_GET_FW_STATUS_FLAG           : eUserCmd = VDEC_USER_CMD_GET_FW_STATUS_FLAG;break;

        case E_VDEC_EX_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO  : eUserCmd = VDEC_USER_CMD_GET_MVC_SUB_FRAME_DISP_INFO; break;
        case E_VDEC_EX_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY    : eUserCmd = VDEC_USER_CMD_GET_MVC_BBU2_DECQ_VACANCY; break;
        case E_VDEC_EX_USER_CMD_GET_MVC_ES2_READ_PTR         : eUserCmd = VDEC_USER_CMD_GET_MVC_ES2_READ_PTR; break;
        case E_VDEC_EX_USER_CMD_GET_MVC_ES2_WRITE_PTR        : eUserCmd = VDEC_USER_CMD_GET_MVC_ES2_WRITE_PTR; break;
        case E_VDEC_EX_USER_CMD_GET_ES_QUANTITY              : eUserCmd = VDEC_USER_CMD_GET_ES_QUANTITY; break;
        case E_VDEC_EX_USER_CMD_GET_ES2_QUANTITY             : eUserCmd = VDEC_USER_CMD_GET_ES2_QUANTITY; break;
        case E_VDEC_EX_USER_CMD_DROP_ERR_FRAME               : eUserCmd = VDEC_USER_CMD_DROP_ERR_FRAME; break;
        case E_VDEC_EX_USER_CMD_GET_XC_LOW_DELAY_INT_STATE   : eUserCmd = VDEC_USER_CMD_GET_XC_LOW_DELAY_INT_STATE; break;
        case E_VDEC_EX_USER_CMD_GET_FB_USAGE_MEM             : eUserCmd = VDEC_USER_CMD_GET_FB_USAGE_MEM; break;

        //Group3-4:Preset Control command=============================
        case E_VDEC_EX_USER_CMD_PRESET_CONTROL_BASE         : eUserCmd = VDEC_USER_CMD_PRESET_CONTROL_BASE; break;
        case E_VDEC_EX_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE  : eUserCmd = VDEC_USER_CMD_HVD_ONE_PENDING_BUFFER_MODE; break;
        case E_VDEC_EX_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE  : eUserCmd = VDEC_USER_CMD_MVD_HWBUFFER_REMAPPING_MODE; break;

        default:
            break;
    }
    return eUserCmd;
}

VDEC_EX_Result MApi_VDEC_EX_GetFreeStream(void *pInfo, MS_U32 u32Size, VDEC_EX_Stream eStreamType, VDEC_EX_CodecType eCodecType)
{
    VDEC_StreamId *pStreamId = pInfo;
    if(E_VDEC_EX_MAIN_STREAM == eStreamType)
    {
        pStreamId->u32Id = VDEC_MAIN_ID;
        pStreamId->u32Version = stStreamId.u32Version;
        return E_VDEC_EX_OK;
    }
    else
    {
        printf("%s fail, only support main stream\n", __FUNCTION__);
        return E_VDEC_EX_FAIL;
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
VDEC_EX_Result MApi_VDEC_EX_Init(VDEC_StreamId *pStreamId, VDEC_EX_InitParam *pExInitParam)
{
    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_InitParam stInitPara;
    VDEC_InitParam_EX1 stInitPara_EX1;

    if (!pExInitParam)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    if (VDEC_MAIN_ID != pStreamId->u32Id)
    {
        goto _init_ret;
    }

    if((pExInitParam->stFBReduction.eLumaFBReduction != VDEC_EX_FB_REDUCTION_NONE)
    || (pExInitParam->stFBReduction.eChromaFBReduction  != VDEC_EX_FB_REDUCTION_NONE)
    || (pExInitParam->stFBReduction.bEnableAutoMode != FALSE))
    {
        if (!_VDEC_MapInitExParam(pExInitParam, &stInitPara_EX1))
        {
            return E_VDEC_EX_RET_INVALID_PARAM;
        }
        eRetVal = MApi_VDEC_Init_EX(&stInitPara_EX1, sizeof(stInitPara_EX1));
    }
    else
    {
        if (!_VDEC_MapInitParam(pExInitParam, &stInitPara))
        {
            return E_VDEC_EX_RET_INVALID_PARAM;
        }
        eRetVal = MApi_VDEC_Init(&stInitPara);
    }

_init_ret:
    eExRetVal = _VDEC_MapExResult(eRetVal);

    if (E_VDEC_EX_OK == eExRetVal)
    {
        //printf("%s: Update _ExVdecInitParam\n", __FUNCTION__);
        _VDEC_Memcpy(&_ExVdecInitParam, pExInitParam, sizeof(VDEC_EX_InitParam));
    }

#ifdef _MVD_EVENT_DBG
    _VDEC_MVDEventDbg();
#endif

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Reset decoder.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Rst(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;

    eRetVal = MApi_VDEC_Rst();
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Stop playback
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Exit(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_OK;

    eRetVal = MApi_VDEC_Exit();
    eExRetVal = _VDEC_MapExResult(eRetVal);

    stStreamId.u32Version = 0;
    stStreamId.u32Id = VDEC_U32_MAX;

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Check if information for display setting is ready or not
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CheckDispInfoRdy(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_CheckDispInfoRdy());
}

static VDEC_FrcMode _VDEC_MapFrcMode(VDEC_EX_FrcMode eExFrcMode)
{
    VDEC_FrcMode eFrcMode = E_VDEC_FRC_NORMAL;

    switch (eExFrcMode)
    {
        case E_VDEC_EX_FRC_NORMAL:
            eFrcMode = E_VDEC_FRC_NORMAL;
            break;
        case E_VDEC_EX_FRC_DISP_TWICE:
            eFrcMode = E_VDEC_FRC_DISP_TWICE;
            break;
        case E_VDEC_EX_FRC_3_2_PULLDOWN:
            eFrcMode = E_VDEC_FRC_3_2_PULLDOWN;
            break;
        case E_VDEC_EX_FRC_PAL_TO_NTSC:
            eFrcMode = E_VDEC_FRC_PAL_TO_NTSC;
            break;
        case E_VDEC_EX_FRC_NTSC_TO_PAL:
            eFrcMode = E_VDEC_FRC_NTSC_TO_PAL;
            break;
        case E_VDEC_EX_FRC_MODE_50P_60P:
            eFrcMode = E_VDEC_FRC_MODE_50P_60P;
            break;
        case E_VDEC_EX_FRC_MODE_60P_50P:
            eFrcMode = E_VDEC_FRC_MODE_60P_50P;
            break;
        default:
           eFrcMode = E_VDEC_FRC_NORMAL;
            break;
    }

    return eFrcMode;
}

//------------------------------------------------------------------------------
/// Set up frame rate conversion mode
/// @param eFrcMode \b IN : frame rate conversion mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetFrcMode(VDEC_StreamId *pStreamId, VDEC_EX_FrcMode eExFrcMode)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetFrcMode(_VDEC_MapFrcMode(eExFrcMode)));
}

//------------------------------------------------------------------------------
/// Set up some parameters about dynamic scaling in FW
/// @param u32Addr \b IN : the physical start address of parameter set
/// @param u32Size \b IN : the size of parameter set
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetDynScalingParams(VDEC_StreamId *pStreamId, MS_PHYADDR u32Addr, MS_U32 u32Size)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetDynScalingParams(u32Addr, u32Size));
}

//------------------------------------------------------------------------------
/// Start playback
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Play(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_Play());
}

//------------------------------------------------------------------------------
/// Pause decode and display
/// @return return VDEC_Result
//-----------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Pause(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_Pause());
}

//------------------------------------------------------------------------------
/// Resume decode and display
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Resume(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_Resume());
}

//------------------------------------------------------------------------------
/// Trigger to display one frame (background may still decoding)
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_StepDisp(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_StepDisp());
}

//------------------------------------------------------------------------------
/// Check if step play done or not
/// @return VDEC_Result
///     - E_VDEC_OK: step display done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsStepDispDone(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsStepDispDone());
}

//------------------------------------------------------------------------------
/// Decoder will start decode with full  speed until target PTS is reached (equal or larger) then pause.
/// @param u32PTS \b IN : target PTS
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SeekToPTS(VDEC_StreamId *pStreamId, MS_U32 u32PTS)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SeekToPTS(u32PTS));
}

//------------------------------------------------------------------------------
/// Decoder will skip all frames before target PTS, and start decode with full  speed after target PTS is reached.
/// @param u32PTS \b IN : target PTS
///         - 0: turn off this mode.
///         - any not zero: enable this mode.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SkipToPTS(VDEC_StreamId *pStreamId, MS_U32 u32PTS)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SkipToPTS(u32PTS));
}

//------------------------------------------------------------------------------
/// Trigger decoder to decode one frame.
/// Decoder seeks to an I frame, decodes and displays it.
/// @return return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_StepDecode(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_StepDecode());
}

//------------------------------------------------------------------------------
/// Check if step decode done or not
/// @return VDEC_Result
///     - E_VDEC_OK: step decode done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsStepDecodeDone(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsStepDecodeDone());
}

//------------------------------------------------------------------------------
/// Set up trick decode mode for decode I, decode IP and decode all.
/// @param eTrickDec \b IN : trick decode mode
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetTrickMode(VDEC_StreamId *pStreamId, VDEC_EX_TrickDec eExTrickDec)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetTrickMode(_VDEC_MapTrickDecMode(eExTrickDec)));
}

//------------------------------------------------------------------------------
/// Push decode command queue
/// @param pCmd \b IN : the pointer to decode command
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_PushDecQ(VDEC_StreamId *pStreamId, VDEC_EX_DecCmd *pExCmd)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_DecCmd stDecCmd;
    _VDEC_MapDecCmd(pExCmd, &stDecCmd);
    eRetVal = MApi_VDEC_PushDecQ(&stDecCmd);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Fire the decode command which be pushed since last fired
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_FireDecCmd(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_FireDecCmd());
}

//------------------------------------------------------------------------------
/// Get current queue vacancy
/// @return vacancy number
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetDecQVacancy(VDEC_StreamId *pStreamId)
{
    MS_U32 u32DecQVac = 0;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32DecQVac;
    else
        return MApi_VDEC_GetDecQVacancy();
}

MS_BOOL MApi_VDEC_EX_IsCCAvailable(VDEC_StreamId *pStreamId)
{
    MS_BOOL bIsCCAvail = FALSE;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return bIsCCAvail;
    else
        return MApi_VDEC_IsCCAvailable();
}

VDEC_EX_Result MApi_VDEC_EX_GetCCInfo(VDEC_StreamId *pStreamId, void *pInfo, MS_U32 u32Size)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_OK;

    MS_U32 u32Version = *((MS_U32 *) pInfo);

    if (u32Version == 0 && u32Size == sizeof(VDEC_EX_CC_Info))
    {
        VDEC_CC_Info stCCInfo;

        _VDEC_Memset(&stCCInfo, 0, sizeof(VDEC_CC_Info));

        eRetVal = MApi_VDEC_GetCCInfo(&stCCInfo, sizeof(VDEC_CC_Info));

        ((VDEC_EX_CC_Info *)pInfo)->u8PicStructure = stCCInfo.u8PicStructure;
        ((VDEC_EX_CC_Info *)pInfo)->u8TopFieldFirst = stCCInfo.u8TopFieldFirst;
        ((VDEC_EX_CC_Info *)pInfo)->u16TempRef = stCCInfo.u16TempRef;
        ((VDEC_EX_CC_Info *)pInfo)->u32Pts = stCCInfo.u32Pts;
        ((VDEC_EX_CC_Info *)pInfo)->u32UserDataBuf = stCCInfo.u32UserDataBuf;
        ((VDEC_EX_CC_Info *)pInfo)->u32UserDataSize = stCCInfo.u32UserDataSize;
        ((VDEC_EX_CC_Info *)pInfo)->eFrameType = E_VDEC_EX_FRM_TYPE_OTHER;

    eExRetVal = _VDEC_MapExResult(eRetVal);
    }
    else
    {
        VDEC_INFO(printf("%s err: wrong pInfo\n", __FUNCTION__));
        eExRetVal = E_VDEC_EX_RET_INVALID_PARAM;
    }

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Flush command queue and internal data of decoder
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_Flush(VDEC_StreamId *pStreamId, VDEC_EX_FreezePicSelect eExFreezePic)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_Flush(_VDEC_MapFreezePicSel(eExFreezePic)));
}

//------------------------------------------------------------------------------
/// Get ES buffer write pointer
/// @return write pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetESWritePtr(VDEC_StreamId *pStreamId)
{
    MS_U32 u32WrPtr = 0;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32WrPtr;
    else
        return MApi_VDEC_GetESWritePtr();
}

//------------------------------------------------------------------------------
/// Get decoder ES buffer read pointer
/// @return read pointer
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetESReadPtr(VDEC_StreamId *pStreamId)
{
    MS_U32 u32RdPtr = 0;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32RdPtr;
    else
        return MApi_VDEC_GetESReadPtr();
}

//------------------------------------------------------------------------------
/// Enable to let decoder force to display all decoded frame when out of input data
/// @param bEnable \b IN : TRUE - enable; FALSE - disable
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_EnableLastFrameShow(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_EnableLastFrameShow(bEnable));
}

//------------------------------------------------------------------------------
/// Check if all decoded frames are displayed when input stream is run out
/// @return VDEC_Result
///     - E_VDEC_OK: finished
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsDispFinish(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsDispFinish());
}

//------------------------------------------------------------------------------
/// Set up display speed
/// @param eSpeedType \b IN : display speed type
/// @param eSpeed \b IN : multiple
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetSpeed(VDEC_StreamId *pStreamId, VDEC_EX_SpeedType eExSpeedType, VDEC_EX_DispSpeed eExSpeed)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_SpeedType eSpeedType = _VDEC_MapSpeedType(eExSpeedType);
    VDEC_DispSpeed eSpeed = _VDEC_MapDispSpeed(eExSpeed);

    return _VDEC_MapExResult(MApi_VDEC_SetSpeed(eSpeedType, eSpeed));
}

//------------------------------------------------------------------------------
/// Check if the first frame is showed after play function is called
/// @return VDEC_Result
///     - E_VDEC_OK: ready
///     - E_VDEC_FAIL: not ready
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsFrameRdy(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsFrameRdy());
}

//------------------------------------------------------------------------------
/// Let FW stop updating frames when vsync, but decoding process is still going.
/// @param bEnable \b IN : Enable/Disable
/// @return -The result of command freeze image.
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetFreezeDisp(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetFreezeDisp(bEnable));
}

//------------------------------------------------------------------------------
/// Set blue screen on/off
/// @param bOn \b IN : turn on / off blue screen
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetBlueScreen(VDEC_StreamId *pStreamId, MS_BOOL bOn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetBlueScreen(bOn));
}

//------------------------------------------------------------------------------
/// Reset PTS
/// @param u32PtsBase \b IN : new PTS base value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_ResetPTS(VDEC_StreamId *pStreamId, MS_U32 u32PtsBase)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_ResetPTS(u32PtsBase));
}

//------------------------------------------------------------------------------
/// Switch AV sync on/off
/// @param bOn \b IN : on/off
/// @param u32SyncDelay \b IN : delay value
/// @param u16SyncTolerance \b IN : tolerance value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_AVSyncOn(VDEC_StreamId *pStreamId, MS_BOOL bOn, MS_U32 u32SyncDelay, MS_U16 u16SyncTolerance)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_AVSyncOn(bOn, u32SyncDelay, u16SyncTolerance));
}

//------------------------------------------------------------------------------
/// Switch AV sync free run threshold
/// @param u32Threshold \b IN : threshold value
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetAVSyncFreerunThreshold(VDEC_StreamId *pStreamId, MS_U32 u32Threshold )
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetAVSyncFreerunThreshold(u32Threshold));
}


//------------------------------------------------------------------------------
/// Check if AV sync on or not
/// @return VDEC_Result
///     - E_VDEC_OK: AV sync on
///     - E_VDEC_FAIL: freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsAVSyncOn(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsAVSyncOn());
}

//------------------------------------------------------------------------------
/// Get the PTS of current displayed frame
/// @return PTS
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetPTS(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetPTS();
}

//------------------------------------------------------------------------------
/// Get the PTS of next displayed frame
/// @return next PTS (unit:ms)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetNextPTS(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetNextPTS();
}

//------------------------------------------------------------------------------
/// Check if decoder is doing AV sync now
/// @return VDEC_Result
///     - E_VDEC_OK: doing AV sync
///     - E_VDEC_FAIL: still freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsStartSync(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsStartSync());
}

//------------------------------------------------------------------------------
/// Check if the distance of Video time stamp and STC is closed enough.
/// @return VDEC_Result
///     - E_VDEC_OK: sync complete
///     - E_VDEC_FAIL: sync is incomplete
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsReachSync(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsReachSync());
}

//------------------------------------------------------------------------------
/// Check if decoder is avsync freerun
/// @return VDEC_Result
///     - E_VDEC_OK: freerun
///     - E_VDEC_FAIL: not freerun
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsFreerun(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsFreerun());
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, decode one I frame
/// @param u32FrameBufAddr \b IN : output buffer address
/// @param u32SrcSt \b IN : source start address
/// @param u32SrcEnd \b IN : source end address
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_MHEG_DecodeIFrame(VDEC_StreamId *pStreamId,
                                            MS_PHYADDR u32FrameBufAddr,
                                            MS_PHYADDR u32SrcSt,
                                            MS_PHYADDR u32SrcEnd)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_MHEG_DecodeIFrame(u32FrameBufAddr, u32SrcSt, u32SrcEnd));
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, check if I frame decode done
/// @return VDEC_Result
///     - E_VDEC_OK: decode done
///     - E_VDEC_FAIL: not yet
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_MHEG_IsIFrameDecoding(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_MHEG_IsIFrameDecoding());
}

//------------------------------------------------------------------------------
/// For MHEG5 MW, reset decoder after I frame decode done
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_MHEG_RstIFrameDec(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_InitParam stInitPara;
    VDEC_EX_InitParam *pExInitParam = &_ExVdecInitParam;

    eRetVal = MApi_VDEC_Exit();
    if (E_VDEC_OK != eRetVal)
    {
        printf("%s fail to exit 0x%x\n", __FUNCTION__, eRetVal);
    }

    if (!_VDEC_MapInitParam(pExInitParam, &stInitPara))
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    eRetVal = MApi_VDEC_Init(&stInitPara);
    if (E_VDEC_OK != eRetVal)
    {
        printf("%s fail to init 0x%x\n", __FUNCTION__, eRetVal);
    }

    return _VDEC_MapExResult(eRetVal);
}

//------------------------------------------------------------------------------
/// Get error code
/// @return error code
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetErrCode(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetErrCode();
}

//------------------------------------------------------------------------------
/// Get accumulated error counter
/// @return error counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetErrCnt(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetErrCnt();
}

//------------------------------------------------------------------------------
/// Get codec type which be initialed
/// @return VDEC_CodecType
//------------------------------------------------------------------------------
VDEC_EX_CodecType MApi_VDEC_EX_GetActiveCodecType(VDEC_StreamId *pStreamId)
{
    VDEC_EX_CodecType eExCodecType = E_VDEC_EX_CODEC_TYPE_NONE;
    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return eExCodecType;
    else
        return _VDEC_MapExCodecType(MApi_VDEC_GetActiveCodecType());
}

//------------------------------------------------------------------------------
/// Get bitrate which decoder retrieved from stream
/// @return bitrate
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetBitsRate(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetBitsRate();
}

//------------------------------------------------------------------------------
/// Get 3:2 pull down flag which decoder retrieved from stream
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_VDEC_EX_Is32PullDown(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_Is32PullDown();
}

//------------------------------------------------------------------------------
/// Check whether the status of decoder is running or not.
/// @return VDEC_Result: E_VDEC_OK/E_VDEC_RET_NOT_RUNNING/E_VDEC_RET_NOT_INIT
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsAlive(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsAlive());
}

//------------------------------------------------------------------------------
/// Get the value of PTS - STC for video
/// @return the value of PTS - STC for video
//------------------------------------------------------------------------------
MS_S64 MApi_VDEC_EX_GetVideoPtsStcDelta(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetVideoPtsStcDelta();
}

//------------------------------------------------------------------------------
/// Check if decoder got valid stream to decode
/// @return TRUE or FALSE
///     - TRUE: find valid stream
///     - FALSE: not yet
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsWithValidStream(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsWithValidStream());
}

//------------------------------------------------------------------------------
/// Check if decoder found I frame after reset or flush queue
/// @return VDEC_Result
///     - E_VDEC_OK: found
///     - E_VDEC_FAIL: not found
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsIFrameFound(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsIFrameFound());
}

//------------------------------------------------------------------------------
/// Check if input stream is with low delay flag
/// @return VDEC_Result
///     - E_VDEC_OK: with low delay
///     - E_VDEC_FAIL: without low delay
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsWithLowDelay(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsWithLowDelay());
}

//------------------------------------------------------------------------------
/// Check if all date buffers are empty or not
/// @return VDEC_Result
///     - E_VDEC_OK: All of the buffers are empty
///     - E_VDEC_FAIL: Some of the buffers are not empty
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsAllBufferEmpty(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsAllBufferEmpty());
}

//------------------------------------------------------------------------------
/// Get picture counter of current GOP
/// @return picture counter
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_EX_GetGOPCnt(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetGOPCnt();
}

//-----------------------------------------------------------------------------
/// Get accumulated decoded frame Count
/// @return - decoded frame Count
//-----------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetFrameCnt(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetFrameCnt();
}

//------------------------------------------------------------------------------
/// Get skipped counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetSkipCnt(VDEC_StreamId *pStreamId)
{
   _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

   return MApi_VDEC_GetSkipCnt();
}

//------------------------------------------------------------------------------
/// Get dropped frame counter
/// @return counter
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetDropCnt(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetDropCnt();
}

//------------------------------------------------------------------------------
/// Get display information
/// @param pDispinfo \b OUT : pointer to display setting information
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_DispInfo *pExDispinfo)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_DispInfo stDispinfo;

    if (pExDispinfo)
    {
        eRetVal =  MApi_VDEC_GetDispInfo(&stDispinfo);
        if (eRetVal == E_VDEC_OK)
        {
            _VDEC_MapExDispInfo(&stDispinfo, pExDispinfo);
        }
        else
        {
            _VDEC_Memset(pExDispinfo, 0, sizeof(VDEC_DispInfo));
        }
        eExRetVal = _VDEC_MapExResult(eRetVal);
    }
    else
    {
        VDEC_INFO(printf("%s err: null input\n", __FUNCTION__));
        eExRetVal = E_VDEC_EX_RET_INVALID_PARAM;
    }
    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get current trick decode mode of decoder
/// @return VDEC_TrickDec
//------------------------------------------------------------------------------
VDEC_EX_TrickDec MApi_VDEC_EX_GetTrickMode(VDEC_StreamId *pStreamId)
{
    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return E_VDEC_EX_TRICK_DEC_NUM;
    else
        return _VDEC_MapExTrickDecMode(MApi_VDEC_GetTrickMode());
}

//------------------------------------------------------------------------------
/// Get stream active format which decoder retrieved
/// @return active format ID
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_EX_GetActiveFormat(VDEC_StreamId *pStreamId)
{
    MS_U8 u8ActFmt = 0xFF;
    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u8ActFmt;
    else
        return MApi_VDEC_GetActiveFormat();
}

//------------------------------------------------------------------------------
/// Get stream colour primaries
/// @return stream colour primaries
//------------------------------------------------------------------------------
MS_U8 MApi_VDEC_EX_GetColourPrimaries(VDEC_StreamId *pStreamId)
{
    MS_U8 u8ColourPrimaries = 0xFF;
    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u8ColourPrimaries;
    else
        return MApi_VDEC_GetColourPrimaries();
}

extern VDEC_Result MApi_VDEC_GetHWKey(MS_U8 *pu8Key);
VDEC_EX_Result MApi_VDEC_EX_GetHWKey(VDEC_StreamId *pStreamId, MS_U8 *pu8Key)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_GetHWKey(pu8Key);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Check if sequence header is different from previous one
/// @return VDEC_Result
///     - E_VDEC_OK: changed
///     - E_VDEC_FAIL: not changed
///     - E_VDEC_RET_NOT_INIT: not initial yet
///     - E_VDEC_RET_UNSUPPORTED: not supported with current decoder configuration
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_IsSeqChg(VDEC_StreamId *pStreamId)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_IsSeqChg());
}

//------------------------------------------------------------------------------
/// Set up debug message level
/// @param eDbgLevel \b IN : message level
/// @return VDEC_Result
///     - E_VDEC_OK: Success
///     - E_VDEC_FAIL: Failed
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetDbgLevel(VDEC_StreamId *pStreamId, VDEC_EX_DbgLevel eExDbgLevel)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetDbgLevel(_VDEC_MapDbgLevel(eExDbgLevel)));
}

//------------------------------------------------------------------------------
/// Get the information of the latest decoded frame.
/// @param pFrmInfo \b IN : the information of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDecFrameInfo(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfo *pExFrmInfo)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_FrameInfo stFrmInfo;

    if (!pExFrmInfo)
    {
        VDEC_INFO(printf("%s err: null pFrmInfo\n", __FUNCTION__));
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    eRetVal = MApi_VDEC_GetDecFrameInfo(&stFrmInfo);
    if (eRetVal == E_VDEC_OK)
    {
        _VDEC_MapExFrameInfo(&stFrmInfo, pExFrmInfo);
    }
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get the information of current displaying frame.
/// @param pFrmInfo \b IN : the information of displaying frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDispFrameInfo(VDEC_StreamId *pStreamId, VDEC_EX_FrameInfo *pExFrmInfo)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_FrameInfo stFrmInfo;

    if (!pExFrmInfo)
    {
        VDEC_INFO(printf("%s err: null pFrmInfo\n", __FUNCTION__));
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    eRetVal = MApi_VDEC_GetDispFrameInfo(&stFrmInfo);
    if (eRetVal == E_VDEC_OK)
    {
        _VDEC_MapExFrameInfo(&stFrmInfo, pExFrmInfo);
    }
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get the extension information of decoded frame.
/// @param pExtDispinfo \b IN : the extension information of decoded frame.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetExtDispInfo(VDEC_StreamId *pStreamId, VDEC_EX_ExtDispInfo *pExExtDispInfo)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_ExtDispInfo stExtDispInfo;

    if (!pExExtDispInfo)
    {
        VDEC_INFO(printf("%s err: null pExtDispinfo\n", __FUNCTION__));
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    eRetVal = MApi_VDEC_GetExtDispInfo(&stExtDispInfo);
    if (eRetVal == E_VDEC_OK)
    {
        _VDEC_MapExtDispInfo(&stExtDispInfo, pExExtDispInfo);
    }
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest decoded frame.
/// @param pTimeCode \b IN : the time_code of the latest decoded frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDecTimeCode(VDEC_StreamId *pStreamId, VDEC_EX_TimeCode* pExTimeCode)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_TimeCode stTimeCode;

    if (!pExTimeCode)
    {
        VDEC_INFO(printf("%s err: null pExTimeCode\n", __FUNCTION__));
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    eRetVal = MApi_VDEC_GetDecTimeCode(&stTimeCode);

    if (eRetVal == E_VDEC_OK)
    {
        _VDEC_MapExTimeCode(&stTimeCode, pExTimeCode);
    }
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get the time_code of the latest displayed frame.
/// @param pTimeCode \b IN : the time_code of the latest displayed frame
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetDispTimeCode(VDEC_StreamId *pStreamId, VDEC_EX_TimeCode* pExTimeCode)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_TimeCode stTimeCode;
    if (!pExTimeCode)
    {
        return E_VDEC_EX_RET_INVALID_PARAM;
    }

    eRetVal = MApi_VDEC_GetDispTimeCode(&stTimeCode);
    if (eRetVal == E_VDEC_OK)
    {
        _VDEC_MapExTimeCode(&stTimeCode, pExTimeCode);
    }
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// [Obsolete] Start CC data parsing.
/// @param pCCParam \b IN : CC configuration
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_StartParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCCfg *pCCParam)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pCCParam);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Stop CC data parsing.
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_StopParsing(VDEC_StreamId *pStreamId)
{
    printf("%s is obsolete\n", __FUNCTION__);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get write pointer of CC data buffer.
/// @param pu32Write \b OUT : current write pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_GetWritePtr(VDEC_StreamId *pStreamId, MS_U32 *pu32Write)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pu32Write);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get the read pointer of CC data buffer.
/// @param pu32Read \b OUT : current read pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_GetReadPtr(VDEC_StreamId *pStreamId, MS_U32 *pu32Read)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pu32Read);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Update the read pointer of CC data buffer.
/// @param u32EachPacketSize \b IN : new read pointer
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_UpdateReadPtr(VDEC_StreamId *pStreamId, MS_U32 u32EachPacketSize)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(u32EachPacketSize);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

//------------------------------------------------------------------------------
/// [Obsolete] Get if CC data buffer is overflow.
/// @param pbOverflow \b OUT : overflow flag
/// @return -VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_CC_GetIsOverflow(VDEC_StreamId *pStreamId, MS_BOOL *pbOverflow)
{
    printf("%s is obsolete\n", __FUNCTION__);
    UNUSED(pbOverflow);
    return E_VDEC_EX_RET_UNSUPPORTED;
}

VDEC_EX_Result MApi_VDEC_EX_SetEvent(VDEC_StreamId *pStreamId, MS_U32 u32EventFlag, VDEC_EX_EventCb pfn, void* param)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_SetEvent(u32EventFlag, pfn, param);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_UnsetEvent(VDEC_StreamId *pStreamId, MS_U32 u32EventFlag)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_UnsetEvent(u32EventFlag);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// set ISR event CB
/// @param cb_type \b IN : The CB type for VDEC interrupt
/// @param u32EnableEvent \b IN : The event flag for user data CB function
/// @param pfn \b IN : user data CB function
/// @param param \b IN : the param of/for user data CB function
/// @return status
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetEvent_MultiCallback(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag, VDEC_EX_EventCb pfn, void *param)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_SetEvent_EX((VDEC_CB_TYPE)cb_type, u32EventFlag, pfn, param);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Unset ISR event CB
/// @param cb_type \b IN : The CB type for VDEC interrupt
/// @param u32EnableEvent \b IN : The event flag for user data CB function
/// @return status
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_UnsetEvent_MultiCallback(VDEC_StreamId *pStreamId, VDEC_EX_CB_TYPE cb_type, MS_U32 u32EventFlag)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_UnsetEvent_EX((VDEC_CB_TYPE)cb_type, u32EventFlag);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get ISR event status
/// @return status
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_GetEventInfo(VDEC_StreamId *pStreamId, MS_U32* u32EventFlag)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;

    if (u32EventFlag)
    {
        eRetVal = MApi_VDEC_GetEventInfo(u32EventFlag);
        eExRetVal = _VDEC_MapExResult(eRetVal);
    }

    return eExRetVal;
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
VDEC_EX_Result MApi_VDEC_EX_GenPattern(VDEC_StreamId *pStreamId, VDEC_EX_PatternType eExPtnType, MS_PHYADDR u32Addr, MS_U32* u32Size)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    VDEC_PatternType ePtnType = _VDEC_MapPatternType(eExPtnType);
    eRetVal = MApi_VDEC_GenPattern(ePtnType, u32Addr, u32Size);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Get the information of the least length of pattern used for specific function
/// @param pFrmInfo \b IN : the information of the least length of pattern used for specific function
/// @return -the length (Bytes)
//------------------------------------------------------------------------------
MS_U32 MApi_VDEC_EX_GetPatternLeastLength(VDEC_StreamId *pStreamId, VDEC_EX_PatternType eExPatternType)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetPatternLeastLength(_VDEC_MapPatternType(eExPatternType));
}

//------------------------------------------------------------------------------
/// Set deblocking process on/off
/// @param bOn \b IN : turn on / off deblocking process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_DisableDeblocking(VDEC_StreamId *pStreamId, MS_BOOL bDisable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_DisableDeblocking(bDisable));
}

//------------------------------------------------------------------------------
/// Set Quarter Pixel process on/off
/// @param bOn \b IN : turn on / off Quarter Pixel process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_DisableQuarterPixel(VDEC_StreamId *pStreamId, MS_BOOL bDisable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_DisableQuarterPixel(bDisable));
}

//------------------------------------------------------------------------------
/// Turn on/off Auto Remove Last Zero Byte process
/// @param bOn \b IN : turn on / off Auto Remove Last Zero Byte process
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetAutoRmLstZeroByte(VDEC_StreamId *pStreamId, MS_BOOL bOn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetAutoRmLstZeroByte(bOn));
}

//------------------------------------------------------------------------------
/// Set the parameters of Balancing BW
/// @param u8QuarPixelTH \b IN : set Quarter pixel threshold
/// @param u8DeBlockingTH \b IN : set deblocking threshold
/// @param u8UpperBound \b IN : set upper boundary of BW control counter
/// @param u8SafeCoef \b IN : set safety coeffecient of average MB tick.
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_SetBalanceBW(VDEC_StreamId *pStreamId,
                                        MS_U8 u8QuarPixelTH, MS_U8 u8DeBlockingTH, MS_U8 u8UpperBound, MS_U8 u8SafeCoef)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetBalanceBW(u8QuarPixelTH, u8DeBlockingTH, u8UpperBound, u8SafeCoef));
}

extern void MApi_VDEC_DbgDumpStatus(void);
void MApi_VDEC_EX_DbgDumpStatus(VDEC_StreamId *pStreamId)
{
    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return;

    MApi_VDEC_DbgDumpStatus();
}

extern MS_U32 MApi_VDEC_DbgGetProgCnt(void);
MS_U32 MApi_VDEC_EX_DbgGetProgCnt(void)
{
    return MApi_VDEC_DbgGetProgCnt();
}

VDEC_EX_Result MApi_VDEC_EX_SetControl(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, MS_U32 param)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_SetControl(_VDEC_MapUserCmd(cmd_id), param);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_GetControl(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, MS_U32 *param)
{
    VDEC_Result eRetVal  = E_VDEC_OK;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_OK;

    eRetVal = MApi_VDEC_GetControl(_VDEC_MapUserCmd(cmd_id), param);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

MS_U32 MApi_VDEC_EX_GetDispCnt(VDEC_StreamId *pStreamId)
{
    MS_U32 u32Cnt = 0;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32Cnt;
    else
        return MApi_VDEC_GetDispCnt();
}

MS_U32 MApi_VDEC_EX_GetFwVersion(VDEC_StreamId *pStreamId, VDEC_EX_FwType eFwType)
{
    MS_U32 u32FWVer = 0;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32FWVer;
    else
        return MApi_VDEC_GetFwVersion((VDEC_FwType)eFwType);
}

VDEC_EX_Result MApi_VDEC_EX_SetBlockDisplay(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_SetBlockDisplay(bEnable));
}

VDEC_EX_Result MApi_VDEC_EX_EnableESBuffMalloc(VDEC_StreamId *pStreamId, MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_EnableESBuffMalloc(bEnable));
}

MS_U32 MApi_VDEC_EX_GetESBuffVacancy(VDEC_StreamId *pStreamId, void *pData)
{
    _VDEC_WRAP_NOT_INIT_RET_ZERO(pStreamId->u32Id);

    return MApi_VDEC_GetESBuffVacancy(pData);
}

VDEC_EX_Result  MApi_VDEC_EX_GetESBuff(VDEC_StreamId *pStreamId, MS_U32 u32ReqSize, MS_U32 *u32AvailSize, MS_PHYADDR *u32Addr)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_GetESBuff(u32ReqSize, u32AvailSize, u32Addr);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result  MApi_VDEC_EX_GetNextDispFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame **ppDispFrm)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;

    //MS_ASSERT(sizeof(VDEC_EX_DispFrame) == sizeof(VDEC_DispFrame));
    //MS_ASSERT(sizeof(VDEC_EX_FrameInfo) == sizeof(VDEC_FrameInfo));

    eRetVal = MApi_VDEC_GetNextDispFrame((VDEC_DispFrame **)ppDispFrm);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
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
VDEC_EX_Result  MApi_VDEC_EX_DisplayFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_DisplayFrame((VDEC_DispFrame*)pDispFrm);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result  MApi_VDEC_EX_ReleaseFrame(VDEC_StreamId *pStreamId, VDEC_EX_DispFrame *pDispFrm)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_ReleaseFrame((VDEC_DispFrame*)pDispFrm);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

extern VDEC_Result MApi_VDEC_CaptureFrame(MS_U32 u32FrmPriData, MS_BOOL bEnable);
VDEC_EX_Result  MApi_VDEC_EX_CaptureFrame(VDEC_StreamId *pStreamId, MS_U32 u32FrmPriData, MS_BOOL bEnable)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_CaptureFrame(u32FrmPriData, bEnable));
}

VDEC_EX_Result MApi_VDEC_EX_CC_Init(VDEC_StreamId *pStreamId, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_Init(pIn);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_SetCfg(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCType eType, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_SetCfg(_VDEC_MapCCFormat(eFmt), _VDEC_MapCCType(eType), pIn);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_SetBuffStartAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_SetBuffStartAdd(_VDEC_MapCCFormat(eFmt), pIn);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_UpdateWriteAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_UpdateWriteAdd(_VDEC_MapCCFormat(eFmt), pIn);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_UpdateReadAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, MS_U32 *pIn)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_UpdateReadAdd(_VDEC_MapCCFormat(eFmt), pIn);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_DisableParsing(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_DisableParsing(_VDEC_MapCCFormat(eFmt));
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

extern VDEC_Result MApi_VDEC_CC_GetInfo(VDEC_CCFormat eFmt, VDEC_CCInfoCmd eCmd, MS_U32 *pOut);
VDEC_EX_Result MApi_VDEC_EX_CC_GetInfo(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt, VDEC_EX_CCInfoCmd eCmd, MS_U32 *pOut)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_GetInfo(_VDEC_MapCCFormat(eFmt), _VDEC_MapCCInfoCmd(eCmd), pOut);
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_GetIsRstDone(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_GetIsRstDone(_VDEC_MapCCFormat(eFmt));
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

VDEC_EX_Result MApi_VDEC_EX_CC_GetIsBuffOverflow(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    VDEC_Result eRetVal  = E_VDEC_FAIL;
    VDEC_EX_Result eExRetVal = E_VDEC_EX_FAIL;
    eRetVal = MApi_VDEC_CC_GetIsBuffOverflow(_VDEC_MapCCFormat(eFmt));
    eExRetVal = _VDEC_MapExResult(eRetVal);

    return eExRetVal;
}

//------------------------------------------------------------------------------
/// Report the physical address of write pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHYADDR
//------------------------------------------------------------------------------
MS_PHYADDR MApi_VDEC_EX_CC_GetWriteAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    MS_PHYADDR u32CCWrPtr = VDEC_U32_MAX;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32CCWrPtr;
    else
        return MApi_VDEC_CC_GetWriteAdd(_VDEC_MapCCFormat(eFmt));
}

//------------------------------------------------------------------------------
/// Report the physical address of read pointer
/// @param eFmt \b IN : the format of closed caption
/// @return MS_PHYADDR
//------------------------------------------------------------------------------
MS_PHYADDR MApi_VDEC_EX_CC_GetReadAdd(VDEC_StreamId *pStreamId, VDEC_EX_CCFormat eFmt)
{
    MS_PHYADDR u32CCRdPtr = VDEC_U32_MAX;

    if(VDEC_MAIN_ID != pStreamId->u32Id)
        return u32CCRdPtr;
    else
        return MApi_VDEC_CC_GetReadAdd(_VDEC_MapCCFormat(eFmt));

}

//------------------------------------------------------------------------------
/// Setup cmd for presetting vdec
/// @param param \b IN : input parameter
/// @return VDEC_Result
//------------------------------------------------------------------------------
VDEC_EX_Result MApi_VDEC_EX_PreSetControl(VDEC_StreamId *pStreamId, VDEC_EX_User_Cmd cmd_id, MS_U32 param)
{
    _VDEC_WRAP_NOT_INIT_RET(pStreamId->u32Id);

    return _VDEC_MapExResult(MApi_VDEC_PreSetControl(_VDEC_MapUserCmd(cmd_id),param));
}

