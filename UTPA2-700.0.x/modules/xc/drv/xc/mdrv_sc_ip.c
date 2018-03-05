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
/// file    Mdrv_sc_ip.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define  _MDRV_SC_IP_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/div64.h>
#else
#include <string.h>
#define do_div(x,y) ((x)/=(y))
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
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
#include "drvXC_HDMI_Internal.h"
#include "drv_xc_calibration.h"
#include "mhal_menuload.h"
#include "drv_sc_scaling.h"
#include "drv_sc_mux.h"

#include "mhal_ip.h"
#include "mhal_mux.h"
#include "halCHIP.h"

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

#ifdef ANDROID
#include <cutils/log.h>
#ifndef LOGE
#define printf(format, args...)          ALOGE(format, ##args);
#else
#define printf(format, args...)          LOGE(format, ##args);
#endif
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define msg_sc_ip(x)                //x
#define FB_DBG(x)                   // x
#define PCMSG_TIMING(x)             //x

#define ENABLE_DEBUG_CLEAR_FB_BEFORE_CAPTURE    0
#if (defined(FPU_SUPPORT) && defined(__arm__) && defined(MSOS_TYPE_LINUX))
#define DATA_CONVERT_NEON_OPTIMIZE_ENABLE   TRUE
#else
#define DATA_CONVERT_NEON_OPTIMIZE_ENABLE   FALSE
#endif

#define R_CHANNEL_MIN    0x00
#define G_CHANNEL_MIN    0x01
#define B_CHANNEL_MIN    0x02
#define R_CHANNEL_MAX    0x03
#define G_CHANNEL_MAX    0x04
#define B_CHANNEL_MAX    0x05

#define HDE_UPPERBOUND_720  1280
#define HDE_LOWERBOUND_720  900
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
typedef struct SC_MEM_YUV444_FMT_s
{
    MS_U32 Cb:10;
    MS_U32 Y:10;
    MS_U32 Cr:10;
    MS_U32 u8Dummy:2;       // [31:30] no use
} SC_MEM_YUV444_FMT_t, *pSC_MEM_YUV444_FMT_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
//MS_BOOL g_bDoCalibration;
#endif

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

//-----------------------------------------------------------------------------
// IP MUX
//-----------------------------------------------------------------------------
void MDrv_XC_reset_ip(void *pInstance, SCALER_WIN eWindow)
{

    if( eWindow == MAIN_WINDOW )
    {
        Hal_SC_ip_software_reset(pInstance, REST_IP_F2, eWindow);
    }
    else
    {
        Hal_SC_ip_software_reset(pInstance, REST_IP_F1, eWindow);
    }
}


//-------------------------------------------------------------------------------------------------
/// do the software reset for the specific window
/// @param  u8Reset             \b IN: reset IP @ref SOFTWARE_REST_TYPE_t
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SoftwareReset_U2(void* pInstance, MS_U8 u8Reset, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_SoftwareReset(pInstance, u8Reset, eWindow );
    _XC_RETURN(pInstance);
}

void MApi_XC_SoftwareReset(MS_U8 u8Reset, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SOFTWARE_RESET XCArgs;
    XCArgs.u8Reset = u8Reset;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SOFTWARE_RESET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_SoftwareReset(void *pInstance, MS_U8 u8Reset, SCALER_WIN eWindow)
{
    Hal_SC_ip_software_reset(pInstance, u8Reset, eWindow );
}

/******************************************************************************/
/// -This function will return input video source status
/// @return Video status
/// - BIT0:VSnc polarity bit(0/1 = positive/negative)
/// - BIT1:HSync polarity bit(0/1 = positive/negative)
/// - BIT2:HSync loss bit
/// - BIT3:VSync loss bit
/// - BIT4:Interlace mode
/// - BIT7:User new mode (Not found in mode table)
/******************************************************************************/
void MDrv_XC_ip_get_sync_status(void *pInstance, XC_IP_SYNC_STATUS *sXC_Sync_Status, E_MUX_INPUTPORT enInputPort , SCALER_WIN eWindow)
{
    MS_U8 u8DetectStatus;
    //MS_U16 u16HDE_temp_size,u16HDE_size =0;
    /* mode detect status */
    sXC_Sync_Status->u8SyncStatus = 0x00;

    u8DetectStatus = Hal_SC_ip_get_sync_detect_status(pInstance, eWindow);
    if ( u8DetectStatus & BIT(0) )
    {
        sXC_Sync_Status->u8SyncStatus |= XC_MD_VSYNC_POR_BIT;
    }
    if ( u8DetectStatus & BIT(1) )
    {
        sXC_Sync_Status->u8SyncStatus |= XC_MD_HSYNC_POR_BIT;
    }
    if ( u8DetectStatus & BIT(3) )
    {
        sXC_Sync_Status->u8SyncStatus |= XC_MD_INTERLACE_BIT;
    }

    /* HSync */
    sXC_Sync_Status->u16Hperiod = MDrv_SC_ip_get_horizontalPeriod(pInstance, eWindow);
    if( (sXC_Sync_Status->u16Hperiod == MST_H_PERIOD_MASK) || (sXC_Sync_Status->u16Hperiod < 10) )
    {
        sXC_Sync_Status->u8SyncStatus |= XC_MD_HSYNC_LOSS_BIT;
    }
    msg_sc_ip(printf("Hperiod=0x%x\n", sXC_Sync_Status->u16Hperiod));

    /* VSync */
    sXC_Sync_Status->u16Vtotal = MDrv_SC_ip_get_verticaltotal(pInstance, eWindow);
    if ( (sXC_Sync_Status->u16Vtotal == MST_V_TOTAL_MASK) || (sXC_Sync_Status->u16Vtotal < 200) )
    {
        sXC_Sync_Status->u8SyncStatus |= XC_MD_VSYNC_LOSS_BIT;
    }

    // Keep original Vtt even we force set interlace
    if (MDrv_SC_GetInterlaceInPModeStatus(pInstance, eWindow)
        && (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_SW))
    {
        sXC_Sync_Status->u16Vtotal >>= 1;
    }

    msg_sc_ip(printf("Vtotal=0x%x\n", sXC_Sync_Status->u16Vtotal));

    //MDrv_HDMI_StablePolling();

    if ( IsDVIPort(enInputPort) ) // DVI
    {
        if ( MDrv_HDMI_dvi_clock_detect(enInputPort) )
            sXC_Sync_Status->u8SyncStatus |= XC_MD_SYNC_LOSS;

         Hal_SC_SetHDMI_Spliter(pInstance,sXC_Sync_Status,enInputPort,eWindow);
    }

    msg_sc_ip(printf("SyncStatus=0x%x\n", sXC_Sync_Status->u8SyncStatus));
}

/********************************************************************************/
/*                   Functions                                                  */
/********************************************************************************/

//-------------------------------------------------------------------------------------------------
/// This function generates specific timing
/// @param  timingtype      \b IN: the specific timing to generate
//-------------------------------------------------------------------------------------------------
void MApi_XC_GenSpecificTiming_U2(void* pInstance, XC_Internal_TimingType timingtype)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_GenSpecificTiming(pInstance, timingtype);
    _XC_RETURN(pInstance);
}

void MApi_XC_GenSpecificTiming(XC_Internal_TimingType timingtype)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_GENERATE_SPECIFIC_TIMING XCArgs;
    XCArgs.timingtype = timingtype;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_GENERATE_SPECIFIC_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function tells whether it's ip generated timing
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SC_Check_IP_Gen_Timing(void *pInstance)
{
    return Hal_SC_Check_IP_Gen_Timing(pInstance);
}

//-------------------------------------------------------------------------------------------------
/// This function initialize IP for internal timing
/// @param  timingtype      \b IN: the specific timing to generate
//-------------------------------------------------------------------------------------------------
void MApi_XC_InitIPForInternalTiming_U2(void* pInstance, XC_Internal_TimingType timingtype)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_InitIPForInternalTiming(pInstance, timingtype, MAIN_WINDOW);
    _XC_RETURN(pInstance);
}

void MApi_XC_InitIPForInternalTiming(XC_Internal_TimingType timingtype)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_INIT_IP_FOR_INTERNAL_TIMING XCArgs;
    XCArgs.timingtype = timingtype;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_INIT_IP_FOR_INTERNAL_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function Gets by-pass mode status:
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SC_Get_DE_Bypass_Mode(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //TRUE: HV mdoe, FALSE:DE mode
    if( eWindow == MAIN_WINDOW )
    {
        return ((SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK01_2F_L,  BIT(15) ))? TRUE : FALSE);
    }
    else
    {
        return ((SC_R2BYTEMSK( psXCInstPri->u32DeviceID, REG_SC_BK03_2F_L,  BIT(15) ))? TRUE : FALSE);
    }
}

//-------------------------------------------------------------------------------------------------
/// This function get DE-Bypass Mode
/// @param  eWindow      \b IN: which window(main or sub) to get
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetDEBypassMode_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bIsDEBypassMode = FALSE;
    _XC_ENTRY(pInstance);
    bIsDEBypassMode = MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return bIsDEBypassMode;
}

MS_BOOL MApi_XC_GetDEBypassMode(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_DE_BYPASS_MODE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DE_BYPASS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// This function get DE window
/// @param  psWin        \b OUT: window info to get
/// @param  eWindow      \b IN: which window(main or sub) to get
//-------------------------------------------------------------------------------------------------
void MDrv_XC_GetDEWindow(void *pInstance, MS_WINDOW_TYPE *psWin, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow) == TRUE)
    {
        printf("Cannot get DE window under HV mode!! please check your xc lib!\n");
    }

    psWin->x      = Hal_SC_ip_de_hstart_info(pInstance, eWindow);
    psWin->width  = (Hal_SC_ip_de_hend_info(pInstance, eWindow) - psWin->x)+1;

    // In froce interlace mode.
    // Original size of V DE height => (Hal_SC_ip_de_vend_info(eWindow) - psWin->y)+1;
    // the "+1" should be *2 also after force interlace mode.
    if (MDrv_SC_GetInterlaceInPModeStatus(pInstance, eWindow)
        && (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_SW))
    {
        psWin->y    = ( Hal_SC_ip_de_vstart_info(pInstance, eWindow) >> 1 ) - 1;
        // original statement => psWin->height = ( (Hal_SC_ip_de_vend_info(eWindow) >> 1 ) - 1 ) - psWin->y) + 1;
        // we can reduce to below statement.
        psWin->height =  (Hal_SC_ip_de_vend_info(pInstance, eWindow) >> 1 )  - psWin->y  ;
    }
    else
    {
        psWin->y      = Hal_SC_ip_de_vstart_info(pInstance, eWindow);
        psWin->height = (Hal_SC_ip_de_vend_info(pInstance, eWindow) - psWin->y)+1;//for progresive signal, VDE = BK1_0x14 - BK1_0x12 + 1
    }

    if (Hal_SC_ip_get_interlace_status(pInstance, eWindow))
    {
        if(psWin->height & 1)
        {
            psWin->height += 1;//for interlace signal, VDE = BK1_0x14 - BK1_0x12 + 2, so here need add extra 1
        }
    }
}

void MApi_XC_GetDEWindow_U2(void* pInstance, MS_WINDOW_TYPE *psWin, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_GetDEWindow(pInstance, psWin, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return;
}

void MApi_XC_GetDEWindow(MS_WINDOW_TYPE *psWin, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_DE_WINDOW XCArgs;
    XCArgs.psWin = psWin;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DE_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_GetDEWidthHeightInDEByPassMode(void *pInstance, MS_U16* pu16Width, MS_U16* pu16Height, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    MS_U16 u16HSDEnabled = 0x00, u16VSDEnabled = 0x00;
#if (HW_DESIGN_3D_VER >= 2)
    MS_U16 u16PixSep = 0;
#endif

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow) != TRUE)
    {
        printf("[%s,%5d] Cannot get HV width and height under DE mode!! please check your xc lib!\n", __FUNCTION__,__LINE__);
    }

#if (HW_DESIGN_3D_VER >= 2)
    // 3d pixel sep reg will affect hsd/vsd info
    u16PixSep = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, 0x0, BIT(0)|BIT(1)|BIT(2));
#endif

    if (eWindow == MAIN_WINDOW )
    {
        // Backup
        u16HSDEnabled = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, BIT(15));
        u16VSDEnabled = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L, BIT(15));

        //F2 force IP read request disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, BIT(14), BIT(14));
    }
    else
    {
        // Backup
        u16HSDEnabled = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L, BIT(15));
        u16VSDEnabled = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_09_L, BIT(15));

        //F1 force IP read request disable
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_42_L, BIT(14), BIT(14));
    }

    // Disable IPM read/write
    Hal_SC_IP_Memory_Write_Request(pInstance, DISABLE, eWindow);
    Hal_SC_IP_Memory_Read_Request(pInstance, DISABLE, eWindow);

    // Disable scaling for get correct DE
    MDrv_SC_Enable_PreScaling(pInstance, FALSE, FALSE, eWindow);

    //IP2 VSD output line count control enable will make sub V DE size wrong
    HAL_SC_Set_vsd_output_line_count(pInstance, DISABLE, 0x00, eWindow);
    //IP2 VSD input line count control enable will make sub v DE size wrong
    HAL_SC_Set_vsd_input_line_count(pInstance, DISABLE, FALSE, 0x00, eWindow);

    // To update VSD, V capture have to set full range to make sure get all line count
    MDrv_SC_SetFullRangeCapture(pInstance, eWindow);

    // Need to wait more time to make sure getting correct DE V size
    MDrv_XC_wait_output_vsync(pInstance, 5, 200, MAIN_WINDOW);

    // Get real DE data after VSD / HSD
    *pu16Height = Hal_SC_get_vsd_line_count(pInstance, eWindow);
    *pu16Width = Hal_SC_get_hsd_pixel_count(pInstance, eWindow);
    // Restore
    MDrv_SC_Enable_PreScaling(pInstance, u16HSDEnabled>>15, u16VSDEnabled>>15, eWindow);

    //  force IP read request enable
    if (eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L, 0, BIT(14));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_42_L, 0, BIT(14));
    }

#if (HW_DESIGN_3D_VER >= 2)
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_6B_L, u16PixSep, BIT(0)|BIT(1)|BIT(2));
#endif

    // Enable IPM read/write
    Hal_SC_IP_Memory_Write_Request(pInstance, ENABLE, eWindow);
    Hal_SC_IP_Memory_Read_Request(pInstance, ENABLE, eWindow);

    if(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus& XC_MD_INTERLACE_BIT)
    {
        *pu16Height <<= 1; //For interlace, multiply 2 for height
    }
}

//-------------------------------------------------------------------------------------------------
/// This function get DE window in DE ByPass Mode. (T12 / T13 can not support HDMI HV mode. So DE window from BK1 can not be retrieved if set to HV mode.)
/// We Bypass pre-scaling and get H/V size from BK2 MDrv_SC_get_vsd_line_count & MDrv_SC_get_hsd_pixel_count.
/// Prepare this function for using in the feature.
/// @param  psWin        \b OUT: window info to get
/// @param  eWindow      \b IN: which window(main or sub) to get
//-------------------------------------------------------------------------------------------------
void MApi_XC_GetDEWidthHeightInDEByPassMode_U2(void* pInstance, MS_U16* pu16Width,MS_U16* pu16Height ,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_PCMonitor_GetDEWidthHeightInDEByPassMode(pInstance, pu16Width, pu16Height, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_GetDEWidthHeightInDEByPassMode(MS_U16* pu16Width,MS_U16* pu16Height ,SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE XCArgs;
    XCArgs.pu16Width = pu16Width;
    XCArgs.pu16Height = pu16Height;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DE_WIN_WIDTH_HEIGHT_IN_DE_BYPASS_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get the capture window for specific window
/// @param  capture_win      \b OUT: the window info to get
/// @param  eWindow          \b IN: which window(main or sub) to get
//-------------------------------------------------------------------------------------------------
void MApi_XC_GetCaptureWindow_U2(void* pInstance, MS_WINDOW_TYPE* capture_win, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_GetCaptureWindow(pInstance, capture_win, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_GetCaptureWindow(MS_WINDOW_TYPE* capture_win, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_CAPTURE_WINDOW XCArgs;
    XCArgs.capture_win = capture_win;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CAPTURE_WINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the Vertical start of capture window for specific window
/// @param  u16Vstart        \b IN: the window info to get
/// @param  eWindow          \b IN: which window(main or sub) to set
//-------------------------------------------------------------------------------------------------
void MDrv_XC_SetCaptureWindowVstart(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].stCapWin.y = u16Vstart;
    Hal_SC_ip_set_capture_v_start(pInstance, u16Vstart, eWindow);
}

void MDrv_XC_SetCaptureWindowVstart_with_Menuload(void *pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].stCapWin.y = u16Vstart;
    Hal_SC_ip_set_capture_v_start_with_Menuload(pInstance, u16Vstart, eWindow);
}

void MApi_XC_SetCaptureWindowVstart_U2(void* pInstance, MS_U16 u16Vstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetCaptureWindowVstart_with_Menuload(pInstance, u16Vstart, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return;
}

void MApi_XC_SetCaptureWindowVstart(MS_U16 u16Vstart , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_VSTART XCArgs;
    XCArgs.u16Vstart = u16Vstart;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CAPTURE_WINDOW_VSTART, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the horizontal start of capture window for specific window
/// @param  u16Hstart        \b IN: the window info to get
/// @param  eWindow          \b IN: which window(main or sub) to set
//-------------------------------------------------------------------------------------------------
void MDrv_XC_SetCaptureWindowHstart(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].stCapWin.x = u16Hstart;
    Hal_SC_ip_set_capture_h_start(pInstance, u16Hstart, eWindow);
}

void MDrv_XC_SetCaptureWindowHstart_with_Menuload(void *pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].stCapWin.x = u16Hstart;
    Hal_SC_ip_set_capture_h_start_with_Menuload(pInstance, u16Hstart, eWindow);
}

void MApi_XC_SetCaptureWindowHstart_U2(void* pInstance, MS_U16 u16Hstart , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetCaptureWindowHstart_with_Menuload(pInstance, u16Hstart, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return;
}

void MApi_XC_SetCaptureWindowHstart(MS_U16 u16Hstart , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_HSTART XCArgs;
    XCArgs.u16Hstart = u16Hstart;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CAPTURE_WINDOW_HSTART, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the Vertical size of capture window for specific window
/// @param  u16Vsize         \b IN: the window info to get
/// @param  eWindow          \b IN: which window(main or sub) to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetCaptureWindowVsize_U2(void* pInstance, MS_U16 u16Vsize , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    gSrcInfo[eWindow].stCapWin.height = u16Vsize;
    Hal_SC_ip_set_capture_v_size(pInstance, u16Vsize, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

}

void MApi_XC_SetCaptureWindowVsize(MS_U16 u16Vsize , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_VSIZE XCArgs;
    XCArgs.u16Vsize = u16Vsize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CAPTURE_WINDOW_VSIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the Horizontal size of capture window for specific window
/// @param  u16Hsize         \b IN: the window info to get
/// @param  eWindow          \b IN: which window(main or sub) to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetCaptureWindowHsize_U2(void* pInstance, MS_U16 u16Hsize , SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    gSrcInfo[eWindow].stCapWin.width = u16Hsize;
    Hal_SC_ip_set_capture_h_size(pInstance, u16Hsize, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetCaptureWindowHsize(MS_U16 u16Hsize , SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CAPTURE_WINDOW_HSIZE XCArgs;
    XCArgs.u16Hsize = u16Hsize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CAPTURE_WINDOW_HSIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_SetCaptureWindow(void *pInstance, MS_WINDOW_TYPE* pstCaptureWin, SCALER_WIN eWindow, MS_BOOL bFire)
{

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    EN_MUTE_TYPE  enMuteType = E_MAIN_WINDOW_MUTE;
    switch(eWindow)
    {
        case MAIN_WINDOW:
            enMuteType = E_MAIN_WINDOW_MUTE;
            break;
        case SUB_WINDOW:
            enMuteType = E_SUB_WINDOW_MUTE;
            break;
        default:
            break;
    }

    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(enMuteType)) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)))
    {
        _MLOAD_ENTRY(pInstance);
        if(eWindow == MAIN_WINDOW)
        {

            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_04_L, (MS_U16)(pstCaptureWin->y), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_05_L, (MS_U16)(pstCaptureWin->x), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_06_L, (MS_U16)(pstCaptureWin->height), 0xFFFF);

            if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15)))
                pstCaptureWin->width |= 0x8000;
            else
                pstCaptureWin->width |= 0x0000;

            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_07_L, (MS_U16)(pstCaptureWin->width), 0xFFFF);
        }
        else if(eWindow == SUB_WINDOW)
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_04_L, (MS_U16)(pstCaptureWin->y), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_05_L, (MS_U16)(pstCaptureWin->x), 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_06_L, (MS_U16)(pstCaptureWin->height), 0xFFFF);
            if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, BIT(15)))
                pstCaptureWin->width |= 0x8000;
            else
                pstCaptureWin->width |= 0x0000;

            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_07_L, (MS_U16)(pstCaptureWin->width), 0xFFFF);
        }
        if(bFire == TRUE)
        {
            if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
            {
                printf("Error: Set MLoad Fire Error!!!!\n ");
            }
        }
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        Hal_SC_ip_set_capture_h_start(pInstance, pstCaptureWin->x,eWindow);
        Hal_SC_ip_set_capture_v_start(pInstance, pstCaptureWin->y,eWindow);
        Hal_SC_ip_set_capture_h_size(pInstance, pstCaptureWin->width,eWindow);
        Hal_SC_ip_set_capture_v_size(pInstance, pstCaptureWin->height,eWindow);
    }
}

//-------------------------------------------------------------------------------------------------
/// enable or disable the Auto Gain for specific window
/// @param  u8Enable         \b IN: enable or disable
/// @param  eWindow          \b IN: which window(main or sub) to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_AutoGainEnable(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_set_auto_gain_function(pInstance, u8Enable, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetFIRDownSampleDivider(void *pInstance, MS_U8 u8Enable , MS_U16 u16OverSamplingCount , SCALER_WIN eWindow )
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_set_fir_down_sample_divider(pInstance, u8Enable, u16OverSamplingCount ,eWindow);
    _XC_RETURN(pInstance);
}

//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_EnableIPAutoNoSignal_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    if(Hal_XC_MApi_XC_IPAutoNoSignal_GetCaps())
    {
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        gSrcInfo[eWindow].bAutoNoSignalEnabled = bEnable;

        if(bEnable==DISABLE)
        {
            Hal_SC_ip_set_IPAutoNoSignal(pInstance, bEnable,eWindow);
        }
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_EnableIPAutoNoSignal(MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_IPAUTONOSIGNAL XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IPAUTONOSIGNAL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_GetIPAutoNoSignal_U2(void* pInstance, SCALER_WIN eWindow )
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = gSrcInfo[eWindow].bAutoNoSignalEnabled;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_GetIPAutoNoSignal(SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_IPAUTONOSIGNAL XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_IPAUTONOSIGNAL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

E_APIXC_ReturnValue MDrv_SC_ForceInterlaceInPMode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);

    gSrcInfo[eWindow].Status2.bForceInterlaceInPMode = bEnable;

    if (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_SW)
    {
        if (bEnable)
        {
            if (eWindow == MAIN_WINDOW)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, BIT(1) | BIT(0), BIT(1) | BIT(0) | BIT(8));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, BIT(1) | BIT(0), BIT(1) | BIT(0) | BIT(8));
            }
        }
        else
        {
            if (eWindow == MAIN_WINDOW)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, 0x00 , BIT(1) | BIT(0));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_21_L, 0x00 , BIT(1) | BIT(0));
            }
        }
    }
    else if (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_HW)
    {
        if(bEnable)
        {
            // memsyn interlace mode invert
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, BIT(10), BIT(10));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_05_L, 0, BIT(10));
        }
    }
    else // ((MDrv_XC_GetForceiSupportType() == ForceI_AUTO))
    {
        // don't have to set anything, HW will auto do it.
    }

    _XC_RETURN(pInstance);

    return E_APIXC_RET_OK;

}

MS_BOOL MDrv_SC_GetInterlaceInPModeStatus(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return gSrcInfo[eWindow].Status2.bForceInterlaceInPMode;
}

//-------------------------------------------------------------------------------------------------
/// Check whether Auto gain result is ready or not
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsAutoGainResultReady(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bAuto_gain;
    _XC_ENTRY(pInstance);
    bAuto_gain = Hal_SC_ip_is_auto_gain_result_ready(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return bAuto_gain;// Hal_SC_ip_is_auto_gain_result_ready(eWindow);
}

//-------------------------------------------------------------------------------------------------
/// Get auto gain min value status
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_GetAutoGainMinValueStatus(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_ip_auto_gain_min_value_status(pInstance, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// Check whether Auto gain max value result
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_GetAutoGainMaxValueStatus(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_ip_auto_gain_max_value_status(pInstance, eWindow);
}


//-------------------------------------------------------------------------------------------------
/// Get the average data from hardware report
/// @return @ref MS_AUTOADC_TYPE
//-------------------------------------------------------------------------------------------------
MS_AUTOADC_TYPE MApi_XC_GetAverageDataFromHWReport(void *pInstance)
{
    MS_U16 u16MaxR,u16MaxG,u16MaxB;
    MS_U16 u16MinR,u16MinG,u16MinB;
    MS_AUTOADC_TYPE ptAdcData;
    MS_U8 u8AvgCnt;
    MS_U8 i;
    MS_U8 u8TimeoutCnt = 0;
    //MS_U8 u8VSyncTime; // VSync time
    //u8VSyncTime = msAPI_Scaler_CalculateVSyncTime(g_wVerticalTotal, g_wHorizontalPeriod); // get VSync time

    ptAdcData.u16CH_AVG[0] = ptAdcData.u16CH_AVG[1] = ptAdcData.u16CH_AVG[2] = 0;
    u8AvgCnt = 4;
    Hal_SC_autogain_enable(pInstance, ENABLE, MAIN_WINDOW);       //auto gain enable
    msg_sc_ip( printf("check rdy bit\n") );
    //MsOS_DelayTask(u8VSyncTime * 3); // wait stable

    //Auto Gain Result Ready. #0: Result not ready.

    for(i=0; i<u8AvgCnt; i++)
    {
        for(u8TimeoutCnt = 0; u8TimeoutCnt < 10; u8TimeoutCnt++)
        {
            while(Hal_SC_autogain_status(pInstance, MAIN_WINDOW) == 1);       // skip 1st v blanking
            while(Hal_SC_autogain_status(pInstance, MAIN_WINDOW) == 0);       // wait until next v blanking
            // ATG ready, read min/max RGB
            u16MinR = Hal_SC_rgb_average_info(pInstance, R_CHANNEL_MIN, MAIN_WINDOW);
            u16MaxR = Hal_SC_rgb_average_info(pInstance, R_CHANNEL_MAX, MAIN_WINDOW);
            u16MinG = Hal_SC_rgb_average_info(pInstance, G_CHANNEL_MIN, MAIN_WINDOW);
            u16MaxG = Hal_SC_rgb_average_info(pInstance, G_CHANNEL_MAX, MAIN_WINDOW);
            u16MinB = Hal_SC_rgb_average_info(pInstance, B_CHANNEL_MIN, MAIN_WINDOW);
            u16MaxB = Hal_SC_rgb_average_info(pInstance, B_CHANNEL_MAX, MAIN_WINDOW);

            if(Hal_SC_autogain_status(pInstance, MAIN_WINDOW))
            {

                ptAdcData.u16CH_AVG[0]+=(u16MinR+u16MaxR+1)/2;
                ptAdcData.u16CH_AVG[1]+=(u16MinG+u16MaxG+1)/2;
                ptAdcData.u16CH_AVG[2]+=(u16MinB+u16MaxB+1)/2;

                msg_sc_ip( printf("MAX[%04d,%04d,%04d]\n",u16MaxR,u16MaxG,u16MaxB) );
                msg_sc_ip( printf("MIN[%04d,%04d,%04d]\n",u16MinR,u16MinG,u16MinR) );
                break;
            }
            else
            {
                msg_sc_ip( printf("Avg err, again\n") );
            }
        }
    }
    ptAdcData.u16CH_AVG[0] /= u8AvgCnt;
    ptAdcData.u16CH_AVG[1] /= u8AvgCnt;
    ptAdcData.u16CH_AVG[2] /= u8AvgCnt;

    msg_sc_ip( printf("ptAdcData[%04d,%04d,%04d]\n",ptAdcData.u16CH_AVG[0],ptAdcData.u16CH_AVG[1],ptAdcData.u16CH_AVG[2]) );
    return ptAdcData;
}

#if DATA_CONVERT_NEON_OPTIMIZE_ENABLE
//ARM Neon Optimize function for XC_FRC buffer data to normal ARGB8888
void MDrv_XC_FRC_Rgb888_2_Argb8888_Neon(MS_U32 u32DstAddr, MS_U32 u32SrcAddr, MS_U32 u32PixelNum)
{
        __asm__ __volatile__(
                " LDR             R0, %0 \t\n"
                " LDR             R1, %1 \t\n"
                " LDR             R2, %2 \t\n"
                " VMOV.I8         D3, #0xFF \t\n"             //Set A=0xFF
"1:               SUB             R2, R2, #16 \t\n"           //Convert 16 pixel eanch time
"2:               VLDR            D1, [R1] \t\n"              //load G
                " VLDR            D0, [R1,#16] \t\n"          //load B
                " VLDR            D2, [R1,#32] \t\n"          //load R
                " VST4.8          {D0,D1,D2,D3}, [R0]! \t\n"  //Store 8 pixel's as ARGB8888
                " ADD             R1, R1, #8 \t\n"
                " TST             R0, #0x3F \t\n"             //if we finished 16 pixel convert ?
                " BNE             2b \t\n"                    //No, jump
                " ADD             R1, R1, #32 \t\n"           //Jump to next 16 pixel
                " ORRS            R2, R2, #0x0 \t\n"          //Is all pixel finished
                " BNE             1b \t\n"                    //No, jump
                : :"m"(u32DstAddr), "m"(u32SrcAddr), "m"(u32PixelNum)
                :"r0","r1","r2","d0","d1","d2","d3","cc","memory");
}

//ARM Neon Optimize function for XC_FRC buffer data to normal ABGR8888
void MDrv_XC_FRC_Rgb888_2_Abgr8888_Neon(MS_VIRT u32DstAddr, MS_VIRT u32SrcAddr, MS_U32 u32PixelNum)
{
        __asm__ __volatile__(
                " LDR             R0, %0 \t\n"
                " LDR             R1, %1 \t\n"
                " LDR             R2, %2 \t\n"
                " VMOV.I8         D3, #0xFF \t\n"             //Set A=0xFF
"1:               SUB             R2, R2, #16 \t\n"           //Convert 16 pixel eanch time
"2:               VLDR            D1, [R1] \t\n"              //load G
                " VLDR            D2, [R1,#16] \t\n"          //load B
                " VLDR            D0, [R1,#32] \t\n"          //load R
                " VST4.8          {D0,D1,D2,D3}, [R0]! \t\n"  //Store 8 pixel's as ABGR8888
                " ADD             R1, R1, #8 \t\n"
                " TST             R0, #0x3F \t\n"             //if we finished 16 pixel convert ?
                " BNE             2b \t\n"                    //No, jump
                " ADD             R1, R1, #32 \t\n"           //Jump to next 16 pixel
                " ORRS            R2, R2, #0x0 \t\n"          //Is all pixel finished
                " BNE             1b \t\n"                    //No, jump
                : :"m"(u32DstAddr), "m"(u32SrcAddr), "m"(u32PixelNum)
                :"r0","r1","r2","d0","d1","d2","d3","cc","memory");
}
#endif

static void MDrv_XC_Access_BufferData(void *pInstance, E_XC_OUTPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pMemRect, void *pDataBuf, MS_WINDOW_TYPE *pSrcRect, SCALER_WIN eWindow, MS_BOOL bIsReadMode)
{
    if (pDataBuf == NULL)
    {
        MS_CRITICAL_MSG(printf("Data buffer is NULL! \n"));
        return;
    }

    if (  (eBufType != E_XC_OUTPUTDATA_FRC_RGB8BITS)
        &&((pMemRect == NULL) || ((!bIsReadMode) && (pSrcRect == NULL))))
    {
        MS_CRITICAL_MSG(printf("---%s:%d Data rectangle or memory rectangle is NULL! \n", __FUNCTION__, __LINE__));
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    void *pPoint;
    volatile MS_PHY pMEM;
    volatile MS_U8 *pMEM_temp;

    MS_U16 u16LBOffset = 0;
    MS_U16 u16PointSize = 0;
    MS_U8 u8BytePerTwoPixel = 0;
    MS_U16 x, y;
    MS_U16 x0, x1, y0, y1;
    MS_U8 u8MemFmt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_02_L) & 0x0F;
    MS_U8 u8FieldMode = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L) & 0x07;
    MS_U8 u8FieldNum;

    MS_BOOL bHmirror, bVmirror;

    MS_PHY u32Y_Offset, u32X_Offset;
    MS_U8 u8Discardpixels = 0;
    MS_U8 u8Offsetpixels = 0;
    MS_U8 u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;

    MS_U16 xRegion, accessStart;
    MS_S8 yNext, xNext, accessNext;
    MS_U8 u8packinginterlacedshift = 0;
    MS_U16 u16YCSeparateOffset = BYTE_PER_WORD;
    MS_U8 u8FrameNum;
    MS_U32 u32JumpOffset = 0;
    MS_U32 u32Accesspixel;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    SC_FRAMEBUF_INFO_t FBInfo;
    SC_MIUSEL_t FBMiuSelInfo;
    MS_PHY u32Main_FB_Start_Addr = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr;
    MS_PHY u32Sub_FB_Start_Addr = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr;

    Hal_SC_get_framebuf_Info(pInstance, &FBInfo, eWindow);
    Hal_SC_get_miusel(pInstance, &FBMiuSelInfo);

    if(u32Main_FB_Start_Addr >= HAL_MIU1_BASE)
    {
        u32Main_FB_Start_Addr -= HAL_MIU1_BASE;
    }

    if(u32Sub_FB_Start_Addr >= HAL_MIU1_BASE)
    {
        u32Sub_FB_Start_Addr -= HAL_MIU1_BASE;
    }


    if(eBufType <= E_XC_OUTPUTDATA_4228BITS)
    {
        if((pMemRect!=NULL) &&
           ((pMemRect->x >= gSrcInfo[eWindow].u16H_SizeAfterPreScaling) ||
      ((pMemRect->x + pMemRect->width) > gSrcInfo[eWindow].u16H_SizeAfterPreScaling) ||
       (pMemRect->y >= gSrcInfo[eWindow].u16V_SizeAfterPreScaling ) ||
            ((pMemRect->y + pMemRect->height) > gSrcInfo[eWindow].u16V_SizeAfterPreScaling)))
    {
            MS_CRITICAL_MSG(printf("---%s:%d invalid parameter!\n", __FUNCTION__, __LINE__));
            return;
        }
    }
    else if(eBufType <= E_XC_OUTPUTDATA_FRC_4228BITS)
    {
        if(  (pMemRect!=NULL)
           &&(  ((pMemRect->x + pMemRect->width) > gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width)
              ||((pMemRect->y + pMemRect->height) > gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Height)
              ||(pMemRect->x >= gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width)
              ||(pMemRect->y >= gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Height)))
        {
            MS_CRITICAL_MSG(printf("---%s:%d invalid parameter!\n", __FUNCTION__, __LINE__));
            return;
        }
    }

    if(eWindow == MAIN_WINDOW)
    {
        bHmirror = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, BIT(12)) >> 12;
        bVmirror = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, BIT(13)) >> 13;
    }
    else
    {
        bHmirror = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L, BIT(12)) >> 12;
        bVmirror = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L, BIT(13)) >> 13;
    }

    u8FrameNum = FBInfo.u8FBNum;

    //F2 select 8-field(0), 10-field(1), 12-field(2), 14-field(3), 16-field(4)  mem config
    u8FieldNum = 8 + u8FieldMode*2;
    if(eBufType == E_XC_OUTPUTDATA_FRC_RGB8BITS)
    {
#if 0 //FRC_INSIDE
        MS_VIRT *pu32Dstaddr=NULL;
        MS_U32 u32PixelNum=0;
        MS_VIRT *pu8Srcaddr=NULL;
        MS_U8 u8OutputBytePerPixel=4,u8FRCMemBytePerPixel=3, u8ReadBank=0; //read bank=Wbank number - 1;
        MS_PHY u32DstPhyAddr = (MS_PHY)pDataBuf;
        MS_PHY u32SrcPhyAddr = 0, u32Offset = 0;
        if(u32DstPhyAddr==0)
        {
            printf("---%s:%d Wrong Dst address = %lx\n", __FUNCTION__, __LINE__, (MS_U32)u32DstPhyAddr);
            return;
        }

        if(u32DstPhyAddr >= HAL_MIU1_BASE)
        {
            u32DstPhyAddr -= HAL_MIU1_BASE;
            //Hal_SC_set_miusel(1); //Need implement for XC_FRC
        }
        else
        {
            //Hal_SC_set_miusel(0); //Need implement for XC_FRC
        }

        u32Offset = (gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width + FRC_OFFSET_PIXEL_ALIGNMENT - 1) & (~(FRC_OFFSET_PIXEL_ALIGNMENT-1));
        if (gSrcInfo[eWindow].Status2.bInFRCMode == FALSE)
        {
            //Non-FRC case, set FRC_IP to enable capture
            //only for FRC image capture function, set to 1 frame mode to save memory
            u8FrameNum = 1;
            u32PixelNum = gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width*
                           gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Height*
                           u8FrameNum;
            u32SrcPhyAddr = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_MEMC_MemAddr;
            if((u32SrcPhyAddr==0) || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_MEMC_MemSize < u32PixelNum*u8FRCMemBytePerPixel))
            {
                printf("---%s:%d Wrong Src address = %lx, %lx\n", __FUNCTION__, __LINE__, (MS_U32)u32SrcPhyAddr, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u32FRC_MEMC_MemSize);
                return;
            }

            if(u32SrcPhyAddr >= HAL_MIU1_BASE)
            {
                u32SrcPhyAddr -= HAL_MIU1_BASE;
                //Hal_SC_set_miusel(1); //Need implement for XC_FRC
            }
            else
            {
                //Hal_SC_set_miusel(0); //Need implement for XC_FRC
            }

            SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK43_08_L, u32SrcPhyAddr/BYTE_PER_WORD);//ipm_base
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK43_01_L, BIT(11), BIT(11)); //Freeze image first to avoid write a wrong base addr
            MDrv_XC_wait_output_vsync(pInstance, 1, DEFAULT_OUTPUT_VSYNC_TIMEOUT, eWindow);//Wait base addr and freeze take effect after vsync

            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK40_68_L, 0x5000);// arb
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK40_69_L, 0x0420);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK40_6A_L, 0x5000);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK40_6B_L, 0x0420);// arb

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK43_50_L, BIT(15), BIT(15)); //by pass compress mode
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK43_02_L, 0x0311);// mem mode user define enable: [0][4] 444 enable (1:444 0:422),
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK43_19_L, u8FrameNum, 0x1F);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK43_0E_L, gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width);//ipm offset
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK43_0F_L, gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width);//ipm fetch
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK43_07_L, 0x88, 0xFF); //enable frc_ipm to miu
        }
        else
        {
            u32SrcPhyAddr = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK43_08_L)*BYTE_PER_WORD;
            u8FrameNum = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK43_19_L) & 0x1F;
            u32PixelNum = gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Width*
                           gSrcInfo[eWindow].Status2.stXCPanelDefaultTiming.u16Height*
                           u8FrameNum;
        }

        if((u32SrcPhyAddr == 0) || (u32PixelNum == 0))
        {
            printf("---%s %d:Setting error=%lx,%lx\n", __FUNCTION__, __LINE__, u32SrcPhyAddr, u32PixelNum);
            return;
        }
        UNUSED(u8ReadBank);//Add readbank logic here: read bank=Wbank number - 1
        pu32Dstaddr = (MS_VIRT *)MsOS_PA2KSEG1(u32DstPhyAddr);
        if(0 == pu32Dstaddr)
        {
            printf("%s :pu32Dstaddr_1 = 0!!,error!!!\n", __FUNCTION__);
            assert(pu32Dstaddr != 0);
        }
        pu8Srcaddr = (MS_VIRT *)MsOS_PA2KSEG1(u32SrcPhyAddr);
        if(0 == pu8Srcaddr)
        {
            printf("%s :pu8Srcaddr_1 = 0!,error!!!\n", __FUNCTION__);
            assert(pu8Srcaddr != 0);
        }
#ifdef MSOS_TYPE_LINUX
        if(pu8Srcaddr == NULL)
        {
            if (!MsOS_MPool_Mapping(0, u32SrcPhyAddr,  u32PixelNum*u8FRCMemBytePerPixel, 1))
            {
                printf("---%s:%d error, map fail\n", __FUNCTION__, __LINE__);
                return;
            }
            pu8Srcaddr = (MS_VIRT *)MsOS_PA2KSEG1(u32SrcPhyAddr);
            if(0 == pu8Srcaddr)
            {
                printf("%s :pu8Srcaddr_2 = 0!,error!!!\n", __FUNCTION__);
                assert(pu8Srcaddr != 0);
            }
        }

        if(pu32Dstaddr == NULL)
        {
            if (!MsOS_MPool_Mapping(0, u32DstPhyAddr,  u32PixelNum*u8OutputBytePerPixel, 1))
            {
                printf("---%s:%d error, map fail\n", __FUNCTION__, __LINE__);
                return;
            }
            pu32Dstaddr = (MS_VIRT *)MsOS_PA2KSEG1(u32DstPhyAddr);
            if(0 == pu32Dstaddr)
            {
                printf("%s :pu32Dstaddr_2 = 0!,error!!!\n", __FUNCTION__);
                assert(pu32Dstaddr != 0);
            }
        }
#endif
        if((pu32Dstaddr == NULL) || (pu8Srcaddr == NULL))
        {
            printf("---%s:%d error: Address[%lx,%lx] map fail\n", __FUNCTION__, __LINE__, (MS_U32)pu8Srcaddr, (MS_U32)pu32Dstaddr);
            return;
        }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK43_01_L, 0, BIT(11));//Un freeze to start capture
        MDrv_XC_wait_output_vsync(pInstance, u8FrameNum+1, (u8FrameNum+1)*DEFAULT_OUTPUT_VSYNC_TIMEOUT, eWindow);//If multi frame, try use u8ReadBank to get data to skip wait vsync
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK43_01_L, BIT(11), BIT(11));//Capture done, re-freeze to do image process
        MDrv_XC_wait_output_vsync(pInstance, 1, DEFAULT_OUTPUT_VSYNC_TIMEOUT, eWindow);//Wait freeze take effect after vsync
        MsOS_ReadMemory();//Flush memory
#if DATA_CONVERT_NEON_OPTIMIZE_ENABLE
        MDrv_XC_FRC_Rgb888_2_Abgr8888_Neon(pu32Dstaddr, pu8Srcaddr, u32PixelNum);
#else
        MS_U8 u8R=0, u8G=0, u8B=0;
        MS_U32 i=0, j=0, u32ARGB=0;
        UNUSED(u8OutputBytePerPixel);
        do
        {
            for(j=0; j<BYTE_PER_WORD; j++)
            {
                u8G = *(pu8Srcaddr+j);
                u8B = *(pu8Srcaddr+BYTE_PER_WORD*u8FrameNum+j);
                u8R = *(pu8Srcaddr+BYTE_PER_WORD*(u8FrameNum<<1)+j);
                u32ARGB = (0xFF<<24) | (u8R<<16) | (u8G<<8) | (u8B);
                *(pu32Dstaddr+j)=u32ARGB;
            }
            i+=BYTE_PER_WORD;
            pu8Srcaddr  += BYTE_PER_WORD*3*u8FrameNum; // 3 byte/pixel and FramNum frames
            pu32Dstaddr += BYTE_PER_WORD;
        }while(i<u32PixelNum);
#endif
        MsOS_ReadMemory();
#endif //FRC inside
        return;
    }

    //Non-FRC capture case
    MsOS_ReadMemory();
#if (_FIELD_PACKING_MODE_SUPPORTED)
    if (FBInfo.bMemFormat422 == TRUE)
    {
        FB_DBG(printf("source is YUV 422 mode\n"));
        u16YCSeparateOffset = u8FrameNum * BYTE_PER_WORD;
        u8BytePerTwoPixel = FBInfo.u8BitPerPixel * 2 / 8;

        if(4 == u8BytePerTwoPixel)
        {
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;

            if ( FBInfo.bLinearAddrMode )
                u32JumpOffset = 0;
            else
                u32JumpOffset = BYTE_PER_WORD * (u8FrameNum + u8FrameNum -1);

        }
        else if(5 == u8BytePerTwoPixel)
        {
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;

            if ( FBInfo.bLinearAddrMode )
                u32JumpOffset = 0;
            else
                u32JumpOffset = BYTE_PER_WORD * (u8FrameNum * 2 + u8FrameNum -1);
        }
        else if(6 == u8BytePerTwoPixel)
        {
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;

            if ( FBInfo.bLinearAddrMode )
                u32JumpOffset = 0;
            else
                u32JumpOffset = BYTE_PER_WORD * (u8FrameNum * 3 + u8FrameNum -1);
        }
    }
    else
    {
        FB_DBG(printf("source is YUV 444 mode, bit: %u\n", FBInfo.u8BitPerPixel));
        if( 1 == u8MemFmt)
            u8BytePerTwoPixel = 6;
        else
            u8BytePerTwoPixel = FBInfo.u8BitPerPixel * 2 / 8;

        u16YCSeparateOffset = u8FrameNum * BYTE_PER_WORD;

        u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT/2;

    }
#else
    if (FBInfo.bMemFormat422 == TRUE)
    {
        FB_DBG(printf("source is YUV 422 mode\n"));
        //we will enable BK12_02[9:8] to keep same memory layout, hence, BytePerPixel is 3
        // F0     F0     F2     F2     F0   F0   F2   F2   F0     F0     F2     F2
        // Y8/C8  Y8/C8  Y8/C8  Y8/C8  LSB  YM4  LSB  YM4  Y8/C8  Y8/C8  Y8/C8  Y8/C8=>24bit Y10C10M4
        // Y8/C8  Y8/C8  Y8/C8  Y8/C8  LSB  XXX  LSB  XXX  Y8/C8  Y8/C8  Y8/C8  Y8/C8=>20bit Y10C10
        // Y8/C8  Y8/C8  Y8/C8  Y8/C8  XXX  YM4  XXX  YM4  Y8/C8  Y8/C8  Y8/C8  Y8/C8=>20bit Y8YC8M4
        // Y8/C8  Y8/C8  Y8/C8  Y8/C8  XXX  XXX  XXX  XXX  Y8/C8  Y8/C8  Y8/C8  Y8/C8=>16bit Y8C8
        u8BytePerTwoPixel = FBInfo.u8BitPerPixel * 2 / 8;

        if(15 == u8MemFmt)
        {
            u16YCSeparateOffset = (u8FieldNum/2) * BYTE_PER_WORD;
            u8FrameNum = u8FieldNum/2;
            u8BytePerTwoPixel = 6;
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;
            if ( FBInfo.bLinearAddrMode )
                u32JumpOffset = BYTE_PER_WORD * 2;
            else
                u32JumpOffset = BYTE_PER_WORD * 2 * (u8FieldNum -1); // (u8FieldNum/2 -1)*2 MIU + u8FieldNum/2 MIU (for LSB) + u8FieldNum/2 MIU (for Motion)
        }
        else if(4 == u8BytePerTwoPixel)
        {
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT /2;
            if ( FBInfo.bLinearAddrMode )
            {
                u32JumpOffset = 0;
            }
            else
            {
                u32JumpOffset = BYTE_PER_WORD * 2;

                if (u8FrameNum >= 4)
                    u8FrameNum = u8FrameNum / 2 ;
            }
        }
        else if(5 == u8BytePerTwoPixel)
        {
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;
            if ( FBInfo.bLinearAddrMode )
            {
                u32JumpOffset = BYTE_PER_WORD;
            }
            else
            {
                u32JumpOffset = BYTE_PER_WORD * 4;

                if (u8FrameNum >= 4)
                    u8FrameNum = u8FrameNum / 2 ;
            }
        }
        else if(6 == u8BytePerTwoPixel)
        {
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;
            if ( FBInfo.bLinearAddrMode )
            {
                u32JumpOffset = BYTE_PER_WORD * 2;
            }
            else
            {
                u32JumpOffset = BYTE_PER_WORD * 6;

                if (u8FrameNum >= 4)
                    u8FrameNum = u8FrameNum / 2 ;
            }
        }
    }
    else
    {
        FB_DBG(printf("source is YUV 444 mode, bit: %u\n", FBInfo.u8BitPerPixel));
        u8BytePerTwoPixel = FBInfo.u8BitPerPixel * 2 / 8;

        if ( FBInfo.bLinearAddrMode )
            u32JumpOffset = 0;
        else
            u32JumpOffset = BYTE_PER_WORD * 2;

        if(32 == FBInfo.u8BitPerPixel)
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT/2;
        else
            u8pixels_per_unit = OFFSET_PIXEL_ALIGNMENT;
    }
#endif

    if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
    {
        u16PointSize = sizeof(MS_PIXEL_32BIT);
    }
    else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
    {
        u16PointSize = sizeof(MS_PIXEL_24BIT);
    }
    else if(eBufType == E_XC_OUTPUTDATA_4228BITS)
    {
        u16PointSize = sizeof(MS_PIXEL_422_8BIT);
    }

    x0 = pMemRect->x;
    x1 = pMemRect->x + pMemRect->width -1;
    y0 = pMemRect->y;
    y1 = pMemRect->y + pMemRect->height -1;

    // calculate the MIU counts need to access for H line
    xRegion = x1/u8pixels_per_unit - x0/u8pixels_per_unit + 1;

    accessStart = 0;
    yNext = 1;
    xNext = u8pixels_per_unit;
    accessNext = 1;

    // for mirror mode calculation
    //
    // (x0,y0)                                   (x1, y1)
    //   -----------    mirror mode     -----------
    //   |                |       ------->     |                |
    //   -----------                           -----------
    //                   (x1, y1)                                (x0, y0)
    //                                             access from x0 to x1, from y0 to y1, decrease to next point
    if (bHmirror)
    {
        x0 = (gSrcInfo[eWindow].u16H_SizeAfterPreScaling - pMemRect->x) -1;
        x1 = gSrcInfo[eWindow].u16H_SizeAfterPreScaling - (pMemRect->x + pMemRect->width);

        xRegion = x0/u8pixels_per_unit - x1/u8pixels_per_unit + 1;
        accessStart = u8pixels_per_unit -1;
        xNext = -1 * u8pixels_per_unit;
        accessNext = -1;
    }
    if (bVmirror)
    {
        y0 = gSrcInfo[eWindow].u16V_SizeAfterPreScaling  - pMemRect->y -1;
        y1 = gSrcInfo[eWindow].u16V_SizeAfterPreScaling  - (pMemRect->y + pMemRect->height);

        yNext = -1;
    }

    FB_DBG(printf("real region: (%u, %u)-(%u, %d) \n", x0, y0, x1, y1));

    u16LBOffset = x0 % u8pixels_per_unit;

    pPoint = pDataBuf;

    MS_U16 yCount, xCount, accessCount;
    for (y = y0, yCount = 0; yCount < pMemRect->height ; y = y + yNext, yCount+=1)
    {
        if(bIsReadMode)
            pPoint = (MS_U8*)pDataBuf + (yCount % pMemRect->height) * pMemRect->width * u16PointSize;
        else
            pPoint = (MS_U8*)pDataBuf + (yCount % pSrcRect->height) * pSrcRect->width * u16PointSize;

        MS_PIXEL_32BIT *pPoint32 = (MS_PIXEL_32BIT*)pPoint;
        MS_PIXEL_24BIT *pPoint24 = (MS_PIXEL_24BIT*)pPoint;
        MS_PIXEL_422_8BIT *pPoint422_8 = (MS_PIXEL_422_8BIT*)pPoint;

        // Y offset
        if (gSrcInfo[eWindow].bInterlace)
        {
            u32Y_Offset = (MS_PHY)y/2 * FBInfo.u16IPMOffset * (MS_U32)u8BytePerTwoPixel/2 * u8FrameNum;

#if (_FIELD_PACKING_MODE_SUPPORTED)
            if(eWindow == MAIN_WINDOW)
                u32Y_Offset += u32Main_FB_Start_Addr;
            else
                u32Y_Offset += u32Sub_FB_Start_Addr;

            if (y & 0x1)
                u8packinginterlacedshift = 0;
            else
                u8packinginterlacedshift = BYTE_PER_WORD;
#else
            if(eWindow == MAIN_WINDOW)
            {
                if (y & 0x1)
                    u32Y_Offset += u32Main_FB_Start_Addr;
                else
                    u32Y_Offset += u32Main_FB_Start_Addr + (FBInfo.u32IPMBase1 - FBInfo.u32IPMBase0);
            }
            else
            {
                if (y & 0x1)
                    u32Y_Offset += u32Sub_FB_Start_Addr;
                else
                    u32Y_Offset += u32Sub_FB_Start_Addr + (FBInfo.u32IPMBase1 - FBInfo.u32IPMBase0);
            }
#endif
        }
        else
        {
            if ( FBInfo.bLinearAddrMode )
                u32Y_Offset = (MS_PHY)y * FBInfo.u16IPMOffset * (MS_U32)u8BytePerTwoPixel/2;
            else
                u32Y_Offset = (MS_PHY)y * FBInfo.u16IPMOffset * (MS_U32)u8BytePerTwoPixel/2 * u8FrameNum;

            if(eWindow == MAIN_WINDOW)
                u32Y_Offset += u32Main_FB_Start_Addr;
            else
                u32Y_Offset += u32Sub_FB_Start_Addr;
        }

        FB_DBG(printf("[1]u32Y_Offset:0x%x, u16IPMOffset:%u, y:%u, u8BytePerTwoPixel:%u, u8FBNum:%u\n",
            (int)u32Y_Offset, FBInfo.u16IPMOffset, y, u8BytePerTwoPixel, FBInfo.u8FBNum));

        for (x = x0 - u16LBOffset, xCount = 0; xCount < xRegion; x = x + xNext, xCount+=1)
        {
            // X offset
            if(FBInfo.bLinearAddrMode)
                u32X_Offset = x * (MS_PHY)u8BytePerTwoPixel/2;
            else
                u32X_Offset = x * (MS_PHY)u8BytePerTwoPixel/2 * u8FrameNum;

            FB_DBG(printf("[2]u32X_Offset=0x%x, align(x)=%u\n", (int)u32X_Offset, x));

            pMEM = 0UL + u32Y_Offset + u32X_Offset;

            if((eWindow == MAIN_WINDOW)? FBMiuSelInfo.u8MainFBSel : FBMiuSelInfo.u8SubFBSel)
            {
                pMEM += HAL_MIU1_BASE;
            }

            FB_DBG(printf("[3]pMEM=0x%x, IPMBase0:0x%x, x:%u, y:%u \n", (int)pMEM, (int)FBInfo.u32IPMBase0, x, y));

            pMEM_temp = (MS_U8 *)MS_PA2KSEG1(pMEM);

            if (FBInfo.bMemFormat422 == TRUE)
            {
                if(!bHmirror)
                {
                    //  |<----- MIU block--- ---->|<----- MIU block--- ---->|
                    // 1st  x                          x0              x1
                    //  |<--u8Offsetpixels-->|<-->|<->|<--u8Discardpixels-->|
                    //                                          2nd x
                    //  x is at start point of MIU block includes x0, then increase to next start point of MIU block

                    if(x<x0)
                        u8Offsetpixels = u16LBOffset;
                    else
                        u8Offsetpixels = 0;

                    if(x+u8pixels_per_unit > x1)
                        u8Discardpixels = x+u8pixels_per_unit - x1;
                    else
                        u8Discardpixels = 0;
                }
                else
                {
                    // Mirror mode:
                    //  |<----- MIU block--- ---->|<----- MIU block--- ---->|
                    //                                  x1     1st  x     x0
                    //  |<--u8Offsetpixels-->|<-->|<--->|<-u8Discardpixels->|
                    // 2nd x
                    //  x is at start point of MIU block includes x0, then decrease to previous start point of MIU block

                    if(x<x1)
                        u8Offsetpixels = x1 - x;
                    else
                        u8Offsetpixels = 0;

                    if(x+u8pixels_per_unit > x0)
                        u8Discardpixels = x+u8pixels_per_unit - x0;
                    else
                        u8Discardpixels = 0;
                }

                int index = accessStart;
                MS_U16 u16WriteModeSrcPoint = 0;
                for(accessCount = 0; accessCount < u8pixels_per_unit; index = index + accessNext, accessCount+=1)
                {
                    if((index < u8Offsetpixels) || (index > (u8pixels_per_unit - u8Discardpixels)))
                        continue;

                    if(!bIsReadMode)
                    {
                        if(!bHmirror)
                        {
                            u16WriteModeSrcPoint = (xCount * u8pixels_per_unit + (index - u16LBOffset)) % pSrcRect->width;
                        }
                        else
                        {
                            u16WriteModeSrcPoint = (xCount * u8pixels_per_unit + (u16LBOffset - index)) % pSrcRect->width;
                        }
                    }
#if (_FIELD_PACKING_MODE_SUPPORTED)
                    if(true)
#else
                    if(FBInfo.bYCSeparate)
#endif
                    {
                        if(index < BYTE_PER_WORD)
                        {
                            u32Accesspixel = index;
                        }
                        else
                        {
#if (_FIELD_PACKING_MODE_SUPPORTED)
                            u32Accesspixel = index + u32JumpOffset;
#else
                            u32Accesspixel = index + BYTE_PER_WORD + u32JumpOffset;
#endif
                        }
                        u32Accesspixel += u8packinginterlacedshift;

                        if(bIsReadMode)
                        {
                            // for get buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // source 422 format, output R10G10B10 mode
                                pPoint32->G_Y = pMEM_temp[u32Accesspixel] << 2;
                                if (index&0x1)
                                {
                                    pPoint32->B_Cb = pMEM_temp[u32Accesspixel-1+u16YCSeparateOffset] << 2;
                                    pPoint32->R_Cr = pMEM_temp[u32Accesspixel+u16YCSeparateOffset] << 2;
                                }
                                else {
                                    pPoint32->B_Cb = pMEM_temp[u32Accesspixel+u16YCSeparateOffset] << 2;
                                    pPoint32->R_Cr = pMEM_temp[u32Accesspixel+1+u16YCSeparateOffset] << 2;
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // source 422 format, output R8G8B8 mode
                                pPoint24->G_Y = pMEM_temp[u32Accesspixel];
                                if (index&0x1)
                                {
                                    pPoint24->B_Cb = pMEM_temp[u32Accesspixel-1+u16YCSeparateOffset];
                                    pPoint24->R_Cr = pMEM_temp[u32Accesspixel+u16YCSeparateOffset];
                                }
                                else {
                                    pPoint24->B_Cb = pMEM_temp[u32Accesspixel+u16YCSeparateOffset];
                                    pPoint24->R_Cr = pMEM_temp[u32Accesspixel+1+u16YCSeparateOffset];
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_4228BITS)
                            {
                                // source 422 format, output Y8CbCr8 mode
                                pPoint422_8->u8G_Y = pMEM_temp[u32Accesspixel];
                                pPoint422_8->u8BR_CbCr = pMEM_temp[u32Accesspixel+u16YCSeparateOffset];
                            }

                        }
                        else
                        {
                            // for set buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // target 422 format, input 10G10B10 mode
                                pMEM_temp[u32Accesspixel] = pPoint32[u16WriteModeSrcPoint].G_Y >> 2;
                                if (index&0x1)
                                {
                                    pMEM_temp[u32Accesspixel-1+u16YCSeparateOffset] = pPoint32[u16WriteModeSrcPoint].B_Cb >> 2;
                                    pMEM_temp[u32Accesspixel+u16YCSeparateOffset] = pPoint32[u16WriteModeSrcPoint].R_Cr >> 2;
                                }
                                else {
                                    pMEM_temp[u32Accesspixel+u16YCSeparateOffset] = pPoint32[u16WriteModeSrcPoint].B_Cb >> 2;
                                    pMEM_temp[u32Accesspixel+1+u16YCSeparateOffset] = pPoint32[u16WriteModeSrcPoint].R_Cr >> 2;
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // target 422 format, input R8G8B8 mode
                                pMEM_temp[u32Accesspixel] = pPoint24[u16WriteModeSrcPoint].G_Y;
                                if (index&0x1)
                                {
                                    pMEM_temp[u32Accesspixel-1+u16YCSeparateOffset] = pPoint24[u16WriteModeSrcPoint].B_Cb;
                                    pMEM_temp[u32Accesspixel+u16YCSeparateOffset] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                                }
                                else {
                                    pMEM_temp[u32Accesspixel+u16YCSeparateOffset] = pPoint24[u16WriteModeSrcPoint].B_Cb;
                                    pMEM_temp[u32Accesspixel+1+u16YCSeparateOffset] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_4228BITS)
                            {
                                // target 422 format, input Y8CbCr8 mode
                                pMEM_temp[u32Accesspixel] = pPoint422_8[u16WriteModeSrcPoint].u8G_Y;
                                pMEM_temp[u32Accesspixel+u16YCSeparateOffset] = pPoint422_8[u16WriteModeSrcPoint].u8BR_CbCr;
                            }
                        }
                    }
                    else
                    {
                        if(index < BYTE_PER_WORD)
                        {
                            u32Accesspixel = index*2;
                        }
                        else
                        {
                            u32Accesspixel = index*2 + u32JumpOffset;
                        }

                        if(bIsReadMode)
                        {
                            // for get buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // source 422 format, output R10G10B10 mode
                                pPoint32->G_Y = pMEM_temp[u32Accesspixel] << 2;
                                if (index&0x1) {
                                    pPoint32->B_Cb = pMEM_temp[u32Accesspixel-1] << 2;
                                    pPoint32->R_Cr = pMEM_temp[u32Accesspixel+1] << 2;
                                }
                                else {
                                    pPoint32->B_Cb = pMEM_temp[u32Accesspixel+1] << 2;
                                    pPoint32->R_Cr= pMEM_temp[u32Accesspixel+3] << 2;
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // source 422 format, output R8G8B8 mode
                                pPoint24->G_Y = pMEM_temp[u32Accesspixel];
                                if (index&0x1) {
                                    pPoint24->B_Cb = pMEM_temp[u32Accesspixel-1];
                                    pPoint24->R_Cr = pMEM_temp[u32Accesspixel+1];
                                }
                                else {
                                    pPoint24->B_Cb = pMEM_temp[u32Accesspixel+1];
                                    pPoint24->R_Cr= pMEM_temp[u32Accesspixel+3];
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_4228BITS)
                            {
                                // source 422 format, output Y8CbCr8 mode
                                pPoint422_8->u8G_Y = pMEM_temp[u32Accesspixel];
                                pPoint422_8->u8BR_CbCr = pMEM_temp[u32Accesspixel+1];
                            }
                        }
                        else
                        {
                            // for set buffer data mode,
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // target 422 format, input R10G10B10 mode
                                pMEM_temp[u32Accesspixel] = pPoint32[u16WriteModeSrcPoint].G_Y >> 2;
                                if (index&0x1) {
                                    pMEM_temp[u32Accesspixel-1] = pPoint32[u16WriteModeSrcPoint].B_Cb >> 2;
                                    pMEM_temp[u32Accesspixel+1] = pPoint32[u16WriteModeSrcPoint].R_Cr >> 2;
                                }
                                else {
                                    pMEM_temp[u32Accesspixel+1] = pPoint32[u16WriteModeSrcPoint].B_Cb >> 2;
                                    pMEM_temp[u32Accesspixel+3] = pPoint32[u16WriteModeSrcPoint].R_Cr>> 2;
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // target 422 format, inout R8G8B8 mode
                                pMEM_temp[u32Accesspixel] = pPoint24[u16WriteModeSrcPoint].G_Y;
                                if (index&0x1) {
                                    pMEM_temp[u32Accesspixel-1] = pPoint24[u16WriteModeSrcPoint].B_Cb;
                                    pMEM_temp[u32Accesspixel+1] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                                }
                                else {
                                    pMEM_temp[u32Accesspixel+1] = pPoint24[u16WriteModeSrcPoint].B_Cb;
                                    pMEM_temp[u32Accesspixel+3] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                                }
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_4228BITS)
                            {
                                // target 422 format, input Y8CbCr8 mode
                                pMEM_temp[u32Accesspixel] = pPoint422_8[u16WriteModeSrcPoint].u8G_Y;
                                pMEM_temp[u32Accesspixel+1] = pPoint422_8[u16WriteModeSrcPoint].u8BR_CbCr;
                            }
                        }
                    }

                    if(bIsReadMode)
                    {
                        if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                        {
                            FB_DBG(printf("==> index: %d, R: %d, G:%d, B: %d \n", index, pPoint32->R_Cr, pPoint32->G_Y, pPoint32->B_Cb));
                            pPoint32++;
                        }
                        else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                        {
                            FB_DBG(printf("==> index: %d, R: %d, G:%d, B: %d \n", index, pPoint24->R_Cr, pPoint24->G_Y, pPoint24->B_Cb));
                            pPoint24++;
                        }
                        else if(eBufType == E_XC_OUTPUTDATA_4228BITS)
                        {
                            FB_DBG(printf("==> index: %d, Y: %d, CbCr:%d \n", index, pPoint422_8->u8G_Y, pPoint422_8->u8BR_CbCr));
                            pPoint422_8++;
                        }
                    }

                }

            }
            else //444 case
            {
                if(!bHmirror)
                {
                    //  |<----- MIU block--- ---->|<----- MIU block--- ---->|
                    // 1st  x                          x0              x1
                    //  |<--u8Offsetpixels-->|<-->|<->|<--u8Discardpixels-->|
                    //                                          2nd x
                    //  x is at start point of MIU block includes x0, then increase to next start point of MIU block

                    if(x<x0)
                        u8Offsetpixels = u16LBOffset;
                    else
                        u8Offsetpixels = 0;

                    if(x+u8pixels_per_unit > x1)
                        u8Discardpixels = x+u8pixels_per_unit - x1;
                    else
                        u8Discardpixels = 0;
                }
                else
                {
                    // Mirror mode:
                    //  |<----- MIU block--- ---->|<----- MIU block--- ---->|
                    //                                  x1     1st  x     x0
                    //  |<--u8Offsetpixels-->|<-->|<--->|<-u8Discardpixels->|
                    // 2nd x
                    //  x is at start point of MIU block includes x0, then decrease to previous start point of MIU block

                    if(x<x1)
                        u8Offsetpixels = x1 - x;
                    else
                        u8Offsetpixels = 0;

                    if(x+u8pixels_per_unit > x0)
                        u8Discardpixels = x+u8pixels_per_unit - x0;
                    else
                        u8Discardpixels = 0;
                }

                int index = accessStart;
                MS_U16 u16WriteModeSrcPoint = 0;
                for(accessCount = 0; accessCount < u8pixels_per_unit; index = index + accessNext, accessCount+=1)
                {
                    if((index < u8Offsetpixels) || (index > (u8pixels_per_unit - u8Discardpixels)))
                        continue;

                    if(!bIsReadMode)
                    {
                        if(!bHmirror)
                        {
                            u16WriteModeSrcPoint = (xCount * u8pixels_per_unit + (index - u16LBOffset)) % pSrcRect->width;
                        }
                        else
                        {
                            u16WriteModeSrcPoint = (xCount * u8pixels_per_unit + (u16LBOffset - index)) % pSrcRect->width;
                        }
                    }
#if (_FIELD_PACKING_MODE_SUPPORTED)
                    u32Accesspixel = index;
#else
                    u32Accesspixel = index*u8BytePerTwoPixel/2;
#endif

#if (_FIELD_PACKING_MODE_SUPPORTED)
                    if(false)
#else
                    if(32 == FBInfo.u8BitPerPixel) //10bits
#endif
                    {
                        if(u32Accesspixel>= 2*BYTE_PER_WORD)
                            u32Accesspixel += u32JumpOffset;

                        if(bIsReadMode)
                        {
                            // for get buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // source 444 10bits format, output R10G10B10 mode
                                pPoint32->B_Cb = ( pMEM_temp[u32Accesspixel] ) | ( (pMEM_temp[u32Accesspixel + 1] & 0x03 ) << 8 );
                                pPoint32->G_Y = ( pMEM_temp[u32Accesspixel+ 1] >> 2 ) | ( (pMEM_temp[u32Accesspixel+ 2] & 0x0F ) << 6 );
                                pPoint32->R_Cr = ( pMEM_temp[u32Accesspixel+ 2] >> 4 ) | ( (pMEM_temp[u32Accesspixel+ 3] & 0x3F ) << 4 );
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // source 444 10bits format, output R8G8B8 mode
                                pPoint24->B_Cb = (( pMEM_temp[u32Accesspixel] ) | ( (pMEM_temp[u32Accesspixel + 1] & 0x03 ) << 8 )) >> 2;
                                pPoint24->G_Y = (( pMEM_temp[u32Accesspixel+ 1] >> 2 ) | ( (pMEM_temp[u32Accesspixel+ 2] & 0x0F ) << 6 )) >> 2;
                                pPoint24->R_Cr = (( pMEM_temp[u32Accesspixel+ 2] >> 4 ) | ( (pMEM_temp[u32Accesspixel+ 3] & 0x3F ) << 4 )) >> 2;
                            }
                        }
                        else
                        {
                            // for set buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // target 444 10bits format, input R10G10B10 mode
                                pMEM_temp[u32Accesspixel] = pPoint32[u16WriteModeSrcPoint].B_Cb & 0xFF;
                                pMEM_temp[u32Accesspixel+1] = ((pPoint32[u16WriteModeSrcPoint].G_Y & 0x3F) << 2) |  (pPoint32[u16WriteModeSrcPoint].B_Cb >> 8);
                                pMEM_temp[u32Accesspixel+2] = ((pPoint32[u16WriteModeSrcPoint].R_Cr & 0x0F) << 4) | (pPoint32[u16WriteModeSrcPoint].G_Y >> 6);
                                pMEM_temp[u32Accesspixel+3] = pPoint32[u16WriteModeSrcPoint].R_Cr>> 4;

                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // target 444 10bits format, input R8G8B8 mode
                                pMEM_temp[u32Accesspixel] = (pPoint24[u16WriteModeSrcPoint].B_Cb & 0x3F) << 2;
                                pMEM_temp[u32Accesspixel+1] = ((pPoint24[u16WriteModeSrcPoint].G_Y & 0x0F) << 4) | ((pPoint24[u16WriteModeSrcPoint].B_Cb >>6) & 0x03);
                                pMEM_temp[u32Accesspixel+2] = ((pPoint24[u16WriteModeSrcPoint].R_Cr & 0x03) << 6) | (pPoint24[u16WriteModeSrcPoint].G_Y >> 4);
                                pMEM_temp[u32Accesspixel+3] = pPoint24[u16WriteModeSrcPoint] .R_Cr >>2;
                            }
                        }

                    }
                    else // 8bits
                    {
#if (_FIELD_PACKING_MODE_SUPPORTED)
                        if(bIsReadMode)
                        {
                            // for get buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // source 444 8bits format, output R10G10B10 mode
                                pPoint32->B_Cb = pMEM_temp[u32Accesspixel] << 2;
                                pPoint32->G_Y = pMEM_temp[u32Accesspixel+u16YCSeparateOffset] << 2;
                                pPoint32->R_Cr = pMEM_temp[u32Accesspixel+u16YCSeparateOffset+u16YCSeparateOffset] << 2;
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // source 444 8bits format, output R8G8B8 mode
                                pPoint24->B_Cb = pMEM_temp[u32Accesspixel];
                                pPoint24->G_Y = pMEM_temp[u32Accesspixel+u16YCSeparateOffset];
                                pPoint24->R_Cr = pMEM_temp[u32Accesspixel+u16YCSeparateOffset+u16YCSeparateOffset];
                            }
                        }
                        else
                        {
                            // for set buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // target 444 8bits format, input R10G10B10 mode
                                pMEM_temp[u32Accesspixel] = pPoint32[u16WriteModeSrcPoint].B_Cb>> 2;
                                pMEM_temp[u32Accesspixel+u16YCSeparateOffset]= pPoint32[u16WriteModeSrcPoint].G_Y>> 2;
                                pMEM_temp[u32Accesspixel+u16YCSeparateOffset+u16YCSeparateOffset]= pPoint32[u16WriteModeSrcPoint].R_Cr>> 2;
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // target 444 8bits format, input R8G8B8 mode
                                pMEM_temp[u32Accesspixel] = pPoint24[u16WriteModeSrcPoint].B_Cb;
                                pMEM_temp[u32Accesspixel+u16YCSeparateOffset] = pPoint24[u16WriteModeSrcPoint].G_Y;
                                pMEM_temp[u32Accesspixel+u16YCSeparateOffset+u16YCSeparateOffset] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                            }
                            else
                                printf("-->  not support type \n");
                        }
#else
                        MS_U8 u8JumpNum = u32Accesspixel/ (2*BYTE_PER_WORD);
                        MS_U8 u8BoundMax = (2*BYTE_PER_WORD) + 2* u8JumpNum * (2*BYTE_PER_WORD);

                        u32Accesspixel += u32JumpOffset*u8JumpNum;

                        if(bIsReadMode)
                        {
                            // for get buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // source 444 8bits format, output R10G10B10 mode
                                pPoint32->B_Cb = pMEM_temp[u32Accesspixel] << 2;

                                if(u32Accesspixel+1 >= u8BoundMax)
                                    pPoint32->G_Y = pMEM_temp[u32Accesspixel+1+u32JumpOffset] << 2;
                                else
                                    pPoint32->G_Y = pMEM_temp[u32Accesspixel+1] << 2;

                                if(u32Accesspixel+2 >= u8BoundMax)
                                    pPoint32->R_Cr = pMEM_temp[u32Accesspixel+2+u32JumpOffset] << 2;
                                else
                                    pPoint32->R_Cr = pMEM_temp[u32Accesspixel+2] << 2;
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // source 444 8bits format, output R8G8B8 mode
                                pPoint24->B_Cb = pMEM_temp[u32Accesspixel];

                                if(u32Accesspixel+1 >= u8BoundMax)
                                    pPoint24->G_Y = pMEM_temp[u32Accesspixel+1+u32JumpOffset];
                                else
                                    pPoint24->G_Y = pMEM_temp[u32Accesspixel+1];

                                if(u32Accesspixel+2 >= u8BoundMax)
                                    pPoint24->R_Cr = pMEM_temp[u32Accesspixel+2+u32JumpOffset];
                                else
                                    pPoint24->R_Cr = pMEM_temp[u32Accesspixel+2];
                            }
                        }
                        else
                        {
                            // for set buffer data mode
                            if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                            {
                                // target 444 8bits format, input R10G10B10 mode
                                pMEM_temp[u32Accesspixel] = pPoint32[u16WriteModeSrcPoint].B_Cb >> 2;

                                if(u32Accesspixel+1 >= u8BoundMax)
                                    pMEM_temp[u32Accesspixel+1+u32JumpOffset] = pPoint32[u16WriteModeSrcPoint].G_Y >> 2;
                                else
                                    pMEM_temp[u32Accesspixel+1]= pPoint32[u16WriteModeSrcPoint].G_Y>> 2;

                                if(u32Accesspixel+2 >= u8BoundMax)
                                    pMEM_temp[u32Accesspixel+2+u32JumpOffset] = pPoint32[u16WriteModeSrcPoint].R_Cr>> 2;
                                else
                                    pMEM_temp[u32Accesspixel+2]= pPoint32[u16WriteModeSrcPoint].R_Cr >> 2;
                            }
                            else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                            {
                                // target 444 8bits format, input R8G8B8 mode
                                pMEM_temp[u32Accesspixel] = pPoint24[u16WriteModeSrcPoint].B_Cb;

                                if(u32Accesspixel+1 >= u8BoundMax)
                                    pMEM_temp[u32Accesspixel+1+u32JumpOffset] = pPoint24[u16WriteModeSrcPoint].G_Y;
                                else
                                    pMEM_temp[u32Accesspixel+1] = pPoint24[u16WriteModeSrcPoint].G_Y;

                                if(u32Accesspixel+2 >= u8BoundMax)
                                    pMEM_temp[u32Accesspixel+2+u32JumpOffset] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                                else
                                    pMEM_temp[u32Accesspixel+2] = pPoint24[u16WriteModeSrcPoint].R_Cr;
                            }
                            else
                                printf("-->  not support type \n");
                        }
#endif
                    }

                    if(bIsReadMode)
                    {
                        if(eBufType == E_XC_OUTPUTDATA_RGB10BITS)
                        {
                            FB_DBG(printf("==> index: %d, R: %d, G:%d, B: %d \n", index, pPoint32->R_Cr, pPoint32->G_Y, pPoint32->B_Cb));
                            pPoint32++;
                        }
                        else if(eBufType == E_XC_OUTPUTDATA_RGB8BITS)
                        {
                            FB_DBG(printf("==> index: %d, R: %d, G:%d, B: %d \n", index, pPoint24->R_Cr, pPoint24->G_Y, pPoint24->B_Cb));
                            pPoint24++;
                        }
                    }
                }
            }

            FB_DBG(printf("pMEM_temp=0x%x\n", (int)pMEM_temp));
            FB_DBG(printf("pMEM_temp[%u]=", (u16LBOffset)));
            FB_DBG(printf( "0x%x\n",pMEM_temp[u16LBOffset]));

        }
    }
    FB_DBG(printf("=>Get Memory finish\n"));
    MsOS_ReadMemory(); //Flush CPU operation
}

void MApi_XC_Set_BufferData_U2(void* pInstance, E_XC_INPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pDstRect, void *pSrcBuf, MS_WINDOW_TYPE *pSrcRect, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MApi_XC_Set_BufferData_WithoutMutex(pInstance, eBufType, pDstRect, pSrcBuf, pSrcRect, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_Set_BufferData(E_XC_INPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pDstRect, void *pSrcBuf, MS_WINDOW_TYPE *pSrcRect, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BUFFERDATA XCArgs;
    XCArgs.eBufType = eBufType;
    XCArgs.pDstRect = pDstRect;
    XCArgs.pSrcBuf = pSrcBuf;
    XCArgs.pSrcRect = pSrcRect;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BUFFERDATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Get_BufferData_U2( void* pInstance, E_XC_OUTPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pRect, void *pRectBuf, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MApi_XC_Get_BufferData_WithoutMutex(pInstance, eBufType, pRect, pRectBuf, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_Get_BufferData(E_XC_OUTPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pRect, void *pRectBuf, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_GET_BUFFERDATA XCArgs;
    XCArgs.eBufType = eBufType;
    XCArgs.pRect = pRect;
    XCArgs.pRectBuf = pRectBuf;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_BUFFERDATA, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_BufferData_WithoutMutex(void *pInstance, E_XC_INPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pDstRect, void *pSrcBuf, MS_WINDOW_TYPE *pSrcRect, SCALER_WIN eWindow)
{
    E_XC_OUTPUTDATA_TYPE eDataBufType;

    if (eBufType == E_XC_INPUTDATA_RGB8BITS)
    {
        eDataBufType = E_XC_OUTPUTDATA_RGB8BITS;
    }
    else if (eBufType == E_XC_INPUTDATA_RGB10BITS)
    {
        eDataBufType = E_XC_OUTPUTDATA_RGB10BITS;
    }
    else if (eBufType == E_XC_INPUTDATA_4228BITS)
    {
        eDataBufType = E_XC_OUTPUTDATA_4228BITS;
    }
    else
    {
        printf("Unsupported Buf Type! \n");
        return;
    }


    MDrv_XC_Access_BufferData(pInstance, eDataBufType, pDstRect, pSrcBuf, pSrcRect, eWindow, FALSE);
}


void MApi_XC_Get_BufferData_WithoutMutex(void *pInstance, E_XC_OUTPUTDATA_TYPE eBufType, MS_WINDOW_TYPE *pRect, void *pRectBuf, SCALER_WIN eWindow)
{
    MDrv_XC_Access_BufferData(pInstance, eBufType, pRect, pRectBuf, NULL, eWindow, TRUE);
}

#if 0
//-------------------------------------------------------------------------------------------------
/// Get frame data
/// @param  stSrcInfo           \b IN: @ref XC_ApiStatus
/// @param  eRect_Info          \b IN: @ref MS_RECT_INFO
//-------------------------------------------------------------------------------------------------
void MApi_XC_GetFrameData(XC_ApiStatus *stSrcInfo, MS_RECT_INFO *eRect_Info)
{
    MS_U32 u32RectSize;

    if(eRect_Info->s16X_Start <0 || eRect_Info->s16X_Start >= (stSrcInfo->u16H_SizeAfterPreScaling -1) ||
       eRect_Info->s16X_Size <0 || eRect_Info->s16X_Size >= (stSrcInfo->u16H_SizeAfterPreScaling -1) ||
       eRect_Info->s16X_Start > eRect_Info->s16X_Size || eRect_Info->s16Y_Start > eRect_Info->s16Y_Size||
       eRect_Info->s16Y_Start <0 || eRect_Info->s16Y_Start >= (stSrcInfo->u16V_SizeAfterPreScaling -1) ||
       eRect_Info->s16Y_Size <0 || eRect_Info->s16Y_Size >= (stSrcInfo->u16V_SizeAfterPreScaling -1))
    {
        MS_CRITICAL_MSG(printf("invalid parameter!\n"));
        return;
    }

    u32RectSize = eRect_Info->u32RectPitch * (eRect_Info->s16Y_Size - eRect_Info->s16Y_Start + 1) * eRect_Info->u16PointSize;

    MDrv_SC_GetFrameDataCore(eRect_Info->s16X_Start, eRect_Info->s16Y_Start, eRect_Info->s16X_Size, eRect_Info->s16Y_Size,
                              eRect_Info->pRect,
                              eRect_Info->u32RectPitch,
                              u32RectSize,
                              eRect_Info->u16PointSize);

}
#endif
//void MApi_XC_SetFrameData(XC_ApiStatus stSrcInfo, MS_RECT_INFO eRect_Info);


static void _MDrv_SC_WritePixel(volatile MS_U8* pMEM, MS_BOOL bMemFmt422,
                         void *pPoint, MS_U16 u16PointSize,
                         MS_BOOL bR_Cr)
{
    MS_PIXEL_24BIT *pPoint24 = (MS_PIXEL_24BIT*)pPoint;
    MS_PIXEL_32BIT *pPoint32 = (MS_PIXEL_32BIT*)pPoint;

    if (bMemFmt422)
    {
        if (u16PointSize == 4)
        {
            pMEM[0] = pPoint32->G_Y>>2;
            pMEM[1] = ((bR_Cr) ? pPoint32->R_Cr : pPoint32->B_Cb)>>2;
        }
        else
        {
            pMEM[0] = pPoint24->G_Y;
            pMEM[1] = (bR_Cr) ? pPoint24->R_Cr : pPoint24->B_Cb;
        }
    }
    else
    {
        if (u16PointSize == 4)
        {
            pMEM[0] = (MS_U8)(pPoint32->B_Cb);
            pMEM[1] = (MS_U8)((pPoint32->G_Y << 2) | (pPoint32->B_Cb>> 8));
            pMEM[2] = (MS_U8)((pPoint32->R_Cr<< 4) | (pPoint32->G_Y >> 6));
            pMEM[3] = (MS_U8)(pPoint32->R_Cr >> 4);
        }
        else
        {
            pMEM[0] = (pPoint24->B_Cb<< 2);
            pMEM[1] = (pPoint24->G_Y << 4) | (pPoint24->B_Cb>> 6);
            pMEM[2] = (pPoint24->R_Cr<< 6) | (pPoint24->G_Y >> 4);
            pMEM[3] = (pPoint24->R_Cr>> 2);
        }
    }
}


void MDrv_SC_SetFrameDataCore(void *pInstance, MS_S16 x0, MS_S16 y0, MS_S16 x1, MS_S16 y1,
                              MS_U8 *pRect,
                              MS_U32 u32RectPitch,
                              MS_U32 u32RectSize,
                              MS_U16 u16PointSize)
{
    MS_U8 *pPoint;
    volatile MS_PHY pMEM;
    volatile MS_U8 *pMEM_temp;
    MS_U8 u8BytePerPixel;
    MS_U8 u8LSB_YM4_Size;
    MS_S16 x, y;
    MS_U16 u16OffsetPixelAlign;
    MS_U32 u32Y_Offset, u32X_Offset;
    SC_FRAMEBUF_INFO_t FBInfo;

    Hal_SC_get_framebuf_Info(pInstance, &FBInfo, MAIN_WINDOW);

    // not implement pre-scaling down
    if (FBInfo.bMemFormat422 == TRUE)
    {
        if (FBInfo.u8BitPerPixel == 24)
        {
            FB_DBG(printf("422_10BIT\n"));
            u8BytePerPixel = 3;
            u8LSB_YM4_Size = BYTE_PER_WORD*2;
            u16OffsetPixelAlign = BYTE_PER_WORD/2*2; // *2 (F0 F0)
        }
        else //if (FBInfo.u8BitPerPixel == 16)
        {
            FB_DBG(printf("422_8BIT\n"));
            u8BytePerPixel = 2;
            u8LSB_YM4_Size = 0;
            u16OffsetPixelAlign = BYTE_PER_WORD/2*2; // *2 (F0 F0)
        }
    }
    else
    {   // using 444 10 bit
        FB_DBG(printf("444_10BIT\n"));
        u8BytePerPixel = 4;
        u8LSB_YM4_Size = 0;
        u16OffsetPixelAlign = BYTE_PER_WORD/4*2; // *2 (F0 F0)
    }

    for (y = y0; y <= y1; y++)
    {
        if (FBInfo.bInterlace)
        {
            u32Y_Offset = (MS_U32)y/2 * FBInfo.u16IPMOffset * u8BytePerPixel * FBInfo.u8FBNum;
            if (y & 0x1)
                u32Y_Offset += FBInfo.u32IPMBase1;
            else
                u32Y_Offset += FBInfo.u32IPMBase0;
        }
        else
        {
            u32Y_Offset = (MS_U32)y * FBInfo.u16IPMOffset * u8BytePerPixel * FBInfo.u8FBNum;
            u32Y_Offset += FBInfo.u32IPMBase0;
        }
        FB_DBG(printf("[1]u32Y_Offset:0x%x, u16IPMOffset:%u, y:%u, u8BytePerPixel:%u, u8FBNum:%u\n",
            (int)u32Y_Offset, FBInfo.u16IPMOffset, y, u8BytePerPixel, FBInfo.u8FBNum));

        for (x = x0; x <= x1; x++)
        {
            u32X_Offset = (MS_U32)x / u16OffsetPixelAlign * u16OffsetPixelAlign * u8BytePerPixel * FBInfo.u8FBNum;

            FB_DBG(printf("[2]u32X_Offset=0x%x, u16OffsetPixelAlign=%u, x=%u, align(x)=%u\n",
                (int)u32X_Offset, u16OffsetPixelAlign, x, x / u16OffsetPixelAlign * u16OffsetPixelAlign));

            pPoint = pRect+ (u32RectPitch * (y - y0) + (x - x0)) * u16PointSize;
            if ((x / u16OffsetPixelAlign) & 0x1)
            {
                pMEM = (MS_PHY)(0UL + u32Y_Offset + u32X_Offset + u8LSB_YM4_Size);
            }
            else
            {
                pMEM = (MS_PHY)(0UL + u32Y_Offset + u32X_Offset);
            }
            pMEM_temp = (MS_U8 *)MS_PA2KSEG1(pMEM);
            FB_DBG(printf("[3]pMEM=0x%x, x:%u, y:%u, odd:%u\n",
                (int)pMEM, x, y, ((x / u16OffsetPixelAlign) & 0x1)));

            if (FBInfo.bMemFormat422 == TRUE)
            {
                _MDrv_SC_WritePixel(&pMEM_temp[(x%u16OffsetPixelAlign)*2],
                                    FBInfo.bMemFormat422,
                                    pPoint,
                                    u16PointSize,
                                    (x&0x1));
            }
            else
            {
                _MDrv_SC_WritePixel(&pMEM_temp[(x%u16OffsetPixelAlign)*4],
                                    FBInfo.bMemFormat422,
                                    pPoint,
                                    u16PointSize,
                                    (x&0x1));
            }
        }
    }
}

#if 1
#define H_CAP_ADD_FOR_GARBAGE 2
#define V_CAP_ADD_FOR_GARBAGE 2

typedef struct
{
    MS_U8 u8NRenable;
    MS_U8 u8Memory_FMT;
    MS_U8 u8IPM_Memory_FMT;
    MS_U8 u8OPM_Memory_FMT;

    SC_FRAMEBUF_INFO_t pFBInfo;

    MS_U8 u8IPMux;
    MS_U8 u8IPClk;
    MS_U8 u8OPMux;

    MS_U8 u8CapSource;
    MS_U8 u8CapMethod;
    MS_U8 u8User_Interlace;
    MS_WINDOW_TYPE capture_win;
    MS_BOOL bIsRGB_Format;
    MS_BOOL bIsHPreDown;
    MS_BOOL bIsVPreDown;
    MS_BOOL bIs444to422_filter;
    MS_U8 u8Bit_Format;
    MS_U32 u32IRQ_Mask;
}XC_IPM_Setting_Status;
//-------------------------------------------------------------------------------------------------
/// Store the current Setting before reroute the path from VOP to IP
/// @param  pXC_PreSetting      \b IN: The current setting before reroute
/// @return @ref XC_IPM_Setting_Status
//-------------------------------------------------------------------------------------------------
void MApi_XC_Get_Current_Setting(void *pInstance, XC_IPM_Setting_Status *pXC_PreSetting)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    //Step3, Store original setting
    pXC_PreSetting->u8NRenable= HAL_SC_get_dnr_setting(pInstance, MAIN_WINDOW);
    //Step4, Get input/output display data format
    pXC_PreSetting->u8Memory_FMT = HAL_SC_get_memory_fmt(pInstance);
    pXC_PreSetting->u8OPM_Memory_FMT = HAL_SC_get_OPM_memory_fmt(pInstance);
    pXC_PreSetting->u8IPM_Memory_FMT = HAL_SC_get_IPM_memory_fmt(pInstance);
    //Step5, Store Memory Address for Capture
    Hal_SC_get_framebuf_Info(pInstance, &(pXC_PreSetting->pFBInfo), MAIN_WINDOW );
    //Step6, Store INT from Main Capture port
    pXC_PreSetting->u32IRQ_Mask = HAL_SC_get_IRQ_mask_0_31(pInstance);
    //Step7/8, Store IP MUX/Store IP Clock
    pXC_PreSetting->u8IPMux = HAL_SC_ip_get_ipmux(pInstance);
    pXC_PreSetting->u8IPClk = HAL_SC_ip_get_ipclk(pInstance);
    //Step9, Store OP MUX
    pXC_PreSetting->u8OPMux = HAL_SC_get_VOP_mux(pInstance);
    //Step10, Store Capture Source
    pXC_PreSetting->u8CapSource = HAL_SC_ip_get_capturesource(pInstance);
    //Step11,
    pXC_PreSetting->u8CapMethod = HAL_SC_ip_get_capture_method(pInstance);
    //Step12,
    pXC_PreSetting->u8User_Interlace= Hal_SC_ip_get_user_def_interlace_status(pInstance);
    //Step13, Store capture Infor
    Hal_SC_ip_get_capture_window(pInstance, &(pXC_PreSetting->capture_win), MAIN_WINDOW);
    //Setp14, Store capture format
    pXC_PreSetting->bIsRGB_Format = HAL_SC_ip_get_capture_format(pInstance);
    //Setp15, Store Pre_memory scaling down status
        //H Scaling Down enable
    pXC_PreSetting->bIsHPreDown = HAL_SC_ip_get_h_predown_info(pInstance);
        //V Scaling Down enable
    pXC_PreSetting->bIsHPreDown = HAL_SC_ip_get_v_predown_info(pInstance);
    //Step 16, Store the 444to422 setting
    pXC_PreSetting->bIs444to422_filter = HAL_SC_ip_get_444to422_filter_mod(pInstance);
    pXC_PreSetting->u8Bit_Format = HAL_SC_get_memory_bit_fmt(pInstance, MAIN_WINDOW);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}
//-------------------------------------------------------------------------------------------------
/// Setting for reroute the path from VOP to IP
/// @param  eXC_PreSetting      \b IN: The new setting for reroute
/// @param  bRestore               \b IN: Enable Restore mode or not
//-------------------------------------------------------------------------------------------------
void MApi_XC_Set_New_Setting(void *pInstance, XC_IPM_Setting_Status eXC_PreSetting, MS_BOOL bRestore)
{
    //Step 3, Store original setting
    HAL_SC_enable_dnr(pInstance, eXC_PreSetting.u8NRenable, MAIN_WINDOW);
    //Step4, Set OP memory format
    HAL_SC_set_OPM_memory_fmt(pInstance, eXC_PreSetting.u8OPM_Memory_FMT);

    //Step 5, Set Memory Address for Capture
    //Set IPM Offset and Fetch
    Hal_SC_set_framebuf_Info(pInstance, eXC_PreSetting.pFBInfo);

    //Step 6, Set IRQ for Main Capture port
    HAL_SC_set_IRQ_mask_0_31(pInstance, eXC_PreSetting.u32IRQ_Mask);
    //Step 7/8, Set IP MUX and IP Clock
    HAL_SC_ip_set_ipmux(pInstance, eXC_PreSetting.u8IPMux);
    HAL_SC_ip_set_ipclk(pInstance, eXC_PreSetting.u8IPClk);
    //Step 9~12
    if(bRestore)
    {
        HAL_SC_set_capture_image2ip(pInstance, DISABLE);
        HAL_SC_set_VOP_mux(pInstance, eXC_PreSetting.u8OPMux);

        HAL_SC_ip_set_capturesource(pInstance, eXC_PreSetting.u8CapSource);
        //Set Capture Method
        Hal_SC_ip_set_image_wrap(pInstance, (MS_BOOL)eXC_PreSetting.u8CapMethod&BIT(0),
                                                       (MS_BOOL)eXC_PreSetting.u8CapMethod&BIT(1), MAIN_WINDOW);
        Hal_SC_ip_set_input_sync_reference_edge(pInstance, (MS_BOOL)eXC_PreSetting.u8CapMethod&BIT(3),
                                                       (MS_BOOL)eXC_PreSetting.u8CapMethod&BIT(4), MAIN_WINDOW);
        Hal_SC_ip_set_input_vsync_delay(pInstance, (MS_BOOL)eXC_PreSetting.u8CapMethod&BIT(5), MAIN_WINDOW);
        Hal_SC_ip_set_de_only_mode(pInstance, (MS_BOOL)eXC_PreSetting.u8CapMethod&BIT(6), MAIN_WINDOW);
        //Disable User def Interlace mode
        Hal_SC_ip_set_user_def_interlace_status(pInstance, (MS_BOOL)eXC_PreSetting.u8User_Interlace&BIT(0),
                                                                              (MS_BOOL)eXC_PreSetting.u8User_Interlace&BIT(1),MAIN_WINDOW);
        HAL_SC_ip_set_444to422_filter_mod(pInstance, eXC_PreSetting.bIs444to422_filter);
    }
    else
    {
        HAL_SC_set_capture_image2ip(pInstance, ENABLE);
        /*BK0F_46_L[7:4]
        0 : VOP2_dp input data
        1 : BRI output
        2 : HBC output
        3 : CON_BRI output
        4 : FWC output
        5 : gamma output
        6 : noise dither output
        */
        HAL_SC_set_VOP_mux(pInstance, 0x50);
        //Set Input Source
        /* #000: Analog 1.
            #001: Analog 2.
            #010: Analog 3.
            #011: DVI.
            #100: Video.
            #101: HDTV
            #111: HDMI. */
        HAL_SC_ip_set_capturesource(pInstance, 0x03);
        //Set Capture Method
        Hal_SC_ip_set_image_wrap(pInstance, DISABLE, DISABLE, MAIN_WINDOW);
        Hal_SC_ip_set_input_sync_reference_edge(pInstance, DISABLE, DISABLE, MAIN_WINDOW);
        Hal_SC_ip_set_input_vsync_delay(pInstance, DISABLE, MAIN_WINDOW);
        Hal_SC_ip_set_de_only_mode(pInstance, DISABLE, MAIN_WINDOW);
        //Disable User def Interlace mode
        Hal_SC_ip_set_user_def_interlace_status(pInstance, DISABLE, DISABLE,MAIN_WINDOW);

    }

    //Step 13, Set New Capture Window
    Hal_SC_ip_set_capture_v_start(pInstance, eXC_PreSetting.capture_win.y , MAIN_WINDOW);
    Hal_SC_ip_set_capture_h_start(pInstance, eXC_PreSetting.capture_win.x , MAIN_WINDOW);
    Hal_SC_ip_set_capture_v_size(pInstance, eXC_PreSetting.capture_win.height, MAIN_WINDOW);
    Hal_SC_ip_set_capture_h_size(pInstance, eXC_PreSetting.capture_win.width, MAIN_WINDOW);
    //Step 14, Set capture format, Default Display out is RGB
    HAL_SC_ip_set_capture_format(pInstance, eXC_PreSetting.bIsRGB_Format);
    //Step 15, setting H/V pre down mode
    HAL_SC_ip_set_h_predown_info(pInstance, eXC_PreSetting.bIsHPreDown);
    HAL_SC_ip_set_v_predown_info(pInstance, eXC_PreSetting.bIsVPreDown);
    //Step 16, Set Capture memory store format
    HAL_SC_set_IPM_memory_fmt(pInstance, eXC_PreSetting.u8IPM_Memory_FMT);

}
void MApi_XC_Rerout_for_Capture_VOP_Data(void *pInstance)
{
    XC_IPM_Setting_Status stXC_PreSetting;
    XC_IPM_Setting_Status stXC_NewSetting;
    MS_U16 u16NewIPMFetch, u16NewIPMOffset;
    SC_FRAMEBUF_INFO_t FBInfo;

    memset(&stXC_PreSetting,0,sizeof(XC_IPM_Setting_Status));
    memset(&stXC_NewSetting,0,sizeof(XC_IPM_Setting_Status));
    //Step 1, Freeze image
    Hal_SC_set_freezeimg(pInstance, ENABLE, MAIN_WINDOW);
    //Step 2, Wait 1/20 second
    MsOS_DelayTaskUs(50); //wait at 1/20 second

    //Store Current setting
    MApi_XC_Get_Current_Setting(pInstance, &stXC_PreSetting);
    //Init the value using pre setting value
    memcpy(&stXC_NewSetting, &stXC_PreSetting, sizeof(XC_IPM_Setting_Status));
    //Set the new setting
    stXC_NewSetting.u8NRenable=0x00;
    switch(stXC_PreSetting.u8Memory_FMT)
    {
        case 0:
            stXC_NewSetting.u8OPM_Memory_FMT=0x00;
            break;
        case 1:
            stXC_NewSetting.u8OPM_Memory_FMT=0x05;
            break;
        case 4:
            stXC_NewSetting.u8OPM_Memory_FMT=0x06;
            break;
        case 5:
            stXC_NewSetting.u8OPM_Memory_FMT=0x0B;
            break;
        case 6:
            stXC_NewSetting.u8OPM_Memory_FMT=0x05;
            break;
        case 7:
        default:
            stXC_NewSetting.u8OPM_Memory_FMT=0x00;
            break;
    }


    u16NewIPMFetch = (stXC_PreSetting.pFBInfo.u16IPMFetch + H_CAP_ADD_FOR_GARBAGE)&~1;

    if(u16NewIPMFetch > MST_LINE_BFF_MAX)
        u16NewIPMFetch = MST_LINE_BFF_MAX;
    u16NewIPMOffset = u16NewIPMFetch;
    if(u16NewIPMOffset & 0x0F)
         u16NewIPMOffset = (u16NewIPMOffset & 0xFFF0) + 16;

    u16NewIPMFetch = (u16NewIPMFetch + 1) & ~1;
    stXC_NewSetting.pFBInfo.u16IPMFetch = u16NewIPMFetch;
    stXC_NewSetting.pFBInfo.u16IPMOffset = u16NewIPMOffset;
    // TODO: Implement V Length
    //stXC_NewSetting.pFBInfo->u16VLength =;
    stXC_NewSetting.pFBInfo.bLinearAddrMode = TRUE;
    stXC_NewSetting.u32IRQ_Mask = stXC_NewSetting.u32IRQ_Mask|0x033C03C0;
    stXC_NewSetting.u8IPMux = 0x70;
    stXC_NewSetting.u8IPClk = 0x1C;
    //Step 14, Set capture format YCbCr/RGB by Display out format
    //Default is RGB
    stXC_NewSetting.bIsRGB_Format=FALSE;

    stXC_NewSetting.bIsHPreDown=DISABLE;
    stXC_NewSetting.bIsVPreDown=DISABLE;
    Hal_SC_get_framebuf_Info(pInstance, &FBInfo, MAIN_WINDOW);
    if (FBInfo.bMemFormat422 == TRUE)
    {
        if (FBInfo.u8BitPerPixel == 20)//24
        {
            FB_DBG(printf("YC 422_10BIT\n"));
            stXC_NewSetting.u8IPM_Memory_FMT=0x05;
            stXC_NewSetting.u8Bit_Format=0x00;
        }
        else //if (FBInfo.u8BitPerPixel == 16)
        {
            FB_DBG(printf("YC 422_8BIT\n"));
            stXC_NewSetting.u8IPM_Memory_FMT=0x00;
            stXC_NewSetting.u8Bit_Format=0x00;
        }
    }
    else
    {
        if (FBInfo.u8BitPerPixel == 24)
        {
            FB_DBG(printf("RGB/YCbCr 444_8BIT\n"));
            stXC_NewSetting.u8IPM_Memory_FMT=0x01;
            stXC_NewSetting.u8Bit_Format=0x01;
        }
    }
    MApi_XC_Set_New_Setting(pInstance, stXC_NewSetting, DISABLE);
    //Step 17, Wait 1/20 second
    MsOS_DelayTaskUs(50); //wait at 1/20 second
    //Step 18, Set Capture Enable/Start
    HAL_SC_set_IPM_capture_start(pInstance, ENABLE);
    //Step 19, Wait 1/20 second
    MsOS_DelayTaskUs(50); //wait at 1/20 second
    //Step 20, Set Capture Disable/Stop
    HAL_SC_set_IPM_capture_start(pInstance, DISABLE);
    //Step 21, Wait 1/20 second
    MsOS_DelayTaskUs(50); //wait at 1/20 second

    //Restore the previous setting
    MApi_XC_Set_New_Setting(pInstance, stXC_PreSetting, ENABLE);
    //Step 24, Wait 1/20 second
    MsOS_DelayTaskUs(50); //wait at 1/20 second

    //Step 26, Freeze image
    Hal_SC_set_freezeimg(pInstance, DISABLE, MAIN_WINDOW);

}

#endif

//-------------------------------------------------------------------------------------------------
/// Get the average data from memory
/// @param  mem_win      \b IN: The window position and size
/// @return @ref MS_AUTOADC_TYPE
//-------------------------------------------------------------------------------------------------
MS_AUTOADC_TYPE MApi_XC_GetAverageDataFromMemory(void *pInstance, MS_WINDOW_TYPE* mem_win)
{

    MS_IMAGE_MEM_INFO  pMemInfo;
    _XC_ENTRY(pInstance);
    pMemInfo.eCal_Win_Size.u16X = mem_win->x ;
    pMemInfo.eCal_Win_Size.u16Y = mem_win->y ;
    pMemInfo.eCal_Win_Size.u16Xsize = mem_win->width;
    pMemInfo.eCal_Win_Size.u16Ysize = mem_win->height ;
    MApi_XC_ReadRgbData(pInstance, &pMemInfo);
    _XC_RETURN(pInstance);
    return pMemInfo.pixelAverage;

#if 0

    #define GET_MEMAVG_WITHOUT_MAXMIN 0
    #define ADC_READ_PATTERN_FROM_EXTERNAL_SIGNAL 0

    #define RGB_CAL_MEM_BLOCKSIZE   (16)
    #define SCALER_SLOT_SIZE        (16)    // 128 bits

    #define RGB_CAL_BYTES_PER_PIXEL (4)
    #define RGB_CAL_PIXELS_PER_SLOT (SCALER_SLOT_SIZE/RGB_CAL_BYTES_PER_PIXEL)

    MS_AUTOADC_TYPE ptAdcData;
    MS_U8 *pu8VirMemBaseAdr, *pu8VirPixelAdr;
    MS_U32 u32PixelAddrOffset;
    MS_U32 u32Rtt, u32Gtt, u32Btt;
    MS_U32 i, x, y, tmp;
    MS_U16 u16LineOffset;
    MS_U8 u8Bank;
    MS_U16 u16MemVStart, u16MemHStart, u16CaptureHStart, u16CaptureVStart;
    MS_U16 u16MemVEnd, u16MemHEnd, u16CaptureHSize, u16CaptureVSize;

#if GET_MEMAVG_WITHOUT_MAXMIN
    MS_U16 u16MaxR,u16MaxG,u16MaxB;
    MS_U16 u16MinR,u16MinG,u16MinB;
    MS_U16 u16NumMaxR,u16NumMaxG,u16NumMaxB;
    MS_U16 u16NumMinR,u16NumMinG,u16NumMinB;
    MS_U16 u16NoMinMaxAvgCb, u16NoMinMaxAvgY, u16NoMinMaxAvgCr;
#endif

    SC_MEM_YUV444_FMT_t sMemBuf[mem_win->height * mem_win->width];


    ptAdcData.u16CH_AVG[0] = ptAdcData.u16CH_AVG[1] = ptAdcData.u16CH_AVG[2] = 0;


    //SC_BK_STORE;
    u8Bank = MDrv_ReadByte(BK_SELECT_00);

    // get line offset
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_SCMI);
    u16LineOffset = MDrv_Read2Byte(L_BK_SCMI(0x0E));
    msg_sc_ip(printf("F2 IP frame buffer line offset (pixel unit) %d\n", u16LineOffset));


    pu8VirMemBaseAdr = (MS_U8*)g_XC_InitData.u32Main_FB_Start_Addr;
    msg_sc_ip(printf("DNR Buf Adr 0x%lx\n", (MS_U32)pu8VirMemBaseAdr));


    i = 0;
    u32Rtt = 0;
    u32Gtt = 0;
    u32Btt = 0;

#if GET_MEMAVG_WITHOUT_MAXMIN
    u16MaxR = 0;
    u16MaxG = 0;
    u16MaxB = 0;
    u16MinR = 0;
    u16MinG = 0;
    u16MinB = 0;

    u16NumMaxR = 0;
    u16NumMaxG = 0;
    u16NumMaxB = 0;
    u16NumMinR = 0;
    u16NumMinG = 0;
    u16NumMinB = 0;

    u16NoMinMaxAvgCb = 0;
    u16NoMinMaxAvgY = 0;
    u16NoMinMaxAvgCr = 0;
#endif

    MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    u16CaptureVStart = MDrv_Read2Byte(L_BK_IP1F2(0x04));
    u16CaptureHStart = MDrv_Read2Byte(L_BK_IP1F2(0x05));
    u16CaptureVSize = MDrv_Read2Byte(L_BK_IP1F2(0x06));
    u16CaptureHSize = MDrv_Read2Byte(L_BK_IP1F2(0x07));
    u16MemVStart = 0;
    u16MemHStart = 0;
    u16MemVEnd = u16CaptureVSize;
    u16MemHEnd = u16CaptureHSize;


    msg_sc_ip(printf("Capture H Start %d V Start %d\n", u16CaptureHStart, u16CaptureVStart));
    msg_sc_ip(printf("Capture H End   %d V Eend  %d\n",
        (u16CaptureHStart + u16CaptureHSize),
        (u16CaptureVStart + u16CaptureVSize)));

    u16MemVStart = mem_win->y;
    u16MemHStart = mem_win->x;

    /*
    switch (u8Position)
    {
        case 0: //Frame
            u16MemVStart = 0;
            u16MemHStart = 0;
            break;
        case 1: //Up-Left Coner
            u16MemVStart = 0;
            u16MemHStart = 0;
            break;
        case 2: //Up-Right Coner
            u16MemVStart = 0;
            u16MemHStart = u16MemHEnd - mem_win->width;
            break;
        case 3: //Bottom-Left Coner
            u16MemVStart = u16MemVEnd - mem_win->height;
            u16MemHStart = 0;
            break;
        case 4: //Bottom-Right Coner
            u16MemVStart = u16MemVEnd - mem_win->height;
            u16MemHStart = u16MemHEnd - mem_win->width;
            break;
        case 5: //Center
            u16MemVStart = ((u16MemVEnd - u16MemVStart) / 2) - (mem_win->height / 2);
            u16MemHStart = ((u16MemHEnd - u16MemHStart) / 2) - (mem_win->width / 2);
            break;
#if ADC_READ_PATTERN_FROM_EXTERNAL_SIGNAL
        case 6: //White - Panel 1366x768, MSPG-925FS Model:217(1280x720p), Pattern: 15 (8 color bar)
            u16MemVStart = 0x170;
            u16MemHStart = 0x20;
            break;
        case 7: //Black - Panel 1366x768, MSPG-925FS Model:217(1280x720p), Pattern: 15 (8 color bar)
            u16MemVStart = 0x170;
            u16MemHStart = 0x520;
            break;
#endif
        default:
            break;
    }
    */

#if ADC_READ_PATTERN_FROM_EXTERNAL_SIGNAL
    msg_sc_ip(printf("Block Positon (%d, %d) size %dx%d\n",
        u16MemHStart,
        u16MemVStart,
        u16HBlockSize,
        mem_win.height));
    // Enable Debug Cross
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_DLC);
    MDrv_WriteRegBit(L_BK_DLC(0x68), ENABLE, BIT(0));
    for(y=u16MemVStart; y<u16MemVStart+mem_win->height; y++)
    {
        for(x=u16MemHStart; x<u16MemHStart+mem_win->width; x++)
        {
            MS_U16 u16Y, u16Cb, u16Cr;

            MDrv_WriteByte(BK_SELECT_00, REG_BANK_DLC);

            MDrv_Write2Byte(L_BK_DLC(0x69), x);
            MDrv_Write2Byte(L_BK_DLC(0x6A), y);

            MsOS_DelayTask(10);

            u16Y = (MDrv_Read2Byte(L_BK_DLC(0x6B)) & 0x03FF);
            u16Cb = (MDrv_Read2Byte(L_BK_DLC(0x6C)) & 0x03FF);
            u16Cr = (MDrv_Read2Byte(L_BK_DLC(0x6D)) & 0x03FF);

            //printf("Y = 0x%04x, Cb = 0x%04x, Cr = 0x%04x\n", u16Y, u16Cb, u16Cr);
            u32Rtt += u16Cr;
            u32Gtt += u16Y;
            u32Btt += u16Cb;
        }
    }
    // Disable Debug Cross
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_DLC);
    MDrv_WriteRegBit(L_BK_DLC(0x68), DISABLE, BIT(0));

    ptAdcData.u16CH_AVG[0] = u32Rtt / (mem_win.height * mem_win->width);
    ptAdcData.u16CH_AVG[1] = u32Gtt / (mem_win.height * mem_win->width);
    ptAdcData.u16CH_AVG[2] = u32Btt / (mem_win.height * mem_win->width);

    //SC_BK_RESTORE;
    MDrv_WriteByte(BK_SELECT_00, u8Bank);

    msg_sc_ip(printf("AvgCr AvgY AvgCb\n"));
    msg_sc_ip(printf("%04d  %04d %04d\n\n", ptAdcData.R_CH_AVG, ptAdcData.G_CH_AVG, ptAdcData.B_CH_AVG));


#else


    msg_sc_ip(printf("Block Positon (%d, %d) Mem Start (%d, %d) size %dx%d\n",
        (u16MemHStart + u16CaptureHStart),
        (u16MemVStart + u16CaptureVStart),
        u16MemHStart,
        u16MemVStart,
        mem_win->width,
        mem_win->height));


    for(y=u16MemVStart; y<u16MemVStart+mem_win->height; y++)
    {
        for(x=u16MemHStart; x<u16MemHStart+mem_win->width; x++)
        {
            u32PixelAddrOffset = y * u16LineOffset * RGB_CAL_BYTES_PER_PIXEL * 2;
            u32PixelAddrOffset += ((MS_U32)(x / RGB_CAL_PIXELS_PER_SLOT)) * SCALER_SLOT_SIZE * 2;
            pu8VirPixelAdr = pu8VirMemBaseAdr + u32PixelAddrOffset +
                                    (x % RGB_CAL_PIXELS_PER_SLOT) * RGB_CAL_BYTES_PER_PIXEL;

            memcpy(&(sMemBuf[i]), pu8VirPixelAdr, RGB_CAL_BYTES_PER_PIXEL);
            memcpy(&tmp, pu8VirPixelAdr, RGB_CAL_BYTES_PER_PIXEL);


#if GET_MEMAVG_WITHOUT_MAXMIN
            if (i == 0)
            {
                //printf("pu8VirPixelAdr 0x%lx\n", (U32)pu8VirPixelAdr);
                u16MaxR = sMemBuf[i].Cr;
                u16MinR = sMemBuf[i].Cr;
                u16MaxG = sMemBuf[i].Y;
                u16MinG = sMemBuf[i].Y;
                u16MaxB = sMemBuf[i].Cb;
                u16MinB = sMemBuf[i].Cb;
                u16NumMaxR = 1;
                u16NumMaxG = 1;
                u16NumMaxB = 1;
                u16NumMinR = 1;
                u16NumMinG = 1;
                u16NumMinB = 1;
                //while (1);
            }
            else
            {
                if (sMemBuf[i].Cr > u16MaxR)
                {
                    u16MaxR = sMemBuf[i].Cr;
                    u16NumMaxR = 1;
                }
                else if (sMemBuf[i].Cr == u16MaxR)
                {
                    u16NumMaxR++;
                }

                if (sMemBuf[i].Cr < u16MinR)
                {
                    u16MinR = sMemBuf[i].Cr;
                    u16NumMinR = 1;
                }
                else if (sMemBuf[i].Cr == u16MinR)
                {
                    u16NumMinR++;
                }


                if (sMemBuf[i].Y > u16MaxG)
                {
                    u16MaxG = sMemBuf[i].Y;
                    u16NumMaxG = 1;
                }
                else if (sMemBuf[i].Y == u16MaxG)
                {
                    u16NumMaxG++;
                }

                if (sMemBuf[i].Y < u16MinG)
                {
                    u16MinG = sMemBuf[i].Y;
                    u16NumMinG = 1;
                }
                else if (sMemBuf[i].Y == u16MinG)
                {
                    u16NumMinG++;
                }

                if (sMemBuf[i].Cb > u16MaxB)
                {
                    u16MaxB = sMemBuf[i].Cb;
                    u16NumMaxB = 1;
                }
                else if (sMemBuf[i].Cb == u16MaxB)
                {
                    u16NumMaxB++;
                }

                if (sMemBuf[i].Cb < u16MinB)
                {
                    u16MinB = sMemBuf[i].Cb;
                    u16NumMinB = 1;
                }
                else if (sMemBuf[i].Cb == u16MinB)
                {
                    u16NumMinB++;
                }
            }
#endif
            u32Rtt += sMemBuf[i].Cr;
            u32Gtt += sMemBuf[i].Y;
            u32Btt += sMemBuf[i].Cb;
            i++;
        }
    }



#if 0
    printf("\n");

    printf("CrBlock %dx%d", u16HBlockSize, u16VBlockSize);
    for (i = 0; i < (u16VBlockSize * u16HBlockSize); i++)
    {
        if ((i % u16HBlockSize) == 0)
        {
            printf("\n");
        }
        printf("%04d ", sMemBuf[i].Cr);
    }
    printf("\n");

    printf("YBlock %dx%d", u16HBlockSize, u16VBlockSize);
    for (i = 0; i < (u16VBlockSize * u16HBlockSize); i++)
    {
        if ((i % u16HBlockSize) == 0)
        {
            printf("\n");
        }
        printf("%04d ", sMemBuf[i].Y);
    }
    printf("\n");

    printf("CbBlock %dx%d", u16HBlockSize, u16VBlockSize);
    for (i = 0; i < (u16VBlockSize * u16HBlockSize); i++)
    {
        if ((i % u16HBlockSize) == 0)
        {
            printf("\n");
        }
        printf("%04d ", sMemBuf[i].Cb);
    }
    printf("\n");
#endif

#if GET_MEMAVG_WITHOUT_MAXMIN
    tmp = u32Rtt - ((MS_U32)u16MaxR * (MS_U32)u16NumMaxR) - ((MS_U32)u16MinR * (MS_U32)u16NumMinR);
    if ( tmp != 0 )
    {
        u16NoMinMaxAvgCr = tmp / ((mem_win->height * mem_win->width) - (u16NumMaxR + u16NumMinR));
    }

    tmp = u32Gtt - ((MS_U32)u16MaxG * (MS_U32)u16NumMaxG) - ((MS_U32)u16MinG * (MS_U32)u16NumMinG);
    if ( tmp != 0 )
    {
        u16NoMinMaxAvgY = tmp / ((mem_win->height * mem_win->width) - (u16NumMaxG + u16NumMinG));
    }

    tmp = u32Btt - ((MS_U32)u16MaxB * (MS_U32)u16NumMaxB) - ((MS_U32)u16MinB * (MS_U32)u16NumMinB);
    if ( tmp != 0 )
    {
        u16NoMinMaxAvgCb = tmp / ((mem_win->height * mem_win->width) - (u16NumMaxB + u16NumMinB));
    }
#endif

    ptAdcData.u16CH_AVG[0] = u32Rtt / (mem_win->height * mem_win->width);
    ptAdcData.u16CH_AVG[1] = u32Gtt / (mem_win->height * mem_win->width);
    ptAdcData.u16CH_AVG[2] = u32Btt / (mem_win->height * mem_win->width);


    //SC_BK_RESTORE;
    MDrv_WriteByte(BK_SELECT_00, u8Bank);

    msg_sc_ip( printf("ptAdcData[%04d,%04d,%04d]\n",ptAdcData.R_CH_AVG,ptAdcData.G_CH_AVG,ptAdcData.B_CH_AVG) );

#if GET_MEMAVG_WITHOUT_MAXMIN
    printf("MaxCr    MinCr    MaxY    MinY    MaxCb    MinCb    AvgCr AvgY AvgCb\n");
    printf("%04d     %04d     %04d    %04d    %04d     %04d     %04d  %04d %04d\n",
        u16MaxR, u16MinR, u16MaxG, u16MinG, u16MaxB, u16MinB, ptAdcData.R_CH_AVG, ptAdcData.G_CH_AVG, ptAdcData.B_CH_AVG);

    printf("NumMaxCr NumMinCr NumMaxY NumMinY NumMaxCb NumMinCb AvgCr AvgY AvgCr\n");
    printf("%04d     %04d     %04d    %04d    %04d     %04d     %04d  %04d %04d\n\n",
        u16NumMaxR, u16NumMinR, u16NumMaxG, u16NumMinG, u16NumMaxB, u16NumMinB, u16NoMinMaxAvgCr, u16NoMinMaxAvgY, u16NoMinMaxAvgCb);
#endif

#endif

    return ptAdcData;
#endif

}



MS_U8 MDrv_XC_wait_input_vsync(void *pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    MS_U32 u32Time;
    MS_U8 u8VsyncPolarity = Hal_SC_ip_get_input_vsync_polarity(pInstance);
    MS_U8 bVSync = !u8VsyncPolarity;
    MS_U8 u8VSyncVal;
    u32Time = MsOS_GetSystemTime();

    while (1)
    {
        u8VSyncVal = Hal_SC_ip_get_input_vsync_value(pInstance, eWindow);

    //for Special case,(like U4) if no subwindow, don't wait vsync. break immediately
        if(u8VSyncVal == 0xFF)
            break;

        if (u8VSyncVal == bVSync)
        {
            if ((bVSync == u8VsyncPolarity) && --u8NumVSyncs == 0)
                break;
            bVSync = !bVSync;
        }

        if ( ( MsOS_GetSystemTime() - u32Time) >= u16Timeout )
        {
            break;
        }
    }

    return u8NumVSyncs;
}

MS_U8 MDrv_XC_wait_output_vsync(void *pInstance, MS_U8 u8NumVSyncs, MS_U16 u16Timeout, SCALER_WIN eWindow)
{
    MS_U32 u32Time;
    MS_U8 bVSync;
    MS_U8 u8VSyncVal;

    if(u8NumVSyncs == 0)
    {
       return u8NumVSyncs;
    }

    bVSync = 1;
    u32Time = MsOS_GetSystemTime();

    while (1)
    {
        u8VSyncVal = Hal_SC_ip_get_output_vsync_value(pInstance, eWindow);

        //for Special case,(like U4) if no subwindow don't wait vsync. break immediately
        if(u8VSyncVal == 0xFF)
            break;

        if (u8VSyncVal == bVSync)
        {
            if (bVSync)
            {
                u8NumVSyncs -= 1;
                if (u8NumVSyncs == 0)
                    break;
            }
            bVSync = !bVSync;
        }

        if ( (MsOS_GetSystemTime() - u32Time) >= u16Timeout )
        {
            break;
        }
    }
    return u8NumVSyncs;
}

//-------------------------------------------------------------------------------------------------
// This function will delay both 'u32DelayTime' ms and vsync signal
// @param  u32DelayTime      \b IN: delay time in ms unit
// @return MS_U32: real time cost of this timedelay
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_XC_DelayAndWaitOutVsync(void *pInstance, MS_U32 u32DelayTime, SCALER_WIN eWindow)
{
    #define TIME_OUPUTVSYNC_TIMEOUT 30

    MS_U32 u32TimeStart, u32TimeSpend=0;
    MS_U8 bVSync;
    MS_U8 u8VSyncVal;

    bVSync = 1;
    u32TimeStart = MsOS_GetSystemTime();

    while (1)
    {
        u8VSyncVal = Hal_SC_ip_get_output_vsync_value(pInstance, eWindow);
        //for Special case,(like U4) if no subwindow don't wait vsync. break immediately
        if(u8VSyncVal == 0xFF)
            break;
        if (u8VSyncVal == bVSync)
        {
            if ((u32TimeSpend >= u32DelayTime) && !bVSync)
            {
                //delay time is enough and now is also vsync ,so break
                break;
            }
            bVSync = !bVSync;
        }
        if (u32TimeSpend  >= (u32DelayTime + TIME_OUPUTVSYNC_TIMEOUT))
        {
            //wait vysnc time out
            break;
        }
        u32TimeSpend =  MsOS_GetSystemTime() - u32TimeStart;
    }
    return u32TimeSpend;
}
//-------------------------------------------------------------------------------------------------
/// This function will calculate and return H Frequency x 10
/// @param  u16HPeriod      \b IN: Horizontal period
/// @return MS_U16: H Frequency x 10
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_XC_CalculateHFreqx10(void* pInstance, MS_U16 u16HPeriod)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(u16HPeriod)
    {
        return ( (((MS_U32)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock + (u16HPeriod/2)) / u16HPeriod ) / 100 ); //KHz
    }
    else
    {
        return 1;   // avoid devide by 0 error
    }
}

MS_U16 MApi_XC_CalculateHFreqx10_U2(void* pInstance, MS_U16 u16HPeriod)
{
    MS_U16 u16Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u16Return = MDrv_XC_CalculateHFreqx10(pInstance, u16HPeriod);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u16Return;
}

MS_U16 MApi_XC_CalculateHFreqx10(MS_U16 u16HPeriod)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_HFREQX10 XCArgs;
    XCArgs.u16HPeriod = u16HPeriod;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_HFREQX10, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will calculate and return H Frequency x 1000
/// @param  u16HPeriod      \b IN: Horizontal period
/// @return MS_U32: H Frequency x 1000
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_XC_CalculateHFreqx1K(void *pInstance, MS_U16 u16HPeriod)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(u16HPeriod)
    {
        return (((MS_U32)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock + (u16HPeriod/2)) / u16HPeriod ); //MHz
    }
    else
    {
        return 1;   // avoid devide by 0 error
    }
}

MS_U32 MApi_XC_CalculateHFreqx1K_U2(void* pInstance, MS_U16 u16HPeriod)
{
    MS_U32 u32Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u32Return = MDrv_XC_CalculateHFreqx1K(pInstance, u16HPeriod);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32Return;
}

MS_U32 MApi_XC_CalculateHFreqx1K(MS_U16 u16HPeriod)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_HFREQX1K XCArgs;
    XCArgs.u16HPeriod = u16HPeriod;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_HFREQX1K, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will calculate and return V Frequency x 10
/// @param  u16HFreqx10      \b IN: Horizontal frequency x 10
/// @param  u16VTotal     \b IN: Vertical Total, usually the scan lines counts of a frame
/// @return MS_U16: V Frequency x 10
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_XC_CalculateVFreqx10(void *pInstance, MS_U16 u16HFreqx10, MS_U16 u16VTotal)
{
    if(u16HFreqx10 && u16VTotal)
    {
        return ( (((MS_U32)u16HFreqx10 * 1000 ) + (u16VTotal/2)) / u16VTotal ); // Hz
    }
    else
    {
        return 0;
    }
}

MS_U16 MApi_XC_CalculateVFreqx10_U2(void* pInstance, MS_U16 u16HFreq, MS_U16 u16VTotal)
{
    MS_U16 u16Return = 0;
    u16Return = MDrv_XC_CalculateVFreqx10(pInstance, u16HFreq, u16VTotal);
    return u16Return;
}

MS_U16 MApi_XC_CalculateVFreqx10(MS_U16 u16HFreq, MS_U16 u16VTotal)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_VFREQX10 XCArgs;
    XCArgs.u16HFreq = u16HFreq;
    XCArgs.u16VTotal = u16VTotal;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VFREQX10, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will calculate and return V Frequency x 1000
/// @param  u32HFreqx1K      \b IN: Horizontal frequency x 1000
/// @param  u16VTotal     \b IN: Vertical Total, usually the scan lines counts of a frame
/// @return MS_U32: V Frequency x 1000
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_XC_CalculateVFreqx1K(void *pInstance, MS_U32 u32HFreqx1K, MS_U16 u16VTotal)
{
    if(u32HFreqx1K && u16VTotal)
    {
        return ( (((MS_U32)u32HFreqx1K * 1000 ) + (u16VTotal/2)) / u16VTotal ); // (10^-3)Hz
    }
    else
    {
        return 0;
    }
}

MS_U32 MApi_XC_CalculateVFreqx1K_U2(void* pInstance, MS_U32 u32HFreqx1K, MS_U16 u16VTotal)
{
    if(u32HFreqx1K && u16VTotal)
    {
        return ( (((MS_U32)u32HFreqx1K * 1000 ) + (u16VTotal/2)) / u16VTotal ); // (10^-3)Hz
    }
    else
    {
        return 0;
    }
}

MS_U32 MApi_XC_CalculateVFreqx1K(MS_U32 u32HFreqx1K, MS_U16 u16VTotal)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_VFREQX1K XCArgs;
    XCArgs.u32HFreqx1K = u32HFreqx1K;
    XCArgs.u16VTotal = u16VTotal;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VFREQX1K, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will get V Frequency x 1000
/// @param  eWindow      \b IN: the window we are going to get V frequency
/// @return MS_U32: Accurate V Frequency x 1000
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_XC_GetAccurateVFreqx1K(void* pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32AccuratePeriod = 0;
    MS_U32 u32AccurateVFreq = 0;
    MS_U64 u64XTALClk = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    // for more correct VPeriod
    u32AccuratePeriod = MDrv_SC_ip_get_verticalperiod(pInstance, eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_SC_ip_get_verticalperiod = %tu\n", (ptrdiff_t)u32AccuratePeriod);

    if(u32AccuratePeriod != 0)
    {
        u64XTALClk = (MS_U64)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000;
        do_div(u64XTALClk,u32AccuratePeriod);
        u32AccurateVFreq = (MS_U32)u64XTALClk;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32AccurateVFreq = %tu\n", (ptrdiff_t)u32AccurateVFreq);
    return u32AccurateVFreq;
}

//-------------------------------------------------------------------------------------------------
/// MApi_XC_GetAccurateVFreqx1K is obsolete, please use MApi_XC_CalculateVFreqx10 instead!
/// the return vfreq of this function is not accurate when signal is not stable or have noise, so it's not
/// suitable as interface for Ap use at any time.
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_XC_GetAccurateVFreqx1K_U2(void* pInstance, SCALER_WIN eWindow)
{
    printf("[%s,%5d]Attention: MApi_XC_GetAccurateVFreqx1K is obsolete, please use MApi_XC_CalculateVFreqx10 instead!\n",__FUNCTION__,__LINE__);
    return 0;
}

MS_U32 MApi_XC_GetAccurateVFreqx1K(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_ACCURATE_VFREQX1k XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_ACCURATE_VFREQX1k, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will calculate and return Vsync time
/// @param  u16VTotal       \b IN: Vercital total
/// @param  u16HPeriod      \b IN: Horizontal period
/// @return MS_U8:  VSync time
//-------------------------------------------------------------------------------------------------
MS_U8 MApi_XC_CalculateVsyncTime(void *pInstance, MS_U16 u16VTotal, MS_U16 u16HPeriod)
{
    MS_U16 wHFreqX10; // horizontal frequency
    wHFreqX10 = MDrv_XC_CalculateHFreqx10(pInstance, u16HPeriod); // calculate HSync frequency
    return (MS_U8)(((u16VTotal*10)+(wHFreqX10/2))/wHFreqX10);
}
/******************************************************************************/
///Set line buffer clock
///@param u16HTotal \b IN
///- Horizontal total
///@return
///- Output Pixel colok
/******************************************************************************/
MS_U16 MDrv_XC_cal_pixclk(void *pInstance, MS_U16 u16HTotal, SCALER_WIN eWindow)
{
    return (MS_U16)
    (((MS_U32)MDrv_XC_CalculateHFreqx10(pInstance, Hal_SC_ip_get_horizontalPeriod(pInstance, eWindow))*u16HTotal+5000)/10000);
}


#if ENABLE_DEBUG_CAPTURE_N_FRAMES
static void _MApi_XC_InputVSyncISR(SC_INT_SRC eIntNum, void * pParam)
{
    volatile MS_BOOL *bFinishedISR = (volatile MS_BOOL *) pParam;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_IP.u8Cnt++;

#if 1
    if(pXCResourcePrivate->stdrvXC_IP.u8Cnt == 4)
    {
        Hal_SC_set_freezeimg(pInstance, ENABLE, MAIN_WINDOW);
        pXCResourcePrivate->stdrvXC_IP.u8Cnt = 0;
        *bFinishedISR = TRUE;
    }
#else
    printf("(%3d) E %4x W %d, R %d, pha %4x, prd %4x\n",
        u8Cnt,
        SC_R2BYTEMSK(0, REG_SC_BK01_1E_L, 0x0400),
        SC_R2BYTEMSK(0, REG_SC_BK12_3A_L, 0x07),           // current write bank
        SC_R2BYTEMSK(0, REG_SC_BK12_3A_L, 0x70) >> 4,      // current read bank
        MDrv_Read2Byte(L_BK_LPLL(0x11)),                // phase error
        MDrv_Read2Byte(L_BK_LPLL(0x13)));               // prd error
#endif
}
#endif

MS_BOOL MApi_XC_InterruptAttach_U2(void* pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_BOOL bReturn = MDrv_XC_InterruptAttach(pInstance, eIntNum, pIntCb,pParam);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_InterruptAttach(SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_INTERRUPT_ATTACH XCArgs;
    XCArgs.eIntNum = eIntNum;
    XCArgs.pIntCb = pIntCb;
    XCArgs.pParam = pParam;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_INTERRUPT_ATTACH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_InterruptDeAttach_U2(void* pInstance, SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_BOOL bReturn = MDrv_XC_InterruptDeAttach(pInstance, eIntNum, pIntCb,pParam);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_InterruptDeAttach(SC_INT_SRC eIntNum, SC_InterruptCb pIntCb, void * pParam)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_INTERRUPT_DEATTACH XCArgs;
    XCArgs.eIntNum = eIntNum;
    XCArgs.pIntCb = pIntCb;
    XCArgs.pParam = pParam;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_INTERRUPT_DEATTACH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
//
// Attention! This function should only be used in the xc interrupt!!!
//
/// This function will tell the current read bank
/// @param  eWindow          \b IN: Window
/// @return  MS_U16   Current read bank
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_GetCurrentReadBank_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16CurrentReadBank = 0;
#if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8Bank = 0;
    u8Bank = MDrv_ReadByte( BK_SELECT_00 );
#endif
    if(MAIN_WINDOW == eWindow)
    {
        u16CurrentReadBank = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_3A_L, 0xF0) >> 4 ;
    }
    else
    {
        u16CurrentReadBank = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_7A_L, 0xF0) >> 4 ;
    }
#if (!ENABLE_REGISTER_SPREAD)
    MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
    return u16CurrentReadBank;
}

MS_U16 MApi_XC_GetCurrentReadBank(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_CURRENT_READBANK XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CURRENT_READBANK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
//
// Attention! This function should only be used in the xc interrupt!!!
//
/// This function will tell the current write bank
/// @param  eWindow          \b IN: Window
/// @return  MS_U16   Current write bank
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_GetCurrentWriteBank_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16CurrentWriteBank = 0;
#if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8Bank = 0;
    u8Bank = MDrv_ReadByte( BK_SELECT_00 );
#endif
    if(MAIN_WINDOW == eWindow)
    {
        u16CurrentWriteBank = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_3A_L, 0xF);
    }
    else
    {
        u16CurrentWriteBank = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_7A_L, 0xF);
    }
#if (!ENABLE_REGISTER_SPREAD)
    MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
    return u16CurrentWriteBank;
}

MS_U16 MApi_XC_GetCurrentWriteBank(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_CURRENT_WRITEBANK XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u16ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CURRENT_WRITEBANK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u16ReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// This function will disable input source
/// @param  bDisable       \b IN: TRUE : Disable; FALSE: Enable
//-------------------------------------------------------------------------------------------------
void MApi_XC_DisableInputSource_U2(void* pInstance, MS_BOOL bDisable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    gSrcInfo[eWindow].bBluescreenEnabled = bDisable;
    //when enable auto no signal, the signal goto stable state.
    //and then disable input source at timing monitor function not here.
    if(gSrcInfo[eWindow].bAutoNoSignalEnabled && Hal_SC_ip_get_IPAutoNoSignal(pInstance, eWindow)
       && bDisable)
    {
        msg_sc_ip(printf(" return disable input source\n"));
    }
    else if(!((bDisable == FALSE) &&
       (eWindow == SUB_WINDOW) &&
       gSrcInfo[eWindow].Status2.bIPMemWriteRequestDisabled ))
    {
        //Sub window use disable IP memory request to disable input source
        //So, if bIPMemWriteRequestDisabled = TRUE, need skip enable SUB input source
        MDrv_XC_DisableInputSource(pInstance, bDisable, eWindow);
    }
    _XC_RETURN(pInstance);


#if ENABLE_DEBUG_CAPTURE_N_FRAMES
    printf("MApi_XC_DisableInputSource %d %d\n", bDisable, eWindow);

    if(bDisable)
    {
        // start ISR monitor
        MDrv_XC_InterruptDeAttach(pInstance, SC_INT_F2_IPVS_SB, _MApi_XC_InputVSyncISR, (void *) &pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR);
    }
    else
    {
        pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR = FALSE;

        if(!MDrv_XC_InterruptIsAttached(pInstance, SC_INT_F2_IPVS_SB, _MApi_XC_InputVSyncISR, (void *) &pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR))
        {
            MDrv_XC_InterruptAttach(pInstance, SC_INT_F2_IPVS_SB, _MApi_XC_InputVSyncISR, (void *) &pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR);

            while(*((volatile MS_BOOL *) &pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR) == FALSE)
            {
            }

            MDrv_XC_InterruptDeAttach(pInstance, SC_INT_F2_IPVS_SB, _MApi_XC_InputVSyncISR, (void *) &pXCResourcePrivate->stdrvXC_IP.bFinishedInputVSyncISR);
        }
    }
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

/*    if(bDisable)
    {
        if(eWindow==0)
            printf("Main win Blue Screen ON\n");
        else
            printf("Sub win Blue Screen ON\n");
    }
    else
    {
        if(eWindow==0)
            printf("Main win Blue Screen OFF\n");
        else
            printf("Sub win Blue Screen OFF\n");
    }
*/
}

void MApi_XC_DisableInputSource(MS_BOOL bDisable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_DISABLE_INPUTSOURCE XCArgs;
    XCArgs.bDisable = bDisable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DISABLE_INPUTSOURCE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

// Used in driver
void MDrv_XC_DisableInputSource(void *pInstance, MS_BOOL bDisable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].bBluescreenEnabled = bDisable;

    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        && (!MDrv_SC_CheckMuteStatusByRegister(pInstance, eWindow)))
    {
        _MLOAD_ENTRY(pInstance);
        // HW PIP architeucture
        // Becasue BK3_02[8] and BK20_11[15] should enable together, otherwise garbage will be showed,
        // otherwise garbage will be showed,we need to use BK12_47[0] to instead.
        if( eWindow == MAIN_WINDOW )
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK01_02_L, (bDisable ? BIT(7):0), BIT(7));
        }
        else
        {
            if(DISABLE==bDisable)
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK03_02_L, 0 , BIT(7));
            }
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK12_47_L, (bDisable ? BIT(0) : 0), BIT(0));
            #if (HW_DESIGN_4K2K_VER == 4)
                if ((psXCInstPri->u32DeviceID==0)
                     && !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
                {
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK32_47_L, (bDisable ? BIT(0) : 0), BIT(0));
                }
            #endif
        }
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        if( eWindow == MAIN_WINDOW )
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L,   (bDisable ? BIT(7) : 0), BIT(7));
        }
        else
        {
            if(!MDrv_SC_CheckMuteStatusByRegister(pInstance, eWindow)&&!MDrv_SC_GetSkipWaitVsync(pInstance, eWindow))
            {
                MDrv_XC_wait_output_vsync(pInstance, 2, 200, MAIN_WINDOW);
            }
            if(DISABLE==bDisable)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID ,REG_SC_BK03_02_L, 0 , BIT(7));
            }
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID ,REG_SC_BK12_47_L, (bDisable ? BIT(0) : 0), BIT(0));
            #if (HW_DESIGN_4K2K_VER == 4)
                if ((psXCInstPri->u32DeviceID==0)
                       && !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID ,REG_SC_BK32_47_L, (bDisable ? BIT(0) : 0), BIT(0));
                }
            #endif
        }
    }

#ifndef DONT_USE_CMA
#if (XC_SUPPORT_CMA ==TRUE)
    if (
#if (HW_DESIGN_4K2K_VER == 7)
        (psXCInstPri->u32DeviceID == 0) &&
#endif
        (DISABLE == bDisable)&&( eWindow == MAIN_WINDOW ))
    {
        if (bNeedDeattachCMAISR == TRUE)
        {
            MHal_XC_CMAPatch(pInstance, MAIN_WINDOW);
            bNeedDeattachCMAISR = FALSE;
        }
    }
#endif
#endif

    if((DISABLE==bDisable) && (eWindow == SUB_WINDOW))
    {
        MDrv_XC_Enable_Extra_Request(pInstance, ENABLE);
    }
}

MS_BOOL MDrv_XC_IsInputSourceDisabledByHwAutoNoSignal(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if (eWindow == MAIN_WINDOW)
        return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_02_L, 0x0080) >> 7);
    else
        return (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_02_L, 0x0080) >> 7);
}

MS_BOOL MDrv_XC_IsInputSourceDisabled(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bInputSourceDisabled = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(gSrcInfo[eWindow].bAutoNoSignalEnabled)
        bInputSourceDisabled = MDrv_XC_IsInputSourceDisabledByHwAutoNoSignal(pInstance, eWindow);
    else
        bInputSourceDisabled = gSrcInfo[eWindow].bBluescreenEnabled;
    return bInputSourceDisabled;
}

MS_BOOL MApi_XC_IsInputSourceDisabled_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    bReturn = MDrv_XC_IsInputSourceDisabled(pInstance, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_IsInputSourceDisabled(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_INPUTSOURCE_DISABLED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_INPUTSOURCE_DISABLED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get the auto phase value for the specific window
/// @param  eWindow                \b IN: which window we are going to get
/// @return MS_U32: Return full image sun of difference value between two pixles
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_XC_GetPhaseValue(void *pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32PhaseValue; // double word buffer
    MS_U16 u8TimeOut = 0;
    MS_U16 u16Dummy = 1000;
    MS_U8 u8FirstReady =0 ,u8SecondReady = 0;

    //u8Bank = MDrv_ReadByte( BK_SELECT_00 );
    //MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F2 );
    _XC_ENTRY(pInstance);
    while ( Hal_SC_ip_is_auto_phase_result_ready(pInstance, eWindow) && u8TimeOut < 500 )
    {
        u8TimeOut++;
    }
    //printf(" %u timeout \n",u8TimeOut );
    if ( u8TimeOut >= 500 )
    {
        _XC_RETURN(pInstance);
        return 0; // Timeout
    }
    u8TimeOut = 0 ;

    while ( u8TimeOut < 100 )
    {
        // Waiting for ready

        while( !( Hal_SC_ip_is_auto_phase_result_ready(pInstance, eWindow) ) && (u16Dummy--) ) ;

        u8FirstReady = Hal_SC_ip_is_auto_phase_result_ready(pInstance, eWindow);

        u32PhaseValue = Hal_SC_ip_get_auto_phase_value(pInstance, eWindow);

        u8SecondReady = Hal_SC_ip_is_auto_phase_result_ready(pInstance, eWindow);

        if ( u8FirstReady == TRUE && u8SecondReady == TRUE )
        {
            _XC_RETURN(pInstance);
            return u32PhaseValue;
        }
        u8TimeOut++;
    }
    _XC_RETURN(pInstance);
    return 0;

}

//-------------------------------------------------------------------------------------------------
/// Auto-tune R/G/B Offset of ADC.
/// @param  u8Type              \b IN: report AUTO_MIN_[RGB] or AUTO_MAX_[RGB]
/// @param  u8DelayMS           \b IN: delay how long to get the report, usually, the delay will be 1 * VSyncTime
/// @return @ref MS_BOOL the min/max gain report
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_GetAutoGainResult(void *pInstance, AUTO_GAIN_TYPE u8Type, MS_U8 u8DelayMS, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U8 u8Ready0, u8Ready1, u8GainRptTimeout = 255;
    MS_U16 u16Value = 0;
    //MS_U8 u8Bank;

    //u8Bank = MDrv_ReadByte( BK_SELECT_00 );

    if(eWindow == MAIN_WINDOW)
    {
        //MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F2 );
        //MDrv_WriteByteMask(L_BK_IP1F2(0x0F), u8Type, BIT(2) | BIT(1) | BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u8Type, BIT(2) | BIT(1) | BIT(0));
    }
    else
    {
        //MDrv_WriteByte( BK_SELECT_00, REG_BANK_IP1F1 );
        //MDrv_WriteByteMask(L_BK_IP1F1(0x0F), u8Type, BIT(2) | BIT(1) | BIT(0));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_0F_L, u8Type, BIT(2) | BIT(1) | BIT(0));
    }

    MsOS_DelayTask(u8DelayMS);

    // Sometimes the gain report will give a very un-reasonable value,
    // So, we've to wait the IP1F2_0E[1] to be ready (=1) to get the gain report
    // For safety reason, you may use this flow:
    //    (1) Set what to get AUTO_MIN_[RGB] or AUTO_MAX_[RGB]
    //    (2) wait for ready bit to be off
    //                           (because sometimes when you change the AUTO_MIN_[RGB] or AUTO_MAX_[RGB],
    //                                you'll get a ready immediately but the report is the old value)
    //    (3) wait for ready bit to be on
    //    (4) get the report when ready bit is on (note: see source code below)
    //
    // For quick response, you may get the report when ready bit is on, the report is rarely wrong if ready bit is on.

    /*
    // wait for ready bit to be off
    while(MDrv_ReadByte(L_BK_IP1F2(0x0E)) & (0x1 << 1))
    {
        u8Ready = MDrv_ReadByte(L_BK_IP1F2(0x0E));
        u8Value = MDrv_ReadByte(L_BK_IP1F2(0x11));
        printf("DA:C=0x%bx,R=0x%bx,V=0x%bx\n", MDrv_ReadByte(L_BK_IP1F2(0x0F)), u8Ready, u8Value);
    }

    // wait for ready bit to be on
    while(!(MDrv_ReadByte(L_BK_IP1F2(0x0E)) & (0x1 << 1)))
    {
        u8Ready = MDrv_ReadByte(L_BK_IP1F2(0x0E));
        u8Value = MDrv_ReadByte(L_BK_IP1F2(0x11));
        printf("DA:C=0x%bx,R=0x%bx,V=0x%bx\n", MDrv_ReadByte(L_BK_IP1F2(0x0F)), u8Ready, u8Value);
    }
    */

    while(u8GainRptTimeout)
    {
        // It's important to read the 2 values by this order
        // (1) ready bit first
        // (2) report next
        // and no waste time between these 2 commands

        // Waiting until ready bit raise
        MS_U16 u16Dummy = 1000;
        while(!( MApi_XC_IsAutoGainResultReady(pInstance, eWindow) ) && (u16Dummy--)) ;

        if(eWindow == MAIN_WINDOW)
        {
            u8Ready0 = MApi_XC_IsAutoGainResultReady(pInstance, eWindow);
            //u16Value = (MDrv_Read2Byte(L_BK_IP1F2(0x11)) & 0x3FF);  // the 10 bit report is 0.0 ~ 255.75, [9:2] = 0 ~ 255, [1:0] = 0, 0.25, 0.5, 0.75
            u16Value = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_11_L) & 0x3FF);  // the 10 bit report is 0.0 ~ 255.75, [9:2] = 0 ~ 255, [1:0] = 0, 0.25, 0.5, 0.75
            u8Ready1 = MApi_XC_IsAutoGainResultReady(pInstance, eWindow);
        }
        else
        {
            u8Ready0 = MApi_XC_IsAutoGainResultReady(pInstance, eWindow);
            //u16Value = (MDrv_Read2Byte(L_BK_IP1F1(0x11)) & 0x3FF);  // the 10 bit report is 0.0 ~ 255.75, [9:2] = 0 ~ 255, [1:0] = 0, 0.25, 0.5, 0.75
            u16Value = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_11_L) & 0x3FF);  // the 10 bit report is 0.0 ~ 255.75, [9:2] = 0 ~ 255, [1:0] = 0, 0.25, 0.5, 0.75
            u8Ready1 = MApi_XC_IsAutoGainResultReady(pInstance, eWindow);
        }

        // Check if the report is reasonable here
        if( u8Ready0 && u8Ready1)
        {
            break;
        }
        u8GainRptTimeout--;
    }
    MS_ASSERT(u8GainRptTimeout > 0);

    //MDrv_WriteByte( BK_SELECT_00, u8Bank );

    return u16Value;

}

//-------------------------------------------------------------------------------------------------
/// Check whether Auto phase result is ready or not
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsAutoPhaseResultReady(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_ip_is_auto_phase_result_ready(pInstance, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// Get auto phase result value
/// @param  eWindow                \b IN: which window we are going to query
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_U32 MApi_XC_GetAutoPhaseResult(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_ip_get_auto_phase_value(pInstance, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// check whether the auto position is ready or not for the specific window
/// @param  eWindow             \b IN: which window we are going to set
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_IsAutoPositionResultReady(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bAuto_position;
    _XC_ENTRY(pInstance);
    bAuto_position = Hal_SC_ip_is_auto_position_result_ready(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return bAuto_position ; //Hal_SC_ip_is_auto_position_result_ready(eWindow);

}

//-------------------------------------------------------------------------------------------------
/// enable or disable the auto position for the specific window
/// @param  u8Enable            \b IN: enable or disable the auto position
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_AutoPositionEnable(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_set_auto_position_function(pInstance, u8Enable, eWindow);
    _XC_RETURN(pInstance);
}

//-------------------------------------------------------------------------------------------------
/// enable or disable the auto phase for the specific window
/// @param  bEnable            \b IN: enable or disable the auto phase
/// @param  eWindow             \b IN: which window we are going to set
/// @return @ref MS_BOOL the auto phase enable setting result
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_AutoPhaseEnable(void *pInstance, MS_BOOL bEnable , SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if( eWindow == MAIN_WINDOW )
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,  REG_SC_BK01_19_L, (bEnable?BIT(0):0) , BIT(0) );
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,  REG_SC_BK03_19_L, (bEnable?BIT(0):0) , BIT(0) );
    }
    _XC_RETURN(pInstance);
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// set valid data threshold for the specific window
/// @param  u8Threshold         \b IN: the threshold to set
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MDrv_XC_SetValidDataThreshold(void *pInstance, MS_U8 u8Threshold , SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_set_valid_data_threshold(pInstance, u8Threshold, eWindow);
    _XC_RETURN(pInstance);
}

//-------------------------------------------------------------------------------------------------
/// get auto position vstart for the specific window
/// @param  eWindow             \b IN: which window we are going to get
/// @return @ref MS_U16 the auto position vstart
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_SetAutoPositionVstart(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16V_start;
    _XC_ENTRY(pInstance);
    u16V_start = Hal_SC_ip_get_auto_position_v_start(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return u16V_start ;//Hal_SC_ip_get_auto_position_v_start(eWindow);
}

//-------------------------------------------------------------------------------------------------
/// get auto position hstart for the specific window
/// @param  eWindow             \b IN: which window we are going to get
/// @return @ref MS_U16 the auto position hstart
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_SetAutoPositionHstart(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16H_start;
    _XC_ENTRY(pInstance);
    u16H_start = Hal_SC_ip_get_auto_position_h_start(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return u16H_start;//Hal_SC_ip_get_auto_position_h_start(eWindow);
}

//-------------------------------------------------------------------------------------------------
/// get auto position vendt for the specific window
/// @param  eWindow             \b IN: which window we are going to get
/// @return @ref MS_U16 the auto position vend
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_SetAutoPositionVend(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16V_end;
    _XC_ENTRY(pInstance);
    u16V_end = Hal_SC_ip_get_auto_position_v_end(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return u16V_end;//Hal_SC_ip_get_auto_position_v_end(eWindow);
}

//-------------------------------------------------------------------------------------------------
/// get auto position Horizontal end for the specific window
/// @param  eWindow             \b IN: which window we are going to get
/// @return @ref MS_U16 the auto position horizontal end
//-------------------------------------------------------------------------------------------------
MS_U16 MApi_XC_SetAutoPositionHend(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16H_end;
    _XC_ENTRY(pInstance);
    u16H_end = Hal_SC_ip_get_auto_position_h_end(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return u16H_end;//Hal_SC_ip_get_auto_position_h_end(eWindow);
}

//-------------------------------------------------------------------------------------------------
/// get auto position force pixel mode for the specific window
/// @param  eWindow             \b IN: which window we are going to get
/// @return @ref MS_BOOL auto position force pixel mode enabled
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_GetAutoPositionForcePixelMode(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_10_L, BIT(8)) >> 8;
    else
        return SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_10_L, BIT(8)) >> 8;
}

//-------------------------------------------------------------------------------------------------
/// set auto position force pixel mode for the specific window
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MDrv_XC_SetAutoPositionForcePixelMode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_10_L, (bEnable ? BIT(8) : 0), BIT(8));
    else
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_10_L, (bEnable ? BIT(8) : 0), BIT(8));
}

//-------------------------------------------------------------------------------------------------
/// set auto range window for the specific window
/// @param  u16Vstart             \b IN: vertical start
/// @param  u16Hstart             \b IN: horizontal start
/// @param  u16Vsize              \b IN: vertical size
/// @param  u16Hsize              \b IN: horizontal size
/// @param  eWindow             \b IN: which window we are going to get
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetAutoRangeWindow(void *pInstance, MS_U16 u16Vstart, MS_U16 u16Hstart, MS_U16 u16Vsize, MS_U16 u16Hsize, SCALER_WIN eWindow)
{
    Hal_SC_ip_set_auto_range_window(pInstance, u16Vstart, u16Hstart, u16Vsize, u16Hsize, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// enable or disable the auto range for the specific window
/// @param  u8Enable            \b IN: enable or disable the auto range
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_AutoRangeEnable(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow)
{
    Hal_SC_ip_set_auto_range_function(pInstance, u8Enable, eWindow);
}

//-------------------------------------------------------------------------------------------------
/// enable or disable the Vsync polarity detect mode for the specific window
/// @param  u8Enable            \b IN: enable or disable
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_VsyncPolarityDetectMode(void *pInstance, MS_U8 u8Enable , SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_set_reg_usr_vspolmd(pInstance, u8Enable, eWindow);
    _XC_RETURN(pInstance);
}

//-------------------------------------------------------------------------------------------------
/// init register for timing change
/// @param  eWindow             \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_TimingChangeInit(void *pInstance, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_init_reg_in_timingchange(pInstance, eWindow);
    _XC_RETURN(pInstance);
}


void MDrv_XC_init_riu_base(MS_VIRT u32riu_base, MS_VIRT u32PMriu_base)
{
    Hal_SC_init_riu_base(u32riu_base, u32PMriu_base);
}

//-----------------------------------------------------------------------------
/// Enable/Disable Euro/Australia Hdtv Detection support in driver
/// @param  bEnable:
/// - ENABLE : enable support
/// - DISABLE : disable disable
/// @return None.
//-----------------------------------------------------------------------------
void MApi_XC_EnableEuroHdtvSupport_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context._bEnableEuro = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_EnableEuroHdtvSupport(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_EUROHDTV_SUPPORT_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_EUROHDTV_SUPPORT_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-----------------------------------------------------------------------------
/// Enable Euro/Australia Hdtv Detection
/// @param  bEnable:
/// - ENABLE : enable detection
/// - DISABLE : disable detection
/// @return None.
//-----------------------------------------------------------------------------
void MApi_XC_EnableEuroHdtvDetection_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    //MS_U8 u8Bank;
    MS_U32 u32Reg_1F_H, u32Reg_29_L;

    _XC_ENTRY(pInstance);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    // switch to BK1F2
    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        u32Reg_1F_H = REG_SC_BK01_1F_H;
        u32Reg_29_L = REG_SC_BK01_29_L;
    }
    else
    {
        u32Reg_1F_H = REG_SC_BK03_1F_H;
        u32Reg_29_L = REG_SC_BK03_29_L;
    }
    if(bEnable)
    {
        //(printf("Enable detecting EURO_HDTV\n");
        // set read HSync pulse width
        // old setting: we don't switch hsync/vsync-read in new chip because hsync & vsync are reported by different register.
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Reg_1F_H, 0, BIT(5));// set read HSync pulse width instead of VSync pulse width
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Reg_29_L, (BIT(7)|BIT(1)), 0x00FF);// All case, auto field
    }
    else
    {
        //printf("Disable detecting EURO_HDTV\n");
        Hal_SC_set_cs_det_cnt(pInstance, 0, eWindow);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Reg_29_L, 0x00, 0x00FF);// All case, auto field
    }
    _XC_RETURN(pInstance);

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

void MApi_XC_EnableEuroHdtvDetection(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_EUROHDTV_DETECTION_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_EUROHDTV_DETECTION_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//-----------------------------------------------------------------------------
/// Get Euro/Australia Hdtv Detection Result
/// @return MS_DETECT_EURO_HDTV_STATUS_TYPE:
/// - EURO_HDTV_DETECTED
/// - AUSTRALIA_HDTV_DETECTED
/// - NORMAL : No Euro/Australia HDTV detected.
//-----------------------------------------------------------------------------
MS_DETECT_EURO_HDTV_STATUS_TYPE MDrv_XC_ip_GetEuroHdtvStatus(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //MS_U8 u8Status;
    //MS_U8 u8Bank;
    MS_U16 u16Status;
    MS_DETECT_EURO_HDTV_STATUS_TYPE Euro_status;

    // switch to BK1F2
    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if(eWindow == MAIN_WINDOW)
    {
        /*MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        u8Status = MDrv_ReadByte(L_BK_IP1F2(0x29));*/
        u16Status = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_29_L);
    }
    else
    {
        /*MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F1);
        u8Status = MDrv_ReadByte(L_BK_IP1F1(0x29));*/
        u16Status = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK03_29_L);
    }

    //if( (u8Status & 0x0C) == 0x0C )
    if( (u16Status & 0x000C) == 0x000C )
    {
        msg_sc_ip(printf("EURO_HDTV_DETECTED (0x%x)\n", u16Status));
        Euro_status = EURO_HDTV_DETECTED;
    }
    //else if( (u8Status & 0x0C) == 0x08 )
    else if( (u16Status & 0x000C) == 0x0008 )
    {
        msg_sc_ip(printf("AUSTRALIA_HDTV_DETECTED (0x%x)\n", u16Status));
        Euro_status = AUSTRALIA_HDTV_DETECTED;
    }
    else
    {
        msg_sc_ip(printf("NORMAL_HDTV_DETECTED (0x%x)\n", eWindow ? SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK03_29_L) : SC_R2BYTE(0,REG_SC_BK01_29_L)));
        Euro_status = EURO_AUS_HDTV_NORMAL;
    }

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);

    return Euro_status;
}

//-----------------------------------------------------------------------------
/// Euro/Australia Hdtv Detection Handler, this handler was executed if no input sync detected
/// and will try to detect if Euro/Australia/Normal input.
/// @param  eWindow             \b IN: which window we are going to set
/// @return None.
//-----------------------------------------------------------------------------
void MApi_XC_EuroHdtvHandler(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //40ms delay in this function will cause ZUI MApp_ZUI_ACT_TransitionEffectDoing too slow.
    //in order to speed up ZUI MApp_ZUI_ACT_TransitionEffectDoing, we disable 40ms delay in this function
    //and let g_ucModeDetectPollingCounter in MApp_PCMode_TimingMonitor to handle the delay
    //MApp_PCMode_TimingMonitor will be called every 30ms while MDrv_Mode_DetectEuroHdtvHandler will be called every 60ms
    if(pXCResourcePrivate->stdrvXC_IP.u8Count)
    {
        pXCResourcePrivate->stdrvXC_IP.u8Count--;
        return;
    }
    _XC_ENTRY(pInstance);
    pXCResourcePrivate->stdrvXC_IP.u8Count = 1;

    // Toggle Composite Sync Separate Decision Count
    if( Hal_SC_get_cs_det_cnt(pInstance, eWindow) == 0x00)
    {
        Hal_SC_set_cs_det_cnt(pInstance, Hal_SC_get_plus_width(pInstance, eWindow), eWindow);
    }
    else
    {
        _XC_RETURN(pInstance);
        MDrv_XC_HWAuto_SyncSeparate(pInstance, eWindow);
        _XC_ENTRY(pInstance);
    }
    //printf("++++++++++++++++++++++++XC_MD_SYNC_LOSS<>:%x, %x\n",Hal_SC_get_cs_det_cnt(eWindow),Hal_SC_get_plus_width(eWindow));

    //MsOS_DelayTask(40);     // Delay till stable

    //printf("SepCnt=0x%bx\n", MDrv_ReadByte(H_BK_IP1F2(0x0C)));

    _XC_RETURN(pInstance);
    return;
}

//-----------------------------------------------------------------------------
// set composite sync separate decision count to 0:HW Auto Decide
// @return None.
//-----------------------------------------------------------------------------
void MDrv_XC_HWAuto_SyncSeparate(void *pInstance, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    // Toggle Composite Sync Separate Decision Count
    if( Hal_SC_get_cs_det_cnt(pInstance, eWindow) != 0x00)
    {
        Hal_SC_set_cs_det_cnt(pInstance, 0x00, eWindow);

        if(eWindow == MAIN_WINDOW)
            Hal_SC_ip_software_reset(pInstance,REST_IP_F2,eWindow);
        else
            Hal_SC_ip_software_reset(pInstance,REST_IP_F1,eWindow);
    }

    _XC_RETURN(pInstance);

    return;
}

//-----------------------------------------------------------------------------
// set FilmMode_P
// @return None.
//-----------------------------------------------------------------------------
void MApi_XC_FilmMode_P_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    HAL_SC_FilmMode_Patch1(pInstance);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_FilmMode_P(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FILM_MODE_PATCH, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_set_FD_Mask(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FDMASK_BYWIN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = MAIN_WINDOW;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}


MS_BOOL MApi_XC_Get_FD_Mask(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FDMASK_BYWIN XCArgs;
    XCArgs.eWindow      = MAIN_WINDOW;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_Set_FD_Mask_ByWin_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_Set_FD_Mask_ByWin(pInstance, bEnable, eWindow);
    _XC_RETURN(pInstance);

    return;
}

void MApi_XC_set_FD_Mask_byWin(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FDMASK_BYWIN XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_Get_FD_Mask_ByWin_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = HAL_SC_ip_Get_FD_Mask_ByWin(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_Get_FD_Mask_byWin(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FDMASK_BYWIN XCArgs;
    XCArgs.eWindow      = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FDMASK_BYWIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Get_FD_Mask_Status_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = HAL_SC_ip_Get_FD_Mask_Status(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_Get_FD_Mask_Status(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FDMASK_STATUS XCArgs;
    XCArgs.eWindow      = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FDMASK_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_set_GOP_Enable_U2(void* pInstance, MS_U8 MaxGOP ,MS_U8 UseNum, MS_U8 u8MuxNum, MS_BOOL bEnable)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    bRet = HAL_SC_set_GOP_Enable(pInstance, MaxGOP , UseNum, u8MuxNum, bEnable);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_set_GOP_Enable(MS_U8 MaxGOP ,MS_U8 UseNum, MS_U8 u8MuxNum, MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_GOP_ENABLE XCArgs;
    XCArgs.MaxGOP = MaxGOP;
    XCArgs.UseNum = UseNum;
    XCArgs.u8MuxNum = u8MuxNum;
    XCArgs.bEnable = bEnable;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_GOP_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_ip_sel_for_gop_U2(void* pInstance, MS_U8 u8MuxNum ,MS_XC_IPSEL_GOP ipSelGop)
{
    _XC_ENTRY(pInstance);
    HAL_SC_ip_sel_for_gop(pInstance, u8MuxNum , ipSelGop);
    _XC_RETURN(pInstance);
}

void MApi_XC_ip_sel_for_gop(MS_U8 u8MuxNum ,MS_XC_IPSEL_GOP ipSelGop)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SELECT_IP_FOR_GOP XCArgs;
    XCArgs.u8MuxNum = u8MuxNum;
    XCArgs.ipSelGop = ipSelGop;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SELECT_IP_FOR_GOP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set Scaler VOP New blending level
/******************************************************************************/
void MApi_XC_SetVOPNBL_U2(void* pInstance)
{
    _XC_ENTRY(pInstance);
    HAL_SC_SetVOPNBL(pInstance);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetVOPNBL(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_VOP_NEW_BLENDING_LEVEL, NULL) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MApi_XC_GetDstInfo_IP1(void *pInstance, MS_XC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo, SCALER_WIN eWindow)
{
    MS_BOOL bResult = FALSE;
    MS_WINDOW_TYPE capWin;

    if(NULL == pDstInfo)
    {
        msg_sc_ip(printf("MApi_XC_GetDstInfo_IP1():pDstInfo is NULL\n");)
        return FALSE;
    }

    if(u32SizeofDstInfo != sizeof(MS_XC_DST_DispInfo))
    {
        msg_sc_ip(printf("MApi_XC_GetDstInfo_IP1():u16SizeofDstInfo is different from the MS_XC_DST_DispInfo defined, check header file!\n");)
        return FALSE;
    }

    Hal_SC_ip_get_capture_window(pInstance, &capWin, eWindow);
    pDstInfo->VDTOT = Hal_SC_ip_get_verticaltotal(pInstance, eWindow);
    pDstInfo->HDTOT = 0;//cannot get this info
    pDstInfo->DEHST = capWin.x;
    pDstInfo->DEHEND= capWin.x + capWin.width - 1;
    pDstInfo->DEVST= capWin.y;
    pDstInfo->DEVEND= capWin.y + capWin.height - 1;
    pDstInfo->bInterlaceMode = Hal_SC_ip_get_interlace_status(pInstance, eWindow);

    pDstInfo->bYUVInput = Hal_SC_get_ip2_csc(pInstance, eWindow);
    bResult = TRUE;

    msg_sc_ip(printf("pDstInfo:bInterlaceMode=%x,HDTOT=%lx,VDTOT=%lx,DEHST=%lx,DEHEND=%lx,DEVST=%lx,DEVEND=%lx\n",
        pDstInfo->bInterlaceMode, pDstInfo->HDTOT, pDstInfo->VDTOT, pDstInfo->DEHST, pDstInfo->DEHEND,
        pDstInfo->DEVST, pDstInfo->DEVEND);)
    return bResult;

}

//------------------------------------------------------------------------------
/// XC IP0 Get Destination Information for GOP mixer
/// @return TRUE/FALSE
//------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetDstInfo_U2(void* pInstance, MS_XC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo, EN_GOP_XCDST_TYPE XCDstType)
{
    MS_BOOL bResult = FALSE;
    MS_WINDOW_TYPE stXCMainDisWin;
    memset(&stXCMainDisWin, 0, sizeof(MS_WINDOW_TYPE));
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);

    if(NULL == pDstInfo)
    {
        msg_sc_ip(printf("MApi_XC_GetDstInfo_IP():pDstInfo is NULL\n");)
        bResult = FALSE;
    }else if(u32SizeofDstInfo != sizeof(MS_XC_DST_DispInfo))
    {
        msg_sc_ip(printf("MApi_XC_GetDstInfo_IP():u16SizeofDstInfo is different from the MS_XC_DST_DispInfo defined, check header file!\n");)
        bResult = FALSE;
    }else if(XCDstType >= MAX_GOP_XCDST_SUPPORT)
    {
        msg_sc_ip(printf("MApi_XC_GetDstInfo_IP():XCDstType(%x) is out of range!\n", XCDstType);)
        bResult = FALSE;
    }
    else
    {
        switch(XCDstType)
        {
            default:
            case E_GOP_XCDST_IP2:
                msg_sc_ip(printf("MApi_XC_GetDstInfo_IP():XCDstType(%x) is not supported temporarily!\n", XCDstType);)
                pDstInfo->bInterlaceMode = FALSE;
                pDstInfo->bYUVInput = FALSE;
                pDstInfo->HDTOT = 0;
                pDstInfo->VDTOT = 0;
                pDstInfo->DEHST = 0;
                pDstInfo->DEHEND= 0;
                pDstInfo->DEVST = 0;
                pDstInfo->DEVEND= 0;
                bResult = FALSE;
                break;

            case E_GOP_XCDST_IP1_MAIN:
                bResult = MApi_XC_GetDstInfo_IP1(pInstance, pDstInfo, u32SizeofDstInfo, MAIN_WINDOW);
                break;

            case E_GOP_XCDST_IP1_SUB:
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                bResult = MApi_XC_GetDstInfo_IP1(g_pDevice1Instance, pDstInfo, u32SizeofDstInfo, MAIN_WINDOW);
#else
                bResult = MApi_XC_GetDstInfo_IP1(pInstance, pDstInfo, u32SizeofDstInfo, SUB_WINDOW);
#endif

                break;

            case E_GOP_XCDST_OP1_MAIN:
                MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stXCMainDisWin);
                pDstInfo->bInterlaceMode = FALSE;
                pDstInfo->bYUVInput = FALSE;
                pDstInfo->HDTOT = 0;
                pDstInfo->VDTOT = 0;
                pDstInfo->DEHST = stXCMainDisWin.x;
                pDstInfo->DEHEND= stXCMainDisWin.width+stXCMainDisWin.x-1;
                pDstInfo->DEVST = stXCMainDisWin.y;
                pDstInfo->DEVEND= stXCMainDisWin.height+stXCMainDisWin.y-1;
                bResult = TRUE;
                break;
        }
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bResult;
}

MS_BOOL MApi_XC_GetDstInfo(MS_XC_DST_DispInfo *pDstInfo, MS_U32 u32SizeofDstInfo, EN_GOP_XCDST_TYPE XCDstType)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_DESTINATION_INFO XCArgs;
    XCArgs.pDstInfo = pDstInfo;
    XCArgs.u32SizeofDstInfo = u32SizeofDstInfo;
    XCArgs.XCDstType = XCDstType;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DESTINATION_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set IP1 Test Pattern
/// @param  eWindow          \b IN: IP1 Test Patter Enable
/// @param  eWindow          \b IN: IP1 Test Pattern Type
/// @param  eWindow          \b IN: which window(main or sub) to get
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetIP1TestPattern_U2(void* pInstance, MS_U8 u8Enable, MS_U16 u6Pattern_type, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_ip_Set_TestPattern(pInstance, (MS_U16)u8Enable, (MS_U32)u6Pattern_type, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetIP1TestPattern(MS_U8 u8Enable, MS_U16 u6Pattern_type, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_IP1_TESTPATTERN XCArgs;
    XCArgs.u8Enable = u8Enable;
    XCArgs.u6Pattern_type = u6Pattern_type;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IP1_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#ifdef UFO_XC_TEST_PATTERN
void MApi_XC_GenerateTestPattern_U2(void* pInstance,EN_XC_TEST_PATTERN_MODE ePatternMode,void* para, MS_U16 u16Length)
{
    _XC_ENTRY(pInstance);
   switch(ePatternMode)
   {
           case E_XC_ADC_PATTERN_MODE:
            {
                p_XC_SET_ADC_TESTPATTERN_t pXCArgs = (p_XC_SET_ADC_TESTPATTERN_t)para;
                if(u16Length != sizeof(XC_SET_ADC_TESTPATTERN_t))
                {
                    printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);
                    _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_adc_Set_TestPattern(pInstance,pXCArgs->u8EnableADCType,pXCArgs->u16Ramp);
                break;
            }
        case E_XC_IPMUX_PATTERN_MODE:
            {
                p_XC_SET_IPMUX_TESTPATTERN_t pXCArgs = (p_XC_SET_IPMUX_TESTPATTERN_t)para;
                if(u16Length != sizeof(XC_SET_IPMUX_TESTPATTERN_t))
                {
                    printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);
                    _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_ipmux_Set_TestPattern(pInstance,pXCArgs->bEnable,pXCArgs->u16R_CR_Data,pXCArgs->u16G_Y_Data,pXCArgs->u16B_CB_Data);
                break;
            }
        case E_XC_IP1_PATTERN_MODE:
            {
              p_XC_SET_IP1_TESTPATTERN_t pXCArgs = (p_XC_SET_IP1_TESTPATTERN_t)para;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            USE_OTHER_SC_AS_SUB_WINDOW();
#endif
              if(u16Length != sizeof(XC_SET_IP1_TESTPATTERN_t))

              {
                    printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);
                    _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_ip_Set_TestPattern(pInstance,pXCArgs->u16Enable, pXCArgs->u32Pattern_type, pXCArgs->eWindow);
                break;
            }
        case E_XC_OP_PATTERN_MODE:
            {
                p_XC_SET_OP_TESTPATTERN_t pXCArgs = (p_XC_SET_OP_TESTPATTERN_t)para;

              if(u16Length != sizeof(XC_SET_OP_TESTPATTERN_t))

              {
                    printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);
                    _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_op_Set_TestPattern(pInstance,pXCArgs->bMiuLineBuff, pXCArgs->bLineBuffHVSP);
                break;
            }
        case E_XC_VOP_PATTERN_MODE:
            {
                p_XC_SET_VOP_TESTPATTERN_t pXCArgs = (p_XC_SET_VOP_TESTPATTERN_t)para;

              if(u16Length != sizeof(XC_SET_VOP_TESTPATTERN_t))

              {
                    printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);
                    _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_vop_Set_TestPattern(pInstance,pXCArgs->bEnable);
                break;
            }
        case E_XC_VOP2_PATTERN_MODE:
            {
                p_XC_SET_VOP2_TESTPATTERN_t pXCArgs = (p_XC_SET_VOP2_TESTPATTERN_t)para;
                if(u16Length != sizeof(XC_SET_VOP2_TESTPATTERN_t))

              {
                    printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);

                  _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_vop2_Set_TestPattern(pInstance,pXCArgs->bEnable,pXCArgs->u16R_CR_Data,pXCArgs->u16G_Y_Data,pXCArgs->u16B_CB_Data);
                break;
            }
        case E_XC_MOD_PATTERN_MODE:
            {
                p_XC_SET_MOD_TESTPATTERN_t pXCArgs = (p_XC_SET_MOD_TESTPATTERN_t)para;
                if(u16Length != sizeof(XC_SET_MOD_TESTPATTERN_t))
                {
                      printf("[%s,%5d]the size of the struct passing to func isn't match the one in driver, please check if the ePatternMode is matching with para\n",__FUNCTION__,__LINE__);
                    _XC_RETURN(pInstance);
                    return;
                }
                Hal_SC_mod_Set_TestPattern(pInstance,pXCArgs->bEnable,pXCArgs->u16R_CR_Data,pXCArgs->u16G_Y_Data,pXCArgs->u16B_CB_Data);

          break;
            }
        default:
            {
                printf("[%s,%5d]unknown pattern type = %d\n",__FUNCTION__,__LINE__,ePatternMode);
                break;
            }
    }
    _XC_RETURN(pInstance);
}

void MApi_XC_GenerateTestPattern (EN_XC_TEST_PATTERN_MODE ePatternMode,void* para, MS_U16 u16Length)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_TESTPATTERN XCArgs;
    XCArgs.ePatternMode = ePatternMode;
    XCArgs.para = para;
    XCArgs.u16Length = u16Length;


    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_TESTPATTERN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}
#endif

void MApi_XC_InitRegInTimingChange_Post(void *pInstance, MS_U8 u8CurrentSyncStatus, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);

    //if( MP_IS_COMP() ) // just for component
    {
        // For Master 925FS timing 221 (1080i)

        if ( u8CurrentSyncStatus & (BIT(2)|BIT(3)|BIT(4)))// sync loss | MD_INTERLACE_BIT) )
            HAL_SC_ip_detect_mode(pInstance, ENABLE, eWindow);
        else
            HAL_SC_ip_detect_mode(pInstance, DISABLE, eWindow);
    }

    _XC_RETURN(pInstance);
}

MS_BOOL MApi_XC_Is_H_Sync_Active_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    if(Hal_SC_ip_get_h_v_sync_active(pInstance, eWindow) & 0x40)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_Is_H_Sync_Active(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_HSYNC_ACTIVE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_HSYNC_ACTIVE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MApi_XC_Is_V_Sync_Active_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    _XC_ENTRY(pInstance);
    if(Hal_SC_ip_get_h_v_sync_active(pInstance, eWindow) & 0x80)
    {
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_Is_V_Sync_Active(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_VSYNC_ACTIVE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_VSYNC_ACTIVE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_U8 MApi_XC_GetVSyncWidth_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U8 u8VsyncWidth = 0;
    _XC_ENTRY(pInstance);
    u8VsyncWidth = Hal_SC_getVSyncWidth(pInstance, eWindow);
    _XC_RETURN(pInstance);
    return u8VsyncWidth;
}

MS_U8 MApi_XC_GetVSyncWidth(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_VSYNC_WIDTH XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_VSYNC_WIDTH, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
      else
    {        return XCArgs.u8ReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_OutputFrameCtrl_U2(void* pInstance, MS_BOOL bEnable,XC_OUTPUTFRAME_Info *pstOutFrameInfo,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    if(!bEnable)
    {
        gSrcInfo[eWindow].bRWBankAuto = TRUE;
        Hal_SC_set_skp_fd(pInstance, FALSE, eWindow);
        // if support _FIELD_PACKING_MODE_SUPPORTED, it will control by QMAP(J2/A5/A6/A3)
        #if(!_FIELD_PACKING_MODE_SUPPORTED)
        Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
        #endif
    }
    else
    {
        //When bRWBankAuto = FALSE, wr_bank_mapping must be controled by QMap
        gSrcInfo[eWindow].bRWBankAuto = FALSE;

#ifdef UFO_XC_FB_LEVEL
        if(gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
#else
        if(gSrcInfo[eWindow].bFBL)
#endif
        {
            Hal_SC_set_skp_fd(pInstance, FALSE, eWindow);
        }
        else
        {
            if(!pstOutFrameInfo->bInterlace) //progress
            {
                Hal_SC_set_skp_fd(pInstance, FALSE, eWindow);
            }
            else    //interlace
            {
#if (HW_DESIGN_4K2K_VER == 7)
                Hal_SC_set_skp_fd(pInstance, TRUE, eWindow);
#else
                 if(pstOutFrameInfo->u16InVFreq > pstOutFrameInfo->u16OutVFreq)    //Drop frame case
                {
                    Hal_SC_set_skp_fd(pInstance, TRUE, eWindow);
                }
                else
                {
                    Hal_SC_set_skp_fd(pInstance, FALSE, eWindow);
                }
#endif
            }
        }
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_OutputFrameCtrl(MS_BOOL bEnable,XC_OUTPUTFRAME_Info *pstOutFrameInfo,SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OUTPUT_FRAME_CONTROL XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.pstOutFrameInfo = pstOutFrameInfo;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_FRAME_CONTROL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MApi_XC_SetIPMux_U2(void* pInstance, MS_U8 u8Val)
{
    _XC_ENTRY(pInstance);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    HAL_SC_ip_set_ipmux(pInstance, u8Val);

    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        XC_OPTEE_MUX_DATA st_xc_mux_data;
        memset(&st_xc_mux_data, 0, sizeof(XC_OPTEE_MUX_DATA));
        MDrv_XC_OPTEE_Mux(pInstance, E_XC_OPTEE_SET_MUX, st_xc_mux_data);
    }

    _XC_RETURN(pInstance);
}

void MApi_XC_SetIPMux(MS_U8 u8Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_IPMUX XCArgs;
    XCArgs.u8Val = u8Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IPMUX, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_SetVGASogEn(void *pInstance, MS_BOOL bVGASogEn)
{
    HAL_SC_SetVGASogEn(pInstance, bVGASogEn);
}

MS_BOOL MDrv_XC_GetVGASogEn(void *pInstance)
{
    return HAL_SC_GetVGASogEn(pInstance);
}

void MDrv_XC_SetSogDetectStatus(void *pInstance, MS_U8  u8SogDetectStatus)
{
    HAL_SC_SetSogDetectStatus(pInstance, u8SogDetectStatus);
}

MS_U8  MDrv_XC_GetSogDetectStatus(void *pInstance)
{
    return HAL_SC_GetSogDetectStatus(pInstance);
}

EN_FORCEi_SUPPORT_TYPE MDrv_XC_GetForceiSupportType(void *pInstance)
{
    return HAL_SC_ip_get_forceI_support_type(pInstance);
}

MS_U32 MDrv_SC_ip_get_verticalperiod(void *pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32AccurateVPeriod = 0, u32PrevAccurateVPeriod = 0;
    MS_U8 u8Count = 0, u8VSyncTime = MDrv_XC_PCMonitor_GetVSyncTime(pInstance, eWindow);

    if(u8VSyncTime == 0)
    {
        //fix MsOS_DelayTask(0) assert
        u8VSyncTime = 20;//delay 20ms(frame rate = 50hz)
    }

    while (u8Count < MAX_GET_VPERIOD_TIME)
    {
        u32AccurateVPeriod = HAL_SC_ip_get_verticalperiod(pInstance, eWindow);
        if (abs(u32AccurateVPeriod - u32PrevAccurateVPeriod) <= PERIOD_TOLARANCE)
        {
            break;
        }
        u32PrevAccurateVPeriod = u32AccurateVPeriod;
        MsOS_DelayTask(u8VSyncTime);
        u8Count++;
    }
    return u32AccurateVPeriod;
}

void MDrv_XC_SetHVSyncStatusCheck(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    return HAL_SC_ip_set_hv_sync_status_check(pInstance, bEnable, eWindow);
}

void MDrv_XC_SetNewModeInterlacedDetect(void *pInstance, MS_BOOL bEnable, MS_U8 u8Source, MS_BOOL bVttByPixelClk, MS_BOOL bVInvert, MS_BOOL bHInvert, SCALER_WIN eWindow)
{
    HAL_SC_ip_set_hv_sync_source_select(pInstance, u8Source, eWindow);
    HAL_SC_ip_set_vtotal_count_by_pixel_clock(pInstance, bVttByPixelClk, eWindow);
    HAL_SC_ip_set_vsync_invert(pInstance, bVInvert, eWindow);
    HAL_SC_ip_set_hsync_invert(pInstance, bHInvert, eWindow);
    HAL_SC_ip_set_new_mode_interlaced_detect(pInstance, bEnable, eWindow);
}

MS_BOOL MDrv_XC_PCMonitor_Init(void *pInstance, MS_U8 u8MaxWindowNum)
{
    MS_U8 i = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    for(i = 0; i < u8MaxWindowNum; i++)
    {
        pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[i].eCurrentSrc = INPUT_SOURCE_NONE;
    }
    return TRUE;
}

XC_PCMONITOR_HK_INFO MDrv_XC_PCMonitor_GetStatus(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow];
}

MS_BOOL MDrv_XC_PCMonitor_SetStatus(void *pInstance, XC_PCMONITOR_HK_INFO sPCMONITOR_HK_Info, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    memcpy(&pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow], &sPCMONITOR_HK_Info, sizeof(XC_PCMONITOR_HK_INFO));

    return TRUE;
}

MS_U8 MDrv_XC_PCMonitor_GetStableCounter(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return (IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc)?25:25);
}

MS_U8 MDrv_XC_PCMonitor_GetNoSyncCounter(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return (IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc)?40:40);
}

MS_U16 MDrv_XC_PCMonitor_Get_HFreqx10(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if((pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState == E_XC_PCMONITOR_UNSTABLE)
        || (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        return 0;
    }
    else
    {
        return MDrv_XC_CalculateHFreqx10(pInstance, pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod);
    }
}

MS_U32 MDrv_XC_PCMonitor_Get_HFreqx1K(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if((pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState == E_XC_PCMONITOR_UNSTABLE)
        || (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        return 0;
    }
    else
    {
        return MDrv_XC_CalculateHFreqx1K(pInstance, pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod);
    }
}

MS_U32 MDrv_XC_PCMonitor_Get_VFreqx1K(void* pInstance, SCALER_WIN eWindow)
{
    MS_U32 u32VFreq = 0, u32AccurateVFreq = 0, u32AccurateVPeriod = 0;
    MS_U64 u64XTALClk = 0;
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);

    if((stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_UNSTABLE) || (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        // Original return value is 1
        // Change to 1 to prevent 4K2K home screen (4K2K 30Hz) force SetPanelTiming with no signal cause core dump issue.
        return 1;
    }
    else
    {
        if(FALSE == (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFast_Get_VFreq))
        {
            u32VFreq = MDrv_XC_CalculateVFreqx1K(pInstance, MDrv_XC_PCMonitor_Get_HFreqx1K(pInstance, eWindow), stPCMonitorInfo.u16Vtotal);
            if(stPCMonitorInfo.u8SyncStatus& XC_MD_INTERLACE_BIT)
            {
                u32VFreq *= 2;
            }
            XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "u32VFreq = %tu\n", (ptrdiff_t)u32VFreq);

            // for more correct VPeriod
            u32AccurateVPeriod = MDrv_SC_ip_get_verticalperiod(pInstance, eWindow);

            XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "u32AccurateVPeriod = %tu\n", (ptrdiff_t)u32AccurateVPeriod);

            if(u32AccurateVPeriod != 0)
            {
                u64XTALClk = (MS_U64)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000 + (u32AccurateVPeriod/2);
                do_div(u64XTALClk,u32AccurateVPeriod);
                u32AccurateVFreq = (MS_U32)(u64XTALClk);
            }
            XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "u32AccurateVFreq = %tu\n", (ptrdiff_t)u32AccurateVFreq);

            XC_LOG_TRACE(XC_DBGLEVEL_MODEPARSE, "InputVFreq=%tu->%tu, Tolerance=%tu\n", (ptrdiff_t)u32VFreq, (ptrdiff_t)u32AccurateVFreq, (ptrdiff_t)ACCURATE_VFREQ_TOLERANCE * 100);
            if(abs(u32VFreq - u32AccurateVFreq) < ACCURATE_VFREQ_TOLERANCE * 100)
            {
                u32VFreq = u32AccurateVFreq; //In tolerance range, using new accurate vfreq
            }
        }
        else
        {
            u32VFreq = stPCMonitorInfo.u32FrameRatex1K;
        }
        return u32VFreq;
    }
}

MS_U16 MDrv_XC_PCMonitor_Get_Vtotal(void *pInstance, SCALER_WIN eWindow)
{
    XC_PCMONITOR_HK_INFO stPCMonitorInfo;
    stPCMonitorInfo = MDrv_XC_PCMonitor_GetStatus(pInstance, eWindow);
    if((stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_UNSTABLE) || (stPCMonitorInfo.eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        // Original return value is 1
        // Change to 1 to prevent 4K2K home screen (4K2K 30Hz) force SetPanelTiming with no signal cause core dump issue.
        return 1;
    }
    else
    {
        return stPCMonitorInfo.u16Vtotal;
    }
}

void MDrv_XC_PCMonitor_Restart(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState = E_XC_PCMONITOR_STABLE_NOSYNC;           // no sync detected yet

    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u32LastExecutedTime = MsOS_GetSystemTime();           // set to current time, because bInputTimingChange is TRUE, will execute timing monitor anyway
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].bInputTimingChange = TRUE;                              // force timing monitor to detect timing
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus = XC_MD_SYNC_LOSS;
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].ucInputTimingStableCounter = 0;
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].ucTimingChangeDebounce = 0;
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].bInputTimingStable = TRUE;
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].bDoModeSetting = FALSE;

    // analog
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod = 0;
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16VSyncTime = 0;
    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u32FrameRatex1K= 0;

    pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal = 0;
    gSrcInfo[eWindow].Status2.u8IP_PixelRep = INVALID_IP_PIXELREPTITION;
    // digital
    memset(&(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info), 0, sizeof(MS_WINDOW_TYPE));
}

void MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(void *pInstance, SCALER_WIN eWindow,MS_WINDOW_TYPE* msWin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if((pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState == E_XC_PCMONITOR_UNSTABLE)
        || (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentState == E_XC_PCMONITOR_STABLE_NOSYNC))
    {
        return;
    }
    else
    {
        msWin->x = pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.x;
        msWin->y = pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.y;
        msWin->height= pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.height;

        if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType))
        {
            if(MDrv_HDMI_CheckHDMI20_Setting(MDrv_XC_Mux_GetHDMIPort(pInstance, gSrcInfo[eWindow].enInputSourceType)))
            {
                msWin->width= (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width)*2;
                printf("HDMI is 4K2K 2P mode\n");
            }
            else
            {
                MS_U16 u16Vfrqx10 = 0;
                u16Vfrqx10 = MDrv_XC_CalculateVFreqx10(pInstance,
                                          MDrv_XC_CalculateHFreqx10(pInstance, pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod),
                                          pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal);

                if((pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width >= 0x77F)
                 &&(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.height >= 0x86F)
                 &&(u16Vfrqx10 >= 490))
                {
                    msWin->width= (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width)*2;
                    printf("HDMI is 420 1P mode\n");
                }
                else
                {
                    if(((MDrv_HDMI_avi_infoframe_info(_BYTE_1)& 0x60) == 0x20)&& (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width <= HDE_UPPERBOUND_720) && (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width > HDE_LOWERBOUND_720) )
                    {
                        msWin->width= MDrv_HDMI_GetHDE();
                    }else
                    {
                        msWin->width= pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width;
                    }
                }
            }
        }
        else
        {
                msWin->width= pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width;
        }
    }
}

MS_BOOL MDrv_XC_PCMonitor_SyncLoss(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_SYNC_LOSS)
        return TRUE;
    else
        return FALSE;
}

MS_BOOL MDrv_XC_PCMonitor_InvalidTimingDetect(void *pInstance, MS_BOOL bPollingOnly, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bResult = FALSE;
    XC_IP_SYNC_STATUS sXC_Sync_Status;
    memset(&sXC_Sync_Status, 0, sizeof(XC_IP_SYNC_STATUS));
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_XC_GetSyncStatus(pInstance, pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc, &sXC_Sync_Status, eWindow);

    if ( ( sXC_Sync_Status.u8SyncStatus & XC_MD_SYNC_LOSS ) ||  // no sync
        ( (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_VSYNC_POR_BIT) != (sXC_Sync_Status.u8SyncStatus & XC_MD_VSYNC_POR_BIT) ) ||  // sync polarity changed
        ( (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_HSYNC_POR_BIT) != (sXC_Sync_Status.u8SyncStatus & XC_MD_HSYNC_POR_BIT) ) ||
        ( (pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus & XC_MD_INTERLACE_BIT) != (sXC_Sync_Status.u8SyncStatus & XC_MD_INTERLACE_BIT) ) )
    {
        bResult = TRUE;

        PCMSG_TIMING(printf("oldSyncStatus=0x%x, new=0x%x\n",
                        pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus, sXC_Sync_Status.u8SyncStatus));
    }
    else // have signal
    {
        if ( abs( sXC_Sync_Status.u16Hperiod - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod ) > MD_HPERIOD_TORLANCE )
        {
            PCMSG_TIMING(printf("Hperiod=%x->%x, Tolerance %x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod, sXC_Sync_Status.u16Hperiod, (MS_U16)MD_HPERIOD_TORLANCE));
            bResult = TRUE;
        }
        else
        {
            //PCMSG(printf("HPD=%x\n", sXC_Sync_Status.u16Hperiod));
        }

        // HSync
        if ( bPollingOnly != TRUE )
            pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Hperiod = sXC_Sync_Status.u16Hperiod;

        if ( abs( sXC_Sync_Status.u16Vtotal - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal ) > MD_VTOTAL_TORLANCE )
        {
            PCMSG_TIMING(printf("VTT=%x->%x, Tolerance %x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal, sXC_Sync_Status.u16Vtotal,  (MS_U16)MD_VTOTAL_TORLANCE));
            bResult = TRUE;
        }
        else
        {
            //printf("Vtt %x\n", sXC_Sync_Status.u16Vtotal);
        }

        // VSync
        if ( bPollingOnly != TRUE )
            pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u16Vtotal = sXC_Sync_Status.u16Vtotal;
        // FrameRate
        if ( bPollingOnly != TRUE )
        {
            pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u32FrameRatex1K = MDrv_XC_PCMonitor_Get_FrameRatex1K(pInstance, eWindow);
        }
        if( (IsSrcTypeHDMI(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc) || IsSrcTypeDVI(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc))
          &&(MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_DE) )
        {
            MS_WINDOW_TYPE sWin;
            memset(&sWin, 0, sizeof(MS_WINDOW_TYPE));

            MDrv_XC_GetDEWindow(pInstance, &sWin, eWindow);

            if (abs( sWin.width - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width) > MD_HDE_TORLANCE)
            {
                PCMSG_TIMING(printf("HDE %x->%x, Tolerance=%x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.width, sWin.width, (MS_U16)MD_HDE_TORLANCE));
                bResult = TRUE;
            }

            if (abs(sWin.height - pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.height) > MD_VDE_TORLANCE)
            {
                PCMSG_TIMING(printf("VDE %x->%x, Tolerance=%x\n", pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info.height, sWin.height, (MS_U16)MD_VDE_TORLANCE));
                bResult = TRUE;
            }

            if ( bPollingOnly != TRUE )
                memcpy(&(pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].sDVI_HDMI_DE_Info), &sWin, sizeof(MS_WINDOW_TYPE));


#if(!SUPPORT_HDMI20)
        //if chip doesn't support HDMI YUV420, regard yuv420 as invalid timing
        if(pXCResourcePrivate->stHdmiExtendPacketReceive.bPKT_AVI_RECEIVE)
        {
            if((!MDrv_HDMI_CheckHDMI20_Setting_U2(pInstance, pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].eCurrentSrc))
                   &&((MDrv_HDMI_avi_infoframe_info(_BYTE_1)& 0x60)==0x60))
            {
                bResult = TRUE;
            }
        }
#endif

        }
    }

    if(bPollingOnly != TRUE )
    {
        pXCResourcePrivate->stdrvXC_IP.sPCMONITOR_status[eWindow].u8SyncStatus = sXC_Sync_Status.u8SyncStatus;
    }

    return bResult; // stable sync
}

void MDrv_XC_Set_FD_Mask_ByWin(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow)
{
    HAL_SC_ip_Set_FD_Mask_ByWin(pInstance, bEnable, eWindow);
#if (HW_DESIGN_4K2K_VER == 4)
    // for FD_mask can control FRCM write
    Hal_SC_set_frcm_to_FD_mask(pInstance, bEnable, eWindow);
#endif
}

#undef  _MDRV_SC_IP_C_

