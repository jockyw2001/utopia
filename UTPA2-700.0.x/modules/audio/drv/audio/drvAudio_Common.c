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
//  [for Doxygen]
/// file drvAudio_Common.c
/// @brief Subroutine for advance sound effect
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "ULog.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "drvBDMA.h"

#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvAUDIO_internal.h"
#include "./internal/drvADVSOUND.h"
#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvSOUND.h"
#include "./internal/drvSIF.h"
#include "./internal/drvAudio_Common.h"

#include "regAUDIO.h"
#include "drvBDMA.h"
#include "halAUDIO.h"
#include "halADVAUDIO.h"

#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "halAUDIO.h"
#include "regAUDIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
MS_BOOL (*pFuncPtr_Setsystem)(AUDIO_DEC_ID DecId, AudioDecStatus_t *p_AudioDecStatus) = NULL;

#define DRVAUDIO_COMMON_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if ( MDrv_AUDIO_SHM_Init() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern MS_U8 MDrv_AUTH_IPCheck(MS_U8 u8Bit);
extern AUDIO_SHARED_VARS2 * g_AudioVars2;
extern pFuncPtr_SetAudioParam2  g_FuncPrt_Hal_SetAudioParam2;
extern pFuncPtr_GetAudioInfo2   g_FuncPrt_Hal_GetAudioInfo2;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static ADVSND_TYPE MDrv_CurrAdvSndtype = ADV_NONE;
static MS_U8 ADVSOUND_IP_BIT[] =
{
  AU_DVB2_AUTHBIT_NULL,                  //MSAPI_AUD_ADVSND_PL2,          ///<ID = 0
  AU_DVB2_AUTHBIT_BBE,                   //MSAPI_AUD_ADVSND_BBE,          ///<ID = 1
  AU_DVB2_AUTHBIT_SRS,                   //MSAPI_AUD_ADVSND_SRS,          ///<ID = 2
  AU_DVB2_AUTHBIT_VDS,                   //MSAPI_AUD_ADVSND_VDS,          ///<ID = 3
  AU_DVB2_AUTHBIT_VSPK,                  //MSAPI_AUD_ADVSND_VSPK,         ///<ID = 4
  AU_DVB2_AUTHBIT_NULL,                  //MSAPI_AUD_ADVSND_SUPV,         ///<ID = 5
  AU_DVB2_AUTHBIT_TSHD,                  //MSAPI_AUD_ADVSND_TSHD,         ///<ID = 6
  AU_DVB2_AUTHBIT_SRS_StudioSND,         //MSAPI_AUD_ADVSND_SSHD_TSHD,    ///<ID = 55
  AU_DVB2_AUTHBIT_BONGIOVI_DPS,          //MSAPI_AUD_ADVSND_DPS,          ///<ID = 93
};


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

void MDrv_AUDIO_CheckHashkey(void)
{
    MS_U32 ip_auth = 0;
    DRVAUDIO_COMMON_CHECK_SHM_INIT;
    MS_BOOL DolbyAACFlag = 0;
    MS_BOOL DolbyEncFlag = 0;

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
     *      ip_auth[22]= 1: Disable Dolby Volume
     *      ip_auth[23]= 1: Disable SRS_PURESND
     */

    if ( g_AudioVars2->g_bHashkeyFlag == TRUE )
    {
        return;
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "===== Check Audio Decoder Protection from hash-key IP =====\n");
    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_AC3))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DD.\n");
    }
    else
    {
        ip_auth |= 0x1;    /* Disable DD */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DD Fail, no DD license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_AC3P))
    {
        ip_auth = ip_auth & 0xFFFFFFFE;    /* If DD+ is enabled, DD should be included as well */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DD+.\n");
    }
    else
    {
        ip_auth |= 0x2;    /* Disable DD+ */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DDP Fail, no DD+ license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DOLBY_MS11))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby MS11.\n");
        DolbyAACFlag = 1;
        DolbyEncFlag = 1;
    }
    else if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DOLBY_MS11_WO_AAC))
    {
        ip_auth |= 0x10; /* Disable MS10_DDT */
    }
    else
    {
        ip_auth |= 0x200;    /* Disable Dolby MS11 */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check Dolby MS11 Fail, no Dolby MS11 license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_LC))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby MS12 LC profile.\n");
        //dolby_aac_flag = 1;
    }
    else
    {
        ip_auth |= (1 << 18);    /* Disable Dolby MS12 LC profile*/
        //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check Dolby MS12 LC profile Fail, no Dolby MS12 LC profile license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_D))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby MS12 D profile.\n");
        DolbyAACFlag = 1;
        DolbyEncFlag = 1;
    }
    else if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DOLBY_MS12_D_WO_AAC))
    {
        ip_auth |= 0x10; /* Disable MS10_DDT */
    }
    else
    {
        ip_auth |= (1 << 19);    /* Disable Dolby MS12 D profile */
        //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check Dolby MS12 D profile Fail, no Dolby MS12 D profile license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_B))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby MS12 B profile.\n");
        DolbyAACFlag = 1;
        DolbyEncFlag = 1;
    }
    else if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DOLBY_MS12_B_WO_AAC))
    {
        ip_auth |= 0x10; /* Disable MS10_DDT */
    }
    else
    {
        ip_auth |= (1 << 21);    /* Disable Dolby MS12 B profile */
        //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check Dolby MS12 B profile Fail, no Dolby MS12 B profile license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_MS10_DDT))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby Pulse(MS10-DDT).\n");
        DolbyAACFlag = 1;
        DolbyEncFlag = 1;
    }
    else if (!(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DOLBY_MS11)||MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_B)||MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_D))  )
    {
        ip_auth |= 0x10; /* Disable MS10_DDT */
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_GAAC))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Generic HE-AAC!!\n");
        /* See if need to translate to Generic HE-AAC package */
    }
    else
    {
        ip_auth |= 0x100; /* Disable Generic HE-AAC */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check AAC Fail, no any AAC license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DDE))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby DDCO \n");
        DolbyEncFlag = 1;
    }
    else
    {
        ip_auth |= 0x4;    /* Disable DDCO */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DDCO Fail, no DDCO license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DTSDEC))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DTS DMP.\n");
    }
    else
    {
        ip_auth |= 0x8;    /* Disable DTS */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DTS Fail, no DTS license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_WMA))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support WMA.\n");
    }
    else
    {
        ip_auth |= 0x20;    /* Disable WMA */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check WMA Fail, no WMA license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DRA))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DRA.\n");
    }
    else
    {
        ip_auth |= 0x40;    /* Disable DRA */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DRA Fail, no DRA license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DTS_LBR))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DTSLBR.\n");
    }
    else
    {
        ip_auth |= 0x80;    /* Disable DTSLBR */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DTSLBR Fail, no DTSLBR license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DTSE))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DTSE.\n");
    }
    else
    {
        ip_auth |= 0x400;    /* Disable DTSE */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DTSE Fail, no DTSE license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DTSNeoUltra))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DTSNeoUltra.\n");
    }
    else
    {
        ip_auth |= 0x800;    /* Disable DTSNeoUltra */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DTSNeoUltra Fail, no DTSNeoUltra license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support SRS_TSHD.\n");
    }
    else
    {
        ip_auth |= 0x2000;    /* Disable SRS_TSHD */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check SRS_TSHD Fail, no SRS_TSHD license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS_StudioSND))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support SRS_THEATERSOUND.\n");
    }
    else
    {
        ip_auth |= 0x4000;    /* Disable SRS_THEATERSOUND */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check SRS_THEATERSOUND Fail, no SRS_THEATERSOUND license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DTS_StudioSound3D))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DTS_StudioSound3D.\n");
    }
    else
    {
        ip_auth |= 0x8000;    /* Disable DTS_StudioSound3D */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DTS_StudioSound3D Fail, no DTS_StudioSound3D license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_COOK))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support COOK.\n");
    }
    else
    {
        ip_auth |= 0x10000;    /* Disable COOK */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check COOK Fail, no COOK license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DTSHD))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support DTS_HD.\n");
        ip_auth &= 0xFFFFFBFF;  //if DTSHD then enable DTSE
    }
    else
    {
        ip_auth |= 0x20000;    /* Disable DTS_HD */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check DTS_HD Fail, no DTS_HD license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DolbyVolume))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support Dolby volume.\n");
    }
    else
    {
        ip_auth |= 0x400000;    /* Disable Dolby volume */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check Dolby volume Fail, no Dolby volume license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS_PURESND))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash-key Support SRS_PURESND.\n");
    }
    else
    {
        ip_auth |= 0x800000;    /* Disable SRS_PURESND */
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Hash Key Check SRS_PURESND Fail, no SRS_PURESND license!!\n");
    }

    if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DEMOMODE))
    {
        ip_auth |= 0x1000;
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "This is Demo Mode!!\n");
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "===== Check Protection IP End                         =====\n");
    HAL_AUDIO_CheckVersionInfo();

    HAL_AUDIO_SET_IPAUTH(ip_auth);

    if (HAL_AUDIO_CheckHashkeyDone(ip_auth) == -A_EINVAL)
        MS_ASSERT(0);

    g_AudioVars2->DolbyAACFlag = DolbyAACFlag;
    g_AudioVars2->DolbyEncFlag = DolbyEncFlag;

    g_AudioVars2->g_bHashkeyFlag = TRUE;

}

MS_BOOL MDrv_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param )
{
    MS_BOOL bRet;

    if ( g_FuncPrt_Hal_SetAudioParam2 )
    {
        if ( paramType == Audio_ParamType_setAudioDecoder ||
             paramType == Audio_ParamType_AllocDecoder )
        {
            MMA_AudioType AudioType;

            AudioType = (MMA_AudioType) Param;
            MDrv_AUDIO_CheckHashkey();

            bRet = g_FuncPrt_Hal_SetAudioParam2(DecId, paramType, AudioType);
        }
        else
        {
            bRet = g_FuncPrt_Hal_SetAudioParam2(DecId, paramType, Param);
        }
    }
    else
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: g_FuncPrt_Hal_SetAudioParam2 is NULL !!\r\n", __FUNCTION__);
        bRet = FALSE;
    }

    return bRet;
}

//====== STAOS PIP END 2012/02/01 ========

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetSystem()
/// @brief \b Function \b Description:  This function will load
///                                     different audio decoder to audio system.
///
///                                     In Mstar audio system, we can support 1 or 2 decoders.
///                                     One is for main DTV audio or Multimedia audio player.
///                                     Another is for second audio decode (ex: used for Audio Description purpose)
///
/// @param enDecSystem  \b : (En_DVB_decSystemType) decoder type
///                        - parameter type "MSAPI_AUD_DVB_XXXX" is decoder type for main audio decoder
///                        - parameter type "MSAPI_AUD_DVB2_XXXX" is decoder type for second audio decoder
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{
    MS_BOOL bRet = FALSE;

    MDrv_AUDIO_CheckHashkey();
    HAL_AUDIO_SetSystem(enDecSystem);
    bRet = TRUE;

    return bRet;
}

MS_BOOL MDrv_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId, AudioDecStatus_t * p_AudioDecStatus)
{
    MS_BOOL bRet = FALSE;

    DRVAUDIO_COMMON_CHECK_SHM_INIT;

    if (NULL == p_AudioDecStatus)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : AudioDecStatus is null\n", __FUNCTION__);
        return bRet;
    }

    // check hash key and set to correct DSP code type
    MDrv_AUDIO_CheckHashkey();

    if (pFuncPtr_Setsystem != NULL)
    {
        bRet = pFuncPtr_Setsystem(DecId,p_AudioDecStatus);
    }

    if (TRUE == bRet)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Reload Success\n");
        memcpy(&g_AudioVars2->AudioDecStatus[DecId], p_AudioDecStatus, sizeof(AudioDecStatus_t));
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Input\n");
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eAudFormat = %d\n", p_AudioDecStatus->eAudFormat);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eGroup = %d\n", p_AudioDecStatus->eGroup);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eDSPId = %d\n", p_AudioDecStatus->eDSPId);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eStcSource = %d\n", p_AudioDecStatus->eStcSource);

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "DecId = %d\n", DecId);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eAudFormat = %d\n", g_AudioVars2->AudioDecStatus[DecId].eAudFormat);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eGroup = %d\n", g_AudioVars2->AudioDecStatus[DecId].eGroup);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eDSPId = %d\n", g_AudioVars2->AudioDecStatus[DecId].eDSPId);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "eStcSource = %d\n", g_AudioVars2->AudioDecStatus[DecId].eStcSource);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_VDS_Enable()
/// @brief \b Function \b Description:  This function is used to set VDS
/// @param u8enVDS     \b : 1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_VDS_Enable(MS_U8 u8enVDS)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_VDS))
    {
        HAL_SOUND_VDS_Enable(u8enVDS);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_VSPK_Enable()
/// @brief \b Function \b Description:  This function is used to set VSPK
/// @param u8enVSPK    \b : 1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_VSPK_Enable(MS_U8 u8enVSPK)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_VSPK))
    {
        HAL_SOUND_VSPK_Enable(u8enVSPK);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SRS_SetTsxtPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-XT
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS) && (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD) == 0))
    {
        HAL_SOUND_SRS_SetTsxtPara(u8mode, u8value);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SRS_SetTshdPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-HD
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
    {
        HAL_SOUND_SRS_SetTshdPara(u8mode, u8value);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SRS_Enable()
/// @brief \b Function \b Description:  This function is used to set SRS-XT
/// @param u8enSRS     \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SRS_Enable(MS_U8 u8enSRS)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS) && (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD) == 0))
    {
        HAL_SOUND_SRS_Enable(u8enSRS);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_Enable()
/// @brief \b Function \b Description:  This function is used to set SRS-HD
/// @param u8enSRS     \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_Enable(MS_U8 u8enTSHD)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
    {
        HAL_SOUND_TSHD_Enable(u8enTSHD);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_VSPK_SetWMod()
/// @brief \b Function \b Description:  This function is used to set VSPK W Mode
/// @param u8VSPK_WMode \b :
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_VSPK_SetWMod(MS_U8 u8VSPK_WMode)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_VSPK))
    {
        HAL_SOUND_VSPK_SetWMod(u8VSPK_WMode);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_VSPK_SetSMod()
/// @brief \b Function \b Description:  This function is used to set VSPK S Mode
/// @param u8VSPK_SMode \b u8VSPK_SMode:
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_VSPK_SetSMod(MS_U8 u8VSPK_SMode)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_VSPK))
    {
        HAL_SOUND_VSPK_SetSMod(u8VSPK_SMode);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_BBE_SetBbeMode()
/// @brief \b Function \b Description:  This function is used to set BBE Mode
/// @param u8EnBBE     \b :
/// @param u8BBEMode   \b :
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_BBE_SetBbeMode(MS_U8 u8EnBBE, MS_U8 u8BBEMode)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_BBE))
    {
        HAL_SOUND_BBE_SetBbeMode(u8EnBBE, u8BBEMode);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SRS_DC()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-XT Dialog Clarity
/// @param u8SRSenDC   \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SRS_DC(MS_U8 u8SRSenDC)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS) && (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD) == 0))
    {
        HAL_SOUND_SRS_DC(u8SRSenDC);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SRS_TruBass()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-XT Trubass
/// @param u8SRSenTruBass \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SRS_TruBass(MS_U8 u8SRSenTruBass)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS) && (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD) == 0))
    {
        HAL_SOUND_SRS_TruBass(u8SRSenTruBass);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_TruBass()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Trubass
/// @param u8TSHDenTruBass \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_TruBass(MS_U8 u8TSHDenTruBass)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
    {
        HAL_SOUND_TSHD_TruBass(u8TSHDenTruBass);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_Definition()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Definition
/// @param u8TSHDenDefinition \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_Definition(MS_U8 u8TSHDenDefinition)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
    {
        HAL_SOUND_TSHD_Definition(u8TSHDenDefinition);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_Clarity()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD DC
/// @param u8TSHDenClarity \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_Clarity(MS_U8 u8TSHDenClarity)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
    {
        HAL_SOUND_TSHD_Clarity(u8TSHDenClarity);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ADVSOUND_ProcessEnable
/// @brief \b Function \b Description: This routine is used to enable process of advanced sound effect.
/// @param <IN>        \b ADVSND_TYPE type
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_ADVSOUND_ProcessEnable(ADVSND_TYPE type)
{
    MS_BOOL status = FALSE;

    if (!ADVSND_TYPE_Compare(type, MDrv_CurrAdvSndtype))
    {
        // set advance surround sound system
        // 0: PL2,  1:BBE,  2: SRS  3:VDS, 4:VSPK, 5:SUPV, 6:TSHD, 7:DTS_ULTRATV

        if (ADVSND_TYPE_Compare(type, BBE))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_BBE))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_BBE);
                MDrv_CurrAdvSndtype = BBE;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : BBE !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No BBE license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, SRS_TSXT))
        {
            if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_SRS);
                MDrv_CurrAdvSndtype = SRS_TSXT;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : SRS TSXT !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No SONICMOTION ABS3D license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, DOLBY_PL2VDS))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_VDS))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_VDS);
                MDrv_CurrAdvSndtype = DOLBY_PL2VDS;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : VDS !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No VDS license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, DOLBY_PL2VDPK))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_VSPK))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_VSPK);
                MDrv_CurrAdvSndtype = DOLBY_PL2VDPK;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : VSPK !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No VSPK license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, SRS_TSHD))
        {
             if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_TSHD))
             {
                 status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_TSHD);
                 MDrv_CurrAdvSndtype = SRS_TSHD;
                 MsOS_DelayTask(5);
                 AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : SRS TSHD !!\033[0m \n");
             }
             else
             {
                 AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No SRS TSHD license!!\033[0m \n");
                 status = FALSE;
             }
        }
        else if (ADVSND_TYPE_Compare(type, SRS_THEATERSOUND))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS_StudioSND))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_THEATERSOUND);
                MDrv_CurrAdvSndtype = SRS_THEATERSOUND;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : SRS StudioSound HD!!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No SRS StudioSound HD license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, DTS_STUDIOSOUND_3D))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DTS_StudioSound3D))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_STUDIOSOUND_3D);
                MDrv_CurrAdvSndtype = DTS_STUDIOSOUND_3D;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : SRS StudioSound 3D!!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No SRS StudioSound 3D license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, DTS_ULTRATV))
        {
            status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_DTS_ULTRA_TV);
            MDrv_CurrAdvSndtype = DTS_ULTRATV;
        }
        else if (ADVSND_TYPE_Compare(type, AUDYSSEY))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_AUDSY))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_AUDSY);
                MDrv_CurrAdvSndtype = AUDYSSEY;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : AUDUSSEY!!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No AUDUSSEY license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, SUPER_VOICE))
        {
            status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_SUPV);
            MDrv_CurrAdvSndtype = SUPER_VOICE;
        }
        else if (ADVSND_TYPE_Compare(type, DBX))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DBX))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_DBX);
                MDrv_CurrAdvSndtype = DBX;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : DBX !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No DBX license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, SRS_PURESND))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SRS_PURESND))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_PURESND);
                MDrv_CurrAdvSndtype = SRS_PURESND;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : SRS PureSound!!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No SRS PureSound license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, SONICMOTION_ABS3D))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_SONICMOTION_ABS3D))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_SONICMOTION_ABS3D);
                MDrv_CurrAdvSndtype = SONICMOTION_ABS3D;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : SONICMOTION ABS3D!!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No SONICMOTION ABS3D license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, BONGIOVI_DPS))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_BONGIOVI_DPS))
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_BONGIOVI_DPS);
                MDrv_CurrAdvSndtype = BONGIOVI_DPS;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : BONGIOVI DPS !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No BONGIOVI DPS license!!\033[0m \n");
                status = FALSE;
            }
        }
        else if (ADVSND_TYPE_Compare(type, DOLBY_DAP))
        {
            if (MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_B) ||
                MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DOLBY_MS12_B_WO_AAC) ||
                MDrv_AUTH_IPCheck(AU_DVB_AUTHBIT_DOLBY_MS12_LC))  //MS12 profile D not support DAP
            {
                status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_DOLBY_DAP);
                MDrv_CurrAdvSndtype = DOLBY_DAP;
                MsOS_DelayTask(5);
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m Hash-key Support : DOLBY DAP !!\033[0m \n");
            }
            else
            {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\n\033[0;31m [Warning!!]No DOLBY DAP license!!\033[0m \n");
                status = FALSE;
            }
        }
        else
        {
            status = HAL_ADVSOUND_ProcessEnable(AU_DVB2_ADVSND_NONE);
            MDrv_CurrAdvSndtype = ADV_NONE;
        }
    }
    else
    {
        status = TRUE;
    }

    return (status);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ADVSOUND_SubProcessEnable(ADVFUNC proc, BOOL enable)
/// @brief \b Function \b Description: This routine is used to enable subprocess of advanced sound effect.
/// @param <IN>        \b proc, enable
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
{
    DRVAUDIO_COMMON_CHECK_SHM_INIT;

    if (ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, SRS_TSXT))
    {
        if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_3D_EN) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_EN) ||
	          ADVFUNC_Compare(proc, SRS_TSXT_TRUBASS) ||
	          ADVFUNC_Compare(proc, SRS_TSXT_DC) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_TRUBASS) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_DC) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_SRS3D) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUBASS) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_DC) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_SURR) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_LEVEL_INDPT) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TOTAL))
            return(HAL_ADVSOUND_SubProcessEnable(proc, enable));
        else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return SRS_TSXT, unknown parameter=%s using.\033[0m \n", proc);
    }
    else if (ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, SRS_TSHD))
    {
        if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_3D_EN) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_EN) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_TRUBASS) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_DC) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_DEFINITION) ||
	          ADVFUNC_Compare(proc, SRS_TSHD_SRS3D) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUBASS) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_DC) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_DEFINITION) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_SURR) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_LEVEL_INDPT) ||
	          ADVFUNC_Compare(proc, SRS_THEATERSOUND_TOTAL))
            return(HAL_ADVSOUND_SubProcessEnable(proc, enable));
        else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return TSHD, Unknown parameter=%s using. \033[0m \n", proc);
    }
    else if ((ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, DTS_STUDIOSOUND_3D)) ||
        (ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, SRS_THEATERSOUND)))
    {
        if (ADVFUNC_Compare(proc, SRS_THEATERSOUND_3D_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUBASS) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_DC) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_DEFINITION) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_HARDLIMITER) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_HPF) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUEQ) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME_NOISE_MNGR) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_SURR) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TSHD_LEVEL_INDPT) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CS) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUDIALOG) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME_NORMALIZER) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TRUVOLUME_SMOOTH) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_HPF_END) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_TOTAL) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_DEPTH_PROCESS_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_3D_SURR_BOOST_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_FADE_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_TSHD_MIX_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_CC3D_TBHDX_EN) ||
            ADVFUNC_Compare(proc, SRS_THEATERSOUND_GEQ_EN) ||
            ADVFUNC_Compare(proc, SRS_PURESOUND_HL) ||
            ADVFUNC_Compare(proc, SRS_PURESOUND_AEQ) ||
            ADVFUNC_Compare(proc, SRS_PURESOUND_HPF))
            return(HAL_ADVSOUND_SubProcessEnable(proc, enable));
         else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return DTS_STUDIOSOUND, Unknown parameter=%s using.\033[0m \n", proc);
    }
    else if (ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, SRS_PURESND))
    {
        if (ADVFUNC_Compare(proc, SRS_PURESOUND_HL) ||
            ADVFUNC_Compare(proc, SRS_PURESOUND_AEQ) ||
            ADVFUNC_Compare(proc, SRS_PURESOUND_HPF) ||
            ADVFUNC_Compare(proc, SRS_PURESOUND_TBHD))
            return(HAL_ADVSOUND_SubProcessEnable(proc, enable));
        else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return PURESOUND, Unknown parameter=%s using.\033[0m \n", proc);
    }
    else if (ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, BONGIOVI_DPS))
    {
        if (ADVFUNC_Compare(proc, BONGIOVI_DPS_SURR_EN))
        {
            return(HAL_ADVSOUND_SubProcessEnable(proc, enable));
        }
        else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return Bongiovi DPS, Unknown parameter=%s using.\033[0m \n", proc);
    }
    else if (ADVSND_TYPE_Compare(MDrv_CurrAdvSndtype, DOLBY_DAP))
    {
        if (ADVFUNC_Compare(proc, DOLBY_DAP_EN))
        {
            g_AudioVars2->g_DolbyDAPEnable = enable;
            return(HAL_ADVSOUND_SubProcessEnable(proc, enable));
        }
        else
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return Dolby DAP, Unknown parameter=%s using.\033[0m \n", proc);
    }
    else
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\n\033[0;31m [Warning!!] return , Unknown IP Security=%s.\033[0m \n", MDrv_CurrAdvSndtype);
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, DBXTV_TotVolMode TotVolMode, DBXTV_TotSurMode TotSurMode)
/// @brief \b Function \b Description: This routine is used to set dbx TV mode
/// @param <IN>        \b param, TotSonMode, TotVolMode, TotSurMode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void MDrv_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
{
    if(MDrv_AUTH_IPCheck(AU_DVB2_AUTHBIT_DBX))
    {
        HAL_DBXTV_SetMode(totsonmode, totvolmode, totsurmode,enable);
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ADVSOUND_GetInfo()
/// @brief \b Function \b Description: This routine is used for Get ADV Sound info
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_ADVSOUND_GetInfo(AdvSound_GET_Type Type)
{
    MS_U8 i;
    MS_U32 rvalue = 0;
    if(Type == ADVSOUND_EN_TYPE)
    {
        for(i = 0; i <= 6; i++)
        {
            if(MDrv_AUTH_IPCheck(ADVSOUND_IP_BIT[i]))
            {
                rvalue = (MS_U32)i;
            }
        }
    }
    return rvalue;
}
