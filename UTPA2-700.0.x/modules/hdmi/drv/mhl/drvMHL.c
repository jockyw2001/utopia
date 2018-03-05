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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mdrv_mhl.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MHL_C_
#define _DRV_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsIRQ.h"
#include "MsOS.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#include "utopia.h"
#include "utopia_dapi.h"

#include "mdrv_mhl_st.h"
#include "apiMHL_private.h"
#include "apiMHL.h"
#include "halMHL.h"
#include "drvMHL.h"

#include "drvSYS.h"
#include "drvMMIO.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MDRV_MHL_MSG_INFO(format, args...)      ULOGI("MHL", format, ##args)
#define MDRV_MHL_MSG_WARNING(format, args...)   ULOGW("MHL", format, ##args)
#define MDRV_MHL_MSG_DEBUG(format, args...)     ULOGD("MHL", format, ##args)
#define MDRV_MHL_MSG_ERROR(format, args...)     ULOGE("MHL", format, ##args)
#define MDRV_MHL_MSG_FATAL(format, args...)     ULOGF("MHL", format, ##args)

#else
#define MDRV_MHL_MSG_INFO(format, args...)      printf(format, ##args)
#define MDRV_MHL_MSG_WARNING(format, args...)   printf(format, ##args)
#define MDRV_MHL_MSG_DEBUG(format, args...)     printf(format, ##args)
#define MDRV_MHL_MSG_ERROR(format, args...)     printf(format, ##args)
#define MDRV_MHL_MSG_FATAL(format, args...)     printf(format, ##args)

#endif

#define MHL_DEBUG_RECEIVE_MSC       0
#define MHL_DEBUG_TIMER_EVENT       0
#define MHL_DEBUG_SOURCE_INFO       0
#define MHL_DEBUG_BIST_MODE         0
#define MHL3_AUTO_EQ_ENABLE         1

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
MHL_RESOURCE_PRIVATE stMHLResourcePrivate;

#endif

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                   _mdrv_mhl_InitRiuBase()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_InitRiuBase(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bGetBaseFlag = FALSE;
    MS_VIRT ulRiuBaseAddress = 0;
    MS_VIRT ulPMRiuBaseAddress = 0;
    MS_PHY ulNonPMBankSize = 0;
    MS_PHY ulPMBankSize = 0;

    // get MMIO base
    if(!MDrv_MMIO_GetBASE(&ulRiuBaseAddress, &ulNonPMBankSize, MS_MODULE_XC))
    {
        MDRV_MHL_MSG_ERROR("** MHL get XC base failed\n");
    }
    else if(!MDrv_MMIO_GetBASE(&ulPMRiuBaseAddress, &ulPMBankSize, MS_MODULE_PM))
    {
        MDRV_MHL_MSG_ERROR("** MHL get PM base failed\n");
    }
    else
    {
        pMHLResourcePrivate->ulRiuBaseAddress = ulRiuBaseAddress;
        pMHLResourcePrivate->ulPMRiuBaseAddress = ulPMRiuBaseAddress;

        mhal_mhl_SetRegisterBaseAddress(pMHLResourcePrivate->ulRiuBaseAddress, pMHLResourcePrivate->ulPMRiuBaseAddress);

        bGetBaseFlag = TRUE;
    }

    return bGetBaseFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ClearCbusFlag
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ClearCbusFlag(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucSelect)
{
    MS_U8 uctemp = 0;

    switch(ucSelect)
    {
        case MHL_CLEAR_INITIAL_FLAG:
            pMHLResourcePrivate->ulSystemIndex = 0;
            pMHLResourcePrivate->ulCurrentTimerTick = (MsOS_GetSystemTime()) %CBUS_TIMER_CONTROL_MASK;
            pMHLResourcePrivate->uc3DTotalEntryDTD = 0;
            pMHLResourcePrivate->uc3DTotalEntryVIC = 0;
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_NONE;
            pMHLResourcePrivate->ucCallBackFunctionIndex = 0;
            pMHLResourcePrivate->slMHLPollingTaskID = -1;
            pMHLResourcePrivate->usPrePowerState = 0;
            pMHLResourcePrivate->ucHighEndVICTotalEntry = 0;
            pMHLResourcePrivate->stSignalStatusInfo.ucImpedanceValue = MHL_IMPEDANCE_VALUE;
            pMHLResourcePrivate->stSignalStatusInfo.ucImpedanceOffset = MHL_IMPEDANCE_VALUE;
            pMHLResourcePrivate->stSignalStatusInfo.ucIControlValue = 0;

            for(uctemp = 0; uctemp <MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
            {
                if(uctemp < MHL_3D_DTD_INFORMATION_LENGTH)
                {
                    pMHLResourcePrivate->uc3DInformationDTD[uctemp] = 0;
                }

                if(uctemp < MHL_3D_VIC_INFORMATION_LENGTH)
                {
                    pMHLResourcePrivate->uc3DInformationVIC[uctemp] = 0;
                }

                if(uctemp < MHL_HEV_VIC_INFORMATION_LENGTH)
                {
                    pMHLResourcePrivate->usHighEndVICTable[uctemp] = 0;
                }

                if(uctemp < MHL_AUDIO_TUNNEL_INFORMATION_LENGTH)
                {
                    pMHLResourcePrivate->ulAudioTunnelTable[uctemp] = 0;
                }

                if(uctemp < MHL_EMSC_SUPPORT_INFORMATION_LENGTH)
                {
                    pMHLResourcePrivate->usEMSCSupportTable[uctemp] = 0;
                }
            }

            //pMHLResourcePrivate->usHighEndVICTable[0] = MHL_HEV_VIC_INDEX0;
            //pMHLResourcePrivate->usHighEndVICTable[1] = MHL_HEV_VIC_INDEX1;
            //pMHLResourcePrivate->usHighEndVICTable[2] = MHL_HEV_VIC_INDEX2;
            //pMHLResourcePrivate->usHighEndVICTable[3] = MHL_HEV_VIC_INDEX3;
            //pMHLResourcePrivate->usHighEndVICTable[4] = MHL_HEV_VIC_INDEX4;
            //pMHLResourcePrivate->usHighEndVICTable[5] = MHL_HEV_VIC_INDEX5;

            pMHLResourcePrivate->ulAudioTunnelTable[0] = MHL_AUDIO_TUNNEL_INDEX0;
            pMHLResourcePrivate->ulAudioTunnelTable[1] = MHL_AUDIO_TUNNEL_INDEX1;
            pMHLResourcePrivate->ulAudioTunnelTable[2] = MHL_AUDIO_TUNNEL_INDEX2;
            pMHLResourcePrivate->ulAudioTunnelTable[3] = MHL_AUDIO_TUNNEL_INDEX3;
            pMHLResourcePrivate->ulAudioTunnelTable[4] = MHL_AUDIO_TUNNEL_INDEX4;
            pMHLResourcePrivate->ulAudioTunnelTable[5] = MHL_AUDIO_TUNNEL_INDEX5;

            pMHLResourcePrivate->usEMSCSupportTable[0] = MHL_EMSC_SUPPORT_BURST_ID0;
            pMHLResourcePrivate->usEMSCSupportTable[1] = MHL_EMSC_SUPPORT_BURST_ID1;
            pMHLResourcePrivate->usEMSCSupportTable[2] = MHL_EMSC_SUPPORT_BURST_ID2;
            pMHLResourcePrivate->usEMSCSupportTable[3] = MHL_EMSC_SUPPORT_BURST_ID3;
            pMHLResourcePrivate->usEMSCSupportTable[4] = MHL_EMSC_SUPPORT_BURST_ID4;
            pMHLResourcePrivate->usEMSCSupportTable[5] = MHL_EMSC_SUPPORT_BURST_ID5;

            break;

        case MHL_CLEAR_CABLE_LOSS_FLAG:
        case MHL_CLEAR_CONNECT_LOSS_FLAG:
            pMHLResourcePrivate->ucProcDoneIndex = 0;
            pMHLResourcePrivate->usDisplayIndex = 0;
            pMHLResourcePrivate->ulCbusReceiveIndex = 0;
            pMHLResourcePrivate->ulCbusSendIndex = 0;
            pMHLResourcePrivate->ucPreRCPKeyCode = 0;
            pMHLResourcePrivate->ulEnableIndex = 0;
            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_NONE;
            pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_NONE;
            pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_NONE;
            pMHLResourcePrivate->ucECbusTrainState = MHL_ECBUS_STATE_DISABLE;
            pMHLResourcePrivate->ucWriteBurstSequence = 0;
            pMHLResourcePrivate->usEMSCTxBufferSize = MHL_EMSC_DATA_SIZE;
            pMHLResourcePrivate->usEMSCRxBufferSize = MHL_EMSC_DATA_SIZE;
            pMHLResourcePrivate->ucEMSCSendStatus = MHL_EMSC_SEND_NONE;

            for(uctemp = 0; uctemp <MHL_CBUS_TOTAL_QUEUE_NUM; uctemp++)
            {
                pMHLResourcePrivate->stCbusQueueInfo[uctemp].ucQueueState = MHL_QUEUE_STATE_IDLE;
            }

            pMHLResourcePrivate->stSignalStatusInfo.bECbusEnableFlag = FALSE;
            pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag = FALSE;
            pMHLResourcePrivate->stSignalStatusInfo.bClockStableFlag = FALSE;
            pMHLResourcePrivate->stSignalStatusInfo.bAutoEnableFlag = FALSE;
            pMHLResourcePrivate->stSignalStatusInfo.bSignalStableFlag = FALSE;
            pMHLResourcePrivate->stSignalStatusInfo.ucAutoEQState = MHL_AUTO_EQ_START;
            pMHLResourcePrivate->stSignalStatusInfo.ucStableCount = 0;
            pMHLResourcePrivate->stSignalStatusInfo.ucECbusTrainState = MHL_EMSC_SEND_NONE;

            break;

        case MHL_CLEAR_CBUS_TIME_OUT_FLAG:
            break;

        case MHL_CLEAR_MSG_RESPONSE_FLAG:
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusGetLeastFlag()
//  [Description]:
//                  MHL Cbus get the least True flag in index
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_CbusGetLeastFlag(MS_U8 unindex)
{
    MS_U8 uctemp = BIT(0);

    if(unindex == 0)
    {
        return unindex;
    }

    while(!(unindex &uctemp))
    {
        uctemp = uctemp <<1;
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetLeastBitNum()
//  [Description]:
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_GetLeastBitNum(MS_U16 usData)
{
    MS_U8 uctemp = 0;

    while(!(usData &BIT(0)))
    {
        usData = usData >>1;
        uctemp++;
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSetPacket(BYTE ucHeader, Bool bControl, BYTE ucContent)
//  [Description]
//                  MHL Cbus Packet setting
//  [Arguments]:
//                  ucHeader: Packet header type
//                  bControl: Packet control bit type
//                  ucContent: Packet payload
//  [Return]:
//                  Packet word for register write
//**************************************************************************
MS_U16 _mdrv_mhl_CbusSetPacket(MS_U8 ucHeader, MS_BOOL bControl, MS_U8 ucContent)
{
    MS_BOOL bParity = 0;
    MS_U16 usPacket = (ucHeader <<9) |(bControl <<8) |ucContent;

    do
    {
        bParity ^= (usPacket & BIT(0));
        usPacket = usPacket >>1;
    }
    while(usPacket);

    usPacket = (bParity <<11) |(ucHeader <<9) |(bControl <<8) |ucContent;

    return (usPacket |BIT(15));
}
//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusTxWriteCommand()
//  [Description]:
//                  MHL Cbus send command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus command send successfully
//                  FALSE: Cbus command send fail
//**************************************************************************
MS_BOOL _mdrv_mhl_CbusTxWriteCommand(MS_BOOL bECbusEnableFlag, stMHL_QUEUE_INFO *stCbusQueueInfo)
{
    MS_BOOL bIndex = TRUE;
    MS_U8 uctemp = 0;
    mhalCbusFifo_S pCbusFifo;

    pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, stCbusQueueInfo->ucCommand);

    switch(stCbusQueueInfo->ucCommand)
    {
        case MSC_ACK:
            pCbusFifo.lens = stCbusQueueInfo->ucLength;
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_ACK);

            if(stCbusQueueInfo->ucLength == 2)
            {
                // payload 2
                pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[0]);
            }
            break;

        case MSC_NACK:
            pCbusFifo.lens = 1;
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_NACK);
            break;

        case MSC_ABORT:
            pCbusFifo.lens = 1;
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_ABORT);
            break;

        case MSC_WRITE_STAT_OR_SET_INT: // 0x60
        case MSC_WRITE_XSTAT: // 0x70
            // payload 1
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_WRITE_STAT_OR_SET_INT);
            // payload 2
            pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[0]);
            // payload 3
            pCbusFifo.databuf[2] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[1]);
            // message length
            pCbusFifo.lens = 3;
            break;

        case MSC_READ_DEVCAP: // 0x61
        case MSC_READ_XDEVCAP: // 0x71
            // payload 1
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_READ_DEVCAP);
            // payload 2
            pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[0]);
            // message length
            pCbusFifo.lens = 2;
            break;

        case MSC_GET_STATE: // 0x62
        case MSC_GET_VENDOR_ID: // 0x63
        case MSC_SET_HPD: // 0x64
        case MSC_CLR_HPD: // 0x65
        case MSC_GET_SC1_ERRORCODE: // 0x69 - Get channel 1 command error code
        case MSC_GET_DDC_ERRORCODE: // 0x6A - Get DDC channel command error code.
        case MSC_GET_MSC_ERRORCODE: // 0x6B - Get MSC command error code.
        case MSC_GET_SC3_ERRORCODE: // 0x6D - Get channel 3 command error code.
            // payload 1
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, pReq->cmd);
            // message length
            pCbusFifo.lens = 1;
            break;

        case MSC_MSC_MSG: // 0x68
            // payload 1
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_MSC_MSG);
            // payload 2
            pCbusFifo.databuf[1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[0]);
            // payload 3
            pCbusFifo.databuf[2] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[1]);
            // message length
            pCbusFifo.lens = 3;
            break;

        case MSC_WRITE_BURST: // 0x6C
            // payload 1
            //pCbusFifo.databuf[0] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_WRITE_BURST);
            // payload 2 ~ 17
            for(uctemp =0; uctemp <stCbusQueueInfo->ucLength; uctemp++) // offset packet + the maxi 16 packets
            {
                pCbusFifo.databuf[uctemp +1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_DATA, stCbusQueueInfo->ucData[uctemp]);
            }
            // EOF
            pCbusFifo.databuf[stCbusQueueInfo->ucLength +1] = _mdrv_mhl_CbusSetPacket(MSC_HEADER, CBUS_CONTROL, MSC_EOF);
            // message length
            pCbusFifo.lens = stCbusQueueInfo->ucLength +2;
            break;

        default:
            bIndex = FALSE;
            break;
    }

    if(bIndex)
    {
        if(!mhal_mhl_CBusWrite(bECbusEnableFlag, &pCbusFifo))
        {
            return FALSE;
        }
    }

    return bIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetFreeQueueIndex()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_GetFreeQueueIndex(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 uctemp = 0;
    MS_U8 ucQueueIndex = 0;

    for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
    {
        ucQueueIndex = (pMHLResourcePrivate->ucCurrentQueueIndex +uctemp +1) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

        if(pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState == MHL_QUEUE_STATE_IDLE)
        {
            break;
        }
        else
        {
            ucQueueIndex = MHL_CBUS_TOTAL_QUEUE_NUM;
        }
    }

    if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM) // Have free queue
    {
        uctemp = (ucQueueIndex +MHL_CBUS_TOTAL_QUEUE_NUM -2) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

        if(pMHLResourcePrivate->stCbusQueueInfo[uctemp].ucQueueState == MHL_QUEUE_STATE_IDLE)
        {
            ucQueueIndex = uctemp;
        }
    }

    return ucQueueIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusQueueIndex()
//  [Description]:
//                  MHL Cbus get transmitting queue index
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_GetCbusQueueIndex(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 uctemp = 0;
    MS_U8 ucQueueIndex = 0;
    
    // Check Rx queue have packet transmit success or not
    if(pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucQueueState != MHL_QUEUE_STATE_IDLE)
    {
        ucQueueIndex = MHL_CBUS_RX_QUEUE_INDEX;
    }
    else // Check Tx queue have packet transmit success or not, search start  from current index
    {
        for(uctemp = 0; uctemp <(MHL_CBUS_TOTAL_QUEUE_NUM -1); uctemp++)
        {
            ucQueueIndex = (pMHLResourcePrivate->ucCurrentQueueIndex +uctemp) %(MHL_CBUS_TOTAL_QUEUE_NUM -1);

            if(pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState != MHL_QUEUE_STATE_IDLE)
            {
                break;
            }
            else
            {
                ucQueueIndex = MHL_CBUS_TOTAL_QUEUE_NUM;
            }
        }
    }

    return ucQueueIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetCbusTimerEvent()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetCbusTimerEvent(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucTimerEvent, MS_U16 usTimerTick)
{
    if(usTimerTick > 0)
    {
        pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].bEnableFlag = TRUE;

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL set timer event %d tick %d\r\n", ucTimerEvent, usTimerTick);
#endif
    }

    pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].usTimerTick = usTimerTick;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRxQueueInsertMSG(BYTE subCmd, BYTE databuf)
//  [Description]
//                  MHL Cbus Reply MSC RCP or RAP into Queue
//  [Arguments]:
//                  subCmd: MSC subcommands
//                  databuf: command code
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
void _mdrv_mhl_CbusRxQueueInsertMSG(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucSubCommand, MS_U8 ucKeyCode)
{
    pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucQueueState = MHL_QUEUE_STATE_PENDING;
    pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucLength = 2;
    pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucCommand = MSC_MSC_MSG;
    pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucData[0] = ucSubCommand;
    pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucData[1] = ucKeyCode;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_WriteBurstCheckSum()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_WriteBurstCheckSum(MS_U8 *pData)
{
    MS_U8 uctemp = 0;
    MS_U8 ucCheckSum = 0;

    for(uctemp = 0; uctemp <MHL_MSC_SCRATCHPAD_SIZE; uctemp++)
    {
        ucCheckSum = ucCheckSum +pData[uctemp];
    }

    ucCheckSum = (ucCheckSum ^0xFF) +1;

    return ucCheckSum;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckSendCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_CheckSendCommand(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bSendCommand = FALSE;
    MS_BOOL bMHLPort = mhal_mhl_CheckInputPort(pMHLResourcePrivate->ucMHLSupportPath);

    if(bMHLPort && mhal_mhl_CheckPIPWindow()) // MHL port and scaler mux on
    {
        bSendCommand = TRUE;
    }
    else if(GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_FORCE_SEND_COMMAND_FLAG) && GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_FORCE_SEND_COMMAND_FLAG))
    {
        bSendCommand = TRUE;
    }

    return bSendCommand;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetRAPEnableFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetRAPEnableFlag(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bRAPEnable = FALSE;

    if(!mhal_mhl_CheckInputPort(pMHLResourcePrivate->ucMHLSupportPath))
    {
        bRAPEnable = TRUE;
    }
    else if(!mhal_mhl_CheckPIPWindow())
    {
        bRAPEnable = TRUE;
    }
    else if(pMHLResourcePrivate->stSignalStatusInfo.bSignalStableFlag)
    {
        bRAPEnable = TRUE;
    }

    return bRAPEnable;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckCbusModeChange()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_CheckCbusModeChange(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bNotCbusChange = TRUE;

    if(pMHLResourcePrivate->ucMSGData[1] == MHL_RAP_KEY_CBUS_MODE_UP)
    {
        bNotCbusChange = FALSE;

        pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_RECEIVE_MODE_UP;
    }
    else if(pMHLResourcePrivate->ucMSGData[1] == MHL_RAP_KEY_CBUS_MODE_DOWN)
    {
        bNotCbusChange = FALSE;

        pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_RECEIVE_MODE_DOWN;
    }

    return bNotCbusChange;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_DTDPresentinEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_DTDPresentinEDID(MS_U8 *pData)
{
    MS_BOOL bPresent = FALSE;
    MS_U8 uctemp = 0;

    for(uctemp = 0; uctemp < 3; uctemp++)
    {
        if(pData[uctemp] > 0)
        {
            bPresent = TRUE;

            break;
        }
    }

    return bPresent;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDTD3DTimingInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_GetDTD3DTimingInformation(MS_U8 *pData)
{
    MS_BOOL bInterlace = ((pData[17] &BIT(7)) ?TRUE :FALSE);
    MS_U8 ucIndex = 0;
    MS_U16 usHActive = ((pData[4] &BMASK(7:4)) <<4) |pData[2];
    MS_U16 usHBlank = ((pData[4] &BMASK(3:0)) <<8) |pData[3];
    MS_U16 usVActive = ((pData[7] &BMASK(7:4)) <<4) |pData[5];
    MS_U16 usVBlank = ((pData[7] &BMASK(3:0)) <<8) |pData[6];
    MS_U16 usHTotal = usHActive +usHBlank;
    MS_U16 usVTotal = usVActive +usVBlank;
    MS_U16 usPixelClk = (pData[1] <<8) |pData[0];
    MS_U32 ulFrequenct = 0;

    if((usHTotal > 0) && (usVTotal > 0))
    {
        ulFrequenct = (usPixelClk *100 /usHTotal) *1000 /usVTotal;

        if((usHActive == 1920) && (usVActive == 1080)) // 1920x1080p
        {
            if((ulFrequenct > 230) && (ulFrequenct < 245)) // 24Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1920x1080p24Hz;
            }
        }
        else if((usHActive == 1920) && (usVActive == 540) && bInterlace) // 1920x1080i
        {
            if((ulFrequenct > 590) && (ulFrequenct < 610)) // 60Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1920x1080i60Hz;
            }
            else if((ulFrequenct > 490) && (ulFrequenct < 510)) // 50Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1920x1080i50Hz;
            }
        }
        else if((usHActive == 1280) && (usVActive == 720)) // 1280x720p
        {
            if((ulFrequenct > 590) && (ulFrequenct < 610)) // 60Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1280x720p60Hz;
            }
            else if((ulFrequenct > 490) && (ulFrequenct < 510)) // 50Hz
            {
                ucIndex = MHL_3D_PRESENT_VIC_1280x720p50Hz;
            }
        }
    }

    return ucIndex;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertDTD3DInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_InsertDTD3DInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucIndex, MS_U8 ucData)
{
    MS_U8 uctemp = 0;

    ucData = ((ucData &BMASK(6:5)) >> 4) |(ucData &BIT(0));

    switch(ucData)
    {
        case MHL_3D_DTD_STEREO_FIELD_SEQUENTIAL_RIGHT:
            uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_SUPPORT_MASK);
            break;

        case MHL_3D_DTD_STEREO_FIELD_SEQUENTIAL_LEFT:
            uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_SUPPORT_MASK);
            break;

        case MHL_3D_DTD_STEREO_2WAY_INTERLEAVED_RIGHT:
            // No setting
            break;

        case MHL_3D_DTD_STEREO_2WAY_INTERLEAVED_LEFT:
            // No setting
            break;

        case MHL_3D_DTD_STEREO_4WAY_INTERLEAVED:
            // No setting
            break;

        case MHL_3D_DTD_STEREO_SIDE_BY_SIDE_INTERLEAVED:
            uctemp = MASKSET(uctemp, MHL_3D_LR_SUPPORT, MHL_3D_SUPPORT_MASK);
            break;

        default: // MHL_3D_DTD_STEREO_NORMAL_DISPLAY

            break;
    };

    pMHLResourcePrivate->uc3DInformationDTD[ucIndex] = uctemp;

    for(uctemp = 0, ucData = 0; uctemp < (ucIndex +1); uctemp++)
    {
        if(pMHLResourcePrivate->uc3DInformationDTD[uctemp] > 0)
        {
            ucData = uctemp +1;
        }
    }

    return ucData;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckVIC3DStructureAll()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_CheckVIC3DStructureAll(MS_U16 usData)
{
    MS_U8 uctemp = 0;

    if((usData &BIT(0)) == BIT(0)) // Support frame packing 3D formats
    {
        uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_FS_SUPPORT);
    }

    if((usData &BIT(6)) == BIT(6)) // Support top and bottom 3D formats
    {
        uctemp = MASKSET(uctemp, MHL_3D_TB_SUPPORT, MHL_3D_TB_SUPPORT);
    }

    if((usData &BIT(8)) == BIT(8)) // Support side by side 3D formats
    {
        uctemp = MASKSET(uctemp, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckVIC3DStructure()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_CheckVIC3DStructure(MS_U8 ucData)
{
    MS_U8 uctemp = 0;

    if(ucData == 0) // Support frame packing
    {
        uctemp = MASKSET(uctemp, MHL_3D_FS_SUPPORT, MHL_3D_FS_SUPPORT);
    }
    else if(ucData == 6) // Support top and bottom
    {
        uctemp = MASKSET(uctemp, MHL_3D_TB_SUPPORT, MHL_3D_TB_SUPPORT);
    }
    else if(ucData == 8) // Support side by side
    {
        uctemp = MASKSET(uctemp, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
    }

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertHighEndVICTable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertHighEndVICTable(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucVICCode)
{
    switch(ucVICCode)
    {
        case MHL_HEV_VIC_INDEX0:
        case MHL_HEV_VIC_INDEX1:
        case MHL_HEV_VIC_INDEX2:
        case MHL_HEV_VIC_INDEX3:
        case MHL_HEV_VIC_INDEX4:
        case MHL_HEV_VIC_INDEX5:
            pMHLResourcePrivate->usHighEndVICTable[pMHLResourcePrivate->ucHighEndVICTotalEntry] = ucVICCode;
            pMHLResourcePrivate->ucHighEndVICTotalEntry++;
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingVIC3DInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_ParsingVIC3DInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucExtendLength, MS_U8 *pData)
{
    MS_BOOL bVICParseFinish = TRUE;
    MS_BOOL b3DPresent = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucIndex = 0;
    MS_U8 ucValue = 0;
    MS_U8 ucLength = 0;
    MS_U8 ucVSDBAddr = 0;
    MS_U8 ucVSDBLength = 0;
    MS_U8 uc3DMultiPresent = 0;
    MS_U8 ucVICTimingNum = 0;
    MS_U8 ucVICTiming[MHL_3D_VIC_INFORMATION_LENGTH] = {0};
    MS_U16 usMaskValue = 0;

    for(uctemp = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
    {
        ucVICTiming[uctemp] = 0;

        pMHLResourcePrivate->uc3DInformationVIC[uctemp] = 0;
    }

    for(uctemp = 0; uctemp < ucExtendLength; uctemp++)
    {
        ucValue = (pData[uctemp] &BMASK(7:5)) >>5;
        ucLength = pData[uctemp] &BMASK(4:0);

        switch(ucValue)
        {
            case MHL_EDID_AUDIO_DATA_BLOCK_CODE:
                MDRV_MHL_MSG_INFO("** MHL get audio data block length %d in port %c\r\n", ucLength, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                break;

            case MHL_EDID_VIDEO_DATA_BLOCK_CODE:
                MDRV_MHL_MSG_INFO("** MHL get video data block length %d in port %c\r\n", ucLength, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                if(ucLength > MHL_3D_VIC_INFORMATION_LENGTH)
                {
                    ucVICTimingNum = MHL_3D_VIC_INFORMATION_LENGTH;
                }
                else
                {
                    ucVICTimingNum = ucLength;
                }

                for(ucIndex = 0; ucIndex <ucLength; ucIndex++)
                {
                    if(ucIndex <ucVICTimingNum)
                    {
                        ucVICTiming[ucIndex] = pData[uctemp +ucIndex +1];
                    }

                    _mdrv_mhl_InsertHighEndVICTable(pMHLResourcePrivate, pData[uctemp +ucIndex +1]);
                }

                break;

            case MHL_EDID_VENDER_SPECIFIC_DATA_BLOCK_CODE:
                MDRV_MHL_MSG_INFO("** MHL get vender specific data block length %d in port %c\r\n", ucLength, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                ucVSDBAddr = uctemp;

                bVICParseFinish = FALSE;

                break;

            case MHL_EDID_SPEAKER_ALLOCATION_DATA_BLOCK_CODE:
                MDRV_MHL_MSG_INFO("** MHL get speaker allocation data block length %d in port %c\r\n", ucLength, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                break;

            case MHL_EDID_VESA_DTC_DATA_BLOCK_CODE:
                MDRV_MHL_MSG_INFO("** MHL get VESA DTC data block length %d in port %c\r\n", ucLength, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                break;

            case MHL_EDID_USE_EXTENDED_TAG_BLOCK_CODE:
                MDRV_MHL_MSG_INFO("** MHL get use extended tag length %d in port %c\r\n", ucLength, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                break;

            default:

                break;
        };

        uctemp = uctemp +ucLength;
    }

    ucIndex = MHL_VIC_PARSING_START;
    ucVSDBLength = ucVSDBAddr +1;

    while(!bVICParseFinish)
    {
        ucValue = pData[ucVSDBAddr];

        if(ucVSDBAddr > ucVSDBLength) // Check total length
        {
            ucIndex = MHL_VIC_PARSING_FINISH;
        }

        switch(ucIndex)
        {
            case MHL_VIC_PARSING_START:
                ucVSDBLength = pData[ucVSDBAddr] &BMASK(4:0);

                if(ucVSDBLength < 8) // HDMI_Video_Present absent, no 3D information
                {
                    ucIndex = MHL_VIC_PARSING_FINISH;
                }
                else
                {
                    ucVSDBAddr = ucVSDBAddr +8;

                    ucIndex = MHL_VIC_PARSING_CHECK_HDMI_VIDEO_PRESENT;
                }

                ucVSDBLength = ucVSDBAddr +ucVSDBLength;

                break;

            case MHL_VIC_PARSING_CHECK_HDMI_VIDEO_PRESENT:
                if((ucValue &BIT(5)) != BIT(5)) // HDMI_Video_Present absent, no 3D information
                {
                    ucIndex = MHL_VIC_PARSING_FINISH;
                }
                else
                {
                    ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT;

                    if((ucValue &BIT(7)) != BIT(7)) // Latency_Fields_Present absent, no video and audio latency and no interlaced video and audio latency
                    {
                        ucVSDBAddr++;
                    }
                    else
                    {
                        if((ucValue &BIT(6)) != BIT(6)) // I_Latency_Fields_Present absent, no video and audio latency
                        {
                            ucVSDBAddr = ucVSDBAddr +3;
                        }
                        else
                        {
                            ucVSDBAddr = ucVSDBAddr +5;
                        }
                    }
                }

                break;

            case MHL_VIC_PARSING_CHECK_3D_PRESENT:
                if((ucValue &BIT(7)) != BIT(7)) // 3D_Present absent, no 3D information
                {
                    ucIndex = MHL_VIC_PARSING_FINISH;
                }
                else
                {
                    b3DPresent = TRUE;
                    uc3DMultiPresent = (ucValue &BMASK(6:5)) >> 5;

                    if(uc3DMultiPresent == 0)
                    {
                        ucIndex = MHL_VIC_PARSING_3D_MULTI_PRESENT_00;
                    }
                    else if(uc3DMultiPresent == 3) // Reserved for future use
                    {
                        ucIndex = MHL_VIC_PARSING_FINISH;
                    }
                    else
                    {
                        ucIndex = MHL_VIC_PARSING_CHECK_HDMI_VIC;

                        ucVSDBAddr++;
                    }
                }

                break;

            case MHL_VIC_PARSING_CHECK_HDMI_VIC:
                ucLength = (ucValue &BMASK(7:5)) >> 5;

                ucVSDBAddr = ucVSDBAddr +ucLength +1;

                if(uc3DMultiPresent == 1)
                {
                    ucIndex = MHL_VIC_PARSING_3D_MULTI_PRESENT_01;
                }
                else // uc3DMultiPresent = 2
                {
                    ucIndex = MHL_VIC_PARSING_3D_MULTI_PRESENT_10;
                }

                ucLength = ucVSDBAddr +(ucValue &BMASK(4:0)) -1; // HDMI_3D_LEN

                break;

            case MHL_VIC_PARSING_3D_MULTI_PRESENT_00:
                ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING;

                break;

            case MHL_VIC_PARSING_3D_MULTI_PRESENT_01:
                ucValue = _mdrv_mhl_CheckVIC3DStructureAll((pData[ucVSDBAddr +1] |(pData[ucVSDBAddr] << 8)));

                for(uctemp = 0; uctemp <ucVICTimingNum; uctemp++)
                {
                    pMHLResourcePrivate->uc3DInformationVIC[uctemp] = ucValue;
                }

                ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING;

                break;

            case MHL_VIC_PARSING_3D_MULTI_PRESENT_10:
                ucValue = _mdrv_mhl_CheckVIC3DStructureAll((pData[ucVSDBAddr +1] |(pData[ucVSDBAddr] << 8)));

                usMaskValue = pData[ucVSDBAddr +3] |(pData[ucVSDBAddr +2] << 8);

                for(uctemp = 0; uctemp <ucVICTimingNum; uctemp++)
                {
                    if((usMaskValue &BIT(uctemp)) == BIT(uctemp))
                    {
                        pMHLResourcePrivate->uc3DInformationVIC[uctemp] = ucValue;
                    }
                }

                ucVSDBAddr = ucVSDBAddr +4;

                ucIndex = MHL_VIC_PARSING_CHECK_3D_STRUCTURE;

                break;

            case MHL_VIC_PARSING_CHECK_3D_STRUCTURE:
                if(ucVSDBAddr > ucLength) // No 3D sturcture
                {
                    ucIndex = MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING;
                }
                else
                {
                    uctemp = (ucValue &BMASK(7:4)) >> 4;
                    ucValue = ucValue &BMASK(3:0);

                    pMHLResourcePrivate->uc3DInformationVIC[uctemp] = _mdrv_mhl_CheckVIC3DStructure(ucValue);

                    if(ucValue < 8) // Only 2D_VIC_Order and 3D_Structure, 1 byte
                    {
                        ucVSDBAddr++;
                    }
                    else // Add 3D_Detail, 2 byte
                    {
                        ucVSDBAddr = ucVSDBAddr +2;
                    }
                }

                break;

            case MHL_VIC_PARSING_CHECK_3D_PRESENT_TIMING:
                for(uctemp = 0; uctemp <ucVICTimingNum; uctemp++)
                {
                    ucValue = pMHLResourcePrivate->uc3DInformationVIC[uctemp];

                    switch(ucVICTiming[uctemp])
                    {
                        case MHL_3D_PRESENT_VIC_1920x1080p24Hz: // Support frame packing with top and bottom
                        case MHL_3D_PRESENT_VIC_1280x720p60Hz: // Support frame packing with top and bottom
                        case MHL_3D_PRESENT_VIC_1280x720p50Hz: // Support frame packing with top and bottom
                            ucValue = MASKSET(ucValue, (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT), (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT));
                            pMHLResourcePrivate->uc3DInformationVIC[uctemp] = ucValue;

                            break;

                        case MHL_3D_PRESENT_VIC_1920x1080i60Hz: // Support side by side
                        case MHL_3D_PRESENT_VIC_1920x1080i50Hz: // Support side by side
                            ucValue = MASKSET(ucValue, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
                            pMHLResourcePrivate->uc3DInformationVIC[uctemp] = ucValue;

                            break;

                        default:

                            break;
                    };
                }

                ucIndex = MHL_VIC_PARSING_FINISH;

                break;

            case MHL_VIC_PARSING_FINISH:
                bVICParseFinish = TRUE;

                break;

            default:

                break;
        };
    }

    for(uctemp = 0, ucValue = 0; uctemp < MHL_3D_VIC_INFORMATION_LENGTH; uctemp++)
    {
        if(pMHLResourcePrivate->uc3DInformationVIC[uctemp] > 0)
        {
            ucValue = uctemp +1;
        }
    }

    pMHLResourcePrivate->uc3DTotalEntryVIC = ucValue;

    return b3DPresent;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_InsertVCInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_InsertVCInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_BOOL bReceiveVC, MS_U8 ucVCNumber, MS_U8 ucVCFeatureID, MS_U8 ucVCSlotSize)
{
    MS_BOOL bFindVC = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucTargetVC = MHL_VIRTUAL_CHANNEL_LENGTH;
    MS_U8 ucTotalTimeSlot = 0;
    MS_U8 ucResponseCode = MHL_VC_RESPONSE_ACCEPT;

    if(ucVCFeatureID >= MHL_VC_FEATURE_ID_END) // Invalid feature ID
    {
        ucResponseCode = MHL_VC_RESPONSE_REJECT_FEATURE_ID;
    }
    else if((ucVCFeatureID == MHL_VC_FEATURE_ID_EMSC) && (ucVCSlotSize != MHL_EMSC_TIME_SLOT_DEFAULT))
    {
        ucResponseCode = MHL_VC_RESPONSE_REJECT_CHANNEL_SIZE;
    }

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(!bFindVC)
        {
            if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber == ucVCNumber) // VC present, information change
            {
                ucTargetVC = uctemp;

                bFindVC = TRUE;
            }
            else if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber == 0) // VC absent, insert new information
            {
                ucTargetVC = uctemp;

                pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCNumber = ucVCNumber;
                pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCFeatureID = ucVCFeatureID;

                break;
            }
        }

        ucTotalTimeSlot = ucTotalTimeSlot +pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize;
    }

    if(ucTargetVC < MHL_VIRTUAL_CHANNEL_LENGTH) // Can insert new VC information
    {
        pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].bVCConfirm = TRUE;

        if(pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCFeatureID != ucVCFeatureID) // Same VC number, but different feature ID
        {
            ucResponseCode = MHL_VC_RESPONSE_REJECT_NUMBER;
        }
        else if(ucTotalTimeSlot +ucVCSlotSize > MHL_ECBUS_S_TIME_SLOT_TOTAL -1) // Time slot not enough
        {
            ucResponseCode = MHL_VC_RESPONSE_REJECT_CHANNEL_SIZE;
        }
        else
        {
            if(bFindVC)
            {
                pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCResponseCode = pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCSlotSize;
            }

            pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCSlotSize = ucVCSlotSize;
        }

        if(bReceiveVC)
        {
            pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCResponseCode = ucResponseCode;
        }
    }
    else // Virtual channel length is too small
    {
        MDRV_MHL_MSG_INFO("** MHL virtual channel length too small port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckVCInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckVCInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucVCNumber, MS_U8 ucVCFeatureID, MS_U8 ucResponseCode)
{
    MS_U8 uctemp = 0;
    MS_U8 ucTargetVC = MHL_VIRTUAL_CHANNEL_LENGTH;

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber == ucVCNumber) // VC present, information change
        {
            ucTargetVC = uctemp;

            if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCFeatureID != ucVCFeatureID)
            {

            }

            break;
        }
    }

    if(ucTargetVC < MHL_VIRTUAL_CHANNEL_LENGTH) // Can insert new VC information
    {
        if(ucResponseCode == MHL_VC_RESPONSE_REJECT_CHANNEL_SIZE)
        {
            pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCSlotSize = pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCResponseCode;
        }
        else if(ucResponseCode == MHL_VC_RESPONSE_REJECT_NUMBER)
        {
            pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCSlotSize = 0;
        }
        else if(ucResponseCode == MHL_VC_RESPONSE_REJECT_FEATURE_ID)
        {
            pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCSlotSize = 0;
        }

        pMHLResourcePrivate->stVirtualChannelInfo[ucTargetVC].ucVCResponseCode = MHL_VC_RESPONSE_ACCEPT;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingEDIDfor3D()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingEDIDfor3D(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pEdid)
{
    MS_BOOL bDTDParseFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucValue = 0;
    MS_U8 ucIndex = 0;
    MS_U8 ucDTDTimingNum = 0;
    MS_U8 ucThirdDTDStart = 0;
    MS_U8 ucDTDTiming[MHL_3D_DTD_INFORMATION_LENGTH] = {0};

    if(pEdid != NULL)
    {
        // Parsing 3D DTD information Start
        if(_mdrv_mhl_DTDPresentinEDID(&pEdid[MHL_EDID_FIRST_DTD_START])) // Check first DTD
        {
            ucValue = _mdrv_mhl_InsertDTD3DInformation(pMHLResourcePrivate, MHL_3D_FIRST_DTD, pEdid[MHL_EDID_FIRST_DTD_START +MHL_EDID_DTD_LENGTH -1]);

            ucDTDTiming[MHL_3D_FIRST_DTD] = _mdrv_mhl_GetDTD3DTimingInformation(&pEdid[MHL_EDID_FIRST_DTD_START]);
        }
        else // First DTD absent
        {
            bDTDParseFinish = TRUE;

            ucDTDTimingNum = 0;

            MDRV_MHL_MSG_INFO("** MHL first DTD absent in port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
        }

        if(!bDTDParseFinish)
        {
            if(_mdrv_mhl_DTDPresentinEDID(&pEdid[MHL_EDID_SECOND_DTD_START])) // Check second DTD
            {
                ucValue = _mdrv_mhl_InsertDTD3DInformation(pMHLResourcePrivate, MHL_3D_SECOND_DTD, pEdid[MHL_EDID_SECOND_DTD_START +MHL_EDID_DTD_LENGTH -1]);

                ucDTDTiming[MHL_3D_SECOND_DTD] = _mdrv_mhl_GetDTD3DTimingInformation(&pEdid[MHL_EDID_SECOND_DTD_START]);
            }
            else    // Second DTD absent
            {
                bDTDParseFinish = TRUE;

                ucDTDTimingNum = 1;

                MDRV_MHL_MSG_INFO("** MHL second DTD absent in port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }
        }

        if(pEdid[MHL_EDID_EXTENSION_FLAG] > 0) // Have extend EDID
        {
            ucThirdDTDStart = MHL_EDID_BLOCK_LENGTH +pEdid[MHL_EDID_RESERVED_DATA_BLOCK];

            MDRV_MHL_MSG_INFO("** MHL third DTD start at %x in port %c\r\n", ucThirdDTDStart, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            if(!bDTDParseFinish)
            {
                for(uctemp = 0; uctemp <(MHL_3D_DTD_INFORMATION_LENGTH -2); uctemp++)
                {
                    if(_mdrv_mhl_DTDPresentinEDID(&pEdid[ucThirdDTDStart +MHL_EDID_DTD_LENGTH *uctemp])) // Check others DTD
                    {
                        ucValue = _mdrv_mhl_InsertDTD3DInformation(pMHLResourcePrivate, MHL_3D_THIRD_DTD +uctemp, pEdid[ucThirdDTDStart +MHL_EDID_DTD_LENGTH *(uctemp +1) -1]);

                        ucDTDTiming[MHL_3D_THIRD_DTD +uctemp] = _mdrv_mhl_GetDTD3DTimingInformation(&pEdid[ucThirdDTDStart +MHL_EDID_DTD_LENGTH *uctemp]);
                    }
                    else    // Last DTD find
                    {
                        bDTDParseFinish = TRUE;

                        ucDTDTimingNum = uctemp +2;

                        MDRV_MHL_MSG_INFO("** MHL total have %d DTD in port %c\r\n", ucDTDTimingNum, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                        break;
                    }
                }
            }

            // Parsing 3D VIC information Start
            if(ucThirdDTDStart > (MHL_EDID_RESERVED_DATA_BLOCK +2))
            {
                if(_mdrv_mhl_ParsingVIC3DInformation(pMHLResourcePrivate, pEdid[MHL_EDID_RESERVED_DATA_BLOCK] -4, &pEdid[MHL_EDID_RESERVED_DATA_BLOCK +2]))
                {
                    // 3D Present
                    for(uctemp = 0; uctemp <ucDTDTimingNum; uctemp++)
                    {
                        ucIndex = pMHLResourcePrivate->uc3DInformationDTD[uctemp];

                        switch(ucDTDTiming[uctemp])
                        {
                            case MHL_3D_PRESENT_VIC_1920x1080p24Hz: // Support frame packing with top and bottom
                            case MHL_3D_PRESENT_VIC_1280x720p60Hz: // Support frame packing with top and bottom
                            case MHL_3D_PRESENT_VIC_1280x720p50Hz: // Support frame packing with top and bottom
                                ucIndex = MASKSET(ucIndex, (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT), (MHL_3D_FS_SUPPORT |MHL_3D_TB_SUPPORT));
                                pMHLResourcePrivate->uc3DInformationDTD[uctemp] = ucIndex;

                                break;

                            case MHL_3D_PRESENT_VIC_1920x1080i60Hz: // Support side by side
                            case MHL_3D_PRESENT_VIC_1920x1080i50Hz: // Support side by side
                                ucIndex = MASKSET(ucIndex, MHL_3D_LR_SUPPORT, MHL_3D_LR_SUPPORT);
                                pMHLResourcePrivate->uc3DInformationDTD[uctemp] = ucIndex;

                                break;

                            default:

                                break;
                        };

                        if(ucIndex > 0)
                        {
                            ucValue = uctemp +1;
                        }
                    }
                }
            }
        }

        // Parsing 3D DTD information End
        pMHLResourcePrivate->uc3DTotalEntryDTD = ucValue;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SlotAllocationAlgorithm()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SlotAllocationAlgorithm(MS_U8 ucChannelIndex, MS_U8 ucNValue, MS_U8 ucMValue, MS_U8 *pSlotAllocation)
{
    MS_U8 uctemp = 0;
    MS_U8 ucCounter = 0;

    for(uctemp = 0; uctemp <MHL_ECBUS_S_TIME_SLOT_TOTAL; uctemp++)
    {
        if(pSlotAllocation[uctemp] == MHL_CBUS_SLOT_NOT_ALLOCATE)
        {
            if(ucCounter >= ucNValue)
            {
                pSlotAllocation[uctemp] = ucChannelIndex;
            }

            ucCounter = (ucCounter +ucNValue) %(ucNValue +ucMValue);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_eCbusTimeSlotAllocation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_eCbusTimeSlotAllocation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 uctemp = 0;
    MS_U8 ucTimeSlotNumber = MHL_ECBUS_S_TIME_SLOT_TOTAL -1;
    MS_U8 ucSlotAllocation[MHL_ECBUS_S_TIME_SLOT_TOTAL];
    MS_U8 ucVirtualChannel[MHL_VIRTUAL_CHANNEL_LENGTH +2];

    for(uctemp = 0; uctemp <(MHL_VIRTUAL_CHANNEL_LENGTH +2); uctemp++)
    {
        ucVirtualChannel[uctemp] = 0;
    }

    for(uctemp = 0; uctemp <MHL_ECBUS_S_TIME_SLOT_TOTAL; uctemp++)
    {
        if(uctemp == 0)
        {
            ucSlotAllocation[uctemp] = 0;
        }
        else
        {
            ucSlotAllocation[uctemp] = MHL_CBUS_SLOT_NOT_ALLOCATE;
        }
    }

    ucVirtualChannel[0] = MHL_CBUS1_TIME_SLOT_DEFAULT;

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize == 0)
        {
            break;
        }
        else
        {
            ucVirtualChannel[uctemp +1] = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize;
            ucTimeSlotNumber = ucTimeSlotNumber -pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize;
        }
    }

    ucVirtualChannel[uctemp +1] = ucTimeSlotNumber;

    for(uctemp = 0; uctemp <(MHL_VIRTUAL_CHANNEL_LENGTH +2); uctemp++)
    {
        MDRV_MHL_MSG_INFO("** MHL time slot assign %d\r\n", ucVirtualChannel[uctemp]);
    }

    ucTimeSlotNumber = MHL_ECBUS_S_TIME_SLOT_TOTAL -1;

    for(uctemp = MHL_VIRTUAL_CHANNEL_LENGTH +1; ucTimeSlotNumber > 0; uctemp--)
    {
        if(ucVirtualChannel[uctemp] > 0)
        {
            ucTimeSlotNumber = ucTimeSlotNumber -ucVirtualChannel[uctemp];

            _mdrv_mhl_SlotAllocationAlgorithm(uctemp, ucTimeSlotNumber, ucVirtualChannel[uctemp], ucSlotAllocation);
        }
    }

    for(uctemp = 0; uctemp <MHL_ECBUS_S_TIME_SLOT_TOTAL; uctemp++)
    {
        MDRV_MHL_MSG_INFO("** MHL time slot allocation %d\r\n", ucSlotAllocation[uctemp]);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SortingVCInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SortingVCInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_BOOL bClearFlag)
{
    MS_U8 uctemp = 0;
    MS_U8 ucIndex = 0;
    MS_U8 ucTargetVC = MHL_VIRTUAL_CHANNEL_LENGTH;
    stMHL_VIRTUAL_CHANNEL_INFO stVirtualChannelTemp = {FALSE, 0, 0, 0, 0};

    for(ucIndex = 0; ucIndex <(ucTargetVC -1); ucIndex++)
    {
        for(uctemp = 0; uctemp <(ucTargetVC -ucIndex -1); uctemp++)
        {
            if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCNumber == 0)
            {
                ucTargetVC = uctemp +1;

                break;
            }
            else
            {
                if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize == 0)
                {
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber = 0xFF;
                }

                if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCNumber < pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber)
                {
                    // temp = n +1
                    stVirtualChannelTemp.bVCConfirm = pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].bVCConfirm;
                    stVirtualChannelTemp.ucVCFeatureID = pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCFeatureID;
                    stVirtualChannelTemp.ucVCNumber = pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCNumber;
                    stVirtualChannelTemp.ucVCResponseCode = pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCResponseCode;
                    stVirtualChannelTemp.ucVCSlotSize = pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCSlotSize;
                    // n +1 = n
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].bVCConfirm = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].bVCConfirm;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCFeatureID = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCFeatureID;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCNumber = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCResponseCode = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCResponseCode;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp +1].ucVCSlotSize = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize;
                    // n = temp
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp].bVCConfirm = stVirtualChannelTemp.bVCConfirm;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCFeatureID = stVirtualChannelTemp.ucVCFeatureID;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber = stVirtualChannelTemp.ucVCNumber;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCResponseCode = stVirtualChannelTemp.ucVCResponseCode;
                    pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize = stVirtualChannelTemp.ucVCSlotSize;
                }
            }
        }
    }

    for(uctemp = 0; uctemp <MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize == 0)
        {
            pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber = 0;
        }

        if(bClearFlag)
        {
            pMHLResourcePrivate->stVirtualChannelInfo[uctemp].bVCConfirm = FALSE;
        }

        MDRV_MHL_MSG_INFO("** MHL VC sorting number %d\r\n", pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber);
        MDRV_MHL_MSG_INFO("** MHL VC sorting slot %d\r\n", pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendSetHPD()
//  [Description]:
//                  MHL Cbus set HPD state
//  [Arguments]:
//                  TRUE: SET_HPD
//                  FALSE: CLR_HPD
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendSetHPD(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_BOOL bSetHPDFlag)
{
    MS_U8 ucQueueIndex = _mdrv_mhl_GetFreeQueueIndex(pMHLResourcePrivate);

    if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
    {
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_PENDING;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucLength = 0;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucCommand = (bSetHPDFlag? MSC_SET_HPD : MSC_CLR_HPD);

        if(bSetHPDFlag)
        {
            SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG);
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG);
        }
        else
        {
            CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG);
            CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusWriteStatusOrSetInt(BYTE addr, BYTE val)
//  [Description]:
//                  MHL Cbus write status or set interrupt
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus message send successfully
//                  FALSE: Cbus message send fail
//**************************************************************************
MS_BOOL _mdrv_mhl_CbusWriteStatusOrSetInt(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucAddress, MS_U8 ucValue, MS_BOOL bExtended)
{
    MS_BOOL bInsertFlag = FALSE;
    MS_U8 ucQueueIndex = _mdrv_mhl_GetFreeQueueIndex(pMHLResourcePrivate);

    if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
    {
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_PENDING;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucLength = 2;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucCommand = (bExtended? MSC_WRITE_XSTAT : MSC_WRITE_STAT_OR_SET_INT);
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[0] = ucAddress;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[1] = ucValue;

        bInsertFlag = TRUE;
    }

    return bInsertFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendSubMessage
//  [Description]
//                  MHL Cbus MSC Send RCP or RAP
//  [Arguments]:
//                  subCmd: MSC subcommands
//                  databuf: command code
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL _mdrv_mhl_CbusSendSubMessage(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucSubCommand, MS_U8 ucKeyCode, MS_BOOL bWaitReply)
{
    MS_BOOL bInsertFlag = FALSE;
    MS_U8 ucQueueIndex = MHL_CBUS_TOTAL_QUEUE_NUM;

    if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_MSG_COMMAND_FLAG))
    {
        ucQueueIndex = _mdrv_mhl_GetFreeQueueIndex(pMHLResourcePrivate);

        if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
        {
            pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_PENDING;
            pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucLength = 2;
            pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucCommand = MSC_MSC_MSG;
            pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[0] = ucSubCommand;
            pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[1] = ucKeyCode;

            bInsertFlag = TRUE;
        }

        if(bWaitReply && bInsertFlag)
        {
            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_WAIT_MSG_RESPONSE, MHL_MSG_WAIT_RESPONSE_TIME);

            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_MSG_COMMAND_FLAG);
        }
    }

    return bInsertFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendGetVenderID()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus message send successfully
//                  FALSE: Cbus message send fail
//**************************************************************************
MS_BOOL _mdrv_mhl_CbusSendGetVenderID(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bInsertFlag = FALSE;
    MS_U8 ucQueueIndex = _mdrv_mhl_GetFreeQueueIndex(pMHLResourcePrivate);

    if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
    {
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_PENDING;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucLength = 0;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucCommand = MSC_GET_VENDOR_ID;

        bInsertFlag = TRUE;
    }

    return bInsertFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReadDeviceCapability()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus message send successfully
//                  FALSE: Cbus message send fail
//**************************************************************************
MS_BOOL _mdrv_mhl_CbusReadDeviceCapability(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucBitNumber, MS_BOOL bExtended)
{
    MS_BOOL bInsertFlag = FALSE;
    MS_U8 ucQueueIndex = _mdrv_mhl_GetFreeQueueIndex(pMHLResourcePrivate);

    if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
    {
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_PENDING;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucLength = 1;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucCommand = (bExtended? MSC_READ_XDEVCAP : MSC_READ_DEVCAP);
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[0] = (bExtended? (0x80 +ucBitNumber) : ucBitNumber);

        bInsertFlag = TRUE;
    }

    return bInsertFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendWriteBurst()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: Cbus message send successfully
//                  FALSE: Cbus message send fail
//**************************************************************************
MS_BOOL _mdrv_mhl_CbusSendWriteBurst(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucAddress, MS_U8 ucLength, MS_U8 *ucData)
{
    MS_BOOL bInsertFlag = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucQueueIndex = _mdrv_mhl_GetFreeQueueIndex(pMHLResourcePrivate);

    if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
    {
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_PENDING;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucLength = ucLength +1;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucCommand = MSC_WRITE_BURST;
        pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[0] = ucAddress;

        for(uctemp = 0; uctemp < ucLength; uctemp++)
        {
            pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucData[uctemp +1] = ucData[uctemp];
        }

        bInsertFlag = TRUE;
    }

    return bInsertFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendPathEnable()
//  [Description]:
//                  MHL Cbus set path enable
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendPathEnable(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_BOOL bEnableFlag)
{
    MS_BOOL bInsertFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x31, (bEnableFlag ? BIT(3) : 0), FALSE);

    if(bInsertFlag)
    {
        mhal_mhl_Cbus_SetPathEn(bEnableFlag);

        if(bEnableFlag)
        {
            SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_PATH_EN_FLAG);
        }
        else
        {
            CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_PATH_EN_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendDevcapReady()
//  [Description]:
//                  MHL Cbus set capbility ready
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendDevcapReady(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bInsertFlag = FALSE;
    MS_U8 ucDevcapValue = BIT(0);

    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
    {
        ucDevcapValue = ucDevcapValue| BIT(1);
    }

    bInsertFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x30, ucDevcapValue, FALSE);

    if(bInsertFlag)
    {
        SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_READY_FLAG);
    }
    else
    {
        CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_READY_FLAG);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendDevcapChange()
//  [Description]:
//                  MHL Cbus set capbility change
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendDevcapChange(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bInsertFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x20, BIT(0), FALSE);

    if(bInsertFlag)
    {
        SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_CHANGE_FLAG);
    }
    else
    {
        CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_CHANGE_FLAG);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendVersionCode()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendVersionCode(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bInsertFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x32, pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_VERSION], FALSE);

    if(bInsertFlag)
    {
        SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_VERSION_CODE_FLAG);
    }
    else
    {
        CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_VERSION_CODE_FLAG);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusSendLinkStatusNormal()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusSendLinkStatusNormal(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bInsertFlag = FALSE;

    if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_LINK_STATUS_NORMAL_FLAG))
    {
        bInsertFlag = _mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x91, 0x02, TRUE);

        if(bInsertFlag)
        {
            SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_LINK_STATUS_NORMAL_FLAG);
        }
        else
        {
            CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_LINK_STATUS_NORMAL_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetEnhanceCbusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetEnhanceCbusFlag(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bECbusSupport = GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_ECBUS_ENGINE_SUPPORT_FLAG);

    if(pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_VERSION] < 0x30)
    {
        bECbusSupport = FALSE;
    }

    return bECbusSupport;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusTimerEventProc()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusTimerEventProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_CBUS_SILENCE_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_CBUS_SILENCE_FLAG);
        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_SEND_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_CBUS_SILENCE);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_DISPLAY_COMMAND_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_DISPLAY_COMMAND_FLAG);
        SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_DISPLAY_COMMAND_ENABLE_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_DISPLAY_COMMAND);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_WAIT_MSG_RESPONSE_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_WAIT_MSG_RESPONSE_FLAG);
        CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_MSG_COMMAND_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_WAIT_MSG_RESPONSE);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_RCP_AUTO_RELEASE_FLAG))
    {
        if(_mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RCP, (pMHLResourcePrivate->ucPreRCPKeyCode |BIT(7)), TRUE))
        {
            CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_RCP_AUTO_RELEASE_FLAG);
            CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_RCP_RELEASE_ENABLE_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
            MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_RCP_AUTO_RELEASE);
#endif
        }
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_WAIT_WRITE_GRANT_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_WAIT_WRITE_GRANT_FLAG);

        if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
        {
            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_START;
        }

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_WAIT_WRITE_GRANT);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_CHECK_RECEIVE_LOSE_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_CHECK_RECEIVE_LOSE_FLAG);

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_RECEIVE_LOSE_FLAG))
        {
            CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_RECEIVE_LOSE_FLAG);

            if(pMHLResourcePrivate->ucReceiveCounter == 0)
            {
                pMHLResourcePrivate->ucReceiveCounter++;
            }
        }

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_CHECK_RECEIVE_LOSE);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_SEND_MSG_RESPONSE_FLAG))
    {
        if(_mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, pMHLResourcePrivate->ucMSGData[0], pMHLResourcePrivate->ucMSGData[1], FALSE))
        {
            CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_SEND_MSG_RESPONSE_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
            MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_SEND_MSG_RESPONSE);
#endif
        }
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_CHECK_CONTENT_ON_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_CHECK_CONTENT_ON_FLAG);
        CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CHECK_CONTENT_ON_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_CHECK_CONTENT_ON);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_AUTO_SWITCH_STOP_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_AUTO_SWITCH_STOP_FLAG);
        CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_STOP_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_AUTO_SWITCH_STOP);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_WAIT_ECBUS_LOCK_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_WAIT_ECBUS_LOCK_FLAG);

        if(pMHLResourcePrivate->ucECbusState != MHL_ECBUS_STATE_STABLE_POLLING)
        {
            if(!(pMHLResourcePrivate->ucBISTModeState == MHL_CBUS_BIST_ECBUS_SWITCH)) // Not BIST mode
            {
                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_UNLOCK_RESET;
            }
        }

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_WAIT_ECBUS_LOCK);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_AUTO_EQ_PROCESS_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_AUTO_EQ_PROCESS_FLAG);
        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_EQ_FUNCTION_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_AUTO_EQ_PROCESS);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_BIST_PROCESS_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_BIST_PROCESS_FLAG);

        if(pMHLResourcePrivate->ucBISTModeState == MHL_CBUS_BIST_ECBUS_SWITCH)
        {
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_EVENT_FINISH;
        }

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_BIST_PROCESS);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_TX_QUEUE_FREEZE_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_TX_QUEUE_FREEZE_FLAG);
        CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_TX_QUQUE_FREEZE);
#endif
    }

    if(GET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_POWER_ON_PATCH_FLAG))
    {
        CLR_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, MHL_TIMER_POWER_ON_PATCH_FLAG);

        if(pMHLResourcePrivate->ucPowerOnPatchState == MHL_POWER_ON_PATCH_STATE_WAIT_PATCH_ON)
        {
            pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_PATCH_ON;
        }
        else if(pMHLResourcePrivate->ucPowerOnPatchState == MHL_POWER_ON_PATCH_STATE_WAIT_PATCH_OFF)
        {
            pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_PATCH_OFF;
        }

#if(MHL_DEBUG_TIMER_EVENT)
        MDRV_MHL_MSG_INFO("** MHL timer event %d timeout\r\n", MHL_TIMER_EVENT_POWER_ON_PATCH);
#endif
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_TimerHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_TimerHandler(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucTimerEvent = 0;
    MS_U32 ulTimerInterval = 0;
    MS_U32 ulTimerTick = (MsOS_GetSystemTime()) %CBUS_TIMER_CONTROL_MASK;

    if(ulTimerTick != pMHLResourcePrivate->ulCurrentTimerTick)
    {
        ulTimerInterval = (ulTimerTick +CBUS_TIMER_CONTROL_MASK -pMHLResourcePrivate->ulCurrentTimerTick) %CBUS_TIMER_CONTROL_MASK;

        if(ulTimerInterval < 50)
        {
            for(ucTimerEvent = 0; ucTimerEvent < MHL_TIMER_EVENT_MASK; ucTimerEvent++)
            {
                if(pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].bEnableFlag)
                {
                    if(pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].usTimerTick > ulTimerInterval)
                    {
                        pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].usTimerTick = pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].usTimerTick -ulTimerInterval;
                    }
                    else
                    {
                        pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].usTimerTick = 0;

                        SET_MHL_TIMER_FLAG(pMHLResourcePrivate->ulTimerIndex, BIT(ucTimerEvent));

                        pMHLResourcePrivate->stTimerEventInfo[ucTimerEvent].bEnableFlag = FALSE;
                    }
                }
            }
        }

        pMHLResourcePrivate->ulCurrentTimerTick = ulTimerTick;
    }

    if(pMHLResourcePrivate->ulTimerIndex > 0)
    {
        _mdrv_mhl_CbusTimerEventProc(pMHLResourcePrivate);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ECbusStateHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ECbusStateHandler(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucECbusEvent = MHL_ECBUS_EVENT_NONE;
    MS_U8 ucECbusStatusFlag = mhal_mhl_GetECbusStatusFlag();

    if((pMHLResourcePrivate->stSignalStatusInfo.ucECbusTrainState < MHL_ECBUS_STATE_FAIL) && (pMHLResourcePrivate->stSignalStatusInfo.ucECbusTrainState >= MHL_ECBUS_STATE_SOURCE_ACTIVE))
    {
        ucECbusStatusFlag |= MHL_ECBUS_STATUS_TRAINING_PASS;
    }

    switch(pMHLResourcePrivate->ucECbusState)
    {
        case MHL_ECBUS_STATE_RECEIVE_MODE_UP:
            if(_mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RAPK, 0x00, FALSE))
            {
                // Freeze Tx queue, not allow any Cbus command insert Tx queue
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);

                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_TRIGGER_MODE_UP;

                ucECbusEvent = MHL_ECBUS_EVENT_PREPARE_MODE_UP;

                MDRV_MHL_MSG_INFO("** MHL receive mode up !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }

            break;

        case MHL_ECBUS_STATE_BIST_MODE_UP:
            // Freeze Tx queue, not allow any Cbus command insert Tx queue
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);

            pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_TRIGGER_MODE_UP;

            ucECbusEvent = MHL_ECBUS_EVENT_PREPARE_MODE_UP;

            MDRV_MHL_MSG_INFO("** MHL BIST trigger mode up !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            break;

        case MHL_ECBUS_STATE_TRIGGER_MODE_UP:
            // Wait Tx queue empty, then trigger eCbus mode up
            if(_mdrv_mhl_GetCbusQueueIndex(pMHLResourcePrivate) == MHL_CBUS_TOTAL_QUEUE_NUM)
            {
                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_PROCESS_MODE_UP;
                pMHLResourcePrivate->stSignalStatusInfo.bECbusEnableFlag = TRUE;

                ucECbusEvent = MHL_ECBUS_EVENT_MODE_UP_PROCESS;

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_WAIT_ECBUS_LOCK, MHL_WAIT_ECBUS_LOCK_TIME);

                MDRV_MHL_MSG_INFO("** MHL Tx queue empty trigger mode up !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }

            break;

        case MHL_ECBUS_STATE_PROCESS_MODE_UP:
            if(ucECbusStatusFlag &MHL_ECBUS_STATUS_SLOT_SYNC_DONE)
            {
                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_EQ_FUNCTION_FLAG);
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS1_FUNCTION_FLAG);

                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_MODE_UP_CLOCK_LOCK;

                MDRV_MHL_MSG_INFO("** MHL Cbus1 command enable !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }

            break;

        case MHL_ECBUS_STATE_MODE_UP_CLOCK_LOCK:
            if(ucECbusStatusFlag &MHL_ECBUS_STATUS_EMSC_SYNC_DONE)
            {
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_EMSC_FUNCTION_FLAG);

                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_STABLE_POLLING;

                MDRV_MHL_MSG_INFO("** MHL eMSC command enable !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG))
                {
                    CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG);
                }
            }

            break;

        case MHL_ECBUS_STATE_STABLE_POLLING:
            if((ucECbusStatusFlag &(MHL_ECBUS_STATUS_TRAINING_PASS)) == 0)
            {
                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_NONE;

                ucECbusEvent = MHL_ECBUS_EVENT_STUCK_TO_LOW;

                MDRV_MHL_MSG_INFO("** MHL eCbus lose lock to reset !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }
            else if(ucECbusStatusFlag &MHL_ECBUS_STATUS_RETRY_TIMEOUT)
            {
                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_NONE;

                ucECbusEvent = MHL_ECBUS_EVENT_STUCK_TO_LOW;
            }
            else
            {
                ucECbusEvent = MHL_ECBUS_EVENT_ECBUS_COMMAND_PARSING;
            }

            break;

        case MHL_ECBUS_STATE_UNLOCK_RESET:
            pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_NONE;

            ucECbusEvent = MHL_ECBUS_EVENT_STUCK_TO_LOW;

            MDRV_MHL_MSG_INFO("** MHL eCbus training timeout to reset !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            break;

        case MHL_ECBUS_STATE_RECEIVE_MODE_DOWN:
            if(_mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RAPK, 0x00, FALSE))
            {
                // Freeze Tx queue, not allow any Cbus command insert Tx queue
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);

                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_TRIGGER_MODE_DOWN;

                MDRV_MHL_MSG_INFO("** MHL receive mode down !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }

            break;

        case MHL_ECBUS_STATE_BIST_MODE_DOWN:
            // Freeze Tx queue, not allow any Cbus command insert Tx queue
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);

            pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_TRIGGER_MODE_DOWN;

            MDRV_MHL_MSG_INFO("** MHL BIST trigger mode down !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            break;

        case MHL_ECBUS_STATE_TRIGGER_MODE_DOWN:
            // Wait Tx queue empty, then trigger eCbus mode up
            if(_mdrv_mhl_GetCbusQueueIndex(pMHLResourcePrivate) == MHL_CBUS_TOTAL_QUEUE_NUM)
            {
                pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_PROCESS_MODE_DOWN;
                pMHLResourcePrivate->stSignalStatusInfo.bECbusEnableFlag = FALSE;

                ucECbusEvent = MHL_ECBUS_EVENT_MODE_DOWN_PROCESS;

                MDRV_MHL_MSG_INFO("** MHL Tx queue empty trigger mode down !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
            }

            break;

        default:

            break;
    };

    if(pMHLResourcePrivate->ucECbusTrainState != pMHLResourcePrivate->stSignalStatusInfo.ucECbusTrainState)
    {
        pMHLResourcePrivate->ucECbusTrainState = pMHLResourcePrivate->stSignalStatusInfo.ucECbusTrainState;

        MDRV_MHL_MSG_INFO("** MHL eCbus state %x\r\n", pMHLResourcePrivate->ucECbusTrainState);
    }

    mhal_mhl_ECbusEventProc(pMHLResourcePrivate->ucMHLSupportPath, ucECbusEvent, pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag, &(pMHLResourcePrivate->stBISTParameterInfo));
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_DisplayCommandCheckFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_DisplayCommandCheckFlag(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bCheckFlag = TRUE;
    MS_U16 usDisplayCheck = (pMHLResourcePrivate->usDisplayIndex) >> MHL_CBUS_DISPLAY_MASK_SHIFT;

    if(((usDisplayCheck &MHL_CBUS_DISPLAY_MASK) ^MHL_CBUS_DISPLAY_MASK) > 0)
    {
        bCheckFlag = FALSE;
    }

    return bCheckFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_PowerOnPatchProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_PowerOnPatchProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    switch(pMHLResourcePrivate->ucPowerOnPatchState)
    {
        case MHL_POWER_ON_PATCH_STATE_START:
            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_POWER_ON_PATCH, MHL_WAIT_PATCH_ON_TIME);

            pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_WAIT_PATCH_ON;

            break;

        case MHL_POWER_ON_PATCH_STATE_PATCH_ON:
            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_POWER_ON_PATCH, MHL_WAIT_PATCH_OFF_TIME);

            pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_WAIT_PATCH_OFF;

            mhal_mhl_ECbusEventProc(pMHLResourcePrivate->ucMHLSupportPath, MHL_ECBUS_EVENT_POWER_ON_PATCH_ON, pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag, &(pMHLResourcePrivate->stBISTParameterInfo));

            MDRV_MHL_MSG_INFO("** MHL power on patch on !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            break;

        case MHL_POWER_ON_PATCH_STATE_PATCH_OFF:
            pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_DONE;

            mhal_mhl_ECbusEventProc(pMHLResourcePrivate->ucMHLSupportPath, MHL_ECBUS_EVENT_POWER_ON_PATCH_OFF, pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag, &(pMHLResourcePrivate->stBISTParameterInfo));

            MDRV_MHL_MSG_INFO("** MHL power on patch off !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            break;

        case MHL_POWER_ON_PATCH_STATE_DONE:
            pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_NONE;
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckStatusPolling()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckStatusPolling(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bSourceChangeFlag = FALSE;
    MS_U8 ucCurrentHDMIPort = mhal_mhl_GetInputPort();

    // For cable detect
    if(mhal_mhl_CableDetect(pMHLResourcePrivate->ucMHLSupportPath)) // Cable plugged
    {
        if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
        {
            mhal_mhl_CablePlugProc(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stSignalStatusInfo));

            MDRV_MHL_MSG_INFO("** MHL Cable plugged!! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG);
        }

        if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_SOURCE_CHANGE_FLAG))
        {
            SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_SOURCE_CHANGE_FLAG);
        }

        // For Cbus
        if(mhal_mhl_CbusStatus() == 0x03) // Cbus connect
        {
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
            {
                mhal_mhl_CbusConnectProc(pMHLResourcePrivate->ucMHLSupportPath);

                pMHLResourcePrivate->ucTMDSClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_DISPLAY_COMMAND, MHL_CBUS_CONNECT_HOLD_TIME);

                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_DISPLAY_COMMAND_ENABLE_FLAG);

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_CBUS_SILENCE, MHL_CBUS_CONNECT_SILENCE_TIME);

                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_SEND_FLAG);

                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CHECK_CONTENT_ENABLE_FLAG))
                {
                    _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_CHECK_CONTENT_ON, MHL_CHECK_CONTENT_ON_TIME);

                    SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CHECK_CONTENT_ON_FLAG);
                    SET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_PATH_EN_FLAG);
                }

                MDRV_MHL_MSG_INFO("** MHL Cbus connected~~ ^Q^ port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
            }

            if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_ECBUS_FUNCTION_FLAG))
            {
                _mdrv_mhl_ECbusStateHandler(pMHLResourcePrivate);

#if(MHL3_AUTO_EQ_ENABLE)
                if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_EQ_FUNCTION_FLAG))
                {
                    CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_EQ_FUNCTION_FLAG);

                    mhal_mhl_AutoEQEventProc(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stSignalStatusInfo));

                    _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_AUTO_EQ_PROCESS, MHL_AUTO_EQ_PROCESS_TIME);
                }
#endif
            }
            else if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG))
            {
                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG);
            }

            mhal_mhl_CDRModeMonitor(pMHLResourcePrivate->ucMHLSupportPath, _mdrv_mhl_DisplayCommandCheckFlag(pMHLResourcePrivate), &(pMHLResourcePrivate->stSignalStatusInfo));
        }
        else // Cbus disconnection
        {
            if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
            {
                mhal_mhl_CbusStucktoLowProc(pMHLResourcePrivate->ucMHLSupportPath);

                mhal_mhl_CDRModeMonitor(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));

                _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_CONNECT_LOSS_FLAG);

                if(pMHLResourcePrivate->ucTMDSClockMode == MHL_PHY_CLOCK_PACKET_PIXEL)
                {
                    mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));

                    pMHLResourcePrivate->ucTMDSClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
                }

                MDRV_MHL_MSG_INFO("** MHL Cbus stuck to low @@ port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
            }
        }
    }
    else // Cable unplugged
    {
        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
        {
            mhal_mhl_CableRemoveProc(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stSignalStatusInfo));

            mhal_mhl_ECbusEventProc(pMHLResourcePrivate->ucMHLSupportPath, MHL_ECBUS_EVENT_STUCK_TO_LOW, pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag, &(pMHLResourcePrivate->stBISTParameterInfo));

            _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_CABLE_LOSS_FLAG);

            MDRV_MHL_MSG_INFO("** MHL Cable remove~~~ port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG);

            if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
            {
                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
            }
        }

        mhal_mhl_Accumulator_Clr(pMHLResourcePrivate->ucMHLSupportPath);
    }

    // For source change
    if(pMHLResourcePrivate->ucCurrentHDMIPort != ucCurrentHDMIPort)
    {
        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_SOURCE_CHANGE_FLAG))
        {
            if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
            {
                bSourceChangeFlag = TRUE;
            }
            else if(!mhal_mhl_CheckInputPort(pMHLResourcePrivate->ucMHLSupportPath))
            {
                bSourceChangeFlag = TRUE;
            }

            if(bSourceChangeFlag)
            {
                mhal_mhl_SourceChangeProc(pMHLResourcePrivate->ucMHLSupportPath, pMHLResourcePrivate->stSignalStatusInfo.ucMainLinkRate);

                if(pMHLResourcePrivate->ucTMDSClockMode == MHL_PHY_CLOCK_PACKET_PIXEL)
                {
                    mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, TRUE, &(pMHLResourcePrivate->stSignalStatusInfo));
                }
            }

            CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_SOURCE_CHANGE_FLAG);
        }

        pMHLResourcePrivate->ucCurrentHDMIPort = ucCurrentHDMIPort;
    }

    // Normal Cbus engine
    if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_NORMAL_FLAG))
    {
        mhal_mhl_CbusIsolate(pMHLResourcePrivate->ucMHLSupportPath, FALSE);
        mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_HW_DETECT);
        mhal_mhl_CbusFloating(FALSE);

        _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_CABLE_LOSS_FLAG);

        SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_NORMAL_FLAG);

        pMHLResourcePrivate->ucPowerOnPatchState = MHL_POWER_ON_PATCH_STATE_START;
    }

    _mdrv_mhl_PowerOnPatchProc(pMHLResourcePrivate);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetCbusReceiveMSC()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetCbusReceiveMSC(MS_U8 *ucReceiveMSC)
{
    MS_BOOL bGetMSCFlag = FALSE;
    MS_U8 uctemp = 0;
    MS_U16 usLength = 0;
    MS_U16 usValue = 0;

    usLength = (mhal_mhl_GetSRAMReceiveData()) &BMASK(6:0); // Get MSC command length

    if(usLength > 0)
    {
        ucReceiveMSC[0] = usLength -1;

#if(MHL_DEBUG_RECEIVE_MSC)
        MDRV_MHL_MSG_DEBUG(" MHL receive MSG length = %x\r\n", (usLength &BMASK(7:0)));
#endif

        usValue = mhal_mhl_GetSRAMReceiveData(); // Get MSC command

        if(usValue &BIT(8))
        {
            ucReceiveMSC[1] = usValue &BMASK(7:0);

#if(MHL_DEBUG_RECEIVE_MSC)
            MDRV_MHL_MSG_DEBUG(" MHL receive MSG command = %x\r\n", (usValue &BMASK(7:0)));
#endif
        }
        else
        {
            ucReceiveMSC[0] = 2;
            ucReceiveMSC[1] = MSC_ACK;
            ucReceiveMSC[2] = usValue &BMASK(7:0);

#if(MHL_DEBUG_RECEIVE_MSC)
            MDRV_MHL_MSG_DEBUG(" MHL receive MSG data = %x\r\n", (usValue &BMASK(7:0)));
#endif
        }

        for(uctemp = 0; uctemp < (usLength -2); uctemp++)
        {
            usValue = mhal_mhl_GetSRAMReceiveData(); // Get MSC data

            if((usValue &BIT(8)) == BIT(8)) // Command in data error happen
            {
                // For CTS 6.3.6.5 entry 4
                if((usValue &BMASK(7:0)) == MSC_ABORT)
                {
                    ucReceiveMSC[1] = usValue &BMASK(7:0);
                }
                else if((usValue &BMASK(7:0)) == MSC_EOF)
                {
                    // EOF is command but it is not error
                    ucReceiveMSC[uctemp +2] = usValue &BMASK(7:0);
                }
                else // Ignort command
                {
                    ucReceiveMSC[1] = MSC_NONE;
                }

#if(MHL_DEBUG_RECEIVE_MSC)
                MDRV_MHL_MSG_DEBUG(" MHL receive MSG command = %x\r\n", (usValue &BMASK(7:0)));
#endif
            }
            else
            {
                ucReceiveMSC[uctemp +2] = usValue &BMASK(7:0);

#if(MHL_DEBUG_RECEIVE_MSC)
                MDRV_MHL_MSG_DEBUG(" MHL receive MSG data = %x\r\n", (usValue &BMASK(7:0)));
#endif
            }
        }

        bGetMSCFlag = TRUE;
    }

    return bGetMSCFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SetReceiveCaseFlag()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SetReceiveCaseFlag(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *ucReceiveMSC)
{
    MS_U8 uctemp = 0;

    switch(ucReceiveMSC[1])
    {
        case MSC_ACK:
            SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ACK_FLAG);

            if(ucReceiveMSC[0] == 2) // Command length = 2
            {
                if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG))
                {
                    CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG);
                    SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG);

                    pMHLResourcePrivate->ucAskData = ucReceiveMSC[2];
                }
            }

            break;

        case MSC_ABORT:
            SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ABORT_FLAG);
            break;

        case MSC_MSC_MSG:
            SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSG_FLAG);

            pMHLResourcePrivate->ucMSGData[0] = ucReceiveMSC[2];
            pMHLResourcePrivate->ucMSGData[1] = ucReceiveMSC[3];
            pMHLResourcePrivate->usMSGKeyInfo = (MS_U16)(ucReceiveMSC[2] << 8)| ucReceiveMSC[3];

            break;

        case MSC_WRITE_STAT_OR_SET_INT:
            if(ucReceiveMSC[2] == 0x31) // Switch to PP mode or 24 bit mode
            {
                if((ucReceiveMSC[3] & BIT(3)) == BIT(3))
                {
                    //SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PATH_EN_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(4)) == BIT(4))
                {
                    //SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MUTED_FLAG);
                }

                ucReceiveMSC[3] &= BMASK(2:0);

                if(ucReceiveMSC[3] == MHL_PHY_CLOCK_PACKET_PIXEL)
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG);
                    CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG);
                }
                else if(ucReceiveMSC[3] == MHL_PHY_CLOCK_NORMAL_24BIT)
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG);
                    CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG);
                }
            }

            if(ucReceiveMSC[2] == 0x30)
            {
                if((ucReceiveMSC[3] & BIT(0)) == BIT(0)) // Devcap ready
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(1)) == BIT(1)) // Extended device capability support
                {
                    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
                    {
                        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG);
                    }
                }
            }

            if((ucReceiveMSC[2] == 0x32) && (ucReceiveMSC[3] >= 0x30)) // Source support MHL3
            {
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MHL3_VERSION_FLAG);
            }

            if(ucReceiveMSC[2] == 0x20)
            {
                if((ucReceiveMSC[3] & BIT(0)) == BIT(0)) // Devcap change
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG);

                    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
                    {
                        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG);
                    }
                }

                if((ucReceiveMSC[3] & BIT(1)) == BIT(1)) // Scratchpad register change
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DSCR_CHANGE_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(2)) == BIT(2)) // Burst write request
                {
                    //SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_REQUEST_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(3)) == BIT(3)) // Burst write grant
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_GRANT_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(4)) == BIT(4)) // 3D request
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_3D_REQ_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(5)) == BIT(5)) // Feature request
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_FEATURE_REQUEST_FLAG);
                }

                if((ucReceiveMSC[3] & BIT(6)) == BIT(6)) // Feature complete
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_FEATURE_COMPLETE_FLAG);
                }
            }

            break;

        case MSC_WRITE_XSTAT:
            if(ucReceiveMSC[2] == 0x90) // eCbus mode
            {
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ECBUS_MODE_FLAG);
            }

            if(ucReceiveMSC[2] == 0x92) // Link rate
            {
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_LINK_RATE_FLAG);

                pMHLResourcePrivate->stSignalStatusInfo.ucMainLinkRate = ucReceiveMSC[3];
            }

            break;

        case MSC_WRITE_BURST:
            SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_BURST_FLAG);

            pMHLResourcePrivate->ucWriteBurstAddress = ucReceiveMSC[2];
            pMHLResourcePrivate->ucWriteBurstLength = MHL_MSC_SCRATCHPAD_SIZE +1;

            for(uctemp = 0; uctemp < (MHL_MSC_SCRATCHPAD_SIZE +1); uctemp++)
            {
                if(uctemp < pMHLResourcePrivate->ucWriteBurstLength)
                {
                    if((ucReceiveMSC[uctemp +3] == MSC_EOF) && (uctemp >= (ucReceiveMSC[0] -3)))
                    {
                        pMHLResourcePrivate->ucWriteBurstInformation[uctemp] = 0;
                        pMHLResourcePrivate->ucWriteBurstLength = uctemp;
                    }
                    else
                    {
                        pMHLResourcePrivate->ucWriteBurstInformation[uctemp] = ucReceiveMSC[uctemp +3];
                    }
                }
                else if(uctemp < MHL_MSC_SCRATCHPAD_SIZE)
                {
                    pMHLResourcePrivate->ucWriteBurstInformation[uctemp] = 0;
                }
            }

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusDisplayLinkCheck()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusDisplayLinkCheck(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucDisplayIndex = (pMHLResourcePrivate->usDisplayIndex) &BMASK(7:0);
    MS_U8 ucDisplayMask = MHL_CBUS_DISPLAY_MASK;
    MS_U16 usDisplayCheck = (pMHLResourcePrivate->usDisplayIndex) >> MHL_CBUS_DISPLAY_MASK_SHIFT;

    if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_ECBUS_FUNCTION_FLAG))
    {
        ucDisplayMask = ucDisplayMask |MHL_ECBUS_DISPLAY_MASK;
    }

    if((ucDisplayIndex ^ usDisplayCheck) > 0)
    {
        usDisplayCheck = usDisplayCheck ^ ucDisplayMask;

        if(usDisplayCheck > 0)
        {
            pMHLResourcePrivate->usDisplayIndex |= (_mdrv_mhl_CbusGetLeastFlag(usDisplayCheck) << MHL_CBUS_DISPLAY_MASK_SHIFT);

            MDRV_MHL_MSG_INFO("** MHL Cbus Receive ACK for Link\r\n");
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstVCAssign()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_ParsingWriteBurstVCAssign(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bParsingFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstInformation[4];
    MS_U8 ucNumberEntry = pMHLResourcePrivate->ucWriteBurstInformation[5];

    if(ucNumberEntry +(ucSequence -1) *MHL_VC_SEQUENCE_LENGTH == pMHLResourcePrivate->ucWriteBurstInformation[3]) // Number entry + (sequece -1) x length = total entry
    {
        bParsingFinish = TRUE;
    }

    for(uctemp = 0; uctemp <ucNumberEntry; uctemp++)
    {
        _mdrv_mhl_InsertVCInformation(pMHLResourcePrivate, TRUE, pMHLResourcePrivate->ucWriteBurstInformation[uctemp *MHL_VC_SEQUENCE_LENGTH +3], pMHLResourcePrivate->ucWriteBurstInformation[uctemp *MHL_VC_SEQUENCE_LENGTH +4], pMHLResourcePrivate->ucWriteBurstInformation[uctemp *MHL_VC_SEQUENCE_LENGTH +5]);
    }

    return bParsingFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstVCConfirm()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstVCConfirm(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bParsingFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstInformation[4];
    MS_U8 ucNumberEntry = pMHLResourcePrivate->ucWriteBurstInformation[5];

    if(ucNumberEntry +(ucSequence -1) *MHL_VC_SEQUENCE_LENGTH == pMHLResourcePrivate->ucWriteBurstInformation[3]) // Number entry + (sequece -1) x length = total entry
    {
        bParsingFinish = TRUE;
    }

    for(uctemp = 0; uctemp <ucNumberEntry; uctemp++)
    {
        _mdrv_mhl_CheckVCInformation(pMHLResourcePrivate, pMHLResourcePrivate->ucWriteBurstInformation[uctemp *MHL_VC_SEQUENCE_LENGTH +3], pMHLResourcePrivate->ucWriteBurstInformation[uctemp *MHL_VC_SEQUENCE_LENGTH +4], pMHLResourcePrivate->ucWriteBurstInformation[uctemp *MHL_VC_SEQUENCE_LENGTH +5]);
    }

    if(bParsingFinish)
    {
        _mdrv_mhl_SortingVCInformation(pMHLResourcePrivate, TRUE);

        _mdrv_mhl_eCbusTimeSlotAllocation(pMHLResourcePrivate);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckAVLineVideoMode()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_CheckAVLineVideoMode(MS_U8 ucAVLinkVideoMode)
{
    MS_U8 ucFrameRate = 1;

    switch(ucAVLinkVideoMode)
    {
        case MHL_VIC_INDEX1:
        case MHL_VIC_INDEX2:
        case MHL_VIC_INDEX3:
        case MHL_VIC_INDEX4:
        case MHL_VIC_INDEX5:
        case MHL_VIC_INDEX6:
        case MHL_VIC_INDEX7:
        case MHL_VIC_INDEX16:
            ucFrameRate = 60;
            break;

        case MHL_VIC_INDEX17:
        case MHL_VIC_INDEX18:
        case MHL_VIC_INDEX19:
        case MHL_VIC_INDEX20:
        case MHL_VIC_INDEX21:
        case MHL_VIC_INDEX22:
        case MHL_VIC_INDEX31:
            ucFrameRate = 50;
            break;

        case MHL_VIC_INDEX34:
        case MHL_VIC_INDEX95:
        case MHL_VIC_INDEX100:
            ucFrameRate = 30;
            break;

        case MHL_VIC_INDEX63:
            ucFrameRate = 120;
            break;

        case MHL_VIC_INDEX64:
            ucFrameRate = 100;
            break;

        case MHL_VIC_INDEX93:
        case MHL_VIC_INDEX98:
            ucFrameRate = 24;
            break;

        case MHL_VIC_INDEX94:
        case MHL_VIC_INDEX99:
            ucFrameRate = 25;
            break;

        default:

            break;
    };

    return ucFrameRate;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstBISTSetup()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstBISTSetup(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucReadyIndex = 0x70;
    MS_U8 ucBISTeCbusPattern = pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_ECBUS_SPEEDS];
    MS_U8 ucBISTAVLinkRate = pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_TMDS_SPEEDS];
    MS_U16 usBISTeCbusPatternFixed = 0;
    MS_U16 usBISTAVLinkFixed = 0;
    MS_U16 usBISTeCbusDuration = 0;
    MS_U16 usBISTAVLinkDuration = 0;

    // ucWriteBurstInformation[0]: BIST_SETUP_H
    // ucWriteBurstInformation[1]: BIST_SETUP_L
    // ucWriteBurstInformation[2]: CHECK_SUM
    // ucWriteBurstInformation[3]: eCBUS_DURATION
    // ucWriteBurstInformation[4]: eCBUS_PATTERN
    // ucWriteBurstInformation[5]: eCBUS_FIXED_H
    // ucWriteBurstInformation[6]: eCBUS_FIXED_L
    // ucWriteBurstInformation[7]: Reserved
    // ucWriteBurstInformation[8]: AV LINK_DATA_RATE
    // ucWriteBurstInformation[9]: AV LINK_PATTERN
    // ucWriteBurstInformation[10]: AV LINK_VIDEO_MODE
    // ucWriteBurstInformation[11]: AV LINK_DURATION
    // ucWriteBurstInformation[12]: AV LINK_FIXED_H
    // ucWriteBurstInformation[13]: AV LINK_FIXED_L
    // ucWriteBurstInformation[14]: AV LINK_RANDOMIZER
    // ucWriteBurstInformation[15]: IMPEDANCE_MODE

    if((ucBISTeCbusPattern &BIT(4)) == BIT(4)) // MHL_ECBUS_D_150
    {
        ucBISTeCbusPattern = MHL_BIST_ECBUS_PATTERN_FIXED10;
    }
    else if((ucBISTeCbusPattern &BIT(0)) == BIT(0)) // MHL_ECBUS_S_075
    {
        ucBISTeCbusPattern = MHL_BIST_ECBUS_PATTERN_FIXED8;
    }
    else
    {
        ucBISTeCbusPattern = MHL_BIST_ECBUS_PATTERN_PRBS;
    }

    if((ucBISTAVLinkRate & BIT(2)) == BIT(2)) // MHL_TMDS_600
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_600;
    }
    else if((ucBISTAVLinkRate & BIT(1)) == BIT(1)) // MHL_TMDS_300
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_300;
    }
    else if((ucBISTAVLinkRate & BIT(0)) == BIT(0)) // MHL_TMDS_150
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_150;
    }
    else
    {
        ucBISTAVLinkRate = MHL_BIST_AV_LINK_RESERVED;
    }

    if(pMHLResourcePrivate->ucWriteBurstInformation[4] <= ucBISTeCbusPattern) // Check eCbus pattern
    {
       ucReadyIndex = MASKSET(ucReadyIndex, BIT(0), (BIT(0) |BIT(4)));

       ucBISTeCbusPattern = pMHLResourcePrivate->ucWriteBurstInformation[4];
       usBISTeCbusPatternFixed = ((MS_U16)pMHLResourcePrivate->ucWriteBurstInformation[5] <<8) |pMHLResourcePrivate->ucWriteBurstInformation[6];
    }

    if(pMHLResourcePrivate->ucWriteBurstInformation[8] <= ucBISTAVLinkRate) // Check AV link rate
    {
        ucReadyIndex = MASKSET(ucReadyIndex, BIT(1), (BIT(1) |BIT(5)));

        ucBISTAVLinkRate = pMHLResourcePrivate->ucWriteBurstInformation[8];
        usBISTAVLinkFixed = ((MS_U16)pMHLResourcePrivate->ucWriteBurstInformation[12] <<8) |pMHLResourcePrivate->ucWriteBurstInformation[13];
    }

    //if() // Unknow condition
    {
        ucReadyIndex = MASKSET(ucReadyIndex, BIT(2), (BIT(2) |BIT(6)));
    }

    if(pMHLResourcePrivate->ucWriteBurstInformation[3] == 0) // Check eCbus duration
    {
        // Can't allow
    }
    else
    {
        usBISTeCbusDuration = (MS_U16)pMHLResourcePrivate->ucWriteBurstInformation[3] *1010ul;
    }

    if(pMHLResourcePrivate->ucWriteBurstInformation[9] <= MHL_BIST_LINK_PATTERN_FIXED8)
    {
        usBISTAVLinkDuration = ((MS_U16)pMHLResourcePrivate->ucWriteBurstInformation[11] *32 /_mdrv_mhl_CheckAVLineVideoMode(pMHLResourcePrivate->ucWriteBurstInformation[10]) +1) *1000;
    }
    else if(pMHLResourcePrivate->ucWriteBurstInformation[9] == MHL_BIST_LINK_PATTERN_FIXED10)
    {
        usBISTAVLinkDuration = (MS_U16)pMHLResourcePrivate->ucWriteBurstInformation[11] *1010ul;
    }

    pMHLResourcePrivate->stBISTParameterInfo.ucBISTReadyIndex = ucReadyIndex;
    pMHLResourcePrivate->stBISTParameterInfo.ucBISTeCbusPattern = ucBISTeCbusPattern;
    pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusPatternFixed = usBISTeCbusPatternFixed;
    pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration = usBISTeCbusDuration;
    pMHLResourcePrivate->stBISTParameterInfo.ucBISTAVLinkRate = ucBISTAVLinkRate;
    pMHLResourcePrivate->stBISTParameterInfo.ucBISTAVLinkPattern = pMHLResourcePrivate->ucWriteBurstInformation[9];
    pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkFixed = usBISTAVLinkFixed;
    pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkDuration = usBISTAVLinkDuration;
    pMHLResourcePrivate->stBISTParameterInfo.ucBISTAVLinkRateRandomizer = pMHLResourcePrivate->ucWriteBurstInformation[14];
    pMHLResourcePrivate->stBISTParameterInfo.ucBISTImpedanceMode = pMHLResourcePrivate->ucWriteBurstInformation[15];

#if(MHL_DEBUG_BIST_MODE)
    MDRV_MHL_MSG_INFO("** MHL BIST eCbus pattern = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.ucBISTeCbusPattern);
    MDRV_MHL_MSG_INFO("** MHL BIST eCbus fixed = %x\r\n", pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusPatternFixed);
    MDRV_MHL_MSG_INFO("** MHL BIST eCbus duration = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration);
    MDRV_MHL_MSG_INFO("** MHL BIST AV link rate = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.ucBISTAVLinkRate);
    MDRV_MHL_MSG_INFO("** MHL BIST AV link pattern = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.ucBISTAVLinkPattern);
    MDRV_MHL_MSG_INFO("** MHL BIST AV link fixed = %x\r\n", pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkFixed);
    MDRV_MHL_MSG_INFO("** MHL BIST AV link duration = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkDuration);
    MDRV_MHL_MSG_INFO("** MHL BIST AV link random = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.ucBISTAVLinkRateRandomizer);
    MDRV_MHL_MSG_INFO("** MHL BIST Impedance mode = %d\r\n", pMHLResourcePrivate->stBISTParameterInfo.ucBISTImpedanceMode);
#endif
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingWriteBurstInformation()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingWriteBurstInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 uctemp = 0;
    MS_U8 ucCheckSum = 0;
    MS_U16 usWriteBurstID = ((MS_U16)pMHLResourcePrivate->ucWriteBurstInformation[0] <<8) |pMHLResourcePrivate->ucWriteBurstInformation[1];

    for(uctemp = 0; uctemp <MHL_MSC_SCRATCHPAD_SIZE; uctemp++)
    {
        ucCheckSum = ucCheckSum +pMHLResourcePrivate->ucWriteBurstInformation[uctemp];
    }

    if(ucCheckSum == 0)
    {
        switch(usWriteBurstID)
        {
            case MHL_BURST_ID_VC_ASSIGN:
                if(_mdrv_mhl_ParsingWriteBurstVCAssign(pMHLResourcePrivate))
                {
                    SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_VC_ASSIGN_FLAG);
                }
                break;

            case MHL_BURST_ID_VC_CONFIRM:
                _mdrv_mhl_ParsingWriteBurstVCConfirm(pMHLResourcePrivate);
                break;

            case MHL_BURST_ID_BIST_SETUP:
                pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_RECEIVE_SETUP;

                _mdrv_mhl_ParsingWriteBurstBISTSetup(pMHLResourcePrivate);

                break;

            default:

                break;
        };
    }
    else
    {
        MDRV_MHL_MSG_INFO("** MHL write burst check sum error port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTTriggerProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTTriggerProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucKeyCode)
{
    MS_BOOL bGeteCbusDuration = FALSE;
    MS_BOOL bGetAVLinkDuration = FALSE;
    MS_U8 ucBISTDeviceType = pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_DEV_CAT] &BMASK(3:0);
    MS_U8 ucBISTeCbusSelect = pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_ECBUS_SPEEDS];
    MS_U16 usTimerCount = 0;

    if(((ucKeyCode &BIT(3)) == BIT(3)) && ((ucBISTeCbusSelect &BIT(4)) != BIT(4))) // Not support eCbus-D
    {
        // Chip only support eCbus-S
    }

    if((ucKeyCode &BIT(0)) == BIT(0)) // Source device and eCbus Tx
    {
        bGeteCbusDuration = TRUE;

        pMHLResourcePrivate->stBISTParameterInfo.bBISTeCbusTxFlag = TRUE;
    }
    else
    {
        pMHLResourcePrivate->stBISTParameterInfo.bBISTeCbusTxFlag = FALSE;
    }

    if((ucKeyCode &BIT(1)) == BIT(1)) // Sink device and eCbus link Rx
    {
        bGeteCbusDuration = TRUE;

        pMHLResourcePrivate->stBISTParameterInfo.bBISTeCbusRxFlag = TRUE;
    }
    else
    {
        pMHLResourcePrivate->stBISTParameterInfo.bBISTeCbusRxFlag = FALSE;
    }

    if((ucBISTDeviceType == MHL_DEVICE_SOURCE) || (ucBISTDeviceType == MHL_DEVICE_DIRECT_ATTACH_SOURCE))
    {
        if((ucKeyCode &BIT(4)) == BIT(4)) // Source device and AV link Tx
        {

        }
    }
    else if((ucBISTDeviceType == MHL_DEVICE_SINK) || (ucBISTDeviceType == MHL_DEVICE_DIRECT_ATTACH_SINK))
    {
        if((ucKeyCode &BIT(6)) == BIT(6)) // Impedance test
        {
            bGeteCbusDuration = TRUE;
        }

        if((ucKeyCode &BIT(5)) == BIT(5)) // Sink device and AV link Rx
        {
            bGetAVLinkDuration = TRUE;
        }
    }

    if(pMHLResourcePrivate->ucBISTModeState == MHL_CBUS_BIST_WAIT_TRIGGER)
    {
        if(bGeteCbusDuration)
        {
            usTimerCount = pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration;
        }

        if(bGetAVLinkDuration)
        {
            if(usTimerCount < pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkDuration)
            {
                usTimerCount = pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkDuration;
            }
        }

        if(bGeteCbusDuration || bGetAVLinkDuration)
        {
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_RECEIVE_TRIGGER;
            pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration = usTimerCount;
        }
    }

    MDRV_MHL_MSG_INFO("** MHL receive BIST trigger in port %c keycode %x\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTRequestStatusProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTRequestStatusProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucKeyCode)
{
    //if(ucKeyCode == MHL_BIST_REQ_RETURN_STATUS)
    {
        pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_REQUEST_STATUS;
    }

    MDRV_MHL_MSG_INFO("** MHL receive BIST request status in port %c keycode %x\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusBISTStopProc()
//  [Description]
//                  Disable AV LINK_BIST when currently running
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTStopProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucKeyCode)
{
    if(ucKeyCode == 0) // Key code must be set to 0
    {
        if(pMHLResourcePrivate->ucBISTModeState == MHL_CBUS_BIST_ECBUS_SWITCH)
        {
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_EVENT_FINISH;
        }
    }

    MDRV_MHL_MSG_INFO("** MHL receive BIST stop in port %c keycode %x\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucKeyCode);
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusReceiveEventProc()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusReceiveEventProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucQueueIndex = 0;
    MS_U16 usTimeTick = MHL_CBUS_IDEL_FREE_TIME;

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSC_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSC_FLAG);

        if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ABORT_FLAG))
        {
            CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ABORT_FLAG);

            usTimeTick = MHL_CBUS_ABORT_HOLD_TIME;
        }

        _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_CBUS_SILENCE, usTimeTick);
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ACK_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ACK_FLAG);

        if(pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucQueueState != MHL_QUEUE_STATE_IDLE)
        {
            pMHLResourcePrivate->stCbusQueueInfo[MHL_CBUS_RX_QUEUE_INDEX].ucQueueState = MHL_QUEUE_STATE_IDLE;
        }
        else if(pMHLResourcePrivate->stCbusQueueInfo[pMHLResourcePrivate->ucCurrentQueueIndex].ucQueueState != MHL_QUEUE_STATE_PENDING)
        {
            pMHLResourcePrivate->stCbusQueueInfo[pMHLResourcePrivate->ucCurrentQueueIndex].ucQueueState = MHL_QUEUE_STATE_IDLE;
        }

        ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(pMHLResourcePrivate);

        // Update current index to search index, but not Rx queue index
        if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
        {
            pMHLResourcePrivate->ucCurrentQueueIndex = ucQueueIndex;
        }

        _mdrv_mhl_CbusDisplayLinkCheck(pMHLResourcePrivate);
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG);

        if(pMHLResourcePrivate->ucTMDSClockMode == MHL_PHY_CLOCK_PACKET_PIXEL)
        {
            mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));

            MDRV_MHL_MSG_INFO("** MHL switch back to 24 bits mode port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            pMHLResourcePrivate->ucTMDSClockMode = MHL_PHY_CLOCK_NORMAL_24BIT;
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG);

        if(pMHLResourcePrivate->ucTMDSClockMode == MHL_PHY_CLOCK_NORMAL_24BIT)
        {
            mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, TRUE, &(pMHLResourcePrivate->stSignalStatusInfo));

            MDRV_MHL_MSG_INFO("** MHL switch to packet pixel mode port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            pMHLResourcePrivate->ucTMDSClockMode = MHL_PHY_CLOCK_PACKET_PIXEL;
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MHL3_VERSION_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MHL3_VERSION_FLAG);

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
        {
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_ECBUS_FUNCTION_FLAG);
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_GRANT_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_GRANT_FLAG);

        if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_SEND_REQUEST)
        {
            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_REVEIVE_GRANT;
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_3D_REQ_FLAG))
    {
        if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_INFORMATION_FLAG))
        {
            CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_3D_REQ_FLAG);
        }
        else if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_NONE)
        {
            CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_3D_REQ_FLAG);
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_INFORMATION_FLAG);
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_DTD_FLAG);
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_VIC_FLAG);
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DSCR_CHANGE_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DSCR_CHANGE_FLAG);

        pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_END;
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_FEATURE_REQUEST_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_FEATURE_REQUEST_FLAG);

        if(pMHLResourcePrivate->ucFeatureRequestState == MHL_CBUS_FEATURE_NONE)
        {
            pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_START;
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_FEATURE_COMPLETE_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_FEATURE_COMPLETE_FLAG);

        if(pMHLResourcePrivate->ucFeatureRequestState == MHL_CBUS_FEATURE_SEND_FEATURE_REQUEST)
        {
            pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_END;
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_LINK_RATE_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_LINK_RATE_FLAG);
        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_LINK_STATUS_NORMAL_FLAG);

        if(pMHLResourcePrivate->stSignalStatusInfo.ucMainLinkRate < MHL_AV_LINK_NONE)
        {
            mhal_mhl_SetMainLinkRate(pMHLResourcePrivate->ucMHLSupportPath, pMHLResourcePrivate->stSignalStatusInfo.ucMainLinkRate);

            mhal_mhl_CDRModeMonitor(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));

            MDRV_MHL_MSG_INFO("** MHL set AV link rate mode %d port %c\r\n", pMHLResourcePrivate->stSignalStatusInfo.ucMainLinkRate, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
        }
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_BURST_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WRITE_BURST_FLAG);

        _mdrv_mhl_ParsingWriteBurstInformation(pMHLResourcePrivate);
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_VC_ASSIGN_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_VC_ASSIGN_FLAG);
        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_INFORMATION_FLAG);
        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_CONFIRM_FLAG);
    }

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ECBUS_MODE_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_ECBUS_MODE_FLAG);

        if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS1_FUNCTION_FLAG))
        {
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);

            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_TX_QUQUE_FREEZE, MHL_CBUS_1_SECOND_TIME);

            MDRV_MHL_MSG_INFO("** MHL CTS 6.3.9.1 patch port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusMSGEventProc()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusMSGEventProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bReceiveResponse = FALSE;
    MS_BOOL bResponseErrorFlag = FALSE;
    MS_U8 ucStatus = MHL_RCP_NO_ERROR;
    MS_U8 ucSubCommand = 0;
    MS_U8 ucKeyCode = MHL_RCP_NO_ERROR;

    if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSG_FLAG))
    {
        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSG_FLAG);

        switch(pMHLResourcePrivate->ucMSGData[0])
        {
            case MSC_MSG_MSGE:

                break;

            case MSC_MSG_RCP:
                if(!GET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RCP_FUNCTION_FLAG))
                {
                    ucStatus = MHL_RCP_NO_ERROR;

                    MDRV_MHL_MSG_WARNING("** MHL RCP call back NULL port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                }
                else if(!(pMHLResourcePrivate->ucMSGData[1] &BIT(7))) // Ignore release key
                {
                    SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);
                }

                if(ucStatus == MHL_RCP_NO_ERROR)
                {
                    ucSubCommand = MSC_MSG_RCPK;
                    ucKeyCode = pMHLResourcePrivate->ucMSGData[1];
                }
                else
                {
                    ucSubCommand = MSC_MSG_RCPE;
                    ucKeyCode = ucStatus;

                    pMHLResourcePrivate->ucMSGData[0] = MSC_MSG_RCPK;
                    bResponseErrorFlag = TRUE;
                }

                _mdrv_mhl_CbusRxQueueInsertMSG(pMHLResourcePrivate, ucSubCommand, ucKeyCode);

                break;

            case MSC_MSG_RCPK:
            case MSC_MSG_RCPE:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_RAP:
                if(!GET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RAP_FUNCTION_FLAG))
                {
                    ucStatus = MHL_RAP_NO_ERROR;

                    MDRV_MHL_MSG_WARNING("** MHL RAP call back NULL port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                }
                else if(_mdrv_mhl_CheckCbusModeChange(pMHLResourcePrivate))
                {
                    if(_mdrv_mhl_GetRAPEnableFlag(pMHLResourcePrivate))
                    {
                        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);
                    }

                    ucSubCommand = MSC_MSG_RAPK;
                    ucKeyCode = ucStatus;

                    _mdrv_mhl_CbusRxQueueInsertMSG(pMHLResourcePrivate, ucSubCommand, ucKeyCode);
                }
                else if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG))
                {
                    CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG);
                }

                break;

            case MSC_MSG_RAPK:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_UCP:
                if(!GET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_UCP_FUNCTION_FLAG))
                {
                    ucStatus = MHL_UCP_NO_ERROR;

                    MDRV_MHL_MSG_WARNING("** MHL UCP call back NULL port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                }
                else
                {
                    SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);
                }

                if(ucStatus == MHL_UCP_NO_ERROR)
                {
                    ucSubCommand = MSC_MSG_UCPK;
                    ucKeyCode = pMHLResourcePrivate->ucMSGData[1];
                }
                else
                {
                    ucSubCommand = MSC_MSG_UCPE;
                    ucKeyCode = ucStatus;

                    pMHLResourcePrivate->ucMSGData[0] = MSC_MSG_UCPK;
                    bResponseErrorFlag = TRUE;
                }

                _mdrv_mhl_CbusRxQueueInsertMSG(pMHLResourcePrivate, ucSubCommand, ucKeyCode);

                break;

            case MSC_MSG_UCPK:
            case MSC_MSG_UCPE:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_ATT:
                if(!GET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_ATT_FUNCTION_FLAG))
                {
                    ucStatus = MHL_ATT_NO_ERROR;

                    MDRV_MHL_MSG_WARNING("** MHL ATT call back NULL port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                }
                else
                {
                    SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);
                }

                ucSubCommand = MSC_MSG_ATTK;
                ucKeyCode = ucStatus;

                _mdrv_mhl_CbusRxQueueInsertMSG(pMHLResourcePrivate, ucSubCommand, ucKeyCode);

                break;

            case MSC_MSG_ATTK:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_RBP:
                if(!GET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RBP_FUNCTION_FLAG))
                {
                    ucStatus = MHL_RBP_NO_ERROR;

                    MDRV_MHL_MSG_WARNING("** MHL RBP call back NULL port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
                }
                else
                {
                    SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);
                }

                if(ucStatus == MHL_RBP_NO_ERROR)
                {
                    ucSubCommand = MSC_MSG_RBPK;
                    ucKeyCode = pMHLResourcePrivate->ucMSGData[1];
                }
                else
                {
                    ucSubCommand = MSC_MSG_RBPE;
                    ucKeyCode = ucStatus;

                    pMHLResourcePrivate->ucMSGData[0] = MSC_MSG_RBPK;
                    bResponseErrorFlag = TRUE;
                }

                _mdrv_mhl_CbusRxQueueInsertMSG(pMHLResourcePrivate, ucSubCommand, ucKeyCode);

                break;

            case MSC_MSG_RBPK:
            case MSC_MSG_RBPE:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_RUSB:

                break;

            case MSC_MSG_RUSBK:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_RHID:

                break;

            case MSC_MSG_RHIDK:
                bReceiveResponse = TRUE;
                break;

            case MSC_MSG_BIST_TRIGGRT:
                _mdrv_mhl_CbusBISTTriggerProc(pMHLResourcePrivate, pMHLResourcePrivate->ucMSGData[1]);
                break;

            case MSC_MSG_BIST_REQUEST_STAT:
                _mdrv_mhl_CbusBISTRequestStatusProc(pMHLResourcePrivate, pMHLResourcePrivate->ucMSGData[1]);
                break;

            case MSC_MSG_BIST_STOP:
                _mdrv_mhl_CbusBISTStopProc(pMHLResourcePrivate, pMHLResourcePrivate->ucMSGData[1]);
                break;

            case MSC_MSG_SI_UNKNOWN:
                _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_SI_UNKNOWNK, 0, FALSE);
                break;

            default:
                if(pMHLResourcePrivate->ucMSGData[0] < 0x20) // For CTS 6.3.11.23
                {
                    _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_MSGE, MHL_MSGE_INVALID_SUB_COMMAND_CODE, FALSE);
                }

                break;
        };

        if(bReceiveResponse)
        {
            CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_MSG_COMMAND_FLAG);
        }

        if(bResponseErrorFlag)
        {
            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_SEND_MSG_RESPONSE, MHL_MSG_SEND_RESPONSE_TIME);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_EMSCParsingEMSCSupport()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_EMSCParsingEMSCSupport(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *bReveiceEMSC)
{
    MS_U8 ucLength = bReveiceEMSC[3] *MHL_EMSC_SUPPORT_ITEM_LENGTH +6;

    if(pMHLResourcePrivate != NULL)
    {

    }

    return ucLength;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ParsingEMSCReceiveData()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ParsingEMSCReceiveData(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 uctemp = 3;
    MS_U8 ucReceiveEMSC[MHL_EMSC_DATA_SIZE +1] = {0};
    MS_U16 usEMSCRxBufferSize = 0;
    MS_U16 usEMSCTxBufferSize = 0;
    MS_U16 usWriteBurstID = 0;

    mhal_mhl_GetEMSCReceiveData(ucReceiveEMSC);

    usEMSCTxBufferSize = ucReceiveEMSC[1]; // Rx unload ACK

    if(ucReceiveEMSC[2] > 0) // Check length remaining
    {
        while((ucReceiveEMSC[2] +3) > uctemp)
        {
            usWriteBurstID = (ucReceiveEMSC[uctemp] << 8) |ucReceiveEMSC[uctemp +1];

            switch(usWriteBurstID)
            {
                case MHL_BURST_ID_BLK_RCV_BUFFER_INFO:
                    usEMSCTxBufferSize = usEMSCTxBufferSize +((ucReceiveEMSC[uctemp +3] << 8) |ucReceiveEMSC[uctemp +2]) -MHL_EMSC_DATA_SIZE;

                    uctemp = uctemp +4;

                    MDRV_MHL_MSG_INFO("** MHL eMSC parsing buffer info in port %c !!\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                    break;

                case MHL_BURST_ID_BITS_PER_PIXEL_FMT:
                    if(ucReceiveEMSC[uctemp +7] == MHL_VIEW_PIXEL_FORMATE_16BPP)
                    {
                        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG);
                        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG);
                    }
                    else if(ucReceiveEMSC[uctemp +7] == MHL_VIEW_PIXEL_FORMATE_24BPP)
                    {
                        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_24BIT_MODE_FLAG);
                        CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_PP_MODE_FLAG);
                    }

                    uctemp = uctemp +8;

                    MDRV_MHL_MSG_INFO("** MHL eMSC parsing pexel FMT in port %c !!\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                    break;

                case MHL_BURST_ID_EMSC_SUPPORT:
                    uctemp = uctemp +_mdrv_mhl_EMSCParsingEMSCSupport(pMHLResourcePrivate, &ucReceiveEMSC[uctemp +3]);

                    MDRV_MHL_MSG_INFO("** MHL eMSC parsing eMSC support in port %c !!\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                    break;

                default:
                    MDRV_MHL_MSG_INFO("** MHL eMSC parsing unknow burst ID in port %c !!\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

                    uctemp = ucReceiveEMSC[2] +3;

                    break;
            };
        }
    }

    pMHLResourcePrivate->usEMSCRxBufferSize = pMHLResourcePrivate->usEMSCRxBufferSize - ucReceiveEMSC[0];
    pMHLResourcePrivate->usEMSCTxBufferSize = usEMSCTxBufferSize + pMHLResourcePrivate->usEMSCTxBufferSize;
    usEMSCRxBufferSize = pMHLResourcePrivate->usEMSCRxBufferSize;
    usEMSCTxBufferSize = pMHLResourcePrivate->usEMSCTxBufferSize;

    MDRV_MHL_MSG_INFO("** MHL eMSC receive Rx buffer size %d in port %c !!\r\n", usEMSCRxBufferSize, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
    MDRV_MHL_MSG_INFO("** MHL eMSC receive Tx buffer size %d in port %c !!\r\n", usEMSCTxBufferSize, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusRxHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusRxHandler(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucReceiveCount = 0;
    MS_U8 ucReceiveMSC[MHL_CBUS_DATA_SIZE +4];

    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
    {
        // Check receive MSC
        if(mhal_mhl_CheckSRAMReceiveBuffer())
        {
            ucReceiveCount = pMHLResourcePrivate->ucReceiveCounter;

            if(ucReceiveCount > 0)
            {
                for( ; ucReceiveCount > 0; ucReceiveCount--)
                {
                    if(_mdrv_mhl_GetCbusReceiveMSC(ucReceiveMSC))
                    {
                        _mdrv_mhl_SetReceiveCaseFlag(pMHLResourcePrivate, ucReceiveMSC);

                        pMHLResourcePrivate->ucReceiveCounter--;
                    }
                }
            }
            else if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_RECEIVE_LOSE_FLAG))
            {
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_RECEIVE_LOSE_FLAG);

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_CHECK_RECEIVE_LOSE, MHL_CHECK_RECEIVE_LOSE_TIME);
            }
        }

        // Check receive EMSC
        if(pMHLResourcePrivate->ucEMSCReceiveCounter > 0)
        {
            _mdrv_mhl_ParsingEMSCReceiveData(pMHLResourcePrivate);

            pMHLResourcePrivate->ucEMSCReceiveCounter--;
        }

        if(pMHLResourcePrivate->ulCbusReceiveIndex > 0)
        {
            _mdrv_mhl_CbusReceiveEventProc(pMHLResourcePrivate);

            _mdrv_mhl_CbusMSGEventProc(pMHLResourcePrivate);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_TxQueueHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_TxQueueHandler(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucQueueIndex = 0;

    if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_SEND_FLAG))
    {
        ucQueueIndex = _mdrv_mhl_GetCbusQueueIndex(pMHLResourcePrivate);

        if(ucQueueIndex < MHL_CBUS_TOTAL_QUEUE_NUM)
        {
            if(_mdrv_mhl_CbusTxWriteCommand(pMHLResourcePrivate->stSignalStatusInfo.bECbusEnableFlag, &(pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex])))
            {
                // Retry send packet twice
                if(pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState == MHL_QUEUE_STATE_SEND)
                {
                    pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_IDLE;
                }
                else if(pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState > MHL_QUEUE_STATE_SEND)
                {
                    pMHLResourcePrivate->stCbusQueueInfo[ucQueueIndex].ucQueueState = MHL_QUEUE_STATE_SEND;
                }

                // Update current index to search index, but not Rx queue index
                if(ucQueueIndex < MHL_CBUS_RX_QUEUE_INDEX)
                {
                    pMHLResourcePrivate->ucCurrentQueueIndex = ucQueueIndex;
                }

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_CBUS_SILENCE, MHL_CBUS_SEND_SILENCE_TIME);

                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_SEND_FLAG);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendDisplayCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendDisplayCommand(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucDisplayIndex = (pMHLResourcePrivate->usDisplayIndex) &BMASK(7:0);
    MS_U8 ucDisplayMask = MHL_CBUS_DISPLAY_MASK;

    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_DISPLAY_COMMAND_ENABLE_FLAG))
    {
        if(_mdrv_mhl_CheckSendCommand(pMHLResourcePrivate))
        {
            if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_ECBUS_FUNCTION_FLAG))
            {
                ucDisplayMask = ucDisplayMask |MHL_ECBUS_DISPLAY_MASK;
            }

            ucDisplayIndex = ucDisplayIndex ^ucDisplayMask;

            if(ucDisplayIndex > 0)
            {
                switch(_mdrv_mhl_CbusGetLeastFlag(ucDisplayIndex))
                {
                    case MHL_CBUS_HPD_SET_FLAG:
                        _mdrv_mhl_CbusSendSetHPD(pMHLResourcePrivate, TRUE);
                        break;

                    case MHL_CBUS_DEVCAP_READY_FLAG:
                        _mdrv_mhl_CbusSendDevcapReady(pMHLResourcePrivate);
                        break;

                    case MHL_CBUS_PATH_EN_FLAG:
                        _mdrv_mhl_CbusSendPathEnable(pMHLResourcePrivate, TRUE);
                        break;

                    case MHL_CBUS_DEVCAP_CHANGE_FLAG:
                        _mdrv_mhl_CbusSendDevcapChange(pMHLResourcePrivate);
                        break;

                    case MHL_CBUS_VERSION_CODE_FLAG:
                        _mdrv_mhl_CbusSendVersionCode(pMHLResourcePrivate);
                        break;

                    case MHL_CBUS_LINK_STATUS_NORMAL_FLAG:
                        _mdrv_mhl_CbusSendLinkStatusNormal(pMHLResourcePrivate);
                        break;

                    default:

                        break;
                };
            }
        }
        else if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CHECK_CONTENT_ON_FLAG))
        {
            if(!GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG))
            {
                _mdrv_mhl_CbusSendSetHPD(pMHLResourcePrivate, TRUE);
            }
            else if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_PATH_EN_FLAG))
            {
                _mdrv_mhl_CbusSendPathEnable(pMHLResourcePrivate, FALSE);
            }
            else if(!GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_READY_FLAG))
            {
                _mdrv_mhl_CbusSendDevcapReady(pMHLResourcePrivate);
            }
        }
        else
        {
            if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG))
            {
                _mdrv_mhl_CbusSendSetHPD(pMHLResourcePrivate, FALSE);

                CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_PATH_EN_FLAG);

                if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_LINK_STATUS_NORMAL_FLAG))
                {
                    CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_LINK_STATUS_NORMAL_FLAG);
                }
            }

            if(GET_MHL_DEVICE_FLAG(pMHLResourcePrivate->usDeviceIndex, MHL_DEVICE_ROKU_DONGLE_FLAG))
            {
                if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_READY_FLAG))
                {
                    _mdrv_mhl_CbusSendDevcapReady(pMHLResourcePrivate);
                }
                else if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_PATH_EN_FLAG))
                {
                    _mdrv_mhl_CbusSendPathEnable(pMHLResourcePrivate, TRUE);
                }
            }

            if(GET_MHL_DEVICE_FLAG(pMHLResourcePrivate->usDeviceIndex, MHL_DEVICE_PHONE_HTC_FLAG))
            {
                if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_READY_FLAG))
                {
                    _mdrv_mhl_CbusSendDevcapReady(pMHLResourcePrivate);
                }
            }
            else if(GET_MHL_DEVICE_FLAG(pMHLResourcePrivate->usDeviceIndex, MHL_DEVICE_PHONE_SONY_FLAG))
            {
                if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_DEVCAP_READY_FLAG))
                {
                    _mdrv_mhl_CbusSendDevcapReady(pMHLResourcePrivate);
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetVenderIDProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_GetVenderIDProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_GET_VENDER_ID_FLAG))
    {
        if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_GET_VENDER_ID_FLAG))
        {
            if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG))
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG);
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_GET_VENDER_ID_FLAG);
                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_GET_VENDER_ID_FLAG);
                SET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_GET_VENDER_ID_DONE_FLAG);

                pMHLResourcePrivate->ucSourceVenderID = pMHLResourcePrivate->ucAskData;

#if(MHL_DEBUG_SOURCE_INFO)
                MDRV_MHL_MSG_INFO("** MHL get vender ID %d\r\n", pMHLResourcePrivate->ucSourceVenderID);
#endif
            }
        }
        else if((!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG)) && (!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG)))
        {
            if(_mdrv_mhl_CbusSendGetVenderID(pMHLResourcePrivate))
            {
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_GET_VENDER_ID_FLAG);
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckDeviceCapability()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckDeviceCapability(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U16 usAdopterID = 0;
    MS_U16 usDeviceID = 0;

    usAdopterID = pMHLResourcePrivate->ucSourceDevcapTable[4] |(pMHLResourcePrivate->ucSourceDevcapTable[3] << 8);
    usDeviceID = pMHLResourcePrivate->ucSourceDevcapTable[12] |(pMHLResourcePrivate->ucSourceDevcapTable[11] << 8);

    pMHLResourcePrivate->usDeviceIndex = 0;

    if(usAdopterID == MHL_ADOPTER_ID_ROKU)
    {
        SET_MHL_DEVICE_FLAG(pMHLResourcePrivate->usDeviceIndex, MHL_DEVICE_ROKU_DONGLE_FLAG);
    }

    if(usAdopterID == MHL_ADOPTER_ID_HTC)
    {
        SET_MHL_DEVICE_FLAG(pMHLResourcePrivate->usDeviceIndex, MHL_DEVICE_PHONE_HTC_FLAG);
    }

    if(usAdopterID == MHL_ADOPTER_ID_SONY)
    {
        SET_MHL_DEVICE_FLAG(pMHLResourcePrivate->usDeviceIndex, MHL_DEVICE_PHONE_SONY_FLAG);
    }

    if(usDeviceID == MHL_DEVICE_ID_HTC)
    {
        // Do nothing
    }

    if(pMHLResourcePrivate->ucSourceDevcapTable[MHL_CBUS_FEATURE_FLAG] &BIT(0))
    {
        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_RCP_FUNCTION_FLAG);
    }

    if(pMHLResourcePrivate->ucSourceDevcapTable[MHL_CBUS_FEATURE_FLAG] &BIT(1))
    {
        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_RAP_FUNCTION_FLAG);
    }

    if(pMHLResourcePrivate->ucSourceDevcapTable[MHL_CBUS_VERSION] >= 0x30)
    {
        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MHL3_VERSION_FLAG);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ReadDeviceCapabilityProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ReadDeviceCapabilityProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bReDoFlag = FALSE;
    MS_U8 ucBitNumber = 0;

    if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_DEVCAP_FLAG))
    {
        do
        {
            bReDoFlag = FALSE;

            if(pMHLResourcePrivate->usReadDevcapMask > 0)
            {
                ucBitNumber = _mdrv_mhl_GetLeastBitNum(pMHLResourcePrivate->usReadDevcapMask);

                if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_READ_DEVCAP_FLAG))
                {
                    if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG))
                    {
                        CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG);
                        CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_READ_DEVCAP_FLAG);

                        pMHLResourcePrivate->ucSourceDevcapTable[ucBitNumber] = pMHLResourcePrivate->ucAskData;

#if(MHL_DEBUG_SOURCE_INFO)
                        MDRV_MHL_MSG_INFO("** MHL read devcap[%d] = %x\r\n", ucBitNumber, pMHLResourcePrivate->ucSourceDevcapTable[ucBitNumber]);
#endif

                        bReDoFlag = TRUE;
                    }
                }
                else if((!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG)) && (!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG)))
                {
                    if(_mdrv_mhl_CbusReadDeviceCapability(pMHLResourcePrivate, ucBitNumber, FALSE))
                    {
                        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_READ_DEVCAP_FLAG);
                        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG);
                    }
                }

                if(bReDoFlag)
                {
                    pMHLResourcePrivate->usReadDevcapMask = MASKSET(pMHLResourcePrivate->usReadDevcapMask, 0, BIT(ucBitNumber));

                    if(pMHLResourcePrivate->usReadDevcapMask == 0)
                    {
                        SET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_READ_DEVCAP_DONE_FLAG);

                        _mdrv_mhl_CheckDeviceCapability(pMHLResourcePrivate);
                    }
                }
            }
            else
            {
                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_DEVCAP_FLAG);
            }
        }while(bReDoFlag);

        if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG))
        {
            CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG);
        }
    }
    else
    {
        if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG))
        {
            if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_XDEVCAP_FLAG))
            {
                CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG);
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_DEVCAP_FLAG);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ReadExtendDeviceCapabilityProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ReadExtendDeviceCapabilityProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bReDoFlag = FALSE;
    MS_U8 ucBitNumber = 0;

    if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_XDEVCAP_FLAG))
    {
        do
        {
            bReDoFlag = FALSE;

            if(pMHLResourcePrivate->usReadExtendDevcapMask > 0)
            {
                ucBitNumber = _mdrv_mhl_GetLeastBitNum(pMHLResourcePrivate->usReadExtendDevcapMask);

                if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_READ_DEVCAP_FLAG))
                {
                    if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG))
                    {
                        CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG);
                        CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_READ_DEVCAP_FLAG);

                        pMHLResourcePrivate->ucSourceDevcapTable[MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber] = pMHLResourcePrivate->ucAskData;

#if(MHL_DEBUG_SOURCE_INFO)
                        MDRV_MHL_MSG_INFO("** MHL read extend devcap[%d] = %x\r\n", ucBitNumber, pMHLResourcePrivate->ucSourceDevcapTable[MHL_DEVICE_CAPABILITY_SIZE +ucBitNumber]);
#endif

                        bReDoFlag = TRUE;
                    }
                }
                else if((!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG)) && (!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_FINISH_FLAG)))
                {
                    if(_mdrv_mhl_CbusReadDeviceCapability(pMHLResourcePrivate, ucBitNumber, TRUE))
                    {
                        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_READ_DEVCAP_FLAG);
                        SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_ASK_DATA_FLAG);
                    }
                }

                if(bReDoFlag)
                {
                    pMHLResourcePrivate->usReadExtendDevcapMask = MASKSET(pMHLResourcePrivate->usReadExtendDevcapMask, 0, BIT(ucBitNumber));

                    if(pMHLResourcePrivate->usReadExtendDevcapMask == 0)
                    {
                        SET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_READ_XDEVCAP_DONE_FLAG);

                        _mdrv_mhl_CheckDeviceCapability(pMHLResourcePrivate);
                    }
                }
            }
            else
            {
                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_XDEVCAP_FLAG);
            }
        }while(bReDoFlag);

        if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG))
        {
            CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG);
        }
    }
    else
    {
        if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG))
        {
            if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_DEVCAP_FLAG))
            {
                CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_XDEVCAP_SUPPORT_FLAG);
                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_XDEVCAP_FLAG);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_WriteBurstEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_WriteBurstEventProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    switch(pMHLResourcePrivate->ucWriteBurstState)
    {
        case MHL_CBUS_WRITE_BURST_START:
            if(_mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x20, BIT(2), FALSE))
            {
                pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_REQUEST;

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_WAIT_WRITE_GRANT, MHL_WAIT_WRITE_GRANT_TIME);
            }

            break;

        case MHL_CBUS_WRITE_BURST_SEND_DATA:
            if(_mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x20, BIT(1), FALSE))
            {
                pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_END;
            }
            break;

        case MHL_CBUS_WRITE_BURST_END:
            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_NONE;
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetDTDBurstfor3D()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetDTDBurstfor3D(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucDTDCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;

    ucDTDCount = ucSequence *MHL_3D_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_3D_DTD >> 8);
    pData[1] = (MHL_BURST_ID_3D_DTD &BMASK(7:0));
    pData[3] = pMHLResourcePrivate->uc3DTotalEntryDTD;

    if(pMHLResourcePrivate->uc3DTotalEntryDTD == 0)
    {
        bFinish = TRUE;
    }

    for(uctemp = 0; uctemp < MHL_3D_SEQUENCE_LENGTH; uctemp++)
    {
        if((!bFinish) && ((ucDTDCount +uctemp) == (pMHLResourcePrivate->uc3DTotalEntryDTD -1)))
        {
            bFinish = TRUE;
        }

        if((ucDTDCount +uctemp) < pMHLResourcePrivate->uc3DTotalEntryDTD)
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = pMHLResourcePrivate->uc3DInformationDTD[ucDTDCount +uctemp] &BMASK(3:0);
        }
        else
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = 0;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;

    if(bFinish)
    {
        pData[5] = pMHLResourcePrivate->uc3DTotalEntryDTD -ucDTDCount;
        ucSequence = 0;
    }
    else
    {
        pData[5] = MHL_3D_SEQUENCE_LENGTH;
    }

    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetVICBurstfor3D()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetVICBurstfor3D(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucVICCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;

    ucVICCount = ucSequence *MHL_3D_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_3D_VIC >> 8);
    pData[1] = (MHL_BURST_ID_3D_VIC &BMASK(7:0));
    pData[3] = pMHLResourcePrivate->uc3DTotalEntryVIC;

    if(pMHLResourcePrivate->uc3DTotalEntryVIC == 0)
    {
        bFinish = TRUE;
    }

    for(uctemp = 0; uctemp < MHL_3D_SEQUENCE_LENGTH; uctemp++)
    {
        if((!bFinish) && ((ucVICCount +uctemp) == (pMHLResourcePrivate->uc3DTotalEntryVIC -1)))
        {
            bFinish = TRUE;
        }

        if((ucVICCount +uctemp) < pMHLResourcePrivate->uc3DTotalEntryVIC)
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = pMHLResourcePrivate->uc3DInformationVIC[ucVICCount +uctemp] &BMASK(3:0);
        }
        else
        {
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH] = 0;
            pData[6 +uctemp *MHL_3D_ITEM_LENGTH +1] = 0;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;

    if(bFinish)
    {
        pData[5] = pMHLResourcePrivate->uc3DTotalEntryVIC -ucVICCount;
        ucSequence = 0;
    }
    else
    {
        pData[5] = MHL_3D_SEQUENCE_LENGTH;
    }

    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_Send3DTimingInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_Send3DTimingInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_DTD_FLAG))
    {
        if(_mdrv_mhl_GetDTDBurstfor3D(pMHLResourcePrivate, ucData))
        {
            bFinish = TRUE;
        }

        if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
        {
            if(bFinish)
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_DTD_FLAG);
            }

            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

            MDRV_MHL_MSG_INFO("** MHL sent 3D DTD port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
        }
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_VIC_FLAG))
    {
        if(_mdrv_mhl_GetVICBurstfor3D(pMHLResourcePrivate, ucData))
        {
            bFinish= TRUE;
        }

        if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
        {
            if(bFinish)
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_VIC_FLAG);
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_INFORMATION_FLAG);
            }

            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

            MDRV_MHL_MSG_INFO("** MHL sent 3D VIC port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetBurstforVirtualChannel()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetBurstforVirtualChannel(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_BOOL bVCConfirm, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucVCCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;
    MS_U8 ucVCTotalEntry = 0;
    MS_U8 ucVCFinishEntry = 0;
    MS_U16 usWriteBurstID = MHL_BURST_ID_VC_ASSIGN;

    if(bVCConfirm)
    {
        usWriteBurstID = MHL_BURST_ID_VC_CONFIRM;
    }

    ucVCFinishEntry = ucSequence *MHL_VC_SEQUENCE_LENGTH;

    pData[0] = (usWriteBurstID >> 8);
    pData[1] = (usWriteBurstID &BMASK(7:0));
    pData[15] = 0;

    for(uctemp = 0; uctemp < MHL_VIRTUAL_CHANNEL_LENGTH; uctemp++)
    {
        if(pMHLResourcePrivate->stVirtualChannelInfo[uctemp].bVCConfirm)
        {
            ucVCTotalEntry++;

            if(ucVCFinishEntry > 0)
            {
                ucVCFinishEntry--;
            }
            else if(ucVCCount < MHL_VC_SEQUENCE_LENGTH)
            {
                pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH] = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCNumber;
                pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH +1] = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCFeatureID;

                if(bVCConfirm)
                {
                    pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH +2] = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCResponseCode;
                }
                else
                {
                    pData[6 +ucVCCount *MHL_VC_ITEM_LENGTH +2] = pMHLResourcePrivate->stVirtualChannelInfo[uctemp].ucVCSlotSize;
                }

                ucVCCount++;
            }
        }
    }

    ucSequence++;

    pData[3] = ucVCTotalEntry;
    pData[4] = ucSequence;
    pData[5] = ucVCCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_VC_SEQUENCE_LENGTH +ucVCCount) == ucVCTotalEntry)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendVirtualChannelInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendVirtualChannelInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bFinish = FALSE;
    MS_BOOL bSendVCConfirm = FALSE;
    MS_U8 ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    if(_mdrv_mhl_GetBurstforVirtualChannel(pMHLResourcePrivate, GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_CONFIRM_FLAG), ucData))
    {
        bFinish = TRUE;
    }

    if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
    {
        if(bFinish)
        {
            if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_CONFIRM_FLAG))
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_CONFIRM_FLAG);

                bSendVCConfirm = TRUE;
            }

            CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_INFORMATION_FLAG);
        }

        pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

        MDRV_MHL_MSG_INFO("** MHL sent VC information port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
    }

    if(bSendVCConfirm)
    {
        _mdrv_mhl_SortingVCInformation(pMHLResourcePrivate, TRUE);

        _mdrv_mhl_eCbusTimeSlotAllocation(pMHLResourcePrivate);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetHighEndVideoVIC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetHighEndVideoVIC(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucEntryCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;
    MS_U8 ucFinishEntry = 0;
    MS_U8 ucTotalEntry = pMHLResourcePrivate->ucHighEndVICTotalEntry;

    ucFinishEntry = ucSequence *MHL_HEV_VIC_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_HEV_VIC >> 8);
    pData[1] = (MHL_BURST_ID_HEV_VIC &BMASK(7:0));
    pData[3] = ucTotalEntry;

    for(uctemp = 0; uctemp < MHL_HEV_VIC_SEQUENCE_LENGTH; uctemp++)
    {
        if((uctemp +ucFinishEntry) == ucTotalEntry)
        {
            break;
        }

        pData[6 +(uctemp *2)] = pMHLResourcePrivate->usHighEndVICTable[uctemp +ucFinishEntry] >> 8;
        pData[7 +(uctemp *2)] = pMHLResourcePrivate->usHighEndVICTable[uctemp +ucFinishEntry] &BMASK(7:0);

        ucEntryCount++;
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[5] = ucEntryCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_HEV_VIC_SEQUENCE_LENGTH +ucEntryCount) == ucTotalEntry)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetHighEndVideoDTDA()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetHighEndVideoDTDA(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucEntryCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;
    MS_U8 ucFinishEntry = 0;
    MS_U8 ucTotalEntry = MHL_HEV_DTD_TOTAL_ENTRY;

    ucFinishEntry = ucSequence *MHL_HEV_DTD_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_HEV_DTDA >> 8);
    pData[1] = (MHL_BURST_ID_HEV_DTDA &BMASK(7:0));
    pData[3] = MHL_HEV_DTD_TOTAL_ENTRY;

    for(uctemp = 0; uctemp < ucTotalEntry; uctemp++)
    {
        if(ucFinishEntry > 0)
        {
            ucFinishEntry--;
        }
        else if(ucEntryCount < MHL_HEV_DTD_SEQUENCE_LENGTH)
        {
            ucEntryCount++;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_HEV_DTD_SEQUENCE_LENGTH +ucEntryCount) == MHL_HEV_DTD_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_GetHighEndVideoDTDB()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetHighEndVideoDTDB(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucEntryCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;
    MS_U8 ucFinishEntry = 0;
    MS_U8 ucTotalEntry = MHL_HEV_DTD_TOTAL_ENTRY;

    ucFinishEntry = ucSequence *MHL_HEV_DTD_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_HEV_DTDB >> 8);
    pData[1] = (MHL_BURST_ID_HEV_DTDB &BMASK(7:0));
    pData[3] = MHL_HEV_DTD_TOTAL_ENTRY;

    for(uctemp = 0; uctemp < ucTotalEntry; uctemp++)
    {
        if(ucFinishEntry > 0)
        {
            ucFinishEntry--;
        }
        else if(ucEntryCount < MHL_HEV_DTD_SEQUENCE_LENGTH)
        {
            ucEntryCount++;
        }
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_HEV_DTD_SEQUENCE_LENGTH +ucEntryCount) == MHL_HEV_DTD_TOTAL_ENTRY)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendHighEndVideoSupport()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendHighEndVideoSupport(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_VIC_FLAG))
    {
        if(_mdrv_mhl_GetHighEndVideoVIC(pMHLResourcePrivate, ucData))
        {
            bFinish = TRUE;
        }

        if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
        {
            if(bFinish)
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_VIC_FLAG);
            }

            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

            MDRV_MHL_MSG_INFO("** MHL sent HEV VIC port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
        }
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_DTDA_FLAG))
    {
        if(_mdrv_mhl_GetHighEndVideoDTDA(pMHLResourcePrivate, ucData))
        {
            bFinish = TRUE;
        }

        if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
        {
            if(bFinish)
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_DTDA_FLAG);
            }

            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

            MDRV_MHL_MSG_INFO("** MHL sent HEV DTDA port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
        }
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_DTDB_FLAG))
    {
        if(_mdrv_mhl_GetHighEndVideoDTDB(pMHLResourcePrivate, ucData))
        {
            bFinish = TRUE;
        }

        if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
        {
            if(bFinish)
            {
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_DTDB_FLAG);
                CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HIGH_END_VIDEO_FLAG);
            }

            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

            MDRV_MHL_MSG_INFO("** MHL sent HEV DTDB port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetAudioDataTunnel()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GetAudioDataTunnel(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucEntryCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;
    MS_U8 ucFinishEntry = 0;
    MS_U8 ucTotalEntry = MHL_AUDIO_TUNNEL_TOTAL_ENTRY;

    ucFinishEntry = ucSequence *MHL_AUDIO_TUNNEL_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_ADT_BURSTID >> 8);
    pData[1] = (MHL_BURST_ID_ADT_BURSTID &BMASK(7:0));
    pData[3] = ucTotalEntry;

    if((ucSequence == 0) && (MHL_AUDIO_TUNNEL_SADB_RNTRY > 0)) // Support speaker allocation data block
    {
        pData[5] = 1;
    }

    for(uctemp = 0; uctemp < MHL_AUDIO_TUNNEL_SEQUENCE_LENGTH; uctemp++)
    {
        if((uctemp +ucFinishEntry) == ucTotalEntry)
        {
            break;
        }

        pData[7 +(uctemp *3)] = pMHLResourcePrivate->ulAudioTunnelTable[uctemp +ucFinishEntry] >> 16;
        pData[8 +(uctemp *3)] = pMHLResourcePrivate->ulAudioTunnelTable[uctemp +ucFinishEntry] >> 8;
        pData[9 +(uctemp *3)] = pMHLResourcePrivate->ulAudioTunnelTable[uctemp +ucFinishEntry] &BMASK(7:0);

        ucEntryCount++;
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[6] = ucEntryCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_AUDIO_TUNNEL_SEQUENCE_LENGTH +ucEntryCount) == ucTotalEntry)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendAudioDataTunnelSupport()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendAudioDataTunnelSupport(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    if(_mdrv_mhl_GetAudioDataTunnel(pMHLResourcePrivate, ucData))
    {
        bFinish = TRUE;
    }

    if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
    {
        if(bFinish)
        {
            CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_AUDIO_DATA_TUNNEL_FLAG);
        }

        pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

        MDRV_MHL_MSG_INFO("** MHL sent audio tunnel port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GeteMSCSupportInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_GeteMSCSupportInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 *pData)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucEntryCount = 0;
    MS_U8 ucSequence = pMHLResourcePrivate->ucWriteBurstSequence;
    MS_U8 ucFinishEntry = 0;
    MS_U8 ucTotalEntry = MHL_EMSC_SUPPORT_TOTAL_ENTRY;

    ucFinishEntry = ucSequence *MHL_EMSC_SUPPORT_SEQUENCE_LENGTH;

    pData[0] = (MHL_BURST_ID_EMSC_SUPPORT >> 8);
    pData[1] = (MHL_BURST_ID_EMSC_SUPPORT &BMASK(7:0));
    pData[3] = ucTotalEntry;

    for(uctemp = 0; uctemp < MHL_EMSC_SUPPORT_SEQUENCE_LENGTH; uctemp++)
    {
        if((uctemp +ucFinishEntry) == ucTotalEntry)
        {
            break;
        }

        pData[6 +(uctemp *2)] = pMHLResourcePrivate->usEMSCSupportTable[uctemp +ucFinishEntry] >> 8;
        pData[7 +(uctemp *2)] = pMHLResourcePrivate->usEMSCSupportTable[uctemp +ucFinishEntry] &BMASK(7:0);

        ucEntryCount++;
    }

    ucSequence++;

    pData[4] = ucSequence;
    pData[5] = ucEntryCount;
    pData[2] = _mdrv_mhl_WriteBurstCheckSum(pData);

    if(((ucSequence -1) *MHL_EMSC_SUPPORT_SEQUENCE_LENGTH +ucEntryCount) == ucTotalEntry)
    {
        bFinish = TRUE;
        ucSequence = 0;
    }

    pMHLResourcePrivate->ucWriteBurstSequence = ucSequence;

    return bFinish;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendeMSCSupportInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendeMSCSupportInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bFinish = FALSE;
    MS_U8 ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    if(_mdrv_mhl_GeteMSCSupportInformation(pMHLResourcePrivate, ucData))
    {
        bFinish = TRUE;
    }

    if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
    {
        if(bFinish)
        {
            CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_EMSC_DATA_TUNNEL_FLAG);
        }

        pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

        MDRV_MHL_MSG_INFO("** MHL sent eMSC support port %c in %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), ucData[4]);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendBISTReturnStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendBISTReturnStatus(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucData[MHL_MSC_SCRATCHPAD_SIZE] = {0};

    ucData[0] = (MHL_BURST_ID_BIST_RETURN_STAT >> 8);
    ucData[1] = (MHL_BURST_ID_BIST_RETURN_STAT &BMASK(7:0));
    ucData[12] = (pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusErrorCount >> 8);
    ucData[13] = (pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusErrorCount &BMASK(7:0));
    ucData[14] = (pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkErrorCount >> 8);
    ucData[15] = (pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkErrorCount &BMASK(7:0));
    ucData[2] = _mdrv_mhl_WriteBurstCheckSum(ucData);

    if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, MHL_MSC_SCRATCHPAD_SIZE, ucData))
    {
        CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_BIST_RETURN_STATUS_FLAG);

        pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

        MDRV_MHL_MSG_INFO("** MHL send BIST eCbus port %c error count %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusErrorCount);
        MDRV_MHL_MSG_INFO("** MHL send BIST AV link port %c error count %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkErrorCount);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendWriteBurstInformation()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_SendWriteBurstInformation(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bSendWriteBurst = FALSE;
    MS_U8 ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_NONE;

    if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_USER_DATA;
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_INFORMATION_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION;
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_VC_INFORMATION_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL;
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HIGH_END_VIDEO_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO;
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_AUDIO_DATA_TUNNEL_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL;
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_EMSC_DATA_TUNNEL_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL;
    }
    else if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_BIST_RETURN_STATUS_FLAG))
    {
        ucWriteBurstType = MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS;
    }

    if(ucWriteBurstType != MHL_CBUS_SEND_WRITE_BURST_NONE)
    {
        if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_NONE)
        {
            pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_START;
        }
        else if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
        {
            bSendWriteBurst = TRUE;
        }
    }

    if(bSendWriteBurst)
    {
        switch(ucWriteBurstType)
        {
            case MHL_CBUS_SEND_WRITE_BURST_3D_INFORMATION:
                _mdrv_mhl_Send3DTimingInformation(pMHLResourcePrivate);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_VIRTUAL_CHANNEL:
                _mdrv_mhl_SendVirtualChannelInformation(pMHLResourcePrivate);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_HIGH_END_VIDEO:
                _mdrv_mhl_SendHighEndVideoSupport(pMHLResourcePrivate);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_AUDIO_DATA_TUNNEL:
                _mdrv_mhl_SendAudioDataTunnelSupport(pMHLResourcePrivate);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_EMSC_DATA_TUNNEL:
                _mdrv_mhl_SendeMSCSupportInformation(pMHLResourcePrivate);
                break;

            case MHL_CBUS_SEND_WRITE_BURST_BIST_RETURN_STATUS:
                _mdrv_mhl_SendBISTReturnStatus(pMHLResourcePrivate);
                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_FeatureRequestStateProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_FeatureRequestStateProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    switch(pMHLResourcePrivate->ucFeatureRequestState)
    {
        case MHL_CBUS_FEATURE_START:
            pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_SEND_3D_INFORMATION;
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_INFORMATION_FLAG);
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_DTD_FLAG);
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_VIC_FLAG);

            break;

        case MHL_CBUS_FEATURE_SEND_3D_INFORMATION:
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_3D_INFORMATION_FLAG))
            {
                pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO;
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HIGH_END_VIDEO_FLAG);
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_VIC_FLAG);
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_DTDA_FLAG);
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HEV_DTDB_FLAG);
            }

            break;

        case MHL_CBUS_FEATURE_SEND_HIGH_END_VIDEO:
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_HIGH_END_VIDEO_FLAG))
            {
                pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL;
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_AUDIO_DATA_TUNNEL_FLAG);
            }

            break;

        case MHL_CBUS_FEATURE_SEND_AUDIO_DATA_TUNNEL:
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_AUDIO_DATA_TUNNEL_FLAG))
            {
                pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL;
                SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_EMSC_DATA_TUNNEL_FLAG);
            }

            break;

        case MHL_CBUS_FEATURE_SEND_EMSC_DATA_TUNNEL:
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_EMSC_DATA_TUNNEL_FLAG))
            {
                if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_NONE)
                {
                    pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE;
                }
            }

            break;

        case MHL_CBUS_FEATURE_SEND_FEATURE_COMPLETE:
            if(_mdrv_mhl_CbusWriteStatusOrSetInt(pMHLResourcePrivate, 0x20, BIT(6), FALSE))
            {
                pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_END;
            }

            break;

        case MHL_CBUS_FEATURE_END:
            pMHLResourcePrivate->ucFeatureRequestState = MHL_CBUS_FEATURE_NONE;
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mapi_mhl_CbusBISTStateProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusBISTStateProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucBISTECbusMode = 0;

    switch(pMHLResourcePrivate->ucBISTModeState)
    {
        case MHL_CBUS_BIST_RECEIVE_SETUP:
            if(_mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_BIST_READY, pMHLResourcePrivate->stBISTParameterInfo.ucBISTReadyIndex, FALSE)) // Send BIST ready
            {
                pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_WAIT_TRIGGER;

                mhal_mhl_SetBISTParameterInfo(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stBISTParameterInfo));
            }

            break;

        case MHL_CBUS_BIST_RECEIVE_TRIGGER:
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_ECBUS_SWITCH;
            ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_UP;

            MDRV_MHL_MSG_INFO("** MHL BIST receive trigger in port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            if(pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration > 0)
            {
                MDRV_MHL_MSG_INFO("** MHL set BIST trigger timer in port %c timer %d\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath), pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration);

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_BIST_PROCESS, pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusDuration);
            }

            break;

        case MHL_CBUS_BIST_ECBUS_SWITCH:
            //mhal_mhl_GetBISTErrorCount(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stBISTParameterInfo));
            break;

        case MHL_CBUS_BIST_EVENT_FINISH:
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_WAIT_TRIGGER;
            ucBISTECbusMode = MHL_RAP_KEY_CBUS_MODE_DOWN;

            MDRV_MHL_MSG_INFO("** MHL BIST run finish in port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

            break;

        case MHL_CBUS_BIST_REQUEST_STATUS:
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_BIST_RETURN_STATUS_FLAG);
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_RETURN_STATUS;
            break;

        case MHL_CBUS_BIST_RETURN_STATUS:
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_BIST_RETURN_STATUS_FLAG))
            {
                pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_WAIT_TRIGGER;
            }
            break;

        case MHL_CBUS_BIST_END:
            pMHLResourcePrivate->ucBISTModeState = MHL_CBUS_BIST_NONE;
            pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag = FALSE;
            break;

        default:

            break;
    };

    if(ucBISTECbusMode > 0)
    {
        if(ucBISTECbusMode == MHL_RAP_KEY_CBUS_MODE_UP)
        {
            pMHLResourcePrivate->stBISTParameterInfo.usBISTeCbusErrorCount = 0;
            pMHLResourcePrivate->stBISTParameterInfo.usBISTAVLinkErrorCount = 0;
            pMHLResourcePrivate->stSignalStatusInfo.bBISTEnableFlag = TRUE;
            pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_BIST_MODE_UP;
        }
        else if(ucBISTECbusMode == MHL_RAP_KEY_CBUS_MODE_DOWN)
        {
            pMHLResourcePrivate->ucECbusState = MHL_ECBUS_STATE_BIST_MODE_DOWN;

            mhal_mhl_GetBISTErrorCount(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stBISTParameterInfo));
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_SendEMSCDataString()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mdrv_mhl_SendEMSCDataString(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_U8 ucLength, MS_U8 *ucSendData)
{
    MS_BOOL bDoAgainFlag = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucSendStatus = 0;
    MS_U8 ucSendEMSC[MHL_EMSC_DATA_SIZE] = {0};

    do
    {
        bDoAgainFlag = FALSE;

        switch(pMHLResourcePrivate->ucEMSCSendStatus)
        {
            case MHL_EMSC_SEND_NONE:
                if((ucLength +2) <= pMHLResourcePrivate->usEMSCTxBufferSize) // eMSC data length must small than Tx buffer +2
                {
                    ucSendEMSC[0] = MHL_EMSC_DATA_SIZE - pMHLResourcePrivate->usEMSCRxBufferSize;
                    ucSendEMSC[1] = ucLength;

                    if(ucSendData != 0)
                    {
                        for(uctemp = 0; uctemp < ucLength; uctemp++)
                        {
                            ucSendEMSC[uctemp +2] = ucSendData[uctemp];
                        }
                    }

                    pMHLResourcePrivate->ucEMSCSendStatus = MHL_EMSC_SEND_DATA;
                    bDoAgainFlag = TRUE;
                }

                break;

            case MHL_EMSC_SEND_DATA:
                ucSendStatus = mhal_mhl_GetEMSCSendStatus();
                mhal_mhl_InsertEMSCSendData(ucLength +2, ucSendEMSC);

                pMHLResourcePrivate->ucEMSCSendStatus = MHL_EMSC_SEND_CHECK;

                break;

            case MHL_EMSC_SEND_CHECK:
                ucSendStatus = mhal_mhl_GetEMSCSendStatus();

                if(ucSendStatus != MHL_EMSC_SEND_NONE)
                {
                    pMHLResourcePrivate->ucEMSCSendStatus = ucSendStatus;
                }

                break;

            case MHL_EMSC_SEND_PASS:
                pMHLResourcePrivate->ucEMSCSendStatus = MHL_EMSC_SEND_NONE;
                break;

            case MHL_EMSC_SEND_FAIL:
                pMHLResourcePrivate->ucEMSCSendStatus = MHL_EMSC_SEND_NONE;
                break;

            default:
                pMHLResourcePrivate->ucEMSCSendStatus = MHL_EMSC_SEND_NONE;
                break;
        };
    }while(bDoAgainFlag);

    if(pMHLResourcePrivate->ucEMSCSendStatus == MHL_EMSC_SEND_PASS)
    {
        MDRV_MHL_MSG_INFO("** MHL eMSC send pass !! port %c\r\n", MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));

        pMHLResourcePrivate->usEMSCRxBufferSize = MHL_EMSC_DATA_SIZE;
        pMHLResourcePrivate->usEMSCTxBufferSize = pMHLResourcePrivate->usEMSCTxBufferSize -ucLength -2;

        MDRV_MHL_MSG_INFO("** MHL eMSC send Rx buffer size %d in port %c !!\r\n", pMHLResourcePrivate->usEMSCRxBufferSize, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
        MDRV_MHL_MSG_INFO("** MHL eMSC send Tx buffer size %d in port %c !!\r\n", pMHLResourcePrivate->usEMSCTxBufferSize, MHL_INPUT_PORT(pMHLResourcePrivate->ucMHLSupportPath));
    }

    return pMHLResourcePrivate->ucEMSCSendStatus;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CheckEMSCRxBufferSize()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CheckEMSCRxBufferSize(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_U8 ucSendStatus = 0;

    if(pMHLResourcePrivate->usEMSCRxBufferSize < MHL_EMSC_DATA_SIZE_THRESHOLD)
    {
        ucSendStatus = _mdrv_mhl_SendEMSCDataString(pMHLResourcePrivate, 0, NULL);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CbusTxHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_CbusTxHandler(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
    {
        _mdrv_mhl_TxQueueHandler(pMHLResourcePrivate);

        if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CBUS_TX_QUEUE_FREEZE_FLAG))
        {
            _mdrv_mhl_SendDisplayCommand(pMHLResourcePrivate);

            _mdrv_mhl_WriteBurstEventProc(pMHLResourcePrivate);

            _mdrv_mhl_SendWriteBurstInformation(pMHLResourcePrivate);

            if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_ECBUS_FUNCTION_FLAG))
            {
                _mdrv_mhl_ReadExtendDeviceCapabilityProc(pMHLResourcePrivate);

                _mdrv_mhl_FeatureRequestStateProc(pMHLResourcePrivate);

                _mdrv_mhl_CheckEMSCRxBufferSize(pMHLResourcePrivate);
            }

            _mdrv_mhl_GetVenderIDProc(pMHLResourcePrivate);

            _mdrv_mhl_ReadDeviceCapabilityProc(pMHLResourcePrivate);
        }

        if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_ECBUS_FUNCTION_FLAG))
        {
            _mdrv_mhl_CbusBISTStateProc(pMHLResourcePrivate);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_AutoSwitchProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_AutoSwitchProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bOnLinePortFlag = FALSE;

    if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG))
    {
        if(mhal_mhl_CheckInputPort(pMHLResourcePrivate->ucMHLSupportPath) && mhal_mhl_CheckPIPWindow())
        {
            bOnLinePortFlag = TRUE;
        }

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_AUTO_SWITCH_SETTING_FLAG))
        {
            if(GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WAKE_FLAG))
            {
                CLR_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WAKE_FLAG);
                CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_AUTO_SWITCH_SETTING_FLAG);
                SET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG);
            }
            else // Setting but not wake up
            {
                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
                {
                    mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, MHL_VBUS_CHARGE_ON);
                }
                else
                {
                    mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, MHL_VBUS_HW_DETCET);
                }

                if(bOnLinePortFlag)
                {
                    mhal_mhl_CbusWakeupIntSetting(pMHLResourcePrivate->ucMHLSupportPath, FALSE);

                    CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_AUTO_SWITCH_SETTING_FLAG);
                    CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
                }
            }
        }
        else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
        {
            CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
        }
        else // Not setting
        {
            if(!bOnLinePortFlag)
            {
                mhal_mhl_CbusWakeupIntSetting(pMHLResourcePrivate->ucMHLSupportPath, TRUE);

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_AUTO_SWITCH_SETTING_FLAG);
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ForceSendClearHPD()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_ForceSendClearHPD(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    stMHL_QUEUE_INFO stCbusQueueInfo = {0};

    stCbusQueueInfo.ucQueueState = MHL_QUEUE_STATE_PENDING;
    stCbusQueueInfo.ucLength = 0;
    stCbusQueueInfo.ucCommand = MSC_CLR_HPD;

    if(_mdrv_mhl_CbusTxWriteCommand(pMHLResourcePrivate->stSignalStatusInfo.bECbusEnableFlag, &stCbusQueueInfo))
    {
        CLR_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_RtermControlHWMode()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_RtermControlHWMode(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate, MS_BOOL bEnableFlag)
{
    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
    {
        mhal_mhl_RtermControlHWMode(pMHLResourcePrivate->ucMHLSupportPath, bEnableFlag);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_PollingEventProc
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_PollingEventProc(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    _mdrv_mhl_TimerHandler(pMHLResourcePrivate);

    _mdrv_mhl_RtermControlHWMode(pMHLResourcePrivate, FALSE);

    _mdrv_mhl_CheckStatusPolling(pMHLResourcePrivate);

    _mdrv_mhl_RtermControlHWMode(pMHLResourcePrivate, TRUE);

    _mdrv_mhl_CbusRxHandler(pMHLResourcePrivate);

    _mdrv_mhl_CbusTxHandler(pMHLResourcePrivate);

    _mdrv_mhl_AutoSwitchProc(pMHLResourcePrivate);
}

#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ResourceGetPrivate
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_ResourceGetPrivate(void** ppPrivate)
{
    MS_BOOL bGetPrivateFlag = FALSE;

    *ppPrivate = &stMHLResourcePrivate;

    if(*ppPrivate != NULL)
    {
        bGetPrivateFlag = TRUE;
    }

    return bGetPrivateFlag;
}

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_PollingTask
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_mhl_PollingTask(void)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bMHLTaskProcFlag = TRUE;
    MS_U32 usPollingTaskInterval = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaModuleGetPtr(MODULE_MHL, &pModule);

    while(bMHLTaskProcFlag)
    {
        if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
        }
        else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
        }
        else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
        {
            _mdrv_mhl_PollingEventProc(pMHLResourcePrivate);

            if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG))
            {
                usPollingTaskInterval = MHL_POLLING_INTERVAL_BUSY;
            }
            else
            {
                usPollingTaskInterval = MHL_POLLING_INTERVAL_NORMAL;
            }

            bMHLTaskProcFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_TASK_PROCESS_FLAG);
        }

        UtopiaResourceRelease(pMHLResource);

        MsOS_DelayTask(usPollingTaskInterval);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_CreateTask
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_mhl_CreateTask(MHL_RESOURCE_PRIVATE *pMHLResourcePrivate)
{
    MS_BOOL bCreateSuccess = TRUE;

    if(pMHLResourcePrivate->slMHLPollingTaskID < 0)
    {
        pMHLResourcePrivate->slMHLPollingTaskID = MsOS_CreateTask((TaskEntry) _mdrv_mhl_PollingTask,
                                     (MS_VIRT) NULL,
                                     E_TASK_PRI_MEDIUM,
                                     TRUE,
                                     (void *)pMHLResourcePrivate->ucMHLPollingStack,
                                     MHL_POLLING_STACK_SIZE,
                                     "MHL task");

        SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_TASK_PROCESS_FLAG);
    }

    if(pMHLResourcePrivate->slMHLPollingTaskID < 0)
    {
        MDRV_MHL_MSG_ERROR("MsOS_CreateTask failed!!\n");

        MsOS_DeleteEventGroup(pMHLResourcePrivate->slMHLPollingTaskID);

        bCreateSuccess = FALSE;
    }

    return bCreateSuccess;
}

#endif

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_IsrHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void _mdrv_mhl_IsrHandler(InterruptNum eIntNum)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bGetResourceFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bGetResourceFlag = TRUE;
    }

    UNUSED(eIntNum);

    //MsOS_DisableInterrupt(E_INT_IRQ_MHL_CBUS_PM);

    if(mhal_mhl_CbusIsMscMsgReceived() && bGetResourceFlag)
    {
        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSC_FLAG);
        CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_RECEIVE_LOSE_FLAG);

        pMHLResourcePrivate->ucReceiveCounter++;
    }

    if(mhal_mhl_CbusStucktoLowFlag())
    {
        mhal_mhl_CbusStucktoLow(FALSE);

        if(bGetResourceFlag)
        {
            mhal_mhl_CbusStucktoLowProc(pMHLResourcePrivate->ucMHLSupportPath);

            mhal_mhl_CDRModeMonitor(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));
        }
    }

    if(mhal_mhl_CbusWakeupIntFlag())
    {
        mhal_mhl_CbusWakeupInterrupt(FALSE);

        if(bGetResourceFlag)
        {
            SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WAKE_FLAG);
        }
    }

    if(bGetResourceFlag)
    {
        if(GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_INTERRUPT_USE_PM_IRQ_FLAG))
        {
            MsOS_EnableInterrupt(E_INT_IRQ_PM);
        }
        else
        {
            MsOS_EnableInterrupt(E_INT_IRQ_MHL_CBUS_PM);
        }
    }

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bGetResourceFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaModuleGetPtr(MODULE_MHL, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bGetResourceFlag = TRUE;
    }

    UNUSED(eIntNum);

    //MsOS_DisableInterrupt(E_INT_IRQ_MHL_CBUS_PM);

    if(mhal_mhl_CbusIsMscMsgReceived() && bGetResourceFlag)
    {
        SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_MSC_FLAG);
        CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_RECEIVE_LOSE_FLAG);

        pMHLResourcePrivate->ucReceiveCounter++;
    }

    if(mhal_mhl_CbusStucktoLowFlag())
    {
        mhal_mhl_CbusStucktoLow(FALSE);

        if(bGetResourceFlag)
        {
            mhal_mhl_CbusStucktoLowProc(pMHLResourcePrivate->ucMHLSupportPath);

            mhal_mhl_CDRModeMonitor(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));
        }
    }

    if(mhal_mhl_CbusWakeupIntFlag())
    {
        mhal_mhl_CbusWakeupInterrupt(FALSE);

        if(bGetResourceFlag)
        {
            SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WAKE_FLAG);
        }
    }

    if(bGetResourceFlag)
    {
        if(GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_INTERRUPT_USE_PM_IRQ_FLAG))
        {
            MsOS_EnableInterrupt(E_INT_IRQ_PM);
        }
        else
        {
            MsOS_EnableInterrupt(E_INT_IRQ_MHL_CBUS_PM);
        }
    }

    UtopiaResourceRelease(pMHLResource);

#endif
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_mhl_ECbusIsrHandler()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//
//**************************************************************************
static void _mdrv_mhl_ECbusIsrHandler(InterruptNum eIntNum)
{
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
    MS_BOOL bGetResourceFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bGetResourceFlag = TRUE;
    }

    UNUSED(eIntNum);

    if(bGetResourceFlag)
    {
        if(mhal_mhl_GetECbusStateChangeFlag(&(pMHLResourcePrivate->stSignalStatusInfo), &(pMHLResourcePrivate->stBISTParameterInfo)))
        {

        }
    }
    else
    {
        if(mhal_mhl_GetECbusStateChangeFlag(NULL, NULL))
        {

        }
    }

    if(mhal_mhl_GetEMSCReceiveFlag() && bGetResourceFlag)
    {
        pMHLResourcePrivate->ucEMSCReceiveCounter++;
    }

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bGetResourceFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaModuleGetPtr(MODULE_MHL, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bGetResourceFlag = TRUE;
    }

    UNUSED(eIntNum);

    if(bGetResourceFlag)
    {
        if(mhal_mhl_GetECbusStateChangeFlag(&(pMHLResourcePrivate->stSignalStatusInfo), &(pMHLResourcePrivate->stBISTParameterInfo)))
        {

        }
    }
    else
    {
        if(mhal_mhl_GetECbusStateChangeFlag(NULL, NULL))
        {

        }
    }

    if(mhal_mhl_GetEMSCReceiveFlag() && bGetResourceFlag)
    {
        pMHLResourcePrivate->ucEMSCReceiveCounter++;
    }

    UtopiaResourceRelease(pMHLResource);

#endif

    MsOS_EnableInterrupt(E_INT_IRQ_MHL_ECBUS_INT);
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
#if(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_10)
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_STREventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_mhl_STREventProc(EN_POWER_MODE usPowerState)
{
    MS_U32 ulReturnValue = UTOPIA_STATUS_FAIL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(pMHLResourcePrivate->usPrePowerState != usPowerState)
        {
            if(usPowerState == E_POWER_SUSPEND)
            {
                if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG))
                {
                    _mdrv_mhl_ForceSendClearHPD(pMHLResourcePrivate);
                }

                ulReturnValue = UTOPIA_STATUS_SUCCESS;
            }
            else if(usPowerState == E_POWER_RESUME)
            {
                pMHLResourcePrivate->ucChipCapability = mhal_mhl_initial(pMHLResourcePrivate->ucMHLSupportPath, NULL, NULL, pMHLResourcePrivate->ucDeviceVenderID);

                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_NORMAL_FLAG);

                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
                {
                    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
                    {
                        mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));

                        mhal_mhl_CbusConnectProc(pMHLResourcePrivate->ucMHLSupportPath);

                        if(pMHLResourcePrivate->ucTMDSClockMode == MHL_PHY_CLOCK_PACKET_PIXEL)
                        {
                            mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, TRUE, &(pMHLResourcePrivate->stSignalStatusInfo));
                        }
                    }
                    else
                    {
                        mhal_mhl_CablePlugProc(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stSignalStatusInfo));
                    }
                }

                ulReturnValue = UTOPIA_STATUS_SUCCESS;
            }

            pMHLResourcePrivate->usPrePowerState = usPowerState;
        }
    }

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_MHLSupportPath()
//  [Description]
//                  MHL support path
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_MHLSupportPath(MS_U8 ucSelect)
{
    MS_BOOL bMHLEfuseFlag = TRUE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        // Clear flag
        if(pMHLResourcePrivate->ucInitialIndex != 0xA5)
        {
            _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_INITIAL_FLAG);

            pMHLResourcePrivate->ucInitialIndex = 0xA5;

            if(_mdrv_mhl_InitRiuBase(pMHLResourcePrivate)) // Get base success
            {
                MDRV_MHL_MSG_INFO("** MHL get XC and PM base success \n");

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG);
            }
        }
        else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
        {
            mhal_mhl_SetRegisterBaseAddress(pMHLResourcePrivate->ulRiuBaseAddress, pMHLResourcePrivate->ulPMRiuBaseAddress);
        }

        mhal_mhl_MHLSupportPath(ucSelect);

        bMHLEfuseFlag = MDrv_SYS_Query(E_SYS_QUERY_MHL_SUPPORTED);

        if(mhal_mhl_CheckEfuseControlFlag(bMHLEfuseFlag))
        {
            MDRV_MHL_MSG_INFO("** MHL function not support in this chip ###\r\n");
        }
        else
        {
            for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK ;uctemp++)
            {
                if(ucSelect &BIT(uctemp))
                {
                    pMHLResourcePrivate->ucMHLSupportPath = uctemp;

                    MDRV_MHL_MSG_INFO("** MHL function support in port %c\r\n", MHL_INPUT_PORT(uctemp));
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_Initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//                  *edid: MHL EDID data
//                  *devcap: MHL device capability
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_Initial(MS_U8 *edid, MS_U8 *DevCap)
{
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        // Clear flag
        if(pMHLResourcePrivate->ucInitialIndex != 0xA5)
        {
            _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_INITIAL_FLAG);

            pMHLResourcePrivate->ucInitialIndex = 0xA5;
        }

        if(_mdrv_mhl_InitRiuBase(pMHLResourcePrivate)) // Get base success
        {
            MDRV_MHL_MSG_INFO("** MHL get XC and PM base success \n");

            SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG);
        }

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
        {
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_PREVENT_REINITIAL_FLAG))
            {
                // Chip initial
                pMHLResourcePrivate->ucChipCapability = mhal_mhl_initial(pMHLResourcePrivate->ucMHLSupportPath, edid, DevCap, pMHLResourcePrivate->ucDeviceVenderID);

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_PREVENT_REINITIAL_FLAG);
            }
            else
            {
                // Load EDID
                mhal_mhl_LoadEDID(edid);

                // Load vendor ID
                mhal_mhl_SetVenderID(pMHLResourcePrivate->ucDeviceVenderID);

                // Load DevCap
                mhal_mhl_LoadDeviceCapability(DevCap);
            }

            // Parsing EDID
            _mdrv_mhl_ParsingEDIDfor3D(pMHLResourcePrivate, edid);

            // Check and insert device capability
            if(DevCap != NULL)
            {
                for(uctemp = 0; uctemp < MHL_DEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    pMHLResourcePrivate->ucDeviceCapabilityTable[uctemp] = DevCap[uctemp];
                }

                if(_mdrv_mhl_GetEnhanceCbusFlag(pMHLResourcePrivate)) // Chip support MHL 3.0
                {
                    SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG);

                    for(uctemp = 0; uctemp < MHL_XDEVICE_CAPABILITY_SIZE; uctemp++)
                    {
                        pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_DEVICE_CAPABILITY_SIZE +uctemp] = DevCap[MHL_DEVICE_CAPABILITY_SIZE +uctemp];
                    }
                }
                else if(DevCap[MHL_CBUS_VERSION] >= 0x30) // Change MHL version to 2.1
                {
                    pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_VERSION] = 0x21;

                    mhal_mhl_LoadDeviceCapability(pMHLResourcePrivate->ucDeviceCapabilityTable);

                    MDRV_MHL_MSG_INFO("** MHL this chip not supprt MHL 3.0 \r\n");
                }
            }

            // Attach Cbus ISR
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ISR_ATTACH_FLAG))
            {
                if(GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_INTERRUPT_USE_PM_IRQ_FLAG))
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_PM, _mdrv_mhl_IsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_PM);
                }
                else
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_MHL_CBUS_PM, _mdrv_mhl_IsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_MHL_CBUS_PM);
                }

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ISR_ATTACH_FLAG);
            }

            // Attach eCbus ISR
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG))
            {
                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_MHL_ECBUS_INT, _mdrv_mhl_ECbusIsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_MHL_ECBUS_INT);

                    SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG);
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_Handler()
//  [Description]
//                  MHL handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 mdrv_mhl_Handler(void)
{
    MS_U16 usMSGKeyInfo = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        _mdrv_mhl_PollingEventProc(pMHLResourcePrivate);

        if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG))
        {
            CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);

            usMSGKeyInfo = pMHLResourcePrivate->usMSGKeyInfo;
        }
    }

    return usMSGKeyInfo;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_AutoSwitchHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_AutoSwitchHandler(MS_BOOL bReset, MS_U8 *ucCbusPath)
{
    MS_BOOL bAutoSwitchFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bReset)
        {
            CLR_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG);
            CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
        }
        else
        {
            if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG))
            {
                CLR_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG);

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_AUTO_SWITCH_STOP, MHL_AUTO_SWITCH_STOP_TIME);

                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_STOP_FLAG);

                bAutoSwitchFlag = TRUE;
            }
            else if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_STOP_FLAG))
            {
                if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG))
                {
                    if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
                    {
                        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
                    }
                }
            }
        }
    }

    return bAutoSwitchFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetPowerState()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_mhl_SetPowerState(EN_POWER_MODE usPowerState)
{
    return mdrv_mhl_STREventProc(usPowerState);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CbusControl(MS_U8 ucState)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        switch(ucState)
        {
            case MHL_CBUS_FORCE_READY_DEVCAP:
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG);
                break;

            case MHL_CBUS_FORCE_RECEIVE_3D_REQ:
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_3D_REQ_FLAG);
                break;

            case MHL_CBUS_FORCE_SEND_COMMAND_ENABLE:
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_FORCE_SEND_COMMAND_FLAG);
                break;

            case MHL_CBUS_FORCE_SEND_COMMAND_DISABLE:
                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_FORCE_SEND_COMMAND_FLAG);
                break;

            case MHL_CBUS_CHECK_CONTEND_ON_ENABLE:
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CHECK_CONTENT_ENABLE_FLAG);
                break;

            case MHL_CBUS_CHECK_CONTEND_ON_DISABLE:
                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CHECK_CONTENT_ENABLE_FLAG);
                break;

            case MHL_CBUS_ISOLATION_ON:
                mhal_mhl_CbusIsolate(pMHLResourcePrivate->ucMHLSupportPath, TRUE);
                break;

            case MHL_CBUS_ISOLATION_OFF:
                mhal_mhl_CbusIsolate(pMHLResourcePrivate->ucMHLSupportPath, FALSE);
                break;

            case MHL_CBUS_FLOATING_ON:
                mhal_mhl_CbusFloating(TRUE);
                break;

            case MHL_CBUS_FLOATING_OFF:
                mhal_mhl_CbusFloating(FALSE);
                break;

            case MHL_VBUS_HW_DETCET:
                mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_HW_DETECT);
                break;

            case MHL_VBUS_CHARGE_ON:
                mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_SW_CHARGE);
                break;

            case MHL_VBUS_CHARGE_OFF:
                mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_SW_UNCHARGE);
                break;

            default:

                break;
        };
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_LoadEDID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_LoadEDID(MS_U8 *edid)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        mhal_mhl_LoadEDID(edid);

        // Parsing EDID
        _mdrv_mhl_ParsingEDIDfor3D(pMHLResourcePrivate, edid);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_ReadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(usSize > 0)
        {
            mhal_mhl_ReadEDID(usSize, edid);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_LoadDeviceCapability()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_LoadDeviceCapability(MS_U8 *devcap)
{
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(devcap != NULL)
        {
            for(uctemp = 0; uctemp < MHL_DEVICE_CAPABILITY_SIZE; uctemp++)
            {
                pMHLResourcePrivate->ucDeviceCapabilityTable[uctemp] = devcap[uctemp];
            }

            if(_mdrv_mhl_GetEnhanceCbusFlag(pMHLResourcePrivate)) // Chip support MHL 3.0
            {
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG);

                for(uctemp = 0; uctemp < MHL_XDEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_DEVICE_CAPABILITY_SIZE +uctemp] = devcap[MHL_DEVICE_CAPABILITY_SIZE +uctemp];
                }
            }
            else if(devcap[MHL_CBUS_VERSION] >= 0x30) // Change MHL version to 2.1
            {
                pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_VERSION] = 0x21;

                mhal_mhl_LoadDeviceCapability(pMHLResourcePrivate->ucDeviceCapabilityTable);

                MDRV_MHL_MSG_INFO("** MHL this chip not supprt MHL 3.0 \r\n");
            }

            // Attach eCbus ISR
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG))
            {
                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_MHL_ECBUS_INT, _mdrv_mhl_ECbusIsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_MHL_ECBUS_INT);

                    SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG);
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetVenderID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_SetVenderID(MS_U8 ucVenderID)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        pMHLResourcePrivate->ucDeviceVenderID = ucVenderID;

        mhal_mhl_SetVenderID(ucVenderID);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_InvertCableDetect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_InvertCableDetect(MS_BOOL bCableDetectInvert)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        mhal_mhl_InvertCableDetect(pMHLResourcePrivate->ucMHLSupportPath, bCableDetectInvert);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_VbusConfigSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_VbusConfigSetting(MS_U8 ucState)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        mhal_mhl_VbusConfigSetting(pMHLResourcePrivate->ucMHLSupportPath, ucState);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_AdjustSettingIControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_AdjustSettingIControl(MS_U8 ucIControl)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        pMHLResourcePrivate->stSignalStatusInfo.ucIControlValue = ucIControl;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_AdjustImpedanceSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        pMHLResourcePrivate->stSignalStatusInfo.ucImpedanceOffset = ucImpedance;
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCableDetectFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_BOOL mdrv_mhl_GetCableDetectFlag(void)
{
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG);
    }
    
    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCbusConnectFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetCbusConnectFlag(void)
{
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
    }
    
    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusWakeupIntFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_CbusWakeupIntFlag(void)
{
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WAKE_FLAG);
    }
    
    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SrcRCPSupportFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_SrcRCPSupportFlag(void)
{
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_RCP_FUNCTION_FLAG);
    }

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SrcRAPSupportFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_SrcRAPSupportFlag(void)
{
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_RAP_FUNCTION_FLAG);
    }
    
    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusGetStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_CbusGetStatusFlag(MS_U8 ucState)
{
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        switch(ucState)
        {
            case MHL_CBUS_STATUS_FLAG_CABLE_DETECT:
                bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG);
                break;

            case MHL_CBUS_STATUS_FLAG_CBUS_CONNECT:
                bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
                break;

            case MHL_CBUS_STATUS_FLAG_PATH_EN:
                break;

            case MHL_CBUS_STATUS_FLAG_RECEIVE_PATH_EN:
                break;

            case MHL_CBUS_STATUS_FLAG_RECEIVE_MUTED:
                break;

            case MHL_CBUS_STATUS_FLAG_COMMUNICATE_BUSY:
                bStatusFlag = GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG);
                break;

            default:

                break;
        };
    }

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendRAPCommand
//  [Description]
//                  MHL Cbus MSC Send RAP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendRAPCommand(MS_U8 ucKeyCode)
{
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RAP, ucKeyCode, TRUE);
    }

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendRCPAutoReleaseCmd
//  [Description]
//                  MHL Cbus MSC Send RCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendRCPAutoReleaseCmd(MS_U8 ucKeyCode)
{
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(pMHLResourcePrivate->ucPreRCPKeyCode != ucKeyCode) // Key change
        {
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_RCP_RELEASE_ENABLE_FLAG))
            {
                bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RCP, ucKeyCode, TRUE);
            }
        }
        else
        {
            bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RCP, ucKeyCode, TRUE);
        }

        if(bSendFlag)
        {
            pMHLResourcePrivate->ucPreRCPKeyCode = ucKeyCode;

            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_RCP_AUTO_RELEASE, MHL_CBUS_RCP_RELEASE_TIME);

            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_RCP_RELEASE_ENABLE_FLAG);
        }
    }

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendUCPCommand
//  [Description]
//                  MHL Cbus MSC Send UCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendUCPCommand(MS_U8 ucKeyCode)
{
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_UCP, ucKeyCode, TRUE);
    }

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendWriteBurst
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendWriteBurst(MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *ucData)
{
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bSendFlag = _mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, ucAddr, ucLength, ucData);
    }

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendUserWriteBurst
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_CbusSendUserWriteBurst(MS_U8 ucLength, MS_U8 *ucData)
{
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG))
        {
            if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
            {
                if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, ucLength, ucData))
                {
                    CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG);

                    pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

                    bSendFlag = TRUE;
                }
            }
        }
        else if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_NONE)
        {
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG);
        }
    }
    
    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetDeviceCapability
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetDeviceCapability(MS_U16 usDevcapMask, MS_U8 *ucDevcap)
{
    MS_BOOL bDoneFlag = FALSE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_DEVCAP_FLAG))
        {
            pMHLResourcePrivate->usReadDevcapMask = usDevcapMask;
        }

        if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_READ_DEVCAP_DONE_FLAG))
        {
            if(ucDevcap != NULL)
            {
                for(uctemp = 0; uctemp < MHL_DEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    if(usDevcapMask &BIT(uctemp))
                    {
                        ucDevcap[uctemp] = pMHLResourcePrivate->ucSourceDevcapTable[uctemp];
                    }
                    else
                    {
                        ucDevcap[uctemp] = 0;
                    }
                }
            }

            bDoneFlag = TRUE;
        }
    }

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetExtendDeviceCapability
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetExtendDeviceCapability(MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap)
{
    MS_BOOL bDoneFlag = FALSE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_XDEVCAP_FLAG))
        {
            pMHLResourcePrivate->usReadExtendDevcapMask = usExtendDevcapMask;
        }

        if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_READ_XDEVCAP_DONE_FLAG))
        {
            if(ucExtendDevcap != NULL)
            {
                for(uctemp = 0; uctemp < MHL_XDEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    if(usExtendDevcapMask &BIT(uctemp))
                    {
                        ucExtendDevcap[uctemp] = pMHLResourcePrivate->ucSourceDevcapTable[MHL_DEVICE_CAPABILITY_SIZE +uctemp];
                    }
                    else
                    {
                        ucExtendDevcap[uctemp] = 0;
                    }
                }
            }

            bDoneFlag = TRUE;
        }
    }

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetDeviceVenderID
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetDeviceVenderID(MS_U8 *ucVenderID)
{
    MS_BOOL bDoneFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_GET_VENDER_ID_DONE_FLAG))
        {
            *ucVenderID = pMHLResourcePrivate->ucSourceVenderID;

            bDoneFlag = TRUE;
        }
        else
        {
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_GET_VENDER_ID_FLAG);
        }
    }

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetWriteBurstData
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetWriteBurstData(MS_U8 *ucData)
{
    MS_BOOL bDoneFlag = FALSE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(pMHLResourcePrivate->ucWriteBurstAddress > 0)
        {
            pMHLResourcePrivate->ucWriteBurstAddress = 0;

            for(uctemp = 0; uctemp < pMHLResourcePrivate->ucWriteBurstLength; uctemp++)
            {
                ucData[uctemp] = pMHLResourcePrivate->ucWriteBurstInformation[uctemp];
            }

            bDoneFlag = TRUE;
        }
    }

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterCallBackFunctions()
//  [Description]
//                  RCP &RAP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterCallBackFunctions(MS_BOOL bRCPfunctionFlag, MS_BOOL bRAPfunctionFlag)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRCPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RCP_FUNCTION_FLAG);
        }

        if(bRAPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RAP_FUNCTION_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterRcpCallBackFunction()
//  [Description]
//                  RCP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterRcpCallBackFunction(MS_BOOL bRCPfunctionFlag)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRCPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RCP_FUNCTION_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterRapCallBackFunction()
//  [Description]
//                  RAP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterRapCallBackFunction(MS_BOOL bRAPfunctionFlag)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRAPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RAP_FUNCTION_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterUcpCallBackFunction()
//  [Description]
//                  UCP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterUcpCallBackFunction(MS_BOOL bUCPfunctionFlag)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bUCPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_UCP_FUNCTION_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterAttCallBackFunction()
//  [Description]
//                  ATT call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterAttCallBackFunction(MS_BOOL bATTfunctionFlag)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bATTfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_ATT_FUNCTION_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterRbpCallBackFunction()
//  [Description]
//                  RBP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterRbpCallBackFunction(MS_BOOL bRBPfunctionFlag)
{
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRBPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RBP_FUNCTION_FLAG);
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetConfiguration()
//  [Description]
//                  MHL get init config
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
stMHL_INITIAL_CONFIG_INFO mdrv_mhl_GetConfiguration(void)
{
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;
    stMHL_INITIAL_CONFIG_INFO stInitialConfigInfo;

    if(!_mdrv_mhl_ResourceGetPrivate((void*)&pMHLResourcePrivate))
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(_mdrv_mhl_InitRiuBase(pMHLResourcePrivate)) // Get base success
        {
            MDRV_MHL_MSG_INFO("** MHL get XC and PM base success \n");
        }

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_PREVENT_REINITIAL_FLAG))
        {
            stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_SUCCESS;
            stInitialConfigInfo.ucMHLSupportPath = pMHLResourcePrivate->ucMHLSupportPath;

            for(uctemp = 0; uctemp < (MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE); uctemp++)
            {
                stInitialConfigInfo.ucDeviceCapabilityTable[uctemp] = pMHLResourcePrivate->ucDeviceCapabilityTable[uctemp];
            }
        }
    }

    return stInitialConfigInfo;
}

#elif(MHL_USE_UTOPIA_VERSION == MHL_UTOPIA_VERSION_20)
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_STREventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_mhl_STREventProc(void* pModule, EN_POWER_MODE usPowerState)
{
    void* pMHLResource = NULL;
    MS_U32 ulReturnValue = UTOPIA_STATUS_FAIL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(pMHLResourcePrivate->usPrePowerState != usPowerState)
        {
            if(usPowerState == E_POWER_SUSPEND)
            {
                if(GET_MHL_DISPLAY_FLAG(pMHLResourcePrivate->usDisplayIndex, MHL_CBUS_HPD_SET_FLAG))
                {
                    _mdrv_mhl_ForceSendClearHPD(pMHLResourcePrivate);
                }

                ulReturnValue = UTOPIA_STATUS_SUCCESS;
            }
            else if(usPowerState == E_POWER_RESUME)
            {
#if defined(MSOS_TYPE_LINUX_KERNEL)
                UtopiaStrWaitCondition("mhl", usPowerState, 0);
#endif
                pMHLResourcePrivate->ucChipCapability = mhal_mhl_initial(pMHLResourcePrivate->ucMHLSupportPath, NULL, NULL, pMHLResourcePrivate->ucDeviceVenderID);

                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_NORMAL_FLAG);

                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG))
                {
                    if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
                    {
                        mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, FALSE, &(pMHLResourcePrivate->stSignalStatusInfo));

                        mhal_mhl_CbusConnectProc(pMHLResourcePrivate->ucMHLSupportPath);

                        if(pMHLResourcePrivate->ucTMDSClockMode == MHL_PHY_CLOCK_PACKET_PIXEL)
                        {
                            mhal_mhl_ClockModeSwitchProc(pMHLResourcePrivate->ucMHLSupportPath, TRUE, &(pMHLResourcePrivate->stSignalStatusInfo));
                        }
                    }
                    else
                    {
                        mhal_mhl_CablePlugProc(pMHLResourcePrivate->ucMHLSupportPath, &(pMHLResourcePrivate->stSignalStatusInfo));
                    }
                }

                ulReturnValue = UTOPIA_STATUS_SUCCESS;
            }

            pMHLResourcePrivate->usPrePowerState = usPowerState;
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_MHLSupportPath()
//  [Description]
//                  MHL support path
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_MHLSupportPath(void* pInstance, MS_U8 ucSelect)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bMHLEfuseFlag = TRUE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        // Clear flag
        if(pMHLResourcePrivate->ucInitialIndex != 0xA5)
        {
            _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_INITIAL_FLAG);

            pMHLResourcePrivate->ucInitialIndex = 0xA5;

            if(_mdrv_mhl_InitRiuBase(pMHLResourcePrivate)) // Get base success
            {
                MDRV_MHL_MSG_INFO("** MHL get XC and PM base success \n");

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG);
            }
        }
        else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
        {
            mhal_mhl_SetRegisterBaseAddress(pMHLResourcePrivate->ulRiuBaseAddress, pMHLResourcePrivate->ulPMRiuBaseAddress);
        }

        mhal_mhl_MHLSupportPath(ucSelect);

        bMHLEfuseFlag = MDrv_SYS_Query(E_SYS_QUERY_MHL_SUPPORTED);

        if(mhal_mhl_CheckEfuseControlFlag(bMHLEfuseFlag))
        {
            MDRV_MHL_MSG_INFO("** MHL function not support in this chip ###\r\n");
        }
        else
        {
            for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK ;uctemp++)
            {
                if(ucSelect &BIT(uctemp))
                {
                    pMHLResourcePrivate->ucMHLSupportPath = uctemp;

                    MDRV_MHL_MSG_INFO("** MHL function support in port %c\r\n", MHL_INPUT_PORT(uctemp));
                }
            }
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_Initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//                  *edid: MHL EDID data
//                  *devcap: MHL device capability
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_Initial(void* pInstance, MS_U8 *edid, MS_U8 *DevCap)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        // Clear flag
        if(pMHLResourcePrivate->ucInitialIndex != 0xA5)
        {
            _mdrv_mhl_ClearCbusFlag(pMHLResourcePrivate, MHL_CLEAR_INITIAL_FLAG);

            pMHLResourcePrivate->ucInitialIndex = 0xA5;
        }

        if(_mdrv_mhl_InitRiuBase(pMHLResourcePrivate)) // Get base success
        {
            MDRV_MHL_MSG_INFO("** MHL get XC and PM base success \n");

            SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG);
        }

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
        {
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_PREVENT_REINITIAL_FLAG))
            {
                // Chip initial
                pMHLResourcePrivate->ucChipCapability = mhal_mhl_initial(pMHLResourcePrivate->ucMHLSupportPath, edid, DevCap, pMHLResourcePrivate->ucDeviceVenderID);

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_PREVENT_REINITIAL_FLAG);
            }
            else
            {
                // Load EDID
                mhal_mhl_LoadEDID(edid);

                // Load vendor ID
                mhal_mhl_SetVenderID(pMHLResourcePrivate->ucDeviceVenderID);

                // Load DevCap
                mhal_mhl_LoadDeviceCapability(DevCap);
            }

            // Parsing EDID
            _mdrv_mhl_ParsingEDIDfor3D(pMHLResourcePrivate, edid);

            // Check and insert device capability
            if(DevCap != NULL)
            {
                for(uctemp = 0; uctemp < MHL_DEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    pMHLResourcePrivate->ucDeviceCapabilityTable[uctemp] = DevCap[uctemp];
                }

                if(_mdrv_mhl_GetEnhanceCbusFlag(pMHLResourcePrivate)) // Chip support MHL 3.0
                {
                    SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG);

                    for(uctemp = 0; uctemp < MHL_XDEVICE_CAPABILITY_SIZE; uctemp++)
                    {
                        pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_DEVICE_CAPABILITY_SIZE +uctemp] = DevCap[MHL_DEVICE_CAPABILITY_SIZE +uctemp];
                    }
                }
                else if(DevCap[MHL_CBUS_VERSION] >= 0x30) // Change MHL version to 2.1
                {
                    pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_VERSION] = 0x21;

                    mhal_mhl_LoadDeviceCapability(pMHLResourcePrivate->ucDeviceCapabilityTable);

                    MDRV_MHL_MSG_INFO("** MHL this chip not supprt MHL 3.0 \r\n");
                }
            }

            // Attach Cbus ISR
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ISR_ATTACH_FLAG))
            {
                if(GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_INTERRUPT_USE_PM_IRQ_FLAG))
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_PM, _mdrv_mhl_IsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_PM);
                }
                else
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_MHL_CBUS_PM, _mdrv_mhl_IsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_MHL_CBUS_PM);
                }

                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ISR_ATTACH_FLAG);
            }

            // Attach eCbus ISR
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG))
            {
                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_MHL_ECBUS_INT, _mdrv_mhl_ECbusIsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_MHL_ECBUS_INT);

                    SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG);
                }
            }

            if(GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_SELF_CREATE_TASK_FLAG))
            {
                if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_SELF_CREATE_TASK_FLAG))
                {
                    if(_mdrv_mhl_CreateTask(pMHLResourcePrivate))
                    {
                        MDRV_MHL_MSG_INFO("** MHL create polling task by self \r\n");

                        SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_SELF_CREATE_TASK_FLAG);
                    }
                }
            }
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_Handler()
//  [Description]
//                  MHL handler
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 mdrv_mhl_Handler(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_U16 usMSGKeyInfo = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    
    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(!GET_MHL_CHIP_FLAG(pMHLResourcePrivate->ucChipCapability, MHL_CHIP_SELF_CREATE_TASK_FLAG))
        {
            _mdrv_mhl_PollingEventProc(pMHLResourcePrivate);
        }

        if(GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG))
        {
            CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_CALLBACK_FUNCTION_FLAG);

            usMSGKeyInfo = pMHLResourcePrivate->usMSGKeyInfo;
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return usMSGKeyInfo;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_AutoSwitchHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_AutoSwitchHandler(void* pInstance, MS_BOOL bReset, MS_U8 *ucCbusPath)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bAutoSwitchFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    
    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bReset)
        {
            CLR_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG);
            CLR_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
        }
        else
        {
            if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG))
            {
                CLR_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_AUTO_SWITCH_DONE_FLAG);

                _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_AUTO_SWITCH_STOP, MHL_AUTO_SWITCH_STOP_TIME);

                SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_STOP_FLAG);

                bAutoSwitchFlag = TRUE;
            }
            else if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_STOP_FLAG))
            {
                if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG))
                {
                    if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG))
                    {
                        SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_AUTO_SWITCH_FLAG);
                    }
                }
            }
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bAutoSwitchFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetPowerState()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_mhl_SetPowerState(void* pInstance, EN_POWER_MODE usPowerState)
{
    void* pModule = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    
    return mdrv_mhl_STREventProc(pModule, usPowerState);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_CbusControl(void* pInstance, MS_U8 ucState)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        switch(ucState)
        {
            case MHL_CBUS_FORCE_READY_DEVCAP:
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_DEVCAP_FLAG);
                break;

            case MHL_CBUS_FORCE_RECEIVE_3D_REQ:
                SET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_3D_REQ_FLAG);
                break;

            case MHL_CBUS_FORCE_SEND_COMMAND_ENABLE:
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_FORCE_SEND_COMMAND_FLAG);
                break;

            case MHL_CBUS_FORCE_SEND_COMMAND_DISABLE:
                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_FORCE_SEND_COMMAND_FLAG);
                break;

            case MHL_CBUS_CHECK_CONTEND_ON_ENABLE:
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CHECK_CONTENT_ENABLE_FLAG);
                break;

            case MHL_CBUS_CHECK_CONTEND_ON_DISABLE:
                CLR_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CHECK_CONTENT_ENABLE_FLAG);
                break;

            case MHL_CBUS_ISOLATION_ON:
                mhal_mhl_CbusIsolate(pMHLResourcePrivate->ucMHLSupportPath, TRUE);
                break;

            case MHL_CBUS_ISOLATION_OFF:
                mhal_mhl_CbusIsolate(pMHLResourcePrivate->ucMHLSupportPath, FALSE);
                break;

            case MHL_CBUS_FLOATING_ON:
                mhal_mhl_CbusFloating(TRUE);
                break;

            case MHL_CBUS_FLOATING_OFF:
                mhal_mhl_CbusFloating(FALSE);
                break;

            case MHL_VBUS_HW_DETCET:
                mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_HW_DETECT);
                break;

            case MHL_VBUS_CHARGE_ON:
                mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_SW_CHARGE);
                break;

            case MHL_VBUS_CHARGE_OFF:
                mhal_mhl_VbusCharge(pMHLResourcePrivate->ucMHLSupportPath, VBUS_SW_UNCHARGE);
                break;

            default:

                break;
        };
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_LoadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_LoadEDID(void* pInstance, MS_U8 *edid)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        mhal_mhl_LoadEDID(edid);

        // Parsing EDID
        _mdrv_mhl_ParsingEDIDfor3D(pMHLResourcePrivate, edid);
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_ReadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_ReadEDID(void* pInstance, MS_U16 usSize, MS_U8 *edid)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(usSize > 0)
        {
            mhal_mhl_ReadEDID(usSize, edid);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_LoadDeviceCapability()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_LoadDeviceCapability(void* pInstance, MS_U8 *devcap)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(devcap != NULL)
        {
            for(uctemp = 0; uctemp < MHL_DEVICE_CAPABILITY_SIZE; uctemp++)
            {
                pMHLResourcePrivate->ucDeviceCapabilityTable[uctemp] = devcap[uctemp];
            }

            if(_mdrv_mhl_GetEnhanceCbusFlag(pMHLResourcePrivate)) // Chip support MHL 3.0
            {
                SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG);

                for(uctemp = 0; uctemp < MHL_XDEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_DEVICE_CAPABILITY_SIZE +uctemp] = devcap[MHL_DEVICE_CAPABILITY_SIZE +uctemp];
                }
            }
            else if(devcap[MHL_CBUS_VERSION] >= 0x30) // Change MHL version to 2.1
            {
                pMHLResourcePrivate->ucDeviceCapabilityTable[MHL_CBUS_VERSION] = 0x21;

                mhal_mhl_LoadDeviceCapability(pMHLResourcePrivate->ucDeviceCapabilityTable);

                MDRV_MHL_MSG_INFO("** MHL this chip not supprt MHL 3.0 \r\n");
            }

            // Attach eCbus ISR
            if(!GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG))
            {
                if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_ENHANCE_FLAG))
                {
                    MsOS_AttachInterrupt(E_INT_IRQ_MHL_ECBUS_INT, _mdrv_mhl_ECbusIsrHandler);
                    MsOS_EnableInterrupt(E_INT_IRQ_MHL_ECBUS_INT);

                    SET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_ECBUS_ISR_ATTACH_FLAG);
                }
            }
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SetVenderID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_SetVenderID(void* pInstance, MS_U8 ucVenderID)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        pMHLResourcePrivate->ucDeviceVenderID = ucVenderID;

        mhal_mhl_SetVenderID(ucVenderID);
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_InvertCableDetect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_InvertCableDetect(void* pInstance, MS_BOOL bCableDetectInvert)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        mhal_mhl_InvertCableDetect(pMHLResourcePrivate->ucMHLSupportPath, bCableDetectInvert);
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_VbusConfigSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_VbusConfigSetting(void* pInstance, MS_U8 ucState)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    
    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        mhal_mhl_VbusConfigSetting(pMHLResourcePrivate->ucMHLSupportPath, ucState);
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_AdjustSettingIControl()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_AdjustSettingIControl(void* pInstance, MS_U8 ucIControl)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        pMHLResourcePrivate->stSignalStatusInfo.ucIControlValue = ucIControl;
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_AdjustImpedanceSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_AdjustImpedanceSetting(void* pInstance, MS_U8 ucImpedance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        pMHLResourcePrivate->stSignalStatusInfo.ucImpedanceOffset = ucImpedance;
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCableDetectFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetCableDetectFlag(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG);
    }

    UtopiaResourceRelease(pMHLResource);
    
    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetCbusConnectFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetCbusConnectFlag(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
    }

    UtopiaResourceRelease(pMHLResource);

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusWakeupIntFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_CbusWakeupIntFlag(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_RECEIVE_FLAG(pMHLResourcePrivate->ulCbusReceiveIndex, MHL_CBUS_RECEIVE_WAKE_FLAG);
    }

    UtopiaResourceRelease(pMHLResource);

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SrcRCPSupportFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_SrcRCPSupportFlag(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_RCP_FUNCTION_FLAG);
    }

    UtopiaResourceRelease(pMHLResource);

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SrcRAPSupportFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_SrcRAPSupportFlag(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bStatusFlag = GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_RAP_FUNCTION_FLAG);
    }

    UtopiaResourceRelease(pMHLResource);

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusGetStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_CbusGetStatusFlag(void* pInstance, MS_U8 ucState)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bStatusFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        switch(ucState)
        {
            case MHL_CBUS_STATUS_FLAG_CABLE_DETECT:
                bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG);
                break;

            case MHL_CBUS_STATUS_FLAG_CBUS_CONNECT:
                bStatusFlag = GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG);
                break;

            case MHL_CBUS_STATUS_FLAG_PATH_EN:
                break;

            case MHL_CBUS_STATUS_FLAG_RECEIVE_PATH_EN:
                break;

            case MHL_CBUS_STATUS_FLAG_RECEIVE_MUTED:
                break;

            case MHL_CBUS_STATUS_FLAG_COMMUNICATE_BUSY:
                bStatusFlag = GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_COMMUNICATE_BUSY_FLAG);
                break;

            default:

                break;
        };
    }

    UtopiaResourceRelease(pMHLResource);

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendRAPCommand
//  [Description]
//                  MHL Cbus MSC Send RAP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendRAPCommand(void* pInstance, MS_U8 ucKeyCode)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RAP, ucKeyCode, TRUE);
    }

    UtopiaResourceRelease(pMHLResource);

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendRCPAutoReleaseCmd
//  [Description]
//                  MHL Cbus MSC Send RCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendRCPAutoReleaseCmd(void* pInstance, MS_U8 ucKeyCode)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(pMHLResourcePrivate->ucPreRCPKeyCode != ucKeyCode) // Key change
        {
            if(!GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_RCP_RELEASE_ENABLE_FLAG))
            {
                bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RCP, ucKeyCode, TRUE);
            }
        }
        else
        {
            bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_RCP, ucKeyCode, TRUE);
        }

        if(bSendFlag)
        {
            pMHLResourcePrivate->ucPreRCPKeyCode = ucKeyCode;

            _mdrv_mhl_SetCbusTimerEvent(pMHLResourcePrivate, MHL_TIMER_EVENT_RCP_AUTO_RELEASE, MHL_CBUS_RCP_RELEASE_TIME);

            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_RCP_RELEASE_ENABLE_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendUCPCommand
//  [Description]
//                  MHL Cbus MSC Send UCP Command
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendUCPCommand(void* pInstance, MS_U8 ucKeyCode)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bSendFlag = _mdrv_mhl_CbusSendSubMessage(pMHLResourcePrivate, MSC_MSG_UCP, ucKeyCode, TRUE);
    }

    UtopiaResourceRelease(pMHLResource);

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_SendWriteBurst
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_SendWriteBurst(void* pInstance, MS_U8 ucAddr, MS_U8 ucLength, MS_U8 *ucData)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        bSendFlag = _mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, ucAddr, ucLength, ucData);
    }

    UtopiaResourceRelease(pMHLResource);

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CbusSendUserWriteBurst
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//                  TRUE: success
//                  FALSE: fail
//**************************************************************************
MS_BOOL mdrv_mhl_CbusSendUserWriteBurst(void* pInstance, MS_U8 ucLength, MS_U8 *ucData)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bSendFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(GET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG))
        {
            if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_REVEIVE_GRANT)
            {
                if(_mdrv_mhl_CbusSendWriteBurst(pMHLResourcePrivate, 0x40, ucLength, ucData))
                {
                    CLR_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG);

                    pMHLResourcePrivate->ucWriteBurstState = MHL_CBUS_WRITE_BURST_SEND_DATA;

                    bSendFlag = TRUE;
                }
            }
        }
        else if(pMHLResourcePrivate->ucWriteBurstState == MHL_CBUS_WRITE_BURST_NONE)
        {
            SET_MHL_SEND_FLAG(pMHLResourcePrivate->ulCbusSendIndex, MHL_CBUS_SEND_USER_WRITE_BURST_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bSendFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetDeviceCapability
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetDeviceCapability(void* pInstance, MS_U16 usDevcapMask, MS_U8 *ucDevcap)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bDoneFlag = FALSE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_DEVCAP_FLAG))
        {
            pMHLResourcePrivate->usReadDevcapMask = usDevcapMask;
        }

        if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_READ_DEVCAP_DONE_FLAG))
        {
            if(ucDevcap != NULL)
            {
                for(uctemp = 0; uctemp < MHL_DEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    if(usDevcapMask &BIT(uctemp))
                    {
                        ucDevcap[uctemp] = pMHLResourcePrivate->ucSourceDevcapTable[uctemp];
                    }
                    else
                    {
                        ucDevcap[uctemp] = 0;
                    }
                }
            }

            bDoneFlag = TRUE;
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetExtendDeviceCapability
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetExtendDeviceCapability(void* pInstance, MS_U16 usExtendDevcapMask, MS_U8 *ucExtendDevcap)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bDoneFlag = FALSE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(!GET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_READ_XDEVCAP_FLAG))
        {
            pMHLResourcePrivate->usReadExtendDevcapMask = usExtendDevcapMask;
        }

        if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_READ_XDEVCAP_DONE_FLAG))
        {
            if(ucExtendDevcap != NULL)
            {
                for(uctemp = 0; uctemp < MHL_XDEVICE_CAPABILITY_SIZE; uctemp++)
                {
                    if(usExtendDevcapMask &BIT(uctemp))
                    {
                        ucExtendDevcap[uctemp] = pMHLResourcePrivate->ucSourceDevcapTable[MHL_DEVICE_CAPABILITY_SIZE +uctemp];
                    }
                    else
                    {
                        ucExtendDevcap[uctemp] = 0;
                    }
                }
            }

            bDoneFlag = TRUE;
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetDeviceVenderID
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetDeviceVenderID(void* pInstance, MS_U8 *ucVenderID)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bDoneFlag = FALSE;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(GET_MHL_DONE_FLAG(pMHLResourcePrivate->ucProcDoneIndex, MHL_GET_VENDER_ID_DONE_FLAG))
        {
            *ucVenderID = pMHLResourcePrivate->ucSourceVenderID;

            bDoneFlag = TRUE;
        }
        else
        {
            SET_MHL_ENABLE_FLAG(pMHLResourcePrivate->ulEnableIndex, MHL_ENABLE_GET_VENDER_ID_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetWriteBurstData
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_mhl_GetWriteBurstData(void* pInstance, MS_U8 *ucData)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_BOOL bDoneFlag = FALSE;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(pMHLResourcePrivate->ucWriteBurstAddress > 0)
        {
            pMHLResourcePrivate->ucWriteBurstAddress = 0;

            for(uctemp = 0; uctemp < pMHLResourcePrivate->ucWriteBurstLength; uctemp++)
            {
                ucData[uctemp] = pMHLResourcePrivate->ucWriteBurstInformation[uctemp];
            }

            bDoneFlag = TRUE;
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return bDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterCallBackFunctions()
//  [Description]
//                  RCP &RAP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterCallBackFunctions(void* pInstance, MS_BOOL bRCPfunctionFlag, MS_BOOL bRAPfunctionFlag)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);
    
    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRCPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RCP_FUNCTION_FLAG);
        }

        if(bRAPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RAP_FUNCTION_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterRcpCallBackFunction()
//  [Description]
//                  RCP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterRcpCallBackFunction(void* pInstance, MS_BOOL bRCPfunctionFlag)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRCPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RCP_FUNCTION_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterRapCallBackFunction()
//  [Description]
//                  RAP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterRapCallBackFunction(void* pInstance, MS_BOOL bRAPfunctionFlag)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRAPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RAP_FUNCTION_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterUcpCallBackFunction()
//  [Description]
//                  UCP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterUcpCallBackFunction(void* pInstance, MS_BOOL bUCPfunctionFlag)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bUCPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_UCP_FUNCTION_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterAttCallBackFunction()
//  [Description]
//                  ATT call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterAttCallBackFunction(void* pInstance, MS_BOOL bATTfunctionFlag)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bATTfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_ATT_FUNCTION_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_RegisterRbpCallBackFunction()
//  [Description]
//                  RBP call back
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_RegisterRbpCallBackFunction(void* pInstance, MS_BOOL bRBPfunctionFlag)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_GET_BASE_SUCCESS_FLAG))
    {
        if(bRBPfunctionFlag)
        {
            SET_MHL_CALLBACK_FLAG(pMHLResourcePrivate->ucCallBackFunctionIndex, MHL_CALLBACK_RBP_FUNCTION_FLAG);
        }
    }

    UtopiaResourceRelease(pMHLResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_GetConfiguration()
//  [Description]
//                  MHL get init config
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
stMHL_INITIAL_CONFIG_INFO mdrv_mhl_GetConfiguration(void* pInstance)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MS_U8 uctemp = 0;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;
    stMHL_INITIAL_CONFIG_INFO stInitialConfigInfo = {0};

    stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_ERR_NOT_AVAIL;

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(_mdrv_mhl_InitRiuBase(pMHLResourcePrivate)) // Get base success
        {
            MDRV_MHL_MSG_INFO("** MHL get XC and PM base success \n");
        }

        if(GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_PREVENT_REINITIAL_FLAG))
        {
            stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_SUCCESS;
            stInitialConfigInfo.ucMHLSupportPath = pMHLResourcePrivate->ucMHLSupportPath;

            for(uctemp = 0; uctemp < (MHL_DEVICE_CAPABILITY_SIZE +MHL_XDEVICE_CAPABILITY_SIZE); uctemp++)
            {
                stInitialConfigInfo.ucDeviceCapabilityTable[uctemp] = pMHLResourcePrivate->ucDeviceCapabilityTable[uctemp];
            }
        }
    }

    UtopiaResourceRelease(pMHLResource);

    return stInitialConfigInfo;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_MDCMDEchoCommand()
//  [Description]
//                  MHL MDCMD echo command function
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine)
{
    char pch[] = "=,";
    char* psep;

    MdbPrint(u64ReqHdl, "LINE:%d, MDBCMD_CMDLINE\n", __LINE__);
    MdbPrint(u64ReqHdl, "pcCmdLine: %s\n", pcCmdLine);

    psep = strsep(&pcCmdLine, pch);

    if(strncmp("MHLOn", psep, 5) == 0)
    {

    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_MDCMDGetInfo()
//  [Description]
//                  MHL MDCMD get info function
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_mhl_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl)
{
    void* pModule = NULL;
    void* pMHLResource = NULL;
    MHL_RESOURCE_PRIVATE *pMHLResourcePrivate = NULL;

    if(UtopiaResourceObtain(pModule, MHL_POOL, &pMHLResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pMHLResource, (void*)&pMHLResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_MHL_MSG_ERROR("[%s,%5d] Get MHL resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        MdbPrint(u64ReqHdl, "LINE:%d, MDBCMD_GETINFO\n", __LINE__);
        MdbPrint(u64ReqHdl, "----------------  Mstar MHL Info -----------------\n");
        MdbPrint(u64ReqHdl, "Debug Cbus:\n");
        MdbPrint(u64ReqHdl, "Cable detect flag:    %d\n", GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CABLE_PLUG_FLAG));
        MdbPrint(u64ReqHdl, "Cbus connect flag:    %d\n", GET_MHL_SYSTEM_FLAG(pMHLResourcePrivate->ulSystemIndex, MHL_SYSTEM_CBUS_CONNECT_FLAG));
        MdbPrint(u64ReqHdl, "System index info:  0x%x\n", pMHLResourcePrivate->ulSystemIndex);
        MdbPrint(u64ReqHdl, "Timer index info:   0x%x\n", pMHLResourcePrivate->ulTimerIndex);
        MdbPrint(u64ReqHdl, "Enable index info:  0x%x\n", pMHLResourcePrivate->ulEnableIndex);
        MdbPrint(u64ReqHdl, "Cbus receive info:  0x%x\n", pMHLResourcePrivate->ulCbusReceiveIndex);
        MdbPrint(u64ReqHdl, "Cbus send info:     0x%x\n", pMHLResourcePrivate->ulCbusSendIndex);
        MdbPrint(u64ReqHdl, "Debug eCbus:\n");
        MdbPrint(u64ReqHdl, "eCbus state:          %d\n", pMHLResourcePrivate->ucECbusState);
        MdbPrint(u64ReqHdl, "Debug PHY:\n");
        MdbPrint(u64ReqHdl, "TMDS clock mode:      %d\n", pMHLResourcePrivate->ucTMDSClockMode);
        MdbPrint(u64ReqHdl, "Main link rate:       %d\n", pMHLResourcePrivate->stSignalStatusInfo.ucMainLinkRate);
        MdbPrint(u64ReqHdl, "Clock stable flag:    %d\n", pMHLResourcePrivate->stSignalStatusInfo.bClockStableFlag);
        MdbPrint(u64ReqHdl, "Signal stable flag:   %d\n", pMHLResourcePrivate->stSignalStatusInfo.bSignalStableFlag);
    }

    UtopiaResourceRelease(pMHLResource);
}

#endif

#endif

#endif // _MDRV_MHL_C_
