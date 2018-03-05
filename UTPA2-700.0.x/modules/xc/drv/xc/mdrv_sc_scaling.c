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
#define DRV_SC_SCALING_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "UFO.h"
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
#include "utopia_dapi.h"

//#include "Debug.h"
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
#include "drv_sc_mux.h"
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
#include "mhal_ip.h"
#include "mhal_hdmi.h"

#include "mhal_pip.h"
#include "halCHIP.h"
#include "drv_sc_scaling.h"
#include "xc_Analog_Reg.h"
#include "xc_hwreg_utility2.h"
#include "mdrv_sc_dynamicscaling.h"
#include "mhal_dynamicscaling.h"
#include "hwreg_pm_sleep.h"
#if (defined (ANDROID))
#include "drvPQ.h"
#endif
//////////////////////////Remove when drvpq change us Mscommon.h /////////////
#define E_XRULE_HSD         0
#define E_XRULE_VSD         1
#define E_XRULE_HSP         2
#define E_XRULE_VSP         3
#define E_XRULE_CSC         4
#define E_XRULE_NUM         5

#define E_GRULE_NR          0
#define E_GRULE_NUM         1

#define RFBL_Y8M4_Pixel_Bits    12

#define PIP_SUPPORTED (MAX_WINDOW_NUM > 1)

#define WIDTH_4K2K     (3840)
#define WIDTH_4K2K_OFFSET   (10)

#define DUAL_DUMMYDATA_SIZE (4)
#define DUAL_GARDBAND_SIZE (32)

#ifndef SUPPORT_DUAL_MIU_MIRROR_SWAP_IPM
#define SUPPORT_DUAL_MIU_MIRROR_SWAP_IPM FALSE
#endif

typedef enum
{
   E_DRVSC_FBNUM  = 0,
}DRV_SC_SRCINFO ;

extern PQ_Function_Info  s_PQ_Function_Info[MAX_XC_DEVICE_NUM];

/******************************************************************************/
/*                     Local                                                  */
/* ****************************************************************************/

#define MS_IPM_BASE0(win)     (pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[(win)] / BYTE_PER_WORD )
#define MS_IPM_BASE1(win)     ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[(win)] + (pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[(win)]/2)) / BYTE_PER_WORD)
#define MS_FRCM_BASE0(win)    (pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[(win)] / BYTE_PER_WORD )
#define MS_FRCM_BASE1(win)    ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[(win)] + (pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[(win)]/2)) / BYTE_PER_WORD)

#define MS_IPM_DUAL_MIU_BASE0(win)     (pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[(win)] / BYTE_PER_WORD )
#define MS_IPM_DUAL_MIU_BASE1(win)     ((pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[(win)] + (pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[(win)]/2)) / BYTE_PER_WORD)


#define VFREQDELTA            10
#define MAX_1P_MODE_CLK       320000000   //(300*1024*1024)   // max 1p mode clk value
#define MVOP_H_BLANKING       300
#define MVOP_V_BLANKING       45

#ifdef UFO_XC_SUPPORT_2STEP_SCALING
MS_BOOL bIsSupport2StepScaling;
#endif
/******************************************************************************/
/*                   Functions                                                */
/******************************************************************************/

extern MS_BOOL MDrv_VD_IsSyncLocked(void);
extern void MDrv_PQDS_Update_PanelID(void *, MS_U16);

MS_BOOL MDrv_XC_WBankReset(void *pInstance, MS_BOOL bIsFRCM, SCALER_WIN eWindow)
{
#if _FIELD_PACKING_MODE_SUPPORTED //new SCMI mech
    MS_U32 u32Reg = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (bIsFRCM)
    {
        if(eWindow ==MAIN_WINDOW )
        {
            u32Reg = REG_SC_BK32_05_L;
        }
        else
        {
            u32Reg = REG_SC_BK32_45_L;
        }
    }
    else
    {
        if(eWindow ==MAIN_WINDOW )
        {
            u32Reg = REG_SC_BK12_05_L;
        }
        else
        {
            u32Reg = REG_SC_BK12_45_L;
        }
    }

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,  u32Reg, BIT(7), BIT(7) );
        MsOS_DelayTask(1);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID,  u32Reg, 0, BIT(7) );
        MsOS_DelayTask(1);

#endif
    return TRUE;
}

void MDrv_SC_set_write_limit(void *pInstance, MS_PHY u32WritelimitAddrBase, SCALER_WIN eWindow)
{
    Hal_SC_set_write_limit(pInstance, u32WritelimitAddrBase, eWindow);

}
void MDrv_SC_set_dual_write_limit(void *pInstance, MS_PHY u32WritelimitAddrBase, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    Hal_SC_set_dual_write_limit(pInstance, u32WritelimitAddrBase, eWindow);
#endif
}

MS_BOOL MDrv_SC_CheckMuteStatusByRegister(void *pInstance, SCALER_WIN eWindow)
{
    return Hal_SC_CheckMuteStatusByRegister(pInstance, eWindow);
}

void MDrv_SC_SetPQHSDFlag(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling._bPQSetHPreScalingratio = bEnable;
}

MS_BOOL MDrv_SC_GetPQHSDFlag(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Scaling._bPQSetHPreScalingratio;
}

#if (ENABLE_OPM_WRITE_SUPPORTED)
void _MDrv_SC_set_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow)
{
    // Only support address of Main_Window
    if (eWindow != MAIN_WINDOW )
    {
        return;
    }
    u32OPWlimitAddr |= (bEnable?F2_OPW_WRITE_LIMIT_EN:0) | (bFlag?F2_OPW_WRITE_LIMIT_MIN:0);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_5E_L, u32OPWlimitAddr);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_38_L, (bEnable?BIT(2):0),BIT(2)); //OPW memory address limit enable, refer to F2 memory limit from BK0D_40/BK0D_42
}
void _MDrv_SC_set_dual_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow)
{
    // Only support address of Main_Window
    if (eWindow != MAIN_WINDOW )
    {
        return;
    }
    u32OPWlimitAddr |= (bEnable?F2_OPW_WRITE_LIMIT_EN:0) | (bFlag?F2_OPW_WRITE_LIMIT_MIN:0);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //OPM write limit share the same min/max baseAddress setting with IPM
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0D_38_L, (bEnable?BIT(6):0),BIT(6)); //OPW memory address limit enable, refer to F2 memory limit from BK0D_48/BK0D_4d
}

#endif

void MDrv_SC_set_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow)
{
#if (ENABLE_OPM_WRITE_SUPPORTED)
    _MDrv_SC_set_opm_write_limit(pInstance, bEnable, bFlag, u32OPWlimitAddr, eWindow);
    //Note: Hal_SC_set_opm_write_limit() is deprecated. use _MDrv_SC_set_opm_write_limit() instead.
#endif //(ENABLE_OPM_WRITE_SUPPORTED)
}

void MDrv_SC_set_dual_opm_write_limit(void *pInstance, MS_BOOL bEnable, MS_BOOL bFlag, MS_PHY u32OPWlimitAddr, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
#if (ENABLE_OPM_WRITE_SUPPORTED)
    _MDrv_SC_set_dual_opm_write_limit(pInstance, bEnable, bFlag, u32OPWlimitAddr, eWindow);
    //Note: Hal_SC_set_opm_write_limit() is deprecated. use _MDrv_SC_set_opm_write_limit() instead.
#endif //(ENABLE_OPM_WRITE_SUPPORTED)
#endif
}

void MDrv_SC_set_frcm_write_limit(void *pInstance, MS_BOOL bEnable, MS_PHY u32FRCMLimitBase, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    if(eWindow == MAIN_WINDOW)
        u32FRCMLimitBase |= (bEnable?F2_FRCM_WRITE_LIMIT_EN:0);
    else if(eWindow == SUB_WINDOW)
        u32FRCMLimitBase |= (bEnable?F1_FRCM_WRITE_LIMIT_EN:0);


    Hal_SC_set_frcm_write_limit(pInstance, u32FRCMLimitBase, eWindow);
#endif
}

//-----------------------------------------------------------------------------------------------------------
/// When the muteType is enabled, the vsync will be skipped and the registers will be written directly
/// @param enMuteType              \b IN: the mute type could be MainWindow, SubWindow and Panel
/// @param bIsForceWrite      \b IN: Force write or not
//-----------------------------------------------------------------------------------------------------------
void MDrv_SC_SetForceWrite( EN_MUTE_TYPE enMuteType, MS_BOOL bForceWrite)
{
   XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
   UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
   if(enMuteType == E_MAIN_WINDOW_MUTE)
   {
       pXCResourcePrivate->stdrvXC_Scaling._bMainWinSkipWaitOutputVsync = bForceWrite;
   }
   else if(enMuteType == E_SUB_WINDOW_MUTE)
   {
       pXCResourcePrivate->stdrvXC_Scaling._bSubWinSkipWaitOutputVsync = bForceWrite;
   }
   else if(enMuteType == E_PANEL_MUTE)
   {
        pXCResourcePrivate->stdrvXC_Scaling._bPanelSkipWaitOutputVsync = bForceWrite;
   }
   else
   {
        MS_ASSERT(0);
   }
}


//-------------------------------------------------------------------------------------------------
// Skip Wait Vsync
/// @param eWindow               \b IN: Enable
/// @param Skip wait Vsync      \b IN: Disable wait Vsync
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetSkipWaitVsync(void *pInstance, MS_BOOL eWindow,MS_BOOL bIsSkipWaitVsyn)
{
    EN_MUTE_TYPE enMuteType = E_MAIN_WINDOW_MUTE;
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
    MDrv_SC_SetForceWrite(enMuteType, bIsSkipWaitVsyn);
}

MS_BOOL MDrv_SC_GetForceWrite( EN_MUTE_TYPE enMuteType)
{
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));
    if(enMuteType == E_MAIN_WINDOW_MUTE)
    {
        return  pXCResourcePrivate->stdrvXC_Scaling._bMainWinSkipWaitOutputVsync;
    }
    else if(enMuteType == E_SUB_WINDOW_MUTE)
    {
        return pXCResourcePrivate->stdrvXC_Scaling._bSubWinSkipWaitOutputVsync;
    }
    else if(enMuteType == E_PANEL_MUTE)
   {
        return pXCResourcePrivate->stdrvXC_Scaling._bPanelSkipWaitOutputVsync;
   }
   else
   {
        MS_ASSERT(0);
        return FALSE;
   }
}

MS_BOOL MDrv_SC_GetSkipWaitVsync(void *pInstance, MS_BOOL bScalerWin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(bScalerWin == MAIN_WINDOW)
    {
        return  pXCResourcePrivate->stdrvXC_Scaling._bMainWinSkipWaitOutputVsync;
    }
    else
    {
        return pXCResourcePrivate->stdrvXC_Scaling._bSubWinSkipWaitOutputVsync;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set XC CMA Heap ID
/// @param  u8CMAHeapID            \b IN: CMA Heap ID
/// @param  eWindow                 \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetCMAHeapID(void *pInstance, MS_U8 u8CMAHeapID,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].u8CMAHeapID = u8CMAHeapID;
}

void MDrv_SC_sw_db(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    // Set ficlk -> sw_db  for prevent garbage when no Vpre-scaling down
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if ( !gSrcInfo[eWindow].bPreV_ScalingDown )
    {
        Hal_SC_set_ficlk(pInstance, FALSE, eWindow );
    }

    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        && (!MDrv_SC_CheckMuteStatusByRegister(pInstance, eWindow)))
    {
        Hal_SC_sw_db_burst(pInstance, pDBreg, eWindow );
    }
    else
    {
        if((!MDrv_SC_CheckMuteStatusByRegister(pInstance, eWindow))&&(!MDrv_SC_GetSkipWaitVsync(pInstance, eWindow)))
        {
            MDrv_XC_wait_output_vsync(pInstance, 2, 200, eWindow);
        }
        Hal_SC_sw_db(pInstance, pDBreg, eWindow );
    }

    // Set sw_db -> ficlk for prevent garbage when Vpre-scaling down
    if ( gSrcInfo[eWindow].bPreV_ScalingDown )
    {
        Hal_SC_set_ficlk(pInstance, TRUE, eWindow );
    }

}

void MDrv_SC_dual_sw_db(void *pInstance, P_SC_SWDB_INFO pMainDBreg, P_SC_SWDB_INFO pSubDBreg)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    // Set ficlk -> sw_db  for prevent garbage when no Vpre-scaling down
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if ( !gSrcInfo[MAIN_WINDOW].bPreV_ScalingDown )
    {
        Hal_SC_set_ficlk(pInstance, FALSE, MAIN_WINDOW );
    }
    if ( !gSrcInfo[SUB_WINDOW].bPreV_ScalingDown )
    {
        Hal_SC_set_ficlk(pInstance, FALSE, SUB_WINDOW );
    }

    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_MAIN_WINDOW_MUTE)) && (!MDrv_SC_GetForceWrite(E_SUB_WINDOW_MUTE)) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
    {
        Hal_SC_dual_sw_db_burst(pInstance, pMainDBreg, pSubDBreg );
    }
    else
    {
        if((!MDrv_SC_GetForceWrite(E_MAIN_WINDOW_MUTE)) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)))
        {
            MDrv_XC_wait_output_vsync(pInstance, 2, 200, MAIN_WINDOW);
        }
        Hal_SC_sw_db(pInstance, pMainDBreg, MAIN_WINDOW );

        if((!MDrv_SC_GetForceWrite(E_SUB_WINDOW_MUTE)) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)))
        {
            MDrv_XC_wait_output_vsync(pInstance, 2, 200, SUB_WINDOW);
        }
        Hal_SC_sw_db(pInstance, pMainDBreg, SUB_WINDOW );
    }

    // Set sw_db -> ficlk for prevent garbage when Vpre-scaling down
    if ( gSrcInfo[MAIN_WINDOW].bPreV_ScalingDown )
    {
        Hal_SC_set_ficlk(pInstance, TRUE, MAIN_WINDOW );
    }
    if ( gSrcInfo[SUB_WINDOW].bPreV_ScalingDown )
    {
        Hal_SC_set_ficlk(pInstance, TRUE, SUB_WINDOW );
    }

}


void MApi_SWDS_Fire_U2(void *pInstance, SCALER_WIN eWindow )
{
    MS_U32 ActiveSWClearEn = 0;
    MS_U32 u32SW_Reset_Enable = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if ((HW_DESIGN_4K2K_VER == 4)||(HW_DESIGN_4K2K_VER == 6)||(HW_DESIGN_4K2K_VER == 7))
    _DS_ENTRY(pInstance);
    if(eWindow == MAIN_WINDOW)
    {
        if(pXCResourcePrivate->sthal_SC.bDynamicScalingEnable)
        {
            pXCResourcePrivate->sthal_SC.u8DynamicScalingNextIndex[MAIN_WINDOW] = pXCResourcePrivate->sthal_SC.u8DynamicScalingCurrentIndex_Ring[MAIN_WINDOW];
        }
    }
    PSTXC_DS_CMDCNT.u16CMDCNT_IPM =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_IPS =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_OPM =0;
    PSTXC_DS_CMDCNT.u16CMDCNT_OPS =0;
    //ActiveSWClearEn enable in first DS IP command ;
    u32SW_Reset_Enable = 0x4000; // bit14: reg_ds_active_sw_clr_en,set ds_activating cleared by SW
    ActiveSWClearEn = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK1F_17_L, 0xFFFF);
    ActiveSWClearEn |= u32SW_Reset_Enable;

    Hal_SC_WriteSWDSCommand(pInstance,MAIN_WINDOW,REG_SC_BK1F_17_L,(MS_U16)(ActiveSWClearEn),DS_IP,DS_XC,&PSTXC_DS_CMDCNT);

    Hal_SC_Add_NullCommand(pInstance, MAIN_WINDOW, DS_IP, &PSTXC_DS_CMDCNT);
    if(!((gSrcInfo[MAIN_WINDOW].bR_FBL) || (gSrcInfo[MAIN_WINDOW].bFBL)))
    {
        Hal_SC_Add_NullCommand(pInstance, MAIN_WINDOW, DS_OP, &PSTXC_DS_CMDCNT);
    }
    MDrv_SWDS_Fire(pInstance,eWindow);
    _DS_RETURN(pInstance);
#endif
}

void MApi_SWDS_Fire(SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SWDS_FIRE XCArgs;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SWDS_FIRE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#if ((HW_DESIGN_4K2K_VER == 4)||(HW_DESIGN_4K2K_VER == 6)||(HW_DESIGN_4K2K_VER == 7))
#if ((defined (ANDROID) || defined(MSOS_TYPE_LINUX_KERNEL)) && defined(UFO_XC_DS_PQ))
extern void _MDrv_PQ_Load_DS_Table(void* pInstance,PQ_WIN eWindow,MS_U32 V_Size, MS_U32 H_Size);
#endif
void MDrv_SWDS_AddCmd(void *pInstance, P_SC_SWDB_INFO pDBreg, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if ((defined (ANDROID) || defined(MSOS_TYPE_LINUX_KERNEL)) && defined(UFO_XC_DS_PQ))
    /// To prevent Mutex lock for XC >> PQ >> XC
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    MDrv_PQDS_Update_PanelID(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width);
    _MDrv_PQ_Load_DS_Table(pInstance,eWindow,pDBreg->u16V_CapSize,pDBreg->u16H_CapSize);

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
#endif


    Hal_SC_SWDS_AddCmd(pInstance,pDBreg, eWindow);
}

void MDrv_SWDS_Fire(void *pInstance, SCALER_WIN eWindow )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (SUPPORT_KERNEL_DS == 1)
    MDrv_XC_FireSWDSIndex(pInstance, eWindow);
#else
    Hal_SC_SWDS_Fire(pInstance, eWindow);
#endif
}

#endif

void MDrv_XC_VIP_Peaking_Setting(void *pInstance, SCALER_WIN eWindow)
{
    Hal_SC_VIP_Peaking_Setting(pInstance, eWindow);
}

void MDrv_XC_DTVPatch(void *pInstance, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    MHal_XC_DTVPatch(pInstance,eWindow);
#endif
}

void MDrv_XC_init_fbn_win(void *pInstance, SCALER_WIN eWindow)
{
    SC_SWDB_INFO SWDBreg;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    memset(&SWDBreg, 0, sizeof(SC_SWDB_INFO));
    SWDBreg.u32H_PreScalingRatio = 0x0;
    SWDBreg.u32V_PreScalingRatio = 0x0;
    SWDBreg.u32H_PostScalingRatio = 0x0;
    SWDBreg.u32V_PostScalingRatio = 0x0;
    SWDBreg.u16VLen = 0x240;
#if (HW_DESIGN_4K2K_VER == 4)
    SWDBreg.u16FRCMVLen = 0x240;
#endif
    if(eWindow == MAIN_WINDOW)
    {
        SWDBreg.u16VWritelimit = 0x240 | F2_V_WRITE_LIMIT_EN;
#if (HW_DESIGN_4K2K_VER == 4)
        SWDBreg.u16VFRCMWritelimit = 0x240 | F2_V_WRITE_LIMIT_EN;
#endif
    }
    else
    {
        SWDBreg.u16VWritelimit = 0x240 | F1_V_WRITE_LIMIT_EN;
#if (HW_DESIGN_4K2K_VER == 4)
        SWDBreg.u16VFRCMWritelimit = 0x240 | F1_V_WRITE_LIMIT_EN;
#endif
    }

    SWDBreg.u16LBOffset  = 0x0;
    SWDBreg.u16DispOffset = 0x0;
    if(eWindow == MAIN_WINDOW)
    {
        SWDBreg.u16DNROffset = 0x400;
        SWDBreg.u16OPMOffset = 0x400;
        SWDBreg.u16DNRFetch  = 0x2D0;
        SWDBreg.u16OPMFetch  = 0x2D0;
#if (HW_DESIGN_4K2K_VER == 4)
        SWDBreg.u16FRCM_WOffset = 0x400;
        SWDBreg.u16FRCM_ROffset = 0x400;
        SWDBreg.u16FRCM_WFetch  = 0x2D0;
        SWDBreg.u16FRCM_RFetch  = 0x2D0;
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        SWDBreg.u16DualDNROffset = 0x400;
        SWDBreg.u16DualOPMOffset = 0x400;
        SWDBreg.u16DualDNRFetch  = 0x2D0;
        SWDBreg.u16DualOPMFetch  = 0x2D0;
#endif
    }
    else
    {
        SWDBreg.u16DNROffset = 0x80;
        SWDBreg.u16OPMOffset = 0x80;
        SWDBreg.u16DNRFetch  = 0x80;
        SWDBreg.u16OPMFetch  = 0x80;
#if (HW_DESIGN_4K2K_VER == 4)
        SWDBreg.u16FRCM_WOffset = 0x80;
        SWDBreg.u16FRCM_ROffset = 0x80;
        SWDBreg.u16FRCM_WFetch  = 0x80;
        SWDBreg.u16FRCM_RFetch  = 0x80;
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        SWDBreg.u16DualDNROffset = 0x80;
        SWDBreg.u16DualOPMOffset = 0x80;
        SWDBreg.u16DualDNRFetch  = 0x80;
        SWDBreg.u16DualOPMFetch  = 0x80;
#endif

    }

    // Attention! Writelimit base also set in MDrv_SC_set_mirror for upper layer usage.
    if (IsVMirrorMode(eWindow))
    {
        SWDBreg.u32OPMBase0 = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] / BYTE_PER_WORD;
        SWDBreg.u32OPMBase1 = (pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]/2) / BYTE_PER_WORD;
        SWDBreg.u32OPMBase2 = 0;
        SWDBreg.u32DNRBase0 = SWDBreg.u32OPMBase0 + SWDBreg.u16DNROffset * (SWDBreg.u16VLen-1) * 3 / BYTE_PER_WORD;
        SWDBreg.u32DNRBase1 = SWDBreg.u32OPMBase1 + SWDBreg.u16DNROffset * (SWDBreg.u16VLen-1) * 3 / BYTE_PER_WORD;
        SWDBreg.u32DNRBase2 = SWDBreg.u32OPMBase2;
        if(eWindow == MAIN_WINDOW)
            SWDBreg.u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow]) / BYTE_PER_WORD - 1) | (F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN);
        else
            SWDBreg.u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow]) / BYTE_PER_WORD - 1) | (F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN);
#if (HW_DESIGN_4K2K_VER == 4)
        SWDBreg.u32FRCM_RBase0 = pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] / BYTE_PER_WORD;
        SWDBreg.u32FRCM_RBase1 = (pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]/2) / BYTE_PER_WORD;
        SWDBreg.u32FRCM_RBase2 = 0;
        SWDBreg.u32FRCM_WBase0 = SWDBreg.u32FRCM_RBase0 + SWDBreg.u16FRCM_WOffset * (SWDBreg.u16FRCMVLen-1) * 3 / BYTE_PER_WORD;
        SWDBreg.u32FRCM_WBase1 = SWDBreg.u32FRCM_RBase1 + SWDBreg.u16FRCM_WOffset * (SWDBreg.u16FRCMVLen-1) * 3 / BYTE_PER_WORD;
        SWDBreg.u32FRCM_WBase2 = SWDBreg.u32FRCM_RBase2;
        if(eWindow == MAIN_WINDOW)
            SWDBreg.u32FRCMWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow]) / BYTE_PER_WORD - 1) | (F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN);
        else
            SWDBreg.u32FRCMWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow]) / BYTE_PER_WORD - 1) | (F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN);
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        SWDBreg.u32DualOPMBase0 = pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow] / BYTE_PER_WORD;
        SWDBreg.u32DualOPMBase1 = (pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow]/2) / BYTE_PER_WORD;
        SWDBreg.u32DualOPMBase2 = 0;
        SWDBreg.u32DualDNRBase0 = SWDBreg.u32DualOPMBase0 + SWDBreg.u16DualDNROffset * (SWDBreg.u16VLen-1) * 3 / BYTE_PER_WORD;
        SWDBreg.u32DualDNRBase1 = SWDBreg.u32DualOPMBase1 + SWDBreg.u16DualDNROffset * (SWDBreg.u16VLen-1) * 3 / BYTE_PER_WORD;
        SWDBreg.u32DualDNRBase2 = SWDBreg.u32DualOPMBase2;
        if(eWindow == MAIN_WINDOW)
            SWDBreg.u32DualWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow]) / BYTE_PER_WORD - 1) | (F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN);
        else
            SWDBreg.u32DualWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow]) / BYTE_PER_WORD - 1) | (F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN);
#endif
    }
    else
    {
        SWDBreg.u32DNRBase0 = SWDBreg.u32OPMBase0 =
        SWDBreg.u32DNRBase1 = SWDBreg.u32OPMBase1 =
        SWDBreg.u32DNRBase2 = SWDBreg.u32OPMBase2 = MS_IPM_BASE0(eWindow);
        if(eWindow == MAIN_WINDOW)
            SWDBreg.u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow])
                                        / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
        else
            SWDBreg.u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow])
                                        / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
#if (HW_DESIGN_4K2K_VER == 4)
        SWDBreg.u32FRCM_WBase0 = SWDBreg.u32FRCM_RBase0 =
        SWDBreg.u32FRCM_WBase1 = SWDBreg.u32FRCM_RBase1 =
        SWDBreg.u32FRCM_WBase2 = SWDBreg.u32FRCM_RBase2 = MS_FRCM_BASE0(eWindow);
        if(eWindow == MAIN_WINDOW)
            SWDBreg.u32FRCMWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow])
                                        / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
        else
            SWDBreg.u32FRCMWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow])
                                        / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        SWDBreg.u32DualDNRBase0 = SWDBreg.u32DualOPMBase0 =
        SWDBreg.u32DualDNRBase1 = SWDBreg.u32DualOPMBase1 =
        SWDBreg.u32DualDNRBase2 = SWDBreg.u32DualOPMBase2 = MS_IPM_DUAL_MIU_BASE0(eWindow);
        if(eWindow == MAIN_WINDOW)
            SWDBreg.u32DualWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow])
                                        / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
        else
            SWDBreg.u32DualWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow])
                                        / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
#endif
    }

//    printf("@@#@@[%s][%d]SWDBreg.u32DNRBase0[%d]=%lx\n", __FUNCTION__, __LINE__, eWindow, SWDBreg.u32DNRBase0);
//    printf("@@#@@[%s][%d]SWDBreg.u32OPMBase0[%d]=%lx\n", __FUNCTION__, __LINE__, eWindow, SWDBreg.u32OPMBase0);
//    printf("@@#@@[%s][%d]SWDBreg.u32WritelimitBase[%d]=%lx\n", __FUNCTION__, __LINE__, eWindow, SWDBreg.u32WritelimitBase);

//    printf("@@#@@[%s][%d]SWDBreg.u32FRCM_RBase0[%d]=%lx\n", __FUNCTION__, __LINE__, eWindow, SWDBreg.u32FRCM_RBase0);
//    printf("@@#@@[%s][%d]SWDBreg.u32FRCM_WBase0[%d]=%lx\n", __FUNCTION__, __LINE__, eWindow, SWDBreg.u32FRCM_WBase0);
//    printf("@@#@@[%s][%d]SWDBreg.u32FRCMWritelimitBase[%d]=%lx\n", __FUNCTION__, __LINE__, eWindow, SWDBreg.u32FRCMWritelimitBase);

    MDrv_SC_set_write_limit(pInstance, SWDBreg.u32WritelimitBase, eWindow);
#if (HW_DESIGN_4K2K_VER == 4)
    MDrv_SC_set_frcm_write_limit(pInstance, ENABLE, SWDBreg.u32FRCMWritelimitBase, eWindow);
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    MDrv_SC_set_dual_write_limit(pInstance, SWDBreg.u32DualWritelimitBase, eWindow);
#endif


    #if (ENABLE_OPM_WRITE_SUPPORTED)
        MS_PHY u32OPWLimitBase = 0x00;
        u32OPWLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) ;
        MDrv_SC_set_opm_write_limit(pInstance, ENABLE, 0 , u32OPWLimitBase , eWindow);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        u32OPWLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow]) / BYTE_PER_WORD - 1) ;
        MDrv_SC_set_dual_opm_write_limit(pInstance, ENABLE, 0 , u32OPWLimitBase , eWindow);
#endif
    #endif

    SWDBreg.u16H_CapStart = 0x1;
    SWDBreg.u16H_CapSize  = 0x10;
    SWDBreg.u16V_CapStart = 0x1;
    SWDBreg.u16V_CapSize  = 0x10;
    if(eWindow == MAIN_WINDOW)
    {
        SWDBreg.u16H_DisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
        SWDBreg.u16H_DisEnd   = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1;
        SWDBreg.u16V_DisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
        SWDBreg.u16V_DisEnd   = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1;
    }
    else
    {
        SWDBreg.u16H_DisStart = 0;
        SWDBreg.u16H_DisEnd   = 0xFFF;
        SWDBreg.u16V_DisStart = 0;
        SWDBreg.u16V_DisEnd   = 0xFFF;
    }
    MDrv_SC_sw_db(pInstance, &SWDBreg, eWindow);
    memcpy(&(gSrcInfo[eWindow].stDispWin), &(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), sizeof(MS_WINDOW_TYPE));
}

//-------------------------------------------------------------------------------------------------
/// Enable or disable scaler memory read/write request
/// @param  bEnable    \b IN:  TURE=Enable memory request of 'eType';  FALSE= Disable memory request of 'eType'
/// @param  eType      \b IN:  @ref E_XC_MEMORY_REQUEST_TYPE
/// @param  eWindow    \b IN:  @ref SCALER_WIN
/// @return @ref E_APIXC_ReturnValue
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MDrv_XC_SetScalerMemoryRequest(void *pInstance, MS_BOOL bEnable, E_XC_MEMORY_REQUEST_TYPE eRequestType, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_OK;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch(eRequestType)
    {
        case E_XC_MEMORY_IP_READ_REQUEST:
            Hal_SC_IP_Memory_Read_Request(pInstance, bEnable, eWindow);
            break;
        case E_XC_MEMORY_IP_WRITE_REQUEST:
            gSrcInfo[eWindow].Status2.bIPMemWriteRequestDisabled = !bEnable;
            if(!((eWindow == SUB_WINDOW) &&
               (bEnable == TRUE)&&
               (gSrcInfo[eWindow].bBluescreenEnabled == TRUE)))
            {
                //Sub window use disable IP memory request to disable input source
                //So, if bBluescreenEnabled = TRUE, need skip enable SUB IP_Memory_Write
                Hal_SC_IP_Memory_Write_Request(pInstance, bEnable, eWindow);
            }
            break;
        case E_XC_MEMORY_OP_READ_REQUEST:
            MDrv_XC_EnableWindow(pInstance, bEnable, eWindow);//Enable/Disable window to ctrl OP read
            break;
        case E_XC_MEMORY_OP_WRITE_REQUEST:
            Hal_SC_OP_Memory_Write_Request(pInstance, bEnable, eWindow);//When disable sub window, OPW also stopped
            break;
        default:
            eRet = E_APIXC_RET_FAIL_ENUM_NOT_SUPPORT;
            break;
    }
    return eRet;
}

E_APIXC_ReturnValue MApi_XC_SetScalerMemoryRequest_U2(void* pInstance, MS_BOOL bEnable, E_XC_MEMORY_REQUEST_TYPE eRequestType, SCALER_WIN eWindow)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    eReturn = MDrv_XC_SetScalerMemoryRequest(pInstance, bEnable, eRequestType, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eReturn;
}

E_APIXC_ReturnValue MApi_XC_SetScalerMemoryRequest(MS_BOOL bEnable, E_XC_MEMORY_REQUEST_TYPE eRequestType, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_SCALER_MEMORY_REQUEST XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eRequestType = eRequestType;
    XCArgs.eWindow = eWindow;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SCALER_MEMORY_REQUEST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//Obsolte function, do not use it any more
E_APIXC_ReturnValue MApi_XC_SetMemoryWriteRequest_U2(void* pInstance, MS_BOOL bEnable)
{
    E_APIXC_ReturnValue eReturn = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    eReturn = MDrv_XC_SetScalerMemoryRequest(pInstance, bEnable, E_XC_MEMORY_IP_WRITE_REQUEST, MAIN_WINDOW);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return eReturn;

}

E_APIXC_ReturnValue MApi_XC_SetMemoryWriteRequest(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_MEMORY_WRITE_REQUEST XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_MEMORY_WRITE_REQUEST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set DNR base0 address
/// @param  u32FBAddress      \b IN: the framebuffer address
/// @param  u32FBSize         \b IN: the framebuffer size
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFrameBufferAddress_U2(void* pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetFrameBufferAddress(pInstance, u32FBAddress, u32FBSize, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetFrameBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRAMEBUFFER_ADDRESS XCArgs;
    XCArgs.u32FBAddress = u32FBAddress;
    XCArgs.u32FBSize = u32FBSize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRAMEBUFFER_ADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set DNR base0 address
/// @param  u32FBAddress      \b IN: the framebuffer address
/// @param  u32FBSize         \b IN: the framebuffer size
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFRCMFrameBufferAddress_U2(void* pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetFRCMFrameBufferAddress(pInstance, u32FBAddress, u32FBSize, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetFRCMFrameBufferAddress(MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRAMEBUFFER_ADDRESS XCArgs;
    XCArgs.u32FBAddress = u32FBAddress;
    XCArgs.u32FBSize = u32FBSize;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRCM_FRAMEBUFFER_ADDRESS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_XC_EnableMiuDualMode(void* pInstance, MS_BOOL bEnable, MS_U32 u32LeftFactor, MS_U32 u32RightFactor, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].Status2.stMiuDualModCfg.bEnableMiuDualMod = bEnable;
    gSrcInfo[eWindow].Status2.stMiuDualModCfg.u32LeftFactor = u32LeftFactor;
    gSrcInfo[eWindow].Status2.stMiuDualModCfg.u32RightFactor = u32RightFactor;
#else
    return FALSE;
#endif
    return TRUE;
}

void MDrv_XC_SetFrameBufferAddressSilently(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    MS_U8 u8MIUSel = E_CHIP_MIU_0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow] = u32FBSize;

    if(eWindow == MAIN_WINDOW)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr = u32FBAddress;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size = u32FBSize;
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Main_FB_Start_Addr=%lx\n",u32FBAddress);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Main_FB_Start_Addr=%tx\n",(ptrdiff_t)u32FBAddress);
#endif
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr = u32FBAddress;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size = u32FBSize;
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Sub_FB_Start_Addr=%lx\n",u32FBAddress);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Sub_FB_Start_Addr=%tx\n",(ptrdiff_t)u32FBAddress);
#endif
    }

    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);
    UNUSED(u8MIUSel);
    MHal_XC_SetDNRBufAddress(pInstance, u32FBAddress ,eWindow);
    MHal_XC_SetDNRBufSize(pInstance, u32FBSize ,eWindow) ;

}

void MDrv_XC_SetFRCMFrameBufferAddressSilently(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    MS_U8 u8MIUSel = E_CHIP_MIU_0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);
    UNUSED(u8MIUSel);

    pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow] = u32FBSize;

    if(eWindow == MAIN_WINDOW)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Start_Addr = u32FBAddress;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Size = u32FBSize;
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u32Main_FRCM_FB_Start_Addr=%lx\n",u32FBAddress);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u32Main_FRCM_FB_Start_Addr=%tx\n",(ptrdiff_t)u32FBAddress);
#endif
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Start_Addr = u32FBAddress;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Size = u32FBSize;
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u32Sub_FRCM_FB_Start_Addr=%lx\n",u32FBAddress);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u32Sub_FRCM_FB_Start_Addr=%tx\n",(ptrdiff_t)u32FBAddress);
#endif
    }
    MHal_XC_SetFRCMBufAddress(pInstance, u32FBAddress ,eWindow);
    MHal_XC_SetFRCMBufSize(pInstance, u32FBSize ,eWindow) ;

    MS_PHY u32FRCMLimitBase = 0x00;
    u32FRCMLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]) / BYTE_PER_WORD - 1) ;
    MDrv_SC_set_frcm_write_limit(pInstance, ENABLE, u32FRCMLimitBase , eWindow);
#endif
}

void MDrv_XC_SetFrameBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    MS_U8 u8MIUSel = E_CHIP_MIU_0;
    MS_BOOL bNeedRestore = FALSE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    MS_BOOL bNeedChangeInstance = FALSE;
    void *pInstanceTmp = NULL;
    // change instance to sc1 instance if ewindow is main and using sc1 as sub
    if ((psXCInstPri->u32DeviceID != 1) && (eWindow == SUB_WINDOW))
    {
        pInstanceTmp = pInstance;  // store instance 0
        pInstance = g_pDevice1Instance; //switch to instance 1
        eWindow = MAIN_WINDOW;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
        bNeedChangeInstance = TRUE; // mark instance had change
    }
#endif


    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] == u32FBAddress)
        && (pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow] == u32FBSize))
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (bNeedChangeInstance == TRUE)
        {
            pInstance = pInstanceTmp;  // restore instance 0
        }
#endif
        return;
    }

    pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow]    = u32FBSize;

    if (Hal_SC_Is_InputSource_Disable(pInstance, eWindow)==0)
    {

        bNeedRestore = TRUE;
        MDrv_XC_DisableInputSource(pInstance, TRUE, eWindow);
        //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"input source (Window:%d) must be disabled before set frame buffer address. Ex:when video play, you can¡¦t use this function. \n",eWindow);
    }

    //remove the offset
    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);

    pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow] = u32FBSize;

    Hal_SC_Enable_MiuMask(pInstance);

    Hal_SC_set_freezeimg(pInstance, ENABLE, eWindow);
    Hal_SC_set_miusel(pInstance, u8MIUSel);

    if (IsVMirrorMode(eWindow))
    {
        Hal_SC_set_memoryaddress(pInstance,
                                 u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000,
                                 u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, eWindow);
    }
    else
    {
        Hal_SC_set_memoryaddress(pInstance,
                                 u32FBAddress, u32FBAddress, u32FBAddress,
                                 u32FBAddress, u32FBAddress, u32FBAddress, eWindow);
    }
    MDrv_SC_set_mirrorEx(pInstance, gSrcInfo[eWindow].Status2.eMirrorMode, eWindow);     // set write limit address


    #if (ENABLE_OPM_WRITE_SUPPORTED)
        MS_PHY u32OPWLimitBase = 0x00;

        u32OPWLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) ;
        MDrv_SC_set_opm_write_limit(pInstance, ENABLE, 0 , u32OPWLimitBase , eWindow);
    #endif


    Hal_SC_set_freezeimg(pInstance, DISABLE, eWindow);

    Hal_SC_Disable_MiuMask(pInstance);

    if(eWindow == MAIN_WINDOW)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Start_Addr = pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow];
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FB_Size = pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow];
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Main_FB_Start_Addr=%lx, Size=%lx\n", u32FBAddress, u32FBSize);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Main_FB_Start_Addr=%tx, Size=%tx\n", (ptrdiff_t)u32FBAddress, (ptrdiff_t)u32FBSize);
#endif
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Start_Addr = pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow];
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FB_Size = pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow];
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Sub_FB_Start_Addr=%lx, Size=%lx\n", u32FBAddress, u32FBSize);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Sub_FB_Start_Addr=%tx, Size=%tx\n", (ptrdiff_t)u32FBAddress, (ptrdiff_t)u32FBSize);
#endif
    }

    MHal_XC_SetDNRBufAddress(pInstance, u32FBAddress ,eWindow);
    MHal_XC_SetDNRBufSize(pInstance, u32FBSize ,eWindow) ;

#if ((HW_DESIGN_4K2K_VER == 4)||(HW_DESIGN_4K2K_VER == 6))
    MS_PHY u32WLimitBase = 0x00;
    if (eWindow == MAIN_WINDOW)
        u32WLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1)  | F2_WRITE_LIMIT_EN;
    else
        u32WLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1)  | F1_WRITE_LIMIT_EN;

    MDrv_SC_set_write_limit(pInstance, u32WLimitBase , eWindow);
#endif

    if(MAIN_WINDOW == eWindow)
    {
        //Main window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(4) |BIT(5) |BIT(6));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, 0, BIT(7));
    }
    else
    {
        //Sub window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(4), BIT(4) |BIT(5) |BIT(6));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_47_L, 0, BIT(7));
    }

    if (bNeedRestore == TRUE)
    {
        bNeedRestore = FALSE;
        MDrv_XC_DisableInputSource(pInstance, FALSE, eWindow);
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if (bNeedChangeInstance == TRUE)
    {
        pInstance = pInstanceTmp;  // restore instance 0
    }
#endif
}

MS_BOOL MDrv_XC_SetDualFrameBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    MS_U8 u8MIUSel = E_CHIP_MIU_0;
    MS_BOOL bNeedRestore = FALSE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_4K2K_VER == 6)
    if (eWindow == SUB_WINDOW)
    {
        return bRet;
    }
#endif
    if((pXCResourcePrivate->stdrvXC_Scaling._u32PreDualFBAddress[eWindow] == u32FBAddress)
        && (pXCResourcePrivate->stdrvXC_Scaling._u32PreDualFBSize[eWindow] == u32FBSize))
    {
        return FALSE;
    }
    pXCResourcePrivate->stdrvXC_Scaling._u32PreDualFBAddress[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._u32PreDualFBSize[eWindow]    = u32FBSize;

    if (Hal_SC_Is_InputSource_Disable(pInstance, eWindow)==0)
    {

        bNeedRestore = TRUE;
        MDrv_XC_DisableInputSource(pInstance, TRUE, eWindow);
        //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"input source (Window:%d) must be disabled before set frame buffer address. Ex:when video play, you can¡¦t use this function. \n",eWindow);
    }

    //remove the offset
    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);

    pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow] = u32FBSize;

    Hal_SC_Enable_MiuMask(pInstance);

    Hal_SC_set_freezeimg(pInstance, ENABLE, eWindow);
    Hal_SC_set_dual_miusel(pInstance, u8MIUSel);

    if (IsVMirrorMode(eWindow))
    {
        Hal_SC_set_dual_memoryaddress(pInstance,
                                 u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000,
                                 u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, eWindow);
    }
    else
    {
        Hal_SC_set_dual_memoryaddress(pInstance,
                                 u32FBAddress, u32FBAddress, u32FBAddress,
                                 u32FBAddress, u32FBAddress, u32FBAddress, eWindow);
    }
    //FIXME: Richard.Deng check support dual miu
    MDrv_SC_set_mirrorEx(pInstance, gSrcInfo[eWindow].Status2.eMirrorMode, eWindow);     // set write limit address


#if (ENABLE_OPM_WRITE_SUPPORTED)
    MS_PHY u32OPWLimitBase = 0x00;

    u32OPWLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow]) / BYTE_PER_WORD - 1) ;
    MDrv_SC_set_dual_opm_write_limit(pInstance, ENABLE, 0 , u32OPWLimitBase , eWindow);
#endif


    Hal_SC_set_freezeimg(pInstance, DISABLE, eWindow);

    Hal_SC_Disable_MiuMask(pInstance);
//Monet default use cma, this will be opened for no cma dual case
#if 0
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Dual_FB_Start_Addr[eWindow] = pXCResourcePrivate->stdrvXC_Scaling._u32PreDualFBAddress[eWindow];
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Dual_FB_Size[eWindow] = pXCResourcePrivate->stdrvXC_Scaling._u32PreDualFBSize[eWindow];
#endif
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Dual_FB_Start_Addr[%d] =%lx, Size=%lx\n", eWindow, u32FBAddress, u32FBSize);
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Dual_FB_Start_Addr[%d] =%llx, Size=%llx\n", eWindow, u32FBAddress, u32FBSize);
#endif

    MHal_XC_SetDualDNRBufAddress(pInstance, u32FBAddress ,eWindow);
    MHal_XC_SetDualDNRBufSize(pInstance, u32FBSize ,eWindow) ;

#if (HW_DESIGN_4K2K_VER == 6)
    MS_PHY u32WLimitBase = 0x00;
    u32WLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN ;
    MDrv_SC_set_dual_write_limit(pInstance, u32WLimitBase , eWindow);
#endif

    if (bNeedRestore == TRUE)
    {
        bNeedRestore = FALSE;
        MDrv_XC_DisableInputSource(pInstance, FALSE, eWindow);
    }
    bRet = TRUE;
#endif
    return bRet;
}

void MDrv_XC_SetFRCMFrameBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, SCALER_WIN eWindow)
{
    MS_U8 u8MIUSel = E_CHIP_MIU_0;
    MS_BOOL bNeedRestore = FALSE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // is need to do for str?
#if 0
    if((pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] == u32FBAddress)
        && (pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow] == u32FBSize))
    {
        return;
    }

    pXCResourcePrivate->stdrvXC_Scaling._au32PreFBAddress[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._au32PreFBSize[eWindow]    = u32FBSize;
#endif
    MS_PHY u32PreFBAddress = u32FBAddress;
    MS_PHY u32PreFBSize = u32FBSize;

    if (Hal_SC_Is_InputSource_Disable(pInstance, eWindow)==0)
    {

        bNeedRestore = TRUE;
        MDrv_XC_DisableInputSource(pInstance, TRUE, eWindow);
        //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"input source (Window:%d) must be disabled before set frame buffer address. Ex:when video play, you can¡¦t use this function. \n",eWindow);
    }

    //remove the offset
    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);

    pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow] = u32FBSize;

    Hal_SC_Enable_MiuMask(pInstance);

    Hal_SC_set_frcm_freezeimg(pInstance, ENABLE, eWindow);
    Hal_SC_set_miusel(pInstance, u8MIUSel);

    if (IsVMirrorMode(eWindow))
    {
        Hal_SC_set_frcm_memoryaddress(pInstance,
                                 u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000,
                                 u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, u32FBAddress+ u32FBSize-1-0x100000, eWindow);
    }
    else
    {
        Hal_SC_set_frcm_memoryaddress(pInstance,
                                 u32FBAddress, u32FBAddress, u32FBAddress,
                                 u32FBAddress, u32FBAddress, u32FBAddress, eWindow);
    }
    MDrv_SC_set_mirrorEx(pInstance, gSrcInfo[eWindow].Status2.eMirrorMode, eWindow);     // set write limit address

    #if (HW_DESIGN_4K2K_VER == 4)
    MHal_XC_SetFRCMBufAddress(pInstance, u32FBAddress ,eWindow);
    MHal_XC_SetFRCMBufSize(pInstance, u32FBSize ,eWindow) ;
#endif

#if (HW_DESIGN_4K2K_VER == 4)
        MS_PHY u32FRCMLimitBase = 0x00;

        u32FRCMLimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]) / BYTE_PER_WORD - 1) ;
        MDrv_SC_set_frcm_write_limit(pInstance, ENABLE, u32FRCMLimitBase , eWindow);
    #endif

    Hal_SC_set_frcm_freezeimg(pInstance, DISABLE, eWindow);

    Hal_SC_Disable_MiuMask(pInstance);

    if(eWindow == MAIN_WINDOW)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Start_Addr = u32PreFBAddress;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Main_FRCM_FB_Size = u32PreFBSize;
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Main_FB_Start_Addr=%lx, Size=%lx\n", u32FBAddress, u32FBSize);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Main_FB_Start_Addr=%tx, Size=%tx\n", (ptrdiff_t)u32FBAddress, (ptrdiff_t)u32FBSize);
#endif
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Start_Addr = u32PreFBAddress;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32Sub_FRCM_FB_Size = u32PreFBSize;
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Sub_FB_Start_Addr=%lx, Size=%lx\n", u32FBAddress, u32FBSize);
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change Sub_FB_Start_Addr=%tx, Size=%tx\n", (ptrdiff_t)u32FBAddress, (ptrdiff_t)u32FBSize);
#endif
    }


    if(MAIN_WINDOW == eWindow)
    {
        //Main window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_07_L, BIT(4), BIT(4) |BIT(5) |BIT(6));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_07_L, 0, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_07_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_07_L, 0, BIT(7));
    }
    else
    {
        //Sub window
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_47_L, BIT(4), BIT(4) |BIT(5) |BIT(6));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_47_L, 0, BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_47_L, BIT(7), BIT(7));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_47_L, 0, BIT(7));
    }

    if (bNeedRestore == TRUE)
    {
        bNeedRestore = FALSE;
        MDrv_XC_DisableInputSource(pInstance, FALSE, eWindow);
    }
}

MS_BOOL MApi_XC_IsFrameBufferEnoughForCusScaling_U2(void* pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //DTV max u8BitsPerPixel is 24. And here only consider 2 frame mode.
    if ((MS_PHY)pstXC_SetWin_Info->u16PreVCusScalingDst *
            (MS_PHY)pstXC_SetWin_Info->u16PreHCusScalingDst * 24 * 2 > pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow] * 8)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return FALSE;
    }
    else
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return TRUE;
    }
}

MS_BOOL MApi_XC_IsFrameBufferEnoughForCusScaling(XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING XCArgs;
    XCArgs.pstXC_SetWin_Info = pstXC_SetWin_Info;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FRAMEBUFFER_ENOUGH_FOR_CUSSCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_SC_set_fbl(void *pInstance, MS_BOOL bEn)
{
    Hal_SC_set_fbl(pInstance, bEn);
}

MS_BOOL MDrv_SC_get_fbl(void *pInstance)
{
    return Hal_SC_get_fbl(pInstance);
}


/******************************************************************************/
///Set Capture window (Input window) information.
///@param pstWindow \b IN
///- pointer to window information
/******************************************************************************/
void MDrv_sc_set_capture_window(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    // HDMI HV mode uses DE by pass, so capture win should be set as full range
    if( ( IsSrcTypeHDMI( gSrcInfo[eWindow].enInputSourceType ) ) && (MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_HV) )
    {
        // Capture win must keep full range in HV mode
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapStart = 0xC;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapStart = 0xC;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapSize = 0x1FFF;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize = 0x1FFF;
    }
    else
    {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapStart = gSrcInfo[eWindow].stCapWin.y;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapStart = gSrcInfo[eWindow].stCapWin.x;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapSize  = gSrcInfo[eWindow].stCapWin.height;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_CapSize  = gSrcInfo[eWindow].stCapWin.width;

        // In Force Interlace mode, upper layer does not know we already change the V capture size.
        // So we correct it here.
        if (MDrv_SC_GetInterlaceInPModeStatus(pInstance, eWindow)
            && (MDrv_XC_GetForceiSupportType(pInstance) == ForceI_SW))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapStart = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapStart+1) * 2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_CapSize <<=1;
        }
    }

}


/******************************************************************************/
///Set output display window size
///@param pstWindow \b IN
///-  pointer to window information
/******************************************************************************/
void MDrv_SC_set_display_window(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(gSrcInfo[eWindow].bDisplayNineLattice)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart +pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width-1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height-1;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisStart = gSrcInfo[eWindow].stDispWin.x;

        if ( gSrcInfo[eWindow].stDispWin.width == 0 )
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd = gSrcInfo[eWindow].stDispWin.x;
        else
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16H_DisEnd = gSrcInfo[eWindow].stDispWin.x +  gSrcInfo[eWindow].stDispWin.width - 1;

        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart = gSrcInfo[eWindow].stDispWin.y;

        if ( gSrcInfo[eWindow].stDispWin.height == 0 )
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = gSrcInfo[eWindow].stDispWin.y;
        else
        {
#ifndef DISABLE_3D_FUNCTION
             //dualview case handling
            if( MDrv_SC_3D_Is_LR_Sbs2Line(pInstance) &&
                (E_XC_3D_INPUT_MODE_NONE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)) &&
                (MAIN_WINDOW == eWindow))
            {
                if(IsVMirrorMode(eWindow) )
                {
                    if(gSrcInfo[eWindow].stDispWin.y >= gSrcInfo[eWindow].stDispWin.height)
                    {
                        gSrcInfo[eWindow].stDispWin.y -= gSrcInfo[eWindow].stDispWin.height;
                    }
                    else
                    {
                        gSrcInfo[eWindow].stDispWin.y = 0;
                    }
                    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisStart = gSrcInfo[eWindow].stDispWin.y;
                }
                //main window need set to fullscreen to prevent show garbage when atv mirror channel change
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = gSrcInfo[eWindow].stDispWin.y +  gSrcInfo[eWindow].stDispWin.height * 2 - 1;
            }
            else
#endif
            {
                pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16V_DisEnd = gSrcInfo[eWindow].stDispWin.y +  gSrcInfo[eWindow].stDispWin.height - 1;
            }
        }
    }
}


/******************************************************************************/
///Fill display window according to panel
/******************************************************************************/

void MDrv_SC_set_std_display_window(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    gSrcInfo[eWindow].stDispWin.x       = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart;
    gSrcInfo[eWindow].stDispWin.y       = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
    gSrcInfo[eWindow].stDispWin.width   = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
    gSrcInfo[eWindow].stDispWin.height  = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
}

//////////////////////////////////////////////////////////////////
// Compute ScaledCropWin according to CropWin and prescaling ratio
// parameter:
//    [IN/OUT] pSrcInfo->stCropWin
//    [IN/OUT] pSrcInfo->stScaledCropWin
//    [IN]     pSrcInfo->u16H_CapSize
//    [IN]     pSrcInfo->u16H_SizeAfterPreScaling
//    [IN]     pSrcInfo->u16V_CapSize
//    [IN]     pSrcInfo->u16V_SizeAfterPreScaling
//
void MDrv_SC_set_crop_window(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_WINDOW_TYPE *pCropWin = &pSrcInfo->stCropWin;

    // for Compression mode
    MS_BOOL bopm_lcnt_inv = FALSE;
    MS_U8 u8ScaledCropY = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pSrcInfo->stCapWin.width == 0 || pSrcInfo->stCapWin.height == 0)
    {
        printf("XC CapWin is error ! (Width=%d,Height=%d)\n",pSrcInfo->stCapWin.width,pSrcInfo->stCapWin.height);
        return;
    }

    if(!pSrcInfo->bDisplayNineLattice) // need refine for dircet display
    {
        gSrcInfo[eWindow].ScaledCropWin.x     = (MS_U16)((MS_U32)pCropWin->x * pSrcInfo->u16H_SizeAfterPreScaling / pSrcInfo->stCapWin.width);
        gSrcInfo[eWindow].ScaledCropWin.width = (MS_U16)( ( (MS_U32)pCropWin->width * pSrcInfo->u16H_SizeAfterPreScaling + ( pSrcInfo->stCapWin.width / 2  ) ) / pSrcInfo->stCapWin.width);

        gSrcInfo[eWindow].ScaledCropWin.y     = (MS_U16)( (MS_U32) pCropWin->y * pSrcInfo->u16V_SizeAfterPreScaling / pSrcInfo->stCapWin.height);
        gSrcInfo[eWindow].ScaledCropWin.height= (MS_U16)( ( (MS_U32)pCropWin->height * pSrcInfo->u16V_SizeAfterPreScaling + ( pSrcInfo->stCapWin.height / 2 ) ) / pSrcInfo->stCapWin.height);
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"W=%u, W PSD=%u\n", pSrcInfo->stCapWin.width  , pSrcInfo->u16H_SizeAfterPreScaling);
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[1] x,y,w,h=%u,%u,%u,%u\n",gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y, gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height);

        // Scaled crop win may introduce imprecise calculation result, which may lead to pre and post scaling both working
        // When driver do auto fit, we only want one of pre and post working
        // Here we ensure only pre scaling is working
        // This may conflict with 3D, need refine later
#if (HW_DESIGN_3D_VER >= 2)
#ifndef DISABLE_3D_FUNCTION
        if(MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_MODE_NONE)
#endif
        {
            //HW version 1's display window is 1/2 of panel's display width in output side by side case.
            //so not into here. but HW version >1's display width is same with the panel.
            if((gSrcInfo[eWindow].ScaledCropWin.width != pSrcInfo->stDispWin.width) &&
               (pXCResourcePrivate->stdrvXC_Scaling._bHAutoFitPrescalingEna == TRUE))
            {
                //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Force change ScaledCropWin.width: %u->%u\n", gSrcInfo[eWindow].ScaledCropWin.width, pSrcInfo->stDispWin.width);
                gSrcInfo[eWindow].ScaledCropWin.width = pSrcInfo->stDispWin.width;
            }

            if((gSrcInfo[eWindow].ScaledCropWin.height != pSrcInfo->stDispWin.height) &&
               (pXCResourcePrivate->stdrvXC_Scaling._bVAutoFitPrescalingEna == TRUE))
            {
                //XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Force change ScaledCropWin.height: %u->%u\n", gSrcInfo[eWindow].ScaledCropWin.height, pSrcInfo->stDispWin.height);
                gSrcInfo[eWindow].ScaledCropWin.height= pSrcInfo->stDispWin.height;
            }
        }
#endif
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[2] x,y,w,h=%u,%u,%u,%u\n",gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y, gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height);

        #ifdef ENABLE_SCALING_WO_MUTE
            // For H scaling down to too small size, there is HW limitation only support 16x H scaling down
            MDrv_SC_Adjust_H_Crop(pInstance, eWindow);
        #endif

        u8ScaledCropY = gSrcInfo[eWindow].ScaledCropWin.y;

        // Error handling.
        if (IsVMirrorMode(eWindow))//Error handling for mirror case
        {
#ifndef DISABLE_3D_FUNCTION
           MDrv_SC_3D_AdjustMirrorCrop(pInstance, pSrcInfo,eWindow);
#endif
        }
        else//Error handling for Non-mirror case
        {
            if(!MDrv_XC_Is_SupportSWDS(pInstance))
            {
                if( pSrcInfo->u16H_SizeAfterPreScaling < (gSrcInfo[eWindow].ScaledCropWin.width + gSrcInfo[eWindow].ScaledCropWin.x) )
                {
                    if(gSrcInfo[eWindow].ScaledCropWin.width <= pSrcInfo->u16H_SizeAfterPreScaling)
                    {
                        gSrcInfo[eWindow].ScaledCropWin.x = (pSrcInfo->u16H_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.width);
                    }
                    else
                    {
                        gSrcInfo[eWindow].ScaledCropWin.x = 0;
                        gSrcInfo[eWindow].ScaledCropWin.width = pSrcInfo->u16H_SizeAfterPreScaling;
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[2.c] Force x=%u, width=%u\n",gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.width);
                }
            }

#if (HW_DESIGN_4K2K_VER == 4)
            if(gSrcInfo[eWindow].bInterlace
                &&
                (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
                    ||(psXCInstPri->u32DeviceID == 1)))
#else
            // for interlace, keep y in multiple of 2
            if(gSrcInfo[eWindow].bInterlace)
#endif
            {
                gSrcInfo[eWindow].ScaledCropWin.y = (gSrcInfo[eWindow].ScaledCropWin.y + 1) & ~0x1;
                gSrcInfo[eWindow].ScaledCropWin.height = gSrcInfo[eWindow].ScaledCropWin.height & ~0x1;
            }

            if(!MDrv_XC_Is_SupportSWDS(pInstance))
            {
                    if( pSrcInfo->u16V_SizeAfterPreScaling < (gSrcInfo[eWindow].ScaledCropWin.height + gSrcInfo[eWindow].ScaledCropWin.y) )
                    {
                        if(gSrcInfo[eWindow].ScaledCropWin.height <= pSrcInfo->u16V_SizeAfterPreScaling)
                        {
                            gSrcInfo[eWindow].ScaledCropWin.y = (pSrcInfo->u16V_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.height) & ~0x1;
                        }
                        else
                        {
                            gSrcInfo[eWindow].ScaledCropWin.height = pSrcInfo->u16V_SizeAfterPreScaling;
                        }
                        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[2.d] Force y=%u, height=%u\n",gSrcInfo[eWindow].ScaledCropWin.y, gSrcInfo[eWindow].ScaledCropWin.height);
                    }
            }
        }

#if (HW_DESIGN_4K2K_VER == 4)
        if(gSrcInfo[eWindow].bInterlace
           &&
           (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
             ||(psXCInstPri->u32DeviceID == 1)))
#else
        if(gSrcInfo[eWindow].bInterlace)
#endif
        {
            gSrcInfo[eWindow].ScaledCropWin.height = gSrcInfo[eWindow].ScaledCropWin.height & ~0x1;
        }

#ifdef SUPPORT_BWD
        //For BWR: XC write memory using YUV420, ScaledCropWin.y/ScaledCropWin.heigh need 2 align.
        if ((IS_INPUT_TOP_BOTTOM(eWindow) || IS_INPUT_LINE_ALTERNATIVE(eWindow) || IS_INPUT_FRAME_PACKING(eWindow))
             && (!IS_OUTPUT_MODE_NONE()))
        {
            // 3D case: SN have crop.Y/2 as L/R view
            gSrcInfo[eWindow].ScaledCropWin.y = (gSrcInfo[eWindow].ScaledCropWin.y + 1) & ~0x1;
            // 3D case: crop.Height/2 need to do in utopia
            gSrcInfo[eWindow].ScaledCropWin.height = gSrcInfo[eWindow].ScaledCropWin.height & ~0x3;

            if(!MDrv_XC_Is_SupportSWDS(pInstance))
            {
                // check crop.height out of window.
                if( pSrcInfo->u16V_SizeAfterPreScaling < (gSrcInfo[eWindow].ScaledCropWin.height + gSrcInfo[eWindow].ScaledCropWin.y) )
                {
                    gSrcInfo[eWindow].ScaledCropWin.height -=4;
                }
            }
        }
        else
        {
            // For no-3D, just need crop.y and crop.height 2 align.
            gSrcInfo[eWindow].ScaledCropWin.y = (gSrcInfo[eWindow].ScaledCropWin.y + 1) & ~0x1;
            gSrcInfo[eWindow].ScaledCropWin.height = gSrcInfo[eWindow].ScaledCropWin.height & ~0x1;
            if(!MDrv_XC_Is_SupportSWDS(pInstance))
            {
                // check crop.height out of window.
                if( pSrcInfo->u16V_SizeAfterPreScaling < (gSrcInfo[eWindow].ScaledCropWin.height + gSrcInfo[eWindow].ScaledCropWin.y) )
                {
                    gSrcInfo[eWindow].ScaledCropWin.height -= 2;
                }
            }
        }
#endif

#if (HW_DESIGN_4K2K_VER == 4)
    if(MDrv_XC_Is_SupportSWDS(pInstance))
    {
        if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
        {
            pSrcInfo->u16FRCMV_Length = pSrcInfo->u16V_SizeAfterPreScaling;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCMVLen = pSrcInfo->u16FRCMV_Length;
        }
    }
#endif

        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[3] x,y,w,h=%u,%u,%u,%u\n",gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y, gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height);
#ifndef DISABLE_3D_FUNCTION
        MDrv_SC_3D_Disable_Crop(pInstance, pSrcInfo, eWindow);
#endif
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Scaled crop[4] x,y,w,h=%u,%u,%u,%u\n",pSrcInfo->ScaledCropWin.x, pSrcInfo->ScaledCropWin.y, pSrcInfo->ScaledCropWin.width, pSrcInfo->ScaledCropWin.height);
    }

    // for Compression mode control
    //new compression algorithm port from toshiba
    //to do:DS+compression mode haven't been tested
#if (HW_DESIGN_4K2K_VER == 4)
    if(gSrcInfo[eWindow].bInterlace
       &&
       (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
         ||(psXCInstPri->u32DeviceID == 1)))
#else
    if(gSrcInfo[eWindow].bInterlace)
#endif
    {
        if(IsVMirrorMode(eWindow))
        {
            if ((u8ScaledCropY/2)%2)
            {
                if ((gSrcInfo[eWindow].ScaledCropWin.height/2)%2)
                {
                    bopm_lcnt_inv = TRUE;
                }
                else
                {
                    bopm_lcnt_inv = FALSE;
                }
            }
            else
            {
                if ((gSrcInfo[eWindow].ScaledCropWin.height/2)%2)
                {
                    bopm_lcnt_inv = FALSE;
                }
                else
                {
                    bopm_lcnt_inv = TRUE;
                }
            }
        }
        else
        {
            if ((gSrcInfo[eWindow].ScaledCropWin.y/2)%2)
            {
                bopm_lcnt_inv = TRUE;
            }
            else
            {
                bopm_lcnt_inv = FALSE;
            }
        }
    }
    else
    {
        if(IsVMirrorMode(eWindow))
        {
            if (u8ScaledCropY%2)
            {
                if (gSrcInfo[eWindow].ScaledCropWin.height%2)
                {
                    bopm_lcnt_inv = TRUE;
                }
                else
                {
                    bopm_lcnt_inv = FALSE;
                }
            }
            else
            {
                if (gSrcInfo[eWindow].ScaledCropWin.height%2)
                {
                    bopm_lcnt_inv = FALSE;
                }
                else
                {
                    bopm_lcnt_inv = TRUE;
                }
            }
        }
        else
        {
            if(gSrcInfo[eWindow].ScaledCropWin.y%2)
            {
                bopm_lcnt_inv = TRUE;
            }
            else
            {
                bopm_lcnt_inv = FALSE;
            }
        }
    }
    //Hal_SC_sw_lcnt_en(pInstance, bLcnt_en, eWindow);
    //Hal_SC_set_sw_lcnt(pInstance, bLcnt, eWindow);
    //// Change to use menuload and set it by sw_db_burst
    if(1)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.bOpmLcntInv_En = bopm_lcnt_inv;
    }
    else
    {
        Hal_SC_set_opm_lcnt_inv(pInstance, bopm_lcnt_inv, eWindow);

        //monaco/muji 3D mode need control sub for some output format, or else the edge of object in the image may shake
    #if (HW_DESIGN_4K2K_VER == 4)
        E_XC_3D_OUTPUT_MODE e3DOutputFormat = MDrv_XC_Get_3D_Output_Mode(pInstance);
        if((e3DOutputFormat == E_XC_3D_OUTPUT_LINE_ALTERNATIVE)
            || (e3DOutputFormat == E_XC_3D_OUTPUT_TOP_BOTTOM) || (e3DOutputFormat == E_XC_3D_OUTPUT_TOP_BOTTOM_HW)
            || (e3DOutputFormat == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF) || (e3DOutputFormat == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW)
            || (e3DOutputFormat == E_XC_3D_OUTPUT_FRAME_PACKING))
        {
            Hal_SC_set_opm_lcnt_inv(pInstance, bopm_lcnt_inv, SUB_WINDOW);
        }
    #endif
    }
}

MS_U16 MDrv_SC_GetAutoPrescalingDst(XC_InternalStatus *pSrcInfo, MS_BOOL bHorizontal)
{
    MS_U16 u16ScaleDst;
    if(bHorizontal)
    {
        if ( pSrcInfo->stCropWin.width == 0 )
        {
            u16ScaleDst = 32;
        }
        else if (pSrcInfo->stCropWin.width > pSrcInfo->stDispWin.width)
        {
            u16ScaleDst = (( (MS_U32)pSrcInfo->stCapWin.width * pSrcInfo->stDispWin.width + ( pSrcInfo->stCropWin.width / 2 ) ) / pSrcInfo->stCropWin.width);
        }
        else
        {
            u16ScaleDst = pSrcInfo->stCapWin.width;
        }

        if((pSrcInfo->stCapWin.width <= (WIDTH_4K2K - WIDTH_4K2K_OFFSET)) && (u16ScaleDst > SCALER_LINE_BUFFER_MAX))
        {
            u16ScaleDst = SCALER_LINE_BUFFER_MAX;
        }
    }
    else
    {
        if (pSrcInfo->stCropWin.height == 0) // error handling
        {
            u16ScaleDst = 32;
        }
        else
        {
            if( !pSrcInfo->bInterlace)
            {
                // Progressive
                // Pre-scaling to specific size. This formula will make sure driver only use one of Presacling or PostScaling
                if (pSrcInfo->stCropWin.height > pSrcInfo->stDispWin.height)
                {
                    u16ScaleDst = ( (MS_U32)pSrcInfo->stCapWin.height * pSrcInfo->stDispWin.height + ( pSrcInfo->stCropWin.height / 2 ) ) / pSrcInfo->stCropWin.height;
                }
                else
                {
                    u16ScaleDst = pSrcInfo->stCapWin.height;
                }

            }
            else
            {
                // Interlace
                u16ScaleDst = (pSrcInfo->stCapWin.height / 2);
            }
        }
    }
    return u16ScaleDst;
}

//  1. mirror+freerun (P mode. eg 25p,30p),  at least 3 frames (interlace mode have no this limitation)
//  2. mirror (24p, and not 1:1, 1:2 FRC),      at least 3 frames
//  3. mirror+uc (24p, and not 1:1, 1:2 FRC),       at least 4 frames
//  4. non-mirror+uc (P mode),   at least 3 frames (interlace mode only need 4 field, eg. 25_4R_MC)
//  5. sub win (no matter mirror or not),         at least 3 frames
//  6. we also have this limitation, but we don't cover PQ for it:
//         mirror+uc(I mode), at least 6 fields (currently, have no 6 fields MC madi, so use 25_6R_MC(8 fields) instead)
void MDrv_XC_AdjustPrescalingForFrameNum(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_U16 u16FrameCount = 0;
    MS_U8 u8Alignment = OFFSET_PIXEL_ALIGNMENT - 1;
    SC_FRC_SETTING astCurFRCTable[MAX_FRC_TABLE_INDEX];
    MS_U16 u16H_SizeAfterPreScalingAlign = 0;

    memset(astCurFRCTable, 0, sizeof(SC_FRC_SETTING)*MAX_FRC_TABLE_INDEX);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_SC_GetCurrentFRCTable(astCurFRCTable);

    if(pSrcInfo == NULL)
    {
        MS_ASSERT(0);
        return;
    }
    u16H_SizeAfterPreScalingAlign = (pSrcInfo->u16H_SizeAfterPreScaling + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
#if (HW_DESIGN_4K2K_VER == 5)
    if(SC_R2BYTEMSK(0, REG_SC_BK20_01_L, 0x1) == 0)
    {
        // Clippers MVOP directly bypass to HVSP
        // Not using XC FB
        return;
    }
#endif
#if (HW_DESIGN_4K2K_VER == 7)
    if(Hal_SC_get_fbl(pInstance))
    {
        // HW_DESIGN_4K2K_VER == 7(Kano), 4K2K input must force FBL
        // So do not use XC FB
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"HW_DESIGN_4K2K_VER == 7, 4K2K in, do not set XC FB\n");
        return;
    }
#endif

#ifndef DISABLE_3D_FUNCTION
    if(MDrv_SC_3D_IsDoubleAlign(pInstance))
    {
        u8Alignment = 2 * OFFSET_PIXEL_ALIGNMENT - 1;
    }
#endif
    //when mirror, need use 3 or more frames mode for 24p video
#ifdef UFO_XC_FB_LEVEL
    if(IsVMirrorMode(eWindow)
        && (pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FB)
        && (!pSrcInfo->bInterlace)
    )
#else
    if(IsVMirrorMode(eWindow) && (!pSrcInfo->bFBL)
         && (!pSrcInfo->bR_FBL) && (!pSrcInfo->bInterlace))
#endif
    {
        if(MDrv_SC_IsForceFreerun(pInstance)) //when mirror+freerun, all progresive video timing need go 3 frame mode
        {
#if (HW_DESIGN_3D_VER > 2)
#ifndef DISABLE_3D_FUNCTION
            if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
            {
                u16FrameCount = 6; //per cc, input 3d:framealternative will double data at dnr
            }
            else
#endif
#endif
            {
                u16FrameCount = 3;
            }

            MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                           u16FrameCount,
                                                           pSrcInfo->u16V_SizeAfterPreScaling);
            if((u16H_SizeAfterPreScalingAlign > u32TempWidth)&&(u32TempWidth!=0))
            {
                pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~u8Alignment;
            }
        }
        else if((pSrcInfo->u16InputVFreq > 235) && (pSrcInfo->u16InputVFreq < 245))
        {
            if((astCurFRCTable[1].u8FRC_Out % astCurFRCTable[1].u8FRC_In) != 0)// not 1:1,1:2 frc
            {
#if (HW_DESIGN_3D_VER > 2)
#ifndef DISABLE_3D_FUNCTION
                if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                {
                    u16FrameCount = 6; //per cc, input 3d:framealternative will double data at dnr
                }
                else
#endif
#endif
                {
                    u16FrameCount = 3;
                }

                MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                               u16FrameCount,
                                                               pSrcInfo->u16V_SizeAfterPreScaling);
                if((u16H_SizeAfterPreScalingAlign > u32TempWidth)&&(u32TempWidth!=0))
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~u8Alignment;
                }
            }
        }
    }

    //when pip/pop + no mirror, need use 3 or more frames mode(for interlace, 6 fields or above)
    //when pip/pop + mirror, need use 4 or more frames mode(for interlace, 8 fields or above)
    if((eWindow == SUB_WINDOW)
#ifndef DISABLE_3D_FUNCTION
        && (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE)
#endif
        )
    {
        if(IsVMirrorMode(eWindow))
        {
            u16FrameCount = 4;
        }
        else
        {
            u16FrameCount = 3;
        }

        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"force to %u or more frame mode for pip/pop\n", u16FrameCount);
        {
            MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, SUB_WINDOW,
                                                           u16FrameCount,
                                                           pSrcInfo->u16V_SizeAfterPreScaling);
            if((u16H_SizeAfterPreScalingAlign > u32TempWidth)&&(u32TempWidth!=0))
            {
                pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~u8Alignment;
            }
        }
    }

    if(!MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow))
    {
        if((!IsVMirrorMode(eWindow)) && (!pSrcInfo->bInterlace))
        {
            //UC case, p mode need at least 3 frame mode
            u16FrameCount = 3;

            MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                           u16FrameCount,
                                                           pSrcInfo->u16V_SizeAfterPreScaling);
            if((u16H_SizeAfterPreScalingAlign > u32TempWidth)&&(u32TempWidth!=0))
            {
                pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Warning: Driver do more H-prescaling to keep 3 frame mode for UC!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
        }
        else if(IsVMirrorMode(eWindow) && (!pSrcInfo->bInterlace)
                && (pSrcInfo->u16InputVFreq > 235) && (pSrcInfo->u16InputVFreq < 245)
                && ((astCurFRCTable[1].u8FRC_Out % astCurFRCTable[1].u8FRC_In) != 0))
        {
            //24p mirror UC case, need at least 4 frame number to avoid tearing
            if(MDrv_SC_GetScmiV1Speical4frame(pInstance, MDrv_XC_GetPQSuggestedFrameNum(pInstance, eWindow)))
            {
                u16FrameCount = 8;//a7p 4 frame mode: store 8 frame in memory actually
            }
            else
            {
                u16FrameCount = 4;
            }

            MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, MAIN_WINDOW,
                                                           u16FrameCount,
                                                           pSrcInfo->u16V_SizeAfterPreScaling);
            if((u16H_SizeAfterPreScalingAlign > u32TempWidth)&&(u32TempWidth!=0))
            {
                pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~(OFFSET_PIXEL_ALIGNMENT -1);
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                printf("!!Warning: Driver do more H-prescaling to keep 4 frame mode for UC 24p mirror!!\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }
        }
    }

    //if input vfreq > panel can support vfreq, need go 3 or more frame mode, or else it will be tearing
    if(((pSrcInfo->u16InputVFreq > 600 + VFREQDELTA) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq <= 600))
        || ((pSrcInfo->u16InputVFreq > 1200 + VFREQDELTA) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq <= 1200)))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"force to 3 or more frame mode for input vfreq > panel default vfreq\n");
        {
            MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, eWindow,
                                                           3,
                                                           pSrcInfo->u16V_SizeAfterPreScaling);
            if((u16H_SizeAfterPreScalingAlign > u32TempWidth)&&(u32TempWidth!=0))
            {
                pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~u8Alignment;
            }
        }
    }
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE

    // only lock frequancy, video tearing in 2 frame mode, change to 3 frame mode in 3D mode for RD'advice
    if(MDrv_XC_FPLL_IsSupportLockFreqOnly(pInstance) && (!pSrcInfo->bInterlace))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"force to 3 or more frame mode for only lock freq in 3D mode\n");

        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, eWindow,
                                                       3,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
        if(u16H_SizeAfterPreScalingAlign > u32TempWidth)
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~u8Alignment;
        }
    }
#endif
    // make sure at least 2 frame buffer can fit-in the mmap, specially for small xc buffer size, like 6M
    // we need to prescaling H for it.
    {
#ifdef ENABLE_RFBL_Y8M4_SUPPORTED

        if(gSrcInfo[eWindow].u8BitPerPixel == RFBL_Y8M4_Pixel_Bits)   //RFBL_Y8M4  mode
        {
            u16FrameCount = 1;
        }
        else
        {
            u16FrameCount = 2;
        }
#else

#if (HW_DESIGN_4K2K_VER == 7)   //Kano Patch
        //Set the frame is 4 on Kano XC1 with Rx in, because FRC case
        if((E_XC_DEVICE1 == psXCInstPri->u32DeviceID)&&
            (IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType)
            || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType)))
        {
            u16FrameCount = 4;
        }
        else
        {
            u16FrameCount = 2;
        }
#else
        u16FrameCount = 2;
#endif
#endif


        MS_U32 u32TempWidth = MDrv_XC_GetAvailableSize(pInstance, eWindow,
                                                       u16FrameCount,
                                                       pSrcInfo->u16V_SizeAfterPreScaling);
#ifdef UFO_XC_FB_LEVEL
        if((u16H_SizeAfterPreScalingAlign > u32TempWidth) && (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FB))
#else
        if((u16H_SizeAfterPreScalingAlign > u32TempWidth) && !gSrcInfo[eWindow].bFBL)
#endif
        {
            if(u32TempWidth!=0)
                pSrcInfo->u16H_SizeAfterPreScaling = u32TempWidth & ~u8Alignment;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"prescaling H to (%u), to make sure 2 frames can be fit in xc buffer size\n",
                         pSrcInfo->u16H_SizeAfterPreScaling);
        }
    }
}

extern MS_BOOL MApi_PNL_GetOutputInterlaceTiming(void);

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
//DS HDMI policy
//Base on HDMI policy, do pre-scaling to reduce video quality
#define Min(x, y) x<y ? x:y
static MS_U8 _Mdrv_XC_PolicyMapping(MS_U16 u16CodedWidth, MS_U32 u32HDMIPolicyInfo)
{
    MS_U8 u8Policy = 0;
    if(u16CodedWidth >= 3840) //4K2K
    {
        u8Policy = (MS_U8)((u32HDMIPolicyInfo & 0xff000000) >> 24);
    }
    else if(u16CodedWidth >= 1920) //FHD
    {
        u8Policy = (MS_U8)((u32HDMIPolicyInfo & 0x00ff0000) >> 16);
    }
    else if(u16CodedWidth >= 1280) //HD
    {
        u8Policy = (MS_U8)((u32HDMIPolicyInfo & 0x0000ff00) >> 8);
    }
    else //SD
    {
        u8Policy = (MS_U8)((u32HDMIPolicyInfo & 0x000000ff));
    }
    return u8Policy;
}

static MS_U16 _Mdrv_XC_HPreScalingPolicy(MS_U8 u8Policy, MS_U16 u16HSizeAfterPreScaling)
{
    MS_U16 u16HPreScaling = u16HSizeAfterPreScaling;
    switch(u8Policy)
    {
        case 0: //Do nothing
        case 1: //Blue Screen
            break;
        case 2: //SD
            u16HPreScaling = Min(u16HSizeAfterPreScaling, 720);
            break;
        case 3: //HD
            u16HPreScaling = Min(u16HSizeAfterPreScaling, 1280);
            break;
        case 4: //FHD
            u16HPreScaling = Min(u16HSizeAfterPreScaling, 1920);
            break;
        case 5: //4K2K
            u16HPreScaling = Min(u16HSizeAfterPreScaling, 3840);
            break;
        default:
            printf("no this policy type\n");
    }
    return u16HPreScaling;
}

static MS_U16 _Mdrv_XC_VPreScalingPolicy(MS_U8 u8Policy, MS_U16 u16VSizeAfterPreScaling)
{
    MS_U16 u16VPreScaling = u16VSizeAfterPreScaling;
    switch(u8Policy)
    {
        case 0: //Do nothing
        case 1: //Blue Screen
            break;
        case 2: //SD
            u16VPreScaling = Min(u16VSizeAfterPreScaling, 576);
            break;
        case 3: //HD
            u16VPreScaling = Min(u16VSizeAfterPreScaling, 720);
            break;
        case 4: //FHD
            u16VPreScaling = Min(u16VSizeAfterPreScaling, 1080);
            break;
        case 5: //4K2K
            u16VPreScaling = Min(u16VSizeAfterPreScaling, 2160);
            break;
        default:
            printf("no this policy type\n");
    }
    return u16VPreScaling;
}
#endif

void MDrv_SC_set_prescaling_ratio(void *pInstance, INPUT_SOURCE_TYPE_t enInputSourceType, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32TmpPre=0;
    MS_U16 u16ScaleSrc = 0, u16ScaleDst = 0;
    MS_BOOL bEnPreDown = 0, bAdvMode = 0 , bBilinearMode = 0, bPQSupportBilinear = 0;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U16 u16SrcHWidth = 0;
    MS_BOOL bForcePreVScalingDown = FALSE;
    MS_BOOL bForcePreHScalingDown = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 bFixPreVRatio = 0;//FALSE;
    MS_U16 u16AutoPrescalingDst = 0;
    MS_BOOL bHSDSkipInQmap = FALSE;
    MS_U64 u64_result = 0;

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
    //DS HDMI policy
    MS_U8 u8Policy;
    MS_U32 u32HDMIPolicyInfo;
#endif

    // PQ related.
    PQ_HSD_SAMPLING_INFO stHSDSamplingInfo = {0,0};
    MS_U32 _u32HSD_Ratio[9] = {0x100000, 0x120000, 0x140000, 0x160000, 0x180000, 0x1A0000, 0x1C0000, 0x1E0000, 0x200000};
    MS_U32 _u32HSD_Out[9] = {1000, 888, 800, 728, 666, 616, 572, 534, 500};

    pXCResourcePrivate->stdrvXC_Scaling._bVAutoFitPrescalingEna = FALSE;
    pXCResourcePrivate->stdrvXC_Scaling._bHAutoFitPrescalingEna = FALSE;
    pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually = FALSE;

    // check HW limitation of post-scaling and return TRUE/FALSE flag of force H/V pre-scaling.
    Hal_XC_IsForcePrescaling(pInstance, pSrcInfo, &bForcePreVScalingDown, &bForcePreHScalingDown, eWindow);

    if ((eWindow == SUB_WINDOW) &&
#ifndef DISABLE_3D_FUNCTION
        (MDrv_XC_Get_3D_Output_Mode(pInstance)==E_XC_3D_OUTPUT_MODE_NONE) &&
#endif
         bInterlace &&
        (pSrcInfo->stCapWin.height > 1000) &&
        (pSrcInfo->stCapWin.height > 2*(pSrcInfo->stDispWin.height)))
    {
        // very special case: for PIP 1080i V scaling down < 50% case,
        // per COLOr team request, just VSD 50% at HD sub case at pre-scaling for better quality
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "SUB + 1080i + scaling<0.5 case, ForcePreVScalingDown \n");
        bForcePreVScalingDown = TRUE;
        bFixPreVRatio = 2;
    }

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
    u32HDMIPolicyInfo = Hal_SC_get_HDMIpolicy(pInstance);
    u8Policy = _Mdrv_XC_PolicyMapping(pSrcInfo->stCapWin.width, u32HDMIPolicyInfo);

    if(u8Policy != 0)
    {
        if(u8Policy == 1)
        {
            Hal_SC_set_main_black_screen(pInstance, TRUE);
        }
        else
        {
            Hal_SC_set_main_black_screen(pInstance, FALSE);
        }
    }
#endif
    //-----------------------------------------
    // Vertical
    //-----------------------------------------
    if (pSrcInfo->Status2.bPreVCusScaling == TRUE)
    {
        if((MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
        {
            // If 4k resolution scaling down to less than original height, force V pre-scaling
#ifdef UFO_XC_FB_LEVEL
            if(((pSrcInfo->stCapWin.height >= 2160) && (pSrcInfo->stCropWin.height >= 2160) && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height))
                && ((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FB)
                  || !(gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode) || !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160))
            )
#else
            if(((pSrcInfo->stCapWin.height >= 2160) && (pSrcInfo->stCropWin.height >= 2160) && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height)) && !(((gSrcInfo[eWindow].bFBL) || (gSrcInfo[eWindow].bR_FBL)) && gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160)))
#endif
            {
                u16ScaleSrc = pSrcInfo->stCapWin.height;
                u16ScaleDst = pSrcInfo->stDispWin.height;
            }
#ifdef VDEC_4K2K_PATCH_MODIFY_INFO
            else if((pSrcInfo->stCapWin.height >= 2160 && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height)) && ((gSrcInfo[eWindow].bFBL) || (gSrcInfo[eWindow].bR_FBL)) && gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160))
            {
                u16ScaleSrc = pSrcInfo->stCapWin.height;
                u16ScaleDst = pSrcInfo->stDispWin.height;
            }
#endif
            else
            {
                u16ScaleSrc = pSrcInfo->stCapWin.height;
                u16ScaleDst = pSrcInfo->stCapWin.height;//disable vertical prescaling
#ifdef VIDEO_4K_I_PATCH
                if((pSrcInfo->stCapWin.height > VIDEO_4K_I_BIG_HEIGHT)&(pSrcInfo->bInterlace))
                {
                    u16ScaleDst = (pSrcInfo->stDispWin.height);
                }
#endif
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s,%5d] [DS] [PRE] eWindow[%u] VSrc=%u, VDst=%u, bBilinear=%u  \n",__func__,__LINE__,eWindow , u16ScaleSrc, u16ScaleDst, bBilinearMode);
        }
        else
        {
            u16ScaleSrc = pSrcInfo->Status2.u16PreVCusScalingSrc;
            u16ScaleDst = pSrcInfo->Status2.u16PreVCusScalingDst;
            // Error handling for Refined Capture Height not same as Customer Requested Prescaling V Source size
            // For MFTEVENTFT-19684
            if ((pSrcInfo->Status2.u16PreVCusScalingSrc != pSrcInfo->stCapWin.height) && (pSrcInfo->Status2.u16PreVCusScalingSrc - pSrcInfo->stCapWin.height == 2))
            {
                u16ScaleSrc = pSrcInfo->stCapWin.height;
            }

            // Napoli FHD MM dual decode with DS PSD issue patch
            if(!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC &&
                    MDrv_XC_Is_SubWindowEanble(pInstance) &&
                    (eWindow == MAIN_WINDOW) &&
                    ((pSrcInfo->stDispWin.width == pSrcInfo->stCropWin.width/2) &&
                    (pSrcInfo->stDispWin.height == pSrcInfo->stCropWin.height/2))
            )
            {
                u16ScaleDst = pSrcInfo->stDispWin.height;
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[PRE]eWindow[%u] DualDecode PATCH: VSrc=%u, VDst=%u\n",eWindow , u16ScaleSrc, u16ScaleDst);
            }
        }
    }
    else
    {
        if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
        {
            // If 4k resolution scaling down to less than original height, force V pre-scaling
#ifdef UFO_XC_FB_LEVEL
            if((pSrcInfo->stCapWin.height >= 2160 && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height))
                && ((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FB)
                  || !(gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode) || !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160))
            )
#else
            if((pSrcInfo->stCapWin.height >= 2160 && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height)) && !(((gSrcInfo[eWindow].bFBL) || (gSrcInfo[eWindow].bR_FBL)) && gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160)))
#endif
            {
                u16ScaleSrc = pSrcInfo->stCapWin.height;
                u16ScaleDst = pSrcInfo->stDispWin.height;
            }
            else
            {
                u16ScaleSrc = pSrcInfo->stCapWin.height;
                u16ScaleDst = pSrcInfo->stCapWin.height;//disable vertical prescaling
            }

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s,%5d] [DS] [PRE] eWindow[%u] VSrc=%u, VDst=%u, bBilinear=%u  \n",__func__,__LINE__,eWindow , u16ScaleSrc, u16ScaleDst, bBilinearMode);
        }
        else
        {
            // Put full image into memory as default
            u16ScaleSrc = u16ScaleDst = pSrcInfo->stCapWin.height;

#ifdef UFO_XC_FB_LEVEL
            if (pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FB)
#else
            if (!pSrcInfo->bFBL )
#endif
            {
                // Nine Lattice case
                if(pSrcInfo->bDisplayNineLattice)
                {
                    u16ScaleDst = pSrcInfo->stDispWin.height;
                }
                else
                {
                    if (bForcePreVScalingDown)
                    {
                        u16ScaleDst = MDrv_SC_GetAutoPrescalingDst(pSrcInfo, FALSE);
                        if(!pSrcInfo->bInterlace)
                        {
                            pXCResourcePrivate->stdrvXC_Scaling._bVAutoFitPrescalingEna = TRUE;
                        }
                    }

                }
    #ifndef DISABLE_3D_FUNCTION
                // Adjust for 3D
                u16ScaleDst = MDrv_SC_3D_Adjust_PreVerDstSize(pInstance, enInputSourceType, pSrcInfo, u16ScaleDst, eWindow);
    #endif
            }
        }
    }

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
    if(u8Policy != 0)
    {
        u16ScaleDst = _Mdrv_XC_VPreScalingPolicy(u8Policy, u16ScaleDst);
    }
#endif

#if SUPPORT_SEAMLESS_ZAPPING
    if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
    if( bInterlace )
#endif
    {
        if (u16ScaleDst % 2 )
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"==============================================================\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"======= Pre-scaling V size have to align 2 !!!!!!!!===========\n");
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"==============================================================\n");

            u16ScaleDst = ( u16ScaleDst + 1) & ~0x1;
            if(u16ScaleDst > pSrcInfo->stCapWin.height)
            {
                u16ScaleDst -= 2; //Prescaling size must in range of capture window
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s,%5d] New Dst After refined [%u]\n",__func__,__LINE__,u16ScaleDst);
        }
    }


    // Check prescaling status. Set bPreScalingRatioChanged as true only when source/dest is different with previous source/dest
    // We must update this information before pre-scaling filter.
    //(1. We can skip pre-scaling filter update. 2. We can decide which method we can use for load registers. DS or Menuload.)
    if ( (pXCResourcePrivate->stdrvXC_Scaling._u16VScaleSrc[eWindow] != u16ScaleSrc) || (pXCResourcePrivate->stdrvXC_Scaling._u16VScaleDst[eWindow] != u16ScaleDst) )
    {
        pSrcInfo->bPreScalingRatioChanged = TRUE;

        pXCResourcePrivate->stdrvXC_Scaling._u16VScaleSrc[eWindow] = u16ScaleSrc;
        pXCResourcePrivate->stdrvXC_Scaling._u16VScaleDst[eWindow] = u16ScaleDst;

    }
    else
    {
        pSrcInfo->bPreScalingRatioChanged = FALSE;
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR); //K3_PQ
//#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
//#endif
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl)
        {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW,
                                       E_PQ_IOCTL_PREVSD_BILINEAR,
                                       (void *)&bPQSupportBilinear,
                                       sizeof(bPQSupportBilinear));
#else
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow,
                                       E_PQ_IOCTL_PREVSD_BILINEAR,
                                       (void *)&bPQSupportBilinear,
                                       sizeof(bPQSupportBilinear));
#endif
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex)
        {
            s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow,
                                       E_PQ_IOCTL_PREVSD_BILINEAR,
                                       (void *)&bPQSupportBilinear,
                                       sizeof(bPQSupportBilinear));
        }
    }
#endif

//#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0 || psXCInstPri->u32DeviceID == PQ_XC_ID_1)
//#endif
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable && !MDrv_XC_Is_SupportSWDS(pInstance))
        {
            // dump prescaling filter
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            bBilinearMode = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW,
                                    E_XRULE_VSD,
                                    (u16ScaleSrc != u16ScaleDst ? TRUE : FALSE),
                                    bInterlace,
                                    pSrcInfo->bMemYUVFmt,
                                    u16ScaleSrc,
                                    u16ScaleDst);
#else
            bBilinearMode = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((PQ_WIN)eWindow,
                                    E_XRULE_VSD,
                                    (u16ScaleSrc != u16ScaleDst ? TRUE : FALSE),
                                    bInterlace,
                                    pSrcInfo->bMemYUVFmt,
                                    u16ScaleSrc,
                                    u16ScaleDst);
#endif
        }
        else
        {
            bBilinearMode = 0;
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex)
        {
            // dump prescaling filter
            bBilinearMode = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow,
                                    E_XRULE_VSD,
                                    (u16ScaleSrc != u16ScaleDst ? TRUE : FALSE),
                                    bInterlace,
                                    pSrcInfo->bMemYUVFmt,
                                    u16ScaleSrc,
                                    u16ScaleDst);
        }
        else
        {
            bBilinearMode = 0;
        }
    }
#endif

    if(bPQSupportBilinear == 0)
        bBilinearMode = 0;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR); //K3_PQ

    _XC_ENTRY(pInstance);

#ifdef UFO_XC_FB_LEVEL
    if(((pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FBL) || (pSrcInfo->bR_FBL == E_XC_FB_LEVEL_RFBL_DI))
      && pSrcInfo->bInterlace
    )
#else
    if (pSrcInfo->bFBL && pSrcInfo->bInterlace)
#endif
    {
        //HAL_SC_Enable_VInitFactor(FALSE,eWindow);//Let AP layer to decide enable/dsiable for FBL interlace
        HAL_SC_Set_VInitFactorOne(pInstance, 0x0,eWindow);
    }
    else
    {
        HAL_SC_Enable_VInitFactor(pInstance, FALSE,eWindow);
        HAL_SC_Set_VInitFactorOne(pInstance, 0x0,eWindow);
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE]eWindow[%u] VSrc=%u, VDst=%u, bBilinear=%u\n",eWindow , u16ScaleSrc, u16ScaleDst, bBilinearMode);

    if (u16ScaleSrc > u16ScaleDst) //pre scaling only support scaling down
    {
        MS_U32 u32Value;
#ifndef DISABLE_3D_FUNCTION
        if(MDrv_SC_3D_AdjustPreVerSrcForFramepacking(pInstance, eWindow))
        {
            if(bBilinearMode)
            {
                if (u16ScaleDst == 0 )
                    u32TmpPre = 0;
                else
#ifdef MSOS_TYPE_LINUX_KERNEL
                {
                    V_PreScalingDownRatioBilinear(u16ScaleSrc+1, u16ScaleDst);
                    u32TmpPre = (MS_U32)u64_result;
                }
#else
                    u32TmpPre = V_PreScalingDownRatioBilinear(u16ScaleSrc+1, u16ScaleDst);
#endif
            }
            else
            {
#ifdef MSOS_TYPE_LINUX_KERNEL
                V_PreScalingDownRatio(u16ScaleSrc+1, u16ScaleDst);
                u32TmpPre = (MS_U32)u64_result;
#else
                u32TmpPre = V_PreScalingDownRatio(u16ScaleSrc+1, u16ScaleDst);
#endif
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "3D: special handle for 1080P double HD case, u16ScaleSrc+1=%u, u16ScaleDst=%u\n", u16ScaleSrc+1, u16ScaleDst);
        }
        else
#endif
        {
            if(bBilinearMode)
            {
                if (u16ScaleDst == 0 )
                    u32TmpPre = 0;
                else
                {
                    if (bFixPreVRatio != 0)
                    {
                        //When the V Scaling Down Mode is Bilinear mode
                        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE]eWindow[%u] VSrc=%u, VDst=%u, bBilinear=%u\n",eWindow , u16ScaleSrc, u16ScaleDst, bBilinearMode);
                        u32TmpPre = (MS_U32)bFixPreVRatio*1048576UL;
                        //u16ScaleDst =  ( ( (u16ScaleSrc -1) * 1048576UL ) / (u32TmpPre - 1) ) ;
                        u64_result = (MS_U64)(u16ScaleSrc -1) * 1048576UL;
                        do_div(u64_result,(u32TmpPre - 1));
                        u16ScaleDst = u64_result;
#if SUPPORT_SEAMLESS_ZAPPING
                        if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
                        if( bInterlace )
#endif
                        {
                            if (u16ScaleDst % 2 )
                            {
                                //Pre-scaling V size have to align 2
                                u16ScaleDst = ( u16ScaleDst + 1) & ~0x1;
                            }
                        }
                    }
                    else
                    {
                        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE]eWindow[%u] VSrc=%u,  VDst=%u, bBilinear=%u\n",eWindow , u16ScaleSrc, u16ScaleDst, bBilinearMode);
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PreScalingDownRatioBilinear(u16ScaleSrc, u16ScaleDst);
                        u32TmpPre = (MS_U32)u64_result;
#else
                        u32TmpPre = V_PreScalingDownRatioBilinear(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                }
            }
            else
            {
                if (u16ScaleSrc == 0)
                    u32TmpPre = 0;
                else
                {
                    if (bFixPreVRatio != 0)
                    {
                        //When the V Scaling Down Mode is CB mode
                        u64_result = 1048576ul;
                        do_div(u64_result,bFixPreVRatio);
                        //u32TmpPre = 1048576ul/bFixPreVRatio;
                        u32TmpPre = u64_result;
                        //u16ScaleDst =  ( ( (u32TmpPre -1) *u16ScaleSrc)/1048576ul);
                        u64_result = (MS_U64)(u32TmpPre -1) *u16ScaleSrc;
                        do_div(u64_result,1048576ul);
                        u16ScaleDst = u64_result;
#if SUPPORT_SEAMLESS_ZAPPING
                        if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
                        if( bInterlace )
#endif
                        {
                            if (u16ScaleDst % 2 )
                            {
                                //Pre-scaling V size have to align 2
                                u16ScaleDst = ( u16ScaleDst + 1) & ~0x1;
                            }
                        }
                    }
                    else
                    {
                        #ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PreScalingDownRatio(u16ScaleSrc, u16ScaleDst);;
                        u32TmpPre = (MS_U32)u64_result;
#else
                        u32TmpPre = V_PreScalingDownRatio(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                }
            }
        }
        u32TmpPre &= 0xFFFFFFL;

        // Enable Bilinear mode.
        if (bBilinearMode)
        {
            u32TmpPre |= 0x40000000L;
        }

        u32TmpPre |= 0x80000000L;

        pSrcInfo->bPreV_ScalingDown = TRUE;

        // Error checking
        if (u16ScaleSrc == 0)
            u32Value = 0;
        else
            u32Value = (MS_U32)u16ScaleDst * 0x80000 / u16ScaleSrc;

        // if SC1 is interlace out, two init factor mode is used for P2I.
        // so can not use in this case
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            HAL_SC_Set_VInitFactorTwo(pInstance, u32Value,eWindow);
        }
#if (XC_DEVICE1_IS_INTERLACE_OUT == 0)
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            HAL_SC_Set_VInitFactorTwo(pInstance, u32Value,eWindow);
        }
#endif


        HAL_SC_Set_vsd_output_line_count(pInstance, DISABLE,u16ScaleDst,eWindow);
        HAL_SC_Set_vsd_input_line_count(pInstance, DISABLE,FALSE,0,eWindow);

    }
    else
    {
        pXCResourcePrivate->stdrvXC_Scaling._bVAutoFitPrescalingEna = FALSE;
        if ((eWindow == SUB_WINDOW)
#ifndef DISABLE_3D_FUNCTION
            &&(MDrv_XC_Get_3D_Output_Mode(pInstance)==E_XC_3D_OUTPUT_MODE_NONE)
#endif
        )
        {
           u32TmpPre = Hal_SC_CheckSubWinPreScaling(u16ScaleDst,pSrcInfo->bInterlace);

            if(u32TmpPre == 0)
            {
               pSrcInfo->bPreV_ScalingDown = FALSE;
            }
            else
            {
               pSrcInfo->bPreV_ScalingDown = TRUE;
            }
        }
        else
        {
            u16ScaleDst = u16ScaleSrc;
            u32TmpPre = 0;
            pSrcInfo->bPreV_ScalingDown = FALSE;
            HAL_SC_Set_vsd_output_line_count(pInstance, DISABLE,u16ScaleDst,eWindow);
            HAL_SC_Set_vsd_input_line_count(pInstance, DISABLE,FALSE,0,eWindow);

        }
        // SC1 two init factor mode is used for P2I
        // so can not use in this case
        if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
        {
            HAL_SC_Set_VInitFactorTwo(pInstance, 0x80000, eWindow);
        }
#if (XC_DEVICE1_IS_INTERLACE_OUT == 0)
        else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
        {
            HAL_SC_Set_VInitFactorTwo(pInstance, 0x80000, eWindow);
        }
#endif
    }

     _XC_RETURN(pInstance);

    //store prescaling info here ( Why using u16PreVCusScalingSrc and u16PreVCusScalingDst??)
    pSrcInfo->Status2.u16PreVCusScalingSrc = u16ScaleSrc;
    pSrcInfo->Status2.u16PreVCusScalingDst = u16ScaleDst;
    pSrcInfo->u16V_SizeAfterPreScaling = u16ScaleDst;
    pSrcInfo->u16V_Length = pSrcInfo->u16V_SizeAfterPreScaling;

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "[%s,%5d] [DS] u16V_Length:%d \n",__func__,__LINE__,pSrcInfo->u16V_Length);
#if (HW_DESIGN_4K2K_VER == 4)
    pSrcInfo->u16FRCMV_Length = pSrcInfo->u16V_Length;
#endif
#ifndef DISABLE_3D_FUNCTION
    if (((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_MODE_NONE)
        && (MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) != E_XC_3D_INPUT_NORMAL_2D_HW))
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        || (Hal_SC_3D_IsDualViewMode(pInstance, eWindow))
#endif
     )//hw 2d->3d is not related with sub window, so skip it.
    {
        MDrv_SC_3D_Adjust_FillLBMode(pInstance, eWindow);
    }

    MDrv_SC_3D_SetWriteMemory(pInstance, eWindow);
    MDrv_SC_3D_CloneMainSettings(pInstance, pSrcInfo, eWindow);
    MDrv_SC_3D_Adjust_PreVLength(pInstance, enInputSourceType, pSrcInfo, eWindow);
#endif
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio = u32TmpPre;

    //-----------------------------------------
    // Horizontal
    // HSD rule: Framecount ensure > 3D scaling > customer scaling > QMap HSD sampling > auto fit
    //-----------------------------------------

    // Note!! Formula:  FHD - If  (H Crop size /2 ) > ( H display size ), Must-do H pre-scaling down.

    if (pSrcInfo->bDisplayNineLattice)
    {
        u16SrcHWidth = pSrcInfo->stCapWin.width;
        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Nine Lattice: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
    }
    else
    {
        //for prevent Src == Dst, HCusScaling = TRUE case
        if(pSrcInfo->Status2.u16PreHCusScalingSrc == pSrcInfo->Status2.u16PreHCusScalingDst
            && (!IsSrcTypeDTV(enInputSourceType)))
            //DTV do not execute that only for reduceing side effect. Academically,all source do not execute.
        {
            pSrcInfo->Status2.bPreHCusScaling = FALSE;
        }

        // Custimize
        if(pSrcInfo->Status2.bPreHCusScaling)
        {
            u16SrcHWidth = pSrcInfo->Status2.u16PreHCusScalingSrc;

            // If 4k resolution scaling down to less than original width, force H pre-scaling
            if(((pSrcInfo->stCapWin.width >= 3840) && (pSrcInfo->stCropWin.width >= 3840) && (pSrcInfo->stDispWin.width< pSrcInfo->stCapWin.width)) && !(((gSrcInfo[eWindow].bFBL) || (gSrcInfo[eWindow].bR_FBL)) && gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width < 3840)))
            {
                pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
            }
            else
            {
                if (u16SrcHWidth > pSrcInfo->Status2.u16PreHCusScalingDst)
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->Status2.u16PreHCusScalingDst;
                }
                else
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = u16SrcHWidth;
                }
        #ifndef DISABLE_3D_FUNCTION
                MDrv_SC_3D_Adjust_PreHorDstSize(pInstance, enInputSourceType, pSrcInfo, eWindow);
        #endif
                MDrv_XC_AdjustPrescalingForFrameNum(pInstance, pSrcInfo, eWindow);
                // 3d adjust may make the pre h dst bigger than cus_pre_H, we need adjust it again.
                if (pSrcInfo->u16H_SizeAfterPreScaling > pSrcInfo->Status2.u16PreHCusScalingDst)
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->Status2.u16PreHCusScalingDst;
                }
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Customer H scaling: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);

        }
        else
        {
            if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
            {
                u16SrcHWidth = pSrcInfo->stCapWin.width;

                // If 4k resolution scaling down to less than original height, force V pre-scaling
#ifdef UFO_XC_FB_LEVEL
                if((pSrcInfo->stCapWin.height >= 2160 && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height))
                    && ((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FB)
                      || !gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode
                      || !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160))
                )
#else
                if((pSrcInfo->stCapWin.height >= 2160 && (pSrcInfo->stDispWin.height< pSrcInfo->stCapWin.height)) && !(((gSrcInfo[eWindow].bFBL) || (gSrcInfo[eWindow].bR_FBL)) && gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height < 2160)))
#endif
                {
                    // Need to force H pre-scaling if already V pre-scaling
                    // Force H pre-scaling down to less than 1280px
                    if(pSrcInfo->stDispWin.width > 1280)
                    {
                        pSrcInfo->u16H_SizeAfterPreScaling = 1280;
                    }
                    else
                    {
                        if(pSrcInfo->stCapWin.width > pSrcInfo->stDispWin.width)
                        {
                            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;

                        }
                        else
                        {
                            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
                        }
                    }
                }
                else
                {
                	  if(!(gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width < 3840)))
                	  {
		                    if(pSrcInfo->stCapWin.width*6 > pSrcInfo->stDispWin.width*10)
		                    {
		                        pSrcInfo->u16H_SizeAfterPreScaling = (pSrcInfo->stCapWin.width/4 + 1) & ~0x01;
		                    }
		                    else
		                    {
		                        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;//disable horizontal prescaling
		                    }
                    }
                    else
                    {
		                pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;//disable horizontal prescaling
                    }
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] [DS]  eWindow[%u] HSrc=%u, HDst=%u\n",__func__,__LINE__, eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
            }
            else
            {
                u16SrcHWidth = pSrcInfo->stCapWin.width;
                pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;

                // this DTV flow don't obey the HSD rule, but HDuplicate won't affect QMap non-prescaling case,
                // because the HDuplicate condition is no bigger than 704, refer to MVOP_AutoGenMPEGTiming()
                // need to refine later
                if(IsSrcTypeDTV(enInputSourceType) && pSrcInfo->bHDuplicate) // for better quality
                {
                    MS_U16 capwidth = pSrcInfo->stCapWin.width / 2;
                    if(eWindow == SUB_WINDOW)
                    {
                        if (capwidth > pSrcInfo->stDispWin.width && pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width < 1800)
                            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width;
                        else if(capwidth > pSrcInfo->stDispWin.width)
                            pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
                    }
                    else
                    {
                        pSrcInfo->u16H_SizeAfterPreScaling = capwidth;
                    }
                    pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually = TRUE;
                }
            //!!NOTE START:
            //if QMAP support HSD Sampling, this item need to disable and the scaling size will control by QMAP
            //If we want to follow the HSD rule, we cannot enable ENABLE_YPBPR_PRESCALING_TO_ORIGINAL and
            // ENABLE_VD_PRESCALING_TO_DOT75
            #if (ENABLE_YPBPR_PRESCALING_TO_ORIGINAL)
                else if(IsSrcTypeYPbPr(enInputSourceType) && pSrcInfo->bHDuplicate)
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width / 2;
                    pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually = TRUE;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"ENABLE_YPBPR_PRESCALING_TO_ORIGINAL: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
                }
            #endif
            //!!NOTE END
            #if (ENABLE_VD_PRESCALING_TO_DOT75)
                else if(IsSrcTypeDigitalVD(enInputSourceType))
                {

                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stCapWin.width * 3 / 4;
                    pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually = TRUE;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"ENABLE_VD_PRESCALING_TO_DOT75: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
                }
            #endif
                else
                {
                    // H scaling controlled by Qmap.
    //    #ifdef ENABLE_TV_SC2_PQ
                    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
    //    #endif
                    {
                        if((eWindow == MAIN_WINDOW) && (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl != NULL))
                        {
                            MS_BOOL bHSDRet = FALSE;
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                            bHSDRet = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW, E_PQ_IOCTL_HSD_SAMPLING, &stHSDSamplingInfo, sizeof(PQ_HSD_SAMPLING_INFO));
#else
                            bHSDRet = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow, E_PQ_IOCTL_HSD_SAMPLING, &stHSDSamplingInfo, sizeof(PQ_HSD_SAMPLING_INFO));
#endif
                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"stHSDSamplingInfo.u32ratio=%tu,bHSDRet=%x\n", (ptrdiff_t)stHSDSamplingInfo.u32ratio, bHSDRet);
                            // Using ratio from Qmap only in acceptable post-scaling
                            if(stHSDSamplingInfo.u32ratio && bHSDRet == TRUE)
                            {
                                switch(stHSDSamplingInfo.u32ratio)
                                {
                                    default:
                                    case 1000:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 0;
                                        break;

                                    case 875:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 1;
                                        break;

                                    case 750:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 2;
                                        break;

                                    case 625:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 3;
                                        break;

                                    case 500:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 4;
                                        break;

                                    case 375:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 5;
                                        break;

                                    case 250:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 6;
                                        break;

                                case 125:
                                    pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 7;
                                    break;

                                    case 1:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 8;
                                        break;
                                }
                                pSrcInfo->u16H_SizeAfterPreScaling = (MS_U16)(((MS_U32)pSrcInfo->stCapWin.width * _u32HSD_Out[pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx])/1000);
                                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"pSrcInfo->u16H_SizeAfterPreScaling=%u\n", pSrcInfo->u16H_SizeAfterPreScaling);
                                MDrv_SC_SetPQHSDFlag(pInstance, TRUE);
                            }
                            else
                            {
                                //cannot retrieve HSD_Sampling or HSD_Y and HSD_C is true in skip rule
                                //then we should use HSDRule to dump hsd_y, hsd_c
                                bHSDSkipInQmap = TRUE;
                            }
                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"PQ HSD_SAMPLING: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);

                        }
                    }
#ifdef ENABLE_TV_SC2_PQ
                    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
                    {
                        if((eWindow == MAIN_WINDOW) && (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex != NULL))
                        {
                            MS_BOOL bHSDRet = FALSE;
                            bHSDRet = s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow, E_PQ_IOCTL_HSD_SAMPLING, &stHSDSamplingInfo, sizeof(PQ_HSD_SAMPLING_INFO));

                            // Using ratio from Qmap only in acceptable post-scaling
                            if(stHSDSamplingInfo.u32ratio && bHSDRet == TRUE)
                            {
                                switch(stHSDSamplingInfo.u32ratio)
                                {
                                    default:
                                    case 1000:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 0;
                                        break;

                                    case 875:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 1;
                                        break;

                                    case 750:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 2;
                                        break;

                                    case 625:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 3;
                                        break;

                                    case 500:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 4;
                                        break;

                                    case 375:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 5;
                                        break;

                                    case 250:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 6;
                                        break;

                                    case 125:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 7;
                                        break;

                                    case 1:
                                        pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx = 8;
                                        break;
                                }
                                pSrcInfo->u16H_SizeAfterPreScaling = (MS_U16)(((MS_U32)pSrcInfo->stCapWin.width * _u32HSD_Out[pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx])/1000);
                                MDrv_SC_SetPQHSDFlag(pInstance, TRUE);
                            }
                            else
                            {
                                //cannot retrieve HSD_Sampling or HSD_Y and HSD_C is true in skip rule
                                //then we should use HSDRule to dump hsd_y, hsd_c
                                bHSDSkipInQmap = TRUE;
                            }

                            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"PQ HSD_SAMPLING: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);

                        }
                    }
#endif

                    // Scaling donw only when special case.
                    if ((!MDrv_SC_GetPQHSDFlag(pInstance))&& bForcePreHScalingDown)
                    {
                        pSrcInfo->u16H_SizeAfterPreScaling = MDrv_SC_GetAutoPrescalingDst(pSrcInfo, TRUE);
                        u16AutoPrescalingDst = pSrcInfo->u16H_SizeAfterPreScaling;
                        pXCResourcePrivate->stdrvXC_Scaling._bHAutoFitPrescalingEna = TRUE;
                        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Driver logic, auto fit: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
                    }

                    if (eWindow == SUB_WINDOW )
                    {
                        // Notice!!!! Sub win only have 960 pixels.
                        // If we use 1280 pixel for Sub Win, Main win has 1280 pixels remain. (Total line buffer : 1920 + 960 )
#if (HW_DESIGN_3D_VER == 4)
                        if(!((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)==E_XC_3D_INPUT_FRAME_PACKING)&&
                           (MDrv_XC_Get_3D_Output_Mode(pInstance)==E_XC_3D_OUTPUT_TOP_BOTTOM)))
#endif
                        {
                            if ( pSrcInfo->u16H_SizeAfterPreScaling > SUB_MST_LINE_BFF_MAX )
                            {
                                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE] SUB LINE BFF LIMIT : HSrc=%u, HDst=%lu\n",pSrcInfo->u16H_SizeAfterPreScaling, SUB_MST_LINE_BFF_MAX);
                                pSrcInfo->u16H_SizeAfterPreScaling = SUB_MST_LINE_BFF_MAX ;
                            }
                            // Avoid Post HSD in PIP/POP case
                            if(pSrcInfo->u16H_SizeAfterPreScaling >= pSrcInfo->stDispWin.width)
                            {
                                #if FRC_INSIDE
                                if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC) &&
                                    ((pSrcInfo->stCropWin.x != 0) || (pSrcInfo->stCropWin.width != pSrcInfo->stCapWin.width)))
                                {
                                    // Consider Crop case,  the post HSD clk is not enough for FRC enable case
                                    pSrcInfo->u16H_SizeAfterPreScaling  = ((pSrcInfo->stCapWin.width*pSrcInfo->stDispWin.width)+(pSrcInfo->stCropWin.width/2))/pSrcInfo->stCropWin.width;
                                }
                                else
                                #endif
                                {
                                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
                                }
                                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"To avoid Post HSD, HDst=%u\n",pSrcInfo->u16H_SizeAfterPreScaling);
                            }
                        }
                    }
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE]Force Prescaling Down : %d  _bPQSetPreScalingrate : %d \n",bForcePreHScalingDown, MDrv_SC_GetPQHSDFlag(pInstance));
                }

                MS_U16 u16OriHSize = pSrcInfo->u16H_SizeAfterPreScaling;
#ifndef DISABLE_3D_FUNCTION
                MDrv_SC_3D_Adjust_PreHorDstSize(pInstance, enInputSourceType, pSrcInfo, eWindow);
#endif
                MDrv_XC_AdjustPrescalingForFrameNum(pInstance, pSrcInfo, eWindow);

                if(u16OriHSize != pSrcInfo->u16H_SizeAfterPreScaling)
                {
                    pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually = TRUE;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"xc: 3D has changed the H prescaling setting, need to skip PQ HSD_Sampling/HSD_Y/HSD_C loading\n");
                }

                //for 4096 input case, force prescaling to display window width
                if((pSrcInfo->u16H_SizeAfterPreScaling > WIDTH_4K2K)
                    && (pSrcInfo->u16H_SizeAfterPreScaling > pSrcInfo->stDispWin.width))
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->stDispWin.width;
                }
            }
        }
    }

#ifdef UFO_XC_SUPPORT_HDMI_DOWNSCALE_OUTPUT_POLICY
    if(u8Policy != 0)
    {
        pSrcInfo->u16H_SizeAfterPreScaling = _Mdrv_XC_HPreScalingPolicy(u8Policy, pSrcInfo->u16H_SizeAfterPreScaling);
    }
#endif
    //Setting Line Buffer Start offset for Sub Win
    if(pSrcInfo->u16H_SizeAfterPreScaling & BIT(0))
    {
        //YUV422, h size must be even to prevent showing of garbage at the end of line
        if(pSrcInfo->u16H_SizeAfterPreScaling < pSrcInfo->stCapWin.width)
        {
            if(MDrv_SC_GetPQHSDFlag(pInstance) == TRUE)
            {
                pSrcInfo->u16H_SizeAfterPreScaling = (pSrcInfo->u16H_SizeAfterPreScaling - 1) & ~0x1;
            }
            else
            {
                pSrcInfo->u16H_SizeAfterPreScaling = (pSrcInfo->u16H_SizeAfterPreScaling + 1) & ~0x1;
            }
        }
        else
        {
            pSrcInfo->u16H_SizeAfterPreScaling = u16SrcHWidth;
        }
    }

    _XC_ENTRY(pInstance);
    if(eWindow==MAIN_WINDOW)
    {
#if (HW_DESIGN_3D_VER == 4)

        if((MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)==E_XC_3D_INPUT_FRAME_PACKING)&&
           (MDrv_XC_Get_3D_Output_Mode(pInstance)==E_XC_3D_OUTPUT_TOP_BOTTOM))
            HAL_SC_SetSubLineBufferOffset(pInstance, 0x00);
        else
        {
            // Main-it-on-top is enabled.
            if ( pXCResourcePrivate->stdrvXC_MVideo_Context.bMainIsOnTop )
            {
                // If Top window (a window on the top) turn on 8 line (reg_di8lb_en_f2), HW will enable reg_di8lb_en_f1 together internally(HW¼g¦º).
                // That means we only have 1920 pixels line buffer (the total length of 8 line UCNR line buffer is 1920 only.) for two windows.
                // For convenience, we separate 1920 to both window. 960 pixel for each.
                if ( !MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow) )
                {
                    if (pSrcInfo->u16H_SizeAfterPreScaling > 960)
                    {
                        pSrcInfo->u16H_SizeAfterPreScaling = 960;
                    }
                    // When Main-is-top set. Main Line buffer is blong to sub window.
                    HAL_SC_SetMainLineBufferOffset(pInstance, 0x3C0);
                    // When Main-is-top set. Sub Line buffer is blong to Main window.
                    HAL_SC_SetSubLineBufferOffset(pInstance, 0x00);
                }
            }
            else
            {
                HAL_SC_SetMainLineBufferOffset(pInstance, 0x00);
                HAL_SC_SetSubLineBufferOffset(pInstance, SCALER_LINE_BUFFER_MAX + SUB_MAIN_LINEOFFSET_GUARD_BAND);
            }
        }
#else
        // Main-is-on-top is enabled.
        if ( pXCResourcePrivate->stdrvXC_MVideo_Context.bMainIsOnTop )
        {
            // If Top window (a window on the top) turn on 8 line (reg_di8lb_en_f2), HW will enable reg_di8lb_en_f1 together internally(HW¼g¦º).
            // That means we only have 1920 pixels line buffer (the total length of 8 line UCNR line buffer is 1920 only.) for two windows.
            // For convenience, we separate 1920 to both window. 960 pixel for each.
            if ( !MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow) )
            {
                if (pSrcInfo->u16H_SizeAfterPreScaling > 960)
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = 960;
                }
                // When Main-is-top set. Main Line buffer is blong to sub window.
                HAL_SC_SetMainLineBufferOffset(pInstance, 0x3C0);
                // When Main-is-top set. Sub Line buffer is blong to Main window.
                HAL_SC_SetSubLineBufferOffset(pInstance, 0x00);
            }
        }
        else
        {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            //FIX ME:why need this?
            if (psXCInstPri->u32DeviceID == 0)
#endif
            {
                HAL_SC_SetMainLineBufferOffset(pInstance, 0x00);
            }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            else
            {
                HAL_SC_SetMainLineBufferOffset(pInstance, 0x01);
            }
#endif
            HAL_SC_SetSubLineBufferOffset(pInstance, SCALER_LINE_BUFFER_MAX + SUB_MAIN_LINEOFFSET_GUARD_BAND);
        }
#endif
    }
    else // eWindow == SUB_WINDOW
    {
        // POP case
        if ((gSrcInfo[MAIN_WINDOW].stDispWin.width == pSrcInfo->stDispWin.width)
            && (gSrcInfo[MAIN_WINDOW].stDispWin.width + pSrcInfo->stDispWin.width == pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width))
        {
            // UCNR on
            if ( !MDrv_XC_Get_OPWriteOffEnable(pInstance, MAIN_WINDOW) )
            {
                // reg_di8lb_en_f2 ON and it is in POP mode
                // For old chip:
                //     - main and sub share 1920 pixel line buffer in length
                //     - each window has 1920/2 = 960 = 0x3C0, sub window starts at MST_LINE_BFF_MAX/2
                // For new chip: like Nike, Napoli
                //     - main and sub share 1920 + 960 pixel line buffer in length
                //     - sub window starts at 1920 = MST_LINE_BFF_MAX/2
                HAL_SC_SetMainLineBufferOffset(pInstance, 0x000);
                HAL_SC_SetSubLineBufferOffset(pInstance, MST_LINE_BFF_MAX/2);
            }
        }
        else
        {
            HAL_SC_SetMainLineBufferOffset(pInstance, 0x00);
            HAL_SC_SetSubLineBufferOffset(pInstance, SCALER_LINE_BUFFER_MAX + SUB_MAIN_LINEOFFSET_GUARD_BAND);
        }
    }
#ifndef DISABLE_3D_FUNCTION
#if (HW_DESIGN_3D_VER <= 1)
    MDrv_SC_3D_Adjust_SubLineBufferOffset(pInstance, enInputSourceType, pSrcInfo, eWindow);
#endif
#endif
    _XC_RETURN(pInstance);

    if(u16AutoPrescalingDst != pSrcInfo->u16H_SizeAfterPreScaling)
    {
        pXCResourcePrivate->stdrvXC_Scaling._bHAutoFitPrescalingEna = FALSE;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);

    // Check prescaling status. Set bPreScalingRatioChanged as true only when src != dest.
    // We update this information before pre-scaling filter.
    if ( (pXCResourcePrivate->stdrvXC_Scaling._u16HScaleSrc[eWindow] != u16SrcHWidth) || (pXCResourcePrivate->stdrvXC_Scaling._u16HScaleDst[eWindow] != pSrcInfo->u16H_SizeAfterPreScaling) )
    {
        gSrcInfo[eWindow].bPreScalingRatioChanged = TRUE;

        pXCResourcePrivate->stdrvXC_Scaling._u16HScaleSrc[eWindow] = u16SrcHWidth;
        pXCResourcePrivate->stdrvXC_Scaling._u16HScaleDst[eWindow] = pSrcInfo->u16H_SizeAfterPreScaling;
    }
    else
    {
        gSrcInfo[eWindow].bPreScalingRatioChanged |= FALSE;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    // below cases should dump HSDRule:
    // 1. hsd_y, hsd_c skip rule true
    // 2. customer scaling
    // 3. manually change h pre size
    if((bHSDSkipInQmap == TRUE)
        ||(gSrcInfo[eWindow].bHCusScaling)
        ||(gSrcInfo[eWindow].Status2.bPreHCusScaling)
        ||(MDrv_XC_GetHSizeChangeManuallyFlag(pInstance,eWindow)))
    {

//#ifdef ENABLE_TV_SC2_PQ
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_0 || psXCInstPri->u32DeviceID == PQ_XC_ID_1)
//#endif
        {
            if( (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable == NULL)|| MDrv_XC_Is_SupportSWDS(pInstance))
            {
                bAdvMode = 0;
            }
            else
            {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                bAdvMode =
                    s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW,
                                        E_XRULE_HSD,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16SrcHWidth,
                                        pSrcInfo->u16H_SizeAfterPreScaling);
#else
                bAdvMode =
                    s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((PQ_WIN)eWindow,
                                        E_XRULE_HSD,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16SrcHWidth,
                                        pSrcInfo->u16H_SizeAfterPreScaling);
#endif
            }
        }
#ifdef ENABLE_TV_SC2_PQ
        else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex == NULL)
            {
                bAdvMode = 0;
            }
            else
            {
                bAdvMode =
                    s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow,
                                        E_XRULE_HSD,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16SrcHWidth,
                                        pSrcInfo->u16H_SizeAfterPreScaling);
            }
        }
#endif
    }

    //if customer scaling case, ratio and filter is decided by Customer and Xrule
    //if not customer and PQ HSD sampling case, ratio and filter is decided by PQ
    //if not customer and not PQ HSD sampling case, ratio and filter is decided by driver autofit and Xrule
    //Note: if HSD_Y, HSD_C is in PQ skip rule, and the skip is Y, the driver logic will be very carefully.
    //we should treat it as "none PQ setting HSD sampling" case.
    //because HSD filter is related with bAdvMode, which are paired.
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    if((MDrv_SC_GetPQHSDFlag(pInstance)) && (!pSrcInfo->Status2.bPreHCusScaling) && (!pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually))
    {
        bAdvMode = stHSDSamplingInfo.bADVMode;
    }


    // Dixon, FixMe, subwindow 960 no pre-scaling down can't use ADV mode
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if((eWindow == MAIN_WINDOW) && (psXCInstPri->u32DeviceID == PQ_XC_ID_1))
#else
    if(eWindow == SUB_WINDOW)
#endif
    {
        bAdvMode = FALSE;
    }

    //printf("bEnPreDown=%u, bAdvMode=%u\n", (MS_U16)bEnPreDown, (MS_U16)bAdvMode);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE]eWindow[%u] HSrc=%u, HDst=%u, bAdvMode=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling, bAdvMode);

    if (u16SrcHWidth > pSrcInfo->u16H_SizeAfterPreScaling)
    {
        MS_U16 u16div = 1;

        #ifndef SUPPORT_4K2K_PHOTO_ZOOM
        // To avoid overflow when do H_PreScalingDownRatioAdv/H_PreScalingDownRatioAdv_No_Minus1
        if(u16SrcHWidth > 4096)
        {
            while(1)
            {
                MS_U16 u16ret;

                u16div <<= 1;

                if (u16div == 0)
                {
                    u16div = 1;
                    break;
                }

                u16ret = u16SrcHWidth/u16div;

                if(u16ret == 0)
                {
                    u16div = 1;
                    break;
                }
                else if(u16ret< 4096 && u16ret > pSrcInfo->u16H_SizeAfterPreScaling)
                {
                    break;
                }
            }
        }
        #endif

        if((MDrv_SC_GetPQHSDFlag(pInstance)) && (!pSrcInfo->Status2.bPreHCusScaling) && (!pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually))
        {
            //PQ set HPrescaling case, don't use bAdvMode
            u32TmpPre = _u32HSD_Ratio[pXCResourcePrivate->stdrvXC_Scaling._u8HSD_Idx];
        }
        else
        {
            if (bAdvMode)
            { // enable prescaling fitler
#ifdef MSOS_TYPE_LINUX_KERNEL
                H_PreScalingDownRatioAdv((u16SrcHWidth/u16div), pSrcInfo->u16H_SizeAfterPreScaling);
                u32TmpPre = (MS_U32) (u64_result * u16div);
#else
                u32TmpPre = (H_PreScalingDownRatioAdv((u16SrcHWidth/u16div), pSrcInfo->u16H_SizeAfterPreScaling)) * u16div;
#endif
            }
            else
            { // disable prescaling filter, use cb mode scaling ratio
                if (u16SrcHWidth == 0 )
                    u32TmpPre = 0;
                else
#ifdef MSOS_TYPE_LINUX_KERNEL
                {
                    H_PreScalingDownRatio(u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
                    u32TmpPre = (MS_U32)u64_result;
                }
#else
                    u32TmpPre = (H_PreScalingDownRatio(u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling));
#endif
            }
        }
        bEnPreDown = 1;
        u32TmpPre &= 0x7FFFFFL;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Scaling._bHAutoFitPrescalingEna = FALSE;
        pSrcInfo->u16H_SizeAfterPreScaling = u16SrcHWidth;

        if(bAdvMode)
        {
            bEnPreDown = 1;
            u32TmpPre = 0x100000L;
        }
        else
        {
            bEnPreDown = 0;
            u32TmpPre = 0x100000L;
        }

        // Dixon, FixMe, subwindow 960 no pre-scaling down can't turn on scaling and set ratio to 1
        if(eWindow==SUB_WINDOW)
            bEnPreDown = 0;

        // Note: Patch Start --this patch is only for A3 HW issue!!
        #ifdef HW_DISABLE_HSD
        // To fix ATV pink issue, we should turn off pre-hscaling, if it is unnescerry to scaling
        bEnPreDown = 0;
        u32TmpPre = 0x00L;
        #endif
        // Note: Patch End --

    }
    //store prescaling info here
    pSrcInfo->Status2.u16PreHCusScalingSrc = u16SrcHWidth;
    pSrcInfo->Status2.u16PreHCusScalingDst = pSrcInfo->u16H_SizeAfterPreScaling;

    if (bEnPreDown)
    {
        if(bAdvMode)
        {
            u32TmpPre |= 0x40000000L;
        }

        u32TmpPre |= 0x80000000L;
    }

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PreScalingRatio = u32TmpPre;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCMVLen = pSrcInfo->u16FRCMV_Length;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PRE]HRatio=0x%tx, VRatio=0x%tx, u16V_Length = %u\n",
                                       (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PreScalingRatio,
                                       (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio,
                                       pSrcInfo->u16V_Length);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_SC_set_postscaling_ratio(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    #define MAX_PST_SCALING_DOWN_PERCENTAGE_FCLK170M (80)
#ifdef MSOS_TYPE_LINUX_KERNEL
    MS_U64 u64_result = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32TmpPost=0;
    MS_U16 u16ScaleSrc, u16ScaleDst;
    MS_BOOL bInterlace = pSrcInfo->bInterlace, bPstScalingDownCheck;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    //Check pst scaling limit for FULL HD panel, and currently only check FULL HD input

    MS_BOOL bIsInterlace = FALSE;
    bIsInterlace = MApi_PNL_GetOutputInterlaceTiming();

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID)
    {
        bPstScalingDownCheck = ((MDrv_ReadByte(REG_CKG_FCLK) & CKG_FCLK_MASK) == CKG_FCLK_170MHZ) ?
                                (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1920 && pSrcInfo->ScaledCropWin.width > 1280) : FALSE;
    }
#if (HW_DESIGN_4K2K_VER == 4)
    else if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID)
    {
        bPstScalingDownCheck = ((MDrv_ReadByte(REG_CKG_S2_FCLK) & CKG_S2_FCLK_MASK) == CKG_S2_FCLK_172MHZ) ?
                                (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1920 && pSrcInfo->ScaledCropWin.width > 1280) : FALSE;
    }
#endif
    else
    {
        /// others
        bPstScalingDownCheck = FALSE;
    }
    //-----------------------------------------
    // Horizontal
    //-----------------------------------------
    if (pSrcInfo->bHCusScaling == FALSE)
    {
        u16ScaleSrc = pSrcInfo->ScaledCropWin.width;
        u16ScaleDst = pSrcInfo->stDispWin.width;
#ifndef DISABLE_3D_FUNCTION
        u16ScaleSrc = MDrv_SC_3D_Adjust_PostHorSrcSize(pInstance, pSrcInfo, u16ScaleSrc, eWindow);

        u16ScaleDst = MDrv_SC_3D_Adjust_PostHorDstSize(pInstance, pSrcInfo, u16ScaleDst, eWindow);
#endif
    }
    else
    {
        u16ScaleSrc = pSrcInfo->u16HCusScalingSrc;
        u16ScaleDst = pSrcInfo->u16HCusScalingDst;
        //make sure crop window in the range of window after prescaling
        if((u16ScaleSrc > pSrcInfo->u16H_SizeAfterPreScaling) ||
           (u16ScaleSrc + pSrcInfo->ScaledCropWin.x > pSrcInfo->u16H_SizeAfterPreScaling))
        {
            MS_U16 u16PstScaledWidth;
            u16PstScaledWidth = (MS_U32)(u16ScaleDst*(pSrcInfo->u16H_SizeAfterPreScaling - pSrcInfo->ScaledCropWin.x))/u16ScaleSrc;
            //Check if the scaled content can full fill the display window, otherwise garbage will show out.
            if(u16PstScaledWidth < pSrcInfo->stDispWin.width)
            {
                if(u16ScaleSrc > pSrcInfo->u16H_SizeAfterPreScaling)
                {
                    u16ScaleSrc = pSrcInfo->u16H_SizeAfterPreScaling & ~0x01;
                    pSrcInfo->ScaledCropWin.x = 0;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]Limit ScaledCropWin.x to x=%u, HScaleSrc to: Src=%u\n", pSrcInfo->ScaledCropWin.x, u16ScaleSrc);
                }
                else
                {
                    pSrcInfo->ScaledCropWin.x = (pSrcInfo->u16H_SizeAfterPreScaling - u16ScaleSrc) & ~0x01;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]Limit ScaledCropWin.x to x=%u\n", pSrcInfo->ScaledCropWin.x);
                }
            }
        }
    }
    if(pSrcInfo->bDisplayNineLattice)
    {
        // nine lattice disable post scaling
        u16ScaleSrc = u16ScaleDst = 1920;
    }
    //store pst scaling info here
    pSrcInfo->u16HCusScalingSrc = u16ScaleSrc;
    pSrcInfo->u16HCusScalingDst = u16ScaleDst;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]HSrc=%u, HDst=%u\n", u16ScaleSrc, u16ScaleDst);

    if(bPstScalingDownCheck)
    {
        if((u16ScaleDst < u16ScaleSrc) && (u16ScaleSrc*MAX_PST_SCALING_DOWN_PERCENTAGE_FCLK170M/100) > u16ScaleDst)
        {
            MS_ASSERT(0);
            printf("[PST]!!!!!! ALERT: This FCLK can not support H PST scaling down too much[%u -> %u] !!!!!! \n", u16ScaleSrc, u16ScaleDst);
        }
    }

    if (u16ScaleDst != u16ScaleSrc)
    {
#ifdef MSOS_TYPE_LINUX_KERNEL
        H_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
        u32TmpPost = (MS_U32)u64_result;
#else
        u32TmpPost = H_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
#endif

#ifdef XC_SUPPORT_2STEP_SCALING
        if(MDrv_ReadByte(REG_CHIP_REVISION) >= 1)//Monet U02 fix .
        {
            #ifdef UFO_XC_SUPPORT_2STEP_SCALING
            if(bIsSupport2StepScaling)
            {
                u32TmpPost = u32TmpPost *2;
            }
            #endif
        }
#endif

        u32TmpPost &= 0xFFFFFFL;
        #if 1//( CHIP_FAMILY_TYPE == CHIP_FAMILY_S4LE )
        u32TmpPost |= 0x1000000L;
        #else
        u32TmpPost |= 0x800000L;
        #endif
    }
    else
    {
        u32TmpPost = 0x100000L;
    }

    if(MHal_XC_Check_Bypass(pInstance,E_XC_BYPASS_HVSP) == FALSE)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR); //K3_PQ
//#ifdef ENABLE_TV_SC2_PQ
        if (psXCInstPri->u32DeviceID == PQ_XC_ID_0 || psXCInstPri->u32DeviceID == PQ_XC_ID_1)
//#endif
        {
            if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable != NULL)
            {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW,
                                        E_XRULE_HSP,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16ScaleSrc,
                                        u16ScaleDst);
#else
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((PQ_WIN)eWindow,
                                        E_XRULE_HSP,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16ScaleSrc,
                                        u16ScaleDst);
#endif
            }
        }
#ifdef ENABLE_TV_SC2_PQ
        else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
        {
            if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex != NULL)
            {
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow,
                                        E_XRULE_HSP,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16ScaleSrc,
                                        u16ScaleDst);
            }
        }
#endif
        _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR); //K3_PQ
    }
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio = u32TmpPost;

    //-----------------------------------------
    // Vertical
    //-----------------------------------------
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]VCusScaling=%s\n",pSrcInfo->bVCusScaling ? "TRUE":"FALSE");
    if (pSrcInfo->bVCusScaling == FALSE)
    {
        u16ScaleSrc = pSrcInfo->ScaledCropWin.height;
        u16ScaleDst = pSrcInfo->stDispWin.height;
#ifndef DISABLE_3D_FUNCTION
        u16ScaleSrc = MDrv_SC_3D_Adjust_PostVerSrcSize(pInstance, pSrcInfo, u16ScaleSrc, eWindow);
#endif
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]VSrc=%u, VDst=%u, eDeInterlaceMode=%u, bFBL=%u, bInterlace=%u\n", u16ScaleSrc, u16ScaleDst, pSrcInfo->eDeInterlaceMode, pSrcInfo->bFBL, bInterlace)
    }
    else
    {
        u16ScaleSrc = pSrcInfo->u16VCusScalingSrc;
        u16ScaleDst = pSrcInfo->u16VCusScalingDst;
        //make sure crop window in the range of window after prescaling
        if((u16ScaleSrc > pSrcInfo->u16V_SizeAfterPreScaling) ||
           (u16ScaleSrc + pSrcInfo->ScaledCropWin.y > pSrcInfo->u16V_SizeAfterPreScaling))
        {
            MS_U16 u16PstScaledHeight;
            u16PstScaledHeight = (MS_U32)(u16ScaleDst*(pSrcInfo->u16V_SizeAfterPreScaling - pSrcInfo->ScaledCropWin.y))/u16ScaleSrc;
            //Check if the scaled content can full fill the display window, otherwise garbage will show out.
            if(u16PstScaledHeight < pSrcInfo->stDispWin.height)
            {
                if(u16ScaleSrc > pSrcInfo->u16V_SizeAfterPreScaling)
                {
                    u16ScaleSrc = pSrcInfo->u16V_SizeAfterPreScaling & ~0x01;
                    pSrcInfo->ScaledCropWin.y = 0;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]Limit ScaledCropWin.y to y=%u, VScaleSrc to: Src=%u\n", pSrcInfo->ScaledCropWin.y, u16ScaleSrc);
                }
                else
                {
                    pSrcInfo->ScaledCropWin.y = (pSrcInfo->u16V_SizeAfterPreScaling - u16ScaleSrc) & ~0x01;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]Limit ScaledCropWin.y to y=%u\n", pSrcInfo->ScaledCropWin.y);
                }
            }
        }
    }

    if(pSrcInfo->bDisplayNineLattice)
    {
        // nine lattice disable post scaling
        u16ScaleSrc = u16ScaleDst = 1080;
    }

#ifdef ENABLE_SCALING_WO_MUTE
    MS_U16 u16TmpSrc = MDrv_SC_Cal_Skip_OPM_line(pInstance, eWindow, u16ScaleSrc, u16ScaleDst, pSrcInfo);
#endif

    //store pst scaling info here
    pSrcInfo->u16VCusScalingSrc = u16ScaleSrc;
    pSrcInfo->u16VCusScalingDst = u16ScaleDst;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]VSrc=%u, VDst=%u, eDeInterlaceMode=%u, bFBL=%u, bInterlace=%u\n", u16ScaleSrc, u16ScaleDst, pSrcInfo->eDeInterlaceMode, pSrcInfo->bFBL, bInterlace);
    if(bPstScalingDownCheck)
    {
        if((u16ScaleDst < u16ScaleSrc) && (u16ScaleSrc*MAX_PST_SCALING_DOWN_PERCENTAGE_FCLK170M/100) > u16ScaleDst)
        {
            MS_ASSERT(0);
            printf("[PST]!!!!!! ALERT: This FCLK can not support V PST scaling down too much[%u -> %u] !!!!!! \n", u16ScaleSrc, u16ScaleDst);
        }
    }
    if (u16ScaleSrc == u16ScaleDst)
    {
        if( !(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow))) //!DS
        {
            if( bInterlace && (pSrcInfo->eDeInterlaceMode <= MS_DEINT_2DDI_BOB) )
            {
                // no need to do V post scaling
                if( !(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
                {
                    if ( (u16ScaleDst-1) == 0 )
                        u32TmpPost = 0;
                    else
                    {
                    #ifdef ENABLE_SCALING_WO_MUTE
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio(u16TmpSrc/2, u16ScaleDst);;
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio(u16TmpSrc/2, u16ScaleDst);
#endif
                    #else
                        u16ScaleSrc /= 2;
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);;
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
#endif
                    #endif
                    }
                }
             }
             else
             {
                 u32TmpPost = 0x100000L;
             }
        }
        else//DS
        {
            u32TmpPost = 0x100000L;
        }
    }
    else
    {
        if( !(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow))) //!DS
        {
            // for T2, 25/27 mode input line is full lines, BOB mode is half lines
            if( bInterlace && (pSrcInfo->eDeInterlaceMode <= MS_DEINT_2DDI_BOB))
            {
                if ( (u16ScaleDst-1) == 0)
                    u32TmpPost = 0 ;
                else
                {
                #ifdef ENABLE_SCALING_WO_MUTE
                    u32TmpPost = V_PostScalingRatio(u16TmpSrc/2, u16ScaleDst);
                #else
                    u16ScaleSrc /= 2;
                    if(psXCInstPri->u32DeviceID == 1 && XC_DEVICE1_IS_INTERLACE_OUT)
                    {
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                    else if (bIsInterlace == TRUE)
                    {
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                    else
                    {
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                #endif
                }
            }
            else
            {
                if ( (u16ScaleDst-1) == 0)
                    u32TmpPost = 0 ;
                else
                {
                    if(psXCInstPri->u32DeviceID == 1 && XC_DEVICE1_IS_INTERLACE_OUT)
                    {
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                    else if (bIsInterlace == TRUE)
                    {
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
#endif
                    }
                    else
                    {
#ifdef ENABLE_SCALING_WO_MUTE
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio(u16TmpSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio(u16TmpSrc, u16ScaleDst);
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
                        V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
                        u32TmpPost = (MS_U32)u64_result;
#else
                        u32TmpPost = V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
#endif
#endif
                    }
                }
            }
        }
        else //DS
        {
            if ( (u16ScaleDst-1) == 0)
                u32TmpPost = 0 ;
            else
            {
                if(psXCInstPri->u32DeviceID == 1 && XC_DEVICE1_IS_INTERLACE_OUT)
                {
#ifdef MSOS_TYPE_LINUX_KERNEL
                    V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
                    u32TmpPost = (MS_U32)u64_result;
#else
                    u32TmpPost = V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
#endif
                }
                else if (bIsInterlace == TRUE)
                {
#ifdef MSOS_TYPE_LINUX_KERNEL
                    V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
                    u32TmpPost = (MS_U32)u64_result;
#else
                    u32TmpPost = V_PostScalingRatio_P2I(u16ScaleSrc, u16ScaleDst);
#endif
                }
                else
                {
#ifdef ENABLE_SCALING_WO_MUTE
#ifdef MSOS_TYPE_LINUX_KERNEL
                    V_PostScalingRatio(u16TmpSrc, u16ScaleDst);
                    u32TmpPost = (MS_U32)u64_result;
#else
                    u32TmpPost = V_PostScalingRatio(u16TmpSrc, u16ScaleDst);
#endif
#else
#ifdef MSOS_TYPE_LINUX_KERNEL
                    V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
                    u32TmpPost = (MS_U32)u64_result;
#else
                    u32TmpPost = V_PostScalingRatio(u16ScaleSrc, u16ScaleDst);
#endif
#endif
                }
            }
        }
    }

#ifdef XC_SUPPORT_2STEP_SCALING
    if(MDrv_ReadByte(REG_CHIP_REVISION) >= 1)//Monet U02 fix .
    {
        #ifdef UFO_XC_SUPPORT_2STEP_SCALING
        if(bIsSupport2StepScaling)
        {
            u32TmpPost = u32TmpPost *2;
        }
        #endif
    }
#endif

    u32TmpPost &= 0xFFFFFFL;    //scaling factor support 24bits

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR); //K3_PQ
    if(MHal_XC_Check_Bypass(pInstance,E_XC_BYPASS_HVSP) == FALSE)
    {
//#ifdef ENABLE_TV_SC2_PQ
        if (psXCInstPri->u32DeviceID == PQ_XC_ID_0 || psXCInstPri->u32DeviceID == PQ_XC_ID_1)
//#endif
        {
            if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable != NULL)
            {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW,
                                        E_XRULE_VSP,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16ScaleSrc,
                                        u16ScaleDst);
#else
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable((PQ_WIN)eWindow,
                                        E_XRULE_VSP,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16ScaleSrc,
                                        u16ScaleDst);
#endif
            }
        }
#ifdef ENABLE_TV_SC2_PQ
        else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
        {
            if (s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex != NULL)
            {
                s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_load_scalingtable_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow,
                                        E_XRULE_VSP,
                                        pSrcInfo->bPreV_ScalingDown,
                                        bInterlace,
                                        pSrcInfo->bMemYUVFmt,
                                        u16ScaleSrc,
                                        u16ScaleDst);
            }
        }
#endif
    }
    // Set two init factor mode here to do P2I for Kaiser SC1
    if(E_XC_DEVICE1 == psXCInstPri->u32DeviceID && XC_DEVICE1_IS_INTERLACE_OUT)
    {
        HAL_SC_Set_VInitFactorTwo(pInstance, (u32TmpPost/2), eWindow);
        HAL_SC_Enable_VInitFactor(pInstance, TRUE, eWindow);

        // Field inverse in NTSC output mode
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 480)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 0x0002, 0x0002);
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_13_L, 0x0000, 0x0002);
        }
    }
    else if (bIsInterlace == TRUE)
    {
        HAL_SC_Set_VInitFactorTwo(pInstance, u32TmpPost/2, eWindow);
        HAL_SC_Enable_VInitFactor(pInstance, TRUE, eWindow);
    }

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR); //K3_PQ
    if (u32TmpPost != 0x100000L)
    #if 1//( CHIP_FAMILY_TYPE == CHIP_FAMILY_S4LE )
        u32TmpPost |= 0x1000000L;
    #else
        u32TmpPost |= 0x800000L;
    #endif

    pSrcInfo->u16V_Length = u16ScaleSrc;

#ifdef VIDEO_4K_I_PATCH
    if((pSrcInfo->stCapWin.height > VIDEO_4K_I_BIG_HEIGHT)&&(gSrcInfo[eWindow].bInterlace == TRUE))
    {
        pSrcInfo->u16V_Length -=2;
    }
#endif

#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_PostVLength(pInstance, pSrcInfo, eWindow);
#endif
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio = u32TmpPost;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[PST]HRatio=0x%tx, VRatio=0x%tx, u16V_Length=%u\n",
                                       (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32H_PostScalingRatio,
                                       (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PostScalingRatio,
                                       u16ScaleSrc);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}


// for mirror mode and dynamic scaling store u32BaseOffset
void MDrv_SC_set_DNRBaseOffset(void *pInstance, SCALER_WIN eWindow,MS_PHY u32BaseOffset)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling.u32DNRBaseOffset[eWindow] = u32BaseOffset * BYTE_PER_WORD;
}

MS_PHY MDrv_SC_get_DNRBaseOffset(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_PHY u32Return = 0;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u32Return = pXCResourcePrivate->stdrvXC_Scaling.u32DNRBaseOffset[eWindow];
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32Return;
}

void MDrv_SC_set_FRCMBaseOffset(void *pInstance, SCALER_WIN eWindow,MS_PHY u32BaseOffset)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling.u32FRCMBaseOffset[eWindow] = u32BaseOffset * BYTE_PER_WORD;
}

MS_PHY MDrv_SC_get_FRCMBaseOffset(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_PHY u32Return = 0;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u32Return = pXCResourcePrivate->stdrvXC_Scaling.u32FRCMBaseOffset[eWindow];
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32Return;
}

MS_PHY  MApi_XC_Get_DNRBaseOffset_U2(void* pInstance, SCALER_WIN eWindow)
{
    return MDrv_SC_get_DNRBaseOffset(pInstance, eWindow);
}

MS_PHY  MApi_XC_Get_DNRBaseOffset(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_DNR_BASEOFFSET XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_DNR_BASEOFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

MS_PHY MDrv_XC_Get_FRCM_W_BaseAddr(void *pInstance,MS_U8 u8id,SCALER_WIN eWindow)
{
    MS_PHY Addr = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_4K2K_VER == 4)
    Addr = Hal_SC_Get_FRCMBaseAddr(pInstance, u8id,eWindow);
#else
    Addr = 0;
#endif
    return Addr;
}

/******************************************************************************/
/*                     Frame number related                                   */
/* ****************************************************************************/
static void _Mdrv_XC_Refine_FrameNum_Rule(void *pInstance, SCALER_WIN eWindow,
                                                         MS_U32 u32Framesize,
                                                         MS_U32 u32DNRBufSize,
                                                         XC_FRAME_STORE_NUMBER *pFrameStoreNumber,
                                                         MS_BOOL *pbLinearMode,
                                                         MS_BOOL bInterlace,
                                                         XC_FRAME_STORE_NUMBER enPQSuggestedFrameNum)
{
    if((pFrameStoreNumber == NULL) || (pbLinearMode == NULL))
    {
        MS_ASSERT(0);
        return;
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch( enPQSuggestedFrameNum )
    {
        case IMAGE_STORE_13_FRAMES:
            if ((u32Framesize*13) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_13_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*13 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

    case IMAGE_STORE_12_FRAMES:
        if ((u32Framesize*12) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_12_FRAMES;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*12 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
        break;

        case IMAGE_STORE_11_FRAMES:
            if ((u32Framesize*11) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_11_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*11 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

        case IMAGE_STORE_10_FRAMES:
            if ((u32Framesize*10) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_10_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*10 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

        case IMAGE_STORE_9_FRAMES:
            if ((u32Framesize*9) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_9_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*9 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

    case IMAGE_STORE_8_FRAMES:
        if ((u32Framesize*8) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_8_FRAMES;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*8 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
        break;

        case IMAGE_STORE_7_FRAMES:
            if ((u32Framesize*7) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_7_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*7 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

    case IMAGE_STORE_6_FRAMES:
        if ((u32Framesize*6) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_6_FRAMES;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*6 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
        break;

        case IMAGE_STORE_5_FRAMES:
            if ((u32Framesize*5) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_5_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*5 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

    case IMAGE_STORE_4_FRAMES:
        if(MDrv_SC_GetScmiV1Speical4frame(pInstance, enPQSuggestedFrameNum) && ((u32Framesize*8) < u32DNRBufSize ))
        {
            *pFrameStoreNumber = IMAGE_STORE_4_FRAMES;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*8 < %td for Special4Frame\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
        else if ((u32Framesize*4) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_4_FRAMES;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*4 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
        else if ((u32Framesize*3) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
            #if  (_LINEAR_ADDRESS_MODE_SUPPORTED)
            *pbLinearMode = TRUE;
            #endif
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*3 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
#endif
        break;

    case IMAGE_STORE_3_FRAMES:
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
        if ((u32Framesize*3) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
            #if  (_LINEAR_ADDRESS_MODE_SUPPORTED)
            *pbLinearMode = TRUE;
            #endif
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*3 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
        }
#endif
        break;

        case IMAGE_STORE_2_FRAMES:
            if ((u32Framesize*2) < u32DNRBufSize )
            {
                *pFrameStoreNumber = IMAGE_STORE_2_FRAMES;
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*2 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
            }
            break;

#if (ENABLE_16_FIELD_SUPPORTED)
    case IMAGE_STORE_6_FIELDS:
        if ((u32Framesize*3) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_6_FIELDS;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"ENABLE_8_FIELD_SUPPORTED %d  \n",ENABLE_16_FIELD_SUPPORTED);
        }
        break;

    case IMAGE_STORE_8_FIELDS:
        if ((u32Framesize*4) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_8_FIELDS;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"ENABLE_8_FIELD_SUPPORTED %d  \n",ENABLE_16_FIELD_SUPPORTED);
        }
        break;

    case IMAGE_STORE_12_FIELDS:
        if ((u32Framesize*6) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_12_FIELDS;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"ENABLE_12_FIELD_SUPPORTED %d  \n",ENABLE_16_FIELD_SUPPORTED);
        }
        break;

    case IMAGE_STORE_14_FIELDS:
        if ((u32Framesize*7) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_14_FIELDS;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"ENABLE_14_FIELD_SUPPORTED %d  \n",ENABLE_16_FIELD_SUPPORTED);
        }
        break;

    case IMAGE_STORE_16_FIELDS:
        if ((u32Framesize*8) < u32DNRBufSize )
        {
            *pFrameStoreNumber = IMAGE_STORE_16_FIELDS;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"ENABLE_16_FIELD_SUPPORTED %d  \n",ENABLE_16_FIELD_SUPPORTED);
        }
        break;
#endif

    default:
        break;
    }

    //final protect handling:
    //mirror and sub window:               progressive: 4 frame mode; interlace: 8 field mode
    //sub window:                          progressive: 3 frame mode; interlace: 6 field mode
    //input vfreq > panel supported vfreq: progressive: 3 frame mode; interlace: 6 field mode
    //mirror:                              progressive: 3 frame mode; interlace: not care
    if(IsVMirrorMode(eWindow)
       && (eWindow == SUB_WINDOW)
       && (u32Framesize * 4 <= u32DNRBufSize))
    {
        if(bInterlace)
        {
#if (ENABLE_16_FIELD_SUPPORTED)
            if(*pFrameStoreNumber < IMAGE_STORE_8_FIELDS)
            {
               *pFrameStoreNumber = IMAGE_STORE_8_FIELDS;
            }
#endif
        }
        else
        {
#if (_4FRAME_BUFFER_PMODE_SUPPORTED)
            if(*pFrameStoreNumber < IMAGE_STORE_4_FRAMES)
            {
               *pFrameStoreNumber = IMAGE_STORE_4_FRAMES;
            }
#endif
        }
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*4 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
    }
    else if((( eWindow == SUB_WINDOW)
               || ((gSrcInfo[eWindow].u16InputVFreq > 600 + VFREQDELTA)
                    && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq <= 600))
               || ((gSrcInfo[eWindow].u16InputVFreq > 1200 + VFREQDELTA)
                    && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq <= 1200)))
            && (u32Framesize*3 < u32DNRBufSize))
    {
        if(bInterlace)
        {
#if (HW_DESIGN_4K2K_VER != 4)
    #if (ENABLE_16_FIELD_SUPPORTED)
            if(*pFrameStoreNumber < IMAGE_STORE_6_FIELDS)
            {
               *pFrameStoreNumber = IMAGE_STORE_6_FIELDS;
            }
    #endif
#endif
        }
        else
        {
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
            if(*pFrameStoreNumber < IMAGE_STORE_3_FRAMES)
            {
                // 3 frame mode need to be set as linear mode.
    #if (_LINEAR_ADDRESS_MODE_SUPPORTED)
                *pbLinearMode = TRUE;
    #endif
               *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
            }
#endif
        }
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*3 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
    }
    else if((IsVMirrorMode(eWindow)
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
        || (MDrv_XC_FPLL_IsSupportLockFreqOnly(pInstance))
#endif
    )
            && (u32Framesize*3 < u32DNRBufSize)
            && (!bInterlace))
    {
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
            if(*pFrameStoreNumber < IMAGE_STORE_3_FRAMES)
            {
                // 3 frame mode need to be set as linear mode.
    #if (_LINEAR_ADDRESS_MODE_SUPPORTED)
                *pbLinearMode = TRUE;
    #endif
               *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
            }
#endif
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%td*3 < %td\n", (ptrdiff_t)u32Framesize, (ptrdiff_t)u32DNRBufSize );
    }

    if((bInterlace!=TRUE)&&(enPQSuggestedFrameNum == IMAGE_STORE_2_FRAMES))
    {
        if(u32Framesize*3 <= u32DNRBufSize)
        {
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%ld*3 < %ld\n",(long int)u32Framesize,(long int)u32DNRBufSize );
        }
        else
        {
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"%ld*3 > %ld\n",(long int)u32Framesize,(long int)u32DNRBufSize );
        }

        if(u32Framesize*3 <= u32DNRBufSize)
        {

        #if (_3FRAME_BUFFER_PMODE_SUPPORTED)
            if(*pFrameStoreNumber < IMAGE_STORE_3_FRAMES)
            {
                // 3 frame mode need to be set as linear mode.
            #if (_LINEAR_ADDRESS_MODE_SUPPORTED)
                *pbLinearMode = TRUE;
            #endif
                *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
            }
        #endif
        }
    }

#if _FIELD_PACKING_MODE_SUPPORTED
    if(enPQSuggestedFrameNum != *pFrameStoreNumber )
    {
        printf("Note: enPQSuggestedFrameNum(%u) != *pFrameStoreNumber(%u)\n", enPQSuggestedFrameNum,*pFrameStoreNumber);
    }
#endif
}

/// Decide best frame buffer number used.
static void _Mdrv_XC_Refine_FrameNum(void *pInstance, MS_BOOL *pbLinearMode,
                        XC_FRAME_STORE_NUMBER *pFrameStoreNumber,
                        MS_BOOL bInterlace,
                        MS_U32 u32Framesize,
                        MS_U32 u32DNRBufSize,
                        SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U32 u32Dynamic_scale_size = (MS_U32)gSrcInfo[eWindow].Status2.u16VirtualBox_Width*
                                gSrcInfo[eWindow].Status2.u16VirtualBox_Height*
                                gSrcInfo[eWindow].u8BitPerPixel / 8 ;

    XC_FRAME_STORE_NUMBER enPQSuggestedFrameNum = MDrv_XC_GetPQSuggestedFrameNum(pInstance, eWindow);

    if(pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForXc)
    {
        if((enPQSuggestedFrameNum >= IMAGE_STORE_2_FRAMES)&&(enPQSuggestedFrameNum <= IMAGE_STORE_13_FRAMES))
        {
            switch(enPQSuggestedFrameNum)
            {
                case IMAGE_STORE_2_FRAMES:
                case IMAGE_STORE_3_FRAMES:
                    enPQSuggestedFrameNum = IMAGE_STORE_3_FRAMES;
                    break;
                case IMAGE_STORE_4_FRAMES:
                case IMAGE_STORE_5_FRAMES:
                    enPQSuggestedFrameNum = IMAGE_STORE_5_FRAMES;
                    break;
                case IMAGE_STORE_6_FRAMES:
                case IMAGE_STORE_7_FRAMES:
                    enPQSuggestedFrameNum = IMAGE_STORE_7_FRAMES;
                    break;
                case IMAGE_STORE_8_FRAMES:
                case IMAGE_STORE_9_FRAMES:
                    enPQSuggestedFrameNum = IMAGE_STORE_9_FRAMES;
                    break;
                case IMAGE_STORE_10_FRAMES:
                case IMAGE_STORE_11_FRAMES:
                    enPQSuggestedFrameNum = IMAGE_STORE_11_FRAMES;
                    break;
                case IMAGE_STORE_12_FRAMES:
                case IMAGE_STORE_13_FRAMES:
                    enPQSuggestedFrameNum = IMAGE_STORE_13_FRAMES;
                    break;
                default:
                    enPQSuggestedFrameNum = IMAGE_STORE_3_FRAMES;
                    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] invlid Frame num,set defualt value:3 frames\n",__func__,__LINE__);
                    break;
            }
        }

    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] u32Framesize:%td u32DNRBufSize:%td \n",__func__,__LINE__, (ptrdiff_t)u32Framesize,(ptrdiff_t)u32DNRBufSize);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] u32Dynamic_scale_size:%td enPQSuggestedFrameNum:%d \n",__func__,__LINE__, (ptrdiff_t)u32Dynamic_scale_size,enPQSuggestedFrameNum);

    if((pbLinearMode == NULL) || (pFrameStoreNumber == NULL))
    {
        MS_ASSERT(0);
        return;
    }

    *pbLinearMode = FALSE;

    if(bInterlace)
    {
        *pFrameStoreNumber = IMAGE_STORE_4_FIELDS;
    }
    else
    {
        if(pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForXc)
        {
            *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        }
        else
        {
            *pFrameStoreNumber = IMAGE_STORE_2_FRAMES;
        }
    }

    if((MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)) && (eWindow == MAIN_WINDOW))
    {
        if(bInterlace )
        {
            _Mdrv_XC_Refine_FrameNum_Rule(pInstance, eWindow,
                                            u32Framesize,
                                            u32DNRBufSize,
                                            pFrameStoreNumber,
                                            pbLinearMode,
                                            bInterlace,
                                            enPQSuggestedFrameNum);
        }
        else
        {
        #if SUPPORT_SEAMLESS_ZAPPING /// for SeamlessZapping
            if(Hal_SC_GetSeamlessZappingStatus(pInstance, eWindow))
            {
                _Mdrv_XC_Refine_FrameNum_Rule(pInstance, eWindow,
                                                u32Framesize,
                                                u32DNRBufSize,
                                                pFrameStoreNumber,
                                                pbLinearMode,
                                                bInterlace,
                                                enPQSuggestedFrameNum);
            }
            else
        #endif
            if(pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForXc)
            {
                if((u32Dynamic_scale_size*5<u32DNRBufSize) && (! MDrv_SC_GetScmiV1Speical4frame(pInstance, enPQSuggestedFrameNum)))
                {
#ifdef UFO_XC_SUPPORT_3D_DS
                    if (IS_INPUT_FRAME_ALTERNATIVE(eWindow))
                    {
                        *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
                    }
                    else
#endif
                    {
                        *pFrameStoreNumber = IMAGE_STORE_5_FRAMES;
                    }
                }
                else if((u32Dynamic_scale_size*10<u32DNRBufSize) && (MDrv_SC_GetScmiV1Speical4frame(pInstance, enPQSuggestedFrameNum)))
                {//special 4 frame mode(store 8 frame in memory)
                    *pFrameStoreNumber = IMAGE_STORE_5_FRAMES;
                }
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
                else if(u32Dynamic_scale_size*4 < u32DNRBufSize)
                {
                    *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
                    #if (_LINEAR_ADDRESS_MODE_SUPPORTED)
                        *pbLinearMode = TRUE;
                    #endif
                }
#endif
                else
                {
                    *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
                }
            }
            else
            {
#if (_4FRAME_BUFFER_PMODE_SUPPORTED)
                if((u32Dynamic_scale_size*4<u32DNRBufSize) && (! MDrv_SC_GetScmiV1Speical4frame(pInstance, enPQSuggestedFrameNum)))
                {
#ifdef UFO_XC_SUPPORT_3D_DS
                    if (IS_INPUT_FRAME_ALTERNATIVE(eWindow))
                    {
                        *pFrameStoreNumber = IMAGE_STORE_2_FRAMES;
                    }
                    else
#endif
                    {
                        *pFrameStoreNumber = IMAGE_STORE_4_FRAMES;
                    }
                }
                else if((u32Dynamic_scale_size*8<u32DNRBufSize) && (MDrv_SC_GetScmiV1Speical4frame(pInstance, enPQSuggestedFrameNum)))
                {//special 4 frame mode(store 8 frame in memory)
                    *pFrameStoreNumber = IMAGE_STORE_4_FRAMES;
                }
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
                else if(u32Dynamic_scale_size*3 < u32DNRBufSize)
                {
                    *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
                    #if (_LINEAR_ADDRESS_MODE_SUPPORTED)
                        *pbLinearMode = TRUE;
                    #endif
                }
#endif
                else
#endif
                {
                    *pFrameStoreNumber = IMAGE_STORE_2_FRAMES;
                }
            }
    }
    }
    else
    {
        _Mdrv_XC_Refine_FrameNum_Rule(pInstance, eWindow,
                                        u32Framesize,
                                        u32DNRBufSize,
                                        pFrameStoreNumber,
                                        pbLinearMode,
                                        bInterlace,
                                        enPQSuggestedFrameNum);

#ifndef DISABLE_3D_FUNCTION
        // Adjust frame mode by 3D content
        *pFrameStoreNumber = MDrv_SC_3D_Adjust_FrameNum(pInstance, *pFrameStoreNumber, eWindow);
#endif
    }

    if(*pFrameStoreNumber != IMAGE_STORE_3_FRAMES)
    {
        *pbLinearMode = FALSE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "if it's not 3 frame mode, linear mode should also be closed\n");
    }

#if (HW_DESIGN_4K2K_VER == 7)  //Kano Patch
    if((psXCInstPri->u32DeviceID==0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IPMUX_HDR_MODE))
    {
        //For Kano HDR design case, HDR is for FB mode and FRC in XC, so it need 3 frame or 6 fieldto FRC
        if(bInterlace && *pFrameStoreNumber < IMAGE_STORE_6_FIELDS)
        {
            *pFrameStoreNumber = IMAGE_STORE_6_FIELDS;
        }
        else if(bInterlace == FALSE && *pFrameStoreNumber < IMAGE_STORE_3_FRAMES)
        {
            *pFrameStoreNumber = IMAGE_STORE_3_FRAMES;
        }
    }
    //Set the frame is 4 on Kano XC1 with Rx in, because FRC case
    else if((psXCInstPri->u32DeviceID==1) &&
            (IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)
             || IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType)
             || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType)))
    {
        if(bInterlace)
        {
            *pFrameStoreNumber = IMAGE_STORE_8_FIELDS;
        }
        else
        {
            *pFrameStoreNumber = IMAGE_STORE_4_FRAMES;
        }
    }

    //This is Kano SW patch for release
    //I mode must do FRC in XC, so need 6 field
#ifdef HW_DESIGN_HDR_VER
#if (HW_DESIGN_HDR_VER == 0)
    if(psXCInstPri->u32DeviceID==0 && ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_IPMUX_HDR_MODE) == 0))
    {
        if((bInterlace == TRUE) && (*pFrameStoreNumber < IMAGE_STORE_6_FIELDS))
        {
            *pFrameStoreNumber = IMAGE_STORE_6_FIELDS;
        }
        //Kano SWDS case do not to set 4 frames
        //Need only 2 Frame now
        if((bInterlace == FALSE) && (*pFrameStoreNumber > IMAGE_STORE_2_FRAMES) && (MDrv_XC_GetDynamicScalingStatus(pInstance)))
        {
            *pFrameStoreNumber = IMAGE_STORE_2_FRAMES;
        }
    }
#endif
#endif
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"pFrameStoreNumber: %d , corresponding factor is: %u\n",
            *pFrameStoreNumber, MDrv_XC_TransFrameNumToFactor(pInstance, *pFrameStoreNumber, *pbLinearMode, _FIELD_PACKING_MODE_SUPPORTED));
}

#if (HW_DESIGN_4K2K_VER == 4)
/// Decide best FRCM frame buffer number used.
static void _Mdrv_XC_Refine_FRCMFrameNum(void *pInstance, MS_BOOL *pbLinearMode,
                        XC_FRAME_STORE_NUMBER *pFRCMFrameStoreNumber,
                        MS_U32 u32Framesize,
                        MS_U32 u32FRCMBufSize,
                        SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U32 u32Dynamic_scale_size = (MS_U32)gSrcInfo[eWindow].Status2.u16VirtualBox_Width*
                                gSrcInfo[eWindow].Status2.u16VirtualBox_Height*
                                gSrcInfo[eWindow].u8BitPerPixel / 8 ;

    XC_FRAME_STORE_NUMBER enPQSuggestedFrameNum = MDrv_XC_GetPQSuggestedFRCMFrameNum(pInstance, eWindow);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"[%s,%5d] u32Dynamic_scale_size:%td enPQSuggestedFrameNum:%d \n",__func__,__LINE__, (ptrdiff_t)u32Dynamic_scale_size,enPQSuggestedFrameNum);

    if((pbLinearMode == NULL) || (pFRCMFrameStoreNumber == NULL))
    {
        MS_ASSERT(0);
        return;
    }

    *pbLinearMode = FALSE;

    *pFRCMFrameStoreNumber = IMAGE_STORE_2_FRAMES;

    if((MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)) && (eWindow == MAIN_WINDOW))
    {
        if (u32Dynamic_scale_size*4<u32FRCMBufSize)
        {
            *pFRCMFrameStoreNumber = IMAGE_STORE_4_FRAMES;
        }
#if (_3FRAME_BUFFER_PMODE_SUPPORTED)
        else if(u32Dynamic_scale_size*3 < u32FRCMBufSize)
        {
            *pFRCMFrameStoreNumber = IMAGE_STORE_3_FRAMES;
            #if (_LINEAR_ADDRESS_MODE_SUPPORTED)
                *pbLinearMode = TRUE;
            #endif
        }
#endif
        else
        {
            *pFRCMFrameStoreNumber = IMAGE_STORE_2_FRAMES;
        }
    }
    else
    {
        _Mdrv_XC_Refine_FrameNum_Rule(pInstance, eWindow,
                                        u32Framesize,
                                        u32FRCMBufSize,
                                        pFRCMFrameStoreNumber,
                                        pbLinearMode,
                                        FALSE,
                                        enPQSuggestedFrameNum);

#ifndef DISABLE_3D_FUNCTION
        // Adjust frame mode by 3D content
        *pFRCMFrameStoreNumber = MDrv_SC_3D_Adjust_FrameNum(pInstance, *pFRCMFrameStoreNumber, eWindow);
#endif
    }

    if(*pFRCMFrameStoreNumber != IMAGE_STORE_3_FRAMES)
    {
        *pbLinearMode = FALSE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "if it's not 3 frame mode, linear mode should also be closed\n");
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"pFRCMFrameStoreNumber: %d , corresponding factor is: %u\n",
            *pFRCMFrameStoreNumber, MDrv_XC_TransFrameNumToFactor(pInstance, *pFRCMFrameStoreNumber, *pbLinearMode, _FIELD_PACKING_MODE_SUPPORTED));
}
#endif

//save current frame number factor into static var
void MDrv_XC_Save_FrameNumFactor(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FrameNum)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling._au8StoreFrameNumFactor[eWindow] = u8FrameNum;
}

#if (HW_DESIGN_4K2K_VER == 4)
//save current frcm frame number factor into static var
void MDrv_XC_Save_FRCMFrameNumFactor(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FrameNum)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling._u8StoreFRCMFrameNumFactor[eWindow] = u8FrameNum;
}
#endif

//get stored scm frame number factor from static var
MS_U8 MApi_XC_Get_FrameNumFactor_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_U8 u8Return = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u8Return = pXCResourcePrivate->stdrvXC_Scaling._au8StoreFrameNumFactor[eWindow];
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u8Return;
}

MS_U8 MApi_XC_Get_FrameNumFactor(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_FRAMENUM_FACTOR XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FRAMENUM_FACTOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

//get stored frcm frame number factor from static var
MS_U8 MApi_XC_Get_FRCM_FrameNum_U2(void* pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_U8 u8Return = FALSE;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (HW_DESIGN_4K2K_VER == 4)
    u8Return = pXCResourcePrivate->stdrvXC_Scaling._u8StoreFRCMFrameNumFactor[eWindow];
#endif
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u8Return;
}

MS_U8 MApi_XC_Get_FRCMFrameNum(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_FRCM_FRAMENUM XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FRCM_FRAMENUM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

#if _FIELD_PACKING_MODE_SUPPORTED
static MS_U16 _MDrv_XC_Get_FrameNumFromReg(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16FrameCount = 0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == MAIN_WINDOW)
    {
       u16FrameCount = 0x1F & SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_19_L);
    }
    else
    {
       u16FrameCount = 0x1F & SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_59_L);
    }

    return u16FrameCount;
}

#if (HW_DESIGN_4K2K_VER == 4)
static MS_U16 _MDrv_XC_Get_FRCMFrameNumFromReg(void *pInstance, SCALER_WIN eWindow)
{
    MS_U16 u16FRCMFrameCount = 0;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(eWindow == MAIN_WINDOW)
    {
       u16FRCMFrameCount = 0x1F & SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_19_L);
    }
    else
    {
       u16FRCMFrameCount = 0x1F & SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK32_59_L);
    }

    return u16FRCMFrameCount;
}
#endif
#endif

XC_FRAME_STORE_NUMBER MDrv_XC_GetPQSuggestedFrameNum(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if _FIELD_PACKING_MODE_SUPPORTED

    MS_U16 u16FrameCount = 0x00;

#ifdef K3_U2
    if(E_XC_DEVICE0 == psXCInstPri->u32DeviceID) // SC0
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl)
        {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW, E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
#else
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow, E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
#endif
            {
                u16FrameCount = _MDrv_XC_Get_FrameNumFromReg(pInstance, eWindow);

                if(u16FrameCount == 0)
                {
                    u16FrameCount = 2;
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original method\n", eWindow);
            }
        }
        else
        {
            u16FrameCount = _MDrv_XC_Get_FrameNumFromReg(pInstance, eWindow);

            if(u16FrameCount == 0)
            {
                u16FrameCount = 2;
            }
            printf("Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original frame mode\n", eWindow);
        }

        if(Hal_SC_IsPX2MemFormat(pInstance, eWindow))
        {
            u16FrameCount /= 2;
        }
    }
    else // SC1
    {
        if(gSrcInfo[eWindow].bInterlace)
        {
            u16FrameCount = 8;
        }
        else
        {
            u16FrameCount = 4;
        }
    }
#else
#ifdef ENABLE_TV_SC2_PQ
    if (  (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        ||(psXCInstPri->u32DeviceID == PQ_XC_ID_1)
#endif
    )
#endif
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl)
        {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW, E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
#else
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow, E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
#endif
            {
                u16FrameCount = _MDrv_XC_Get_FrameNumFromReg(pInstance, eWindow);

                if(u16FrameCount == 0)
                {
                    u16FrameCount = 2;
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original method\n", eWindow);
            }
        }
        else
        {
            u16FrameCount = _MDrv_XC_Get_FrameNumFromReg(pInstance, eWindow);

            if(u16FrameCount == 0)
            {
                u16FrameCount = 2;
            }
            printf("Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original frame mode\n", eWindow);
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow, E_PQ_IOCTL_PQ_SUGGESTED_FRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
            {
                u16FrameCount = _MDrv_XC_Get_FrameNumFromReg(pInstance, eWindow);

                if(u16FrameCount == 0)
                {
                    u16FrameCount = 2;
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original method\n", eWindow);
            }
        }
        else
        {
            u16FrameCount = _MDrv_XC_Get_FrameNumFromReg(pInstance, eWindow);

            if(u16FrameCount == 0)
            {
                u16FrameCount = 2;
            }
            printf("Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original frame mode\n", eWindow);
        }
    }
#else
#if (HW_DESIGN_4K2K_VER == 7) //Kano Patch
    if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(gSrcInfo[eWindow].bInterlace)
        {
            u16FrameCount = 8;
        }
        else
        {
            u16FrameCount = 4;
        }
    }
#endif
#endif
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_XC_GetPQSuggestedFrameNum(%u) get %u frame count\n", eWindow, u16FrameCount);

    return MDrv_XC_TransFactorToFrameNum(pInstance, eWindow, u16FrameCount);
#else
    return Hal_SC_GetFrameStoreMode(pInstance, eWindow);
#endif
}
#if (HW_DESIGN_4K2K_VER == 4)
XC_FRAME_STORE_NUMBER MDrv_XC_TransFactorToFRCMFrameNum(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FrameNumFactor)
{
#if _FIELD_PACKING_MODE_SUPPORTED
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch(u8FrameNumFactor) // Get field type.
    {
        default:
            return IMAGE_STORE_2_FRAMES;
        case 3:
            return IMAGE_STORE_3_FRAMES;
        case 4:
            return IMAGE_STORE_4_FRAMES;
        case 6:
            return IMAGE_STORE_6_FRAMES;
        case 8:
            return IMAGE_STORE_8_FRAMES;
        case 10:
            return IMAGE_STORE_10_FRAMES;
        case 12:
            return IMAGE_STORE_12_FRAMES;
    }
#else
    return IMAGE_STORE_UNDEFINE;
#endif
}

XC_FRAME_STORE_NUMBER MDrv_XC_GetPQSuggestedFRCMFrameNum(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16FrameCount = 0x00;

#ifdef ENABLE_TV_SC2_PQ
    if (  (psXCInstPri->u32DeviceID == PQ_XC_ID_0)
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        ||(psXCInstPri->u32DeviceID == PQ_XC_ID_1)
#endif
    )
#endif
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl)
        {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((psXCInstPri->u32DeviceID == 1)?PQ_SUB_WINDOW:PQ_MAIN_WINDOW, E_PQ_IOCTL_PQ_SUGGESTED_FRCMFRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
#else
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl((PQ_WIN)eWindow, E_PQ_IOCTL_PQ_SUGGESTED_FRCMFRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
#endif
            {
                if (  (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SAVE_MEM_MODE)
                    &&(  IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType)
                       ||IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType)))
                {
                    if (gSrcInfo[eWindow].stCapWin.width>=XC_4K2K_WIDTH_MIN)
                    {
                        u16FrameCount = 2;
                    }
                    else if ((gSrcInfo[eWindow].stCapWin.width > 1900) && (gSrcInfo[eWindow].stCapWin.height > 2100) && (gSrcInfo[eWindow].stCapWin.height < 2200))
                    {
                        u16FrameCount = 4;
                    }
                    else if((gSrcInfo[eWindow].bInterlace && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )))
                    {
                        u16FrameCount = 4;
                    }
                    else
                    {
                        u16FrameCount = 3;
                    }
                }
                else
                {
                    u16FrameCount = 4;
                }
#if 0
                u16FrameCount = _MDrv_XC_Get_FRCMFrameNumFromReg(pInstance, eWindow);

                if(u16FrameCount == 0)
                {
                    u16FrameCount = 2;
                }
#endif
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original method\n", eWindow);
            }
        }
        else
        {
            u16FrameCount = _MDrv_XC_Get_FRCMFrameNumFromReg(pInstance, eWindow);

            if(u16FrameCount == 0)
            {
                u16FrameCount = 4;
            }
            printf("Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original frame mode\n", eWindow);
        }
    }
#ifdef ENABLE_TV_SC2_PQ
    else if (psXCInstPri->u32DeviceID == PQ_XC_ID_1)
    {
        if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex)
        {
            if(s_PQ_Function_Info[psXCInstPri->u32DeviceID].pq_ioctl_ex(psXCInstPri->u32DeviceID, (PQ_WIN)eWindow, E_PQ_IOCTL_PQ_SUGGESTED_FRCMFRAMENUM, (void *)&u16FrameCount, sizeof(u16FrameCount)) == FALSE)
            {
                // temp patch wait PQ ready
                u16FrameCount = 4;
            #if 0
                u16FrameCount = _MDrv_XC_Get_FRCMFrameNumFromReg(pInstance, eWindow);

                if(u16FrameCount == 0)
                {
                    u16FrameCount = 2;
                }
            #endif
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original method\n", eWindow);
            }
        }
        else
        {
            u16FrameCount = _MDrv_XC_Get_FRCMFrameNumFromReg(pInstance, eWindow);

            if(u16FrameCount == 0)
            {
                u16FrameCount = 4;
            }
            printf("Alert: cannot get frameCount from IOCTL, eWindow=%u! Fall back to original frame mode\n", eWindow);
        }
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "MDrv_XC_GetPQSuggestedFrameNum(%u) get %u frame count\n", eWindow, u16FrameCount);

    return MDrv_XC_TransFactorToFRCMFrameNum(pInstance, eWindow, u16FrameCount);
}
#endif

static void _MDrv_XC_Set_FrameNum(void *pInstance, SCALER_WIN eWindow, MS_BOOL bIsFRCMBuff, XC_FRAME_STORE_NUMBER enFBType, MS_BOOL bInterlace)
{
#if _FIELD_PACKING_MODE_SUPPORTED
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8FrameCount = 2;
    MS_U32 u32Reg = 0;
    u8FrameCount = MDrv_XC_TransFrameNumToFactor(pInstance, enFBType, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "bIsFRCMBuff(%u) enFBType = %u \n", bIsFRCMBuff, enFBType);
    // If memory format is PX2 progressive mode, frame count need to set doubled
#if SUPPORT_SEAMLESS_ZAPPING
    if(Hal_SC_IsPX2MemFormat(pInstance, eWindow)) // SC0
    {
        u8FrameCount *= 2;
    }
#endif
    if (bIsFRCMBuff)
    {
        if( eWindow == MAIN_WINDOW )
        {
            u32Reg = REG_SC_BK32_19_L;
        }
        else
        {
            u32Reg = REG_SC_BK32_59_L;
        }
    }
    else
    {
        if( eWindow == MAIN_WINDOW )
        {
            u32Reg = REG_SC_BK12_19_L;
        }
        else
        {
            u32Reg = REG_SC_BK12_59_L;
        }
    }

#ifdef K3_U2
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, u32Reg, u8FrameCount , BITMASK(4:0) );
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
#endif
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Reg, u8FrameCount , BITMASK(4:0) );
    }
#else
    HAL_SC_Set_FB_Num(pInstance, eWindow, enFBType, bInterlace);
#endif
}

XC_FRAME_STORE_NUMBER MDrv_XC_TransFactorToFrameNum(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FrameNumFactor)
{
#if _FIELD_PACKING_MODE_SUPPORTED
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    switch( u8FrameNumFactor) // Get field type.
    {
        default:
            if (gSrcInfo[eWindow].bInterlace)
            {
                return IMAGE_STORE_4_FIELDS;
            }
            else
            {
                return IMAGE_STORE_2_FRAMES;
            }
        case 3:
            return IMAGE_STORE_3_FRAMES;
        case 4:
            if (gSrcInfo[eWindow].bInterlace)
            {
                return IMAGE_STORE_4_FIELDS;
            }
            else
            {
                return IMAGE_STORE_4_FRAMES;
            }
        case 5:
            return IMAGE_STORE_5_FRAMES;
        case 6:
            if (gSrcInfo[eWindow].bInterlace)
            {
                return IMAGE_STORE_6_FIELDS;
            }
            else
            {
                return IMAGE_STORE_6_FRAMES;
            }
        case 8:
            if (gSrcInfo[eWindow].bInterlace)
            {
                return IMAGE_STORE_8_FIELDS;
            }
            else
            {
                return IMAGE_STORE_8_FRAMES;
            }
        case 10:
            if (gSrcInfo[eWindow].bInterlace)
            {
                return IMAGE_STORE_10_FIELDS;
            }
            else
            {
                return IMAGE_STORE_10_FRAMES;
            }
        case 12:
            if (gSrcInfo[eWindow].bInterlace)
            {
                return IMAGE_STORE_12_FIELDS;
            }
            else
            {
                return IMAGE_STORE_12_FRAMES;
            }
        case 14:
            return IMAGE_STORE_14_FIELDS;
        case 16:
            return IMAGE_STORE_16_FIELDS;

    }
#else
    return IMAGE_STORE_UNDEFINE;
#endif
}

MS_U8 MDrv_XC_TransFrameNumToFactor(void *pInstance, XC_FRAME_STORE_NUMBER eFrameStoreNumber, MS_BOOL bLinearMode, MS_BOOL bFieldPackingMode)
{
    MS_U8 u8Factor = 0;

    switch(eFrameStoreNumber)
    {
#if ENABLE_16_FIELD_SUPPORTED
        case IMAGE_STORE_6_FIELDS:

            if(bFieldPackingMode)
            {
                u8Factor = 6;
            }
            else
            {
                u8Factor = 3;
            }
            break;

        case IMAGE_STORE_8_FIELDS:

            if(bFieldPackingMode)
            {
                u8Factor = 8;
            }
            else
            {
                u8Factor = 4;
            }

            break;

        case IMAGE_STORE_10_FIELDS:

            if(bFieldPackingMode)
            {
                u8Factor = 10;
            }
            else
            {
                u8Factor = 5;
            }

            break;

        case IMAGE_STORE_12_FIELDS:

            if(bFieldPackingMode)
            {
                u8Factor = 12;
            }
            else
            {
                u8Factor = 6;
            }

            break;
        case IMAGE_STORE_14_FIELDS:

            if(bFieldPackingMode)
            {
                u8Factor = 14;
            }
            else
            {
                u8Factor = 7;
            }

             break;
        case IMAGE_STORE_16_FIELDS:

            if(bFieldPackingMode)
            {
                u8Factor = 16;
            }
            else
            {
                u8Factor = 8;
            }

            break;
#endif

        case IMAGE_STORE_3_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 3;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;

        case IMAGE_STORE_4_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 4;
                }
                else
                {
                    if(MDrv_SC_GetScmiV1Speical4frame(pInstance, IMAGE_STORE_4_FRAMES))
                    {
                        u8Factor =  4;
                    }
                    else
                    {
                        u8Factor =  2;
                    }
                }
            }

            break;
        case IMAGE_STORE_5_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 5;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;
        case IMAGE_STORE_6_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 6;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;
        case IMAGE_STORE_7_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 7;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;

        case IMAGE_STORE_8_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                if(bFieldPackingMode)
                {
                    u8Factor = 8;
                }
                else
                {
                    u8Factor = 4;
                }
            }
            break;

        case IMAGE_STORE_9_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 9;
                }
                else
                {
                    u8Factor = 4;
                }
            }

        break;

        case IMAGE_STORE_10_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 10;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;

        case IMAGE_STORE_11_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 11;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;

        case IMAGE_STORE_12_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 12;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;

        case IMAGE_STORE_13_FRAMES:

            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 13;
                }
                else
                {
                    u8Factor = 2;
                }
            }

        break;

        case IMAGE_STORE_4_FIELDS:
            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 4;
                }
                else
                {
                    u8Factor = 2;
                }
            }
            break;

        case IMAGE_STORE_2_FIELDS:
            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // Put all field together.
                if(bFieldPackingMode)
                {
                    u8Factor = 2;
                }
                else
                {
                    u8Factor = 1;
                }
            }
            break;

        case IMAGE_STORE_2_FRAMES:
        default:
            if (bLinearMode)
            {
                u8Factor =  1;
            }
            else
            {
                // 2 frame mode is same for packet or non packet mode
                u8Factor =  2;
            }
            break;
    }

    return u8Factor;
}

void MDrv_SC_set_ipmsetting(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    gSrcInfo[eWindow].u16IPMFetch = pSrcInfo->u16H_SizeAfterPreScaling;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"===================================================\n");
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u16DNRFetch=%d \n",gSrcInfo[eWindow].u16IPMFetch);
//#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
    if (pSrcInfo->bMemFmt422)
        gSrcInfo[eWindow].u16IPMFetch = (gSrcInfo[eWindow].u16IPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
//#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC," %d \n",gSrcInfo[eWindow].u16IPMFetch);

    if(eWindow == MAIN_WINDOW)
    {
        if (gSrcInfo[MAIN_WINDOW].u16IPMFetch > MST_LINE_BFF_MAX)
        {
            gSrcInfo[MAIN_WINDOW].u16IPMFetch = MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign IPM fetch as %lu due to HW limitation \n", MST_LINE_BFF_MAX);
        }
    }
    else
    {
#if (HW_DESIGN_3D_VER < 2)
#ifndef DISABLE_3D_FUNCTION
        if(MDrv_SC_3D_IsIPMFetchNoLimitCase(pInstance, eWindow))
        {
            //no any limits
        }
        else if((E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance))
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Output_Mode(pInstance))
                || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
               )
        {
            if(gSrcInfo[SUB_WINDOW].u16IPMFetch > MS_3D_LINE_BFF_MAX)
            {
                gSrcInfo[SUB_WINDOW].u16IPMFetch = MS_3D_LINE_BFF_MAX;
            }
        }
        else
#endif
        if (gSrcInfo[SUB_WINDOW].u16IPMFetch > SUB_MST_LINE_BFF_MAX)
#else
#ifndef DISABLE_3D_FUNCTION

        if((E_XC_3D_PANEL_NONE == MDrv_XC_Get_3D_Panel_Type(pInstance))
          &&(gSrcInfo[SUB_WINDOW].u16IPMFetch > SUB_MST_LINE_BFF_MAX))
#endif
        {

#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
            //Normal PIP case
            gSrcInfo[SUB_WINDOW].u16IPMFetch = SUB_MST_LINE_BFF_MAX;
#endif
        }
#endif
    }
}

void MDrv_SC_set_dnrsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL bIsConnectOPM, MS_PHY* pu32OneLineOffset, MS_BOOL bDoMirror, SCALER_WIN eWindow)
{
    MS_U16 u16V_Writelimit;
    MS_PHY u32Offset, u32DNRBase0, u32DNRBase1, u32DNRBase2, u32WritelimitBase=0;
    MS_PHY u32OPMBase0, u32OPMBase1, u32OPMBase2;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U16 u16AlignWidth = 0;
    MS_PHY u32BaseOffset = 0, u32Framesize;
    MS_PHY u32BaseAddr, u32BuffSize;
    MS_PHY u32OneLineOffset = 0;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16DNROffset = 0;
    MS_U8 u8FrameNum = 0;
    MS_U16 u16H_SizeAfterPreScaling = pSrcInfo->u16H_SizeAfterPreScaling;
    MS_U16 u16DNROffsetFor3D = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (bIsConnectOPM)
    {
        u32BaseAddr = MS_IPM_BASE0(eWindow);
        u32BuffSize = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow];
    }
    else
    {
        u32BaseAddr = MS_FRCM_BASE0(eWindow);
        u32BuffSize = pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow];
    }

    if(pSrcInfo->bDisplayNineLattice)
    {
        u16DNROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment
    }
    else
    {
        if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow))
        {
            u16DNROffset = (gSrcInfo[eWindow].Status2.u16VirtualBox_Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
        }
        else
        {
        u16DNROffset = (gSrcInfo[eWindow].u16IPMFetch + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment
    }
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u16DNROffset=%u\n",u16DNROffset);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC," %d\n",(int)gSrcInfo[eWindow].u16IPMFetch);
    if (bDoMirror)
    {
#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
        Mdrv_SC_set_frcm_pre_align_pixel(pInstance, FALSE, 0, eWindow);
        if (IsHMirrorMode(eWindow))
        {
            u16AlignWidth = u16DNROffset - gSrcInfo[eWindow].u16IPMFetch;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"align width = %d\n", u16AlignWidth);

            // the hardware alignment in OPM > IPM case, it will get more data for alignment
            // disable it in the nine lattice mode
            if(pSrcInfo->bDisplayNineLattice)
            {
                Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
            }
            else
            {
                //these chips: they need sw to decide prealign or not.
                //here set to TRUE, then hw will prealign u16AlignWidth
                //if here is FALSE, hw won't do prealign

                //to fix H/W bugs, OPM need to be aligned.
                //so do not use pre-align. use OPM line buffer offset instead.
                Hal_SC_set_pre_align_pixel(pInstance, FALSE, 0, eWindow);
            }
        }
#else
        if (IsHMirrorMode(eWindow))
        {
            //M10, J2, A2 and later chip, HW will auto insert pixel to alignment u16IPMFetch
            //below u16AlignWidth is used for CBCR swap control
#ifdef K3_U2
            if (pSrcInfo->bMemFmt422)
                u16AlignWidth = u16DNROffset - pSrcInfo->u16H_SizeAfterPreScaling;
            else
                u16AlignWidth = u16DNROffset - gSrcInfo[eWindow].u16IPMFetch;;
#endif
            // the hardware alignment in OPM > IPM case, it will get more data for alignment
            // disable it in the nine lattice mode
            if(pSrcInfo->bDisplayNineLattice)
            {
                Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
            }
            else
            {
                //these chips: when sw set it to TRUE, then hw will do the prealign.
                //because hw can calculate by itself, even for the alignwidth==0 case,
                //so sw don't need to worry about it, just set it to TRUE.
                Hal_SC_set_pre_align_pixel(pInstance, TRUE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at start
            }
        }
#endif
        else
        {
            u16AlignWidth = 0;
            Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
        }
    }
    else
    {
        u16AlignWidth = 0;
        Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
        u16AlignWidth = u16DNROffset - gSrcInfo[eWindow].u16IPMFetch;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"align width in frcm = %d\n", u16AlignWidth);
        if (IsHMirrorMode(eWindow))
        {
            Mdrv_SC_set_frcm_pre_align_pixel(pInstance, TRUE, u16AlignWidth, eWindow);
        }
        else
        {
            Mdrv_SC_set_frcm_pre_align_pixel(pInstance, FALSE, 0, eWindow);
        }
    }

#ifndef DISABLE_3D_FUNCTION
        if((E_XC_3D_INPUT_SIDE_BY_SIDE_FULL == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_CHECK_BORAD == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_PIXEL_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)))
        {
            u16AlignWidth /= 2;            //opm fetch actual align size
            u16H_SizeAfterPreScaling /= 2; //actual prescaling hsize
        }
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"align width = %d\n", u16AlignWidth);
    // alignwidth must be even, other color will be wrong and need to
    // use cb cr swap to correct it.
    if((u16AlignWidth & 0x01) && (HAL_SC_get_memory_bit_fmt(pInstance, eWindow) == 0))
    {
        Hal_SC_set_422_cbcr_swap(pInstance, ENABLE, eWindow);
    }
    else
    {
        Hal_SC_set_422_cbcr_swap(pInstance, DISABLE, eWindow);
    }

    if(pSrcInfo->bDisplayNineLattice)
    {
        pSrcInfo->u16V_Length = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }

    u16DNROffsetFor3D = u16DNROffset;
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_DNROffset(pInstance, &u16DNROffsetFor3D,&u16DNROffset, eWindow);
#endif

    if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
    {
        u32Framesize = gSrcInfo[eWindow].Status2.u16VirtualBox_Height * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    }
    else
    {
        u32Framesize = (MS_U32)pSrcInfo->u16V_SizeAfterPreScaling * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    }

#ifndef DISABLE_3D_FUNCTION
#if (HW_DESIGN_3D_VER >= 2)
    if((E_XC_3D_INPUT_FRAME_PACKING == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
            && ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
#if ((HW_DESIGN_3D_VER == 3) && (HW_DESIGN_4K2K_VER == 7))
                || (pSrcInfo->stCapWin.height == DOUBLEHD_576X2I_VSIZE)
                || (pSrcInfo->stCapWin.height == DOUBLEHD_480X2I_VSIZE)
#endif
                ))
    {
        //special case, hw will use field size 540 as deinterlace size
        u32Framesize /= 2;
    }
#if (HW_DESIGN_3D_VER > 2)
    else if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {//per designer, input 3d:framealternative will double data at dnr
        u32Framesize *= 2;
    }
#endif
#endif
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>Framesize(%d)= VSizeAfterPreSD(%d) * DNROffset(%d) * BytesPer2Pixel(%d)/2\n",(int)u32Framesize,pSrcInfo->u16V_SizeAfterPreScaling,u16DNROffset,u8BytesPer2Pixel);

#ifdef UFO_XC_FB_LEVEL
    if((pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FB) && (u32Framesize * 2 > u32BuffSize))
#else
    if((!pSrcInfo->bFBL)
        && (!pSrcInfo->bR_FBL)
        && (u32Framesize * 2 > u32BuffSize))
#endif
    {
        //this case should always not be happened
        printf("Attention! Scaler DNR Buffer is not enough for this timing, eWindow=%u!\n", eWindow);
    }

    // enable 4 frame mode if input is progressive & frame buffer is enough
    // , regardless the input source and timing because 4 frame mode is better in avoid tearing
    //
    if(pSrcInfo->bDisplayNineLattice)
    {
#if (_LINEAR_ADDRESS_MODE_SUPPORTED)
        gSrcInfo[eWindow].bLinearMode = TRUE;
        Hal_SC_set_linearmem_mode(ENABLE,MAIN_WINDOW);
#endif
    }
    else
    {
        XC_FRAME_STORE_NUMBER enLastFBNum = gSrcInfo[eWindow].Status2.eFrameStoreNumber;

        /*Update Frame buffer usage status and memory linear mode*/
        // MDrv_XC_GetPQSuggestedFrameNum(eWindow) is the frame count that PQ want to set
        // gSrcInfo[eWindow].Status2.eFrameStoreNumber is the frame count that sw code(_Mdrv_SC_CalcFBNum)
        //     decides according to frame size and other condition
        if (bIsConnectOPM)
        {
            _Mdrv_XC_Refine_FrameNum(pInstance, &gSrcInfo[eWindow].bLinearMode,
                               &gSrcInfo[eWindow].Status2.eFrameStoreNumber,
                               bInterlace,
                               u32Framesize,
                               u32BuffSize,
                               eWindow);
            _MDrv_XC_Set_FrameNum(pInstance, eWindow, FALSE, gSrcInfo[eWindow].Status2.eFrameStoreNumber,bInterlace);
        }
        #if (HW_DESIGN_4K2K_VER == 4)
        else
        {
            _Mdrv_XC_Refine_FRCMFrameNum(pInstance, &gSrcInfo[eWindow].bLinearMode,
                               &gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber,
                               u32Framesize,
                               u32BuffSize,
                               eWindow);
            gSrcInfo[eWindow].Status2.eFrameStoreNumber = gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber;
            _MDrv_XC_Set_FrameNum(pInstance, eWindow, FALSE, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber,bInterlace);
            _MDrv_XC_Set_FrameNum(pInstance, eWindow, TRUE, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber,bInterlace);
        }
        #endif

        if (!IsVMirrorMode(eWindow))
        {
            if (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES)
            {
                // 3/4 value is suggested by designer, it is a experimental value.
                MS_U16 u16SwitchCnt = gSrcInfo[eWindow].stCapWin.height * 3 / 4;
                MDrv_SC_EnableReadBankSwitchControl(pInstance, ENABLE, u16SwitchCnt, eWindow);
            }
            else
            {
                MDrv_SC_EnableReadBankSwitchControl(pInstance, DISABLE, 0, eWindow);
            }
        }
        else
        {
            if (bDoMirror)
            {
                MDrv_SC_EnableReadBankSwitchControl(pInstance, DISABLE, 0, eWindow);
            }
        }

        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"gSrcInfo[eWindow].bLinearMode %d \n",gSrcInfo[eWindow].bLinearMode);
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"FrameMode %d \n",gSrcInfo[eWindow].Status2.eFrameStoreNumber);

        // Set memory frame buffer setting.
        if (bIsConnectOPM)
        {
#if (HW_DESIGN_4K2K_VER == 4)
            if (psXCInstPri->u32DeviceID == 0)
            {
                if (eWindow == MAIN_WINDOW)
                {
                    if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
                    {
                        MDrv_SC_SwitchIPMWriteBank(pInstance, FALSE, E_XC_IPM_CONNETC_MAIN_OPM);
                    }
                    else
                    {
                        MDrv_SC_SwitchIPMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_MAIN_OPM);
                    }
                }
                else
                {
                    MDrv_SC_SwitchIPMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_SUB_OPM);
                }
            }
#endif
        }
        else
        {
#if (HW_DESIGN_4K2K_VER == 4)
            if (psXCInstPri->u32DeviceID == 0)
            {
                if (eWindow == MAIN_WINDOW)
                {
                    if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
                    {
                        MDrv_SC_SwitchIPMWriteBank(pInstance, FALSE, E_XC_IPM_CONNETC_MAIN_FRCM);
                    }
                    else
                    {
                        MDrv_SC_SwitchIPMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_MAIN_FRCM);
                    }
                }
                else
                {
                    MDrv_SC_SwitchIPMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_SUB_FRCM);
                }
            }
#endif
        }

        // to avoid mm play photo garbage, because of big changing FB number
        if((enLastFBNum != gSrcInfo[eWindow].Status2.eFrameStoreNumber) && (TRUE == MDrv_XC_Get_SCMI_Type()))
        {
            MDrv_XC_WBankReset(pInstance, FALSE, eWindow);
        }

#if (_LINEAR_ADDRESS_MODE_SUPPORTED)
        // Set linear mode
        if (gSrcInfo[eWindow].bLinearMode)
        {
            Hal_SC_set_linearmem_mode(ENABLE,eWindow);
        }
        else
        {
            Hal_SC_set_linearmem_mode(DISABLE,eWindow);
        }
#endif
        // if support _FIELD_PACKING_MODE_SUPPORTED, it will control by QMAP(J2/A5/A6/A3)
#if(!_FIELD_PACKING_MODE_SUPPORTED)
        // avoid tearing problem when we can not do frame lock
        if(pSrcInfo->bRWBankAuto)
        {
#if (HW_DESIGN_3D_VER == 1)
            //this 3d method need special wr bank mapping
            if(((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                  && (E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance)))
                || ((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                   && (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Output_Mode(pInstance)))
                )
            {
                if(eWindow == MAIN_WINDOW)
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x01, eWindow);
                }
                else
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
                }
            }
            else
#endif
            // Set read/write bank as larger than 0x01 if frame count >= 3
            if (( gSrcInfo[eWindow].Status2.eFrameStoreNumber >= IMAGE_STORE_3_FRAMES)&&
                       gSrcInfo[eWindow].Status2.eFrameStoreNumber <= IMAGE_STORE_10_FRAMES)
            {
                if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, BIT(4)) != 0)
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
                else
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x01, eWindow);
            }
            else // Interlace
            {
                //For 25_4R_MC mode only
                if((!MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow)) && (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_4_FIELDS)
                    && (!MDrv_SC_IsForceFreerun(pInstance)))
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x02, eWindow);
                }
                else
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
                }
            }
        }
#endif
    }

    //u32Offset = (MS_U32)pSrcInfo->u16V_Length * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    //fix u32DNRBase1 at SCALER_DNR_BUF_LEN/2 for not using blue-screen at channel change in ATV

    if( gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES )
        u32Offset =  u32BuffSize/3;
    else
        u32Offset =  u32BuffSize/2;

#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32Offset :%lu, DNRBufSize=%lu\n", u32Offset, u32BuffSize);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32Offset :%tu, DNRBufSize=%tu\n", (ptrdiff_t)u32Offset, (ptrdiff_t)u32BuffSize);
#endif

    u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    u32DNRBase0 = u32BaseAddr;//MS_IPM_BASE0(eWindow);
#if SUPPORT_2_FRAME_MIRROR
    MS_U32 u32pitch = 0;

    if( (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES) && (!gSrcInfo[eWindow].bInterlace) )
    {
        u32pitch = ( ( (MS_U32)(pSrcInfo->u8BitPerPixel/8) * (MS_U32)gSrcInfo[eWindow].u16IPMFetch * (MS_U32)u8FrameNum * (MS_U32)(pSrcInfo->u16V_SizeAfterPreScaling - 1) ) / (MS_U32)BYTE_PER_WORD);//Pitch = Byte_per_pix*align(fetch)*2*(height-1)/byte_per_miu_word
        u32DNRBase1 = u32DNRBase0 + u32pitch;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32pitch:%x u32DNRBase1:%x \n",(int)u32pitch,(int)u32DNRBase1);
#else
    u32DNRBase1 = u32BaseAddr+ (( u32Offset/BYTE_PER_WORD + 0x01) & ~0x01L);

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>MAIN DNRBase1[%x]= MS_IPM_BASE0[%u]=(%x) + ((u32Offset[%x]/%d + 0x01) & ~0x01L)\n",(int)u32DNRBase1, eWindow,(int)u32BaseAddr,(int)u32Offset, BYTE_PER_WORD);
#endif

#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_DNRBase(pInstance,
                              &u32DNRBase0,
                              &u32DNRBase1,
                              eWindow);
#endif

    if( gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES  )
        u32DNRBase2 = u32DNRBase1 + (( u32Offset/BYTE_PER_WORD + 0x01) & ~0x01L);
    else
        u32DNRBase2 = u32DNRBase0;

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>DNRBase2[%x])\n", (int)u32DNRBase2);
#if 1
    if(pSrcInfo->bDisplayNineLattice)
    {
        u32OPMBase1 = u32OPMBase0 = u32OPMBase2 = u32DNRBase0;
    }
    else
    {
        u32OPMBase0 = u32DNRBase0;
        u32OPMBase1 = u32DNRBase1;
        u32OPMBase2 = u32DNRBase2;
#ifndef DISABLE_3D_FUNCTION
        MDrv_SC_3D_Adjust_OPMBase(pInstance,
                                  &u32OPMBase0,
                                  &u32OPMBase1,
                                  &u32OPMBase2,
                                  u32Offset,
                                  eWindow);
#endif

    }
    if (bIsConnectOPM)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0  = u32OPMBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1  = u32OPMBase1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2  = u32OPMBase2;
    }
    else
    {
#if (HW_DESIGN_4K2K_VER == 4)
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0  = u32OPMBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1  = u32OPMBase1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2  = u32OPMBase2;
#endif
    }
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>u32OPMBase0[%x]= MS_IPM_BASE0[%u][%x] => ",(int)u32OPMBase0, eWindow, (int)u32BaseAddr);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32OPMBase1[%x]= u32IPMBase1[%x] \n",(int)u32OPMBase1,(int)u32DNRBase1);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32OPMBase2[%x] \n",(int)u32OPMBase2);
#endif

    // Cal DNR V write limit
    if(pSrcInfo->bDisplayNineLattice)
    {
        u16V_Writelimit = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }
    else
    {
        u16V_Writelimit = pSrcInfo->stCapWin.height;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u16V_Writelimit:%d \n",u16V_Writelimit);
    if( bInterlace )
        u16V_Writelimit = (u16V_Writelimit+1) / 2;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> i=%d, u16V_Writelimit[%d]= (u16V_Writelimit+1) / 2\n",bInterlace,u16V_Writelimit);
    // Cal DNR write limit
  #if 1//( CHIP_FAMILY_TYPE == CHIP_FAMILY_S4LE )

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"preview =>WritelimitBase[%u][%x] = (MS_IPM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD)\n",eWindow,(int)u32WritelimitBase, eWindow, (int)u32BaseAddr,(int)u32Offset);

    if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES)
        u32WritelimitBase = (u32BaseAddr) + (((u32Offset + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD);
    else
        u32WritelimitBase = (u32BaseAddr) + (((u32Offset + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD);

     XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>WritelimitBase[%x] = (MS_IPM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD)\n",(int)u32WritelimitBase,eWindow,(int)u32BaseAddr,(int)u32Offset);
     u32WritelimitBase = (eWindow == MAIN_WINDOW) ? (u32WritelimitBase - 1) | F2_WRITE_LIMIT_EN : (u32WritelimitBase - 1) | F1_WRITE_LIMIT_EN;
     XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u32WritelimitBase[%x] = (u32WritelimitBase - 1) | 0x2000000\n",(int)u32WritelimitBase);
  #else
    u32WritelimitBase = MS_DNR_F2_BASE0 + ((u32Offset/8 + 0x01) & ~0x01L) * 2;
    u32WritelimitBase = (u32WritelimitBase - 1);
  #endif
    if (bDoMirror)
    {
        if (IsVMirrorMode(eWindow))
        {
            if(!pSrcInfo->bDisplayNineLattice)
            {
                MS_U16 u16V_SizeAfterPreScaling = pSrcInfo->u16V_SizeAfterPreScaling;
                MS_U16 u16DNROffsetTemp = u16DNROffset;
                MS_U16 u16Ratio = 1;
                MS_BOOL bMirrorBufferOverflow = FALSE;

                if(MDrv_XC_GetDynamicScalingStatus(pInstance)
#ifdef K3_U2
                    && (eWindow == MAIN_WINDOW)
#if SUPPORT_SEAMLESS_ZAPPING
                    && (pXCResourcePrivate->sthal_SC.bSeamlessZappingEnable[eWindow] == FALSE)
#endif
#endif
                  )
                {
                    //only for mm ds go into ficlk patch: gSrcInfo[eWindow].Status2.u16PreVCusScalingDst
                    //is one(progressive) or two lines(interlace)  less than gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc
                    u16V_SizeAfterPreScaling = gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc;
                }

#ifndef DISABLE_3D_FUNCTION
                MDrv_SC_3D_Adjust_MirrorDNROffset(pInstance, pSrcInfo, &u16V_SizeAfterPreScaling, &u16DNROffsetTemp, &u16Ratio, eWindow);
#endif
                //base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
                u32OneLineOffset = u8FrameNum * (MS_PHY)u16Ratio * (MS_U32)u16DNROffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;

#if SUPPORT_SEAMLESS_ZAPPING
                if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
                if( bInterlace )
#endif
                {
                    if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
                    {
                        u32BaseOffset = (MS_PHY)gSrcInfo[eWindow].Status2.u16VirtualBox_Width * (MS_PHY)((gSrcInfo[eWindow].Status2.u16VirtualBox_Height-1)/2) * (MS_PHY)u8BytesPer2Pixel/2/BYTE_PER_WORD * u8FrameNum;
                    }
                    else
                    {
                         MS_U16 u16Offset;
                        //u16Offset = u16DNROffsetTemp * MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED) / 2;
                        u16Offset = u16DNROffsetTemp * u8FrameNum / 2;
                        u32BaseOffset = (MS_PHY)(u16V_SizeAfterPreScaling-2) * (MS_PHY)u16Ratio * (MS_PHY)u16Offset * (MS_PHY)u8BytesPer2Pixel/2/BYTE_PER_WORD;
                    }
                }
                else
                {
                    if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
                    {
                        u32BaseOffset = (MS_PHY)gSrcInfo[eWindow].Status2.u16VirtualBox_Width * (MS_PHY)(gSrcInfo[eWindow].Status2.u16VirtualBox_Height -1) * (MS_PHY)u8BytesPer2Pixel/2/BYTE_PER_WORD * u8FrameNum;
                    }
                    else
                    {
                        //Progressived mode, non-linear address
                        //base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
                        u32BaseOffset = (MS_PHY)((MS_PHY)u16Ratio * u8FrameNum*(u16V_SizeAfterPreScaling-1)) * (MS_PHY)u16DNROffsetTemp * (MS_PHY)u8BytesPer2Pixel/2/BYTE_PER_WORD;
                    }
                }

                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>u16Ratio[%u] * u8FrameNum[%u] *(u16V_SizeAfterPreScaling-1)[%u]*u16DNROffsetTemp[%u]*u8BytesPer2Pixel[%u]\n",
                    u16Ratio,u8FrameNum,(u16V_SizeAfterPreScaling-1),u16DNROffsetTemp,u8BytesPer2Pixel);

                if (bIsConnectOPM)
                    MDrv_SC_set_DNRBaseOffset(pInstance, eWindow,u32BaseOffset);
                else
                    MDrv_SC_set_FRCMBaseOffset(pInstance, eWindow,u32BaseOffset);
                u32DNRBase0 += u32BaseOffset;

                //to avoid set dnr base1 out of scaler dnr memory when 2 frame mode
                if((u32DNRBase1 + u32BaseOffset) <= (u32BaseAddr + u32BuffSize/BYTE_PER_WORD - u32OneLineOffset))
                {
                    u32DNRBase1 += u32BaseOffset;
                }

                if((u32DNRBase2 + u32BaseOffset) <= (u32BaseAddr + u32BuffSize/BYTE_PER_WORD - u32OneLineOffset))
                {
                    u32DNRBase2 += u32BaseOffset;
                }

                if(MDrv_XC_Get_SCMI_Type())
                {
                    // field packing mode, only dnrbase0 is used
                    if((u32DNRBase0 < u32BaseOffset) || (u32DNRBase0 - u32BaseOffset < u32BaseAddr))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
                else //not field packing mode
                {
                    if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES)
                    {
                        // 2 frame mode, only dnrbase0 is used
                        if((u32DNRBase0 < u32BaseOffset) || (u32DNRBase0 - u32BaseOffset < u32BaseAddr))
                        {
                            bMirrorBufferOverflow = TRUE;
                        }
                    }
                    else if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES)
                    {
                        // 3 frame mode, dnrbase0, dnrbase1, dnrbase2 is used
                        if((u32DNRBase0 < u32BaseOffset)
                           || (u32DNRBase1 < u32BaseOffset)
                           || (u32DNRBase2 < u32BaseOffset)
                           || (u32DNRBase0 - u32BaseOffset < u32BaseAddr)
                           || (u32DNRBase1 - u32BaseOffset < u32BaseAddr)
                           || (u32DNRBase2 - u32BaseOffset < u32BaseAddr))
                        {
                            bMirrorBufferOverflow = TRUE;
                        }
                    }
                    else
                    {
                        //other frame mode, dnr base0, dnr base1 is used
                        if((u32DNRBase0 < u32BaseOffset)
                           || (u32DNRBase1 < u32BaseOffset)
                           || (u32DNRBase0 - u32BaseOffset < u32BaseAddr)
                           || (u32DNRBase1 - u32BaseOffset < u32BaseAddr))
                        {
                            bMirrorBufferOverflow = TRUE;
                        }
                    }
                }

                if(bMirrorBufferOverflow)
                {
                    printf("[%s,%5d] Attention! Scaler memory is overflow!\n",__FUNCTION__,__LINE__);
                    MS_ASSERT(0);
                }
            }

            u32WritelimitBase = u32BaseAddr - 1;

            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase= u32WritelimitBase | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
            else
                u32WritelimitBase= u32WritelimitBase | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
        }
    }

    //store FrameNum
    MDrv_XC_Save_FrameNumFactor(pInstance, eWindow,
               MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED));

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen = pSrcInfo->u16V_Length;
    if(eWindow == MAIN_WINDOW)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = u16V_Writelimit | F2_V_WRITE_LIMIT_EN;

        //DS need set FRCM_W limit here
        if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VFRCMWritelimit = u16V_Writelimit | F2_V_WRITE_LIMIT_EN;
        }
    }
    else
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = u16V_Writelimit | F1_V_WRITE_LIMIT_EN;

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32WritelimitBase = u32WritelimitBase;

#if (HW_DESIGN_4K2K_VER == 4)
    static MS_PHY u32PreviousWritelimitBase = 0;
    /// need to check the IPM write base, if it used the monaco 4K mode, the IPM need to change to frcm base!
    if(u32WritelimitBase != u32PreviousWritelimitBase)
	    MDrv_SC_set_write_limit(pInstance, u32WritelimitBase, eWindow);
    u32PreviousWritelimitBase = u32WritelimitBase;
#endif

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = u16DNROffset;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch  = gSrcInfo[eWindow].u16IPMFetch;

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0  = u32DNRBase0;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1  = u32DNRBase1;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase2  = u32DNRBase2;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = u16DNROffset;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16AlignWidth = u16AlignWidth;
#if (HW_DESIGN_4K2K_VER == 4)
    if ((psXCInstPri->u32DeviceID==0) && !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset = u16DNROffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset = u16DNROffset;
    }
#endif
    pSrcInfo->u32IPMBase0 = u32DNRBase0 * BYTE_PER_WORD;
    pSrcInfo->u32IPMBase1 = u32DNRBase1 * BYTE_PER_WORD;
    pSrcInfo->u32IPMBase2 = u32DNRBase2 * BYTE_PER_WORD;
    pSrcInfo->u16IPMOffset = u16DNROffset;
    *pu32OneLineOffset = u32OneLineOffset;

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final DNR Offset/Fetch=0x%x, 0x%x, OPM Offset=0x%x, V_Length=0x%x\n",
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen
                 );
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final u32DNRBase0/1/2=0x%lx, 0x%lx, 0x%lx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase2
                                      );
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final u32DNRBase0/1/2=0x%tx, 0x%tx, 0x%tx\n",
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase2
                                      );
#endif
#if (HW_DESIGN_4K2K_VER == 4)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC," Final u16FRCM_ROffset = 0x%x   u16FRCM_WOffset = 0x%x\n",
                 (int)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset,
                 (int)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset);
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"---------------------------------------------------\n");
}

void MDrv_SC_set_opmsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_BOOL bDoMirror, MS_BOOL bDoCrop, MS_U32 u32OneLineOffset, SCALER_WIN eWindow)
{
    MS_U16 u16LBOffset, u16DispOffset, u16OPMFetch, u16OPMFetchAlignment;
    MS_PHY u32OPMBase0, u32OPMBase1, u32OPMBase2, u32OPMCropOffset =0, u32OPMPixelOffset;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U32 u32Framesize;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16DNROffsetFor3D = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16DNROffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset;
    u16DNROffsetFor3D = u16DNROffset;

    if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
    {
        u32Framesize = gSrcInfo[eWindow].Status2.u16VirtualBox_Height * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    }
    else
    {
        u32Framesize = (MS_U32)pSrcInfo->u16V_SizeAfterPreScaling * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    }

#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_DNROffset(pInstance, &u16DNROffsetFor3D,&u16DNROffset, eWindow);

#if (HW_DESIGN_3D_VER >= 2)
    if((E_XC_3D_INPUT_FRAME_PACKING == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
            && ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
#if ((HW_DESIGN_3D_VER == 3) && (HW_DESIGN_4K2K_VER == 7))
                || (pSrcInfo->stCapWin.height == DOUBLEHD_576X2I_VSIZE)
                || (pSrcInfo->stCapWin.height == DOUBLEHD_480X2I_VSIZE)
#endif
                ))
    {
        //special case, hw will use field size 540 as deinterlace size
        u32Framesize /= 2;
    }
#if (HW_DESIGN_3D_VER > 2)
    else if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {//per designer, input 3d:framealternative will double data at dnr
        u32Framesize *= 2;
    }
#endif
#endif
#endif

    u32OPMBase0 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0;
    u32OPMBase1 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1;
    u32OPMBase2 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2;
    /////////////////////////////////////////////////
    // Set OPM Memeory Settings                    //
    /////////////////////////////////////////////////

    // Cal OPM Base address

    // compute address offset for cropping
    // cropping shift h unit: 16 pixel
    // v unit: 2 line(for interlace), 1 line(for progressive)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop info: (x,y)=(%d,%d),(width,height)=(%d,%d), interlaced = %d\n",
        gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y,
        gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height,
        bInterlace);

    if(pSrcInfo->bDisplayNineLattice)
    {
        if (IsVMirrorMode(eWindow) && bDoMirror)
        {
            if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_8_FIELDS)
            {
                u32OPMPixelOffset = (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            else if( bInterlace )
            {
                u32OPMPixelOffset = (MS_PHY)((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))/2)*u16DNROffset;
            }
            else
            {
                u32OPMPixelOffset = (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            u32OPMPixelOffset += (MS_PHY)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width
                                - (gSrcInfo[eWindow].stDispWin.x - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart + gSrcInfo[eWindow].stDispWin.width));
        }
        else
        {
            if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_8_FIELDS)
            {
                u32OPMPixelOffset = (MS_PHY)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            else if( bInterlace )
            {
                u32OPMPixelOffset = (MS_PHY)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)/2)*u16DNROffset;
            }
            else
            {
                u32OPMPixelOffset = (MS_PHY)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            u32OPMPixelOffset += (MS_PHY)(gSrcInfo[eWindow].stDispWin.x - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart);
        }
    }
    else
    {
        if (bDoCrop)
        {
#if SUPPORT_SEAMLESS_ZAPPING
            if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
            if( bInterlace )
#endif
            {
                u32OPMPixelOffset = (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.y/2) * u16DNROffsetFor3D;
            }
            else
            {
                u32OPMPixelOffset = (MS_PHY)((gSrcInfo[eWindow].ScaledCropWin.y) * u16DNROffsetFor3D);
            }
            u32OPMPixelOffset += (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.x);
        }
        else
        {
            u32OPMPixelOffset = 0;
        }

#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
        if (IsVMirrorMode(eWindow) && bDoMirror)
        {
            u32OPMPixelOffset += pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16AlignWidth;
        }
#else
        if (IsVMirrorMode(eWindow) && pSrcInfo->bMemFmt422 && bDoMirror)
        {
            u32OPMPixelOffset += (u16DNROffset - pSrcInfo->u16H_SizeAfterPreScaling);
        }
#endif
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts %td\n",(ptrdiff_t)u32OPMPixelOffset);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop info: (x,y)=(%d,%d),(width,height)=(%d,%d), interlaced = %d\n",
        gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y,
        gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height,
        bInterlace);

    // calculate LB offset
    u16LBOffset = u32OPMPixelOffset % OFFSET_PIXEL_ALIGNMENT;            // these pixels rely on line buffer offset
    gSrcInfo[eWindow].Status2.u16LBOffset = u16LBOffset;
    u32OPMPixelOffset -= u16LBOffset;                                    // these pixels rely on base offset

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"LB offset %d, Pixel offset %d Alighment: %d bytes \n", u16LBOffset, (int)u32OPMPixelOffset,OFFSET_PIXEL_ALIGNMENT);

    // calcuate OPM offset
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"eFrameStoreNumber %d\n", (gSrcInfo[eWindow].Status2.eFrameStoreNumber));
    u32OPMPixelOffset *= MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
#ifdef K3_U2
    if(Hal_SC_IsPX2MemFormat(pInstance, eWindow))
    {
        u32OPMPixelOffset *= 2;
    }
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"bLinearMode = %s\n",gSrcInfo[eWindow].bLinearMode ?("TRUE"):("FALSE"));
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts for field/frame %td (eFrameStoreNumber: %d)\n", (ptrdiff_t)u32OPMPixelOffset, gSrcInfo[eWindow].Status2.eFrameStoreNumber);

    if(u32OPMPixelOffset != 0)
    {
        MS_U8 u8BytesPer2PixelTemp = u8BytesPer2Pixel;
#ifndef DISABLE_3D_FUNCTION
        MDrv_SC_3D_Adjust_BytesPer2Pixel(pInstance, &u8BytesPer2PixelTemp,eWindow);
#endif
        u32OPMCropOffset = u32OPMPixelOffset * u8BytesPer2PixelTemp / 2 / BYTE_PER_WORD;
#ifdef _FIELD_PACKING_MODE_SUPPORTED_MULTIPLE_TEMP
        //Attention, this is for temp solve crop address offset error in Kappa. Remove this if Kappa code flow has refined.
        printf("[Kappa_temp_patch] u32OPMCropOffset*=2\n");
        u32OPMCropOffset *= 2;
#endif
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f.LineBuf offset %d, OPMOffset 0x%tx\n", u16LBOffset, (ptrdiff_t)u32OPMCropOffset);

    //KT: T3 PIP HW issue, When SUB ON, the HW operation will decrease "u16LBOffset" 2 times.
    MS_U16 CropWinWidthTemp = 0;
    if (bDoCrop)
    {
        CropWinWidthTemp = gSrcInfo[eWindow].ScaledCropWin.width;
    }
    else
    {
        CropWinWidthTemp = gSrcInfo[eWindow].u16H_SizeAfterPreScaling;
    }
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_CropWinWidth(pInstance, &CropWinWidthTemp,eWindow);
#endif
    u16OPMFetch = CropWinWidthTemp + u16LBOffset; // using LBOffset still need to add some points to avoid generating garbage

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[%s,%5d] u16OPMFetch:%d  \n", __func__,__LINE__,u16OPMFetch);

    if(pSrcInfo->bDisplayNineLattice) // for display ninelattice no line buffer offset
    {
        u16LBOffset = u16DispOffset=0;
        u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width + 1) & ~0x1;
#ifdef K3_U2
        u16OPMFetch += HW_PIP_BUG_SW_PATCH_FETCH_INCREASE; // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
#endif
    }
    else
    {
        //  IPM offset / IPM fetch /OPM offset: All project --> 32pix align
        //  OPM fetch:
        //            Janus /T7 --> 32pix align
        //            Others ( T8/T9/T12/T13/m10/J2 ) --> 2pix align
#ifndef DISABLE_3D_FUNCTION
        MS_U16 u16HShift = 0;
#endif
        MS_U16 u16tempOPM;
        if((u16LBOffset & 0x1) != 0)
        {
            //HW issue for OPM fetch&422to444: when lboffset not even, OPM should add 2 to avoid right line garbage.
            //if box chip need define HW_OPM_FETCH_NEEDPLUS2
#ifdef HW_OPM_FETCH_NEEDPLUS2
            u16tempOPM = (( u16OPMFetch + 1 ) & ~1) + 2;
#else
            u16tempOPM = (( u16OPMFetch + 1 ) & ~1);
#endif
            if (!bDoCrop || (u16tempOPM > pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset)))
            {
                u16LBOffset--;//Overflow, set lb offset even and alignment OPM
                u16OPMFetch = u16OPMFetch & ~1; //Make even directly, since lb offset has minus 1
            }
            else
            {
                u16OPMFetch = u16tempOPM;
            }
        }
        else if((u16OPMFetch != gSrcInfo[eWindow].u16IPMFetch)
#ifndef DISABLE_3D_FUNCTION
            &&  (E_XC_3D_OUTPUT_MODE_NONE == MDrv_XC_Get_3D_Output_Mode(pInstance))
#endif
        )
        {
            //UCDi need grab six more pixels, not 3D case.(a7p need 6 more, others need 2 more)
            u16tempOPM = (( u16OPMFetch + 1 ) & ~1) + 6;
            if (!bDoCrop || (u16tempOPM > pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset)))
            {
                u16OPMFetch = u16OPMFetch & ~1;
            }
            else
            {
                u16OPMFetch = u16tempOPM;
            }
        }
        else
        {
            u16OPMFetch = ( u16OPMFetch + 1 ) & ~1;  //  pixels  alignment
        }
#ifdef K3_U2
        u16OPMFetch += HW_PIP_BUG_SW_PATCH_FETCH_INCREASE; // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
#endif
        if (!pSrcInfo->bMemFmt422)
        {
            u16OPMFetchAlignment = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
            if (bDoCrop)
            {
                //HW issue for OPM fetch when OP write enable, OPM Fetch have to offset pixels alignment avoid green garbage .
                if (pSrcInfo->u16H_SizeAfterPreScaling >= (u16OPMFetchAlignment+gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
                {
                    u16OPMFetch = u16OPMFetchAlignment;
                }
                else
                {
                    //Overflow, set the OPM Fetch to the max data area .
                    u16OPMFetch = pSrcInfo->u16H_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.x + u16LBOffset;
                    u16OPMFetch = u16OPMFetch  & ~1;
                }
            }
            else
            {
                u16OPMFetch = u16OPMFetch  & ~1;
            }
        }
        else
        {
            if(2200>=u16OPMFetch)   //Nike 4k2k opm fetch can't do ALIGNMENT
                u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
        }
#ifndef DISABLE_3D_FUNCTION
        if(MDrv_SC_3D_Is2Dto3DCase(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance)))
        {
            u16HShift = MDrv_XC_Get_3D_HShift(pInstance);
            if ((u16OPMFetch + u16HShift) > gSrcInfo[eWindow].u16IPMFetch)
            {
                if(gSrcInfo[eWindow].u16IPMFetch > u16HShift)
                {
                    //opmfetch need align although ipmfetch may not need align
                    u16OPMFetch = ( gSrcInfo[eWindow].u16IPMFetch  - u16HShift + 1 ) & ~1;
                }
                else
                {
                    u16OPMFetch = 0;
                }
            }
        }
        else
#endif
        {
            if(u16OPMFetch > pSrcInfo->u16H_SizeAfterPreScaling  - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"!!!Alert OPMFetch(%u) Overflowed, check crop setting!!!\n", u16OPMFetch);
        }
    }

#ifdef K3_U2
    // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
    if(eWindow == MAIN_WINDOW)
    {
        if (u16OPMFetch > MST_LINE_BFF_MAX)
        {
            u16OPMFetch = MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign OPM fetch as %lu due to HW limitation \n", MST_LINE_BFF_MAX);
        }
    }
    else
    {
        if(u16OPMFetch > SUB_MST_LINE_BFF_MAX)
        {
            u16OPMFetch = SUB_MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign OPM fetch as %lu due to HW limitation \n", SUB_MST_LINE_BFF_MAX);
        }
    }
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f1. u16OPMFetch after alignment %d, lboffset %d \n", u16OPMFetch, u16LBOffset);
    //printf("1.SC Crop width=0x%x, LBOffset=0x%x\n", gSrcInfo[eWindow].ScaledCropWin.width, u16LBOffset);
    u16DispOffset = 0;

    //printf("cropy=%u,cropx=%u,dnroffset=%u,bp2p=%bu\n", _stScaledCropWin.y, _stScaledCropWin.x, u16DNROffset, u8BytesPer2Pixel);
    //printf("2.OPM Crop Offset=0x%lx, LBOffset=0x%x\n", u32OPMCropOffset, u16LBOffset);

    // adjsut OPMBase address for cropping
    if(pSrcInfo ->bDisplayNineLattice)
    {
    // for linear mode set all base address the same
        u32OPMBase0 += u32OPMCropOffset;
        //u32DNRBase1 += u32OPMCropOffset;
        //u32OPMBase2 += u32OPMCropOffset;
        u32OPMBase2 = u32OPMBase1 = u32OPMBase0;
        //printf("--u32DNRBase0 %lx\n",u32DNRBase0);
    }
    else
    {
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"h.=> u32OPMBase0,1=%x,%x -> +u32OPMCropOffset[%x] =",(int)u32OPMBase0,(int)u32OPMBase1,(int)u32OPMCropOffset);
        u32OPMBase0 += u32OPMCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"0:%x \n",(int)u32OPMBase0);
        u32OPMBase1 += u32OPMCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"1:%x \n",(int)u32OPMBase1);
        u32OPMBase2 += u32OPMCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"2:%x \n",(int)u32OPMBase2);
    }

#if SUPPORT_2_FRAME_MIRROR

    MS_S32 s32cropshift_base0 = 0;
    MS_S32 s32cropshift_base1 = 0;
    MS_U32 u32LBOffset_mirror = 0;
    if (bDoCrop)
    {
        s32cropshift_base0 = gSrcInfo[eWindow].ScaledCropWin.y * (MS_S32) (u16DNROffsetFor3D) + gSrcInfo[eWindow].ScaledCropWin.x;
        s32cropshift_base1 = - gSrcInfo[eWindow].ScaledCropWin.y * (MS_S32) (u16DNROffsetFor3D) + gSrcInfo[eWindow].ScaledCropWin.x;
        u32LBOffset_mirror = gSrcInfo[eWindow].ScaledCropWin.x % OFFSET_PIXEL_ALIGNMENT;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32LBOffset:%d  \n",u32LBOffset_mirror);

    s32cropshift_base0 = ( s32cropshift_base0 - u32LBOffset_mirror ) * (MS_U32) MDrv_XC_TransFrameNumToFactor(gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    s32cropshift_base1 = ( s32cropshift_base1 - u32LBOffset_mirror ) * (MS_U32) MDrv_XC_TransFrameNumToFactor(gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    s32cropshift_base0 = ( s32cropshift_base0 * (MS_U32)(pSrcInfo->u8BitPerPixel/8) ) / (MS_U32)(BYTE_PER_WORD);
    s32cropshift_base1 = ( s32cropshift_base1 * (MS_U32)(pSrcInfo->u8BitPerPixel/8) ) / (MS_U32)(BYTE_PER_WORD);

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] s32cropshift_base0:%d  s32cropshift_base1:%d \n", s32cropshift_base0,s32cropshift_base1);

    if( (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES) && (!gSrcInfo[eWindow].bInterlace) )
    {
       u32OPMBase0 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0 + s32cropshift_base0;
       u32OPMBase1 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1 + s32cropshift_base1;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32DNRBase0:%x  u32DNRBase1:%x \n", (int)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0,(int)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32OPMBase0:%x  u32OPMBase1:%x \n", (int)u32OPMBase0,(int)u32OPMBase1);

#endif
    if (bDoCrop)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset  = u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DispOffset  = u16DispOffset;
    }
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0  = u32OPMBase0;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1  = u32OPMBase1;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2  = u32OPMBase2;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch  = u16OPMFetch;

#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final OPM Fetch=0x%x,  u32OPMBase0/1/2=0x%lx, 0x%lx, 0x%lx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final OPM Fetch=0x%x,  u32OPMBase0/1/2=0x%tx, 0x%tx, 0x%tx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2);

#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"---------------------------------------------------\n");
}

void MDrv_SC_set_frcmwsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_PHY* pu32OneLineOffset, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    MS_U16 u16V_FRCMWritelimit;
    MS_PHY u32Offset, u32FRCM_WBase0, u32FRCM_WBase1, u32FRCM_WBase2, u32WritelimitBase=0;
    MS_PHY u32OneLineOffset = 0;
    MS_U16 u16AlignWidth=0;
    MS_PHY u32BaseOffset = 0;
    MS_PHY u32Framesize;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16FRCM_WOffset = 0;
    MS_U8 u8FrameNum = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    /////////////////////////////////////////////////
    // Set FRCM_W Memeory Settings                    //
    /////////////////////////////////////////////////
    // FRCM_W Fetch/Offset = OPM Fetch/Offset
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WFetch = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset;
    //gSrcInfo[eWindow].u16IPMFetch = pSrcInfo->u16H_SizeAfterPreScaling;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"===================================================\n");
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u16DNRFetch=%d ->",(int)gSrcInfo[eWindow].u16IPMFetch);

    if(pSrcInfo->bDisplayNineLattice)
    {
        u16FRCM_WOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment
    }
    else
    {
        if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
        {
            u16FRCM_WOffset = (gSrcInfo[eWindow].Status2.u16VirtualBox_Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
        }
        else
        {
        u16FRCM_WOffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment
    }
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u16FRCM_WFetch=%u  u16FRCM_WOffset=%u\n",pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WFetch,u16FRCM_WOffset);

    if(pSrcInfo->bDisplayNineLattice)
    {
        pSrcInfo->u16FRCMV_Length = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }

    if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
    {
        u32Framesize = gSrcInfo[eWindow].Status2.u16VirtualBox_Height * (MS_U32)u16FRCM_WOffset * (MS_U32)u8BytesPer2Pixel/2;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>Framesize(%d)= VirtualBox_Height(%d) * u16FRCM_WOffset(%d) * BytesPer2Pixel(%d)/2\n",(int)u32Framesize,gSrcInfo[eWindow].Status2.u16VirtualBox_Height,u16FRCM_WOffset,u8BytesPer2Pixel);
    }
    else
    {
        u32Framesize = (MS_U32)pSrcInfo->u16V_SizeAfterPreScaling * (MS_U32)u16FRCM_WOffset * (MS_U32)u8BytesPer2Pixel/2;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>Framesize(%d)= VSizeAfterPreSD(%d) * u16FRCM_WOffset(%d) * BytesPer2Pixel(%d)/2\n",(int)u32Framesize,pSrcInfo->u16V_SizeAfterPreScaling,u16FRCM_WOffset,u8BytesPer2Pixel);
    }

#ifndef DISABLE_3D_FUNCTION
#if (HW_DESIGN_3D_VER >= 2)
    if((E_XC_3D_INPUT_FRAME_PACKING == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
            && (pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
    {
        //special case, hw will use field size 540 as deinterlace size
        u32Framesize /= 2;
    }
#if (HW_DESIGN_3D_VER > 2)
    else if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {//per designer, input 3d:framealternative will double data at dnr
        u32Framesize *= 2;
    }
#endif
#endif
#endif

#ifdef UFO_XC_FB_LEVEL
    if((pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FB)
        &&  (u32Framesize * 2 > pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow])
    )
#else
    if((!pSrcInfo->bFBL)
        && (!pSrcInfo->bR_FBL)
        && (u32Framesize * 2 > pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]))
#endif
    {
        //this case should always not be happened
        printf("Attention! Scaler FRCM Buffer is not enough for this timing, eWindow=%u!\n", eWindow);
    }

    // enable 4 frame mode if input is progressive & frame buffer is enough
    // , regardless the input source and timing because 4 frame mode is better in avoid tearing
    //
    if(pSrcInfo->bDisplayNineLattice)
    {
#if (_LINEAR_ADDRESS_MODE_SUPPORTED)
        gSrcInfo[eWindow].bLinearMode = TRUE;
        Hal_SC_set_linearmem_mode(ENABLE,MAIN_WINDOW);
#endif
    }
    else
    {
        XC_FRAME_STORE_NUMBER enLastFBNum = gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber;

        /*Update Frame buffer usage status and memory linear mode*/
        // MDrv_XC_GetPQSuggestedFrameNum(eWindow) is the frame count that PQ want to set
        // gSrcInfo[eWindow].Status2.eFrameStoreNumber is the frame count that sw code(_Mdrv_SC_CalcFBNum)
        //     decides according to frame size and other condition
        _Mdrv_XC_Refine_FRCMFrameNum(pInstance, &gSrcInfo[eWindow].bLinearMode,
                           &gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber,
                           u32Framesize,
                           pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow],
                           eWindow);

        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"gSrcInfo[eWindow].bLinearMode %d \n",gSrcInfo[eWindow].bLinearMode);
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"FrameMode %d \n",gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber);

        if (pSrcInfo->bInterlace && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) ))
        {
            // hw limition, interlace mode and open ds, scm and frcm frame number need same
            if (gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber > gSrcInfo[eWindow].Status2.eFrameStoreNumber)
            {
                printf("[MDrv_SC_set_frcmwsetting] Waring!!! eFRCMFrameStoreNumber > eFrameStoreNumber!\n");
                gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber = gSrcInfo[eWindow].Status2.eFrameStoreNumber;
                _MDrv_XC_Set_FrameNum(pInstance, eWindow, FALSE, gSrcInfo[eWindow].Status2.eFrameStoreNumber, pSrcInfo->bInterlace);
            }
            else if (gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber < gSrcInfo[eWindow].Status2.eFrameStoreNumber)
            {
                printf("[MDrv_SC_set_frcmwsetting] Waring!!! eFRCMFrameStoreNumber < eFrameStoreNumber!\n");
                gSrcInfo[eWindow].Status2.eFrameStoreNumber = gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber;
            }
        }
        // Set memory frame buffer setting.
        _MDrv_XC_Set_FrameNum(pInstance, eWindow, TRUE, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, FALSE);

        //store FrameNum
        MDrv_XC_Save_FRCMFrameNumFactor(pInstance, eWindow,
               MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED));

        // to avoid mm play photo garbage, because of big changing FB number
        if((enLastFBNum != gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber) && (TRUE == MDrv_XC_Get_SCMI_Type()))
        {
            MDrv_XC_WBankReset(pInstance, TRUE, eWindow);
        }

        // if support _FIELD_PACKING_MODE_SUPPORTED, it will control by QMAP(J2/A5/A6/A3)
#if(!_FIELD_PACKING_MODE_SUPPORTED)
        // avoid tearing problem when we can not do frame lock
        if(pSrcInfo->bRWBankAuto)
        {
#if (HW_DESIGN_3D_VER == 1)
            //this 3d method need special wr bank mapping
            if(((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                  && (E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance)))
                || ((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                   && (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Output_Mode(pInstance)))
                )
            {
                if(eWindow == MAIN_WINDOW)
                {
                    Hal_SC_set_frcm_wr_bank_mapping(pInstance, 0x01, eWindow);
                }
                else
                {
                    Hal_SC_set_frcm_wr_bank_mapping(pInstance, 0x00, eWindow);
                }
            }
            else
#endif
            // Set read/write bank as larger than 0x01 if frame count >= 3
            if (( gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber >= IMAGE_STORE_3_FRAMES)&&
                       gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber <= IMAGE_STORE_10_FRAMES)
            {
                if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_04_L, BIT(4)) != 0)
                    Hal_SC_set_frcm_wr_bank_mapping(pInstance, 0x00, eWindow);
                else
                    Hal_SC_set_frcm_wr_bank_mapping(pInstance, 0x01, eWindow);
            }
            else // Interlace
            {
                //For 25_4R_MC mode only
                if((!MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow)) && (gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_4_FIELDS)
                    && (!MDrv_SC_IsForceFreerun(pInstance)))
                {
                    Hal_SC_set_frcm_wr_bank_mapping(pInstance, 0x02, eWindow);
                }
                else
                {
                    Hal_SC_set_frcm_wr_bank_mapping(pInstance, 0x00, eWindow);
                }
            }
        }
#endif
    }

    //u32Offset = (MS_U32)pSrcInfo->u16V_Length * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    //fix u32DNRBase1 at SCALER_DNR_BUF_LEN/2 for not using blue-screen at channel change in ATV

    if( gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_3_FRAMES)
        u32Offset =  pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]/3;
    else
        u32Offset =  pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]/2;
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32Offset :%lu, DNRBufSize=%lu\n", u32Offset, pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32Offset :%tu, DNRBufSize=%tu\n", (ptrdiff_t)u32Offset, (ptrdiff_t)pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]);
#endif

    u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    u32FRCM_WBase0 = MS_FRCM_BASE0(eWindow);
#if SUPPORT_2_FRAME_MIRROR
    MS_U32 u32pitch = 0;

    if (gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_2_FRAMES)
    {
        u32pitch = ( ( (MS_U32)(pSrcInfo->u8BitPerPixel/8) * (MS_U32)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WFetch * (MS_U32)u8FrameNum * (MS_U32)(pSrcInfo->u16V_SizeAfterPreScaling - 1) ) / (MS_U32)BYTE_PER_WORD);//Pitch = Byte_per_pix*align(fetch)*2*(height-1)/byte_per_miu_word
        u32FRCM_WBase1 = u32FRCM_WBase0 + u32pitch;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32pitch:%x u32DNRBase1:%x \n",(int)u32pitch,(int)u32DNRBase1);
#else
    u32FRCM_WBase1 = MS_FRCM_BASE0(eWindow) + (( u32Offset/BYTE_PER_WORD + 0x01) & ~0x01L);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>MAIN u32FRCM_WBase1[%x]= MS_FRCM_BASE0[%u]=(%x) + ((u32Offset[%x]/%d + 0x01) & ~0x01L)\n",(int)u32FRCM_WBase1, eWindow,(int)MS_FRCM_BASE0(eWindow),(int)u32Offset, BYTE_PER_WORD);
#endif

    if( gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_3_FRAMES)
        u32FRCM_WBase2 = u32FRCM_WBase1 + (( u32Offset/BYTE_PER_WORD + 0x01) & ~0x01L);
    else
        u32FRCM_WBase2 = u32FRCM_WBase0;

    if(pSrcInfo->bDisplayNineLattice)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0  = u32FRCM_WBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1  = u32FRCM_WBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2  = u32FRCM_WBase0;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0  = u32FRCM_WBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1  = u32FRCM_WBase1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2  = u32FRCM_WBase2;
    }

    // Cal FRCM V write limit
    if(pSrcInfo->bDisplayNineLattice)
    {
        u16V_FRCMWritelimit = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }
    else
    {
        u16V_FRCMWritelimit = pSrcInfo->stCapWin.height;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u16V_Writelimit[%d]= u16V_CapSize=%d \n",u16V_FRCMWritelimit,pSrcInfo->stCapWin.height);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u16V_Writelimit[%d]= (u16V_Writelimit+1) / 2\n",u16V_FRCMWritelimit);
    // Cal FRCM write limit
  #if 1//( CHIP_FAMILY_TYPE == CHIP_FAMILY_S4LE )

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"preview =>WritelimitBase[%u][%x] = (MS_FRCM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD)\n",eWindow,(int)u32WritelimitBase, eWindow, (int)MS_FRCM_BASE0(eWindow) ,(int)u32Offset);

    if(gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_3_FRAMES)
        u32WritelimitBase = (MS_FRCM_BASE0(eWindow)) + (((u32Offset + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD);
    else
        u32WritelimitBase = (MS_FRCM_BASE0(eWindow)) + (((u32Offset + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD);

     XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>WritelimitBase[%x] = (MS_FRCM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD)\n",(int)u32WritelimitBase,eWindow,(int)MS_FRCM_BASE0(eWindow),(int)u32Offset);
     u32WritelimitBase = (eWindow == MAIN_WINDOW) ? (u32WritelimitBase - 1) | F2_WRITE_LIMIT_EN : (u32WritelimitBase - 1) | F1_WRITE_LIMIT_EN;
     XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u32WritelimitBase[%x] = (u32WritelimitBase - 1) | 0x2000000\n",(int)u32WritelimitBase);
  #else
    u32WritelimitBase = MS_DNR_F2_BASE0 + ((u32Offset/8 + 0x01) & ~0x01L) * 2;
    u32WritelimitBase = (u32WritelimitBase - 1);
  #endif

    if (IsVMirrorMode(eWindow))
    {
        if(!pSrcInfo->bDisplayNineLattice)
        {
            MS_U16 u16V_SizeAfterPreScaling = pSrcInfo->u16V_SizeAfterPreScaling;
            MS_U16 u16FRCM_WOffsetTemp = u16FRCM_WOffset;
            MS_U16 u16Ratio = 1;
            MS_BOOL bMirrorBufferOverflow = FALSE;
// DS how do it?
#if 0
            if(MDrv_XC_GetDynamicScalingStatus(pInstance)
#ifdef K3_U2
                && (eWindow == MAIN_WINDOW)
#if SUPPORT_SEAMLESS_ZAPPING
                && (pXCResourcePrivate->sthal_SC.bSeamlessZappingEnable[eWindow] == FALSE)
#endif
#endif
              )
            {
                //only for mm ds go into ficlk patch: gSrcInfo[eWindow].Status2.u16PreVCusScalingDst
                //is one(progressive) or two lines(interlace)  less than gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc
                u16V_SizeAfterPreScaling = gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc;
            }
#endif

#ifndef DISABLE_3D_FUNCTION
            MDrv_SC_3D_Adjust_MirrorDNROffset(pInstance, pSrcInfo, &u16V_SizeAfterPreScaling, &u16FRCM_WOffsetTemp, &u16Ratio, eWindow);
#endif
            //base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
            u32OneLineOffset = u8FrameNum * (MS_U32)u16Ratio * (MS_U32)u16FRCM_WOffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;

            //Progressived mode, non-linear address
            //base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
            u32BaseOffset = (MS_U32)((MS_U32)u16Ratio * u8FrameNum*(u16V_SizeAfterPreScaling-1)) * (MS_U32)u16FRCM_WOffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;

            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>u16Ratio[%u] * u8FrameNum[%u] *(u16V_SizeAfterPreScaling-1)[%u]*u16FRCM_WOffsetTemp[%u]*u8BytesPer2Pixel[%u]\n",
                u16Ratio,u8FrameNum,(u16V_SizeAfterPreScaling-1),u16FRCM_WOffsetTemp,u8BytesPer2Pixel);

            MDrv_SC_set_FRCMBaseOffset(pInstance, eWindow,u32BaseOffset);

            u32FRCM_WBase0 += u32BaseOffset;

            //to avoid set dnr base1 out of scaler dnr memory when 2 frame mode
            if((u32FRCM_WBase1 + u32BaseOffset) <= (MS_FRCM_BASE0(eWindow) + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]/BYTE_PER_WORD - u32OneLineOffset))
            {
                u32FRCM_WBase1 += u32BaseOffset;
            }

            if((u32FRCM_WBase2 + u32BaseOffset) <= (MS_FRCM_BASE0(eWindow) + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]/BYTE_PER_WORD - u32OneLineOffset))
            {
                u32FRCM_WBase2+= u32BaseOffset;
            }

            if(MDrv_XC_Get_SCMI_Type())
            {
                // field packing mode, only dnrbase0 is used
                if((u32FRCM_WBase0 < u32BaseOffset) || (u32FRCM_WBase0 - u32BaseOffset < MS_FRCM_BASE0(eWindow)))
                {
                    bMirrorBufferOverflow = TRUE;
                }
            }
            else //not field packing mode
            {
                if(gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_2_FRAMES)
                {
                    // 2 frame mode, only dnrbase0 is used
                    if((u32FRCM_WBase0 < u32BaseOffset) || (u32FRCM_WBase0 - u32BaseOffset < MS_FRCM_BASE0(eWindow)))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
                else if(gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_3_FRAMES)
                {
                    // 3 frame mode, dnrbase0, dnrbase1, dnrbase2 is used
                    if((u32FRCM_WBase0 < u32BaseOffset)
                       || (u32FRCM_WBase1 < u32BaseOffset)
                       || (u32FRCM_WBase2 < u32BaseOffset)
                       || (u32FRCM_WBase0 - u32BaseOffset < MS_FRCM_BASE0(eWindow))
                       || (u32FRCM_WBase1 - u32BaseOffset < MS_FRCM_BASE0(eWindow))
                       || (u32FRCM_WBase2 - u32BaseOffset < MS_FRCM_BASE0(eWindow)))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
                else
                {
                    //other frame mode, dnr base0, dnr base1 is used
                    if((u32FRCM_WBase0< u32BaseOffset)
                       || (u32FRCM_WBase1 < u32BaseOffset)
                       || (u32FRCM_WBase0- u32BaseOffset < MS_FRCM_BASE0(eWindow))
                       || (u32FRCM_WBase1 - u32BaseOffset < MS_FRCM_BASE0(eWindow)))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
            }

            if(bMirrorBufferOverflow)
            {
                printf("[%s,%5d] Attention! Scaler memory is overflow!\n",__FUNCTION__,__LINE__);
                MS_ASSERT(0);
            }
        }

        u32WritelimitBase = MS_FRCM_BASE0(eWindow) - 1;

        if(eWindow == MAIN_WINDOW)
            u32WritelimitBase= u32WritelimitBase | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
        else
            u32WritelimitBase= u32WritelimitBase | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
    }

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCMVLen = pSrcInfo->u16FRCMV_Length;
    if(eWindow == MAIN_WINDOW)
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VFRCMWritelimit = u16V_FRCMWritelimit | F2_V_WRITE_LIMIT_EN;
    else
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VFRCMWritelimit = u16V_FRCMWritelimit | F1_V_WRITE_LIMIT_EN;

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCMWritelimitBase = u32WritelimitBase;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset = u16FRCM_WOffset;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WFetch  = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch;;

    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase0  = u32FRCM_WBase0;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase1  = u32FRCM_WBase1;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase2  = u32FRCM_WBase2;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16AlignWidth = u16AlignWidth;
    *pu32OneLineOffset = u32OneLineOffset;

    //-----------------------------------------------------------------
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final FRCM_W Offset/Fetch=0x%x, 0x%x, FRCM_R Offset=0x%x,  FRCM_VLen=%d\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WFetch,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_ROffset,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCMVLen
                                      );
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final u32FRCM_WBase0/1/2=0x%lx, 0x%lx, 0x%lx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase0,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase1,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase2
                                      );
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final u32FRCM_WBase0/1/2=0x%tx, 0x%tx, 0x%tx\n",
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase0,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase1,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase2
                                      );
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"---------------------------------------------------\n");
#endif
}

void MDrv_SC_set_frcmrsetting(void *pInstance, XC_InternalStatus *pSrcInfo, MS_PHY u32OneLineOffset, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    MS_U16 u16LBOffset, u16DispOffset, u16FRCM_RFetch, u16FRC_RFetchAlignment;
    MS_PHY u32FRCM_RBase0, u32FRCM_RBase1, u32FRCM_RBase2, u32FRCM_RCropOffset =0, u32FRCM_RPixelOffset;
    MS_U16 u16AlignWidth=0;
    MS_PHY u32BaseOffset = 0;
    MS_PHY u32Framesize;
    MS_U8 u8BytesPer2Pixel = pSrcInfo->u8BitPerPixel * 2 / 8;
    MS_U16 u16FRCM_ROffset = 0;
    MS_U8 u8FrameNum = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u16FRCM_ROffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset;
    u32FRCM_RBase0 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0;
    u32FRCM_RBase1 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1;
    u32FRCM_RBase2 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2;

    u32BaseOffset = pXCResourcePrivate->stdrvXC_Scaling.u32FRCMBaseOffset[eWindow];

    if(MDrv_XC_Is_SupportSWDS(pInstance) && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)))
    {
        u32Framesize = gSrcInfo[eWindow].Status2.u16VirtualBox_Height * (MS_U32)u16FRCM_ROffset * (MS_U32)u8BytesPer2Pixel/2;
    }
    else
    {
        u32Framesize = (MS_U32)pSrcInfo->u16V_SizeAfterPreScaling * (MS_U32)u16FRCM_ROffset * (MS_U32)u8BytesPer2Pixel/2;
    }

#ifndef DISABLE_3D_FUNCTION
#if (HW_DESIGN_3D_VER >= 2)
    if((E_XC_3D_INPUT_FRAME_PACKING == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
            && (pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE))
    {
        //special case, hw will use field size 540 as deinterlace size
        u32Framesize /= 2;
    }
#if (HW_DESIGN_3D_VER > 2)
    else if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {//per designer, input 3d:framealternative will double data at dnr
        u32Framesize *= 2;
    }
#endif
#endif
#endif

    u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    /////////////////////////////////////////////////
    // Set FRCM_R Memeory Settings                    //
    /////////////////////////////////////////////////

    // Cal FRCM_R Base address

    // compute address offset for cropping
    // cropping shift h unit: 16 pixel
    // v unit: 2 line(for interlace), 1 line(for progressive)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop info: (x,y)=(%d,%d),(width,height)=(%d,%d), FRCM always progressive\n",
        gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y,
        gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height
        );

    // how to do?
    #if 0
    if(pSrcInfo->bDisplayNineLattice)
    {
        if (IsVMirrorMode(eWindow))
        {
            if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_8_FIELDS)
            {
                u32OPMPixelOffset = (MS_U32)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            else if( bInterlace )
            {
                u32OPMPixelOffset = (MS_U32)((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))/2)*u16DNROffset;
            }
            else
            {
                u32OPMPixelOffset = (MS_U32)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            u32OPMPixelOffset += (MS_U32)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width
                                - (gSrcInfo[eWindow].stDispWin.x - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart + gSrcInfo[eWindow].stDispWin.width));
        }
        else
        {
            if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_8_FIELDS)
            {
                u32OPMPixelOffset = (MS_U32)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            else if( bInterlace )
            {
                u32OPMPixelOffset = (MS_U32)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)/2)*u16DNROffset;
            }
            else
            {
                u32OPMPixelOffset = (MS_U32)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            u32OPMPixelOffset += (MS_U32)(gSrcInfo[eWindow].stDispWin.x - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart);
        }
    }
    else
    #endif
    {
#ifndef DISABLE_3D_FUNCTION
        MS_U16 u16FRCM_ROffset3D = u16FRCM_ROffset;
        MDrv_SC_3D_Adjust_DNROffset(pInstance, &u16FRCM_ROffset3D,&u16FRCM_ROffset, eWindow);
#endif

		if (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow))
		{
				MS_U32 u32FRCM_Mirror_offset;
				u32FRCM_RPixelOffset = (MS_U32) ((gSrcInfo[eWindow].ScaledCropWin.y) * gSrcInfo[eWindow].Status2.u16VirtualBox_Width);
				u32FRCM_RPixelOffset += (MS_U32)(gSrcInfo[eWindow].ScaledCropWin.x);

				switch(gSrcInfo[(eWindow)].Status2.eMirrorMode)
				{
					case MIRROR_H_ONLY:
						u32FRCM_Mirror_offset = (MS_U32)(gSrcInfo[eWindow].Status2.u16VirtualBox_Width - pSrcInfo->u16H_SizeAfterPreScaling);
						break;
					case MIRROR_V_ONLY:
						u32FRCM_Mirror_offset = (MS_U32) ((gSrcInfo[eWindow].Status2.u16VirtualBox_Height - pSrcInfo->u16V_SizeAfterPreScaling) * gSrcInfo[eWindow].Status2.u16VirtualBox_Width);
						break;
					case MIRROR_HV:
						u32FRCM_Mirror_offset = (MS_U32) ((gSrcInfo[eWindow].Status2.u16VirtualBox_Height - pSrcInfo->u16V_SizeAfterPreScaling) * gSrcInfo[eWindow].Status2.u16VirtualBox_Width);
						u32FRCM_Mirror_offset += (MS_U32)(gSrcInfo[eWindow].Status2.u16VirtualBox_Width - pSrcInfo->u16H_SizeAfterPreScaling);
						break;
					default:
						u32FRCM_Mirror_offset = 0;
						break;
				}
            u32FRCM_RPixelOffset += u32FRCM_Mirror_offset;
		}
		else
        {
            u32FRCM_RPixelOffset = (MS_U32)((gSrcInfo[eWindow].ScaledCropWin.y) * u16FRCM_ROffset3D);
            u32FRCM_RPixelOffset += (MS_U32)(gSrcInfo[eWindow].ScaledCropWin.x);

            if (IsHMirrorMode(eWindow) && pSrcInfo->bMemFmt422)
            {
#ifndef DISABLE_3D_FUNCTION
                MDrv_SC_3D_Adjust_PixelOffset(pInstance, pSrcInfo,&u32FRCM_RPixelOffset,&u16FRCM_ROffset,&u16FRCM_ROffset3D, TRUE, eWindow);
#else
                u32FRCM_RPixelOffset += (u16FRCM_ROffset - pSrcInfo->u16H_SizeAfterPreScaling);
#endif
            }
        }
    }

#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts %ld\n",u32FRCM_RPixelOffset);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts %td\n",(ptrdiff_t)u32FRCM_RPixelOffset);
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop info: (x,y)=(%d,%d),(width,height)=(%d,%d)\n",
        gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y,
        gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height
        );
    // calculate LB offset
    u16LBOffset = u32FRCM_RPixelOffset % OFFSET_PIXEL_ALIGNMENT;            // these pixels rely on line buffer offset
    // line buffer need to check.
    gSrcInfo[eWindow].Status2.u16LBOffset = u16LBOffset;
    u32FRCM_RPixelOffset -= u16LBOffset;                                    // these pixels rely on base offset

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"LB offset %d, Pixel offset %d Alighment: %d bytes \n", u16LBOffset, (int)u32FRCM_RPixelOffset,OFFSET_PIXEL_ALIGNMENT);

    // calcuate FRCM_R offset
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"eFRCMFrameStoreNumber %d\n", (gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber));

    u32FRCM_RPixelOffset *= MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
#ifdef K3_U2
    if(Hal_SC_IsPX2MemFormat(pInstance, eWindow))
    {
        u32FRCM_RPixelOffset *= 2;
    }
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"bLinearMode = %s\n",gSrcInfo[eWindow].bLinearMode ?("TRUE"):("FALSE"));
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts for field/frame %ld (eFRCMFrameStoreNumber: %d)\n", u32FRCM_RPixelOffset, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts for field/frame %td (eFRCMFrameStoreNumber: %d)\n", (ptrdiff_t)u32FRCM_RPixelOffset, gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber);
#endif

    if(u32FRCM_RPixelOffset != 0)
    {
        MS_U8 u8BytesPer2PixelTemp = u8BytesPer2Pixel;
#ifndef DISABLE_3D_FUNCTION
        MDrv_SC_3D_Adjust_BytesPer2Pixel(pInstance, &u8BytesPer2PixelTemp,eWindow);
#endif
        u32FRCM_RCropOffset = u32FRCM_RPixelOffset * u8BytesPer2PixelTemp / 2 / BYTE_PER_WORD;
#ifdef _FIELD_PACKING_MODE_SUPPORTED_MULTIPLE_TEMP
        //Attention, this is for temp solve crop address offset error in Kappa. Remove this if Kappa code flow has refined.
        printf("[Kappa_temp_patch] u32FRCM_RCropOffset*=2\n");
        u32FRCM_RCropOffset *= 2;
#endif
    }


#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f.LineBuf offset %d, u32FRCM_RCropOffset 0x%lx\n", u16LBOffset, u32FRCM_RCropOffset);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f.LineBuf offset %d, u32FRCM_RCropOffset 0x%tx\n", u16LBOffset, (ptrdiff_t)u32FRCM_RCropOffset);
#endif

    //KT: T3 PIP HW issue, When SUB ON, the HW operation will decrease "u16LBOffset" 2 times.

    MS_U16 CropWinWidthTemp = gSrcInfo[eWindow].ScaledCropWin.width;
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_CropWinWidth(pInstance, &CropWinWidthTemp,eWindow);
#endif
    u16FRCM_RFetch = CropWinWidthTemp + u16LBOffset; // using LBOffset still need to add some points to avoid generating garbage

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u16FRCM_RFetch:%d \n", u16FRCM_RFetch);

    if(pSrcInfo->bDisplayNineLattice) // for display ninelattice no line buffer offset
    {
        u16LBOffset = u16DispOffset=0;
        u16FRCM_RFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width + 1) & ~0x1;
#ifdef K3_U2
        u16FRCM_RFetch += HW_PIP_BUG_SW_PATCH_FETCH_INCREASE; // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
#endif
    }
    else
    {
        //  IPM offset / IPM fetch /OPM offset: All project --> 32pix align
        //  OPM fetch:
        //            Janus /T7 --> 32pix align
        //            Others ( T8/T9/T12/T13/m10/J2 ) --> 2pix align
        MS_U16 u16tempFRCM_R;

        if((u16LBOffset & 0x1) != 0)
        {
            //HW issue for OPM fetch&422to444: when lboffset not even, OPM should add 2 to avoid right line garbage
            u16tempFRCM_R = (( u16FRCM_RFetch + 1 ) & ~1) + 2;
            if(u16tempFRCM_R > pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
            {
                u16LBOffset--;//Overflow, set lb offset even and alignment OPM
                u16FRCM_RFetch = u16FRCM_RFetch & ~1; //Make even directly, since lb offset has minus 1
            }
            else
            {
                u16FRCM_RFetch = u16tempFRCM_R;
            }
        }
        else if((u16FRCM_RFetch != gSrcInfo[eWindow].u16IPMFetch)  &&  (E_XC_3D_OUTPUT_MODE_NONE == MDrv_XC_Get_3D_Output_Mode(pInstance)))
        {
            //UCDi need grab six more pixels, not 3D case.(a7p need 6 more, others need 2 more)
            u16tempFRCM_R = (( u16FRCM_RFetch + 1 ) & ~1) + 6;
            if(u16tempFRCM_R > pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
            {
                u16FRCM_RFetch = u16FRCM_RFetch & ~1;
            }
            else
            {
                u16FRCM_RFetch = u16tempFRCM_R;
            }
        }
        else
        {
            u16FRCM_RFetch = ( u16FRCM_RFetch + 1 ) & ~1;  //  pixels  alignment
        }
#ifdef K3_U2
        u16FRCM_RFetch += HW_PIP_BUG_SW_PATCH_FETCH_INCREASE; // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
#endif
        if (!pSrcInfo->bMemFmt422)
        {
            u16FRC_RFetchAlignment = (u16FRCM_RFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
            //HW issue for OPM fetch when OP write enable, OPM Fetch have to offset pixels alignment avoid green garbage .
            if(pSrcInfo->u16H_SizeAfterPreScaling >= (u16FRC_RFetchAlignment+gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
            {
                u16FRCM_RFetch = u16FRC_RFetchAlignment;
            }
            else
            {
                //Overflow, set the OPM Fetch to the max data area .
                u16FRCM_RFetch = pSrcInfo->u16H_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.x + u16LBOffset;
                u16FRCM_RFetch = u16FRCM_RFetch  & ~1;
            }
        }
        else
        {
            if(2200>=u16FRCM_RFetch)   //Nike 4k2k opm fetch can't do ALIGNMENT
            {
#ifdef OPMFETCH_PIXEL_ALIGNMENT
                u16FRCM_RFetch = (u16FRCM_RFetch + (OPMFETCH_PIXEL_ALIGNMENT - 1)) & ~(OPMFETCH_PIXEL_ALIGNMENT - 1);
#else
                u16FRCM_RFetch = (u16FRCM_RFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
#endif
            }
        }

#ifndef DISABLE_3D_FUNCTION
        if(MDrv_SC_3D_Is2Dto3DCase(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance)))
        {
            MS_U16 u16HShift = MDrv_XC_Get_3D_HShift(pInstance);
            if ((u16FRCM_RFetch + u16HShift) > gSrcInfo[eWindow].u16IPMFetch)
            {
                if(gSrcInfo[eWindow].u16IPMFetch > u16HShift)
                {
                    //opmfetch need align although ipmfetch may not need align
                    u16FRCM_RFetch = ( gSrcInfo[eWindow].u16IPMFetch  - u16HShift + 1 ) & ~1;
                }
                else
                {
                    u16FRCM_RFetch = 0;
                }
            }
        }
        else
#endif
        {
            if(u16FRCM_RFetch > pSrcInfo->u16H_SizeAfterPreScaling  - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"!!!Alert u16FRCM_RFetch(%u) Overflowed, check crop setting!!!\n", u16FRCM_RFetch);
        }
    }

#ifdef K3_U2
    // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
    if(eWindow == MAIN_WINDOW)
    {
        if (u16FRCM_RFetch > MST_LINE_BFF_MAX)
        {
            u16FRCM_RFetch = MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign OPM fetch as %lu due to HW limitation \n", MST_LINE_BFF_MAX);
        }
    }
    else
    {
        if(u16FRCM_RFetch > SUB_MST_LINE_BFF_MAX)
        {
            u16FRCM_RFetch = SUB_MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign OPM fetch as %lu due to HW limitation \n", SUB_MST_LINE_BFF_MAX);
        }
    }
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f1. u16FRCM_RFetch after alignment %d, lboffset %d \n", u16FRCM_RFetch, u16LBOffset);
    //printf("1.SC Crop width=0x%x, LBOffset=0x%x\n", gSrcInfo[eWindow].ScaledCropWin.width, u16LBOffset);
    u16DispOffset = 0;

    //printf("cropy=%u,cropx=%u,dnroffset=%u,bp2p=%bu\n", _stScaledCropWin.y, _stScaledCropWin.x, u16DNROffset, u8BytesPer2Pixel);
    //printf("2.OPM Crop Offset=0x%lx, LBOffset=0x%x\n", u32OPMCropOffset, u16LBOffset);

    // adjsut OPMBase address for cropping
    if(pSrcInfo ->bDisplayNineLattice)
    {
    // for linear mode set all base address the same
        u32FRCM_RBase0 += u32FRCM_RCropOffset;
        //u32DNRBase1 += u32OPMCropOffset;
        //u32OPMBase2 += u32OPMCropOffset;
        u32FRCM_RBase2 = u32FRCM_RBase1 = u32FRCM_RBase0;
        //printf("--u32DNRBase0 %lx\n",u32DNRBase0);
    }
    else
    {
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"h.=> u32FRCM_RBase0,1=%x,%x -> +u32FRCM_RCropOffset[%x] =",(int)u32FRCM_RBase0,(int)u32FRCM_RBase1,(int)u32FRCM_RCropOffset);
        u32FRCM_RBase0 += u32FRCM_RCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"0:%x \n",(int)u32FRCM_RBase0);
        u32FRCM_RBase1 += u32FRCM_RCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"1:%x \n",(int)u32FRCM_RBase1);
        u32FRCM_RBase2 += u32FRCM_RCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"2:%x \n",(int)u32FRCM_RBase2);
    }

#if SUPPORT_2_FRAME_MIRROR

    MS_S32 s32cropshift_base0;
    MS_S32 s32cropshift_base1;
    MS_U32 u32LBOffset_mirror;

    s32cropshift_base0 = gSrcInfo[eWindow].ScaledCropWin.y * (MS_S32) (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset) + gSrcInfo[eWindow].ScaledCropWin.x;
    s32cropshift_base1 = - gSrcInfo[eWindow].ScaledCropWin.y * (MS_S32) (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_WOffset) + gSrcInfo[eWindow].ScaledCropWin.x;

    u32LBOffset_mirror = gSrcInfo[eWindow].ScaledCropWin.x % OFFSET_PIXEL_ALIGNMENT;

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32LBOffset:%d  \n",u32LBOffset_mirror);

    s32cropshift_base0 = ( s32cropshift_base0 - u32LBOffset_mirror ) * (MS_U32) MDrv_XC_TransFrameNumToFactor(gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    s32cropshift_base1 = ( s32cropshift_base1 - u32LBOffset_mirror ) * (MS_U32) MDrv_XC_TransFrameNumToFactor(gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    s32cropshift_base0 = ( s32cropshift_base0 * (MS_U32)(pSrcInfo->u8BitPerPixel/8) ) / (MS_U32)(BYTE_PER_WORD);
    s32cropshift_base1 = ( s32cropshift_base1 * (MS_U32)(pSrcInfo->u8BitPerPixel/8) ) / (MS_U32)(BYTE_PER_WORD);

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] s32cropshift_base0:%d  s32cropshift_base1:%d \n", s32cropshift_base0,s32cropshift_base1);

    if (gSrcInfo[eWindow].Status2.eFRCMFrameStoreNumber == IMAGE_STORE_2_FRAMES)
    {
       u32FRCM_RBase0 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase0 + s32cropshift_base0;
       u32FRCM_RBase1 = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_WBase1 + s32cropshift_base1;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32FRCM_RBase0:%x  u32FRCM_RBase1:%x \n", (int)u32FRCM_RBase0,(int)u32FRCM_RBase1);

#endif

    //-----------------------------------------------------------------
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset  = u16LBOffset;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DispOffset  = u16DispOffset;


    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0  = u32FRCM_RBase0;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1  = u32FRCM_RBase1;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2  = u32FRCM_RBase2;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch  = u16FRCM_RFetch;
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16AlignWidth = u16AlignWidth;
    //-----------------------------------------------------------------
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final FRCM_R Fetch=0x%x, u32FRCM_RBase0/1/2=0x%lx, 0x%lx, 0x%lx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final FRCM_R Fetch=0x%x, u32FRCM_RBase0/1/2=0x%tx, 0x%tx, 0x%tx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16FRCM_RFetch,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase0,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase1,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32FRCM_RBase2);
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"---------------------------------------------------\n");
#endif
}

/******************************************************************************/
///This function will calculate:
// [DNR]
// DNROffset, DNRFetch, DNRBase1, u32WritelimitBase
// [OPM]
// OPMFetch, OPMBase0, OPMBase1, LBOffset
// [OP1]
// u16VLen
/******************************************************************************/
void MDrv_SC_set_frcm_fetch_number_limit(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    MS_PHY u32OneLineOffset = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MDrv_SC_set_ipmsetting(pInstance, pSrcInfo, eWindow);
#if (HW_DESIGN_4K2K_VER != 4)
    MDrv_SC_set_dnrsetting(pInstance, pSrcInfo, TRUE, &u32OneLineOffset, TRUE, eWindow);
    MDrv_SC_set_opmsetting(pInstance, pSrcInfo, TRUE, TRUE, u32OneLineOffset, eWindow);
#else
    if (eWindow == MAIN_WINDOW)
    {
        MDrv_SC_SwitchFRCMWriteBank(pInstance, FALSE, E_XC_IPM_CONNETC_MAIN_OPM);
    }
    else
    {
        MDrv_SC_SwitchFRCMWriteBank(pInstance, FALSE, E_XC_IPM_CONNETC_SUB_OPM);
    }
    if (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
         ||(psXCInstPri->u32DeviceID == 1))
    {
        MDrv_SC_set_dnrsetting(pInstance, pSrcInfo, TRUE, &u32OneLineOffset, TRUE, eWindow);
        MDrv_SC_set_opmsetting(pInstance, pSrcInfo, TRUE, TRUE, u32OneLineOffset, eWindow);
    }
    else if (IS_INPUT_4K2K(eWindow)||IS_INPUT_4K1K(eWindow)
            ||((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_SAVE_MEM_MODE)&&(gSrcInfo[eWindow].stCapWin.width > 1900)&&(gSrcInfo[eWindow].stCapWin.height>2100)&&(gSrcInfo[eWindow].stCapWin.height<2200)))
    {
        if ((eWindow == MAIN_WINDOW)&&(MDrv_ReadByte(REG_CHIP_REVISION) < 2)&&(MDrv_XC_GetDynamicScalingStatus(pInstance)||MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) ))//Monaco U03 fix .
        {
            MDrv_SC_SwitchFRCMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_MAIN_FRCM);
        }

        if (gSrcInfo[eWindow].enPreStreamFlow != E_XC_DIRECT_FRCM)
        {
            MDrv_SC_set_mirrorEx(pInstance,  gSrcInfo[eWindow].Status2.eMirrorMode, eWindow);
            gSrcInfo[eWindow].enPreStreamFlow = E_XC_DIRECT_FRCM;
        }
        MDrv_SC_set_dnrsetting(pInstance, pSrcInfo, FALSE, &u32OneLineOffset, TRUE, eWindow);
        MDrv_SC_set_frcmrsetting(pInstance, pSrcInfo, u32OneLineOffset, eWindow);
    }
    // for DS VB size 4k2k p mode-> bypass mode
    else if(((gSrcInfo[eWindow].Status2.u16VirtualBox_Width >= 0xF00) && (gSrcInfo[eWindow].Status2.u16VirtualBox_Height>= 0x870))
        && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )&& (!gSrcInfo[eWindow].bInterlace))
    {
        if ( (eWindow == MAIN_WINDOW) && (MDrv_ReadByte(REG_CHIP_REVISION) < 2 ))//Monaco U03 fix .
        {
            MDrv_SC_SwitchFRCMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_MAIN_FRCM);
        }
        MDrv_SC_set_dnrsetting(pInstance, pSrcInfo, FALSE, &u32OneLineOffset, TRUE, eWindow);
        MDrv_SC_set_frcmrsetting(pInstance, pSrcInfo, u32OneLineOffset, eWindow);
    }
    else//DS i mode -> monaco mode
    {
        if (eWindow == MAIN_WINDOW && (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) ) && (MDrv_ReadByte(REG_CHIP_REVISION) < 2 ))//Monaco U03 fix .
        {
            MDrv_SC_SwitchFRCMWriteBank(pInstance, TRUE, E_XC_IPM_CONNETC_MAIN_FRCM);
        }
        if (gSrcInfo[eWindow].enPreStreamFlow != E_XC_SCM_TO_FRCM)
        {
            MDrv_SC_set_mirrorEx(pInstance,  gSrcInfo[eWindow].Status2.eMirrorMode, eWindow);
            gSrcInfo[eWindow].enPreStreamFlow = E_XC_SCM_TO_FRCM;
        }
        MDrv_SC_set_dnrsetting(pInstance, pSrcInfo, TRUE, &u32OneLineOffset, FALSE, eWindow);
        MDrv_SC_set_opmsetting(pInstance, pSrcInfo, FALSE, FALSE, u32OneLineOffset, eWindow);
        MDrv_SC_set_frcmwsetting(pInstance, pSrcInfo, &u32OneLineOffset, eWindow);
        MDrv_SC_set_frcmrsetting(pInstance, pSrcInfo, u32OneLineOffset, eWindow);
    }
#endif

#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_FetchOffset(pInstance, pSrcInfo, eWindow);
#endif

#ifdef ENABLE_SCALING_WO_MUTE
    MDrv_SC_Adjust_Skip_OPM_line(pInstance, eWindow);
#endif
}

/******************************************************************************/
///This function will calculate:
// [DNR]
// DNROffset, DNRFetch, DNRBase1, u32WritelimitBase
// [OPM]
// OPMFetch, OPMBase0, OPMBase1, LBOffset
// [OP1]
// u16VLen
// bBwdEnable: Weather caculator fetch/offset for bwd
// bIsLeftFactor: Under miu dual mode
//                TRUE call for Left side calc, FALSE call for right side calc
/******************************************************************************/
static void _MDrv_SC_set_fetch_number_limit_impl(void *pInstance, XC_InternalStatus *pSrcInfo, MS_U8 u8BitPerPixel, MS_BOOL bBwdEnable, MS_BOOL bIsLeftFactor, SCALER_WIN eWindow)
{
    MS_U16 u16V_Writelimit, u16LBOffset, u16DispOffset, u16OPMFetch, u16OPMFetchAlignment;
    MS_PHY u32Offset, u32DNRBase0, u32DNRBase1, u32DNRBase2, u32WritelimitBase=0;
    MS_PHY u32OPMBase0, u32OPMBase1, u32OPMBase2, u32OPMCropOffset =0, u32OPMPixelOffset;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U16 u16AlignWidth = 0;
    MS_PHY u32BaseOffset = 0;
    MS_PHY u32Framesize;
    MS_U8 u8BytesPer2Pixel = u8BitPerPixel * 2 / 8;
    MS_U16 u16DNROffset = 0;
    MS_PHY u32OneLineOffset = 0;
    MS_U8 u8FrameNum = 0;
    MS_U16 u16H_SizeAfterPreScaling = pSrcInfo->u16H_SizeAfterPreScaling;
    MS_U16 u16DNROffsetFor3D = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
//TODO: buffer addr for sub win
//printf("MDrv_SC_set_fetch_number_limit  %d \n",u8BytesPer2Pixel );
    /////////////////////////////////////////////////
    // Set DNR Memeory Settings                    //
    /////////////////////////////////////////////////
    // Cal DNR Fetch/Offset
    MS_U16 *pu16IPMFetch = NULL;
    MS_PHY u32MsIPMBase0 = 0;
    MS_PHY u32DNRBufSize[MAX_WINDOW];
    memset(&u32DNRBufSize, 0, sizeof(u32DNRBufSize));
#ifdef SUPPORT_BWD
    E_XC_BWR_MEM_CFG_BPP_TYPE eBPPType = E_XC_BWR_MEM_CFG_BPP_24;
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    MS_U32 u32LeftFactor = 0;
    MS_U32 u32RightFactor = 0;

    if (IsEnableDualMode(eWindow))
    {
#if (SUPPORT_DUAL_MIU_MIRROR_SWAP_IPM == TRUE)
        // Miu dual mode  just Mirror Case need swap IPM. And disable dual must not swap IPM.
        // So, add IPM write limit in this function.
        if (IsHMirrorMode(eWindow))
        {
            if(bIsLeftFactor == TRUE)
            {
                u32MsIPMBase0 = MS_IPM_DUAL_MIU_BASE0(eWindow);
                Hal_SC_AdjustIpmWriteLimite(pInstance, TRUE, TRUE, eWindow);
            }
            else
            {
                u32MsIPMBase0 = MS_IPM_BASE0(eWindow);
                Hal_SC_AdjustIpmWriteLimite(pInstance, FALSE, TRUE, eWindow);
            }
        }
        else
        {
            if(bIsLeftFactor == TRUE)
            {
                u32MsIPMBase0 = MS_IPM_BASE0(eWindow);
                Hal_SC_AdjustIpmWriteLimite(pInstance, TRUE, FALSE, eWindow);
            }
            else
            {
                u32MsIPMBase0 = MS_IPM_DUAL_MIU_BASE0(eWindow);
                Hal_SC_AdjustIpmWriteLimite(pInstance, FALSE, FALSE, eWindow);
            }
        }
#else
        if(bIsLeftFactor == FALSE)
        {
            u32MsIPMBase0 = MS_IPM_DUAL_MIU_BASE0(eWindow);
        }
        else
        {
            u32MsIPMBase0 = MS_IPM_BASE0(eWindow);
        }
#endif
    }
    else
#endif
    {
        u32MsIPMBase0 = MS_IPM_BASE0(eWindow);
#ifdef UFO_XC_SUPPORT_DUAL_MIU // Monet & Manhattan
#if (SUPPORT_DUAL_MIU_MIRROR_SWAP_IPM == TRUE) // Just Manhattan
        Hal_SC_AdjustIpmWriteLimite(pInstance, TRUE, FALSE, eWindow);
#endif
#endif
    }

    //To avoid pu16IPMFetch do not entry any condition and Dereferencing null pointer
    //We set a init point here
    pu16IPMFetch = &gSrcInfo[eWindow].u16IPMFetch;
    if (bIsLeftFactor == TRUE)
    {
        u32DNRBufSize[eWindow] = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow];
        pu16IPMFetch = &gSrcInfo[eWindow].u16IPMFetch;
    }
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    else //if (IsEnableDualMode(eWindow))
    {
        u32DNRBufSize[eWindow] = pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow];
        pu16IPMFetch = &gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch;
    }
#endif
    *pu16IPMFetch = pSrcInfo->u16H_SizeAfterPreScaling;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"===================================================\n");
    if (bIsLeftFactor == TRUE)
    {
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u16DNRFetch=%d ->",(int)*pu16IPMFetch);
    }
    else
    {
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u16DualMiuDNRFetch=%d ->",(int)*pu16IPMFetch);
    }
//#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
    if ((pSrcInfo->bMemFmt422)
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        && (!IsEnableDualMode(eWindow))
#endif
        )
        *pu16IPMFetch = (*pu16IPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
//#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC," %d \n",(int)*pu16IPMFetch);
    MS_U16 *pu16IPMFetchTmp = NULL;
    if(eWindow == MAIN_WINDOW)
    {
       pu16IPMFetchTmp = &gSrcInfo[MAIN_WINDOW].u16IPMFetch;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (bIsLeftFactor == FALSE)
        {
            pu16IPMFetchTmp = &gSrcInfo[MAIN_WINDOW].Status2.u16DualMiuIPMFetch;
        }
#endif
        if (*pu16IPMFetchTmp > MST_LINE_BFF_MAX)
        {
            *pu16IPMFetchTmp = MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign IPM fetch as %lu due to HW limitation \n", MST_LINE_BFF_MAX);
        }
    }
    else
    {
       pu16IPMFetchTmp = &gSrcInfo[SUB_WINDOW].u16IPMFetch;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (bIsLeftFactor == FALSE)
        {
            pu16IPMFetchTmp = &gSrcInfo[SUB_WINDOW].Status2.u16DualMiuIPMFetch;
        }
#endif
#if (HW_DESIGN_3D_VER < 2)
#ifndef DISABLE_3D_FUNCTION
        if(MDrv_SC_3D_IsIPMFetchNoLimitCase(pInstance, eWindow))
        {
            //no any limits
        }
        else if((E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance))
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Output_Mode(pInstance))
                || (E_XC_3D_OUTPUT_TOP_BOTTOM_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
                || (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF_HW == MDrv_XC_Get_3D_Output_Mode(pInstance))
               )
        {
            if(*pu16IPMFetchTmp > MS_3D_LINE_BFF_MAX)
            {
                *pu16IPMFetchTmp = MS_3D_LINE_BFF_MAX;
            }
        }
        else
#endif
        if (*pu16IPMFetchTmp > SUB_MST_LINE_BFF_MAX)
#else
#ifndef DISABLE_3D_FUNCTION
        if((E_XC_3D_PANEL_NONE == MDrv_XC_Get_3D_Panel_Type(pInstance))
          &&(*pu16IPMFetchTmp > SUB_MST_LINE_BFF_MAX))
#endif
        {
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
            //Normal PIP case
            *pu16IPMFetchTmp = SUB_MST_LINE_BFF_MAX;
#endif
        }
#endif
    }
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    // Double check Dual enable
    if (IsEnableDualMode(eWindow))
    {
        u32LeftFactor = pSrcInfo->Status2.stMiuDualModCfg.u32LeftFactor;
        u32RightFactor = pSrcInfo->Status2.stMiuDualModCfg.u32RightFactor;
        MS_U16 u16IPMFetchTotal = *pu16IPMFetch;

        if (IsHMirrorMode(eWindow))
        {
            *pu16IPMFetch = u16IPMFetchTotal*u32RightFactor /(u32RightFactor + u32LeftFactor);
            *pu16IPMFetch = (*pu16IPMFetch + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
            gSrcInfo[eWindow].u16IPMFetch = u16IPMFetchTotal - *pu16IPMFetch;
            gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch = *pu16IPMFetch;
        }
        else
        {
            // No mirror, Calc Left Side firstly.
            *pu16IPMFetch = u16IPMFetchTotal*u32LeftFactor /(u32RightFactor + u32LeftFactor);
            *pu16IPMFetch = (*pu16IPMFetch + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
            gSrcInfo[eWindow].u16IPMFetch = *pu16IPMFetch;
            gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch = u16IPMFetchTotal  - *pu16IPMFetch;
        }
    }
#endif

    //printf("==>NineLattice = %x, bMirror = %x\n", pSrcInfo->bDisplayNineLattice,IsVMirrorMode(eWindow));

    if(pSrcInfo->bDisplayNineLattice)
    {
        u16DNROffset = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment
    }
    else
    {
        if((MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow)) &&
            (!IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)))
        {
            u16DNROffset = (gSrcInfo[eWindow].Status2.u16VirtualBox_Width + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
#ifdef XC_DUAL_MIU_SUPPORT_DS
            // EnableDS: u16DNROffset equals u16VirtualBox_Width, so for dual miu need devided dnr_offset for left and right view.
            if (IsEnableDualMode(eWindow))
            {
                MS_U16 u16OffsetTotal = u16DNROffset;
                // Mirror, Calc Left Side firstly.
                if (IsHMirrorMode(eWindow))
                {
                    u16DNROffset = u16DNROffset*u32RightFactor /(u32RightFactor + u32LeftFactor);
                    u16DNROffset = (u16DNROffset + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
                    if (bIsLeftFactor == TRUE)
                    {
                        u16DNROffset = u16OffsetTotal - u16DNROffset;
                    }
                }
                else // No mirror, Calc Left Side firstly.
                {
                    u16DNROffset = u16DNROffset*u32LeftFactor /(u32RightFactor + u32LeftFactor);
                    u16DNROffset = (u16DNROffset + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
                    if (bIsLeftFactor == FALSE)
                    {
                        u16DNROffset = u16OffsetTotal - u16DNROffset;
                    }
                }
            }
#endif
#endif

        }
        else
        {
            u16DNROffset = (*pu16IPMFetch + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment
        }
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u16DNROffset=%u\n",u16DNROffset);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC," %d\n",(int)*pu16IPMFetch);
#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
    if (IsHMirrorMode(eWindow))
    {
        u16AlignWidth = u16DNROffset - *pu16IPMFetch;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"align width = %d\n", u16AlignWidth);

        // the hardware alignment in OPM > IPM case, it will get more data for alignment
        // disable it in the nine lattice mode
        if(pSrcInfo->bDisplayNineLattice)
        {
            Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
        }
        else
        {
            //these chips: they need sw to decide prealign or not.
            //here set to TRUE, then hw will prealign u16AlignWidth
            //if here is FALSE, hw won't do prealign

            //to fix H/W bugs, OPM need to be aligned.
            //so do not use pre-align. use OPM line buffer offset instead.
            Hal_SC_set_pre_align_pixel(pInstance, FALSE, 0, eWindow);
        }
    }
#else
    if (IsHMirrorMode(eWindow))
    {
        //M10, J2, A2 and later chip, HW will auto insert pixel to alignment u16IPMFetch
        //below u16AlignWidth is used for CBCR swap control
#ifdef K3_U2
        if (pSrcInfo->bMemFmt422)
            u16AlignWidth = u16DNROffset - pSrcInfo->u16H_SizeAfterPreScaling;
        else
            u16AlignWidth = u16DNROffset - *pu16IPMFetch;;
#endif
        // the hardware alignment in OPM > IPM case, it will get more data for alignment
        // disable it in the nine lattice mode
        if(pSrcInfo->bDisplayNineLattice)
        {
            Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
        }
        else
        {
            //these chips: when sw set it to TRUE, then hw will do the prealign.
            //because hw can calculate by itself, even for the alignwidth==0 case,
            //so sw don't need to worry about it, just set it to TRUE.
            Hal_SC_set_pre_align_pixel(pInstance, TRUE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at start
        }
    }
#endif
    else
    {
        u16AlignWidth = 0;
        Hal_SC_set_pre_align_pixel(pInstance, FALSE, u16AlignWidth, eWindow);//M10/J2: insert alignment pixel at end
    }

#ifndef DISABLE_3D_FUNCTION
        if((E_XC_3D_INPUT_SIDE_BY_SIDE_FULL == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_CHECK_BORAD == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
           || (E_XC_3D_INPUT_PIXEL_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)))
        {
            u16AlignWidth /= 2;            //opm fetch actual align size
            u16H_SizeAfterPreScaling /= 2; //actual prescaling hsize
        }
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"align width = %d\n", u16AlignWidth);
    // alignwidth must be even, other color will be wrong and need to
    // use cb cr swap to correct it.
    if((u16AlignWidth & 0x01) && (HAL_SC_get_memory_bit_fmt(pInstance, eWindow) == 0))
    {
        Hal_SC_set_422_cbcr_swap(pInstance, ENABLE, eWindow);
    }
    else
    {
        Hal_SC_set_422_cbcr_swap(pInstance, DISABLE, eWindow);
    }

    if(pSrcInfo->bDisplayNineLattice)
    {
        pSrcInfo->u16V_Length = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }

    u16DNROffsetFor3D = u16DNROffset;
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_DNROffset(pInstance, &u16DNROffsetFor3D,&u16DNROffset, eWindow);
#endif

    u32Framesize = (MS_U32)pSrcInfo->u16V_SizeAfterPreScaling * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;

#ifndef DISABLE_3D_FUNCTION
#if (HW_DESIGN_3D_VER >= 2)
    if((E_XC_3D_INPUT_FRAME_PACKING == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
            && ((pSrcInfo->stCapWin.height == DOUBLEHD_1080X2I_VSIZE)
#if ((HW_DESIGN_3D_VER == 3) && (HW_DESIGN_4K2K_VER == 7))
                || (pSrcInfo->stCapWin.height == DOUBLEHD_576X2I_VSIZE)
                || (pSrcInfo->stCapWin.height == DOUBLEHD_480X2I_VSIZE)
#endif
                ))
    {
        //special case, hw will use field size 540 as deinterlace size
        u32Framesize /= 2;
    }
#if (HW_DESIGN_3D_VER > 2)
    else if(E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
    {//per designer, input 3d:framealternative will double data at dnr
        u32Framesize *= 2;
    }
#endif
#endif
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>Framesize(%d)= VSizeAfterPreSD(%d) * DNROffset(%d) * BytesPer2Pixel(%d)/2\n",(int)u32Framesize,pSrcInfo->u16V_SizeAfterPreScaling,u16DNROffset,u8BytesPer2Pixel);

#ifdef UFO_XC_FB_LEVEL
    if((pSrcInfo->eFBLevel == E_XC_FB_LEVEL_FB)
        && (u32Framesize * 2 > u32DNRBufSize[eWindow])
    )
#else
    if((!pSrcInfo->bFBL)
        && (!pSrcInfo->bR_FBL)
        && (u32Framesize * 2 > u32DNRBufSize[eWindow]))
#endif
    {
        //this case should always not be happened
        printf("Attention! Scaler DNR Buffer is not enough for this timing, eWindow=%u!\n", eWindow);
    }

    // enable 4 frame mode if input is progressive & frame buffer is enough
    // , regardless the input source and timing because 4 frame mode is better in avoid tearing
    //
    if(pSrcInfo->bDisplayNineLattice)
    {
#if (_LINEAR_ADDRESS_MODE_SUPPORTED)
        gSrcInfo[eWindow].bLinearMode = TRUE;
        Hal_SC_set_linearmem_mode(ENABLE,MAIN_WINDOW);
#endif
    }
    else
    {
        XC_FRAME_STORE_NUMBER enLastFBNum = gSrcInfo[eWindow].Status2.eFrameStoreNumber;

        /*Update Frame buffer usage status and memory linear mode*/
        // MDrv_XC_GetPQSuggestedFrameNum(eWindow) is the frame count that PQ want to set
        // gSrcInfo[eWindow].Status2.eFrameStoreNumber is the frame count that sw code(_Mdrv_SC_CalcFBNum)
        //     decides according to frame size and other condition
        _Mdrv_XC_Refine_FrameNum(pInstance, &gSrcInfo[eWindow].bLinearMode,
                           &gSrcInfo[eWindow].Status2.eFrameStoreNumber,
                           bInterlace,
                           u32Framesize,
                           u32DNRBufSize[eWindow],
                           eWindow);

#ifdef ENABLE_RFBL_Y8M4_SUPPORTED
    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, BIT(3)))
    {
        gSrcInfo[eWindow].Status2.eFrameStoreNumber = IMAGE_STORE_2_FIELDS;
    }
#endif

        if (!IsVMirrorMode(eWindow))
        {
            if (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES)
            {
                // 3/4 value is suggested by designer, it is a experimental value.
                MS_U16 u16SwitchCnt = gSrcInfo[eWindow].stCapWin.height * 3 / 4;
                MDrv_SC_EnableReadBankSwitchControl(pInstance, ENABLE, u16SwitchCnt, eWindow);
            }
            else
            {
                MDrv_SC_EnableReadBankSwitchControl(pInstance, DISABLE, 0, eWindow);
            }
        }
        else
        {
            MDrv_SC_EnableReadBankSwitchControl(pInstance, DISABLE, 0, eWindow);
        }

        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"gSrcInfo[eWindow].bLinearMode %d \n",gSrcInfo[eWindow].bLinearMode);
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"FrameMode %d \n",gSrcInfo[eWindow].Status2.eFrameStoreNumber);

        // Set memory frame buffer setting.
        _MDrv_XC_Set_FrameNum(pInstance, eWindow, FALSE, gSrcInfo[eWindow].Status2.eFrameStoreNumber,bInterlace);

        // to avoid mm play photo garbage, because of big changing FB number
        if((enLastFBNum != gSrcInfo[eWindow].Status2.eFrameStoreNumber) && (TRUE == MDrv_XC_Get_SCMI_Type()))
        {
            MDrv_XC_WBankReset(pInstance, FALSE, eWindow);
        }

#if (_LINEAR_ADDRESS_MODE_SUPPORTED)
        // Set linear mode
        if (gSrcInfo[eWindow].bLinearMode)
        {
            Hal_SC_set_linearmem_mode(ENABLE,eWindow);
        }
        else
        {
            Hal_SC_set_linearmem_mode(DISABLE,eWindow);
        }
#endif
        // if support _FIELD_PACKING_MODE_SUPPORTED, it will control by QMAP(J2/A5/A6/A3)
#if(!_FIELD_PACKING_MODE_SUPPORTED)
        // avoid tearing problem when we can not do frame lock
        if(pSrcInfo->bRWBankAuto)
        {
#if (HW_DESIGN_3D_VER == 1)
            //this 3d method need special wr bank mapping
            if(((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                  && (E_XC_3D_OUTPUT_TOP_BOTTOM == MDrv_XC_Get_3D_Output_Mode(pInstance)))
                || ((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow))
                   && (E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF == MDrv_XC_Get_3D_Output_Mode(pInstance)))
                )
            {
                if(eWindow == MAIN_WINDOW)
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x01, eWindow);
                }
                else
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
                }
            }
            else
#endif
            // Set read/write bank as larger than 0x01 if frame count >= 3
            if (( gSrcInfo[eWindow].Status2.eFrameStoreNumber >= IMAGE_STORE_3_FRAMES)&&
                       gSrcInfo[eWindow].Status2.eFrameStoreNumber <= IMAGE_STORE_10_FRAMES)
            {
                if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L, BIT(4)) != 0)
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
                else
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x01, eWindow);
            }
            else // Interlace
            {
                //For 25_4R_MC mode only
                if((!MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow)) && (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_4_FIELDS)
                    && (!MDrv_SC_IsForceFreerun(pInstance)))
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x02, eWindow);
                }
                else
                {
                    Hal_SC_set_wr_bank_mapping(pInstance, 0x00, eWindow);
                }
            }
        }
#endif
    }

    //u32Offset = (MS_U32)pSrcInfo->u16V_Length * (MS_U32)u16DNROffset * (MS_U32)u8BytesPer2Pixel/2;
    //fix u32DNRBase1 at SCALER_DNR_BUF_LEN/2 for not using blue-screen at channel change in ATV

    if( gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES )
        u32Offset = u32DNRBufSize[eWindow]/3;
    else
        u32Offset = u32DNRBufSize[eWindow]/2;
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32Offset :%lu, DNRBufSize=%lu\n", u32Offset, u32DNRBufSize[eWindow]);
#else
#endif

    u8FrameNum = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    u32DNRBase0 = u32MsIPMBase0;

#if SUPPORT_2_FRAME_MIRROR
    MS_U32 u32pitch = 0;

    if( (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES) && (!gSrcInfo[eWindow].bInterlace) )
    {
        u32pitch = ( ( (MS_U32)(u8BitPerPixel/8) * (MS_U32)*pu16IPMFetch * (MS_U32)u8FrameNum * (MS_U32)(pSrcInfo->u16V_SizeAfterPreScaling - 1) ) / (MS_U32)BYTE_PER_WORD);//Pitch = Byte_per_pix*align(fetch)*2*(height-1)/byte_per_miu_word
        u32DNRBase1 = u32DNRBase0 + u32pitch;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32pitch:%x u32DNRBase1:%x \n",(int)u32pitch,(int)u32DNRBase1);
#else
    u32DNRBase1 = u32MsIPMBase0 + (( u32Offset/BYTE_PER_WORD + 0x01) & ~0x01L);

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>MAIN DNRBase1[%x]= MS_IPM_BASE0[%u]=(%x) + ((u32Offset[%x]/%d + 0x01) & ~0x01L)\n",(int)u32DNRBase1, eWindow,(int)u32MsIPMBase0,(int)u32Offset, BYTE_PER_WORD);
#endif

#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_DNRBase(pInstance,
                              &u32DNRBase0,
                              &u32DNRBase1,
                              eWindow);
#endif

    if( gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES  )
        u32DNRBase2 = u32DNRBase1 + (( u32Offset/BYTE_PER_WORD + 0x01) & ~0x01L);
    else
        u32DNRBase2 = u32DNRBase0;

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>DNRBase2[%x])\n", (int)u32DNRBase2);

    if(pSrcInfo->bDisplayNineLattice)
    {
        u32OPMBase1 = u32OPMBase0 = u32OPMBase2 = u32DNRBase0;
    }
    else
    {
        u32OPMBase0 = u32DNRBase0;
        u32OPMBase1 = u32DNRBase1;
        u32OPMBase2 = u32DNRBase2;
#ifndef DISABLE_3D_FUNCTION
        MDrv_SC_3D_Adjust_OPMBase(pInstance,
                                  &u32OPMBase0,
                                  &u32OPMBase1,
                                  &u32OPMBase2,
                                  u32Offset,
                                  eWindow);
#endif
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>u32OPMBase0[%x]= MS_IPM_BASE0[%u][%x] => ",(int)u32OPMBase0, eWindow, (int)u32MsIPMBase0);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32OPMBase1[%x]= u32IPMBase1[%x] \n",(int)u32OPMBase1,(int)u32DNRBase1);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"u32OPMBase2[%x] \n",(int)u32OPMBase2);

    // Cal DNR V write limit
    if(pSrcInfo->bDisplayNineLattice)
    {
        u16V_Writelimit = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }
    else
    {
        u16V_Writelimit = pSrcInfo->stCapWin.height;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u16V_Writelimit[%d]= u16V_CapSize=%d \n",u16V_Writelimit,pSrcInfo->stCapWin.height);
    if( bInterlace )
        u16V_Writelimit = (u16V_Writelimit+1) / 2;
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> i=%d, u16V_Writelimit[%d]= (u16V_Writelimit+1) / 2\n",bInterlace,u16V_Writelimit);
    // Cal DNR write limit
  #if 1//( CHIP_FAMILY_TYPE == CHIP_FAMILY_S4LE )

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"preview =>WritelimitBase[%u][%x] = (MS_IPM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD)\n",eWindow,(int)u32WritelimitBase, eWindow, (int)u32MsIPMBase0 ,(int)u32Offset);

    if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES)
    {
        u32WritelimitBase = (u32MsIPMBase0) + (((u32Offset + 0xF) & ~0xFL) * 3 / BYTE_PER_WORD);
    }
    else
    {
        u32WritelimitBase = (u32MsIPMBase0) + (((u32Offset + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD);
    }
     XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>WritelimitBase[%x] = (MS_IPM_BASE0[%u]=%x) + (((u32Offset[%x] + 0xF) & ~0xFL) * 2 / BYTE_PER_WORD)\n",(int)u32WritelimitBase,eWindow,(int)u32MsIPMBase0,(int)u32Offset);
     u32WritelimitBase = (eWindow == MAIN_WINDOW) ? (u32WritelimitBase - 1) | F2_WRITE_LIMIT_EN : (u32WritelimitBase - 1) | F1_WRITE_LIMIT_EN;
     XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=> u32WritelimitBase[%x] = (u32WritelimitBase - 1) | 0x2000000\n",(int)u32WritelimitBase);
  #else
    u32WritelimitBase = MS_DNR_F2_BASE0 + ((u32Offset/8 + 0x01) & ~0x01L) * 2;
    u32WritelimitBase = (u32WritelimitBase - 1);
  #endif

    if (IsVMirrorMode(eWindow))
    {
        if(!pSrcInfo->bDisplayNineLattice)
        {
            MS_U16 u16V_SizeAfterPreScaling = pSrcInfo->u16V_SizeAfterPreScaling;
            MS_U16 u16DNROffsetTemp = u16DNROffset;
            MS_U16 u16Ratio = 1;
            MS_BOOL bMirrorBufferOverflow = FALSE;

            if(MDrv_XC_GetDynamicScalingStatus(pInstance)
#ifdef K3_U2
                && (eWindow == MAIN_WINDOW)
#if SUPPORT_SEAMLESS_ZAPPING
                && (pXCResourcePrivate->sthal_SC.bSeamlessZappingEnable[eWindow] == FALSE)
#endif
#endif
              )
            {
                //only for mm ds go into ficlk patch: gSrcInfo[eWindow].Status2.u16PreVCusScalingDst
                //is one(progressive) or two lines(interlace)  less than gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc
                u16V_SizeAfterPreScaling = gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc;
            }

#ifndef DISABLE_3D_FUNCTION
            MDrv_SC_3D_Adjust_MirrorDNROffset(pInstance, pSrcInfo, &u16V_SizeAfterPreScaling, &u16DNROffsetTemp, &u16Ratio, eWindow);
#endif
            //base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
            u32OneLineOffset = u8FrameNum * (MS_U32)u16Ratio * (MS_U32)u16DNROffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;

#if SUPPORT_SEAMLESS_ZAPPING
            if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
            if( bInterlace )
#endif
            {
                MS_U16 u16Offset;
                u16Offset = u16DNROffsetTemp * MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED) / 2;

                u32BaseOffset = (MS_U32)(u16V_SizeAfterPreScaling-2) * (MS_U32)u16Ratio * (MS_U32)u16Offset * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;
            }
            else
            {
                //Progressived mode, non-linear address
                //base_offset = (frame_line_cnt -2) * line_offset * (N-bits/pix)/64-bits
                u32BaseOffset = (MS_U32)((MS_U32)u16Ratio * u8FrameNum*(u16V_SizeAfterPreScaling-1)) * (MS_U32)u16DNROffsetTemp * (MS_U32)u8BytesPer2Pixel/2/BYTE_PER_WORD;
            }

            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"=>u16Ratio[%u] * u8FrameNum[%u] *(u16V_SizeAfterPreScaling-1)[%u]*u16DNROffsetTemp[%u]*u8BytesPer2Pixel[%u]\n",
                u16Ratio,u8FrameNum,(u16V_SizeAfterPreScaling-1),u16DNROffsetTemp,u8BytesPer2Pixel);


            MDrv_SC_set_DNRBaseOffset(pInstance, eWindow,u32BaseOffset);

            u32DNRBase0 += u32BaseOffset;

            //to avoid set dnr base1 out of scaler dnr memory when 2 frame mode
            if((u32DNRBase1 + u32BaseOffset) <= (u32MsIPMBase0 + u32DNRBufSize[eWindow]/BYTE_PER_WORD - u32OneLineOffset))
            {
                u32DNRBase1 += u32BaseOffset;
            }

            if((u32DNRBase2 + u32BaseOffset) <= (u32MsIPMBase0 + u32DNRBufSize[eWindow]/BYTE_PER_WORD - u32OneLineOffset))
            {
                u32DNRBase2 += u32BaseOffset;
            }

            if(MDrv_XC_Get_SCMI_Type())
            {
                // field packing mode, only dnrbase0 is used
                if((u32DNRBase0 < u32BaseOffset) || (u32DNRBase0 - u32BaseOffset < u32MsIPMBase0))
                {
                    bMirrorBufferOverflow = TRUE;
                }
            }
            else //not field packing mode
            {
                if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES)
                {
                    // 2 frame mode, only dnrbase0 is used
                    if((u32DNRBase0 < u32BaseOffset) || (u32DNRBase0 - u32BaseOffset < u32MsIPMBase0))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
                else if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_3_FRAMES)
                {
                    // 3 frame mode, dnrbase0, dnrbase1, dnrbase2 is used
                    if((u32DNRBase0 < u32BaseOffset)
                       || (u32DNRBase1 < u32BaseOffset)
                       || (u32DNRBase2 < u32BaseOffset)
                       || (u32DNRBase0 - u32BaseOffset < u32MsIPMBase0)
                       || (u32DNRBase1 - u32BaseOffset < u32MsIPMBase0)
                       || (u32DNRBase2 - u32BaseOffset < u32MsIPMBase0))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
                else
                {
                    //other frame mode, dnr base0, dnr base1 is used
                    if((u32DNRBase0 < u32BaseOffset)
                       || (u32DNRBase1 < u32BaseOffset)
                       || (u32DNRBase0 - u32BaseOffset < u32MsIPMBase0)
                       || (u32DNRBase1 - u32BaseOffset < u32MsIPMBase0))
                    {
                        bMirrorBufferOverflow = TRUE;
                    }
                }
            }

            if(bMirrorBufferOverflow)
            {
                printf("[%s,%5d] Attention! Scaler memory is overflow!\n",__FUNCTION__,__LINE__);
                MS_ASSERT(0);
            }
        }

        u32WritelimitBase = u32MsIPMBase0 - 1;

        if(eWindow == MAIN_WINDOW)
            u32WritelimitBase= u32WritelimitBase | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
        else
            u32WritelimitBase= u32WritelimitBase | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
    }

    //store FrameNum
    MDrv_XC_Save_FrameNumFactor(pInstance, eWindow,
               MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED));

    /////////////////////////////////////////////////
    // Set OPM Memeory Settings                    //
    /////////////////////////////////////////////////

    // Cal OPM Base address

    // compute address offset for cropping
    // cropping shift h unit: 16 pixel
    // v unit: 2 line(for interlace), 1 line(for progressive)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop info: (x,y)=(%d,%d),(width,height)=(%d,%d), interlaced = %d\n",
        gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y,
        gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height,
        bInterlace);

    if(pSrcInfo->bDisplayNineLattice)
    {
        if (IsVMirrorMode(eWindow))
        {
            if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_8_FIELDS)
            {
                u32OPMPixelOffset = (MS_U32)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            else if( bInterlace )
            {
                u32OPMPixelOffset = (MS_U32)((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))/2)*u16DNROffset;
            }
            else
            {
                u32OPMPixelOffset = (MS_U32)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height
                                    - (gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            u32OPMPixelOffset += (MS_U32)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width
                                - (gSrcInfo[eWindow].stDispWin.x - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart + gSrcInfo[eWindow].stDispWin.width));
        }
        else
        {
            if(gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_8_FIELDS)
            {
                u32OPMPixelOffset = (MS_U32)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            else if( bInterlace )
            {
                u32OPMPixelOffset = (MS_U32)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)/2)*u16DNROffset;
            }
            else
            {
                u32OPMPixelOffset = (MS_U32)((gSrcInfo[eWindow].stDispWin.y - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart))*u16DNROffset;
            }
            u32OPMPixelOffset += (MS_U32)(gSrcInfo[eWindow].stDispWin.x - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart);
        }
    }
    else
    {
#if SUPPORT_SEAMLESS_ZAPPING
        if( bInterlace || Hal_SC_IsPX2MemFormat(pInstance, eWindow))
#else
        if( bInterlace )
#endif
#ifdef SUPPORT_BWD
        {
            if (bBwdEnable == TRUE) // Designer request: CropWin.y must be even number when BWR enable
            {
                MS_U16 u16CropY = gSrcInfo[eWindow].ScaledCropWin.y;
                u16CropY &= ~0x1;
                u32OPMPixelOffset = (MS_PHY)(u16CropY/2) * u16DNROffsetFor3D;
            }
            else
            {
                u32OPMPixelOffset = (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.y/2) * u16DNROffsetFor3D;
            }
        }
#else
        {
            u32OPMPixelOffset = (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.y/2) * u16DNROffsetFor3D;
        }
#endif
        else
#ifdef SUPPORT_BWD
        {
            if (bBwdEnable == TRUE) // Designer request: CropWin.y must be even number when BWR enable
            {
                MS_U16 u16CropY = gSrcInfo[eWindow].ScaledCropWin.y;
                u16CropY &= ~0x1;
                u32OPMPixelOffset = (MS_PHY)((u16CropY) * u16DNROffsetFor3D);
            }
        else
        {
            u32OPMPixelOffset = (MS_PHY)((gSrcInfo[eWindow].ScaledCropWin.y) * u16DNROffsetFor3D);
        }
        }
#else
        {
            u32OPMPixelOffset = (MS_PHY)((gSrcInfo[eWindow].ScaledCropWin.y) * u16DNROffsetFor3D);
        }
#endif
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            MS_U16 u16CropRight   = 0;
            if (IsHMirrorMode(eWindow)) // H Mirror orignal crop.Left is display crop.Right
            {
                u16CropRight = (pSrcInfo->u16H_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.x - gSrcInfo[eWindow].ScaledCropWin.width);
                if (u16CropRight < gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch)
                {
                    if (bIsLeftFactor == TRUE)
                    {
                        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC, "Mirror dual case: OPM1 need get valid data at edge, pixelOffset is 0.\n");
                    }
                    else
                    {
                        // u32OPMPixlOffSetLeft  = (MS_U32)u16CropRight;
                        if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                          || IS_INPUT_CHECK_BOARD(eWindow)
                          || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
                          || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
                          || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
                          || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
                        {
                            u32OPMPixelOffset += (MS_PHY)u16CropRight/2;
                        }
                        else
                        {
                            u32OPMPixelOffset += (MS_PHY)u16CropRight;
                        }
                        //u32OPMPixelOffset += (MS_U32)u16CropRight;
                    }
                }
                else // Orignal video crop all right side.
                {
                    if (bIsLeftFactor == TRUE)
                    {
                        // u32OPMPixlOffSetRight = (MS_U32)(u16CropRight - gSrcInfo[eWindow].u16DualMiuIPMOffset);
                        u32OPMPixelOffset += (MS_PHY)(u16CropRight - gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch);
                    }
                    else
                    {
                        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC, "Mirror dual case: OPM0 need get guardband data, pixelOffset is 0.\n");
                    }
                }
            }
            else
            {
                if (gSrcInfo[eWindow].ScaledCropWin.x > gSrcInfo[eWindow].u16IPMFetch) // OPM all Left side data was cropped
                {
                    if (bIsLeftFactor == TRUE)
                    {
                        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC, "NO mirror dual case: OPM all Left side data was cropped, OPM0 pixelOffset is 0.\n");
                    }
                    else
                    {
                        //Note: u32OPMPixlOffSetRight = (MS_U32)(gSrcInfo[eWindow].ScaledCropWin.x - gSrcInfo[eWindow].u16IPMOffset);
                        u32OPMPixelOffset += (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.x - gSrcInfo[eWindow].u16IPMOffset);
                    }
                }
                else
                {
                    if (bIsLeftFactor == TRUE)
                    {
                        //u32OPMPixlOffSetLeft  = (MS_U32)(gSrcInfo[eWindow].ScaledCropWin.x);
                        if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                          || IS_INPUT_CHECK_BOARD(eWindow)
                          || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
                          || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
                          || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
                          || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
                        {
                            u32OPMPixelOffset += (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.x)/2;
                        }
                        else
                        {
                            u32OPMPixelOffset += (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.x);
                        }
                    }
                    else
                    {
                        // u32OPMPixlOffSetRight = 0;
                        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC, "NO mirror dual case: OPM1 pixel offset is 0.\n");
                    }
                }
            }
        }
        else
#endif
        {
            u32OPMPixelOffset += (MS_PHY)(gSrcInfo[eWindow].ScaledCropWin.x);
		}

#if (HW_IPM_FETCH_ALIGNMENT==FALSE)
        if (IsHMirrorMode(eWindow))
        {
            u32OPMPixelOffset += u16AlignWidth;
        }
#else
        if (IsHMirrorMode(eWindow) && pSrcInfo->bMemFmt422)
        {
#ifndef DISABLE_3D_FUNCTION
            MDrv_SC_3D_Adjust_PixelOffset(pInstance, pSrcInfo,&u32OPMPixelOffset,&u16DNROffset,&u16DNROffsetFor3D, bIsLeftFactor, eWindow);
#else
            u32OPMPixelOffset += (MS_PHY)(u16DNROffset - pSrcInfo->u16H_SizeAfterPreScaling);
#endif
        }
#endif
    }

#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts %ld\n",u32OPMPixelOffset);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts %td\n",(ptrdiff_t)u32OPMPixelOffset);
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop info: (x,y)=(%d,%d),(width,height)=(%d,%d), interlaced = %d\n",
        gSrcInfo[eWindow].ScaledCropWin.x, gSrcInfo[eWindow].ScaledCropWin.y,
        gSrcInfo[eWindow].ScaledCropWin.width, gSrcInfo[eWindow].ScaledCropWin.height,
        bInterlace);
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if ((IsEnableDualMode(eWindow)) &&
        (IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
          || IS_INPUT_CHECK_BOARD(eWindow)
          || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
          || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
          || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
          || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow)
          )
     )
    {
        if (bIsLeftFactor == TRUE)
        {
            // calculate LB offset
            u16LBOffset = u32OPMPixelOffset % (OFFSET_PIXEL_ALIGNMENT*2);            // these pixels rely on line buffer offset
        }
        else
        {
            // calculate LB offset
            u16LBOffset = u32OPMPixelOffset % OFFSET_PIXEL_ALIGNMENT;            // these pixels rely on line buffer offset
        }
    }
    else
#endif
    // calculate LB offset
    u16LBOffset = u32OPMPixelOffset % OFFSET_PIXEL_ALIGNMENT;            // these pixels rely on line buffer offset
    gSrcInfo[eWindow].Status2.u16LBOffset = u16LBOffset;
    u32OPMPixelOffset -= u16LBOffset;                                    // these pixels rely on base offset

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"LB offset %d, Pixel offset %d Alighment: %d bytes \n", u16LBOffset, (int)u32OPMPixelOffset,OFFSET_PIXEL_ALIGNMENT);

    // calcuate OPM offset
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"eFrameStoreNumber %d\n", (gSrcInfo[eWindow].Status2.eFrameStoreNumber));
    u32OPMPixelOffset *= MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
#ifdef K3_U2
    if(Hal_SC_IsPX2MemFormat(pInstance, eWindow))
    {
        u32OPMPixelOffset *= 2;
    }
#endif
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"bLinearMode = %s\n",gSrcInfo[eWindow].bLinearMode ?("TRUE"):("FALSE"));
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts for field/frame %ld (eFrameStoreNumber: %d)\n", u32OPMPixelOffset, gSrcInfo[eWindow].Status2.eFrameStoreNumber);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Crop total offset pixel counts for field/frame %td (eFrameStoreNumber: %d)\n", (ptrdiff_t)u32OPMPixelOffset, gSrcInfo[eWindow].Status2.eFrameStoreNumber);
#endif
    if(u32OPMPixelOffset != 0)
    {
        MS_U8 u8BytesPer2PixelTemp = u8BytesPer2Pixel;
#ifndef DISABLE_3D_FUNCTION
        MDrv_SC_3D_Adjust_BytesPer2Pixel(pInstance, &u8BytesPer2PixelTemp,eWindow);
#endif
        u32OPMCropOffset = u32OPMPixelOffset * u8BytesPer2PixelTemp / 2 / BYTE_PER_WORD;
#ifdef _FIELD_PACKING_MODE_SUPPORTED_MULTIPLE_TEMP
        //Attention, this is for temp solve crop address offset error in Kappa. Remove this if Kappa code flow has refined.
        printf("[Kappa_temp_patch] u32OPMCropOffset*=2\n");
        u32OPMCropOffset *= 2;
#endif
    }

#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f.LineBuf offset %d, OPMOffset 0x%lx\n", u16LBOffset, u32OPMCropOffset);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f.LineBuf offset %d, OPMOffset 0x%tx\n", u16LBOffset, (ptrdiff_t)u32OPMCropOffset);
#endif
    //KT: T3 PIP HW issue, When SUB ON, the HW operation will decrease "u16LBOffset" 2 times.

    MS_U16 CropWinWidthTemp = gSrcInfo[eWindow].ScaledCropWin.width;
#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_CropWinWidth(pInstance, &CropWinWidthTemp,eWindow);
#endif
    u16OPMFetch = CropWinWidthTemp + u16LBOffset; // using LBOffset still need to add some points to avoid generating garbage


    if(pSrcInfo->bDisplayNineLattice) // for display ninelattice no line buffer offset
    {
        u16LBOffset = u16DispOffset=0;
        u16OPMFetch = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width + 1) & ~0x1;
#ifdef K3_U2
        u16OPMFetch += HW_PIP_BUG_SW_PATCH_FETCH_INCREASE; // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
#endif
    }
    else
    {
        //  IPM offset / IPM fetch /OPM offset: All project --> 32pix align
        //  OPM fetch:
        //            Janus /T7 --> 32pix align
        //            Others ( T8/T9/T12/T13/m10/J2 ) --> 2pix align
#ifndef DISABLE_3D_FUNCTION
        MS_U16 u16HShift = 0;
#endif
        MS_U16 u16tempOPM;
        if((u16LBOffset & 0x1) != 0)
        {
            //HW issue for OPM fetch&422to444: when lboffset not even, OPM should add 2 to avoid right line garbage
            u16tempOPM = (( u16OPMFetch + 1 ) & ~1) + 2;
            if(u16tempOPM > pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
            {
                u16LBOffset--;//Overflow, set lb offset even and alignment OPM
                u16OPMFetch = u16OPMFetch & ~1; //Make even directly, since lb offset has minus 1
            }
            else
            {
                u16OPMFetch = u16tempOPM;
            }
        }
        else if((u16OPMFetch != *pu16IPMFetch)
#ifndef DISABLE_3D_FUNCTION
            &&  (E_XC_3D_OUTPUT_MODE_NONE == MDrv_XC_Get_3D_Output_Mode(pInstance))
#endif
        )
        {
            //UCDi need grab six more pixels, not 3D case.(a7p need 6 more, others need 2 more)
            u16tempOPM = (( u16OPMFetch + 1 ) & ~1) + 6;
            if(u16tempOPM > pSrcInfo->u16H_SizeAfterPreScaling - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
            {
                u16OPMFetch = u16OPMFetch & ~1;
            }
            else
            {
                u16OPMFetch = u16tempOPM;
            }
        }
        else
        {
            u16OPMFetch = ( u16OPMFetch + 1 ) & ~1;  //  pixels  alignment
        }
#ifdef K3_U2
        u16OPMFetch += HW_PIP_BUG_SW_PATCH_FETCH_INCREASE; // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
#endif
        if (!pSrcInfo->bMemFmt422)
        {
            u16OPMFetchAlignment = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
            //HW issue for OPM fetch when OP write enable, OPM Fetch have to offset pixels alignment avoid green garbage .
            if(pSrcInfo->u16H_SizeAfterPreScaling >= (u16OPMFetchAlignment+gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
            {
                u16OPMFetch = u16OPMFetchAlignment;
            }
            else
            {
                //Overflow, set the OPM Fetch to the max data area .
                u16OPMFetch = pSrcInfo->u16H_SizeAfterPreScaling - gSrcInfo[eWindow].ScaledCropWin.x + u16LBOffset;
                u16OPMFetch = u16OPMFetch  & ~1;
            }
        }
        else
        {
            if(2200>=u16OPMFetch)   //Nike 4k2k opm fetch can't do ALIGNMENT
            {
#ifdef OPMFETCH_PIXEL_ALIGNMENT
                u16OPMFetch = (u16OPMFetch + (OPMFETCH_PIXEL_ALIGNMENT - 1)) & ~(OPMFETCH_PIXEL_ALIGNMENT - 1);
#else
                u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
#endif
            }
        }
#ifndef DISABLE_3D_FUNCTION
        if(MDrv_SC_3D_Is2Dto3DCase(pInstance, MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow), MDrv_XC_Get_3D_Output_Mode(pInstance)))
        {
            u16HShift = MDrv_XC_Get_3D_HShift(pInstance);
            if ((u16OPMFetch + u16HShift) > *pu16IPMFetch)
            {
                if(*pu16IPMFetch > u16HShift)
                {
                    //opmfetch need align although ipmfetch may not need align
                    u16OPMFetch = ( *pu16IPMFetch  - u16HShift + 1 ) & ~1;
                }
                else
                {
                    u16OPMFetch = 0;
                }
            }
        }
        else
#endif
        {
            if(u16OPMFetch > pSrcInfo->u16H_SizeAfterPreScaling  - (gSrcInfo[eWindow].ScaledCropWin.x - u16LBOffset))
                XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"!!!Alert OPMFetch(%u) Overflowed, check crop setting!!!\n", u16OPMFetch);
        }
    }

#ifdef K3_U2
    // - For HW Bug in PIP when the gap between H-end of sub window and H-end of main window is too small
    if(eWindow == MAIN_WINDOW)
    {
        if (u16OPMFetch > MST_LINE_BFF_MAX)
        {
            u16OPMFetch = MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign OPM fetch as %lu due to HW limitation \n", MST_LINE_BFF_MAX);
        }
    }
    else
    {
        if(u16OPMFetch > SUB_MST_LINE_BFF_MAX)
        {
            u16OPMFetch = SUB_MST_LINE_BFF_MAX;
            XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Re-assign OPM fetch as %lu due to HW limitation \n", SUB_MST_LINE_BFF_MAX);
        }
    }
#endif

#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (IsEnableDualMode(eWindow))
    {
        MS_U16 u16OPMFetchTmp = u16OPMFetch;
        MS_U16 u16OPMCropRight   = 0;
        MS_U16 u16OPMCropLeft    = 0;
        MS_U16 u16OPMOffsetLeft  = 0;
        MS_U16 u16OPMOffsetRight = 0;

        if (IsHMirrorMode(eWindow))
        {
            // Video Right edge position
            u16OPMCropRight = pSrcInfo->ScaledCropWin.x;
            u16OPMCropLeft  = pSrcInfo->u16H_SizeAfterPreScaling - pSrcInfo->ScaledCropWin.width - pSrcInfo->ScaledCropWin.x;
            u16OPMOffsetLeft = gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch;
            u16OPMOffsetRight = gSrcInfo[eWindow].u16IPMFetch;
        }
        else
        {
            u16OPMCropRight = pSrcInfo->u16H_SizeAfterPreScaling - pSrcInfo->ScaledCropWin.width - pSrcInfo->ScaledCropWin.x;
            u16OPMCropLeft  = pSrcInfo->ScaledCropWin.x;
            u16OPMOffsetLeft = gSrcInfo[eWindow].u16IPMFetch;
            u16OPMOffsetRight = gSrcInfo[eWindow].Status2.u16DualMiuIPMFetch;
        }

        //FIXME: Get more a garbage point, OPM Right fetch 2 align
        if (u16OPMOffsetRight > u16OPMCropRight) // OPM Crop.right at Right side
        {
            if (u16OPMCropLeft > u16OPMOffsetLeft) // OPM all Left side data was cropped
            {
                if (bIsLeftFactor == TRUE)
                {
                    u16OPMFetch = OFFSET_PIXEL_ALIGNMENT;
                    //Need linebuffer Offset skip dummy data.
                    //Fixme: This should in hal just for monet
                    u16LBOffset = u16OPMFetch; /// This is wrong
                }
                else // OPM Right side
                {
                    u16OPMFetch = pSrcInfo->ScaledCropWin.width;
                    u16OPMFetch = (u16OPMFetchTmp + 1) & 0xfffe;
                }
            }
            else // OPM Left/right side have valid data after crop
            {
                u16OPMFetchTmp = u16OPMOffsetRight - u16OPMCropRight;
                if (bIsLeftFactor == TRUE)
                {
                    u16OPMFetch = pSrcInfo->ScaledCropWin.width - u16OPMFetchTmp;
                    if(IS_INPUT_SIDE_BY_SIDE_HALF(eWindow)
                      || IS_INPUT_CHECK_BOARD(eWindow)
                      || IS_INPUT_PIXEL_ALTERNATIVE(eWindow)
                      || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE(eWindow)
                      || IS_INPUT_SIDE_BY_SIDE_HALF_INTERLACE_OPT(eWindow)
                      || IS_INPUT_SIDE_BY_SIDE_FULL(eWindow))
                    {
                        u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT*2 - 1)) & ~(OFFSET_PIXEL_ALIGNMENT*2 - 1);
                    }
                    else
                    {
                        u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
                    }
                }
                else // OPM Right side
                {
                    u16OPMFetch = (u16OPMFetchTmp + 1) & 0xfffe;
                }
            }
        }
        else if (u16OPMOffsetRight == u16OPMCropRight) // OPM Crop.right at Dual Edge
        {
           if (bIsLeftFactor == TRUE) //OPM left side
           {
                u16OPMFetch = pSrcInfo->ScaledCropWin.width;
                u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
           }
           else // OPM Right side
           {
                u16OPMFetch = DUAL_DUMMYDATA_SIZE;
            }
        }
        else // OPM Crop.right in Left side
        {
            if (bIsLeftFactor == TRUE)
            {
                if (u16OPMCropRight >= (u16OPMOffsetRight + DUAL_GARDBAND_SIZE)) // Video right edge have enough
                {
                    u16OPMFetch = pSrcInfo->ScaledCropWin.width + DUAL_GARDBAND_SIZE;
                }
                else // Video right edge have no enough dual gardband
                {
                    u16OPMFetch = pSrcInfo->ScaledCropWin.width + (u16OPMCropRight - u16OPMOffsetRight);
                }
                u16OPMFetch = (u16OPMFetch + (OFFSET_PIXEL_ALIGNMENT - 1)) & ~(OFFSET_PIXEL_ALIGNMENT - 1);
            }
            else
            {
                u16OPMFetch = DUAL_DUMMYDATA_SIZE;
            }
        }
        MDrv_SC_3D_Adjust_DualOPMFetch(pInstance, &u16OPMFetch, eWindow);
    }
#endif

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"f1. u16OPMFetch after alignment %d, lboffset %d \n", u16OPMFetch, u16LBOffset);
    //printf("1.SC Crop width=0x%x, LBOffset=0x%x\n", gSrcInfo[eWindow].ScaledCropWin.width, u16LBOffset);
    u16DispOffset = 0;

    //printf("cropy=%u,cropx=%u,dnroffset=%u,bp2p=%bu\n", _stScaledCropWin.y, _stScaledCropWin.x, u16DNROffset, u8BytesPer2Pixel);
    //printf("2.OPM Crop Offset=0x%lx, LBOffset=0x%x\n", u32OPMCropOffset, u16LBOffset);

    // adjsut OPMBase address for cropping
    if(pSrcInfo ->bDisplayNineLattice)
    {
    // for linear mode set all base address the same
        u32OPMBase0 += u32OPMCropOffset;
        //u32DNRBase1 += u32OPMCropOffset;
        //u32OPMBase2 += u32OPMCropOffset;
        u32DNRBase2 = u32DNRBase1 = u32DNRBase0;
        //printf("--u32DNRBase0 %lx\n",u32DNRBase0);
    }
    else
    {
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"h.=> u32OPMBase0,1=%x,%x -> +u32OPMCropOffset[%x] =",(int)u32OPMBase0,(int)u32OPMBase1,(int)u32OPMCropOffset);
        u32OPMBase0 += u32OPMCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"0:%x \n",(int)u32OPMBase0);
        u32OPMBase1 += u32OPMCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"1:%x \n",(int)u32OPMBase1);
        u32OPMBase2 += u32OPMCropOffset;
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"2:%x \n",(int)u32OPMBase2);
    }

#if SUPPORT_2_FRAME_MIRROR

    MS_S32 s32cropshift_base0;
    MS_S32 s32cropshift_base1;
    MS_U32 u32LBOffset_mirror;
#ifdef SUPPORT_BWD
    if (bBwdEnable == TRUE) // Designer request: CropWin.y must be even number when BWR enable
    {
        MS_U16 u16CropY = gSrcInfo[eWindow].ScaledCropWin.y;
        u16CropY &= ~0x1;
        s32cropshift_base0 = u16CropY * (MS_S32) (u16DNROffsetFor3D) + gSrcInfo[eWindow].ScaledCropWin.x;
        s32cropshift_base1 = - u16CropY * (MS_S32) (u16DNROffsetFor3D) + gSrcInfo[eWindow].ScaledCropWin.x;
    }
    else
#endif
    {
    s32cropshift_base0 = gSrcInfo[eWindow].ScaledCropWin.y * (MS_S32) (u16DNROffsetFor3D) + gSrcInfo[eWindow].ScaledCropWin.x;
    s32cropshift_base1 = - gSrcInfo[eWindow].ScaledCropWin.y * (MS_S32) (u16DNROffsetFor3D) + gSrcInfo[eWindow].ScaledCropWin.x;
    }

    u32LBOffset_mirror = gSrcInfo[eWindow].ScaledCropWin.x % OFFSET_PIXEL_ALIGNMENT;

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32LBOffset:%d  \n",u32LBOffset_mirror);

    s32cropshift_base0 = ( s32cropshift_base0 - u32LBOffset_mirror ) * (MS_U32) MDrv_XC_TransFrameNumToFactor(gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);
    s32cropshift_base1 = ( s32cropshift_base1 - u32LBOffset_mirror ) * (MS_U32) MDrv_XC_TransFrameNumToFactor(gSrcInfo[eWindow].Status2.eFrameStoreNumber, gSrcInfo[eWindow].bLinearMode, _FIELD_PACKING_MODE_SUPPORTED);

    s32cropshift_base0 = ( s32cropshift_base0 * (MS_U32)(u8BitPerPixel/8) ) / (MS_U32)(BYTE_PER_WORD);
    s32cropshift_base1 = ( s32cropshift_base1 * (MS_U32)(u8BitPerPixel/8) ) / (MS_U32)(BYTE_PER_WORD);

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] s32cropshift_base0:%d  s32cropshift_base1:%d \n", s32cropshift_base0,s32cropshift_base1);

    if( (gSrcInfo[eWindow].Status2.eFrameStoreNumber == IMAGE_STORE_2_FRAMES) && (!gSrcInfo[eWindow].bInterlace) )
    {
       u32OPMBase0 = u32DNRBase0 + s32cropshift_base0;
       u32OPMBase1 = u32DNRBase1 + s32cropshift_base1;
    }

    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32DNRBase0:%x  u32DNRBase1:%x \n", (int)u32DNRBase0,(int)u32DNRBase1);
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"[SUPPORT_2_FRAME_MIRROR] u32OPMBase0:%x  u32OPMBase1:%x \n", (int)u32OPMBase0,(int)u32OPMBase1);

#endif

#if 0
    if(!pSrcInfo ->bDisplayNineLattice)
    {
        pSrcInfo->u16V_Length = pSrcInfo->u16V_Length - (gSrcInfo[eWindow].ScaledCropWin.y);
    }
#endif

    //-----------------------------------------------------------------
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen = pSrcInfo->u16V_Length;
    if(eWindow == MAIN_WINDOW)
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = u16V_Writelimit | F2_V_WRITE_LIMIT_EN;
    else
    pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VWritelimit = u16V_Writelimit | F1_V_WRITE_LIMIT_EN;
//Fixme: Need add software patch for dual miu crop window.
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (bIsLeftFactor == FALSE)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualWritelimitBase = u32WritelimitBase;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset = u16DNROffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch  = *pu16IPMFetch;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualLBOffset  = u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDispOffset  = u16DispOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase0  = u32DNRBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase1  = u32DNRBase1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase2  = u32DNRBase2;
        if (IsHMirrorMode(eWindow) && IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0  = u32OPMBase0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1  = u32OPMBase1;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2  = u32OPMBase2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = u16DNROffset;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0  = u32OPMBase0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase1  = u32OPMBase1;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase2  = u32OPMBase2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset = u16DNROffset;
        }
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch  = u16OPMFetch;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualAlignWidth = u16AlignWidth;
    }
    else
#endif
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32WritelimitBase = u32WritelimitBase;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset = u16DNROffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch  = *pu16IPMFetch;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16LBOffset  = u16LBOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DispOffset  = u16DispOffset;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0  = u32DNRBase0;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1  = u32DNRBase1;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase2  = u32DNRBase2;
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsHMirrorMode(eWindow) && IsEnableDualMode(eWindow))
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0  = u32OPMBase0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase1  = u32OPMBase1;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase2  = u32OPMBase2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset = u16DNROffset;
        }
        else
#endif
        {
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0  = u32OPMBase0;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1  = u32OPMBase1;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2  = u32OPMBase2;
            pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = u16DNROffset;
        }
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch = u16OPMFetch;
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16AlignWidth = u16AlignWidth;
    }

#ifdef SUPPORT_BWD
    if(bBwdEnable)
    {
        if(u8BitPerPixel == BIT_PER_PIX_VAULE_16)
        {
            eBPPType = E_XC_BWR_MEM_CFG_BPP_16;
        }
        else if(u8BitPerPixel == BIT_PER_PIX_VAULE_20)
        {
            eBPPType = E_XC_BWR_MEM_CFG_BPP_20;
        }
        else if(u8BitPerPixel == BIT_PER_PIX_VAULE_24)
        {
            eBPPType = E_XC_BWR_MEM_CFG_BPP_24;
        }
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if(bIsLeftFactor == TRUE)
        {
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_DNRBase0 = u32DNRBase0;
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_DNRBase1 = u32DNRBase1;
            if (IsHMirrorMode(eWindow) && IsEnableDualMode(eWindow))
            {
                pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_OPMBase0 = u32OPMBase0;
                pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_OPMBase1 = u32OPMBase1;
            }
            else
            {
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase0 = u32OPMBase0;
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase1 = u32OPMBase1;
            }
            //printf("eBPPType: %d u32BWR_Miu_Left_DNRBase0: 0x%x.\n", eBPPType, pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_DNRBase0);
            //printf("eBPPType: %d u32BWR_Miu_Left_OPMBase0: 0x%x.\n", eBPPType, pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase0);

        }
        else
        {
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_DNRBase0 = u32DNRBase0;
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_DNRBase1 = u32DNRBase1;
            if (IsHMirrorMode(eWindow) && IsEnableDualMode(eWindow))
            {
                pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase0 = u32OPMBase0;
                pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase1 = u32OPMBase1;
            }
            else
            {
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_OPMBase0 = u32OPMBase0;
            pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_OPMBase1 = u32OPMBase1;
            }
            //printf("eBPPType: %d u32BWR_Miu_Right_DNRBase0: 0x%x.\n", eBPPType, pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_DNRBase0);
            //printf("eBPPType: %d u32BWR_Miu_Right_OPMBase0: 0x%x.\n", eBPPType, pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Right_OPMBase0);
        }
#else
        pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_DNRBase0 = u32DNRBase0;
        pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_DNRBase1 = u32DNRBase1;
        pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase0 = u32OPMBase0;
        pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase1 = u32OPMBase1;
        //printf("eBPPType: %d u32BWR_Miu_Left_DNRBase0: 0x%x.\n", eBPPType, pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_DNRBase0);
        //printf("eBPPType: %d u32BWR_Miu_Left_OPMBase0: 0x%x.\n", eBPPType, pSrcInfo->Status2.stBWRBase[eBPPType].u32BWR_Miu_Left_OPMBase0);
#endif
    }
#endif

#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (bIsLeftFactor == FALSE)
    {
        pSrcInfo->Status2.u32DualMiuDNRBase0  = u32DNRBase0 * BYTE_PER_WORD;
        pSrcInfo->Status2.u32DualMiuDNRBase1  = u32DNRBase1 * BYTE_PER_WORD;
        pSrcInfo->Status2.u32DualMiuDNRBase2  = u32DNRBase2 * BYTE_PER_WORD;
        pSrcInfo->Status2.u16DualMiuDNROffset = u16DNROffset;
        pSrcInfo->Status2.u16DualMiuIPMFetch  = *pu16IPMFetch;
    }
    else
#endif
    {
        pSrcInfo->u32IPMBase0 = u32DNRBase0 * BYTE_PER_WORD;
        pSrcInfo->u32IPMBase1 = u32DNRBase1 * BYTE_PER_WORD;
        pSrcInfo->u32IPMBase2 = u32DNRBase2 * BYTE_PER_WORD;
        pSrcInfo->u16IPMOffset = u16DNROffset;
    }


#ifndef DISABLE_3D_FUNCTION
    MDrv_SC_3D_Adjust_FetchOffset(pInstance, pSrcInfo, eWindow);
#endif

#ifdef ENABLE_SCALING_WO_MUTE
    MDrv_SC_Adjust_Skip_OPM_line(pInstance, eWindow);
#endif
    //-----------------------------------------------------------------
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (bIsLeftFactor == FALSE)
    {
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final Dual DNR Offset/Fetch=0x%x, 0x%x, OPM Offset/Fetch=0x%x, 0x%x, V_Length=0x%x\n",
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNROffset,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualDNRFetch,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMOffset,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DualOPMFetch,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen);
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final Dual u32DNRBase0/1/2=0x%lx, 0x%lx, 0x%lx, u32OPMBase0/1/2=0x%lx, 0x%lx, 0x%lx\n",
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase0,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase1,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase2,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase1,
                                      pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase2);
#else
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final Dual u32DNRBase0/1/2=0x%tx, 0x%tx, 0x%tx, u32OPMBase0/1/2=0x%tx, 0x%tx, 0x%tx\n",
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase0,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase1,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualDNRBase2,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase0,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase1,
                                      (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DualOPMBase2);

#endif
    }
    else
#endif
    {
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final DNR Offset/Fetch=0x%x, 0x%x, OPM Offset/Fetch=0x%x, 0x%x, V_Length=0x%x\n",
                     pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNROffset,
                     pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16DNRFetch,
                     pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset,
                     pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMFetch,
                     pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16VLen);
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final u32DNRBase0/1/2=0x%lx, 0x%lx, 0x%lx, u32OPMBase0/1/2=0x%lx, 0x%lx, 0x%lx\n",
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase2,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1,
                                          pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2);
#else
        XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"Final u32DNRBase0/1/2=0x%tx, 0x%tx, 0x%tx, u32OPMBase0/1/2=0x%tx, 0x%tx, 0x%tx\n",
                                          (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase0,
                                          (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase1,
                                          (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32DNRBase2,
                                          (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase0,
                                          (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase1,
                                          (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32OPMBase2);
#endif
    }
    XC_LOG_TRACE(XC_DGBLEVEL_CROPCALC,"---------------------------------------------------\n");
}

void MDrv_SC_set_fetch_number_limit(void *pInstance, XC_InternalStatus *pSrcInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#ifdef SUPPORT_BWD
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (IsEnableDualMode(eWindow))
    {
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_16, TRUE, TRUE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_16, TRUE, FALSE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_20, TRUE, TRUE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_20, TRUE, FALSE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_24, TRUE, TRUE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_24, TRUE, FALSE, eWindow );
    }
    else
#endif
    {
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_16, TRUE, TRUE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_20, TRUE, TRUE, eWindow );
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], BIT_PER_PIX_VAULE_24, TRUE, TRUE, eWindow );
    }
#endif
    _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], gSrcInfo[eWindow].u8BitPerPixel, FALSE, TRUE, eWindow );

#ifdef UFO_XC_SUPPORT_DUAL_MIU
    if (IsEnableDualMode(eWindow))
    {
        _MDrv_SC_set_fetch_number_limit_impl(pInstance, &gSrcInfo[eWindow], gSrcInfo[eWindow].u8BitPerPixel, FALSE, FALSE, eWindow );
    }
#endif
}

void MDrv_SC_set_shift_line(void *pInstance, MS_BOOL bFBL, MS_DEINTERLACE_MODE eDeInterlaceMode, SCALER_WIN eWindow)
{
    MS_U8 u8Val;

    if(!bFBL)
    {  // FB
        u8Val = 0x0;
    }
    else
    {   // FBL
        if((MS_DEINT_2DDI_BOB == eDeInterlaceMode) || (MS_DEINT_2DDI_AVG == eDeInterlaceMode))
            u8Val = 0x0;
        else
            u8Val = 0x2;
    }

    Hal_SC_set_shiftline(pInstance, u8Val, eWindow );
}

MS_BOOL MDrv_XC_EnableMirrorModeEx(void *pInstance, MirrorMode_t eMirrorMode,  SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    #if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 1)
    MS_BOOL bNeedRestore = FALSE;
    #endif

    switch (eMirrorMode)
    {
        case MIRROR_NORMAL:
        case MIRROR_H_ONLY:
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror = FALSE;
            bRet = TRUE;
            break;
        case MIRROR_V_ONLY:
        case MIRROR_HV:
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.bMirror = TRUE;
            bRet = TRUE;
            break;
        default:
            printf("Unsupport MirrorMode Type\n ");
            eMirrorMode = MIRROR_NORMAL;
            bRet = FALSE;
            break;
    }
    gSrcInfo[eWindow].Status2.eMirrorMode = eMirrorMode;

    #if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 1)
    if((psXCInstPri->u32DeviceID == E_XC_DEVICE1) && (Hal_SC_Is_InputSource_Disable(pInstance, eWindow) == FALSE))
    {
        bNeedRestore = TRUE;
    }
    #endif

    MDrv_XC_DisableInputSource(pInstance, TRUE, eWindow);
    MDrv_SC_set_mirrorEx(pInstance, eMirrorMode, eWindow);

    #if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 1)
    if((psXCInstPri->u32DeviceID == E_XC_DEVICE1) && bNeedRestore)
    {
        bNeedRestore = FALSE;
        MDrv_XC_DisableInputSource(pInstance, FALSE, eWindow);
    }
    #endif

    return bRet;
}

void MDrv_SC_set_mirror(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    MS_PHY u32WritelimitBase;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (HW_DESIGN_4K2K_VER != 4)
    if ( bEnable )
    {
        // Set Limit at top of frame buffer
        //limit_min :(0: Maximum   1: Minimum)
        if(eWindow == MAIN_WINDOW)
            u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
        else
            u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
    }
    else
    {
        // Set limit at bottom of frame buffer
        if(eWindow == MAIN_WINDOW)
            u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
        else
            u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
    }
    Hal_SC_set_write_limit(pInstance, u32WritelimitBase, eWindow);
    Hal_SC_set_mirror(pInstance, bEnable, eWindow );
#else
    if (
        (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
           ||(psXCInstPri->u32DeviceID == 1))
        || (IS_INPUT_4K2K(eWindow))
       )
    {
        if ( bEnable )
        {
            // Set Limit at top of frame buffer
            //limit_min :(0: Maximum   1: Minimum)
            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
            else
                u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
        }
        else
        {
            // Set limit at bottom of frame buffer
            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
            else
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
        }
        Hal_SC_set_write_limit(pInstance, u32WritelimitBase, eWindow);
        Hal_SC_set_mirror(pInstance, bEnable, eWindow );
    }
    else
    {
        if ( bEnable )
        {
            // Set Limit at top of frame buffer
            //limit_min :(0: Maximum   1: Minimum)
            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase = (MS_FRCM_BASE0(eWindow) - 1) | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
            else
                u32WritelimitBase = (MS_FRCM_BASE0(eWindow) - 1) | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
        }
        else
        {
            // Set limit at bottom of frame buffer
            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
            else
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
        }
        Hal_SC_set_frcm_write_limit(pInstance, u32WritelimitBase, eWindow);
        Hal_SC_set_frcm_mirror(pInstance, bEnable, eWindow );
        if (gSrcInfo[eWindow].bMemYUVFmt)
        {
            if (bEnable)
            {
                MDrv_SC_set_frcm_cbcr_swap(pInstance, TRUE, eWindow);
            }
            else
            {
                MDrv_SC_set_frcm_cbcr_swap(pInstance, FALSE, eWindow);
            }
        }
    }
#endif
}

static void _MDrv_SC_Set_Mirror(void *pInstance, MirrorMode_t eMirrorMode, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32MirrorReg;

#if (HW_DESIGN_4K2K_VER != 4)
    if(eWindow == MAIN_WINDOW)
    {
        u32MirrorReg = REG_SC_BK12_03_L;
    }
    else
    {
        u32MirrorReg = REG_SC_BK12_43_L;
    }
#else
    if (
        (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
          ||(psXCInstPri->u32DeviceID == 1))
        ||(IS_INPUT_4K2K(eWindow))
       )
    {
        if(eWindow == MAIN_WINDOW)
        {
            u32MirrorReg = REG_SC_BK12_03_L;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_03_L, 0, BIT(12));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_03_L, 0, BIT(13));
        }
        else
        {
            u32MirrorReg = REG_SC_BK12_43_L;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_43_L, 0, BIT(12));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK32_43_L, 0, BIT(13));
        }
    }
    else
    {
        if(eWindow == MAIN_WINDOW)
        {
            u32MirrorReg = REG_SC_BK32_03_L;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, 0, BIT(12));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_03_L, 0, BIT(13));
        }
        else
        {
            u32MirrorReg = REG_SC_BK32_43_L;
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L, 0, BIT(12));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_43_L, 0, BIT(13));
        }
    }
#endif

    if (eMirrorMode == MIRROR_NORMAL)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, 0, BIT(12));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, 0, BIT(13));
        MDrv_SC_set_frcm_cbcr_swap(pInstance, FALSE, eWindow);
    }
    else if (eMirrorMode == MIRROR_H_ONLY)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, BIT(12), BIT(12));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, 0, BIT(13));
        if (!IS_INPUT_4K2K(eWindow))
        {
            if (gSrcInfo[eWindow].bMemYUVFmt)
            {
                MDrv_SC_set_frcm_cbcr_swap(pInstance, TRUE, eWindow);
            }
        }
    }
    else if (eMirrorMode == MIRROR_V_ONLY)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, 0, BIT(12));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, BIT(13), BIT(13));
        MDrv_SC_set_frcm_cbcr_swap(pInstance, FALSE, eWindow);

#if  SUPPORT_2_FRAME_MIRROR
        SC_W2BYTEMSK(u32MirrorReg, 0, BIT(13)); //disable original V mirror
#endif
    }
    else// MIRROR_HV
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32MirrorReg, (BIT(12)|BIT(13)), (BIT(12)|BIT(13)));
        if (!IS_INPUT_4K2K(eWindow))
        {
            if (gSrcInfo[eWindow].bMemYUVFmt)
            {
                MDrv_SC_set_frcm_cbcr_swap(pInstance, TRUE, eWindow);
            }
        }
#if  SUPPORT_2_FRAME_MIRROR
        SC_W2BYTEMSK(u32MirrorReg, 0, BIT(13)); //disable original V mirror
#endif
    }

}

void MDrv_SC_set_mirrorEx(void *pInstance, MirrorMode_t eMirrorMode, SCALER_WIN eWindow)
{
    MS_PHY u32WritelimitBase = 0;
#if (HW_DESIGN_4K2K_VER == 4)
    MS_PHY u32FRCMWritelimitBase = 0;
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (HW_DESIGN_4K2K_VER != 4)
    if ( (eMirrorMode == MIRROR_HV) || (eMirrorMode == MIRROR_V_ONLY) )
    {
        // Set Limit at top of frame buffer
        //limit_min :(0: Maximum   1: Minimum)
        if(eWindow == MAIN_WINDOW)
            u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
        else
            u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
    }
    else // NORMAL or H_MIRROR case
    {
        // Set limit at bottom of frame buffer
        if(eWindow == MAIN_WINDOW)
            u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
        else
            u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
    }
    Hal_SC_set_write_limit(pInstance, u32WritelimitBase, eWindow);
    _MDrv_SC_Set_Mirror(pInstance, eMirrorMode, eWindow);
#else
    if (
        (((psXCInstPri->u32DeviceID == 0) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE))
          ||(psXCInstPri->u32DeviceID == 1))
        ||(IS_INPUT_4K2K(eWindow))
       )
    {
        if ( (eMirrorMode == MIRROR_HV) || (eMirrorMode == MIRROR_V_ONLY) )
        {
            // Set Limit at top of frame buffer
            //limit_min :(0: Maximum   1: Minimum)
            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F2_WRITE_LIMIT_EN | F2_WRITE_LIMIT_MIN;
            else
                u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F1_WRITE_LIMIT_EN | F1_WRITE_LIMIT_MIN;
        }
        else // NORMAL or H_MIRROR case
        {
            // Set limit at bottom of frame buffer
            if(eWindow == MAIN_WINDOW)
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
            else
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
        }
        Hal_SC_set_write_limit(pInstance, u32WritelimitBase, eWindow);
        _MDrv_SC_Set_Mirror(pInstance, eMirrorMode, eWindow);
    }
    else
    {

        if ( (eMirrorMode == MIRROR_HV) || (eMirrorMode == MIRROR_V_ONLY) )
        {
            // Set Limit at top of frame buffer
            //limit_min :(0: Maximum   1: Minimum)
            if(eWindow == MAIN_WINDOW)
            {
                u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F2_WRITE_LIMIT_EN;
                u32FRCMWritelimitBase = (MS_FRCM_BASE0(eWindow) - 1) | F2_FRCM_WRITE_LIMIT_EN;
            }
            else
            {
                u32WritelimitBase = (MS_IPM_BASE0(eWindow) - 1) | F1_WRITE_LIMIT_EN;
                u32FRCMWritelimitBase = (MS_FRCM_BASE1(eWindow) - 1) | F1_FRCM_WRITE_LIMIT_EN;

            }
        }
        else // NORMAL or H_MIRROR case
        {
            // Set limit at bottom of frame buffer
            if(eWindow == MAIN_WINDOW)
            {
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_WRITE_LIMIT_EN;
                u32FRCMWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]) / BYTE_PER_WORD - 1) | F2_FRCM_WRITE_LIMIT_EN;
            }
            else
            {
                u32WritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32DNRBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_WRITE_LIMIT_EN;
                u32FRCMWritelimitBase = ((pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBaseAddr0[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow]) / BYTE_PER_WORD - 1) | F1_FRCM_WRITE_LIMIT_EN;
            }
        }

        Hal_SC_set_write_limit(pInstance, u32WritelimitBase, eWindow);
        /// the following can be ignored
        //Hal_SC_set_frcm_write_limit(pInstance, u32FRCMWritelimitBase, eWindow);
        _MDrv_SC_Set_Mirror(pInstance, eMirrorMode, eWindow);
    }
#endif
}

void MDrv_XC_FilLineBuffer(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    HAL_SC_FillLineBuffer(pInstance, bEnable, eWindow);
}

MS_BOOL MDrv_XC_GetHSizeChangeManuallyFlag(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Scaling._bHSizeChangedManually;
}

MS_PHY MDrv_XC_GetIPMBase(void *pInstance, MS_U8 num, SCALER_WIN eWindow)
{
    MS_PHY Addr = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (num == 0)
    {
        Addr = MS_IPM_BASE0(eWindow);
    }
    else if (num == 1)
    {
        Addr = MS_IPM_BASE1(eWindow);
    }
    return Addr;
}

MS_U32 MDrv_XC_GetDNRBufSize(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow];
}

void MDrv_XC_SetDNRBufSize(void *pInstance, MS_U32 u32DNRBufSize, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow] = u32DNRBufSize;
}

MS_PHY MDrv_XC_GetDualIPMBase(void *pInstance, MS_U8 num, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    MS_PHY Addr = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (num == 0)
    {
        Addr = MS_IPM_DUAL_MIU_BASE0(eWindow);
    }
    else if (num == 1)
    {
        Addr = MS_IPM_DUAL_MIU_BASE1(eWindow);
    }
    return Addr;
#else
    return 0;
#endif
}

MS_U32 MDrv_XC_GetDualDNRBufSize(void *pInstance, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow];
#else
    return 0;
#endif
}

void MDrv_XC_SetDualDNRBufSize(void *pInstance, MS_U32 u32DNRBufSize, SCALER_WIN eWindow)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow] = u32DNRBufSize;
#endif
}


// calc width or height based on existed dnr buffer and byte/pixel
MS_U32 MDrv_XC_GetAvailableSize(void *pInstance, SCALER_WIN eWindow, MS_U8 u8FBNum, MS_U32 u32InputSize)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    MS_U32 u32FrameBuffSize = 0;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(u8FBNum == 0)
    {
        printf("[%s,%5d] the u8FBNum cannot be zero\n",__FUNCTION__,__LINE__);
        return 0;
    }

    if(u32InputSize == 0)
    {
        printf("[%s,%5d] the u32InputSize cannot be zero\n",__FUNCTION__,__LINE__);
        return 0;
    }

    if(gSrcInfo[eWindow].u8BitPerPixel == 0)
    {
        printf("[%s,%5d] the u8BitPerPixel cannot be zero\n",__FUNCTION__,__LINE__);
        return 0;
    }

#if (HW_DESIGN_4K2K_VER == 4)
    if((!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE)
       &&(IS_INPUT_4K2K(eWindow)))
       ||((E_XC_3D_INPUT_FRAME_ALTERNATIVE == MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)) && (!(gSrcInfo[eWindow].bInterlace))))
    {
        u32FrameBuffSize = pXCResourcePrivate->stdrvXC_Scaling._u32FRCMBufSize[eWindow];
    }
    else
#endif
    {
#ifdef UFO_XC_SUPPORT_DUAL_MIU
        if (IsEnableDualMode(eWindow))
        {
            u32FrameBuffSize = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow] + pXCResourcePrivate->stdrvXC_Scaling._u32DualMiuDNRBufSize[eWindow];
        }
        else
#endif
        {
            u32FrameBuffSize = pXCResourcePrivate->stdrvXC_Scaling._u32DNRBufSize[eWindow];
        }
    }

    return (u32FrameBuffSize * 8 / u8FBNum / gSrcInfo[eWindow].u8BitPerPixel / u32InputSize);
}

MS_U32 MApi_XC_GetAvailableSize_U2(void* pInstance, SCALER_WIN eWindow, MS_U8 u8FBNum, MS_U32 u32InputSize)
{
    MS_U32 u32Return = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    u32Return = MDrv_XC_GetAvailableSize(pInstance, eWindow, u8FBNum, u32InputSize);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32Return;
}

MS_U32 MApi_XC_GetAvailableSize(SCALER_WIN eWindow, MS_U8 u8FBNum, MS_U32 u32InputSize)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_AVAILABLE_SIZE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8FBNum = u8FBNum;
    XCArgs.u32InputSize = u32InputSize;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_AVAILABLE_SIZE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u32ReturnValue;
    }
}

void MDrv_SC_Set_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow)
{
    Hal_SC_set_pre_align_pixel(pInstance, bEnable, pixels, eWindow );
}

void MDrv_SC_ClearScalingFactorForInternalCalib(void *pInstance)
{
    Hal_XC_ClearScalingFactorForInternalCalib(pInstance);
}

void MDrv_SC_Enable_PreScaling(void *pInstance, MS_BOOL bHSDEnable, MS_BOOL bVSDEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow==MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_05_L, bHSDEnable<<15, BIT(15));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_09_L,  bVSDEnable<<15, BIT(15));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_05_L,  bHSDEnable<<15, BIT(15));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_09_L,  bVSDEnable<<15, BIT(15));
    }
}

void MDrv_SC_Enable_PostScaling(void *pInstance, MS_BOOL bHSPEnable, MS_BOOL bVSPEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(eWindow == MAIN_WINDOW)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_08_L, bHSPEnable<<8, BIT(8));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_0A_L,  bVSPEnable<<8, BIT(8));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_28_L,  bHSPEnable<<8, BIT(8));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK23_2A_L,  bVSPEnable<<8, BIT(8));
    }
}

void MDrv_SC_SetFullRangeCapture(void *pInstance, SCALER_WIN eWindow)
{
    Hal_SC_ip_set_capture_v_start(pInstance, 0x06,eWindow);
    Hal_SC_ip_set_capture_h_start(pInstance, 0x06,eWindow);
    Hal_SC_ip_set_capture_v_size(pInstance, 0x1FFF,eWindow);
    Hal_SC_ip_set_capture_h_size(pInstance, 0x1FFF,eWindow);
}

MS_BOOL MDrv_SC_GetScmiV1Speical4frame(void *pInstance, XC_FRAME_STORE_NUMBER enStoreNum)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    return (enStoreNum == IMAGE_STORE_4_FRAMES) && ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK12_04_L) & BIT(6)) != 0);
}


//-------------------------------------------------------------------------------------------------
/// Set two initial factors mode to improve quality in FBL
/// @param  bEnable                \b IN: enable or disable two initial factors mode
/// @param  eWindow                \b IN: @ref SCALER_WIN
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Enable_TwoInitFactor_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(bEnable)
    {
#ifdef UFO_XC_FB_LEVEL
        if(((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL) || (gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
            && gSrcInfo[MAIN_WINDOW].bInterlace
        )
#else
        if(gSrcInfo[MAIN_WINDOW].bFBL && gSrcInfo[MAIN_WINDOW].bInterlace)
#endif
        {
            HAL_SC_Enable_VInitFactor(pInstance, TRUE, eWindow);
            bRet = TRUE;
        }
    }
    else
    {
        HAL_SC_Enable_VInitFactor(pInstance, FALSE, eWindow);
        bRet = TRUE;
    }

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bRet;
}

MS_BOOL MApi_XC_Enable_TwoInitFactor(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_2INITFACTOR_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_2INITFACTOR_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

MS_BOOL MDrv_XC_GetUCEnabled(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bUCEnabled = FALSE;

    if (eWindow == MAIN_WINDOW)
    {
        bUCEnabled = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2A_02_L, 0x8000) >> 15;
    }
    else
    {
        bUCEnabled = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2A_02_L, 0x2000) >> 13;
    }

    return bUCEnabled;
}

MS_BOOL MApi_XC_GetUCEnabled_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bUCEnabled;

    _XC_ENTRY(pInstance);
    bUCEnabled = MDrv_XC_GetUCEnabled(pInstance, eWindow);
    _XC_RETURN(pInstance);

    return bUCEnabled;
}

MS_BOOL MApi_XC_GetUCEnabled(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_UC_ENABLED XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_UC_ENABLED, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_SetWRBankMappingNum_U2(void* pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_set_wr_bank_mapping_num(pInstance, u8Val, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetWRBankMappingNum(MS_U8 u8Val, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_RWBANK_MAPPING_NUM XCArgs;
    XCArgs.u8Val = u8Val;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_RWBANK_MAPPING_NUM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U8 MApi_XC_GetWRBankMappingNum_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U8 u8WRBankMappingNum = 0;

    //_XC_ENTRY(pInstance);
    u8WRBankMappingNum = Hal_SC_Get_WR_Bank_Mapping_Num(pInstance, eWindow);
    //_XC_RETURN(pInstance);
    return u8WRBankMappingNum;
}

MS_U8 MApi_XC_GetWRBankMappingNum(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_RWBANK_MAPPING_NUM XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RWBANK_MAPPING_NUM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_U8 MApi_XC_GetWRBankMappingNumForZap_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_U8 u8WRBankMappingNum = 0;
    XC_ApiStatus stXCStatus;
    memset(&stXCStatus, 0, sizeof(XC_ApiStatus));

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    if (MDrv_XC_GetStatus(pInstance, &stXCStatus, eWindow))
    {
        MS_BOOL bInterlace = stXCStatus.bInterlace;
        MS_BOOL bUCEnabled = MDrv_XC_GetUCEnabled(pInstance, eWindow);

        u8WRBankMappingNum = Hal_SC_Get_WR_Bank_Mapping_Num(pInstance, eWindow);

        if (bUCEnabled)
        {
            if (bInterlace)
            {
                u8WRBankMappingNum += 2;
            }
            else
            {
                u8WRBankMappingNum = 3;
            }
        }
    }
    else
    {
        printf("[%s]:get status fail!\n",__FUNCTION__);
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u8WRBankMappingNum;
}

MS_U8 MApi_XC_GetWRBankMappingNumForZap(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_RWBANK_MAPPING_NUM_FOR_ZAP XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.u8ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_RWBANK_MAPPING_NUM_FOR_ZAP, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return 0;
    }
    else
    {
        return XCArgs.u8ReturnValue;
    }
}

MS_BOOL MApi_XC_SetBOBMode_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_BOOL bRet = FALSE;
    XC_ApiStatus stXCStatus;
    memset(&stXCStatus, 0, sizeof(XC_ApiStatus));

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    if (MDrv_XC_GetStatus(pInstance, &stXCStatus, eWindow))
    {
        MS_BOOL bInterlace = stXCStatus.bInterlace;

        if (eWindow == MAIN_WINDOW)
        {
            if (bInterlace && bEnable)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_78_L, 0x008F, 0x00FF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_79_L, 0x008F, 0x00FF);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_78_L, 0, 0x00FF);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_79_L, 0, 0x00FF);
            }
        }
        else
        {
            if (bInterlace && bEnable)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_78_L, 0x8F00, 0xFF00);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_79_L, 0x8F00, 0xFF00);
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_78_L, 0, 0xFF00);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK22_79_L, 0, 0xFF00);
            }
        }
        bRet = TRUE;
    }
    else
    {
        printf("[%s]:get status fail!\n",__FUNCTION__);
        bRet = FALSE;
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bRet;
}

MS_BOOL MApi_XC_SetBOBMode(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_BOBMODE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BOBMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_SetForceReadBank_U2(void* pInstance, MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    MHal_XC_SetForceReadBank(pInstance, bEnable, u8Bank, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetForceReadBank(MS_BOOL bEnable, MS_U8 u8Bank, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FORCE_READBANK XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.u8Bank = u8Bank;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FORCE_READBANK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_SetForceCurrentReadBank(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    MHal_XC_SetForceCurrentReadBank(pInstance, bEnable, eWindow);
}

void MDrv_SC_Adjust_Prescaling_Ratio(void *pInstance, SCALER_WIN eWindow)
{
    #ifdef ENABLE_SCALING_WO_MUTE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    //***************************************************************************
    // special SWDS setwindow
    if (((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_L) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_R)))
    {
        if (FALSE == gSrcInfo[eWindow].Status2.bPreHCusScaling)
        {
            gSrcInfo[eWindow].Status2.bPreHCusScaling = TRUE;
            gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = gSrcInfo[eWindow].stCapWin.width;
            gSrcInfo[eWindow].Status2.u16PreHCusScalingDst = gSrcInfo[eWindow].stCapWin.width;
        }

        if (FALSE == gSrcInfo[eWindow].Status2.bPreVCusScaling)
        {
            gSrcInfo[eWindow].Status2.bPreVCusScaling = TRUE;
            gSrcInfo[eWindow].Status2.u16PreVCusScalingSrc = gSrcInfo[eWindow].stCapWin.height;
            gSrcInfo[eWindow].Status2.u16PreVCusScalingDst = gSrcInfo[eWindow].stCapWin.height;
        }
    }
    //****************************************************************************
    #endif
}

MS_U16 MDrv_SC_Cal_Skip_OPM_line(void *pInstance, SCALER_WIN eWindow, MS_U16 u16Src, MS_U16 u16Dst, XC_InternalStatus *pSrcInfo)
{
    #ifdef ENABLE_SCALING_WO_MUTE
    //***************************************************************************
    // special SWDS setwindow
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    #define V_THRESHOLD 85  // 0.88x
    #define V_THRESHOLD_SD_PANEL 40  // 0.4x
    #define V_THRESHOLD_SD_PANEL_SD_SOURCE 20  // 0.2x
    #define V_THRESHOLD_Sub 60  // 0.6x

    MS_U16 u16TmpCropScaledSrc = u16Src;
    MS_U16 u16TmpSrc = u16Src;
    MS_U16 u16V_Threshold = 100;

    //printf("\033[0;31m [%s][%d] u16ScaleSrc: %d  \033[0m\n", __FUNCTION__, __LINE__, u16ScaleSrc);
    //printf("\033[0;31m [%s][%d] u16ScaleDst: %d  \033[0m\n", __FUNCTION__, __LINE__, u16ScaleDst);
    if ((MAIN_WINDOW == eWindow) &&
        ((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_L) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_R)))
    {
        MS_U16 u16VactSpace=0;
        if(MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow) == E_XC_3D_INPUT_FRAME_PACKING)
        {
            MDrv_SC_3D_GetFP_Info(pInstance, pSrcInfo, &u16TmpCropScaledSrc, &u16VactSpace
                                  , (pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u32V_PreScalingRatio & BIT(31))>>31);
        }
    }

    u16TmpSrc = u16TmpCropScaledSrc;

    if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > 1900)
        && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height > 1000) ) // For FullHD panel
    {
        if (MAIN_WINDOW == eWindow)
            u16V_Threshold = V_THRESHOLD;
        else
            u16V_Threshold = V_THRESHOLD_Sub;
    }
    else
    {
        if (pSrcInfo->stCapWin.width < 900 && pSrcInfo->stCapWin.height < 700)
            u16V_Threshold = V_THRESHOLD_SD_PANEL_SD_SOURCE;
        else
            u16V_Threshold = V_THRESHOLD_SD_PANEL;
    }


    pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[eWindow] = 1;
    if ((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_L) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_R))
    {
        while((u16TmpSrc * u16V_Threshold) > (u16Dst * 100))
        {
            pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[eWindow]++;

            if (pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[eWindow] > 4)
            {
                pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[eWindow] = 4;
                break;
            }

            u16TmpSrc = u16TmpCropScaledSrc/pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[eWindow] -2 ;
        }
    }

    //printf("\033[0;31m [%s][%d] u8OPMFetchRatio: %d  \033[0m\n", __FUNCTION__, __LINE__, g_u8OPMFetchRatio);
    //printf("\033[0;31m [%s][%d] u16TmpSrc: %d  \033[0m\n", __FUNCTION__, __LINE__, u16TmpSrc);

    return u16TmpSrc;

    #else

    return 0;

    #endif
    //****************************************************************************
}

void MDrv_SC_Adjust_H_Crop(void *pInstance, SCALER_WIN eWindow)
{
    #ifdef ENABLE_SCALING_WO_MUTE
    //*********************************************
    // For H scaling down to too small size, there is HW limitation only support 16x H scaling down
    #define H_PostScaledLimit 16
    #define H_CropShift 64
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    while ((gSrcInfo[eWindow].ScaledCropWin.width > H_PostScaledLimit)
        && ((gSrcInfo[eWindow].ScaledCropWin.width / H_PostScaledLimit) > gSrcInfo[eWindow].stDispWin.width))
    {

        //printf("\033[0;31m [%s][%d] Need Crop CASE!!!!!  \033[0m\n", __FUNCTION__, __LINE__);
        //printf("\033[0;31m [%s][%d] gSrcInfo[eWindow].ScaledCropWin.x: %d  \033[0m\n", __FUNCTION__, __LINE__, gSrcInfo[eWindow].ScaledCropWin.x);
        //printf("\033[0;31m [%s][%d] gSrcInfo[eWindow].ScaledCropWin.width: %d  \033[0m\n", __FUNCTION__, __LINE__, gSrcInfo[eWindow].ScaledCropWin.width);

        gSrcInfo[eWindow].ScaledCropWin.x += H_CropShift;

        if (gSrcInfo[eWindow].ScaledCropWin.width < H_CropShift * 2)
        {
            gSrcInfo[eWindow].ScaledCropWin.width = H_CropShift * 2;
            break;
        }

        gSrcInfo[eWindow].ScaledCropWin.width -= H_CropShift * 2;

        //printf("\033[0;31m [%s][%d] NEW .ScaledCropWin.x: %d  \033[0m\n", __FUNCTION__, __LINE__, gSrcInfo[eWindow].ScaledCropWin.x);
        //printf("\033[0;31m [%s][%d] NEW ScaledCropWin.width: %d  \033[0m\n", __FUNCTION__, __LINE__, gSrcInfo[eWindow].ScaledCropWin.width);
    }
    //*********************************************

    #endif
}

void MDrv_SC_Adjust_Skip_OPM_line(void *pInstance, SCALER_WIN eWindow)
{
    #ifdef ENABLE_SCALING_WO_MUTE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (((MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_L) ||
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_R)))
    {
        // adjust OPM offset to make HW skip line
        pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset = pXCResourcePrivate->stdrvXC_MVideo_Context.stDBreg.u16OPMOffset
                                                                          * pXCResourcePrivate->stdrvXC_Scaling._u8OPMFetchRatio[eWindow];
    }
    #endif
}

void MDrv_SC_Enable_LegacyMode(void *pInstance, MS_BOOL bEnable)
{
    HAL_SC_EnableLegacyMode(pInstance, bEnable);
}

void MDrv_SC_SwitchIPMWriteBank(void *pInstance, MS_BOOL bEnable, E_XC_IPM_CONNECT_ID eIPMConnect)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (E_XC_IPM_CONNETC_MAIN_FRCM == eIPMConnect || E_XC_IPM_CONNETC_MAIN_OPM == eIPMConnect)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_07_L, eIPMConnect<<0, 0x03);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_07_L, (bEnable? BIT(3):0), BIT(3));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_47_L, eIPMConnect<<0, 0x03);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK11_47_L, (bEnable? BIT(3):0), BIT(3));
    }
#endif
}

void MDrv_SC_SwitchFRCMWriteBank(void *pInstance, MS_BOOL bEnable, E_XC_FRCMW_CONNECT_ID eFRCMConnect)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (E_XC_FRCMW_CONNETC_MAIN_SCM == eFRCMConnect || E_XC_FRCMW_CONNETC_MAIN_FRCM == eFRCMConnect)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_07_L, eFRCMConnect<<0, 0x03);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_07_L, (bEnable? BIT(3):0), BIT(3));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_47_L, eFRCMConnect<<0, 0x03);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_47_L, (bEnable? BIT(3):0), BIT(3));
    }
#endif
}
static MS_BOOL _MDrv_SC_check_2p_mode(void *pInstance, MS_BOOL bEnable,SCALER_WIN eWindow)
{
#if ((HW_DESIGN_4K2K_VER == 4)||(HW_DESIGN_4K2K_VER == 6))
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U16 u16Width, u16Height = 0;

    MS_U32 u32Vfreqx10 = 0;

    MS_BOOL bstatus = FALSE;
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
    if(psXCInstPri->u32DeviceID == 0x00)
#endif
    {
        if(eWindow == MAIN_WINDOW)
        {
        #if 0
            if(!MDrv_XC_Is_SupportSWDS(pInstance))
            {
                u16Width   = gSrcInfo[eWindow].stCapWin.width;
                u16Height  = gSrcInfo[eWindow].stCapWin.height;
            }
            else
            {
                u16Width   = gSrcInfo[eWindow].Status2.u16VirtualBox_Width;
                u16Height  = gSrcInfo[eWindow].Status2.u16VirtualBox_Height;
            }
        #endif

            if(MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) )
            {
                u16Width   = gSrcInfo[eWindow].Status2.u16VirtualBox_Width;
                u16Height  = gSrcInfo[eWindow].Status2.u16VirtualBox_Height;
            }
            else
            {
                u16Width   = gSrcInfo[eWindow].stCapWin.width;
                u16Height  = gSrcInfo[eWindow].stCapWin.height;
            }
            if(gSrcInfo[eWindow].bInterlace)
            {
                u32Vfreqx10 = gSrcInfo[eWindow].u16InputVFreq/2; //Change field rate to frame rate
            }
            else
            {
                u32Vfreqx10 = gSrcInfo[eWindow].u16InputVFreq;
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u16Width=%u,u16Height=%u,u32Vfreqx10=%tu\n",u16Width, u16Height,(ptrdiff_t)u32Vfreqx10);

#if (HW_DESIGN_4K2K_VER == 4)
            if((u16Width >= 0xF00)&&(u16Height >= 0x870)&&(u32Vfreqx10 >= 330)&&
            ((gSrcInfo[eWindow].enInputSourceType == INPUT_SOURCE_STORAGE)||(gSrcInfo[eWindow].enInputSourceType == INPUT_SOURCE_STORAGE2)
                      ||(gSrcInfo[eWindow].enInputSourceType == INPUT_SOURCE_DTV)||(gSrcInfo[eWindow].enInputSourceType == INPUT_SOURCE_DTV2)))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15), BIT(15));
            }
            else if((u16Width >= 0xF00)&&(u16Height >= 0x870)&&(u32Vfreqx10 >= 230) && bEnable)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15), BIT(15));
            }
            //monaco mm DS VB size always be 4k2k , mvop timing is 4k2k ,  Vfreqx10 >= 330 -> 2p mode
            else if ( (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) ) &&
                       (gSrcInfo[eWindow].Status2.u16VirtualBox_Width >= 0xF00) && (gSrcInfo[eWindow].Status2.u16VirtualBox_Height>= 0x870)&&(u32Vfreqx10 >= 330) )
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15), BIT(15));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x00, BIT(15));
            }
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, bEnable? BIT(15):0x00, BIT(15));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, bEnable? (BIT(1)|BIT(0)):0x00, BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode
#endif

#if (HW_DESIGN_4K2K_VER == 6)

        if((psXCInstPri->u32DeviceID == E_XC_DEVICE1)&&(((MS_U32)(u16Width)*(MS_U32)(u16Height)*(u32Vfreqx10/10)) >= MAX_1P_MODE_CLK))//ip clk >= 300Mhz, SC1 will be setted avg mode
        {
            Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,FALSE,eWindow);
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x0 , BIT(15));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x0 , BIT(15));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(14), BIT(14));
            gSrcInfo[eWindow].stCapWin.width = gSrcInfo[eWindow].stCapWin.width/2;
            if(gSrcInfo[eWindow].Status2.bPreHCusScaling == TRUE)
            {
                gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc = gSrcInfo[eWindow].Status2.u16PreHCusScalingSrc/2;
            }
            return TRUE;
        }

        MS_U32 u32InputCLK = 0;

        if (IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType))
            u32InputCLK = (MS_U32)(u16Width+MVOP_H_BLANKING)*(MS_U32)(u16Height+MVOP_V_BLANKING)*(u32Vfreqx10/10);
        else
            u32InputCLK = (MS_U32)(u16Width)*(MS_U32)(u16Height)*(u32Vfreqx10/10);

        if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType) &&bEnable)//HDMI always 2p mode
        {
            if((u16Width >= 0xF00)&&(u16Height >= 0x870)&&(u32Vfreqx10 >= 230))//real 2p mode
            {
#ifdef UFO_XC_FB_LEVEL
                if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
                     || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI)
                )
#else
                if(gSrcInfo[eWindow].bR_FBL)
#endif
                {
                    Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,TRUE,eWindow);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
                }
                else
                {
                    Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,FALSE,eWindow);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
                }
            }
            else
            {
                Hal_SC_Set_2pmode(pInstance,TRUE,FALSE,FALSE,eWindow);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
            }
        }
        else if(u32InputCLK >= MAX_1P_MODE_CLK)//ip clk >= 300Mhz,will be setted 2p mode.op clk>= ip clk,will be also setted  2p mode.
        {
#ifdef UFO_XC_FB_LEVEL
            if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
                 || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI)
            )
#else
            if(gSrcInfo[eWindow].bR_FBL)
#endif
            {
                Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,TRUE,eWindow);//ip,reg_opm_2p_mode,reg_bypass_all_2p_f2
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
            }
            else
            {
                Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,FALSE,eWindow);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
            }
        }
        else
        {
            if ((u16Width*u16Height*60 >= MAX_1P_MODE_CLK)) // 60 => MAX opm frame rate
            {
#ifdef UFO_XC_FB_LEVEL
                if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
                     || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI)
                )
#else
                if(gSrcInfo[eWindow].bR_FBL)
#endif
                {
                    if(gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)//HSK mdoe need always enable 2p mode
                    {
                        Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,TRUE,eWindow);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
                    }
                    else
                    {
                        Hal_SC_Set_2pmode(pInstance,FALSE,TRUE,TRUE,eWindow);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x0, BIT(15));
                    }
                }
                else
                {
                    if(gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)//HSK mdoe need always enable 2p mode
                    {
                        Hal_SC_Set_2pmode(pInstance,TRUE,TRUE,FALSE,eWindow);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
                    }
                    else
                    {
                        Hal_SC_Set_2pmode(pInstance,FALSE,TRUE,FALSE,eWindow);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x0, BIT(15));
                    }
                }
            }
            else
            {
                if(gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)//HSK mdoe need always enable 2p mode
                {
                    Hal_SC_Set_2pmode(pInstance,TRUE,FALSE,FALSE,eWindow);
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15));
                }
                else
                {
                    //enable op 2p mode will bypass uc.
                    //2k2k,can not enable op 2p mode under 2d,because we are not sure whether to enable uc in the future.
                    //4k1k always need enable op 2p mode.it will not enable uc.
                    if(IS_INPUT_4K1K(eWindow) || (IS_INPUT_2K2K(eWindow) &&
                        (E_XC_3D_INPUT_MODE_NONE != MDrv_XC_Get_3D_Input_Mode(pInstance, eWindow)) ))
                    {
                        Hal_SC_Set_2pmode(pInstance,FALSE,TRUE,FALSE,eWindow);
                        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x0, BIT(15));
                    }
                    else
                    {
                    	Hal_SC_Set_2pmode(pInstance,FALSE,FALSE,FALSE,eWindow);
                    	SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x0, BIT(15));
                    }
                }
            }
        }
#endif
            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, bEnable? BIT(15):0x00, BIT(15));
            //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, bEnable? (BIT(1)|BIT(0)):0x00, BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode

            bstatus = TRUE;
        }
        else if(eWindow == SUB_WINDOW)
        {
            u16Width   = gSrcInfo[eWindow].stCapWin.width;
            u16Height  = gSrcInfo[eWindow].stCapWin.height;
            u32Vfreqx10 = gSrcInfo[eWindow].u16InputVFreq;

            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"u16Width=%u,u16Height=%u,u32Vfreqx10=%tu\n",u16Width, u16Height,(ptrdiff_t)u32Vfreqx10);

            if((u16Width >= 0xF00)&&(u16Height >= 0x870)&&(u32Vfreqx10 >= 330))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, BIT(15), BIT(15));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_07_L, 0x00, BIT(15));
            }

            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK04_0A_L, bEnable?BIT(15):0x00, BIT(15));
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_61_L, bEnable? (BIT(1)|BIT(0)):0x00, BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode

            if(bEnable)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_02_L, BIT(14), BIT(14));  // patch
            }
            else
            {
#ifdef UFO_XC_FB_LEVEL
                if(gSrcInfo[SUB_WINDOW].eFBLevel == E_XC_FB_LEVEL_FB)//FIXME
#else
                if(!gSrcInfo[SUB_WINDOW].bFBL)
#endif
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK42_02_L, 0x00, BIT(14));
                }
            }

            bstatus = TRUE;
        }
        else
        {
            printf("Please check the MAX window number!!\n");
        }
    }

#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 1)

    //Subwindow converts hdmi422 to hdmi444 only at HDMIRX or IP1
    //If Dolby HDR is supported, HDMIRx would bypass the 422to444 convert.
    //then scaler should take the responsibility to convert 422 to 444.
    if( (psXCInstPri->u32DeviceID == 0x1) && (eWindow == MAIN_WINDOW))
    {
        if(!(R2BYTE(REG_PM_EFUSE_08_L)&BIT(6))) //Detect if support DolbyHDR from efuse.
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, (BIT(0)|BIT(1)),(BIT(0)|BIT(1))); //enable 422 to 444 convert
        }
    }
#endif

    return bstatus;
#elif (HW_DESIGN_4K2K_VER == 7)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(psXCInstPri->u32DeviceID == 0x00)
    {
        if(eWindow == MAIN_WINDOW)
        {
            if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType) && bEnable)//HDMI always 2p mode
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, BIT(0), BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, 0x0000, BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
        }
        else if(eWindow == SUB_WINDOW)
        {
            if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType) && bEnable)//HDMI always 2p mode
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_61_L, BIT(0), BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK03_61_L, 0x0000, BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
        }
    }
    else if(psXCInstPri->u32DeviceID == 0x01)
    {
        if(eWindow == MAIN_WINDOW)
        {
            MS_U16 u16Width, u16Height = 0;
            MS_U32 u32Vfreqx10 = 0;

            if(!MDrv_XC_Is_SupportSWDS(pInstance))
            {
                u16Width   = gSrcInfo[eWindow].stCapWin.width;
                u16Height  = gSrcInfo[eWindow].stCapWin.height;
            }
            else
            {
                u16Width   = gSrcInfo[eWindow].Status2.u16VirtualBox_Width;
                u16Height  = gSrcInfo[eWindow].Status2.u16VirtualBox_Height;
            }

            u32Vfreqx10 = gSrcInfo[eWindow].u16InputVFreq;

            if(IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType))//HDMI always 2p mode
            {
                if((u16Width >= 0xF00)&&(u16Height >= 0x870)&&(u32Vfreqx10 >= 330))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(14), BIT(15)|BIT(14));
                }
                else
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15)|BIT(14));
                }
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15), BIT(15));

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, BIT(0), BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
            else if(IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeStorage(gSrcInfo[eWindow].enInputSourceType)) // MVOP always 2p mode
            {
                if((u16Width >= 0xF00)&&(u16Height >= 0x870)&&(u32Vfreqx10 >= 330))
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(14), BIT(15)|BIT(14));
                }
                else
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, BIT(15), BIT(15)|BIT(14));
                }
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, BIT(15), BIT(15));

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, 0x0000, BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK02_0A_L, 0x0000, BIT(15)|BIT(14));
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_07_L, 0x0000, BIT(15));

                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_61_L, 0x0000, BIT(2)|BIT(1)|BIT(0));  // [0] 2p 422 [1] 2p 422 auto mode [2] 2p 422 manual value
            }
        }
    }
    return TRUE;
#else
    return 0;
#endif
}

void MDrv_SC_set_2p_mode(void *pInstance, XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(((pstXC_SetWin_Info->enInputSourceType >= INPUT_SOURCE_HDMI)&&(pstXC_SetWin_Info->enInputSourceType <= INPUT_SOURCE_HDMI_MAX))||
       ((pstXC_SetWin_Info->enInputSourceType >= INPUT_SOURCE_DVI)&&(pstXC_SetWin_Info->enInputSourceType <= INPUT_SOURCE_DVI_MAX)))
    {
#if SUPPORT_HDMI20
        if(Hal_HDMI_CheckHDMI20_Setting(MDrv_XC_Mux_GetHDMIPort(pInstance, pstXC_SetWin_Info->enInputSourceType)))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"HDMI is 2P mode\n");

            //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK40_03_L,0x0000); // [7] reg_spt_2p_oe_en [0] reg_spt_420_en

            if(!_MDrv_SC_check_2p_mode(pInstance,ENABLE,eWindow))
                printf("Please check the Scaler ID and eWindow\n");
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"HDMI is 1P mode\n");

///SUPPORT_HDMI20_420
            /// patch for HDMI2.0 4K2K@420, It should not support P2P mode
            if((pstXC_SetWin_Info->stCapWin.width >= 0x780)
             &&(pstXC_SetWin_Info->stCapWin.height >= 0x870)
             &&(pstXC_SetWin_Info->u16InputVFreq >= 490))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"HDMI is 4K 420 \n");
                if(!_MDrv_SC_check_2p_mode(pInstance,ENABLE,eWindow))
                    printf("Please check the Scaler ID and eWindow\n");
            }
            else
            {
            if(!_MDrv_SC_check_2p_mode(pInstance,DISABLE,eWindow))
                printf("Please check the Scaler ID and eWindow\n");
            }
        }
#endif
    }
    else if((pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_STORAGE)||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_STORAGE2)
          ||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_DTV)||(pstXC_SetWin_Info->enInputSourceType == INPUT_SOURCE_DTV2))
    {
        if((pstXC_SetWin_Info->stCapWin.width >= 0xF00)
         &&(pstXC_SetWin_Info->stCapWin.height >= 0x870)
         &&(pstXC_SetWin_Info->u16InputVFreq >= 330))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"MVOP is 2P mode\n");

            if(!_MDrv_SC_check_2p_mode(pInstance,ENABLE,eWindow))
                printf("Please check the Scaler ID and eWindow\n");
        }
        //monaco mm DS VB size always be 4k2k , mvop timing is 4k2k , 2p mode
        else if ( (MDrv_XC_GetDynamicScalingStatus(pInstance) || MDrv_XC_Is_DSForceIndexEnabled(pInstance, eWindow) ) &&
                (gSrcInfo[eWindow].Status2.u16VirtualBox_Width >= 0xF00) && (gSrcInfo[eWindow].Status2.u16VirtualBox_Height>= 0x870)&&(pstXC_SetWin_Info->u16InputVFreq >= 330) )
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"MVOP is 2P mode\n");
            if(!_MDrv_SC_check_2p_mode(pInstance,ENABLE,eWindow))
                printf("Please check the Scaler ID and eWindow\n");
        }
        else
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"MVOP is 1P mode\n");
            if(!_MDrv_SC_check_2p_mode(pInstance,DISABLE,eWindow))
                printf("Please check the Scaler ID and eWindow\n");
        }
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Other soure is 1P mode\n");
        if(!_MDrv_SC_check_2p_mode(pInstance,DISABLE,eWindow))
            printf("Please check the Scaler ID and eWindow\n");
    }
}

void MDrv_SC_set_frcm_cbcr_swap(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (bEnable == FALSE)
    {
        if(eWindow == MAIN_WINDOW)
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_19_L, (0), BIT(0));
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_59_L, (0), BIT(0));
        }
    }
    else
    {
        // FRCM mirror: BK32_03[12](main), BK32_43[12](sub)
        // FRCM cbcy swap: BK31_19[0](main),BK31_59[0](sub)
        // Bypass CE(compression): BK33_01[0](main),BK33_01[1](sub), 0: ENABLE CE; 1: disable CE
        // --------------------------------------------------------------------------------------------
        // | regs         |  not h_mirror |  h_mirror without compression | h_mirror with compression |
        // | BK32_03[12]  |       0       |              1                |              1            |
        // | BK33_01[0]   |     0 or 1    |              1                |              0            |
        // | BK31_19[0]   |       0       |              1                |              0            |
        // --------------------------------------------------------------------------------------------
        if(eWindow == MAIN_WINDOW)
        {
            if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L, BIT(0)) != 0)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_19_L, BIT(0), BIT(0));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_19_L, 0, BIT(0));
            }
        }
        else
        {
            if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK33_01_L, BIT(1)) != 0)
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_59_L, BIT(0), BIT(0));
            }
            else
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK31_59_L, 0, BIT(0));
            }
        }
    }
#endif
}

MS_BOOL MDrv_XC_MemControl_Switch_Method_By_Vcnt(void *pInstance, MS_BOOL bEnable, MS_U16 u16Vcnt)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_44_L, bEnable?BIT(15):0x0, BIT(15)); // Memory control Switch Method by Vcnt enable
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_44_L, u16Vcnt, 0x1FFF); // Memory control Switch Method by Vcnt
#endif
    return TRUE;
}

MS_BOOL MDrv_XC_edclk_pd_control(void *pInstance, MS_BOOL bEnable, MS_U16 u16ControlStart, MS_U16 u16ControlEnd)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_45_L, bEnable?BIT(15):0x0, BIT(15));  // edclk pd control enable
    if (bEnable)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_45_L, u16ControlStart, 0x1FFF);  // edclk pd control start
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_47_L, u16ControlEnd, 0x1FFF);  // edclk pd control end
    }
#endif
    return TRUE;
}


void Mdrv_SC_set_frcm_pre_align_pixel(void *pInstance, MS_BOOL bEnable, MS_U16 pixels, SCALER_WIN eWindow)
{
#if (HW_DESIGN_4K2K_VER == 4)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    UNUSED(pixels);

    if (bEnable)
    {
        if ( eWindow == MAIN_WINDOW )
        {
            // Enable frcm pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_2A_L, BIT(15), BIT(15));
        }
        else
        {
            // Enable frcm pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_6A_L, BIT(15), BIT(15));
        }

    }
    else
    {
        if ( eWindow == MAIN_WINDOW )
        {
            // Disable frcm pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_2A_L, 0x00 , BIT(15));
        }
        else
        {
            // Disable frcm pre align pixel for mirror mode.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK48_6A_L, 0x00 , BIT(15));
        }
    }
#endif
}

#ifdef SUPPORT_BWD
MS_BOOL MDrv_SC_setBwdConfig(void *pInstance, E_XC_BWD_CONFIG_TYPE eType, void *pstParam, SCALER_WIN eWindow)
{
    MS_BOOL bResult = FALSE;
    switch (eType)
    {
        case E_XC_BWD_UPDATE_BWR_CONFIG:
        {
            if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetSkipWaitVsync(pInstance, eWindow)))
            {
                _MLOAD_ENTRY(pInstance);
                bResult = HAL_SC_set_bwr_config_burst(pInstance, pstParam, eWindow);
                _MLOAD_RETURN(pInstance);
            }
            else
            {
                bResult = HAL_SC_set_bwr_config(pInstance, pstParam, eWindow);
            }
            break;
        }
        default:
            bResult = FALSE;
            break;
    }
    return bResult;
}
#endif


void MDrv_XC_SetIsSupport2StepScalingFlag(MS_BOOL bEnable)
{
#ifdef UFO_XC_SUPPORT_2STEP_SCALING
    bIsSupport2StepScaling = bEnable;
#endif
}

MS_BOOL MDrv_XC_IsSupport2StepScaling(void)
{
#ifdef UFO_XC_SUPPORT_2STEP_SCALING
    return bIsSupport2StepScaling;
#else
    return FALSE;
#endif
}

MS_BOOL MDrv_XC_GetPQPathStatus(void* pInstance, E_XC_PQ_Path_Type ePqPathType, MS_U16 u16Width, MS_U16 u16Height)
{
#ifdef UFO_XC_PQ_PATH
    return MHal_XC_GetPQPathStatus(pInstance, ePqPathType, u16Width, u16Height);
#else
    return FALSE;
#endif
}
