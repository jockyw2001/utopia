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
// [mhal_sc.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MDRV_PIP_C

#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
#include "utopia.h"
#include "utopia_dapi.h"

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
#include "mhal_pip.h"
#include "mdrv_sc_3d.h"
#include "drv_sc_scaling.h"

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

void MDrv_XC_SetMWEStatus(void *pInstance, MS_BOOL bMWEStatus)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->bXCMWEStatus = bMWEStatus;
}

MS_BOOL MDrv_XC_GetMWEStatus(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    return pXCResourcePrivate->bXCMWEStatus;
}
//-------------------------------------------------------------------------------------------------
/// Control the output window(Main/Sub) on or off, no mutex protect
/// @param  bEnable                \b IN: bEnable =1, Turn on the window; bEnable =0, Turn off the window
/// @param  eWindow              \b IN: eWindow =0, for MAIN; eWindow =1, for SUB;
//-------------------------------------------------------------------------------------------------
void MDrv_XC_EnableWindow(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    /*
    ** Save setwindow info for str
    */
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    XC_REGS_SAVE_AREA *pXCResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pXCResourceStr);
    pXCResourceStr->bPIP_enable = TRUE;
#endif
#if (HW_DESIGN_4K2K_VER < 4)
#if(HW_DESIGN_4K2K_VER !=7)
    MS_WINDOW_TYPE stDispWin;
#endif
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (HW_DESIGN_4K2K_VER < 4)
#if(HW_DESIGN_4K2K_VER !=7)
    if((eWindow == SUB_WINDOW) && (bEnable == FALSE))
    {
        stDispWin.x = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
        stDispWin.width   = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1;
        stDispWin.y = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
        stDispWin.height   = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1;
        printf("(x,y,w.h)=(%d,%d,%d,%d)\n", stDispWin.x, stDispWin.y, stDispWin.width, stDispWin.height);
        if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        {
            Hal_SC_Sub_SetDisplayWindow_burst(pInstance, &stDispWin);

            //this part was originally inside Hal_SC_Sub_SetDisplayWindow_burst(), it is moved out because
            //if we let REG_SC_BK20_10_L[1] to enable with other register settings in the same menuload fire
            //it will produce a rectangular horizontal noise. so we move outside to let those register settings
            //to be set first then let REG_SC_BK20_10_L[1] on.

            //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, (TRUE<<1), BIT(1));
            //MDrv_XC_MLoad_Fire(pInstance, TRUE);
        }
        else
        {
            MDrv_XC_wait_output_vsync(pInstance, 2, 200, MAIN_WINDOW);
            Hal_SC_Sub_SetDisplayWindow(pInstance, &stDispWin);
        }
    }
#endif
#endif

#ifdef CONFIG_MSTAR_SRAMPD
        if (bEnable)
        {
            Hal_SC_Sub_SRAM_PowerDown_Control(pInstance,bEnable);
        }
#endif

    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        Hal_SC_enable_window_burst(pInstance, bEnable, eWindow);
    }
    else
    {
        MDrv_XC_wait_output_vsync(pInstance, 2, 200, eWindow);
        Hal_SC_enable_window(pInstance, bEnable, eWindow);
    }

#ifdef CONFIG_MSTAR_SRAMPD
    if (!bEnable)
    {
        Hal_SC_Sub_SRAM_PowerDown_Control(pInstance,bEnable);
    }
#endif

#if(HW_DESIGN_4K2K_VER !=7)
    if((!bEnable) && (eWindow == SUB_WINDOW))
    {
        MDrv_XC_init_fbn_win(pInstance, eWindow);
    }
#endif
}

//-------------------------------------------------------------------------------------------------
/// Control the output window(Main/Sub) on or off, for api with mutex protect
/// @param  bEnable                \b IN: bEnable =1, Turn on the window; bEnable =0, Turn off the window
/// @param  eWindow              \b IN: eWindow =0, for MAIN; eWindow =1, for SUB;
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableWindow_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_EnableWindow(pInstance, bEnable, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_EnableWindow(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_WINDOW_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_WINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Control the main window on or off
/// @param  bEnable                \b IN: bEnable =1, Turn on the window; bEnable =0, Turn off the window
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableMainWindow_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_EnableWindow(pInstance, bEnable, MAIN_WINDOW);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_EnableMainWindow(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_MAINWINDOW_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MAINWINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable sub window and control the window size
/// @param  *pstDispWin                \b IN: Window type
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableSubWindow_U2(void* pInstance, MS_WINDOW_TYPE *pstDispWin)
{
#ifndef DISABLE_PIP_FUNCTION
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_WINDOW_TYPE stDispWin;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    _XC_ENTRY(pInstance);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if((MDrv_XC_GetMWEStatus(pInstance) && IsVMirrorMode(SUB_WINDOW)) || (!MDrv_XC_GetMWEStatus(pInstance) && IsVMirrorMode(MAIN_WINDOW)))
#else
    if(IsVMirrorMode(SUB_WINDOW))
#endif
    {
        if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > ( pstDispWin->x + pstDispWin->width ) )
        {
            pstDispWin->x = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - ( pstDispWin->x + pstDispWin->width );
        }
        else
        {
            pstDispWin->x = 0;
        }

        if( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > ( pstDispWin->y + pstDispWin->height ) )
        {
            pstDispWin->y = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - ( pstDispWin->y + pstDispWin->height );
        }
        else
        {
            pstDispWin->y = 0;
        }
    }

    stDispWin.x = pstDispWin->x;
    stDispWin.y = pstDispWin->y;
    stDispWin.width = pstDispWin->width;
    stDispWin.height = pstDispWin->height;

    if(MDrv_XC_Is2K2KToFrc(pInstance))
    {
        stDispWin.x = stDispWin.x/2;
        stDispWin.width = stDispWin.width/2;
    }


    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "%s %d: Disp x=%u,y=%u,width=%u,height=%u\n", __FUNCTION__, __LINE__, stDispWin.x,stDispWin.y,stDispWin.width,stDispWin.height);

    stDispWin.x += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
    stDispWin.y += pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;

    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        Hal_SC_Sub_SetDisplayWindow_burst(pInstance, &stDispWin);

        //this part was originally inside Hal_SC_Sub_SetDisplayWindow_burst(), it is moved out because
        //if we let REG_SC_BK20_10_L[1] to enable with other register settings in the same menuload fire
        //it will produce a rectangular horizontal noise. so we move outside to let those register settings
        //to be set first then let REG_SC_BK20_10_L[1] on.

        //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK20_10_L, (TRUE<<1), BIT(1));
        //MDrv_XC_MLoad_Fire(pInstance, TRUE);
    }
    else
    {
        MDrv_XC_wait_output_vsync(pInstance, 2, 200, MAIN_WINDOW);
        Hal_SC_Sub_SetDisplayWindow(pInstance, &stDispWin);
    }
    MDrv_XC_EnableCLK_for_SUB(pInstance,TRUE);
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
    MDrv_XC_EnableWindow(pInstance, TRUE,SUB_WINDOW);
#else
    if(MDrv_XC_GetMWEStatus(pInstance))
    {
        MDrv_XC_EnableWindow(pInstance, TRUE,SUB_WINDOW);
    }
    else
    {
        MDrv_XC_EnableWindow(pInstance, TRUE,MAIN_WINDOW);
    }
#endif
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    UNUSED(pstDispWin);
#endif
}

void MApi_XC_EnableSubWindow(MS_WINDOW_TYPE *pstDispWin)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SUBWINDOW_ENABLE XCArgs;
    XCArgs.pstDispWin = pstDispWin;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SUBWINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Disable sub window
/// @param  void                \b IN: none
//-------------------------------------------------------------------------------------------------
void MApi_XC_DisableSubWindow_U2(void* pInstance)
{
#ifndef DISABLE_PIP_FUNCTION
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
    MDrv_XC_EnableWindow(pInstance, FALSE, SUB_WINDOW);
#else
    if(!MDrv_XC_GetMWEStatus(pInstance))
    {
        MDrv_XC_EnableWindow(pInstance, FALSE, MAIN_WINDOW);
    }
    else
    {
        MDrv_XC_EnableWindow(pInstance, FALSE, SUB_WINDOW);
    }
#endif
    // For 3D case: When Sub Window is disabled, the CLK for sub also need to turn off
    MDrv_XC_EnableCLK_for_SUB(pInstance, FALSE);

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
}

void MApi_XC_DisableSubWindow(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SUBWINDOW_DISABLE, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Is sub window enable?
/// @return @ref MS_BOOL
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Is_SubWindowEanble_U2(void* pInstance)
{
    MS_BOOL bRet = FALSE;
#ifndef DISABLE_PIP_FUNCTION
    _XC_ENTRY(pInstance);
     bRet = MDrv_XC_Is_SubWindowEanble(pInstance);
    _XC_RETURN(pInstance);
#endif
    return bRet;
}

MS_BOOL MApi_XC_Is_SubWindowEanble(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_SUBWINDOW_ENABLE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_SUBWINDOW_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set Border format
/// @param  u8WidthIn                \b IN: In-box width
/// @param  u8WidthOut               \b IN: Out-box width
/// @param  u8color                  \b IN: border color
/// @param  eWindow                  \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetBorderFormat_U2( void* pInstance, MS_U8 u8Left, MS_U8 u8Right, MS_U8 u8Up, MS_U8 u8Down, MS_U8 u8color, SCALER_WIN eWindow )
{
#ifndef DISABLE_PIP_FUNCTION

    _XC_ENTRY(pInstance);
    Hal_SC_set_border_format(pInstance, u8Left, u8Right, u8Up, u8Down, u8color, eWindow );
    _XC_RETURN(pInstance);
#else
    UNUSED(u8Left);
    UNUSED(u8Right);
    UNUSED(u8Up);
    UNUSED(u8Down);
    UNUSED(u8color);
    UNUSED(eWindow);
#endif
}

void MApi_XC_SetBorderFormat( MS_U8 u8Left, MS_U8 u8Right, MS_U8 u8Up, MS_U8 u8Down, MS_U8 u8color, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BORDER_FORMAT XCArgs;
    XCArgs.u8Left = u8Left;
    XCArgs.u8Right = u8Right;
    XCArgs.u8Up = u8Up;
    XCArgs.u8Down = u8Down;
    XCArgs.u8color = u8color;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BORDER_FORMAT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// enable or disable Border
/// @param  bEnable                  \b IN: Enable or Disable
/// @param  eWindow                  \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableBorder_U2( void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
#ifndef DISABLE_PIP_FUNCTION

    _XC_ENTRY(pInstance);
    Hal_SC_border_enable(pInstance, bEnable, eWindow);
    _XC_RETURN(pInstance);
#else
    UNUSED(bEnable);
    UNUSED(eWindow);
#endif
}

void MApi_XC_EnableBorder( MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BORDER_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BORDER_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set Z-order main window first or not
/// @param  bMainFirst               \b IN: TRUE: Main window first; FALSE: Sub window first
//-------------------------------------------------------------------------------------------------
void MApi_XC_ZorderMainWindowFirst_U2(void* pInstance, MS_BOOL bMainFirst)
{
#ifndef DISABLE_PIP_FUNCTION

    MS_XC_DST_DispInfo stBottomWin;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    // Get bottom window info.
    MApi_XC_GetDstInfo_IP1(pInstance, &stBottomWin, sizeof(MS_XC_DST_DispInfo), bMainFirst ? SUB_WINDOW : MAIN_WINDOW);

    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        Hal_SC_set_Zorder_main_first_burst(pInstance, bMainFirst);
        Hal_SC_set_Zextra_y_half_burst(pInstance, stBottomWin.bInterlaceMode);
        //printf("shawn (1) MApi_XC_ZorderMainWindowFirst(bMainTop(%d)) \r\n", bMainFirst);
    }
    else
    {
        Hal_SC_set_Zorder_main_first(pInstance, bMainFirst);
        Hal_SC_set_Zextra_y_half(pInstance, stBottomWin.bInterlaceMode);
        //printf("shawn (2) MApi_XC_ZorderMainWindowFirst(bMainTop(%d)) \r\n", bMainFirst);
    }

    //Fill the line buffer in vertical blanking for the window on top
    if(bMainFirst)
    {
        MDrv_XC_FilLineBuffer(pInstance, ENABLE, MAIN_WINDOW);
        MDrv_XC_FilLineBuffer(pInstance, DISABLE, SUB_WINDOW);
    }
    else
    {
        MDrv_XC_FilLineBuffer(pInstance, DISABLE, MAIN_WINDOW);
        MDrv_XC_FilLineBuffer(pInstance, ENABLE, SUB_WINDOW);
    }

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#else
    UNUSED(bMainFirst);
#endif
}

void MApi_XC_ZorderMainWindowFirst(MS_BOOL bMainFirst)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_ZORDER_MAINWINDOW_FIRST XCArgs;
    XCArgs.bMainFirst = bMainFirst;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_ZORDER_MAINWINDOW_FIRST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// enable or disable Subwindow FRC
/// @param  bEnable                  \b IN: Enable or Disable FRC.
/// @param  u8Threashold             \b IN: 0~7: ignore frames number before buffer being Read. larger
///                                              number means ingnore more. 0~7:1, 2, 4, 8, 16, 32, 64, 128
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPIP_FRC(void *pInstance, MS_BOOL enable, MS_U8 u8IgonreLinesNum)
{
#ifndef DISABLE_PIP_FUNCTION

    _XC_ENTRY(pInstance);

    Hal_SC_SetPIP_FRC(pInstance, enable, u8IgonreLinesNum);

    _XC_RETURN(pInstance);
#else
    UNUSED(enable);
    UNUSED(u8IgonreLinesNum);
#endif
}

void MDrv_XC_EnableCLK_for_SUB(void *pInstance, MS_BOOL bEnable)
{
#ifndef DISABLE_PIP_FUNCTION

#if (HW_DESIGN_4K2K_VER == 7)
    //Start: It needs to avoid to set SC1 sub win setting, because XC1 does not sub win
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(psXCInstPri->u32DeviceID == E_XC_DEVICE0)
    {
#endif

    // Enable sub window clock
    if(bEnable)
    {
        // Enable FIClk1
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, DISABLE, CKG_FICLK_F1_INVERT);               // Not Invert
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, DISABLE, CKG_FICLK_F1_GATED);                // Enable clock

        // Enable IDClk1
        MDrv_WriteRegBit(REG_CKG_IDCLK1, DISABLE, CKG_IDCLK1_INVERT);                   // Not Invert
        MDrv_WriteRegBit(REG_CKG_IDCLK1, DISABLE, CKG_IDCLK1_GATED);                    // Enable clock

#ifdef K3_U2
        // Enable SIDClk1
        MDrv_WriteRegBit(REG_CKG_SIDCLK1, DISABLE, CKG_SIDCLK1_INVERT);
        MDrv_WriteRegBit(REG_CKG_SIDCLK1, DISABLE, CKG_SIDCLK1_GATED);
#endif
    }
    else
    {
        // Disable FIClk1
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, CKG_FICLK_F1_INVERT);               // Not Invert
        MDrv_WriteRegBit(REG_CKG_FICLK_F1, ENABLE, CKG_FICLK_F1_GATED);                // Enable clock

        // Disable IDClk1
        MDrv_WriteRegBit(REG_CKG_IDCLK1, ENABLE, CKG_IDCLK1_INVERT);                   // Not Invert
        MDrv_WriteRegBit(REG_CKG_IDCLK1, ENABLE, CKG_IDCLK1_GATED);                    // Enable clock

#ifdef K3_U2
        // Disable SIDClk1
        MDrv_WriteRegBit(REG_CKG_SIDCLK1, ENABLE, CKG_SIDCLK1_INVERT);
        MDrv_WriteRegBit(REG_CKG_SIDCLK1, ENABLE, CKG_SIDCLK1_GATED);
#endif
    }

#if (HW_DESIGN_4K2K_VER == 7)
    //End: It needs to avoid to set SC1 sub win setting, because XC1 does not sub win
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "%s %d: MDrv_XC_EnableCLK_for_SUB: Device ID %u not supporting sub window\n", __FUNCTION__, __LINE__, psXCInstPri->u32DeviceID);
    }
#endif

#ifdef CONFIG_MSTAR_SRAMPD
    Hal_SC_EnableCLK_for_SUB(pInstance, bEnable);
#endif

#else
    UNUSED(bEnable);
#endif
}

void MDrv_XC_Enable_Extra_Request(void *pInstance, MS_BOOL bEnable)
{
    // HW PIP architeucture
    // Becasue BK3_02[7] and BK20_11[15] can not enable toghter, otherwise garbage will be showed,
    // With HW auto no signal, BK3_02[7] will be enabled by HW
    // so, we need to set BK20_11[15] to 0 before no signal, ex. source switch case, need to disable extra request engine
    // Then we need to restore extra request engine after input source is stable
#ifndef DISABLE_PIP_FUNCTION
    Hal_SC_enable_extra_request(pInstance, bEnable);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set Line Buffer merge address (This value is depend on line buffer size of main window)
//-------------------------------------------------------------------------------------------------
void MDrv_SC_Set_LB_MergeAddress(void *pInstance)
{
    HAL_SC_Set_LB_MergeAddress(pInstance);
}

#undef  MDRV_PIP_C
