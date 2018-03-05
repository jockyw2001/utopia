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
/// file    apiAUDIO.c
/// @brief  audio API functions
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/jiffies.h>
#else
#include <string.h>
#endif

#include "ULog.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "drvAUDIO_if.h"
#include "apiAUDIO.h"
#include "apiAUDIO_private.h"
#include "drvAUDIO.h"
#include "utopia_dapi.h"

#include "../drv/audio/internal/drvKTV.h"
#include "../drv/audio/internal/drvSIF.h"
#include "../drv/audio/internal/drvMAD.h"
#include "../drv/audio/internal/drvMAD2.h"
#include "../drv/audio/internal/drvSOUND.h"
#include "../drv/audio/internal/drvADVSOUND.h"
#include "../drv/audio/internal/drvAUDIO_internal.h"
#include "../drv/audio/internal/drvAudio_Common.h"
#include "drvWDT.h" //Refine power on sequence for earphone & DAC pop noise issue



//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define APIAUDIO_CHECK_SHM_INIT \
    do { \
        if (g_AudioVars2 == NULL) \
        { \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Warning! g_AudioVars2 should not be NULL !!!\n", __FUNCTION__); \
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Perform SHM Init here !!!\n", __FUNCTION__); \
            if (MDrv_AUDIO_SHM_Init() == FALSE) \
            { \
                MS_ASSERT(0); \
            } \
        } \
    } while(0)

/*
 * AEON(MM) and MCU use independent memory space,
 * so they can not use the same flag
 */
#if defined(MCU_AEON) && !defined(__AEONR2__)
    /* AEON(MM) */
    #define APIAUDIO_CHECK_INIT_FLAG(bRet) \
        do{ \
            bRet = TRUE; \
        }while(0)
#else
    /* ARM, MIPS, and AEON_R2(NOS_MCU) */
    #define APIAUDIO_CHECK_INIT_FLAG(bRet) \
        do{ \
            bRet = MDrv_AUDIO_GET_INIT_FLAG(); \
            if (bRet == FALSE) \
            { \
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Audio system is not ready yet, please try again later\n", __FUNCTION__); \
            } \
        }while(0)
#endif

#define HINIBBLE(u8Data)    ((u8Data) >> 4)
#define LONIBBLE(u8Data)    ((u8Data) << 4)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern MS_BOOL g_bDSPLoadCode;
extern AUDIO_SHARED_VARS2 * g_AudioVars2;
extern MS_S32 _s32AUDIOMutex;
extern MS_U8 CheckPlayDoneCnt;

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
AUDIO_SPDIF_OUTPUT_TYPE     gSpdifOutputType;
HDMI_TX_OUTPUT_TYPE         gHdmiOutputType;
AUDIOSTATUS                 m_eSifStatus;
AUDIOSTANDARD_TYPE          m_eSifStandard;
MS_U8                       SpeakerMaxVolume = 0x0C; //(MS_U8)(TvVolumeTable[100]>>8);
MS_U8                       DynamicAvl_active_volume = 0x17; // about -12dB for 2Vrms input, value of volume control register

static En_DVB_advsndType    u8CurrAdvSndSys = MSAPI_AUD_ADVSND_NONE;
static En_DVB_decCmdType    enDecoderStatus;
static MS_U8                u8IntTag = 1;
MS_U32 g_audioDumpInfo_interval = 500;

En_DVB_decSystemType        enCurDecSystem, enCurDecSystem2;


static OUTPUT_SOURCE_INFO OutSourceInitSettings=
{
    E_CONNECT_MAIN,  // For Speaker
    E_CONNECT_MAIN,  // For HP
    E_CONNECT_MAIN,  // For Monitor out
    E_CONNECT_SCART,  // For Scart out
    E_CONNECT_MAIN,  // For Spdif out
    E_CONNECT_MAIN,   // For HDMI ARC
    E_CONNECT_MAIN   // For HDMI Tx
};

Audio_MBOX_BackUp_t MBOXBkUp[] ={{0x112D00, 8, {0}},
                                                        {0x112D10, 8, {0}},
                                                        {0x112D20, 8, {0}},
                                                        {0x112D30, 8, {0}},
                                                        {0x112D40, 8, {0}},
                                                        {0x112D50, 8, {0}},
                                                        {0x112D80, 8, {0}},
                                                        {0x112D90, 8, {0}},
                                                        {0x112DC0, 8, {0}},
                                                        {0x112DD0, 8, {0}},
                                                        {0x112C60, 8, {0}},
                                                        //{0x112CE0, 8, {0}},
                                                        {0x112E20, 4, {0}},
                                                        {0x112E80, 8, {0}},
                                                        {0x112E90, 8, {0}},
                                                        {0x160380, 8, {0}},
                                                        {0x160390, 4, {0}},
                                                         /* Add above only, the last one must be null. */
                                                        {0,             0, {0},}
                                                        };

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

static void _ApiDecCmd_To_DrvDecCmd(En_DVB_decCmdType enDecComamnd, AU_DVB_DECCMD *u8DecCmd)
{
    switch(enDecComamnd)
    {
        case MSAPI_AUD_DVB_DECCMD_STOP:
            *u8DecCmd = AU_DVB_DECCMD_STOP;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAY:
            *u8DecCmd = AU_DVB_DECCMD_PLAY;
            break;

        case MSAPI_AUD_DVB_DECCMD_RESYNC:
            *u8DecCmd = AU_DVB_DECCMD_RESYNC;
            break;

        case MSAPI_AUD_DVB_DECCMD_FREE_RUN:
            *u8DecCmd = AU_DVB_DECCMD_FREERUN;
            break;

        case MSAPI_AUD_DVB_DECCMD_AVSYNC:
            *u8DecCmd = AU_DVB_DECCMD_AVSYNC;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILE:
             *u8DecCmd = AU_DVB_DECCMD_PLAYFILE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PAUSE:
            *u8DecCmd = AU_DVB_DECCMD_PAUSE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILETSP:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFILETSP;
            break;

        case MSAPI_AUD_DVB_DECCMD_STARTBROWSE:
            *u8DecCmd = AU_DVB_DECCMD_STARTBROWSE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFRAME;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFRAME_STAGE;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFRAME_GS;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2:
            *u8DecCmd = AU_DVB_DECCMD_PLAYFILE2_PTS;
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAY_AD:
            *u8DecCmd = AU_DVB_DECCMD_PLAY_AD;
            break;

        case MSAPI_AUD_DVB_DECCMD_STOP_AD:
            *u8DecCmd = AU_DVB_DECCMD_STOP_AD;
            break;

        case MSAPI_AUD_DVB_DECCMD_SKIP:
            *u8DecCmd = AU_DVB_DECCMD_SKIP;
            break;

        default:
            // Need refine
            *u8DecCmd = AU_DVB_DECCMD_STOP;
            break;
    }
    return;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_SYSTEM RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus()
/// @brief \b Function \b Description:  Return audio status.
/// @return MS_U16     \b : return structure which include pcm, non pcm, sampling rate.
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_HDMI_RX_GetHdmiInAudioStatus( ST_HDMI_AUDIO_STATUS *p_hdmiAudioSts)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return(MDrv_AUDIO_HDMI_RX_GetHdmiInAudioStatus(p_hdmiAudioSts));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to execute DSP power on/down setting.
/// @param bFlag       \b : TRUE --power on;
///                         FALSE--power off
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPowerOn(MS_BOOL bFlag)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    MDrv_AUDIO_SetPowerOn(bFlag);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUD_Initialize()
/// @brief \b Function \b Description: Audio initialize function incluing DSP power on and load code.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_Initialize(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (MDrv_AUDIO_SHM_Init() == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "MApi_AUDIO_Initialize failes to get SHM buffer\n");
        return;
    }

    if (MDrv_AUDIO_Mutex_Init() == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : fails to init Audio Mutex\n", __FUNCTION__);
        return;
    }

    APIAUDIO_CHECK_SHM_INIT;

    //Init common PCM I/O
    MDrv_AUDIO_PCM_Init();

    OS_OBTAIN_MUTEX(_s32AUDIOMutex, MSOS_WAIT_FOREVER);

    if (MDrv_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        /* Patch for multi-process, some audio utopia call back function did not register */
        /* so if share memory have already init, check call back function again,          */
        /* if call back function is NULL, register it !!                                  */
        MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_Set_CallBackFunc, 0, 0);

        /* Patch for multi-process, MMIO Base address need to setup for different process */
        MDrv_AUDIO_Init();

        OS_RELEASE_MUTEX(_s32AUDIOMutex);

        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "MApi_AUDIO_Initialize doesn't need to init again\n");
        return;
    }

    g_AudioVars2->eAudioSource = E_AUDIO_INFO_ADC_IN;
    g_AudioVars2->g_R2ShmInitFlag = FALSE;

    memcpy(&(g_AudioVars2->AudioOutputSourceInfo), &OutSourceInitSettings, sizeof(g_AudioVars2->AudioOutputSourceInfo));
    MDrv_AUDIO_Init();
    MsOS_DelayTask(10);    //Refine power on sequence for earphone & DAC pop noise issue

    MDrv_AUDIO_WriteInitTable();

    MDrv_AUDIO_SetPowerOn(TRUE);

    MDrv_MAD_SetMemInfo();
    MDrv_MAD2_SetMemInfo();

    // Load DSP DEC Code
    MDrv_SIF_Init();
    MDrv_SOUND_Init();

    // Load SIF DSP code stage
    MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);

    MDrv_MAD_Init();

    MDrv_AUDIO_DecoderLoadCode();  // Load decoder main code

    MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_FS, SPDIF_CS_FS_48K);

    if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DVB)
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_DVB);
    else if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ATSC)
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_ATSC);
    else if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ISDB)
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_ISDB);
    else
        MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_NONE);

    MDrv_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_LoudnessMatching, (MS_U32)g_AudioVars2->g_dolbyLoudness_Enable, (MS_U32)g_AudioVars2->AudioSystemInfo.dtv_system);

   // MApi_AUDIO_SetAdvSndSys(MSAPI_AUD_ADVSND_SRS);

    if ( g_AudioVars2->AudioPathInfo.HpOut != AUDIO_PATH_NULL )
    {
        MDrv_AUDIO_SetInputPath(AUDIO_SRC_INPUT, g_AudioVars2->AudioPathInfo.HpOut);
    }

    MDrv_SOUND_EnableBalance(TRUE);   // Enable Speaker Balance function in audio init .
    //MApi_AUDIO_SetEncInit(AU_DVB_ENCBR_192K, 16);

    //Refine power on sequence for earphone & DAC pop noise issue
    MDrv_AUDIO_EnaEarphone_LowPower_Stage();

//================================================
//  Execute Output Path connection
//================================================
     _MApi_AUDIO_SetOutConnectivity();
//================================================

    //Attach audio interrupts for DEC/SND DSP to MCU
    MDrv_AUDIO_AttachInterrupt(TRUE);

    //Rise init flag
    MDrv_AUDIO_SET_INIT_FLAG(TRUE);

    OS_RELEASE_MUTEX(_s32AUDIOMutex);

}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_Shift()
/// @brief \b Function \b Description: This function is used to provide a API for application to choose what kind of VIF mdoe.
///
////////////////////////////////////////////////////////////////////////////////

void _MApi_AUDIO_SIF_Shift(En_AUD_VIF_Type type)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if(MDrv_SIF_SendCmd(AU_SIF_CMD_GET_VIF_PATCH_ENABLE, NULL, NULL) == 0)
    {
        return;
    }

    switch (type)
    {
        case MSAPI_AUD_SIF_42M:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_SIF_SHIFT_CLK, 0x00, NULL);
            break;

        case MSAPI_AUD_SIF_43M:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_SIF_SHIFT_CLK, 0x01, NULL);
            break;

        case MSAPI_AUD_SIF_44M:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_SIF_SHIFT_CLK, 0x02, NULL);
            break;

        default:
            break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_Audio_Monitor()
/// @brief \b Function \b Description: This function is used to monitor DSP while-loop and ISR running counter.
///                                  If these counters stop longer than a certain period of time in any abnormal condition, DSP reboot will be launched to alive DSP process again.
////////////////////////////////////////////////////////////////////////////////
void _MApi_Audio_Monitor(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;
    MS_BOOL bRecoveryOn = FALSE;
    MS_U8 u8CurrentSeWhile = 0;
    MS_U8 u8CurrentSeISR = 0;
    MS_U8 u8CurrentDecR2While = 0;
    MS_U8 u8CurrentDecR2Timer = 0;
    MS_U8 u8CurrentSndR2While = 0;
    MS_U8 u8CurrentSndR2Timer = 0;
    MS_U32 u32Delay_Counter = 0;
    MS_BOOL bASND_R2_SUPPORT = MDrv_AUDIO_Check_AsndR2_SupportStatus();

    static MS_U8  u8LastSeWhile = 1;
    static MS_U8  u8LastSeISR = 1;
    static MS_U8  u8LastDecR2While = 1;
    static MS_U8  u8LastDecR2Timer = 1;
    static MS_U8  u8LastSndR2While = 1;
    static MS_U8  u8LastSndR2Timer = 1;
    static MS_U8  u8SeWhileFailCount = 0;
    static MS_U8  u8SeISRFailCount = 0;
    static MS_U8  u8DecR2WhileFailCount = 0;
    static MS_U8  u8DecR2TimerFailCount = 0;
    static MS_U8  u8SndR2WhileFailCount = 0;
    static MS_U8  u8SndR2TimerFailCount = 0;
    static MS_U32 Waittime = 0;
    static MS_U32 Waittime2 = 0;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return;
    }

    APIAUDIO_CHECK_SHM_INIT;

    MDrv_AUDIO_SPDIF_Monitor_SamplingRate();    // used in U4 for SPDIF raw out ADC clock setting

    if ( MsOS_Timer_DiffTimeFromNow(Waittime2) > g_audioDumpInfo_interval)
    {
        Waittime2 = MsOS_GetSystemTime();
        MDrv_AUDIO_DumpDspInfo();
    }

    if (MsOS_Timer_DiffTimeFromNow(Waittime) < 100)
    {
        return;
    }

    if (g_bDSPLoadCode== TRUE)
    {
        return;
    }

    Waittime = MsOS_GetSystemTime();

    if((u8SeWhileFailCount >= 7) || (u8SeISRFailCount >= 7)||(u8SndR2WhileFailCount>=7)||(u8SndR2TimerFailCount>=7))
    {
        u32Delay_Counter = u8LastSndR2While + u8LastSndR2Timer + u8LastSeWhile + u8LastSeISR;
        MsOS_DelayTaskUs(u32Delay_Counter);
    }

    u8CurrentSeWhile = MDrv_AUDIO_ReadDspCounter(DSP_SE_MAIN_COUNTER);
    u8CurrentSeISR = MDrv_AUDIO_ReadDspCounter(DSP_SE_ISR_COUNTER);
    u8CurrentDecR2While = MDrv_AUDIO_ReadDspCounter(DEC_R2_MAIN_COUNTER);
    u8CurrentDecR2Timer = MDrv_AUDIO_ReadDspCounter(DEC_R2_TIMER_COUNTER);
    u8CurrentSndR2While =  MDrv_AUDIO_ReadDspCounter(SND_R2_MAIN_COUNTER);
    u8CurrentSndR2Timer = MDrv_AUDIO_ReadDspCounter(SND_R2_TIMER_COUNTER);


    // Checking Decoder DSP counter
    if ( u8CurrentSeWhile == u8LastSeWhile )
    {
        u8SeWhileFailCount++;
    }
    else
    {
        u8SeWhileFailCount = 0;
    }

    if ( u8CurrentSeISR == u8LastSeISR )
    {
        u8SeISRFailCount++;
    }
    else
    {
        u8SeISRFailCount = 0;
    }

    if ( u8CurrentDecR2While == u8LastDecR2While)
    {
        u8DecR2WhileFailCount++;
    }
    else
    {
        u8DecR2WhileFailCount = 0;
    }

    if ( u8CurrentDecR2Timer == u8LastDecR2Timer)
    {
        u8DecR2TimerFailCount++;
    }
    else
    {
        u8DecR2TimerFailCount = 0;
    }

    if ( u8CurrentSndR2While == u8LastSndR2While)
    {
        u8SndR2WhileFailCount++;
    }
    else
    {
        u8SndR2WhileFailCount = 0;
    }

    if ( u8CurrentSndR2Timer == u8LastSndR2Timer)
    {
        u8SndR2TimerFailCount++;
    }
    else
    {
        u8SndR2TimerFailCount = 0;
    }

    if(((u8SeWhileFailCount+u8SeISRFailCount)==3)||((u8SndR2WhileFailCount+u8SndR2TimerFailCount)==3))
    {
        MsOS_DelayTask(1);    //Fix counter mis-detection problem
    }
    else if(((u8SeWhileFailCount+u8SeISRFailCount)==7)||((u8SndR2WhileFailCount+u8SndR2TimerFailCount)==7))
    {
        MsOS_DelayTask(1);
    }

    if ( (u8SeWhileFailCount >= 10) || (u8SeISRFailCount >= 10) )//Checking if rebooting SE DSP
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "u8SeWhileFailCount = %x\n",(MS_U16)u8SeWhileFailCount);
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "u8SeISRFailCount = %x\n",(MS_U16)u8SeISRFailCount);

        bRecoveryOn = TRUE;

        u8SeWhileFailCount = 0;
        u8SeISRFailCount = 0;
        u8DecR2WhileFailCount = 0;
        u8DecR2TimerFailCount = 0;
        u8SndR2WhileFailCount = 0;
        u8SndR2TimerFailCount = 0;

        MDrv_AUDIO_DspReboot(0);

        if (!MDrv_AUDIO_IsDTV())
        {
            //msAPI_AuWriteSIFThresholdTbl();
        }
    }
    else if ((u8DecR2WhileFailCount>=10)||(u8DecR2TimerFailCount>=10))//Checking if rebooting Decoder R2
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "u8DecR2WhileFailCount = %x\n",(MS_U16)u8DecR2WhileFailCount);
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "u8DecR2TimerFailCount = %x\n",(MS_U16)u8DecR2TimerFailCount);

        bRecoveryOn = TRUE;

        u8SeWhileFailCount = 0;
        u8SeISRFailCount = 0;
        u8DecR2WhileFailCount = 0;
        u8DecR2TimerFailCount = 0;
        u8SndR2WhileFailCount = 0;
        u8SndR2TimerFailCount = 0;

        MDrv_AUDIO_DspReboot(0);

        if (!MDrv_AUDIO_IsDTV())
        {
            //msAPI_AuWriteSIFThresholdTbl();
        }
    }
    else if ((bASND_R2_SUPPORT) &&
        ((u8SndR2WhileFailCount>=10)||(u8SndR2TimerFailCount>=10)))//Checking if rebooting SE R2
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "u8SndR2WhileFailCount = %x\n",(MS_U16)u8SndR2WhileFailCount);
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "u8SndR2TimerFailCount = %x\n",(MS_U16)u8SndR2TimerFailCount);

        bRecoveryOn = TRUE;

        u8SeWhileFailCount = 0;
        u8SeISRFailCount = 0;
        u8DecR2WhileFailCount = 0;
        u8DecR2TimerFailCount = 0;
        u8SndR2WhileFailCount = 0;
        u8SndR2TimerFailCount = 0;

        MDrv_AUDIO_DspReboot(0);

        if(!MDrv_AUDIO_IsDTV())
        {
            //msAPI_AuWriteSIFThresholdTbl();
        }
    }

    if (bRecoveryOn == TRUE)
    {
        MS_S32 s32DeviceId = 0;

        // check which PCM device is inused, then re-open it due to DSP is just reset
        for (s32DeviceId = 0; s32DeviceId < AUDIO_PCM_MAXNUM; s32DeviceId++)
        {
            if (g_AudioVars2->g_PcmInfo[s32DeviceId].u8ConnectFlag == TRUE)
            {
                AUDIO_PCM_INFO_t sPcmInfoTmp;

                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Warning! Re-Open PCM device (%d) due to DSP is just reset!\n", s32DeviceId);

                memset(&sPcmInfoTmp, 0x00, sizeof(AUDIO_PCM_INFO_t));
                sPcmInfoTmp.u32StructVersion = AUDIO_PCM_INFO_VERSION;
                sPcmInfoTmp.u32StructSize = sizeof(AUDIO_PCM_INFO_t);
                MDrv_AUDIO_PCM_Get(s32DeviceId, AUDIO_PCM_CMD_ALL, &sPcmInfoTmp);
                MDrv_AUDIO_PCM_Close(s32DeviceId);
                MDrv_AUDIO_PCM_Open(&sPcmInfoTmp);
            }
        }
    }

    // save status
    u8LastSeWhile = u8CurrentSeWhile;
    u8LastSeISR = u8CurrentSeISR;
    u8LastDecR2While = u8CurrentDecR2While;
    u8LastDecR2Timer = u8CurrentDecR2Timer;
    u8LastSndR2While = u8CurrentSndR2While;
    u8LastSndR2Timer = u8CurrentSndR2Timer;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetOutConnectivity()
/// @brief \b Function \b Description:  Set the TV output connectivity information from APP layer
///                                                   to audio driver .  Then audio driver will set the audio output
///                                                   port with these information automatically.
///////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetOutConnectivity(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    AUDIO_PATH_GROUP_TYPE Audio_pathgroup_status;
    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return;
    }

    Audio_pathgroup_status = MDrv_AUDIO_GetPathGroup();

    if (Audio_pathgroup_status == AUDIO_PATH_GROUP_1)
    {   // used in U3, T2, T4, Janus,
        if ( g_AudioVars2->AudioOutputInfo.SpeakerOut==AUDIO_I2S_OUTPUT )   // I2S Amp.
        {
            MDrv_AUDIO_SetOutputPath(AUDIO_PATH_MAIN, AUDIO_I2S_OUTPUT);
        }
        else                                                                               // Analog Amp
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.SpeakerOut, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            MDrv_AUDIO_SetInputPath(AUDIO_SRC_INPUT, AUDIO_PATH_0);
        }

        if ( g_AudioVars2->AudioOutputInfo.HpOut != AUDIO_NULL_OUTPUT )
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.HpOut, g_AudioVars2->AudioOutputInfo.HpOut);
        }

        if ( g_AudioVars2->AudioOutputInfo.MonitorOut != AUDIO_NULL_OUTPUT )
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.MonitorOut, g_AudioVars2->AudioOutputInfo.MonitorOut);
        }

        if ( g_AudioVars2->AudioOutputInfo.ScartOut != AUDIO_NULL_OUTPUT )
        {
            MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.ScartOut, g_AudioVars2->AudioOutputInfo.ScartOut);
        }
        MDrv_AUDIO_SetOutputPath(g_AudioVars2->AudioPathInfo.SpdifOut, AUDIO_SPDIF_OUTPUT);
    }
    else
    {   // used in U4, T3, T8, T9, T11, T12, T13, J2, A1, A2, A5,
     //Set path for Speaker out
        if (  g_AudioVars2->eAudioSource  == E_AUDIO_INFO_KTV_IN )
        {
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.HpOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.MonitorOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.ScartOut);
            MDrv_AUDIO_SetInternalPath(INTERNAL_MIXER, g_AudioVars2->AudioOutputInfo.SpdifOut);
        }
        else  // Normal case
        {
     //Set path for Speaker out
            if( (g_AudioVars2->AudioOutputSourceInfo.SpeakerOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT)) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_SE, g_AudioVars2->AudioOutputInfo.SpeakerOut);
            }
     //Set path for HP out
            if ( (g_AudioVars2->AudioOutputSourceInfo.HpOut == E_CONNECT_SUB) && ( g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.HpOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_TEST, g_AudioVars2->AudioOutputInfo.HpOut);
            }
     //Set path for Monitor out
            if ( (g_AudioVars2->AudioOutputSourceInfo.MonitorOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.MonitorOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_DELAY, g_AudioVars2->AudioOutputInfo.MonitorOut);
            }
     //Set path for Scart out
            if ( (g_AudioVars2->AudioOutputSourceInfo.ScartOut == E_CONNECT_SUB) || (g_AudioVars2->AudioOutputSourceInfo.ScartOut == E_CONNECT_SCART) )
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.ScartOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_DELAY, g_AudioVars2->AudioOutputInfo.ScartOut);
            }
     //Set path for SPDIF out
            if ( (g_AudioVars2->AudioOutputSourceInfo.SpdifOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.SpdifOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM, g_AudioVars2->AudioOutputInfo.SpdifOut); // For new mixer mode . AD sound is mixed in Inter_PCM
            }
     //Set path for ARC out
            if ( (g_AudioVars2->AudioOutputSourceInfo.ArcOut== E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT) ) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.ArcOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM, g_AudioVars2->AudioOutputInfo.ArcOut);
            }

            if( (g_AudioVars2->AudioOutputSourceInfo.HDMIOut == E_CONNECT_SUB) && (g_AudioVars2->eSubAudioSource != AUDIO_NULL_INPUT)) //PIP on case
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_SCART, g_AudioVars2->AudioOutputInfo.HDMIOut);
            }
            else
            {
                MDrv_AUDIO_SetInternalPath(INTERNAL_PCM_SE, g_AudioVars2->AudioOutputInfo.HDMIOut);
            }

            MDrv_AUDIO_SetInternalPath(INTERNAL_LRDIV2, AUDIO_I2S2_OUTPUT);
        }
    }

    MDrv_DBXTV_SetVolPath(g_AudioVars2->AudioOutputInfo.SpeakerOut);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetOutputInfo()
/// @brief \b Function \b Description:  Get audio output information from APP
/// @param pout_info   \b : Audio output path information structure pointer.
///                                    Please refer  AUDIO_OUT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetOutputInfo(AUDIO_OUT_INFO *pout_info)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioOutputInfo.SpeakerOut = pout_info->SpeakerOut;
    g_AudioVars2->AudioOutputInfo.HpOut = pout_info->HpOut;
    g_AudioVars2->AudioOutputInfo.MonitorOut = pout_info->MonitorOut;
    g_AudioVars2->AudioOutputInfo.ScartOut = pout_info->ScartOut;

    if(pout_info->SpeakerOut == AUDIO_SPDIF_OUTPUT)
    {
        g_AudioVars2->AudioOutputInfo.SpdifOut = AUDIO_NULL_OUTPUT;
    }
    else
    {
    g_AudioVars2->AudioOutputInfo.SpdifOut = AUDIO_SPDIF_OUTPUT;
    }

    g_AudioVars2->AudioOutputInfo.ArcOut = AUDIO_HDMI_ARC_OUTPUT;
    g_AudioVars2->AudioOutputInfo.HDMIOut = AUDIO_HDMI_OUTPUT;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSystemInfo()
/// @brief \b Function \b Description:  Function for Driver layer to get the system compile option
///                                                  (TV system , Audio system ....)
/// @param pau_info    \b : Audio output path information structure pointer
///                                    Please refer  AUDIO_INIT_INFO   in header file
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSystemInfo(AUDIO_INIT_INFO *pau_info)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioSystemInfo.tv_system = pau_info->tv_system;
    g_AudioVars2->AudioSystemInfo.dtv_system = pau_info->dtv_system;
    g_AudioVars2->AudioSystemInfo.au_system_sel = pau_info->au_system_sel;
    g_AudioVars2->AudioSystemInfo.miu = pau_info->miu;

    if (MDrv_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_DVB)
            MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_DVB);
        else if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ATSC)
            MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_ATSC);
        else if (g_AudioVars2->AudioSystemInfo.dtv_system == AUDIO_DTV_ISDB)
            MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_ISDB);
        else
            MDrv_AUDIO_SPDIF_SetChannelStatus(SPDIF_CHANNEL_STATUS_CATEGORY,SPDIF_CS_CATEGORY_NONE);

        MDrv_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_LoudnessMatching, (MS_U32)g_AudioVars2->g_dolbyLoudness_Enable, (MS_U32)g_AudioVars2->AudioSystemInfo.dtv_system);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPathInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param path_info   \b : path information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPathInfo(AUDIO_PATH_INFO *path_info)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioPathInfo.SpeakerOut=path_info->SpeakerOut;
    g_AudioVars2->AudioPathInfo.HpOut=path_info->HpOut;
    g_AudioVars2->AudioPathInfo.MonitorOut=path_info->MonitorOut;
    g_AudioVars2->AudioPathInfo.ScartOut=path_info->ScartOut;
    g_AudioVars2->AudioPathInfo.SpdifOut=path_info->SpdifOut;
    g_AudioVars2->AudioPathInfo.ArcOut=AUDIO_PATH_NULL;
    g_AudioVars2->AudioPathInfo.HDMIOut= AUDIO_T3_PATH_HDMI;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPathInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param path_info   \b : path information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_GetPathInfo(AUDIO_PATH_INFO *path_info)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    path_info->SpeakerOut=g_AudioVars2->AudioPathInfo.SpeakerOut;
    path_info->HpOut=g_AudioVars2->AudioPathInfo.HpOut;
    path_info->MonitorOut=g_AudioVars2->AudioPathInfo.MonitorOut;
    path_info->ScartOut=g_AudioVars2->AudioPathInfo.ScartOut;
    path_info->SpdifOut=g_AudioVars2->AudioPathInfo.SpdifOut;
    path_info->ArcOut=g_AudioVars2->AudioPathInfo.ArcOut;
    path_info->HDMIOut=g_AudioVars2->AudioPathInfo.HDMIOut;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetOutputSourceInfo()
/// @brief \b Function \b Description:  Set audio output source information
/// @param path_info   \b : output port source information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->AudioOutputSourceInfo.SpeakerOut = pSourceInfo->SpeakerOut;
    g_AudioVars2->AudioOutputSourceInfo.HpOut = pSourceInfo->HpOut;
    g_AudioVars2->AudioOutputSourceInfo.MonitorOut = pSourceInfo->MonitorOut;
    g_AudioVars2->AudioOutputSourceInfo.ScartOut = pSourceInfo->ScartOut;
    g_AudioVars2->AudioOutputSourceInfo.SpdifOut = pSourceInfo->SpdifOut;
    g_AudioVars2->AudioOutputSourceInfo.ArcOut = pSourceInfo->ArcOut;
    g_AudioVars2->AudioOutputSourceInfo.HDMIOut = pSourceInfo->HDMIOut;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetOutputSourceInfo()
/// @brief \b Function \b Description:  Get audio output path information from APP
/// @param pSourceInfo   \b : output port source information structure pointer
///                                    for Speaker , Head phone , Monitor out , Scart out and SPDIF out
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_GetOutputSourceInfo(OUTPUT_SOURCE_INFO *pSourceInfo)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    memcpy(pSourceInfo, &(g_AudioVars2->AudioOutputSourceInfo), sizeof(g_AudioVars2->AudioOutputSourceInfo));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSourceInfo()
/// @brief \b Function \b Description:  For driver layer to get audio source information from APP
/// @param eSourceType   \b : information structure pointer ==>AUDIO_SOURCE_INFO_TYPE
///                         - E_AUDIO_INFO_DTV_IN,
///                         - E_AUDIO_INFO_ATV_IN,
///                         - E_AUDIO_INFO_HDMI_IN,
///                         - E_AUDIO_INFO_ADC_IN,
///                         - E_AUDIO_INFO_MM_IN,
///                         - E_AUDIO_INFO_SPDIF_IN,
///                         - E_AUDIO_INFO_KTV_IN,
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSourceInfo(AUDIO_SOURCE_INFO_TYPE eSourceType)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    g_AudioVars2->eAudioSource = eSourceType;

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_InputSwitch()
/// @brief \b Function \b Description:  Set audio Input connectivity (for S7 serious only)
/// @param enSource   \b :  Please refer  AUDIO_INPUT_TYPE  enum in header file
///@param  enGroup  \b : Please refer  AUDIO_SWITCH_GROUP  enum in header file
///////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_InputSwitch(AUDIO_INPUT_TYPE enSource, AUDIO_SWITCH_GROUP enGroup)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    AUDIO_PATH_GROUP_TYPE Audio_pathgroup_status;
    MS_U8  u8input_src = LONIBBLE(enSource);

    APIAUDIO_CHECK_SHM_INIT;

    Audio_pathgroup_status = MDrv_AUDIO_GetPathGroup();

    if (Audio_pathgroup_status == AUDIO_PATH_GROUP_1)
    {   // used in U3, T2, T4, Janus,
        if(enGroup==E_AUDIO_GROUP_MAIN)
        {
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_MAIN);
            MDrv_AUDIO_SetInputPath(enSource , g_AudioVars2->AudioPathInfo.MonitorOut);
            MDrv_AUDIO_SetInputPath(enSource , g_AudioVars2->AudioPathInfo.SpdifOut);
        }
        else  if(enGroup==E_AUDIO_GROUP_SCART)
        {
            MDrv_AUDIO_SetInputPath(enSource , g_AudioVars2->AudioPathInfo.ScartOut);
        }

        if ( g_AudioVars2->eAudioSource == E_AUDIO_INFO_KTV_IN )
        {
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n==Source=KTV ==\r\n");
            MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_KTV);
        }
        else if ( g_AudioVars2->eAudioSource == E_AUDIO_INFO_GAME_IN )
        {
            MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_GAME);
        }
        else
        {
            MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_DTV);
        }
    }
    else
    {   // used in U4, T3, T8, T9, T11, T12, T13, J2, A1, A2, A5,
        if(enGroup==E_AUDIO_GROUP_MAIN)
        {
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_4); // fixed in this path
            g_AudioVars2->eMainAudioSource = enSource;
        }
        else  if(enGroup==E_AUDIO_GROUP_SCART)
        {
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_6); //fixed in this path
            g_AudioVars2->eScartAudioSource = enSource;
        }
        else if(enGroup==E_AUDIO_GROUP_SUB)  // PIP SUB or HP source
        {
            /*
             *
             * In TVOS environment, if PIP on and MM is main, Ch5 input should set to another NULL source.
             * Otherwise, sub input source sound would be mixed together to main input source.
             */
            if (((g_AudioVars2->eAudioSource == E_AUDIO_INFO_MM_IN) || (g_AudioVars2->eAudioSource == E_AUDIO_INFO_GAME_IN)) &&
                (MDrv_MAD_GetDecCmd(DVB_Audio_Decoder2) == AU_DVB_DECCMD_PLAYFRAME_GS))
            {
                if (u8input_src == AUDIO_DSP1_INPUT)
                {
                    MDrv_AUDIO_SetInputPath(AUDIO_DSP3_DVB_INPUT , AUDIO_PATH_4);
                }
                else if (u8input_src == AUDIO_DSP3_INPUT)
                {
                    MDrv_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT , AUDIO_PATH_4);
                }
            }

            switch(enSource)  // Use 2nd ADC in sub mode
            {
               case AUDIO_AUIN0_INPUT:
                   enSource=AUDIO_ADC2_AUIN0_INPUT;
                   break;
               case AUDIO_AUIN1_INPUT:
                   enSource=AUDIO_ADC2_AUIN1_INPUT;
                   break;
               case AUDIO_AUIN2_INPUT:
                   enSource=AUDIO_ADC2_AUIN2_INPUT;
                   break;
               case AUDIO_AUIN3_INPUT:
                   enSource=AUDIO_ADC2_AUIN3_INPUT;
                   break;
               case AUDIO_AUIN4_INPUT:
                   enSource=AUDIO_ADC2_AUIN4_INPUT;
                   break;
               case AUDIO_AUIN5_INPUT:
                   enSource=AUDIO_ADC2_AUIN5_INPUT;
                   break;
               case AUDIO_AUMIC_INPUT:
                   enSource=AUDIO_ADC2_AUMIC_INPUT;
                   break;
               default:
                   break;
            }
            MDrv_AUDIO_SetInputPath(enSource , AUDIO_PATH_6); // PIP shared the same path with Scart
            g_AudioVars2->eSubAudioSource = enSource;
        }

        // Set output source
        switch ( g_AudioVars2->eAudioSource )
        {
            case E_AUDIO_INFO_KTV_IN:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_KTV);
                break;

            case E_AUDIO_INFO_GAME_IN:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_GAME);
                break;

            case E_AUDIO_INFO_KTV_STB_IN:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_KTV_STB);
                break;

            case E_AUDIO_INFO_ATV_IN:
                // ATV should not effected by entertainment mode
                break;

            default:
                MDrv_AUDIO_SetEntertainmentMode(AUDIO_ETMODE_DTV);
                break;
        }

        MDrv_AUDIO_InputSwitch(enGroup);

        //Reconnect audio output port again .
        _MApi_AUDIO_SetOutConnectivity();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ReadMailBox()
/// @brief \b Function \b Description:  This routine is used to read the DSP mail box value
/// @param bDspType    \b : Select audio DEC/SE DSP
/// @param u8ParamNum  \b : Mail box address
/// @return MS_U16     \b : Mail Box value
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if(bDspType==DSP_DEC)
    {
        return (MDrv_AUDIO_ReadDecMailBox(u8ParamNum));
    }
    else
    {
        return (MDrv_AUDIO_ReadSeMailBox(u8ParamNum));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_WriteDecMailBox()
/// @brief \b Function \b Description:  This routine is used to write DSP mail box
/// @param bDspType    \b : Select audio DEC/SE DSP
/// @param u8ParamNum  \b : Mail box address
/// @param u16Data     \b : value
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if(bDspType==DSP_DEC)
    {
        _MDrv_AUDIO_WriteDecMailBox(u8ParamNum, u16Data);
    }
    else
    {
        MDrv_AUDIO_WriteSeMailBox(u8ParamNum, u16Data);
    }

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FwTriggerDSP()
/// @brief \b Function \b Description:  This routine send a PIO interrupt to DSP with a command .
/// @param u16Cmd      \b :    0xF0-- for MHEG5 file protocol
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FwTriggerDSP(MS_U16 u16Cmd)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_FwTriggerDSP(u16Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetFwStatus()
/// @brief \b Function \b Description:  FW set DSP load/reload code from DDR or Flash
/// @param bBootOnDDR  \b :
///                     - TRUE==> Boot from DDR;
///                     - FALSE==> Boot from Flash
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetFwStatus(MS_BOOL bBootOnDDR)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_DspBootOnDDR(bBootOnDDR);
    //audio_FW_Status = bBootOnDDR;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ExitAudioSystem()
/// @brief \b Function \b Description: This routine is used to exit audio system
///                                  by performing DSP power down setting.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ExitAudioSystem(void)
{
    MS_BOOL bRet = FALSE;

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);

    if (bRet == FALSE)
    {
        return;
    }

    MDrv_AUDIO_SetPowerOn(FALSE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_RebootDsp(void)
/// @brief \b Function \b Description:  This routine reboot DSP.
/// @param bDspType    \b : 0- DEC DSP; 1- SNDEff DSP
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_RebootDsp(MS_BOOL bDspType)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_RebootDSP(bDspType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SendIntrupt()
/// @brief \b Function \b Description: This routine send a PIO8 interrupt to DSP.
/// @param bDspType    \b :  0- DEC DSP; 1- SNDEff DSP
/// @param u8Cmd       \b :  0xE0, for MHEG5 file protocol
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SendIntrupt(MS_BOOL bDspType,MS_U8 u8Cmd)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SendIntrupt(bDspType,u8Cmd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_I2S_SetMode()
/// @brief \b Function \b Description:  This routine is used to Set the I2S output mode.
/// @param u8Mode      \b : I2S mode select
///                       - AUDIO_I2S_MCLK               ==>Set I2S MCLK
///                       - AUDIO_I2S_WORD_WIDTH, ==>Set I2S Word-width
///                       - AUDIO_I2S_SOURCE_CH,    ==>Set I2S channel Source
///                       - AUDIO_I2S_FORMAT            ==>Set  I2S format
/// @param u8Val      \b : I2S mode value
///                      - I2S_MCLK_64FS=0x04,               ==> MCLK=64 Fs ;
///                      - I2S_MCLK_128FS=0x05,             ==> MCLK=128 Fs ;
///                      - I2S_MCLK_256FS=0x06,             ==> MCLK=256 Fs ; default setting
///                      - I2S_MCLK_384FS=0x07,             ==> MCLK=384 Fs ;
///                      - I2S_WORD_WIDTH_16BIT=0x4,  ==>Word width=16bit;
///                      - I2S_WORD_WIDTH_24BIT=0x5,  ==>Word width=24bit;
///                      - I2S_WORD_WIDTH_32BIT=0x6,  ==>Word width=32bit; default setting
///                      - I2S_FORMAT_STANDARD=0,       ==>I2S  in Standard format
///                      - I2S_FORMAT_LEFT_JUSTIFIED=1 ==> I2S in Left-justify format
///////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_I2S_SetMode(AUDIO_I2S_MODE_TYPE u8Mode, AUDIO_I2S_MODE_VALUE  u8Val)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

   MDrv_AUDIO_I2S_SetMode((MS_U8)u8Mode, (MS_U8)u8Val);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_WritePreInitTable()
/// @brief \b Function \b Description: This routine is to set power on DAC sequence before setting init table.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void _MApi_AUDIO_WritePreInitTable(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    MDrv_AUDIO_InitMMIO();

    if (MDrv_AUDIO_SHM_Init() == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : fails to init Audio SHM\n", __FUNCTION__);
        return;
    }

    if (MDrv_AUDIO_Mutex_Init() == FALSE)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : fails to init Audio Mutex\n", __FUNCTION__);
        return;
    }

    OS_OBTAIN_MUTEX(_s32AUDIOMutex, MSOS_WAIT_FOREVER);

    if (MDrv_AUDIO_GET_INIT_FLAG() == TRUE)
    {
        OS_RELEASE_MUTEX(_s32AUDIOMutex);
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "%s() : doesn't need to init again\n", __FUNCTION__);
        return;
    }

    if (g_AudioVars2->g_Audio_PreInitFlag == FALSE)
    {
        MDrv_AUDIO_SwResetMAD();

        MDrv_AUDIO_WritePreInitTable();
        g_AudioVars2->g_Audio_PreInitFlag = TRUE;
    }

    OS_RELEASE_MUTEX(_s32AUDIOMutex);

    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to enable earphone low power stage.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void _MApi_AUDIO_EnaEarphone_LowPower_Stage(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_EnaEarphone_LowPower_Stage();
    MDrv_AUDIO_EnaEarphone_HighDriving_Stage();
    return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPowerOn()
/// @brief \b Function \b Description: This routine is used to enable earphone high driving stage.
////////////////////////////////////////////////////////////////////////////////
//Refine power on sequence for earphone & DAC pop noise issue
void _MApi_AUDIO_EnaEarphone_HighDriving_Stage(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

	AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "-->>NEW POWER ON SEQ: MApi_AUDIO_EnaEarphone_HighDriving_Stage\n\r");
    MDrv_AUDIO_EnaEarphone_HighDriving_Stage();
    return;
}

////////////////////////////////////////////////////////////////////////////////
//
//          AUDIO_DTV RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSystem()
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
MS_BOOL _MApi_AUDIO_SetSystem(En_DVB_decSystemType enDecSystem)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "apiAud>> enDecSystem %d enCurDecSystem = %d\n", (MS_U8)enDecSystem, (MS_U8)enCurDecSystem);

    if ((enDecSystem & 0xf0) == 0xb0)
    {
        bRet = MDrv_SIF_SetSystem(enDecSystem);
    }
    else
    {
        if (enDecSystem >= 0x20)
        {
            enCurDecSystem2 = enDecSystem;
        }
        else
        {
            enCurDecSystem = enDecSystem;
        }
        bRet = MDrv_AUDIO_SetSystem(enDecSystem);
    }

    return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDecodeSystem()
/// @brief \b Function \b Description:  This function will set DecId decoder as  structure p_AudioDecStatus
/// @param\b DecId  \b : Decoder ID to be controled
/// @param\b p_AudioDecStatus  \b : decoder status structure pointer
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if (NULL == p_AudioDecStatus)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : AudioDecStatus is null\n", __FUNCTION__);
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Invalid Dec ID = %d,  %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    if ((MSAPI_AUD_DVB_INVALID == p_AudioDecStatus->eAudFormat) ||
        (MSAPI_AUD_DVB_NONE == p_AudioDecStatus->eAudFormat))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "Invalid Decoder Type %s() %d\n",__FUNCTION__,__LINE__);
        return bRet;
    }

    if (FALSE == g_AudioVars2->AudioDecStatus[DecId].bConnect)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "DecID %d not connected %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    bRet = MDrv_AUDIO_SetDecodeSystem(DecId, p_AudioDecStatus);

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDecodeSystem()
/// @brief \b Function \b Description:  This function will get DecId decoder status
/// @param DecId  \b : Decoder ID to get status
/// @param p_AudioDecStatus  \b : return status to p_AudioDecStatus pointer
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetDecodeSystem(AUDIO_DEC_ID DecId,  AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if (NULL == p_AudioDecStatus)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : AudioDecStatus is null\n", __FUNCTION__);
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Invalid Dec ID %d %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    if (FALSE == g_AudioVars2->AudioDecStatus[DecId].bConnect)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "DecID %d not connected %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return bRet;
    }

    // copy Dec status
    memcpy(p_AudioDecStatus,&g_AudioVars2->AudioDecStatus[DecId], sizeof(AudioDecStatus_t));

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_OpenDecodeSystem()
/// @brief \b Function \b Description:  This function will open decode system according audio dec status
/// @param p_AudioDecStatus  \b : decoder status stucture
/// @return\b AUDIO_DEC_ID  \b : return available decoder ID
////////////////////////////////////////////////////////////////////////////////
AUDIO_DEC_ID _MApi_AUDIO_OpenDecodeSystem(AudioDecStatus_t * p_AudioDecStatus)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    AUDIO_DEC_ID DecRet = AU_DEC_INVALID;
    MS_BOOL init_flag = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(init_flag);
    if (init_flag == FALSE)
    {
        return DecRet;
    }

    if (NULL == p_AudioDecStatus)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : p_AudioDecStatus is NULL\n", __FUNCTION__);
        return DecRet;
    }

    if ((MSAPI_AUD_DVB_INVALID == p_AudioDecStatus->eAudFormat) ||
        (MSAPI_AUD_DVB_NONE == p_AudioDecStatus->eAudFormat))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() %d\n",__FUNCTION__,__LINE__);
        return DecRet;
    }

    if (p_AudioDecStatus->bIsAD) // Decide AD decoder
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() %d\n",__FUNCTION__,__LINE__);
    }

    if ((p_AudioDecStatus->eSourceType == E_AUDIO_INFO_ADC_IN) ||
       (p_AudioDecStatus->eSourceType == E_AUDIO_INFO_SPDIF_IN))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() %d\n",__FUNCTION__,__LINE__);
        return DecRet;
    }

    // Open Decode system
    DecRet = MDrv_AUDIO_OpenDecodeSystem(p_AudioDecStatus);

    if (DecRet != AU_DEC_INVALID)
    {
        switch (p_AudioDecStatus->eSourceType)
        {
            case E_AUDIO_INFO_ATV_IN:
                if (DecRet == AU_DEC_ID1)
                {
                    g_AudioVars2->g_u8SifDspType = DSP_DEC;
                }
                else
                {
                    g_AudioVars2->g_u8SifDspType = DSP_SE;
                }
                break;

            case E_AUDIO_INFO_HDMI_IN:
                if (DecRet == AU_DEC_ID3)
                {
                    MDrv_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE, TRUE, 0);
                }
                else
                {
                    MDrv_AUDIO_SetAC3PInfo(Audio_AC3P_infoType_hdmiAC3inSE, FALSE, 0);
                }
                break;

            default:
                break;
        }

        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "DecRet = %d %s() %d \n",DecRet,__FUNCTION__,__LINE__);

        // reload DSP code, and back up AudioDecStatus
        MDrv_AUDIO_SetDecodeSystem(DecRet, p_AudioDecStatus);

        // Update status
        g_AudioVars2->AudioDecStatus[DecRet].bConnect = TRUE;

        if (DecRet == AU_DEC_ID3)
        {
            g_AudioVars2->AudioDecStatus[DecRet].eDSPId = AUDIO_DSP_ID_SND;
        }
        else if (DecRet == AU_DEC_ID1)
        {
            g_AudioVars2->AudioDecStatus[DecRet].eDSPId = AUDIO_DSP_ID_DEC;
        }
        else if (DecRet == AU_DEC_ID2)
        {
             // AU_DEC_ID2 only for ATV SIF
             // For other case should not use AU_DEC_ID2
            // Don't know how to set for ATV case
            g_AudioVars2->AudioDecStatus[DecRet].eDSPId = AUDIO_DSP_ID_ALL;
        }

    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "Dec Id %d opened %s() %d \n",DecRet,__FUNCTION__,__LINE__);
    return DecRet;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDecodeCmd()
/// @brief \b Function \b Description:  This function will send decode command to DecId decoder
/// @param DecId  \b : Decoder ID to be controled
/// @param enDecComamnd  \b : decoder command
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetDecodeCmd(AUDIO_DEC_ID DecId, En_DVB_decCmdType enDecComamnd)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        return FALSE;
    }

    if (FALSE == g_AudioVars2->AudioDecStatus[DecId].bConnect)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "DecID %d not connected %s() %d\n",DecId,__FUNCTION__,__LINE__);
        return FALSE;
    }

    //Convert Api dec command to Drv dec command
    AU_DVB_DECCMD u8DecCmd;
    _ApiDecCmd_To_DrvDecCmd(enDecComamnd,&u8DecCmd);

    MDrv_MAD_DecoderCmd(DecId,u8DecCmd);
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ReleaseDecodeSystem()
/// @brief \b Function \b Description:  This function will release/close decode system
/// @param DecId  \b : Decoder ID to be released
/// @return\b MS_BOOL  \b : set decoder system process status FALSE-> fail TRUE-> Success
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ReleaseDecodeSystem(AUDIO_DEC_ID DecId)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if ((AU_DEC_INVALID == DecId) || (AU_DEC_MAX == DecId))
    {
        return bRet;
    }

    // Stop when release
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "DecID %d disconnect %s() %d\n",DecId,__FUNCTION__,__LINE__);
    MDrv_MAD_DecoderCmd(DecId,AU_DVB_DECCMD_STOP);
    memset(&g_AudioVars2->AudioDecStatus[DecId],  0, sizeof(AudioDecStatus_t));

    return bRet;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDecSysSupportStatus()
/// @brief \b Function \b Description: This function will report Audio Decoder support feature status when giving DecSystem type
/// @param <IN>        \b   enDecSystem : Decoder type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetDecSysSupportStatus(En_DVB_decSystemType enDecSystem)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_GetDecSysSupportStatus(enDecSystem));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetH264StreamID_Mod()
/// @brief \b Function \b Description: This routine is used to AC3 Stream ID mod.
/// @param stream_id_mod  \b : DTV stream ID
///                          - stream_id_mod = 0, video MPEG mode;
///                          - stream_id_mod = 1, video H264 mode
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_SetH264StreamID_Mod(MS_U8 stream_id_mod)    //Non-Used
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    //MDrv_MAD_SetH264StreanID_Mod(stream_id_mod);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_IsMadLock()
/// @brief \b Function \b Description: This function show DTV Audio stream lock status
/// @return MS_BOOL    \b :       1 : lock  ; 0 : unlock
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_IsMadLock(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DecStatus);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDecStatus()
/// @brief \b Function \b Description: This routine is used to get the decoder's status (paly or stop)
/// @return En_DVB_decCmdType \b : Decoder status (paly or stop)
////////////////////////////////////////////////////////////////////////////////
En_DVB_decCmdType _MApi_AUDIO_GetDecStatus(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return enDecoderStatus;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetCommand()
/// @brief \b Function \b Description: Set decoder Command for Digital Audio module
/// @param enDecComamnd \b : deccoder command for DVB Audio
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetCommand(En_DVB_decCmdType enDecComamnd)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return;
    }

    switch ( enDecComamnd )
    {
        //////////////////////////////////////////////////////
        //
        //     Command for Audio decoder 1
        //
        //////////////////////////////////////////////////////
        case MSAPI_AUD_DVB_DECCMD_PLAY:
            enDecoderStatus = MSAPI_AUD_DVB_DECCMD_PLAY;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY);
            break;

        case MSAPI_AUD_DVB_DECCMD_STOP:
            enDecoderStatus = MSAPI_AUD_DVB_DECCMD_STOP;
            g_AudioVars2->g_bMMPlayFlag = FALSE;
            u8IntTag = 1;
            MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);    // Clear file mode interrupt variable
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STOP);
            break;

        case MSAPI_AUD_DVB_DECCMD_RESYNC:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_RESYNC);
            break;

        case MSAPI_AUD_DVB_DECCMD_FREE_RUN:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_FREERUN);
            break;

        case MSAPI_AUD_DVB_DECCMD_AVSYNC:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_AVSYNC);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILE:     // start/continue playing file based audio in MHEG5
            MDrv_AUDIO_HDMI_AC3_PathCFG(FALSE);
            g_AudioVars2->g_bMMPlayFlag = TRUE;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PAUSE:    // pause playing file based audio in MHEG5
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PAUSE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFILETSP:     // start/continue playing file based audio in MHEG5
            MDrv_AUDIO_HDMI_AC3_PathCFG(FALSE);
            g_AudioVars2->g_bMMPlayFlag = TRUE;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILETSP);
            break;

        case MSAPI_AUD_DVB_DECCMD_STARTBROWSE:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_STARTBROWSE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME:
                g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = 0;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_STAGE:
                g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = 0;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME_STAGE);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYFRAME_GS:
                g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = 0;
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFRAME_GS);
            break;

        case MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2:
            MDrv_AUDIO_HDMI_AC3_PathCFG(FALSE);
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAYFILE2_PTS);
            break;

        case MSAPI_AUD_DVB_DECCMD_SKIP:
            MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_SKIP);
            break;
        //////////////////////////////////////////////////////
        //
        //     Command for Audio decoder 2
        //
        //////////////////////////////////////////////////////
        case MSAPI_AUD_DVB2_DECCMD_PLAY:
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PLAY);
            break;

        case MSAPI_AUD_DVB2_DECCMD_STOP:
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_STOP);
            break;

        case MSAPI_AUD_DVB2_DECCMD_RESYNC:
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_RESYNC);
            break;

        case MSAPI_AUD_DVB2_DECCMD_FREE_RUN:
            MDrv_MAD2_SetFreeRun(AU_DVB2_FreeRunMode_FreeRun);
            break;

        case MSAPI_AUD_DVB2_DECCMD_AVSYNC:
            MDrv_MAD2_SetFreeRun(AU_DVB2_FreeRunMode_AVsync);
            break;

        case MSAPI_AUD_DVB2_DECCMD_PLAYFILE:     // start/continue playing file based audio in MHEG5
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PLAYFILE);
            break;

        case MSAPI_AUD_DVB2_DECCMD_PAUSE:    // pause playing file based audio in MHEG5
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PAUSE);
            break;

        case MSAPI_AUD_DVB2_DECCMD_PLAYFILETSP:     // start/continue playing file based audio in MHEG5
            MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD_PLAYFILETSP);
            break;

        default:
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetCommand()
/// @brief \b Function \b Description: This routine is to get  the DVB1 decode command.
/// @return AU_DVB_DECCMD \b :
///                          - AU_DVB_DECCMD_STOP,        //0
///                         - AU_DVB_DECCMD_PLAY,        //1
///                         - AU_DVB_DECCMD_PLAYFILETSP, //2
///                          - AU_DVB_DECCMD_RESYNC,
///                                 ....etc
///////////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD _MApi_AUDIO_GetCommand(En_DVB_AudioDecoder AdoDecoderNo)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if ((AdoDecoderNo != DVB_Audio_Decoder1) && (AdoDecoderNo != DVB_Audio_Decoder2))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] Error! Not support AdoDecoderNo(%u)!\n", __FUNCTION__,AdoDecoderNo);
        return 0;
    }

    return MDrv_MAD_GetDecCmd(AdoDecoderNo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetMAD_LOCK()
/// @brief \b Function \b Description: This routine is used to report decoder lock information.
/// @retun MS_U8      \b the decoder status
///////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetMAD_LOCK(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U8   lock_id_reg = 0;
    MS_BOOL mad_lock_status = FALSE;

    lock_id_reg = MDrv_MAD_Rpt_DTVES() & 0x0F;

    if (lock_id_reg == 0x0)
    {
        mad_lock_status = FALSE;
    }
    else
    {
        mad_lock_status = TRUE;
    }

    return  mad_lock_status;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetADOutputMode()
/// @brief \b Function \b Description: Set AD output mode
/// @param out_mode    \b : out_mode:
///                       - AD_OUT_SPEAKER
///                       - AD_OUT_HP
///                       - AD_OUT_BOTH
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetADOutputMode (MS_U8 out_mode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    switch (out_mode)
    {
    case AD_OUT_SPEAKER:
    case AD_OUT_HP:
    case AD_OUT_BOTH:
        MDrv_MAD_SetAdMixMode (AD_MIX_MAIN, MIX_ON);   // Decoder2 mix
        break;

    case AD_OUT_NONE:
    default:
        MDrv_MAD_SetAdMixMode (AD_MIX_MAIN, MIX_OFF);  // Decoder2 don't mix
        break;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetADAbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of AD.
/// @param AbsVolume   \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetADAbsoluteVolume (MS_U8 AbsVolume)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetAdAbsoluteVolume(AbsVolume, 0);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADSetMute()
/// @brief \b Function \b Description: This routine is used to set AD mute.
/// @param EnMute      \b :     TRUE --Mute AD;
///                             FALSE--Unmute AD
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_ADSetMute(MS_BOOL EnMute)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetAdMute(EnMute);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_PVR RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_SetEncodeInit()
/// @brief \b Function \b Description:  This function is used to set Encod init
/// @param BitRate \b :   Encode Output Bitrate
///                      -  AU_DVB_ENCBR_192K,    // 0
///                      -  AU_DVB_ENCBR_384K,    // 1
///                      -  AU_DVB_ENCBR_128K,    // 2
///                      -  AU_DVB_ENCBR_64K      // 3
/// @param EncFrameNum \b :   Frame number per interrupt
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEncInit(AU_DVB_ENCBR BitRate, MS_U8 u8EncFrameNum)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SetEncodeInit(BitRate, u8EncFrameNum);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function   \b Name: MApi_AUDIO_SetEncCommand()
/// @brief \b Function   \b Description: Set Encoder Command for Digital Audio module
/// @param enEnccComamnd \b : Encoder command for DVB Audio
///                                       -   AU_DVB_ENCCMD_STOP,      //0
///                                       -   AU_DVB_ENCCMD_START      // 1
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEncCommand(AU_DVB_ENCCMD enEncComamnd)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_MAD_SetEncCmd(enEncComamnd);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_GetEncodeFrameInfo()
/// @brief \b Function \b Description:  This function is used to get the Encoded frame info
/// @param input       \b EncFrameInfo:   structure to store encoded frame info
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_GetEncodeFrameInfo(AU_DVB_ENC_FRAME_INFO *EncFrameInfo)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_GetEncodeFrameInfo(EncFrameInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetEncodeDoneFlag()
/// @brief \b Function \b Description:  This function is used to get check encode done flag
/// @param input       \b None
/// @param output      \b BOOL  TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_GetEncodeDoneFlag(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return (MDrv_AUDIO_GetEncodeDoneFlag());
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_ATV RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetCommand()
/// @brief \b Function \b Description: Set decoder Command for ATV module
/// @param enDecComamnd \b : deccoder command for ATV AUDIO
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SendCmd(En_AUD_SIF_CmdType enAudSifCommand, MS_U8 comm_arg1, MS_U8 comm_arg2)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    switch( enAudSifCommand )
    {
        case MSAPI_AUD_SIF_CMD_SET_STOP:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_STOP, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_PLAY:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_PLAY, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_HIDEV:
            // comm_arg1 = TRUE or FALSE.
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_HIDEV, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL:
            // comm_arg1 = MSAPI_AUD_SIF_HIDEV_FILTER_BW_L1 ~ MSAPI_AUD_SIF_HIDEV_FILTER_BW_L3 or MSAPI_AUD_SIF_HIDEV_FILTER_BW_DEFAULT
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_HIDEV_FILTER_BW_LEVEL, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_RESET_FC_TRACKING:
            MDrv_SIF_SendCmd(AU_SIF_CMD_RESET_FC_TRACKING, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_FC_TRACKING:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ADC_FROM_VIF_PATH:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ADC_FROM_VIF_PATH, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_AUTO_MUTE:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_MUTE, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_AUTO_SOUNDMODE:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AUTO_SOUNDMODE, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_BEEPER_FUNCTION:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_BEEPER_FUNCTION, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BEEPER_TONE:
            // comm_arg1 = tone, comm_arg2 = volume
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BEEPER_TONE, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_ENABLE_AGC:
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_RESET_AGC:
            MDrv_SIF_SendCmd(AU_SIF_CMD_RESET_AGC, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_STANDARD_RESET:
            MDrv_SIF_SendCmd(AU_SIF_CMD_STANDARD_RESET, NULL, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_DETECT_MAIN_STD_ONLY:
            MDrv_SIF_SendCmd(AU_SIF_CMD_DETECT_MAIN_STD_ONLY, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_FMTX_SET_PREEMP:
            MDrv_SIF_SendCmd(AU_SIF_CMD_FMTX_SET_PREEMP, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ENC_CMD:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ENC_CMD, comm_arg1, NULL);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ENC_Input_attenuation:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ENC_Input_attenuation, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_ENC_Output_scaling:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_ENC_Output_scaling, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_M_Gain:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BTSC_Enc_M_Gain, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_D_Gain:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BTSC_Enc_D_Gain, comm_arg1, comm_arg2);
            break;
        case MSAPI_AUD_SIF_CMD_SET_BTSC_Enc_SAP_Gain:
            MDrv_SIF_SendCmd(AU_SIF_CMD_SET_BTSC_Enc_SAP_Gain, comm_arg1, comm_arg2);
            break;
        default:
            break;
    }
}

//******************************************************************************
//Function name:    MApi_AUDIO_SIF_SetPrescale
//Passing  para:    gain_type:  MSAPI_BTSC_GAIN
//                          MSAPI_FM_StdM_GAIN
//                  db_value:   The prescale value, unit is 0.25dB.
//                                  0: 0db, 1:  0.25dB,  2:  0.5dB, ...,   4: 1.0dB, ...,   8:  2dB
//                              -1:-0.25dB, -2:-0.5dB, ..., -4:-1.0dB, ..., -8:-2dB
//Return parameter: none
//Description:          SIF output level = (original level add db_value gain)
//******************************************************************************
void _MApi_AUDIO_SIF_SetPrescale(MS_U8 gain_type, MS_S32 db_value)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SIF_SetPrescale((AUDIO_SIF_GAIN_TYPE)gain_type, db_value);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_IsPALType()
/// @brief \b Function \b Description: Check the DSP type is pal_type or not
/// @return MS_BOOL    \b : TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_SIF_IsPALType(pal_type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetPALType()
/// @brief \b Function \b Description: Set DSP type to pal_type
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SIF_SetPALType(pal_type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetStandard()
/// @brief \b Function \b Description: Set SIF standard
/// @param system_type \b : standard type
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SetStandard(AUDIOSTANDARD_TYPE standard_type)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_PAL))
    {
        if(standard_type == E_AUDIOSTANDARD_L)
        {   // disable SIF AGC for AM
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, FALSE, NULL);
            MDrv_SIF_SendCmd(AU_SIF_CMD_RESET_AGC, NULL, NULL);
        }
        else
        {
            MDrv_SIF_SendCmd(AU_SIF_CMD_ENABLE_AGC, TRUE, NULL);
        }
    }

    switch(standard_type)
    {
    case E_AUDIOSTANDARD_BG:
    case E_AUDIOSTANDARD_BG_A2:
        MDrv_SIF_SetStandardType(AU_SYS_BG_A2);
        break;

    case E_AUDIOSTANDARD_BG_NICAM:
        MDrv_SIF_SetStandardType(AU_SYS_BG_NICAM);
        break;

    case E_AUDIOSTANDARD_I:
        MDrv_SIF_SetStandardType(AU_SYS_I_NICAM);
        break;

    case E_AUDIOSTANDARD_DK:
    case E_AUDIOSTANDARD_DK1_A2:
        MDrv_SIF_SetStandardType(AU_SYS_DK1_A2);
        break;

    case E_AUDIOSTANDARD_DK2_A2:
        MDrv_SIF_SetStandardType(AU_SYS_DK2_A2);
        break;

    case E_AUDIOSTANDARD_DK3_A2:
        MDrv_SIF_SetStandardType(AU_SYS_DK3_A2);
        break;

    case E_AUDIOSTANDARD_DK_NICAM:
        MDrv_SIF_SetStandardType(AU_SYS_DK_NICAM);
        break;

    case E_AUDIOSTANDARD_L:
        MDrv_SIF_SetStandardType(AU_SYS_L_NICAM);
        break;

    case E_AUDIOSTANDARD_M:
    case E_AUDIOSTANDARD_M_A2:
        // MDrv_SIF_ReLoadCode(AU_SIF_PALSUM);
        MDrv_SIF_SetStandardType(AU_SYS_M_A2);
        break;

    case E_AUDIOSTANDARD_M_BTSC:
        MDrv_SIF_SetSystem(MSAPI_AUD_ATV_BTSC);
        MDrv_SIF_SetStandardType(AU_SYS_M_BTSC);
        break;

    case E_AUDIOSTANDARD_M_EIA_J:
        MDrv_SIF_SetSystem(MSAPI_AUD_ATV_EIAJ);
        MDrv_SIF_SetStandardType(AU_SYS_M_EIAJ);
        break;

    default:
        return;
    }

    m_eSifStandard = standard_type;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetSoundMode()
/// @brief \b Function \b Description: Set sound mode for ATV source.
/// @param u8SifSoundMode  \b : sound mode type
///                           - AU_MODE_MONO
///                           - AU_MODE_STEREO
///                           - AU_MODE_SAP
///                           - AU_MODE_LANG_A
///                           - AU_MODE_LANG_B
///                           - AU_MODE_NICAM_MONO, ...etc
/// @return MS_BOOL    \b : success / fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SIF_SetSoundMode(MS_U8 u8SifSoundMode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    switch(u8SifSoundMode)
    {
        case E_AUDIOMODE_MONO:
        case E_AUDIOMODE_FORCED_MONO:
            MDrv_SIF_SetSoundMode(AU_MODE_MONO);
            break;

        case E_AUDIOMODE_G_STEREO:
        case E_AUDIOMODE_K_STEREO:
            MDrv_SIF_SetSoundMode(AU_MODE_STEREO);
            break;

        case E_AUDIOMODE_MONO_SAP:
        case E_AUDIOMODE_STEREO_SAP:
            MDrv_SIF_SetSoundMode(AU_MODE_SAP);
            break;

        case E_AUDIOMODE_DUAL_A:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_A);
            break;

        case E_AUDIOMODE_DUAL_B:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_B);
            break;

        case E_AUDIOMODE_DUAL_AB:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_AB);
            break;

        case E_AUDIOMODE_NICAM_MONO:
            MDrv_SIF_SetSoundMode(AU_MODE_NICAM_MONO);
            break;

        case E_AUDIOMODE_NICAM_STEREO:
            MDrv_SIF_SetSoundMode(AU_MODE_STEREO);
            break;

        case E_AUDIOMODE_NICAM_DUAL_A:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_A);
            break;

        case E_AUDIOMODE_NICAM_DUAL_B:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_B);
            break;

        case E_AUDIOMODE_NICAM_DUAL_AB:
            MDrv_SIF_SetSoundMode(AU_MODE_LANG_AB);
            break;

        case E_AUDIOMODE_LEFT_LEFT:
            MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, MSAPI_AUD_MODE_LL, 0);
            break;

        case E_AUDIOMODE_RIGHT_RIGHT:
            MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, MSAPI_AUD_MODE_RR, 0);
            break;

        case E_AUDIOMODE_LEFT_RIGHT:
            MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_SoundMode, MSAPI_AUD_MODE_STEREO, 0);
            break;
        default:
            break;

    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetSoundMode()
/// @brief \b Function \b Description: Get SIF BTSC sound mode type
/// @return MS_U8      \b : Sound mode type
///                       - MS_U8 = 0x0 : Mute
///                       - bit 0 : Mono
///                       - bit 1 : Stereo
///                       - bit 2 : SAP
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_SIF_GetSoundMode(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    AUDIOSTATUS eCurrentAudioStatus;

    if ((m_eSifStandard ==E_AUDIOSTANDARD_M_A2)||(m_eSifStandard ==E_AUDIOSTANDARD_M_EIA_J)||(m_eSifStandard ==E_AUDIOSTANDARD_M_BTSC))
        _MApi_AUDIO_SIF_GetAudioStatus(&eCurrentAudioStatus);

    eCurrentAudioStatus = E_STATE_AUDIO_NO_CARRIER;

    // coverity 8854
    if (m_eSifStatus & E_STATE_AUDIO_PRIMARY_CARRIER)
        eCurrentAudioStatus |= E_STATE_AUDIO_PRIMARY_CARRIER;

    if (m_eSifStatus & E_STATE_AUDIO_SECONDARY_CARRIER)
        eCurrentAudioStatus |= E_STATE_AUDIO_SECONDARY_CARRIER;

    if (m_eSifStatus & E_STATE_AUDIO_NICAM)
        eCurrentAudioStatus |= E_STATE_AUDIO_NICAM;

    if (m_eSifStatus & E_STATE_AUDIO_STEREO)
        eCurrentAudioStatus |= E_STATE_AUDIO_STEREO;

    if (m_eSifStatus & E_STATE_AUDIO_BILINGUAL)
        eCurrentAudioStatus |= E_STATE_AUDIO_BILINGUAL;

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n MApi_AUDIO_SIF_GetSoundMode standard= %x",m_eSifStandard);
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "\r\n eCurrentAudioStatus= %x",eCurrentAudioStatus);
    switch( m_eSifStandard )
    {
        case E_AUDIOSTANDARD_M_A2:
        case E_AUDIOSTANDARD_M_EIA_J:
            if ((eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER) ||
                (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER)) )
            {
                return E_AUDIOMODE_MONO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_STEREO) )
            {
                return E_AUDIOMODE_K_STEREO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_DUAL_A;
            }

            return E_AUDIOMODE_INVALID;

        case E_AUDIOSTANDARD_M_BTSC:
            if(eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER)
            {
                return E_AUDIOMODE_MONO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_STEREO))
            {
                return E_AUDIOMODE_K_STEREO;//E_AUDIOMODE_K_STEREO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_MONO_SAP;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_STEREO|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_STEREO_SAP;
            }
            return E_AUDIOMODE_INVALID;

        default:
            if ((eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER) ||
                (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER)) )
            {
                return E_AUDIOMODE_MONO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_STEREO) )
            {
                return E_AUDIOMODE_G_STEREO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_SECONDARY_CARRIER|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_DUAL_A;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_NICAM) )
            {
                return E_AUDIOMODE_NICAM_MONO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_NICAM|E_STATE_AUDIO_STEREO) )
            {
                return E_AUDIOMODE_NICAM_STEREO;
            }
            else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_NICAM|E_STATE_AUDIO_BILINGUAL) )
            {
                return E_AUDIOMODE_NICAM_DUAL_A;
            }

            return E_AUDIOMODE_INVALID;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetAudioStatus()
/// @brief \b Function \b Description: Get the current audio status.
///                                    It can be used for getting available sound modes and real-time audio standard monitor.
/// @param eAudioStatus \b : audio status pointer
/// @return MS_BOOL     \b : TRUE/FALSE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SIF_GetAudioStatus(AUDIOSTATUS * eAudioStatus)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U8 ucValue;
    m_eSifStatus = E_STATE_AUDIO_NO_CARRIER;

    if (MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_BTSC))
    {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_BTSC_MODE_STATUS, NULL, NULL);

        if (ucValue & _STATUS_MOD_MONO_EXIST)
            m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;
        if (ucValue & _STATUS_MOD_STEREO_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        if (ucValue & _STATUS_MOD_DUAL_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;

        *eAudioStatus = m_eSifStatus;
        return TRUE;
    }

    if (MDrv_SIF_IsPALType(AU_SIF_PAL_A2)) {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL);

        if( (ucValue & _STATUS_MOD_A2_PRIMARY_EXIST) != _STATUS_MOD_A2_PRIMARY_EXIST )
        {
            *eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            //debugASPPrint("E_STATE_AUDIO_NO_CARRIER\r\n",0);
            return TRUE;
        }

        m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;

        if( ucValue & _STATUS_MOD_A2_SECONDARY_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_SECONDARY_CARRIER;
        else
        {
            *eAudioStatus = m_eSifStatus;
            return TRUE;
        }

        if(ucValue & _STATUS_MOD_A2_PILOT_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_PILOT;
        else
        {
                *eAudioStatus = m_eSifStatus;
                return TRUE;
        }

        if( ucValue & _STATUS_MOD_STEREO_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        else if( ucValue & _STATUS_MOD_DUAL_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;

        *eAudioStatus = m_eSifStatus;

        return TRUE;
    }
    else if (MDrv_SIF_IsPALType(AU_SIF_PAL_NICAM)) {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_NICAM_MODE_STATUS, NULL, NULL);

        if ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_NOT_EXIST )
        {
            *eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            //debugASPPrint("E_STATE_AUDIO_NO_CARRIER1\r\n",0);
            return TRUE;
        }

        m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;

        if ((ucValue & _MASK_NICAM_STATUS_INFO) != _NICAM_SOUND_MODE_LOCK_STATE )
        {
            *eAudioStatus = m_eSifStatus;
            return TRUE;
        }

        m_eSifStatus |= E_STATE_AUDIO_NICAM;

        if (((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_STEREO) ||
            ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_STEREO) )
        {
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        }
        else if (((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_DUAL) ||
                 ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_DUAL) )
        {
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;
        }

        *eAudioStatus = m_eSifStatus;

        return TRUE;
    }
    else {
        ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL);

        if ((ucValue & _STATUS_MOD_A2_PRIMARY_EXIST) != _STATUS_MOD_A2_PRIMARY_EXIST )
        {
            *eAudioStatus = E_STATE_AUDIO_NO_CARRIER;
            //debugASPPrint("E_STATE_AUDIO_NO_CARRIER4\r\n",0);
            return TRUE;
        }

        m_eSifStatus = E_STATE_AUDIO_PRIMARY_CARRIER;

        if (ucValue & _STATUS_MOD_A2_SECONDARY_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_SECONDARY_CARRIER;
        else
        {
            ucValue = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_NICAM_MODE_STATUS, NULL, NULL);

            if ((ucValue & _MASK_NICAM_STATUS_INFO) != _NICAM_SOUND_MODE_LOCK_STATE )
            {
                *eAudioStatus = m_eSifStatus;
                return TRUE;
            }

            m_eSifStatus |= E_STATE_AUDIO_NICAM;

            if (((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_STEREO) ||
                ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_STEREO) )
            {
                m_eSifStatus |= E_STATE_AUDIO_STEREO;
            }
            else if (((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_NICAM_DUAL) ||
                     ((ucValue & _MASK_NICAM_SOUND_MODE_INFO) == _NICAM_SOUND_MOD_MONO_n_NICAM_DUAL) )
            {
                m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;
            }

            *eAudioStatus = m_eSifStatus;

            return TRUE;
        }

        //Add pilot, DK2, DK3 status, C.P.Chen 2008/01/03
        if (ucValue & _STATUS_MOD_A2_PILOT_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_PILOT;
        else
        {
            *eAudioStatus = m_eSifStatus;
            return TRUE;
        }

        if (ucValue & _STATUS_MOD_A2_DK2_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_DK2;
        else if( ucValue & _STATUS_MOD_A2_DK3_EXIST)
            m_eSifStatus |= E_STATE_AUDIO_DK3;

        if (ucValue & _STATUS_MOD_STEREO_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_STEREO;
        else if( ucValue & _STATUS_MOD_DUAL_EXIST )
            m_eSifStatus |= E_STATE_AUDIO_BILINGUAL;

        *eAudioStatus = m_eSifStatus;

        return TRUE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_StartAutoStandardDetection()
/// @brief \b Function \b Description: To start automatic detection about input audio standard
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_StartAutoStandardDetection(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (m_eSifStandard != E_AUDIOSTANDARD_L)
    {
        MDrv_SIF_StartAutoDetection ();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_GetResultOfAutoStandardDetection()
/// @brief \b Function \b Description: To get result of audio standard after automatic detection called by MDrv_ASP_StartAutoStandardDetection.
/// @return AUDIOSTANDARD_TYPE \b :
////////////////////////////////////////////////////////////////////////////////
AUDIOSTANDARD_TYPE _MApi_AUDIO_SIF_GetResultOfAutoStandardDetection(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U8 cResult, i=0;
    AUDIOSTANDARD_TYPE eStandard;

    cResult = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_STATUS, NULL, NULL);

    while(cResult==0x80)
    {
        i++;
        if(i>=40)
            break;
        MsOS_DelayTask(10);
        cResult = MDrv_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_STATUS, NULL, NULL);
    }

    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "AutoStandardDetection Check Times = %x\r\n",i);

    if (i>=40)
    {
        //debugASPPrint("Detected Audio NO Standard ",0);
        return E_AUDIOSTANDARD_NOTSTANDARD;
    }
    //debugASPPrint("Detected Audio System:0x%x",cResult);

    switch (cResult)
    {
    case _STD_SEL_M_BTSC:
        eStandard = E_AUDIOSTANDARD_M_BTSC;
        break;

    case _STD_SEL_M_EIAJ:
        eStandard = E_AUDIOSTANDARD_M_EIA_J;
        break;

    case _STD_SEL_M_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_M_A2;
        else
            eStandard = E_AUDIOSTANDARD_M;
        break;

    case _STD_SEL_BG_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_BG_A2;
        else
            eStandard = E_AUDIOSTANDARD_BG;
        break;

    case _STD_SEL_DK1_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_DK1_A2;
        else
            eStandard = E_AUDIOSTANDARD_DK;
        break;

    case _STD_SEL_DK2_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_DK2_A2;
        else
            eStandard = E_AUDIOSTANDARD_DK;
        break;

    case _STD_SEL_DK3_A2:
        if( MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_A2_MODE_STATUS, NULL, NULL) & _STATUS_MOD_A2_SECONDARY_EXIST )
            eStandard = E_AUDIOSTANDARD_DK3_A2;
        else
            eStandard = E_AUDIOSTANDARD_DK;
        break;

    case _STD_SEL_BG_NICAM:
        eStandard = E_AUDIOSTANDARD_BG_NICAM;
        break;

    case _STD_SEL_DK_NICAM:
        eStandard = E_AUDIOSTANDARD_DK_NICAM;
        break;

    case _STD_SEL_I_NICAM:
        eStandard = E_AUDIOSTANDARD_I;
        break;

    case _STD_SEL_L_NICAM:
        eStandard = E_AUDIOSTANDARD_L;
        break;

    default:
        eStandard = E_AUDIOSTANDARD_NOTSTANDARD;
        break;
    }

    return eStandard;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_ConvertToBasicAudioStandard()
/// @brief \b Function \b Description: Convert the audio standard to the basic standard
/// @param eStandard   \b : Please refer AUDIOSTANDARD_TYPE enum in header file
/// @return AUDIOSTANDARD_TYPE \b : basic standard
///                               - E_AUDIOSTANDARD_BG
///                               - E_AUDIOSTANDARD_I
///                               - E_AUDIOSTANDARD_DK
///                               - E_AUDIOSTANDARD_L
///                               - E_AUDIOSTANDARD_M
///                               - E_AUDIOSTANDARD_NOTSTANDARD
////////////////////////////////////////////////////////////////////////////////
AUDIOSTANDARD_TYPE _MApi_AUDIO_SIF_ConvertToBasicAudioStandard(AUDIOSTANDARD_TYPE eStandard)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    switch(eStandard)
    {
    case E_AUDIOSTANDARD_BG:
    case E_AUDIOSTANDARD_BG_A2:
    case E_AUDIOSTANDARD_BG_NICAM:
        return E_AUDIOSTANDARD_BG;

    case E_AUDIOSTANDARD_I:
        return E_AUDIOSTANDARD_I;

    case E_AUDIOSTANDARD_DK:
    case E_AUDIOSTANDARD_DK1_A2:
    case E_AUDIOSTANDARD_DK2_A2:
    case E_AUDIOSTANDARD_DK3_A2:
    case E_AUDIOSTANDARD_DK_NICAM:
        return E_AUDIOSTANDARD_DK;

    case E_AUDIOSTANDARD_L:
        return E_AUDIOSTANDARD_L;

    case E_AUDIOSTANDARD_M:
    case E_AUDIOSTANDARD_M_A2:
    case E_AUDIOSTANDARD_M_BTSC:
    case E_AUDIOSTANDARD_M_EIA_J:
        return E_AUDIOSTANDARD_M;
    default:
        break;
    }

    return E_AUDIOSTANDARD_NOTSTANDARD;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SIF_SetThreshold()
/// @brief \b Function \b Description:  This routine is used to set SIF threshold
/// @param ThrTbl  \b : SIF thresholds Table
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE *ThrTbl)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_BTSC))
    {
       MDrv_SIF_SetThreshold(ThrTbl, BTSC_TBL_INDEX, BTSC_TBL_LENGTH, BTSC_STANDARD);
    }
    else if (MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_FM_RADIO))
    {
       MDrv_SIF_SetThreshold(ThrTbl, FM_RADIO_TBL_INDEX, FM_RADIO_TBL_LENGTH, FM_RADIO_STANDARD);
    }
    else if (MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_PAL))
    {
        MDrv_SIF_SetThreshold(ThrTbl, A2_M_TBL_INDEX, A2_M_TBL_LENGTH, A2_M_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, A2_BG_TBL_INDEX, A2_BG_TBL_LENGTH, A2_BG_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, A2_DK_TBL_INDEX, A2_DK_TBL_LENGTH, A2_DK_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, FM_I_TBL_INDEX, FM_I_TBL_LENGTH, A2_I_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, AM_TBL_INDEX, AM_TBL_LENGTH, AM_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, NICAM_BG_TBL_INDEX, NICAM_BG_TBL_LENGTH, NICAM_BG_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, NICAM_I_TBL_INDEX, NICAM_I_TBL_LENGTH, NICAM_I_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_M_TBL_INDEX, HIDEV_M_TBL_LENGTH, HIDEV_M_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_BG_TBL_INDEX, HIDEV_BG_TBL_LENGTH, HIDEV_BG_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_DK_TBL_INDEX, HIDEV_DK_TBL_LENGTH, HIDEV_DK_STANDARD);
        MDrv_SIF_SetThreshold(ThrTbl, HIDEV_I_TBL_INDEX, HIDEV_I_TBL_LENGTH, HIDEV_I_STANDARD);

        if (((ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH].HiByteValue == 0x5A) && (ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH].LowByteValue == 0x5A))  &&
            ((ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH+1].HiByteValue == 0xA5) && (ThrTbl[HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH+1].LowByteValue == 0xA5)))
        {
            MS_U16 value;
            MS_U16 i = HIDEV_I_TBL_INDEX+HIDEV_I_TBL_LENGTH+2;
            MS_U16 j = 0;

            // A2_M Extension threshold
            while (ThrTbl[i].HiByteValue != 0xFF)
            {
                value = ( (0x00FF & ThrTbl[i].HiByteValue) <<8)|(0x00FF & ThrTbl[i].LowByteValue);
                MDrv_SIF_AccessThreshold(A2_M_STANDARD|WRITE_THRESHOLD, A2_M_TBL_LENGTH+j, value);
                i++;
                j++;
            }
            i++;
            j = 0;
            // A2_BG Extension threshold
            while (ThrTbl[i].HiByteValue != 0xFF)
            {
                value = ( (0x00FF & ThrTbl[i].HiByteValue) <<8)|(0x00FF & ThrTbl[i].LowByteValue);
                MDrv_SIF_AccessThreshold(A2_BG_STANDARD|WRITE_THRESHOLD, A2_BG_TBL_LENGTH+j, value);
                i++;
                j++;
            }
            i++;
            j = 0;
            // A2_DK Extension threshold
            while (ThrTbl[i].HiByteValue != 0xFF)
            {
                value = ( (0x00FF & ThrTbl[i].HiByteValue) <<8)|(0x00FF & ThrTbl[i].LowByteValue);
                MDrv_SIF_AccessThreshold(A2_DK_STANDARD|WRITE_THRESHOLD, A2_DK_TBL_LENGTH+j, value);
                i++;
                j++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
//        AUDIO_SPDIF RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_HWEN()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param spdif_en    \b :    0--Disable S/PDIF out;
///                            1--Enable S/PDIF out
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_HWEN(MS_BOOL spdif_en)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SPDIF_HWEN(spdif_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable S/PDIF output (Hardware)
/// @param mute_en     \b :    0--Disable mute;
///                            1--Enable mute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_SetMute(MS_BOOL mute_en)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SPDIF_SetMute(mute_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetMode()
/// @brief \b Function \b Description:  This routine is used to set S/PDIF mode.
/// @param spdif_mode  \b : PCM or Non-PCM mode
///                - spdif_mode = 0x0 : SPDIF enable, PCM mode
///                - spdif_mode = 0x1 : SPDIF off
///                - spdif_mode = 0x2 : SPDIF enable, nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_SetMode(MS_U8 spdif_mode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U8 u8Source = 0;

    APIAUDIO_CHECK_SHM_INIT;

    u8Source = (MS_U8)g_AudioVars2->eAudioSource;
    gSpdifOutputType = spdif_mode;

    MDrv_AUDIO_SPDIF_SetMode(spdif_mode, u8Source);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_GetMode()
/// @brief \b Function \b Description:  This routine is used to get S/PDIF mode.
/// @return MS_U8      \b :  register SPDIF_CTRL value
///                       - 0x0 : SPDIF enable, PCM mode
///                       - 0x1 : SPDIF OFF
///                       - 0x2 : SPDIF enable, non-PCM mode
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_SPDIF_GetMode(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_SPDIF_GetMode());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_SetSCMS()
/// @brief \b Function \b Description:  This routine is used to set SPDIF SCMS.
/// @param C_bit_en  \b : copy right control bit
/// @param L_bit_en  \b : generation bit
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SPDIF_SetSCMS(MS_U8 C_bit_en, MS_U8 L_bit_en)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SPDIF_SetSCMS(C_bit_en, L_bit_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_GetSCMS()
/// @brief \b Function \b Description:  This routine is used to get SPDIF SCMS.
/// @return MS_U8      \b :  SCMS[0] = C bit status, SCMS[1] = L bit status
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_SPDIF_GetSCMS(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_SPDIF_GetSCMS());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SPDIF_ChannelStatus_CTRL()
/// @brief \b Function \b Description:
/// @param pau_info    \b :  AUDIO INIT STRUCTURE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SPDIF_ChannelStatus_CTRL(AUDIO_SPDIF_CS_TYPE cs_mode, AUDIO_SPDIF_CS_TYPE_STATUS status)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_SPDIF_ChannelStatus_CTRL(cs_mode, status));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_Audio_SPDIF_Monitor()
/// @brief \b Function \b Description:
/// @param pau_info    \b :  AUDIO INIT STRUCTURE
////////////////////////////////////////////////////////////////////////////////
void _MApi_Audio_SPDIF_Monitor(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return;
    }

    if (g_bDSPLoadCode== TRUE)
    {
        return;
    }

    MDrv_AUD_Monitor_SPDIF_NONPCM_SmpRate();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_Digital_Out_SetChannelStatus()
/// @brief \b Function \b Description: This routine is used to set SPDIF/HDMI/ARC channel status.
/// @param <IN>        \b   eType   :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_DigitalOut_SetChannelStatus(DIGITAL_OUTPUT_TYPE ePath, Digital_Out_Channel_Status_t *stChannelStatus)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(Mdrv_AUDIO_DigitalOut_SetChannelStatus(ePath, stChannelStatus));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DigitalOut_SetDeviceCapability()
/// @brief \b Function \b Description: This routine is used to set output device capability.
/// @param <IN>        \b eoutType: output type (ARC_OUTPUT or HDMI_OUTPUT)
/// @param <IN>        \b ecodecType: kind of codec
/// @param <IN>        \b *p_codecCapability: codec support capability structure
/// @param <RET>       \b bRet: return api setting success or fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_DigitalOut_SetDeviceCapability(DIGITAL_OUTPUT_TYPE eoutType, AUDIO_FORMAT_CODE ecodecType, Digital_Out_Device_Capability_t *p_codecCapability)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;
    MS_BOOL bRet = FALSE;

    if(ecodecType>CODEC_MAX-1)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Invalid ecodecType parameter\n", __FUNCTION__);
        return bRet;
    }
    switch(eoutType)
        {
        case DIGITAL_HDMI_ARC_OUTPUT:
            memcpy(&(g_AudioVars2->stDigitalOutCodecCapability[0][ecodecType]), p_codecCapability, sizeof(Digital_Out_Device_Capability_t));
            bRet = TRUE;
            break;

        case DIGITAL_HDMI_OUTPUT:
            memcpy(&(g_AudioVars2->stDigitalOutCodecCapability[1][ecodecType]), p_codecCapability, sizeof(Digital_Out_Device_Capability_t));
            bRet = TRUE;
            break;

        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Invalid eoutType parameter\n", __FUNCTION__);
            break;
        }

    return bRet;
}

////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_HDMI RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Tx_SetMute()
/// @brief \b Function \b Description:  This routine is used to enable HDMI Tx output (Software)
/// @param mute_en     \b :    0--Disable mute;
///                            1--Enable mute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_Tx_SetMute(MS_BOOL mute_en)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_HDMI_Tx_SetMute(mute_en);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Tx_GetStatus()
/// @brief \b Function \b Description:  This routine is used to get HDMI Tx status
/// @param onOff     \b : HDMI tx function is availible in this chip ?
/// @param hdmi_SmpFreq \b : Audio current sample freq for tx
/// @param outType \b   : PCM mode or nonPCM mode
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_Tx_GetStatus(MS_BOOL *onOff, AUDIO_FS_TYPE *hdmi_SmpFreq, HDMI_TX_OUTPUT_TYPE *outType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if ((onOff == NULL) || (hdmi_SmpFreq == NULL) || (outType == NULL))
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s() : Invalid parameter\n", __FUNCTION__);
        return;
    }

    MDrv_AUDIO_HDMI_Tx_GetStatus( onOff, hdmi_SmpFreq, outType );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_Monitor()
/// @brief \b Function \b Description: This function report HDMI non-PCM or PCM format (register STATUS_INPUT)
/// @return MS_U8      \b :
///                     - non-PCM --> register STATUS_INPUT[7:6]= 0x1;
///                     - PCM   --> register STATUS_INPUT[7:6]= 0x0
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_HDMI_Monitor(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    return((MS_U8)MDrv_AUDIO_HDMI_NonpcmMonitor());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetOutputType()
/// @brief \b Function \b Description:  To set HDMI output type independently
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_TX_SetMode(HDMI_TX_OUTPUT_TYPE outType)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    gHdmiOutputType = (outType & HDMI_OUT_NONPCM) ? HDMI_OUT_NONPCM : HDMI_OUT_PCM;
    MDrv_AUDIO_HDMI_TX_SetMode(outType, g_AudioVars2->eAudioSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_GetNonpcmFlag()
/// @brief \b Function \b Description: This function report HDMI non-PCM or PCM format (register STATUS_INPUT)
/// @return MS_BOOL    \b :
///                     - TRUE : non-PCM --> register STATUS_INPUT[7:6]= 0x1 ;
///                     - FALSE : PCM     --> register STATUS_INPUT[7:6]= 0x0
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_HDMI_GetNonpcmFlag(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_HDMI_NonpcmMonitor());
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_SetNonpcm(MS_U8 nonPCM_en)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_HDMI_SetNonpcm(nonPCM_en);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
/// @return AUDIO_HDMI_RX_TYPE \b : Return HDMI RX Type
////////////////////////////////////////////////////////////////////////////////
AUDIO_HDMI_RX_TYPE _MApi_AUDIO_HDMI_RX_SetNonpcm(MS_U8 nonPCM_en)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_HDMI_SetNonpcm(nonPCM_en));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_RX_SetNonpcm()
/// @brief \b Function \b Description: This function setting HDMI non-PCM or PCM relational register
/// @param nonPCM_en   \b :    1: nonPCM; 0: PCM
/// @return AUDIO_HDMI_RX_TYPE \b : Return HDMI RX Type
////////////////////////////////////////////////////////////////////////////////
AUDIO_HDMI_RX_TYPE _MApi_AUDIO_HDMI_RX_GetNonPCM(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U8 HDMI_PC_Status;
    HDMI_PC_Status = MDrv_AUDIO_HDMI_DolbyMonitor();

    if(FALSE == MDrv_AUDIO_HDMI_NonpcmMonitor())
    {
        return(HDMI_RX_PCM);
    }

    if (HDMI_PC_Status == 0x1)
    {
        return(HDMI_RX_DD);
    }
    else if (HDMI_PC_Status == 0xB)
    {
        return(HDMI_RX_DTS);
    }
    else if (HDMI_PC_Status == 0x15)
    {
        return(HDMI_RX_DDP);
    }
    else if (HDMI_PC_Status == 0x0)
    {
        return(HDMI_RX_PCM);
    }
    else
    {
        return(HDMI_RX_Other);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DTV_HDMI_CFG()
/// @brief \b Function \b Description:  Set DVB/HDMI AC3 path configure
/// @param ctrl    \b : 0: DVB2_AD path;
///                     1: HDMI path
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DTV_HDMI_CFG(MS_U8 ctrl)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_HDMI_AC3_PathCFG(ctrl);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_GetSynthFreq()
/// @brief \b Function \b Description:  Audio HDMI CTS-N synthesizer input signal detect.
/// @return MS_U16     \b : return freq. If no signal, return 0
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_HDMI_GetSynthFreq(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_HDMI_GetSynthFreq();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_HDMI_SetDownSample()
/// @brief \b Function \b Description:  set HDMI downsample rate
/// @param ratio       \b : donwsample ratio
///                       - 00: Normal (from 1x to 1x).
///                       - 01: Down sample from 2x to 1x.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_HDMI_SetDownSample(MS_U8 ratio)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_HDMI_SetDownSample(ratio);
}





////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_SOUND RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SND_ProcessEnable()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_SND_ProcessEnable(Sound_ENABLE_Type Type, MS_BOOL enable)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_SND_ProcessEnable(Type, enable));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SND_SetParam1()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_SND_SetParam1( Sound_SET_PARAM_Type Type, MS_U16 param1, MS_U16 param2)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    //if(Type == Sound_SET_PARAM_DCOffet)
    //{
    //    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO,  "-->>NEW POWER ON SEQ: Sound_SET_PARAM_DCOffet\n\r");
    //}
    return(MDrv_SND_SetParam(Type,  param1,  param2));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_SND_GetParam1( Sound_GET_PARAM_Type Type, MS_U16 param1)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_SND_GetParam(Type, param1);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SOUND_SetParam()
/// @brief \b Function \b Description: This routine is used for adjust Common Sound parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SOUND_SetParam( Sound_SET_Type Type, MS_U32 *p_param )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] [Type 0x%x] [0]: 0x%x [1]: 0x%x \n", __FUNCTION__, (int)Type, (int)*p_param, (int)*(p_param+1));

    return MDrv_SOUND_SetParam(Type, p_param);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SeInit()
/// @brief \b Function \b Description: This routine is the initialization for Audio sound effect module.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SeInit(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_Init();
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AbsoluteVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of audio u8Path.
/// @param u8Path      \b : for audio u8Path0 ~ u8Path6
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAbsoluteVolume( MS_U8 u8Path, MS_U8 u8Vol1, MS_U8 u8Vol2  )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_AbsoluteVolume(u8Path, u8Vol1, u8Vol2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPreScale()
/// @brief \b Function \b Description: This routine is used to set the prescale of audio u8Path.
/// @param u8Path      \b : for audio u8Path0 ~ u8Path5
/// @param u8PreScale  \b : range from 0x01 to 0xFF , gain: -13.75db to +18db (0.125 db per step)
///                       - 0x00: disable pre-scale
///                       - 0x6F: gain = 0db
///                       - 0xFF: gain = +18db
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPreScale( MS_U8 u8Path, MS_U8 u8PreScale )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
	AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "apiAud>> pre-scale: Path = %d, PreScale = %d\n", u8Path, u8PreScale);
    MDrv_SOUND_SetPreScale(u8Path, u8PreScale);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SourceConnect()
/// @brief \b Function \b Description: This routine is used to set audio mute by processor
/// @param u8Path      \b : eProcessor: Set Main/Sub processor,
/// @param EnMute      \b : TRUE --Mute;
///                                    FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SourceConnect(AUDIO_OUTPORT_SOURCE_TYPE eConnectSourceType, MS_BOOL bConnect )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL EnMute = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    if (bConnect == FALSE)
    {
        EnMute = TRUE;
    }
    else
    {
        EnMute = FALSE;
    }

    //Set mute for Speaker out
    if(g_AudioVars2->AudioOutputSourceInfo.SpeakerOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.SpeakerOut, EnMute);
    }

    //Set mute for HP out
    if(g_AudioVars2->AudioOutputSourceInfo.HpOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.HpOut, EnMute);
    }

    //Set mute for Monitor out
    if(g_AudioVars2->AudioOutputSourceInfo.MonitorOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.MonitorOut, EnMute);
    }

    //Set mute for Scart out
    if(g_AudioVars2->AudioOutputSourceInfo.ScartOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.ScartOut, EnMute);
    }

    //Set mute for SPDIF out
    if(g_AudioVars2->AudioOutputSourceInfo.SpdifOut == eConnectSourceType)
    {
        MDrv_SOUND_SetMute( g_AudioVars2->AudioPathInfo.SpdifOut, EnMute);
    }

    //Set mute for ARC out
    if(g_AudioVars2->AudioOutputSourceInfo.ArcOut== eConnectSourceType)
    {
        //MDrv_AUDIO_HDMI_Tx_SetMute( EnMute);
    }

    //Set mute for HDMI out
    if(g_AudioVars2->AudioOutputSourceInfo.HDMIOut== eConnectSourceType)
    {
        MDrv_AUDIO_HDMI_Tx_SetMute( EnMute);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMute()
/// @brief \b Function \b Description: This routine is used to set audio path S/W mute.
/// @param u8Path      \b : for audio path0 ~ path6
/// @param EnMute      \b : TRUE --Mute;
///                         FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetMute(MS_U8 u8Path, MS_BOOL EnMute )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetMute( u8Path, EnMute);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBalance()
/// @brief \b Function \b Description: This routine is used to set  balance value
/// @param u8Balance   \b : balance (0~100)
///                       - 0    :  R channel is muted ...
///                       - 50   :  L & R channel output the same level...
///                       -  100 :  L channel is muted .
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBalance( MS_U8 u8Balance )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetBalance(u8Balance);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableEQ()
/// @brief \b Function \b Description: This routine is used to enable EQ function.
/// @param EnEQ        \b : TRUE --Enable EQ;
///                                   FALSE--Disable EQ
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableEQ(MS_BOOL EnEQ)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_EnableEQ(EnEQ);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableTone()
/// @brief \b Function \b Description: This routine is used to enable Bass/Treble function.
/// @param EnTone      \b : TRUE --Enable Bass/Treble;
///                         FALSE--Disable Bass/Treble
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableTone(MS_BOOL EnTone)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_EnableTone(EnTone);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableAutoVolume()
/// @brief \b Function \b Description: This routine is used to enable auto volume(AVC) function.
/// @param EnAVC       \b : TRUE --Enable AVC;
///                        FALSE--Disable AVC
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableAutoVolume(MS_BOOL EnAVC)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

   MDrv_SOUND_EnableAutoVolume(EnAVC);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableBalance()
/// @brief \b Function \b Description: This routine is used to enable Balance function.
/// @param EnBalance   \b : Balance Enable bits
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableBalance(MS_BOOL EnBalance)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_EnableBalance(EnBalance);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableSurround()
/// @brief \b Function \b Description: This routine is used to enable surround function.
/// @param EnSurround   \b : TRUE --Enable Surround;
///                          FALSE--Disable Surround
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnableSurround(MS_BOOL EnSurround)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_EnableSurround(EnSurround);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDynamicBass()
/// @brief \b Function \b Description: This routine is used to set DynamicBass level.
/// @param u8Bass      \b :
///
///
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetDynamicBass( MS_U8 u8DynamicBass )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

      MDrv_SOUND_SetDynamicBass(u8DynamicBass);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBass()
/// @brief \b Function \b Description: This routine is used to set Bass level.
/// @param u8Bass      \b :Bass Level (0~100)
///                       - CHIP_FAMILY_U3 & S4LE==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBass( MS_U8 u8Bass )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetBass(u8Bass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_AbsoluteBass()
/// @brief \b Function \b Description: This routine is used to set abs. Bass value.
/// @param u8Bass      \b : register value
///                       - CHIP_FAMILY_U3 & S4LE ==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_AbsoluteBass(MS_U8 u8Bass)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_AbsoluteBass(u8Bass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetTreble()
/// @brief \b Function \b Description: This routine is used to set Treble level.
/// @param u8Treble    \b :Treble Level (0~100)
///                       - CHIP_FAMILY_U3 & S4LE ==>mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7 ==>mapping to -12 ~ +12 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetTreble ( MS_U8 u8Treble )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetTreble(u8Treble);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_AbsoluteTreble()
/// @brief \b Function \b Description: This routine is used to set abs. Treble value.
/// @param u8Treble    \b : register value
///                       - CHIP_FAMILY_U3 & S4LE ==> mapping to -16 ~ +15 dB Gain
///                       - CHIP_FAMILY_S7        ==> mapping to -12 ~ +12 dB Gain
///                       - CHIP_FAMILY_S7J       ==> mapping to -16 ~ +15 dB Gain
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_AbsoluteTreble( MS_U8 u8Treble )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_AbsoluteTreble(u8Treble);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetEq()
/// @brief \b Function \b Description: This routine is used to set the the 5-band EQ level  .
/// @param u8band      \b :  EQ band 0~4
/// @param u8level     \b :  Absolute EQ register value  ;
///                       - 0011-0000: +12.00db. (Max)
///                       - 0000-0001: +0.25db.
///                       - 0000-0000: +0.00db.
///                       - 1111-1111: -0.25db.
///                       - 1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEq(MS_U8 u8band, MS_U8 u8level)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetEq(u8band,  u8level);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetEq_7band()
/// @brief \b Function \b Description: This routine is used to set the the 7-band EQ level  .
/// @param u8band      \b :  EQ band 0~6
/// @param u8level     \b :  Absolute EQ register value  ;
///                          0011-0000: +12.00db. (Max)
///                          0000-0001: +0.25db.
///                          0000-0000: +0.00db.
///                          1111-1111: -0.25db.
///                          1101-0000: -12.00db. (Min)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetEq_7band(MS_U8 u8band, MS_U8 u8level)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetEq7(u8band,  u8level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcThreshold()
/// @brief \b Function \b Description: This routine is used to set the AVC  threshold level.
/// @param Level       \b :  AVC threshold level
///                       - 0x00--  0    dBFS
///                       - 0x01--  -0.5    dBFS
///                       - 0x20--  -16    dBFS
///                       - 0x50--  -40    dBFS
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcThreshold(MS_U8 Level)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetAVCThreshold(Level);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcMode()
/// @brief \b Function \b Description: This routine is used to set the AVC mode.
/// @param <IN>        \b u8AvcMode    :  AVC u8Mode
///                                    - 0: L mode
///                                    - 1: S mode
///                                    - 2: M mode
/// @image html L_mode.JPG "L mode"
/// @image html S_mode.JPG "S mode"
/// @image html M_mode.JPG "M mode"
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcMode( MS_U8 AvcMode )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetAvcMode( AvcMode );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcAT()
/// @brief \b Function \b Description: This routine is used to set the AVC  attack time .
/// @param AvcAT       \b :  AVC attack time
///                       - AvcAT = 0 --> 20  ms
///                       - AvcAT = 1 --> 100 ms
///                       - AvcAT = 2 --> 200 ms
///                       - AvcAT = 3 --> 1 sec
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcAT( MS_U8 AvcAT )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetAvcAT( AvcAT );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAvcRT()
/// @brief \b Function \b Description: This routine is used to set the AVC  release time .
/// @param AvcRT       \b :  AVC release time
///                       - AvcRT = 0 --> 1 sec
///                       - AvcRT = 1 --> 2 sec
///                       - AvcRT = 2 --> 100 ms
///                       - AvcRT = 3 --> 200 ms
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAvcRT( MS_U8 AvcRT )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetAvcRT( AvcRT );
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBufferProcess()
/// @brief \b Function \b Description: sets the value of audio delay time (ms)..
/// @param DelayTime   \b : 0x00~0xFA(0~250), each step=1 ms , 0~250 ms
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBufferProcess(MS_U8 DelayTime)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetCH1AudioDelay(DelayTime);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXA()
/// @brief \b Function \b Description: This routine is used to set the Surround xA echo Gain.
/// @param <IN>        \b mode    :     A Gain : register SURROUND[3:2]
///                       - 0 :    x0.1
///                       - 1 :    x0.15
///                       - 2 :    x0.2
///                       - 3 :    x0.25
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundXA( MS_U8 mode )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetSurroundXA(mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXB()
/// @brief \b Function \b Description: This routine is used to set the Surround xB echo Gain.
/// @param <IN>        \b mode    :   B Gain : register SURROUND[5:4]
///                       - 0 :    x0.25
///                       - 1 :    x0.3
///                       - 2 :    x0.35
///                       - 3 :    x0.45
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundXB( MS_U8 mode )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetSurroundXB(mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundXK()
/// @brief \b Function \b Description: This routine is used to set the Surround xK feedback Gain.
/// @param <IN>        \b mode    :     K Gain : register SURROUND[10:8]
///                       - 0 :    x0.1
///                       - 1 :    x0.2
///                       - 2 :    x0.3
///                       - 3 :    x0.4
///                       - 4 :    x0.5
///                       - 5 :    x0.6
///                       - 6 :    x0.7
///                       - 7 :    x0.8
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundXK( MS_U8 mode )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetSurroundXK(mode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSurroundLPFGain()
/// @brief \b Function \b Description: This routine is used to set the Surround LPF Gain.
/// @image html surround.JPG "Surround block diagram"
/// @param <IN>        \b mode    : LPF Gain : register SURROUND[7:6]
///                       - 0 :    0 dB
///                       - 1 :    2 dB
///                       - 2 :    4 dB
///                       - 3 :    un-support
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSurroundLPFGain( MS_U8 mode )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetSurroundLPFGain(mode);
}

/////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ConvertVolumeUnit()
/// @brief \b Function \b Description: Convert UI Volume to Absolute Volume.
///                                                  valid value of UiVolume is between 0 and 100
/// @param UiVolume    \b :    UI Volume 0 -> 62, 50 -> 12, 100 -> 7
/// @param MS_U8       \b :    register value
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_ConvertVolumeUnit(MS_U8 UiVolume)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U8 tmpVolume = 0;

    if (UiVolume > 100)
    {
        tmpVolume = 7;
    }
    else if (UiVolume >= 50)
    {
        tmpVolume = (12 - ((UiVolume-50)/10));
    }
    else
    {
        tmpVolume = (12 + (50 - UiVolume));
    }

    return tmpVolume;
}

////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_ADVANCE_SOUND RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_ProcessEnable
/// @brief \b Function \b Description: This routine is used to enable process of advanced sound effect.
/// @param <IN>        \b ADVSND_TYPE type
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSOUND_ProcessEnable(ADVSND_TYPE type)
{
	AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_ADVSOUND_ProcessEnable(type);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, BOOL enable)
/// @brief \b Function \b Description: This routine is used to enable subprocess of advanced sound effect.
/// @param <IN>        \b proc, enable
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSOUND_SubProcessEnable(ADVFUNC proc, MS_BOOL enable)
{
	AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_ADVSOUND_SubProcessEnable(proc, enable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, U16 value1, U16 value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, value1, value2
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U16 u16value1, MS_U16 u16value2)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_ADVSOUND_SetParam(param, (MS_U32)u16value1, u16value2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_SetParam(ADVSND_PARAM param, U32 value1, U16 value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, u32value1, u16value2
///                              \b Parameter u32value1: range 0~0x7FFFFF
///                              \b Parameter u16value2: 0/1,
///                              - 0 : if u32value1 is u16, none fix point 24 bit parameter
///                              - 1 : if u32value1 is u32, fix point 24 bit parameter
/// @param <OUT>       \b NONE  :
/// @return <OUT>       \b FALSE : Setting Fail
///                               \b TRUE  : Setting OK
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ADVSND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_ADVSOUND_SetParam(param, u32value1, u16value2);
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_DBXTV_SetMode(DBXTV_TotSonMode TotSonMode, DBXTV_TotVolMode TotVolMode, DBXTV_TotSurMode TotSurMode)
/// @brief \b Function \b Description: This routine is used to set dbx TV mode
/// @param <IN>        \b param, TotSonMode, TotVolMode, TotSurMode
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_DBXTV_SetMode(EN_DBXTV_TotSonMode totsonmode, EN_DBXTV_TotVolMode totvolmode, EN_DBXTV_TotSurMode totsurmode,MS_U32 enable)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

     MDrv_DBXTV_SetMode(totsonmode, totvolmode, totsurmode,enable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ADVSOUND_GetInfo()
/// @brief \b Function \b Description: This routine is used for Get ADV Sound info
/// @return MS_BOOL     \b : True / False
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_ADVSOUND_GetInfo( AdvSound_GET_Type Type)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return (MDrv_ADVSOUND_GetInfo(Type));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_AseSetBinAddress()
/// @brief \b Function \b Description:  This function is used to assign Bin address of advanced sound effect
/// @param u8Index     \b : type index
/// @param BinAddr     \b : address of Bin file
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_AseSetBinAddress(u8Index, BinAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetAdvSndSys()
/// @brief \b Function \b Description: Check Advance sound mod and reload code
/// @param u8AdvSurrMod \b : Audio Advance Sound system mod
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetAdvSndSys(En_DVB_advsndType u8AdvSurrMod)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

     // Marked by coverity_0446
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, "apiAud chk u8AdvSurrMod/u8CurrAdvSndSys = %s / %s\n", u8AdvSurrMod, u8CurrAdvSndSys);

    if (!En_DVB_advsndType_Compare(u8AdvSurrMod, u8CurrAdvSndSys))
    {
        // set advance surround sound system
        // 0: PL2,  1:BBE,  2: SRS  3:VDS, 4:VSPK, 5:SUPV, 6:TSHD, 7:XEN
        if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_BBE))
        {
            MDrv_ADVSOUND_ProcessEnable(BBE);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_BBE;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_SRS))
        {
            MDrv_ADVSOUND_ProcessEnable(SRS_TSXT);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_SRS;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_VDS))
        {
            MDrv_ADVSOUND_ProcessEnable(DOLBY_PL2VDS);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_VDS;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_VSPK))
        {
            MDrv_ADVSOUND_ProcessEnable(DOLBY_PL2VDPK);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_VSPK;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_SUPV))
        {
            MDrv_ADVSOUND_ProcessEnable(SUPER_VOICE);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_SUPV;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_TSHD))
        {
            MDrv_ADVSOUND_ProcessEnable(SRS_TSHD);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_TSHD;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_DTS_ULTRATV))
        {
            MDrv_ADVSOUND_ProcessEnable(DTS_ULTRATV);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_DTS_ULTRATV;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_AUDYSSEY))
        {
            MDrv_ADVSOUND_ProcessEnable(DTS_ULTRATV);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_AUDYSSEY;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_DBX))
        {
            MDrv_ADVSOUND_ProcessEnable(DBX);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_DBX;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_SSHD_TSHD))
        {
            MDrv_ADVSOUND_ProcessEnable(SRS_THEATERSOUND);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_SSHD_TSHD;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_PURESND))
        {
            MDrv_ADVSOUND_ProcessEnable(SRS_PURESND);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_PURESND;
        }
        else if (En_DVB_advsndType_Compare(u8AdvSurrMod, MSAPI_AUD_ADVSND_BONGIOVI_DPS))
        {
            MDrv_ADVSOUND_ProcessEnable(BONGIOVI_DPS);
            u8CurrAdvSndSys = MSAPI_AUD_ADVSND_BONGIOVI_DPS;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VDS()
/// @brief \b Function \b Description: Dolby Virtual Surround
/// @param u8enVDS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetVDS(MS_U8 u8enVDS)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_VDS_Enable(u8enVDS);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetVSPK()
/// @brief \b Function \b Description: Dolby Virtual Speaker
/// @param u8enVSPK    \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetVSPK(MS_U8 u8enVSPK)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_VSPK_Enable(u8enVSPK);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSRS()
/// @brief \b Function \b Description: SRS TruSurr
/// @param u8enSRS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSRS(MS_U8 u8enSRS)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if(u8enSRS)
    {
        MDrv_SOUND_SRS_Enable(TRUE);
    }
    else
    {
        MDrv_SOUND_SRS_Enable(FALSE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SOUND_SRS_SetTsxtPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-TSXT
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_SRS_SetTsxtPara(MS_U8 u8mode, MS_U8 u8value)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SRS_SetTsxtPara(u8mode, u8value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSRSTSHD()
/// @brief \b Function \b Description: SRS TruSurround HD
/// @param u8enSRS     \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSRSTSHD(MS_U8 u8enTSHD)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if(u8enTSHD)
    {
        MDrv_SOUND_TSHD_Enable(TRUE);
    }
    else
    {
        MDrv_SOUND_TSHD_Enable(FALSE);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_SOUND_SRS_SetTshdPara()
/// @brief \b Function \b Description:  This function is used to set paramters of SRS-TSHD
/// @param u8mode      \b : parameter mode
/// @param u8value     \b : parameter value
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_SRS_SetTshdPara(MS_U8 u8mode, MS_U8 u8value)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SRS_SetTshdPara(u8mode, u8value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetBBE()
/// @brief \b Function \b Description: BBE Surround
/// @param u8enBBE     \b :   1: enable;  0: disable
/// @param u8BBEMode   \b :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetBBE(MS_U8 u8enBBE , MS_U8 u8BBEMode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_BBE_SetBbeMode(u8enBBE,u8BBEMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VSPK_WMod()
/// @brief \b Function \b Description: Dolby Virtual Speaker Wide Mod
/// @param u8VSPK_WMode \b : 0: reference mod; 1: wide mod
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_VSPK_WMod(MS_U8 u8VSPK_WMode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_VSPK_SetWMod(u8VSPK_WMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VSPK_SMod()
/// @brief \b Function \b Description: Dolby Virtual Speaker Surround Mod
/// @param u8VSPK_SMode \b : 0: movie mod; 1: music mod
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_VSPK_SMod(MS_U8 u8VSPK_SMode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_VSPK_SetSMod(u8VSPK_SMode);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SRS_DC()
/// @brief \b Function \b Description: SRS Dialog Clarity
/// @param u8SRSenDC   \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SRS_DC(MS_U8 u8SRSenDC)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SRS_DC(u8SRSenDC);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SRS_TruBass()
/// @brief \b Function \b Description:  SRS TruBass
/// @param u8SRSenTruBass \b :   1: enable;  0: disable
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SRS_TruBass(MS_U8 u8SRSenTruBass)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SRS_TruBass(u8SRSenTruBass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_TruBass()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Trubass
/// @param u8TSHDenTruBass \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_TSHD_TruBass(MS_U8 u8TSHDenTruBass)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_TSHD_TruBass(u8TSHDenTruBass);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_Definition()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Definition
/// @param u8TSHDenDefinition \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_TSHD_Definition(MS_U8 u8TSHDenDefinition)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_TSHD_Definition(u8TSHDenDefinition);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_TSHD_Clarity()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD DC
/// @param u8TSHDenClarity \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  _MApi_AUDIO_TSHD_Clarity(MS_U8 u8TSHDenClarity)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_TSHD_Clarity(u8TSHDenClarity);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description: Copy parameter from APP level
/// @param u8enVSPK    \b :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_COPY_Parameter(ADVSND_PARAM type, void* Parameter_ptr, MS_U32 size)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_COPY_Parameter( type, Parameter_ptr, size);
}

////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_KTV RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetKTVInfo()
/// @brief \b Function \b Description: Set KTV parameter
/// @param modelType   \b : KTV model type
/// @param infoType    \b : KTV infomation type
/// @param param1      \b :
/// @param param2      \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_PARAMTYPE paramType, MS_U32 param1, MS_U32 param2)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_SetKTVInfo (modelType, paramType, param1, param2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetKTVInfo()
/// @brief \b Function \b Description: Get KTV parameter
/// @param modelType   \b : KTV model type
/// @param infoType    \b : KTV infomation type
/// @return MS_U32     \b : return KTV value
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetKTVInfo (AUDIO_KTV_MODEL modelType, AUDIO_KTV_INFOTYPE infoType)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return (MDrv_AUDIO_GetKTVInfo (modelType, infoType));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMixModeVolume()
/// @brief \b Function \b Description: This routine is used to set the absolute u8Volume of mix mode.
/// @param eSourceType \b : for audio source
/// @param VolType     \b : for audio vol type
/// @param u8Vol1      \b : MSB 7-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x7E , gain: +12db to   -114db (-1 db per step)
/// @param u8Vol2      \b : LSB 3-bit register value of 10-bit u8Volume
///                         range from 0x00 to 0x07 , gain:  -0db to -0.875db (-0.125 db per step)
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetMixModeVolume(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_U8 u8Vol1, MS_U8 u8Vol2  )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

      MDrv_SOUND_SetMixModeVolume(eSourceType, VolType, u8Vol1, u8Vol2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMixModeMute()
/// @brief \b Function \b Description: This routine is used to set audio mix mode mute
/// @param eSourceType \b : for audio source
/// @param VolType     \b : for audio vol type
/// @param EnMute      \b : TRUE --Mute;
///                         FALSE--Unmute
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetMixModeMute(AUDIO_SOURCE_INFO_TYPE eSourceType, AUDIO_MIX_VOL_TYPE VolType, MS_BOOL EnMute )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetMixModeMute(eSourceType, VolType, EnMute);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PlayMenuSound()
/// @brief \b Function \b Description: This routine is used to Play Menu Sound.
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_PlayMenuSound(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

     MDrv_SOUND_PlayMenuSound();
}




////////////////////////////////////////////////////////////////////////////////
//
//           AUDIO_OTHERS RELATIONAL FUNCTION (Temp)
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetCertMode()
/// @brief \b Function \b Description:
/// @param CertType    \b :
/// @param enable      \b :
///
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MApi_AUDIO_SetCertMode( Audio_CertMode CertType,Audio_CertEnable enCert )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_SetCertMode(CertType, enCert));
}

/*
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetMaxClipping()
/// @brief \b Function \b Description: Limit output if volume setting is high and output is going to exceed maximum power.
///                                    It should be used only when OSD AVL option is Off.
/// @param MaxClippingValue \b : AVC threshold level for maximum volume
/// @param Volume           \b : Current volume value
////////////////////////////////////////////////////////////////////////////////
void MApi_AUDIO_SetMaxClipping( MS_U8 MaxClippingValue, MS_U16 Volume )
{
    MS_U8 value1,value2;
    MS_U16 AVL, VOL_Int, VOL_Fra;
    MS_U8 SpeakerMaxVolume = 0x0C; //(MS_U8)(TvVolumeTable[100]>>8);
    MS_U8 DynamicAvl_active_volume = 0x17; // about -12dB for 2Vrms input, value of volume control register

    value1 = (MS_U8)(Volume>>8) & 0x7F;
    value2 =(MS_U8)((Volume&0x00FF)>>4);

    // Dynamic AVL clipping algorithm
    if(value1 < DynamicAvl_active_volume)
    {
        VOL_Int= (value1 - SpeakerMaxVolume)*2;  //0.5*2 = 1
        VOL_Fra= (value2)/4;       //0.125*4 = 0.5
        if((VOL_Int + VOL_Fra) < MaxClippingValue)
            AVL = MaxClippingValue - (VOL_Int + VOL_Fra);
        else
            AVL = 0;
    }
    else  // value1 >= DynamicAvl_active_volume
        AVL = 0;

    MApi_AUDIO_SetAvcThreshold(AVL);
    //debugAudioPrint ("VOL_int [%d]\r\n", VOL_Int);
    //debugAudioPrint ("VOL_Fra [%d]\r\n", VOL_Fra);
    //debugAudioPrint ("AVL [%d]\r\n", AVL);

}
*/

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetPEQCoef()
/// @brief \b Function \b Description: This routine is used to set PEQ Coefficient.
/// @param <IN>        \b peq_coef
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetPEQCoef(AUDIO_PEQ_COEF *peq_coef)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_SetPEQCoef(peq_coef);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnablePEQ()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b Enpeq
/// @param <OUT>       \b NONE  :
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_EnablePEQ(MS_BOOL Enpeq)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SOUND_EnablePEQ(Enpeq);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_EnableDcRemove()
/// @brief \b Function \b Description: This routine is used to eable PEQ.
/// @param <IN>        \b EnDcRemove
/// @param <OUT>       \b True / False
/// @param <RET>       \b  NONE :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_EnableDcRemove(MS_BOOL EnDcRemove)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_SOUND_EnableDcRemove(EnDcRemove);
}

////////////////////////////////////////////////////////////////////////////////
//
//         AUDIO_MM RELATIONAL API FUNCTION
//
////////////////////////////////////////////////////////////////////////////////
//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetCommAudioInfo()
/// @brief \b Function \b Description: This routine is used for adjust Common Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetCommAudioInfo( Audio_COMM_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_SetCommAudioInfo(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetMpegInfo()
/// @brief \b Function \b Description: This routine is used for adjust MPEG Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetMpegInfo( Audio_MPEG_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_SetMpegInfo(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetAC3Info()
/// @brief \b Function \b Description: This routine is used for adjust AC3 Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetAC3Info( Audio_AC3_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_SetAC3Info(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetAC3PInfo()
/// @brief \b Function \b Description: This routine is used for adjust AC3+ Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetAC3PInfo( Audio_AC3P_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_SetAC3PInfo(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetAACInfo()
/// @brief \b Function \b Description: This routine is used for adjust AAC Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetAACInfo( Audio_AAC_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_SetAACInfo(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetWmaInfo()
/// @brief \b Function \b Description: This routine is used for adjust WMA Audio decoder parameters
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetWmaInfo( Audio_WMA_infoType infoType, MS_VIRT param1, MS_VIRT param2 )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_SetWmaInfo(infoType, param1, param2);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_SetDTSCommonCtrl()
/// @brief \b Function \b Description: This routine is used for adjust DTS Audio decoder control
/// @return MS_BOOL     \b : True / False
//******************************************************************************
MS_BOOL _MApi_AUDIO_SetDTSCommonCtrl( Audio_DTS_infoType infoType, Audio_DTS_CtrlType ctrl_type )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_SetDTSCommonCtrl(infoType, ctrl_type);
}



//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetCommAudioInfo()
/// @brief \b Function \b Description: This routine will return the 64bit Common info according to info type
/// @return MS_U64     \b : 64bit info
//******************************************************************************
MS_U64 _MApi_AUDIO_GetCommAudioInfo( Audio_COMM_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetCommAudioInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetMpegInfo()
/// @brief \b Function \b Description: This routine will return the 32bit mpeg info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetMpegInfo( Audio_MPEG_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetMpegInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetAC3Info()
/// @brief \b Function \b Description: This routine will return the 32bit ac3 info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetAC3Info( Audio_AC3_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetAC3Info(infoType);
}


//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetAC3PInfo()
/// @brief \b Function \b Description: This routine will return the 32bit ac3+ info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetAC3PInfo( Audio_AC3P_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetAC3PInfo(infoType);
}


//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetAACInfo()
/// @brief \b Function \b Description: This routine will return the 32bit AAC info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetAACInfo( Audio_AAC_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetAACInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetWmaInfo()
/// @brief \b Function \b Description: This routine will return the 32bit wma info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetWmaInfo( Audio_WMA_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetWmaInfo(infoType);
}

//******************************************************************************
/// @brief \b Function \b Name: MApi_AUDIO_GetDTSInfo()
/// @brief \b Function \b Description: This routine will return the 32bit DTS info according to info type
/// @return MS_U32     \b : 32bit info
//******************************************************************************
MS_U32 _MApi_AUDIO_GetDTSInfo( Audio_DTS_infoType infoType )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetDTSInfo(infoType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_XPCM_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType      \b :    1--lpcm;
///                               2--MS-ADPCM;
///                               17--IMA-ADPCM
/// @param channels       \b :    0-- mono;
///                               1-- stereo
/// @param sampleRate     \b :    8, 11.025, 16, 22.05, 44.1 and 48 (KHz)
/// @param bitsPerSample  \b :    8--8 bits;
///                               16--16 bits
/// @param blockSize      \b :    block size
/// @param samplePerBlock \b :    samples per block
/// @return MS_U8         \b :    TRUE--parameters are correct;
///                               FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_XPCM_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
     return(MDrv_MAD_XPCM_setParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_XPCM2_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType      \b :    1--lpcm;
///                               2--MS-ADPCM;
///                               17--IMA-ADPCM
/// @param channels       \b :    0-- mono;
///                               1-- stereo
/// @param sampleRate     \b :    8, 11.025, 16, 22.05, 44.1 and 48 (KHz)
/// @param bitsPerSample  \b :    8--8 bits;
///                               16--16 bits
/// @param blockSize      \b :    block size
/// @param samplePerBlock \b :    samples per block
/// @return MS_U8         \b :    TRUE--parameters are correct;
///                               FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_XPCM2_Param(XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate, MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

     MDrv_MAD2_XPCM_setParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_XPCM2_CheckIntStatus()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value for int status by IDMA
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U16  :     sram value for interrupt status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_XPCM2_CheckIntStatus(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_MAD2_XPCM2_CheckIntStatus();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_RA8_Param()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing RA8 LBR data
/// @param mNumCodecs  \b :    1--stereo or mono
///                            2~5-- multi channels
/// @param mSamples    \b :    output samples--256, 512, 1024
/// @param mSampleRate \b :    8, 11.025, 16, 22.05, 44.1 (KHz)
/// @param Channels    \b :    1-- mono
///                                    2-- stereo
/// @param cplStart    \b :    0 ~ 51
/// @param Regions     \b :    0 ~ 31
/// @param cplQbits    \b :    0, 2, 3, 4, 5, 6
/// @param FrameSize   \b :    bitstream size of every NumCodes (bytes)
///                                    Range--0 ~ 65535
/// @return MS_U8      \b :    TRUE--parameters are correct;
///                            FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_RA8_Param(MS_U16 mNumCodecs, MS_U16 mSamples, MS_U16 mSampleRate, MS_U16* Channels, MS_U16* Regions, MS_U16* cplStart, MS_U16* cplQbits, MS_U16* FrameSize)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

     return(MDrv_MAD_RA8_setParam(mNumCodecs, mSamples, mSampleRate, Channels, Regions, cplStart, cplQbits, FrameSize));
}

/******************************************************************************/
// Comomon Function for MM
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_Init()
/// @brief \b Function \b Description: MM Music mode Initial Audio decoder
/// @param <IN>        \b enDecSystem: Audio decoder type
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_Init(En_DVB_decSystemType enDecSystem)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    // need mute by APP

    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);

    u8IntTag = 1;
    MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);

    _MApi_AUDIO_SetSystem(enDecSystem);

    // need unmute by APP
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_StartDecode()
/// @brief \b Function \b Description: start play MM file
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_StartDecode(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYFILE);//MDrv_MAD_SetDecCmd(0x04);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_StartBrowse()
/// @brief \b Function \b Description: start browse file
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_StartBrowse(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    // decode data but output no sound
    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STARTBROWSE);//MDrv_MAD_SetDecCmd(0x05);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_StopDecode()
/// @brief \b Function \b Description: Stop decoder
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_StopDecode(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_STOP);// MDrv_MAD_SetDecCmd(0x00);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PauseDecode()
/// @brief \b Function \b Description: pause decoder
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_PauseDecode(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    _MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PAUSE);//MDrv_MAD_SetDecCmd(0x06);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CheckPlayDone()
/// @brief \b Function \b Description: Check file play done. If file play done, will return TRUE
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U8   : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_CheckPlayDone(void)
{
	AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (MDrv_AUDIO_CheckPlayDone() == TRUE)
    {
        _MApi_AUDIO_StopDecode();
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetResidualBufferSize()
/// @brief \b Function \b Description: Get ES buffer level.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16 : ES buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetResidualBufferSize(void)
{
    return MDrv_AUDIO_ReadDecMailBox(0) << 3; // line size is 8
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPCMBufferSize()
/// @brief \b Function \b Description: Get MP3 PCM buffer level.
/// @param <IN>        \b u16BitRate    : Bit Rate
/// @param <OUT>       \b MS_U16 : MP3 PCM buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetPCMBufferSize(MS_U16 u16BitRate)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U32 u32Tmp;

    u32Tmp = (MS_U32)MDrv_AUDIO_ReadDecMailBox(1);
    // line size is 8, 1 sample 2 bytes and 2 channels (Left & Right)
    //u32Tmp = ((u32Tmp << 1) * ((U32)u16BitRate * 1000 / 8))/MDrv_MAD_MPEG_GetSampleRate();
    if (_MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_SampleRate) != 0)
        u32Tmp = ((u32Tmp << 1) * ((MS_U32)u16BitRate * 1000 / 8))/_MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_SampleRate);
    return (MS_U16)u32Tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPCMBufferSize2()
/// @brief \b Function \b Description: Get PCM buffer level.
/// @param <IN>        \b u16BitRate : Bit Rate;
/// @param <IN>        \b u16SampleRate : Sample Rate
/// @param <OUT>       \b MS_U16 : PCM buffer level
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetPCMBufferSize2(MS_U16 u16BitRate,MS_U16 u16SampleRate)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U32 u32Tmp;

    u32Tmp = (MS_U32)MDrv_AUDIO_ReadDecMailBox(1);
    // line size is 8, 1 sample 2 bytes and 2 channels (Left & Right)
    if(u16SampleRate != 0)//UT
        u32Tmp = ((u32Tmp << 1) * ((MS_U32)u16BitRate * 1000 / 8))/u16SampleRate;

    return (MS_U16)u32Tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetInput()
/// @brief \b Function \b Description: This function will inform DSP that MCU already write data to ES buffer by interrupt.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetInput(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DEC1_MMTag, (MS_U32)u8IntTag, 0);
    MDrv_AUDIO_SendIntrupt(DSP_DEC, 0xE0);
    u8IntTag++;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetSampleRateIndex()
/// @brief \b Function \b Description: Set AAC sample rate index.
/// @param <IN>        \b u16Index    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetSampleRateIndex(MS_U16 u16Index)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    _MDrv_AUDIO_WriteDecMailBox(4,u16Index);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetXPCMParam()
/// @brief \b Function \b Description: Set XPCM paramaters
/// @param <IN>        \b audioType :
/// @param <IN>        \b channels :
/// @param <IN>        \b sampleRate :
/// @param <IN>        \b bitsPerSample :
/// @param <IN>        \b samplePerBlock :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetXPCMParam(XPCM_TYPE audioType, MS_U8 channels, MS_U16 sampleRate, MS_U8 bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    //MDrv_MAD_XPCM_setParam(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
    _MApi_AUDIO_XPCM_Param(audioType, channels, sampleRate, bitsPerSample, blockSize, samplePerBlock);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FileEndNotification()
/// @brief \b Function \b Description: This function will inform DSP that file end
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FileEndNotification(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    CheckPlayDoneCnt = 0;

    _MDrv_AUDIO_WriteDecMailBox(6, (MS_U16)u8IntTag);
    MDrv_AUDIO_FwTriggerDSP(0xe0);        // should change to MDrv_AUDIO_FwTriggerDSP
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FileEndDataHandle()
/// @brief \b Function \b Description: Info DSP that the last ES data left size.
/// @param <IN>        \b u32DataLeft    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FileEndDataHandle(MS_U32 u32DataLeft)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    _MDrv_AUDIO_WriteDecMailBox(5,(MS_U16)(u32DataLeft>>3));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_CheckInputRequest()
/// @brief \b Function \b Description: If MCU receive DSP interrupt, this function will return TRUE and report requested ES data buffer address and size
/// @param <IN>        \b pU32WrtAddr    :
/// @param <IN>        \b pU32WrtBytes   :
/// @param <OUT>       \b MS_U8          : TRUE or FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_CheckInputRequest(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if((pPHYWrtAddr == NULL) || (pU32WrtBytes == NULL))//UT
        return FALSE;
    if (MDrv_AUDIO_GetPlayFileFlag(DSP_DEC))
    {
        MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
        *pPHYWrtAddr = _MDrv_AUDIO_GetDspMadBaseAddr(DSP_DEC)+_MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferAddr);
        *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferSize);
        return TRUE;
    }
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetPlayTick()
/// @brief \b Function \b Description: Get Play Time stamp
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetPlayTick(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_1ms_PTS);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetEsMEMCnt()
/// @brief \b Function \b Description: Get ES Bufffer size
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16  : ES Bufffer size
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetEsMEMCnt(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_ReadDecMailBox(0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetSampleRate()
/// @brief \b Function \b Description: Get Sample Rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U16 : Sample Rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 _MApi_AUDIO_GetSampleRate(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U16 samplerate = 0;
    switch (enCurDecSystem)
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_MP3:
            samplerate = _MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_SampleRate);
            break;
        case MSAPI_AUD_DVB_WMA:
            samplerate = _MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_SampleRate);
            break;
        default:
            break;
    }
    return samplerate;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetBitRate()
/// @brief \b Function \b Description: Get Bit Rate
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : Bit Rate
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetBitRate(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U32 bitrate = 0;
    switch (enCurDecSystem)
    {
        case MSAPI_AUD_DVB_MPEG:
        case MSAPI_AUD_DVB_MP3:
            bitrate = _MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_BitRate);
            break;
        case MSAPI_AUD_DVB_WMA:
            bitrate = _MApi_AUDIO_GetWmaInfo(Audio_WMA_infoType_BitRate);
            break;
        default:
            break;
    }
    return bitrate;
}

/******************************************************************************/
//  for MP3
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetLayer()
/// @brief \b Function \b Description: Get MP3 Layer
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b Audio_MP3_LAYER : MPEG Layer
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
Audio_MP3_LAYER _MApi_AUDIO_GetLayer(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    // MDrv_MAD_MPEG_GetLayer() return the layer of music. 1 & 2 for layer mpeg 1 & 2. 3 is for mp3
    Audio_MP3_LAYER enLayer = Audio_MP3_LAYER_NOT_DETERMINE_YET;

    switch(_MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_Layer))
    {
        case 1:
            enLayer = Audio_MP3_LAYER_1;
            break;
        case 2:
            enLayer = Audio_MP3_LAYER_2;
            break;
        case 3:
            enLayer = Audio_MP3_LAYER_3;
            break;
        default:
            break;
    }
    return(enLayer);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetCurrentFrameNumber()
/// @brief \b Function \b Description: Get MP3 Current Frame Number
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : Current Frame Number
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetCurrentFrameNumber(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return _MApi_AUDIO_GetMpegInfo(Audio_MPEG_infoType_FrameNum);
}

/******************************************************************************/
//  for WMA
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetASFParm()
/// @brief \b Function \b Description: Set WMA ASF Paramaters
/// @param <IN>        \b parm_type  :
/// @param <IN>        \b value   :
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_SetASFParm(WMA_ASF_PARMTYPE parm_type, MS_U32 value)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    _MApi_AUDIO_SetWmaInfo(Audio_WMA_infoType_Asf_Param, parm_type, value);
}


/******************************************************************************/
//  for MM File Hand Shake
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM_SetInput()
/// @brief \b Function \b Description: MCU send interrupt to decoder1/2 in MM mode
/// @param <IN>        \b AUDDecoderNo  : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <IN>        \b u8IntTag   : Interrupt tag, increase by every time
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_MM_SetInput (En_DVB_AudioDecoder AUDDecoderNo, MS_U8 u8IntTag)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (AUDDecoderNo == DVB_Audio_Decoder1)
    {
        MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DEC1_MMTag, (MS_U32)u8IntTag, 0);
        MDrv_AUDIO_SendIntrupt(DSP_DEC, 0xE0);
    }
    else
    {
        MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_DEC2_MMTag, (MS_U32)u8IntTag, 0);
        MDrv_AUDIO_SendIntrupt(DSP_SE, 0xE0);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM_CheckPlayDone()
/// @brief \b Function \b Description: MCU check audio play end in MM mode
/// @param <IN>        \b AUDDecoderNo  : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <OUT>       \b MS_U16    : Value of residual PCM level for file end stop
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_MM_CheckPlayDone (En_DVB_AudioDecoder AUDDecoderNo)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (AUDDecoderNo == DVB_Audio_Decoder1)
    {
        return((MS_U32)MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_MMResidualPCM));
    }
    else
    {
        return((MS_U32)MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC2_MMResidualPCM));
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM_CheckInputRequest()
/// @brief \b Function \b Description  : Decoder request input addr & size in MM mode
/// @param <IN>        \b AUDDecoderNo : DVB_Audio_Decoder1 / DVB_Audio_Decoder2
/// @param <IN>        \b pU32WrtAddr  : Request input address in ES1 buffer
/// @param <IN>        \b pU32WrtBytes : Request input size
/// @param <OUT>       \b MS_U8   : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_MM_CheckInputRequest(En_DVB_AudioDecoder AUDDecoderNo, MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (AUDDecoderNo == DVB_Audio_Decoder1)
    {
        if (MDrv_AUDIO_GetPlayFileFlag(DSP_DEC))
        {
            MDrv_AUDIO_SetPlayFileFlag(DSP_DEC, 0);
            *pPHYWrtAddr =  _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_esBuf_reqAddr);
            *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC1_BufferSize);
            return TRUE;
        }
        return FALSE;
    }
    else
    {
        if (MDrv_AUDIO_GetPlayFileFlag(DSP_SE))
        {
            MDrv_AUDIO_SetPlayFileFlag(DSP_SE, 0);
            *pPHYWrtAddr =  _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC2_esBuf_reqAddr);
            *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_DEC2_BufferSize);
            return TRUE;
        }
        return FALSE;
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM2_initAesInfo()
/// @brief \b Function \b Description  : Initialize AES info
/// @param <IN>        \b dec_id  : Decoder ID
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_MM2_initAesInfo( AUDIO_DEC_ID dec_id )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);
    return MDrv_AUDIO_MM2_initAesInfo( dec_id );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM2_checkAesInfo()
/// @brief \b Function \b Description  : Get AES info from audio decoder
/// @param <IN>        \b dec_id       : Decoder ID
/// @param <IN>        \b aes_info     : AES info structure pointer
/// @param <OUT>       \b MS_BOOL : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_MM2_checkAesInfo( AUDIO_DEC_ID  dec_id, AES_INFO *aes_info )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_MM2_checkAesInfo( dec_id, aes_info );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_MM2_inputAesFinished()
/// @brief \b Function \b Description  : Inform DSP how much ES data is sent
/// @param <IN>        \b dec_id	   : Decoder ID
/// @param <IN>        \b es_size  : Size of ES data
/// @param <IN>        \b ptsExist : Whether ES data contains PTS or not
/// @param <IN>        \b pts	   : PTS
/// @param <OUT>       \b MS_BOOL  : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE     :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_MM2_inputAesFinished( AUDIO_DEC_ID dec_id, MS_U32 es_size, MS_BOOL ptsExist, MS_U64 pts )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_MM2_inputAesFinished( dec_id, es_size, ptsExist, pts );
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_Enable()  @@Need_Modify
/// @brief \b Function \b Description:  Enable/ Disable the path of USB PCM
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_USBPCM_Enable(MS_BOOL bEnable)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_USBPCM_Enable(bEnable);
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_SetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  clear interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b bEnable :  set false to clean interrupt flag
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_USBPCM_SetFlag(MS_BOOL bEnable)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_USBPCM_SetFlag(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_GetFlag()  @@Need_Modify
/// @brief \b Function \b Description:  Get interrupt flag for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b BOOL    :   interrupt flag
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_USBPCM_GetFlag(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_USBPCM_GetFlag();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_USBPCM_GetMemInfo()  @@Need_Modify
/// @brief \b Function \b Description:  Get memory address and size for USBPCM function
///                    \b               (Encoder path)
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b MS_U32  : address, size
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void    _MApi_AUDIO_USBPCM_GetMemInfo(AUDIO_UPLOAD_INFO *uploadInfo)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_USBPCM_GetMemInfo(uploadInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DmaReader_Init(SAMPLE_RATE sampleRate)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_DmaReader_Init(sampleRate);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_AllInput_Init()
/// @brief \b Function \b Description: Used DMA reader in all audio source application
///                    \b              (ex: Key sound) (CHIP_T8/T12) (48KHz only)
/// @param <IN>        \b   eType   : sampleRate
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DmaReader_AllInput_Init(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_DmaReader_AllInput_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaReader_WritePCM()
/// @brief \b Function \b Description: This routine is used to write PCM data into DMA reader Buffer
/// @param <IN>        \b   eType   : buffer size (must be multiple of 8 Byte)
/// @param <OUT>       \b MS_BOOL    : TRUE or FALSE
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_DmaReader_WritePCM(void* buffer, MS_U32 bytes)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return(MDrv_AUDIO_DmaReader_WritePCM(buffer,bytes));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_DmaWriter_Init()
/// @brief \b Function \b Description:
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_DmaWriter_Init()
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_DmaWriter_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_VoIP_Config()
/// @brief \b Function \b Description:  This routine is used for VoIP applications
/// @param <IN>        \b AUDIO_VoIP_CONFIG_TYPE : configType
/// @param <IN>        \b MS_U32 : param1
/// @param <IN>        \b MS_U32 : param2
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_VoIP_Config(AUDIO_VoIP_CONFIG_TYPE configType, MS_U32 param1, MS_U32 param2)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_VoIP_Config(configType, param1, param2);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ALSA_Check()
/// @brief \b Function \b Description:  Check if ALSA Interface is supported
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if it's supported, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ALSA_Check(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_ALSA_Check();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ALSA_Enable()
/// @brief \b Function \b Description:  Enable/ Disable the path of ALSA
/// @param <IN>        \b bEnable : on: TRUE, off: FALSE
/// @param <OUT>       \b MS_BOOL    : return TRUE if ok, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_ALSA_Enable(MS_BOOL bEnable)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if (_MApi_AUDIO_ALSA_Check() != TRUE)
    {
        return FALSE;
    }

    if (bEnable == TRUE)
    {
        _MApi_AUDIO_SetSourceInfo(E_AUDIO_INFO_GAME_IN);
        _MApi_AUDIO_InputSwitch(AUDIO_DSP1_DVB_INPUT, E_AUDIO_GROUP_MAIN);
    }

    return MDrv_AUDIO_ALSA_Enable(bEnable);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_CheckDecodeDone()
/// @brief \b Function \b Description  : Decoder report pcm out addr & size in Uniplayer mode
/// @param <IN>        \b pU32WrtAddr  : Request input address in ES1 buffer
/// @param <IN>        \b pU32WrtBytes : Request input size
/// @param <OUT>       \b MS_U8   : return TRUE if ok, else return FALSE
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_UNI_CheckDecodeDone(MS_PHY *pPHYWrtAddr, MS_U32 *pU32WrtBytes)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    APIAUDIO_CHECK_SHM_INIT;

    if ( g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] != MDrv_AUDIO_GetUniDecodeDoneTag() )
    {
        g_AudioVars2->u16DecodeFrmCnt[AU_DEC_ID1] = MDrv_AUDIO_GetUniDecodeDoneTag();
        *pPHYWrtAddr  = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_pcm_wptr);
        *pU32WrtBytes = _MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_pcm_decSize);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_SetOutput()
/// @brief \b Function \b Description: MCU send interrupt to decoder1 in Uniplayer mode
/// @param <IN>        \b PCMOutCnt   : Get output PCM count by MCU
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_UNI_SetOutput (MS_U32 PCMOutCnt)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_PCMOutCnt, PCMOutCnt, 0);
    MDrv_AUDIO_SendIntrupt(DSP_DEC, 0xE3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_Set_PCMInputWriteAddr()
/// @brief \b Function \b Description: MCU send DEC1-PCM3 wptr to decoder1 in Uniplayer mode
/// @param <IN>        \b PCMIn_Wptr   : PCM3 write pointer to DSP
/// @param <OUT>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_UNI_Set_PCMInputWriteAddr (MS_U32 PCMIn_Wptr)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_AUDIO_SetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_PCMInput_Addr, PCMIn_Wptr, 0);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_UNI_Get_OutPCMLevel()
/// @brief \b Function \b Description: Decoder1 report DEC1-PCM3 buffer level in Uniplayer mode
/// @param <OUT>       \b MS_U32  : DEC1-PCM3 buffer level (byte unit)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_UNI_Get_OutPCMLevel (void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return (MDrv_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_ADEC1_UNI_OutPCMLevel));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_RingTask()
/// @brief \b Function \b Description :
/// @param <IN>        \b auRingTask  :
/// @param <GLOBAL>    \b NONE        :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_RingTask(audioRingTask* auRingTask)        // this function is used for auto ring address
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_U32 u32Addr_temp;
    MS_U32 u32Size_temp;
    auRingTask->u8RingStatus = 0;   // 0: no need to ring
                                    // 1: not ring, but at the edge of buffer, next time will start from the head of the buf
                                    // 2: ring
                                    // 3: special case, pointer out or range

    if((auRingTask->u32ReqAddr >= auRingTask->_stBufInfo.u32BufStartAddr)&&(auRingTask->u32ReqAddr <= auRingTask->_stBufInfo.u32BufEndAddr))
    {
        u32Addr_temp = auRingTask->u32ReqAddr + auRingTask->u32ReqSize; // target end address

        if(u32Addr_temp < auRingTask->_stBufInfo.u32BufEndAddr )   // no need to ring
        {
            auRingTask->u8RingStatus = 0;
            auRingTask->phyRetAddr1 = auRingTask->u32ReqAddr;
            auRingTask->u32RetSize1 = auRingTask->u32ReqSize;
            auRingTask->_stBufInfo.u32BufWrPtr = u32Addr_temp;
        }
        else if(u32Addr_temp == auRingTask->_stBufInfo.u32BufEndAddr)   // not ring, just make
        {
            auRingTask->u8RingStatus = 1;
            auRingTask->phyRetAddr1 = auRingTask->u32ReqAddr;
            auRingTask->u32RetSize1 = auRingTask->u32ReqSize;
            auRingTask->_stBufInfo.u32BufWrPtr = auRingTask->_stBufInfo.u32BufStartAddr;
        }
        else    // ring
        {
            auRingTask->u8RingStatus = 2;
            auRingTask->phyRetAddr1 = auRingTask->u32ReqAddr;
            auRingTask->u32RetSize1 = auRingTask->_stBufInfo.u32BufEndAddr - auRingTask->u32ReqAddr;
            auRingTask->phyRetAddr2 = auRingTask->_stBufInfo.u32BufStartAddr;
            auRingTask->u32RetSize2 = auRingTask->u32ReqSize -auRingTask->u32RetSize1;
            auRingTask->_stBufInfo.u32BufWrPtr = auRingTask->_stBufInfo.u32BufStartAddr + auRingTask->u32RetSize2;
        }
    }
    else if(auRingTask->u32ReqAddr > auRingTask->_stBufInfo.u32BufEndAddr)   // special case, pointer out of range
    {
        auRingTask->u8RingStatus = 3;
        u32Size_temp = auRingTask->u32ReqAddr - auRingTask->_stBufInfo.u32BufEndAddr;
        auRingTask->phyRetAddr1 = auRingTask->_stBufInfo.u32BufStartAddr + u32Size_temp;
        auRingTask->u32RetSize1 = auRingTask->u32ReqSize;
        auRingTask->_stBufInfo.u32BufWrPtr = auRingTask->phyRetAddr1 + auRingTask->u32RetSize1;
    }
    else
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\r\n @@@ request pointer error, smaller than start address @@@ \r\n");
    }
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_Ring_DataTransfer()
/// @brief \b Function \b Description :
/// @param <IN>        \b auRingTask  :
/// @param <GLOBAL>    \b NONE        :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_Ring_DataTransfer(audioRingTask* auRingTask, audioRingTask* auRingTask_1)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    switch(auRingTask->u8RingStatus)     // copy data from PCM1 to PCM2
    {
        case 0:     // no need to ring for read
        case 1:     // just make, no need to ring
            if((auRingTask_1->u8RingStatus == 0)||(auRingTask_1->u8RingStatus == 1))
            {
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1, auRingTask_1->phyRetAddr1, (MS_U16)auRingTask_1->u32RetSize1);
                AU_memcpy((void *) (MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1)), (void *)MsOS_PA2KSEG1(auRingTask->phyRetAddr1), auRingTask_1->u32RetSize1);
            }
            else if(auRingTask_1->u8RingStatus == 2) // need to ring
            {
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1, auRingTask_1->phyRetAddr1, (MS_U16)auRingTask_1->u32RetSize1);
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", (auRingTask->phyRetAddr1 + auRingTask_1->u32RetSize1), auRingTask_1->phyRetAddr2, (MS_U16)auRingTask_1->u32RetSize2);
                AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1), auRingTask_1->u32RetSize1);
                AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1 + auRingTask_1->u32RetSize1), auRingTask_1->u32RetSize2);
            }
            else
            {
               AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\r\n ==== kochien print:  data control error @@@@ \r\n");
            }
            break;

        case 2:
            if((auRingTask_1->u8RingStatus == 0)||(auRingTask_1->u8RingStatus == 1))  // only PCM1 need to ring
            {
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1, auRingTask_1->phyRetAddr1, (MS_U16)auRingTask->u32RetSize1);
                //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr2, auRingTask_1->phyRetAddr1 + auRingTask->u32RetSize1, (MS_U16)auRingTask->u32RetSize2);
                AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1), auRingTask->u32RetSize1);
                AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1 + auRingTask->u32RetSize1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr2), auRingTask->u32RetSize2);
            }
            else if(auRingTask_1->u8RingStatus == 2) // both PCM1 and PCM2 need to ring
            {
                if(auRingTask->u32RetSize1 == auRingTask_1->u32RetSize1)
                {
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1, auRingTask_1->phyRetAddr1, (MS_U16)auRingTask->u32RetSize1);
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr2, auRingTask_1->phyRetAddr2, (MS_U16)auRingTask->u32RetSize2);
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1), auRingTask->u32RetSize1);
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr2), auRingTask->u32RetSize2);
                }
                else if(auRingTask->u32RetSize1 > auRingTask_1->u32RetSize1)
                {
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1, auRingTask_1->phyRetAddr1, (MS_U16)auRingTask->u32RetSize1);
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1 + auRingTask_1->u32RetSize1, auRingTask_1->phyRetAddr2, (MS_U16)(auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1));
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr2, auRingTask_1->phyRetAddr2 + auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1, (MS_U16)auRingTask->u32RetSize2);
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1), auRingTask_1->u32RetSize1);
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1 + auRingTask_1->u32RetSize1), (auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1));
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr2 + auRingTask->u32RetSize1 - auRingTask_1->u32RetSize1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr2), auRingTask->u32RetSize2);
                }
                else
                {
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr1, auRingTask_1->phyRetAddr1, (MS_U16)auRingTask->u32RetSize1);
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", auRingTask->phyRetAddr2, auRingTask_1->phyRetAddr1 + auRingTask->u32RetSize1, (MS_U16)(auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1));
                    //AUDIO_PRINT(AUDIO_DEBUG_LEVEL_INFO, " Sr[0x%lx]->Des[0x%lx], size[0x%x]", (auRingTask->phyRetAddr2 + auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1), auRingTask_1->phyRetAddr2, (MS_U16)auRingTask_1->u32RetSize2);
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr1), auRingTask->u32RetSize1);
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr1 + auRingTask->u32RetSize1)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr2), (auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1));
                    AU_memcpy((void*)(MsOS_PA2KSEG1(auRingTask_1->phyRetAddr2)), (void*)MsOS_PA2KSEG1(auRingTask->phyRetAddr2 + auRingTask_1->u32RetSize1 - auRingTask->u32RetSize1), auRingTask_1->u32RetSize2);
                }
            }
            else
            {
                //AudioDbgPrint_Uniplayer(AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\r\n ==== data control error \r\n" ));
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\r\n ==== case2: data control error \r\n");
            }
            break;

        case 3:
        default:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "\r\n ==== case3: data control error \r\n");
            break;
    }
    MsOS_FlushMemory();
    MsOS_Sync();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_SetDataCaptureSource()
/// @brief \b Function \b Description:  Select source for data capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_SetDataCaptureSource(AUDIO_DEVICE_TYPE eID, AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_SetDataCaptureSource(eID, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Init()
/// @brief \b Function \b Description:  Select source for pcm capture
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b AUDIO_CAPTURE_SOURCE_TYPE : Data Source
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Init(const AUDIO_DEVICE_TYPE eID, const AUDIO_CAPTURE_SOURCE_TYPE eSource)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_PCMCapture_Init(eID, eSource);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Start()
/// @brief \b Function \b Description:  captrue pcm data to DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Start(const AUDIO_DEVICE_TYPE eID)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

   return MDrv_AUDIO_PCMCapture_Start(eID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Stop()
/// @brief \b Function \b Description:  stop captrue pcm data from DDR
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Stop(const AUDIO_DEVICE_TYPE eID)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

     return MDrv_AUDIO_PCMCapture_Stop(eID);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCMCapture_Read()
/// @brief \b Function \b Description:  captrue pcm data from DDR to device
/// @param <IN>        \b AUDIO_DEVICE_TYPE : select 1st or 2nd data capture
/// @param <IN>        \b void* : destination buffer pointer
/// @param <IN>        \b MS_U32 : buffer size need transfered in byte
/// @param <OUT>       \b MS_BOOL    : return TRUE if success, else return FALSE
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_PCMCapture_Read(const AUDIO_DEVICE_TYPE eID, void *buffer, const MS_U32 bytes)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_PCMCapture_Read(eID, buffer, bytes);
}

//====== STAOS PIP START 2012/02/01 ========

MS_BOOL _MApi_AUDIO_SetAudioParam2( AUDIO_DEC_ID DecId, Audio_ParamType paramType, MS_VIRT Param )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    return MDrv_AUDIO_SetAudioParam2(DecId, paramType, Param);
}

MS_BOOL _MApi_AUDIO_GetAudioInfo2( AUDIO_DEC_ID DecId, Audio_InfoType infoType, void * pInfo )
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MS_BOOL bRet = FALSE;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(bRet);
    if (bRet == FALSE)
    {
        return bRet;
    }

    if (NULL == pInfo)
    {
        AUDIO_PRINT(AUDIO_DEBUG_LEVEL_ERROR, "%s: pInfo not be NULL !!\r\n", __FUNCTION__);
        return FALSE;
    }

    return MDrv_AUDIO_GetAudioInfo2(DecId, infoType, pInfo);
}

//====== STAOS PIP END 2012/02/01 ========

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GetSoundMode()
/// @brief \b Function \b Description: Getting FM Radio Sound Mode
/// @param <IN>        \b NONE :
/// @param <OUT>       \b MS_U8   : return E_AUDIOMODE_MONO, E_AUDIOMODE_K_STEREO or E_AUDIOMODE_INVALID
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_FM_RADIO_GetSoundMode(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    AUDIOSTATUS eCurrentAudioStatus = E_STATE_AUDIO_NO_CARRIER;

    if (MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_FM_RADIO))
    {
        return E_AUDIOMODE_INVALID;
    }

    _MApi_AUDIO_SIF_GetAudioStatus(&eCurrentAudioStatus);

    if(m_eSifStatus & E_STATE_AUDIO_PRIMARY_CARRIER)
    {
        eCurrentAudioStatus |= E_STATE_AUDIO_PRIMARY_CARRIER;
    }

    if(m_eSifStatus & E_STATE_AUDIO_STEREO)
    {
        eCurrentAudioStatus |= E_STATE_AUDIO_STEREO;
    }

    if(eCurrentAudioStatus == E_STATE_AUDIO_PRIMARY_CARRIER)
    {
        return E_AUDIOMODE_MONO;
    }
    else if (eCurrentAudioStatus == (E_STATE_AUDIO_PRIMARY_CARRIER|E_STATE_AUDIO_STEREO))
    {
        return E_AUDIOMODE_K_STEREO;
    }
    else
    {
        return E_AUDIOMODE_INVALID;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_SetSoundMode()
/// @brief \b Function \b Description: Setting FM Radio Sound Mode
/// @param <IN>        \b MS_U8 : u8FMRadioSoundMode
/// @param <OUT>       \b NONE :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FM_RADIO_SetSoundMode(MS_U8 u8FMRadioSoundMode)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

   switch(u8FMRadioSoundMode)
   {
        case E_AUDIOMODE_MONO:
        case E_AUDIOMODE_FORCED_MONO:
            MDrv_SIF_SetSoundMode(AU_MODE_MONO);
            break;

        case E_AUDIOMODE_G_STEREO:
        case E_AUDIOMODE_K_STEREO:
            MDrv_SIF_SetSoundMode(AU_MODE_STEREO);
            break;

        default:
            break;
   }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_DeEmphassisOption()
/// @brief \b Function \b Description: FM Radio De-emphassis Option
/// @param <IN>        \b MS_BOOL : Is_Europe
/// @param <OUT>       \b NONE :
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FM_RADIO_DeEmphassisOption(MS_BOOL Is_Europe)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    if(Is_Europe)
        MDrv_SIF_SendCmd(AU_SIF_CMD_SET_FM_RADIO_DeEmp, 1, NULL);
    else
        MDrv_SIF_SendCmd(AU_SIF_CMD_SET_FM_RADIO_DeEmp, 0, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GET_DC_AMP()
/// @brief \b Function \b Description: FM Radio get DC amp
/// @param <IN>        \b NONE
/// @param <OUT>       \b MS_S8 DC amp
////////////////////////////////////////////////////////////////////////////////
MS_S8 _MApi_AUDIO_FM_RADIO_GET_DC_AMP(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return (MS_S8)MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_RADIO_DC_AMP, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_GET_NSR()
/// @brief \b Function \b Description: FM Radio get NSR
/// @param <IN>        \b NONE
/// @param <OUT>       \b MS_U8 NSR
////////////////////////////////////////////////////////////////////////////////
MS_U8 _MApi_AUDIO_FM_RADIO_GET_NSR(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_SIF_SendCmd(AU_SIF_CMD_GET_FM_RADIO_NSR, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetDDRInfo()
/// @brief \b Function \b Description:  Return Audio DDR info
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b EN_AUDIO_DDRINFO : DDR info
/// @param <OUT>       \b MS_U32  : return DDR info
/// @param <RET>       \b NONE    :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_PHY _MApi_AUDIO_GetDDRInfo(AUDIO_DEC_ID DecId, EN_AUDIO_DDRINFO DDRInfo)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    return MDrv_AUDIO_GetDDRInfo(DecId, DDRInfo);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_FM_RADIO_RESET()
/// @brief \b Function \b Description: FM Radio reset
/// @param <IN>        \b NONE
/// @param <OUT>       \b NONE
////////////////////////////////////////////////////////////////////////////////
void _MApi_AUDIO_FM_RADIO_RESET(void)
{
    AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "[%s] \n", __FUNCTION__);

    MDrv_SIF_SendCmd(AU_SIF_CMD_FM_RADIO_RESET, NULL, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_GetCaps()
/// @brief \b Function \b Description: Get Audio Capabilities
/// @param <IN>        \b AUDIO_DEC_ID : select audio processor
/// @param <IN>        \b MS_U32 * : Audio Capabilites pointer
/// @param <OUT>        \b MS_BOOL : return TRUE if success, else return FALSE
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_GetCaps(AUDIO_DEC_ID DecId, MS_U32 *pCaps)
{
    MS_U32 ret = FALSE;

    ret = MDrv_AUDIO_GetCaps(DecId, pCaps);

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_Suspend()
/// @brief \b Function \b Description: audio suspend
/// @param <IN>        \b : void*
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_Suspend(void)
{
    MS_BOOL ret = TRUE;
    int i = 0, j = 0;

    APIAUDIO_CHECK_SHM_INIT;

    APIAUDIO_CHECK_INIT_FLAG(ret);
    if (ret == FALSE)
    {
        return FALSE;
    }

    //SerPrintf("\033[32m  [Utopia2K] %s : %d suspending \033[m\n", __FUNCTION__,__LINE__);

    while (MBOXBkUp[i].Address != 0)
    {
        for ( j = 0; j < MBOXBkUp[i].Length; j++)
        {
            MBOXBkUp[i].u16Data[j] = MDrv_AUDIO_AbsReadReg((MBOXBkUp[i].Address + (j * 2)));
        }

        i++;
    }

    _MApi_AUDIO_SetPowerOn(FALSE);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_Resume()
/// @brief \b Function \b Description: audio resume
/// @param <IN>        \b : void*
////////////////////////////////////////////////////////////////////////////////
MS_BOOL _MApi_AUDIO_Resume(void)
{
    MS_BOOL ret = TRUE;
    int i = 0, j = 0, DecID=AU_DEC_ID1;
    AudioDecStatus_t  stAudioDecStatus;

    APIAUDIO_CHECK_SHM_INIT;

    //printk("\033[32m [Utopia2K] %s : %d Resume \033[m\n", __FUNCTION__,__LINE__);

    g_AudioVars2->g_bHashkeyFlag = FALSE;

    _MApi_AUDIO_WritePreInitTable();
    _MApi_AUDIO_Initialize();

    while (MBOXBkUp[i].Address != 0)
    {
        for (j = 0; j < MBOXBkUp[i].Length; j++)
        {
            MDrv_AUDIO_AbsWriteReg((MBOXBkUp[i].Address + (j * 2)), MBOXBkUp[i].u16Data[j]);
        }

        i++;
    }

    MDrv_AUDIO_ResetDspCodeType();

    switch( g_AudioVars2->eMainAudioSource )
    {
        case AUDIO_DSP1_DVB1_INPUT:
        case AUDIO_DSP1_DVB_INPUT:
        case AUDIO_DSP4_SIF_INPUT:
            for (DecID = 0; DecID < AU_DEC_MAX; DecID++)
            {
                if ( (g_AudioVars2->AudioDecStatus[DecID].bConnect == TRUE) && (g_AudioVars2->AudioDecStatus[DecID].eGroup == E_CONNECT_MAIN) )
                {
                    memcpy(&stAudioDecStatus, &(g_AudioVars2->AudioDecStatus[DecID]), sizeof(AudioDecStatus_t));
                    g_AudioVars2->AudioDecStatus[DecID].eAudFormat = MSAPI_AUD_DVB_NONE;
                    MDrv_AUDIO_SetDecodeSystem(DecID, &stAudioDecStatus);
                    _MApi_AUDIO_InputSwitch(g_AudioVars2->eMainAudioSource, g_AudioVars2->AudioDecStatus[DecID].eGroup);
                    MDrv_MAD_SetDecCmd(AU_DVB_DECCMD_PLAY);
                }
            }
            break;

            default:
                break;
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Open()
/// @brief \b Function \b Description : Open a PCM interface according to specific capabilities.
/// @param <IN>        \b void : Pointer to a block of memory with PCM information.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen device-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Open(void *pData)
{
    return MDrv_AUDIO_PCM_Open(pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Close()
/// @brief \b Function \b Description : Close an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Close(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Close(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Start()
/// @brief \b Function \b Description : Start an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Start(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Start(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Stop()
/// @brief \b Function \b Description : Stop a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Stop(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Stop(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Set()
/// @brief \b Function \b Description : Set configuration to an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory with specific configuration, and this memory is not allowed to be updated by this API.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Set(MS_S32 s32DeviceId, MS_U32 u32Cmd, const void *pData)
{
    return MDrv_AUDIO_PCM_Set(s32DeviceId, u32Cmd, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Get()
/// @brief \b Function \b Description : Get configuration from an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b MS_U32 : The command type.
/// @param <IN>        \b void : Pointer to a block of memory for receiving specific configuration.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Get(MS_S32 s32DeviceId, MS_U32 u32Cmd, void *pData)
{
    return MDrv_AUDIO_PCM_Get(s32DeviceId, u32Cmd, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Read()
/// @brief \b Function \b Description : Read a specific amount of PCM data from a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_PCM_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{
    return MDrv_AUDIO_PCM_Read(s32DeviceId, pBuf, u32Size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Write()
/// @brief \b Function \b Description : Write a specific amount of PCM data to a started PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with PCM data, and this memory is not allowed to be updated by this API.
/// @param <IN>        \b MS_U32 : The size of PCM data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of PCM data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_PCM_Write(MS_S32 s32DeviceId, const void *pBuf, MS_U32 u32Size)
{
    return MDrv_AUDIO_PCM_Write(s32DeviceId, pBuf, u32Size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_PCM_Flush()
/// @brief \b Function \b Description : Flush an opened PCM interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_PCM_Flush(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_PCM_Flush(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Open()
/// @brief \b Function \b Description : Open an encoder interface according to specific device name.
/// @param <IN>        \b char : Pointer to a string with name.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success a nonnegative value is returned, and this value is the chosen device-id. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
///
/// Device Name :
///     "MP3_ENCODER1" : 1st MP3 Encoder
///     "MP3_ENCODER2" : 2nd MP3 Encoder
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_ENCODER_Open(const char *pName)
{
    return MDrv_AUDIO_ENCODER_Open(pName);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Close()
/// @brief \b Function \b Description : Close an opened encoder interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_ENCODER_Close(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_ENCODER_Close(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Start()
/// @brief \b Function \b Description : Start an opened encoder interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_ENCODER_Start(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_ENCODER_Start(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Stop()
/// @brief \b Function \b Description : Stop a started encoder interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_ENCODER_Stop(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_ENCODER_Stop(s32DeviceId);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Ioctl()
/// @brief \b Function \b Description : Set/Get configuration to an opened encoder interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b char : Pointer to a string with command name.
/// @param <IN>        \b void : Pointer to a block of memory with specific configuration to deliver, or to receive specific configuration.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
///
/// Command String Name :
///     "GetFrameSize" : Get the size of current encoded frame.
///     "GetAvailableSize" : Get the size of total available encoded frames.
///     "GetTimestampHigh32Bit" : Get high 32 bits of timestamp from current encoded frame.
///     "GetTimestampLow32Bit" : Get low 32 bits of timestamp from current encoded frame.
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_ENCODER_Ioctl(MS_S32 s32DeviceId, const char *pCmd, void *pData)
{
    return MDrv_AUDIO_ENCODER_Ioctl(s32DeviceId, pCmd, pData);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Read()
/// @brief \b Function \b Description : Read a specific amount of encoded data from a started encoder interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <IN>        \b void : Pointer to a block of memory with encoded data.
/// @param <IN>        \b MS_U32 : The size of encoded data.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_U32 : The total number of encoded data successfully written is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_U32 _MApi_AUDIO_ENCODER_Read(MS_S32 s32DeviceId, void *pBuf, MS_U32 u32Size)
{
    return MDrv_AUDIO_ENCODER_Read(s32DeviceId, pBuf, u32Size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MApi_AUDIO_ENCODER_Flush()
/// @brief \b Function \b Description : Flush an opened encoder interface.
/// @param <IN>        \b MS_S32 : The chosen device-id.
/// @param <OUT>       \b NONE :
/// @param <RET>       \b MS_S32 : On success zero is returned. On error, a negative value is returned.
/// @param <GLOBAL>    \b NONE :
////////////////////////////////////////////////////////////////////////////////
MS_S32 _MApi_AUDIO_ENCODER_Flush(MS_S32 s32DeviceId)
{
    return MDrv_AUDIO_ENCODER_Flush(s32DeviceId);
}