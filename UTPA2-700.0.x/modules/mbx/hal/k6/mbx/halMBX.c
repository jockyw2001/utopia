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
/// file    halMBX.c
/// @brief  MStar MailBox HAL level DDI
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _HAL_MBX_C

//=============================================================================
// Include Files
//=============================================================================
#include "MsCommon.h"
#include "drvMBX.h"
#include "regMBX.h"
#include "halMBX.h"
//=============================================================================
// Compile options
//=============================================================================


//=============================================================================
// Local Defines
//=============================================================================

//=============================================================================
// Debug Macros
//=============================================================================
//#define MBXHAL_DEBUG
#ifdef MBXHAL_DEBUG
    #define MBXHAL_ERROR(fmt, args...)           printf("[MBX HAL Driver USER ERR][%06d]     " fmt, __LINE__, ## args)
    #define MBXHAL_WARN(fmt, args...)            printf("[MBX HAL Driver WARN][%06d]    " fmt, __LINE__, ## args)
    #define MBXHAL_PRINT(fmt, args...)           printf("[MBX HAL Driver][%06d]     " fmt, __LINE__, ## args)
    #define MBXHAL_ASSERT(_cnd, _fmt, _args...)    \
                                    if (!(_cnd)) {              \
                                        MBXHAL_PRINT(_fmt, ##_args);  \
                                    }
#else
    #define MBXHAL_ERROR(fmt, args...)
    #define MBXHAL_WARN(fmt, args...)
    #define MBXHAL_PRINT(fmt, args...)
    #define MBXHAL_ASSERT(_cnd, _fmt, _args...)
#endif
//=============================================================================
// Macros
//=============================================================================

//=============================================================================
// Local Variables
//=============================================================================
static MS_U16 _u16MbxGroupIdMBXHAL[E_MBX_ROLE_MAX][E_MBX_ROLE_MAX] =
                                        { {0xFF, REG_MBX_GROUP0, REG_MBX_GROUP4}, {REG_MBX_GROUP1, 0xFF, REG_MBX_GROUP3}, {REG_MBX_GROUP3, 0xFF, 0xFF} };
static MS_VIRT _virtRIUBaseAddrMBX = 0;
//=============================================================================
// Global Variables
//=============================================================================

//=============================================================================
// Local Function Prototypes
//=============================================================================
static void _MHAL_MBX_FireMsg (MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole);
static void _MHAL_MBX_RecvMsg(MBX_Msg* pMbxMsg, MBX_ROLE_ID eDstRole);

//=============================================================================
// Local Function
//=============================================================================
//-------------------------------------------------------------------------------------------------
/// Fire Msg to MailBox hardware.
/// @param  eSrcCPUID                  \b IN: The Firer CPUID
/// @return void
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void _MHAL_MBX_FireMsg (MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole)
{
    MS_S32 s32Idx;

    /* fill mail box register. */
    //REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcCPUID], REG8_MBX_CTRL) = pMbxMsg->u8Ctrl;
    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID], REG8_MBX_MAIL_CLASS) = pMbxMsg->u8MsgClass;
    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID], REG8_MBX_MAIL_IDX) = pMbxMsg->u8Index;
    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID], REG8_MBX_PARAMETER_CNT) = pMbxMsg->u8ParameterCount;

    for(s32Idx=0;  s32Idx<pMbxMsg->u8ParameterCount; s32Idx++)
    {
        REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID], s32Idx+REG8_MBX_PARAMETER_S) = pMbxMsg->u8Parameters[s32Idx];
    }

    //REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcCPUID], REG8_MBX_STATE_0) = pMbxMsg->u8S0;
    //REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcCPUID], REG8_MBX_STATE_1) = pMbxMsg->u8S1;
}

//-------------------------------------------------------------------------------------------------
/// Recv Msg From MailBox hardware.
/// @param  pMbxMsg                  \b INOUT: The Recv CPUID, and where mail to put
/// @return void
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
void _MHAL_MBX_RecvMsg(MBX_Msg* pMbxMsg, MBX_ROLE_ID eDstRole)
{
    MS_S32 s32Idx;

    pMbxMsg->u8Ctrl = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], REG8_MBX_CTRL);
    pMbxMsg->u8MsgClass = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], REG8_MBX_MAIL_CLASS);
    pMbxMsg->u8Index = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], REG8_MBX_MAIL_IDX);
    pMbxMsg->u8ParameterCount = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], REG8_MBX_PARAMETER_CNT);

    for(s32Idx=0;  s32Idx<10; s32Idx++)
    {
        pMbxMsg->u8Parameters[s32Idx] = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], s32Idx+REG8_MBX_PARAMETER_S);
    }

    pMbxMsg->u8S0 = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], REG8_MBX_STATE_0);
    pMbxMsg->u8S1 = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole], REG8_MBX_STATE_1);
}

//=============================================================================
// Mailbox HAL Driver Function
//=============================================================================

//-------------------------------------------------------------------------------------------------
/// Init MailBox hardware.
/// @param  eHostRole                  \b IN: The host Role ID
/// @param  u32RIUBaseAddrMBX                  \b IN: The RIU Base Addr.
/// @return E_MBX_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBX_Init(MBX_ROLE_ID eHostRole, MS_VIRT virtRIUBaseAddrMBX)
{
    _virtRIUBaseAddrMBX = virtRIUBaseAddrMBX;

    return MHAL_MBX_SetConfig(eHostRole);
}

//-------------------------------------------------------------------------------------------------
/// Set MailBox Group Regs.
/// @param  eHostRole                  \b IN: The host Role ID
/// @return E_MBX_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em>Can't be called Before MHAL_MBX_Init </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBX_SetConfig(MBX_ROLE_ID eHostRole)
{
    MS_S32 s32MailIdx;

    /* clear host mail box register. */
    if(E_MBX_ROLE_HK == eHostRole) //It is HouseKeeping
    {
        for(s32MailIdx=0; s32MailIdx<16; s32MailIdx++)
        {
            REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[E_MBX_ROLE_HK][E_MBX_ROLE_CP], s32MailIdx) = 0x00;
            REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[E_MBX_ROLE_HK][E_MBX_ROLE_PM], s32MailIdx) = 0x00;
        }
    }
    else
    {
        for(s32MailIdx=0; s32MailIdx<16; s32MailIdx++)
        {
            REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eHostRole][E_MBX_ROLE_HK], s32MailIdx) = 0x00;
        }
    }

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Set Information to dedicated Mailbox HW Regs.
/// @param  eTargetRole \b IN: The correpsonded Role ID for which MBX HW Group will be used for Set
/// @param  pU8Info \b IN: The Information which need to set
/// @param  u8Size \b IN: The Size(bytes) of pU8Info
/// @return E_MBX_SUCCESS
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em>It is used for speciall need when Init-Coprocesser</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MHAL_MBX_SetInformation(MBX_ROLE_ID eTargetRole, MS_VIRT virtRIUBaseAddrMBX, MS_U8 *pU8Info, MS_U8 u8Size)
{
    MS_S32 s32Idx;

    _virtRIUBaseAddrMBX = virtRIUBaseAddrMBX;

    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 0+REG8_MBX_PARAMETER_S) = 0x00;
    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 1+REG8_MBX_PARAMETER_S) = 0x00;

    for(s32Idx=0; s32Idx<u8Size; s32Idx++)
    {
        REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], s32Idx+2+REG8_MBX_PARAMETER_S) = pU8Info[s32Idx];
    }

    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 0+REG8_MBX_PARAMETER_S) = 0x78;
    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 1+REG8_MBX_PARAMETER_S) = 0x56;

    while((REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 0+REG8_MBX_PARAMETER_S) == 0x78)
            && (REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 1+REG8_MBX_PARAMETER_S) == 0x56));

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Get Information from dedicated Mailbox HW Regs.
/// @param  eTargetRole \b IN: The correpsonded Role ID for which MBX HW Group will be used for Get
/// @param  pU8Info \b OUT: The Information Where we get to put
/// @param  u8Size \b IN: The Size(bytes) need to get
/// @return E_MBX_SUCCESS
/// @return E_MBX_UNKNOW_ERROR
/// @attention
/// <b>[MXLIB] <em>It is used for speciall need when Init-Coprocesser.</em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result  MHAL_MBX_GetInformation(MBX_ROLE_ID eTargetRole, MS_VIRT virtRIUBaseAddrMBX, MS_U8 *pU8Info, MS_U8 u8Size)
{
    MS_S32 s32Idx;

    _virtRIUBaseAddrMBX = virtRIUBaseAddrMBX;

    while((REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 0+REG8_MBX_PARAMETER_S) != 0x78)
            || (REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 1+REG8_MBX_PARAMETER_S) != 0x56));

    for(s32Idx=0; s32Idx<u8Size; s32Idx++)
    {
        pU8Info[s32Idx] = REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], s32Idx+2+REG8_MBX_PARAMETER_S);
    }

    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 0+REG8_MBX_PARAMETER_S) = 0x00;
    REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eTargetRole][E_MBX_ROLE_HK], 1+REG8_MBX_PARAMETER_S) = 0x00;

    return E_MBX_SUCCESS;
}

MBX_Result MHAL_MBX_ClearAll (MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole)
{
	MS_U8 index = 0;
	for (index = 0; index < 16; index++)
		REG8_MBX_GROUP(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID], index) = 0x00;

	return E_MBX_SUCCESS; 
		

}

//-------------------------------------------------------------------------------------------------
/// Fire Mail to MailBox hardware.
/// @param  eSrcCPUID                  \b IN: The Firer CPUID
/// @return E_MBX_ERR_PEER_CPU_NOTREADY: THE Peer CPU in fetching the msg
/// @return E_MBX_ERR_PEER_CPU_NOT_ALIVE: The Peer CPU Looks like not alive
/// @return E_MBX_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBX_Fire (MBX_Msg* pMbxMsg, MBX_ROLE_ID eSrcRole)
{
    MS_BOOL bIPState;

    /* check paramter. */
    if(0xFF == _u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID])
    {
        return E_MBX_ERR_INVALID_PARAM;
    }

    /* check send bit in CONTROL register. */
    bIPState = _FIRE(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID]);

    if(bIPState > 0)
    {
        bIPState = _BUSY(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID]);
        if(bIPState > 0)
        {//the mail is in process
            MBXHAL_ERROR("[Fire Mail Fail] : co-processor is not ready!\n");
            return  E_MBX_ERR_PEER_CPU_NOTREADY;
        }
        else
        {//mail not processed yet!
            MBXHAL_ERROR("[Fire Mail Fail] : co-processor is not alive!\n");
            return  E_MBX_ERR_PEER_CPU_NOT_ALIVE;
        }
    }

    /* clear status1 register. */
    _S1_C(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID]);
    // Clear Instant Setting:
    _INSTANT_C(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID]);

    /* fill mail box register. */
    _MHAL_MBX_FireMsg(pMbxMsg, eSrcRole);

    /* set instant message attribute */
    if(pMbxMsg->eMsgType == E_MBX_MSG_TYPE_INSTANT)
    {
        _INSTANT_S(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID]);
    }

    _FIRE_S(_u16MbxGroupIdMBXHAL[eSrcRole][pMbxMsg->eRoleID]);

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Get HW Fire Status
/// @param  eSrcRole                  \b IN: The Firer Src Role
/// @param  eDstRole                  \b IN: The Firer Dst Role
/// @param  pFireStatus               \b OUT: THE Fire Status
/// @return E_MBX_ERR_INVALID_PARAM
/// @return E_MBX_SUCCESS
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBX_GetFireStatus(MBX_ROLE_ID eSrcRole, MBX_ROLE_ID eDstRole, MBXHAL_Fire_Status *pFireStatus)
{
    /* check paramter. */
    if((0xFF == _u16MbxGroupIdMBXHAL[eSrcRole][eDstRole])
        || (NULL == pFireStatus))
    {
        return E_MBX_ERR_INVALID_PARAM;
    }

    //still on Firing:
    if(_FIRE(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]))
    {
        *pFireStatus = E_MBXHAL_FIRE_ONGOING;
    }
    else if(_OVERFLOW(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]))
    {    //If Overflow:
        *pFireStatus = E_MBXHAL_FIRE_OVERFLOW;
    }
    else if(_DISABLED(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]))
    {   //If Disabled:
        *pFireStatus = E_MBXHAL_FIRE_DISABLED;
    }
    else
    {
        *pFireStatus = E_MBXHAL_FIRE_SUCCESS;
    }

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// Recv Mail From MailBox hardware.
/// @param  pMbxMsg                  \b INOUT: The Recv Src RoleID, and where mail to put
/// @param  eDstRole                  \b IN: The Recv Dst RoleID
/// @return E_MBX_ERR_MSG_ALREADY_FETCHED: THE Msg has been fetched
/// @return E_MBX_ERR_INVALID_PARAM: THE Input Src Role & Dst Role is incorrect
/// @return E_MBX_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBX_Recv(MBX_Msg* pMbxMsg, MBX_ROLE_ID eDstRole)
{
    MS_BOOL bIPState;

    /* check paramter. */
    if(0xFF == _u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole])
    {
        return E_MBX_ERR_INVALID_PARAM;
    }

    /* check send bit in CONTROL register. */
    bIPState = _FIRE(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole]);
    if(bIPState <= 0) //the message already fetched!
        return  E_MBX_ERR_MSG_ALREADY_FETCHED;

    /* set busy bit */
    _BUSY_S(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole]);

    bIPState = _INSTANT(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole]) | _READBACK(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole]);
    if(bIPState)
        pMbxMsg->eMsgType= E_MBX_MSG_TYPE_INSTANT;
    else
        pMbxMsg->eMsgType= E_MBX_MSG_TYPE_NORMAL;

    /* recv mail msg */
    _MHAL_MBX_RecvMsg(pMbxMsg, eDstRole);

    /* clear busy bit */
    //_BUSY_C(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole]);
    /* clear send bit */
    //_FIRE_C(_u16MbxGroupIdMBXHAL[pMbxMsg->eRoleID][eDstRole]);

    return E_MBX_SUCCESS;
}

//-------------------------------------------------------------------------------------------------
/// End Recv Mail From MailBox hardware. Set Status for FeedBack
/// @param  eSrcRole                  \b IN: The Recv Src RoleID
/// @param  eDstRole                  \b IN: The Recv Dst RoleID
/// @param  eRecvSatus                  \b IN: The Recv Status
/// @return E_MBX_ERR_INVALID_PARAM: THE Input Src Role & Dst Role is incorrect
/// @return E_MBX_SUCCESS: success
/// @attention
/// <b>[MXLIB] <em> </em></b>
//-------------------------------------------------------------------------------------------------
MBX_Result MHAL_MBX_RecvEnd(MBX_ROLE_ID eSrcRole, MBX_ROLE_ID eDstRole, MBXHAL_Recv_Status eRecvSatus)
{
    /* check paramter. */
    if(0xFF == _u16MbxGroupIdMBXHAL[eSrcRole][eDstRole])
    {
        return E_MBX_ERR_INVALID_PARAM;
    }

    switch(eRecvSatus)
    {
        case E_MBXHAL_RECV_OVERFLOW:
            //Set OverFlow & Error:
            _OVERFLOW_S(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]);
            _ERR_S(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]);
            break;
        case E_MBXHAL_RECV_DISABLED:
            //Set Disable & Error:
            _DISABLED_S(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]);
            _ERR_S(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]);
            break;
        default:
            break;
    }

    /* clear busy bit */
    _BUSY_C(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]);
    /* clear send bit */
    _FIRE_C(_u16MbxGroupIdMBXHAL[eSrcRole][eDstRole]);

    return E_MBX_SUCCESS;
}

