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
/// file    drvMBX.c
/// @brief  MStar MailBox DDI
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _DRV_MBX_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMBX.h"
#include "halMBX.h"
#include "halMBXINT.h"
#include "drvMBXMsgPool.h"
#include "drvMMIO.h"
#include "ULog.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================
#define TAG_MBX "MBX"

//=============================================================================
// Debug Macros
//=============================================================================
#define MBX_DBG_PRINT(dbgLevel, fmt, args...)  { \
                                                    if(_mgrMBX.u8DbgLevel&dbgLevel) \
                                                    { \
                                                        MBX_PRINT(fmt, ## args); \
                                                    } \
                                                }
//#define MBX_DEBUG
#ifdef MBX_DEBUG
    #define MBX_ERROR(fmt, args...)  ULOGE(TAG_MBX, "[MBX Driver USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MBX_WARN(fmt, args...)   ULOGW(TAG_MBX, "[MBX Driver WARN][%06d]    " fmt, __LINE__, ## args)
    #define MBX_PRINT(fmt, args...)  ULOGD(TAG_MBX, "[MBX Driver][%06d]     " fmt, __LINE__, ## args)
    #define MBX_ASSERT(_cnd, _fmt, _args...)    \
                                    if (!(_cnd)) {              \
                                        MBX_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define MBX_ERROR(fmt, args...)
    #define MBX_WARN(fmt, args...)
    #define MBX_PRINT(fmt, args...)  ULOGD(TAG_MBX, "[MBX Driver][%06d]     " fmt, __LINE__, ## args)
    #define MBX_ASSERT(_cnd, _fmt, _args...)
#endif
//=============================================================================
// Macros
//=============================================================================
#define INVALID_MBXCLASS(mbxClass) 	((mbxClass) >= E_MBX_CLASS_MAX)

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

#ifdef MSOS_TYPE_NOS
    extern void mhal_interrupt_lock(void);
    extern void mhal_interrupt_unlock(void);

    #define DRV_MBX_LockSend()
    #define DRV_MBX_UnLockSend()

    #define DRV_MBX_LockRecv()  {\
                                                if(!_mgrMBX.bInInterruptHandler)\
                                                {\
                                                    mhal_interrupt_lock();\
                                                }\
                                              }
    #define DRV_MBX_UnLockRecv()    {\
                                                        if(!_mgrMBX.bInInterruptHandler)\
                                                        {\
                                                            mhal_interrupt_unlock();\
                                                        }\
                                                    }

#elif defined(MSOS_TYPE_ECOS) || defined(MSOS_TYPE_UCOS)
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
#else
	#define DRV_MBX_LockSend()
	#define DRV_MBX_UnLockSend()
	#define DRV_MBX_LockRecv()
	 #define DRV_MBX_UnLockRecv()
#endif

//=============================================================================
// Local Variables
//=============================================================================
typedef struct
{
    MBX_ROLE_ID eMbxHostRole;
    MBX_ROLE_ID eMbxCpu2Role[E_MBX_CPU_MAX];
    MBX_CPU_ID eMbxRole2Cpu[E_MBX_ROLE_MAX];
    MS_U32 u32TimeoutMillSecs;
    //MBX_QUEUE_STATUS_NOTIFY_FUNC mbxQStatusNotifier[E_MBX_CLASS_MAX];
    MBX_MAIL_ARRIVE_NOTIFY_FUNC mbxQStatusNotifier[E_MBX_CLASS_MAX];
    MS_S32 s32RefCnt;
    MS_BOOL bMsgRecvEnable;
    MS_BOOL bInInterruptHandler;
    MS_U32 u32IntLockerMBX;
    MS_S32 s32SendMutexMBX;
    MS_S32 s32RecvMutexMBX;
    MS_U8 u8DbgLevel;
}MBX_Mgr;

static MBX_Mgr _mgrMBX =
{
    .eMbxHostRole = E_MBX_ROLE_MAX,
    .mbxQStatusNotifier = { NULL },
    .s32RefCnt = 0,
    .bMsgRecvEnable = FALSE,
    .bInInterruptHandler = FALSE, ////suppose two interrupt has the same priority, won't break each other.
    .s32SendMutexMBX = -1,
    .s32RecvMutexMBX = -1,
    .u8DbgLevel = MBX_DBG_LEVEL_NONE,
};

/// Version string
static MSIF_Version _drv_mbx_version = {
    .DDI = { MBX_DRV_VERSION, },
};

static MS_BOOL _bCalledFromDriver = FALSE;

//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================

//MBX Config functions:
MBX_Result _MDrv_MBX_InitConfig(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs);
MBX_Result  _MDrv_MBX_SetConfig(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs);

//Message Handle functions:
static MBX_Result  _MDrv_MBX_SendMsg(MBX_Msg *pMsg, MBX_ROLE_ID eSrcRole);
static MBX_Result  _MDrv_MBX_RecvMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32Flag);
static void _MDrv_MBX_MsgRecvCb(InterruptNum eIntNum);
//=============================================================================
// Local Function
//=============================================================================

//=============================================================================
//MBX Config functions:

//-------------------------------------------------------------------------------------------------
// Init Config parameters to Driver Config Parameters
// @param  eHostCPU    \b IN: The config Host CPU ID\n
// @return E_MBX_SUCCESS
// @return E_MBX_ERR_INVALID_PARAM
// @return E_MBX_UNKNOW_ERROR
// @attention
// <b>[MXLIB] <em>Int Config in MDrv_MBX_Init, or After MDrv_MBX_Init</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result _MDrv_MBX_InitConfig(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs)
{
    //check input Parameter:
    if(MHAL_MBXINT_CpuInit(eHKCPU)!=E_MBX_SUCCESS)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init] Invalid HK CPU ID \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if((eHostRole < E_MBX_ROLE_HK) || (eHostRole >= E_MBX_ROLE_MAX))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init] Invalid Host Role ID \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(u32TimeoutMillSecs == 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Init] Invalid Timeout Value \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    //Set eMbxHostRole:
    _mgrMBX.eMbxHostRole = eHostRole;
    //Set Timeout:
    _mgrMBX.u32TimeoutMillSecs = u32TimeoutMillSecs;

    //Set eMbxRole2Cpu, eMbxCpu2Role
    _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_HK] = eHKCPU;
    _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_PM] = E_MBX_CPU_PM;
    _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_FRC] = E_MBX_CPU_R2FRC;//frcr2_integration###

    _mgrMBX.eMbxCpu2Role[E_MBX_CPU_PM] = E_MBX_ROLE_PM;
    _mgrMBX.eMbxCpu2Role[E_MBX_CPU_R2FRC] = E_MBX_ROLE_FRC;//frcr2_integration###

    if(E_MBX_CPU_AEON == eHKCPU)
    {
        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_AEON] = E_MBX_ROLE_HK;
        
#if defined(__mips__) || defined(__arm__)
        _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_CP] = E_MBX_CPU_MIPS;
        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_MIPS] = E_MBX_ROLE_CP;
#else
        _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_CP] = E_MBX_CPU_R2M;
        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_R2M] = E_MBX_ROLE_CP;
#endif
    }
#if defined(__mips__) || defined(__arm__)
    else if(E_MBX_CPU_MIPS == eHKCPU)
    {
        _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_CP] = E_MBX_CPU_AEON;

        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_AEON] = E_MBX_ROLE_CP;
        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_MIPS] = E_MBX_ROLE_HK;
    }
#else
    else if(E_MBX_CPU_R2M == eHKCPU)
    {
        _mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_CP] = E_MBX_CPU_AEON;

        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_AEON] = E_MBX_ROLE_CP;
        _mgrMBX.eMbxCpu2Role[E_MBX_CPU_R2M] = E_MBX_ROLE_HK;
    }
#endif
    else
    {
        return E_MBX_ERR_INVALID_PARAM;
    }

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
// Config Host CPU ID of Mailbox driver
// @param  eHostCPU    \b IN: The config Host CPU ID\n
// @return E_MBX_SUCCESS
// @return E_MBX_ERR_INVALID_PARAM
// @return E_MBX_ERR_NOT_INITIALIZED
// @return E_MBX_UNKNOW_ERROR
// @attention
// <b>[MXLIB] <em>Pls Set Config in MDrv_MBX_Init, or After MDrv_MBX_Init</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  _MDrv_MBX_SetConfig(MBX_CPU_ID eHKCPU, MBX_ROLE_ID eHostRole, MS_U32 u32TimeoutMillSecs)
{
    MBX_Result mbxResult = E_MBX_SUCCESS;

    //Check if MBX Already Initialized:
    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetConfig]Reference Cnt <= 0 \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //Config MBX:]
    if(u32TimeoutMillSecs == 0)
    {
        return E_MBX_ERR_INVALID_PARAM;
    }

    if((_mgrMBX.eMbxHostRole != eHostRole) || (_mgrMBX.eMbxRole2Cpu[E_MBX_ROLE_HK] != eHKCPU))
    {
        //Config MBX Hardware
        MHAL_MBX_SetConfig(eHostRole);

        mbxResult = MHAL_MBXINT_ResetHostCPU(_mgrMBX.eMbxRole2Cpu[_mgrMBX.eMbxHostRole], _mgrMBX.eMbxRole2Cpu[eHostRole]);
        if(E_MBX_SUCCESS != mbxResult)
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetConfig] Failed to reset Interrupt! result = %d\n", mbxResult);
            return mbxResult;
        }

        return _MDrv_MBX_InitConfig(eHKCPU, eHostRole, u32TimeoutMillSecs);
    }

    _mgrMBX.u32TimeoutMillSecs = u32TimeoutMillSecs;

    return mbxResult;
}


//=============================================================================
//Mail Message handler functions:

//-------------------------------------------------------------------------------------------------
// Send the message to MailBox
// @param  pMsg                  \b IN: The msg for sending
// @param  eSrcCPUID                  \b IN: the src cpu which send the mail message
// @return E_MBX_ERR_PEER_CPU_BUSY: peer role is still fetching the mail message
// @return E_MBX_ERR_PEER_CPU_NOT_ALIVE: fire bit is on while busy bit is off seems like peer cpu not alive
// @return E_MBX_ERR_PEER_CPU_OVERFLOW: peer role mail queue overflow, send fail
// @return E_MBX_ERR_PEER_CPU_NOTREADY: peer role is not enabled or initialized
// @return E_MBX_SUCCESS:success
// @attention
// <b>[MXLIB] <em> Use spin lock to protect co-access </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  _MDrv_MBX_SendMsg(MBX_Msg *pMsg, MBX_ROLE_ID eSrcRole)
{
    MBX_Result mbxResult = E_MBX_SUCCESS;
    MBXHAL_Fire_Status mbxHalFireStatus;
    MS_U32 u32WaitCnt = 0;
    MS_U32 u32TimeTicket;

    DRV_MBX_LockSend();

	MHAL_MBX_ClearAll(pMsg, eSrcRole);
    mbxResult = MHAL_MBX_Fire(pMsg, eSrcRole);
    if(E_MBX_SUCCESS == mbxResult)
    {//fire interrupt
        MHAL_MBXINT_Fire(_mgrMBX.eMbxRole2Cpu[pMsg->eRoleID], _mgrMBX.eMbxRole2Cpu[eSrcRole]);
        u32TimeTicket = MsOS_GetSystemTime();
        do
        {
            mbxResult = MHAL_MBX_GetFireStatus(eSrcRole, pMsg->eRoleID, &mbxHalFireStatus);

            if(E_MBX_SUCCESS != mbxResult)
            {
                MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "SendMail To Hardware[_MDrv_MBX_SendMsg] Result: %d \n ", mbxResult);
                break;
            }

            if(E_MBXHAL_FIRE_OVERFLOW == mbxHalFireStatus)
            {
                mbxResult = E_MBX_ERR_PEER_CPU_OVERFLOW;
                break;
            }

            if(E_MBXHAL_FIRE_DISABLED == mbxHalFireStatus)
            {
                mbxResult = E_MBX_ERR_PEER_CPU_NOTREADY;
                break;
            }

            if(E_MBXHAL_FIRE_SUCCESS == mbxHalFireStatus)
            {
                mbxResult = E_MBX_SUCCESS;
                break;
            }

            //check If Timeout:
            u32WaitCnt++;
            if(u32WaitCnt >= 0x10000)
            {
                if(MsOS_Timer_DiffTimeFromNow(u32TimeTicket) >= _mgrMBX.u32TimeoutMillSecs)
                {
                    MHAL_MBX_SetConfig(_mgrMBX.eMbxHostRole);
                    mbxResult = E_MBX_ERR_PEER_CPU_NOT_ALIVE;
                    break;
                }
                u32WaitCnt = 0;
            }
        }while(TRUE);
    }
    else
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXHAL, "SendMail To Hardware[_MDrv_MBX_SendMsg] Result: %d \n ", mbxResult);
    }

    DRV_MBX_UnLockSend();

    return mbxResult;
}

//-------------------------------------------------------------------------------------------------
// Recv the message from mail message queue
// @param  mbxAsyncNotifierID                  \b IN: Async Notifier ID
// @param  eTargetClass                  \b IN: The target class for mail message
// @param  pMsg                  \b OUT: fectch message
// @param  u32Flag                  \b IN: indicate MBX_CHECK_NORMAL_MSG / MBX_CHECK_INSTANT_MSG / MBX_CHECK_NORMAL_MSG
// @return E_MBX_ERR_INVALID_PARAM:no any flag setting in u32Flag
// @return E_MBX_ERR_NO_MORE_MSG: no any mail message in message pool
// @return E_MBX_SUCCESS:success
// @attention
// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  _MDrv_MBX_RecvMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32Flag)
{
    MBX_Result mbxResult = E_MBX_ERR_INVALID_PARAM;
    MS_S16 s16MsgQIdx;

    DRV_MBX_LockRecv();

    if(u32Flag == MBX_CHECK_ALL_MSG_CLASS)
    {
        //check Instant Msg for All registered class:
        if (u32Flag & MBX_CHECK_INSTANT_MSG)
        {
            for(s16MsgQIdx=0; s16MsgQIdx<E_MBX_CLASS_NUM; s16MsgQIdx++)
            {
                mbxResult = MDrv_MSGQ_RecvMsg(s16MsgQIdx, pMsg, TRUE);

                if(mbxResult == E_MBX_SUCCESS)
                {
                    DRV_MBX_UnLockRecv();
                    return mbxResult;
                }
            }
        }

        //check Normal Msg for All registered class:
        if (u32Flag & MBX_CHECK_NORMAL_MSG)
        {
            for(s16MsgQIdx=0; s16MsgQIdx<E_MBX_CLASS_NUM; s16MsgQIdx++)
            {
                mbxResult = MDrv_MSGQ_RecvMsg(s16MsgQIdx, pMsg, FALSE);

                if(mbxResult == E_MBX_SUCCESS)
                {
                    DRV_MBX_UnLockRecv();
                    return mbxResult;
                }
            }
        }

        DRV_MBX_UnLockRecv();
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[_MDrv_MBX_RecvMsg]RecvMail Fail Result=%d; Flag=%ld \n ", mbxResult, u32Flag);
        return mbxResult;
    }
    else
    {

    if(u32Flag & MBX_CHECK_INSTANT_MSG)
    {
        //check Instance Msg for All registered class:
        mbxResult = MDrv_MSGQ_RecvMsg(eTargetClass, pMsg, TRUE);
        if(mbxResult == E_MBX_SUCCESS)
        {
            DRV_MBX_UnLockRecv();
            return mbxResult;
        }
     }

    if(u32Flag & MBX_CHECK_NORMAL_MSG)
    {
        //check Instance Msg for All registered class:
        mbxResult = MDrv_MSGQ_RecvMsg(eTargetClass, pMsg, FALSE);
    }
    }

    DRV_MBX_UnLockRecv();

    if((E_MBX_SUCCESS != mbxResult) && (E_MBX_ERR_NO_MORE_MSG != mbxResult))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[_MDrv_MBX_RecvMsg]RecvMail Fail Result: %d; Flag: %ld \n ", mbxResult, u32Flag);
    }

    return mbxResult;
}

//-------------------------------------------------------------------------------------------------
// When Mail Arrived, the function will be called in interrupt tasklet. it will notify the UMD Driver the msg arrived.
// @param  s32Irq                  \b IN: The Irq Number which indicated the interrupt is from which to which.
// @return void
// @attention
// <b>[MXLIB] <em>get message class information from signal information. </em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MBX_MsgRecvCb(InterruptNum eIntNum)
{
    MBX_Msg mbxMsg;
    //MBX_MSGQ_Status msgQueueStatus;
    MBX_Result mbxResult;
    MS_BOOL bAddToQueue;


    _mgrMBX.bInInterruptHandler = TRUE;
    
  #if defined(CONFIG_FRC)
    switch(eIntNum)
    {
        case E_FRCINT_FIQ_HST0_TO_HST1:
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "Receive MAIL from HKCPU!\n");
            mbxMsg.eRoleID = _mgrMBX.eMbxCpu2Role[E_MBX_CPU_MIPS];
            break;
        default:
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "[_MDrv_MBX_MsgRecvCb]Wrong interrupt number called! eIntNum=%d \n", eIntNum);
            _mgrMBX.bInInterruptHandler = FALSE;
            return;
    }
  #else
    switch(eIntNum)
    {
        case E_INT_FIQ_AEON_TO_8051:
        case E_INT_FIQ_AEON_TO_BEON:
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "Receive MAIL from AEON!\n");
            mbxMsg.eRoleID = _mgrMBX.eMbxCpu2Role[E_MBX_CPU_AEON];
            break;
        case E_INT_FIQ_BEON_TO_8051:
        case E_INT_FIQ_BEON_TO_AEON:

#if defined(__mips__) || defined(__arm__)
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "Receive MAIL from MIPS!\n");
            mbxMsg.eRoleID = _mgrMBX.eMbxCpu2Role[E_MBX_CPU_MIPS];
#else
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "Receive MAIL from R2M!\n");
            mbxMsg.eRoleID = _mgrMBX.eMbxCpu2Role[E_MBX_CPU_R2M];
#endif
            break;
        case E_INT_FIQ_8051_TO_AEON:
        case E_INT_FIQ_8051_TO_BEON:
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "Receive MAIL from PM!\n");
            mbxMsg.eRoleID = _mgrMBX.eMbxCpu2Role[E_MBX_CPU_PM];
            break;
        case E_INT_IRQ_FRC_INT_FIQ2HST0://frcr2_integration###
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "Receive MAIL from R2FRC!\n");
            mbxMsg.eRoleID = _mgrMBX.eMbxCpu2Role[E_MBX_CPU_R2FRC];
            break;
        default:
            MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "[_MDrv_MBX_MsgRecvCb]Wrong interrupt number called! eIntNum=%d \n", eIntNum);
            _mgrMBX.bInInterruptHandler = FALSE;
            return;
    }
  #endif

    mbxResult = MHAL_MBX_Recv(&mbxMsg, _mgrMBX.eMbxHostRole);
    if(E_MBX_SUCCESS != mbxResult) //receive message failed, just skip it.
    {
        MBX_DBG_PRINT((MBX_DBG_LEVEL_MBXINT | MBX_DBG_LEVEL_MBXHAL), "[_MDrv_MBX_MsgRecvCb]Recv Mail from HW Failed! Result=%d Mail Src Role = %d\n", mbxResult, mbxMsg.eRoleID);
        _mgrMBX.bInInterruptHandler = FALSE;
        return;
    }

    if(_mgrMBX.bMsgRecvEnable == FALSE)
    {//do not enable msg receive!
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MBXINT, "[_MDrv_MBX_MsgRecvCb]The MBX Disabled! \n");
        MHAL_MBX_RecvEnd(mbxMsg.eRoleID, _mgrMBX.eMbxHostRole, E_MBXHAL_RECV_DISABLED);
        _mgrMBX.bInInterruptHandler = FALSE;
        return;
    }

    //mbxResult = MDrv_MSGQ_AddMSG(&mbxMsg); //Add to msg queue, maybe failed when overflow.
    bAddToQueue = TRUE;
    if(_mgrMBX.mbxQStatusNotifier[mbxMsg.u8MsgClass] != NULL)
    {
        (_mgrMBX.mbxQStatusNotifier[mbxMsg.u8MsgClass])(&mbxMsg, &bAddToQueue);
    }
    if(bAddToQueue)
    {
        mbxResult = MDrv_MSGQ_AddMSG(&mbxMsg); //Add to msg queue, maybe failed when overflow.
    }
    else
    {
       mbxResult = E_MBX_SUCCESS;
    }

    switch(mbxResult)
    {
        case E_MSGPOOL_ERR_NOT_INITIALIZED:
            MBX_DBG_PRINT((MBX_DBG_LEVEL_MBXINT|MBX_DBG_LEVEL_MSGQ), "[_MDrv_MBX_MsgRecvCb]Add Mail To Queue Failed! Result=%d \n", mbxResult);
            MHAL_MBX_RecvEnd(mbxMsg.eRoleID, _mgrMBX.eMbxHostRole, E_MBXHAL_RECV_DISABLED);
            break;
        case E_MSGPOOL_ERR_NO_MORE_MEMORY:
            MBX_DBG_PRINT((MBX_DBG_LEVEL_MBXINT|MBX_DBG_LEVEL_MSGQ), "[_MDrv_MBX_MsgRecvCb]Add Mail To Queue Failed! Result=%d \n", mbxResult);
            MHAL_MBX_RecvEnd(mbxMsg.eRoleID, _mgrMBX.eMbxHostRole, E_MBXHAL_RECV_OVERFLOW);
            break;
        default:
            MHAL_MBX_RecvEnd(mbxMsg.eRoleID, _mgrMBX.eMbxHostRole, E_MBXHAL_RECV_SUCCESS);
            break;
    }

    _mgrMBX.bInInterruptHandler = FALSE;
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
    MS_VIRT ptrRIUBaseAddr; 
	MS_PHY u32RIUBaseSize;
    MBX_Result mbxResult;

    if(_mgrMBX.s32RefCnt == 0)
    {
        if( !MDrv_MMIO_GetBASE(&ptrRIUBaseAddr, &u32RIUBaseSize, MS_MODULE_MBX))
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "MMIO Get Base Failed! Mailbox driver init failed!\n");
            return E_MBX_UNKNOW_ERROR;
        }

        if(E_MBX_SUCCESS != _MDrv_MBX_InitConfig(eHKCPU, eHostRole, u32TimeoutMillSecs))
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "MHAL_MBXINT_Init Failed! E_MBX_ERR_INVALID_PARAM!\n");
            return E_MBX_ERR_INVALID_PARAM;
        }

        DRV_MBX_InitLock();

        //init mbx hardware:
        MHAL_MBX_Init(eHostRole, ptrRIUBaseAddr);

        //init mbx int hardware:
        mbxResult = MHAL_MBXINT_Init(_mgrMBX.eMbxRole2Cpu[eHostRole], _MDrv_MBX_MsgRecvCb, ptrRIUBaseAddr);
        if(E_MBX_SUCCESS != mbxResult)
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "MHAL_MBXINT_Init Failed! Failed Result=%d!\n", mbxResult);
            return mbxResult;
        }

        //init msg pool:
        mbxResult = (MBX_Result)MDrv_MSGPOOL_Init();
        if(E_MBX_SUCCESS != mbxResult)
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "MDrv_MSGPOOL_Init Failed! Result: %d!\n", mbxResult);
            return mbxResult;
        }

        //init msg queue:
        MDrv_MSGQ_Init();
        _mgrMBX.s32RefCnt++;
    }
    else
    {
        mbxResult = _MDrv_MBX_SetConfig(eHKCPU, eHostRole, u32TimeoutMillSecs);
        if(E_MBX_SUCCESS != mbxResult)
        {
            MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "MDrv_MBX_SetConfig when Init MBX Failed! Result: %d!\n", mbxResult);
            return mbxResult;
        }

        _mgrMBX.s32RefCnt++;
    }

    return E_MBX_SUCCESS;
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
    if(_mgrMBX.s32RefCnt <=0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_DeInit]Reference Cnt Already <= 0 \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    if((--_mgrMBX.s32RefCnt) == 0)
    {
        //De-Init MSGQ:
        if(MDrv_MSGQ_DeInit(bForceDiscardPendingMsg)==E_MBX_ERR_HAS_MSG_PENDING)
        {//still has pending msg:
            _mgrMBX.s32RefCnt++;
            MBX_DBG_PRINT((MBX_DBG_LEVEL_CRITICAL|MBX_DBG_LEVEL_MSGQ), "[MDrv_MBX_DeInit]MSGQ DeInit Result: E_MBX_ERR_HAS_MSG_PENDING \n");
            return E_MBX_ERR_HAS_MSG_PENDING;
        }

        _mgrMBX.bMsgRecvEnable = FALSE;

        //De-Init MSGPool:
        MDrv_MSGPOOL_DeInit();

        //De-Init MBX Interrupt:
        MHAL_MBXINT_DeInit(_mgrMBX.eMbxRole2Cpu[_mgrMBX.eMbxHostRole]);

        DRV_MBX_DeInitLock();

        return E_MBX_SUCCESS;
    }

    return E_MBX_SUCCESS;
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
    MBX_Result mbxResult = E_MBX_SUCCESS;

    if(_mgrMBX.s32RefCnt <=0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RegisterMSG]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if(INVALID_MBXCLASS(eMsgClass) || (u16MsgQueueSize>MAX_MBX_QUEUE_SIZE))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RegisterMSG]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    mbxResult = (MBX_Result)MDrv_MSGQ_RegisterMSG((MS_S16)eMsgClass, u16MsgQueueSize);
    DRV_MBX_UnLockRecv();

    if(E_MBX_SUCCESS != mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_RegisterMSG]MDrv_MSGQ_RegisterMSG Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
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


    mbxResult = MDrv_MBX_RegisterMSG(eMsgClass, u16MsgQueueSize);
    if(mbxResult == E_MBX_SUCCESS)
    {
        _mgrMBX.mbxQStatusNotifier[eMsgClass] = notifier;
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
    MBX_Result mbxResult = E_MBX_SUCCESS;

    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_UnRegisterMSG]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if(INVALID_MBXCLASS(eMsgClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_UnRegisterMSG]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    mbxResult = (MBX_Result)MDrv_MSGQ_UnRegisterMSG((MS_S16)eMsgClass, bForceDiscardMsgQueue);
    DRV_MBX_UnLockRecv();

    if(E_MBX_SUCCESS != mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_UnRegisterMSG]MDrv_MSGQ_UnRegisterMSG Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
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
    MBX_Result mbxResult = E_MBX_SUCCESS;

    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_ClearMSG]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if(INVALID_MBXCLASS(eMsgClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_ClearMSG]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    mbxResult = (MBX_Result)MDrv_MSGQ_ClearMSG((MS_S16)eMsgClass);
    DRV_MBX_UnLockRecv();

    if(E_MBX_SUCCESS != mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_ClearMSG]MDrv_MSGQ_UnRegisterMSG Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
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
    MBX_Result mbxResult = E_MBX_SUCCESS;

    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetMsgQueueStatus]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if(INVALID_MBXCLASS(eTargetClass) || (pMsgQueueStatus==NULL))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetMsgQueueStatus]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    DRV_MBX_LockRecv();
    mbxResult = (MBX_Result)MDrv_MSGQ_GetMsgQStatus((MS_S16)eTargetClass, pMsgQueueStatus);
    DRV_MBX_UnLockRecv();

    if(E_MBX_SUCCESS != mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_MSGQ, "[MDrv_MBX_GetMsgQueueStatus]MDrv_MSGQ_GetMsgQStatus Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Send message
/// @param  pMsg \b IN: Mail Message Class for sending, must not be NULL, @ref MBX_Msg
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_ERR_NOT_INITIALIZED
/// @return E_MBX_ERR_SLOT_NOT_OPENNED
/// @return E_MBX_ERR_INVALID_CPU_ID
/// @return E_MBX_ERR_PEER_CPU_BUSY
/// @return E_MBX_ERR_PEER_CPU_NOT_ALIVE
/// @return E_MBX_ERR_PEER_CPU_OVERFLOW
/// @return E_MBX_ERR_TIME_OUT
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_SendMsg(MBX_Msg *pMsg)
{
    MBX_Result mbxResult = E_MBX_SUCCESS;

    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if((NULL==pMsg) || INVALID_MBXCLASS(pMsg->u8MsgClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    mbxResult = _MDrv_MBX_SendMsg(pMsg, _mgrMBX.eMbxHostRole);

    if(E_MBX_SUCCESS != mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsg]Send Mail Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
}

//-------------------------------------------------------------------------------------------------
/// Send Loopback Msg, for Testing
/// @param  pMsg \b IN: Mail Message Class for sending, must not be NULL, @ref MBX_Msg
/// @param  eHostCPUID \v IN: The src cpu which send the Mail
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
MBX_Result  MDrv_MBX_SendMsgLoopback(MBX_Msg *pMsg, MBX_ROLE_ID eSrcRoleId)
{
    MBX_Result mbxResult = E_MBX_SUCCESS;

    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsgLoopback]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if((NULL==pMsg) || INVALID_MBXCLASS(pMsg->u8MsgClass))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsgLoopback]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "Send mail from %d to %d! \n", pMsg->eRoleID, eSrcRoleId);

    mbxResult = _MDrv_MBX_SendMsg(pMsg, eSrcRoleId);

    if(E_MBX_SUCCESS != mbxResult)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SendMsgLoopback]Send Mail Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
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
    MBX_Result mbxResult = E_MBX_SUCCESS;
    MS_U32 u32Idx;

    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RecvMsg]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    //parameter check:
    if(INVALID_MBXCLASS(eTargetClass) || (pMsg==NULL))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RecvMsg]E_MBX_ERR_INVALID_PARAM! \n");
        return E_MBX_ERR_INVALID_PARAM;
    }

    if(!_bCalledFromDriver)
    {
        if((u32Flag!=MBX_CHECK_INSTANT_MSG)&&(u32Flag!=MBX_CHECK_NORMAL_MSG))
        {
            return E_MBX_ERR_INVALID_PARAM;
        }
        _bCalledFromDriver=FALSE;
    }

    mbxResult = _MDrv_MBX_RecvMsg(eTargetClass, pMsg, u32Flag);

    if((mbxResult == E_MBX_ERR_NO_MORE_MSG) && (u32Flag&MBX_CHECK_BLOCK_RECV) && (!_mgrMBX.bInInterruptHandler))
    {
        for(u32Idx=0; u32Idx<u32WaitMillSecs; u32Idx++)
        {
            mbxResult = _MDrv_MBX_RecvMsg(eTargetClass, pMsg, u32Flag);

            if(mbxResult != E_MBX_ERR_NO_MORE_MSG)
            {
                break;
            }
            MsOS_DelayTask(1);
        }

        if(mbxResult == E_MBX_ERR_NO_MORE_MSG)
        {
            mbxResult = E_MBX_ERR_TIME_OUT;
        }
    }

    if((E_MBX_SUCCESS != mbxResult) && (E_MBX_ERR_NO_MORE_MSG != mbxResult))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_RecvMsg]Recv Mail Failed! Result=%d \n", mbxResult);
    }

    return mbxResult;
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
    if(_mgrMBX.s32RefCnt <= 0)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Enable]E_MBX_ERR_NOT_INITIALIZED! \n");
        return E_MBX_ERR_NOT_INITIALIZED;
    }

    if(_mgrMBX.s32RefCnt > 1)
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_Enable]MBX Other Host used won't take effect! \n");
        return E_MBX_SUCCESS;
    }

    _mgrMBX.bMsgRecvEnable = bEnable;

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Set Information to dedicated Mailbox HW Regs.
/// @param  eTargetRole \b IN: The Target Role ID for which MBX HW Group will be used for Set
/// @param  pU8Info \b IN: The Information which need to set
/// @param  u8Size \b IN: The Size(bytes) of pU8Info
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em>It is used for speciall need when Init-Coprocesser.
///                 Could be called before MDrv_MBX_Init
///                 Could only be called from HK</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_SetInformation(MBX_ROLE_ID eTargetRole, MS_U8 *pU8Info, MS_U8 u8Size)
{
    MS_VIRT ptrRIUBaseAddr; 
	MS_PHY u32RIUBaseSize;

    //Check if Valid Parameter:
    if((eTargetRole != E_MBX_ROLE_CP) && (eTargetRole != E_MBX_ROLE_PM))
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

    if( !MDrv_MMIO_GetBASE(&ptrRIUBaseAddr, &u32RIUBaseSize, MS_MODULE_MBX))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_SetInformation] MMIO Get Base Failed! \n");
        return E_MBX_UNKNOW_ERROR;
    }

    return MHAL_MBX_SetInformation(eTargetRole, ptrRIUBaseAddr, pU8Info, u8Size);
}

//-------------------------------------------------------------------------------------------------
/// Get Information from dedicated Mailbox HW Regs.
/// @param  eTargetRole \b IN: The Target Role ID for which MBX HW Group will be used for Get
/// @param  pU8Info \b OUT: The Information Where we get to put
/// @param  u8Size \b IN: The Size(bytes) need to get
/// @return E_MBX_SUCCESS
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em>It is used for speciall need when Init-Coprocesser.
///                 Could be called before MDrv_MBX_Init
///                 Could only be called from CP, PM</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MDrv_MBX_GetInformation(MBX_ROLE_ID eTargetRole, MS_U8 *pU8Info, MS_U8 u8Size)
{
    MS_VIRT ptrRIUBaseAddr;
	MS_PHY u32RIUBaseSize;

    //Check if Valid Parameter:
    if((eTargetRole != E_MBX_ROLE_CP) && (eTargetRole != E_MBX_ROLE_PM))
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

    if( !MDrv_MMIO_GetBASE(&ptrRIUBaseAddr, &u32RIUBaseSize, MS_MODULE_MBX))
    {
        MBX_DBG_PRINT(MBX_DBG_LEVEL_CRITICAL, "[MDrv_MBX_GetInformation] MMIO Get Base Failed! \n");
        return E_MBX_UNKNOW_ERROR;
    }

    return MHAL_MBX_GetInformation(eTargetRole, ptrRIUBaseAddr, pU8Info, u8Size);
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
    _mgrMBX.u8DbgLevel = u8DbgLevel;
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
    if (NULL == pMbxDrvStatus)
    {
        return FALSE;
    }

    pMbxDrvStatus->bEnabled = _mgrMBX.bMsgRecvEnable;
    pMbxDrvStatus->s32RefCnt = _mgrMBX.s32RefCnt;
    return TRUE;
}


MBX_Result  MDrv_MBX_RemoveLatestMsg(void)
{
	return E_MBX_SUCCESS;
}

MBX_Result  MDrv_MBX_CheckMsg(MBX_Class eTargetClass, MBX_Msg *pMsg, MS_U32 u32WaitMillSecs, MS_U32 u32Flag)
{
	return E_MBX_SUCCESS;
}

void MDrv_MBX_SetCallDrvFlag(MS_BOOL bEnable)
{
    if(bEnable)
    {
        _bCalledFromDriver = TRUE;
    }
    else
    {
        _bCalledFromDriver = FALSE;
    }
}

MS_BOOL MDrv_MBX_GetCallDrvFlag(void)
{
    return _bCalledFromDriver;
}

