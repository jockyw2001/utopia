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
/// file    apiXC_Auto.c
/// @brief  Scaler API layer Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#define _APIXC_AUTO_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#if !defined(MSOS_TYPE_LINUX_KERNEL)
#include "string.h"
#else
#include <linux/string.h>
#endif
//#include <assert.h>
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "utopia.h"
#include "utopia_dapi.h"
// Driver
#include "drvXC_IOPort.h"
#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
// Api
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Auto.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "drv_sc_ip.h"
#include "ULog.h"
#include "mvideo_context.h"
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
#include "apiXC_Auto_v2.h"

#include "drv_sc_scaling.h"
#include "drv_xc_calibration.h"

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
#ifdef MSOS_TYPE_LINUX_KERNEL
#include "drvCMAPool_v2.h"
#else
#include "drvCMAPool.h"
#endif
#include "msos/linux/mdrv_cma_pool_st.h"
#include "mhal_sc.h"
#include "halCHIP.h"
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define AUTOMSG(x)          //x

#define COMPONENT_AUTO_SW_MODE  FALSE
#define AUTO_PHASE_METHOD       0       // 0 = found maximum
#define GET_RGB_REPORT          DISABLE
#define FAST_AUTO_TUNE          1
#define AUTO_GAIN_SW_MODE       0
#define AUTO_PHASE_STEP         4

#define AUTO_YUV_GAIN_STEP      1
#define AUTO_YUV_MAX_CNT        30

#define ENABLE_VGA_EIA_TIMING   0
//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define Y_AUTO_COLOR_75IRE          0   // 0: 100IRE / 1: 75IRE
#define PbPr_AUTO_COLOR_75IRE       0   // 0: 100IRE / 1: 75IRE

#if Y_AUTO_COLOR_75IRE
#define Y_MAX_VALUE                 180 // For 75IRE Y value
#define Y_MIN_VALUE                 16
#else
#define Y_MAX_VALUE                 235 // For 100IRE Y value
#define Y_MIN_VALUE                 16
#endif

#if PbPr_AUTO_COLOR_75IRE
#define PbPr_MAX_VALUE              212 // For 75IRE PbPr value
#define PbPr_MIN_VALUE              44
#else
#define PbPr_MAX_VALUE              240 // For 100IRE PbPr value
#define PbPr_MIN_VALUE              16
#endif

#define Y_AUTO_ACTIVE_RANGE         (Y_MAX_VALUE - Y_MIN_VALUE)
#define PbPr_AUTO_ACTIVE_RANGE      (PbPr_MAX_VALUE - PbPr_MIN_VALUE)
#define Y_AUTO_MIN_VALUE            (Y_MIN_VALUE - 16)
#define PbPr_AUTO_MIN_VALUE         (PbPr_MIN_VALUE)

#define MIN_PC_AUTO_H_START            (StandardInfo->u16HorizontalStart - (StandardInfo->u16HorizontalStart * 8 / 10) )

#if ENABLE_VGA_EIA_TIMING
  #define MAX_PC_AUTO_H_START            (StandardInfo->u16HorizontalStart + (StandardInfo->u16HorizontalStart* 18 / 10) )
#else
  #define MAX_PC_AUTO_H_START            (StandardInfo->u16HorizontalStart + (StandardInfo->u16HorizontalStart* 8 / 10) )
#endif

#define MIN_PC_AUTO_V_START            (1)
#define MAX_PC_AUTO_V_START            (StandardInfo->u16VerticalStart*2 - MIN_PC_AUTO_V_START/*MIN_PC_V_START*/)

/*!
    1. RGB mode, full scale還是維持10bit 1023這個code, 所以gain 還是校正到讀到0.5/0.7*1023.
       Offset也是校正到 0這個code, RGB三個channel 都一樣.
    2. YCbCr mode, 請將Y的full scale改成10bit code 是(235-16)*4, Cb/Cr full scale 改成10bit (240-16)*4.
       所以Y gain值要調到使 digital code 看到 0.5/0.7*(235-16)*4, Cb/Cr則是0.5/0.7*(240-16)*4.
       Offset 則是將 Y 較準到64 (10bit), Cb/Cr 到512 (10bit).
*/

#define   AUTO_GAIN_RANGE_RGB_INTERNAL       731   //0.5/0.7*1023=730.7
#define   AUTO_GAIN_RANGE_Y_INTERNAL         626   //(235-16)*4 *0.5/0.7,
#define   AUTO_GAIN_RANGE_CBCR_INTERNAL      640   //(240-16)*4 *0.5/0.7,

#define   AUTO_GAIN_RANGE_RGB_EXTERNAL       1018  //Find gain setting from 1018. Than fine tune to 1023
#define   AUTO_GAIN_RANGE_Y_EXTERNAL         876   //(235-16)*4,
#define   AUTO_GAIN_RANGE_CBCR_EXTERNAL      896   //(240-16)*4,


#define   AUTO_GAIN_BONDARY           1

#define   AUTO_GAIN_MAX_RGB         (AUTO_GAIN_RANGE_RGB_INTERNAL+AUTO_GAIN_BONDARY )
#define   AUTO_GAIN_MIN_RGB         (AUTO_GAIN_RANGE_RGB_INTERNAL-AUTO_GAIN_BONDARY )

#define   AUTO_GAIN_MAX_Y           (AUTO_GAIN_RANGE_Y_INTERNAL+AUTO_GAIN_BONDARY )
#define   AUTO_GAIN_MAX_CBCR        (AUTO_GAIN_RANGE_CBCR_INTERNAL+AUTO_GAIN_BONDARY )
#define   AUTO_GAIN_MIN_Y           (AUTO_GAIN_RANGE_Y_INTERNAL-AUTO_GAIN_BONDARY )
#define   AUTO_GAIN_MIN_CBCR        (AUTO_GAIN_RANGE_CBCR_INTERNAL-AUTO_GAIN_BONDARY )

#define   AUTO_OFFSET_Y             0x40
#define   AUTO_OFFSET_CBCR          0x200

#define MAX_AUTO_POSITION_WAIT_READY_TIME 500

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

typedef struct
{
      MS_U8   L_BkAtop_01;
      MS_U8   L_BkAtop_0C;
      MS_U8   L_BkAtop_2C;
      MS_U16  L_BkAtop_05;
      MS_U16  L_BkAtop_06;
      MS_U8   L_BkAtop_1C;
      MS_U8   L_BkAtop_1F;

      MS_U16 BkDtop_01;
      MS_U16 BkDtop_02;
      MS_U8   L_BkDtop_06;
      MS_U8   H_BkChipTop_1F;
      MS_U8   L_BkIpMux_1;
      MS_U16 SC_BK1_02;
      MS_U16 SC_BK1_03;
      MS_U16 SC_BK1_04;
      MS_U16 SC_BK1_05;
      MS_U16 SC_BK1_06;
      MS_U16 SC_BK1_07;
      MS_U16 SC_BK1_0E;
      MS_U8   L_SC_BK1_21;
      MS_U8   L_SC_BK10_19;
} MS_AUTOADC_REG_BAK;

typedef enum
{
    E_XC_AUTO_GET_HSTART,
    E_XC_AUTO_GET_HEND,
    E_XC_AUTO_GET_VSTART,
    E_XC_AUTO_GET_VEND,
}XC_Auto_GetAutoPositionType;

typedef enum
{
    E_WAITING_AUTO_POSITION_READY,
    E_WAITING_AUTO_GAIN_READY,
}XC_Auto_WaitingType;

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
static void _MApi_XC_Auto_SetCalibrartinWindow(void *pInstance, MS_BOOL bEnable, SCALER_WIN scwin )
{
    //set auto range window for calibrartin
    MS_U16 Vcut,Hcut;
    MS_WINDOW_TYPE cap_win;
    memset(&cap_win, 0, sizeof(MS_WINDOW_TYPE));

    if (bEnable)
    {
        _XC_ENTRY(pInstance);
        MDrv_XC_GetCaptureWindow(pInstance, &cap_win,scwin );
        _XC_RETURN(pInstance);
        Vcut = cap_win.height/ 10;
        Hcut = cap_win.width/ 10;

        MApi_XC_AutoRangeEnable(pInstance, ENABLE, scwin);

        MApi_XC_SetAutoRangeWindow(pInstance, cap_win.y + (Vcut/2),
                                   cap_win.x + (Hcut/2),
                                   cap_win.height- Vcut,
                                   cap_win.width- Hcut,
                                   scwin
                                   );
    }
    else
    {
        MApi_XC_AutoRangeEnable(pInstance, DISABLE, scwin);
    }

    //printf("capture: %d %d %d %d \n",Vstart, Hstart, Vsize , Hsize );

}

//----------------------------------------------------------------------
//
// gain setting: 00 is maximum ~~ 0xff is minmum
//----------------------------------------------------------------------
enum {
    DOWN_TOP= 1,
    TOP_DOWN= 2,
};

MS_BOOL MApi_XC_Auto_SetValidData_U2(void* pInstance, MS_U8 Value)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stapiXC_Auto._u8ThresholdValue = Value;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
}

MS_BOOL MApi_XC_Auto_SetValidData(MS_U8 Value)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_SETVALIDDATA XCArgs;
    XCArgs.Value = Value;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_SETVALIDDATA, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

static MS_AUTOADC_TYPE _GetAverageDataFromMemory(void *pInstance, MS_WINDOW_TYPE* mem_win, SCALER_WIN eWindow)
{
    MS_IMAGE_MEM_INFO  pMemInfo;

    pMemInfo.eCal_Win_Size.u16X = mem_win->x ;
    pMemInfo.eCal_Win_Size.u16Y = mem_win->y ;
    pMemInfo.eCal_Win_Size.u16Xsize = mem_win->width;
    pMemInfo.eCal_Win_Size.u16Ysize = mem_win->height ;

    MS_PIXEL_32BIT sMemBuf[(pMemInfo.eCal_Win_Size.u16Xsize)*(pMemInfo.eCal_Win_Size.u16Ysize)];
    memset(&sMemBuf, 0 , sizeof(sMemBuf) );
    printf("x=%u, y=%u, xsize=%u, ysize=%u\n",pMemInfo.eCal_Win_Size.u16X,pMemInfo.eCal_Win_Size.u16Y,
        pMemInfo.eCal_Win_Size.u16Xsize,pMemInfo.eCal_Win_Size.u16Ysize);
    Hal_SC_set_freezeimg(pInstance, ENABLE, eWindow);

    pMemInfo.pixelAverage = MDrv_XC_GetAverageData(pInstance, sMemBuf, pMemInfo.eCal_Win_Size, eWindow);
    Hal_SC_set_freezeimg(pInstance, DISABLE, eWindow);

    return pMemInfo.pixelAverage;
}

static void _GetGainRange(void *pInstance, XC_Auto_CalibrationType type,XC_Auto_TuneType tune_type ,MS_AUTOADC_TYPE* Diff, SCALER_WIN eWindow)
{
    MS_AUTOADC_TYPE pAutoAdc_105;
    MS_AUTOADC_TYPE pAutoAdc_055;
    MS_WINDOW_TYPE mem_win;
    int ch ;

    // External YUV need gather Max/Min by differnt win
    if ( tune_type != E_XC_AUTO_TUNE_YUV_COLOR || type != E_XC_EXTERNAL_CALIBRATION )
    {
        //////////////////////////////
        // Get Max value
        if (type == E_XC_INTERNAL_CALIBRATION)
        {
            // internal have to generate internal voltage.
            mem_win.x = 5;
            mem_win.y = 5;
            mem_win.width = 10;
            mem_win.height= 10;
            MApi_XC_ADC_SetInternalVoltage(pInstance, E_ADC_Internal_0_6V);
        }
        else
        {
            mem_win.x = 640;
            mem_win.y = 360;
            mem_win.width = 10;
            mem_win.height= 10;
        }
        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);
        pAutoAdc_105 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);

        //printf("[internal 1.05V]  %04d %04d %04d \n",pAutoAdc_105.u16CH_AVG[0],pAutoAdc_105.u16CH_AVG[1],pAutoAdc_105.u16CH_AVG[2]);

        //////////////////////////////
        // Get Min value
        if (type == E_XC_INTERNAL_CALIBRATION)
        {
            // internal have to generate internal voltage.
            mem_win.x = 5;
            mem_win.y = 5;
            mem_win.width = 10;
            mem_win.height= 10;
            MApi_XC_ADC_SetInternalVoltage(pInstance, E_ADC_Internal_0_1V);  //MDrv_WriteByteMask( L_BK_ADC_DTOP(0x13) , 0x20, 0x30 ); //switch internal to 0.1V
        }
        else
        {
            mem_win.x = 5;
            mem_win.y = 5;
            mem_win.width = 10;
            mem_win.height= 10;
        }
        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);
        pAutoAdc_055 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);

        //printf("[internal 0.55V]  %04d %04d %04d \n",pAutoAdc_055.u16CH_AVG[0],pAutoAdc_055.u16CH_AVG[1],pAutoAdc_055.u16CH_AVG[2]);
        for (ch =0; ch  < 3;ch ++)
          Diff->u16CH_AVG[ch] = pAutoAdc_105.u16CH_AVG[ch] - pAutoAdc_055.u16CH_AVG[ch];
    }
    else
    {
        //////////////////////////////
        // Get Y Max
        mem_win.x = 0X200;
        mem_win.y = 0X10;
        mem_win.width = 10;
        mem_win.height= 10;

        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);

        pAutoAdc_105 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);

        //////////////////////////////
        // Get Y Min
        mem_win.x = 0X200;
        mem_win.y = 0X280;
        mem_win.width = 10;
        mem_win.height= 10;

        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);

        pAutoAdc_055 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);
        Diff->u16CH_AVG[1] = pAutoAdc_105.u16CH_AVG[1] - pAutoAdc_055.u16CH_AVG[1];

        //////////////////////////////
        // Get Pb Max
        mem_win.x = 0X200;
        mem_win.y = 0X220;
        mem_win.width = 10;
        mem_win.height= 10;

        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);

        pAutoAdc_105 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);

        //////////////////////////////
        // Get Pb Min
        mem_win.x = 0X200;
        mem_win.y = 0X60;
        mem_win.width = 10;
        mem_win.height= 10;

        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);

        pAutoAdc_055 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);
        Diff->u16CH_AVG[2] = pAutoAdc_105.u16CH_AVG[2] - pAutoAdc_055.u16CH_AVG[2];

        //////////////////////////////
        // Get Pr Max
        mem_win.x = 0X200;
        mem_win.y = 0X1C0;
        mem_win.width = 10;
        mem_win.height= 10;

        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);

        pAutoAdc_105 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);

        //////////////////////////////
        // Get Pr Min
        mem_win.x = 0X200;
        mem_win.y = 0XB0;
        mem_win.width = 10;
        mem_win.height= 10;

        _XC_ENTRY(pInstance);
        MDrv_XC_wait_input_vsync(pInstance, 5, 200,eWindow);
        _XC_RETURN(pInstance);

        pAutoAdc_055 = _GetAverageDataFromMemory(pInstance, &mem_win, eWindow);
        Diff->u16CH_AVG[0] = pAutoAdc_105.u16CH_AVG[0] - pAutoAdc_055.u16CH_AVG[0];

    }
}

//*************************************************************************
/// -This function internal Auto-tune gain of ADC.
/// @param enAutoTuneType   \b IN:  tune type
/// @param pstADCSetting    \b OUT: gain offset setting
/// @param eWindow          \b IN:  window type, main/sub
/// @return MS_BOOL TRUE succeed, FALSE fail
//*************************************************************************
static MS_BOOL _MApi_XC_Auto_InternalTuneGain(void *pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting, SCALER_WIN eWindow )
{
    #define TOLERATE_ERROR 65 // 10%
    #define MAX_CALIBRATION_ITERATION_EXTERNAL   12

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_AUTOADC_TYPE Diff;
    MS_BOOL bDone[3] = {FALSE, FALSE, FALSE};
    MS_U16 GainTargetLBound[3];
    MS_U16 GainTargetUBound[3];

    MS_U16 Gain[3]      ={0x1000, 0x1000, 0x1000};
    MS_U16 TempGain[3]  ={0x1000, 0x1000, 0x1000};
    MS_U16 delta[3]={0x800, 0x800, 0x800};
    MS_U16 u16UpperDiff[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    MS_U16 u16LowerDiff[3] = {0xFFFF, 0xFFFF, 0xFFFF};
    MS_U16 u16UpperGain[3] = {0, 0, 0};
    MS_U16 u16LowerGain[3] = {0, 0, 0};
    MS_U16 GainTarget[3];
    MS_U8 u8Direction[3] = {0, 0, 0};
    MS_U16 ch;
    MS_U16 y = 0;
    MS_U16 u16Diff;
    MS_BOOL bResult = FALSE;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //////////////////////////////
    // Initialize
    bResult = MDrv_XC_ADC_GetGainSetting(pInstance, pstADCSetting);
    if (bResult)
    {
        if (enAutoTuneType & E_XC_AUTO_TUNE_RGB_GAIN)
        {
            //printf("RGB\n");
            GainTarget[0] = AUTO_GAIN_RANGE_RGB_INTERNAL;
            GainTarget[1] = AUTO_GAIN_RANGE_RGB_INTERNAL;
            GainTarget[2] = AUTO_GAIN_RANGE_RGB_INTERNAL;
        }
        else if(enAutoTuneType & E_XC_AUTO_TUNE_SCART_RGB_GAIN)
        {
            GainTarget[0] = AUTO_GAIN_RANGE_RGB_INTERNAL;
            GainTarget[1] = AUTO_GAIN_RANGE_RGB_INTERNAL;
            GainTarget[2] = AUTO_GAIN_RANGE_RGB_INTERNAL;
        }
        else
        {
            //printf("YUV\n");
            GainTarget[0] = AUTO_GAIN_RANGE_CBCR_INTERNAL;
            GainTarget[1] = AUTO_GAIN_RANGE_Y_INTERNAL;
            GainTarget[2] = AUTO_GAIN_RANGE_CBCR_INTERNAL;
        }

        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GainTarget R: %x  G: %x  B: %x\n",GainTarget[0],GainTarget[1],GainTarget[2]);


        // Set initial gain value
        for (ch=0; ch<3; ch++)
        {
            GainTargetUBound[ch] = GainTarget[ch] + AUTO_GAIN_BONDARY;
            GainTargetLBound[ch] = GainTarget[ch] - AUTO_GAIN_BONDARY;
            MDrv_XC_ADC_SetGain(pInstance, ch, Gain[ch]);
        }

        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GainTarget Upper Bound R: %x  G: %x  B: %x \n", GainTargetUBound[0],GainTargetUBound[1],GainTargetUBound[2]);
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GainTarget Lower Bound R: %x  G: %x  B: %x \n", GainTargetLBound[0],GainTargetLBound[1],GainTargetLBound[2]);

        // Start Gain calibration
        for ( y = 0; y < MAX_CALIBRATION_ITERATION_EXTERNAL; y++)
        {
            _GetGainRange(pInstance, E_XC_INTERNAL_CALIBRATION, enAutoTuneType,&Diff, eWindow);

            //////////////////////////////
            // Adjust for RGB / YUV
            for (ch=0; ch<3; ch++)
            {
                if (ch==0) {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"============No. %02d=============>\n", (y+1));
                }
                if (!bDone[ch])
                {
                    u16Diff = Diff.u16CH_AVG[ch];
                    if ( y == (MAX_CALIBRATION_ITERATION_EXTERNAL-1) )
                    {
                        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Gain[%u]=0x%x, diff=%d\n", ch, Gain[ch], u16Diff);

                        if (u16Diff >= GainTarget[ch])
                        {
                            if (u16UpperDiff[ch] > (u16Diff - GainTarget[ch]))
                            {
                                u16UpperDiff[ch] = u16Diff - GainTarget[ch];
                                u16UpperGain[ch] = TempGain[ch];
                                //AUTOMSG(printk("Cr Upper Diff %04d, Gain %04d\n", u16UpperDiff_R, u16UpperGain_R));
                            }
                        }

                        if (u16Diff <= GainTarget[ch])
                        {
                            if (u16LowerDiff[ch] > (GainTarget[ch] - u16Diff))
                            {
                                u16LowerDiff[ch] = GainTarget[ch] - u16Diff;
                                u16LowerGain[ch] = TempGain[ch];
                                //AUTOMSG(printk("Cr Lower Diff %04d, Gain %04d\n", u16LowerDiff_R, u16LowerGain_R));
                            }
                        }
                    }
                    else
                    {
                        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Gain[%u]=0x%x, diff=%d\n", ch, Gain[ch], u16Diff);

                        // Found a target gain.
                        if ( (u16Diff>=GainTargetLBound[ch] ) && (u16Diff<=GainTargetUBound[ch] ) )
                        {
                            //MS_U16 target,u16Result[5],Step;
                            //MS_AUTOADC_TYPE modifiy_diff;
                            //MS_U8 index,bRetry = TRUE ;

                            bDone[ch] = TRUE;
                            u16UpperDiff[ch] = u16LowerDiff[ch] = 0;

                            u8Direction[ch] = 0;

                            ////////////Find a best solution////////////////////////
                            //target = ( GainTargetLBound[ch] + GainTargetUBound[ch] )  / 2 ;
                            //XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," target : %d  current : %d\n",target,u16Diff);

                            #if 0 // This section takes long time. Remove first.
                            // Retrieve result array, and make sure it is linear.
                            while (bRetry)
                            {
                                for (index = 0, Step = Gain[ch] - 0x08; Step <= Gain[ch] + 0x08 ; Step+= 0x04,index++)
                                {
                                    MDrv_XC_ADC_SetGain(pInstance, ch, Step);
                                    _GetGainRange(E_XC_INTERNAL_CALIBRATION,enAutoTuneType,&modifiy_diff);
                                    u16Result[index] = modifiy_diff.u16CH_AVG[ch];
                                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Result index: %d Gain: 0x%4x   %d \n",index,Step,u16Result[index] );
                                }

                                // check result is linear
                                for ( index = 0 ; index < 5 ; index ++)
                                {
                                    if ( index >= 4 )
                                    {
                                        bRetry = FALSE;
                                        break;
                                    }

                                    if ( u16Result[index] > u16Result[index+1] )
                                    {
                                        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," fail \n");
                                        break;
                                    }
                                }
                                if ( bRetry)
                                    continue;

                                // Find Best solution
                                MS_U8 Index_Start = 0 ,Index_End = 4;
                                MS_BOOL bFound1 = FALSE, bFound2 = FALSE;

                                while ( Index_End > Index_Start && ( !bFound1 || !bFound2) )
                                {
                                    if ( u16Result[Index_Start] != target )
                                        Index_Start++;
                                    else
                                        bFound1 = TRUE;

                                    if ( u16Result[Index_End] != target)
                                        Index_End--;
                                    else
                                        bFound2 = TRUE;
                                }

                                // Founded
                                if ( Index_End >= Index_Start)
                                {
                                    Step = Gain[ch] - 0x08;
                                    Gain[ch] = Step + ( ( (Index_Start + Index_End ) * 10 ) / 2 ) * 0x04 / 10;
                                }
                                else
                                {
                                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," not found \n");
                                }

                            }

                            #endif
                            /////////////////////////////////////////////////

                            u16UpperGain[ch] = u16LowerGain[ch] = Gain[ch];

                            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"=====ch:%u--OK--0x%x=======\n", ch, Gain[ch]);
                        }
                        // continoue
                        else
                        {
                            if (u16Diff >= GainTarget[ch])
                            {
                                if (u16UpperDiff[ch] > (u16Diff - GainTarget[ch]))
                                {
                                    u16UpperDiff[ch] = u16Diff - GainTarget[ch];
                                    u16UpperGain[ch] = Gain[ch];
                                    //AUTOMSG(printk("Upper Diff %04d, Gain %04d\n", u16UpperDiff[ch], u16UpperGain[ch]));
                                }
                            }

                            if (u16Diff <= GainTarget[ch])
                            {
                                if (u16LowerDiff[ch] > (GainTarget[ch] - u16Diff))
                                {
                                    u16LowerDiff[ch] = GainTarget[ch] - u16Diff;
                                    u16LowerGain[ch] = Gain[ch];
                                    //AUTOMSG(printk("Lower Diff %04d, Gain %04d\n", u16LowerDiff[ch], u16LowerGain[ch]));
                                }
                            }

                            if (u16Diff < GainTargetLBound[ch] )
                            {

                                Gain[ch]+=delta[ch];
                                u8Direction[ch] = DOWN_TOP; //DownToUp
                            }
                            else   //(u16Diff > GainTargetUBound[ch] )
                            {

                                Gain[ch]-=delta[ch];
                                u8Direction[ch] = TOP_DOWN; //UpToDown
                            }
                            delta[ch]=delta[ch]>>1;
                        }

                        if ( y == (MAX_CALIBRATION_ITERATION_EXTERNAL-2) )
                        {
                            if (u8Direction[ch] == DOWN_TOP)
                            {
                                // T2 is 1
                                TempGain[ch] = Gain[ch]-0x02;

                            }
                            else if (u8Direction[ch] == TOP_DOWN)
                            {
                                // T2 is 1
                                TempGain[ch] = Gain[ch]+0x02;
                            }
                            MDrv_XC_ADC_SetGain(pInstance, ch, TempGain[ch]);
                        }
                        else
                        {
                            //AUTOMSG( printk("Gain[%u] 0x%x\n", ch, Gain[ch]));
                            //AUTOMSG( printk("delta[%u] 0x%x\n", ch, delta[ch]));
                            MDrv_XC_ADC_SetGain(pInstance, ch, Gain[ch]);
                        }
                    }
                }
            }

            if ( bDone[0] && bDone[1] && bDone[2] )
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"done - \n");
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"gain check ok \33[0;31m 0x%x 0x%x 0x%x \33[m \n",Gain[0],Gain[1],Gain[2]);
                bResult = TRUE;
                break;
            }
        }

    }

    if (!bResult)
    {
        // If code flow reach here, that's meaning we can not find a good gain value.
        // Choose best gain value in TOLERATE_ERROR.
        if ( !(bDone[0] && bDone[1] && bDone[2]) )
        {
            bDone[0] = bDone[1] = bDone[2] = 0;

            for (ch=0; ch<3; ch++)
            {
                if (u16UpperDiff[ch] <= u16LowerDiff[ch])
                {
                    if (u16UpperDiff[ch] <= TOLERATE_ERROR )
                    {
                        Gain[ch] = u16UpperGain[ch];
                        bDone[ch] = TRUE;
                    }
                }
                else
                {
                    if (u16LowerGain[ch] <= TOLERATE_ERROR )
                    {
                        Gain[ch] = u16LowerGain[ch];
                        bDone[ch] = TRUE;
                    }
                }
            }
        }

        if (bDone[0] && bDone[1] && bDone[2])
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Chose one best gain value - ");
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"gain check ok  0x%x 0x%x 0x%x\n",Gain[0],Gain[1],Gain[2]);
            bResult = TRUE;
        }
    }

    if (bResult)
    {
        pstADCSetting->u16RedGain =  Gain[0];
        pstADCSetting->u16GreenGain = Gain[1];
        pstADCSetting->u16BlueGain = Gain[2];
        for (ch=0; ch<3; ch++)
        {
            MDrv_XC_ADC_SetGain(pInstance, ch, Gain[ch]);
        }
    }
    else
    {
        Gain[0] = pstADCSetting->u16RedGain;
        Gain[1] = pstADCSetting->u16GreenGain;
        Gain[2] = pstADCSetting->u16BlueGain;
        for (ch=0; ch<3; ch++)
        {
            MDrv_XC_ADC_SetGain(pInstance, ch, Gain[ch]);
        }
    }
    return bResult;
}

static MS_BOOL _MApi_XC_Auto_Internal_AutoCablication(void* pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting , SCALER_WIN eWindow)
{
    // Do not use 720P and 1080P timing in Internal calibration.
    #define ADC_720P    0
    #define ADC_1080P   0

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    APIXC_AdcGainOffsetSetting tempADCSetting;
    MS_BOOL result;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MirrorMode_t enMirrorBackup[MAX_WINDOW] = {gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode,gSrcInfo[SUB_WINDOW].Status2.eMirrorMode};
    MS_U16 u16BackupHsize, u16BackupVsize;
    _XC_ENTRY(pInstance);
    MS_BOOL bInputSourceEnabledBackup = MDrv_XC_IsInputSourceDisabled(pInstance, eWindow);
    _XC_RETURN(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"======== Start : Calibration Type: %x ======== \n",enAutoTuneType);
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Backup Register setting\n");
    if ((eWindow >= MAX_WINDOW) || (NULL == pstADCSetting))
    {
        printf("%s %d:Parameter error!\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    //MS_AUTOADC_REG_BAK  AutoAdcRegBak;
    MDrv_XC_ADC_BackupAdcReg(pInstance, eWindow);
    u16BackupHsize = gSrcInfo[eWindow].u16H_SizeAfterPreScaling;
    u16BackupVsize = gSrcInfo[eWindow].u16V_SizeAfterPreScaling;
    if ((MIRROR_V_ONLY == enMirrorBackup[eWindow]) || (MIRROR_HV == enMirrorBackup[eWindow]))
    {
        gSrcInfo[eWindow].Status2.eMirrorMode = MIRROR_NORMAL;

        MDrv_XC_FreezeImg(pInstance, ENABLE, eWindow);

        // For re-assign memory address, the input source need to be disabled
        if (bInputSourceEnabledBackup == TRUE)
            MDrv_XC_DisableInputSource(pInstance, ENABLE, eWindow);

        if(eWindow == MAIN_WINDOW)
        {
            MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size, eWindow);
        }
        else
        {
             MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size, eWindow);
        }

        if (bInputSourceEnabledBackup == TRUE)
            MDrv_XC_DisableInputSource(pInstance, DISABLE, eWindow);

        MDrv_SC_set_mirror(pInstance, FALSE, eWindow);
        MDrv_SC_Set_pre_align_pixel(pInstance, DISABLE, 0, eWindow );

        MDrv_XC_FreezeImg(pInstance, DISABLE, eWindow);

    }

    _XC_ENTRY(pInstance);
    MDrv_XC_WaitOutputVSync(pInstance, 1, 50, eWindow);
    MDrv_SC_GenerateBlackVideo(pInstance, ENABLE, eWindow );  //balck screen
    _XC_RETURN(pInstance);
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Initial internal calibration setting");

    if (enAutoTuneType & E_XC_AUTO_TUNE_RGB_GAIN)
    {
        INPUT_SOURCE_TYPE_t enInputSourcetype;

        enInputSourcetype = pXCResourcePrivate->stdrvXC_MVideo._SCShared.StatusInfo[eWindow].enInputSourceType;

        if(enInputSourcetype == INPUT_SOURCE_SCART)
        {
            MDrv_XC_ADC_InitInternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_SCART, &tempADCSetting, eWindow);
            MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, INPUT_SOURCE_SCART));
        }
        else
        {
            MDrv_XC_ADC_InitInternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_VGA, &tempADCSetting, eWindow);
            MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, INPUT_SOURCE_VGA));
        }

    }
    else if (enAutoTuneType & E_XC_AUTO_TUNE_YUV_COLOR)
    {
        MDrv_XC_ADC_InitInternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_YPBPR, &tempADCSetting, eWindow);
        MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, INPUT_SOURCE_YPBPR));
    }
    else
    {
        MDrv_XC_ADC_InitInternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_SCART, &tempADCSetting, eWindow);
        MDrv_XC_ADC_Source_Calibrate(pInstance, MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, INPUT_SOURCE_SCART));
    }
#if ADC_720P
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto Gen timing: 720p\n");
    MApi_XC_ADC_GenClock(pInstance, E_ADC_Gen_720P_Clk);
    _XC_ENTRY(pInstance);
    MDrv_XC_GenSpecificTiming(pInstance, E_XC_720P);
    _XC_RETURN(pInstance);
    MDrv_XC_InitIPForInternalTiming(pInstance, E_XC_720P, eWindow);

    gSrcInfo[eWindow].u16H_SizeAfterPreScaling = 1280;
    gSrcInfo[eWindow].u16V_SizeAfterPreScaling = 720;
#elif ADC_1080P
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto Gen timing: 1080p\n");

    MApi_XC_ADC_GenClock(pInstance, E_ADC_Gen_1080P_Clk);
    _XC_ENTRY(pInstance);
    MDrv_XC_GenSpecificTiming(pInstance, E_XC_1080P);
    _XC_RETURN(pInstance);
    MDrv_XC_InitIPForInternalTiming(pInstance, E_XC_1080P, eWindow);

    gSrcInfo[eWindow].u16H_SizeAfterPreScaling = 1920;
    gSrcInfo[eWindow].u16V_SizeAfterPreScaling = 1080;
#else
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto Gen timing: 480p\n");

    MApi_XC_ADC_GenClock(pInstance, E_ADC_Gen_480P_Clk);
    _XC_ENTRY(pInstance);
    MDrv_XC_GenSpecificTiming(pInstance, E_XC_480P);
    _XC_RETURN(pInstance);
    MDrv_XC_InitIPForInternalTiming(pInstance, E_XC_480P, eWindow);

    gSrcInfo[eWindow].u16H_SizeAfterPreScaling = 720;
    gSrcInfo[eWindow].u16V_SizeAfterPreScaling = 480;
#endif
    //MyMDrv_SetCalibrartinWindow(1, 16);
    //MDrv_ADC_gainoffset_reset();
    MApi_XC_ADC_AutoSetting(pInstance, ENABLE,TRUE,COMPONENT_AUTO_SW_MODE);

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Start Auto Gain Procedure\n");
    result = _MApi_XC_Auto_InternalTuneGain(pInstance, enAutoTuneType, &tempADCSetting , eWindow );
    pstADCSetting->u16RedOffset = tempADCSetting.u16RedOffset;
    pstADCSetting->u16GreenOffset = tempADCSetting.u16GreenOffset;
    pstADCSetting->u16BlueOffset = tempADCSetting.u16BlueOffset;

    pstADCSetting->u16RedGain = tempADCSetting.u16RedGain;
    pstADCSetting->u16GreenGain = tempADCSetting.u16GreenGain;
    pstADCSetting->u16BlueGain = tempADCSetting.u16BlueGain;

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"gain check ok  0x%02x 0x%02x 0x%02x\n",
        pstADCSetting->u16RedGain,pstADCSetting->u16GreenGain,pstADCSetting->u16BlueGain);

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"offset check ok  0x%02x 0x%02x 0x%02x\n",
        pstADCSetting->u16RedOffset,pstADCSetting->u16GreenOffset,pstADCSetting->u16BlueOffset);

    MApi_XC_ADC_SetInternalVoltage(pInstance, E_ADC_Internal_None );

    //remove the delay time to HAL layer escape from garbage when interal SW calibration
    //MsOS_DelayTask(200);
    MApi_XC_AutoGainEnable(pInstance, DISABLE,eWindow);
    //MDrv_WriteByte(L_BK_IP1F2(0x0E), 0x00); // disable auto gain function
    //MDrv_WriteByte( BK_SELECT_00, u8Bank );
    //test----start----
#if ( GET_RGB_REPORT == ENABLE )
    if ( result == TRUE )
    {
       GetTestReport();

       GetTestReport_Bk1A();
     }
     //test----start----
#endif
    _XC_ENTRY(pInstance);
    MDrv_XC_GenSpecificTiming(pInstance, E_XC_OFF);
    _XC_RETURN(pInstance);

    if ((MIRROR_V_ONLY == enMirrorBackup[eWindow]) || (MIRROR_HV == enMirrorBackup[eWindow]))
    {

        gSrcInfo[eWindow].Status2.eMirrorMode = enMirrorBackup[eWindow];
    }
    MDrv_XC_ADC_RestoreAdcReg(pInstance, eWindow);

    gSrcInfo[eWindow].u16H_SizeAfterPreScaling = u16BackupHsize;
    gSrcInfo[eWindow].u16V_SizeAfterPreScaling = u16BackupVsize;

    return  result;

}


//-----------------------------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------------------------------




static void _MApi_XC_Auto_WaitStatusReady(void *pInstance, XC_Auto_WaitingType type, SCALER_WIN eWindow  )
{
    MS_U16 u16Dummy = 1000;
    if ( type == E_WAITING_AUTO_POSITION_READY )
        while(!( MApi_XC_IsAutoPositionResultReady(pInstance, eWindow) ) && (u16Dummy--)) ;
    else
        while(!( MApi_XC_IsAutoGainResultReady(pInstance, eWindow) ) && (u16Dummy--)) ;
}


//*************************************************************************
//Function name: MDrv_Auto_CheckSyncLoss
//Passing parameter: NO
//Return parameter:
//  MS_BOOL: If PC mode change, return TRUE.
//Description: Check PC mode change when auto-tune executive.
//*************************************************************************
static MS_BOOL _MApi_XC_Auto_CheckSyncLoss(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bResult = FALSE;
    _XC_ENTRY(pInstance);
    bResult = MDrv_XC_PCMonitor_InvalidTimingDetect(pInstance, TRUE, eWindow);
    _XC_RETURN(pInstance);
    if(bResult)
    {
        MDrv_XC_PCMonitor_Restart(pInstance, eWindow);
    }
    return bResult;
}


//*************************************************************************
//Function name: _MApi_XC_Auto_GetPosition
//Passing parameter:
//  MS_U8 u8RegIndex: Register index
//  MS_U8 u8VSyncTime: VSync time
//Return parameter:
//  MS_U16: Position value.
//Description: Get position by register select
//*************************************************************************
static MS_U16 _MApi_XC_Auto_GetPosition(void *pInstance, XC_Auto_GetAutoPositionType type , MS_U8 u8VSyncTime, SCALER_WIN eWindow)
{
    MS_U16 u16ComparePos=0,u16AutoPos; // position buffer,  Add the initial value
    MS_U8 u8Dummy = 20; // loop dummy
    MS_U8 u8Count = 0; // counter of compare alike
    MS_U16 (*GetPositionFunction)(void *pInstance,  SCALER_WIN eWindow ) = MApi_XC_SetAutoPositionHstart;//simply assign a trivial initial value


    MApi_XC_AutoPositionEnable(pInstance, ENABLE, eWindow);

    switch ( type )
    {
        case E_XC_AUTO_GET_HSTART:
            GetPositionFunction = MApi_XC_SetAutoPositionHstart;
            break;
        case E_XC_AUTO_GET_HEND:
            GetPositionFunction = MApi_XC_SetAutoPositionHend;
            break;
        case E_XC_AUTO_GET_VSTART:
            GetPositionFunction = MApi_XC_SetAutoPositionVstart;
            break;
        case E_XC_AUTO_GET_VEND:
            GetPositionFunction = MApi_XC_SetAutoPositionVend;
            break;
        default:
            break;
    }
    while(u8Dummy--)
    {
        _MApi_XC_Auto_WaitStatusReady(pInstance, E_WAITING_AUTO_POSITION_READY, eWindow); // auto position result ready

        u16AutoPos = GetPositionFunction(pInstance, eWindow); //MDrv_Read2Byte(u32RegIndex) & 0xFFF; // get auto position
        if(u16AutoPos == u16ComparePos) // match
            u8Count++;
        else // different
        {
            u8Count = 0; // reset counter
            u16ComparePos = u16AutoPos; // reset position
        }

        if(u8Count == 3) // match counter ok
            break;

        if(_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow)) // check no signal
        {

            return -1; // return fail
        }

        MsOS_DelayTask(u8VSyncTime); // wait next frame
    } // while
    return u16AutoPos;
}

#if (!FAST_AUTO_TUNE)
//*************************************************************************
//Function name: MDrv_Auto_SetValidData
//Passing parameter:
//  MS_U8 u8VSyncTime : VSync time for delay
//Return parameter:
//  MS_BOOL: If PC mode change, return FALSE.
//Description: Auto set valid data value.
//*************************************************************************
static MS_BOOL _MApi_XC_Auto_SetValidData(void *pInstance, MS_U8 u8VSyncTime, SCALER_WIN eWindow)
{
    MS_U8 u8ValidData; // valide dataa value
    MS_U16 u16PhaseIndex; // phase index
    MS_U16 u16ComapreHPos; // compare horizontal position

    for(u8ValidData = 0x04; u8ValidData != 0x10; u8ValidData++)
    {
        MDrv_XC_SetValidDataThreshold(pInstance, u8ValidData, eWindow);

        MDrv_XC_ADC_SetPhaseEx(pInstance, 0x00); // set phase
        MsOS_DelayTask(u8VSyncTime);

        u16ComapreHPos = _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HSTART ,u8VSyncTime , eWindow); // horizontal position
        for(u16PhaseIndex = 0x01; u16PhaseIndex < 0x10; u16PhaseIndex++)
        {
            MDrv_XC_ADC_SetPhaseEx(pInstance, u16PhaseIndex * 4); // set phase
            MsOS_DelayTask(u8VSyncTime);

            if(abs(u16ComapreHPos - _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HSTART ,u8VSyncTime , eWindow)) > 3) // check lose data
                break;

            if(_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow)) // check Sync change
            {
                return FALSE;
            }
        } // for

        if(u16PhaseIndex == 0x10)
            break;
    } // for

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Valid=0x%x\n", u8ValidData << 4));
    return TRUE;
}
#endif



//*************************************************************************
//Function name: _MApi_XC_Auto_TunePosition
//Passing parameter:
//  MS_U8 u8VSyncTime : VSync time
//  MS_PCADC_MODESETTING_TYPE *pstModesetting: Current PC mode setting
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: Auto-tune vertical and horizontal position for PC mode
//*************************************************************************
static MS_BOOL _MApi_XC_Auto_TunePosition(void *pInstance, MS_U8 u8VSyncTime, XC_Auto_Signal_Info_Ex *Active , XC_Auto_Signal_Info_Ex *StandardInfo , SCALER_WIN eWindow)
{
    MS_U16 u16PosBff; // position buffer
    MS_BOOL bResult=TRUE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // horizotal position
    // auto horizontal start position detected result
    u16PosBff = _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HSTART ,u8VSyncTime , eWindow);

#if ENABLE_VGA_EIA_TIMING
    if (MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow) &XC_MD_INTERLACE_BIT)
    {
        u16PosBff++;
    }
#endif

    if(u16PosBff > MAX_PC_AUTO_H_START || u16PosBff < MIN_PC_AUTO_H_START)
    {
        //printf("H start limit: 0x%x (%x->%x)\n", u16PosBff, (MS_U16) MIN_PC_AUTO_H_START, (MS_U16) MAX_PC_AUTO_H_START);
        u16PosBff = StandardInfo->u16HorizontalStart;
        bResult = FALSE;
    }

    Active->u16HorizontalStart = u16PosBff;

    MDrv_XC_SetCaptureWindowHstart(pInstance, u16PosBff , eWindow);

    // vertical positoin
    // auto vertical start position detected result

    u16PosBff =_MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_VSTART ,u8VSyncTime , eWindow);

    if(u16PosBff > MAX_PC_AUTO_V_START || u16PosBff < MIN_PC_AUTO_V_START)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"V start limit: 0x%x (%x->%x)\n", u16PosBff, (MS_U16) MIN_PC_AUTO_V_START, (MS_U16) MAX_PC_AUTO_V_START);
        u16PosBff = StandardInfo->u16VerticalStart;
        bResult = FALSE;
    }

    Active->u16VerticalStart = u16PosBff;

    MDrv_XC_SetCaptureWindowVstart(pInstance, u16PosBff, eWindow);

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoPosition VStart=0x%x\n", Active->u16VerticalStart);

    return bResult;
}

//*************************************************************************
//Function name: _MApi_XC_Auto_GetTransPos
//Passing parameter:
//  MS_U8 u8VSyncTime : VSync time
//Return parameter:
//  MS_U16: If faile, return -1. else return phase setting
//Description: Get trans-position.
//*************************************************************************
static MS_U16 _MApi_XC_Auto_GetTransPos(void *pInstance, MS_U8 u8VSyncTime, SCALER_WIN eWindow)
{
    MS_U16 u16ComparePos; // compare start position
    MS_U16 u16AdjustPhase = 0x20;
    MS_U8 u8PhaseDelta = 0x20; // phase data buffer

    MDrv_XC_ADC_SetPhaseEx(pInstance, 0x00); // intialize
    u16ComparePos = _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HSTART ,u8VSyncTime , eWindow);

    while(1)
    {
        MDrv_XC_ADC_SetPhaseEx(pInstance, u16AdjustPhase); // set phase

        u8PhaseDelta /= 2; // next step
        if(u8PhaseDelta == 0x00) // check end
            break;

        if( _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HSTART ,u8VSyncTime , eWindow) == u16ComparePos) // find critical phase
            u16AdjustPhase += u8PhaseDelta; // right shift
        else
            u16AdjustPhase -= u8PhaseDelta; // left shift

        if(_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow)) // check no signal
        {
            return -1;
        }
    } // while

    return (u16AdjustPhase);
}

//*************************************************************************
//Function name: _MApi_XC_Auto_GetActualWidth
//Passing parameter:
//  MS_U8 u8VSyncTime : VSync time
//Return parameter:
//  MS_U16: return actual image width
//Description: Get actual image width.
//*************************************************************************
static MS_U16 _MApi_XC_Auto_GetActualWidth(void *pInstance, MS_U8 u8VSyncTime, SCALER_WIN eWindow)
{
    MS_U16 u16HStart; // actual horizontal start
    MS_U16 u16Width;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_ADC_SetPhaseEx(pInstance, 0x00); // initialize phase value

    u16HStart =  _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HSTART ,u8VSyncTime , eWindow); // horizontal start position
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Get %s Window's VSync Time =%d and H Start=%d\n", eWindow?("Sub"):("Main"), u8VSyncTime, u16HStart);
    _MApi_XC_Auto_GetTransPos(pInstance, u8VSyncTime, eWindow); // seek critical phase

    u16Width = (( _MApi_XC_Auto_GetPosition(pInstance, E_XC_AUTO_GET_HEND ,u8VSyncTime , eWindow) - u16HStart) + 1); // actual image width

    return u16Width;
}

//*************************************************************************
//Function name: _MApi_XC_Auto_TuneHTotal
//Passing parameter:
//  MS_U8 u8VSyncTime: VSync time
//  MS_PCADC_MODESETTING_TYPE *pstModesetting: Current PC mode setting
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: auto-tune horizontal total.
//*************************************************************************
static MS_BOOL _MApi_XC_Auto_TuneHTotal(void *pInstance, MS_U8 u8VSyncTime, XC_Auto_Signal_Info_Ex *Active , XC_Auto_Signal_Info_Ex *StandardInfo , SCALER_WIN eWindow)
{
    MS_U16 u16ActualWidth; // actual width
    MS_U16 u16StdWidth; // standard width
    MS_U16 u16HTotalBff; // horizontal total buffer
    MS_BOOL bResult = FALSE;

    MDrv_XC_ADC_SetPhaseEx(pInstance, 0x00); // initialize phase value

    // Get Active width from Scaler.
    u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);

    // Get standard width from PCmonitor (mode table)
    u16StdWidth = StandardInfo->u16HResolution;

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Step1 u16StdWidth=%u, u16ActualWidth=%u\n",u16StdWidth, u16ActualWidth);

    if(abs(u16ActualWidth - u16StdWidth) > (u16StdWidth / 6)) // check actual width over standard
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AWoverSW(%u,%u)\n", u16ActualWidth, u16StdWidth);
        return FALSE;
    }
    u16HTotalBff = Active->u16HorizontalTotal; // intialize horizontal total buffer
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Step2 Htt=%u, Std Htt=%u\n", u16HTotalBff, StandardInfo->u16HorizontalTotal);

    if(abs(u16ActualWidth - u16StdWidth) > 1) // check width difference
    {
        // calculate horizontal total
        u16HTotalBff = ((MS_U32) Active->u16HorizontalTotal * u16StdWidth) / u16ActualWidth;

        // check over range of adjusting
        if(abs(u16HTotalBff - StandardInfo->u16HorizontalTotal) > (Active->u16HorizontalTotal/4) )
        {
            //XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"OverADJ %u\n", ADJUST_CLOCK_RANGE));
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"OverADJ\n");
            return FALSE;
        }
        MDrv_XC_ADC_SetPcClock(pInstance, u16HTotalBff); // set clock
        // check width
        u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Step3 HTT=%u, Actual Width=%u\n", u16HTotalBff, u16ActualWidth);

    if(u16ActualWidth != u16StdWidth) // match width
    {
        // adjust horizontal total
        u16HTotalBff = u16HTotalBff + (u16StdWidth - u16ActualWidth);

        MDrv_XC_ADC_SetPcClock(pInstance, u16HTotalBff);

        u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);

        // adjust horizontal total again
        u16HTotalBff = u16HTotalBff + (u16StdWidth - u16ActualWidth);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Step4 HTT=%u, Actual Width=%u\n", u16HTotalBff, u16ActualWidth);
    if(u16HTotalBff & 0x01) // match width and check odd
    {
        MDrv_XC_ADC_SetPcClock(pInstance, (u16HTotalBff - 1)); // find decrement

        u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);
        if(u16ActualWidth == u16StdWidth) // match width
            u16HTotalBff--;
        else
        {
            MDrv_XC_ADC_SetPcClock(pInstance, (u16HTotalBff + 1)); // find increment
            u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);
            if(u16ActualWidth == u16StdWidth) // match width
                u16HTotalBff++;
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Step5 HTT=%u, Actual Width=%u\n", u16HTotalBff, u16ActualWidth);
    // check horizontal total range
    if(abs(u16HTotalBff - (StandardInfo->u16HorizontalTotal)) < (StandardInfo->u16HorizontalTotal/4) )
    {
        Active->u16HorizontalTotal = u16HTotalBff;
        Active->u16HResolution = u16ActualWidth;
        bResult = TRUE;
    }
    else
    {
        Active->u16HResolution = 0xFFFF;
    }

    MDrv_XC_ADC_SetPcClock(pInstance, Active->u16HorizontalTotal); // setting ADC clock
    MDrv_XC_ADC_SetPhaseEx(pInstance, Active->u16Phase); // setting ADC phase

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoHtt bResult=%u, Htt = %u, Actual Width=%u\n", bResult, Active->u16HorizontalTotal, u16ActualWidth);
    return bResult;
}


//*************************************************************************
//Function name: _MApi_XC_Auto_TunePhase
//Passing parameter:
//  MS_U8 u8VSyncTime: VSync time
//  MS_PCADC_MODESETTING_TYPE *pstModesetting: Current PC mode setting
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: auto-tune phase.
//*************************************************************************

MS_BOOL _MApi_XC_Auto_TunePhase(void *pInstance, MS_U8 u8VSyncTime, XC_Auto_Signal_Info_Ex *Active, SCALER_WIN eWindow)
{

#if AUTO_PHASE_METHOD
    MS_U16 u16Index; // loop index
    MS_U16 u16PhaseRange = MDrv_XC_ADC_GetPhaseRange(pInstance) - 1;
    MS_U32 u32AutoPhaseVal; // auto phase value result
    MS_U32 u32MiniPhaseVal = -1; // minimum phase value
    MS_U16 u16WorstPhase1 = 0, u16WorstPhase2 = 0;

    MApi_XC_AutoPhaseEnable(pInstance, ENABLE, eWindow);

    u16WorstPhase1 = 0x00; // initizlize
    for(u16Index = u16WorstPhase1; u16Index <= u16PhaseRange; u16Index += AUTO_PHASE_STEP)
    {
        MDrv_XC_ADC_SetPhaseEx(pInstance, u16Index);
        MsOS_DelayTask(u8VSyncTime); // delay 1 frame
        u32AutoPhaseVal = MApi_XC_GetPhaseValue(pInstance, eWindow);

        if(u32AutoPhaseVal < u32MiniPhaseVal) // check minimum
        {
            u16WorstPhase1 = u16Index; // refresh best phase
            u32MiniPhaseVal = u32AutoPhaseVal; // refresh minimum value
        }

        if(_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow)) // check no signal
            return FALSE;
    } // for

       // initizlize
    u16WorstPhase2 = (u16WorstPhase1 - AUTO_PHASE_STEP + 1) & u16PhaseRange;
    u16WorstPhase1 = (u16WorstPhase1 + AUTO_PHASE_STEP) & u16PhaseRange;
    u32MiniPhaseVal = -1;
    for(u16Index = u16WorstPhase2; u16Index != u16WorstPhase1; u16Index = ((u16Index + 1) & u16PhaseRange))
    {
          MDrv_XC_ADC_SetPhaseEx(pInstance, u16Index);
        MsOS_DelayTask(u8VSyncTime); // delay 1 frame
        u32AutoPhaseVal = MApi_XC_GetPhaseValue(pInstance, eWindow);

        if(u32AutoPhaseVal < u32MiniPhaseVal) // check minimum
        {
            u16WorstPhase2 = u16Index; // refresh best phase
            u32MiniPhaseVal = u32AutoPhaseVal; // refresh minimum value
        }

        if(_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow)) // check no signal
            return FALSE;
    } // for

      Active->u16Phase = (u16WorstPhase2 + (u16PhaseRange / 2)) & u16PhaseRange;
    MDrv_XC_ADC_SetPhaseEx(pInstance, Active->u16Phase);

#else

    MS_U16 u16PhaseRange = MDrv_XC_ADC_GetPhaseRange(pInstance);
    const MS_U16 u16PhaseStep = (u16PhaseRange + 63) / 64;

    MS_U32 u32AutoPhaseVal, u32MaxPhaseVal = 0; // maximum phase value
    MS_U16 u16CurPhase, u16BestPhase = 0;

    MApi_XC_AutoPhaseEnable(pInstance, ENABLE, eWindow);

     for (u16CurPhase = 0; u16CurPhase < u16PhaseRange; u16CurPhase += u16PhaseStep)
    {
        MDrv_XC_ADC_SetPhaseEx(pInstance, u16CurPhase);
        MsOS_DelayTask(u8VSyncTime);

        u32AutoPhaseVal = MApi_XC_GetPhaseValue(pInstance, eWindow);

        if (u32AutoPhaseVal > u32MaxPhaseVal) // check maximum
        {
            u16BestPhase = u16CurPhase;
            u32MaxPhaseVal = u32AutoPhaseVal;
        }

        if (_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow))
            return FALSE;
    }

     // 2nd stage for searching best phase around prvious "BestPhase"
    // initizlize (Set range)
    u16CurPhase   = u16BestPhase == 0 ? 0 : u16BestPhase - u16PhaseStep + 1;
    u16PhaseRange = u16BestPhase + u16PhaseStep;
    u32MaxPhaseVal = 0;

    for (; u16CurPhase < u16PhaseRange; u16CurPhase += 1)
    {
        MDrv_XC_ADC_SetPhaseEx(pInstance, u16CurPhase);
        MsOS_DelayTask(u8VSyncTime);

        u32AutoPhaseVal = MApi_XC_GetPhaseValue(pInstance, eWindow);

        if (u32AutoPhaseVal > u32MaxPhaseVal) // check maximum
        {
            u16BestPhase = u16CurPhase;
            u32MaxPhaseVal = u32AutoPhaseVal;
        }

        if (_MApi_XC_Auto_CheckSyncLoss(pInstance, eWindow))
            return FALSE;
    }

    MDrv_XC_ADC_SetPhaseEx(pInstance, u16BestPhase);
    Active->u16Phase = u16BestPhase;
#endif

    //printf("AutoPhase %x\n", Active->u16Phase);
    return TRUE;
}

//*************************************************************************
//Function name: _MApi_XC_Auto_ExternalTuneVgaOffset
//Passing parameter:
//  MS_U8 u8VSyncTime: VSync time
//  APIXC_AdcGainOffsetSetting * pstADCSetting: ADC setting of Current PC mode
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: Auto-tune R/G/B Offset of ADC.
//*************************************************************************

#if 1
static MS_BOOL _MApi_XC_Auto_ExternalTuneVgaOffset(void *pInstance, MS_U8 u8VSyncTime, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    APIXC_AdcGainOffsetSetting stTmpAdcSetting, stBackupAdcSetting;
    MS_U16 u16OffsetDelta = (MDrv_XC_ADC_GetMaximalOffsetValue(pInstance) + 1) >> 1; // adjust step
    MS_U8 u8ATGStatus = 0; // auto gain status//ERROR FIX Prevent Tool 070522
    MS_U8 u8FlowFlag = 0x00; // underflow or overflow flag
    MS_BOOL bResult = FALSE;
    memset(&stTmpAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));
    memset(&stBackupAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));

    bResult = MDrv_XC_ADC_GetGainSetting(pInstance, &stBackupAdcSetting);
    if (bResult)
    {
        bResult = MDrv_XC_ADC_GetOffsetSetting(pInstance, &stBackupAdcSetting);
        if (bResult)
        {
            bResult = MDrv_XC_ADC_InitExternalCalibration(pInstance, INPUT_SOURCE_VGA, &stTmpAdcSetting);
            if (bResult)
            {
                while(TRUE)
                {
                    u16OffsetDelta /= 2; // next

                    if(u16OffsetDelta == 0x00) // check end
                        break;

                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &stTmpAdcSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &stTmpAdcSetting);
                    _XC_RETURN(pInstance);

                    MsOS_DelayTask(u8VSyncTime * 3); // wait stable
                    _MApi_XC_Auto_WaitStatusReady(pInstance, E_WAITING_AUTO_GAIN_READY, MAIN_WINDOW);

                    {
                        MS_U16 u16ResultR,u16ResultG,u16ResultB;

                        u16ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_R, u8VSyncTime, MAIN_WINDOW);
                        u16ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_G, u8VSyncTime, MAIN_WINDOW);
                        u16ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_B, u8VSyncTime, MAIN_WINDOW);
                        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," Result R: %d  G: %d  B: %d \n", u16ResultR,u16ResultG,u16ResultB);
                        if (u16ResultR == 0x00)
                            u8ATGStatus |= BIT(2);
                        else
                            u8ATGStatus &= ~BIT(2);

                        if (u16ResultG == 0x00)
                            u8ATGStatus |= BIT(1);
                        else
                            u8ATGStatus &= ~BIT(1);


                        if (u16ResultB == 0x00)
                            u8ATGStatus |= BIT(0);
                        else
                            u8ATGStatus &= ~BIT(0);
                    }

                    // red
                    if(u8ATGStatus & BIT(2) )
                    {
                        stTmpAdcSetting.u16RedOffset += u16OffsetDelta;
                        u8FlowFlag |= BIT(0);
                    }
                    else
                    {
                        stTmpAdcSetting.u16RedOffset -= u16OffsetDelta;
                        u8FlowFlag |= BIT(1);
                    }

                    // green
                    if( u8ATGStatus & BIT(1) )
                    {
                        stTmpAdcSetting.u16GreenOffset += u16OffsetDelta;
                        u8FlowFlag |= BIT(2);
                    }
                    else
                    {
                        stTmpAdcSetting.u16GreenOffset -= u16OffsetDelta;
                        u8FlowFlag |= BIT(3);
                    }

                    // blue
                    if(u8ATGStatus & BIT(0))
                    {
                        stTmpAdcSetting.u16BlueOffset += u16OffsetDelta;
                        u8FlowFlag |= BIT(4);
                    }
                    else
                    {
                        stTmpAdcSetting.u16BlueOffset -= u16OffsetDelta;
                        u8FlowFlag |= BIT(5);
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"OffATG=0x%x,FF=0x%x\n", u8ATGStatus, u8FlowFlag);
                } // while

                // if calibration successed, will write new value, else, write original value
                if(u8FlowFlag == 0x3f)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"VGA calibration succeed!!\n");
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," offset R: %d G: %d  B: %d \n",pstADCSetting->u16RedOffset,
                        pstADCSetting->u16GreenOffset, pstADCSetting->u16BlueOffset);
                    pstADCSetting->u16RedGain = stBackupAdcSetting.u16RedGain;
                    pstADCSetting->u16GreenGain = stBackupAdcSetting.u16GreenGain;
                    pstADCSetting->u16BlueGain = stBackupAdcSetting.u16BlueGain;
                    pstADCSetting->u16RedOffset = stTmpAdcSetting.u16RedOffset;
                    pstADCSetting->u16GreenOffset = stTmpAdcSetting.u16GreenOffset;
                    pstADCSetting->u16BlueOffset = stTmpAdcSetting.u16BlueOffset;
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, pstADCSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, pstADCSetting);
                    _XC_RETURN(pInstance);
                    bResult = TRUE;
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"VGA calibration fail!!\n");
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &stBackupAdcSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &stBackupAdcSetting);
                    _XC_RETURN(pInstance);

                    bResult = FALSE;
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"External offset Calibration initial fail !! \n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneVgaOffset GetOffsetSetting fail!!\n");
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneVgaOffset GetGainSetting fail!!\n");
    }
    return bResult;
}
#endif

MS_BOOL _MApi_XC_Auto_ExternalTuneVgaRGBGain(void *pInstance, MS_U8 u8VSyncTime, APIXC_AdcGainOffsetSetting *pstADCSetting,SCALER_WIN eWindow)
{
    APIXC_AdcGainOffsetSetting stTmpAdcSetting, stBackupAdcSetting;
    MS_U16 u8GainDelta = (MDrv_XC_ADC_GetMaximalGainValue(pInstance) + 1) >> 1;
    MS_U8 u8ATGStatus = 0 ;
    MS_U8 u8FlowFlag = 0x00;
    MS_BOOL bResult = FALSE;
    memset(&stTmpAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));
    memset(&stBackupAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));

    bResult = MDrv_XC_ADC_GetGainSetting(pInstance, &stBackupAdcSetting);
    if (bResult)
    {
        bResult = MDrv_XC_ADC_GetOffsetSetting(pInstance, &stBackupAdcSetting);
        if (bResult)
        {
            bResult = MDrv_XC_ADC_InitExternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_VGA, &stTmpAdcSetting);
            if (bResult)
            {
                while (TRUE)
                {
                    u8GainDelta /= 2; // next

                    if (u8GainDelta == 0x00) // check end
                        break;

                    MDrv_XC_ADC_SetGainSetting(pInstance, &stTmpAdcSetting);
                    MsOS_DelayTask(u8VSyncTime * 3); // wait stable
                    _MApi_XC_Auto_WaitStatusReady(pInstance, E_WAITING_AUTO_GAIN_READY, eWindow);

                    u8ATGStatus = MApi_XC_GetAutoGainMaxValueStatus(pInstance, MAIN_WINDOW);
                    // red
                    if (u8ATGStatus & BIT(2))
                    {
                        stTmpAdcSetting.u16RedGain -= u8GainDelta;
                        u8FlowFlag |= BIT(0);
                    }
                    else
                    {
                        stTmpAdcSetting.u16RedGain += u8GainDelta;
                        u8FlowFlag |= BIT(1);
                    }

                    // green
                    if (u8ATGStatus & BIT(1) )
                    {
                        stTmpAdcSetting.u16GreenGain -= u8GainDelta;
                        u8FlowFlag |= BIT(2);
                    }
                    else
                    {
                        stTmpAdcSetting.u16GreenGain += u8GainDelta;
                        u8FlowFlag |= BIT(3);
                    }

                    // blue
                    if (u8ATGStatus & BIT(0) )
                    {
                        stTmpAdcSetting.u16BlueGain -= u8GainDelta;
                        u8FlowFlag |= BIT(4);
                    }
                    else
                    {
                        stTmpAdcSetting.u16BlueGain += u8GainDelta;
                        u8FlowFlag |= BIT(5);
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GainATG=0x%x,FF=0x%x\n", u8ATGStatus, u8FlowFlag);
                } // while

                // if calibration successed, will write new value, else, write original value
                if (u8FlowFlag == 0x3f)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"VGA calibration succeed!!\n");
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," Gain R: %d G: %d  B: %d \n",pstADCSetting->u16RedGain,
                        pstADCSetting->u16GreenGain, pstADCSetting->u16BlueGain);
                    pstADCSetting->u16RedGain = stTmpAdcSetting.u16RedGain;
                    pstADCSetting->u16GreenGain = stTmpAdcSetting.u16GreenGain;
                    pstADCSetting->u16BlueGain = stTmpAdcSetting.u16BlueGain;
                    pstADCSetting->u16RedOffset = stTmpAdcSetting.u16RedOffset;
                    pstADCSetting->u16GreenOffset = stTmpAdcSetting.u16GreenOffset;
                    pstADCSetting->u16BlueOffset = stTmpAdcSetting.u16BlueOffset;
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, pstADCSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, pstADCSetting);
                    _XC_RETURN(pInstance);
                    bResult = TRUE;
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"VGA calibration fail!!\n");
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &stBackupAdcSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &stBackupAdcSetting);
                    _XC_RETURN(pInstance);
                    bResult = FALSE;
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"External VGA Calibration initial fail !! \n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneVgaRGBGain GetOffsetSetting fail!!\n");
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneVgaRGBGain GetGainSetting fail!!\n");
    }
    return bResult;
}

MS_BOOL _MApi_XC_Auto_ExternalTuneScartRGBGain(void *pInstance, MS_U8 u8VSyncTime, APIXC_AdcGainOffsetSetting *pstADCSetting,SCALER_WIN eWindow)
{
    APIXC_AdcGainOffsetSetting stTmpADCSetting, stBackupAdcSetting;
    MS_U16 u16GainDelta = (MDrv_XC_ADC_GetMaximalGainValue(pInstance) + 1) >> 1; // adjust step
    MS_U8 u8ATGStatus = 0 ; // auto gain status
    MS_U8 u8FlowFlag = 0x00; // underflow or overflow flag
    MS_U16 u16ResultR = 0;
    MS_U16 u16ResultG = 0;
    MS_U16 u16ResultB = 0;
    MS_BOOL bResult = FALSE;
    memset(&stTmpADCSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));
    memset(&stBackupAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));

    bResult = MDrv_XC_ADC_GetGainSetting(pInstance, &stBackupAdcSetting);
    if (bResult)
    {
        bResult = MDrv_XC_ADC_GetOffsetSetting(pInstance, &stBackupAdcSetting);
        if (bResult)
        {
            bResult = MDrv_XC_ADC_InitExternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_SCART, &stTmpADCSetting);
            if (bResult)
            {
                while (TRUE)
                {
                    u16GainDelta /= 2; // next
                    MDrv_XC_ADC_SetGainSetting(pInstance, &stTmpADCSetting);
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," Gain R: %d  G: %d  B: %d \n",stTmpADCSetting.u16RedGain,
                            stTmpADCSetting.u16GreenGain,
                            stTmpADCSetting.u16BlueGain);

                    if (u16GainDelta == 0x00) // check end
                    {
                        break;
                    }

                    u16ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_R, u8VSyncTime, MAIN_WINDOW);
                    u16ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_G, u8VSyncTime, MAIN_WINDOW);
                    u16ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_B, u8VSyncTime, MAIN_WINDOW);
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," Result R: %d  G: %d  B: %d \n", u16ResultR,u16ResultG,u16ResultB);

                    // Target is 235 ( 10 bit mode : 235 * 4 = 940 = 0x3AC)
                    if (u16ResultR >= 0x3AC)
                    {
                        u8ATGStatus |= BIT(2);
                    }
                    else
                    {
                        u8ATGStatus &= ~BIT(2);
                    }

                    if (u16ResultG >= 0x3AC)
                    {
                        u8ATGStatus |= BIT(1);
                    }
                    else
                    {
                        u8ATGStatus &= ~BIT(1);
                    }


                    if (u16ResultB >= 0x3AC)
                    {
                        u8ATGStatus |= BIT(0);
                    }
                    else
                    {
                        u8ATGStatus &= ~BIT(0);
                    }

                    //adjust gain
                    // red
                    if (u8ATGStatus & BIT(2))
                    {
                        stTmpADCSetting.u16RedGain -= u16GainDelta;
                        u8FlowFlag |= BIT(0);
                    }
                    else
                    {
                        stTmpADCSetting.u16RedGain += u16GainDelta;
                        u8FlowFlag |= BIT(1);
                    }

                    // green
                    if (u8ATGStatus & BIT(1) )
                    {
                        stTmpADCSetting.u16GreenGain -= u16GainDelta;
                        u8FlowFlag |= BIT(2);
                    }
                    else
                    {
                        stTmpADCSetting.u16GreenGain += u16GainDelta;
                        u8FlowFlag |= BIT(3);
                    }

                    // blue
                    if (u8ATGStatus & BIT(0) )
                    {
                        stTmpADCSetting.u16BlueGain -= u16GainDelta;
                        u8FlowFlag |= BIT(4);
                    }
                    else
                    {
                        stTmpADCSetting.u16BlueGain += u16GainDelta;
                        u8FlowFlag |= BIT(5);
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GainATG=0x%x,FF=0x%x\n", u8ATGStatus, u8FlowFlag);
                } // while

                // calibration failed, restore original value
                if (u8FlowFlag != 0x3f)
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Calibration fail !! \n");
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &stBackupAdcSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &stBackupAdcSetting);
                    _XC_RETURN(pInstance);
                    MApi_XC_ADC_ExitExternalCalibration(pInstance, INPUT_SOURCE_SCART, &stBackupAdcSetting);
                    bResult = FALSE;
                }
                else
                {
                    //adjust gain(+1/-1) after auto-tune
                    u16ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_R, u8VSyncTime, MAIN_WINDOW);
                    u16ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_G, u8VSyncTime, MAIN_WINDOW);
                    u16ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_B, u8VSyncTime, MAIN_WINDOW);
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," Result R: %d  G: %d  B: %d \n", u16ResultR,u16ResultG,u16ResultB);

                    // Target is 235 ( 10 bit mode : 235 * 4 = 940 = 0x3AC)
                    if (u16ResultR < 0x3AC)
                    {
                        stTmpADCSetting.u16RedGain++;
                    }

                    if (u16ResultG < 0x3AC)
                    {
                        stTmpADCSetting.u16GreenGain++;
                    }

                    if (u16ResultB < 0x3AC)
                    {
                        stTmpADCSetting.u16BlueGain++;
                    }

                    MDrv_XC_ADC_SetGainSetting(pInstance, &stTmpADCSetting);

                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Calibration success !! \n");
                    // Because there are variaty between IP1 HW and memory.
                    // Add Gain value on all channel for this variaty (All channel have reach to 0x3AC by reading from memory)
                    // Add Gain value on all channel 26. (We do not know 0~255. The only part we know is 0~235.)
                    pstADCSetting->u16RedGain = stTmpADCSetting.u16RedGain + 0x0220;
                    pstADCSetting->u16GreenGain = stTmpADCSetting.u16GreenGain + 0x0220;
                    pstADCSetting->u16BlueGain = stTmpADCSetting.u16BlueGain + 0x0220;
                    pstADCSetting->u16RedOffset = stTmpADCSetting.u16RedOffset;
                    pstADCSetting->u16GreenOffset = stTmpADCSetting.u16GreenOffset;
                    pstADCSetting->u16BlueOffset = stTmpADCSetting.u16BlueOffset;
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, pstADCSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, pstADCSetting);
                    _XC_RETURN(pInstance);
                    MApi_XC_ADC_ExitExternalCalibration(pInstance, INPUT_SOURCE_SCART, pstADCSetting);
                    bResult = TRUE;
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"External Scart Calibration initial fail !! \n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneScartRGBGain GetOffsetSetting fail!!\n");
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneScartRGBGain GetGainSetting fail!!\n");
    }
    return bResult;
}


//*************************************************************************
//Function name: _MApi_XC_Auto_ColorYUV
//Passing parameter:
//  MS_U8 u8VSyncTime: VSync time
//  APIXC_AdcGainOffsetSetting * pstADCSetting: ADC setting of Current YCbCr mode
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: Auto-tune Y gain of ADC.
//*************************************************************************
#if COMPONENT_AUTO_SW_MODE

static MS_BOOL _MApi_XC_Auto_ExternalTuneComponetYpbprGain(void *pInstance, MS_U8 u8VSyncTime, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    APIXC_AdcGainOffsetSetting tempADCSetting;
    MS_U8 u8ATGStatus =0, u8Tmp, u8TuneStep = 4;
    MS_U16 u8ResultR,u8ResultG,u8ResultB;

    // Using SMPTE 100% or 75% depends on the definition of devAuto.h
    tempADCSetting.u16RedGain = 0x1000;
    tempADCSetting.u16GreenGain = 0x1000;
    tempADCSetting.u16BlueGain = 0x1000;
    tempADCSetting.u16RedOffset = 0x800;
    tempADCSetting.u16GreenOffset =0x100;
    tempADCSetting.u16BlueOffset = 0x800;

    // Tune Gain
    for(u8Tmp=0; u8Tmp<0xFF; u8Tmp++)
    {
        _XC_ENTRY(pInstance);
        MDrv_XC_ADC_SetOffsetSetting(pInstance, &tempADCSetting);
        MDrv_XC_ADC_SetGainSetting(pInstance, &tempADCSetting);
        _XC_RETURN(pInstance);

        MsOS_DelayTask(u8VSyncTime *3 ); // wait stable

        // Get Pr active value (Max. value - Min. value)
        u8ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_R, u8VSyncTime, MAIN_WINDOW);
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Min R: 0x%x ; ", u8ResultR);
        u8ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_R, u8VSyncTime, MAIN_WINDOW) - u8ResultR;
        // Get Y active value (Max. value - Min. value)
        u8ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_G, u8VSyncTime, MAIN_WINDOW);
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Min G: 0x%x ; ", u8ResultG);
        u8ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_G, u8VSyncTime, MAIN_WINDOW) - u8ResultG;
        // Get Pb active value (Max. value - Min. value)
        u8ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_B, u8VSyncTime, MAIN_WINDOW);
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Min B: 0x%x; \n", u8ResultB);
        u8ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_B, u8VSyncTime, MAIN_WINDOW) - u8ResultB;
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Diff R=0x%x, Diff G=0x%x,Diff B=0x%x\n", u8ResultR, u8ResultG, u8ResultB);

        if((u8ResultR >= (PbPr_AUTO_ACTIVE_RANGE<<2)) && (u8ResultR <= ((PbPr_AUTO_ACTIVE_RANGE<<2)+1)))  // Range: 0x10~0xF0
        //if(u8ResultR == (PbPr_AUTO_ACTIVE_RANGE<<2))
        {
            u8ATGStatus |= BIT(4);
        }
        else
        {
            tempADCSetting.u16RedGain += (signed)((PbPr_AUTO_ACTIVE_RANGE<<2)*u8TuneStep - u8ResultR*u8TuneStep);
            u8ATGStatus &= ~BIT(4);
        }

        if( (u8ResultG >= (Y_AUTO_ACTIVE_RANGE<<2)) && (u8ResultG <= ((Y_AUTO_ACTIVE_RANGE<<2)+1)) )  // Range: 0x00~0xDB
        //if(u8ResultG == (Y_AUTO_ACTIVE_RANGE<<2))
        {
            u8ATGStatus |= BIT(3);
        }
        else
        {
            tempADCSetting.u16GreenGain += (signed)((Y_AUTO_ACTIVE_RANGE<<2)*u8TuneStep - u8ResultG*u8TuneStep);
            u8ATGStatus &= ~BIT(3);
        }

        if( (u8ResultB >= (PbPr_AUTO_ACTIVE_RANGE<<2)) && (u8ResultB <= ((PbPr_AUTO_ACTIVE_RANGE<<2)+1)))  // Range: 0x10~0xF0
        //if(u8ResultB == (PbPr_AUTO_ACTIVE_RANGE<<2))
        {
            u8ATGStatus |= BIT(2);
        }
        else
        {
            tempADCSetting.u16BlueGain += (signed)((PbPr_AUTO_ACTIVE_RANGE<<2)*u8TuneStep - u8ResultB*u8TuneStep);
            u8ATGStatus &= ~BIT(2);
        }

        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"u8ATGStatus=0x%x\n", u8ATGStatus);
        if(u8ATGStatus == 0x1C)
            break;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"R gain: 0x%x, G gain: 0x%x, B gain: %x\n",tempADCSetting.u16RedGain, tempADCSetting.u16GreenGain, tempADCSetting.u16BlueGain);

    // Tune Offset
    for(u8Tmp=0; u8Tmp<0xFF; u8Tmp++)
    {
        _XC_ENTRY(pInstance);
        MDrv_XC_ADC_SetOffsetSetting(pInstance, &tempADCSetting);
        MDrv_XC_ADC_SetGainSetting(pInstance, &tempADCSetting);
        _XC_RETURN(pInstance);
        MsOS_DelayTask(u8VSyncTime * 3); // wait stable

        u8ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_R, u8VSyncTime, MAIN_WINDOW);
        u8ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_G, u8VSyncTime, MAIN_WINDOW);
        u8ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_B, u8VSyncTime, MAIN_WINDOW);
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Min R=0x%x,G=0x%x,B=0x%x\n", u8ResultR, u8ResultG, u8ResultB);
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Max R=0x%x,G=0x%x,B=0x%x\n", (MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_R, u8VSyncTime, MAIN_WINDOW)>>2),
            (MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_G, u8VSyncTime, MAIN_WINDOW)>>2), (MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_B, u8VSyncTime, MAIN_WINDOW)>>2));

        // Red - Pr
        if ( (u8ResultR >= (PbPr_AUTO_MIN_VALUE<<2)) && (u8ResultR <= ((PbPr_AUTO_MIN_VALUE<<2)+1)) )    // Range (0x10~0xF0)
        //if ( u8ResultR == (PbPr_AUTO_MIN_VALUE<<2) )
        {
            u8ATGStatus |= BIT(7);
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"old R offset: 0x%x\n", tempADCSetting.u16RedOffset);
            tempADCSetting.u16RedOffset += (signed)((PbPr_AUTO_MIN_VALUE<<2) - (u8ResultR));
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"new R offset: 0x%x\n", tempADCSetting.u16RedOffset);
            u8ATGStatus &= ~BIT(7);
        }

        // Green - Y
        if ( (u8ResultG >= (Y_MIN_VALUE<<2)) && (u8ResultG <= ((Y_MIN_VALUE<<2)+1)) ) // Range (0x10~0xEB)
        //if ( u8ResultG == (Y_MIN_VALUE<<2) )
        {
            u8ATGStatus |= BIT(6);
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"old G offset: 0x%x\n", tempADCSetting.u16GreenOffset);
            tempADCSetting.u16GreenOffset += (signed)((Y_MIN_VALUE<<2) - (u8ResultG));
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"new G offset: 0x%x\n", tempADCSetting.u16GreenOffset);
            u8ATGStatus &= ~BIT(6);
        }

        // Blue - Pb
        if ( (u8ResultB >= (PbPr_AUTO_MIN_VALUE<<2)) && (u8ResultB <= ((PbPr_AUTO_MIN_VALUE<<2)+1)) )    // Range (0x10~0xF0)
        //if ( u8ResultB == (PbPr_AUTO_MIN_VALUE<<2) )
        {
            u8ATGStatus |= BIT(5);
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"old B offset: 0x%x\n",tempADCSetting.u16BlueOffset);
            tempADCSetting.u16BlueOffset += (signed)((PbPr_AUTO_MIN_VALUE<<2) - (u8ResultB));
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"new B offset: 0x%x\n",tempADCSetting.u16BlueOffset);
            u8ATGStatus &= ~BIT(5);
        }

        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"u8ATGStatus =0x%x\n", u8ATGStatus);

        if(u8ATGStatus == 0xFC)
            break;
    }

    if(u8ATGStatus == 0xFC)
    {
        // succssed, copy setting to user
        pstADCSetting->u16RedGain = tempADCSetting.u16RedGain;
        pstADCSetting->u16GreenGain = tempADCSetting.u16GreenGain;
        pstADCSetting->u16BlueGain = tempADCSetting.u16BlueGain;
        pstADCSetting->u16RedOffset = tempADCSetting.u16RedOffset;
        pstADCSetting->u16GreenOffset = tempADCSetting.u16GreenOffset;
        pstADCSetting->u16BlueOffset = tempADCSetting.u16BlueOffset;
    }
    else
    {
        _XC_ENTRY(pInstance);
        MDrv_XC_ADC_SetOffsetSetting(pInstance, pstADCSetting);
        MDrv_XC_ADC_SetGainSetting(pInstance, pstADCSetting);
        _XC_RETURN(pInstance);
    }

    return (u8ATGStatus == 0xFC ? TRUE : FALSE);

}

#if 0
static MS_BOOL _MApi_XC_Auto_ColorYUV(MS_U8 u8VSyncTime, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    APIXC_AdcGainOffsetSetting tempADCSetting;
    MS_U8 u8ATGStatus, u8Tmp, u8FlowFlag;
    MS_U8 u8ResultR,u8ResultG,u8ResultB;

    // Using SMPTE 100% or 75% depends on the definition of devAuto.h
    tempADCSetting.u16RedGain = 0x30;
    tempADCSetting.u16GreenGain = 0x30;
    tempADCSetting.u16BlueGain = 0x30;
    tempADCSetting.u16RedOffset = 0x80;
    tempADCSetting.u16GreenOffset =0x80;
    tempADCSetting.u16BlueOffset = 0x80;

    MApi_XC_ADC_AutoSetting(pInstance, DISABLE, TRUE, COMPONENT_AUTO_SW_MODE);

    u8ATGStatus = 0;
    u8FlowFlag = 0;

    // Tune offset
    for(u8Tmp=0; u8Tmp<0xFF; u8Tmp++)
    {
        //printf("u8Tmp=0x%bx\n", u8Tmp);
        MApi_XC_ADC_AdjustGainOffset(&tempADCSetting);

        MsOS_DelayTask(u8VSyncTime * 3); // wait stable

        u8ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_R, u8VSyncTime, MAIN_WINDOW);
        u8ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_G, u8VSyncTime, MAIN_WINDOW);
        u8ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MIN_B, u8VSyncTime, MAIN_WINDOW);

        if ((u8ResultR >= PbPr_AUTO_MIN_VALUE) && (u8ResultR <= (PbPr_AUTO_MIN_VALUE+1)))    // Range (0x10~0xF0)
            u8ATGStatus |= BIT(7);
        else
            u8ATGStatus &= ~BIT(7);

        if ((u8ResultG >= (Y_AUTO_MIN_VALUE+1)) && (u8ResultG <= (Y_AUTO_MIN_VALUE+2))) // Range (0x00~0xDB)
            u8ATGStatus |= BIT(6);
        else
            u8ATGStatus &= ~BIT(6);

        if ((u8ResultB >= PbPr_AUTO_MIN_VALUE) && (u8ResultB <= (PbPr_AUTO_MIN_VALUE+1)))    // Range (0x10~0xF0)
            u8ATGStatus |= BIT(5);
        else
            u8ATGStatus &= ~BIT(5);

        // Red - Pr
        if ( !(u8FlowFlag&BIT(0)) )
        {
            if (_bit7_(u8ATGStatus))
            {
                // do nothing to RedOffset
                u8FlowFlag |= BIT(0);
            }
            else
            {
                tempADCSetting.u16RedOffset += (signed)(PbPr_AUTO_MIN_VALUE - u8ResultR);
            }
        }

        // Green - Y
        if ( !(u8FlowFlag&BIT(2)) )
        {
            if (_bit6_(u8ATGStatus))
            {
                // do nothing to GreenOffset
                u8FlowFlag |= BIT(2);
            }
            else
            {
                tempADCSetting.u16GreenOffset += (signed)(Y_AUTO_MIN_VALUE+1 - u8ResultG);
            }
        }

        // Blue - Pb
        if ( !(u8FlowFlag&BIT(4)) )
        {
            if (_bit5_(u8ATGStatus))
            {
                // do nothing to BlueOffset
                u8FlowFlag |= BIT(4);
            }
            else
            {
                tempADCSetting.u16BlueOffset += (signed)(PbPr_AUTO_MIN_VALUE - u8ResultB);
            }
        }

        //printf("ATG=0x%bx, FF=0x%bx, Min R=0x%bx,G=0x%bx,B=0x%bx\n", u8ATGStatus, u8FlowFlag, u8ResultR, u8ResultG, u8ResultG);

        // Get Pr active value (Max. value - Min. value)
        u8ResultR = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_R, u8VSyncTime, MAIN_WINDOW)-u8ResultR;
        // Get Y active value (Max. value - Min. value)
        u8ResultG = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_G, u8VSyncTime, MAIN_WINDOW)-u8ResultG;
        // Get Pb active value (Max. value - Min. value)
        u8ResultB = MApi_XC_GetAutoGainResult(pInstance, AUTO_MAX_B, u8VSyncTime, MAIN_WINDOW)-u8ResultB;

        if((u8ResultR >= PbPr_AUTO_ACTIVE_RANGE) && (u8ResultR <= (PbPr_AUTO_ACTIVE_RANGE+1)))  // Range: 0x10~0xF0
        {
            u8ATGStatus |= BIT(4);
        }
        else
        {
            u8FlowFlag &= ~(BIT(1)+BIT(0));
            u8ATGStatus &= ~BIT(4);
        }

        if((u8ResultG >= Y_AUTO_ACTIVE_RANGE) && (u8ResultG <= (Y_AUTO_ACTIVE_RANGE+1)))  // Range: 0x00~0xDB
        {
            u8ATGStatus |= BIT(3);
        }
        else
        {
            u8FlowFlag &= ~(BIT(3)+BIT(2));
            u8ATGStatus &= ~BIT(3);
        }

        if((u8ResultB >= PbPr_AUTO_ACTIVE_RANGE) && (u8ResultB <= (PbPr_AUTO_ACTIVE_RANGE+1)))  // Range: 0x10~0xF0
        {
            u8ATGStatus |= BIT(2);
        }
        else
        {
            u8FlowFlag &= ~(BIT(5)+BIT(4));
            u8ATGStatus &= ~BIT(2);
        }

        // Red - Pr
        if ( !(u8FlowFlag&BIT(1)) )
        {
            if (_bit4_(u8ATGStatus))
            {
                u8FlowFlag |= BIT(1);
            }
            else
            {
                tempADCSetting.u16RedGain += (signed)(PbPr_AUTO_ACTIVE_RANGE - u8ResultR);
                u8FlowFlag &= ~BIT(1);
            }
        }

        // Green - Y
        if ( !(u8FlowFlag&BIT(3)) )
        {
            if (_bit3_(u8ATGStatus))
            {
                u8FlowFlag |= BIT(3);
            }
            else
            {
                tempADCSetting.u16GreenGain += (signed)(Y_AUTO_ACTIVE_RANGE - u8ResultG);
                u8FlowFlag &= ~BIT(3);
            }
        }

        // Blue - Pb
        if ( !(u8FlowFlag&BIT(5)) )
        {
            if (_bit2_(u8ATGStatus))
            {
                u8FlowFlag |= BIT(5);
            }
            else
            {
                tempADCSetting.u16BlueGain += (signed)(PbPr_AUTO_ACTIVE_RANGE - u8ResultB);
                u8FlowFlag &= ~BIT(5);
            }
        }

        //printf("ATG=0x%bx, FF=0x%bx, Max R=0x%bx,G=0x%bx,B=0x%bx\n", u8ATGStatus, u8FlowFlag, u8ResultR, u8ResultG, u8ResultG);

        if(u8FlowFlag == 0x3F)
            break;
    }

    MsOS_DelayTask(u8VSyncTime * 2);



    //printf("FF=0x%bx, Gain R=0x%bx,G=0x%bx,B=0x%bx\n", u8FlowFlag,
    //    ~tempADCSetting.u16RedGain, ~tempADCSetting.u16GreenGain, ~tempADCSetting.u16BlueGain);
    //printf("FF=0x%bx, Offset R=0x%bx,G=0x%bx,B=0x%bx\n", u8FlowFlag,
    //    ~tempADCSetting.u16RedOffset, ~tempADCSetting.u16GreenOffset, ~tempADCSetting.u16BlueOffset);


    if(u8FlowFlag == 0x3F)
    {
        // succssed, copy setting to user
        pstADCSetting->u16RedGain = tempADCSetting.u16RedGain;
        pstADCSetting->u16GreenGain = tempADCSetting.u16GreenGain;
        pstADCSetting->u16BlueGain = tempADCSetting.u16BlueGain;
        pstADCSetting->u16RedOffset = tempADCSetting.u16RedOffset;
        pstADCSetting->u16GreenOffset = tempADCSetting.u16GreenOffset;
        pstADCSetting->u16BlueOffset = tempADCSetting.u16BlueOffset;
    }
    else
    {
        MApi_XC_ADC_AdjustGainOffset(pstADCSetting);
        MApi_XC_ADC_AdjustGainOffset(pstADCSetting);
    }

    return (u8FlowFlag == 0x3F ? TRUE : FALSE);
}
#endif


#else

static MS_BOOL _MApi_XC_Auto_ExternalTuneComponetYpbprGain(void *pInstance, MS_U8 u8VSyncTime, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    APIXC_AdcGainOffsetSetting tempADCSetting, stBackupAdcSetting;
    MS_U8 u8ATGStatus;
    MS_U16 u16GainDelta = (MDrv_XC_ADC_GetMaximalGainValue(pInstance) + 1) >> 1;
    MS_U16 i = 0;
    MS_U8 u8FlowFlag = 0; // Add the initial value
    MS_BOOL bResult = FALSE;
    memset(&tempADCSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));
    memset(&stBackupAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));

    bResult = MDrv_XC_ADC_GetGainSetting(pInstance, &stBackupAdcSetting);
    if (bResult)
    {
        bResult = MDrv_XC_ADC_GetOffsetSetting(pInstance, &stBackupAdcSetting);
        if (bResult)
        {
            // Initial
            bResult = MDrv_XC_ADC_InitExternalCalibrationWithFixedOffset(pInstance, INPUT_SOURCE_YPBPR, &tempADCSetting);
            if (bResult)
            {
                MApi_XC_ADC_AutoSetting(pInstance, ENABLE, TRUE, COMPONENT_AUTO_SW_MODE);

                while(TRUE)
                {
                    u16GainDelta >>= 1;
                    if(u16GainDelta == 0x00)
                        break;

                    // Update gain.
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &tempADCSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &tempADCSetting);
                    _XC_RETURN(pInstance);
                    ///MApi_XC_ADC_AutoSetting(pInstance, ENABLE, TRUE, COMPONENT_AUTO_SW_MODE);
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"RGain %x \n", tempADCSetting.u16RedGain );
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GGain %x \n", tempADCSetting.u16GreenGain );
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"BGain %x \n", tempADCSetting.u16BlueGain );
                    MsOS_DelayTask(u8VSyncTime * 3); // wait stable
                    _MApi_XC_Auto_WaitStatusReady(pInstance, E_WAITING_AUTO_GAIN_READY, MAIN_WINDOW);
                    u8ATGStatus = MApi_XC_GetAutoGainMaxValueStatus(pInstance, MAIN_WINDOW); // get auto gain status

                    // Red - Pr
                    if ( u8ATGStatus & BIT(2) )
                    {
                        tempADCSetting.u16RedGain -= u16GainDelta;
                        u8FlowFlag |= BIT(0);
                    }
                    else
                    {
                        tempADCSetting.u16RedGain += u16GainDelta;
                        u8FlowFlag |= BIT(1);
                    }

                    // Green - Y
                    if ( u8ATGStatus & BIT(1) )
                    {
                        tempADCSetting.u16GreenGain -= u16GainDelta;
                        u8FlowFlag |= BIT(2);
                    }
                    else
                    {
                        tempADCSetting.u16GreenGain += u16GainDelta;
                        u8FlowFlag |= BIT(3);
                    }

                    // Blue - Pb
                    if (u8ATGStatus & BIT(0) )
                    {
                        tempADCSetting.u16BlueGain -= u16GainDelta;
                        u8FlowFlag |= BIT(4);
                    }
                    else
                    {
                        tempADCSetting.u16BlueGain += u16GainDelta;
                        u8FlowFlag |= BIT(5);
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"u8FlowFlag %x\n",u8FlowFlag);
                }

                u8FlowFlag = 0;

                //Decrease the gain setting and fine tune it
                if(tempADCSetting.u16RedGain > 10)
                {
                    tempADCSetting.u16RedGain -= 10;
                }
                else
                {
                    tempADCSetting.u16RedGain = 0;
                }

                if(tempADCSetting.u16GreenGain > 10)
                {
                    tempADCSetting.u16GreenGain -= 10;
                }
                else
                {
                    tempADCSetting.u16GreenGain = 0;
                }

                if(tempADCSetting.u16BlueGain > 10)
                {
                    tempADCSetting.u16BlueGain -= 10;
                }
                else
                {
                    tempADCSetting.u16BlueGain = 0;
                }

                u16GainDelta =  MDrv_XC_ADC_GetMaximalGainValue(pInstance);
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY," fine tune\n");
                for(i=0; i<u16GainDelta; i++)
                {
                    // Update gain.
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &tempADCSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &tempADCSetting);
                    _XC_RETURN(pInstance);

                    //MApi_XC_ADC_AutoSetting(pInstance, ENABLE, TRUE, COMPONENT_AUTO_SW_MODE);
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"RGain %x \n", tempADCSetting.u16RedGain );
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"GGain %x\n", tempADCSetting.u16GreenGain );
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"BGain %x \n", tempADCSetting.u16BlueGain );
                    MsOS_DelayTask(u8VSyncTime * 3); // wait stable
                    _MApi_XC_Auto_WaitStatusReady(pInstance, E_WAITING_AUTO_GAIN_READY, MAIN_WINDOW);

                    u8ATGStatus = MApi_XC_GetAutoGainMaxValueStatus(pInstance, MAIN_WINDOW); // get auto gain status

                    // Red - Pr
                    if ((u8FlowFlag&(BIT(0)|BIT(1))) != (BIT(0)|BIT(1)))
                    {
                        if(u8ATGStatus & BIT(2))
                        {
                            u8FlowFlag |= BIT(0);
                            if ((u8FlowFlag&BIT(1)) != BIT(1))
                            {
                                tempADCSetting.u16RedGain -= AUTO_YUV_GAIN_STEP;
                                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"R-\n");
                            }
                        }
                        else
                        {
                            tempADCSetting.u16RedGain += AUTO_YUV_GAIN_STEP;
                            u8FlowFlag |= BIT(1);
                            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"R+\n");
                        }
                    }

                    // Green - Y
                    if((u8FlowFlag&(BIT(2)|BIT(3))) != (BIT(2)|BIT(3)))
                    {
                        if(u8ATGStatus & BIT(1))
                        {
                            u8FlowFlag |= BIT(2);
                            if ((u8FlowFlag&BIT(3)) != BIT(3))
                            {
                                tempADCSetting.u16GreenGain -= AUTO_YUV_GAIN_STEP;
                                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"G+\n");
                            }
                        }
                        else
                        {
                            tempADCSetting.u16GreenGain += AUTO_YUV_GAIN_STEP;
                            u8FlowFlag |= BIT(3);
                            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"G-\n");
                        }
                    }

                    // Blue - Pb
                    if((u8FlowFlag&(BIT(4)|BIT(5))) != (BIT(4)|BIT(5)))
                    {
                        if(u8ATGStatus & BIT(0) )
                        {
                            u8FlowFlag |= BIT(4);
                            if ((u8FlowFlag&BIT(5)) != BIT(5))
                            {
                                tempADCSetting.u16BlueGain -= AUTO_YUV_GAIN_STEP;
                                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"B+\n");
                            }
                        }
                        else
                        {
                            tempADCSetting.u16BlueGain += AUTO_YUV_GAIN_STEP;
                            u8FlowFlag |= BIT(5);
                            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"B-\n");
                        }
                    }

                    if (u8FlowFlag == 0x3F || i > AUTO_YUV_MAX_CNT)
                    {
                        break;
                    }
                }

                // adjust gain after auto-tune
                if (u8FlowFlag == 0x3f)
                {
                    // successed
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"success\n");
                    pstADCSetting->u16RedGain = tempADCSetting.u16RedGain;
                    pstADCSetting->u16GreenGain = tempADCSetting.u16GreenGain;
                    pstADCSetting->u16BlueGain = tempADCSetting.u16BlueGain;
                    pstADCSetting->u16RedOffset = tempADCSetting.u16RedOffset;
                    pstADCSetting->u16GreenOffset = tempADCSetting.u16GreenOffset;
                    pstADCSetting->u16BlueOffset = tempADCSetting.u16BlueOffset;
                    bResult = TRUE;
                }
                else
                {
                    // Restore original setting.
                    _XC_ENTRY(pInstance);
                    MDrv_XC_ADC_SetOffsetSetting(pInstance, &stBackupAdcSetting);
                    MDrv_XC_ADC_SetGainSetting(pInstance, &stBackupAdcSetting);
                    _XC_RETURN(pInstance);
                    bResult = FALSE;
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"External YPbPr Calibration initial fail !! \n");
            }
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneComponetYpbprGain GetOffsetSetting fail!!\n");
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"ExternalTuneComponetYpbprGain GetGainSetting fail!!\n");
    }
    return bResult;
}


#endif

static MS_BOOL _MApi_XC_Auto_External_AutoCalibration(void *pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting,SCALER_WIN eWindow)
{
    MS_BOOL bAutoResult = TRUE;
    MS_U8 u8VSyncTime; // VSync time
    XC_IP_SYNC_STATUS stXC_Sync_Status = {0, 0, 0};

    _MApi_XC_Auto_SetCalibrartinWindow(pInstance, ENABLE,eWindow);
    MApi_XC_AutoGainEnable(pInstance,ENABLE,eWindow);

    _XC_ENTRY(pInstance);
    MDrv_XC_GetSyncStatus(pInstance, INPUT_SOURCE_SCART, &stXC_Sync_Status, eWindow);
    _XC_RETURN(pInstance);

    //for scart pcmonitor can't give vsync time, so we calculate it ourselves
    u8VSyncTime = MApi_XC_CalculateVsyncTime(pInstance, stXC_Sync_Status.u16Vtotal, stXC_Sync_Status.u16Hperiod); // get VSync time

    // auto RGB offset
    if(enAutoTuneType & E_XC_AUTO_TUNE_RGB_OFFSET)
    {
        if (!MDrv_XC_ADC_use_hardware_auto_offset(pInstance, INPUT_SOURCE_VGA))
        {
            // Run software tune offset
            MDrv_XC_ADC_Source_Calibrate(pInstance,  MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance,  INPUT_SOURCE_VGA));
            if(_MApi_XC_Auto_ExternalTuneVgaOffset(pInstance, u8VSyncTime, pstADCSetting) == FALSE)
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoOffset failed\n");
                //enAutoTuneType = E_XC_AUTO_TUNE_NULL;
                bAutoResult = FALSE;
            }
        }
        else
        {
            APIXC_AdcGainOffsetSetting stTmpAdcSetting;
            memset(&stTmpAdcSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));

            // Using HW offset, so use default offset value.
            MDrv_XC_ADC_GetDefaultGainOffset(pInstance, INPUT_SOURCE_VGA, &stTmpAdcSetting);
            pstADCSetting->u16RedOffset = stTmpAdcSetting.u16RedOffset;
            pstADCSetting->u16GreenOffset = stTmpAdcSetting.u16GreenOffset;
            pstADCSetting->u16BlueOffset = stTmpAdcSetting.u16BlueOffset;
        }
    } // auto-tune ADC offset


    // auto RGB gain
    if(enAutoTuneType & E_XC_AUTO_TUNE_RGB_GAIN)
    {
        MDrv_XC_ADC_Source_Calibrate(pInstance,  MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance,  INPUT_SOURCE_VGA));
        if(_MApi_XC_Auto_ExternalTuneVgaRGBGain(pInstance, u8VSyncTime,pstADCSetting,eWindow) == FALSE)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoGain failed\n");
            //enAutoTuneType = E_XC_AUTO_TUNE_NULL;
            bAutoResult = FALSE;
        }
    } // auto-tune ADC gain


    // auto Scart RGB gain
    if(enAutoTuneType & E_XC_AUTO_TUNE_SCART_RGB_GAIN)
    {
        MDrv_XC_ADC_Source_Calibrate(pInstance,  MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance,  INPUT_SOURCE_SCART));
        if(_MApi_XC_Auto_ExternalTuneScartRGBGain(pInstance, u8VSyncTime,pstADCSetting,eWindow) == FALSE)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoGain failed\n");
            //enAutoTuneType = E_XC_AUTO_TUNE_NULL;
            bAutoResult = FALSE;
        }
    } // auto-tune ADC gain

    if(enAutoTuneType & E_XC_AUTO_TUNE_YUV_COLOR)
    {
        MDrv_XC_ADC_Source_Calibrate(pInstance,  MDrv_XC_ADC_ConvertSrcToADCSrc(pInstance, INPUT_SOURCE_YPBPR));
        if (_MApi_XC_Auto_ExternalTuneComponetYpbprGain(pInstance, u8VSyncTime, pstADCSetting) == FALSE)
        {
            //enAutoTuneType = E_XC_AUTO_TUNE_NULL;
            bAutoResult = FALSE;
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoColorYUV failed\n");
        }
    } // auto-tune ADC gain

    // disable auto gain function
    MApi_XC_AutoGainEnable(pInstance,DISABLE,eWindow);
    _MApi_XC_Auto_SetCalibrartinWindow(pInstance, DISABLE,eWindow);

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Offset R=0x%x,G=0x%x,B=0x%x\n",
                   pstADCSetting->u16RedOffset,
                   pstADCSetting->u16GreenOffset,
                   pstADCSetting->u16BlueOffset);
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Gain R=0x%x,G=0x%x,B=0x%x\n",
                   pstADCSetting->u16RedGain,
                   pstADCSetting->u16GreenGain,
                   pstADCSetting->u16BlueGain);
    return bAutoResult;
}

//
// The reason to wait stable here is some pattern gen such as MSPG-1025D,
// when change timing of RGB, the pattern gen will send out H/V sync first,
// then RGB signal later. (This is not measured by the scope. If pause the program,
// change pattern gen timing, the video come out after around 5 seconds.
// If used MSPG-925FS, change pattern gen timing, the video come out
// within 1 second.) So, we've to wait auto-position H_Start, H_End avaliable and then proceed
// RGB auto function, otherwise, the auto function will be failed because of using the wrong auto-position
// H_Start, H_End.
//
// When waiting the H_Start, H_End to be stable, there are some cases:
// (1) H_Start is 0x7ff, H_Size is 0x00: -> un-stable
// (2) H_Start != 0x7ff and H_Size != 0x00, but H_Start == H_Size: -> un-stable
// (3) H_Start != 0x7ff and H_Size != 0x00, and H_Start != H_Size, but H_Start is only about H_Size -1 or -2: -> un-stable
static MS_BOOL _MApi_XC_Auto_waitstable(void *pInstance, MS_U8 u8VSyncTime, MS_U16 u16WaitCnt, SCALER_WIN eWindow)
{
    MS_U16 i;
    MS_U16 u16HStart, u16HEnd;
    MS_WINDOW_TYPE sWin;
    memset(&sWin, 0, sizeof(MS_WINDOW_TYPE));

    // use a short delay for stable

    for(i=0; i<u16WaitCnt; i++)
    {
        MsOS_DelayTask(u8VSyncTime);

        _XC_ENTRY(pInstance);
        MDrv_XC_GetDEWindow(pInstance, &sWin, eWindow);
        _XC_RETURN(pInstance);

        u16HStart = sWin.x;
        u16HEnd = sWin.x + sWin.width;

        if(((u16HStart != 0x7FF) && (u16HEnd != 0x000)) ||
            (u16HStart < u16HEnd))
        {
            if(abs(u16HStart - u16HEnd) > 10)
            {
                break;
            }
        }
    }

    return (i == u16WaitCnt) ? FALSE : TRUE;
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

MS_BOOL MApi_XC_Auto_GainOffset_U2(void* pInstance, XC_Auto_CalibrationType type, XC_Auto_TuneType enAutoTuneType,  APIXC_AdcGainOffsetSetting *pstADCSetting, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_BOOL bReturn = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
    MS_BOOL bIsGetCMA = FALSE;
    if((eWindow==MAIN_WINDOW)&& (E_XC_SW_CALIBRATION == pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode))
    {
        if (  ((gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_SELF_MEM]+gSrcInfo[MAIN_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_SELF_MEM]) == 0)
            &&((gSrcInfo[MAIN_WINDOW].u32PreCMAMemSCMSize[CMA_XC_COBUFF_MEM]+gSrcInfo[MAIN_WINDOW].u32PreCMAMemFRCMSize[CMA_XC_COBUFF_MEM]) == 0)
            )
        {
            MS_BOOL bIsGetCMA[CMA_XC_MEM_NUM] = {FALSE, };
            MS_U32 au32CMAMemSCMSize[CMA_XC_MEM_NUM] = {0, };
            MS_U32 au32CMAMemFRCMSize[CMA_XC_MEM_NUM] = {0, };
            MS_U32 u32DualMiuMemSize = 0;
            MS_U32 u32CMAAddr = 0;
            XC_CMA_CLIENT enCmaClient = CMA_XC_MEM_NUM;

            MHal_XC_Get_CMA_UsingCondition(pInstance, bIsGetCMA, au32CMAMemSCMSize, au32CMAMemFRCMSize, &u32DualMiuMemSize, CMA_XC_MEM_NUM, eWindow);

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d][CMA] auto calibration bIsGetCMA[self]=%d bIsGetCMA[cobuff]=%d \n",__func__,__LINE__,bIsGetCMA[CMA_XC_SELF_MEM],bIsGetCMA[CMA_XC_COBUFF_MEM]);
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d][CMA] auto calibration au32CMAMemSCMSize[self]=%x au32CMAMemSCMSize[cobuff]=%x \n",__func__,__LINE__,au32CMAMemSCMSize[CMA_XC_SELF_MEM],au32CMAMemSCMSize[CMA_XC_COBUFF_MEM]);

            MHal_XC_CMAPatchClose(pInstance, MAIN_WINDOW);
            if (bIsGetCMA[CMA_XC_SELF_MEM] == TRUE)
            {
                enCmaClient = CMA_XC_SELF_MEM;
                u32CMAAddr = MHal_XC_Get_CMA_Addr(pInstance, enCmaClient,  au32CMAMemSCMSize[enCmaClient]+au32CMAMemFRCMSize[enCmaClient], eWindow);
            }
            else if (bIsGetCMA[CMA_XC_COBUFF_MEM] == TRUE)
            {
                enCmaClient = CMA_XC_COBUFF_MEM;
                u32CMAAddr = MHal_XC_Get_CMA_Addr(pInstance, enCmaClient,  au32CMAMemSCMSize[enCmaClient]+au32CMAMemFRCMSize[enCmaClient], eWindow);
            }
            else
            {
                printf("\033[35m   [CMA] Function = %s, Line = %d, CMA_POOL_INIT GetMem ERROR!!\033[m\n", __PRETTY_FUNCTION__, __LINE__);
                while(1);
            }

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d][CMA] auto calibration u32CMAAddr=%x \n",__func__,__LINE__,u32CMAAddr);

            gSrcInfo[eWindow].u32PreCMAMemSCMSize[enCmaClient] = au32CMAMemSCMSize[enCmaClient];
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr = (MS_U32)u32CMAAddr;
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size = gSrcInfo[eWindow].u32PreCMAMemSCMSize[enCmaClient];//48MB

            // default frame buffer address init
            MDrv_XC_SetFrameBufferAddress(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size, MAIN_WINDOW);

            Hal_SC_IP_Memory_Write_Request(pInstance, ENABLE, MAIN_WINDOW);
#if (HW_DESIGN_4K2K_VER == 4)
            Hal_SC_frcmw_Memory_Write_Request(pInstance, ENABLE, MAIN_WINDOW);
#endif
            Hal_SC_set_freezeimg(pInstance, DISABLE, MAIN_WINDOW);
        }
    }
#endif
#endif
    if(type == E_XC_INTERNAL_CALIBRATION)
    {
        if (E_XC_SW_CALIBRATION == pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode)
        {
            MDrv_XC_ADC_EnableHWCalibration(pInstance, DISABLE);
            bReturn = ( _MApi_XC_Auto_Internal_AutoCablication(pInstance, enAutoTuneType ,pstADCSetting,eWindow)) ;
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
            if(eWindow==MAIN_WINDOW && bIsGetCMA)
                MHal_XC_CMAPatch(pInstance, MAIN_WINDOW);
#endif
#endif
            return bReturn;
        }
        else
        {
            MDrv_XC_ADC_EnableHWCalibration(pInstance, ENABLE);
            bReturn = MDrv_XC_Auto_GetHWFixedGainOffset(pInstance, enAutoTuneType, pstADCSetting);
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return bReturn;
        }
    }
    else
    {
        if (E_XC_SW_CALIBRATION == pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode)
        {
#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
            // autonosignal will disable input source when ypbpr source do EXTERNAL CALIBRATION and SW CALIBRATION
            if(   eWindow==MAIN_WINDOW
               && gSrcInfo[eWindow].bAutoNoSignalEnabled
               && IsSrcTypeYPbPr( gSrcInfo[eWindow].enInputSourceType ))
            {
                MHal_XC_CMAPatchClose(pInstance, MAIN_WINDOW);
                bNeedDeattachCMAISR = TRUE;
            }
#endif
#endif
            MDrv_XC_ADC_EnableHWCalibration(pInstance, DISABLE);
            bReturn = ( _MApi_XC_Auto_External_AutoCalibration(pInstance, enAutoTuneType ,pstADCSetting,eWindow)) ;
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return bReturn;
        }
        else
        {
            // Not support HW mode + external calibration now
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return FALSE;
        }

    }
}

MS_BOOL MApi_XC_Auto_GainOffset(XC_Auto_CalibrationType type, XC_Auto_TuneType enAutoTuneType,  APIXC_AdcGainOffsetSetting *pstADCSetting, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }
    static APIXC_AdcGainOffsetSetting tmp_pstADCSetting;
    memcpy(&tmp_pstADCSetting,pstADCSetting,sizeof(APIXC_AdcGainOffsetSetting));
    stXC_AUTO_GAINOFFSET XCArgs;
    XCArgs.type = type;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.pstADCSetting = &tmp_pstADCSetting;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_GAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        memcpy(pstADCSetting,&tmp_pstADCSetting,sizeof(APIXC_AdcGainOffsetSetting));
        return XCArgs.bReturnValue;
    }
}


MS_BOOL MDrv_XC_Auto_GetHWFixedGainOffset(void *pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
{
    MS_BOOL bResult = FALSE;
    INPUT_SOURCE_TYPE_t enInputSrc = INPUT_SOURCE_NONE;
    APIXC_AdcGainOffsetSetting tempADCSetting;
    memset(&tempADCSetting, 0, sizeof(APIXC_AdcGainOffsetSetting));
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"======== Start : HW Calibration Type: %x ======== \n",enAutoTuneType);

    if ((E_XC_AUTO_TUNE_NULL == enAutoTuneType) || (NULL == pstADCSetting))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"======== HW Calibration Error Parameter: enAutoTuneType = %x, (pstADCSetting == NULL) = %u ======== \n",enAutoTuneType, (NULL == pstADCSetting));
        return FALSE;
    }

    if (MDrv_XC_ADC_CheckHWCalibrationSupport(pInstance))
    {
        if (enAutoTuneType & E_XC_AUTO_TUNE_RGB_GAIN)
        {
            enInputSrc = INPUT_SOURCE_VGA;
            bResult = TRUE;
        }
        else if (enAutoTuneType & E_XC_AUTO_TUNE_YUV_COLOR)
        {
            enInputSrc = INPUT_SOURCE_YPBPR;
            bResult = TRUE;
        }
        else if (enAutoTuneType & E_XC_AUTO_TUNE_SCART_RGB_GAIN)
        {
            enInputSrc = INPUT_SOURCE_SCART;
            bResult = TRUE;
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Unsupported AutoTuneType: %x ======== \n",enAutoTuneType);
            bResult = FALSE;
        }

        if (bResult)
        {
            bResult = MDrv_ADC_GetFixedOffset(pInstance, enInputSrc, &tempADCSetting);
            if (bResult)
            {
                bResult = MDrv_XC_ADC_GetFixedGain(pInstance, enInputSrc, &tempADCSetting);
                if (bResult)
                {
                    pstADCSetting->u16RedGain = tempADCSetting.u16RedGain;
                    pstADCSetting->u16GreenGain = tempADCSetting.u16GreenGain;
                    pstADCSetting->u16BlueGain = tempADCSetting.u16BlueGain;

                    pstADCSetting->u16RedOffset = tempADCSetting.u16RedOffset;
                    pstADCSetting->u16GreenOffset = tempADCSetting.u16GreenOffset;
                    pstADCSetting->u16BlueOffset = tempADCSetting.u16BlueOffset;
                }
                else
                {
                    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"****** get initial gain fail!!! ******\n");
                }
            }
            else
            {
                XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"****** get initial offset fail!!! ******\n");
            }
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"****** Unsupport HW Calibration ******\n");
        bResult = FALSE;
    }
    return bResult;
}
}

MS_BOOL MApi_XC_Auto_GetHWFixedGainOffset_U2(void* pInstance, XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    return MDrv_XC_Auto_GetHWFixedGainOffset(pInstance, enAutoTuneType, pstADCSetting);
}

MS_BOOL MApi_XC_Auto_GetHWFixedGainOffset(XC_Auto_TuneType enAutoTuneType, APIXC_AdcGainOffsetSetting *pstADCSetting)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_GETHWFIXEDGAINOFFSET XCArgs;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.pstADCSetting = pstADCSetting;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_GETHWFIXEDGAINOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


//*************************************************************************
//Function name: MApi_XC_Auto_Geometry
//Passing parameter:
//  enAutoTuneType: Auto function select
//  ActiveInfo: Current PC mode setting
//  StandardInfo: Standard mode. (This is singal information of stardard)
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: Auto-tune for PC mode
//*************************************************************************
MS_BOOL MApi_XC_Auto_Geometry_U2(void* pInstance, XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info *ActiveInfo, XC_Auto_Signal_Info *StandardInfo , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_Auto_Signal_Info_Ex ActiveInfoEx, StandardInfoEx;
    MS_BOOL bResult;

    ActiveInfoEx.u16HorizontalStart = ActiveInfo->u16HorizontalStart;
    ActiveInfoEx.u16VerticalStart = ActiveInfo->u16VerticalStart;
    ActiveInfoEx.u16HorizontalTotal = ActiveInfo->u16HorizontalTotal;
    ActiveInfoEx.u16HResolution = ActiveInfo->u16HResolution;
    ActiveInfoEx.u16Phase = (MS_U16)ActiveInfo->u8Phase;
    ActiveInfoEx.u16Version = 0;

    StandardInfoEx.u16HorizontalStart = StandardInfo->u16HorizontalStart;
    StandardInfoEx.u16VerticalStart = StandardInfo->u16VerticalStart;
    StandardInfoEx.u16HorizontalTotal = StandardInfo->u16HorizontalTotal;
    StandardInfoEx.u16HResolution = StandardInfo->u16HResolution;
    StandardInfoEx.u16Phase = (MS_U16)StandardInfo->u8Phase;
    StandardInfoEx.u16Version= 0;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    bResult = MDrv_XC_Auto_Geometry_Ex(pInstance,enAutoTuneType, &ActiveInfoEx, &StandardInfoEx , eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    ActiveInfo->u16HorizontalStart = ActiveInfoEx.u16HorizontalStart;
    ActiveInfo->u16VerticalStart = ActiveInfoEx.u16VerticalStart;
    ActiveInfo->u16HorizontalTotal = ActiveInfoEx.u16HorizontalTotal;
    ActiveInfo->u16HResolution = ActiveInfoEx.u16HResolution;
/*obsolete u8Phase*/    ActiveInfo->u8Phase = (MS_U8)ActiveInfoEx.u16Phase;

    return bResult;
}

MS_BOOL MApi_XC_Auto_Geometry(XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info *ActiveInfo, XC_Auto_Signal_Info *StandardInfo , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_GEOMETRY XCArgs;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.ActiveInfo = ActiveInfo;
    XCArgs.StandardInfo = StandardInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_GEOMETRY, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

//*************************************************************************
//Function name: MApi_XC_Auto_Geometry_Ex
//Passing parameter:
//  enAutoTuneType: Auto function select
//  ActiveInfo: Current PC mode setting
//  StandardInfo: Standard mode. (This is singal information of stardard)
//Return parameter:
//  MS_BOOL: Success status. (If faile, return FALSE.)
//Description: Auto-tune for PC mode
//*************************************************************************
MS_BOOL MDrv_XC_Auto_Geometry_Ex(void *pInstance, XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info_Ex *ActiveInfo, XC_Auto_Signal_Info_Ex *StandardInfo , SCALER_WIN eWindow)
{
{
    MS_U8 u8VSyncTime; // VSync time
    MS_BOOL bAutoResult = TRUE;
    MS_U16 u16OldClk = MDrv_XC_ADC_GetPcClock(pInstance);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // Reset flag. bHaltAutoGeometry should be update during auto geometry
    pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry = FALSE;

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"\n======== Auto Tune Type : 0x%x, Htt %u->%u ======== \n",enAutoTuneType, u16OldClk, ActiveInfo->u16HorizontalTotal);
    if( u16OldClk != ActiveInfo->u16HorizontalTotal )
    {
        MS_U16  u16OrgDefaultHtotal, u16OrgDefaultPhase;
        MS_BOOL bOrgHDuplicate;
        //Save original timing info first
        bOrgHDuplicate = gSrcInfo[eWindow].bHDuplicate;
        u16OrgDefaultHtotal = gSrcInfo[eWindow].u16DefaultHtotal;
        u16OrgDefaultPhase  = gSrcInfo[eWindow].u16DefaultPhase;

        //Use new timing info to tune
        gSrcInfo[eWindow].bHDuplicate = FALSE;
        gSrcInfo[eWindow].u16DefaultPhase = ActiveInfo->u16Phase;
        gSrcInfo[eWindow].u16DefaultHtotal = ActiveInfo->u16HorizontalTotal;
        PQ_ADC_SAMPLING_INFO stPQADCSamplingInfo;
        memset(&stPQADCSamplingInfo,0,sizeof(PQ_ADC_SAMPLING_INFO));
        if ( IsSrcTypeVga(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeYPbPr(gSrcInfo[eWindow].enInputSourceType) )
        {
            if(gSrcInfo[eWindow].bHDuplicate)
            {
                MDrv_XC_ADC_GetPQADCSamplingInfo(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], &stPQADCSamplingInfo, eWindow);
            }
        }
        MDrv_XC_ADC_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], &stPQADCSamplingInfo, eWindow);

        //Restore original timing info
        gSrcInfo[eWindow].bHDuplicate = bOrgHDuplicate;
        gSrcInfo[eWindow].u16DefaultHtotal = u16OrgDefaultHtotal ;
        gSrcInfo[eWindow].u16DefaultPhase = u16OrgDefaultPhase;
    }
    MApi_XC_AutoGainEnable(pInstance,ENABLE, eWindow);

    u8VSyncTime = MDrv_XC_PCMonitor_GetVSyncTime(pInstance, eWindow); // get VSync time

    // Should wait stable to proceed auto
    _MApi_XC_Auto_waitstable(pInstance, u8VSyncTime, ActiveInfo->u16HorizontalTotal*4/10, eWindow); // use u16HorizontalTotal as 2nd para is just to estimate

    #if (!FAST_AUTO_TUNE)
    // auto position valid data theshold
    if(enAutoTuneType & E_XC_AUTO_TUNE_VALID_DATA)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Start to tune Valid data.\n");

        if(_MApi_XC_Auto_SetValidData(pInstance, u8VSyncTime, eWindow) == FALSE)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"AutoValidData failed\n");

            bAutoResult = FALSE;
        }
        else // fixed valid data
        {
            MDrv_XC_SetValidDataThreshold(pInstance, 0x04, eWindow);
        }
    }
    #else

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Force set valid data.\n");

    MDrv_XC_SetValidDataThreshold(pInstance, pXCResourcePrivate->stapiXC_Auto._u8ThresholdValue, eWindow);

    #endif

    ////////////////Stop Geometry checker/////////////////////////////
if (pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Halt auto geometry\n");
        pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry = FALSE;
        return FALSE;
    }

    // Auto Tune H total
    if(enAutoTuneType & E_XC_AUTO_TUNE_FREQ)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Start to tune HTotal\n");

        if(_MApi_XC_Auto_TuneHTotal(pInstance, u8VSyncTime, ActiveInfo, StandardInfo, eWindow) == FALSE)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Tune HTotal fail!!\n");

            bAutoResult = FALSE;
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Tune HTotal success!!\n");
        }
    }

    ////////////////Stop Geometry checker/////////////////////////////
    if (pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Halt auto geometry\n");
        pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry = FALSE;
        return FALSE;
    }

    // auto phase
    if(enAutoTuneType & E_XC_AUTO_TUNE_PHASE)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Start to tune phase\n");

        if(_MApi_XC_Auto_TunePhase(pInstance, u8VSyncTime, ActiveInfo, eWindow) == FALSE)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Tune phase fail!!\n");

            bAutoResult = FALSE;
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Tune phase success!!\n");
        }
    }

    ////////////////Stop Geometry checker/////////////////////////////
    if (pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Halt auto geometry\n");
        pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry = FALSE;
        return FALSE;
    }

    // auto position
    if(enAutoTuneType & E_XC_AUTO_TUNE_POSITION)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Start to tune position\n");

        if(_MApi_XC_Auto_TunePosition(pInstance, u8VSyncTime, ActiveInfo, StandardInfo, eWindow) == FALSE)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Tune position fail!!\n");
            //enAutoTuneType = E_XC_AUTO_TUNE_NULL;
            bAutoResult = FALSE;
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Tune position success!!\n");
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Result: %d \n",bAutoResult);

    return    bAutoResult;
}
}

MS_BOOL MApi_XC_Auto_Geometry_Ex_U2(void* pInstance, XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info_Ex *ActiveInfo, XC_Auto_Signal_Info_Ex *StandardInfo , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bReturn = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    bReturn = MDrv_XC_Auto_Geometry_Ex(pInstance,enAutoTuneType, ActiveInfo, StandardInfo , eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_Auto_Geometry_Ex(XC_Auto_TuneType enAutoTuneType, XC_Auto_Signal_Info_Ex *ActiveInfo, XC_Auto_Signal_Info_Ex *StandardInfo , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_GEOMETRY_EX XCArgs;
    XCArgs.enAutoTuneType = enAutoTuneType;
    XCArgs.ActiveInfo = ActiveInfo;
    XCArgs.StandardInfo = StandardInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_GEOMETRY_EX, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


// Set flag as true to halt auto geometry
MS_BOOL MApi_XC_Auto_StopAutoGeometry_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Stop auto geometry.\n");
    pXCResourcePrivate->stapiXC_Auto.bHaltAutoGeometry = TRUE;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return TRUE;
}

MS_BOOL MApi_XC_Auto_StopAutoGeometry(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_STOPAUTOGEOMETRY XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_STOPAUTOGEOMETRY, (void*)&XCArgs) != 0)
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
/// -This function will recalibration ADC offset, must be called when mode change.
/// @param bEnable \b IN:
/// - 0: Disable offset recalibration
/// - 1: Enable offset recalibration
/// @param bIsYPbPrFlag \b IN: to tell if YPbPr in
/// - 0: Vga mode
/// - 1: YPbPr mode
/******************************************************************************/
void MApi_XC_Auto_AutoOffset_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag)
{
    MApi_XC_ADC_AutoSetting(pInstance, bEnable, bIsYPbPrFlag, COMPONENT_AUTO_SW_MODE);
}

void MApi_XC_Auto_AutoOffset(MS_BOOL bEnable, MS_BOOL bIsYPbPrFlag)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_AUTO_AUTOOFFSET XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.bIsYPbPrFlag = bIsYPbPrFlag;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_AUTOOFFSET, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_Auto_DetectWidth_U2(void* pInstance, XC_Auto_Signal_Info_Ex *pSignalInfo , SCALER_WIN eWindow)
{
    MS_U8 u8VSyncTime ; // VSync time
    MS_BOOL bAutoResult = TRUE;
    MS_U16 u16ActualWidth = 0xFFFF;
    MS_U16 u16OldClk = MDrv_XC_ADC_GetPcClock(pInstance);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"\n======== Auto DetectWidth : eWindow=%u, Htt %u->%u ======== \n",eWindow, u16OldClk, pSignalInfo->u16HorizontalTotal);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( u16OldClk != pSignalInfo->u16HorizontalTotal )
    {
        MS_U16  u16OrgDefaultHtotal, u16OrgDefaultPhase;
        MS_BOOL bOrgHDuplicate;
        //Save original timing info first

        bOrgHDuplicate = gSrcInfo[eWindow].bHDuplicate;
        u16OrgDefaultHtotal = gSrcInfo[eWindow].u16DefaultHtotal;
        u16OrgDefaultPhase  = gSrcInfo[eWindow].u16DefaultPhase;

        //Use new timing info to tune
        gSrcInfo[eWindow].bHDuplicate = FALSE;
        gSrcInfo[eWindow].u16DefaultPhase = pSignalInfo->u16Phase;
        gSrcInfo[eWindow].u16DefaultHtotal = pSignalInfo->u16HorizontalTotal;
        PQ_ADC_SAMPLING_INFO stPQADCSamplingInfo;
        memset(&stPQADCSamplingInfo,0,sizeof(PQ_ADC_SAMPLING_INFO));
        if ( IsSrcTypeVga(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeYPbPr(gSrcInfo[eWindow].enInputSourceType) )
        {
            if(gSrcInfo[eWindow].bHDuplicate)
            {
                _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
                MDrv_XC_ADC_GetPQADCSamplingInfo(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], &stPQADCSamplingInfo, eWindow);
                _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
            }
        }
        MDrv_XC_ADC_SetMode(pInstance, gSrcInfo[eWindow].enInputSourceType, &gSrcInfo[eWindow], &stPQADCSamplingInfo, eWindow);

        //Restore original timing info
        gSrcInfo[eWindow].bHDuplicate = bOrgHDuplicate;
        gSrcInfo[eWindow].u16DefaultHtotal = u16OrgDefaultHtotal ;
        gSrcInfo[eWindow].u16DefaultPhase = u16OrgDefaultPhase;

    }
    MApi_XC_AutoGainEnable(pInstance, ENABLE, eWindow);
    u8VSyncTime = MDrv_XC_PCMonitor_GetVSyncTime(pInstance, eWindow); // get VSync time
    // Should wait stable to proceed auto
    _MApi_XC_Auto_waitstable(pInstance, u8VSyncTime, pSignalInfo->u16HorizontalTotal*4/10, eWindow); // use u16HorizontalTotal as 2nd para is just to estimate
#if (!FAST_AUTO_TUNE)
    // auto position valid data theshold
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto DetectWidth: Start to tune Valid data.\n");
    if(_MApi_XC_Auto_SetValidData(pInstance, u8VSyncTime, eWindow) == FALSE)
    {
        // auto position valid data theshold
        XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto DetectWidth: Start to tune Valid data.\n");
        bAutoResult = FALSE;
    }
    else // fixed valid data
    {
        MDrv_XC_SetValidDataThreshold(pInstance, 0x04, eWindow);
        u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);
    }
#else
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto DetectWidth: Force set valid data.\n");
    MDrv_XC_SetValidDataThreshold(pInstance, pXCResourcePrivate->stapiXC_Auto._u8ThresholdValue, eWindow);
    u16ActualWidth = _MApi_XC_Auto_GetActualWidth(pInstance, u8VSyncTime, eWindow);
#endif
    pSignalInfo->u16HResolution = u16ActualWidth; //Set to invalid resolution
    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"Auto DetectWidth: bResult=%u, DetectWidth=%u.\n", bAutoResult, u16ActualWidth);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bAutoResult;
}

MS_BOOL MApi_XC_Auto_DetectWidth(XC_Auto_Signal_Info_Ex *pSignalInfo , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_DETECTWIDTH XCArgs;
    XCArgs.pSignalInfo = pSignalInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_DETECTWIDTH, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


MS_BOOL MApi_XC_Auto_SetCalibrationMode_U2(void* pInstance, XC_Auto_CalibrationMode eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (E_XC_SW_CALIBRATION == eMode)
    {
        pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode = eMode;
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return TRUE;
    }
    else if ((E_XC_HW_CALIBRATION == eMode) && MDrv_XC_ADC_CheckHWCalibrationSupport(pInstance))
    {
        pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode = eMode;
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return TRUE;
    }
    else
    {
        pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode = E_XC_SW_CALIBRATION;
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return FALSE;
    }
}

MS_BOOL MApi_XC_Auto_SetCalibrationMode(XC_Auto_CalibrationMode eMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_AUTO_SETCALIBRATIONMODE XCArgs;
    XCArgs.eMode = eMode;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_SETCALIBRATIONMODE, (void*)&XCArgs) != 0)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_Auto_GetCalibrationMode_U2(void* pInstance, XC_Auto_CalibrationMode *eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    *eMode = pXCResourcePrivate->stapiXC_Auto.g_enCalbrationMode;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_Auto_GetCalibrationMode(XC_Auto_CalibrationMode *eMode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_AUTO_GETCALIBRATIONMODE XCArgs;
    XCArgs.eMode = eMode;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_GETCALIBRATIONMODE, (void*)&XCArgs) != 0)
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
/// get auto position window with the corresponding valid data threshold
/// @param  u8ValidData                 \b IN: valid data threshold
/// @param  eWindow                     \b IN: which window we are going to get
/// @param  pstAutoPositionWindow       \b OUT: auto position window parameters
/// @return MS_BOOL     TRUE: auto position window is valid     FALSE: auto position window is invalid
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetAutoPositionWindow_U2(void* pInstance, MS_U8 u8ValidData, SCALER_WIN eWindow, MS_WINDOW_TYPE *pstAutoPositionWindow)
{
    MS_U16 u16VStart = 0, u16HStart = 0, u16VEnd = 0, u16HEnd = 0;
    MS_U16 u16Count = MAX_AUTO_POSITION_WAIT_READY_TIME;
    MS_BOOL bReady = FALSE;
    MS_WINDOW_TYPE stCapWin;
    memset(&stCapWin, 0, sizeof(MS_WINDOW_TYPE));
    MS_U8 u8VSyncTime = 0;
    MS_BOOL bHdmiDeMode = FALSE;
    MS_BOOL bDEModeGlitchProtectEnabled = FALSE;
    MS_U8 u8PostGlitchRemoval = 0;
    INPUT_SOURCE_TYPE_t enInputSourceType;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    enInputSourceType = pXCResourcePrivate->stdrvXC_MVideo._SCShared.StatusInfo[eWindow].enInputSourceType;

    if(IsSrcTypeHDMI(enInputSourceType) && (MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_DE))
    {
        bHdmiDeMode = TRUE;
        u8VSyncTime = MDrv_XC_PCMonitor_GetVSyncTime(pInstance, eWindow);
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    if (bHdmiDeMode)
    {
        MDrv_XC_SetAutoPositionForcePixelMode(pInstance, ENABLE, eWindow);
    }

    _XC_ENTRY(pInstance);
    MDrv_XC_GetCaptureWindow(pInstance, &stCapWin, eWindow);
    _XC_RETURN(pInstance);

    MApi_XC_AutoRangeEnable(pInstance, DISABLE, eWindow);
    MApi_XC_SetAutoRangeWindow(pInstance, stCapWin.y, stCapWin.x, stCapWin.height, stCapWin.width, eWindow);
    MApi_XC_AutoRangeEnable(pInstance, ENABLE, eWindow);

    MApi_XC_AutoPositionEnable(pInstance, DISABLE, eWindow);
    MDrv_XC_SetValidDataThreshold(pInstance, u8ValidData, eWindow);
    MApi_XC_AutoPositionEnable(pInstance, ENABLE, eWindow);

    while(1)
    {
        bReady = MApi_XC_IsAutoPositionResultReady(pInstance, eWindow);
        if (bReady == TRUE)
        {
            break;
        }
        if (bReady == FALSE && u16Count == 0)
        {
            MApi_XC_AutoRangeEnable(pInstance, DISABLE, eWindow);
            return FALSE;
        }
        MsOS_DelayTask(1);
        u16Count--;
    }

    if(bHdmiDeMode)
    {
        MsOS_DelayTask(4 * u8VSyncTime);
    }

    u16VStart = MApi_XC_SetAutoPositionVstart(pInstance, eWindow);
    u16HStart = MApi_XC_SetAutoPositionHstart(pInstance, eWindow);
    u16VEnd = MApi_XC_SetAutoPositionVend(pInstance, eWindow);
    u16HEnd = MApi_XC_SetAutoPositionHend(pInstance, eWindow);

    if (bHdmiDeMode)
    {
        bDEModeGlitchProtectEnabled = MDrv_SC_ip_get_DE_mode_glitch_protect_enabled(pInstance, eWindow);
        if (bDEModeGlitchProtectEnabled)
        {
            u8PostGlitchRemoval = MDrv_SC_ip_get_post_glitch_removal(pInstance, eWindow);
        }
        else
        {
            u8PostGlitchRemoval = 0;
        }

        u16HStart += u8PostGlitchRemoval * 8;
        u16HEnd += u8PostGlitchRemoval * 8;
    }

    MApi_XC_AutoRangeEnable(pInstance, DISABLE, eWindow);

    if (bHdmiDeMode)
    {
        MDrv_XC_SetAutoPositionForcePixelMode(pInstance, DISABLE, eWindow);
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR)
        _XC_ENTRY(pInstance);
        while (MDrv_XC_PCMonitor_InvalidTimingDetect(pInstance, TRUE, eWindow));
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    }

    if ((u16VEnd < u16VStart) || (u16HEnd < u16HStart))
    {
        return FALSE;
    }

    pstAutoPositionWindow->x = u16HStart;
    pstAutoPositionWindow->y = u16VStart;
    pstAutoPositionWindow->width = u16HEnd - u16HStart + 1;
    pstAutoPositionWindow->height = u16VEnd - u16VStart + 1;

    return TRUE;
}

MS_BOOL MApi_XC_GetAutoPositionWindow(MS_U8 u8ValidData, SCALER_WIN eWindow, MS_WINDOW_TYPE *pstAutoPositionWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_AUTO_POSITION_WINDOW XCArgs;
    XCArgs.u8ValidData = u8ValidData;
    XCArgs.eWindow = eWindow;
    XCArgs.pstAutoPositionWindow = pstAutoPositionWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_AUTO_POSITION_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}
//*************************************************************************
//Function name: MApi_XC_AUTO_GetSyncInfo_U2
//Return parameter:
//  XC_AUTO_SYNC_INFO: sync info.
//Description: The function just be used after sync stable. (ex. between setWindow and AutoAdjust)
//*************************************************************************
void MApi_XC_AUTO_GetSyncInfo_U2(void* pInstance,XC_AUTO_SYNC_INFO *pSyncInfo)
{
    MS_U8 u8VSyncTime = 0;
    XC_IP_SYNC_STATUS stXC_Sync_Status = {0, 0, 0};

    u8VSyncTime = MDrv_XC_PCMonitor_GetVSyncTime(pInstance, pSyncInfo->eWindow); // get VSync time
    MDrv_XC_GetSyncStatus(pInstance, pSyncInfo->eCurrentSrc, &stXC_Sync_Status, pSyncInfo->eWindow);

    //fill polarity
    if(stXC_Sync_Status.u8SyncStatus & MD_FLAG_POR_HPVP)
    {
        pSyncInfo->eSyncPolarity = E_XC_AUTO_POR_HPVP;
    }
    else if(stXC_Sync_Status.u8SyncStatus & MD_FLAG_POR_HPVN)
    {
        pSyncInfo->eSyncPolarity = E_XC_AUTO_POR_HPVN;
    }
    else if(stXC_Sync_Status.u8SyncStatus & MD_FLAG_POR_HNVP)
    {
        pSyncInfo->eSyncPolarity = E_XC_AUTO_POR_HNVP;
    }
    else
    {
        pSyncInfo->eSyncPolarity = E_XC_AUTO_POR_HNVN;
    }

    //fill hsync start and hsync end
    pSyncInfo->u32HSyncStart = MApi_XC_SetAutoPositionHstart(pInstance, pSyncInfo->eWindow);
    pSyncInfo->u32HSyncEnd   = MApi_XC_SetAutoPositionHend(pInstance, pSyncInfo->eWindow);

    XC_LOG_TRACE(XC_DBGLEVEL_AUTOGEOMETRY,"\n======== Get SyncInfo:Polarity=%u,HSyncStart=%td,VSyncStart=%td ======== \n",pSyncInfo->eSyncPolarity, (ptrdiff_t)pSyncInfo->u32HSyncStart,(ptrdiff_t)pSyncInfo->u32HSyncEnd);
}

void MApi_XC_AUTO_GetSyncInfo(XC_AUTO_SYNC_INFO *pSyncInfo)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return ;
    }

    stXC_AUTO_GETSYNCINFO XCArgs;
    XCArgs.pSyncInfo = pSyncInfo;

    if(UtopiaIoctl(pu32XCInst, E_XC_AUTO_CMD_GETSYNCINFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return ;
    }
    else
    {
        return ;
    }
}

#undef _APIXC_AUTO_C_
