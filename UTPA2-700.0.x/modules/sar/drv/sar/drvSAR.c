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
// Copyright (c) 2009-2010 MStar Semiconductor, Inc.
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
/// file    drvSAR.c
/// @brief  SAR Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////


//##################################
//
//  Access SAR settings by using Mailbox
//  sent commands to PM
//
//##################################
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h> //for memcpy, memset
#else
#include <string.h> //for memcpy, memset
#endif // end MSOS_TYPE_LINUX_KERNEL
#include "MsCommon.h"
#include "MsVersion.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include "drvMBX.h"
#endif // end MSOS_TYPE_LINUX_KERNEL
#include "drvSAR.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "drvSAR_v2.h"
#include "drvSAR_private.h"

#include "ULog.h"

// Internal Definition
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TAG_SAR "SAR"
#define SAR_MBX_QUEUESIZE       8
#define SAR_MBX_TIMEOUT         5000

#ifdef CONFIG_MSTAR_UTPA2K_SAR_ENABLE
#define SAR_UTOPIA20  (1)
#else
#define SAR_UTOPIA20  (0)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Reserved[16]; //Dummy Write for flushing MIU FIFO

}SAR_Dummy;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

#if (SAR_UTOPIA20)
MS_BOOL bU20SAROpened = FALSE;
void* pInstantSar;
void* pAttributeSar;
#endif
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SAR_DbgLvl _gSARDbgLevel = E_SAR_DBGLVL_ERROR;

static MSIF_Version _drv_sar_version = {
    .DDI = { SAR_DRV_VERSION },
};

static MS_BOOL gbSARInitialized = FALSE;
static SAR_AckFlags gSARAckFlags;
static MS_U8 gu8SARKey;
static MS_U8 gu8SARRepeat;
static MS_U8 gu8SARStatus;
static SAR_DrvInfo gSARDrvInfo;
static SAR_Callback gfSARCallback = NULL;
static SAR_Result gSARResult;
#ifndef MSOS_TYPE_LINUX_KERNEL
static MBX_Result gMBXResult;
#endif // end MSOS_TYPE_LINUX_KERNEL

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define SAR_DBG_FUNC()               if (_gSARDbgLevel >= E_SAR_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SAR, "\t====   %s   ====\n", __FUNCTION__);)}
#define SAR_DBG_INFO(x, args...)     if (_gSARDbgLevel >= E_SAR_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGI(TAG_SAR, x, ##args);)}
#define SAR_DBG_ERR(x, args...)      if (_gSARDbgLevel >= E_SAR_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGE(TAG_SAR, x, ##args);)}
#define SAR_DBG_WARN(x, args...)     if (_gSARDbgLevel >= E_SAR_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(ULOGW(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGW(TAG_SAR, x, ##args);)}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//define mutex
#define SAR_MUTEX_CREATE()
#define SAR_MUTEX_LOCK()
#define SAR_MUTEX_UNLOCK()
#define SAR_MUTEX_DELETE()


#ifndef MSOS_TYPE_LINUX_KERNEL
//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
//it is called by PM51 side MailBox driver(MailBox ISR)
static void _SAR_MailBoxCallbackHandler( MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    SAR_DBG_FUNC();

    *pbAddToQueue = FALSE; //no need to add to queue

    if(!pMsg) return;

    if(pMsg->u8MsgClass!= E_MBX_CLASS_SAR_NOWAIT)
    {
        SAR_DBG_ERR("Invalid Message Class!\n");
        return;
    }

    // check control type
    if ((pMsg->u8Ctrl != 4) && (pMsg->u8Ctrl != 5))
    {
        gSARResult = E_SAR_FAIL;
        SAR_DBG_INFO("Control: 0x%02x\n", pMsg->u8Ctrl);
        SAR_DBG_ERR("Not Implemented!\n");
        return;
    }
    SAR_DBG_INFO("Get SAR command: 0x%02x\n", pMsg->u8Index);
    SAR_DBG_INFO("Parameter[0]=%d\n",  pMsg->u8Parameters[0]);
    SAR_DBG_INFO("Parameter[1]=%d\n",  pMsg->u8Parameters[1]);
    gSARResult = E_SAR_FAIL;
    if (pMsg->u8Index == E_SAR_51ToCPU_CMD_KEYCODE)
    {
        //Notification from MCU51
        gSARResult = E_SAR_OK;
        gu8SARKey = (MS_U8)pMsg->u8Parameters[0];
        gu8SARRepeat = (MS_U8)pMsg->u8Parameters[1];
        SAR_DBG_INFO("Notify => (key=0x%02x, repeat=0x%02x)\n", gu8SARKey,gu8SARRepeat);
        if(gfSARCallback)
        {
            gfSARCallback(gu8SARKey, gu8SARRepeat);
        }
    }

    return;

}

static void _SAR_MailBoxHandler(MBX_Class eMsgClass, MBX_MSGQ_Status *pMsgQueueStatus)
{
    MBX_Msg MB_Command;

    SAR_DBG_FUNC();

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gMBXResult = MDrv_MBX_RecvMsg(E_MBX_CLASS_SAR, &MB_Command, SAR_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);
    if (gMBXResult == E_MBX_ERR_TIME_OUT)
    {
        SAR_DBG_ERR("Handler Timeout!\n");
        return;
    }

    if (gMBXResult == E_MBX_SUCCESS)
    {
        if ((MB_Command.u8Ctrl != 0) && (MB_Command.u8Ctrl != 1))
        {
            gSARResult = E_SAR_FAIL;
            SAR_DBG_ERR("Not Implemented!\n");
            return;
        }
        SAR_DBG_INFO("Get SAR command: 0x%02x.\n", MB_Command.u8Index);
        SAR_DBG_INFO("Parameter[0]=0x%02x\n",  MB_Command.u8Parameters[0]);
        SAR_DBG_INFO("Parameter[1]=0x%02x\n",  MB_Command.u8Parameters[1]);
        gSARResult = E_SAR_FAIL;
        switch (MB_Command.u8Index)
        {
            case E_SAR_51ToCPU_CMD_ACK_INIT:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_INIT);
                break;
            case E_SAR_51ToCPU_CMD_ACK_CONFIG:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_CONFIG);
                break;
            case E_SAR_51ToCPU_CMD_ACK_KEYCODE:
                gSARResult = E_SAR_OK;
                gu8SARKey = MB_Command.u8Parameters[0];
                gu8SARRepeat = MB_Command.u8Parameters[1];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_KEYCODE);
                break;
            case E_SAR_51ToCPU_CMD_ACK_SETCLBK:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_SETCLBK);
                break;
            case E_SAR_51ToCPU_CMD_ACK_LIBVER:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_LIBVER);
                break;
            case E_SAR_51ToCPU_CMD_ACK_STATUS:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gu8SARStatus = MB_Command.u8Parameters[1];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_STATUS);
                break;
            case E_SAR_51ToCPU_CMD_ACK_ENABLE:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_ENABLE);
                break;
            case E_SAR_51ToCPU_CMD_ACK_INFO:
                gSARResult = (SAR_Result)MB_Command.u8Parameters[0];
                gSARAckFlags &= (SAR_AckFlags)(~E_SAR_ACKFLG_WAIT_INFO);
                break;
            default:
                break;
        }
    }

    return;
}

//Software patch to flush MIU FIFO data to DRAM
static void _MDrv_SAR_DummyMiuFifo(void)
{
    static SAR_Dummy SarDummy;
    MS_U8 u8Idx;
    int count=0;

    for(u8Idx = 0; u8Idx<16; u8Idx++)
    {
        SarDummy.u8Reserved[u8Idx] = u8Idx;
        //Coverity Fixed
        if( SarDummy.u8Reserved[u8Idx] < 8)
        {
            count++;
        }
        else
        {
            count--;
        }
    }
    MsOS_FlushMemory();
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// SAR Iinitialized function before using SAR to receive keys. (This sends command to PM51 using mailbox.)
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_Init(void)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    if(gbSARInitialized == TRUE)
    {
    	 ULOGE(TAG_SAR,"SAR Driver Already Init!\r\n");
         return E_SAR_OK;
    }

    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //(1) register handler callback in MBX
    //gbSARInitialized = FALSE;
    MbxResult = MDrv_MBX_RegisterMSGWithCallBack(E_MBX_CLASS_SAR_NOWAIT,SAR_MBX_QUEUESIZE,_SAR_MailBoxCallbackHandler);
    if(MbxResult!=E_MBX_SUCCESS)
    {
    	if (MbxResult == E_MBX_ERR_SLOT_AREADY_OPENNED)
    	{
            SAR_DBG_WARN("Mailbox has been registered, classNumber=%d !\n", E_MBX_CLASS_SAR_NOWAIT);
    	}
    	else
    	{
            SAR_DBG_ERR("Mailbox registered with callback fails!\n");
            return E_SAR_FAIL;
        }
    }
    MbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_SAR,SAR_MBX_QUEUESIZE);
    if(MbxResult!=E_MBX_SUCCESS)
    {
    	if (MbxResult == E_MBX_ERR_SLOT_AREADY_OPENNED)
    	{
            SAR_DBG_WARN("Mailbox has been registered, classNumber=%d !\n", E_MBX_CLASS_SAR);
    	}
    	else
    	{
            SAR_DBG_ERR("Mailbox registered fails!\n");
            return E_SAR_FAIL;
        }
    }

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_INIT;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_INIT;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_SAR_FAIL;
    }

    //(3) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_SAR_FAIL;
          }
       }

    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_INIT) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gSARResult == E_SAR_FAIL)
    {
        gSARResult = E_SAR_FAIL;
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        gbSARInitialized = TRUE;
        SAR_DBG_INFO("OK\n");
    }
    return gSARResult;
}

//-------------------------------------------------------------------------------------------------
/// SAR parameter configuration function. (This sends command to PM51 using mailbox.)
/// @param pSARRegCfg \b IN: pointer to the SAR configuration structure.
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_Config(SAR_RegCfg *pSARRegCfg)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_VIRT virtSrcAddr;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //(1) set address to get final result
    virtSrcAddr = (MS_VIRT)pSARRegCfg;

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_CONFIG;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_CONFIG;
    MB_Command.u8ParameterCount = 4;
    MB_Command.u8Parameters[0] = (MS_S8)(virtSrcAddr>>24);
    MB_Command.u8Parameters[1] = (MS_S8)(virtSrcAddr>>16);
    MB_Command.u8Parameters[2] = (MS_S8)(virtSrcAddr>>8);
    MB_Command.u8Parameters[3] = (MS_S8)(virtSrcAddr&0x000000FFUL);
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_SAR_FAIL;
    }

    //(3) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_SAR_FAIL;
          }
       }
    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_CONFIG) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gSARResult == E_SAR_FAIL)
    {
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        SAR_DBG_INFO("OK\n");
    }
    return gSARResult;
}

//-------------------------------------------------------------------------------------------------
/// SAR get key code function. (This sends command to PM51 using mailbox.)
/// @param pu8Key \b OUT: Get SAR key code
/// @param pu8Repeat \b OUT: Get SAR key repeat status
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    //(0) check SAR driver is initialized
    *pu8Key = SAR_DUMMY_KEY;
    *pu8Repeat = FALSE;
    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //check SAR is initialized
    if(gbSARInitialized==FALSE)
    {
        SAR_DBG_ERR("Not initialized!\n");
        return E_SAR_FAIL;
    }

    //(1) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_KEYCODE;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_KEYCODE;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_SAR_FAIL;
    }

    //(3) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_SAR_FAIL;
          }
       }
    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_KEYCODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result and get key
    if( gSARResult == E_SAR_FAIL)
    {
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        *pu8Key = gu8SARKey;
        *pu8Repeat = gu8SARRepeat;
        SAR_DBG_INFO("OK\n");
    }
    return gSARResult;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR callback function when receive SAR key. Support only one callback. If call it twice,
/// the first callback does not be called. (This sends command to PM51 using mailbox.)
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate SAR interrupt.
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_SetCallback(SAR_Callback pCallback)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    //(0) check SAR driver is initialized
    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //check SAR is initialized
    if(gbSARInitialized==FALSE)
    {
        SAR_DBG_ERR("Not initialized!\n");
        return E_SAR_FAIL;
    }

    //(1) set local callback
    gfSARCallback = pCallback;

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_SETCLBK;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_SETCLBK;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_SAR_FAIL;
    }

    //(3) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_SAR_FAIL;
          }
       }
    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_SETCLBK) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gSARResult == E_SAR_FAIL)
    {
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        SAR_DBG_INFO("OK\n");
    }
    return gSARResult;

}

//-------------------------------------------------------------------------------------------------
/// Get SAR callback function which receive SAR key.
/// @return the callback function when generate SAR interrupt
//-------------------------------------------------------------------------------------------------
SAR_Callback MDrv_SAR_GetCallback(void)
{
    SAR_DBG_FUNC();

    return gfSARCallback;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR library version function.
/// @param ppVersion \b OUT: pointer to library structure
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        SAR_DBG_ERR("Fail\n");
        return E_SAR_FAIL;
    }
    *ppVersion = &_drv_sar_version;
    SAR_DBG_INFO("OK\n");
    return E_SAR_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR status function. (This sends command to PM51 using mailbox.)
/// @param pu8SARStatus \b OUT: pointer to status structure
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_GetStatus(MS_U8 *pu8SARStatus)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    //(0) check SAR driver is initialized
     *pu8SARStatus = SAR_DUMMY_STATUS;
    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //check SAR is initialized
    if(gbSARInitialized==FALSE)
    {
        SAR_DBG_ERR("Not initialized!\n");
        return E_SAR_FAIL;
    }

    //(1) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_STATUS;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_STATUS;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_SAR_FAIL;
    }

    //(2) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_SAR_FAIL;
          }
       }
    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_STATUS) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    if(gSARResult == E_SAR_FAIL)
    {
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        *pu8SARStatus = gu8SARStatus;
        SAR_DBG_INFO("OK\n");
    }
    return gSARResult;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR enable function. (This sends command to PM51 using mailbox.)
/// @param bEnable \b IN: TRUE: enable SAR, FALSE: disable SAR
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_Enable(MS_BOOL bEnable)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    //(0) check SAR driver is initialized
    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //check SAR is initialized
    if(gbSARInitialized==FALSE)
    {
        SAR_DBG_ERR("Not initialized!\n");
        return E_SAR_FAIL;
    }

    //(1) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_ENABLE;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_ENABLE;
    MB_Command.u8ParameterCount = 1;
    MB_Command.u8Parameters[0] = (MS_S8)bEnable;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_SAR_FAIL;
    }

    //(2) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_SAR_FAIL;
          }
       }
    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_ENABLE) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    if(gSARResult == E_SAR_FAIL)
    {
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        SAR_DBG_INFO("OK\n");
    }
    return gSARResult;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR driver information function. (This sends command to PM51 using mailbox.)
/// @return @ref SAR_DrvInfo structure
//-------------------------------------------------------------------------------------------------
const SAR_DrvInfo* MDrv_SAR_GetInfo(void)
{
    MS_VIRT virtSrcAddr;
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    SAR_DBG_FUNC();

    //(0) check SAR driver is initialized
    //flush MIU FIFO
    _MDrv_SAR_DummyMiuFifo();
    //check SAR is initialized
    if(gbSARInitialized==FALSE)
        return (SAR_DrvInfo*)NULL;

    //(1) set address to get final result
    virtSrcAddr = (MS_VIRT)&gSARDrvInfo;
    SAR_DBG_INFO("virtSrcAddr = 0x%08lX\n", virtSrcAddr);

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gSARAckFlags |= E_SAR_ACKFLG_WAIT_INFO;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_SAR_NOWAIT;
    MB_Command.u8Index = E_SAR_CPUTo51_CMD_INFO;
    MB_Command.u8ParameterCount = 4;
    MB_Command.u8Parameters[0] = (MS_S8)(virtSrcAddr>>24);
    MB_Command.u8Parameters[1] = (MS_S8)(virtSrcAddr>>16);
    MB_Command.u8Parameters[2] = (MS_S8)(virtSrcAddr>>8);
    MB_Command.u8Parameters[3] = (MS_S8)(virtSrcAddr&0x000000FFUL);
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return (SAR_DrvInfo*)NULL;
    }

    //(3) Waiting for SAR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
       _SAR_MailBoxHandler(E_MBX_CLASS_SAR,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return (SAR_DrvInfo*)NULL;
          }
       }
    }
    while((gSARAckFlags & E_SAR_ACKFLG_WAIT_INFO) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gSARResult == E_SAR_FAIL)
    {
        SAR_DBG_ERR("Fail\n");
    }
    else
    {
        SAR_DBG_INFO("OK\n");
    }
    return &gSARDrvInfo;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR debug function level.
/// @param eLevel \b IN: E_SAR_DBGLVL_NONE/E_SAR_DBGLVL_WARNING/E_SAR_DBGLVL_ERROR/E_SAR_DBGLVL_INFO/E_SAR_DBGLVL_ALL
/// @return E_SAR_OK: Success
/// @return E_SAR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_Result MDrv_SAR_SetDbgLevel(SAR_DbgLvl eLevel)
{
    SAR_DBG_INFO("Debug level: %u\n", eLevel);

    _gSARDbgLevel = eLevel;
    return E_SAR_OK;
}

#endif // end MSOS_TYPE_KERNEL

//##################################
//
//  Access SAR settings directly by RIU without
//  using Mailbox sent commands to PM
//
//##################################
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsOS.h"
#include "drvMMIO.h"
#include "drvSAR.h"
#include "halSAR.h"

// Internal Definition
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
///SAR keypad max supported channels (cabability)
#define CFG_SAR_KPD_CHANNELS        HAL_SAR_KPD_SUPPCHS
///SAR keypad max available channels
#define CFG_SAR_KPD_AVLBCHS         HAL_SAR_KPD_AVLBCHS
///SAR keypad adc max levels of all channels
#define CFG_SAR_KPD_AVLBLVLS        8 //### max level for available channels (Maximum level <= 8)

///SAR keypad driver parameters
#define SAR_KEY_NULL                0xFFUL
#define SAR_LEVEL_STABLE_NUM        10
#define SAR_LEVEL_STABLE_NUM_MIN    9
#define SAR_KEY_VALIDATION          2
#define SAR_REPEAT_KEY_CHECK        SAR_KEY_VALIDATION + 2
#define SAR_REPEAT_KEY_CHECK_1      SAR_KEY_VALIDATION + 3
#define SAR_REPEAT_PERIOD           5
#define SAR_REPEAT_PERIOD_1         SAR_REPEAT_PERIOD/2
#define SAR_CH_MAXID                0xFFUL

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
/// Version string

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SAR_KpdStatus g_u8SARKpdStatus = E_SAR_KPD_INVALID;
static SAR_KpdDrvInfo g_SARKpdDrvInfo;
static MS_BOOL gu8StorageChUsed[CFG_SAR_KPD_AVLBCHS];
static MS_U8 gu8StorageChInfo[CFG_SAR_KPD_AVLBCHS];
static MS_U8 gu8StorageLevelInfo[CFG_SAR_KPD_AVLBCHS][CFG_SAR_KPD_AVLBLVLS];
static MS_U8 gu8StorageLevels[CFG_SAR_KPD_AVLBCHS];
static MS_U8 gu8StorageKeyInfo[CFG_SAR_KPD_AVLBCHS][CFG_SAR_KPD_AVLBLVLS];
static MS_U8 gu8SARCheckCount[CFG_SAR_KPD_AVLBCHS], gu8SARPrevCMD[CFG_SAR_KPD_AVLBCHS], gu8SARPressKey[CFG_SAR_KPD_AVLBCHS];
static MS_U32 gu32SARTimePeriod[CFG_SAR_KPD_AVLBCHS];
static MS_U8 gu8ShortPressFlag[CFG_SAR_KPD_AVLBCHS] = {0};

static MS_U8 gu8KeyCmdBak[CFG_SAR_KPD_AVLBCHS] = {0};


static SAR_KpdDbgLvl _gSARKpdDbgLevel = E_SAR_KPD_DBGLVL_ERROR;
static SAR_AdcDbgLvl _gSARAdcDbgLevel = E_SAR_ADC_DBGLVL_ERROR;
static SAR_GpioDbgLvl _gSARGpioDbgLevel = E_SAR_GPIO_DBGLVL_ERROR;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define SAR_KPD_DBG_FUNC()               if (_gSARKpdDbgLevel >= E_SAR_KPD_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SAR, "\t====   %s   ====\n", __FUNCTION__);)}
#define SAR_KPD_DBG_INFO(x, args...)     if (_gSARKpdDbgLevel >= E_SAR_KPD_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGI(TAG_SAR, x, ##args);)}
#define SAR_KPD_DBG_ERR(x, args...)      if (_gSARKpdDbgLevel >= E_SAR_KPD_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGE(TAG_SAR, x, ##args);)}
#define SAR_KPD_DBG_WARN(x, args...)     if (_gSARKpdDbgLevel >= E_SAR_KPD_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(ULOGW(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGW(TAG_SAR, x, ##args);)}

#define SAR_ADC_DBG_FUNC()               if (_gSARAdcDbgLevel >= E_SAR_ADC_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SAR, "\t====   %s   ====\n", __FUNCTION__);)}
#define SAR_ADC_DBG_INFO(x, args...)     if (_gSARAdcDbgLevel >= E_SAR_ADC_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGI(TAG_SAR, x, ##args);)}
#define SAR_ADC_DBG_ERR(x, args...)      if (_gSARAdcDbgLevel >= E_SAR_ADC_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGE(TAG_SAR, x, ##args);)}
#define SAR_ADC_DBG_WARN(x, args...)     if (_gSARAdcDbgLevel >= E_SAR_ADC_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(ULOGW(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGW(TAG_SAR, x, ##args);)}

#define SAR_GPIO_DBG_FUNC()               if (_gSARGpioDbgLevel >= E_SAR_GPIO_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_SAR, "\t====   %s   ====\n", __FUNCTION__);)}
#define SAR_GPIO_DBG_INFO(x, args...)     if (_gSARGpioDbgLevel >= E_SAR_GPIO_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGI(TAG_SAR, x, ##args);)}
#define SAR_GPIO_DBG_ERR(x, args...)      if (_gSARGpioDbgLevel >= E_SAR_GPIO_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGE(TAG_SAR, x, ##args);)}
#define SAR_GPIO_DBG_WARN(x, args...)     if (_gSARGpioDbgLevel >= E_SAR_GPIO_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(ULOGW(TAG_SAR, "[%s]: ", __FUNCTION__); ULOGW(TAG_SAR, x, ##args);)}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static MS_U8 MDrv_SAR_Kpd_GetCfgChannels(void)
{
    MS_U8 i,u8Count=0;

    for(i=0;i<CFG_SAR_KPD_AVLBCHS;i++)
    {
        //compare valid channel id
        if(gu8StorageChInfo[i] != SAR_CH_MAXID)
        {
            u8Count++;
        }
    }
    return u8Count;
}

static MS_U8 MDrv_SAR_Kpd_GetChIndex(MS_U8 u8Channel)
{
    MS_U8 i;

    for(i=0;i<CFG_SAR_KPD_AVLBCHS;i++)
    {
        //compare valid channel id
        if(gu8StorageChInfo[i] == u8Channel)
            break;
    }
    return i;
}

static MS_U8 MDrv_SAR_Kpd_GetChID(MS_U8 u8ChIdx)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return SAR_CH_MAXID;
    return gu8StorageChInfo[u8ChIdx];
}

static MS_U8 MDrv_SAR_Kpd_GetChLevels(MS_U8 u8ChIdx)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return 0;
    return gu8StorageLevels[u8ChIdx];
}

static MS_U8 MDrv_SAR_Kpd_GetChIdxLevel(MS_U8 u8ChIdx,MS_U8 u8LvlIdx)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return 0;
    return gu8StorageLevelInfo[u8ChIdx][u8LvlIdx];
}

static MS_U8 MDrv_SAR_Kpd_GetChIdxKey(MS_U8 u8ChIdx,MS_U8 u8LvlIdx)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return 0;
    return gu8StorageKeyInfo[u8ChIdx][u8LvlIdx];
}

static void MDrv_SAR_Kpd_SetChID(MS_U8 u8ChIdx,MS_U8 u8ChID)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return;
    gu8StorageChInfo[u8ChIdx]=u8ChID;
}

static void MDrv_SAR_Kpd_SetChLevels(MS_U8 u8ChIdx,MS_U8 u8Levels)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return;
    gu8StorageLevels[u8ChIdx]=u8Levels;
}

static void MDrv_SAR_Kpd_SetChIdxLevel(MS_U8 u8ChIdx,MS_U8 u8LvlIdx,MS_U8 u8Val)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return;
    gu8StorageLevelInfo[u8ChIdx][u8LvlIdx]= u8Val;
}

static void MDrv_SAR_Kpd_SetChIdxKey(MS_U8 u8ChIdx,MS_U8 u8LvlIdx,MS_U8 u8Val)
{
    if(u8ChIdx>=CFG_SAR_KPD_AVLBCHS)
        return;
    gu8StorageKeyInfo[u8ChIdx][u8LvlIdx]=u8Val;
}

static SAR_KpdResult MDrv_SAR_Kpd_CfgChInfo(SAR_KpdRegCfg *pHalSarChInfo)
{
    MS_U8 u8Channel;
    MS_U8 i,j,k;

    SAR_KPD_DBG_INFO("MDrv_SAR_Kpd_CfgChInfo: ================= start\n");
    SAR_KPD_DBG_INFO("u8SARChID=0x%02X\n",pHalSarChInfo->u8SARChID);
    SAR_KPD_DBG_INFO("tSARChBnd.u8UpBnd=0x%02X\n",pHalSarChInfo->tSARChBnd.u8UpBnd);
    SAR_KPD_DBG_INFO("tSARChBnd.u8LoBnd=0x%02X\n",pHalSarChInfo->tSARChBnd.u8LoBnd);
    SAR_KPD_DBG_INFO("u8KeyLevelNum=0x%02X\n",pHalSarChInfo->u8KeyLevelNum);
    for(k=0;k<pHalSarChInfo->u8KeyLevelNum;k++)
        SAR_KPD_DBG_INFO("u8KeyThreshold[%d]=0x%02X\n",k,pHalSarChInfo->u8KeyThreshold[k]);
    for(k=0;k<pHalSarChInfo->u8KeyLevelNum;k++)
        SAR_KPD_DBG_INFO("u8KeyCode[%d]=0x%02X\n",k,pHalSarChInfo->u8KeyCode[k]);
    SAR_KPD_DBG_INFO("MDrv_SAR_Kpd_CfgChInfo: ================= end\n");

    // get channel ID
    u8Channel = (MS_U8)pHalSarChInfo->u8SARChID;
    // configure
    for(i=0;i<CFG_SAR_KPD_AVLBCHS;i++)
    {
        if((MDrv_SAR_Kpd_GetChID(i) == SAR_CH_MAXID) || (MDrv_SAR_Kpd_GetChID(i) == u8Channel))
        {
            MDrv_SAR_Kpd_SetChID(i,u8Channel);
            MDrv_SAR_Kpd_SetChLevels(i,pHalSarChInfo->u8KeyLevelNum);
            for(j=0;j<pHalSarChInfo->u8KeyLevelNum;j++)
            {
                MDrv_SAR_Kpd_SetChIdxLevel(i,j,pHalSarChInfo->u8KeyThreshold[j]);
                MDrv_SAR_Kpd_SetChIdxKey(i,j,pHalSarChInfo->u8KeyCode[j]);
            }
            HAL_SAR_CfgChannelBound(u8Channel,(HAL_SAR_BndCfg*)&pHalSarChInfo->tSARChBnd);

            //acording to u8UpBnd & u8LoBnd , it decide gu8StorageChUsed
            if ((MS_U8)pHalSarChInfo->tSARChBnd.u8UpBnd > (MS_U8)pHalSarChInfo->tSARChBnd.u8LoBnd)
              gu8StorageChUsed[u8Channel] = TRUE;
            else
              gu8StorageChUsed[u8Channel] = FALSE;

            return E_SAR_KPD_OK;
        }
    }
    return E_SAR_KPD_FAIL;
}

static void MDrv_SAR_Kpd_CfgAnalogInputs(void)
{
    MS_U8 i,j;

    // configure analog input & output enable according to used channels
    for(j=0;j<CFG_SAR_KPD_CHANNELS;j++)
    {
        for(i=0;i<CFG_SAR_KPD_AVLBCHS;i++)
        {
            if(MDrv_SAR_Kpd_GetChID(i) == j)
                break;
        }
        if(i<CFG_SAR_KPD_AVLBCHS)//matches
        {
            HAL_SAR_CfgAanlogInputSelect(j,ENABLE);
            HAL_SAR_CfgOutputEnable(j,DISABLE);
        }
        else
        {
            HAL_SAR_CfgAanlogInputSelect(j,DISABLE);
            //HAL_SAR_CfgOutputEnable(j,DISABLE);
        }
    }
}

static void MDrv_SAR_Kpd_ResetAllChInfo(void)
{
    SAR_KpdBndCfg tSARChBnd;
    MS_U8 u8ChId;

    tSARChBnd.u8LoBnd = 0x00;
    tSARChBnd.u8UpBnd = 0x00;
    for(u8ChId=0;u8ChId < CFG_SAR_KPD_CHANNELS;u8ChId++)
    {
        HAL_SAR_CfgChannelBound(u8ChId,(HAL_SAR_BndCfg*)&tSARChBnd);
        HAL_SAR_CfgAanlogInputSelect(u8ChId,DISABLE);
        //HAL_SAR_CfgOutputEnable(u8ChId,DISABLE);
    }

    for(u8ChId=0;u8ChId < CFG_SAR_KPD_AVLBCHS;u8ChId++)
    {
        gu8StorageChInfo[u8ChId] = SAR_CH_MAXID;
    }
}

static void MDrv_SAR_Kpd_SetDefaultChannels(void)
{
    MDrv_SAR_Kpd_ResetAllChInfo();
    //MDrv_SAR_Kpd_CfgAnalogInputs();
}

static MS_U32 _GetSystemTime(MS_U8 u8MsDivisor)
{
    if(u8MsDivisor==0)
        return 0;
    return (MsOS_GetSystemTime()/(MS_U32)u8MsDivisor);
}

static MS_U8 MDrv_SAR_Kpd_GetCHKey(MS_U8 u8Channel, MS_U8 *pu8Key , MS_U8 *pu8Repeat)
{
    MS_U8 ret_val=FALSE;
    MS_U8 i,k,KEY_LV[CFG_SAR_KPD_AVLBLVLS],Key_Value;
    MS_U8 u8Index;

    *pu8Key = 0xFF;
    *pu8Repeat = 1;

    u8Index = MDrv_SAR_Kpd_GetChIndex(u8Channel);
    if(u8Index>=g_SARKpdDrvInfo.u8ConfigUsedChs)
    {
        return ret_val;
    }

    for(i=0; i<CFG_SAR_KPD_AVLBLVLS; i++)
        KEY_LV[i] = 0;

    for (k=0; k<SAR_LEVEL_STABLE_NUM; k++)
    {
        Key_Value = MDrv_SAR_Adc_GetValue(u8Channel);
        for (i=0;i<MDrv_SAR_Kpd_GetChLevels(u8Index);i++)
        {
            if (Key_Value <= MDrv_SAR_Kpd_GetChIdxLevel(u8Index,i))
            {
            	{
	                KEY_LV[i] += 1;
	                break;
            	}
            }
        }
    }

    for(i=0; i<MDrv_SAR_Kpd_GetChLevels(u8Index); i++)
    {
        if(KEY_LV[i] >= SAR_LEVEL_STABLE_NUM_MIN)
        {
            SAR_KPD_DBG_INFO("[%d_%d]: Key_Value = 0x%02X\n",u8Channel,u8Index,Key_Value);
            gu8SARPressKey[u8Index] = TRUE;
            *pu8Key = MDrv_SAR_Kpd_GetChIdxKey(u8Index,i);

            if (gu8SARPrevCMD[u8Index] != *pu8Key)
            {
                gu8SARPrevCMD[u8Index] = *pu8Key;
                gu8SARCheckCount[u8Index] = 0;
                gu8KeyCmdBak[u8Index] = 0xFF;
            }
            else
            {
                if (gu8SARCheckCount[u8Index] < SAR_KEY_VALIDATION)
                {
                    gu8SARCheckCount[u8Index]++;
                    return FALSE;
                }
                else if (gu8SARCheckCount[u8Index] == SAR_KEY_VALIDATION)
                {
                    gu8SARCheckCount[u8Index]++;
                    gu32SARTimePeriod[u8Index] = _GetSystemTime(100);
                    gu8KeyCmdBak[u8Index]=*pu8Key;
                    gu8ShortPressFlag[u8Index]=TRUE;
                    //return TRUE;
                }

                if (gu8SARCheckCount[u8Index] == SAR_REPEAT_KEY_CHECK)    //SAR_KEY_VALIDATION+2
                {
                    if (_GetSystemTime(100) > (gu32SARTimePeriod[u8Index] + SAR_REPEAT_PERIOD/3))
                    {
                        gu32SARTimePeriod[u8Index] = _GetSystemTime(100);
                        gu8SARCheckCount[u8Index] = SAR_REPEAT_KEY_CHECK_1;
                        gu8ShortPressFlag[u8Index]=FALSE;
                        *pu8Repeat = 0x01;
                    }
                    else
                    {
                        *pu8Key = 0xFF;
                        *pu8Repeat = 0x01;
                    }
                    return TRUE;
                }
                else if (gu8SARCheckCount[u8Index] == SAR_REPEAT_KEY_CHECK_1) //SAR_KEY_VALIDATION+3
                {
                    if (_GetSystemTime(100) > gu32SARTimePeriod[u8Index])
                    {
                        gu32SARTimePeriod[u8Index] = _GetSystemTime(100);
                        gu8SARCheckCount[u8Index] = SAR_REPEAT_KEY_CHECK_1;
                        gu8ShortPressFlag[u8Index]=FALSE;
                        *pu8Repeat = 0x01;
                    }
                    else
                    {
                        *pu8Key = 0xFF;
                        *pu8Repeat = 0x01;
                    }
                    return TRUE;
                }

                if (_GetSystemTime(100) > (gu32SARTimePeriod[u8Index] + SAR_REPEAT_PERIOD))  //if 700ms
                {
                    gu32SARTimePeriod[u8Index] = _GetSystemTime(100);
                    gu8SARCheckCount[u8Index] = SAR_REPEAT_KEY_CHECK; //SAR_KEY_VALIDATION+2
                    gu8ShortPressFlag[u8Index] = 0;
                    *pu8Repeat = 0x01;
                    return TRUE;
                }
            }
        }
    }

    if (gu8SARPressKey[u8Index])
		{
         gu8SARPressKey[u8Index] = FALSE;
    }
    else
    {
			if(gu8ShortPressFlag[u8Index])
			{
				gu8ShortPressFlag[u8Index] = FALSE;
				*pu8Key = gu8KeyCmdBak[u8Index];
				*pu8Repeat = 0x00;
				return TRUE;
			}
			gu8SARPrevCMD[u8Index] = 0xFF;
    }
    return ret_val;
}

static void MDrv_SAR_Kpd_IntClear(void)
{
    HAL_SAR_CfgIntClear(ENABLE);
    HAL_SAR_CfgIntClear(DISABLE);
}

static void MDrv_SAR_Kpd_SetStatus(SAR_KpdStatus SARStatus, MS_BOOL bEnable)
{
    if(bEnable)
        g_u8SARKpdStatus |= SARStatus;
    else
        g_u8SARKpdStatus &= (SAR_KpdStatus)(~SARStatus);
}

static void MDrv_SAR_Kpd_IntEnable(MS_BOOL bEnable)
{
    HAL_SAR_CfgIntMask((bEnable)? DISABLE : ENABLE);
    //MDrv_SAR_Kpd_SetStatus(E_SAR_KPD_INT_ENABLE,bEnable);
}

SAR_KpdResult MDrv_SAR_Kpd_MMIO_Init(void)
{
    MS_VIRT virtBaseAddr = 0;
    MS_PHY u32BaseSize = 0;

    SAR_KPD_DBG_FUNC();

    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
    {
        SAR_KPD_DBG_ERR("Get PM IOMAP Base faill!\n");
        return E_SAR_KPD_FAIL;
    }
    HAL_SAR_SetIOMapBase(virtBaseAddr);
    SAR_KPD_DBG_INFO("virtBaseAddr(PM) = %lx\n", virtBaseAddr);

    if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_HW))
    {
        SAR_KPD_DBG_ERR("Get NPM IOMAP Base faill!\n");
        return E_SAR_KPD_FAIL;
    }
    HAL_SAR_SetIOMapBase_NPM(virtBaseAddr);
    SAR_KPD_DBG_INFO("virtBaseAddr(NPM) = %lx\n", virtBaseAddr);

    return E_SAR_KPD_OK;
}

SAR_KpdResult MDrv_SAR_Kpd_CfgChannelBound(MS_U8 u8Channel, MS_U16 u16Upper, MS_U16 u16Lower)
{
	HAL_SAR_BndCfg  sBnd;
	sBnd.u8UpBnd = (MS_U8) u16Upper;
	sBnd.u8LoBnd = (MS_U8) u16Lower;

	return HAL_SAR_CfgChannelBound(u8Channel, &sBnd);
}



//-------------------------------------------------------------------------------------------------
/// Set SAR Keypad initialized function.
/// @param None:
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_Init(void)
{
    MS_VIRT virtBaseAddr = 0;
    MS_PHY u32BaseSize = 0;
    MS_U32 i;

    SAR_KPD_DBG_FUNC();

    if (!(g_u8SARKpdStatus & E_SAR_KPD_INIT_OK))
    {
        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_PM))
        {
            SAR_KPD_DBG_ERR("Get PM IOMAP Base faill!\n");
            return E_SAR_KPD_FAIL;
        }
        HAL_SAR_SetIOMapBase(virtBaseAddr);
        SAR_KPD_DBG_INFO("virtBaseAddr(PM) = %lx\n", virtBaseAddr);

        if(!MDrv_MMIO_GetBASE(&virtBaseAddr, &u32BaseSize, MS_MODULE_HW))
        {
            SAR_KPD_DBG_ERR("Get NPM IOMAP Base faill!\n");
            return E_SAR_KPD_FAIL;
        }
        HAL_SAR_SetIOMapBase_NPM(virtBaseAddr);
        SAR_KPD_DBG_INFO("virtBaseAddr(NPM) = %lx\n", virtBaseAddr);
    }
    else
    {
        SAR_KPD_DBG_ERR("SAR Kpd has been initialized!\n");
        return E_SAR_KPD_OK;
    }

    // set default channels
    MDrv_SAR_Kpd_SetDefaultChannels();
    // set configured parameters
    g_SARKpdDrvInfo.u8TrigMode = HAL_SAR_EDGE;
    g_SARKpdDrvInfo.u8SingleChEn = DISABLE;
    g_SARKpdDrvInfo.u8DigitOprMode = HAL_SAR_FREERUN;
    g_SARKpdDrvInfo.u8AtopFreerun = ENABLE;
    g_SARKpdDrvInfo.u8DigitPwrDn = DISABLE;
    g_SARKpdDrvInfo.u8AtopPwrDn = DISABLE;
    g_SARKpdDrvInfo.u8HighChEn = ENABLE;
    g_SARKpdDrvInfo.u8InterruptEn = DISABLE;
    g_SARKpdDrvInfo.u8ConfigUsedChs = 0;
    g_SARKpdDrvInfo.u8MaxKpdSuppChs = CFG_SAR_KPD_CHANNELS;
    g_SARKpdDrvInfo.u8MaxKpdAvlbChs = CFG_SAR_KPD_AVLBCHS;
    g_SARKpdDrvInfo.u8MaxKpdAvlbLvls = CFG_SAR_KPD_AVLBLVLS;
    // config sar parameters
    HAL_SAR_CfgSingleChannel(0x00);
    HAL_SAR_CfgTriggerMode(g_SARKpdDrvInfo.u8TrigMode); //0:edge trigger, 1: level trigger
    HAL_SAR_CfgSingleChannelEn(g_SARKpdDrvInfo.u8SingleChEn);
    HAL_SAR_CfgDigitOperMode(g_SARKpdDrvInfo.u8DigitOprMode);
    HAL_SAR_CfgDigitPowerdown(g_SARKpdDrvInfo.u8DigitPwrDn);
    HAL_SAR_CfgAtopPowerdown(g_SARKpdDrvInfo.u8AtopPwrDn);
    HAL_SAR_CfgAtopFreeRun(g_SARKpdDrvInfo.u8AtopFreerun);
    HAL_SAR_CfgHighChannel(g_SARKpdDrvInfo.u8HighChEn);
    MDrv_SAR_Kpd_IntEnable(g_SARKpdDrvInfo.u8InterruptEn);
    HAL_SAR_CfgStart(DISABLE);
    HAL_SAR_CfgSelection(DISABLE);
    HAL_SAR_CfgClockSamplePeriod(0x05);

    MDrv_SAR_Kpd_IntClear();
    MDrv_SAR_Kpd_SetStatus(E_SAR_KPD_INIT_OK,TRUE);
    for (i = 0; i<CFG_SAR_KPD_AVLBCHS; i++)
    {
    	gu8SARCheckCount[i] = 0;
        gu8SARPrevCMD[i] = 0xFF;
    	gu8SARPressKey[i] =FALSE;
    	gu32SARTimePeriod[i] = 0;
    }
    SAR_KPD_DBG_INFO("SAR keypad is initialized completely\n");
    return E_SAR_KPD_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR Keypad channel information function.
/// @param sarChInfo: sar channel info pointer
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_SetChInfo_U2K(SAR_KpdRegCfg *sarChInfo)
{
    SAR_KpdResult ret_val;

    SAR_KPD_DBG_FUNC();

    if(sarChInfo->u8SARChID >= CFG_SAR_KPD_AVLBCHS)
        return E_SAR_KPD_FAIL;

    if(g_SARKpdDrvInfo.u8ConfigUsedChs >= CFG_SAR_KPD_AVLBCHS)
        return E_SAR_KPD_FAIL;

    ret_val = MDrv_SAR_Kpd_CfgChInfo(sarChInfo);
    g_SARKpdDrvInfo.u8ConfigUsedChs = MDrv_SAR_Kpd_GetCfgChannels();
    SAR_KPD_DBG_INFO("Configured Channels = %d\n",g_SARKpdDrvInfo.u8ConfigUsedChs);

    // configure analog input & output enable according to used channels
    MDrv_SAR_Kpd_CfgAnalogInputs();
    return ret_val;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad key code Alogrithm function.
/// @param pu8Key: key code pointer
/// @param pu8Repeat: key repeat pointer
/// @param u8Idx: Inspect SAR index
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------

SAR_KpdResult MDrv_SAR_Kpa_GetKeyCode_Alogrithm(MS_U8 *pu8Key, MS_U8 *pu8Repeat, MS_U8 u8Idx)
{
    MS_U8 u8Channel;
    u8Channel = MDrv_SAR_Kpd_GetChID(u8Idx) ;

    if((u8Channel<CFG_SAR_KPD_AVLBCHS) && (gu8StorageChUsed[u8Channel]==TRUE))
    {
        if(MDrv_SAR_Kpd_GetCHKey(u8Channel, pu8Key, pu8Repeat))
            return E_SAR_KPD_OK;
    }

    return E_SAR_KPD_FAIL;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad key code function.
/// @param pu8Key: key code pointer
/// @param pu8Repeat: key repeat pointer
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetKeyCode_U2K(MS_U8 *pu8Key, MS_U8 *pu8Repeat)
{
    MS_U8 i;

    SAR_KPD_DBG_FUNC();

    for(i=0;i<g_SARKpdDrvInfo.u8ConfigUsedChs;i++)
    {
        if (MDrv_SAR_Kpa_GetKeyCode_Alogrithm(pu8Key, pu8Repeat, i) == E_SAR_KPD_OK)
  	    return E_SAR_KPD_OK;
    }
    return E_SAR_KPD_FAIL;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR Multi-Keypad key code function with every SAR channel
/// @param SAR_KeyValue: key code struct pointer
/// @param u32Size: Struct Size
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_GetMultiKeyCode(SAR_KeyValue *sSarKey, MS_U32 u32Size)
{
    MS_U8 i;
    MS_U8 u8Key;
    MS_U8 u8Repeat;

    if (sSarKey == NULL)
        return E_SAR_KPD_FAIL;

    if ((sSarKey->u32Version == 0) && (sizeof(SAR_KeyValue) == u32Size))
    {
        sSarKey->u32Num = 0; //initalize u32Num = 0
        for(i=0;i<g_SARKpdDrvInfo.u8ConfigUsedChs;i++)
        {
            u8Key = 0;
	    u8Repeat = 0;
            if (MDrv_SAR_Kpa_GetKeyCode_Alogrithm(&u8Key, &u8Repeat, i) == E_SAR_KPD_OK)
	    {
	        sSarKey->unSarKeyData.sData[sSarKey->u32Num].u8Channel = MDrv_SAR_Kpd_GetChID(i) ;
		sSarKey->unSarKeyData.sData[sSarKey->u32Num].u8Key = u8Key;
		sSarKey->unSarKeyData.sData[sSarKey->u32Num].u8Repeat = u8Repeat;
		sSarKey->u32Num++;
            }
        }

        return (sSarKey->u32Num == 0? E_SAR_KPD_FAIL : E_SAR_KPD_OK);
    }
    else
    {
        SAR_DBG_ERR("Invalid SAR_KeyValue Version, version:[%d], struct size:[%d <--> %d]!!!!\n",
	    sSarKey->u32Version, u32Size, sizeof(SAR_KeyValue));
    }
    return E_SAR_KPD_FAIL;
}
//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad current status function.
/// @param None:
/// @return E_SAR_KPD_INIT_OK: Init OK
/// @return others: other status
//-------------------------------------------------------------------------------------------------
SAR_KpdStatus MDrv_SAR_Kpd_GetStatus(void)
{
    SAR_KPD_DBG_FUNC();

    return g_u8SARKpdStatus;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR Keypad driver information function.
/// @return @ref SAR_KpdDrvInfo structure
//-------------------------------------------------------------------------------------------------
const SAR_KpdDrvInfo* MDrv_SAR_Kpd_GetInfo(void)
{
    SAR_KPD_DBG_FUNC();

    return &g_SARKpdDrvInfo;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR Keypad debug function level.
/// @param eLevel \b IN: E_SAR_KPD_DBGLVL_NONE/E_SAR_KPD_DBGLVL_WARNING/E_SAR_KPD_DBGLVL_ERROR/E_SAR_KPD_DBGLVL_INFO/E_SAR_KPD_DBGLVL_ALL
/// @return E_SAR_KPD_OK: Success
/// @return E_SAR_KPD_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_KpdResult MDrv_SAR_Kpd_SetDbgLevel(SAR_KpdDbgLvl eLevel)
{
    SAR_KPD_DBG_FUNC();

    SAR_DBG_INFO("Debug level: %u\n", eLevel);
    _gSARKpdDbgLevel = eLevel;
    return E_SAR_KPD_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR as ADC channel.
/// @param u8Channel: sar ADC channel 0~7
/// @param bEnable: 1: configured as ADC, 0: configured as GPIO input
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_Config(MS_U8 u8Channel,MS_BOOL bEnable)
{
    SAR_ADC_DBG_FUNC();

    if(bEnable)
    {
        HAL_SAR_CfgAanlogInputSelect(u8Channel,ENABLE);
        HAL_SAR_CfgOutputEnable(u8Channel,DISABLE);
    }
    else
    {
        HAL_SAR_CfgAanlogInputSelect(u8Channel,DISABLE);
        HAL_SAR_CfgOutputEnable(u8Channel,DISABLE);
    }
    return E_SAR_ADC_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR ADC value function for each SAR channel.
/// @param u8Channel: sar ADC channel 0~7
/// @return MS_U8: ADC value
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_SAR_Adc_GetValue(MS_U8 u8Channel)
{
    SAR_ADC_DBG_FUNC();

    return HAL_SAR_GetChannelADC(u8Channel);
}

//-------------------------------------------------------------------------------------------------
/// Set SAR High channel function for RGB HSync application
/// @param bEnable: 1: enable high channel, 0: disable high channel
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_SetHSyncChEn(MS_BOOL bEnable)
{
    SAR_ADC_DBG_FUNC();

    HAL_SAR_CfgHighChannel(bEnable);

    return E_SAR_ADC_OK;
}

//-------------------------------------------------------------------------------------------------
/// Switch RGB HSync MUX to direct signal to SAR High channel
/// @param u8HsynCh: HSync channel: 0, 1, 2
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_SetHSyncCh(SAR_AdcHSyncCh eHSyncCh)
{
    MS_BOOL bResult;

    SAR_ADC_DBG_FUNC();

    switch(eHSyncCh)
    {
        case E_SAR_ADC_HSYNC_CH0:
            bResult= HAL_SAR_SetAdcHSyncChannel(0);
            break;
        case E_SAR_ADC_HSYNC_CH1:
            bResult= HAL_SAR_SetAdcHSyncChannel(1);
            break;
        case E_SAR_ADC_HSYNC_CH2:
            bResult= HAL_SAR_SetAdcHSyncChannel(2);
            break;
        case E_SAR_ADC_HSYNC_CH3:
            bResult= HAL_SAR_SetAdcHSyncChannel(3);
            break;
        default:
            return E_SAR_ADC_FAIL;
    }
    return ((bResult)? E_SAR_ADC_OK : E_SAR_ADC_FAIL);
}

//-------------------------------------------------------------------------------------------------
/// Set SAR ADC debug function level.
/// @param eLevel \b IN: E_SAR_ADC_DBGLVL_NONE/E_SAR_ADC_DBGLVL_WARNING/E_SAR_ADC_DBGLVL_ERROR/E_SAR_ADC_DBGLVL_INFO/E_SAR_ADC_DBGLVL_ALL
/// @return E_SAR_ADC_OK: Success
/// @return E_SAR_ADC_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_AdcResult MDrv_SAR_Adc_SetDbgLevel(SAR_AdcDbgLvl eLevel)
{
    SAR_ADC_DBG_FUNC();

    SAR_DBG_INFO("Debug level: %u\n", eLevel);
    _gSARAdcDbgLevel = eLevel;
    return E_SAR_ADC_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR as GPIO channel.
/// @param u8Channel: sar GPIO channel 0~7
/// @param u8InOut: 1: Input, 0:output
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_CfgDir(MS_U8 u8Channel,MS_BOOL u8InOut)
{
    SAR_GPIO_DBG_FUNC();

    HAL_SAR_CfgAanlogInputSelect(u8Channel,DISABLE);
    HAL_SAR_CfgOutputEnable(u8Channel,(u8InOut)? DISABLE : ENABLE);
    return E_SAR_GPIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR GPIO channel High/Low
/// @param u8Channel: sar GPIO channel 0~7
/// @param bHighLow: 1: High, 0:low
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_SetOutput(MS_U8 u8Channel,MS_BOOL bHighLow)
{
    SAR_GPIO_DBG_FUNC();

    HAL_SAR_SetOutput(u8Channel,bHighLow);
    return E_SAR_GPIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get SAR GPIO channel High/Low
/// @param u8Channel: sar GPIO channel 0~7
/// @param pbResult: pointer to get Input level
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_GetInput(MS_U8 u8Channel, MS_BOOL* pbResult)
{
    SAR_GPIO_DBG_FUNC();

    *pbResult = HAL_SAR_GetInput(u8Channel);
    return E_SAR_GPIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR GPIO debug function level.
/// @param eLevel \b IN: E_SAR_GPIO_DBGLVL_NONE/E_SAR_GPIO_DBGLVL_WARNING/E_SAR_GPIO_DBGLVL_ERROR/E_SAR_GPIO_DBGLVL_INFO/E_SAR_GPIO_DBGLVL_ALL
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_Gpio_SetDbgLevel(SAR_GpioDbgLvl eLevel)
{
    SAR_GPIO_DBG_FUNC();

    SAR_DBG_INFO("Debug level: %u\n", eLevel);
    _gSARGpioDbgLevel = eLevel;
    return E_SAR_GPIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR Interrupt mask
/// @param u8Channel: sar GPIO channel 0~7
/// @param bEnable:  1: enable interrupt, 0: disable interrupt
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_CfgInterrupt(MS_U8 u8Channel, MS_BOOL bEnable)
{
    HAL_SAR_Interrupt_EN(u8Channel, !bEnable);
    return E_SAR_GPIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR Interrupt wakeup
/// @param bEnable:  1: enable interrupt, 0: disable interrupt
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_CfgPMWakeup(MS_BOOL bEnable)
{
    HAL_SAR_Wakeup_EN(!bEnable);
    return E_SAR_GPIO_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set SAR Clear Interrupt Status
/// @param u8Channel: sar GPIO channel 0~7
/// @return E_SAR_GPIO_OK: Success
/// @return E_SAR_GPIO_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
void MDrv_SAR_ClearInterrupt(MS_U8 u8Channel)
{
    HAL_SAR_CLR_INT(u8Channel);
}

//-------------------------------------------------------------------------------------------------
/// Get SAR Status
/// @param u8Channel: sar GPIO channel 0~7
/// @return E_SAR_GPIO_OK: trigger interrupt
/// @return E_SAR_GPIO_FAIL or interrrupt do not trigger
//-------------------------------------------------------------------------------------------------
SAR_GpioResult MDrv_SAR_GetInterruptStatus(MS_U8 u8Channel)
{
    return HAL_SAR_INT_Status(u8Channel)? E_SAR_GPIO_OK: E_SAR_GPIO_FAIL;
}


//-------------------------------------------------------------------------------------------------
/// Set SAR 2.2V / 3.3V Level
/// @param eSarLevel:  E_SAR_33V: 3.3V, E_SAR_22V: 2.2V
/// @return Void
/// @return Void
//-------------------------------------------------------------------------------------------------
void MDrv_SAR_SetLevel(SAR_LEVEL eSarLevel)
{
	HAL_SAR_SET_LEVEL((MS_BOOL) eSarLevel);
}


MS_U32 MDrv_SAR_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = UTOPIA_STATUS_FAIL;
	if (u16PowerState == E_POWER_SUSPEND)
	{
        // suspend reset initflag
    gbSARInitialized = FALSE;
		_prev_u16PowerState = u16PowerState;
		u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            //for high sar channel set
            HAL_SAR_CfgHighChannel(ENABLE);

			_prev_u16PowerState = u16PowerState;
			u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
		}
		else
		{
			ULOGE(TAG_SAR, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE(TAG_SAR, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = UTOPIA_STATUS_FAIL;
	}

	return u16Return;// for success
}

#if (SAR_UTOPIA20)
// below are utopia10 interface and will call to utopia20 core
MS_BOOL _MDrv_SAR_CheckUtopia20Open(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    if (FALSE == bU20SAROpened)
    {
        if(UtopiaOpen(MODULE_SAR | KERNEL_MODE, pInstance, u32ModuleVersion, pAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("Open SAR fail\n");
    		return FALSE;
        }
        bU20SAROpened = TRUE;
    }
    return TRUE;
}
#endif

SAR_KpdResult MDrv_SAR_Kpd_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat)
{
#if (SAR_UTOPIA20)
    MS_U32 u32Ret;
    SAR_PRIVATE_PARAM_Kpd_GetKeyCode SarParam_Kpd_GetKeyCode;

    if ((pu8Key==NULL) || (pu8Repeat==NULL))
    {
        printf("[ERROR] %s, pu8Key or pu8Repeat is null\n", __FUNCTION__);
        return E_SAR_KPD_FAIL;
    }

    if (FALSE == _MDrv_SAR_CheckUtopia20Open(&pInstantSar, 0, pAttributeSar))
    {
    	printf("[ERROR] %s, _MDrv_SAR_CheckUtopia20Open fail\n", __FUNCTION__);
        return E_SAR_KPD_FAIL;
    }

    u32Ret = UtopiaIoctl(pInstantSar,MDrv_CMD_Kpd_GetKeyCode,(MS_U32*) &SarParam_Kpd_GetKeyCode);
    if (u32Ret == UTOPIA_STATUS_SUCCESS)
    {
        *pu8Key = SarParam_Kpd_GetKeyCode.u8Key;
        *pu8Repeat = SarParam_Kpd_GetKeyCode.u8Repeat;
    }
    u32Ret = SarParam_Kpd_GetKeyCode.u32Retn;
    return u32Ret;
#else
     return MDrv_SAR_Kpd_GetKeyCode_U2K(pu8Key,pu8Repeat);
#endif
}

SAR_KpdResult MDrv_SAR_Kpd_SetChInfo(SAR_KpdRegCfg *sarChInfo)
{
#if (SAR_UTOPIA20)
		SAR_KpdResult u32Ret;
		PSAR_PRIVATE_PARAM_KeyConfig pSarParam_Config = NULL;
		pSarParam_Config = (PSAR_PRIVATE_PARAM_KeyConfig)malloc(sizeof(SAR_PRIVATE_PARAM_KeyConfig));
		pSarParam_Config->pKeyRegCfg= sarChInfo;
		if (FALSE == _MDrv_SAR_CheckUtopia20Open(&pInstantSar, 0, pAttributeSar))
			return 0;
		u32Ret = UtopiaIoctl(pInstantSar,MDrv_CMD_SAR_Config,(MS_U32*)pSarParam_Config);
		free(pSarParam_Config);
		return (SAR_KpdResult)u32Ret;
#else
		return MDrv_SAR_Kpd_SetChInfo_U2K(sarChInfo);
#endif
}

SAR_KpdResult MDrv_SAR_GetConfig(SAR_KpdRegCfg *sarChInfo)
{
#if (SAR_UTOPIA20)
     SAR_KpdResult u32Ret;
     PSAR_PRIVATE_PARAM_KeyConfig pSarParam_Config = NULL;
     pSarParam_Config = (PSAR_PRIVATE_PARAM_KeyConfig)malloc(sizeof(SAR_PRIVATE_PARAM_KeyConfig));
     pSarParam_Config->pKeyRegCfg= sarChInfo;
     if (FALSE == _MDrv_SAR_CheckUtopia20Open(&pInstantSar, 0, pAttributeSar))
        return 0;
     u32Ret = UtopiaIoctl(pInstantSar,MDrv_CMD_SAR_GetConfig,(MS_U32*)pSarParam_Config);
     free(pSarParam_Config);
     return (SAR_KpdResult)u32Ret;
#else
     return MDrv_SAR_Kpd_SetChInfo_U2K(sarChInfo);
#endif
}


#ifdef CONFIG_MSTAR_T_SENSOR_ENABLE

static TSENSOR_CB_FUNC _pTSensorCbFunc = NULL;
static MS_BOOL _bSARInit  = FALSE;

static void _MDrv_TSensor_INTHandler(InterruptNum eIntNum)
{
    if(!_bSARInit)
    {
        MDrv_TSensor_Init();
        _bSARInit = TRUE;
    }
    if(NULL == _pTSensorCbFunc)
        return;
    if(!HAL_TSensorINTCheck())
    {
        printf("Not T-Sensor IRQ\n");
        return;
    }
    printf("T-Sensor IRQ\n");
    if(_pTSensorCbFunc == NULL)
        return;
    else
        _pTSensorCbFunc();
    MsOS_EnableInterrupt(eIntNum);
}

MS_BOOL MDrv_TSensor_GetTemperatureRange(MS_S16 *s16UpBound_C, MS_S16 *s16LowBound_C)
{
    if(!_bSARInit)
    {
        MDrv_TSensor_Init();
        _bSARInit = TRUE;
    }
    if((s16UpBound_C== NULL) ||(s16LowBound_C==NULL))
        return FALSE;
    *s16UpBound_C = HAL_TSensorGetUpBound();
    *s16LowBound_C = HAL_TSensorGetLowBound();
     return TRUE;
}
MS_S16 MDrv_TSensor_GetTemperature(void)
{
    if(!_bSARInit)
    {
        MDrv_TSensor_Init();
        _bSARInit = TRUE;
    }
    return HAL_TSensorGetCurTemp_C();
}
MS_BOOL MDrv_TSensor_Init(void)
{
    MS_BOOL bRet = TRUE;
    if(_bSARInit)
    {
        return bRet;
    }
    MDrv_SAR_Kpd_MMIO_Init();
    bRet &= HAL_TSensorEnable(); //disable interrupt in SAR bank
    _bSARInit = TRUE;
    return bRet;
}
MS_BOOL MDrv_TSensor_SetTemperatureMointerRange(MS_S16 s16UpBound_C, MS_S16 s16LowBound_C, TSENSOR_CB_FUNC pTSensor)
{
    static MS_BOOL ISRIsAttched = FALSE;
    MS_BOOL bRet = TRUE;
    if(!_bSARInit)
    {
        MDrv_TSensor_Init();
        _bSARInit = TRUE;
    }
    if(pTSensor)
    {
        _pTSensorCbFunc = pTSensor;
    }
    else
    {
        return FALSE;
    }
    if(!ISRIsAttched)
    {
        bRet &= MsOS_AttachInterrupt(E_INT_IRQ_KEYPAD, (InterruptCb)_MDrv_TSensor_INTHandler);
    }
    ISRIsAttched = TRUE;
    bRet &= MsOS_EnableInterrupt(E_INT_IRQ_KEYPAD); //enable interrupt in INTR bank
    bRet &= HAL_TSensorSetBound(s16UpBound_C,s16LowBound_C);

    return bRet;
}
MS_BOOL MDrv_TSensorIRQClear(void)
{
    if(!_bSARInit)
    {
        MDrv_TSensor_Init();
        _bSARInit = TRUE;
    }
    return HAL_TSensorIRQClear();
}

#endif



