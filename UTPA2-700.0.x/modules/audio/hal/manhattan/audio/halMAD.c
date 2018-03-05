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
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
// Internal Definition
#include "regCHIP.h"
#include "drvAUDIO.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSOUND.h"
#include "halSIF.h"
#include "r2_shm_comm.h"
#include "decR2_shm.h"
#if ASND_R2_SUPPORT
#include "sndR2_shm.h"
#endif
#include "halAUR2.h"
#include "ddr_config.h"
#include "audio_comm2.h"

#define OMX_AUDIO_DEBUG  0

#if (defined ANDROID)

  #include <sys/mman.h>
  #include <cutils/ashmem.h>
  #include <cutils/log.h>

  #ifndef LOGI // android 4.1 rename LOGx to ALOGx
    #define LOGI ALOGI
  #endif

  #ifndef LOGE // android 4.1 rename LOGx to ALOGx
    #define LOGE ALOGE
  #endif

  #if(OMX_AUDIO_DEBUG)
    #define HALMAD_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
  #else
    #define HALMAD_PRINT(fmt, args...)
  #endif

  #define HALMAD_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)

#else
    #ifdef CONFIG_MBOOT //mboot Speed up
        #define HALMAD_PRINT(fmt, args...)
        #define HALMAD_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #else
        #if(OMX_AUDIO_DEBUG)
            #define HALMAD_PRINT(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
        #else
            #define HALMAD_PRINT(fmt, args...)
        #endif

        #define HALMAD_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #endif
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_MAD(msg) //msg
#define AUD_ERRMSG(msg)  MS_CRITICAL_MSG(msg)
#define AUD_DBGMSG(msg)  MS_DEBUG_MSG(msg)

#define HALMAD_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            HALMAD_ERROR("%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            HALMAD_ERROR("%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
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

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
//static MS_BOOL EncodeInProcess;
//#define ENC_BUF_SIZE 6;
extern AUDIO_INPUT_TYPE  g_audioSrcType;
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
MS_BOOL FLAG_HDMI_AC3_IN_SE = TRUE;
MS_BOOL g_DDPBypassenable_1 = TRUE;
MS_BOOL g_DDPBypassenable_2 = TRUE;
static MS_U32 g_ReadTargetAddr=0x2D00;
static MS_BOOL g_bR2Adec1AD = FALSE;
static MS_BOOL g_bR2Adec2AD = FALSE;

#define LAYER_MIN   1
#define LAYER_MAX   3
#define MPEG_BITRATE_TABLE_ROW_NUM  6
#define MPEG_BITRATE_TABLE_COL_NUM  16

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

MS_U32  u32SeProcessFrameSmpUnit = SE_PROCESS_FRAME_SMP_UNIT;  // For Bifrost audio MT. Default : SE_PROCESS_FRAME_SMP_UNIT. For some advance sound effect ip like SRS TheaterSound, it is 256 because it is 256 samples per frame processing.

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MMA_AudioType _GetAudioCaps(AUDIO_DEC_ID eDecId)
{
    MMA_AudioType eAudioCaps = 0;

    if ( eDecId == AU_DEC_ID1 )
    {
        eAudioCaps = Audio_DEC_PCM        | Audio_DEC_MPEG     |
                     Audio_DEC_MP3        | Audio_DEC_AC3      |
                     Audio_DEC_AC3P       | Audio_DEC_AAC_LC   |
                     Audio_DEC_HEAAC_V1   | Audio_DEC_HEAAC_V2 |
                     Audio_DEC_DOLBYPULSE | Audio_DEC_RA8      |
                     Audio_DEC_WMA        | Audio_DEC_WMAPRO   |
                     Audio_DEC_XPCM       | Audio_DEC_AMR_NB   |
                     Audio_DEC_AMR_WB     | Audio_DEC_VORBIS   |
                     Audio_DEC_DTS        | Audio_DEC_FLAC     |
                     Audio_DEC_DTSLBR     | Audio_DEC_DTS_HD_ADO_R2 |
                     Audio_DEC_DRA;
    }
    else if ( eDecId == AU_DEC_ID2 )
    {
        eAudioCaps = 0;
    }
    else if ( eDecId == AU_DEC_ID3 )
    {
        eAudioCaps = Audio_DEC_PCM        | Audio_DEC_MPEG     |
                     Audio_DEC_MP3        | Audio_DEC_AC3      |
                     Audio_DEC_AC3P       | Audio_DEC_AAC_LC   |
                     Audio_DEC_HEAAC_V1   | Audio_DEC_HEAAC_V2 |
                     Audio_DEC_DOLBYPULSE | Audio_DEC_RA8      |
                     Audio_DEC_WMA        | Audio_DEC_WMAPRO   |
                     Audio_DEC_XPCM       | Audio_DEC_AMR_NB   |
                     Audio_DEC_AMR_WB     | Audio_DEC_VORBIS   |
                     Audio_DEC_DTS        | Audio_DEC_FLAC     |
                     Audio_DEC_DTSLBR     | Audio_DEC_DTS_HD_ADO_R2 |
                     Audio_DEC_DRA;
    }

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
  // Remove all memory Info(SE-DSP & R2) to SetMemInfo2
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
MS_S32 uniPlay_ackCnt[2] = {0};
void HAL_MAD_SetDecCmd(AU_DVB_DECCMD u8DecCmd)
{
    MS_U8   FREERUN_STATE = 0;
    MS_U16  u16_timeout = 0;
    MS_U32  u32_r2_cmd = 0, u32_driver_cmd = 0;
    DBG_MAD(printf("HAL_MAD_setDecCmd:(%x)\r\n",u8DecCmd));

    if (u8DecCmd == AU_DVB_DECCMD_AVSYNC)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, 0x00);  // bit[15] = 0
    }
    else if (u8DecCmd == AU_DVB_DECCMD_FREERUN)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, 0x80); // bit[15] = 1, AU_DEC_R2_CMD_FREERUN
    }
    else
    {

        switch(u8DecCmd)
        {
            case AU_DVB_DECCMD_STOP:
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0, 0);           //Set ES write pointer  to start point.
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0xEF, AU_DEC_R2_CMD_STOP);
                uniPlay_ackCnt[0] = 0;
                break;

            case AU_DVB_DECCMD_PLAY:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_PLAY);
                break;

            case AU_DVB_DECCMD_PLAYFILETSP:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_PLAYFILETSP);
                break;

            case AU_DVB_DECCMD_PLAYFRAME_GS :
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_PLAYUNI);
                break;

            case AU_DVB_DECCMD_PLAYFILE:
            case AU_DVB_DECCMD_PLAYFILE2_PTS:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x6F, AU_DEC_R2_CMD_PLAYFILE);
                break;

            case AU_DVB_DECCMD_PLAY_AD:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x10, AU_DEC_R2_CMD_PLAY_AD);
                break;

            case AU_DVB_DECCMD_STOP_AD:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x10, 0x00);
                break;

            case AU_DVB_DECCMD_PAUSE:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x40, AU_DEC_R2_CMD_PAUSE);
                break;

            default:
                break;
        }

        FREERUN_STATE = HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD)&0x80; // check free-run
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, FREERUN_STATE);

        /* handshake scheme between R2 and Driver */
        if((u8DecCmd == AU_DVB_DECCMD_PLAY)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILETSP)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE)
        ||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_STAGE)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_GS)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE2_PTS))
        {
            u32_driver_cmd = R2_DECCMD_PLAY;  //DEC_PLAYSTATE_PLAY
        }
        else if(u8DecCmd == AU_DVB_DECCMD_STOP)
        {
            u32_driver_cmd = R2_DECCMD_STOP;  //DEC_PLAYSTATE_STOP
        }
        else
        {
            return;
        }

        /* wait command passing to r2 */
        while(u16_timeout++ < 100)
        {
            u32_r2_cmd = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC1)&0x00FFFFFF;
            AUDIO_DELAY1MS(1);
            if (u32_r2_cmd == u32_driver_cmd)
                break;
        }

        if (u16_timeout >= 100 )
        {
            HALMAD_ERROR(" ===  HAL_MAD_SetDecCmd %d fail===\n",u8DecCmd);
        }
    }
}

void HAL_MAD_SetDecCmd2(AUDIO_DEC_ID DecId, AU_DVB_DECCMD u8DecCmd)
{
    if (AUDIO_DSP_ID_DEC == g_AudioVars2->AudioDecStatus[DecId].eDSPId)
    {
        // ADEC DSP
        HAL_MAD_SetDecCmd(u8DecCmd);
    }
    else if (AUDIO_DSP_ID_SND == g_AudioVars2->AudioDecStatus[DecId].eDSPId)
    {
        // ASND DSP
        HAL_MAD_SetDSP2DecCmd(u8DecCmd);
    }
    else
    {
        // AU_DEC_ID2 only for ATV SIF
        // For other case should not use AU_DEC_ID2
        return;
    }

    // Check AD play command
    if((AU_DVB_DECCMD_PLAY_AD == u8DecCmd) || (AU_DVB_DECCMD_STOP_AD == u8DecCmd))
    {
        if(g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_DTV_IN)
        {
            switch(DecId)
            {
                case AU_DEC_ID1:
                case AU_DEC_ID2:
                {
                    switch(u8DecCmd)
                    {
                        case AU_DVB_DECCMD_PLAY_AD:
                        {
                            g_bR2Adec1AD = TRUE;
                        }
                        break;
                        case AU_DVB_DECCMD_STOP_AD:
                        {
                            g_bR2Adec1AD = FALSE;
                        }
                        break;
                        default:
                            break;
                    }
                }
                break;
                case AU_DEC_ID3:
                {
                    switch(u8DecCmd)
                    {
                        case AU_DVB_DECCMD_PLAY_AD:
                        {
                            g_bR2Adec2AD = TRUE;
                        }
                        break;
                        case AU_DVB_DECCMD_STOP_AD:
                        {
                            g_bR2Adec2AD = FALSE;
                        }
                        break;
                        default:
                            break;
                    }
                }
                break;
                default:
                    break;
            }
        }
    }

    switch(DecId)
    {
        case AU_DEC_ID1:
        case AU_DEC_ID2:
        {
            switch(g_bR2Adec1AD)
            {
                case TRUE:
                {
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY_AD);
                }
                break;
                case FALSE:
                {
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP_AD);
                }
                break;
                default:
                    break;
            }
        }
        break;
        case AU_DEC_ID3:
        {
            switch(g_bR2Adec2AD)
            {
                case TRUE:
                {
                    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAY_AD);
                }
                break;
                case FALSE:
                {
                    HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP_AD);
                }
                break;
                default:
                    break;
            }
        }
        break;
        default:
            break;
    }
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
    MS_U8   FREERUN_STATE = 0;
    MS_U16  u16_timeout = 0;
    MS_U32  u32_r2_cmd = 0, u32_driver_cmd = 0;
    DBG_MAD(printf("HAL_MAD_SetDSP2DecCmd:(%x)\r\n", u8DecCmd));

    if (u8DecCmd == AU_DVB_DECCMD_AVSYNC)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x80, 0x00);  // bit[15] = 0
    }
    else if (u8DecCmd == AU_DVB_DECCMD_FREERUN)
    {
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x80, 0x80); // bit[15] = 1, AU_DEC_R2_CMD_FREERUN
    }
    else
    {

        switch(u8DecCmd)
        {
            case AU_DVB_DECCMD_STOP:
                uniPlay_ackCnt[1] = 0;
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC2, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0, 0);           //Set ES write pointer  to start point.
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0xFF, AU_DEC_R2_CMD_STOP);           break;

            case AU_DVB_DECCMD_PLAY:             HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_PLAY);          break;

            case AU_DVB_DECCMD_PLAYFILETSP:  HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_PLAYFILETSP);          break;

            case AU_DVB_DECCMD_PLAYFRAME_GS :  HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_PLAYUNI);          break;

            case AU_DVB_DECCMD_PLAYFILE:
                HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x7F, AU_DEC_R2_CMD_PLAYFILE);        break;

            case AU_DVB_DECCMD_PLAY_AD:           HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x10, AU_DEC_R2_CMD_PLAY_AD);          break;

            case AU_DVB_DECCMD_STOP_AD:           HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x10, 0x00);          break;

            case AU_DVB_DECCMD_PAUSE:           HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x40, AU_DEC_R2_CMD_PAUSE);          break;
            default:                                              break;
        }

        FREERUN_STATE = HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_CMD)&0x80; // check free-run
        HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE2_CMD, 0x80, FREERUN_STATE);
        /* handshake scheme between R2 and Driver */
        if((u8DecCmd == AU_DVB_DECCMD_PLAY)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILETSP)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE)
        ||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_STAGE)||(u8DecCmd == AU_DVB_DECCMD_PLAYFRAME_GS)||(u8DecCmd == AU_DVB_DECCMD_PLAYFILE2_PTS))
        {
            u32_driver_cmd = R2_DECCMD_PLAY;  //DEC_PLAYSTATE_PLAY
        }
        else if(u8DecCmd == AU_DVB_DECCMD_STOP)
        {
            u32_driver_cmd = R2_DECCMD_STOP;  //DEC_PLAYSTATE_STOP
        }
        else if(u8DecCmd == AU_DVB_DECCMD_PAUSE)
        {
            u32_driver_cmd = R2_DECCMD_PAUSE;  //DEC_PLAYSTATE_PAUSE
        }
        else
        {
            return;
        }

        /* wait command passing to r2 */
        while(u16_timeout++ < 100)
        {
            u32_r2_cmd = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PLAY_STATE, ADEC2)&0x00FFFFFF;
            AUDIO_DELAY1MS(1);
            if (u32_r2_cmd == u32_driver_cmd)
                break;
        }

        if (u16_timeout >= 100 )
        {
            HALMAD_ERROR(" ===  HAL_MAD_SetDSP2DecCmd %d fail===\n",u8DecCmd);
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
    AU_DEC_R2_CMD   r2cmd_status = AU_DEC_R2_CMD_STOP;
    AU_DVB_DECCMD  ret_deccmd = AU_DVB_DECCMD_STOP;

    switch (AdoDecoderNo)
    {
        case DVB_Audio_Decoder1:
        {
            r2cmd_status = HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD);
            r2cmd_status &= ~AU_DEC_R2_CMD_PLAY_AD; /* Discard AU_DEC_R2_CMD_PLAY_AD */
            r2cmd_status &= ~AU_DEC_R2_CMD_FREERUN; /* Discard AU_DEC_R2_CMD_FREERUN */
            break;
        }

        case DVB_Audio_Decoder2:
        {
            r2cmd_status = HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_CMD);
            r2cmd_status = r2cmd_status & 0x7F;
            break;
        }

        default:
            break;
    }

    /*
        need to report status cmd first (stauts cmd : free-run, pause, browse)
    */

    if((r2cmd_status & AU_DEC_R2_CMD_PAUSE) == AU_DEC_R2_CMD_PAUSE) // check pause
    {
        ret_deccmd = AU_DVB_DECCMD_PAUSE;
    }
    else if((r2cmd_status & AU_DEC_R2_CMD_STARTBROWSE) == AU_DEC_R2_CMD_STARTBROWSE)   // check startbrowse
    {
        ret_deccmd = AU_DVB_DECCMD_STARTBROWSE;
    }
    else if((r2cmd_status & AU_DEC_R2_CMD_PLAY_AD) == AU_DEC_R2_CMD_PLAY_AD)    // check play AD
    {
        ret_deccmd = AU_DVB_DECCMD_PLAY_AD;
    }
    else    // others : not status cmd
    {
        switch(r2cmd_status)
        {
            case AU_DEC_R2_CMD_STOP:
                ret_deccmd = AU_DVB_DECCMD_STOP;
                break;

            case AU_DEC_R2_CMD_PLAY:
                ret_deccmd = AU_DVB_DECCMD_PLAY;
                break;

            case AU_DEC_R2_CMD_PLAYFILETSP:
                ret_deccmd = AU_DVB_DECCMD_PLAYFILETSP;
                break;

            case AU_DEC_R2_CMD_PLAYUNI:
                ret_deccmd = AU_DVB_DECCMD_PLAYFRAME_GS;
                break;

            case AU_DEC_R2_CMD_PLAYFILE:
                ret_deccmd = AU_DVB_DECCMD_PLAYFILE;
                break;

            default:
                break;
        }
    }

    return ret_deccmd;
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
    DBG_MAD(printf("HAL_MAD_setFreeRun:(%x)\r\n",u8FreeRun));
    if( u8FreeRun >= 2 ){
        DBG_MAD(printf("Invalid mode\r\n"));
        return;
    }

    HAL_AUDIO_AbsWriteMaskByte(REG_R2_DECODE1_CMD, 0x80, (u8FreeRun << 7));
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
    MS_U8   Rpt_DTVES_tmp;
    Rpt_DTVES_tmp = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1);

    return Rpt_DTVES_tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_SetStreamID_Mod()
/// @brief \b Function \b Description: This routine is used to AC3 Stream ID mod.
/// @param <IN>        \b stream_id_mod    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_MAD_SetStreamID_Mod(MS_U8 stream_id_mod)
{
    //NOT_DEFINE : not used, remove
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
    MS_U32 timestamp = 0;
    MS_U32  pts = 0;

    pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
    timestamp = pts/45;

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
    HAL_AUDIO_WriteMaskReg(REG_MB_DEC_PIO_ID,0xFF00, cmd<<8);
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
    //NOT_DEFINE : not used, remove
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
static MS_U32 prv_PTS = 0;
static MS_U32 accum_ES = 0, prv_accum_ES = 0;
MS_BOOL HAL_MAD_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{
    if ( dec_id == AU_DEC_ID1 )
    {
        audio_aes_info[0].aes_base_addr =  OFFSET_ES1_DRAM_ADDR;
        audio_aes_info[0].aes_BufSize = ES1_DRAM_SIZE;
        audio_aes_info[0].aes_end_addr = audio_aes_info[0].aes_base_addr + audio_aes_info[0].aes_BufSize;
        audio_aes_info[0].aes_write_addr = audio_aes_info[0].aes_base_addr;
        audio_aes_info[0].aes_freeSpace = audio_aes_info[0].aes_BufSize -  (BYTES_IN_MIU_LINE*2);
        audio_aes_info[0].aes_level = 0;
        audio_aes_info[0].aes_lastPTS = 0;
        audio_aes_info[0].aes_ptsCnt = 0;

        prv_PTS = 0;
        accum_ES = 0;
        prv_accum_ES = 0;

        /* Set ES write pointer to DSP */
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, dec_id, 0, 0);           //Set ES write pointer  to start point.

        DBG_MAD(printf("HAL_AUDIO_MM2_init end !!\r\n"));
        return TRUE;
    }
    else
        return FALSE;
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
        audio_aes_info[0].aes_level = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, dec_id);

        /* calculate free space for next data input */
        tmp1 = audio_aes_info[0].aes_end_addr - audio_aes_info[0].aes_write_addr;
        tmp2 = audio_aes_info[0].aes_BufSize - audio_aes_info[0].aes_level;

        if ( tmp2 <  audio_aes_info[0].aes_BufSize/2)
            tmp2 = 0;
        else
            tmp2 -= (audio_aes_info[0].aes_BufSize/2);

        if (tmp2 > (audio_aes_info[0].aes_BufSize/4))
            tmp2 = audio_aes_info[0].aes_BufSize/4;

        audio_aes_info[0].aes_freeSpace = (tmp1 > tmp2)?tmp2:tmp1;
        audio_aes_info[0].aes_read_addr = OFFSET_ES1_DRAM_ADDR + HAL_DEC_R2_Get_SHM_INFO( R2_SHM_INFO_ES_RD_PTR, ADEC1 );

        memcpy( (void *)aes_info,  (void *)&audio_aes_info[0], sizeof(AES_INFO));
        DBG_MAD(printf("[HAL_MAD_MM2_checkAesInfo] Ptr[w:%x,r:%x] LvL:%x, FreeSpace:%x, tmpFree:%x \n",
            audio_aes_info[0].aes_write_addr,
            audio_aes_info[0].aes_read_addr,
            audio_aes_info[0].aes_level,
            audio_aes_info[0].aes_BufSize - audio_aes_info[0].aes_level,
            audio_aes_info[0].aes_freeSpace));

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
    MS_U32 es_writeLineOffset;
    MS_U32 tmp1, tmp2;

    if ( dec_id != AU_DEC_ID1 )
        return FALSE;

    if (es_size == 0)
        return FALSE;

    /* es level */
    audio_aes_info[0].aes_level = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, dec_id);

    /* calculate free space for next data input */
    tmp1 = audio_aes_info[0].aes_end_addr - audio_aes_info[0].aes_write_addr;
    tmp2 = audio_aes_info[0].aes_BufSize - audio_aes_info[0].aes_level;

    audio_aes_info[0].aes_freeSpace = (tmp1 - tmp2)?tmp2:tmp1;

    if (es_size > audio_aes_info[0].aes_freeSpace)
    {
        printf("==> Error, input AES size(0x%08u) is larger than current Free space(0x%08u)\r\n", (unsigned int)es_size, (unsigned int)audio_aes_info[0].aes_freeSpace );
        return FALSE;
    }

    DBG_MAD(printf("[HAL_MAD_MM2_inputAesFinished] wPtr:%08x => ", audio_aes_info[0].aes_write_addr));

    /* update write pointer */
    audio_aes_info[0].aes_write_addr += es_size;

    if ( audio_aes_info[0].aes_write_addr >= audio_aes_info[0].aes_end_addr )
        audio_aes_info[0].aes_write_addr -= audio_aes_info[0].aes_BufSize;

#if defined (__aarch64__)
    DBG_MAD(printf("%08x, size=%08x, isPtsExist:%d, PTS=%09lx\n",
#else
    DBG_MAD(printf("%08x, size=%08x, isPtsExist:%d, PTS=%09llx\n",
#endif
        audio_aes_info[0].aes_write_addr,
        es_size,
        ptsExist,
        pts/90));

    if (ptsExist)
    {
        if (prv_PTS != 0)
        {
            DBG_MAD(printf(",PTS_diff=%08lld,accum_ES = %08lx,ES_diff= %08ld, ES PTS diff=%08ld\n",
                (pts - prv_PTS)/90,
                accum_ES,
                accum_ES - prv_accum_ES,
                (accum_ES - prv_accum_ES)*32/1536));
        }
        else
        {
            DBG_MAD(printf(", PTS_diff=%08x, accum_ES = %08x\n", 0, 0));
        }

        prv_PTS = pts;
        prv_accum_ES = accum_ES;
    }
    else
    {
        DBG_MAD(printf("\n"));
    }

    accum_ES += es_size;

    es_writeLineOffset = audio_aes_info[0].aes_write_addr - audio_aes_info[0].aes_base_addr;

    if (ptsExist)
        {
            if ((audio_aes_info[0].aes_lastPTS  == pts)&&(audio_aes_info[0].aes_ptsCnt != 0))
                printf(" ======== Error PTS is same !! =============\n");
            else
                {
                        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_INPUT_PTS, dec_id, (pts >> 1), 0);
                        audio_aes_info[0].aes_ptsCnt++;
                        audio_aes_info[0].aes_lastPTS = pts;
                        AUDIO_DELAY1US(500);
                }
        }

    /* Set ES write pointer to DSP */
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, dec_id, es_writeLineOffset, 0);
    AUDIO_DELAY1US(250);                                                                                        //Since DSP timer update ES_MEM_CNT at each 125us, need this delay to keep

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

    data = (MS_U16)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_HEADER, ADEC1);

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
    return (MS_U16)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
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
   return (MS_U16) HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
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
    return (MPEG_SOUND_MODE)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_STEREOMODE, ADEC1);
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
    AUDIO_DSP_CODE_TYPE MainDspCodeType = AU_DVB_STANDARD_INVALID;
    AUDIO_DSP_CODE_TYPE Dsp1CodeType = AU_DVB_STANDARD_INVALID;
    Digital_Out_Channel_Status_t stDigitalChannelStatusTx1, stDigitalChannelStatusTx2;
    MS_BOOL retTx1, retTx2;

    HALMAD_CHECK_SHM_INIT;

    memset(&stDigitalChannelStatusTx1, 0x00, sizeof(Digital_Out_Channel_Status_t));
    memset(&stDigitalChannelStatusTx2, 0x00, sizeof(Digital_Out_Channel_Status_t));

    eAudioSource = (MS_U8)g_AudioVars2->eAudioSource;
    Dsp1CodeType = g_AudioVars2->g_DspCodeType;

    HAL_AUDIO_SPDIF_SetMode(gSpdifOutputType, eAudioSource);

    u8SpdifMode = _HAL_AUDIO_SPDIF_GetMode();

    retTx1 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_SPDIF_OUTPUT, &stDigitalChannelStatusTx1);
    retTx2 = HAL_AUDIO_DigitalOut_GetChannelStatus(DIGITAL_HDMI_ARC_OUTPUT, &stDigitalChannelStatusTx2);
    if (retTx1 == FALSE || retTx2 == FALSE)
    {
        HALMAD_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
    }

    switch(eAudioSource)
    {
        case E_AUDIO_INFO_DTV_IN:
        case E_AUDIO_INFO_MM_IN:
        case E_AUDIO_INFO_GAME_IN:
        {
            MainDspCodeType = Dsp1CodeType;

            switch(MainDspCodeType)
            {
                case AU_DVB_STANDARD_AC3:
                case AU_DVB_STANDARD_AC3P:
                case AU_DVB_STANDARD_MS10_DDC:
                {
                    if (u8SpdifMode == SPDIF_OUT_PCM)
                    {
                        break;
                    }

                    switch(HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1))
                    {
                        case 44100:
                        {
                            /* change SPDIF Tx 1&2 channel status --> 44.1KHz */
                            stDigitalChannelStatusTx1.u8SamplingRate = 0x00;
                            stDigitalChannelStatusTx2.u8SamplingRate = 0x00;
                            break;
                        }

                        case 32000:
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

                    if (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1) == 44100)
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
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_VERSION, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_CHANNELS:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELS, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_SAMPLERATE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_SAMPLERATE, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_BYTERATE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BYTERATE, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_BLOCKALIGN:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BLOCKALIGN, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_ENCOPT:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_ENCOPT, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_PARSINGBYAPP:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_PARSINGBYAPP, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_BITS_PER_SAMPLE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BITS_PER_SAMPLE, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_CHANNELMASK:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELMASK, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_PARAM_EXIST:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PARAM_EXIST, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_RMS_AMP_REF:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_RMS_AMP_TARGET:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_PEAK_AMP_REF:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_DRC_PEAK_AMP_TARGET:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET, ADEC1, value, 0);
            break;
        case WMA_PARAMTYPE_MAX_PACKET_SIZE:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_MAX_PACKET_SIZE, ADEC1, value, 0);
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

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, ADEC1, mNumCodecs, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, ADEC1, mSamples, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, ADEC1, mSampleRate, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[0], 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[0], 0 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[1], 1 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[1], 1 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[2], 2 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[2], 2 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[3], 3 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[3], 3 );

    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, Channels[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, Regions[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, cplStart[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, cplQbits[4], 4 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, FrameSize[4], 4 );

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
    //NOT_DEFINE : not used
    return 0;
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
    //NOT_DEFINE : not used
    return 0;
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
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT, ADEC1, param1, 0);
            break;

        case Audio_Comm_infoType_Set_UNI_ES_Wptr:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, param1, 0);
            break;

        case Audio_Comm_infoType_Set_SE_Test_Init:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(REG_SOUND_MAIN_SNDEFFECT,0xFFFF, 0x00);
                HAL_AUDIO_WriteMaskReg(REG_SOUND_ADV_CFG0,0xFFFF, 0x00);
                HAL_AUDIO_WriteByte(REG_SOUND_NR_THRESHOLD, 0x00);
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_GPA_ISR_DISABLE_AND_SE_DMA_Init);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_GPA_ISR_DISABLE_AND_SE_DMA_Init);
                AUDIO_DELAY1MS(10); // need this, don't mark.
            }
            u32SeProcessFrameSmpUnit = SE_PROCESS_FRAME_SMP_UNIT;
            break;

        case Audio_Comm_infoType_Set_SE_Test_End:
            if (param1 == 0)
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_GPA_ISR_ENABLE);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_GPA_ISR_ENABLE);
                HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_SRS_SEL, 0x01, 0x00);
                HAL_AUR2_WriteMaskByte(REG_SNDR2_MBOX_BYTE_ABS3D_SEL, 0x01, 0x00);
            }
            else if (param1 == 1)
            {
                // In order not to create a new enum:
                // At Bifrost, define Audio_Comm_infoType_Set_DECODER_Test_Init as Audio_Comm_infoType_Set_SE_Test_Init.
                // And param==1 is for what's need at Audio_Comm_infoType_Set_DECODER_Test_Init.
                g_AudioVars2->g_bHashkeyFlag = FALSE;  // set g_bHashkeyFlag to FALSE to let dynamic hash key change
                {
                    extern void MDrv_AUDIO_CheckHashkey(void);
                    MDrv_AUDIO_CheckHashkey();
                }
            }
            break;

        case Audio_Comm_infoType_Set_SE_TONE_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_TONE_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_TONE_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_PEQ_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_PEQ_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_PEQ_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_GEQ_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_GEQ_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_GEQ_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_AVC_Test_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_AVC_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_AVC_TEST_BEGIN);
            }
            break;

        case Audio_Comm_infoType_Set_SE_SRS_THEATER_SOUND_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SRS_THEATER_SOUND_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SRS_THEATER_SOUND_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_SONICEMOTION_ABS3D_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SONICEMOTION_ABS3D_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SONICEMOTION_ABS3D_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_BONGIOVI_DPS_Begin:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_BONGIOVI_DPS_TEST_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_BONGIOVI_DPS_TEST_BEGIN);
            }
            u32SeProcessFrameSmpUnit = 256;
            break;

        case Audio_Comm_infoType_Set_SE_Input_Main_Buffer_Feed:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SE_PROCESS_BEGIN);
//                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD2, 0xFFFF, param1);
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_FEED);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SE_PROCESS_FEED);
            }
            break;

        case Audio_Comm_infoType_Set_SE_Output_Main_Buffer_Consume:
            {
                MS_U16 u16RegVal;

                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_BEGIN);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while (u16RegVal != MBOX_DBGCMD_SE_PROCESS_BEGIN);
//                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD2, 0xFFFF, param1);
                HAL_AUDIO_WriteMaskReg(M2S_MBOX_DBG_CMD1, 0xFFFF, MBOX_DBGCMD_SE_PROCESS_CONSUME);
                do
                {
                    u16RegVal = HAL_AUDIO_ReadReg(S2M_MBOX_DBG_RESULT2);
                } while ((u16RegVal != MBOX_DBGCMD_SE_PROCESS_CONSUME_NEED) && (u16RegVal != MBOX_DBGCMD_SE_PROCESS_CONSUME_NO_NEED));
                if (u16RegVal == MBOX_DBGCMD_SE_PROCESS_CONSUME_NEED)
                    return TRUE;
                else
                    return FALSE;
            }
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
                HALMAD_ERROR("%s() : Failed to get channel status !!!\n", __FUNCTION__);
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_PUSI_disable_Info:
            if(param1)
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_Parser_Spec:
            if(param1)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_SPEC, ADEC1, 1, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_SPEC, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_PARSER_PUSI_DISABLE, ADEC1, 0, 0);
            }
            break;

        case Audio_Comm_infoType_setNR_Threshold:
            HAL_AUDIO_WriteByte(0x2D32, (MS_U8) param1);
            break;

        case Audio_Comm_infoType_Set_CallBackFunc:
            if ( g_FuncPrt_Hal_SetAudioParam2 == NULL )
            {
                g_FuncPrt_Hal_SetAudioParam2 = HAL_MAD_SetAudioParam2;
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_SetAudioParam2 hook OK !!\r\n", __FUNCTION__);
            }
            else
            {
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_SetAudioParam2 already register !!\r\n", __FUNCTION__);
            }
            if ( g_FuncPrt_Hal_GetAudioInfo2 == NULL )
            {
                g_FuncPrt_Hal_GetAudioInfo2 = HAL_MAD_GetAudioInfo2;
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_GetAudioInfo2 hook OK !!\r\n", __FUNCTION__);
            }
            else
            {
                HALMAD_ERROR ("%s: g_FuncPrt_Hal_GetAudioInfo2 already register !!\r\n", __FUNCTION__);
            }
            break;

        /********************************************************************************
        *       DECODER 1
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC1_Set_play_smpRate:
        {
            MS_U16 play_cmd;
            MS_U32 synth_H;

            play_cmd = HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD) & 0xF;
            if (play_cmd != 0x02)
                printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: audio play cmd is not Ts file Play !!\n", __FUNCTION__);

            if (param1)
            {
                if ((param2 > 50000) ||(param2<6000))
#if defined (__aarch64__)
                    printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: play rate [%ld] is invalid !!\n", __FUNCTION__, param2);
#else
                    printf("[%s : Audio_Comm_infoType_ADEC1_Set_play_smpRate]: play rate [%d] is invalid !!\n", __FUNCTION__, (int)param2);
#endif
                synth_H = 216000000 / param2;
                synth_H <<= 16;
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_EXT_SMPRATE_CTRL, ADEC1, 1, synth_H);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_EXT_SMPRATE_CTRL, ADEC1, 0, 0x11940000);
            }
        }
        break;

        case Audio_Comm_infoType_ADEC1_soundmod:
            {
                MS_U8 value = 0;

                switch((AUD_Dolby_SOUNDMODE)param1)
                {
                    case AUD_Dolby_SOUNDMODE_LR:     value = 0x0;  break;
                    case AUD_Dolby_SOUNDMODE_LL:     value = 0x1;  break;
                    case AUD_Dolby_SOUNDMODE_RR:     value = 0x2;  break;
                    default:                         value = 0x0;  break;
                }

                //NOT_DEFINE : to do
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SOUND_MODE, ADEC1, value, 0);
            }
            break;

        case Audio_Comm_infoType_SoundMode:                     //HAL_MAD_SetSoundMode
            HAL_AUDIO_SeWriteMaskByte(REG_MB_MODE_SELECT, 0x03, (AU_DVB_MODE) param1);
            break;

        case Audio_Comm_infoType_ADEC1_reqDataSize:
        case Audio_Comm_infoType_MMFileSize:                    //HAL_MAD_SetMadFileSize
            {
                MS_U32   file_size_value = 0x800;

                switch (param1)
                {
                    case FILE_SIZE_1KB:     file_size_value = 0x00400;        break;
                    case FILE_SIZE_2KB:     file_size_value = 0x00800;        break;
                    case FILE_SIZE_4KB:     file_size_value = 0x01000;        break;
                    case FILE_SIZE_8KB:     file_size_value = 0x02000;        break;
                    case FILE_SIZE_16KB:    file_size_value = 0x04000;        break;
                    case FILE_SIZE_32KB:    file_size_value = 0x08000;        break;
                    case FILE_SIZE_64KB:    file_size_value = 0x10000;        break;
                    default:                                                 break;
                }
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, ADEC1, file_size_value, 0);
            }
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_informDataRdy:
        case Audio_Comm_infoType_DEC1_MMTag:                    //HAL_MAD_SetMMIntTag
            {
               MS_U32 es_wrPtr = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0);
               MS_U32 es_bufSz = ES1_DRAM_SIZE;
               MS_U32 es_reqsz = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1);
                es_wrPtr += es_reqsz;

                if ( es_wrPtr >= es_bufSz )
                    es_wrPtr -= es_bufSz;

                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, es_wrPtr, 0);
            }
            break;

        case Audio_Comm_infoType_ADEC1_FF2X:
        case Audio_Comm_infoType_MM_FFx2:                       //HAL_MAD_SetMMFFx2Mode
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_ADEC1_mute:
            if ( param1 )
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, ADEC1, 0, 0);
            break;

        case Audio_Comm_infoType_ADEC1_play_wo_output:
            if (param1)
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x80);
            else
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DEC_CTRL, 0x80, 0x00);
            break;

        case Audio_Comm_infoType_ADEC1_playControl:
            switch((MMA_AUDIO_CONTROL)param1)
            {
                case MMA_STOP:
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
                    HAL_MAD_SetDecCmd(AU_DVB_DECCMD_AVSYNC);
                    g_AudioVars2->g_bDecPlayFileFlag = FALSE;
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
            break;

        case Audio_Comm_infoType_ADEC1_setDecParam:
            //NOT_DEFINE : remove
            break;

        case Audio_Comm_infoType_ADEC1_UNI_PCMOutCnt:
            uniPlay_ackCnt[0]++;
            HALMAD_PRINT("\r\nTrigger decInt[%d] %d\r\n", 0, uniPlay_ackCnt[0]);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT, ADEC1, param1, 0);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_PCMInput_Addr:   //assume UNI get data from PCM1.
            HAL_AUDIO_WriteReg(REG_DEC1_UNI_PCM3_WPTR, (MS_U16)((param1>>BYTES_IN_MIU_LINE_LOG2) - OFFSET_PCM1_DRAM_ADDR));
            break;

        case Audio_Comm_infoType_DEC1_setBufferProcess:
            // Need Decoder Add delay support in DEC1-DSP
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
        case Audio_Comm_infoType_DEC2_MMTag:                    //HAL_MAD2_SetMMIntTag
            {
                MS_U32 es_wrPtr, es_bufSz, es_reqsz;
                es_wrPtr = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0);
                es_bufSz = ES2_DRAM_SIZE;
                es_reqsz = param2;
                es_wrPtr += es_reqsz;

                if ( es_wrPtr >= es_bufSz )
                   es_wrPtr -= es_bufSz;
                 HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, es_wrPtr, 0);
            }
            break;

        case Audio_Comm_infoType_ADEC2_playControl:
            {
                switch((MMA_AUDIO_CONTROL)param1)
                {
                    case MMA_STOP:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STOP);
                        g_AudioVars2->g_bSePlayFileFlag = FALSE;
                        break;

                    case MMA_PLAY:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAY);
                        break;

                    case MMA_PLAY_FILETSP:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAYFILETSP);
                        break;

                    case MMA_RESYNC:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_RESYNC);
                        break;

                    case MMA_PLAY_FILE:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAYFILE);
                        break;

                    case MMA_BROWSE:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_STARTBROWSE);
                        break;

                    case MMA_PAUSE:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PAUSE);
                        break;

                    case MMA_PLAY_FRAME:
                        HAL_MAD_SetDSP2DecCmd(AU_DVB_DECCMD_PLAYFRAME);
                        break;

                    default:
                        break;
                }
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
            HAL_MAD_Write_DSP_sram(DSP1PmAddr_video_TD, param1, DSP_MEM_TYPE_PM);
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Protect:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_protect, param1, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_Vorbis_header_size:
            if ( param1 )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_VORBIS_HEADER_SIZE, ADEC1, param2, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_VORBIS_HEADER_SIZE, ADEC1, param2, 0);
            }
            break;

        case Audio_Comm_infoType_HDMI_Unstable_Threshold:
            //NOT_DEFINE : to do
            HAL_MAD2_Write_DSP_sram(DSP2DmAddr_hdmi_unstable_threshold, param1, DSP_MEM_TYPE_DM);
            break;

        case Audio_Comm_infoType_Set_Parser:

            switch(param1)
            {
                case 0 :   // MainWindow ADEC1 :  AFIFO_A->ES1  ; AFIFO_B->ES2
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0000); // AFIFO_A-> ES1
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0000); // AFIFO_B-> ES2
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0000); // AFIFO_C-> ES3(Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x0000); // AFIFO_D-> ES4(Dummy)
                    break;

                case 1 :   // MainWindow ADEC2 :  AFIFO_A->ES3  ; AFIFO_B->ES4
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0020); // AFIFO_C-> ES1 (Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0100); // AFIFO_D-> ES2 (Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0800); // AFIFO_A-> ES3
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x4000); // AFIFO_B-> ES4
                    break;

                case 2 :  // SubWindow ADEC1 :  AFIFO_B->ES1  ;
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0010); // AFIFO_B-> ES1
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0080); // AFIFO_C-> ES2(Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0800); // AFIFO_A-> ES3
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x0000); // AFIFO_D-> ES4(Dummy)
                    break;

                case 3 :  // SubWindow ADEC3 :  AFIFO_B->ES3  ;
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0030, 0x0000); // AFIFO_A-> ES1
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0180, 0x0080); // AFIFO_C-> ES2(Dummy)
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x0C00, 0x0C00); // AFIFO_B-> ES3
                    HAL_AUDIO_WriteMaskReg(REG_AUDIO_PARSER_CTRL, 0x6000, 0x0000); // AFIFO_D-> ES4(Dummy)
                    break;

                default :
                    break;
            };

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
            printf("%s: infoType[0x%x,0x%lx,0x%lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
#else
            printf("%s: infoType[0x%x,0x%x,0x%x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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

    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        /********************************************************************************
        *       Common
        ********************************************************************************/
        case Audio_Comm_infoType_getHDMI_CopyRight_C_Bit:
            /* this case is used to read HDMI bank register by SN, it should be implement by HDMI team */
            break;

        case Audio_Comm_infoType_getHDMI_CopyRight_L_Bit:
            /* this case is used to read HDMI bank register by SN, it should be implement by HDMI team */
            break;

        case Audio_Comm_infoType_Get_UNI_ES_MEMCNT:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC1);
            break;

        case Audio_Comm_infoType_Get_UNI_ES_Base:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_UNI_ES_Size:
            result = ES1_DRAM_SIZE ;
            break;

        case Audio_Comm_infoType_Get_UNI_PCM_Base:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_ADDR, ADEC1);
            break;

        case Audio_Comm_infoType_Get_UNI_PCM_Size:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_SIZE, ADEC1);
            break;

        case Audio_Comm_infoType_Get_AccumEsCnt:
//            result = HAL_MAD_Read_DSP_sram(REG_PTS_AccumEsCnt, DSP_MEM_TYPE_DM);
            result = 0;
            break;

        case Audio_Comm_infoType_Is_Decoder_In_R2:
            result = 1;
            break;

        /********************************************************************************
        *       DECODER 1
        ********************************************************************************/
        case Audio_Comm_infoType_ADEC1_capability:
        {
            MMA_AudioType audioType = Audio_DEC_NULL;
            audioType = _GetAudioCaps(AU_DEC_ID1);

        #if defined (CONFIG_UTOPIA_ANDROID_L)
            result = (MS_U64)audioType;
        #else
            result = (long long)audioType;
        #endif

            break;
        }

        case Audio_Comm_infoType_ADEC1_currAudDecStatus:
        case Audio_Comm_infoType_DecStatus:                     //no change
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_AudioSyncStatus:
            if(HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD)&0x80)
                result = 0;  // Free-run
            else
                result = 1;  // AV-sync
            break;

        case Audio_Comm_infoType_ADEC1_sampleRate:
        case Audio_Comm_infoType_SampleRate:                    //HAL_MAD_GetMadSamplingRate
        {
            MS_U32 r2_smpRate = 0xFFFF;
            r2_smpRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
            result = AUDIO_FS_NONE;

            switch(r2_smpRate)
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
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_acmod:
        case Audio_Comm_infoType_ChannelMode:                   //HAL_MAD_GetMadChannelMode
        {
            MS_U32 ac_mode = 0xFF;
            MS_U32 channel_mapping = 0x0;
            ac_mode = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC1);
            channel_mapping = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);

            switch(ac_mode)
            {
                case adec_ch_mono:
                    result =  AUD_CH_MODE_MONO;
                    break;

                case adec_ch_joint_stereo:
                    result =  AUD_CH_MODE_JOINT_STEREO;
                    break;

                case adec_ch_stereo:
                    result =  AUD_CH_MODE_STEREO;
                    break;

                case adec_ch_dual_mono:
                    result =  AUD_CH_MODE_DUAL_MONO;
                    break;

                case adec_ch_multi:
                    {
                        MS_U32 channel_mapping_mask_C_L_R, channel_mapping_mask_Ls_Rs =0x00;
                        channel_mapping_mask_Ls_Rs = (channel_mapping&(ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS));
                        channel_mapping_mask_C_L_R = (channel_mapping&(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R));

                        if(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_3_2_MODE;
                            else if(channel_mapping_mask_Ls_Rs == ACMOD_CH_MASK_NULL)
                               result =  AUD_CH_MODE_3_0_MODE;
                            else
                               result =  AUD_CH_MODE_3_1_MODE;
                        }
                        else if((channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L))||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                                ||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_R)))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_2_2_MODE;
                            else
                               result =  AUD_CH_MODE_2_1_MODE;
                        }
                        else //other multi-channel case, set it to AUD_CH_MODE_3_2_MODE
                        {
                            result =  AUD_CH_MODE_3_2_MODE;
                        }
                    }
                    break;

                default:
                    result =  AUD_CH_MODE_NONE;
                    break;
            }
        }
            break;

        case Audio_Comm_infoType_ADEC1_soundmod:
        case Audio_Comm_infoType_SoundMode:
            result = HAL_AUDIO_ReadReg(REG_MB_MODE_SELECT)&0x3;
            break;

        case Audio_Comm_infoType_ADEC1_okFrmCnt:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_errFrmCnt:
        case Audio_Comm_infoType_DecodeErrorCnt:                //HAL_MAD_Get_DecodeErrorCnt
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_1ms_timeStamp:
        case Audio_Comm_infoType_1ms_PTS:                       //no change
            {
                MS_U32  pts;
                pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
                result = pts/45;
            }
            break;

        case Audio_Comm_infoType_ADEC1_33bit_PTS:
        case Audio_Comm_infoType_33Bit_PTS:                     //HAL_MAD_GetMadPts
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC1);
            result <<= 1;
            break;

        case Audio_Comm_infoType_ADEC1_33bit_STCPTS_DIFF:
        case Audio_Comm_infoType_33Bit_STCPTS_DIFF:             //HAL_MAD_GetMadPtsStcDelta
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, ADEC1);

            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC1_Get_PTS_latency:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PTS_LATENCY, ADEC1);

            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC1_Get_PCM_bufferEmpty:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_EMPTY_CNT, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqFlag:
        {
            if (HAL_AUDIO_GetPlayFileFlag(DSP_DEC))
                result = 1;
            else
                result = 0;
        }
            break;

        case Audio_Comm_infoType_DEC1_BufferAddr:               //HAL_MAD_GetBufferAddr
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqAddr:
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_reqSize:
        case Audio_Comm_infoType_DEC1_BufferSize:               //HAL_MAD_GetBufferSize
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_esBuf_currLevel:
        case Audio_Comm_infoType_DEC1_ESBufferSize:             //HAL_MAD_GetESBufferSize
             result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_pcmBuf_currLevel:
        case Audio_Comm_infoType_DEC1_MMResidualPCM:            //HAL_MAD_GetMMResidualPCM
        case Audio_Comm_infoType_DEC1_PCMBufferSize:            //HAL_MAD_GetPCMBufferSize
           /*
             * The original pcm level from R2 is 10 channels,
             * but we only need 2 channels,
             * as a result devide by 5 here.
             */
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC1)/5;
            break;

        case Audio_Comm_infoType_ADEC1_UNI_pcm_wptr:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR + HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_WR_PTR, 0);
            break;

        case Audio_Comm_infoType_DMAReader_BufferSize:
           result = ((DSP2_DMA_READER_DRAM_SIZE+1)*BYTES_IN_MIU_LINE);
           break;

        case Audio_Comm_infoType_Get_SE_Input_Main_BufferBase:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_SE_MAIN_IN_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_SE_Input_Main_BufferSize:
            result = SE_MAIN_IN_DRAM_SIZE;
            break;

        case Audio_Comm_infoType_Get_SE_Output_Main_BufferBase:
            result = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_SE_MAIN_OUT_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_Get_SE_Output_Main_BufferSize:
            result = SE_MAIN_OUT_DRAM_SIZE;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_FRAME_SMP_UNIT:
            result = u32SeProcessFrameSmpUnit;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_FETCH_CHANNELS:
            result = SE_PROCESS_FETCH_CHANNELS;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_STORE_CHANNELS:
            result = SE_PROCESS_STORE_CHANNELS;
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_FETCH_FRAME_BYTE_SIZE:
            result = SE_PROCESS_FETCH_FRAME_LINE_SIZE * BYTES_IN_MIU_LINE * (u32SeProcessFrameSmpUnit/SE_PROCESS_FRAME_SMP_UNIT);
            break;

        case Audio_Comm_infoType_Get_SE_PROCESS_STORE_FRAME_BYTE_SIZE:
            result = SE_PROCESS_STORE_FRAME_LINE_SIZE * BYTES_IN_MIU_LINE * (u32SeProcessFrameSmpUnit/SE_PROCESS_FRAME_SMP_UNIT);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_pcm_decSize:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_UNI_OutPCMLevel:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC1);
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
                // not used, remove
            }
            break;

        case Audio_Comm_infoType_DMAReader_BufferLevel:
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0010);
            result = ((MS_U32)HAL_AUDIO_ReadReg(0x2B8C)<<3);      // 0x2B8C is line size
            HAL_AUDIO_WriteMaskReg(0x2B80, 0x0010, 0x0000);
            break;

        case Audio_Comm_infoType_ADEC1_getDecChannelInfo:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);
            break;

        case Audio_Comm_infoType_ADEC1_audio_unsupported:
            // NOT_DEFINE : to do
            if(HAL_AUDIO_ReadByte(REG_DEC1_INT_ID) == 0x09)
                result=1;
            else
                result=0;
            break;

        case Audio_Comm_infoType_ADEC1_avSyncOffset:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, ADEC1) & 0x00FFFFFF;
            break;

        /*******************************************************************************
        *       DECODER 2
        *******************************************************************************/
        case Audio_Comm_infoType_ADEC2_capability:
            result = Audio_DEC_MP3;
            break;

        case Audio_Comm_infoType_ADEC2_currAudDecStatus:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_sampleRate:
            {
                MS_U32 r2_smpRate = 0xFFFF;
                r2_smpRate = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC2);
                result = AUDIO_FS_NONE;

                switch(r2_smpRate)
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

        case Audio_Comm_infoType_ADEC2_bitRate:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_acmod:
        {
            MS_U32 ac_mode = 0xFF;
            MS_U32 channel_mapping = 0x0;
            ac_mode = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC2);
            channel_mapping = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC2);

            switch(ac_mode)
            {
                case adec_ch_mono:
                    result =  AUD_CH_MODE_MONO;
                    break;

                case adec_ch_joint_stereo:
                    result =  AUD_CH_MODE_JOINT_STEREO;
                    break;

                case adec_ch_stereo:
                    result =  AUD_CH_MODE_STEREO;
                    break;

                case adec_ch_dual_mono:
                    result =  AUD_CH_MODE_DUAL_MONO;
                    break;

                case adec_ch_multi:
                    {
                        MS_U32 channel_mapping_mask_C_L_R, channel_mapping_mask_Ls_Rs =0x00;
                        channel_mapping_mask_Ls_Rs = (channel_mapping&(ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS));
                        channel_mapping_mask_C_L_R = (channel_mapping&(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R));

                        if(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result = AUD_CH_MODE_3_2_MODE;
                            else if(channel_mapping_mask_Ls_Rs == ACMOD_CH_MASK_NULL)
                               result = AUD_CH_MODE_3_0_MODE;
                            else
                               result = AUD_CH_MODE_3_1_MODE;
                        }
                        else if((channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L))||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                                ||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_R)))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result = AUD_CH_MODE_2_2_MODE;
                            else
                               result = AUD_CH_MODE_2_1_MODE;
                        }
                        else //other multi-channel case, set it to AUD_CH_MODE_3_2_MODE
                        {
                            result = AUD_CH_MODE_3_2_MODE;
                        }
                    }
                    break;

                default:
                    result =  AUD_CH_MODE_NONE;
                    break;
            }
        }
            break;

        case Audio_Comm_infoType_ADEC2_soundmod:
        case Audio_Comm_infoType_ADEC2_okFrmCnt:
        case Audio_Comm_infoType_ADEC2_errFrmCnt:
            break;

        case Audio_Comm_infoType_ADEC2_1ms_timeStamp:
            {
                MS_U32  pts;
                pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC2);
                result = pts/45;
            }
            break;

        case Audio_Comm_infoType_ADEC2_33bit_PTS:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, ADEC2);
            result <<= 1;
            break;

        case Audio_Comm_infoType_ADEC2_33bit_STCPTS_DIFF:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TD, ADEC2);
            //sign ext
            result <<= 32;
            result >>= 31;
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqFlag:
        {
            if (HAL_AUDIO_GetPlayFileFlag(DSP_SE))
                result = 1;
            else
                result = 0;
        }
            break;

        case Audio_Comm_infoType_DEC2_BufferAddr:               //HAL_MAD2_GetBufferAddr
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES2_DRAM_ADDR;
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqAddr:
            result = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES2_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_reqSize:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC2);
            break;

        case Audio_Comm_infoType_DEC2_BufferSize:               //HAL_MAD2_GetBufferSize
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_esBuf_currLevel:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, ADEC2);
            break;

        case Audio_Comm_infoType_ADEC2_pcmBuf_currLevel:
        case Audio_Comm_infoType_DEC2_MMResidualPCM:            //HAL_MAD2_GetMMResidualPCM
           /*
             * The original pcm level from R2 is 10 channels,
             * but we only need 2 channels,
             * as a result devide by 5 here.
             */
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, ADEC2)/5;
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

        case Audio_Comm_infoType_ADEC2_getDecChannelInfo:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC2);
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
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_AVSYNC_OFFSET, ADEC2)& 0x00FFFFFF;
            break;

        case Audio_comm_infoType_Get_audio_crash_status:
            result = g_AudioVars2->g_u8AudioCrashFlag;
            break;

        case Audio_Comm_infoType_Get_Parser_Architecture:
            result = (MS_U64)E_AUD_4_PARSER_type1; // 2 AFIFO & 4 audio parser arch
            break;

        /* not support cmd */
        default:
            printf("%s: cmd not Supported\n", __FUNCTION__ );
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPEG_SOUNDMODE, ADEC1, (MS_U32)value, 0);
            }
            break;

        case Audio_MPEG_infoType_MMFileSize:                //HAL_MAD_MPEG_SetFileSize
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        case Audio_MPEG_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))              //delay offset range from 30ms ~ 300ms
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, ADEC1, (MS_U32)MPG_AV_DEFAULT_DELAY, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, ADEC1, (MS_U32)param1, 0);
            break;

        case Audio_MPEG_DTV_AVSYNC_DISABLE_SETTING:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MPG_AVOFFSET, ADEC1, (MS_U32)MPG_AV_DEFAULT_DELAY, 0);
            break;

        default:
#if defined (__aarch64__)
            printf("%s: infoType[0x%x,0x%08lx,0x%08lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
#else
            printf("%s: infoType[0x%x,0x%08x,0x%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
            if (((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_mpeg) && (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1) & 0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_MPEG_infoType_BitRate:       //HAL_MAD_MPEG_GetBitRate
            result = HAL_MAD_MPEG_GetBitRate();
            break;

        case Audio_MPEG_infoType_FrameNum:      //HAL_MAD_MPEG_GetframeNum
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_OK_FRMCNT, ADEC1);
            break;

        case Audio_MPEG_infoType_Layer:         //HAL_MAD_MPEG_GetLayer
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MPEG_LAYER, ADEC1);
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
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
            printf("Currently, it is not used\n");
            break;

        case Audio_WMA_infoType_MMFileSize:
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        default:
#if defined (__aarch64__)
            printf("%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
#else
            printf("%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
            break;

        case Audio_WMA_infoType_SampleRate:                 //HAL_MAD_WMA_GetSampleRate
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
            break;

        default:
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        case Audio_AC3P_infoType_DownmixMode:                //HAL_MAD_SetDolbyDownmixMode
            if((DOLBY_DOWNMIX_MODE)param1 == DOLBY_DOWNMIX_MODE_LORO)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 1, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC2, 1, 0);
            }
            else if ((DOLBY_DOWNMIX_MODE)param1 == DOLBY_DOWNMIX_MODE_LTRT)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC2, 0, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 2, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC2, 2, 0);
            }
            break;

        case Audio_AC3P_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            if((DOLBY_DRC_MODE)param1 == RF_MODE)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 1, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC2, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC2, 0, 0);
            }
            break;

        case Audio_AC3P_infoType_MMFileSize:            //HAL_MAD_ac3p_setFileSize
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_MMFileSize, param1, param2);
            break;

        case Audio_AC3P_infoType_syncSTC_in_mmTs:
            HAL_MAD_SetCommInfo(Audio_Comm_infoType_syncSTC_in_mmTs, param1, param2);
            break;

        case Audio_AC3P_infoType_hdmiTxBypass_enable:
            if (param1 > 1)
            {
                #if defined (CONFIG_UTOPIA_ANDROID_L)
                    printf("%s: param1[%x,%08x,%08x] out of range\n",__FUNCTION__, infoType, param1, param2);
                #else
                    printf("%s: param1[%x,%x,%x] out of range\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
                #endif
            }

            if( param2 == 0)
            {
                if(g_DDPBypassenable_1 != param1)
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DDP_HDMI_BYPASS,  param2, param1, 0);
                    HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x40, (param1 << 6));
                    g_DDPBypassenable_1 = param1;
                }
            }
            else
            {
                if(g_DDPBypassenable_2 != param1)
                {
                    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DDP_HDMI_BYPASS,  param2, param1, 0);
                    HAL_AUDIO_DecWriteMaskByte(REG_M2D_MAILBOX_SPDIF_CTRL+1, 0x40, (param1 << 6));
                    g_DDPBypassenable_2 = param1;
                }
            }
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, ADEC1, AC3_AV_DEFAULT_DELAY, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, ADEC1, param1, 0);
            break;

        case Audio_AC3P_DTV_AVSYNC_DISABLE_SETTING:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AC3_AVOFFSET, ADEC1, AC3_AV_DEFAULT_DELAY, 0);
            break;

        case Audio_AC3P_infoType_hdmiAC3inSE:
            if(param1 == TRUE)
            {
                FLAG_HDMI_AC3_IN_SE = TRUE;
            }
            else
            {
                FLAG_HDMI_AC3_IN_SE = FALSE;
            }
            break;

        case Audio_AC3P_infoType_enableDolbyBulletin11:
        case Audio_AC3P_infoType_LoudnessMatching:
            if(param1)
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x80, 0x80);
                g_AudioVars2->g_dolbyLoudness_Enable = TRUE;
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x80, 0x00);
                g_AudioVars2->g_dolbyLoudness_Enable = FALSE;
            }

            if ((g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ATSC) || (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DMBT))
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x60, 0x40);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = ATSC_MODE;
            }
            else if(g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DVB)
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x60, 0x00);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = DVB_MODE;
            }
            else
            {
                HAL_AUDIO_DecWriteMaskByte(REG_MB_DOLBY_LOUDNESS_INFO+1, 0x60, 0x20);
                g_AudioVars2->g_dolbyLoudness_DTVsystem = OTHER_MODE;
            }

            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_RFSIGNAL_TYPE, ADEC1, g_AudioVars2->g_dolbyLoudness_DTVsystem, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_TB11_ENABLE, ADEC1, g_AudioVars2->g_dolbyLoudness_Enable, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_RFSIGNAL_TYPE, ADEC2, g_AudioVars2->g_dolbyLoudness_DTVsystem, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_TB11_ENABLE, ADEC2, g_AudioVars2->g_dolbyLoudness_Enable, 0);
            break;

        case Audio_AC3P_infoType_Drc_HighCutScale:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_HIGH_CUT, ADEC1, param1, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_HIGH_CUT, ADEC2, param1, 0);
            break;

        case Audio_AC3P_infoType_Drc_LowBootScale:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_LOW_BOOST, ADEC1, param1, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_LOW_BOOST, ADEC2, param1, 0);
            break;

        case Audio_AC3P_infoType_Mixer_Balance:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MIXER_BALANCE, ADEC1, param1, 0);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MIXER_BALANCE, ADEC2, param1, 0);
            break;

        default:
#if defined (__aarch64__)
            printf("%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
#else
            printf("%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        case Audio_AC3P_infoType_DecStatus:
            if ((((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3) ||
                ((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3p)) &&
                (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1)&0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_AC3P_infoType_SampleRate:
            {
                MS_U32 SMP_RATE = 0;

                SMP_RATE = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);        //DD+
                switch(SMP_RATE)
                {
                    case 44100: result = SAMPLE_RATE_44100; break;
                    case 32000: result = SAMPLE_RATE_32000; break;
                    default:
                    case 48000: result = SAMPLE_RATE_48000; break;
                }
            }
            break;

        case Audio_AC3P_infoType_BitRate:  //unit : kbps
            {
                result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);        //DD+
            }
            break;

        case Audio_AC3P_infoType_Acmod:
            result = HAL_MAD_GetCommInfo(Audio_Comm_infoType_ADEC1_acmod);
            break;

        case Audio_AC3P_infoType_DrcMode:
            if (HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 0) == 1)
                result = RF_MODE;
            else
                result = LINE_MODE;
            break;

        case Audio_AC3P_infoType_DownmixMode:
            if (HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 0) == 1)
                result = DOLBY_DOWNMIX_MODE_LORO;
            else
                result = DOLBY_DOWNMIX_MODE_LTRT;
            break;

        case Audio_AC3P_infoType_AC3Type:
            if ((((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3) ||
                ((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3p)) &&
                (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1) & 0x0001))
            {
                result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_DEC_TYPE, ADEC1);
                result += 1;
            }
            else
            {
                result = 0;
            }
            break;

        case Audio_AC3P_infoType_FrameSize:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_FRAME_SIZE, ADEC1);
            break;

        case Audio_AC3P_infoType_BSMode:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DOLBY_BS_MODE, ADEC1);
            break;


        case Audio_AC3P_infoType_hdmiAC3inSE:
            if(FLAG_HDMI_AC3_IN_SE == TRUE)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
            break;

         case  Audio_AC3P_infoType_hdmi_in_support_DDP:
            result = 1;
            break;

        case Audio_AC3P_infoType_ATMOS_support:
            result = ((g_AudioVars2->stDigitalOutCodecCapability[0][CODEC_DDP].support_SAD_byte3) & 0x1);
            break;

        default:
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
            printf("%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
#else
            printf("%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
            if ((((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3) ||
                ((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_ac3p)) &&
                (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1) & 0x0001))
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
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_LFE_ENABLE, ADEC1, 0, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_LFE_ENABLE, ADEC1, 1, 0);
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_MODE, ADEC1, 0, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DTS_DMX_MODE, ADEC1, 1, 0);
            }
            break;

        case Audio_DTS_infoType_ESDataServiceMode:
            //force Audio_DTS_Ctrl_INTRPTMode in T3
            break;

        case Audio_DTS_infoType_ESData_ByteSwap:
            printf("Currently, it is not used\n");
            break;

        case Audio_DTS_infoType_CertMode:
            //DTS Certification Mode, Need to disable Sound Eeffect
            if (ctrl_type == Audio_DTS_Ctrl_CertMode_ON)
            {
                Flag_MAIN_SNDEFFET_CHG = TRUE;
                Reg_REG_SOUND_MAIN_SNDEFFECT = HAL_AUDIO_ReadReg(REG_SOUND_MAIN_SNDEFFECT);
                HAL_AUDIO_WriteReg(REG_SOUND_MAIN_SNDEFFECT,0x0000);
                Reg_REG_SOUND_ADV_CFG0 = HAL_AUDIO_ReadReg(REG_SOUND_ADV_CFG0);
                HAL_AUDIO_WriteReg(REG_SOUND_ADV_CFG0,0x0000);
            }
            if (ctrl_type == Audio_DTS_Ctrl_CertMode_OFF)
            {
                if(Flag_MAIN_SNDEFFET_CHG)
                {
                    HAL_AUDIO_WriteReg(REG_SOUND_MAIN_SNDEFFECT, Reg_REG_SOUND_MAIN_SNDEFFECT);
                    HAL_AUDIO_WriteReg(REG_SOUND_ADV_CFG0, Reg_REG_SOUND_ADV_CFG0);
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

    HALMAD_CHECK_SHM_INIT;

    switch(infoType)
    {
        case Audio_DTS_infoType_DecStatus:
            if (((HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_TYPE) & adec_type_regmask) == adec_type_dts) && (HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, ADEC1)&0x0001))
                result = 1;
            else
                result = 0;
            break;

        case Audio_DTS_infoType_KernelVersion:
            printf("Currently, it is not used\n");
            break;

        case Audio_DTS_infoType_SampleRate: // Original FS from Header
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, ADEC1);
            break;

        case Audio_DTS_infoType_BitRate:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_BIT_RATE, ADEC1);
            break;

        case Audio_DTS_infoType_CD_MODE:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DTS_CD_MODE, ADEC1);
            break;

        case Audio_DTS_infoType_Amode:
        {
            MS_U32 ac_mode = 0xFF;
            MS_U32 channel_mapping = 0x0;
            ac_mode = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MODE, ADEC1);
            channel_mapping = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_CHANNEL_MAPPING, ADEC1);

            switch(ac_mode)
            {
                case adec_ch_mono:
                    result =  AUD_CH_MODE_MONO;
                    break;

                case adec_ch_joint_stereo:
                    result =  AUD_CH_MODE_JOINT_STEREO;
                    break;

                case adec_ch_stereo:
                    result =  AUD_CH_MODE_STEREO;
                    break;

                case adec_ch_dual_mono:
                    result =  AUD_CH_MODE_DUAL_MONO;
                    break;

                case adec_ch_multi:
                    {
                        MS_U32 channel_mapping_mask_C_L_R, channel_mapping_mask_Ls_Rs =0x00;
                        channel_mapping_mask_Ls_Rs = (channel_mapping&(ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS));
                        channel_mapping_mask_C_L_R = (channel_mapping&(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R));

                        if(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_3_2_MODE;
                            else if(channel_mapping_mask_Ls_Rs == ACMOD_CH_MASK_NULL)
                               result =  AUD_CH_MODE_3_0_MODE;
                            else
                               result =  AUD_CH_MODE_3_1_MODE;
                        }
                        else if((channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_L))||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_L|ACMOD_CH_MASK_R))
                                ||(channel_mapping_mask_C_L_R==(ACMOD_CH_MASK_C|ACMOD_CH_MASK_R)))
                        {
                            if(channel_mapping_mask_Ls_Rs == (ACMOD_CH_MASK_LS|ACMOD_CH_MASK_RS))
                               result =  AUD_CH_MODE_2_2_MODE;
                            else
                               result =  AUD_CH_MODE_2_1_MODE;
                        }
                        else //other multi-channel case, set it to AUD_CH_MODE_3_2_MODE
                        {
                            result =  AUD_CH_MODE_3_2_MODE;
                        }
                    }
                    break;

                 default:
                    result =  AUD_CH_MODE_NONE;
                    break;
            }
        }
            break;

        case Audio_DTS_infoType_HDMI_DSP_SEL:
            result = 0x0;                   //0: in DEC DSP, 1: in SND DSP
            break;

        default:
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 1, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_DMX_MODE, ADEC1, 0, 0);
            break;

        case Audio_AAC_infoType_DrcMode:                    //HAL_MAD_SetDolbyDrcMode
            if((DOLBY_DRC_MODE)param1 == RF_MODE)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 1, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC2, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC1, 0, 0);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_RF_MODE, ADEC2, 0, 0);
            }
            break;

        case Audio_AAC_DTV_AVSYNC_OFFSET:
            if ((param1<DELAY_LOWER_LIMIT)||(param1>DELAY_UPPER_LIMIT))                                                      //delay offset range from 30ms ~ 300ms
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, ADEC1, AAC_AV_DEFAULT_DELAY, 0);
            else
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, ADEC1, param1, 0);
            break;

        case Audio_AAC_mm_single_frame_mode:
            if (param1 > 0)
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER, ADEC1, 0, 0);
            }
            break;

        case Audio_AAC_DTV_AVSYNC_DISABLE_SETTING:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AAC_AVOFFSET, ADEC1, AAC_AV_DEFAULT_DELAY, 0);
            break;

        default:
#if defined (__aarch64__)
            printf("%s: infoType[%x,%08lx,%08lx] not Supported\n",__FUNCTION__, infoType, param1, param2);
#else
            printf("%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
#define DEC_UNSUPPORT_CNT   10
#define AAC_UNSUPPORT_ADDR  0x3824
MS_U32 HAL_MAD_GetAACInfo( Audio_AAC_infoType infoType )
{
    MS_U32 result=0;
    //MS_U32 result1=0, result2=0,
    MS_U8 MainDspCodeType = 0;

    HALMAD_CHECK_SHM_INIT;

    MainDspCodeType = g_AudioVars2->g_DspCodeType;
    if ((MainDspCodeType != AU_DVB_STANDARD_AAC) || (MainDspCodeType != AU_DVB_STANDARD_MS10_DDT) )
        return result;

    switch(infoType)
    {
        case Audio_AAC_error_count:
            result = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, ADEC1);
            break;

        case Audio_AAC_dec_unsupported:
            //decode unsupported, threshold 10
            printf("Currently, it is not used\n");
            break;

        default:
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
            printf("%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
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
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Samples:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_SampleRate:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Channels1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Regions1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_cplStart1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_cplQbits1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_FrameSize1:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 0 );
            break;
        case Audio_RA8LBR_Channels2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_Regions2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_cplStart2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_cplQbits2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_FrameSize2:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 1 );
            break;
        case Audio_RA8LBR_Channels3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_Regions3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_cplStart3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_cplQbits3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_FrameSize3:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 2 );
            break;
        case Audio_RA8LBR_Channels4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_Regions4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_cplStart4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_cplQbits4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_FrameSize4:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 3 );
            break;
        case Audio_RA8LBR_Channels5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_Regions5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_cplStart5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_cplQbits5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, ADEC1, param1, 4 );
            break;
        case Audio_RA8LBR_FrameSize5:
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, ADEC1, param1, 4 );
            break;
        default:
            printf("%s: infoType[%x,%08x,%08x] not Supported\n",__FUNCTION__, infoType, (unsigned int)param1, (unsigned int)param2);
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
            printf("%s: infoType[%x] not Supported\n",__FUNCTION__, infoType);
            break;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_LoadCode()
/// @brief \b Function \b Description: This routine is used to load the DVB algorithm code
/// @param u8Type      \b algorithm type
/// @return MS_BOOL    \b TRUE --DSP load code okay
///                       FALSE--DSP load code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_MAD_LoadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    dspCodeType = dspCodeType;

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_MAD_XPCM_setParam
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_MAD_XPCM_setParam(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_TYPE, ADEC1, audioType, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_CHANNELS, ADEC1, channels, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLERATES, ADEC1, sampleRate, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE, ADEC1, bitsPerSample, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BLOCK_SIZE, ADEC1, blockSize, 0 );
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK, ADEC1, samplePerBlock, 0 );

    return TRUE;
}

//====== STAOS PIP START 2012/02/01 ========

MS_BOOL HAL_MAD_XPCM_setParam2( AUDIO_DEC_ID DecId, Audio_XPCM_Param * pParam )
{
    Audio_XPCM_Param    * pParamXPCM = pParam;
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if ( adec_id < ADEC3 )
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_TYPE, adec_id, (MS_U32) pParamXPCM->audioType, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_CHANNELS, adec_id, (MS_U32) pParamXPCM->channels, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLERATES, adec_id, (MS_U32) pParamXPCM->sampleRate, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE, adec_id, (MS_U32) pParamXPCM->bitsPerSample, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_BLOCK_SIZE, adec_id, (MS_U32) pParamXPCM->blockSize, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK, adec_id, (MS_U32) pParamXPCM->samplePerBlock, 0 );
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MAD_COOK_setParam2( AUDIO_DEC_ID DecId, Audio_COOK_Param * pParam )
{
    Audio_COOK_Param * pParamCOOK = pParam;
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if ( adec_id < ADEC3 )
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_NUMCODECS, adec_id, pParamCOOK->mNumCodecs, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLES, adec_id, pParamCOOK->mSamples, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_SAMPLERATES, adec_id, pParamCOOK->mSampleRate, 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[0], 0 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[0], 0 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[1], 1 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[1], 1 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[2], 2 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[2], 2 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[3], 3 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[3], 3 );

        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CHANNELS, adec_id, pParamCOOK->Channels[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_REGIONS, adec_id, pParamCOOK->Regions[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLSTART, adec_id, pParamCOOK->cplStart[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_CPLQBITS, adec_id, pParamCOOK->cplQbits[4], 4 );
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_COOK_FRAMESIZE, adec_id, pParamCOOK->FrameSize[4], 4 );

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MAD_WMA_SetASFParm2( AUDIO_DEC_ID DecId, Audio_ASF_Param * pParam )
{
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

    if ( adec_id < ADEC3 )
    {
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_VERSION, adec_id, pParam->u32Version, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELS, adec_id, pParam->u32Channels, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_SAMPLERATE, adec_id, pParam->u32SampleRate, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BYTERATE, adec_id, pParam->u32ByteRate, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BLOCKALIGN, adec_id, pParam->u32BlockAlign, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_ENCOPT, adec_id, pParam->u32Encopt, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_PARSINGBYAPP, adec_id, pParam->u32ParsingByApp, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_BITS_PER_SAMPLE, adec_id, pParam->u32BitsPerSample, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_CHANNELMASK, adec_id, pParam->u32ChannelMask, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PARAM_EXIST, adec_id, pParam->u32DrcParamExist, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF, adec_id, pParam->u32DrcRmsAmpRef, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET, adec_id, pParam->u32DrcRmsAmpTarget, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF, adec_id, pParam->u32DrcPeakAmpRef, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET, adec_id, pParam->u32DrcPeakAmpTarget, 0);
        HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ASF_MAX_PACKET_SIZE, adec_id, pParam->u32MaxPacketSize, 0);

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL HAL_MAD_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param )
{
    MS_BOOL  bRet = TRUE;
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

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
            if ( DecId == AU_DEC_ID3 )
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
            else
            {
                switch ( (MMA_AUDIO_CONTROL) Param )
                {
                    case MMA_STOP:
                        if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                            (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                        {
                            HAL_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
                        }
                        else
                        {
                            HALMAD_ERROR ("%s: DecId %d NOT used by ANDROID MM, used by %d !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eSourceType );
                        }
                        g_AudioVars2->u16DecodeFrmCnt[DecId] = 0;
                        g_AudioVars2->g_bDecPlayFileFlag = FALSE;
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

                    case MMA_PLAY_FRAME_GS:
                        if ((g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_MM_IN) ||
                            (g_AudioVars2->AudioDecStatus[DecId].eSourceType == E_AUDIO_INFO_GAME_IN))
                        {
                        HAL_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME_GS);
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
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_TS_SYNC_STC, ADEC1, 0, 0);
            }
            break;
        }

        case Audio_ParamType_acMode:
        {
            // DD, DDP, HEAAC
            switch ( (AUD_Dolby_SOUNDMODE) Param )
            {
                case AUD_Dolby_SOUNDMODE_LR:     Param = 0x0;  break;
                case AUD_Dolby_SOUNDMODE_LL:     Param = 0x1;  break;
                case AUD_Dolby_SOUNDMODE_RR:     Param = 0x2;  break;
                default:                         Param = 0x0;  break;
            }
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_SOUND_MODE, ADEC1, Param, 0);
            break;
        }

        case Audio_ParamType_reqDataSize:
        {
            if ( DecId == AU_DEC_ID3 )
            {
                if ( (HAL_AUR2_ReadByte(REG_R2_DECODE2_CMD) & 0x7F) != 0 )
                {
                    break;
                }
            }
            else
            {
                if ( (HAL_AUR2_ReadByte(REG_R2_DECODE1_CMD) & 0x7F) != 0 )
                {
                    break;
                }
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

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FILE_REQ_SIZE, adec_id, Param, 0);
            break;
        }

        case Audio_ParamType_MM_FFx2:
        {
            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            if ( Param )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, adec_id, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_MM_FF2X, adec_id, 0, 0);
            }
            break;
        }

        case Audio_ParamType_mute:
        {
            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            if ( Param )
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, adec_id, 1, 0);
            }
            else
            {
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_DECODE_MUTE, adec_id, 0, 0);
            }
            break;
        }

        case Audio_ParamType_predecode:
        {
            printf("Currently, it is not used\n");
            break;
        }

        case Audio_ParamType_esBuf_informDataRdy:
            break;

        case Audio_ParamType_setWMADecParam:
            bRet = HAL_MAD_WMA_SetASFParm2(DecId, (Audio_ASF_Param *) Param);
            break;

        case Audio_ParamType_setCOOKDecParam:
            bRet = HAL_MAD_COOK_setParam2(DecId, (Audio_COOK_Param *) Param);
            break;

        case Audio_ParamType_setXPCMDecParam:
            bRet = HAL_MAD_XPCM_setParam2(DecId, (Audio_XPCM_Param *) Param);
            break;

        case Audio_ParamType_UNI_PCM3_Input_Addr:
            printf("Currently, it is not used\n");    // transfer to line unit
            break;

        case Audio_ParamType_UNI_NEED_DECODE_FRMCNT:

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            HALMAD_PRINT("\r\n=====ID:%X   Audio_ParamType_UNI_NEED_DECODE_FRMCNT =0x%X==========\r\n", adec_id, Param);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT, adec_id, Param, 0);
            break;

        case Audio_ParamType_UNI_ES_Wptr:  // OMX Wptr = offset to MAD start address
            if(adec_id == ADEC2)  // ES For 2nd Decoder
            {
                Param = Param - (MS_U32)(ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE + OFFSET_ES3_DRAM_ADDR);
            }
            else  // ES For 1st Decoder
            {
                Param = Param - (MS_U32)(ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE + OFFSET_ES1_DRAM_ADDR);
            }

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: adec_id %d not Supported\n", __FUNCTION__, adec_id );
                return FALSE;
            }

            HALMAD_PRINT("\r\n===== ID:%X  Audio_ParamType_UNI_ES_Wptr =0x%X==========\r\n", adec_id, Param);
            HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, adec_id, Param, 0);
            break;

        case Audio_ParamType_UNI_PCMOutCnt:
        {
            if(adec_id < MAX_ADEC)
            {
                uniPlay_ackCnt[adec_id]++;
                HALMAD_PRINT("\r\nTrigger decInt[%d] %d\r\n", adec_id, uniPlay_ackCnt[adec_id]);
                HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT, adec_id, Param, 0);
            }
            else
            {
                HALMAD_ERROR ("%s: adec_id %d over array size !!\r\n", __FUNCTION__, adec_id);
                return FALSE;
            }
            break;
        }

        case Audio_ParamType_AllocDecoder:
        {
            HALMAD_PRINT("\r\n=====DecID:%X  Audio_ParamType_AllocDecoder =0x%X==========\r\n", (unsigned int)DecId, (unsigned int)Param);
            MMA_AudioType eAudioType = (MMA_AudioType) Param;
            MMA_AudioType eAudioCaps;

            OS_OBTAIN_MUTEX(_s32MutexLoadCode, MSOS_WAIT_FOREVER);

            if ( g_AudioVars2->AudioDecStatus[DecId].bConnect == TRUE )
            {
                HALMAD_ERROR ("%s: DecId %d in Used !!\r\n", __FUNCTION__, DecId);
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
                    case Audio_DEC_XPCM:            eAudioFormat = MSAPI_AUD_DVB_XPCM;        break;
                    case Audio_DEC_MPEG:            eAudioFormat = MSAPI_AUD_DVB_MPEG;        break;
                    case Audio_DEC_MP3:             eAudioFormat = MSAPI_AUD_DVB_MP3;         break;
                    case Audio_DEC_AC3:             eAudioFormat = MSAPI_AUD_DVB_AC3;         break;
                    case Audio_DEC_AC3P:            eAudioFormat = MSAPI_AUD_DVB_AC3P;        break;
                    case Audio_DEC_AAC_LC:
                    case Audio_DEC_HEAAC_V1:
                    case Audio_DEC_HEAAC_V2:        eAudioFormat = MSAPI_AUD_DVB_AAC;         break;
                    case Audio_DEC_DOLBYPULSE:      eAudioFormat = MSAPI_AUD_DVB_MS10_DDT;    break;
                    case Audio_DEC_WMA:             eAudioFormat = MSAPI_AUD_DVB_WMA;         break;
                    case Audio_DEC_WMAPRO:          eAudioFormat = MSAPI_AUD_DVB_WMA_PRO;     break;
                    case Audio_DEC_RA8:             eAudioFormat = MSAPI_AUD_DVB_RA8LBR;      break;
                    case Audio_DEC_DTS:             eAudioFormat = MSAPI_AUD_DVB_DTS;         break;
                    case Audio_DEC_FLAC:            eAudioFormat = MSAPI_AUD_DVB_FLAC;        break;
                    case Audio_DEC_VORBIS:          eAudioFormat = MSAPI_AUD_DVB_VORBIS;      break;
                    case Audio_DEC_AMR_NB:          eAudioFormat = MSAPI_AUD_DVB_AMR_NB;      break;
                    case Audio_DEC_AMR_WB:          eAudioFormat = MSAPI_AUD_DVB_AMR_WB;      break;
                    case Audio_DEC_DRA:             eAudioFormat = MSAPI_AUD_DVB_DRA;         break;
                    case Audio_DEC_DTSLBR:          eAudioFormat = MSAPI_AUD_DVB_DTSLBR;      break;
                    case Audio_DEC_DTS_HD_ADO_R2:   eAudioFormat = MSAPI_AUD_DVB_DTSHDADO;    break;
                    case Audio_DEC_KTV:
                    case Audio_DEC_KTV2:
                    default:                        eAudioFormat = MSAPI_AUD_DVB_INVALID;     break;
                }
                bRet = HAL_AUDIO_SetSystem2(DecId, eAudioFormat);
                if ( bRet == TRUE )
                {
                    g_AudioVars2->AudioDecStatus[DecId].bConnect = TRUE;
                    g_AudioVars2->AudioDecStatus[DecId].eSourceType = E_AUDIO_INFO_MM_IN;
                    g_AudioVars2->AudioDecStatus[DecId].eAudFormat =(En_DVB_decSystemType) eAudioFormat;

                    /* expect used this path is OMX MM, so need to setup ch5 input source path to the decoder used to OMX MM */
                    if(DecId == AU_DEC_ID3)
                    {
                        HAL_AUDIO_SetInputPath(AUDIO_DSP3_DVB_INPUT, AUDIO_PATH_4);
                    }
                    else //DecID == AU_DEC_ID1, AU_DEC_ID2 is not used to OMX MM.
                    {
                        HAL_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT, AUDIO_PATH_4);
                    }
                }
            }
            else
            {
                HALMAD_ERROR ("%s: DecId %d Audio Type (0x%08X) not Support !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eAudFormat );
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
                        g_AudioVars2->g_DspCodeType = MSAPI_AUD_DVB_NONE;
                    }
                    else if ( DecId == AU_DEC_ID3 )
                    {
                        g_AudioVars2->g_Dsp2CodeType = MSAPI_AUD_DVB_NONE;
                    }
                }
                else
                {
                    HALMAD_ERROR ("%s: DecId %d NOT used by ANDROID MM, used by %d !!\r\n", __FUNCTION__, DecId, g_AudioVars2->AudioDecStatus[DecId].eSourceType);
                }
            }
            else
            {
                HALMAD_ERROR ("%s: DecId %d NOT in USED !!\r\n", __FUNCTION__, DecId);
                bRet = FALSE;
            }

            OS_RELEASE_MUTEX(_s32MutexLoadCode);

            break;

        case Audio_ParamType_omx_spdif_mode:
            if ( adec_id < ADEC3 )
            {
                MS_U32 u32SpdifParam;
                Audio_OMX_SPDIF_Param eMode;
                u32SpdifParam = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, 0);
                eMode = (Audio_OMX_SPDIF_Param) Param;
                if ( eMode == Audio_OMX_SPDIF_ON )
                {
                    u32SpdifParam |= 0x00000001;
                }
                else
                {
                    u32SpdifParam &= 0xFFFFFFFE;
                }
                //HALMAD_ERROR ("%s: DecId %d SpdifParam 0x%08X !!\r\n", __FUNCTION__, DecId, u32SpdifParam);
                if ( (bRet = HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, u32SpdifParam, 0)) != TRUE )
                {
                    HALMAD_ERROR ("%s: DecId %d Fail to switch SPDIF mode to %s !!\r\n", __FUNCTION__, DecId, (eMode == Audio_OMX_SPDIF_ON ? "ON" : "OFF"));
                }
            }
            else
            {
                HALMAD_ERROR ("%s: Param Type %d DecId %d Un-Support Now !!\r\n", __FUNCTION__, Audio_ParamType_omx_spdif_mode, DecId);
                bRet = FALSE;
            }
            break;

        case Audio_ParamType_omx_spdif_play:

            if ( adec_id < ADEC3 )
            {
                MS_U32 u32SpdifParam;
                Audio_OMX_SPDIF_Param eMode;
                u32SpdifParam = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, 0);
                eMode = (Audio_OMX_SPDIF_Param) Param;
                if ( eMode == Audio_OMX_SPDIF_PLAY )
                {
                    u32SpdifParam |= 0x00000002;
                }
                else
                {
                    u32SpdifParam &= 0xFFFFFFFD;
                }
                //HALMAD_ERROR ("%s: DecId %d SpdifParam 0x%08X !!\r\n", __FUNCTION__, DecId, u32SpdifParam);
                if ( (bRet = HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_CTRL, adec_id, u32SpdifParam, 0)) != TRUE )
                {
                    HALMAD_ERROR ("%s: DecId %d Fail to switch SPDIF mode to %s !!\r\n", __FUNCTION__, DecId, (eMode == Audio_OMX_SPDIF_ON ? "ON" : "OFF"));
                }
            }
            else
            {
                HALMAD_ERROR ("%s: Param Type %d DecId %d Un-Support Now !!\r\n", __FUNCTION__, Audio_ParamType_omx_spdif_play, DecId);
                bRet = FALSE;
            }
            break;

        case Audio_ParamType_omx_spdif_pcmLevel:

            if ( adec_id < ADEC3 )
            {
                if ( (bRet = HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_OMX_SPDIF_PCM_LEVEL, adec_id, Param, 0)) != TRUE )
                {
                    HALMAD_ERROR ("%s: DecId %d Fail to setup PCM Level !!\r\n", __FUNCTION__, DecId);
                }
            }
            else
            {
                HALMAD_ERROR ("%s: Param Type %d DecId %d Un-Support Now !!\r\n", __FUNCTION__, Audio_ParamType_omx_spdif_pcmLevel, DecId);
                bRet = FALSE;
            }
            break;

        /* not supported cmd */
        default:
            HALMAD_ERROR ("%s: infoType[%x] not Supported\n", __FUNCTION__, paramType);
            bRet = FALSE;
            break;
    }

    return bRet;
}

MS_BOOL HAL_MAD_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    MS_BOOL result = TRUE;
    Audio_id adec_id = HAL_AUDIO_Convert_DecId_to_ADECId(DecId);

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

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            if ( HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_DEC_STAUS, adec_id) != 0 )
            {
                *pResult = 1;
            }
            else
            {
                *pResult = 0;
            }
            break;
        }

        case Audio_infoType_AudioSyncStatus:
        {
            MS_U8 * pResult = (MS_U8 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                if(HAL_AUDIO_AbsReadByte(REG_R2_DECODE2_CMD)&0x80)
                    *pResult = 0;  // Free-run
                else
                    *pResult = 1;  // AV-sync
            }
            else
            {
                if(HAL_AUDIO_AbsReadByte(REG_R2_DECODE1_CMD)&0x80)
                    *pResult = 0;  // Free-run
                else
                    *pResult = 1;  // AV-sync
            }
            break;
        }

        case Audio_infoType_SampleRate:                    //HAL_MAD_GetMadSamplingRate
        {
            MS_U32 fs_type = 0xFFFFFFFF;
            AUDIO_FS_TYPE * pFS_Type = (AUDIO_FS_TYPE *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            fs_type = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_SMP_RATE, adec_id);

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

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ERR_FRMCNT, adec_id);

            break;
        }

        case Audio_infoType_1ms_timeStamp:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            MS_U32  pts;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            pts = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, adec_id);
            *pRet = pts/45;

            break;
        }

        case Audio_infoType_33bit_PTS:
        {
            MS_U64 * pRet = (MS_U64 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = ((MS_U64)HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_CURR_PTS, adec_id))<<1;
            break;
        }

        case Audio_infoType_BufferAddr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES3_DRAM_ADDR;
            }
            else
            {
                *pRet = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR;
            }

            break;
        }

        case Audio_infoType_BufferSize:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, adec_id);

            break;
        }

        case Audio_infoType_esBuf_req:
        {
            MS_BOOL bFlag = FALSE;

            if ( DecId == AU_DEC_ID3 )
            {
                bFlag = g_AudioVars2->g_bSePlayFileFlag;
            }
            else
            {
                bFlag = g_AudioVars2->g_bDecPlayFileFlag;
            }

            if ( bFlag )
            {
                Audio_MM_Data_Request * pDataReq = (Audio_MM_Data_Request *) pInfo;

                if ( DecId == AU_DEC_ID3 )
                {
                    pDataReq->U32WrtBytes = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC2);
                    pDataReq->U32WrtAddr  = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC2, 0) + OFFSET_ES2_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
                }
                else
                {
                    pDataReq->U32WrtBytes = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_MM_FILE_REQ_SIZE, ADEC1);
                    pDataReq->U32WrtAddr  = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_ES_WR_PTR, ADEC1, 0) + OFFSET_ES1_DRAM_ADDR + HAL_AUDIO_GetDspMadBaseAddr(DSP_SE);
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

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);

            break;
        }

        case Audio_infoType_pcmBuf_currLevel:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, adec_id);

            break;
        }

        case Audio_infoType_get_audio_filter:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32) FILTER_TYPE_AUDIO_2;
            }
            else if ( DecId == AU_DEC_ID1 )
            {
                *pRet = (MS_U32) FILTER_TYPE_AUDIO_1;
            }
            else
            {
                *pRet = (MS_U32) FILTER_TYPE_AUDIO_INVALID;
            }

            break;
        }

        case Audio_infoType_audio_unsupported:
        {
            printf("Currently, it is not used\n");
            break;
        }

        case Audio_infoType_UNI_PCM1_Info:
        {
           Audio_MM_PCM_Info * pPCMInfo = (Audio_MM_PCM_Info *) pInfo;

           if ( HAL_AUDIO_GetUniDecodeFrameCnt(DecId) != g_AudioVars2->u16DecodeFrmCnt[DecId] )
           {
                if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
                {
                    HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                    return FALSE;
                }

                g_AudioVars2->u16DecodeFrmCnt[DecId] = HAL_AUDIO_GetUniDecodeFrameCnt(DecId);

                if ( DecId == AU_DEC_ID3 )
                {
                    pPCMInfo->U32WrtAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM2_DRAM_ADDR+HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR, adec_id);
                }
                else
                {
                    pPCMInfo->U32WrtAddr = HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR+ HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR, adec_id);
                }

                pPCMInfo->U32WrtBytes = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE, adec_id);
                HALMAD_PRINT("\r\n===DecId:%d  Audio_infoType_UNI_PCM1_Info  addr :0x%X     size:0x%X      DecodeFrame=0x%X==========\r\n", (MS_U32)DecId,(MS_U32)pPCMInfo->U32WrtAddr, (MS_U32)pPCMInfo->U32WrtBytes,(MS_U32)g_AudioVars2->u16DecodeFrmCnt[DecId]);
            }

            pPCMInfo->U32DecodeFrame = g_AudioVars2->u16DecodeFrmCnt[DecId];
        }
            break;

        case Audio_infoType_UNI_PCM3_Level:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_PCM_LEVEL, adec_id);
            break;
        }

        case Audio_infoType_UNI_ES_Level:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ES_LEVEL, adec_id);
            HALMAD_PRINT("\r\n=====ID:%X  Audio_infoType_UNI_ES_Level =0x%X==========\r\n", (MS_U32)adec_id, (MS_U32)*pRet);
            break;
        }

        case Audio_infoType_UNI_ES_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;

            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES3_DRAM_ADDR);
            }
            else
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + OFFSET_ES1_DRAM_ADDR);
            }
            break;
        }

        case Audio_infoType_UNI_ES_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = ES3_DRAM_SIZE;
            }
            else
            {
                *pRet = ES1_DRAM_SIZE;
            }
            break;
        }

        case Audio_infoType_UNI_PCM_Base:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM2_DRAM_ADDR);
            }
            else
            {
                *pRet = (MS_U32) (HAL_AUDIO_GetDspMadBaseAddr(DSP_SE) + ASND_DSP_DDR_SIZE + OFFSET_PCM1_DRAM_ADDR);
            }
            break;
        }

        case Audio_infoType_UNI_PCM_Size:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = PCM2_DRAM_SIZE;
            }
            else
            {
                *pRet = PCM1_DRAM_SIZE;
            }
            break;
        }

        case Audio_infoType_MadBaseAddr:
        {
            MS_U32 * pRet = (MS_U32 *) pInfo;
            if ( DecId == AU_DEC_ID3 )
            {
                *pRet = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
            }
            else
            {
                *pRet = HAL_AUDIO_GetDspMadBaseAddr(DSP_ADV);
            }
            break;
        }

        case Audio_infoType_hashkey:
        {
            /* IP AUTH :
             *      ip_auth[0] = 1: Disable AC3
             *      ip_auth[1] = 1: Disable AC3P
             *      ip_auth[2] = 1: Disable DDCO (HE-AAC)
             *      ip_auth[3] = 1: Disable DTS
             *      ip_auth[4] = 1: Disable Dolby MS10-DDT (HE-AAC)
             *      ip_auth[5] = 1: Disable WMA
             *      ip_auth[6] = 1: Disable DRA
             *      ip_auth[7] = 1: Disable DTSLBR
             *      ip_auth[8] = 1: Disable Generic HE-AAC
             *      ip_auth[9] = 1: Disable Dolby MS11
             *      ip_auth[10]= 1: Disable DTSE
             *      ip_auth[11]= 1: Disable DTS NeoUltra
             *      ip_auth[12]= 1: Demo Mode
             *      ip_auth[13]= 1: Disable SRS_TSHD
             *      ip_auth[14]= 1: Disable SRS_THEATERSOUND
             *      ip_auth[15]= 1: Disable DTS StudioSound3D
             *      ip_auth[16]= 1: Disable COOK
             *      ip_auth[17]= 1: Disable DTS XLL
             *      ip_auth[18]= 1: Disable MS12 LC profile
             *      ip_auth[19]= 1: Disable MS12 C profile
             *      ip_auth[20]= 1: Disable ABS3D
             *      ip_auth[21]= 1: Disable MS12 B profile
             */
            Audio_Hashkey_Info * pHashkeyInfo = (Audio_Hashkey_Info *) pInfo;
            MS_U32 audio_ipauth = HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_HASH_KEY, ADEC1, 0);

            pHashkeyInfo->U32IsHashkeySupported = TRUE;

            switch ( pHashkeyInfo->U32Name )
            {
                case Audio_DEC_AC3:
                {
                    if ((audio_ipauth & 0x1) && (audio_ipauth & 0x200) )
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_AC3P:
                {
                    if ( (audio_ipauth & 0x2) && (audio_ipauth & 0x200) )
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
                    if ( (audio_ipauth & 0x10) && (audio_ipauth & 0x100) )
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_WMA:
                case Audio_DEC_WMAPRO:
                {
                    if ( (audio_ipauth & 0x20) )
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DTS:
                {
                    if ( (audio_ipauth & 0x8) )
                    {
                        pHashkeyInfo->U32IsHashkeySupported = FALSE;
                    }
                    break;
                }

                case Audio_DEC_DRA:
                {
                    if ( (audio_ipauth & 0x40) )
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

            if(adec_id >= MAX_ADEC)    // SHM only has 2 mem
            {
                HALMAD_ERROR ("%s: DEC_ID  not Supported\n", __FUNCTION__ );
                return FALSE;
            }

            *pRet = HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_ACCUM_ES_BYTECNT, adec_id);
            break;
        }

        /* not support cmd */
        default:
            HALMAD_ERROR ("%s: cmd not Supported\n", __FUNCTION__ );
            result = FALSE;
            break;
    }

    return result;
}

//====== STAOS PIP END 2012/02/01 ========