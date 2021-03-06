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
#include "regCHIP.h"
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"
#include "halAUDIO.h"

// Internal Definition
#include "halSOUND.h"
#include "regAUDIO.h"
#include "halMAD2.h"

#include "audio_mbox2.h"
#include "audio_comm2.h"

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
    #define HALSOUND_PRINT(fmt, args...)    LOGI("<<android>>      " fmt, ## args)
  #else
    #define HALSOUND_PRINT(fmt, args...)
  #endif

  #define HALSOUND_ERROR(fmt, args...)    LOGE("<<android>>      " fmt, ## args)

#else
    #ifdef CONFIG_MBOOT //mboot Speed up
        #define HALSOUND_PRINT(fmt, args...)
        #define HALSOUND_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #else
        #define HALSOUND_PRINT(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
        #define HALSOUND_ERROR(fmt, args...)    printf("[[utopia]]      " fmt, ## args)
    #endif
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DBG_SOUND(msg) //msg
#define MAX_PEQ_BAND  8
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_U16 g_BalanceMask;
extern AUDIO_SHARED_VARS2 * g_AudioVars2;
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static MS_U32 peq_band_enable = 0xFFFFFFFF;
static MS_U32 peq_band_precision = 0x00000000;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_Init() @@VVV
/// @brief \b Function \b Description: This routine is the initialization for Audio sound effect module.
/// @param <IN>        \b InitTbl    : Sound effect  initial table
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_Init(void)
{
    // Toggle  to initialize DAC DATA SRAM.
    HAL_AUDIO_SeWriteMaskByte(0x2B40,0x02,0x02);
    AUDIO_DELAY1MS(1);
    HAL_AUDIO_SeWriteMaskByte(0x2B40,0x02,0x00);

    //reset DSP
    HAL_AUDIO_WriteByte( REG_SE_IDMA_CTRL0, 0x82);
    AUDIO_DELAY1MS(2);
    HAL_AUDIO_WriteByte( REG_SE_IDMA_CTRL0, 0x83);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_Init2
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_Init2(void)
{

    HAL_SOUND_Init();
    HAL_AUDIO_SeSystemLoadCode();  // Do nothing in single DSP chip
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetMute() @@VVV
/// @brief \b Function \b Description: This routine is used to set audio u8Path S/W mute.
/// @param <IN>        \b u8Path    : for audio u8Path0 ~ u8Path6
/// @param <IN>        \b bEnable    :     TRUE --Mute
///                                                    FALSE--Unmute
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetMute(MS_U8 u8Path, MS_BOOL bEnable)
{
    DBG_SOUND(printf("HAL_SOUND_SetMute = %x\n", (MS_U8)bEnable ));

    switch(u8Path)
    {
        case AUDIO_T3_PATH_AUOUT0:
            if(bEnable)  // Mute
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x80, 0x80);
            else        // UnMute
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x80, 0x00);
            break;

        case AUDIO_T3_PATH_AUOUT1:
            if(bEnable)
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x80, 0x80 );
            else
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x80, 0x00 );
            break;

        case AUDIO_T3_PATH_AUOUT2:
            if(bEnable)
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x80, 0x80);
            else
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x80, 0x00);
            break;

        case AUDIO_T3_PATH_AUOUT3:
            if(bEnable)
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0x80, 0x80);
            else
              HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0x80, 0x00);
            break;

        case AUDIO_T3_PATH_I2S:
            if(bEnable)
              HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x80, 0x80);
            else
              HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x80, 0x00);
            break;

        case AUDIO_T3_PATH_SPDIF:
            _HAL_AUDIO_SPDIF_SetMute(bEnable);
            break;

        case AUDIO_T3_PATH_MIXER_MAIN:
        {
            MS_U32 u32Value;

            /* TODO
             * It's a patch here
             * also control multi channel volume
             */
            u32Value = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL);

            if (bEnable)
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_CH5_MIX_VOL_INT, 0x80, 0x80);

                u32Value = u32Value | 0x008000;
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_CH5_MIX_VOL_INT, 0x80, 0x00);

                u32Value = u32Value & 0xFF7FFF;
            }

            HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL, u32Value);

            break;
        }

        case AUDIO_T3_PATH_MIXER_SECONDARY:
            if(bEnable)
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_CH6_MIX_VOL_INT, 0x80, 0x80);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_CH6_MIX_VOL_INT, 0x80, 0x00);
            }
            break;

        case AUDIO_T3_PATH_MIXER_DMA_IN:
            if(bEnable)
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_CH8_MIX_VOL_INT, 0x80, 0x80);
            }
            else
            {
                HAL_AUDIO_WriteMaskByte(REG_SOUND_CH8_MIX_VOL_INT, 0x80, 0x00);
            }
            break;

        case AUDIO_T3_PATH_PCM_CAPTURE1:
        {
            MS_U32 u32Value;

            u32Value = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_pcmCapture_volume);

            if (bEnable)
            {
                u32Value = u32Value | 0x008000;
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_pcmCapture_volume, u32Value);
            }
            else
            {
                u32Value = u32Value & 0xFF7FFF;
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_pcmCapture_volume, u32Value);
            }

            break;
        }

        case AUDIO_T3_PATH_PCM_CAPTURE2:
        {
            MS_U32 u32Value;

            u32Value = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_pcmCapture2_volume);

            if (bEnable)
            {
                u32Value = u32Value | 0x008000;
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_pcmCapture2_volume, u32Value);
            }
            else
            {
                u32Value = u32Value & 0xFF7FFF;
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_pcmCapture2_volume, u32Value);
            }

            break;
        }

         case AUDIO_T3_PATH_HDMI:
            if(bEnable)
                {
                //HDMI Tx mute bit, also control by main channel
                HAL_AUDIO_WriteMaskByte(REG_SOUND_HDMI_TX_VOLUME, 0x80, 0x80);
                _HAL_AUDIO_HDMI_Tx_SetMute(TRUE);
                }
            else
                {
                //HDMI Tx mute bit, also control by main channel
                HAL_AUDIO_WriteMaskByte(REG_SOUND_HDMI_TX_VOLUME, 0x80, 0x00);
                _HAL_AUDIO_HDMI_Tx_SetMute(FALSE);
                }
            break;

        case AUDIO_T3_PATH_MIXER_MCH_IN: //Multi-Ch 1~4
        {
            MS_U32 u32Value;

            u32Value = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL);

            if (bEnable)
            {
                u32Value = u32Value | 0x008000;
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL, u32Value);
            }
            else
            {
                u32Value = u32Value & 0xFF7FFF;
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL, u32Value);
            }

            break;
        }

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_AbsoluteVolume()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the absolute volume of audio u8Path.
/// @param <IN>        \b u8Path    : for audio CH1~CH6
/// @param <IN>        \b u8u8Vol1    :     MSB 7-bit register u8Value of 10-bit volume
///                                range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param <IN>        \b u8u8Vol2    :     LSB 3-bit register u8Value of 10-bit volume
///                                range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_AbsoluteVolume(MS_U8 u8Path, MS_U8 u8u8Vol1, MS_U8 u8u8Vol2)
{
    switch(u8Path)
    {
        case AUDIO_T3_PATH_AUOUT0:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT0_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x01, 0x01);   // bEnable DSP CH1 sound effect
            break;

        case AUDIO_T3_PATH_AUOUT1:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT1_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x02, 0x02);   // bEnable DSP CH2 sound effect
            break;

        case AUDIO_T3_PATH_AUOUT2:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT2_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x04, 0x04);   // bEnable DSP CH3 sound effect
            break;
        case AUDIO_T3_PATH_AUOUT3:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AUOUT3_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x08, 0x08);   // bEnable DSP CH4 sound effect
            break;

        case AUDIO_T3_PATH_I2S:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_I2S_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x10, 0x10);   // bEnable DSP CH5 sound effect
            break;

        case AUDIO_T3_PATH_SPDIF:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_SPDIF_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_SPDIF_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN, 0x20, 0x20);   // bEnable DSP CH6 sound effect

            break;

        case AUDIO_T3_PATH_HDMI:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_HDMI_TX_VOLUME, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_HDMI_TX_VOL_FRAC, 0xE0, (u8u8Vol2<<5));
            HAL_AUDIO_WriteMaskByte(M2S_MBOX_VOLUME_EN+1, 0x04, 0x04);   // bEnable AUDIO_T3_PATH_HDMI
            break;

        case AUDIO_T3_PATH_MIXER_MAIN:  //CH5
        {
            MS_U32 u32Value;
            MS_U32 u32Muteflag;

            HAL_AUDIO_WriteMaskByte(REG_SOUND_CH5_MIX_VOL_INT, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_CH5_MIX_VOL_FRC, 0xE0, (u8u8Vol2<<5));

            /* TODO
             * It's a patch here
             * also control multi channel volume
             */
            u32Muteflag = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL);

            u8u8Vol1 = u8u8Vol1 & 0x7F;
            u8u8Vol2 = (u8u8Vol2<<5) & 0xE0;
            u32Value = ((MS_U32)u8u8Vol1)<<8;
            u32Value = u32Value + (MS_U32)u8u8Vol2;
            u32Value = u32Value | (u32Muteflag & 0x008000);
            HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL, u32Value);

            break;
        }

        case AUDIO_T3_PATH_MIXER_SECONDARY: //CH6
            HAL_AUDIO_WriteMaskByte(REG_SOUND_CH6_MIX_VOL_INT, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_CH6_MIX_VOL_FRC, 0xE0, (u8u8Vol2<<5));
            break;

        case AUDIO_T3_PATH_MIXER_DMA_IN: //CH8
            HAL_AUDIO_WriteMaskByte(REG_SOUND_CH8_MIX_VOL_INT, 0x7F, u8u8Vol1);
            HAL_AUDIO_WriteMaskByte(REG_SOUND_CH8_MIX_VOL_FRC, 0xE0, (u8u8Vol2<<5));
            break;

        case AUDIO_T3_PATH_PCM_CAPTURE1:
        {
            MS_U32 u32Value;
            MS_U32 u32Muteflag;
            u32Muteflag = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_pcmCapture_volume);

            u8u8Vol1 = u8u8Vol1 & 0x7F;
            u8u8Vol2 = (u8u8Vol2<<5) & 0xE0;
            u32Value = ((MS_U32)u8u8Vol1)<<8;
            u32Value = u32Value + (MS_U32)u8u8Vol2;
            u32Value = u32Value | (u32Muteflag & 0x008000);
            HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_pcmCapture_volume, u32Value);

            break;
        }

        case AUDIO_T3_PATH_PCM_CAPTURE2:
        {
            MS_U32 u32Value;
            MS_U32 u32Muteflag;
            u32Muteflag = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_pcmCapture2_volume);

            u8u8Vol1 = u8u8Vol1 & 0x7F;
            u8u8Vol2 = (u8u8Vol2<<5) & 0xE0;
            u32Value = ((MS_U32)u8u8Vol1)<<8;
            u32Value = u32Value + (MS_U32)u8u8Vol2;
            u32Value = u32Value | (u32Muteflag & 0x008000);
            HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_pcmCapture2_volume, u32Value);

            break;
        }

        case AUDIO_T3_PATH_MIXER_MCH_IN: //Multi-Ch 1~4
        {
            MS_U32 u32Value;
            MS_U32 u32Muteflag;
            u32Muteflag = HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL);

            u8u8Vol1 = u8u8Vol1 & 0x7F;
            u8u8Vol2 = (u8u8Vol2<<5) & 0xE0;
            u32Value = ((MS_U32)u8u8Vol1)<<8;
            u32Value = u32Value + (MS_U32)u8u8Vol2;
            u32Value = u32Value | (u32Muteflag & 0x008000);
            HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_Multi_Channel_VOL, u32Value);

            break;
        }

        default:
            break;
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetPreScale() @@VVV
/// @brief \b Function \b Description: This routine is used to set the u8Prescale of audio u8Path.
/// @param <IN>        \b u8Path    : for audio u8Path0 ~ u8Path5
/// @param <IN>        \b u8Prescale    :     range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                                    0x00: disable pre-scale
///                                    0x6F: gain = 0db
///                                    0xFF: gain = +18db
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetPreScale(MS_U8 u8Path, MS_U8 u8Prescale)
{
    switch(u8Path)
    {
        case AUDIO_PATH_MAIN:
            HAL_AUDIO_WriteByte(REG_SOUND_MAIN_PERSCALE, u8Prescale);
            break;

        default:
            HAL_AUDIO_WriteByte(REG_SOUND_MAIN_PERSCALE, u8Prescale);
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetPreScale()
/// @brief \b Function \b Description: This routine is used to set the u8Prescale of audio u8Path.
/// @param <IN>        \b u8Path    : for audio u8Path0 ~ u8Path5
/// @param <IN>        \b u8Prescale    :     range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                                    0x00: disable pre-scale
///                                    0x6F: gain = 0db
///                                    0xFF: gain = +18db
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetPreScale(MS_U8 u8Path)
{
    MS_U16 tmp=0;
    switch ( u8Path )
    {
        case AUDIO_PATH_MAIN:
            tmp = HAL_AUDIO_ReadByte(REG_SOUND_MAIN_PERSCALE);
            break;

        default:
            tmp = HAL_AUDIO_ReadByte(REG_SOUND_MAIN_PERSCALE);
            break;
    }
    return tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableBalance()   @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable Balance featue.
/// @param <IN>        \b u8Enable    :    Balance Enable bits
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableBalance(MS_U8 u8Enable)
{
    HAL_AUDIO_WriteMaskByte(M2S_MBOX_BALANCE_EN+1, 0x80, (u8Enable<<7)); // Balance enable flag

    if(u8Enable)
       HAL_AUDIO_WriteMaskReg(M2S_MBOX_BALANCE_EN, g_BalanceMask, 0xFFFF); // Balance enable
    else
       HAL_AUDIO_WriteMaskReg(M2S_MBOX_BALANCE_EN, g_BalanceMask, 0x0000); // Balance disable
}

const MS_U8 BalanceTab[]=
{
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,  /// 0 ~ 9
    0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x12,0x13,  /// 10 ~ 19
    0x14,0x15,0x16,0x17,0x18,0x1A,0x1B,0x1C,0x1F,0x20,  /// 20 ~ 29
    0x22,0x23,0x25,0x27,0x29,0x2B,0x2E,0x30,0x33,0x36,  /// 30 ~ 39
    0x38,0x3D,0x41,0x46,0x4B,0x51,0x59,0x66,0x72,0x8B,  /// 40 ~ 49
    0xFF    /// 50
};

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetBalance_L()    @@VVV
/// @brief \b Function \b Description: This routine is used to set absolute balance u8Value.
/// @param <IN>        \b u8Balance    : balance register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetBalance(MS_U8 u8Balance)
{
    MS_U8 value_l = 0, value_r = 0;

    //printf("apiAud>> u8Balance = %02bx\n", u8Balance);
    if(u8Balance==50)
    {
        value_l = 0x00;
        value_r = 0x00;
    }
    else if(u8Balance<50)
    {
        value_l = 0x00;
        value_r = BalanceTab[(50-u8Balance)];
    }
    else if(u8Balance>50)
    {
        value_l = BalanceTab[(u8Balance-50)];
        value_r = 0x00;
    }
    //AUD_DEBUG(printf("\napiAud>> Balance UI:0x%bx L:0x%bx R:0x%bx",u8Balance,value_l,value_r));
    HAL_SOUND_SetBalance_L(value_l );
    HAL_SOUND_SetBalance_R(value_r );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetBalance_L()    @@VVV
/// @brief \b Function \b Description: This routine is used to set absolute balance u8Value of main u8Path L-channel.
/// @param <IN>        \b u8Balance_L    : balance register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetBalance_L(MS_U8 u8Balance_L)
{
    HAL_AUDIO_WriteByte(REG_SOUND_BALANCEL, u8Balance_L);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetBalance_L()
/// @brief \b Function \b Description: This routine is used to set absolute balance u8Value of main u8Path L-channel.
/// @param <IN>        \b u8Balance_L    : balance register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetBalance_L(void)
{
    return(HAL_AUDIO_ReadByte(REG_SOUND_BALANCEL));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetBalance_R()    @@VVV
/// @brief \b Function \b Description: This routine is used to set absolute balance u8Value of main u8Path R-channel.
/// @param <IN>        \b u8Balance_R    : balance register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetBalance_R(MS_U8 u8Balance_R)
{
    HAL_AUDIO_WriteByte(REG_SOUND_BALANCER, u8Balance_R);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetBalance_R()
/// @brief \b Function \b Description: This routine is used to set absolute balance u8Value of main u8Path L-channel.
/// @param <IN>        \b u8Balance_L    : balance register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetBalance_R(void)
{
    return(HAL_AUDIO_ReadByte(REG_SOUND_BALANCER));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetDynamicBass()
/// @brief \b Function \b Description: This routine is used to set the DynamicBass u8Level of main u8Path .
/// @param <IN>        \b u8Level    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetDynamicBass(MS_U8 u8Level)
{
    if (u8Level > 0x0C)
         u8Level = 0x0C;

    HAL_AUDIO_WriteMaskByte(REG_SOUND_EQ_BASE, 0xF0, u8Level<<4);
    if(u8Level == 0)
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_MAIN_SNDEFFECT, 0x20, 0x00);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_SOUND_MAIN_SNDEFFECT, 0x20, 0x20);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetBass() @@VVV
/// @brief \b Function \b Description: This routine is used to set the Bass u8Level of main u8Path .
/// @param <IN>        \b u8Level    : Bass u8Level (0~100) mapping to -16~+15dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetBass(MS_U8 u8Level)
{
    int u8Value;

    if(u8Level==100)
    {
        u8Value = 0x30;
    }
    else
    {
        u8Value = ((int)u8Level-50)*48/50; //16/50;
    }

    DBG_SOUND(printf("Bass:%d",u8Level));
    DBG_SOUND(printf("==>%x\r\n",u8Value));

    HAL_AUDIO_WriteByte(REG_SOUND_BASS, (MS_U8)u8Value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetBass()
/// @brief \b Function \b Description: This routine is used to set the Treble u8Level of main u8Path.
/// @param <IN>        \b u8Level    : Treble u8Level (0~100) mapping to -16~+15dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetBass(void)
{
    MS_U16 value = 0;

    value = HAL_AUDIO_ReadByte(REG_SOUND_BASS);
    return value;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_AbsoluteBass()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the absolute Bass u8Value of main u8Path .
/// @param <IN>        \b bass    :  -16~+15dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_AbsoluteBass(MS_U8 u8Bass)
{
    HAL_AUDIO_WriteByte(REG_SOUND_BASS, u8Bass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetTreble()   @@VVV
/// @brief \b Function \b Description: This routine is used to set the Treble u8Level of main u8Path.
/// @param <IN>        \b u8Level    : Treble u8Level (0~100) mapping to -16~+15dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetTreble(MS_U8 u8Level)
{
    int u8Value;

    if(u8Level==100)
    {
        u8Value = 0x30;
    }
    else
    {
        u8Value = ((int)u8Level-50)*48/50; //16/50;
    }

    DBG_SOUND(printf("Treble:%d",u8Level));
    DBG_SOUND(printf("==>%x\r\n",u8Value));

    HAL_AUDIO_WriteByte(REG_SOUND_TREBLE, (MS_U8)u8Value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetTreble()
/// @brief \b Function \b Description: This routine is used to set the Treble u8Level of main u8Path.
/// @param <IN>        \b u8Level    : Treble u8Level (0~100) mapping to -16~+15dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetTreble(void)
{
    MS_U16 value = 0;

    value = HAL_AUDIO_ReadByte(REG_SOUND_TREBLE);
    return value;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_AbsoluteTreble()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the absolute Treble u8Value of main u8Path .
/// @param <IN>        \b treble    :  -16~+15dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_AbsoluteTreble(MS_U8 u8Treble)
{
    HAL_AUDIO_WriteByte(REG_SOUND_TREBLE, u8Treble);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetEq()   @@VVV
/// @brief \b Function \b Description: This routine is used to set the the 5-band EQ u8Level  .
/// @param <IN>        \b band    :  EQ band 0~4
/// @param <IN>        \b u8Level    :  Absolute EQ register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetEq(MS_U8 u8Band, MS_U8 u8Level)
{
    MS_U8 value;

    if( u8Band>4)
    {
        return;
    }

    if(u8Level>=100)
    {
        value = 0x30;
    }
    else
    {
        value = ((int)u8Level-50)*48/50;
    }

    HAL_AUDIO_WriteByte(REG_SOUND_EQ1 + (u8Band*2), value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetEq()
/// @brief \b Function \b Description: This routine is used to set the the 5-band EQ u8Level  .
/// @param <IN>        \b band    :  EQ band 0~4
/// @param <IN>        \b u8Level    :  Absolute EQ register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetEq(MS_U8 u8Band)
{
    MS_U16 value = 0;

    value = HAL_AUDIO_ReadByte(REG_SOUND_EQ1 + (u8Band*2));
    return value;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetEq7()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the the 7-band EQ u8Level  .
/// @param <IN>        \b u8Band    :  EQ band 0~6
/// @param <IN>        \b u8Level    :  Absolute EQ register u8Value
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetEq7(MS_U8 u8Band, MS_U8 u8Level)
{
    MS_U8 value;

    if( u8Band>6)
      return;

    if(u8Level==100)
        value = 0x30;
    else
        value = ((int)u8Level-50)*48/50;

    HAL_AUDIO_WriteByte(REG_SOUND_EQ_BASE + (u8Band*2), value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetADCThreshold() @@VVV
/// @brief \b Function \b Description: This routine is used to set the ADC input energy threshold to reduce the background noise .
/// @param <IN>        \b u8Threshold    :  NR threshold u8Level
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetADCThreshold(MS_U8 u8Threshold)
{
    HAL_AUDIO_WriteByte(REG_SOUND_NR_THRESHOLD, u8Threshold);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetDRCThreshold() @@VVV
/// @brief \b Function \b Description: This routine is used to set the DRC  threshold u8Level.
/// @param <IN>        \b u8Threshold    :  DRC threshold u8Level
///                                    0x00--  0    dBFS
///                                    0x01--  -0.5    dBFS
///                                    0x20--  -16    dBFS
///                                    0x50--  -40    dBFS
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetDRCThreshold(MS_U8 u8Level)
{
    if (u8Level >= 0x50)
    u8Level = 0x50;

    HAL_AUDIO_WriteByte(REG_SOUND_DRC_THRESHOLD,u8Level);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetAVCThreshold() @@VVV
/// @brief \b Function \b Description: This routine is used to set the AVC  threshold u8Level.
/// @param <IN>        \b u8Threshold    :  AVC threshold u8Level
///                                    0x00--  0    dBFS
///                                    0x01--  -0.5    dBFS
///                                    0x20--  -16    dBFS
///                                    0x50--  -40    dBFS
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetAVCThreshold(MS_U8 u8Level)
{
    if (u8Level >= 0x50)
    u8Level = 0x50;

    HAL_AUDIO_WriteByte(REG_SOUND_AVC_THRESHOLD,u8Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetAVCThreshold()
/// @brief \b Function \b Description: This routine is used to set the AVC  threshold u8Level.
/// @param <IN>        \b NONE
/// @param <OUT>       \b Threshold value    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetAVCThreshold(void)
{
    return(HAL_AUDIO_SeReadByte(REG_SOUND_AVC_THRESHOLD));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetAvcMode()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the AVC  mode.
/// @param <IN>        \b u8AvcMode    :  AVC threshold u8Level
///                                    0: L mode
///                                    1: S mode
///                                    2: M mode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetAvcMode(MS_U8 u8AvcMode )
{
    switch(u8AvcMode)
    {
        case 0:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AVC_MODE ,0x03 ,0x00);
            break;
        case 1:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AVC_MODE ,0x03 ,0x01);
            break;
        case 2:
            HAL_AUDIO_WriteMaskByte(REG_SOUND_AVC_MODE ,0x03 ,0x02);
            break;
        default:
            break;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetAvcMode()
/// @brief \b Function \b Description: This routine is used to set the AVC  mode.
/// @param <RET>        \b u8AvcMode    :  AVC threshold u8Level
///                                    0: L mode
///                                    1: S mode
///                                    2: M mode
////////////////////////////////////////////////////////////////////////////////
MS_U16  HAL_SOUND_GetAvcMode(void)
{
    return(HAL_AUDIO_ReadByte(REG_SOUND_AVC_MODE)&0x03);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetAvcAT()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the AVC  attack time .
/// @param <IN>        \b u8AvcAT    :  AVC attack time
///                                 AvcAT = 0 --> 2 sec
///                                 AvcAT = 1 --> 1 sec
///                                 AvcAT = 2 --> 500 ms
///                                 AvcAT = 3 --> 400 ms
///                                 AvcAT = 4 --> 300 ms
///                                 AvcAT = 5 --> 200 ms
///                                 AvcAT = 6 --> 100 ms
///                                 AvcAT = 7 --> 20  ms
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetAvcAT(MS_U8 u8AvcAT )
{
    if(u8AvcAT>7)
        u8AvcAT=7;

    HAL_AUDIO_WriteMaskByte(REG_SOUND_AVC_AT ,0xE0 ,(u8AvcAT<<5));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetAvcAT()
/// @brief \b Function \b Description: This routine is used to set the AVC  mode.
/// @param <RET>
///                                 AvcAT = 0 --> 2 sec
///                                 AvcAT = 1 --> 1 sec
///                                 AvcAT = 2 --> 500 ms
///                                 AvcAT = 3 --> 400 ms
///                                 AvcAT = 4 --> 300  ms
///                                 AvcAT = 5 --> 200 ms
///                                 AvcAT = 6 --> 100 ms
///                                 AvcAT = 7 --> 20  ms
////////////////////////////////////////////////////////////////////////////////
MS_U16  HAL_SOUND_GetAvcAT(void)
{
    return((HAL_AUDIO_ReadByte(REG_SOUND_AVC_AT)&0xE0)>>5);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetAvcRT()    @@VVV
/// @brief \b Function \b Description: This routine is used to set the AVC  release time .
/// @param <IN>        \b u8AvcRT    :  AVC release time
///                                 u8AvcRT = 0 --> 2 sec
///                                 u8AvcRT = 1 --> 1 sec
///                                 u8AvcRT = 2 --> 500 ms
///                                 u8AvcRT = 3 --> 400 ms
///                                 u8AvcRT = 4 --> 300 ms
///                                 u8AvcRT = 5 --> 200 ms
///                                 u8AvcRT = 6 --> 100 ms
///                                 u8AvcRT = 7 --> 20  ms
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetAvcRT(MS_U8 u8AvcRT )
{
    if(u8AvcRT>7)
        u8AvcRT=7;

    HAL_AUDIO_WriteMaskByte(REG_SOUND_AVC_RT ,0x1C ,(u8AvcRT<<2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetAvcRT()
/// @brief \b Function \b Description: This routine is used to set the AVC  mode.
/// @param <RET>        \b u8AvcRT    :  AVC release time
///                                 u8AvcRT = 0 --> 2 sec
///                                 u8AvcRT = 1 --> 1 sec
///                                 u8AvcRT = 2 --> 500 ms
///                                 u8AvcRT = 3 --> 400 ms
///                                 u8AvcRT = 4 --> 300 ms
///                                 u8AvcRT = 5 --> 200 ms
///                                 u8AvcRT = 6 --> 100 ms
///                                 u8AvcRT = 7 --> 20  ms
////////////////////////////////////////////////////////////////////////////////
MS_U16  HAL_SOUND_GetAvcRT(void)
{
    MS_U16 Mode=0;

    Mode = (HAL_AUDIO_ReadByte(REG_SOUND_AVC_RT)&0x1C) >> 2;
   return Mode;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableEaseVol()  @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable the EaseVol featue.
/// @param <IN>        \b bEnable :     TRUE  -- Enable  EaseVol
///                                     FALSE -- Disable EaseVol
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableEaseVol(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D20, 0x40, bEnable<<6);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableEQ()   @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable EQ featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable EQ
///                                                    FALSE--Disable EQ
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableEQ(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D20, 0x80, bEnable<<7);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetEQ_Status()
/// @brief \b Function \b Description: This routine is used to get the EQ is enable or not
/// @param <IN>       \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>        \b bEnable    :     TRUE --Enable EQ
///                                                    FALSE--Disable EQ
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetEQ_Status(void)
{
    if((HAL_AUDIO_ReadByte(0x2D20)&0x80) == 0x80)
    {
        return 1;
    }else{
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableSurround()  @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable Surround featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable Surround
///                                                    FALSE--Disable Surround
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableSurround(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D21, 0x04, bEnable<<2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetSurround_Status()
/// @brief \b Function \b Description: This routine is used to get the Surround is enable or not
/// @param <IN>       \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>        \b bEnable    :     TRUE --Enable Surround
///                                                    FALSE--Disable Surround
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetSurround_Status(void)
{
    if((HAL_AUDIO_ReadByte(0x2D21)&0x04) == 0x04)
    {
        return 1;
    }else{
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableTone()  @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable the treble & Bass featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable Tone
///                                                    FALSE--Disable Tone
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableTone(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D21, 0x08, bEnable<<3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetTone_Status()
/// @brief \b Function \b Description: This routine is used to get the Tone is enable or not
/// @param <IN>       \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>        \b bEnable    :     TRUE --Enable Tone
///                                                    FALSE--Disable Tone
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetTone_Status(void)
{
    if((HAL_AUDIO_ReadByte(0x2D21)&0x08) == 0x08)
    {
       return 1;
    }
    else
    {
       return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableDRC() @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable the DRC featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable DRC
///                                                    FALSE--Disable DRC
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableDRC(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D21, 0x20, bEnable<<5);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableAutoVolume() @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable the AVC featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable AutoVolume
///                                                    FALSE--Disable AutoVolume
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableAutoVolume(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D21, 0x10, bEnable<<4);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetAutoVolume_Status()
/// @brief \b Function \b Description: This routine is used to get the AutoVolume is enable or not
/// @param <IN>       \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>        \b bEnable    :     TRUE --Enable AutoVolume
///                                                    FALSE--Disable AutoVolume
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetAutoVolume_Status(void)
{
    if((HAL_AUDIO_ReadByte(0x2D21)&0x10) == 0x10)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableNR()
/// @brief \b Function \b Description: This routine is used to bEnable/disable the Noise Reduction featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable NR
///                                                    FALSE--Disable NR
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableNR(MS_U8 u8Level)
{
    HAL_AUDIO_WriteMaskByte(0x2D32, 0xFF, u8Level); // dm (IO_102);!2D24[0,7]
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetNRAttenuate()
/// @brief \b Function \b Description: This routine is used to set the attenuate u8Level while NR is bEnabled.
/// @param <IN>        \b u8Mode    :     0 :    Auto fading to -24 dB
///                                                  1 :    Auto fading to -24 dB
///                                                  2 :    Auto fading to -24 dB
///                                                  3 :    Auto fading to -24 dB
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetNRAttenuate(MS_U8 u8Mode)
{
    UNUSED(u8Mode);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableKTVEcho()  @@VVV
/// @brief \b Function \b Description: This routine is used to bEnable/disable the KTVEcho featue.
/// @param <IN>        \b bEnable    :     TRUE --Enable KTVEcho
///                                                    FALSE--Disable KTVEcho
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnableKTVEcho(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D20, 0x02, bEnable<<1);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_PowerDown_Wait() @@VVV
/// @brief \b Function \b Description: This routine is used to DSP power-down wait function.
/// @param <IN>        \b bEnable    :     TRUE --Not wait
///                                                    FALSE--wait
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_PowerDown_Wait(MS_BOOL bEnable)
{
    HAL_AUDIO_WriteMaskByte(0x2D31, 0x01, bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetAdAbsoluteVolume()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the absolute volume of AD.
/// @param <IN>        \b u8u8Vol1    :     MSB 7-bit register u8Value of 10-bit volume
///                                range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param <IN>        \b u8u8Vol2    :     LSB 3-bit register u8Value of 10-bit volume
///                                range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetAdAbsoluteVolume(MS_U8 u8Vol1, MS_U8 u8Vol2)
{
    MS_U32 Vol = ((u8Vol1<<3) | (u8Vol2&0x07));
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AD_VOLUME, ADEC1, Vol, 0);
    // Vol[10:0] =  mute[10]|int[9:3]|fra[2:0]
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetAdMute()  @@VVV
/// @brief \b Function \b Description: This routine is used to set AD mute.
/// @param <IN>        \b bEnable    :     TRUE --Mute AD
///                                        FALSE--Unmute AD
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetAdMute(MS_BOOL bEnable)
{
    HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_AD_VOLUME_MUTE, ADEC1, bEnable, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetCH1AudioDelay()
/// @brief \b Function \b Description: This routine is used to set the u8Value of audio delay.
/// @param <IN>        \b delay    :     0x20~0xC8(32~200), in ms unit
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetCH1AudioDelay(MS_U16 u16Delay)
{
    if (u16Delay > AUDIO_DELAY_UPPER_BOUND)
    {
        printf("Warning! Invalid value (%u), main audio delay should be less than %u ms. \r\n", u16Delay, AUDIO_DELAY_UPPER_BOUND);
        u16Delay = AUDIO_DELAY_UPPER_BOUND;
    }

    if (u16Delay < AUDIO_DELAY_LOWER_BOUND)
    {
        printf("Warning! Invalid value (%u), main audio delay should be great than %u ms. \r\n", u16Delay, AUDIO_DELAY_LOWER_BOUND);
        u16Delay = AUDIO_DELAY_LOWER_BOUND;
    }

    HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_mainAudioDelay, u16Delay);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetCH1AudioDelay()
/// @brief \b Function \b Description: This routine is used to set the u8Value of audio delay.
/// @param <RET>        \b delay    :     0x14~0xC8(20~200), in ms unit
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetCH1AudioDelay(void)
{
    return((MS_U16)HAL_MAD2_Read_DSP_Xbox(DSP2XboxAddr_mainAudioDelay));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetSpdifAudioDelay()
/// @brief \b Function \b Description: This routine is used to set the u8Value of spdif delay.
/// @param <IN>        \b delay    :     0x14~0xFA(20~250), in ms unit
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetSpdifAudioDelay(MS_U16 u16Delay)
{
    if (u16Delay > SPDIF_DELAY_UPPER_BOUND)
    {
        printf("Warning! Invalid value (%u), spdif audio delay should be less than %u ms. \r\n", u16Delay, SPDIF_DELAY_UPPER_BOUND);
        u16Delay = SPDIF_DELAY_UPPER_BOUND;
    }

    if (u16Delay < SPDIF_DELAY_LOWER_BOUND)
    {
        printf("Warning! Invalid value (%u), spdif audio delay should be great than %u ms. \r\n", u16Delay, SPDIF_DELAY_LOWER_BOUND);
        u16Delay = SPDIF_DELAY_LOWER_BOUND;
    }

    HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_spdifDelay, u16Delay);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetHdmiAudioDelay()
/// @brief \b Function \b Description: This routine is used to set the u8Value of independent hdmi delay.
/// @param <IN>        \b delay    :     0x14~0xFA(20~250), in ms unit
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetHdmiAudioDelay(MS_U16 u16Delay)
{
    if (u16Delay > HDMI_DELAY_UPPER_BOUND)
    {
        printf("Warning! Invalid value (%u), hdmi audio delay should be less than %u ms. \r\n", u16Delay, HDMI_DELAY_UPPER_BOUND);
        u16Delay = HDMI_DELAY_UPPER_BOUND;
    }

    if (u16Delay < HDMI_DELAY_LOWER_BOUND)
    {
        printf("Warning! Invalid value (%u), hdmi audio delay should be great than %u ms. \r\n", u16Delay, HDMI_DELAY_LOWER_BOUND);
        u16Delay = HDMI_DELAY_LOWER_BOUND;
    }

    HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_hdmiDelay, u16Delay);

}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetMenuSound() @@Need_Modify
/// @brief \b Function \b Description: This routine is used to Play Menu Sound.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_PlayMenuSound(void)
{
     /*Cathy0721
     HAL_AUDIO_WriteMaskByte(0x2DAE, 0x01, 0x01);
     AUDIO_DELAY1MS(10);
     HAL_AUDIO_WriteMaskByte(0x2DAE, 0x01, 0x00);*/
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetSurroundXA() @@VVV
/// @brief \b Function \b Description: This routine is used to set the Surround xA Gain.
/// @param <IN>        \b u8Mode    :     0 :    0.1         xA : 0x2D8A[3:2] 00: 0.1
///                                    1 :    0.15                         01: 0.15
///                                    2 :    0.2                          10: 0.2
///                                    3 :    0.25                         11: 0.25
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetSurroundXA(MS_U8 u8Mode)
{
   switch(u8Mode)
    {
        case 0:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x0C ,0x00);
            break;
        case 1:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x0C ,0x04);
            break;
        case 2:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x0C ,0x08);
            break;
        case 3:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x0C ,0x0C);
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetSurroundXA()
/// @brief \b Function \b Description: This routine is used to set the Surround xA Gain.
/// @param <RET> MODE  \b u8Mode    :     0 :    0.1         xA : 0x2D8A[3:2] 00: 0.1
///                                    1 :    0.15                         01: 0.15
///                                    2 :    0.2                          10: 0.2
///                                    3 :    0.25                         11: 0.25
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetSurroundXA(void)
{
    return((HAL_AUDIO_ReadByte(0x2D26)&0x0C)>>2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetSurroundXB()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the Surround xB Gain.
/// @param <IN>        \b u8Mode    :     0 :    0.25         xB : 0x2D8A[5:4] 00: 0.25
///                                                  1 :    0.3                           01: 0.3
///                                                  2 :    0.35                          10: 0.35
///                                                  3 :    0.45                          11: 0.45
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetSurroundXB(MS_U8 u8Mode)
{
    switch(u8Mode)
    {
        case 0:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x30 ,0x00);
            break;
        case 1:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x30 ,0x10);
            break;
        case 2:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x30 ,0x20);
            break;
        case 3:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0x30 ,0x30);
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetSurroundXB()
/// @brief \b Function \b Description: This routine is used to set the Surround xB Gain.
/// @param <RET> MODE  \b u8Mode    :     0 :    0.1         xB : 0x2D8A[3:2] 00: 0.1
///                                    1 :    0.15                         01: 0.15
///                                    2 :    0.2                          10: 0.2
///                                    3 :    0.25                         11: 0.25
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetSurroundXB(void)
{
    return((HAL_AUDIO_ReadByte(0x2D26)&0x30)>>2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetSurroundXK() @@VVV
/// @brief \b Function \b Description: This routine is used to set the Surround xK Gain.
/// @param <IN>        \b u8Mode    :     0 :    0.1         xK : 0x2D8A[7:6]  00: 0.1
///                                                  1 :    0.2                           01: 0.2
///                                                  2 :    0.3                           10: 0.3
///                                                  3 :    0.4                           11: 0.4
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetSurroundXK(MS_U8 u8Mode)
{
     HAL_AUDIO_WriteMaskByte(0x2D27 ,0x07 ,u8Mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetSurroundXK()
/// @brief \b Function \b Description: This routine is used to set the Surround xK Gain.
/// @param <RET> MODE  \b u8Mode    :     0 :    0.1         xK : 0x2D27[2:0] 00: 0.1
///                                                  1 :    0.2                           01: 0.2
///                                                  2 :    0.3                           10: 0.3
///                                                  3 :    0.4                           11: 0.4
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetSurroundXK(void)
{
    MS_U16 Mode=0;

    Mode = HAL_AUDIO_ReadByte(0x2D27)&0x07;

    return Mode;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetSurroundLPFGain() @@VVV
/// @brief \b Function \b Description: This routine is used to set the Surround LPF Gain.
/// @param <IN>        \b u8Mode    :     0 :    0 dB        LPF Gain : 0x2DA0[7:6]  00: 0 dB
///                                                  1 :    2 dB                                01: 2 dB
///                                                  2 :    4 dB                                10: 4 dB
///                                                  3 :    un-support                          11: un-support
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetSurroundLPFGain(MS_U8 u8Mode)
{
   switch(u8Mode)
    {
        case 0:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0xC0 ,0x00);
            break;
        case 1:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0xC0 ,0x40);
            break;
        case 2:
            HAL_AUDIO_WriteMaskByte(0x2D26 ,0xC0 ,0x80);
            break;
        case 3:
            //HAL_AUDIO_WriteMaskByte(0x2D26 ,0xC0 ,0xC0);
            break;
        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetSurroundLPFGain()
/// @brief \b Function \b Description: This routine is used to set the Surround xK Gain.
/// @param <RET>   \b LPFGain    :
//                                      0 :    0dB
///                                    1 :    2dB
///                                    2 :    4dB
///                                    3 :    up-support
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetSurroundLPFGain(void)
{
    return((HAL_AUDIO_ReadByte(0x2D26)&0xC0)>>6);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetPEQCoef()
/// @brief \b Function \b Description: This routine is used to set PEQ Coefficient.
/// @param <IN>        \b peq_coef
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
{
    MS_U16 dsp_addr;
    MS_U16 dsp_scale_addr;

    if(peq_coef->sfs == 0)
    {
        DBG_SOUND(printf("!!!only support 48K coefficient ! ! !\n");)
        return;
    }

    //PEQ setting
    if(peq_coef->type == AUDIO_COEF_PEQ || peq_coef->type == AUDIO_COEF_PREVER_PEQ)  // PEQ setting
    {
        if(peq_coef->enable == FALSE)
            peq_band_enable &= (0xFFFFFFFF ^ ((MS_U32)1<<peq_coef->band));
        else
            peq_band_enable |= ((MS_U32)1<<peq_coef->band);

        if(peq_coef->precision == AUDIO_SINGLE_PRECISION)
            peq_band_precision &= (0xFFFFFFFF ^ ((MS_U32)1<<peq_coef->band));
        else
            peq_band_precision |= ((MS_U32)1<<peq_coef->band);

        dsp_addr = DSP2XboxAddr_peq48KCoeffAddr + peq_coef->band*5;
        dsp_scale_addr = DSP2XboxAddr_peqscale48KAddr + peq_coef->band;

        HAL_AUDIO_WriteMaskByte(M2S_MBOX_SNDEFF_EN, 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT), 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT)); //disable  updating coefficient to prevent pop
        HAL_MAD2_Write_DSP_Xbox(dsp_addr,peq_coef->a0);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+1,peq_coef->a1);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+2,peq_coef->a2);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+3,peq_coef->b1);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+4,peq_coef->b2);
        HAL_MAD2_Write_DSP_Xbox(dsp_scale_addr,(MS_U32)peq_coef->scale);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peqbandEnAddr, peq_band_enable);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peqbandDoubleAddr, peq_band_precision);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peqUpdateFlag, 0x1);
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_SNDEFF_EN, 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT), 0x00); //enable coefficient update
    }
    // HLPF setting
    else  if(peq_coef->type == AUDIO_COEF_HPF)
    {
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_SNDEFF_EN, 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT), 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT)); //disable  updating coefficient to prevent pop
        dsp_addr = DSP2XboxAddr_hpf48KCoeffAddr;
        HAL_MAD2_Write_DSP_Xbox(dsp_addr,peq_coef->a0);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+1,peq_coef->a1);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+2,peq_coef->a2);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+3,peq_coef->b1);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+4,peq_coef->b2);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_hpUpdateFlag, 0x1);
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_SNDEFF_EN, 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT), 0x00); //disable oefficient update
    }
    //TONE setting
    else  if(peq_coef->type == AUDIO_COEF_BASS ||peq_coef->type == AUDIO_COEF_TREBLE)
    {
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_SNDEFF_EN, 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT), 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT)); //disable  updating coefficient to prevent pop
        if(peq_coef->type == AUDIO_COEF_BASS)
        {
            dsp_addr = DSP2XboxAddr_bass48KCoeffAddr;
        }
        else
        {
            dsp_addr = DSP2XboxAddr_treble48KCoeffAddr;
            dsp_scale_addr =DSP2XboxAddr_treblescale48KAddr;
            HAL_MAD2_Write_DSP_Xbox(dsp_scale_addr,(MS_U32)peq_coef->scale);
         }
        HAL_MAD2_Write_DSP_Xbox(dsp_addr,peq_coef->a0);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+1,peq_coef->a1);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+2,peq_coef->a2);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+3,peq_coef->b1);
        HAL_MAD2_Write_DSP_Xbox(dsp_addr+4,peq_coef->b2);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_toneSelectAddr,0x01);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_toneUpdateFlag, 0x1);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_trebleUpdateFlag, 0x1);
        HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_bassUpdateFlag, 0x1);
        HAL_AUDIO_WriteMaskByte(M2S_MBOX_SNDEFF_EN, 1<< (M2S_MBOX_COEFFUPDATE_EN_BIT), 0x00); //disable oefficient update
    }
    else
    {
        DBG_SOUND(printf("!!! undefined coefficient type ! ! !\n");)
        return;
    }

    DBG_SOUND(printf("HAL a0 = %x", (MS_U8)(peq_coef->a0>>16)));
    DBG_SOUND(printf(" %x", (MS_U8)(peq_coef->a0>>8)));
    DBG_SOUND(printf(" %x\r\n", (MS_U8)peq_coef->a0));

    DBG_SOUND(printf("HAL a1 = %x", (MS_U8)(peq_coef->a1>>16)));
    DBG_SOUND(printf(" %x", (MS_U8)(peq_coef->a1>>8)));
    DBG_SOUND(printf(" %x\r\n", (MS_U8)peq_coef->a1));

    DBG_SOUND(printf("HAL a2 = %x", (MS_U8)(peq_coef->a2>>16)));
    DBG_SOUND(printf(" %x", (MS_U8)(peq_coef->a2>>8)));
    DBG_SOUND(printf(" %x\r\n", (MS_U8)peq_coef->a2));

    DBG_SOUND(printf("HAL b1 = %x", (MS_U8)(peq_coef->b1>>16)));
    DBG_SOUND(printf(" %x", (MS_U8)(peq_coef->b1>>8)));
    DBG_SOUND(printf(" %x\r\n", (MS_U8)peq_coef->b1));

    DBG_SOUND(printf("HAL b2 = %x", (MS_U8)(peq_coef->b2>>16)));
    DBG_SOUND(printf(" %x", (MS_U8)(peq_coef->b2>>8)));
    DBG_SOUND(printf(" %x\r\n", (MS_U8)peq_coef->b2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnablePEQ()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b Enpeq
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_EnablePEQ(MS_BOOL Enpeq)
{
    if(Enpeq)
    {
        HAL_AUDIO_WriteMaskByte(0x2D20, 0x01, 0x01);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(0x2D20, 0x01, 0x00);
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_GetPEQ_Status()
/// @brief \b Function \b Description: This routine is used to get the PEQ is enable or not
/// @param <IN>       \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>        \b bEnable    :     TRUE --Enable PEQ
///                                                    FALSE--Disable PEQ
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_SOUND_GetPEQ_Status(void)
{
    if((HAL_AUDIO_ReadByte(0x2D20)&0x01) == 0x01)
    {
       return 1;
    }else
    {
       return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_EnableDcRemove()
/// @brief \b Function \b Description: This routine is used to eable DC remove.
/// @param <IN>        \b EnDcRemove
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL HAL_SOUND_EnableDcRemove(MS_BOOL EnDcRemove)
{
    if(EnDcRemove)
    {
        HAL_AUDIO_WriteMaskByte(0x2D20, 0x08, 0x08);
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(0x2D20, 0x08, 0x00);
    }
    return 1;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetMixModeVolume()  @@VVV
/// @brief \b Function \b Description: This routine is used to set the absolute volume of mix mode.
/// @param <IN>        \b eSourceType: for audio source
/// @param <IN>        \b VolType    : for vol type
/// @param <IN>        \b u8u8Vol1   :     MSB 7-bit register u8Value of 10-bit volume
///                                range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param <IN>        \b u8u8Vol2   :     LSB 3-bit register u8Value of 10-bit volume
///                                range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  )
{
    switch (eSourceType)
    {
        case E_AUDIO_INFO_KTV_IN:
            switch(VolType)
            {
                case PCM_VOL:
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_DMA_IN, u8Vol1, u8Vol2);
                    break;

                case MIC_VOL:
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_MAIN, u8Vol1, u8Vol2);
                    break;

                case MP3_VOL:
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_SECONDARY, u8Vol1, u8Vol2);
		     break;

		 case ECHO1_VOL:
		 	if(u8Vol1<14)
		 	    u8Vol1 = 14;
		      HAL_AUDIO_WriteMaskByte(0x2D57, 0x7F, u8Vol1);
                    HAL_AUDIO_WriteMaskByte(0x2D56, 0xE0, (u8Vol2<<5));
                    break;

		case ECHO2_VOL:
		      HAL_AUDIO_WriteMaskByte(0x2D59, 0x7F, u8Vol1);
                    HAL_AUDIO_WriteMaskByte(0x2D58, 0xE0, (u8Vol2<<5));
                    break;

                default:
                    break;
            }
            break;

        case E_AUDIO_INFO_GAME_IN:
            switch(VolType)
            {
                case PCM_VOL:
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_DMA_IN, u8Vol1, u8Vol2);
                    break;

                case GAME1_VOL:
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_MAIN, u8Vol1, u8Vol2);
                    break;

                case GAME2_VOL:
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_SECONDARY, u8Vol1, u8Vol2);
                    break;
                default:
                    break;
            }
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SOUND_SetMixModeMute() @@VVV
/// @brief \b Function \b Description: This routine is used to set audio mix mode S/W mute.
/// @param <IN>        \b eSourseType: for audio source type
/// @param <IN>        \b VolType    : for vol type
/// @param <IN>        \b bEnable    :     TRUE --Mute
///                                  :     FALSE--Unmute
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SOUND_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute )
{
    switch (eSourceType)
    {
        case E_AUDIO_INFO_KTV_IN:
            switch(VolType)
            {
                case PCM_VOL:
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_DMA_IN, EnMute);
                    break;

                case MIC_VOL:
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_MAIN, EnMute);
                    break;

                case MP3_VOL:
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_SECONDARY, EnMute);
                    break;

                default:
                    break;
            }
            break;

        case E_AUDIO_INFO_GAME_IN:
            switch(VolType)
            {
                case PCM_VOL:
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_DMA_IN, EnMute);
                    break;

                case GAME1_VOL:
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_MAIN, EnMute);
                    break;

                case GAME2_VOL:
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_SECONDARY, EnMute);
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
}

//******************************************************************************
/// @brief \b Function \b Name: HAL_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL HAL_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param )
{
    MS_U8 param1 = (MS_U8)(p_param[0]);
    MS_U8 param2 = (MS_U8)(p_param[1]);
    MS_U8 param3 = (MS_U8)(p_param[2]);
    MS_U8 param4 = (MS_U8)(p_param[3]);

    switch(Type)
    {
        case Sound_SET_Type_SetPreScale:
            HAL_SOUND_SetPreScale(param1, param2);
            break;

        case Sound_SET_Type_SetMute:
            HAL_SOUND_SetMute(param1, param2);
            break;

        case Sound_SET_Type_SetBalance:
            HAL_SOUND_SetBalance(param1);
            break;

        case Sound_SET_Type_EnableEQ:
            HAL_SOUND_EnableEQ(param1);
            break;

        case Sound_SET_Type_EnablePEQ:
            HAL_SOUND_EnablePEQ(param1);
            break;

        case Sound_SET_Type_EnableTone:
            HAL_SOUND_EnableTone(param1);
            break;

        case Sound_SET_Type_EnableAutoVolume:
            HAL_SOUND_EnableAutoVolume(param1);
            break;

        case Sound_SET_Type_EnableSurround:
            HAL_SOUND_EnableSurround(param1);
            break;

        case Sound_SET_Type_SetBass:
            HAL_SOUND_SetBass(param1);
            break;

        case Sound_SET_Type_EnableBalance:
            HAL_SOUND_EnableBalance(param1);
            break;

        case Sound_SET_Type_AbsoluteBass:
            HAL_SOUND_AbsoluteBass(param1);
            break;

        case Sound_SET_Type_SetTreble:
            HAL_SOUND_SetTreble(param1);
            break;

        case Sound_SET_Type_SetEq:
            HAL_SOUND_SetEq(param1, param2);
            break;

        case Sound_SET_Type_SetEq_7band:
            HAL_SOUND_SetEq7(param1, param2);
            break;

        case Sound_SET_Type_SetAvc:
            HAL_SOUND_SetAVCThreshold(param1);
            HAL_SOUND_SetAvcMode(param2);
            HAL_SOUND_SetAvcAT(param3);
            HAL_SOUND_SetAvcRT(param4);
            break;

        case Sound_SET_Type_SetAudioDelay:
            HAL_SOUND_SetCH1AudioDelay(param1);
            break;

        case Sound_SET_Type_SetSurround:
            HAL_SOUND_SetSurroundXA(param1);
            HAL_SOUND_SetSurroundXB(param2);
            HAL_SOUND_SetSurroundXK(param3);
            HAL_SOUND_SetSurroundLPFGain(param4);
            break;

        case Sound_SET_Type_ConvertVolumeUnit:
            break;

        case Sound_SET_Type_SetDCOffet:
            HAL_AUDIO_WriteByte(0x2B50 ,param1);
            HAL_AUDIO_WriteByte(0x2B51 ,param2);
            break;

        case Sound_SET_Type_EnableSineTone:
        default:
            break;
    }
    return TRUE;
}

//******************************************************************************
/// @brief \b Function \b Name: HAL_SND_ProcessEnable()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
//******************************************************************************
MS_BOOL HAL_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
{
    MS_BOOL status = FALSE;

    switch(Type)
    {
        case Sound_ENABL_Type_EQ:
            HAL_SOUND_EnableEQ(enable);
            status = TRUE;
            break;

        case Sound_ENABL_Type_PEQ:
            HAL_SOUND_EnablePEQ(enable);
            status = TRUE;
            break;

        case Sound_ENABL_Type_Tone:
            HAL_SOUND_EnableTone(enable);
            status = TRUE;
            break;

        case Sound_ENABL_Type_AutoVolume:
            HAL_SOUND_EnableAutoVolume(enable);
            status = TRUE;
            break;

        case Sound_ENABL_Type_Surround:
            HAL_SOUND_EnableSurround(enable);
            status = TRUE;
            break;

        case Sound_ENABL_Type_Balance:
            HAL_SOUND_EnableBalance(enable);
            status = TRUE;
            break;

    case Sound_ENABL_Type_DRC:
            HAL_SOUND_EnableDRC(enable);
            status = TRUE;
            break;

       case Sound_ENABL_Type_KTVEcho:
            HAL_SOUND_EnableKTVEcho(enable);
            status = TRUE;
            break;

        case Sound_ENABL_Type_EaseVol:
            HAL_SOUND_EnableEaseVol(enable);
            status = TRUE;
            break;

        default:
            status = FALSE;
            break;
    }
    return(status);
}


//******************************************************************************
/// @brief \b Function \b Name: HAL_SND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL HAL_SND_SetParam(Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
{
    MS_BOOL status = FALSE;
    MS_U16  data = 0;

    switch(Type)
    {
        case Sound_SET_PARAM_PreScale:
            HAL_SOUND_SetPreScale(param1, param2);      //param1: path, param2:prescale
            status = TRUE;
            break;

        case Sound_SET_PARAM_Balance:
            HAL_SOUND_SetBalance(param1);               // 0~100
            status = TRUE;
            break;

        case Sound_SET_PARAM_EQ:
            HAL_SOUND_SetEq(param1, param2);            //param1: band 0~4, param2:level 0~100
            break;

        case Sound_SET_PARAM_Surround_XA:
            HAL_SOUND_SetSurroundXA(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Surround_XB:
            HAL_SOUND_SetSurroundXB(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Surround_XK:
            HAL_SOUND_SetSurroundXK(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Surround_LPFGAIN:
            HAL_SOUND_SetSurroundLPFGain(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Treble:
            HAL_SOUND_SetTreble(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Bass:
            HAL_SOUND_SetBass(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_AbsoluteBass:
            HAL_SOUND_AbsoluteBass(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Avc_Mode:
            HAL_SOUND_SetAvcMode(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_NR_Threshold:
            HAL_AUDIO_WriteByte(0x2D32, (MS_U8) param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Avc_Threshold:
            HAL_SOUND_SetAVCThreshold(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Avc_AT:
            HAL_SOUND_SetAvcAT(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_Avc_RT:
            HAL_SOUND_SetAvcRT(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_AudioDelay:
            HAL_SOUND_SetCH1AudioDelay(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_DCOffet:
            HAL_AUDIO_WriteByte(0x2B51, param1);
            HAL_AUDIO_WriteByte(0x2B50, param2);
            status = TRUE;
            break;

        case Sound_SET_PARAM_PEQ_48K_A0:
            if((param1>>8) > MAX_PEQ_BAND-1)
                printf("PEQ paramater Band should be 0~4\n");
            else
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peq48KCoeffAddr+(param1>>8)*5, ((param1&0xFF)<<16)+param2);
            break;

        case Sound_SET_PARAM_PEQ_48K_A1:
            if((param1>>8) > MAX_PEQ_BAND-1)
                printf("PEQ paramater Band should be 0~4\n");
            else
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peq48KCoeffAddr+(param1>>8)*5+1, ((param1&0xFF)<<16)+param2);
            break;

        case Sound_SET_PARAM_PEQ_48K_A2:
            if((param1>>8) > MAX_PEQ_BAND-1)
                printf("PEQ paramater Band should be 0~4\n");
            else
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peq48KCoeffAddr+(param1>>8)*5+2, ((param1&0xFF)<<16)+param2);
            break;

        case Sound_SET_PARAM_PEQ_48K_B1:
            if((param1>>8) > MAX_PEQ_BAND-1)
                printf("PEQ paramater Band should be 0~4\n");
            else
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peq48KCoeffAddr+(param1>>8)*5+3, ((param1&0xFF)<<16)+param2);
            break;

        case Sound_SET_PARAM_PEQ_48K_B2:
            if((param1>>8) > MAX_PEQ_BAND-1)
                printf("PEQ paramater Band should be 0~4\n");
            else
                HAL_MAD2_Write_DSP_Xbox(DSP2XboxAddr_peq48KCoeffAddr+(param1>>8)*5+4, ((param1&0xFF)<<16)+param2);
            break;

        case Sound_SET_PARAM_PEQ_32K_A0:
            // No support 32K PEQ in T12
            break;

        case Sound_SET_PARAM_PEQ_32K_A1:
            // No support 32K PEQ in T12
            break;

        case Sound_SET_PARAM_PEQ_32K_A2:
            // No support 32K PEQ in T12
            break;

        case Sound_SET_PARAM_PEQ_32K_B1:
            // No support 32K PEQ in T12
            break;

        case Sound_SET_PARAM_PEQ_32K_B2:
            // No support 32K PEQ in T12
            break;

        case Sound_SET_PARAM_AbsoluteEQ:
            if(param1 > 4)
                printf("GEQ  Band should be 0~4\n");
            else
            {
                HAL_AUDIO_WriteByte(REG_SOUND_EQ1 + (param1*2), (MS_U8)param2);
            }
            break;

        case Sound_SET_PARAM_Drc_Threshold:
            HAL_SOUND_SetDRCThreshold(param1);
            status = TRUE;
            break;

        case Sound_SET_PARAM_DMAReader:
            if(param1 == 0)
            {
                if(param2 >= 4)
                {
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_DMA_IN, true);
                }
                else
                {
                    data = 0x0C + param2*4;
                    HAL_SOUND_AbsoluteVolume(AUDIO_T3_PATH_MIXER_DMA_IN,data,0);
                    HAL_SOUND_SetMute(AUDIO_T3_PATH_MIXER_DMA_IN, false);
                }
                status = TRUE;
            }
            break;

        case Sound_SET_PARAM_Ease_Type_A:
        case Sound_SET_PARAM_Ease_Type_B:
        {
            MS_U32 u32EaseType = AUDIO_EASE_TYPE_LINEAR;
            MS_U32 u32DspPmEaseBase = 0;

            status = TRUE;

            switch (Type)
            {
                case Sound_SET_PARAM_Ease_Type_A:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    break;
                }

                case Sound_SET_PARAM_Ease_Type_B:
                default:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 4;
                    break;
                }
            }

            switch (param1)
            {
                case E_AUDIO_EASE_TYPE_LINEAR:
                {
                    u32EaseType = AUDIO_EASE_TYPE_LINEAR;
                    break;
                }

                case E_AUDIO_EASE_TYPE_INCUBIC:
                {
                    u32EaseType = AUDIO_EASE_TYPE_INCUBIC;
                    break;
                }

                case E_AUDIO_EASE_TYPE_OUTCUBIC:
                {
                    u32EaseType = AUDIO_EASE_TYPE_OUTCUBIC;
                    break;
                }

                case E_AUDIO_EASE_TYPE_INOUTCUBIC:
                {
                    u32EaseType = AUDIO_EASE_TYPE_INOUTCUBIC;
                    break;
                }

                default:
                {
                    HALSOUND_ERROR("%s: invalid audio ease type %d\n", __FUNCTION__, param1);
                    status = FALSE;
                    break;
                }
            }

            if (status == TRUE)
            {
                HAL_MAD2_Write_DSP_Xbox((u32DspPmEaseBase + 2), u32EaseType);
            }

            break;
        }

        case Sound_SET_PARAM_Ease_Duration_A:
        case Sound_SET_PARAM_Ease_Duration_B:
        {
            MS_U32 u32MiniSecond = 0;
            MS_U32 u32SamplesOfMiniSecond = 0;
            MS_U32 u32DspPmEaseBase = 0;

            status = TRUE;

            switch (Type)
            {
                case Sound_SET_PARAM_Ease_Duration_A:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    break;
                }

                case Sound_SET_PARAM_Ease_Duration_B:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 4;
                    break;
                }

                default:
                {
                    HALSOUND_ERROR("%s: invalid audio ease command %d\n", __FUNCTION__, Type);
                    status = FALSE;
                    break;
                }
            }

            if (status == TRUE)
            {
                if (param1 > 60000)
                {
                    HALSOUND_ERROR("%s: target duration (%d) of audio ease is too large!\n", __FUNCTION__, param1);
                    u32MiniSecond = 60000;
                }
                else
                {
                    u32MiniSecond = (MS_U32)param1;
                }

                u32SamplesOfMiniSecond = u32MiniSecond * 48; /* samples of 1 ms in 48KHz */

                HAL_MAD2_Write_DSP_Xbox((u32DspPmEaseBase + 1), u32SamplesOfMiniSecond);
            }

            break;
        }

        case Sound_SET_PARAM_Ease_Gain_A:
        case Sound_SET_PARAM_Ease_Gain_B:
        {
            MS_U32 u32Gain = 0;
            MS_U32 u32DspPmEaseBase = 0;
            MS_U32 u32DspPmUpdateFlag = 0;

            status = TRUE;

            switch (Type)
            {
                case Sound_SET_PARAM_Ease_Gain_A:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    u32DspPmUpdateFlag = DSP2XboxAddr_easeAUpdateFlag;
                    break;
                }

                case Sound_SET_PARAM_Ease_Gain_B:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 4;
                    u32DspPmUpdateFlag = DSP2XboxAddr_easeBUpdateFlag;
                    break;
                }

                default:
                {
                    HALSOUND_ERROR("%s: invalid audio ease command %d\n", __FUNCTION__, Type);
                    status = FALSE;
                    break;
                }
            }

            if (status == TRUE)
            {
                u32Gain = (MS_U32)(((param1 & 0xFF) << 16) | (param2 & 0xFFFF));
                if (u32Gain > 0x7FFFFF)
                {
                    HALSOUND_ERROR("%s: target gain (0x%X) of audio ease is too large!\n", __FUNCTION__, (unsigned int)u32Gain);
                    u32Gain = 0x7FFFFF;
                }
                HAL_MAD2_Write_DSP_Xbox(u32DspPmEaseBase, u32Gain);

                /* trigger Audio Ease to apply current settings, this call should only be here! */
                HAL_MAD2_Write_DSP_Xbox(u32DspPmUpdateFlag, 0x1);
            }

            break;
        }

        case Sound_SET_PARAM_Ease_Target_Channel_A:
        case Sound_SET_PARAM_Ease_Target_Channel_B:
        {
            MS_U32 u32Mask = 0x000000;
            MS_U32 u32TargetChannel = AUDIO_EASE_CH_NONE;
            MS_U32 u32ChannelSetting = 0;
            MS_U32 u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 8;

            status = TRUE;

            if (param2 == TRUE)
            {
                switch (Type)
                {
                    case Sound_SET_PARAM_Ease_Target_Channel_A:
                    {
                        u32TargetChannel = AUDIO_EASE_CH_A;
                        break;
                    }

                    case Sound_SET_PARAM_Ease_Target_Channel_B:
                    default:
                    {
                        u32TargetChannel = AUDIO_EASE_CH_B;
                        break;
                    }
                }
            }
            else
            {
                u32TargetChannel = AUDIO_EASE_CH_NONE;
            }

            switch (param1)
            {
                case AUDIO_PATH_4:
                {
                    /* for Channel 5 */
                    u32Mask = 0x000F;
                    break;
                }

                case AUDIO_PATH_5:
                {
                    /* for Channel 6 */
                    u32Mask = 0x0000F0;
                    u32TargetChannel <<= 4;
                    break;
                }

                case AUDIO_PATH_6:
                {
                    /* for Channel 7 */
                    u32Mask = 0x000F00;
                    u32TargetChannel <<= 8;
                    break;
                }

                case AUDIO_PATH_7:
                {
                    /* for Channel 8 */
                    u32Mask = 0x00F000;
                    u32TargetChannel <<= 12;
                    break;
                }

                case AUDIO_PATH_0:
                {
                    /* for Channel 1~4 (Multi-Channel) */
                    u32Mask = 0x0F0000;
                    u32TargetChannel <<= 16;
                    break;
                }

                default:
                {
                    HALSOUND_ERROR("%s: invalid audio ease path %d\n", __FUNCTION__, param1);
                    status = FALSE;
                    break;
                }
            }

            if (status == TRUE)
            {
                u32ChannelSetting = HAL_MAD2_Read_DSP_Xbox(u32DspPmEaseBase);
                u32ChannelSetting = (u32ChannelSetting & (~u32Mask)) | u32TargetChannel;
                HAL_MAD2_Write_DSP_Xbox(u32DspPmEaseBase, u32ChannelSetting);
            }

            break;
        }

        default:
            status = FALSE;
            break;
    }
    return(status);
}



//******************************************************************************
/// @brief \b Function \b Name: HAL_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_U16 HAL_SND_GetParam( Sound_GET_PARAM_Type Type, MS_U16 param1)
{
    MS_U16 tmp=0;
    switch(Type)
    {
        case Sound_GET_PARAM_PreScale:
            tmp = HAL_SOUND_GetPreScale(param1);                           //param1: path
            break;

        case Sound_GET_PARAM_Balance_L:
            tmp = HAL_SOUND_GetBalance_L();                                // 0~0xFF
            break;

        case Sound_GET_PARAM_Balance_R:
            tmp = HAL_SOUND_GetBalance_R();                                // 0~0xFF
            break;

        case Sound_GET_PARAM_EQ:
           tmp = HAL_SOUND_GetEq(param1);                                  //param1: band 0~4
            break;

        case Sound_GET_PARAM_Surround_XA:
            tmp = HAL_SOUND_GetSurroundXA();
            break;

        case Sound_GET_PARAM_Surround_XB:
            tmp = HAL_SOUND_GetSurroundXB();
            break;

        case Sound_GET_PARAM_Surround_XK:
            tmp = HAL_SOUND_GetSurroundXK();
            break;

        case Sound_GET_PARAM_Surround_LPFGAIN:
            tmp = HAL_SOUND_GetSurroundLPFGain();
            break;

        case Sound_GET_PARAM_Treble:
            tmp = HAL_SOUND_GetTreble();
            break;

        case Sound_GET_PARAM_Bass:
            tmp = HAL_SOUND_GetBass();
            break;

        case Sound_GET_PARAM_Avc_Mode:
            tmp = HAL_SOUND_GetAvcMode();
            break;

        case Sound_GET_PARAM_Avc_Threshold:
            tmp = HAL_SOUND_GetAVCThreshold();
            break;

        case Sound_GET_PARAM_Avc_AT:
            tmp = HAL_SOUND_GetAvcAT();
            break;

        case Sound_GET_PARAM_Avc_RT:
            tmp = HAL_SOUND_GetAvcRT();
            break;

        case Sound_GET_PARAM_AudioDelay:
            tmp = HAL_SOUND_GetCH1AudioDelay();
            break;

        case Sound_GET_PARAM_DCOffet:
            tmp = HAL_AUDIO_ReadByte(0x2B51);
            tmp = ((MS_U16)HAL_AUDIO_ReadByte(0x2B50) |(tmp<<8));
            break;

        case Sound_GET_PARAM_NR_Threshold:
            tmp = HAL_AUDIO_ReadByte(0x2D24);
            break;

        case Sound_GET_PARAM_NR_Status:
           // tmp = (HAL_AUDIO_ReadByte(0x2A78) & 0x01);
            break;

        case Sound_GET_PARAM_getSignal_Energy:
          //  tmp = HAL_MAD_Read_DSP_sram(0x3F2B, DSP_MEM_TYPE_DM);
            break;

        case Sound_GET_PARAM_EQ_Status:
            tmp = HAL_SOUND_GetEQ_Status();
            break;

        case Sound_GET_PARAM_PEQ_Status:
            tmp = HAL_SOUND_GetPEQ_Status();
            break;

        case Sound_GET_PARAM_Tone_Status:
            tmp = HAL_SOUND_GetTone_Status();
            break;

        case Sound_GET_PARAM_AutoVolume_Status:
            tmp = HAL_SOUND_GetAutoVolume_Status();
            break;

        case Sound_GET_PARAM_Surround_Status:
            tmp = HAL_SOUND_GetSurround_Status();
            break;

        case Sound_GET_PARAM_DMAReader_Buffer_Addr:
            tmp = (MS_U16)(DMA_READER_TEMP_BUFFER_ADDR>>8);
            break;

        case Sound_GET_PARAM_Ease_Type_A:
        case Sound_GET_PARAM_Ease_Type_B:
        {
            MS_U32 u32EaseType = AUDIO_EASE_TYPE_LINEAR;
            MS_U32 u32DspPmEaseBase = 0;

            switch (Type)
            {
                case Sound_GET_PARAM_Ease_Type_A:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    break;
                }

                case Sound_GET_PARAM_Ease_Type_B:
                default:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 4;
                    break;
                }
            }

            u32EaseType = HAL_MAD2_Read_DSP_Xbox((u32DspPmEaseBase + 2));

            switch (u32EaseType)
            {
                case AUDIO_EASE_TYPE_LINEAR:
                {
                    tmp = E_AUDIO_EASE_TYPE_LINEAR;
                    break;
                }

                case AUDIO_EASE_TYPE_INCUBIC:
                {
                    tmp = E_AUDIO_EASE_TYPE_INCUBIC;
                    break;
                }

                case AUDIO_EASE_TYPE_OUTCUBIC:
                {
                    tmp = E_AUDIO_EASE_TYPE_OUTCUBIC;
                    break;
                }

                case AUDIO_EASE_TYPE_INOUTCUBIC:
                {
                    tmp = E_AUDIO_EASE_TYPE_INOUTCUBIC;
                    break;
                }

                default:
                {
                    HALSOUND_ERROR("%s: invalid audio ease type %u\n", __FUNCTION__, (unsigned int)u32EaseType);
                    break;
                }
            }

            break;
        }

        case Sound_GET_PARAM_Ease_Duration_A:
        case Sound_GET_PARAM_Ease_Duration_B:
        {
            MS_U32 u32MiniSecond = 0;
            MS_U32 u32DspPmEaseBase = 0;

            switch (Type)
            {
                case Sound_GET_PARAM_Ease_Duration_A:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    break;
                }

                case Sound_GET_PARAM_Ease_Duration_B:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 4;
                    break;
                }

                default:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    break;
                }
            }

            u32MiniSecond = HAL_MAD2_Read_DSP_Xbox((u32DspPmEaseBase + 1)) / 48;
            tmp = (MS_U16)u32MiniSecond;

            break;
        }

        case Sound_GET_PARAM_Ease_Gain_A:
        case Sound_GET_PARAM_Ease_Gain_B:
        {
            MS_U32 u32Value;
            MS_U32 u32DspPmEaseBase = 0;

            switch (Type)
            {
                case Sound_GET_PARAM_Ease_Gain_A:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr;
                    break;
                }

                case Sound_GET_PARAM_Ease_Gain_B:
                default:
                {
                    u32DspPmEaseBase = DSP2XboxAddr_VolEaseAddr + 4;
                    break;
                }
            }

            u32Value = HAL_MAD2_Read_DSP_Xbox((u32DspPmEaseBase + 3));
            tmp = (MS_U16)(u32Value >> 8);

            break;
        }

        default:
            break;
    }
    return tmp;

}

