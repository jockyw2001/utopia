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
// Copyright (c) 2008-2011 MStar Semiconductor, Inc.
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
/// file    halMJPEG.c
/// @brief  MJPEG hal interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "halCHIP.h"
// Internal Definition
//#include "regCHIP.h"
#include "drvMMIO.h" //get RIU base
#include "drvMJPEG.h"
#include "VirtualhalMJPEG.h"

#include "regVPU_EX.h" // For VPU
#include "halVPU_EX.h"


#include "drvSYS.h"

#include "../hvd_v3/fwHVD_if.h"
#define NJPD_FW_VERSION  "NJPD.FW.00130062"



static HAL_MJPEG_Stream _stMJPEGStream[E_MJPEG_MJPEG_TOTAL] =
{
#ifdef VDEC3
    {E_HAL_MJPEG_N_STREAM0, FALSE},
    {E_HAL_MJPEG_N_STREAM1, FALSE},
    {E_HAL_MJPEG_N_STREAM2, FALSE},
    {E_HAL_MJPEG_N_STREAM3, FALSE},
    {E_HAL_MJPEG_N_STREAM4, FALSE},
    {E_HAL_MJPEG_N_STREAM5, FALSE},
    {E_HAL_MJPEG_N_STREAM6, FALSE},
    {E_HAL_MJPEG_N_STREAM7, FALSE},
    {E_HAL_MJPEG_N_STREAM8, FALSE},
#else
    {E_HAL_MJPEG_MAIN_STREAM0, FALSE},
    {E_HAL_MJPEG_SUB_STREAM0, FALSE},
#endif
};

/*================================ MJPEG =====================================*/

/******************************************************************************/
///Stop VPU
/******************************************************************************/
void VirtualHal_MJPEG_StopVPU(void)
{
    //HAL_VPU_PowerCtrl(FALSE);
    HAL_VPU_EX_DeInit();
}

/******************************************************************************/
///Release VPU
/******************************************************************************/
void VirtualHal_MJPEG_ReleaseVPU(void)
{
    HAL_VPU_EX_SwRstRelse();
}


/******************************************************************************/
///Check VPU MailBox ready or not
///@return TRUE or FALSE
///    - TRUE, MailBox is free
///    - FALSE, MailBox is busy
///@param u8MBox \b IN: MailBox to check
///     - VPU_HI_MBOX0,
///     - VPU_HI_MBOX1,
///     - VPU_RISC_MBOX0,
///     - VPU_RISC_MBOX1,
/******************************************************************************/
MS_BOOL VirtualHal_MJPEG_IsVPUMBoxReady(MS_U8 u8MBox)
{
    MS_BOOL bResult = FALSE;
    bResult = HAL_VPU_EX_MBoxRdy((MS_U32)u8MBox);
    return bResult;
}

/******************************************************************************/
///Read message from VPU
///@return TRUE or FALSE
///    - TRUE, success
///    - FALSE, failed
///@param u8MBox \b IN: MailBox to read
///    - VPU_RISC_MBOX0
///    - VPU_RISC_MBOX1
///@param u32Msg \b OUT: message read
/******************************************************************************/
MS_BOOL VirtualHal_MJPEG_ReadVPUMBox(MS_U8 u8MBox, MS_U32 *u32Msg)
{
    MS_BOOL bResult = TRUE;
    bResult = HAL_VPU_EX_MBoxRead((MS_U32)u8MBox, u32Msg);
    return bResult;
}

///-----------------------------------------------------------------------------
/// Send message to VPU
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - VPU_HI_MBOX0,
///     - VPU_HI_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL VirtualHal_MJPEG_SendVPUMBox(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 u32Msg)
{
    MS_BOOL bResult = TRUE;
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    u32Msg |= (u8TaskId << 24);

    bResult = HAL_VPU_EX_MBoxSend((MS_U32)u8MBox, u32Msg);
    return bResult;
}

///-----------------------------------------------------------------------------
/// Clear VPU RISC MBox
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - VPU_RISC_MBOX0,
///     - VPU_RISC_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL VirtualHal_MJPEG_ClearVPUMBox(MS_U8 u8MBox)
{
    HAL_VPU_EX_MBoxClear((MS_U32)u8MBox);
    return TRUE;
}

MJPEG_MSTAR_CHIP_ID VirtualHal_MJPEG_Get_Chip_ID(void)
{
    return E_MJPEG_MSTAR_CHIP_T8;
}

HAL_MJPEG_StreamId VirtualHal_MJPEG_GetFreeStream(HAL_MJPEG_StreamType eStreamType)
{
    MS_U32 i = 0;

    if (eStreamType == E_HAL_MJPEG_MAIN_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_MJPEG_MAIN_STREAM_MAX - E_HAL_MJPEG_MAIN_STREAM_BASE) +
              (E_HAL_MJPEG_SUB_STREAM_MAX - E_HAL_MJPEG_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_MJPEG_MAIN_STREAM_BASE & _stMJPEGStream[i].eStreamId) && (FALSE == _stMJPEGStream[i].bUsed))
            {
                return _stMJPEGStream[i].eStreamId;
            }
        }
    }
    else if (eStreamType == E_HAL_MJPEG_SUB_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_MJPEG_MAIN_STREAM_MAX - E_HAL_MJPEG_MAIN_STREAM_BASE) +
              (E_HAL_MJPEG_SUB_STREAM_MAX - E_HAL_MJPEG_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_MJPEG_SUB_STREAM_BASE & _stMJPEGStream[i].eStreamId) && (FALSE == _stMJPEGStream[i].bUsed))
            {
                return _stMJPEGStream[i].eStreamId;
            }
        }
    }
#ifdef VDEC3
    else if ((eStreamType >= E_HAL_MJPEG_N_STREAM) && (eStreamType < E_HAL_MJPEG_N_STREAM + E_MJPEG_MJPEG_TOTAL))
    {
        i = eStreamType - E_HAL_MJPEG_N_STREAM;
        if (!_stMJPEGStream[i].bUsed)
            return _stMJPEGStream[i].eStreamId;
    }
#endif

    return E_HAL_MJPEG_STREAM_NONE;
}


void VirtualHal_MJPEG_Init(MS_U8 u8DrvId)
{
    _stMJPEGStream[u8DrvId].bUsed = TRUE;

}

void VirtualHal_MJPEG_DeInit(MS_U8 u8DrvId)
{
    _stMJPEGStream[u8DrvId].bUsed = FALSE;
}


#endif
