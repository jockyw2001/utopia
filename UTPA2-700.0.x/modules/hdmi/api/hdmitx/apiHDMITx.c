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
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (¡§MStar Confidential Information¡¨) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiHDMITx.c
/// @author MStar Semiconductor,Inc.
/// @brief  HDMI Tx Api
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdlib.h>
#include <string.h>
#else
#include <linux/time.h>
#endif /* #include MSOS_TYPE_LINUX_KERNEL */

#include "MsCommon.h"
#include "drvHDMITx.h"
#include "apiHDMITx.h"

//wilson@kano: for U2
#include "MsOS.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiHDMITx_v2.h"
#include "HDMITX_private.h"
#include "MsDevice.h"



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

// Mutex & Lock
static MS_S32 _s32HDMITX_Mutex = -1;

#define HDMITX_ENTRY()      if(!MsOS_ObtainMutex(_s32HDMITX_Mutex, MSOS_WAIT_FOREVER))                     \
                            {                                                                        \
                            }

#define HDMITX_RETURN()     MsOS_ReleaseMutex(_s32HDMITX_Mutex);

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//extern MDrvHDMITX_PARAMETER_LIST     gHDMITxInfo;
//extern MS_BOOL     g_bDisableRegWrite;
//extern MS_BOOL     g_bDisableTMDSCtrl;
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


//*********************//
//        DVI / HDMI   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine initial HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_Init_U2(void *pInstance)
{
    MS_BOOL bReturn;
    char word[] = {"_HDMITX_Mutex"};

    if (_s32HDMITX_Mutex == -1)
    {
        _s32HDMITX_Mutex = MsOS_CreateMutex(E_MSOS_FIFO, word, MSOS_PROCESS_SHARED);
        if (_s32HDMITX_Mutex == -1)
        {
            (printf("[HDMITX][%06d] create mutex fail\n", __LINE__));
            return FALSE;
        }
    }
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    _HDMITX_GET_VARIABLE();
    HDMITX_ENTRY();

    bReturn = MDrv_HDMITxHDCPTx_Init(pInstance); //MDrv_HDMITx_Init(pInstance);
    psHDMITXResPri->bInit = bReturn;


    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bReturn;
}

MS_BOOL MApi_HDMITx_Init(void)
{
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_Init UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }
#ifdef UTOPIA_20
    stHDMITx_Init HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;



    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_INIT, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_Init Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_Init_U2(pu32HDMITXInst);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine exits HDMI Tx Module
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_Exit_U2(void* pInstance)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    _HDMITX_GET_VARIABLE();
    HDMITX_ENTRY();

    MDrv_HDMITx_Power_OnOff(FALSE);
    psHDMITXResPri->bInit = FALSE;

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
    MsOS_DeleteMutex(_s32HDMITX_Mutex);
    _s32HDMITX_Mutex = -1;

    return TRUE;
}

MS_BOOL MApi_HDMITx_Exit(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_Exit UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_Exit HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_EXIT, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_Exit Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_Exit_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module
/// @param[in] state ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_TurnOnOff_U2(void *pInstance, MS_BOOL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    _HDMITX_GET_VARIABLE();
    HDMITX_ENTRY();

    MDrv_HDMITx_TurnOnOff(pInstance, state);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_TurnOnOff(MS_BOOL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_TurnOnOff UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_TurnOnOff HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_TURNONOFF, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_TurnOnOff Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_TurnOnOff_U2(pu32HDMITXInst, state);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine will set or stop all HDMI packet generation
/// @param[in] bflag  True: Enable packet gen, False : Disable packet gen
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_EnablePacketGen_U2(void* pInstance, MS_BOOL bflag)
{
    HDMITX_ENTRY();

    MDrv_HDMITx_EnablePacketGen(bflag);

    HDMITX_RETURN();
}

void MApi_HDMITx_EnablePacketGen(MS_BOOL bflag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_EnablePacketGen UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_EnablePacketGen HDMITXArgs;
    HDMITXArgs.bflag = bflag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_ENABLEPACKETGEN, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_EnablePacketGen Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_EnablePacketGen_U2(pu32HDMITXInst, bflag);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module output mode (DVI/HDMI)
/// @param[in] mode ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_SetHDMITxMode_U2(void* pInstance, HDMITX_OUTPUT_MODE mode)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    if (MDrv_HDMITx_GetOutputMode(pInstance) & 0x01U) // check HDCP
    {
        MDrv_HDMITx_SetHDCPOnOff(pInstance, TRUE);
    }
    else
    {
        MDrv_HDMITx_SetHDCPOnOff(pInstance, FALSE);
    }

    switch (mode)
    {
        case HDMITX_DVI:
            MDrv_HDMITx_SetHDMITxMode(pInstance, E_HDMITX_DVI);
        break;

        case HDMITX_DVI_HDCP:
            MDrv_HDMITx_SetHDMITxMode(pInstance, E_HDMITX_DVI_HDCP);
        break;

        case HDMITX_HDMI:
            MDrv_HDMITx_SetHDMITxMode(pInstance, E_HDMITX_HDMI);
        break;

        case HDMITX_HDMI_HDCP:
            MDrv_HDMITx_SetHDMITxMode(pInstance, E_HDMITX_HDMI_HDCP);
        break;

        default:
            printf("[HDMITx]: Error output mode: %d\n", mode);
        return;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetHDMITxMode(HDMITX_OUTPUT_MODE mode)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetHDMITxMode UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetHDMITxMode HDMITXArgs;
    HDMITXArgs.mode = mode;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETHDMITXMODE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetHDMITxMode Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetHDMITxMode_U2(pu32HDMITXInst,    mode);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module output mode (DVI/HDMI) and deep color depth
/// @param[in] mode ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_SetHDMITxMode_CD_U2(void* pInstance, HDMITX_OUTPUT_MODE mode, HDMITX_VIDEO_COLORDEPTH_VAL val)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MsHDMITX_OUTPUT_MODE enHDMIMode = E_HDMITX_HDMI;
    MsHDMITX_VIDEO_COLORDEPTH_VAL enDeepColorMode = E_HDMITX_VIDEO_CD_NoID;
    MS_BOOL bInvalidArg = FALSE;

    switch (mode)
    {
        case HDMITX_DVI:
            enHDMIMode = E_HDMITX_DVI;
        break;

        case HDMITX_DVI_HDCP:
            enHDMIMode = E_HDMITX_DVI_HDCP;
        break;

        case HDMITX_HDMI:
            enHDMIMode = E_HDMITX_HDMI;
        break;

        case HDMITX_HDMI_HDCP:
            enHDMIMode = E_HDMITX_HDMI_HDCP;
        break;

        default:
            printf("[HDMITx]: Error output mode: %d\n", mode);
            bInvalidArg = TRUE;
        break;
    }

    switch (val)
    {
        case HDMITX_VIDEO_CD_NoID:
            enDeepColorMode = E_HDMITX_VIDEO_CD_NoID;
        break;

        case HDMITX_VIDEO_CD_24Bits:
            enDeepColorMode = E_HDMITX_VIDEO_CD_24Bits;
        break;

        case HDMITX_VIDEO_CD_30Bits:
            enDeepColorMode = E_HDMITX_VIDEO_CD_30Bits;
        break;

        case HDMITX_VIDEO_CD_36Bits:
            enDeepColorMode = E_HDMITX_VIDEO_CD_36Bits;
        break;

        case HDMITX_VIDEO_CD_48Bits:
            enDeepColorMode = E_HDMITX_VIDEO_CD_48Bits;
        break;

        default:
            printf("[HDMITx]: Error deep color argument: %d\n", val);
            bInvalidArg = TRUE;
        break;
    }

    if (bInvalidArg == FALSE)
    {
        #if 0
        if ((MDrv_HDMITx_GetColorFormat(pInstance) != E_HDMITX_VIDEO_COLOR_YUV420) &&
            ((MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_1920x2205p_24Hz) ||
            (MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_1280x1470p_50Hz) ||
            (MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_1280x1470p_60Hz) ||
            (MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_3840x2160p_50Hz) ||
            (MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_3840x2160p_60Hz) ||
            (MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_4096x2160p_50Hz) ||
            (MDrv_HDMITx_GetOutputTiming(pInstance) == E_HDMITX_RES_4096x2160p_60Hz)))
                enDeepColorMode = E_HDMITX_VIDEO_CD_24Bits;
        #endif
        MDrv_HDMITx_SetDeepColorMode(pInstance, enDeepColorMode);
        MDrv_HDMITx_SetHDMITxMode(pInstance, enHDMIMode);
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    #if 0 //this will be done in MDrv_HDMITx_SetHDMITxMode()
    if (MDrv_HDMITx_GetOutputMode() & 0x01U) // check HDCP
    {
        MDrv_HDMITx_SetHDCPOnOff(TRUE);
    }
    else
    {
        MDrv_HDMITx_SetHDCPOnOff(FALSE);
    }
    #endif
}

void MApi_HDMITx_SetHDMITxMode_CD(HDMITX_OUTPUT_MODE mode, HDMITX_VIDEO_COLORDEPTH_VAL val)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetHDMITxMode_CD UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetHDMITxMode_CD HDMITXArgs;
    HDMITXArgs.mode = mode;
    HDMITXArgs.val = val;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETHDMITXMODE_CD, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetHDMITxMode_CD Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetHDMITxMode_CD_U2(pu32HDMITXInst, mode, val);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module TMDS signal On/Off
/// @param[in] state ON/OFF state
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_SetTMDSOnOff_U2(void* pInstance, MS_BOOL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    _HDMITX_GET_VARIABLE();
    HDMITX_ENTRY();

    MDrv_HDMITx_SetTMDSOnOff(pInstance, state);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetTMDSOnOff(MS_BOOL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetTMDSOnOff UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetTMDSOnOff HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETTMDSONOFF, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetTMDSOnOff Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetTMDSOnOff_U2(pu32HDMITXInst, state);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine disable HDMI Tx TMDS control
/// @param[in] bFlag Disable/Enable
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_DisableTMDSCtrl_U2(void* pInstance, MS_BOOL bFlag)
{
    MDrv_HDMITx_DisableTMDSCtrl(bFlag);
}

void MApi_HDMITx_DisableTMDSCtrl(MS_BOOL bFlag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_DisableTMDSCtrl UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_DisableTMDSCtrl HDMITXArgs;
    HDMITXArgs.bFlag = bFlag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_DISABLETMDSCTRL, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_DisableTMDSCtrl Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_DisableTMDSCtrl_U2(pu32HDMITXInst, bFlag);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine swap R and B channel color
/// @param[in] state ON/OFF state
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetRBChannelSwap_U2(void* pInstance, MS_BOOL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    _HDMITX_GET_VARIABLE();

    MDrv_HDMITx_SetRBChannelSwap(pInstance, state);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetRBChannelSwap(MS_BOOL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetRBChannelSwap UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetRBChannelSwap HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETRBCHANNELSWAP, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetRBChannelSwap Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetRBChannelSwap_U2(pu32HDMITXInst, state);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine doing HDMITx exhibition
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_Exhibit_U2(void* pInstance)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MDrv_HDMITx_Exhibit(pInstance);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_Exhibit(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_Exhibit UtopiaOpen HDMITX failed\n");
        }
    }

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_EXHIBIT, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_Exhibit Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_Exhibit_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting the connected HDMI Rx status
/// @return Ture: Rx is alive
///             FALSE: Rx is dead
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetRxStatus_U2(void)
{
    MS_BOOL bRet = FALSE;
    bRet = MDrv_HDMITx_GetRxStatus();

    return bRet;
}

MS_BOOL MApi_HDMITx_GetRxStatus(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetRxStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetRxStatus HDMITXArgs;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETRXSTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetRxStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetRxStatus_U2();
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's deep color definition from EDID
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetRxDCInfoFromEDID_U2(void* pInstance, HDMITX_VIDEO_COLORDEPTH_VAL *val)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        *val = (HDMITX_VIDEO_COLORDEPTH_VAL)MDrv_HDMITx_GetRxDCInfoFromEDID(pInstance);
        bRet = TRUE;
    }
    else
    {
        *val = (HDMITX_VIDEO_COLORDEPTH_VAL)HDMITX_VIDEO_CD_NoID;
        bRet = FALSE;
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;

}

MS_BOOL MApi_HDMITx_GetRxDCInfoFromEDID(HDMITX_VIDEO_COLORDEPTH_VAL *val)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetRxDCInfoFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetRxDCInfoFromEDID HDMITXArgs;
    HDMITXArgs.val = val;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETRXDCINFOFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetRxDCInfoFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetRxDCInfoFromEDID_U2(pu32HDMITXInst, val);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's support video format from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] u8BufSize buffer size
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetRxVideoFormatFromEDID_U2(void* pInstance, MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetRxVideoFormatFromEDID(pInstance, pu8Buffer, u8BufSize);
        bRet = TRUE;
    }
    else
    {
        memset(pu8Buffer, 0x00, u8BufSize);
        bRet = FALSE;
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetRxVideoFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetRxVideoFormatFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetRxVideoFormatFromEDID HDMITXArgs;
    HDMITXArgs.pu8Buffer = pu8Buffer;
    HDMITXArgs.u8BufSize = u8BufSize;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETRXVIDEODORMATFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetRxVideoFormatFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetRxVideoFormatFromEDID_U2(pu32HDMITXInst, pu8Buffer, u8BufSize);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's support VIC list from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] u8BufSize buffer size
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetVICListFromEDID_U2(void* pInstance, MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetVICListFromEDID(pInstance, pu8Buffer, u8BufSize);
        bRet = TRUE;
    }
    else
    {
        memset(pu8Buffer, 0x00, u8BufSize);
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetVICListFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetVICListFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetVICListFromEDID HDMITXArgs;
    HDMITXArgs.pu8Buffer = pu8Buffer;
    HDMITXArgs.u8BufSize = u8BufSize;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETVICLISTFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetVICListFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetVICListFromEDID_U2(pu32HDMITXInst, pu8Buffer, u8BufSize);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's data block length from EDID
/// @param[in] pu8Length pointer of buffer
/// @param[in] u8TagCode
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetDataBlockLengthFromEDID_U2(void* pInstance, MS_U8 *pu8Length, MS_U8 u8TagCode)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetDataBlockLengthFromEDID(pInstance, pu8Length, u8TagCode);
        bRet = TRUE;
    }
    else
    {
        *pu8Length = 0x00;
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetDataBlockLengthFromEDID(MS_U8 *pu8Length, MS_U8 u8TagCode)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetDataBlockLengthFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetDataBlockLengthFromEDID HDMITXArgs;
    HDMITXArgs.pu8Length = pu8Length;
    HDMITXArgs.u8TagCode = u8TagCode;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETDATABLOCKLENGTHFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetDataBlockLengthFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetDataBlockLengthFromEDID_U2(pu32HDMITXInst, pu8Length, u8TagCode);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's support audio format from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] u8BufSize buffer size
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetRxAudioFormatFromEDID_U2(void* pInstance, MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetRxAudioFormatFromEDID(pInstance, pu8Buffer, u8BufSize);
        bRet = TRUE;
    }
    else
    {
        memset(pu8Buffer, 0x00, u8BufSize);
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetRxAudioFormatFromEDID(MS_U8 *pu8Buffer, MS_U8 u8BufSize)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetRxAudioFormatFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetRxAudioFormatFromEDID HDMITXArgs;
    HDMITXArgs.pu8Buffer = pu8Buffer;
    HDMITXArgs.u8BufSize = u8BufSize;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETRXAUDIOFORMATFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetRxAudioFormatFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetRxAudioFormatFromEDID_U2(pu32HDMITXInst, pu8Buffer, u8BufSize);
#endif
}

MS_BOOL MApi_HDMITx_EDID_HDMISupport_U2(void* pInstance, MS_BOOL *HDMI_Support)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        *HDMI_Support = MDrv_HDMITx_EdidGetHDMISupportFlag(pInstance);
        _HDMITX_SEMAPHORE_RETURN(pInstance);
        return TRUE;
    }
    else
    {
        _HDMITX_SEMAPHORE_RETURN(pInstance);
        return FALSE;
    }
}

MS_BOOL MApi_HDMITx_EDID_HDMISupport(MS_BOOL *HDMI_Support)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_EDID_HDMISupport UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_EDID_HDMISupport HDMITXArgs;
    HDMITXArgs.HDMI_Support = HDMI_Support;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_EDID_HDMISUPPORT, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_EDID_HDMISupport Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_EDID_HDMISupport_U2(pu32HDMITXInst, HDMI_Support);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's ID Manu facturer Name from EDID
/// @param[in] pu8Buffer pointer of buffer
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetRxIDManufacturerName_U2(void* pInstance, MS_U8 *pu8Buffer)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetRxIDManufacturerName(pInstance, pu8Buffer);
        bRet = TRUE;
    }
    else
    {
        memset(pu8Buffer, 0x00, 0x03);
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetRxIDManufacturerName(MS_U8 *pu8Buffer)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetRxIDManufacturerName UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetRxIDManufacturerName HDMITXArgs;
    HDMITXArgs.pu8Buffer = pu8Buffer;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETRXIDMANUFACTURERNAME, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetRxIDManufacturerName Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetRxIDManufacturerName_U2(pu32HDMITXInst, pu8Buffer);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine getting Rx's EDID Data
/// @param[in] pu8Buffer pointer of buffer
/// @param[in] BlockIdx 0: for 0~127, 1: for 128~255
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetEDIDData_U2(void* pInstance, MS_U8 *pu8Buffer, MS_BOOL BlockIdx)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetEDIDData(pInstance, pu8Buffer, BlockIdx);
        bRet = TRUE;
    }
    else
    {
        memset(pu8Buffer, 0x00, 128*sizeof(MS_U8));
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetEDIDData(MS_U8 *pu8Buffer, MS_BOOL BlockIdx)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetEDIDData UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetEDIDData HDMITXArgs;
    HDMITXArgs.pu8Buffer = pu8Buffer;
    HDMITXArgs.BlockIdx = BlockIdx;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETEDIDDATA, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetEDIDData Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetEDIDData_U2(pu32HDMITXInst, pu8Buffer, BlockIdx);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine gets Rx's supported 3D structures of specific timing from EDID
/// @param[in] timing input video timing
/// @param[in] p3DStructure pointer of supported 3D structure
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetRx3DStructureFromEDID_U2(void* pInstance, HDMITX_VIDEO_TIMING timing, HDMITX_EDID_3D_STRUCTURE_ALL *p3DStructure)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetRx3DStructureFromEDID(pInstance, (MsHDMITX_VIDEO_TIMING)timing, (MsHDMITX_EDID_3D_STRUCTURE_ALL *)p3DStructure);
        bRet = TRUE;
    }
    else
    {
        *p3DStructure = 0x00;
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetRx3DStructureFromEDID(HDMITX_VIDEO_TIMING timing, HDMITX_EDID_3D_STRUCTURE_ALL *p3DStructure)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetRx3DStructureFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetRx3DStructureFromEDID HDMITXArgs;
    HDMITXArgs.timing = timing;
    HDMITXArgs.p3DStructure = p3DStructure;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETRX3DSTRUCTUREFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetRx3DStructureFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetRx3DStructureFromEDID_U2(pu32HDMITXInst, timing, p3DStructure);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine gets Rx's supported color of specific timing from EDID
/// @param[in] timing input video timing
/// @param[in] pColorFmt pointer of supported color format
/// @return Ture: EDID is ready
///             FALSE: EDID is not ready
//-----------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetColorFormatFromEDID_U2(void* pInstance, HDMITX_VIDEO_TIMING timing, HDMITX_EDID_COLOR_FORMAT *pColorFmt)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        MDrv_HDMITx_GetColorFormatFromEDID(pInstance, (MsHDMITX_VIDEO_TIMING)timing, (MsHDMITX_EDID_COLOR_FORMAT *)pColorFmt);
        bRet = TRUE;
    }
    else
    {
        *pColorFmt = HDMITX_EDID_Color_RGB_444;
        bRet = FALSE;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;

}

MS_BOOL MApi_HDMITx_GetColorFormatFromEDID(HDMITX_VIDEO_TIMING timing, HDMITX_EDID_COLOR_FORMAT *pColorFmt)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetColorFormatFromEDID UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetColorFormatFromEDID HDMITXArgs;
    HDMITXArgs.timing = timing;
    HDMITXArgs.pColorFmt = pColorFmt;
    HDMITXArgs.bReturn= FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETCOLORFORMATFROMEDID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetColorFormatFromEDID Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetColorFormatFromEDID_U2(pu32HDMITXInst, timing, pColorFmt);
#endif
}


//------------------------------------------------------------------------------
/// @brief This function clear settings of user defined packet
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_PKT_User_Define_Clear_U2(void* pInstance)
{
    MDrv_HDMITx_PKT_User_Define_Clear();
}

void MApi_HDMITx_PKT_User_Define_Clear(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_PKT_User_Define_Clear UtopiaOpen HDMITX failed\n");
        }
    }

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_PKT_USER_DEFINE_CLEAR, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_PKT_User_Define_Clear Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_PKT_User_Define_Clear_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This function set user defined hdmi packet
/// @param[in] packet_type: type of user defined packet
/// @param[in] def_flag: enable/disable user defined packet
/// @param[in] def_process: packet process of user defined packet
/// @param[in] def_fcnt: frame counts of user defined packet (0~31)
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_PKT_User_Define_U2(void* pInstance, HDMITX_PACKET_TYPE packet_type, MS_BOOL def_flag,
    HDMITX_PACKET_PROCESS def_process, MS_U8 def_fcnt)
{
    HDMITX_ENTRY();
    MDrv_HDMITx_PKT_User_Define((MsHDMITX_PACKET_TYPE)packet_type, def_flag, (MsHDMITX_PACKET_PROCESS)def_process, def_fcnt);
    HDMITX_RETURN();
}

void MApi_HDMITx_PKT_User_Define(HDMITX_PACKET_TYPE packet_type, MS_BOOL def_flag,
    HDMITX_PACKET_PROCESS def_process, MS_U8 def_fcnt)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_PKT_User_Define UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_PKT_User_Define HDMITXArgs;
    HDMITXArgs.packet_type = packet_type;
    HDMITXArgs.def_flag = def_flag;
    HDMITXArgs.def_process = def_process;
    HDMITXArgs.def_fcnt = def_fcnt;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_PKT_USER_DEFINE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_PKT_User_Define Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_PKT_User_Define_U2(pu32HDMITXInst, packet_type, def_flag, def_process, def_fcnt);
#endif
}

//------------------------------------------------------------------------------
/// @brief This function let user define packet's content
/// @param[in] packet_type: type of user defined packet
/// @param[in] data: content of user defined packet
/// @param[in] length: length of packet content
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_PKT_Content_Define_U2(void* pInstance, HDMITX_PACKET_TYPE packet_type, MS_U8 *data, MS_U8 length)
{
    MS_BOOL bRet = FALSE;

    HDMITX_ENTRY();
    bRet = MDrv_HDMITx_PKT_Content_Define((MsHDMITX_PACKET_TYPE)packet_type, data, length);
    HDMITX_RETURN();

    return bRet;
}

MS_BOOL MApi_HDMITx_PKT_Content_Define(HDMITX_PACKET_TYPE packet_type, MS_U8 *data, MS_U8 length)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_PKT_Content_Define UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_PKT_Content_Define HDMITXArgs;
    HDMITXArgs.packet_type = packet_type;
    HDMITXArgs.data = data;
    HDMITXArgs.length = length;
    HDMITXArgs.bReturn = FALSE;

    if(UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_PKT_CONTENT_DEFINE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_PKT_Content_Define Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_PKT_Content_Define_U2(pu32HDMITXInst, packet_type, data, length);
#endif
}

//*********************//
//             Video   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module video output
/// @param[in] state ON/OFF state
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetVideoOnOff_U2(void* pInstance, MS_BOOL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MDrv_HDMITx_SetVideoOnOff(pInstance, state);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetVideoOnOff(MS_BOOL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetVideoOnOff UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetVideoOnOff HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETVIDEOONOFF, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetVideoOnOff Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetVideoOnOff_U2(pu32HDMITXInst, state);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module video color format
/// @param[in] in_color input color format
/// @param[in] out_color output color format
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetColorFormat_U2(void* pInstance, HDMITX_VIDEO_COLOR_FORMAT in_color, HDMITX_VIDEO_COLOR_FORMAT out_color)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MS_BOOL bInvalidArg = FALSE;
    MsHDMITX_VIDEO_COLOR_FORMAT enInputColor = E_HDMITX_VIDEO_COLOR_RGB444;
    MsHDMITX_VIDEO_COLOR_FORMAT enOutputColor = E_HDMITX_VIDEO_COLOR_RGB444;

    //convert different enum define
    switch (in_color)
    {
        case HDMITX_VIDEO_COLOR_RGB444:
            enInputColor = E_HDMITX_VIDEO_COLOR_RGB444;
        break;

        case HDMITX_VIDEO_COLOR_YUV422:
            enInputColor = E_HDMITX_VIDEO_COLOR_YUV422;
        break;

        case HDMITX_VIDEO_COLOR_YUV444:
            enInputColor = E_HDMITX_VIDEO_COLOR_YUV444;
        break;

        case HDMITX_VIDEO_COLOR_YUV420:
            enInputColor = E_HDMITX_VIDEO_COLOR_YUV420;
        break;

        default:
            bInvalidArg = TRUE;
            printf("[HDMITx]: Error video color format(in): %d\n", in_color);
        break;
    }

    switch (out_color)
    {
        case HDMITX_VIDEO_COLOR_RGB444:
            enOutputColor = E_HDMITX_VIDEO_COLOR_RGB444;
        break;

        case HDMITX_VIDEO_COLOR_YUV422:
            enOutputColor = E_HDMITX_VIDEO_COLOR_YUV422;
        break;

        case HDMITX_VIDEO_COLOR_YUV444:
            enOutputColor = E_HDMITX_VIDEO_COLOR_YUV444;
        break;

        case HDMITX_VIDEO_COLOR_YUV420:
            enOutputColor = E_HDMITX_VIDEO_COLOR_YUV420;
        break;

        default:
            bInvalidArg = TRUE;
            printf("[HDMITx]: Error video color format(out): %d\n", out_color);
        break;
    }

    if (bInvalidArg == FALSE)
        MDrv_HDMITx_SetColorFormat(pInstance, enInputColor, enOutputColor);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_FORMAT in_color, HDMITX_VIDEO_COLOR_FORMAT out_color)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetColorFormat UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetColorFormat HDMITXArgs;
    HDMITXArgs.in_color = in_color;
    HDMITXArgs.out_color = out_color;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETCOLORFORMAT, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetColorFormat Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetColorFormat_U2(pu32HDMITXInst, in_color, out_color);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx vs packet content
/// @param[in] in_color input color format
/// @param[in] out_color output color format
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_Set_VS_InfoFrame_U2(void* pInstance, HDMITX_VIDEO_VS_FORMAT vs_format, HDMITX_VIDEO_3D_STRUCTURE vs_3d, HDMITX_VIDEO_4k2k_VIC vs_vic)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MsHDMITX_VIDEO_VS_FORMAT enVSFmt = E_HDMITX_VIDEO_VS_No_Addition;
    MsHDMITX_VIDEO_4k2k_VIC enVid4K2KVic = E_HDMITx_VIDEO_4k2k_Reserved;
    MsHDMITX_VIDEO_3D_STRUCTURE enVid3DStruct = E_HDMITx_VIDEO_3D_Not_in_Use;

    //convert enum type
    switch (vs_format)
    {
        case HDMITX_VIDEO_VS_4k_2k:
            {
                enVSFmt = E_HDMITX_VIDEO_VS_4k_2k;

                switch (vs_vic)
                {
                    case HDMITX_VIDEO_4k2k_30Hz:
                        enVid4K2KVic = E_HDMITX_VIDEO_4k2k_30Hz;
                    break;

                    case HDMITX_VIDEO_4k2k_25Hz:
                        enVid4K2KVic = E_HDMITX_VIDEO_4k2k_25Hz;
                    break;

                    case HDMITX_VIDEO_4k2k_24Hz:
                        enVid4K2KVic = E_HDMITX_VIDEO_4k2k_24Hz;
                    break;

                    case HDMITx_VIDEO_4k2k_24Hz_SMPTE:
                        enVid4K2KVic = E_HDMITx_VIDEO_4k2k_24Hz_SMPTE;
                    break;

                    case HDMITx_VIDEO_4k2k_Reserved:
                        enVid4K2KVic = E_HDMITX_VIDEO_4k2k_30Hz;
                    break;

                    default:
                        enVid4K2KVic = E_HDMITx_VIDEO_4k2k_Reserved;
                    break;
                }
            }
            break;

        case HDMITX_VIDEO_VS_3D:
            enVSFmt = E_HDMITX_VIDEO_VS_3D;
            switch (vs_3d)
            {
                case HDMITX_VIDEO_3D_FramePacking: //0000
                    enVid3DStruct = E_HDMITX_VIDEO_3D_FramePacking;
                break;

                case HDMITX_VIDEO_3D_FieldAlternative: //0001
                    enVid3DStruct = E_HDMITX_VIDEO_3D_FieldAlternative;
                break;

                case HDMITX_VIDEO_3D_LineAlternative: //0010
                    enVid3DStruct = E_HDMITX_VIDEO_3D_LineAlternative;
                break;

                case HDMITX_VIDEO_3D_SidebySide_FULL: //0011
                    enVid3DStruct = E_HDMITX_VIDEO_3D_SidebySide_FULL;
                break;

                case HDMITX_VIDEO_3D_L_Dep: //0100
                    enVid3DStruct = E_HDMITX_VIDEO_3D_L_Dep;
                break;

                case HDMITX_VIDEO_3D_L_Dep_Graphic_Dep: //0101
                    enVid3DStruct = E_HDMITX_VIDEO_3D_L_Dep_Graphic_Dep;
                break;

                case HDMITX_VIDEO_3D_TopandBottom: //0110
                    enVid3DStruct = E_HDMITX_VIDEO_3D_TopandBottom;
                break;

                case HDMITX_VIDEO_3D_SidebySide_Half: //1000
                    enVid3DStruct = E_HDMITX_VIDEO_3D_SidebySide_Half;
                break;

                default: //1111
                    enVid3DStruct = E_HDMITx_VIDEO_3D_Not_in_Use;
                break;
            }
            break;

            default:
                enVSFmt = E_HDMITX_VIDEO_VS_No_Addition;
            break;
    }

    MDrv_HDMITx_Set_VS_InfoFrame(pInstance, enVSFmt, enVid4K2KVic, enVid3DStruct);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_Set_VS_InfoFrame(HDMITX_VIDEO_VS_FORMAT vs_format, HDMITX_VIDEO_3D_STRUCTURE vs_3d, HDMITX_VIDEO_4k2k_VIC vs_vic)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_Set_VS_InfoFrame UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_Set_VS_InfoFrame HDMITXArgs;
    HDMITXArgs.vs_format = vs_format;
    HDMITXArgs.vs_3d = vs_3d;
    HDMITXArgs.vs_vic = vs_vic;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SET_VS_INFOFRAME, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_Set_VS_InfoFrame Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_Set_VS_InfoFrame_U2(pu32HDMITXInst, vs_format, vs_3d, vs_vic);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module video output timing
/// @param[in] mode output video timing
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetVideoOutputTiming_U2(void* pInstance, HDMITX_VIDEO_TIMING mode)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

#ifdef URANUS
    if ((MS_U32)mode > (MS_U32)HDMITX_RES_720x576p)
    {
        printf("[%s][%d] Unsupport HDMI mode 0x%08x\n", __FUNCTION__, __LINE__, mode);
        return;
    }
#endif
#ifdef OBERON
    if ((MS_U32)mode >= (MS_U32)HDMITX_RES_MAX)
    {
        printf("[%s][%d] Unsupport HDMI mode 0x%08x\n", __FUNCTION__, __LINE__, mode);
        return;
    }
#endif

    // 3D frame packing only support up to 24-bit color depth
    // For HDMI 2.0 spec, 4k2k@60 only support up to 24-bit color depth
    #if 0
    if ((MDrv_HDMITx_GetColorFormat(pInstance) != E_HDMITX_VIDEO_COLOR_YUV420) &&
        ((mode == HDMITX_RES_1920x2205p_24Hz) ||
        (mode == HDMITX_RES_1280X1470p_50Hz) ||
        (mode == HDMITX_RES_1280X1470p_60Hz) ||
        (mode == HDMITX_RES_3840x2160p_50Hz) ||
        (mode == HDMITX_RES_3840x2160p_60Hz) ||
        (mode == HDMITX_RES_4096x2160p_50Hz) ||
        (mode == HDMITX_RES_4096x2160p_60Hz)))
    {
        if (MDrv_HDMITx_GetDeepColorMode(pInstance) >= E_HDMITX_VIDEO_CD_30Bits)
        {
            MDrv_HDMITx_SetDeepColorMode(pInstance, E_HDMITX_VIDEO_CD_24Bits);
            MDrv_HDMITx_SetHDMITxMode(pInstance, MDrv_HDMITx_GetOutputMode(pInstance));
        }
    }
    #endif

    switch (mode)
    {
        case HDMITX_RES_640x480p:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_640x480p);
        break;

        case HDMITX_RES_720x480i:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_720x480i);
        break;

        case HDMITX_RES_720x576i:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_720x576i);
        break;

        case HDMITX_RES_720x480p:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_720x480p);
        break;

        case HDMITX_RES_720x576p:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_720x576p);
        break;

        case HDMITX_RES_1280x720p_50Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1280x720p_50Hz);
        break;

        case HDMITX_RES_1280x720p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1280x720p_60Hz);
        break;

        case HDMITX_RES_1920x1080i_50Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080i_50Hz);
        break;

        case HDMITX_RES_1920x1080i_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080i_60Hz);
        break;

        case HDMITX_RES_1920x1080p_24Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080p_24Hz);
        break;

        case HDMITX_RES_1920x1080p_25Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080p_25Hz);
        break;

        case HDMITX_RES_1920x1080p_30Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080p_30Hz);
        break;

        case HDMITX_RES_1920x1080p_50Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080p_50Hz);
        break;

        case HDMITX_RES_1920x1080p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x1080p_60Hz);
        break;

        case HDMITX_RES_1920x2205p_24Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1920x2205p_24Hz);
        break;

        case HDMITX_RES_1280X1470p_50Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1280x1470p_50Hz);
        break;

        case HDMITX_RES_1280X1470p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1280x1470p_60Hz);
        break;

        case HDMITX_RES_3840x2160p_24Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_3840x2160p_24Hz);
        break;

        case HDMITX_RES_3840x2160p_25Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_3840x2160p_25Hz);
        break;

        case HDMITX_RES_3840x2160p_30Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_3840x2160p_30Hz);
        break;

        case HDMITX_RES_3840x2160p_50Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_3840x2160p_50Hz);
        break;

        case HDMITX_RES_3840x2160p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_3840x2160p_60Hz);
        break;

        case HDMITX_RES_4096x2160p_24Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_4096x2160p_24Hz);
        break;

        case HDMITX_RES_4096x2160p_25Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_4096x2160p_25Hz);
        break;

        case HDMITX_RES_4096x2160p_30Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_4096x2160p_30Hz);
        break;

        case HDMITX_RES_4096x2160p_50Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_4096x2160p_50Hz);
        break;

        case HDMITX_RES_4096x2160p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_4096x2160p_60Hz);
        break;

        case HDMITX_RES_1600x1200p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1600x1200p_60Hz);
        break;

        case HDMITX_RES_1440x900p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1440x900p_60Hz);
        break;

        case HDMITX_RES_1280x1024p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1280x1024p_60Hz);
        break;

        case HDMITX_RES_1024x768p_60Hz:
            MDrv_HDMITx_SetVideoOutputMode(pInstance, E_HDMITX_RES_1024x768p_60Hz);
        break;

        default:
            printf("[HDMITx]: Error video timing: %d\n", mode);
        break;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetVideoOutputTiming(HDMITX_VIDEO_TIMING mode)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetVideoOutputTiming UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetVideoOutputTiming HDMITXArgs;
    HDMITXArgs.mode = mode;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETVIDEOOUTPUTTIMING, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetVideoOutputTiming Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetVideoOutputTiming_U2(pu32HDMITXInst, mode);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx video output aspect ratio
/// @param[in] out_ar output video aspect ratio
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetVideoOutputAsepctRatio_U2(void* pInstance, HDMITX_VIDEO_ASPECT_RATIO out_ar)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    switch (out_ar)
    {
        case HDMITX_VIDEO_AR_Reserved:
            MDrv_HDMITx_SetVideoOutputAsepctRatio(pInstance, E_HDMITX_VIDEO_AR_Reserved);
        break;

        case HDMITX_VIDEO_AR_4_3:
            MDrv_HDMITx_SetVideoOutputAsepctRatio(pInstance, E_HDMITX_VIDEO_AR_4_3);
        break;

        case HDMITX_VIDEO_AR_16_9:
            MDrv_HDMITx_SetVideoOutputAsepctRatio(pInstance, E_HDMITX_VIDEO_AR_16_9);
        break;

        case HDMITX_VIDEO_AR_21_9:
            MDrv_HDMITx_SetVideoOutputAsepctRatio(pInstance, E_HDMITX_VIDEO_AR_21_9);
        break;

        default:
            printf("[HDMITx]: Error aspect ratio: %d\n", out_ar);
        break;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetVideoOutputAsepctRatio(HDMITX_VIDEO_ASPECT_RATIO out_ar)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetVideoOutputAsepctRatio UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetVideoOutputAsepctRatio HDMITXArgs;
    HDMITXArgs.out_ar = out_ar;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETVIDEOOUTPUTASEPCTRATIO, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetVideoOutputAsepctRatio Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetVideoOutputAsepctRatio_U2(pu32HDMITXInst, out_ar);
#endif
}

// HDMI Tx video output Overscan and AFD ratio
void MApi_HDMITx_SetVideoOutputOverscan_AFD_U2(void* pInstance, MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    switch (out_scaninfo)
    {
        case HDMITX_VIDEO_SI_NoData:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, 0x00, bflag, E_HDMITX_VIDEO_SI_NoData, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        case HDMITX_VIDEO_SI_Overscanned:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, 0x00, bflag, E_HDMITX_VIDEO_SI_Overscanned, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        case HDMITX_VIDEO_SI_Underscanned:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, 0x00, bflag, E_HDMITX_VIDEO_SI_Underscanned, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        case HDMITX_VIDEO_SI_Reserved:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, 0x00, bflag, E_HDMITX_VIDEO_SI_Reserved, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        default:
            printf("[HDMITx]: Error scan information: %d\n", out_scaninfo);
        break;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetVideoOutputOverscan_AFD(MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetVideoOutputOverscan_AFD UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetVideoOutputOverscan_AFD HDMITXArgs;
    HDMITXArgs.bflag = bflag;
    HDMITXArgs.out_scaninfo = out_scaninfo;
    HDMITXArgs.out_afd = out_afd;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetVideoOutputOverscan_AFD Obtain HDMITX engine fail\n");
        //return FALSE;
    }
    else
    {
        //return HDMITXArgs.bReturn;
    }
#else
    MApi_HDMITx_SetVideoOutputOverscan_AFD_U2(pu32HDMITXInst, bflag, out_scaninfo, out_afd);
#endif
}

//*********************//
//             Audio   //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module audio output
/// @param[in] state output video timing
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetAudioOnOff_U2(void* pInstance, MS_BOOL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MDrv_HDMITx_SetAudioOnOff(pInstance, state);

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetAudioOnOff(MS_BOOL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAudioOnOff UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetAudioOnOff HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAUDIOONOFF, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAudioOnOff Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetAudioOnOff_U2(pu32HDMITXInst, state);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module audio output sampling frequency and channel count
/// @param[in] freq audio frequency
/// @return None
//   For Uranus
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetAudioFrequency_U2(void* pInstance, HDMITX_AUDIO_FREQUENCY freq)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    switch (freq)
    {
        case HDMITX_AUDIO_FREQ_NO_SIG:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_FREQ_NO_SIG);
        break;

        case HDMITX_AUDIO_32K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_32K);
        break;

        case HDMITX_AUDIO_44K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_44K);
        break;

        case HDMITX_AUDIO_48K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_48K);
        break;

        case HDMITX_AUDIO_88K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_88K);
        break;

        case HDMITX_AUDIO_96K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_96K);
        break;

        case HDMITX_AUDIO_176K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_176K);
        break;

        case HDMITX_AUDIO_192K:
            MDrv_HDMITx_SetAudioFrequency(pInstance, E_HDMITX_AUDIO_192K);
        break;

        default:
            printf("[HDMITx]: Error audio frequency: %d\n", freq);
        break;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);

}

void MApi_HDMITx_SetAudioFrequency(HDMITX_AUDIO_FREQUENCY freq)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAudioFrequency UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetAudioFrequency HDMITXArgs;
    HDMITXArgs.freq = freq;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAUDIOFREQUENCY, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAudioFrequency Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetAudioFrequency_U2(pu32HDMITXInst, freq);
#endif
}

void MApi_HDMITx_SetVideoOutputOverscan_AFD_II_U02(void* pInstance, MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd, MS_U8 A0)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    switch (out_scaninfo)
    {
        case HDMITX_VIDEO_SI_NoData:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, A0, bflag, E_HDMITX_VIDEO_SI_NoData, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        case HDMITX_VIDEO_SI_Overscanned:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, A0, bflag, E_HDMITX_VIDEO_SI_Overscanned, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        case HDMITX_VIDEO_SI_Underscanned:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, A0, bflag, E_HDMITX_VIDEO_SI_Underscanned, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        case HDMITX_VIDEO_SI_Reserved:
            MDrv_HDMITx_SetVideoOutputOverscan_AFD(pInstance, A0, bflag, E_HDMITX_VIDEO_SI_Reserved, (MsHDMITX_VIDEO_AFD_RATIO)out_afd);
        break;

        default:
            printf("[HDMITx]: Error scan information: %d\n", out_scaninfo);
        break;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);

}

void MApi_HDMITx_SetVideoOutputOverscan_AFD_II(MS_BOOL bflag, HDMITX_VIDEO_SCAN_INFO out_scaninfo, MS_U8 out_afd, MS_U8 A0 )
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetVideoOutputOverscan_AFD_II UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetVideoOutputOverscan_AFD_II HDMITXArgs;
    HDMITXArgs.bflag = bflag;
    HDMITXArgs.out_scaninfo = out_scaninfo;
    HDMITXArgs.out_afd = out_afd;
    HDMITXArgs.A0 = A0;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETVIDEOOUTPUTOVERSCAN_AFD_II, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetVideoOutputOverscan_AFD_II Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetVideoOutputOverscan_AFD_II_U02(pu32HDMITXInst, bflag, out_scaninfo, out_afd, A0);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module audio output: sampling frequency, channel count and coding type
/// @param[in]  freq audio frequency
/// @param[in]  ch audio channel type
/// @param[in]  type audio coding type
/// @return None
//   For Oberon
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetAudioConfiguration_U2(void* pInstance, HDMITX_AUDIO_FREQUENCY freq, HDMITX_AUDIO_CHANNEL_COUNT ch, HDMITX_AUDIO_CODING_TYPE type)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MsHDMITX_AUDIO_FREQUENCY enAudFreq = E_HDMITX_AUDIO_FREQ_NO_SIG;
    MsHDMITX_AUDIO_CHANNEL_COUNT enChCnt = E_HDMITX_AUDIO_CH_2;
    MsHDMITX_AUDIO_CODING_TYPE enAudFmt = E_HDMITX_AUDIO_PCM;
    MS_BOOL bInvalidArg = FALSE;

    //convert enum value
    switch (freq)
    {
        case HDMITX_AUDIO_FREQ_NO_SIG:
            enAudFreq = E_HDMITX_AUDIO_FREQ_NO_SIG;
        break;

        case HDMITX_AUDIO_32K:
            enAudFreq = E_HDMITX_AUDIO_32K;
        break;

        case HDMITX_AUDIO_44K:
            enAudFreq = E_HDMITX_AUDIO_44K;
        break;

        case HDMITX_AUDIO_48K:
            enAudFreq = E_HDMITX_AUDIO_48K;
        break;

        case HDMITX_AUDIO_88K:
            enAudFreq = E_HDMITX_AUDIO_88K;
        break;

        case HDMITX_AUDIO_96K:
            enAudFreq = E_HDMITX_AUDIO_96K;
        break;

        case HDMITX_AUDIO_176K:
            enAudFreq = E_HDMITX_AUDIO_176K;
        break;

        case HDMITX_AUDIO_192K:
            enAudFreq = E_HDMITX_AUDIO_192K;
        break;

        default:
            printf("[HDMITx]: Error audio frequency: %d\n", freq);
            bInvalidArg = TRUE;
        break;
    }

    switch (ch)
    {
        case HDMITX_AUDIO_CH_2:
            enChCnt = E_HDMITX_AUDIO_CH_2;
        break;

        case HDMITX_AUDIO_CH_8:
            enChCnt = E_HDMITX_AUDIO_CH_8;
        break;

        default:
            printf("[HDMITx]: Error audio channel: %d\n", ch);
            bInvalidArg = TRUE;
        break;
    }

    switch (type)
    {
        case HDMITX_AUDIO_PCM:
            enAudFmt = E_HDMITX_AUDIO_PCM;
        break;

        case HDMITX_AUDIO_NONPCM:
            enAudFmt = E_HDMITX_AUDIO_NONPCM;
        break;

        default:
            printf("[HDMITx]: Error audio type: %d\n", type);
            bInvalidArg = TRUE;
        break;
    }

    if (bInvalidArg == FALSE)
    {
        MDrv_HDMITx_SetAudioChCnt(pInstance, enChCnt);
        MDrv_HDMITx_SetAudioFmt(pInstance, enAudFmt);
        MDrv_HDMITx_SetAudioFrequency(pInstance, enAudFreq);
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetAudioConfiguration(HDMITX_AUDIO_FREQUENCY freq, HDMITX_AUDIO_CHANNEL_COUNT ch, HDMITX_AUDIO_CODING_TYPE type)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAudioConfiguration UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetAudioConfiguration HDMITXArgs;
    HDMITXArgs.freq = freq;
    HDMITXArgs.ch = ch;
    HDMITXArgs.type = type;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAUDIOCONFIGURATION, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAudioConfiguration Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetAudioConfiguration_U2(pu32HDMITXInst, freq, ch, type);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx audio mode
/// @param[in]  fmt : audio source format select
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetAudioSourceFormat_U2(void* pInstance, HDMITX_AUDIO_SOURCE_FORMAT fmt)
{
    HDMITX_ENTRY();

    MDrv_HDMITx_SetAudioSourceFormat((MsHDMITX_AUDIO_SOURCE_FORMAT) fmt);

    HDMITX_RETURN();
}

void MApi_HDMITx_SetAudioSourceFormat(HDMITX_AUDIO_SOURCE_FORMAT fmt)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAudioSourceFormat UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetAudioSourceFormat HDMITXArgs;
    HDMITXArgs.fmt = fmt;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAUDIOSOURCEFORMAT, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAudioSourceFormat Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetAudioSourceFormat_U2(pu32HDMITXInst, fmt);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine Set extended colorimetry field of AVIInfoframe
/// @return CTS
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_SetAVIInfoExtColorimetry_U2(void* pInstance, HDMITX_AVI_EXTENDED_COLORIMETRY enExtColorimetry, HDMITX_AVI_YCC_QUANT_RANGE enYccQuantRange)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    MsHDMITX_EXT_COLORIMETRY extColorimetry = E_HDMITX_EXT_COLORIMETRY_XVYCC601;
    MsHDMITX_YCC_QUANT_RANGE QuantRange = E_HDMITX_YCC_QUANT_LIMIT;
    MS_BOOL bInvalidArg = FALSE;

    switch (enExtColorimetry)
    {
        case HDMITX_EXT_COLORIMETRY_XVYCC601:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_XVYCC601;
        break;

        case HDMITX_EXT_COLORIMETRY_XVYCC709:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_XVYCC709;
        break;

        case HDMITX_EXT_COLORIMETRY_SYCC601:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_SYCC601;
        break;

        case HDMITX_EXT_COLORIMETRY_ADOBEYCC601:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_ADOBEYCC601;
        break;

        case HDMITX_EXT_COLORIMETRY_ADOBERGB:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_ADOBERGB;
        break;

        case HDMITX_EXT_COLORIMETRY_BT2020CYCC:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_BT2020CYCC;
        break;

        case HDMITX_EXT_COLORIMETRY_BT2020YCC:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_BT2020YCC;
        break;

        case HDMITX_EXT_COLORIMETRY_BT2020RGB:
            extColorimetry = E_HDMITX_EXT_COLORIMETRY_BT2020RGB;
        break;

        default:
            bInvalidArg = TRUE;
        break;
    }

    switch(enYccQuantRange)
    {
        case HDMITX_YCC_QUANT_LIMIT:
            QuantRange = E_HDMITX_YCC_QUANT_LIMIT;
        break;

        case HDMITX_YCC_QUANT_FULL:
            QuantRange = E_HDMITX_YCC_QUANT_FULL;
        break;

        case HDMITX_YCC_QUANT_RESERVED:
            QuantRange = E_HDMITX_YCC_QUANT_RESERVED;
        break;

        default:
            bInvalidArg = TRUE;
        break;
    }

    if (bInvalidArg == FALSE)
    {
        MDrv_HDMITx_SetAVIInfoExtColorimetry(pInstance, extColorimetry, QuantRange);
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_SetAVIInfoExtColorimetry(HDMITX_AVI_EXTENDED_COLORIMETRY enExtColorimetry, HDMITX_AVI_YCC_QUANT_RANGE enYccQuantRange)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAVIInfoExtColorimetry UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetAVIInfoExtColorimetry HDMITXArgs;
    HDMITXArgs.enExtColorimetry = enExtColorimetry;
    HDMITXArgs.enYccQuantRange = enYccQuantRange;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAVIINFOEXTCOLORIMETRY, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAVIInfoExtColorimetry Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_SetAVIInfoExtColorimetry_U2(pu32HDMITXInst, enExtColorimetry, enYccQuantRange);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine Get Audio CTS value
/// @return CTS
//------------------------------------------------------------------------------
MS_U32 MApi_HDMITx_GetAudioCTS_U2(void* pInstance)
{
    MS_U32 u32Return;

    HDMITX_ENTRY();

    u32Return = MDrv_HDMITx_GetAudioCTS();

    HDMITX_RETURN();

    return u32Return;
}

MS_U32 MApi_HDMITx_GetAudioCTS(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetAudioCTS UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetAudioCTS HDMITXArgs;
    HDMITXArgs.u32Return = 0;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETAUDIOCTS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetAudioCTS Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.u32Return;
    }
#else
    return MApi_HDMITx_GetAudioCTS_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine Mute Audio FIFO
/// @param[in] bflag: True: mute audio, False: unmute audio
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_MuteAudioFIFO_U2(void* pInstance, MS_BOOL bflag)
{
    HDMITX_ENTRY();

    MDrv_HDMITx_MuteAudioFIFO(bflag);

    HDMITX_RETURN();
}

void MApi_HDMITx_MuteAudioFIFO(MS_BOOL bflag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_MuteAudioFIFO UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_MuteAudioFIFO HDMITXArgs;
    HDMITXArgs.bflag = bflag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_MUTEAUDIOFIFO, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_MuteAudioFIFO Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_MuteAudioFIFO_U2(pu32HDMITXInst, bflag);
#endif
}


//*********************//
//             HDCP    //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine get HDCP key
/// @param[in] useinternalkey: TRUE -> from internal, FALSE -> from external, like SPI flash
/// @param[in] data: data point
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_GetHdcpKey_U2(void* pInstance, MS_BOOL useinternalkey, MS_U8 *data)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDCPTx_GetHdcpKey(pInstance, useinternalkey, data);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_GetHdcpKey(MS_BOOL useinternalkey, MS_U8 *data)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetHdcpKey UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_GetHdcpKey HDMITXArgs;
    HDMITXArgs.useinternalkey = useinternalkey;
    HDMITXArgs.data = data;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETHDCPKEY, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetHdcpKey Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_GetHdcpKey_U2(pu32HDMITXInst, useinternalkey, data);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine get HDCP bksv
/// @param[in] data: data point
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetBksv_U2(void* pInstance, MS_U8 *pdata)
{
    MS_BOOL bRet;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    bRet = MDrv_HDCPTx_GetBksv(pInstance, pdata); //wilson@kano

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetBksv(MS_U8 *pdata)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetBksv UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetBksv HDMITXArgs;
    HDMITXArgs.pdata = pdata;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETBKSV, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetBksv Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetBksv_U2(pu32HDMITXInst, pdata);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine get HDCP aksv
/// @param[in] data: data point
/// @return None
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetAksv_U2(void* pInstance, MS_U8 *pdata)
{
    MS_BOOL bRet;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    bRet = MDrv_HDCPTx_GetAksv(pInstance, pdata); //wilson@kano

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetAksv(MS_U8 *pdata)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetAksv UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetBksv HDMITXArgs;
    HDMITXArgs.pdata = pdata;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETBKSV, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetAksv Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetAksv_U2(pu32HDMITXInst, pdata);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx Module HDCP encryption
/// @param[in] state ON/OFF state
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetHDCPOnOff_U2(void* pInstance, MS_BOOL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDMITx_SetHDCPOnOff(pInstance, state);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetHDCPOnOff(MS_BOOL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetHDCPOnOff UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetHDCPOnOff HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETHDCPONOFF, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetHDCPOnOff Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetHDCPOnOff_U2(pu32HDMITXInst, state);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine set HDMI Tx AVMUTE
/// @argument:
///              - bflag: True=> SET_AVMUTE, FALSE=>CLEAR_AVMUTE
///
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_SetAVMUTE_U2(void* pInstance, MS_BOOL bflag)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDMITx_SetAVMUTE(pInstance, bflag);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetAVMUTE(MS_BOOL bflag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAVMUTE UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetAVMUTE HDMITXArgs;
    HDMITXArgs.bflag = bflag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAVMUTE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAVMUTE Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetAVMUTE_U2(pu32HDMITXInst, bflag);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine get HDMI Tx AVMUTE status
/// @argument:
///
/// @return:
///              - True=> SET_AVMUTE, FALSE=>CLEAR_AVMUTE
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetAVMUTEStatus_U2(void* pInstance)
{
    MS_BOOL bRet;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    bRet = MDrv_HDMITx_GetAVMUTEStatus(pInstance); //wilson@kano

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetAVMUTEStatus(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetAVMUTEStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetAVMUTEStatus HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETAVMUTESTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetAVMUTEStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetAVMUTEStatus_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine update Revocation key list
/// @argument:
///              - *data: data point
///              -  size: amount of revoked key ( size 1 = 1*5 bytes, size 2 = 2*5 bytes)
/// @return
//------------------------------------------------------------------------------
void MApi_HDMITx_HDCP_RevocationKey_List_U2(void* pInstance, MS_U8 *data, MS_U16 size)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDCPTx_GetRevocationKeyList(pInstance, data, size);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}


void MApi_HDMITx_HDCP_RevocationKey_List(MS_U8 *data, MS_U16 size)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP_RevocationKey_List UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_HDCP_RevocationKey_List HDMITXArgs;
    HDMITXArgs.data = data;
    HDMITXArgs.size = size;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP_REVOCATIONKEY_LIST, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP_RevocationKey_List Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_HDCP_RevocationKey_List_U2(pu32HDMITXInst, data, size);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine return revocation key check state
/// @argument:
/// @return:
//------------------------------------------------------------------------------
HDMITX_REVOCATION_STATE MApi_HDMITx_HDCP_RevocationKey_Check_U2(void* pInstance)
{
    return (HDMITX_REVOCATION_STATE)MDrv_HDCPTx_RevocationKey_Check(pInstance);
}

HDMITX_REVOCATION_STATE MApi_HDMITx_HDCP_RevocationKey_Check(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP_RevocationKey_Check UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_HDCP_RevocationKey_Check HDMITXArgs;
    HDMITXArgs.stReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP_REVOCATIONKEY_CHECK, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP_RevocationKey_Check Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.stReturn;
    }
#else
    return MApi_HDMITx_HDCP_RevocationKey_Check_U2(pu32HDMITXInst);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine check whether SRM DSA signauter is valid or not
/// @argument:
///              - *data: data point
///              -   size: size of SRM list(bytes)
/// @return:
///              Ture: valid, FALSE: invalid
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_HDCP_IsSRMSignatureValid_U2(void* pInstance, MS_U8 *data, MS_U32 size)
{
    return MDrv_HDCPTx_IsSRMSignatureValid(data, size);
}

MS_BOOL MApi_HDMITx_HDCP_IsSRMSignatureValid(MS_U8 *data, MS_U32 size)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP_IsSRMSignatureValid UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_HDCP_IsSRMSignatureValid HDMITXArgs;
    HDMITXArgs.data = data;
    HDMITXArgs.size = size;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP_ISSRMSIGNATUREVALID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP_IsSRMSignatureValid Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_HDCP_IsSRMSignatureValid_U2(pu32HDMITXInst, data, size);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine get HDCP authentication status
/// @argument:
/// @return:
///                  E_HDCP_DISABLE      = 0, // HDCP disable
///                  E_HDCP_FAIL = 1, // HDCP fail
///                  E_HDCP_PASS = 2, // HDCP pass
//------------------------------------------------------------------------------
HDMITX_HDCP_STATUS MApi_HDMITx_GetHDCPStatus_U2(void* pInstance)
{
    HDMITX_HDCP_STATUS RetVal;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetHdcpEnFlag(pInstance) == FALSE)
    {
        RetVal = E_HDCP_DISABLE;
    }
    else
    {
        if (MDrv_HDCPTx_GetAuthDoneFlag(pInstance) == TRUE)
        {
            RetVal = E_HDCP_PASS;
        }
        else
        {
            RetVal = E_HDCP_FAIL;
        }
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return RetVal;
}

HDMITX_HDCP_STATUS MApi_HDMITx_GetHDCPStatus(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetHDCPStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetHDCPStatus HDMITXArgs;
    HDMITXArgs.stReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETHDCPSTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetHDCPStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.stReturn;
    }
#else
    return MApi_HDMITx_GetHDCPStatus_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine will start/stop HDCP authentication
/// @argument: bFlag : TURE for start Authentication, FALSE for stop Authentication
/// @return:
//------------------------------------------------------------------------------
void MApi_HDMITx_HDCP_StartAuth_U2(void* pInstance, MS_BOOL bFlag)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDCPTx_StartAuth(pInstance, bFlag);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_HDCP_StartAuth(MS_BOOL bFlag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP_StartAuth UtopiaOpen HDMITX failed\n");
            //return FALSE;
        }
    }

    stHDMITx_HDCP_StartAuth HDMITXArgs;
    HDMITXArgs.bFlag = bFlag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP_STARTAUTH, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP_StartAuth Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    return MApi_HDMITx_HDCP_StartAuth_U2(pu32HDMITXInst, bFlag);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine get HDCP authentication status
/// @argument:
/// @return:
///                  HDMITX_INT_HDCP_DISABLE  = 0, // HDCP disable
///                  HDMITX_INT_HDCP_FAIL     = 1, // HDCP fail
///                  HDMITX_INT_HDCP_PASS     = 2, // HDCP pass
///                  HDMITX_INT_HDCP_PROCESS  = 3, // HDCP processing
//------------------------------------------------------------------------------
HDMITX_INT_HDCP_STATUS MApi_HDMITx_GetINTHDCPStatus_U2(void* pInstance)
{
    HDMITX_INT_HDCP_STATUS ret;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetHdcpEnFlag(pInstance) == FALSE)
    {
        ret = E_HDCP_DISABLE;
    }
    else
    {
        if (MDrv_HDCPTx_GetAuthDoneFlag(pInstance) == TRUE)
        {
            ret = E_HDCP_PASS;
        }
        else if (MDrv_HDCPTx_CheckAuthFailFlag(pInstance) == TRUE)
        {
            ret = E_HDCP_FAIL;
        }
        else
        {
            ret = HDMITX_INT_HDCP_PROCESS;
        }
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return ret;
}

HDMITX_INT_HDCP_STATUS MApi_HDMITx_GetINTHDCPStatus(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetINTHDCPStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetINTHDCPStatus HDMITXArgs;
    HDMITXArgs.stReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETINTHDCPSTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetINTHDCPStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.stReturn;
    }
#else
    return MApi_HDMITx_GetINTHDCPStatus_U2(pu32HDMITXInst);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine get HDCP authentication pre-status
/// @argument:
/// @return:
///                  HDMITX_INT_HDCP_FAIL     = 1, // HDCP fail
///                  HDMITX_INT_HDCP_PASS     = 2, // HDCP pass
///                  HDMITX_INT_HDCP_PROCESS  = 3, // HDCP processing
//------------------------------------------------------------------------------
HDMITX_INT_HDCP_STATUS MApi_HDMITx_GetHDCP_PreStatus_U2(void* pInstance)
{
    HDMITX_INT_HDCP_STATUS ret;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDCPTx_GetAuthDoneFlag(pInstance) == TRUE)
    {
        ret = HDMITX_INT_HDCP_PASS;
    }
    else if (MDrv_HDCPTx_GetPreState(pInstance) == HDCP14Tx_SubFSM_AuthFail)
    {
        ret = HDMITX_INT_HDCP_FAIL;
    }
    else
    {
        ret = HDMITX_INT_HDCP_PROCESS;
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return ret;
}

HDMITX_INT_HDCP_STATUS MApi_HDMITx_GetHDCP_PreStatus(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetHDCP_PreStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetHDCP_PreStatus HDMITXArgs;
    HDMITXArgs.stReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETHDCP_PRESTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetHDCP_PreStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.stReturn;
    }
#else
    return MApi_HDMITx_GetHDCP_PreStatus_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine set HDMI video output or blank or encryption while connected with unsupport HDCP Rx
/// @argument:
///                 E_UNHDCPRX_NORMAL_OUTPUT      = 0, // still display normally
///                 E_UNHDCPRX_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
///                 E_UNHDCPRX_BLUE_SCREEN = 2, // blue screen
/// @return:
//------------------------------------------------------------------------------
void MApi_HDMITx_UnHDCPRxControl_U2(void* pInstance, HDMITX_UNHDCPRX_CONTROL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (state == E_UNHDCPRX_HDCP_ENCRYPTION)
    {
        MDrv_HDCPTx_SetUnHDCPRxCtrl(pInstance, E_HDCP_ENCRYPTION);
    }
    else if (state == E_UNHDCPRX_BLUE_SCREEN)
    {
        MDrv_HDCPTx_SetUnHDCPRxCtrl(pInstance, E_BLUE_SCREEN);
    }
    else
    {
        MDrv_HDCPTx_SetUnHDCPRxCtrl(pInstance, E_NORMAL_OUTPUT);
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_UnHDCPRxControl(HDMITX_UNHDCPRX_CONTROL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_UnHDCPRxControl UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_UnHDCPRxControl HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_UNHDCPRXCONTROL, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_UnHDCPRxControl Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_UnHDCPRxControl_U2(pu32HDMITXInst, state);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine set HDMI video output or blank or encryption while HDCP authentication fail
/// @argument:
///                 E_HDCPRXFail_NORMAL_OUTPUT      = 0, // still display normally
///                 E_HDCPRXFail_HDCP_ENCRYPTION = 1, // HDCP encryption to show snow screen
///                 E_HDCPRXFail_BLUE_SCREEN = 2, // blue screen
/// @return:
//------------------------------------------------------------------------------
void MApi_HDMITx_HDCPRxFailControl_U2(void* pInstance, HDMITX_HDCPRXFail_CONTROL state)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (state == E_HDCPRXFail_HDCP_ENCRYPTION)
    {
        MDrv_HDCPTx_SetHDCPRxFailCtrl(pInstance, E_HDCP_ENCRYPTION);
    }
    else if (state == E_HDCPRXFail_BLUE_SCREEN)
    {
        MDrv_HDCPTx_SetHDCPRxFailCtrl(pInstance, E_BLUE_SCREEN);
    }
    else
    {
        MDrv_HDCPTx_SetHDCPRxFailCtrl(pInstance, E_NORMAL_OUTPUT);
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_HDCPRxFailControl(HDMITX_HDCPRXFail_CONTROL state)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        printf("MApi_HDMITx_HDCPRxFailControl >>>>>>>>>>>> pu32HDMITXInst NULL ");
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCPRxFailControl UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_HDCPRxFailControl HDMITXArgs;
    HDMITXArgs.state = state;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCPRXFAILCONTROL, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCPRxFailControl Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_HDCPRxFailControl_U2(pu32HDMITXInst, state);
#endif
}


// Debug
MS_BOOL MApi_HDMITx_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion)
{
    return MDrv_HDMITx_GetLibVer(ppVersion);
}

MS_BOOL MApi_HDMITx_GetLibVer(const MSIF_Version **ppVersion)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        printf("MApi_HDMITx_GetLibVer >>>>>>>>>>>> pu32HDMITXInst NULL ");
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetLibVer UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetLibVer HDMITXArgs;
    HDMITXArgs.ppVersion = ppVersion;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETLIBVER, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetLibVer Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetLibVer_U2(pu32HDMITXInst, ppVersion);
#endif
}

MS_BOOL MApi_HDMITx_GetInfo_U2(void* pInstance, HDMI_TX_INFO *pInfo)
{
    MS_HDMI_TX_INFO Info;

    MDrv_HDMITx_GetInfo(&Info);
    pInfo->Reserved = Info.Reserved;

    return TRUE;
}

MS_BOOL MApi_HDMITx_GetInfo(HDMI_TX_INFO *pInfo)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        printf("MApi_HDMITx_GetInfo >>>>>>>>>>>> pu32HDMITXInst NULL ");
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetInfo UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetInfo HDMITXArgs;
    HDMITXArgs.pInfo = pInfo;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETINFO, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetInfo Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetInfo_U2(pu32HDMITXInst, pInfo);
#endif
}


MS_BOOL MApi_HDMITx_GetStatus_U2(void* pInstance, HDMI_TX_Status *pStatus)
{
    MS_HDMI_TX_Status Status;
    memset(&Status, 0, sizeof(MS_HDMI_TX_Status));

    MDrv_HDMITx_GetStatus(&Status);
    pStatus->bIsInitialized = Status.bIsInitialized;
    pStatus->bIsRunning = Status.bIsRunning;
    return TRUE;
}

MS_BOOL MApi_HDMITx_GetStatus(HDMI_TX_Status *pStatus)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetStatus HDMITXArgs;
    HDMITXArgs.pStatus = pStatus;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETSTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetStatus_U2(pu32HDMITXInst, pStatus);
#endif
}

/**
* @brief set debug mask
* @param[in] u16DbgSwitch DEBUG MASK,
*   0x01: Debug HDMITX, 0x02: Debug HDCP
*/
MS_BOOL MApi_HDMITx_SetDbgLevel_U2(MS_U16 u16DbgSwitch)
{
    MS_BOOL bRet = FALSE;

    HDMITX_ENTRY();

    bRet = MDrv_HDMITx_SetDbgLevel(u16DbgSwitch);

    HDMITX_RETURN();

    return bRet;
}

MS_BOOL MApi_HDMITx_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    return MDrv_HDMITx_SetDbgLevel(u16DbgSwitch);

#if 0
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetDbgLevel UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }
    stHDMITx_SetDbgLevels HDMITXArgs;
    HDMITXArgs.u16DbgSwitch = u16DbgSwitch;
    HDMITXArgs.bReturn = FALSE;

    if(UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETSTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetDbgLevel Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#endif

}

void MApi_HDMITx_SetHPDGpioPin_U2(void* pInstance, MS_U8 u8pin)
{
    MDrv_HDMITx_SetHPDGpioPin(u8pin);
}

void MApi_HDMITx_SetHPDGpioPin(MS_U8 u8pin)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetHPDGpioPin UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetHPDGpioPin HDMITXArgs;
    HDMITXArgs.u8pin= u8pin;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETHPDGPIOPIN, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetHPDGpioPin Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetHPDGpioPin_U2(pu32HDMITXInst, u8pin);
#endif
}


// Adjust HDMITx analog setting for HDMI test or compliant issue
void MApi_HDMITx_AnalogTuning_U2(void* pInstance, HDMITX_ANALOG_TUNING *pInfo)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDMITx_AnalogTuning(pInstance, (drvHDMITX_ANALOG_TUNING*)pInfo);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_AnalogTuning(HDMITX_ANALOG_TUNING *pInfo)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_AnalogTuning UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_AnalogTuning HDMITXArgs;
    HDMITXArgs.pInfo = pInfo;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_ANALOGTUNING, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_AnalogTuning Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_AnalogTuning_U2(pu32HDMITXInst,pInfo);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx output force mode
/// @argument:
///              - bflag: TRUE: force mode, FALSE: auto mode
///              - output_mode: HDMITX_DVI: DVI, HDMITX_HDMI: HDMI
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_ForceHDMIOutputMode_U2(void* pInstance, MS_BOOL bflag, HDMITX_OUTPUT_MODE output_mode)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDMITx_ForceHDMIOutputMode(pInstance, bflag, (MsHDMITX_OUTPUT_MODE)output_mode);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_ForceHDMIOutputMode(MS_BOOL bflag, HDMITX_OUTPUT_MODE output_mode)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_ForceHDMIOutputMode UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_ForceHDMIOutputMode HDMITXArgs;
    HDMITXArgs.bflag = bflag;
    HDMITXArgs.output_mode = output_mode;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_FORCEHDMIOUTPUTMODE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_ForceHDMIOutputMode Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_ForceHDMIOutputMode_U2(pu32HDMITXInst, bflag, output_mode);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx output force color format
/// @argument:
///              - bflag: TRUE: force output color format, FALSE: auto mode
///              - output_mode: HDMITX_VIDEO_COLOR_RGB444: RGB, HDMITX_VIDEO_COLOR_YUV444: YUV
/// @return Ture: Set force output color format successfully
///             FALSE: Fail to set force output color format
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_ForceHDMIOutputColorFormat_U2(void* pInstance, MS_BOOL bflag, HDMITX_VIDEO_COLOR_FORMAT output_color)
{
    return MDrv_HDMITx_ForceHDMIOutputColorFormat(pInstance, bflag, (MsHDMITX_VIDEO_COLOR_FORMAT)output_color);
}

MS_BOOL MApi_HDMITx_ForceHDMIOutputColorFormat(MS_BOOL bflag, HDMITX_VIDEO_COLOR_FORMAT output_color)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_ForceHDMIOutputColorFormat UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_ForceHDMIOutputColorFormat HDMITXArgs;
    HDMITXArgs.bflag = bflag;
    HDMITXArgs.output_color = output_color;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_FORCEHDMIOUTPUTCOLORFORMAT, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_ForceHDMIOutputColorFormat Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_ForceHDMIOutputColorFormat_U2(pu32HDMITXInst, bflag, output_color);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine set flag to mask register write for special case \n
///            e.g. MBoot to APP with logo display
/// @argument:
///              - bFlag: TRUE: Mask register write, FALSE: not Mask
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_DisableRegWrite_U2(void* pInstance, MS_BOOL bFlag)
{
    MDrv_HDMITx_DisableRegWrite(bFlag);
}

void MApi_HDMITx_DisableRegWrite(MS_BOOL bFlag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_DisableRegWrite UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_DisableRegWrite HDMITXArgs;
    HDMITXArgs.bFlag = bFlag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_DISABLEREGWRITE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_DisableRegWrite Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_DisableRegWrite_U2(pu32HDMITXInst, bFlag);
#endif
}



//*********************//
//             CEC                 //
//*********************//

//------------------------------------------------------------------------------
/// @brief This routine get EDID physical address
/// @argument:
///              - pdata: two bytes of physical address,  ie. 1.0.0.0 => 0x10 0x00
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_GetEDIDPhyAdr_U2(void* pInstance, MS_U8 *pdata)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDMITx_GetEDIDPhyAdr(pInstance, pdata);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_GetEDIDPhyAdr(MS_U8 *pdata)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetEDIDPhyAdr UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_GetEDIDPhyAdr HDMITXArgs;
    HDMITXArgs.pdata = pdata;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETEDIDPHYADR, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetEDIDPhyAdr Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_GetEDIDPhyAdr_U2(pu32HDMITXInst, pdata);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine turn on/off HDMI Tx CEC
/// @argument:
///              - bflag: TRUE: Enable, FALSE: Disable
/// @return None
//------------------------------------------------------------------------------
void MApi_HDMITx_SetCECOnOff_U2(void* pInstance, MS_BOOL bflag)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    MDrv_HDMITx_SetCECOnOff(pInstance, bflag);

    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_SetCECOnOff(MS_BOOL bflag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetCECOnOff UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetCECOnOff HDMITXArgs;
    HDMITXArgs.bflag = bflag;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETCECONOFF, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetCECOnOff Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_SetCECOnOff_U2(pu32HDMITXInst, bflag);
#endif
}


//------------------------------------------------------------------------------
/// @brief This routine get HDMI Tx CEC On/Off status
/// @argument: None
/// @return: CEC On/Off status
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_GetCECStatus_U2(void* pInstance)
{
    return MDrv_HDMITx_GetCECStatus(pInstance);
}

MS_BOOL MApi_HDMITx_GetCECStatus(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetCECStatus UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetCECStatus HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETCECSTATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetCECStatus Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetCECStatus_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine get EDID from reciver
/// @argument: None
/// @return: read status
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_EdidChecking_U2(void* pInstance)
{
    MS_BOOL bRet;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();

    if (MDrv_HDMITx_GetRxStatus() == TRUE)
    {
        bRet = MDrv_HDMITx_EdidChecking(pInstance);
    }
    else
        bRet = FALSE;

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}


MS_BOOL MApi_HDMITx_EdidChecking(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetHPDGpioPin UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_EdidChecking HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;

    if(UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_EDIDCHECKING, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_EdidChecking Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_EdidChecking_U2(pu32HDMITXInst);
#endif
}

//*********************//
//      RxBypassMode         //
//*********************//
//------------------------------------------------------------------------------
/// @brief This routine set Rx Bypass Mode
/// @argument: freq: Input Freq; bflag:DDC/CEC/ARC bypass
/// @return: setting status.
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_RxBypass_Mode_U2(void* pInstance, HDMITX_INPUT_FREQ freq, MS_BOOL bflag)
{
    return MDrv_HDMITx_RxBypass_Mode(pInstance, (MsHDMITX_INPUT_FREQ) freq, bflag);
}

MS_BOOL MApi_HDMITx_RxBypass_Mode(HDMITX_INPUT_FREQ freq, MS_BOOL bflag)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_RxBypass_Mode UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_RxBypass_Mode HDMITXArgs;
    HDMITXArgs.freq = freq;
    HDMITXArgs.bflag = bflag;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_RXBYPASS_MODE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_RxBypass_Mode Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_RxBypass_Mode_U2(pu32HDMITXInst, freq, bflag);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine disable Rx Bypass Mode
/// @return: setting status.
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_Disable_RxBypass_U2(void* pInstance)
{
    return MDrv_HDMITx_Disable_RxBypass(pInstance);
}

MS_BOOL MApi_HDMITx_Disable_RxBypass(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_Disable_RxBypass UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_Disable_RxBypass HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_DISABLE_RXBYPASS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_Disable_RxBypass Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_Disable_RxBypass_U2(pu32HDMITXInst);
#endif
}

//------------------------------------------------------------------------------
/// @brief This routine to set the time interval from sent aksv to R0.
/// The HDCP Transmitter must not read the R0' value sooner than 100ms after writing Aksv.
/// @return: setting status.
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_SetAksv2R0Interval_U2(void* pInstance, MS_U32 u32Interval)
{
    MDrv_HDCPTx_SetAksv2R0Interval(u32Interval);

    return TRUE;
}

MS_BOOL MApi_HDMITx_SetAksv2R0Interval(MS_U32 u32Interval)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetAksv2R0Interval UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_SetAksv2R0Interval HDMITXArgs;
    HDMITXArgs.u32Interval = u32Interval;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETAKSV2R0INTERVAL, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetAksv2R0Interval Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_SetAksv2R0Interval_U2(pu32HDMITXInst, u32Interval);
#endif
}




//------------------------------------------------------------------------------
/// @brief This routine to Get if Rx support HDCP
/// @return: setting status.
//------------------------------------------------------------------------------
MS_BOOL MApi_HDMITx_IsHDCPRxValid_U2(void* pInstance)
{
    MS_BOOL bRet;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    bRet = MDrv_HDCPTx_GetRxValid(pInstance);

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_IsHDCPRxValid(void)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_IsHDCPRxValid UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_IsHDCPRxValid HDMITXArgs;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_ISRXVALID, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_IsHDCPRxValid Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_IsHDCPRxValid_U2(pu32HDMITXInst);
#endif
}



//******************************************************************************
/// API for Query HDMITx Capability
/// @param eCapType \b IN: Capability type
/// @param pRet     \b OUT: return value
/// @param ret_size \b IN: input structure size
/// @return MS_BOOL - Success
//******************************************************************************
MS_BOOL MApi_HDMITx_GetChipCaps_U2(void* pInstance, EN_HDMITX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{

    MS_BOOL bRet = TRUE;
    switch(eCapType)
    {
        case E_HDMITX_CAP_SUPPORT_DVI:
            *pRet = (MS_U32)MDrv_HDMITx_IsSupportDVIMode();
            break;
        default:
            printf("[%s]not support HDMITx capability case: %d\n",__FUNCTION__, eCapType);
            bRet = FALSE;
    }

    return bRet;
}

MS_BOOL MApi_HDMITx_GetChipCaps(EN_HDMITX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetChipCaps UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetChipCaps HDMITXArgs;
    HDMITXArgs.eCapType = eCapType;
    HDMITXArgs.pRet = pRet;
    HDMITXArgs.ret_size = ret_size;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETCHIPCAPS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetChipCaps Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetChipCaps_U2(pu32HDMITXInst, eCapType, pRet, ret_size);
#endif
}



//******************************************************************************
/// API for speed up resume process from power saving mode
/// @param EN_POWER_MODE \b IN: power state
//******************************************************************************
MS_U32 MApi_HDMITx_SetPowerState_U2(void* pInstance, EN_POWER_MODE u16PowerState)
{
    MS_U32 u32RetVal;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    u32RetVal= MDrv_HDMITx_SetPowerState(pInstance, u16PowerState);

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return u32RetVal;
}

MS_U32 MApi_HDMITx_SetPowerState(EN_POWER_MODE u16PowerState)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetPowerState UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_SetPowerState HDMITXArgs;
    HDMITXArgs.u16PowerState = u16PowerState;
    HDMITXArgs.u32Return = 0x00;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETPOWERSTATE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetPowerState Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.u32Return;
    }
#else
    return MApi_HDMITx_SetPowerState_U2(pu32HDMITXInst, u16PowerState);
#endif
}

//******************************************************************************
/// API for getting certain EDID data block
/// @param HDMITX_CEA_DB_TAG_CODE \b IN: tag code of EDID block
/// @param HDMITX_CEA_EXT_TAG_CODE \b IN: extended tag code of EDID data block
/// @param MS_U8* \b IN: pointer to store data block
/// @param MS_U32 \b IN: buffer length of data pointer
/// @param MS_U32* \b IN: real data length of certain data block
/// @param MS_BOOL  \b IN: return if EDID is valid
//******************************************************************************
MS_BOOL MApi_HDMITx_GetEdidDataBlocks_U2(void* pInstance, HDMITX_CEA_DB_TAG_CODE enTagCode, HDMITX_CEA_EXT_TAG_CODE enExtTagCode, MS_U8* pu8Data, MS_U32 u32DataLen, MS_U32* pu32RealLen)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    if (MDrv_HDMITx_GetEdidRdyFlag(pInstance) == TRUE)
    {
        bRet = TRUE;

        switch (enTagCode)
        {
            case E_CEA_TAG_CODE_AUDIO:
                MDrv_HDMITx_GetAudioDataBlock(pInstance, pu8Data, u32DataLen, pu32RealLen);
            break;

            case E_CEA_TAG_CODE_VIDEO:
                MDrv_HDMITx_GetVideoDataBlock(pInstance, pu8Data, u32DataLen, pu32RealLen);
            break;

            case E_CEA_TAG_CODE_VSDB:
                MDrv_HDMITx_GetVSDB(pInstance, pu8Data, u32DataLen, pu32RealLen);
            break;

            case E_CEA_TAG_CODE_HFVSDB:
                MDrv_HDMITx_GetHFVSDB(pInstance, pu8Data, u32DataLen, pu32RealLen);
            break;

            case E_CEA_TAG_CODE_SPEAKER_ALLOCAT:
                //TBD: not support currently
                printf("%s:: block Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_TAG_CODE_SPEAKER_ALLOCAT);
                bRet = FALSE;
            break;

            case E_CEA_TAG_CODE_VDTC:
                //TBD: not support currently
                printf("%s:: block Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_TAG_CODE_VDTC);
                bRet = FALSE;
            break;

            case E_CEA_TAG_CODE_EXT_TAG:
            {
                switch (enExtTagCode)
                {
                    case E_CEA_EXT_TAG_VCDB:
                        //TBD: not support currently
                        printf("%s:: Extended Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_EXT_TAG_VCDB);
                        bRet = FALSE;
                    break;

                    case E_CEA_EXT_TAG_VSVDB:
                        //TBD: not support currently
                        printf("%s:: Extended Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_EXT_TAG_VSVDB);
                        bRet = FALSE;
                    break;

                    case E_CEA_EXT_TAG_VDDDB:
                        //TBD: not support currently
                        printf("%s:: Extended Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_EXT_TAG_VDDDB);
                        bRet = FALSE;
                    break;

                    case E_CEA_EXT_TAG_VVTBE:
                        //TBD: not support currently
                        printf("%s:: Extended Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_EXT_TAG_VVTBE);
                        bRet = FALSE;
                    break;

                    case E_CEA_EXT_TAG_CDB:
                        MDrv_HDMITx_GetColorimetryDataBlock(pInstance, pu8Data, u32DataLen, pu32RealLen);
                    break;

                    case E_CEA_EXT_TAG_HSMDB:
                        MDrv_HDMITx_GetHDRStaticMetaDataBlock(pInstance, pu8Data, u32DataLen, pu32RealLen);
                    break;

                    case E_CEA_EXT_TAG_420VDB:
                        MDrv_HDMITx_Get420VideoDataBlock(pInstance, pu8Data, u32DataLen, pu32RealLen);
                    break;

                    case E_CEA_EXT_TAG_420CMDB:
                        MDrv_HDMITx_Get420CapabilityMapDataBlock(pInstance, pu8Data, u32DataLen, pu32RealLen);
                    break;

                    case E_CEA_EXT_TAG_VSADB:
                        //TBD: not support currently
                        printf("%s:: Extended Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_EXT_TAG_VSADB);
                        bRet = FALSE;
                    break;

                    case E_CEA_EXT_TAG_IFDB:
                        //TBD: not support currently
                        printf("%s:: Extended Tag %d NOT support!!\r\n", __FUNCTION__, E_CEA_EXT_TAG_IFDB);
                        bRet = FALSE;
                    break;

                    default:
                        printf("%s::Invalid Argument!!\r\n", __FUNCTION__);
                        bRet = FALSE;
                    break;

                }
            }
            break;

            default:
                printf("%s::Invalid Argument!!\r\n", __FUNCTION__);
                bRet = FALSE;
            break;
        }
    }
    else
    {
        printf("%s::EDID is NOT valid yet!!\r\n", __FUNCTION__);
        bRet = FALSE;
    }

    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetEdidDataBlocks(HDMITX_CEA_DB_TAG_CODE enTagCode, HDMITX_CEA_EXT_TAG_CODE enExtTagCode, MS_U8* pu8Data, MS_U32 u32DataLen, MS_U32* pu32RealLen)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GetEdidDataBlocks UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetEdidDataBlocks HDMITXArgs;
    HDMITXArgs.enTagCode = enTagCode;
    HDMITXArgs.enExtTagCode = enExtTagCode;
    HDMITXArgs.pu8Data = pu8Data;
    HDMITXArgs.u32DataLen = u32DataLen;
    HDMITXArgs.pu32RealLen = pu32RealLen;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETEDIDDATABLOCK, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_GetEdidDataBlocks Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetEdidDataBlocks_U2(pu32HDMITXInst, enTagCode, enExtTagCode, pu8Data, u32DataLen, pu32RealLen);
#endif
}

MS_BOOL MApi_HDMITx_GetKSVList_U2(void* pInstance, MS_U8 *pu8Bstatus, MS_U8* pu8KSVList, MS_U16 u16BufLen, MS_U16 *pu16KSVLength)
{
    MS_BOOL bRet = FALSE;

    bRet = MDrv_HDCPTx_GetKSVList(pu8Bstatus, pu8KSVList, u16BufLen, pu16KSVLength);

    return bRet;
}

MS_BOOL MApi_HDMITx_GetKSVList(MS_U8 *pu8Bstatus, MS_U8* pu8KSVList, MS_U16 u16BufLen, MS_U16 *pu16KSVLength)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDCPTxGetKSVList UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_GetKSVList HDMITXArgs;
    HDMITXArgs.pu8Bstatus = pu8Bstatus;
    HDMITXArgs.pu8KSVList = pu8KSVList;
    HDMITXArgs.u16BufLen = u16BufLen;
    HDMITXArgs.pu16KSVLength = pu16KSVLength;
    HDMITXArgs.bReturn = FALSE;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GETKSVLIST, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDCPTxGetKSVList Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_GetKSVList_U2(pu32HDMITXInst, pu8Bstatus, pu8KSVList, u16BufLen, pu16KSVLength);
#endif
}


/*********************************************************************/
/*                                                                                                                     */
/*                                         HDCP22 Relative                                                    */
/*                                                                                                                     */
/*********************************************************************/
MS_BOOL MApi_HDMITx_HDCP2AccessX74_U2(void* pInstance, MS_U8 u8PortIdx, MS_U8 u8OffsetAddr, MS_U8 u8OpCode, MS_U8 *pu8RdBuf, MS_U16 u16RdLen, MS_U8 *pu8WRBuff, MS_U16 u16WrLen)
{
    MS_BOOL bRet = FALSE;

    _HDMITX_SEMAPHORE_ENTRY(pInstance);

    switch (u8OffsetAddr)
    {
        case E_HDCP1_OFFSETADDR_Bksv:
        case E_HDCP1_OFFSETADDR_RiPrime:
        case E_HDCP1_OFFSETADDR_PjPrime:
        case E_HDCP1_OFFSETADDR_VPrime:
        case E_HDCP1_OFFSETADDR_VPrimeH1:
        case E_HDCP1_OFFSETADDR_VPrimeH2:
        case E_HDCP1_OFFSETADDR_VPrimeH3:
        case E_HDCP1_OFFSETADDR_VPrimeH4:
        case E_HDCP1_OFFSETADDR_BCaps:
        case E_HDCP1_OFFSETADDR_BStatus:
        case E_HDCP1_OFFSETADDR_KsvFifo:
        case E_HDCP2_OFFSETADDR_HDCP2Version:
        case E_HDCP2_OFFSETADDR_RxStatus:
        case E_HDCP2_OFFSETADDR_ReadMessage:
        {
            if (u8OpCode == E_HDCP2_OPCODE_READ)
            {
                if (pu8RdBuf != NULL)
                    bRet = MDrv_HDMITx_HDCP2AccessX74(u8PortIdx, u8OffsetAddr, u8OpCode, pu8RdBuf, pu8WRBuff, u16RdLen, u16WrLen);
                else
                    printf("%s:: Read buffer is NULL\r\n", __FUNCTION__);
            }
            else
            {
                printf("%s:: Invalid Opeartion Code\r\n", __FUNCTION__);
            }
        }
        break;

        case E_HDCP1_OFFSETADDR_Aksv:
        case E_HDCP1_OFFSETADDR_Ainfo:
        case E_HDCP1_OFFSETADDR_An:
        case E_HDCP2_OFFSETADDR_WriteMessage:
        {
            if (u8OpCode == E_HDCP2_OPCODE_WRITE)
            {
                if (pu8WRBuff != NULL)
                    bRet = MDrv_HDMITx_HDCP2AccessX74(u8PortIdx, u8OffsetAddr, u8OpCode, pu8RdBuf, pu8WRBuff, u16RdLen, u16WrLen);
                else
                    printf("%s:: Write buffer is NULL\r\n", __FUNCTION__);
            }
            else
            {
                printf("%s:: Invalid Opeartion Code\r\n", __FUNCTION__);
            }
        }
        break;

        //case E_HDCP1_OFFSETADDR_DBG: //same offset address;
        case E_HDCP2_OFFSETADDR_DBG:
        {
            if (u8OpCode == E_HDCP2_OPCODE_READ)
            {
                if (pu8RdBuf != NULL)
                    bRet = MDrv_HDMITx_HDCP2AccessX74(u8PortIdx, u8OffsetAddr, u8OpCode, pu8RdBuf, pu8WRBuff, u16RdLen, u16WrLen);
                else
                    printf("%s:: Read buffer is NULL\r\n", __FUNCTION__);
            }
            else
            {
                if (pu8WRBuff != NULL)
                    bRet = MDrv_HDMITx_HDCP2AccessX74(u8PortIdx, u8OffsetAddr, u8OpCode, pu8RdBuf, pu8WRBuff, u16RdLen, u16WrLen);
                else
                    printf("%s:: Write buffer is NULL\r\n", __FUNCTION__);
            }
        }
        break;

        default:
            printf("%s::Invalid HDCP22 x74 Offset Address\r\n", __FUNCTION__);
        break;
    }
    _HDMITX_SEMAPHORE_RETURN(pInstance);

    return bRet;
}

MS_BOOL MApi_HDMITx_HDCP2AccessX74(MS_U8 u8PortIdx, MS_U8 u8OffsetAddr, MS_U8 u8OpCode, MS_U8 *pu8RdBuf, MS_U16 u16RdLen, MS_U8 *pu8WRBuff, MS_U16 u16WrLen)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP2AccessX74 UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    stHDMITx_HDCP2AccessX74 HDMITXArgs;
    HDMITXArgs.u8PortIdx = u8PortIdx;
    HDMITXArgs.u8OffsetAddr = u8OffsetAddr;
    HDMITXArgs.u8OpCode = u8OpCode;
    HDMITXArgs.pu8RdBuf = pu8RdBuf;
    HDMITXArgs.pu8WRBuff = pu8WRBuff;
    HDMITXArgs.u16RdLen = u16RdLen;
    HDMITXArgs.u16WrLen = u16WrLen;
    HDMITXArgs.bReturn = FALSE;

    if(UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP2ACCESSX74, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP2AccessX74 Obtain HDMITX engine fail\n");
        return FALSE;
    }
    else
    {
        return HDMITXArgs.bReturn;
    }
#else
    return MApi_HDMITx_HDCP2AccessX74_U2(pu32HDMITXInst, u8PortIdx, u8OffsetAddr, u8OpCode, pu8RdBuf, u16RdLen, pu8WRBuff, u16WrLen);
#endif
}

void MApi_HDMITx_HDCP2TxInit_U2(void *pInstance, MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    MDrv_HDMITx_HDCP2TxInit(u8PortIdx, bEnable);
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_HDCP2TxInit(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP2TxInit UtopiaOpen HDMITX failed\n");
            return;
        }
    }

    stHDMITx_HDCP2TxInit HDMITXArgs;
    HDMITXArgs.u8PortIdx = u8PortIdx;
    HDMITXArgs.bEnable = bEnable;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP2TxInit, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP2TxInit Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_HDCP2TxInit_U2(pu32HDMITXInst, u8PortIdx, bEnable);
#endif
}

void MApi_HDMITx_HDCP2TxEnableEncrypt_U2(void *pInstance, MS_U8 u8PortIdx, MS_BOOL bEnable)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    MDrv_HDMITx_HDCP2TxEnableEncryptEnable(u8PortIdx, bEnable);
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_HDCP2TxEnableEncrypt(MS_U8 u8PortIdx, MS_BOOL bEnable)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP2TxEnableEncrypt UtopiaOpen HDMITX failed\n");
            return;
        }
    }

    stHDMITx_HDCP2TxEnableEncrypt HDMITXArgs;
    HDMITXArgs.u8PortIdx = u8PortIdx;
    HDMITXArgs.bEnable = bEnable;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP2TxEnableEncrypt, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP2TxEnableEncrypt Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_HDCP2TxEnableEncrypt_U2(pu32HDMITXInst, u8PortIdx, bEnable);
#endif
}

void MApi_HDMITx_HDCP2TxFillCipherKey_U2(void *pInstance, MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8KsXORLC128)
{
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    MDrv_HDMITx_HDCP2TxFillCipherKey(u8PortIdx, pu8Riv, pu8KsXORLC128);
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

void MApi_HDMITx_HDCP2TxFillCipherKey(MS_U8 u8PortIdx, MS_U8 *pu8Riv, MS_U8 *pu8KsXORLC128)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_HDCP2TxFillCipherKey UtopiaOpen HDMITX failed\n");
            return;
        }
    }

    stHDMITx_HDCP2TxFillCipherKey HDMITXArgs;
    HDMITXArgs.u8PortIdx = u8PortIdx;
    HDMITXArgs.pu8Riv = pu8Riv;
    HDMITXArgs.pu8KsXORLC128 = pu8KsXORLC128;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_HDCP2TxFillCipherKey, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_HDCP2TxFillCipherKey Obtain HDMITX engine fail\n");
        return;
    }
    else
    {
        return;
    }
#else
    MApi_HDMITx_HDCP2TxFillCipherKey_U2(pu32HDMITXInst, u8PortIdx, pu8Riv, pu8KsXORLC128);
#endif
}

MS_BOOL MApi_HDMITx_GeneralCtrl_U2(void* pInstance, MS_U32 u32Cmd, MS_U8* pu8Buf, MS_U32 u32BufSize)
{
    return TRUE;
}

MS_BOOL MApi_HDMITx_ColorandRange_Transform_U2(void* pInstance, HDMITX_VIDEO_COLOR_FORMAT incolor, HDMITX_VIDEO_COLOR_FORMAT outcolor, HDMITX_QUANT_RANGE inange, HDMITX_QUANT_RANGE outrange)
{
    MS_BOOL ubRet = 0;
    pInstance = pInstance;

    ubRet = MDrv_HDMITx_ColorandRange_Transform(pInstance, (MsHDMITX_VIDEO_COLOR_FORMAT) incolor,(MsHDMITX_VIDEO_COLOR_FORMAT) outcolor, (MsHDMITX_YCC_QUANT_RANGE) inange, (MsHDMITX_YCC_QUANT_RANGE) outrange);
    return ubRet;
}

MS_BOOL MApi_HDMITx_ColorandRange_Transform(HDMITX_VIDEO_COLOR_FORMAT incolor, HDMITX_VIDEO_COLOR_FORMAT outcolor, HDMITX_QUANT_RANGE inange, HDMITX_QUANT_RANGE outrange)
{
    HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS HDMITXArgs;
    HDMITXArgs.input_color = incolor;
    HDMITXArgs.output_color = outcolor;
    HDMITXArgs.input_range = inange;
    HDMITXArgs.output_range = outrange;
    HDMITXArgs.result = 0;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
       printf("MApi_HDMITx_GeneralCtrl Obtain HDMITX engine fail @ E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM\n");
       return FALSE;
    }
    else
    {
       //HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS *HDMITXArgs = (HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS*)pBuf;
       //HDMITXArgs->result = MDrv_HDMITx_ColorandRange_Transform((MsHDMITX_VIDEO_COLOR_FORMAT) pParameters->input_color,(MsHDMITX_VIDEO_COLOR_FORMAT) pParameters->output_color, (MsHDMITX_VIDEO_COLOR_RANGE) pParameters->input_range, (MsHDMITX_VIDEO_COLOR_RANGE) pParameters->output_range);
       return HDMITXArgs.result;
    }
}

MS_BOOL MApi_HDMITx_SSC_Enable_U2(void* pInstance, MS_U8 ubSSCEn)
{
    MS_BOOL ubRet = FALSE;

    ubRet = MDrv_HDMITx_SSC_Enable(pInstance, ubSSCEn);
    return ubRet;
}

MS_BOOL MApi_HDMITx_SSC_Enable(MS_U8 ubSSCEn)
{
    HDMITX_SSCENABLE HDMITXArgs;
    HDMITXArgs.u8SSCEn = ubSSCEn;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SSC_ENABLE, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
       printf("MApi_HDMITx_GeneralCtrl Obtain HDMITX engine fail @ E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM\n");
       return FALSE;
    }
    else
    {
       //HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS *HDMITXArgs = (HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS*)pBuf;
       //HDMITXArgs->result = MDrv_HDMITx_ColorandRange_Transform((MsHDMITX_VIDEO_COLOR_FORMAT) pParameters->input_color,(MsHDMITX_VIDEO_COLOR_FORMAT) pParameters->output_color, (MsHDMITX_VIDEO_COLOR_RANGE) pParameters->input_range, (MsHDMITX_VIDEO_COLOR_RANGE) pParameters->output_range);
       return HDMITXArgs.result;
    }
}

MS_U8 MApi_HDMITx_SetAVIInfoColorimetry_U2(void* pInstance, HDMITX_AVI_COLORIMETRY enColorimetry)
{
    MS_U8 ubRet = FALSE;

    ubRet = MDrv_HDMITx_SetAVIInfoColorimetry(pInstance, (MsHDMITX_COLORIMETRY) enColorimetry);
    return ubRet;
}

MS_U8 MApi_HDMITx_SetAVIInfoColorimetry(HDMITX_AVI_COLORIMETRY enColorimetry)
{
    HDMITX_SET_COLORIMETRY HDMITXArgs;
    HDMITXArgs.colorimetry = enColorimetry;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SET_COLORIMETRY, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
       printf("MApi_HDMITx_GeneralCtrl Obtain HDMITX engine fail @ E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM\n");
       return FALSE;
    }
    else
    {
       return HDMITXArgs.u8Return;
    }
}

MS_U32 MApi_HDMITx_GetFullRxStatus_U2(void* pInstance)
{
    MS_U32 enRxStatus = 0;
    enRxStatus = (MS_U32) MDrv_HDMITx_GetFullRxStatus(pInstance);

    return enRxStatus;
}

HDMITX_RX_STATUS MApi_HDMITx_GetFullRxStatus(void)
{
    HDMITX_GET_FULL_RX_STATUS HDMITXArgs;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GET_FULL_RX_STATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
       printf("MApi_HDMITx_GeneralCtrl Obtain HDMITX engine fail @ E_HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM\n");
       return FALSE;
    }
    else
    {
       return (HDMITX_RX_STATUS) HDMITXArgs.u32RxStatus;
    }
}

HDMITX_TIMING_ERROR MApi_HDMITx_CheckLegalTiming_U2(void* pInstance, HDMITX_OUTPUT_MODE eOutputMode, HDMITX_VIDEO_TIMING idx, HDMITX_VIDEO_COLOR_FORMAT incolor_fmt, HDMITX_VIDEO_COLOR_FORMAT outcolor_fmt, HDMITX_VIDEO_COLORDEPTH_VAL color_depth)
{
    HDMITX_TIMING_ERROR uiLegalStatus = 0;
    uiLegalStatus = (HDMITX_TIMING_ERROR)MDrv_HDMITx_CheckLegalTiming(pInstance, (MsHDMITX_OUTPUT_MODE)eOutputMode, (MsHDMITX_VIDEO_TIMING)idx, (MsHDMITX_VIDEO_COLOR_FORMAT)incolor_fmt, (MsHDMITX_VIDEO_COLOR_FORMAT)outcolor_fmt, (MsHDMITX_VIDEO_COLORDEPTH_VAL)color_depth);

    return uiLegalStatus;
}

HDMITX_TIMING_ERROR MApi_HDMITx_CheckLegalTiming(HDMITX_OUTPUT_MODE eOutputMode, HDMITX_VIDEO_TIMING idx, HDMITX_VIDEO_COLOR_FORMAT incolor_fmt, HDMITX_VIDEO_COLOR_FORMAT outcolor_fmt, HDMITX_VIDEO_COLORDEPTH_VAL color_depth)
{
    HDMITX_CHECK_LEGAL_TIMING HDMITXArgs;

    HDMITXArgs.eTiming = idx;
    HDMITXArgs.eInColor = incolor_fmt;
    HDMITXArgs.eOutColor = outcolor_fmt;
    HDMITXArgs.eColorDepth = color_depth;
    HDMITXArgs.eOutputMode = eOutputMode;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_TIMING_CAPABILITY_CHECK, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
       printf("MApi_HDMITx_GeneralCtrl Obtain HDMITX engine fail @ E_HDMITX_CMD_TIMING_CAPABILITY_CHECK\n");
       return FALSE;
    }
    else
    {
       return HDMITXArgs.ubRet;
    }
}

MS_U32 MApi_HDMITx_GetPanelSize_FromEDID(HDMITX_GET_PANELSIZE_FROM_EDID* pstPanelSize)
{
    MS_U8 ubEDIDData[128] = {0};
    MS_BOOL ubRet = 0;

    ubRet = MApi_HDMITx_GetEDIDData(ubEDIDData, 0);

    if(ubEDIDData[0x15] != 0 && ubEDIDData[0x16]!= 0)
    {
        pstPanelSize->u32PanelWidth = ubEDIDData[0x15];
        pstPanelSize->u32PanelHeight = ubEDIDData[0x16];
    }
    else
    {
        pstPanelSize->u32PanelWidth = 0;
        pstPanelSize->u32PanelHeight = 0;
    }

    return ubRet;
}

MS_U32 MApi_HDMITx_GetTMDSStatus_U2(void* pInstance)
{
    MS_U32 u32TMDSStatus = 0;
    u32TMDSStatus = MDrv_HDMITx_GetTMDSStatus(pInstance);
    return u32TMDSStatus;
}

MS_U32 MApi_HDMITx_GetTMDSStatus(HDMITX_GET_TMDS_STATUS* pstTMDSStatus)
{
    HDMITX_GET_TMDS_STATUS HDMITXArgs;

    HDMITXArgs.u32TMDSStatus = 0;
    HDMITXArgs.u32Ret= 0;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_GET_TMDS_STATUS, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
       printf("MApi_HDMITx_GeneralCtrl Obtain HDMITX engine fail @ E_HDMITX_CMD_GET_TMDS_STATUS\n");
       return FALSE;
    }
    else
    {
       pstTMDSStatus->u32TMDSStatus = HDMITXArgs.u32TMDSStatus;
       return HDMITXArgs.u32Ret;
    }
}

MS_BOOL SYMBOL_WEAK MApi_HDMITx_GeneralCtrl(MS_U32 u32Cmd, void* pu8Buf, MS_U32 u32BufSize)
{

    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_GeneralCtrl UtopiaOpen HDMITX failed\n");
            return FALSE;
        }
    }

    switch(u32Cmd)
    {
        case HDMITX_CMD_COLOR_AND_RANGE_TRANSFORM:
        {
            if (u32BufSize == sizeof(HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS))
            {
                    HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS *HDMITXArgs = (HDMITX_COLOR_AND_RANGE_TRANSFORM_PARAMETERS*)pu8Buf;
                    HDMITXArgs->result = MApi_HDMITx_ColorandRange_Transform(HDMITXArgs->input_color, HDMITXArgs->output_color, HDMITXArgs->input_range, HDMITXArgs->output_range);
                    return HDMITXArgs->result;
            }
        }
        break;

        case HDMITX_CMD_SSC_ENABLE:
        {
            HDMITX_SSCENABLE *HDMITXArgs = (HDMITX_SSCENABLE*)pu8Buf;
            HDMITXArgs->result = MApi_HDMITx_SSC_Enable(HDMITXArgs->u8SSCEn);
            return HDMITXArgs->result;
        }
        break;

        case HDMITX_CMD_SET_COLORIMETRY:
        {
            if (u32BufSize == sizeof(HDMITX_SET_COLORIMETRY))
            {
                HDMITX_SET_COLORIMETRY *HDMITXArgs = (HDMITX_SET_COLORIMETRY*)pu8Buf;
                HDMITXArgs->u8Return = MApi_HDMITx_SetAVIInfoColorimetry(HDMITXArgs->colorimetry);
                return HDMITXArgs->u8Return;
            }
        }
        break;

        case HDMITX_CMD_GET_FULL_RX_STATUS:
        {
            if (u32BufSize == sizeof(HDMITX_GET_FULL_RX_STATUS))
            {
                HDMITX_GET_FULL_RX_STATUS *HDMITXArgs = (HDMITX_GET_FULL_RX_STATUS*)pu8Buf;
                HDMITXArgs->u32RxStatus = (MS_U32) MApi_HDMITx_GetFullRxStatus();
                return HDMITXArgs->u32RxStatus;
            }
        }
        break;

        case HDMITX_CMD_TIMING_CAPABILITY_CHECK:
        {
            if (u32BufSize == sizeof(HDMITX_CHECK_LEGAL_TIMING))
            {
                HDMITX_CHECK_LEGAL_TIMING *HDMITXArgs = (HDMITX_CHECK_LEGAL_TIMING*)pu8Buf;
                HDMITXArgs->ubRet = MApi_HDMITx_CheckLegalTiming(HDMITXArgs->eOutputMode, HDMITXArgs->eTiming, HDMITXArgs->eInColor, HDMITXArgs->eOutColor, HDMITXArgs->eColorDepth);
                return HDMITXArgs->ubRet;
            }
        }
        break;

        case HDMITX_CMD_GET_PANELSIZE_FROM_EDID:
        {
            if (u32BufSize == sizeof(HDMITX_GET_PANELSIZE_FROM_EDID))
            {
                HDMITX_GET_PANELSIZE_FROM_EDID *HDMITXArgs = (HDMITX_GET_PANELSIZE_FROM_EDID*)pu8Buf;
                HDMITXArgs->u32Ret = MApi_HDMITx_GetPanelSize_FromEDID(HDMITXArgs);
                return (MS_BOOL)HDMITXArgs->u32Ret;
            }
        }
        break;

        case HDMITX_CMD_GET_TMDS_STATUS:
        {
            if (u32BufSize == sizeof(HDMITX_GET_TMDS_STATUS))
            {
                HDMITX_GET_TMDS_STATUS *HDMITXArgs = (HDMITX_GET_TMDS_STATUS*)pu8Buf;
                HDMITXArgs->u32Ret = MApi_HDMITx_GetTMDSStatus(HDMITXArgs);
                return (MS_BOOL)HDMITXArgs->u32Ret;
            }
        }
        break;

        case HDMITX_CMD_SET_TIMING_INFO_BY_CUSTOMER:
        {
            if (u32BufSize == sizeof(HDMITX_SET_TIMING_INFO_BY_CUSTOMER))
            {
                HDMITX_SET_TIMING_INFO_BY_CUSTOMER *HDMITXArgs = (HDMITX_SET_TIMING_INFO_BY_CUSTOMER*)pu8Buf;
                MApi_HDMITx_SetVideoOutputTimingByCustomer(HDMITXArgs->u8Timing, HDMITXArgs->stTimingInfo);
                return TRUE;
            }
        }
        break;

        default:
        {
            printf("MApi_HDMITx_GeneralCtrl with unknow command\r\n");
        }
        break;
    }

    return FALSE;
}
//------------------------------------------------------------------------------
/// @brief This routine config HDMI Tx Module video output timing by Customers
/// @param[in] mode output video timing and timing detail
/// @return None
//-----------------------------------------------------------------------------
void MApi_HDMITx_SetVideoOutputTimingByCustomer_U2(void* pInstance, HDMITX_VIDEO_TIMING mode, HDMITX_TIMING_INFO_BY_CUSTOMER timinginfo)
{
    stHDMITX_TIMING_INFO_BY_CUSTOMER stTimingInfo;
    _HDMITX_SEMAPHORE_ENTRY(pInstance);
    HDMITX_ENTRY();
#ifdef URANUS
    if ((MS_U32)mode > (MS_U32)HDMITX_RES_720x576p)
    {
        printf("[%s][%d] Unsupport HDMI mode 0x%08x\n", __FUNCTION__, __LINE__, mode);
    }
#endif
#ifdef OBERON
    if ((MS_U32)mode >= (MS_U32)HDMITX_RES_MAX)
    {
        printf("[%s][%d] Unsupport HDMI mode 0x%08x\n", __FUNCTION__, __LINE__, mode);
    }
#endif

    // 3D frame packing only support up to 24-bit color depth
    // For HDMI 2.0 spec, 4k2k@60 only support up to 24-bit color depth
    stTimingInfo.i_p_mode = timinginfo.i_p_mode;
    stTimingInfo.h_polarity = timinginfo.h_polarity;
    stTimingInfo.v_polarity = timinginfo.v_polarity;
    stTimingInfo.vs_width = timinginfo.vs_width;
    stTimingInfo.vs_bporch = timinginfo.vs_bporch;
    stTimingInfo.vde_width = timinginfo.vde_width;
    stTimingInfo.hs_width = timinginfo.hs_width;
    stTimingInfo.hs_bporch = timinginfo.hs_bporch;
    stTimingInfo.hde_width = timinginfo.hde_width;
    stTimingInfo.vtotal = timinginfo.vtotal;
    stTimingInfo.htotal = timinginfo.htotal;
    stTimingInfo.vs_delayline = timinginfo.vs_delayline;
    stTimingInfo.vs_delaypixel = timinginfo.vs_delaypixel;
    stTimingInfo.hs_delay = timinginfo.hs_delay;
    switch (mode)
    {

        case HDMITX_RES_1280x720p_50Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1280x720p_50Hz, stTimingInfo);
        break;

        case HDMITX_RES_1280x720p_60Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1280x720p_60Hz, stTimingInfo);
        break;

        case HDMITX_RES_1920x1080p_50Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1920x1080p_50Hz, stTimingInfo);
        break;

        case HDMITX_RES_1920x1080p_60Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1920x1080p_60Hz, stTimingInfo);
        break;

        case HDMITX_RES_1600x1200p_60Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1600x1200p_60Hz, stTimingInfo);
        break;

        case HDMITX_RES_1440x900p_60Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1440x900p_60Hz, stTimingInfo);
        break;

        case HDMITX_RES_1280x1024p_60Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1280x1024p_60Hz, stTimingInfo);
        break;

        case HDMITX_RES_1024x768p_60Hz:
            MDrv_HDMITx_SetVideoOutputModeByCustomer(pInstance, E_HDMITX_RES_1024x768p_60Hz, stTimingInfo);
        break;

        default:
            printf("[HDMITx]: Error video timing: %d\n", mode);
        break;
    }

    HDMITX_RETURN();
    _HDMITX_SEMAPHORE_RETURN(pInstance);
}

//17.8.11 settiming info by customer
void MApi_HDMITx_SetVideoOutputTimingByCustomer(HDMITX_VIDEO_TIMING u8mode, HDMITX_TIMING_INFO_BY_CUSTOMER stTiminginfo)
{
#ifdef UTOPIA_20
    if (pu32HDMITXInst == NULL)
    {
        if (UtopiaOpen(MODULE_HDMITX, &pu32HDMITXInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("MApi_HDMITx_SetVideoOutputTimingByCustomer UtopiaOpen HDMITX failed\n");
        }
    }

    stHDMITx_SetVideoOutputTimingByCustomer HDMITXArgs;
    HDMITXArgs.u8Mode = u8mode;
    HDMITXArgs.stTiminginfo = stTiminginfo;

    if (UtopiaIoctl(pu32HDMITXInst, E_HDMITX_CMD_SETVIDEOUTPUTTIMINGBYCUSTOMER, (void*)&HDMITXArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("MApi_HDMITx_SetVideoOutputTimingByCustomer Obtain HDMITX engine fail\n");
    }
    else
    {

    }
#else
     MApi_HDMITx_SetVideoOutputTimingByCustomer_U2(pu32HDMITXInst, u8mode, stTiminginfo);
#endif
}

// *************  For customer NDS **************//
#ifdef CUSTOMER_NDS

//17.7.7.5 Ioctl: Get Events
MS_BOOL MApi_HDMITx_Get_Events(MS_U32 *events, MS_BOOL bEraseEvent)
{
    *events = gHDMITxInfo.events;

    if(bEraseEvent)
        gHDMITxInfo.events = 0;
    return TRUE;
}

//17.7.7.8 Ioctl: Get Connection Status
MS_BOOL MApi_HDMITx_Get_Rx_CONNECTION_STATUS(HDMIConnectionStatus *status)
{
    HDMI_TX_Status Status;

    if(MDrv_HDMITx_GetRxStatus())
        status->attached = 1; // attached
    else
        status->attached = 0; // not attached

    MApi_HDMITx_GetStatus(&Status);
    if(Status.bIsRunning == TRUE)
        status->powered = 1; // powered
    else
        status->powered = 0; // not powered

    return TRUE;
}

//17.7.7.9 Ioctl: Get EEDID Block
/* This function returns 0 if success, otherwise returns non-zero */
MS_BOOL MApi_HDMITx_Get_EEDID_Block(HDMIGetEEDIDBlock *eedid)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    if(MDrv_HDMITx_EdidReadBlock(eedid->blockNumber, eedid->eedid))
        return TRUE;
    else
        return FALSE;
}

//17.7.7.10 Ioctl: Start
MS_BOOL MApi_HDMITx_Set_HDMI_Protocol(MS_U8 protocol)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    if(protocol == HDMI_PROTOCOL_DVI)
        MApi_HDMITx_SetHDMITxMode(HDMITX_DVI);
    else if(protocol == HDMI_PROTOCOL_HDMI)
        MApi_HDMITx_SetHDMITxMode(HDMITX_HDMI);

    return TRUE;
}

//17.7.7.11 Ioctl: Stop
MS_BOOL MApi_HDMITx_Stop(void)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    MApi_HDMITx_SetTMDSOnOff(FALSE);

    return TRUE;
}

//17.7.7.14 Ioctl: Set Video Code
// VIC codes: {6, 21, 2, 17, 19, 4, 20, 5, 32, 33, 34, 31, 16} => 480i60,576i50,480p60,576p50,720p50,720p60,1080i50,1080i60,1080p24,1080p25,1080p30,1080p50,1080p60
MS_BOOL MApi_HDMITx_Set_Video_Code(MS_U8 *videoCode)
{
    HDMITX_VIDEO_TIMING val = HDMITX_RES_720x480i;

    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    if(*videoCode == 1)
        val = HDMITX_RES_640x480p;
    else if(*videoCode == 6)
        val = HDMITX_RES_720x480i;
    else if(*videoCode == 21)
        val = HDMITX_RES_720x576i;
    else if(*videoCode == 2)
        val = HDMITX_RES_720x480p;
    else if(*videoCode == 17)
        val = HDMITX_RES_720x576p;
    else if(*videoCode == 19)
        val = HDMITX_RES_1280x720p_50Hz;
    else if(*videoCode == 4)
        val = HDMITX_RES_1280x720p_60Hz;
    else if(*videoCode == 20)
        val = HDMITX_RES_1920x1080i_50Hz;
    else if(*videoCode == 5)
        val = HDMITX_RES_1920x1080i_60Hz;
    else if(*videoCode == 32)
        val = HDMITX_RES_1920x1080p_24Hz;
    else if(*videoCode == 33)
        val = HDMITX_RES_1920x1080p_25Hz;
    else if(*videoCode == 34)
        val = HDMITX_RES_1920x1080p_30Hz;
    else if(*videoCode == 31)
        val = HDMITX_RES_1920x1080p_50Hz;
    else if(*videoCode == 16)
        val = HDMITX_RES_1920x1080p_60Hz;

    MApi_HDMITx_SetVideoOutputTiming(val);

    return TRUE;
}

//17.7.7.15 Ioctl: Set Pixel Format
MS_BOOL MApi_HDMITx_Set_Pixel_Format(MS_U8 *pixelFormat)
{
    HDMITX_VIDEO_COLOR_FORMAT val;

    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;
//#if 0
    if(*pixelFormat == HDMI_PIXEL_FORMAT_RGB)
        val = HDMITX_VIDEO_COLOR_RGB444;
    else if(*pixelFormat == HDMI_PIXEL_FORMAT_YCbCr422)
        val = HDMITX_VIDEO_COLOR_YUV422;
    else if(*pixelFormat == HDMI_PIXEL_FORMAT_YCbCr444)
        val = HDMITX_VIDEO_COLOR_YUV444;
    else if(*pixelFormat == HDMI_PIXEL_FORMAT_YCbCr420)
        val = HDMITX_VIDEO_COLOR_YUV420;


    MApi_HDMITx_SetColorFormat(HDMITX_VIDEO_COLOR_YUV444, val);
//#else
    MDrv_HDMITx_Set_AVI_InfoFrame(E_HDMITX_AVI_PIXEL_FROMAT, (MS_U16 *)pixelFormat);
//#endif

    return TRUE;
}

//17.7.7.16 Ioctl: Set Aspect Ratio
MS_BOOL MApi_HDMITx_Set_Aspect_Ratio(HDMIAspectRatio *aspectRatio)
{
    MS_U16 u16reg_val[6];

    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    u16reg_val[0] = ((aspectRatio->pictureAspectRatio)|(aspectRatio->activeFormatDescriptor))<<8 | ((aspectRatio->afdValidity) | (aspectRatio->barinfoValidity) | (aspectRatio->scanInfo));
    u16reg_val[1] = (aspectRatio->scalingInfo) << 8;
    if( (aspectRatio->barinfoValidity == HDMI_BARINFO_INVALID) || (aspectRatio->barinfoValidity == HDMI_BARINFO_VERTICAL_VALID))
    {
        u16reg_val[2] = 0;
        u16reg_val[3] = 0;
    }
    else
    {
        u16reg_val[2] = (aspectRatio->topBarEndsLine);
        u16reg_val[3] = (aspectRatio->bottomBarStartsLine);
    }
    if( (aspectRatio->barinfoValidity == HDMI_BARINFO_INVALID) || (aspectRatio->barinfoValidity == HDMI_BARINFO_HORIZONTAL_VALID))
    {
        u16reg_val[4] = 0;
        u16reg_val[5] = 0;
    }
    else
    {
        u16reg_val[4] = (aspectRatio->leftBarEndsPixel);
        u16reg_val[5] = (aspectRatio->rightBarStartsPixel);
    }
    MDrv_HDMITx_Set_AVI_InfoFrame(E_HDMITX_AVI_ASPECT_RATIO, u16reg_val);

    return TRUE;
}

//17.7.7.17 Ioctl: Set Colorimetry
MS_BOOL MApi_HDMITx_Set_Colorimetry(HDMIColorimetry *colorimetryData)
{
   MS_U16 u16reg_val[2];

    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    u16reg_val[0] = (colorimetryData->colorimetry) << 8;
    u16reg_val[1] = colorimetryData->extendedColorimetry;
    MDrv_HDMITx_Set_AVI_InfoFrame(E_HDMITX_AVI_COLORIMETRY, u16reg_val);

    return TRUE;
}


//17.7.7.18 Ioctl: Set AVMUTE
//17.7.7.19 Ioctl: Clear AVMUTE
MS_BOOL MApi_HDMITx_Set_AVMUTE(MS_BOOL bflag)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    MDrv_HDMITx_Set_AVMUTE(bflag);
    return TRUE;
}

//17.7.7.20 Ioctl: HDCP Start Authentication
MS_BOOL MApi_HDMITx_HDCP_Start_Authentication(void)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    //MApi_HDMITx_SetHDCPOnOff(TRUE);
    //MDrv_HDMITx_FSM_ChangeState(E_HDMITX_FSM_HDCP_AUTH_WAIT_RX);
    return MDrv_HDMITx_HDCP_Start_Authentication();

    //return 0;
}

//17.7.7.21 Ioctl: HDCP Stop Authentication
MS_BOOL MApi_HDMITx_HDCP_Stop_Authentication(void)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return -1;

    //MApi_HDMITx_SetHDCPOnOff(FALSE);
    return MDrv_HDMITx_HDCP_Stop_Authentication();

    //return 0;
}

//17.7.7.22 Ioctl: HDCP Get Bcaps
/* This function returns 0 if success, otherwise returns non-zero */
MS_BOOL MApi_HDMITx_HDCP_Get_BCaps(HDMIHDCPBCaps *arg)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    if(!MDrv_HDMITx_HDCP_Get_BCaps(&(arg->bcaps)))
        return FALSE;

    return TRUE;
}

//17.7.7.23 Ioctl: HDCP Get Bstatus
/* This function returns 0 if success, otherwise returns non-zero */
MS_BOOL MApi_HDMITx_HDCP_Get_BStatus(HDMIHDCPBStatus *arg)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    if(!MDrv_HDMITx_HDCP_Get_BStatus(&(arg->bstatus)))
        return FALSE;

    return TRUE;
}

//17.7.7.24 Ioctl: HDCP Send SRM List
MS_BOOL MApi_HDMITx_HDCP_Send_SRM(DigitalCPSrm *srm)
{
    if(MApi_HDMITx_HDCP_IsSRMSignatureValid(srm->data, srm->length))
        return TRUE;
    else
        return FALSE;
}

void MApi_HDMITx_HDCP_Reset_SRM(void)
{
    MDrv_HDMITx_HDCP_Reset_SRM();
}

//17.7.7.25 Ioctl: HDCP Encryption Control
MS_BOOL MApi_HDMITx_HDCP_Encryption_Enable(MS_U8 control)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    if(control)    // enable encryption
        MDrv_HDMITx_HdcpSetEncrypt(TRUE);
    else // disable encryption
        MDrv_HDMITx_HdcpSetEncrypt(FALSE);

    return TRUE;
}

//17.7.7.26 Ioctl: HDCP Get Status
MS_BOOL MApi_HDMITx_HDCP_Get_HDCP_Status(MS_U8 *status)
{
    if(!MDrv_HDMITx_GetRxStatus()) // RX is not alive
        return FALSE;

    MDrv_HDMITx_HDCP_Get_HDCP_Status(status);

    return TRUE;
}

//17.7.7.27 Ioctl: HDCP Configure
MS_BOOL MApi_HDMITx_HDCP_Configure(HDMIHDCPConfiguration *arg)
{
    MDrv_HDMITx_HDCP_Configure(arg);

    return TRUE;
}


#endif //CUSTOMER_NDS

