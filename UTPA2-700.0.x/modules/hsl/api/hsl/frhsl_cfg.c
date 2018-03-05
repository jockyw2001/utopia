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
// Copyright (c) 2006-2009 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (uMStar Confidential Informationv) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
#if defined (MSOS_TYPE_LINUX)
#include "frhsl_ringbuffer.h"
#include "frhsl_cfg.h"
#include "frhsl_RAWLOB.h"

#include "MsCommon.h"
#include "drvWDT.h"

#include <pthread.h>

static MS_BOOL _bFrHslReset = FALSE;

#define COUNTERS_PER_US 12 /* 1 counter = (1/12M) sec  */

/*******************************************************************************
* Function    : frHslReset
* Group       : PC Simulation, Framework, HSL, Internals
* Parameter   :
* Returns     : nothing
* Description :
*******************************************************************************/
void FrHslResetConnection(stHslController* const _hsl)
{
  _bFrHslReset = TRUE;

    CmdInfo cmdinfo;
    cmdinfo.u16Cmd = HSL_CMD_RESET;

    bufferProcessCommand(_hsl, &cmdinfo);
}

/*******************************************************************************
* Function    : FrHslInitialiseBegin
* Group       : PC Simulation, Framework, HSL, Internals
* Parameter   : none
* Returns     : nothing
* Description : This function is called to perform any initialisation prior
*               to other generic initialisation configuration.
*******************************************************************************/
MS_BOOL FrHslInitialiseBegin(void)
{
    /* Initialise the counter for geting timestamp info. */

    MS_BOOL bRet = FALSE;

    if ( E_WDT_OK == MDrv_WDT_Init(E_WDT_DBGLV_ERR_ONLY) )    
    {       
        MDrv_TIMER_Count(E_TIMER_1, ENABLE);        
        bRet = TRUE;    
    }

    return bRet;
}

/*******************************************************************************
* Function    : FrHslInitialiseEnd
* Group       : Linux uart, Framework, HSL, Internals
* Parameter   : none
* Returns     : nothing
* Description : This function is called to indicate when initialisation is
*               completed and we are ready to do some logging.
*******************************************************************************/
MS_BOOL FrHslInitialiseEnd(void)
{
    /* Nothing to do as stdout doesn't need closing here.
    However we could possible output some extra information to let the
    PC know that the logging can start.
    Typical info could be outputting a single message log point.
    This is useful to help the PC detect that the target system has been reset.*/

    return TRUE;
}

MS_BOOL FrHslStop(void)
{
    return TRUE;
}

/*******************************************************************************
* Function    : FrHslWritePayload
* Group       : Linux uart, Framework, HSL, Internals
* Parameter   : MS_BOOL: is the last payload?
* Parameter   : MS_U16: payload to write to the given file descriptor.
* Parameter   : MS_U8*: buffer to be written to
* Returns     : nothing
* Description : This function is used to write payload
*******************************************************************************/
void FrHslWritePayload (const MS_BOOL bLast, const MS_U16 u16Payload, MS_U8 *pu8Buff)
{
    if (bLast)
    {
        pu8Buff[HSL_RAWLOB_OFFSET_OF_PAYLOAD_MARKER_IN_PACKET] = HSL_RAWLOB_LAST_PAYLOAD_MARKER;
    }
    else
    {
        pu8Buff[HSL_RAWLOB_OFFSET_OF_PAYLOAD_MARKER_IN_PACKET] = HSL_RAWLOB_MORE_PAYLOAD_MARKER;
    }

    memcpy(&pu8Buff[HSL_RAWLOB_OFFSET_OF_PAYLOAD_IN_PACKET], &u16Payload, 2);
}

/*******************************************************************************
* Function    : FrHslWriteHeader
* Group       : Linux uart, Framework, HSL, Internals
* Parameter   : MS_U8: Message type
* Parameter   : MS_U16: Header id write to the given file descriptor.
* Parameter   : MS_U8*: buffer to be written to
* Returns     : nothing
* Description : This function is used to write header
*******************************************************************************/
void FrHslWriteHeader (const MS_U8 u8MsgType, const MS_U16 u16ID, MS_U8 *pu8Buff, MS_U32 u32Time)
{
    MS_U16 u16TimeStamp = ~((MS_U16)u32Time);

    pu8Buff[HSL_RAWLOB_OFFSET_OF_MSG_TYPE] = u8MsgType;

    memcpy(&pu8Buff[HSL_RAWLOB_OFFSET_OF_MSG_ID], &u16ID, 2);

    memcpy(&pu8Buff[HSL_RAWLOB_OFFSET_OF_TIME_STAMP], &u16TimeStamp, 2);
}

#endif
