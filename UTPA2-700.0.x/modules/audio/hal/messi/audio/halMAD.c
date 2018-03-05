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
#include "ULog.h"
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include <string.h>

// Internal Definition
#include "regCHIP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSOUND.h"
#include "halSIF.h"
#include "ddr_config.h"
#include "../../../drv/audio/internal/drvAUDIO_internal.h"

#define OMX_AUDIO_DEBUG  0

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

#define HALMAD_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (HAL_AUDIO_InitialVars() == FALSE) \
            { \
                MS_ASSERT(0);\
            } \
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define H2BYTE(value)             ((MS_U8)((value) / 0x10000))
#define HIBYTE(value)             ((MS_U8)((value >>8) & 0x0000FF))
#define LOBYTE(value)             ((MS_U8)((value) & 0x0000FF))

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

AES_INFO audio_aes_info[1];

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SOURCE_INFO_TYPE AudioInputSrcInfo;
extern MS_VIRT                 _gMIO_MapBase;
extern MS_U32                 g_audioDumpInfo_interval;
extern AUDIO_SPDIF_OUTPUT_TYPE gSpdifOutputType;
extern MS_PHY g_DSPDDRBinBaseAddress[MAX_AUDIO_BASE_ADDRESS_NUM2];
MS_U32                        g_AudioDumpInfoCtrl = Audio_DumpInfo_Off;

extern MS_S32  _s32MutexLoadCode;
extern MS_S32  _s32AUDIOMutexIDMA;

//====== STAOS PIP START 2012/02/12 ========

extern pFuncPtr_SetAudioParam2  g_FuncPrt_Hal_SetAudioParam2;
extern pFuncPtr_GetAudioInfo2   g_FuncPrt_Hal_GetAudioInfo2;

//====== STAOS PIP END 2012/02/12 ========

MS_U32 g_ac3_delay = AC3_AV_DEFAULT_DELAY;
MS_U32 g_aac_delay = AAC_AV_DEFAULT_DELAY;
MS_U32 g_mpeg_delay = MPG_AV_DEFAULT_DELAY;

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDRAInfo()
/// @brief \b Function \b Description: This routine return info type of dra decoder
/// @param <IN>        \b Audio_DRA_infoType    : dra parameter type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32 :    dra paramter info
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetDRAInfo( Audio_DRA_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_DRA_infoType_SampleRate:
            {
                MS_U8 SampleRateIndex = 0;

                SampleRateIndex  = (MS_U8)HAL_MAD2_Read_DSP_sram(DSP1DmAddr_dra_nSampleRateIndex, DSP_MEM_TYPE_DM);        //DD+
                switch(SampleRateIndex)
                {
                    case 0: result = SAMPLE_RATE_8000; break;
                    case 1: result = SAMPLE_RATE_11025; break;
                    case 2: result = SAMPLE_RATE_12000; break;
                    case 3: result = SAMPLE_RATE_16000; break;
                    case 4: result = SAMPLE_RATE_22050; break;
                    case 5: result = SAMPLE_RATE_24000; break;
                    case 6: result = SAMPLE_RATE_32000; break;
                    case 7: result = SAMPLE_RATE_44100; break;
                    default:
                    case 8: result = SAMPLE_RATE_48000; break;
                }
            }
            break;

        case Audio_DRA_infoType_NumCh:
            {
                MS_U8 NumNormalCh = 0;
                MS_U8 NumLfeCh = 0;

                NumNormalCh  = (MS_U8)HAL_MAD2_Read_DSP_sram(DSP1DmAddr_dra_nNumNormalCh, DSP_MEM_TYPE_DM);
                NumLfeCh  = (MS_U8)HAL_MAD2_Read_DSP_sram(DSP1DmAddr_dra_nNumLfeCh, DSP_MEM_TYPE_DM);
                if(NumNormalCh == 1)
                    result = DRA_Mono;
                else if(NumNormalCh == 2 && NumLfeCh == 0)
                    result = DRA_Stereo;
                else if(NumNormalCh == 2 && NumLfeCh == 1)
                    result = DRA_2_1;
                else if(NumNormalCh == 3 && NumLfeCh == 0)
                    result = DRA_3_0;
                else if(NumNormalCh == 3 && NumLfeCh == 1)
                    result = DRA_3_1;
                else if(NumNormalCh == 4 && NumLfeCh == 0)
                    result = DRA_4_0;
                else if(NumNormalCh == 4 && NumLfeCh == 1)
                    result = DRA_4_1;
                else if(NumNormalCh == 5 && NumLfeCh == 0)
                    result = DRA_5_0;
                else if(NumNormalCh == 5 && NumLfeCh == 1)
                    result = DRA_5_1;
            }
            break;

        default:
//            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//static MS_BOOL EncodeInProcess;
//#define ENC_BUF_SIZE 6;
extern AUDIO_SHARED_VARS2 *g_AudioVars2;
#ifndef MSOS_TYPE_NUTTX
extern AUDIO_TEE_INFO_SHARE_MEM   *pAudioTeeInfoShm;
extern MS_BOOL g_bAudioTeeEnabled, g_bAudioTeeMbxInitialized;
extern MS_BOOL g_bDecDspSecureTeeAccessed, g_bSndDspSecureTeeAccessed;
#endif
extern MS_BOOL (*g_FuncPtr_CheckCapability)(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus);

MS_U32  EncFrameIdx;
MS_U32  EncBuf_W_idx;
MS_U32  EncBuf_R_idx;
MS_U8   EncBuf_Count;
MS_U16  Reg_REG_SOUND_MAIN_SNDEFFECT = 0x0;
MS_U16  Reg_REG_SOUND_ADV_CFG0 = 0x0;
MS_BOOL Flag_MAIN_SNDEFFET_CHG = FALSE;

AU_DVB_ENC_FRAME_INFO MPEG_EN_BUF[6];
DOLBY_DRC_MODE g_drcMode = RF_MODE;

static MS_U32 g_ReadTargetAddr=0x2D00;
MS_S32 uniPlay_ackCnt = 0;

#define MS_CHANGE_TO_TIME_STAMP     90

#define LAYER_MIN   1
#define LAYER_MAX   3
#define MPEG_BITRATE_TABLE_ROW_NUM  6
#define MPEG_BITRATE_TABLE_COL_NUM  16

#define AUDIO_HASHKEY_DISABLE_AC3                 0x1
#define AUDIO_HASHKEY_DISABLE_AC3P                0x2
#define AUDIO_HASHKEY_DISABLE_DDCO                0x4
#define AUDIO_HASHKEY_DISABLE_DTS                 0x8
#define AUDIO_HASHKEY_DISABLE_DOLBY_MS10          0x10
#define AUDIO_HASHKEY_DISABLE_WMA                 0x20
#define AUDIO_HASHKEY_DISABLE_DRA                 0x40
#define AUDIO_HASHKEY_DISABLE_DTSLBR              0x80
#define AUDIO_HASHKEY_DISABLE_GAAC                0x100
#define AUDIO_HASHKEY_DISABLE_DOLBY_MS11          0x200
#define AUDIO_HASHKEY_DISABLE_DTSE                0x400
#define AUDIO_HASHKEY_DISABLE_DTS_NEO_ULTRA       0x800
#define AUDIO_HASHKEY_DEMO_MODE                   0x1000
#define AUDIO_HASHKEY_DISABLE_SRS_TSHD            0x2000
#define AUDIO_HASHKEY_DISABLE_SRS_THEATERSOUND    0x4000
#define AUDIO_HASHKEY_DISABLE_DTS_STUDIO_SOUND_3D 0x8000
#define AUDIO_HASHKEY_DISABLE_COOK                0x10000
#define AUDIO_HASHKEY_DISABLE_DTS_XLL             0x20000
#define AUDIO_HASHKEY_DISABLE_MS12_LC_PROFILE     0x40000
#define AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE      0x80000
#define AUDIO_HASHKEY_DISABLE_ABS_3D              0x100000
#define AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE      0x200000

const MS_U16 MpegBitRateTable[MPEG_BITRATE_TABLE_ROW_NUM][MPEG_BITRATE_TABLE_COL_NUM] =
{
    // V1, L1
    {0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0},
    // V1, L2
    {0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0},
    // V1, L3
    {0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0},
    // V2, L1
    {0, 32, 48, 56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, 0},
    // V2, L2
    {0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0},
    // V2, L3
    {0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0},
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MMA_AudioType _GetAudioCaps(AUDIO_DEC_ID eDecId)
{
    MMA_AudioType eAudioCaps = 0;

    eAudioCaps =     Audio_DEC_PCM        | Audio_DEC_MPEG     |
                     Audio_DEC_MP3        | Audio_DEC_AC3      |
                     Audio_DEC_AC3P       | Audio_DEC_AAC_LC   |
                     Audio_DEC_HEAAC_V1   | Audio_DEC_HEAAC_V2 |
                     Audio_DEC_DOLBYPULSE | Audio_DEC_RA8      |
                     Audio_DEC_WMA        | Audio_DEC_WMAPRO   |
                     Audio_DEC_XPCM       | Audio_DEC_AMR_NB   |
                     Audio_DEC_AMR_WB     | Audio_DEC_VORBIS   |
                     Audio_DEC_DTS        | Audio_DEC_FLAC     |
                     Audio_DEC_DRA;

    return eAudioCaps;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAudioCapability()
/// @brief \b Function \b Description: This routine is used to get DSP capability
/// @param <IN>        \b eDecId    : Dec ID
/// @param <IN>        \b p_AudioDecStatus :   Audio Dec Status structure
/// @param <RET>       \b value :     0-- eDecId not support
///                                                  1--  eDecId support
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_GetAudioCapability(AUDIO_DEC_ID eDecId, AudioDecStatus_t * p_AudioDecStatus)
{
    MS_BOOL ret = FALSE;

    if (eDecId == AU_DEC_ID2)
    {
        return ret;
    }

    switch ((p_AudioDecStatus->eSourceType))
    {
        case E_AUDIO_INFO_HDMI_IN: // HDMI should decode in SE and DEC DSP, always return true
        case E_AUDIO_INFO_ATV_IN:
            ret = TRUE;
            break;

        case E_AUDIO_INFO_DTV_IN:
            if ((p_AudioDecStatus->eGroup == E_CONNECT_MAIN) ||
                 (p_AudioDecStatus->eGroup == E_CONNECT_SUB))
            {
                ret = TRUE;
            }
            break;

        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
            ret = TRUE;
            break;

        default:
            if ((p_AudioDecStatus->eAudFormat) == (_GetAudioCaps(eDecId) & (p_AudioDecStatus->eAudFormat)))
            {
                ret = TRUE;
            }
            break;
    }

    return ret;
}
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD SYSETM Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Read_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value by IDMA
/// @param <IN>        \b dsp_addr    : DSP internal sram address
/// @param <IN>        \b dsp_memory_type :    0--DSP_MEM_TYPE_PM
///                                1--DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    U32 sram value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_Read_DSP_sram(MS_U16 u16Dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
// Do nothing because no DEC-DSP in Monoco
    u16Dsp_addr = u16Dsp_addr;
    dsp_memory_type = dsp_memory_type;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Write_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Write DSP internal sram value by IDMA
/// @param <IN>        \b dsp_addr    : DSP internal sram address
/// @param <IN>        \b value     : data want to write
/// @param <IN>        \b dsp_memory_type :    0-- write to DSP_MEM_TYPE_PM
///                                        1-- write to DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  : TRUE/FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_Write_DSP_sram(MS_U16 dsp_addr, MS_U32 value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
 // Do nothing because no DEC-DSP in Monoco
    dsp_addr = dsp_addr;
    value = value;
    dsp_memory_type = dsp_memory_type;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Init()
/// @brief \b Function \b Description:  This routine is the initialization for DVB module
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_Init(void)
{
    HAL_AUDIO_WriteMaskReg(REG_DEC_IDMA_CTRL0, 0x00FF, 0x0002);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteMaskReg(REG_DEC_IDMA_CTRL0, 0x00FF, 0x0003);

    g_FuncPrt_Hal_SetAudioParam2 = HAL_MAD_SetAudioParam2;
    g_FuncPrt_Hal_GetAudioInfo2 = HAL_MAD_GetAudioInfo2;
    g_FuncPtr_CheckCapability = HAL_MAD_GetAudioCapability;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMemInfo()
/// @brief \b Function \b Description:  This routine is used to set the DEC-DSP memory information
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetMemInfo(void)
{
  // Remove all memory Info to SetMemInfo2
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetEncCmd()
/// @brief \b Function \b Description: This routine is to set encode command.
/// @param <IN>        \b u8EncCmd    :
///                                    0--STOP
///                                    1--START ENCODE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetEncCmd(AU_DVB_ENCCMD u8EncCmd)
{
    // not use,  remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_EncodeInit()    @@ function can be extended
/// @brief \b Function \b Description: This routine is to init encode parameter.
/// @param <IN>        \b u8EncBr    :
///                               Encode Output Bitrate:   0:192kBps, 1:384kBps, 2:128kBps, 3:64kBps
///                           \b
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_EncodeInit(AU_DVB_ENCBR u8EncBr, MS_U8 u8EncFrameNum)
{
    // not use,  remove
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_GetEncodeFrameInfo()   @@kochien added for PVR
/// @brief \b Function \b Description:  This routine is used to get encoded frame information
/// @param <IN>        \b u32Addr    : encoded frame start address
/// @param <IN>        \b u32Size     : encoded frame data size
/// @param <IN>        \b u64EncIdx  :  frame idx for PTS
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *FrameInfo)
{
    // not use,  remove
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_setDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB1 decode command.
/// @param <IN>        \b u8DecCmd    :
///                                    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetDecCmd(AU_DVB_DECCMD u8DecCmd)
{
    HAL_MAD_SetDSP2DecCmd(u8DecCmd);
}

void HAL_MAD_SetDecCmd2(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd)
{
        DecId = DecId;
        HAL_MAD_SetDSP2DecCmd(u8DecCmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDSP2DecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB3 decode command.
/// @param <IN>        \b u8DecCmd    :
///                                    0--STOP
///                                    1--PLAY
///                                    1--RESYNC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD u8DecCmd)
{
    MS_U8   AVSYNC_STATE = 0;

    HALMAD_CHECK_SHM_INIT;

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "HAL_MAD_SetDSP2DecCmd:(%x)\r\n", u8DecCmd);

    if (u8DecCmd == AU_DVB_DECCMD_AVSYNC)
    {
        HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x3F, (MS_U8) (HAL_MAD_GetDSP2DecCmd() & 0x1F) );
    }
    else if (u8DecCmd == AU_DVB_DECCMD_FREERUN)
    {
        HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x3F, (MS_U8) (HAL_MAD_GetDSP2DecCmd() | 0x20) );
    }
    else if ( (u8DecCmd != AU_DVB_DECCMD_PLAY_AD) && (u8DecCmd != AU_DVB_DECCMD_STOP_AD) ) //Monet have no AD
    {
        AVSYNC_STATE = HAL_MAD_GetDSP2DecCmd() & 0x20;
      HAL_AUDIO_WriteMaskByte(REG_SE_DECODE_CMD, 0x3F, (MS_U8) (u8DecCmd | AVSYNC_STATE));
        if (u8DecCmd == AU_DVB_DECCMD_STOP)
        {
            // clean counter when stop,  tempority solution to patch interrupt lost
            g_AudioVars2->int_mb_cnt = 0;
            uniPlay_ackCnt = 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB1 decode command.
/// @param <OUT>        \b AU_DVB_DECCMD   :
//                                          AU_DVB_DECCMD_STOP,      //0
//                                          AU_DVB_DECCMD_PLAY,      //1
//                                          AU_DVB_DECCMD_PLAYFILETSP = 2,
//                                          AU_DVB_DECCMD_RESYNC,
//                                          ....etc
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD HAL_MAD_GetDecCmd(En_DVB_AudioDecoder AdoDecoderNo)
{
    return HAL_MAD_GetDSP2DecCmd();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDSP2DecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB3 decode command.
/// @param <OUT>        \b AU_DVB_DECCMD   :
//                                          AU_DVB_DECCMD_STOP,      //0
//                                          AU_DVB_DECCMD_PLAY,      //1
//                                          AU_DVB_DECCMD_PLAYFILETSP = 2,
//                                          AU_DVB_DECCMD_RESYNC,
//                                          ....etc
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD HAL_MAD_GetDSP2DecCmd(void)
{
    MS_U16   deccmd_status;

    deccmd_status = HAL_AUDIO_ReadReg(REG_SE_DECODE_CMD);
    return((AU_DVB_DECCMD)(deccmd_status&0x003F));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_setFreeRun()
/// @brief \b Function \b Description: This routine is to set  the decoder in free run or AV-sync mode.
/// @param <IN>        \b u8FreeRun    :    0--normal AV sync
///                                        1--free-run mode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetFreeRun( MS_U8  u8FreeRun )
{
    if(u8FreeRun == 0)
        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_AVSYNC);
    else
        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_FREERUN);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_DisEn_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040 );
    AUDIO_DELAY1MS(1);
    // Enable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0000 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Dis_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_Dis_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040 );        // disable
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x0080 );          // reset MAD module
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_RSTMAD_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB1 MIU request command.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_RSTMAD_DisEn_MIUREQ(void)
{
    // Disable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0040 );        // disable
    // Reset MAD
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x0080 );          // reset MAD module
    AUDIO_DELAY1MS(1);
    // Set MAD
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0080, 0x00 );
    // Enable MIU Request
    HAL_AUDIO_WriteMaskReg(REG_DEC_AUD_DTRL, 0x0040, 0x0000 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DSPACK()
/// @brief \b Function \b Description: This routine is to report DSP reload ACK cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_GetReloadCodeAck(void)
{
    //NOT_DEFINE : to do
    return(HAL_AUDIO_ReadByte(REG_MB_DE_ACK2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DSPACK()
/// @brief \b Function \b Description: This routine is to report DSP reload ACK cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_GetLoadCodeAck(void)
{
    //NOT_DEFINE : to do
    return(HAL_AUDIO_ReadByte(REG_MB_DE_ACK1));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Rpt_DTVES()
/// @brief \b Function \b Description: This routine is used to report AC3/MPEG stream if exist or not.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8  :    the decoder status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_Rpt_DTVES(void)
{
    MS_U8  u8DecStatus;

    u8DecStatus = HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS);
    u8DecStatus = HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS);        // add one time read to avoid mailbox issue

    return u8DecStatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_ReadTimeStamp()
/// @brief \b Function \b Description: This routine returns the stmestamp while decoding file format.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b timestamp    :    time stamp value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_ReadTimeStamp(void)
{

    MS_U32 w16_1 = HAL_AUDIO_ReadReg(REG_MB_TIME_STAMP_SEC);
    MS_U32 w16_2 = (HAL_AUDIO_ReadReg(REG_MB_TIME_STAMP_4ms) & 0x00FF);
    MS_U32 timestamp = (w16_1*1000) + (w16_2<<2);
    return timestamp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD__SetAdMixMode()
/// @brief \b Function \b Description: This routine is used to set MPEG mixed with AD or AD mixed with MPEG or not.
/// @param <IN>        \b u8Mix_mode    :    0--main mix to AD
///                                    1--AD mix to main
/// @param <IN>        \b bEnMix    :    0--disable AD mix
///                                    1--enable AD mix
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetAdMixMode (AD_MIX_MODE u8Mix_mode, MS_BOOL bEnMix)
{
    //NOT_DEFINE
    switch (u8Mix_mode)
    {
    case MAIN_MIX_AD:
    case AD_MIX_MAIN:
        HAL_AUDIO_WriteMaskReg(REG_MB_DEC3_CTRL, 0x8000, bEnMix<<15);  // IO_100 [15]
        break;

    default:
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMcuCmd()
/// @brief \b Function \b Description: This routine is to write MCU cmd for PIO.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetPIOCmd(MS_U8 cmd)
{
    cmd = cmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMcuCmd()
/// @brief \b Function \b Description: This routine is to write MCU cmd.
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetMcuCmd(MS_U8 cmd)
{
    cmd = cmd;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDspIDMA()
/// @brief \b Function \b Description:  This function is used to set DSP IDMA.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetDspIDMA(void)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_TriggerPIO8()
/// @brief \b Function \b Description:  This function is used to trigger PIO8 init.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_TriggerPIO8(void)
{
    //NOT_DEFINE : not used, remove
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_DvbFLockSynthesizer_En()
/// @brief \b Function \b Description:  This function is used to set DSP IDMA.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_DvbFLockSynthesizer_En(void)
{
    // enable force lock current DVB SYNC synthesizer freq
    // SIF only in SE DSP
    HAL_AUDIO_WriteMaskReg(REG_AUDIO_INPUT_CFG, 0x0040, 0x0040 );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SetCertMode()
/// @brief \b Function \b Description:
/// @param CertType    \b :   AUDIO_DOLBY_DD_Cert,
/// @param CertType    \b :   AUDIO_DOLBY_DDP_Cert,
/// @param CertType    \b :   AUDIO_DOLBY_PULSE_Cert,
/// @param CertType    \b :   AUDIO_DOLBY_MS10_Cert,
/// @param CertType    \b :   AUDIO_DTS_DMP_Cert,
/// @param CertType    \b :   AUDIO_DTS_BC_Cert,
/// @param CertType    \b :   AUDIO_DTS_PremiumTV_Cert,
/// @param CertType    \b :   AUDIO_SRS_XT_Cert,
/// @param enable      \b :   AUDIO_CERT_Enable / AUDIO_CERT_Disable
///
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable enCert )
{
    switch(CertType)
    {
        case AUDIO_DOLBY_DD_Cert:
        case AUDIO_DOLBY_DDP_Cert:
        case AUDIO_DOLBY_PULSE_Cert:
        case AUDIO_DOLBY_MS10_Cert:
        case AUDIO_DTS_DMP_Cert:
        case AUDIO_DTS_BC_Cert:
            // sound effect bypass setting
            break;

        case AUDIO_DOLBY_DDCO_Cert:
            break;

        case AUDIO_DTS_SYMMETRY_Cert:
            break;

        case AUDIO_DTS_ENVELO_Cert:
            break;

        case AUDIO_SRS_XT_Cert:
            break;

        default:
            break;
    }
    return(true);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MM2_initAesInfo()
/// @brief \b Function \b Description  : Initialize AES info
/// @param <IN>        \b dec_id  : Decoder ID
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{
        audio_aes_info[0].aes_base_addr =  OFFSET_ES1_DRAM_ADDR;
        audio_aes_info[0].aes_BufSize = ES1_DRAM_SIZE_BYTE;
        audio_aes_info[0].aes_end_addr = audio_aes_info[0].aes_base_addr + audio_aes_info[0].aes_BufSize;
        audio_aes_info[0].aes_write_addr = audio_aes_info[0].aes_base_addr;
        audio_aes_info[0].aes_freeSpace = audio_aes_info[0].aes_BufSize -  (BYTES_IN_MIU_LINE*2);
        audio_aes_info[0].aes_level = 0;
        audio_aes_info[0].aes_lastPTS = 0;
        audio_aes_info[0].aes_ptsCnt = 0;
        /* Set ES write pointer to DSP */
        HAL_AUDIO_WriteReg( M2S_MBOX_UNI_INPUT_ES_WPTR, (MS_U16)DSP2_ES1_DRAM_BASE);

        return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MM2_checkAesInfo()
/// @brief \b Function \b Description  : Get AES info from audio decoder
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b aes_info     : AES info structure pointer
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{
    if ( dec_id == AU_DEC_ID1 )
    {
        MS_U32 tmp1, tmp2;

        /* es level */
        audio_aes_info[0].aes_level = HAL_AUDIO_ReadReg(S2M_MBOX_ES_MEMCNT) << BYTES_IN_MIU_LINE_LOG2;

        /* calculate free space for next data input */
        tmp1 = audio_aes_info[0].aes_end_addr - audio_aes_info[0].aes_write_addr;
        tmp2 = audio_aes_info[0].aes_BufSize - audio_aes_info[0].aes_level;

        if ( tmp2 < (BYTES_IN_MIU_LINE*2) )                 //¦Ü¤Ö keep 2 ±ø line ªºªÅ¶¡
            tmp2 = 0;
        else
            tmp2 -= (BYTES_IN_MIU_LINE*2) ;

        audio_aes_info[0].aes_freeSpace = (tmp1 > tmp2)?tmp2:tmp1;

        memcpy( (void *)aes_info,  (void *)&audio_aes_info[0], sizeof(AES_INFO));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MM2_inputAesFinished()
/// @brief \b Function \b Description  : Inform DSP how much ES data is sent
/// @param <IN>        \b dec_id	   : Decoder ID
/// @param <IN>        \b es_size  : Size of ES data
/// @param <IN>        \b ptsExist : Whether ES data contains PTS or not
/// @param <IN>        \b pts	   : PTS
/// @param <OUT>       \b MS_BOOL  : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{
    MS_U32 es_writeLineOffset, pts_h, pts_l;
    MS_U32 tmp1, tmp2;

    if ( dec_id != AU_DEC_ID1 )
        return FALSE;

    if (es_size == 0)
        return FALSE;

    /* es level */
    audio_aes_info[0].aes_level = HAL_AUDIO_ReadReg(S2M_MBOX_ES_MEMCNT) << BYTES_IN_MIU_LINE_LOG2;  ///@@@

    /* calculate free space for next data input */
    tmp1 = audio_aes_info[0].aes_end_addr - audio_aes_info[0].aes_write_addr;
    tmp2 = audio_aes_info[0].aes_BufSize - audio_aes_info[0].aes_level;

    audio_aes_info[0].aes_freeSpace = (tmp1 > tmp2)?tmp2:tmp1;

    if (es_size > audio_aes_info[0].aes_freeSpace)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "==> Error, input AES size(0x%08u) is larger than current Free space(0x%08u)\r\n", (unsigned int)es_size, (unsigned int)audio_aes_info[0].aes_freeSpace );
        return FALSE;
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[HAL_MAD_MM2_inputAesFinished] wPtr:%08x => ", audio_aes_info[0].aes_write_addr);

    /* update write pointer */
    audio_aes_info[0].aes_write_addr += es_size;

    if ( audio_aes_info[0].aes_write_addr >= audio_aes_info[0].aes_end_addr )
        audio_aes_info[0].aes_write_addr -= audio_aes_info[0].aes_BufSize;

    es_writeLineOffset = audio_aes_info[0].aes_write_addr;
    es_writeLineOffset >>= BYTES_IN_MIU_LINE_LOG2;

    if (ptsExist)
        {
            if ((audio_aes_info[0].aes_lastPTS  == pts)&&(audio_aes_info[0].aes_ptsCnt != 0))
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " ======== Error PTS is same !! =============\n");
            else
                {
                    pts_h = (pts >> 24) & 0x1FF;        //PTS[32:24]
                    pts_l = pts & 0xFFFFFF;                 //PTS[23:0]

                    /* prepare PTS to DSP */
                    HAL_MAD2_Write_DSP_sram( DSP2DmAddr_sys_Working + SYS_FILE_PTS1_H, pts_h,DSP_MEM_TYPE_DM);
                    HAL_MAD2_Write_DSP_sram( DSP2DmAddr_sys_Working + SYS_FILE_PTS1_L, pts_l,DSP_MEM_TYPE_DM);
                    audio_aes_info[0].aes_ptsCnt++;
                    audio_aes_info[0].aes_lastPTS = pts;
                    HAL_AUDIO_SendIntrupt( DSP_SE, MBOX_DBGCMD_FILE_PTS_INT>>8);

                        AUDIO_DELAY1US(500);
                }
        }

    /* Set ES write pointer to DSP */
    HAL_AUDIO_WriteReg( M2S_MBOX_UNI_INPUT_ES_WPTR, es_writeLineOffset);
    AUDIO_DELAY1US(500);                                                                                        //Since DSP timer update ES_MEM_CNT at each 125us, need this delay to keep

    return TRUE;
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD MPEG Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetHeaderInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg header
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b U32    : mpeg header information
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_MPEG_GetHeaderInfo(void)
{
    MS_U16 data;

    data = HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE, DSP_MEM_TYPE_DM);

    return (data & 0x0FFF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetSampleRate()
/// @brief \b Function \b Description: This routine will return the 16bit mpeg samplerate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b U16    : mpeg sampling rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_MPEG_GetSampleRate(void)
{
    MS_U8 data1;
    MS_U16 smprate_status;

    data1 = (HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE, DSP_MEM_TYPE_DM) & 0xF00)>>8;

    switch(data1)
    {
        case 0: smprate_status = 44100; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "44.1K \n");
                break;
        case 1: smprate_status = 48000; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "48K \n");
                break;
        case 2: smprate_status = 32000; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "32K \n");
                break;
        case 3: smprate_status = 22050; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "22.05K \n");
                break;
        case 4: smprate_status = 24000; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "24K \n");
                break;
        case 5: smprate_status = 16000; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "16K \n");
                break;
        case 6: smprate_status = 11025; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "11.025K \n");
                break;
        case 7: smprate_status = 12000; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "12K \n");
                break;
        case 8: smprate_status = 8000; //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "8K \n");
                break;
        default: AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Invalid sample rate\n"); return 0; break;
    }
    return (smprate_status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetBitRate()
/// @brief \b Function \b Description: This routine will return the 16-bit mpeg bit rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b U16    : mpeg bit rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_MPEG_GetBitRate(void)
{
    MS_U8 data1;

    MS_U8 bitrate_index, layer_index, version_index, ver_layer_index = 0;
    // Read BitRate
    bitrate_index = (HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE, DSP_MEM_TYPE_DM) & 0xF0)>>4;

    // Read Layer
    data1 = (HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE, DSP_MEM_TYPE_DM) & 0x0C)>>2;

    if((data1 < LAYER_MIN) || (data1 > LAYER_MAX))
        return 0;

    layer_index = data1 - 1;                // 0->layer1, 1->layer2, 2->layer3

    // Read MPEG version
    version_index = HAL_MAD2_Read_DSP_sram(REG_MPEG_VERSION, DSP_MEM_TYPE_DM);

    ver_layer_index = version_index*3+layer_index;

    if((bitrate_index >= MPEG_BITRATE_TABLE_COL_NUM) || (ver_layer_index >= MPEG_BITRATE_TABLE_ROW_NUM))
        return 0;

    return(MpegBitRateTable[ver_layer_index][bitrate_index]);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_MPEG_GetSoundMode()
/// @brief \b Function \b Description: This routine will return the 8bit mpeg sound mode
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MPEG_SOUND_MODE :
///                                         MPEG_SOUND_MODE_STEREO =0,
///                                         MPEG_SOUND_MODE_JOINT_STEREO,
///                                         MPEG_SOUND_MODE_DUAL_CHAN,
///                                         MPEG_SOUND_MODE_MONO
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MPEG_SOUND_MODE HAL_MAD_MPEG_GetSoundMode(void)
{
    MS_U8  data1;

    if( (HAL_AUDIO_GetDspCodeType()) != AU_DVB_STANDARD_MPEG ) {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "DSP need to relaod mpeg\r\n");
        return (MPEG_SOUND_MODE)0;
    }

    // get MPEG Sound Mode
    data1 = HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE, DSP_MEM_TYPE_DM) & 0x03;
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "    Sound Mode: %x\n", data1);

    switch(data1)
    {
        case MPEG_SOUND_MODE_JOINT_STEREO:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Joint Stereo \n");
            break;

        case MPEG_SOUND_MODE_DUAL_CHAN:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Dual Channel \n");
            break;

        case MPEG_SOUND_MODE_MONO:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Mono \n");
            break;

        case MPEG_SOUND_MODE_STEREO:
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Stereo \n");
            break;
    }

    return((MPEG_SOUND_MODE)data1);
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD Dolby Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_Monitor_DDPlus_SPDIF_Rate()
/// @brief \b Function \b Description: This routine is used to set reference ADC clock by DD+ dsp code
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_Monitor_DDPlus_SPDIF_Rate(void)
{
    MS_U8 u8SpdifMode = SPDIF_OUT_PCM;
    MS_U8 eAudioSource = 0;
    MS_U8 u8MainDspCodeType = AU_DVB_STANDARD_INVALID;
    MS_U8 u8Dsp2CodeType = AU_DVB_STANDARD_INVALID;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    MS_BOOL retTx1, retTx2;

    HALMAD_CHECK_SHM_INIT;

    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));

    eAudioSource = (MS_U8)g_AudioVars2->eAudioSource;
    u8Dsp2CodeType = g_AudioVars2->g_u8Dsp2CodeType;

    HAL_AUDIO_SPDIF_SetMode(gSpdifOutputType, eAudioSource);

    u8SpdifMode = (_HAL_AUDIO_SPDIF_GetMode() & SPDIF_OUT_NONPCM) ? SPDIF_OUT_NONPCM : SPDIF_OUT_PCM;

    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Failed to get channel status !!!\n", __FUNCTION__);
    }

    switch(eAudioSource)
    {
        case E_AUDIO_INFO_DTV_IN:
        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
        {
            u8MainDspCodeType = u8Dsp2CodeType;

            switch(u8MainDspCodeType)
            {
                case AU_DVB_STANDARD_AC3:
                case AU_DVB_STANDARD_AC3P:
                case AU_DVB_STANDARD_MS10_DDC:
                {
                    if (u8SpdifMode == SPDIF_OUT_PCM)
                    {
                        break;
                    }

                    switch(HAL_MAD2_Read_DSP_sram(DSP2DmAddr_SAMPLERATE, DSP_MEM_TYPE_DM) & 0x3)
                    {
                        case 1:
                        {
                            /* change SPDIF Tx 1&2 channel status --> 44.1KHz */
                            stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                            stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                            break;
                        }

                        case 2:
                        {
                            /* change SPDIF Tx 1&2 channel status --> 32KHz */
                            stDigitalChannelStatusTx1.u8SamplingRate = 0xC0;
                            stDigitalChannelStatusTx2.u8SamplingRate = 0xC0;
                            break;
                        }

                        default:
                        {
                            /* change SPDIF Tx 1&2 channel status --> 48KHz */
                            stDigitalChannelStatusTx1.u8SamplingRate = 0x40;
                            stDigitalChannelStatusTx2.u8SamplingRate = 0x40;
                            break;
                        }
                    }
                    break;
                }

                case AU_DVB_STANDARD_DTS:
                {
                    if (u8SpdifMode == SPDIF_OUT_PCM)
                    {
                        break;
                    }

                    if ((HAL_MAD2_Read_DSP_sram(DSP2DmAddr_SAMPLERATE, DSP_MEM_TYPE_DM) & 0x3) == 0x01 )
                    {
                        /* 44.1KHz */
                        stDigitalChannelStatusTx1.u8SamplingRate = 0x00;     /* change SPDIF Tx1 channel status --> 44.1KHz */
                        stDigitalChannelStatusTx2.u8SamplingRate = 0x00;     /* change SPDIF Tx2 channel status --> 44.1KHz */
                     }
                    else
                    {
                        /* 48KHz */
                        stDigitalChannelStatusTx1.u8SamplingRate = 0x40;     /* change SPDIF Tx1 channel status --> 48KHz */
                        stDigitalChannelStatusTx2.u8SamplingRate = 0x40;     /* change SPDIF Tx2 channel status --> 48KHz */
                    }
                    break;
                }

                default:
                {
                    break;
                }
            }
            break;
        }

        default:
        {
            break;
        }
    }
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
}

///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD WMA Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_WMA_SetASFParm()
/// @brief \b Function \b Description: This routine will set WMA ASF Paramaters.
/// @param <IN>        \b NONE    : WMA_ASF_PARMTYPE, value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_WMA_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
{
    switch ( parm_type )
    {
        case WMA_PARAMTYPE_VERSION:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+0, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_CHANNELS:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+1, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_SAMPLERATE:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+2, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_BYTERATE:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+3, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_BLOCKALIGN:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+4, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_ENCOPT:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+5, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_PARSINGBYAPP:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+6, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_BITS_PER_SAMPLE:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+7, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_CHANNELMASK:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+8, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_DRC_PARAM_EXIST:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+9, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_DRC_RMS_AMP_REF:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+10, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_DRC_RMS_AMP_TARGET:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+11, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_DRC_PEAK_AMP_REF:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+12, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_DRC_PEAK_AMP_TARGET:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+13, value, DSP_MEM_TYPE_DM);
            break;
        case WMA_PARAMTYPE_MAX_PACKET_SIZE:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+14, value, DSP_MEM_TYPE_DM);
            break;
        default:
            break;
    }
}


///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///
///        AUDIO_MAD OTHER Relational Hal Function
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
///-----------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_RA8_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing RA8 LBR data
/// @param <IN>        \b mNumCodecs    :    1--stereo or mono
///                                        2~5-- multi channels
/// @param <IN>        \b mSamples    :         output samples--256, 512, 1024
/// @param <IN>        \b mSampleRate    :    8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param <IN>        \b Channels    :    1-- mono
///                                    2-- stereo
/// @param <IN>        \b cplStart    :    0 ~ 51
/// @param <IN>        \b Regions    :    0 ~ 31
/// @param <IN>        \b cplQbits    :    0, 2, 3, 4, 5, 6
/// @param <IN>        \b FrameSize    :    bitstream size of every NumCodes (bytes)
///                                    Range--0 ~ 65535
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8 :    TRUE--parameters are correct
///                            FALSE--parameters are incorrect
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_RA8_setParam(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate,
                           MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits,
                           MS_U16* FrameSize)
{
    MS_U8  codecsIdx = 0, paramNum1 = 0;
    MS_U16 cTmp = 0, cWriteBuf = 0;
    MS_U16 au_ra8_para_address[11]= { 0x1000, 0x1001, 0x1002, 0x1003, 0x1004, 0x1005,
                                                        0x1006, 0x1007, 0x1008, 0x1009, 0x100A                };

    if ( Channels == NULL || Regions == NULL || cplStart == NULL ||
         cplQbits == NULL || FrameSize == NULL )
    {
        return FALSE;
    }

    for ( codecsIdx = 0; codecsIdx < 5; codecsIdx++ )
    {
        if ( codecsIdx <= mNumCodecs - 1 )
        {
            if ( (Channels[codecsIdx] < 1) || (Channels[codecsIdx] > 2) )
                return FALSE;
            if (Regions[codecsIdx] > 51)
                return FALSE;
            if (cplStart[codecsIdx] > 31)
                return FALSE;
            if ( (cplQbits[codecsIdx] != 0) &&
                 ((cplQbits[codecsIdx] < 2) || (cplQbits[codecsIdx] > 6)) )
                return FALSE;
        }
        else
        {
            break;
        }
    }

    switch ( mNumCodecs )
    {
        case 5:
            cTmp = 4;
            break;
        case 4:
            cTmp = 3;
            break;
        case 3:
            cTmp = 2;
            break;
        case 2:
            cTmp = 1;
            break;
        case 1:
            cTmp = 0;
            break;
        default:
            return FALSE;
    }
    cWriteBuf = cTmp << 13;

    switch ( mSamples )
    {
        case 1024:
            cTmp = 2;
            break;
        case 512:
            cTmp = 1;
            break;
        case 256:
            cTmp = 0;
            break;
        default:
            return FALSE;
    }
    cWriteBuf |= cTmp << 11;

    switch ( mSampleRate )
    {
        case 48000:
            cTmp = 0;
            break;
        case 44100:
            cTmp = 1;
            break;
        case 22050:
            cTmp = 2;
            break;
        case 16000:
            cTmp = 3;
            break;
        case 11025:
            cTmp = 4;
            break;
        case 8000:
            cTmp = 5;
            break;
        default:
            return FALSE;
    }
    cWriteBuf |= cTmp << 8;
    HAL_MAD2_Write_DSP_sram(au_ra8_para_address[paramNum1],cWriteBuf, DSP_MEM_TYPE_PM);

    for ( codecsIdx = 0; codecsIdx < 5; codecsIdx++ )
    {
        paramNum1++;

        if ( codecsIdx <= mNumCodecs - 1 )
        {
            cWriteBuf  = Channels[codecsIdx] << 14;
            cWriteBuf |= Regions[codecsIdx]  << 8;
            cWriteBuf |= cplStart[codecsIdx] << 3;
            cWriteBuf |= cplQbits[codecsIdx];
            HAL_MAD2_Write_DSP_sram(au_ra8_para_address[paramNum1],cWriteBuf, DSP_MEM_TYPE_PM);
            paramNum1++;
            cWriteBuf = FrameSize[codecsIdx];
            HAL_MAD2_Write_DSP_sram(au_ra8_para_address[paramNum1],cWriteBuf, DSP_MEM_TYPE_PM);
        }
        else
        {
            cWriteBuf = 0;
            HAL_MAD2_Write_DSP_sram(au_ra8_para_address[paramNum1],cWriteBuf, DSP_MEM_TYPE_PM);
            paramNum1++;
            HAL_MAD2_Write_DSP_sram(au_ra8_para_address[paramNum1],cWriteBuf, DSP_MEM_TYPE_PM);
        }
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_XPCM_GetParaBase()
/// @brief \b Function \b Description: This routine is used to get the base address for XPCM parameter
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  : XPCM parameter base address in DSP
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_XPCM_GetParaBase(void)
{
    return (MS_U16)REG_DEC2_PARAM_BASE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_RA8_GetParaBase()
/// @brief \b Function \b Description: This routine is used to get the base address for Ra8 parameter
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  : Ra8 parameter base address in DSP
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_MAD_Ra8_GetParaBase(void)
{
    return (MS_U16)0x0FBD;
}

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetCommInfo()
/// @brief \b Function \b Description: This routine set different paramter to audio decoder
/// @param <IN>        \b Audio_COMM_infoType    : common audio parameter type
/// @param <IN>        \b MS_U32    : common audio parameter 1
/// @param <IN>        \b MS_U32    : common audio parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetCommInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    HALMAD_CHECK_SHM_INIT;

    MS_BOOL retTx1, retTx2;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));

    switch(infoType)
    {
        /********************************************************************************
        *       Other CMD
        ********************************************************************************/
        case Audio_Comm_infoType_Set_UNI_NEED_DECODE_FRMCNT:
            HAL_AUDIO_WriteMaskReg(REG_DEC2_UNI_NEED_DEC_FRMNUM, 0xFFFF, param1);
            break;

        case Audio_Comm_infoType_Set_UNI_ES_Wptr:
            HAL_AUDIO_WriteReg(REG_DEC2_UNI_ES_WPTR, (MS_U16)(param1>>4));    // transfer to line unit
            break;

        case Audio_Comm_infoType_setBypassSPDIF_PAPB_chk:       //HAL_MAD_SetDtsBypassFlag
            break;

        case Audio_Comm_infoType_MMA_init:
            HAL_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_GAME);
            HAL_SOUND_SetMixModeVolume(E_AUDIO_INFO_GAME_IN, GAME1_VOL, 0x12, 0x00);     //DEC1 Vol
            HAL_SOUND_SetMixModeVolume(E_AUDIO_INFO_GAME_IN, GAME2_VOL, 0x12, 0x00);     //DEC2 Vol
            HAL_SOUND_SetMixModeVolume(E_AUDIO_INFO_GAME_IN, PCM_VOL,   0x12, 0x00);     //DEC3 Vol
            break;

        case Audio_Comm_infoType_MMA_finish:
            HAL_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_DTV);
            break;

        case Audio_Comm_infoType_DMAReader_Command:
            if (param1)
            {
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0001, 0x0001);
                AUDIO_DELAY1MS(2);
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0100);
            }
            else
            {
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0001, 0x0000);
                AUDIO_DELAY1MS(2);
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0100, 0x0000);
                AUDIO_DELAY1MS(2);
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0040, 0x0040); //read_init, clear dma reader data & pointer
                AUDIO_DELAY1MS(1);
                HAL_AUDIO_WriteMaskReg(0x2B80, 0x0040, 0x0000);
            }
            break;

        case Audio_Comm_infoType_CompressBin_LoadCode:
            g_AudioVars2->CompressInfo = param1;
            break;

        case Audio_Comm_infoType_CompressBin_DDRAddress:
            g_AudioVars2->g_DSPDDRBinBaseAddress[param1] = param2;
            g_DSPDDRBinBaseAddress[param1] = param2;
            break;

        case Audio_Comm_infoType_SetSCMS:
        {
            retTx1= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            retTx2= HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);

            if (retTx1 == FALSE || retTx2 == FALSE)
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Failed to get channel status !!!\n", __FUNCTION__);
                break;
            }

            if(param1)
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 1;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 1;
            }
            else
            {
                stDigitalChannelStatusTx1.stCopyRight.CBit = 0;
                stDigitalChannelStatusTx2.stCopyRight.CBit = 0;
            }

            if(param2)
            {
                stDigitalChannelStatusTx1.stCopyRight.LBit = 1;
                stDigitalChannelStatusTx2.stCopyRight.LBit = 1;
            }
            else
            {
                stDigitalChannelStatusTx1.stCopyRight.LBit = 0;
                stDigitalChannelStatusTx2.stCopyRight.LBit = 0;
            }

            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
            HAL_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
       }
            break;

        case Audio_Comm_infoType_ADC_InputGain:
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0xE0, (AUD_ADC_GAIN_TYPE)param1<<5);
            break;

        case Audio_Comm_infoType_ADC1_InputGain:
            HAL_AUDIO_WriteMaskByte(0x2CE5 , 0x1C, (AUD_ADC_GAIN_TYPE)param1<<2);
            break;

        case Audio_Comm_infoType_KTV_SetType:
            switch((AUDIO_KTV_MODEL)param1)
            {
                case AUD_KTV_MODEL1:     // iSing99
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x00);  // No this model in T12
                    break;
                case AUD_KTV_MODEL2:     // Multak
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x21);
                    break;
                case AUD_KTV_MODEL3:     // ¹p¥Û
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x02);
                    break;
                case AUD_KTV_MODEL4:     // Konka
                    HAL_AUDIO_WriteMaskByte(0x2D47 , 0x7F, 0x14);
                    break;
                default:
                    break;
            }
            break;

        case Audio_Comm_infoType_syncSTC_in_mmTs:
            if ( param1 )
                HAL_AUDIO_DecWriteMaskByte(0x2D8E, 0x10, 0x10);
            else
                HAL_AUDIO_DecWriteMaskByte(0x2D8E, 0x10, 0x00);
            break;

        case Audio_Comm_infoType_PUSI_disable_Info:
            break;

        case Audio_Comm_infoType_Parser_Spec:
            break;

        case Audio_Comm_infoType_setNR_Threshold:
            HAL_AUDIO_WriteByte(0x2D32, (MS_U8) param1);
            break;

        case Audio_Comm_infoType_Set_CallBackFunc:
            if ( g_FuncPrt_Hal_SetAudioParam2 == NULL )
            {
                g_FuncPrt_Hal_SetAudioParam2 = HAL_MAD_SetAudioParam2;
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: g_FuncPrt_Hal_SetAudioParam2 hook OK !!\r\n", __FUNCTION__);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: g_FuncPrt_Hal_SetAudioParam2 already register !!\r\n", __FUNCTION__);
            }
            if ( g_FuncPrt_Hal_GetAudioInfo2 == NULL )
            {
                g_FuncPrt_Hal_GetAudioInfo2 = HAL_MAD_GetAudioInfo2;
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: g_FuncPrt_Hal_GetAudioInfo2 hook OK !!\r\n", __FUNCTION__);
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: g_FuncPrt_Hal_GetAudioInfo2 already register !!\r\n", __FUNCTION__);
            }
            break;

        /********************************************************************************
        *       DECODER 1
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC1_Set_play_smpRate:
            {
                MS_U16 play_cmd;
                MS_U32 synth_H;

                play_cmd = HAL_AUDIO_ReadReg(REG_SE_DECODE_CMD) & 0x1F;
                if (play_cmd != 0x02)
                    printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: play command is not 2 !!\n", __FUNCTION__);

                if (param1)
                {
                    if ((param2 > 50000) ||(param2<6000))
                    {
#if defined (__aarch64__)
                        printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: play rate [%ld] is invalid !!\n", __FUNCTION__, (unsigned int)param2);
#else
                        printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: play rate [%d] is invalid !!\n", __FUNCTION__, (unsigned int)param2);
#endif
                    }

                    synth_H = 216000000 / param2;
                    HAL_MAD2_Write_DSP_sram(DSP2PmAddr_ext_Synth_L, 0, DSP_MEM_TYPE_PM);
                    HAL_MAD2_Write_DSP_sram(DSP2PmAddr_ext_Synth_H, synth_H, DSP_MEM_TYPE_PM);
                    HAL_AUDIO_WriteMaskByte(M2S_MBOX_DEC_CTRL, 0x01, 0x01);
                }
                else
                {
                    HAL_AUDIO_WriteMaskByte(M2S_MBOX_DEC_CTRL, 0x01, 0x00);
                }
            }
        break;

        case Audio_Comm_infoType_ADEC1_soundmod:
            {
                MS_U8 value = 0;

                switch((AUD_Dolby_SOUNDMODE)param1)
                {
                    case AUD_Dolby_SOUNDMODE_LR:     value = 0x0;  break;
                    case AUD_Dolby_SOUNDMODE_LL:     value = 0x4;  break;
                    case AUD_Dolby_SOUNDMODE_RR:     value = 0x8;  break;
                    default:                         value = 0x0;  break;
                }

                HAL_AUDIO_WriteMaskReg(REG_MB_DEC_CTRL, 0x000C, value);
            }
            break;

        case Audio_Comm_infoType_SoundMode:                     //HAL_MAD_SetSoundMode
            HAL_AUDIO_SeWriteMaskByte(REG_MB_MODE_SELECT, 0x03, (AU_DVB_MODE) param1);
            break;

        case Audio_Comm_infoType_ADEC1_reqDataSize:
        case Audio_Comm_infoType_MMFileSize:                    //HAL_MAD_SetMadFileSize
            if ((HAL_AUDIO_ReadByte(REG_DEC_DECODE_CMD)&0x1F) == 0)
            {
                MS_U16   file_size_value = (0x0800  / BYTES_IN_MIU_LINE);     //default = 2KB

                switch (param1)
                {
                    case FILE_SIZE_64:      file_size_value = (0x0040  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_128:     file_size_value = (0x0080  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_256:     file_size_value = (0x0100  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_512:     file_size_value = (0x0200  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_1KB:     file_size_value = (0x0400  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_2KB:     file_size_value = (0x0800  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_4KB:     file_size_value = (0x1000  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_8KB:     file_size_value = (0x2000  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_16KB:    file_size_value = (0x4000  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_32KB:    file_size_value = (0x8000  / BYTES_IN_MIU_LINE);       break;
                    case FILE_SIZE_64KB:    file_size_value = (0x10000 / BYTES_IN_MIU_LINE);       break;
                    default:                                                 break;
                }
                HAL_MAD2_Write_DSP_sram(REG_DEC2_MHEG5_SIZE, file_size_value, DSP_MEM_TYPE_PM);
            }
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_informDataRdy:
            //Messi only have one decoder "DSP_SE"
            if (HAL_AUDIO_GetPlayFileFlag(DSP_SE) == TRUE)
            {
                HAL_AUDIO_SetPlayFileFlag(DSP_SE, 0);
                HAL_AUDIO_WriteByte(REG_MB_DEC2_MM_INT_TAG, param1);
                HAL_AUDIO_SendIntrupt(DSP_SE, 0xE0);
            }
            break;

        case Audio_Comm_infoType_DEC1_MMTag:                    //HAL_MAD_SetMMIntTag
            HAL_AUDIO_WriteByte(REG_MB_DEC2_MM_INT_TAG, param1);
            break;

        case Audio_Comm_infoType_ADEC1_FF2X:
        case Audio_Comm_infoType_MM_FFx2:                       //HAL_MAD_SetMMFFx2Mode
            if ( param1 )
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x20, 0x20);
            else
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x20, 0x00);
            break;

        case Audio_Comm_infoType_ADEC1_mute:
            if ( param1 )
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x40, 0x40);
            else
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x40, 0x00);
            break;

        case Audio_Comm_infoType_ADEC1_play_wo_output:
            if (param1)
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x80);
            else
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x00);
            break;

        case Audio_Comm_infoType_ADEC1_playControl:
            switch((MMA_AUDIO_CONTROL)param1)
            {
                case MMA_STOP:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_AVSYNC);
                    //Messi only have one decoder "DSP_SE"
                    HAL_AUDIO_SetPlayFileFlag(DSP_SE, 0);
                    break;

                case MMA_PLAY:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY);
                    break;

                case MMA_PLAY_FILETSP:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILETSP);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                    break;

                case MMA_RESYNC:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_RESYNC);
                    break;

                case MMA_PLAY_FILE:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                    break;

                case MMA_BROWSE:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STARTBROWSE);
                    break;

                case MMA_PAUSE:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PAUSE);
                    break;

                case MMA_PLAY_FRAME:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
                    break;

                default:
                    break;
            }
            HAL_SOUND_SetMixModeMute( E_AUDIO_INFO_GAME_IN, GAME1_VOL, false );
            break;

        case Audio_Comm_infoType_ADEC1_setDecParam:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE+param1, param2, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_PCMOutCnt:
            uniPlay_ackCnt++;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n========Trigger decInt =0x%X===========\r\n", (unsigned int)uniPlay_ackCnt);
            HAL_AUDIO_WriteReg(REG_DEC2_UNI_PCM_OUTCNT, (MS_U16)(param1>>BYTES_IN_MIU_LINE_LOG2));  // transfer to line unit
            break;

        case Audio_Comm_infoType_ADEC1_UNI_PCMInput_Addr:   //assume UNI get data from PCM1.
            HAL_AUDIO_WriteReg(REG_DEC2_UNI_PCM3_WPTR, (MS_U16)((param1 - 0x300000)>>BYTES_IN_MIU_LINE_LOG2));    // transfer to line unit
            break;

        case Audio_Comm_infoType_DEC1_setBufferProcess:
            // Need Decoder Add delay support in DEC1-DSP
            break;

        case Audio_Comm_infoType_ADEC1_avsync_delay:
            {
                MS_U32 delay = 0;
                MS_U32 delay_ms = 0;
                delay_ms = param1 * MS_CHANGE_TO_TIME_STAMP;

                delay = g_ac3_delay + delay_ms;
                if(delay>DELAY_UPPER_LIMIT)
                {
                    delay = DELAY_UPPER_LIMIT;
                }
                HAL_MAD2_Write_DSP_sram(REG_AC3_AVdelay, delay, DSP_MEM_TYPE_PM);

                delay = g_aac_delay + delay_ms;
                if(delay>DELAY_UPPER_LIMIT)
                {
                    delay = DELAY_UPPER_LIMIT;
                }
                HAL_MAD2_Write_DSP_sram(REG_AAC_AVdelay, delay, DSP_MEM_TYPE_PM);

                delay = g_mpeg_delay + delay_ms;
                if(delay>DELAY_UPPER_LIMIT)
                {
                    delay = DELAY_UPPER_LIMIT;
                }
                HAL_MAD2_Write_DSP_sram(REG_MPG_AVdelay, delay, DSP_MEM_TYPE_PM);

            }
            break;
        /********************************************************************************
        *       DECODER 2
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC2_soundmod:
        case Audio_Comm_infoType_ADEC2_reqDataSize:
        case Audio_Comm_infoType_ADEC2_FF2X:
        case Audio_Comm_infoType_ADEC2_mute:
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_informDataRdy:
            if (HAL_AUDIO_GetPlayFileFlag(DSP_SE) == TRUE)
            {
                HAL_AUDIO_SetPlayFileFlag(DSP_SE, 0);
                HAL_AUDIO_WriteByte(REG_MB_DEC2_MM_INT_TAG, param1);
                HAL_AUDIO_SendIntrupt(DSP_SE, 0xE0);
            }
            break;

        case Audio_Comm_infoType_DEC2_MMTag:                    //HAL_MAD2_SetMMIntTag
            HAL_AUDIO_WriteByte(REG_MB_DEC2_MM_INT_TAG, param1);
            break;

        case Audio_Comm_infoType_ADEC2_playControl:
            {
                switch((MMA_AUDIO_CONTROL)param1)
                {
                    case MMA_STOP:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_STOP);
                        HAL_AUDIO_SetPlayFileFlag(DSP_SE, 0);
                        break;

                    case MMA_PLAY:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_PLAY);
                        break;

                    case MMA_PLAY_FILETSP:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_PLAYFILETSP);
                        break;

                    case MMA_RESYNC:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_RESYNC);
                        break;

                    case MMA_PLAY_FILE:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
                        break;

                    case MMA_BROWSE:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_STARTBROWSE);
                        break;

                    case MMA_PAUSE:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_PAUSE);
                        break;

                    case MMA_PLAY_FRAME:
                        HAL_MAD2_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME);
                        break;

                    default:
                        break;
                }

                HAL_SOUND_SetMixModeMute( E_AUDIO_INFO_GAME_IN, GAME2_VOL, false );
            }
            break;

        case Audio_Comm_infoType_ADEC2_setDecParam:
            //NOT_DEFINE : remove
            break;

        /********************************************************************************
        *       OTHERS
        ********************************************************************************/
        case Audio_Comm_infoType_setSpdifDelay: // set spdif dealy time
		case Audio_Comm_infoType_setSpdif_BufferProcess:
            HAL_SOUND_SetSpdifAudioDelay((MS_U16)param1);
            break;

        case Audio_Comm_infoType_ReadByte: // For APP to read  register value directly.
            g_ReadTargetAddr = param1;
            break;

        case Audio_Comm_infoType_WriteByte: // For APP to write register value directly.
            HAL_AUDIO_AbsWriteMaskByte(param1,0xFF,(MS_U8)(param2&0xFF));
            break;

        case Audio_Comm_infoType_PTS_info:
            HAL_AUDIO_PTS_info(param1);
            break;

        case Audio_Comm_infoType_RTSP_Mem:
            {
                AUDIO_RTSP_INFO *ptmp = (AUDIO_RTSP_INFO *)param1;
                ptmp->Mem_addr = RTSP_MEM_ADDR;
                ptmp->Mem_size= RTSP_MEM_SIZE;
            }
            break;

        case Audio_Comm_infoType_Dump_Dsp_Info:
            {
                 g_AudioDumpInfoCtrl = param1;

                if (param2 > 50 )
                    g_audioDumpInfo_interval = param2;
            }
            break;

        case Audio_Comm_infoType_Set_Video_STCPTS_DIFF:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP1PmAddr_video_TD, param1, DSP_MEM_TYPE_PM);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Protect:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_protect, param1, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_Vorbis_header_size:
            //Cathy need to do
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Threshold:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_threshold, param1, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_Set_Parser:
           // Dummy , no need to set parser
            break;

        case Audio_Comm_infoType_Set_Debug_LOG_Level:
            g_AudioVars2->g_debug_level = param1;
            break;

#ifndef MSOS_TYPE_NUTTX
        case Audio_Comm_infoType_Set_TEE_Capibility:
            HALAUDIO_CHECK_TEE_INFO_SHM_INIT;
            switch ((Audio_TEE_Capability)param1)
            {
                case Audio_TEE_Setup:
                    g_bAudioTeeEnabled = TRUE;       audio_tee_enabled = TRUE;
                    if (audio_tee_mbx_initialized == FALSE)
                    {
                        if (HAL_AUDIO_RegisterMBX() == TRUE)
                        {
                            g_bAudioTeeMbxInitialized = TRUE;   audio_tee_mbx_initialized = TRUE;
                        }
                    }
                    break;

                case Audio_TEE_Setup_and_ALL_DSP_Secure_Accessed:
                    g_bAudioTeeEnabled = TRUE;        audio_tee_enabled = TRUE;
                    if (audio_tee_mbx_initialized == FALSE)
                    {
                        if (HAL_AUDIO_RegisterMBX() == TRUE)
                        {
                            g_bAudioTeeMbxInitialized = TRUE;             audio_tee_mbx_initialized = TRUE;
                            g_bDecDspSecureTeeAccessed = TRUE;        dec_dsp_secure_tee_accessed = TRUE;
                            g_bSndDspSecureTeeAccessed = TRUE;        snd_dsp_secure_tee_accessed = TRUE;
                        }
                    }
                    break;

                case Audio_TEE_Set_DSP_Secure_Accessed:
                    if ((audio_tee_enabled ==  TRUE) && (audio_tee_mbx_initialized == TRUE))
                    {
                        if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_DEC)
                        {
                            g_bDecDspSecureTeeAccessed = TRUE;            dec_dsp_secure_tee_accessed = TRUE;
                        }
                        else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_SND)
                        {
                            g_bSndDspSecureTeeAccessed = TRUE;           snd_dsp_secure_tee_accessed = TRUE;
                        }
                        else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_ALL)
                        {
                            g_bDecDspSecureTeeAccessed = TRUE;            dec_dsp_secure_tee_accessed = TRUE;
                            g_bSndDspSecureTeeAccessed = TRUE;            snd_dsp_secure_tee_accessed = TRUE;
                        }
                    }
                    break;

                case Audio_TEE_Set_DSP_Normal_Accessed:
                    if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_DEC)
                    {
                        g_bDecDspSecureTeeAccessed = FALSE;            dec_dsp_secure_tee_accessed = FALSE;
                    }
                    else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_SND)
                    {
                        g_bSndDspSecureTeeAccessed = FALSE;           snd_dsp_secure_tee_accessed = FALSE;
                    }
                    else if (((AUDIO_DSP_ID)param2) == AUDIO_DSP_ID_ALL)
                    {
                        g_bDecDspSecureTeeAccessed = FALSE;           dec_dsp_secure_tee_accessed = FALSE;
                        g_bSndDspSecureTeeAccessed = FALSE;           snd_dsp_secure_tee_accessed = FALSE;
                    }
                    break;

                default:
                    break;
            }
            break;
#endif

        /* not supported cmd */
        default:
#if defined (__aarch64__)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[0x%x,0x%lx,0x%lx] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[0x%x,0x%x,0x%x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#endif
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetCommInfo()
/// @brief \b Function \b Description: This routine info of audio decoders
/// @param <IN>        \b Audio_COMM_infoType    : request info type of audio decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b long long :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U64 HAL_MAD_GetCommInfo( Audio_COMM_infoType infoType )
{
    MS_U64 result = 0;
    MS_U8 cmd;

    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        /********************************************************************************
        *       Common
        ********************************************************************************/
        case Audio_Comm_infoType_getHDMI_CopyRight_C_Bit:
            result = _AU_AbsReadByte(0x1027B4) & 0x20;
            break;

        case Audio_Comm_infoType_getHDMI_CopyRight_L_Bit:
            result = _AU_AbsReadByte(0x1027B5) & 0x01;
            break;

        case Audio_Comm_infoType_Get_UNI_ES_MEMCNT:
            result = ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_UNI_ES_MEMCNT) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_Get_UNI_ES_Base:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR;
           break;

        case Audio_Comm_infoType_Get_UNI_ES_Size:
            result = ES1_DRAM_SIZE_BYTE ;
            break;

        case Audio_Comm_infoType_Get_UNI_PCM_Base:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + DSP_DDR_SIZE+OFFSET_PCM1_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_UNI_PCM_Size:
            result = PCM1_DRAM_SIZE_BYTE;
            break;

        case Audio_Comm_infoType_Get_AccumEsCnt:
//            result = HAL_MAD2_Read_DSP_sram(REG_PTS_AccumEsCnt, DSP_MEM_TYPE_DM);
            result = 0;
            break;

        case Audio_Comm_infoType_Is_Decoder_In_R2:
            result = 0;
            break;

        /********************************************************************************
        *       DECODER 1
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC1_capability:
            result = Audio_DEC_PCM  | Audio_DEC_MPEG   |
                     Audio_DEC_MP3  | Audio_DEC_AC3    |
                     Audio_DEC_AC3P | Audio_DEC_AAC_LC |
                     Audio_DEC_HEAAC_V1 |Audio_DEC_HEAAC_V2 |
                     Audio_DEC_DOLBYPULSE | Audio_DEC_RA8|
                     Audio_DEC_WMA  | Audio_DEC_WMAPRO |
                     Audio_DEC_XPCM | Audio_DEC_DTS | Audio_DEC_FLAC;
            break;

        case Audio_Comm_infoType_ADEC1_currAudDecStatus:
        case Audio_Comm_infoType_DecStatus:                     //no change
            if ((HAL_MAD_Rpt_DTVES()&0x3)!=0)
                result = 1;
            else
                result = 0;
            break;

        case Audio_Comm_infoType_ADEC1_AudioSyncStatus:
            if((HAL_AUDIO_ReadByte(REG_DEC_DECODE_CMD)&0x20)==0)
                result = 1;  // AV-sync
            else
                 result = 0;  // Free-run
            break;

        case Audio_Comm_infoType_ADEC1_sampleRate:
        case Audio_Comm_infoType_SampleRate:                    //HAL_MAD_GetMadSamplingRate
        {
                MS_U32 fs_type = 0xFFFF;
                fs_type = HAL_MAD2_Read_DSP_sram(REG_DEC2_SampleRate,DSP_MEM_TYPE_PM);
            result = AUDIO_FS_NONE;

                switch(fs_type)
            {
                case 8000:   result = AUDIO_FS_8KHZ;   break;
                case 11025:  result = AUDIO_FS_11KHZ;  break;
                case 12000:  result = AUDIO_FS_12KHZ;  break;
                case 16000:  result = AUDIO_FS_16KHZ;  break;
                case 22050:  result = AUDIO_FS_22KHZ;  break;
                case 24000:  result = AUDIO_FS_24KHZ;  break;
                case 32000:  result = AUDIO_FS_32KHZ;  break;
                case 44100:  result = AUDIO_FS_44KHZ;  break;
                case 48000:  result = AUDIO_FS_48KHZ;  break;
                case 65535:  result = AUDIO_FS_NONE;   break;
                case 64000:  //result = AUDIO_FS_64KHZ;  break;
                case 88200:  //result = AUDIO_FS_88KHZ;  break;
                case 96000:  //result = AUDIO_FS_96KHZ;  break;
                case 176000: //result = AUDIO_FS_176KHZ; break;
                case 192000: //result = AUDIO_FS_192KHZ; break;
                default:     result = AUDIO_FS_NONE;   break;
            }
        }
        break;

        case Audio_Comm_infoType_ADEC1_bitRate:
            break;

        case Audio_Comm_infoType_ADEC1_acmod:
        case Audio_Comm_infoType_ChannelMode:                   //HAL_MAD_GetMadChannelMode
        {
                MS_U8 acmod = 0xFF;

                acmod = HAL_MAD2_Read_DSP_sram(REG_DEC2_SoundMode,DSP_MEM_TYPE_PM);
                result = AUD_CH_MODE_NONE;

                if ((HAL_MAD_Rpt_DTVES()&0x0F)==0x00 )          // Decoder abnormal, return CH_MODE_NOE
                    break;

                if ((HAL_MAD_Rpt_DTVES()&0xF0)==0x40 )          // MPEG/MP3
                {
                    if (acmod == 0)
                    result =  AUD_CH_MODE_STEREO;
                    else if (acmod == 1)
                        result = AUD_CH_MODE_JOINT_STEREO;
                    else if (acmod == 2)
                    result =  AUD_CH_MODE_DUAL_MONO;
                    else if (acmod == 3)
                        result = AUD_CH_MODE_MONO;
                        }
                else if ((HAL_MAD_Rpt_DTVES()&0xF0)==0x50 )     // MS10
                    result = (AUD_CH_MODE_TYPE)acmod;
                else if ((HAL_MAD_Rpt_DTVES()&0xF0)==0x70 )     // DDP
                    result = (AUD_CH_MODE_TYPE)acmod;
                            else
                    result = (AUD_CH_MODE_TYPE)acmod;           //For XPCM case
        }
            break;

        case Audio_Comm_infoType_ADEC1_soundmod:
        case Audio_Comm_infoType_SoundMode:
            result = HAL_AUDIO_ReadReg(REG_MB_MODE_SELECT)&0x3;
            break;

        case Audio_Comm_infoType_ADEC1_okFrmCnt:
            result = HAL_MAD2_Read_DSP_sram((DSP2DmAddr_sys_DecStatus+SYS_DECSTATUS_OK_CNT), DSP_MEM_TYPE_DM) & 0x0000FFFF;
            break;

        case Audio_Comm_infoType_ADEC1_errFrmCnt:
        case Audio_Comm_infoType_DecodeErrorCnt:                //HAL_MAD_Get_DecodeErrorCnt
            result = HAL_MAD2_Read_DSP_sram(REG_DEC2_ErrorCount, DSP_MEM_TYPE_PM);
            break;

        case Audio_Comm_infoType_ADEC1_1ms_timeStamp:
        case Audio_Comm_infoType_1ms_PTS:                       //no change
            result = HAL_MAD_ReadTimeStamp();
            break;

        case Audio_Comm_infoType_ADEC1_33bit_PTS:
        case Audio_Comm_infoType_33Bit_PTS:                     //HAL_MAD_GetMadPts
            cmd = HAL_MAD2_GetDecCmd()&0x1F;
            if ((cmd == AU_DVB_DECCMD_PLAYFILETSP)||(cmd == AU_DVB_DECCMD_PLAYFILE2_PTS))
            {
                MS_U16 pts_lo = HAL_AUDIO_ReadReg(S2M_MBOX_MM_PTS_LO);
                MS_U16 pts_me = HAL_AUDIO_ReadReg(S2M_MBOX_MM_PTS_ME);
                MS_U16 pts_hi = HAL_AUDIO_ReadReg(S2M_MBOX_MM_PTS_HI);

                result = pts_hi;
                result <<= 16;
                result += pts_me;
                result <<= 16;
                result += pts_lo;
            }
            else
            {
                result = HAL_MAD2_Read_DSP_sram(REG_DEC2_AUDIO_PTS_H, DSP_MEM_TYPE_PM);
                result <<= 24;
                result += HAL_MAD2_Read_DSP_sram(REG_DEC2_AUDIO_PTS_L, DSP_MEM_TYPE_PM);
            }
            break;

        case Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF:
        case Audio_Comm_infoType_33Bit_STCPTS_DIFF:             //HAL_MAD_GetMadPtsStcDelta
            result = HAL_MAD2_Read_DSP_sram(REG_DEC2_PtsStcDelta_H, DSP_MEM_TYPE_PM);
            result <<= 24;
            result += HAL_MAD2_Read_DSP_sram(REG_DEC2_PtsStcDelta_L, DSP_MEM_TYPE_PM);

            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC1_Get_PTS_latency:
            break;

        case Audio_Comm_infoType_ADEC1_Get_PCM_bufferEmpty:
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqFlag:
        {
            if (HAL_AUDIO_GetPlayFileFlag(DSP_SE))
                result = 1;
            else
                result = 0;
        }
            break;

        case Audio_Comm_infoType_DEC1_BufferAddr:               //HAL_MAD_GetBufferAddr
            result = ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_ADDR) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqAddr:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC)+((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_ADDR) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqSize:
        case Audio_Comm_infoType_DEC1_BufferSize:               //HAL_MAD_GetBufferSize
            result = ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_SIZE) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_currLevel:
        case Audio_Comm_infoType_DEC1_ESBufferSize:             //HAL_MAD_GetESBufferSize
            result = ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_DDR_ES_BUF_SIZE) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_pcmBuf_currLevel:
        case Audio_Comm_infoType_DEC1_MMResidualPCM:            //HAL_MAD_GetMMResidualPCM
        case Audio_Comm_infoType_DEC1_PCMBufferSize:            //HAL_MAD_GetPCMBufferSize
            result = ((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_DDR_PCM_BUF_SIZE) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_pcm_wptr:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_UNI_PCM_WPTR) << BYTES_IN_MIU_LINE_LOG2) + 0x100000;
            break;

        case Audio_Comm_infoType_ADEC1_UNI_pcm_decSize:
            result = (((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_UNI_PCM_OUTSIZE)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_OutPCMLevel:
            result = (((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_UNI_PCM3_LEVEL)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC1_getDecInfo1:
        case Audio_Comm_infoType_ADEC1_getDecInfo2:
        case Audio_Comm_infoType_ADEC1_getDecInfo3:
        case Audio_Comm_infoType_ADEC1_getDecInfo4:
        case Audio_Comm_infoType_ADEC1_getDecInfo5:
        case Audio_Comm_infoType_ADEC1_getDecInfo6:
        case Audio_Comm_infoType_ADEC1_getDecInfo7:
        case Audio_Comm_infoType_ADEC1_getDecInfo8:
            {
                MS_U8 param_offset = infoType - Audio_Comm_infoType_ADEC1_getDecInfo1;
                result = HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE+param_offset, DSP_MEM_TYPE_DM);
            }
            break;

        case Audio_Comm_infoType_DMAReader_BufferLevel:
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0010);
            result = ((MS_U32)HAL_AUDIO_ReadReg(0x2B8C)<<3);      // 0x2B8C is line size
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0000);
            break;

        case Audio_Comm_infoType_ADEC1_getDecChannelInfo:
           // result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_audio_unsupported:
            // NOT_DEFINE : to do
            if(HAL_AUDIO_ReadByte(S2M_MBOX_INTR_CMDTYPE + 1) == 0x09)
                result=1;
            else
                result=0;
            break;

        case Audio_Comm_infoType_ADEC1_avSyncOffset:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_AvSync+SYS_AVSYNC_AVSYNC_OFFSET, DSP_MEM_TYPE_DM) & 0x00FFFFFF;
            break;

        case Audio_Comm_infoType_ADEC1_getlatest_PTS:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_AvSync+SYS_AVSYNC_PTS_H, DSP_MEM_TYPE_DM)&0x00FFFFFF;
            result <<= 24;
            result += HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_AvSync+SYS_AVSYNC_PTS_L, DSP_MEM_TYPE_DM)&0x00FFFFFF;

            //sign ext
            result <<= 31;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC1_avsync_delay_upperlimit:
            if(g_ac3_delay>g_aac_delay)
            {
                if(g_ac3_delay>g_mpeg_delay)
                {
                    result = ((DELAY_UPPER_LIMIT - g_ac3_delay)/MS_CHANGE_TO_TIME_STAMP);
                }
                else
                {
                    result = ((DELAY_UPPER_LIMIT - g_mpeg_delay)/MS_CHANGE_TO_TIME_STAMP);
                }
            }
            else
            {
                if(g_aac_delay>g_mpeg_delay)
                {
                    result = ((DELAY_UPPER_LIMIT - g_aac_delay)/MS_CHANGE_TO_TIME_STAMP);
                }
                else
                {
                    result = ((DELAY_UPPER_LIMIT - g_mpeg_delay)/MS_CHANGE_TO_TIME_STAMP);
                }
            }
            break;
        /*******************************************************************************
        *       DECODER 2
        *******************************************************************************/
        case Audio_Comm_infoType_ADEC2_capability:
            result = Audio_DEC_MP3;
            break;

        case Audio_Comm_infoType_ADEC2_currAudDecStatus:
            if (HAL_AUDIO_SeReadByte(REG_MB_DEC2_ID_STATUS)&0x3)
                result = 1;
            else
                result = 0;
            break;

        case Audio_Comm_infoType_ADEC2_sampleRate:
            {
                MS_U32 fs_type = 0xFFFF;

                fs_type = HAL_MAD2_Read_DSP_sram(REG_DEC2_SampleRate,DSP_MEM_TYPE_PM);
                result = AUDIO_FS_NONE;

                if ((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0x0F)==0x00 )          // Decoder abnormal, return CH_MODE_NOE
                    break;

                if ((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0xF0)==0x40 )          // MPEG/MP3
                {
                    if (fs_type == 0)
                        result = AUDIO_FS_44KHZ;
                    else if (fs_type == 1)
                        result = AUDIO_FS_48KHZ;
                    else if (fs_type == 2)
                        result = AUDIO_FS_32KHZ;
                    else if (fs_type == 3)
                        result = AUDIO_FS_22KHZ;
                    else if (fs_type == 4)
                        result = AUDIO_FS_24KHZ;
                    else if (fs_type == 5)
                        result = AUDIO_FS_16KHZ;
                    else if (fs_type == 6)
                        result = AUDIO_FS_11KHZ;
                    else if (fs_type == 7)
                        result = AUDIO_FS_12KHZ;
                    else if (fs_type == 8)
                        result = AUDIO_FS_8KHZ;
                }
                else if (((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0xF0)==0x50)||((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0xF0)==0x70 ))     // MS10 / DDP
                {
                    if (fs_type == 44100)
                        result = AUDIO_FS_44KHZ;
                    else if (fs_type == 32000)
                        result = AUDIO_FS_32KHZ;
                    else if (fs_type == 48000)
                        result = AUDIO_FS_48KHZ;
                }
            }
            break;

        case Audio_Comm_infoType_ADEC2_bitRate:
            break;

        case Audio_Comm_infoType_ADEC2_acmod:
        {
                MS_U8 acmod = 0xFF;

                acmod = HAL_MAD2_Read_DSP_sram(REG_DEC2_SoundMode,DSP_MEM_TYPE_PM);
                result = AUD_CH_MODE_NONE;

                if ((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0x0F)==0x00 )          // Decoder abnormal, return CH_MODE_NOE
                    break;

                if ((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0xF0)==0x40 )          // MPEG/MP3
                {
                    if (acmod == 0)
                    result =  AUD_CH_MODE_STEREO;
                    else if (acmod == 1)
                        result = AUD_CH_MODE_JOINT_STEREO;
                    else if (acmod == 2)
                    result =  AUD_CH_MODE_DUAL_MONO;
                    else if (acmod == 3)
                        result = AUD_CH_MODE_MONO;
            }
                else if ((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0xF0)==0x50 )     // MS10
                    result = (AUD_CH_MODE_TYPE)acmod;
                else if ((HAL_AUDIO_ReadByte(REG_MB_DEC2_ID_STATUS)&0xF0)==0x70 )     // DDP
                    result = (AUD_CH_MODE_TYPE)acmod;
        }
            break;

        case Audio_Comm_infoType_ADEC2_soundmod:
        case Audio_Comm_infoType_ADEC2_okFrmCnt:
        case Audio_Comm_infoType_ADEC2_errFrmCnt:
            break;

        case Audio_Comm_infoType_ADEC2_1ms_timeStamp:
            {
            long long tmp1 = (long long) HAL_MAD2_Read_DSP_sram(REG_DEC2_TIME_STAMP_SEC, DSP_MEM_TYPE_DM);
            long long tmp2 = (long long) HAL_MAD2_Read_DSP_sram(REG_DEC2_TIME_STAMP_MS, DSP_MEM_TYPE_DM);
            result = (tmp1 * 1000) + ( (tmp2 & 0xFF) * 4 );
            }
            break;

        case Audio_Comm_infoType_ADEC2_33bit_PTS:
            if(HAL_AUDIO_ReadByte(S2M_MBOX_INTR_CMDTYPE+1) == 0x05)
            {
                result = ( ((long long)HAL_MAD2_Read_DSP_sram(REG_DEC2_TS_PTS_H,DSP_MEM_TYPE_DM) << 32) |
                           ((long long)HAL_MAD2_Read_DSP_sram(REG_DEC2_TS_PTS_M,DSP_MEM_TYPE_DM) << 16) |
                           ((long long)HAL_MAD2_Read_DSP_sram(REG_DEC2_TS_PTS_L,DSP_MEM_TYPE_DM) << 0) );
            }
            else
                result=0;
            break;

        case Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF:
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqFlag:
            result = g_AudioVars2->g_bSePlayFileFlag;
            break;

        case Audio_Comm_infoType_DEC2_BufferAddr:               //HAL_MAD2_GetBufferAddr
            result = ((HAL_MAD2_Read_DSP_sram(REG_DEC2_LINE_BUF_ADDR,DSP_MEM_TYPE_DM)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqAddr:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_DEC)+((HAL_MAD2_Read_DSP_sram(REG_DEC2_LINE_BUF_ADDR,DSP_MEM_TYPE_DM)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqSize:
        case Audio_Comm_infoType_DEC2_BufferSize:               //HAL_MAD2_GetBufferSize
            result = ((HAL_MAD2_Read_DSP_sram(REG_DEC2_LINE_BUF_SIZE,DSP_MEM_TYPE_DM)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_currLevel:
            result = ((HAL_MAD2_Read_DSP_sram(REG_DEC2_DDR_ES_BUF_SIZE,DSP_MEM_TYPE_DM)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC2_pcmBuf_currLevel:
        case Audio_Comm_infoType_DEC2_MMResidualPCM:            //HAL_MAD2_GetMMResidualPCM
            result = ((HAL_MAD2_Read_DSP_sram(REG_DEC2_RESIDUAL_PCM,DSP_MEM_TYPE_DM)) << BYTES_IN_MIU_LINE_LOG2);
            break;

        case Audio_Comm_infoType_ADEC2_getDecInfo1:
        case Audio_Comm_infoType_ADEC2_getDecInfo2:
        case Audio_Comm_infoType_ADEC2_getDecInfo3:
        case Audio_Comm_infoType_ADEC2_getDecInfo4:
        case Audio_Comm_infoType_ADEC2_getDecInfo5:
        case Audio_Comm_infoType_ADEC2_getDecInfo6:
        case Audio_Comm_infoType_ADEC2_getDecInfo7:
        case Audio_Comm_infoType_ADEC2_getDecInfo8:
            //{
            //    MS_U8 param_offset = infoType - Audio_Comm_infoType_ADEC2_getDecInfo1;
            //    result = HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE+param_offset, DSP_MEM_TYPE_DM);
            //}
            break;

        /********************************************************************************
        *       DECODER 3
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC3_capability:
        case Audio_Comm_infoType_ADEC3_currAudDecStatus:
        case Audio_Comm_infoType_ADEC3_sampleRate:
        case Audio_Comm_infoType_ADEC3_bitRate:
        case Audio_Comm_infoType_ADEC3_acmod:
        case Audio_Comm_infoType_ADEC3_soundmod:
        case Audio_Comm_infoType_ADEC3_reqDataSize:
        case Audio_Comm_infoType_ADEC3_okFrmCnt:
        case Audio_Comm_infoType_ADEC3_errFrmCnt:
        case Audio_Comm_infoType_ADEC3_1ms_timeStamp:
        case Audio_Comm_infoType_ADEC3_33bit_PTS:
        case Audio_Comm_infoType_ADEC3_33bit_STCPTS_DIFF:
        case Audio_Comm_infoType_ADEC3_esBuf_reqFlag:
        case Audio_Comm_infoType_ADEC3_esBuf_reqAddr:
        case Audio_Comm_infoType_ADEC3_esBuf_reqSize:
        case Audio_Comm_infoType_ADEC3_esBuf_currLevel:
        case Audio_Comm_infoType_ADEC3_pcmBuf_currLevel:
        case Audio_Comm_infoType_ADEC3_getDecInfo1:
        case Audio_Comm_infoType_ADEC3_getDecInfo2:
        case Audio_Comm_infoType_ADEC3_getDecInfo3:
        case Audio_Comm_infoType_ADEC3_getDecInfo4:
        case Audio_Comm_infoType_ADEC3_getDecInfo5:
        case Audio_Comm_infoType_ADEC3_getDecInfo6:
        case Audio_Comm_infoType_ADEC3_getDecInfo7:
        case Audio_Comm_infoType_ADEC3_getDecInfo8:
        case Audio_Comm_infoType_ADEC3_getDecChannelInfo:
            break;

        case Audio_Comm_infoType_ReadByte:
            result = (MS_U64)HAL_AUDIO_AbsReadByte(g_ReadTargetAddr);
            break;

        case Audio_Comm_infoType_getSignal_Energy:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_dec1_signal_energy, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_getNR_Status:
            result = (HAL_AUDIO_ReadByte(S2M_MBOX_NR_STATUS)& 0x01);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Protect:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_hdmi_unstable_protect, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Threshold:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_hdmi_unstable_threshold, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_ADEC3_avSyncOffset:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_AvSync+SYS_AVSYNC_AVSYNC_OFFSET, DSP_MEM_TYPE_DM) & 0x00FFFFFF;
            break;

        case Audio_comm_infoType_Get_audio_crash_status:
            result = g_AudioVars2->g_u8AudioCrashFlag;
            break;

        case Audio_Comm_infoType_Get_Parser_Architecture:
            result = (MS_U64)E_AUD_1_PARSER; // 1 AFIFO arch
            break;

        case Audio_comm_infoType_Get_Main_Input_Type:
            result = g_AudioVars2->eMainAudioSource;
            break;

        case Audio_comm_infoType_Get_Sub_Input_Type:
            result = g_AudioVars2->eSubAudioSource;
            break;

        case Audio_Comm_infoType_Get_Dra_ChannelMode:
            result = HAL_MAD_GetDRAInfo(Audio_DRA_infoType_NumCh);
            break;

        case Audio_Comm_infoType_Get_Dra_SampleRate:
            result = HAL_MAD_GetDRAInfo(Audio_DRA_infoType_SampleRate);
            break;

        /* not support cmd */
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: cmd not Supported\n", __FUNCTION__ );
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetMpegInfo()
/// @brief \b Function \b Description: This routine set different paramter to mpeg decoder
/// @param <IN>        \b Audio_MPEG_infoType    : mpeg audio parameter type
/// @param <IN>        \b MS_U32    : common mpeg parameter 1
/// @param <IN>        \b MS_U32    : common mpeg parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    switch(infoType)
    {
        case Audio_MPEG_infoType_SoundMode:                 //HAL_MAD_MPEG_SetSoundMode
            {
                MS_U8 value = 0;                            //default LL mode

                switch((AUD_MPEG_SOUNDMODE)param1)
                {
                    case AUD_MPEG_SOUNDMODE_LL:     value = 0x4;  break;
                    case AUD_MPEG_SOUNDMODE_RR:     value = 0x8;  break;
                    case AUD_MPEG_SOUNDMODE_LR:     value = 0x0;  break;
                    case AUD_MPEG_SOUNDMODE_MIX_LR: value = 0xC;  break;
                    default:                        value = 0x0;  break;
                }
                HAL_AUDIO_WriteMaskReg(REG_MB_DEC_CTRL, 0x000C, value);
            }
            break;

        case Audio_MPEG_infoType_MMFileSize:                //HAL_MAD_MPEG_SetFileSize
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        case Audio_MPEG_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))              //delay offset range from 30ms ~ 300ms
            {
            	g_mpeg_delay = MPG_AV_DEFAULT_DELAY;
            }
            else
            {
            	g_mpeg_delay = param1;
            }
            HAL_MAD2_Write_DSP_sram(REG_MPG_AVdelay, g_mpeg_delay, DSP_MEM_TYPE_PM);
            break;

        case Audio_MPEG_DTV_AVSYNC_DISABLE_SETTING:
            g_mpeg_delay = MPG_AV_DEFAULT_DELAY;
            HAL_MAD2_Write_DSP_sram(REG_MPG_AVdelay, g_mpeg_delay, DSP_MEM_TYPE_PM);
            break;

        default:
#if defined (__aarch64__)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[0x%x,0x%08lx,0x%08lx] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[0x%x,0x%08x,0x%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#endif
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetMpegInfo()
/// @brief \b Function \b Description: This routine info of mpeg decoder
/// @param <IN>        \b Audio_MPEG_infoType    : request info type of mpeg decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetMpegInfo( Audio_MPEG_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_MPEG_infoType_DecStatus:     //no change
            if ((HAL_MAD_Rpt_DTVES()&0x40) && (HAL_MAD_Rpt_DTVES()&0x01))
                result = 1;
            else
                result = 0;
            break;

        case Audio_MPEG_infoType_BitRate:       //HAL_MAD_MPEG_GetBitRate
            result = HAL_MAD_MPEG_GetBitRate();
            break;

        case Audio_MPEG_infoType_FrameNum:      //HAL_MAD_MPEG_GetframeNum
            result = HAL_MAD2_Read_DSP_sram(REG_MPEG_FRAMENUM, DSP_MEM_TYPE_DM);
            break;

        case Audio_MPEG_infoType_Layer:         //HAL_MAD_MPEG_GetLayer
            result = (HAL_MAD2_Read_DSP_sram(REG_DEC2_INFO_BASE, DSP_MEM_TYPE_DM) & 0x0C)>>2;
            break;

        case Audio_MPEG_infoType_SampleRate:    //HAL_MAD_MPEG_GetSampleRate
            result = HAL_MAD_MPEG_GetSampleRate();
            break;

        case Audio_MPEG_infoType_stereoMode:    //HAL_MAD_MPEG_GetSoundMode
            result = HAL_MAD_MPEG_GetSoundMode();
            break;

        case Audio_MPEG_infoType_Header:        //HAL_MAD_MPEG_GetHeaderInfo
            result = HAL_MAD_MPEG_GetHeaderInfo();
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetWmaInfo()
/// @brief \b Function \b Description: This routine set different paramter to wma decoder
/// @param <IN>        \b Audio_WMA_infoType    : wma audio parameter type
/// @param <IN>        \b MS_U32    : common wma parameter 1
/// @param <IN>        \b MS_U32    : common wma parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    switch(infoType)
    {
        case Audio_WMA_infoType_Asf_Param:
            HAL_MAD_WMA_SetASFParm((WMA_ASF_PARMTYPE)param1, param2);       //HAL_MAD_WMA_SetASFParm
            break;

        case Audio_WMA_infoType_FFSpeed:                                    //HAL_MAD_WMA_SetFastFowardSpeed
            if (param1 <= 3)
                HAL_MAD2_Write_DSP_sram(0x185D,param1,DSP_MEM_TYPE_DM);
            break;

        case Audio_WMA_infoType_MMFileSize:
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        default:
#if defined (__aarch64__)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#endif
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetWmaInfo()
/// @brief \b Function \b Description: This routine info of wma decoder
/// @param <IN>        \b Audio_WMA_infoType    : request info type of wma decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetWmaInfo( Audio_WMA_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_WMA_infoType_BitRate:                    //HAL_MAD_WMA_GetBitRate
            result = (HAL_MAD2_Read_DSP_sram(DSP2DmAddr_wma_BitRate,DSP_MEM_TYPE_DM))<<3;
            break;

        case Audio_WMA_infoType_SampleRate:                 //HAL_MAD_WMA_GetSampleRate
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_wma_smpRate, DSP_MEM_TYPE_DM);
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetAC3PInfo()
/// @brief \b Function \b Description: This routine set different parameter to ac3+ decoder
/// @param <IN>        \b Audio_AC3P_infoType    : AC3+ parameter type
/// @param <IN>        \b MS_U32    : AC3+ parameter 1
/// @param <IN>        \b MS_U32    : AC3+ parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    switch(infoType)
    {
        case Audio_AC3P_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            if((DOLBY_DOWNMIX_MODE)param1 == DOLBY_DOWNMIX_MODE_LORO)
            {
                HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x40, 0x40);       // LoRo Mod
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x40, 0x00);       // LtRt Mod
            }
            break;

        case Audio_AC3P_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            if((DOLBY_DRC_MODE)param1 == RF_MODE) {
                HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x80, 0x80);       // RF Mod
                g_drcMode = RF_MODE;
            }
            else {
                HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL, 0x80, 0x00);       // Line Mod
                g_drcMode = LINE_MODE;
            }

            /* update PCM level setting after g_drcMode is updated */
            HAL_MAD_PcmLevelControl();
            break;

        case Audio_AC3P_infoType_MMFileSize:            //HAL_MAD_ac3p_setFileSize
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        case Audio_AC3P_infoType_syncSTC_in_mmTs:
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_syncSTC_in_mmTs, param1, param2);
            break;

        case Audio_AC3P_infoType_hdmiTxBypass_enable:
            break;

        case Audio_AC3P_infoType_BYTESWAP:
             if(param1 == 1) {
                 HAL_AUDIO_WriteMaskByte(0x2C48, 0x10, 0x10);
                 HAL_AUDIO_WriteMaskByte(0x2C49, 0x05, 0x05);
                 }else{
                 HAL_AUDIO_WriteMaskByte(0x2C48, 0x10, 0x00);
                 HAL_AUDIO_WriteMaskByte(0x2C49, 0x05, 0x00);
        }
            break;

        case Audio_AC3P_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))
            {
            	g_ac3_delay = AC3_AV_DEFAULT_DELAY;
            }
            else
            {
            	g_ac3_delay = param1;
            }
                HAL_MAD2_Write_DSP_sram(REG_AC3_AVdelay, g_ac3_delay, DSP_MEM_TYPE_PM);
            break;

        case Audio_AC3P_DTV_AVSYNC_DISABLE_SETTING:
            g_ac3_delay = AC3_AV_DEFAULT_DELAY;
            HAL_MAD2_Write_DSP_sram(REG_AC3_AVdelay, g_ac3_delay, DSP_MEM_TYPE_PM);
            break;

        case Audio_AC3P_infoType_hdmiAC3inSE:
            break;

        case Audio_AC3P_infoType_enableDolbyBulletin11:
            if ( param1 )
                g_AudioVars2->g_dolbyLoudness_Enable = TRUE;
            else
                g_AudioVars2->g_dolbyLoudness_Enable = FALSE;

            HAL_MAD_PcmLevelControl();
            break;

        case Audio_AC3P_infoType_Drc_HighCutScale:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT, 0x0F, param1);
            break;

        case Audio_AC3P_infoType_Drc_LowBootScale:
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT, 0xF0, param1<<4);
            break;

        case Audio_AC3P_infoType_LoudnessMatching:
            if(param1)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT+1, 0x80, 0x80);
                g_AudioVars2->g_dolbyLoudness_Enable = TRUE;
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT+1, 0x80, 0x00);
                g_AudioVars2->g_dolbyLoudness_Enable = FALSE;
            }

            if( (DOLBY_LOUDNESS_MODE)param2 == DVB_MODE )
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT+1, 0x60, 0x00);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = DVB_MODE;
            }
            else if ((DOLBY_LOUDNESS_MODE)param2 == ATSC_MODE)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT+1, 0x60, 0x40);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = ATSC_MODE;
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_LOW_HIGH_CUT+1, 0x60, 0x20);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = OTHER_MODE;
            }

            HAL_MAD_PcmLevelControl();
            break;
        default:
#if defined (__aarch64__)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#endif
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAC3PInfo()
/// @brief \b Function \b Description: This routine return info type of ac3+ decoder
/// @param <IN>        \b Audio_AC3P_infoType    : AC3+ parameter type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32 :    AC3+ paramter info
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetAC3PInfo( Audio_AC3P_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_AC3P_infoType_DecStatus:
            if ((HAL_MAD_Rpt_DTVES()&0xF1)==0x71)
                result = 1;
            else
                result = 0;
            break;

        case Audio_AC3P_infoType_SampleRate:
            {
                MS_U8 fscod = 0;

                fscod  = (MS_U8)HAL_MAD2_Read_DSP_sram(DSP2DmAddr_ac3p_fscod, DSP_MEM_TYPE_DM);        //DD+
                switch(fscod)
                {
                    case 1: result = SAMPLE_RATE_44100; break;
                    case 2: result = SAMPLE_RATE_32000; break;
                    default:
                    case 0: result = SAMPLE_RATE_48000; break;
                }
            }
            break;

        case Audio_AC3P_infoType_BitRate:  //unit : kbps
            {
                MS_U8 fscod;

                fscod  = (MS_U8)HAL_MAD2_Read_DSP_sram(DSP2DmAddr_ac3p_fscod, DSP_MEM_TYPE_DM);        //DD+
                result = (MS_U8)HAL_MAD2_Read_DSP_sram(DSP2DmAddr_ac3p_frmSz, DSP_MEM_TYPE_DM);        //DD+
                result <<= 4;           //to bits

                switch( fscod )
                {
                    case 1:     //44.1K
                        result *= 29;   break;

                    case 2:     //32K
                        result *= 21;   break;

                    default:    //48K
                        result *= 31;   break;
                }                       //to bps

                result >>= 10;          //to kbps
            }
            break;

        case Audio_AC3P_infoType_Acmod:
            result = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_acmod);
            break;

        case Audio_AC3P_infoType_DrcMode:
            if (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL) & 0x80)
                result = RF_MODE;
            else
                result = LINE_MODE;
            break;

        case Audio_AC3P_infoType_DownmixMode:
            if (HAL_AUDIO_ReadByte(REG_M2D_MAILBOX_SPDIF_CTRL) & 0x40)
                result = DOLBY_DOWNMIX_MODE_LORO;
            else
                result = DOLBY_DOWNMIX_MODE_LTRT;
            break;

        case Audio_AC3P_infoType_AC3Type:
            if ((HAL_MAD_Rpt_DTVES()&0xF1)!=0x71)
            {
                result = 0;             //Invalid
            }
            else
            {
                /* bsid <= 8 */
                if (HAL_MAD2_Read_DSP_sram(DSP2DmAddr_ac3p_bsid, DSP_MEM_TYPE_DM) <= 8)
                    result = 1;         //DD
                else
                    result = 2;         //DD+
            }
            break;

        case Audio_AC3P_infoType_FrameSize:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_ac3p_frmSz, DSP_MEM_TYPE_DM)<<1;
            break;

        case Audio_AC3P_infoType_BSMode:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_ac3p_bsMod, DSP_MEM_TYPE_DM);
            break;


        case Audio_AC3P_infoType_hdmiAC3inSE:
            result = 1;
            break;

         case  Audio_AC3P_infoType_hdmi_in_support_DDP:
            result = 1;
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetAC3Info()
/// @brief \b Function \b Description: This routine set different paramter to ac3 decoder
/// @param <IN>        \b Audio_WMA_infoType    : ac3 audio parameter type
/// @param <IN>        \b MS_U32    : common ac3 parameter 1
/// @param <IN>        \b MS_U32    : common ac3 parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    switch(infoType)
    {
        case Audio_AC3_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType_DownmixMode, param1, param2);
            break;

        case Audio_AC3_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            HAL_MAD_SetAC3PInfo( Audio_AC3P_infoType_DrcMode, param1, param2);
            break;

        default:
#if defined (__aarch64__)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#endif
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAC3Info()
/// @brief \b Function \b Description: This routine info of ac3 decoder
/// @param <IN>        \b Audio_AC3_infoType    : request info type of ac3 decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetAC3Info( Audio_AC3_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_AC3_infoType_DecStatus:
            if (((HAL_MAD_Rpt_DTVES()&0xF1)==0x71)||((HAL_MAD_Rpt_DTVES()&0xF1)==0x81))
                result = 1;
            else
                result = 0;
            break;

        case Audio_AC3_infoType_Acmod:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_Acmod);
            break;

        case Audio_AC3_infoType_BitRate:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_BitRate);
            break;

        case Audio_AC3_infoType_SampleRate:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_SampleRate);
            break;

        case Audio_AC3_infoType_DownmixMode:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_DownmixMode);
            break;

        case Audio_AC3_infoType_DrcMode:
            result = HAL_MAD_GetAC3PInfo(Audio_AC3P_infoType_DrcMode);
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDTSCommonCtrl()
/// @brief \b Function \b Description: This routine set different control to DTS decoder
/// @param <IN>        \b Audio_DTS_infoType    : DTS control type
/// @param <IN>        \b MS_U16    : DTS control value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    switch(infoType)
    {
        case Audio_DTS_infoType_DMIX_LFE_EN:
            if (ctrl_type == Audio_DTS_Ctrl_DMIX_LFE_OFF)
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DTS_CH_CTRL, 0x02, 0x00);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DTS_CH_CTRL, 0x02, 0x02);
            }
            break;

        case Audio_DTS_infoType_Dialnorm_EN:
            // always enable
            break;

        case Audio_DTS_infoType_DrcMode:
            // always disable in T3
            break;

        case Audio_DTS_infoType_DownmixMode:
            if (ctrl_type == Audio_DTS_Ctrl_Downmix_LoRo)
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DTS_CH_CTRL, 0x01, 0x00);
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DTS_CH_CTRL, 0x01, 0x01);
            }
            break;

        case Audio_DTS_infoType_ESDataServiceMode:
            //force Audio_DTS_Ctrl_INTRPTMode in T3
            break;

        case Audio_DTS_infoType_ESData_ByteSwap:
            if (ctrl_type == Audio_DTS_Ctrl_ByteSwap_ON)
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DTS_CH_CTRL, 0x80, 0x80);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DTS_CH_CTRL, 0x80, 0x00);
            }
            break;

        case Audio_DTS_infoType_CertMode:
            //DTS Certification Mode, Need to disable Sound Eeffect
            if (ctrl_type == Audio_DTS_Ctrl_CertMode_ON)
            {
                Flag_MAIN_SNDEFFET_CHG = TRUE;
                Reg_REG_SOUND_MAIN_SNDEFFECT = HAL_AUDIO_ReadByte(REG_SOUND_MAIN_SNDEFFECT);
                HAL_AUDIO_WriteByte(REG_SOUND_MAIN_SNDEFFECT,0x00);
                Reg_REG_SOUND_ADV_CFG0 = HAL_AUDIO_ReadByte(REG_SOUND_ADV_CFG0);
                HAL_AUDIO_WriteByte(REG_SOUND_ADV_CFG0,0x00);
            }
            if (ctrl_type == Audio_DTS_Ctrl_CertMode_OFF)
            {
                if(Flag_MAIN_SNDEFFET_CHG)
                {
                    HAL_AUDIO_WriteByte(REG_SOUND_MAIN_SNDEFFECT,Reg_REG_SOUND_MAIN_SNDEFFECT);
                    HAL_AUDIO_WriteByte(REG_SOUND_ADV_CFG0,Reg_REG_SOUND_ADV_CFG0);
                    Flag_MAIN_SNDEFFET_CHG = FALSE;
                }
            }
            break;

        default:
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetDTSInfo()
/// @brief \b Function \b Description: This routine return info type of DTS decoder
/// @param <IN>        \b Audio_DTS_infoType    : DTS parameter type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :    DTS paramter info
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetDTSInfo( Audio_DTS_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        case Audio_DTS_infoType_DecStatus:
            if ((HAL_MAD_Rpt_DTVES()&0x60) == 0x60)
                result = 1;
            else
                result = 0;
            break;

        case Audio_DTS_infoType_KernelVersion:
            //Cathy need to do   result = HAL_MAD2_Read_DSP_sram(DSPPmAddr_kernelVer, DSP_MEM_TYPE_PM);
            break;

        case Audio_DTS_infoType_SampleRate: // Original FS from Header
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_dts_smpRate, DSP_MEM_TYPE_DM);
            break;

        case Audio_DTS_infoType_BitRate:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_dts_BitRate, DSP_MEM_TYPE_DM);
            break;

        case Audio_DTS_infoType_CD_MODE:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_dts_cdMode, DSP_MEM_TYPE_DM);
            break;

        case Audio_DTS_infoType_Amode:
            result = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_dts_amode, DSP_MEM_TYPE_DM);
            break;

        case Audio_DTS_infoType_HDMI_DSP_SEL:
            result = 0x1;                   //0: in DEC DSP, 1: in SND DSP
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetAACInfo()
/// @brief \b Function \b Description: This routine set different paramter to aac decoder
/// @param <IN>        \b Audio_WMA_infoType    : aac audio parameter type
/// @param <IN>        \b MS_U32    : common aac parameter 1
/// @param <IN>        \b MS_U32    : common aac parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    switch(infoType)
    {
        case Audio_AAC_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            if((DOLBY_DOWNMIX_MODE)param1 == DOLBY_DOWNMIX_MODE_LORO)
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_DRC_SETTING, 0x40, 0x40);       // LoRo Mod
            else
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_DRC_SETTING, 0x40, 0x00);       // LtRt Mod
            break;

        case Audio_AAC_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            if((DOLBY_DRC_MODE)param1 == RF_MODE)
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_DRC_SETTING, 0x80, 0x80);       // RF Mod
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(M2S_MBOX_DRC_SETTING, 0x80, 0x00);       // Line Mod
            }
            g_drcMode = (DOLBY_DRC_MODE)param1;
            HAL_MAD_PcmLevelControl();
            break;

        case Audio_AAC_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))                                                      //delay offset range from 30ms ~ 300ms
            {
                g_aac_delay = AAC_AV_DEFAULT_DELAY;
            }
            else
            {
                g_aac_delay = param1;
            }
            HAL_MAD2_Write_DSP_sram(REG_AAC_AVdelay, g_aac_delay, DSP_MEM_TYPE_PM);
            break;

        case Audio_AAC_DTV_AVSYNC_DISABLE_SETTING:
            g_aac_delay = AAC_AV_DEFAULT_DELAY;
            HAL_MAD2_Write_DSP_sram(REG_AAC_AVdelay, g_aac_delay, DSP_MEM_TYPE_PM);
            break;

        default:
#if defined (__aarch64__)
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
#endif
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAACInfo()
/// @brief \b Function \b Description: This routine info of aac decoder
/// @param <IN>        \b Audio_AAC_infoType    : request info type of aac decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetAACInfo( Audio_AAC_infoType infoType )
{
    MS_U32 result=0;

    if((HAL_MAD_Rpt_DTVES() & 0xF0) != 0x50)
        return result;

    switch(infoType)
    {
        case Audio_AAC_error_count:
            result = HAL_MAD2_Read_DSP_sram(REG_ErrorCount, DSP_MEM_TYPE_PM);
            break;

        case Audio_AAC_dec_unsupported:
            result = 0;
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetDtsInfo()
/// @brief \b Function \b Description: This routine set different paramter to aac decoder
/// @param <IN>        \b Audio_WMA_infoType    : aac audio parameter type
/// @param <IN>        \b MS_U32    : common aac parameter 1
/// @param <IN>        \b MS_U32    : common aac parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetDtsInfo( Audio_DTS_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch(infoType)
    {
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetAACInfo()
/// @brief \b Function \b Description: This routine info of aac decoder
/// @param <IN>        \b Audio_AAC_infoType    : request info type of aac decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U32  :  info type
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetDtsInfo( Audio_DTS_infoType infoType )
{
    MS_U32 result = 0;

    switch(infoType)
    {
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetRA8LBRInfo()
/// @brief \b Function \b Description: This routine set different paramter to ra8 lbr decoder
/// @param <IN>        \b Audio_RA8LBR_infoType : ra8 lbr audio parameter type
/// @param <IN>        \b MS_U32                : common ra8 lbr parameter 1
/// @param <IN>        \b MS_U32                : common ra8 lbr parameter 2
/// @param <OUT>       \b NONE                  :
/// @param <RET>       \b MS_BOOL               :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE                  :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_SetRA8LBRInfo( Audio_RA8LBR_infoType infoType, MS_U32 param1, MS_U32 param2 )
{
    switch ( infoType )
    {
        case Audio_RA8LBR_NumCodes:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 0, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Samples:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 1, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_SampleRate:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 2, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Channels1:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 3, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Regions1:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 4, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplStart1:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 5, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplQbits1:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 6, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_FrameSize1:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 7, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Channels2:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 8, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Regions2:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 9, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplStart2:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 10, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplQbits2:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 11, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_FrameSize2:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 12, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Channels3:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 13, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Regions3:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 14, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplStart3:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 15, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplQbits3:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 16, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_FrameSize3:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 17, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Channels4:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 18, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Regions4:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 19, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplStart4:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 20, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplQbits4:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 21, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_FrameSize4:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 22, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Channels5:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 23, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_Regions5:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 24, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplStart5:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 25, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_cplQbits5:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 26, param1, DSP_MEM_TYPE_DM);
            break;
        case Audio_RA8LBR_FrameSize5:
            HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 27, param1, DSP_MEM_TYPE_DM);
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
            break;
    }

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_GetRA8LBRInfo()
/// @brief \b Function \b Description: This routine info of ra8 lbr decoder
/// @param <IN>        \b Audio_RA8LBR_infoType : request info type of ra8 lbr decoder
/// @param <OUT>       \b NONE                  :
/// @param <RET>       \b MS_U32                :  info type
/// @param <GLOBAL>    \b NONE                  :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_MAD_GetRA8LBRInfo( Audio_RA8LBR_infoType infoType )
{
    MS_U32 result = 0;

    switch ( infoType )
    {
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

//******************************************************************************
//  [Function Name]:
//      HAL_MAD_PcmLevelControl
//  [Description]:
//      check Dolby spec Bullutin 11, \\Mstarnas\vivaldi\SPEC\sev_s_devdoc_techbull11levelmatchingdvb.pdf
//  [Arguments]:
//
//*******************************************************************************
void HAL_MAD_PcmLevelControl( void )
{
    if ((AudioInputSrcInfo != E_AUDIO_INFO_GAME_IN) && (g_AudioVars2->g_dolbyLoudness_Enable))
    {
        if ( g_drcMode == LINE_MODE )
        {
            //DVB Line mode
            HAL_AUDIO_DecWriteMaskByte(0x2D8F,0x10,0x00);          //disable  DD/DD+ -3dB
            HAL_AUDIO_SeWriteMaskByte(0x2D8E,0x20,0x00);           //disable  spdif PCM output -8dB
            HAL_AUDIO_DecWriteMaskByte(0x2D86,0x02,0x02);          //enable  non-Dolby main decoder PCM output -8dB
            HAL_AUDIO_SeWriteMaskByte(0x2DD9,0x40,0x40);           //enable  non-Dolby AD decoder PCM output -8dB
        }
        else if ( g_drcMode == RF_MODE )
        {
            //DVB RF mode
            HAL_AUDIO_DecWriteMaskByte(0x2D8F,0x10,0x10);          //enable  DD/DD+ -3dB
            HAL_AUDIO_SeWriteMaskByte(0x2D8E,0x0020,0x0020);       //enable   spdif PCM output -8dB
            HAL_AUDIO_DecWriteMaskByte(0x2D86,0x02,0x00);          //disable  non-Dolby main decoder PCM output -8dB
            HAL_AUDIO_SeWriteMaskByte(0x2DD9,0x40,0x00);           //disable  non-Dolby AD decoder PCM output -8dB
        }
    }
    else
    {   //for other input (ex: HDMI / ATV / AV / MM input )
        HAL_AUDIO_DecWriteMaskByte(0x2D8F,0x10,0x00);              //disable  DD/DD+ -3dB
        HAL_AUDIO_SeWriteMaskByte(0x2D8E,0x0020,0x0000);           //disable  spdif PCM output -8dB
        HAL_AUDIO_DecWriteMaskByte(0x2D86,0x02,0x00);              //disable  non-Dolby main decoder PCM output -8dB
        HAL_AUDIO_SeWriteMaskByte(0x2DD9,0x40,0x00);               //disable  non-Dolby AD decoder PCM output -8dB
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_LoadCode()
/// @brief \b Function \b Description: This routine is used to load the DVB algorithm code
/// @param u8Type      \b algorithm type
/// @return MS_BOOL    \b TRUE --DSP load code okay
///                       FALSE--DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_LoadCode(AUDIO_DSP_CODE_TYPE u8Type)
{
    u8Type = u8Type;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_XPCM_setParam
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_XPCM_setParam(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    MS_U16 u16ParaBaseAddr;
    MS_U8 dsp_memory_type = DSP_MEM_TYPE_DM;
    u16ParaBaseAddr=HAL_MAD_XPCM_GetParaBase();
    HAL_MAD2_Write_DSP_sram(u16ParaBaseAddr, (MS_U32)audioType, (AUDIO_DSP_MEMORY_TYPE)dsp_memory_type);             //audio type
    HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+1), (MS_U32)channels, (AUDIO_DSP_MEMORY_TYPE)dsp_memory_type);          //channel numbers
    HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+2), (MS_U32)sampleRate, (AUDIO_DSP_MEMORY_TYPE)dsp_memory_type);        //sample rate
    HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+3), (MS_U32)bitsPerSample, (AUDIO_DSP_MEMORY_TYPE)dsp_memory_type);     //bits per sample
    HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+4), (MS_U32)blockSize, (AUDIO_DSP_MEMORY_TYPE)dsp_memory_type);         //block size (ADPCM)
    HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+5), (MS_U32)samplePerBlock, (AUDIO_DSP_MEMORY_TYPE)dsp_memory_type);    //sample per block (ADPCM) or endian (LPCM)
    return TRUE;
}

//====== STAOS PIP START 2012/02/01 ========

MS_BOOL HAL_MAD_XPCM_setParam2( AUDIO_DEC_ID DecId, Audio_XPCM_Param * pParam )
{
    Audio_XPCM_Param    * pParamXPCM = pParam;
    AUDIO_DSP_MEMORY_TYPE dsp_memory_type = DSP_MEM_TYPE_DM;
    MS_U16 u16ParaBaseAddr;

        u16ParaBaseAddr = REG_DEC2_PARAM_BASE;

        HAL_MAD2_Write_DSP_sram((REG_DEC2_PARAM_BASE + 0),   (MS_U32) pParamXPCM->audioType,      dsp_memory_type);  //audio type
        HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+1), (MS_U32) pParamXPCM->channels,       dsp_memory_type);  //channel numbers
        HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+2), (MS_U32) pParamXPCM->sampleRate,     dsp_memory_type);  //sample rate
        HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+3), (MS_U32) pParamXPCM->bitsPerSample,  dsp_memory_type);  //bits per sample
        HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+4), (MS_U32) pParamXPCM->blockSize,      dsp_memory_type);  //block size (ADPCM)
        HAL_MAD2_Write_DSP_sram((u16ParaBaseAddr+5), (MS_U32) pParamXPCM->samplePerBlock, dsp_memory_type);  //sample per block (ADPCM) or endian (LPCM)

    return TRUE;
}

MS_BOOL HAL_MAD_COOK_setParam2( AUDIO_DEC_ID DecId, Audio_COOK_Param * pParam )
{
        Audio_COOK_Param * pParamCOOK = pParam;

        DecId = DecId;

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 0, pParamCOOK->mNumCodecs,  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 1, pParamCOOK->mSamples,    DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 2, pParamCOOK->mSampleRate, DSP_MEM_TYPE_DM);

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 3, pParamCOOK->Channels[0],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 4, pParamCOOK->Regions[0],   DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 5, pParamCOOK->cplStart[0],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 6, pParamCOOK->cplQbits[0],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 7, pParamCOOK->FrameSize[0], DSP_MEM_TYPE_DM);

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 8,  pParamCOOK->Channels[1],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 9,  pParamCOOK->Regions[1],   DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 10, pParamCOOK->cplStart[1],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 11, pParamCOOK->cplQbits[1],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 12, pParamCOOK->FrameSize[1], DSP_MEM_TYPE_DM);

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 13, pParamCOOK->Channels[2],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 14, pParamCOOK->Regions[2],   DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 15, pParamCOOK->cplStart[2],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 16, pParamCOOK->cplQbits[2],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 17, pParamCOOK->FrameSize[2], DSP_MEM_TYPE_DM);

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 18, pParamCOOK->Channels[3],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 19, pParamCOOK->Regions[3],   DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 20, pParamCOOK->cplStart[3],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 21, pParamCOOK->cplQbits[3],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 22, pParamCOOK->FrameSize[3], DSP_MEM_TYPE_DM);

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 23, pParamCOOK->Channels[4],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 24, pParamCOOK->Regions[4],   DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 25, pParamCOOK->cplStart[4],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 26, pParamCOOK->cplQbits[4],  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 27, pParamCOOK->FrameSize[4], DSP_MEM_TYPE_DM);

    return TRUE;
}

MS_BOOL HAL_MAD_WMA_SetASFParm2( AUDIO_DEC_ID DecId, Audio_ASF_Param * pParam )
{
        DecId = DecId;

        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 0,  pParam->u32Version,          DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 1,  pParam->u32Channels,         DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 2,  pParam->u32SampleRate,       DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 3,  pParam->u32ByteRate,         DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 4,  pParam->u32BlockAlign,       DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 5,  pParam->u32Encopt,           DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 6,  pParam->u32ParsingByApp,     DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 7,  pParam->u32BitsPerSample,    DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 8,  pParam->u32ChannelMask,      DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 9,  pParam->u32DrcParamExist,    DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 10, pParam->u32DrcRmsAmpRef,     DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 11, pParam->u32DrcRmsAmpTarget,  DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 12, pParam->u32DrcPeakAmpRef,    DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 13, pParam->u32DrcPeakAmpTarget, DSP_MEM_TYPE_DM);
        HAL_MAD2_Write_DSP_sram(REG_DEC2_PARAM_BASE + 14, pParam->u32MaxPacketSize,    DSP_MEM_TYPE_DM);

    return TRUE;
}

MS_BOOL HAL_MAD_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param )
{
    MS_BOOL  bRet = TRUE;

    HALMAD_CHECK_SHM_INIT;

    switch ( paramType )
    {
        case Audio_ParamType_setAudioDecoder:
        {
            En_DVB_decSystemType eAudioFormat;
            switch ( (MMA_AudioType) Param )
            {
                case Audio_DEC_PCM:
                case Audio_DEC_XPCM:        eAudioFormat = MSAPI_AUD_DVB_XPCM;        break;
                case Audio_DEC_MPEG:        eAudioFormat = MSAPI_AUD_DVB_MPEG;        break;
                case Audio_DEC_MP3:         eAudioFormat = MSAPI_AUD_DVB_MP3;         break;
                case Audio_DEC_AC3:         eAudioFormat = MSAPI_AUD_DVB_AC3;         break;
                case Audio_DEC_AC3P:        eAudioFormat = MSAPI_AUD_DVB_AC3P;        break;
                case Audio_DEC_AAC_LC:
                case Audio_DEC_HEAAC_V1:
                case Audio_DEC_HEAAC_V2:    eAudioFormat = MSAPI_AUD_DVB_AAC;         break;
                case Audio_DEC_DOLBYPULSE:  eAudioFormat = MSAPI_AUD_DVB_MS10_DDT;    break;
                case Audio_DEC_WMA:         eAudioFormat = MSAPI_AUD_DVB_WMA;         break;
                case Audio_DEC_WMAPRO:      eAudioFormat = MSAPI_AUD_DVB_WMA_PRO;     break;
                case Audio_DEC_RA8:         eAudioFormat = MSAPI_AUD_DVB_RA8LBR;      break;
                case Audio_DEC_DTS:         eAudioFormat = MSAPI_AUD_DVB_DTS;         break;
                case Audio_DEC_FLAC:        eAudioFormat = MSAPI_AUD_DVB_FLAC;        break;
                case Audio_DEC_VORBIS:      eAudioFormat = MSAPI_AUD_DVB_VORBIS;      break;
                case Audio_DEC_AMR_NB:      eAudioFormat = MSAPI_AUD_DVB_AMR_NB;      break;
                case Audio_DEC_AMR_WB:      eAudioFormat = MSAPI_AUD_DVB_AMR_WB;      break;
                case Audio_DEC_DRA:         eAudioFormat = MSAPI_AUD_DVB_DRA;         break;
                case Audio_DEC_DTSLBR:      eAudioFormat = MSAPI_AUD_DVB_DTSLBR;      break;
                case Audio_DEC_KTV:
                case Audio_DEC_KTV2:
                default:                    eAudioFormat = MSAPI_AUD_DVB_INVALID;     break;
            }
            bRet = HAL_AUDIO_SetSystem2(DecId, eAudioFormat);
            if ( bRet == TRUE )
            {
                g_AudioVars2->AudioDecStatus[DecId].bConnect= TRUE;
                g_AudioVars2->AudioDecStatus[DecId].eSourceType = E_AUDIO_INFO_MM_IN;
                g_AudioVars2->AudioDecStatus[DecId].eAudFormat = (En_DVB_decSystemType) eAudioFormat;
            }
            break;
        }

        case Audio_ParamType_playControl:
        {
            {
                switch ( (MMA_AUDIO_CONTROL) Param )
                {
                    case MMA_STOP:
                        if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                            (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                        {
                            HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_STOP);
                        }
                        g_AudioVars2->u16DecodeFrmCnt[DecId] = 0;
                        g_AudioVars2->g_bSePlayFileFlag = FALSE;
                        break;

                    case MMA_PLAY:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAY);
                        break;

                    case MMA_PLAY_FILETSP:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFILETSP);
                        break;

                    case MMA_RESYNC:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_RESYNC);
                        break;

                    case MMA_PLAY_FILE:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFILE);
                        break;

                    case MMA_BROWSE:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_STARTBROWSE);
                        break;

                    case MMA_PAUSE:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PAUSE);
                        break;

                    case MMA_PLAY_FRAME:
                        HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFRAME);
                        break;

                    case MMA_PLAY_FRAME_GS:
                        if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                            (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                        {
                            HAL_MAD2_SetDecCmd2(AU_DVB_DECCMD_PLAYFRAME_GS);
                        }
                        break;

                    default:
                        break;
                }
            }

            break;
        }

        case Audio_ParamType_SoundMode:                     //HAL_MAD_SetSoundMode
        {
            HAL_AUDIO_SeWriteMaskByte(REG_MB_MODE_SELECT, 0x03, (AU_DVB_MODE) Param);
            break;
        }

        case Audio_ParamType_PTS_info:
        {
            HAL_AUDIO_PTS_info(Param);
            break;
        }

        case Audio_ParamType_syncSTC:
        {
            if ( Param )
            {
                HAL_AUDIO_DecWriteMaskByte(0x2D8E, 0x10, 0x10);
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(0x2D8E, 0x10, 0x00);
            }
            break;
        }

        case Audio_ParamType_acMode:
        {
            // DD, DDP, HEAAC
            switch ( (AUD_Dolby_SOUNDMODE) Param )
            {
                case AUD_Dolby_SOUNDMODE_LR:     Param = 0x0;  break;
                case AUD_Dolby_SOUNDMODE_LL:     Param = 0x4;  break;
                case AUD_Dolby_SOUNDMODE_RR:     Param = 0x8;  break;
                default:                         Param = 0x0;  break;
            }
            HAL_AUDIO_WriteMaskReg(REG_MB_DEC_CTRL, 0x000C, Param);
            break;
        }

        case Audio_ParamType_reqDataSize:
        {
            if ( (HAL_AUDIO_ReadByte(REG_SE_DECODE_CMD) & 0x1F) != 0 )
            {
                break;
            }

            switch ( (FILE_SIZE) Param )
            {
                case FILE_SIZE_64:      Param = 0x0004;        break;
                case FILE_SIZE_128:     Param = 0x0008;        break;
                case FILE_SIZE_256:     Param = 0x0010;        break;
                case FILE_SIZE_512:     Param = 0x0020;        break;
                case FILE_SIZE_1KB:     Param = 0x0040;        break;
                case FILE_SIZE_2KB:     Param = 0x0080;        break;
                case FILE_SIZE_4KB:     Param = 0x0100;        break;
                case FILE_SIZE_8KB:     Param = 0x0200;        break;
                case FILE_SIZE_16KB:    Param = 0x0400;        break;
                case FILE_SIZE_32KB:    Param = 0x0800;        break;
                case FILE_SIZE_64KB:    Param = 0x1000;        break;
                default:                Param = 0x0100;        break;  // default = 2KB
            }

            HAL_MAD2_Write_DSP_sram(REG_DEC2_MHEG5_SIZE, Param, DSP_MEM_TYPE_PM);
            break;
        }

        case Audio_ParamType_MM_FFx2:
        {
            if ( Param )
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x20, 0x20);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x20, 0x00);
            }
            break;
        }

        case Audio_ParamType_mute:
        {
            if ( Param )
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x40, 0x40);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x40, 0x00);
            }
            break;
        }

        case Audio_ParamType_predecode:
        {
            if ( Param )
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x80);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x00);
            }
            break;
        }

        case Audio_ParamType_esBuf_informDataRdy:
        {
            if (g_AudioVars2->g_bSePlayFileFlag == TRUE)
            {
                g_AudioVars2->g_bSePlayFileFlag = FALSE;
                HAL_AUDIO_WriteByte(REG_MB_DEC2_MM_INT_TAG, g_AudioVars2->g_u8IntTag);
                HAL_MAD2_SetPIOCmd(0xE0);
                HAL_MAD2_TriggerPIO8();
            }
            g_AudioVars2->g_u8IntTag++;
            break;
        }

        case Audio_ParamType_setWMADecParam:
        {
            bRet = HAL_MAD_WMA_SetASFParm2(DecId, (Audio_ASF_Param *) Param);
            break;
        }

        case Audio_ParamType_setCOOKDecParam:
        {
            bRet = HAL_MAD_COOK_setParam2(DecId, (Audio_COOK_Param *) Param);
            break;
        }

        case Audio_ParamType_setXPCMDecParam:
        {
            bRet = HAL_MAD_XPCM_setParam2(DecId, (Audio_XPCM_Param *) Param);
            break;
        }

        case Audio_ParamType_UNI_PCM3_Input_Addr:
        {
            HAL_AUDIO_WriteReg(REG_DEC2_UNI_PCM3_WPTR, (MS_U16)((Param - 0x300000) >> BYTES_IN_MIU_LINE_LOG2));    // transfer to line unit
            break;
        }

        case Audio_ParamType_UNI_NEED_DECODE_FRMCNT:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====NEED_DECODE_FRMCNT =0x%X==========\r\n", (unsigned int)Param);

            HAL_AUDIO_WriteMaskReg(REG_DEC2_UNI_NEED_DEC_FRMNUM, 0xFFFF, Param);
            break;


        case Audio_ParamType_UNI_ES_Wptr:
            Param = Param - ADEC__R2_DDR_SIZE - ASND__R2_DDR_SIZE;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====UNI_ES_Wptr =0x%X==========\r\n", (unsigned int)Param);
            HAL_AUDIO_WriteReg(REG_DEC2_UNI_ES_WPTR, (MS_U16)(Param >> BYTES_IN_MIU_LINE_LOG2));    // transfer to line unit
            break;

        case Audio_ParamType_UNI_PCMOutCnt:
            uniPlay_ackCnt++;
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====Trigger decInt=0x%X\r\n", (unsigned int)uniPlay_ackCnt);
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====UNI_PCMOutCnt =0x%X==========\r\n", (unsigned int)Param);
            HAL_AUDIO_WriteReg(REG_DEC2_UNI_PCM_OUTCNT, (MS_U16)(Param >> BYTES_IN_MIU_LINE_LOG2));  // transfer to line unit
            HAL_AUDIO_SendIntrupt(DSP_SE, 0xE3);
            break;


        case Audio_ParamType_AllocDecoder:
        {
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====DecID:%X  Audio_ParamType_AllocDecoder =0x%X==========\r\n", (unsigned int)DecId, (unsigned int)Param);
            MMA_AudioType eAudioType = (MMA_AudioType) Param;
            MMA_AudioType eAudioCaps;

            OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

            if ( g_AudioVars2->AudioDecStatus[DecId].bConnect == TRUE )
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d in Used !!\r\n", __FUNCTION__, DecId);
                bRet = FALSE;

                OS_RELEASE_MUTEX(_s32MutexLoadCode);

                break;
            }

            eAudioCaps = _GetAudioCaps(DecId);

            if ( (eAudioCaps & eAudioType) == eAudioType )
            {
                En_DVB_decSystemType eAudioFormat;
                switch ( eAudioType )
                {
                    case Audio_DEC_PCM:
                    case Audio_DEC_XPCM:        eAudioFormat = MSAPI_AUD_DVB_XPCM;        break;
                    case Audio_DEC_MPEG:        eAudioFormat = MSAPI_AUD_DVB_MPEG;        break;
                    case Audio_DEC_MP3:         eAudioFormat = MSAPI_AUD_DVB_MP3;         break;
                    case Audio_DEC_AC3:         eAudioFormat = MSAPI_AUD_DVB_AC3;         break;
                    case Audio_DEC_AC3P:        eAudioFormat = MSAPI_AUD_DVB_AC3P;        break;
                    case Audio_DEC_AAC_LC:
                    case Audio_DEC_HEAAC_V1:
                    case Audio_DEC_HEAAC_V2:    eAudioFormat = MSAPI_AUD_DVB_AAC;         break;
                    case Audio_DEC_DOLBYPULSE:  eAudioFormat = MSAPI_AUD_DVB_MS10_DDT;    break;
                    case Audio_DEC_WMA:         eAudioFormat = MSAPI_AUD_DVB_WMA;         break;
                    case Audio_DEC_WMAPRO:      eAudioFormat = MSAPI_AUD_DVB_WMA_PRO;     break;
                    case Audio_DEC_RA8:         eAudioFormat = MSAPI_AUD_DVB_RA8LBR;      break;
                    case Audio_DEC_DTS:         eAudioFormat = MSAPI_AUD_DVB_DTS;         break;
                    case Audio_DEC_FLAC:        eAudioFormat = MSAPI_AUD_DVB_FLAC;        break;
                    case Audio_DEC_VORBIS:      eAudioFormat = MSAPI_AUD_DVB_VORBIS;      break;
                    case Audio_DEC_AMR_NB:      eAudioFormat = MSAPI_AUD_DVB_AMR_NB;      break;
                    case Audio_DEC_AMR_WB:      eAudioFormat = MSAPI_AUD_DVB_AMR_WB;      break;
                    case Audio_DEC_DRA:         eAudioFormat = MSAPI_AUD_DVB_DRA;         break;
                    case Audio_DEC_DTSLBR:      eAudioFormat = MSAPI_AUD_DVB_DTSLBR;      break;
                    case Audio_DEC_KTV:
                    case Audio_DEC_KTV2:
                    default:                    eAudioFormat = MSAPI_AUD_DVB_INVALID;     break;
                }
                bRet = HAL_AUDIO_SetSystem2(DecId, eAudioFormat);
                if ( bRet == TRUE )
                {
                    g_AudioVars2->AudioDecStatus[DecId].bConnect = TRUE;
                    g_AudioVars2->AudioDecStatus[DecId].eSourceType = E_AUDIO_INFO_MM_IN;
                    g_AudioVars2->AudioDecStatus[DecId].eAudFormat =(En_DVB_decSystemType) eAudioFormat;

                     /* ADC Mic input is also from ch5, so not to change ch5 path here in KTV input source mode */
                    if(g_AudioVars2->eAudioSource != E_AUDIO_INFO_KTV_IN)
                    {
                        /* expect used this path is OMX MM, so need to setup input source path to 0x81 */
                        HAL_AUDIO_WriteMaskByte(0x2C65, 0x8F, 0x81);    // to avoid ch5 mixed with OMX
                    }
                }
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d Audio Type (0x%08X) not Support !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eAudFormat );
                bRet = FALSE;
            }

            OS_RELEASE_MUTEX(_s32MutexLoadCode);

            break;
        }

        case Audio_ParamType_FreeDecoder:

            OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

            if ( g_AudioVars2->AudioDecStatus[DecId].bConnect == TRUE )
            {
                if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                    (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                {

                    memset(&g_AudioVars2->AudioDecStatus[DecId],  0, sizeof(AudioDecStatus_t));
                    if ( DecId == AU_DEC_ID1 )
                    {
                        g_AudioVars2->g_u8DspCodeType = MSAPI_AUD_DVB_NONE;
                    }
                    else if ( DecId == AU_DEC_ID3 )
                    {
                        g_AudioVars2->g_u8Dsp2CodeType = MSAPI_AUD_DVB_NONE;
                    }
                }
                else
                {
                    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d NOT used by ANDROID MM, used by %d !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eSourceType);
                }
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d NOT in USED !!\r\n", __FUNCTION__, DecId);
                bRet = FALSE;
            }

            OS_RELEASE_MUTEX(_s32MutexLoadCode);

            break;

        case Audio_ParamType_omx_spdif_mode:
            {
                MS_U32 u32SpdifParam;
                Audio_OMX_SPDIF_Param eMode;
                u32SpdifParam = HAL_MAD2_Read_DSP_sram(REG_DEC2_OMX_SPDIF_PARAM, DSP_MEM_TYPE_DM);
                eMode = (Audio_OMX_SPDIF_Param) Param;
                if ( eMode == Audio_OMX_SPDIF_ON )
                {
                    u32SpdifParam |= 0x00000001;
                }
                else
                {
                    u32SpdifParam &= 0xFFFFFFFE;
                }
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d SpdifParam 0x%08X !!\r\n", __FUNCTION__, DecId, u32SpdifParam);
                if ( (bRet = HAL_MAD2_Write_DSP_sram(REG_DEC2_OMX_SPDIF_PARAM, u32SpdifParam, DSP_MEM_TYPE_DM)) != TRUE )
                {
                    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d Fail to switch SPDIF mode to %s !!\r\n", __FUNCTION__, DecId, (eMode == Audio_OMX_SPDIF_ON ? "ON" : "OFF"));
                }
            }
            break;

        case Audio_ParamType_omx_spdif_play:
            {
                MS_U32 u32SpdifParam;
                Audio_OMX_SPDIF_Param eMode;

                u32SpdifParam = HAL_MAD2_Read_DSP_sram(REG_DEC2_OMX_SPDIF_PARAM, DSP_MEM_TYPE_DM);
                eMode = (Audio_OMX_SPDIF_Param) Param;
                if ( eMode == Audio_OMX_SPDIF_PLAY )
                {
                    u32SpdifParam |= 0x00000002;
                }
                else
                {
                    u32SpdifParam &= 0xFFFFFFFD;
                }
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d SpdifParam 0x%08X !!\r\n", __FUNCTION__, DecId, u32SpdifParam);
                if ( (bRet = HAL_MAD2_Write_DSP_sram(REG_DEC2_OMX_SPDIF_PARAM, u32SpdifParam, DSP_MEM_TYPE_DM)) != TRUE )
                {
                    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d Fail to switch SPDIF to %s !!\r\n", __FUNCTION__, DecId, (eMode == Audio_OMX_SPDIF_PLAY ? "PLAY" : "STOP"));
                }
            }
            break;

        case Audio_ParamType_omx_spdif_pcmLevel:
             if ( (bRet = HAL_MAD2_Write_DSP_sram(REG_DEC2_OMX_PCM_DIFF, Param, DSP_MEM_TYPE_DM)) != TRUE )
             {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: DecId %d Fail to setup PCM Level !!\r\n", __FUNCTION__, DecId);
             }
            break;

        /* not supported cmd */
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: infoType[%x] not Supported\n", __FUNCTION__, paramType);
            bRet = FALSE;
            break;
    }

    return bRet;
}

MS_BOOL HAL_MAD_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    MS_BOOL result = TRUE;

    HALMAD_CHECK_SHM_INIT;

    switch ( infoType )
    {
        case Audio_infoType_capability:
        {
            MMA_AudioType * pA_Type = (MMA_AudioType *) pInfo;
            *pA_Type = _GetAudioCaps(DecId);
            break;
        }

        case Audio_infoType_DecStatus:                     //no change
        {
            MS_U8 * pResult = (MS_U8 *) pInfo;
            {
                if ( HAL_AUDIO_SeReadByte(REG_MB_DEC2_ID_STATUS) & 0x3 )
                {
                    *pResult = 1;
                }
                else
                {
                    *pResult = 0;
                }
            }

            break;
        }

        case Audio_infoType_AudioSyncStatus:
        {
            MS_U8 * pResult = (MS_U8 *) pInfo;
            {
                if ( (HAL_AUDIO_ReadByte(REG_SE_DECODE_CMD) & 0x20) == 0 )
                {
                    *pResult = 1;  // AV-sync
                }
                else
                {
                    *pResult = 0;  // Free-run
                }
            }

            break;
        }

        case Audio_infoType_SampleRate:                    //HAL_MAD_GetMadSamplingRate
        {
            MS_U16 fs_type = 0xFFFF;
            AUDIO_FS_TYPE * pFS_Type = (AUDIO_FS_TYPE *) pInfo;

            fs_type = HAL_MAD2_Read_DSP_sram(REG_DEC2_SampleRate, DSP_MEM_TYPE_PM);

            *pFS_Type = AUDIO_FS_NONE;

            switch ( fs_type )
            {
                case 8000:   *pFS_Type = AUDIO_FS_8KHZ;   break;
                case 11025:  *pFS_Type = AUDIO_FS_11KHZ;  break;
                case 12000:  *pFS_Type = AUDIO_FS_12KHZ;  break;
                case 16000:  *pFS_Type = AUDIO_FS_16KHZ;  break;
                case 22050:  *pFS_Type = AUDIO_FS_22KHZ;  break;
                case 24000:  *pFS_Type = AUDIO_FS_24KHZ;  break;
                case 32000:  *pFS_Type = AUDIO_FS_32KHZ;  break;
                case 44100:  *pFS_Type = AUDIO_FS_44KHZ;  break;
                case 48000:  *pFS_Type = AUDIO_FS_48KHZ;  break;
                default:     *pFS_Type = AUDIO_FS_NONE;   break;
            }

            break;
        }

        case Audio_infoType_SoundMode:
        {
            MS_U16 * pRet = (MS_U16 *) pInfo;
            *pRet = HAL_AUDIO_ReadReg(REG_MB_MODE_SELECT) & 0x3;

            break;
        }

        case Audio_infoType_errFrmCnt:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_MAD2_Read_DSP_sram(REG_DEC2_ErrorCount, DSP_MEM_TYPE_PM);

            break;
        }

        case Audio_infoType_1ms_timeStamp:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (HAL_AUDIO_ReadReg(REG_DEC2_TIME_STAMP_SEC) * 1000) + ((HAL_AUDIO_ReadReg(REG_DEC2_TIME_STAMP_MS) & 0x00FF) << 2);
            break;
        }

        case Audio_infoType_33bit_PTS:
        {
            MS_U64 * pRet = (MS_U64 *) pInfo;
            *pRet   = HAL_MAD2_Read_DSP_sram(REG_DEC2_AUDIO_PTS_H, DSP_MEM_TYPE_PM);
            *pRet <<= 24;
            *pRet  += HAL_MAD2_Read_DSP_sram(REG_DEC2_AUDIO_PTS_L, DSP_MEM_TYPE_PM);
            break;
        }

        case Audio_infoType_BufferAddr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_ADDR) << BYTES_IN_MIU_LINE_LOG2) + OFFSET_ES1_DRAM_ADDR;
            break;
        }

        case Audio_infoType_BufferSize:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_SIZE) << BYTES_IN_MIU_LINE_LOG2);

            break;
        }

        case Audio_infoType_esBuf_req:
        {
            MS_BOOL bFlag = FALSE;

            if ( DecId == AU_DEC_ID3 )
            {
                bFlag = g_AudioVars2->g_bSePlayFileFlag;
            }

            if ( bFlag )
            {
                Audio_MM_Data_Request * pDataReq = (Audio_MM_Data_Request *) pInfo;
                {
                    pDataReq->U32WrtAddr  = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_ADDR) << BYTES_IN_MIU_LINE_LOG2) + OFFSET_ES1_DRAM_ADDR;
                    pDataReq->U32WrtBytes = ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_LINE_BUF_SIZE) << BYTES_IN_MIU_LINE_LOG2);
                }

                result = TRUE;
            }
            else
            {
                result = FALSE;
            }

            break;
        }

        case Audio_infoType_esBuf_currLevel:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_DDR_ES_BUF_SIZE) << BYTES_IN_MIU_LINE_LOG2);
            break;
        }

        case Audio_infoType_pcmBuf_currLevel:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_RESIDUAL_PCM) << BYTES_IN_MIU_LINE_LOG2);
            break;
        }

        case Audio_infoType_get_audio_filter:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (MS_U32) FILTER_TYPE_AUDIO_2;

            break;
        }

        case Audio_infoType_audio_unsupported:
        {
            MS_U8 * pRet = (MS_U8 *) pInfo;
            {
                if ( HAL_AUDIO_ReadByte(S2M_MBOX_INTR_CMDTYPE + 1) == 0x09 )
                {
                    *pRet = 1;
                }
                else
                {
                    *pRet = 0;
                }
            }

            break;
        }

        case Audio_infoType_UNI_PCM1_Info:
        {
           Audio_MM_PCM_Info * pPCMInfo = (Audio_MM_PCM_Info *) pInfo;
        #if(OMX_AUDIO_DEBUG)
            MS_U32 tmp_H, tmp_L;
            MS_U32 ES_Lvl, PCM_Lvl;
            static MS_U32 ReadCount = 0 ;
            MS_U32 sysTime;

            ReadCount ++;
            ES_Lvl = (HAL_AUDIO_ReadReg(S2M_MBOX_ES_MEMCNT));
            PCM_Lvl = (HAL_AUDIO_ReadReg(S2M_MBOX_PCM_MEMCNT));
        #endif

                if ( HAL_AUDIO_GetUniDecodeFrameCnt(DecId) != g_AudioVars2->u16DecodeFrmCnt[DecId] )
                {
                    MS_U32 addr = 0;
                    g_AudioVars2->u16DecodeFrmCnt[DecId] = HAL_AUDIO_GetUniDecodeFrameCnt(DecId);

                        addr = (MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_UNI_PCM_WPTR) + DSP2_PCM1_DRAM_BASE;
                        addr <<= BYTES_IN_MIU_LINE_LOG2;
                        pPCMInfo->U32WrtAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + addr;
                        pPCMInfo->U32WrtBytes = (((MS_U32)HAL_AUDIO_ReadReg(REG_DEC2_UNI_PCM_OUTSIZE)) << BYTES_IN_MIU_LINE_LOG2);
                   #if(OMX_AUDIO_DEBUG)
                        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====UNI_PCM1_Info  WriteAddress =0x%X==========\r\n", pPCMInfo->U32WrtAddr);
                        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n=====UNI_PCM1_Info  WriteByte =0x%X==========\r\n", pPCMInfo->U32WrtBytes);
                        ReadCount = 10;
                   #endif
                }
                pPCMInfo->U32DecodeFrame = g_AudioVars2->u16DecodeFrmCnt[DecId];

         #if(OMX_AUDIO_DEBUG)
            if(ReadCount%10==0)
            {
                sysTime = MsOS_GetSystemTime();
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "SysTime:[%08u]", sysTime);

                tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_AvSync+SYS_AVSYNC_ACCUM_ES_WORDCNT, DSP_MEM_TYPE_DM)&0xFFFF;
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "ES=0x%03X(accumES=0x%04X),", ES_Lvl, tmp_L);

                tmp_L = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_DecStatus+SYS_DECSTATUS_OK_CNT, DSP_MEM_TYPE_DM)&0x0000FFFF;
                tmp_H = HAL_MAD2_Read_DSP_sram(DSP2DmAddr_sys_DecStatus+SYS_DECSTATUS_ERR_CNT, DSP_MEM_TYPE_DM)&0x0000FFFF;
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "frmCnt:0x%04X,",     tmp_L);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "ErrCnt:0x%04X,",    tmp_H);

                tmp_L = HAL_MAD2_Read_DSP_sram(0x3BF1, DSP_MEM_TYPE_DM)&0x0000FFFF;
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "sysDec1_mmUni_AckInt_Cnt:0x%04X,",    tmp_L);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "sysDec1_mmUni_totalFrm_Cnt:0x%04X",  g_AudioVars2->u16DecodeFrmCnt[DecId]);
            }
        #endif

            break;
        }

        case Audio_infoType_UNI_PCM3_Level:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_UNI_PCM3_LEVEL)) << BYTES_IN_MIU_LINE_LOG2);
            break;
        }

        case Audio_infoType_UNI_ES_Level:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = ((MS_U32) HAL_AUDIO_ReadReg(REG_DEC2_UNI_ES_MEMCNT) << BYTES_IN_MIU_LINE_LOG2);
            break;
        }

        case Audio_infoType_UNI_ES_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR);
            break;
        }

        case Audio_infoType_UNI_ES_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = ES1_DRAM_SIZE_BYTE;
            break;
        }

        case Audio_infoType_UNI_PCM_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR);
            break;
        }

        case Audio_infoType_UNI_PCM_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = PCM1_DRAM_SIZE_BYTE;
            break;
        }

        case Audio_infoType_MadBaseAddr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
            break;
        }

        case Audio_infoType_hashkey:
        {
            Audio_Hashkey_Info * pHashkeyInfo = (Audio_Hashkey_Info *) pInfo;
            MS_U32 audio_ipauth = HAL_MAD2_Read_DSP_sram(REG_DEC2_AUDIO_IPAUTH, DSP_MEM_TYPE_PM);

            pHashkeyInfo->U32IsHashkeySupported = TRUE;

            switch ( pHashkeyInfo->U32Name )
            {
                case Audio_DEC_AC3:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_AC3)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS11)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_LC_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_AC3P:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_AC3P)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS11)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_LC_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_AAC_LC:
                case Audio_DEC_HEAAC_V1:
                case Audio_DEC_HEAAC_V2:
                case Audio_DEC_DOLBYPULSE:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS10)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_GAAC)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DOLBY_MS11)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_C_PROFILE)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_MS12_B_PROFILE))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_WMA:
                case Audio_DEC_WMAPRO:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_WMA))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DTS:
                {
                	if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DTS)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DTS_XLL))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DTSLBR:
                {
                	if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DTSLBR)
                        && (audio_ipauth & AUDIO_HASHKEY_DISABLE_DTS_XLL))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DRA:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_DRA))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_RA8:
                {
                    if ((audio_ipauth & AUDIO_HASHKEY_DISABLE_COOK))
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                default:
                    break;
            }

            break;
        }

        case Audio_infoType_AccumEsCnt:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            *pRet = HAL_MAD2_Read_DSP_sram(DSP2_DEC1_ACCUM_ES_CNT, DSP_MEM_TYPE_DM);
            break;
        }

        /* not support cmd */
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: cmd not Supported\n", __FUNCTION__ );
            result = FALSE;
            break;
    }

    return result;
}

//====== STAOS PIP END 2012/02/01 ========