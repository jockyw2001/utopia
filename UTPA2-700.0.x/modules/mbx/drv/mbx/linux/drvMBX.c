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
/// file    drvMBX.c for Linux
/// @brief  MStar MailBox DDI
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _GNU_SOURCE

#define _DRV_MBX_C

//=============================================================================
// Include Files
//=============================================================================
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMBX.h"
#include "mdrv_mbx_io.h"
#include "mdrv_signals.h"
#include "drvDMD_VD_MBX.h"
#include "MsDevice.h"
#include "ULog.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================
#define MBXCLASS_IN_SIGNAL_MASK      0x0000FF00
#define MBXCLASS_IN_SIGNAL_SHIFT     0x8
#define TAG_MBX "MBX"

//=============================================================================
// Debug Macros
//=============================================================================

#define MBX_DBG_PRINT(dbgLevel, fmt, args...)  { \
                                                    if(_u8DbgLevel&dbgLevel) \
                                                    { \
                                                        MBX_PRINT(fmt, ## args); \
                                                    } \
                                                   } \



/*
#define MBX_DBG_PRINT(dbgLevel, fmt, args...)  { \
                                                    { \
                                                        MBX_PRINT(fmt, ## args); \
                                                    } \
                                                   }
*/


//#define MBX_DEBUG
#ifdef MBX_DEBUG
    #define MBX_PRINT(fmt, args...)      ULOGD(TAG_MBX, "[MailBox (Driver)][%05d] " fmt, __LINE__, ## args)
    #define MBX_ASSERT(_cnd, _fmt, _args...)                    \
                                    if (!(_cnd)) {              \
                                        ULOGD(TAG_MBX, _fmt, ##_args);  \
                                        while(1);               \
                                    }
#else
    #define MBX_PRINT(fmt, args...)   ULOGD(TAG_MBX, "[MailBox (Driver)][%05d] " fmt, __LINE__, ## args)
    #define MBX_ASSERT(_cnd, _fmt, _args...)
#endif

//=============================================================================
// Macros
//=============================================================================
//#define INVALID_MBXCLASS(mbxClass)  ((mbxClass<0) || (mbxClass >=E_MBX_CLASS_NUM))
#define INVALID_MBXCLASS(mbxClass)  (mbxClass >=E_MBX_CLASS_MAX)

///////////////////////////////////////////////////////////////////////////////////////////////////
// New Designed MBX Send/Recv will leads to DeadLock if we disable Interrupt in Send             //
// Because Send will Wait Ack from Recv, if Two side send at the same time dead lock will happen //
// So we can't disable interrupt when send                                                       //
// But this will request that we never send Mail in Interrupt otherwise Interrupt Hanlder may be //
// Blocked!                                                                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////

#define DRV_MBX_InitLock()  {\
                                    _mgrMBX.s32SendMutexMBX = MsOS_CreateMutex(E_MSOS_FIFO, "MBX_Send_Mutex", MSOS_PROCESS_SHARED);\
                                    if(_mgrMBX.s32SendMutexMBX<0)\
                                    {\
                                        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "!!!Acquire Send Mutex for MBX Failed!\n");\
                                        return E_MBX_UNKNOW_ERROR;\
                                    }\
                                    _mgrMBX.s32RecvMutexMBX = MsOS_CreateMutex(E_MSOS_FIFO, "MBX_Recv_Mutex", MSOS_PROCESS_SHARED);\
                                    if(_mgrMBX.s32RecvMutexMBX<0)\
                                    {\
                                        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "!!!Acquire Recv Mutex for MBX Failed!\n");\
                                        MsOS_DeleteMutex(_mgrMBX.s32SendMutexMBX);\
                                        return E_MBX_UNKNOW_ERROR;\
                                    }\
                               }
#define DRV_MBX_DeInitLock()   {\
                                    MsOS_DeleteMutex(_mgrMBX.s32SendMutexMBX);\
                                    MsOS_DeleteMutex(_mgrMBX.s32RecvMutexMBX);\
                                 }

#ifdef MSOS_TYPE_LINUX
    #define DRV_MBX_LockSend()   {\
                                        {\
                                            MsOS_ObtainMutex(_mgrMBX.s32SendMutexMBX, MSOS_WAIT_FOREVER);\
                                        }\
                                   }
    #define DRV_MBX_UnLockSend()   {\
                                        {\
                                            MsOS_ReleaseMutex(_mgrMBX.s32SendMutexMBX);\
                                        }\
                                     }

    #define DRV_MBX_LockRecv()   {\
                                        if(!_mgrMBX.bInInterruptHandler)\
                                        {\
                                            MsOS_ObtainMutex(_mgrMBX.s32RecvMutexMBX, MSOS_WAIT_FOREVER);\
                                            _mgrMBX.u32IntLockerMBX = MsOS_DisableAllInterrupts();\
                                        }\
                                   }
    #define DRV_MBX_UnLockRecv()   {\
                                        if(!_mgrMBX.bInInterruptHandler)\
                                        {\
                                            MsOS_RestoreAllInterrupts(_mgrMBX.u32IntLockerMBX); \
                                            MsOS_ReleaseMutex(_mgrMBX.s32RecvMutexMBX);\
                                        }\
                                     }
#endif

//=============================================================================
// Local Variables
//=============================================================================
static MS_S32 _s32FdMBX = -1;
static MBX_MAIL_ARRIVE_NOTIFY_FUNC _mbxQStatusNotifierMBX[E_MBX_CLASS_MAX] = { NULL };

static MS_U8 _u8DbgLevel = MBX_DBG_LEVEL_NONE;

/// Version string
static MSIF_Version _drv_mbx_version = {
    .DDI = { MBX_DRV_VERSION, },
};
//for Fastboot
static MBX_CPU_ID g_eHKCPU;
static MBX_ROLE_ID g_eHostRole; 
static MS_U32 g_u32TimeoutMillSecs;

typedef struct
{
    MS_BOOL bInInterruptHandler;
    MS_U32 u32IntLockerMBX;
    MS_S32 s32SendMutexMBX;
    MS_S32 s32RecvMutexMBX;
}MBX_Mgr;

static MBX_Mgr _mgrMBX =
{
    .bInInterruptHandler = FALSE, ////suppose two interrupt has the same priority, won't break each other.
    .s32SendMutexMBX = -1,
    .s32RecvMutexMBX = -1,
};

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================
static void _MDrv_MBX_MsgArrived(int signo, siginfo_t *sig_info,void *unused);

//=============================================================================
// Local Function

//-------------------------------------------------------------------------------------------------
// Handle  the signal of "message arrived"
// @param  signo                  \b IN: the signal number
// @param  sig_info                   \b IN: the signal information attached
// @param  unused                  \b IN: unused
// @return @ref void
// @attention
// <b>[OBAMA] <em>If no callback with application, just skip the signal</em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MBX_MsgArrived(int signo, siginfo_t *sig_info,void *unused)
{
    MBX_Class msgClass;
    //MBX_MSGQ_Status msgQueueStatus;
    MBX_Msg mbxMsg;
    MBX_Result mbxResult;
    MS_BOOL bAddToQueue;

    int err = errno;

    //get message class:
    if(SIGIO_MAILBOX != signo)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[_MDrv_MBX_MsgArrived]Wrong Signal callback received! \n");
    }

    //get message class:
    msgClass = (MBX_Class)((sig_info->si_code&MBXCLASS_IN_SIGNAL_MASK)>>MBXCLASS_IN_SIGNAL_SHIFT);

    MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "R Class %d\n", msgClass);
    //MDrv_MBX_GetMsgQueueStatus(msgClass, &msgQueueStatus);
    //MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "R Normal Msg Count %d\n", msgQueueStatus.u32NormalMsgCount);

    if(NULL == _mbxQStatusNotifierMBX[msgClass])
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "[_MDrv_MBX_MsgArrived]Signal callback is NULL! \n");
        errno = err;
        return;
    }

    //mbxResult = MDrv_MBX_GetMsgQueueStatus(msgClass, &msgQueueStatus);
    //mbxResult = MDrv_MBX_RecvMsg (msgClass, &mbxMsg, 0, MBX_CHECK_ALL_MSG_CLASS);
    mbxResult = MDrv_MBX_CheckMsg (msgClass, &mbxMsg, 0, MBX_CHECK_ALL_MSG_CLASS);
    if(E_MBX_SUCCESS == mbxResult)
    {
        //_mbxQStatusNotifierMBX[msgClass](msgClass, &msgQueueStatus);
        _mbxQStatusNotifierMBX[msgClass](&mbxMsg, &bAddToQueue);
        if(!bAddToQueue)
            MDrv_MBX_RemoveLatestMsg();
    }
    else
    {
        MBX_DBG_PRINT((MBX_DBG_LEVEL_MBXINT|MBX_DBG_LEVEL_MSGQ), "[_MDrv_MBX_MsgArrived]MDrv_MBX_GetMsgQueueStatus Failed! Result=%d \n", mbxResult);
    }
    errno = err;
}

//=============================================================================
// Mailbox Driver Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Init Mailbox driver
/// @param  eHKCPU    \b IN: The configged HK CPU ID\n
/// @param  eHostRole    \b IN: The configged Host ID which mbx driver will run on\n
/// @param  u32TimeoutMillSecs    \b IN: The configged timeout which mbx driver will send & wait\n
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_NO_MORE_MEMORY
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  _MDrv_MBX_Init_(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs)
{
    MS_MBX_INIT_INFO stInitInfo;
    struct sigaction action;

    MDrv_SYS_DMD_VD_MBX_Init();
    //check input Parameter:
    if((eHKCPU < E_MBX_CPU_AEON) || (eHKCPU >= E_MBX_CPU_MAX))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init] Invalid HK CPU ID \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    //if((eHostRole < E_MBX_ROLE_HK) || (eHostRole >= E_MBX_ROLE_MAX)) // fix coverity issue
    if(eHostRole >= E_MBX_ROLE_MAX)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init] Invalid Host Role ID \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(u32TimeoutMillSecs == 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init] Invalid Timeout Value \n");
        return E_MBX_ERR_INVALID_PARAM;
    }
	//for fastboot
	g_eHKCPU = eHKCPU;
	g_eHostRole = eHostRole;
	g_u32TimeoutMillSecs = u32TimeoutMillSecs;

    stInitInfo.eHKCPU = eHKCPU;
    stInitInfo.eHostRole = eHostRole;
    stInitInfo.u32TimeoutMillSecs = u32TimeoutMillSecs;
    stInitInfo.mbxResult = E_MBX_SUCCESS;

    if (_s32FdMBX < 0)   //First time open
    {
        _s32FdMBX  = open("/dev/msmailbox", O_RDWR);

        if (_s32FdMBX > 0) //open device success
        {
            if(ioctl(_s32FdMBX, MDRV_MBX_IOC_INIT, &stInitInfo))
            {
                MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init]MDRV_MBX_IOC_INIT Failed! \n");
                return E_MBX_UNKNOW_ERROR;
            }

            if(stInitInfo.mbxResult != E_MBX_SUCCESS)
            {
                MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init]MDrv_MBX_Init Failed! Result=%d \n", stInitInfo.mbxResult);
                return stInitInfo.mbxResult;
            }

            DRV_MBX_InitLock();

            //handle signal related logics:
            {
                if (fcntl(_s32FdMBX, F_SETOWN, getpid()) < 0)
					return E_MBX_UNKNOW_ERROR;
                if (fcntl(_s32FdMBX, F_SETSIG, SIGIO_MAILBOX) < 0)
					return E_MBX_UNKNOW_ERROR;
                if (fcntl(_s32FdMBX, F_SETFL, FASYNC) < 0) //register it right now
					return E_MBX_UNKNOW_ERROR;

                // register the handler for signal "SIGIO"
                memset(&action, 0, sizeof(action));
                sigemptyset(&action.sa_mask);
                action.sa_sigaction = _MDrv_MBX_MsgArrived;
                action.sa_flags = SA_SIGINFO;
                sigaction(SIGIO_MAILBOX, &action, NULL);
            }
        }
        else
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init]Open Device Failed! \n");
        }
    }
    else
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init]Device Already Opened! \n");
    }

    return stInitInfo.mbxResult;
}

extern MBX_Result  MApi_MBX_Init(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs);

MBX_Result  MDrv_MBX_Init(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs)
{
	return MApi_MBX_Init(eHKCPU, eHostRole, u32TimeoutMillSecs);
}

//-------------------------------------------------------------------------------------------------
/// Deinitialize Mailbox driver
/// @param  bForceDiscardPendingMsg    \b IN: If Discard Pending Mail Message or not \n
///              - # TRUE: Discard it and De-Init
///              - # FALSE: if has pending mail message,
///                     No Discard and Do not De-Init, return E_MBX_ERR_HAS_MSG_PENDING
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_HAS_MSG_PENDING
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_DeInit(MS_BOOL bForceDiscardPendingMsg)
{
    MS_MBX_SET_BINFO stSetBInfo = { FALSE, E_MBX_UNKNOW_ERROR };

    if(!bForceDiscardPendingMsg) //if do not discard if still has mail message
    {
        stSetBInfo.bInfo = bForceDiscardPendingMsg;

        if (ioctl(_s32FdMBX, MDRV_MBX_IOC_DEINIT, &stSetBInfo))
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_DeInit]MDRV_MBX_IOC_DEINIT Failed! \n");
            return E_MBX_UNKNOW_ERROR;
        }

        if(E_MBX_ERR_HAS_MSG_PENDING == stSetBInfo.mbxResult) //there has pending mail message!
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_DeInit]DeInit Failed! Result=E_MBX_ERR_HAS_MSG_PENDING! \n");
            return E_MBX_ERR_HAS_MSG_PENDING;
        }
    }

    //no pending Mail Message, or just discard pending mail message:
    _s32FdMBX = close(_s32FdMBX);
    _s32FdMBX = -1;

    return stSetBInfo.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Regisiter Message queue for special class
/// @param  eMsgClass \b IN: Mail Message Class, @ref MBX_Class
/// @param  u16MsgQueueSize \b IN: The mail message queue size,
///                                The Max should not exceed @ref MAX_MBX_QUEUE_SIZE
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_NO_MORE_MEMORY
/// @return E_MBX_ERR_SLOT_BUSY
/// @return E_MBX_ERR_SLOT_AREADY_OPENNED
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_RegisterMSG(MBX_Class eMsgClass, MS_U16 u16MsgQueueSize)
{
    MS_MBX_REGISTER_MSG stRegisterMsg;

    //parameter check:
    if(INVALID_MBXCLASS(eMsgClass) || (u16MsgQueueSize>MAX_MBX_QUEUE_SIZE))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RegisterMSG]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    stRegisterMsg.eMsgClass= eMsgClass;
    stRegisterMsg.u16MsgQueueSize = u16MsgQueueSize;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_REGISTERMSG, &stRegisterMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RegisterMSG]MDRV_MBX_IOC_REGISTERMSG Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

	if (E_MBX_ERR_SLOT_AREADY_OPENNED == stRegisterMsg.mbxResult)
		stRegisterMsg.mbxResult = E_MBX_SUCCESS;


    if(E_MBX_SUCCESS != stRegisterMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_RegisterMSG]MDrv_MBX_RegisterMSG Failed! Result=%d \n", stRegisterMsg.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stRegisterMsg.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Regisiter Message queue for special class With Callback Handler Function
/// @param  eMsgClass \b IN: Mail Message Class, @ref MBX_Class
/// @param  u16MsgQueueSize \b IN: The mail message queue size,
///                                The Max should not exceed @ref MAX_MBX_QUEUE_SIZE
/// @param  notifier \b IN: status change notify call function about this message queue, could be NULL
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_NOT_IMPLEMENTED
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_NO_MORE_MEMORY
/// @return E_MBX_ERR_SLOT_BUSY
/// @return E_MBX_ERR_SLOT_AREADY_OPENNED
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_RegisterMSGWithCallBack(MBX_Class eMsgClass, MS_U16 u16MsgQueueSize, MBX_MAIL_ARRIVE_NOTIFY_FUNC notifier)
{
    MBX_Result mbxResult = E_MBX_SUCCESS;

//    _mbxQStatusNotifierMBX[eMsgClass] = notifier;

    mbxResult = MDrv_MBX_RegisterMSG(eMsgClass, u16MsgQueueSize);
    if(mbxResult == E_MBX_SUCCESS)
    {
        _mbxQStatusNotifierMBX[eMsgClass] = notifier;
    }

    return mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// UnRegisiter Message queue for special class
/// @param  eMsgClass \b IN: Mail Message Class, @ref MBX_Class
/// @param  bForceDiscardMsgQueue    \b IN: If Discard Pending Mail Message or not \n
///              - # TRUE: Discard it and De-Init
///              - # FALSE: if has pending mail message,
///                     No Discard and Do not De-Init, return E_MBX_ERR_HAS_MSG_PENDING
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_ERR_HAS_MSG_PENDING
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_UnRegisterMSG(MBX_Class eMsgClass, MS_BOOL bForceDiscardMsgQueue)
{
    MS_MBX_UNREGISTER_MSG stUnRegisterMsg;

    //parameter check:
    if(INVALID_MBXCLASS(eMsgClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_UnRegisterMSG]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    stUnRegisterMsg.eMsgClass = eMsgClass;
    stUnRegisterMsg.bForceDiscardMsgQueue = bForceDiscardMsgQueue;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_UNREGISTERMSG, &stUnRegisterMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_UnRegisterMSG]MDRV_MBX_IOC_UNREGISTERMSG Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stUnRegisterMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_UnRegisterMSG]MDrv_MBX_UnRegisterMSG Failed! Result=%d \n", stUnRegisterMsg.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stUnRegisterMsg.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Clear Message queue for special class
/// @param  eMsgClass \b IN: Mail Message Class, @ref MBX_Class
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_ClearMSG(MBX_Class eMsgClass)
{
    MS_MBX_CLEAR_MSG stClearMsg;

    //parameter check:
    if(INVALID_MBXCLASS(eMsgClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_ClearMSG]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    stClearMsg.eMsgClass = eMsgClass;

    if (ioctl(_s32FdMBX, MDRV_mBX_IOC_CLEARMSG, &stClearMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_ClearMSG]MDRV_mBX_IOC_CLEARMSG Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stClearMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_ClearMSG]MDrv_MBX_ClearMSG Failed! Result=%d \n", stClearMsg.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stClearMsg.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Send message
/// @param  pMsg \b IN: Mail Message Class for sending, must not be NULL, @ref MBX_Msg
/// @param  bWaitPeerIdle \b IN: wait Peer CPU or not
/// @param  u32WaitMillSecs \b IN: if bWaitPeerIdle, indentify the timeout millsecons before return
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_ERR_INVALID_CPU_ID
/// @return E_MBX_ERR_PEER_CPU_BUSY
/// @return E_MBX_ERR_PEER_CPU_NOT_ALIVE
/// @return E_MBX_ERR_TIME_OUT
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_SendMsg(MBX_Msg *pMsg)
{
    MS_MBX_SEND_MSG stSendMsg;

    //parameter check:
    //if((NULL==pMsg) || INVALID_MBXCLASS(pMsg->u8MsgClass))
    if((NULL==pMsg) || (pMsg->u8MsgClass>=E_MBX_CLASS_MAX))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockSend();
    stSendMsg.pMsg = pMsg;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_SENDMSG, &stSendMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]MDRV_MBX_IOC_SENDMSG Failed! \n");
        DRV_MBX_UnLockSend();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stSendMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_SendMsg]MDrv_MBX_SendMsg Failed! Result=%d \n", stSendMsg.mbxResult);
    }
    DRV_MBX_UnLockSend();

    return stSendMsg.mbxResult;
}

MBX_Result  MDrv_MBX_SendMsgLoopback(MBX_Msg *pMsg, MBX_ROLE_ID eSrcRoleId)
{
    MS_MBX_SEND_MSG stSendMsg;

    //parameter check:

    //if((NULL==pMsg) || INVALID_MBXCLASS(pMsg->u8MsgClass))
    if((NULL==pMsg) || (pMsg->u8MsgClass>=E_MBX_CLASS_MAX))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockSend();
    stSendMsg.pMsg = pMsg;
    stSendMsg.mbxResult = (MBX_Result)eSrcRoleId;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_SENDMSGLOOPBACK, &stSendMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]MDRV_MBX_IOC_SENDMSG Failed! \n");
        DRV_MBX_UnLockSend();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stSendMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_SendMsg]MDrv_MBX_SendMsg Failed! Result=%d \n", stSendMsg.mbxResult);
    }
    DRV_MBX_UnLockSend();

    return stSendMsg.mbxResult;
}


//-------------------------------------------------------------------------------------------------
/// Check the message queue status of a special message class
/// @param  eTargetClass \b IN: target class, @ref MBX_Class
/// @param  pMsgQueueStatus \b IN: return the queue status, @ref MBX_MSGQ_Status
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_GetMsgQueueStatus(MBX_Class eTargetClass, MBX_MSGQ_Status *pMsgQueueStatus)
{
    MS_MBX_GET_MSGQSTATUS stGetMsgQStatus;

    //parameter check:
    if((pMsgQueueStatus ==NULL) || INVALID_MBXCLASS(eTargetClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetMsgQueueStatus]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    stGetMsgQStatus.eTargetClass = eTargetClass;
    stGetMsgQStatus.pMsgQueueStatus = pMsgQueueStatus;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_GETMSGQSTATUS, &stGetMsgQStatus))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetMsgQueueStatus]MDRV_MBX_IOC_GETMSGQSTATUS Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stGetMsgQStatus.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_SendMsg]MDrv_MBX_SendMsg Failed! Result=%d \n", stGetMsgQStatus.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stGetMsgQStatus.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Recv message
/// @param  eTargetClass \b IN: target class, @ref MBX_Class
/// @param  pMsg \b IN: Mail Message Class for receive, must not be NULL, @ref MBX_Msg
/// @param  u32WaitMillSecs \b IN: if MBX_CHECK_BLOCK_RECV, indentify the timeout millsecons before return
/// @param  u32Flag \b IN: recv flag, has the following possible meaning bits
///                  - # MBX_CHECK_ALL_MSG_CLASS: recv any message class, this means, the targetClass is useless if this bit is set.
///                  - # MBX_CHECK_INSTANT_MSG: check INSTANT message.
///                  - # MBX_CHECK_NORMAL_MSG: Check Normal message.
///                  - # MBX_CHECK_BLOCK_RECV: block this function call until time out if no message available.
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_ERR_NO_MORE_MSG
/// @return E_MBX_ERR_TIME_OUT
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_RecvMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32WaitMillSecs, MS_U32 u32Flag)
{
    MS_MBX_RECV_MSG stRecvMsg;

    //parameter check:
    if((pMsg ==NULL) || INVALID_MBXCLASS(eTargetClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RecvMsg]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    stRecvMsg.eTargetClass = eTargetClass;
    stRecvMsg.pMsg = pMsg;
    stRecvMsg.u32WaitMillSecs = u32WaitMillSecs;
    stRecvMsg.u32Flag = u32Flag;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_RECVMSG, &stRecvMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RecvMsg]MDRV_MBX_IOC_RECVMSG Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stRecvMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_RecvMsg]MDrv_MBX_RecvMsg Failed! Result=%d \n", stRecvMsg.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stRecvMsg.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Check message
/// @param  eTargetClass \b IN: target class, @ref MBX_Class
/// @param  pMsg \b IN: Mail Message Class for receive, must not be NULL, @ref MBX_Msg
/// @param  u32WaitMillSecs \b IN: if MBX_CHECK_BLOCK_RECV, indentify the timeout millsecons before return
/// @param  u32Flag \b IN: recv flag, has the following possible meaning bits
///                  - # MBX_CHECK_ALL_MSG_CLASS: recv any message class, this means, the targetClass is useless if this bit is set.
///                  - # MBX_CHECK_INSTANT_MSG: check INSTANT message.
///                  - # MBX_CHECK_NORMAL_MSG: Check Normal message.
///                  - # MBX_CHECK_BLOCK_RECV: block this function call until time out if no message available.
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_ERR_NO_MORE_MSG
/// @return E_MBX_ERR_TIME_OUT
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_CheckMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32WaitMillSecs, MS_U32 u32Flag)
{
    MS_MBX_RECV_MSG stRecvMsg;

    //parameter check:
    if((pMsg ==NULL) || INVALID_MBXCLASS(eTargetClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_CheckMsg]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    stRecvMsg.eTargetClass = eTargetClass;
    stRecvMsg.pMsg = pMsg;
    stRecvMsg.u32WaitMillSecs = u32WaitMillSecs;
    stRecvMsg.u32Flag = u32Flag;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_CHECKMSG, &stRecvMsg))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_CheckMsg]MDRV_MBX_IOC_CHECKMSG Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stRecvMsg.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_CheckMsg]MDrv_MBX_CheckMsg Failed! Result=%d \n", stRecvMsg.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stRecvMsg.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Remove the latest received message from queue, this function is used right after MDrv_MBX_CheckMsg()
/// @param  none
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_RemoveLatestMsg(void)
{
    MS_MBX_SET_BINFO stSetBInfo;

    DRV_MBX_LockRecv();
    stSetBInfo.bInfo = TRUE;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_REMOVEMSG, &stSetBInfo))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RemoveLatestMsg]MDrv_MBX_RemoveLatestMsg Failed! \n");
        DRV_MBX_UnLockRecv();
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stSetBInfo.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_RemoveLatestMsg]MDrv_MBX_RemoveLatestMsg Failed! Result=%d \n", stSetBInfo.mbxResult);
    }
    DRV_MBX_UnLockRecv();

    return stSetBInfo.mbxResult;
}


//-------------------------------------------------------------------------------------------------
/// Enable Message Queue to recv message from peer CPUs, Enable receiving regisited messages in kernel
/// @param  bEnable \b IN: Enable or not
///                  - # TRUE: enable
///                  - # FALSE: disable
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_Enable(MS_BOOL bEnable)
{
    MS_MBX_SET_BINFO stSetBInfo;

    stSetBInfo.bInfo = bEnable;

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_MBXENABLE, &stSetBInfo))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Enable]MDRV_MBX_IOC_MBXENABLE Failed! \n");
        return E_MBX_UNKNOW_ERROR;
    }

    if(E_MBX_SUCCESS != stSetBInfo.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_Enable]MDrv_MBX_Enable Failed! Result=%d \n", stSetBInfo.mbxResult);
    }

    return stSetBInfo.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Set Information to dedicated Mailbox HW Regs.
/// @param  eTargetCPU \b IN: The correpsonded CPU ID for which MBX HW Group will be used for Set
/// @param  pU8Info \b IN: The Information which need to set
/// @param  u8Size \b IN: The Size(bytes) of pU8Info
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em>It is used for speciall need when Init-Coprocesser</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_SetInformation(MBX_ROLE_ID eTargetRole, MS_U8 *pU8Info, MS_U8 u8Size)
{
    MS_MBX_CPROSYNC_INFORMATION stSetInfo;

    //Check if Valid Parameter:
    if((eTargetRole != E_MBX_ROLE_CP) && (eTargetRole != E_MBX_ROLE_PM) && (eTargetRole != E_MBX_ROLE_FRC))//frcr2_integration###
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation] Invalid Role ID \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(NULL == pU8Info)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation]Invalid pU8Info! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(u8Size > MAX_MBX_INFORMATION_SIZE)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation]Invalid Information Size! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    stSetInfo.eTargetRole = eTargetRole;
    stSetInfo.pU8Info = pU8Info;
    stSetInfo.u8Size = u8Size;

    if(_s32FdMBX < 0)   //First time open
    {
        _s32FdMBX  = open("/dev/msmailbox", O_RDWR);

        if (_s32FdMBX > 0) //open device success
        {
            if (ioctl(_s32FdMBX, MDRV_MBX_IOC_SETINFORMATION, &stSetInfo))
            {
                MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation]MDRV_MBX_IOC_SETINFORMATION Failed! \n");
                return E_MBX_UNKNOW_ERROR;
            }
        }
        else //open Fail.
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation]Open Device Failed! \n");
            return E_MBX_UNKNOW_ERROR;
        }

        _s32FdMBX = close(_s32FdMBX);
        _s32FdMBX = -1;
    }
    else
    {
        if (ioctl(_s32FdMBX, MDRV_MBX_IOC_SETINFORMATION, &stSetInfo))
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation]MDRV_MBX_IOC_SETINFORMATION Failed! \n");
            return E_MBX_UNKNOW_ERROR;
        }
    }

    if(E_MBX_SUCCESS != stSetInfo.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_SetInformation]MDrv_MBX_SetInformation Failed! Result=%d \n", stSetInfo.mbxResult);
    }

    return stSetInfo.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Get Information from dedicated Mailbox HW Regs.
/// @param  eTargetCPU \b IN: The correpsonded CPU ID for which MBX HW Group will be used for Get
/// @param  pU8Info \b OUT: The Information Where we get to put
/// @param  u8Size \b IN: The Size(bytes) need to get
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em>It is used for speciall need when Init-Coprocesser.</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_GetInformation(MBX_ROLE_ID eTargetRole, MS_U8 *pU8Info, MS_U8 u8Size)
{
    MS_MBX_CPROSYNC_INFORMATION stGetInfo;

    //Check if Valid Parameter:
    if((eTargetRole != E_MBX_ROLE_CP) && (eTargetRole != E_MBX_ROLE_PM) && (eTargetRole != E_MBX_ROLE_FRC))//frcr2_integration###
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation] Invalid Role ID \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(NULL == pU8Info)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetInformation]Invalid pU8Info! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(u8Size > MAX_MBX_INFORMATION_SIZE)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetInformation]Invalid Information Size! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    stGetInfo.eTargetRole = eTargetRole;
    stGetInfo.pU8Info = pU8Info;
    stGetInfo.u8Size = u8Size;

    if(_s32FdMBX < 0)   //First time open
    {
        _s32FdMBX = open("/dev/msmailbox", O_RDWR);

        if(_s32FdMBX > 0) //open device success
        {
            if(ioctl(_s32FdMBX, MDRV_MBX_IOC_GETINFORMATION, &stGetInfo))
            {
                MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetInformation]MDRV_MBX_IOC_GETINFORMATION Failed! \n");
                return E_MBX_UNKNOW_ERROR;
            }
        }
        else //open Fail.
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetInformation]Open Device Failed! \n");
            return E_MBX_UNKNOW_ERROR;
        }

        _s32FdMBX = close(_s32FdMBX);
        _s32FdMBX = -1;
    }
    else
    {
        if(ioctl(_s32FdMBX, MDRV_MBX_IOC_GETINFORMATION, &stGetInfo))
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetInformation]MDRV_MBX_IOC_GETINFORMATION Failed! \n");
            return E_MBX_UNKNOW_ERROR;
        }
    }

    if(E_MBX_SUCCESS != stGetInfo.mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "[MDrv_MBX_GetInformation]MDrv_MBX_GetInformation Failed! Result=%d \n", stGetInfo.mbxResult);
    }

    return stGetInfo.mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Set the Debug Lever to MBX Driver
/// @param  u8DbgLevel    \b IN: The debug level. will be the value of belows \n
///              - # MBX_DBG_LEVEL_NONE: No any Debug Message \n
///              - # MBX_DBG_LEVEL_MBXHAL: Print HAL Related Message \n
///              - # MBX_DBG_LEVEL_MSGQ: Print MSGQ Related Message \n
///              - # MBX_DBG_LEVEL_MBXINT: Print Interrupt callback realted Message \n
///              - # MBX_DBG_LEVEL_CRITICAL: Print critical Message \n
///              - # MBX_DBG_LEVEL_ALL: Print All above kinds of Message
/// @return void
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void MDrv_MBX_SetDbgLevel(MS_U8 u8DbgLevel)
{
    _u8DbgLevel = u8DbgLevel;
}

//------------------------------------------------------------------------------
/// Get MBX driver version
/// @return -the pointer to the driver version
//------------------------------------------------------------------------------
MS_BOOL MDrv_MBX_GetLibVer(const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return FALSE;
    }

    *ppVersion = &_drv_mbx_version;
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get MBX driver Info
/// @return -the pointer to the driver Ifno
//------------------------------------------------------------------------------
MS_BOOL MDrv_MBX_GetInfo(MBX_DrvInfo *pMbxDrvInfo)
{
    if (NULL == pMbxDrvInfo)
    {
        return FALSE;
    }

    pMbxDrvInfo->u8HWMBXGroupNum = 0x6;
    return TRUE;
}

//------------------------------------------------------------------------------
/// Get MBX driver status
/// @return -the pointer to the driver status
//------------------------------------------------------------------------------
MS_BOOL MDrv_MBX_GetStatus(MBX_DrvStatus *pMbxDrvStatus)
{
    MS_MBX_GET_DRVSTATUS stGetDrvStatus;

    if (NULL == pMbxDrvStatus)
    {
        return FALSE;
    }

    if (ioctl(_s32FdMBX, MDRV_MBX_IOC_GETDRVSTATUS, &stGetDrvStatus))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetStatus]MDRV_MBX_IOC_MBXGETDRVSTATUS Failed! \n");
        return FALSE;
    }

    pMbxDrvStatus->bEnabled = stGetDrvStatus.bEnabled;
    pMbxDrvStatus->s32RefCnt = stGetDrvStatus.s32RefCnt;

    return TRUE;
}

MS_U16 MDrv_MBX_SetPowerState(EN_POWER_MODE u16PowerState)
{
	static EN_POWER_MODE _prev_u16PowerState = E_POWER_MECHANICAL;
	MS_U16 u16Return = FALSE;

	if (u16PowerState == E_POWER_SUSPEND)
	{
		_prev_u16PowerState = u16PowerState;
		u16Return = 2;//SUSPEND_OK;
	}
	else if (u16PowerState == E_POWER_RESUME)
	{

		if (_prev_u16PowerState == E_POWER_SUSPEND)
		{
			 MDrv_MBX_Init(g_eHKCPU, g_eHostRole, g_u32TimeoutMillSecs);
			_prev_u16PowerState = u16PowerState;
			u16Return = 1;//RESUME_OK;
		}
		else
		{
			ULOGE(TAG_MBX, "[%s,%5d]It is not suspended yet. We shouldn't resume\n",__FUNCTION__,__LINE__);
			u16Return = 3;//SUSPEND_FAILED;
		}
	}
	else
	{
		ULOGE(TAG_MBX, "[%s,%5d]Do Nothing: %d\n",__FUNCTION__,__LINE__,u16PowerState);
		u16Return = FALSE;
	}

	return u16Return;// for success
}

void MDrv_MBX_SetCallDrvFlag(MS_BOOL bEnable)
{   
}

MS_BOOL MDrv_MBX_GetCallDrvFlag(void)
{
    return TRUE;
}

