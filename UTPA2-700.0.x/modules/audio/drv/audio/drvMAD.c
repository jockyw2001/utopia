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
//******************************************************************************
//  Copyright (c) 2008-2009 MStar Semiconductor, Inc.
//  All rights reserved.
//
//  [Module Name]:
//      drvMAD.c
//  [Abstract]:
//      This module contains code for Audio 1st Decoder driver
//      procedure and subroutin
//
//  [for Doxygen]
/// file drvMAD.c
/// @brief Subroutine for DVB
/// @author MStarSemi Inc.
//*******************************************************************************

#ifndef _DRVMAD_C_
#define _DRVMAD_C_

#include "ULog.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"


#include "drvAUDIO.h"

#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvSIF.h"
#include "./internal/drvAudio_Common.h"
#include "./internal/drvAUDIO_internal.h"

#include "halAUDIO.h"
#include "halMAD.h"
#include "regAUDIO.h"
#if (defined(CHIP_JANUS) || defined(CHIP_MARIA10) || defined(CHIP_MACAW12) || defined(CHIP_EDEN) || defined(CHIP_EULER))
#else
#include "halMAD2.h"
#endif
#include "halSIF.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DRVMAD_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if ( MDrv_AUDIO_SHM_Init() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

extern AUDIO_SHARED_VARS2    * g_AudioVars2;
//====== STAOS PIP START 2012/02/12 ========

pFuncPtr_SetAudioParam2  g_FuncPrt_Hal_SetAudioParam2;
pFuncPtr_GetAudioInfo2   g_FuncPrt_Hal_GetAudioInfo2;

//====== STAOS PIP END 2012/02/12 ========

MS_BOOL (*g_FuncPtr_CheckCapability)(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus) = NULL;
//Temp mask by Cathy extern MS_U8 CheckIPControlBit(MS_U8 u8Bit);




//=====================================

MS_U8  DVB_IP_CONTROL_BIT[] =
{
    255,                         //MPEG
    11,                          //AC3
    12,                          //AC3P
    15,                          //DTSDec
    255,                         //AAC
    255,                         //MP3
    255,                         //WMA
    255,                         //CDLPCM
    255,                         //RA8LBR
    255,                         //XPCM
    255,                         //AC3BP
    255,                         //NONE
};


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD SYSETM Relational Driver Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_Init()
/// @brief \b Function \b Description:  This routine is the initialization for DVB module
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_Init(void)
{

//====== STAOS PIP START 2012/02/12 ========

    g_FuncPrt_Hal_SetAudioParam2 = NULL;
    g_FuncPrt_Hal_GetAudioInfo2 = NULL;

//====== STAOS PIP END 2012/02/12 ========

    HAL_MAD_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_SetMemInfo()
/// @brief \b Function \b Description:  This routine is used to set the DEC-DSP memory information
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_SetMemInfo(void)
{
    HAL_MAD_SetMemInfo();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_DisEn_MIUREQ(void)
{
    HAL_MAD_DisEn_MIUREQ();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_Dis_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_Dis_MIUREQ(void)
{
    HAL_MAD_Dis_MIUREQ();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_RSTMAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_RSTMAD_DisEn_MIUREQ(void)
{
    HAL_MAD_RSTMAD_DisEn_MIUREQ();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_LoadCode()
/// @brief \b Function \b Description: This routine is used to load the DVB algorithm code
/// @param u8Type      \b algorithm type
/// @return MS_BOOL    \b TRUE --DSP load code okay
///                       FALSE--DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MAD_LoadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    return HAL_MAD_LoadCode(dspCodeType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_SetAdMixMode()
/// @brief \b Function \b Description: This routine is used to set MPEG mixed with AD or AD mixed with MPEG or not.
/// @param u8Mix_mode  \b MAIN_MIX_AD 0--main mix to AD
///                       AD_MIX_MAIN 1--AD mix to main
/// @param u8EnMix     \b MIX_OFF 0--disable AD mix
///                       MIX_ON 1--enable AD mix
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_SetAdMixMode (AD_MIX_MODE u8Mix_mode, MIX_ONOFF u8EnMix)
{
    HAL_MAD_SetAdMixMode(u8Mix_mode, (MS_BOOL)u8EnMix);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUD_Monitor_SPDIF_NONPCM_SmpRate()
/// @brief \b Function \b Description: This routine is used to set reference ADC clock by DD+ dsp code
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUD_Monitor_SPDIF_NONPCM_SmpRate(void)
{
    HAL_MAD_Monitor_DDPlus_SPDIF_Rate();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetCertMode()
/// @brief \b Function \b Description:
/// @param CertType    \b :
/// @param enable      \b :
///
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SetCertMode( Audio_CertMode CertType, Audio_CertEnable enCert )
{
    return(HAL_AUDIO_SetCertMode(CertType, enCert));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_MM2_initAesInfo()
/// @brief \b Function \b Description  : Initialize AES info
/// @param <IN>        \b dec_id  : Decoder ID
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID  dec_id )
{
    return HAL_MAD_MM2_initAesInfo( dec_id );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_MM2_checkAesInfo()
/// @brief \b Function \b Description  : Get AES info from audio decoder
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b aes_info     : AES info structure pointer
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{
    return HAL_MAD_MM2_checkAesInfo( dec_id,  aes_info);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_MM2_inputAesFinished()
/// @brief \b Function \b Description  : Inform DSP how much ES data is sent
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b es_size  : Size of ES data
/// @param <IN>        \b ptsExist : Whether ES data contains PTS or not
/// @param <IN>        \b pts      : PTS
/// @param <OUT>       \b MS_BOOL  : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{
    return HAL_MAD_MM2_inputAesFinished( dec_id,  es_size,  ptsExist,  pts);
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD Common Relational Driver Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetMpegInfo()
/// @brief \b Function \b Description: This routine is used for adjust MPEG Audio decoder parameters
/// @return MS_BOOL    \b : True / False
//******************************************************************************
MS_BOOL MDrv_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    MS_BOOL result = TRUE;//UT

    if (infoType == Audio_Comm_infoType_ADEC1_setAudioDecoder)
    {
        switch((MMA_AudioType)param1)
        {
            case Audio_DEC_PCM:
            case Audio_DEC_XPCM:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_XPCM);        break;
            case Audio_DEC_MPEG:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_MPEG);        break;
            case Audio_DEC_MP3:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_MP3);         break;
            case Audio_DEC_AC3:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3);         break;
            case Audio_DEC_AC3P:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_AC3P);        break;
            case Audio_DEC_AAC_LC:
            case Audio_DEC_HEAAC_V1:
            case Audio_DEC_HEAAC_V2:    MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_AAC);         break;
            case Audio_DEC_DOLBYPULSE:  MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_MS10_DDT);    break;
            case Audio_DEC_WMA:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_WMA);         break;
            case Audio_DEC_WMAPRO:      MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_WMA_PRO);     break;
            case Audio_DEC_RA8:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_RA8LBR);      break;
            case Audio_DEC_DTS:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_DTS);         break;
            case Audio_DEC_FLAC:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_FLAC);        break;
            case Audio_DEC_VORBIS:      MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_VORBIS);      break;
            case Audio_DEC_AMR_NB:      MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_AMR_NB);      break;
            case Audio_DEC_AMR_WB:      MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_AMR_WB);      break;
            case Audio_DEC_DRA:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_DRA);      break;
            case Audio_DEC_Dolby_TrueHD_Bypass_R2:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_DolbyTrueHDBypass);      break;
            case Audio_DEC_DTS_HD_ADO_R2:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB_DTSHDADO);      break;
            case Audio_DEC_KTV:
            case Audio_DEC_KTV2:
            default:
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Unsupported Audio type\n");
                result = FALSE;//UT
                break;
        }
    }
    else if (infoType == Audio_Comm_infoType_ADEC2_setAudioDecoder)
    {
        switch((MMA_AudioType)param1)
        {
            case Audio_DEC_PCM:
            case Audio_DEC_XPCM:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_XPCM);      break;
            case Audio_DEC_MPEG:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_MPEG);      break;
            case Audio_DEC_MP3:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_MP3);       break;
            case Audio_DEC_AC3:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_AC3);       break;
            case Audio_DEC_AC3P:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_AC3P);      break;
            case Audio_DEC_AAC_LC:
            case Audio_DEC_HEAAC_V1:
            case Audio_DEC_HEAAC_V2:    MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_GAAC);       break;
            case Audio_DEC_DOLBYPULSE:  MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_AAC);    break;
            case Audio_DEC_WMA:
            case Audio_DEC_WMAPRO:
            case Audio_DEC_RA8:
            case Audio_DEC_DTS:
            case Audio_DEC_FLAC:
            case Audio_DEC_KTV:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_KTV);       break;
            case Audio_DEC_KTV2:        MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_KTV2);      break;
            case Audio_DEC_Dolby_TrueHD_Bypass_R2:     MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_DolbyTrueHDBypass);      break;
            case Audio_DEC_DTS_HD_ADO_R2:         MDrv_AUDIO_SetSystem(MSAPI_AUD_DVB2_DTSHDADO);      break;
            case Audio_DEC_DRA:
            default:
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Unsupported Audio type\n");
                result = FALSE;//UT
                break;
        }
    }
    else if (infoType == Audio_Comm_infoType_ADEC3_setAudioDecoder)
    {
        switch((MMA_AudioType)param1)
        {
            case Audio_DEC_PCM:
            case Audio_DEC_XPCM:
            case Audio_DEC_MPEG:
            case Audio_DEC_MP3:
            case Audio_DEC_AC3:
            case Audio_DEC_AC3P:
            case Audio_DEC_AAC_LC:
            case Audio_DEC_HEAAC_V1:
            case Audio_DEC_HEAAC_V2:
            case Audio_DEC_DOLBYPULSE:
            case Audio_DEC_WMA:
            case Audio_DEC_WMAPRO:
            case Audio_DEC_RA8:
            case Audio_DEC_DTS:
            case Audio_DEC_FLAC:
            case Audio_DEC_KTV:
            case Audio_DEC_KTV2:
            case Audio_DEC_DRA:
            default:
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Unsupported Audio type\n");
                result = FALSE;//UT
                break;
        }
    }
    else if (infoType == Audio_comm_infoType_Set_Input_Path)
    {
        MDrv_AUDIO_SetInputPath((AUDIO_INPUT_TYPE)param1, (AUDIO_PATH_TYPE)param2);
    }
    else
    {
        result = HAL_MAD_SetCommInfo(infoType, param1, param2);
    }

    return result;
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetMpegInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U64 MDrv_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
{
    return HAL_MAD_GetCommInfo(infoType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_SetEncCmd()    // kochien added for PVR
/// @brief \b Function \b Description: This routine is to set encode command.
/// @param u8DecCmd    \b command type   :  AU_DVB_ENCCMD_STOP,      //0
///                                         AU_DVB_ENCCMD_PLAY,      //1
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_SetEncCmd(AU_DVB_ENCCMD u8EncCmd)
{
    HAL_MAD_SetEncCmd(u8EncCmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetEncodeInit()
/// @brief \b Function \b Description           :  This function is used to init ecode parameters
/// @param input       \b Encode Quality(Output Bitrate)-- 0:(default) 192kBps,  1:384kB,  2:128kB, 3:64kB
///                    \b Encode Frame Number   :  how many frame number per interrupt
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_SetEncodeInit(AU_DVB_ENCBR EncBitrate, MS_U8 u8EncFrameNum)
{
    HAL_MAD_EncodeInit(EncBitrate, u8EncFrameNum);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetEncodeFrameInfo()
/// @brief \b Function \b Description   :  This function is used to get the Encoded frame info
/// @param input       \b FrameInfo     :  structure to store encoded frame info
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo)
{
    HAL_AUDIO_GetEncodeFrameInfo(FrameInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_SetDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB1 decode command.
/// @param u8DecCmd    \b command type   :
///                                AU_DVB_DECCMD_STOP = 0,
///                                AU_DVB_DECCMD_PLAY = 1,
///                                AU_DVB_DECCMD_PLAYFILETSP = 2,
///                                AU_DVB_DECCMD_RESYNC,
///                                    ....etc
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MAD_DecoderCmd(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd)
{
    DRVMAD_CHECK_SHM_INIT;

    HAL_MAD_SetDecCmd2(DecId, u8DecCmd);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_SetDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB1 decode command.
/// @param u8DecCmd    \b command type   :
///                                AU_DVB_DECCMD_STOP = 0,
///                                AU_DVB_DECCMD_PLAY = 1,
///                                AU_DVB_DECCMD_PLAYFILETSP = 2,
///                                AU_DVB_DECCMD_RESYNC,
///                                    ....etc
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD_SetDecCmd(AU_DVB_DECCMD u8DecCmd)
{
    HAL_MAD_SetDecCmd(u8DecCmd);
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function      \b Name: MDrv_MAD_GetDecCmd()
/// @brief \b Function      \b Description: This routine is to get  the DVB1 decode command.
/// @return AU_DVB_DECCMD   \b command type:    AU_DVB_DECCMD_STOP = 0,
///                                             AU_DVB_DECCMD_PLAY = 1,
///                                             AU_DVB_DECCMD_PLAYFILETSP = 2,
///                                             AU_DVB_DECCMD_RESYNC,
///                                             ....etc
///////////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD MDrv_MAD_GetDecCmd(En_DVB_AudioDecoder AdoDecoderNo)
{
    return (AU_DVB_DECCMD) (HAL_MAD_GetDecCmd(AdoDecoderNo)&0xF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_Rpt_DTVES()
/// @brief \b Function \b Description: This routine is used to report AC3/MPEG stream if exist or not.
/// @return MS_U8      \b the decoder status
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_MAD_Rpt_DTVES(void)
{
    return (HAL_MAD_Rpt_DTVES());
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD MPEG Relational Driver Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetMpegInfo()
/// @brief \b Function \b Description: This routine is used for adjust MPEG Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MDrv_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    return HAL_MAD_SetMpegInfo( infoType, param1, param2 );
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetMpegInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 MDrv_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
{
    return HAL_MAD_GetMpegInfo( infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetWmaInfo()
/// @brief \b Function \b Description: This routine is used for adjust WMA Audio decoder parameters
/// @return MS_BOOL    \b : True
///                         False
//******************************************************************************
MS_BOOL MDrv_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    return HAL_MAD_SetWmaInfo( infoType, param1, param2 );
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetWmaInfo()
/// @brief \b Function \b Description: This routine will return the 32bit wma info according to info type
/// @return MS_U16     \b : 32bit info
//******************************************************************************
MS_U32 MDrv_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
{
    return HAL_MAD_GetWmaInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetAC3Info()
/// @brief \b Function \b Description: This routine is used for adjust AC3 Audio decoder parameters
/// @return MS_BOOL    \b : True
///                         False
//******************************************************************************
MS_BOOL MDrv_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    return HAL_MAD_SetAC3Info( infoType, param1, param2 );
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetAC3Info()
/// @brief \b Function \b Description: This routine will return the 32bit ac3 info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 MDrv_AUDIO_GetAC3Info( Audio_AC3_infoType infoType )
{
    return HAL_MAD_GetAC3Info(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetAC3PInfo()
/// @brief \b Function \b Description: This routine is used for adjust AC3 Audio decoder parameters
/// @return MS_BOOL    \b : True
///                         False
//******************************************************************************
MS_BOOL MDrv_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    return HAL_MAD_SetAC3PInfo( infoType, param1, param2 );
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetAC3PInfo()
/// @brief \b Function \b Description: This routine will return the 32bit ac3 info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 MDrv_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
{
    return HAL_MAD_GetAC3PInfo( infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetDTSCommonCtrl()
/// @brief \b Function \b Description: This routine is used for adjust DTS CA Control
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL MDrv_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    return HAL_MAD_SetDTSCommonCtrl( infoType, ctrl_type );
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetDTSInfo()
/// @brief \b Function \b Description: This routine will return the 32bit DTS info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 MDrv_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
{
    return HAL_MAD_GetDTSInfo( infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_SetAACInfo()
/// @brief \b Function \b Description: This routine is used for adjust AAC Audio decoder parameters
/// @return MS_BOOL    \b : True
///                         False
//******************************************************************************
MS_U32 MDrv_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    return HAL_MAD_SetAACInfo( infoType, param1, param2 );
}

//******************************************************************************
/// @brief \b Function \b Name: MDrv_AUDIO_GetAACInfo()
/// @brief \b Function \b Description: This routine will return the 32bit AAC info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 MDrv_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
{
    return HAL_MAD_GetAACInfo( infoType );
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD OTHER Relational Drv Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_XPCM_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType   \b type:               1--lpcm
///                                           2--MS-ADPCM
///                                           17--IMA-ADPCM
/// @param Channels     \b  channel num     : 0-- mono
///                                           1-- stereo
/// @param sampleRate   \b  sample rate     : 8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param bitsPerSample\b  bit per sample  : 8--8 bits
///                                           16--16 bits
/// @param blockSize    \b  block size
/// @param samplePerBlock \b samples per block
/// @return MS_U8       \b  TRUE--parameters are correct
///                         FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_MAD_XPCM_setParam (XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
     return HAL_MAD_XPCM_setParam(audioType, channels, sampleRate,
                                                   bitsPerSample, blockSize,  samplePerBlock);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD_RA8_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing RA8 LBR data
/// @param mNumCodecs  \b 1--stereo or mono
///                       2~5-- multi channels
/// @param mSamples    \b output samples--256, 512, 1024
/// @param mSampleRate \b 8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param Channels    \b 1-- mono
///                       2-- stereo
/// @param cplStart    \b 0 ~ 51
/// @param Regions     \b 0 ~ 31
/// @param cplQbits    \b 0, 2, 3, 4, 5, 6
/// @param FrameSize   \b bitstream size of every NumCodes (bytes)
///                       Range--0 ~ 65535
/// @return MS_U8      \b TRUE--parameters are correct
///                       FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                            MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                            MS_U16* FrameSize)
{
    if((Channels == NULL) || (Regions == NULL )|| (cplStart == NULL) || (cplQbits  == NULL) || (FrameSize == NULL))
        return FALSE;//UT

    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_NumCodes,   (MS_U32) mNumCodecs,   0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Samples,    (MS_U32) mSamples,     0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_SampleRate, (MS_U32) mSampleRate,  0);

    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Channels1,  (MS_U32) Channels[0],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Regions1,   (MS_U32) Regions[0],   0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplStart1,  (MS_U32) cplStart[0],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplQbits1,  (MS_U32) cplQbits[0],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_FrameSize1, (MS_U32) FrameSize[0], 0);

    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Channels2,  (MS_U32) Channels[1],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Regions2,   (MS_U32) Regions[1],   0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplStart2,  (MS_U32) cplStart[1],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplQbits2,  (MS_U32) cplQbits[1],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_FrameSize2, (MS_U32) FrameSize[1], 0);

    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Channels3,  (MS_U32) Channels[2],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Regions3,   (MS_U32) Regions[2],   0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplStart3,  (MS_U32) cplStart[2],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplQbits3,  (MS_U32) cplQbits[2],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_FrameSize3, (MS_U32) FrameSize[2], 0);

    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Channels4,  (MS_U32) Channels[3],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Regions4,   (MS_U32) Regions[3],   0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplStart4,  (MS_U32) cplStart[3],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplQbits4,  (MS_U32) cplQbits[3],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_FrameSize4, (MS_U32) FrameSize[3], 0);

    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Channels5,  (MS_U32) Channels[4],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_Regions5,   (MS_U32) Regions[4],   0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplStart5,  (MS_U32) cplStart[4],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_cplQbits5,  (MS_U32) cplQbits[4],  0);
    HAL_MAD_SetRA8LBRInfo(Audio_RA8LBR_FrameSize5, (MS_U32) FrameSize[4], 0);

    return TRUE;
}

//====== STAOS PIP END 2012/02/01 ========

MS_BOOL MDrv_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    if ( g_FuncPrt_Hal_GetAudioInfo2 )
    {
        return g_FuncPrt_Hal_GetAudioInfo2(DecId, infoType, pInfo);
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: g_FuncPrt_Hal_GetAudioInfo2 is NULL !!\r\n", __FUNCTION__);
    return FALSE;
}

//====== STAOS PIP END 2012/02/01 ========

MS_BOOL MDrv_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus)
{
    if (g_FuncPtr_CheckCapability != NULL)
    {
        return g_FuncPtr_CheckCapability(eDecId, p_AudioDecStatus);
    }
    else
    {
        return FALSE;
    }
}

#endif  //#ifndef _DRVMAD_C_