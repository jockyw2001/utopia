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
#ifndef DISABLE_DIP_FUNCTION

#define DRV_SC_DIPSCALING_C

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia.h"
// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_DWIN.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_ModeParse.h"
#include "drvXC_HDMI_if.h"
#include "mvideo_context.h"
#include "drv_sc_ip.h"
#include "halCHIP.h"
#include "drv_sc_scaling.h"
#include "xc_Analog_Reg.h"
#if (LD_ENABLE==1)
#include "mdrv_ld.h"
#include "mdrv_ldalgo.h"
#endif
#include "mdrv_sc_3d.h"
#include "drv_sc_menuload.h"
#include "drvXC_ADC_Internal.h"
#include "mdrv_frc.h"
#include "mhal_frc.h"
#include "mhal_sc.h"
#include "mhal_dynamicscaling.h"
#include "XC_private.h"
#include "apiXC_v2.h"
#include "drvXC_HDMI_Internal.h"

#include "hwreg_ipmux.h"

#include "drv_sc_mux.h"
#include "drv_sc_DIP_scaling.h"
#include "mdrv_sc_dynamicscaling.h"
#include "drvscaler_nr.h"
#include "drvMMIO.h"

//Add for A5
#include "mhal_dip.h"

// Tobe refined
#include "xc_hwreg_utility2.h"
#include "mhal_mux.h"
#include "mhal_hdmi.h"
#include "mhal_ip.h"
#include "mhal_menuload.h"
#ifdef UTOPIA_V2
#include "XC_DWIN_private.h"
#include "utopia_dapi.h"
#endif
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#endif

//////////////////////////Remove when drvpq change us Mscommon.h /////////////
//#define E_XRULE_VSD         1

//#define PIP_SUPPORTED (MAX_WINDOW_NUM > 1)

#if 0
typedef enum
{
   E_DRVSC_FBNUM  = 0,
}DRV_SC_SRCINFO ;
#endif

//extern PQ_Function_Info    s_PQ_Function_Info;
///////////////////////////////////////

#define msg_video(x) //x
/******************************************************************************/
/*                     Local                                                  */
/* ****************************************************************************/
MS_XC_MEM_FMT s_DIPeMemFmt[MAX_DIP_WINDOW] = {E_MS_XC_MEM_FMT_AUTO};
#ifdef UTOPIA_V2
#define g_DIPSrcInfo            pDipResPri->g_DIPSrcInfo
#endif

// Debug Log
#include "ULog.h"
MS_U32 u32DIPDbgLevel_drv;

// Debug Logs, level form low(INFO) to high(FATAL, always show)
// Function information, ex function entry
#define DIP_D_INFO(x, args...) if(u32DIPDbgLevel_drv >= E_XC_DIP_Debug_Level_HIGH){ULOGI("DIP DRV", x, ##args);}
// Warning, illegal paramter but can be self fixed in functions
#define DIP_D_WARN(x, args...)  if(u32DIPDbgLevel_drv >= E_XC_DIP_Debug_Level_HIGH){ULOGW("DIP DRV", x, ##args);}
//  Need debug, illegal paramter.
#define DIP_D_DBUG(x, args...)  if(u32DIPDbgLevel_drv >= E_XC_DIP_Debug_Level_MED){ULOGD("DIP DRV", x, ##args);}
// Error, function will be terminated but system not crash
#define DIP_D_ERR(x, args...)  if(u32DIPDbgLevel_drv >= E_XC_DIP_Debug_Level_LOW){ULOGE("DIP DRV", x, ##args);}
// Critical, system crash. (ex. assert)
#define DIP_D_FATAL(x, args...)  if(u32DIPDbgLevel_drv >= E_XC_DIP_Debug_Level_LOW){ULOGF("DIP DRV", x, ##args);}

/******************************************************************************/
/*                   Functions                                                */
/******************************************************************************/
static void MDrv_XC_DIP_CMDQBegin(void* pInstance,SCALER_DIP_WIN eWindow)
{
    HAL_XC_DIP_CMDQBegin(pInstance,eWindow);
}

static void MDrv_XC_DIP_CMDQEnd(void* pInstance,SCALER_DIP_WIN eWindow)
{
    HAL_XC_DIP_CMDQEnd(pInstance,eWindow);
}

void MDrv_XC_DIP_CMDQ_SetAction(void* pInstance, EN_XC_DIP_CMDQ_ACTION eAction, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);

    switch (eAction)
    {
        case E_XC_DIP_CMDQ_BEGIN:
            MDrv_XC_DIP_CMDQBegin(pInstance, eWindow);
        break;

        case E_XC_DIP_CMDQ_END:
            MDrv_XC_DIP_CMDQEnd(pInstance, eWindow);
        break;

        default:
            printf("[Warnning] %s, Line:%d, eWindow:%d Error Action Item:%d\n",
            __FUNCTION__, __LINE__, eWindow, eAction);
        break;
    }

    _XC_RETURN(pInstance);
}

void MDrv_XC_DIP_CMDQ_SetOperations(void* pInstance, cmd_mload_utopia_interface *pOps, MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_CMDQ_SetOperations(pInstance, pOps, bEnable, eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DIP_SWReset(void* pInstance,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
	HAL_XC_DIP_SWReset(pInstance, eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_Init(void* pInstance,SCALER_DIP_WIN eWindow)
{
    //check the mutex init
    //shoud init XC befor DWIN init.
    if (_XC_Mutex == -1)
    {
        msg_video(DIP_D_ERR("[MAPI DWIN][%06d] Should call XC init first.\n", __LINE__));
        return;
    }

    // init DWIN address if use NR
    MDrv_XC_DWIN_DisableInputSource(pInstance,ENABLE, eWindow);

    MDrv_XC_DWIN_init_swdb(pInstance,eWindow);

    MDrv_XC_DWIN_Disable_IPM_ReadWriteRequest(pInstance,ENABLE, eWindow);

    MDrv_XC_DWIN_EnableIPAutoCoast(pInstance,ENABLE, eWindow);

    HAL_XC_DIP_Init(pInstance, eWindow);
}

void MDrv_XC_DWIN_init_swdb(void* pInstance,SCALER_DIP_WIN eWindow)
{
    SC_DIP_SWDB_INFO SWDBreg;

    SWDBreg.u32H_PreScalingRatio = 0x0;
    SWDBreg.u32V_PreScalingRatio = 0x0;
    SWDBreg.u16H_CapStart = 0x1;
    SWDBreg.u16H_CapSize  = 0x10;
    SWDBreg.u16V_CapStart = 0x1;
    SWDBreg.u16V_CapSize  = 0x10;

    MDrv_XC_DWIN_sw_db(pInstance,&SWDBreg, eWindow);

}

void MDrv_XC_DWIN_sw_db(void* pInstance,P_SC_DIP_SWDB_INFO pDBreg, SCALER_DIP_WIN eWindow )
{
    // Set ficlk -> sw_db  for prevent garbage when no Vpre-scaling down
    #if 0
    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetSkipWaitVsync(eWindow)) )
    {
        Hal_SC_DWIN_sw_db_burst(pInstance, pDBreg, eWindow );
    }
    else
    #endif
    {
		_XC_ENTRY(pInstance);
        Hal_SC_DWIN_sw_db( pInstance, pDBreg, eWindow );
		_XC_RETURN(pInstance);
    }
}

void MDrv_XC_DWIN_SetInputSource(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, SCALER_DIP_WIN eWindow)
{
    static INPUT_SOURCE_TYPE_t enOldDWINInputSrc = INPUT_SOURCE_NONE;

    if(enOldDWINInputSrc == enInputSourceType)
    {
        //printf("MApi_XC_SetInputSource[Win %u] return because old src %d -> %d\n", eWindow, enOldInputSrc[eWindow], enInputSourceType);
        return;
    }
    else
    {
        enOldDWINInputSrc = enInputSourceType;
    }

    MS_U8 u8InputSrcSel, u8SyncSel, u8VideoSel=0, u8isYPbPr;

#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
#if SUPPORT_IP_HDMI_FOR_HV_MODE
	_XC_ENTRY(pInstance);
    // Should disable "pixel repetition" for other source
    if(!(IsSrcTypeHDMI(enInputSourceType) || IsSrcTypeDVI(enInputSourceType)))
    {
        Hal_SC_DWIN_set_fir_down_sample_divider(pInstance, DISABLE, 0x00, eWindow); //Reset Pixel repetition, when switch hv mode
    }
	_XC_RETURN(pInstance);
#endif
    if(IsSrcTypeVga(enInputSourceType))
    {
        u8InputSrcSel = IP_ANALOG1;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = FALSE;
        //MApi_XC_ADC_SwitchSource(1); // Force ADC to reset after source switch
    }
    else if(IsSrcTypeYPbPr(enInputSourceType))
    {
        u8InputSrcSel = IP_ANALOG1;
        u8SyncSel = (SYNC_ON_GREEN << 1) | SOG;
        u8isYPbPr = TRUE;
        //MApi_XC_ADC_SwitchSource(1); // Force ADC to reset after source switch
    }
    else if(IsSrcTypeHDMI(enInputSourceType))
    {
#if SUPPORT_IP_HDMI_FOR_HV_MODE
        u8InputSrcSel = IP_HDMI;
        u8VideoSel = IP_HDMI;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
#else
        if( MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_DE)
        {
            u8InputSrcSel = IP_HDMI;
            u8VideoSel = IP_HDMI;
            u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        }
        else
        {
            u8InputSrcSel = IP_VIDEO;
            u8VideoSel = IP_MST_VD_A;
            u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        }
#endif
        u8isYPbPr = TRUE;

    }
    else if(IsSrcTypeDigitalVD(enInputSourceType))
    {
        u8InputSrcSel = IP_VIDEO;
        u8VideoSel = IP_MST_VD_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
    }
    #ifdef __EXTVD
    else if(IsUseExtVDPort(enInputPortType))
    {
        u8InputSrcSel = IP_VIDEO;
        u8VideoSel = IP_CCIR656_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
    }
    #endif
    else if(IsSrcTypeDTV(enInputSourceType) || (IsSrcTypeStorage(enInputSourceType)))
    {
        u8InputSrcSel = IP_VIDEO;
        u8VideoSel = IP_MST_VD_A;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = TRUE;
    }
    else if(IsSrcTypeCapture(enInputSourceType))
    {
        u8InputSrcSel = IP_DVI;
        u8SyncSel = (AUTO_DETECT << 1) | CSYNC;
        u8isYPbPr = FALSE;
    }
    else
    {
        //printf("Unknow Input Type \n");
        return;
    }
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
	_XC_ENTRY(pInstance);
    Hal_SC_DWIN_SetInputSource( pInstance, u8InputSrcSel, u8SyncSel, u8VideoSel, u8isYPbPr, eWindow);
    if(IsSrcTypeVga(enInputSourceType))
    {
        Hal_SC_DWIN_set_image_wrap( pInstance, DISABLE, DISABLE, eWindow );
    }
    else
    {
        Hal_SC_DWIN_set_image_wrap( pInstance, ENABLE, ENABLE, eWindow );
    }

    Hal_SC_DWIN_set_de_bypass_mode(pInstance, DISABLE, eWindow); // fix HDMI <-> component switch problems

    if(IsSrcTypeVga(enInputSourceType))
    {
        Hal_SC_DWIN_set_input_sync_reference_edge( pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        Hal_SC_DWIN_set_input_vsync_delay( pInstance, DELAY_QUARTER_HSYNC, eWindow );
        Hal_SC_DWIN_set_de_only_mode( pInstance, DISABLE, eWindow );
        Hal_SC_DWIN_set_input_10bit( pInstance, ENABLE, eWindow );

        Hal_SC_DWIN_set_coast_input( pInstance, PC_SOURCE, eWindow );
        Hal_SC_DWIN_set_ms_filter(pInstance, ENABLE, 0, eWindow);
        Hal_SC_DWIN_set_coast_window( pInstance, 0x0C, 0x0C, eWindow );
        Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, NORMAL_MODE, eWindow );
        Hal_SC_DWIN_set_post_glitch_removal( pInstance, DISABLE, 0x00, eWindow );
    }
    else if(IsSrcTypeYPbPr(enInputSourceType))
    {
        Hal_SC_DWIN_set_input_sync_reference_edge( pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        Hal_SC_DWIN_set_input_vsync_delay( pInstance, NO_DELAY, eWindow );
        Hal_SC_DWIN_set_ms_filter(pInstance, ENABLE, 0, eWindow);
        Hal_SC_DWIN_set_de_only_mode( pInstance, DISABLE, eWindow );
        Hal_SC_DWIN_set_input_10bit( pInstance, ENABLE, eWindow );

        Hal_SC_DWIN_set_coast_input( pInstance, COMPONENT_VIDEO, eWindow);
        Hal_SC_DWIN_set_coast_window( pInstance, 0x0C, 0x0C, eWindow );
        Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, GLITCH_REMOVAL, eWindow );
        Hal_SC_DWIN_set_post_glitch_removal( pInstance, ENABLE, 0x01, eWindow );
    }
    else if(IsSrcTypeHDMI(enInputSourceType))
    {
        Hal_SC_DWIN_set_input_sync_reference_edge( pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        Hal_SC_DWIN_set_input_vsync_delay( pInstance, DELAY_QUARTER_HSYNC, eWindow );

        if( MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_DE)
        {
            Hal_SC_DWIN_set_input_vsync_delay( pInstance, DELAY_QUARTER_HSYNC, eWindow );
            Hal_SC_DWIN_set_de_only_mode( pInstance, ENABLE, eWindow );
            Hal_SC_DWIN_set_de_bypass_mode(pInstance, DISABLE, eWindow);
            Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, NORMAL_MODE, eWindow );
            Hal_SC_DWIN_set_post_glitch_removal( pInstance, ENABLE, 0x07, eWindow );
            //Hal_HDMI_Set_YUV422to444_Bypass(TRUE);
        }
        else
        {
            Hal_SC_DWIN_set_input_vsync_delay( pInstance, NO_DELAY, eWindow );
            Hal_SC_DWIN_set_de_only_mode( pInstance, DISABLE, eWindow );
            // Use DE as capture window
            Hal_SC_DWIN_set_de_bypass_mode(pInstance, ENABLE, eWindow);
            // Disable scaling for get correct DE.
            //MDrv_SC_Enable_PreScaling(FALSE, FALSE, eWindow);
            // Set full range capture size for de-bypass mode
            //MDrv_SC_SetFullRangeCapture(eWindow);

            Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, GLITCH_REMOVAL, eWindow );
            Hal_SC_DWIN_set_post_glitch_removal( pInstance, ENABLE, 0x01, eWindow );
            //al_HDMI_Set_YUV422to444_Bypass(FALSE);
        }

        Hal_SC_DWIN_set_input_10bit( pInstance, ENABLE, eWindow );

        Hal_SC_DWIN_set_ms_filter(pInstance, DISABLE, 0, eWindow);

        //MDrv_SC_ip_set_coast_input( PC_SOURCE, eWindow );
        Hal_SC_DWIN_set_ms_filter(pInstance, DISABLE, 0, eWindow);
        //MDrv_SC_ip_set_coast_window( 0x0C, 0x0C, eWindow );
        Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, NORMAL_MODE, eWindow );
        Hal_SC_DWIN_set_post_glitch_removal( pInstance, ENABLE, 0x07, eWindow );
    }
    else if(IsSrcTypeDigitalVD(enInputSourceType)
    #ifdef __EXTVD
    | IsUseExtVDPort(enInputPortType)
    #endif
    )
    {
        Hal_SC_DWIN_set_input_sync_reference_edge( pInstance, TAILING_EDGE, TAILING_EDGE, eWindow );
        Hal_SC_DWIN_set_input_vsync_delay( pInstance, DELAY_QUARTER_HSYNC, eWindow );
        Hal_SC_DWIN_set_de_only_mode( pInstance, DISABLE, eWindow );

        #ifdef __EXTVD
        if(IsUseExtVDPort(enInputPortType))
        {
            Hal_SC_DWIN_set_input_10bit( pInstance, DISABLE, eWindow );

            //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
            //MDrv_WriteByte(L_BK_IP1F2(0x08), (MDrv_ReadByte(L_BK_IP1F2(0x08)) & 0x7F));
            //u16tmp = SC_R2BYTE(REG_SC_BK01_08_L);
            //SC_W2BYTEMSK(REG_SC_BK01_08_L, u16tmp & 0x007F, 0x00FF);
        }
        else
        #endif

        {
            Hal_SC_DWIN_set_input_10bit( pInstance, ENABLE, eWindow );
        }
        //MDrv_SC_ip_set_coast_input( PC_SOURCE, eWindow );
        Hal_SC_DWIN_set_ms_filter(pInstance, DISABLE, 0, eWindow);
        //MDrv_SC_ip_set_coast_window( 0x0C, 0x0C, eWindow );
        Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, NORMAL_MODE, eWindow );
        Hal_SC_DWIN_set_post_glitch_removal( pInstance, DISABLE, 0x00, eWindow );
    }
    else if(IsSrcTypeCapture(enInputSourceType))
    {
        Hal_SC_DWIN_set_input_sync_reference_edge( pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        Hal_SC_DWIN_set_input_vsync_delay( pInstance, DELAY_QUARTER_HSYNC, eWindow );
        Hal_SC_DWIN_set_de_only_mode( pInstance, DISABLE, eWindow );
        Hal_SC_DWIN_set_input_10bit( pInstance, ENABLE, eWindow );

        Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, GLITCH_REMOVAL, eWindow );
        Hal_SC_DWIN_set_post_glitch_removal( pInstance, ENABLE, 0x01, eWindow );
    }
    else //DTV
    {
        //set HSYNC reference to leading edge, the capature start will be 1/2 Hsync when input source is from DC0/DC1
        Hal_SC_DWIN_set_input_sync_reference_edge( pInstance, LEADING_EDGE, TAILING_EDGE, eWindow );
        Hal_SC_DWIN_set_input_vsync_delay( pInstance, DELAY_QUARTER_HSYNC, eWindow );
        Hal_SC_DWIN_set_de_only_mode( pInstance, DISABLE, eWindow );
        Hal_SC_DWIN_set_input_10bit( pInstance, ENABLE, eWindow );

        //MDrv_SC_ip_set_coast_input( PC_SOURCE, eWindow );
        Hal_SC_DWIN_set_ms_filter(pInstance, DISABLE, 0, eWindow);
        //MDrv_SC_ip_set_coast_window( 0x0C, 0x0C, eWindow );
        Hal_SC_DWIN_set_input_sync_sample_mode( pInstance, NORMAL_MODE, eWindow );
        Hal_SC_DWIN_set_post_glitch_removal( pInstance, DISABLE, 0x00, eWindow );
    }

    // set  h0027 [2] : true to enable DE only mode Glitch Protect for position
    // to fix HDMI 720P YUV422 HStart error & HDE lack of 1
    if (IsSrcTypeHDMI(enInputSourceType))
        Hal_SC_DWIN_set_DE_Mode_Glitch(pInstance, 0x24, eWindow); // VSync glitch removal with line less than 2(DE only)
    else
        Hal_SC_DWIN_set_DE_Mode_Glitch(pInstance, 0x04, eWindow); // CHANNEL_ATTRIBUTE function control
    MDrv_XC_DWIN_Scaler_Field_Detect(pInstance,enInputSourceType, GOPDWIN_SCAN_MODE_AUTO, eWindow );
    g_DIPSrcInfo[eWindow].enInputSourceType = enInputSourceType;
	_XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MDrv_XC_DWIN_GetInputSource(void* pInstance,INPUT_SOURCE_TYPE_t *enInputSourceType, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    switch(eWindow)
    {
        case DIP_WINDOW :
            *enInputSourceType = g_DIPSrcInfo[DIP_WINDOW].enInputSourceType;
            break;
        default :
            *enInputSourceType = INPUT_SOURCE_NONE;
            break;
    }
}

void MDrv_XC_DWIN_DisableInputSource(void* pInstance, MS_BOOL bDisable, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);

    #if 0
    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetSkipWaitVsync(eWindow) ) )
    {
        _MLOAD_ENTRY(pInstance);

        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK33_02_L, (bDisable ? BIT(7):0), BIT(7));

        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    #endif
    {
        Hal_SC_DWIN_DisableInputSource(pInstance, bDisable, eWindow);
    }
    _XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_SetFrameBufferAddress(void* pInstance,MS_PHY u64FBAddress, MS_PHY u64FBSize, SCALER_DIP_WIN eWindow)
{

}

void MDrv_XC_DWIN_set_capture_v_start(void* pInstance,MS_U16 u16Vstart , SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
	g_DIPSrcInfo[eWindow].stCapWin.y = u16Vstart;
	_XC_ENTRY(pInstance);
    Hal_SC_DWIN_set_capture_v_start( pInstance, u16Vstart, eWindow);
	_XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_set_capture_h_start(void* pInstance,MS_U16 u16Vstart , SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
	g_DIPSrcInfo[eWindow].stCapWin.x = u16Vstart;
	_XC_ENTRY(pInstance);
    Hal_SC_DWIN_set_capture_h_start( pInstance, u16Vstart, eWindow);
	_XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_set_capture_v_size(void* pInstance,MS_U16 u16Vsize , SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
	g_DIPSrcInfo[eWindow].stCapWin.height = u16Vsize;
	_XC_ENTRY(pInstance);
    Hal_SC_DWIN_set_capture_v_size( pInstance, u16Vsize, eWindow);
	_XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_set_capture_h_size(void* pInstance,MS_U16 u16Hsize , SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    g_DIPSrcInfo[eWindow].stCapWin.width = u16Hsize;
	_XC_ENTRY(pInstance);
    Hal_SC_DWIN_set_capture_h_size( pInstance, u16Hsize, eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_get_capture_window(void* pInstance,MS_WINDOW_TYPE* capture_win,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_DWIN_get_capture_window(pInstance, capture_win, eWindow);
    _XC_RETURN(pInstance);
}

/******************************************************************************/
///Set Capture window (Input window) information.
///@param pstWindow \b IN
///- pointer to window information
/******************************************************************************/
void MDrv_XC_DWIN_set_capture_window(void* pInstance,SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    // HDMI HV mode uses DE by pass, so capture win should be set as full range
#if 0 //DIP do not need this, only XC_IP1/2 need this
    if(0)//( ( IsSrcTypeHDMI( g_DIPSrcInfo[eWindow].enInputSourceType ) ) && (MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_HV) )
    {
        // Capture win must keep full range in HV mode
        stDIP_DBreg.u16V_CapStart = 0xC;
        stDIP_DBreg.u16H_CapStart = 0xC;
        stDIP_DBreg.u16V_CapSize = 0x1FFF;
        stDIP_DBreg.u16H_CapSize = 0x1FFF;
    }
    else
#endif
    if(g_DIPSrcInfo[eWindow].bInterlace)
    {
        //For interlace, set clip window V Pos/Size= field Pos/Size
        stDIP_DBreg[eWindow].u16V_CapStart = (g_DIPSrcInfo[eWindow].stCapWin.y)>>1;
        stDIP_DBreg[eWindow].u16H_CapStart = g_DIPSrcInfo[eWindow].stCapWin.x;
        stDIP_DBreg[eWindow].u16V_CapSize  = (g_DIPSrcInfo[eWindow].stCapWin.height)>>1;
        stDIP_DBreg[eWindow].u16H_CapSize  = g_DIPSrcInfo[eWindow].stCapWin.width;
    }
    else
    {
        stDIP_DBreg[eWindow].u16V_CapStart = g_DIPSrcInfo[eWindow].stCapWin.y;
        stDIP_DBreg[eWindow].u16H_CapStart = g_DIPSrcInfo[eWindow].stCapWin.x;
        stDIP_DBreg[eWindow].u16V_CapSize  = g_DIPSrcInfo[eWindow].stCapWin.height;
        stDIP_DBreg[eWindow].u16H_CapSize  = g_DIPSrcInfo[eWindow].stCapWin.width;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Set DIP ClipWin x:%u, y:%u, w:%u, h:%u\n", stDIP_DBreg[eWindow].u16H_CapStart, stDIP_DBreg[eWindow].u16H_CapSize, stDIP_DBreg[eWindow].u16V_CapStart, stDIP_DBreg[eWindow].u16V_CapSize);
}

void MDrv_XC_DWIN_get_DE_Window(void* pInstance,MS_WINDOW_TYPE* psWin, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_DWIN_get_de_window(pInstance, psWin, eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_EnableR2YCSC(void* pInstance,MS_BOOL benable, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_DWIN_EnableR2YCSC( pInstance, benable, eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_set_delayline(void* pInstance,MS_U8 u8DelayLines, SCALER_DIP_WIN eWindow )
{
    _XC_ENTRY(pInstance);
    Hal_SC_DWIN_set_delayline(pInstance, u8DelayLines,eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_set_prescaling_ratio(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow)
{
    MS_U32 u32TmpPre;
    MS_U16 u16ScaleSrc = 0, u16ScaleDst = 0;
    MS_BOOL bEnPreDown = 0;
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
    MS_U16 u16SrcHWidth = 0;
    MS_U64 u64_result = 0;

    // Store Pre-scaling source / dest size of previouse
    static MS_U16 _u16HScaleSrc[MAX_DIP_WINDOW] = {0} ,_u16HScaleDst[MAX_DIP_WINDOW] = {0};
    static MS_U16 _u16VScaleSrc[MAX_DIP_WINDOW] = {0} ,_u16VScaleDst[MAX_DIP_WINDOW] = {0};

    //-----------------------------------------
    // Vertical
    //-----------------------------------------
    if (pSrcInfo->bPreVCusScaling == TRUE)
    {
        u16ScaleSrc = pSrcInfo->u16PreVCusScalingSrc;
        u16ScaleDst = pSrcInfo->u16PreVCusScalingDst;
    }
    else
    {
        // Put full image into memory as default
        //u16ScaleSrc = u16ScaleDst = pSrcInfo->stCapWin.height;
        // for DIP do pre scaling no post scaling.
        u16ScaleSrc = pSrcInfo->stCapWin.height;
        u16ScaleDst = pSrcInfo->stCapWin.height;
    }


    if( bInterlace )
    {
        if (u16ScaleDst % 2 )
        {
            XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"==============================================================\n");
            XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"======= Pre-scaling V size have to align 2 !!!!!!!!===========\n");
            XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"==============================================================\n");
            MS_ASSERT(0);
            u16ScaleDst = ( u16ScaleDst + 1) & ~0x1;
        }
    }


    // Check prescaling status. Set bPreScalingRatioChanged as true only when source/dest is different with previous source/dest
    // We must update this information before pre-scaling filter.
    //(1. We can skip pre-scaling filter update. 2. We can decide which method we can use for load registers. DS or Menuload.)
    if ( (_u16VScaleSrc[eWindow] != u16ScaleSrc) || (_u16VScaleDst[eWindow] != u16ScaleDst) )
    {

        _u16VScaleSrc[eWindow] = u16ScaleSrc;
        _u16VScaleDst[eWindow] = u16ScaleDst;

    }
    else
    {
    }

    _XC_ENTRY(pInstance);

    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"[PRE]eWindow[%u] VSrc=%u, VDst=%u\n",eWindow , u16ScaleSrc, u16ScaleDst);

    if (u16ScaleSrc > u16ScaleDst) //pre scaling only support scaling down
    {
        //MS_U32 u32Value = 0;

        if (u16ScaleSrc == 0)
        {
            u32TmpPre = 0;
        }
        else
        {
            u32TmpPre = V_PreScalingDownRatioDIP(u16ScaleSrc, u16ScaleDst);
        }

        u32TmpPre &= 0xFFFFFFL;

        u32TmpPre |= 0x80000000L;

#if 0
        // Error checking
        if (u16ScaleSrc == 0)
            u32Value = 0;
        else
            u32Value = (MS_U32)u16ScaleDst * 0x80000 / u16ScaleSrc;
#endif
        //HAL_SC_Set_VInitFactorTwo(u32Value,eWindow);
        Hal_SC_DWIN_Set_vsd_output_line_count(pInstance, DISABLE, u16ScaleDst, eWindow);
        Hal_SC_DWIN_Set_vsd_input_line_count(pInstance, DISABLE, FALSE, 0, eWindow);

    }
    else // no pre-V scaling case
    {
        u16ScaleSrc = pSrcInfo->u16PreVCusScalingSrc;
        u16ScaleDst = pSrcInfo->u16PreVCusScalingDst;
        u32TmpPre = 0;
        Hal_SC_DWIN_Set_vsd_output_line_count(pInstance, DISABLE, u16ScaleDst, eWindow);
        Hal_SC_DWIN_Set_vsd_input_line_count(pInstance, DISABLE, FALSE, 0, eWindow);
        //HAL_SC_Set_VInitFactorTwo(0x80000,eWindow);
    }

     _XC_RETURN(pInstance);

    //store prescaling info here ( Why using u16PreVCusScalingSrc and u16PreVCusScalingDst??)
    pSrcInfo->u16PreVCusScalingSrc = u16ScaleSrc;
    pSrcInfo->u16PreVCusScalingDst = u16ScaleDst;
    pSrcInfo->u16V_SizeAfterPreScaling = u16ScaleDst;

    stDIP_DBreg[eWindow].u32V_PreScalingRatio = u32TmpPre;

    //-----------------------------------------
    // Horizontal
    // HSD rule: 3D scaling > customer scaling > QMap HSD sampling > auto fit
    //-----------------------------------------

    // Note!! Formula:  FHD - If  (H Crop size /2 ) > ( H display size ), Must-do H pre-scaling down.

    {
        // Custimize
        if(pSrcInfo->bPreHCusScaling)
        {
            u16SrcHWidth = pSrcInfo->u16PreHCusScalingSrc;
            if (u16SrcHWidth > pSrcInfo->u16PreHCusScalingDst)
            {
                pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->u16PreHCusScalingDst;
            }
            else
            {
                pSrcInfo->u16H_SizeAfterPreScaling = u16SrcHWidth;
            }
            //MDrv_SC_3D_Adjust_PreHorDstSize(enInputSourceType, pSrcInfo, eWindow);
            // 3d adjust may make the pre h dst bigger than cus_pre_H, we need adjust it again.
            if (pSrcInfo->u16H_SizeAfterPreScaling > pSrcInfo->u16PreHCusScalingDst)
            {
                pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->u16PreHCusScalingDst;
            }
            XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Customer H scaling: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
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
                MS_U16 capwidth = 0;

                u64_result = pSrcInfo->stCapWin.width;
                do_div(u64_result,2);
                capwidth = u64_result;
                {
                    pSrcInfo->u16H_SizeAfterPreScaling = capwidth;
                }
            }
         //!!NOTE START:
         //if QMAP support HSD Sampling, this item need to disable and the scaling size will control by QMAP
         //If we want to follow the HSD rule, we cannot enable ENABLE_YPBPR_PRESCALING_TO_ORIGINAL and
         // ENABLE_VD_PRESCALING_TO_DOT75
         #if (ENABLE_YPBPR_PRESCALING_TO_ORIGINAL)
            else if(IsSrcTypeYPbPr(enInputSourceType) && pSrcInfo->bHDuplicate)
            {
                u64_result = pSrcInfo->stCapWin.width;
                do_div(u64_result,2);
                pSrcInfo->u16H_SizeAfterPreScaling = u64_result;
               XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"ENABLE_YPBPR_PRESCALING_TO_ORIGINAL: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
            }
         #endif
         //!!NOTE END
         #if (ENABLE_VD_PRESCALING_TO_DOT75)
            else if(IsSrcTypeDigitalVD(enInputSourceType))
            {
                u64_result = pSrcInfo->stCapWin.width* 3 ;
                do_div(u64_result,4);
                pSrcInfo->u16H_SizeAfterPreScaling = u64_result;
               XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"ENABLE_VD_PRESCALING_TO_DOT75: eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);
            }
         #endif
            else
            {
                // H scaling controlled by Qmap.
            //    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"[PRE]Force Prescaling Down : %d  _bPQSetPreScalingrate : %d \n",bForcePreHScalingDown, MDrv_SC_GetPQHSDFlag());
            }
         }
    }
    //Setting Line Buffer Start offset for Sub Win

    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);

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

    // Check prescaling status. Set bPreScalingRatioChanged as true only when src != dest.
    // We update this information before pre-scaling filter.
    if ( (_u16HScaleSrc[eWindow] != u16SrcHWidth) || (_u16HScaleDst[eWindow] != pSrcInfo->u16H_SizeAfterPreScaling) )
    {

        _u16HScaleSrc[eWindow] = u16SrcHWidth;
        _u16HScaleDst[eWindow] = pSrcInfo->u16H_SizeAfterPreScaling;
    }
    else
    {
    }

    //printf("bEnPreDown=%u, bAdvMode=%u\n", (MS_U16)bEnPreDown, (MS_U16)bAdvMode);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"[PRE]eWindow[%u] HSrc=%u, HDst=%u\n", eWindow, u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling);

    if (u16SrcHWidth > pSrcInfo->u16H_SizeAfterPreScaling)
    {
        MS_U16 u16div = 1;

        // To avoid overflow when do H_PreScalingDownRatioAdv/H_PreScalingDownRatioAdv_No_Minus1
        if(u16SrcHWidth > 4096)
        {
            while(1)
            {
                MS_U16 u16ret;

                u16div <<= 1;

                if (u16div == 0)
                {
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

        // disable prescaling filter, use cb mode scaling ratio
        if (u16SrcHWidth == 0 )
            u32TmpPre = 0;
        else
            u32TmpPre = (H_PreScalingDownRatioDIP(u16SrcHWidth, pSrcInfo->u16H_SizeAfterPreScaling));

        bEnPreDown = 1;
        u32TmpPre &= 0x7FFFFFL;
    }
    else
    {
        pSrcInfo->u16H_SizeAfterPreScaling = pSrcInfo->u16PreHCusScalingDst;
        {
            bEnPreDown = 0;
            u32TmpPre = 0x100000L;
        }

    }
    //store prescaling info here
    pSrcInfo->u16PreHCusScalingSrc = u16SrcHWidth;
    pSrcInfo->u16PreHCusScalingDst = pSrcInfo->u16H_SizeAfterPreScaling;

    if (bEnPreDown)
    {
        u32TmpPre |= 0x80000000L;
    }

    stDIP_DBreg[eWindow].u32H_PreScalingRatio = u32TmpPre;

}

void _Mdrv_XC_DWIN_CalcFBNum(void* pInstance,MS_BOOL *pIsLinearMode,
                        XC_FRAME_STORE_NUMBER *pFrameStoreNumber,
                        MS_BOOL bInterlace,
                        MS_PHY u64Framesize,
                        MS_PHY u64DNRBufSize,
                        SCALER_DIP_WIN eWindow)
{
    // For DWIN debg mode, use 4 frame for I mode, and 2 frame for P mode
    if(bInterlace)
    {
        *pFrameStoreNumber = IMAGE_STORE_4_FIELDS;
    }
    else
    {
        *pFrameStoreNumber = IMAGE_STORE_2_FRAMES;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"u8BestFBNum %d \n",*pFrameStoreNumber);

}
// for mirror mode and dynamic scaling store u32BaseOffset

void MDrv_XC_DWIN_set_storeFrameNUM(void* pInstance,SCALER_DIP_WIN eWindow,MS_U8 u8FrameNum)
{

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
void MDrv_XC_DWIN_set_fetch_number_limit(void* pInstance,XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow)
{

    /////////////////////////////////////////////////
    // Set DNR Memeory Settings                    //
    /////////////////////////////////////////////////
    // Cal DNR Fetch/Offset
    stDIP_DBreg[eWindow].u16DNRFetch  = pSrcInfo->u16H_SizeAfterPreScaling;
	stDIP_DBreg[eWindow].u16DNROffset = (stDIP_DBreg[eWindow].u16DNRFetch + OFFSET_PIXEL_ALIGNMENT - 1) & ~(OFFSET_PIXEL_ALIGNMENT-1);  //TODO: T3 should be 32 alignment;

	XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"u16DNRFetch=%u\n",stDIP_DBreg[eWindow].u16DNRFetch);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"u16DNROffset=%u\n",stDIP_DBreg[eWindow].u16DNROffset);

 }
void MDrv_XC_DWIN_EnableIPAutoCoast(void* pInstance,MS_BOOL bEnable, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_DWIN_EnableIPAutoCoast(pInstance, bEnable, eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DWIN_Scaler_Field_Detect(void* pInstance,INPUT_SOURCE_TYPE_t enInputSrcType, EN_XC_DWIN_SCAN_TYPE enScanType, SCALER_DIP_WIN eWindow)
{

    MS_U8 reg_IP1F2_1D = 0;
    MS_U8 reg_IP1F2_23 = 0;
    MS_U16 reg_IP1F2_21 = 0;
    //MS_U16 u16Vtt = 0;

    // Note:
    // IP1F2_1D[13]: Enable auto no signal filter mode.
    // This functionality is to improve timing detection stability.

    if(IsSrcTypeDigitalVD(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "VD \n");
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_21 = 0x0403;
        reg_IP1F2_23 = 0x30;
    }
    else if(IsSrcTypeDTV(enInputSrcType) || IsSrcTypeStorage(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "DTV ");
        if (enScanType == GOPDWIN_SCAN_MODE_INTERLACE)
        {
            reg_IP1F2_1D = 0xA1;
            reg_IP1F2_21 = 0x0403;
            reg_IP1F2_23 = 0x30;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0400;
            reg_IP1F2_23 = 0x00;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
        }
    }
    else if (IsSrcTypeHDMI(enInputSrcType))
    {

        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "HDMI ");
        if (enScanType == GOPDWIN_SCAN_MODE_INTERLACE)
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0003;      // enable DE -> no field invert, disable DE -> use field invert
            reg_IP1F2_23 = 0x20;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0000;
            reg_IP1F2_23 = 0x00;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
        }

        reg_IP1F2_21 |= BIT(12);

        if( (MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_HV)
          &&(enScanType == GOPDWIN_SCAN_MODE_INTERLACE) )
        {
            MS_U16 u16Cur_IP1F2_21Val = SC_R2BYTE(0,REG_SC_BK01_21_L);
            if( u16Cur_IP1F2_21Val&0xC000 )
            {
                if( reg_IP1F2_21&BIT(8) )
                {
                    reg_IP1F2_21 &= ~BIT(8);
                }
                else
                {
                    reg_IP1F2_21 |= BIT(8);
                }
                //printf("daniel_test patch REG_SC_BK01_21_L: FieldInvert\n");
            }
            else
            {
            }
        }

    }
    else if(IsSrcTypeVga(enInputSrcType))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "DSUB ");
        if (enScanType == GOPDWIN_SCAN_MODE_INTERLACE)
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0103;
            reg_IP1F2_23 = 0x10;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Interlace\n");
        }
        else
        {
            reg_IP1F2_1D = 0x21;
            reg_IP1F2_21 = 0x0000;
            reg_IP1F2_23 = 0x00;
            XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "-> Progressive\n");
        }
    }
    else // YPbPr
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "YPbPr ");

        if(MDrv_XC_ip_GetEuroHdtvStatus(pInstance, MAIN_WINDOW) == EURO_AUS_HDTV_NORMAL)
        {
            reg_IP1F2_21 = 0x0100;
        }
        else
        {
            reg_IP1F2_21 = 0x0000;
        }
        reg_IP1F2_1D = 0xA1;
        reg_IP1F2_23 = 0x08; //0x20  // Vtt will use "IP1F2_23 * 16" as lower bound.
    }
    Hal_SC_DWIN_setfield(pInstance, reg_IP1F2_1D, reg_IP1F2_21, reg_IP1F2_23, eWindow);
}

void MDrv_XC_DWIN_Disable_IPM_ReadWriteRequest(void* pInstance,MS_BOOL bDisable, SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    Hal_SC_DWIN_Disable_IPM_ReadWriteRequest(pInstance, bDisable, eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DWIN_SetMode(void* pInstance,INPUT_SOURCE_TYPE_t enInputSourceType, XC_DIP_InternalStatus *pSrcInfo, SCALER_DIP_WIN eWindow )
{
    MS_BOOL bInterlace = pSrcInfo->bInterlace;
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //MS_BOOL bUseYUVSpace;
    //MS_U8 interlace_type;
    //----------------------------------------------------
    // Setup Memory format
    //----------------------------------------------------
    if(s_DIPeMemFmt[eWindow] == E_MS_XC_MEM_FMT_AUTO)
    {
        if(!bInterlace &&
          ((IsSrcTypeHDMI(enInputSourceType) && !pXCResourcePrivate->stdrv_HDMI.enStatus_info.b_St_HDMI_Mode) || // DVI plugged into HDMI connector
          IsSrcTypeVga(enInputSourceType)  ) )
        {   // always keep memory foramt to 444 for corresponding to Y/C post-scaling filter
            pSrcInfo->bMemFmt422 = FALSE;
        }
        else
        {   // if input source is 422, HW will convert to 444 automatically
            pSrcInfo->bMemFmt422 = TRUE;
        }
    }
    else
    {
        if(s_DIPeMemFmt[eWindow] == E_MS_XC_MEM_FMT_422)
            pSrcInfo->bMemFmt422 = TRUE;
        else
            pSrcInfo->bMemFmt422 = FALSE;
    }

    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    msg_video(DIP_D_INFO("MemFmt422=%u\n", (MS_U16)pSrcInfo->bMemFmt422));

#if 0
    if(s_PQ_Function_Info.pq_get_memyuvfmt == NULL)
        pSrcInfo->bMemYUVFmt =  TRUE;
    else
        pSrcInfo->bMemYUVFmt = s_PQ_Function_Info.pq_get_memyuvfmt(eWindow == DIP_SUB_WINDOW ? PQ_SUB_WINDOW : PQ_MAIN_WINDOW,
                                  g_DIPSrcInfo[eWindow].bForceRGBin == ENABLE ? PQ_FOURCE_COLOR_RGB : PQ_FOURCE_COLOR_DEFAULT);
#endif
	_XC_ENTRY(pInstance);
    //-------------------------------------------
    // Set De-interlaced mode and Memory format
    //-------------------------------------------
    //printf("==> Set PQ MEMORY Mode: %d\n", eWindow);

    // copy MADI mode from IP1
    MS_U16 u16MADImode = SC_R2BYTE(0,REG_SC_BK12_01_L);
    SC_W2BYTE(0,REG_SC_BK35_01_L, u16MADImode);

    //pSrcInfo->bInterlace = SC_R2BYTEMSK(REG_SC_BK33_1E_L, BIT(11)) >> 11;
    //pSrcInfo->bInterlace = (pSrcInfo->bInterlace || (SC_R2BYTEMSK(REG_SC_BK35_01_L, BIT(10)) >> 10));

    if (IsSrcTypeVga(enInputSourceType))
        {
		    SC_W2BYTEMSK(0, REG_SC_BK35_02_L, 0x311 , BITMASK(9:0) );
        }
    else
        {
		    SC_W2BYTEMSK(0, REG_SC_BK35_02_L, 0x3BB , BITMASK(9:0) );
        }
	_XC_RETURN(pInstance);
    //printf("DI mode =%x, bitPerPixel=%d\n",pSrcInfo->eDeInterlaceMode, pSrcInfo->u8BitPerPixel);
    // Setup delay line
    MDrv_XC_DWIN_set_delayline( pInstance,pSrcInfo->u8DelayLines, eWindow );           // set delay line for trigger point, for 2DDI

}



//#define _NOP_(_loop)           { volatile int i; for (i=0; i<(_loop); i++); }


static MS_BOOL _Mdrv_XC_DIP_CopySetWinInfo(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    if(pstXC_SetWin_Info == NULL)
    {
        return FALSE;
    }
    g_DIPSrcInfo[eWindow].enInputSourceType     = pstXC_SetWin_Info->enInputSourceType;

    g_DIPSrcInfo[eWindow].stCapWin.x            = pstXC_SetWin_Info->stCapWin.x;
    g_DIPSrcInfo[eWindow].stCapWin.y            = pstXC_SetWin_Info->stCapWin.y;
    g_DIPSrcInfo[eWindow].stCapWin.width        = pstXC_SetWin_Info->stCapWin.width;
    g_DIPSrcInfo[eWindow].stCapWin.height       = pstXC_SetWin_Info->stCapWin.height;

    g_DIPSrcInfo[eWindow].bInterlace            = pstXC_SetWin_Info->bInterlace;
    g_DIPSrcInfo[eWindow].bHDuplicate           = pstXC_SetWin_Info->bHDuplicate;
    g_DIPSrcInfo[eWindow].bHCusScaling          = pstXC_SetWin_Info->bHCusScaling;
    g_DIPSrcInfo[eWindow].u16HCusScalingSrc     = pstXC_SetWin_Info->u16HCusScalingSrc;
    g_DIPSrcInfo[eWindow].u16HCusScalingDst     = pstXC_SetWin_Info->u16HCusScalingDst;
    g_DIPSrcInfo[eWindow].bVCusScaling          = pstXC_SetWin_Info->bVCusScaling;
    g_DIPSrcInfo[eWindow].u16VCusScalingSrc     = pstXC_SetWin_Info->u16VCusScalingSrc;
    g_DIPSrcInfo[eWindow].u16VCusScalingDst     = pstXC_SetWin_Info->u16VCusScalingDst;

    return TRUE;
}

//=============== DIP =====================//
void MDrv_XC_DIP_SetWindow(void* pInstance,XC_SETWIN_INFO *pstXC_SetWin_Info, MS_U32 u32InitDataLen, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    //MDrv_SC_SetPQHSDFlag(FALSE); //Reset variable
    // we've to support interface changed without re-compile user application
    // please specify new added interface default value here

   HAL_XC_DIP_2P_Width_Check(pInstance, pstXC_SetWin_Info, eWindow);

#ifndef MSOS_TYPE_LINUX_KERNEL
    if(u32InitDataLen != sizeof(XC_SETWIN_INFO))
    {
        // customer application different version with our driver
        // start from customized pre H/V scaling
        g_DIPSrcInfo[eWindow].bPreHCusScaling = FALSE;
        g_DIPSrcInfo[eWindow].bPreVCusScaling = FALSE;
    }
    else
#endif
    {
        g_DIPSrcInfo[eWindow].bPreHCusScaling = pstXC_SetWin_Info->bPreHCusScaling;
        g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc = pstXC_SetWin_Info->u16PreHCusScalingSrc;
        g_DIPSrcInfo[eWindow].u16PreHCusScalingDst = pstXC_SetWin_Info->u16PreHCusScalingDst;

        if(g_DIPSrcInfo[eWindow].u16PreHCusScalingDst > g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"H can't do pre scaling up, change to auto prescaling\n");
            g_DIPSrcInfo[eWindow].bPreHCusScaling = FALSE;
        }

        if (g_DIPSrcInfo[eWindow].bPreHCusScaling)
        {
            if (g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc == 0 || g_DIPSrcInfo[eWindow].u16PreHCusScalingDst == 0)
            {
                XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"PreHCus Src or Dst can not be 0 \n");

                return;
            }
        }

        g_DIPSrcInfo[eWindow].bPreVCusScaling = pstXC_SetWin_Info->bPreVCusScaling;
        g_DIPSrcInfo[eWindow].u16PreVCusScalingSrc = pstXC_SetWin_Info->u16PreVCusScalingSrc;
        g_DIPSrcInfo[eWindow].u16PreVCusScalingDst = pstXC_SetWin_Info->u16PreVCusScalingDst;

        if(g_DIPSrcInfo[eWindow].u16PreVCusScalingDst > g_DIPSrcInfo[eWindow].u16PreVCusScalingSrc)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"V can't do pre scaling up, change to auto prescaling\n");
            g_DIPSrcInfo[eWindow].bPreVCusScaling = FALSE;
        }

        if (g_DIPSrcInfo[eWindow].bPreVCusScaling)
        {
            if (g_DIPSrcInfo[eWindow].u16PreVCusScalingSrc == 0 || g_DIPSrcInfo[eWindow].u16PreVCusScalingDst == 0  )
            {
                XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"PreVCus Src or Dst can not be 0 \n");

                return;
            }
        }
    }

    // copy user data to internal data
    // here is a patch first, will remove after seperate Internal XC status with set window information
    //u32CopyLen = sizeof(XC_SETWIN_INFO) - (sizeof(MS_BOOL)*2) - (sizeof(MS_U16)*4);
    //memcpy(&g_DIPSrcInfo[eWindow], pstXC_SetWin_Info, u32CopyLen);
    //do not use this copy command, strongly depend on structure definition
    _Mdrv_XC_DIP_CopySetWinInfo(pInstance,pstXC_SetWin_Info, eWindow);

    // assert check
    MS_ASSERT((g_DIPSrcInfo[eWindow].bHCusScaling==TRUE)?((g_DIPSrcInfo[eWindow].u16HCusScalingSrc!=0)&&(g_DIPSrcInfo[eWindow].u16HCusScalingDst!=0)):1);
    MS_ASSERT((g_DIPSrcInfo[eWindow].bVCusScaling==TRUE)?((g_DIPSrcInfo[eWindow].u16VCusScalingSrc!=0)&&(g_DIPSrcInfo[eWindow].u16VCusScalingDst!=0)):1);
    MS_ASSERT((g_DIPSrcInfo[eWindow].bPreHCusScaling==TRUE)?((g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc!=0)&&(g_DIPSrcInfo[eWindow].u16PreHCusScalingDst!=0)):1);
    MS_ASSERT((g_DIPSrcInfo[eWindow].bPreVCusScaling==TRUE)?((g_DIPSrcInfo[eWindow].u16PreVCusScalingSrc!=0)&&(g_DIPSrcInfo[eWindow].u16PreVCusScalingDst!=0)):1);

    // dump debug msg
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"============= SetWindow Start (Window : %u, src: %u) =============\n",eWindow, g_DIPSrcInfo[eWindow].enInputSourceType);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",g_DIPSrcInfo[eWindow].stCapWin.x, g_DIPSrcInfo[eWindow].stCapWin.y, g_DIPSrcInfo[eWindow].stCapWin.width, g_DIPSrcInfo[eWindow].stCapWin.height);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"H/V total = (%u, %u), VFreq = %u\n", pstXC_SetWin_Info->u16DefaultHtotal, pstXC_SetWin_Info->u16InputVTotal, pstXC_SetWin_Info->u16InputVFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Interlace/Hdup = %u/%u\n", pstXC_SetWin_Info->bInterlace, pstXC_SetWin_Info->bHDuplicate);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Pre  H cus scaling %u (%u -> %u)\n", g_DIPSrcInfo[eWindow].bPreHCusScaling, g_DIPSrcInfo[eWindow].u16PreHCusScalingSrc, g_DIPSrcInfo[eWindow].u16PreHCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Pre  V cus scaling %u (%u -> %u)\n", g_DIPSrcInfo[eWindow].bPreVCusScaling, g_DIPSrcInfo[eWindow].u16PreVCusScalingSrc, g_DIPSrcInfo[eWindow].u16PreVCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Post H cus scaling %u (%u -> %u)\n", g_DIPSrcInfo[eWindow].bHCusScaling, g_DIPSrcInfo[eWindow].u16HCusScalingSrc, g_DIPSrcInfo[eWindow].u16HCusScalingDst);
    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Post V cus scaling %u (%u -> %u)\n", g_DIPSrcInfo[eWindow].bVCusScaling, g_DIPSrcInfo[eWindow].u16VCusScalingSrc, g_DIPSrcInfo[eWindow].u16VCusScalingDst);

    // Capture width & height can not be 0 !!
    if ((0 == g_DIPSrcInfo[eWindow].stCapWin.width) || (0 == g_DIPSrcInfo[eWindow].stCapWin.height))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"Cap/Crop/Disp width or height can not be 0 \n");

        return;
    }

    //MDrv_XC_DWIN_SetMode(pInstance,g_DIPSrcInfo[eWindow].enInputSourceType, &g_DIPSrcInfo[eWindow], eWindow);

    MDrv_XC_DWIN_set_prescaling_ratio(pInstance,g_DIPSrcInfo[eWindow].enInputSourceType, &g_DIPSrcInfo[eWindow], eWindow );

    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"CapWin  x: %4u  y: %4u  w: %4u  h: %4u \n",g_DIPSrcInfo[eWindow].stCapWin.x, g_DIPSrcInfo[eWindow].stCapWin.y, g_DIPSrcInfo[eWindow].stCapWin.width, g_DIPSrcInfo[eWindow].stCapWin.height);

    // FIXME: direct use variable instead of stDBreg?

    MDrv_XC_DWIN_set_fetch_number_limit(pInstance,&g_DIPSrcInfo[eWindow], eWindow );

    MDrv_XC_DWIN_set_capture_window(pInstance,eWindow );

    MDrv_XC_DWIN_sw_db(pInstance,&stDIP_DBreg[eWindow], eWindow );

    XC_LOG_TRACE(XC_DBGLEVEL_DWIN_SETWINDOW,"============= SetWindow Done (Window : %u, src: %u) =============\n",eWindow, g_DIPSrcInfo[eWindow].enInputSourceType);


}
void MDrv_XC_DIP_SetFRC(void* pInstance,MS_BOOL bEnable,MS_U16 u16In,MS_U16 u16Out,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetFRC(pInstance, bEnable,u16In,u16Out,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_EnableCaptureStream(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    if(bEnable)
    {
        HAL_XC_DIP_SWReset(pInstance, eWindow);//Reset HW before DIP enable to avoid garbage
        HAL_XC_DIP_EnableCaptureStream(pInstance, bEnable,eWindow);
    }
    else
    {
      	MS_U16 u16IntrStus = 0;
        HAL_XC_DIP_EnableCaptureStream(pInstance, bEnable,eWindow);
		//Auto clear status to zero
		u16IntrStus = HAL_XC_DIP_GetIntrStatus(pInstance, eWindow);
		HAL_XC_DIP_ClearIntr(pInstance, u16IntrStus,eWindow);
        HAL_XC_DIP_SWReset(pInstance, eWindow);//Reset after disable dip to avoid get old temproary info
    }
    _XC_RETURN(pInstance);
}
#define XC_DIP_CAP_TIMEOUT_CNT 100
void MDrv_XC_DIP_CaptureOneFrame(void* pInstance,SCALER_DIP_WIN eWindow)
{
	MS_U16 regval = 0;
    MS_U16 u16Count = 0;
	//_XC_ENTRY(pInstance);
    HAL_XC_DIP_SWReset(pInstance, eWindow);//Reset HW before DIP enable to avoid garbage
    HAL_XC_DIP_CpatureOneFrame2(pInstance, eWindow);
	//_XC_RETURN(pInstance);
	do{
        regval = HAL_XC_DIP_GetIntrStatus(pInstance, eWindow);
        if(regval > 0x0)
            break;
        u16Count++;
        MsOS_DelayTask(1);
    }while(u16Count < XC_DIP_CAP_TIMEOUT_CNT);
}
void MDrv_XC_DIP_CaptureOneFrame2(void* pInstance,SCALER_DIP_WIN eWindow)
{
    //_XC_ENTRY(pInstance);
    HAL_XC_DIP_SWReset(pInstance, eWindow);//Reset HW before DIP enable to avoid garbage
    HAL_XC_DIP_CpatureOneFrame2(pInstance, eWindow);
    //_XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetWinProperty(void* pInstance,ST_XC_DIP_WINPROPERTY *pstDIPWinProperty, SCALER_DIP_WIN eWindow)

{
    MS_U8  i=0,u8MIUSel=0,u8IsIntlacW=0, u8BufCnt=0;
    MS_U16 u16Width=0,u16LineOft=0, u16Height=0, u16Bpp=0,u16NewWidth=0,u16NewLineOft=0,u16NewHeight=0;
    MS_PHY u64BufEnd=0, u64BufStart=0, u64TotalLen=0,u64YLen=0,u64CLen=0,u64Offset=0;
    MS_PHY u64_result = 0;
    EN_DRV_XC_DWIN_DATA_FMT fmt=XC_DWIN_DATA_FMT_MAX;
    EN_XC_DWIN_SCAN_TYPE enScan=GOPDWIN_SCAN_MODE_MAX;
    SCALER_DIP_SOURCE_TYPE enSource=MAX_SCALER_DIP_SOURCE_NUM;
    MS_BOOL bPIP = FALSE;
    MS_BOOL b2P_Enable =FALSE;
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    if(pstDIPWinProperty != NULL)
    {
        g_DIPSrcInfo[eWindow].enDIPSourceType = pstDIPWinProperty->enSource;
    }

    //check bucnt
    if(  (pstDIPWinProperty == NULL)
       ||(pstDIPWinProperty->u8BufCnt > MAX_BUFF_CNT)
       ||(pstDIPWinProperty->u32BufEnd == pstDIPWinProperty->u32BufStart)
       ||(pstDIPWinProperty->u16Width == 0)
       ||(pstDIPWinProperty->u16Height== 0))
    {
        DIP_D_ERR("[%s:%d] Clear Buffer info\n",__FUNCTION__,__LINE__);
        memset(&g_DIPSrcInfo[eWindow].stBufInfo, 0, sizeof(XC_DIP_BUFFER_INFO));
        return;
    }

    //width alignment check
    fmt = HAL_XC_DIP_GetDataFmt(pInstance, eWindow);
    if(fmt == XC_DWIN_DATA_FMT_MAX)
    {
        return;
    }

    u8BufCnt   = pstDIPWinProperty->u8BufCnt;
    u16Width   = pstDIPWinProperty->u16Width;
    u16Height  = pstDIPWinProperty->u16Height;
    u64BufStart= pstDIPWinProperty->u32BufStart;
    u64BufEnd  = pstDIPWinProperty->u32BufEnd;
    enSource    = pstDIPWinProperty->enSource;

	u16Bpp = HAL_XC_DIP_GetBPP(pInstance, fmt,eWindow);
	if( pstDIPWinProperty->u16Pitch == 0 )
	{
		u16LineOft = u16Width;
	}
	else
	{
            //if( (fmt == XC_DWIN_DATA_FMT_YUV420) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265)|| (fmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS)|| (fmt == XC_DWIN_DATA_FMT_YUV420_PLANER)|| (fmt == XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER))
            //{
            //    u64_result = (MS_PHY)(pstDIPWinProperty->u16Pitch*2);
            //    do_div(u64_result,3);
            //    u16LineOft = u64_result;
            //}
            //else
            {
                u64_result = (MS_PHY)(pstDIPWinProperty->u16Pitch);
                do_div(u64_result,u16Bpp);
                u16LineOft = u64_result;
            }

		if(u16LineOft < u16Width)
		{
			DIP_D_ERR("[%s:%d] Pitch is not correct\n",__FUNCTION__,__LINE__);
        	return;
		}
	}

	if( (fmt == XC_DWIN_DATA_FMT_YUV420) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS))
	{
		u16NewWidth = ALIGN_CHECK(u16Width,DIP_HVD_TILE_BLOCK_H);
		u16NewLineOft = ALIGN_CHECK(u16LineOft,DIP_HVD_TILE_BLOCK_H);
	}
	else
	{
	    u64_result = HAL_XC_DIP_GetBusSize(pInstance, eWindow);
        do_div(u64_result,u16Bpp);
	    u16NewWidth = ALIGN_CHECK(u16Width,u64_result);
		u16NewLineOft = ALIGN_CHECK(u16LineOft,u64_result);
	}
    //if is interlace mode
    enScan = HAL_XC_DIP_GetSourceScanType(pInstance, eWindow);
    if(enScan == GOPDWIN_SCAN_MODE_MAX)
        return;
    if(enScan == GOPDWIN_SCAN_MODE_INTERLACE)
    {
        u64_result = u16Height;
        do_div(u64_result,2);
        u16NewHeight = u64_result;
    }
    else
        u16NewHeight = u16Height;

    //buffer size check
    // offset unit : pixel
    if( (fmt == XC_DWIN_DATA_FMT_YUV420) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS) || (fmt == XC_DWIN_DATA_FMT_YUV420_PLANER) || (fmt == XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER))
    {
        if( (fmt == XC_DWIN_DATA_FMT_YUV420) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS))
        {
    	    //Tile Y block is 16X32
    	    u64YLen = u16NewLineOft*ALIGN_CHECK(u16NewHeight,DIP_HVD_TILE_BLOCK_V);
		    //Tile C block is 16X64
		    u64_result = u16NewLineOft*ALIGN_CHECK(u16NewHeight,DIP_HVD_TILE_BLOCK_V*2);
            do_div(u64_result,2);
            u64CLen = u64_result;
        }
        else
        {
            u64YLen = (MS_PHY)u16NewLineOft*u16NewHeight;
            u64_result = (MS_PHY)u16NewLineOft*u16NewHeight;
            do_div(u64_result,2);
            u64CLen = u64_result;
        }
		u64Offset = u64YLen+u64CLen;
		if(u8BufCnt == AUTO_BUFF_CNT)
		{
            u64_result = u64BufEnd - u64BufStart;
            do_div(u64_result,(u64Offset*u16Bpp));
            //u8BufCnt = (u64BufEnd - u64BufStart)/(u64Offset*u16Bpp);
            u8BufCnt = u64_result;
			u64TotalLen = u8BufCnt*u64Offset*u16Bpp;
		}
		else
		{
        	u64TotalLen = u8BufCnt*u64Offset*u16Bpp;
		}
		for(i=0;i<u8BufCnt;i++)
        {
        	g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_YBuf[i] = u64BufStart+u64Offset*i;
			g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_CBuf[i] = u64BufStart+u64YLen+u64Offset*i;
        }
    }
    else
    {
    	u64Offset = (MS_PHY)u16NewLineOft*u16NewHeight;
		if(u8BufCnt == AUTO_BUFF_CNT)
		{
            u64_result = u64BufEnd - u64BufStart;
            do_div(u64_result,(u64Offset*u16Bpp));
            //u8BufCnt = (u64BufEnd - u64BufStart)/(u64Offset*u16Bpp);
            u8BufCnt = u64_result;
			u64TotalLen = u8BufCnt*u64Offset*u16Bpp;
		}
		else
		{
			u64TotalLen = u8BufCnt*u64Offset*u16Bpp;
		}
		for(i=0;i<u8BufCnt;i++)
        {
            g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_YBuf[i] = u64BufStart+u64Offset*i*u16Bpp;
            if( (fmt == XC_DWIN_DATA_FMT_YC422) )
            	g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_CBuf[i] = u64BufStart+u64Offset*(i*u16Bpp+1);
        }
    }
	g_DIPSrcInfo[eWindow].stBufInfo.u8DIP_BufCnt = u8BufCnt;
    //if is deinterlace mode
    u8IsIntlacW = HAL_XC_DIP_GetInterlaceWrite(pInstance, eWindow);
    if(u8IsIntlacW)
   	{
        u64TotalLen *=2;
        u64Offset *=2;
 	}
    if( u64TotalLen > (u64BufEnd - u64BufStart))
    {
    	u64BufEnd = u64TotalLen + u64BufStart;
#if defined (__aarch64__)
        DIP_D_DBUG("[%s:%d] Buffer size is not correct,need = %lx\n",__FUNCTION__,__LINE__,u64TotalLen);
#else
        DIP_D_DBUG("[%s:%d] Buffer size is not correct,need = %tx\n",__FUNCTION__,__LINE__,(ptrdiff_t)u64TotalLen);
#endif
    }

    _phy_to_miu_offset(u8MIUSel, u64BufStart, u64BufStart);
    _phy_to_miu_offset(u8MIUSel, u64BufEnd, u64BufEnd);

    b2P_Enable = MApi_XC_GetDynamicScalingStatus();
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetMiuSel(pInstance, u8MIUSel,eWindow);
    HAL_XC_DIP_SetBase0(pInstance, u64BufStart,u64BufEnd,eWindow);

    //check if is Sub PIP or POP
    if(enSource == SCALER_DIP_SOURCE_TYPE_OP_SUB)
    {
        MS_WINDOW_TYPE stDisp[MAX_WINDOW];

        MDrv_XC_get_dispwin_from_reg(pInstance, MAIN_WINDOW, &stDisp[MAIN_WINDOW]);
        MDrv_XC_get_dispwin_from_reg(pInstance, SUB_WINDOW, &stDisp[SUB_WINDOW]);

        if ( ((stDisp[MAIN_WINDOW].x + stDisp[MAIN_WINDOW].width) >= (stDisp[SUB_WINDOW].x + stDisp[SUB_WINDOW].width)) && (stDisp[MAIN_WINDOW].x  <= stDisp[SUB_WINDOW].x) )
        {
            bPIP = TRUE;
        }

    }

    //set clock and mux
    HAL_XC_DIP_MuxDispatch(pInstance, enSource,eWindow);

    //set win property
    HAL_XC_DIP_SetWinProperty(pInstance, u8BufCnt,u16NewWidth,u16NewLineOft,u16NewHeight,u64Offset,enSource,bPIP,b2P_Enable, eWindow);
    if( (fmt == XC_DWIN_DATA_FMT_YUV420) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS) || (fmt == XC_DWIN_DATA_FMT_YUV420_PLANER) || (fmt == XC_DWIN_DATA_FMT_YUV420_SEMI_PLANER) )
    {
        HAL_XC_DIP_SetBase1(pInstance, u64BufStart+u64YLen,u64BufEnd,eWindow);
        HAL_XC_DIP_SetWinProperty1(pInstance, u64Offset,eWindow);
    }
    else if( (fmt == XC_DWIN_DATA_FMT_YC422) )
    {
        HAL_XC_DIP_SetBase1(pInstance, u64BufStart+u64Offset,u64BufEnd,eWindow);
        HAL_XC_DIP_SetWinProperty1(pInstance, u64Offset,eWindow);
    }

    _XC_RETURN(pInstance);

}
BUFFER_INFO MDrv_XC_DIP_GetBufferInfo(void* pInstance,SCALER_DIP_WIN eWindow)
{
	MS_U8 i;
	BUFFER_INFO BufInfo;
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
	BufInfo.u8BufCnt = g_DIPSrcInfo[eWindow].stBufInfo.u8DIP_BufCnt;
	for(i =0;i<BufInfo.u8BufCnt;i++)
	{
		BufInfo.u32YBuf[i] = g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_YBuf[i];
		BufInfo.u32CBuf[i] = g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_CBuf[i];
	}
    return BufInfo;
}
MS_U16 MDrv_XC_DIP_GetIntrStatus(void* pInstance,SCALER_DIP_WIN eWindow)
{
    MS_U16 regval =0;
    regval = HAL_XC_DIP_GetIntrStatus(pInstance, eWindow);
    return regval;
}
void MDrv_XC_DIP_SelectSourceScanType(void* pInstance,EN_XC_DWIN_SCAN_TYPE enScan,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SelectSourceScanType(pInstance, enScan,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetInterlaceWrite(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetInterlaceWrite(pInstance, bEnable,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetY2R(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetY2R(pInstance, bEnable,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetDataFmt(void* pInstance,EN_DRV_XC_DWIN_DATA_FMT fmt,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetDataFmt(pInstance, fmt,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetAlphaValue(void* pInstance,MS_U8 u8AlphaVal,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetAlphaValue(pInstance, u8AlphaVal,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetUVSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetUVSwap(pInstance, bEnable,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetYCSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetYCSwap(pInstance, bEnable,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetRGBSwap(void* pInstance,MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetRGBSwap(pInstance, bEnable,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_EnableIntr(void* pInstance,MS_U16 u8mask, MS_BOOL bEnable,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_EnableIntr(pInstance, u8mask, bEnable,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_ClearIntr(void* pInstance,MS_U16 u16mask,SCALER_DIP_WIN eWindow)
{
    HAL_XC_DIP_ClearIntr(pInstance, u16mask,eWindow);
}
void MDrv_XC_DIP_SetOutputCapture(void* pInstance,MS_BOOL bEnable,EN_XC_DIP_OP_CAPTURE eOpCapture,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetOutputCapture(pInstance, bEnable,eOpCapture,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetMirror(void* pInstance,MS_BOOL bHMirror,MS_BOOL bVMirror,SCALER_DIP_WIN eWindow)
{
    _XC_ENTRY(pInstance);
    HAL_XC_DIP_SetMirror(pInstance, bHMirror,bVMirror,eWindow);
    _XC_RETURN(pInstance);
}
void MDrv_XC_DIP_SetDIPRProperty(void* pInstance,ST_XC_DIPR_PROPERTY *pstDIPRProperty, SCALER_DIP_WIN eWindow)
{
    MS_U8 u8MIUSel = 0,u8MIUSelTMP = 0;
    MS_U16 u16Bpp = 0,u16Width = 0,u16LineOft = 0;
    EN_DRV_XC_DWIN_DATA_FMT fmt = 0;
    MS_U64 u64_result = 0;

    _XC_ENTRY(pInstance);
    //Transfer pitch to lineoffset
    u16Bpp = HAL_XC_DIP_GetBPP(pInstance, (EN_DRV_XC_DWIN_DATA_FMT)(pstDIPRProperty->enDataFmt),eWindow);
    if( pstDIPRProperty->u16Pitch == 0 )
    {
        u16LineOft = u16Width;
    }
    else
    {
        fmt = (EN_DRV_XC_DWIN_DATA_FMT)(pstDIPRProperty->enDataFmt);
        //if( (fmt == XC_DWIN_DATA_FMT_YUV420) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265) || (fmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS))
        //{
        //    u64_result = (MS_PHY)(pstDIPRProperty->u16Pitch*2);
        //    do_div(u64_result,3);
        //    u16LineOft = u64_result;
        //}
        //else
        {
            u64_result = (MS_PHY)(pstDIPRProperty->u16Pitch);
            do_div(u64_result,u16Bpp);
            u16LineOft = u64_result;
        }

        if(u16LineOft < u16Width)
        {
            DIP_D_ERR("[%s:%d] Pitch is not correct\n",__FUNCTION__,__LINE__);
            return;
        }
    }
    pstDIPRProperty->u16Pitch = u16LineOft;

    //DIPr = DIPw MIU selection, so change DIPW MIU selection
    _phy_to_miu_offset(u8MIUSel, pstDIPRProperty->u32YBufAddr, pstDIPRProperty->u32YBufAddr);
    _phy_to_miu_offset(u8MIUSelTMP, pstDIPRProperty->u32CBufAddr, pstDIPRProperty->u32CBufAddr);
    _phy_to_miu_offset(u8MIUSelTMP, pstDIPRProperty->u32YBufAddr10Bits, pstDIPRProperty->u32YBufAddr10Bits);
    _phy_to_miu_offset(u8MIUSelTMP, pstDIPRProperty->u32CBufAddr10Bits, pstDIPRProperty->u32CBufAddr10Bits);
    HAL_XC_DIP_SetDIPRMiuSel(pInstance, u8MIUSel,eWindow);
    HAL_XC_DIP_SetDIPRProperty(pInstance, pstDIPRProperty,eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DIP_SetDIPRProperty_EX(void* pInstance,ST_XC_DIPR_PROPERTY_EX *pstDIPRProperty_ex, SCALER_DIP_WIN eWindow)
{
    MS_U8 u8MIUSel = 0,u8MIUSelTMP = 0;
    MS_U16 u16Bpp = 0,u16Width = 0,u16LineOft = 0;
    EN_DRV_XC_DWIN_DATA_FMT enfmt = 0;
    MS_U64 u64_result = 0;

    _XC_ENTRY(pInstance);
    //Transfer pitch to lineoffset
    u16Bpp = HAL_XC_DIP_GetBPP(pInstance, (EN_DRV_XC_DWIN_DATA_FMT)(pstDIPRProperty_ex->stDIPRProperty.enDataFmt),eWindow);
    if( pstDIPRProperty_ex->stDIPRProperty.u16Pitch == 0 )
    {
        u16LineOft = u16Width;
    }
    else
    {
        enfmt = (EN_DRV_XC_DWIN_DATA_FMT)(pstDIPRProperty_ex->stDIPRProperty.enDataFmt);
        //if( (enfmt == XC_DWIN_DATA_FMT_YUV420) || (enfmt == XC_DWIN_DATA_FMT_YUV420_H265) || (enfmt == XC_DWIN_DATA_FMT_YUV420_H265_10BITS))
        //{
        //    u64_result = (pstDIPRProperty_ex->stDIPRProperty.u16Pitch*2);
        //    do_div(u64_result,3);
        //    u16LineOft = u64_result;
        //}
        //else
        {
            u64_result = (pstDIPRProperty_ex->stDIPRProperty.u16Pitch);
            do_div(u64_result,u16Bpp);
            u16LineOft = u64_result;
        }

        if(u16LineOft < u16Width)
        {
            DIP_D_ERR("[%s:%d] Pitch is not correct\n",__FUNCTION__,__LINE__);
            return;
        }
    }
    pstDIPRProperty_ex->stDIPRProperty.u16Pitch = u16LineOft;

    //DIPr = DIPw MIU selection, so change DIPW MIU selection
    _phy_to_miu_offset(u8MIUSel, pstDIPRProperty_ex->stDIPRProperty.u32YBufAddr, pstDIPRProperty_ex->stDIPRProperty.u32YBufAddr);
    _phy_to_miu_offset(u8MIUSelTMP, pstDIPRProperty_ex->stDIPRProperty.u32CBufAddr, pstDIPRProperty_ex->stDIPRProperty.u32CBufAddr);
    _phy_to_miu_offset(u8MIUSelTMP, pstDIPRProperty_ex->stDIPRProperty.u32YBufAddr10Bits, pstDIPRProperty_ex->stDIPRProperty.u32YBufAddr10Bits);
    _phy_to_miu_offset(u8MIUSelTMP, pstDIPRProperty_ex->stDIPRProperty.u32CBufAddr10Bits, pstDIPRProperty_ex->stDIPRProperty.u32CBufAddr10Bits);
    HAL_XC_DIP_SetDIPRMiuSel(pInstance, u8MIUSel,eWindow);
    HAL_XC_DIP_SetDIPRProperty(pInstance, &pstDIPRProperty_ex->stDIPRProperty,eWindow);
    HAL_XC_DIP_SetDIPRProperty_MFDEC(pInstance, pstDIPRProperty_ex->stDIPR_MFDecInfo,eWindow);
    HAL_XC_DIP_SetDIPRProperty_DI(pInstance, &pstDIPRProperty_ex->stDIPRProperty, &pstDIPRProperty_ex->stDIPR3DDISetting,eWindow);
    _XC_RETURN(pInstance);
}

void MDrv_XC_DIP_SetRotation(void* pInstance,MS_BOOL bRotation,EN_XC_DIP_ROTATION eRoDirection,SCALER_DIP_WIN eTmpWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
	_XC_ENTRY(pInstance);
	HAL_XC_DIP_Rotation(pInstance, bRotation,eRoDirection,g_DIPSrcInfo[eTmpWindow].stBufInfo.u64DIP_YBuf[0],eTmpWindow);
	_XC_RETURN(pInstance);
}

MS_BOOL MDrv_XC_DIP_SetHVSP(void* pInstance,MS_BOOL bSelect, SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    MS_BOOL bResource = FALSE;

	_XC_ENTRY(pInstance);
	bResource = HAL_XC_DIP_SetHVSP(pInstance,bSelect,eWindow);
	_XC_RETURN(pInstance);

    return bResource;
}

MS_BOOL MDrv_XC_DIP_Set420TileBlock(void* pInstance,EN_XC_DIP_TILE_BLOCK eTileBlock,SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
	UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    MS_BOOL bSupport = FALSE;

	_XC_ENTRY(pInstance);
	bSupport = HAL_XC_DIP_Set420TileBlock(pInstance,eTileBlock,eWindow);
	_XC_RETURN(pInstance);

    return bSupport;
}

void MDrv_XC_DIP_SetPinpon(void* pInstance,MS_BOOL bPinpon,MS_PHY u64PinponAddr,SCALER_DIP_WIN eWindow)
{
#ifdef UTOPIA_V2
    DIP_INSTANCE_PRIVATE* pDipPri = NULL;
    DIP_SHARE_RESOURCE_PRIVATE* pDipResPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void*)&pDipPri);
    UtopiaResourceGetPrivate(pDipPri->pShareResource,(void*)&(pDipResPri));
#endif
    MS_PHY u64_result;
	_XC_ENTRY(pInstance);
	if(bPinpon == TRUE)
	{
		MS_U16 u16Bpp = 0;
		EN_DRV_XC_DWIN_DATA_FMT fmt;
		fmt = HAL_XC_DIP_GetDataFmt(pInstance, eWindow);
		u16Bpp = HAL_XC_DIP_GetBPP(pInstance, fmt,eWindow);
        u64_result = u64PinponAddr - g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_YBuf[0];
        do_div(u64_result,u16Bpp);
        //HAL_XC_DIP_SetPinpon(pInstance, bPinpon,u64PinponAddr,(u64PinponAddr - g_DIPSrcInfo[eWindow].stBufInfo.u64DIP_YBuf[0])/u16Bpp,eWindow);
        HAL_XC_DIP_SetPinpon(pInstance, bPinpon,u64PinponAddr,u64_result,eWindow);
	}
	else
	{
		HAL_XC_DIP_SWReset(pInstance, eWindow);
	}
	_XC_RETURN(pInstance);
}

void MDrv_XC_DIP_InterruptAttach(void* pInstance,InterruptCb pIntCb,SCALER_DIP_WIN eWindow)
{
	_XC_ENTRY(pInstance);
	HAL_XC_DIP_InterruptAttach(pInstance, pIntCb,eWindow);
	_XC_RETURN(pInstance);
}
void MDrv_XC_DIP_InterruptDetach(void* pInstance,SCALER_DIP_WIN eWindow)
{
	_XC_ENTRY(pInstance);
	HAL_XC_DIP_InterruptDetach(pInstance, eWindow);
	_XC_RETURN(pInstance);
}

void MDrv_XC_DIP_SetBdgLevel(EN_XC_DIP_DEBUG_LEVEL level)
{
    u32DIPDbgLevel_drv = level;
}

#ifdef CONFIG_UTOPIA_PROC_DBG_SUPPORT
static void MDrv_XC_DIP_Read_Info(MS_U64* u64ReqHdl,SCALER_DIP_WIN eWindow)
{
    MS_U16 u16DIPInit=0;
    MS_U16 u16DIPREnable=0;

    MS_PHY phyOutYAdress=0;
    MS_PHY phyOutCAdress=0;
    MS_U8 u8OutputMiu=0;
    MS_U16 u16OutputWidth=0;
    MS_U16 u16OutputHeight=0;
    MS_U16 u16OutputPitch=0;

    char string_OutputFormat[10];
    char string_OutputTileFormat[8];

    MS_U16 u16Clip_H_Start=0;
    MS_U16 u16Clip_H_End=0;
    MS_U16 u16Clip_V_Start=0;
    MS_U16 u16Clip_V_End=0;
    MS_U16 u16HMirror=0;
    MS_U16 u16VMirror=0;

    MS_U16 u16H_Scaling_Enable=0;
    MS_U32 u32H_Scaling_Ratio=0;
    MS_U16 u16V_Scaling_Enable=0;
    MS_U32 u32V_Scaling_Ratio=0;

    MS_PHY phyInputYAdress=0;
    MS_PHY phyInputCAdress=0;
    MS_U8 u8InputMiu=0;

    MS_U16 u16InputWidth=0;
    MS_U16 u16InputHeight=0;
    MS_U16 u16InputPitch=0;

    char string_InputFormat[10];
    char string_InputTileFormat[8];

    MS_U16 u16tmp=0;
    MS_U16 u16tmp2=0;
    MS_U16 u16BusSize = 0;

    char DIP_format_YUV422[] = "YUV422";
    char DIP_format_RGB565[] = "RGB565";
    char DIP_format_ARGB8888[] = "ARGB8888";
    char DIP_format_YUV420[] = "YUV420";

    char DIP_tile_format_Linear[] = "Linear";
    char DIP_tile_format_16x32[] = "16x32";
    char DIP_tile_format_32x16[] = "32x16";
    char DIP_tile_format_32x32[] = "32x32";

    if( eWindow == DIP_WINDOW )
    {
        //Check DIP initialized or not
        u16DIPInit = SC_R2BYTEMSK(0, REG_SC_BK36_01_L, BIT(6));
        if(u16DIPInit == 0 )
        {
            MdbPrint(u64ReqHdl,"DIP%d Not Initialized\n",eWindow);
            return;
        }
        u16BusSize = HAL_XC_DIP_GetBusSize(NULL, eWindow);

        //DIP capture from
        u16DIPREnable = SC_R2BYTEMSK(0, REG_SC_BK34_7F_L, BIT(6));

        //DIPW address/miu
        phyOutYAdress =  SC_R4BYTE(0, REG_SC_BK36_10_L) * u16BusSize;
        phyOutCAdress =  SC_R4BYTE(0, REG_SC_BK36_20_L) * u16BusSize;
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_02_L, BIT(13)) >> 13;
        u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_07_L, BIT(15)) >> 15;
        u8OutputMiu= u16tmp | (u16tmp2<<1);

        //DIPW width/height/pitch
        u16OutputWidth = SC_R2BYTE(0, REG_SC_BK36_1F_L);
        u16OutputHeight = SC_R2BYTE(0, REG_SC_BK36_2F_L);
        u16OutputPitch = SC_R2BYTE(0, REG_SC_BK36_2F_L);

        //DIPW format/tile format
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_01_L, BMASK(5:4)) >> 4;
        if(u16tmp == 0)
        {
            strcpy(string_OutputFormat, DIP_format_YUV422);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 1)
        {
            strcpy(string_OutputFormat, DIP_format_RGB565);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 2)
        {
            strcpy(string_OutputFormat, DIP_format_ARGB8888);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else
        {
            strcpy(string_OutputFormat, DIP_format_YUV420);
            u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7E_L, BIT(14));
            if(u16tmp != 0)
            {
                strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
            }
            else
            {
                u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(12));
                u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(14));
                if((u16tmp==0)&&(u16tmp2==0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_16x32);
                }
                else if((u16tmp!=0)&&(u16tmp2==0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_32x16);
                }
                else if((u16tmp==0)&&(u16tmp2!=0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_32x32);
                }
                else
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
                }
            }
        }
        //clip window info
        u16Clip_H_Start = SC_R2BYTE(0, REG_SC_BK34_61_L);
        u16Clip_H_End = SC_R2BYTE(0, REG_SC_BK34_62_L);
        u16Clip_V_Start = SC_R2BYTE(0, REG_SC_BK34_63_L);
        u16Clip_V_End = SC_R2BYTE(0, REG_SC_BK34_63_L);

        //HV Mirror status
        u16HMirror = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(9)) >> 9;
        u16VMirror = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(10)) >> 10;

        //HV scaling enable/ratio
        HAL_XC_DIP_Check_Scale(u64ReqHdl, &u16H_Scaling_Enable,&u32H_Scaling_Ratio,&u16V_Scaling_Enable,&u32V_Scaling_Ratio,eWindow);

        //DIPR address/miu
        phyInputYAdress = SC_R4BYTE(0, REG_SC_BK36_78_L) * u16BusSize;
        phyInputCAdress = SC_R4BYTE(0, REG_SC_BK36_78_L) * u16BusSize;
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15)) >> 15;
        u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14)) >> 14;
        u8InputMiu= u16tmp | (u16tmp2<<1);

        //DIPR width/height/pitch
        u16InputWidth = SC_R2BYTE(0, REG_SC_BK36_27_L);
        u16InputHeight = SC_R2BYTE(0, REG_SC_BK36_37_L);
        u16InputPitch = SC_R2BYTE(0, REG_SC_BK36_74_L);

        //DIPR format/tile format
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7D_L, BMASK(7:6)) >> 6;
        if(u16tmp == 0)
        {
            strcpy(string_InputFormat, DIP_format_YUV422);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 1)
        {
            strcpy(string_InputFormat, DIP_format_RGB565);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 2)
        {
            strcpy(string_InputFormat, DIP_format_ARGB8888);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else
        {
            strcpy(string_InputFormat, DIP_format_YUV420);
            u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7E_L, BIT(14));
            if(u16tmp != 0)
            {
                strcpy(string_InputTileFormat, DIP_tile_format_Linear);
            }
            else
            {
                u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(13));
                u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(15));
                if((u16tmp==0)&&(u16tmp2==0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_16x32);
                }
                else if((u16tmp!=0)&&(u16tmp2==0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_32x16);
                }
                else if((u16tmp==0)&&(u16tmp2!=0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_32x32);
                }
                else
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_Linear);
                }
            }
        }
    }
    else if( eWindow == DWIN0_WINDOW )
    {
        //Check DIP initialized or not
        u16DIPInit = SC_R2BYTEMSK(0, REG_SC_BK3B_01_L, BIT(6));
        if(u16DIPInit == 0 )
        {
            MdbPrint(u64ReqHdl,"DIP%d Not Initialized\n",eWindow);
            return;
        }
        u16BusSize = HAL_XC_DIP_GetBusSize(NULL, eWindow);

        //DIP capture from
        u16DIPREnable = SC_R2BYTEMSK(0, REG_SC_BK3B_73_L, BIT(6));

        //DIPW address/miu
        phyOutYAdress =  SC_R4BYTE(0, REG_SC_BK3B_10_L) * u16BusSize;
        phyOutCAdress =  SC_R4BYTE(0, REG_SC_BK3B_20_L) * u16BusSize;
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK3B_02_L, BIT(13)) >> 13;
        u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK3B_07_L, BIT(15)) >> 15;
        u8OutputMiu= u16tmp | (u16tmp2<<1);

        //DIPW width/height/pitch
        u16OutputWidth = SC_R2BYTE(0, REG_SC_BK3B_1F_L);
        u16OutputHeight = SC_R2BYTE(0, REG_SC_BK3B_2F_L);
        u16OutputPitch = SC_R2BYTE(0, REG_SC_BK3B_2F_L);

        //DIPW format/tile format
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK3B_01_L, BMASK(5:4)) >> 4;
        if(u16tmp == 0)
        {
            strcpy(string_OutputFormat, DIP_format_YUV422);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 1)
        {
            strcpy(string_OutputFormat, DIP_format_RGB565);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 2)
        {
            strcpy(string_OutputFormat, DIP_format_ARGB8888);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else
        {
            strcpy(string_OutputFormat, DIP_format_YUV420);
            u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7E_L, BIT(14));
            if(u16tmp != 0)
            {
                strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
            }
            else
            {
                u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(12));
                u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(14));
                if((u16tmp==0)&&(u16tmp2==0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_16x32);
                }
                else if((u16tmp!=0)&&(u16tmp2==0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_32x16);
                }
                else if((u16tmp==0)&&(u16tmp2!=0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_32x32);
                }
                else
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
                }
            }
        }
        //clip window info
        u16Clip_H_Start = SC_R2BYTE(0, REG_SC_BK3B_63_L);
        u16Clip_H_End = SC_R2BYTE(0, REG_SC_BK3B_64_L);
        u16Clip_V_Start = SC_R2BYTE(0, REG_SC_BK3B_65_L);
        u16Clip_V_End = SC_R2BYTE(0, REG_SC_BK3B_66_L);

        //HV Mirror status
        u16HMirror = SC_R2BYTEMSK(0, REG_SC_BK3B_0B_L, BIT(9)) >> 9;
        u16VMirror = SC_R2BYTEMSK(0, REG_SC_BK3B_0B_L, BIT(10)) >> 10;

        //HV scaling enable/ratio
        HAL_XC_DIP_Check_Scale(u64ReqHdl, &u16H_Scaling_Enable,&u32H_Scaling_Ratio,&u16V_Scaling_Enable,&u32V_Scaling_Ratio,eWindow);

        //DIPR address/miu
        phyInputYAdress = SC_R4BYTE(0, REG_SC_BK36_78_L) * u16BusSize;
        phyInputCAdress = SC_R4BYTE(0, REG_SC_BK36_78_L) * u16BusSize;
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15)) >> 15;
        u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14)) >> 14;
        u8InputMiu= u16tmp | (u16tmp2<<1);

        //DIPR width/height/pitch
        u16InputWidth = SC_R2BYTE(0, REG_SC_BK36_27_L);
        u16InputHeight = SC_R2BYTE(0, REG_SC_BK36_37_L);
        u16InputPitch = SC_R2BYTE(0, REG_SC_BK36_74_L);

        //DIPR format/tile format
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7D_L, BMASK(7:6)) >> 6;
        if(u16tmp == 0)
        {
            strcpy(string_InputFormat, DIP_format_YUV422);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 1)
        {
            strcpy(string_InputFormat, DIP_format_RGB565);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 2)
        {
            strcpy(string_InputFormat, DIP_format_ARGB8888);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else
        {
            strcpy(string_InputFormat, DIP_format_YUV420);
            u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7E_L, BIT(14));
            if(u16tmp != 0)
            {
                strcpy(string_InputTileFormat, DIP_tile_format_Linear);
            }
            else
            {
                u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(13));
                u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(15));
                if((u16tmp==0)&&(u16tmp2==0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_16x32);
                }
                else if((u16tmp!=0)&&(u16tmp2==0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_32x16);
                }
                else if((u16tmp==0)&&(u16tmp2!=0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_32x32);
                }
                else
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_Linear);
                }
            }
        }
    }
    else if( eWindow == DWIN1_WINDOW )
    {
        //Check DIP initialized or not
        u16DIPInit = SC_R2BYTEMSK(0, REG_SC_BK3C_01_L, BIT(6));
        if(u16DIPInit == 0 )
        {
            MdbPrint(u64ReqHdl,"DIP%d Not Initialized\n",eWindow);
            return;
        }
        u16BusSize = HAL_XC_DIP_GetBusSize(NULL, eWindow);

        //DIP capture from
        u16DIPREnable = SC_R2BYTEMSK(0, REG_SC_BK3C_73_L, BIT(6));

        //DIPW address/miu
        phyOutYAdress =  SC_R4BYTE(0, REG_SC_BK3C_10_L) * u16BusSize;
        phyOutCAdress =  SC_R4BYTE(0, REG_SC_BK3C_20_L) * u16BusSize;
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK3C_02_L, BIT(13)) >> 13;
        u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK3C_07_L, BIT(15)) >> 15;
        u8OutputMiu= u16tmp | (u16tmp2<<1);

        //DIPW width/height/pitch
        u16OutputWidth = SC_R2BYTE(0, REG_SC_BK3C_1F_L);
        u16OutputHeight = SC_R2BYTE(0, REG_SC_BK3C_2F_L);
        u16OutputPitch = SC_R2BYTE(0, REG_SC_BK3C_2F_L);

        //DIPW format/tile format
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK3C_01_L, BMASK(5:4)) >> 4;
        if(u16tmp == 0)
        {
            strcpy(string_OutputFormat, DIP_format_YUV422);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 1)
        {
            strcpy(string_OutputFormat, DIP_format_RGB565);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 2)
        {
            strcpy(string_OutputFormat, DIP_format_ARGB8888);
            strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
        }
        else
        {
            strcpy(string_OutputFormat, DIP_format_YUV420);
            u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7E_L, BIT(14));
            if(u16tmp != 0)
            {
                strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
            }
            else
            {
                u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(12));
                u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(14));
                if((u16tmp==0)&&(u16tmp2==0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_16x32);
                }
                else if((u16tmp!=0)&&(u16tmp2==0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_32x16);
                }
                else if((u16tmp==0)&&(u16tmp2!=0))
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_32x32);
                }
                else
                {
                    strcpy(string_OutputTileFormat, DIP_tile_format_Linear);
                }
            }
        }
        //clip window info
        u16Clip_H_Start = SC_R2BYTE(0, REG_SC_BK3C_63_L);
        u16Clip_H_End = SC_R2BYTE(0, REG_SC_BK3C_64_L);
        u16Clip_V_Start = SC_R2BYTE(0, REG_SC_BK3C_65_L);
        u16Clip_V_End = SC_R2BYTE(0, REG_SC_BK3C_66_L);

        //HV Mirror status
        u16HMirror = SC_R2BYTEMSK(0, REG_SC_BK3C_0B_L, BIT(9)) >> 9;
        u16VMirror = SC_R2BYTEMSK(0, REG_SC_BK3C_0B_L, BIT(10)) >> 10;

        //HV scaling enable/ratio
        HAL_XC_DIP_Check_Scale(u64ReqHdl, &u16H_Scaling_Enable,&u32H_Scaling_Ratio,&u16V_Scaling_Enable,&u32V_Scaling_Ratio,eWindow);

        //DIPR address/miu
        phyInputYAdress = SC_R4BYTE(0, REG_SC_BK36_78_L) * u16BusSize;
        phyInputCAdress = SC_R4BYTE(0, REG_SC_BK36_78_L) * u16BusSize;
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7D_L, BIT(15)) >> 15;
        u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_7C_L, BIT(14)) >> 14;
        u8InputMiu= u16tmp | (u16tmp2<<1);

        //DIPR width/height/pitch
        u16InputWidth = SC_R2BYTE(0, REG_SC_BK36_27_L);
        u16InputHeight = SC_R2BYTE(0, REG_SC_BK36_37_L);
        u16InputPitch = SC_R2BYTE(0, REG_SC_BK36_74_L);

        //DIPR format/tile format
        u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7D_L, BMASK(7:6)) >> 6;
        if(u16tmp == 0)
        {
            strcpy(string_InputFormat, DIP_format_YUV422);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 1)
        {
            strcpy(string_InputFormat, DIP_format_RGB565);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else if(u16tmp == 2)
        {
            strcpy(string_InputFormat, DIP_format_ARGB8888);
            strcpy(string_InputTileFormat, DIP_tile_format_Linear);
        }
        else
        {
            strcpy(string_InputFormat, DIP_format_YUV420);
            u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_7E_L, BIT(14));
            if(u16tmp != 0)
            {
                strcpy(string_InputTileFormat, DIP_tile_format_Linear);
            }
            else
            {
                u16tmp = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(13));
                u16tmp2 = SC_R2BYTEMSK(0, REG_SC_BK36_0B_L, BIT(15));
                if((u16tmp==0)&&(u16tmp2==0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_16x32);
                }
                else if((u16tmp!=0)&&(u16tmp2==0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_32x16);
                }
                else if((u16tmp==0)&&(u16tmp2!=0))
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_32x32);
                }
                else
                {
                    strcpy(string_InputTileFormat, DIP_tile_format_Linear);
                }
            }
        }
    }
    else
    {
        MdbPrint(u64ReqHdl,"SCALER_DIP_WIN does not support\n");
        return;
    }

    MdbPrint(u64ReqHdl,"Capture from:%s\n",(u16DIPREnable?"DRAM":"XC"));
    MdbPrint(u64ReqHdl,"Output Y address=0x%x\n",phyOutYAdress);
    MdbPrint(u64ReqHdl,"Output C address=0x%x\n",phyOutCAdress);
    MdbPrint(u64ReqHdl,"Output Miu=%d\n",u8OutputMiu);
    MdbPrint(u64ReqHdl,"Output Width(pixel)=%d\n",u16OutputWidth);
    MdbPrint(u64ReqHdl,"Output Height(pixel)=%d\n",u16OutputHeight);
    MdbPrint(u64ReqHdl,"Output Pitch(pixel)=%d\n",u16OutputPitch);
    MdbPrint(u64ReqHdl,"Output Format:%s\n",string_OutputFormat);
    MdbPrint(u64ReqHdl,"Output Tile Format:%s\n",string_OutputFormat);
    MdbPrint(u64ReqHdl,"Clip Window H Start=%d\n",u16Clip_H_Start);
    MdbPrint(u64ReqHdl,"Clip Window H End=%d\n",u16Clip_H_End);
    MdbPrint(u64ReqHdl,"Clip Window V Start=%d\n",u16Clip_V_Start);
    MdbPrint(u64ReqHdl,"Clip Window V End=%d\n",u16Clip_V_End);
    MdbPrint(u64ReqHdl,"H Mirror=%s\n",(u16HMirror?"TRUE":"FALSE"));
    MdbPrint(u64ReqHdl,"H Mirror=%s\n",(u16VMirror?"TRUE":"FALSE"));
    MdbPrint(u64ReqHdl,"H Scaling Enable=%s\n",(u16H_Scaling_Enable?"TRUE":"FALSE"));
    MdbPrint(u64ReqHdl,"H Scaling Ratio=0x%x\n",u32H_Scaling_Ratio);
    MdbPrint(u64ReqHdl,"V Scaling Enable=%s\n",(u16V_Scaling_Enable?"TRUE":"FALSE"));
    MdbPrint(u64ReqHdl,"V Scaling Ratio=0x%x\n",u32V_Scaling_Ratio);
    if(u16DIPREnable != 0)
    {
        MdbPrint(u64ReqHdl,"Input Y Address=0x%x\n",phyInputYAdress);
        MdbPrint(u64ReqHdl,"Input C Address=0x%x\n",phyInputCAdress);
        MdbPrint(u64ReqHdl,"Input Miu=%d\n",u8InputMiu);
        MdbPrint(u64ReqHdl,"Input_Width(pixel)=%d\n",u16InputWidth);
        MdbPrint(u64ReqHdl,"Input_Height(pixel)=%d\n",u16InputHeight);
        MdbPrint(u64ReqHdl,"Input_Pitch(pixel)=%d\n",u16InputPitch);
        MdbPrint(u64ReqHdl,"Input_Format=%s\n",string_InputTileFormat);
        MdbPrint(u64ReqHdl,"Input_Tile_Format=%s\n",string_InputTileFormat);
    }
}


void MDrv_XC_DIP_Mdb_Cmdline(MS_U64* u64ReqHdl,MS_U32 u32CmdSize,char *pcCmdLine)
{

}

void MDrv_XC_DIP_Mdb_GetInfo(MS_U64* u64ReqHdl)
{
    MS_U8 u8dipIndex = 0;
    MS_U32 u32DipChipCaps = 0;
    MS_U8 u8dipTotal = 0;

    HAL_XC_DIP_Check_Clock(u64ReqHdl, DIP_WINDOW);

    for(u8dipIndex=0;u8dipIndex<MAX_DIP_WINDOW;u8dipIndex++)
    {
       DIP_CHIP_CAP(u8dipIndex,u32DipChipCaps);
       if((u32DipChipCaps&DIP_CAP_EXIST) != 0)
       {
           MdbPrint(u64ReqHdl,"--------- MStar DIP%d Info ---------\n",u8dipIndex);
           MDrv_XC_DIP_Read_Info(u64ReqHdl, (SCALER_DIP_WIN)u8dipIndex);
           u8dipTotal++;
       }
    }

    MdbPrint(u64ReqHdl,"Chip Support Total %d DIP\n",u8dipTotal);
}
#endif
MS_U32 MDrv_XC_DIP_ConfigPipe_U2(void* pInstance,MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode)
{
    if(u32OperationMode == XC_DIP_OPMODE_DISABLE)
    {
        HAL_XC_DIP_Enable(pInstance,TRUE);
    }
    else if(u32OperationMode == XC_DIP_OPMODE_ENABLE)
    {
        HAL_XC_DIP_Enable(pInstance,FALSE);
    }
    return 0;
}

#endif
//=============== DIP =====================//


