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
/// file    msAPI_CEC.c
/// @author MStar Semiconductor Inc.
/// @brief  HDMI CEC API Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#define _APICEC_C_

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "utopia.h"
#include "utopia_dapi.h"

#include "apiCEC.h"
#include "drvCEC.h"
#include "apiCEC_private.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MAPI_CEC_MSG_INFO(format, args...)       ULOGI("CEC", format, ##args)
#define MAPI_CEC_MSG_WARNING(format, args...)    ULOGW("CEC", format, ##args)
#define MAPI_CEC_MSG_DEBUG(format, args...)      ULOGD("CEC", format, ##args)
#define MAPI_CEC_MSG_ERROR(format, args...)      ULOGE("CEC", format, ##args)
#define MAPI_CEC_MSG_FATAL(format, args...)      ULOGF("CEC", format, ##args)

#else
#define MAPI_CEC_MSG_INFO(format, args...)       printf(format, ##args)
#define MAPI_CEC_MSG_WARNING(format, args...)    printf(format, ##args)
#define MAPI_CEC_MSG_DEBUG(format, args...)      printf(format, ##args)
#define MAPI_CEC_MSG_ERROR(format, args...)      printf(format, ##args)
#define MAPI_CEC_MSG_FATAL(format, args...)      printf(format, ##args)

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
static MSIF_Version _api_cec_version = {
    .DDI = { CEC_API_VERSION },
};

static MS_CEC_Status _cec_status = {.bIsInitialized = FALSE, .bIsRunning = FALSE,};
static MS_U16 _u16CEC_DBG_SWITCH = FALSE;

void* pulCECInst;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_PortSelcet()
//  [Description]
//                   Select CEC bank mapping
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void SYMBOL_WEAK MApi_CEC_PortSelcet(MsCEC_INPUT_PORT InputPort)
{
    stCEC_PORT_SELECT pCECArgs = {0};

    pCECArgs.InputPort = InputPort;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_PORT_SELECT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetLibVer()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_CEC_GetLibVer(const MSIF_Version **ppVersion)              ///< Get version
{
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_api_cec_version;
    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_DDC2BI_GetInfo()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_DDC2BI_GetInfo(MS_CEC_INFO_LIST *pInfo)
{
    stCEC_DDC2BI_GET_INFO pCECArgs = {0};

    pCECArgs.eInfo = pInfo;
    pCECArgs.bGetInfo = FALSE;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_DDC2BI_GET_INFO, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.bGetInfo;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_DDC2BI_GetStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_DDC2BI_GetStatus(MS_CEC_Status *pStatus)
{
    if (!pStatus)
    {
        return FALSE;
    }

    memcpy((void*)pStatus, (void*)&_cec_status, sizeof(MS_CEC_Status ));

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_DDC2BI_SetDbgLevel()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_DDC2BI_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    _u16CEC_DBG_SWITCH = u16DbgSwitch;
    //_u16CEC_DBG_SWITCH = _u16CEC_DBG_SWITCH;
    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Exit()
//  [Description]
//                   Mask CEC interrupt and release CEC
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_Exit(void)
{
    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_EXIT, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Init()
//  [Description]
//                  CEC initial sequence
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_Init(MS_CEC_INIT_INFO *pCECInfo)
{
    stCEC_INIT pCECArgs = {0};

    pCECArgs.u32XTAL_CLK_Hz = pCECInfo->u32XTAL_CLK_Hz;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_INIT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_InitChip()
//  [Description]
//                  CEC chip relatived part initial (Initiation)
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_InitChip(MS_CEC_INIT_INFO *pCECInfo)
{
    MS_BOOL bInitialFlag = FALSE;
    stCEC_INIT_CHIP pCECArgs = {0};

    pCECArgs.u32XTAL_CLK_Hz = pCECInfo->u32XTAL_CLK_Hz;

    if(pulCECInst == NULL)
    {
        if(UtopiaOpen(MODULE_CEC, &pulCECInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_CEC_MSG_ERROR("UtopiaOpen pulCECInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_INIT_CHIP, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetMyLogicalAddress()
//  [Description]
//                  CEC set own logical address
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetMyLogicalAddress(MsCEC_DEVICELA myLA)
{
    stCEC_SET_MY_LOGICAL_ADDRESS pCECArgs = {0};

    pCECArgs.myLA = myLA;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_MY_LOGICAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MApi_CEC_ChkRxBuf()
//  [Description]
//                  Use to store CEC received messages
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_ChkRxBuf(void)
{
    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_CHK_RX_BUF, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_TxSendMsg()
//  [Description]
//                   Use to send any CEC message by user defined
//                   If msg = E_MSG_UI_PRESS, than will send msg E_MSG_UI_RELEASE immediately
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg:            CEC message
//                   cmd:            message parameters
//                   len:              parameter length
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_TxSendMsg(MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8 *cmd, MS_U8 len)
{
    stCEC_TX_SEND_MSG pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eMsg = msg;
    pCECArgs.ucCmd = cmd;
    pCECArgs.ucLen = len;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_TX_SEND_MSG, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_TxSendMsg2()
//  [Description]
//                   Use to send any CEC message by user defined
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg:            CEC message
//                   cmd:            message parameters
//                   len:              parameter length
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_TxSendMsg2(MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* cmd, MS_U8 len)
{
    stCEC_TX_SEND_MSG2 pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eMsg = msg;
    pCECArgs.ucCmd = cmd;
    pCECArgs.ucLen = len;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_TX_SEND_MSG2, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecTxSendPollingMsg()
//  [Description]
//                   Use to send any CEC polling msg by user defined
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MsAPI_CecTxSendPollingMsg(MsCEC_DEVICELA dst_address)
{
    stCEC_TX_SEND_POLLING_MSG pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_TX_SEND_POLLING_MSG, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ActiveSource()
//  [Description]
//                   Used by a new source to indicate that it has started to transmit a stream or
//                   used in response to a <Request Active Source>
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  All sources
//                   Follower:  TV, CEC Switches
//  [Arguments]:
//
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ActiveSource(void)
{
    stCEC_TX_SEND_POLLING_MSG pCECArgs = {0};

    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_ACTIVE_SOURCE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_RoutingChange()
//  [Description]
//                   Used by a CEC switch when it's manually switched to inform akk other devices.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  CEC switches and TV(with 2 or more HDMI inputs)
//                   Follower:  CEC switches
//  [Arguments]:
//                   orig_address: The original physicall address
//                   new_address: The new physical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_RoutingChange(MS_U8* orig_address, MS_U8* new_address)
{
    stCEC_MSG_ROUTING_CHANGE pCECArgs = {0};

    pCECArgs.ucOrigAddr = orig_address;
    pCECArgs.ucNewAddr = new_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_ROUTING_CHANGE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqActiveSource()
//  [Description]
//                   Used by a new device to discover the status of the system.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:
//                   Follower:  All, except for CEC Switches and devices which cannot become a source.
//  [Arguments]:
//
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReqActiveSource(void)
{
    stCEC_MSG_REQ_ACTIVE_SOURCE pCECArgs = {0};

    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_ACTIVE_SOURCE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_SetStreamPath()
//  [Description]
//                   Used by the TV to request a streaming path from the specified physical address.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:
//                   Follower:  CEC Switches
//  [Arguments]:
//                   new_address: The new physical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_SetStreamPath(MS_U8* new_address)
{
    stCEC_MSG_SET_STREAM_PATH pCECArgs = {0};

    pCECArgs.ucNewAddr = new_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_SET_STREAM_PATH, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_Standby()
//  [Description]
//                   Use to switch one or all devices into standby mode.
//                   One device: it's specific logical address, like "E_LA_RECORDER1"
//                   All device:   Use broadcast logical address, "E_LA_BROADCAST"
//  [Message property]
//                   Address:  Directly / Broadcast
//                   Initiator:  TV(Broadcast Address)
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_Standby(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_STANDBY pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_STANDBY, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_RecordOff()
//  [Description]
//                   Requests a device to stop a recording.
//  [Message property]
//                   Address:  Directly
//                   Initiator:  Device Initiating a recording
//                   Follower:  Recording Device if implementing <Record On>
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_RecordOff(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_RECORD_OFF pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_RECORD_OFF, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_RecordOn()
//  [Description]
//                   Attempt to record the specified source.
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MsAPI_CEC_Msg_RecordOn(MsCEC_DEVICELA dst_address, MsCEC_MSG_RECORD_SOURCE_TYPE cmd)
{
    stCEC_MSG_RECORD_ON pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_RECORD_ON, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReportCECVersion()
//  [Description]
//                   Used to indicate the supported CEC version, in response to a <Get CEC Version>
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//                   version: CEC version
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReportCECVersion(MsCEC_DEVICELA dst_address, MS_U8 version)
{
    stCEC_MSG_REPORT_CEC_VERSION pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.ucVersion = version;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REPORT_CEC_VERSION, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqCECVersion()
//  [Description]
//                   Used by a device to enquire which version of CEC the target supports
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReqCECVersion(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_REQ_CEC_VERSION pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_CEC_VERSION, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReportPhycalAddress()
//  [Description]
//                   Use to inform all other devices of the mapping between physical and logical address of the initiator
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  All
//                   Follower:  TV
//  [Arguments]:
//
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReportPhycalAddress(void)
{
    stCEC_MSG_REPORT_PHYCAL_ADDRESS pCECArgs = {0};

    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REPORT_PHYCAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqPhycalAddress()
//  [Description]
//                   A request to a device to return its physical address
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  All, except for CEC Switches using logical address 15
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReqPhycalAddress(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_REQ_PHYCAL_ADDRESS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_PHYCAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_DeckControl()
//  [Description]
//                   Use to control a device's media functions
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      deck control mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_DeckControl(MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_CTRL_PARM cmd)
{
    stCEC_MSG_DECK_CONTROL pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_DECK_CONTROL, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_DeckStatus()
//  [Description]
//                   Use to provide a device's status when received "Give Deck Status"
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      deck info mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_DecStatus(MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_STATUS_PARM cmd)
{
    stCEC_MSG_DEC_STATUS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_DEC_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_GiveDeckStatus()
//  [Description]
//                   Use to request device's status
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      status request mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_MSg_GiveDeckStatus(MsCEC_DEVICELA dst_address, MsCEC_MSG_STATUS_REQUEST_PARM cmd)
{
    stCEC_MSG_GIVE_DECK_STATUS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_GIVE_DECK_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_DCPlay()
//  [Description]
//                   Use to control the playback behaviour of a source device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      play mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_MSg_DCPlay(MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_PLAY_PARM cmd)
{
    stCEC_MSG_DC_PLAY pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_DC_PLAY, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqMenuStatus()
//  [Description]
//                   Request from TV for a device to show/remove a menu or to query status
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd: menu request type
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReqMenuStatus(MsCEC_DEVICELA dst_address, MsCEC_MSG_MENU_REQ_TYPE cmd)
{
    stCEC_MSG_REQ_MENU_STATUS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_MENU_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_UserCtrlPressed()
//  [Description]
//                   Use to indicated that user pressed which control button
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd: UI command
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_UserCtrlPressed(MS_BOOL bUserCtrlEn, MsCEC_DEVICELA dst_address, MsCEC_MSG_USER_CTRL_PARM cmd)
{
    stCEC_MSG_USER_CTRL_PRESSED pCECArgs = {0};

    pCECArgs.bUserCtrlEn = bUserCtrlEn;
    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_USER_CTRL_PRESSED, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_UserCtrlReleased()
//  [Description]
//                   Use to indicated that user released which control button
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_UserCtrlReleased(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_USER_CTRL_RELEASED pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_USER_CTRL_RELEASED, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReportPowerStatus()
//  [Description]
//                   Use to inform a requesting device of the current power status
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All (except CEC switches)
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReportPowerStatus(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_REPORT_POWER_STATUS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REPORT_POWER_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqPowerStatus()
//  [Description]
//                   Use to determine the current power status of a target device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  All (except CEC switches)
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReqPowerStatus(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_REQ_POWER_STATUS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_POWER_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_FeatureAbort()
//  [Description]
//                   Use to indicate that device doesn't support message type
//  [Message property]
//                   Address:  Directly
//                   Initiator:  Generate if a message is not supported
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg: the requested message type
//                   cmd: unsupport reason
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_FeatureAbort(MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MsCEC_MSG_ABORT_REASON cmd)
{
    stCEC_MSG_FEATURE_ABORT pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eMsg = msg;
    pCECArgs.eCmd = cmd;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_FEATURE_ABORT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_Abort()
//  [Description]
//                   This message is reserved for testing purposes.
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  All except for CEC switches
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_Abort(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_ABORT pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_ABORT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Msg_SendMenuLanguage()
//  [Description]
//                   Used by a CEC switch when it's manually switched language to inform akk other devices.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  CEC switches and TV(with 2 or more HDMI inputs)
//                   Follower:  CEC switches
//  [Arguments]:
//                   MenuLanguageCode: iso639-2 Language Code, 3 bytes
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_SendMenuLanguage(MS_U8* pu8MenuLanguageCode)
{
    stCEC_MSG_SEND_MENU_LANGUAGE pCECArgs = {0};

    pCECArgs.pu8MenulanguageCode = pu8MenuLanguageCode;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_SEND_MENU_LANGUAGE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqARCInitiation()
//  [Description]
//                   Used by an ARC TX device to request an ARC RX device to active
//                   the ARC functionility in ARC TX device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  ARC RX device
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MsAPI_CecMsg_ReqARCInitiation(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_REQ_ARC_INITIATION pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_ARC_INITIATION, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_ReqARCTermination()
//  [Description]
//                   Used by an ARC TX device to request an ARC RX device to deactive
//                   the ARC functionility in ARC TX device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  ARC RX device
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MsAPI_CecMsg_ReqARCTermination(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_REQ_ARC_TERMINATION pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REQ_ARC_TERMINATION, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MsAPI_CecMsg_AudioModeReq()
//  [Description]
//                   A device implementing System Audio Control and which has volume control
//                   RC buttons request to use System Audio Mode to the Amplifier
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   amp_switch: 1: unmute amplifier. 0: mute amplifier
//                   my_address: source to be used is the device specified at this address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MsAPI_CecMsg_AudioModeReq(MsCEC_DEVICELA dst_address, MS_BOOL amp_switch, MS_U8* my_address )
{
    stCEC_MSG_AUDIO_MODE_REQ pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.bAmpSwitch = amp_switch;
    pCECArgs.ucMyAddress = my_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_AUDIO_MODE_REQ, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Msg_GiveAudioStatus()
//  [Description]
//                   Use to request an Amplifier to send its volume and mute status
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All (except CEC switches)
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_GiveAudioStatus(MsCEC_DEVICELA dst_address)
{
    stCEC_MSG_GIVE_AUDIO_STATUS pCECArgs = {0};

    pCECArgs.eDetAddr = dst_address;
    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_GIVE_AUDIO_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Get_Header()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_Get_Header(void)
{
    stCEC_GET_HEADER pCECArgs = {0};

    pCECArgs.ucValue = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_HEADER, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucValue;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Get_OpCode()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_Get_OpCode(void)
{
    stCEC_GET_OPCODE pCECArgs = {0};

    pCECArgs.ucValue = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_OPCODE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucValue;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Get_Para()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_Get_Para(MS_U8 u8Idx)
{
    stCEC_GET_PARA pCECArgs = {0};

    pCECArgs.ucIdx = u8Idx;
    pCECArgs.ucValue = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_PARA, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucValue;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetCmdLen()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_GetCmdLen(void)
{
    stCEC_GET_CMD_LEN pCECArgs = {0};

    pCECArgs.ucValue = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_CMD_LEN, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucValue;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetActiveLogicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetActiveLogicalAddress(MsCEC_DEVICELA Addr)
{
    stCEC_SET_ACTIVE_LOGICAL_ADDRESS pCECArgs = {0};

    pCECArgs.eAddr = Addr;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_ACTIVE_LOGICAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetActiveLogicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MsCEC_DEVICELA MApi_CEC_GetActiveLogicalAddress(void)
{
    stCEC_GET_ACTIVE_LOGICAL_ADDRESS pCECArgs = {0};

    pCECArgs.eAddr = E_LA_UNREGISTERED;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_ACTIVE_LOGICAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eAddr;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MsCEC_MSG_POWER_STATUS_PARM MApi_CEC_GetPowerStatus(void)
{
    stCEC_GET_POWER_STATUS pCECArgs = {0};

    pCECArgs.ePowerStatus = E_MSG_PWRSTA_UNKNOW;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_POWER_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ePowerStatus;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetFifoIdx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_GetFifoIdx(void)
{
    stCEC_GET_FIFO_IDX pCECArgs = {0};

    pCECArgs.ucFifoIdx = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_FIFO_IDX, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucFifoIdx;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetFifoIdx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetFifoIdx(MS_U8 u8Idx)
{
    stCEC_SET_FIFO_IDX pCECArgs = {0};

    pCECArgs.ucIdx = u8Idx;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_FIFO_IDX, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetActivePowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetActivePowerStatus(MsCEC_MSG_POWER_STATUS_PARM Status)
{
    stCEC_SET_ACTIVE_POWER_STATUS pCECArgs = {0};

    pCECArgs.eStatus = Status;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_ACTIVE_POWER_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetActivePowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MsCEC_MSG_POWER_STATUS_PARM MApi_CEC_GetActivePowerStatus(void)
{
    stCEC_GET_ACTIVE_POWER_STATUS pCECArgs = {0};

    pCECArgs.ePowerStatus = E_MSG_PWRSTA_UNKNOW;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_ACTIVE_POWER_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ePowerStatus;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetActivePhysicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetActivePhysicalAddress(MS_U8 u8Para1, MS_U8 u8Para2)
{
    stCEC_SET_ACTIVE_PHYSICAL_ADDRESS pCECArgs = {0};

    pCECArgs.ucPara1 = u8Para1;
    pCECArgs.ucPara2 = u8Para2;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_ACTIVE_PHYSICAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetActiveDeviceCECVersion()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetActiveDeviceCECVersion(MS_U8 u8Ver)
{
    stCEC_SET_ACTIVE_DEVICE_CEC_VERSION pCECArgs = {0};

    pCECArgs.ucVer = u8Ver;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_ACTIVE_DEVICE_CEC_VERSION, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetActiveDeviceType()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetActiveDeviceType(MsCEC_DEVICE_TYPE Type)
{
    stCEC_SET_ACTIVE_DEVICE_TYPE pCECArgs = {0};

    pCECArgs.eType = Type;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_ACTIVE_DEVICE_TYPE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetMsgCnt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_GetMsgCnt(void)
{
    stCEC_GET_MSG_CNT pCECArgs = {0};

    pCECArgs.ucMsgCnt = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_MSG_CNT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucMsgCnt;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetMsgCnt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetMsgCnt(MS_U8 u8Cnt)
{
    stCEC_SET_MSG_CNT pCECArgs = {0};

    pCECArgs.ucCnt = u8Cnt;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_MSG_CNT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetRxData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_GetRxData(MS_U8 u8Fifoidx, MS_U8 u8Idx)
{
    stCEC_GET_RX_DATA pCECArgs = {0};

    pCECArgs.ucFifoIdx = u8Fifoidx;
    pCECArgs.ucIdx = u8Idx;
    pCECArgs.ucRxData = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_RX_DATA, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucRxData;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_CheckFrame()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_CEC_CheckFrame(MsCEC_MSG_TRANS_TYPE msg_type, MS_U8 ucLen)
{
    stCEC_CHECK_FRAME pCECArgs = {0};

    pCECArgs.eMsgType = msg_type;
    pCECArgs.ucLen = ucLen;
    pCECArgs.bValid = FALSE;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_CHECK_FRAME, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.bValid;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_CheckExistDevices()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_CheckExistDevices(void)
{
    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_CHECK_EXIST_DEVICES, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_ConfigWakeUp()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_ConfigWakeUp(void)
{
    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_CONFIG_WAKEUP, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Enabled()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_Enabled(MS_BOOL bEnableFlag)
{
    stCEC_ENABLED pCECArgs = {0};

    pCECArgs.bEnableFlag = bEnableFlag;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_ENABLED, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetMyPhysicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetMyPhysicalAddress(MS_U8 *pdata)
{
    stCEC_SET_MY_PHYSICAL_ADDRESS pCECArgs = {0};

    pCECArgs.ucData = pdata;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_MY_PHYSICAL_ADDRESS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

#if ENABLE_CEC_MULTIPLE
//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetMyLogicalAddress2()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetMyLogicalAddress2(MsCEC_DEVICELA myLA)
{
    stCEC_SET_MY_LOGICAL_ADDRESS2 pCECArgs = {0};

    pCECArgs.myLA = myLA;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_MY_LOGICAL_ADDRESS2, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Msg_ReportPhycalAddress2()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
CEC_ERROR_CODE MApi_CEC_Msg_ReportPhycalAddress2(void)
{
    stCEC_MSG_REPORT_PHYCAL_ADDRESS2 pCECArgs = {0};

    pCECArgs.eErrorCode = E_CEC_FEATURE_ABORT;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_MSG_REPORT_PHYCAL_ADDRESS2, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eErrorCode;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetMyPhysicalAddress2()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetMyPhysicalAddress2(MS_U8 *pdata)
{
    stCEC_SET_MY_PHYSICAL_ADDRESS2 pCECArgs = {0};

    pCECArgs.ucData = pdata;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_MY_PHYSICAL_ADDRESS2, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetInitiator()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetInitiator(MsCEC_DEVICELA IniLA)
{
    stCEC_SET_INITIATOR pCECArgs = {0};

    pCECArgs.eIniLa = IniLA;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_INITIATOR, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

#endif

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_GetTxStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MApi_CEC_GetTxStatus(void)
{
    stCEC_GET_TX_STATUS pCECArgs = {0};

    pCECArgs.ucTxStatus = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_TX_STATUS, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.ucTxStatus;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetPowerState()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 MApi_CEC_SetPowerState(EN_POWER_MODE u16PowerState)
{
    stCEC_SET_POWER_STATE pCECArgs = {0};

    pCECArgs.ePowerState = u16PowerState;
    pCECArgs.u32Status = 0;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_POWER_STATE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.u32Status;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_SetRetryCount()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_SetRetryCount(MS_U8 u8RetryCount)
{
    stCEC_SET_RETRY_COUNT pCECArgs = {0};

    pCECArgs.ucRetryCount = u8RetryCount;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_SET_RETRY_COUNT, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_ConfigWakeupInfoVendorID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_ConfigWakeupInfoVendorID(MS_U8 *u8CecVendorID)
{
    stCEC_CONFIG_WAKEUP_INFO_VENDOR_ID pCECArgs = {0};

    pCECArgs.ucVendorID = u8CecVendorID;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_CONFIG_WAKEUP_INFO_VENDOR_ID, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_NextDevice()
//  [Description]
//                   Set logical address to next active device
//  [Arguments]:
//
//  [Return]:
//                   Next CEC active device's logical address
//**************************************************************************
MsCEC_DEVICELA MApi_CEC_NextDevice(void)
{
    stCEC_NEXT_DEVICE pCECArgs = {0};

    pCECArgs.eNextDeviceLA = E_LA_UNREGISTERED;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_NEXT_DEVICE, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.eNextDeviceLA;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_IsRxBufEmpty()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_CEC_IsRxBufEmpty(void)
{
    stCEC_IS_RX_BUF_EMPTY pCECArgs = {0};

    pCECArgs.bEmpty = FALSE;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_IS_RX_BUF_EMPTY, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.bEmpty;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_CheckDeviceIsTx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_CEC_CheckDeviceIsTx(void)
{
    stCEC_DEVICE_IS_TX pCECArgs = {0};

    pCECArgs.bIsTx = FALSE;

    if(pulCECInst == NULL)
    {
        MAPI_CEC_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling mapi_cec_initchip() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_CHECK_DEVICE_IS_TX, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
    }

    return pCECArgs.bIsTx;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_AttachDriverISR()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MApi_CEC_AttachDriverISR(MS_BOOL bAttachDrvFlag)
{
    MS_BOOL bInitialFlag = FALSE;
    stCEC_ATTACH_DRIVER_ISR pCECArgs = {0};

    pCECArgs.bAttachDrvFlag = bAttachDrvFlag;

    if(pulCECInst == NULL)
    {
        if(UtopiaOpen(MODULE_CEC, &pulCECInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_CEC_MSG_ERROR("UtopiaOpen pulCECInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_ATTACH_DRIVER_ISR, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_Ctrl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MApi_CEC_Ctrl(MS_U32 u32Cmd, void *pBuf, MS_U32 u32BufSize)
{
    MS_BOOL bret = FALSE;

    switch(u32Cmd)
    {
        case E_CEC_CMD_ATTACH_DRIVER_ISR:
            {
                if(u32BufSize == sizeof(stCMD_CEC_ATTACH_DRIVER_ISR))
                {
                    MApi_CEC_AttachDriverISR(((stCMD_CEC_ATTACH_DRIVER_ISR *)pBuf)->bAttachDrvFlag);

                    bret = TRUE;
                }
            }
            break;

        default:
            pBuf = pBuf;
            u32BufSize = u32BufSize;
            bret = FALSE;
            break;
    }

    return bret;
}

//**************************************************************************
//  [Function Name]:
//                  mapi_CEC_GetConfig()
//  [Description]
//                  CEC get init config
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mapi_CEC_GetConfig(stCEC_INITIAL_CONFIG *stInitialConfig)
{
    MS_BOOL bInitialFlag = FALSE;
    MS_U32 ulReturnValue = UTOPIA_STATUS_ERR_NOT_AVAIL;
    stCEC_GET_CONFIGURATION pCECArgs;

    pCECArgs.stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_ERR_NOT_AVAIL;

    if(pulCECInst == NULL)
    {
        if(UtopiaOpen(MODULE_CEC, &pulCECInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_CEC_MSG_ERROR("UtopiaOpen pulCECInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulCECInst, MAPI_CMD_CEC_GET_CONFIGURATION, (void*)&pCECArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MAPI_CEC_MSG_ERROR("Obtain CEC engine fail\n");
        }
        else
        {
            stInitialConfig->bAttachDrvISR = pCECArgs.stInitialConfigInfo.bAttachDrvISR;
            stInitialConfig->ulXTAL_CLK_Hz = pCECArgs.stInitialConfigInfo.ulXTAL_CLK_Hz;

            ulReturnValue = pCECArgs.stInitialConfigInfo.ulReturnValue;
        }
    }

    return ulReturnValue;
}

