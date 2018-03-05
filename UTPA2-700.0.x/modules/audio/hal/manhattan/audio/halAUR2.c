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
// Internal Definition
#include "regCHIP.h"
#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halAUR2.h"
#include "r2_shm_comm.h"
#include"decR2_shm.h"
#if ASND_R2_SUPPORT
#include"sndR2_shm.h"
#endif

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#define code
#define BYTE MS_U8

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define AUR2_ERRMSG(msg)  MS_CRITICAL_MSG(msg)
#define AUR2_DBGMSG(msg)  MS_DEBUG_MSG(msg)

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HINIBBLE(u8Data)    ((u8Data) >> 4)
#define LONIBBLE(u8Data)    ((u8Data) << 4)

#define R2_0_DDR_BASE                (HAL_AUDIO_GetDspMadBaseAddr(2)+ADEC__R2_DDR_SIZE)             //SND_R2 base
#define REG_R2_0_CTRL_BASE           0x112900
#define REG_R2_0_MAILBOX_BASE        0x112E00
#define REG_R2_0_CTRL                REG_R2_0_CTRL_BASE+0x80
#define REG_R2_0_PARAM_TYPE          REG_R2_0_MAILBOX_BASE+0x80
#define REG_R2_0_ID_SELECT           REG_R2_0_MAILBOX_BASE+0x82
#define REG_R2_0_PARAM_VAL1          REG_R2_0_MAILBOX_BASE+0x84
#define REG_R2_0_PARAM_VAL2          REG_R2_0_MAILBOX_BASE+0x86
#define REG_R2_0_CMD_TOKEN           REG_R2_0_MAILBOX_BASE+0xA3
#define REG_R2_0_INFO1               REG_R2_0_MAILBOX_BASE+0xA4
#define REG_R2_0_INFO2               REG_R2_0_MAILBOX_BASE+0xA6

#define R2_1_DDR_BASE                (HAL_AUDIO_GetDspMadBaseAddr(2))                                               //DEC_R2 base
#define REG_R2_1_CTRL_BASE           0x163000
#define REG_R2_1_MAILBOX_BASE        0x160300
#define REG_R2_1_CTRL                REG_R2_1_CTRL_BASE+0x80
#define REG_R2_1_PARAM_TYPE          REG_R2_1_MAILBOX_BASE+0x80
#define REG_R2_1_ID_SELECT           REG_R2_1_MAILBOX_BASE+0x82
#define REG_R2_1_PARAM_VAL1          REG_R2_1_MAILBOX_BASE+0x84
#define REG_R2_1_PARAM_VAL2          REG_R2_1_MAILBOX_BASE+0x86
#define REG_R2_1_CMD_TOKEN           REG_R2_1_MAILBOX_BASE+0xA3
#define REG_R2_1_INFO1               REG_R2_1_MAILBOX_BASE+0xA4
#define REG_R2_1_INFO2               REG_R2_1_MAILBOX_BASE+0xA6

#define AUR2_DEBUG_PRINT(x)         //x

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
extern MS_VIRT _gMIO_MapBase;
extern MS_S32  _s32AUDIOMutexIDMA;
extern AUDIO_SHARED_VARS2 * g_AudioVars2;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

/////////////`///////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUR2_ReadByte
/// @brief \b Function  \b Description: read 1 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U8 : 8-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U8 HAL_AUR2_ReadByte(MS_U32 u32RegAddr)
{
    return  HAL_AUDIO_AbsReadByte(u32RegAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUR2_ReadReg
/// @brief \b Function  \b Description: read 2 Byte data
/// @param <IN>         \b u32RegAddr: register address
/// @param <OUT>        \b None :
/// @param <RET>        \b MS_U16 : 16-bit register value
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
MS_U16 HAL_AUR2_ReadReg(MS_U32 u32RegAddr)
{
    return HAL_AUDIO_AbsReadReg(u32RegAddr);  // Add  audio bank offset
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUR2_WriteByte
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 8-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUR2_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val)
{
    HAL_AUDIO_AbsWriteByte(u32RegAddr, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUR2_WriteReg
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 16-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUR2_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val)
{
    HAL_AUDIO_AbsWriteReg(u32RegAddr, u16Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUR2_WriteMaskByte
/// @brief \b Function  \b Description: Mask write 8-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 8-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUR2_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val)
{
    HAL_AUDIO_AbsWriteMaskByte(u32RegAddr, u8Mask, u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name: HAL_AUR2_WriteMaskReg
/// @brief \b Function  \b Description: Mask write 16-bit data
/// @param <IN>         \b u32RegAddr: register address
/// @param <IN>         \b u16Val : 16-bit data
/// @param <OUT>        \b None :
/// @param <RET>        \b None :
/// @param <GLOBAL>     \b None :
////////////////////////////////////////////////////////////////////////////////
void HAL_AUR2_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val)
{
    HAL_AUDIO_AbsWriteMaskReg(u32RegAddr, u16Mask, u16Val);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SND_R2_EnableR2() / HAL_DEC_R2_EnableR2()
/// @brief \b Function \b Description: This routine load Audio R2 code
/// @param <IN>        \b NONE  :
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b NONE  : (OK) TRUE / (FAIL) FALE
/// @param <GLOBAL>    \b NONE  :
////////////////////////////////////////////////////////////////////////////////
void HAL_SND_R2_EnableR2( MS_BOOL bEnable)
{
    if (bEnable)
    {
        HAL_AUR2_WriteMaskByte(REG_R2_0_CTRL, 0xFF, 0x27);
        HAL_AUR2_WriteMaskByte(0x112940, 0xFF, 0x10);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x01, 0x01);        //BANK 1129 control by R2
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x01, 0x00);        //BANK 1129 control by RIU
        HAL_AUR2_WriteMaskByte(REG_R2_0_CTRL, 0xFF, 0x00);
    }
}

void HAL_DEC_R2_EnableR2(MS_BOOL bEnable )
{
    if (bEnable)
    {
        HAL_AUR2_WriteMaskByte(REG_R2_1_CTRL, 0xFF, 0x27);
        HAL_AUR2_WriteMaskByte(0x163040, 0xFF, 0x10);
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x02, 0x02);        //BANK 1630 control by R2
    }
    else
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x02, 0x00);        //BANK 1630 control by MIU
        HAL_AUR2_WriteMaskByte(REG_R2_1_CTRL, 0xFF, 0x00);
    }
}

void HAL_DEC_R2_init_SHM_param( void )
{
    MS_S32 dec_id;
    DEC_R2_SHARE_MEM  *p_shm = (DEC_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_1_DDR_BASE+DEC_R2_SHM_DDR_OFFSET));
    AUR2_ADEC_PARAM_SHM     *p_shm_param;
    AUR2_ADEC_ES_PARAM_SHM  *p_shm_esparam;

    p_shm->RfSignalType = 0;

    for (dec_id = 0; dec_id < MAX_ADEC; dec_id++ )
    {
        AUR2_DOLBY_PARAM *pDolby;
        AUR2_DTS_PARAM *pDTS;
        AUR2_MPEG_PARAM *pMPEG;

        p_shm_param = &p_shm->adec_param_shm[dec_id];

        if (dec_id == 0)
            p_shm_esparam = &p_shm->adec_esParam_shm[0];        //dec0, use es0
        else
            p_shm_esparam = &p_shm->adec_esParam_shm[2];        //dec1, use es2

        pDolby  = &p_shm_param->dolby_param;
        pDTS = &p_shm_param->dts_param;
        pMPEG = &p_shm_param->mpeg_param;

        MsOS_Dcache_Flush((MS_VIRT) &(p_shm_param->bootMagicID), sizeof(MS_U32));

        if ((g_AudioVars2->g_R2ShmInitFlag == FALSE) ||
            (p_shm_param->bootMagicID != SHM_PARAM_BOOT_MAGIC_ID))
        {
            memset(p_shm_param, 0, sizeof(AUR2_ADEC_PARAM_SHM));
            memset(p_shm_esparam, 0, sizeof(AUR2_ADEC_ES_PARAM_SHM));

            p_shm_esparam->ES_wrPtr = 0;
            p_shm_esparam->MM_inputPTS = 0;

            p_shm_param->dtv_mpg_avDelay = 0x3E1E;
            p_shm_param->dtv_ac3_avDelay = 0x4300;
            p_shm_param->dtv_aac_avDelay = 0x327E;

            p_shm_param->adMix_mainVol = 0x60;          //0 dB
            p_shm_param->adMix_asocVol = 0x60;          //0 dB

            p_shm_param->mmTs_ctrl_flag = 0;
            p_shm_param->mmUni_needDecodeFrmCnt = 1;
            p_shm_param->OMX_SPDIF_Ctrl = 0;
            p_shm_param->OMX_SPDIF_PCM_Level = 0x1200;

            p_shm_param->mmFileReqSize = 0;
            p_shm_param->bootMagicID = SHM_PARAM_BOOT_MAGIC_ID;
            p_shm_param->decCtrl = 0;
            p_shm_param->hashKey = 0;

            p_shm_param->extSynthsizer_en = 0;
            p_shm_param->extSynthsizer_value = 0x11940000;
            p_shm_param->fast_forward = 0;

            p_shm_param->ES_Limiter_EN = 0;
            p_shm_param->ES_Limiter_Threshold= 0x2000;
            p_shm_param->PCM_Limiter_EN = 0;
            p_shm_param->PCM_Limiter_Threshold = 0x4B00;

            /* init Dolby param */
            pDolby->high_cut = 100;
            pDolby->low_boost = 100;
            pDolby->dmxMode = 0;
            pDolby->drcMode = 1;
            pDolby->gain = 0x7FFFFFFF;
            pDolby->mul_frame_header = 0;
            pDolby->TB11_enable = 1;
            pDolby->DDP_HDMI_bypass = 1;

            /* init DTS param */
            pDTS->drcMode = 0;
            pDTS->dmxLfeEnable = 0;
            pDTS->dialnormEnable = 0;
            pDTS->certMode = 0;
            pDTS->dmxMode = 0;

            /* init MPEG param */
            pMPEG->MPEG_soundMode = 0;
            pMPEG->gain = 0x7FFFFFFF;

            printf("[======= initialize DEC-R2[%d] SHM =======]\n", (int)dec_id);
        }
    }

    MsOS_Dcache_Flush((MS_VIRT)p_shm, sizeof(DEC_R2_SHARE_MEM));

    g_AudioVars2->g_R2ShmInitFlag = TRUE;

}

///////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_sndR2_SetCommInfo() / HAL_decR2_SetCommInfo()
/// @brief \b Function \b Description: This routine set different paramter to audio decoder
/// @param <IN>        \b AudioR2_COMM_infoType    : common audio parameter type
/// @param <IN>        \b MS_U32    : common audio parameter 1
/// @param <IN>        \b MS_U32    : common audio parameter 2
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_BOOL :    TRUE ( SUCCESS ) / FALSE (FAIL)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void HAL_SND_R2_SetCommInfo(R2_AudioParamType infoType, Audio_id id, MS_U16 param1, MS_U16 param2)
{
    MS_U16  tmpVal, token, i=0;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    token = HAL_AUR2_ReadByte( REG_R2_0_CMD_TOKEN );

    tmpVal = (id&0xFF) | 0x100;
    HAL_AUR2_WriteReg( REG_R2_0_PARAM_VAL1, param1 );
    HAL_AUR2_WriteReg( REG_R2_0_PARAM_VAL2, param2 );
    HAL_AUR2_WriteReg( REG_R2_0_ID_SELECT,  tmpVal );
    HAL_AUR2_WriteReg( REG_R2_0_PARAM_TYPE, infoType );

    while( i < 500 )
    {
        if (token != HAL_AUR2_ReadByte( REG_R2_0_CMD_TOKEN ))
            break;

        i++;
        AUDIO_DELAY1US(20);
    }

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    AUR2_DEBUG_PRINT(printf("[%s] infoType:%x, id:%x, param: [%04x, %04x]\n", __FUNCTION__, infoType, id, param1, param2));
}

void HAL_DEC_R2_SetCommInfo(R2_AudioParamType infoType, Audio_id id, MS_U16 param1, MS_U16 param2)
{
    MS_U16  tmpVal, token, i=0;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    token = HAL_AUR2_ReadByte( REG_R2_1_CMD_TOKEN );

    tmpVal = (id&0xFF) | 0x100;
    HAL_AUR2_WriteReg( REG_R2_1_PARAM_VAL1, param1 );
    HAL_AUR2_WriteReg( REG_R2_1_PARAM_VAL2, param2 );
    HAL_AUR2_WriteReg( REG_R2_1_ID_SELECT,  tmpVal );
    HAL_AUR2_WriteReg( REG_R2_1_PARAM_TYPE, infoType );

    while( i < 500 )
    {
        if (token != HAL_AUR2_ReadByte( REG_R2_1_CMD_TOKEN ))
            break;

        i++;
        AUDIO_DELAY1US(20);
    }

    #if 0
    if (i >= 10)
    {
        HAL_AUDIO_WriteMaskByte(REG_AUDIO_SYNTH_EXPANDER, 0x02, 0x00);        //BANK 1630 control by MIU
        printf("R2 PC:");
        for( i=0; i<10; i++)
        {
            MS_U16 tmp_H, tmp_L;
            tmp_H = HAL_AUR2_ReadReg(0x163012);
            tmp_L = HAL_AUR2_ReadReg(0x163010);
            printf("[%04x%04x],", tmp_H, tmp_L);
        }

        HALAUDIO_PRINT("\n[%s] infoType:%x, id:%x, param: [%04x, %04x] fail\n", __FUNCTION__, infoType, id, param1, param2);
    }
    #endif

    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    AUR2_DEBUG_PRINT(printf("[%s] infoType:%x, id:%x, param: [%04x, %04x]\n", __FUNCTION__, infoType, id, param1, param2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SND_R2_GetCommInfo() / HAL_DEC_R2_GetCommInfo()
/// @brief \b Function \b Description: This routine info of audio decoders
/// @param <IN>        \b AudioR2_COMM_infoType    : request info type of audio decoder
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b long long :  return info
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 HAL_SND_R2_GetCommInfo(R2_AudioInfoType infoType, Audio_id id, MS_U16 param1, MS_U16 param2)
{
    MS_U16  tmpVal, token, i=0;
    MS_U32  ret_val;

    token = HAL_AUR2_ReadByte( REG_R2_0_CMD_TOKEN );

    tmpVal = (id&0xFF);
    HAL_AUR2_WriteReg( REG_R2_0_PARAM_VAL1, param1 );
    HAL_AUR2_WriteReg( REG_R2_0_PARAM_VAL2, param2 );
    HAL_AUR2_WriteReg( REG_R2_0_ID_SELECT,  tmpVal );
    HAL_AUR2_WriteReg( REG_R2_0_PARAM_TYPE, infoType );

    token = token + 0x01;               //if send cmd successfully, token number should increase
    while( i < 500 )
    {
        if (token == HAL_AUR2_ReadByte( REG_R2_0_CMD_TOKEN ))
            break;

        i++;
        AUDIO_DELAY1US(20);
    }

    if ( i >= 10 )
    {
        printf("%s:%x, %x, %x, %x cmd fail\n",__FUNCTION__, infoType, id, param1, param2);
        return 0;
    }

    ret_val = (HAL_AUR2_ReadReg(REG_R2_0_INFO1)<<16) + (HAL_AUR2_ReadReg(REG_R2_0_INFO2));

    AUR2_DEBUG_PRINT(printf("[%s] infoType:%4x, id:%x, param: [%04x, %04x] ==> %lx\n", __FUNCTION__, infoType, id, param1, param2, ret_val));
    return ret_val;
}

MS_U32 HAL_DEC_R2_GetCommInfo(R2_AudioInfoType infoType, Audio_id id, MS_U16 param1, MS_U16 param2)
{
    MS_U16  tmpVal, token, i=0;
    MS_U32 ret_val;

    OS_OBTAIN_MUTEX(_s32AUDIOMutexIDMA, MSOS_WAIT_FOREVER);

    token = HAL_AUR2_ReadByte( REG_R2_1_CMD_TOKEN );

    tmpVal = (id&0xFF);
    HAL_AUR2_WriteReg( REG_R2_1_PARAM_VAL1, param1 );
    HAL_AUR2_WriteReg( REG_R2_1_PARAM_VAL2, param2 );
    HAL_AUR2_WriteReg( REG_R2_1_ID_SELECT,  tmpVal );
    HAL_AUR2_WriteReg( REG_R2_1_PARAM_TYPE, infoType );

    token = token + 0x01;               //if send cmd successfully, token number should increase
    while( i < 500 )
    {
        if (token == HAL_AUR2_ReadByte( REG_R2_1_CMD_TOKEN ))
            break;

        i++;
        AUDIO_DELAY1US(20);
    }

    if ( i >= 10 )
    {
        printf("%s:%x, %x, %x, %x cmd fail\n",__FUNCTION__, infoType, id, param1, param2);
        OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);
        return 0;
    }

    ret_val = (HAL_AUR2_ReadReg(REG_R2_1_INFO1)<<16) + (HAL_AUR2_ReadReg(REG_R2_1_INFO2));
    OS_RELEASE_MUTEX(_s32AUDIOMutexIDMA);

    AUR2_DEBUG_PRINT(printf("[%s] infoType:%4x, id:%x, param: [%04x, %04x] ==> %lx\n", __FUNCTION__, infoType, id, param1, param2, ret_val));
    return ret_val;
}

MS_U32 HAL_SND_R2_Get_SHM_INFO(R2_SHM_INFO_TYPE decInfoType, MS_U8 dec_id )
{
    MS_U32 ret_value = 0;

#if 0
    volatile MS_U32 *ptr;
    SND_R2_SHARE_MEM *p_shm = (SND_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_0_DDR_BASE+SND_R2_SHM_DDR_OFFSET));

    switch( decInfoType )
    {
        default:
            return 0;
    }

    //D-cache Invalidate
    MsOS_Dcache_Flush((MS_VIRT) ptr, sizeof(MS_U32));
    ret_value = *ptr;
#endif

    AUR2_DEBUG_PRINT(printf("[%s] shmType:%4x, id:%x ==> %x\n", __FUNCTION__, decInfoType, dec_id, ret_value));

    return ret_value;
}



MS_U32 HAL_DEC_R2_Get_SHM_INFO(R2_SHM_INFO_TYPE decInfoType, MS_U8 dec_id )
{
    MS_U32 ret_value;
    volatile MS_U32 *ptr;
    DEC_R2_SHARE_MEM  *p_shm = (DEC_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_1_DDR_BASE+DEC_R2_SHM_DDR_OFFSET));
    AUR2_ADEC_INFO_SHM      *p_shm_info = &p_shm->adec_info_shm[dec_id];

    MS_U8  es_id = 0;
    if (dec_id == 1)
        es_id = 2;

    AUR2_ADEC_ES_INFO_SHM   *p_shm_esinfo = &p_shm->adec_esInfo_shm[es_id];

    switch( decInfoType )
    {
        case R2_SHM_INFO_CAPABILITY:    ptr = (volatile MS_U32 *) &p_shm_info->capability;                                break;
        case R2_SHM_INFO_DEC_STAUS:     ptr = (volatile MS_U32 *) &p_shm_info->decStatus;                                 break;
        case R2_SHM_INFO_DEC_ERROR_ID:  ptr = (volatile MS_U32 *) &p_shm_info->dec_error_id;                              break;
        case R2_SHM_INFO_SMP_RATE:  ptr = (volatile MS_U32 *) &p_shm_info->smpRate;                                       break;
        case R2_SHM_INFO_BIT_RATE:  ptr = (volatile MS_U32 *) &p_shm_info->bitRate;                                       break;
        case R2_SHM_INFO_SYNTH_RATE:  ptr = (volatile MS_U32 *) &p_shm_info->synthRate;                                   break;
        case R2_SHM_INFO_DEC_CHANNEL_MODE:   ptr = (volatile MS_U32 *) &p_shm_info->dec_ch_mode;                          break;
        case R2_SHM_INFO_DEC_CHANNEL_MAPPING:  ptr = (volatile MS_U32 *) &p_shm_info->dec_ch_mapping;                     break;

        case R2_SHM_INFO_ES_LEVEL:          ptr = (volatile MS_U32 *) &p_shm_esinfo->ES_Level;                             break;
        case R2_SHM_INFO_PCM_LEVEL:         ptr = (volatile MS_U32 *) &p_shm_info->PCM_Level;                                     break;
        case R2_SHM_INFO_ES_RD_PTR:         ptr = (volatile MS_U32 *) &p_shm_esinfo->ES_rdPtr;                             break;

        case R2_SHM_INFO_PCM_WR_PTR:        ptr = (volatile MS_U32 *) &p_shm_info->PCM_wrPtr;                                 break;
        case R2_SHM_INFO_MM_FILE_REQ_SIZE:  ptr = (volatile MS_U32 *) &p_shm_info->mmFileReqSize;                          break;

        case R2_SHM_INFO_ACCUM_ES_BYTECNT:  ptr = (volatile MS_U32 *) &p_shm_esinfo->ES_accumCnt;                         break;
        case R2_SHM_INFO_WAIT_1STPTS_CNT:   ptr = (volatile MS_U32 *) &p_shm_info->wait1stPtsCnt;                         break;
        case R2_SHM_INFO_WAIT_STC_CNT:      ptr = (volatile MS_U32 *) &p_shm_info->waitStcCnt;                                break;
        case R2_SHM_INFO_SMP_FLUSH_CNT:     ptr = (volatile MS_U32 *) &p_shm_info->smpFlushCnt;                               break;

        case R2_SHM_INFO_AVSYNC_STATE:        ptr = (volatile MS_U32 *) &p_shm_info->avSyncState;                               break;
        case R2_SHM_INFO_AVSYNC_FREERUN_TYPE: ptr = (volatile MS_U32 *) &p_shm_info->freeRunType;                       break;
        case R2_SHM_INFO_AVSYNC_OFFSET:       ptr = (volatile MS_U32 *) &p_shm_info->avSyncOffset;                              break;

        case R2_SHM_INFO_PTS_TAG_WR_IDX:    ptr = (volatile MS_U32 *) &p_shm_info->ptsTagWRIdx;                           break;
        case R2_SHM_INFO_PTS_TAG_RD_IDX:    ptr = (volatile MS_U32 *) &p_shm_info->ptsTagRDIdx;                           break;

        case R2_SHM_INFO_PLAY_STATE:        ptr = (volatile MS_U32 *) &p_shm_info->playState;                             break;
        case R2_SHM_INFO_DEC_TYPE:          ptr = (volatile MS_U32 *) &p_shm_info->decType;                               break;
        case R2_SHM_INFO_PLAYSMPFLAG:       ptr = (volatile MS_U32 *) &p_shm_info->playSmpFlag;                           break;
        case R2_SHM_INFO_DEC_CALLING_CNT:   ptr = (volatile MS_U32 *) &p_shm_info->decCallingCnt;                         break;
        case R2_SHM_INFO_RECEIVE_STOP_CNT:  ptr = (volatile MS_U32 *) &p_shm_info->receiveStopCnt;                        break;

        case R2_SHM_INFO_OK_FRMCNT:         ptr = (volatile MS_U32 *) &p_shm_info->ok_frmCnt;                                 break;
        case R2_SHM_INFO_ERR_FRMCNT:        ptr = (volatile MS_U32 *) &p_shm_info->err_frmCnt;                                break;
        case R2_SHM_INFO_SKIP_FRMCNT:       ptr = (volatile MS_U32 *) &p_shm_info->skip_frmCnt;                               break;
        case R2_SHM_INFO_REPT_FRMCNT:       ptr = (volatile MS_U32 *) &p_shm_info->rept_frmCnt;                               break;
        case R2_SHM_INFO_SYNC_MISS_CNT:     ptr = (volatile MS_U32 *) &p_shm_info->syncMiss_Cnt;                              break;
        case R2_SHM_INFO_PCM_EMPTY_CNT:     ptr = (volatile MS_U32 *) &p_shm_info->pcmBufEmptyCnt;                              break;

        case R2_SHM_INFO_STC:               ptr = (volatile MS_U32 *) &p_shm_info->STC;                                   break;
        case R2_SHM_INFO_PTS:               ptr = (volatile MS_U32 *) &p_shm_info->PTS;                                   break;
        case R2_SHM_INFO_CURR_PTS:          ptr = (volatile MS_U32 *) &p_shm_info->currPTS;                                   break;
        case R2_SHM_INFO_TD:                ptr = (volatile MS_U32 *) &p_shm_info->Td;                                    break;
        case R2_SHM_INFO_PTS_LATENCY:       ptr = (volatile MS_U32 *) &p_shm_info->pts_latency;                                    break;

        case R2_SHM_INFO_UNI_DECODE_DONE_CNT:       ptr = (volatile MS_U32 *) &p_shm_info->uni_decDone_cnt;                   break;
        case R2_SHM_INFO_UNI_DECODE_DONE_PCM_ADDR:  ptr = (volatile MS_U32 *) &p_shm_info->uni_decDone_pcmAddr;           break;
        case R2_SHM_INFO_UNI_DECODE_DONE_PCM_SIZE:  ptr = (volatile MS_U32 *) &p_shm_info->uni_decDone_pcmSize;           break;

        case R2_SHM_INFO_PCM_ADDR:          ptr = (volatile MS_U32 *) &p_shm_info->pcmAddr;                                       break;
        case R2_SHM_INFO_PCM_SIZE:          ptr = (volatile MS_U32 *) &p_shm_info->pcmSize;                                       break;
        case R2_SHM_INFO_spdifbuf_LEVEL:    ptr = (volatile MS_U32 *) &p_shm_info->spdifbuf_Level;                           break;
        case R2_SHM_INFO_spdifbuf_WR_PTR:   ptr = (volatile MS_U32 *) &p_shm_info->encRawBuf_wrPtr;                      break;

        case R2_SHM_INFO_DOLBY_DEC_TYPE:    ptr = (volatile MS_U32 *) &p_shm_info->dolby_info.decType;                    break;
        case R2_SHM_INFO_DOLBY_FRAME_SIZE:  ptr = (volatile MS_U32 *) &p_shm_info->dolby_info.frameSize;                  break;
        case R2_SHM_INFO_DOLBY_BS_MODE:     ptr = (volatile MS_U32 *) &p_shm_info->dolby_info.bsMode;                     break;

        case R2_SHM_INFO_DTS_CD_MODE:       ptr = (volatile MS_U32 *) &p_shm_info->dts_info.cdMode;                       break;

        case R2_SHM_INFO_MPEG_LAYER:        ptr = (volatile MS_U32 *) &p_shm_info->mpeg_info.MPEG_Layer;                  break;
        case R2_SHM_INFO_MPEG_STEREOMODE:   ptr = (volatile MS_U32 *) &p_shm_info->mpeg_info.MPEG_stereoMode;             break;
        case R2_SHM_INFO_MPEG_HEADER:       ptr = (volatile MS_U32 *) &p_shm_info->mpeg_info.MPEG_Header;                 break;
        case R2_SHM_INFO_TRANSCODE_FLAG:    ptr = (volatile MS_U32 *) &p_shm_info->common_info.spdif_info_flag;           break;

        case R2_SHM_INFO_SPDIFTx_SMP_RATE:  ptr = (volatile MS_U32 *) &p_shm_info->spdifTx_smpRate;                       break;
        case R2_SHM_INFO_HDMITx_SMP_RATE:   ptr = (volatile MS_U32 *) &p_shm_info->hdmiTx_smpRate;                        break;

        default:
            //ret_value = 0;
            return 0;
    }

    //D-cache Invalidate
    MsOS_Dcache_Flush((MS_VIRT)p_shm_info, sizeof(AUR2_ADEC_INFO_SHM));
    MsOS_Dcache_Flush((MS_VIRT)p_shm_esinfo, sizeof(AUR2_ADEC_ES_INFO_SHM));
    ret_value = *ptr;

 //   HALAUDIO_PRINT("[%s] shmType:%4x, id:%x ==> %x\n", __FUNCTION__, decInfoType, dec_id, ret_value);

    return ret_value;
}

MS_BOOL HAL_SND_R2_Set_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param, MS_U32 param2 )
{
#if 0
    volatile MS_U32 *ptr;
    SND_R2_SHARE_MEM  *p_shm = (SND_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_0_DDR_BASE+SND_R2_SHM_DDR_OFFSET));
    MS_U32 offset_R2_to_DSP, offset_R2_to_CommDDR;

    offset_R2_to_DSP = ASND__R2_DDR_SIZE;
    offset_R2_to_CommDDR = ASND__R2_DDR_SIZE + ASND_DSP_DDR_SIZE;

    switch( decParamType )
    {
        default:
            break;
    }
#endif

    AUR2_DEBUG_PRINT(printf("[%s] shmType:%4x, id:%x, param: [%08lx, %08lx]\n", __FUNCTION__, decParamType, dec_id, param, param2));

    return TRUE;
}


MS_BOOL HAL_DEC_R2_Set_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param, MS_U32 param2 )
{
    MS_U32 ret_value;
    volatile MS_U32 *ptr;
    DEC_R2_SHARE_MEM  *p_shm = (DEC_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_1_DDR_BASE+DEC_R2_SHM_DDR_OFFSET));
    AUR2_ADEC_PARAM_SHM     *p_shm_param = &p_shm->adec_param_shm[dec_id];
    AUR2_ADEC_ES_PARAM_SHM  *p_shm_esparam;
    MS_U32 offset_R2_to_DSP, offset_R2_to_CommDDR;

    offset_R2_to_DSP = ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE;
    offset_R2_to_CommDDR = ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE + ASND_DSP_DDR_SIZE;

    if (dec_id == 0)
        p_shm_esparam = &p_shm->adec_esParam_shm[0];        //dec0, use es0
    else
        p_shm_esparam = &p_shm->adec_esParam_shm[2];        //dec1, use es2

    switch( decParamType )
    {
        case R2_SHM_PARAM_ES_WR_PTR:
                ptr = (volatile MS_U32 *) &p_shm_esparam->ES_wrPtr;
                *ptr = param;               //(param + offset_R2_to_DSP + OFFSET_ES1_DRAM_ADDR;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                HAL_DEC_R2_SetCommInfo(AUR2_CommParam_ADEC_updateEsWritePtr, dec_id, 0, 0);
                break;
        case R2_SHM_PARAM_EXT_SMPRATE_CTRL:
                ptr = (volatile MS_U32 *) &p_shm_param->extSynthsizer_en;      *ptr = param;   MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                ptr = (volatile MS_U32 *) &p_shm_param->extSynthsizer_value;   *ptr = param2;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_MPG_AVOFFSET:     ptr = (volatile MS_U32 *) &p_shm_param->dtv_mpg_avDelay;     *ptr = param;   MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_AC3_AVOFFSET:     ptr = (volatile MS_U32 *) &p_shm_param->dtv_ac3_avDelay;     *ptr = param;   MsOS_Dcache_Flush((MS_VIRT)ptr,4);    break;
        case R2_SHM_PARAM_AAC_AVOFFSET:     ptr = (volatile MS_U32 *) &p_shm_param->dtv_aac_avDelay;     *ptr = param;   MsOS_Dcache_Flush((MS_VIRT)ptr,4);    break;
        case R2_SHM_PARAM_MM_FILE_REQ_SIZE: ptr = (volatile MS_U32 *) &p_shm_param->mmFileReqSize;       *ptr = param;   MsOS_Dcache_Flush((MS_VIRT)ptr,4);    break;
        case R2_SHM_PARAM_MM_INPUT_PTS:
                ptr = (volatile MS_U32 *) &p_shm_esparam->MM_inputPTS;
                *ptr = param;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                HAL_DEC_R2_SetCommInfo(AUR2_CommParam_ADEC_PTS_input, dec_id, param>>16, param&0xFFFF);
                break;

        case R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT:  ptr = (volatile MS_U32 *) &p_shm_param->mmUni_needDecodeFrmCnt;   *ptr = param; MsOS_Dcache_Flush((MS_VIRT)ptr,4);  break;
        case R2_SHM_PARAM_UNI_PCM_SET_OUTPUT_CNT:
                HAL_DEC_R2_SetCommInfo(AUR2_CommParam_ADEC_UNI_setOutput, dec_id, param>>16, param&0xFFFF);
                break;

        case R2_SHM_PARAM_DOLBY_DRC_MODE:
        case R2_SHM_PARAM_DOLBY_RF_MODE:
                ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.drcMode;
                *ptr = param;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_DOLBY_DMX_MODE:   ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.dmxMode;      *ptr = param;   MsOS_Dcache_Flush((MS_VIRT)ptr,4);    break;
        case R2_SHM_PARAM_DOLBY_HIGH_CUT:   ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.high_cut;     *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_LOW_BOOST:  ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.low_boost;    *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_GAIN:       ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.gain;         *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_MUL_FRAME_HEADER: ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.mul_frame_header; *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_AAC_BYPASS:       ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.AAC_HDMI_bypass;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_DDP_HDMI_BYPASS:   ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.DDP_HDMI_bypass;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_TB11_ENABLE:      ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.TB11_enable;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DOLBY_MIXER_BALANCE:
                {
                    MS_S32 temp = param - 32;
                    ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.mixer_balance;      *ptr = temp;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                }
                break;

        case R2_SHM_PARAM_PARSER_SPEC:       //[8]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x100;
                else
                    *ptr &= 0xFFFFFEFF;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_PARSER_PES_BYPASS:       //[7]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x80;
                else
                    *ptr &= 0xFFFFFF7F;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_PARSER_PUSI_DISABLE:       //[6]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x40;
                else
                    *ptr &= 0xFFFFFFBF;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_REPORT_MM_TS_PTS_WITH_MSB:       //[5]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x20;
                else
                    *ptr &= 0xFFFFFFDF;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;
        case R2_SHM_PARAM_MM_TS_SYNC_STC:       //[4]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x10;
                else
                    *ptr &= 0xFFFFFFEF;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_MM_FF2X:                  //[3]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x8;
                else
                    *ptr &= 0xFFFFFFF7;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_DECODE_MUTE:      //[2]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                if (param)
                    *ptr |= 0x4;
                else
                    *ptr &= 0xFFFFFFFB;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_SOUND_MODE:
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl;
                *ptr &= 0xFFFFFFFC;
                *ptr |= (param&0x03);
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_MPEG_SOUNDMODE:        ptr = (volatile MS_U32 *) &p_shm_param->mpeg_param.MPEG_soundMode;     *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_MPEG_GAIN:             ptr = (volatile MS_U32 *) &p_shm_param->mpeg_param.gain;               *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_OMX_SPDIF_CTRL:        ptr = (volatile MS_U32 *) &p_shm_param->OMX_SPDIF_Ctrl;                *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_OMX_SPDIF_PCM_LEVEL:   ptr = (volatile MS_U32 *) &p_shm_param->OMX_SPDIF_PCM_Level;           *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;


        case R2_SHM_PARAM_MAIN_VOLUME:           ptr = (volatile MS_U32 *) &p_shm_param->adMix_mainVol;                 *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_MAIN_VOLUME_MUTE:
                ptr = (volatile MS_U32 *) &p_shm_param->adMix_mainVol;
                if(param)
                *ptr |= 0x00000400;
                else
                *ptr &= 0x000003FF;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;
        case R2_SHM_PARAM_AD_VOLUME:   ptr = (volatile MS_U32 *) &p_shm_param->adMix_asocVol;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_AD_VOLUME_MUTE:
                ptr = (volatile MS_U32 *) &p_shm_param->adMix_asocVol;
                if(param)
                *ptr |= 0x00000400;
                else
                *ptr &= 0x000003FF;
                MsOS_Dcache_Flush((MS_VIRT)ptr,4);
                break;

        case R2_SHM_PARAM_COOK_NUMCODECS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.mNumCodecs;        *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_SAMPLES:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.mSamples;          *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_SAMPLERATES:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.mSampleRate;       *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_CHANNELS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.Channels[param2];  *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_REGIONS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.Regions[param2];   *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_CPLSTART:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.cplStart[param2];   *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_CPLQBITS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.cplQbits[param2];   *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_COOK_FRAMESIZE:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.FrameSize[param2];  *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_DTS_DMX_LFE_ENABLE:
            ptr = (volatile MS_U32 *) &p_shm_param->dts_param.dmxLfeEnable;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_DTS_DMX_MODE:
            ptr = (volatile MS_U32 *) &p_shm_param->dts_param.dmxMode;           *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_ASF_VERSION:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.asf_version;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_CHANNELS:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.nChannels;        *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_SAMPLERATE:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.smpRate;          *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_BYTERATE:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.byteRate;         *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_BLOCKALIGN:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.blockAlign;       *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_ENCOPT:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.encOpt;           *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_PARSINGBYAPP:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.parsingByApp;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_BITS_PER_SAMPLE:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.bitsPerSmp;        *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_CHANNELMASK:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.channelMask;       *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_DRC_PARAM_EXIST:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.drcParamExist;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_DRC_RMS_AMP_REF:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.drcRmsAmpRef;       *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_DRC_RMS_AMP_TARGET:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.drcRmsAmpTarget;    *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_DRC_PEAK_AMP_REF:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.drcPeakAmpRef;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_DRC_PEAK_AMP_TARGET:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.drcPeakAmpTarget;    *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_ASF_MAX_PACKET_SIZE:
            ptr = (volatile MS_U32 *) &p_shm_param->asf_param.maxPacketSize;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_XPCM_TYPE:
            ptr = (volatile MS_U32 *) &p_shm_param->xpcm_param.xPcmType;         *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_XPCM_CHANNELS:
            ptr = (volatile MS_U32 *) &p_shm_param->xpcm_param.nChannels;        *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_XPCM_SAMPLERATES:
            ptr = (volatile MS_U32 *) &p_shm_param->xpcm_param.smpRate;          *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_XPCM_BITS_PER_SAMPLE:
            ptr = (volatile MS_U32 *) &p_shm_param->xpcm_param.bitsPerSmp;        *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_XPCM_BLOCK_SIZE:
            ptr = (volatile MS_U32 *) &p_shm_param->xpcm_param.blockSize;         *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;
        case R2_SHM_PARAM_XPCM_SAMPLE_PER_BLOCK:
            ptr = (volatile MS_U32 *) &p_shm_param->xpcm_param.samplePerBlock;    *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_SPDIF_TYPE:
            ptr = (volatile MS_U32 *) &p_shm_param->spdif_type;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_RFSIGNAL_TYPE:
            ptr = (volatile MS_U32 *) &p_shm->RfSignalType;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_VORBIS_HEADER_SIZE:
            ptr = (volatile MS_U32 *) &p_shm_param->vorbis_header_size;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_TTS_EN:
            ptr = (volatile MS_U32 *) &p_shm_param->TTS_On_flag;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_ES_Limiter_EN:
            ptr = (volatile MS_U32 *) &p_shm_param->ES_Limiter_EN;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_ES_Limiter_Threshold:
            ptr = (volatile MS_U32 *) &p_shm_param->ES_Limiter_Threshold;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_PCM_Limiter_EN:
            ptr = (volatile MS_U32 *) &p_shm_param->PCM_Limiter_EN;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        case R2_SHM_PARAM_PCM_Limiter_Threshold:
            ptr = (volatile MS_U32 *) &p_shm_param->PCM_Limiter_Threshold;      *ptr = param;  MsOS_Dcache_Flush((MS_VIRT)ptr,4);     break;

        default:
            ret_value = 0;
            break;
    }

  //  HALAUDIO_PRINT("[%s] shmType:%4x, id:%x, param: [%08lx, %08lx]\n", __FUNCTION__, decParamType, dec_id, param, param2);
    return TRUE;
}

MS_U32 HAL_sndR2_Get_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param )
{
    MS_U32 ret_value = 0;

#if 0
    volatile MS_U32 *ptr;
    SND_R2_SHARE_MEM  *p_shm = (SND_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_0_DDR_BASE+SND_R2_SHM_DDR_OFFSET));
    MS_U32 offset_R2_to_DSP, offset_R2_to_CommDDR;

    offset_R2_to_DSP = ASND__R2_DDR_SIZE;
    offset_R2_to_CommDDR = ASND__R2_DDR_SIZE + ASND_DSP_DDR_SIZE;

    switch( decParamType )
    {
        default:
            ret_value = 0;
            break;
    }
#endif

    AUR2_DEBUG_PRINT(printf("[%s] shmType:%4x, id:%x, param: [%08lx] ==> %x\n", __FUNCTION__, decParamType, dec_id, param, ret_value));

    return ret_value;
}



MS_U32 HAL_DEC_R2_Get_SHM_PARAM(R2_SHM_PARAM_TYPE decParamType, MS_U8 dec_id, MS_U32 param )
{
    MS_U32 ret_value = 0;
    volatile MS_U32 *ptr;
    DEC_R2_SHARE_MEM  *p_shm = (DEC_R2_SHARE_MEM *)(MS_PA2KSEG1(R2_1_DDR_BASE+DEC_R2_SHM_DDR_OFFSET));
    AUR2_ADEC_PARAM_SHM     *p_shm_param = &p_shm->adec_param_shm[dec_id];
    AUR2_ADEC_ES_PARAM_SHM  *p_shm_esparam = &p_shm->adec_esParam_shm[dec_id];
    MS_U32 offset_R2_to_DSP, offset_R2_to_CommDDR;

    offset_R2_to_DSP = ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE;
    offset_R2_to_CommDDR = ADEC__R2_DDR_SIZE + ASND__R2_DDR_SIZE + ASND_DSP_DDR_SIZE;

#if 0   //debug usage
    tmp = 0x743E000;
    printf("&adec_info_shm[0] = %lx, &adec_info_shm[1] = %lx\n",
        tmp + sizeof(AUR2_ADEC_INFO_SHM)*0,
        tmp + sizeof(AUR2_ADEC_INFO_SHM)*1);

    tmp += (sizeof(AUR2_ADEC_INFO_SHM)*2);
    printf("&adec_esInfo_shm[0] = %lx, &adec_esInfo_shm[1] = %lx, &adec_esInfo_shm[2] = %lx, &adec_esInfo_shm[3] = %lx\n",
        tmp + sizeof(AUR2_ADEC_ES_INFO_SHM)*0,
        tmp + sizeof(AUR2_ADEC_ES_INFO_SHM)*1,
        tmp + sizeof(AUR2_ADEC_ES_INFO_SHM)*2,
        tmp + sizeof(AUR2_ADEC_ES_INFO_SHM)*3);

    tmp += (sizeof(AUR2_ADEC_ES_INFO_SHM)*4);
    printf("&adec_param_shm[0] = %lx, &adec_param_shm[1] = %lx\n",
        tmp + sizeof(AUR2_ADEC_PARAM_SHM)*0,
        tmp + sizeof(AUR2_ADEC_PARAM_SHM)*1);

    tmp += (sizeof(AUR2_ADEC_PARAM_SHM)*2);
    printf("&adec_esParam_shm[0] = %lx, &adec_esParam_shm[1] = %lx, &adec_esParam_shm[2] = %lx, &adec_esParam_shm[3] = %lx\n",
        tmp + sizeof(AUR2_ADEC_ES_PARAM_SHM)*0,
        tmp + sizeof(AUR2_ADEC_ES_PARAM_SHM)*1,
        tmp + sizeof(AUR2_ADEC_ES_PARAM_SHM)*2,
        tmp + sizeof(AUR2_ADEC_ES_PARAM_SHM)*3);
#endif

    switch( decParamType )
    {
        case R2_SHM_PARAM_ES_WR_PTR:
            if (dec_id == 0)
            {
                ptr = (volatile MS_U32 *) &p_shm_esparam->ES_wrPtr;    MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
            }
            else if (dec_id == 1)
            {
                ptr = (volatile MS_U32 *) &p_shm_esparam->ES_wrPtr;    MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
            }
            break;

        case R2_SHM_PARAM_MPG_AVOFFSET:     ptr = (volatile MS_U32 *) &p_shm_param->dtv_mpg_avDelay;     MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_AC3_AVOFFSET:     ptr = (volatile MS_U32 *) &p_shm_param->dtv_ac3_avDelay;     MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_AAC_AVOFFSET:     ptr = (volatile MS_U32 *) &p_shm_param->dtv_aac_avDelay;     MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_MM_FILE_REQ_SIZE: ptr = (volatile MS_U32 *) &p_shm_param->mmFileReqSize;       MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_MM_INPUT_PTS:     ptr = (volatile MS_U32 *) &p_shm_esparam->MM_inputPTS;       MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;


        case R2_SHM_PARAM_DOLBY_DRC_MODE:
        case R2_SHM_PARAM_DOLBY_RF_MODE:
                ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.drcMode;
                MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                break;

        case R2_SHM_PARAM_DOLBY_DMX_MODE:   ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.dmxMode;      MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_DOLBY_HIGH_CUT:   ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.high_cut;     MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_DOLBY_LOW_BOOST:  ptr = (volatile MS_U32 *) &p_shm_param->dolby_param.low_boost;    MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;

        case R2_SHM_PARAM_UNI_SETUP_DECODE_FRMCNT:  ptr = (volatile MS_U32 *) &p_shm_param->mmUni_needDecodeFrmCnt;      MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;

        case R2_SHM_PARAM_OMX_SPDIF_CTRL:
                ptr = (volatile MS_U32 *) &p_shm_param->OMX_SPDIF_Ctrl;      MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;

        case R2_SHM_PARAM_PARSER_SPEC:       //[8]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x100)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;

        case R2_SHM_PARAM_PARSER_PES_BYPASS:       //[7]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x80)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;

        case R2_SHM_PARAM_PARSER_PUSI_DISABLE:       //[6]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x40)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;

        case R2_SHM_PARAM_REPORT_MM_TS_PTS_WITH_MSB:       //[5]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x20)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;
        case R2_SHM_PARAM_MM_TS_SYNC_STC:       //[4]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x10)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;

        case R2_SHM_PARAM_MM_FF2X:                  //[3]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x8)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;

        case R2_SHM_PARAM_DECODE_MUTE:      //[2]
                ptr = (volatile MS_U32 *) &p_shm_param->decCtrl; MsOS_Dcache_Flush((MS_VIRT)ptr, 4);
                ret_value = *ptr;
                if (ret_value&0x4)
                    ret_value = 1;
                else
                    ret_value = 0;
                break;

      case R2_SHM_PARAM_COOK_NUMCODECS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.mNumCodecs;        MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_SAMPLES:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.mSamples;          MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_SAMPLERATES:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.mSampleRate;       MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_CHANNELS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.Channels[param];   MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_REGIONS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.Regions[param];    MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_CPLSTART:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.cplStart[param];   MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_CPLQBITS:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.cplQbits[param];   MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_COOK_FRAMESIZE:
            ptr = (volatile MS_U32 *) &p_shm_param->cook_param.FrameSize[param];  MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;
        case R2_SHM_PARAM_HASH_KEY:
            ptr = (volatile MS_U32 *) &p_shm_param->hashKey;                      MsOS_Dcache_Flush((MS_VIRT)ptr, 4);  ret_value = *ptr;       break;

        default:
            ret_value = 0;
            break;
    }

//     HALAUDIO_PRINT("[%s] shmType:%4x, id:%x, param: [%08lx] ==> %x\n", __FUNCTION__, decParamType, dec_id, param, ret_value);

    return ret_value;
}