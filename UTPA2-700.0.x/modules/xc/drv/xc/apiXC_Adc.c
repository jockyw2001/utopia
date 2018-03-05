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
/// file  apiXC_Adc.c
/// brief  Scaler API layer Interface
/// author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_XC_ADC_C_
#define _API_XC_ADC_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition

// Common
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
#include "apiXC_Adc_v2.h"
#include "xc_hwreg_utility2.h"

// Internal Definition

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

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

//******************************************************************************
/// Enable and Disable CVBS out. This will setup ADC part for CVBS out (monitor out).
/// @param bEnable \b IN Enable CVBS Out
/// @param enInputPortType \b IN The source need to monitor out.
/// @param isDigitalSource \b IN Tell the driver current source is digital source or not. (ex. If internal VIF used in ATV, it will be digital source)
//******************************************************************************
void MApi_XC_ADC_SetCVBSOut_U2(void* pInstance, MS_BOOL bEnable, E_DEST_TYPE enOutputType , INPUT_SOURCE_TYPE_t enInputPortType, MS_BOOL isDigitalSource)
{
    E_ADC_CVBSOUT_TYPE enCVBSoutType = ADC_CVBSOUT_DISABLE_1;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    E_XC_SOURCE_TO_VE eSourceToVE = E_XC_NONE;
/*
    printf("====>CVBS out is %s", (bEnable) ?("ENABLE"):("DISABLE"));
    printf(" and Output is from =%s(%x), eSourceToVE=%x, enInputPortType=%u\n", (enOutputType==20)?("CVBS_OUT1"):
                                                    (enOutputType==21)?("CVBS_OUT2"):("Others"), enOutputType, gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE, enInputPortType);
*/
    if ( bEnable )
    {
        if ( enOutputType == OUTPUT_CVBS1)
        {
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
            UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
			eSourceToVE = gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE;
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            if (IsSrcTypeDTV(enInputPortType) ||
                ( eSourceToVE!= E_XC_NONE))
            {
                //IP/DI/OP2 to VE case, need goes with VIF_VE path
                enCVBSoutType = ADC_CVBSOUT_VIF_VE_1;
            }
            else if ( IsSrcTypeATV(enInputPortType) && isDigitalSource )
            {
                enCVBSoutType = ADC_CVBSOUT_VIF_VIF_1;  // VIF source.
            }
            else if ( IsSrcTypeSV(enInputPortType) )
            {
                enCVBSoutType = ADC_CVBSOUT_SV_ON_1;
            }
            else
            {
                enCVBSoutType = ADC_CVBSOUT_CVBS_ON_1;
            }
        }
        else if ( enOutputType == OUTPUT_CVBS2)
        {
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
            UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
			eSourceToVE = gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE;
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

            if (IsSrcTypeDTV(enInputPortType) ||
                (eSourceToVE != E_XC_NONE))
            {
                //IP/DI/OP2 to VE case, need goes with VIF_VE path
                enCVBSoutType = ADC_CVBSOUT_VIF_VE_2;
            }
            else if ( IsSrcTypeATV(enInputPortType) && isDigitalSource )
            {
                enCVBSoutType = ADC_CVBSOUT_VIF_VIF_2;  // VIF source.
            }
            else if ( IsSrcTypeSV(enInputPortType) )
            {
                enCVBSoutType = ADC_CVBSOUT_SV_ON_2;
            }
            else
            {
                enCVBSoutType = ADC_CVBSOUT_CVBS_ON_2;
            }
        }
    }
    else
    {
        if ( enOutputType == OUTPUT_CVBS1)
        {
            enCVBSoutType = ADC_CVBSOUT_DISABLE_1;
        }
        else if ( enOutputType == OUTPUT_CVBS2)
        {
            enCVBSoutType = ADC_CVBSOUT_DISABLE_2;
        }
    }
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_ADC_set_cvbs_out(pInstance, enCVBSoutType );
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

}

void MApi_XC_ADC_SetCVBSOut(MS_BOOL bEnable, E_DEST_TYPE enOutputType , INPUT_SOURCE_TYPE_t enInputPortType, MS_BOOL isDigitalSource)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETCVBSOUT XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.enOutputType = enOutputType;
    XCArgs.enInputPortType = enInputPortType;
    XCArgs.isDigitalSource = isDigitalSource;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETCVBSOUT, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_ADC_IsCVBSOutEnabled_U2(void* pInstance, E_DEST_TYPE enOutputType)
{
    if (enOutputType == OUTPUT_CVBS1 || enOutputType == OUTPUT_CVBS2)
    {
        return MDrv_XC_ADC_Is_cvbs_out_enabled(pInstance, enOutputType);
    }
    else
        return FALSE;

}

MS_BOOL MApi_XC_ADC_IsCVBSOutEnabled(E_DEST_TYPE enOutputType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_ADC_ISCVBSOUTENABLED XCArgs;
    XCArgs.enOutputType = enOutputType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_ISCVBSOUTENABLED, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

/******************************************************************************/
/// Set ADC Sample clock
/// @param u16Value \b IN value need to be set.
/******************************************************************************/
void MApi_XC_ADC_SetPcClock_U2( void* pInstance, MS_U16 u16Value )
{
    //add for  mantis 0513674
    _XC_ENTRY(pInstance);
    MDrv_XC_wait_input_vsync(pInstance, 1, 50, MAIN_WINDOW);
    _XC_RETURN(pInstance);

    MDrv_XC_ADC_SetPcClock(pInstance, u16Value);
}

void MApi_XC_ADC_SetPcClock( MS_U16 u16Value )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETPCCLOCK XCArgs;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETPCCLOCK, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/******************************************************************************/
/// Set ADC Phase, pls use MApi_XC_ADC_SetPhaseEx instead
/// @param u8Value \b IN The adc phase need to be update
/******************************************************************************/
void MApi_XC_ADC_SetPhase_U2(void* pInstance, MS_U8 u8Value)
{
    MDrv_XC_ADC_SetPhaseWithVSync(pInstance, (MS_U16)u8Value);
}

void MApi_XC_ADC_SetPhase(MS_U8 u8Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETPHASE XCArgs;
    XCArgs.u8Value = u8Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETPHASE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


/******************************************************************************/
/// Set ADC Phase Ex
/// @param u16Value \b IN The adc phase need to be update
/******************************************************************************/
void MApi_XC_ADC_SetPhaseEx_U2( void* pInstance, MS_U16 u16Value )
{
    MDrv_XC_ADC_SetPhaseWithVSync(pInstance, u16Value);
}

void MApi_XC_ADC_SetPhaseEx( MS_U16 u16Value )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETPHASEEX XCArgs;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETPHASEEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//******************************************************************************
/// Get current ADC Phase set
/// @return Current Phase
//******************************************************************************
MS_U16 MApi_XC_ADC_GetPhaseRange_U2( void* pInstance )
{
    return MDrv_XC_ADC_GetPhaseRange(pInstance);
}

MS_U16 MApi_XC_ADC_GetPhaseRange( void )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETPHASERANGE XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETPHASERANGE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//******************************************************************************
/// Get current ADC Phase set
/// @return Current Phase
//******************************************************************************
MS_U8 MApi_XC_ADC_GetPhase_U2( void* pInstance )
{
    return MDrv_XC_ADC_GetPhase(pInstance);
}

MS_U8 MApi_XC_ADC_GetPhase( void )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETPHASE XCArgs;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETPHASE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U16 MApi_XC_ADC_GetPhaseEx_U2( void* pInstance )
{
    return MDrv_XC_ADC_GetPhaseEx(pInstance);
}

MS_U16 MApi_XC_ADC_GetPhaseEx( void )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETPHASEEX XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETPHASEEX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}


MS_BOOL MApi_XC_ADC_IsScartRGB_U2(void* pInstance)
{
    return MDrv_XC_ADC_IsScartRGB(pInstance);
}

MS_BOOL MApi_XC_ADC_IsScartRGB(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_ADC_ISSCARTRGB XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_ISSCARTRGB, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


MS_U16 MApi_XC_ADC_GetPcClock_U2(void* pInstance)
{
    return MDrv_XC_ADC_GetPcClock(pInstance);
}

MS_U16 MApi_XC_ADC_GetPcClock(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETPCCLOCK XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETPCCLOCK, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

/******************************************************************************/
///Get SOG level range
///@param u32Min \b OUT: min of SOG level
///@param u32Max \b OUT: max of SOG level
///@param u32Recommend_value \b OUT: recommend value
/******************************************************************************/
void MApi_XC_ADC_GetSoGLevelRange_U2(void* pInstance, MS_U32 *u32min, MS_U32 *u32max, MS_U32 *u32Recommend_value)
{
    MDrv_XC_ADC_GetSoGLevelRange(pInstance, u32min, u32max, u32Recommend_value);
}

void MApi_XC_ADC_GetSoGLevelRange(MS_U32 *u32min, MS_U32 *u32max, MS_U32 *u32Recommend_value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_GETSOGLEVELRANGE XCArgs;
    XCArgs.u32min = u32min;
    XCArgs.u32max = u32max;
    XCArgs.u32Recommend_value = u32Recommend_value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETSOGLEVELRANGE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


/******************************************************************************/
///Set SOG level
///@param u32Min \b IN: set SOG level
/******************************************************************************/
void MApi_XC_ADC_SetSoGLevel_U2(void* pInstance, MS_U32 u32Value)
{
    MDrv_XC_ADC_SetSoGLevel(pInstance, u32Value);
}

void MApi_XC_ADC_SetSoGLevel(MS_U32 u32Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETSOGLEVEL XCArgs;
    XCArgs.u32Value = u32Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETSOGLEVEL, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//******************************************************************************
/// Turn off ADC
//******************************************************************************
void MApi_XC_ADC_PowerOff_U2(void* pInstance)
{
    MDrv_XC_ADC_PowerOff(pInstance);
}

void MApi_XC_ADC_PowerOff(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_POWEROFF, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


//******************************************************************************
/// Get ADC Default gain / offset by referring input structure.
/// @param eSource \b IN The input source
/// @param pstADCSetting \b IN gain / offset need to be set
//******************************************************************************
void MApi_XC_ADC_GetDefaultGainOffset_U2(void* pInstance, INPUT_SOURCE_TYPE_t enSource , APIXC_AdcGainOffsetSetting* pstADCSetting)
{
    MDrv_XC_ADC_GetDefaultGainOffset(pInstance, enSource, pstADCSetting);
}

void MApi_XC_ADC_GetDefaultGainOffset(INPUT_SOURCE_TYPE_t enSource , APIXC_AdcGainOffsetSetting* pstADCSetting)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_GETDEFAULTGAINOFFSET XCArgs;
    XCArgs.enSource = enSource;
    XCArgs.pstADCSetting = pstADCSetting;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETDEFAULTGAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//******************************************************************************
/// get maximal value of offset
/// @param
/// @return maximal value of offset
//******************************************************************************
MS_U16 MApi_XC_ADC_GetMaximalOffsetValue_U2(void* pInstance)
{
    return MDrv_XC_ADC_GetMaximalOffsetValue(pInstance);
}

MS_U16 MApi_XC_ADC_GetMaximalOffsetValue(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETMAXIMALOFFSETVALUE XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETMAXIMALOFFSETVALUE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}


//******************************************************************************
/// get maximal value of gain
/// @param
/// @return maximal value of gain
//******************************************************************************
MS_U16 MApi_XC_ADC_GetMaximalGainValue_U2(void* pInstance)
{
    return MDrv_XC_ADC_GetMaximalGainValue(pInstance);
}

MS_U16 MApi_XC_ADC_GetMaximalGainValue(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETMAXIMALGAINVALUE XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETMAXIMALGAINVALUE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }

}

//******************************************************************************
/// get HW center Gain value
/// @param
/// @return HW default Gain value
//******************************************************************************
MS_U16 MApi_XC_ADC_GetCenterGain_U2(void* pInstance)
{
    return MDrv_XC_ADC_GetCenterGain(pInstance);
}

MS_U16 MApi_XC_ADC_GetCenterGain(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETCENTERGAIN XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETCENTERGAIN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//******************************************************************************
/// get HW center Gain value
/// @param
/// @return HW default Gain value
//******************************************************************************
MS_U16 MApi_XC_ADC_GetCenterOffset_U2(void* pInstance)
{
    return MDrv_XC_ADC_GetCenterOffset(pInstance);
}

MS_U16 MApi_XC_ADC_GetCenterOffset(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_ADC_GETCENTEROFFSET XCArgs;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GETCENTEROFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//******************************************************************************
/// Set Gain value
/// @param u8color \b IN The channel type need to be set
/// @param u16value \b IN The value need to be set.
//******************************************************************************
void MApi_XC_ADC_SetGain_U2(void* pInstance, MS_U8 u8Color, MS_U16 u16Value)
{
    MDrv_XC_ADC_SetGain(pInstance, u8Color, u16Value);
}

void MApi_XC_ADC_SetGain(MS_U8 u8Color, MS_U16 u16Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETGAIN XCArgs;
    XCArgs.u8Color = u8Color;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETGAIN, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//******************************************************************************
/// Set offset value
/// @param u8color \b IN The channel type need to be set
/// @param u16Value \b IN The value need to be set.
//******************************************************************************
void MApi_XC_ADC_SetOffset_U2(void* pInstance, MS_U8 u8Color, MS_U16 u16Value)
{
    MDrv_XC_ADC_SetOffset(pInstance, u8Color, u16Value);
}

void MApi_XC_ADC_SetOffset(MS_U8 u8Color, MS_U16 u16Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETOFFSET XCArgs;
    XCArgs.u8Color = u8Color;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//******************************************************************************
/// Set ADC gain / offset by referring input structure.
/// @param pstADCSetting \b IN gain / offset need to be set
//******************************************************************************
void MApi_XC_ADC_AdjustGainOffset_U2(void* pInstance, APIXC_AdcGainOffsetSetting* pstADCSetting)
{
    if(pstADCSetting == NULL)
    {
        return;
    }
    _XC_ENTRY(pInstance);
    MDrv_XC_ADC_SetOffsetSetting(pInstance, pstADCSetting);
    MDrv_XC_ADC_SetGainSetting(pInstance, pstADCSetting);
    _XC_RETURN(pInstance);
}

void MApi_XC_ADC_AdjustGainOffset(APIXC_AdcGainOffsetSetting* pstADCSetting)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_ADJUSTGAINOFFSET XCArgs;
    XCArgs.pstADCSetting = pstADCSetting;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_ADJUSTGAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//******************************************************************************
/// Set ADC calibration for each input source
/// @param enInputSource \b IN The source array.
//******************************************************************************
void MApi_XC_ADC_Source_Calibrate_U2(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, enInputSourceType));
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_ADC_Source_Calibrate(INPUT_SOURCE_TYPE_t enInputSourceType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SOURCE_CALIBRATE XCArgs;
    XCArgs.enInputSourceType = enInputSourceType;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SOURCE_CALIBRATE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

/******************************************************************************/
///Set SOG level
/******************************************************************************/
void MApi_XC_ADC_SetSoGCal_U2(void* pInstance)
{
    MDrv_XC_ADC_SetSoGCal(pInstance);
}

void MApi_XC_ADC_SetSoGCal(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETSOGCAL, NULL) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_SetRGB_PIPE_Delay_U2(void* pInstance, MS_U8 u8Value)
{
    MDrv_XC_ADC_SetRGB_PIPE_Delay(pInstance, u8Value);
}

void MApi_XC_ADC_SetRGB_PIPE_Delay(MS_U8 u8Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETRGB_PIPE_DELAY XCArgs;
    XCArgs.u8Value = u8Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETRGB_PIPE_DELAY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_ScartRGB_SOG_ClampDelay_U2(void* pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur)
{
    MDrv_XC_ADC_ScartRGB_SOG_ClampDelay(pInstance, u16Clpdly, u16Caldur);
}

void MApi_XC_ADC_ScartRGB_SOG_ClampDelay(MS_U16 u16Clpdly, MS_U16 u16Caldur)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SCARTRGB_SOG_CLAMPDELAY XCArgs;
    XCArgs.u16Clpdly = u16Clpdly;
    XCArgs.u16Caldur = u16Caldur;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SCARTRGB_SOG_CLAMPDELAY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_Set_YPbPrLooseLPF_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_ADC_Set_YPbPrLooseLPF(pInstance, bEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_ADC_Set_YPbPrLooseLPF(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SET_YPBPRLOOSELPF XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SET_YPBPRLOOSELPF, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_Set_SOGBW_U2(void* pInstance, MS_U16 u16Value)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_ADC_Set_SOGBW(pInstance, u16Value);
    _XC_RETURN(pInstance);
}

void MApi_XC_ADC_Set_SOGBW(MS_U16 u16Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SET_SOGBW XCArgs;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SET_SOGBW, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ADC_SetClampDuration_U2(void* pInstance, MS_U16 u16Value)
{
    MDrv_XC_ADC_SetClampDuration(pInstance, u16Value);
}

void MApi_XC_ADC_SetClampDuration(MS_U16 u16Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_ADC_SETCLAMPDURATION XCArgs;
    XCArgs.u16Value = u16Value;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SETCLAMPDURATION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


MS_BOOL MApi_XC_ADC_EnableHWCalibration_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_ADC_EnableHWCalibration(pInstance, bEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
}

MS_BOOL MApi_XC_ADC_EnableHWCalibration(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_ADC_ENABLEHWCALIBRATION XCArgs;
    XCArgs.bEnable = bEnable;
	XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_ENABLEHWCALIBRATION, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_ADC_SetIdacCurrentMode_U2(void* pInstance, ADC_DacCurrentMode_t eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bReturn = FALSE;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_ADC_SetIdacCurrentMode(pInstance, eMode);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return  bReturn;
}

MS_BOOL MApi_XC_ADC_SetIdacCurrentMode(ADC_DacCurrentMode_t eMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_ADC_SETIDACCURRENTMODE XCArgs;
    XCArgs.eMode = eMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_SET_IDACCURRENTMODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

ADC_DacCurrentMode_t MApi_XC_ADC_GetIdacCurrentMode_U2(void* pInstance)
{
    ADC_DacCurrentMode_t eReturnMode = E_ADC_IDAC_FULL_MODE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    eReturnMode = MDrv_XC_ADC_GetIdacCurrentMode(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return eReturnMode;
}

ADC_DacCurrentMode_t MApi_XC_ADC_GetIdacCurrentMode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_ADC_GETIDACCURRENTMODE XCArgs;
    XCArgs.eReturnMode = E_ADC_IDAC_FULL_MODE;

    if(UtopiaIoctl(pu32XCInst, E_XC_ADC_CMD_GET_IDACCURRENTMODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.eReturnMode;
    }
}

#undef _API_XC_ADC_C_
#endif  // _API_XC_ADC_C_
