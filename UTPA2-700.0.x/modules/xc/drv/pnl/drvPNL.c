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
/// file    drvPNL.c
/// @brief  Panel Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define _DRVPNL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#include <linux/delay.h>
#include <asm/div64.h>
#else
#include "string.h"
#define do_div(x,y) ((x)/=(y))
#endif
// Internal Definition
#include "drvMMIO.h"

#ifndef _DRV_SYS_H_
#include "drvSYS.h"
#endif
#include "utopia.h"
#include "utopia_dapi.h"
#include "apiPNL.h"
#include "apiPNL_v2.h"
#include "drvPNL.h"
#include "halPNL.h"
#include "PNL_private.h"
#include "pnl_hwreg_utility2.h"
#include "halCHIP.h"

#ifdef MSOS_TYPE_LINUX_KERNEL
#if (KERNEL_DRIVER_PATCH ==1)
#if !defined(CONFIG_ARM64)
#include <linux/math64.h>
//__aeabi_uldivmod
unsigned long long __aeabi_uldivmod(unsigned long long n, unsigned long long d)
{
    return div64_u64(n, d);
}

//__aeabi_ldivmod
long long __aeabi_ldivmod(long long n, long long d)
{
    return div64_s64(n, d);
}
#endif
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define UNUSED(x)       (x=x)
#define TCON_TAB_MAX_SIZE  2048

#define NONPM_BASE                  (0x100000)
#define OD_BYTE_PER_WORD   16
#define PANEL_VSTART_OFFSET 8 //8    // for PANEL_DE_VSTART add offset

//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------
static const MS_U8       _cPnlVer[] = PNL_LIB_VERSION;

// Internal Data

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
// Function in mdrv_xc_menuload.c
typedef enum
{
    E_MLOAD_UNSUPPORTED = 0,
    E_MLOAD_DISABLED    = 1,
    E_MLOAD_ENABLED     = 2,
}MLOAD_TYPE;

typedef enum
{
    E_MLG_UNSUPPORTED = 0,
    E_MLG_DISABLED    = 1,
    E_MLG_ENABLED     = 2,
}MLG_TYPE;

extern MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire(MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt);
extern MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire(MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MLOAD_TYPE MApi_XC_MLoad_GetStatus(void);
extern MLG_TYPE MApi_XC_MLG_GetStatus(void);
extern void MApi_XC_MLG_Fire(MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue);
extern MS_BOOL MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub(void);

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
static void _MDrv_PNL_Init_XC_T3D(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if (pstPanelInitData->u32PNL_MISC & E_DRVPNL_MISC_SKIP_T3D_CONTROL)
    {
        //printf("\033[0;31m [%s][%d] *** SKIP T3D control ***  \033[0m\n", __FUNCTION__, __LINE__);
    }
    else
    {
        //Need move those setting to hal to avoid write Reg which is not T3D bank by any chip
        #if 0
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_55_L, pstPanelInitData->u16Width, 0x1FFF);//pixel width
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_66_L, pstPanelInitData->u16Height, 0x1FFF);//reg_col_height
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_51_L, pstPanelInitData->u16Width, 0x1FFF);//reg_ln_width
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_52_L, pstPanelInitData->u16Height, 0x1FFF);//reg_col_height
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_61_L, pstPanelInitData->u16Width, 0x3FFF);//reg_ln_width
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_62_L, pstPanelInitData->u16Height, 0x1FFF);//reg_col_height

        //per designer, should always enable t3d, since it will affect osd/video's pipeline
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_61_L, BIT(0), BIT(0));//Enable Depth Render, for osd pipe line adjustment
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK62_08_L, BIT(4), BIT(4));//mtv bypass mode
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK63_57_L,(BIT(0)|BIT(1)),(BIT(0)|BIT(1)));//T3D fix subde enable, fix for T3D/PIP conflict issue (bit 0)     Bug Fix miu eco (bit 1)
        #endif
        MHal_PNL_Set_T3D_Setting(pInstance);
    }
}

static void _MDrv_PNL_Init_XC_VOP(void *pInstance, PNL_InitData *pstPanelInitData)
{
    //Note: H/V Total, H/V DE End, H/V Syncwidth all need minus 1 when set to register
    MS_U16 u16DE_HEND = pstPanelInitData->u16HStart + pstPanelInitData->u16Width - 1;
    MS_U16 u16DE_HStart = pstPanelInitData->u16HStart;
    MS_U16 u16DE_VEND = pstPanelInitData->u16VStart + pstPanelInitData->u16Height - 1;
    MS_U16 u16DE_VStart = pstPanelInitData->u16VStart;
    MS_U16 u16VsyncStart = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
    {
        u16VsyncStart = pstPanelInitData->u16VTotal - pstPanelInitData->u16VSyncStart +PANEL_VSTART_OFFSET;// vsync start = Vtt - (VSyncWidth + BackPorch)
    }
    else
    {
        u16VsyncStart = pstPanelInitData->u16VTotal - pstPanelInitData->u16VSyncStart ;// vsync start = Vtt - (VSyncWidth + BackPorch)
    }

    // Htotal/Vtotal
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_0C_L, (pstPanelInitData->u16HTotal - 1), VOP_HTT_MASK);         // output htotal
#if defined(CONFIG_MBOOT) || !defined(PATCH_HW_VTT_LIMITATION)
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_0D_L, (pstPanelInitData->u16VTotal - 1), VOP_VTT_MASK);         // output vtotal
#endif
#if defined(PATCH_HW_VTT_LIMITATION)
    if(R1BYTEMSK(REG_CHIP_REVISION,0xFF) < HW_VTT_LIMITATION_CHIPREV)
    {
        MOD_W2BYTEMSK(REG_MOD_BK00_65_L,pstPanelInitData->u16VTotal,0x1FFF);
        MHal_PNL_SetVopVttByBK68(pPNLInstancePrivate->u32DeviceID, (pstPanelInitData->u16VTotal - 1)&VOP_VTT_MASK);
    }
#endif
    // DE H/V start/size
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_04_L, u16DE_HStart, VOP_DE_HSTART_MASK);                                       // DE H start
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_05_L, u16DE_HEND  , VOP_DE_HEND_MASK);  // DE H end
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_06_L, u16DE_VStart, VOP_DE_VSTART_MASK);                                       // DE V start
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_07_L, u16DE_VEND  , VOP_DE_VEND_MASK);  // DE V end
    //If support sc1, init sub vop
    if (MDrv_XC_IsSupportPipPatchUsingSc1MainAsSc0Sub())
    {
        //follow HW_DESIGN_4K2K_VER = 6 rule
        SC_W2BYTEMSK(1, REG_SC_BK10_0C_L, (pstPanelInitData->u16HTotal/2 - 1), VOP_HTT_MASK);
        SC_W2BYTEMSK(1, REG_SC_BK10_0D_L, VOP_VTT_MASK, VOP_VTT_MASK);         // output vtotal

        // DE H/V start/size
        SC_W2BYTEMSK(1, REG_SC_BK10_04_L, u16DE_HStart/2, VOP_DE_HSTART_MASK);                                       // DE H start
        SC_W2BYTEMSK(1, REG_SC_BK10_05_L, u16DE_HStart + pstPanelInitData->u16Width/2 - 1 , VOP_DE_HEND_MASK);  // DE H end
        SC_W2BYTEMSK(1, REG_SC_BK10_06_L, u16DE_VStart, VOP_DE_VSTART_MASK);                                       // DE V start
        //FIX ME: need review
        SC_W2BYTEMSK(1, REG_SC_BK10_07_L, u16DE_VStart+pstPanelInitData->u16Height-1, VOP_DE_VEND_MASK);  // DE V end
    }
    // Display H/V start/size
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_08_L, u16DE_HStart , VOP_DISPLAY_HSTART_MASK);                                               // Display H start
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_09_L, u16DE_HEND   , VOP_DISPLAY_HEND_MASK);  // Display H end
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_0A_L, u16DE_VStart , VOP_DISPLAY_VSTART_MASK);                                               // Display V start
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_0B_L, u16DE_VEND   , VOP_DISPLAY_VEND_MASK);  // Display V end

    // H/V sync start/width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_01_L, (pstPanelInitData->u8HSyncWidth-1), LBMASK);                    // hsync width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_02_L, ((u16VsyncStart & 0x7FF) | ((u16VsyncStart & 0x800) << 1)), 0x17FF);     // vsync start = Vtt - (VSyncWidth + BackPorch)
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_03_L, (u16VsyncStart + pstPanelInitData->u8VSyncWidth - 1), VOP_VSYNC_END_MASK);    // vsync end = Vsync start + Vsync Width
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_10_L, (pstPanelInitData->bManuelVSyncCtrl << 15), BIT(15));                                  // manual vsync control
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_10_L, BIT(14), BIT(14));       //default set to mode1                                                                       // disable auto_htotal
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_23_L, BIT(7), BIT(7));

    // output control
    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_21_L, pstPanelInitData->u16OCTRL);

    if (pstPanelInitData->eLPLL_Type == E_PNL_TYPE_LVDS ||
        pstPanelInitData->eLPLL_Type == E_PNL_TYPE_DAC_I ||
        pstPanelInitData->eLPLL_Type == E_PNL_TYPE_DAC_P)
    {
        // LVDS
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_22_L, 0x11, LBMASK);  //   0x2f44 : Enable clock of internal control - LVDS or TTL Output
    }
    else
    {
        // RSDS
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_22_L, 0x00, LBMASK);
    }

    SC_W2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_46_L, pstPanelInitData->u16OSTRL);

    // output driving current
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_47_L, pstPanelInitData->u16ODRV, LBMASK);
}

MS_U16 MDrv_PNL_GetDefaultVfreq(void *pInstance, MS_U16 u16Vfreq)
{
    MS_U16 u16DefaultVFreq = 0;

    UNUSED(pInstance);

    if(u16Vfreq > 2100)
    {
        u16DefaultVFreq = 2400;
    }
    else if(u16Vfreq > 1900)
    {
        u16DefaultVFreq = 2000;
    }
    else if(u16Vfreq > 1100)
    {
        u16DefaultVFreq = 1200;
    }
    else if(u16Vfreq > 900)
    {
        u16DefaultVFreq = 1000;
    }
    else if(u16Vfreq > 550)
    {
        u16DefaultVFreq = 600;
    }
    else if(u16Vfreq > 350)
    {
        u16DefaultVFreq = 500;
    }
    else if(u16Vfreq > 250)
    {
        u16DefaultVFreq = 300;
    }
    else if(u16Vfreq > 245)
    {
        u16DefaultVFreq = 250;
    }
    else if(u16Vfreq > 235)
    {
        u16DefaultVFreq = 240;
    }
    else
    {
        u16DefaultVFreq = 150;
    }

    return u16DefaultVFreq;
}

static void _MDrv_PNL_Init_LPLL(void *pInstance, PNL_InitData *pstPanelInitData)
{
    MS_U64 ldHz = 1;
    MS_U16 u16DefaultVFreq = 0;

    MS_U16 u16HTotal = pstPanelInitData->u16HTotal;
    MS_U16 u16VTotal = pstPanelInitData->u16VTotal;

    u16DefaultVFreq = MDrv_PNL_GetDefaultVfreq(pInstance, pstPanelInitData->u16DefaultVFreq);

    if(((pstPanelInitData->u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE) || (pstPanelInitData->u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE_60HZ))
        &&(pstPanelInitData->u16Width == 1280)
        &&(pstPanelInitData->u16Height == 720))
    {
        u16HTotal = 2200;
        u16VTotal = 1130;
    }

    printf("[%s][%d]pstPanelInitData->u16Width=%u, pstPanelInitData->u16Height=%u\n",
        __FUNCTION__,__LINE__,pstPanelInitData->u16Width, pstPanelInitData->u16Height);
    printf("[%s][%d]u16HTotal=%u,u16VTotal=%u,pstPanelInitData->u16HTotal=%u,pstPanelInitData->u16VTotal=%u, u16DefaultVFreq=%u\n",
        __FUNCTION__,__LINE__,u16HTotal,u16VTotal,pstPanelInitData->u16HTotal,pstPanelInitData->u16VTotal,u16DefaultVFreq);

    ldHz = (MS_U64) (((MS_U64)u16HTotal) * (MS_U64)u16VTotal * (MS_U64)u16DefaultVFreq);

    MHal_PNL_Init_LPLL(pInstance, pstPanelInitData->eLPLL_Type,pstPanelInitData->eLPLL_Mode,ldHz);

    if ((pstPanelInitData->u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE) ||
        (pstPanelInitData->u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE_60HZ) ||
        (E_PNL_LPLL_VBY1_10BIT_8LANE == pstPanelInitData->eLPLL_Type)) // Napoli VB1_8lane default use FRC, so need 2 LPLL
    {
        MHal_PNL_FRC_lpll_src_sel(pInstance, 0);
    }
    else
    {
        MHal_PNL_FRC_lpll_src_sel(pInstance, 1);
    }

}

static MS_U64 _MDrv_PNL_DClkFactor(void *pInstance, PNL_MODE eLPLL_Mode, PNL_TYPE eLPLL_Type, MS_U64 ldHz)
{
    return (((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * MHal_PNL_Get_LPLL_LoopGain(pInstance, eLPLL_Mode, eLPLL_Type, ldHz)));  //216*524288*8=0x36000000
}

static void _MDrv_PNL_Init_Output_Dclk(void *pInstance, PNL_InitData *pstPanelInitData)
{
    MS_U32 u32OutputDclk;
    MS_U16 u16DefaultVFreq = 0;
    MS_U64 ldHz = 1;

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    MS_U16 u16HTotal = pstPanelInitData->u16HTotal;
    MS_U16 u16VTotal = pstPanelInitData->u16VTotal;

    u16DefaultVFreq = MDrv_PNL_GetDefaultVfreq(pInstance, pstPanelInitData->u16DefaultVFreq);

    /// for A5/Agate FRC Active  FPP 720P
    if(((pstPanelInitData->u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE) || (pstPanelInitData->u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE_60HZ))
        &&(pstPanelInitData->u16Width == 1280)
        &&(pstPanelInitData->u16Height== 720))
    {
        u16HTotal = 2200;
        u16VTotal = 1130;
    }
    printf("[%s][%d]pstPanelInitData->u16Width=%u, pstPanelInitData->u16Height=%u\n",
        __FUNCTION__,__LINE__,pstPanelInitData->u16Width, pstPanelInitData->u16Height);
    printf("[%s][%d]u16HTotal=%u,u16VTotal=%u,pstPanelInitData->u16HTotal=%u,pstPanelInitData->u16VTotal=%u, u16DefaultVFreq=%u\n",
        __FUNCTION__,__LINE__,u16HTotal,u16VTotal,pstPanelInitData->u16HTotal,pstPanelInitData->u16VTotal,u16DefaultVFreq);

    ldHz = (MS_U64) (((MS_U64)u16HTotal) * u16VTotal * u16DefaultVFreq);

    u32OutputDclk = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)ldHz, FALSE);

    //printf("_MDrv_PNL_Init_Output_Dclk u32div = %lu, u32LpllSet = %lx\n", (MS_U32)u32div, (MS_U32)ldPllSet);

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    W2BYTEMSK(L_BK_LPLL(0x0C), FALSE, BIT(3));
    W4BYTE(L_BK_LPLL(0x0F), u32OutputDclk);

    if((pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized == 0)&&
       (pPNLResourcePrivate->stapiPNL._bSkipTimingChange == FALSE))
    {
        MHal_PNL_ChannelFIFOPointerADjust(pInstance);
    }
#if SUPPORT_FRC
    ldHz = (MS_U64) (((MS_U64)pstPanelInitData->u16HTotal) * pstPanelInitData->u16VTotal * u16DefaultVFreq);

    Mhal_PNL_Flock_LPLLSet(pInstance, ldHz);
#endif
}


/*static void _MDrv_PNL_TCON_GetTabSize(MS_U8 *pTconTab, MS_U16 u16tabtype, MS_U32 *pu32TconTabsize)
{
    MS_U8 u8Colslen = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U16 u16Addr;
    if (pTconTab == NULL)
    {
        PNL_ASSERT(0, "[%s] TCONTable error \n.", __FUNCTION__);
        return;
    }
    switch(u16tabtype )
    {
    case TCON_TABTYPE_GENERAL:
    case TCON_TABTYPE_GPIO:
    case TCON_TABTYPE_SCALER:
    case TCON_TABTYPE_MOD:
    case TCON_TABTYPE_GAMMA:
        u8Colslen = 4;
        break;
    case TCON_TABTYPE_POWER_SEQUENCE_ON:
    case TCON_TABTYPE_POWER_SEQUENCE_OFF:
        u8Colslen = 7;
        break;
    default:
        PNL_ASSERT(0, "[%s] GetTable Size :unknown Tab Size \n.", __FUNCTION__);
        return ;
    }
    while (--u16timeout)
    {
        u16Addr = (pTconTab[*pu32TconTabsize]<<8) + pTconTab[(*pu32TconTabsize +1)];
        if (u16Addr == REG_TABLE_END) // check end of table
        {
            *pu32TconTabsize = *pu32TconTabsize + 4;
            break;
        }
        *pu32TconTabsize = *pu32TconTabsize + u8Colslen;
    }
    if (u16timeout==0)
    {
        PNL_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }

    PNL_DBG(PNL_DBGLEVEL_INIT, "<<*pu32TconTabsize= %ld>>\n", *pu32TconTabsize);

    if(*pu32TconTabsize > TCON_TAB_MAX_SIZE)
    {
        PNL_ASSERT(0, "[%s] Tab size too large than TCON_TAB_MAX_SIZE = %d \n.", __FUNCTION__, TCON_TAB_MAX_SIZE);
    }
}*/

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
const MS_U8 * MDrv_PNL_GetLibVer(void)              ///< Get version
{
    return _cPnlVer;
}

ST_PNL_DRV_INFO MDrv_PNL_GetInfo(void *pInstance)          ///< Get info from driver
{
    ST_PNL_DRV_INFO stPNLInfo = {GAMMA_TYPE, GAMMA_MAPPING};

    UNUSED(pInstance);

    return stPNLInfo;
}

MS_BOOL MDrv_PNL_Control_Out_Swing(void *pInstance, MS_U16 u16Swing_Level)
{
    return (MS_BOOL)MHal_PNL_MOD_Control_Out_Swing(pInstance, u16Swing_Level);
}

PNL_Result MDrv_PNL_PreInit(void *pInstance, PNL_OUTPUT_MODE eParam)
{
    if ( eParam == E_PNL_OUTPUT_MAX )
        return E_PNL_FAIL;

    MHal_PNL_PreInit(pInstance, eParam);

    return E_PNL_OK;
}

// Output Dclk
MS_U32 MDrv_PNL_CalculateLPLLSETbyDClk(void *pInstance, MS_U64 u64LdHz, MS_BOOL bHighAccurate)
{

    MS_U64 u64LdPllSet = 0;
    MS_U32 u32Div = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    // loop div and loop gain use default parameters to avoid dclk floating out of range and getting wrong value
    MS_U64 u64DefaultLdHz = (MS_U64) (((MS_U64)pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16HTotal)
                                       * pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16VTotal
                                       * pPNLResourcePrivate->stdrvPNL._stPnlInitData.u16DefaultVFreq);
#if defined (__aarch64__)
    PNL_DBG(PNL_DBGLEVEL_INIT, "CalculateLPLLSETbyDClk u32KHz = %lu, u64DefaultLdHz = %lu\n", u64LdHz, u64DefaultLdHz);
#else
    PNL_DBG(PNL_DBGLEVEL_INIT, "CalculateLPLLSETbyDClk u32KHz = %llu, u64DefaultLdHz = %llu\n", u64LdHz, u64DefaultLdHz);
#endif

    #if (HW_DESIGN_HDMITX_VER == 2)
    if ((E_PNL_TYPE_DAC_P == pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) || (E_PNL_TYPE_DAC_I == pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type))
    {
        // For Maxim HDMITx different color depth, need to use u64LdHz to parse loop_div
        if (bHighAccurate)
            u64DefaultLdHz = u64LdHz/100;
        else
            u64DefaultLdHz = u64LdHz;
    }
    #endif

#if (PNL_SUPPORT_2P_MODE == TRUE)
    /// Mini LVDS, EPI34/28, LVDS_1CH, Vx1_1P are 1P structure
    if(!((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_TTL)||
        ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_LVDS)&&(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode==E_PNL_MODE_SINGLE))||
        ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_HS_LVDS)&&(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode==E_PNL_MODE_SINGLE))||
        (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_1LANE)||(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_1LANE)||
        ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type >= E_PNL_LPLL_MINILVDS_2CH_3P_8BIT)&&(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type <= E_PNL_LPLL_MINILVDS_1CH_6P_6BIT))||
        ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type >= E_PNL_LPLL_EPI34_2P)&&(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type <= E_PNL_LPLL_EPI28_4P))))
    {
        u64LdHz = u64LdHz/2;
    }
#endif

    MS_U32 u32Factor = 10;

    if (bHighAccurate)
    {
        u32Factor = 1000;
    }

    switch(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type)
    {

        case E_PNL_TYPE_MINILVDS:
        case E_PNL_TYPE_ANALOG_MINILVDS:
        case E_PNL_TYPE_DIGITAL_MINILVDS:
        case E_PNL_LPLL_MINILVDS_5P_2L:
        case E_PNL_LPLL_MINILVDS_4P_2L:
        case E_PNL_LPLL_MINILVDS_3P_2L:
        case E_PNL_LPLL_MINILVDS_6P_1L:
        case E_PNL_LPLL_MINILVDS_5P_1L:
        case E_PNL_LPLL_MINILVDS_4P_1L:
        case E_PNL_LPLL_MINILVDS_3P_1L:

        case E_PNL_LPLL_MINILVDS_1CH_3P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_4P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_5P_6BIT:
        case E_PNL_LPLL_MINILVDS_1CH_6P_6BIT:

        case E_PNL_LPLL_MINILVDS_2CH_3P_6BIT:
        case E_PNL_LPLL_MINILVDS_2CH_4P_6BIT:
        case E_PNL_LPLL_MINILVDS_2CH_5P_6BIT:
        case E_PNL_LPLL_MINILVDS_2CH_6P_6BIT:

        case E_PNL_LPLL_MINILVDS_1CH_3P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_4P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_5P_8BIT:
        case E_PNL_LPLL_MINILVDS_1CH_6P_8BIT:

        case E_PNL_LPLL_MINILVDS_2CH_3P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_4P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_5P_8BIT:
        case E_PNL_LPLL_MINILVDS_2CH_6P_8BIT:

        case E_PNL_LPLL_EPI34_8P:
        case E_PNL_LPLL_EPI28_8P:
        case E_PNL_LPLL_EPI34_6P:
        case E_PNL_LPLL_EPI28_6P:

        case E_PNL_TYPE_TTL_TCON:
        case E_PNL_TYPE_TTL:
        case E_PNL_TYPE_LVDS:
        case E_PNL_TYPE_RSDS:
        case E_PNL_TYPE_MFC:
        case E_PNL_TYPE_DAC_I:
        case E_PNL_TYPE_DAC_P:
        case E_PNL_TYPE_PDPLVDS:
        case E_PNL_TYPE_HF_LVDS:
        case E_PNL_TYPE_HS_LVDS:
        case E_PNL_LPLL_HDMI_BYPASS_MODE:
        case E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE:
        case E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE:
        default:
            //the first " *2 " is from  the dual mode
            u32Div=(MS_U32)(MHal_PNL_Get_Loop_DIV(pInstance, pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode, pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type, u64DefaultLdHz));
            u64LdPllSet = (_MDrv_PNL_DClkFactor(pInstance, pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Mode, pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type, u64DefaultLdHz) * 1000000 * u32Factor *2) + ((u64LdHz * u32Div) >> 1);
            if(u32Div != 0)
            {
#if (KERNEL_DRIVER_PATCH ==1)
                u64LdPllSet /= u64LdHz;
#else
                do_div(u64LdPllSet, u64LdHz);
#endif
                do_div(u64LdPllSet, u32Div);
            }
            break;
    }

    PNL_DBG(PNL_DBGLEVEL_INIT, "CalculateLPLLSETbyDClk u32LpllSet = %tx\n", (ptrdiff_t)u64LdPllSet);
    return (MS_U32)u64LdPllSet;

}

PNL_Result MDrv_PNL_Init(void *pInstance)
{
    MS_PHY u32NonPMBankSize, u32PMBankSize;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &g_ptr_PnlRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_PNL ) != TRUE)
    {
        PNL_ASSERT(0, "%s %lx %lx\n", "Get base address failed", g_ptr_PnlRiuBaseAddr, u32NonPMBankSize);
        return E_PNL_GET_BASEADDR_FAIL;
    }
    else
    {
#if defined (__aarch64__)
        printf("MDrv_PNL_Init u32PnlRiuBaseAddr = %lx\n", g_ptr_PnlRiuBaseAddr);
#else
        printf("MDrv_PNL_Init u32PnlRiuBaseAddr = %tx\n", (ptrdiff_t)g_ptr_PnlRiuBaseAddr);
#endif
    }

    if(MDrv_MMIO_GetBASE( &g_ptr_PMRiuBaseAddr, &u32PMBankSize, MS_MODULE_PM ) != TRUE)
    {
        PNL_ASSERT(0, "%s %lx %lx\n", "Get PM base address failed", g_ptr_PMRiuBaseAddr, u32PMBankSize);
        return E_PNL_GET_BASEADDR_FAIL;
    }
    else
    {
#if defined (__aarch64__)
        printf("MDrv_PNL_Init u32PnlRiuBaseAddr = %lx\n", g_ptr_PnlRiuBaseAddr);
#else
        printf("MDrv_PNL_Init u32PMRiuBaseAddr = %tx\n",(ptrdiff_t) g_ptr_PMRiuBaseAddr);
#endif
    }

    MHal_PNL_Set_Device_Bank_Offset(pInstance);
    MHal_PQ_Clock_Gen_For_Gamma(pInstance);
    MHal_PNL_Init(pInstance);

    return E_PNL_OK;
}

PNL_Result MDrv_PNL_SetPanelType(void *pInstance, PNL_InitData *pstPanelInitData)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    if(
    ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_TYPE_TTL_TCON)||
    ((pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type >= E_PNL_LPLL_MINILVDS_5P_2L)&&
    (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type <= E_PNL_LPLL_MINILVDS_1CH_6P_6BIT))))
    {
        if(MDrv_SYS_Query(E_SYS_QUERY_TCON_SUPPORTED) == FALSE)
        {
            printf("[%s][%d] ATTENTION!!! TCON PNL but not support TCON\n\n", __FUNCTION__, __LINE__);
            return E_PNL_FAIL;
        }
    }

    if (pPNLResourcePrivate->stdrvPNL._stPnlInitData.u32PNL_MISC == E_DRVPNL_MISC_MFC_ENABLE)
    {
        // For FRC case, check efuse about 120Hz panel support or not
        if (MHal_PNL_Is_Support120Hz() == FALSE)
        {
            printf("[%s][%d] Only support 60Hz panel\n\n", __FUNCTION__, __LINE__);
            return E_PNL_FAIL;
        }
    }

    if(   ( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )||
          (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
          (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        if(MHal_PNL_VBY1_IsSupport_Hardware_TrainingMode(pInstance)!= TRUE)
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En = FALSE;
            printf("[%s][%d] ATTENTION!!! Not support HW Training Mode\n\n", __FUNCTION__, __LINE__);
        }

        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En)
        {
            if((pPNLResourcePrivate->stapiPNL._stPnlStatus.bPanel_Initialized == 0)&&
               (pPNLResourcePrivate->stapiPNL._bSkipTimingChange == FALSE))
            {
                MOD_W2BYTE(REG_MOD_BK00_60_L, 0x0000);
            }
        }
        else
        {
            //Close the RT51 handshack code flow
            MOD_W2BYTEMSK(REG_MOD_BK00_44_L, BIT(14), BIT(14));
        }
    }

    if(pPNLResourcePrivate->stapiPNL._bSkipTimingChange == FALSE)
    {
        MHal_PNL_Init_XC_Clk(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);
    }
    _MDrv_PNL_Init_XC_VOP(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);
    _MDrv_PNL_Init_XC_T3D(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);
    MHal_PNL_Init_MOD(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);
    _MDrv_PNL_Init_LPLL(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);
    _MDrv_PNL_Init_Output_Dclk(pInstance, &pPNLResourcePrivate->stdrvPNL._stPnlInitData);



    if(   ( IsVBY1(pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type) )||
          (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
          (pPNLResourcePrivate->stdrvPNL._stPnlInitData.eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        if(pPNLResourcePrivate->stdrvPNL._stPnlInitData.bVideo_HW_Training_En)
        {
//            MOD_W2BYTEMSK(REG_MOD_BK00_63_L, 0x8000, 0x8000);
            //MHal_PNL_VBY1_Hardware_TrainingMode_En(pInstance, TRUE ,DISABLE);
            MsOS_DelayTaskUs(100);
            MHal_PNL_VBY1_Hardware_TrainingMode_En(pInstance, TRUE ,ENABLE);
//            MsOS_DelayTaskUs(10);
//            MOD_W2BYTEMSK(REG_MOD_BK00_63_L, 0x0000, 0x8000);
        }
        else
        {
            //Open the RT51 handshack code flow
            MOD_W2BYTEMSK(REG_MOD_BK00_44_L, 0, BIT(14));
        }
    }

    return E_PNL_OK;
}

PNL_Result MDrv_PNL_Close(void *pInstance)
{
    UNUSED(pInstance);
    return E_PNL_OK;
}

static void _MDrv_VOP_SetGammaMappingMode(void *pInstance, MS_U8 u8Mapping)
{
    MHal_VOP_SetGammaMappingMode(pInstance, u8Mapping);
}

/// set gamma table
PNL_Result MDrv_PNL_SetGammaTbl(void *pInstance, DRVPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3],
                                 DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode)
{
    MS_U8 u8TgtChannel = 0;
    MS_U8* pcTab1;
    MS_U8 u8BackupMainSubEnableFlag = 0;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    UNUSED(eGammaType);

    // delay 2ms to wait LPLL stable (if not stable, it will make gamma value incorrect)
#ifdef MSOS_TYPE_LINUX_KERNEL
    mdelay(2);
#else
    MsOS_DelayTask(2);
#endif


    // If gamma is on, turn if off
    //if(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, BIT(0)))
    if((SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, BIT(0)))&&(MApi_XC_MLG_GetStatus() != E_MLG_ENABLED))
    {
        if(MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
        {
            MS_U32 u32Addr[2];
            MS_U16 u16Data[2], u16Mask[2];
            printf("MApi_XC_MLoad_GetStatus()\n");
            // Disable Gamma of MainWindow
            u32Addr[0] = 0x10A0;
            u16Data[0] = 0x0000;
            u16Mask[0] = 0x0001;
            // Disable Gamma of SubWindow
            u32Addr[1] = 0x0F30;
            u16Data[1] = 0x0000;
            u16Mask[1] = 0x0001;
            MApi_XC_MLoad_WriteCmds_And_Fire(u32Addr ,u16Data ,u16Mask , 2);
        }
        else
        {

            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, FALSE, BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK0F_18_L, FALSE, BIT(0));
        }

        u8BackupMainSubEnableFlag |= BIT(0);
    }
#ifdef USE_PANEL_GAMMA
    else if (SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_26_L, BIT(3)))
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_26_L, FALSE, BIT(3));
        u8BackupMainSubEnableFlag |= BIT(3);
    }
#endif


    _MDrv_VOP_SetGammaMappingMode(pInstance, (GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING) ? GAMMA_8BIT_MAPPING : GAMMA_10BIT_MAPPING);

    if  (MApi_XC_MLG_GetStatus() == E_MLG_ENABLED)
    {
        MS_U8 u8Channel;
        MS_U16 pMaxGammaValue[3] ;
        MS_U16 u16NumOfLevel = ((GammaMapMode == E_DRVPNL_GAMMA_8BIT_MAPPING) ? 256 : 1024);
        pMaxGammaValue[0] = pMaxGammaValue[1] = pMaxGammaValue[2] = 0;
        MApi_XC_MLG_Fire(pu8GammaTab[0], pu8GammaTab[1], pu8GammaTab[2], u16NumOfLevel, pMaxGammaValue);
        // write max. value of gamma
        for(u8Channel = 0; u8Channel < 3; u8Channel ++)
        {
            hal_PNL_SetMaxGammaValue(pInstance, u8Channel, pMaxGammaValue[u8Channel]);
        }
    }
    else
    {

        while( u8TgtChannel < 3 )   // 0 for R, 1 for G, 2 for B
        {
            pcTab1 = pu8GammaTab[u8TgtChannel];
#ifdef MONACO_SC2
            if(pPNLInstancePrivate->u32DeviceID == 0)
#endif
                Hal_PNL_Set12BitGammaPerChannel(pInstance, u8TgtChannel, pcTab1, GammaMapMode);
#ifdef MONACO_SC2
            else
                Hal_PNL_Set12BitGammaPerChannel_SC2(pInstance, u8TgtChannel, pcTab1, GammaMapMode);
#endif
            u8TgtChannel++;
        }
    }

    // Recovery gamma table
    //if( u8BackupMainSubEnableFlag & BIT(0) )
    if(( u8BackupMainSubEnableFlag & BIT(0) )&&(MApi_XC_MLG_GetStatus() != E_MLG_ENABLED))
    {

        if(MApi_XC_MLoad_GetStatus() == E_MLOAD_ENABLED)
        {
            MS_U32 u32Addr[2];
            MS_U16 u16Data[2], u16Mask[2];

            // Enable Gamma of MainWindow
            u32Addr[0] = 0x10A0;
            u16Data[0] = 0x0001;
            u16Mask[0] = 0x0001;
            // Enable Gamma of SubWindow
            u32Addr[1] = 0x0F30;
            u16Data[1] = 0x0001;
            u16Mask[1] = 0x0001;
            MApi_XC_MLoad_WriteCmds_And_Fire(u32Addr ,u16Data ,u16Mask , 2);
        }
        else
        {
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, BIT(0), BIT(0));
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK0F_18_L, BIT(0), BIT(0));
        }
    }
    else if ( u8BackupMainSubEnableFlag & BIT(3) )
    {
        #ifdef USE_PANEL_GAMMA
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK24_26_L, BIT(3), BIT(3));
        #endif
    }
    return E_PNL_OK;
}



PNL_Result MDrv_PNL_GetGammaTbl(void *pInstance, DRVPNL_GAMMA_TYPE eGammaType, MS_U8* pu8GammaTab[3],
                                 DRVPNL_GAMMA_MAPPEING_MODE GammaMapMode)
{
    MS_U8 u8TgtChannel = 0;
    MS_U8* pcTab1;
    while( u8TgtChannel < 3 )   // 0 for R, 1 for G, 2 for B
    {
        pcTab1 = pu8GammaTab[u8TgtChannel];
        Hal_PNL_Get12BitGammaPerChannel(pInstance, u8TgtChannel, pcTab1, GammaMapMode);
        u8TgtChannel++;
    }
    return E_PNL_OK;
}


PNL_Result MDrv_PNL_SetGammaValue(void *pInstance, MS_U8 u8Channel, MS_U16 u16Offset, MS_U16 u16GammaValue)
{
    MS_U16 u16MaxGammaValue;
    MS_U8 u8BackupMainSubEnableFlag = 0;
    MS_U16 u16MaxOffset = 0;
    MS_BOOL bUsingBurstWrite = FALSE;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    // delay 2ms to wait LPLL stable (if not stable, it will make gamma value incorrect)
#ifdef MSOS_TYPE_LINUX_KERNEL
    mdelay(2);
#else
    MsOS_DelayTask(2);
#endif

    if (u8Channel > 2)
    {
        return E_PNL_FAIL;
    }

    if (Hal_VOP_Is_GammaMappingMode_enable(pInstance))
    {
        u16MaxOffset = 1024;
        // Check write mode
        bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance, E_DRVPNL_GAMMA_10BIT_MAPPING);
    }
    else
    {
        u16MaxOffset = 256;
        bUsingBurstWrite = !Hal_VOP_Is_GammaSupportSignalWrite(pInstance, E_DRVPNL_GAMMA_8BIT_MAPPING);
    }

    if (u16Offset >= u16MaxOffset)
    {
        return E_PNL_FAIL;
    }

    // If gamma is on, turn if off
    if(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, BIT(0)))
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, FALSE, BIT(0));
        u8BackupMainSubEnableFlag |= BIT(0);
    }

    hal_PNL_WriteGamma12Bit(pInstance, u8Channel,bUsingBurstWrite, u16Offset, u16GammaValue);

    u16MaxGammaValue = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_7A_L + 2 * u8Channel), 0xFFF);
    if (u16GammaValue > u16MaxGammaValue)
    {
        u16MaxGammaValue = u16GammaValue;
    }

    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_7A_L + 4 * u8Channel), u16MaxGammaValue, 0xFFF);           // max. base 0
    SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, (REG_SC_BK10_7B_L + 4 * u8Channel), u16MaxGammaValue, 0xFFF);           // max. base 1

    // Recovery gamma table
    if( u8BackupMainSubEnableFlag & BIT(0) )
    {
        SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_50_L, BIT(0), BIT(0));
    }


    return E_PNL_OK;
}

PNL_Result MDrv_PNL_En(void *pInstance, MS_BOOL bPanelOn, MS_BOOL bCalEn)
{
    return MHal_PNL_En(pInstance, bPanelOn, bCalEn);
}

//-----------------------------------------------------------------------------
// SSC
//-----------------------------------------------------------------------------
// SPAN value, recommend value is 20KHz ~ 40KHz
// STEP percent value, recommend is under 3%

// recommend value.
// u16Periodx100Hz == 350, u16Percentx100 == 200
PNL_Result MDrv_PNL_SetSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    MS_U16 u16Span;
    MS_U16 u16Step;
    MS_U32 u32PLL_SET;/// = MDrv_Read3Byte(L_BK_LPLL(0x0F));


    PNL_DBG(PNL_DBGLEVEL_SSC, "[%s][%d]\n", __FUNCTION__, __LINE__);
    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x0F));
    // Set SPAN
    if(u16Fmodulation < 200 || u16Fmodulation > 400)
        u16Fmodulation = 300;
    u16Span =(((((MS_U64)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR ) / (u16Fmodulation) ) * 10000) / ((MS_U32)u32PLL_SET) ) ;

    // Set STEP
    if(u16Rdeviation > 300)
        u16Rdeviation = 300;
    u16Step = ((MS_U32)u32PLL_SET*u16Rdeviation) / ((MS_U32)u16Span*10000);

    W2BYTE(L_BK_LPLL(0x17), u16Step & 0x03FF);// LPLL_STEP
    W2BYTE(L_BK_LPLL(0x18), u16Span & 0x3FFF);// LPLL_SPAN
    W2BYTEMSK((L_BK_LPLL(0x0D)), (bEnable << 11), BIT(11)); // Enable ssc


    return E_PNL_OK;
}


//-----------------------------------------------------------------------------
// OSD SSC
//-----------------------------------------------------------------------------
// SPAN value, recommend value is 20KHz ~ 40KHz
// STEP percent value, recommend is under 3%
// recommend value.
// u16Periodx100Hz == 350, u16Percentx100 == 200
PNL_Result MDrv_PNL_SetOSDSSC(void *pInstance, MS_U16 u16Fmodulation, MS_U16 u16Rdeviation, MS_BOOL bEnable)
{
    MHal_PNL_SetOSDSSC(pInstance, u16Fmodulation, u16Rdeviation, bEnable);
    return E_PNL_OK;
}

void MDrv_PNL_SetOutputPattern(void *pInstance, MS_BOOL bEnable, MS_U16 u16Red , MS_U16 u16Green, MS_U16 u16Blue)
{

    //printf("MDrv_PNL_SetOutputPattern: bEnable = %u, R/G/B=0x%x, 0x%x, 0x%x\n", bEnable, u16Red , u16Green, u16Blue);
    MHal_PNL_SetOutputPattern(pInstance, bEnable, u16Red , u16Green, u16Blue);
}

void MDrv_PNL_SetOutputType(void *pInstance, PNL_OUTPUT_MODE eOutputMode, PNL_TYPE eLPLL_Type)
{
    MHal_PNL_SetOutputType(pInstance, eOutputMode, eLPLL_Type);
}

#define REG_TABLE_END   0xFFFFFFFF
void MDrv_PNL_TCON_DumpSCRegTab(void *pInstance, MS_U8* pu8TconTab)
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;
//    MS_BOOL bHiByte;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    PNL_DBG(PNL_DBGLEVEL_INIT, "MDrv_PNL_TCON_DumpSCRegTab\n");
    PNL_DBG(PNL_DBGLEVEL_INIT, "tab: sc \n");

    if (pu8TconTab == NULL)
    {
        PNL_ASSERT(0, "[%s] TCONTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((pu8TconTab[u32tabIdx]<<24) + (pu8TconTab[(u32tabIdx +1)]<<16) + (pu8TconTab[(u32tabIdx +2)]<<8) + pu8TconTab[(u32tabIdx +3)]) & 0xFFFFFFFF;
        u16Mask  = pu8TconTab[(u32tabIdx +4)] & 0xFF;
        u16Value = pu8TconTab[(u32tabIdx +5)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
        break;

#if 1
        W1BYTEMSK(u32Addr, u16Value, u16Mask);
#else
        bHiByte = (u32Addr & 0x8000) ? TRUE : FALSE;
        u32Addr = ((u32Addr & 0x7F00) | ((u32Addr & 0xFF) << 1));

        //PNL_DBG(PNL_DBGLEVEL_INIT, "[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);
        //PNL_DBG(PNL_DBGLEVEL_INIT, "[addr=%04lx, msk=%02x, val=%02x] \n", BK_SC((u32Addr & 0x00FF))|bHiByte, u16Mask, u16Value);

        if (bHiByte)
        {
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, u32Addr, (u16Value << 8), (u16Mask << 8));
        }
        else
        {
            SC_W2BYTEMSK(pPNLInstancePrivate->u32DeviceID, u32Addr, u16Value, u16Mask);
        }
#endif
        u32tabIdx = u32tabIdx + 6;
    }
    if (u16timeout==0)
    {
        PNL_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "MDrv_PNL_TCON_DumpSCRegTab end\n");
}

void MDrv_PNL_TCON_DumpMODRegTab(void *pInstance, MS_U8* pu8TconTab)
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;
//    MS_BOOL bHiByte;
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));

    PNL_DBG(PNL_DBGLEVEL_INIT, "MDrv_PNL_TCON_DumpMODRegTab\n");

    PNL_DBG(PNL_DBGLEVEL_INIT, "tab: mod \n");

    if (pu8TconTab == NULL)
    {
        PNL_ASSERT(0, "[%s] TCONTable error \n.", __FUNCTION__);
        return;
    }

    pPNLResourcePrivate->stdrvPNL.g_bDrvPnl_UseTConTable_Mod = TRUE;

    while (--u16timeout)
    {
        u32Addr = ((pu8TconTab[u32tabIdx]<<24) + (pu8TconTab[(u32tabIdx +1)]<<16) + (pu8TconTab[(u32tabIdx +2)]<<8) + pu8TconTab[(u32tabIdx +3)]) & 0xFFFFFFFF;
        u16Mask  = pu8TconTab[(u32tabIdx +4)] & 0xFF;
        u16Value = pu8TconTab[(u32tabIdx +5)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
        break;

        //printf("[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);

        PNL_DBG(PNL_DBGLEVEL_INIT, "[addr=%04tx, msk=%02x, val=%02x] \n",(ptrdiff_t) u32Addr, u16Mask, u16Value);

        W1BYTEMSK(u32Addr, u16Value, u16Mask);

        u32tabIdx = u32tabIdx + 6;
    }

    if (u16timeout==0)
    {
        PNL_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
    /// Add the TCON patch at here for different chip
    MHal_PNL_TCON_Patch();

    PNL_DBG(PNL_DBGLEVEL_INIT, "MDrv_PNL_TCON_DumpMODRegTab END: REG_MOD_BK00_6E_L=%X\n", MOD_R2BYTE(REG_MOD_BK00_6E_L));
}

void MDrv_PNL_TCON_DumpGENRegTab(void *pInstance, MS_U8* pu8TconTab)
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U8 u8Mask;
    MS_U8 u8Value;
    MS_U16 u16TconSubBank;

    PNL_DBG(PNL_DBGLEVEL_INIT, "MDrv_PNL_TCON_DumpGENRegTab\n");

    PNL_DBG(PNL_DBGLEVEL_INIT, "tab: gen \n");

    if (pu8TconTab == NULL)
    {
        PNL_ASSERT(0, "[%s] TCONTable error \n.", __FUNCTION__);
        return;
    }

    u16TconSubBank = MHal_PNL_Read_TCON_SubBank(pInstance);
    PNL_DBG(PNL_DBGLEVEL_INIT, "u16TconSubBank=%x\n", u16TconSubBank);

    while (--u16timeout)
    {
        u32Addr = ((pu8TconTab[u32tabIdx]<<24) + (pu8TconTab[(u32tabIdx +1)]<<16) + (pu8TconTab[(u32tabIdx +2)]<<8) + pu8TconTab[(u32tabIdx +3)]) & 0xFFFFFFFF;
        u8Mask  = pu8TconTab[(u32tabIdx +4)] & 0xFF;
        u8Value = pu8TconTab[(u32tabIdx +5)] & 0xFF;

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        PNL_DBG(PNL_DBGLEVEL_INIT, "[addr=%04tx, msk=%02x, val=%02x] \n", (ptrdiff_t)u32Addr, u8Mask, u8Value);

        W1BYTEMSK(u32Addr, u8Value, u8Mask);

        u32tabIdx = u32tabIdx + 6;
    }

    MHal_PNL_Switch_TCON_SubBank(pInstance, u16TconSubBank);

    if (u16timeout==0)
    {
        PNL_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
    PNL_DBG(PNL_DBGLEVEL_INIT, "MDrv_PNL_TCON_DumpGENRegTab end\n");
}

#if 0 // mode to APP
static void _MDrv_PNL_TCON_DumpPSRegTab(MS_U8 u8TconTab[TCON_TAB_MAX_SIZE], MS_U8 u8Tcontype, MS_U8 *pu8delay)
{
    MS_U32 u32tabIdx = 0;
    MS_U16 u16timeout = 0x3FFF;
    MS_U32 u32Addr;
    MS_U16 u16Mask;
    MS_U16 u16Value;
    MS_U8  u8signal_type = 0;

    PNL_DBG(PNL_DBGLEVEL_INIT, "tab: power sequence \n");

    if (u8TconTab == NULL)
    {
        PNL_ASSERT(0, "[%s] TCONTable error \n.", __FUNCTION__);
        return;
    }

    while (--u16timeout)
    {
        u32Addr = ((u8TconTab[u32tabIdx]<<8) + u8TconTab[(u32tabIdx +1)]) & 0xFFFF;
        u16Mask  = u8TconTab[(u32tabIdx +2)] & 0xFF;
        u16Value = u8TconTab[(u32tabIdx +3)] & 0xFF;
        u8signal_type = u8TconTab[(u32tabIdx +6)];

        if (u32Addr == REG_TABLE_END) // check end of table
            break;

        u32Addr = (u32Addr | 0x100000);

        if(u8Tcontype == u8signal_type)
        {
            PNL_DBG(PNL_DBGLEVEL_INIT, "[addr=%04lx, msk=%02x, val=%02x] \n", u32Addr, u16Mask, u16Value);

            if (u32Addr & 0x1)
    {
                u32Addr --;
                W2BYTEMSK(u32Addr, (u16Value << 8), (u16Mask << 8));
    }
    else
    {
                W2BYTEMSK(u32Addr, u16Value, u16Mask);
    }
            *pu8delay = u8TconTab[(u32tabIdx +5)];
}
        u32tabIdx = u32tabIdx + 7;
    }

    if (u16timeout==0)
    {
        PNL_ASSERT(0, "[%s] fails: timeout \n.", __FUNCTION__);
    }
}
#endif


void MDrv_PNL_TCON_Count_Reset(void *pInstance, MS_BOOL bEnable)
{
    UNUSED(pInstance);
    W2BYTEMSK(L_BK_TCON(0x03), (bEnable << 14), BIT(14));
}

void MDrv_PNL_TCON_Init(void *pInstance)
{
    MHal_PNL_TCON_Init(pInstance);
}


MS_BOOL MDrv_PNL_GetDataFromRegister(void *pInstance, PNL_TimingInfo *PNLTiming)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);

    if(NULL == PNLTiming)
    {
        return FALSE;
    }

#if defined(PATCH_HW_VTT_LIMITATION)
    if(R1BYTEMSK(REG_CHIP_REVISION,0xFF) < HW_VTT_LIMITATION_CHIPREV)
    {
        PNLTiming->u16VTotal    = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK);         // output vtotal
    }
    else
#endif
    {
        //Vtotal
        PNLTiming->u16VTotal    = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK);         // output vtotal
    }
    PNLTiming->u16DEVStart  = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_06_L, VOP_DE_VSTART_MASK);                                       // DE V start
    PNLTiming->u16DEVEnd    = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_07_L, VOP_DE_VEND_MASK);       // DE V end
    PNLTiming->u16VSyncStart= SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_02_L, 0xFFF);       // DE V end
    PNLTiming->u16VSyncEnd  = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK);       // DE V end


    // Htotal
    PNLTiming->u16HTotal    = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_0C_L, VOP_HTT_MASK);         // output htotal
    PNLTiming->u16DEHStart  = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_04_L, VOP_DE_HSTART_MASK);                                       // DE H start
    PNLTiming->u16DEHEnd    = SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_05_L, VOP_DE_HEND_MASK);        // DE H end
    PNLTiming->u16HSyncWidth= SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_01_L, LBMASK);        // DE H end
    PNLTiming->bIsPanelManualVysncMode = (MS_BOOL)(SC_R2BYTEMSK(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_10_L, BIT(15)) >> 15);

    PNLTiming->bInterlaceOutput = MHal_PNL_GetOutputInterlaceTiming(pInstance);
    //printf("----MDrv_PNL_GetDataFromRegister().DAC flag------------%x,%x,%x\n", PM_R1BYTE(L_BK_DAC(REG_DAC_HD_CTRL),  0:0),
    //                                        PM_R1BYTE(L_BK_DAC(REG_DAC_SD_CTRL),  2:0),
    //                                        PM_R1BYTE(H_BK_HDMITX(REG_VE_CONFIG_01),  0:0));
    return TRUE;
}

void MDrv_PNL_SetSSC_En(void *pInstance, MS_BOOL bEnable)
{
    //printf("bEnable = %d\n", bEnable);
    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    W2BYTEMSK((L_BK_LPLL(0x0D)), (bEnable << 11), BIT(11)); // Enable ssc
}

void MDrv_PNL_SetOSDSSC_En(void *pInstance, MS_BOOL bEnable)
{
    MHal_PNL_SetOSDSSC_En(pInstance, bEnable);
}

void MDrv_PNL_SetSSC_Fmodulation(void *pInstance, MS_U16 u16Fmodulation)
{

#if 1
    MS_U64 u64Span;
    //MS_U16 u16Step;
    MS_U32 u32PLL_SET;/// = MDrv_Read3Byte(L_BK_LPLL(0x0F));

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x0F));
    //printf("u16Fmodulation = %d\n", u16Fmodulation );
    u64Span = (((MS_U64)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR)/(u16Fmodulation) ) * 10000;
    u64Span = (u64Span+(u32PLL_SET/2))/u32PLL_SET;
    //u16Span =( ( (((MS_U32)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR ) / (u16Fmodulation) ) * 10000) + ((MS_U32)u32PLL_SET/2)) / ((MS_U32)u32PLL_SET)  ;
    //printf("u64Span = 0x%x\n", u64Span);
    W2BYTE(L_BK_LPLL(0x18), (MS_U16)u64Span & 0x3FFF);// LPLL_SPAN

#endif

}

void MDrv_PNL_SetOSDSSC_Fmodulation(void *pInstance, MS_U16 u16Fmodulation)
{
#if 1
    MS_U64 u64Span;
    //MS_U16 u16Step;
    MS_U32 u32PLL_SET;/// = MDrv_Read3Byte(L_BK_LPLL(0x0F));

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x48));
    //printf("u16Fmodulation = %d\n", u16Fmodulation );
    u64Span = (((MS_U64)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR)/(u16Fmodulation) ) * 10000;
    u64Span = (u64Span+(u32PLL_SET/2))/u32PLL_SET;
    //u16Span =( ( (((MS_U32)LVDS_MPLL_CLOCK_MHZ*LVDS_SPAN_FACTOR ) / (u16Fmodulation) ) * 10000) + ((MS_U32)u32PLL_SET/2)) / ((MS_U32)u32PLL_SET)  ;
    //printf("u64Span = 0x%x\n", u64Span);
    W2BYTE(L_BK_LPLL(0x4F),(MS_U16)u64Span& 0x3FFF);// LPLL_SPAN
#endif
}


void MDrv_PNL_SetSSC_Rdeviation(void *pInstance, MS_U16 u16Rdeviation)
{
#if 1
    MS_U16 u16Span;
    MS_U16 u16Step;
    MS_U32 u32PLL_SET;

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x0F));
    u16Span =R2BYTE(L_BK_LPLL(0x18)) ;
    //printf("u16Rdeviation = %d\n", u16Rdeviation);
    u16Step = (((MS_U32)u32PLL_SET*u16Rdeviation) + ((MS_U32)u16Span*5000)) / ((MS_U32)u16Span*10000);
    //printf("u16Step = 0x%x\n", u16Step);
    W2BYTE(L_BK_LPLL(0x17), u16Step & 0x0FFF);// LPLL_STEP
#endif
}

void MDrv_PNL_SetOSDSSC_Rdeviation(void *pInstance, MS_U16 u16Rdeviation)
{
#if 1
    MS_U16 u16Span;
    MS_U16 u16Step;
    MS_U32 u32PLL_SET;

    MHal_PNL_Switch_LPLL_SubBank(pInstance, 0x00);
    u32PLL_SET = R4BYTE(L_BK_LPLL(0x48));
    u16Span =R2BYTE(L_BK_LPLL(0x4F)) ;
    //printf("u16Rdeviation = %d\n", u16Rdeviation);
    u16Step = (((MS_U32)u32PLL_SET*u16Rdeviation) + ((MS_U32)u16Span*5000)) / ((MS_U32)u16Span*10000);
    //printf("u16Step = 0x%x\n", u16Step);
    W2BYTE(L_BK_LPLL(0x4E), u16Step & 0x0FFF);// LPLL_STEP
#endif
}

void MDrv_Mod_Calibration_Init(void *pInstance, PNL_ModCali_InitData *pstModCaliInitData)
{
    MHal_MOD_Calibration_Init(pInstance, pstModCaliInitData);
}

void MDrv_BD_LVDS_Output_Type(void *pInstance, MS_U16 Type)
{
    MHal_BD_LVDS_Output_Type(pInstance, Type);
}

MS_BOOL MDrv_PNL_SkipTimingChange_GetCaps(void *pInstance)
{
    return Hal_PNL_SkipTimingChange_GetCaps(pInstance);
}

void  MDrv_PNL_PreSetModeOn(void *pInstance, MS_BOOL bSetMode)
{
    MHal_PNL_PreSetModeOn(pInstance, bSetMode);
}

void MDrv_PNL_HWLVDSReservedtoLRFlag(void *pInstance, PNL_DrvHW_LVDSResInfo lvdsresinfo)
{
    MHal_PNL_HWLVDSReservedtoLRFlag(pInstance, lvdsresinfo);
}

void MDrv_PNL_OverDriver_Init(void *pInstance, MS_PHY u32OD_MSB_Addr, MS_PHY u32OD_LSB_Addr, MS_U8 u8ODTbl[1056])
{
    MS_PHY u32OD_MSB_limit, u32OD_LSB_limit;

    MS_U8 u8MIUSel = 0;

#if defined(__AEONR2__)
    if( u32OD_MSB_Addr > HAL_MIU1_BASE)
    {
        u32OD_MSB_Addr = u32OD_MSB_Addr - HAL_MIU1_BASE;
        u8MIUSel = 1;
    }

    if( u32OD_LSB_Addr > HAL_MIU1_BASE)
    {
        u32OD_LSB_Addr = u32OD_LSB_Addr - HAL_MIU1_BASE;
    }
#else
    if( u32OD_MSB_Addr > HAL_MIU2_BASE)
    {
        u32OD_MSB_Addr = u32OD_MSB_Addr - HAL_MIU2_BASE;
        u8MIUSel = 2;
    }
    else if( u32OD_MSB_Addr > HAL_MIU1_BASE)
    {
        u32OD_MSB_Addr = u32OD_MSB_Addr - HAL_MIU1_BASE;
        u8MIUSel = 1;
    }

    if( u32OD_LSB_Addr > HAL_MIU2_BASE)
    {
        u32OD_LSB_Addr = u32OD_LSB_Addr - HAL_MIU2_BASE;
    }
    else if( u32OD_LSB_Addr > HAL_MIU1_BASE)
    {
        u32OD_LSB_Addr = u32OD_LSB_Addr - HAL_MIU1_BASE;
    }
#endif
    u32OD_MSB_Addr = (u32OD_MSB_Addr / OD_BYTE_PER_WORD) & 0xFFFFFFFF;
    u32OD_MSB_limit = u32OD_MSB_Addr + (((1920 *1080 *15)/(8 * OD_BYTE_PER_WORD)) + (4096/OD_BYTE_PER_WORD) + 20 + 16024);
    u32OD_LSB_Addr = (u32OD_LSB_Addr / OD_BYTE_PER_WORD) & 0xFFFFFFFF;
    u32OD_LSB_limit = u32OD_LSB_Addr + (((1920 *1080 *6)/(8 * OD_BYTE_PER_WORD)) + 20 + 80);
    if (SUPPORT_OVERDRIVE)
    {
        MHal_PNL_OverDriver_Init(pInstance, u32OD_MSB_Addr, u32OD_MSB_limit, u32OD_LSB_Addr, u32OD_LSB_limit, u8MIUSel);
        MHal_PNL_OverDriver_TBL(pInstance, u8ODTbl);
    }
}

void MDrv_PNL_OverDriver_Enable(void *pInstance, MS_BOOL bEnable)
{
    if (SUPPORT_OVERDRIVE)
    {
        MHal_PNL_OverDriver_Enable(pInstance, bEnable);
    }
}

MS_BOOL MDrv_PNL_Is_SupportFRC(void *pInstance)
{
    UNUSED(pInstance);
    return SUPPORT_FRC;
}

MS_BOOL MDrv_PNL_Is_SupportTCON(void *pInstance)
{
    UNUSED(pInstance);
    return (MS_BOOL)SUPPORT_TCON;
}

void MDrv_FRC_MOD_ForcePairSwap(void *pInstance, MS_U32 u32Polarity)
{
    MHal_FRC_MOD_PairSwap_UserMode(pInstance, u32Polarity);
}

MS_U16 MDrv_PNL_Get_DEVstart(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    return SC_R2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_06_L) & 0x1FFF; //pnl de vstart, B[12:0]
}

MS_U16 MDrv_PNL_Get_DEHstart(void *pInstance)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    return SC_R2BYTE(pPNLInstancePrivate->u32DeviceID, REG_SC_BK10_04_L) & 0x3FFF; //pnl de vstart, B[13:0]
}

MS_BOOL MDrv_PNL_isYUVOutput(void *pInstance)
{
    //Warning : this function is obsolete, please change to use MApi_XC_GetStatusEx
    //Because if all HW CSC engine is off, then pnl output color space is same with the input signal
    return MHal_PNL_IsYUVOutput(pInstance);
}

MS_BOOL MDrv_PNL_GetDACOut(void *pInstance)
{
    UNUSED(pInstance);
    return IS_DAC_OUT;
}

void MDrv_PNL_CalExtLPLLSETbyDClk(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 u8LPLL_Type, MS_U64 ldHz)
{
    //printf("u8LPLL_Mode=%u,u8LPLL_Type=%u, ldHz=%u\n",u8LPLL_Mode, u8LPLL_Type, ldHz);
    MHal_PNL_CalExtLPLLSETbyDClk(pInstance, u8LPLL_Mode, u8LPLL_Type, ldHz);
}

MS_BOOL MDrv_PNL_EnableInternalTermination(void *pInstance, MS_BOOL bEnable)
{
    UNUSED(pInstance);

    if(bEnable)
    {
        MOD_W2BYTE(REG_MOD_BK00_75_L, 0x3FFF); //GCR_EN_RINT (internal termination open)
    }
    else
    {
        MOD_W2BYTE(REG_MOD_BK00_75_L, 0x0000); //GCR_EN_RINT (internal termination close)
    }

    return TRUE;
}

MS_BOOL MDrv_PNL_VBY1_Handshake(void *pInstance)
{
    return MHal_PNL_VBY1_Handshake(pInstance);
}

MS_BOOL MDrv_PNL_VBY1_OC_Handshake(void *pInstance)
{
    return MHal_PNL_VBY1_OC_Handshake(pInstance);
}

PNL_Result MDrv_PNL_SetOutputInterlaceTiming(void *pInstance, MS_BOOL bEnable)
{
    return MHal_PNL_SetOutputInterlaceTiming(pInstance, bEnable);
}

void MDrv_PNL_GetOutputInterlaceTiming(void *pInstance, MS_BOOL* bIsInterlaceOutput)
{
    *bIsInterlaceOutput = MHal_PNL_GetOutputInterlaceTiming(pInstance);
}

void MDrv_PNL_SetOSDCOutputType(void *pInstance, MS_U16 eLPLL_Type, MS_U8 eOC_OutputFormat)
{
    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;

    UtopiaInstanceGetPrivate(pInstance, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    PNL_DBG(PNL_DBGLEVEL_INIT, "[%s][%d]\n", __FUNCTION__, __LINE__);

    if(((eLPLL_Type >= E_PNL_LPLL_VBY1_10BIT_4LANE)&&
        (eLPLL_Type <= E_PNL_LPLL_VBY1_8BIT_8LANE))||
      (eLPLL_Type == E_PNL_LPLL_VBY1_8BIT_4LANE_BYPASS_MODE) ||
      (eLPLL_Type == E_PNL_LPLL_VBY1_10BIT_4LANE_BYPASS_MODE))
    {
        if(MHal_PNL_VBY1_IsSupport_Hardware_TrainingMode(pInstance)!= TRUE)
        {
            pPNLResourcePrivate->stdrvPNL._stPnlInitData.bOSD_HW_Training_En = FALSE;
            printf("[%s][%d] ATTENTION!!! Not support HW Training Mode\n\n", __FUNCTION__, __LINE__);
        }
    }

    MHal_PNL_SetOSDCOutputType(pInstance, (PNL_TYPE) eLPLL_Type, (E_PNL_OSDC_OUTPUT_FORMAT)eOC_OutputFormat);
}

MS_U32 MDrv_PNL_Get_Semaphore(void *pInstance,E_PNL_POOL_ID eID)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (eID >= E_PNL_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown PNL Pool ID\n",__func__, __LINE__);
        return u32Return;
    }

    if (pInstance == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_PNL_Init() first\n",__FUNCTION__,__LINE__);
        return u32Return;
    }

    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    void* pModule = NULL;
    UtopiaInstanceGetModule(pInstance, &pModule);
    if(UtopiaResourceObtain(pModule, eID, &g_pPNLResource[eID]) != UTOPIA_STATUS_SUCCESS)
    {
        printf("UtopiaResourceObtain fail\n");
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
    u32Return = UTOPIA_STATUS_SUCCESS;
    return u32Return;
}

MS_U32 MDrv_PNL_Release_Semaphore(void *pInstance,E_PNL_POOL_ID eID)
{
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;

    if (eID >= E_PNL_POOL_ID_MAX)
    {
        printf("[%s,%5d] Unknown PNL Pool ID\n",__func__, __LINE__);
        return u32Return;
    }
    #if 0 // for Debug. It is important to check the tid
    pid_t tid;
    tid = syscall(SYS_gettid);
    printf("tid = (%d)\n",tid);
    #endif

    u32Return = UtopiaResourceRelease(g_pPNLResource[eID]);
    return u32Return;
}

MS_U32 MDrv_PNL_GetDeviceNum(void)
{
    return PNL_SUPPORT_DEVICE_NUM;
}

MS_U16 MDrv_PNL_GetPanelVStart(void)
{
    return MHal_PNL_GetPanelVStart();
}

MS_BOOL MDrv_PNL_Check_VBY1_Handshake_Status(void *pInstance)
{
    return MHal_PNL_Check_VBY1_Handshake_Status(pInstance);
}

void MDrv_PNL_MOD_PECurrent_Setting(void *pInstance, MS_U16 u16Current_Level, MS_U16 u16Channel_Select)
{
    MHal_PNL_MOD_PECurrent_Setting(pInstance, u16Current_Level, u16Channel_Select);
}

void MDrv_PNL_VBY1_Hardware_TrainingMode_En(void *pInstance, MS_BOOL bIsVideoMode ,MS_BOOL bEnable)
{
    MHal_PNL_VBY1_Hardware_TrainingMode_En(pInstance,bIsVideoMode,bEnable);
}

const char* MDrv_PNL_GetName(void)
{
    const char* pPanelName="";
    if (pu32PNLInst == NULL)
    {
        return pPanelName;
    }

    PNL_INSTANCE_PRIVATE *pPNLInstancePrivate = NULL;
    PNL_RESOURCE_PRIVATE* pPNLResourcePrivate = NULL;
    UtopiaInstanceGetPrivate(pu32PNLInst, (void**)&pPNLInstancePrivate);
    UtopiaResourceGetPrivate(g_pPNLResource[PNL_GET_INTERNAL_POOL_ID(pPNLInstancePrivate->u32DeviceID)],(void**)(&pPNLResourcePrivate));
    pPanelName = pPNLResourcePrivate->stdrvPNL._stPnlInitData.pPanelName;
    //printf("\033[1;31m ###***###[%s][%d]pPanelName = %s\033[0m\n",__func__,__LINE__,pPanelName);
    return pPanelName;
}

