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
/// file    Mdrv_adc.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MDRV_ADC_C

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
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "apiXC.h"
#include "mvideo_context.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drvXC_ADC_Internal.h"
#include "mhal_adc.h"
#include "apiXC_PCMonitor.h"
#include "drv_sc_ip.h"
#include "drv_sc_scaling.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define TRACE_ADC_INPUT(x) //x
#define MAX_ADCSOUCE_NUM 2

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
//  Global Functions
//-------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void MDrv_XC_ADC_set_cvbs_out(void *pInstance, E_ADC_CVBSOUT_TYPE e_cvbs_out_type)
{
    Hal_ADC_set_cvbs_out(pInstance, e_cvbs_out_type);
}

MS_BOOL MDrv_XC_ADC_Is_cvbs_out_enabled(void *pInstance, E_DEST_TYPE enOutputType)
{
    if (enOutputType == OUTPUT_CVBS1)
    {
        return Hal_ADC_is_cvbs_out_enabled(pInstance, OUTPUT_PORT_CVBS1);
    }
    else if (enOutputType == OUTPUT_CVBS2)
    {
        return Hal_ADC_is_cvbs_out_enabled(pInstance, OUTPUT_PORT_CVBS2);
    }
    else
    {
        return FALSE;
    }

}

MS_BOOL MDrv_XC_ADC_use_hardware_auto_offset(void *pInstance, INPUT_SOURCE_TYPE_t eSource)
{
    if ( IsSrcTypeYPbPr(eSource) )
    {
        return ADC_HARDWARE_AUTOOFFSET_YPBPR;
    }
    else if ( IsSrcTypeVga(eSource) )
    {
        return ADC_HARDWARE_AUTOOFFSET_RGB;
    }
    else if ( IsSrcTypeScart(eSource) )
    {
        return ADC_HARDWARE_AUTOOFFSET_SCARTRGB;
    }
    else
    {
        return ENABLE;
    }

}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
//******************************************************************************
/// set offset to HW
/// @param  pstADCSetting \b IN: offset
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
void MDrv_XC_ADC_SetOffsetSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    Hal_ADC_SetOffsetR(pInstance, pstADCSetting->u16RedOffset);
    Hal_ADC_SetOffsetG(pInstance, pstADCSetting->u16GreenOffset);
    Hal_ADC_SetOffsetB(pInstance, pstADCSetting->u16BlueOffset);
}

//******************************************************************************
/// set gain to HW
/// @param  pstADCSetting \b IN: gain
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
void MDrv_XC_ADC_SetGainSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    Hal_ADC_SetGainR(pInstance, pstADCSetting->u16RedGain);
    Hal_ADC_SetGainG(pInstance, pstADCSetting->u16GreenGain);
    Hal_ADC_SetGainB(pInstance, pstADCSetting->u16BlueGain);
}

//******************************************************************************
/// get offset from HW
/// @param  pstADCSetting \b OUT: offset
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_XC_ADC_GetOffsetSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    if (NULL == pstADCSetting)
    {
        return FALSE;
    }
    pstADCSetting->u16RedOffset = Hal_ADC_GetOffsetR(pInstance);
    pstADCSetting->u16GreenOffset = Hal_ADC_GetOffsetG(pInstance);
    pstADCSetting->u16BlueOffset = Hal_ADC_GetOffsetB(pInstance);
    return TRUE;
}

//******************************************************************************
/// get gain from HW
/// @param  pstADCSetting \b OUT: gain
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_XC_ADC_GetGainSetting(void *pInstance, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    if (NULL == pstADCSetting)
    {
        return FALSE;
    }
    pstADCSetting->u16RedGain= Hal_ADC_GetGainR(pInstance);
    pstADCSetting->u16GreenGain = Hal_ADC_GetGainG(pInstance);
    pstADCSetting->u16BlueGain = Hal_ADC_GetGainB(pInstance);
    return TRUE;
}

void MDrv_XC_ADC_Source_Calibrate(void *pInstance, ADC_INPUTSOURCE_TYPE enADCInput)
{
    Hal_ADC_Set_Source_Calibration(pInstance, enADCInput);
}

//******************************************************************************
/// Initial with middle values of max gain and max offset
/// before external calibration
/// @param  enInputSrc  \b IN:input source
/// @param  pstInitialGainOffset \b OUT: gain offset
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_XC_ADC_InitExternalCalibration(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset)
{
    MS_BOOL bRet = FALSE;
    ADC_INPUTSOURCE_TYPE enAdcSource = ADC_INPUTSOURCE_UNKNOW;

    if ( IsSrcTypeYPbPr(enInputSrc) )
    {
        enAdcSource = ADC_INPUTSOURCE_ONLY_YPBPR;
        bRet = TRUE;
    }
    else if ( IsSrcTypeVga(enInputSrc) )
    {
        enAdcSource = ADC_INPUTSOURCE_ONLY_RGB;
        bRet = TRUE;
    }
    else if ( IsSrcTypeScart(enInputSrc) )
    {
        enAdcSource = ADC_INPUTSOURCE_ONLY_SCART;
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    if (bRet)
    {
        bRet = MDrv_ADC_GetInitCalibrationGain(pInstance, enInputSrc, pstInitialGainOffset);
        if (bRet)
        {
            bRet = MDrv_ADC_GetInitCalibrationOffset(pInstance, enInputSrc, pstInitialGainOffset);
            if (bRet)
            {
                _XC_ENTRY(pInstance);
                bRet = Hal_ADC_InitExternalCalibration(pInstance, enAdcSource);
                if (bRet)
                {
                    Hal_ADC_gain_setting(pInstance, (XC_AdcGainOffsetSetting*)pstInitialGainOffset);
                    Hal_ADC_offset_setting(pInstance, (XC_AdcGainOffsetSetting*)pstInitialGainOffset);
                }
                _XC_RETURN(pInstance);
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Offset fail!!\n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Gain fail!!\n");
        }
    }
    return bRet;
}

//******************************************************************************
/// Initial with fixed offset before external calibrating gain
/// @param  enInputSrc  \b IN:input source
/// @param  pstInitialGainOffset \b OUT: gain offset
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_XC_ADC_InitExternalCalibrationWithFixedOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset)
{
    MS_BOOL bRet = FALSE;
    ADC_INPUTSOURCE_TYPE enAdcSource = ADC_INPUTSOURCE_UNKNOW;

    if (IsSrcTypeYPbPr(enInputSrc))
    {
        enAdcSource = ADC_INPUTSOURCE_ONLY_YPBPR;
        bRet = TRUE;
    }
    else if ( IsSrcTypeVga(enInputSrc) )
    {
        enAdcSource = ADC_INPUTSOURCE_ONLY_RGB;
        bRet = TRUE;
    }
    else if ( IsSrcTypeScart(enInputSrc) )
    {
        enAdcSource = ADC_INPUTSOURCE_ONLY_SCART;
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    if (bRet)
    {
        bRet = MDrv_ADC_GetInitCalibrationGain(pInstance, enInputSrc, pstInitialGainOffset);
        if (bRet)
        {
            bRet = MDrv_ADC_GetFixedOffset(pInstance, enInputSrc, pstInitialGainOffset);
            if (bRet)
            {
                _XC_ENTRY(pInstance);
                bRet = Hal_ADC_InitExternalCalibration(pInstance, enAdcSource);
                if (bRet)
                {
                    Hal_ADC_gain_setting(pInstance, (XC_AdcGainOffsetSetting*)pstInitialGainOffset);
                    Hal_ADC_offset_setting(pInstance, (XC_AdcGainOffsetSetting*)pstInitialGainOffset);
                }
                _XC_RETURN(pInstance);
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Offset fail!!\n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Gain fail!!\n");
        }
    }
    return bRet;
}

//******************************************************************************
/// Initial before internal calibration
/// @param  enInputSrc  \b IN:input source
/// @param  pstInitialGainOffset \b OUT: gain offset
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_XC_ADC_InitInternalCalibration(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset)
{
    MS_BOOL bRet = FALSE;
    ADC_INPUTSOURCE_TYPE enAdcSrc = ADC_INPUTSOURCE_UNKNOW;

    if (IsSrcTypeYPbPr(enInputSrc))
    {
        enAdcSrc = ADC_INPUTSOURCE_ONLY_YPBPR;
        bRet = TRUE;
    }
    else if (IsSrcTypeVga(enInputSrc))
    {
        enAdcSrc = ADC_INPUTSOURCE_ONLY_RGB;
        bRet = TRUE;
    }
    else if (IsSrcTypeScart(enInputSrc))
    {
        enAdcSrc = ADC_INPUTSOURCE_ONLY_SCART;
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    if (bRet)
    {
        bRet = MDrv_ADC_GetInitCalibrationGain(pInstance, enInputSrc, pstInitialGainOffset);
        if (bRet)
        {
            bRet = MDrv_ADC_GetInitCalibrationOffset(pInstance, enInputSrc, pstInitialGainOffset);
            if (bRet)
            {
                _XC_ENTRY(pInstance);
                Hal_ADC_InitInternalCalibration(pInstance, enAdcSrc, MAIN_WINDOW);
                Hal_ADC_disable_csc(pInstance, MAIN_WINDOW);
                _XC_RETURN(pInstance);
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Offset fail!!\n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Gain fail!!\n");
        }
    }
    return bRet;
}

//******************************************************************************
/// Initial before internal calibrate gain
/// @param  enInputSrc  \b IN:input source
/// @param  pstInitialGainOffset \b OUT: gain offset
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_XC_ADC_InitInternalCalibrationWithFixedOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGainOffset, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    ADC_INPUTSOURCE_TYPE enAdcSrc = ADC_INPUTSOURCE_UNKNOW;

    if (IsSrcTypeYPbPr(enInputSrc))
    {
        enAdcSrc = ADC_INPUTSOURCE_ONLY_YPBPR;
        bRet = TRUE;
    }
    else if (IsSrcTypeVga(enInputSrc))
    {
        enAdcSrc = ADC_INPUTSOURCE_ONLY_RGB;
        bRet = TRUE;
    }
    else if (IsSrcTypeScart(enInputSrc))
    {
        enAdcSrc = ADC_INPUTSOURCE_ONLY_SCART;
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    if (bRet)
    {
        bRet = MDrv_ADC_GetInitCalibrationGain(pInstance, enInputSrc, pstInitialGainOffset);
        if (bRet)
        {
            bRet = MDrv_ADC_GetFixedOffset(pInstance, enInputSrc, pstInitialGainOffset);
            if (bRet)
            {
                _XC_ENTRY(pInstance);
                Hal_ADC_InitInternalCalibration(pInstance, enAdcSrc, eWindow);
                Hal_ADC_disable_csc(pInstance, eWindow);
                _XC_RETURN(pInstance);
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Offset fail!!\n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Get Initial Calibration Gain fail!!\n");
        }
    }
    return bRet;
}

//******************************************************************************
/// Initial gain before internal calibration
/// @param enInputSrc         \b IN: input source
/// @param pstInitialGain   \b OUT: gain value struct
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_ADC_GetInitCalibrationGain(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialGain)
{
    MS_BOOL bRet = FALSE;

    if (IsSrcTypeVga(enInputSrc) || IsSrcTypeYPbPr(enInputSrc) || IsSrcTypeScart(enInputSrc))
    {
        // Initial gain
        pstInitialGain->u16RedGain = (1 << ADC_GAIN_BIT_CNT) >> 1;
        pstInitialGain->u16GreenGain = (1 << ADC_GAIN_BIT_CNT) >> 1;
        pstInitialGain->u16BlueGain = (1 << ADC_GAIN_BIT_CNT) >> 1;
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    return bRet;
}

//******************************************************************************
/// Initial offset before internal calibration
/// @param enInputSrc         \b IN: input source
/// @param pstInitialOffset   \b OUT: offset value struct
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_ADC_GetInitCalibrationOffset(void *pInstance, INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialOffset)
{
    MS_BOOL bRet = FALSE;

    if (IsSrcTypeVga(enInputSrc) || IsSrcTypeYPbPr(enInputSrc) || IsSrcTypeScart(enInputSrc))
    {
        // Initial offset
        pstInitialOffset->u16RedOffset = (1 << ADC_OFFSET_BIT_CNT) >> 1;
        pstInitialOffset->u16GreenOffset = (1 << ADC_OFFSET_BIT_CNT) >> 1;
        pstInitialOffset->u16BlueOffset = (1 << ADC_OFFSET_BIT_CNT) >> 1;
        bRet = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    return bRet;
}

void MApi_XC_ADC_ExitExternalCalibration(void *pInstance, INPUT_SOURCE_TYPE_t eSource, APIXC_AdcGainOffsetSetting* InitialGainOffset)
{

    ADC_INPUTSOURCE_TYPE eADCSource = ADC_INPUTSOURCE_UNKNOW;

    if ( IsSrcTypeYPbPr(eSource) )
    {
        eADCSource = ADC_INPUTSOURCE_ONLY_YPBPR;
    }
    else if ( IsSrcTypeVga(eSource) )
    {
        eADCSource = ADC_INPUTSOURCE_ONLY_RGB;
    }
    else if ( IsSrcTypeScart(eSource) )
    {
        eADCSource = ADC_INPUTSOURCE_ONLY_SCART;
    }
    else
    {
        // Undefined
    }

    Hal_ADC_ExitExternalCalibration(pInstance, eADCSource,(XC_AdcGainOffsetSetting*)InitialGainOffset);

}

//******************************************************************************
/// Generate ADC clock
/// @param clocktype \b IN Clock type need to generate
//******************************************************************************
void MApi_XC_ADC_GenClock(void *pInstance, ADC_Gen_Clock_Type clocktype)
{

    _XC_ENTRY(pInstance);
    Hal_ADC_clk_gen_setting(pInstance, clocktype);
    _XC_RETURN(pInstance);
}

//******************************************************************************
/// Generate Internal voltage in ADC
/// @param InternalVoltage \b IN Voltage type will be generate
//******************************************************************************
void MApi_XC_ADC_SetInternalVoltage(void *pInstance, ADC_Internal_Voltage InternalVoltage)
{
    _XC_ENTRY(pInstance);
    Hal_ADC_dtop_internaldc_setting(pInstance, InternalVoltage);
    _XC_RETURN(pInstance);
    MsOS_DelayTask(50);  //** must delay n Vsync
}

//******************************************************************************
/// Get ADC Default gain / offset by referring input structure.
/// @param eSource \b IN The input source
/// @param pstADCSetting \b IN gain / offset need to be set
//******************************************************************************
void MDrv_XC_ADC_GetDefaultGainOffset(void *pInstance, INPUT_SOURCE_TYPE_t eSource , APIXC_AdcGainOffsetSetting* pstADCSetting)
{
    if ( IsSrcTypeYPbPr(eSource) )
    {
        pstADCSetting->u16RedGain = ADC_YPBPR_DEFAULT_GAIN_R;
        pstADCSetting->u16GreenGain = ADC_YPBPR_DEFAULT_GAIN_G;
        pstADCSetting->u16BlueGain = ADC_YPBPR_DEFAULT_GAIN_B;
        pstADCSetting->u16RedOffset = ADC_YPBPR_DEFAULT_OFFSET_R;
        pstADCSetting->u16GreenOffset= ADC_YPBPR_DEFAULT_OFFSET_G;
        pstADCSetting->u16BlueOffset= ADC_YPBPR_DEFAULT_OFFSET_B;
    }
    else if ( IsSrcTypeVga(eSource) )
    {
        pstADCSetting->u16RedGain = ADC_VGA_DEFAULT_GAIN_R;
        pstADCSetting->u16GreenGain = ADC_VGA_DEFAULT_GAIN_G;
        pstADCSetting->u16BlueGain = ADC_VGA_DEFAULT_GAIN_B;
        pstADCSetting->u16RedOffset = ADC_VGA_DEFAULT_OFFSET_R;
        pstADCSetting->u16GreenOffset= ADC_VGA_DEFAULT_OFFSET_G;
        pstADCSetting->u16BlueOffset= ADC_VGA_DEFAULT_OFFSET_B;
    }
    else if ( IsSrcTypeScart(eSource) )
    {
        pstADCSetting->u16RedGain = ADC_SCART_DEFAULT_GAIN_R;
        pstADCSetting->u16GreenGain = ADC_SCART_DEFAULT_GAIN_G;
        pstADCSetting->u16BlueGain = ADC_SCART_DEFAULT_GAIN_B;
        pstADCSetting->u16RedOffset = ADC_SCART_DEFAULT_OFFSET_R;
        pstADCSetting->u16GreenOffset= ADC_SCART_DEFAULT_OFFSET_G;
        pstADCSetting->u16BlueOffset= ADC_SCART_DEFAULT_OFFSET_B;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", eSource);
    }
}

//******************************************************************************
/// get HW center Gain value
/// @param
/// @return HW default Gain value
//******************************************************************************
MS_U16 MDrv_XC_ADC_GetCenterGain(void *pInstance)
{
    return ADC_CENTER_GAIN;
}

//******************************************************************************
/// get HW center Gain value
/// @param
/// @return HW default Gain value
//******************************************************************************
MS_U16 MDrv_XC_ADC_GetCenterOffset(void *pInstance)
{
    return ADC_CENTER_OFFSET;
}

//******************************************************************************
/// get maximal value of offset
/// @param
/// @return maximal value of offset
//******************************************************************************
MS_U16 MDrv_XC_ADC_GetMaximalOffsetValue(void *pInstance)
{
    MS_U8 u8AdcOffsetBitCnt = ADC_OFFSET_BIT_CNT;

    return  (1 << u8AdcOffsetBitCnt ) - 1;
}

//******************************************************************************
/// get maximal value of gain
/// @param
/// @return maximal value of gain
//******************************************************************************
MS_U16 MDrv_XC_ADC_GetMaximalGainValue(void *pInstance)
{
    MS_U8 u8AdcGainBitCnt = ADC_GAIN_BIT_CNT;

    return  (1 << u8AdcGainBitCnt ) - 1;
}

//******************************************************************************
/// Set Gain value
/// @param u8color \b IN The channel type need to be set
/// @param u16value \b IN The value need to be set.
//******************************************************************************
void MDrv_XC_ADC_SetGain(void *pInstance, MS_U8 u8Color, MS_U16 u16Value)
{
    _XC_ENTRY(pInstance);
    switch(u8Color)
    {
        case ADC_CHR:
            Hal_ADC_SetGainR(pInstance, u16Value);
            break;
        case ADC_CHG:
            Hal_ADC_SetGainG(pInstance, u16Value);
            break;
        case ADC_CHB:
            Hal_ADC_SetGainB(pInstance, u16Value);
            break;
        case ADC_ALL:
        default:
            Hal_ADC_SetGainR(pInstance, u16Value);
            Hal_ADC_SetGainG(pInstance, u16Value);
            Hal_ADC_SetGainB(pInstance, u16Value);
            break;
    }
    _XC_RETURN(pInstance);
}

//******************************************************************************
/// Set offset value
/// @param u8color \b IN The channel type need to be set
/// @param u16Value \b IN The value need to be set.
//******************************************************************************
void MDrv_XC_ADC_SetOffset(void *pInstance, MS_U8 u8Color, MS_U16 u16Value)
{
    _XC_ENTRY(pInstance);
    switch(u8Color)
    {
        case ADC_CHR:
            Hal_ADC_SetOffsetR(pInstance, u16Value);
            break;
        case ADC_CHG:
            Hal_ADC_SetOffsetG(pInstance, u16Value);
            break;
        case ADC_CHB:
            Hal_ADC_SetOffsetB(pInstance, u16Value);
            break;
        case ADC_ALL:
        default:
            Hal_ADC_SetOffsetR(pInstance, u16Value);
            Hal_ADC_SetOffsetG(pInstance, u16Value);
            Hal_ADC_SetOffsetB(pInstance, u16Value);
            break;
    }
    _XC_RETURN(pInstance);
}

//******************************************************************************
/// Enable auto function (auto gain/offset) for Component or RGB source
/// @param bEnable \b IN Enable or Disable
/// @param bIsYPbPrFlag \b IN Is current source Ypbpr or not
/// @param bIsAutoSWMode \b IN Is auto software?
//******************************************************************************
void MApi_XC_ADC_AutoSetting(void *pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag, MS_BOOL bIsAutoSWMode)
{
    _XC_ENTRY(pInstance);
    Hal_ADC_dtop_calibration_target_setting(pInstance, bIsYPbPrFlag);
    Hal_ADC_dtop_sw_mode_setting(pInstance, bEnable, bIsAutoSWMode);
    _XC_RETURN(pInstance);
}

void MDrv_XC_ADC_reset(void *pInstance, MS_U16 u16Resete)
{
    Hal_ADC_reset(pInstance, u16Resete);
}

void MDrv_XC_ADC_Set_Source(void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType, E_MUX_INPUTPORT* enInputPortType, MS_U8 u8PortCount)
{
    Hal_ADC_Set_Source(pInstance, enADC_SourceType, enInputPortType, u8PortCount);
}

/******************************************************************************/
/// Set ADC Sample clock
/// @param u16Value \b IN value need to be set.
/******************************************************************************/
void MDrv_XC_ADC_SetPcClock(void *pInstance, MS_U16 u16Value )
{
    _XC_ENTRY(pInstance);
    Hal_ADC_dtop_clk_setting(pInstance, u16Value);
    _XC_RETURN(pInstance);
}

void MDrv_XC_ADC_hpolarity_setting(void *pInstance, MS_BOOL bHightActive )
{
    Hal_ADC_hpolarity_setting(pInstance, bHightActive);
}

/******************************************************************************/
/// Set ADC Phase Ex
/// @param u16Value \b IN The adc phase need to be update
/******************************************************************************/
void MDrv_XC_ADC_SetPhaseWithVSync(void *pInstance, MS_U16 u16Value)
{
    _XC_ENTRY(pInstance);
	if (MDrv_SC_GetSkipWaitVsync(pInstance,MAIN_WINDOW) == FALSE)
    {
    	MDrv_XC_wait_input_vsync(pInstance, 1, 50, MAIN_WINDOW);
	}
    Hal_ADC_set_phaseEx(pInstance, u16Value);
    _XC_RETURN(pInstance);
}

/******************************************************************************/
/// Set ADC Phase Ex
/// @param u16Value \b IN The adc phase need to be update
/******************************************************************************/
void MDrv_XC_ADC_SetPhaseEx(void *pInstance, MS_U16 u16Value )
{
    _XC_ENTRY(pInstance);
    Hal_ADC_set_phaseEx(pInstance, u16Value);
    _XC_RETURN(pInstance);
}

//******************************************************************************
/// Get current ADC Phase set
/// @return Current Phase
//******************************************************************************
MS_U16 MDrv_XC_ADC_GetPhaseRange(void *pInstance)
{
    return Hal_ADC_get_phase_range(pInstance);
}

/******************************************************************************/
///Get SOG level range
///@param u32Min \b OUT: min of SOG level
///@param u32Max \b OUT: max of SOG level
///@param u32Recommend_value \b OUT: recommend value
/******************************************************************************/
void MDrv_XC_ADC_GetSoGLevelRange(void *pInstance, MS_U32 *u32Min, MS_U32 *u32Max, MS_U32 *u32Recommend_value)
{
    Hal_ADC_get_SoG_LevelRange(pInstance, u32Min, u32Max, u32Recommend_value);
}

/******************************************************************************/
///Set SOG level
///@param u32Min \b IN: set SOG level
/******************************************************************************/
void MDrv_XC_ADC_SetSoGLevel(void *pInstance, MS_U32 u32Value)
{
    Hal_ADC_set_SoG_Level(pInstance, u32Value);
}

/******************************************************************************/
///Set SOG level
/******************************************************************************/
void MDrv_XC_ADC_SetSoGCal(void *pInstance)
{
    Hal_ADC_set_SoG_Calibration(pInstance);
}

//******************************************************************************
/// Get current ADC Phase set
/// @return Current Phase
//******************************************************************************
MS_U8 MDrv_XC_ADC_GetPhase( void *pInstance )
{
    MS_U8 u8ADC_phase;
    _XC_ENTRY(pInstance);
    u8ADC_phase = Hal_ADC_get_phase(pInstance);
    _XC_RETURN(pInstance);
    return u8ADC_phase;//Hal_ADC_get_phase();
}

MS_U16 MDrv_XC_ADC_GetPhaseEx( void *pInstance )
{
    MS_U16 u16ADC_phase;
    _XC_ENTRY(pInstance);
    u16ADC_phase = Hal_ADC_get_phaseEx(pInstance);
    _XC_RETURN(pInstance);
    return u16ADC_phase;
}

void MDrv_XC_ADC_clamp_placement_setting(void *pInstance, MS_U16 u16InputClockMHz)
{
    Hal_ADC_clamp_placement_setting(pInstance, u16InputClockMHz);
}

//******************************************************************************
/// Set V clamping type
/// @param type \b IN The Vclampling type
//******************************************************************************
void MApi_XC_ADC_SetVClampLevel(void *pInstance, ADC_VClamp_Level_Type type)
{
    Hal_XC_ADC_Set_VClamp_level(pInstance, type);
}


/******************************************************************************/
///This function will set ADC registers for different port
///@param enInputSourceType \b IN: source type
///@param u8InputClock \b IN: pixel clock
/******************************************************************************/
void MDrv_XC_ADC_vco_ctrl (void *pInstance, MS_BOOL bIsDVIPort, MS_U16 u16InputClock)
{
    Hal_ADC_set_vco_ctrl(pInstance, bIsDVIPort, u16InputClock);
}

//******************************************************************************
/// Turn off ADC
//******************************************************************************
void MDrv_XC_ADC_PowerOff(void *pInstance)
{
    Hal_ADC_poweroff(pInstance);
}

void MDrv_XC_ADC_poweron_source (void *pInstance, ADC_INPUTSOURCE_TYPE enADC_SourceType)
{
    Hal_XC_ADC_poweron_source(pInstance, enADC_SourceType);
}

void MDrv_ADC_SourceSwitch(void *pInstance, MS_BOOL bSwitch)
{
   Hal_ADC_SourceSwitch(pInstance, bSwitch);
}

void MDrv_XC_ADC_Reload(void *pInstance, INPUT_SOURCE_TYPE_t enSourceType, MS_U16 u16PixelClockPerSecond, MS_U16 u16HorizontalTotal, MS_U16 u16SamplingRatio)
{
    if (IsSrcTypeVga(enSourceType) || IsSrcTypeYPbPr(enSourceType) || IsSrcTypeScart(enSourceType) )
    {
	    Hal_ADC_set_mode(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, enSourceType), u16PixelClockPerSecond, u16HorizontalTotal, u16SamplingRatio);
    }
}

void MDrv_XC_ADC_BackupAdcReg(void *pInstance, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_ADC_auto_adc_backup(pInstance, eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_ADC_RestoreAdcReg(void *pInstance, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_ADC_auto_adc_restore(pInstance, eWindow);
    _XC_RETURN(pInstance);
}

MS_BOOL MDrv_XC_ADC_IsScartRGB(void *pInstance)
{
    MS_BOOL bRGB = 0;
    bRGB = Hal_ADC_is_scart_rgb(pInstance);

    return bRGB;
}

MS_U16 MDrv_XC_ADC_GetPcClock(void *pInstance)
{
    return Hal_ADC_get_clk(pInstance);
}

void MDrv_XC_ADC_SetRGB_PIPE_Delay(void *pInstance, MS_U8 u8Value)
{
    Hal_ADC_set_RGB_PIPE_Delay(pInstance, u8Value);
}

void MDrv_XC_ADC_ScartRGB_SOG_ClampDelay(void *pInstance, MS_U16 u16Clpdly, MS_U16 u16Caldur)
{
    Hal_ADC_set_ScartRGB_SOG_ClampDelay(pInstance, u16Clpdly, u16Caldur);
}

void MDrv_XC_ADC_Set_YPbPrLooseLPF(void *pInstance, MS_BOOL bEnable)
{
    Hal_ADC_set_YPbPrLooseLPF(pInstance, bEnable);
}

void MDrv_XC_ADC_Set_SOGBW(void *pInstance, MS_U16 u16value)
{
    Hal_ADC_Set_SOGBW(pInstance, u16value);
}

//******************************************************************************
/// Convert input source type to ADC input source type
/// @param enInputSource \b IN The source array.
//******************************************************************************
ADC_INPUTSOURCE_TYPE MDrv_XC_ADC_ConvertSrcToADCSrc(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    if ( IsSrcTypeVga(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_RGB;
    }
    else if ( IsSrcTypeYPbPr(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_YPBPR;
    }
    else if ( IsSrcTypeATV(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_ATV;
    }
    else if ( IsSrcTypeSV(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_SVIDEO;
    }
    else if ( IsSrcTypeAV(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_CVBS;
    }
    else if ( IsSrcTypeScart(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_SCART;
    }
    else if ( IsSrcTypeHDMI(enInputSourceType) )
    {
        return ADC_INPUTSOURCE_ONLY_DVI;
    }
    else if ( IsSrcTypeStorage(enInputSourceType) || IsSrcTypeDTV(enInputSourceType))
    {
        return ADC_INPUTSOURCE_ONLY_MVOP;
    }
    else
    {
        return ADC_INPUTSOURCE_UNKNOW;
    }
}

void MDrv_XC_ADC_SetClampDuration(void *pInstance, MS_U16 u16Value)
{
    Hal_ADC_dtop_vClampDuration_setting(pInstance, u16Value);
}

void MDrv_XC_ADC_EnableHWCalibration(void *pInstance, MS_BOOL bEnable)
{
    Hal_ADC_Enable_HWCalibration(pInstance, bEnable);
}
MS_BOOL MDrv_XC_ADC_CheckHWCalibrationSupport(void *pInstance)
{
    return ADC_HARDWARE_AUTOGAIN_SUPPORTED;
}

void MDrv_ADC_ISOG_Enable(void *pInstance, MS_BOOL bEnable)
{
    Hal_ADC_ISOG_Enable(pInstance, bEnable);
}

void MDrv_ADC_ISOG_SetDetectMode(void *pInstance, ADC_iSogDetectMode stMode)
{
    if(Hal_ADC_ISOG_CheckEnabled(pInstance))
    {
        switch(stMode)
        {
            default:
            case E_ADC_ISOG_NORMAL_MODE:
                Hal_ADC_ISOG_SetResetWidth(pInstance, 0);
                break;
            case E_ADC_ISOG_STANDBY_MODE:
                Hal_ADC_ISOG_SetResetWidth(pInstance, 1);
                break;
        }
    }
}

MS_BOOL MDrv_XC_ADC_GetFixedGain(void *pInstance, const INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstAdcFixedGain)
{
    MS_BOOL bResult = FALSE;
    if ((enInputSrc >= INPUT_SOURCE_NUM) || (NULL == pstAdcFixedGain))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Error parameter InputSource:%u, pointer null:%u!!\n", enInputSrc, (NULL == pstAdcFixedGain));
        return FALSE;
    }

#if ADC_HARDWARE_AUTOGAIN_SUPPORTED
    if (IsSrcTypeYPbPr(enInputSrc))
    {
        pstAdcFixedGain->u16RedGain = ADC_YPBPR_FIXED_GAIN_R;
        pstAdcFixedGain->u16GreenGain = ADC_YPBPR_FIXED_GAIN_G;
        pstAdcFixedGain->u16BlueGain = ADC_YPBPR_FIXED_GAIN_B;
        bResult = TRUE;
    }
    else if (IsSrcTypeVga(enInputSrc))
    {
        pstAdcFixedGain->u16RedGain = ADC_VGA_FIXED_GAIN_R;
        pstAdcFixedGain->u16GreenGain = ADC_VGA_FIXED_GAIN_G;
        pstAdcFixedGain->u16BlueGain = ADC_VGA_FIXED_GAIN_B;
        bResult = TRUE;
    }
    else if (IsSrcTypeScart(enInputSrc))
    {
        pstAdcFixedGain->u16RedGain = ADC_SCART_FIXED_GAIN_R;
        pstAdcFixedGain->u16GreenGain = ADC_SCART_FIXED_GAIN_G;
        pstAdcFixedGain->u16BlueGain = ADC_SCART_FIXED_GAIN_B;
        bResult = TRUE;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bResult = FALSE;
    }
#else
    UNUSED(pstAdcFixedGain);
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Unsupport ADC HW calibration on this chip!!\n");
    bResult = FALSE;
#endif

    return bResult;
}

//******************************************************************************
/// Get fixed offset
/// @param enInputSrc         \b IN: input source
/// @param pstInitialOffset   \b OUT: offset value struct
/// @return MS_BOOL \b:TRUE succeed, FALSE fail
//******************************************************************************
MS_BOOL MDrv_ADC_GetFixedOffset(void *pInstance, const INPUT_SOURCE_TYPE_t enInputSrc, APIXC_AdcGainOffsetSetting* pstInitialOffset)
{
    MS_BOOL bRet = FALSE;

    if ((enInputSrc >= INPUT_SOURCE_NUM) || (NULL == pstInitialOffset))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Error parameter InputSource:%u, pointer null:%u!!\n", enInputSrc, (NULL == pstInitialOffset));
        return FALSE;
    }

    if (IsSrcTypeVga(enInputSrc))
    {
#if ADC_HARDWARE_AUTOOFFSET_RGB
        // Initial offset for VGA
        pstInitialOffset->u16RedOffset = ADC_VGA_FIXED_OFFSET_R;
        pstInitialOffset->u16GreenOffset = ADC_VGA_FIXED_OFFSET_G;
        pstInitialOffset->u16BlueOffset = ADC_VGA_FIXED_OFFSET_B;
        bRet = TRUE;
#endif
    }
    else if (IsSrcTypeYPbPr(enInputSrc))
    {
#if ADC_HARDWARE_AUTOOFFSET_YPBPR
        // Initial offset for YPbPr
        pstInitialOffset->u16RedOffset = ADC_YPBPR_FIXED_OFFSET_R;
        pstInitialOffset->u16GreenOffset = ADC_YPBPR_FIXED_OFFSET_G;
        pstInitialOffset->u16BlueOffset = ADC_YPBPR_FIXED_OFFSET_B;
        bRet = TRUE;
#endif
    }
    else if (IsSrcTypeScart(enInputSrc))
    {
#if ADC_HARDWARE_AUTOOFFSET_SCARTRGB
        // Initial offset for Scart
        pstInitialOffset->u16RedOffset = ADC_SCART_FIXED_OFFSET_R;
        pstInitialOffset->u16GreenOffset = ADC_SCART_FIXED_OFFSET_G;
        pstInitialOffset->u16BlueOffset = ADC_SCART_FIXED_OFFSET_B;
        bRet = TRUE;
#endif
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY, "Wrong input source %u!!\n", enInputSrc);
        bRet = FALSE;
    }

    return bRet;
}



//******************************************************************************
/// Set ADC for specific input source
/// @param enInputSource \b IN The source array. It is possible there are multi-active sources at the same time (For ex. RGB source with ATV monitor out).
/// @param u8InputSourceCount \b IN The number of active source.
//******************************************************************************
void MDrv_XC_ADC_SetInputSource(void *pInstance, INPUT_SOURCE_TYPE_t*  enInputSource, MS_U8 u8InputSourceCount)
{
    ADC_INPUTSOURCE_TYPE adc_source_index[MAX_ADCSOUCE_NUM] = {ADC_INPUTSOURCE_UNKNOW, ADC_INPUTSOURCE_UNKNOW};
    E_MUX_INPUTPORT enPorts[3] = {INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT, INPUT_PORT_NONE_PORT};
    MS_U8 u8Port_count = 0;
    MS_BOOL isMultipleSource = ( u8InputSourceCount > 1 );
    MS_U8 i;
    MS_BOOL bIsYPbPrSource = FALSE;


    TRACE_ADC_INPUT(
        for(i=0; i<u8InputSourceCount; i++)
        {
            printf("(src %d)enInputSource(%s), u8InputSourceCount(%d)\n",
                    i,
                    enInputSource[i] == INPUT_SOURCE_VGA?"VGA" :
                    enInputSource[i] == INPUT_SOURCE_TV? "TV"  :
                    enInputSource[i] == INPUT_SOURCE_YPBPR? "YPBPR"  :
                    enInputSource[i] == INPUT_SOURCE_YPBPR2? "YPBPR2"  :
                    enInputSource[i] == INPUT_SOURCE_YPBPR3? "YPBPR3"  :
                    "Others Input",
                    u8InputSourceCount);
        }
    );

    if(u8InputSourceCount > MAX_ADCSOUCE_NUM)
    {
        printf("[ASSERT]MDrv_XC_ADC_SetInputSource: u8InputSourceCount(%u) shouldn't be bigger than MAX_ADCSOUCE_NUM(%u)!\n",u8InputSourceCount,MAX_ADCSOUCE_NUM);
        MS_ASSERT(0);
        return;
    }

    for(i=0;i <u8InputSourceCount; i++)
    {
        adc_source_index[i] = MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, enInputSource[i]);
        if(IsSrcTypeYPbPr(enInputSource[i]))
        {
            bIsYPbPrSource |= TRUE;
        }
    }

    // Per ADC HW designer's suggestion, only for YPbPr case, internal SOG reset signal should be configured before sync detect.
    if (bIsYPbPrSource)
    {
        MDrv_ADC_ISOG_SetDetectMode(pInstance, E_ADC_ISOG_NORMAL_MODE);
    }

    TRACE_ADC_INPUT(printf("===> %s\n", isMultipleSource?"Multi Input":"Single Input"));

    if(isMultipleSource)
    {
        // set Power for main/sub
        if(adc_source_index[0] != ADC_INPUTSOURCE_UNKNOW && adc_source_index[1] != ADC_INPUTSOURCE_UNKNOW)
        {
            MDrv_XC_ADC_poweron_source(pInstance, (ADC_INPUTSOURCE_TYPE)(adc_source_index[0]|adc_source_index[1]));
        }

        // set mux for main
        if(adc_source_index[0] != ADC_INPUTSOURCE_UNKNOW)
        {
            MDrv_XC_Mux_GetPort(pInstance, enInputSource[0] , enPorts , &u8Port_count );
            MDrv_XC_ADC_Set_Source(pInstance, (ADC_INPUTSOURCE_TYPE)(adc_source_index[0] | adc_source_index[1]), enPorts, u8Port_count);
        }
    }
    else
    {
        // set Mux & Power
        if(adc_source_index[0] != ADC_INPUTSOURCE_UNKNOW)
        {
            MDrv_XC_Mux_GetPort(pInstance, enInputSource[0] , enPorts , &u8Port_count );
            MDrv_XC_ADC_poweron_source(pInstance, adc_source_index[0]);
            MDrv_XC_ADC_Set_Source(pInstance, adc_source_index[0], enPorts, u8Port_count);
        }
    }
}

MS_BOOL MDrv_XC_ADC_SetIdacCurrentMode(void *pInstance, ADC_DacCurrentMode_t eMode)
{
    return Hal_ADC_SetIdacCurrentMode(pInstance, (MS_U16)eMode);
}

ADC_DacCurrentMode_t MDrv_XC_ADC_GetIdacCurrentMode(void *pInstance)
{
    return (ADC_DacCurrentMode_t)Hal_ADC_GetIdacCurrentMode(pInstance);
}

void MDrv_XC_ADC_SCART_RGB_setMode(void *pInstance, MS_BOOL bEnable)
{
    Hal_ADC_SCART_RGB_setMode(pInstance,bEnable);
}

#undef MDRV_ADC_C
