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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMX_tee.c
/// @brief  Demux TEE API
/// @author MStar Semiconductor,Inc.
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

#include "asmCPU.h"

#include "drvSEAL.h"
#include "drvTSP_tee.h"
#include "apiDMX_tee.h"

#include "utopia.h"
#include "halCHIP.h"

#if defined (MSOS_TYPE_NUTTX)
#include "../../drv/mbx/apiMBX_St.h"
#include "../../drv/mbx/apiMBX.h"
#endif  //MSOS_TYPE_NUTTX

//------------------------------------------------------------------------------
// Compile options
//------------------------------------------------------------------------------
#define DMX_TEE_DBG                             (0)

//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
// for debugging test only

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if defined (MSOS_TYPE_NUTTX)
#define DMX_STACK_SIZE                                                  (4096)
#endif

#define PRINTF(x)                                                       //x

//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------
#define DMX_MAX_PVR_ENG_NUM                     (4)
#define DMX_MAX_FILEIN_ENG_NUM                  (4)
#define DMX_MAX_MMFI_ENG_NUM                    (2)

//------------------------------------------------------------------------------
// Internal data structure
//------------------------------------------------------------------------------
#if defined (MSOS_TYPE_NUTTX)
typedef enum
{
    E_DMX_TEE_TO_REE_MBX_MSG_NULL,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_INVALID,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_NO_TEE,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL
} DMX_TEE_TO_REE_MBX_ACK_TYPE;
#endif   //MSOS_TYPE_NUTTX

typedef struct _DMX_TEE_PRIV_PARAM
{
    MS_U8   _u8TspFwBufMiuSel;
    MS_U32  _u32TspFwBufSize;
    MS_U32  _u32TspFwBufAddr;
    MS_U32  _u32TspFwBufAddr_H;
    MS_U32  _u32TspFwBufSerSta;

    MS_U8   _u8VQBufMiuSel;
    MS_U32  _u32VQBufSize;
    MS_U32  _u32VQBufAddr;
    MS_U32  _u32VQBufAddr_H;
    MS_U32  _u32VQBufSerSta;

    MS_U8   _u8PvrBufMiuSel[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_Buf0_Size[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_Buf0_Addr[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_Buf0_Addr_H[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_Buf1_Size[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_Buf1_Addr[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_Buf1_Addr_H[DMX_MAX_PVR_ENG_NUM];
    MS_U32  _u32PVR_BufSerSta[DMX_MAX_PVR_ENG_NUM];

    MS_U8   _u8FileinMiuSel[DMX_MAX_FILEIN_ENG_NUM];
    MS_U32  _u32FileinBufAddr[DMX_MAX_FILEIN_ENG_NUM];
    MS_U32  _u32FileinBufAddr_H[DMX_MAX_FILEIN_ENG_NUM];
    MS_U32  _u32FileinBufSize[DMX_MAX_FILEIN_ENG_NUM];
    MS_U32  _u32FileinBufSerSta[DMX_MAX_FILEIN_ENG_NUM];

    MS_U8   _u8MMFIMiuSel[DMX_MAX_MMFI_ENG_NUM];
    MS_U32  _u32MMFIBufAddr[DMX_MAX_MMFI_ENG_NUM];
    MS_U32  _u32MMFIBufAddr_H[DMX_MAX_MMFI_ENG_NUM];
    MS_U32  _u32MMFIBufSize[DMX_MAX_MMFI_ENG_NUM];
    MS_U32  _u32MMFIBufSerSta[DMX_MAX_MMFI_ENG_NUM];

}DMX_TEE_PRIV_PARAM;

//------------------------------------------------------------------------------
// Helper Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------

#if defined (MSOS_TYPE_NUTTX)
MS_U8   _u8DmxTeeStack[DMX_STACK_SIZE];
MS_U8   _u8DmxMbxMsgClass = 0;
MS_S32  _s32Dmx_TEE_EventId = -1;
MS_S32  _s32Dmx_TEE_TaskId = -1;
MBX_Msg _DmxTeeToReeMbxMsg;
MBX_Msg _Dmx_TEE_msg;
#endif

MS_BOOL _bInit = FALSE;

static DMX_TEE_PRIV_PARAM stDMX_Tee_Priv_Param =
{
    ._u8TspFwBufMiuSel = 0,
    ._u32TspFwBufSize = 0,
    ._u32TspFwBufAddr = 0,
    ._u32TspFwBufAddr_H = 0,
    ._u32TspFwBufSerSta = 0,

    ._u8VQBufMiuSel = 0,
    ._u32VQBufSize = 0,
    ._u32VQBufAddr = 0,
    ._u32VQBufAddr_H = 0,
    ._u32VQBufSerSta = 0,

    ._u8PvrBufMiuSel = {0,0},
    ._u32PVR_Buf0_Size = {0,0},
    ._u32PVR_Buf0_Addr = {0,0},
    ._u32PVR_Buf0_Addr_H = {0,0},
    ._u32PVR_Buf1_Size = {0,0},
    ._u32PVR_Buf1_Addr = {0,0},
    ._u32PVR_Buf1_Addr_H = {0,0},
    ._u32PVR_BufSerSta = {0, 0},

    ._u8FileinMiuSel = {0,0,0},
    ._u32FileinBufAddr = {0,0,0},
    ._u32FileinBufAddr_H = {0,0,0},
    ._u32FileinBufSize = {0,0,0},
    ._u32FileinBufSerSta = {0, 0, 0},

    ._u8MMFIMiuSel = {0,0},
    ._u32MMFIBufAddr = {0,0},
    ._u32MMFIBufAddr_H = {0,0},
    ._u32MMFIBufSize = {0,0},
    ._u32MMFIBufSerSta = {0, 0},
};

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------
#if 0
MS_BOOL _DMX_TEE_RIU_LockInit(void)
{
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_FW, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_FW, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP_WP_VQ, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP_RP_VQ, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_PVR, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_PVR, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_PVR1, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_PVR1, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_FILEIN, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_FILEIN, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_MMFI_WP_MMFI0, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_MMFI_RP_MMFI0, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_MMFI_WP_MMFI1, TRUE) == FALSE)
        return FALSE;
    if(MDrv_Seal_BufferLock(E_SEAL_MMFI_RP_MMFI1, TRUE) == FALSE)
        return FALSE;

    return TRUE;
}
#endif

static MS_PHY _DMX_TEE_U32AddrToPhyAddr(MS_U32 u32AddrL, MS_U32 u32AddrH)
{
    MS_PHY phyAddr = 0;
    MS_U32* pu32Temp = (MS_U32*)(&phyAddr);

    if(sizeof(MS_PHY) == 8)
    {
        pu32Temp[0] = u32AddrL;
        pu32Temp[1] = u32AddrH;
        return phyAddr;
    }
    else
    {
        return u32AddrL;
    }
}

static MS_BOOL _DMX_TEE_PhyAddrToU32Addr(MS_PHY phyAddr, MS_U32* pu32AddrL, MS_U32* pu32AddrH)
{
    MS_U32* pu32Temp = NULL;

    if(sizeof(MS_PHY) == 8)
    {
        pu32Temp = (MS_U32*)(&phyAddr);
        *pu32AddrL = pu32Temp[0];
        *pu32AddrH = pu32Temp[1];
    }
    else
    {
        *pu32AddrL = phyAddr;
        *pu32AddrH = 0;
    }

    return TRUE;
}

#if defined (MSOS_TYPE_NUTTX)
MS_BOOL _DMX_TEE_BufParamToMbxParam(MS_U32 u32BufAddr, MS_U32 u32Size, MS_U8 u8MiuSel, MBX_Msg *pMsg)
{
    pMsg->u8Parameters[0] = (MS_U8)(u32Size & 0xFFUL);
    pMsg->u8Parameters[1] = (MS_U8)((u32Size >> 8UL) & 0xFFUL);
    pMsg->u8Parameters[2] = (MS_U8)((u32Size >> 16UL) & 0xFFUL);
    pMsg->u8Parameters[3] = (MS_U8)((u32Size >> 24UL) & 0xFFUL);

    pMsg->u8Parameters[4] = (MS_U8)(u32BufAddr & 0xFFUL);
    pMsg->u8Parameters[5] = (MS_U8)((u32BufAddr >> 8UL) & 0xFFUL);
    pMsg->u8Parameters[6] = (MS_U8)((u32BufAddr >> 16UL) & 0xFFUL);
    pMsg->u8Parameters[7] = (MS_U8)((u32BufAddr >> 24UL) & 0xFFUL);
    pMsg->u8Parameters[8] = u8MiuSel;

    pMsg->u8ParameterCount = 9;

    return TRUE;
}

MS_BOOL _DMX_TEE_MbxParamToBufParam(MBX_Msg *pMsg, MS_U32 *pu32BufAddr, MS_U32 *pu32Size, MS_U8* pu8MiuSel)
{
    if(pMsg->u8ParameterCount < 9)
    {
        return FALSE;
    }

    PRINTF(printf("[%s][%d] %02x %02x %02x %02x %02x %02x %02x %02x\n", __FUNCTION__, __LINE__,
        pMsg->u8Parameters[0], pMsg->u8Parameters[1], pMsg->u8Parameters[2], pMsg->u8Parameters[3],
        pMsg->u8Parameters[4], pMsg->u8Parameters[5], pMsg->u8Parameters[6], pMsg->u8Parameters[7]));

    *pu32Size = ((MS_U32)(pMsg->u8Parameters[0])) & 0xFFUL;
    *pu32Size += ((((MS_U32)(pMsg->u8Parameters[1])) << 8UL) & 0xFF00UL);
    *pu32Size += ((((MS_U32)(pMsg->u8Parameters[2])) << 16UL) & 0xFF0000UL);
    *pu32Size += ((((MS_U32)(pMsg->u8Parameters[3])) << 24UL) & 0xFF000000UL);

    *pu32BufAddr = ((MS_U32)(pMsg->u8Parameters[4])) & 0xFFUL;
    *pu32BufAddr += ((((MS_U32)(pMsg->u8Parameters[5])) << 8UL) & 0xFF00UL);
    *pu32BufAddr += ((((MS_U32)(pMsg->u8Parameters[6])) << 16UL) & 0xFF0000UL);
    *pu32BufAddr += ((((MS_U32)(pMsg->u8Parameters[7])) << 24UL) & 0xFF000000UL);

    *pu8MiuSel = pMsg->u8Parameters[8];

    return TRUE;
}

void _DMX_TEE_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    if(pMsg==NULL)
    {
        PRINTF(printf("[DMX]%s,pMsg is a null pointer\n",__FUNCTION__));
        return;
    }

    if (_s32Dmx_TEE_EventId < 0)
    {
        PRINTF(printf("[DMX]%s,DMX TEE event group is not created\n",__FUNCTION__));
        return;
    }
    memcpy(&_Dmx_TEE_msg, pMsg, sizeof(MBX_Msg));
    MsOS_SetEvent(_s32Dmx_TEE_EventId, 1);
    *pbAddToQueue = FALSE;
    return;
}

void _DMX_TEE_Proc(void)
{
    MS_U32 u32Events;
    MS_U8  u8Index = 0, u8Opt = 0;
    DMX_TEE_BUF_Param stTeeBuf = {0, 0, 0, 0, 0};
    //MBX_Result result;

    PRINTF(printf("DMX TEE_Proc\n"));

    while(1)
    {
        MsOS_WaitEvent(_s32Dmx_TEE_EventId, 1, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        u8Index = _Dmx_TEE_msg.u8Index;

        memset(&_DmxTeeToReeMbxMsg,0,sizeof(MBX_Msg));
        _DmxTeeToReeMbxMsg.eRoleID = E_MBX_CPU_MIPS;
        _DmxTeeToReeMbxMsg.u8Ctrl = 0;
        _DmxTeeToReeMbxMsg.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        _DmxTeeToReeMbxMsg.u8MsgClass = _Dmx_TEE_msg.u8MsgClass;
        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS;
        _DmxTeeToReeMbxMsg.u8Parameters[0] = 0;
        _DmxTeeToReeMbxMsg.u8ParameterCount = 0;

        switch(u8Index)
        {
            case E_DMX_REE_TO_TEE_CMD_FW_LoadCode:
                PRINTF(printf("DMX receive load f/w code in R2\n"));
                stTeeBuf.u32EngId = ((MS_U32)_Dmx_TEE_msg.u8Parameters[0]) & 0xFF;
                stTeeBuf.u32BufAddr = stDMX_Tee_Priv_Param._u32TspFwBufAddr;
                stTeeBuf.u32BufAddr_H = stDMX_Tee_Priv_Param._u32TspFwBufAddr_H;
                stTeeBuf.u32BufSize = stDMX_Tee_Priv_Param._u32TspFwBufSize;
                if(MApi_DMX_TEE_ProcReeCmd(u8Index, (void*)(&stTeeBuf)) != 0)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                break;
            case E_DMX_REE_TO_TEE_CMD_SET_VqBuf:
                stTeeBuf.u32BufAddr = stDMX_Tee_Priv_Param._u32VQBufAddr;
                stTeeBuf.u32BufAddr_H = stDMX_Tee_Priv_Param._u32VQBufAddr_H;
                stTeeBuf.u32BufSize = stDMX_Tee_Priv_Param._u32VQBufSize;
                if(MApi_DMX_TEE_ProcReeCmd(u8Index, (void*)(&stTeeBuf)) != 0)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                break;
            case E_DMX_REE_TO_TEE_CMD_SET_PvrBuf:
                stTeeBuf.u32EngId = ((MS_U32)_Dmx_TEE_msg.u8Parameters[0]) & 0xFF;
                u8Opt = _Dmx_TEE_msg.u8Parameters[1];
                if(u8Opt == 0)
                {
                    stTeeBuf.u32BufSize = 0xFFFFFFFF;
                }
                if(MApi_DMX_TEE_ProcReeCmd(u8Index, (void*)(&stTeeBuf)) != 0)
                {
                    stTeeBuf.u32BufSize = 0;
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    break;
                }
                if(u8Opt == 0)
                {
                    _DMX_TEE_BufParamToMbxParam(stTeeBuf.u32BufAddr, stTeeBuf.u32BufSize, (MS_U8)stTeeBuf.u32Opt, &_DmxTeeToReeMbxMsg);
                }
                break;
            case E_DMX_REE_TO_TEE_CMD_SET_FileinBuf:
            case E_DMX_REE_TO_TEE_CMD_SET_MMFIBuf:
                stTeeBuf.u32EngId = ((MS_U32)_Dmx_TEE_msg.u8Parameters[9]) & 0xFF;
                if(_DmxTeeToReeMbxMsg.u8ParameterCount != 10)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    break;
                }
                if(_DMX_TEE_MbxParamToBufParam(&_Dmx_TEE_msg, &stTeeBuf.u32BufAddr, &stTeeBuf.u32BufSize, &u8Index) == FALSE)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    break;
                }
                stTeeBuf.u32Opt = ((MS_U32)u8Index) & 0xFF;
                if(MApi_DMX_TEE_ProcReeCmd(u8Index, (void*)(&stTeeBuf)) != 0)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                break;
            case E_DMX_REE_TO_TEE_CMD_GET_PvrWPtr:
                stTeeBuf.u32EngId = ((MS_U32)_Dmx_TEE_msg.u8Parameters[0]) & 0xFF;
                if(MApi_DMX_TEE_ProcReeCmd(u8Index, (void*)(&stTeeBuf)) != 0)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                else
                {
                    _DmxTeeToReeMbxMsg.u8ParameterCount = 5;
                    _DmxTeeToReeMbxMsg.u8Parameters[0] = stDMX_Tee_Priv_Param._u8PvrBufMiuSel[stTeeBuf.u32EngId];
                    _DmxTeeToReeMbxMsg.u8Parameters[1] = (MS_U8)(stTeeBuf.u32BufAddr & 0xFFUL);
                    _DmxTeeToReeMbxMsg.u8Parameters[2] = (MS_U8)((stTeeBuf.u32BufAddr >> 8UL) & 0xFFUL);
                    _DmxTeeToReeMbxMsg.u8Parameters[3] = (MS_U8)((stTeeBuf.u32BufAddr >> 16UL) & 0xFFUL);
                    _DmxTeeToReeMbxMsg.u8Parameters[4] = (MS_U8)((stTeeBuf.u32BufAddr >> 24UL) & 0xFFUL);
                }
                break;
            default:
                _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                PRINTF(printf("[DMX]%s,wrong command %d sent from HK\n",__FUNCTION__,(unsigned int)u8Index));
                break;
        }

        MApi_MBX_SendMsg(&_DmxTeeToReeMbxMsg);

#if 0
        if (E_MBX_SUCCESS != result)
        {
            PRINTF(printf("[DMX]Ack fail,classid:%x,cmd:%x\n",_DmxTeeToReeMbxMsg.u8MsgClass, _DmxTeeToReeMbxMsg.u8Index));
        }
        else
        {
            PRINTF(printf("[DMX]Ack ok,classid:%x\n",_DmxTeeToReeMbxMsg.u8MsgClass));
        }
#endif
    }
}

#endif   //MSOS_TYPE_NUTTX

//-------------------------------------------------------------------------------------------------
/// Create DMX Tee service task
/// @return TRUE or FALSE for Pass or Fail
/// @note Please call this API when Tee system init, and must create this task before DMX init is called
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_Create_Tee_System(void)
{

#if defined (MSOS_TYPE_NUTTX)
    if(_u8DmxMbxMsgClass == 0)
    {
        if(MApi_MBX_GenerateDynamicClass(E_MBX_CPU_MIPS, "DMX_TEE",&_u8DmxMbxMsgClass) != E_MBX_SUCCESS)
        {
            PRINTF(printf("[DMX]DMX GenerateDynamicClass error\n"));
            _u8DmxMbxMsgClass = 0;
            return FALSE;
        }

        if (E_MBX_SUCCESS != MApi_MBX_RegisterMSGWithCallBack(_u8DmxMbxMsgClass,10,_DMX_TEE_HandleDynamicMBX))
        {
            PRINTF(printf("[DMX]TEE Register MBX MSG error\n"));
            _u8DmxMbxMsgClass = 0;
            return FALSE;
        }
    }

    if(_s32Dmx_TEE_EventId == -1)
    {
        _s32Dmx_TEE_EventId = MsOS_CreateEventGroup("DMX_TEE_Event");
        if (_s32Dmx_TEE_EventId < 0)
        {
            PRINTF(printf("[DMX]TEE CreateEventGroup error....\n"));
            return FALSE;
        }
    }
#endif  //MSOS_TYPE_NUTTX

        _bInit = MDrv_TSP_Tee_Init();

#if defined (MSOS_TYPE_NUTTX)
    if(_s32Dmx_TEE_TaskId == -1)
    {
        _s32Dmx_TEE_TaskId = MsOS_CreateTask((TaskEntry)_DMX_TEE_Proc,
                                              NULL,
                                              E_TASK_PRI_MEDIUM,
                                              TRUE,
                                              (void*)_u8DmxTeeStack,
                                              DMX_STACK_SIZE,
                                              "DMX_TEE_Task");
        if (_s32Dmx_TEE_TaskId < 0)
        {
            MsOS_DeleteEventGroup(_s32Dmx_TEE_EventId);
            PRINTF(printf("[DMX]TEE CreateTask error....\n"));
            return FALSE;
        }
    }
#endif //MSOS_TYPE_NUTTX

    return TRUE;
}

//------------------------------------------------------------------------------
/// SetControl for TEE
/// @param cmd \b IN : TEE user cmd
/// @param param \b IN : TEE control parameters
/// @return MS_BOOL
/// @note For buffer control commend, the input parameters should be DMX_TEE_BUF_Param
//------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_SetControl(DMX_TEE_USER_CMD cmd, void* param)
{
    MS_BOOL ret = TRUE;
    MS_PHY phyAddr = 0;
    DMX_TEE_BUF_Param *pTeeBuf = (DMX_TEE_BUF_Param*)param;

    if(param == NULL)
    {
        PRINTF(printf("[DMX_TEE]Invalid param \n"));
    }

    switch(cmd)
    {
        case E_DMX_TEECMD_SET_TSPFW_BUF:
            stDMX_Tee_Priv_Param._u32TspFwBufAddr = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32TspFwBufAddr_H = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32TspFwBufSize = pTeeBuf->u32BufSize;
            stDMX_Tee_Priv_Param._u8TspFwBufMiuSel = (MS_U8)pTeeBuf->u32Opt;
            PRINTF(printf("DMX_TEE cmd %x, buf %x %x %d\n", (int)cmd,
                (int)stDMX_Tee_Priv_Param._u32TspFwBufAddr, (int)stDMX_Tee_Priv_Param._u32TspFwBufSize, (int)stDMX_Tee_Priv_Param._u8TspFwBufMiuSel));
            break;

        case E_DMX_TEECMD_SET_TSPVQ_BUF:
            stDMX_Tee_Priv_Param._u32VQBufAddr = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32VQBufAddr_H = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32VQBufSize = pTeeBuf->u32BufSize;
            stDMX_Tee_Priv_Param._u8VQBufMiuSel = (MS_U8)(pTeeBuf->u32Opt);
            PRINTF(printf("[DMX_TEE] cmd %x, buf %x %x %d\n", (int)cmd,
                (int)stDMX_Tee_Priv_Param._u32VQBufAddr, (int)stDMX_Tee_Priv_Param._u32VQBufSize, (int)stDMX_Tee_Priv_Param._u8VQBufMiuSel));
            break;

        case E_DMX_TEECMD_SET_PVR_BUF:
            if(pTeeBuf->u32EngId >= DMX_MAX_PVR_ENG_NUM)
            {
                return FALSE;
            }

            stDMX_Tee_Priv_Param._u8PvrBufMiuSel[pTeeBuf->u32EngId] = ((MS_U8)pTeeBuf->u32Opt);
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId] = pTeeBuf->u32BufSize >> 1;

            PRINTF(printf("[DMX_TEE] cmd %x, eng %d , buf %x %x \n", (int)cmd, (int)pTeeBuf->u32EngId,
                (int)stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId], (int)stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId]));
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(pTeeBuf->u32BufAddr, pTeeBuf->u32BufAddr_H);
            phyAddr += stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId];
            _DMX_TEE_PhyAddrToU32Addr(phyAddr, &stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[pTeeBuf->u32EngId], &stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr_H[pTeeBuf->u32EngId]);
            stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[pTeeBuf->u32EngId] = stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId];
            break;

        case E_DMX_TEECMD_SET_FILEIN_BUF:
            if(pTeeBuf->u32EngId >= DMX_MAX_FILEIN_ENG_NUM)
            {
                return FALSE;
            }
            stDMX_Tee_Priv_Param._u8FileinMiuSel[pTeeBuf->u32EngId] = (MS_U8)(pTeeBuf->u32Opt);
            stDMX_Tee_Priv_Param._u32FileinBufAddr[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32FileinBufAddr_H[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32FileinBufSize[pTeeBuf->u32EngId] = pTeeBuf->u32BufSize;

            PRINTF(printf("[DMX_TEE] cmd %x, eng %d, buf %x %x %d\n", (int)cmd, (int)pTeeBuf->u32EngId,
                (int)stDMX_Tee_Priv_Param._u32FileinBufAddr[pTeeBuf->u32EngId], (int)stDMX_Tee_Priv_Param._u32FileinBufSize[pTeeBuf->u32EngId], (int)stDMX_Tee_Priv_Param._u8FileinMiuSel[pTeeBuf->u32EngId]));
            break;

        case E_DMX_TEECMD_SET_MMFI_BUF:
            if(pTeeBuf->u32EngId >= DMX_MAX_MMFI_ENG_NUM)
            {
                return FALSE;
            }
            stDMX_Tee_Priv_Param._u8MMFIMiuSel[pTeeBuf->u32EngId] = ((MS_U8)pTeeBuf->u32Opt);
            stDMX_Tee_Priv_Param._u32MMFIBufAddr[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32MMFIBufAddr_H[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32MMFIBufSize[pTeeBuf->u32EngId] = pTeeBuf->u32BufSize;
            break;

        default:
            PRINTF(printf("[DMX_TEE] Not support cmd,%d\n", (unsigned int)cmd));
            break;
    }

    return ret;
}

//------------------------------------------------------------------------------
/// Process REE commend
/// @param cmd \b IN : Ree Commend
/// @param param \b IN : TEE control parameters
/// @return MS_U32 Error code
/// @note For buffer control commend, the input parameters should be DMX_TEE_BUF_Param
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_TEE_ProcReeCmd(DMX_REE_TO_TEE_CMD_TYPE cmd, void* param)
{
    DMX_TEE_BUF_Param *pTeeBuf = (DMX_TEE_BUF_Param*)param;
    MS_PHY phyAddr = 0 ,phyAddr1 = 0;
    MS_U32 u32Res = 0xFFFFFFFF;

    PRINTF(printf("[DMX_TEE] cmd %d, eng %d , opt %d, buf %x, buf_h %x,, size %d\n", (int)cmd, (int)pTeeBuf->u32EngId, (int)pTeeBuf->u32Opt,
        (int)pTeeBuf->u32BufAddr , (int)pTeeBuf->u32BufAddr_H, (int)pTeeBuf->u32BufSize));

    if(_bInit == FALSE)
    {
        if((_bInit = MDrv_TSP_Tee_Init()) == FALSE)
            return u32Res;
    }

    switch(cmd)
    {
        case E_DMX_REE_TO_TEE_CMD_FW_LoadCode:
            #if DMX_TEE_DBG
            stDMX_Tee_Priv_Param._u32TspFwBufAddr = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32TspFwBufAddr_H = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32TspFwBufSize = pTeeBuf->u32BufSize;
            stDMX_Tee_Priv_Param._u8TspFwBufMiuSel = (MS_U8)pTeeBuf->u32Opt;
            #endif

            if(stDMX_Tee_Priv_Param._u32TspFwBufSize == 0)
            {
                break;
            }
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32TspFwBufAddr, stDMX_Tee_Priv_Param._u32TspFwBufAddr_H);

            if(pTeeBuf->u32EngId == 0)
            {
                #if (defined(MSOS_TYPE_OPTEE) && (DMX_TEE_DBG == 0))
                MApi_DMX_ConfigPipe(DMX_PIPE_TSPFW, 1, 0); //lock fw buffer first, for oneway
                #endif
                if(MDrv_TSP_Tee_CopyFw(MsOS_PA2KSEG1(phyAddr), phyAddr, stDMX_Tee_Priv_Param._u32TspFwBufSize) == TRUE)
                {
                    u32Res = 0;
                }
            }
            else
            {
                memset((void*)MsOS_PA2KSEG1(phyAddr), 0, stDMX_Tee_Priv_Param._u32TspFwBufSize);
                MAsm_CPU_Sync();
                MsOS_FlushMemory();
                #if (defined(MSOS_TYPE_OPTEE) && (DMX_TEE_DBG == 0))
                MApi_DMX_ConfigPipe(DMX_PIPE_TSPFW, 0, 0); //unlock fw buffer first, for oneway
                #endif
                u32Res = 0;
            }
            break;

        case E_DMX_REE_TO_TEE_CMD_SET_VqBuf:
            #if DMX_TEE_DBG
            stDMX_Tee_Priv_Param._u32VQBufAddr = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32VQBufAddr_H = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32VQBufSize = pTeeBuf->u32BufSize;
            stDMX_Tee_Priv_Param._u8VQBufMiuSel = (MS_U8)(pTeeBuf->u32Opt);
            #endif
            if(stDMX_Tee_Priv_Param._u32VQBufSize == 0)
            {
                break;
            }

            #if (defined(MSOS_TYPE_OPTEE) && (DMX_TEE_DBG == 0))
            //MApi_DMX_ConfigPipe(DMX_PIPE_TSPVQ, 1, 1); //lock vq buffer first
            #endif

            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32VQBufAddr, stDMX_Tee_Priv_Param._u32VQBufAddr_H);
            if(MDrv_TSP_Tee_Set_VQBuffer(phyAddr, stDMX_Tee_Priv_Param._u32VQBufSize) == TRUE)
            {
                u32Res = 0;
            }
            break;

        case E_DMX_REE_TO_TEE_CMD_SET_PvrBuf:
            #if DMX_TEE_DBG
            stDMX_Tee_Priv_Param._u8PvrBufMiuSel[pTeeBuf->u32EngId] = ((MS_U8)pTeeBuf->u32Opt);
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId] = pTeeBuf->u32BufSize >> 1;
            stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr+stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId];
            stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr_H[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[pTeeBuf->u32EngId] = pTeeBuf->u32BufSize >> 1;
            #endif
            if(pTeeBuf->u32BufSize != 0)
            {
                if((stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId] == 0) || (stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId] == 0))
                {
                    break;
                }
                phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId], stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[pTeeBuf->u32EngId]);
                phyAddr1 = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[pTeeBuf->u32EngId], stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr_H[pTeeBuf->u32EngId]);
                if(MDrv_TSP_Tee_Set_PVRBuffer((MS_U8)pTeeBuf->u32EngId, phyAddr, stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId], phyAddr1,
                    stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[pTeeBuf->u32EngId]) == TRUE)
                {
                    pTeeBuf->u32BufAddr = stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId];
                    pTeeBuf->u32BufAddr_H = stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[pTeeBuf->u32EngId];
                    pTeeBuf->u32BufSize = stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId] + stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[pTeeBuf->u32EngId];
                    pTeeBuf->u32Opt = ((MS_U32)(stDMX_Tee_Priv_Param._u8PvrBufMiuSel[pTeeBuf->u32EngId])) & 0xFF;
                    u32Res = 0;
                }
            }
            else //reset buffer to 0
            {
                if(MDrv_TSP_Tee_Set_PVRBuffer((MS_U8)pTeeBuf->u32EngId, 0, 0, 0, 0) == TRUE)
                {
                    stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u32EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[pTeeBuf->u32EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u32EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[pTeeBuf->u32EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr_H[pTeeBuf->u32EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[pTeeBuf->u32EngId] = 0;
                    stDMX_Tee_Priv_Param._u8PvrBufMiuSel[pTeeBuf->u32EngId] = 0;
                    u32Res = 0;
                }
            }
            break;

        case E_DMX_REE_TO_TEE_CMD_SET_FileinBuf:
            #if DMX_TEE_DBG
            stDMX_Tee_Priv_Param._u8FileinMiuSel[pTeeBuf->u32EngId] = (MS_U8)(pTeeBuf->u32Opt);
            stDMX_Tee_Priv_Param._u32FileinBufAddr[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32FileinBufAddr_H[pTeeBuf->u32EngId] = pTeeBuf->u32BufAddr_H;
            stDMX_Tee_Priv_Param._u32FileinBufSize[pTeeBuf->u32EngId] = pTeeBuf->u32BufSize;
            #endif

            phyAddr = _DMX_TEE_U32AddrToPhyAddr(pTeeBuf->u32BufAddr, pTeeBuf->u32BufAddr_H);
            phyAddr1 = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32FileinBufAddr[pTeeBuf->u32EngId], stDMX_Tee_Priv_Param._u32FileinBufAddr_H[pTeeBuf->u32EngId]);

            PRINTF(printf("[DMX_TEE] cmd %x, eng %d, phyAddr %x phyAddr1 %x \n", (int)cmd, (int)pTeeBuf->u32EngId, (int)phyAddr, (int)phyAddr1));
            if((((MS_U8)pTeeBuf->u32Opt) == stDMX_Tee_Priv_Param._u8FileinMiuSel[pTeeBuf->u32EngId]) &&
                    (phyAddr >= phyAddr1) &&
                    ((phyAddr + pTeeBuf->u32BufSize) <= (phyAddr1 + stDMX_Tee_Priv_Param._u32FileinBufSize[pTeeBuf->u32EngId])))
            {
                if(MDrv_TSP_Tee_Set_FileinBuffer((MS_U8)pTeeBuf->u32EngId, phyAddr, pTeeBuf->u32BufSize) == TRUE)
                {
                    u32Res = 0;
                }
            }
            else
            {
                MDrv_TSP_Tee_Set_FileinBuffer((MS_U8)pTeeBuf->u32EngId, 0, 0);
            }
            break;

        case E_DMX_REE_TO_TEE_CMD_SET_MMFIBuf:
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(pTeeBuf->u32BufAddr, pTeeBuf->u32BufAddr_H);
            phyAddr1 = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32MMFIBufAddr[pTeeBuf->u32EngId], stDMX_Tee_Priv_Param._u32MMFIBufAddr_H[pTeeBuf->u32EngId]);
            if((((MS_U8)pTeeBuf->u32Opt) == stDMX_Tee_Priv_Param._u8MMFIMiuSel[pTeeBuf->u32EngId]) &&
                    (phyAddr >= phyAddr1) &&
                    ((phyAddr + pTeeBuf->u32BufSize) <= (phyAddr1 + stDMX_Tee_Priv_Param._u32MMFIBufSize[pTeeBuf->u32EngId])))
            {
                if(MDrv_TSP_Tee_Set_MMFIBuffer((MS_U8)pTeeBuf->u32EngId, phyAddr, pTeeBuf->u32BufSize) == TRUE)
                {
                    u32Res = 0;
                }
            }
            else
            {
                MDrv_TSP_Tee_Set_MMFIBuffer((MS_U8)pTeeBuf->u32EngId, 0, 0);
            }
            break;

        case E_DMX_REE_TO_TEE_CMD_GET_PvrWPtr:
            if(MDrv_TSP_Tee_Get_PVRWriteAddr((MS_U8)pTeeBuf->u32EngId, &phyAddr) == TRUE)
            {
                _DMX_TEE_PhyAddrToU32Addr(phyAddr, &pTeeBuf->u32BufAddr, &pTeeBuf->u32BufAddr_H);
                PRINTF(printf("[DMX_TEE] W 0x%x\n", (int)phyAddr));
                u32Res = 0;
            }
            else
            {
                pTeeBuf->u32BufAddr = 0;
                pTeeBuf->u32BufAddr_H = 0;
            }
            break;

        default:
            break;
    }

    return u32Res;
}

#ifdef MSOS_TYPE_OPTEE
//------------------------------------------------------------------------------
/// Get secure resource by Pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param pu32MaxNRetRes \b IN : Pointer to store max request resource number
/// @param pResource \b OUT : Pointer to store resource data
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_GetResourceByPipeID(MS_U32 u32PipeID, MS_U32* pu32MaxNRetRes, DMX_TEE_RESOURCE* pResource)
{
    PRINTF(printf("[DMX_TEE] u32PipeID %x \n", (int)u32PipeID));

    if(u32PipeID == DMX_PIPE_NULL)
        return UTOPIA_STATUS_FAIL;

    pResource->ID[0] = u32PipeID;
    pResource->ID[1] = 0; pResource->ID[2] = 0; pResource->ID[3] = 0;

    switch(u32PipeID)
    {
        case DMX_PIPE_TSPFW:
            pResource->ID[0] = stDMX_Tee_Priv_Param._u32TspFwBufSize;;
            pResource->ID[1] = stDMX_Tee_Priv_Param._u32TspFwBufAddr;
            pResource->ID[2] = stDMX_Tee_Priv_Param._u32TspFwBufAddr_H;
            break;

        case DMX_PIPE_TSPVQ:
            pResource->ID[0] = stDMX_Tee_Priv_Param._u32VQBufSize;
            pResource->ID[1] = stDMX_Tee_Priv_Param._u32VQBufAddr;
            pResource->ID[2] = stDMX_Tee_Priv_Param._u32VQBufAddr_H;
            break;

        case DMX_PIPE_PVR_0:
        case DMX_PIPE_PVR_1:
        case DMX_PIPE_PVR_2:
        case DMX_PIPE_PVR_3:
            pResource->ID[0] = stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[u32PipeID & 0xFF] + stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[u32PipeID & 0xFF];
            pResource->ID[1] = stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[u32PipeID & 0xFF];
            pResource->ID[2] = stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[u32PipeID & 0xFF];
            break;

        case DMX_PIPE_FILE_0:
        case DMX_PIPE_FILE_1:
        case DMX_PIPE_FILE_2:
        case DMX_PIPE_FILE_3:
            pResource->ID[0] = stDMX_Tee_Priv_Param._u32FileinBufSize[u32PipeID & 0xFF];
            pResource->ID[1] = stDMX_Tee_Priv_Param._u32FileinBufAddr[u32PipeID & 0xFF];
            pResource->ID[2] = stDMX_Tee_Priv_Param._u32FileinBufAddr_H[u32PipeID & 0xFF];
            break;

        case DMX_PIPE_MMFI_0:
        case DMX_PIPE_MMFI_1:
            pResource->ID[0] = stDMX_Tee_Priv_Param._u32MMFIBufSize[u32PipeID & 0xFF];
            pResource->ID[1] = stDMX_Tee_Priv_Param._u32MMFIBufAddr[u32PipeID & 0xFF];
            pResource->ID[2] = stDMX_Tee_Priv_Param._u32MMFIBufAddr_H[u32PipeID & 0xFF];
            break;

        default:
            return UTOPIA_STATUS_NOT_SUPPORTED;
    }

    *pu32MaxNRetRes = 1;

    PRINTF(printf("[DMX_TEE] pu32MaxNRetRes %x, pResource %x %x %x %x \n",(int)pu32MaxNRetRes, (int)pResource->ID[0], (int)pResource->ID[1], (int)pResource->ID[2], (int)pResource->ID[3]));

    return UTOPIA_STATUS_SUCCESS;
}

//------------------------------------------------------------------------------
/// Configure secure resource by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_ConfigPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    MS_PHY phyAddr = 0;
    MS_U32 u32Size = 0;
    MS_U32 u32EngId = u32PipeID;

    PRINTF(printf("[DMX_TEE] u32PipeID %x , u32SecureDMA %x, u32OperationMode %x\n", (int)u32PipeID, (int)u32SecureDMA, (int)u32OperationMode));

    if(u32PipeID == DMX_PIPE_NULL)
        return UTOPIA_STATUS_FAIL;


    if(MDrv_Seal_ChangeIPSecureDMAAbillity(MODULE_DMX, 0 , u32SecureDMA) == FALSE)
    {
        PRINTF(printf("[DMX_TEE] MDrv_Seal_ChangeIPSecureDMAAbillity fail\n"));
        return UTOPIA_STATUS_FAIL;
    }


    switch(u32PipeID)
    {
        case DMX_PIPE_TSPFW:
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32TspFwBufAddr, stDMX_Tee_Priv_Param._u32TspFwBufAddr_H);
            u32Size = stDMX_Tee_Priv_Param._u32TspFwBufSize;
            stDMX_Tee_Priv_Param._u32TspFwBufSerSta = u32SecureDMA;
            break;

        case DMX_PIPE_TSPVQ:
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32VQBufAddr, stDMX_Tee_Priv_Param._u32VQBufAddr_H);
            u32Size = stDMX_Tee_Priv_Param._u32VQBufSize;
            stDMX_Tee_Priv_Param._u32VQBufSerSta = u32SecureDMA;
            break;

        case DMX_PIPE_PVR_0:
        case DMX_PIPE_PVR_1:
        case DMX_PIPE_PVR_2:
        case DMX_PIPE_PVR_3:
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[u32PipeID & 0xFF], stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr_H[u32PipeID & 0xFF]);
            u32Size = stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[u32PipeID & 0xFF] + stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[u32PipeID & 0xFF];
            stDMX_Tee_Priv_Param._u32PVR_BufSerSta[u32PipeID & 0xFF] = u32SecureDMA;
            break;

        case DMX_PIPE_FILE_0:
        case DMX_PIPE_FILE_1:
        case DMX_PIPE_FILE_2:
        case DMX_PIPE_FILE_3:
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32FileinBufAddr[u32PipeID & 0xFF], stDMX_Tee_Priv_Param._u32FileinBufAddr_H[u32PipeID & 0xFF]);
            u32Size = stDMX_Tee_Priv_Param._u32FileinBufSize[u32PipeID & 0xFF];
            stDMX_Tee_Priv_Param._u32FileinBufSerSta[u32PipeID & 0xFF] = u32SecureDMA;
            break;

        case DMX_PIPE_MMFI_0:
        case DMX_PIPE_MMFI_1:
            phyAddr = _DMX_TEE_U32AddrToPhyAddr(stDMX_Tee_Priv_Param._u32MMFIBufAddr[u32PipeID & 0xFF], stDMX_Tee_Priv_Param._u32MMFIBufAddr_H[u32PipeID & 0xFF]);
            u32Size = stDMX_Tee_Priv_Param._u32MMFIBufSize[u32PipeID & 0xFF];
            stDMX_Tee_Priv_Param._u32MMFIBufSerSta[u32PipeID & 0xFF] = u32SecureDMA;
            break;

        default:
            return UTOPIA_STATUS_NOT_SUPPORTED;
    }

    if(MDrv_Seal_SetSecureRange(phyAddr, u32Size, u32OperationMode) == FALSE)
    {
        PRINTF(printf("[DMX_TEE] MDrv_Seal_SetSecureRange fail\n"));
        return UTOPIA_STATUS_FAIL;
    }


    if((u32PipeID & DMX_PIPE_FILE_0) == DMX_PIPE_FILE_0)
    {
        if((u32PipeID != DMX_PIPE_MMFI_0) && (u32PipeID != DMX_PIPE_MMFI_1))
        {
            u32EngId = DMX_PIPE_FILE_0 | MDrv_TSP_Tee_FileEngId_Vir2Phy((MS_U8)u32PipeID);
        }
    }

    switch(u32EngId)
    {
        case DMX_PIPE_TSPFW:
            MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_FW, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_FW, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_TSPVQ:
            MDrv_Seal_BufferLock(E_SEAL_TSP_WP_VQ, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP_RP_VQ, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_PVR_0:
            MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_PVR, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_PVR, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_PVR_1:
            MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_PVR1, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_PVR1, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_PVR_2:
            MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_PVR2, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_PVR2, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_PVR_3:
            MDrv_Seal_BufferLock(E_SEAL_TSP_WP_PVR3, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP_RP_PVR3, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_FILE_0:
            MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_FILEIN, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_FILEIN, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_FILE_1:
            MDrv_Seal_BufferLock(E_SEAL_TSP0_WP_FILEIN1, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP0_RP_FILEIN1, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_FILE_2:
            MDrv_Seal_BufferLock(E_SEAL_TSP_WP_FILEIN2, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP_RP_FILEIN2, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_FILE_3:
            MDrv_Seal_BufferLock(E_SEAL_TSP_WP_FILEIN3, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_TSP_WP_FILEIN3, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_MMFI_0:
            MDrv_Seal_BufferLock(E_SEAL_MMFI_WP_MMFI0, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_MMFI_RP_MMFI0, (MS_BOOL)u32OperationMode);
            break;

        case DMX_PIPE_MMFI_1:
            MDrv_Seal_BufferLock(E_SEAL_MMFI_WP_MMFI1, (MS_BOOL)u32OperationMode);
            MDrv_Seal_BufferLock(E_SEAL_MMFI_RP_MMFI1, (MS_BOOL)u32OperationMode);
            break;

        default:
            return UTOPIA_STATUS_NOT_SUPPORTED;
    }

    PRINTF(printf("[DMX_TEE] MApi_DMX_ConfigPipe OK\n"));

    return UTOPIA_STATUS_SUCCESS;
}

//------------------------------------------------------------------------------
/// Check secure resource status by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Status: 0 -> Configure failed; 1 -> Configure OK
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_CheckPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    PRINTF(printf("[DMX_TEE] u32PipeID %x , u32SecureDMA %x, u32OperationMode %x\n", (int)u32PipeID, (int)u32SecureDMA, (int)u32OperationMode));

    if(u32PipeID == DMX_PIPE_NULL)
        return UTOPIA_STATUS_FAIL;

    switch(u32PipeID)
    {
        case DMX_PIPE_TSPFW:
            if(stDMX_Tee_Priv_Param._u32TspFwBufSerSta == 0)
                return UTOPIA_STATUS_FAIL;

        case DMX_PIPE_TSPVQ:
            if(stDMX_Tee_Priv_Param._u32VQBufSerSta == 0)
                return UTOPIA_STATUS_FAIL;

        case DMX_PIPE_PVR_0:
        case DMX_PIPE_PVR_1:
        case DMX_PIPE_PVR_2:
        case DMX_PIPE_PVR_3:
            if(stDMX_Tee_Priv_Param._u32PVR_BufSerSta[u32PipeID & 0xFF] == 0)
                return UTOPIA_STATUS_FAIL;

        case DMX_PIPE_FILE_0:
        case DMX_PIPE_FILE_1:
        case DMX_PIPE_FILE_2:
        case DMX_PIPE_FILE_3:
            if(stDMX_Tee_Priv_Param._u32FileinBufSerSta[u32PipeID & 0xFF] == 0)
                return UTOPIA_STATUS_FAIL;

        case DMX_PIPE_MMFI_0:
        case DMX_PIPE_MMFI_1:
            if(stDMX_Tee_Priv_Param._u32MMFIBufSerSta[u32PipeID & 0xFF] == 0)
                return UTOPIA_STATUS_FAIL;

        default:
            return UTOPIA_STATUS_FAIL;
    }

    return UTOPIA_STATUS_SUCCESS;
}
#endif //MSOS_TYPE_OPTEE
