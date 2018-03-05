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
// Copyright (c) 2008-2010 MStar Semiconductor, Inc.
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
/// file    mdrv_frc.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#define MDRV_FRC_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "utopia_dapi.h"
#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "mvideo_context.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "drv_sc_ip.h"
#include "mhal_frc.h"
#include "mdrv_frc.h"
#include "mhal_sc.h"
#include "XC_private.h"
#include "mdrv_sc_dynamicscaling.h"
#include "drvMIU.h"
#include "halCHIP.h"


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
//static MS_FRC_RGBGAINOFFSET_INFO stGainOffsetSetting;


/******************************************************************************/
/*                      Debug information                                      */
/******************************************************************************/
#define _u16FRCDbgSwitch 0

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

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

void MDrv_FRC_ByPass_Enable(void *pInstance, MS_BOOL bEnable)
{
    MHal_FRC_ByPass_Enable(pInstance, bEnable);
}

void MDrv_FRC_UpdateMDE(void *pInstance, MS_WINDOW_TYPE stDisplayWin)
{
    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U16 u16DsipX = (stDisplayWin.x +1) &~1; // Hstart need to be even
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled) &&
        MHal_FRC_IsFHDToFSC(pInstance,MAIN_WINDOW))
    {
        u16DsipX = (stDisplayWin.x/2 +1) &~1; // Hstart need to be even
        MHal_FRC_TGEN_SetMdeStartEndY(pInstance, stDisplayWin.y/2, (stDisplayWin.y/2 + stDisplayWin.height/2 - 1));
        MHal_FRC_TGEN_SetMdeStartEndX(pInstance, u16DsipX, (u16DsipX + stDisplayWin.width/2 - 1));
    }
    else if (MDrv_XC_Get_3D_Output_Mode(pInstance) != E_XC_3D_OUTPUT_MODE_NONE)
    {
        // 3D case, copy bk10 disp to bk68
        MS_U16 u16Start = 0, u16End = 0, u16DispStart = 0, u16DispEnd = 0, u16Height = 0;

        // copy main disp Y
        u16DispStart = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0A_L);
        u16DispEnd = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_0B_L);
        u16Height = u16DispEnd - u16DispStart + 1;
        // FRC 3D,SC transfer to TB,FRC transfer to LA,tb out 3D main display window height is 1/2 of LA
        if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW) && (pXCResourcePrivate->stdrvXC_3D._bLAToTB) && IS_OUTPUT_4K2K_60HZ_PANEL())
        {
            if(!IsVMirrorMode(MAIN_WINDOW))
            {
                u16End = (u16DispStart -pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)/2 + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + u16Height/2 -1;
                u16Start = (u16DispStart -pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)/2 + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
            }
            else
            {
                u16Start = (u16DispStart -pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart)/2 + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + u16Height/2 -1;
                u16End = u16Start + u16Height/2 -1;
            }
        }
        else
        {
            u16Start = u16DispStart;
            u16End = u16DispEnd;
        }
        MHal_FRC_TGEN_SetMdeStartEndY(pInstance, u16Start, u16End);

        // copy main disp X
        u16Start = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_08_L);
        u16End = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK10_09_L);
        MHal_FRC_TGEN_SetMdeStartEndX(pInstance, u16Start, u16End);

        // copy sub disp Y
        u16Start = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK0F_09_L);
        u16End = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK0F_0A_L);
        MHal_FRC_TGEN_SetSubMdeStartEndY(pInstance, u16Start, u16End);

        // copy sub disp X
        u16Start = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK0F_07_L);
        u16End = SC_R2BYTE(psXCInstPri->u32DeviceID,REG_SC_BK0F_08_L);
        MHal_FRC_TGEN_SetSubMdeStartEndX(pInstance, u16Start, u16End);
    }
    else
    {
        MHal_FRC_TGEN_SetMdeStartEndY(pInstance, stDisplayWin.y, (stDisplayWin.y + stDisplayWin.height - 1));
        MHal_FRC_TGEN_SetMdeStartEndX(pInstance, u16DsipX, (u16DsipX + stDisplayWin.width - 1));
    }
    #endif
}

void MDrv_FRC_Tx_SetTgen(void *pInstance, PMST_PANEL_INFO_t pPanelInfo)
{
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16VTotal = %d\n", pPanelInfo->u16VTotal);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16HTotal = %d\n", pPanelInfo->u16HTotal);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16DE_VStart = %d\n", pPanelInfo->u16DE_VStart);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16Height = %d\n", pPanelInfo->u16Height);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16HStart = %d\n", pPanelInfo->u16HStart);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16u16Width = %d\n", pPanelInfo->u16Width);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16HSyncStart = %d, pPanelInfo->u16HSyncEnd = %d\n", pPanelInfo->u16HSyncStart, pPanelInfo->u16HSyncEnd);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16VSyncStart = %d, pPanelInfo->u16VSyncEnd = %d\n", pPanelInfo->u16VSyncStart, pPanelInfo->u16VSyncEnd);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16VTrigY = %d\n", pPanelInfo->u16VTrigY);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "pPanelInfo->u16VTrigX = %d\n", pPanelInfo->u16VTrigX);

    MHal_FRC_TGEN_SetVTotal(pInstance, pPanelInfo->u16VTotal);
    MHal_FRC_TGEN_SetHTotal(pInstance, pPanelInfo->u16HTotal);
    MHal_FRC_TGEN_SetFdeStartEndY(pInstance, pPanelInfo->u16DE_VStart, (pPanelInfo->u16DE_VStart + pPanelInfo->u16Height - 1));
    MHal_FRC_TGEN_SetFdeStartEndX(pInstance, pPanelInfo->u16HStart, (pPanelInfo->u16HStart + pPanelInfo->u16Width - 1));

    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MDrv_FRC_UpdateMDE(pInstance, gSrcInfo[MAIN_WINDOW].stDispWin);
    #else
    MHal_FRC_TGEN_SetMdeStartEndY(pInstance, pPanelInfo->u16DE_VStart, (pPanelInfo->u16DE_VStart + pPanelInfo->u16Height - 1));
    MHal_FRC_TGEN_SetMdeStartEndX(pInstance, pPanelInfo->u16HStart, (pPanelInfo->u16HStart + pPanelInfo->u16Width - 1));
    #endif
    MHal_FRC_TGEN_SetHSyncStartEndX(pInstance, pPanelInfo->u16HSyncStart, pPanelInfo->u16HSyncEnd);
    MHal_FRC_TGEN_SetVSyncStartEndY(pInstance, pPanelInfo->u16VSyncStart, pPanelInfo->u16VSyncEnd);
    MHal_FRC_TGEN_SetVTrigY(pInstance, pPanelInfo->u16VTrigY);
    MHal_FRC_TGEN_SetVTrigX(pInstance, pPanelInfo->u16VTrigX);
    MHal_FRC_SetYTrig(pInstance, 0x88f, 0x890);

    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_U8 u8LPLL_Type = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;
    if (IsVBY1_16LANE(u8LPLL_Type))
    {
        // for maserati mode,
        // input 50/60 switch, LPLL needs to lock at 100/120Hz
        // need to adjust FO_TGEN Vtt for LPLL lock
        MHal_FRC_FO_TGEN_SetVTotal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal);
    }
    MHal_XC_PixelShiftStatusChange(pInstance, EN_XC_PIXEL_SHIFT_FORCE_UPDATE);
    #endif
}

void MDrv_FRC_SetCSC(void *pInstance, MS_BOOL bEnable)
{
    MHal_FRC_IPM_Csc(pInstance, bEnable);
    MHal_FRC_OP2_ColorMatrixEn(pInstance, bEnable);
}

void MDrv_FRC_SetMemFormat(void *pInstance, PFRC_INFO_t pFRCInfo)
{
    MS_U16 u16LineLimit;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if 0

printf("[%s][%d]pFRCInfo->u32FRC_FrameSize=%lu,pFRCInfo->u16FB_YcountLinePitch=%u,pFRCInfo->u16PanelWidth=%u\n",
		__FUNCTION__,__LINE__,pFRCInfo->u32FRC_FrameSize,pFRCInfo->u16FB_YcountLinePitch,pFRCInfo->u16PanelWidth);
printf("pFRCInfo->u16PanelHeigh=%u,pFRCInfo->u8FRC3DPanelType=%u,pFRCInfo->u83Dmode=%u\n",
		pFRCInfo->u16PanelHeigh,pFRCInfo->u8FRC3DPanelType,pFRCInfo->u83Dmode);
printf("pFRCInfo->u8IpMode=%u,pFRCInfo->u8MirrorMode=%u,pFRCInfo->u83D_FI_out=%u,pFRCInfo->bFRC=%u\n",
		pFRCInfo->u8IpMode,pFRCInfo->u8MirrorMode,pFRCInfo->u83D_FI_out,pFRCInfo->bFRC);
#endif
    u16LineLimit = MHal_FRC_IPM_GetLineLimit(pInstance, pFRCInfo->u8IpMode, pFRCInfo->u16PanelWidth, pFRCInfo->u8MirrorMode);
    pFRCInfo->u16FB_YcountLinePitch = MHal_FRC_IPM_GetYcoutLinePitch(pInstance, pFRCInfo->u8IpMode, u16LineLimit);

    MHal_FRC_SetMemMode(pInstance, pFRCInfo->u8MirrorMode, pFRCInfo->u8IpMode);

	MHal_FRC_IPM_SetYCoutLinePitch(pInstance, pFRCInfo->u16FB_YcountLinePitch);

//=-------------------
///  set frame buffer.
    MHal_FRC_IPM_PacketInitCnt(pInstance, pFRCInfo->u8IpMode, u16LineLimit);
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u8IpMode == FRC_IP_MEM_IP_RGB_10_SPECIAL)
    {
        MHal_FRC_IPM_SetFrameBufferNum(pInstance, 7);
    }
    else
    {
        MHal_FRC_IPM_SetFrameBufferNum(pInstance, 10);
    }
    pFRCInfo->u32FRC_FrameSize = MHal_FRC_IPM_GetFrameYcout(pInstance, pFRCInfo->u16PanelHeigh, pFRCInfo->u16FB_YcountLinePitch);
    MHal_FRC_IPM_SetBaseAddr(pInstance, pFRCInfo->u32FRC_MEMC_MemAddr, pFRCInfo->u32FRC_FrameSize, pFRCInfo->u16FB_YcountLinePitch, pFRCInfo->u8MirrorMode);
    MHal_FRC_OPM_SetBaseAddr(pInstance, pFRCInfo->u32FRC_MEMC_MemAddr, pFRCInfo->u32FRC_FrameSize);

}

void MDrv_FRC_IPM_Init(void *pInstance, PMST_PANEL_INFO_t pPanelInfo, PFRC_INFO_t pFRCInfo)
{

    MHal_FRC_IPM_TestPattern(pInstance, FALSE, 0);
    MHal_FRC_IPM_SetHTotal(pInstance, pPanelInfo->u16HTotal);
    MHal_FRC_IPM_SetVTotal(pInstance, pPanelInfo->u16VTotal);
    MHal_FRC_IPM_HActive(pInstance, 0, pPanelInfo->u16Width);
    MHal_FRC_IPM_VActive(pInstance, 0, pPanelInfo->u16Height);

    MHal_FRC_IPM_SetIp2Mc(pInstance, ENABLE);
    MHal_FRC_IPM_SetRfifoThr(pInstance, 0x1F10); //0x1F2F for Film 32 mode
    MHal_FRC_IPM_SetWfifoThr(pInstance, 0x4020);
    MHal_FRC_IPM_SetReadLength(pInstance, 0x0820);
    MHal_FRC_IPM_SetWriteLength(pInstance, 0x0040);
    MHal_FRC_IPM_SetRmaskNum(pInstance, 0x00);
    MHal_FRC_IPM_SetWmaskNum(pInstance, 0x00);
    MHal_FRC_IPM_SetLvdsInputMode(pInstance, 0x03);

    MHal_FRC_IPM_SetOsdWinIdx(pInstance, 0x00);
    MHal_FRC_IPM_SetVPulseLineRst(pInstance, 0x0003);
    MHal_FRC_IPM_SetVPulseLoc(pInstance, 0x0001);
    MHal_FRC_IPM_SetLockIntCtrl(pInstance, 0x10);
    MHal_FRC_IPM_SetBlankBoundary(pInstance, 0x00DB);
    MHal_FRC_IPM_SetIpCtrl(pInstance, ENABLE, 0x03, 0x00);
    MHal_FRC_IPM_SetHRefLock(pInstance, 0x05);

    if(MHal_FRC_IsRGB(pInstance, pFRCInfo->u8IpMode))
    {
        MHal_FRC_IPM_RWEn(pInstance, DISABLE,ENABLE);
    }
    else
    {
        MHal_FRC_IPM_RWEn(pInstance, ENABLE, ENABLE);
    }
    MHal_FRC_IPM_CscDither(pInstance, FALSE);
    MHal_FRC_IPM_CscRound(pInstance, TRUE);
    MHal_FRC_IPM_CscSaturation(pInstance, TRUE);
}

void MDrv_FRC_OPM_Init(void *pInstance)
{

    MHal_FRC_OPM_SetMlbOutRstCycle(pInstance, 0x08);
    MHal_FRC_OPM_SetFifoTrigThr(pInstance, 0x6060);


    // 0x40, fine tune BW at frameinterleave @ 120Hz.
    // 0x20, decrease BW at 720P frameinterleave
    MHal_FRC_OPM_SetFifoMaxReadReq(pInstance, 0x60);//40);   // 0x50 //0x7f

    MHal_FRC_OPM_FuncEn(pInstance, 0x01, 0x02);
    MHal_FRC_OPM_SetGatingClk(pInstance, 0x20, 0);
}

void MDrv_FRC_Init(void *pInstance, PMST_PANEL_INFO_t pPanelInfo, PFRC_INFO_t pFRCInfo)
{
#ifdef UFO_XC_SUPPORT_DUAL_MIU
    MDrv_FRC_IsDdr4Dram(pInstance,pFRCInfo);
#endif
    MHal_FRC_Init(pInstance, pPanelInfo, pFRCInfo);
}


//FRC_3D_FHD_FI_2x_Passive,       // 3,  1920x1080
//FRC_3D_FHD_HH_FI_2x_Passive, // 4,  960x1080
//FRC_3D_FHD_HV_FI_2x_Passive, // 5,  1920x540

void MDrv_FRC_Set_3D_QMap(void *pInstance, MS_U8 u8FRC3DPanelType, MS_U16 u16sc_in_3dformat, MS_U16 u16sc_out_3dformat, MS_U8 u83D_FI_out)
{
    MHal_FRC_Set_3D_QMap(pInstance, u8FRC3DPanelType,  u16sc_in_3dformat,  u16sc_out_3dformat,  u83D_FI_out);
}

void MDrv_FRC_OPM_SetBaseOfset(void *pInstance, FRC_INFO_t *FRCInfo, E_XC_3D_OUTPUT_MODE u16out_3dformat)
{
	#ifdef Eagle_Bringup
		return;
	#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if ((u16out_3dformat == E_XC_3D_OUTPUT_TOP_BOTTOM) ||
        (u16out_3dformat == E_XC_3D_OUTPUT_MODE_NONE))
    {
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            MHal_FRC_OPM_SetBaseOffset_Data0(pInstance, FRCInfo->u32FRC_FrameSize);
            MHal_FRC_OPM_SetBaseOffset_Data1(pInstance, FRCInfo->u32FRC_FrameSize);
            MHal_FRC_OPM_SetBaseOffset_Me0(pInstance, FRCInfo->u32FRC_FrameSize);
            MHal_FRC_OPM_SetBaseOffset_Me1(pInstance, FRCInfo->u32FRC_FrameSize);
            MHal_FRC_OPM_SetBaseOffset_Me3(pInstance, FRCInfo->u32FRC_FrameSize);
            MHal_FRC_OPM_SetBaseOffset_Mr1(pInstance, FRCInfo->u32FRC_FrameSize);
        }
        else
        {
            MHal_FRC_OPM_SetBaseOffset_Data0(pInstance, FRCInfo->u32FRC_FrameSize/2);
            MHal_FRC_OPM_SetBaseOffset_Data1(pInstance, FRCInfo->u32FRC_FrameSize/2);
            MHal_FRC_OPM_SetBaseOffset_Me0(pInstance, FRCInfo->u32FRC_FrameSize/2);
            MHal_FRC_OPM_SetBaseOffset_Me1(pInstance, FRCInfo->u32FRC_FrameSize/2);
            MHal_FRC_OPM_SetBaseOffset_Me3(pInstance, FRCInfo->u32FRC_FrameSize/2);
            MHal_FRC_OPM_SetBaseOffset_Mr1(pInstance, FRCInfo->u32FRC_FrameSize/2);
        }
    }
    else if (u16out_3dformat == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
        MHal_FRC_OPM_SetBaseOffset_Data0(pInstance, FRCInfo->u32FRC_FrameSize);
        MHal_FRC_OPM_SetBaseOffset_Data1(pInstance, FRCInfo->u32FRC_FrameSize);
        MHal_FRC_OPM_SetBaseOffset_Me0(pInstance, FRCInfo->u32FRC_FrameSize);
        MHal_FRC_OPM_SetBaseOffset_Me1(pInstance, FRCInfo->u32FRC_FrameSize);
        MHal_FRC_OPM_SetBaseOffset_Me3(pInstance, FRCInfo->u32FRC_FrameSize);
        MHal_FRC_OPM_SetBaseOffset_Mr1(pInstance, FRCInfo->u32FRC_FrameSize);

        if ((FRCInfo->u83D_FI_out== E_XC_3D_OUTPUT_FI_1920x1080) ||
           (FRCInfo->u83D_FI_out == E_XC_3D_OUTPUT_FI_960x1080))
        {
            MHal_FRC_OPM_SetBaseOffset_Da0_L(pInstance, (FRCInfo->u32FRC_FrameSize + FRCInfo->u16FB_YcountLinePitch));
            MHal_FRC_OPM_SetBaseOffset_Da1_L(pInstance, (FRCInfo->u32FRC_FrameSize + FRCInfo->u16FB_YcountLinePitch));
            MHal_FRC_OPM_SetBaseOffset_Me0_L(pInstance, (FRCInfo->u32FRC_FrameSize + FRCInfo->u16FB_YcountLinePitch));
            MHal_FRC_OPM_SetBaseOffset_Me1_L(pInstance, (FRCInfo->u32FRC_FrameSize + FRCInfo->u16FB_YcountLinePitch));
            MHal_FRC_OPM_SetBaseOffset_Me3_L(pInstance, (FRCInfo->u32FRC_FrameSize + FRCInfo->u16FB_YcountLinePitch));
            MHal_FRC_OPM_SetBaseOffset_Mr1_L(pInstance, (FRCInfo->u32FRC_FrameSize + FRCInfo->u16FB_YcountLinePitch));
        }
        else if(FRCInfo->u83D_FI_out == E_XC_3D_OUTPUT_FI_1920x540)
        {
            MHal_FRC_OPM_SetBaseOffset_Da0_L(pInstance, 0);
            MHal_FRC_OPM_SetBaseOffset_Da1_L(pInstance, 0);
            MHal_FRC_OPM_SetBaseOffset_Me0_L(pInstance, 0);
            MHal_FRC_OPM_SetBaseOffset_Me1_L(pInstance, 0);
            MHal_FRC_OPM_SetBaseOffset_Me3_L(pInstance, 0);
            MHal_FRC_OPM_SetBaseOffset_Mr1_L(pInstance, 0);
        }
    }
    else if(u16out_3dformat == E_XC_3D_OUTPUT_SIDE_BY_SIDE_HALF)
    {
        MHal_FRC_OPM_SetBaseOffset_Data0(pInstance, FRCInfo->u16FB_YcountLinePitch/2);
        MHal_FRC_OPM_SetBaseOffset_Data1(pInstance, FRCInfo->u16FB_YcountLinePitch/2);
        MHal_FRC_OPM_SetBaseOffset_Me0(pInstance, FRCInfo->u16FB_YcountLinePitch/2);
        MHal_FRC_OPM_SetBaseOffset_Me1(pInstance, FRCInfo->u16FB_YcountLinePitch/2);
        MHal_FRC_OPM_SetBaseOffset_Me3(pInstance, FRCInfo->u16FB_YcountLinePitch/2);
        MHal_FRC_OPM_SetBaseOffset_Mr1(pInstance, FRCInfo->u16FB_YcountLinePitch/2);
    }
}

MS_BOOL MDrv_XC_SendCmdToFRC(void *pInstance, MS_U8 u8Cmd, MS_U8 count, FRC_R2_CMD_PARAMETER_t pFRC_R2_Para )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_BOOL bValue = FALSE;

    if(u8Cmd == E_XC_FRC_MB_CMD_SET_MFC_MDOE)
    {
#if (HW_DESIGN_4K2K_VER == 6) // For Manhattan, FSC+FRC
//g_u16MEMCLevel defined only by  (HW_DESIGN_4K2K_VER == 6)
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16MEMCLevel =  pFRC_R2_Para.p1;
#endif
        if((gSrcInfo[MAIN_WINDOW].bEnableFPLL == FALSE)
            &&((pFRC_R2_Para.p1 >= MEMC_LEVEL_LOW) && (pFRC_R2_Para.p1 <= MEMC_LEVEL_HIGH)))
        {
            pFRC_R2_Para.p1 = MEMC_LEVEL_OFF;
        }

        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_FREERUN_UCNR_ENABLE)
        {
            pFRC_R2_Para.p1 = MEMC_LEVEL_OFF;
        }
    }
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        bValue = MHal_XC_SendCmdToFRC(pInstance, u8Cmd, count, pFRC_R2_Para.p1, pFRC_R2_Para.p2, pFRC_R2_Para.p3,
                          pFRC_R2_Para.p4, pFRC_R2_Para.p5, pFRC_R2_Para.p6, pFRC_R2_Para.p7, pFRC_R2_Para.p8);

#if (HW_DESIGN_3D_VER == 4)
        // for 2Dto3D case, the FRC FICLK need to be changed
        //E_MAPI_FRC_MB_CMD_SET_2D_TO_3D_MODE = 0x40,  ParaCnt=5,
        //          P0 = 2D to 3D ConversionMode(0=Disable, 1=Enable)
        //          P1 = 3D Depth Gain(0~31)
        //          P2 = 3D Depth Offset(0~127)
        //          P3 = Artificial Gain(0~15)
        //          P4 = L/R Swap (0=Normal, 1=L/R Swap)
        if(u8Cmd == 0x40)
        {
            if(pFRC_R2_Para.p1==ENABLE)
            {
                W2BYTEMSK(L_CLKGEN1(0x35), 0x0004, 0x0004); // clk_idclk_frc
                W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0002); // Not Invert
                W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0001); // Enable clock
            }
            else
            {
                W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0004); // clk_fdclk_frc
                W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0002); // Not Invert
                W2BYTEMSK(L_CLKGEN1(0x35), 0x0000, 0x0001); // Enable clock
            }
        }
#endif
    }

    return bValue;
}
MS_BOOL MDrv_XC_GetMsgFromFRC(void *pInstance, MS_U8* pu8Cmd, MS_U8* pu8ParaCount, MS_U8* pu8Para)
{
    MS_BOOL bValue;
    bValue = MHal_XC_GetMsgFromFRC(pInstance, pu8Cmd, pu8ParaCount, pu8Para);
    return bValue;
}

void MDrv_XC_set_OC_Reg(void *pInstance, PMST_PANEL_INFO_t pPanelInfo)
{
    MHal_XC_OC_set_HSyncStartEnd(pInstance, pPanelInfo->u16HSyncStart, pPanelInfo->u16HSyncEnd);
    MHal_XC_OC_set_VSyncStartEnd(pInstance, pPanelInfo->u16VSyncStart, pPanelInfo->u16VSyncEnd);
    MHal_XC_OC_set_HFdeStartEnd(pInstance, pPanelInfo->u16HStart, (pPanelInfo->u16HStart + pPanelInfo->u16Width -1));
    MHal_XC_OC_set_VFdeStartEnd(pInstance, pPanelInfo->u16VStart, (pPanelInfo->u16VStart+pPanelInfo->u16Height-1));
    MHal_XC_OC_set_HTotal(pInstance, (pPanelInfo->u16HTotal-1));
    MHal_XC_OC_set_VTotal(pInstance, (pPanelInfo->u16VTotal-1));
}

void MDrv_FRC_OP2_ColorMatrixEn(void *pInstance, MS_BOOL bEnable)
{
    MHal_FRC_OP2_ColorMatrixEn(pInstance, bEnable);
}
void MDrv_FRC_OP2_CscDitherEn(void *pInstance, MS_BOOL bEnable)
{
    MHal_FRC_OP2_CscDitherEn(pInstance, bEnable);
}
/// FRC_SC30_0x5F[15]
void MDrv_FRC_OP2_BrightnessEn(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
        MHal_FRC_OP2_DataPathEn(pInstance, BIT(15),  0x00);
    }
    else
    {
        MHal_FRC_OP2_DataPathEn(pInstance, 0x00,  BIT(15));
    }
}
/// FRC_SC30_0x5F[14]
void MDrv_FRC_OP2_ContrastEn(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
        MHal_FRC_OP2_DataPathEn(pInstance, BIT(14), 0x00);
    }
    else
    {
        MHal_FRC_OP2_DataPathEn(pInstance, 0x00, BIT(14));
    }
}
/// FRC_SC30_0x5F[13]
void MDrv_FRC_OP2_NoiseRoundEn(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
        MHal_FRC_OP2_DataPathEn(pInstance, BIT(13), 0x00);
    }
    else
    {
        MHal_FRC_OP2_DataPathEn(pInstance, 0x00, BIT(13));
    }
}

void MDrv_FRC_OP2_SetRGBGain(void *pInstance, MS_U16 u16RedGain, MS_U16 u16GreenGain, MS_U16 u16BlueGain)
{
//    stGainOffsetSetting.u16RedGain = u16RedGain ;
//    stGainOffsetSetting.u16GreenGain = u16GreenGain ;
//    stGainOffsetSetting.u16BlueGain = u16BlueGain ;

    MHal_FRC_OP2_SetGain_R(pInstance, u16RedGain);
    MHal_FRC_OP2_SetGain_G(pInstance, u16GreenGain);
    MHal_FRC_OP2_SetGain_B(pInstance, u16BlueGain);
}

void MDrv_FRC_OP2_SetRGBOffset(void *pInstance, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset)
{
//    stGainOffsetSetting.u16RedGain = u16RedOffset ;
//    stGainOffsetSetting.u16GreenGain = u16GreenOffset ;
//    stGainOffsetSetting.u16BlueGain = u16BlueOffset ;

    MHal_FRC_OP2_SetOffset_R(pInstance, u16RedOffset);
    MHal_FRC_OP2_SetOffset_G(pInstance, u16GreenOffset);
    MHal_FRC_OP2_SetOffset_B(pInstance, u16BlueOffset);
}

void MDrv_FRC_OP2_SetDither(void *pInstance, MS_U16 u16dither)
{
    MHal_FRC_OP2_SetDither(pInstance, u16dither);
}

void MDrv_FRC_TGEN_SetHTotal(void *pInstance, MS_U16 u16HTotal)
{
	MHal_FRC_TGEN_SetHTotal(pInstance, u16HTotal);
}

void MDrv_FRC_TGEN_SetVTotal(void *pInstance, MS_U16 u16VTotal)
{
	MHal_FRC_TGEN_SetVTotal(pInstance, u16VTotal);
}

//FRC IPM/OPM
void MDrv_FRC_IPM_SetOffset(void *pInstance, MS_U16 offset)
{
    return MHal_FRC_IPM_SetOffset(pInstance, offset);
}

void MDrv_FRC_IPM_SetFetchNum(void *pInstance,MS_U16 fetch)
{
    return MHal_FRC_IPM_SetFetchNum(pInstance, fetch);
}

void MDrv_FRC_OPM_SetOffset(void *pInstance,MS_U16 offset)
{
    return MHal_FRC_OPM_SetOffset(pInstance, offset);
}

void MDrv_FRC_OPM_SetFetchNum(void *pInstance,MS_U16 fetch)
{
    return MHal_FRC_OPM_SetFetchNum(pInstance, fetch);
}

//FRC OP HSU/VSU
void MDrv_FRC_HSU_SetScalingSize(void *pInstance, MS_U16 input, MS_U16 output)
{
    return MHal_FRC_HSU_SetScalingSize(pInstance, input, output);
}

void MDrv_FRC_VSU_SetScalingSize(void *pInstance, MS_U16 input, MS_U16 output)
{
    return MHal_FRC_VSU_SetScalingSize(pInstance, input, output);
}

//force RGB 444
void MDrv_FRC_CSC_SelectPath(void *pInstance, MS_U8 type)
{
    return MHal_FRC_CSC_SelectPath(pInstance, type);
}

void MDrv_FRC_PNLInfo_Transform(void *pInstance, XC_PANEL_INFO *XC_PNLInfo, XC_PREINIT_INFO_t *XC_PNLInfo_Adv,
                                                 MST_PANEL_INFO_t *PanelInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));


    PanelInfo->u16VSyncStart        = (XC_PNLInfo->u16VTotal -
                                     (MS_U16)(XC_PNLInfo_Adv->u8PanelVSyncBackPorch + XC_PNLInfo_Adv->u8PanelVSyncWidth));
    PanelInfo->u16VSyncEnd          = (XC_PNLInfo->u16VTotal -
                                     (MS_U16)(XC_PNLInfo_Adv->u8PanelVSyncBackPorch));
    // 4K2K patch, 3840x2160, 4096x2160
    if( (XC_PNLInfo->u16Width >= 3840) && (XC_PNLInfo->u16Height == 2160) )
    {
        PanelInfo->u16HSyncStart        = 0;
        PanelInfo->u16HSyncEnd          = XC_PNLInfo_Adv->u8PanelHSyncWidth;

        printf("@@@ 4K2K patch for VTrigX/VtrigY tunning\n");
        PanelInfo->u16VTrigX           = PanelInfo->u16HSyncEnd + 0x150;
        PanelInfo->u16VTrigY           = PanelInfo->u16VSyncEnd + 6;
    }
    else
    {
        PanelInfo->u16HSyncStart        = (XC_PNLInfo->u16HTotal -
                                         (MS_U16)(XC_PNLInfo_Adv->u8PanelHSyncBackPorch + XC_PNLInfo_Adv->u8PanelHSyncWidth));
        PanelInfo->u16HSyncEnd          = (XC_PNLInfo->u16HTotal -
                                         (MS_U16)(XC_PNLInfo_Adv->u8PanelHSyncBackPorch));

        PanelInfo->u16VTrigX           = XC_PNLInfo_Adv->u16VTrigX;
        PanelInfo->u16VTrigY           = XC_PNLInfo_Adv->u16VTrigY;
    }

    PanelInfo->u16HTotal           = XC_PNLInfo->u16HTotal;
    PanelInfo->u16VTotal           = XC_PNLInfo->u16VTotal;
    // DE
    PanelInfo->u16HStart           = XC_PNLInfo->u16HStart;
    PanelInfo->u16DE_VStart        = XC_PNLInfo->u16VStart;
    PanelInfo->u16Width            = XC_PNLInfo->u16Width;
    PanelInfo->u16Height           = XC_PNLInfo->u16Height;
    PanelInfo->u16DefaultVFreq     = XC_PNLInfo->u16DefaultVFreq;

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
    {
        PanelInfo->u16VTotal           = PanelInfo->u16VTotal/2;
        PanelInfo->u16VSyncStart       = PanelInfo->u16VSyncStart/2;
        PanelInfo->u16VSyncEnd         = PanelInfo->u16VSyncStart + XC_PNLInfo_Adv->u8PanelVSyncWidth;
        PanelInfo->u16DE_VStart        = PanelInfo->u16DE_VStart/2;
        PanelInfo->u16Height           = PanelInfo->u16Height/2;
    }
    else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
    {
        PanelInfo->u16VTotal           = PanelInfo->u16VTotal/4;
        PanelInfo->u16VSyncStart       = PanelInfo->u16VSyncStart/4;
        PanelInfo->u16VSyncEnd         = PanelInfo->u16VSyncStart + XC_PNLInfo_Adv->u8PanelVSyncWidth;
        PanelInfo->u16DE_VStart        = PanelInfo->u16DE_VStart/4;
        PanelInfo->u16Height           = PanelInfo->u16Height/4;
    }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
    {
        // Manhattan FSC enable case, if SD/FHD input, STGEN need to set as FHD size
        if (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW))
        {
            PanelInfo->u16HTotal           = PanelInfo->u16HTotal/2;
            PanelInfo->u16HSyncStart       = PanelInfo->u16HSyncStart/2;
            PanelInfo->u16HSyncEnd         = PanelInfo->u16HSyncStart + XC_PNLInfo_Adv->u8PanelHSyncWidth;
            PanelInfo->u16HStart           = (PanelInfo->u16HStart/2 +1) & ~1;  // Hstart need to be even
            PanelInfo->u16Width            = PanelInfo->u16Width/2;

            PanelInfo->u16VTotal           = PanelInfo->u16VTotal/2;
            PanelInfo->u16VSyncStart       = PanelInfo->u16VSyncStart/2;
            PanelInfo->u16VSyncEnd         = PanelInfo->u16VSyncStart + XC_PNLInfo_Adv->u8PanelVSyncWidth;
            PanelInfo->u16DE_VStart        = PanelInfo->u16DE_VStart/2;
            PanelInfo->u16Height           = PanelInfo->u16Height/2;
        }
        else if(MHal_FRC_IsEnableFRC3D(pInstance, MAIN_WINDOW))
        {
            if(IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
           {
                PanelInfo->u16VTotal           = PanelInfo->u16VTotal*2;
                PanelInfo->u16VSyncStart       = PanelInfo->u16VSyncStart*2;
                PanelInfo->u16VSyncEnd         = PanelInfo->u16VSyncStart + XC_PNLInfo_Adv->u8PanelVSyncWidth;
                //PanelInfo->u16DE_VStart        = PanelInfo->u16DE_VStart*2;
                PanelInfo->u16Height           = PanelInfo->u16Height*2;
            }
        }
    }
#endif
}

void MDrv_FRC_Mute(void *pInstance, MS_BOOL bEnable)
{
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "FRC Mute enable = %d\n", bEnable);
    MHal_FRC_Mute(pInstance, bEnable);
}

MS_BOOL MDrv_XC_Set_FRC_InputTiming(void *pInstance, E_XC_FRC_InputTiming enFRC_InputTiming)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    #if FRC_INSIDE
    pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming = enFRC_InputTiming;
    #endif
    return TRUE;
}

void MDrv_XC_FRC_Set_Input3DFormat(void *pInstance, E_XC_3D_INPUT_MODE enFrcInput3DType)
{
    MS_U8 u8FrcInput3DType = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "FRC Input 3D Type= %d\n", enFrcInput3DType);

    switch (enFrcInput3DType) //R2 Input3DFormat(0: 2D, 1: Side-by-side, 2: Top-Bottom, 4: Frame alternative)
    {
        case E_XC_3D_INPUT_MODE_NONE:
            u8FrcInput3DType = E_FRC_INPUT_2D;   //R2 : 2D
            break;
        case E_XC_3D_INPUT_SIDE_BY_SIDE_HALF:
            u8FrcInput3DType = E_FRC_INPUT_3D_SBS;   //R2 : side by side
            break;
        case E_XC_3D_INPUT_TOP_BOTTOM:
            u8FrcInput3DType = E_FRC_INPUT_3D_TD;   //R2 : Top-Down
            break;
        case E_XC_3D_INPUT_FRAME_ALTERNATIVE:
            u8FrcInput3DType = E_FRC_INPUT_3D_FI;    //R2 :  Frame Interleave
            break;
        default :
            u8FrcInput3DType = E_FRC_INPUT_2D;    //R2 : 2D
            break;
    }

    Hal_XC_FRC_R2_Set_Input3DFormat(pInstance, u8FrcInput3DType);
}

MS_BOOL MDrv_XC_FRC_R2_Set_InputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize )
{
    return Hal_XC_FRC_R2_Set_InputFrameSize(pInstance, u16HSize, u16VSize );
}

MS_BOOL MDrv_XC_FRC_R2_Set_OutputFrameSize(void *pInstance, MS_U16 u16HSize, MS_U16 u16VSize )
{
    return Hal_XC_FRC_R2_Set_OutputFrameSize(pInstance, u16HSize, u16VSize );
}

MS_BOOL MDrv_FRC_IsDdr4Dram(void *pInstance,PFRC_INFO_t pFRCInfo)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U8 u8MIUSel_L = 0,u8MIUSel_R=0;
    MS_U32 u32OffsetAddr = 0;
    MIU_DDR_TYPE eFrc_L_Type = E_MIU_DDR3,eFrc_R_Type = E_MIU_DDR3;
    MS_BOOL bRet = FALSE;

    _phy_to_miu_offset(u8MIUSel_L, u32OffsetAddr, pFRCInfo->u32FRC_MEMC_MemAddr);

    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_DUAL_MIU) &&
    (pFRCInfo->u32FRC_R2_MemSize != 0))
    {
        _phy_to_miu_offset(u8MIUSel_R, u32OffsetAddr, pFRCInfo->u32FRC_MEMC_MemAddr);
    }
    else
    {
        u8MIUSel_R = u8MIUSel_L;
    }

    bRet = MDrv_MIU_GetDramType(u8MIUSel_L,&eFrc_L_Type);
    if(bRet == FALSE)
    {
        printf("Get dram type fail %s,%d\n",__FUNCTION__,__LINE__);
    }

    bRet = MDrv_MIU_GetDramType(u8MIUSel_R,&eFrc_R_Type);
    if(bRet == FALSE)
    {
        printf("Get dram type fail %s,%d\n",__FUNCTION__,__LINE__);
    }

    if((eFrc_L_Type == E_MIU_DDR4) || (eFrc_R_Type == E_MIU_DDR4))
    {
        pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForFrc = TRUE;

    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo.bUseDdr4DramForFrc = FALSE;
    }

    return bRet;
}

void MDrv_FRC_SetMemoryAddress(void *pInstance, PXC_FRC_ADDRESS_INFO pFRCAddrInfo)
{
    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsAddrUpdated = TRUE;

    pXCResourcePrivate->stdrvXC_MVideo_Context.g_stXC_FRC_Address.u64FRC_IPM_L_MemAddr = pFRCAddrInfo->u64FRC_IPM_L_MemAddr;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_stXC_FRC_Address.u64FRC_IPM_L_MemSize = pFRCAddrInfo->u64FRC_IPM_L_MemSize;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_stXC_FRC_Address.u64FRC_IPM_R_MemAddr = pFRCAddrInfo->u64FRC_IPM_R_MemAddr;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_stXC_FRC_Address.u64FRC_IPM_R_MemSize = pFRCAddrInfo->u64FRC_IPM_R_MemSize;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_stXC_FRC_Address.u64FRC_PQ_MemAddr = pFRCAddrInfo->u64FRC_PQ_MemAddr;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_stXC_FRC_Address.u64FRC_PQ_MemSize = pFRCAddrInfo->u64FRC_PQ_MemSize;
    #endif

}


