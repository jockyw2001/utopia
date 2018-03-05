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
//==============================================================================


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#define _DRV_CEC_C_

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "cec_hwreg_utility2.h"
#include "cec_Analog_Reg.h"

#include "apiCEC_private.h"
#include "apiCEC.h"
#include "drvCEC.h"
#include "drvMMIO.h"
#include "mhal_CEC.h"
#include "drvIPAUTH.h"

#ifndef _DRV_SYS_H_
#include "drvSYS.h"
#endif

#include "utopia.h"
#include "utopia_dapi.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MDRV_CEC_MSG_INFO(format, args...)      //ULOGI("CEC", format, ##args)
#define MDRV_CEC_MSG_WARNING(format, args...)   ULOGW("CEC", format, ##args)
#define MDRV_CEC_MSG_DEBUG(format, args...)     ULOGD("CEC", format, ##args)
#define MDRV_CEC_MSG_ERROR(format, args...)     ULOGE("CEC", format, ##args)
#define MDRV_CEC_MSG_FATAL(format, args...)     ULOGF("CEC", format, ##args)

#else
#define MDRV_CEC_MSG_INFO(format, args...)      //printf(format, ##args)
#define MDRV_CEC_MSG_WARNING(format, args...)   printf(format, ##args)
#define MDRV_CEC_MSG_DEBUG(format, args...)     printf(format, ##args)
#define MDRV_CEC_MSG_ERROR(format, args...)     printf(format, ##args)
#define MDRV_CEC_MSG_FATAL(format, args...)     printf(format, ##args)

#endif

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//MS_VIRT CEC_RIU_BASE;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_init_riu_base()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _MDrv_CEC_init_riu_base(CEC_RESOURCE_PRIVATE *pCECResourcePrivate)
{
    MS_VIRT _XCRIUBaseAddress = 0, _PMRIUBaseAddress = 0;
    MS_PHY u32NonPMBankSize = 0, u32PMBankSize = 0;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &_XCRIUBaseAddress, &u32NonPMBankSize, MS_MODULE_XC ) != TRUE)
    {
        MDRV_CEC_MSG_FATAL("CEC GetBase failed\n");
    }
    else if(MDrv_MMIO_GetBASE( &_PMRIUBaseAddress, &u32PMBankSize, MS_MODULE_PM ) != TRUE)
    {
        MDRV_CEC_MSG_FATAL("CEC GetBase failed\n");
    }
    else
    {
        pCECResourcePrivate->ulXCRiuBaseAddress = _XCRIUBaseAddress;
        pCECResourcePrivate->ulPMRiuBaseAddress = _PMRIUBaseAddress;

        mhal_CEC_init_riu_base(pCECResourcePrivate->ulXCRiuBaseAddress, pCECResourcePrivate->ulPMRiuBaseAddress);

        pCECResourcePrivate->bRIUAddrReady = TRUE;
    }
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CecRxChkBuf()
//  [Description]
//                   Driver layer: Use to retrieve CEC message and store into CEC Rx buffer
//  [Arguments]:
//
//  [Return]:
//                   TRUE: While get the new CEC message
//                   FALSE: No new CEC message
//**************************************************************************
void _MDrv_CEC_ChkRxBuf(CEC_RESOURCE_PRIVATE *pCECResourcePrivate, MS_BOOL bGetResourceFlag)  //don't place print message in this function
{
    MS_U8 i;
   // MS_BOOL ret = FALSE;
    MS_U8 hashinfo[18];
    MS_U8 temp1[11];
    MS_U8 temp2[11];
    MS_U8 k;

    if(mhal_CEC_IsMessageReceived())
    {
        if(bGetResourceFlag)
        {
            pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength = mhal_CEC_ReceivedMessageLen();

            if((pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength>1) && (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength<17)) //1: polling message, 17: over size, not needed to handle
            {
                for(i = 0 ; i < pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength ; i++)
                {
                    pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[i] = mhal_CEC_GetMessageByte(i);
                }
                if(pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0] != mhal_CEC_HeaderSwap(pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0]) )
                {
                    pCECResourcePrivate->bCecMsgCnt++;
                    pCECResourcePrivate->CecFifoIdxE=((++pCECResourcePrivate->CecFifoIdxE)>=CEC_FIFO_NUM_COUNT)? 0 : pCECResourcePrivate->CecFifoIdxE;
                }
                else if( pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0] == 0xFF && pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[1] == E_MSG_STANDBY)//12.04 Albert-PT
                {
                    pCECResourcePrivate->bCecMsgCnt++;
                    pCECResourcePrivate->CecFifoIdxE=((++pCECResourcePrivate->CecFifoIdxE)>=CEC_FIFO_NUM_COUNT)? 0 : pCECResourcePrivate->CecFifoIdxE;
                }
                else if(pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0] == 0xFF && pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[1] == E_MSG_RC_REQUEST_ACTIVE_SOURCE)//2014.4.23 Cindy
                {
                    pCECResourcePrivate->bCecMsgCnt++;
                    pCECResourcePrivate->CecFifoIdxE=((++pCECResourcePrivate->CecFifoIdxE)>=CEC_FIFO_NUM_COUNT)? 0 : pCECResourcePrivate->CecFifoIdxE;
                }
                else if (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0] == mhal_CEC_HeaderSwap(pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0]))
                {
                    if((pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[1] == 0xA0) &&
                        (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[2] == 0x36) &&
                        (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[3] == 0x97) &&
                        (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[4] == 0x00) )
                        {
                            MDrv_AUTH_GetHashInfo (hashinfo);
                            temp1[0] = 0x00;
                            temp1[1] = hashinfo[0];
                            temp1[2] = hashinfo[1];
                            temp2[0] = 0x01;
                            temp2[1] = hashinfo[0];
                            temp2[2] = hashinfo[1];
                            for(i = 0; i< 8; i++)
                            {
                                temp1[3+i] = hashinfo[2+i];
                                temp2[3+i] = hashinfo[10+i];
                            }

                            k = mhal_CEC_SendFramex(0, 0xA0, temp1, 0x0B);
                            k = mhal_CEC_SendFramex(0, 0xA0, temp2, 0x0B);
                            MDRV_CEC_MSG_INFO("mhal_CEC_SendFramex: res = 0x[%x]\n", k);
                       }
                }
            }
        }

        mhal_CEC_ClearRxStatus();
    }

#if ENABLE_CEC_MULTIPLE
    if(mhal_CEC_IsMessageReceived2())
    {
        if(bGetResourceFlag)
        {
            pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength = mhal_CEC_ReceivedMessageLen2();

            if((pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength>1) && (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength<17)) //1: polling message, 17: over size, not needed to handle
            {
                for(i = 0 ; i < pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].ucLength ; i++)
                {
                    pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[i] = mhal_CEC_GetMessageByte2(i);
                }
                if( pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0] != mhal_CEC_HeaderSwap(pCECResourcePrivate->CecRxBufgCECInfo.CecFifoIdxE].tRxData[0]) )
                {
                    pCECResourcePrivate->bCecMsgCnt++;
                    pCECResourcePrivate->CecFifoIdxE=((++pCECResourcePrivate->CecFifoIdxE)>=CEC_FIFO_NUM_COUNT)? 0 : pCECResourcePrivate->CecFifoIdxE;
                }
                else if(pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[0] == 0xFF && pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxE].tRxData[1] == E_MSG_STANDBY)
                {
                    pCECResourcePrivate->bCecMsgCnt++;
                    pCECResourcePrivate->CecFifoIdxE=((++pCECResourcePrivate->CecFifoIdxE)>=CEC_FIFO_NUM_COUNT)? 0 : pCECResourcePrivate->CecFifoIdxE;
                }
            }
        }

        mhal_CEC_ClearRxStatus2();
    }
#endif
}

//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_InitVariable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _MDrv_CEC_InitVariable(CEC_RESOURCE_PRIVATE *pCECResourcePrivate)
{
    MS_U8 i = 0;
    MS_U8 j = 0;

    pCECResourcePrivate->bCECIsSource = MDrv_SYS_GetChipType();
    pCECResourcePrivate->bRIUAddrReady = FALSE;
    pCECResourcePrivate->CecFifoIdxS = 0;
    pCECResourcePrivate->CecFifoIdxE = 0;
    pCECResourcePrivate->bCecMsgCnt = 0;
    pCECResourcePrivate->bCecInitFinish = FALSE;
    pCECResourcePrivate->bCecISRAttach = FALSE;
    pCECResourcePrivate->bAttachDrvISR = TRUE;
    pCECResourcePrivate->ucRetryCount = RETRY_CNT;
    pCECResourcePrivate->ucVendorID[0] = 0x00;
    pCECResourcePrivate->ucVendorID[1] = 0x00;
    pCECResourcePrivate->ucVendorID[2] = 0x00;
    pCECResourcePrivate->usPrePowerState = 0;

    if(pCECResourcePrivate->bCECIsSource) // STB series
    {
        pCECResourcePrivate->MyLogicalAddress = E_LA_TUNER1; // Tuner
        pCECResourcePrivate->MyPhysicalAddress[0] = 0x10; //default (1,0,0,0)
        pCECResourcePrivate->MyPhysicalAddress[1] = 0x00;
        pCECResourcePrivate->MyDeviceType = E_DEVICE_TYPE_TUNER; //Tuner device
    }
    else // TV series
    {
        pCECResourcePrivate->MyLogicalAddress = E_LA_TV;       //TV
        pCECResourcePrivate->MyPhysicalAddress[0] = 0x00;      //default (0,0,0,0) for TV
        pCECResourcePrivate->MyPhysicalAddress[1] = 0x00;
        pCECResourcePrivate->MyDeviceType = E_DEVICE_TYPE_TV;    //TV device
    }

    pCECResourcePrivate->MyPowerStatus = E_MSG_PWRSTA_STANDBY2ON;
    pCECResourcePrivate->ActiveLogicalAddress = E_LA_TV;
    pCECResourcePrivate->ActiveDeviceType = E_DEVICE_TYPE_RESERVED;
    pCECResourcePrivate->ActivePowerStatus = E_MSG_PWRSTA_STANDBY;
    pCECResourcePrivate->ActivePhysicalAddress[0] = 0x00;
    pCECResourcePrivate->ActivePhysicalAddress[1] = 0x00;
    pCECResourcePrivate->ActiveDeviceCECVersion = CEC_VERSION_13a;

    for(i=0;i<CEC_FIFO_NUM_COUNT;i++)
    {
        pCECResourcePrivate->CecRxBuf[i].ucLength = 0;
        for(j=0;j<16;j++)
            pCECResourcePrivate->CecRxBuf[i].tRxData[j] = 0;
    }
    for(i=0;i<15;i++)
    {
        pCECResourcePrivate->CecDevicesExisted[i] = FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_SendFrame()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static MS_U8 _MDrv_CEC_SendFrame(MS_U8 header, MS_U8 opcode, MS_U8* operand, MS_U8 len)
{
    return (mhal_CEC_SendFrame(header, opcode, operand, len));
}

//******************************************************************************
//
//  [Function Name]:
//      CecPingApi
//  [Arguments]:
//      destination address (range:0x01~0x0E)
//  [Return]:
//      CEC_ERROR_CODE
//*******************************************************************************
CEC_ERROR_CODE _MDrv_CEC_PingApi(MsCEC_DEVICELA addr)
{
    MS_U8 res;
    CEC_ERROR_CODE error_code = E_CEC_FEATURE_ABORT;
    MS_U8* Oprend_ptr = NULL;

    if(addr>=0x0f)
    {
        error_code = E_CEC_FEATURE_ABORT;
        return error_code;
    }

    res = _MDrv_CEC_SendFrame( ((addr<<4)&0xF0) |(addr&0x0F), 0x00, Oprend_ptr, 0);
    MDRV_CEC_MSG_INFO("_MDrv_CEC_SendFrame: res = %u\n", res);

    if(res&E_CEC_TX_SUCCESS)
        error_code = E_CEC_TX_SUCCESS;
    else if(res&E_CEC_RF)
        error_code = E_CEC_RF;
    else if(res&E_CEC_LOST_ABT)
        error_code = E_CEC_LOST_ABT;
    else if(res&E_CEC_SYSTEM_BUSY)
        error_code = E_CEC_SYSTEM_BUSY;

    return error_code;
}

//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_FindMyAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _MDrv_CEC_FindMyAddress(CEC_RESOURCE_PRIVATE *pCECResourcePrivate)
{
    MDRV_CEC_MSG_INFO("\r\n CEC Find My address \r\n");

    if(pCECResourcePrivate->bCECIsSource)// STB series
    {
        if(_MDrv_CEC_PingApi(E_LA_TUNER1) == E_CEC_RF)
        {
            pCECResourcePrivate->MyLogicalAddress = E_LA_TUNER1;
        }
        else if(_MDrv_CEC_PingApi(E_LA_TUNER2) == E_CEC_RF)
        {
            pCECResourcePrivate->MyLogicalAddress = E_LA_TUNER2;
        }
        else if(_MDrv_CEC_PingApi(E_LA_TUNER3) == E_CEC_RF)
        {
            pCECResourcePrivate->MyLogicalAddress = E_LA_TUNER3;
        }
        else if(_MDrv_CEC_PingApi(E_LA_TUNER4) == E_CEC_RF)
        {
            pCECResourcePrivate->MyLogicalAddress = E_LA_TUNER4;
        }
        else if(_MDrv_CEC_PingApi(E_LA_FREE_USE) == E_CEC_RF)
            pCECResourcePrivate->MyLogicalAddress = E_LA_FREE_USE;
        else
            pCECResourcePrivate->MyLogicalAddress = E_LA_TUNER1;
    }
    else // TV series
    {
        if(_MDrv_CEC_PingApi(E_LA_TV) == E_CEC_RF)
        {
            pCECResourcePrivate->MyLogicalAddress = E_LA_TV;
        }
        else if(_MDrv_CEC_PingApi(E_LA_FREE_USE) == E_CEC_RF)
            pCECResourcePrivate->MyLogicalAddress = E_LA_FREE_USE;
        else
            pCECResourcePrivate->MyLogicalAddress = E_LA_TV;
    }
    mhal_CEC_SetMyAddress((MS_U8)pCECResourcePrivate->MyLogicalAddress);

    MDRV_CEC_MSG_INFO("\r\nMy logical address=%x\r\n", pCECResourcePrivate->MyLogicalAddress);
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CecCheckExistedDevices()
//  [Description]
//                  Driver layer: Use to check the existed CEC devices currently
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _MDrv_CEC_CheckExistDevices(CEC_RESOURCE_PRIVATE *pCECResourcePrivate)
{
    MS_U8 i, res;
    MS_U8* Oprend_ptr = NULL;

    MDRV_CEC_MSG_INFO("\r\n Existed CEC device \r\n");
    for (i=E_LA_TV; i<E_LA_UNREGISTERED; i++)
    {
    #if ENABLE_CUST01_CEC
        if(pCECResourcePrivate->bCECIsSource) // STB series
        {
            if(!((i==E_LA_TV) ||(i==E_LA_AUDIO_SYS)) )
                continue;
        }
        else // TV series
        {
            if(!((i==E_LA_RECORDER1) || (i==E_LA_TUNER1) || (i==E_LA_PLAYBACK1) || (i==E_LA_AUDIO_SYS) || (i==E_LA_PLAYBACK2) || (i==E_LA_PLYBACK3)))
                continue;
        }
    #endif // #if ENABLE_CUST01_CEC

        MDRV_CEC_MSG_INFO("CEC CheckDev:%u\n",i);

        // Just request a response.
        res = _MDrv_CEC_SendFrame( ((pCECResourcePrivate->MyLogicalAddress<<4)&0xF0) |(i&0x0F), 0x00, Oprend_ptr, 0);
        MDRV_CEC_MSG_INFO("_MDrv_CEC_SendFrame: res = %u\n", res);

        if (res&E_CEC_TX_SUCCESS)
        {
            pCECResourcePrivate->CecDevicesExisted[i] = TRUE;
            pCECResourcePrivate->ActiveLogicalAddress = (MsCEC_DEVICELA)i;
            MDRV_CEC_MSG_INFO("\r\n DEVICE ID= %d \r\n", i);
        }
        else
        {
            pCECResourcePrivate->CecDevicesExisted[i]= FALSE;
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_TxApi()
//  [Description]
//                   Driver layer: Use to send CEC message
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg:         CEC message
//                   operand_ptr: message parameters
//                   len:         parameter length
//  [Return]:
//                   error_code:  return status
//**************************************************************************
CEC_ERROR_CODE _MDrv_CEC_TxApi(MS_U8 ucInitiatorLA, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* operand_ptr, MS_U8 len)
{
    MS_U8 res, header;
    CEC_ERROR_CODE error_code = E_CEC_FEATURE_ABORT;

    header = ((ucInitiatorLA<<4)&0xF0)|(dst_address&0x0F);

    res=_MDrv_CEC_SendFrame(header, msg, operand_ptr, len);
    MDRV_CEC_MSG_INFO("_MDrv_CEC_SendFrame: res = 0x[%x]\n", res);

    if(res&E_CEC_TX_SUCCESS)
        error_code = E_CEC_TX_SUCCESS;
    else if(res&E_CEC_RF)
        error_code = E_CEC_RF;
    else if(res&E_CEC_LOST_ABT)
        error_code = E_CEC_LOST_ABT;
    else if(res&E_CEC_SYSTEM_BUSY)
        error_code = E_CEC_SYSTEM_BUSY;

#if ENABLE_CUST01_CEC
    if( (msg==E_MSG_UI_PRESS) && (error_code==E_CEC_TX_SUCCESS) )
    {
        res=_MDrv_CEC_SendFrame(header, E_MSG_UI_RELEASE, operand_ptr, 0);
        MDRV_CEC_MSG_INFO("_MDrv_CEC_SendFrame: res = %u\n", res);

        if(res&E_CEC_TX_SUCCESS)
            error_code = E_CEC_TX_SUCCESS;
        else if(res&E_CEC_RF)
            error_code = E_CEC_RF;
        else if(res&E_CEC_LOST_ABT)
            error_code = E_CEC_LOST_ABT;
        else if(res&E_CEC_SYSTEM_BUSY)
            error_code = E_CEC_SYSTEM_BUSY;
    }
#endif
    return error_code;
}

//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_TxApi2()
//  [Description]
//                   Driver layer: Use to send CEC message
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg:         CEC message
//                   operand_ptr: message parameters
//                   len:         parameter length
//  [Return]:
//                   error_code:  return status
//**************************************************************************
CEC_ERROR_CODE _MDrv_CEC_TxApi2(MS_U8 ucInitiatorLA, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* operand_ptr, MS_U8 len)
{
    MS_U8 res, header;
    CEC_ERROR_CODE error_code = E_CEC_FEATURE_ABORT;

    header = ((ucInitiatorLA<<4)&0xF0)|(dst_address&0x0F);

    res =_MDrv_CEC_SendFrame(header, msg, operand_ptr, len);
    MDRV_CEC_MSG_INFO("_MDrv_CEC_SendFrame: res = 0x[%x]\n", res);

    if(res&E_CEC_TX_SUCCESS)
        error_code = E_CEC_TX_SUCCESS;
    else if(res&E_CEC_RF)
        error_code = E_CEC_RF;
    else if(res&E_CEC_LOST_ABT)
        error_code = E_CEC_LOST_ABT;
    else if(res&E_CEC_SYSTEM_BUSY)
        error_code = E_CEC_SYSTEM_BUSY;

    return error_code;
}

//#if !CEC_DEVICE_IS_SOURCE// TV series
//**************************************************************************
//  [Function Name]:
//                   _MDrv_CEC_RxIsr()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void _MDrv_CEC_RxIsr(InterruptNum eIntNum)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL bGetResourceFlag = FALSE;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaModuleGetPtr(MODULE_CEC, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            bGetResourceFlag = TRUE;
        }
    }

    UNUSED(eIntNum);

    if(ENABLE_CEC_INT)
    {
        _MDrv_CEC_ChkRxBuf(pCECResourcePrivate, bGetResourceFlag);
    }

    UtopiaResourceRelease(pCECResource);

    MsOS_EnableInterrupt(E_INT_IRQ_CEC);
}
//#endif // TV series

//**************************************************************************
//  [Function Name]:
//                  _MDrv_CEC_Init()
//  [Description]
//                  Driver layer: CEC initiail sequence
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _MDrv_CEC_Init(CEC_RESOURCE_PRIVATE *pCECResourcePrivate, MS_U32 u32XTAL_CLK_Hz)
{
    if(pCECResourcePrivate->ucInitialIndex != 0xA5)
    {
        _MDrv_CEC_InitVariable(pCECResourcePrivate);

        pCECResourcePrivate->ucInitialIndex = 0xA5;
    }

    _MDrv_CEC_init_riu_base(pCECResourcePrivate);

    if(pCECResourcePrivate->bRIUAddrReady)
    {
        if(!pCECResourcePrivate->bCecInitFinish)
        {
            mhal_CEC_Init(u32XTAL_CLK_Hz, pCECResourcePrivate->MyLogicalAddress, pCECResourcePrivate->ucRetryCount);

            pCECResourcePrivate->bCecInitFinish = TRUE;
        }

        if((!pCECResourcePrivate->bCecISRAttach) && (pCECResourcePrivate->bAttachDrvISR))
        {
//#if !CEC_DEVICE_IS_SOURCE // TV series
#if ENABLE_CEC_INT
            MsOS_AttachInterrupt(E_INT_IRQ_CEC, _MDrv_CEC_RxIsr);
            MsOS_EnableInterrupt(E_INT_IRQ_CEC);
#endif
//#endif
            pCECResourcePrivate->bCecISRAttach = TRUE;
        }

        pCECResourcePrivate->MyPowerStatus = E_MSG_PWRSTA_ON;

        MDRV_CEC_MSG_INFO("** Complete CEC Initiation!! **\r\n");
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mdrv_CEC_STREventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_CEC_STREventProc(void* pModule, EN_POWER_MODE usPowerState)
{
    void* pCECResource = NULL;
    MS_U32 ulReturnValue = UTOPIA_STATUS_FAIL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if(pCECResourcePrivate->usPrePowerState != usPowerState)
            {
                if(usPowerState == E_POWER_SUSPEND)
                {
                    ulReturnValue = UTOPIA_STATUS_SUCCESS;
                }
                else if(usPowerState == E_POWER_RESUME)
                {
                    if(pCECResourcePrivate->bRIUAddrReady)
                    {
                        mhal_CEC_Init(pCECResourcePrivate->ulXTAL_CLK_Hz, pCECResourcePrivate->MyLogicalAddress, pCECResourcePrivate->ucRetryCount);
                    }

                    ulReturnValue = UTOPIA_STATUS_SUCCESS;
                }

                pCECResourcePrivate->usPrePowerState = usPowerState;
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

    return ulReturnValue;
}

MS_BOOL MDrv_CEC_DDC2BI_GetInfo(void* pInstance, MS_CEC_INFO_LIST *pInfo)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL bReturnValue = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucIndex = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if(pInfo != NULL)
            {
                bReturnValue = TRUE;

                pInfo->ActiveDeviceCECVersion = pCECResourcePrivate->ActiveDeviceCECVersion;
                pInfo->ActiveDeviceType = pCECResourcePrivate->ActiveDeviceType;
                pInfo->ActiveLogicalAddress =  pCECResourcePrivate->ActiveLogicalAddress;
                pInfo->ActivePowerStatus = pCECResourcePrivate->ActivePowerStatus;
                pInfo->bCecMsgCnt = pCECResourcePrivate->bCecMsgCnt;
                pInfo->CecFifoIdxE = pCECResourcePrivate->CecFifoIdxE;
                pInfo->CecFifoIdxS = pCECResourcePrivate->CecFifoIdxS;
                pInfo->fCecInitFinish = pCECResourcePrivate->bCecInitFinish;
#if(ENABLE_CEC_MULTIPLE)
                pInfo->IniLogicalAddress = pCECResourcePrivate->IniLogicalAddress;
                pInfo->MyDeviceType2 = pCECResourcePrivate->MyDeviceType2;
                pInfo->MyLogicalAddress2 = pCECResourcePrivate->MyLogicalAddress2;

#endif
                pInfo->MyDeviceType = pCECResourcePrivate->MyDeviceType;
                pInfo->MyLogicalAddress = pCECResourcePrivate->MyLogicalAddress;
                pInfo->MyPowerStatus = pCECResourcePrivate->MyPowerStatus;

                for(uctemp = 0; uctemp < CEC_DEVICE_EXIST_COUNT; uctemp++)
                {
                    if(uctemp < CEC_PHY_ADDR_COUNT)
                    {
                        pInfo->ActivePhysicalAddress[uctemp] = pCECResourcePrivate->ActivePhysicalAddress[uctemp];
                        pInfo->MyPhysicalAddress[uctemp] = pCECResourcePrivate->MyPhysicalAddress[uctemp];
#if(ENABLE_CEC_MULTIPLE)
                        pInfo->MyPhysicalAddress2[uctemp] = pCECResourcePrivate->MyPhysicalAddress2[uctemp];
#endif
                    }

                    if(uctemp < CEC_DEVICE_EXIST_COUNT)
                    {
                        pInfo->CecDevicesExisted[uctemp] = pCECResourcePrivate->CecDevicesExisted[uctemp];
                    }

                    if(uctemp < CEC_FIFO_NUM_COUNT)
                    {
                        pInfo->CecRxBuf[uctemp].ucLength = pCECResourcePrivate->CecRxBuf[uctemp].ucLength;

                        for(ucIndex = 0; ucIndex < CEC_FIFO_DATA_NUMBER; ucIndex++)
                        {
                            pInfo->CecRxBuf[uctemp].tRxData[ucIndex] = pCECResourcePrivate->CecRxBuf[uctemp].tRxData[ucIndex];
                        }
                    }
                }
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

    return bReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CEC_PortSelect()
//  [Description]
//                  Port Select
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_PortSelect(void* pInstance, MsCEC_INPUT_PORT InputPort)
{
    mhal_CEC_PortSelect(InputPort);
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CecExit()
//  [Description]
//                  Driver layer: CEC initiail sequence
//  [Arguments]:
//
//  [Return]:
//
//*************************************************************************
void MDrv_CEC_Exit(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if((pCECResourcePrivate->bCecISRAttach) && (pCECResourcePrivate->bAttachDrvISR))
            {
                // Release CEC
                MsOS_DetachInterrupt(E_INT_IRQ_CEC);
                pCECResourcePrivate->bCecISRAttach = FALSE;
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

    // Mask CEC interrupt
    mhal_CEC_INTEn(FALSE);

}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CecInit()
//  [Description]
//                  Driver layer: CEC set own logical address
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetMyLogicalAddress(void* pInstance, MsCEC_DEVICELA myLA)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->MyLogicalAddress = myLA;
            mhal_CEC_SetMyAddress((MS_U8)myLA);
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CEC_InitChip()
//  [Description]
//                  Driver layer: CEC initiail chip
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_InitChip(void* pInstance, MS_U32 u32XTAL_CLK_Hz)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            MDRV_CEC_MSG_INFO("** CEC init: MDrv_CEC_InitChip **\n");

            pCECResourcePrivate->ulXTAL_CLK_Hz = u32XTAL_CLK_Hz;
            _MDrv_CEC_Init(pCECResourcePrivate, pCECResourcePrivate->ulXTAL_CLK_Hz);
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Init()
//  [Description]
//                  CEC initial sequence
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_Init(void* pInstance, MS_U32 u32XTAL_CLK_Hz)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL bIsSource = FALSE;
    MS_BOOL bDeviceExisted[CEC_DEVICE_EXIST_COUNT] = {0};
    MS_U8 ucInitiatorLA = 0;
    MS_U8* pPowerStatus = 0;
    MsCEC_DEVICELA i;
#if !ENABLE_SW_CEC_WAKEUP
    MS_U8 u8PowerOnMode = 0;
#endif
	CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            _MDrv_CEC_Init(pCECResourcePrivate, u32XTAL_CLK_Hz);
            _MDrv_CEC_FindMyAddress(pCECResourcePrivate);       //logical address for this TV.
            _MDrv_CEC_CheckExistDevices(pCECResourcePrivate);

            bIsSource = pCECResourcePrivate->bCECIsSource;

#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            pPowerStatus = (MS_U8*)&(pCECResourcePrivate->MyPowerStatus);

            for (i=E_LA_TV; i<E_LA_UNREGISTERED; i++)
            {
                bDeviceExisted[i] = pCECResourcePrivate->CecDevicesExisted[i];
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

	// report power status to the active CEC devices
    for (i=E_LA_TV; i<E_LA_UNREGISTERED; i++)
    {
#if ENABLE_CUST01_CEC
        if(bIsSource)
        {
            if(!((i==E_LA_TV) ||(i==E_LA_AUDIO_SYS)) )
                continue;
        }
        else
        {
            if(!((i==E_LA_RECORDER1) || (i==E_LA_TUNER1) || (i==E_LA_PLAYBACK1) || (i==E_LA_AUDIO_SYS) || (i==E_LA_PLAYBACK2) || (i==E_LA_PLYBACK3)))
                continue;
        }
#endif // #if ENABLE_CUST01_CEC
        if(bDeviceExisted[i])
        {
            _MDrv_CEC_TxApi(ucInitiatorLA, i, E_MSG_PS_REPORT_POWER_STATUS, pPowerStatus, 1); // report TV power status
        }
    }

    //some dvd player may send the active source too early,
    //so we add here another request active source to get the proper cmd from player
    //to get the correct the HDMI port to switch to.
#if !ENABLE_SW_CEC_WAKEUP
    mdrv_pm_ctrl_read(CTRL_READ_CMD, CP_READ_POWERON_MODE, 1, &u8PowerOnMode);
    if(u8PowerOnMode == M_POWER_ON_BY_HDMI_CEC)
    {
        MDRV_CEC_MSG_INFO("cec wake up, then req active source\n");
        _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_RC_REQ_ACTIVE_SOURCE, (MS_U8*)E_MSG_FEATURE_ABORT, 0);
    }
#endif

}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CecCheckExistedDevices()
//  [Description]
//                  Driver layer: Use to check the existed CEC devices currently
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_CheckExistDevices(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            _MDrv_CEC_CheckExistDevices(pCECResourcePrivate);
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_NextDevice()
//  [Description]
//                   Set logical address to next active device
//  [Arguments]:
//
//  [Return]:
//                   Next CEC active device's logical address
//**************************************************************************
MsCEC_DEVICELA MDrv_CEC_NextDevice(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 uctemp = 0;
    MsCEC_DEVICELA eReturnValue = E_LA_UNREGISTERED;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            _MDrv_CEC_CheckExistDevices(pCECResourcePrivate);

            eReturnValue = pCECResourcePrivate->ActiveLogicalAddress;

            for(uctemp = (pCECResourcePrivate->ActiveLogicalAddress +1); uctemp < E_LA_UNREGISTERED; uctemp++)
            {
                if(pCECResourcePrivate->CecDevicesExisted[uctemp] && (uctemp != E_LA_TV) && (uctemp != E_LA_FREE_USE))
                {
                    eReturnValue = (MsCEC_DEVICELA)uctemp;
                }
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

    return eReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CecRxChkBuf()
//  [Description]
//                   Driver layer: Use to retrieve CEC message and store into CEC Rx buffer
//  [Arguments]:
//
//  [Return]:
//                   TRUE: While get the new CEC message
//                   FALSE: No new CEC message
//**************************************************************************
void MDrv_CEC_ChkRxBuf(void* pInstance)  //don't place print message in this function
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL bGetResourceFlag = FALSE;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            bGetResourceFlag = TRUE;
        }
    }

    _MDrv_CEC_ChkRxBuf(pCECResourcePrivate, bGetResourceFlag);

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_TxSendMsg()
//  [Description]
//                   Use to send any CEC message by user defined
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg:            CEC message
//                   cmd:            message parameters
//                   len:              parameter length
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_TxSendMsg(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8 *cmd, MS_U8 len)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {

#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, msg, cmd, len);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MApi_CEC_TxSendMsg2()
//  [Description]
//                   Use to send any CEC message by user defined
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg:            CEC message
//                   cmd:            message parameters
//                   len:              parameter length
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_TxSendMsg2(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MS_U8* cmd, MS_U8 len)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi2(ucInitiatorLA, dst_address, msg, cmd, len);

    return res;
}
//**************************************************************************
//  [Function Name]:
//                   MDrv_CecTxSendPollingMsg()
//  [Description]
//                   Use to send any CEC polling msg by user defined
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CecTxSendPollingMsg(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, (MsCEC_MSGLIST)0x00, NULL, 0x00);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ActiveSource()
//  [Description]
//                   Used by a new source to indicate that it has started to transmit a stream or
//                   used in response to a <Request Active Source>
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  All sources
//                   Follower:  TV, CEC Switches
//  [Arguments]:
//
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ActiveSource(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 txbuf[2] = {0};
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = pCECResourcePrivate->MyPhysicalAddress[0];
            txbuf[1] = pCECResourcePrivate->MyPhysicalAddress[1];
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_ACTIVE_SOURCE, txbuf, 2);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_RoutingChange()
//  [Description]
//                   Used by a CEC switch when it's manually switched to inform akk other devices.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  CEC switches and TV(with 2 or more HDMI inputs)
//                   Follower:  CEC switches
//  [Arguments]:
//                   orig_address: The original physicall address
//                   new_address: The new physical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_RoutingChange(void* pInstance, MS_U8* orig_address, MS_U8* new_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 txbuf[4] = {0};
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = orig_address[0];
            txbuf[1] = orig_address[1];
            txbuf[2] = new_address[0];
            txbuf[3] = new_address[1];
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_RC_ROUTING_CHANGE, txbuf, 4);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReqActiveSource()
//  [Description]
//                   Used by a new device to discover the status of the system.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:
//                   Follower:  All, except for CEC Switches and devices which cannot become a source.
//  [Arguments]:
//
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReqActiveSource(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_RC_REQ_ACTIVE_SOURCE, (MS_U8*)E_MSG_FEATURE_ABORT, 0);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_SetStreamPath()
//  [Description]
//                   Used by the TV to request a streaming path from the specified physical address.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:
//                   Follower:  CEC Switches
//  [Arguments]:
//                   new_address: The new physical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_SetStreamPath(void* pInstance, MS_U8* new_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 txbuf[2] = {0};
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = new_address[0];
            txbuf[1] = new_address[1];
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_RC_SET_STREM_PATH, txbuf, 2);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_Standby()
//  [Description]
//                   Use to switch one or all devices into standby mode.
//                   One device: it's specific logical address, like "E_LA_RECORDER1"
//                   All device:   Use broadcast logical address, "E_LA_BROADCAST"
//  [Message property]
//                   Address:  Directly / Broadcast
//                   Initiator:  TV(Broadcast Address)
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_Standby(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            pCECResourcePrivate->MyPowerStatus = E_MSG_PWRSTA_STANDBY;
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_STANDBY, (MS_U8*)E_MSG_FEATURE_ABORT, 0);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_RecordOff()
//  [Description]
//                   Requests a device to stop a recording.
//  [Message property]
//                   Address:  Directly
//                   Initiator:  Device Initiating a recording
//                   Follower:  Recording Device if implementing <Record On>
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_RecordOff(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_OTR_RECORD_OFF, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_RecordOn()
//  [Description]
//                   Attempt to record the specified source.
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_RecordOn(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_RECORD_SOURCE_TYPE cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);
    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_OTR_RECORD_ON, (MS_U8*)&cmd, 1);

    return res;
}


//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReportCECVersion()
//  [Description]
//                   Used to indicate the supported CEC version, in response to a <Get CEC Version>
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//                   version: CEC version
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReportCECVersion(void* pInstance, MsCEC_DEVICELA dst_address, MS_U8 version)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_SI_CEC_VERSION, &version, 1);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReqCECVersion()
//  [Description]
//                   Used by a device to enquire which version of CEC the target supports
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReqCECVersion(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_SI_GET_CEC_VERSION, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReportPhycalAddress()
//  [Description]
//                   Use to inform all other devices of the mapping between physical and logical address of the initiator
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  All
//                   Follower:  TV
//  [Arguments]:
//
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReportPhycalAddress(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    MS_U8 txbuf[3] = {0};
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = pCECResourcePrivate->MyPhysicalAddress[0];
            txbuf[1] = pCECResourcePrivate->MyPhysicalAddress[1];
            txbuf[2] = pCECResourcePrivate->MyDeviceType;
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_SI_REPORT_PHY_ADDR, txbuf, 3);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReqPhycalAddress()
//  [Description]
//                   A request to a device to return its physical address
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  All, except for CEC Switches using logical address 15
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReqPhycalAddress(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
           MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_SI_REQUEST_PHY_ADDR, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_DeckControl()
//  [Description]
//                   Use to control a device's media functions
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      deck control mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_DeckControl(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_CTRL_PARM cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_DC_DECK_CTRL, (MS_U8*)&cmd, 1);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_DecStatus()
//  [Description]
//                   Use to provide a device's status when received "Give Deck Status"
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      deck info mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_DecStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_DECK_STATUS_PARM cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_DC_DECK_STATUS, (MS_U8*)&cmd, 1);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_MSg_GiveDeckStatus()
//  [Description]
//                   Use to request device's status
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      status request mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_MSg_GiveDeckStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_STATUS_REQUEST_PARM cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_DC_GIVE_DECK_STATUS, (MS_U8*)&cmd, 1);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_MSg_DCPlay()
//  [Description]
//                   Use to control the playback behaviour of a source device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd:      play mode
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_MSg_DCPlay(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_DC_PLAY_PARM cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_DC_PLAY, (MS_U8*)&cmd, 1);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReqMenuStatus()
//  [Description]
//                   Request from TV for a device to show/remove a menu or to query status
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd: menu request type
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReqMenuStatus(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSG_MENU_REQ_TYPE cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_DMC_MENU_REQUEST, (MS_U8*)&cmd, 1);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_UserCtrlPressed()
//  [Description]
//                   Use to indicated that user pressed which control button
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   cmd: UI command
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_UserCtrlPressed(void* pInstance, MS_BOOL bUserCtrlEn, MsCEC_DEVICELA dst_address, MsCEC_MSG_USER_CTRL_PARM cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    if(bUserCtrlEn) // menu status is activated
    {
        res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_UI_PRESS, (MS_U8*)&cmd, 1);
    }
    else // if menu status ia deactivated, send request to active device
    {
        //res = MApi_CEC_Msg_ReqMenuStatus(dst_address, E_MSG_MENU_REQ_ACTIVATED);
        res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_DMC_MENU_REQUEST, E_MSG_MENU_REQ_ACTIVATED, 1);
    }

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_UserCtrlReleased()
//  [Description]
//                   Use to indicated that user released which control button
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_UserCtrlReleased(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {

#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_UI_RELEASE, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_GiveAudioStatus()
//  [Description]
//                   Use to request an Amplifier to send its volume and mute status
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All (except CEC switches)
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_GiveAudioStatus(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_SAC_GIVE_AUDIO_STATUS, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReportPowerStatus()
//  [Description]
//                   Use to inform a requesting device of the current power status
//  [Message property]
//                   Address:  Directly
//                   Initiator:  All (except CEC switches)
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReportPowerStatus(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            MDRV_CEC_MSG_INFO("*CEC*:Report My PowerSts: 0x%x\n", pCECResourcePrivate->MyPowerStatus);
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_PS_REPORT_POWER_STATUS, (MS_U8*)&(pCECResourcePrivate->MyPowerStatus), 1);
    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReqPowerStatus()
//  [Description]
//                   Use to determine the current power status of a target device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  All (except CEC switches)
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReqPowerStatus(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_PS_GIVE_POWER_STATUS, (MS_U8*)E_MSG_FEATURE_ABORT, 0);
    MDRV_CEC_MSG_INFO("*CEC*:Req PowerSts: 0x%x\n", (MS_U16)res);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_FeatureAbort()
//  [Description]
//                   Use to indicate that device doesn't support message type
//  [Message property]
//                   Address:  Directly
//                   Initiator:  Generate if a message is not supported
//                   Follower:  All
//  [Arguments]:
//                   dst_address: destination logical address
//                   msg: the requested message type
//                   cmd: unsupport reason
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_FeatureAbort(void* pInstance, MsCEC_DEVICELA dst_address, MsCEC_MSGLIST msg, MsCEC_MSG_ABORT_REASON cmd)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 txbuf[2] = {0};
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = msg;
            txbuf[1] = cmd;
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_FEATURE_ABORT, txbuf, 2);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_Abort()
//  [Description]
//                   This message is reserved for testing purposes.
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  All except for CEC switches
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_Abort(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_ABORT_MESSAGE, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_SendMenuLanguage()
//  [Description]
//                   Used by a CEC switch when it's manually switched language to inform akk other devices.
//  [Message property]
//                   Address:  Broadcast
//                   Initiator:  CEC switches and TV(with 2 or more HDMI inputs)
//                   Follower:  CEC switches
//  [Arguments]:
//                   MenuLanguageCode: iso639-2 Language Code, 3 bytes
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_SendMenuLanguage(void* pInstance, MS_U8* pu8MenuLanguageCode)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    MS_U8 txbuf[3] = {0};
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = pu8MenuLanguageCode[0];
            txbuf[1] = pu8MenuLanguageCode[1];
            txbuf[2] = pu8MenuLanguageCode[2];
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_SI_SET_MENU_LANGUAGE, txbuf, 3);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CecMsg_ReqARCInitiation()
//  [Description]
//                   Used by an ARC TX device to request an ARC RX device to active
//                   the ARC functionility in ARC TX device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  ARC RX device
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CecMsg_ReqARCInitiation(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_ARC_REQUEST_ARC_INITATION, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CecMsg_ReqARCTermination()
//  [Description]
//                   Used by an ARC TX device to request an ARC RX device to deactive
//                   the ARC functionility in ARC TX device
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:  ARC RX device
//  [Arguments]:
//                   dst_address: destination logical address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CecMsg_ReqARCTermination(void* pInstance, MsCEC_DEVICELA dst_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_ARC_REQUEST_ARC_TERMINATION, (MS_U8*)E_MSG_FEATURE_ABORT, 0);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CecMsg_AudioModeReq()
//  [Description]
//                   A device implementing System Audio Control and which has volume control
//                   RC buttons request to use System Audio Mode to the Amplifier
//  [Message property]
//                   Address:  Directly
//                   Initiator:
//                   Follower:
//  [Arguments]:
//                   dst_address: destination logical address
//                   amp_switch: 1: unmute amplifier. 0: mute amplifier
//                   my_address: source to be used is the device specified at this address
//  [Return]:
//                   error_code: return status
//**************************************************************************
CEC_ERROR_CODE MDrv_CecMsg_AudioModeReq(void* pInstance, MsCEC_DEVICELA dst_address, MS_BOOL amp_switch, MS_U8* my_address)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 txbuf[2] = {0};
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
        }
    }

    UtopiaResourceRelease(pCECResource);

    if(amp_switch)
    {
        txbuf[0] = my_address[0];
        txbuf[1] = my_address[1];
        res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_SAC_SYSTEM_AUDIO_MODE_REQUEST, txbuf, 2);
    }
    else
    {
        res = _MDrv_CEC_TxApi(ucInitiatorLA, dst_address, E_MSG_SAC_SYSTEM_AUDIO_MODE_REQUEST, (MS_U8*)E_MSG_FEATURE_ABORT, 0);
    }

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CecCheckFrame()
//  [Description]
//                   Driver layer: Use to check CEC frame is valid or not
//  [Arguments]:
//                   msg_type:    CEC message type
//                   uclen:           message length, include header and opcode
//  [Return]:
//                   True or False
//**************************************************************************
MS_BOOL MDrv_CEC_CheckFrame(void* pInstance, MsCEC_MSG_TRANS_TYPE msg_type, MS_U8 ucLen)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL a = FALSE;
    MS_BOOL b = FALSE;
    MS_BOOL bReturnFlag = FALSE;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            a = (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[0]&0x0F) == 0x0F;	  //broadcast case or not (addressed)
            b = (pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].ucLength) >= ucLen;		  //para length same or bigger than want (spec)

            if((pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[0]&0xF0) == 0xF0)	  //message from unregistered device
            {
                switch(pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[1])	//opcode
                {
                    case E_MSG_RC_INACTIVE_SOURCE:
                    case E_MSG_RC_REQ_ACTIVE_SOURCE:
                    case E_MSG_RC_ROUTING_CHANGE:
                    case E_MSG_RC_ROUTING_INFO:
                    case E_MSG_RC_SET_STREM_PATH:

                    case E_MSG_STANDBY: 			  //=> system standby
                    // following command invoke broadcast response
                    case E_MSG_SI_REQUEST_PHY_ADDR: 	 //=> Report_Physical_Address
                    case E_MSG_SI_GET_MENU_LANGUAGE:	 //=> Set_Menu_Language
                    case E_MSG_VS_GIVE_VENDOR_ID:		 //=> Device_Vendor_ID
                        break;
                    default:
                        bReturnFlag = FALSE;
                        break;
                };
            }

            switch(msg_type)
            {
                case E_TRANS_BROADCAST_MSG:
                    bReturnFlag = a&b;
                    break;
                case E_TRANS_DIRECT_MSG:
                    bReturnFlag = (!a)&b;
                    break;
                case E_TRANS_BOTHTYPE_MSG:
                    bReturnFlag = b;
                    break;
            };
        }
    }

    UtopiaResourceRelease(pCECResource);

	return bReturnFlag;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetRetryCount()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetRetryCount(void* pInstance, MS_U8 u8RetryCount)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ucRetryCount = u8RetryCount;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_ConfigWakeUp()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_ConfigWakeUp(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if (!pCECResourcePrivate->bRIUAddrReady)
            {
                _MDrv_CEC_init_riu_base(pCECResourcePrivate); // Get base address to prevent core dump
            }

            mhal_CEC_ConfigWakeUp(pCECResourcePrivate->ucRetryCount, pCECResourcePrivate->ucVendorID, pCECResourcePrivate->ulXTAL_CLK_Hz,
                pCECResourcePrivate->MyPhysicalAddress, pCECResourcePrivate->MyDeviceType, pCECResourcePrivate->bCECIsSource);
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_ConfigWakeupInfoVendorID()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_ConfigWakeupInfoVendorID(void* pInstance, MS_U8 *u8CecVendorID)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ucVendorID[0] = u8CecVendorID[0];
            pCECResourcePrivate->ucVendorID[1] = u8CecVendorID[1];
            pCECResourcePrivate->ucVendorID[2] = u8CecVendorID[2];
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Enabled()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_Enabled(void* pInstance, MS_BOOL bEnableFlag)
{
    if (bEnableFlag)
    {
        mhal_CEC_ClearRxStatus();
      #if ENABLE_CEC_MULTIPLE
        mhal_CEC_ClearRxStatus2();
      #endif
        mhal_CEC_INTEn(TRUE);
        mhal_CEC_Enabled(TRUE);
    }
    else
    {
        mhal_CEC_INTEn(FALSE);
        mhal_CEC_Enabled(FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_TxStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_TxStatus(void* pInstance)
{
    return mhal_CEC_TxStatus();
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_CheckDeviceIsTx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MDrv_CEC_CheckDeviceIsTx(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL bCECIsSourceFlag = FALSE;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            bCECIsSourceFlag = pCECResourcePrivate->bCECIsSource;
        }
    }

    UtopiaResourceRelease(pCECResource);

    return bCECIsSourceFlag;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetPowerState()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 MDrv_CEC_SetPowerState(void* pInstance, EN_POWER_MODE u16PowerState)
{
    void* pModule = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    return mdrv_CEC_STREventProc(pModule, u16PowerState);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Get_Header()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_Get_Header(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ucReturnValue = pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[0];
        }
    }

    UtopiaResourceRelease(pCECResource);

	return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Get_OpCode()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_Get_OpCode(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ucReturnValue = pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[1];
        }
    }

    UtopiaResourceRelease(pCECResource);

    return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Get_Para()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_Get_Para(void* pInstance, MS_U8 u8Idx)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if(u8Idx+2 > 15)
            {
                ucReturnValue = pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[0];
            }
            else
            {
                ucReturnValue = pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].tRxData[u8Idx+2];
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

	return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetCmdLen()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_GetCmdLen(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ucReturnValue = pCECResourcePrivate->CecRxBuf[pCECResourcePrivate->CecFifoIdxS].ucLength;
        }
    }

    UtopiaResourceRelease(pCECResource);

    return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_IsRxBufEmpty()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MDrv_CEC_IsRxBufEmpty(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_BOOL bReturnFlag = FALSE;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if(pCECResourcePrivate->bCecMsgCnt == 0 )
            {
                bReturnFlag = TRUE;
            }
            else
            {
                bReturnFlag = FALSE;
            }
        }
    }

    UtopiaResourceRelease(pCECResource);

	return bReturnFlag;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetActiveLogicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetActiveLogicalAddress(void* pInstance, MsCEC_DEVICELA Addr)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ActiveLogicalAddress = Addr;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetActiveLogicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MsCEC_DEVICELA MDrv_CEC_GetActiveLogicalAddress(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MsCEC_DEVICELA eReturnValue = E_LA_UNREGISTERED;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            eReturnValue = pCECResourcePrivate->ActiveLogicalAddress;
        }
    }

    UtopiaResourceRelease(pCECResource);

	return eReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetPowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MsCEC_MSG_POWER_STATUS_PARM MDrv_CEC_GetPowerStatus(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MsCEC_MSG_POWER_STATUS_PARM eReturnValue = E_MSG_PWRSTA_UNKNOW;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            eReturnValue = pCECResourcePrivate->MyPowerStatus;
        }
    }

    UtopiaResourceRelease(pCECResource);

	return eReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetFifoIdx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_GetFifoIdx(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ucReturnValue = pCECResourcePrivate->CecFifoIdxS;
        }
    }

    UtopiaResourceRelease(pCECResource);

	return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetFifoIdx()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetFifoIdx(void* pInstance, MS_U8 u8Idx)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->CecFifoIdxS = u8Idx;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetActivePowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetActivePowerStatus(void* pInstance, MsCEC_MSG_POWER_STATUS_PARM Status)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ActivePowerStatus = Status;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetActivePowerStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MsCEC_MSG_POWER_STATUS_PARM MDrv_CEC_GetActivePowerStatus(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MsCEC_MSG_POWER_STATUS_PARM eReturnValue = E_MSG_PWRSTA_UNKNOW;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            eReturnValue = pCECResourcePrivate->ActivePowerStatus;
        }
    }

    UtopiaResourceRelease(pCECResource);

	return eReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetActivePhysicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetActivePhysicalAddress(void* pInstance, MS_U8 u8Para1, MS_U8 u8Para2)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ActivePhysicalAddress[0] = u8Para1;
            pCECResourcePrivate->ActivePhysicalAddress[1] = u8Para2;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetActiveDeviceCECVersion()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetActiveDeviceCECVersion(void* pInstance, MS_U8 u8Ver)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ActiveDeviceCECVersion= u8Ver;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetActiveDeviceType()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetActiveDeviceType(void* pInstance, MsCEC_DEVICE_TYPE Type)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->ActiveDeviceType = Type;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetMsgCnt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_GetMsgCnt(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ucReturnValue = pCECResourcePrivate->bCecMsgCnt;
        }
    }

    UtopiaResourceRelease(pCECResource);

	return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetMsgCnt()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetMsgCnt(void* pInstance, MS_U8 u8Cnt)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->bCecMsgCnt = u8Cnt;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_GetRxData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_CEC_GetRxData(void* pInstance, MS_U8 u8Fifoidx, MS_U8 u8Idx)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 ucReturnValue = 0;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ucReturnValue = pCECResourcePrivate->CecRxBuf[u8Fifoidx].tRxData[u8Idx];
        }
    }

    UtopiaResourceRelease(pCECResource);

	return ucReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetMyPhysicalAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetMyPhysicalAddress(void* pInstance, MS_U8 *pdata)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->MyPhysicalAddress[0] = pdata[0];
            pCECResourcePrivate->MyPhysicalAddress[1] = pdata[1];
        }
    }

    UtopiaResourceRelease(pCECResource);
}

#if ENABLE_CEC_MULTIPLE
//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetMyLogicalAddress2()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetMyLogicalAddress2(void* pInstance, MsCEC_DEVICELA myLA)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->MyLogicalAddress2= myLA;

            mhal_CEC_SetMyAddress2((MS_U8)pCECResourcePrivate->MyLogicalAddress2);
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_Msg_ReportPhycalAddress2()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
CEC_ERROR_CODE MDrv_CEC_Msg_ReportPhycalAddress2(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    MS_U8 txbuf[3] = {0};
    MS_U8 ucInitiatorLA = 0;
    CEC_ERROR_CODE res = E_CEC_FEATURE_ABORT;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
#if ENABLE_CEC_MULTIPLE
            ucInitiatorLA = pCECResourcePrivate->IniLogicalAddress;
#else
            ucInitiatorLA = pCECResourcePrivate->MyLogicalAddress;
#endif
            txbuf[0] = pCECResourcePrivate->MyPhysicalAddress2[0];
            txbuf[1] = pCECResourcePrivate->MyPhysicalAddress2[1];
            txbuf[2] = pCECResourcePrivate->MyDeviceType2;
        }
    }

    UtopiaResourceRelease(pCECResource);

    res = _MDrv_CEC_TxApi(ucInitiatorLA, E_LA_BROADCAST, E_MSG_SI_REPORT_PHY_ADDR, txbuf, 3);

    return res;
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetMyPhysicalAddress2()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetMyPhysicalAddress2(void* pInstance, MS_U8 *pdata)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->MyPhysicalAddress2[0] = pdata[0];
            pCECResourcePrivate->MyPhysicalAddress2[1] = pdata[1];
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_SetInitiator()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_SetInitiator(void* pInstance, MsCEC_DEVICELA IniLA)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
   CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pCECResourcePrivate->IniLogicalAddress = IniLA ;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

#endif

//**************************************************************************
//  [Function Name]:
//                   MDrv_CEC_AttachISR()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_CEC_AttachDriverISR(void* pInstance, MS_BOOL bAttachDrvFlag)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if(pCECResourcePrivate->ucInitialIndex != 0xA5)
            {
                _MDrv_CEC_InitVariable(pCECResourcePrivate);

                pCECResourcePrivate->ucInitialIndex = 0xA5;
            }

            pCECResourcePrivate->bAttachDrvISR = bAttachDrvFlag;
        }
    }

    UtopiaResourceRelease(pCECResource);
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_CEC_GetConfiguration()
//  [Description]
//                  CEC get init config
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
stCEC_INITIAL_CONFIG_INFO MDrv_CEC_GetConfiguration(void* pInstance)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;
    stCEC_INITIAL_CONFIG_INFO stInitialConfigInfo = {0};

    stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_ERR_NOT_AVAIL;

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        _MDrv_CEC_init_riu_base(pCECResourcePrivate);

        if(pCECResourcePrivate->bCecInitFinish)
        {
            stInitialConfigInfo.ulReturnValue = UTOPIA_STATUS_SUCCESS;
            stInitialConfigInfo.bAttachDrvISR = pCECResourcePrivate->bAttachDrvISR;
            stInitialConfigInfo.ulXTAL_CLK_Hz = pCECResourcePrivate->ulXTAL_CLK_Hz;
        }
    }

    UtopiaResourceRelease(pCECResource);
    return stInitialConfigInfo;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
//**************************************************************************
//  [Function Name]:
//                  mdrv_CEC_MDCMDEchoCommand()
//  [Description]
//                  CEC MDCMD echo command function
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_CEC_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine)
{
    char pch[] = "=,";
    char* psep;

    MdbPrint(u64ReqHdl, "LINE:%d, MDBCMD_CMDLINE\n", __LINE__);
    MdbPrint(u64ReqHdl, "pcCmdLine: %s\n", pcCmdLine);

    psep = strsep(&pcCmdLine, pch);

    if(strncmp("CECOn", psep, 5) == 0)
    {

    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_CEC_MDCMDGetInfo()
//  [Description]
//                  CEC MDCMD get info function
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_CEC_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl)
{
    void* pModule = NULL;
    void* pCECResource = NULL;
    CEC_RESOURCE_PRIVATE *pCECResourcePrivate = NULL;

    if(UtopiaResourceObtain(pModule, CEC_POOL, &pCECResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pCECResource, (void*)&pCECResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_CEC_MSG_ERROR("[%s,%5d] Get CEC resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        MdbPrint(u64ReqHdl, "LINE:%d, MDBCMD_GETINFO\n", __LINE__);
        MdbPrint(u64ReqHdl, "----------------  Mstar CEC Info -----------------\n");
#if ENABLE_CEC_MULTIPLE
            MdbPrint(u64ReqHdl,"Logical Address:  %d\n",pCECResourcePrivate->IniLogicalAddress);
#else
			MdbPrint(u64ReqHdl,"Logical Address:  %d\n",pCECResourcePrivate->MyLogicalAddress);
#endif
    }

    UtopiaResourceRelease(pCECResource);
}

#endif
