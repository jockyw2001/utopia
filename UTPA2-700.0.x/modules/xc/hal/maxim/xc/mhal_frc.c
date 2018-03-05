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
// [mhal_frc.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MHAL_FRC_C

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <asm/div64.h>
#else
#include <string.h>
#define do_div(x,y) ((x)/=(y))
#endif

// Common Definition
#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#include "mhal_xc_chip_config.h"
#include "utopia_dapi.h"
// Registers
#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_adc_atop.h"
#include "hwreg_adc_dtop.h"
//#include "hwreg_chiptop.h"
#include "hwreg_hdmi.h"
#include "hwreg_pm_sleep.h"//alex_tung
#include "hwreg_ipmux.h"

#include "apiXC.h"

#include "xc_Analog_Reg.h"

#include "mhal_mux.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_ADC_Internal.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "mhal_adc.h"
#include "drvXC_HDMI_if.h"
#include "drvXC_HDMI_Internal.h"
#include "mhal_sc.h"
#include "mhal_hdmi.h"

#include "mdrv_frc.h"
#include "mhal_frc.h"
#include "drvMBX.h"
#include "apiMBX.h"
//#include "MHal_FRC_4K2K_QualityMap.h"
#include "Maxim_2D_4K2K.c"
#include "Maxim_2D_FHD.c"

#include "Maxim_FRC_PAS_4K2K_60.c"
#include "Maxim_FRC_PAS_4K2K_120.c"
#include "Maxim_FRC_ACT_4K2K_120.c"
#include "Maxim_FRC_ACT_4K1K_120.c"
#include "Maxim_FRC_ACT_4K0_5K_LLRR_240.c"
#include "Maxim_FRC_ACT_4K1K_LLRR_240.c"

#include "XC_private.h"
#include "halCHIP.h"
#include "mdrv_sc_dynamicscaling.h"
#include "mhal_dynamicscaling.h"

//--------------------------------------------------------------------------------------------------
//  Macro definition
//--------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_U16 g_u16LastOffsetH = 0;
MS_U16 g_u16LastOffsetV = 0;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#define _u16FRCDbgSwitch 1

#define FRC_DBG_MSG(fmt,...)                                                                 \
    MS_DEBUG_MSG(do{                                                                                      \
        if(_u16FRCDbgSwitch){                                                                   \
           printf( "[PNL_DBG_MSG]: %s: %d \n"  fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__); \
          }                                                                                  \
      } while(0))

#define FRC_PRINT_VAR(var)                                                                   \
    do{                                                                                      \
        if(_u16FRCDbgSwitch){                                                                   \
          FRC_DBG_MSG("%30s: %d\n", #var, (var));                                            \
          }                                                                                  \
      }while(0)



//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
//static MS_FRC_RGBGAINOFFSET_INFO stGainOffsetSetting;
#ifdef PATCH_TCON_BRING_UP
        const char* MDrv_PNL_GetName(void);
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Exernal
//------------------------------------------------------------------------------


//======================================================================
// FRC TGEN
//======================================================================

void MHal_FRC_TGEN_Init(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    #if 1 // for mantis 0915469: MM zoom display window position wrong, need wait for DS ready
    MHal_FRC_TGEN_Enable_Lock_Source(pInstance, ENABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, FALSE);
    #endif

    //bk68_20[0]: reg_tg_lock_en: 0: free run, 1: lock mode
    //             [1]: reg_tg_lock_src_sel_usr_en
    //             [2]: reg_tg_lock_src_sel_usr: 1: from origin TGEN
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_20_L, (BIT(0)|BIT(1)|BIT(2)), (BIT(0)|BIT(1)|BIT(2)));
    //MHal_FRC_TGEN_Enable_LockMode(pInstance, ENABLE);
    //MHal_FRC_TGEN_Enable_Source_Select_Mode(pInstance, ENABLE);
    //MHal_FRC_TGEN_Set_Lock_Source(pInstance, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN);

    //bk68_2F[0]: reg_vop2_stgen_dummy:  0:reg_ckg_odclk_2_mode, 1: do double buffer
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_2F_L, BIT(0), BIT(0));


}

void MHal_FRC_TGEN_DoubleBuf(void *pInstance, MS_BOOL bEnDis)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_23_L, bEnDis?BIT(0):0x00, BIT(0));
}

void MHal_FRC_TGEN_SetVTotal(void *pInstance, MS_U16 u16VTotal)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //BK68_0F [12:0]:reg_v_total
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_0F_L, (u16VTotal -1), 0x1FFF);
}

void MHal_FRC_TGEN_SetHTotal(void *pInstance, MS_U16 u16HTotal)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //BK68_07 [13:0]:reg_h_total
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_07_L, (u16HTotal -1), 0x3FFF);
}

void MHal_FRC_TGEN_SetVSyncStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16Start += VSYNCSTART_OFFSET;
    u16End += VSYNCSTART_OFFSET;

    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        &&(!MHal_FRC_IsFHDToFSC(pInstance,MAIN_WINDOW))
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange == 0)
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange == 0)) //Check if PIXEL FEATURE is ENABLE or NOT.
    {
        u16Start += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
        u16End += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
    }

    // Erorr handling: there is line garbage at video bottom side, ex. VGA 800x600 case
    // (Vsync position - V FDE end)  should be > 10
    MS_U16 u16VFDE_Start = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0B_L);
    MS_U16 u16VFDE_End = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0C_L);
    MS_U16 u16VTotal = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0F_L);
    MS_U16 u16VsyncSize = u16End - u16Start;

    if (((u16Start > u16VFDE_End) && ((u16Start - u16VFDE_End) < VSYNC_FRONT_PORCH))
        ||(u16Start <= u16VFDE_End)
       )
    {
        u16Start = u16VFDE_End + VSYNC_FRONT_PORCH;
        u16End = u16Start + u16VsyncSize;
    }

    // Error handling : Vsync start/end is out of Vtotal
    if(u16Start > u16VTotal)
    {
        u16Start -= u16VTotal;
        u16End -= u16VTotal;
    }
    else if((u16Start < u16VTotal)&&(u16End > u16VTotal))
    {
        u16End = u16VFDE_Start - 1;
        u16Start = u16End - u16VsyncSize;
    }

#ifdef PATCH_TCON_BRING_UP
        if((((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type) == E_XC_PNL_LPLL_EPI28_12P)
          && !(strcmp(panelName_UD_VB1_8LANE_DRDEPI,MDrv_PNL_GetName())))
        {
            u16Start = FRC_TGEN_VSYNC_START_FOR_UD_VB1_8LANE_DRDEPI;
            u16End = FRC_TGEN_VSYNC_END_FOR_UD_VB1_8LANE_DRDEPI;
        }
#endif

    //BK68_09 [12:0]:reg_vs_yst
    //Bk68_0A [12:0]:reg_vs_yend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_09_L, u16Start, 0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_0A_L, u16End, 0x1FFF);
}

void MHal_FRC_TGEN_SetHSyncStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //BK68_01 [13:0]:reg_hs_xst
    //BK68_02 [13:0]:reg_hs_xend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_01_L, u16Start, 0x3FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_02_L, u16End, 0x3FFF);
}

void MHal_FRC_TGEN_SetFdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16Start += VSTART_OFFSET;
    u16End += VSTART_OFFSET;

    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        &&(!MHal_FRC_IsFHDToFSC(pInstance,MAIN_WINDOW))
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange == 0)
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange == 0)) //Check if PIXEL FEATURE is ENABLE or NOT.
    {
        u16Start += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
        u16End += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
    }

    //BK68_0B [12:0]:reg_f_de_yst
    //BK68_0C [12:0]:reg_f_de_yend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_0B_L, u16Start, 0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_0C_L, u16End, 0x1FFF);
}

void MHal_FRC_TGEN_SetMdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16Start += VSTART_OFFSET;
    u16End += VSTART_OFFSET;

    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        &&(!MHal_FRC_IsFHDToFSC(pInstance,MAIN_WINDOW))
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange == 0)
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange == 0)) //Check if PIXEL FEATURE is ENABLE or NOT.
    {
        u16Start += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
        u16End += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
    }
    //BK68_0D [12:0]:reg_m_de_yst
    //BK68_0E [12:0]:reg_m_de_yend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_0D_L, u16Start, 0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_0E_L, u16End, 0x1FFF);
}

void MHal_FRC_TGEN_SetMdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //BK68_05 [13:0]:reg_m_de_xst
    //BK68_06 [13:0]:reg_m_de_xend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_05_L, u16Start, 0x3FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_06_L, u16End, 0x3FFF);
}

void MHal_FRC_TGEN_SetFdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //BK68_03 [13:0]:reg_f_de_xst
    //BK68_04 [13:0]:reg_f_de_xend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_03_L, u16Start, 0x3FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_04_L, u16End, 0x3FFF);
}

void MHal_FRC_TGEN_SetSubMdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    u16Start += VSTART_OFFSET;
    u16End += VSTART_OFFSET;

    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        &&(!MHal_FRC_IsFHDToFSC(pInstance,MAIN_WINDOW))
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange == 0)
        &&(pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange == 0)) //Check if PIXEL FEATURE is ENABLE or NOT.
    {
        u16Start += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
        u16End += (FRC_PIPE_DELAY_VCNT_FSC_FHD - FRC_PIPE_DELAY_VCNT_FSC_4K);
    }

    //BK68_0D [12:0]:reg_m_de_yst
    //BK68_0E [12:0]:reg_m_de_yend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_12_L, u16Start, 0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_13_L, u16End, 0x1FFF);
}

void MHal_FRC_TGEN_SetSubMdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    //BK68_05 [13:0]:reg_m_de_xst
    //BK68_06 [13:0]:reg_m_de_xend
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_10_L, u16Start, 0x3FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_11_L, u16End, 0x3FFF);
}

void MHal_SWDS_AddTGENMdeStartEndY(void *pInstance, MS_U16 u16Start, MS_U16 u16End, SCALER_WIN eWindow)
{
    u16Start += VSTART_OFFSET;
    u16End += VSTART_OFFSET;

    if (SUPPORT_3D_DS && (g_bSWDSGenScenario == TRUE))
    {
        if (eWindow == MAIN_WINDOW)
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_0D_L, u16Start, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_0E_L, u16End, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
        }
        else
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_12_L, u16Start, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_13_L, u16End, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
        }
    }
}

void MHal_SWDS_AddTGENMdeStartEndX(void *pInstance, MS_U16 u16Start, MS_U16 u16End, SCALER_WIN eWindow)
{
    if (SUPPORT_3D_DS && (g_bSWDSGenScenario == TRUE))
    {
        if (eWindow == MAIN_WINDOW)
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_05_L, u16Start, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_06_L, u16End, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
        }
        else
        {
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_10_L, u16Start, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
            Hal_SC_WriteSWDSCommand(pInstance, MAIN_WINDOW, REG_SC_BK68_11_L, u16End, DS_OP,DS_XC,&PSTXC_DS_CMDCNT);
        }
    }
}

void MHal_FRC_TGEN_Enable_LockMode(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_20_L, bEn?BIT(0):0x00, BIT(0));
}

void MHal_FRC_TGEN_Enable_Source_Select_Mode(void *pInstance, MS_BOOL bEn)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_20_L, bEn?BIT(1):0x00, BIT(1));
}

#if 0
void MHal_FRC_TGEN_Set_Lock_Source(void *pInstance, E_FRC_TGEN_LOCK_SOURCE_MODE eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_20_L, eMode<<2, BIT(2));
}
#endif

void MHal_FRC_TGEN_Enable_Lock_Source(void *pInstance, MS_BOOL bEn, E_FRC_TGEN_LOCK_SOURCE_MODE eMode, MS_BOOL bMenuload)
{
    if (ENABLE == bMenuload)
    {
        MS_U16 u16STGEN_Setting = 0x00;
        if (bEn)
        {
            u16STGEN_Setting |= BIT(5);
        }

        if (E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN == eMode)
        {
            u16STGEN_Setting |= BIT(2);
        }

        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_20_L, BIT(0) | u16STGEN_Setting , (BIT(0)|BIT(2)|BIT(5))); // reg_stgen_en: use simple tgen to trigger op/vip

        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_28_L, bEn ? (BIT(0)|BIT(1)|BIT(2)) : 0x00, (BIT(0)|BIT(1)|BIT(2)));
    }
    else
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_20_L, bEn ? BIT(5): 0x00, BIT(5)); // reg_stgen_en: use simple tgen to trigger op/vip
        //bk68_28[0]: reg_opvip_ctrl_sel_usr_en: user mode of opvip control selection enable
        //             [1]: reg_opvip_ctrl_sel_usr
        //             [2]: reg_opvip_ctrl_trig_sel
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_28_L, bEn ? (BIT(0)|BIT(1)|BIT(2)) : 0x00, (BIT(0)|BIT(1)|BIT(2)));

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_20_L, BIT(0) | (eMode<<2), (BIT(0)|BIT(2)));
    }

}

void MHal_FRC_SetYTrig(void *pInstance, MS_U16 u16Trig1, MS_U16 u16Trig2)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8LPLL_Type = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;


    // FO setting
    if (IsVBY1_16LANE(u8LPLL_Type))
    {
        //Maxim not support 120Hz output (16 lane case)
    }
    else
    {
            MS_U16 u16BK10_VTotal = 0;
            MS_U16 u16DEVStart    = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L, VOP_DE_VSTART_MASK);
#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            u16BK10_VTotal = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
        }
        else
#endif
        {
            u16BK10_VTotal = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK)+1;
        }
        //MS_U16 u16VSyncStart  = (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, 0x1000) >> 1) + SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, 0x7FF);

        //after PQSNV:10729,memory format contrl by CFD,CFD will change color format as YUV except P2P mode
        //such as,bUseYUVSpace is RGB which decide from PQ,but CFD will change curret memory format as YUV.CFD will not sync to gSrcInfo[MAIN_WINDOW].bUseYUVSpace
        //so,gSrcInfo[MAIN_WINDOW].bUseYUVSpace will be inaccurate.bForceRGBin always means RGB except P2P mode.so, only bForceRGBin can be use to show curret memory format with CFD
        //if (gSrcInfo[MAIN_WINDOW].bUseYUVSpace)
        if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
        {
            if (IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) // 4K2K out
            {
                u16Trig1 = (u16BK10_VTotal + u16DEVStart - FRC_Y_TRIGGER_DELAY) ;
                u16Trig2 = (u16BK10_VTotal + u16DEVStart - FRC_Y_TRIGGER_DELAY) + 1;
            }
            else if (IS_OUTPUT_4K0_5K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) // 4K2K out
            {
                u16Trig1 = (u16BK10_VTotal + u16DEVStart - (FRC_Y_TRIGGER_DELAY/7)) ;
                u16Trig2 = (u16BK10_VTotal + u16DEVStart - (FRC_Y_TRIGGER_DELAY/7)) + 1;
            }
            else
            {
                // FHD case, FRC trig need to before DE Vstart about 32lines
                u16Trig1 = (u16BK10_VTotal + u16DEVStart - (FRC_Y_TRIGGER_DELAY/2)) ;
                u16Trig2 = (u16BK10_VTotal + u16DEVStart - (FRC_Y_TRIGGER_DELAY/2)) + 1;
            }
        }
        else
        {
            u16Trig1 = (u16BK10_VTotal + u16DEVStart - FRC_Y_TRIGGER_DELAY_RGB);
            u16Trig2 = (u16BK10_VTotal + u16DEVStart - FRC_Y_TRIGGER_DELAY_RGB) + 1;
        }

        MS_U16 u16VDEend = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, VOP_VTT_MASK);
        MS_U16 u16Pipedelay = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_51_L, 0x1FFF);

        if(u16Trig2 <= (u16VDEend+u16Pipedelay)) //error handling : if Ytrig is <= VDEend+pipedelay, put Ytrig  at VDEend+Pipedela+1.
        {
            if( u16VDEend+u16Pipedelay < u16BK10_VTotal-1)
            {
                u16Trig1 = u16VDEend+u16Pipedelay;
                u16Trig2 = u16Trig1+1;
            }
            else
            {
                u16Trig1 = u16BK10_VTotal-2;
                u16Trig2 = u16Trig1+1;
            }
        }

#ifdef PATCH_TCON_BRING_UP
            if((((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type) == E_XC_PNL_LPLL_EPI28_12P)
              && !(strcmp(panelName_UD_VB1_8LANE_DRDEPI,MDrv_PNL_GetName())))
            {
                if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
                {
                    //original value(not TCON): 60HZ : VTT 2260, Trig1 = 0x88f (2191), Trig2 = 0x890 (2192)
                    u16Trig1 = (u16BK10_VTotal - FRC_Y_TRIGGER_START_OFFSET_YUV_FOR_UD_VB1_8LANE_DRDEPI + FrcPipDelayVcntValue) + VSYNC_OFFSET_FOR_UD_VB1_8LANE_DRDEPI;
                    u16Trig2 = (u16BK10_VTotal - FRC_Y_TRIGGER_START_OFFSET_YUV_FOR_UD_VB1_8LANE_DRDEPI + FrcPipDelayVcntValue) + VSYNC_OFFSET_FOR_UD_VB1_8LANE_DRDEPI + 1;
                }
                else
                {
                    //original value(not TCON): 60HZ : VTT 2260, Trig1 = 0x88f (2191), Trig2 = 0x890 (2192)
                    u16Trig1 = (u16BK10_VTotal - FRC_Y_TRIGGER_START_OFFSET_RGB_FOR_UD_VB1_8LANE_DRDEPI) + VSYNC_OFFSET_FOR_UD_VB1_8LANE_DRDEPI;
                    u16Trig2 = (u16BK10_VTotal - FRC_Y_TRIGGER_START_OFFSET_RGB_FOR_UD_VB1_8LANE_DRDEPI) + VSYNC_OFFSET_FOR_UD_VB1_8LANE_DRDEPI + 1;
                }
            }
#endif

        if(u16BK10_VTotal != 0)
        {
            u16Trig1 %= u16BK10_VTotal;
            u16Trig2 %= u16BK10_VTotal;
        }
    }

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_65_L, u16Trig1, 0x1FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_66_L, u16Trig2, 0x1FFF);
}

//======================================================================
// FRC MIU Select
//======================================================================
static SC_MIUMASK_t MHal_FRC_get_miu0mask(void)
{
    SC_MIUMASK_t mask;

    mask.u16MiuG0Mask = MDrv_Read2Byte(MIU0_G0_REQUEST_MASK);
    mask.u16MiuG1Mask = MDrv_Read2Byte(MIU0_G1_REQUEST_MASK);
    mask.u16MiuG2Mask = MDrv_Read2Byte(MIU0_G2_REQUEST_MASK);
    mask.u16MiuG3Mask = MDrv_Read2Byte(MIU0_G3_REQUEST_MASK);
    mask.u16MiuG4Mask = MDrv_Read2Byte(MIU0_G4_REQUEST_MASK);
    mask.u16MiuG5Mask = MDrv_Read2Byte(MIU0_G5_REQUEST_MASK);
    mask.u16MiuG6Mask = MDrv_Read2Byte(MIU0_G6_REQUEST_MASK);

    return mask;
}

static SC_MIUMASK_t MHal_FRC_get_miu1mask(void)
{
    SC_MIUMASK_t mask;

    mask.u16MiuG0Mask = MDrv_Read2Byte(MIU1_G0_REQUEST_MASK);
    mask.u16MiuG1Mask = MDrv_Read2Byte(MIU1_G1_REQUEST_MASK);
    mask.u16MiuG2Mask = MDrv_Read2Byte(MIU1_G2_REQUEST_MASK);
    mask.u16MiuG3Mask = MDrv_Read2Byte(MIU1_G3_REQUEST_MASK);
    mask.u16MiuG4Mask = MDrv_Read2Byte(MIU1_G4_REQUEST_MASK);
    mask.u16MiuG5Mask = MDrv_Read2Byte(MIU1_G5_REQUEST_MASK);
    mask.u16MiuG6Mask = MDrv_Read2Byte(MIU1_G6_REQUEST_MASK);

    return mask;
}

static void MHal_FRC_set_miu0mask(SC_MIUMASK_t mask)
{
    MDrv_Write2Byte(MIU0_G0_REQUEST_MASK, mask.u16MiuG0Mask);
    MDrv_Write2Byte(MIU0_G1_REQUEST_MASK, mask.u16MiuG1Mask);
    MDrv_Write2Byte(MIU0_G2_REQUEST_MASK, mask.u16MiuG2Mask);
    MDrv_Write2Byte(MIU0_G3_REQUEST_MASK, mask.u16MiuG3Mask);
    MDrv_Write2Byte(MIU0_G4_REQUEST_MASK, mask.u16MiuG4Mask);
    MDrv_Write2Byte(MIU0_G5_REQUEST_MASK, mask.u16MiuG5Mask);
    MDrv_Write2Byte(MIU0_G6_REQUEST_MASK, mask.u16MiuG6Mask);
}

static void MHal_FRC_set_miu1mask(SC_MIUMASK_t mask)
{
    MDrv_Write2Byte(MIU1_G0_REQUEST_MASK, mask.u16MiuG0Mask);
    MDrv_Write2Byte(MIU1_G1_REQUEST_MASK, mask.u16MiuG1Mask);
    MDrv_Write2Byte(MIU1_G2_REQUEST_MASK, mask.u16MiuG2Mask);
    MDrv_Write2Byte(MIU1_G3_REQUEST_MASK, mask.u16MiuG3Mask);
    MDrv_Write2Byte(MIU1_G4_REQUEST_MASK, mask.u16MiuG4Mask);
    MDrv_Write2Byte(MIU1_G5_REQUEST_MASK, mask.u16MiuG5Mask);
    MDrv_Write2Byte(MIU1_G6_REQUEST_MASK, mask.u16MiuG6Mask);
}


void MHal_FRC_Enable_MiuMask(void *pInstance)
{
#if FRC_INSIDE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->sthal_FRC.FRC_Miu0MaskOld = MHal_FRC_get_miu0mask();
    pXCResourcePrivate->sthal_FRC.FRC_Miu1MaskOld = MHal_FRC_get_miu1mask();

    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask = pXCResourcePrivate->sthal_FRC.FRC_Miu0MaskOld;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask = pXCResourcePrivate->sthal_FRC.FRC_Miu1MaskOld;

    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG0Mask |= MIU_FRC_G0REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG1Mask |= MIU_FRC_G1REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG2Mask |= MIU_FRC_G2REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG3Mask |= MIU_FRC_G3REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG4Mask |= MIU_FRC_G4REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG5Mask |= MIU_FRC_G5REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG6Mask |= MIU_FRC_G6REQUEST_MASK;

    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG0Mask |= MIU_FRC_G0REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG1Mask |= MIU_FRC_G1REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG2Mask |= MIU_FRC_G2REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG3Mask |= MIU_FRC_G3REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG4Mask |= MIU_FRC_G4REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG5Mask |= MIU_FRC_G5REQUEST_MASK;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG6Mask |= MIU_FRC_G6REQUEST_MASK;

    MHal_FRC_set_miu0mask(pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask);
    MHal_FRC_set_miu1mask(pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask);
#endif
}

void MHal_FRC_Disable_MiuMask(void *pInstance)
{
#if FRC_INSIDE
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->sthal_FRC.FRC_Miu0MaskOld = MHal_FRC_get_miu0mask();
    pXCResourcePrivate->sthal_FRC.FRC_Miu1MaskOld = MHal_FRC_get_miu1mask();

    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask = pXCResourcePrivate->sthal_FRC.FRC_Miu0MaskOld;
    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask = pXCResourcePrivate->sthal_FRC.FRC_Miu1MaskOld;

    pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask.u16MiuG5Mask &= ~MIU_FRC_G5REQUEST_MASK;

    pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask.u16MiuG5Mask &= ~MIU_FRC_G5REQUEST_MASK;

    MHal_FRC_set_miu0mask(pXCResourcePrivate->sthal_FRC.FRC_Miu0Mask);
    MHal_FRC_set_miu1mask(pXCResourcePrivate->sthal_FRC.FRC_Miu1Mask);

#endif
}

void MHal_FRC_set_miusel(void *pInstance, MS_U8 u8MIUSel)
{
    // Scaler control MIU by itself
    // So MIU1 IP-select is set to 1. At this moment, MIU0 Miu select is not working.
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if (u8MIUSel==0)
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK40_01_L, 0x0000, 0x00C0);     // reg_miu_select_f2 to MIU0
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK40_01_L, 0x00C0, 0x00C0);     // reg_miu_select_f2 to MIU1
    }

#if 0
    if (u8MIUSel==0)
    {
        SC_W2BYTEMSK(REG_SC_BK43_05_L, 0x0000, 0x0010);     // reg_miu_select_f2 to MIU0
    }
    else
    {
        SC_W2BYTEMSK(REG_SC_BK43_05_L, 0x0010, 0x0010);     // reg_miu_select_f2 to MIU1
    }
    MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF0), 0x00, MIU_SC_G0REQUEST_MASK); // MIU select (Group0)
    MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF2), 0x00, MIU_SC_G1REQUEST_MASK); // MIU select (Group1)
    MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF4), 0x00, MIU_SC_G2REQUEST_MASK); // MIU select (Group2)
    MDrv_Write2ByteMask((REG_MIU0_BASE + 0xF6), 0x00, MIU_SC_G3REQUEST_MASK); // MIU select (Group3)

    MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF0), MIU_SC_G0REQUEST_MASK, MIU_SC_G0REQUEST_MASK); // MIU select (Group0)
    MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF2), MIU_SC_G1REQUEST_MASK, MIU_SC_G1REQUEST_MASK); // MIU select (Group1)
    MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF4), MIU_SC_G2REQUEST_MASK, MIU_SC_G2REQUEST_MASK); // MIU select (Group2)
    MDrv_Write2ByteMask((REG_MIU1_BASE + 0xF6), MIU_SC_G3REQUEST_MASK, MIU_SC_G3REQUEST_MASK); // MIU select (Group3)
#endif
}

//======================================================================
// FRC IPM
//======================================================================

void MHal_FRC_IPM_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32FrameYcout, MS_U16 u16FB_YcountLinePitch, MS_U8 u8MirrorMode )
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;
#ifdef FRC_MEMORY_PROTECT
    MS_PHY u32MaxLimitAddress =0;
#endif

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    // Note: if set address again, MIU mask toggle would causes temp garbage
    if (R4BYTE(REG_FRC_BK134_08_L) == (addr / FRC_BYTE_PER_WORD))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Same Main_FRCM_F2_Start_Addr=%tu, no need to set again, return it! \n", (ptrdiff_t)addr);
        return;
    }

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    // IPM_L miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(2)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel <<2, BIT(2));
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W4BYTE(REG_FRC_BK134_08_L, addr);
    u32MemSize = XC_FRC_IPM_L;

    #ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK30D_20_L, addr); //IPM_L memory min address
    W4BYTE(REG_FRC_BK30D_22_L, u32MaxLimitAddress); //IPM_L memory max address
    W2BYTEMSK(REG_FRC_BK30D_10_L, BIT(0),BIT(0)); //IPM_L memory address limit enable
    #endif

    #if 0
    // IPM_R miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(3)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel <<3, BIT(3));
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK134_48_L, addr);
    u32MemSize = XC_FRC_IPM_R;
    #endif

    // MEDS_L miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(0)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel, BIT(0));
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK13A_08_L, addr);
    u32MemSize = XC_FRC_MEDS_L;

#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK30D_28_L, addr); //MEDS_L memory min address
    W4BYTE(REG_FRC_BK30D_2A_L, u32MaxLimitAddress); //MEDS_L memory max address
    W2BYTEMSK(REG_FRC_BK30D_10_L, BIT(2),BIT(2)); //MEDS_L memory address limit enable
#endif

    #if 0
    // MEDS_R miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(1)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel<<1, BIT(1));
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK13A_48_L, addr);
    u32MemSize = XC_FRC_MEDS_R;
    #endif

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_L_Start_Addr=%tu\n", (ptrdiff_t)addr);
}

void MHal_FRC_IPM_R_SetBaseAddr(void *pInstance, MS_PHY addr)
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;
#ifdef FRC_MEMORY_PROTECT
    MS_PHY u32MaxLimitAddress =0;
#endif

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    // Note: if set address again, MIU mask toggle would causes temp garbage
    if (R4BYTE(REG_FRC_BK134_48_L) == (addr / FRC_BYTE_PER_WORD))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Same Main_FRCM_F2_Start_Addr=%tu, no need to set again, return it! \n", (ptrdiff_t)addr);
        return;
    }

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    #if 0
    // IPM_L miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(2)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel <<2, BIT(2));
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W4BYTE(REG_FRC_BK134_08_L, addr);
    u32MemSize = XC_FRC_IPM_L;
    #endif

    // IPM_R miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(3)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel <<3, BIT(3));
    addr = addr / FRC_BYTE_PER_WORD;
    W4BYTE(REG_FRC_BK134_48_L, addr);
    u32MemSize = XC_FRC_IPM_R;

#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK30D_24_L, addr); //IPM_R memory min address
    W4BYTE(REG_FRC_BK30D_26_L, u32MaxLimitAddress); //IPM_R memory max address
    W2BYTEMSK(REG_FRC_BK30D_10_L, BIT(1),BIT(1)); //IPM_R memory address limit enable
#endif

    #if 0
    // MEDS_L miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(0)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel, BIT(0));
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK13A_08_L, addr);
    u32MemSize = XC_FRC_MEDS_L;
    #endif

    // MEDS_R miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, BIT(1)); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel<<1, BIT(1));
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK13A_48_L, addr);
    u32MemSize = XC_FRC_MEDS_R;

#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK30D_2C_L, addr); //MEDS_R memory min address
    W4BYTE(REG_FRC_BK30D_2E_L, u32MaxLimitAddress); //MEDS_R memory max address
    W2BYTEMSK(REG_FRC_BK30D_10_L, BIT(3),BIT(3)); //MEDS_R memory address limit enable
#endif

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_R_Start_Addr=%tu\n", (ptrdiff_t)addr);
}

void MHal_FRC_OPM_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32FrameYcout)
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    // Note: if set address again, MIU mask toggle would causes temp garbage
    if (R4BYTE(REG_FRC_BK34_10_L) == (addr / FRC_BYTE_PER_WORD))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Same Main_FRCM_F2_Start_Addr=%tu, no need to set again, return it! \n", (ptrdiff_t)addr);
        return;
    }

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    // OPM_L miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(4)|BIT(6)|BIT(8))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(4)|BIT(6)|BIT(8)):0x00, (BIT(4)|BIT(6)|BIT(8)));
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W4BYTE(REG_FRC_BK134_10_L, addr);
    W4BYTE(REG_FRC_BK13A_10_L, addr);
    W4BYTE(REG_FRC_BK13C_10_L, addr);
    u32MemSize = XC_FRC_IPM_L;

    #if 0
    // OPM_R miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(5)|BIT(7)|BIT(9))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(5)|BIT(7)|BIT(9)):0x00, (BIT(5)|BIT(7)|BIT(9)));
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK136_10_L, addr);
    W4BYTE(REG_FRC_BK13B_10_L, addr);
    W4BYTE(REG_FRC_BK13D_10_L, addr);
    u32MemSize = XC_FRC_IPM_R;
    #endif

    // MEDS_L addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK134_4A_L, addr);
    u32MemSize = XC_FRC_MEDS_L;

    #if 0
    // MEDS_R addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK134_4C_L, addr);
    u32MemSize = XC_FRC_MEDS_R;
    #endif

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_L_Start_Addr=%tu\n", (ptrdiff_t)addr);
}

void MHal_FRC_OPM_R_SetBaseAddr(void *pInstance, MS_PHY addr)
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    // Note: if set address again, MIU mask toggle would causes temp garbage
    if (R4BYTE(REG_FRC_BK36_10_L) == (addr / FRC_BYTE_PER_WORD))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Same Main_FRCM_F2_Start_Addr=%tu, no need to set again, return it! \n", (ptrdiff_t)addr);
        return;
    }

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    #if 0
    // OPM_L miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(4)|BIT(6)|BIT(8))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(4)|BIT(6)|BIT(8)):0x00, (BIT(4)|BIT(6)|BIT(8)));
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W4BYTE(REG_FRC_BK134_10_L, addr);
    W4BYTE(REG_FRC_BK13A_10_L, addr);
    W4BYTE(REG_FRC_BK13C_10_L, addr);
    u32MemSize = XC_FRC_IPM_L;
    #endif

    // OPM_R miu_sel and addr
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(5)|BIT(7)|BIT(9))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(5)|BIT(7)|BIT(9)):0x00, (BIT(5)|BIT(7)|BIT(9)));
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W4BYTE(REG_FRC_BK136_10_L, addr);
    W4BYTE(REG_FRC_BK13B_10_L, addr);
    W4BYTE(REG_FRC_BK13D_10_L, addr);
    u32MemSize = XC_FRC_IPM_R;

    #if 0
    // MEDS_L addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK134_4A_L, addr);
    u32MemSize = XC_FRC_MEDS_L;
    #endif

    // MEDS_R addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W4BYTE(REG_FRC_BK134_4C_L, addr);
    u32MemSize = XC_FRC_MEDS_R;

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_R_Start_Addr=%tu\n", (ptrdiff_t)addr);
}

void MHal_FRC_ME1_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32Size)
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;
#ifdef FRC_MEMORY_PROTECT
    MS_PHY u32MaxLimitAddress =0;
#endif

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    // ME1 miu_sel
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(10)|BIT(11))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(10)|BIT(11)):0x00, (BIT(10)|BIT(11)));

    // ME1_X1 addr
    // ME1_X1 has a read from end rule, would read memory before ME1_X1 start address, causes MIU hit
    // so ME1_X1 start address needs to add XC_FRC_ME1_X1_READ size
    addr = (addr/FRC_BYTE_PER_WORD) + (XC_FRC_ME1_X1_READ/FRC_BYTE_PER_WORD); // unit: 1/256bit
    W2BYTE(REG_FRC_BK23A_3B_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK23A_3A_L, (addr >> 16), 0x07FF);
    u32MemSize = XC_FRC_ME1_X1;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_2E_L, (addr - (XC_FRC_ME1_X1_READ/FRC_BYTE_PER_WORD))); //ME1_X1 memory min address
    W4BYTE(REG_FRC_BK119_30_L, u32MaxLimitAddress); //ME1_X1 memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(3),BIT(3)); //ME1_X1 memory address limit enable
#endif

    // ME1_S1 addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W2BYTE(REG_FRC_BK233_44_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK233_43_L, (addr >> 16), 0x07FF);
    u32MemSize = XC_FRC_ME1_S1;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_32_L, addr); //ME1_S1 memory min address
    W4BYTE(REG_FRC_BK119_34_L, u32MaxLimitAddress); //ME1_S1 memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(4),BIT(4)); //ME1_S1 memory address limit enable
#endif

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_L_Start_Addr=%tu\n", (ptrdiff_t)addr);
}
void MHal_FRC_ME2_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32Size)
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;
#ifdef FRC_MEMORY_PROTECT
    MS_PHY u32MaxLimitAddress =0;
#endif

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    // ME2 miu_sel
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(10)|BIT(11)|BIT(14))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(10)|BIT(11)|BIT(14)):0x00, (BIT(10)|BIT(11)|BIT(14)));

    // ME2_X2 addr
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W2BYTE(REG_FRC_BK23A_3F_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK23A_3E_L, (addr >> 16), 0x07FF);
    u32MemSize = XC_FRC_ME2_X2;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_36_L, addr); //ME2_X2 memory min address
    W4BYTE(REG_FRC_BK119_38_L, u32MaxLimitAddress); //ME2_X2 memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(5),BIT(5)); //ME2_X2 memory address limit enable
#endif

    // ME2_Y2 addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W2BYTE(REG_FRC_BK23A_1A_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK23A_19_L, (addr >> 16), 0x07FF);
    u32MemSize = XC_FRC_ME2_Y2;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_3A_L, addr); //ME2_Y2 memory min address
    W4BYTE(REG_FRC_BK119_3C_L, u32MaxLimitAddress); //ME2_Y2 memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(6),BIT(6)); //ME2_Y2 memory address limit enable
#endif

    // ME2_F2 addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W2BYTE(REG_FRC_BK23A_1C_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK23A_1B_L, (addr >> 16), 0x07FF);
    u32MemSize = XC_FRC_ME2_F2;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_3E_L, addr); //ME2_F2 memory min address
    W4BYTE(REG_FRC_BK119_40_L, u32MaxLimitAddress); //ME2_F2 memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(7),BIT(7)); //ME2_F2 memory address limit enable
#endif

    // ME2_LOGO addr
    W2BYTEMSK(REG_FRC_BK218_74_L, BIT(0), BIT(0)); // SW patch for HW bug
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W2BYTE(REG_FRC_BK218_72_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK218_73_L, (addr >> 16), 0x07FF);
    u32MemSize = XC_FRC_ME2_LOGO;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_42_L, addr); //ME2_LOGO memory min address
    W4BYTE(REG_FRC_BK119_44_L, u32MaxLimitAddress); //ME2_LOGO memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(8),BIT(8)); //ME2_LOGO memory address limit enable
#endif

    // ME2_S2 addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W2BYTE(REG_FRC_BK20A_6D_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK20A_6C_L, (addr >> 16), 0x07FF);
    W2BYTEMSK(REG_FRC_BK20A_6B_L, 0x0808, 0x0F0F);
    u32MemSize = XC_FRC_ME2_S2;

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_L_Start_Addr=%tu\n", (ptrdiff_t)addr);
}

void MHal_FRC_Halo_SetBaseAddr(void *pInstance, MS_PHY addr, MS_U32 u32Size)
{
    MS_U8  u8MIUSel;
    //MS_PHY u32StartOffset;
    MS_U32 u32MemSize = 0;
    //MS_U32 u32Width = 3840/16;
#ifdef FRC_MEMORY_PROTECT
    MS_PHY u32MaxLimitAddress =0;
#endif

    // remove the MIU offset
    _phy_to_miu_offset(u8MIUSel, addr, addr);
    //UNUSED(u32StartOffset);

    MHal_FRC_Enable_MiuMask(pInstance);
    //MHal_FRC_set_freezeimg(ENABLE,MAIN_WINDOW);  // ask HW RD

    // HR miu_sel
    W2BYTEMSK(REG_FRC_BK3F_11_L, 0x00, (BIT(12)|BIT(13)|BIT(14))); // miu_sel from register
    W2BYTEMSK(REG_FRC_BK3F_10_L, u8MIUSel?(BIT(12)|BIT(13)|BIT(14)):0x00, (BIT(12)|BIT(13)|BIT(14)));

    // HR addr
    addr = addr / FRC_BYTE_PER_WORD; // unit: 1/256bit
    W2BYTE(REG_FRC_BK22C_60_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK22C_61_L, (addr >> 16), 0x07FF);
    //W2BYTEMSK(REG_FRC_BK22C_62_L, (u32Width/4), 0xFF);  // Move to control by FRC_R2.bin
    //W2BYTEMSK(REG_FRC_BK22C_68_L, (u32Width/4), 0xFF);  // Move to control by FRC_R2.bin
    u32MemSize = XC_FRC_HR;
#ifdef FRC_MEMORY_PROTECT
    u32MaxLimitAddress = addr + (u32MemSize/FRC_BYTE_PER_WORD) -1;
    W4BYTE(REG_FRC_BK119_1A_L, addr); //HR memory min address
    W4BYTE(REG_FRC_BK119_1C_L, u32MaxLimitAddress); //HR memory max address
    W2BYTEMSK(REG_FRC_BK119_10_L, BIT(2),BIT(2)); //HR memory address limit enable
#endif

    // HR_BUF23 addr
    addr = addr + (u32MemSize/FRC_BYTE_PER_WORD);
    W2BYTE(REG_FRC_BK22C_52_L, (addr&0xFFFF));
    W2BYTEMSK(REG_FRC_BK22C_51_L, (addr >> 16), 0x07FF);
    //W2BYTEMSK(REG_FRC_BK22C_53_L, (u32Width/8), 0xFF);  // Move to control by FRC_R2.bin
    //W2BYTEMSK(REG_FRC_BK22C_54_L, (u32Width/8), 0xFF);  // Move to control by FRC_R2.bin
    u32MemSize = XC_FRC_HR_BUF23;

    //MHal_FRC_set_freezeimg(DISABLE,MAIN_WINDOW);
    MHal_FRC_Disable_MiuMask(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Set Main_FRC_L_Start_Addr=%tu\n", (ptrdiff_t)addr);
}


void MHal_FRC_Init(void *pInstance, PMST_PANEL_INFO_t pPanelInfo, PFRC_INFO_t pFRCInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB)
    if(psXCInstPri->u32DeviceID == 1)
    {
        // not support
        return;
    }
#endif

    MS_BOOL isYuv = FALSE;
    MS_U32 u32FrameYcout = 0;
    MS_U16 u16FB_YcountLinePitch, u16LineLimit;;
    MS_U8 u8FRC_PnlType = E_FRC_PNL_TYPE_2D; //E_FRC_PNL_TYPE_PASSIVE;
    MS_PHY u32ME2Addr = 0, u32ME2Size = 0;
    MS_PHY u32HRAddr = 0, u32HRSize = 0;
    MS_PHY u32MEMC_R_Addr = 0, u32MEMC_R_Size = 0;

    //FRCTABLE_INFO FRCTableInfo;

    if(pFRCInfo->u8FRC3DPanelType == E_XC_3D_PANEL_SHUTTER)
    {
        u8FRC_PnlType = E_FRC_PNL_TYPE_ACTIVE;
    }
    else if(pFRCInfo->u8FRC3DPanelType == E_XC_3D_PANEL_PELLICLE)
    {
        u8FRC_PnlType = E_FRC_PNL_TYPE_PASSIVE;
    }
    else
    {
        u8FRC_PnlType = E_FRC_PNL_TYPE_2D;
    }

    pFRCInfo->u16PanelWidth = pPanelInfo->u16Width;
    pFRCInfo->u16PanelHeigh = pPanelInfo->u16Height;
    pFRCInfo->u83Dmode = 0;
    pFRCInfo->u8IpMode = FRC_IP_MEM_IP_YC_10_SPECIAL;
    pFRCInfo->u8MirrorMode = 0;

    printf("[%s]FRC 3D panel type=%x\n", __FUNCTION__, pFRCInfo->u8FRC3DPanelType);
    printf("[%s]R2 3D panel type=%x\n", __FUNCTION__, u8FRC_PnlType);
    printf("pFRCInfo->u32FRC_MEMC_MemAddr = %tu\n",(ptrdiff_t)pFRCInfo->u32FRC_MEMC_MemAddr);
    printf("pFRCInfo->u32FRC_MEMC_Memsize = %tu\n",(ptrdiff_t)pFRCInfo->u32FRC_MEMC_MemSize);

    u16LineLimit = MHal_FRC_IPM_GetLineLimit(pFRCInfo->u8IpMode, pFRCInfo->u16PanelWidth, 0);
    u16FB_YcountLinePitch = MHal_FRC_IPM_GetYcoutLinePitch(pFRCInfo->u8IpMode, u16LineLimit);
    u32FrameYcout = MHal_FRC_IPM_GetFrameYcout(pFRCInfo->u16PanelHeigh, u16FB_YcountLinePitch);

    #if 0
    if(u8FRC_PnlType == E_FRC_PNL_TYPE_PASSIVE)
    {
        FRCTableInfo.u8FRC_InputType_Num = FRC_INPUTTYPE_NUM_FRC_Mapping_mode;
        FRCTableInfo.u8FRC_IP_Num =FRC__NUM_FRC_Mapping_mode;
        FRCTableInfo.pFRC_Map_Aray = (void*)FRC_FRC_mapping_FRC_Mapping_mode;
        FRCTableInfo.pIPTAB_Info = (void*) FRC_IPTAB_INFO_FRC_Mapping_mode;
    }
    else //if(u8FRC_PnlType == E_FRC_PNL_TYPE_ACTIVE)
    {
        FRCTableInfo.u8FRC_InputType_Num = FRC_INPUTTYPE_NUM_FRC_Mapping_mode;
        FRCTableInfo.u8FRC_IP_Num =FRC__NUM_FRC_Mapping_mode;
        FRCTableInfo.pFRC_Map_Aray = (void*)FRC_FRC_mapping_FRC_Mapping_mode;
        FRCTableInfo.pIPTAB_Info = (void*) FRC_IPTAB_INFO_FRC_Mapping_mode;
    }
    _MDrv_FRC_AddTable(&FRCTableInfo);
    #endif

    MHal_FRC_IPM_SetBaseAddr(pInstance, pFRCInfo->u32FRC_MEMC_MemAddr, u32FrameYcout, u16FB_YcountLinePitch, 0);
    MHal_FRC_OPM_SetBaseAddr(pInstance, pFRCInfo->u32FRC_MEMC_MemAddr, u32FrameYcout);

    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_DUAL_MIU) &&
        (pFRCInfo->u32FRC_R2_MemSize != 0))
    {
        // FRC dual MIU case, use R2 address for IPM_R using
        u32MEMC_R_Addr = pFRCInfo->u32FRC_R2_MemAddr;
        u32MEMC_R_Size = pFRCInfo->u32FRC_R2_MemSize;
    }
    else
    {
        u32MEMC_R_Addr = pFRCInfo->u32FRC_MEMC_MemAddr + XC_FRC_IPM_L + XC_FRC_MEDS_L;
        u32MEMC_R_Size = XC_FRC_IPM_R + XC_FRC_MEDS_R;
    }

    MHal_FRC_IPM_R_SetBaseAddr(pInstance, u32MEMC_R_Addr);
    MHal_FRC_OPM_R_SetBaseAddr(pInstance, u32MEMC_R_Addr);

    MHal_FRC_ME1_SetBaseAddr(pInstance, pFRCInfo->u32FRC_ME1_MemAddr, pFRCInfo->u32FRC_ME1_MemSize);
    u32ME2Addr = pFRCInfo->u32FRC_ME1_MemAddr + XC_FRC_ME1_X1_READ + XC_FRC_ME1_X1 + XC_FRC_ME1_S1;
    u32ME2Size = XC_FRC_ME2_X2 + XC_FRC_ME2_Y2 + XC_FRC_ME2_F2 + XC_FRC_ME2_LOGO + XC_FRC_ME2_S2;
    MHal_FRC_ME2_SetBaseAddr(pInstance, u32ME2Addr, u32ME2Size);
    u32HRAddr = u32ME2Addr + u32ME2Size;
    u32HRSize = XC_FRC_HR + XC_FRC_HR_BUF23;
    MHal_FRC_Halo_SetBaseAddr(pInstance, u32HRAddr, u32HRSize);


#if FRC_INSIDE
        if( E_MBX_SUCCESS != MApi_MBX_RegisterMSG(E_MBX_CLASS_FRC, FRCR2_MBX_QUEUESIZE))
        {
            printf("\033[0;31m [%s][%d] FRCR2_MBX_Init - register msg error !!! \033[0m\n", __FUNCTION__, __LINE__);
        }

        MS_U8 u8ChipVersion = 0;
        u8ChipVersion = MDrv_ReadByte(REG_CHIP_REVISION);

        // case FRC_MB_CMD_INIT:
        // ParaCnt=3,  P0 = Panel 3D type (P0 = Panel 3D type(0: 2D, 1: passive, 2: active)
        //          P1 = Panel max V Freq, 60/120/240
        //          p2 = LocalDimming panel type
        //          P3 = ChipRevision (0=U01, 1=U02, 2=U03, etc.)

        // R2 command init
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            Hal_XC_FRC_R2_Init(pInstance, u8FRC_PnlType, (pPanelInfo->u16DefaultVFreq/10*2), 0x00, u8ChipVersion);
        }
        else
        {
            Hal_XC_FRC_R2_Init(pInstance, u8FRC_PnlType, (pPanelInfo->u16DefaultVFreq/10), 0x00, u8ChipVersion);
        }
        Hal_XC_FRC_R2_Set_Timing(pInstance, 60, (pPanelInfo->u16DefaultVFreq/10));
        Hal_XC_FRC_R2_Set_InputFrameSize(pInstance, pFRCInfo->u16PanelWidth, pFRCInfo->u16PanelHeigh );
        Hal_XC_FRC_R2_Set_OutputFrameSize(pInstance, pFRCInfo->u16PanelWidth, pFRCInfo->u16PanelHeigh);
        Hal_XC_FRC_R2_Set_Input3DFormat(pInstance, E_FRC_INPUT_2D);
        Hal_XC_FRC_R2_Set_FPLL_Lockdone(pInstance, FALSE);
#endif


    MDrv_FRC_IPM_Init(pInstance, pPanelInfo, pFRCInfo);

#if 0
    MHal_FRC_IPM_CheckBoardEn(0); //Check board

    MHal_FRC_IPM_CeLineCountEn(ENABLE);
#endif
#if 0
    MHal_FRC_OPM_CeLineCountEn(ENABLE);

    MDrv_FRC_OPM_Init();

    MHal_FRC_OPM_3dFuncEn(0, (BIT(0) | BIT(3)));
    MHal_FRC_OPM_SetSrcPixNum(pPanelInfo->u16Width);
    MHal_FRC_OPM_SetSrcLineNum(pPanelInfo->u16Height);
#endif
    MDrv_FRC_SetMemFormat(pInstance, pFRCInfo);

    isYuv = !MHal_FRC_IsRGB(pFRCInfo->u8IpMode);
    MHal_FRC_IPM_Csc(isYuv);

#if 0
    // SNR
    MHal_FRC_SNR_SetPixelHorixontalNum(pPanelInfo->u16Width);
    MHal_FRC_SNR_SetLineVerticalNum(pPanelInfo->u16Height);

    // SCP1
    MHal_FRC_SCP_HSU1_HSP(0, 0);
    MHal_FRC_SCP_HSU1_Init_Position(0);
    MHal_FRC_SCP_HSU1_Scaling_Mode(0);
    MHal_FRC_SCP_HSU1_444to422_Mode(0);
    MHal_FRC_SCP_HSU1_VSU_Scaling_Mode(0);
    MHal_FRC_SCP_HSU1_VSU_Scaling_coef(0);

    // SCP2
    MHal_FRC_SCP_HSU2_HSP(0, 0);
    MHal_FRC_SCP_HSU2_Init_Position(0);
    MHal_FRC_SCP_HSU2_Scaling_Mode(0);
#endif

    // TGEN
    MHal_FRC_TGEN_1Clk2PixOut(TRUE);
    MHal_FRC_TGEN_Init(pInstance);
    MDrv_FRC_Tx_SetTgen(pInstance, pPanelInfo);

#if 0
    // OP2
    MHal_FRC_OP2_ColorMatrixEn(isYuv);
    MHal_FRC_OP2_CscDitherEn(FALSE);
    MHal_FRC_OP2_ColorMatrixEn(ENABLE);
    MHal_FRC_OP2_DebugFuncEn((BIT(3) | BIT(5) | BIT(6)) , BIT(5));
    MHal_FRC_OP2_SetDither(0x2d00);

    // SCTOP
    MHal_FRC_SCTOP_SCMI_Bypass_Enable(DISABLE);
    MHal_FRC_SCTOP_FRC_Bypass_Enable(DISABLE);

#endif

    //--- for demo, init setting here ----
    MDrv_WriteByteMask( REG_FRC_BK134_A0, 0x00, 0x01); // compression mode
    MDrv_WriteByteMask( REG_FRC_BK134_A8, 0x00, 0x01); // compression mode

    // for mantis 1028207: YUV/RGB use same MEDS setting to avoid MIU HIT issue
    MDrv_WriteByteMask( REG_FRC_BK13A_32, 0x0A, 0x0F); // ipm meds frame no.
    MDrv_WriteByteMask( REG_FRC_BK13A_B2, 0x0A, 0x0F); // ipm meds frame no.

    MDrv_WriteByteMask( REG_FRC_BK13A_A0, 0x01, 0x01); // compression mode
    MDrv_WriteByteMask( REG_FRC_BK13A_A8, 0x01, 0x01); // compression mode

    if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
    {
        MDrv_WriteByteMask( REG_FRC_BK134_32, 0x0A, 0x0F); // ipm frame no.
        MDrv_WriteByteMask( REG_FRC_BK134_B2, 0x0A, 0x0F); // ipm frame no.

        MDrv_WriteByteMask( REG_FRC_BK22C_82, 0x7A, 0xFF);  // frame cnt = 10 + rw_diff = 7

        MDrv_Write2ByteMask( REG_FRC_BK31B_10, 0x0101, 0x0101); // YUV in

        //blanking color need to black.
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_28_L, BIT(7), BIT(7));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_29_L, 0x200);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2A_L, 0x0);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2B_L, 0x200);
    }
    else
    {
        MDrv_WriteByteMask( REG_FRC_BK134_32, 0x07, 0x0F); // ipm frame no.
        MDrv_WriteByteMask( REG_FRC_BK134_B2, 0x07, 0x0F); // ipm frame no.

        MDrv_WriteByteMask( REG_FRC_BK22C_82, 0x47, 0xFF);  // frame cnt = 7 + rw_diff = 4

        MDrv_Write2ByteMask( REG_FRC_BK31B_10, 0x0000, 0x0101); // RGB in

        //blanking color need to black.
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_28_L, BIT(7), BIT(7));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_29_L, 0x0);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2A_L, 0x0);
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2B_L, 0x0);
    }

    // --- for FRC effi setting ----
    MDrv_Write2ByteMask( REG_FRC_BK134_CE, 0x0100, 0x0100);

    // miu merge
    MDrv_Write2Byte( REG_FRC_BK320_E4, 0xFF04);
    MDrv_Write2Byte( REG_FRC_BK320_E6, 0x1010);
    MDrv_Write2Byte( REG_FRC_BK320_E8, 0xFF04);
    MDrv_Write2Byte( REG_FRC_BK320_EA, 0x1010);

    // ipm
    MDrv_Write2Byte( REG_FRC_BK134_42, 0x1818);
    MDrv_Write2Byte( REG_FRC_BK134_44, 0x1800);
    MDrv_Write2Byte( REG_FRC_BK134_4A, 0x300A);

    MDrv_Write2Byte( REG_FRC_BK134_C2, 0x1818);
    MDrv_Write2Byte( REG_FRC_BK134_C4, 0x1800);
    MDrv_Write2Byte( REG_FRC_BK134_CA, 0x300A);

    MDrv_Write2Byte( REG_FRC_BK13A_42, 0x1818);
    MDrv_Write2Byte( REG_FRC_BK13A_44, 0x1800);
    MDrv_Write2Byte( REG_FRC_BK13A_4A, 0x300A);

    MDrv_Write2Byte( REG_FRC_BK13A_C2, 0x1818);
    MDrv_Write2Byte( REG_FRC_BK13A_C4, 0x1800);
    MDrv_Write2Byte( REG_FRC_BK13A_CA, 0x300A);

    // opm
    MDrv_Write2Byte( REG_FRC_BK134_46, 0x1008);
    MDrv_Write2Byte( REG_FRC_BK134_48, 0x4018);

    MDrv_Write2Byte( REG_FRC_BK136_46, 0x1008);
    MDrv_Write2Byte( REG_FRC_BK136_48, 0x4018);

    MDrv_Write2Byte( REG_FRC_BK13A_46, 0x1008);
    MDrv_Write2Byte( REG_FRC_BK13A_48, 0x4018);

    MDrv_Write2Byte( REG_FRC_BK13B_46, 0x1008);
    MDrv_Write2Byte( REG_FRC_BK13B_48, 0x4018);

    MDrv_Write2Byte( REG_FRC_BK13C_46, 0x1008);
    MDrv_Write2Byte( REG_FRC_BK13C_48, 0x4018);

    MDrv_Write2Byte( REG_FRC_BK13D_46, 0x1008);
    MDrv_Write2Byte( REG_FRC_BK13D_48, 0x4018);

}

void MHal_FRC_ByPass_Enable(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB)
    if(psXCInstPri->u32DeviceID == E_XC_DEVICE1)
    {
        // not support
        return;
    }
#endif
    if (bEnable)
    {
        // disable clk and miu
        W2BYTEMSK(REG_FRC_BK119_02_L, 0x4141, 0x4041);
        W2BYTEMSK(REG_FRC_BK119_03_L, 0x0100, 0x0100);
        W2BYTEMSK(REG_FRC_BK119_04_L, 0x0001, 0x0001);
        W2BYTEMSK(REG_FRC_BK121_03_L, 0x1000, 0x1000);
        MHal_FRC_Enable_MiuMask(pInstance);
    }
    else
    {
        // enable clk and miu
        W2BYTEMSK(REG_FRC_BK119_02_L, 0x0000, 0x4041);
        W2BYTEMSK(REG_FRC_BK119_03_L, 0x0000, 0x0100);
        W2BYTEMSK(REG_FRC_BK119_04_L, 0x0000, 0x0001);
        W2BYTEMSK(REG_FRC_BK121_03_L, 0x0000, 0x1000);
        MHal_FRC_Disable_MiuMask(pInstance);
    }

    #if 0 // Need to check with HW RD
    //MHal_CLKGEN_FRC_Bypass_Enable(bEnable);
    #if 0
    W2BYTEMSK(L_BK_CLKGEN0(0x53),0x00,0x02); // Not Invert
    W2BYTEMSK(L_BK_CLKGEN0(0x53),0x00,0x01); // Enable clock
    // Enable CLK_ODCLK
    W2BYTEMSK(L_BK_CLKGEN0(0x53),0xC0C,0xC0C);   // LPLL output clock
    #endif

    #if 0  ////// For Einstein FRC bypass

    if(!bEnable)
    {
        FRC_DBG_MSG("\\*********************************\n");
        FRC_DBG_MSG("\\Load FRC Normal mode table\n");
        FRC_DBG_MSG("\\*********************************\n");

        Hal_SC_set_bws_mode(ENABLE,MAIN_WINDOW);
        //=====> MIU
        MDrv_WriteByte(0x101282, 0x1c);
    }
    #endif

    // MOD control for FRC bypass (enable frc 1px2 input)
    if (bEnable)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_45_L, 0x0000, 0x8000);
    }
    else
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_45_L, 0x8000, 0x8000);
    }
    #endif
}

void MHal_FRC_Composer_User_Mode_Enable(void *pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_40_L, bEnable?BIT(2):0x00, BIT(2));
}

void MHal_FRC_Set_Composer_User_Mode(void *pInstance, E_FRC_COMPOSER_SOURCE_MODE eMode, MS_BOOL bMenuload)
{
    if (ENABLE == bMenuload)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_40_L, eMode, 0x03);
    }
    else
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_40_L, eMode, 0x03);
    }

}

void MHal_FRC_Set_Pipe_Delay_Mode(void *pInstance, E_FRC_PIPE_DELAY_MODE eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_50_L, eMode, 0x0B); // [1:0]: mode, [3]: reset
}

void MHal_FRC_Set_Pipe_Delay_Value(void *pInstance, MS_U16 u16Hcnt, MS_U16 u16Vcnt)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));


    //MS_U16 u16VsyncEnd = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L);
    //MS_U16 u16FRCTrig2 = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_66_L);

    // For Raptor Vsync end too large causes video flicker issue
    // mantis: 1025485: VG849 timing 851, video flicker with Raptor
    // --------|------------|**** bk68 V delay delay ****|-----------
    //   bk10 Vsync End                          bk68 FRC trig2
    // HW RD said, the bk10 Vsync End must be before (bk68 FRC trij2 - bk68 V delay value)
    // The limitation isn't existed in the maxim chip, according to HW RD

    /*if ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(15)) == BIT(15)) &&  // Not Auto Vsync case
        ((u16FRCTrig2 - u16Vcnt) < u16VsyncEnd) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled == TRUE))// If FRC not enable, pipe delay sould not be affected by FRCTrig point
    {
        if(u16FRCTrig2 > u16VsyncEnd)
        {
            u16Vcnt = u16FRCTrig2 - u16VsyncEnd;
        }
        else
        {
            u16Vcnt = 0;
        }

    }*/

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_52_L, u16Hcnt, 0x3FFF);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_51_L, u16Vcnt, 0x1FFF);
}

void MHal_FRC_Set_Pipe_Delay_Reset(void *pInstance, MS_BOOL bEnable, MS_BOOL bMenuload)
{
    if (ENABLE == bMenuload)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_50_L, bEnable?BIT(3):0x00, BIT(3));
    }
    else
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

        // if STGEN enabled, need to reset
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_50_L, bEnable?BIT(3):0x00, BIT(3)); //[3]: reset
    }
}

void MHal_FRC_Set_STGEN_ODCLK(void *pInstance, E_FRC_STGEN_ODCLK eMode, MS_BOOL bMenuload)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8LPLL_Type = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;
    MS_U8 u8LPLL_Mode = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode;

    E_FSC_IDCLK eFSCIDCLK = E_FSC_IDCLK_1;

    if( (E_XC_PNL_LPLL_TTL == u8LPLL_Type)||
       ((E_XC_PNL_LPLL_LVDS == u8LPLL_Type)&&(E_XC_MOD_OUTPUT_SINGLE == u8LPLL_Mode))||
       ((E_XC_PNL_LPLL_HS_LVDS == u8LPLL_Type)&&(E_XC_MOD_OUTPUT_SINGLE == u8LPLL_Mode)))
    {
        eMode = E_FRC_STGEN_ODCLK_2;
    }

    if (eMode == E_FRC_STGEN_ODCLK_2)
    {
        eFSCIDCLK = E_FSC_IDCLK_2;
    }
    else if (eMode == E_FRC_STGEN_ODCLK_4)
    {
        eFSCIDCLK = E_FSC_IDCLK_4;
    }

    if (ENABLE == bMenuload)
    {
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK3F_60_L, eMode, 0x03);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1420, 0x04, (eFSCIDCLK<<8), 0x0300);
    }
    else
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK3F_60_L, eMode, 0x03);
        MDrv_WriteByteMask( REG_FSC_BK20_05, eFSCIDCLK, 0x03);
    }
}

void MHal_FRC_Set_FSC_DE_Selection(void *pInstance, E_FRC_FSC_SOURCE_MODE eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_28_L, eMode<<4, BIT(4));
}

void MHal_FRC_Set_3D_QMap(void *pInstance, MS_U8 u8FRC3DPanelType, MS_U16 u16sc_in_3dformat, MS_U16 u16sc_out_3dformat, MS_U8 u83D_FI_out)
{
#if FRC_INSIDE
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled) &&
        (psXCInstPri->u32DeviceID != 1))
    {
        if (IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K2K out
            ||IS_OUTPUT_4K1K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height) /// 4k1k out
            ||IS_OUTPUT_4K0_5K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) /// 4k0.5k out
        {
            if (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW))
            {
                if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
                {
                    MFC_3D_2D_4K2K_2D_FHD_YUV(); // load FRC 3D-table as FHD in
                }
                else
                {
                    MFC_3D_2D_4K2K_2D_FHD_RGB_BYPASS();   // for by pass MEMC test
                }

                // For Maxim HDMI Tx 4096x2160 case
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == SCALER_LINE_BUFFER_MAX)
                {
                    MDrv_WriteByteMask( REG_SC_BK4F_50, 0x00, 0xff); // hsp_hsize_in0
                    MDrv_WriteByteMask( REG_SC_BK4F_51, 0x08, 0xff); // hsp_hsize_in1
                    MDrv_WriteByteMask( REG_SC_BK4F_54, 0x00, 0xff); // hsp_hsize_out0
                    MDrv_WriteByteMask( REG_SC_BK4F_55, 0x10, 0xff); // hsp_hsize_out1
                }
            }
            else
            {
                if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
                {
                    MFC_3D_2D_4K2K_2D_4K2K_YUV(); // load FRC 3D-table as 4K in
                }
                else
                {
                    MFC_3D_2D_4K2K_2D_4K2K_RGB_BYPASS();  // for by pass MEMC test
                }

                // For Maxim HDMI Tx 4096x2160 case
                if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == SCALER_LINE_BUFFER_MAX)
                {
                    MDrv_WriteByteMask( REG_SC_BK4F_50, 0x00, 0xff); // hsp_hsize_in0
                    MDrv_WriteByteMask( REG_SC_BK4F_51, 0x10, 0xff); // hsp_hsize_in1
                    MDrv_WriteByteMask( REG_SC_BK4F_54, 0x00, 0xff); // hsp_hsize_out0
                    MDrv_WriteByteMask( REG_SC_BK4F_55, 0x10, 0xff); // hsp_hsize_out1
                }
            }
        }
        else  // FHD out
        {
            if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
            {
                MFC_3D_2D_FHD_2D_FHD_YUV(); // load FRC 3D-table as FHD in
            }
            else
            {
                MFC_3D_2D_FHD_2D_FHD_RGB_BYPASS();   // for by pass MEMC test
            }
        }


#if !defined (MSOS_TYPE_NOS)
        if(pXCResourcePrivate->stdrvXC_3D._bFRC3DEnabled )//FRC 3D enabled, need load 3D table
        {
            if(IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K2K out
                && IsVBY1_16LANE((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)///120Hz
                && !(pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA)///non FA_LLRR out
                && (pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode == E_XC_3D_INPUT_TOP_BOTTOM) //TB in
                && (pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))//LA out
            {
                MFC_3D_FRC_PAS_4K2K_120_3D_FHD_TB();//TB to LA
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 4k2k 120Hz load 3D tabel TB to LA\n");
            }
            else if(IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K2K out
                && (580 < pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq) ///60Hz
                && (620 > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq)///60Hz
                && (pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode == E_XC_3D_INPUT_TOP_BOTTOM) //TB in
                && (pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode == E_XC_3D_OUTPUT_LINE_ALTERNATIVE))//LA out
            {
                MFC_3D_FRC_PAS_4K2K_60_3D_FHD_TB();//TB to LA
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 4k2k 60Hz load 3D tabel TB to LA\n");
            }
            else if(IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K2K out
                && IsVBY1_16LANE((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)///120Hz
                && !(pXCResourcePrivate->stdrvXC_3D._bFALLRRToFA)///non FA_LLRR out
                && (pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode == E_XC_3D_INPUT_TOP_BOTTOM) //TB in
                && (pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))//FA out
            {
                MFC_3D_FRC_ACT_4K2K_120_3D_FHD_TB();//TB to FA
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 4k2k 120Hz  load 3D tabel TB to FA\n");
            }
            else if(IS_OUTPUT_4K1K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K1K out
                && (1180 < pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq) ///120Hz
                && (1220 > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq)///120Hz
                && (pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode == E_XC_3D_INPUT_TOP_BOTTOM) //TB in
                && (pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))//FA out
            {
                MFC_3D_FRC_ACT_4K1K_120_3D_FHD_TB();//TB to FA
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 4k1k 120Hz  load 3D tabel TB to FA\n");
            }
            else if(IS_OUTPUT_4K1K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K1K out
                && (2380 < pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq) ///240Hz
                && (2420 > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq)///240Hz
                && (pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode == E_XC_3D_INPUT_TOP_BOTTOM) //TB in
                && (pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))//FA out
            {
                MFC_3D_FRC_ACT_4K1K_LLRR_240_3D_FHD_TB();//TB to FA
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 4k1k 240Hz load 3D tabel TB to FA\n");
            }
            else if(IS_OUTPUT_4K0_5K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, \
                             pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)// 4K0.5K out
                && (2380 < pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq) ///240Hz
                && (2420 > pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq)///240Hz
                && (pXCResourcePrivate->stdrvXC_3D._eFrcInput3DMode == E_XC_3D_INPUT_TOP_BOTTOM) //TB in
                && (pXCResourcePrivate->stdrvXC_3D._eFrcOutput3DMode == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))//FA out
            {
                MFC_3D_FRC_ACT_4K0_5K_LLRR_240_3D_FHD_TB();//TB to FA
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "## 4k0.5k 240Hz  load 3D tabel TB to FA\n");
            }

        }
#endif

        if (!gSrcInfo[MAIN_WINDOW].bForceRGBin)
        {
            MDrv_WriteByteMask( REG_FRC_BK134_32, 0x0A, 0x0F); // ipm frame no.
            MDrv_WriteByteMask( REG_FRC_BK134_B2, 0x0A, 0x0F); // ipm frame no.

            MDrv_WriteByteMask( REG_FRC_BK22C_82, 0x7A, 0xFF);  // frame cnt = 10 + rw_diff = 7

            MDrv_Write2ByteMask( REG_FRC_BK31B_10, 0x0101, 0x0101); // YUV in

            //blanking color need to black.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_28_L, BIT(7), BIT(7));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_29_L, 0x200);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2A_L, 0x0);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2B_L, 0x200);

        }
        else
        {
            MDrv_WriteByteMask( REG_FRC_BK134_32, 0x07, 0x0F); // ipm frame no.
            MDrv_WriteByteMask( REG_FRC_BK134_B2, 0x07, 0x0F); // ipm frame no.

            MDrv_WriteByteMask( REG_FRC_BK22C_82, 0x47, 0xFF);  // frame cnt = 7 + rw_diff = 4

            MDrv_Write2ByteMask( REG_FRC_BK31B_10, 0x0000, 0x0101); // RGB in

            //blanking color need to black.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_28_L, BIT(7), BIT(7));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_29_L, 0x0);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2A_L, 0x0);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_2B_L, 0x0);

        }

        // set FSC trig point
        MDrv_WriteByteMask( REG_FSC_BK03_24, 0x0A, 0xff);
        MDrv_WriteByteMask( REG_FSC_BK03_62, 0x0A, 0xff);
        MDrv_WriteByteMask( REG_FSC_BK03_64, 0x0A, 0xff);
    }
#endif
}

MS_BOOL MHal_FRC_IsFHDToFSC(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    // For Manhattan, for non-4K2K input, need to sent FHD to FSC
    if ( pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal> 3000) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal> 2000) && //4K2K out
        (psXCInstPri->u32DeviceID != 1) && //sc1 no fsc
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_MODE_NONE) &&
        ((!((MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW))||(MDrv_XC_GetDynamicScalingStatus(pInstance))))  || // with non DS case
         (((MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW))||(MDrv_XC_GetDynamicScalingStatus(pInstance))) && IS_SUPPORT_DS_SR(gSrcInfo[eWindow].u16InputVFreq))) && //with DS case
        (((100 * gSrcInfo[eWindow].stDispWin.width) > (266 * gSrcInfo[eWindow].stCropWin.width)) &&  // Maxim FSC only support post-scaling > 2.66x case
         ((100 * gSrcInfo[eWindow].stDispWin.height) > (266 * gSrcInfo[eWindow].stCropWin.height)))
        )
    {
        bRet = TRUE;

        #if (defined(CONFIG_PURE_SN))
        // For pure-SN, MHEG5 test items, always use 4K to FSC
        if(IsSrcTypeDTV(gSrcInfo[MAIN_WINDOW].enInputSourceType)) // DTV
        {
            bRet = FALSE;
        }
        #endif
    }
#endif

    return bRet;
}

MS_BOOL MHal_FRC_IsEnableFRC3D(void *pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bRet = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_U8 u8LPLL_Type = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;
    if (((!IsVBY1_16LANE(u8LPLL_Type) && pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled
         &&(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL() ||IS_OUTPUT_4K2K_60HZ_PANEL()))
         || IsVBY1_16LANE(u8LPLL_Type))
         && (psXCInstPri->u32DeviceID != 1)
        //Frc cant do memc under RGB source.Memc will bypass if RGB source in.
        //Under memc bypass mode it cant send 3D LR flag so that RGB 3D should disable FRC enable flag.
         && gSrcInfo[MAIN_WINDOW].bUseYUVSpace
         && (!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE) )
         )
    {
        bRet = TRUE;
    }
#endif
    return bRet;
}

MS_BOOL MHal_FRC_IsSupportFRC_byEfuse(void *pInstance)
{
    MS_BOOL bResult = FALSE;

    MS_U32 u32EfuseResult = 0;
    MS_BOOL bDisableFRC = FALSE;
    MS_BOOL bIgnoreDisableFRC = FALSE;

    W2BYTE(0x2050, 0x21BC); // efuse address 0x6F *4 = 0x1BC
    MsOS_DelayTaskUs(5);
    u32EfuseResult = R2BYTE(0x2058);

    bDisableFRC = ((u32EfuseResult & 0x0010) >> 4);  // 6F[4]: 1: disable FRC
    bIgnoreDisableFRC = (u32EfuseResult & 0x0001);  //  6F[0]: 1: ignore previous efuse bit for disable FRC

    if (bIgnoreDisableFRC || (bDisableFRC == FALSE))
    {
        bResult = TRUE; // Support FRC
    }

    return bResult;
}

MS_BOOL MHal_FRC_Check_Condition(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_BOOL bSupport = FALSE;

    #if 1
    bSupport = TRUE;  // for Maserati, all case can support FRC
    #else
    if (!(MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance)) && !(IsSrcTypeStorage(gSrcInfo[MAIN_WINDOW].enInputSourceType) && IS_INPUT_4K2K(MAIN_WINDOW) && (gSrcInfo[MAIN_WINDOW].u16InputVFreq > 330)))//Non DS: exclusive of 4K@50/60
         ||((MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance)) && (!IS_DS_4K2K(MAIN_WINDOW)))//DS case
         ||((MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance)) && (IS_DS_4K2K(MAIN_WINDOW)) && (gSrcInfo[MAIN_WINDOW].u16InputVFreq < 330))  //DS case
    {
        bSupport = TRUE;
    }
    #endif

    //For HDMIPC 4K2K, close FRC for bandwidth issue for dolby
    if
    (IsSrcTypeHDMI(gSrcInfo[MAIN_WINDOW].enInputSourceType) && (pXCResourcePrivate->stdrvXC_MVideo.s_eMemFmt[MAIN_WINDOW] == E_MS_XC_MEM_FMT_444)
    && IS_INPUT_4K2K(MAIN_WINDOW) && (gSrcInfo[MAIN_WINDOW].u16InputVFreq > 330)
    )
    {
        bSupport = FALSE;
    }

    // For Maxim HDMI Tx case, turn off FRC case:
    // 1. output size is under FHD
    // 2. output H size > 3840
    // 3. input frame rate > output frame rate
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        if(!IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height))
        {
            bSupport = FALSE;
        }
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width > FRC_LINE_BUFFER_MAX) //FRC hw limitation, line buffer only 3840, it can't output larger than 3840.
        {
            bSupport = FALSE;
        }
        if((gSrcInfo[MAIN_WINDOW].u16InputVFreq/10) > (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq/10))
        {
            bSupport = FALSE;
        }
    }

    return bSupport;
}

void MHal_FRC_AdjustGOPPosition(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    // Adjust FSC V12L en
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)&&(psXCInstPri->u32DeviceID != 1))
    {
        if (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW))
        {
            if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
            {
                MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1423, 0x26, 0x1000, 0xFFFF);
            }
            else
            {
                MDrv_Write2ByteMask( REG_FSC_BK23_26, 0x1000, 0x1000);
            }
        }
        else
        {
            if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
            {
                MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1423, 0x26, 0x0000, 0xFFFF);
            }
            else
            {
                MDrv_Write2ByteMask( REG_FSC_BK23_26, 0x0000, 0x1000);
            }
        }
    }

    // For Maserati, no need this SW patch
    #if 0
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32HOriginalValue = 0;
    MS_U32 u32VOriginalValue = 0;

    MS_U32 u32GOPSetting = 0x00;

    MS_U32 u32HData = 0x00;
    MS_U32 u32VData = 0x16;

    MS_U16 u16NowOffsetH = 0;
    MS_U16 u16NowOffsetV = 0;

    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)&&(psXCInstPri->u32DeviceID != 1))
    {
        if (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW))
        {
            u16NowOffsetH = GOP_FSC_FHD_OFFSET_H;
            u16NowOffsetV = GOP_FSC_FHD_OFFSET_V;

            // Adjust FSC V12L en
            MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1423, 0x26, 0x1000, 0xFFFF);
        }
        else
        {
            u16NowOffsetH = GOP_FSC_4K_OFFSET_H;
            u16NowOffsetV = GOP_FSC_4K_OFFSET_V;

            // Adjust FSC V12L en
            MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1423, 0x26, 0x0000, 0xFFFF);
        }
    }
    else
    {
        u16NowOffsetH = GOP_NO_FSC_OFFSET_H;
        u16NowOffsetV = GOP_NO_FSC_OFFSET_V;
    }


    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)&&(psXCInstPri->u32DeviceID != 1))
    {
        // Patch for GOP
        u32GOPSetting = (MDrv_Read2Byte(0x120200) & ~0x4002) | (0x0002 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1202, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x120500) & ~0x4002) | (0x0002 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1205, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x120800) & ~0x4002) | (0x0002 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1208, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x120B00) & ~0x4002) | (0x0002 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x120B, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x121B00) & ~0x4002) | (0x0002 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x121B, 0x00, u32GOPSetting, 0xFFFF);
    }

    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1202, 0xFE, 0x0200, 0xFFFF);

    // GOP 1202
    u32HOriginalValue = MDrv_Read2Byte(0x12021E) + g_u16LastOffsetH;
    u32HData = u32HOriginalValue - u16NowOffsetH;
    u32VOriginalValue = MDrv_Read2Byte(0x120268) - g_u16LastOffsetV;
    u32VData = u32VOriginalValue + u16NowOffsetV;
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1202, 0x1E, u32HData, 0xFFFF);
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1202, 0x68, u32VData, 0xFFFF);

    // GOP 1205
    u32HOriginalValue = MDrv_Read2Byte(0x12051E) + g_u16LastOffsetH;
    u32HData = u32HOriginalValue - u16NowOffsetH;
    u32VOriginalValue = MDrv_Read2Byte(0x120568) - g_u16LastOffsetV;
    u32VData = u32VOriginalValue + u16NowOffsetV;
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1205, 0x1E, u32HData, 0xFFFF);
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1205, 0x68, u32VData, 0xFFFF);

    // GOP 1208
    u32HOriginalValue = MDrv_Read2Byte(0x12081E) + g_u16LastOffsetH;
    u32HData = u32HOriginalValue - u16NowOffsetH;
    u32VOriginalValue = MDrv_Read2Byte(0x120868) - g_u16LastOffsetV;
    u32VData = u32VOriginalValue + u16NowOffsetV;
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1208, 0x1E, u32HData, 0xFFFF);
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1208, 0x68, u32VData, 0xFFFF);

    // GOP 120B
    u32HOriginalValue = MDrv_Read2Byte(0x120B1E) + g_u16LastOffsetH;
    u32HData = u32HOriginalValue - u16NowOffsetH;
    u32VOriginalValue = MDrv_Read2Byte(0x120B68) - g_u16LastOffsetV;
    u32VData = u32VOriginalValue + u16NowOffsetV;
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x120B, 0x1E, u32HData, 0xFFFF);
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x120B, 0x68, u32VData, 0xFFFF);

    // GOP 121B
    u32HOriginalValue = MDrv_Read2Byte(0x121B1E) + g_u16LastOffsetH;
    u32HData = u32HOriginalValue - u16NowOffsetH;
    u32VOriginalValue = MDrv_Read2Byte(0x121B68) - g_u16LastOffsetV;
    u32VData = u32VOriginalValue + u16NowOffsetV;
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x121B, 0x1E, u32HData, 0xFFFF);
    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x121B, 0x68, u32VData, 0xFFFF);

    MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1202, 0xFE, 0x0000, 0xFFFF);

    g_u16LastOffsetH = u16NowOffsetH;
    g_u16LastOffsetV = u16NowOffsetV;

    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16GOPOffsetH = g_u16LastOffsetH;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16GOPOffsetV = g_u16LastOffsetV;

    if (!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled))
    {
        // Revert for GOP
        u32GOPSetting = (MDrv_Read2Byte(0x120200) & ~0x4002) | (0x4000 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1202, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x120500) & ~0x4002) | (0x4000 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1205, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x120800) & ~0x4002) | (0x4000 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x1208, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x120B00) & ~0x4002) | (0x4000 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x120B, 0x00, u32GOPSetting, 0xFFFF);
        u32GOPSetting = (MDrv_Read2Byte(0x121B00) & ~0x4002) | (0x4000 & 0x4002);
        MDrv_XC_MLoad_WriteCommand_NonXC(pInstance, 0x121B, 0x00, u32GOPSetting, 0xFFFF);
    }
    #endif
}

void MHal_FRC_GetGOPOffset(void *pInstance, MS_U16 *u16OffsetH, MS_U16 *u16OffsetV)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    *u16OffsetH = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16GOPOffsetH;
    *u16OffsetV = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16GOPOffsetV;
}

void MHal_FRC_Set_Mload_Trig_Mode(void *pInstance, E_FRC_MLOAD_TRIG_MODE eMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_5F_L, eMode << 15, BIT(15));
}

////////////////////////////////////////////////////////////////////////////////
// Added by Daniel for control A5 FRC_R2
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MHal_XC_SendCmdToFRC(void *pInstance, MS_U8 u8Cmd, MS_U8 count, MS_U8 p1, MS_U8 p2, MS_U8 p3, MS_U8 p4, MS_U8 p5, MS_U8 p6, MS_U8 p7, MS_U8 p8)
{
    MS_BOOL befuse = FALSE;
    MS_BOOL bReturn = FALSE;

    befuse = MHal_FRC_IsSupportFRC_byEfuse(pInstance);

    if(befuse)
    {

        MBX_Msg mbxMsg;
        #if 1
        MBX_Result mbxResult;
        #endif
        MS_U32 u32Time;


        mbxMsg.eRoleID          = E_MBX_ROLE_FRC;
        mbxMsg.eMsgType         = E_MBX_MSG_TYPE_NORMAL;
        mbxMsg.u8Ctrl           = 0; // Useless?
        mbxMsg.u8MsgClass       = E_MBX_CLASS_FRC;
        mbxMsg.u8Index          = u8Cmd;
        mbxMsg.u8ParameterCount = count;
        mbxMsg.u8Parameters[0]  = p1;
        mbxMsg.u8Parameters[1]  = p2;
        mbxMsg.u8Parameters[2]  = p3;
        mbxMsg.u8Parameters[3]  = p4;
        mbxMsg.u8Parameters[4]  = p5;
        mbxMsg.u8Parameters[5]  = p6;
        mbxMsg.u8Parameters[6]  = p7;
        mbxMsg.u8Parameters[7]  = p8;
        mbxMsg.u8Parameters[8]  = 0;
        mbxMsg.u8Parameters[9]  = 0;
        mbxMsg.u8S0             = 0;
        mbxMsg.u8S1             = 0;

        u32Time = MsOS_GetSystemTime();
        #if 1 // wait for R2 ready
        do
        {
            mbxResult = MApi_MBX_SendMsg(&mbxMsg);

            switch(mbxResult)
            {
                case E_MBX_SUCCESS:
                    // Do nothing
                    bReturn = TRUE;
                    break;
            #if 0
                case E_MBX_ERR_PEER_CPU_BUSY:
                case E_MBX_ERR_PEER_CPU_NOTREADY:
                case E_MBX_ERR_PEER_CPU_NOT_ALIVE:
                    //Error Handling here...
                    break;
                case E_MBX_ERR_PEER_CPU_OVERFLOW:
                    //MsOS_DelayTask(1);
                    break;
            #endif
                default:
                    printf("[XC]Send Cmd fail=%u\n", mbxResult);
                    break;
            }

            if( (MsOS_GetSystemTime() - u32Time) > 3000 )
            {
                printf("[XC]Send Cmd timeout!!\n");
                bReturn = FALSE;
                break;
            }

        }while(E_MBX_SUCCESS != mbxResult);
        #endif
    }

    return bReturn;
}


#define MAX_MAILBOX_BUF     8
MS_BOOL MHal_XC_GetMsgFromFRC(void *pInstance, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para)
{
    MS_BOOL bReturn = FALSE;
    MBX_Msg mbxMsg={0};
    MBX_Result mbxResult;
    MS_U32 u32Time;


    if( (pu8Cmd == NULL) || (pu8ParaCount == NULL) || (pu8Para == NULL) )
        return FALSE;

    u32Time = MsOS_GetSystemTime();
    while( 1 )
    {
        mbxResult = MApi_MBX_RecvMsg(E_MBX_CLASS_FRC, &mbxMsg, 0, MBX_CHECK_NORMAL_MSG);
        if (mbxResult == E_MBX_SUCCESS)
        {
            MS_U8 i, u8ParaCnt;

            u8ParaCnt = mbxMsg.u8ParameterCount;
            if( u8ParaCnt > MAX_MAILBOX_BUF )
                u8ParaCnt = MAX_MAILBOX_BUF;

            *pu8Cmd = mbxMsg.u8Index;
            *pu8ParaCount = u8ParaCnt;
            //printf("Got cmd: cls=%X, idx=%X, ParaCnt=%u, ", mbxMsg.u8MsgClass, mbxMsg.u8Index, mbxMsg.u8ParameterCount);
            for( i = 0; i < u8ParaCnt; i++ )
            {
                pu8Para[i] = mbxMsg.u8Parameters[i];
                //printf("[%X]", mbxMsg.u8Parameters[u16Idx]);
            }
            //printf("\n");
            bReturn = TRUE;
            break;
        }
        else if (mbxResult != E_MBX_ERR_NO_MORE_MSG)
        {
            printf("[XC]Recieve FRC-MSG Failed!!\n");
            break;
        }

        if( (MsOS_GetSystemTime()-u32Time) > 3000 )
        {
            printf("[XC]Recieve FRC-MSG timeout!\n");
            break;
        }
    }

    return bReturn;
}

#define FRC_MB_CMD_VERSION  2

#if 0 // move to apiXC.h
typedef enum
{
    FRC_MB_CMD_NONE = 0x00,
    FRC_MB_CMD_TEST = 0x01, // Print parameter
    FRC_MB_CMD_GET_SW_VERSION = 0x02, // ParaCnt=0, return 2 Byte, P0=Main, P1=Minor
    FRC_MB_CMD_GET_CMD_VERSION = 0x03, // ParaCnt=0, return 2 Byte, P0=Low byte, P1=High byte

    FRC_MB_CMD_INIT = 0x10,         // ParaCnt=5, P0 = Panel 3D type(0=2D, 1=passive, 2=active, 3=PDP)
                                    //          P1 = Panel max V Freq, 60/120/240, but 240 Hz is not supported
                                    //          P2 = LocalDimming panel type, (0=LG32inch_LR10, 1=LG37inch_LR10, 2=LG42inch_LR16, 3=LG47inch_LR16, 4=LG55inch_LR16,
                                    //               5=TOSHIBA_LR16, 6=TOSHIBA_TB16, 8=LG50inchCinema_TB32, 9=LG60inchCinema_TB32, A=LG47inch_Direct72, B=LG55inch_Direct96, C=LG72inch_Direct480)
                                    //          P3 = ChipRevision (0=U01, 1=U02, 2=U03, etc.)
                                    //          P4 = Is60HzPackage (0=FALSE, 1=TRUE)
    FRC_MB_CMD_SET_TIMING = 0x11,   // ParaCnt=2, P0 = Input V Freq, P1 = Output V Freq
    FRC_MB_CMD_SET_INPUT_FRAME_SIZE = 0x12, // ParaCnt=4, P0 = Width[15:8], P1 = width[7:0], P2 = height[15:8], P3 = height[7:0]
    FRC_MB_CMD_SET_OUTPUT_FRAME_SIZE = 0x13,// ParaCnt=4, P0 = Width[15:8], P1 = width[7:0], P2 = height[15:8], P3 = height[7:0]
    FRC_MB_CMD_SET_INPUT_3D_FORMAT = 0x14,  // ParaCnt=2, P0 = input 3D format(0=2D, 1=Side-by-side, 2=Top-Down, 3=Check-Board, 4=Frame-interleave, 6=Line alternative, 7=Frame-packing)
                                            //          P1 = 3DModeConfig(0=Normal, 1=L/R Swap, 2=L-Only, 3=R-Only)

    FRC_MB_CMD_SET_3D_DEPTH_OFFSET = 0x15,    // ParaCnt=2, P0 = Enable3DDepthOffset(0=Disable, 1=Enable)
                                    //          P1 = 3DDepthOffset(0~255)

    FRC_MB_CMD_SET_FPLL_LOCK_DONE = 0x16,  // ParaCnt=1, P0 = FPLL lock done(0: free run or not locked yet, 1: Locked)

    FRC_MB_CMD_SET_MFC_MDOE = 0x20,     // ParaCnt=3, P0 = MFCMode(0=ff, 1=Low, 2=Mid, 3=High, 4=User, 5=55 pull down, 6=64 pull down)
                                        //          P1 = DeJudderLevel(0~10) [for User mode only]
                                        //          P2 = DeBlurLevel(0~10) [for User mode only]

    FRC_MB_CMD_SET_MFC_DEMO_MODE = 0x21,    // ParaCnt=1, P0 = DemoModeEnable(0=Disable,1=Enable)

    FRC_MB_CMD_SET_LOCAL_DIMMING_MODE = 0x30,   // ParaCnt=1, P0 = LocalDimmingMode(0=Off, 1=Low, 2=Mid, 3=High, 4=Demo)

    FRC_MB_CMD_SET_2D_TO_3D_MODE = 0x40,    // ParaCnt=5, P0 = 2D to 3D ConversionMode(0=Disable, 1=Enable)
                                            //          P1 = 2D to 3D Depth Gain(0~31)
                                            //          P2 = 2D to 3D Depth Offset(0~127)
                                            //          P3 = Artificial Gain(0~15)
                                            //          P4 = L/R Swap (0=Normal, 1=L/R Swap)

///*    the value is not decided yet
    FRC_MB_CMD_SET_PWM_ENABLE = 0x50,   // ParaCnt=1, P0 = PWM Enable(0=Disable, 1=Enable)

    FRC_MB_CMD_SET_PWM_DUTY_PHASE = 0x51,   // ParaCnt=6, P0 = PWM No.(0, 1, 3)
                                            //                  P1 P2 = PWMX_DutyPercentX100(0~65536)
                                            //                  P3 P4 = PWMX_360DegreeX100(0~65536)
                                            //                  P5 = PWMX_Polarity(0=not invert, 1=invert)

    FRC_MB_CMD_SET_PWM_FREQ = 0x52,     // ParaCnt=4, P0 = PWM No.(0, 1, 3)
                                            //                  P1 = PWMX_FreqMul
                                            //                  P2 = PWMX_FreqDiv
                                            //                  P3 = PWMX_VsorHsRST
                                            //                  P4 = PWMX_SyncRstCnt

    FRC_MB_CMD_SET_FORCE_PWM = 0x53,    // ParaCnt=0

    FRC_MB_CMD_SET_IR_EMMITER_LR_SHIFT = 0x54,  // ParaCnt=3, P0 = Select the CMD(0=Lo, 1=Lc, 2=Ro, 3=Rc)
                                            //                  P2 P1= Pulse time shift(0~65536)

    FRC_MB_CMD_SET_IR_EMMITER_LR_MODE = 0x55,      // ParaCnt=1, P0 = CMD Shift(0=Lo Lc, 1=Lc Ro, 2=Ro Rc, 3=Rc Lo)
/*
    FRC_MB_CMD_SET_IR_EMMITER_LR_MODE = 0x55,      // ParaCnt=4, P0 = Action Pattern Selection(0=Action Pattern 1, 1=Action Pattern 2)
                                            //          P1 = L frame CMD(0=Lo Lc, 1=Lc Ro, 2=Ro Rc, 3=Rc Lo for Pattern 1; 0=Lo Rc, 1=Rc Ro, 2=Ro Lc, 3=Lc Lo for Pattern 2)
                                            //          P2 = First CMD shift(0~255)
                                            //          P3 = Second CMD shift(0~255)
*/
/*
    FRC_MB_CMD_SET_PWM_CMD_MODE = 0x56,      // ParaCnt=4, P0 = Action Pattern Selection(0=Action Pattern 1, 1=Action Pattern 2)
                                            //          P1 = L frame CMD(0=Lo Lc, 1=Lc Ro, 2=Ro Rc, 3=Rc Lo for Pattern 1; 0=Lo Rc, 1=Rc Ro, 2=Ro Lc, 3=Lc Lo for Pattern 2)
                                            //          P2 = First CMD shift(0~255)
                                            //          P3 = Second CMD shift(0~255)
*/

} EnuFrcMailboxCmdType;
#endif

#define HIBYTE(value)             ((MS_U8)((value >>8) & 0x0000FF))
#define LOBYTE(value)             ((MS_U8)(value&0x0000FF))

#define XC_SEND_CMD_TO_FRC_R2__0P(pInstance,cmd)              MHal_XC_SendCmdToFRC(pInstance,cmd,0,   0,   0,   0,   0, 0, 0, 0, 0)
#define XC_SEND_CMD_TO_FRC_R2__1P(pInstance,cmd,p1)           MHal_XC_SendCmdToFRC(pInstance,cmd,1,(p1),   0,   0,   0, 0, 0, 0, 0)
#define XC_SEND_CMD_TO_FRC_R2__2P(pInstance,cmd,p1,p2)        MHal_XC_SendCmdToFRC(pInstance,cmd,2,(p1),(p2),   0,   0, 0, 0, 0, 0)
#define XC_SEND_CMD_TO_FRC_R2__3P(pInstance,cmd,p1,p2,p3)     MHal_XC_SendCmdToFRC(pInstance,cmd,3,(p1),(p2),(p3),   0, 0, 0, 0, 0)
#define XC_SEND_CMD_TO_FRC_R2__4P(pInstance,cmd,p1,p2,p3,p4)  MHal_XC_SendCmdToFRC(pInstance,cmd,4,(p1),(p2),(p3),(p4), 0, 0, 0, 0)

MS_U16 Hal_XC_FRC_R2_Get_SwVersion(void *pInstance)
{
    MS_U16 u16SwVersion = 0;
    MS_U8 u8Cmd;
    MS_U8 u8ParaCount;
    MS_U8 au8Para[10] = {0};
    MS_BOOL bReturn;


    // Send cmd to FRC: Get SW version
    bReturn = XC_SEND_CMD_TO_FRC_R2__0P(pInstance, E_XC_FRC_MB_CMD_GET_SW_VERSION);
    if( bReturn == FALSE )
    {
        printf("%s Send failed!\n", __FUNCTION__);
        return u16SwVersion;
    }

    // Recieve message from FRC:
    bReturn = MHal_XC_GetMsgFromFRC(pInstance, &u8Cmd, &u8ParaCount, au8Para);
    if( bReturn == TRUE )
    {
        //printf("u8Cmd=%X,u8ParaCount=%X\n", u8Cmd, u8ParaCount);
        //printf("p0=%X,P1=%X\n", au8Para[0], au8Para[1]);
        u16SwVersion = (((MS_U16)au8Para[0])<<8) + au8Para[1];
        //printf("FRC_R2 SW version=%X\n", u16SwVersion);
    }
    return u16SwVersion;
}

MS_U16 Hal_XC_FRC_R2_Get_CmdVersion(void *pInstance)
{
    MS_U16 u16CmdVersion = 0;
    MS_U8 u8Cmd;
    MS_U8 u8ParaCount;
    MS_U8 au8Para[10];
    MS_BOOL bReturn;

    memset(au8Para,0,sizeof(au8Para));
    // Send cmd to FRC: Get Cmd version
    bReturn = XC_SEND_CMD_TO_FRC_R2__0P(pInstance, E_XC_FRC_MB_CMD_GET_CMD_VERSION);
    if( bReturn == FALSE )
    {
        printf("%s Send failed!\n", __FUNCTION__);
        return u16CmdVersion;
    }

    // Recieve message from FRC:
    bReturn = MHal_XC_GetMsgFromFRC(pInstance, &u8Cmd, &u8ParaCount, au8Para);
    if( bReturn == TRUE )
    {
        u16CmdVersion = (((MS_U16)au8Para[1])<<8) + au8Para[0];
        //printf("FRC_R2 Cmd version=%X\n", u16CmdVersion);
    }
    return u16CmdVersion;
}

MS_BOOL Hal_XC_FRC_R2_Init(void *pInstance, MS_U8 u8Panel3DType, MS_U8 u8PanelMaxFreq, MS_U8 u8LocalDimingPanelType, MS_U8 u8ChipRevision)
{
    return XC_SEND_CMD_TO_FRC_R2__4P(pInstance, E_XC_FRC_MB_CMD_INIT, u8Panel3DType, u8PanelMaxFreq, u8LocalDimingPanelType, u8ChipRevision);
}

MS_BOOL Hal_XC_FRC_R2_Set_Timing(void *pInstance, MS_U8 u8InVFreq, MS_U8 u8OutVFreq)
{
    return XC_SEND_CMD_TO_FRC_R2__2P(pInstance, E_XC_FRC_MB_CMD_SET_TIMING, u8InVFreq, u8OutVFreq );
}
MS_BOOL Hal_XC_FRC_R2_Set_InputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize )
{
    return XC_SEND_CMD_TO_FRC_R2__4P(pInstance, E_XC_FRC_MB_CMD_SET_INPUT_FRAME_SIZE, HIBYTE(u16HSize), LOBYTE(u16HSize), HIBYTE(u16VSize), LOBYTE(u16VSize));
}
MS_BOOL Hal_XC_FRC_R2_Set_OutputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize )
{
    return XC_SEND_CMD_TO_FRC_R2__4P(pInstance, E_XC_FRC_MB_CMD_SET_OUTPUT_FRAME_SIZE, HIBYTE(u16HSize), LOBYTE(u16HSize), HIBYTE(u16VSize), LOBYTE(u16VSize));
}
MS_BOOL Hal_XC_FRC_R2_Set_FPLL_Lockdone(void *pInstance, MS_BOOL bLockDone)
{
    return XC_SEND_CMD_TO_FRC_R2__1P(pInstance, E_XC_FRC_MB_CMD_SET_FPLL_LOCK_DONE, bLockDone);
}

MS_BOOL Hal_XC_FRC_R2_Enable_MEMC(void *pInstance, MS_U16 u16MEMCLevel)
{

    return XC_SEND_CMD_TO_FRC_R2__1P(pInstance, E_XC_FRC_MB_CMD_SET_MFC_MDOE, u16MEMCLevel);//MEMC level
}

MS_BOOL Hal_XC_FRC_R2_Set_Input3DFormat(void *pInstance, MS_U8 u8FrcInput3DType)
{
    return XC_SEND_CMD_TO_FRC_R2__1P(pInstance, E_XC_FRC_MB_CMD_SET_INPUT_3D_FORMAT, u8FrcInput3DType);
}

MS_BOOL Hal_XC_Get_Stgen_Lock_Ip_status(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    if(SC_R2BYTEMSK(psXCInstPri->u32DeviceID,REG_SC_BK68_20_L,BIT(0)|BIT(2)) == (BIT(0)|BIT(2)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
