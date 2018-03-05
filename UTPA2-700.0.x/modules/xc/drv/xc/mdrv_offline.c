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

/******************************************************************************/
/*                           Header Files                                     */
/******************************************************************************/

// Common Definition
#include "MsCommon.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"
#include "mhal_offline.h"
#include "xc_hwreg_utility2.h"
#include "drv_sc_mux.h"
#include "mhal_mux.h"
#include "utopia_dapi.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif


//----------------------------
//
//----------------------------
#define    AIS_Debug(x)    //x


//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
MS_U8 MDrv_XC_GetOffLineDetection(void *pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8CheckSrc = INPUT_PORT_NONE_PORT;

    u8CheckSrc = MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(pInstance, u8InputSourceType);

    AIS_Debug(printf("u8CheckSrc=%u\n", u8CheckSrc);)

    return Hal_XC_GetOffLineDetection(pInstance, u8CheckSrc);
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
void MDrv_XC_SetOffLineDetection (void *pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8CheckSrc = INPUT_PORT_NONE_PORT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_OPTEE_MUX_DATA st_xc_mux_data;
    memset(&st_xc_mux_data,0,sizeof(st_xc_mux_data));

    _XC_ENTRY(pInstance);

    u8CheckSrc = MDrv_XC_MUX_MapInputSourceToVDYMuxPORT(pInstance, u8InputSourceType);
    AIS_Debug(printf("u8InputSourceType = %x, u8CheckSrc = %x\n", u8InputSourceType, (MS_U16)u8CheckSrc);)

    Hal_XC_SetOfflineDetectClk(pInstance);

    if (u8CheckSrc == INPUT_PORT_NONE_PORT)
    {
        Hal_XC_OffLineExit(pInstance);
    }
    _XC_RETURN(pInstance);

    if ((u8CheckSrc >= (MS_U8)INPUT_PORT_YMUX_CVBS0) &&
        (u8CheckSrc <= (MS_U8)INPUT_PORT_YMUX_CVBS7))
    {
        MsOS_DelayTask(10);

        _XC_ENTRY(pInstance);
        AIS_Debug(printf("MDrv_SetOffLineToSog VD Related Signal!\n");)
        Hal_XC_SetOffLineToSog_AV(pInstance, u8CheckSrc - INPUT_PORT_YMUX_CVBS0);
        if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
            (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
        {
            MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
        }

        MsOS_DelayTask(10);
        Hal_XC_OffLineInit(pInstance);

        _XC_RETURN(pInstance);

        MsOS_DelayTask(20);

        _XC_ENTRY(pInstance);
        Hal_XC_WaitForHVCleared_AV(pInstance, u8CheckSrc - INPUT_PORT_YMUX_CVBS0);
        Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_ADC_B);
        if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
            (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
        {
            MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
        }
         _XC_RETURN(pInstance);

        MsOS_DelayTask(50);
    }
    else if((u8CheckSrc >= (MS_U8)INPUT_PORT_ANALOG0)&&
           (u8CheckSrc <= (MS_U8)INPUT_PORT_ANALOG4))
    {
        MsOS_DelayTask(10);

        _XC_ENTRY(pInstance);
        AIS_Debug(printf("MDrv_SetOffLineToSog_YUV Component!\n");)
        Hal_XC_SetOffLineToSog_YUV(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0);
        if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
            (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
        {
            MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
        }

        MsOS_DelayTask(10);
        Hal_XC_OffLineInit(pInstance);
        _XC_RETURN(pInstance);

        MsOS_DelayTask(20);

        _XC_ENTRY(pInstance);
        Hal_XC_WaitForHVCleared(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0);
        Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_ADC_B);
        if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
            (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
        {
            MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
        }
        _XC_RETURN(pInstance);

        MsOS_DelayTask(50);
    }
    else if ((u8CheckSrc >= INPUT_PORT_ANALOG0_SYNC)  &&
             (u8CheckSrc <= (MS_U8)INPUT_PORT_ANALOG4_SYNC) )
    {
        MsOS_DelayTask(10);

        _XC_ENTRY(pInstance);
        AIS_Debug(printf("MDrv_SetOffLineToHV VGA!\n");)
        Hal_XC_SetOffLineToHv(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0_SYNC);
        if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
            (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
        {
            MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
        }
        _XC_RETURN(pInstance);

        MsOS_DelayTask(20);

        _XC_ENTRY(pInstance);
        Hal_XC_WaitForHVCleared(pInstance, u8CheckSrc - INPUT_PORT_ANALOG0_SYNC);
        Hal_XC_TurnOnDetectClkPath(pInstance, SC_SUBWIN_IPMUX_ADC_B);
        if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
            (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
        {
            MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
        }
        _XC_RETURN(pInstance);

        MsOS_DelayTask(50);
    }
    else if ((u8CheckSrc >= INPUT_PORT_DVI0) && (u8CheckSrc <= INPUT_PORT_DVI3))
    {
        AIS_Debug(printf("MDrv_SetOffLineToHDMI 1~3!\n");)
        Hal_XC_SetOffLineToHDMI(pInstance, u8CheckSrc);
    }
    else if (u8CheckSrc == INPUT_PORT_MVOP )
    {
        AIS_Debug(printf("MDrv_SetOffLineToUSB!\n");)
        Hal_XC_SetOffLineToUSB(pInstance, u8CheckSrc);
    }
}

MS_BOOL MDrv_XC_SetOffLineSogThreshold(void *pInstance, MS_U8 u8Threshold)
{
    return Hal_XC_SetOffLineSogThreshold(pInstance, u8Threshold);
}

MS_BOOL MDrv_XC_SetOffLineSogBW(void *pInstance, MS_U8 u8BW)
{
    return Hal_XC_SetOffLineSogBW(pInstance, u8BW);
}

//-------------------------------------------------------------------------------------------------
/// Get Offline detection status to specific input source
/// @param  u8InputSourceType                \b IN: the specific input source type to do offline detection
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_GetOffLineDetection_U2( void* pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MS_U8 u8Ret = 0;
    _XC_ENTRY(pInstance);
    u8Ret = MDrv_XC_GetOffLineDetection(pInstance, u8InputSourceType);
    _XC_RETURN(pInstance);
    return u8Ret;
}

MS_U8 MApi_XC_GetOffLineDetection( INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_OFFLINE_DETECTION XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OFFLINE_DETECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Offline detection to specific input source
/// @param  u8InputSourceType                \b IN: the specific input source type to do offline detection
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetOffLineDetection_U2 ( void* pInstance, INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    MDrv_XC_SetOffLineDetection(pInstance, u8InputSourceType);
}

void MApi_XC_SetOffLineDetection ( INPUT_SOURCE_TYPE_t u8InputSourceType )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OFFLINE_DETECTION XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_DETECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Offline detection Sog Threshold
/// @param  u8Threshold                \b IN: threshold value t2,u3,u4: 0~31 other chip: 0~255
/// @Return TRUE: Successful FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetOffLineSogThreshold_U2(void* pInstance, MS_U8 u8Threshold)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_SetOffLineSogThreshold(pInstance, u8Threshold);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_SetOffLineSogThreshold(MS_U8 u8Threshold)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_OFFLINE_SOG_THRESHOLD XCArgs;
    XCArgs.u8Threshold = u8Threshold;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_SOG_THRESHOLD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set Offline Detection Sog filter bandwidth
/// @param  u8BW                \b IN: bandwidth value t2,u3,u4: 0~7 other chip: 0~31
/// @Return TRUE: Successful FALSE: Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_SetOffLineSogBW_U2(void* pInstance, MS_U8 u8BW)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_SetOffLineSogBW(pInstance, u8BW);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_SetOffLineSogBW(MS_U8 u8BW)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_OFFLINE_SOG_BW XCArgs;
    XCArgs.u8BW = u8BW;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_SOG_BW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_OffLineInit_U2(void* pInstance)
{
    _XC_ENTRY(pInstance);
    Hal_XC_OffLineInit(pInstance);
    MDrv_XC_SetOffLineSogThreshold(pInstance, 0x40);
    MDrv_XC_SetOffLineSogBW(pInstance, 0x1F);
    _XC_RETURN(pInstance);
    return TRUE;
}

MS_BOOL MApi_XC_OffLineInit(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_OFFLINE_INIT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OFFLINE_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

