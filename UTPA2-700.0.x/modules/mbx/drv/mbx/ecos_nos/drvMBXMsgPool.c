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
/// file    drvMBXMsgPool.c
/// @brief  MStar MailBox Msg Pool DDI
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _DRV_MBX__MSGPOOL_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "drvMBX.h"
#include "drvMBXMsgPool.h"
#include "ULog.h"

//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================
#define MBX_MSGPOOL_PAGES  (14)

#ifndef PAGE_SHIFT
    #define PAGE_SHIFT (12)
#endif

#define TAG_MBX "MBX"

//=============================================================================
// Debug Macros
//=============================================================================
//#define MSGPOOL_DEBUG
#ifdef MSGPOOL_DEBUG
    #define MSGPOOL_ERROR(fmt, args...)           ULOGE(TAG_MBX, "[MBX MSGPool Driver USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MSGPOOL_WARN(fmt, args...)            ULOGW(TAG_MBX, "[MBX MSGPool Driver WARN][%06d]    " fmt, __LINE__, ## args)
    #define MSGPOOL_PRINT(fmt, args...)      ULOGD(TAG_MBX, "[MBX MSGPool (Driver)][%05d] " fmt, __LINE__, ## args)
    #define MSGPOOL_ASSERT(_cnd, _fmt, _args...)                   \
                                    if (!(_cnd)) {              \
                                        MSGPOOL_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define MSGPOOL_ERROR(fmt, args...)
    #define MSGPOOL_WARN(fmt, args...)
    #define MSGPOOL_PRINT(fmt, args...)
    #define MSGPOOL_ASSERT(_cnd, _fmt, _args...)
#endif

//----------------------------------------------------------------------------
// Macros
//----------------------------------------------------------------------------
#define MSGPOOL_MSGQ_MAX  E_MBX_CLASS_MAX

//----------------------------------------------------------------------------
// Local Variables
//----------------------------------------------------------------------------
//the msg pool for Chakra2-Aeon:
static MSGPOOL_MsgPoolItem _msgPoolMSGP[(MBX_MSGPOOL_PAGES*(0x01<<PAGE_SHIFT))/sizeof(MSGPOOL_MsgPoolItem)];
static MSGPOOL_MsgPoolInfo _infoMSGP;
static MSGPOOL_MsgQMgr _msgQMgrMSGQ[MSGPOOL_MSGQ_MAX];

//----------------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------------

//=============================================================================
// Local Function Prototypes
//=============================================================================

//=============================================================================
// Mailbox Driver MSG POOL Function
static void _MDrv_MSGPOOL_FreeMSGPoolItemQ(MS_S16 s16MsgFirst);
static void _MDrv_MSGPOOL_SetMsgToPoolItem(MS_S16 s16SlotIdx, MBX_Msg* pMbxMsg);
static void _MDrv_MSGPOOL_GetMsgFromPoolItem(MS_S16 s16SlotIdx, MBX_Msg* pMbxMsg);
static MSGPOOL_Result _MDrv_MSGPOOL_AllocateSlot(MS_S16* s16SlotIdx);
static MSGPOOL_Result _MDrv_MSGPOOL_FreeSlot(MS_S16 s16SlotIdx);
static MS_S16 _MDrv_MSGPOOL_GetNextSlot(MS_S16 s16SlotIdx);
static MSGPOOL_Result _MDrv_MSGPOOL_RegisterSlots(MS_U16 u16RegisteSlotNum);
static MSGPOOL_Result _MDrv_MSGPOOL_UnRegisterSlots(MS_U16 u16RegisteSlotNum);

//=============================================================================
// Mailbox Driver MSG Q Function
static void _MDrv_MSGQ_FreeMSG(MS_S16 s16MsgQIdx);

//=============================================================================
// Mailbox Driver MSG POOL Function

//-------------------------------------------------------------------------------------------------
/// Free mail message items in message queue
/// @param  s16MsgFirst                  \b IN: The first message item slot idx for msg q which need free
/// @return void
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MSGPOOL_FreeMSGPoolItemQ(MS_S16 s16MsgFirst)
{
    MS_S16 s16MsgNext = s16MsgFirst;
    while(IS_VALID_PTR(s16MsgNext))
    {
        s16MsgFirst = s16MsgNext;
	 s16MsgNext = _MDrv_MSGPOOL_GetNextSlot(s16MsgFirst);

	 _MDrv_MSGPOOL_FreeSlot(s16MsgFirst);
    }
}

//-------------------------------------------------------------------------------------------------
/// set mail message to mail message item
/// @param  s16SlotIdx                  \b IN: The  message item slot idx for setting msg
/// @param  pMbxMsg                  \b IN: The  message to set
/// @return void
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MSGPOOL_SetMsgToPoolItem(MS_S16 s16SlotIdx, MBX_Msg* pMbxMsg)
{
    _infoMSGP.pMsgPool[s16SlotIdx].mbxMsg = *pMbxMsg;
}

//-------------------------------------------------------------------------------------------------
/// get mail message from mail message item
/// @param  s16SlotIdx                  \b IN: The  message item slot idx for getting msg
/// @param  pMbxMsg                  \b OUT: the output msg
/// @return void
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MSGPOOL_GetMsgFromPoolItem(MS_S16 s16SlotIdx, MBX_Msg* pMbxMsg)
{
    *pMbxMsg = _infoMSGP.pMsgPool[s16SlotIdx].mbxMsg;
}

//-------------------------------------------------------------------------------------------------
/// allocate a free slot for message
/// @param  s16SlotIdx                  \b OUT: The Allocated slot idx
/// @return E_MSGPOOL_ERR_NO_MORE_MEMORY: no more free slots
/// @return E_MSGPOOL_SUCCESS:success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result _MDrv_MSGPOOL_AllocateSlot(MS_S16* s16SlotIdx)
{
    MS_S16 s16Slot;
    volatile MSGPOOL_MsgPoolItem * pMsgPoolItem = _infoMSGP.pMsgPool;

    if(_infoMSGP.u16FreeSlots<=0)
    {
        return E_MSGPOOL_ERR_NO_MORE_MEMORY;
    }

    for(s16Slot=0; s16Slot<_infoMSGP.u16Slots; s16Slot++)
    {
        if(pMsgPoolItem[s16Slot].u16Usage == FALSE )
        {
            pMsgPoolItem[s16Slot].u16Usage = TRUE;
            pMsgPoolItem[s16Slot].s16Next = INVALID_PTR;
	     _infoMSGP.u16FreeSlots--;
            *s16SlotIdx = s16Slot;
            break;
        }

    }

    return  E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// free a slot for message
/// @param  s16SlotIdx                  \b IN: The free slot idx
/// @return E_MSGPOOL_SUCCESS:success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result _MDrv_MSGPOOL_FreeSlot(MS_S16 s16SlotIdx)
{
    volatile MSGPOOL_MsgPoolItem * pMsgPoolItem = _infoMSGP.pMsgPool;

    MSGPOOL_ASSERT((IS_VALID_PTR(s16SlotIdx)), "The slot idx for free is invliad!\n");
    MSGPOOL_ASSERT((pMsgPoolItem[s16SlotIdx].u16Usage==TRUE), "The slot for free is not used!\n");

    pMsgPoolItem[s16SlotIdx].u16Usage = FALSE;
    pMsgPoolItem[s16SlotIdx].s16Next = INVALID_PTR;
    _infoMSGP.u16FreeSlots++;

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Get Next Slot Idx from input slot idx;
/// @param  s16SlotIdx                  \b IN: The slot idx for get next
/// @return MS_S16:the next slot idx of input idx;
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MS_S16 _MDrv_MSGPOOL_GetNextSlot(MS_S16 s16SlotIdx)
{
    MSGPOOL_ASSERT((IS_VALID_PTR(s16SlotIdx)), "The slot idx is invliad! %x\n", s16SlotIdx);

    return _infoMSGP.pMsgPool[s16SlotIdx].s16Next;
}

//-------------------------------------------------------------------------------------------------
/// set Next Slot Idx to input slot idx;
/// @param  s16PreSlotIdx                  \b IN: The slot idx for set next
/// @param  s16NextSlotIdx                  \b IN: The next slot idx for set
/// @return void
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MSGPOOL_SetNextSlot(MS_S16 s16PreSlotIdx, MS_S16 s16NextSlotIdx)
{
    _infoMSGP.pMsgPool[s16PreSlotIdx].s16Next = s16NextSlotIdx;
}

//-------------------------------------------------------------------------------------------------
/// register a number of slots from msg pool
/// @param  u16RegisteSlotNum                  \b IN: The register slot number
/// @return E_MSGPOOL_ERR_NO_MORE_MEMORY: no more free slots for register
/// @return E_MSGPOOL_SUCCESS:success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result _MDrv_MSGPOOL_RegisterSlots(MS_U16 u16RegisteSlotNum)
{
    if((_infoMSGP.u16RegistedSlots+u16RegisteSlotNum) > _infoMSGP.u16Slots) //no enough slots for registe
    {
        return E_MSGPOOL_ERR_NO_MORE_MEMORY;
    }

    //have memory for register:
    _infoMSGP.u16RegistedSlots += u16RegisteSlotNum;

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// un-register a number of slots from msg pool
/// @param  u16UnRegisteSlotNum                  \b IN: The un register slot number
/// @return E_MSGPOOL_ERR_NO_MORE_MEMORY: no more free slots for register
/// @return E_MSGPOOL_SUCCESS:success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result _MDrv_MSGPOOL_UnRegisterSlots(MS_U16 u16UnRegisteSlotNum)
{
    if(_infoMSGP.u16RegistedSlots < u16UnRegisteSlotNum)
    {
        return E_MSGPOOL_ERR_INVALID_PARAM;
    }

    _infoMSGP.u16RegistedSlots -= u16UnRegisteSlotNum;

    return E_MSGPOOL_SUCCESS;
}

//=============================================================================
// Mailbox Driver MSG Q Function

//-------------------------------------------------------------------------------------------------
/// Free mail message items in message queue
/// @param  s16MsgQIdx                  \b IN: The first message item slot idx for msg q which need free
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MSGQ_FreeMSG(MS_S16 s16MsgQIdx)
{
    _MDrv_MSGPOOL_FreeMSGPoolItemQ(_msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst);
    _MDrv_MSGPOOL_FreeMSGPoolItemQ(_msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst);
    _MDrv_MSGPOOL_UnRegisterSlots(_msgQMgrMSGQ[s16MsgQIdx].u16MsgQSize);

    _msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst = _msgQMgrMSGQ[s16MsgQIdx].s16MsgEnd
		= _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst = _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgEnd = INVALID_PTR;

    _msgQMgrMSGQ[s16MsgQIdx].u16MsgNum = _msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum = 0;

    _msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus = E_MSGQ_INVALID;
    _msgQMgrMSGQ[s16MsgQIdx].u16MsgQSize = 0;
}

//-------------------------------------------------------------------------------------------------
/// Clear mail message items in message queue
/// @param  s16MsgQIdx                  \b IN: The first message item slot idx for msg q which need free
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void _MDrv_MSGQ_ClearMSG(MS_S16 s16MsgQIdx)
{
    _MDrv_MSGPOOL_FreeMSGPoolItemQ(_msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst);
    _MDrv_MSGPOOL_FreeMSGPoolItemQ(_msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst);

    _msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst = _msgQMgrMSGQ[s16MsgQIdx].s16MsgEnd
		= _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst = _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgEnd = INVALID_PTR;

    _msgQMgrMSGQ[s16MsgQIdx].u16MsgNum = _msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum = 0;

    _msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus = E_MSGQ_EMPTY;
}
//----------------------------------------------------------------------------
//=============================================================================
// Mailbox Driver MSG Pool Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Init the message Pool
/// @return E_MSGPOOL_ERR_NO_MORE_MEMORY:allocate pool failed
/// @return E_MSGPOOL_SUCCESS:success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGPOOL_Init()
{
    MS_U16 u16Idx;
    _infoMSGP.pMsgPool = _msgPoolMSGP;

    _infoMSGP.u16Slots = (MBX_MSGPOOL_PAGES*(0x01<<PAGE_SHIFT))/sizeof(MSGPOOL_MsgPoolItem);
    _infoMSGP.u16FreeSlots = _infoMSGP.u16Slots;
    _infoMSGP.u16RegistedSlots = 0;

    for(u16Idx=0; u16Idx<_infoMSGP.u16Slots; u16Idx++)
    {
        _infoMSGP.pMsgPool[u16Idx].u16Usage = FALSE;
        _infoMSGP.pMsgPool[u16Idx].s16Next = INVALID_PTR;
    }

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// DeInit the message Pool
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
void MDrv_MSGPOOL_DeInit()
{
    MS_U16 u16Idx;

    for(u16Idx=0; u16Idx<_infoMSGP.u16Slots; u16Idx++)
    {
        _infoMSGP.pMsgPool[u16Idx].u16Usage = FALSE;
        _infoMSGP.pMsgPool[u16Idx].s16Next = INVALID_PTR;
    }
}

//----------------------------------------------------------------------------
//=============================================================================
// Mailbox Driver MSG Q Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Init the message Q
/// @return E_MSGPOOL_SUCCESS:success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_Init()
{
    MS_U16 u16MsgQIdx;

    for(u16MsgQIdx = 0; u16MsgQIdx < MSGPOOL_MSGQ_MAX; u16MsgQIdx++)
    {
        _msgQMgrMSGQ[u16MsgQIdx].s16MsgFirst = _msgQMgrMSGQ[u16MsgQIdx].s16MsgEnd
		    = _msgQMgrMSGQ[u16MsgQIdx].s16InstantMsgFirst = _msgQMgrMSGQ[u16MsgQIdx].s16InstantMsgEnd = INVALID_PTR;

        _msgQMgrMSGQ[u16MsgQIdx].u16MsgNum = _msgQMgrMSGQ[u16MsgQIdx].u16InstantMsgNum = 0;

        _msgQMgrMSGQ[u16MsgQIdx].u16MsgQStatus = E_MSGQ_INVALID;
        _msgQMgrMSGQ[u16MsgQIdx].u16MsgQSize = 0;
    }

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// De-Init MSGQ
/// @param  bForceDiscardPendingMsg                  \b IN: If TRUE, De-Init whatever the status of msg queue
/// @return E_MSGPOOL_ERR_HAS_MSG_PENDING: has pendign msg in msg queue for un-register
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_DeInit(MS_BOOL bForceDiscardPendingMsg)
{
    MS_U16 u16MsgQIdx;

    //check if has pending msg:
    if(!bForceDiscardPendingMsg)
    {
        for(u16MsgQIdx = 0; u16MsgQIdx < MSGPOOL_MSGQ_MAX; u16MsgQIdx++)
        {
            if((_msgQMgrMSGQ[u16MsgQIdx].u16MsgNum > 0) || (_msgQMgrMSGQ[u16MsgQIdx].u16InstantMsgNum > 0))
            {
                return E_MSGPOOL_ERR_HAS_MSG_PENDING;
            }
        }
    }

    for(u16MsgQIdx = 0; u16MsgQIdx < MSGPOOL_MSGQ_MAX; u16MsgQIdx++)
    {
        _MDrv_MSGQ_FreeMSG(u16MsgQIdx);
    }

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Register message class
/// @param  s16MsgQID                  \b IN: The Register Msg Class ID
/// @param  u16MsgQSize                  \b IN: The size of Msg Queue with register class
/// @return E_MSGPOOL_ERR_SLOT_AREADY_OPENNED:the class already registered by this Async. Notifier
/// @return E_MSGPOOL_ERR_NO_MORE_MEMORY:no request msg q size avaiable
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_RegisterMSG(MS_S16 s16MsgQID, MS_U16 u16MsgQSize)
{
    if(_msgQMgrMSGQ[s16MsgQID].u16MsgQStatus != E_MSGQ_INVALID) //already registed by this app
    {
        return E_MSGPOOL_ERR_SLOT_AREADY_OPENNED;
    }

    if(E_MSGPOOL_ERR_NO_MORE_MEMORY == _MDrv_MSGPOOL_RegisterSlots(u16MsgQSize))
    {
        return E_MSGPOOL_ERR_NO_MORE_MEMORY;
    }

    //success registed, then allocate the msg q:
    _msgQMgrMSGQ[s16MsgQID].s16MsgFirst = _msgQMgrMSGQ[s16MsgQID].s16MsgEnd
		= _msgQMgrMSGQ[s16MsgQID].s16InstantMsgFirst = _msgQMgrMSGQ[s16MsgQID].s16InstantMsgEnd = INVALID_PTR;

    _msgQMgrMSGQ[s16MsgQID].u16MsgNum = _msgQMgrMSGQ[s16MsgQID].u16InstantMsgNum = 0;

    _msgQMgrMSGQ[s16MsgQID].u16MsgQStatus = E_MSGQ_EMPTY;
    _msgQMgrMSGQ[s16MsgQID].u16MsgQSize = u16MsgQSize;

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Register message class
/// @param  s16MsgQID                  \b IN: The Register Msg Class ID
/// @param  bForceDiscardMsgQueue                  \b IN: If TRUE, un-register whatever the status of msg queue
/// @return E_MSGPOOL_ERR_SLOT_NOT_OPENNED:the class have not been registered yet
/// @return E_MSGPOOL_ERR_HAS_MSG_PENDING: has pendign msg in msg queue for un-register
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_UnRegisterMSG(MS_S16 s16MsgQID, MS_BOOL bForceDiscardMsgQueue)
{
    //if not opened yet?
    if(E_MSGQ_INVALID == _msgQMgrMSGQ[s16MsgQID].u16MsgQStatus)
    {
        return E_MSGPOOL_ERR_SLOT_NOT_OPENNED;
    }

    //if has pending msg and app not force discard:
    if((!bForceDiscardMsgQueue) && ((_msgQMgrMSGQ[s16MsgQID].u16InstantMsgNum+_msgQMgrMSGQ[s16MsgQID].u16MsgNum) >  0))
    {
        return E_MSGPOOL_ERR_HAS_MSG_PENDING;
    }

    _MDrv_MSGQ_FreeMSG(s16MsgQID);

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Clear message class
/// @param  s16MsgQID                  \b IN: The Clear Msg Class ID
/// @return E_MSGPOOL_ERR_SLOT_NOT_OPENNED:the class have not been registered yet
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_ClearMSG(MS_S16 s16MsgQID)
{
    //if not opened yet?
    if(E_MSGQ_INVALID == _msgQMgrMSGQ[s16MsgQID].u16MsgQStatus)
    {
        return E_MSGPOOL_ERR_SLOT_NOT_OPENNED;
    }

    _MDrv_MSGQ_ClearMSG(s16MsgQID);

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// get status of message class
/// @param  s16MsgQID                  \b IN: The Msg Class ID to get info.
/// @param  pMsgQStatus                  \b OUT: the msg class info to put
/// @return E_MSGPOOL_ERR_SLOT_NOT_OPENNED:the class have not been registered yet
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_GetMsgQStatus(MS_S16 s16MsgQID, MBX_MSGQ_Status *pMsgQStatus)
{
    //if not opened yet?
    if(E_MSGQ_INVALID == _msgQMgrMSGQ[s16MsgQID].u16MsgQStatus)
    {
        return E_MSGPOOL_ERR_SLOT_NOT_OPENNED;
    }

    pMsgQStatus->status = 0;

    if(_msgQMgrMSGQ[s16MsgQID].u16MsgQStatus == E_MSGQ_OVERFLOW)
    {
        pMsgQStatus->status |= MBX_STATUS_QUEUE_OVER_FLOW;
    }

    if(_msgQMgrMSGQ[s16MsgQID].u16MsgNum > 0)
    {
        pMsgQStatus->status |= MBX_STATUS_QUEUE_HAS_NORMAL_MSG;
    }

    if(_msgQMgrMSGQ[s16MsgQID].u16InstantMsgNum > 0)
    {
        pMsgQStatus->status |= MBX_STATUS_QUEUE_HAS_INSTANT_MSG;
    }

    pMsgQStatus->u32InstantMsgCount = _msgQMgrMSGQ[s16MsgQID].u16InstantMsgNum;
    pMsgQStatus->u32NormalMsgCount= _msgQMgrMSGQ[s16MsgQID].u16MsgNum;

    return E_MSGPOOL_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// recv message from msg class queue
/// @param  s16MsgQIdx                  \b IN: The  msg Q ID for recv
/// @param  pMsg                  \b OUT: the msg to put
/// @param  bInstantMsg                  \b IN: get the instant msg or normal msg
/// @return E_MSGPOOL_ERR_NO_MORE_MSG: no msg for recv
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em>Use spin lock to protect co-access</em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_RecvMsg(MS_S16 s16MsgQIdx, MBX_Msg* pMsg, MS_BOOL bInstantMsg)
{
    MS_S16 s16MsgSlotIdx;
    MSGPOOL_Result msgPoolResult;

    if(bInstantMsg)
    {//get instant msg:
        if(_msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum>0)
        {
            s16MsgSlotIdx = _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst;
            _MDrv_MSGPOOL_GetMsgFromPoolItem(s16MsgSlotIdx, pMsg);
            _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst = _MDrv_MSGPOOL_GetNextSlot(s16MsgSlotIdx);
            _MDrv_MSGPOOL_FreeSlot(s16MsgSlotIdx);
            _msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum--;
            msgPoolResult = E_MSGPOOL_SUCCESS;
        }
        else
        {
            msgPoolResult = E_MSGPOOL_ERR_NO_MORE_MSG;
        }
    }
    else
    {//get normal msg:
        if(_msgQMgrMSGQ[s16MsgQIdx].u16MsgNum>0)
        {
            s16MsgSlotIdx = _msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst;
            _MDrv_MSGPOOL_GetMsgFromPoolItem(s16MsgSlotIdx, pMsg);
            _msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst = _MDrv_MSGPOOL_GetNextSlot(s16MsgSlotIdx);
            _MDrv_MSGPOOL_FreeSlot(s16MsgSlotIdx);
            _msgQMgrMSGQ[s16MsgQIdx].u16MsgNum--;

            msgPoolResult = E_MSGPOOL_SUCCESS;
        }
        else
        {
            msgPoolResult = E_MSGPOOL_ERR_NO_MORE_MSG;
        }
    }

    if((_msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum+_msgQMgrMSGQ[s16MsgQIdx].u16MsgNum)<_msgQMgrMSGQ[s16MsgQIdx].u16MsgQSize)
    {
        _msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus = E_MSGQ_NORMAL;
    }

    if((_msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum+_msgQMgrMSGQ[s16MsgQIdx].u16MsgNum)<=0)
    {
        _msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus = E_MSGQ_EMPTY;
    }

    return msgPoolResult;
}

//-------------------------------------------------------------------------------------------------
/// add message to msg class queue
/// @param  pMsg                  \b IN: the msg to put
/// @return E_MSGPOOL_ERR_NOT_INITIALIZED: no msg not actived yet
/// @return E_MSGPOOL_ERR_NO_MORE_MSG: no msg memory to add
/// @return E_MSGPOOL_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em></em></b>
//-------------------------------------------------------------------------------------------------
MSGPOOL_Result MDrv_MSGQ_AddMSG(MBX_Msg* pMbxMsg)
{
    MS_S16 s16MsgItemIdx = INVALID_PTR;
    MS_S16 s16MsgQIdx = pMbxMsg->u8MsgClass;

    if((_msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus == E_MSGQ_INVALID))
    {
        return E_MSGPOOL_ERR_NOT_INITIALIZED;
    }

    if(_msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus == E_MSGQ_OVERFLOW)
    {
        return E_MSGPOOL_ERR_NO_MORE_MEMORY;
    }

    if(_MDrv_MSGPOOL_AllocateSlot(&s16MsgItemIdx) == E_MSGPOOL_SUCCESS)
    {
        _MDrv_MSGPOOL_SetMsgToPoolItem(s16MsgItemIdx, pMbxMsg);
        if(pMbxMsg->eMsgType == E_MBX_MSG_TYPE_INSTANT)
        {
            if(_msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum > 0)
            {
                _MDrv_MSGPOOL_SetNextSlot(_msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgEnd,s16MsgItemIdx);
                _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgEnd = s16MsgItemIdx;
            }
            else
            {
                _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgFirst = _msgQMgrMSGQ[s16MsgQIdx].s16InstantMsgEnd = s16MsgItemIdx;
            }

            _msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum++;
        }
        else
        {
            if(_msgQMgrMSGQ[s16MsgQIdx].u16MsgNum > 0)
            {
                _MDrv_MSGPOOL_SetNextSlot(_msgQMgrMSGQ[s16MsgQIdx].s16MsgEnd,s16MsgItemIdx);
                _msgQMgrMSGQ[s16MsgQIdx].s16MsgEnd = s16MsgItemIdx;
            }
            else
            {
                _msgQMgrMSGQ[s16MsgQIdx].s16MsgFirst = _msgQMgrMSGQ[s16MsgQIdx].s16MsgEnd = s16MsgItemIdx;
            }

            _msgQMgrMSGQ[s16MsgQIdx].u16MsgNum++;
        }

        if(_msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus == E_MSGQ_EMPTY)
        {
            _msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus = E_MSGQ_NORMAL;
        }

        if((_msgQMgrMSGQ[s16MsgQIdx].u16MsgNum+_msgQMgrMSGQ[s16MsgQIdx].u16InstantMsgNum)>=_msgQMgrMSGQ[s16MsgQIdx].u16MsgQSize)
        {
            _msgQMgrMSGQ[s16MsgQIdx].u16MsgQStatus = E_MSGQ_OVERFLOW;
        }

        return E_MSGPOOL_SUCCESS;
    }

    return E_MSGPOOL_ERR_NO_MORE_MEMORY;
}
