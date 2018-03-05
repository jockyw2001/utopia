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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiMHL.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_MHL_C_
#define _API_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"

#include "mdrv_mhl_st.h"
#include "apiMHL.h"
#include "halMHL.h"
#include "drvMHL.h"
#include "apiMHL_private.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MAPI_MHL_MSG_INFO(format, args...)      ULOGI("MHL", format, ##args)
#define MAPI_MHL_MSG_WARNING(format, args...)   ULOGW("MHL", format, ##args)
#define MAPI_MHL_MSG_DEBUG(format, args...)     ULOGD("MHL", format, ##args)
#define MAPI_MHL_MSG_ERROR(format, args...)     ULOGE("MHL", format, ##args)
#define MAPI_MHL_MSG_FATAL(format, args...)     ULOGF("MHL", format, ##args)

#else
#define MAPI_MHL_MSG_INFO(format, args...)      printf(format, ##args)
#define MAPI_MHL_MSG_WARNING(format, args...)   printf(format, ##args)
#define MAPI_MHL_MSG_DEBUG(format, args...)     printf(format, ##args)
#define MAPI_MHL_MSG_ERROR(format, args...)     printf(format, ##args)
#define MAPI_MHL_MSG_FATAL(format, args...)     printf(format, ##args)

#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
link_cbus_event_handle_func pfRCPCallBackFunction = NULL;
link_cbus_event_handle_func pfRAPCallBackFunction = NULL;
link_cbus_event_handle_func pfUCPCallBackFunction = NULL;
link_cbus_event_handle_func pfATTCallBackFunction = NULL;
link_cbus_event_handle_func pfRBPCallBackFunction = NULL;

void* pulMHLInst;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_MSGCallBackProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mapi_mhl_MSGCallBackProc(MS_U8 ucSubCommand, MS_U8 ucKeyCode)
{
    MS_U8 ucStatus = 0;

    switch(ucSubCommand)
    {
        case MSC_MSG_RCP:
            if(pfRCPCallBackFunction != NULL)
            {
                ucStatus = pfRCPCallBackFunction(ucKeyCode);

                MAPI_MHL_MSG_INFO("** MHL RCP callback function keycode %x status %d\r\n", ucKeyCode, ucStatus);
            }

            break;

        case MSC_MSG_RAP:
            if(pfRAPCallBackFunction != NULL)
            {
                ucStatus = pfRAPCallBackFunction(ucKeyCode);

                MAPI_MHL_MSG_INFO("** MHL RAP callback function keycode %x status %d\r\n", ucKeyCode, ucStatus);
            }

            break;

        case MSC_MSG_UCP:
            if(pfUCPCallBackFunction != NULL)
            {
                ucStatus = pfUCPCallBackFunction(ucKeyCode);

                MAPI_MHL_MSG_INFO("** MHL UCP callback function keycode %x status %d\r\n", ucKeyCode, ucStatus);
            }

            break;

        case MSC_MSG_ATT:
            if(pfATTCallBackFunction != NULL)
            {
                ucStatus = pfATTCallBackFunction(ucKeyCode);

                MAPI_MHL_MSG_INFO("** MHL ATT callback function keycode %x status %d\r\n", ucKeyCode, ucStatus);
            }

            break;

        case MSC_MSG_RBP:
            if(pfRBPCallBackFunction != NULL)
            {
                ucStatus = pfRBPCallBackFunction(ucKeyCode);

                MAPI_MHL_MSG_INFO("** MHL RBP callback function keycode %x status %d\r\n", ucKeyCode, ucStatus);
            }

            break;

        default:

            break;
    };
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_MHLSupportPath()
//  [Description]
//                  MHL support path
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_MHLSupportPath(MS_U8 ucSelect)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_MHLSupportPath(ucSelect);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    MS_BOOL bInitialFlag = FALSE;
    stMHL_SET_SUPPORT_PATH pMHLArgs = {0};

    pMHLArgs.ucSupportPath = ucSelect;

    if(pulMHLInst == NULL)
    {
        if(UtopiaOpen(MODULE_MHL, &pulMHLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_MHL_MSG_ERROR("UtopiaOpen pulMHLInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SUPPORT_PATH, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
        }
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_Init()
//  [Description]
//                  MHL init
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_init(MS_U8 *edid, MS_U8 *DevCap)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_Initial(edid, DevCap);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    MS_BOOL bInitialFlag = FALSE;
    stMHL_INITIAL_SETTING pMHLArgs = {0};

    pMHLArgs.ucEDIDTable = edid;
    pMHLArgs.ucDevcapTable = DevCap;

    if(pulMHLInst == NULL)
    {
        if(UtopiaOpen(MODULE_MHL, &pulMHLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_MHL_MSG_ERROR("UtopiaOpen pulMHLInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_INITIAL, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
        }
    }
    
#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_handler()
//  [Description]
//                  MHL handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_handler(void)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_U16 usMSGKeyInfo = 0;

    usMSGKeyInfo = mdrv_mhl_Handler();

    if(usMSGKeyInfo != 0)
    {
        _mapi_mhl_MSGCallBackProc((usMSGKeyInfo >> 8), (usMSGKeyInfo &BMASK(7:0)));
    }

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_POLLING_HANDLER pMHLArgs = {0};

    pMHLArgs.usMSGKeyInfo = 0;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_HANDLER, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    if(pMHLArgs.usMSGKeyInfo != 0)
    {
        _mapi_mhl_MSGCallBackProc((pMHLArgs.usMSGKeyInfo >> 8), (pMHLArgs.usMSGKeyInfo &BMASK(7:0)));
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_AutoSwitchHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_AutoSwitchHandler(MS_BOOL bReset, MS_U8 *ucCbusPath)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_AutoSwitchHandler(bReset, ucCbusPath);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_AUTO_SWITCH_HANDLER pMHLArgs = {0};

    pMHLArgs.bAutoSwitchFlag = FALSE;
    pMHLArgs.bResetFlag = bReset;
    pMHLArgs.ucCbusPath = ucCbusPath;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_AUTO_SWITCH, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bAutoSwitchFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SetPowerState()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mapi_mhl_SetPowerState(EN_POWER_MODE usPowerState)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SetPowerState(usPowerState);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SET_POWER_STATE pMHLArgs = {0};

    pMHLArgs.ulFailFlag = 1;
    pMHLArgs.usPowerState = usPowerState;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SET_POWER_STATE, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.ulFailFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_CbusControl(MS_U8 ucState)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_CbusControl(ucState);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_CBUS_CONTROL pMHLArgs = {0};

    pMHLArgs.ucState = ucState;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_CBUS_CONTROL, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_LoadEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_LoadEDID(MS_U8 *edid)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_LoadEDID(edid);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_LOAD_EDID pMHLArgs = {0};

    pMHLArgs.ucEDIDTable = edid;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_LOAD_EDID, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_ReadEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_ReadEDID(usSize, edid);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_READ_EDID pMHLArgs = {0};

    pMHLArgs.ucEDIDSize = usSize;
    pMHLArgs.ucEDIDTable = edid;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_READ_EDID, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_LoadDeviceCapability()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_LoadDeviceCapability(MS_U8 *devcap)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_LoadDeviceCapability(devcap);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_LOAD_DEVCAP pMHLArgs = {0};

    pMHLArgs.ucDevcapTable = devcap;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_LOAD_DEVCAP, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SetVenderID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_SetVenderID(MS_U8 ucVenderID)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_SetVenderID(ucVenderID);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SET_VENDER_ID pMHLArgs = {0};

    pMHLArgs.ucVenderID = ucVenderID;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SET_VENDER_ID, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_InvertCableDetect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_InvertCableDetect(MS_BOOL bCableDetectInvert)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_InvertCableDetect(bCableDetectInvert);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SET_CABLE_DETECT_INVERT pMHLArgs = {0};

    pMHLArgs.bCableDetectInvert = bCableDetectInvert;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SET_CABLE_DETECT_INVERT, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_VbusConfigSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_VbusConfigSetting(MS_U8 ucState)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_VbusConfigSetting(ucState);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_VBUS_CONFIG_SETTING pMHLArgs = {0};

    pMHLArgs.ucConfigMode = ucState;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_VBUS_CONFIG_SETTING, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_AdjustSettingIControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_AdjustSettingIControl(MS_U8 ucIControl)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_AdjustSettingIControl(ucIControl);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_ADJUST_I_CONTROL pMHLArgs = {0};

    pMHLArgs.ucIControlValue = ucIControl;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_ADJUST_I_CONTROL, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_AdjustImpedanceSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    mdrv_mhl_AdjustImpedanceSetting(ucImpedance);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_ADJUST_IMPEDANCE pMHLArgs = {0};

    pMHLArgs.ucImpedanceValue = ucImpedance;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_ADJUST_IMPEDANCE, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CableDetect()
//  [Description]
//                  Get MHL cable detection flag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_CableDetect(void)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_GetCableDetectFlag();

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_CABLE_DETECT_FLAG pMHLArgs = {0};

    pMHLArgs.bCableDetectFlag = FALSE;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_CABLE_DETECT_FLAG, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bCableDetectFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusStatus()
//  [Description]
//                  Get MHL CBUS connetion flag
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_CbusStatus(void)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_GetCbusConnectFlag();

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_CBUS_CONNECT_FLAG pMHLArgs = {0};

    pMHLArgs.bCbusConnectFlag = FALSE;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_CBUS_CONNECT_FLAG, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bCbusConnectFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusWakeupIntFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_CbusWakeupIntFlag(void)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_CbusWakeupIntFlag();

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_CBUS_WAKEUP_INT_FLAG pMHLArgs = {0};

    pMHLArgs.bCbusWakeupFlag = FALSE;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_CBUS_WAKEUP_INT_FLAG, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bCbusWakeupFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SrcRCPSupportFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_SrcRCPSupportFlag(void)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SrcRCPSupportFlag();

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SOURCE_SUPPORT_RCP pMHLArgs = {0};

    pMHLArgs.bSourceSupportRCP = FALSE;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SOURCE_SUPPORT_RCP, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSourceSupportRCP;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SrcRAPSupportFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_SrcRAPSupportFlag(void)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SrcRAPSupportFlag();

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SOURCE_SUPPORT_RAP pMHLArgs = {0};

    pMHLArgs.bSourceSupportRAP = FALSE;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SOURCE_SUPPORT_RAP, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSourceSupportRAP;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusGetStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_CbusGetStatusFlag(MS_U8 ucState)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_CbusGetStatusFlag(ucState);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_GET_STATUS_FLAG pMHLArgs = {0};

    pMHLArgs.bStatusFlag = FALSE;
    pMHLArgs.ucStatusType = ucState;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_GET_STATUS_FLAG, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bStatusFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendRAPCmd
//  [Description]
//                  MHL Cbus MSC Send RAP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mapi_mhl_SendRAPCmd(MS_U8 databuf)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SendRAPCommand(databuf);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SEND_RAP_COMMAND pMHLArgs = {0};

    pMHLArgs.bSendFlag = FALSE;
    pMHLArgs.ucKeyCode = databuf;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SEND_RAP_COMMAND, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSendFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendRCPAutoReleaseCmd
//  [Description]
//                  MHL Cbus MSC Send RCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mapi_mhl_SendRCPAutoReleaseCmd(MS_U8 ucKeyCode)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SendRCPAutoReleaseCmd(ucKeyCode);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SEND_RCP_COMMAND pMHLArgs = {0};

    pMHLArgs.bSendFlag = FALSE;
    pMHLArgs.ucKeyCode = ucKeyCode;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SEND_RCP_COMMAND, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSendFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendUCPCmd
//  [Description]
//                  MHL Cbus MSC Send UCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mapi_mhl_SendUCPCmd(MS_U8 databuf)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SendUCPCommand(databuf);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SEND_UCP_COMMAND pMHLArgs = {0};

    pMHLArgs.bSendFlag = FALSE;
    pMHLArgs.ucKeyCode = databuf;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SEND_UCP_COMMAND, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSendFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_SendWriteBurst
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mapi_mhl_SendWriteBurst(MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *buffer)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_SendWriteBurst(ucAddr, ucLength, buffer);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SEND_WRITE_BURST pMHLArgs = {0};

    pMHLArgs.bSendFlag = FALSE;
    pMHLArgs.ucAddress = ucAddr;
    pMHLArgs.ucLength = ucLength;
    pMHLArgs.ucData = buffer;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SEND_WRITE_BURST, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSendFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusSendUserWriteBurst
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL SYMBOL_WEAK mapi_mhl_CbusSendUserWriteBurst(MS_U8 ucLength, MS_U8 *pBuffer)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_CbusSendUserWriteBurst(ucLength, pBuffer);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_SEND_USER_WRITE_BURST pMHLArgs = {0};

    pMHLArgs.bSendFlag = FALSE;
    pMHLArgs.ucLength = ucLength;
    pMHLArgs.ucData = pBuffer;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_SEND_USER_WRITE_BURST, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bSendFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetDeviceCapacibility
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_GetDeviceCapacibility(MS_U16 usDevcapMask, MS_U8 *ucDevcap)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_GetDeviceCapability(usDevcapMask, ucDevcap);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_GET_DEVICE_CAPABILITY pMHLArgs = {0};

    pMHLArgs.bObtainFlag = FALSE;
    pMHLArgs.usDevcapMask = usDevcapMask;
    pMHLArgs.ucDeviceCapability = ucDevcap;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_GET_DEVICE_CAPABILITY, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bObtainFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetExtendDeviceCapacibility
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL SYMBOL_WEAK mapi_mhl_GetExtendDeviceCapacibility(MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_GetExtendDeviceCapability(usExtendDevcapMask, ucExtendDevcap);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_GET_EXTEND_DEVICE_CAPABILITY pMHLArgs = {0};

    pMHLArgs.bObtainFlag = FALSE;
    pMHLArgs.usExtendDevcapMask = usExtendDevcapMask;
    pMHLArgs.ucExtendDeviceCapability = ucExtendDevcap;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_GET_EXTEND_DEVICE_CAPABILITY, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bObtainFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetDeviceVenderID
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_GetDeviceVenderID(MS_U8 *ucVenderID)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_GetDeviceVenderID(ucVenderID);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_GET_VENDER_ID pMHLArgs = {0};

    pMHLArgs.bObtainFlag = FALSE;
    pMHLArgs.ucVenderID = ucVenderID;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_GET_VENDER_ID, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bObtainFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetWriteBurstData
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mapi_mhl_GetWriteBurstData(MS_U8 *ucData)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    return mdrv_mhl_GetWriteBurstData(ucData);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_GET_WRITE_BURST_DATA pMHLArgs = {0};

    pMHLArgs.bObtainFlag = FALSE;
    pMHLArgs.ucWriteBurstData = ucData;

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_GET_WRITE_BURST_DATA, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

    return pMHLArgs.bObtainFlag;

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_RegisterCallBackFunctions()
//  [Description]
//                  RCP &RAP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_RegisterCallBackFunctions(link_cbus_event_handle_func rcp, link_cbus_event_handle_func rap)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bRCPfunctionFlag = FALSE;
    MS_BOOL bRAPfunctionFlag = FALSE;

    if(rcp != NULL)
    {
        bRCPfunctionFlag = TRUE;

        pfRCPCallBackFunction = rcp;
    }

    if(rap != NULL)
    {
        bRAPfunctionFlag = TRUE;

        pfRAPCallBackFunction = rap;
    }

    mdrv_mhl_RegisterCallBackFunctions(bRCPfunctionFlag, bRAPfunctionFlag);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_CALL_BACK_FUNCTION pMHLArgs = {0};

    if(rcp == NULL)
    {
        pMHLArgs.bRCPfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bRCPfunctionFlag = TRUE;

        pfRCPCallBackFunction = rcp;
    }

    if(rap == NULL)
    {
        pMHLArgs.bRAPfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bRAPfunctionFlag = TRUE;

        pfRAPCallBackFunction = rap;
    }

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_CALL_BACK_FUNCTION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_RegisterRcpCallBackFunction()
//  [Description]
//                  RCP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_RegisterRcpCallBackFunction(link_cbus_event_handle_func rcp)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bRCPfunctionFlag = FALSE;

    if(rcp != NULL)
    {
        bRCPfunctionFlag = TRUE;

        pfRCPCallBackFunction = rcp;
    }

    mdrv_mhl_RegisterRcpCallBackFunction(bRCPfunctionFlag);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_RCP_CALL_BACK_FUNCTION pMHLArgs = {0};

    if(rcp == NULL)
    {
        pMHLArgs.bRCPfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bRCPfunctionFlag = TRUE;

        pfRCPCallBackFunction = rcp;
    }

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_RCP_CALL_BACK_FUNCTION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_RegisterRapCallBackFunction()
//  [Description]
//                  RAP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_RegisterRapCallBackFunction(link_cbus_event_handle_func rap)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bRAPfunctionFlag = FALSE;

    if(rap != NULL)
    {
        bRAPfunctionFlag = TRUE;

        pfRCPCallBackFunction = rap;
    }

    mdrv_mhl_RegisterRapCallBackFunction(bRAPfunctionFlag);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_RAP_CALL_BACK_FUNCTION pMHLArgs = {0};

    if(rap == NULL)
    {
        pMHLArgs.bRAPfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bRAPfunctionFlag = TRUE;

        pfRAPCallBackFunction = rap;
    }

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_RAP_CALL_BACK_FUNCTION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_RegisterUcpCallBackFunction()
//  [Description]
//                  UCP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_RegisterUcpCallBackFunction(link_cbus_event_handle_func Ucp)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bUCPfunctionFlag = FALSE;

    if(Ucp != NULL)
    {
        bUCPfunctionFlag = TRUE;

        pfRCPCallBackFunction = Ucp;
    }

    mdrv_mhl_RegisterUcpCallBackFunction(bUCPfunctionFlag);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_UCP_CALL_BACK_FUNCTION pMHLArgs = {0};

    if(Ucp == NULL)
    {
        pMHLArgs.bUCPfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bUCPfunctionFlag = TRUE;

        pfUCPCallBackFunction = Ucp;
    }

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_UCP_CALL_BACK_FUNCTION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_RegisterAttCallBackFunction()
//  [Description]
//                  ATT call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void SYMBOL_WEAK mapi_mhl_RegisterAttCallBackFunction(link_cbus_event_handle_func Att)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bATTfunctionFlag = FALSE;

    if(Att != NULL)
    {
        bATTfunctionFlag = TRUE;

        pfRCPCallBackFunction = Att;
    }

    mdrv_mhl_RegisterAttCallBackFunction(bATTfunctionFlag);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_ATT_CALL_BACK_FUNCTION pMHLArgs = {0};

    if(Att == NULL)
    {
        pMHLArgs.bATTfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bATTfunctionFlag = TRUE;

        pfATTCallBackFunction = Att;
    }

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_ATT_CALL_BACK_FUNCTION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_RegisterRbpCallBackFunction()
//  [Description]
//                  RBP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void SYMBOL_WEAK mapi_mhl_RegisterRbpCallBackFunction(link_cbus_event_handle_func Rbp)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bRBPfunctionFlag = FALSE;

    if(Rbp != NULL)
    {
        bRBPfunctionFlag = TRUE;

        pfRCPCallBackFunction = Rbp;
    }

    mdrv_mhl_RegisterRbpCallBackFunction(bRBPfunctionFlag);

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    stMHL_RBP_CALL_BACK_FUNCTION pMHLArgs = {0};

    if(Rbp == NULL)
    {
        pMHLArgs.bRBPfunctionFlag = FALSE;
    }
    else
    {
        pMHLArgs.bRBPfunctionFlag = TRUE;

        pfRBPCallBackFunction = Rbp;
    }

    if(pulMHLInst == NULL)
    {
        MAPI_MHL_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_mhl_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_RBP_CALL_BACK_FUNCTION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
    }

#endif
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_GetConfig()
//  [Description]
//                  MHL get init config
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mapi_mhl_GetConfig(stMHL_INITIAL_CONFIG *stInitialConfig)
{
    MS_U8 uctemp = 0;
    MS_U32 ulReturnValue = UTOPIA_STATUS_ERR_NOT_AVAIL;

#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    stMHL_INITIAL_CONFIG_INFO stInitialConfigInfo;

    stInitialConfigInfo = mdrv_mhl_GetConfiguration();

    stInitialConfig->ucSupportPath = stInitialConfigInfo.ucMHLSupportPath;
    stInitialConfig->ucEDIDTable = NULL;

    if(stInitialConfig->ucDevcapTable != NULL)
    {
        for(uctemp = 0; uctemp < (MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE); uctemp++)
        {
            stInitialConfig->ucDevcapTable[uctemp] = stInitialConfigInfo.ucDeviceCapabilityTable[uctemp];
        }
    }

    ulReturnValue = stInitialConfigInfo.ulReturnValue;

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    MS_BOOL bInitialFlag = FALSE;
    stMHL_GET_CONFIGURATION pMHLArgs;

    pMHLArgs.stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_ERR_NOT_AVAIL;

    if(pulMHLInst == NULL)
    {
        if(UtopiaOpen(MODULE_MHL, &pulMHLInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_MHL_MSG_ERROR("UtopiaOpen pulMHLInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulMHLInst, MAPI_CMD_MHL_GET_CONFIGURATION, (void*)&pMHLArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_MHL_MSG_ERROR("Obtain MHL engine fail\n");
        }
        else
        {
            stInitialConfig->ucSupportPath = pMHLArgs.stInitialConfigInfo.ucMHLSupportPath;
            stInitialConfig->ucEDIDTable = NULL;

            if(stInitialConfig->ucDevcapTable != NULL)
            {
                for(uctemp = 0; uctemp < (MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE); uctemp++)
                {
                    stInitialConfig->ucDevcapTable[uctemp] = pMHLArgs.stInitialConfigInfo.ucDeviceCapabilityTable[uctemp];
                }
            }

            ulReturnValue = pMHLArgs.stInitialConfigInfo.ulReturnValue;
        }
    }

#endif

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_Send3DInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_Send3DInformation(void)
{
    // Remove
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_CbusWakeupIntSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_CbusWakeupIntSetting(void)
{
    // Remove
}

//**************************************************************************
//  [Function Name]:
//                  mapi_mhl_PowerCtrl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mapi_mhl_PowerCtrl(E_MHL_POWERCTRL_Mode pctrl)
{
    // Remove
    pctrl = E_MHL_POWER_DOWN;
}

#endif // _MAPI_MHL_C_
