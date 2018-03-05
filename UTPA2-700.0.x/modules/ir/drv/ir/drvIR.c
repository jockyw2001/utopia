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
/// file    drvIR.c
/// @brief  IR Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include <string.h>
#include "MsCommon.h"
#include "MsVersion.h"
#include "utopia_dapi.h"
#include "utopia.h"
#include "MsOS.h"
#include "drvMBX.h"
#include "drvIR.h"
#include "ULog.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define IR_MBX_QUEUESIZE        8
#define IR_MBX_TIMEOUT          5000
#define TAG_IR "IR"

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 u8Reserved[16]; //Dummy Write for flushing MIU FIFO

}IR_Dummy;


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static IR_DbgLvl _gIRDbgLevel = E_IR_DBGLVL_ERROR;

static MSIF_Version _drv_ir_version = {
    .DDI = { IR_DRV_VERSION },
};

static MS_BOOL gbIRInitialized = FALSE;
static IR_AckFlags gIRAckFlags;
static MS_U8 gu8IRKey;
static MS_U8 gu8IRRepeat;
static MS_U8 gu8IRStatus;
static IR_Callback gfIRCallback = NULL;
static IR_Result gIRResult;
static MBX_Result gMBXResult;
#ifdef MSOS_TYPE_LINUX
static IR_DrvInfo* pIRDrvInfo;
#else
static IR_DrvInfo gIRDrvInfo;
#endif
//for Fast boot
IR_RegCfg g_IRCfg_Fast;


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define IR_DBG_FUNC()               if (_gIRDbgLevel >= E_IR_DBGLVL_ALL) \
                                        {MS_DEBUG_MSG(ULOGD(TAG_IR, "\t====   %s   ====\n", __FUNCTION__));}
#define IR_DBG_INFO(x, args...)     if (_gIRDbgLevel >= E_IR_DBGLVL_INFO ) \
                                        {MS_DEBUG_MSG(ULOGI(TAG_IR, "[%s]: ", __FUNCTION__); ULOGI(TAG_IR, x, ##args));}
#define IR_DBG_ERR(x, args...)      if (_gIRDbgLevel >= E_IR_DBGLVL_ERROR) \
                                        {MS_DEBUG_MSG(ULOGE(TAG_IR, "[%s]: ", __FUNCTION__); ULOGE(TAG_IR, x, ##args);)}
#define IR_DBG_WARN(x, args...)     if (_gIRDbgLevel >= E_IR_DBGLVL_WARNING) \
                                        {MS_DEBUG_MSG(ULOGW(TAG_IR, "[%s]: ", __FUNCTION__); ULOGW(TAG_IR, x, ##args);)}

#define IR_MUTEX_LOCK()            
#define IR_MUTEX_UNLOCK()          
#define IR_MUTEX_DELETE()          


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//it is called by PM51 side MailBox driver(MailBox ISR)
static void _IR_MailBoxCallbackHandler( MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    IR_DBG_FUNC();

    *pbAddToQueue = FALSE; //no need to add to queue

    if(!pMsg) return;

    if(pMsg->u8MsgClass != E_MBX_CLASS_IRKEY_NOWAIT)
    {
        IR_DBG_INFO("Invalid Message Class!\n");
        return;
    }

    // check control type
    if ((pMsg->u8Ctrl != 4) && (pMsg->u8Ctrl != 5))
    {
        gIRResult = E_IR_FAIL;
        IR_DBG_INFO("Control: 0x%02x\n", pMsg->u8Ctrl);
        IR_DBG_ERR("Not Implemented!\n");
        return;
    }
    IR_DBG_INFO("Get IR command: 0x%02x\n", pMsg->u8Index);
    IR_DBG_INFO("Parameter[0]=%d\n",  pMsg->u8Parameters[0]);
    IR_DBG_INFO("Parameter[1]=%d\n",  pMsg->u8Parameters[1]);
    gIRResult = E_IR_FAIL;

    if (pMsg->u8Index == E_IR_51ToCPU_CMD_KEYCODE)
    {
        gIRResult = E_IR_OK;
        gu8IRKey = (MS_U8)pMsg->u8Parameters[0];
        gu8IRRepeat = (MS_U8)pMsg->u8Parameters[1];
        IR_DBG_INFO("Notify => (key=0x%02x, repeat=0x%02x)\n", gu8IRKey,gu8IRRepeat);
        if(gfIRCallback)
        {
            gfIRCallback(gu8IRKey, gu8IRRepeat);
        }
    }

    return;
}

static void _IR_MailBoxHandler(MBX_Class eMsgClass, MBX_MSGQ_Status *pMsgQueueStatus)
{
    MBX_Msg MB_Command;

    IR_DBG_FUNC();

    if(eMsgClass != E_MBX_CLASS_IRKEY)
    {
        IR_DBG_INFO("Invalid Message Class!\n");
        return;
    }

    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    gMBXResult = MDrv_MBX_RecvMsg(eMsgClass, &MB_Command, IR_MBX_TIMEOUT/*ms*/, MBX_CHECK_NORMAL_MSG);
    if (gMBXResult == E_MBX_ERR_TIME_OUT)
    {
        IR_DBG_INFO("Handler Timeout!\n");
        return;
    }

    if (gMBXResult == E_MBX_SUCCESS)
    {
        if ((MB_Command.u8Ctrl != 0) && (MB_Command.u8Ctrl != 1))
        {
            gIRResult = E_IR_FAIL;
            IR_DBG_ERR("Not Implemented!\n");
            return;
        }
        IR_DBG_INFO("Get IR command: 0x%02x.\n", MB_Command.u8Index);
        IR_DBG_INFO("Parameter[0]=%d\n",  MB_Command.u8Parameters[0]);
        gIRResult = E_IR_FAIL;
        switch (MB_Command.u8Index)
        {
            case E_IR_51ToCPU_CMD_ACK_INIT:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_INIT);
                break;
            case E_IR_51ToCPU_CMD_ACK_CONFIG:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_CONFIG);
                break;
            case E_IR_51ToCPU_CMD_ACK_KEYCODE:
                gIRResult = E_IR_OK;
                gu8IRKey = MB_Command.u8Parameters[0];
                gu8IRRepeat = MB_Command.u8Parameters[1];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_KEYCODE);
                break;
            case E_IR_51ToCPU_CMD_ACK_SETCLBK:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_SETCLBK);
                break;
            case E_IR_51ToCPU_CMD_ACK_LIBVER:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_LIBVER);
                break;
            case E_IR_51ToCPU_CMD_ACK_STATUS:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gu8IRStatus = MB_Command.u8Parameters[1];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_STATUS);
                break;
            case E_IR_51ToCPU_CMD_ACK_ENABLE:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_ENABLE);
                break;
            case E_IR_51ToCPU_CMD_ACK_INFO:
                gIRResult = (IR_Result)MB_Command.u8Parameters[0];
                gIRAckFlags &= (IR_AckFlags)(~E_IR_ACKFLG_WAIT_INFO);
                break;
            default:
                break;
        }

    }

    return;
}

//Software patch to flush MIU FIFO data to DRAM
static void _MDrv_IR_DummyMiuFifo(void)
{
    static IR_Dummy IrDummy;
    MS_U8 u8Idx;
    int count=0;

    for(u8Idx = 0; u8Idx<16; u8Idx++)
    {
        IrDummy.u8Reserved[u8Idx] = u8Idx;
        //Coverity Fixed
        if( IrDummy.u8Reserved[u8Idx] < 8)
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
/// IR Iinitialized function before using IR to receive keys.
/// @param pIRRegCfg \b IN: pointer to IR configuration structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_Init(IR_RegCfg *pIRRegCfg)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_PHYADDR u32SrcAddr;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //(0) register handler callback in MBX
    gbIRInitialized = FALSE;
    MbxResult = MDrv_MBX_RegisterMSGWithCallBack(E_MBX_CLASS_IRKEY_NOWAIT,IR_MBX_QUEUESIZE,_IR_MailBoxCallbackHandler);
    if(MbxResult!=E_MBX_SUCCESS)
    {
    	if (MbxResult == E_MBX_ERR_SLOT_AREADY_OPENNED)
    	{
            IR_DBG_WARN("Mailbox has been registered, classNumber=%d !\n", E_MBX_CLASS_IRKEY_NOWAIT);
    	}
    	else
    	{
            IR_DBG_ERR("Mailbox registered with callback fails!\n");
            return E_IR_FAIL;
        }
    }
    MbxResult = MDrv_MBX_RegisterMSG(E_MBX_CLASS_IRKEY,IR_MBX_QUEUESIZE);
    if(MbxResult!=E_MBX_SUCCESS)
    {
    	if (MbxResult == E_MBX_ERR_SLOT_AREADY_OPENNED)
    	{
            IR_DBG_WARN("Mailbox has been registered, classNumber=%d !\n", E_MBX_CLASS_IRKEY);
    	}
    	else
    	{
            IR_DBG_ERR("Mailbox registered fails!\n");
            return E_IR_FAIL;
        }
    }

    //(1) set address to get final result
    u32SrcAddr = (MS_PHYADDR)pIRRegCfg;
    #ifdef MSOS_TYPE_LINUX
    //backup physical address
    pIRDrvInfo = (IR_DrvInfo*)pIRRegCfg;
    #endif

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_INIT;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_INIT;
    MB_Command.u8ParameterCount = 4;
    MB_Command.u8Parameters[0] = (MS_S8)(u32SrcAddr>>24);
    MB_Command.u8Parameters[1] = (MS_S8)(u32SrcAddr>>16);
    MB_Command.u8Parameters[2] = (MS_S8)(u32SrcAddr>>8);
    MB_Command.u8Parameters[3] = (MS_S8)(u32SrcAddr&0x000000FFUL);
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_IR_FAIL;
    }

    //(3) Waiting for IR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_IR_FAIL;
          }
       }

    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_INIT) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        gbIRInitialized = TRUE;
        IR_DBG_INFO("OK\n");
    }
	
    return gIRResult;
}

//-------------------------------------------------------------------------------------------------
/// IR parameter configuration function.
/// @param pIRRegCfg \b IN: pointer to the IR configuration structure.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_Config(IR_TimeBnd *pIRTimeBnd)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_PHYADDR u32SrcAddr;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();

    //(0) check IR driver is initialized
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //check IR is initialized
    if(gbIRInitialized==FALSE)
    {
        IR_DBG_ERR("Not initialized!\n");
        return E_IR_FAIL;
    }

    //(1) set address to get final result
    u32SrcAddr = (MS_PHYADDR)pIRTimeBnd;

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_CONFIG;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_CONFIG;
    MB_Command.u8ParameterCount = 4;
    MB_Command.u8Parameters[0] = (MS_S8)(u32SrcAddr>>24);
    MB_Command.u8Parameters[1] = (MS_S8)(u32SrcAddr>>16);
    MB_Command.u8Parameters[2] = (MS_S8)(u32SrcAddr>>8);
    MB_Command.u8Parameters[3] = (MS_S8)(u32SrcAddr&0x000000FFUL);
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_IR_FAIL;
    }

    //(3) Waiting for IR message done
    do
    {
        if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_IR_FAIL;
          }
       }
    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_CONFIG) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        IR_DBG_INFO("OK\n");
    }
    return gIRResult;
}

//-------------------------------------------------------------------------------------------------
/// IR get key code function.
/// @param pu8Key \b OUT: Get IR key code
/// @param pu8Repeat \b OUT: Get IR key repeat status
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_GetKeyCode(MS_U8 *pu8Key, MS_U8 *pu8Repeat)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();

    //(0) check IR driver is initialized
    *pu8Key = IR_DUMMY_KEY;
    *pu8Repeat = FALSE;
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //check IR is initialized
    if(gbIRInitialized==FALSE)
    {
        IR_DBG_ERR("Not initialized!\n");
        return E_IR_FAIL;
    }

    //(1) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_KEYCODE;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_KEYCODE;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_IR_FAIL;
    }

    //(2) Waiting for IR message done
    do
    {
        if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_IR_FAIL;
          }
       }
    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_KEYCODE) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(3) check result and get key
    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        *pu8Key = gu8IRKey;
        *pu8Repeat = gu8IRRepeat;
        IR_DBG_INFO("OK\n");
    }
    return gIRResult;
}

//-------------------------------------------------------------------------------------------------
/// Set IR callback function when receive IR key. Support only one callback. If call it twice,
/// the first callback does not be called.
/// Note: The callback runs at interrupt mode.
/// @param pCallback \b IN: Set the callback function when generate IR interrupt.
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_SetCallback(IR_Callback pCallback)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();

    //(0) check IR driver is initialized
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //check IR is initialized
    if(gbIRInitialized==FALSE)
    {
        IR_DBG_ERR("Not initialized!\n");
        return E_IR_FAIL;
    }

    //(1) set local callback
    gfIRCallback = pCallback;

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_SETCLBK;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_SETCLBK;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_IR_FAIL;
    }

    //(3) Waiting for IR message done
    do
    {
       if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_IR_FAIL;
          }
       }
    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_SETCLBK) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        IR_DBG_INFO("OK\n");
    }
    return gIRResult;
}

//-------------------------------------------------------------------------------------------------
/// Get IR callback function which receive IR key.
/// @return the callback function when generate IR interrupt
//-------------------------------------------------------------------------------------------------
IR_Callback MDrv_IR_GetCallback(void)
{
    IR_DBG_FUNC();

    return gfIRCallback;
}

//-------------------------------------------------------------------------------------------------
/// Get IR library version function.
/// @param ppVersion \b OUT: pointer to library structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        IR_DBG_ERR("Fail\n");
        return E_IR_FAIL;
    }
    *ppVersion = &_drv_ir_version;
    IR_DBG_INFO("OK\n");
    return E_IR_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get IR status function.
/// @param pu8IRStatus \b OUT: pointer to status structure
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_GetStatus(MS_U8 *pu8IRStatus)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();

    //(0) check IR driver is initialized
    *pu8IRStatus = IR_DUMMY_STATUS;
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //check IR is initialized
    if(gbIRInitialized==FALSE)
    {
        IR_DBG_ERR("Not initialized!\n");
        return E_IR_FAIL;
    }

    //(1) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_STATUS;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_STATUS;
    MB_Command.u8ParameterCount = 0;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_IR_FAIL;
    }

    //(2) Waiting for IR message done
    do
    {
        if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_IR_FAIL;
          }
       }
    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_STATUS) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        *pu8IRStatus = gu8IRStatus;
        IR_DBG_INFO("OK\n");
    }
    return gIRResult;
}

//-------------------------------------------------------------------------------------------------
/// Set IR enable function.
/// @param bEnable \b IN: TRUE: enable IR, FALSE: disable IR
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_Enable(MS_BOOL bEnable)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();

    //(0) check IR driver is initialized
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //check IR is initialized
    if(gbIRInitialized==FALSE)
    {
        IR_DBG_ERR("Not initialized!\n");
        return E_IR_FAIL;
    }

    //(1) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_ENABLE;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_ENABLE;
    MB_Command.u8ParameterCount = 1;
    MB_Command.u8Parameters[0] = (MS_S8)bEnable;
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return E_IR_FAIL;
    }

    //(2) Waiting for IR message done
    do
    {
        if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return E_IR_FAIL;
          }
       }
    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_ENABLE) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        IR_DBG_INFO("OK\n");
    }
    return gIRResult;
}

//-------------------------------------------------------------------------------------------------
/// Get IR driver information function.
/// @return @ref IR_DrvInfo structure
//-------------------------------------------------------------------------------------------------
const IR_DrvInfo* MDrv_IR_GetInfo(void)
{
    MBX_Result MbxResult=E_MBX_UNKNOW_ERROR;
    MBX_Msg MB_Command;
    MBX_MSGQ_Status MbxMsgQStatus;
    MS_PHYADDR u32SrcAddr;
    MS_U32 MBX_Resendcnt=0;

    IR_DBG_FUNC();

    //(0) check IR driver is initialized
    //flush MIU FIFO
    _MDrv_IR_DummyMiuFifo();
    //check IR is initialized
    if(gbIRInitialized==FALSE)
    {
        IR_DBG_ERR("Not initialized!\n");
        return (IR_DrvInfo*)NULL;
    }

    //(1) set address to get final result
    #ifdef MSOS_TYPE_LINUX
    u32SrcAddr = (MS_PHYADDR)pIRDrvInfo;
    #else
    u32SrcAddr = (MS_PHYADDR)&gIRDrvInfo;
    #endif
    IR_DBG_INFO("u32SrcAddr = 0x%08lX\n", u32SrcAddr);

    //(2) send msg to CPU to get keycode if interrupt happens
    // set flag
    gIRAckFlags |= E_IR_ACKFLG_WAIT_INFO;
    // send to PM51
    memset((void*)&MB_Command, 0, sizeof(MBX_Msg));
    MB_Command.eRoleID = E_MBX_ROLE_PM;
    MB_Command.eMsgType = E_MBX_MSG_TYPE_INSTANT;
    MB_Command.u8Ctrl = 0;
    MB_Command.u8MsgClass = E_MBX_CLASS_IRKEY_NOWAIT;
    MB_Command.u8Index = E_IR_CPUTo51_CMD_INFO;
    MB_Command.u8ParameterCount = 4;
    MB_Command.u8Parameters[0] = (MS_S8)(u32SrcAddr>>24);
    MB_Command.u8Parameters[1] = (MS_S8)(u32SrcAddr>>16);
    MB_Command.u8Parameters[2] = (MS_S8)(u32SrcAddr>>8);
    MB_Command.u8Parameters[3] = (MS_S8)(u32SrcAddr&0x000000FFUL);
    MbxResult = MDrv_MBX_SendMsg(&MB_Command);
    while(E_MBX_SUCCESS != MbxResult)
    {
        //Error Handling here:
        return (IR_DrvInfo*)NULL;
    }

    //(3) Waiting for IR message done
    do
    {
        if(MBX_Resendcnt<10)
       {
        _IR_MailBoxHandler(E_MBX_CLASS_IRKEY,&MbxMsgQStatus);
        MBX_Resendcnt++;
       }
       else
       {
          MBX_Resendcnt=0;
          MbxResult = MDrv_MBX_SendMsg(&MB_Command);
          while(E_MBX_SUCCESS != MbxResult)
          {
            //Error Handling here:
            return (IR_DrvInfo*)NULL;
          }
       }
    }
    while((gIRAckFlags & E_IR_ACKFLG_WAIT_INFO) && (gMBXResult != E_MBX_ERR_TIME_OUT));

    //(4) check result
    if(gIRResult == E_IR_FAIL)
    {
        IR_DBG_ERR("Fail\n");
    }
    else
    {
        IR_DBG_INFO("OK\n");
    }
    #ifdef MSOS_TYPE_LINUX
    return pIRDrvInfo;
    #else
    return &gIRDrvInfo;
    #endif
}

//-------------------------------------------------------------------------------------------------
/// Set IR debug function.
/// @param eLevel \b IN: E_IR_DBGLVL_NONE/E_IR_DBGLVL_WARNING/E_IR_DBGLVL_ERROR/E_IR_DBGLVL_INFO/E_IR_DBGLVL_ALL
/// @return E_IR_OK: Success
/// @return E_IR_FAIL or other values: Failure
//-------------------------------------------------------------------------------------------------
IR_Result MDrv_IR_SetDbgLevel(IR_DbgLvl eLevel)
{
    IR_DBG_INFO("Debug level: %u\n", eLevel);

    _gIRDbgLevel = eLevel;
    return E_IR_OK;
}

#ifdef MSOS_TYPE_LINUX
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
//#include <asm-mips/types.h>
#include <errno.h>

#define IR_MODULE_KERNAL_NAME       "/dev/ir"
static int IR_fd = 0;

void MDrv_IR_OpenDevice(void)
{
    if (IR_fd==0)   //First time open
    {
        IR_fd = open(IR_MODULE_KERNAL_NAME, O_RDWR);
        if (IR_fd < 0)
        {
            IR_DBG_ERR("Fail to open IR Kernal Module\n");
        }
    }
}

void MDrv_IR_InitCfg(IR_InitCfgParam* pInitCfg)
{
    IR_InitCfgParam InitCfgParam;
    int iResult;

    if (IR_fd < 0)
    {
        IR_DBG_ERR("IR file descriptor is not been opened\n");
    }

    memcpy((void*)&InitCfgParam,(void*)pInitCfg,sizeof(IR_InitCfgParam));
    iResult = ioctl(IR_fd, MDRV_IR_INITCFG,&InitCfgParam);
    if(iResult < 0)
    {
        IR_DBG_ERR("IR Initial Configuration Fails!\n");
    }
}

void MDrv_IR_TimeCfg(IR_TimeCfgParam* pTimeCfg)
{
    IR_TimeCfgParam TimeCfgParam;
    int iResult;

    if (IR_fd < 0)
    {
        IR_DBG_ERR("IR file descriptor is not been opened\n");
    }

    memcpy((void*)&TimeCfgParam,(void*)pTimeCfg,sizeof(IR_TimeCfgParam));
    iResult = ioctl(IR_fd, MDRV_IR_TIMECFG,&TimeCfgParam);
    if(iResult < 0)
    {
        IR_DBG_ERR("IR Time Configuration Fails!\n");
    }
}

void MDrv_IR_GetPulseShot(IR_PulseShotInfo* pPulseShotInfo)
{
    int iResult;

    if (IR_fd < 0)
    {
        IR_DBG_ERR("IR file descriptor is not been opened\n");
    }
    iResult = ioctl(IR_fd, MDRV_IR_GET_SWSHOT_BUF, pPulseShotInfo);
    if(iResult < 0)
    {
        IR_DBG_ERR("IR Get Pulse Shot Fails!\n");
    }
}

void MDrv_IR_SetMultiHeaderCode(MS_MultiIR_HeaderInfo* pMultiHeaderCode)
{
    MS_MultiIR_HeaderInfo MultiHeaderCode;
    int iResult;

    if (IR_fd < 0)
    {
        IR_DBG_ERR("IR file descriptor is not been opened\n");
    }

    memcpy((void*)&MultiHeaderCode,(void*)pMultiHeaderCode,sizeof(MS_MultiIR_HeaderInfo));
    iResult = ioctl(IR_fd, MDRV_IR_SET_HEADER, &MultiHeaderCode);
    if(iResult < 0)
    {
        IR_DBG_ERR("IR Set Multi-Header Code Fails!\n");
    }
}

#endif
//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to register IR module
//--------------------------------------------------------------------------------------------------
enum
{
    IR_POOL_ID_IR0=0
} eIR_PoolID;

void IRRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. create a module(module_name, SHM_size), and register to utopia2.0
    void* pUtopiaModule = NULL;
    UtopiaModuleCreate(MODULE_IR, 0, &pUtopiaModule);
    UtopiaModuleRegister(pUtopiaModule);

    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pUtopiaModule, (FUtopiaOpen)IROpen, (FUtopiaClose)IRClose, (FUtopiaIOctl)IRIoctl);

    // 2. Resource register
    void* psResource = NULL;
    
    // start func to add resources of a certain Resource_Pool
    UtopiaModuleAddResourceStart(pUtopiaModule, IR_POOL_ID_IR0);
    
    // create a resouce and regiter it to a certain Resource_Pool, resource can alloc private for internal use
    UtopiaResourceCreate("ir0", sizeof(IR_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pUtopiaModule, psResource, IR_POOL_ID_IR0);

    UtopiaModuleAddResourceEnd(pUtopiaModule, IR_POOL_ID_IR0);

}

//--------------------------------------------------------------------------------------------------
// Utopia2.0 will call this function to get a instance to use IR
// @ \b in: 32ModuleVersion => this is for checking if API version is the same
//--------------------------------------------------------------------------------------------------
MS_U32 IROpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MS_U32 u32RetShmID, u32RetShmSize;
	MS_VIRT u32RetAddr;
    MS_U32 u32Ret;

    //Check DRV_REVISION
    if(u32ModuleVersion != IR_VERSION)
    {
        //ULOGE(TAG_IR, "\033[35mFunction = %s, Line = %d, ERROR, DRV_VERSION is not IR_VERSION\033[m\n", __PRETTY_FUNCTION__, __LINE__);
        //return FALSE;
    }

    // for multi-process safe, check if already other Instance exist
    IR_MUTEX_LOCK();
    u32Ret = MsOS_SHM_GetId((MS_U8*)"IR_Instance", sizeof(MS_U32), &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_QUERY);
    if(u32Ret == 0)
    {
        // first create, set value as 0
        MsOS_SHM_GetId((MS_U8*)"IR_Instance", sizeof(MS_U32), &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_CREATE);
        *(MS_U32 *)u32RetAddr = 0;
    }

    if(*(MS_U32 *)u32RetAddr == 1)
    {
        IR_MUTEX_UNLOCK();
        return FALSE;
    }
    else
    {
        *(MS_U32 *)u32RetAddr = 1;
        IR_MUTEX_UNLOCK();
    }

    IR_INSTANT_PRIVATE *pIRPri = NULL;

    // instance is allocated here, also can allocate private for internal use, ex, IR_INSTANT_PRIVATE, ppInstance will point to a pointer of the created UTOPIA_INSTANCE
    UtopiaInstanceCreate(sizeof(IR_INSTANT_PRIVATE), ppInstance);
    // set the pIRPri point to the private of UTOPIA_INSTANCE
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&pIRPri);
    


	MDrv_IR_Init((IR_RegCfg *)pAttribute);

    return TRUE;
}

MS_U32 IRIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	return 0; // FIXME: error code
}

MS_U32 IRClose(void* pInstance)
{
    MS_U32 u32RetShmID, u32RetShmSize;
	MS_VIRT u32RetAddr;
    MS_U32 u32Ret;

	u32RetAddr = 0 ; //fix coverity isue
    UtopiaInstanceDelete(pInstance);
    
    u32Ret = MsOS_SHM_GetId((MS_U8*)"IR_Instance", sizeof(MS_U32), &u32RetShmID, &u32RetAddr, &u32RetShmSize, MSOS_SHM_QUERY);
    
    if( (u32Ret == 0) || (*(MS_U32 *)u32RetAddr == 0) )
    {
        return FALSE;
    }
    else
    {
        IR_MUTEX_LOCK();
        *(MS_U32 *)u32RetAddr = 0;
        IR_MUTEX_UNLOCK();
        return TRUE;
    }
}



MS_U32 MDrv_IR_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = UTOPIA_STATUS_FAIL; 
	IR_RegCfg tmpRegCfg;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u16Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
            MDrv_IR_Init(&tmpRegCfg);

			_prev_u16PowerState = u16PowerState;
			u16Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
		}
		else
		{
			ULOGE(TAG_IR, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE(TAG_IR, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = UTOPIA_STATUS_FAIL;
	}

	return u16Return;// for success
}



