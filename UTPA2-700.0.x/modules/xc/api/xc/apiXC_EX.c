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
/// file  apiXC_Adc_EX.c
/// brief  Scaler API layer Interface
/// author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_ADC_EX_C_
#define _API_XC_ADC_EX_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition

#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#else
#include <linux/string.h>
#endif

#include "MsCommon.h"
#include "utopia.h"
#include "utopia_dapi.h"
#include "mhal_xc_chip_config.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"

#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "drv_sc_ip.h"
#include "mhal_sc.h"
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
#include "apiXC_v2.h"

#include "xc_hwreg_utility2.h"


// Common
#include "apiXC_EX.h"
#include "apiXC_Adc_v2.h"
#include "apiXC_Adc_EX.h"
#include "apiXC_Auto_v2.h"
#include "apiXC_Auto_EX.h"
#include "apiXC_PCMonitor_v2.h"
#include "apiXC_PCMonitor_EX.h"
#include "apiXC_ModeParse_v2.h"
#include "apiXC_ModeParse_EX.h"
#include "drvXC_HDMI_if_v2.h"
#include "drvXC_HDMI_if_EX.h"

// Driver
//#include "drvXC_ADC_Internal_ex.h"

// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//#define TRACE_ADC_INPUT(x) //x

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define _XC_SELECT_INSTANCE(u32Id) ((u32Id == 0)? pu32XCInst : pu32XCInst_1)
#define _XC_ST_CHECK_SIZE(st, st_ex) if(sizeof(st) != sizeof(st_ex)) \
                                    { \
                                        printf("\033[1;35[%s][%d] XC_EX check size fail.\033[0m\n",__FUNCTION__,__LINE__); \
                                    }

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    MApi_XC_ADC_EX_SetCVBSOut(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_DEST_TYPE enOutputType , XC_EX_INPUT_SOURCE_TYPE enInputPortType, MS_BOOL isDigitalSource)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETCVBSOUT XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.enOutputType = enOutputType;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.isDigitalSource = isDigitalSource;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETCVBSOUT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
MS_BOOL MApi_XC_ADC_EX_IsCVBSOutEnabled(XC_DEVICE_ID *pDeviceId, XC_EX_DEST_TYPE enOutputType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_ADC_ISCVBSOUTENABLED XCArgs;
    XCArgs.enOutputType = enOutputType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_ISCVBSOUTENABLED, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_ADC_EX_SetPcClock(XC_DEVICE_ID *pDeviceId, MS_U16 u16Clock)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETPCCLOCK XCArgs;
    XCArgs.u16Value = u16Clock;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETPCCLOCK, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_ADC_EX_SetPhase(XC_DEVICE_ID *pDeviceId, MS_U8 u8Value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETPHASE XCArgs;
    XCArgs.u8Value = u8Value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETPHASE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_ADC_EX_SetPhaseEx(XC_DEVICE_ID *pDeviceId, MS_U16 u16Value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETPHASEEX XCArgs;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETPHASEEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16     MApi_XC_ADC_EX_GetPhaseRange( XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETPHASERANGE XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETPHASERANGE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U8    MApi_XC_ADC_EX_GetPhase( XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETPHASE XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETPHASE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }

}
MS_U16   MApi_XC_ADC_EX_GetPhaseEx( XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETPHASEEX XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETPHASEEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_BOOL MApi_XC_ADC_EX_IsScartRGB(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_ADC_ISSCARTRGB XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_ISSCARTRGB, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
MS_U16  MApi_XC_ADC_EX_GetPcClock(XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETPCCLOCK XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETPCCLOCK, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void    MApi_XC_ADC_EX_GetSoGLevelRange(XC_DEVICE_ID *pDeviceId, MS_U32 *u32min, MS_U32 *u32max, MS_U32 *u32Recommend_value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_GETSOGLEVELRANGE XCArgs;
    XCArgs.u32min = u32min;
    XCArgs.u32max = u32max;
    XCArgs.u32Recommend_value = u32Recommend_value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETSOGLEVELRANGE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_ADC_EX_SetSoGLevel(XC_DEVICE_ID *pDeviceId, MS_U32 u32Value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETSOGLEVEL XCArgs;
    XCArgs.u32Value = u32Value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETSOGLEVEL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

// power
void    MApi_XC_ADC_EX_PowerOff(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_POWEROFF, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

// ADC calibration
void    MApi_XC_ADC_EX_GetDefaultGainOffset(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE eSource , XC_ADC_EX_AdcGainOffsetSetting* pstADCSetting)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(APIXC_AdcGainOffsetSetting,XC_ADC_EX_AdcGainOffsetSetting);
    stXC_ADC_GETDEFAULTGAINOFFSET XCArgs;
    XCArgs.enSource = eSource;
    XCArgs.pstADCSetting = (APIXC_AdcGainOffsetSetting*)pstADCSetting;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETDEFAULTGAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16  MApi_XC_ADC_EX_GetMaximalOffsetValue(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETMAXIMALOFFSETVALUE XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETMAXIMALOFFSETVALUE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U16  MApi_XC_ADC_EX_GetMaximalGainValue(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETMAXIMALGAINVALUE XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETMAXIMALGAINVALUE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U16 MApi_XC_ADC_EX_GetCenterGain(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETCENTERGAIN XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETCENTERGAIN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U16 MApi_XC_ADC_EX_GetCenterOffset(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_ADC_GETCENTEROFFSET XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_GETCENTEROFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

// Set Gain & Offset
void     MApi_XC_ADC_EX_SetGain(XC_DEVICE_ID *pDeviceId, MS_U8 u8color, MS_U16 u16value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETGAIN XCArgs;
    XCArgs.u8Color = u8color;
    XCArgs.u16Value = u16value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETGAIN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void     MApi_XC_ADC_EX_SetOffset(XC_DEVICE_ID *pDeviceId, MS_U8 u8color, MS_U16 u16value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETOFFSET XCArgs;
    XCArgs.u8Color = u8color;
    XCArgs.u16Value = u16value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_ADC_EX_AdjustGainOffset(XC_DEVICE_ID *pDeviceId, XC_ADC_EX_AdcGainOffsetSetting* pstADCSetting)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(APIXC_AdcGainOffsetSetting,XC_ADC_EX_AdcGainOffsetSetting);
    stXC_ADC_ADJUSTGAINOFFSET XCArgs;
    XCArgs.pstADCSetting = (APIXC_AdcGainOffsetSetting*)pstADCSetting;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_ADJUSTGAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_ADC_EX_Source_Calibrate(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE enInputSourceType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SOURCE_CALIBRATE XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SOURCE_CALIBRATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_ADC_EX_SetSoGCal(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETSOGCAL, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//Set SCART RGB PIPE Delay
void MApi_XC_ADC_EX_SetRGB_PIPE_Delay(XC_DEVICE_ID *pDeviceId, MS_U8 u8Value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETRGB_PIPE_DELAY XCArgs;
    XCArgs.u8Value = u8Value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETRGB_PIPE_DELAY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//Set Scart RGB Sync on green clamp delay value
void MApi_XC_ADC_EX_ScartRGB_SOG_ClampDelay(XC_DEVICE_ID *pDeviceId, MS_U16 u16Clpdly, MS_U16 u16Caldur)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SCARTRGB_SOG_CLAMPDELAY XCArgs;
    XCArgs.u16Clpdly = u16Clpdly;
    XCArgs.u16Caldur = u16Caldur;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SCARTRGB_SOG_CLAMPDELAY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_EX_Set_YPbPrLooseLPF(XC_DEVICE_ID *pDeviceId, MS_BOOL benable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SET_YPBPRLOOSELPF XCArgs;
    XCArgs.bEnable = benable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SET_YPBPRLOOSELPF, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_EX_Set_SOGBW(XC_DEVICE_ID *pDeviceId, MS_U16 u16value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SET_SOGBW XCArgs;
    XCArgs.u16Value = u16value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SET_SOGBW, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

// Set Clamp Duration
void MApi_XC_ADC_EX_SetClampDuration(XC_DEVICE_ID *pDeviceId, MS_U16 u16Value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_ADC_SETCLAMPDURATION XCArgs;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_SETCLAMPDURATION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_ADC_EX_EnableHWCalibration(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_ADC_ENABLEHWCALIBRATION XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_ADC_CMD_ENABLEHWCALIBRATION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//XC_Auto
MS_BOOL MApi_XC_Auto_EX_Geometry(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_TuneType enAutoTuneType, XC_Auto_EX_Signal_Info *ActiveInfo, XC_Auto_EX_Signal_Info *StandardInfo, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_Auto_Signal_Info,XC_Auto_EX_Signal_Info);
    stXC_AUTO_GEOMETRY XCArgs;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.ActiveInfo = (XC_Auto_Signal_Info*)ActiveInfo;
    XCArgs.StandardInfo = (XC_Auto_Signal_Info*)StandardInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_GEOMETRY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Auto_EX_Geometry_Ex(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_TuneType enAutoTuneType, XC_Auto_EX_Signal_Info_Ex *ActiveInfo, XC_Auto_EX_Signal_Info_Ex *StandardInfo, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_Auto_Signal_Info_Ex,XC_Auto_EX_Signal_Info_Ex);
    stXC_AUTO_GEOMETRY_EX XCArgs;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.ActiveInfo = (XC_Auto_Signal_Info_Ex*)ActiveInfo;
    XCArgs.StandardInfo = (XC_Auto_Signal_Info_Ex*)StandardInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_GEOMETRY_EX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Auto_EX_StopAutoGeometry(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_AUTO_STOPAUTOGEOMETRY XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_STOPAUTOGEOMETRY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Auto_EX_GainOffset(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_CalibrationType type, XC_Auto_EX_TuneType enAutoTuneType,  XC_ADC_EX_AdcGainOffsetSetting *pstADCSetting, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(APIXC_AdcGainOffsetSetting,XC_ADC_EX_AdcGainOffsetSetting);
    stXC_AUTO_GAINOFFSET XCArgs;
    XCArgs.type = type;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.pstADCSetting = (APIXC_AdcGainOffsetSetting*)pstADCSetting;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_GAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Auto_EX_GetHWFixedGainOffset(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_TuneType enAutoTuneType, XC_ADC_EX_AdcGainOffsetSetting *pstADCSetting)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(APIXC_AdcGainOffsetSetting,XC_ADC_EX_AdcGainOffsetSetting);
    stXC_AUTO_GETHWFIXEDGAINOFFSET XCArgs;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.pstADCSetting = (APIXC_AdcGainOffsetSetting*)pstADCSetting;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_GETHWFIXEDGAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

MS_BOOL MApi_XC_Auto_EX_SetValidData(XC_DEVICE_ID *pDeviceId, MS_U8 Value)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_AUTO_SETVALIDDATA XCArgs;
    XCArgs.Value = Value;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_SETVALIDDATA, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_Auto_EX_AutoOffset(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_AUTO_AUTOOFFSET XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bIsYPbPrFlag = bIsYPbPrFlag;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_AUTOOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_Auto_EX_DetectWidth(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_Signal_Info_Ex *pSignalInfo , XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_Auto_Signal_Info_Ex,XC_Auto_EX_Signal_Info_Ex);
    stXC_AUTO_DETECTWIDTH XCArgs;
    XCArgs.pSignalInfo = (XC_Auto_Signal_Info_Ex*)pSignalInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_DETECTWIDTH, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Auto_EX_SetCalibrationMode(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_CalibrationMode eMode)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_AUTO_SETCALIBRATIONMODE XCArgs;
    XCArgs.eMode = eMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_SETCALIBRATIONMODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_Auto_EX_GetCalibrationMode(XC_DEVICE_ID *pDeviceId, XC_Auto_EX_CalibrationMode *eMode)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_Auto_CalibrationMode,XC_Auto_EX_CalibrationMode);
    stXC_AUTO_GETCALIBRATIONMODE XCArgs;
    XCArgs.eMode = (XC_Auto_CalibrationMode*)eMode;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_AUTO_CMD_GETCALIBRATIONMODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//XC_Monitor
MS_BOOL MApi_XC_PCMonitor_EX_Init(XC_DEVICE_ID *pDeviceId, MS_U8 u8MaxWindowNum)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_PCMONITOR_INIT XCArgs;
    XCArgs.u8MaxWindowNum = u8MaxWindowNum;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_INIT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
void MApi_XC_PCMonitor_EX_Restart(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_PCMONITOR_RESTART XCArgs;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_RESTART, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
void MApi_XC_PCMonitor_EX_SetTimingCount(XC_DEVICE_ID *pDeviceId, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_PCMONITOR_SETTIMINGCOUNT XCArgs;
    XCArgs.u16TimingStableCounter = u16TimingStableCounter;
    XCArgs.u16TimingNosyncCounter = u16TimingNosyncCounter;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_SETTIMINGCOUNT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_PCMONITOR_EX_STATUS MApi_XC_PCMonitor_EX(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }

    stXC_PCMONITOR XCArgs;
    XCArgs.eCurrentSrc = src;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_XC_PCMONITOR_STABLE_NOSYNC;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// get input timing information
XC_PCMONITOR_EX_STATUS MApi_XC_PCMonitor_EX_GetCurrentState(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }

    stXC_PCMONITOR_GETCURRENTSTATE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_XC_PCMONITOR_STABLE_NOSYNC;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GETCURRENTSTATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_PCMONITOR_STABLE_NOSYNC;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_U8 MApi_XC_PCMonitor_EX_GetSyncStatus(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_PCMONITOR_GETSYNCSTATUS XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GETSYNCSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U16 MApi_XC_PCMonitor_EX_Get_HFreqx10(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_PCMONITOR_GET_HFREQX10 XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GET_HFREQX10, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U32 MApi_XC_PCMonitor_EX_Get_HFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_PCMONITOR_GET_HFREQX1K XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GET_HFREQX1K, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_U16 MApi_XC_PCMonitor_EX_Get_VFreqx10(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_PCMONITOR_GET_VFREQX10 XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GET_VFREQX10, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}


MS_U32 MApi_XC_PCMonitor_EX_Get_VFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_PCMONITOR_GET_VFREQX1K XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GET_VFREQX1K, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_U16 MApi_XC_PCMonitor_EX_Get_Vtotal(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_PCMONITOR_GET_VTOTAL XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GET_VTOTAL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void MApi_XC_PCMonitor_EX_Get_Dvi_Hdmi_De_Info(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow,XC_EX_WINDOW_TYPE* msWin)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_PCMONITOR_GET_DVI_HDMI_DE_INFO XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.msWin = (MS_WINDOW_TYPE*)msWin;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_GET_DVI_HDMI_DE_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_PCMonitor_EX_SyncLoss(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_PCMONITOR_SYNCLOSS XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_SYNCLOSS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_PCMonitor_EX_InvalidTimingDetect(XC_DEVICE_ID *pDeviceId, MS_BOOL bPollingOnly, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_PCMONITOR_INVALIDTIMINGDETECT XCArgs;
    XCArgs.bPollingOnly = bPollingOnly;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_INVALIDTIMINGDETECT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_PCMonitor_EX_SetTimingCountEx( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE eCurrentSrc, MS_U16 u16TimingStableCounter, MS_U16 u16TimingNosyncCounter)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_PCMONITOR_SETTIMINGCOUNTEX XCArgs;
    XCArgs.eCurrentSrc = eCurrentSrc;
    XCArgs.u16TimingStableCounter = u16TimingStableCounter;
    XCArgs.u16TimingNosyncCounter = u16TimingNosyncCounter;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_PCMONITOR_CMD_SETTIMINGCOUNTEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}


//XC_ModeParse
MS_BOOL MApi_XC_ModeParse_EX_Init(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_MODEPARSE_INIT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_MODEPARSE_CMD_INIT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_MODEPARSE_EX_RESULT MApi_XC_ModeParse_EX_MatchMode(XC_DEVICE_ID *pDeviceId, XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE* ModeDB, MS_U8 u16NumberOfItems , XC_MODEPARSE_EX_INPUT_INFO *psInputInfo)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return XC_MODEPARSE_NOT_PARSED;
    }

    _XC_ST_CHECK_SIZE(MS_PCADC_MODETABLE_TYPE,XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE);
    _XC_ST_CHECK_SIZE(XC_MODEPARSE_INPUT_INFO,XC_MODEPARSE_EX_INPUT_INFO);
    stXC_MODEPARSE_MATCHMODE XCArgs;
    XCArgs.ModeDB = (MS_PCADC_MODETABLE_TYPE*)ModeDB;
    XCArgs.u8NumberOfItems = u16NumberOfItems;
    XCArgs.psInputInfo = (XC_MODEPARSE_INPUT_INFO*)psInputInfo;
    XCArgs.eReturnValue = XC_MODEPARSE_NOT_PARSED;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_MODEPARSE_CMD_MATCHMODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XC_MODEPARSE_NOT_PARSED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_MODEPARSE_EX_RESULT MApi_XC_ModeParse_EX_MatchModeEx(XC_DEVICE_ID *pDeviceId, XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE_EX* ModeDB, MS_U8 u16NumberOfItems , XC_MODEPARSE_EX_INPUT_INFO *psInputInfo)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return XC_MODEPARSE_NOT_PARSED;
    }

    _XC_ST_CHECK_SIZE(MS_PCADC_MODETABLE_TYPE_EX,XC_MODEPARSE_EX_PCADC_MODETABLE_TYPE_EX);
    _XC_ST_CHECK_SIZE(XC_MODEPARSE_INPUT_INFO,XC_MODEPARSE_EX_INPUT_INFO);
    stXC_MODEPARSE_MATCHMODEEX XCArgs;
    XCArgs.ModeDB = (MS_PCADC_MODETABLE_TYPE_EX*)ModeDB;
    XCArgs.u8NumberOfItems = u16NumberOfItems;
    XCArgs.psInputInfo = (XC_MODEPARSE_INPUT_INFO*)psInputInfo;
    XCArgs.eReturnValue = XC_MODEPARSE_NOT_PARSED;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_MODEPARSE_CMD_MATCHMODEEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return XC_MODEPARSE_NOT_PARSED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }

}

//drvXC_HDMI_if_EX.h
HDMI_EX_RET_VALUE  MDrv_HDMI_EX_GetLibVer(const MSIF_Version **ppVersion)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_HDMI_RET_FAIL;
    }

    stHDMI_GETLIBVER XCArgs;
    XCArgs.ppVersion = ppVersion;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETLIBVER, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
HDMI_EX_RET_VALUE  MDrv_HDMI_EX_GetInfo(HDMI_EX_INFO *pInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_HDMI_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(MS_HDMI_INFO,HDMI_EX_INFO);
    stHDMI_GETINFO XCArgs;
    XCArgs.pInfo = (MS_HDMI_INFO*)pInfo;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETINFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}
HDMI_EX_RET_VALUE  MDrv_HDMI_EX_GetStatus(HDMI_EX_Status *pStatus)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_HDMI_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(MS_HDMI_Status,HDMI_EX_Status);
    stHDMI_GETSTATUS XCArgs;
    XCArgs.pStatus = (MS_HDMI_Status*)pStatus;
    XCArgs.eReturnValue = E_HDMI_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GETSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MDrv_HDMI_EX_Get_AVIInfoActiveInfoPresent(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stHDMI_GET_AVIIINFO_ACTIVEINFOPRESENT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AVIINFO_ACTIVEINFOPRESENT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U16 MDrv_HDMI_EX_Func_Caps(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_FUNC_CAPS XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_FUNC_CAPS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void MDrv_HDMI_EX_init( XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDMI_IF_CMD_INIT, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_EX_Exit( XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDMI_IF_CMD_EXIT, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_EX_pkt_reset( XC_DEVICE_ID *pDeviceId, HDMI_EX_REST breset )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stHDMI_PKT_RESET XCArgs;
    XCArgs.breset = breset;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDMI_IF_CMD_PKT_RESET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_EX_pullhpd( MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_PULLHPD XCArgs;
    XCArgs.bHighLow = bHighLow;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bInverse = bInverse;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PULLHPD, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16 MDrv_HDMI_EX_GC_Info(HDMI_EX_GControl_INFO gcontrol)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_GC_INFO XCArgs;
    XCArgs.gcontrol = gcontrol;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GC_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

HDMI_EX_PACKET_VALUE MDrv_HDMI_EX_Packet_Received(MS_BOOL bEnable)
{
    _XC_ST_CHECK_SIZE(MS_HDMI_PACKET_VALUE_t,HDMI_EX_PACKET_VALUE);
    stHDMI_PACKET_RECEIVED XCArgs;
    HDMI_EX_PACKET_VALUE stPacketValue = {0};
    XCArgs.bEnable = bEnable;

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PACKET_RECEIVED, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
    }
    else
    {

    }

    return stPacketValue;
}

HDMI_EX_COLOR_FORMAT MDrv_HDMI_EX_Get_ColorFormat(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return MS_HDMI_COLOR_UNKNOWN;
    }

    stHDMI_GET_COLORFORMAT XCArgs;
    XCArgs.eReturnValue = MS_HDMI_COLOR_UNKNOWN;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_COLORFORMAT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return MS_HDMI_COLOR_UNKNOWN;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

HDMI_EX_COLOR_RANGE MDrv_HDMI_EX_Get_ColorRange(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_HDMI_COLOR_RANGE_RESERVED;
    }

    stHDMI_GET_COLORRANGE XCArgs;
    XCArgs.eReturnValue = E_HDMI_COLOR_RANGE_RESERVED;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_COLORRANGE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_COLOR_RANGE_RESERVED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

HDMI_EX_CONTENT_TYPE MDrv_HDMI_EX_Get_Content_Type(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return MS_HDMI_CONTENT_NoData;
    }

    stHDMI_GET_CONTENT_TYPE XCArgs;
    XCArgs.enReturnValue = MS_HDMI_CONTENT_NoData;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_CONTENT_TYPE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return MS_HDMI_CONTENT_NoData;
    }
    else
    {
        return XCArgs.enReturnValue;
    }
}

HDMI_EX_EXT_COLORIMETRY_FORMAT MDrv_HDMI_EX_Get_ExtColorimetry(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return MS_HDMI_EXT_COLOR_UNKNOWN;
    }

    stHDMI_GET_EXT_COLORIMETRY XCArgs;
    XCArgs.enReturnValue = MS_HDMI_EXT_COLOR_UNKNOWN;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_EXTCOLORIMETRY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return MS_HDMI_EXT_COLOR_UNKNOWN;
    }
    else
    {
        return XCArgs.enReturnValue;
    }
}

void MDrv_HDMI_EX_Set_EQ(XC_DEVICE_ID *pDeviceId, HDMI_EX_EQ enEq, MS_U8 u8EQValue)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stHDMI_SET_EQ XCArgs;
    XCArgs.enEq = enEq;
    XCArgs.u8EQValue = u8EQValue;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDMI_IF_CMD_SET_EQ, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_EX_Set_EQ_To_Port(XC_DEVICE_ID *pDeviceId, HDMI_EX_EQ enEq, MS_U8 u8EQValue, E_MUX_INPUTPORT enInputPortType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stHDMI_SET_EQ_TO_PORT XCArgs;
    XCArgs.enEq = enEq;
    XCArgs.u8EQValue = u8EQValue;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDMI_IF_CMD_SET_EQ_TO_PORT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDMI_EX_Audio_MUTE_Enable(MS_U16 u16MuteEvent, MS_U16 u16MuteMask)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_AUDIO_MUTE_ENABLE XCArgs;
    XCArgs.u16MuteEvent = u16MuteEvent;
    XCArgs.u16MuteMask = u16MuteMask;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_MUTE_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
void MDrv_HDMI_EX_Audio_Status_Clear(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_STATUS_CLEAR, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

HDMI_EX_AR_TYPE MDrv_HDMI_EX_Get_AspectRatio(MS_BOOL bEn_PAR, MS_BOOL bEn_AFAR)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return HDMI_AF_AR_Reserve_0;
    }

    stHDMI_GET_ASPECTRATIO XCArgs;
    XCArgs.bEn_PAR = bEn_PAR;
    XCArgs.bEn_AFAR = bEn_AFAR;
    XCArgs.eReturnValue = HDMI_AF_AR_Reserve_0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_ASPECTRATIO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return HDMI_AF_AR_Reserve_0;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

HDMI_EX_AR_TYPE MDrv_HDMI_EX_Get_ActiveFormat_AspectRatio(MS_BOOL bEn_AFAR)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return HDMI_AF_AR_Reserve_0;
    }

    stHDMI_GET_ACTIVEFORMAT_ASPECTRATIO XCArgs;
    XCArgs.bEn_AFAR = bEn_AFAR;
    XCArgs.eReturnValue = HDMI_AF_AR_Reserve_0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_ACTIVEFORMAT_ASPECTRATIO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return HDMI_AF_AR_Reserve_0;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

HDMI_EX_AVI_INFOFRAME_VERSION MDrv_HDMI_EX_Get_AVIInfoFrameVer(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_AVI_INFOFRAME_VERSION_NON;
    }

    stHDMI_GET_AVIINFOFRAMEVER XCArgs;
    XCArgs.eReturnValue = E_AVI_INFOFRAME_VERSION_NON;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_AVIINFOFRAMEVER, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_AVI_INFOFRAME_VERSION_NON;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_U8 MDrv_HDMI_EX_err_status_update(MS_U8 u8value, MS_BOOL bread)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_ERR_STATUS_UPDATE XCArgs;
    XCArgs.u8value = u8value;
    XCArgs.bread = bread;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ERR_STATUS_UPDATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

void MDrv_HDMI_EX_Get_PollingStatus(HDMI_POLLING_STATUS_t **pStatus)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_GET_POLLINGSTATUS XCArgs;
    XCArgs.pStatus = NULL;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_POLLINGSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_EX_IsHDMI_Mode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stHDMI_ISHDMI_MODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ISHDMI_MODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDMI_EX_Get_MID_info(MS_U8 * pu8ManufactorData)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_GET_MID_INFO XCArgs;
    XCArgs.pu8ManufactorData = pu8ManufactorData;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_MID_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_EX_get_packet_value(HDMI_EX_PACKET_STATE u8state, MS_U8 u8byte_idx, MS_U8 *pValue)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stHDMI_GET_PACKET_VALUE XCArgs;
    XCArgs.u8state = u8state;
    XCArgs.u8byte_idx = u8byte_idx;
    XCArgs.pValue = pValue;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_PACKET_VALUE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//DVI
MS_U16 MDrv_DVI_EX_ChannelPhaseStatus( HDMI_EX_DVI_CHANNEL_TYPE u8Channel)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stDVI_CHANNELPHASESTATUS XCArgs;
    XCArgs.u8Channel = u8Channel;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_CHANNELPHASESTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void MDrv_DVI_EX_SoftwareReset( MS_U16 u16Reset)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stDVI_SOFTWARERESET XCArgs;
    XCArgs.u16Reset = u16Reset;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_SOFTWARERESET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void mdrv_dvi_ex_reset(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_DVI_IF_CMD_RESET, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_DVI_EX_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stDVI_CLKPULLLOW XCArgs;
    XCArgs.bPullLow = bPullLow;
    XCArgs.enInputPortType = enInputPortType;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_CLKPULLLOW, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_DVI_EX_SwitchSrc(E_MUX_INPUTPORT enInputPortType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stDVI_SWITCHSRC XCArgs;
    XCArgs.enInputPortType = enInputPortType;

    if(UtopiaIoctl(pu32XCInst, E_XC_DVI_IF_CMD_SWITCHSRC, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_EX_SetForClockLessThan70Mhz( E_MUX_INPUTPORT InputSourceType  )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stHDMI_SETFORCLOCKLESSTHAN70MHZ XCArgs;
    XCArgs.enInputPortType = InputSourceType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SETFORCLOCKLESSTHAN70MHZ, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDMI_EX_dvi_adjust( MS_BOOL bClockLessThan70MHz)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_DVI_ADJUST XCArgs;
    XCArgs.bClockLessThan70MHz = bClockLessThan70MHz;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_DVI_ADJUST, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_EX_SetUpdatePhaseLineCount(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE enInputSourceType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stHDMI_SETUPDATEPHASELINECOUNT XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDMI_IF_CMD_SETUPDATEPHASELINECOUNT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//HDCP
void MDrv_HDCP_EX_Enable(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stHDCP_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDCP_IF_CMD_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_EX_SetHdcpEnable(E_MUX_INPUTPORT enInputPortType,MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stHDMI_SET_HDCPENABLE XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_SET_HDCP_ENABLE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_HDCP_EX_ClearStatus( XC_DEVICE_ID *pDeviceId )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDCP_IF_CMD_CLEARSTATUS, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDCP_EX_initproductionkey( MS_U8 * pu8HdcpKeyData )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDCP_INITPRODUCTIONKEY XCArgs;
    XCArgs.pu8HdcpKeyData = pu8HdcpKeyData;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_INITPRODUCTIONKEY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

HDCP_EX_STATUS_INFO* MDrv_HDCP_EX_GetStatus(XC_DEVICE_ID *pDeviceId)
{
    stHDCP_GETSTATUS XCArgs = {0};
    static HDCP_EX_STATUS_INFO stHDCPStatus = {0};

    XCArgs.usHDCPStatus = 0;

    if(_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
    }

    _XC_ST_CHECK_SIZE(MS_HDCP_STATUS_INFO_t, HDCP_EX_STATUS_INFO);

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_HDCP_IF_CMD_GETSTATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");

    }
    else
    {

    }

    return &stHDCPStatus;
}

void MDrv_HDCP_EX_Vsync_end_en(MS_BOOL bStable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDCP_VSYNC_END_EN XCArgs;
    XCArgs.bStable = bStable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDCP_IF_CMD_VSYNC_END_EN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//Audio
void MDrv_HDMI_EX_audio_output( MS_BOOL bEnable )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_AUDIO_OUTPUT XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_OUTPUT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8 MDrv_HDMI_EX_audio_cp_hdr_info(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_AUDIO_CP_HDR_INFO XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_CP_HDR_INFO, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U8 MDrv_HDMI_EX_audio_channel_status(MS_U8 u8byte)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_AUDIO_CHANNEL_STATUS XCArgs;
    XCArgs.u8byte = u8byte;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AUDIO_CHANNEL_STATUS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

// DDC Ram
void MDrv_HDMI_EX_READ_DDCRAM(XC_EX_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_DDCRAM_PROG_INFO,XC_EX_DDCRAM_PROG_INFO);
    stHDMI_READ_DDCRAM XCArgs;
    XCArgs.pstDDCRam_Info = (XC_DDCRAM_PROG_INFO*)pstDDCRam_Info;
    XCArgs.u32SizeOfInfo = u32SizeOfInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_READ_DDCRAM, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_EX_PROG_DDCRAM(XC_EX_DDCRAM_PROG_INFO *pstDDCRam_Info, MS_U32 u32SizeOfInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_DDCRAM_PROG_INFO,XC_EX_DDCRAM_PROG_INFO);
    stHDMI_PROG_DDCRAM XCArgs;
    XCArgs.pstDDCRam_Info = (XC_DDCRAM_PROG_INFO*)pstDDCRam_Info;
    XCArgs.u32SizeOfInfo = u32SizeOfInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_PROG_DDCRAM, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8 MDrv_HDMI_EX_Get_Pixel_Repetition(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_GET_PIXEL_REPETITION XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_PIXEL_REPETITION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//#if defined(CHIP_T8) || defined(CHIP_J2) // HDMI 1.4 new feature
void MDrv_HDMI_EX_3D_4Kx2K_Process(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_3D_4KX2K_PROCESS, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_HDMI_EX_AVG_ScaleringDown(MS_BOOL btrue)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_AVG_SCALERINGDOWN XCArgs;
    XCArgs.bTrue = btrue;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_AVG_SCALERINGDOWN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_HDMI_EX_Check4K2K(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stHDMI_CHECK_4K2K XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CHECK_4K2K, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

HDMI_EX_ADDITIONAL_VIDEO_FORMAT MDrv_HDMI_EX_Check_Additional_Format(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_HDMI_NA;
    }

    stHDMI_CHECK_ADDITIONAL_FORMAT XCArgs;
    XCArgs.eReturnValue = E_HDMI_NA;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_CHECK_ADDITIONAL_FORMAT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_HDMI_NA;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_3D_INPUT_MODE MDrv_HDMI_EX_Get_3D_Structure(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_XC_3D_INPUT_MODE_NONE;
    }

    stHDMI_GET_3D_STRUCTURE XCArgs;
    XCArgs.eReturnValue = E_XC_3D_INPUT_MODE_NONE;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_3D_STRUCTURE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_INPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

HDMI_EX_3D_EXT_DATA MDrv_HDMI_EX_Get_3D_Ext_Data(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_3D_EXT_DATA_MODE_MAX;
    }

    stHDMI_GET_3D_EXT_DATA XCArgs;
    XCArgs.eReturnValue = E_3D_EXT_DATA_MODE_MAX;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_CMD_GET_3D_EXT_DATA, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_3D_EXT_DATA_MODE_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MDrv_HDMI_EX_Get_3D_Meta_Field(HDMI_EX_3D_META_FIELD *pdata)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    _XC_ST_CHECK_SIZE(sHDMI_3D_META_FIELD,HDMI_EX_3D_META_FIELD);
    stHDMI_GET_3D_META_FIELD XCArgs;
    XCArgs.pdata = (sHDMI_3D_META_FIELD*)pdata;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_3D_META_FIELD, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8 MDrv_HDMI_EX_Get_VIC_Code(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stHDMI_GET_VIC_CODE XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_VIC_CODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

HDMI_EX_VIC_4Kx2K_CODE MDrv_HDMI_EX_Get_4Kx2K_VIC_Code(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_VIC_NA;
    }

    stHDMI_GET_4KX2K_VIC_CODE XCArgs;
    XCArgs.eReturnValue = E_VIC_NA;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_GET_4KX2K_VIC_CODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_VIC_NA;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

// HDMI ARC pin control
void MDrv_HDMI_EX_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stHDMI_ARC_PINCONTROL XCArgs;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.bEnable = bEnable;
    XCArgs.bDrivingHigh = bDrivingHigh;

    if(UtopiaIoctl(pu32XCInst, E_XC_HDMI_IF_CMD_ARC_PINCONTROL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//API XC
XC_EX_RETURN_VALUE MApi_XC_EX_GetLibVer(const MSIF_Version **ppVersion)
{
    return MApi_XC_GetLibVer(ppVersion);

    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_LIB_VER XCArgs;
    XCArgs.ppVersion = ppVersion;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_LIB_VERSION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

const XC_EX_ApiInfo * MApi_XC_EX_GetInfo(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return NULL;
    }

    stXC_CMD_GET_INFO XCArgs;
    static XC_ApiInfo Getinfo;

    XCArgs.stReturnValue = &Getinfo;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return (XC_EX_ApiInfo*)XCArgs.stReturnValue;
    }
    else
    {
        return (XC_EX_ApiInfo*)XCArgs.stReturnValue;
    }
}

MS_BOOL MApi_XC_EX_GetStatus(XC_DEVICE_ID *pDeviceId, XC_EX_ApiStatus *pDrvStatus, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_ApiStatus,XC_EX_ApiStatus);
    stXC_GET_STATUS XCArgs;
    XCArgs.pDrvStatus = (XC_ApiStatus*)pDrvStatus;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U16  MApi_XC_EX_GetStatusEx(XC_DEVICE_ID *pDeviceId, XC_EX_ApiStatusEx *pDrvStatusEx, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    _XC_ST_CHECK_SIZE(XC_ApiStatusEx,XC_EX_ApiStatusEx);
    stXC_GET_STATUS_EX XCArgs;
    XCArgs.pDrvStatusEx = (XC_ApiStatusEx*)pDrvStatusEx;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_STATUS_EX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetDbgLevel(MS_U16 u16DbgSwitch)
{
    if (pu32XCInst == NULL)
    {
        XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
        INIT_XC_INSTANT_ATTR(stXCInstantAttribute);
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_SET_DBG_LEVEL XCArgs;
    XCArgs.u16DbgSwitch = u16DbgSwitch;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DBG_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_FPLLDbgMode(XC_DEVICE_ID *pDeviceId, XC_EX_FPLL_DBG_MODE DbgMode, XC_EX_FPLL_DBG_FLAG DbgFlag, MS_U32 u32Data)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FPLL_CUSTOMERMODE XCArgs;
    XCArgs.eMode = DbgMode;
    XCArgs.eFlag = DbgFlag;
    XCArgs.u32Data = u32Data;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FPLL_CUSTOMERMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_FPLLCustomerMode(XC_DEVICE_ID *pDeviceId, XC_EX_FPLL_MODE eMode, XC_EX_FPLL_FLAG eFlag, MS_U32 u32Data)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FPLL_CUSTOMERMODE XCArgs;
    XCArgs.eMode = eMode;
    XCArgs.eFlag = eFlag;
    XCArgs.u32Data = u32Data;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FPLL_CUSTOMERMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_XC_EX_SetIOMapBase(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CMD_SET_IOMAPBASE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IOMAPBASE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Init(XC_DEVICE_ID *pDeviceId, XC_EX_INITDATA *pXC_InitData, MS_U32 u32InitDataLen)
{
    XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
    INIT_XC_INSTANT_ATTR(stXCInstantAttribute);
    if (pDeviceId->u32Id == 0 && pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    else if (pDeviceId->u32Id == 1 && pu32XCInst_1 == NULL)
    {
        stXCInstantAttribute.u32DeviceID= 1;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(XC_INITDATA,XC_EX_INITDATA);
    stXC_INIT XCArgs;
    XCArgs.pXC_InitData = (XC_INITDATA*)pXC_InitData;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_GetConfig(XC_DEVICE_ID *pDeviceId, XC_EX_INITDATA *pXC_EX_InitData)
{
    XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
    INIT_XC_INSTANT_ATTR(stXCInstantAttribute);
    if (pDeviceId->u32Id == 0 && pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    else if (pDeviceId->u32Id == 1 && pu32XCInst_1 == NULL)
    {
        stXCInstantAttribute.u32DeviceID= 1;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(XC_INITDATA, XC_EX_INITDATA);
    stXC_GET_CONFIG XCArgs;
    XCArgs.pXC_InitData = (XC_INITDATA*)pXC_EX_InitData;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_CONFIG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("XC engine GET_CONFIG Ioctl fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Init_MISC(XC_DEVICE_ID *pDeviceId, XC_EX_INITMISC *pXC_Init_Misc, MS_U32 u32InitMiscDataLen)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(XC_INITMISC,XC_EX_INITMISC);
    stXC_INIT_MISC XCArgs;
    XCArgs.pXC_Init_Misc = (XC_INITMISC*)pXC_Init_Misc;
    XCArgs.u32InitMiscDataLen = u32InitMiscDataLen;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_INIT_MISC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_GetMISCStatus(XC_DEVICE_ID *pDeviceId, XC_EX_INITMISC *pXC_Init_Misc)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(XC_INITMISC,XC_EX_INITMISC);
    stXC_GET_MISC_STATUS XCArgs;
    XCArgs.pXC_Init_Misc = (XC_INITMISC*)pXC_Init_Misc;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_MISC_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_U32  MApi_XC_EX_GetCapability(MS_U32 u32Id)
{
    if (pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, NULL) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }

    stXC_GET_CAPABILITY XCArgs;
    XCArgs.u32Id = u32Id;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CAPABILITY, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_GetChipCaps(XC_EX_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_CHIP_CAPS XCArgs;
    XCArgs.eCapType = eCapType;
    XCArgs.pRet = pRet;
    XCArgs.ret_size = ret_size;
    XCArgs.eReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CHIPCAPS, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Exit(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_EXIT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_EXIT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetDynamicScaling(XC_DEVICE_ID *pDeviceId, XC_EX_DynamicScaling_Info *pstDSInfo, MS_U32 u32DSInfoLen, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_DynamicScaling_Info,XC_EX_DynamicScaling_Info);
    stXC_SET_DYNAMIC_SCALING XCArgs;
    XCArgs.pstDSInfo = (XC_DynamicScaling_Info*)pstDSInfo;
    XCArgs.u32DSInfoLen = u32DSInfoLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DYNAMIC_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_GetDynamicScalingStatus(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_DS_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DS_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_PHY MApi_XC_EX_Get_DNRBaseOffset(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_DNR_BASEOFFSET XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DNR_BASEOFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_U8   MApi_XC_EX_Get_FrameNumFactor(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_FRAMENUM_FACTOR XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_FRAMENUM_FACTOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetWindow(XC_DEVICE_ID *pDeviceId, XC_EX_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_SETWIN_INFO,XC_EX_SETWIN_INFO);
    stXC_SET_WINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info = (XC_SETWIN_INFO*)pstXC_SetWin_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetDualWindow(XC_DEVICE_ID *pDeviceId, XC_EX_SETWIN_INFO *pstXC_SetWin_Info_Main, XC_EX_SETWIN_INFO *pstXC_SetWin_Info_Sub)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_SETWIN_INFO,XC_EX_SETWIN_INFO);
    stXC_SET_DUALWINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info_Main = (XC_SETWIN_INFO*)pstXC_SetWin_Info_Main;
    XCArgs.pstXC_SetWin_Info_Sub = (XC_SETWIN_INFO*)pstXC_SetWin_Info_Sub;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DUALWINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetTravelingWindow(XC_DEVICE_ID *pDeviceId, XC_EX_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_SETWIN_INFO,XC_EX_SETWIN_INFO);
    stXC_SET_TRAVELING_WINDOW XCArgs;
    XCArgs.pstXC_SetWin_Info = (XC_SETWIN_INFO*)pstXC_SetWin_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_TRAVELING_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_SetInputSource( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE enInputSourceType, XC_EX_SCALER_WIN eWindow )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_INPUTSOURCE XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_INPUTSOURCE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsYUVSpace(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_YUVSPACE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_YUVSPACE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_IsMemoryFormat422(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_MEMORYFORMAT422 XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_MEMORYFORMAT422, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_EnableForceRGBin(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FORCE_RGBIN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FORCE_RGBIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL    MApi_XC_EX_EnableMirrorModeEx( XC_DEVICE_ID *pDeviceId, XC_EX_MirrorMode eMirrorMode,  XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_MIRRORMODE_EX XCArgs;
    XCArgs.eMirrorMode = eMirrorMode;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MIRRORMODE_EX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_MirrorMode MApi_XC_EX_GetMirrorModeTypeEx(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return MIRROR_MAX;
    }

    stXC_GET_MIRRORMODE_TYPEEX XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = MIRROR_MAX;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_MIRRORMODE_TYPEEX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return MIRROR_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_GetSyncStatus(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE eCurrentSrc , XC_EX_IP_SYNC_STATUS *sXC_Sync_Status, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_IP_SYNC_STATUS,XC_EX_IP_SYNC_STATUS);
    stXC_GET_SYNC_STATUS XCArgs;
    XCArgs.eCurrentSrc = eCurrentSrc;
    XCArgs.sXC_Sync_Status = (XC_IP_SYNC_STATUS*)sXC_Sync_Status;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_SYNC_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8   MApi_XC_EX_WaitOutputVSync(XC_DEVICE_ID *pDeviceId, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_SET_WAIT_OUTPUT_VSYNC XCArgs;
    XCArgs.u8NumVSyncs = u8NumVSyncs;
    XCArgs.u16Timeout = u16Timeout;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0xFF;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_WAIT_OUTPUT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0xFF;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U8   MApi_XC_EX_WaitInputVSync(XC_DEVICE_ID *pDeviceId,MS_U8 u8NumVSyncs, MS_U16 u16Timeout, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_SET_WAIT_INPUT_VSYNC XCArgs;
    XCArgs.u8NumVSyncs = u8NumVSyncs;
    XCArgs.u16Timeout = u16Timeout;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0xFF;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_WAIT_INPUT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0xFF;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

void    MApi_XC_EX_SetHdmiSyncMode(XC_EX_HDMI_SYNC_TYPE esynctype)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stXC_SET_HDMI_SYNCMODE XCArgs;
    XCArgs.eSynctype = esynctype;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_HDMI_SYNCMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_HDMI_SYNC_TYPE MApi_XC_EX_GetHdmiSyncMode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return HDMI_SYNC_DE;
    }

    stXC_GET_HDMI_SYNCMODE XCArgs;
    XCArgs.eReturnValue = HDMI_SYNC_DE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_HDMI_SYNCMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return HDMI_SYNC_DE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_SetRepWindow(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_WINDOW_TYPE Window, MS_U8 u8Color)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_SET_REPORT_WINDOW XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.Window = *((MS_WINDOW_TYPE*)&Window);
    XCArgs.u8Color = u8Color;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_REPORT_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_Set_OPWriteOffEnable(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_ForceSet_OPWriteOffEnable(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable , XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_FORCESET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_FORCESET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetDispWinToReg(XC_DEVICE_ID *pDeviceId, XC_EX_WINDOW_TYPE *pstDspwin, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_SET_DISPLAY_WIN_TO_REG XCArgs;
    XCArgs.pstDspwin = (MS_WINDOW_TYPE*)pstDspwin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DISPLAY_WIN_TO_REG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_GetDispWinFromReg(XC_DEVICE_ID *pDeviceId, XC_EX_WINDOW_TYPE *pstDspwin, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_GET_DISPLAY_WIN_FROM_REG XCArgs;
    XCArgs.pstDspwin = (MS_WINDOW_TYPE*)pstDspwin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DISPLAY_WIN_FROM_REG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_FreezeImg(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FREEZEIMG XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FREEZEIMG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsFreezeImg(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_FREEZEIMG XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_FREEZEIMG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_GenerateBlackVideoForBothWin( XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BOTHWINDOW_BLACKVIDEO XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BOTHWINDOW_BLACKVIDEO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_Set_BLSK(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BLACKSCREEN XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BLACKSCREEN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_GenerateBlackVideo(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BLACKVIDEO XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BLACKVIDEO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsBlackVideoEnable(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_BLACKVIDEO_ENABLE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_BLACKVIDEO_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_EnableFrameBufferLess(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRAMEBUFFERLESS XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsCurrentFrameBufferLessMode(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_FRAMEBUFFERLESS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_EnableRequest_FrameBufferLess(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_REQUEST_FRAMEBUFFERLESS XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_REQUEST_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsCurrentRequest_FrameBufferLessMode(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_REQUEST_FRAMEBUFFERLESS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_REQUEST_FRAMEBUFFERLESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U16  MApi_XC_EX_Get_3D_HW_Version(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_3D_HWVERSION XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_HWVERSION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Get_3D_IsSupportedHW2DTo3D(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_3D_SUPPORT_HW2DTO3D XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_3D_SUPPORT_HW2DTO3D, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_Mode(XC_DEVICE_ID *pDeviceId,
                            XC_EX_3D_INPUT_MODE e3dInputMode,
                            XC_EX_3D_OUTPUT_MODE e3dOutputMode,
                            XC_EX_3D_PANEL_TYPE e3dPanelType,
                            XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_MODE XCArgs;
    XCArgs.e3dInputMode = e3dInputMode;
    XCArgs.e3dOutputMode = e3dOutputMode;
    XCArgs.e3dPanelType = e3dPanelType;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_MainWin_FirstMode(XC_DEVICE_ID *pDeviceId, MS_BOOL bMainFirst)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_MAINWIN_FIRST XCArgs;
    XCArgs.bMainFirst = bMainFirst;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_MAINWIN_FIRST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_LR_Frame_Exchg(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_LR_EXCHANGE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_LR_EXCHANGE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_3D_Is_LR_Frame_Exchged(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_3D_LR_EXCHANGED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_3D_LR_EXCHANGED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_3D_INPUT_MODE MApi_XC_EX_Get_3D_Input_Mode(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_3D_INPUT_MODE_NONE;
    }

    stXC_GET_3D_INPUT_MODE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_XC_3D_INPUT_MODE_NONE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_INPUT_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_INPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_3D_OUTPUT_MODE MApi_XC_EX_Get_3D_Output_Mode(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_3D_OUTPUT_MODE_NONE;
    }

    stXC_GET_3D_OUTPUT_MODE XCArgs;
    XCArgs.eReturnValue = E_XC_3D_OUTPUT_MODE_NONE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_OUTPUT_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_OUTPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

#ifdef UFO_XC_GET_3D_FORMAT
MS_BOOL MApi_XC_EX_Get3DFormat(XC_DEVICE_ID *pDeviceId, XC_EX_3D_ATTRIBUTE_TYPE e3DAttrType, void* para, void* p3DFormat)
{
    if(_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET3DFORMAT XCArgs;
    XCArgs.e3DAttrType = e3DAttrType;
    XCArgs.para = para;
    XCArgs.p3DFormat = p3DFormat;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET3DFORMAT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("\n %s : %d, Obtain XC engine fail \n ",__FUNCTION__,__LINE__);
        return E_XC_EX_3D_OUTPUT_MODE_NONE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}
#endif

XC_EX_3D_PANEL_TYPE MApi_XC_EX_Get_3D_Panel_Type(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_XC_3D_PANEL_MAX;
    }

    stXC_GET_3D_PANELTYPE XCArgs;
    XCArgs.eReturnValue = E_XC_3D_PANEL_MAX;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_PANELTYPE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_XC_3D_PANEL_MAX;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Get_3D_MainWin_First(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_3D_MAINWIN_FIRST XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_MAINWIN_FIRST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_3DMainSub_IPSync(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_3D_MAINSUB_IPSYNC XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_3D_MAINSUB_IPSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_VerVideoOffset(XC_DEVICE_ID *pDeviceId, MS_U16 u163DVerVideoOffset)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_VERTICALVIDEO_OFFSET XCArgs;
    XCArgs.u163DVerVideoOffset = u163DVerVideoOffset;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_VERTICALVIDEO_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U16  MApi_XC_EX_Get_3D_VerVideoOffset(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_3D_VERTICALVIDEO_OFFSET XCArgs;
    XCArgs.u16ReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_VERTICALVIDEO_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Is3DFormatSupported(XC_DEVICE_ID *pDeviceId,
                                    XC_EX_3D_INPUT_MODE e3dInputMode,
                                    XC_EX_3D_OUTPUT_MODE e3dOutputMode)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_3D_FORMAT_SUPPORTED XCArgs;
    XCArgs.e3dInputMode = e3dInputMode;
    XCArgs.e3dOutputMode = e3dOutputMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_3D_FORMAT_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_HShift(XC_DEVICE_ID *pDeviceId, MS_U16 u16HShift)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_HORIZONTAL_SHIFT XCArgs;
    XCArgs.u16HShift = u16HShift;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_HORIZONTAL_SHIFT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Enable_3D_LR_Sbs2Line(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_LR_SBS2LINE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_LR_SBS2LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U16  MApi_XC_EX_Get_3D_HShift(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_3D_HORIZONTAL_SHIFT XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_HORIZONTAL_SHIFT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_HW2DTo3D_Buffer(XC_DEVICE_ID *pDeviceId, MS_PHY u32HW2DTO3D_DD_Buf, MS_PHY u32HW2DTO3D_DR_Buf)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_HW2DTO3D_BUFFER XCArgs;
    XCArgs.u32HW2DTO3D_DD_Buf = u32HW2DTO3D_DD_Buf;
    XCArgs.u32HW2DTO3D_DR_Buf = u32HW2DTO3D_DR_Buf;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_HW2DTO3D_BUFFER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_HW2DTo3D_Parameters(XC_DEVICE_ID *pDeviceId, XC_EX_3D_HW2DTO3D_PARA st3DHw2DTo3DPara)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_3D_HW2DTO3D_PARA,XC_EX_3D_HW2DTO3D_PARA);
    stXC_SET_3D_HW2DTO3D_PARAMETERS XCArgs;
    XCArgs.st3DHw2DTo3DPara = *((MS_XC_3D_HW2DTO3D_PARA*)&st3DHw2DTo3DPara);
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_HW2DTO3D_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Get_3D_HW2DTo3D_Parameters(XC_DEVICE_ID *pDeviceId, XC_EX_3D_HW2DTO3D_PARA *pst3DHw2DTo3DPara)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_3D_HW2DTO3D_PARA,XC_EX_3D_HW2DTO3D_PARA);
    stXC_GET_3D_HW2DTO3D_PARAMETERS XCArgs;
    XCArgs.pst3DHw2DTo3DPara = (MS_XC_3D_HW2DTO3D_PARA*)pst3DHw2DTo3DPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_HW2DTO3D_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_Detect3DFormat_Parameters(XC_DEVICE_ID *pDeviceId, XC_EX_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_3D_DETECT3DFORMAT_PARA,XC_EX_3D_DETECT3DFORMAT_PARA);
    stXC_SET_3D_DETECT_3DFORMAT_PARAMETERS XCArgs;
    XCArgs.pstDetect3DFormatPara = (MS_XC_3D_DETECT3DFORMAT_PARA*)pstDetect3DFormatPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_DETECT_3DFORMAT_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Get_3D_Detect3DFormat_Parameters(XC_DEVICE_ID *pDeviceId, XC_EX_3D_DETECT3DFORMAT_PARA *pstDetect3DFormatPara)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_3D_DETECT3DFORMAT_PARA,XC_EX_3D_DETECT3DFORMAT_PARA);
    stXC_GET_3D_DETECT_3DFORMAT_PARAMETERS XCArgs;
    XCArgs.pstDetect3DFormatPara = (MS_XC_3D_DETECT3DFORMAT_PARA*)pstDetect3DFormatPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_DETECT_3DFORMAT_PARAMETERS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_3D_INPUT_MODE MApi_XC_EX_Detect3DFormatByContent(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_3D_FORMAT_DETECTED_BY_CONTENT XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_FORMAT_DETECTED_BY_CONTENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_EX_DetectNL(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow, XC_EX_DETECTNL_PARA* pstDetectNLatticePara)
{
    _XC_ST_CHECK_SIZE(ST_DETECTNL_PARA,XC_EX_DETECTNL_PARA);
    stXC_SET_NINELATTICE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.pstDetectNLatticePara = (ST_DETECTNL_PARA*)pstDetectNLatticePara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_NINELATTICE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_3D_PostPQSetting(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_POST_PQSETTING XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_POST_PQSETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_FPInfo(XC_DEVICE_ID *pDeviceId, XC_EX_3D_FPINFO_PARA *pstFPInfoPara)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_3D_FPINFO_PARA,XC_EX_3D_FPINFO_PARA);
    stXC_SET_3D_FRAMEPACKING_INFO XCArgs;
    XCArgs.pstFPInfoPara = (MS_XC_3D_FPINFO_PARA*)pstFPInfoPara;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_FRAMEPACKING_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_EnableAutoDetect3D(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_3D_AUTODETECT_METHOD enDetectMethod)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_AUTODETECT XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.enDetectMethod = enDetectMethod;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_AUTODETECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
MS_BOOL MApi_XC_EX_GetAutoDetect3DFlag(XC_DEVICE_ID *pDeviceId, XC_EX_3D_AUTODETECT_METHOD *penDetectMethod, MS_BOOL *pbEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_3D_AUTODETECT_3DFLAG XCArgs;
    XCArgs.penDetectMethod = (E_XC_3D_AUTODETECT_METHOD*)penDetectMethod;
    XCArgs.pbEnable = pbEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_3D_AUTODETECT_3DFLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_3D_SubWinClk(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_SUBWINCLOCK XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_SUBWINCLOCK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_3D_Is_LR_Sbs2Line(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_3D_LR_SBS2LINE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_3D_LR_SBS2LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_SC_EX_3D_Is_Skip_Default_LR_Flag(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_3D_SKIP_DEFAULT_LR_FLAG XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_3D_SKIP_DEFAULT_LR_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_3D_Enable_Skip_Default_LR_Flag(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_3D_SKIP_DEFAULT_LR_FLAG XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_3D_SKIP_DEFAULT_LR_FLAG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

typedef void (*pExFunc)(INPUT_SOURCE_TYPE_t , E_MUX_INPUTPORT* , MS_U8* );
void    MApi_XC_EX_Mux_Init(XC_DEVICE_ID *pDeviceId, void (*input_source_to_input_port)(XC_EX_INPUT_SOURCE_TYPE src_ids , E_MUX_INPUTPORT* port_ids , MS_U8* u8port_count ) )
{
    XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
    INIT_XC_INSTANT_ATTR(stXCInstantAttribute);

    if (pDeviceId->u32Id == 0 && pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }
    else if (pDeviceId->u32Id == 1 && pu32XCInst_1 == NULL)
    {
        stXCInstantAttribute.u32DeviceID = 1;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return;
        }
    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return;
    }

    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MUX_INIT XCArgs;
    XCArgs.input_source_to_input_port = (pExFunc)input_source_to_input_port;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_Mux_SourceMonitor(XC_DEVICE_ID *pDeviceId, MS_BOOL bRealTimeMonitorOnly)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        //printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MUX_SOURCE_MONITOR XCArgs;
    XCArgs.bRealTimeMonitorOnly = bRealTimeMonitorOnly;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_SOURCE_MONITOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_S16  MApi_XC_EX_Mux_CreatePath(XC_DEVICE_ID *pDeviceId, XC_EX_MUX_PATH_INFO* Path_Info, MS_U32 u32InitDataLen)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    _XC_ST_CHECK_SIZE(XC_MUX_PATH_INFO,XC_EX_MUX_PATH_INFO);
    stXC_SET_MUX_CREATE_PATH XCArgs;
    XCArgs.Path_Info = (XC_MUX_PATH_INFO*)Path_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_CREATE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

MS_S16  MApi_XC_EX_Mux_DeletePath(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src, XC_EX_DEST_TYPE dest)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_SET_MUX_DELETE_PATH XCArgs;
    XCArgs.src = src;
    XCArgs.dest = dest;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_DELETE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

MS_S16  MApi_XC_EX_Mux_EnablePath(XC_DEVICE_ID *pDeviceId, MS_U16 PathId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_SET_MUX_ENABLE_PATH XCArgs;
    XCArgs.PathId = PathId;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_ENABLE_PATH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

void    MApi_XC_EX_Mux_TriggerPathSyncEvent(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src , void* para)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MUX_TRIGGER_PATH_SYNC_EVENT XCArgs;
    XCArgs.src = src;
    XCArgs.para = para;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_TRIGGER_PATH_SYNC_EVENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_Mux_TriggerDestOnOffEvent(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src , void* para)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MUX_TRIGGER_DEST_ONOFF_EVENT XCArgs;
    XCArgs.src = src;
    XCArgs.para = para;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_TRIGGER_DEST_ONOFF_EVENT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_S16  MApi_XC_EX_Mux_OnOffPeriodicHandler(XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_SET_MUX_ONOFF_PERIODIC_HANDLER XCArgs;
    XCArgs.src = src;
    XCArgs.bEnable = bEnable;
    XCArgs.s16ReturnValue = -1;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_ONOFF_PERIODIC_HANDLER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return -1;
    }
    else
    {
        return XCArgs.s16ReturnValue;
    }
}

MS_U8   MApi_XC_EX_Mux_GetPathInfo(XC_DEVICE_ID *pDeviceId, XC_EX_MUX_PATH_INFO* Paths)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    _XC_ST_CHECK_SIZE(XC_MUX_PATH_INFO,XC_EX_MUX_PATH_INFO);
    stXC_GET_MUX_PATHINFO XCArgs;
    XCArgs.Paths = (XC_MUX_PATH_INFO*)Paths;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_MUX_PATHINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
} // Return current all path info. Max path number is MAX_DATA_PATH_SUPPORTED

XC_EX_RETURN_VALUE MApi_XC_EX_Mux_SetSupportMhlPathInfo(XC_DEVICE_ID *pDeviceId, MS_U8 u8MhlSupportInfo)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_MUX_SUPPORT_MHL_PATHINFO XCArgs;
    XCArgs.u8MhlSupportInfo = u8MhlSupportInfo;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_SUPPORT_MHL_PATHINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Mux_SetMhlHotPlugInverseInfo(XC_DEVICE_ID *pDeviceId, MS_BOOL bIsMhlHotPlugInverse)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_MUX_MHL_HOTPLUG_INVERSE_INFO XCArgs;
    XCArgs.bIsMhlHotPlugInverse = bIsMhlHotPlugInverse;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MUX_MHL_HOTPLUG_INVERSE_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_MUX_INPUTPORT MApi_XC_EX_Mux_GetHDMIPort( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE src )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return INPUT_PORT_NONE_PORT;
    }

    stXC_GET_MUX_HDMIPORT XCArgs;
    XCArgs.src = src;
    XCArgs.eReturnValue = INPUT_PORT_NONE_PORT;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_MUX_HDMIPORT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return INPUT_PORT_NONE_PORT;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_U8   MApi_XC_EX_MUX_MapInputSourceToVDYMuxPORT( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE u8InputSourceType )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_MUX_INPUTSOURCE2VDYMUXPORT XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_MUX_INPUTSOURCE2VDYMUXPORT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

void    MApi_XC_EX_Set_NR(XC_DEVICE_ID *pDeviceId, MS_BOOL bEn, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_NR XCArgs;
    XCArgs.bEn = bEn;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_NR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_FilmMode_P(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FILM_MODE_PATCH, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_GetUCEnabled(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_UC_ENABLED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_UC_ENABLED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_GenSpecificTiming(XC_DEVICE_ID *pDeviceId, XC_EX_Internal_TimingType timingtype)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_GENERATE_SPECIFIC_TIMING XCArgs;
    XCArgs.timingtype = timingtype;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_GENERATE_SPECIFIC_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_GetDEBypassMode(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_DE_BYPASS_MODE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DE_BYPASS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_GetDEWindow(XC_DEVICE_ID *pDeviceId, XC_EX_WINDOW_TYPE *psWin, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_GET_DE_WINDOW XCArgs;
    XCArgs.psWin = (MS_WINDOW_TYPE*)psWin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DE_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_GetDEWidthHeightInDEByPassMode(XC_DEVICE_ID *pDeviceId, MS_U16* pu16Width,MS_U16* pu16Height ,XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE XCArgs;
    XCArgs.pu16Width = pu16Width;
    XCArgs.pu16Height = pu16Height;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_GetCaptureWindow(XC_DEVICE_ID *pDeviceId, XC_EX_WINDOW_TYPE* capture_win, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

     _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_GET_CAPTURE_WINDOW XCArgs;
    XCArgs.capture_win = (MS_WINDOW_TYPE*)capture_win;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_CAPTURE_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetCaptureWindowVstart(XC_DEVICE_ID *pDeviceId, MS_U16 u16Vstart , XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_VSTART XCArgs;
    XCArgs.u16Vstart = u16Vstart;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CAPTURE_WINDOW_VSTART, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetCaptureWindowHstart(XC_DEVICE_ID *pDeviceId, MS_U16 u16Hstart , XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_HSTART XCArgs;
    XCArgs.u16Hstart = u16Hstart;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CAPTURE_WINDOW_HSTART, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetCaptureWindowVsize(XC_DEVICE_ID *pDeviceId, MS_U16 u16Vsize , XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_VSIZE XCArgs;
    XCArgs.u16Vsize = u16Vsize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CAPTURE_WINDOW_VSIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetCaptureWindowHsize(XC_DEVICE_ID *pDeviceId, MS_U16 u16Hsize , XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_HSIZE XCArgs;
    XCArgs.u16Hsize = u16Hsize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CAPTURE_WINDOW_HSIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SoftwareReset(XC_DEVICE_ID *pDeviceId, MS_U8 u8Reset, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_SOFTWARE_RESET XCArgs;
    XCArgs.u8Reset = u8Reset;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SOFTWARE_RESET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16  MApi_XC_EX_CalculateHFreqx10(XC_DEVICE_ID *pDeviceId, MS_U16 u16HPeriod)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_HFREQX10 XCArgs;
    XCArgs.u16HPeriod = u16HPeriod;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_HFREQX10, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U32 MApi_XC_EX_CalculateHFreqx1K(XC_DEVICE_ID *pDeviceId, MS_U16 u16HPeriod)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }


    stXC_GET_HFREQX1K XCArgs;
    XCArgs.u16HPeriod = u16HPeriod;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_HFREQX1K, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_U16  MApi_XC_EX_CalculateVFreqx10(XC_DEVICE_ID *pDeviceId, MS_U16 u16HFreq, MS_U16 u16VTotal)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_VFREQX10 XCArgs;
    XCArgs.u16HFreq = u16HFreq;
    XCArgs.u16VTotal = u16VTotal;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_VFREQX10, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U32 MApi_XC_EX_CalculateVFreqx1K(XC_DEVICE_ID *pDeviceId, MS_U32 u16HFreq, MS_U16 u16VTotal)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_VFREQX1K XCArgs;
    XCArgs.u32HFreqx1K = u16HFreq;
    XCArgs.u16VTotal = u16VTotal;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_VFREQX1K, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_U32 MApi_XC_EX_GetAccurateVFreqx1K(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_ACCURATE_VFREQX1k XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_ACCURATE_VFREQX1k, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_InterruptAttach(XC_DEVICE_ID *pDeviceId, SC_EX_INT_SRC enIntNum, SC_EX_InterruptCb pIntCb, void * pParam)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_INTERRUPT_ATTACH XCArgs;
    XCArgs.eIntNum = enIntNum;
    XCArgs.pIntCb = (SC_InterruptCb)pIntCb;
    XCArgs.pParam = pParam;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_INTERRUPT_ATTACH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_InterruptDeAttach(XC_DEVICE_ID *pDeviceId, SC_EX_INT_SRC enIntNum, SC_EX_InterruptCb pIntCb, void * pParam)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_INTERRUPT_DEATTACH XCArgs;
    XCArgs.eIntNum = enIntNum;
    XCArgs.pIntCb = (SC_InterruptCb)pIntCb;
    XCArgs.pParam = pParam;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_INTERRUPT_DEATTACH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_DisableInputSource(XC_DEVICE_ID *pDeviceId, MS_BOOL bDisable, XC_EX_SCALER_WIN eWidnow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_DISABLE_INPUTSOURCE XCArgs;
    XCArgs.bDisable = bDisable;
    XCArgs.eWindow = eWidnow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DISABLE_INPUTSOURCE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsInputSourceDisabled(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_INPUTSOURCE_DISABLED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_INPUTSOURCE_DISABLED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_ChangePanelType(XC_DEVICE_ID *pDeviceId, XC_EX_PANEL_INFO *pstPanelInfo)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_PANEL_INFO,XC_EX_PANEL_INFO);
    stXC_SET_CHANGE_PANELTYPE XCArgs;
    XCArgs.pstPanelInfo = (XC_PANEL_INFO*)pstPanelInfo;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CHANGE_PANELTYPE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16  MApi_XC_EX_GetCurrentReadBank(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_CURRENT_READBANK XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_CURRENT_READBANK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_U16  MApi_XC_EX_GetCurrentWriteBank(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_CURRENT_WRITEBANK XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_CURRENT_WRITEBANK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void    MApi_XC_EX_SetAutoPreScaling(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_AUTO_PRESCALING XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_AUTO_PRESCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8   MApi_XC_EX_GetVSyncWidth(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_VSYNC_WIDTH XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_VSYNC_WIDTH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_set_GOP_Enable(XC_DEVICE_ID *pDeviceId, MS_U8 MaxGOP, MS_U8 UseNum, MS_U8 u8MuxNum, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_GOP_ENABLE XCArgs;
    XCArgs.MaxGOP = MaxGOP;
    XCArgs.UseNum = UseNum;
    XCArgs.u8MuxNum = u8MuxNum;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_GOP_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_ip_sel_for_gop(XC_DEVICE_ID *pDeviceId, MS_U8 u8MuxNum , XC_EX_IPSEL_GOP ipSelGop)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_SELECT_IP_FOR_GOP XCArgs;
    XCArgs.u8MuxNum = u8MuxNum;
    XCArgs.ipSelGop = ipSelGop;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SELECT_IP_FOR_GOP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetVOPNBL(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VOP_NEW_BLENDING_LEVEL, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_GetDstInfo(XC_DEVICE_ID *pDeviceId, XC_EX_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo, XC_EX_GOP_XCDST_TYPE XCDstType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_DST_DispInfo,XC_EX_DST_DispInfo);
    stXC_GET_DESTINATION_INFO XCArgs;
    XCArgs.pDstInfo = (MS_XC_DST_DispInfo*)pDstInfo;
    XCArgs.u32SizeofDstInfo = u32SizeofDstInfo;
    XCArgs.XCDstType = XCDstType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DESTINATION_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_set_FD_Mask(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FDMASK_BYWIN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = MAIN_WINDOW;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Get_FD_Mask(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_FDMASK_BYWIN XCArgs;
    XCArgs.eWindow      = MAIN_WINDOW;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_set_FD_Mask_byWin(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FDMASK_BYWIN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Get_FD_Mask_byWin(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_FDMASK_BYWIN XCArgs;
    XCArgs.eWindow      = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Get_FD_Mask_Status(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FDMASK_STATUS XCArgs;
    XCArgs.eWindow      = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_FDMASK_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_SetIP1TestPattern(XC_DEVICE_ID *pDeviceId, MS_U8 u8Enable, MS_U16 u6Pattern_type, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_IP1_TESTPATTERN XCArgs;
    XCArgs.u8Enable = u8Enable;
    XCArgs.u6Pattern_type = u6Pattern_type;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IP1_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_InitIPForInternalTiming(XC_DEVICE_ID *pDeviceId, XC_EX_Internal_TimingType timingtype)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_INIT_IP_FOR_INTERNAL_TIMING XCArgs;
    XCArgs.timingtype = timingtype;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_INIT_IP_FOR_INTERNAL_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetIPMux(XC_DEVICE_ID *pDeviceId, MS_U8 u8Val)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_IPMUX XCArgs;
    XCArgs.u8Val = u8Val;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IPMUX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Is_H_Sync_Active(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_HSYNC_ACTIVE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_HSYNC_ACTIVE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Is_V_Sync_Active(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_VSYNC_ACTIVE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_VSYNC_ACTIVE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_GetAutoPositionWindow(XC_DEVICE_ID *pDeviceId, MS_U8 u8ValidData, XC_EX_SCALER_WIN eWindow, XC_EX_WINDOW_TYPE *pstAutoPositionWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE,XC_EX_WINDOW_TYPE);
    stXC_GET_AUTO_POSITION_WINDOW XCArgs;
    XCArgs.u8ValidData = u8ValidData;
    XCArgs.eWindow = eWindow;
    XCArgs.pstAutoPositionWindow = (MS_WINDOW_TYPE*)pstAutoPositionWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_AUTO_POSITION_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_SetFrameBufferAddress(XC_DEVICE_ID *pDeviceId, MS_PHY u32FBAddress, MS_PHY u32FBSize, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRAMEBUFFER_ADDRESS XCArgs;
    XCArgs.u32FBAddress = u32FBAddress;
    XCArgs.u32FBSize = u32FBSize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRAMEBUFFER_ADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_IsFrameBufferEnoughForCusScaling(XC_DEVICE_ID *pDeviceId, XC_EX_SETWIN_INFO *pstXC_SetWin_Info, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_SETWIN_INFO,XC_EX_SETWIN_INFO);
    stXC_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING XCArgs;
    XCArgs.pstXC_SetWin_Info = (XC_SETWIN_INFO*)pstXC_SetWin_Info;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetScalerMemoryRequest(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_MEMORY_REQUEST_TYPE eType, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_SCALER_MEMORY_REQUEST XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eRequestType = eType;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SCALER_MEMORY_REQUEST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_Get_PixelData(XC_DEVICE_ID *pDeviceId, MS_U16 u16CorX,  MS_U16 u16CorY, XC_EX_PIXEL_32BIT* pixel)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_PIXEL_32BIT,XC_EX_PIXEL_32BIT);
    stXC_GET_PIXEL_DATA XCArgs;
    XCArgs.u16CorX = u16CorX;
    XCArgs.u16CorY = u16CorY;
    XCArgs.pixel = (MS_PIXEL_32BIT*)pixel;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_PIXEL_DATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U32  MApi_XC_EX_GetAvailableSize(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow, MS_U8 u8FBNum, MS_U32 u32InputSize)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_AVAILABLE_SIZE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8FBNum = u8FBNum;
    XCArgs.u32InputSize = u32InputSize;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_AVAILABLE_SIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

void    MApi_XC_EX_SetFrameColor(XC_DEVICE_ID *pDeviceId, MS_U32 u32aRGB)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRAME_COLOR XCArgs;
    XCArgs.u32aRGB = u32aRGB;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRAME_COLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetDispWindowColor(XC_DEVICE_ID *pDeviceId, MS_U8 u8Color, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_DISPLAY_WINDOW_COLOR XCArgs;
    XCArgs.u8Color = u8Color;
    XCArgs.eWindow = eWindow;
    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DISPLAY_WINDOW_COLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SupportSourceToVE(XC_DEVICE_ID *pDeviceId, MS_U16* pOutputCapability)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_SUPPORT_SOURCE_TO_VE XCArgs;
    XCArgs.pOutputCapability = pOutputCapability;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_SUPPORT_SOURCE_TO_VE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetOutputCapture(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SOURCE_TO_VE eSourceToVE)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OUTPUT_CAPTURE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eSourceToVE = eSourceToVE;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OUTPUT_CAPTURE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_SetGammaOnOff (XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_GAMMA_ONOFF XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_GAMMA_ONOFF, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetPreGammaGain(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow, XC_EX_VOP_CHANNEL eVop_Channel, MS_U16 u16Val)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_PREGAMMA_GAIN XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eVop_Channel = eVop_Channel;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_PREGAMMA_GAIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetPreGammaOffset(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow, XC_EX_VOP_CHANNEL eVop_Channel, MS_U16 u16Val)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_PREGAMMA_OFFSET XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eVop_Channel = eVop_Channel;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_PREGAMMA_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetPanelTiming(XC_DEVICE_ID *pDeviceId, XC_EX_SetTiming_Info *pTimingInfo, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_SetTiming_Info,XC_EX_SetTiming_Info);
    stXC_SET_PANEL_TIMING XCArgs;
    XCArgs.pTimingInfo = (XC_SetTiming_Info*)pTimingInfo;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_PANEL_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetOutTimingMode(XC_DEVICE_ID *pDeviceId, XC_EX_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_OUTPUT_TIMING_MODE XCArgs;
    XCArgs.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OUTPUT_TIMING_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetFreeRunTiming(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FREERUN_TIMING, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Set_CustomerSyncInfo(XC_DEVICE_ID *pDeviceId, MS_U16 u16FrontPorch, MS_U16 u16SyncWidth, MS_U16 u16OutputVfreq)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_CUSTOMER_SYNC_INFO XCArgs;
    XCArgs.u16FrontPorch = u16FrontPorch;
    XCArgs.u16SyncWidth = u16SyncWidth;
    XCArgs.u16OutputVfreq = u16OutputVfreq;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CUSTOMER_SYNC_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_WaitFPLLDone(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_WAIT_FPLL_DONE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_WAIT_FPLL_DONE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U16  MApi_XC_EX_GetOutputVFreqX100(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_OUTPUT_VFREQX100 XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_OUTPUT_VFREQX100, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_FrameLockCheck(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_FRAMELOCK XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_FRAMELOCK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_CustomizeFRCTable(XC_DEVICE_ID *pDeviceId, XC_EX_FRC_SETTING* stFRCTable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(SC_FRC_SETTING, XC_EX_FRC_SETTING);
    stXC_SET_CUSTOMIZE_FRC_TABLE XCArgs;
    XCArgs.stFRCTable = (SC_FRC_SETTING*)stFRCTable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CUSTOMIZE_FRC_TABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_OutputFrameCtrl(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_OUTPUTFRAME_Info * stOutFrameInfo,XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(XC_OUTPUTFRAME_Info, XC_EX_OUTPUTFRAME_Info);
    stXC_SET_OUTPUT_FRAME_CONTROL XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.pstOutFrameInfo = (XC_OUTPUTFRAME_Info*)stOutFrameInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OUTPUT_FRAME_CONTROL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_SC_EX_Enable_FPLL_FSM(XC_DEVICE_ID *pDeviceId, MS_BOOL bTrue)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FPLL_FSM_ENABLE XCArgs;
    XCArgs.bTrue = bTrue;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FPLL_FSM_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_SC_EX_ForceFreerun(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FORCE_FREERUN XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FORCE_FREERUN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_SC_EX_IsForceFreerun(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_FORCE_FREERUN XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_FORCE_FREERUN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_SC_EX_SetFreerunVFreq(XC_DEVICE_ID *pDeviceId, XC_EX_VFREQ_SEL VFreq)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FREERUN_FREQ XCArgs;
    XCArgs.VFreq = VFreq;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FREERUN_FREQ, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_SetExPanelInfo(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_PANEL_INFO_EX *pPanelInfoEx)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(XC_PANEL_INFO_EX, XC_EX_PANEL_INFO_EX);
    stXC_SET_EXPANEL_INFO XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.pPanelInfoEx = (XC_PANEL_INFO_EX*)pPanelInfoEx;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_EXPANEL_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_Enable_FPLL_Thresh_Mode(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FPLL_THRESHMODE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FPLL_THRESHMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_EnableIPAutoNoSignal(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_IPAUTONOSIGNAL XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IPAUTONOSIGNAL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_EX_GetIPAutoNoSignal(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_IPAUTONOSIGNAL XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_IPAUTONOSIGNAL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_EnableIPAutoCoast(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_IPAUTOCOAST XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IPAUTOCOAST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_EnableIPCoastDebounce(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IPCOAST_DEBOUNCE_ENABLE, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_ClearIPCoastStatus(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CLEAR_IPCOAST_STATUS, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_EnableFpllManualSetting(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FPLL_MANUAL_SETTING_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FPLL_MANUAL_SETTING_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}                           // debug purpose

void    MApi_XC_EX_FpllBoundaryTest(XC_DEVICE_ID *pDeviceId, MS_U32 u32FpllSetOffset, MS_U16 u16TestCnt)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FPLL_BOUNDARY_TEST XCArgs;
    XCArgs.u32FpllSetOffset = u32FpllSetOffset;
    XCArgs.u16TestCnt = u16TestCnt;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FPLL_BOUNDARY_TEST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}       // debug purpose

void    MApi_XC_EX_SetOffLineDetection ( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE u8InputSourceType )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_OFFLINE_DETECTION XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OFFLINE_DETECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8   MApi_XC_EX_GetOffLineDetection( XC_DEVICE_ID *pDeviceId, XC_EX_INPUT_SOURCE_TYPE u8InputSourceType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_OFFLINE_DETECTION XCArgs;
    XCArgs.u8InputSourceType = u8InputSourceType;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_OFFLINE_DETECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetOffLineSogThreshold(XC_DEVICE_ID *pDeviceId, MS_U8 u8Threshold)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_OFFLINE_SOG_THRESHOLD XCArgs;
    XCArgs.u8Threshold = u8Threshold;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OFFLINE_SOG_THRESHOLD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
} //t2,u3,u4: 0~31 other chip: 0~255

MS_BOOL MApi_XC_EX_SetOffLineSogBW(XC_DEVICE_ID *pDeviceId, MS_U8 u8BW)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_OFFLINE_SOG_BW XCArgs;
    XCArgs.u8BW = u8BW;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OFFLINE_SOG_BW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_OffLineInit(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_OFFLINE_INIT XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OFFLINE_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_Set_Extra_fetch_adv_line(XC_DEVICE_ID *pDeviceId, MS_U8 u8val)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_EXTRA_FETCH_ADV_LINE XCArgs;
    XCArgs.u8val = u8val;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_EXTRA_FETCH_ADV_LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetVGASogEn(XC_DEVICE_ID *pDeviceId, MS_BOOL bVGASogEn)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VGA_SOG_ENABLE XCArgs;
    XCArgs.bVGASogEn = bVGASogEn;
    XCArgs.eReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VGA_SOG_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_EnableWindow(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_WINDOW_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_WINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Is_SubWindowEanble(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_SUBWINDOW_ENABLE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_SUBWINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_SetBorderFormat( XC_DEVICE_ID *pDeviceId, MS_U8 u8Left, MS_U8 u8Right, MS_U8 u8Up, MS_U8 u8Down, MS_U8 u8color, XC_EX_SCALER_WIN eWindow )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BORDER_FORMAT XCArgs;
    XCArgs.u8Left = u8Left;
    XCArgs.u8Right = u8Right;
    XCArgs.u8Up = u8Up;
    XCArgs.u8Down = u8Down;
    XCArgs.u8color = u8color;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BORDER_FORMAT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_EnableBorder(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BORDER_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BORDER_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_ZorderMainWindowFirst(XC_DEVICE_ID *pDeviceId, MS_BOOL bMainFirst)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_ZORDER_MAINWINDOW_FIRST XCArgs;
    XCArgs.bMainFirst = bMainFirst;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_ZORDER_MAINWINDOW_FIRST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_PQ_LoadFunction(XC_DEVICE_ID *pDeviceId, PQ_EX_Function_Info* function_Info , MS_U32 u32InitDataLen)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(PQ_Function_Info, PQ_EX_Function_Info);
    stXC_SET_PQ_LOADFUNCTION XCArgs;
    XCArgs.function_Info = (PQ_Function_Info*)function_Info;
    XCArgs.u32InitDataLen = u32InitDataLen;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_PQ_LOADFUNCTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Check_HNonLinearScaling(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_H_NONLINEAR_SCALING XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_H_NONLINEAR_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_EnableEuroHdtvSupport(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_EUROHDTV_SUPPORT_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_EUROHDTV_SUPPORT_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_EnableEuroHdtvDetection(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_EUROHDTV_DETECTION_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_EUROHDTV_DETECTION_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8   MApi_XC_EX_ReadByte(MS_U32 u32Reg)
{
    return (MS_U8)MDrv_ReadByte( u32Reg );
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return 0;
    }

    stXC_READBYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_READBYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

void    MApi_XC_EX_WriteByte(MS_U32 u32Reg, MS_U8 u8Val)
{
    MDrv_WriteByte( u32Reg, u8Val );
    return;
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stXC_WRITEBYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u8Val = u8Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_WRITEBYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_WriteByteMask(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Msk)
{
    MDrv_WriteByteMask( u32Reg, u8Val, u8Msk );
    return;
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stXC_WRITEBYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u8Val = u8Val;
    XCArgs.u8Msk = u8Msk;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_WRITEBYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_Write2ByteMask(MS_U32 u32Reg, MS_U16 u16val, MS_U16 u16Mask)
{
    MDrv_Write2ByteMask(u32Reg, u16val, u16Mask);
    return;
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return;
    }

    stXC_WRITE2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16val = u16val;
    XCArgs.u16Mask = u16Mask;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_WRITE2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_W2BYTE(XC_DEVICE_ID *pDeviceId, MS_U32 u32Reg, MS_U16 u16Val)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_W2BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_W2BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16  MApi_XC_EX_R2BYTE(XC_DEVICE_ID *pDeviceId, MS_U32 u32Reg)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_R2BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_R2BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void    MApi_XC_EX_W4BYTE(XC_DEVICE_ID *pDeviceId, MS_U32 u32Reg, MS_U32 u32Val)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_W4BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u32Val = u32Val;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_W4BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U32  MApi_XC_EX_R4BYTE(XC_DEVICE_ID *pDeviceId, MS_U32 u32Reg)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_R4BYTE XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_R4BYTE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_U16  MApi_XC_EX_R2BYTEMSK(XC_DEVICE_ID *pDeviceId, MS_U32 u32Reg, MS_U16 u16Mask)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_R2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16mask = u16Mask;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_R2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

void    MApi_XC_EX_W2BYTEMSK(XC_DEVICE_ID *pDeviceId, MS_U32 u32Reg, MS_U16 u16Val, MS_U16 u16Mask)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_W2BYTEMSK XCArgs;
    XCArgs.u32Reg = u32Reg;
    XCArgs.u16Val = u16Val;
    XCArgs.u16mask = u16Mask;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_W2BYTEMSK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_MLoad_Init(XC_DEVICE_ID *pDeviceId, MS_PHY PhyAddr, MS_U32 u32BufByteLen)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_MLOAD_INIT XCArgs;
    XCArgs.PhyAddr = PhyAddr;
    XCArgs.u32BufByteLen = u32BufByteLen;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLOAD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_MLoad_Enable(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_MLOAD_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLOAD_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_MLOAD_TYPE MApi_XC_EX_MLoad_GetStatus(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_MLOAD_UNSUPPORTED;
    }

    stXC_MLOAD_GETSTATUS XCArgs;
    XCArgs.eReturnValue = E_MLOAD_UNSUPPORTED;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLOAD_GETSTATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_MLOAD_UNSUPPORTED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_EX_MLoad_WriteCmd_And_Fire(XC_DEVICE_ID *pDeviceId, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_MLOAD_WRITE_CMD_AND_FIRE XCArgs;
    XCArgs.u32Addr = u32Addr;
    XCArgs.u16Data = u16Data;
    XCArgs.u16Mask = u16Mask;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLOAD_WRITE_CMD_AND_FIRE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_MLoad_WriteCmds_And_Fire(XC_DEVICE_ID *pDeviceId, MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_MLOAD_WRITE_CMDS_AND_FIRE XCArgs;
    XCArgs.pu32Addr = pu32Addr;
    XCArgs.pu16Data = pu16Data;
    XCArgs.pu16Mask = pu16Mask;
    XCArgs.u16CmdCnt = u16CmdCnt;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLOAD_WRITE_CMDS_AND_FIRE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_MLG_Init(XC_DEVICE_ID *pDeviceId, MS_PHY PhyAddr, MS_U32 u32BufByteLen)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_MLG_INIT XCArgs;
    XCArgs.PhyAddr = PhyAddr;
    XCArgs.u32BufByteLen = u32BufByteLen;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLG_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_MLG_Enable(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_MLG_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLG_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_MLG_GetCaps(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_MLG_GET_CAPS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLG_GET_CAPS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_MLG_TYPE MApi_XC_EX_MLG_GetStatus(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_MLG_UNSUPPORTED;
    }

    stXC_MLG_GETSTATUS XCArgs;
    XCArgs.eReturnValue = E_MLG_UNSUPPORTED;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_MLG_GETSTATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_MLG_UNSUPPORTED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_SetOSD2VEMode(XC_DEVICE_ID *pDeviceId, XC_EX_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_OSD2VEMODE XCArgs;
    XCArgs.eVOPSelOSD_MUX = eVOPSelOSD_MUX;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OSD2VEMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_IP2_PreFilter_Enable(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_IP2_PREFILTER_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_IP2_PREFILTER_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_GET_PixelRGB_ReturnValue MApi_XC_EX_Get_Pixel_RGB(XC_DEVICE_ID *pDeviceId, XC_EX_Get_Pixel_RGB *pData, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_GET_PIXEL_RET_OUT_OF_RANGE;
    }

    _XC_ST_CHECK_SIZE(XC_Get_Pixel_RGB, XC_EX_Get_Pixel_RGB);
    stXC_GET_PIXELRGB XCArgs;
    XCArgs.pData = (XC_Get_Pixel_RGB*)pData;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_GET_PIXEL_RET_OUT_OF_RANGE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_PIXELRGB, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_GET_PIXEL_RET_OUT_OF_RANGE;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_KeepPixelPointerAppear(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_KEEP_PIXELPOINTER_APPEAR XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_KEEP_PIXELPOINTER_APPEAR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Set_MemFmtEx(XC_DEVICE_ID *pDeviceId, XC_EX_MEM_FMT eMemFmt, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_MEMORY_FORMAT_EX XCArgs;
    XCArgs.eMemFmt = eMemFmt;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MEMORY_FORMAT_EX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_IsRequestFrameBufferLessMode(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_CHECK_REQUEST_FRAMEBUFFERLESS_MODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_CHECK_REQUEST_FRAMEBUFFERLESS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SkipSWReset(XC_DEVICE_ID *pDeviceId, MS_BOOL bFlag)
{
    XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
    INIT_XC_INSTANT_ATTR(stXCInstantAttribute);

    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        if (pDeviceId->u32Id == 0)
        {
            if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaOpen XC failed\n");
                return E_APIXC_RET_FAIL;
            }
        }
        else if (pDeviceId->u32Id == 1)
        {
            stXCInstantAttribute.u32DeviceID = 1;
            if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
            {
                printf("UtopiaOpen XC failed\n");
                return E_APIXC_RET_FAIL;
            }
        }

    }

    stXC_SET_SKIP_SWRESET XCArgs;
    XCArgs.bFlag = bFlag;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SKIP_SWRESET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_EnableRepWindowForFrameColor(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_REPORTWINDOW_FOR_FRAMECOLOR XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_REPORTWINDOW_FOR_FRAMECOLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetOSDLayer(XC_DEVICE_ID *pDeviceId, XC_EX_VOP_OSD_LAYER_SEL eVOPOSDLayer, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OSDLAYER XCArgs;
    XCArgs.eVOPOSDLayer = eVOPOSDLayer;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OSDLAYER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_VOP_OSD_LAYER_SEL MApi_XC_EX_GetOSDLayer(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_VOP_LAYER_RESERVED;
    }

    stXC_GET_OSDLAYER XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_VOP_LAYER_RESERVED;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_OSDLAYER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_VOP_LAYER_RESERVED;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetVideoAlpha(XC_DEVICE_ID *pDeviceId, MS_U8 u8Val, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VIDEO_ALPHA XCArgs;
    XCArgs.u8Val = u8Val;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VIDEO_ALPHA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_GetVideoAlpha(XC_DEVICE_ID *pDeviceId, MS_U8 *pu8Val, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_VIDEO_ALPHA XCArgs;
    XCArgs.pu8Val = pu8Val;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_VIDEO_ALPHA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SkipWaitVsync( XC_DEVICE_ID *pDeviceId, MS_BOOL eWindow, MS_BOOL bIsSkipWaitVsyn)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SKIP_WAIT_VSYNC XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bIsSkipWaitVsyn = bIsSkipWaitVsyn;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SKIP_WAIT_VSYNC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_OP2VOPDESel(XC_DEVICE_ID *pDeviceId, XC_EX_OP2VOP_DE_SEL eVopDESel)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP2VOP_DE_SELECTION XCArgs;
    XCArgs.eVopDESel = eVopDESel;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OP2VOP_DE_SELECTION, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_FRC_SetWindow(XC_DEVICE_ID *pDeviceId, XC_EX_3D_INPUT_MODE e3dInputMode, XC_EX_3D_OUTPUT_MODE e3dOutputMode, XC_EX_3D_PANEL_TYPE e3dPanelType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRC_WINDOW XCArgs;
    XCArgs.e3dInputMode = e3dInputMode;
    XCArgs.e3dOutputMode = e3dOutputMode;
    XCArgs.e3dPanelType = e3dPanelType;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRC_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Enable_TwoInitFactor(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_2INITFACTOR_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_2INITFACTOR_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }

}

MS_BOOL MApi_XC_EX_IsFieldPackingModeSupported(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d] ID %ld, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,0L);
        return FALSE;
    }

    stXC_CHECK_FIELDPACKINGMODE_SUPPORTED XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FIELDPACKINGMODE_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_PreInit( XC_DEVICE_ID *pDeviceId, XC_EX_PREINIT eType ,void* para, MS_U32 u32Length)
{
    XC_INSTANT_ATTRIBUTE stXCInstantAttribute;
    INIT_XC_INSTANT_ATTR(stXCInstantAttribute);

    if (pDeviceId->u32Id == 0 && pu32XCInst == NULL)
    {
        if(UtopiaOpen(MODULE_XC, &pu32XCInst, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    else if (pDeviceId->u32Id == 1 && pu32XCInst_1 == NULL)
    {
        stXCInstantAttribute.u32DeviceID = 1;
        if(UtopiaOpen(MODULE_XC, &pu32XCInst_1, 0, &stXCInstantAttribute) != UTOPIA_STATUS_SUCCESS)
        {
            printf("UtopiaOpen XC failed\n");
            return E_APIXC_RET_FAIL;
        }
    }
    else if(pDeviceId->u32Id > 1)
    {
        printf("Device Id over 2\n");
        return E_APIXC_RET_FAIL;
    }

    stXC_PREINIT XCArgs;
    XCArgs.eType = eType;
    XCArgs.para = para;
    XCArgs.u32Length = u32Length;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_PREINIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void    MApi_XC_EX_Get_BufferData(XC_DEVICE_ID *pDeviceId, XC_EX_OUTPUTDATA_TYPE eBufType, XC_EX_WINDOW_TYPE *pRect, void *pRectBuf, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE, XC_EX_WINDOW_TYPE);
    stXC_GET_BUFFERDATA XCArgs;
    XCArgs.eBufType = eBufType;
    XCArgs.pRect = (MS_WINDOW_TYPE*)pRect;
    XCArgs.pRectBuf = pRectBuf;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_BUFFERDATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_Set_BufferData(XC_DEVICE_ID *pDeviceId, XC_EX_INPUTDATA_TYPE eBufType, XC_EX_WINDOW_TYPE *pDstRect, void *pSrcBuf, XC_EX_WINDOW_TYPE *pSrcRect, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE, XC_EX_WINDOW_TYPE);
    stXC_SET_BUFFERDATA XCArgs;
    XCArgs.eBufType = eBufType;
    XCArgs.pDstRect = (MS_WINDOW_TYPE*)pDstRect;
    XCArgs.pSrcBuf = pSrcBuf;
    XCArgs.pSrcRect = (MS_WINDOW_TYPE*)pSrcRect;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BUFFERDATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_EnableMainWindow(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MAINWINDOW_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MAINWINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
} // Turn on/off main/sub window

void    MApi_XC_EX_EnableSubWindow(XC_DEVICE_ID *pDeviceId, XC_EX_WINDOW_TYPE *pstDispWin)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(MS_WINDOW_TYPE, XC_EX_WINDOW_TYPE);
    stXC_SET_SUBWINDOW_ENABLE XCArgs;
    XCArgs.pstDispWin = (MS_WINDOW_TYPE*)pstDispWin;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SUBWINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_DisableSubWindow(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SUBWINDOW_DISABLE, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetPixelShift(XC_DEVICE_ID *pDeviceId, MS_S8 s8H, MS_S8 s8V)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_PIXEL_SHIFT XCArgs;
    XCArgs.s8H = s8H;
    XCArgs.s8V = s8V;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_PIXEL_SHIFT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetVideoOnOSD(XC_DEVICE_ID *pDeviceId, XC_EX_VIDEO_ON_OSD_LAYER enlayer, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VIDEO_ON_OSD XCArgs;
    XCArgs.enlayer = enlayer;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VIDEO_ON_OSD, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetOSDBlendingFormula(XC_DEVICE_ID *pDeviceId, E_XC_EX_OSD_INDEX enOsdIndex, XC_EX_OSD_BlENDING_TYPE enType, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OSD_BLENDING_FORMULA XCArgs;
    XCArgs.enOsdIndex = enOsdIndex;
    XCArgs.enType = enType;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_OSD_BLENDING_FORMULA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_EX_ReportPixelInfo(XC_DEVICE_ID *pDeviceId, XC_EX_REPORT_PIXELINFO *pstRepPixInfo)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(MS_XC_REPORT_PIXELINFO, XC_EX_REPORT_PIXELINFO);
    stXC_GET_REPORT_PIXEL_INFO XCArgs;
    XCArgs.pstRepPixInfo = (MS_XC_REPORT_PIXELINFO*)pstRepPixInfo;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_REPORT_PIXEL_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_SetScaling( XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALING_TYPE eScaling_type, XC_EX_VECTOR_TYPE eVector_type, XC_EX_SCALER_WIN eWindow )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_SCALING XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eScaling_type = eScaling_type;
    XCArgs.eVector_type = eVector_type;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_SetMCDIBufferAddress(XC_DEVICE_ID *pDeviceId, MS_PHY u32FBAddress, MS_PHY u32FBSize, XC_EX_MCDI_TYPE eType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MCDI_BUFFERADDRESS XCArgs;
    XCArgs.u32FBAddress = u32FBAddress;
    XCArgs.u32FBSize = u32FBSize;
    XCArgs.eType = eType;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MCDI_BUFFERADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void    MApi_XC_EX_EnableMCDI(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_MCDI_TYPE eType)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_MCDI_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eType = eType;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_MCDI_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_SendCmdToFRC( XC_DEVICE_ID *pDeviceId, MS_U8 u8Cmd, MS_U8 count, XC_EX_FRC_R2_CMD_PARAMETER pFRC_R2_Para )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    _XC_ST_CHECK_SIZE(FRC_R2_CMD_PARAMETER_t, XC_EX_FRC_R2_CMD_PARAMETER);
    stXC_SEND_CMD2FRC XCArgs;
    XCArgs.u8Cmd = u8Cmd;
    XCArgs.count = count;
    XCArgs.pFRC_R2_Para = *((FRC_R2_CMD_PARAMETER_t*)(&pFRC_R2_Para));
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SEND_CMD2FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_GetMsgFromFRC(XC_DEVICE_ID *pDeviceId, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_MSG_FROM_FRC XCArgs;
    XCArgs.pu8Cmd = pu8Cmd;
    XCArgs.pu8ParaCount = pu8ParaCount;
    XCArgs.pu8Para = pu8Para;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_MSG_FROM_FRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void    MApi_XC_EX_EnableRWBankAuto(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_AUTO_RWBANK_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_AUTO_RWBANK_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_SetWRBankMappingNum(XC_DEVICE_ID *pDeviceId, MS_U8 u8Val, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_RWBANK_MAPPING_NUM XCArgs;
    XCArgs.u8Val = u8Val;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_RWBANK_MAPPING_NUM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8 MApi_XC_EX_GetWRBankMappingNum(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return 0;
    }

    stXC_GET_RWBANK_MAPPING_NUM XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_RWBANK_MAPPING_NUM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U8 MApi_XC_EX_GetWRBankMappingNumForZap(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_RWBANK_MAPPING_NUM_FOR_ZAP XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_RWBANK_MAPPING_NUM_FOR_ZAP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_BOOL MApi_XC_EX_SetBOBMode(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_BOBMODE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BOBMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_EX_SetForceReadBank(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, MS_U8 u8Bank, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FORCE_READBANK XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Bank = u8Bank;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FORCE_READBANK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_LD_Init(XC_DEVICE_ID *pDeviceId, XC_EX_LD_PANEL_TYPE eLDPanelType )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_LD_INIT XCArgs;
    XCArgs.enLDPanelType = eLDPanelType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_LD_INIT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_LD_SetMemoryAddress(XC_DEVICE_ID *pDeviceId, MS_U8 u8MIU ,MS_U32 u32LDFBase0,MS_U32 u32LDFBase1,MS_U32 u32LDBBase0,MS_U32 u32LDBBase1,MS_U32 u32EDGE2DBase,MS_U32 u32LEDoffset)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_LD_MEMORYADDRESS XCArgs;
    XCArgs.u8MIU = u8MIU;
    XCArgs.u32LDFBase0 = u32LDFBase0;
    XCArgs.u32LDFBase1 = u32LDFBase1;
    XCArgs.u32LDBBase0 = u32LDBBase0;
    XCArgs.u32LDBBase1 = u32LDBBase1;
    XCArgs.u32EDGE2DBase = u32EDGE2DBase;
    XCArgs.u32LEDOffset = u32LEDoffset;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_LD_MEMORYADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_LD_SetLevel(XC_DEVICE_ID *pDeviceId, XC_EX_LD_MODE eMode)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_LD_LEVEL XCArgs;
    XCArgs.enMode = eMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_LD_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_TurnoffLDBL(XC_DEVICE_ID *pDeviceId, MS_BOOL bturnoffbl)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_LD_TURNOFF_LDBL XCArgs;
    XCArgs.bTurnoffBL = bturnoffbl;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_LD_TURNOFF_LDBL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_notUpdateSPIDataFlags(XC_DEVICE_ID *pDeviceId, MS_BOOL brefreshSpidata)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_NOTUPDATE_SPI_DATA_FLAGS XCArgs;
    XCArgs.bRefreshSPIData = brefreshSpidata;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_NOTUPDATE_SPI_DATA_FLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_UsermodeLDFlags(XC_DEVICE_ID *pDeviceId, MS_BOOL buserLDFlags)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_LD_USERMODEFLAGS XCArgs;
    XCArgs.bUserLDFlags = buserLDFlags;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_LD_USERMODEFLAGS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_EX_Set_BLLevel(XC_DEVICE_ID *pDeviceId, MS_U8 u8BLLevel)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_SET_BL_LEVEL XCArgs;
    XCArgs.u8BLLevel = u8BLLevel;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BL_LEVEL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_EX_Set_BWS_Mode(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BWS_MODE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BWS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_FRC_ColorPathCtrl(XC_DEVICE_ID *pDeviceId, MS_U16 u16Path_sel, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRC_COLOR_PATH_CONTROL XCArgs;
    XCArgs.u16Path_sel = u16Path_sel;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRC_COLOR_PATH_CONTROL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_FRC_OP2_SetRGBGain(XC_DEVICE_ID *pDeviceId, MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRC_OP2_RGBGAIN XCArgs;
    XCArgs.u16RedGain = u16RedGain;
    XCArgs.u16GreenGain = u16GreenGain;
    XCArgs.u16BlueGain = u16BlueGain;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRC_OP2_RGBGAIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_FRC_OP2_SetRGBOffset(XC_DEVICE_ID *pDeviceId, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRC_OP2_RGBOFFSET XCArgs;
    XCArgs.u16RedOffset = u16RedOffset;
    XCArgs.u16GreenOffset = u16GreenOffset;
    XCArgs.u16BlueOffset = u16BlueOffset;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRC_OP2_RGBOFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_FRC_OP2_SetDither(XC_DEVICE_ID *pDeviceId, MS_U16 u16dither)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_FRC_OP2_DITHER XCArgs;
    XCArgs.u16dither = u16dither;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FRC_OP2_DITHER, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_FRC_BypassMFC(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_FRC_BYPASS XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_FRC_BYPASS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_ForceReadFrame(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, MS_U16 u16FrameIndex)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_FORCE_READ_FRAME XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u16FrameIndex = u16FrameIndex;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_FORCE_READ_FRAME, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MApi_XC_EX_SetCsc( XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_SCALER_WIN eWindow )
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_CSC XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_CSC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

void MApi_XC_EX_RegisterPQSetFPLLThreshMode(XC_DEVICE_ID *pDeviceId, void (*fpPQCB)(MS_U8 u8PQWin))
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_REGISTER_PQ_FPLL_THRESH_MODE XCArgs;
    XCArgs.fpPQCB = fpPQCB;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_REGISTER_PQ_FPLL_THRESH_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_GetFreeRunStatus(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_FREERUN_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_FREERUN_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Get_DSForceIndexSupported(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_DS_FORCE_INDEX_SUPPORTED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_GET_DS_FORCE_INDEX_SUPPORTED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Set_DSForceIndex(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, MS_U8 u8Index, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_FORCE_INDEX XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Index = u8Index;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DS_FORCE_INDEX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Set_DSIndexSourceSelect(XC_DEVICE_ID *pDeviceId, XC_EX_DS_INDEX_SOURCE eDSIdxSrc, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_DS_INDEX_SOURCE_SELECT XCArgs;
    XCArgs.eDSIdxSrc = eDSIdxSrc;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_DS_INDEX_SOURCE_SELECT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_OSDC_InitSetting(XC_DEVICE_ID *pDeviceId,
                                             XC_EX_OSDC_TGEN_Type E_XC_OSDC_TGEN_Type,
                                             XC_EX_OSDC_TGEN_INFO *pstOC_Tgen_Cus,
                                             XC_EX_OSDC_CTRL_INFO *pstOC_Ctrl)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(MS_XC_OSDC_TGEN_INFO, XC_EX_OSDC_TGEN_INFO);
    _XC_ST_CHECK_SIZE(MS_XC_OSDC_CTRL_INFO, XC_EX_OSDC_CTRL_INFO);
    stXC_OSDC_INITSETTING XCArgs;
    XCArgs.e_osdctgen_type = E_XC_OSDC_TGEN_Type;
    XCArgs.pstOC_Tgen_Cus = (MS_XC_OSDC_TGEN_INFO*)pstOC_Tgen_Cus;
    XCArgs.pstOC_Ctrl = (MS_XC_OSDC_CTRL_INFO*)pstOC_Ctrl;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_OSDC_INITSETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_OSDC_Control(XC_DEVICE_ID *pDeviceId, MS_U32 eCtrl_type)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_OSDC_CONTROL XCArgs;
    XCArgs.eCtrl_type = eCtrl_type;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_OSDC_CONTROL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_OSDC_GetDstInfo(XC_DEVICE_ID *pDeviceId, XC_EX_OSDC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    _XC_ST_CHECK_SIZE(MS_OSDC_DST_DispInfo,XC_EX_OSDC_DST_DispInfo);
    stXC_OSDC_GET_DESTINATION_INFO XCArgs;
    XCArgs.pDstInfo = (MS_OSDC_DST_DispInfo*)pDstInfo;
    XCArgs.u32SizeofDstInfo = u32SizeofDstInfo;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_OSDC_GET_DESTINATION_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MApi_XC_EX_OSDC_SetOutVfreqx10(XC_DEVICE_ID *pDeviceId, MS_U16 u16Vfreq)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_OSDC_SET_OUTPUTVFREQX10 XCArgs;
    XCArgs.u16Vfreq = u16Vfreq;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_OSDC_SET_OUTPUTVFREQX10, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_BYPASS_SetOSDVsyncPos(XC_DEVICE_ID *pDeviceId, MS_U16 u16VsyncStart, MS_U16 u16VsyncEnd)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BYPASS_OSDVSYNC_POS XCArgs;
    XCArgs.u16VsyncStart = u16VsyncStart;
    XCArgs.u16VsyncEnd = u16VsyncEnd;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BYPASS_OSDVSYNC_POS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_BYPASS_SetInputSrc(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, XC_EX_BYPASS_InputSource input)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BYPASS_INPUTSRC XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.input = input;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BYPASS_INPUTSRC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_EX_BYPASS_SetCSC(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    stXC_SET_BYPASS_CSC XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_BYPASS_CSC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_SetSeamlessZapping(XC_DEVICE_ID *pDeviceId, XC_EX_SCALER_WIN eWindow, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_SEAMLESS_ZAPPING XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bEnable = bEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_SEAMLESS_ZAPPING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Vtrack_SetPayloadData(XC_DEVICE_ID *pDeviceId, MS_U16 u16Timecode, MS_U8 u8OperatorID)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VTRACK_PAYLOADDATA XCArgs;
    XCArgs.u16Timecode = u16Timecode;
    XCArgs.u8OperatorID = u8OperatorID;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VTRACK_PAYLOADDATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Vtrack_SetUserDefindedSetting(XC_DEVICE_ID *pDeviceId, MS_BOOL bUserDefinded, MS_U8 *pu8Setting)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VTRACK_USERDEFINED_SETTING XCArgs;
    XCArgs.bUserDefinded = bUserDefinded;
    XCArgs.pu8Setting = pu8Setting;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VTRACK_USERDEFINED_SETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Vtrack_Enable(XC_DEVICE_ID *pDeviceId, MS_U8 u8FrameRate, MS_BOOL bEnable)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_VTRACK_ENABLE XCArgs;
    XCArgs.u8FrameRate = u8FrameRate;
    XCArgs.bEnable = bEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_SET_VTRACK_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MApi_XC_EX_PreSetPQInfo(XC_DEVICE_ID *pDeviceId, XC_EX_SETWIN_INFO *pstXC_SetWin_Info, XC_EX_SCALER_WIN eWindow)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return;
    }

    _XC_ST_CHECK_SIZE(XC_SETWIN_INFO,XC_EX_SETWIN_INFO);
    stXC_PRESET_PQINFO XCArgs;
    XCArgs.pstXC_SetWin_Info = (XC_SETWIN_INFO*)pstXC_SetWin_Info;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_CMD_PRESET_PQINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_EX_Is_OP1_TestPattern_Enabled(XC_DEVICE_ID *pDeviceId)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return FALSE;
    }

    stXC_GET_OP1_TESTPATTERN_ENABLED XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_GET_OP1_TESTPATTERN_ENABLED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

XC_EX_RETURN_VALUE MApi_XC_EX_Set_OP1_TestPattern(XC_DEVICE_ID *pDeviceId, XC_EX_OP1_PATTERN ePattern, XC_EX_OP1_PATGEN_MODE eMode)
{
    if (_XC_SELECT_INSTANCE(pDeviceId->u32Id) == NULL)
    {
        printf("[%s,%5d] ID %td, No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__,(ptrdiff_t)pDeviceId->u32Id);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OP1_TESTPATTERN XCArgs;
    XCArgs.ePattern = ePattern;
    XCArgs.eMode = eMode;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(_XC_SELECT_INSTANCE(pDeviceId->u32Id), E_XC_SET_OP1_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

#include "apiXC_DWIN.h"
#include "apiXC_DWIN_EX.h"
//XC_DWIN
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_InitByDIP(XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_InitByDIP((SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetFrameBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetFrameBufferAddress(u32FBAddress, u32FBSize, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetInputSource(XC_EX_INPUT_SOURCE_TYPE enInputSourceType, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetInputSource((INPUT_SOURCE_TYPE_t)enInputSourceType, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_GetInputSource(XC_EX_INPUT_SOURCE_TYPE *enInputSourceType, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_GetInputSource((INPUT_SOURCE_TYPE_t *)enInputSourceType, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_DisableInputSource(MS_BOOL bDisable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_DisableInputSource(bDisable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetWindow(XC_EX_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetWindow((XC_SETWIN_INFO*)pstXC_SetWin_Info, u32InitDataLen, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_GetDEWindow(XC_EX_WINDOW_TYPE *psWin, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_GetDEWindow((MS_WINDOW_TYPE*)psWin, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowVstart(MS_U16 u16Vstart, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetCaptureWindowVstart(u16Vstart, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowHstart(MS_U16 u16Hstart, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetCaptureWindowHstart(u16Hstart, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowVsize(MS_U16 u16Vsize, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetCaptureWindowVsize(u16Vsize, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetCaptureWindowHsize(MS_U16 u16Hsize, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetCaptureWindowHsize(u16Hsize, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_GetCaptureWindow(XC_EX_WINDOW_TYPE* capture_win, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_GetCaptureWindow((MS_WINDOW_TYPE*)capture_win, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnableR2Y(MS_BOOL benable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_EnableR2Y(benable, (SCALER_DIP_WIN)eWindow);
}

//==============Set===================
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SWReset(XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SWReset((SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_FrameRateCtrl(MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_FrameRateCtrl(bEnable, u16In, u16Out, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_CapOneFrame(XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_CapOneFrame((SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_CapOneFrameFast(XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_CapOneFrameFast((SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetPinpon(MS_BOOL bPinpon,MS_PHY u32PinponAddr,XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetPinpon(bPinpon, u32PinponAddr, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_ClearInt(MS_U16 u8mask, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_ClearInt(u8mask, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_Ena(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_Ena(bEnable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnaInt(MS_U16 u8mask, MS_BOOL bEnable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_EnaInt(u8mask, bEnable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetSourceScanType(XC_DIP_EX_SCAN_MODE enScanMode, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetSourceScanType((EN_XC_DIP_SCAN_MODE)enScanMode, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnaInterlaceWrite(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_EnaInterlaceWrite(bEnable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetOutputDataFmt(XC_DIP_EX_DATA_FMT fmt, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetOutputDataFmt((EN_XC_DIP_DATA_FMT)fmt, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetDIPWinProperty(XC_DIP_EX_WINPROPERTY *pstDIPWinProperty, XC_DIP_EX_WIN eWindow)
{
    ST_XC_DIP_WINPROPERTY local_pstDIPWinProperty;
    local_pstDIPWinProperty.u8BufCnt = pstDIPWinProperty->u8BufCnt;
    local_pstDIPWinProperty.u16Width = pstDIPWinProperty->u16Width;
    local_pstDIPWinProperty.u16Height = pstDIPWinProperty->u16Height;
    local_pstDIPWinProperty.u16Pitch = pstDIPWinProperty->u16Pitch;
    local_pstDIPWinProperty.u32BufStart = pstDIPWinProperty->u32BufStart;
    local_pstDIPWinProperty.u32BufEnd = pstDIPWinProperty->u32BufEnd;
    local_pstDIPWinProperty.enSource = pstDIPWinProperty->enSource;
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetDIPWinProperty(&local_pstDIPWinProperty, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_EnableY2R(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_EnableY2R(bEnable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetAlpha(MS_U8 u8AlphaVal, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetAlpha(u8AlphaVal, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SwapUV(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SwapUV(bEnable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SwapYC(MS_BOOL bEnable, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SwapYC(bEnable, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SwapRGB(MS_BOOL bEnable, XC_DIP_EX_RGB_SWAP_TYPE enSwapType, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SwapRGB(bEnable, (EN_XC_DIP_RGB_SWAP_TYPE)enSwapType, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetWinProperty_Ex(MS_U8 u8BufCnt ,MS_U16 u16Width, MS_U16 u16Height, MS_PHY u32BufStart ,MS_PHY u32BufEnd, XC_DIP_EX_SOURCE_TYPE eSource,XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetWinProperty_Ex(u8BufCnt, u16Width, u16Height, u32BufStart, u32BufEnd, (SCALER_DIP_SOURCE_TYPE)eSource, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetOutputCapture(MS_BOOL bEnable, XC_DIP_EX_OP_CAPTURE eOpCapture, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetOutputCapture(bEnable, (EN_XC_DIP_OP_CAPTURE)eOpCapture, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetMirror(MS_BOOL bHMirror, MS_BOOL bVMirror, XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetMirror(bHMirror, bVMirror, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_SetDIPRProperty(XC_DIP_EX_DIPR_PROPERTY *pstDIPRProperty, XC_DIP_EX_WIN eWindow)
{
    ST_XC_DIPR_PROPERTY local_pstDIPRProperty;
    local_pstDIPRProperty.u16Width = pstDIPRProperty ->u16Width;
    local_pstDIPRProperty.u16Height = pstDIPRProperty ->u16Height;
    local_pstDIPRProperty.u16Pitch = pstDIPRProperty ->u16Pitch;
    local_pstDIPRProperty.u32YBufAddr = pstDIPRProperty ->u32YBufAddr;
    local_pstDIPRProperty.u32CBufAddr = pstDIPRProperty ->u32CBufAddr;
    local_pstDIPRProperty.enDataFmt = pstDIPRProperty ->enDataFmt;
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_SetDIPRProperty(&local_pstDIPRProperty, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_InterruptAttach(InterruptCb pIntCb,XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_InterruptAttach(pIntCb, (SCALER_DIP_WIN)eWindow);
}
XC_EX_RETURN_VALUE MApi_XC_DIP_EX_InterruptDetach(XC_DIP_EX_WIN eWindow)
{
    return (XC_EX_RETURN_VALUE)MApi_XC_DIP_InterruptDetach((SCALER_DIP_WIN)eWindow);
}
//==============Get===================
MS_U16 MApi_XC_DIP_EX_GetIntStatus(XC_DIP_EX_WIN eWindow)
{
    return MApi_XC_DIP_GetIntStatus((SCALER_DIP_WIN)eWindow);
}
XC_DIP_EX_BUFFER_INFO MApi_XC_DIP_EX_GetBufInfo(XC_DIP_EX_WIN eWindow)
{
    BUFFER_INFO ret = MApi_XC_DIP_GetBufInfo((SCALER_DIP_WIN)eWindow);
    XC_DIP_EX_BUFFER_INFO ret_ex;
    ret_ex.u8BufCnt = ret.u8BufCnt;
    MS_U8 i;
    for(i = 0; i < XC_DIP_EX_MAX_BUFF_CNT; i++)
    {
        ret_ex.u32YBuf[i] = ret.u32YBuf[i];
        ret_ex.u32CBuf[i] = ret.u32CBuf[i];
    }
    return ret_ex;
}

#include "hwreg_ipmux.h"
XC_EX_RETURN_VALUE MApi_XC_EX_Set_WhiteBalance_Pattern(XC_DEVICE_ID *pDeviceId, MS_BOOL bEnable, MS_U16 u16R_CR_Data, MS_U16 u16G_Y_Data ,MS_U16 u16B_CB_Data)
{
    XC_EX_RETURN_VALUE enResult = E_XC_EX_RET_OK;

    W2BYTEMSK(REG_IPMUX_10_L, (bEnable << 9) , BIT(9));
    W2BYTEMSK(REG_IPMUX_10_L, (bEnable << 12), BIT(12));
    W2BYTEMSK(REG_IPMUX_10_L, (bEnable << 13), BIT(13));
    W2BYTEMSK(REG_IPMUX_10_L, (bEnable << 14), BIT(14));

    if(bEnable == TRUE)
    {
        W2BYTEMSK(REG_IPMUX_1D_L, u16R_CR_Data, BMASK(9:0));
        W2BYTEMSK(REG_IPMUX_1E_L, u16G_Y_Data,  BMASK(9:0));
        W2BYTEMSK(REG_IPMUX_1F_L, u16B_CB_Data, BMASK(9:0));
    }

    return enResult;
}

#undef _API_XC_ADC_EX_C_
#endif  // _API_XC_ADC_EX_C_
