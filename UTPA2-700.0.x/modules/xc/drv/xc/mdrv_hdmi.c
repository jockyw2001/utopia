
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
/// file    Mdrv_hdmi.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  MDRV_HDMI_C
//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "drvXC_HDMI_Internal.h"
#include "mhal_hdmi.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_adc.h"
#include "xc_hwreg_utility2.h"
#include "drv_sc_mux.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_if_v2.h"
#include "apiHDMIRx_private.h"

#ifdef  MSOS_TYPE_LINUX
#include <assert.h>
#else
#define assert(b)   \
    do {    \
        if (!(b)){  \
            printf("assert at [%s][%d] \n", __FUNCTION__, __LINE__);   \
            *((int*)0) = 0; \
        }   \
    } while(0)
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define msg_hdmi(x) //x
#define DRV_HDMI_HDR(x) //x
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define HDMI_POLLING_INTERVAL           10U
#define HDMI_HDCP_WAIT_ENENT_TIMEOUT    100U

#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MDRV_HDMIRX_MSG_INFO(format, args...)       ULOGI("HDMIRX", format, ##args)
#define MDRV_HDMIRX_MSG_WARNING(format, args...)    ULOGW("HDMIRX", format, ##args)
#define MDRV_HDMIRX_MSG_DEBUG(format, args...)      ULOGD("HDMIRX", format, ##args)
#define MDRV_HDMIRX_MSG_ERROR(format, args...)      ULOGE("HDMIRX", format, ##args)
#define MDRV_HDMIRX_MSG_FATAL(format, args...)      ULOGF("HDMIRX", format, ##args)

#else
#define MDRV_HDMIRX_MSG_INFO(format, args...)       printf(format, ##args)
#define MDRV_HDMIRX_MSG_WARNING(format, args...)    printf(format, ##args)
#define MDRV_HDMIRX_MSG_DEBUG(format, args...)      printf(format, ##args)
#define MDRV_HDMIRX_MSG_ERROR(format, args...)      printf(format, ##args)
#define MDRV_HDMIRX_MSG_FATAL(format, args...)      printf(format, ##args)

#endif

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
MS_BOOL bInitEnable = FALSE;
MS_BOOL ucHDMIInfoSourceSelect = HDMI_INFO_SOURCE0;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
HDCP22_Recv_CBF fpHdcp22RecMsgCBFunc = NULL; //wilson for u2k //type, portIdx, msg, msg length, context
//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/// Debug information
static MSIF_Version _drv_hdmi_version = {.DDI = { HDMI_DRV_VERSION },};
void* pulHDMIRxInst;

MS_BOOL bHDCPInterruptProcFlag = FALSE;
MS_BOOL bHDCPInterruptCreateFlag = FALSE;
MS_S32 slHDCPInterruptTaskID = -1;
MS_U8 ucHDCPInterruptStack[HDMI_HDCP_INTERRUPT_STACK_SIZE];

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void MDrv_HDMI_ReadEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u8EDIDSize);
static void MDrv_HDMI_WriteEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u16EDIDSize);

extern MS_U32 MsOS_GetSystemTime(void);
//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDMI_CheckPacketReceiveProc
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_HDMI_CheckPacketReceiveProc(HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate)
{
    MS_U8 ucHDMIInfoSource = 0;

    for(ucHDMIInfoSource = HDMI_INFO_SOURCE0; ucHDMIInfoSource < HDMI_INFO_SOURCE_MAX; ucHDMIInfoSource++)
    {
        if(pHDMIRxResourcePrivate->ucReceiveIntervalCount[ucHDMIInfoSource] < HDMI_PACKET_RECEIVE_COUNT)
        {
            pHDMIRxResourcePrivate->ucReceiveIntervalCount[ucHDMIInfoSource]++;
        }
        else
        {
            pHDMIRxResourcePrivate->ulPacketStatus[ucHDMIInfoSource] = Hal_HDMI_packet_info(ucHDMIInfoSource);

            pHDMIRxResourcePrivate->ucReceiveIntervalCount[ucHDMIInfoSource] = 0;
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDMI_CheckHDCP14RiProc
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mdrv_HDMI_CheckHDCP14RiProc(HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate)
{
    MS_U8 enPortIndex = 0;

    for(enPortIndex = INPUT_PORT_DVI0; enPortIndex <= INPUT_PORT_DVI_END; enPortIndex++)
    {
        if(pHDMIRxResourcePrivate->usHDCP14RiCount[HDMI_GET_PORT_SELECT(enPortIndex)] < HDMI_HDCP14_RI_COUNT)
        {
            pHDMIRxResourcePrivate->usHDCP14RiCount[HDMI_GET_PORT_SELECT(enPortIndex)]++;
        }
        else
        {
            if(Hal_HDCP_getstatus(enPortIndex) & BIT(13)) // [13] Ri
            {
                pHDMIRxResourcePrivate->bHDCP14RiFlag[HDMI_GET_PORT_SELECT(enPortIndex)] = TRUE;
                Hal_HDCP_ClearStatus(enPortIndex, BIT(5));
            }
            else
            {
                pHDMIRxResourcePrivate->bHDCP14RiFlag[HDMI_GET_PORT_SELECT(enPortIndex)] = FALSE;
            }

            pHDMIRxResourcePrivate->usHDCP14RiCount[HDMI_GET_PORT_SELECT(enPortIndex)] = 0;
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDMI_PollingTask
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void _mdrv_HDMI_PollingTask(void)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_BOOL bHDMITaskProcFlag = TRUE;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaModuleGetPtr(MODULE_HDMIRX, &pModule);

    while(bHDMITaskProcFlag)
    {
        if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
            {
                MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
            }
            else
            {
                Hal_HDMI_StablePolling(pHDMIRxResourcePrivate->ucMHLSupportPath, pHDMIRxResourcePrivate->stHDMIPollingInfo);

                _mdrv_HDMI_CheckPacketReceiveProc(pHDMIRxResourcePrivate);
                _mdrv_HDMI_CheckHDCP14RiProc(pHDMIRxResourcePrivate);

                bHDMITaskProcFlag = pHDMIRxResourcePrivate->bHDMITaskProcFlag;
            }
        }

        UtopiaResourceRelease(pHDMIRxResource);

        MsOS_DelayTask(HDMI_POLLING_INTERVAL);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDMI_CreateTask
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_HDMI_CreateTask(HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate)
{
    MS_BOOL bCreateSuccess = TRUE;

    if(pHDMIRxResourcePrivate->slHDMIPollingTaskID < 0)
    {
        pHDMIRxResourcePrivate->slHDMIPollingTaskID = MsOS_CreateTask((TaskEntry) _mdrv_HDMI_PollingTask,
                                     (MS_VIRT) NULL,
                                     E_TASK_PRI_MEDIUM,
                                     TRUE,
                                     (void *)pHDMIRxResourcePrivate->ucHDMIPollingStack,
                                     HDMI_POLLING_STACK_SIZE,
                                     "HDMI Rx task");

        pHDMIRxResourcePrivate->bHDMITaskProcFlag = TRUE;
    }

    if(pHDMIRxResourcePrivate->slHDMIPollingTaskID < 0)
    {
        MDRV_HDMIRX_MSG_ERROR("MsOS_CreateTask failed!!\n");

        MsOS_DeleteEventGroup(pHDMIRxResourcePrivate->slHDMIPollingTaskID);

        bCreateSuccess = FALSE;;
    }

    return bCreateSuccess;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDMI_CheckHDCP14Bksv()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_HDMI_CheckHDCP14Bksv(MS_U8 *ucBksvTable)
{
    MS_BOOL bHDCP14KeyVaild = FALSE;
    MS_U8 uctemp = 0;
    MS_U8 ucOneCount = 0;
    MS_U8 ucBksvValue = 0;

    if(ucBksvTable != NULL)
    {
        for(uctemp = 0; uctemp < HDMI_HDCP_BKSV_LENGTH; uctemp++)
        {
            ucBksvValue = ucBksvTable[uctemp];

            while(ucBksvValue > 0)
            {
                if(ucBksvValue & BIT(0))
                {
                    ucOneCount++;
                }

                ucBksvValue = ucBksvValue >> 1;
            }
        }

        if(ucOneCount == 20)
        {
            bHDCP14KeyVaild = TRUE;
        }
    }

    return bHDCP14KeyVaild;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDCP_InterruptTask
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void _mdrv_HDCP_InterruptTask(void)
{
    MS_BOOL bHDCPTaskProcFlag = TRUE;
    E_MUX_INPUTPORT enInputPortType = 0;
    stHDCP22_HANDLER XCArgs = {0};
    stHDCP22_WAIT_EVENT stHDMIRxArgs = {0};

    XCArgs.bHdcp22RecvMsgFlag = FALSE;
    XCArgs.bIRQModeFlag = TRUE;
    XCArgs.ucHdcp22PortType = E_HDCP22_IF_HDMI;
    XCArgs.ucHdcp22MsgLen = 0;
    stHDMIRxArgs.ucHDCPWriteDoneIndex = 0;

    while(bHDCPTaskProcFlag)
    {
        if(pulHDMIRxInst == NULL)
        {
            MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
        }
        else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_WAIT_EVENT, (void*)&stHDMIRxArgs) != 0)
        {
            MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
        }

        //MDRV_HDMIRX_MSG_INFO("** TMDS wait event happen user space !!!!!!\r\n");

        for(enInputPortType = INPUT_PORT_DVI0; enInputPortType <= INPUT_PORT_DVI_END; enInputPortType++)
        {
            if(stHDMIRxArgs.ucHDCPWriteDoneIndex &BIT(HDMI_GET_PORT_SELECT(enInputPortType)))
            {
                XCArgs.ucPortIdx = HDMI_GET_PORT_SELECT(enInputPortType);

                if(pulHDMIRxInst == NULL)
                {
                    MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
                }
                else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_HANDLER, (void*)&XCArgs) != 0)
                {
                    MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
                }

                if(XCArgs.bHdcp22RecvMsgFlag)
                {
                    if (fpHdcp22RecMsgCBFunc != NULL)
                    {
                        fpHdcp22RecMsgCBFunc(XCArgs.ucHdcp22PortType, HDMI_GET_PORT_SELECT(enInputPortType), XCArgs.pucHdcp22MsgData, XCArgs.ucHdcp22MsgLen, NULL);
                    }
                }
            }
        }

        bHDCPTaskProcFlag = bHDCPInterruptProcFlag;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDCP_CreateInterruptTask
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mdrv_HDCP_CreateInterruptTask(void)
{
    MS_BOOL bCreateSuccess = TRUE;

    if(slHDCPInterruptTaskID < 0)
    {
        slHDCPInterruptTaskID = MsOS_CreateTask((TaskEntry) _mdrv_HDCP_InterruptTask,
                                     (MS_VIRT) NULL,
                                     E_TASK_PRI_MEDIUM,
                                     TRUE,
                                     (void *)ucHDCPInterruptStack,
                                     HDMI_HDCP_INTERRUPT_STACK_SIZE,
                                     "HDCP int task");

        bHDCPInterruptProcFlag = TRUE;
    }

    if(slHDCPInterruptTaskID < 0)
    {
        MDRV_HDMIRX_MSG_ERROR("MsOS_CreateTask failed!!\n");

        MsOS_DeleteEventGroup(slHDCPInterruptTaskID);

        bCreateSuccess = FALSE;;
    }

    return bCreateSuccess;
}

//**************************************************************************
//  [Function Name]:
//                  _mdrv_HDCP_IsrHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void _mdrv_HDCP_IsrHandler(InterruptNum eIntNum)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_BOOL bGetResourceFlag = FALSE;
    E_MUX_INPUTPORT enInputPortType = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaModuleGetPtr(MODULE_HDMIRX, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        bGetResourceFlag = TRUE;
    }

    UNUSED(eIntNum);

    for(enInputPortType = INPUT_PORT_DVI0; enInputPortType <= INPUT_PORT_DVI_END; enInputPortType++)
    {
        if(Hal_HDCP22_PollingWriteDone(HDMI_GET_PORT_SELECT(enInputPortType)) && bGetResourceFlag)
        {
            pHDMIRxResourcePrivate->ucHDCPWriteDoneIndex |= BIT(HDMI_GET_PORT_SELECT(enInputPortType));

            if(GET_HDMI_FLAG(pHDMIRxResourcePrivate->ucHDCPReadDoneIndex, BIT(HDMI_GET_PORT_SELECT(enInputPortType))))
            {
                CLR_HDMI_FLAG(pHDMIRxResourcePrivate->ucHDCPReadDoneIndex, BIT(HDMI_GET_PORT_SELECT(enInputPortType)));
            }
        }

        if(Hal_HDCP22_PollingReadDone(HDMI_GET_PORT_SELECT(enInputPortType)) && bGetResourceFlag)
        {
            if(pHDMIRxResourcePrivate->ucHDCP22RecvIDListSend[HDMI_GET_PORT_SELECT(enInputPortType)])
            {
                Hal_HDCP22_SetReadyBit(HDMI_GET_PORT_SELECT(enInputPortType), FALSE); //clear ready bit after source read re

                pHDMIRxResourcePrivate->ucHDCP22RecvIDListSend[HDMI_GET_PORT_SELECT(enInputPortType)] = FALSE;
            }

            SET_HDMI_FLAG(pHDMIRxResourcePrivate->ucHDCPReadDoneIndex, BIT(HDMI_GET_PORT_SELECT(enInputPortType)));
        }
    }

    if(bGetResourceFlag)
    {
        if(pHDMIRxResourcePrivate->ucHDCPWriteDoneIndex > 0)
        {
            MsOS_SetEvent(pHDMIRxResourcePrivate->slHDMIHDCPEventID, HDMI_HDCP_EVENT_WRITE_DONE);
        }
    }

    MsOS_EnableInterrupt(E_INT_IRQ_DVI_HDMI_HDCP);

    UtopiaResourceRelease(pHDMIRxResource);
}

void MDrv_HDCP22_FillCipherKey(MS_U8 ucPortIdx, MS_U8* pucRiv, MS_U8* pucSessionKey)
{
    stHDCP22_FILL_CIPHER_KEY XCArgs;

    XCArgs.ucPortIdx = ucPortIdx;
    XCArgs.pucRiv = pucRiv;
    XCArgs.pucSessionKey = pucSessionKey;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_FILL_CIPHER_KEY, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

void MDrv_HDCP22_InitCBFunc(HDCP22_Recv_CBF pCBFunc, void* pContext)
{
    if(pCBFunc != NULL)
    {
        fpHdcp22RecMsgCBFunc = pCBFunc;
    }
}

void MDrv_HDCP22_PortInit(MS_U8 ucPortIdx)
{
    stHDCP22_PORT_INIT XCArgs = {0};

    XCArgs.ucPortIdx = ucPortIdx;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_PORT_INIT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

MS_BOOL MDrv_HDCP22_PollingReadDone(MS_U8 ucPortIdx)
{
    stHDCP22_POLLING_READ_DONE XCArgs = {0};

    XCArgs.ucPortIdx = ucPortIdx;
    XCArgs.bReturnValue = FALSE;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_POLLING_READ_DONE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return XCArgs.bReturnValue;
}

void MDrv_HDCP22_EnableCipher(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable)
{
    stHDCP22_ENABLE_CIPHER XCArgs = {0};

    XCArgs.ucPortType = ucPortType;
    XCArgs.ucPortIdx = ucPortIdx;
    XCArgs.bIsEnable = bIsEnable;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_ENABLE_CIPHER, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

void MDrv_HDCP22_SendMsg(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy)
{
    stHDCP22_SEND_MESSAGE XCArgs;

    XCArgs.ucPortType = ucPortType;
    XCArgs.ucPortIdx = ucPortIdx;
    XCArgs.pucData = pucData;
    XCArgs.dwDataLen = dwDataLen;
    XCArgs.pDummy = pDummy;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_SEND_MESSAGE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

void MDrv_HDCP22_Handler(MS_U8 ucPortIdx)
{
    stHDCP22_HANDLER XCArgs = {0};

    XCArgs.bHdcp22RecvMsgFlag = FALSE;
    XCArgs.bIRQModeFlag = FALSE;
    XCArgs.ucPortIdx = ucPortIdx;
    XCArgs.ucHdcp22PortType = E_HDCP22_IF_HDMI;
    XCArgs.ucHdcp22MsgLen = 0;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_HDCP22_HANDLER, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    if(XCArgs.bHdcp22RecvMsgFlag)
    {
        if (fpHdcp22RecMsgCBFunc != NULL)
        {
            fpHdcp22RecMsgCBFunc(XCArgs.ucHdcp22PortType, ucPortIdx, XCArgs.pucHdcp22MsgData, XCArgs.ucHdcp22MsgLen, NULL);
        }
    }
}

//-------------------------------------------------------------------------------------------------
/// report which patch function hdmi needs
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_Func_Caps_U2(void* pInstance)
{
    return Hal_HDMI_Func_Caps();
}

MS_U16 MDrv_HDMI_Func_Caps(void)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_FUNC_CAPS XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_FUNC_CAPS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Check the HDCP is exist or not
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDCP_isexist(void)
{
    MS_HDCP_STATUS_INFO_t* enStatus_info;
    enStatus_info = MDrv_HDCP_GetStatus();

    if(enStatus_info->b_St_Reserve_1 && enStatus_info->b_St_ENC_En)
    {
        MDRV_HDMIRX_MSG_INFO("HDCP is exit");
        return TRUE;
    }
    else
    {
        MDRV_HDMIRX_MSG_INFO("No HDCP");
        return FALSE;
    }
}
//-------------------------------------------------------------------------------------------------
/// DDC clock on/off control
/// @param  bEnable      \b IN: ON/OFF control
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_Enable_U2(void* pInstance, MS_BOOL bEnable)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDCP_ddc_en(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType), bEnable);
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDCP_ddc_en(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType), bEnable);
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    Hal_HDCP_ddc_en(bEnable);
#endif
}

void MDrv_HDCP_Enable(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_ENABLE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// DDC clock on/off control
/// @param  enInputPortType  \b IN: Input source selection
/// @param  bEnable          \b IN: ON/OFF control
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_SetHdcpEnable_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable)
{
    /*Only DVI port can enable/disable hdcp key*/
    if(enInputPortType < INPUT_PORT_DVI0 ||  enInputPortType > INPUT_PORT_DVI3)
    {
        return FALSE;
    }

#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDCP_ddc_en(enInputPortType, bEnable);
#else
    enInputPortType = enInputPortType;
    Hal_HDCP_ddc_en(bEnable);
#endif
    return TRUE;
}

MS_BOOL MDrv_HDMI_SetHdcpEnable(E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_SET_HDCPENABLE XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Check the ending of the Vsync
/// @param  bStable      \b IN: Stable or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_Vsync_end_en_U2(void* pInstance, MS_BOOL bStable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(bStable)
    {
        if(!pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count)
        {
            pXCResourcePrivate->stdrv_HDMI.u32VsyncTimer = MsOS_GetSystemTime();
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count = TRUE;
        }
        if ((pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done) && ((MsOS_GetSystemTime() - pXCResourcePrivate->stdrv_HDMI.u32VsyncTimer) > 4500) )
        {
            Hal_HDCP_Vsync_end_en(TRUE);
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done = FALSE;
        }
    }
    else
    {
        if (!pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done)
        {
            Hal_HDCP_Vsync_end_en(FALSE);
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_done = TRUE;
            pXCResourcePrivate->stdrv_HDMI._bHDCP_Vsync_count = FALSE;
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_HDCP_Vsync_end_en(MS_BOOL bStable)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDCP_VSYNC_END_EN XCArgs;
    XCArgs.bStable = bStable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_VSYNC_END_EN, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Exit the HDMI and turn off the related setting of HDMI
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Exit_U2( void* pInstance )
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDMI_exit(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType));
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDMI_exit(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType));
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    Hal_HDMI_exit();
#endif
#if 0
    if(g_bIMMESWITCH_DVI_POWERSAVING)
        memset(u8PSDVIStableCount, 0, 4*sizeof(MS_U8));
#endif
}

void MDrv_HDMI_Exit( void )
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_EXIT, NULL) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


//-------------------------------------------------------------------------------------------------
/// Init the setting for HDMI
//-------------------------------------------------------------------------------------------------
stHDMI_INITIAL_TABLE MDrv_HDMI_init_internal(void *pInstance)
{
    stHDMI_INITIAL_TABLE stInitialTable = {0};
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrv_HDMI._info.u8SupportPorts = 0;
    pXCResourcePrivate->stdrv_HDMI._status.bIsInitialized = TRUE;
    pXCResourcePrivate->stdrv_HDMI._status.bIsRunning     = TRUE;

    bInitEnable = TRUE;

    if (SUPPORT_IMMESWITCH)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IMMESWITCH);
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IMMESWITCH_DVI_POWERSAVING) >> 1;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH = FALSE;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING = FALSE;
    }
    #ifdef K3_U2
    if (SUPPORT_DVI_AUTO_EQ)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_DVI_AUTO_EQ) >> 2;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ = FALSE;
    }
    #else
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ = SUPPORT_DVI_AUTO_EQ;
    #endif

    MDRV_HDMIRX_MSG_INFO("[%s][%d]pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH=%x\n", __FUNCTION__, __LINE__,
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH);
    MDRV_HDMIRX_MSG_INFO("[%s][%d]pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING=%x\n", __FUNCTION__, __LINE__,
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING);

    Hal_HDMI_init(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH, pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo);

    stInitialTable.bImmeswitchSupport = pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH;
    stInitialTable.ucMHLSupportPath = pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo;

    return stInitialTable;
}

stHDMI_INITIAL_TABLE MDrv_HDMI_init_U2( void* pInstance )
{
    stHDMI_INITIAL_TABLE stInitialTable = {0};
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    stInitialTable = MDrv_HDMI_init_internal(pInstance);

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return stInitialTable;
}

void MDrv_HDMI_init( void )
{
    MS_BOOL bInitialFlag = FALSE;
    stHDMI_INITIAL_SETTING XCArgs;

    XCArgs.stInitialTable.bImmeswitchSupport = FALSE;
    XCArgs.stInitialTable.ucMHLSupportPath = 0;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    if(pulHDMIRxInst == NULL)
    {
        if(UtopiaOpen(MODULE_HDMIRX, &pulHDMIRxInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("UtopiaOpen pulHDMIRxInst failed\n");

            bInitialFlag = TRUE;
        }
    }

    if(!bInitialFlag)
    {
        if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_INITIAL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
        }
    }

    if(!bHDCPInterruptCreateFlag)
    {
        _mdrv_HDCP_CreateInterruptTask();

        bHDCPInterruptCreateFlag = TRUE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the equalizer volumn for HDMI
/// @param  enEq      \b IN: ON/OFF the equlizer
/// @param  u8EQValue      \b IN: The volumn of equalizer
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Set_EQ_U2(void* pInstance, MS_HDMI_EQ enEq, MS_U8 u8EQValue)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    INPUT_SOURCE_TYPE_t src;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    for(src = INPUT_SOURCE_HDMI; src < INPUT_SOURCE_HDMI_MAX; src++)
    {
        Hal_HDMI_Set_EQ(MDrv_XC_Mux_GetHDMIPort(pInstance, src), enEq, u8EQValue);
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    Hal_HDMI_Set_EQ(enEq, u8EQValue);
#endif
}

void MDrv_HDMI_Set_EQ(MS_HDMI_EQ enEq, MS_U8 u8EQValue)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_SET_EQ XCArgs;
    XCArgs.enEq = enEq;
    XCArgs.u8EQValue = u8EQValue;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_EQ, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the equalizer volumn for HDMI to single port
/// @param  enEq      \b IN: ON/OFF the equlizer
/// @param  u8EQValue      \b IN: The volumn of equalizer
/// @param  enInputPortType      \b IN: The appointed port
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_Set_EQ_To_Port_U2(void* pInstance, MS_HDMI_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDMI_Set_EQ(enInputPortType, enEq, u8EQValue);
    return TRUE;
#else
    MDRV_HDMIRX_MSG_INFO("[Warning]Unsupport set EQ to single port!");
    return FALSE;
#endif
}

MS_BOOL MDrv_HDMI_Set_EQ_To_Port(MS_HDMI_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_SET_EQ_TO_PORT XCArgs;
    XCArgs.enEq = enEq;
    XCArgs.u8EQValue = u8EQValue;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable the Audio mute event for Vivaldi
/// @param  u16MuteEvent      \b IN: The mute event
/// @param  u16MuteMask       \b IN: The mute mask
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Audio_MUTE_Enable_U2(void* pInstance, MS_U16 u16MuteEvent,MS_U16 u16MuteMask)
{
    Hal_HDMI_Audio_MUTE_Enable(u16MuteEvent, u16MuteMask);
}

void MDrv_HDMI_Audio_MUTE_Enable(MS_U16 u16MuteEvent,MS_U16 u16MuteMask)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_AUDIO_MUTE_ENABLE XCArgs;
    XCArgs.u16MuteEvent = u16MuteEvent;
    XCArgs.u16MuteMask = u16MuteMask;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Clear the Audio mute status.
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Audio_Status_Clear_U2(void* pInstance)
{
    Hal_HDMI_Audio_Status_Clear();
}

void MDrv_HDMI_Audio_Status_Clear(void)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR, NULL) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Control the Hot Plug Detection
/// in HIGH Level, the voltage level is 2.4 ~ 5.3V
/// in LOW Level,  the voltage level is 0 ~ 0.4V
/// Enable hotplug GPIO[0] output and set output value
/// @param  bHighLow                \b IN: High/Low control
/// @param  enInputPortType      \b IN: Input source selection
/// @param  bInverse                 \b IN: Inverse or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_pullhpd_internal(void *pInstance, MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    MDRV_HDMIRX_MSG_INFO("src hpd = %x \n",enInputPortType);
    // Fix me. Do not use HDMI_PORT_ALL and HDMI_PORT_A.
    // use E_MUX_INPUTPORT.
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bHDMIInverseBackUp[enInputPortType - INPUT_PORT_DVI0] = bInverse;

    Hal_HDMI_pullhpd(bHighLow, enInputPortType, bInverse, pXCResourcePrivate->stdrvXC_MVideo_Context._u8MhlSupportInfo);
}

void MDrv_HDMI_pullhpd_U2( void* pInstance, MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

    //MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Enter  enInputPortType= %d, bHighLow=%x, tick=%d \n",__FUNCTION__,__LINE__, enInputPortType, bHighLow , MsOS_GetSystemTime());

    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    MDrv_HDMI_pullhpd_internal(pInstance, bHighLow, enInputPortType, bInverse);

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    //MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Exit  enInputPortType= %d, bHighLow=%x, tick=%d \n",__FUNCTION__,__LINE__, enInputPortType, bHighLow , MsOS_GetSystemTime());
}

void MDrv_HDMI_pullhpd( MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    stHDMI_PULLHPD XCArgs = {0};
    stHDMI_SET_HPD_INVERT_FLAG stHDMIRxArgs = {0};

    XCArgs.bHighLow = bHighLow;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bInverse = bInverse;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PULLHPD, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    stHDMIRxArgs.enInputPortType = enInputPortType;
    stHDMIRxArgs.bInverse = bInverse;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_SET_HPD_INVERT_FLAG, (void*)&stHDMIRxArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//-------------------------------------------------------------------------------------------------
/// HDMI G control information
/// @param  gcontrol                \b IN: G control setting
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_GC_Info_U2(void* pInstance, MS_U8 ucHDMIInfoSource, HDMI_GControl_INFO_t gcontrol)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16Return = Hal_HDMI_gcontrol_info(gcontrol, ucHDMIInfoSource);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u16Return;
}

MS_U16 MDrv_HDMI_GC_Info(HDMI_GControl_INFO_t gcontrol)
{
    stHDMI_GC_INFO XCArgs;

    XCArgs.gcontrol = gcontrol;
    XCArgs.u16ReturnValue = 0;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GC_INFO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.u16ReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// HDMI PLL control setting 1
/// @param  pllctrl                \b IN: PLL control
/// @param  bread                \b IN: Read/Write (1/0)
/// @param  u16value           \b IN: Write value
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_pll_ctrl1(HDMI_PLL_CTRL_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    return (Hal_HDMI_pll_ctrl1(pllctrl, bread, u16value));
}
//-------------------------------------------------------------------------------------------------
/// HDMI PLL control setting 2
/// @param  pllctrl                \b IN: PLL control
/// @param  bread                \b IN: Read/Write (1/0)
/// @param  u16value           \b IN: Write value
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_HDMI_pll_ctrl2(HDMI_PLL_CTRL2_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    return (Hal_HDMI_pll_ctrl2(pllctrl, bread, u16value));
}

////////////////////////


/******************************************************************************/
/// for DVI
/******************************************************************************/
//-------------------------------------------------------------------------------------------------
/// Check the clock is less 70MHz or not with DVI source
/// @param  enInputPortType                \b IN: Input source
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_SetForClockLessThan70Mhz_U2( void* pInstance, E_MUX_INPUTPORT enInputPortType )
{
    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:
        case INPUT_PORT_DVI1:
        case INPUT_PORT_DVI3:
            return Hal_DVI_clk_lessthan70mhz_usingportc(FALSE);
        case INPUT_PORT_DVI2:
            return Hal_DVI_clk_lessthan70mhz_usingportc(TRUE);
    }
}

MS_BOOL MDrv_HDMI_SetForClockLessThan70Mhz( E_MUX_INPUTPORT enInputPortType )
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_SETFORCLOCKLESSTHAN70MHZ XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_DVI_SoftwareReset_U2( void* pInstance, MS_U16 u16Reset )
{
    Hal_DVI_sw_reset(u16Reset);

}

void MDrv_DVI_SoftwareReset( MS_U16 u16Reset )
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_SOFTWARERESET XCArgs;
    XCArgs.u16Reset = u16Reset;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_SOFTWARERESET, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Detect the the clock is lose or not from ADC DTOP
/// @param  enInputPortType                \b IN: Input source
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_dvi_clock_detect(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bIsLoss;

#if SUPPORT_HDMI_RX_NEW_FEATURE
    bIsLoss = Hal_DVI_clklose_det(enInputPortType);
#else
    if (enInputPortType == INPUT_PORT_DVI2)
    {
        bIsLoss = Hal_DVI_clklose_det(TRUE);
    }
    else
    {
        bIsLoss = Hal_DVI_clklose_det(FALSE);
    }
#endif

    return bIsLoss;
}

//-------------------------------------------------------------------------------------------------
/// Reset the DVI setting
//-------------------------------------------------------------------------------------------------
void mdrv_dvi_reset_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    INPUT_SOURCE_TYPE_t enInputSourceType = INPUT_SOURCE_NONE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        enInputSourceType = gSrcInfo[MAIN_WINDOW].enInputSourceType;
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        enInputSourceType = gSrcInfo[SUB_WINDOW].enInputSourceType;
    }
    else
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return;
    }
    if(Hal_DVI_irq_info(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), IRQ_DVI_CK_CHG) && (!pXCResourcePrivate->stdrv_HDMI._ResetDone))
    {
        MS_U16 u8_IRQ_STATUS;

        u8_IRQ_STATUS = Hal_DVI_irq_info(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), IRQ_DVI_CK_CHG);
        Hal_DVI_irq_clear(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), u8_IRQ_STATUS);
        Hal_DVI_irq_clear(MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType), ~IRQ_ALL_BIT);

        if (u8_IRQ_STATUS == IRQ_DVI_CK_CHG)
        {
            // not do DVI software reset at here; Avoid the snow image

            pXCResourcePrivate->stdrv_HDMI._ResetDone = TRUE;
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(Hal_DVI_irq_info(IRQ_DVI_CK_CHG) && (!pXCResourcePrivate->stdrv_HDMI._ResetDone))
    {
        MS_U16 u8_IRQ_STATUS;

        u8_IRQ_STATUS = Hal_DVI_irq_info(IRQ_DVI_CK_CHG);
        Hal_DVI_irq_clear(u8_IRQ_STATUS);
        Hal_DVI_irq_clear(~IRQ_ALL_BIT);

        if (u8_IRQ_STATUS == IRQ_DVI_CK_CHG)
        {
            // not do DVI software reset at here; Avoid the snow image

            pXCResourcePrivate->stdrv_HDMI._ResetDone = TRUE;
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
}

void mdrv_dvi_reset(void)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_RESET, NULL) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#ifdef DVI_NEW_Patch
// 20081226 - DVI+HDCP snow noise patch - start ...
//-------------------------------------------------------------------------------------------------
/// avoid the snow image while AC on/off and source change for DVI 70Mhz issue, setting 1
/// @param  enInputPortType                \b IN: Input source
//-------------------------------------------------------------------------------------------------
void mdrv_dvi_clock_70mhz_swpatch1(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    MS_BOOL bClockLess70MHz;
    E_MUX_INPUTPORT enMuxPort =MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType);

//    if(MDrv_ReadByte( H( REG_HDMI_INT_STATUS) ) & BIT5)
    if(R2BYTEMSK(REG_HDMI_61_L, BIT13))
    {
        #if ( WATCH_DOG == ENABLE )
            MDrv_Sys_ClearWatchDog();
        #endif

        switch(enMuxPort)
        {
        default:
        case INPUT_PORT_DVI0:
        case INPUT_PORT_DVI1:
        case INPUT_PORT_DVI3:
//            if(!(MDrv_ReadByte(H_BK_ADC_DTOP(0x57)) & BIT3)) // DVI clock is stable
            if(!Hal_DVI_clk_stable_usingportc(NO)) // DVI clock is stable
            {
                //MDRV_HDMIRX_MSG_INFO("\r\nAaron - HDCP status = %bx\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x79)));
//                if(!(R2BYTEMSK(0x2C33, BIT5))) // Video clock is unstable
                if(!(R2BYTEMSK(0x2C32, BIT13))) // Video clock is unstable
                {
                    g_DVI70MhzUnstableCounter++;
                    if(g_DVI70MhzUnstableCounter >= 2)
                    {
                        g_DVI70MhzUnstableCounter = 0;
                        //bClockLess70MHz = (MDrv_ReadByte(H_BK_ADC_DTOP(0x57)) & BIT2) > 0 ? 1 : 0;
                        bClockLess70MHz = Hal_DVI_clk_lessthan70mhz_usingportc(NO); //Not Port C
                        //MDRV_HDMIRX_MSG_INFO("\r\nAaron - CLK_CNT = %bx, AF= %bx\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x57)), MDrv_ReadByte(H_BK_ADC_DTOP(0x57)));
                        //MDRV_HDMIRX_MSG_INFO("\r\nAaron - VCO= %bx\r\n", MDrv_ReadByte(L_BK_ADC_ATOP(0x68)));

                        if (g_bDVILess70MHz != bClockLess70MHz)
                        {
                            //MDRV_HDMIRX_MSG_INFO("\r\nAaron - SWP1  CL70_old = %d, CL70_new = %d\r\n", g_bDVILess70MHz, bClockLess70MHz);
                            g_bDVILess70MHz = bClockLess70MHz;
                            MDrv_HDMI_dvi_adjust(bClockLess70MHz);

                        }
                    }
                }
                g_DVI70MhzPollingCounter = 20; // 20 msec
            }
            break;

        case INPUT_PORT_DVI2:

//            if(!(MDrv_ReadByte(H_BK_ADC_DTOP(0x58)) & BIT3)) // DVI clock is stable
            if(!Hal_DVI_clk_stable_usingportc(YES)) // DVI clock is stable
            {
//                if(!(R2BYTEMSK(0x2C33, BIT5))) // Video clock is unstable
                if(!(R2BYTEMSK(0x2C32, BIT13))) // Video clock is unstable
                {
                    g_DVI70MhzUnstableCounter++;
                    if(g_DVI70MhzUnstableCounter >= 2)
                    {
                        g_DVI70MhzUnstableCounter = 0;

                        //bClockLess70MHz = (MDrv_ReadByte(H_BK_ADC_DTOP(0x58)) & BIT2) > 0 ? 1 : 0;
                        bClockLess70MHz = Hal_DVI_clk_lessthan70mhz_usingportc(YES); //Is Port C
                        //MDRV_HDMIRX_MSG_INFO("\r\nAaron - CLK_CNT = %bx, AF= %bx\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x58)), MDrv_ReadByte(H_BK_ADC_DTOP(0x58)));
                        //MDRV_HDMIRX_MSG_INFO("\r\nAaron - VCO= %bx\r\n", MDrv_ReadByte(L_BK_ADC_ATOP(0x68)));

                        if (g_bDVILess70MHz != bClockLess70MHz)
                        {
                            //MDRV_HDMIRX_MSG_INFO("\r\nAaron - CL70_old = %d, CL70_new = %d\r\n", g_bDVILess70MHz, bClockLess70MHz);
                            g_bDVILess70MHz = bClockLess70MHz;
                            MDrv_HDMI_dvi_adjust(bClockLess70MHz);

                        }
                    }
                }
                g_DVI70MhzPollingCounter = 20; // 20 msec
            }
            break;
        }

        // clear interrupt status
        W2BYTEMSK(REG_HDMI_63_L, BIT13, BIT13);  //REG_HDMI_INT_CLEAR
        W2BYTEMSK(REG_HDMI_63_L,     0, BIT13);  //REG_HDMI_INT_CLEAR
        //MDRV_HDMIRX_MSG_INFO("\r\nAaron- DVICLOCK_PATH = %d\r\n", DVICLOCK_PATH);
    }
}

//-------------------------------------------------------------------------------------------------
/// avoid the snow image while AC on/off and source change for DVI 70Mhz issue, setting 2
/// @param  enInputPortType                \b IN: Input source
//-------------------------------------------------------------------------------------------------
void mdrv_dvi_clock_70mhz_swpatch2(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType) //NO_USE
{
    MS_BOOL bClockLess70MHz;
    static MS_U16 g_DVITMDSClock = 0;
    MS_U16 u16DVITMDSClock, u16DVIClockDiff;
    E_MUX_INPUTPORT enMuxPort =MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType);

//    if(R2BYTEMSK(0x2C33, BIT5)) // Video clock is stable
    if(R2BYTEMSK(0x2C32, BIT13)) // Video clock is stable
    {
        g_DVI70MhzUnstableCounter = 0;
        g_DVI70MhzPollingCounter = 0;

        #if ( WATCH_DOG == ENABLE )
            //msAPI_Timer_ResetWDT();
            MDrv_Sys_ClearWatchDog();
        #endif

        switch(enMuxPort)
        {
        default:
        case INPUT_PORT_DVI0:
        case INPUT_PORT_DVI1:
        case INPUT_PORT_DVI3:
            if(Hal_DVI_clk_stable_usingportc(NO)) // input clock is not really stable
            {
                //MDRV_HDMIRX_MSG_INFO("\r\nAaron - False alarm, SWP2 TMDS Clock= %d\r\n", MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF);
                return;
            }
            break;

        case INPUT_PORT_DVI2:
            if(Hal_DVI_clk_stable_usingportc(YES)) // input clock is not really stable
            {
                //MDRV_HDMIRX_MSG_INFO("\r\nAaron - False alarm, SWP2 TMDS Clock= %d\r\n", MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF);
                return;
            }
            break;
        }

//        u16DVITMDSClock = MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF;
        u16DVITMDSClock = R2BYTE(REG_HDMI_11_L) & 0x1FFF;
        u16DVIClockDiff = (g_DVITMDSClock >= u16DVITMDSClock) ? (g_DVITMDSClock - u16DVITMDSClock) : (u16DVITMDSClock - g_DVITMDSClock);
        if(u16DVIClockDiff > 10) // tolerance = 1MHz
        {
            //MDRV_HDMIRX_MSG_INFO("\r\nAaron - SWP2 Clock_Old = %d <-> Clock_New = %d \r\n", g_DVITMDSClock, u16DVITMDSClock);
            g_DVITMDSClock = u16DVITMDSClock;
            g_DVI70MhzPollingCounter = 20; // 20 msec
            return;
        }
//        bClockLess70MHz = (MDrv_ReadByte(H(__REG_HDMI(0x11))) & 0x60) > 0 ? 0 : 1;
        bClockLess70MHz = (R2BYTEMSK(REG_HDMI_11_L, HBMASK) & 0x60) > 0 ? 0 : 1;
     //MDRV_HDMIRX_MSG_INFO("\r\nAaron - HDCP status = %bx, INT timer= %d\r\n", MDrv_ReadByte(L_BK_ADC_DTOP(0x79)), MDrv_ReadByte(H(__REG_HDMI(0x09))));
     //MDRV_HDMIRX_MSG_INFO("\r\nAaron - SWP2 CL70_old = %d, CL70_new = %d\r\n", g_bDVILess70MHz, bClockLess70MHz);
     //MDRV_HDMIRX_MSG_INFO("\r\nAaron - SWP2 TMDS Clock = %d\r\n", MDrv_Read2Byte(__REG_HDMI(0x11)) & 0x1FFF );
        if (g_bDVILess70MHz != bClockLess70MHz)
        {
            g_bDVILess70MHz = bClockLess70MHz;
            MDrv_HDMI_dvi_adjust(bClockLess70MHz);
        }
    }
}
// 20081226 - DVI+HDCP snow noise patch - end ...
#endif
//-------------------------------------------------------------------------------------------------
/// Get/Set error status
/// @param  u8value                \b IN: Write value
/// @param  bread                   \b IN: Read/Write (1/0)
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_err_status_update_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U8 u8value, MS_BOOL bread)
{
   MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
   _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
   u8Return = Hal_HDMI_err_status_update(ucHDMIInfoSource, u8value,  bread);
   _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
   return u8Return;
}

MS_U8 MDrv_HDMI_err_status_update(MS_U8 u8value, MS_BOOL bread)
{
    stHDMI_ERR_STATUS_UPDATE XCArgs = {0};

    XCArgs.u8value = u8value;
    XCArgs.bread = bread;
    XCArgs.u8ReturnValue = 0;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.u8ReturnValue;
}

MS_U16 MDrv_HDMI_GetTMDSFreq_U2(void* pInstance)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    return Hal_HDMI_GetTMDSFreq();
#else
    return 0;
#endif
}

MS_U16 MDrv_HDMI_GetTMDSFreq(void)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stHDMI_GET_TMDS_FREQ XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_TMDS_FREQ, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// HDMI packet reset
/// @param  breset                \b IN: Enabl reset or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_pkt_reset_internal(void *pInstance, HDMI_REST_t breset )
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDMI_pkt_reset(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType), breset);
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDMI_pkt_reset(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType), breset);
    }
#else
    Hal_HDMI_pkt_reset(breset);
#endif

#if SUPPORT_HDMI_RX_NEW_FEATURE
    //pkt reset cannot clear status in DVI mode  in non-immeswitch case, RD suggest to use DVI sw reset
    if(!pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH) // Only apply in non-imme switch case
    {
        if(breset & REST_HDMI_STATUS )
        {
            Hal_DVI_Software_Reset(INPUT_PORT_DVI0, BIT(6));
        }
    }
#else
    // Do nothing
#endif
}

void MDrv_HDMI_pkt_reset_U2( void* pInstance, HDMI_REST_t breset )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDMI_pkt_reset_internal(pInstance, breset);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_HDMI_pkt_reset( HDMI_REST_t breset )
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_PKT_RESET XCArgs;
    XCArgs.breset = breset;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PKT_RESET, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable Hot plug Detection out enable
/// @param  benable                \b IN: Enabl output or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_hpd_en(MS_BOOL benable)
{
    Hal_HDMI_hpd_en(benable);
}

void MDrv_HDCP_initproductionkey_internal(void *pInstance, MS_U8 * pu8HdcpKeyData )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8HdcpKeyDataBackUp, pu8HdcpKeyData, HDCP_KEY_LENGTH * sizeof(MS_U8));// HDCP key length is 289 byte

    if(bInitEnable == FALSE)
    {
        MDRV_HDMIRX_MSG_INFO("[%s] Please follow the flow: MDrv_HDMI_init() -> MDrv_HDCP_initproductionkey()\n", __FUNCTION__);
        assert(bInitEnable != FALSE);
    }

    if(!pXCResourcePrivate->stdrv_HDMI.bHDCP14RxREEFlag)
    {
        pXCResourcePrivate->stdrv_HDMI.bHDCP14RxREEFlag = TRUE;
    }

    Hal_HDCP_initproductionkey(pu8HdcpKeyData);
}

//-------------------------------------------------------------------------------------------------
/// Init the HDCP production key
/// @param  pu8HdcpKeyData                \b IN: HDCP key
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_initproductionkey_U2( void* pInstance, MS_U8 * pu8HdcpKeyData )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    MDrv_HDCP_initproductionkey_internal(pInstance, pu8HdcpKeyData);

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_HDCP_initproductionkey( MS_U8 * pu8HdcpKeyData )
{
    stHDCP_INITPRODUCTIONKEY XCArgs;
    XCArgs.pu8HdcpKeyData = pu8HdcpKeyData;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_CHECK_HDCP14_KEY, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//-------------------------------------------------------------------------------------------------
/// Clear the HDCP Flag
//-------------------------------------------------------------------------------------------------
void MDrv_HDCP_ClearStatus_U2( void* pInstance )
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        Hal_HDCP_clearflag(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType));
    }
    else if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
    {
        Hal_HDCP_clearflag(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType));
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    Hal_HDCP_clearflag();
#endif
}

void MDrv_HDCP_ClearStatus( void )
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_CLEARSTATUS, NULL) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Enable audio output or not
/// @param  bEnable                \b IN: Enable or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_audio_output_U2( void* pInstance, MS_BOOL bEnable )
{
    Hal_HDMI_audio_output(bEnable);
}

void MDrv_HDMI_audio_output( MS_BOOL bEnable )
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_AUDIO_OUTPUT XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_OUTPUT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// audio content protection info
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_audio_cp_hdr_info_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u8Return = Hal_HDMI_audio_content_protection_info(ucHDMIInfoSource);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u8Return;
}

MS_U8 MDrv_HDMI_audio_cp_hdr_info(void)
{
    stHDMI_AUDIO_CP_HDR_INFO XCArgs = {0};

    XCArgs.u8ReturnValue = 0;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.u8ReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the AVI infoframe information
/// @param  u8byte                \b IN: byte info selection
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_avi_infoframe_info(MS_U8 u8byte)
{
    return Hal_HDMI_avi_infoframe_info(HDMI_INFO_SOURCE0, u8byte);
}

//-------------------------------------------------------------------------------------------------
/// Get manufactor ID information(0x47~0x53)
/// @param  u8byte *               \b IN: byte info selection
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Get_MID_info_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U8 * pu8ManufactorData)
{
    MS_U8 uctemp = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);

    Hal_HDMI_packet_ctrl(ucHDMIInfoSource, 0x04); //set SPD

    for(uctemp = 0; uctemp < 25; uctemp++)
    {
        (pu8ManufactorData[uctemp]) = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, uctemp +2 *(47 -40) +1);
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
}

void MDrv_HDMI_Get_MID_info(MS_U8 * pu8ManufactorData)
{
    stHDMI_GET_MID_INFO XCArgs;

    XCArgs.pu8ManufactorData = pu8ManufactorData;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_MID_INFO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI packet value
/// @param  u8state                \b IN: MS_HDMI_PACKET_STATE_t
/// @param  u8byte_idx                \b IN: Byte index
/// @param  *pValue                \b IN: packet value
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_get_packet_value_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pValue)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    bReturn = Hal_HDMI_get_packet_value(ucHDMIInfoSource, u8state, u8byte_idx, pValue);
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MDrv_HDMI_get_packet_value(MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pValue)
{
    stHDMI_GET_PACKET_VALUE XCArgs;

    XCArgs.u8state = u8state;
    XCArgs.u8byte_idx = u8byte_idx;
    XCArgs.pValue = pValue;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.bReturnValue = FALSE;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_PACKET_VALUE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.bReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the audio channel status
/// @param  u8byte                \b IN: byte info selection
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_audio_channel_status_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    u8Return = Hal_HDMI_audio_channel_status(ucHDMIInfoSource, u8byte);
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
    return u8Return;
}

MS_U8 MDrv_HDMI_audio_channel_status(MS_U8 u8byte)
{
    stHDMI_AUDIO_CHANNEL_STATUS XCArgs = {0};

    XCArgs.u8byte = u8byte;
    XCArgs.u8ReturnValue = 0;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.u8ReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the HDCP status
/// @return @ref MS_HDCP_STATUS_INFO_t
//-------------------------------------------------------------------------------------------------
MS_U16 _MDrv_HDCP_GetStatus(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if SUPPORT_HDMI_RX_NEW_FEATURE
    MS_U16 u16Status = 0;
    if(IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType))
    {
        u16Status = Hal_HDCP_getstatus(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[MAIN_WINDOW].enInputSourceType));
    }
    else
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        XC_RESOURCE_PRIVATE* pXCResourcePrivate_1 = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(1)],(void**)(&pXCResourcePrivate_1));
        if(IsSrcTypeHDMI(gSrcInfo_1[MAIN_WINDOW].enInputSourceType))
        {
            u16Status = Hal_HDCP_getstatus(MDrv_XC_Mux_GetHDMIPort(g_pDevice1Instance, gSrcInfo_1[MAIN_WINDOW].enInputSourceType));
        }
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_HDMI_Mode = u16Status & BIT(0);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_Reserve_1 = u16Status & BIT(1);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_HDCP_Ver = u16Status & BIT(2);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_AVMute = u16Status & BIT(3);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_ENC_En = u16Status & BIT(4);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_AKsv_Rcv = u16Status & BIT(5);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_ENC_Dis = u16Status & BIT(6);
        pXCResourcePrivate_1->stdrv_HDMI.enStatus_info.b_St_Reserve_7 = u16Status & BIT(7);
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#else
        if(IsSrcTypeHDMI(gSrcInfo[SUB_WINDOW].enInputSourceType))
        {
            u16Status = Hal_HDCP_getstatus(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[SUB_WINDOW].enInputSourceType));
        }
#endif
    }
#else
    MS_U16 u16Status = Hal_HDCP_getstatus();
#endif

    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_HDMI_Mode = u16Status & BIT(0);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_Reserve_1 = u16Status & BIT(1);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_HDCP_Ver = u16Status & BIT(2);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_AVMute = u16Status & BIT(3);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_ENC_En = u16Status & BIT(4);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_AKsv_Rcv = u16Status & BIT(5);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_ENC_Dis = u16Status & BIT(6);
    pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_Reserve_7 = u16Status & BIT(7);

    return u16Status;
}

MS_U16 MDrv_HDCP_GetStatus_U2(void* pInstance)
{
    MS_U16 usHDCPStatus = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    usHDCPStatus = _MDrv_HDCP_GetStatus(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return usHDCPStatus;
}

MS_HDCP_STATUS_INFO_t* MDrv_HDCP_GetStatus(void )
{
    stHDCP_GETSTATUS XCArgs = {0};
    static MS_HDCP_STATUS_INFO_t stHDCPStatus = {0};

    XCArgs.usHDCPStatus = 0;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_GETSTATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
    else
    {
        stHDCPStatus.b_St_HDMI_Mode = ((XCArgs.usHDCPStatus &BIT(0))? TRUE: FALSE);
        stHDCPStatus.b_St_Reserve_1 = ((XCArgs.usHDCPStatus &BIT(1))? TRUE: FALSE);
        stHDCPStatus.b_St_HDCP_Ver = ((XCArgs.usHDCPStatus &BIT(2))? TRUE: FALSE);
        stHDCPStatus.b_St_AVMute = ((XCArgs.usHDCPStatus &BIT(3))? TRUE: FALSE);
        stHDCPStatus.b_St_ENC_En = ((XCArgs.usHDCPStatus &BIT(4))? TRUE: FALSE);
        stHDCPStatus.b_St_AKsv_Rcv = ((XCArgs.usHDCPStatus &BIT(5))? TRUE: FALSE);
        stHDCPStatus.b_St_ENC_Dis = ((XCArgs.usHDCPStatus &BIT(6))? TRUE: FALSE);
        stHDCPStatus.b_St_Reserve_7 = ((XCArgs.usHDCPStatus &BIT(7))? TRUE: FALSE);
    }

    return &stHDCPStatus;
}

void MDrv_DVI_HF_adjust_internal(void)
{
    Hal_DVI_HF_adjust();
}

void MDrv_DVI_HF_adjust_U2( void* pInstance )
{
    MDrv_DVI_HF_adjust_internal();
}

void MDrv_DVI_HF_adjust(void)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_DVI_HF_ADJUST, NULL) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Returne Phase status
/// @param  bClockLessThan70MHz                \b IN: u8Channel
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_DVI_ChannelPhaseStatus_U2(void* pInstance, MS_DVI_CHANNEL_TYPE u8Channel)
{
    return Hal_DVI_ChannelPhaseStatus(u8Channel);
}

MS_U16 MDrv_DVI_ChannelPhaseStatus( MS_DVI_CHANNEL_TYPE u8Channel)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stDVI_CHANNELPHASESTATUS XCArgs;
    XCArgs.u8Channel = u8Channel;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_CHANNELPHASESTATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Check DVI accumulator and reset status.
/// @param
/// Due to DVI accumulator may be overflow when previous source is MHL,
/// this function will monitor MHL port's accumulator, and reset when it hits boundary.
//-------------------------------------------------------------------------------------------------
void MDrv_DVI_Accumulator_Monitor(void)
{
    Hal_DVI_Accumulator_Monitor();
}

//-------------------------------------------------------------------------------------------------
/// Adjust the ADC for DVI by the condition is less than 70Mhz or not
/// @param  bClockLessThan70MHz                \b IN: Is Less 70Mhz or not
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_dvi_adjust_U2( void* pInstance, MS_BOOL bClockLessThan70MHz)
{
    Hal_DVI_adc_adjust(bClockLessThan70MHz);
}

void MDrv_HDMI_dvi_adjust( MS_BOOL bClockLessThan70MHz)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_DVI_ADJUST XCArgs;
    XCArgs.bClockLessThan70MHz = bClockLessThan70MHz;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_DVI_ADJUST, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// audio content payload info
/// @param  u8byte                \b IN: Byte number
/// @return @ref MS_U8
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_audio_cp_data_info(MS_U8 u8byte)
{
    return Hal_HDMI_audio_payload_info(HDMI_INFO_SOURCE0, u8byte);
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI packet
/// @param  bEnable                \b IN: Enable receive or not
/// @return @ref MS_HDMI_PACKET_VALUE_t
//-------------------------------------------------------------------------------------------------
void MDrv_HDMI_Packet_Received_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEnable, MS_U32 ulPacketStatus)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(bEnable)
    {
        pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] = ulPacketStatus;
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

MS_HDMI_PACKET_VALUE_t MDrv_HDMI_Packet_Received(MS_BOOL bEnable)
{
    stHDMI_PACKET_RECEIVED XCArgs = {0};
    MS_HDMI_PACKET_VALUE_t stPacketValue = {0};

    XCArgs.bEnable = bEnable;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.ulPacketStatus = 0;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_GET_PACKET_RECEIVE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
    else
    {
        stPacketValue.PKT_MPEG_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_MPEG_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_AUI_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_AUDIO_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_SPD_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_SPD_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_AVI_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_AVI_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_GC_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_GCP_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_ASAMPLE_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_AUDIO_SAMPLE_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_ACR_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_ACR_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_VS_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_NULL_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_NULL_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_ISRC2_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_ISRC2_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_ISRC1_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_ISRC1_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_ACP_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_ACP_PACKET_RECEIVE_FLAG;
        stPacketValue.PKT_ONEBIT_AUD_VALUE = (MS_U16)XCArgs.ulPacketStatus &HDMI_STATUS_DSD_PACKET_RECEIVE_FLAG;
    }

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PACKET_RECEIVED, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return stPacketValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI color format
/// @return @ref MS_HDMI_COLOR_FORMAT
//-------------------------------------------------------------------------------------------------
MS_HDMI_COLOR_FORMAT MDrv_HDMI_Get_ColorFormat_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_HDMI_COLOR_FORMAT pkt_ColorFormat=MS_HDMI_COLOR_UNKNOWN;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);

    switch(Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_1)& 0x60 )
    {
        case 0x00:
            pkt_ColorFormat = MS_HDMI_COLOR_RGB;
            break;
        case 0x40:
            pkt_ColorFormat = MS_HDMI_COLOR_YUV_444;
            break;
        case 0x20:
            pkt_ColorFormat = MS_HDMI_COLOR_YUV_422;
            break;
        case 0x60:
            pkt_ColorFormat = MS_HDMI_COLOR_YUV_420;
            break;
        default:
            break;
    };

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

    return pkt_ColorFormat;
}

MS_HDMI_COLOR_FORMAT MDrv_HDMI_Get_ColorFormat(void)
{
    stHDMI_GET_COLORFORMAT XCArgs = {0};

    XCArgs.eReturnValue = MS_HDMI_COLOR_UNKNOWN;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_COLORFORMAT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI color range
/// @return @ref EN_HDMI_COLOR_RANGE
//-------------------------------------------------------------------------------------------------
EN_HDMI_COLOR_RANGE MDrv_HDMI_Get_ColorRange_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    EN_HDMI_COLOR_RANGE enPKTColorRange = E_HDMI_COLOR_RANGE_DEFAULT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);

    if((Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_1)& 0x60) == 0x00)  // RGB
    {
        switch(Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_3) & 0x0C)
        {
            case 0x00:
                enPKTColorRange = E_HDMI_COLOR_RANGE_DEFAULT;
                break;
            case 0x04:
                enPKTColorRange = E_HDMI_COLOR_RANGE_LIMIT;
                break;
            case 0x08:
                enPKTColorRange = E_HDMI_COLOR_RANGE_FULL;
                break;
            case 0x0C:
                enPKTColorRange = E_HDMI_COLOR_RANGE_RESERVED;
                break;
            default:
                break;
        };
    }
    else // YUV
    {
        switch(Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_5) & 0xC0)
        {
            case 0x00:
                enPKTColorRange = E_HDMI_COLOR_RANGE_LIMIT;
                break;
            case 0x40:
                enPKTColorRange = E_HDMI_COLOR_RANGE_FULL;
                break;
            case 0x80:
                enPKTColorRange = E_HDMI_COLOR_RANGE_RESERVED;
                break;
            case 0xC0:
                enPKTColorRange = E_HDMI_COLOR_RANGE_RESERVED;
                break;
            default:
                break;
        };
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

    return enPKTColorRange;
}

EN_HDMI_COLOR_RANGE MDrv_HDMI_Get_ColorRange(void)
{
    stHDMI_GET_COLORRANGE XCArgs = {0};

    XCArgs.eReturnValue = E_HDMI_COLOR_RANGE_RESERVED;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_COLORRANGE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI aspect ratio info
/// @param  bEn_PAR                \b IN: Enable to get the Picture Aspect Ratio
/// @param  bEn_AFAR              \b IN: Enable to get the Active Format Aspect Ratio
/// @return @ref MS_HDMI_AR_TYPE
//-------------------------------------------------------------------------------------------------
MS_HDMI_AR_TYPE MDrv_HDMI_Get_AspectRatio_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR)
{
    MS_HDMI_AR_TYPE enPar_type;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    MS_U8 u8AR = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_2) & HDMI_AR_REG_MASK;
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

    // Picture Aspect Ratio - PAR
    if(bEn_PAR)
    {
        switch(u8AR & HDMI_AR_MASK)
        {
            case 0x00:     // IF0[13..12] 00
                enPar_type = HDMI_Pic_AR_NODATA;
                break;
            case 0x10:     // IF0[13..12] 01, 4:3
                enPar_type = HDMI_Pic_AR_4x3;
                break;
            case 0x20:     // IF0[13..12] 10, 16:9
                enPar_type = HDMI_Pic_AR_16x9;
                break;
            case 0x30:     // IF0[13..12] 11, reserved
            default:
                enPar_type = HDMI_Pic_AR_RSV;
                break;
        };
    }
    else
    {
        enPar_type = HDMI_Pic_AR_RSV;
    }

    return enPar_type;
}

MS_HDMI_AR_TYPE MDrv_HDMI_Get_AspectRatio(MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR)
{
    stHDMI_GET_ASPECTRATIO XCArgs = {0};

    XCArgs.bEn_PAR = bEn_PAR;
    XCArgs.bEn_AFAR = bEn_AFAR;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.eReturnValue = HDMI_AF_AR_Reserve_0;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_ASPECTRATIO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}
//-------------------------------------------------------------------------------------------------
/// Get the HDMI aspect ratio info
/// @param  bEn_AFAR              \b IN: Enable to get the Active Format Aspect Ratio
/// @return @ref MS_HDMI_AR_TYPE
//-------------------------------------------------------------------------------------------------
MS_HDMI_AR_TYPE MDrv_HDMI_Get_ActiveFormat_AspectRatio_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEn_AFAR)
{
    MS_HDMI_AR_TYPE enAfar_types;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    MS_U8 u8AR = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_2) & HDMI_AR_REG_MASK;
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

    // Active Format Aspect Ratio - AFAR
    if(bEn_AFAR)
    {
        switch(u8AR & HDMI_AFAR_MASK)
        {
            case 0x02:    // IF0[11..8] 0010, box 16:9 (top)
                enAfar_types = HDMI_AF_AR_16x9_Top;
                break;
            case 0x03:    // IF0[11..8] 0011, box 14:9 (top)
                enAfar_types = HDMI_AF_AR_14x9_Top;
                break;
            case 0x04:    // IF0[11..8] 0100, box >16:9 (centre)
                enAfar_types = HDMI_AF_AR_GT_16x9;
                break;
            case 0x08:    // IF0[11..8] 1000, same as picture
                enAfar_types = HDMI_AF_AR_SAME;
                break;
            case 0x09:    // IF0[11..8] 1001, 4:3 Center
                enAfar_types = HDMI_AF_AR_4x3_C;
                break;
            case 0x0A:    // IF0[11..8] 1010, 16:9 Center
                enAfar_types = HDMI_AF_AR_16x9_C;
                break;
            case 0x0B:    // IF0[11..8] 1011, 14:9 Center
                enAfar_types = HDMI_AF_AR_14x9_C;
                break;
            case 0x0D:    // IF0[11..8] 1101, 4:3 with shoot and protect 14:9 centre
                enAfar_types = HDMI_AF_AR_4x3_with_14x9_C;
                break;
            case 0x0E:    // IF0[11..8] 1110, 16:9 with shoot and protect 14:9 centre
                enAfar_types = HDMI_AF_AR_16x9_with_14x9_C;
                break;
            case 0x0F:    // IF0[11..8] 1111, 16:9 with shoot and protect 4:3 centre.
                enAfar_types = HDMI_AF_AR_16x9_with_4x3_C;
                break;

            default:
                enAfar_types = HDMI_AF_AR_SAME;
                break;
        }
    }
    else
    {
        enAfar_types = HDMI_AF_AR_SAME;
    }

    return enAfar_types;
}

MS_HDMI_AR_TYPE MDrv_HDMI_Get_ActiveFormat_AspectRatio(MS_BOOL bEn_AFAR)
{
    stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO XCArgs = {0};

    XCArgs.bEn_AFAR = bEn_AFAR;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.eReturnValue = HDMI_AF_AR_Reserve_0;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the content type reported from player
/// @return @ref MS_HDMI_CONTENT_TYPE
//-------------------------------------------------------------------------------------------------
MS_HDMI_CONTENT_TYPE MDrv_HDMI_Get_Content_Type_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_HDMI_CONTENT_TYPE enCN = MS_HDMI_CONTENT_NoData;
    MS_U8 u8Temp = 0;
    MS_BOOL bITC = 0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    bITC = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_3)& 0x80;
    u8Temp = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_5)& 0x30;
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

    switch(u8Temp)
    {
        case 0x00:
            if(bITC)
                enCN = MS_HDMI_CONTENT_Graphics;
            else
                enCN = MS_HDMI_CONTENT_NoData;
            break;
        case 0x10:
            enCN = MS_HDMI_CONTENT_Photo;
            break;
        case 0x20:
            enCN = MS_HDMI_CONTENT_Cinema;
            break;
        case 0x30:
            enCN = MS_HDMI_CONTENT_Game;
            break;
        default:
            enCN = MS_HDMI_CONTENT_NoData;
            break;
    };

    return enCN;
}

MS_HDMI_CONTENT_TYPE MDrv_HDMI_Get_Content_Type(void)
{
    stHDMI_GET_CONTENT_TYPE XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.enReturnValue = MS_HDMI_CONTENT_NoData;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.enReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the Pixel repetition from player
/// @return @ref MS_HDMI_COLOR_FORMAT
//-------------------------------------------------------------------------------------------------
MS_U8 MDrv_HDMI_Get_Pixel_Repetition_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_U8 u8Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    u8Return = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_5)& 0x0F;
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
    return u8Return;
}

MS_U8 MDrv_HDMI_Get_Pixel_Repetition(void)
{
    stHDMI_GET_PIXEL_REPETITION XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.u8ReturnValue = 0;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.u8ReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the AVI InfoFrame version
/// @return @ref EN_AVI_INFOFRAME_VERSION
//-------------------------------------------------------------------------------------------------
EN_AVI_INFOFRAME_VERSION MDrv_HDMI_Get_AVIInfoFrameVer_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    EN_AVI_INFOFRAME_VERSION enPktVersion= E_AVI_INFOFRAME_VERSION_NON;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    enPktVersion = Hal_HDMI_avi_infoframe_info_ver(ucHDMIInfoSource);
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
    return enPktVersion;
}

EN_AVI_INFOFRAME_VERSION MDrv_HDMI_Get_AVIInfoFrameVer(void)
{
    stHDMI_GET_AVIINFOFRAMEVER XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.eReturnValue = E_AVI_INFOFRAME_VERSION_NON;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Get the active information present from AVI
/// @return
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_HDMI_Get_AVIInfoActiveInfoPresent_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);

    if(Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_1) & BIT(4))
    {
        bReturn = TRUE;
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

    return bReturn;
}

MS_BOOL MDrv_HDMI_Get_AVIInfoActiveInfoPresent(void)
{
    stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.bReturnValue = FALSE;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.bReturnValue;
}

//-------------------------------------------------------------------------------------------------
/// Pull DVI Clock to low
/// @param  bPullLow                          \b IN: Set DVI clock to low
/// @param  enInputPortType              \b IN: Input source
//-------------------------------------------------------------------------------------------------
void MDrv_DVI_ClkPullLow_U2(void* pInstance, MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
    Hal_DVI_ClkPullLow(bPullLow, enInputPortType);
}

void MDrv_DVI_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_CLKPULLLOW XCArgs;
    XCArgs.bPullLow = bPullLow;
    XCArgs.enInputPortType = enInputPortType;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_CLKPULLLOW, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Switch DVI source
/// @param  enInputPortType              \b IN: Input source
//-------------------------------------------------------------------------------------------------
void MDrv_DVI_SwitchSrc_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucHDMIInfoSource)
{
    Hal_DVI_SwitchSrc(enInputPortType, ucHDMIInfoSource);
}

void MDrv_DVI_SwitchSrc(E_MUX_INPUTPORT enInputPortType)
{
    stDVI_SWITCHSRC XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.enInputPortType = enInputPortType;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_SWITCHSRC, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
}

void MDrv_DVI_IMMESWITCH_PS_SW_Path(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH) // Only apply in imme switch case
    {
        Hal_DVI_IMMESWITCH_PS_SW_Path();
    }
}

/// lib version
HDMI_Ret_Value MDrv_HDMI_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion)
{
    if (!ppVersion)
    {
        return E_HDMI_RET_FAIL;
    }

    *ppVersion = &_drv_hdmi_version;
    return E_HDMI_RET_OK;
}

HDMI_Ret_Value MDrv_HDMI_GetLibVer(const MSIF_Version **ppVersion)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETLIBVER XCArgs;
    XCArgs.ppVersion = ppVersion;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETLIBVER, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// lib information
HDMI_Ret_Value  MDrv_HDMI_GetInfo_U2(void* pInstance, MS_HDMI_INFO *pInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    memcpy((void*)pInfo, (void*)&(pXCResourcePrivate->stdrv_HDMI._info), sizeof(MS_HDMI_INFO));

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return E_HDMI_RET_OK;
}

HDMI_Ret_Value  MDrv_HDMI_GetInfo(MS_HDMI_INFO *pInfo)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETINFO XCArgs;
    XCArgs.pInfo = pInfo;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETINFO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// curretn status
HDMI_Ret_Value  MDrv_HDMI_GetStatus_U2(void* pInstance, MS_HDMI_Status *pStatus)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy((void*)pStatus, (void*)&(pXCResourcePrivate->stdrv_HDMI._status), sizeof(MS_HDMI_Status));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return E_HDMI_RET_OK;
}

HDMI_Ret_Value  MDrv_HDMI_GetStatus(MS_HDMI_Status *pStatus)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETSTATUS XCArgs;
    XCArgs.pStatus = pStatus;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETSTATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// DDC
static void MDrv_HDMI_ReadEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u8EDIDSize)
{
    MS_U16 i;

    HAL_HDMI_DDCRam_SelectPort(eDDCRamType);

    for( i = 0; i < u8EDIDSize; i++)
    {
        if(eDDCRamType == E_XC_PROG_VGA_EDID)
        {
            DDC_WRITE_MASK(REG_DDC_A_ACCESS,REG_DDC_A_ACCESS_BITS(1),REG_DDC_A_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEADDR,REG_DDC_A_WRITEADDR_BITS(i),REG_DDC_A_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_A_READPULSE,REG_DDC_A_READPULSE_BITS(1),REG_DDC_A_READPULSE_MASK);
            MsOS_DelayTaskUs(2);
            *(u8EDID + i) = (MS_U8)(DDC_READ_MASK(REG_DDC_A_READDATA, REG_DDC_A_READDATA_MASK)>>8);
        }
        else
        {
            DDC_WRITE_MASK(REG_DDC_D_ACCESS,REG_DDC_D_ACCESS_BITS(1),REG_DDC_D_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEADDR,REG_DDC_D_WRITEADDR_BITS(i),REG_DDC_D_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_D_READPULSE,REG_DDC_D_READPULSE_BITS(1),REG_DDC_D_READPULSE_MASK);
            MsOS_DelayTaskUs(2);
            *(u8EDID + i) = (MS_U8)(DDC_READ_MASK(REG_DDC_D_READDATA, REG_DDC_D_READDATA_MASK)>>8);
        }
    }
}

// DDC
static void MDrv_HDMI_WriteEDID(E_XC_DDCRAM_PROG_TYPE eDDCRamType, MS_U8 *u8EDID, MS_U16 u8EDIDSize)
{
    MS_U16 i;

    HAL_HDMI_DDCRam_SelectPort(eDDCRamType);

    for( i = 0; i < u8EDIDSize; i++)
    {
        if(eDDCRamType == E_XC_PROG_VGA_EDID)
        {
            DDC_WRITE_MASK(REG_DDC_A_ACCESS,REG_DDC_A_ACCESS_BITS(0),REG_DDC_A_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEADDR,REG_DDC_A_WRITEADDR_BITS(i),REG_DDC_A_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEDATA,REG_DDC_A_WRITEDATA_BITS(*(u8EDID + i)),REG_DDC_A_WRITEDATA_MASK);
            DDC_WRITE_MASK(REG_DDC_A_WRITEPULSE,REG_DDC_A_WRITEPULSE_BITS(1),REG_DDC_A_WRITEPULSE_MASK);
        }
        else
        {
            DDC_WRITE_MASK(REG_DDC_D_ACCESS,REG_DDC_D_ACCESS_BITS(0),REG_DDC_D_ACCESS_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEADDR,REG_DDC_D_WRITEADDR_BITS(i),REG_DDC_D_WRITEADDR_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEDATA,REG_DDC_D_WRITEDATA_BITS(*(u8EDID + i)),REG_DDC_D_WRITEDATA_MASK);
            DDC_WRITE_MASK(REG_DDC_D_WRITEPULSE,REG_DDC_D_WRITEPULSE_BITS(1),REG_DDC_D_WRITEPULSE_MASK);
        }
    }
    switch(eDDCRamType)
    {
        case E_XC_PROG_DVI0_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI0_WP,BITS(13:13,1),BMASK(13:13)); // Write Protect 0 ddc
            break;
        case E_XC_PROG_DVI1_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI1_WP,BITS(13:13,1),BMASK(13:13)); // Write Protect 1 ddc
            break;
        case E_XC_PROG_DVI2_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI2_WP,BITS(5:5,1),BMASK(5:5));     // Write Protect 2 ddc
            break;
        case E_XC_PROG_DVI3_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI3_WP,BITS(5:5,1),BMASK(5:5));     //Write Protect  3 ddc
            break;
        case E_XC_PROG_VGA_EDID:
            DDC_WRITE_MASK(REG_DDC_ADC0_WP,BITS(13:13,1),BMASK(13:13)); // Write Protect analog ddc
            break;
        default:
            MS_ASSERT(0);
    }
}

static void MDrv_HDMI_DDCRAM_Enable(E_XC_DDCRAM_PROG_TYPE eDDCRamType)
{
    switch(eDDCRamType)
    {
        case E_XC_PROG_DVI0_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI0_EN,BITS(15:15,1),BMASK(15:15)); // enable digital 0 ddc
            break;
        case E_XC_PROG_DVI1_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI1_EN,BITS(15:15,1),BMASK(15:15)); // enable digital 1 ddc
            break;
        case E_XC_PROG_DVI2_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI2_EN,BITS(7:7,1),BMASK(7:7));     // enable digital 2 ddc
            break;
        case E_XC_PROG_DVI3_EDID:
            DDC_WRITE_MASK(REG_DDC_DVI3_EN,BITS(7:7,1),BMASK(7:7));     // enable digital 3 ddc
            break;
        case E_XC_PROG_VGA_EDID:
            DDC_WRITE_MASK(REG_DDC_ADC0_EN,BITS(15:15,1),BMASK(15:15)); // enable analog ddc
            break;
        default:
            MS_ASSERT(0);
    }
}

void MDrv_HDMI_READ_DDCRAM_U2(void* pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    (u32SizeOfInfo)=(u32SizeOfInfo); //this Var is temp for UNUSE

    MDrv_HDMI_DDCRAM_Enable(pstDDCRam_Info->eDDCProgType);

    MDrv_HDMI_ReadEDID(pstDDCRam_Info->eDDCProgType, pstDDCRam_Info->EDID, pstDDCRam_Info->u16EDIDSize);

    HAL_HDMI_DDCRAM_SetPhyAddr(pstDDCRam_Info);
}

void MDrv_HDMI_READ_DDCRAM(XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_READ_DDCRAM XCArgs;
    XCArgs.pstDDCRam_Info = pstDDCRam_Info;
    XCArgs.u32SizeOfInfo = u32SizeOfInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_READ_DDCRAM, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_PROG_DDCRAM_internal(void *pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    (u32SizeOfInfo)=(u32SizeOfInfo); //this Var is temp for UNUSE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pstDDCRam_Info->eDDCProgType != E_XC_PROG_VGA_EDID)
    {
        memcpy(pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8EDID_DVIBackUp[pstDDCRam_Info->eDDCProgType - E_XC_PROG_DVI0_EDID], pstDDCRam_Info->EDID, EDID_KEY_LENGTH*sizeof(MS_U8));
    }
    MDrv_HDMI_DDCRAM_Enable(pstDDCRam_Info->eDDCProgType);

    MDrv_HDMI_WriteEDID(pstDDCRam_Info->eDDCProgType, pstDDCRam_Info->EDID, pstDDCRam_Info->u16EDIDSize);

    HAL_HDMI_DDCRAM_SetPhyAddr(pstDDCRam_Info);
}

void MDrv_HDMI_PROG_DDCRAM_U2(void* pInstance, XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_HDMI_PROG_DDCRAM_internal(pInstance, pstDDCRam_Info, u32SizeOfInfo);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_HDMI_PROG_DDCRAM(XC_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_PROG_DDCRAM XCArgs;
    XCArgs.pstDDCRam_Info = pstDDCRam_Info;
    XCArgs.u32SizeOfInfo = u32SizeOfInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PROG_DDCRAM, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/** According to the previous experience, some specific source devices would
send strange DVI / HDMI signal in slow frequency(like 480p) let us in trouble.
So it's better to increase DVI updated phase count to 100 lines in low
resolution to overcome that situation, originally DVI updated phase count is 1.
So the patch would check input DVI frequency first, if small than 50MHz, DVI
updated phase count would increase to 100 lines. Otherwise, DVI updated phase
count would be recovered to 1 line. */
void MDrv_HDMI_SetUpdatePhaseLineCount_U2(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    E_MUX_INPUTPORT enPort = INPUT_PORT_NONE_PORT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    enPort = MDrv_XC_Mux_GetHDMIPort(pInstance, enInputSourceType);
    if( enPort == INPUT_PORT_DVI0 || enPort == INPUT_PORT_DVI1 ||
        enPort == INPUT_PORT_DVI3 )
    {
        if( R2BYTEMSK(REG_DVI_ATOP_61_L, BMASK(9:8)) == 2<<8 ||
            R2BYTEMSK(REG_DVI_ATOP_61_L, BMASK(9:8)) == 3<<8 ) //<50Mhz
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0x0000, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0x0000);
//            MDRV_HDMIRX_MSG_INFO("Low resolution\n");

        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0);
//            MDRV_HDMIRX_MSG_INFO("High resolution\n");
        }

    }
    else if(enPort == INPUT_PORT_DVI2)
    {
        if( R2BYTEMSK(REG_DVI_ATOP_6A_L, BMASK(9:8)) == 2<<8 ||
            R2BYTEMSK(REG_DVI_ATOP_6A_L, BMASK(9:8)) == 3<<8 ) //<50Mhz
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0x0000, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0x0000);
//            MDRV_HDMIRX_MSG_INFO("Low resolution 2\n");
        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP_29_L, 0, 0xFF00);
            W2BYTE(REG_DVI_DTOP_2A_L, 0);
//            MDRV_HDMIRX_MSG_INFO("High resolution 2\n");
        }

    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_HDMI_SetUpdatePhaseLineCount(INPUT_SOURCE_TYPE_t enInputSourceType)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_SETUPDATEPHASELINECOUNT XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_Get_PollingStatus_U2(void* pInstance, HDMI_POLLING_STATUS_t **pStatus)
{
    *pStatus = &g_HdmiPollingStatus;
}

void MDrv_HDMI_Get_PollingStatus(HDMI_POLLING_STATUS_t **pStatus)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_GET_POLLINGSTATUS XCArgs;
    XCArgs.pStatus = NULL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_IsHDMI_Mode_U2(void* pInstance)
{
    MS_BOOL tmp;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    tmp= pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_HDMI_Mode;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return tmp;
}

MS_BOOL MDrv_HDMI_IsHDMI_Mode(void)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_ISHDMI_MODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ISHDMI_MODE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Get the extended colorimetry from packet
/// @return @ref MS_HDMI_EXT_COLORIMETRY_FORMAT
//-------------------------------------------------------------------------------------------------
MS_HDMI_EXT_COLORIMETRY_FORMAT MDrv_HDMI_Get_ExtColorimetry_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_HDMI_EXT_COLORIMETRY_FORMAT pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_UNKNOWN;
    MS_U8 u8AVI_info2, u8AVI_info3;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    u8AVI_info2 = Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_2);
    MDRV_HDMIRX_MSG_INFO(" MDrv_HDMI_Get_ExtColorimetry_U2 u8AVI_info2 is 0X%X , avi_info2&0xc0  0x%X \n",u8AVI_info2,(u8AVI_info2&0xc0));
    if((u8AVI_info2&0xc0) == 0xc0 )
    {
        u8AVI_info3= Hal_HDMI_avi_infoframe_info(ucHDMIInfoSource, _BYTE_3);
        MDRV_HDMIRX_MSG_INFO(" MDrv_HDMI_Get_ExtColorimetry_U2 u8AVI_info3 is 0X%X \n",u8AVI_info3);
        switch (u8AVI_info3 & 0x70)
        {
            case 0x00:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_XVYCC601;
                break;
            case 0x10:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_XVYCC709;
                break;
            case 0x20:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_SYCC601;
                break;
            case 0x30:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_ADOBEYCC601;
                break;
            case 0x40:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_ADOBERGB;
                break;
            case 0x50:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_BT2020YcCbcCrc;
                break;
            case 0x60:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_BT2020RGBYCbCr;
                break;
            default:
                pkt_ExtColorimetry = MS_HDMI_EXT_COLOR_UNKNOWN;
                break;
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
    return pkt_ExtColorimetry;
}

MS_HDMI_EXT_COLORIMETRY_FORMAT MDrv_HDMI_Get_ExtColorimetry(void)
{
    stHDMI_GET_EXT_COLORIMETRY XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.enReturnValue = MS_HDMI_EXT_COLOR_UNKNOWN;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.enReturnValue;
}

MS_U16 MDrv_HDMI_GetHDE(void)
{
    return Hal_HDMI_GetHDEInfo(HDMI_INFO_SOURCE0);
}
#if 0
void MDrv_HDMI_ENABLE_IMMESWITCH( MS_BOOL bEnImmeswitch,MS_BOOL bEnPowSaving )
{
    if(SUPPORT_IMMESWITCH)
    {
        g_bIsIMMESWITCH = bEnImmeswitch;
        g_bIMMESWITCH_DVI_POWERSAVING = bEnPowSaving;
    }
    else
    {
        g_bIsIMMESWITCH = FALSE;
        g_bIMMESWITCH_DVI_POWERSAVING = FALSE;
    }
}
#endif

#if 0
void MDrv_HDMI_ImmeswitchPS_Handler(void)
{
    E_MUX_INPUTPORT port;
    if(g_bIMMESWITCH_DVI_POWERSAVING)
    {
        for(port = INPUT_PORT_DVI0; port <=INPUT_PORT_DVI3; port++)
        {
            //if(!Hal_DVI_GetPowerSavingStatus(port)) // not need to check if power saving mode is not enable
            //continue;

            if(Hal_DVI_GetDEStableStatus(port)) // stable
            {
                if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] < 255)
                    u8PSDVIStableCount[port-INPUT_PORT_DVI0] += 1;
            }
            else // unstable
            {
                if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] > 0)
                {
                    Hal_DVI_EnhanceImmeswitch(port, FALSE);
                    MDRV_HDMIRX_MSG_INFO("$$$ MDrv_HDMI_ImmeswitchPS_Handler:  port %d is unstable\n", port-INPUT_PORT_DVI0);
                }
                u8PSDVIStableCount[port-INPUT_PORT_DVI0] = 0;
            }

            if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] >= 3) // enable Immeswithc enhancement only when continous DE stable
            {
                if(u8PSDVIStableCount[port-INPUT_PORT_DVI0] == 3)
                {
                    Hal_DVI_EnhanceImmeswitch(port, TRUE);
                    MDRV_HDMIRX_MSG_INFO("$$$ MDrv_HDMI_ImmeswitchPS_Handler:  port %d is stable\n", port-INPUT_PORT_DVI0);
                }
            }
        }
    }
}

void MDrv_DVI_ForceAllPortsEnterPS(void)
{
    if(g_bIMMESWITCH_DVI_POWERSAVING)
    {
        MDRV_HDMIRX_MSG_INFO("$$$ MDrv_DVI_ForceAllPortsEnterPS \n");
        Hal_DVI_ForceAllPortsEnterPS();
    }
}
#endif

MS_BOOL MDrv_HDMI_Check4K2K_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_BOOL bReturnFlag = FALSE;
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val = E_HDMI_NA;
    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        val = Hal_HDMI_Check_Additional_Format(ucHDMIInfoSource);

        if(Hal_HDMI_Is_Input_Large_166MHz(ucHDMIInfoSource))
        {
            MDRV_HDMIRX_MSG_INFO("~~~ HDMI 4Kx2K case, input bigger than 166Mhz ~~~\n");
            _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
            bReturnFlag = TRUE;
        }
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif

    return bReturnFlag;
}

MS_BOOL MDrv_HDMI_Check4K2K(void)
{
    stHDMI_CHECK_4K2K XCArgs = {0};

    XCArgs.bReturnValue = FALSE;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CHECK_4K2K, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.bReturnValue;
}

void MDrv_HDMI_3D_4Kx2K_Process_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val = E_HDMI_NA;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        val = Hal_HDMI_Check_Additional_Format(ucHDMIInfoSource);

        if(Hal_HDMI_Is_Input_Large_166MHz(ucHDMIInfoSource))
        {
            Hal_HDMI_AVG_ScaleringDown(ucHDMIInfoSource, TRUE);
            MDRV_HDMIRX_MSG_INFO("~~~ HDMI 3D or 4Kx2K scaling down= %x\n", val);
        }
        else
        {
            Hal_HDMI_AVG_ScaleringDown(ucHDMIInfoSource, FALSE);
        }
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);
#else
    // Do nothing
#endif
}

void MDrv_HDMI_3D_4Kx2K_Process(void)
{
    stHDMI_3D_4K2K_PROCESS XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
}

void MDrv_HDMI_AVG_ScaleringDown_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bTrue)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDMI_AVG_ScaleringDown(ucHDMIInfoSource, bTrue);
#else
    // Do nothing
#endif
}

void MDrv_HDMI_AVG_ScaleringDown(MS_BOOL bTrue)
{
    stHDMI_AVG_SCALERINGDOWN XCArgs = {0};

    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;
    XCArgs.bTrue = bTrue;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
}

E_HDMI_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_Check_Additional_Format_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    E_HDMI_ADDITIONAL_VIDEO_FORMAT eAdditionalFormat = E_HDMI_NA;
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        eAdditionalFormat = Hal_HDMI_Check_Additional_Format(ucHDMIInfoSource);
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

#endif

    return eAdditionalFormat;
}

E_HDMI_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_Check_Additional_Format(void)
{
    stHDMI_CHECK_ADDITIONAL_FORMAT XCArgs = {0};

    XCArgs.eReturnValue = E_HDMI_NA;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

E_XC_3D_INPUT_MODE MDrv_HDMI_Get_3D_Structure_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    E_XC_3D_INPUT_MODE e3DStructure = E_XC_3D_INPUT_MODE_NONE;
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        e3DStructure = Hal_HDMI_Get_3D_Structure(ucHDMIInfoSource);
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

#endif

    return e3DStructure;
}

E_XC_3D_INPUT_MODE MDrv_HDMI_Get_3D_Structure(void)
{
    stHDMI_GET_3D_STRUCTURE XCArgs = {0};

    XCArgs.eReturnValue = E_XC_3D_INPUT_MODE_NONE;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

E_HDMI_3D_EXT_DATA_T MDrv_HDMI_Get_3D_Ext_Data_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    E_HDMI_3D_EXT_DATA_T e3DExtendData = E_3D_EXT_DATA_MODE_MAX;
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance, E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        e3DExtendData = Hal_HDMI_Get_3D_Ext_Data(ucHDMIInfoSource);
    }

    _XC_SEMAPHORE_RETURN(pInstance, E_XC_ID_VAR);

#endif

    return e3DExtendData;
}

E_HDMI_3D_EXT_DATA_T MDrv_HDMI_Get_3D_Ext_Data(void)
{
    stHDMI_GET_3D_EXT_DATA XCArgs = {0};

    XCArgs.eReturnValue = E_3D_EXT_DATA_MODE_MAX;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_CMD_GET_3D_EXT_DATA, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

void MDrv_HDMI_Get_3D_Meta_Field_U2(void* pInstance, MS_U8 ucHDMIInfoSource, sHDMI_3D_META_FIELD *pdata)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        Hal_HDMI_Get_3D_Meta_Field(ucHDMIInfoSource, pdata);
    }
    else
    {
        memset(pdata, 0, sizeof(sHDMI_3D_META_FIELD));
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    // Do nothing
#endif
}

void MDrv_HDMI_Get_3D_Meta_Field(sHDMI_3D_META_FIELD *pdata)
{
    stHDMI_GET_3D_META_FIELD XCArgs = {0};

    XCArgs.pdata = pdata;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_3D_META_FIELD, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
}

MS_U8 MDrv_HDMI_Get_VIC_Code_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    MS_U8 ucVICCode = 0;
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_AVI_PACKET_RECEIVE_FLAG) // AVI packet should be received first
    {
        ucVICCode = Hal_HDMI_Get_VIC_Code(ucHDMIInfoSource);
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#endif

    return ucVICCode;
}

MS_U8 MDrv_HDMI_Get_VIC_Code(void)
{
    stHDMI_GET_VIC_CODE XCArgs = {0};

    XCArgs.u8ReturnValue = 0;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_VIC_CODE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.u8ReturnValue;
}

E_HDMI_VIC_4Kx2K_CODE MDrv_HDMI_Get_4Kx2K_VIC_Code_U2(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    E_HDMI_VIC_4Kx2K_CODE e4K2KVICCode = E_VIC_NA;
#if SUPPORT_HDMI_RX_NEW_FEATURE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG) // VSP packet should be received first
    {
        e4K2KVICCode = Hal_HDMI_Get_4Kx2K_VIC_Code(ucHDMIInfoSource);
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

#endif

    return e4K2KVICCode;
}

E_HDMI_VIC_4Kx2K_CODE MDrv_HDMI_Get_4Kx2K_VIC_Code(void)
{
    stHDMI_GET_4KX2K_VIC_CODE XCArgs;

    XCArgs.eReturnValue = E_VIC_NA;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.eReturnValue;
}

// HDMI ARC pin control
//     - enInputPortType: INPUT_PORT_DVI0 / INPUT_PORT_DVI1 / INPUT_PORT_DVI2 / INPUT_PORT_DVI3
//     - bEnable: ARC enable or disable
//     - bDrivingHigh: ARC driving current high or low, suggest driving current should be set to high when ARC is enable
void MDrv_HDMI_ARC_PINControl_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_HDMI_ARC_PINControl(enInputPortType, bEnable, bDrivingHigh);
#else
    // Do nothing
#endif
}

void MDrv_HDMI_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_ARC_PINCONTROL XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;
    XCArgs.bDrivingHigh = bDrivingHigh;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ARC_PINCONTROL, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//[6]: soft-reset hdmi
//[5]: soft-reset hdcp
//[4]: soft-reset dvi
void MDrv_DVI_Software_Reset_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset)
{
#if SUPPORT_HDMI_RX_NEW_FEATURE
    Hal_DVI_Software_Reset(enInputPortType, u16Reset);
#else
    // Do nothing
#endif
}

void MDrv_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stDVI_SOFTWARE_RESET XCArgs;
    XCArgs.enInputPortType= enInputPortType;
    XCArgs.u16Reset = u16Reset;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_DVI_SOFTWARE_RESET, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

MS_U32 MDrv_HDMI_SetPowerState_U2(void* pInstance, EN_POWER_MODE enPowerState)
{
    MS_U16 u32Return = UTOPIA_STATUS_FAIL;
    //MS_CEC_INIT_INFO stCECInfo;
    XC_DDCRAM_PROG_INFO stDDCRamInfo;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (enPowerState == E_POWER_SUSPEND)
    {
        pXCResourcePrivate->stdrv_HDMI._prev_enPowerState = enPowerState;
        u32Return = UTOPIA_STATUS_SUCCESS;//SUSPEND_OK;
    }
    else if (enPowerState == E_POWER_RESUME)
    {

        if (pXCResourcePrivate->stdrv_HDMI._prev_enPowerState == E_POWER_SUSPEND)
        {
            // EDID and DDC Ram
            {

            // init Digital EDID 0
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8EDID_DVIBackUp[E_XC_PROG_DVI0_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI0_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x10; //0x5555;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0xA1;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0xA0;

            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));


            // init Digital EDID 1
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8EDID_DVIBackUp[E_XC_PROG_DVI1_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI1_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x20; //0x3131;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0xA1;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0xA0;

            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));

            // init Digital EDID 2
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8EDID_DVIBackUp[E_XC_PROG_DVI2_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI2_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x30; //9292;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0xA1;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0xA0;

            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));

            // init Digital EDID 3
            stDDCRamInfo.EDID = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8EDID_DVIBackUp[E_XC_PROG_DVI3_EDID];
            stDDCRamInfo.u16EDIDSize = EDID_KEY_LENGTH;
            stDDCRamInfo.eDDCProgType = E_XC_PROG_DVI3_EDID;
            stDDCRamInfo.u16CECPhyAddr = 0x40; //0x9292;
            stDDCRamInfo.u8CECPhyAddrIdxL = 0x9D;    // the CEC physical address start from 0xA1 byte in EDID
            stDDCRamInfo.u8CECPhyAddrIdxH = 0x9C;


            MDrv_HDMI_PROG_DDCRAM_internal(pInstance, &stDDCRamInfo, sizeof(stDDCRamInfo));
            }

            E_MUX_INPUTPORT eHDMIPort;
            //clear all HPD
            for (eHDMIPort = INPUT_PORT_DVI0;eHDMIPort <= INPUT_PORT_DVI3;eHDMIPort++)
            {
                MDrv_HDMI_pullhpd_internal(pInstance, DISABLE, eHDMIPort, pXCResourcePrivate->stdrvXC_MVideo_Context.g_bHDMIInverseBackUp[eHDMIPort - INPUT_PORT_DVI0]);
                Hal_DVI_ClkPullLow(TRUE, eHDMIPort);
            }

            // HDMI initial
            MDrv_HDMI_init_internal(pInstance);

            // init HDCP key again.
            if(pXCResourcePrivate->stdrv_HDMI.bHDCP14RxREEFlag)
            {
                MDrv_HDCP_initproductionkey_internal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8HdcpKeyDataBackUp);
            }

            // Delay 100ms
            MsOS_DelayTask(100);

            //set all HPD
            for (eHDMIPort = INPUT_PORT_DVI0;eHDMIPort <= INPUT_PORT_DVI3;eHDMIPort++)
            {
                Hal_DVI_ClkPullLow(FALSE, eHDMIPort);
                MDrv_HDMI_pullhpd_internal(pInstance, ENABLE, eHDMIPort, pXCResourcePrivate->stdrvXC_MVideo_Context.g_bHDMIInverseBackUp[eHDMIPort - INPUT_PORT_DVI0]);
            }

#if 0
            // CEC
            stCECInfo.u32XTAL_CLK_Hz = MST_XTAL_CLOCK_HZ;
            MApi_CEC_Init(&stCECInfo);
#endif
            pXCResourcePrivate->stdrv_HDMI._prev_enPowerState = enPowerState;
            u32Return = UTOPIA_STATUS_SUCCESS;//RESUME_OK;
        }
        else
        {
            MDRV_HDMIRX_MSG_INFO("[%s,%5d]It is not suspended yet. We shouldn't resume\n",__func__,__LINE__);
            u32Return = UTOPIA_STATUS_FAIL;//SUSPEND_FAILED;
        }
    }
    else
    {
        MDRV_HDMIRX_MSG_INFO("[%s,%5d]Do Nothing: %d\n",__func__,__LINE__,enPowerState);
        u32Return = UTOPIA_STATUS_NOT_SUPPORTED;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32Return;// for success
}

MS_U32 MDrv_HDMI_SetPowerState(EN_POWER_MODE enPowerState)
{
    stHDMI_SET_POWERSTATE XCArgs;
    stHDMI_INITIAL_SETTING stHDMIRxArgs;

    XCArgs.enPowerState = enPowerState;
    XCArgs.u32ReturnValue = UTOPIA_STATUS_FAIL;
    stHDMIRxArgs.stInitialTable.bImmeswitchSupport = FALSE;
    stHDMIRxArgs.stInitialTable.ucMHLSupportPath = 0;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_POWERSTATE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }
    else
    {
        if(pulHDMIRxInst == NULL)
        {
            MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
        }
        else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_INITIAL, (void*)&stHDMIRxArgs) != 0)
        {
            MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
        }
    }

    return XCArgs.u32ReturnValue;
}

MS_BOOL MDrv_HDMI_CheckHDMI20_Setting_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
#if SUPPORT_HDMI20
    return Hal_HDMI_CheckHDMI20_Setting(enInputPortType);
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_HDMI_CheckHDMI20_Setting(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_CHECK_HDMI20_SETTING XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CHECK_HDMI20_SETTING, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDMI_StablePolling(void)
{
    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_STABLE_POLLING, NULL) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

MS_U8 MDrv_HDMI_GetSourceVersion(E_MUX_INPUTPORT enInputPortType)
{
    stHDMI_GET_SOURCE_VERSION XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucSourceVersion = HDMI_SOURCE_VERSION_NOT_SURE;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_GET_SOURCE_VERSION, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return XCArgs.ucSourceVersion;
}

void MDrv_HDMI_Set5VDetectGPIOSelect_U2(void* pInstance, MS_U32 ul5VDetectGPIOIndex)
{
    Hal_HDMI_Set5VDetectGPIOSelect(ul5VDetectGPIOIndex);
}

void SYMBOL_WEAK MDrv_HDMI_Set5VDetectGPIOSelect(MS_U32 ul5VDetectGPIOIndex)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stHDMI_SET_5V_DETECT_GPIO_SELECT XCArgs;
    XCArgs.ul5VDetectGPIOSelect = ul5VDetectGPIOIndex;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_5V_DETECT_GPIO_SELECT, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_GetDEStableStatus_U2(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    return Hal_DVI_GetDEStableStatus(enInputPortType);
}

MS_BOOL SYMBOL_WEAK MDrv_HDMI_GetDEStableStatus(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stHDMI_GET_DE_STABLE_STATUS XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bStatus= FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_DE_STABLE_STATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bStatus;
    }
}

MS_BOOL SYMBOL_WEAK MDrv_HDMI_CheckHDCP14(void)
{
    stHDMI_GET_HDCP14_KEY_VAILD XCArgs = {0};

    XCArgs.bHDCP14KeyVaildFlag = FALSE;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_GET_HDCP14_KEY_VAILD, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return XCArgs.bHDCP14KeyVaildFlag;
}

E_HDMI_HDCP_STATE MDrv_HDMI_CheckHDCPState(E_MUX_INPUTPORT enInputPortType)
{
    stHDMI_CHECK_HDCP_STATE XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucHDCPState = E_HDCP_NO_ENCRYPTION;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_CHECK_HDCP_STATE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return (E_HDMI_HDCP_STATE)XCArgs.ucHDCPState;
}

E_HDMI_HDCP_ENCRYPTION_STATE MDrv_HDMI_CheckHDCPENCState(E_MUX_INPUTPORT enInputPortType)
{
    stHDMI_CHECK_HDCP_ENC_STATE XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucHDCPENCState = E_HDCP_NOT_ENCRYPTION;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_CHECK_HDCP_ENC_STATE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return (E_HDMI_HDCP_ENCRYPTION_STATE)XCArgs.ucHDCPENCState;
}

MS_BOOL MDrv_HDMI_SwitchVSDBtoHDRPacket(MS_BOOL bEnableHDR)
{
    Hal_HDMI_SwitchVSDBtoHDRPacket(bEnableHDR);
    return TRUE;
}

MS_BOOL MDrv_HDMI_ParseHDRInfoFrame(void* pInstance, MS_U8 ucHDMIInfoSource, void *pPacket, sHDR_METADATA *pHdrMetadata)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_BOOL bRet = FALSE;

    if(pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_HDR_PACKET_RECEIVE_FLAG)
    {
        if (pHdrMetadata->u16Version == HDMI_HDR_METADATA_VERSION)
        {
            // HDR metadata structure version is the same.
            MS_U16 *pInfoFrame = (MS_U16*)pPacket;
            pHdrMetadata->u8EOTF = ((pInfoFrame[0]>>8)&0x07); // 8~15bits, 0:SDR gamma,  1:HDR gamma,  2:SMPTE ST2084,  3:Future EOTF,  4-7:Reserved
            pHdrMetadata->u8Static_Metadata_ID = (pInfoFrame[1]&0x0007);    // 0:Static Metadata Type 1,  1-7:Reserved for future use
            pHdrMetadata->u16Display_Primaries_X[0] = (pInfoFrame[2]<<8) + (pInfoFrame[1]>>8);

            pHdrMetadata->u16Display_Primaries_Y[0] = (pInfoFrame[3]<<8) + (pInfoFrame[2]>>8);
            pHdrMetadata->u16Display_Primaries_X[1] = (pInfoFrame[4]<<8) + (pInfoFrame[3]>>8);
            pHdrMetadata->u16Display_Primaries_Y[1] = (pInfoFrame[5]<<8) + (pInfoFrame[4]>>8);
            pHdrMetadata->u16Display_Primaries_X[2] = (pInfoFrame[6]<<8) + (pInfoFrame[5]>>8);
            pHdrMetadata->u16Display_Primaries_Y[2] = (pInfoFrame[7]<<8) + (pInfoFrame[6]>>8);
            pHdrMetadata->u16White_Point_X = (pInfoFrame[8]<<8) + (pInfoFrame[7]>>8);
            pHdrMetadata->u16White_Point_Y = (pInfoFrame[9]<<8) + (pInfoFrame[8]>>8);
            pHdrMetadata->u16Max_Display_Mastering_Luminance = (pInfoFrame[10]<<8) + (pInfoFrame[9]>>8);
            pHdrMetadata->u16Min_Display_Mastering_Luminance = (pInfoFrame[11]<<8) + (pInfoFrame[10]>>8);
            pHdrMetadata->u16Maximum_Content_Light_Level = (pInfoFrame[12]<<8) + (pInfoFrame[11]>>8);
            pHdrMetadata->u16Maximum_Frame_Average_Light_Level = (pInfoFrame[13]<<8) + (pInfoFrame[12]>>8);
        }
        else
        {
            // Error handle.
            if (pHdrMetadata->u16Size <= HDMI_HDR_METADATA_VERSION_1_SIZE)
            {
                MS_U16 *pInfoFrame = (MS_U16*)pPacket;
                pHdrMetadata->u8EOTF = ((pInfoFrame[0]>>8)&0x07); // 8~15bits, 0:SDR gamma,  1:HDR gamma,  2:SMPTE ST2084,  3:Future EOTF,  4-7:Reserved
                pHdrMetadata->u8Static_Metadata_ID = (pInfoFrame[1]&0x0007);    // 0:Static Metadata Type 1,  1-7:Reserved for future use
                pHdrMetadata->u16Display_Primaries_X[0] = (pInfoFrame[2]<<8) + (pInfoFrame[1]>>8);

                pHdrMetadata->u16Display_Primaries_Y[0] = (pInfoFrame[3]<<8) + (pInfoFrame[2]>>8);
                pHdrMetadata->u16Display_Primaries_X[1] = (pInfoFrame[4]<<8) + (pInfoFrame[3]>>8);
                pHdrMetadata->u16Display_Primaries_Y[1] = (pInfoFrame[5]<<8) + (pInfoFrame[4]>>8);
                pHdrMetadata->u16Display_Primaries_X[2] = (pInfoFrame[6]<<8) + (pInfoFrame[5]>>8);
                pHdrMetadata->u16Display_Primaries_Y[2] = (pInfoFrame[7]<<8) + (pInfoFrame[6]>>8);
                pHdrMetadata->u16White_Point_X = (pInfoFrame[8]<<8) + (pInfoFrame[7]>>8);
                pHdrMetadata->u16White_Point_Y = (pInfoFrame[9]<<8) + (pInfoFrame[8]>>8);
                pHdrMetadata->u16Max_Display_Mastering_Luminance = (pInfoFrame[10]<<8) + (pInfoFrame[9]>>8);
                pHdrMetadata->u16Min_Display_Mastering_Luminance = (pInfoFrame[11]<<8) + (pInfoFrame[10]>>8);
                pHdrMetadata->u16Maximum_Content_Light_Level = (pInfoFrame[12]<<8) + (pInfoFrame[11]>>8);
                pHdrMetadata->u16Maximum_Frame_Average_Light_Level = (pInfoFrame[13]<<8) + (pInfoFrame[12]>>8);
            }
        }

        //MDRV_HDMIRX_MSG_DEBUG("HDMI HDR metadata is :\n");
        //MDRV_HDMIRX_MSG_DEBUG("u8EOTF is 0x%x.\033[m\n", pHdrMetadata->u8EOTF);
        //MDRV_HDMIRX_MSG_DEBUG("u8Static_Metadata_ID is 0x%x.\n", pHdrMetadata->u8Static_Metadata_ID);
        //MDRV_HDMIRX_MSG_DEBUG("u16Display_Primaries_X[0] is 0x%x.\n", pHdrMetadata->u16Display_Primaries_X[0]);
        //MDRV_HDMIRX_MSG_DEBUG("u16Display_Primaries_Y[0] is 0x%x.\n", pHdrMetadata->u16Display_Primaries_Y[0]);
        //MDRV_HDMIRX_MSG_DEBUG("u16Display_Primaries_X[1] is 0x%x.\n", pHdrMetadata->u16Display_Primaries_X[1]);
        //MDRV_HDMIRX_MSG_DEBUG("u16Display_Primaries_Y[1] is 0x%x.\n", pHdrMetadata->u16Display_Primaries_Y[1]);
        //MDRV_HDMIRX_MSG_DEBUG("u16Display_Primaries_X[2] is 0x%x.\n", pHdrMetadata->u16Display_Primaries_X[2]);
        //MDRV_HDMIRX_MSG_DEBUG("u16Display_Primaries_Y[2] is 0x%x.\n", pHdrMetadata->u16Display_Primaries_Y[2]);
        //MDRV_HDMIRX_MSG_DEBUG("u16White_Point_X is 0x%x.\n", pHdrMetadata->u16White_Point_X);
        //MDRV_HDMIRX_MSG_DEBUG("u16White_Point_Y is 0x%x.\n", pHdrMetadata->u16White_Point_Y);
        //MDRV_HDMIRX_MSG_DEBUG("u16Max_Display_Mastering_Luminance is 0x%x.\n", pHdrMetadata->u16Max_Display_Mastering_Luminance);
        //MDRV_HDMIRX_MSG_DEBUG("u16Min_Display_Mastering_Luminance is 0x%x.\n", pHdrMetadata->u16Min_Display_Mastering_Luminance);
        //MDRV_HDMIRX_MSG_DEBUG("u16Maximum_Content_Light_Level is 0x%x.\n", pHdrMetadata->u16Maximum_Content_Light_Level);
        //MDRV_HDMIRX_MSG_DEBUG("u16Maximum_Frame_Average_Light_Level is 0x%x.\n", pHdrMetadata->u16Maximum_Frame_Average_Light_Level);

        bRet = TRUE;
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return bRet;
}

MS_BOOL MDrv_HDMI_Extend_Packet_Received(void* pInstance, MS_U8 ucHDMIInfoSource, MS_BOOL bEnable, MS_HDMI_EXTEND_PACKET_RECEIVE_t *pExtendPacketReceive)
{
    MS_BOOL bReturn = FALSE;
    MS_U16 u16CopiedLength = sizeof(MS_HDMI_EXTEND_PACKET_RECEIVE_t);
    MS_HDMI_EXTEND_PACKET_RECEIVE_t stExtendPacketReceive = {0};
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((NULL == pExtendPacketReceive) || (pExtendPacketReceive->u16Size == 0))
    {
        printf("MDrv_HDMI_Extend_Packet_Received: Null parameter or Wrong u16Size\n");
        return bReturn;
    }

    if(pExtendPacketReceive->u16Version < 1)
    {
        printf("MDrv_HDMI_Extend_Packet_Received: please check your u16Version, it should not set to 0\n");
        return bReturn;
    }

    if (pExtendPacketReceive->u16Version > HDMI_EXTEND_PACKET_RECEIVE_VERSION)
    {
#if defined (__aarch64__)
        printf("MDrv_HDMI_Extend_Packet_Received: this old version xc lib has only length:%lu driver status!!\n",sizeof(MS_HDMI_EXTEND_PACKET_RECEIVE_t));
#else
        printf("MDrv_HDMI_Extend_Packet_Received: this old version xc lib has only length:%u driver status!!\n",sizeof(MS_HDMI_EXTEND_PACKET_RECEIVE_t));
#endif
        u16CopiedLength = sizeof(MS_HDMI_EXTEND_PACKET_RECEIVE_t);
    }

    if ((pExtendPacketReceive->u16Version < HDMI_EXTEND_PACKET_RECEIVE_VERSION) || (pExtendPacketReceive->u16Size < sizeof(MS_HDMI_EXTEND_PACKET_RECEIVE_t)))
    {
        printf("MDrv_HDMI_Extend_Packet_Received: new version xc lib shouldn't access to the space which doesn't exist in old structure!!\n");
        u16CopiedLength = pExtendPacketReceive->u16Size;
    }

    stExtendPacketReceive.bPKT_MPEG_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_MPEG_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_AUI_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_AUDIO_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_SPD_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_SPD_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_AVI_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_AVI_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_GC_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_GCP_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_ASAMPLE_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_AUDIO_SAMPLE_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_ACR_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_ACR_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_VS_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VS_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_NULL_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_NULL_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_ISRC2_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_ISRC2_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_ISRC1_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_ISRC1_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_ACP_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_ACP_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_ONEBIT_AUD_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_DSD_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_GM_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_GM_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_HBR_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_HBR_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_VBI_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_VBI_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_HDR_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_HDR_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_RSV_RECEIVE = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_RESERVED_PACKET_RECEIVE_FLAG)? TRUE: FALSE;
    stExtendPacketReceive.bPKT_EDR_VALID = (pXCResourcePrivate->stdrv_HDMI.ulPacketStatus[ucHDMIInfoSource] &HDMI_STATUS_EDR_VALID_FLAG)? TRUE: FALSE;

    memcpy(pExtendPacketReceive, &stExtendPacketReceive, u16CopiedLength);

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return TRUE;
}

MS_BOOL MDrv_HDMI_Ctrl_U2(void* pInstance, MS_U8 ucHDMIInfoSource, MS_U32 u32Cmd, void *pBuf, MS_U32 u32BufSize)
{
    MS_BOOL bRet = FALSE;

    switch(u32Cmd)
    {
        case E_HDMI_SWITCH_VSDB_TO_HDR_PACKET:
            if (u32BufSize == sizeof(MS_HDMI_Switch_VSDB_to_HDR_Parameters))
            {
                MS_HDMI_Switch_VSDB_to_HDR_Parameters *pParameters = (MS_HDMI_Switch_VSDB_to_HDR_Parameters*)pBuf;
                bRet = MDrv_HDMI_SwitchVSDBtoHDRPacket(pParameters->bEnableHDR);
            }
            break;

        case E_HDMI_HDR_INFOFRAME_GET:
            if (u32BufSize == sizeof(MS_HDMI_HDR_InfoFrame_Get_Parameters))
            {
                MS_HDMI_HDR_InfoFrame_Get_Parameters *pParameters = (MS_HDMI_HDR_InfoFrame_Get_Parameters*)pBuf;
                MS_U16 HdrPacket[HDMI_HDR_PACKET_SIZE];
                Hal_HDMI_Get_InfoFrame(ucHDMIInfoSource, PKT_HDR, HdrPacket);
                bRet = MDrv_HDMI_ParseHDRInfoFrame(pInstance, ucHDMIInfoSource, HdrPacket, pParameters->pstHDRMetadata);
            }
            break;

        case E_HDMI_EXTEND_PACKET_RECEIVE_GET:
            if (u32BufSize == sizeof(MS_HDMI_Extend_Packet_Receive_Get_Parameters))
            {
                MS_HDMI_Extend_Packet_Receive_Get_Parameters *pParameters = (MS_HDMI_Extend_Packet_Receive_Get_Parameters*)pBuf;
                bRet = MDrv_HDMI_Extend_Packet_Received(pInstance, ucHDMIInfoSource, pParameters->bEnable, pParameters->pstPacketReceive);
            }
            break;

        default:
            break;
    }

    return bRet;
}

MS_BOOL MDrv_HDMI_Ctrl(MS_U32 u32Cmd, void *pBuf, MS_U32 u32BufSize)
{
    MS_BOOL bRetValue = FALSE;

    if(u32Cmd < 0x500) // XC resource
    {
        stHDMI_CONTROL XCArgs;

        XCArgs.u32Cmd = u32Cmd;
        XCArgs.pBuf = pBuf;
        XCArgs.u32BufSize = u32BufSize;
        XCArgs.bRet = FALSE;
        XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

        if(pu32XCInst == NULL)
        {
            MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        }
        else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CTRL, (void*)&XCArgs) != 0)
        {
            MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
        }

        bRetValue = XCArgs.bRet;
    }
    else // HDMIRX resource
    {
        bRetValue = MDrv_HDMI_IOCTL(u32Cmd, pBuf, u32BufSize);
    }

    return bRetValue;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDMI_GetDataInfo_U2
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 MDrv_HDMI_GetDataInfo_U2(void* pInstance, E_HDMI_GET_DATA_INFO enInfo, MS_U8 ucHDMIInfoSource)
{
    MS_U16 usVideoInfo = 0;

    usVideoInfo = Hal_HDMI_GetDataInfo(enInfo, ucHDMIInfoSource);

    return usVideoInfo;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDMI_GetDataInfo
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 MDrv_HDMI_GetDataInfo(E_HDMI_GET_DATA_INFO enInfo)
{
    stHDMI_GET_DATA_INFO XCArgs = {0};

    XCArgs.usReturnValue = 0;
    XCArgs.enInfo = enInfo;
    XCArgs.ucHDMIInfoSource = ucHDMIInfoSourceSelect;

    if(pu32XCInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_DATA_INFO, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain XC engine fail\n");
    }

    return XCArgs.usReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_WriteX74
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_HDCP_WriteX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset, MS_U8 ucData)
{
    stHDCP_WRITE_X74 XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucOffset = ucOffset;
    XCArgs.ucData = ucData;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_WRITE_X74, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_ReadX74
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_HDCP_ReadX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset)
{
    stHDCP_READ_X74 XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucOffset = ucOffset;
    XCArgs.ucRetData = 0;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_READ_X74, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return XCArgs.ucRetData;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_SetRepeater
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_HDCP_SetRepeater(E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsRepeater)
{
    stHDCP_SET_REPEATER XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bIsRepeater = bIsRepeater;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_SET_REPEATER, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_SetBstatus
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_HDCP_SetBstatus(E_MUX_INPUTPORT enInputPortType, MS_U16 usBstatus)
{
    stHDCP_SET_BSTATUS XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.usBstatus = usBstatus;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_SET_BSTATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_SetHDMIMode
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_HDCP_SetHDMIMode(E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMIMode)
{
    stHDCP_SET_HDMI_MODE XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bHDMIMode = bHDMIMode;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_SET_HDMI_MODE, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_GetInterruptStatus
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 MDrv_HDCP_GetInterruptStatus(E_MUX_INPUTPORT enInputPortType)
{
    stHDCP_GET_INTERRUPT_STATUS XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.ucRetIntStatus = 0;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_GET_INTERRUPT_STATUS, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }

    return XCArgs.ucRetIntStatus;
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_WriteKSVList
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_HDCP_WriteKSVList(E_MUX_INPUTPORT enInputPortType, MS_U8* pucKSV, MS_U32 ulDataLen)
{
    stHDCP_WRITE_KSV_LIST XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.pucKSV = pucKSV;
    XCArgs.ulLen = ulDataLen;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_WRITE_KSV_LIST, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDCP_SetVPrime
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void MDrv_HDCP_SetVPrime(E_MUX_INPUTPORT enInputPortType, MS_U8* pucVPrime)
{
    stHDCP_SET_VPRIME XCArgs = {0};

    XCArgs.enInputPortType = enInputPortType;
    XCArgs.pucVPrime = pucVPrime;

    if(pulHDMIRxInst == NULL)
    {
        MDRV_HDMIRX_MSG_WARNING("[%s,%5d]No instance existed, please get an instance by calling MDrv_HDMI_init() first\n",__FUNCTION__,__LINE__);
    }
    else if(UtopiaIoctl(pulHDMIRxInst, MAPI_CMD_HDMIRX_SET_VPRIME, (void*)&XCArgs) != 0)
    {
        MDRV_HDMIRX_MSG_ERROR("Obtain HDMI Rx engine fail\n");
    }
}

//**************************************************************************
//  [Function Name]:
//                  MDrv_HDMI_IOCTL
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL MDrv_HDMI_IOCTL(MS_U32 ulCommand, void *pBuffer, MS_U32 ulBufferSize)
{
    MS_BOOL bret = FALSE;

    switch(ulCommand)
    {
        case E_HDMI_INTERFACE_CMD_GET_HDCP_STATE:
            {
                if(ulBufferSize == sizeof(stCMD_HDMI_CHECK_HDCP_STATE))
                {
                    ((stCMD_HDMI_CHECK_HDCP_STATE *)pBuffer)->ucHDCPState = (MS_U8)MDrv_HDMI_CheckHDCPState(((stCMD_HDMI_CHECK_HDCP_STATE *)pBuffer)->enInputPortType);

                    bret = TRUE;
                }
                else
                {
                    ((stCMD_HDMI_CHECK_HDCP_STATE *)pBuffer)->ucHDCPState  = HDMI_HDCP_NO_ENCRYPTION;
                }
            }
            break;

        /*************************** HDCP Repeater ***************************/
        case E_HDMI_INTERFACE_CMD_WRITE_X74:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_WRITE_X74))
                {
                    MDrv_HDCP_WriteX74(((stCMD_HDCP_WRITE_X74 *)pBuffer)->enInputPortType, ((stCMD_HDCP_WRITE_X74 *)pBuffer)->ucOffset, ((stCMD_HDCP_WRITE_X74 *)pBuffer)->ucData);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_READ_X74:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_READ_X74))
                {
                    ((stCMD_HDCP_READ_X74 *)pBuffer)->ucRetData = MDrv_HDCP_ReadX74(((stCMD_HDCP_READ_X74 *)pBuffer)->enInputPortType, ((stCMD_HDCP_READ_X74 *)pBuffer)->ucOffset);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_SET_REPEATER:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_SET_REPEATER))
                {
                    MDrv_HDCP_SetRepeater(((stCMD_HDCP_SET_REPEATER *)pBuffer)->enInputPortType, ((stCMD_HDCP_SET_REPEATER *)pBuffer)->bIsRepeater);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_SET_BSTATUS:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_SET_BSTATUS))
                {
                    MDrv_HDCP_SetBstatus(((stCMD_HDCP_SET_BSTATUS *)pBuffer)->enInputPortType, ((stCMD_HDCP_SET_BSTATUS *)pBuffer)->usBstatus);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_SET_HDMI_MODE:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_SET_HDMI_MODE))
                {
                    MDrv_HDCP_SetHDMIMode(((stCMD_HDCP_SET_HDMI_MODE *)pBuffer)->enInputPortType, ((stCMD_HDCP_SET_HDMI_MODE *)pBuffer)->bHDMIMode);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_GET_INTERRUPT_STATUS:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_GET_INTERRUPT_STATUS))
                {
                    ((stCMD_HDCP_GET_INTERRUPT_STATUS *)pBuffer)->ucRetIntStatus = MDrv_HDCP_GetInterruptStatus(((stCMD_HDCP_GET_INTERRUPT_STATUS *)pBuffer)->enInputPortType);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_WRITE_KSV_LIST:
            {
                MS_U8 ucKSVBuffer[HDMI_HDCP_KSV_LIST_LENGTH] = {0};
                MS_U8 *pucKSVBuffer = NULL;
                MS_U32 ultemp = 0;
                MS_U32 ulLength = 0;

                if(ulBufferSize == sizeof(stCMD_HDCP_WRITE_KSV_LIST))
                {
                    ulLength = ((stCMD_HDCP_WRITE_KSV_LIST *)pBuffer)->ulLen;
                    pucKSVBuffer = ((stCMD_HDCP_WRITE_KSV_LIST *)pBuffer)->pucKSV;

                    for(ultemp = 0; ultemp < ulLength; ultemp++)
                    {
                        ucKSVBuffer[ultemp] = pucKSVBuffer[ultemp];
                    }

                    MDrv_HDCP_WriteKSVList(((stCMD_HDCP_WRITE_KSV_LIST *)pBuffer)->enInputPortType, ucKSVBuffer, ulLength);

                    bret = TRUE;
                }
            }
            break;

        case E_HDMI_INTERFACE_CMD_SET_VPRIME:
            {
                if(ulBufferSize == sizeof(stCMD_HDCP_SET_VPRIME))
                {
                    MDrv_HDCP_SetVPrime(((stCMD_HDCP_SET_VPRIME *)pBuffer)->enInputPortType, ((stCMD_HDCP_SET_VPRIME *)pBuffer)->pucVPrime);

                    bret = TRUE;
                }
            }
            break;
        /*************************** HDCP Repeater ***************************/
        case E_HDMI_INTERFACE_CMD_GET_HDCP_ENC_STATE:
            {
                if(ulBufferSize == sizeof(stCMD_HDMI_CHECK_HDCP_ENC_STATE))
                {
                    ((stCMD_HDMI_CHECK_HDCP_ENC_STATE *)pBuffer)->ucHDCPENCState = (MS_U8)MDrv_HDMI_CheckHDCPENCState(((stCMD_HDMI_CHECK_HDCP_ENC_STATE *)pBuffer)->enInputPortType);

                    bret = TRUE;
                }
                else
                {
                    ((stCMD_HDMI_CHECK_HDCP_ENC_STATE *)pBuffer)->ucHDCPENCState  = HDMI_HDCP_NOT_ENCRYPTION;
                }
            }
            break;

        default:
            pBuffer = pBuffer;
            ulBufferSize = ulBufferSize;
            bret = FALSE;
            break;
    };

    return bret;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_STREventProc
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_HDMI_STREventProc(void* pModule, EN_POWER_MODE usPowerState)
{
    void* pHDMIRxResource = NULL;
    MS_U32 ulReturnValue = UTOPIA_STATUS_FAIL;
    E_MUX_INPUTPORT enInputPortType = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(pHDMIRxResourcePrivate->usPrePowerState != usPowerState)
        {
            if(usPowerState == E_POWER_SUSPEND)
            {
                ulReturnValue = UTOPIA_STATUS_SUCCESS;

                MsOS_SetEvent(pHDMIRxResourcePrivate->slHDMIHDCPEventID, HDMI_HDCP_EVENT_STR_SUSPEND);
            }
            else if(usPowerState == E_POWER_RESUME)
            {
                // Clear all HPD
                for(enInputPortType = INPUT_PORT_DVI0; enInputPortType <= INPUT_PORT_DVI_END; enInputPortType++)
                {
                    Hal_HDMI_pullhpd(FALSE, enInputPortType, pHDMIRxResourcePrivate->bHPDInvertFlag[HDMI_GET_PORT_SELECT(enInputPortType)], pHDMIRxResourcePrivate->ucMHLSupportPath);
                    Hal_DVI_ClkPullLow(TRUE, enInputPortType);
                }

                // HDMI initial
                Hal_HDMI_init(pHDMIRxResourcePrivate->bImmeswitchSupport, pHDMIRxResourcePrivate->ucMHLSupportPath);

                // Load HDCP key
                if(pHDMIRxResourcePrivate->bHDCP14RxREEFlag)
                {
                    Hal_HDCP_initproductionkey(pHDMIRxResourcePrivate->ucHDCPKeyTable);
                }

                // Delay 100ms
                MsOS_DelayTask(100);

                // Set all HPD
                for(enInputPortType = INPUT_PORT_DVI0; enInputPortType <= INPUT_PORT_DVI_END; enInputPortType++)
                {
                    Hal_DVI_ClkPullLow(FALSE, enInputPortType);
                    Hal_HDMI_pullhpd(TRUE, enInputPortType, pHDMIRxResourcePrivate->bHPDInvertFlag[HDMI_GET_PORT_SELECT(enInputPortType)], pHDMIRxResourcePrivate->ucMHLSupportPath);

                    pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bNoInputFlag = TRUE;
                    pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bPowerOnLane = FALSE;
                    pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag = FALSE;
                    pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag = FALSE;
                    pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bPowerSavingFlag = FALSE;
                }

                ulReturnValue = UTOPIA_STATUS_SUCCESS;
#if defined(MSOS_TYPE_LINUX_KERNEL)
                UtopiaStrSendCondition("hdmi_rx", usPowerState, 0);
#endif
            }

            pHDMIRxResourcePrivate->usPrePowerState = usPowerState;
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ulReturnValue;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_InitialSetting
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDMI_InitialSetting(void* pInstance, stHDMI_INITIAL_TABLE stInitialTable)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    E_MUX_INPUTPORT enInputPortType = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(pHDMIRxResourcePrivate->ucInitialIndex != 0xA5)
        {
            for(enInputPortType = INPUT_PORT_DVI0; enInputPortType <= INPUT_PORT_DVI_END; enInputPortType++)
            {
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMIModeFlag = FALSE;
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = FALSE;
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQRetrigger = FALSE;
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion = HDMI_SOURCE_VERSION_NOT_SURE;
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_NO_ENCRYPTION;
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bIsRepeater = FALSE;
                pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPInt = 0;
            }

            pHDMIRxResourcePrivate->bSelfCreateTaskFlag = FALSE;
            pHDMIRxResourcePrivate->bHDMITaskProcFlag = FALSE;
            pHDMIRxResourcePrivate->bHDCPIRQAttachFlag = FALSE;
            pHDMIRxResourcePrivate->slHDMIPollingTaskID = -1;
            pHDMIRxResourcePrivate->slHDMIHDCPEventID = MsOS_CreateEventGroup("HDMI_HDCP_Event");
            pHDMIRxResourcePrivate->bImmeswitchSupport = stInitialTable.bImmeswitchSupport;
            pHDMIRxResourcePrivate->ucMHLSupportPath = stInitialTable.ucMHLSupportPath;
            pHDMIRxResourcePrivate->ucHDCPReadDoneIndex = 0;
            pHDMIRxResourcePrivate->ucHDCPWriteDoneIndex = 0;
            pHDMIRxResourcePrivate->bHDCP14RxREEFlag = FALSE;

            pHDMIRxResourcePrivate->ucInitialIndex = 0xA5;
        }

        for(enInputPortType = INPUT_PORT_DVI0; enInputPortType <= INPUT_PORT_DVI_END; enInputPortType++)
        {
            pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bNoInputFlag = TRUE;
            pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bPowerOnLane = FALSE;
            pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag = FALSE;
            pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag = FALSE;
            pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bPowerSavingFlag = FALSE;
        }

        if(!pHDMIRxResourcePrivate->bSelfCreateTaskFlag)
        {
            if(_mdrv_HDMI_CreateTask(pHDMIRxResourcePrivate))
            {
                pHDMIRxResourcePrivate->bSelfCreateTaskFlag = TRUE;
            }
        }

        if(!pHDMIRxResourcePrivate->bHDCPIRQAttachFlag)
        {
            MsOS_AttachInterrupt(E_INT_IRQ_DVI_HDMI_HDCP, _mdrv_HDCP_IsrHandler);
            MsOS_EnableInterrupt(E_INT_IRQ_DVI_HDMI_HDCP);

            pHDMIRxResourcePrivate->bHDCPIRQAttachFlag = TRUE;
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_StablePolling
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDMI_StablePolling(void* pInstance)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        Hal_HDMI_StablePolling(pHDMIRxResourcePrivate->ucMHLSupportPath, pHDMIRxResourcePrivate->stHDMIPollingInfo);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_GetSourceVersion
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mdrv_HDMI_GetSourceVersion(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_U8 ucSourceVersion = HDMI_SOURCE_VERSION_NOT_SURE;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion != HDMI_SOURCE_VERSION_NOT_SURE)
        {
            if(pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag)
            {
                ucSourceVersion = HDMI_SOURCE_VERSION_HDMI20;
            }
            else if(!pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMIModeFlag)
            {
                ucSourceVersion = HDMI_SOURCE_VERSION_HDMI14;
            }
            else
            {
                ucSourceVersion = pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion;
            }
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ucSourceVersion;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_CheckHDCPState
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mdrv_HDMI_CheckHDCPState(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_U8 ucHDCPState = HDMI_HDCP_NO_ENCRYPTION;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(!pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bIsRepeater) // not Repeater mode
        {
            if(Hal_HDCP_GetEncryptionFlag(enInputPortType) && (!Hal_DVI_clklose_det(enInputPortType))) // HDCP RX detection ENC_EN status
            {
                if(pHDMIRxResourcePrivate->bHDCP14RiFlag[HDMI_GET_PORT_SELECT(enInputPortType)]) // [13] Ri: HDCP1.4
                {
                    ucHDCPState = HDMI_HDCP_1_4;

                    MDRV_HDMIRX_MSG_ERROR("** ENC_EN, HDMI HDCP state 1.4, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
                }
                else if(pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState == HDMI_HDCP_2_2) // AKE_Init interrupt status: HDCP2.2
                {
                    ucHDCPState = HDMI_HDCP_2_2;

                    MDRV_HDMIRX_MSG_ERROR("** ENC_EN, HDMI HDCP state 2.2, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
                }
            }
        }
        else // is Repeater mode
        {
            ucHDCPState = pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState;
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ucHDCPState;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_CheckHDCPENCState
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mdrv_HDMI_CheckHDCPENCState(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_U8 ucHDCPENCState = HDMI_HDCP_NOT_ENCRYPTION;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(Hal_HDCP_GetEncryptionFlag(enInputPortType) && (!Hal_DVI_clklose_det(enInputPortType))) // HDCP RX detection ENC_EN status
        {
            if(pHDMIRxResourcePrivate->bHDCP14RiFlag[HDMI_GET_PORT_SELECT(enInputPortType)]) // [13] Ri: HDCP1.4
            {
                ucHDCPENCState = HDMI_HDCP_1_4_ENCRYPTION;

                MDRV_HDMIRX_MSG_ERROR("** ENC_EN, HDMI HDCP ENC state 1.4, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
            else if(pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState == HDMI_HDCP_2_2) // AKE_Init interrupt status: HDCP2.2
            {
                ucHDCPENCState = HDMI_HDCP_2_2_ENCRYPTION;

                MDRV_HDMIRX_MSG_ERROR("** ENC_EN, HDMI HDCP ENC state 2.2, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ucHDCPENCState;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_CheckHDCP14KeyVaild
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDMI_CheckHDCP14KeyVaild(void* pInstance, MS_U8 *ucHDCPKeyData)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_U16 ustemp = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        for(ustemp = 0; ustemp < HDMI_HDCP_KEY_LENGTH; ustemp++)
        {
            pHDMIRxResourcePrivate->ucHDCPKeyTable[ustemp] = ucHDCPKeyData[ustemp];
        }

        if(_mdrv_HDMI_CheckHDCP14Bksv(pHDMIRxResourcePrivate->ucHDCPKeyTable))
        {
            pHDMIRxResourcePrivate->bHDCP14KeyVaildFlag = TRUE;
        }
        else
        {
            MDRV_HDMIRX_MSG_INFO("** TMDS HDCP 1.4 key Bksv data fail~~\r\n");
        }

        if(!pHDMIRxResourcePrivate->bHDCP14RxREEFlag)
        {
            pHDMIRxResourcePrivate->bHDCP14RxREEFlag = TRUE;
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_GetHDCP14KeyVaildFlag
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_HDMI_GetHDCP14KeyVaildFlag(void* pInstance)
{
    MS_BOOL bHDCP14KeyVaild = FALSE;
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        bHDCP14KeyVaild = pHDMIRxResourcePrivate->bHDCP14KeyVaildFlag;
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return bHDCP14KeyVaild;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_SetHPDInvertFlag
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDMI_SetHPDInvertFlag(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHPDInvertFlag)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            pHDMIRxResourcePrivate->bHPDInvertFlag[HDMI_GET_PORT_SELECT(enInputPortType)] = bHPDInvertFlag;
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDMI_GetPacketReceiveStatus
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 mdrv_HDMI_GetPacketReceiveStatus(void* pInstance, MS_U8 ucHDMIInfoSource)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_U32 ulPacketStatus = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            ulPacketStatus = pHDMIRxResourcePrivate->ulPacketStatus[ucHDMIInfoSource];
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ulPacketStatus;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
//-------------------------------------------------------------------------------------------------
///mdrv_HDMI_MDCMDEchoCommand
/// @param  u64ReqHdl               \b IN: Module index
/// @param  pcCmdLine               \b IN: Command line info
//-------------------------------------------------------------------------------------------------
void mdrv_HDMI_MDCMDEchoCommand(void* pInstance, MS_U64* u64ReqHdl, char* pcCmdLine)
{
    char pch[] = "=,";
    char* psep;

    MdbPrint(u64ReqHdl, "LINE:%d, MDBCMD_CMDLINE\n", __LINE__);
    MdbPrint(u64ReqHdl, "pcCmdLine: %s\n", pcCmdLine);

    psep = strsep(&pcCmdLine, pch);

    if(strncmp("HDMIRxOn", psep, 8) == 0)
    {

    }
}

//-------------------------------------------------------------------------------------------------
///mdrv_HDMI_MDCMDGetInfo
/// @param  u64ReqHdl               \b IN: Module index
//-------------------------------------------------------------------------------------------------
void mdrv_HDMI_MDCMDGetInfo(void* pInstance, MS_U64* u64ReqHdl)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        MdbPrint(u64ReqHdl, "LINE:%d, MDBCMD_GETINFO\n", __LINE__);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

#endif

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_WriteX74
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  ucOffset                      \b IN: x74 offset
/// @param   ucData                       \b IN: x74 data
//-------------------------------------------------------------------------------------------------
void mdrv_HDCP_WriteX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset, MS_U8 ucData)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        Hal_HDCP_WriteX74(enInputPortType, ucOffset, ucData);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_ReadX74
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  ucOffset                      \b IN: x74 offset
/// return x74 data
//-------------------------------------------------------------------------------------------------
MS_U8 mdrv_HDCP_ReadX74(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8 ucOffset)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;
    MS_U8 ucRetData = 0;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        ucRetData = Hal_HDCP_ReadX74(enInputPortType, ucOffset);
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ucRetData;
}

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_SetRepeater
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  bIsRepeater                 \b IN: Set REAPEATER or not
//-------------------------------------------------------------------------------------------------
void mdrv_HDCP_SetRepeater(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsRepeater)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;
    MS_U8 ucBcaps = 0x00;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bIsRepeater = bIsRepeater;

        ucBcaps = Hal_HDCP_ReadX74(enInputPortType, 0x40);//Read Bcaps

        if(bIsRepeater)
        {
            ucBcaps = ucBcaps | 0x40;
        }
        else
        {
            ucBcaps = ucBcaps & 0xBF;
        }

        Hal_HDCP_WriteX74(enInputPortType, 0x40, ucBcaps);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//-------------------------------------------------------------------------------------------------
///mdrv_HDCP_SetBStatus
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  usBStatus                    \b IN: Set Bstatus
//-------------------------------------------------------------------------------------------------
void mdrv_HDCP_SetBstatus(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U16 usBstatus)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;
    MS_U8 ucBStatus = 0x00;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        ucBStatus = (MS_U8)(usBstatus & BMASK(7:0)); // [6:0]: Device cnt
        Hal_HDCP_WriteX74(enInputPortType, 0x41, ucBStatus);

        ucBStatus = (MS_U8)((usBstatus & BMASK(15:8)) >> 8);//[10:8]: Depth and [12]: HDMI mode
        Hal_HDCP_WriteX74(enInputPortType, 0x42, ucBStatus);

        MDRV_HDMIRX_MSG_INFO("SN Set Bstatus=%x\r\n", ucBStatus);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_SetHDMIMode
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  bHDMIMode                  \b IN: Set HDMI mode or not
//-------------------------------------------------------------------------------------------------
void mdrv_HDCP_SetHDMIMode(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMIMode)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;
    MS_U8 ucBstatusHighByte = 0;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        ucBstatusHighByte = Hal_HDCP_ReadX74(enInputPortType, 0x42);
        ucBstatusHighByte = bHDMIMode? (ucBstatusHighByte|0x10): (ucBstatusHighByte&0xEF);

        Hal_HDCP_WriteX74(enInputPortType, 0x42, ucBstatusHighByte);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_GetInterruptStatus
/// @param  enInputPortType           \b IN: HDMI Port number
/// @return  interrupt status
//-------------------------------------------------------------------------------------------------
MS_U8 mdrv_HDCP_GetInterruptStatus(void* pInstance, E_MUX_INPUTPORT enInputPortType)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;
    MS_U8 ucRetIntStatus = 0;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        ucRetIntStatus = (Hal_HDCP_getstatus(enInputPortType) & BMASK(15:8)) >> 8;

        pHDMIRxResourcePrivate->stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPInt = ucRetIntStatus;

        if(ucRetIntStatus & BIT(0)) // [8]: Aksv
        {
            MDRV_HDMIRX_MSG_INFO("[%s@%d]Clear Ready", __FUNCTION__, __LINE__);
            Hal_HDCP_SetReady(enInputPortType, FALSE); // Clear Ready
        }

        Hal_HDCP_ClearStatus(enInputPortType, (ucRetIntStatus & BMASK(4:0))); // Clear [0] Aksv, [1] An, [2] Km, [3] Bksv, [4] R0 interrupt status
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ucRetIntStatus;
}

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_WriteKSVList
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  bHDMIMode                  \b IN: Set HDMI mode or not
//-------------------------------------------------------------------------------------------------
void mdrv_HDCP_WriteKSVList(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucKSV, MS_U32 ulDataLen)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        Hal_HDCP_WriteKSVList(enInputPortType, pucKSV, ulDataLen);
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//-------------------------------------------------------------------------------------------------
/// mdrv_HDCP_SetVPrime
/// @param  enInputPortType           \b IN: HDMI Port number
/// @param  ubVPrime                     \b IN: Set V prime
//-------------------------------------------------------------------------------------------------
void mdrv_HDCP_SetVPrime(void* pInstance, E_MUX_INPUTPORT enInputPortType, MS_U8* pucVPrime)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;
    MS_U8 ucCnt = 0;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        for(ucCnt = 0; ucCnt < HDMI_HDCP_VPRIME_LENGTH; ucCnt++)
        {
            Hal_HDCP_WriteX74(enInputPortType, 0x20 + ucCnt, *(pucVPrime + ucCnt));
        }

        MDRV_HDMIRX_MSG_INFO("[%s@%d]Set Ready", __FUNCTION__, __LINE__);
        Hal_HDCP_SetReady(enInputPortType, TRUE); // Set Ready
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_FillCipherKey
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDCP22_FillCipherKey(void* pInstance, MS_U8 ucPortIdx, MS_U8* pucRiv, MS_U8* pucSessionKey)
{
    Hal_HDCP22_FillCipherKey(ucPortIdx, pucRiv, pucSessionKey);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_PortInit
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDCP22_PortInit(void* pInstance, MS_U8 ucPortIdx)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        Hal_HDCP22_PortInit(ucPortIdx);

        Hal_HDCP_WriteDoneInterruptEnable(INPUT_PORT_DVI0 +ucPortIdx, TRUE);

        pHDMIRxResourcePrivate->ucHDCP22RecvIDListSend[ucPortIdx] = FALSE;
        pHDMIRxResourcePrivate->bHDCP22IRQMaskFlag[ucPortIdx] = TRUE;
    }

    UtopiaResourceRelease(pHDMIRxResource);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_PollingReadDone
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_HDCP22_PollingReadDone(void* pInstance, MS_U8 ucPortIdx)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_BOOL bReadDoneFlag = FALSE;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(Hal_HDCP22_PollingReadDone(ucPortIdx))
        {
            bReadDoneFlag = TRUE;
        }
        else if(GET_HDMI_FLAG(pHDMIRxResourcePrivate->ucHDCPReadDoneIndex, BIT(ucPortIdx)))
        {
            CLR_HDMI_FLAG(pHDMIRxResourcePrivate->ucHDCPReadDoneIndex, BIT(ucPortIdx));

            bReadDoneFlag = TRUE;
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return bReadDoneFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_EnableCipher
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDCP22_EnableCipher(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable)
{
    Hal_HDCP22_EnableCipher(ucPortType, ucPortIdx, bIsEnable);
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_SendMessage
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mdrv_HDCP22_SendMessage(void* pInstance, MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_U8 ubRecIDListSetDone = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    Hal_HDCP22_SendMsg(ucPortType, ucPortIdx, pucData, dwDataLen, pDummy, &ubRecIDListSetDone);

    if (*(MS_U8*)(pucData) == 0xC)
    {
        UtopiaInstanceGetModule(pInstance, &pModule);

        if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
        }
        else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
        {
            MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
        }
        else
        {
            if (*(MS_U8*)(pucData) == 0xC)
            {
                pHDMIRxResourcePrivate->ucHDCP22RecvIDListSend[ucPortIdx] = ubRecIDListSetDone;
            }
        }

        UtopiaResourceRelease(pHDMIRxResource);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_Handler
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mdrv_HDCP22_Handler(void* pInstance, MS_U8 ucPortIdx, MS_U8* ucPortType, MS_U8* pucMsgData, MS_U32* wMsgLen, MS_BOOL bIRQModeFlag)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_BOOL bCallbackFlag = FALSE;
    MS_U8 ucMsgData[HDMI_HDCP22_MESSAGE_LENGTH +1] = {0};
    MS_U16 ustemp = 0;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        if(bIRQModeFlag)
        {
            if(Hal_HDCP22_RecvMsg(ucPortIdx, ucMsgData))
            {
                *ucPortType = E_HDCP22_IF_HDMI;

                for(ustemp = 0; ustemp < HDMI_HDCP22_MESSAGE_LENGTH; ustemp++)
                {
                    pucMsgData[ustemp] = ucMsgData[ustemp];
                }

                *wMsgLen = ucMsgData[HDMI_HDCP22_MESSAGE_LENGTH];

                bCallbackFlag = TRUE;
            }
        }
        else
        {
            if(pHDMIRxResourcePrivate->bHDCP22IRQMaskFlag[ucPortIdx])
            {
                Hal_HDCP_WriteDoneInterruptEnable(INPUT_PORT_DVI0 +ucPortIdx, FALSE);

                pHDMIRxResourcePrivate->bHDCP22IRQMaskFlag[ucPortIdx] = FALSE;
            }

            if(Hal_HDCP22_PollingWriteDone(ucPortIdx)) //polling msg
            {
                if(Hal_HDCP22_RecvMsg(ucPortIdx, ucMsgData))
                {
                    *ucPortType = E_HDCP22_IF_HDMI;

                    for(ustemp = 0; ustemp < HDMI_HDCP22_MESSAGE_LENGTH; ustemp++)
                    {
                        pucMsgData[ustemp] = ucMsgData[ustemp];
                    }

                    *wMsgLen = ucMsgData[HDMI_HDCP22_MESSAGE_LENGTH];

                    bCallbackFlag = TRUE;
                }
            }
            else
            {
                *wMsgLen = 0;
            }

            if(ucMsgData[0] == 0x00)
            {
                if(pHDMIRxResourcePrivate->ucHDCP22RecvIDListSend[ucPortIdx])
                {
                    if(Hal_HDCP22_PollingReadDone(ucPortIdx))
                    {
                        pHDMIRxResourcePrivate->ucHDCP22RecvIDListSend[ucPortIdx] = FALSE;

                        Hal_HDCP22_SetReadyBit(ucPortIdx, FALSE); //clear ready bit after source read re
                    }
                }
            }
        }
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return bCallbackFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_HDCP22_WaitEvent
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mdrv_HDCP22_WaitEvent(void* pInstance)
{
    void* pModule = NULL;
    void* pHDMIRxResource = NULL;
    MS_BOOL bGetEventIDFlag = FALSE;
    MS_U8 ucHDCPWriteDoneIndex = 0;
    MS_U32 ulEvent = 0;
    MS_S32 slHDMIHDCPEventID = -1;
    HDMI_RX_RESOURCE_PRIVATE *pHDMIRxResourcePrivate = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        slHDMIHDCPEventID = pHDMIRxResourcePrivate->slHDMIHDCPEventID;
        bGetEventIDFlag = TRUE;
    }

    UtopiaResourceRelease(pHDMIRxResource);

    if(bGetEventIDFlag)
    {
        MsOS_WaitEvent_Interrupt(slHDMIHDCPEventID, HDMI_HDCP_ALL_EVENT, &ulEvent, E_OR_CLEAR, MSOS_WAIT_FOREVER);
    }

    //MDRV_HDMIRX_MSG_INFO("** TMDS wait event happen kernel space !!!!!!\r\n");

    if(UtopiaResourceObtain(pModule, HDMI_RX_POOL, &pHDMIRxResource) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] UtopiaResourceObtainToInstant failed\n", __FUNCTION__, __LINE__);
    }
    else if(UtopiaResourceGetPrivate(pHDMIRxResource, (void*)&pHDMIRxResourcePrivate) != UTOPIA_STATUS_SUCCESS)
    {
        MDRV_HDMIRX_MSG_ERROR("[%s,%5d] Get HDMI Rx resource failed\n", __FUNCTION__, __LINE__);
    }
    else
    {
        ucHDCPWriteDoneIndex = pHDMIRxResourcePrivate->ucHDCPWriteDoneIndex;
        pHDMIRxResourcePrivate->ucHDCPWriteDoneIndex = 0;
    }

    UtopiaResourceRelease(pHDMIRxResource);

    return ucHDCPWriteDoneIndex;
}

