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
/// file    Mdrv_sc_display.c
/// @brief  Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <asm/div64.h>
#else
#define do_div(x,y) ((x)/=(y))
#endif

// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
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
#include "drv_sc_scaling.h"
#include "mdrv_sc_3d.h"
#include "mhal_ip.h"
#include "apiXC_DWIN.h"
#include "drv_sc_DIP_scaling.h"
#include "mhal_dip.h"
#include "halCHIP.h"
#include "mhal_menuload.h"
#include "drvXC_HDMI_Internal.h"
#include "drvSYS.h"
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> // O_RDWR
#endif
#include "mdrv_xc_io.h"
#include "mdrv_sc_dynamicscaling.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define VERIFY_DISPLAY_FPGA 0
#define PHASE_DIFF_LIMIT  0xFFFFUL
#ifdef UFO_XC_AUTO_DOWNLOAD
#define AUTO_DOWNLOAD_HDR_IN_KERNEL
#define AUTO_DOWNLOAD_XVYCC_IN_KERNEL
#endif

MS_U32 _u32ZorderMLoadCmd[MS_MLOAD_MAX_CMD_CNT];
MS_U16 _u16ZorderMLoadMsk[MS_MLOAD_MAX_CMD_CNT];
MS_U16 _u16ZorderMLoadVal[MS_MLOAD_MAX_CMD_CNT];
MS_U16 _u16ZorderMLoadCmdCnt = 0;
MS_BOOL _bZorderMLoadEn = FALSE;
//#ifdef ENABLE_SPREADMODE
//MS_BOOL _bSpreandEnable = FALSE;
//#endif

// for high accurate input VFreq case, to avoid reconfig Vtt/Htt
#define CHANGE_VTT_TOLERANCE                5
#define CHANGE_HTT_TOLERANCE                10

#define MINMUN_V_BACKPORCH  4
#define IsFSCDisplayWindowEnable() (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled == TRUE)
//-----------------------------------------------------------------------------
//  below definitions are for FSM FPLL
//-----------------------------------------------------------------------------
typedef enum
{
    EN_OUTPUT_TIMING_CHG_DCLK   = 0,
    EN_OUTPUT_TIMING_HTOTAL = 1,
    EN_OUTPUT_TIMING_VTOTAL = 2,
} EN_OUTPUT_TIMING_ADJUST_MODE;

#define FPLL_FSM_STABLE_TIME    (10000)         // if exceed this stable time, the FPLL FSM will re-init
#define FPLL_PRD_MAX            (0xFFFF)        // Max PRD value
#define FPLL_PHASE_VAR_LIMIT    (0x10)          // If phase error variation exceed this, means un-stable
#define FPLL_PRD_THRESHOLD      (0x100)         // The PRD must be smaller than this value to indicate almost finished PRD lock
#define FPLL_PRD_VAR_LIMIT      (0x10)          // If PRD variation exceed this, means un-stable
#define FPLL_PHASE_MAX          (0xFFFF)        // Max PHASE value
#define FPLL_PHASE_DIFF_MAX     (0x4000)       // Max PHASE diff value
#define FPLL_PRD_STABLE_COUNT   0x06
#define FPLL_SWITCH_LOCKVFREQ_TIME  (500)      // if exceed this stable time, the FPLL would switch to LockVfreq mode

#define GATE_PHASELIMIT         0x3000

#define FPLL_STEP_LIMIT_FOR_ADJUST_SET  0x100  //step limit when adjusting SET

// FPLL
#define IVS_OVS_MAX_DIFF    0x1000UL    // before calculate 2nd SET, will check if IVS/OVS diff too big to verify the LPLL SET accuracy
#define IVS_VAR_LIMIT       0x10        // before calculate 2nd SET, the IVS should be very stable, if the variable exceed this, it's un-stable and not cal. 2nd SET
#define VD_HSYNC_LOCKED                 BIT(14)

//Timing
#define VFREQ_DIFF_TOLERANCE         10        //Value to distinguish two Vfreq(x10Hz)

#define PHASE_LOCK_EN     0

#ifdef MAXIM_ATV_LOCK_PHASE_PATCH
#define ENABLE_LOCK_IVS_OVS_FOR_ATV         0
#else
#define ENABLE_LOCK_IVS_OVS_FOR_ATV         1
#endif

#define SLOWLY_MOVE_STEPS   (1)

#define VTT_OFFSET_FOR_PHASE_LOCK 8
#define DEFAULT_IP_VSYNC_PERIOD   (20)

#define V_PERIOD_DIFF_TOLERANCE   10
//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static SC_FRC_SETTING sFRC_Tbl_Default[MAX_FRC_TABLE_INDEX] =
{
    {0, 160, 0, 1, 4, MS_FRC_1_4},
    {160, 210, 0, 1, 3, MS_FRC_1_3},
    {210, 245, 0, 2, 5, MS_FRC_2_5},  // 24 to 48 {210, 245, 1, 2, MS_FRC_1_2}
    {245, 260, 0, 1, 2, MS_FRC_1_2},
    {260, 400, 0, 1, 2, MS_FRC_1_2},
    {470, 490, 0, 4, 5, MS_FRC_4_5},
    {400, 510, 0, 1, 1, MS_FRC_1_1},
    {510, 0xFFFF, 0, 1, 1, MS_FRC_1_1},
};

static void (*_fpPQSetFPLLThreshMode)(MS_U8) = NULL;

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
MS_S32   _s32FdScaler = -1;
#endif
#endif
#endif
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_FBL_Calc_Vtt(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow);
MS_U16 MDrv_SC_VFreq_Select(void *pInstance, MS_U16 u16AutoVFreq);

// Output Dclk and Loop DIV : Ref from PNL driver. It is easy to maintain if XC and PNL use same function.
MS_U32  MDrv_PNL_CalculateLPLLSETbyDClk(void *pInstance, MS_U64 ldHz, MS_BOOL bHighAccurate);
MS_U8   MHal_PNL_Get_Loop_DIV(void *pInstance, MS_U8 u8LPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz);
MS_U16  MHal_PNL_Get_LPLL_LoopGain(void *pInstance, MS_U8 eLPLL_Mode, MS_U8 eLPLL_Type, MS_U64 ldHz);
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
static void _MApi_XC_FPLL_EnableLcokFreqOnly(void *pInstance,MS_BOOL bEnable);
#endif
static MS_BOOL _MDrv_SC_IsOutVFreValid(void *pInstance, MS_U16 u16OutputVFreq, MS_U16 u16Delta);

#ifdef PATCH_TCON_BRING_UP
const char* MDrv_PNL_GetName(void);
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
extern MS_BOOL MDrv_XC_SetHDRWindow(ST_KDRV_XC_WINDOW_INFO *pstWindowInfo);
extern MS_BOOL MDrv_XC_SetInputSourceType(EN_KDRV_XC_INPUT_SOURCE_TYPE enInputSourceType);
extern MS_BOOL MDrv_XC_SetColorFormat(EN_KDRV_XC_HDR_COLOR_FORMAT enColorFormat);
extern MS_BOOL MDrv_XC_SetShareMemInfo(ST_KDRV_XC_SHARE_MEMORY_INFO *pstShmemInfo);
extern MS_BOOL MDrv_XC_Set3DLutInfo(ST_KDRV_XC_3DLUT_INFO *pst3DLutInfo);
extern MS_BOOL MDrv_XC_SetHDRType(EN_KDRV_XC_HDR_TYPE enHDRType);
extern MS_BOOL MDrv_XC_EnableHDR(MS_BOOL bEnableHDR);
extern MS_BOOL MDrv_XC_ConfigAutoDownload(ST_KDRV_XC_AUTODOWNLOAD_CONFIG_INFO *pstConfigInfo);
extern MS_BOOL MDrv_XC_WriteAutoDownload(ST_KDRV_XC_AUTODOWNLOAD_DATA_INFO *pstDataInfo);
extern MS_BOOL MDrv_XC_FireAutoDownload(EN_KDRV_XC_AUTODOWNLOAD_CLIENT enClient);
extern MS_BOOL KDrv_XC_GetAutoDownloadCaps(ST_KDRV_XC_AUTODOWNLOAD_CLIENT_SUPPORTED_CAPS *pstAutoDownlaodCaps);
extern MS_BOOL MDrv_XC_GetHDRType(EN_KDRV_XC_HDR_TYPE *penHDRType);
extern MS_BOOL MDrv_XC_CFDControl(ST_KDRV_XC_CFD_CONTROL_INFO *pstKdrvCFDCtrlInfo);
extern MS_BOOL KDrv_XC_SetSWDRInfo(ST_KDRV_XC_SWDR_INFO* pstKdrvXcSwdrInfo);
extern MS_BOOL KDrv_XC_GetSWDRInfo(ST_KDRV_XC_SWDR_INFO* pstKdrvXcSwdrInfo);
#endif

//-----------------------------------------------------------------------------
// screen
//-----------------------------------------------------------------------------
void MDrv_SC_Set_BLSK(void *pInstance, MS_BOOL bEnable)
{
    if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        Hal_SC_set_blsk_burst(pInstance, bEnable);
    else
        Hal_SC_set_blsk(pInstance, bEnable);
}

void MDrv_SC_GenerateBlackVideoForBothWin(void *pInstance, MS_BOOL bEnable)
{
    if ( MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED )
        Hal_SC_set_main_sub_black_screen_burst(pInstance, bEnable);
    else
        Hal_SC_set_main_sub_black_screen(pInstance, bEnable);
}

void MDrv_SC_GetCurrentFRCTable(SC_FRC_SETTING* pstFRCTable)
{
    if(pstFRCTable == NULL)
        return;

    memcpy(pstFRCTable, sFRC_Tbl_Default, sizeof(sFRC_Tbl_Default));
}

MS_BOOL _MDrv_SC_IsOutVFreValid(void *pInstance, MS_U16 u16OutputVFreq, MS_U16 u16Delta)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return (((u16OutputVFreq >= 500 - u16Delta) && (u16OutputVFreq <= 500 + u16Delta)) ||
                     ((u16OutputVFreq >= 600 - u16Delta) && (u16OutputVFreq <= 600 + u16Delta)) ||
                     ((u16OutputVFreq >= 300- u16Delta) && (u16OutputVFreq <= 300 + u16Delta)) ||
                     ((u16OutputVFreq >= 240- u16Delta) && (u16OutputVFreq <= 250 + u16Delta)) ||
                     ((((u16OutputVFreq >= 1000- 2*u16Delta) && (u16OutputVFreq <= 1000 + 2*u16Delta))||
                    ((u16OutputVFreq >= 1200- 2*u16Delta) && (u16OutputVFreq <= 1200 + 2*u16Delta)))&&
                    ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 1000)||
                    (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq == 1200))));
}

#ifdef UFO_XC_SETBLACKVIDEOBYMODE
/******************************************************************************/
///enable/diable output black pattern by mode
///@param bEnable \b IN
///-  False:Disable
///-  TRUE :enable
/// @param eWindow  \b IN: which window we are going to set
/// @param eMode  \b IN: which black video mode we are going to use
/******************************************************************************/
E_APIXC_ReturnValue MDrv_SC_GenerateBlackVideoByMode(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow, EN_XC_BLACK_VIDEO_MODE eMode)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;

    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "SC GenerateBlackVideo enable = %d,ewindow = %d\n", bEnable,eWindow);

    if(eWindow == MAIN_WINDOW)
    {
        if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
            eRet = Hal_SC_set_main_black_screen_by_mode_burst(pInstance, bEnable, eMode);
        else
            eRet = Hal_SC_set_main_black_screen_by_mode(pInstance, bEnable, eMode);
    }
    else
    {
        if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
            eRet = Hal_SC_set_sub_blue_screen_by_mode_burst(pInstance, bEnable, DISABLE, eMode);
        else
            eRet = Hal_SC_set_sub_blue_screen_by_mode(pInstance, bEnable, DISABLE, eMode);
    }

    return eRet;
}
#endif
void _notify_kernel(MS_BOOL bEnable)
{
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
                return ;
            }
        }
#endif

        ST_KDRV_XC_CFD_CONTROL_INFO stXCCfdControlInfo;
        ST_KDRV_XC_CFD_UPDATE_STATUS stXCCfdUpdateStatus;
        memset(&stXCCfdControlInfo, 0, sizeof(ST_KDRV_XC_CFD_CONTROL_INFO));
        memset(&stXCCfdUpdateStatus, 0, sizeof(stXCCfdUpdateStatus));
        stXCCfdControlInfo.enCtrlType = E_KDRV_XC_CFD_CTRL_UPDATE_STATUS;
        if(bEnable)
            stXCCfdUpdateStatus.enCFD_status = E_KDRV_XC_CFD_STATUS_SYNC_GEN_BLACK_SCREEN_ENABLE;
        else
            stXCCfdUpdateStatus.enCFD_status = E_KDRV_XC_CFD_STATUS_SYNC_GEN_BLACK_SCREEN_DISABLE;

        stXCCfdControlInfo.pParam = &stXCCfdUpdateStatus;
        stXCCfdControlInfo.u32ParamLen = sizeof(stXCCfdUpdateStatus);
        stXCCfdControlInfo.u16ErrCode = 0;
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if (ioctl(_s32FdScaler, MDRV_XC_IOC_CFDCONCTRL, &stXCCfdControlInfo))
        {
            printf("MDRV_XC_IOC_CFDCONCTRL fail!!!!\n");
        }
#endif
}
/******************************************************************************/
///enable/diable output black pattern
///@param bEnable \b IN
///-  False:Disable
///-  TRUE :enable
/******************************************************************************/
void MDrv_SC_GenerateBlackVideo(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //MS_U8 u8Bank;
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "SC%d GenerateBlackVideo enable = %d,ewindow = %d\n", psXCInstPri->u32DeviceID,bEnable,eWindow);
    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    EN_MUTE_TYPE eMuteType = E_MAIN_WINDOW_MUTE;
    switch(eWindow)
    {
        case MAIN_WINDOW:
            eMuteType = E_MAIN_WINDOW_MUTE;
            break;
        case SUB_WINDOW:
            eMuteType = E_SUB_WINDOW_MUTE;
            break;
        default:
            break;
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if(psXCInstPri->u32DeviceID == 0)
#else
    if(eWindow == MAIN_WINDOW)
#endif
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(eWindow == MAIN_WINDOW)
#endif
        {
            if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(eMuteType)) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)))
                Hal_SC_set_main_black_screen_burst(pInstance, bEnable);
            else
                Hal_SC_set_main_black_screen(pInstance, bEnable);
            _notify_kernel(bEnable);
        }
    }
    else
    {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB //follow HW_DESIGN_4K2K_VER = 6 rule
        if(eWindow == MAIN_WINDOW)
        {
            if((MDrv_XC_MLoad_GetStatus(g_pDevice0Instance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetSkipWaitVsync(g_pDevice0Instance, eWindow)))
                Hal_SC_set_sub_blue_screen_burst(g_pDevice0Instance, bEnable, DISABLE);
            else
                Hal_SC_set_sub_blue_screen(g_pDevice0Instance, bEnable, DISABLE);
        }
#else
        if((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(eMuteType)) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)))
            Hal_SC_set_sub_blue_screen_burst(pInstance, bEnable, DISABLE);
        else
            Hal_SC_set_sub_blue_screen(pInstance, bEnable, DISABLE);
#endif
    }

    #if VERIFY_DISPLAY_FPGA

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    //----- enable IP signal for FPLL ---//
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    //MDrv_WriteRegBit(H_BK_IP1F2(0x03), 1, BIT(7));
    //MDrv_WriteRegBit(H_BK_IP1F2(0x03), 1, BIT(3));
    //MDrv_WriteByte(BK_SELECT_00, u8Bank);



    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0x8000, BIT(15));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0x0800, BIT(11));

    //--- set LVDS format ----//
#ifdef SUPPORT_MOD_ADBANK_SEPARATE

#else
    MOD_W2BYTEMSK(REG_MOD_BK00_40_L, 0x002C, 0x00FF);  // set LVDS format
    MOD_W2BYTEMSK(REG_MOD_BK00_4B_L, 0x0000, 0x00FF);  // set LVDS format
    MOD_W2BYTEMSK(REG_MOD_BK00_78_L, 0x007E, 0x00FF);  // set LVDS driving
#endif
    #endif
    //--- restore bank select ---//

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);

}

void MDrv_XC_SetDispWindowColor(void *pInstance, MS_U8 u8Color, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u8DispWindowColor_Suspend[eWindow] = u8Color;
    Hal_SC_set_nosignal_color(pInstance, u8Color,eWindow);
}

//-------------------------------------------------------------------------------------------------
/// set display window color
/// @param  u8Color                 \b IN: the color to set
/// @param  eWindow                 \b IN: which window we are going to set
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetDispWindowColor_U2(void* pInstance, MS_U8 u8Color, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetDispWindowColor(pInstance, u8Color, eWindow);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetDispWindowColor(MS_U8 u8Color, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_DISPLAY_WINDOW_COLOR XCArgs;
    XCArgs.u8Color = u8Color;
    XCArgs.eWindow = eWindow;
    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_DISPLAY_WINDOW_COLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//get the greatest common divisor
MS_U16 MDrv_SC_GCD(MS_U16 m, MS_U16 n)
{
    MS_U16 m_cup, n_cup, res;

    m_cup = m;
    n_cup = n;
    res = m_cup % n_cup;
    while (res != 0)
    {
        m_cup = n_cup;
        n_cup = res;
        res = m_cup % n_cup;
    }

    return n_cup;
}

void MDrv_SC_Handle_Special_FRCTbl(void *pInstance, MS_U16 u16DefaultVFreq, MS_U16 u16InputVFreq, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8Ratio = 0;
    MS_U16 u16FRCType=0;
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bIsOutput48hz = (!pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable) && MDrv_SC_3D_IsOutputVfreq48hz(pInstance, eWindow, u16InputVFreq);
#else
    MS_BOOL bIsOutput48hz = FALSE;
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@@ bIsOutput48hz = %s\n",bIsOutput48hz?"TRUE":"FALSE")
    memcpy(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl, sFRC_Tbl_Default, sizeof(sFRC_Tbl_Default));

    // for muji 4k 0.5k@240 case
    // AP send correct FRC in/out table, so no need to modify by  u8Ratio
    if(u16DefaultVFreq > 1800)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "=====>  @@@ 4k 0.5k@240 u16DefaultVFreq = %d\n",u16DefaultVFreq)
        u8Ratio = 1;
    }
    //when the panel is 120hz default vfreq, the FRC table should be double the ratio.
    //eg. input:30, original x2 out:60, but for 120hz panel, we should x4 out:120.
    //    the multiple is from x2 to x4
    else if(u16DefaultVFreq > 900)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            if((u16InputVFreq > 235) && (u16InputVFreq <= 245))
            {
                u8Ratio = 5;
            }
            else
            {
                u8Ratio = 4;
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FRC table FRC_OUT x4 for 240 hz panel output\n")
        }
        else
        {
            u8Ratio = 2;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FRC table FRC_OUT x2 for 120 hz panel output\n")
        }
    }
    else
    {
            u8Ratio = 1;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16DefaultVFreq=%u, u8Ratio = %u\n", u16DefaultVFreq, u8Ratio) ;

    for(u16FRCType = 0; u16FRCType < sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING); u16FRCType++)
    {
        // 24Hz -> 48Hz
        if (bIsOutput48hz &&((u16DefaultVFreq == 600)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)))
        {
            // check FRC, and modify FRC table.
            if((u16InputVFreq > pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16LowerBound) &&
            (u16InputVFreq <= pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16HigherBound))
            {
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In = 1;
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out = 2;
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].eFRC_Type= MS_FRC_1_2;
            }
        }

        if (u16DefaultVFreq > 900)
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                if(((u16InputVFreq > 235)
                    && (u16InputVFreq <= 245))
                   ||((u16InputVFreq > 290)
                      && (u16InputVFreq <= 310)))
                {
                    pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In = 1;
                    pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out = 2;
                    pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].eFRC_Type= MS_FRC_1_2;
                }
            }
        }

        pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out *= u8Ratio;
    }

}
//-----------------------------------------------------------------------------
// frame rate convert
//-----------------------------------------------------------------------------

E_APIXC_ReturnValue MApi_XC_CustomizeFRCTable_U2(void* pInstance, SC_FRC_SETTING* stFRCTable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (stFRCTable == NULL )
    {
        SC_FRC_SETTING sFRC_Tbl_Temp[MAX_FRC_TABLE_INDEX] =
        {
            {0, 160, 0, 1, 4, MS_FRC_1_4},
            {160, 210, 0, 1, 3, MS_FRC_1_3},
            {210, 245, 0, 2, 5, MS_FRC_2_5},
            {245, 260, 0, 1, 2, MS_FRC_1_2},
            {260, 400, 0, 1, 2, MS_FRC_1_2},
            {400, 510, 0, 1, 1, MS_FRC_1_1},
            {510, 0xFFFF, 0, 1, 1, MS_FRC_1_1},
        };
        pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable = FALSE;
        memcpy( sFRC_Tbl_Default, sFRC_Tbl_Temp , sizeof(sFRC_Tbl_Default) );
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Attention! default frc table is using!\n");
    }
    else
    {
        MS_U8 u8Index = 0;
        for(u8Index=0;u8Index<MAX_FRC_TABLE_INDEX;u8Index++) // calculate the counts of customer frc table
        {
            if((stFRCTable[u8Index]).u16HigherBound == 0xFFFF) // 0xFFFF was found
                break;
        }
        if((stFRCTable[u8Index]).u16HigherBound != 0xFFFF) // 0xFFFF was not found
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Warning! custom frc table is invalid(the last row should define a range of vfreq up to 0xFFFF), using the default table instead!!\n");
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable = TRUE;
            memcpy( sFRC_Tbl_Default, stFRCTable, MIN((sizeof(sFRC_Tbl_Default)),(sizeof(SC_FRC_SETTING)*(u8Index+1))) );
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Attention! custom frc table is using!\n");
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_CustomizeFRCTable(SC_FRC_SETTING* stFRCTable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_CUSTOMIZE_FRC_TABLE XCArgs;
    XCArgs.stFRCTable = stFRCTable;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CUSTOMIZE_FRC_TABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Register PQ Set FPLL thresh mode WR bank mapping function.
/// @param  fpXCCB              \b IN: function pointer of PQ set FPLL thresh mode WR bank mapping
//-------------------------------------------------------------------------------------------------
void MApi_XC_RegisterPQSetFPLLThreshMode_U2(void* pInstance, void (*fpPQCB)(MS_U8 u8PQWin))
{
    _fpPQSetFPLLThreshMode = fpPQCB;
}

void MApi_XC_RegisterPQSetFPLLThreshMode(void (*fpPQCB)(MS_U8 u8PQWin))
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_REGISTER_PQ_FPLL_THRESH_MODE XCArgs;
    XCArgs.fpPQCB = fpPQCB;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_REGISTER_PQ_FPLL_THRESH_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

#ifdef ENABLE_SPREADMODE
void _MApi_XC_FPLL_EnableSpreadMode(void *pInstance, EN_FPLL_SPREAD_MODE eSpreadMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(eSpreadMode!=FPLL_SPREAD_MODE_NONE)
    {
        MS_U32 u32Spreadmodetemp=0;
        MS_U8 u8FrameIndiv=0;
        MS_U8 u8FrameOutdiv=0;

        if(eSpreadMode==FPLL_SPREAD_MODE_1_2)
        {
            u8FrameIndiv=1;
            u8FrameOutdiv=2;
        }
        else if(eSpreadMode==FPLL_SPREAD_MODE_2_5)
        {
            u8FrameIndiv=2;
            u8FrameOutdiv=5;
        }
         else if(eSpreadMode==FPLL_SPREAD_MODE_3_5)
        {
            u8FrameIndiv=3;
            u8FrameOutdiv=5;
        }
         else if(eSpreadMode==FPLL_SPREAD_MODE_4_5)
        {
            u8FrameIndiv=4;
            u8FrameOutdiv=5;
        }
        else if(eSpreadMode==FPLL_SPREAD_MODE_5_6)
        {
            u8FrameIndiv=5;
            u8FrameOutdiv=6;
        }
        else if(eSpreadMode==FPLL_SPREAD_MODE_5_12)
        {
            u8FrameIndiv=5;
            u8FrameOutdiv=12;
        }
        else if(eSpreadMode==FPLL_SPREAD_MODE_12_25)
        {
            u8FrameIndiv=12;
            u8FrameOutdiv=25;
        }
        else if(eSpreadMode==FPLL_SPREAD_MODE_24_25)
        {
            u8FrameIndiv=24;
            u8FrameOutdiv=25;
        }

        MsOS_DelayTask(50);
        u32Spreadmodetemp= (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_28_L) + 1) * (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1F_L)) * u8FrameIndiv / u8FrameOutdiv ;
        W2BYTEMSK(L_BK_LPLL(0x7E), BIT(15), BIT(15)); //open sprend mode
        W2BYTEMSK(L_BK_LPLL(0x7A), u8FrameOutdiv-1 , 0x00FF);
        W2BYTEMSK(L_BK_LPLL(0x19), u32Spreadmodetemp, 0xFFFF);
        W2BYTEMSK(L_BK_LPLL(0x1A), u32Spreadmodetemp>>16, 0xFFFF);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SpreadMode: u8FrameIndiv = %d, u8FrameOutdiv = %d\n",u8FrameIndiv,u8FrameOutdiv);
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bSpreandEnable=TRUE;

    }
    else
    {
        W2BYTEMSK(L_BK_LPLL(0x7E), 0 , BIT(15));  //close sprend mode
        W2BYTEMSK(L_BK_LPLL(0x7A), 0, 0x00FF);
        W2BYTEMSK(L_BK_LPLL(0x19), 0, 0xFFFF);
        W2BYTEMSK(L_BK_LPLL(0x1A), 0, 0xFFFF);
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bSpreandEnable=FALSE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Close spread mode\n");
    }

}
#endif


MS_U16 MDrv_SC_Cal_FRC_Output_Vfreq(void *pInstance, MS_U16 u16InputVFreq,
                             MS_U16 u16DefaultVFreq,
                             MS_BOOL bFBL,
                             MS_U8 *u8FrcIn,
                             MS_U8 *u8FrcOut,
                             MS_BOOL *bSetFrameLock,
                             MS_U8 u8Lpll_bank,
                             SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16OutputVFreq = u16InputVFreq;
#ifndef DISABLE_3D_FUNCTION
    MS_BOOL bIsOutput48hz = (!pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable) && MDrv_SC_3D_IsOutputVfreq48hz(pInstance, eWindow, u16InputVFreq);
#else
    MS_BOOL bIsOutput48hz = FALSE;
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"u16DefaultVFreq = %d, u16InputVFreq =%d\n",
                                        u16DefaultVFreq, u16InputVFreq);

    MDrv_SC_Handle_Special_FRCTbl(pInstance, u16DefaultVFreq, u16InputVFreq, eWindow);

    if (!bFBL)
    {
        MS_U16 u16FRCType=0;
        for(u16FRCType = 0; u16FRCType < (sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING)); u16FRCType++)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,
                "InputV = %d, Tbl[%d] = (%d, %d)(%d, %d)\n", u16InputVFreq, u16FRCType,
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16LowerBound,
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16HigherBound,
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In,
                pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out) ;

            // check FRC
            if((u16InputVFreq > pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16LowerBound) &&
            (u16InputVFreq <= pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u16HigherBound))
            {
                break;
            }
        }

        MS_ASSERT(u16FRCType <= sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING));

        if(u16FRCType >= (sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING)))
        {
            u16FRCType = (sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING))-1;
        }

        // Add this condition to avoid the divider is 0
        if(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In == 0)
        {
            pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In = 1;
        }

        // Set output V
        if(u16FRCType < sizeof(pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl)/sizeof(SC_FRC_SETTING))
        {
            u16OutputVFreq = u16InputVFreq * pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out
                                          / pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In;

            *u8FrcIn = pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_In;
            *u8FrcOut = pXCResourcePrivate->stdrvXC_Display.sFRC_Tbl[u16FRCType].u8FRC_Out;

            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16DefaultVFreq=%d,u8FrcOut = %d, u16OutputVFreq = %d\n", u16DefaultVFreq, *u8FrcOut, u16OutputVFreq) ;

            /// for Default Vfreq = 100/120Hz
            if((u16DefaultVFreq > 900)&&(u16DefaultVFreq < 1100))
            {
                if(u16OutputVFreq > 1100)
                {
                    u16OutputVFreq/=2;
                    *u8FrcOut/=2;
                }
            }
            else if((u16DefaultVFreq > 1100)&&(u16DefaultVFreq < 1300))
            {
                if(u16OutputVFreq > 1300)
                {
                    u16OutputVFreq/=2;
                    *u8FrcOut/=2;
                }
            }
        }
        else
        {
            u16OutputVFreq = 0;
            *u8FrcIn = 1;
            *u8FrcOut = 1;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16DefaultVFreq=%d,u8FrcOut = %d, u16OutputVFreq = %d\n", u16DefaultVFreq, *u8FrcOut, u16OutputVFreq) ;

        #if FRC_INSIDE
        // For 4K2K 2nd FRC used!!!
        if ((1 == u8Lpll_bank) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal> 3000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000) &&
            (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_4K2K)
            {
                if (u16InputVFreq >= 320)
                {
                    u16OutputVFreq = u16InputVFreq/2;
                    *u8FrcIn = 2;
                    *u8FrcOut = 1;
                }
                else
                {
                    u16OutputVFreq = u16InputVFreq;
                    *u8FrcIn = 1;
                    *u8FrcOut = 1;
                }
            }

        }
        #endif
    }
    else
    {
        *u8FrcIn = 1;
        *u8FrcOut = 1;
    }

    LPLL_BK_STORE;
    if(u8Lpll_bank)
      LPLL_BK_SWITCH(0x01);
    else
      LPLL_BK_SWITCH(0x00);

    // 4K1K FRC 120Hz output case
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ) && (0 == u8Lpll_bank))
    {
        if ((u16InputVFreq >= 235) && (u16InputVFreq <= 245))
        {
            *u8FrcIn = 1;
            *u8FrcOut = 5;
        }
        else
        {
            *u8FrcOut = *u8FrcOut*2;
        }
    }

    // 60->120 or 50->100
    if((u16DefaultVFreq == 1200)||(u16DefaultVFreq == 1000))
    {
        if(((u16InputVFreq >= 1180) && (u16InputVFreq <= 1220))||
           ((u16InputVFreq >= 980) && (u16InputVFreq <= 1020)))
        {
            if(bIsOutput48hz)
            {
                *u8FrcIn = 1;
                *u8FrcOut = 5;
            }
            else
            {
                *u8FrcIn = 1;
                *u8FrcOut = 2;
            }
        }

    }

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
    {
        // Napoli 4K 0.5K FRC 240Hz output case
        if (0 == u8Lpll_bank)
            *u8FrcOut = *u8FrcOut*4;

    }
    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
#ifndef DISABLE_3D_FUNCTION
    else if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        &&((u16DefaultVFreq == 600)||(u16DefaultVFreq == 500))
        &&(MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
        {
            if(bIsOutput48hz)
            {
                *u8FrcIn = 1;
                *u8FrcOut = 2;
            }
        }
        else
        {
            *u8FrcIn = 1;
            *u8FrcOut = 2;
        }
    }
#endif
    #endif
#ifdef FA_1920X540_OUTPUT
    else
    if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
      ((MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_TOP_BOTTOM)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FIELD_ALTERNATIVE))&&
      (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
        *u8FrcIn = 1;
        *u8FrcOut = 2;
    }
#endif
#ifdef TBP_1920X2160_OUTPUT
    else if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
            ((gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
             (gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))&&
            (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FRAME_PACKING)&&
            (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM))
    {
        *u8FrcIn = 1;
        *u8FrcOut = 1;
    }
#endif
#if (HW_DESIGN_4K2K_VER == 6) // for maserati 4K2K@120Hz case
    else if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        && IsVBY1_16LANE((MS_U8)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)))
    {
        *u8FrcOut = *u8FrcOut*2;
    }
#endif

    if((pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
        &&(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_SRC))
    {
        if((u16DefaultVFreq == 300) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal> 3000) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000))
        {
            *u8FrcIn = 2;
            *u8FrcOut = 1;
        }
        else
        {
            *u8FrcIn = 1;
            *u8FrcOut = 1;
        }
    }
    pXCResourcePrivate->stdrvXC_Display.g_u8FrcIn = *u8FrcIn;
    pXCResourcePrivate->stdrvXC_Display.g_u8FrcOut = *u8FrcOut;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16DefaultVFreq=%d,Final u8FrcIn = %d,u8FrcOut = %d, u16OutputVFreq = %d\n", u16DefaultVFreq, *u8FrcIn,*u8FrcOut, u16OutputVFreq) ;
#ifdef ENABLE_SPREADMODE
        if(MHal_XC_IsSupportSpreadMode(pInstance))
        {
            if(!(MDrv_XC_FPLL_IsSupportLockFreqOnly(pInstance))
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6)) // for maserati 4K2K@120Hz case, not use spread mode
        && !((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
            && IsVBY1_16LANE((MS_U8)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)))
#endif
        )
            {
                if((*u8FrcIn==1) && (*u8FrcOut==2))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_1_2);
                }
                else if((*u8FrcIn==2) && (*u8FrcOut==5))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_2_5);
                }
                else if((*u8FrcIn==3) && (*u8FrcOut==5))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_3_5);
                }
                else if((*u8FrcIn==4) && (*u8FrcOut==5))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_4_5);
                }
                else if((*u8FrcIn==5) && (*u8FrcOut==6))
                {
                  // 5:6 case have not implement
                  // _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_5_6);
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_NONE);
                }
                else if((*u8FrcIn==5) && (*u8FrcOut==12))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_5_12); //2
                }
                else if((*u8FrcIn==12) && (*u8FrcOut==25))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_12_25); //2
                }
                else if((*u8FrcIn==24) && (*u8FrcOut==25))
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_24_25);
                }
                else
                {
                    _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_NONE); //close spread mode for other source
                }

            }
        else
        {
            _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_NONE); //close spread mode for lockFreqOnlymod
        }
        }
#endif

    //extend frc bit for lpll 0
    if(u8Lpll_bank == 0)
    {
        MS_U8 u8FrcOutTmp = *u8FrcOut;
#ifdef ENABLE_SPREADMODE
            if(MHal_XC_IsSupportSpreadMode(pInstance))
            {
                // 5:6 case have not implement
                if (!(MDrv_XC_FPLL_IsSupportLockFreqOnly(pInstance)) && (((*u8FrcIn==1) && (*u8FrcOut==2)) || ((*u8FrcIn==2) && (*u8FrcOut==5)) || ((*u8FrcIn==3) && (*u8FrcOut==5))|| ((*u8FrcIn==4) && (*u8FrcOut==5))/*|| ((*u8FrcIn==5) && (*u8FrcOut==6))*/|| ((*u8FrcIn==5) && (*u8FrcOut==12))|| ((*u8FrcIn==12) && (*u8FrcOut==25))|| ((*u8FrcIn==24) && (*u8FrcOut==25)))
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6)) // for maserati 4K2K@120Hz case, not use spread mode
                && !((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
                    && IsVBY1_16LANE((MS_U8)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)))
#endif
                )
                {
                    u8FrcOutTmp=1;
                }
             }
#endif
        #ifdef SUPPORT_FPLL_DOUBLE_OVS
        // for muji U02, 4K 0.5K@240Hz case, there is new bit 0x3C[11] to control from 5:24 to 5:48
        if (MDrv_ReadByte(REG_CHIP_REVISION) >= 1)
        {
            if(u8FrcOutTmp > 32)
            {
                u8FrcOutTmp = u8FrcOutTmp/2;
                MDrv_Write2ByteMask(L_BK_LPLL(0x3C), 0, BIT(11));
            }
            else
            {
                MDrv_Write2ByteMask(L_BK_LPLL(0x3C), BIT(11), BIT(11));
            }
        }
        #endif
        MDrv_WriteByte(H_BK_LPLL(0x0C), (((u8FrcOutTmp-1)&0xF) << 4) | ((*u8FrcIn-1)&0xF));
        MDrv_WriteByte(L_BK_LPLL(0x0E), (((u8FrcOutTmp-1)>>4) << 4) | ((*u8FrcIn-1)>>4));
    }
    else
    {
        MDrv_WriteByte(H_BK_LPLL(0x0C), ((*u8FrcOut-1) << 4) | (*u8FrcIn-1));
    }
    LPLL_BK_RESTORE;

    //for force case
    if (pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "force free run\n") ;
        if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_SRC)
            u16OutputVFreq = MDrv_SC_VFreq_Select(pInstance, u16InputVFreq);
        else
            u16OutputVFreq = MDrv_SC_VFreq_Select(pInstance, u16OutputVFreq);

        MDrv_WriteByte(H_BK_LPLL(0x0C), 0x00); //clear FRC before set freerun (fps_in:fps_out = 1:1)
        MDrv_WriteByte(L_BK_LPLL(0x0E), 0x00);

        *bSetFrameLock = FALSE;
    }
    //for normal case
    else if (_MDrv_SC_IsOutVFreValid(pInstance, u16OutputVFreq, 10)
#if (HW_DESIGN_3D_VER == 4)
            // for FRC FPP-FA 1280x1470@60/SBSH-FA/TBP-FA
            // for FRC FPP-FA 1280x1470@50/SBSH-FA/TBP-FA
             ||((((u16OutputVFreq >= 1180) && (u16OutputVFreq <= 1220))
               ||((u16OutputVFreq >= 980) && (u16OutputVFreq <= 1020))
               ||((u16OutputVFreq >= 2380) && (u16OutputVFreq <= 2420)))
             && ((u16DefaultVFreq == 600)||(u16DefaultVFreq == 500))
             && (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
             || ((u16OutputVFreq >= 1980) && (u16OutputVFreq <= 2020))
             || ((u16OutputVFreq >= 2380) && (u16OutputVFreq <= 2420))

#endif
             || ((pXCResourcePrivate->stdrvXC_Display._bUseCustomFRCTable)
                && _MDrv_SC_IsOutVFreValid(pInstance, u16OutputVFreq, 25))
             || (bIsOutput48hz && (u16OutputVFreq >= 460) && (u16OutputVFreq <= 490))
             // For 4K2K 2nd FRC used!!
             || ((u16DefaultVFreq == 300)
                && (1 == u8Lpll_bank)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal> 3000)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal > 2000)
                && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500)
                && (((u16OutputVFreq >= 490) && (u16OutputVFreq <= 510)) ||
                    ((u16OutputVFreq >= 590) && (u16OutputVFreq <= 610)) ||
                    ((u16OutputVFreq >= 290) && (u16OutputVFreq <= 310)) ||
                    ((u16OutputVFreq >= 230) && (u16OutputVFreq <= 260))))
             // For Muji 4K 0.5K case
             || ((((u16OutputVFreq >= 1980) && (u16OutputVFreq <= 2020))||
                ((u16OutputVFreq >= 2380) && (u16OutputVFreq <= 2420)))&&
                ((u16DefaultVFreq == 2000)||(u16DefaultVFreq == 2400))))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Frame Lock = True\n");
        //u16OutputVFreq = _MDrv_SC_RefineVFreq(MAIN_WINDOW, u16InputVFreq, u16OutputVFreq);
        *bSetFrameLock = TRUE;
    }
    //abnormal case
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Free Run\n");
        // For FRC panel and freerun at specific framerate
        if(u16DefaultVFreq > 900)
        {
            u16OutputVFreq = 1200;
        }
        else if(((u16OutputVFreq >= 480) && (u16OutputVFreq < 490))
                || ((u16OutputVFreq > 510) && (u16OutputVFreq <= 520)))
        {
            u16OutputVFreq = 500; //for better quality
        }
        else
        {
            // Free run at panel default timing.
            u16OutputVFreq = u16DefaultVFreq;
        }
        *bSetFrameLock = FALSE;
    }

#if (HW_DESIGN_HDMITX_VER == 1 || HW_DESIGN_HDMITX_VER == 2)
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        if(u16OutputVFreq >= 595 && u16OutputVFreq < 605)
            u16OutputVFreq = 600;
        else if(u16OutputVFreq >= 495 && u16OutputVFreq < 505)
            u16OutputVFreq = 500;
        else if(u16OutputVFreq >= 295 && u16OutputVFreq < 305)
            u16OutputVFreq = 300;
        else if(u16OutputVFreq >= 245 && u16OutputVFreq < 255)
            u16OutputVFreq = 250;
        else if(u16OutputVFreq >= 235 && u16OutputVFreq < 245)
            u16OutputVFreq = 240;
    }
#endif

    return u16OutputVFreq;
}


//-----------------------------------------------------------------------------
// LPLL
//-----------------------------------------------------------------------------
void MDrv_SC_set_fpll(void *pInstance, MS_BOOL bEnable, MS_BOOL bEnablePhaseLock, MS_U8 u8Lpll_bank)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U32 u32OrigLPLL = 0;
#if(!PHASE_LOCK_EN)
    bEnablePhaseLock = 0;
#endif
    // store bank
    LPLL_BK_STORE;
    // switch bank
    if(u8Lpll_bank)
        LPLL_BK_SWITCH(0x01);
    else
        LPLL_BK_SWITCH(0x00);

    u32OrigLPLL = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_SC_set_fpll(%u,%u)\n", bEnable, bEnablePhaseLock) ;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32OrigLPLL = %tx\n", (ptrdiff_t)u32OrigLPLL) ;

    if(bEnable)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), BIT(3), BIT(3));
        if(u8Lpll_bank==1)
        {
            //printf("1111\n\n");
            MDrv_Write2ByteMask(L_BK_LPLL(0x7F), 0x00, BIT(8));
        }
    }
    else
    {
        if(u8Lpll_bank==1)
        {
            //printf("2222\n\n");
            MDrv_Write2ByteMask(L_BK_LPLL(0x7F), BIT(8), BIT(8));
        }
        MDrv_Write2ByteMask(L_BK_LPLL(0x0C), 0x00, BIT(3));
    }
    MDrv_Write2ByteMask(L_BK_LPLL(0x0C), bEnablePhaseLock << 6, BIT(6));

    #if (HW_DESIGN_HDMITX_VER == 1 || HW_DESIGN_HDMITX_VER == 2) //for Maxim HDMITX and Maserati + Raptor, to make u32OrigLPLL change smoothly
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        MS_U32 u32CurrentLockLPLL = MDrv_Read4Byte(L_BK_LPLL(0x28));
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32CurrentLockLPLL);
        while(abs(u32OrigLPLL - u32CurrentLockLPLL) > FPLL_STEP_LIMIT_FOR_ADJUST_SET)
        {
            if(u32CurrentLockLPLL > u32OrigLPLL)
            {
                u32CurrentLockLPLL -= FPLL_STEP_LIMIT_FOR_ADJUST_SET;
            }
            else
            {
                u32CurrentLockLPLL += FPLL_STEP_LIMIT_FOR_ADJUST_SET;
            }
            MsOS_DelayTask(1);
            MDrv_Write3Byte(L_BK_LPLL(0x0F), u32CurrentLockLPLL);
        }
    }
    #endif

    // !!Initial set have to re-update again to force LPLL free run at initial state or
    // lock from initial set!!
    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32OrigLPLL = %tx, L_BK_LPLL(0x0F) = %tx\n", (ptrdiff_t)u32OrigLPLL, (ptrdiff_t)MDrv_Read4Byte(L_BK_LPLL(0x0F))) ;

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
    if(u8Lpll_bank == 0)
    {
        MS_BOOL bLockEn = FALSE;
        if(MDrv_XC_FPLL_IsSupportLockFreqOnly(pInstance) == TRUE)
            bLockEn = bEnable;
        if(bLockEn && (!IsSrcTypeDigitalVD(gSrcInfo[MAIN_WINDOW].enInputSourceType)))
        {
            bLockEn = TRUE;
        }
        else
        {
            bLockEn = FALSE;
        }
        _MApi_XC_FPLL_EnableLcokFreqOnly(pInstance,bLockEn);
    }
#endif

    // restore bank
    LPLL_BK_RESTORE;


}

void MDrv_SC_clear_lpll(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, 0x40, BIT(6));
}

void MDrv_SC_monitor_lpll(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16Tmp;
    //MS_U8 u8Bank;

    u16Tmp = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L);

    if (u16Tmp & BIT(6))
    {
        //MDrv_WriteByte(L_BK_IP1F2(0x0F), u8Tmp & ~(BIT(6)));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16Tmp & ~(BIT(6)));
    }

}

static void MDrv_SC_adjust_output_htt_vtt(MS_U16 *u16Htt, MS_U16 *u16Vtt,
                                          MS_U32 u32HighAccurateInputVFreq,
                                          MS_U16 u16DefaultHtt,
                                          MS_U16 u16DefaultVtt,
                                          MS_U16 u16DefaultVFreq,
                                          MS_BOOL bChangeVtt)
{
    MS_U64 u64DCLK;
    u64DCLK = ((MS_U64)u16DefaultHtt) * u16DefaultVtt * u16DefaultVFreq;///10;
    do_div(u64DCLK,10);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,
        "default Htt=%u, Vtt=%u, VFreq=%u, dclk=%u\n",
        u16DefaultHtt,
        u16DefaultVtt,
        u16DefaultVFreq, (unsigned int)u64DCLK);

    if(u32HighAccurateInputVFreq == 0)
    {
        return;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "1.Htt=%u, Vtt=%u, VFreq=%u, dclk=%u\n",
        *u16Htt, *u16Vtt, (unsigned int)(u32HighAccurateInputVFreq/100) ,(unsigned int)(((MS_U32)(*u16Htt)) * (*u16Vtt) * (u32HighAccurateInputVFreq/100)/10));

    if ((u32HighAccurateInputVFreq/100) != u16DefaultVFreq)
    {
        if (bChangeVtt)
        {
            u64DCLK = u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Htt)/100)/2;
            do_div(u64DCLK,(u32HighAccurateInputVFreq * (*u16Htt)/100));
            //*u16Vtt = (u64DCLK*10 + (u16VFreq * (*u16Htt))/2) / (u16VFreq * (*u16Htt));
            *u16Vtt = (MS_U16)u64DCLK;
        }
        else
        {
            u64DCLK =  u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Vtt)/100)/2;
            do_div(u64DCLK,(u32HighAccurateInputVFreq * (*u16Vtt)/100));
            *u16Htt = (MS_U16)u64DCLK;
            //*u16Htt = (u64DCLK*10 + (u32HighAccurateInputVFreq * (*u16Vtt)/100)/2) / (u32HighAccurateInputVFreq * (*u16Vtt)/100);
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "2.Adjusted Htt=%u, Vtt=%u, VFreq=%u, dclk=%u\n",
        *u16Htt, *u16Vtt, (unsigned int)(u32HighAccurateInputVFreq/100), (unsigned int)(((MS_U32)(*u16Htt)) * (*u16Vtt) * (u32HighAccurateInputVFreq/100)/10));

}

void MDrv_SC_set_fastlpll(void *pInstance, MS_BOOL bEn)
{
    MDrv_WriteRegBit(H_BK_LPLL(0x1E), bEn, 0x20);  //Enable/Disable Fast LPLL
}

MS_BOOL MDrv_SC_get_fastlpll(void *pInstance)
{
    return MDrv_ReadRegBit(H_BK_LPLL(0x1E), 0x20);  //Enable/Disable FBL
}
E_APIXC_ReturnValue MApi_XC_SetOutputAdjustSetting_U2(void* pInstance, XC_OUTPUT_TIMING_ADJUST_SETTING *stAdjSetting)
{
    XC_PANEL_INFO_EX stPanelInfoEx;
    if (stAdjSetting==NULL)
    {
        return E_APIXC_RET_FAIL;
    }

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
    stPanelInfoEx.bVttDelayValid = TRUE;
    stPanelInfoEx.u16HttDelay = stAdjSetting->u16VTotal_Delay;
    stPanelInfoEx.bVttStepsValid = TRUE;
    stPanelInfoEx.u16HttSteps = stAdjSetting->u16VTotal_Step;
    stPanelInfoEx.bHttDelayValid = TRUE;
    stPanelInfoEx.u16VttDelay = stAdjSetting->u16HTotal_Delay;
    stPanelInfoEx.bHttStepsValid = TRUE;
    stPanelInfoEx.u16VttSteps= stAdjSetting->u16HTotal_Step;
    stPanelInfoEx.bDclkDelayValid = TRUE;
    stPanelInfoEx.u16DclkDelay = stAdjSetting->u16Dclk_Delay;
    stPanelInfoEx.bDclkStepsValid = TRUE;
    stPanelInfoEx.u16DclkSteps= stAdjSetting->u16Dclk_Step;
    stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
    stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);
    //Step setting for change to 50hz
    stPanelInfoEx.u16VFreq = 500;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    if(MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx) == FALSE)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return E_APIXC_RET_FAIL;
    }
    //Step setting for change to 60hz
    stPanelInfoEx.u16VFreq = 600;
    if(MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx) == FALSE)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return E_APIXC_RET_FAIL;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MApi_XC_SetOutputAdjustSetting(XC_OUTPUT_TIMING_ADJUST_SETTING *stAdjSetting)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OUTPUT_ADJUST_SETTING XCArgs;
    XCArgs.stAdjSetting = stAdjSetting;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_ADJUST_SETTING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

MS_BOOL MApi_XC_Set_CustomerSyncInfo_U2(void* pInstance, MS_U16 u16FrontPorch, MS_U16 u16SyncWidth, MS_U16 u16OutputVfreq)
{
    XC_PANEL_INFO_EX stPanelInfoEx;
    MS_U16 u16Vtt;
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode != E_XC_PNL_CHG_VTOTAL)
    {
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return bReturn;
    }
    if(u16OutputVfreq == pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq)
    {
        u16Vtt = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;
    }
    else
    {
        u16Vtt = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal*pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq/u16OutputVfreq;
    }
    memset(&stPanelInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
    stPanelInfoEx.bVSyncBackPorchValid = TRUE;
    stPanelInfoEx.bVSyncWidthValid     = TRUE;
    stPanelInfoEx.u16VSyncBackPorch = u16Vtt - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - u16FrontPorch - u16SyncWidth;
    stPanelInfoEx.u16VSyncWidth = u16SyncWidth;
    stPanelInfoEx.u16VFreq = u16OutputVfreq;
    stPanelInfoEx.u32PanelInfoEx_Version = PANEL_INFO_EX_VERSION;
    stPanelInfoEx.u16PanelInfoEX_Length = sizeof(XC_PANEL_INFO_EX);

    bReturn = MDrv_XC_SetExPanelInfo(pInstance, TRUE, &stPanelInfoEx);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_Set_CustomerSyncInfo(MS_U16 u16FrontPorch, MS_U16 u16SyncWidth, MS_U16 u16OutputVfreq)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_CUSTOMER_SYNC_INFO XCArgs;
    XCArgs.u16FrontPorch = u16FrontPorch;
    XCArgs.u16SyncWidth = u16SyncWidth;
    XCArgs.u16OutputVfreq = u16OutputVfreq;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CUSTOMER_SYNC_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

/*
    Only for adjusting.
    E_XC_PNL_CHG_DCLK,
    E_XC_PNL_CHG_HTOTAL,
    E_XC_PNL_CHG_VTOTAL
    Adjust different type of output timing paramete base on Step/Delay and Ratio to u16Target_Value
*/
#define MIN_STEP_VALUE                  10
#define MIN_DIFF_VALUE_FOR_STEPCHANGE   50
static E_APIXC_ReturnValue _MDrv_XC_SetHttVtt_Steply(void *pInstance, EN_OUTPUT_TIMING_ADJUST_MODE AdjustMode,MS_U16 u16Target_Value,
                                                     MS_U16 u16UserStep, MS_U16 u16UserDelay)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    //follow HW_DESIGN_4K2K_VER = 6 rule
    if (psXCInstPri->u32DeviceID != 0)
    {
        return E_APIXC_RET_OK;
    }
#endif
    MS_U16 u16Step, u16Temp, u16Old_Value = 0;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetHttVtt Steply: Mode=%u, Value=%u, Step=%u, Delay=%u\n", AdjustMode, u16Target_Value, u16UserStep, u16UserDelay)
    if (u16UserStep > 1 && AdjustMode != EN_OUTPUT_TIMING_CHG_DCLK)
    {
        if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
        {
#if defined(PATCH_HW_VTT_LIMITATION)
            if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
            {
                u16Old_Value = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
            }
            else
#endif
            {
                u16Old_Value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L)+1;
            }
        }
        else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
        {
            u16Old_Value = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L)+1;
        }
        else
        {
            u16Old_Value = 0;
        }
        u16Temp = abs(u16Target_Value - u16Old_Value);
        u16Step = (u16Temp + u16UserStep - 1)/u16UserStep;
        if(u16Temp < MIN_DIFF_VALUE_FOR_STEPCHANGE)
        {
            //If the difference or stepvalue is too small, take it as one step to save time
            //Add this to prevent 60HZ FBL to 60HZ FB step change
            u16Step = u16Temp;
            u16UserStep = 1;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Step start from %u\n", u16Old_Value)

        if ( u16Old_Value >= u16Target_Value )
        {
            while(u16Old_Value > u16Target_Value + u16Step)
            {
                u16Old_Value -= u16Step;
                if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
                {
                    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
                    {
#if defined(PATCH_HW_VTT_LIMITATION)
                        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
                        {
                            _MLOAD_ENTRY(pInstance);
                            MHal_XC_SetVopVttByBK68(pInstance, (u16Old_Value - 1)&VOP_VTT_MASK, 1);
                            _MLOAD_RETURN(pInstance);
                            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                        }
                        else
#endif
                        {
                            _MLOAD_ENTRY(pInstance);
                            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Old_Value - 1)&VOP_VTT_MASK, VOP_VTT_MASK  );
                            MDrv_XC_MLoad_Fire(pInstance, TRUE);
                            _MLOAD_RETURN(pInstance);
                        }
                    }
                    else
                    {
#if defined(PATCH_HW_VTT_LIMITATION)
                        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
                        {
                            MHal_XC_SetVopVttByBK68(pInstance, (u16Old_Value - 1)&VOP_VTT_MASK, 0);
                            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                        }
                        else
#endif
                        {
                            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16Old_Value - 1)&VOP_VTT_MASK);
                        }
                    }
                }
                else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
                {
                    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
                    {
                        _MLOAD_ENTRY(pInstance);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16Old_Value - 1), VOP_HTT_MASK  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN(pInstance);
                    }
                    else
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,(u16Old_Value - 1));
                    }
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, " -> %u", (u16Old_Value - 1));
                MsOS_DelayTask(u16UserDelay);
            }
        }
        else
        {
            while(u16Old_Value < u16Target_Value - u16Step)
            {
                u16Old_Value += u16Step;
                if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
                {
                    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
                    {
#if defined(PATCH_HW_VTT_LIMITATION)
                        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
                        {
                            _MLOAD_ENTRY(pInstance);
                            MHal_XC_SetVopVttByBK68(pInstance, (u16Old_Value - 1)&VOP_VTT_MASK, 1);
                            _MLOAD_RETURN(pInstance);
                            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                        }
                        else
#endif
                        {
                            _MLOAD_ENTRY(pInstance);
                            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16Old_Value - 1)&VOP_VTT_MASK, VOP_VTT_MASK  );
                            MDrv_XC_MLoad_Fire(pInstance, TRUE);
                            _MLOAD_RETURN(pInstance);
                        }
                    }
                    else
                    {
#if defined(PATCH_HW_VTT_LIMITATION)
                        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
                        {
                            MHal_XC_SetVopVttByBK68(pInstance, (u16Old_Value - 1)&VOP_VTT_MASK, 0);
                            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                        }
                        else
#endif
                        {
                            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16Old_Value - 1)&VOP_VTT_MASK);
                        }
                    }
                }
                else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
                {
                    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
                    {
                        _MLOAD_ENTRY(pInstance);
                        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16Old_Value - 1), VOP_HTT_MASK  );
                        MDrv_XC_MLoad_Fire(pInstance, TRUE);
                        _MLOAD_RETURN(pInstance);
                    }
                    else
                    {
                        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,(u16Old_Value - 1));
                    }
                }
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, " -> %u", (u16Old_Value - 1));
                MsOS_DelayTask(u16UserDelay);
            }
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, " -> %u\n", (u16Target_Value-1));
    if (AdjustMode==EN_OUTPUT_TIMING_VTOTAL)
    {
        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
        {
#if defined(PATCH_HW_VTT_LIMITATION)
            if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
            {
                _MLOAD_ENTRY(pInstance);

#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
                {
                    MS_U8 u8delaycnt = 1;
                    MS_U16 u16delaytime =0;

                    if(u16Target_Value > 2500)
                        u16delaytime = 1000*u8delaycnt/50;
                    else
                        u16delaytime = 1000*u8delaycnt/60;

                    // Update changed VTT to inform Raptor
                    MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                    MOD_W2BYTEMSK(REG_MOD_BK00_65_L,u16Target_Value,0x1FFF);
                    MsOS_DelayTask(u16delaytime);

                    MHal_XC_SetVopVttByBK68(pInstance, (u16Target_Value - 1)&VOP_VTT_MASK, 0);
                    _MLOAD_RETURN(pInstance);
                }
                else
#endif
                {
                    MHal_XC_SetVopVttByBK68(pInstance, (u16Target_Value - 1)&VOP_VTT_MASK, 1);
                    _MLOAD_RETURN(pInstance);
                    MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                }
            }
            else
#endif
            {
                _MLOAD_ENTRY(pInstance);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L, (u16Target_Value-1)&VOP_VTT_MASK, VOP_VTT_MASK);
                _MLOAD_RETURN(pInstance);
            }
        }
        else
        {
#if defined(PATCH_HW_VTT_LIMITATION)
                if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
                {
#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
                    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
                    {
                        // Update changed VTT to inform Raptor
                        MOD_W2BYTEMSK(REG_MOD_BK00_65_L,u16Target_Value,0x1FFF);
                        MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                        MHal_XC_SetVopVttByBK68(pInstance, (u16Target_Value - 1)&VOP_VTT_MASK, 0);
                    }
                    else
#endif
                    {
                        MHal_XC_SetVopVttByBK68(pInstance, (u16Target_Value - 1)&VOP_VTT_MASK, 0);
                        MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                    }
                }
                else
#endif
                {
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (u16Target_Value-1)&VOP_VTT_MASK);
                }
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(MDrv_XC_MLoad_GetStatus(g_pDevice1Instance) == E_MLOAD_ENABLED)
        {
            _MLOAD_ENTRY(g_pDevice1Instance);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_0D_L,VOP_VTT_MASK, VOP_VTT_MASK  );
            _MLOAD_RETURN(g_pDevice1Instance);
        }
        else
        {
            SC_W2BYTE(1, REG_SC_BK10_0D_L,VOP_VTT_MASK);
        }
#endif

    }
    else if (AdjustMode==EN_OUTPUT_TIMING_HTOTAL)
    {
        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
        {
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,u16Target_Value-1, VOP_HTT_MASK  );
            _MLOAD_RETURN(pInstance);
        }
        else
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,u16Target_Value-1);
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if(MDrv_XC_MLoad_GetStatus(g_pDevice1Instance) == E_MLOAD_ENABLED)
        {
            _MLOAD_ENTRY(g_pDevice1Instance);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_0C_L,(u16Target_Value/2 - 1), VOP_HTT_MASK  );
            _MLOAD_RETURN(g_pDevice1Instance);
        }
        else
        {
            SC_W2BYTE(1, REG_SC_BK10_0C_L,(u16Target_Value/2 - 1));
        }
#endif
    }
    else
    {
        //Todo: Adjust Dclk
        return E_APIXC_RET_FAIL;
    }

    return E_APIXC_RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// Set the FPLL mode:
/// @param  bTrue      \b IN: True: FSM mode, False: Non FSM mode
//-------------------------------------------------------------------------------------------------
void MDrv_XC_FPLLCusReset(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support for device id %td\n",__FUNCTION__,(ptrdiff_t)psXCInstPri->u32DeviceID);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MDrv_XC_FPLLCusReset() -> Not support for device id %td\n", (ptrdiff_t)psXCInstPri->u32DeviceID);
        return;
    }
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    int i;
    for (i = 0; i < E_FPLL_FLAG_MAX; i++)
    {
        pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[i]    = FALSE;
        pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[i]  = 0;
    }
}

//-------------------------------------------------------------------------------------------------
/// Set the custmer FPLL parameters(You can just call this function once when system init for testing)
/// @param  eMode:Enable/disable customer setting      \b IN: @ref EN_FPLL_MODE
/// @param  eFlag:The specified FPLL parameter type which to be modified      \b IN: @ref EN_FPLL_FLAG
/// @param  eFlag:The value to set      \b IN: @ref MS_U32
/// Note: the setting will take effect after running of function MApi_XC_SetPanelTiming
//-------------------------------------------------------------------------------------------------
void MApi_XC_FPLLCustomerMode_U2(void* pInstance, EN_FPLL_MODE eMode, EN_FPLL_FLAG eFlag, MS_U32 u32Data)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support for device id %td\n",__FUNCTION__,(ptrdiff_t)psXCInstPri->u32DeviceID);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_XC_EX_FPLLCustomerMode() -> Not support for device id %td\n", (ptrdiff_t)psXCInstPri->u32DeviceID);
        return;
    }

    if (eMode >= E_FPLL_MODE_MAX)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Wrong Fpll Mode selected!\n");
        return;
    }

    if (eFlag >= E_FPLL_FLAG_MAX)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Wrong Fpll Flag!\n");
        return;
    }
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"Fpll Customer Mode: eMode=0x%x, eFlag=0x%x, u32Data=0x%tx\n", eMode, eFlag, (ptrdiff_t)u32Data);
    if (eMode == E_FPLL_MODE_DISABLE_ALL)
    {
        MDrv_XC_FPLLCusReset(pInstance);
    }
    else if (eMode == E_FPLL_MODE_DISABLE)
    {
        pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[eFlag]      = FALSE;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[eFlag]      = TRUE;
        pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[eFlag]    = u32Data;
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_FPLLCustomerMode(EN_FPLL_MODE eMode, EN_FPLL_FLAG eFlag, MS_U32 u32Data)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_CUSTOMERMODE XCArgs;
    XCArgs.eMode = eMode;
    XCArgs.eFlag = eFlag;
    XCArgs.u32Data = u32Data;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_CUSTOMERMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//For debug purpose only
void MApi_XC_FPLLDbgMode(EN_FPLL_DBG_MODE DbgMode, EN_FPLL_DBG_FLAG DbgFlag, MS_U32 u32Data)
{
    MApi_XC_FPLLCustomerMode((EN_FPLL_MODE)DbgMode, (EN_FPLL_FLAG)DbgFlag, u32Data);
}

//-------------------------------------------------------------------------------------------------
// This function will force freerun in MDrv_Scaler_SetPanelTiming(),
// otherwise MDrv_Scaler_SetPanelTiming() will decide to do framelock or not by itself.
//
// Note: this function is difference with MDrv_SC_set_fpll() which is used to
//       unlock framelock after MDrv_Scaler_SetPanelTiming().
//       But this function is used to affect MDrv_Scaler_SetPanelTiming()'s behavior.
//-------------------------------------------------------------------------------------------------
void MApi_SC_ForceFreerun_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bForceFreerun = bEnable;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_SC_ForceFreerun = %u\n", bEnable);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

}

void MApi_SC_ForceFreerun(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FORCE_FREERUN XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FORCE_FREERUN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
// This function will return the force freerun state
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_SC_IsForceFreerun(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MDrv_SC_IsForceFreerun = %u\n", pXCResourcePrivate->stdrvXC_Display._bForceFreerun);
    return pXCResourcePrivate->stdrvXC_Display._bForceFreerun;
}

MS_BOOL MApi_SC_IsForceFreerun_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_BOOL bReturn = MDrv_SC_IsForceFreerun(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_SC_IsForceFreerun(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FORCE_FREERUN XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FORCE_FREERUN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_SC_SetFreerunVFreq_U2(void* pInstance, E_VFREQ_SEL VFreq)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING,"MApi_SC_SetFreerunVFreq = %u\n", VFreq);
    pXCResourcePrivate->stdrvXC_Display._ForceVFreq     = VFreq;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_SC_SetFreerunVFreq(E_VFREQ_SEL VFreq)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FREERUN_FREQ XCArgs;
    XCArgs.VFreq = VFreq;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FREERUN_FREQ, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16 MDrv_SC_VFreq_Select(void *pInstance, MS_U16 u16AutoVFreq)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_50HZ)
    {
        return 500;
    }
    else if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_60HZ)
    {
        return 600;
    }
    else if(pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_SRC)
    {
        return u16AutoVFreq;
    }
    else if (pXCResourcePrivate->stdrvXC_Display._ForceVFreq == VFREQ_FROM_PANEL)
    {
        return pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    }
    else
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_SC_VFreq_Select(): wrong Vfreq_SEL input, use default logic!!");
        return u16AutoVFreq;
    }
}
//-------------------------------------------------------------------------------------------------
// This function will Get the exact ext_panel info structure for input vfreq
// IN:
//    pPanelInfoEx: ext-panel info for return,
//                  if u16VFreq=0, pPanelInfoEx= next address to save new panel info
//                  else return the panel info pointer of "VFreq"
//    u16VFreq: the request vfreq for getting ext-panel info
// Return:
//    TRUE: Success, FALSE: FAIL
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_XC_GetExPanelInfo(void *pInstance, XC_PANEL_INFO_EX **ppPanelInfoEx, MS_U16 u16VFreq)
{
    MS_BOOL bRet = FALSE;
    MS_U8 i;
    *ppPanelInfoEx = NULL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16VFreq=%u\n", u16VFreq)

    for(i=0; i<PANEL_INFO_EX_NUM; i++)
    {
        if(u16VFreq == 0)
        {
            //Get next available place for store
            if(gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i].u32PanelInfoEx_Version == 0)
            {
                bRet = TRUE;
                *ppPanelInfoEx = &gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i];
                break;
            }
        }
        else
        {
            //Get ex panel info of specified "u16VFreq"
            if((gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i].u32PanelInfoEx_Version != 0) &&
               (abs(gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i].u16VFreq - u16VFreq) <= VFREQ_DIFF_TOLERANCE) )
            {
                bRet = TRUE;
                *ppPanelInfoEx = &gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx[i];
                break;
            }
        }
    }
    //XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Get ExPanelInfo u16VFreq=%u, pPanelInfoEx=0x%x, bRet=%u\n", u16VFreq, (MS_U32)*ppPanelInfoEx, bRet);
    return bRet;
}

//-------------------------------------------------------------------------------------------------
/// Set external panel info for output
/// Currently, only support setting of two different vfreqs at same time
/// @param  bEnable             \b IN: TRUE: Enable this XC_PANEL_INFO_EX, FALSE: Disable this XC_PANEL_INFO_EX
/// @param  pPanelInfoEx        \b IN: The extern panel information to set @ref XC_PANEL_INFO_EX
/// @return @ref MS_BOOL               TRUE: Success, FALSE: Failed
/// Note: the setting will take effect after running of function MApi_XC_SetPanelTiming
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_XC_SetExPanelInfo(void *pInstance, MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx)
{
    MS_BOOL bRet = TRUE;
    XC_PANEL_INFO_EX *pPnlInfoEx = NULL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(NULL == pPanelInfoEx || pPanelInfoEx->u32PanelInfoEx_Version < 2)
    {
        //We consider compatible operation from version2 , so reject the info init when version invalid
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MApi_XC_EnablePanelInfo: Version or pPanelInfoEx is Invalid!!")
        bRet = FALSE;
    }
    if(bRet)
    {
        MS_U16 u16VFreq = pPanelInfoEx->u16VFreq;
        MS_U16 u16Length = (pPanelInfoEx->u16PanelInfoEX_Length > sizeof(XC_PANEL_INFO_EX)) ? \
               sizeof(XC_PANEL_INFO_EX) : pPanelInfoEx->u16PanelInfoEX_Length;
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_XC_SetExPanelInfo: bEnable=%u, u16VFreq=%u\n", bEnable, u16VFreq)
        if(bEnable)
        {
            bRet = _MDrv_XC_GetExPanelInfo(pInstance, &pPnlInfoEx, 0);
            if(bRet && pPnlInfoEx != NULL)
            {
                //There are still space to store this info, so copy it
                memcpy(pPnlInfoEx, pPanelInfoEx, u16Length);
            }
        }
        else
        {
            if(u16VFreq == PANEL_INFO_EX_INVALID_ALL)
            {
                //Clear all info in ex panel info structure
                memset(gSrcInfo[MAIN_WINDOW].Status2.stPanelInfoEx, 0, (sizeof(XC_PANEL_INFO_EX))*PANEL_INFO_EX_NUM);
            }
            else
            {
                //Clear ex panel info structure of specified "VFreq"
                bRet = _MDrv_XC_GetExPanelInfo(pInstance, &pPnlInfoEx, u16VFreq);
                if(bRet && pPnlInfoEx != NULL)
                {
                    memset(pPnlInfoEx, 0, sizeof(XC_PANEL_INFO_EX));
                }
            }
        }
    }
    //XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MDrv_XC_SetExPanelInfo Exit: bRet=%u, pPnlInfoEx=%x\n", bRet, (MS_U32)pPnlInfoEx)
    return bRet;
}

MS_BOOL MApi_XC_SetExPanelInfo_U2(void* pInstance, MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_SetExPanelInfo(pInstance, bEnable, pPanelInfoEx);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}
MS_BOOL MApi_XC_SetExPanelInfo(MS_BOOL bEnable, XC_PANEL_INFO_EX *pPanelInfoEx)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_SET_EXPANEL_INFO XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.pPanelInfoEx = pPanelInfoEx;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_EXPANEL_INFO, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

static void _MDrv_Scaler_SetIGainPGain(void *pInstance, MS_U8 u8FRC_Out, MS_U16 u16FRC_VFreq, SCALER_WIN eWindow)
{
    //    MS_U8   i,u8Vco,u8LGain2 = 1,u8LGain1=1;
    MS_U8   u8Vco, u8LGain1=1;
    MS_U64  u64dclk=0;
    MS_U8   u8GainI, u8GainP;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16HTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    MS_U16 u16VTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;

    //check parameter valid, avoiding the diving zero failure
    if(0 == u8FRC_Out)
    {
        u8FRC_Out = 1;
    }
    //=================================================
    //  FPLL  I/P gain setting
    //
    //                        MM * 524288*loop_gain
    //  i_gain = ---------------------------
    //                   8 * Htt * Vtt * (Ovs + 1 ) * Vco
    //
    //  MM = MPLL / Xtal = 216 / 12(MHz) = 18
    //  loop_gain = 2^(reg_loop_gain_1st) * loop_gain_2nd
    //  loop_gain_2nd = loop_gain_2nd == 0 ? 1 : reg_loop_gain_2nd;
    //  Vco = LVDS == Dual mode ? 7 : 3.5;
    //  p_gain = i_gain *2;
    //  i_gain = 2^(reg_i_gain-5)
    //  p_gain = 2^(reg_p_gain-5)
    //  reg_loop_gain_1st = BK_0x3101[1:0]
    //  loop_gain_2nd       = BK_0x3101[15:8]
    //  Dual mode              = BK_0x3103[8]
    //  Ovs                         = BK_0x310C[15:12]
    //  reg_i_gain              = BK_0x310B[11:8]
    //  reg_p_gain              = BK_0x310B[15:12]
    //--------------------------------------------------------
    u64dclk = ((MS_U64)u16VTotal)*u16HTotal*u16FRC_VFreq;

    u8LGain1 = MHal_PNL_Get_LPLL_LoopGain(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode
                                          , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type
                                          , u64dclk);

    u8Vco = MHal_PNL_Get_Loop_DIV(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode
                                  , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type
                                  , u64dclk);

    // MM = MPLL / Xtal = (LVDS_MPLL_CLOCK_MHZ*1000000UL) /g_XC_InitData.u32XTAL_Clock

    MHal_XC_Calc_IGainPGain(pInstance, &u8GainI
                           , &u8GainP
                           , pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock
                           , u8LGain1, u8Vco, u16HTotal, u16VTotal, u8FRC_Out);

#ifdef UFO_XC_FB_LEVEL
    if(((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
             || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
           && !gSrcInfo[eWindow].Status2.bMVOPHSKMode
    )
#else
    if ((gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL) && !gSrcInfo[eWindow].Status2.bMVOPHSKMode)
#endif
    {
        u8GainP += 2;
        u8GainI += 2;
    }

    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled)
    {
        if(IsVBY1_16LANE((MS_U8)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)))
        {
        }
        else
        {
            u8GainP += 2;
            u8GainI += 2;
        }
    }
    #endif

    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_IGAIN])
    {
        u8GainI = (MS_U8)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_IGAIN];
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "user forces REG_IGain= 0x%X (%u)\n", u8GainI, u8GainI);
    }
    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PGAIN])
    {
        u8GainP = (MS_U8)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PGAIN];
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "user forces REG_PGain= 0x%X (%u)\n", u8GainP, u8GainP);
    }

    LPLL_BK_STORE;
    #if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case, adjust IGain PGain to coordinate HW timing change limit
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        MDrv_WriteByte(H_BK_LPLL(0x0B), 0x54);  //Gain
    else
        MDrv_WriteByte(H_BK_LPLL(0x0B), (u8GainP<<4)|u8GainI);  //Gain
    #else
        MDrv_WriteByte(H_BK_LPLL(0x0B), (u8GainP<<4)|u8GainI);  //Gain
    #endif
    LPLL_BK_RESTORE;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "REG_IGain= 0x%X (%u)\n", u8GainI, u8GainI)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "REG_PGain= 0x%X (%u)\n", u8GainP, u8GainP)
}
//-------------------------------------------------------------------------------------------------
// This function will Set the H or V sync information: SyncStart/end/width
// IN:
//    H/V Sync infomation and H/V output total that corresponding to this sync info.
// Return:
//    NONE
//-------------------------------------------------------------------------------------------------
static void _MDrv_XC_SetHVSync(void *pInstance, MS_BOOL bHSync, MS_U16 u16SyncWidth, MS_U16 u16SyncBackPorch, MS_U16 u16HVTotal)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //Change to big one: Change Vtt first to avoid changed vsync end > old vtt
    MS_U16 u16Temp;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "bHSync=%u, SyncWidth=%u, SyncBackPorch=%u, Total=%u\n",
                 bHSync, u16SyncWidth, u16SyncBackPorch, u16HVTotal)
    if(bHSync)
    {
        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
        {
            _MLOAD_ENTRY(pInstance);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if (psXCInstPri->u32DeviceID == 0)
#endif
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);  // output HDE start
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_04_L, (u16SyncWidth + u16SyncBackPorch)/2, VOP_DE_HSTART_MASK);  // output HDE start
#endif
            }
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);    // output Hsync Width
            _MLOAD_RETURN(pInstance);
        }
        else
        {
            u16Temp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, 0xFF);//Old Hsync Width
            if(u16Temp > u16SyncWidth-1)
            {
                //Change to small one
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);  // output HDE start
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                if (psXCInstPri->u32DeviceID == 0)
#endif
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                    SC_W2BYTEMSK(1, REG_SC_BK10_04_L, (u16SyncWidth + u16SyncBackPorch)/2, VOP_DE_HSTART_MASK);
#endif
                }
            }
            else
            {
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                if (psXCInstPri->u32DeviceID == 0)
#endif
                {
                    //Change to big one
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, u16SyncWidth + u16SyncBackPorch, VOP_DE_HSTART_MASK);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
                    SC_W2BYTEMSK(1, REG_SC_BK10_04_L, (u16SyncWidth + u16SyncBackPorch)/2, VOP_DE_HSTART_MASK);
#endif
                }
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, u16SyncWidth-1, 0xFF);
            }
        }
    }
    else
    {
        #if FRC_INSIDE
        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            u16HVTotal = u16HVTotal/4;
            u16SyncBackPorch = u16SyncBackPorch/4;
        }
        #endif

        MS_U16 u16NewVsyncEnd = u16HVTotal-u16SyncBackPorch;
        MS_U16 u16VsyncStart = u16NewVsyncEnd - u16SyncWidth;

#ifdef PATCH_TCON_BRING_UP
        if((((MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type) == E_XC_PNL_LPLL_EPI28_12P)
          && !(strcmp(panelName_UD_VB1_8LANE_DRDEPI,MDrv_PNL_GetName())))
        {
            u16NewVsyncEnd = VSYNC_END_FOR_UD_VB1_8LANE_DRDEPI;
            u16VsyncStart = VSYNC_START_FOR_UD_VB1_8LANE_DRDEPI;
        }
#endif

        MS_U16 u16NewVsyncStart = (u16VsyncStart & 0x7FF) | ((u16VsyncStart & 0x800) << 1);

#if 0
        // Raptor case, swap position of Vsync and DE
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        {
            u16VsyncStart = 1;
            u16NewVsyncEnd = u16VsyncStart + u16SyncWidth;
            u16NewVsyncStart = (u16VsyncStart & 0x7FF) | ((u16VsyncStart & 0x800) << 1);
        }
#endif

        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
        {
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_03_L, u16NewVsyncEnd-1, VOP_VSYNC_END_MASK);    // output Vsync end
            _MLOAD_RETURN(pInstance);
        }
        else
        {
            u16Temp = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK);//Old Vsync End
            if(u16Temp > u16NewVsyncEnd-1)
            {
                //Change to small one
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, u16NewVsyncEnd-1);
            }
            else
            {
                //Change to big one
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, u16NewVsyncEnd-1);
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, u16NewVsyncStart, 0x17FF);  // output Vsync start
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
// This function will Enable/Disable manual Vsync Mode
// IN:
//    bManualMode: TRUE---Manual Mode, False---Auto Mode
// Return:
//    NONE
//-------------------------------------------------------------------------------------------------
static void _MDrv_XC_SetOutputVSyncMode(void *pInstance, MS_BOOL bManualMode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    //Change to big one: Change Vtt first to avoid changed vsync end > old vtt
    MS_U16 u16OldVSyncMode = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L) & BIT(15);
    if((((MS_U16)bManualMode)<<15) !=  u16OldVSyncMode)
    {
        u16OldVSyncMode = (((MS_U16)bManualMode)<<15);
        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
        {
            _MLOAD_ENTRY(pInstance);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_10_L, u16OldVSyncMode, BIT(15));  // output Vsync start
            _MLOAD_RETURN(pInstance);
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, u16OldVSyncMode, BIT(15));    //Manual output Vsync
        }
    }
}

void MDrv_SC_set_Htt_Vtt(void *pInstance, XC_SetTiming_Info *pTimingInfo, MS_U16 *pu16VFreq,MS_U32 u32HighAccurateInputVFreq, XC_DISPLAYOUT_INFO *pDisplyout_Info, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16OldHtt, u16OldVtt, u16OPHtt,u16VsyncStart, u16VsyncEnd = 0;
    MS_U16 u16HSyncWidth, u16HBackPorch, u16VSyncWidth, u16VBackPorch = 0;
    MS_BOOL bManualVsyncMode = FALSE;
    MS_U16 u16ChangeHttSteps=1,u16ChangeHttDelay=0,u16ChangeVttSteps=CHANGE_VTT_STEPS,u16ChangeVttDelay=CHANGE_VTT_DELAY;
    XC_PANEL_INFO_EX *pPanelInfoEx = NULL;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pDisplyout_Info->u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal;
    pDisplyout_Info->u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal;

    MS_U16 u16DEVStart = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart;
    MS_U16 u16DEVEnd =  u16DEVStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER < 6))
    if(((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ))
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1280)
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height== 720))
    {
        pDisplyout_Info->u16Htt_out = 2200;
        pDisplyout_Info->u16Vtt_out = 1130;
    }
#endif

    u16HSyncWidth = pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth;
    u16HBackPorch = pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch;
    u16VSyncWidth = pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth;
    u16VBackPorch = pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch;

#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
    {
        u16OldVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
    }
    else
#endif
    {
        u16OldVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK) + 1;
    }
    u16OldHtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, VOP_HTT_MASK) + 1;
    u16OPHtt = pDisplyout_Info->u16Htt_out;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Default SyncWidth/back porch: H=%u,%u,V=%u,%u, Old Htt/Vtt=%u,%u\n"
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch
                                      , u16OldHtt, u16OldVtt)
    if(_MDrv_XC_GetExPanelInfo(pInstance, &pPanelInfoEx, *pu16VFreq) /*&& pPanelInfoEx != NULL*/)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "PanelInfoEx assigned\n")
        if(pPanelInfoEx->bHttDelayValid)
        {
            u16ChangeHttDelay = pPanelInfoEx->u16HttDelay;
        }
        if(pPanelInfoEx->bHttStepsValid)
        {
            u16ChangeHttSteps = pPanelInfoEx->u16HttSteps;
        }
        if(pPanelInfoEx->bVttDelayValid)
        {
            u16ChangeVttDelay = pPanelInfoEx->u16VttDelay;
        }
        if(pPanelInfoEx->bVttStepsValid)
        {
            u16ChangeVttSteps = pPanelInfoEx->u16VttSteps;
        }
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "PanelInfoEx: %s, org htt=%u, vtt=%u, vfreq=%u; HighAccurateVFreq=%u; Output Vfreq=%u, Mode=%u\n", (pPanelInfoEx==NULL) ? "NO": "YES",
                 pDisplyout_Info->u16Htt_out, pDisplyout_Info->u16Vtt_out,
                 pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                 *pu16VFreq,(unsigned int)u32HighAccurateInputVFreq, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode)

#ifdef UFO_XC_FB_LEVEL
    if(((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
         || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
       && !gSrcInfo[eWindow].Status2.bMVOPHSKMode
    )
#else
    if((gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL) && !gSrcInfo[eWindow].Status2.bMVOPHSKMode)
#endif
    {
        if(!(pXCResourcePrivate->stdrvXC_Display._bForceFreerun
            && (pXCResourcePrivate->stdrvXC_Display._ForceVFreq != VFREQ_FROM_SRC)))
        {
            //Not Force Free Run@Fixed 50/60 hz, need adjust vtt
            //calculate Vtt_out for FBL, Vtt_out = Vtt_in * Vde_out / Vde_in
            pDisplyout_Info->u16Vtt_out = MDrv_SC_FBL_Calc_Vtt(pInstance, pTimingInfo, eWindow);
        }
    }

    switch(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode)
    {
        case E_XC_PNL_CHG_HTOTAL:
            if((pPanelInfoEx == NULL) ||
               (pPanelInfoEx->bHTotalValid == FALSE))
            {
                //Change htt, all case can do it
                MDrv_SC_adjust_output_htt_vtt(&pDisplyout_Info->u16Htt_out, &pDisplyout_Info->u16Vtt_out,u32HighAccurateInputVFreq,
                            pDisplyout_Info->u16Htt_out,
                            pDisplyout_Info->u16Vtt_out,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                            FALSE);
            }
            break;
        case E_XC_PNL_CHG_VTOTAL:
            if((pPanelInfoEx == NULL) ||
               (pPanelInfoEx->bVTotalValid == FALSE))
            {
#ifdef UFO_XC_FB_LEVEL
                if(((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)
                     || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
                   && !gSrcInfo[eWindow].Status2.bMVOPHSKMode
                )
#else
                if((gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bR_FBL) && !gSrcInfo[eWindow].Status2.bMVOPHSKMode)
#endif
                {
                    //FBL+FPLL or FBL+FreeRun@Src can not adjust vtt
                    if(pXCResourcePrivate->stdrvXC_Display._bForceFreerun
                        && (pXCResourcePrivate->stdrvXC_Display._ForceVFreq != VFREQ_FROM_SRC))
                    {
                        MDrv_SC_adjust_output_htt_vtt(&pDisplyout_Info->u16Htt_out, &pDisplyout_Info->u16Vtt_out,u32HighAccurateInputVFreq,
                                    pDisplyout_Info->u16Htt_out,
                                    pDisplyout_Info->u16Vtt_out,
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                    TRUE);
                    }

                }
                else
                {
                    //Not FBL case, can adjust vtt
                    MDrv_SC_adjust_output_htt_vtt(&pDisplyout_Info->u16Htt_out, &pDisplyout_Info->u16Vtt_out,u32HighAccurateInputVFreq,
                                pDisplyout_Info->u16Htt_out,
                                pDisplyout_Info->u16Vtt_out,
                                pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                TRUE);
                }
            }
            break;
        case E_XC_PNL_CHG_DCLK:
        default:
            break;
    }

    if(pPanelInfoEx != NULL)
    {
        MS_BOOL bExceeded = FALSE;
        if(!pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@@ Set Htt/Vtt By Ex panel\n\n");
            if(pPanelInfoEx->bVTotalValid)
            {
                pDisplyout_Info->u16Vtt_out = pPanelInfoEx->u16VTotal;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "11: u16Vtt_out=%u\n",pDisplyout_Info->u16Vtt_out);
            }

            if(pPanelInfoEx->bHTotalValid)
            {
                pDisplyout_Info->u16Htt_out = pPanelInfoEx->u16HTotal;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "22: u16Htt_out=%u\n",pDisplyout_Info->u16Htt_out);
            }
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "33: u16MaxVTotal = %u, u16MinVTotal = %u, u16MaxHTotal = %u, u16MinHTotal = %u\n",
            pPanelInfoEx->u16MaxVTotal, pPanelInfoEx->u16MinVTotal,
            pPanelInfoEx->u16MaxHTotal, pPanelInfoEx->u16MinHTotal);

            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "44: bMaxVTotalValid = %u, bMinVTotalValid = %u, bMaxHTotalValid = %u, bMinHTotalValid = %u\n",
            pPanelInfoEx->bMaxVTotalValid, pPanelInfoEx->bMinVTotalValid,
            pPanelInfoEx->bMaxHTotalValid, pPanelInfoEx->bMinHTotalValid);


            if(pPanelInfoEx->bMaxVTotalValid && (pDisplyout_Info->u16Vtt_out > pPanelInfoEx->u16MaxVTotal))
            {
                pDisplyout_Info->u16Vtt_out = pPanelInfoEx->u16MaxVTotal;
                bExceeded = TRUE;
            }

            if(pPanelInfoEx->bMinVTotalValid && (pDisplyout_Info->u16Vtt_out < pPanelInfoEx->u16MinVTotal))
            {
                pDisplyout_Info->u16Vtt_out = pPanelInfoEx->u16MinVTotal;
                bExceeded = TRUE;
            }

            if(pPanelInfoEx->bMaxHTotalValid && (pDisplyout_Info->u16Htt_out > pPanelInfoEx->u16MaxHTotal))
            {
                pDisplyout_Info->u16Htt_out = pPanelInfoEx->u16MaxHTotal;
                bExceeded = TRUE;
            }

            if(pPanelInfoEx->bMinHTotalValid && (pDisplyout_Info->u16Htt_out < pPanelInfoEx->u16MinHTotal))
            {
                pDisplyout_Info->u16Htt_out = pPanelInfoEx->u16MinHTotal;
                bExceeded = TRUE;
            }

            if(bExceeded)
            {
                printf("ALERT: HTotal or VTotal is out of range, Set to default timing and freerun !!!!!!\n");
                pDisplyout_Info->u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal;
                pDisplyout_Info->u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal;
                *pu16VFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
                MDrv_XC_SetFreeRunTiming(pInstance);
            }
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "new htt=%u, vtt=%u, vfreq=%u\n", pDisplyout_Info->u16Htt_out, pDisplyout_Info->u16Vtt_out, *pu16VFreq);
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal = pDisplyout_Info->u16Htt_out;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal = pDisplyout_Info->u16Vtt_out;

    if(pPanelInfoEx != NULL)
    {
        if(pPanelInfoEx->bHSyncBackPorchValid ||
           pPanelInfoEx->bHSyncWidthValid)
        {
            pXCResourcePrivate->stdrvXC_Display._bHSyncChanged = TRUE;
            if(pPanelInfoEx->bHSyncBackPorchValid)
            {
                u16HBackPorch = pPanelInfoEx->u16HSyncBackPorch;
            }
            if(pPanelInfoEx->bHSyncWidthValid)
            {
                u16HSyncWidth = pPanelInfoEx->u16HSyncWidth;
            }
        }
        if(pPanelInfoEx->bVSyncBackPorchValid ||
           pPanelInfoEx->bVSyncWidthValid)
        {
            pXCResourcePrivate->stdrvXC_Display._bVSyncChanged = TRUE;
            if(pPanelInfoEx->bVSyncBackPorchValid)
            {
                u16VBackPorch = pPanelInfoEx->u16VSyncBackPorch;
            }
            if(pPanelInfoEx->bVSyncWidthValid)
            {
                u16VSyncWidth = pPanelInfoEx->u16VSyncWidth;
            }
        }
    }
#ifndef PATCH_HW_VTT_LIMITATION
    // for high accurate input VFreq case, to avoid reconfig Vtt/Htt
    if(abs((MS_S16)pDisplyout_Info->u16Vtt_out - (MS_S16)u16OldVtt) >= CHANGE_VTT_TOLERANCE) // fixme: mantis 1216841
#endif
    {
        //if Vsync in DE zone then move Vsync to out of DE
        //so adjust BackPorch
        MS_BOOL bVsynInDE = FALSE;
        u16VsyncEnd = pDisplyout_Info->u16Vtt_out - u16VBackPorch;
        u16VsyncStart = u16VsyncEnd - u16VSyncWidth;
        if(u16VsyncStart < u16DEVEnd)
        {
            while(u16VsyncStart < u16DEVEnd)
            {
                u16VBackPorch = u16VBackPorch / 2;
                if(u16VBackPorch <= MINMUN_V_BACKPORCH)
                {
                    u16VBackPorch = MINMUN_V_BACKPORCH;
                    break;
                }
                u16VsyncEnd = pDisplyout_Info->u16Vtt_out - u16VBackPorch;
                u16VsyncStart = u16VsyncEnd - u16VSyncWidth;
            }
            bVsynInDE = TRUE;
        }

        if(pDisplyout_Info->u16Vtt_out > u16OldVtt)
        {
            //Change to big one: Change Vtt first to avoid changed Vsync end > old Vtt
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_VTOTAL, pDisplyout_Info->u16Vtt_out, u16ChangeVttSteps, u16ChangeVttDelay);
            if(pXCResourcePrivate->stdrvXC_Display._bVSyncChanged || bVsynInDE)
            {
                _MDrv_XC_SetHVSync(pInstance, FALSE, u16VSyncWidth, u16VBackPorch, pDisplyout_Info->u16Vtt_out);
            }
        }
        else
        {
            if(pXCResourcePrivate->stdrvXC_Display._bVSyncChanged || bVsynInDE)
            {
                _MDrv_XC_SetHVSync(pInstance, FALSE, u16VSyncWidth, u16VBackPorch, pDisplyout_Info->u16Vtt_out);
            }
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_VTOTAL, pDisplyout_Info->u16Vtt_out, u16ChangeVttSteps, u16ChangeVttDelay);
        }

    }

    u16OPHtt = pDisplyout_Info->u16Htt_out;  //mantis 1100871  u16OPHtt did not update after HTT changed
    #if FRC_INSIDE
    if ((pDisplyout_Info->u16Htt_out > 3000) && (pDisplyout_Info->u16Vtt_out > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
        {
            // FHD 50/60 in case, OP timing is 2K2K, Htt is 2K
            u16OPHtt = u16OPHtt/2;
        }
    }
    #endif

    // for high accurate input VFreq case, to avoid reconfig Vtt/Htt
    if(abs((MS_S16)u16OPHtt - (MS_S16)u16OldHtt) > CHANGE_HTT_TOLERANCE)
    {
        if(u16OPHtt > u16OldHtt)
        {
            //Change to big one: Change Htt first to avoid changed Hsync end > old Htt
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_HTOTAL, u16OPHtt, u16ChangeHttSteps, u16ChangeHttDelay);
            if(pXCResourcePrivate->stdrvXC_Display._bHSyncChanged)
            {
                _MDrv_XC_SetHVSync(pInstance, TRUE, u16HSyncWidth, u16HBackPorch, pDisplyout_Info->u16Htt_out);
            }
        }
        else
        {
            if(pXCResourcePrivate->stdrvXC_Display._bHSyncChanged)
            {
                _MDrv_XC_SetHVSync(pInstance, TRUE, u16HSyncWidth, u16HBackPorch, pDisplyout_Info->u16Htt_out);
            }
            _MDrv_XC_SetHttVtt_Steply(pInstance, EN_OUTPUT_TIMING_HTOTAL, u16OPHtt, u16ChangeHttSteps, u16ChangeHttDelay);
        }
    }
    if(pXCResourcePrivate->stdrvXC_Display._bHSyncChanged || pXCResourcePrivate->stdrvXC_Display._bVSyncChanged)
    {
        bManualVsyncMode = TRUE;
        _MDrv_XC_SetOutputVSyncMode(pInstance, TRUE);
    }
    else
    {
        bManualVsyncMode = FALSE;
        _MDrv_XC_SetOutputVSyncMode(pInstance, FALSE);
    }
#if (PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB == 0)
    if ((!bManualVsyncMode) && (gSrcInfo[SUB_WINDOW].enInputSourceType == INPUT_SOURCE_SCALER_OP))//Temp solution: DE mode not change it
    {
        //patch for DIP capture in DIP goes with sub_ip/sub_op case, the sub capture window x/y need adjust if OP vtt changes
        MS_S16 s16VttDiff = u16OldVtt - pDisplyout_Info->u16Vtt_out;
        MS_S16 s16HttDiff = u16OldHtt - pDisplyout_Info->u16Htt_out;
        if((MS_S16)(gSrcInfo[SUB_WINDOW].stCapWin.y) >= s16VttDiff)
        {
            gSrcInfo[SUB_WINDOW].stCapWin.y -= s16VttDiff;
        }
        if((MS_S16)(gSrcInfo[SUB_WINDOW].stCapWin.x) >= s16HttDiff)
        {
            gSrcInfo[SUB_WINDOW].stCapWin.x -= s16HttDiff;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Change CapWin for source OP x/y=%u,%u, H/V Diff=%d,%d\n",
                     gSrcInfo[SUB_WINDOW].stCapWin.x, gSrcInfo[SUB_WINDOW].stCapWin.y, s16HttDiff, s16VttDiff)
        MDrv_XC_SetCaptureWindow(pInstance, &gSrcInfo[SUB_WINDOW].stCapWin,SUB_WINDOW, FALSE);
    }
#endif
    if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
    {
        _MLOAD_ENTRY(pInstance);
        if (!MDrv_XC_MLoad_Fire(pInstance, TRUE))
        {
            printf("Set Htt/vtt ML Fire Error!!!!\n ");
        }
        _MLOAD_RETURN(pInstance);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Htt_out, Vtt_out= %u , %u\n", pDisplyout_Info->u16Htt_out, pDisplyout_Info->u16Vtt_out)

#if (FRC_INSIDE)
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_HTOTAL )
    {
        MDrv_FRC_TGEN_SetHTotal(pInstance, pDisplyout_Info->u16Htt_out);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Set Htt_out to TGEN, Htt_out= %u \n", pDisplyout_Info->u16Htt_out)
    }
    else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_VTOTAL )
    {
        if(pDisplyout_Info->u16Vtt_out < (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height+42))
        {
            pDisplyout_Info->u16Vtt_out = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height+42);
        }
        MDrv_FRC_TGEN_SetVTotal(pInstance, pDisplyout_Info->u16Vtt_out);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Set Vtt_out to TGEN, Vtt_out= %u \n", pDisplyout_Info->u16Vtt_out)
    }

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
    {
        if ((*pu16VFreq>=2380) && (*pu16VFreq<=2420))
        {
            MDrv_FRC_TGEN_SetVTotal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2);
        }
        else if ((*pu16VFreq>=1980) && (*pu16VFreq<=2020))
        {
            MDrv_FRC_TGEN_SetVTotal(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*60/50/2);
        }
    }
    else
    {
        MDrv_FRC_TGEN_SetVTotal(pInstance, pDisplyout_Info->u16Vtt_out);
    }

    // 4K2K patch
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))

    MST_PANEL_INFO_t stFRCPanelInfo;
    //XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@@ 4K2K panel patch for FRC TGEN\n");

    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.u8PanelHSyncWidth = pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.u8PanelHSyncBackPorch = pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.u8PanelVSyncWidth = pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth;
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.u8PanelVSyncBackPorch = pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch;
    }

    MDrv_FRC_PNLInfo_Transform(pInstance, &pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo,&pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc, &stFRCPanelInfo);
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
    {
        pDisplyout_Info->u16Vtt_out = stFRCPanelInfo.u16VTotal;
    }
    MDrv_FRC_Tx_SetTgen(pInstance, &stFRCPanelInfo);
#endif

#endif
}

static void _MDrv_XC_Set_FPLL_Thresh_Mode(void)
{
#if FPLL_THRESH_MODE_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

#ifdef UFO_XC_FB_LEVEL
    if(pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && ((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FB)
         || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)
       && gSrcInfo[MAIN_WINDOW].bEnableFPLL
    )
#else
    if (pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && ( (!(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) ) || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode) && gSrcInfo[MAIN_WINDOW].bEnableFPLL)
#endif
    {
        MDrv_Write2Byte(L_BK_LPLL(0x1F), 0x2EE0);
        MDrv_WriteRegBit(H_BK_LPLL(0x78), ENABLE, BIT(4));
    }
    else
    {
        MDrv_Write2Byte(L_BK_LPLL(0x1F), 0x0080);
        MDrv_WriteRegBit(H_BK_LPLL(0x78), DISABLE, BIT(4));
    }
    LPLL_BK_RESTORE;
#endif
}

static void _MDrv_XC_PQ_Set_FPLL_Thresh_Mode(void *pInstance)
{
#if FPLL_THRESH_MODE_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#ifdef UFO_XC_FB_LEVEL
        if(pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
            && ((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FB)
             || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)
           && gSrcInfo[MAIN_WINDOW].bEnableFPLL
        )
#else
    if (pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && ( (!(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) ) || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode) && gSrcInfo[MAIN_WINDOW].bEnableFPLL)
#endif
    {
        if (_fpPQSetFPLLThreshMode != NULL)
        {
            _fpPQSetFPLLThreshMode(MAIN_WINDOW);
            if (MDrv_XC_Is_SubWindowEanble(pInstance))
            {
                _fpPQSetFPLLThreshMode(SUB_WINDOW);
            }
        }
    }
#endif
}

void MApi_XC_SetOutTimingMode_U2(void* pInstance, E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetOutTimingMode(E_XC_PNL_OUT_TIMING_MODE enPnl_Out_Timing_Mode)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OUTPUT_TIMING_MODE XCArgs;
    XCArgs.enPnl_Out_Timing_Mode = enPnl_Out_Timing_Mode;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_TIMING_MODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_U16 MDrv_SC_FBL_Calc_Vtt(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16  u16Vde_in, u16Vde_out, u16Vtt_in, u16Vtt_out;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    u16Vde_in = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L) & 0x1FFF;

    if ((MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow)) && (u16Vde_in == 0x1FFF))
    {
        MS_WINDOW_TYPE stWinType;
        memset(&stWinType, 0, sizeof(MS_WINDOW_TYPE));
        MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(pInstance, eWindow, &stWinType);
        u16Vde_in = stWinType.height;
    }

    if(u16Vde_in == 0) // avoid divide by zero.
    {
        return 0;
    }
    // Remove storage Port later ( Driver level does not know what storagePort is )
    // Merge MVD port and Storage port
    if( pTimingInfo->bMVOPSrc)
    {
        u16Vtt_in = pTimingInfo->u16InputVTotal;//MDrv_Read2Byte(VOP_FRAME_VCOUNT) & 0x7FF;
    }
    else if(IsSrcTypeAnalog(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType))
    {
        u16Vtt_in = MDrv_XC_PCMonitor_Get_Vtotal(pInstance, eWindow);
    }
    else
    {
        u16Vtt_in = Hal_SC_ip_get_verticaltotal(pInstance, MAIN_WINDOW);//MDrv_Read2Byte(L_BK_IP1F2(0x1f)) & MST_V_TOTAL_MASK;
    }

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //u16Vde_out = (MDrv_Read2Byte(L_BK_VOP(0x0B))&0x7FF) - (MDrv_Read2Byte(L_BK_VOP(0x0A))&0x7FF) + 1;
    u16Vde_out = (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L)&VOP_DISPLAY_VEND_MASK) - (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L)&VOP_DISPLAY_VSTART_MASK) + 1;
    MS_ASSERT(u16Vde_in);

    u16Vtt_out = (MS_U16)(((MS_U32)u16Vtt_in * u16Vde_out + u16Vde_in/2) / u16Vde_in);

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Vde_in= 0x%X (%u)\n", u16Vde_in, u16Vde_in)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Vde_out= 0x%X (%u)\n", u16Vde_out, u16Vde_out)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Vtt_in= 0x%X (%u)\n", u16Vtt_in, u16Vtt_in)

    return u16Vtt_out;
}

void MDrv_SC_Set_LockFreeze_Point(void *pInstance, XC_SetTiming_Info *pTimingInfo, MS_U16 u16Vtt_out, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16  u16LockPoint = 0, u16FreezePoint = 0;
    MS_BOOL bForceReadBank = FALSE;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if( !gSrcInfo[eWindow].bFBL)
    {
#ifdef UFO_XC_FB_LEVEL
        if((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL)||(gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
#else
        if(gSrcInfo[eWindow].bR_FBL) //for M10 Reguest FBL mode, it doesn't the same with FBL
#endif
        {
            u16LockPoint = u16Vtt_out;
            u16FreezePoint = u16LockPoint - 2;

            // Setip Y lock
#if (HW_DESIGN_4K2K_VER == 6)
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x5, BITMASK(3:0));
#else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x2, BITMASK(3:0));
#endif

#if (HW_DESIGN_4K2K_VER == 4)
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_LEGACY_MODE)
            {
                gSrcInfo[eWindow].u8DelayLines = 8;
            }
            else
#endif
#if (HW_DESIGN_4K2K_VER == 6)

            // Setup delay line
            if((pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow])
                && (!pTimingInfo->bInterlace))
            {
                gSrcInfo[eWindow].u8DelayLines = 0x10;//rfbl+uc+pmode, need to adjust delay+1
            }
            else
            {
                gSrcInfo[eWindow].u8DelayLines = 0xF;
            }
#else
            // Setup delay line
            if((pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow])
                && (!pTimingInfo->bInterlace))
            {
                gSrcInfo[eWindow].u8DelayLines = 7;//rfbl+uc+pmode, need to adjust delay+1
            }
            else
            {
                gSrcInfo[eWindow].u8DelayLines = 6;
            }
#endif
            bForceReadBank = FALSE;
        }
        else
        {
            /* 2010-07-05
            * according to the discussion from designer, we have the following formula:
            * freeze point = vde_end (reg_value) + 1;
            * lock = vde_end(reg_value) + 3;
            */
            u16FreezePoint = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L) + 1;
            u16LockPoint = u16FreezePoint + 2;

            // no matter progressive or interlace
            // in FB mode we set to 0x5 (originally progressive = 0x3, interlace = 0x4)
            // but in Open HDR mode(with MVOP DE mode)
            // if we set to 3 or 4 it will be too late for autodownload to work.
            // so we change to 5 based upon CC's experiement suggestion.
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x0C, BITMASK(3:0));

            if((gSrcInfo[eWindow].eDeInterlaceMode == MS_DEINT_2DDI_AVG) ||
                (gSrcInfo[eWindow].eDeInterlaceMode == MS_DEINT_2DDI_BOB))
            {
                gSrcInfo[eWindow].u8DelayLines = 1;    // 5
            }
            else
            {
                gSrcInfo[eWindow].u8DelayLines = 2;
            }

            //if HV mode with capture size 0x1fff, we need to adjust delay line to 6.
            if ((MDrv_XC_GetHdmiSyncMode(pInstance) == HDMI_SYNC_HV) &&
                (IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType)) &&
                (!pTimingInfo->bInterlace))
            {
                MS_WINDOW_TYPE stWinType;
                memset(&stWinType, 0, sizeof(MS_WINDOW_TYPE));
                MDrv_XC_PCMonitor_Get_Dvi_Hdmi_De_Info(pInstance, eWindow, &stWinType);

                if(HVMODE_HDE_VALIDATION(stWinType.width))
                {
                    gSrcInfo[eWindow].u8DelayLines = 6;
                }
            }

            bForceReadBank = FALSE;
        }
        // Setup delay line
        Hal_SC_set_delayline(pInstance,  gSrcInfo[eWindow].u8DelayLines, eWindow );
    }
    else
    {
        //New method for lock point in FBL
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_09_L, 0x2, BITMASK(3:0));

        MS_U16 u16Vde_in = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_06_L) & 0x1FFF;

        if(pTimingInfo->bInterlace)
        {
            if (u16Vde_in <= 580)
            {
                gSrcInfo[eWindow].u8DelayLines = 6;
            }
            else
            {
                gSrcInfo[eWindow].u8DelayLines = 7;
            }
            bForceReadBank = FALSE;
        }
        else
        {
            MS_U32 u32VPSDRatio = 0;
            if (eWindow == MAIN_WINDOW)
            {
                u32VPSDRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_09_L) & 0xFFFFFF;
            }
            else
            {
                u32VPSDRatio = SC_R4BYTE(psXCInstPri->u32DeviceID, REG_SC_BK23_29_L) & 0xFFFFFF;
            }

            if((u32VPSDRatio & 0x3FFFFFL) > 0x100000)
            {
                //printf("---scaling down----\n");
                gSrcInfo[eWindow].u8DelayLines = DELAY_LINE_SC_DOWN;//8;
            }
            else
            {
                //printf("----scaling up----\n");
                gSrcInfo[eWindow].u8DelayLines = DELAY_LINE_SC_UP;// 7;
            }
            bForceReadBank = TRUE;
        }

        if ((MDrv_SC_Get_DE_Bypass_Mode(pInstance, eWindow)) && (u16Vde_in == 0x1FFF))
        {
            gSrcInfo[eWindow].u8DelayLines = 12;
        }

        Hal_SC_set_delayline(pInstance, gSrcInfo[eWindow].u8DelayLines, eWindow );

        //SC0F_55 = SC10_0D, SC0F_54 = SC0F_55 -2
        u16LockPoint = u16Vtt_out;
        u16FreezePoint = u16LockPoint - 2;
    }

    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_U16 u16IvsOvs = MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xFF00;
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled) && // FRC + 4K2K case needs to lock phase, FRC + FHD case use lockVfreq mode
        (IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) &&
        (u16IvsOvs == 0x0000))
    {
        u16LockPoint = u16LockPoint/2;
        u16FreezePoint = u16FreezePoint/2;
    }
    #endif

    // Only turn on when FBL (not RFBL) and the signal is progressive mode.
    // to force R bank to be at the first frame
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, (bForceReadBank? ((MS_U16)0x8)<<8 : 0), BITMASK(11:8));

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, BIT(1), BIT(1));  //Using new ovs_ref
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if (psXCInstPri->u32DeviceID == 0)
#endif
    {
#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        {
            MS_U32 u32VTT=SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_55_L, u32VTT/2);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_54_L, u32VTT/2-1);    //FreezePoint = LockPoint-2
        }
        else
#endif
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_55_L, (u16LockPoint-1)&0xFFF);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_54_L, (u16FreezePoint-1)&0xFFF);    //FreezePoint = LockPoint-2
        }
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "LockPoint= 0x%X (%u)\n", u16LockPoint, u16LockPoint)
}

void MApi_XC_SetLockPoint_U2(void* pInstance, MS_U16 u16LockPoint, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    UNUSED(eWindow);

    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_55_L, (u16LockPoint-1)&0x7FF);
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_54_L, (u16LockPoint-2)&0x7FF);    //FreezePoint = LockPoint-2

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetLockPoint(MS_U16 u16LockPoint, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_LOCK_POINT XCArgs;
    XCArgs.u16LockPoint = u16LockPoint;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_LOCK_POINT, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }

}

void MDrv_Scaler_SetPhaseLimit(void *pInstance, MS_U8 u8Lpll_bank)
{
    MS_U16 u16PhaseLimit = PHASE_OFFSET_LIMIT;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //when the dclk max-min value is too small, the panel has hw limitation. we should use
    //much safer parameter to do the FPLL
    //for now, we only found only AU46_T460hw03v1_HD has this problem.
    //So the value:0x0A is coming from AU46_T460hw03v1_HD
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET)
        < GATE_PHASELIMIT)
    {
        u16PhaseLimit = 0x1000;
    }
    #if (HW_DESIGN_HDMITX_VER == 2)//Maxim inside HDMITX case
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        u16PhaseLimit = PHASE_OFFSET_LIMIT_HDMITX;
    }
    #endif
    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT])
    {
        u16PhaseLimit = (MS_U16)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT];
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "user forces Phase Limit = 0x%X\n", u16PhaseLimit);
    }


    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Phase Limit = 0x%X\n", u16PhaseLimit);

    LPLL_BK_STORE;
    if(u8Lpll_bank)
        LPLL_BK_SWITCH(0x01);
    else
        LPLL_BK_SWITCH(0x00);

    MDrv_Write2Byte( L_BK_LPLL(0x0A), u16PhaseLimit);
    LPLL_BK_RESTORE;
}

void MDrv_Scaler_FastFrameLock(void *pInstance, MS_U16 u16InputVFreq,MS_U16 u16OutputVFreq)
{
    MS_U16 u16temp;
    MS_BOOL bCnt;

    //when calling MApi_XC_SetPanelTiming, input source is disabled
    //MApi_XC_DisableInputSource(TRUE, MAIN_WINDOW);
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0, BIT(0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 1, BIT(0));
    MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);//enable FPLL
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Enable Fast FPLL\n")

    u16temp = 0;
    bCnt = 0;

    while(1)
    {
        u16temp++;
        MsOS_DelayTask(1);
        //if(MDrv_ReadByte(H_BK_S_VOP(0x56)) & BIT(7)) // get vcnt freeze status
        if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_H) & BIT(7)) // get vcnt freeze status
        {
           //need wait vcnt down
           bCnt = 1 ;
           break;
        }
        else if(u16temp >((10000/u16OutputVFreq)+2)) // wait at lease one output vsync
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x08, BIT(3));  //SW Reset vcnt freeze
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x00, BIT(3));
            break;
        }
    }

    u16temp = 0;
    if(bCnt)
    {
        //while(MDrv_ReadByte(H_BK_S_VOP(0x56)) & BIT(7))
        while(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_H) & BIT(7))
        {
            u16temp++;
            MsOS_DelayTask(1);
            if(u16temp > ((10000/u16InputVFreq)+2))
            {
                //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(3));  //SW Reset vcnt freeze
                //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 0, BIT(3));
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x08, BIT(3));  //SW Reset vcnt freeze
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x00, BIT(3));
                break;
            }
        }
    }

    //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(3));  //SW Reset vcnt freeze
    //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 0, BIT(3));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "temp= %u\n", u16temp)
}

void MDrv_SC_Set_Output_Dclk_Slowly(void *pInstance, MS_U32 u32PllSet, MS_U32 u32Steps, MS_U32 u32DelayMs, SCALER_WIN eWindow)
{
    //slowly move to target
    MS_U32 u32PLLStep, u32OrigLPLL = 0;
    MS_U8  i;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32OrigLPLL = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    u32PllSet = u32PllSet & 0xFFFFFF;

    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_INITSETSTEP]
        && pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETSTEP])
    {
        u32Steps = pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETSTEP];
    }
    if (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_INITSETDELAY]
        && pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETDELAY])
    {
        u32DelayMs = pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_INITSETDELAY];
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetDCLK: Steps=%tu, DelayMs=%tu\n", (ptrdiff_t)u32Steps, (ptrdiff_t)u32DelayMs);

    u32PLLStep = abs(u32PllSet - u32OrigLPLL) / u32Steps;

if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_DCLK )
{
    if(u32PLLStep > (0x50000/u32Steps))
    {
        for(i=1; i<(u32Steps+1); i++)
        {
            if(u32DelayMs != 0)
            {
                MDrv_XC_DelayAndWaitOutVsync(pInstance, u32DelayMs, eWindow); // change output dclk when blanking
            }
            else
            {
                if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
                {
                    MDrv_XC_wait_output_vsync(pInstance, 2, 50, eWindow);        // change output dclk when blanking
                }
            }
            if(u32PllSet > u32OrigLPLL)
            {
                //add
                MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL + (u32PLLStep * i));
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Slow steps add 0x%tx\n",(ptrdiff_t)(u32OrigLPLL + (u32PLLStep * i)));
            }
            else
            {
                //minus
                MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL - (u32PLLStep * i));
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Slow steps minus 0x%tx\n", (ptrdiff_t)(u32OrigLPLL - (u32PLLStep * i)));
            }
        }
    }
    else
    {
        // direct set
        if ((MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED) && (!MDrv_SC_GetForceWrite(E_PANEL_MUTE)) )
        {
            MDrv_XC_wait_output_vsync(pInstance, 1, 50, eWindow);       // change output dclk when blanking
        }
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Direct set to 0x%tx\n", (ptrdiff_t)u32PllSet);
    }
}
else
{
    #if 1 //leon test
    while(abs(u32PllSet - u32OrigLPLL) > 0x3200)
    {
        if(u32PllSet > u32OrigLPLL)
        {
            u32OrigLPLL += 0x3200;
        }
        else
        {
            u32OrigLPLL -= 0x3200;
        }
        MsOS_DelayTask(1);
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL);
    }

    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);
#else
    if(abs(u32PllSet - u32OrigLPLL) < 0x3200)
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigLPLL);
    else
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Direct set to 0x%tx\n", (ptrdiff_t)u32PllSet);
}

    LPLL_BK_RESTORE;
}

//-------------------------------------------------------------------------------------------------
/// Get the time for scaler output one frame(Vertical Period)
/// @return @ref MS_U16 return output vertical Period(unit: ms)
//-------------------------------------------------------------------------------------------------
MS_U16 _MDrv_SC_GetOutputVPeriod(void *pInstance)
{
    MS_U16 u16VFreq = MDrv_SC_GetOutputVFreqX100(pInstance);
    return ((100000+u16VFreq/2)/u16VFreq); //Period = 1000/(vfreq/100)
}

//-----------------------------------------------------------------------------
//  below functions are for FSM FPLL
//-----------------------------------------------------------------------------
static MS_U16 _MDrv_Scaler_GetAVDStatus(void)
{
    return Hal_SC_GetAVDStatus();
}

void MDrv_Scaler_GetLPLLPhaseOfs(MS_U32* pu32UpOfs, MS_BOOL *bDir)
{
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    *pu32UpOfs   = (MS_U32)MDrv_Read2Byte(L_BK_LPLL(0x11));
    *bDir = MDrv_ReadRegBit(L_BK_LPLL(0x12), BIT(0));
    LPLL_BK_RESTORE;
}

void MDrv_Scaler_GetLPLLPrdOfs(MS_U32* pu32UpOfs, MS_BOOL *bDir)
{
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    *pu32UpOfs   = (MS_U32)MDrv_Read2Byte(L_BK_LPLL(0x13));
    *bDir = MDrv_ReadRegBit(L_BK_LPLL(0x14), BIT(0));
    LPLL_BK_RESTORE;
}

//judge whether the fpll is done or not.
MS_BOOL MDrv_Scaler_GetFPLLDoneStatus(void *pInstance)
{
    MS_U32 u32PhaseOfs,u32PrdOfs;
    MS_BOOL bPhaseDir,bPrdDir;

    MDrv_Scaler_GetLPLLPhaseOfs(&u32PhaseOfs,&bPhaseDir);
    MDrv_Scaler_GetLPLLPrdOfs(&u32PrdOfs,&bPrdDir);

    //printf("Framelock: (%lu,%u,%lu,%u)\n",u32PhaseOfs,bPhaseDir,u32PrdOfs,bPrdDir);
    if(MDrv_ReadRegBit(L_BK_LPLL(0x0C), BIT(3)) &&
       (u32PhaseOfs <= FPLL_PHASE_VAR_LIMIT) &&
       (u32PrdOfs <= FPLL_PRD_VAR_LIMIT))
    {
        //printf("OK.\n");
        return TRUE;
    }
    else
    {
        //printf("No.\n");
        return FALSE;
    }
}

MS_BOOL MDrv_Scaler_IsPRDLock(void *pInstance, MS_U16 *u16OrigPrd)
{
    MS_U16 u16NewPrd = 0;
    MS_U16 u16NewPhase = 0;
    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_U32 u32NewPhaseDiff = 0;
    #endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    u16NewPrd = MDrv_Read2Byte(L_BK_LPLL(0x13));
    u16NewPhase = MDrv_Read2Byte(L_BK_LPLL(0x11));
    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    u32NewPhaseDiff = MDrv_Read4Byte(L_BK_LPLL(0x3D));
    #endif

    LPLL_BK_RESTORE;
#if(PHASE_LOCK_EN)
    if( (u16NewPhase < (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT] ? (pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT]) : (PHASE_OFFSET_LIMIT)))
       && (u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(*u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT))
#else
    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled && (u32NewPhaseDiff < PHASE_OFFSET_LIMIT)) ||   // FRC enable case
       ((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled) && (u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(*u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT))) // non-FRC case
    #else
    if((u16NewPrd < FPLL_PRD_THRESHOLD) && (abs(*u16OrigPrd - u16NewPrd) < FPLL_PRD_VAR_LIMIT))
    #endif
#endif
    {
        *u16OrigPrd = u16NewPrd;
        return TRUE;
    }
    else
    {
        *u16OrigPrd = u16NewPrd;
        return FALSE;
    }
}

MS_BOOL MDrv_Scaler_IsPhaseStable(void *pInstance, MS_U16 *u16OrigPhase)
{
    MS_U16 u16NewPhase = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    u16NewPhase = MDrv_Read2Byte(L_BK_LPLL(0x11));
    LPLL_BK_RESTORE;


    if( (abs(*u16OrigPhase - u16NewPhase) < FPLL_PHASE_VAR_LIMIT) &&
        (u16NewPhase < (pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT] ? (pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT]) : (PHASE_OFFSET_LIMIT))) )
    {
       *u16OrigPhase = u16NewPhase;
        return TRUE;
    }
    else
    {
       *u16OrigPhase = u16NewPhase;
        return FALSE;
    }
}

void MDrv_Scaler_SetPhaseLock_Thresh(MS_U16 u16Thresh, MS_U8 u8Lpll_bank)
{
    LPLL_BK_STORE;
    if(u8Lpll_bank)
        LPLL_BK_SWITCH(0x01);
    else
        LPLL_BK_SWITCH(0x00);
     XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16Thresh= %u\n", u16Thresh);
    MDrv_Write2ByteMask( L_BK_LPLL(0x05), u16Thresh<<8, 0xF00);
    LPLL_BK_RESTORE;
}

static MS_U16 _MDrv_SC_FPLL_GetMCNRDelayTime(void *pInstance)
{
    MS_U16 u16DelayCount = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_U32 u32VFreq = 0;
    MS_U16 u16IPVsyncPeriod = 0;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW])
    {
        u16DelayCount = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber,
                                                        FALSE,  //linear mode
                                                        TRUE);  //force field packing mode here to get correct time

        u32VFreq = MDrv_XC_GetAccurateVFreqx1K(pInstance, MAIN_WINDOW);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "IP vFreqx1K= %u\n", u32VFreq);
        if (u32VFreq != 0)
        {
            u16IPVsyncPeriod = (MS_U16)((1000000UL + u32VFreq - 1)/u32VFreq);
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "IP sync period = %u ms\n", u16IPVsyncPeriod);
        }
        else
        {
            u16IPVsyncPeriod = DEFAULT_IP_VSYNC_PERIOD;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "!!!!!Warning u32VFreq = 0, Force u16IPVsyncPeriod: %u ms, need check input source.\n", u16IPVsyncPeriod);
        }
        u16DelayCount *= u16IPVsyncPeriod; //get delay time, uc just support main.
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MCNR Enable Delay=%u\n", u16DelayCount);
    }
    return u16DelayCount;
}

static MS_U16 _MDrv_SC_FPLL_GetMCNRDelayTime_For_ISR(void *pInstance,MS_U32 u32AccuratePeriod)
{
    MS_U16 u16DelayCount = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_U32 u32VFreq = 0;
    MS_U16 u16IPVsyncPeriod = 0;
    MS_U64 u64XTALClk = 0;

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW])
    {
        u16DelayCount = MDrv_XC_TransFrameNumToFactor(pInstance, gSrcInfo[MAIN_WINDOW].Status2.eFrameStoreNumber,
                                                        FALSE,  //linear mode
                                                        TRUE);  //force field packing mode here to get correct time

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "ip verticalperiod = %tu\n", (ptrdiff_t)u32AccuratePeriod);

        if(u32AccuratePeriod != 0)
        {
            u64XTALClk = (MS_U64)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000;
            do_div(u64XTALClk,u32AccuratePeriod);
            u32VFreq = (MS_U32)u64XTALClk;
        }

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "IP vFreqx1K= %u\n", u32VFreq);
        if (u32VFreq != 0)
        {
            u16IPVsyncPeriod = (MS_U16)((1000000UL + u32VFreq - 1)/u32VFreq);
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "IP sync period = %u ms\n", u16IPVsyncPeriod);
        }
        else
        {
            u16IPVsyncPeriod = DEFAULT_IP_VSYNC_PERIOD;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "!!!!!Warning u32VFreq = 0, Force u16IPVsyncPeriod: %u ms, need check input source.\n", u16IPVsyncPeriod);
        }
        u16DelayCount *= u16IPVsyncPeriod; //get delay time, uc just support main.
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "MCNR Enable Delay=%u\n", u16DelayCount);
    }
    return u16DelayCount;
}

MS_BOOL  MApi_XC_FrameLockCheck_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if FPLL_THRESH_MODE_SUPPORT
#ifdef UFO_XC_FB_LEVEL
    if(pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && ((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FB)
         || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)
    )
#else
    if (pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode
        && ((!(gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL)) || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode))
#endif
    {
        MS_BOOL bFrameLock = FALSE;
        LPLL_BK_STORE;
        LPLL_BK_SWITCH(0x00);
        bFrameLock = (MS_BOOL)(MDrv_ReadRegBit(L_BK_LPLL(0x0C), BIT(3)) >> 3);
        LPLL_BK_RESTORE;
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return bFrameLock;
    }
    else
#endif
    {
        //check  PRD Lock            // check  Phase Stable  ->check Frame lock
        if(pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable  &&  pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock)
        {
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return TRUE;
        }
        else
        {
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
            return FALSE;
        }
    }

}

MS_BOOL  MApi_XC_FrameLockCheck(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_FRAMELOCK XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_FRAMELOCK, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}


#define FPLL_ATV_FSM_FREERUN_STABLE_TIME    100
#define FPLL_ATV_FSM_FREQ_STABLE_TIME           10000
#define FPLL_ATV_FREQ_STABLE_COUNT  3

static MS_U32 gu32AtvIvsOvsLockFreqStableCount = 0;

static MS_BOOL _MApi_XC_FPLL_IsIvsFreqStable(void *pInstance, MS_U32* pu32VPeriod)
{
    static MS_U32 u32PreVPeriod = 0;
    MS_U32 u32VPeriodDiff;
    MS_U32 u32AccurateVPeriod = HAL_SC_ip_get_verticalperiod(pInstance, MAIN_WINDOW);

    if(u32AccurateVPeriod != 0)
    {
        u32VPeriodDiff = (u32PreVPeriod > u32AccurateVPeriod ? u32PreVPeriod - u32AccurateVPeriod: u32AccurateVPeriod - u32PreVPeriod);
        if(u32VPeriodDiff < V_PERIOD_DIFF_TOLERANCE)
        {
            gu32AtvIvsOvsLockFreqStableCount++;
        }
        else
        {
            gu32AtvIvsOvsLockFreqStableCount = 0;
        }
        u32PreVPeriod = u32AccurateVPeriod;

        if(pu32VPeriod != NULL)
        {
            *pu32VPeriod = u32AccurateVPeriod;
        }

        if(gu32AtvIvsOvsLockFreqStableCount > FPLL_ATV_FREQ_STABLE_COUNT)
        {
            return TRUE;
        }
    }

    return FALSE;
}

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
static void _MApi_XC_FPLL_AdjustOvsVtt(void *pInstance, MS_U32 u32AccurateVPeriod)
{
    MS_U16 u16OutVtt, u16OriOutVtt;
    MS_U16 u16AccurateVFreq = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    UtopiaResourceGetPrivate(g_pXCResource[E_XC_POOL_ID_INTERNAL_VARIABLE],(void**)(&pXCResourcePrivate));

    if(u32AccurateVPeriod != 0)
    {
        MS_U32 u32OutDiv = ((((MDrv_Read2Byte(L_BK_LPLL(0x0E)) & 0x00F0)>>4)<< 4 )|((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xF000) >> 12)) + 1;
        MS_U32 u32InDiv  = ((((MDrv_Read2Byte(L_BK_LPLL(0x0E)) & 0x000F)>>0)<< 4 )|((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0x0F00) >>  8)) + 1;

        #ifdef SUPPORT_FPLL_DOUBLE_OVS
        // for muji U02, 4K 0.5K@240Hz case, there is new bit 0x3C[11] to control from 5:24 to 5:48
        if (MDrv_ReadByte(REG_CHIP_REVISION) >= 1)
        {
            if ((MDrv_Read2Byte(L_BK_LPLL(0x3C)) & 0x0800) == 0x0000)
            {
                u32OutDiv = u32OutDiv *2;
            }
        }
        #endif
        //MS_U32 u32AccurateVFreqX1000 = (MS_U32)((MS_U64)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 1000 / u32AccurateVPeriod)*u32OutDiv/u32InDiv;
        // To avoid MS_U64 div  in kernel driver
        MS_U32 u32tmp_u32XTAL_Clock = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock;
        MS_U32 u32tmp_u32XTAL_Clock_mod = u32tmp_u32XTAL_Clock % u32AccurateVPeriod;
        u32tmp_u32XTAL_Clock = u32tmp_u32XTAL_Clock / u32AccurateVPeriod;
        u32tmp_u32XTAL_Clock = u32tmp_u32XTAL_Clock * 1000;
        u32tmp_u32XTAL_Clock_mod = (u32tmp_u32XTAL_Clock_mod * 1000) / u32AccurateVPeriod;
        MS_U32 u32AccurateVFreqX1000 = (u32tmp_u32XTAL_Clock + u32tmp_u32XTAL_Clock_mod)*u32OutDiv/u32InDiv;

        u16AccurateVFreq = (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock * 100 / u32AccurateVPeriod);
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            u16OriOutVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
        }
        else
#endif
        {
            u16OriOutVtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L) + 1;
        }
        //u16OutVtt = (MS_U16)((MS_U64)u16OriOutVtt * ((u16AccurateVFreq + 50) / 100 * 100) / u16AccurateVFreq);
        u16OutVtt = (MS_U16)((MS_U32)u16OriOutVtt * ((u16AccurateVFreq + 50) / 100 * 100 ) / u16AccurateVFreq);

        //if(u16OutVtt > u16OriOutVtt - 2 && u16OutVtt < u16OriOutVtt + 2)
        {
#if defined(PATCH_HW_VTT_LIMITATION)
            if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
            {
#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
                {
                    MS_U8 u8delaycnt = 1;
                    MS_U16 u16delaytime =0;

                    if(u16OutVtt > 2500)
                        u16delaytime = 1000*u8delaycnt/50;
                    else
                        u16delaytime = 1000*u8delaycnt/60;

                    // Update changed VTT to inform Raptor
                    MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
                    MOD_W2BYTEMSK(REG_MOD_BK00_65_L,u16OutVtt,0x1FFF);
                    MsOS_DelayTask(u16delaytime);
                }
#endif
                MHal_XC_SetVopVttByBK68(pInstance, u16OutVtt - 1, 0);
                MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
            }
            else
#endif
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, u16OutVtt - 1);
            }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
            if (psXCInstPri->u32DeviceID == 0)
            {
                SC_W2BYTE(1, REG_SC_BK10_0D_L, VOP_VTT_MASK);
            }
#endif
        }
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            MDrv_SC_set_output_dclk(pInstance, SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) + 1, SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L) + 1, (u16AccurateVFreq + 50) / 100 * 10, u32AccurateVFreqX1000, 0);
        }
        else
#endif
        {
            MDrv_SC_set_output_dclk(pInstance, SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) + 1, SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L) + 1, (u16AccurateVFreq + 50) / 100 * 10, u32AccurateVFreqX1000, 0);
        }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        MHal_FRC_SetYTrig(pInstance, 0x88f, 0x890);    //Need to set Y Trig again after adjust vtt
#endif
    }
}


static void _MApi_XC_FPLL_EnableLockIvsOvsMode(void *pInstance, MS_BOOL bEnable)
{
    if(bEnable)
    {
        MDrv_Write2Byte(L_BK_LPLL(0x1F), 0x2EE);
        MDrv_Write2ByteMask(L_BK_LPLL(0x78), BIT(12), BIT(12));
        gu32AtvIvsOvsLockFreqStableCount = 0;
        MDrv_SC_set_fpll(pInstance, DISABLE, DISABLE, 0);
    }
    else
    {
        MDrv_Write2Byte(L_BK_LPLL(0x1F), 0);
        MDrv_Write2ByteMask(L_BK_LPLL(0x78), 0, BIT(12));
        gu32AtvIvsOvsLockFreqStableCount = 0;
        MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);
    }
}
#endif

//==============================================================================
#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE

#define MAX_OUT_VTT_INCREASE_VALUE          10
#define MAX_LOCK_VALUE                                      0x30000
#define DISABLE_LOCK_VALUE                                  0x200000

static void _MApi_XC_FPLL_EnableLcokFreqOnly(void *pInstance,MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]feature is %x\n",__FUNCTION__, __LINE__, bEnable);
    if(bEnable)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x70), BIT(15), BIT(15));
        W2BYTEMSK(L_CLKGEN0(0x59), BIT(11), BIT(8) | BIT(10) | BIT(11));
        MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY);
    }
    else
    {

        MDrv_Write2ByteMask(L_BK_LPLL(0x70), 0, BIT(15));
#ifdef ENABLE_SPREADMODE
        if(MHal_XC_IsSupportSpreadMode(pInstance) && pXCResourcePrivate->stdrvXC_MVideo_Context.g_bSpreandEnable==TRUE)
        {
            W2BYTEMSK(L_CLKGEN0(0x59), BIT(11) ,BIT(8) | BIT(10) | BIT(11));//spread mode
        }
        else
#endif
        {
            MDrv_Write4Byte(L_BK_LPLL(0x1B), 0);
            MDrv_Write4Byte(L_BK_LPLL(0x19), 0);
#if (HW_DESIGN_HDMITX_VER == 1 || HW_DESIGN_HDMITX_VER == 2)//Maserati +Raptor HDMITX || Maxim + inside HDMITX case
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
            {
                W2BYTEMSK(L_CLKGEN0(0x59), BIT(11), BIT(8) | BIT(10) | BIT(11));
            }
            else
#endif
            {
                W2BYTEMSK(L_CLKGEN0(0x59), BIT(8), BIT(8) | BIT(10) | BIT(11));
            }
        }
    }
}

MS_BOOL MDrv_XC_FPLL_IsSupportLockFreqOnly(void *pInstance)
{
#if HW_SUPPORT_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
    //MS_U8 u8FrcIn, u8FrcOut;
    //MS_U16 u16FRCType=0;
    MS_BOOL bResult = TRUE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_LVDS)
    {
#ifdef UFO_XC_FB_LEVEL
        if(((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL)
                 || (gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
             ||  pXCResourcePrivate->stdrvXC_Display._bForceFreerun
#else
        if((gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL)|| pXCResourcePrivate->stdrvXC_Display._bForceFreerun
#endif
            #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            #ifndef HW_SUPPORT_FRC_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
            || ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled) && // FRC + 4K2K case needs to lock phase, FRC + FHD case use lockVfreq mode
                (IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) &&
                ((pXCResourcePrivate->stdrvXC_Display.g_u8FrcIn == 1)&&(pXCResourcePrivate->stdrvXC_Display.g_u8FrcOut == 1))) // For 50/60 case, use loch phase first
            #endif
            #endif
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableLockFreqOnly == FALSE))/*|| gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode */
        {
            bResult = FALSE;
        }
    }
    else
    {
#ifdef UFO_XC_FB_LEVEL
        if((((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL)
                 || (gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
               && !gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)
             ||  pXCResourcePrivate->stdrvXC_Display._bForceFreerun
#else
        if(((gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) && !gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode ) || pXCResourcePrivate->stdrvXC_Display._bForceFreerun
#endif
            #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            #ifndef HW_SUPPORT_FRC_LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
            || ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled) && // FRC + VB1 case needs to lock phase, FRC + LVDS case use lockVfreq mode
                (IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) &&
                ((pXCResourcePrivate->stdrvXC_Display.g_u8FrcIn == 1)&&(pXCResourcePrivate->stdrvXC_Display.g_u8FrcOut == 1))) // For 50/60 case, use loch phase first
            #endif
            #endif
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableLockFreqOnly == FALSE))/*|| gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode */
        {
            bResult = FALSE;
        }
    }

#if ((HW_DESIGN_HDMITX_VER == 1)||(HW_DESIGN_HDMITX_VER == 2))
    // Maserati Raptor, USEs lock phase mode.
    // For Maxim HDMI Tx, need to lock phase
    // Not use LockVfreq mode
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        bResult = FALSE;
    }
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]gSrcInfo[MAIN_WINDOW].bFBL= %x\n",__FUNCTION__, __LINE__ ,gSrcInfo[MAIN_WINDOW].bFBL );
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]gSrcInfo[MAIN_WINDOW].bR_FBL= %x\n",__FUNCTION__, __LINE__ ,gSrcInfo[MAIN_WINDOW].bR_FBL );
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode= %x\n",__FUNCTION__, __LINE__ ,gSrcInfo[MAIN_WINDOW].Status2.eMirrorMode);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]pXCResourcePrivate->stdrvXC_Display._bForceFreerun= %x\n",__FUNCTION__, __LINE__ ,pXCResourcePrivate->stdrvXC_Display._bForceFreerun);
//    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableLockFreqOnly= %x\n",__FUNCTION__, __LINE__ ,pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableLockFreqOnly);

    #if 0
    else
    {
        MS_U32 u32InDiv, u32OutDiv;
        u32OutDiv = ((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xF000) >> 12) + 1;
        u32InDiv = ((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0x0F00) >> 8) + 1;

        //if(u32OutDiv % u32InDiv != 0) bResult = FALSE;
    }
    #endif

    if(!bResult)
    {
        MDrv_Write2ByteMask(L_BK_LPLL(0x70), 0, BIT(15));
#ifdef ENABLE_SPREADMODE
    if(MHal_XC_IsSupportSpreadMode(pInstance) && pXCResourcePrivate->stdrvXC_MVideo_Context.g_bSpreandEnable==FALSE)
#endif
        {
#if ((HW_DESIGN_HDMITX_VER == 1) || (HW_DESIGN_HDMITX_VER == 2))//Maserati +Raptor HDMITX or Maxim + inside HDMITX case
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
            {
                W2BYTEMSK(L_CLKGEN0(0x59), BIT(11), BIT(8) | BIT(10) | BIT(11));
            }
            else
#endif
            {
                W2BYTEMSK(L_CLKGEN0(0x59), BIT(8), BIT(8) | BIT(10) | BIT(11));
            }
        }
    }
    else
    {
        MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY);
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, " IsSupportLockFreqOnly= %x\n", bResult );

    return bResult;

#else

    return FALSE;

#endif

}

static MS_U32 _MApi_XC_FPLL_CalculateGCD(MS_U32 u32Input, MS_U32 u32Output)
{
    MS_U32 u32GCD = 1;
    while((u32Input !=0) && (u32Output !=0)) // GCD
    {
        if(u32Input > u32Output)
        {
            u32GCD = u32Output;
            u32Input %= u32Output;
        }
        else // Temp A < TempB
        {
            u32GCD = u32Input;
            u32Output %= u32Input;
        }
        //printf("GCD =%u\n", u32GCD);
    }

    //u32GCD never be 0
    return u32GCD;
}

static MS_BOOL _MApi_XC_FPLL_LockFreqOnly(void *pInstance)
{
    MS_U16 u16OutHtt, u16OutVtt, u16InHtt, u16InVtt;
    MS_U32 u32OutputClkDiv, u32InputClkDiv;
    MS_U32 u32InDiv, u32OutDiv=0;
    MS_U32 u32RealInDiv, u32RealOutDiv=0;
    MS_U32 u32GCD = 1;
    MS_U8   i;
    MS_U32 u32MaxOutVtt =1, u32MaxGCD = 1;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    MS_U8 u8LPLL_Type = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);

    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u16OutHtt = SC_R2BYTE(0, REG_SC_BK46_02_L) + 1;
        u16OutVtt = SC_R2BYTE(0, REG_SC_BK46_01_L) + 1;
    }
    else
    #endif
    {
        u16OutHtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) + 1;
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            u16OutVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
        }
        else
#endif
        {
            u16OutVtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L) + 1;
        }
        #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        if (IsVBY1_16LANE(u8LPLL_Type))
        {
            // maserati mode, get Htt/Vtt from FO tgen
            u16OutHtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_02_L) + 1;
            u16OutVtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_01_L) + 1;
        }
        #endif
    }

    MsOS_DelayTask(50); //leon test , 50ms OK, 20ms ng, 32ms ng, 40ng
    u16InHtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_28_L) + 1;
    u16InVtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_1F_L);
    u32OutDiv = ((((MDrv_Read2Byte(L_BK_LPLL(0x0E)) & 0x00F0)>>4)<< 4 )|((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xF000) >> 12)) + 1;
    u32InDiv  = ((((MDrv_Read2Byte(L_BK_LPLL(0x0E)) & 0x000F)>>0)<< 4 )|((MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0x0F00) >>  8)) + 1;

    #ifdef SUPPORT_FPLL_DOUBLE_OVS
    // for muji U02, 4K 0.5K@240Hz case, there is new bit 0x3C[11] to control from 5:24 to 5:48
    if (MDrv_ReadByte(REG_CHIP_REVISION) >= 1)
    {
        if ((MDrv_Read2Byte(L_BK_LPLL(0x3C)) & 0x0800) == 0x0000)
        {
            u32OutDiv = u32OutDiv *2;
        }
    }
    #endif

    u32RealInDiv  = u32InDiv;
    u32RealOutDiv = u32OutDiv;

    if(gSrcInfo[MAIN_WINDOW].bInterlace) u32OutDiv*= 2;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u16InHtt=%u, u16InVtt=%u\n",__FUNCTION__, __LINE__, u16InHtt, u16InVtt);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u16OutHtt=%u, u16OutVtt=%u\n",__FUNCTION__, __LINE__, u16OutHtt, u16OutVtt);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InDiv =%tu, u32OutDiv=%tu\n",__FUNCTION__, __LINE__, (ptrdiff_t)u32InDiv, (ptrdiff_t)u32OutDiv);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%dgSrcInfo[MAIN_WINDOW].bInterlace=%x, reg_bInterlace=%u\n",__FUNCTION__, __LINE__, gSrcInfo[MAIN_WINDOW].bInterlace, SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_21_L, 0x01));

    u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv;
#ifdef SUPPORT_FPLL_REFER_24MXTAL_4P
      u32OutputClkDiv = u16OutHtt * u16OutVtt* u32OutDiv/4;
#else

#ifdef SUPPORT_FPLL_REFER_24MXTAL
    u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv/2;
#else
    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv/2;
    }
    else
    #endif
    {
        u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv;
    }
#endif
#endif
    if(u32InputClkDiv == 0 || u32OutputClkDiv == 0) return FALSE;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%tu, u32OutputClkDiv=%tu\n",__FUNCTION__, __LINE__, (ptrdiff_t)u32InputClkDiv, (ptrdiff_t)u32OutputClkDiv);

    //u16OutVtt-=5;  // scan Vtt  from (Vtt-5)~(Vtt+5) for get the better GCD
    for(i = 0; i < MAX_OUT_VTT_INCREASE_VALUE; i++)
    {
        u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv;
#ifdef SUPPORT_FPLL_REFER_24MXTAL_4P
      u32OutputClkDiv = u16OutHtt * u16OutVtt* u32OutDiv/4;
#else

#ifdef SUPPORT_FPLL_REFER_24MXTAL
    u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv/2;
#else
    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv/2;
    }
    else
    #endif
    {
        u32OutputClkDiv = u16OutHtt * u16OutVtt * u32OutDiv;
    }
#endif
#endif
        u32GCD = _MApi_XC_FPLL_CalculateGCD(u32InputClkDiv, u32OutputClkDiv);
        if(u32GCD > u32MaxGCD)
        {
            u32MaxOutVtt = u16OutVtt;  // from (Vtt-5)~(Vtt+5)
            u32MaxGCD = u32GCD;
        }
        u16OutVtt++;
    }

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u16OutVtt=%u, u32MaxOutVtt=%tu\n",__FUNCTION__, __LINE__, u16OutVtt,(ptrdiff_t)u32MaxOutVtt);

    // Get HDMI Pixel Repetition
    MS_U8 u8CurHDMIPixelRep = 1;

    if( gSrcInfo[MAIN_WINDOW].enInputSourceType >= INPUT_SOURCE_HDMI
    && gSrcInfo[MAIN_WINDOW].enInputSourceType < INPUT_SOURCE_HDMI_MAX)
    {
        u8CurHDMIPixelRep = (MDrv_HDMI_avi_infoframe_info(_BYTE_5)& 0x0F)+1;
    }

    u32InputClkDiv = u16InHtt * u16InVtt * u32InDiv * u8CurHDMIPixelRep;
#ifdef SUPPORT_FPLL_REFER_24MXTAL_4P
      u32OutputClkDiv = u16OutHtt * u32MaxOutVtt* u32OutDiv/4;
#else

#ifdef SUPPORT_FPLL_REFER_24MXTAL
    u32OutputClkDiv = u16OutHtt * u32MaxOutVtt * u32OutDiv/2;
#else
    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u32OutputClkDiv = u16OutHtt * u32MaxOutVtt * u32OutDiv/2;
    }
    else
    #endif
    {
        u32OutputClkDiv = u16OutHtt * u32MaxOutVtt * u32OutDiv;
    }
#endif
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%tu, u32OutputClkDiv=%tu\n",__FUNCTION__, __LINE__, (ptrdiff_t)u32InputClkDiv, (ptrdiff_t)u32OutputClkDiv);

    u32InputClkDiv /= u32MaxGCD;
    u32OutputClkDiv /= u32MaxGCD;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%tu, u32OutputClkDiv=%tu\n",__FUNCTION__, __LINE__, (ptrdiff_t)u32InputClkDiv, (ptrdiff_t)u32OutputClkDiv);

    if(u32InputClkDiv > MAX_LOCK_VALUE || u32OutputClkDiv > MAX_LOCK_VALUE)
    {
        MS_U8 u8GainPI = MDrv_ReadByte(H_BK_LPLL(0x0B));
        if(u8GainPI > 0x21)
        {
            u8GainPI -= 0x11;
            MDrv_WriteByte(H_BK_LPLL(0x0B), u8GainPI);  //Gain
        }
    }

    if(u32InputClkDiv > DISABLE_LOCK_VALUE || u32OutputClkDiv > DISABLE_LOCK_VALUE)
    {
    #if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
        _MApi_XC_FPLL_EnableLcokFreqOnly(pInstance,FALSE);
    #endif
    }

    //if(u32InputClkDiv < 3000 && u32OutputClkDiv < 3000)
    {
        if(u32InputClkDiv > u32OutputClkDiv)
        {   // 0x2000 is experience value provided by HW designer
            u32GCD = 0x2000/u32OutputClkDiv + 1;
        }
        else
        {
            u32GCD = 0x2000/u32InputClkDiv + 1;
        }
        u32InputClkDiv *= u32GCD;
        u32OutputClkDiv *= u32GCD;
    }

    u32InputClkDiv -= 1;
    u32OutputClkDiv -= 1;

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u32InputClkDiv=%tu, u32OutputClkDiv=%tu\n",__FUNCTION__, __LINE__, (ptrdiff_t)u32InputClkDiv, (ptrdiff_t)u32OutputClkDiv);
    LPLL_BK_SWITCH(0x00);
    MDrv_Write4Byte(L_BK_LPLL(0x1B), u32OutputClkDiv);
    MDrv_Write4Byte(L_BK_LPLL(0x19), u32InputClkDiv);

    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        SC_W2BYTE(0, REG_SC_BK46_01_L, u32MaxOutVtt - 1);
    }
    else
    #endif
    {
        if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        {
#if defined(PATCH_HW_VTT_LIMITATION)
            if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
            {
                _MLOAD_ENTRY(pInstance);
                MHal_XC_SetVopVttByBK68(pInstance, u32MaxOutVtt - 1, 1);
                _MLOAD_RETURN(pInstance);
                MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
            }
            else
#endif
            {
                _MLOAD_ENTRY(pInstance);

                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L, u32MaxOutVtt - 1, 0xFFFF);

                MDrv_XC_MLoad_Fire(pInstance, TRUE);

                _MLOAD_RETURN(pInstance);
            }
        }
        else
        {
#if defined(PATCH_HW_VTT_LIMITATION)
            if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
            {
                MHal_XC_SetVopVttByBK68(pInstance, u32MaxOutVtt - 1, 0);
                MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
            }
            else
#endif
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, u32MaxOutVtt - 1);
            }
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (psXCInstPri->u32DeviceID == 0)
        {
            SC_W2BYTE(1, REG_SC_BK10_0D_L, VOP_VTT_MASK);
        }
#endif

        #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        if (IsVBY1_16LANE(u8LPLL_Type))
        {
            // maserati mode, set Vtt to FO tgen
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_01_L, u32MaxOutVtt -1);
        }

        MHal_FRC_SetYTrig(pInstance, 0x88f, 0x890);    //Need to set Y Trig again after adjust vtt
        #endif
    }

#if 1
    //// after Vtt refactoring, the LPLL set need to re-calculate and update
    MS_U64 u64Htt = 0;
    MS_U64 u64Vtt = 0;
    MS_U32 u32OutputVfreq = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;


    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u64Htt = (MS_U64)SC_R2BYTE(0, REG_SC_BK46_02_L) + 1;
        u64Vtt = (MS_U64)SC_R2BYTE(0, REG_SC_BK46_01_L) + 1;
    }
    else
    #endif
    {
        u64Htt = (MS_U64)SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L) + 1;
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            u64Vtt = (MS_U64)SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)+1;
        }
        else
#endif
        {
            u64Vtt = (MS_U64)SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L) + 1;
        }

        #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        if (IsVBY1_16LANE(u8LPLL_Type))
        {
            // maserati mode, get Htt/Vtt from FO tgen
            u64Htt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_02_L) + 1;
            u64Vtt = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BKCB_01_L) + 1;
        }
        #endif
    }


    MS_U64 u64DClk = u64Htt * u64Vtt * (MS_U64)u32OutputVfreq;
    MS_U32 u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, u64DClk, FALSE);

    MDrv_Write3Byte(L_BK_LPLL(0x0F), u32PllSet);


#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u64Htt=%lu, u64Vtt=%lu, u32OutputVfreq =%u\n",
                 __FUNCTION__, __LINE__, u64Htt, u64Vtt, u32OutputVfreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u64DClk=%lu, u32PllSet=0x%x\n",__FUNCTION__, __LINE__, u64DClk, u32PllSet);
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u64Htt=%llu, u64Vtt=%llu, u32OutputVfreq =%tu\n",
                 __FUNCTION__, __LINE__, u64Htt, u64Vtt, (ptrdiff_t)u32OutputVfreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "[%s][%d]u64DClk=%llu, u32PllSet=0x%tx\n",__FUNCTION__, __LINE__, u64DClk, (ptrdiff_t)u32PllSet);
#endif

#endif
    LPLL_BK_RESTORE;

    return TRUE;
}
#endif // LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
//=======================================================
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6)) //Only FRC_INSIDE and HW_DESIGN_4K2K_VER == 6 use this function, for fixing compile warning we use #if here
static void _MApi_XC_FPLL_AdjustVtt_ForPhaseLock(void *pInstance, MS_BOOL bChange)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bPhaseDiffUp = FALSE;
    MS_U16 u16NowVtt = 0;
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    bPhaseDiffUp = MDrv_Read2Byte(L_BK_LPLL(0x12)) &0x01;
    LPLL_BK_RESTORE;

#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
    {
        u16NowVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK) + 1;
    }
    else
#endif
    {
        u16NowVtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK) + 1;
    }

    if (u16NowVtt < VTT_OFFSET_FOR_PHASE_LOCK)
    {
        printf("ALERT!!! Vtt too small so not adjust Vtt for phase lcok~~~\n");
        return;
    }

    if (bChange)
    {
        if (bPhaseDiffUp)
            u16NowVtt = u16NowVtt - VTT_OFFSET_FOR_PHASE_LOCK;
        else
            u16NowVtt = u16NowVtt + VTT_OFFSET_FOR_PHASE_LOCK;
    }
    else // revert to original Vtt here
    {
        if (bPhaseDiffUp)
            u16NowVtt = u16NowVtt + VTT_OFFSET_FOR_PHASE_LOCK;
        else
            u16NowVtt = u16NowVtt - VTT_OFFSET_FOR_PHASE_LOCK;
    }
#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
    {
        _MLOAD_ENTRY(pInstance);
        MHal_XC_SetVopVttByBK68(pInstance, (u16NowVtt - 1)&VOP_VTT_MASK, 1);
        _MLOAD_RETURN(pInstance);
        MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
    }
    else
#endif
    {
        _MLOAD_ENTRY(pInstance);
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16NowVtt - 1)&VOP_VTT_MASK, VOP_VTT_MASK);
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
   }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        MHal_FRC_SetYTrig(pInstance, 0x88f, 0x890);    //Need to set Y Trig again after adjust vtt
#endif
}

#define LOCK_FREQ_RANGE_PROTECT  3
static MS_BOOL _MApi_XC_FPLL_LockFreq_Is_SafeRange(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    MS_U32 u32PhaseDiff = MDrv_Read4Byte(L_BK_LPLL(0x3D));
    MS_U16 u16Limitoffset = MDrv_Read2Byte(L_BK_LPLL(0x0A));
    LPLL_BK_RESTORE;

    //U16 u16Vtrig= MHal_TGEN_GetVTrigY();
    MS_U16 u16Vtotal = 0;
    MS_U32 u32OutVfreqx100 = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC *10;
    //U16 u16VRefpoint = MHal_TGEN_Get_FpllRefPointY();
    MS_U32 u32SafeRangeMin = 0, u32NewSafeRangeMin=0;

#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
    {
        u16Vtotal = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK) + 1;
    }
    else
#endif
    {
        u16Vtotal = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK) + 1;
    }
    // Conver Phase to line base, phase(Xtal clk)
    u32SafeRangeMin = (MS_U64)u16Limitoffset*u16Vtotal*u32OutVfreqx100/(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock*100) + LOCK_FREQ_RANGE_PROTECT;

    if(u32SafeRangeMin > (MS_U16)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2))  // if vtotal = 0.5k, Vref and Vtrigger range will be less than Saferangemin
    {
        u32NewSafeRangeMin = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/2);
    }
    else
    {
        u32NewSafeRangeMin =  u32SafeRangeMin;
    }

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MS_U16 u16IvsOvs = MDrv_Read2Byte(L_BK_LPLL(0x0C)) & 0xFF00;
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled) && // FRC + 4K2K case needs to lock phase, FRC + FHD case use lockVfreq mode
        (IS_OUTPUT_4K2K(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height)) &&
        (u16IvsOvs == 0x0000))
    {
        u32NewSafeRangeMin = (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/4);
    }
#endif

    #if 0 // sync code from ursa, disable first
    // Check the Vref and Vtrigger range far enough than saferange or replace the Saferagnemin by the Vref and Vtigger diff
    if((U16)diff(u16Vtrig, u16VRefpoint) <= u32NewSafeRangeMin)
    {
        //printf("u16VRefpoint:%u and u16Vtrig:%u diff less than u32NewSafeRangeMin:%u\r\n ", u16VRefpoint, u16Vtrig, u32NewSafeRangeMin);
        if(diff(u16Vtrig, u16VRefpoint) <=LOCK_FREQ_RANGE_PROTECT)
        {
           //printf("[Warning]Vtrigger and Vref less than 3 line\r\n");
           return FALSE;
        }
        else
        {
           u32NewSafeRangeMin = diff(u16Vtrig, u16VRefpoint)-LOCK_FREQ_RANGE_PROTECT;
        }
    }
    else if((U16)diff(u16Vtotal,diff(u16Vtrig, u16VRefpoint)) <= u32NewSafeRangeMin)
    {
        //printf("u16VRefpoint:%u and u16Vtrig:%u diff less than u32NewSafeRangeMin:%u\r\n ", u16VRefpoint, u16Vtrig, u32NewSafeRangeMin);
        if(diff(u16Vtotal,diff(u16Vtrig, u16VRefpoint)) <=LOCK_FREQ_RANGE_PROTECT)
        {
           //printf("[Warning]Vtrigger and Vref less than 3 line\r\n");
           return FALSE;
        }
        else
        {
           u32NewSafeRangeMin = diff(u16Vtotal,diff(u16Vtrig, u16VRefpoint))-LOCK_FREQ_RANGE_PROTECT;
        }
    }
    #endif

    //if(u32NewSafeRangeMin != u32SafeRangeMin)  // if the NewSafeRagne change, update the limit offset
    {
        u16Limitoffset = (MS_U64)u32NewSafeRangeMin*pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.u32XTAL_Clock*100/(u16Vtotal*u32OutVfreqx100); // convert phase diff to Xtal unit
    }

    if(u32PhaseDiff > u16Limitoffset)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "not safe case, u16PhaseDiff %x u16Limitoffset: %x \r\n ", u32PhaseDiff, u16Limitoffset);
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}
#endif

/*
   Frame lock mechanism should be detected here.
   msAPI_Scaler_FPLL_FSM wait to lock frame only if the
   phase error is small enough.
*/

static void _MApi_XC_FPLL_FSM_ISR(SC_INT_SRC eIntNum, void * pParam)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U32 u32Ofs;
    MS_BOOL Dir ;
    MS_U16 u16PhaseLimit;
#if ENABLE_LOCK_IVS_OVS_FOR_ATV
    MS_U8 u8LPLL_Type = (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;
#endif

#if (!ENABLE_REGISTER_SPREAD)
    MS_U8 u8Bank;
#endif
    if(eIntNum >= MAX_SC_INT)
    {
        printf("Interrupt number >= max scaler interrupt number\n");
    }
    if(pParam == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR,"ISR pParam is NULL\n");
    }
    //printf("FSM looping pXCResourcePrivate->stdrvXC_Display._enFpllFsm = %x\n", pXCResourcePrivate->stdrvXC_Display._enFpllFsm);
    switch (pXCResourcePrivate->stdrvXC_Display._enFpllFsm)
    {
    case FPLL_IDLE_STATE:
        // no signal, do nothing
        break;

    case FPLL_INIT_STATE://dtv special
        //wait 2 input vsync for framelock phase error stable
        //according our test of 24P video, after enble mvop, need at least 2 input vsync, framelock
        //phase error can be stable.
        pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
        pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;

        #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        {
            Hal_XC_FRC_R2_Set_FPLL_Lockdone(pInstance, FALSE);
        }
        #endif
        if((MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32InputVsyncStart) < pXCResourcePrivate->stdrvXC_Display.s_u32DelayT)
            break;

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL INIT->PRD]\n")
        pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd = FPLL_PRD_MAX;

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
        if (!(IsVBY1_16LANE(u8LPLL_Type)))
            _MApi_XC_FPLL_EnableLockIvsOvsMode(pInstance, FALSE);
#endif

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
        if(MDrv_XC_FPLL_IsSupportLockFreqOnly(pInstance))
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_FREQ_ONLY_LOCK_STATE;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_PRD_LOCK_STATE;
        }
#else
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_PRD_LOCK_STATE;
#endif

        pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
        pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer;//For MCNR enable counter
        //InISR, we can not wait XC entry, because interrupt is higher than xc mutex
#if (!ENABLE_REGISTER_SPREAD)
        u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
        Hal_SC_SetOPWriteOff(pInstance, ENABLE);
#if (!ENABLE_REGISTER_SPREAD)
        MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
        break;
    case FPLL_ATV_INIT_STATE://atv special
#if(PHASE_LOCK_EN)
        pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
#else
        pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = TRUE;
#endif
        pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;
        pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_IP_STABLE_STATE;
        break;

    case FPLL_WAIT_IP_STABLE_STATE://atv special

        #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        {
            Hal_XC_FRC_R2_Set_FPLL_Lockdone(pInstance, FALSE);
        }
        #endif

        if (_MDrv_Scaler_GetAVDStatus() & VD_HSYNC_LOCKED)
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_LPLL_OFS_STATE;

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
            _MApi_XC_FPLL_EnableLcokFreqOnly(pInstance,FALSE);

#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        {
            MDrv_Write2Byte(L_BK_LPLL(0x0A), 0x400);
        }
        else
#endif
        {
            MDrv_Write2Byte(L_BK_LPLL(0x0A), PHASE_OFFSET_LIMIT_FREQ_ONLY);
        }
#endif

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
            if (!(IsVBY1_16LANE(u8LPLL_Type)))
            {
            _MApi_XC_FPLL_EnableLockIvsOvsMode(pInstance, TRUE);
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCK_ATV_ADJUST_IVS_STATE;
            }
#endif

#if (!ENABLE_REGISTER_SPREAD)
            u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
            Hal_SC_SetOPWriteOff(pInstance, ENABLE);
#if (!ENABLE_REGISTER_SPREAD)
            MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
            pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
            pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[WAIT IP STABLE] -> [WAIT LPLL OFS]\n")
        }
        else if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
        }
        break;

#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE

        case FPLL_FREQ_ONLY_LOCK_STATE:
        {
            _MApi_XC_FPLL_LockFreqOnly(pInstance);
            #if 0 //(FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled)
            {
                _MApi_XC_FPLL_AdjustVtt_ForPhaseLock(pInstance, TRUE);
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_FREQ_ONLY_LOCK_WAIT_PHASE_STABLE_STATE;
                pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
            }
            else
            #endif
            {
                //pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCKED_STATE;
                pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
            }
        }
        break;

#endif

#if ENABLE_LOCK_IVS_OVS_FOR_ATV
        case FPLL_LOCK_ATV_ADJUST_IVS_STATE:
            {
                MS_U32 u32VPeriod = 0;
                if(_MApi_XC_FPLL_IsIvsFreqStable(pInstance, &u32VPeriod))
                {
                    _MApi_XC_FPLL_AdjustOvsVtt(pInstance, u32VPeriod);
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCK_ATV_IVS_OVS_STATE;
                    pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
                }
                else if(FPLL_ATV_FSM_FREQ_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCK_ATV_IVS_OVS_STATE;
                    pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
                }
            }
        break;

        case FPLL_LOCK_ATV_IVS_OVS_STATE:
            {
                if(FPLL_ATV_FSM_FREERUN_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
                {
                    MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_LPLL_OFS_STATE;
                    pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
                }
            }
        break;
#endif

    case FPLL_WAIT_LPLL_OFS_STATE://atv special
        // We set PHASE_OFFSET_LIMIT to "limit for lpll phase offset", the "phase dif value" will take
        // PHASE_OFFSET_LIMIT+1 as its max value. then we set the gate to PHASE_OFFSET_LIMIT,
        // every case the "phase dif value" is smaller or equal to the PHASE_OFFSET_LIMIT,
        // we consider the phase is changed to a level, then we can do the adjustment.
        MDrv_Scaler_GetLPLLPhaseOfs(&u32Ofs,&Dir);

        //u16PhaseLimit = pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_PHASELIMIT] ? pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_PHASELIMIT] : PHASE_OFFSET_LIMIT;
        u16PhaseLimit = MDrv_Read2Byte(L_BK_LPLL(0x0A));
#if (!ENABLE_LOCK_IVS_OVS_FOR_ATV)
        if (u32Ofs <= u16PhaseLimit)
        {

            MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_VD_LOCKED_STATE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "Phase offs go into limit~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[WAIT LPLL OFS] -> [VD LOCKED]\n")
        }
        else if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
        }

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "===> Phase Diff=0x%tx, Limit=0x%x\n", (ptrdiff_t)u32Ofs, u16PhaseLimit)
#else
       if (IsVBY1_16LANE(u8LPLL_Type))
       {
            if (u32Ofs <= u16PhaseLimit)
            {

                MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_VD_LOCKED_STATE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "Phase offs go into limit~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[WAIT LPLL OFS] -> [VD LOCKED]\n")
            }
            else if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
            }

                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "===> Phase Diff=0x%tx, Limit=0x%x\n", (ptrdiff_t)u32Ofs, u16PhaseLimit)

       }
       else
       {
            MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_VD_LOCKED_STATE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "Phase offs go into limit~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n")
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[WAIT LPLL OFS] -> [VD LOCKED]\n")
       }

#endif
        break;

    case FPLL_VD_LOCKED_STATE://atv special
        {
            pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable  = TRUE;
            pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock      = TRUE;

             #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
            {
                Hal_XC_FRC_R2_Set_FPLL_Lockdone(pInstance, TRUE);
            }
            #endif

            if(pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR
                && pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
                && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
            {
                MS_U32 u32VPeriod = 0;
                if(_MApi_XC_FPLL_IsIvsFreqStable(pInstance, &u32VPeriod))
                {
                //Wait some vsync, then turn on MCNR
                    if(_MDrv_SC_FPLL_GetMCNRDelayTime_For_ISR( pInstance, u32VPeriod) < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer))
                {
                    if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
                    {
#if (!ENABLE_REGISTER_SPREAD)
                        u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
                        Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
#if (!ENABLE_REGISTER_SPREAD)
                        MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
                    }
                    pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;

                    }
                }
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
            }
        }
        break;

    case FPLL_WAIT_PRD_LOCK_STATE://Dtv special
        if(MDrv_Scaler_IsPRDLock(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd))
        {
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL PRD locked->Phase]\n")
            pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase = FPLL_PHASE_MAX;
            pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
#if(PHASE_LOCK_EN)
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_WAIT_PHASE_STABLE_STATE;
#else
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_CHECK_FRAME_LOCK_STATE;
#endif
        }
        else
        {
            #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            // For FRC case, use LPLL lock fisrt, if lock time too long, switch to LockVfreq mode
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled
                && (FPLL_SWITCH_LOCKVFREQ_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
                && _MApi_XC_FPLL_LockFreq_Is_SafeRange(pInstance)
                #if ((HW_DESIGN_HDMITX_VER == 1)||(HW_DESIGN_HDMITX_VER == 2)) // For Maserati+Raptor /Maxim HDMI Tx, need to lock phase, not switch to LockVfreq mode
                && !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
                #endif
                )
            {
#ifdef ENABLE_SPREADMODE
            if(MHal_XC_IsSupportSpreadMode(pInstance))
            {
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bSpreandEnable==TRUE)       //return spreadmode to FREQ_ONLY_LOCK
                {
                    W2BYTEMSK(L_BK_LPLL(0x0C), R2BYTEMSK(L_BK_LPLL(0x7A), 0x000F)<<12, 0xF000);
                    W2BYTEMSK(L_BK_LPLL(0x0E), R2BYTEMSK(L_BK_LPLL(0x7A), 0x00F0),0x00F0);
                }
                _MApi_XC_FPLL_EnableSpreadMode(pInstance,FPLL_SPREAD_MODE_NONE);
             }
#endif
                _MApi_XC_FPLL_EnableLcokFreqOnly(pInstance,TRUE);

                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_FREQ_ONLY_LOCK_STATE;
            }
            #endif

            if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
            }
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "PRD: phase %04x, prd %04x\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x13)))
        break;
#if(PHASE_LOCK_EN)
    case FPLL_WAIT_PHASE_STABLE_STATE://Dtv special
        if(MDrv_Scaler_IsPhaseStable(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase))
        {

            if(MDrv_Scaler_IsPRDLock(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd)) // read current PRD again
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL phase->check Frame lock]\n")
            }

            MDrv_SC_set_fpll(pInstance, ENABLE, DISABLE, 0);

            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL phase->check Frame lock]\n")

            pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer = MsOS_GetSystemTime();
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_CHECK_FRAME_LOCK_STATE;
        }
        else
        {
            if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
            }
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "PHS: phase %04x, prd %04x\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x13)))
        break;
#endif
    case FPLL_CHECK_FRAME_LOCK_STATE://Dtv special
        {

            //MDrv_Scaler_IsPRDLock(&s_u16OrigPrd);       // read current PRD again
            //MDrv_Scaler_IsPhaseStable(&s_u16OrigPhase); // read current phase again
            if(MDrv_Scaler_IsPRDLock(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPrd))
            {
                pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock = TRUE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL PRD Lock->check Frame lock]\n")
            }
#if(PHASE_LOCK_EN)
            if(MDrv_Scaler_IsPhaseStable(pInstance, &pXCResourcePrivate->stdrvXC_Display.s_u16OrigPhase))
            {
                pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = TRUE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "[FPLL phase Lock->check Frame lock]\n")
            }
#endif
            LPLL_BK_STORE;
            LPLL_BK_SWITCH(0x00);
#if(PHASE_LOCK_EN)
            if((MDrv_Read2Byte(L_BK_LPLL(0x11)) > FPLL_PHASE_VAR_LIMIT) || (MDrv_Read2Byte(L_BK_LPLL(0x13)) > FPLL_PRD_VAR_LIMIT))
#else
            #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled) && (MDrv_Read2Byte(L_BK_LPLL(0x13)) > FPLL_PRD_VAR_LIMIT))
            #else
            if((MDrv_Read2Byte(L_BK_LPLL(0x13)) > FPLL_PRD_VAR_LIMIT))
            #endif
#endif
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "CHK: phase %04x, prd %04x\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x13)))
                if(FPLL_FSM_STABLE_TIME < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display.s_u32FSMTimer))
                {
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;      // force re-start
                    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "CHK Stable count is outof range, force re-start.\n")
                }
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCKED_STATE;
                XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "Locked, the using SET is %tx\n", (ptrdiff_t)MDrv_Read4Byte(L_BK_LPLL(0x28)))
            }
            LPLL_BK_RESTORE;
        }
        break;
    case FPLL_FREQ_ONLY_LOCK_WAIT_PHASE_STABLE_STATE://Dtv special
        {
            #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            LPLL_BK_STORE;
            LPLL_BK_SWITCH(0x00);
            if ((MDrv_Read4Byte(L_BK_LPLL(0x3D)) < FPLL_PHASE_DIFF_MAX))
            {
                _MApi_XC_FPLL_AdjustVtt_ForPhaseLock(pInstance, FALSE);
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_LOCKED_STATE;
            }
            LPLL_BK_RESTORE;
            #endif
        }
        break;
    case FPLL_LOCKED_STATE://Dtv special
        {
            #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
            {
                Hal_XC_FRC_R2_Set_FPLL_Lockdone(pInstance, TRUE);
            }
            #endif
            if(pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR
                && pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
                && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
            {
                MS_U32 u32VPeriod = 0;
                if(_MApi_XC_FPLL_IsIvsFreqStable(pInstance, &u32VPeriod))
                {
                //Wait some vsync, then turn on MCNR
                    if(_MDrv_SC_FPLL_GetMCNRDelayTime_For_ISR( pInstance, u32VPeriod) < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer))
                {
                    if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
                    {
#if (!ENABLE_REGISTER_SPREAD)
                        u8Bank = MDrv_ReadByte(BK_SELECT_00);
#endif
                        Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
#if (!ENABLE_REGISTER_SPREAD)
                        MDrv_WriteByte(BK_SELECT_00, u8Bank);
#endif
                    }
                    pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
                    pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
                }
            }

            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_DEATTACH_ISR_STATE;
            }
         }
         break;

    case FPLL_DEATTACH_ISR_STATE:
        {
            //switch bank back
            //MS_BOOL bret;
            //u8Bank = MDrv_ReadByte(BK_SELECT_00);
            //bret = MDrv_XC_InterruptDeAttachWithoutMutex(SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &bFPLL_FSM);
            //MDrv_WriteByte(BK_SELECT_00, u8Bank);
            // If we cannot lock, the bank31_0c will be 0x48, we clear it to 0x08,
            // which is the same with the Non-FSM case.

           LPLL_BK_STORE;
            LPLL_BK_SWITCH(0x00);
            MDrv_WriteRegBit(L_BK_LPLL(0x0C), FALSE, BIT(6));
            LPLL_BK_RESTORE;
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
            XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR, "FSM ISR: de-attached ISR result=%u, and reset FSM to Idle\n",
                MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM));
        }
        break;

    default:
        break;
    }
}

static void _MApi_XC_FPLL_ThreshMode_ISR(SC_INT_SRC eIntNum, void *pParam)
{
    void *pInstance = pu32XCInst_private;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(eIntNum >= MAX_SC_INT)
    {
        printf("Interrupt number >= max scaler interrupt number\n");
    }
    if(pParam == NULL)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING_ISR,"ISR pParam is NULL\n");
    }

    switch (pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode)
    {
        case FPLL_THRESH_MODE_IDLE_STATE:
        // no signal, do nothing
        break;

        case FPLL_THRESH_MODE_LOCKED_STATE:
        {
            if(pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW] && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
            {
                MS_U32 u32VPeriod = 0;
                if(_MApi_XC_FPLL_IsIvsFreqStable(pInstance, &u32VPeriod))
                {
                //Wait some vsync, then turn on MCNR
                    if(_MDrv_SC_FPLL_GetMCNRDelayTime_For_ISR(pInstance,u32VPeriod) < (MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer))
                {
                    if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
                    {
                        Hal_SC_SetOPWriteOff(pInstance, DISABLE);
                    }
                    pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
                    pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_DEATTACH_ISR_STATE;
                }
            }
            }
            else
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_DEATTACH_ISR_STATE;
            }
        }
        break;

        case FPLL_THRESH_MODE_DEATTACH_ISR_STATE:
        {
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL);
            pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_IDLE_STATE;
        }
        break;

        default:
        break;
    }
}

/*
   Frame lock mechanism should be detected here.
   msAPI_Scaler_FPLL_FSM wait to lock frame only if the
   phase error is small enough.
*/
void msAPI_Scaler_FPLL_FSM_U2(void* pInstance, SCALER_WIN eWindow)
{
    // use _MApi_XC_FPLL_FSM_ISR() now
    UNUSED(eWindow);

    return;
}

void msAPI_Scaler_FPLL_FSM(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_FSM XCArgs;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_FSM, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_SetPanelTiming_FSM(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    //MS_U8   u8Bank;
    MS_U8   u8FRC_In, u8FRC_Out;
    MS_U32  u32HighAccurateInputVFreq = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    XC_DISPLAYOUT_INFO eDisplyout_Info;
    eDisplyout_Info.u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    eDisplyout_Info.u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;

#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        MDrv_WriteByteMask(H_BK_LPLL(0x0D), 0x0, (BIT(1)|BIT(0)));
    }
#endif

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER < 6))
    if(((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ))
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1280)
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height== 720))
    {
        eDisplyout_Info.u16Htt_out = 2200;
        eDisplyout_Info.u16Vtt_out = 1130;
    }
#endif

    //calculate the time of 2 input vsync
   if(pTimingInfo->u16InputVFreq > 0)
        pXCResourcePrivate->stdrvXC_Display.s_u32DelayT = 20000 / pTimingInfo->u16InputVFreq + 20;
   else
        pXCResourcePrivate->stdrvXC_Display.s_u32DelayT = 1;

    MDrv_SC_set_fpll(pInstance, DISABLE, ENABLE, 0);
#if ((HW_DESIGN_4K2K_VER == 6) && FRC_INSIDE)
    if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_FREERUN_UCNR_ENABLE)&&
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled)
    {
        Hal_SC_SetOPWriteOff(pInstance, DISABLE);//If enable UC, no need to disable OP Write
    }
    else
    {
        Hal_SC_SetOPWriteOff(pInstance, ENABLE);//Disable MCNR first, Enable MCNR when FPLL done
    }
#else
        Hal_SC_SetOPWriteOff(pInstance, ENABLE);//Disable MCNR first, Enable MCNR when FPLL done
#endif

    if(!pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug)
    {
        MDrv_Scaler_SetPhaseLimit(pInstance, 0);
    }

    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_S_VOP);
    if (!pTimingInfo->bMVOPSrc)
    {
        // according to customer side test experience, ATV disable framelock point,
        // could get better result.
        //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 0, BIT(1));
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x00 ,BIT(1));
    }
    else
    {
        //MDrv_WriteRegBit(L_BK_S_VOP(0x56), 1, BIT(1));  //Using new ovs_ref
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_56_L, 0x02 ,BIT(1));
    }

    MS_BOOL bTmpFBL = FALSE;
#ifdef UFO_XC_FB_LEVEL
    if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)  || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
    {
        bTmpFBL = FALSE;//FIXME
    }
#else
    bTmpFBL = gSrcInfo[eWindow].bFBL;
#endif

    gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, pTimingInfo->u16InputVFreq,
                                              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                              bTmpFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &gSrcInfo[eWindow].bEnableFPLL,
                                              0,
                                              eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FSM: _u16OutputVFreqAfterFRC = %u\n", gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC );

#ifdef UFO_XC_FB_LEVEL
        if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL)  || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
        {
            bTmpFBL = FALSE;
        }
#else
        bTmpFBL = gSrcInfo[eWindow].bFBL;//FIXME
#endif

    if (bTmpFBL)
    {
        //we don't use FSM flow to do FBL case
        MDrv_XC_SetFreeRunTiming(pInstance);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "---Now it's FBL case, FSM FPLL cannot do it. Just let it freerun.---\n")
    }

    if (gSrcInfo[eWindow].bEnableFPLL == FALSE)
    {
        // if free run, we need to use freerun vfrequency instead of direct calculation
        pTimingInfo->u32HighAccurateInputVFreq = gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC * 100;
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq;
    }
    else
    {
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq * u8FRC_Out / u8FRC_In;
        // 4K1K FRC 120Hz output case
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K1K_120HZ)
        {
            u32HighAccurateInputVFreq = u32HighAccurateInputVFreq / 2;
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            u32HighAccurateInputVFreq = u32HighAccurateInputVFreq / 4;
        }

        #if (HW_DESIGN_4K2K_VER == 6) // for maserati 4K2K@120Hz case
        if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
        && IsVBY1_16LANE((MS_U8)(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)))
        {
            u32HighAccurateInputVFreq = u32HighAccurateInputVFreq / 2;
        }
        #endif
    }

    gSrcInfo[eWindow].Status2.u32HighAccurateOutputVFreqAfterFRC = u32HighAccurateInputVFreq;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32HighAccurateInputVFreq = %tu\n", (ptrdiff_t)u32HighAccurateInputVFreq);

    MDrv_SC_set_Htt_Vtt(pInstance, pTimingInfo, &gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC,u32HighAccurateInputVFreq , &eDisplyout_Info, eWindow);

    //By RD's suggestion, prevent value of lock point is set out of vtt.
    //BK68 vtt patch needs to wait for the vtt change works.
#if defined(PATCH_HW_VTT_LIMITATION)
    if( (MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)&&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE))
    {
         //Check if the tgen output VTT is as expectd.
        MS_U8 u8TimeoutCnt=0;
        while(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK)
              != SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_32_L, VOP_VTT_MASK))
        {
            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
            if(u8TimeoutCnt++ >= 5)
            {
                break;
            }
        }
    }
#endif

    MDrv_SC_Set_LockFreeze_Point(pInstance, pTimingInfo, eDisplyout_Info.u16Vtt_out, eWindow);


    MDrv_SC_set_output_dclk(pInstance, eDisplyout_Info.u16Htt_out,
                            eDisplyout_Info.u16Vtt_out,
                            gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, u32HighAccurateInputVFreq, eWindow);

    _MDrv_Scaler_SetIGainPGain(pInstance, u8FRC_Out, gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, eWindow);

#ifdef FA_1920X540_OUTPUT
    if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
      ((MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_SIDE_BY_SIDE_HALF)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_TOP_BOTTOM)||
       (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FIELD_ALTERNATIVE))&&
      (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE))
    {
#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            MHal_XC_SetVopVttByBK68(pInstance, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2), 0);
            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
        }
        else
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2), VOP_VTT_MASK);
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (psXCInstPri->u32DeviceID == 0)
        {
            SC_W2BYTE(1, REG_SC_BK10_0D_L, VOP_VTT_MASK);
        }
#endif
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        MHal_FRC_SetYTrig(pInstance, 0x88f, 0x890);    //Need to set Y Trig again after adjust vtt
#endif
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "###g_XC_InitData.stPanelInfo.u16DefaultVTotal=%x\n",pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal/2);
    }
#endif
#ifdef TBP_1920X2160_OUTPUT
    if((!pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)&&
        ((gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2P_VSIZE)||
        (gSrcInfo[MAIN_WINDOW].stCapWin.height == DOUBLEHD_1080X2I_VSIZE))&&
        (MDrv_XC_Get_3D_Input_Mode(pInstance, MAIN_WINDOW) == E_XC_3D_INPUT_FRAME_PACKING)&&
        (MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_TOP_BOTTOM))
    {
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            MHal_XC_SetVopVttByBK68(pInstance, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*2), 0);
            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
        }
        else
#endif
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*2), VOP_VTT_MASK);
        }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        if (psXCInstPri->u32DeviceID == 0)
        {
            SC_W2BYTE(1, REG_SC_BK10_0D_L, VOP_VTT_MASK);
        }
#endif
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        MHal_FRC_SetYTrig(pInstance, 0x88f, 0x890);    //Need to set Y Trig again after adjust vtt
#endif
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "###g_XC_InitData.stPanelInfo.u16DefaultVTotal=%x\n",pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*2);
    }
#endif


    /* enable output free-run */
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L, BIT(8), BIT(8));  //output free run
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    //Follow main setting
    if (psXCInstPri->u32DeviceID == 0)
    {
        SC_W2BYTEMSK(1, REG_SC_BK10_1C_L, BIT(8), BIT(8));
    }
#endif
    _MDrv_XC_Set_FPLL_Thresh_Mode();
    if(gSrcInfo[eWindow].bEnableFPLL
#if FRC_INSIDE
        || (Hal_XC_Get_Stgen_Lock_Ip_status(pInstance) &&
        pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
#endif
        )//stgen lock ip under force free run should enable this register bit
    {
        /* Enable/Disable FrameLock */
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15), BIT(15));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0, BIT(15));
    }

    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if ( pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC )
    {
        MDrv_SC_set_fpll(pInstance, gSrcInfo[eWindow].bEnableFPLL, 0, 0);
    }
    else
    #endif
    {
        MS_BOOL bEnable = FALSE;
 //#if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
        bEnable = gSrcInfo[eWindow].bEnableFPLL;
 //#endif
        MDrv_SC_set_fpll(pInstance, bEnable, gSrcInfo[eWindow].bEnableFPLL, 0);
    }

    _MDrv_XC_PQ_Set_FPLL_Thresh_Mode(pInstance);

    pXCResourcePrivate->stdrvXC_Display.s_u32InputVsyncStart = MsOS_GetSystemTime();

    // THIS AREA IS AN EXPERIMENTAL VALUE, so if you encounter anything strange
    // tearing, please try to comment these code blocks.
    // if
    // (1) freerun
    // (2) with non-integer frame rate conversion value
    // we should consider manually control read bank switch
    MS_U16 u16SwitchCnt = 0;
    if(gSrcInfo[eWindow].bEnableFPLL == FALSE)
    {
        if ((u8FRC_In  == 5) && (u8FRC_Out  == 2))
        {
            u16SwitchCnt = 5;
            MDrv_SC_EnableReadBankSwitchControl(pInstance, ENABLE, u16SwitchCnt, eWindow);
        }
        else if ((u8FRC_In  == 2) && (u8FRC_Out  == 1))
        {
            u16SwitchCnt = 2;
            MDrv_SC_EnableReadBankSwitchControl(pInstance, ENABLE, u16SwitchCnt, eWindow);
        }
    }
    else
    {
        //MDrv_SC_EnableReadBankSwitchControl(pInstance, DISABLE, u16SwitchCnt, eWindow);
    }

    if(gSrcInfo[eWindow].bEnableFPLL)
    {
        if(IsSrcTypeDigitalVD(gSrcInfo[eWindow].enInputSourceType))
        {
        #if LOCK_FREQ_ONLY_WITHOUT_LOCK_PHASE
            _MApi_XC_FPLL_EnableLcokFreqOnly(pInstance,FALSE);
        #endif
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_ATV_INIT_STATE;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_INIT_STATE;
        }
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "FPLL disabled, reset FSM to Idle\n")
    }
}

//////////////////////////////////////////////////////----------------

void MApi_XC_SetPanelTiming_NonFSM(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    //MS_U8   u8Bank;
    MS_U8   u8FRC_In, u8FRC_Out;
    MS_U32  u32HighAccurateInputVFreq = 0;
    XC_DISPLAYOUT_INFO eDisplyout_Info;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    eDisplyout_Info.u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    eDisplyout_Info.u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER < 6))
    if(((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
            || (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ))
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width == 1280)
        &&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height == 720))
    {
        eDisplyout_Info.u16Htt_out = 2200;
        eDisplyout_Info.u16Vtt_out = 1130;
    }
#endif

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "bFBL=%u, bMVOPSrc=%u, InputVFreq=%u, InputVtt=%u, bInterlace=%u bFastFL=%u\n",
        gSrcInfo[eWindow].bFBL,
        pTimingInfo->bMVOPSrc,
        pTimingInfo->u16InputVFreq,
        pTimingInfo->u16InputVTotal,
        pTimingInfo->bInterlace,
        pTimingInfo->bFastFrameLock);
    Hal_SC_SetOPWriteOff(pu32XCInst, ENABLE);//Disable MCNR first, Enable MCNR when FPLL done
    gSrcInfo[eWindow].bFastFrameLock = pTimingInfo->bFastFrameLock;

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);           // backup first
    MDrv_SC_set_fpll(pInstance, DISABLE, ENABLE, 0);    // disable FPLL

    MDrv_Scaler_SetPhaseLimit(pInstance, 0);

    //MDrv_WriteByte(H_BK_LPLL(0x05), 0xF0);
    //MDrv_WriteByte(L_BK_LPLL(0x05), 0x22);

#if VERIFY_DISPLAY_FPGA
    MDrv_Write3Byte(L_BK_LPLL(0x06), 0x30000);
    MDrv_Write3Byte(L_BK_LPLL(0x08), 0x30000);
    MDrv_Write2Byte(L_BK_LPLL(0x0A), 0xFFF0);  // limit for lpll phase limit offset; set it to speed up frame lock

    u8FRC_In = 2;
    u8FRC_Out = 5;
    gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC = pTimingInfo->u16InputVFreq * u8FRC_Out/u8FRC_In;

    gSrcInfo[eWindow].bEnableFPLL = TRUE;

#ifdef UFO_XC_FB_LEVEL
    if(gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FB)//FIXME
#else
    if(!gSrcInfo[eWindow].bFBL )
#endif
    {
        MDrv_WriteByte(H_BK_LPLL(0x0C), ((u8FRC_Out-1) << 4) | (u8FRC_In-1));
    }
    else
    {
        // fps_in:fps_out = 1:1  for FBL, only main can use FBL
        MDrv_WriteByte(H_BK_LPLL(0x0C), 0x00);
        MDrv_WriteByte(L_BK_LPLL(0x0E), 0x00);
    }
#else
    MS_BOOL bTmpFBL = FALSE;
#ifdef UFO_XC_FB_LEVEL
        if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL) || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
        {
            bTmpFBL = TRUE;
        }
#else
        bTmpFBL = gSrcInfo[eWindow].bFBL;//FIXME
#endif
    gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, pTimingInfo->u16InputVFreq,
                                              pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
                                              bTmpFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &gSrcInfo[eWindow].bEnableFPLL,
                                              0,
                                              eWindow);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "NON-FSM: _u16OutputVFreqAfterFRC = %u\n", gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC)
    gSrcInfo[eWindow].bDoneFPLL = !gSrcInfo[eWindow].bEnableFPLL;       // if enable FPLL, then not done fpll yet

    if (gSrcInfo[eWindow].bEnableFPLL == FALSE)
    {
        // if free run, we need to use freerun vfrequency instead of direct calculation
        pTimingInfo->u32HighAccurateInputVFreq = gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC * 100;
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq;
    }
    else
    {
        u32HighAccurateInputVFreq = pTimingInfo->u32HighAccurateInputVFreq * u8FRC_Out / u8FRC_In;
    }

    gSrcInfo[eWindow].Status2.u32HighAccurateOutputVFreqAfterFRC = u32HighAccurateInputVFreq;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32HighAccurateInputVFreq = %tu\n", (ptrdiff_t)u32HighAccurateInputVFreq);

    MDrv_SC_set_Htt_Vtt(pInstance, pTimingInfo, &gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC,u32HighAccurateInputVFreq , &eDisplyout_Info, eWindow);

    MDrv_SC_Set_LockFreeze_Point(pInstance, pTimingInfo, eDisplyout_Info.u16Vtt_out, eWindow);

    MDrv_SC_set_output_dclk(pInstance, eDisplyout_Info.u16Htt_out,
                            eDisplyout_Info.u16Vtt_out,
                            gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, 0, eWindow);

    _MDrv_Scaler_SetIGainPGain(pInstance, u8FRC_Out, gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC, eWindow);
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteRegBit(H_BK_VOP(0x1C), 1, BIT(0));  //output free run
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L, BIT(8), BIT(8));  //output free run
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    //Follow main setting
    if (psXCInstPri->u32DeviceID == 0)
    {
        SC_W2BYTEMSK(1, REG_SC_BK10_1C_L, BIT(8), BIT(8));
    }
#endif
    _MDrv_XC_Set_FPLL_Thresh_Mode();

    if (gSrcInfo[eWindow].bEnableFPLL)
    {
        //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
        //MDrv_WriteRegBit(H_BK_IP1F2(0x03), 1, BIT(7));  //enable frame lock
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15), BIT(15));  //enable frame lock

        if (gSrcInfo[eWindow].bFastFrameLock) //MDrv_ReadRegBit(H_BK_LPLL(0x1E), 0x10)
        {
            MDrv_Scaler_FastFrameLock(pInstance, pTimingInfo->u16InputVFreq, gSrcInfo[eWindow].Status2.u16OutputVFreqAfterFRC);
            //MDrv_XC_DisableInputSource(FALSE, MAIN_WINDOW);  //enable Input
        }
        else
        {
            //MDrv_XC_DisableInputSource(FALSE, MAIN_WINDOW);  //enable Input
            HAL_SC_EnableFPLL();
        }
        pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
    }
#if FRC_INSIDE
    //stgen lock ip under force free run should enable this register bit
    if(Hal_XC_Get_Stgen_Lock_Ip_status(pInstance) && pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
    {
        /* Enable/Disable FrameLock */
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15), BIT(15));
    }
#endif
    _MDrv_XC_PQ_Set_FPLL_Thresh_Mode(pInstance);

    #if 0
    u16temp = 0;
    while( !(MDrv_ReadByte(H_BK_LPLL(0x2A)) & BIT0) &&  u16temp<3000 )
    {
        u16temp++;
        MsOS_DelayTask(1);
    }
    printf("[FPLL Speed]Time= %d\n", msAPI_Timer_DiffTimeFromNow(u32TestTimer));
    printf("[FPLL Speed]temp= %d\n", u16temp);
    #endif

    #if 0
    for(u16temp=0; u16temp<20; u16temp++)
    {
        printf("\n%d\n", u16temp);
        printf("[Freeze]Phase dif= 0x%X (%u)\n", MDrv_Read2Byte(L_BK_LPLL(0x11)), MDrv_Read2Byte(L_BK_LPLL(0x11)));
        printf("[Freeze]Phase up= 0x%X (%u)\n", MDrv_ReadByte(L_BK_LPLL(0x12)), MDrv_ReadByte(L_BK_LPLL(0x12)));
        printf("[Freeze]SET using= 0x%X (%u)\n", MDrv_Read2Byte(L_BK_LPLL(0x28))|(MDrv_ReadByte(L_BK_LPLL(0x29))<<16),
                                                 MDrv_Read2Byte(L_BK_LPLL(0x28))|(MDrv_ReadByte(L_BK_LPLL(0x29))<<16));
        MsOS_DelayTask((10000/pTimingInfo->u16InputVFreq));
    }
    #endif
}

#if FRC_INSIDE
void MApi_XC_SetFLock(void *pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    #if (HW_DESIGN_4K2K_VER == 6)
    // For Manhattan, FSC+FRC, 2 TGEN but only 1 LPLL case

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8  u8FRC_In, u8FRC_Out;
    MS_U16 u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    MS_U16 u16InputVFreq = pTimingInfo->u16InputVFreq;
    MS_U16 u16OutputVfreqAfterFRC = 0;
    MS_BOOL bEnableFPLL;
    MS_BOOL bTmpFBL = FALSE;

#ifdef UFO_XC_FB_LEVEL
    if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL) || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
    {
        bTmpFBL = TRUE;
    }
#else
    bTmpFBL = gSrcInfo[eWindow].bFBL;//FIXME
#endif

    u16OutputVfreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, u16InputVFreq,
                                              u16DefaultVFreq,
                                              bTmpFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &bEnableFPLL,
                                              0,
                                              eWindow);
    gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = u16OutputVfreqAfterFRC;

    //use high accurate output Vfreq, or FRC output timing maybe not accurate for MEMC.
    //example input 29.97Hz: FRC output 239Hz calculate by low accurate Vfreq,  FRC output 240Hz calculate by high accurate Vfreq
    u16OutputVfreqAfterFRC = (MS_U16)((gSrcInfo[eWindow].Status2.u32HighAccurateOutputVFreqAfterFRC+50)/100);

    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out != E_XC_3D_OUTPUT_FI_MODE_NONE)
    {
        Hal_XC_FRC_R2_Set_Timing(pInstance, (u16InputVFreq+5)/10, (u16OutputVfreqAfterFRC+5)/10);
    }
    else
    {
#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
        {
            Hal_XC_FRC_R2_Set_Timing(pInstance, (u16OutputVfreqAfterFRC+5)/10, (u16OutputVfreqAfterFRC+5)/10);
        }
        else
#endif
        {
            Hal_XC_FRC_R2_Set_Timing(pInstance, (u16InputVFreq+5)/10, (u16OutputVfreqAfterFRC+5)/10);
        }
    }
    #else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8  u8FRC_In, u8FRC_Out;
    MS_U16 u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    MS_U16 u16InputVFreq = pTimingInfo->u16InputVFreq;
    MS_U16 u16OutputVfreqAfterFRC = 0;

    // For error handling
    if (u16InputVFreq == 0)
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
            u16InputVFreq = 10;
        else
            u16InputVFreq = 5;
    }

    MS_U32 u32DClk;
    MS_U32 u32LpllLimitLow=0x10000;
    MS_U32 u32LpllLimitHigh=0x10000;
    MS_BOOL bEnableFPLL;
    MS_U16 u16HTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    MS_U16 u16VTotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;

    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE)
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq / 2;
    else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_60HZ)
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;

    // For 4K2K used
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K))
    {
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq / 2;
    }
    else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
    {
        // Make OP timing 4K x 0.5K x 120Hz
        u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq * 2;
        u16VTotal = u16VTotal /4;
    }
    else if ((u16HTotal > 3000) && (u16VTotal > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
        {
            // FHD 50/60 in case
            u16HTotal = u16HTotal/2;
        }
        else
        {
            // 4K2K 24in/30in case
            u16DefaultVFreq = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq / 2;
        }
    }

    if(MDrv_XC_Get_3D_Output_Mode(pInstance) == E_XC_3D_OUTPUT_FRAME_ALTERNATIVE)
    {
        if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_960x1080)
        {
            u16HTotal /= 2;
            u16InputVFreq *=2 ;
        }
        else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1920x540)
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_240HZ)
            {
                u16VTotal = u16VTotal;
                u16InputVFreq = u16InputVFreq;
            }
            else
            {
                u16VTotal = u16VTotal/2;
                u16InputVFreq *=2 ;
            }
        }
        else if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out == E_XC_3D_OUTPUT_FI_1280x720)
        {
            u16InputVFreq *=2 ;
        }
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "g_XC_Pnl_Misc.FRCInfo.u83D_FI_out=%u\n",
            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out);

        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16HTotal=%u, u16VTotal=%u, u16InputVFreq=%u\n",
            u16HTotal, u16VTotal, u16InputVFreq);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16HTotal=%u, u16VTotal=%u, u16InputVFreq=%u\n",
        u16HTotal, u16VTotal, u16InputVFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u16DefaultVFreq=%u, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq=%u\n",
        u16DefaultVFreq, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq);


    MDrv_SC_set_fpll(pInstance, DISABLE, ENABLE, 1);
    MDrv_Scaler_SetPhaseLimit(pInstance, 1);

#ifdef UFO_XC_FB_LEVEL
    if((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FBL) || (gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
    {
        bTmpFBL = TRUE;
    }
#else
    bTmpFBL = gSrcInfo[eWindow].bFBL;//FALSE
#endif

    u16OutputVfreqAfterFRC = MDrv_SC_Cal_FRC_Output_Vfreq(pInstance, u16InputVFreq,
                                              u16DefaultVFreq,
                                              bTmpFBL,
                                              &u8FRC_In,
                                              &u8FRC_Out,
                                              &bEnableFPLL,
                                              1,
                                              eWindow);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@========\n")
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetFlock: u16InputVFreq = %u,u16OutputVfreqAfterFRC = %u\n",
                                                   u16InputVFreq, u16OutputVfreqAfterFRC)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetFlock: u8FRC_In = %u,u8FRC_Out = %u, bEnableFPLL=%x\n",
                                                   u8FRC_In, u8FRC_Out, bEnableFPLL)

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "@@========\n")


    if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            MHal_XC_SetVopVttByBK68(pInstance, (u16VTotal - 1)&VOP_VTT_MASK, 1);
            _MLOAD_RETURN(pInstance);
            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
            _MLOAD_ENTRY(pInstance);
        }
        else
#endif
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0D_L,(u16VTotal - 1)&VOP_VTT_MASK, VOP_VTT_MASK);
        }
        MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0C_L,(u16HTotal - 1), VOP_HTT_MASK);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        //FIX ME:follow HW_DESIGN_4K2K_VER = 6 rule
        if (psXCInstPri->u32DeviceID == 0)
        {
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_0D_L,VOP_VTT_MASK, VOP_VTT_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_0C_L,(u16HTotal/2 - 1), VOP_HTT_MASK);
        }
#endif


        // 4K2K 60 out case
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K)
        {
            // Kepp OP timing as 4K2K, DE for bk10 is 4Kx2K
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1), VOP_DE_VEND_MASK);  // DE V end
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            // Kepp OP timing as 4K 0.5K, DE for bk10 is 4Kx0.5K
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/4 - 1), VOP_DE_VEND_MASK);  // DE V end
        }
        else if ((u16VTotal > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
        {
            //if ((gSrcInfo[eWindow].u16InputVFreq != 0) && (u16InputVFreq > 450))
            // The condition above cause H scale problem on both video & OSD with FHD 24/25/30Hz
            // We need more restrict & accurate condition to tell the different between UHD & FHD input.
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
            {
                // FHD in, DE for bk10 is 2Kx2K
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 - 1), VOP_DE_HEND_MASK);
            }
            else
            {
                // 4K2K in, DE for bk10 is 4Kx2K
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1), VOP_DE_HEND_MASK);
            }
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1), VOP_DE_VEND_MASK);  // DE V end
        }

        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
#if defined(PATCH_HW_VTT_LIMITATION)
        if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
        {
            MHal_XC_SetVopVttByBK68(pInstance, (u16VTotal - 1)&VOP_VTT_MASK, 0);
            MDrv_XC_WaitOutputVSync(pInstance, 1, 50, MAIN_WINDOW);
        }
        else
#endif
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L,(u16VTotal - 1)&VOP_VTT_MASK);
        }
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L,(u16HTotal - 1));
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
        //FIX ME:follow HW_DESIGN_4K2K_VER = 6 rule
        if (psXCInstPri->u32DeviceID == 0)
        {
            SC_W2BYTE(1, REG_SC_BK10_0D_L,VOP_VTT_MASK);
            SC_W2BYTE(1, REG_SC_BK10_0C_L,(u16HTotal/2 - 1));
        }
#endif

        // 4K2K 60 out case
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_KEEP_OP_4K2K)
        {
            // Keep OP timing as 4K2K, DE for bk10 is 4Kx2K
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1));
        }
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_A & E_XC_INIT_MISC_A_FRC_INSIDE_4K_HALFK_240HZ)
        {
            // Kepp OP timing as 4K 0.5K, DE for bk10 is 4Kx0.5K
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height/4 - 1));  // DE V end
        }
        else if ((u16VTotal > 2000) && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 500))
        {
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.enFRC_CurrentInputTiming == E_XC_FRC_InputTiming_2K1K)
            {
                // FHD in, DE for bk10 is 2Kx2K
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width/2 - 1));
            }
            else
            {
                // 4K2K in, DE for bk10 is 4Kx2K
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1));
            }
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height- 1));
        }
    }

    u32DClk = u16HTotal * u16VTotal * u16OutputVfreqAfterFRC;
    if((u16InputVFreq >= 235 )&&(u16InputVFreq <= 245 ))
        MDrv_Scaler_SetPhaseLock_Thresh(0x0F, 1);
    else
        MDrv_Scaler_SetPhaseLock_Thresh(0x02, 1);

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Output DCLK = %lu\n\n", u32DClk);
    _MHal_SC_Flock_Caculate_LPLLSet(u32DClk);
    _MHal_SC_Flock_Set_IGainPGain(pInstance, u8FRC_Out, u16OutputVfreqAfterFRC, u16HTotal, u16VTotal);
    _MHal_SC_Set_LPLL_Limit(u32LpllLimitHigh, u32LpllLimitLow, 1);

    MDrv_SC_set_fpll(pInstance, bEnableFPLL, !bEnableFPLL, 1);

    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "Input Freq = %u, After FRC = %u\n\n", u16InputVFreq, u16OutputVfreqAfterFRC);

    gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = u16OutputVfreqAfterFRC;


    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.u83D_FI_out != E_XC_3D_OUTPUT_FI_MODE_NONE)
    {
        Hal_XC_FRC_R2_Set_Timing(pInstance, (u16OutputVfreqAfterFRC+5)/10, (gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC+5)/10);
    }
    else
    {
        Hal_XC_FRC_R2_Set_Timing(pInstance, (u16OutputVfreqAfterFRC+5)/10, (gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC+5)/10);
    }
    #endif
}

#endif

//-------------------------------------------------------------------------------------------------
/// Set the FPLL mode:
/// @param  bTrue      \b IN: True: FSM mode, False: Non FSM mode
//-------------------------------------------------------------------------------------------------

void MApi_SC_Enable_FPLL_FSM_U2(void* pInstance, MS_BOOL bTrue)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting = bTrue;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_SC_Enable_FPLL_FSM(MS_BOOL bTrue)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_FSM_ENABLE XCArgs;
    XCArgs.bTrue = bTrue;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_FSM_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set the FPLL Thresh mode:
/// @param  bEnable      \b IN: TRUE: Thresh mode, FALSE: Non-Thresh mode
//-------------------------------------------------------------------------------------------------

void MApi_XC_Enable_FPLL_Thresh_Mode_U2(void* pInstance, MS_BOOL bEnable)
{
#if FPLL_THRESH_MODE_SUPPORT
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
#endif
}

void MApi_XC_Enable_FPLL_Thresh_Mode(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_THRESHMODE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_THRESHMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get the FPLL Thresh mode:
/// @return TRUE: Thresh mode, FALSE: Non-Thresh mode
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_Get_FPLL_Thresh_Mode_U2(void* pInstance)
{
#if FPLL_THRESH_MODE_SUPPORT
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    bReturn = pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
#else
    return FALSE;
#endif
}

MS_BOOL MApi_XC_Get_FPLL_Thresh_Mode(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FPLL_THRESHMODE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FPLL_THRESHMODE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Setup panel timing (output DCLK)/FRC/FramePLL, and enable input source
/// @param  pTimingInfo      \b IN: the information of the timing to set @ref XC_SetTiming_Info
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPanelTiming_U2(void* pInstance, XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
#ifdef MSOS_TYPE_LINUX_KERNEL
    /*
    ** Save setpaneltiming info for str
    */
    void *pModule;
    UtopiaInstanceGetModule(pInstance, &pModule);
    XC_REGS_SAVE_AREA *pXCResourceStr = NULL;
    UtopiaModuleGetSTRPrivate(pModule, (void**)&pXCResourceStr);
    memcpy(&pXCResourceStr->pTimingInfo,pTimingInfo,sizeof(XC_SetTiming_Info));
#endif
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("MApi_XC_EX_SetPanelTiming() -> Not support for device id %td\n", (ptrdiff_t)psXCInstPri->u32DeviceID);
        return;
    }
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);

    #ifdef ENABLE_4K2K_PATCH
    if(MDrv_Read2Byte(L_BK_LPLL(0x10))<= 0x20)
    {
        _XC_RETURN(pInstance);
        _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
        return;
    }
    #endif

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U8 u8LPLL_Type = 0;
    u8LPLL_Type =(MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type;

    if(pTimingInfo->u32HighAccurateInputVFreq == 0)
    {
        if(IsSrcTypeDigitalVD(gSrcInfo[eWindow].enInputSourceType))
        {
            pTimingInfo->u32HighAccurateInputVFreq = MDrv_XC_GetAccurateVFreqx1K(pInstance, eWindow);
        }
        else if(IsSrcTypeAnalog(gSrcInfo[eWindow].enInputSourceType) || IsSrcTypeHDMI(gSrcInfo[eWindow].enInputSourceType))
        {
            pTimingInfo->u32HighAccurateInputVFreq = MDrv_XC_PCMonitor_Get_VFreqx1K(pInstance, eWindow);
        }
        else
        {
            pTimingInfo->u32HighAccurateInputVFreq = pTimingInfo->u16InputVFreq * 100;
        }
    }

    if(IsSrcTypeAV(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow])
        ||IsSrcTypeSV(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow])
        ||IsSrcTypeATV(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow])
        ||IsSrcTypeScart(pXCResourcePrivate->stdrvXC_MVideo.enOldInputSrc_ForScalerSetSource[eWindow]))
    {
        if(abs(pTimingInfo->u16InputVFreq  - pTimingInfo->u32HighAccurateInputVFreq / 100) > ACCURATE_VFREQ_TOLERANCE_FOR_VD_SOURCE)
        {
            printf("[%s,%5d]Attention! using u16InputVFreq=%u instead of u32HighAccurateInputVFreq=%tu\n", __FUNCTION__, __LINE__, pTimingInfo->u16InputVFreq, (ptrdiff_t)pTimingInfo->u32HighAccurateInputVFreq);
            pTimingInfo->u32HighAccurateInputVFreq = pTimingInfo->u16InputVFreq * 100;
        }
    }
    else
    {
        if(abs(pTimingInfo->u16InputVFreq  - pTimingInfo->u32HighAccurateInputVFreq / 100) > ACCURATE_VFREQ_TOLERANCE)
        {
            printf("[%s,%5d]Attention! using u16InputVFreq=%u instead of u32HighAccurateInputVFreq=%tu\n", __FUNCTION__, __LINE__, pTimingInfo->u16InputVFreq, (ptrdiff_t)pTimingInfo->u32HighAccurateInputVFreq);
            pTimingInfo->u32HighAccurateInputVFreq = pTimingInfo->u16InputVFreq * 100;
        }
    }

#ifdef UFO_XC_FB_LEVEL
    if((pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting)
        && ((gSrcInfo[eWindow].eFBLevel == E_XC_FB_LEVEL_FB)
          || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)
    )
#else
    if((pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Setting) && (((!gSrcInfo[eWindow].bFBL) && (!gSrcInfo[eWindow].bR_FBL)) || gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode))
#endif
    {
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE; //FSM will restart, so clear FSM status to idle
        pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working = TRUE;
        MApi_XC_SetPanelTiming_FSM(pInstance, pTimingInfo, eWindow);
        if (!pXCResourcePrivate->stdrvXC_Display._bFPLL_Thresh_Mode)
        {
            if(gSrcInfo[eWindow].bEnableFPLL)
            {
                //FSM enable fpll, then enable ISR
                if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM))
                {
                    MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
                }
            }
            else
            {
                //FSM disable FPLL, then disable ISR
                if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM))
                {
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
                }
            }
        }
        else
        {
            if(gSrcInfo[eWindow].bEnableFPLL)
            {
                pXCResourcePrivate->stdrvXC_Display._enFpllThreshMode = FPLL_THRESH_MODE_LOCKED_STATE;
                pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer = MsOS_GetSystemTime();
                pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
                Hal_SC_SetOPWriteOff(pInstance, ENABLE);

                //FSM enable fpll, then enable ISR
                if(FALSE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL))
                {
                    MDrv_XC_InterruptAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL);
                }
            }
            else
            {
                //FSM disable FPLL, then disable ISR
                if(TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL))
                {
                    MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_ThreshMode_ISR, NULL);
                }
            }
        }
    }
    else
    {
        if(pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working &&
           (TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM)))
        {
            //State change from FSM to NON-FSM, disable ISR
            MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
            pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
        }
        pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working= FALSE;
        MApi_XC_SetPanelTiming_NonFSM(pInstance, pTimingInfo, eWindow);
    }
#if FRC_INSIDE
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        MApi_XC_SetFLock(pInstance, pTimingInfo, eWindow);
    }
    else
    {
        gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;
    }

    #if (HW_DESIGN_4K2K_VER == 6)
    // For Manhattan, FSC+FRC
    // 1. load FSC Qmap here
    // 2. load FRC 3D table here
    // 3. send R2 cmd to FRC
    // 4. control FRC clk
    // 5. control ODCLK2, composer, and TGEN lock_src
    if (eWindow == MAIN_WINDOW)
    {
        MS_BOOL bUseMenuload = FALSE;
        if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        {
            _MLOAD_ENTRY(pInstance);
            bUseMenuload = TRUE;
        }

        // Maserati mode
        if (IsVBY1_16LANE(u8LPLL_Type))
        {

            MS_BOOL bEnable_MEMC = TRUE;
            if((gSrcInfo[eWindow].bEnableFPLL && (gSrcInfo[MAIN_WINDOW].stCapWin.height < pTimingInfo->u16InputVTotal))
#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case , uses lockfromOP
                    && (!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE))
#endif
               #ifndef MAXIM_ATV_LOCK_PHASE_PATCH
                    && (!IsSrcTypeATV(gSrcInfo[eWindow].enInputSourceType))  //atv ip timing may be unstable under some use case,cant sync from ip
               #endif
               )
            {
                bEnable_MEMC = TRUE;
            }
            else
            {
                bEnable_MEMC = FALSE;
            }

            if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled) &&(psXCInstPri->u32DeviceID != 1))
            {
                // case 5: FSC + FRC
                MHal_FRC_Set_FSC_DE_Selection(pInstance, E_FRC_FSC_SOURCE_MODE_FDE);

#if 0
                if(MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance))
                {
                    MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_2, bUseMenuload);
                }
#endif
                if (MHal_FRC_IsFHDToFSC(pInstance, eWindow))
                {
                    // FHD case, ODCLK2 as ODCLK/4
                    MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_4, bUseMenuload);
                }
                else
                {
                    // 4K 50/60 case, ODLCK2 as ODCLK
                    MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_1, bUseMenuload);
                }
                //MHal_FRC_Set_Pipe_Delay_Reset(pInstance, ENABLE, bUseMenuload);

                // For FSC V12L en control
                MHal_FRC_AdjustGOPPosition(pInstance);

                if (MHal_FRC_IsFHDToFSC(pInstance, eWindow))
                {
                    MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FSC_FHD, FRC_PIPE_DELAY_VCNT_FSC_FHD);
                }
                else
                {
                    MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FSC_4K_120Hz, FRC_PIPE_DELAY_VCNT_FSC_4K_120Hz);
                }

                MHal_FRC_ByPass_Enable(pInstance, FALSE);
                MHal_FRC_Set_Composer_User_Mode(pInstance, E_FRC_COMPOSER_SOURCE_MODE_FSC, bUseMenuload);
                MHal_FRC_TGEN_Enable_Lock_Source(pInstance, ENABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, bUseMenuload);

            }
            else
            {
                // case 4: no FSC + FRC
                //MHal_FRC_Set_Pipe_Delay_Reset(pInstance, DISABLE, bUseMenuload);

                MHal_FRC_ByPass_Enable(pInstance, FALSE);  // enable FRC
                MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_1, bUseMenuload);

                MHal_FRC_Set_Composer_User_Mode(pInstance, E_FRC_COMPOSER_SOURCE_MODE_VIP, bUseMenuload);
                MHal_FRC_Set_FSC_DE_Selection(pInstance, E_FRC_FSC_SOURCE_MODE_MDE);
                MHal_FRC_TGEN_Enable_Lock_Source(pInstance, DISABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, bUseMenuload);

            }
            if(bEnable_MEMC)
            {
                Hal_XC_FRC_R2_Enable_MEMC(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16MEMCLevel);
            }
            else
            {
                Hal_XC_FRC_R2_Enable_MEMC(pInstance, MEMC_LEVEL_OFF);
            }
        }
        else // Manhattan mode
        if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled) &&(psXCInstPri->u32DeviceID != 1))
        {
            MS_BOOL bLockFromIP = FALSE;

            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled &&
               ((gSrcInfo[eWindow].bEnableFPLL
#if FRC_INSIDE
                || (IS_OUTPUT_4K1K_120HZ_PANEL() && //4k1k@120 3d stgen lock ip case
                (MDrv_XC_Get_3D_Output_Mode(pInstance) != E_XC_3D_OUTPUT_MODE_NONE) &&
                pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
#endif
                ) && (gSrcInfo[MAIN_WINDOW].stCapWin.height < pTimingInfo->u16InputVTotal))
#if (HW_DESIGN_HDMITX_VER == 1)//Maserati +Raptor HDMITX case , uses lockfromOP
                && (!(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE))
#endif
               #ifndef MAXIM_ATV_LOCK_PHASE_PATCH
               && (!IsSrcTypeATV(gSrcInfo[eWindow].enInputSourceType))  //atv ip timing may be unstable under some use case,cant sync from ip
               #endif
               )
            {
                bLockFromIP = TRUE;
            }

            // Lock from OP and input Vfreq != output Vfreq, or input Vfeq > Output Vfreq, means it's not 1:1 case, can not enable UC.
            if((!bLockFromIP &&
               (abs(gSrcInfo[MAIN_WINDOW].u16InputVFreq - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq) > VFREQ_DIFF_TOLERANCE))
               || ((gSrcInfo[MAIN_WINDOW].u16InputVFreq - pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq) > VFREQ_DIFF_TOLERANCE))
            {
                SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2A_02_L, 0x0 ,BIT(15));//disable UC feature
            }

            MHal_FRC_Set_FSC_DE_Selection(pInstance, E_FRC_FSC_SOURCE_MODE_FDE);

#if 0
            if(MDrv_XC_Is_DSForceIndexEnabled(pInstance, MAIN_WINDOW)||MDrv_XC_GetDynamicScalingStatus(pInstance))
            {
                MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_2, bUseMenuload);
            }
#endif
            if (MHal_FRC_IsFHDToFSC(pInstance, eWindow))
            {
                // FHD case, ODCLK2 as ODCLK/4
                MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_4, bUseMenuload);
            }
            else if ((gSrcInfo[eWindow].u16InputVFreq < 330) && bLockFromIP
                  && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq > 450))
            {
                // 4K 24/25/30 + output 50/60 case, ODCLK2 as ODCLK/2 for power saving
                MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_2, bUseMenuload);
            }
            else
            {
                // 4K 50/60 case, ODLCK2 as ODCLK
                //For Raptor output 576p/480p case (Vby1 1 lane), xc odclk divided by N, so the stgen has to follow the same setting.
                if( (u8LPLL_Type == E_XC_PNL_LPLL_VBY1_8BIT_1LANE)||(u8LPLL_Type == E_XC_PNL_LPLL_VBY1_10BIT_1LANE))
                    MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_N, bUseMenuload);
                else
                    MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_1, bUseMenuload);
            }
            if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled)
            {
                // case3: FSC+FRC
                //MHal_FRC_Set_Pipe_Delay_Reset(pInstance, ENABLE, bUseMenuload);
                MHal_FRC_AdjustGOPPosition(pInstance);

                #ifdef SUPPORT_FHD_MEMC
                if (IS_OUTPUT_FHD(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height))
                {
                    MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FRC_FHD_OUT, FRC_PIPE_DELAY_VCNT_FRC_FHD_OUT);
                }
                else
                #endif
                {
                    #ifdef HW_FRC_PIPE_DELAY_DTV_4K2K_LIMITATION
                    if(IsSrcTypeDTV(gSrcInfo[eWindow].enInputSourceType)
                    && !(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
                    )
                    {
                         MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FRC_DTV,FRC_PIPE_DELAY_VCNT_FRC_DTV);
                    }
                    else
                    #endif
                    {
                    #if (HW_DESIGN_HDMITX_VER == 1) //Maserati case, fix V pipedelay to 0x1D
                        MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FSC_FHD, FRC_PIPE_DELAY_VCNT_FSC_FHD);
                    #else
                        MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FRC, FRC_PIPE_DELAY_VCNT_FRC);
                    #endif
                    }
                }

                MHal_FRC_ByPass_Enable(pInstance, FALSE);
                MHal_FRC_Set_Composer_User_Mode(pInstance, E_FRC_COMPOSER_SOURCE_MODE_FRC, bUseMenuload);

                // SW patch for LPLL not locked case:
                // at FRC enable case, only FPLL locked can set lock from IP
                if (bLockFromIP)
                {
                    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_FREERUN_UCNR_ENABLE)
                    {
                        pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16MEMCLevel = MEMC_LEVEL_OFF;
                    }
                    MHal_FRC_TGEN_Enable_Lock_Source(pInstance, ENABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_IP, bUseMenuload);
                    MS_U16 u16MEMCLevel = 0;
#if FRC_INSIDE
                    /*force free run should off memc*/
                    if(pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
                    {
                        printf("[%s,%5d],force free run should disable memc \n",__FUNCTION__,__LINE__);
                        u16MEMCLevel = MEMC_LEVEL_OFF;
                    }
                    else
#endif
                    {
                        u16MEMCLevel = pXCResourcePrivate->stdrvXC_MVideo_Context.g_u16MEMCLevel;
                    }

                    if(gSrcInfo[MAIN_WINDOW].bUseYUVSpace)
                    {
                        Hal_XC_FRC_R2_Enable_MEMC(pInstance, u16MEMCLevel);
                    }
                    else
                    {
                        Hal_XC_FRC_R2_Enable_MEMC(pInstance, MEMC_LEVEL_BYPASS);
                    }
                }
                else
                {
                    /*when scaler free run,stgen cant lock source for clk is not enough under 4k1k@120hz,4k0.5k@240 3d*/
                    if((IS_OUTPUT_4K1K_120HZ_PANEL()|| IS_OUTPUT_4K0_5K_240HZ_PANEL())
                       && (MDrv_XC_Get_3D_Output_Mode(pInstance) != E_XC_3D_OUTPUT_MODE_NONE))
                    {
                        MHal_FRC_TGEN_Enable_LockMode(pInstance, DISABLE);
                    }
                    else
                    {
                        MHal_FRC_TGEN_Enable_Lock_Source(pInstance, ENABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, bUseMenuload);
                    }
                    if(gSrcInfo[MAIN_WINDOW].bUseYUVSpace)
                    {
                        Hal_XC_FRC_R2_Enable_MEMC(pInstance, MEMC_LEVEL_OFF);
                    }
                    else
                    {
                        Hal_XC_FRC_R2_Enable_MEMC(pInstance, MEMC_LEVEL_BYPASS);
                    }
                }
            }
            else
            {
                // case2: only FSC
                //MHal_FRC_Set_Pipe_Delay_Reset(pInstance, ENABLE, bUseMenuload);
                MHal_FRC_AdjustGOPPosition(pInstance);

                // For RAPTORS case, keep pipe delay value UNCHANGE to avoid video&osd shift.
                // Instead, adjust the position of BK68 tgen vsync start/end, mde v start/end, fde v start/end
                // Raptor case identified by MISC E_XC_INIT_MISC_B_HDMITX_ENABLE and set by Upper layer.
                if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
                    &&(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange == 0)
                    &&(pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange == 0)) //Check if PIXEL FEATURE is NOT enable.
                {
                    MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FSC_FHD, FRC_PIPE_DELAY_VCNT_FSC_FHD);
                }
                else
                {
                    if (MHal_FRC_IsFHDToFSC(pInstance, eWindow))
                    {
                        MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FSC_FHD, FRC_PIPE_DELAY_VCNT_FSC_FHD);
                    }
                    else
                    {
                        MHal_FRC_Set_Pipe_Delay_Value(pInstance, FRC_PIPE_DELAY_HCNT_FSC_4K, FRC_PIPE_DELAY_VCNT_FSC_4K);
                    }
                }

                MHal_FRC_ByPass_Enable(pInstance, TRUE);
                MHal_FRC_Set_Composer_User_Mode(pInstance, E_FRC_COMPOSER_SOURCE_MODE_FSC, bUseMenuload);
                MHal_FRC_TGEN_Enable_Lock_Source(pInstance, ENABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, bUseMenuload);
            }
        }
        else
        {


            // case1: no FSC+ no FRC
            //MHal_FRC_Set_Pipe_Delay_Reset(pInstance, DISABLE, bUseMenuload);
            MHal_FRC_AdjustGOPPosition(pInstance);

            MHal_FRC_ByPass_Enable(pInstance, TRUE);  // disable FRC
            MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_1, bUseMenuload);

            MHal_FRC_Set_Composer_User_Mode(pInstance, E_FRC_COMPOSER_SOURCE_MODE_VIP, bUseMenuload);
            MHal_FRC_Set_FSC_DE_Selection(pInstance, E_FRC_FSC_SOURCE_MODE_MDE);
            MHal_FRC_TGEN_Enable_Lock_Source(pInstance, DISABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, bUseMenuload);
        }

        if(MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
        {
            MDrv_XC_MLoad_Fire(pInstance, TRUE);
            _MLOAD_RETURN(pInstance);
        }

        if(psXCInstPri->u32DeviceID != 1)
        {
            // Maserati mode
            if (IsVBY1_16LANE(u8LPLL_Type))
            {
                MHal_FRC_Set_Mload_Trig_Mode(pInstance, E_FRC_MLOAD_TRIG_MODE_FROM_OP1); // menuload trig always form stgen
            }
            else // Manhattan mode
            {
                if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled)
                {
                    MHal_FRC_Set_Mload_Trig_Mode(pInstance, E_FRC_MLOAD_TRIG_MODE_FROM_OP2); // with FRC case, menuload trig form tgen
                }
                else
                {
                    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled)
                    {
                        MHal_FRC_Set_Mload_Trig_Mode(pInstance, E_FRC_MLOAD_TRIG_MODE_FROM_OP1); // no FRC case, menuload trig form stgen
                    }
                    else
                    {
                        MHal_FRC_Set_Mload_Trig_Mode(pInstance, E_FRC_MLOAD_TRIG_MODE_FROM_OP2); // with FRC case, menuload trig form tgen
                    }
                }
            }
        }
    }
    #endif
#else
    gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OutputVFreqAfterFRC;
#endif

    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetPanelTiming(XC_SetTiming_Info *pTimingInfo, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PANEL_TIMING XCArgs;
    XCArgs.pTimingInfo = pTimingInfo;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PANEL_TIMING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_XC_SetFrameColor(void *pInstance, MS_U32 u32aRGB )
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32FrameColor_Suspend = u32aRGB;
    Hal_XC_SetFrameColor(pInstance, u32aRGB);
}

//-------------------------------------------------------------------------------------------------
/// set free run(Disable input chanel) color
/// @param  u32aRGB
/// -  0:Black
/// -  1:White
/// -  2:Blue
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFrameColor_U2 ( void* pInstance, MS_U32 u32aRGB )
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);

    _XC_ENTRY(pInstance);
    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteByte(H_BK_VOP(0x19), (MS_U8) (u32aRGB >> 16));                ///< R
    //MDrv_WriteByte(L_BK_VOP(0x1A), (MS_U8) (u32aRGB >> 8));                 ///< G
    //MDrv_WriteByte(H_BK_VOP(0x1A), (MS_U8) u32aRGB);                        ///< B
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_19_L,  (u32aRGB >> 8),0xFF00);            ///< R
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1A_L,  (u32aRGB >> 8),0x00FF);            ///< G
    //SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1A_L,  (u32aRGB << 8),0xFF00);            ///< B
    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
    MDrv_XC_SetFrameColor(pInstance, u32aRGB);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

}

void MApi_XC_SetFrameColor ( MS_U32 u32aRGB )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FRAME_COLOR XCArgs;
    XCArgs.u32aRGB = u32aRGB;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FRAME_COLOR, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
///set free run display window size
/******************************************************************************/
void MDrv_SC_set_freerun_window (void *pInstance)
{
    //MS_U8 u8Bank;

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);

    /*MDrv_Write2Byte(L_BK_VOP(0x08), g_XC_InitData.stPanelInfo.u16HStart); // Image H start
    MDrv_Write2Byte(L_BK_VOP(0x09), g_XC_InitData.stPanelInfo.u16HStart + g_XC_InitData.stPanelInfo.u16Width - 1); // Image H end
    MDrv_Write2Byte(L_BK_VOP(0x0A), g_XC_InitData.stPanelInfo.u16VStart + g_XC_InitData.stPanelInfo.u16Height - 1); // Image V end
    MDrv_Write2Byte(L_BK_VOP(0x0B), g_XC_InitData.stPanelInfo.u16VStart + g_XC_InitData.stPanelInfo.u16Height - 1); // DE V end*/

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart); // Image H start
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width - 1); // Image H end
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1); // Image V end
    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height - 1); // DE V end*/

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

//-----------------------------------------------------------------------------
// frame lock
//-----------------------------------------------------------------------------
static void MDrv_Scaler_SetFrameLock(void *pInstance, MS_BOOL bSignal)
{
    //MS_U8 u8Bank;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U16 u16OutputVtotal = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal;

    MS_BOOL bFramelockEnable = DISABLE;
    MS_BOOL bEnFreerunOutput = DISABLE;
    MS_BOOL bUseMainFB = TRUE;

    MS_BOOL bSetFPLL = DISABLE;

#ifdef UFO_XC_FB_LEVEL
    if((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL) || (gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
    {
        bUseMainFB = FALSE;//FIXME
    }
#else
    bUseMainFB = !gSrcInfo[MAIN_WINDOW].bFBL;
#endif

    //u8Bank = MDrv_ReadByte(BK_SELECT_00);

    if (bSignal == FALSE) // No Signal (Pure Freerun Mode)
    {
        u16OutputVtotal -= 1;
        bEnFreerunOutput = ENABLE;
    }
    else // Have Signal
    {
        if (bUseMainFB)
        {
            bSetFPLL = ENABLE;
            u16OutputVtotal -= 1;
            bEnFreerunOutput = ENABLE;

            bFramelockEnable = ENABLE;
        }
        else
        {
            bSetFPLL = DISABLE;
            u16OutputVtotal -= 1;
            bEnFreerunOutput = ENABLE;

            bFramelockEnable = DISABLE;
        }
    }

    /* set FPLL */
    MDrv_SC_set_fpll(pInstance, bSetFPLL, DISABLE, 0);
#if FRC_INSIDE
    #if (HW_DESIGN_4K2K_VER != 6) // For Manhattan, only 1 LPLL
    MDrv_SC_set_fpll(pInstance, bSetFPLL, DISABLE, 1);
    #endif
#endif

    /* enable output free-run */
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    //MDrv_WriteByte(H_BK_VOP(0x1C), bEnFreerunOutput);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L, bEnFreerunOutput ? BIT(8): 0,BIT(8));
    /* Enable/Disable FrameLock */
    //MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    //MDrv_WriteRegBit(H_BK_IP1F2(0x03), bFramelockEnable, BIT(7));
    if(bFramelockEnable
#if FRC_INSIDE
        || (Hal_XC_Get_Stgen_Lock_Ip_status(pInstance) &&
        pXCResourcePrivate->stdrvXC_Display._bForceFreerun)
#endif
        )//stgen lock ip should enable this bit under force free run
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15),BIT(15));
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, 0,BIT(15));
    }


    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

//-------------------------------------------------------------------------------------------------
/// set free run display window timing
//-------------------------------------------------------------------------------------------------
void MDrv_XC_SetFreeRunTiming (void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working &&
       (TRUE == MDrv_XC_InterruptIsAttached(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM)))
    {
        //FSM, disable FPLL, so disable ISR
        MDrv_XC_InterruptDeAttachWithoutMutex(pInstance, SC_INT_VSINT, _MApi_XC_FPLL_FSM_ISR, (void *) &pXCResourcePrivate->stdrvXC_Display.bFPLL_FSM);
        pXCResourcePrivate->stdrvXC_Display._enFpllFsm = FPLL_IDLE_STATE;
    }

    MDrv_WriteByte(H_BK_LPLL(0x0C), 0x00); //clear FRC before set freerun (fps_in:fps_out = 1:1)
    MDrv_WriteByte(L_BK_LPLL(0x0E), 0x00);

#if VERIFY_DISPLAY_FPGA  //value setting
    MDrv_SC_set_output_dclk(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal,
                            120, eWindow);
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SetFreeRunTiming: Disable FPLL and Clear FSM to Idle state\n")
    gSrcInfo[MAIN_WINDOW].bEnableFPLL = FALSE;
    gSrcInfo[SUB_WINDOW].bEnableFPLL = FALSE;
    MDrv_Scaler_SetFrameLock(pInstance, FALSE);

    pXCResourcePrivate->stdrvXC_Display.u32OutputPhaseStable = FALSE;
    pXCResourcePrivate->stdrvXC_Display.u32OutputPRDLock     = FALSE;
    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        Hal_XC_FRC_R2_Set_FPLL_Lockdone(pInstance, FALSE);
    }
    #endif

    switch (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type)
    {
        case E_XC_PNL_LPLL_DAC_I:
        case E_XC_PNL_LPLL_DAC_P:
            //MDrv_WriteByteMask(H_BK_VOP(0x10), BIT(7)|BIT(6), 0xFF); // set output vsync manually
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(15)|BIT(14), 0xFF00); // set output vsync manually
            break;
        default:
            //MDrv_WriteByteMask(H_BK_VOP(0x10), BIT(6), 0x7F); // set output sync ctl for Freerun
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L, BIT(14), 0x7700); // set output sync ctl for Freerun, bit 11 per RD's suggestion take it to be the default
    }

#if ((HW_DESIGN_4K2K_VER == 6) && (FRC_INSIDE))
    if ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFSCEnabled) &&(psXCInstPri->u32DeviceID != 1))
    {
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bFRCEnabled)
        {
            if (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW))
            {
                // FHD case, ODCLK2 as ODCLK/4
                MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_4, FALSE);
            }
            else
            {
                // 4K 50/60 case, ODLCK2 as ODCLK
                MHal_FRC_Set_STGEN_ODCLK(pInstance, E_FRC_STGEN_ODCLK_1, FALSE);
            }
            /*when scaler free run,stgen cant lock source for clk is not enough under 4k1k@120hz,4k0.5k@240 3d*/
            if((IS_OUTPUT_4K1K_120HZ_PANEL() || IS_OUTPUT_4K0_5K_240HZ_PANEL())
               && (MDrv_XC_Get_3D_Output_Mode(pInstance) != E_XC_3D_OUTPUT_MODE_NONE))
            {
                MHal_FRC_TGEN_Enable_LockMode(pInstance, DISABLE);
            }
            else
            {
                MHal_FRC_TGEN_Enable_Lock_Source(pInstance, ENABLE, E_FRC_TGEN_LOCK_SOURCE_MODE_FROM_TGEN, FALSE);
            }
        }
    }
#endif

    //MDrv_WriteByte(BK_SELECT_00, u8Bank);
}

//-------------------------------------------------------------------------------------------------
/// set free run display window timing
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetFreeRunTiming_U2 (void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MDrv_XC_SetFreeRunTiming(pInstance);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetFreeRunTiming (void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FREERUN_TIMING, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Setup Gamma function ON/OFF
/// @param  bEnable      \b IN: Enable or Disable
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetGammaOnOff_U2 ( void* pInstance, MS_BOOL bEnable )
{
    //MS_U8 u8Bank;
    _XC_ENTRY(pInstance);
    /*u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_VOP);
    MDrv_WriteRegBit(L_BK_VOP(0x50), bEnable, BIT(0));
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_50_L, bEnable, BIT(0));
    _XC_RETURN(pInstance);
}

void MApi_XC_SetGammaOnOff ( MS_BOOL bEnable )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_GAMMA_ONOFF XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_GAMMA_ONOFF, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_SC_set_output_dclk(void *pInstance, MS_U16 u16Htt, MS_U16 u16Vtt, MS_U16 u16VFreq, MS_U32 u32HighAccurateVFreq, SCALER_WIN eWindow)
{
    MS_U32 u32UpBound, u32LowBound, u32Steps = SLOWLY_MOVE_STEPS, u32DelayMs = 0; //delaytime=0 means wait 2vsync as the old logic
    XC_PANEL_INFO_EX *pPanelInfoEx;
    MS_U32 u32PllSet = 0, u32HighAccuratePllSet = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_U64 u64DClk = (MS_U64)u16Htt * u16Vtt * u16VFreq;
    MS_U64 u64HighAccurateDClk = 0;
    MS_U16 u16Htt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HTotal;
    MS_U16 u16Vtt_out = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VTotal;
    MS_U32 u32DefaultDClk = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal
                            * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal
                            * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    MS_U32 u32AdjHighAccurateVFreq = u32HighAccurateVFreq;

    if (u32HighAccurateVFreq == 0)
    {
        u32AdjHighAccurateVFreq = (MS_U32)u16VFreq * 100;
    }

#if (HW_DESIGN_HDMITX_VER == 2 || HW_DESIGN_HDMITX_VER == 1)
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        if(u32AdjHighAccurateVFreq < 61500 && u32AdjHighAccurateVFreq > 59500)
            u32AdjHighAccurateVFreq = 60000;
        else if(u32AdjHighAccurateVFreq < 50500 && u32AdjHighAccurateVFreq > 49500)
            u32AdjHighAccurateVFreq = 50000;
        else if(u32AdjHighAccurateVFreq < 30500 && u32AdjHighAccurateVFreq > 29500)
            u32AdjHighAccurateVFreq = 30000;
        else if(u32AdjHighAccurateVFreq < 24500 && u32AdjHighAccurateVFreq > 23500)
            u32AdjHighAccurateVFreq = 24000;
        else if(u32AdjHighAccurateVFreq < 25500 && u32AdjHighAccurateVFreq > 24500)
            u32AdjHighAccurateVFreq = 25000;
    }
#endif

    u32DefaultDClk = u16Htt_out * u16Vtt_out * pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq;
    pPanelInfoEx = NULL;
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk u16VFreq=%u ***\n", u16VFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk u32AdjHighAccurateVFreq=%tu ***\n", (ptrdiff_t)u32AdjHighAccurateVFreq);
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "DefaultPanelInfo: MinSET = 0x%tx, MaxSET = 0x%tx\n",
                                        (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET,
                                        (ptrdiff_t)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET);

    MS_U32 u32Factor = MHal_SC_Get_LpllSet_Factor(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode,
        (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type,u32DefaultDClk);

    MS_U32 u32LPLLSet_Div = 1;

#if ((HW_DESIGN_4K2K_VER == 4)||(HW_DESIGN_4K2K_VER == 6))
    u32LPLLSet_Div = MHal_SC_Get_LpllSet_Div(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode,
        (MS_U8)pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type,u32DefaultDClk);
#endif

    if (u32LPLLSet_Div == 0)
        u32LPLLSet_Div = 1; // To avoid /0 case

    u32UpBound  = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MaxSET * u32Factor / u32LPLLSet_Div;
    u32LowBound = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u32MinSET * u32Factor / u32LPLLSet_Div;


    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "g_XC_InitData.stPanelInfo.eLPLL_Type.eLPLL_Type=%u, g_XC_InitData.stPanelInfo.eLPLL_Type.eLPLL_Mode=%u\n",
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u8LPLL_Mode)

    if((pXCResourcePrivate->stdrvXC_Display._bForceFreerun == FALSE)
        && _MDrv_XC_GetExPanelInfo(pInstance, &pPanelInfoEx, u16VFreq) && (pPanelInfoEx != NULL))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "PanelInfoEx assigned\n")
        if(pPanelInfoEx->bDClkValid)
        {
            u64DClk = (MS_U64)pPanelInfoEx->u32DClk*10000; //Dclk hz * 10
            if( (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_MINILVDS) ||
                (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_ANALOG_MINILVDS) ||
                (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_DIGITAL_MINILVDS))
            {
                u32DefaultDClk = u64DClk; //For minilvds
            }
        }
        //LowBound for MaxCLK, UpBound for MinCLK
        if(pPanelInfoEx->bMaxDClkValid)
        {
            u32LowBound = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)pPanelInfoEx->u32MaxDClk*10000, FALSE); //Dclk hz * 10
        }
        if(pPanelInfoEx->bMinDClkValid)
        {
            u32UpBound = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)pPanelInfoEx->u32MinDClk*10000, FALSE); //Dclk hz * 10
        }
        if(pPanelInfoEx->bDclkDelayValid)
        {
            u32DelayMs = (MS_U32)pPanelInfoEx->u16DclkDelay;
        }
        if(pPanelInfoEx->bDclkStepsValid)
        {
            u32Steps = (MS_U32)pPanelInfoEx->u16DclkSteps;
        }
    }

    u64HighAccurateDClk = (MS_U64)u32AdjHighAccurateVFreq * u16Htt * u16Vtt;

    #if (HW_DESIGN_HDMITX_VER == 2) // For Maxim HDMITx case, handle ODCLK for different color depth
    if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Init_Misc.u32MISC_B & E_XC_INIT_MISC_B_HDMITX_ENABLE)
    {
        MS_U16 u16ColorDepthFactor = HDMITX_COLORDEPTH_8BIT_FACTOR;

        if (pXCResourcePrivate->stdrvXC_MVideo_Context.enHDMITXColorDepth == E_XC_HDMITX_COLORDEPTH_10BIT)
            u16ColorDepthFactor = HDMITX_COLORDEPTH_10BIT_FACTOR;
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.enHDMITXColorDepth == E_XC_HDMITX_COLORDEPTH_12BIT)
            u16ColorDepthFactor = HDMITX_COLORDEPTH_12BIT_FACTOR;
        else if (pXCResourcePrivate->stdrvXC_MVideo_Context.enHDMITXColorDepth == E_XC_HDMITX_COLORDEPTH_16BIT)
            u16ColorDepthFactor = HDMITX_COLORDEPTH_16BIT_FACTOR;

        u64DClk = (u64DClk * u16ColorDepthFactor)/HDMITX_COLORDEPTH_DEFAULT_FACTOR;
        u64HighAccurateDClk = (u64HighAccurateDClk * u16ColorDepthFactor)/HDMITX_COLORDEPTH_DEFAULT_FACTOR;
    }
    #endif

#if 0
    // No matter what, using high accurate VFreq to generate corresponding high accurate LPLL Set
    // is always desirable.
    if (!(gSrcInfo[eWindow].bFBL || gSrcInfo[eWindow].bFBL))
    {
        u32HighAccuratePllSet = MDrv_PNL_CalculateLPLLSETbyDClk(u64HighAccurateDClk, TRUE);
    }
    else
    {
        u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(u64DClk, FALSE);
    }
#else
    {
        u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, u64DClk, FALSE);
        u32HighAccuratePllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, u64HighAccurateDClk, TRUE);
    }
#endif
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32PllSet = 0x%tx, u32UpBound = 0x%tx, u32LowBound = 0x%tx\n", (ptrdiff_t)u32PllSet, (ptrdiff_t)u32UpBound, (ptrdiff_t)u32LowBound)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "u32HighAccuratePllSet = 0x%tx, u32UpBound = 0x%tx, u32LowBound = 0x%tx\n", (ptrdiff_t)u32HighAccuratePllSet, (ptrdiff_t)u32UpBound, (ptrdiff_t)u32LowBound)
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "User forces Enable=%u, LPLL limit=0x%tx\n"
                                      , pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_D5D6D7]
                                      , (ptrdiff_t)pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_D5D6D7])
    if(pXCResourcePrivate->stdrvXC_Display._bForceFreerun == FALSE)
    {
       HAL_SC_Set_FPLL_Limit(pInstance, &u32HighAccuratePllSet
                             , u32LowBound
                             , u32UpBound
                             , pXCResourcePrivate->stdrvXC_Display._bFpllCusFlag[E_FPLL_FLAG_D5D6D7]
                             , pXCResourcePrivate->stdrvXC_Display._u32FpllCusData[E_FPLL_FLAG_D5D6D7]);
    }

    // just first patch for TCON case
#ifdef UFO_XC_FB_LEVEL
if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_DCLK)
    || ((((gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_FBL) || (gSrcInfo[MAIN_WINDOW].eFBLevel == E_XC_FB_LEVEL_RFBL_DI))
        && !gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)
      && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_VTOTAL))
    || ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_MINILVDS)
      && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_ANALOG_MINILVDS)
      && (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_DIGITAL_MINILVDS))
)
#else
if((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_DCLK) ||
       (((gSrcInfo[MAIN_WINDOW].bFBL || gSrcInfo[MAIN_WINDOW].bR_FBL) && !gSrcInfo[MAIN_WINDOW].Status2.bMVOPHSKMode)&&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.enPnl_Out_Timing_Mode == E_XC_PNL_CHG_VTOTAL)) ||
       ((pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_MINILVDS) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_ANALOG_MINILVDS) &&
        (pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.eLPLL_Type != E_XC_PNL_LPLL_DIGITAL_MINILVDS)))
#endif
{
        MDrv_SC_Set_Output_Dclk_Slowly(pInstance, u32HighAccuratePllSet, u32Steps, u32DelayMs, eWindow);
#if defined (__aarch64__)
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk-Normal CLK=%lu, Set=0x%X***\n", u64DClk, (unsigned int)u32HighAccuratePllSet)
#else
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk-Normal CLK=%llu, Set=0x%X***\n", u64DClk, (unsigned int)u32HighAccuratePllSet)
#endif
    }
    else
    {
        u32PllSet = MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)u32DefaultDClk, FALSE);
        MDrv_SC_Set_Output_Dclk_Slowly(pInstance, u32PllSet, u32Steps, u32DelayMs, eWindow);
        XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "**** SetOutputDclk-MINILVDS CLK=%tu, Set=0x%X***\n", (ptrdiff_t)u32DefaultDClk, (unsigned int)u32PllSet)
    }
}

//Obsolte function, do not use it any more
MS_U32 MApi_XC_Get_Current_OutputVFreqX100_U2(void* pInstance)
{
#if 0
    MS_U32 u32OutFrameRatex100 = 0;  //OutVFreq*100

    MS_U32 u32PllSet= 0; // 3BYTE
    MS_U16 u16Htt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0C_L, VOP_HTT_MASK);
    MS_U16 u16Vtt = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK);
    MS_U64 u64tmp = 0;

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32PllSet= MDrv_Read4Byte(L_BK_LPLL(0x0F)) & 0xFFFFFF; // 3BYTE
    LPLL_BK_RESTORE;
    if((g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_MINILVDS) || (g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_ANALOG_MINILVDS) || (g_XC_InitData.stPanelInfo.eLPLL_Type == E_XC_PNL_LPLL_DIGITAL_MINILVDS))
    {
//        u32OutFrameRatex100 = (MS_U32)((((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * 2) *100000000)/((MS_U64)(u16Htt * u16Vtt) * u32PllSet));
//**NOTE! In 2.6.28.9 Linux Kernel does not support 64bit division**
//        u32OutFrameRatex100 = (MS_U32)((((MS_U64)(LVDS_MPLL_CLOCK_MHZ * 524288) * 2 * 100000000)/(u16Htt * u16Vtt))/ u32PllSet);
        u64tmp = ((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * 2) * 100000000;
        do_div(u64tmp,(MS_U64)(u16Htt * u16Vtt));
        do_div(u64tmp,(u32PllSet));
        u32OutFrameRatex100 = u64tmp;
    }
    else
    {
        if( g_XC_InitData.stPanelInfo.u8LPLL_Mode )
        {
            //u32OutFrameRatex100 = (MS_U32)((((MS_U64)(LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN) * 2 * 100000000)/(u16Htt * u16Vtt * 7))/ u32PllSet);
            u64tmp = ((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN * 2) * 100000000;
            do_div(u64tmp,(u16Htt * u16Vtt * 7));
            do_div(u64tmp,(u32PllSet));
            u32OutFrameRatex100 = u64tmp;

        }
        else
        {
//            u32OutFrameRatex100 = (MS_U32)((((MS_U64)(LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN) * 100000000)/(u16Htt * u16Vtt * 7))/ u32PllSet);
            u64tmp = ((MS_U64)LVDS_MPLL_CLOCK_MHZ * 524288 * LPLL_LOOPGAIN) * 100000000;
            do_div(u64tmp,(u16Htt * u16Vtt * 7));
            do_div(u64tmp,(u32PllSet));
            u32OutFrameRatex100 = u64tmp;
        }
    }
    return u32OutFrameRatex100;
#else
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MS_U32 u32OutputVFreqX100 = 0;
    u32OutputVFreqX100 = MDrv_SC_GetOutputVFreqX100(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return u32OutputVFreqX100;
#endif
}

MS_U32 MApi_XC_Get_Current_OutputVFreqX100(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return 0;
    }

    stXC_GET_CURRENT_OUTPUTVFREQX100 XCArgs;
    XCArgs.u32ReturnValue = 0;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_CURRENT_OUTPUTVFREQX100, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable or disable IP auto coast
/// @param  bEnable                \b IN: Enable or Disable
//-------------------------------------------------------------------------------------------------
void MDrv_XC_EnableIPAutoCoast(void *pInstance, MS_BOOL bEnable)
{
    /*MS_U8 u8Bank;
    u8Bank = MDrv_ReadByte(BK_SELECT_00);
    MDrv_WriteByte(BK_SELECT_00, REG_BANK_IP1F2);
    MDrv_WriteRegBit(L_BK_IP1F2(0x0F), bEnable, BIT(7));
    MDrv_WriteByte(BK_SELECT_00, u8Bank);*/
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, bEnable? BIT(7):0, BIT(7));
}

void MApi_XC_EnableIPAutoCoast_U2(void* pInstance, MS_BOOL bEnable)
{
    _XC_ENTRY(pInstance);
    MDrv_XC_EnableIPAutoCoast(pInstance, bEnable);
    _XC_RETURN(pInstance);
}

void MApi_XC_EnableIPAutoCoast(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_IPAUTOCOAST XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IPAUTOCOAST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable IP auto coast Debounce
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableIPCoastDebounce_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce = 0;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_EnableIPCoastDebounce(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_IPCOAST_DEBOUNCE_ENABLE, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Clear IP auto coast status
//-------------------------------------------------------------------------------------------------
void MApi_XC_ClearIPCoastStatus_U2(void* pInstance)
{
    MS_U16 u16Tmp;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);

    u16Tmp = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L);
    if (u16Tmp & BIT(6))
    {
        pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce++;
        //MDrv_WriteByte(L_BK_IP1F2(0x0F), u8Tmp & ~(BIT(6)));
        SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16Tmp & ~(BIT(6)));
        if(pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce>3)
        {
            if (pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce)
            {
                LPLL_BK_STORE;
                LPLL_BK_SWITCH(0x00);
                MDrv_WriteRegBit(L_BK_LPLL(0x0C), DISABLE, BIT(3));
                LPLL_BK_RESTORE;
            }
            else
            {
                MDrv_XC_wait_output_vsync(pInstance, 1, 50, MAIN_WINDOW);
                Hal_SC_ip_software_reset(pInstance, REST_IP_F2, MAIN_WINDOW);
                //MDrv_WriteByte(L_BK_IP1F2(0x0F), u8Tmp & ~(BIT(6)));
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK01_0F_L, u16Tmp & ~(BIT(6)));
                pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce=0;
                pXCResourcePrivate->stdrvXC_Display._bDisableCoastDebounce = 1;
            }
        }
    }
    else
    {
        MDrv_WriteRegBit(L_BK_LPLL(0x0C), ENABLE, BIT(3));
        pXCResourcePrivate->stdrvXC_Display._u8CoastClearDebounce=0;
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

}

void MApi_XC_ClearIPCoastStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CLEAR_IPCOAST_STATUS, NULL) != UTOPIA_STATUS_SUCCESS)
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
/// Query the capability of scaler about source to Video encoder(VE)
/// @param  OutputCapability              \b OUT: Which part of scaler can output data to Video Encoder (VE).
///Check E_XC_SOURCE_TO_VE for capability bit. Take an example, if bit0 of OutputCapability equal to 1,
///Scaler can output data to ve from its E_XC_IP(IP)
///@return @ref MS_BOOL True if query success.
//-------------------------------------------------------------------------------------------------
E_APIXC_ReturnValue MApi_XC_SupportSourceToVE_U2(void* pInstance, MS_U16* pOutputCapability)
{
    return Hal_SC_support_source_to_ve(pInstance, pOutputCapability);
}

E_APIXC_ReturnValue MApi_XC_SupportSourceToVE(MS_U16* pOutputCapability)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_GET_SUPPORT_SOURCE_TO_VE XCArgs;
    XCArgs.pOutputCapability = pOutputCapability;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SUPPORT_SOURCE_TO_VE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

E_APIXC_ReturnValue MApi_XC_SetOutputCapture_U2(void* pInstance, MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE)
{
    E_APIXC_ReturnValue ret;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    ret = Hal_SC_set_output_capture_enable(pInstance, bEnable,eSourceToVE);
    if(bEnable)
    {
        gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE = eSourceToVE;
    }
    else
    {
        gSrcInfo[MAIN_WINDOW].Status2.eSourceToVE = E_XC_NONE;
    }
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return ret;
}

E_APIXC_ReturnValue MApi_XC_SetOutputCapture(MS_BOOL bEnable,E_XC_SOURCE_TO_VE eSourceToVE)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return E_APIXC_RET_FAIL;
    }

    stXC_SET_OUTPUT_CAPTURE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eSourceToVE = eSourceToVE;
    XCArgs.eReturnValue = E_APIXC_RET_FAIL;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OUTPUT_CAPTURE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return E_APIXC_RET_FAIL;
    }
    else
    {
        return XCArgs.eReturnValue;
    }
}

void MDrv_SC_set_csc(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    Hal_SC_set_csc(pInstance, bEnable, eWindow );
}
void MApi_XC_SetCsc_U2( void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow )
{
    MDrv_SC_set_csc(pInstance, bEnable,  eWindow );
    return ;
}

void MApi_XC_SetCsc( MS_BOOL bEnable, SCALER_WIN eWindow )
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_CSC XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_CSC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_SC_rep_window(void *pInstance, MS_BOOL bEnable,MS_WINDOW_TYPE Win,MS_U8 u8Color)
{
    Hal_SC_set_rep_window(pInstance, bEnable,Win.x,Win.y,Win.width,Win.height,u8Color);
}

//-------------------------------------------------------------------------------------------------
/// update display window registers with input window
/// @param  eWindow                          \b IN: Main or sub window
/// @param  pdspwin                          \b IN: window info that will be setted to registers
//-------------------------------------------------------------------------------------------------
void MDrv_XC_set_dispwin_to_reg(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    Hal_SC_set_disp_window(pInstance, eWindow, pdspwin);
}

//-------------------------------------------------------------------------------------------------
/// update all window registers with input window(Including DE window, and try use MLOAD)
/// @param  pDEWin                          \b IN: Panel DE window info that will be setted to registers
/// @param  pMainWin                        \b IN: Main display window info that will be setted to registers
/// @param  pSubWin                         \b IN: Sub display window info that will be setted to registers
/// Notes: if one of the pxxWin=NULL, the related window will not be setted
//-------------------------------------------------------------------------------------------------
void MDrv_XC_UpdateWindow(void *pInstance, MS_WINDOW_TYPE *pDEWin, MS_WINDOW_TYPE *pMainWin, MS_WINDOW_TYPE *pSubWin, MS_WINDOW_TYPE *pReportWin, MS_WINDOW_TYPE *pNewOSDWin)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    if (psXCInstPri->u32DeviceID == 1)
    {
        /// not support
        return;
    }
    CHECK_DEVICE1_INSTANCE();
    XC_INSTANCE_PRIVATE *psXCInstPri_1 = NULL;
    UtopiaInstanceGetPrivate(g_pDevice1Instance, (void**)&psXCInstPri_1);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate_1 = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri_1->u32DeviceID)],(void**)(&pXCResourcePrivate_1));
#endif
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    E_FRC_MLOAD_TRIG_MODE eTrigMode = E_FRC_MLOAD_TRIG_MODE_FROM_OP1;
    if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_5F_L) & BIT(15))
    {
        eTrigMode = E_FRC_MLOAD_TRIG_MODE_FROM_OP2;
    }
    else
    {
        eTrigMode = E_FRC_MLOAD_TRIG_MODE_FROM_OP1;
    }

    MHal_FRC_Set_Mload_Trig_Mode(pInstance, E_FRC_MLOAD_TRIG_MODE_FROM_OP1);
#endif
    if (MDrv_XC_MLoad_GetStatus(pInstance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(pInstance);
        // Panel DE window
        if(pDEWin != NULL)
        {
            //FIX ME:need follow HW_DESIGN_4K2K_VER = 6 rule
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_04_L, pDEWin->x, VOP_DE_HSTART_MASK);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_05_L, (pDEWin->x+pDEWin->width-1), VOP_DE_HEND_MASK);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_06_L, pDEWin->y, VOP_DE_VSTART_MASK);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_07_L, (pDEWin->y+pDEWin->height-1), VOP_DE_VEND_MASK);
            memcpy(&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), pDEWin, sizeof(MS_WINDOW_TYPE));
            memcpy(&(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), pDEWin, sizeof(MS_WINDOW_TYPE));
        }
        // Main Display window
        if(pMainWin != NULL)
        {
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (IsFSCDisplayWindowEnable())
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_05_L, pMainWin->x, VOP_DISPLAY_HSTART_MASK);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_06_L, (pMainWin->x+pMainWin->width-1), VOP_DISPLAY_HEND_MASK);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_0D_L, pMainWin->y, VOP_DISPLAY_VSTART_MASK);
                // HW limit: MDE_Vend > FDE_Vend will cause PostScaling overrun. So fix this location.
                // MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_0E_L, (pMainWin->y+pMainWin->height-1), VOP_DISPLAY_VEND_MASK);
                MS_U16 u16DEVend = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0C_L);
                if((pMainWin->y+pMainWin->height-1) >= u16DEVend)
                {
                    //when MDE_Vend > FDE_Vend,use defaut
                    //MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_0E_L, u16DEVend, 0x1FFF);
                }
                else
                {
                    //if MDE_Vend < FDE_Vend must move MDE Vend to aviod pixshift last V pix repeat
                    MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK68_0E_L, (pMainWin->y+pMainWin->height-1), 0x1FFF);
                }
            }
            else
#endif
            {
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_08_L, pMainWin->x, VOP_DISPLAY_HSTART_MASK);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_09_L, (pMainWin->x+pMainWin->width-1), VOP_DISPLAY_HEND_MASK);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0A_L, pMainWin->y, VOP_DISPLAY_VSTART_MASK);
                MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK10_0B_L, (pMainWin->y+pMainWin->height-1), VOP_DISPLAY_VEND_MASK);
            }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (IsFSCDisplayWindowEnable() && (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW)))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "FRC_IsFHDToFSC: ByPass chagne global var for DispWin.\n");

            }
            else
#endif
            {
                memcpy(&(gSrcInfo[MAIN_WINDOW].stDispWin), pMainWin, sizeof(MS_WINDOW_TYPE));
            }
        }
        // Sub Display window
        if(pSubWin != NULL)
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_07_L, pSubWin->x, 0x3FFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_08_L, (pSubWin->x+pSubWin->width-1), 0x3FFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_09_L, pSubWin->y, 0x1FFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_0A_L, (pSubWin->y+pSubWin->height-1), 0x1FFF);
            memcpy(&(gSrcInfo[SUB_WINDOW].stDispWin), pSubWin, sizeof(MS_WINDOW_TYPE));
        }
        //Report window
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableRepWindowForFrameColor && (pReportWin != NULL))
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_42_L, pReportWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_43_L, pReportWin->x+pReportWin->width-2, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_44_L, pReportWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK0F_45_L, pReportWin->y+pReportWin->height-1, 0xFFFF);
            memcpy(&pXCResourcePrivate->stdrvXC_MVideo_Context.g_RepWindow, pReportWin, sizeof(MS_WINDOW_TYPE));
        }

        // Please add here for GOP ...
        if ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, BIT(15))) && (pNewOSDWin != NULL))
        {
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_10_L, pNewOSDWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_11_L, pNewOSDWin->x+pNewOSDWin->width-1, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_12_L, pNewOSDWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(pInstance, REG_SC_BK2D_13_L, pNewOSDWin->y+pNewOSDWin->height-1, 0xFFFF);
        }
        MDrv_XC_MLoad_Fire(pInstance, TRUE);
        _MLOAD_RETURN(pInstance);
    }
    else
    {
        // Panel DE window
        if(pDEWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, pDEWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_05_L, (pDEWin->x+pDEWin->width-1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_06_L, pDEWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_07_L, (pDEWin->y+pDEWin->height-1));
            memcpy(&(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), pDEWin, sizeof(MS_WINDOW_TYPE));
            memcpy(&(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), pDEWin, sizeof(MS_WINDOW_TYPE));
        }
        // Main Display window
        if(pMainWin != NULL)
        {
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (IsFSCDisplayWindowEnable())
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_05_L, pMainWin->x);
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_06_L, (pMainWin->x+pMainWin->width-1));
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0D_L, pMainWin->y);
                // HW limit: MDE_Vend > FDE_Vend will cause PostScaling overrun. So fix this location.
                //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0E_L, (pMainWin->y+pMainWin->height-1));
                MS_U16 u16DEVend = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0C_L);
                if((pMainWin->y+pMainWin->height-1) >= u16DEVend)
                {
                    //SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0E_L, u16DEVend);
                }
                else
                {
                    SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0E_L, (pMainWin->y+pMainWin->height-1));
                }
            }
            else
#endif
            {
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_08_L, pMainWin->x);
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_09_L, (pMainWin->x+pMainWin->width-1));
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0A_L, pMainWin->y);
                SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0B_L, (pMainWin->y+pMainWin->height-1));
            }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
            if (IsFSCDisplayWindowEnable() && (MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW)))
            {
                XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW, "FRC_IsFHDToFSC: ByPass chagne global var for DispWin.\n");

            }
            else
#endif
            {
                memcpy(&(gSrcInfo[MAIN_WINDOW].stDispWin), pMainWin, sizeof(MS_WINDOW_TYPE));
            }
        }
        // Sub Display window
        if(pSubWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_07_L, pSubWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_08_L, (pSubWin->x+pSubWin->width-1));
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_09_L, pSubWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_0A_L, (pSubWin->y+pSubWin->height-1));
            memcpy(&(gSrcInfo[SUB_WINDOW].stDispWin), pSubWin, sizeof(MS_WINDOW_TYPE));
        }
        //Report window
        if (pReportWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_42_L, pReportWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_43_L, pReportWin->x+pReportWin->width-2);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_44_L, pReportWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_45_L, pReportWin->y+pReportWin->height-1);
            //Skip enable here, SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK0F_57_L, BIT(8)|BIT(9), BIT(8)|BIT(9)); // Enable display rep window
        }

        // Please add here for GOP ...
        if (pNewOSDWin != NULL)
        {
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, pNewOSDWin->x);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_11_L, pNewOSDWin->x+pNewOSDWin->width-1);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_12_L, pNewOSDWin->y);
            SC_W2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK2D_13_L, pNewOSDWin->y+pNewOSDWin->height-1);
        }
    }
#if PIP_PATCH_USING_SC1_MAIN_AS_SC0_SUB
    //FIX ME:need follow HW_DESIGN_4K2K_VER = 6 rule
    if (MDrv_XC_MLoad_GetStatus(g_pDevice1Instance) == E_MLOAD_ENABLED)
    {
        _MLOAD_ENTRY(g_pDevice1Instance);
        // Panel DE window
        if(pDEWin != NULL)
        {
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_04_L, pDEWin->x, VOP_DE_HSTART_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_05_L, (pDEWin->x+pDEWin->width-1), VOP_DE_HEND_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_06_L, pDEWin->y, VOP_DE_VSTART_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_07_L, (pDEWin->y+pDEWin->height-1), VOP_DE_VEND_MASK);
            memcpy(&(pXCResourcePrivate_1->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), pDEWin, sizeof(MS_WINDOW_TYPE));
            memcpy(&(gSrcInfo_1[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), pDEWin, sizeof(MS_WINDOW_TYPE));
        }
        // Sub Display window
        if(pSubWin != NULL)
        {
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_08_L, pSubWin->x, VOP_DISPLAY_HSTART_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_09_L, (pSubWin->x+pSubWin->width-1), VOP_DISPLAY_HEND_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_0A_L, pSubWin->y, VOP_DISPLAY_VSTART_MASK);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK10_0B_L, (pSubWin->y+pSubWin->height-1), VOP_DISPLAY_VEND_MASK);
            memcpy(&(gSrcInfo_1[MAIN_WINDOW].stDispWin), pSubWin, sizeof(MS_WINDOW_TYPE));
        }
        //Report window
        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableRepWindowForFrameColor && (pReportWin != NULL))
        {
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK0F_42_L, pReportWin->x, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK0F_43_L, pReportWin->x+pReportWin->width-2, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK0F_44_L, pReportWin->y, 0xFFFF);
            MDrv_XC_MLoad_WriteCmd(g_pDevice1Instance, REG_SC_BK0F_45_L, pReportWin->y+pReportWin->height-1, 0xFFFF);
            memcpy(&pXCResourcePrivate_1->stdrvXC_MVideo_Context.g_RepWindow, pReportWin, sizeof(MS_WINDOW_TYPE));
        }
        MDrv_XC_MLoad_Fire(g_pDevice1Instance, TRUE);
        _MLOAD_RETURN(g_pDevice1Instance);
    }
    else
    {
         // Panel DE window
        if(pDEWin != NULL)
        {
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_04_L, pDEWin->x);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_05_L, (pDEWin->x+pDEWin->width-1));
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_06_L, pDEWin->y);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_07_L, (pDEWin->y+pDEWin->height-1));
            memcpy(&(pXCResourcePrivate_1->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo), pDEWin, sizeof(MS_WINDOW_TYPE));
            memcpy(&(gSrcInfo_1[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), pDEWin, sizeof(MS_WINDOW_TYPE));
        }
        // Sub Display window
        if(pSubWin != NULL)
        {
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_08_L, pSubWin->x);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_09_L, (pSubWin->x+pSubWin->width-1));
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_0A_L, pSubWin->y);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK10_0B_L, (pSubWin->y+pSubWin->height-1));
            memcpy(&(gSrcInfo_1[MAIN_WINDOW].stDispWin), pSubWin, sizeof(MS_WINDOW_TYPE));
        }
        //Report window
        if (pReportWin != NULL)
        {
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK0F_42_L, pReportWin->x);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK0F_43_L, pReportWin->x+pReportWin->width-2);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK0F_44_L, pReportWin->y);
            SC_W2BYTE(psXCInstPri_1->u32DeviceID, REG_SC_BK0F_45_L, pReportWin->y+pReportWin->height-1);
        }
    }
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    MHal_FRC_Set_Mload_Trig_Mode(pInstance, eTrigMode);
#endif
#endif
}

//-------------------------------------------------------------------------------------------------
/// get current display window registers setting
/// @param  eWindow                          \b IN : Main or sub window
/// @param  pdspwin                          \b OUT: Pointer for ouput disp window register
//-------------------------------------------------------------------------------------------------
void MDrv_XC_get_dispwin_from_reg(void *pInstance, SCALER_WIN eWindow, MS_WINDOW_TYPE *pdspwin)
{
    Hal_SC_get_disp_window(pInstance, eWindow, pdspwin);
}

//-------------------------------------------------------------------------------------------------
/// Set VOP Gain for Main Window(after 3x3 before Gamma)
/// @param  eVop_Channel                \b IN: Select VOP R/G/B
/// @param  u16Val                           \b IN: Set value
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPreGammaGain_U2(void* pInstance, SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{
    _XC_ENTRY(pInstance);
    HAL_SC_VOP_Set_Contrast_Value(pInstance, eVop_Channel, u16Val, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetPreGammaGain(SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PREGAMMA_GAIN XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eVop_Channel = eVop_Channel;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PREGAMMA_GAIN, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Set VOP Offset for Main Window(after 3x3 before Gamma)
/// @param  eVop_Channel                \b IN: Select VOP R/G/B
/// @param  u16Val                           \b IN: Set value
//-------------------------------------------------------------------------------------------------
void MApi_XC_SetPreGammaOffset_U2(void* pInstance, SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{
    _XC_ENTRY(pInstance);
    HAL_SC_VOP_Set_Brightness_Value(pInstance, eVop_Channel, u16Val, eWindow);
    _XC_RETURN(pInstance);
}

void MApi_XC_SetPreGammaOffset(SCALER_WIN eWindow, MS_XC_VOP_CHANNEL_t eVop_Channel, MS_U16 u16Val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_PREGAMMA_OFFSET XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.eVop_Channel = eVop_Channel;
    XCArgs.u16Val = u16Val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_PREGAMMA_OFFSET, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Wait FPLL finished
/// @return @ref MS_BOOL indicate FPLL finished or not (frame lock finished or not)
//-------------------------------------------------------------------------------------------------
static MS_BOOL _MDrv_SC_WaitFPLLDone(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(HAL_SC_WaitFPLLDone(pInstance))
    {
        if(pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR
            && pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]
            && !pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
        {
            MS_U32 u32timer = MsOS_GetSystemTime() - pXCResourcePrivate->stdrvXC_Display._u32MCNRTimer;
            MS_U32 u32DelayTime = (MS_U32)_MDrv_SC_FPLL_GetMCNRDelayTime(pInstance);
            if(u32DelayTime > u32timer)
            {
                MsOS_DelayTask(u32DelayTime-u32timer); //Turn on before 8 vsync
            }
            if (!pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[MAIN_WINDOW])
            {
                _XC_ENTRY(pInstance);
                Hal_SC_SetOPWriteOff(pInstance, !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[MAIN_WINDOW]);
                _XC_RETURN(pInstance);
            }
            pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = FALSE;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------------
/// Wait FPLL finished
/// @return @ref MS_BOOL indicate FPLL finished or not (frame lock finished or not)
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_WaitFPLLDone_U2(void* pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support FPLL for device id %td\n",__FUNCTION__,(ptrdiff_t)psXCInstPri->u32DeviceID);
        printf("MApi_XC_EX_WaitFPLLDone() -> Not support FPLL for device id %td\n", (ptrdiff_t)psXCInstPri->u32DeviceID);
        return FALSE;
    }
    // we didn't add mutex here because
    // in T3, MDrv_SC_WaitFPLLDone() will use ISR
    // in U3, MDrv_SC_WaitFPLLDone() won't use ISR and will use mutex in HAL layer
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    MS_BOOL bReturn = FALSE;
    if(gSrcInfo[MAIN_WINDOW].bEnableFPLL)
    {
        if(pXCResourcePrivate->stdrvXC_Display._bFSM_FPLL_Working)
        {
            gSrcInfo[MAIN_WINDOW].bDoneFPLL = TRUE; //FSM framelock open screen directly, no need to wait.
        }
        else
        {
            gSrcInfo[MAIN_WINDOW].bDoneFPLL = _MDrv_SC_WaitFPLLDone(pInstance);
        }
    }
    else
    {
        gSrcInfo[MAIN_WINDOW].bDoneFPLL = FALSE; //Freerun, NO fpll
    }
    bReturn = gSrcInfo[MAIN_WINDOW].bDoneFPLL;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);

    return bReturn;
}

MS_BOOL MApi_XC_WaitFPLLDone(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_WAIT_FPLL_DONE XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_WAIT_FPLL_DONE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Get scaler output vertical frequency
/// @return @ref MS_U16 return output vertical frequency x 100
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_GetOutputVFreqX100(void *pInstance)
{
    MS_U16 u16OutputVFreqAfterFRC = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if defined(XC_SUPPORT_FRC_CONVERT) && (XC_SUPPORT_FRC_CONVERT == 0)
    u16OutputVFreqAfterFRC = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq*10;
#else
    u16OutputVFreqAfterFRC = (MS_U16)((gSrcInfo[MAIN_WINDOW].Status2.u32HighAccurateOutputVFreqAfterFRC+5)/10);
#endif
    return u16OutputVFreqAfterFRC;
}

//-------------------------------------------------------------------------------------------------
/// Get OP1 output vertical frequency
/// @return @ref MS_U16 return output vertical frequency x 100
//-------------------------------------------------------------------------------------------------
MS_U16 MDrv_SC_GetOP1OutputVFreqX100(void *pInstance)
{
    MS_U16 u16OP1_OutputVFreqAfterFRC = 0;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if defined(XC_SUPPORT_FRC_CONVERT) && (XC_SUPPORT_FRC_CONVERT == 0)
    u16OP1_OutputVFreqAfterFRC = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq*10;
#else
#if FRC_INSIDE
    #if (HW_DESIGN_4K2K_VER != 6) // for manhattan, only 1 LPLL
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_Pnl_Misc.FRCInfo.bFRC)
    {
        u16OP1_OutputVFreqAfterFRC = gSrcInfo[MAIN_WINDOW].Status2.u16OP1_OutputVFreqAfterFRC*10;
    }
    else
    #endif
    {
        u16OP1_OutputVFreqAfterFRC = (MS_U16)((gSrcInfo[MAIN_WINDOW].Status2.u32HighAccurateOutputVFreqAfterFRC+5)/10);
    }
#else
    u16OP1_OutputVFreqAfterFRC = (MS_U16)((gSrcInfo[MAIN_WINDOW].Status2.u32HighAccurateOutputVFreqAfterFRC+5)/10);
#endif
#endif

    return u16OP1_OutputVFreqAfterFRC;
}


//-------------------------------------------------------------------------------------------------
/// Set scaler display line buffer mode
/// @param  bEnable                \b IN: Select "Enable" to ring buffer mode or "Disable" for line buffer mode
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetDisplay_LineBuffer_Mode(void *pInstance, MS_BOOL bEnable)
{
    HAL_SC_SetDisplay_LineBuffer_Mode(pInstance, bEnable);
}

//-------------------------------------------------------------------------------------------------
/// Set scaler display line buffer Start mode
/// @param  bEnable                \b IN: Select "Enable" to start at faling edge of Vsync_init or
///                                       "Disable" for start at advance 1 display line
//-------------------------------------------------------------------------------------------------
void MDrv_SC_SetDisplay_Start_Mode(void *pInstance, MS_BOOL bEnable)
{
    HAL_SC_SetDisplay_Start_Mode(pInstance, bEnable);
}

void MDrv_XC_SetOSD2VEmode(void *pInstance, EN_VOP_SEL_OSD_XC2VE_MUX  eVOPSelOSD_MUX)
{
    Hal_SC_Set_OSD2VE(pInstance, eVOPSelOSD_MUX);
}

MS_BOOL MDrv_XC_Check_HNonLinearScaling(void* pInstance)
{
    return Hal_SC_Check_HNonLinearScaling(pInstance);
}

MS_BOOL MApi_XC_Check_HNonLinearScaling_U2(void* pInstance)
{
    MS_BOOL bRet;
    _XC_ENTRY(pInstance);
    bRet = MDrv_XC_Check_HNonLinearScaling(pInstance);
    _XC_RETURN(pInstance);
    return bRet;
}

MS_BOOL MApi_XC_Check_HNonLinearScaling(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_CHECK_H_NONLINEAR_SCALING XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_CHECK_H_NONLINEAR_SCALING, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MApi_XC_Set_Extra_fetch_adv_line_U2(void* pInstance, MS_U8 u8val)
{
    _XC_ENTRY(pInstance);
    Hal_SC_Set_extra_fetch_line(pInstance, u8val & 0x0F);
    Hal_SC_Set_extra_adv_line(pInstance, u8val>>4);
    _XC_RETURN(pInstance);
}

void MApi_XC_Set_Extra_fetch_adv_line(MS_U8 u8val)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_EXTRA_FETCH_ADV_LINE XCArgs;
    XCArgs.u8val = u8val;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_EXTRA_FETCH_ADV_LINE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// Enable FPLL related settings not by program but by user's setting from TV tool
/// Including PhaseLimit, limitD5D6D7_RK
/// @param bEnable @ref MS_BOOL          \b IN: enable or not
//-------------------------------------------------------------------------------------------------
void MApi_XC_EnableFpllManualSetting_U2(void* pInstance, MS_BOOL bEnable)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_MVideo._SContext.bEnableFPLLManualDebug = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_EnableFpllManualSetting(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_MANUAL_SETTING_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_MANUAL_SETTING_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

//--------------------------------------------------------------------------------------------------------------------
/// Test if panel output is ok by changing output DClk, this is to test max range that FPLL can do
/// @param u32FpllSetOffset @ref MS_U32     \b IN: the offset to be add/minus the typical DClk output, unit is SET
/// @param u16TestCnt       @ref MS_U16     \b IN: the times to do boundary test
//--------------------------------------------------------------------------------------------------------------------
void MApi_XC_FpllBoundaryTest_U2(void* pInstance, MS_U32 u32FpllSetOffset, MS_U16 u16TestCnt)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 i;
    MS_U32 u32OrigFpllSet;

    if(0)//(psXCInstPri->u32DeviceID > E_XC_DEVICE0)
    {
        printf("[%s] Not support FPLL for device id %td\n",__FUNCTION__,(ptrdiff_t)psXCInstPri->u32DeviceID);
        printf("MApi_XC_EX_EnableFpllBoundaryTest() -> Not support FPLL for device id %td\n",(ptrdiff_t)psXCInstPri->u32DeviceID);
        return;
    }

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    // set to free run
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    _XC_ENTRY(pInstance);
    MDrv_XC_SetFreeRunTiming(pInstance);
    _XC_RETURN(pInstance);

    // set to default output DClk
    printf("MApi_XC_EnableFpllBoundaryTest(0x%tx, %d)\n", (ptrdiff_t)u32FpllSetOffset, u16TestCnt);
    printf("MApi_XC_EnableFpllBoundaryTest() -> Default DClk = %d x %d x %d = %td\n",
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal,
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal,
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq,
        (ptrdiff_t)(MDrv_PNL_CalculateLPLLSETbyDClk(pInstance, (MS_U64)
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal*
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal*
                                    pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq, FALSE))
    );

    MDrv_SC_set_output_dclk(pInstance, pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultHTotal,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVTotal,
                            pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16DefaultVFreq, 0, MAIN_WINDOW);
    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    u32OrigFpllSet = MDrv_Read4Byte(L_BK_LPLL(0x0F));

    printf("MApi_XC_EnableFpllBoundaryTest(): std set = 0x%tx\n", (ptrdiff_t)u32OrigFpllSet);

    // do FpllBoundary testst
    for(i=0; i<u16TestCnt; i++)
    {
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigFpllSet + u32FpllSetOffset);
        MsOS_DelayTask(2);
        MDrv_Write3Byte(L_BK_LPLL(0x0F), u32OrigFpllSet - u32FpllSetOffset);
        MsOS_DelayTask(2);
    }
    LPLL_BK_RESTORE;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_FpllBoundaryTest(MS_U32 u32FpllSetOffset, MS_U16 u16TestCnt)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_FPLL_BOUNDARY_TEST XCArgs;
    XCArgs.u32FpllSetOffset = u32FpllSetOffset;
    XCArgs.u16TestCnt = u16TestCnt;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_FPLL_BOUNDARY_TEST, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_SkipOPWriteOffInSetWindow_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[eWindow] = bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SkipOPWriteOffInSetWindow(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SKIP_OPWRITEOFF_IN_SETWINDOW XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_SKIP_OPWRITEOFF_IN_SETWINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_XC_GetSkipOPWriteOffInSetWindow(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return pXCResourcePrivate->stdrvXC_Display._abSkipOPWOffInSetWindow[eWindow];
}

MS_BOOL MApi_XC_GetSkipOPWriteOffInSetWindow_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_GetSkipOPWriteOffInSetWindow(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_GetSkipOPWriteOffInSetWindow(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_SKIP_OPWRITEOFF_IN_SETWINDOW XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_SKIP_OPWRITEOFF_IN_SETWINDOW, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return FALSE;
    }
    else
    {
        return XCArgs.bReturnValue;
    }
}

void MDrv_XC_SetSkipDisableOPWriteOffInFPLL(void *pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bSkipDisableOPWriteOffInFPLL[eWindow] = bEnable;
}

void MApi_XC_SetSkipDisableOPWriteOffInFPLL_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    MDrv_XC_SetSkipDisableOPWriteOffInFPLL(pInstance, bEnable, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_SetSkipDisableOPWriteOffInFPLL(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL XCArgs;
    XCArgs.bEnable = FALSE;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_SET_SKIP_DISABLE_OPWRITEOFF_IN_FPLL, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_OPWriteOffEnable_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow] = !bEnable;
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_Set_OPWriteOffEnable(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_Set_OPWriteOffEnableToReg_U2(void* pInstance, MS_BOOL bEnable, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow] = !bEnable;
    if(!pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR)
    {
        _XC_ENTRY(pInstance);
        Hal_SC_SetOPWriteOff(pInstance, bEnable);
        _XC_RETURN(pInstance);
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_Set_OPWriteOffEnableToReg(MS_BOOL bEnable, SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_OPWRITEOFF_ENABLE_TO_REG XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_OPWRITEOFF_ENABLE_TO_REG, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MApi_XC_ReLoad_OPWriteOff(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
     pXCResourcePrivate->stdrvXC_Display._bNeedReloadMCNR = TRUE;
}


// If this function is called, driver will set OPW immediatly.
void MApi_XC_ForceSet_OPWriteOffEnable_U2(void* pInstance, MS_BOOL bEnable ,SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;

    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (eWindow == MAIN_WINDOW )
    {
        if (bEnable)
        {
            _XC_ENTRY(pInstance);
            Hal_SC_SetOPWriteOff(pInstance, TRUE);
            _XC_RETURN(pInstance);
            pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR = TRUE;
        }
        else
        {
            pXCResourcePrivate->stdrvXC_Display._bForceDisableMCNR = FALSE;
        }
    }
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
}

void MApi_XC_ForceSet_OPWriteOffEnable(MS_BOOL bEnable ,SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_FORCESET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.bEnable = bEnable;
    XCArgs.eWindow = eWindow;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_FORCESET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

MS_BOOL MDrv_XC_Get_OPWriteOffEnable(void *pInstance, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    return !pXCResourcePrivate->stdrvXC_Display._bEnableMCNR[eWindow];
}

MS_BOOL MApi_XC_Get_OPWriteOffEnable_U2(void* pInstance, SCALER_WIN eWindow)
{
    MS_BOOL bReturn = FALSE;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    bReturn = MDrv_XC_Get_OPWriteOffEnable(pInstance, eWindow);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    return bReturn;
}

MS_BOOL MApi_XC_Get_OPWriteOffEnable(SCALER_WIN eWindow)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_OPWRITEOFF_ENABLE XCArgs;
    XCArgs.eWindow = eWindow;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_OPWRITEOFF_ENABLE, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
// This function will store the H or V sync information(SyncStart/end/width) of panel default for XC to use
// Because their is no other place to get them in XC Lib source code
// IN:
//    NONE
// Return:
//    NONE
//-------------------------------------------------------------------------------------------------
void MDrv_XC_GetDefaultHVSyncInfo(void *pInstance)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    //Note: H/V Total, H/V DE End, H/V Syncwidth all need minus 1 when set to register
    pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_01_L, 0xFF) + 1; //HsyncWidth= reg + 1
    pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_04_L, VOP_DE_HSTART_MASK) - pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth;
    pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK) - ((SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, 0x1000) >> 1) + SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_02_L, 0x7FF)) + 1;
#if defined(PATCH_HW_VTT_LIMITATION)
    if(MDrv_ReadByte(REG_CHIP_REVISION) < HW_VTT_LIMITATION_CHIPREV)
    {
        pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK68_36_L, VOP_VTT_MASK) - SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK); //V backPorch= VTT+1-(VsyncEnd+1)
    }
    else
#endif
    {
        pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch = SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_0D_L, VOP_VTT_MASK) - SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_03_L, VOP_VSYNC_END_MASK); //V backPorch= VTT+1-(VsyncEnd+1)
    }
    if(SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_10_L) & BIT(15)) //If manual Vsync mode, keep SyncWidth/back porch unchanged when vtt changes
    {
        pXCResourcePrivate->stdrvXC_Display._bHSyncChanged = TRUE;
        pXCResourcePrivate->stdrvXC_Display._bVSyncChanged = TRUE;
    }
    XC_LOG_TRACE(XC_DBGLEVEL_SETTIMING, "SyncWidth/back porch: V=%u,%u,H=%u,%u\n"
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultVSyncBackPorch
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncWidth
                                      , pXCResourcePrivate->stdrvXC_Display._u16DefaultHSyncBackPorch)
}

///For K1 OSD Layer Switch
E_APIXC_ReturnValue MDrv_XC_SetOSDLayer(void *pInstance, E_VOP_OSD_LAYER_SEL eVOPOSDLayer, SCALER_WIN eWindow)
{
#if (VIDEO_OSD_SWITCH_VER >= 2) // clippers/kano mode
    MS_U16 u16RegVal = 0;
    switch (eVOPOSDLayer)
    {
        case E_VOP_LAYER_VIDEO_MUX1_MUX2:    // E_VOP_LAYER_VIDEO_MUX1_MUX2_MUX3_MUX4
        case E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2:
        case E_VOP_LAYER_FRAME_VIDEO_MUX2_MUX1:
            u16RegVal = 0;
            break;
        case  E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2_MUX3_MUX4:
            u16RegVal = 1;
            break;
        case E_VOP_LAYER_FRAME_MUX2_VIDEO_MUX1:
        case E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2:    // E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2_MUX3_MUX4
            u16RegVal = 2;
            break;
        case E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO:    // E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO_MUX3_MUX4
        case E_VOP_LAYER_FRAME_MUX2_MUX1_VIDEO:
            u16RegVal = 3;
            break;
        case E_VOP_LAYER_FRAME_MUX1_MUX2_MUX3_VIDEO_MUX4:
            u16RegVal = 4;
            break;
        case E_VOP_LAYER_FRAME_MUX1_MUX2_MUX3_MUX4_VIDEO:
            u16RegVal = 5;
            break;
        default:
            printf("[%s]Unsupport layer Setting %u, it can't be return by this interface.\n",__FUNCTION__,eVOPOSDLayer);
            return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }
    #if (VIDEO_OSD_SWITCH_VER == 3)
    MS_BOOL bCtrlByGop = (SC_R2BYTEMSK(0, REG_SC_BK2F_36_L, BIT(5))) == BIT(5);
    if (bCtrlByGop)
    {
        Hal_SC_Set_OSDLayer(pInstance, u16RegVal, eWindow);
    }
    else
    #endif
    {
        SC_W2BYTEMSK(0, REG_SC_BK2F_36_L, u16RegVal, BITMASK(2:0));
    }
    return E_APIXC_RET_OK;
#else
    return Hal_SC_Set_OSDLayer(pInstance, eVOPOSDLayer, eWindow);
#endif
}

E_VOP_OSD_LAYER_SEL MDrv_XC_GetOSDLayer(void *pInstance, SCALER_WIN eWindow)
{
#if (VIDEO_OSD_SWITCH_VER >= 2) // clippers/kano mode
    MS_U16 u16RegVal = 0;
    #if (VIDEO_OSD_SWITCH_VER == 3)
    MS_BOOL bCtrlByGop = (SC_R2BYTEMSK(0, REG_SC_BK2F_36_L, BIT(5))) == BIT(5);
    if (bCtrlByGop)
    {
        u16RegVal = GOP_R2BYTEMSK(_PK_L_(0x0,0x6A), 0x0007);
    }
    else
    #endif
    {
        u16RegVal = SC_R2BYTEMSK(0, REG_SC_BK2F_36_L, BITMASK(2:0));
    }
    E_VOP_OSD_LAYER_SEL enStbLayer = E_VOP_LAYER_RESERVED;
    switch (u16RegVal)
    {
        case 0:
            enStbLayer = E_VOP_LAYER_VIDEO_MUX1_MUX2;
            break;
        case 1:
            enStbLayer = E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2;
            break;
        case 2:
            enStbLayer = E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2;
            break;
        case 3:
        case 4:
        case 5:
            enStbLayer = E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO;
            break;
        default:
            printf("[%s]found wrong layer Setting %u, it can't be return by this interface.\n",__FUNCTION__,u16RegVal);
            enStbLayer = E_VOP_LAYER_RESERVED;
        break;
    }
    return enStbLayer;
#else
    return Hal_SC_Get_OSDLayer(pInstance, eWindow);
#endif
}

E_APIXC_ReturnValue MDrv_XC_SetVideoAlpha(void *pInstance, MS_U8 u8Val, SCALER_WIN eWindow)
{
    return Hal_SC_Set_VideoAlpha(pInstance, u8Val, eWindow);
}

E_APIXC_ReturnValue MDrv_XC_GetVideoAlpha(void *pInstance, MS_U8 *pu8Val, SCALER_WIN eWindow)
{
    return Hal_SC_Get_VideoAlpha(pInstance, pu8Val, eWindow);
}

E_APIXC_ReturnValue MDrv_SC_ShiftDisplayWindowDirectly(void *pInstance, MS_S8 s8H, MS_S8 s8V, MS_S8 s8preH, MS_S8 s8preV)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_WINDOW_TYPE dsp_main, dsp_sub;
    MS_S8 s8dH = s8H - s8preH, s8dV = s8V - s8preV;
    MS_U16 u16Data_OSD_hs_st=0, u16Data_OSD_hs_end=0;
    MS_U16 u16Data_OSD_vfde_st=0, u16Data_OSD_vfde_end=0;
    MS_U8 u8OSD_vdelay = 0;
    MS_WINDOW_TYPE RepWindow;
    MS_WINDOW_TYPE OSDWindow;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    if (IsFSCDisplayWindowEnable())
    {
        if((pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_DO_OVERSCAN) == PIXEL_SHIFT_DO_OVERSCAN)
        {
            // just do overscan led display window bigger
            dsp_main.x = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_03_L) - pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange / 2;
            dsp_main.y = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0B_L) - pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange / 2;
        }
        else
        {
            dsp_main.x = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_03_L);
            dsp_main.y = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK68_0B_L);
        }
        if(MHal_FRC_IsFHDToFSC(pInstance, MAIN_WINDOW))
        {
            if((pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_DO_OVERSCAN) == PIXEL_SHIFT_DO_OVERSCAN)
            {
                //just do overscan led display window bigger
                dsp_main.width = (gSrcInfo[MAIN_WINDOW].stDispWin.width - pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange)/2 + pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange;
                dsp_main.height = (gSrcInfo[MAIN_WINDOW].stDispWin.height - pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange)/2 + pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange;
            }
            else
            {
                dsp_main.width = gSrcInfo[MAIN_WINDOW].stDispWin.width/2;
                dsp_main.height = gSrcInfo[MAIN_WINDOW].stDispWin.height/2;
            }
        }
        else//4K2k IN FSC
        {
            dsp_main.width =  gSrcInfo[MAIN_WINDOW].stDispWin.width;
            dsp_main.height = gSrcInfo[MAIN_WINDOW].stDispWin.height;
        }
    }
    else
#endif
    {
        memcpy(&dsp_main, &(gSrcInfo[MAIN_WINDOW].stDispWin), sizeof(MS_WINDOW_TYPE));
    }
    memcpy(&dsp_sub, &(gSrcInfo[SUB_WINDOW].stDispWin), sizeof(MS_WINDOW_TYPE));
     // BEWARE OF THE MIRROR CASE

    // DISPLAY MAIN WINDOW is controlled here
    if ((pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_MAIN_WINDOW) == PIXEL_SHIFT_MOVE_MAIN_WINDOW)
    {
#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
        if (IsFSCDisplayWindowEnable())
        {
            // FSC 2p mode,so H just can move even
            s8H &= ~0x1;
            dsp_main.x += s8H;
            dsp_main.y += s8V;
        }
        else
#endif
        {
            dsp_main.x += s8dH;
            dsp_main.y += s8dV;
        }
    }
#if defined(XC_SUPPORT_2STEP_SCALING)
    //2stepscaling is 2p mode,H just can move even
    s8H &= ~0x1;
#endif
    // DISPLAY SUB WINDOW is controlled here
    if ((pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_SUB_WINDOW) == PIXEL_SHIFT_MOVE_SUB_WINDOW)
    {
        dsp_sub.x += s8dH;
        dsp_sub.y += s8dV;
    }

    // REPORT WINDOW is controlled here
    if ((pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_REPORT_WINDOW) == PIXEL_SHIFT_MOVE_REPORT_WINDOW)
    {
        RepWindow.x = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16HStart+ s8H;
        RepWindow.width = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Width;
        RepWindow.y = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart+ s8V;
        RepWindow.height = pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;
    }

    // OSD LAYER is controlled here
    if ((pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_OSD_LAYER) == PIXEL_SHIFT_MOVE_OSD_LAYER)
    {
        if((s8H == 0) && (s8V == 0))
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, 0, BIT(15));
        else
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, BIT(15), BIT(15));

        if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, BIT(15)))
        {
            if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK19_10_L, BIT(7)))
                u8OSD_vdelay +=2;

            if (SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK18_75_L, BIT(0)))
                u8OSD_vdelay +=1;

            u16Data_OSD_vfde_st =  pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16VStart + s8V + u8OSD_vdelay;
            u16Data_OSD_vfde_end = u16Data_OSD_vfde_st + pXCResourcePrivate->stdrvXC_MVideo_Context.g_XC_InitData.stPanelInfo.u16Height;

            if( (-1 <= s8H) && (s8H <= (pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange/2)) )
            {
                u16Data_OSD_hs_st = 0;
                u16Data_OSD_hs_end = 1 + s8H;
            }
            else if( (-(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange/2) <= s8H) && (s8H <= -2) )
            {
                // REG_SC_BK10_0C_L: Expect H total - 1
                // REG_SC_BK10_0E_L: Real H total - 1
                MS_U16 u16HTotal = SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_0E_L) + 1;
                u16Data_OSD_hs_st = 1;
                u16Data_OSD_hs_end = u16HTotal + 1 + s8H;
            }
            else
            {
                u16Data_OSD_hs_st = 0;
                u16Data_OSD_hs_end = 1;
            }
            OSDWindow.x = u16Data_OSD_hs_st;
            OSDWindow.y = u16Data_OSD_vfde_st;
            OSDWindow.width = u16Data_OSD_hs_end + 1 - u16Data_OSD_hs_st;
            OSDWindow.width = u16Data_OSD_vfde_end + 1 - u16Data_OSD_vfde_st;
        }
    }


    // finally we can update windows we want.
    MDrv_XC_UpdateWindow(pInstance,
                         NULL, // always don't change DE in pixel shift case
                        (pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_MAIN_WINDOW)       ? &dsp_main : NULL,
                        (pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_SUB_WINDOW)        ? &dsp_sub  : NULL,
                        (pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_REPORT_WINDOW)     ? &RepWindow : NULL,
                        (pXCResourcePrivate->stdrvXC_MVideo.gePixelShiftFeature & PIXEL_SHIFT_MOVE_OSD_LAYER)         ? &OSDWindow : NULL);

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MDrv_XC_Combine_MLoadEn(void *pInstance, MS_BOOL bEn)
{

    if(bEn == FALSE)
    {
        if(_u16ZorderMLoadCmdCnt)
        {
            MApi_XC_MLoad_WriteCmds_And_Fire_U2(pInstance,
                &_u32ZorderMLoadCmd[0], &_u16ZorderMLoadVal[0], &_u16ZorderMLoadMsk[0], _u16ZorderMLoadCmdCnt);
            _u16ZorderMLoadCmdCnt = 0;
        }
    }
    _bZorderMLoadEn = bEn;
    return E_APIXC_RET_OK;
}

#ifdef UFO_XC_ZORDER
E_APIXC_ReturnValue MDrv_SC_GetVideoOnOSD(void *pInstance, E_VIDEO_ON_OSD_LAYER *penLayer, SCALER_WIN eWindow)
{
    if(MDrv_SYS_GetChipType() == E_SYS_CHIP_TV) // TV zorder
    {
#if (VIDEO_OSD_SWITCH_VER > 0)
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
        MS_U8 u8Layer = 0;
        MS_U8 u8Alpha = 0;
        MS_BOOL bEnable = FALSE;
        MS_U32 u32Addr[] = {REG_SC_BK10_6B_L, REG_SC_BK10_60_L,
                            REG_SC_BK10_61_L, REG_SC_BK10_62_L,
                            REG_SC_BK10_1B_L};
        MS_U16 u16BlendingModeMask[] = {BIT(5)|BIT(4), BIT(7)|BIT(6),
                                        BIT(9)|BIT(8), BIT(13)|BIT(12),
                                        BIT(15)|BIT(14)};
#if (VIDEO_OSD_SWITCH_VER < 2) // unsupport sub blending
        if ((eWindow == SUB_WINDOW) || (psXCInstPri->u32DeviceID == 1))
        {
            // do nothing
            printf("[GetVideoOnOSD] Not support sub window on this chip \n");
            return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
#endif

        for (u8Layer = 0; u8Layer < OSD_LAYER_NUM; u8Layer++)
        {
            if ((MAIN_WINDOW == eWindow) && (psXCInstPri->u32DeviceID == 0))
            {
#if (VIDEO_OSD_SWITCH_VER < 2)  // ver 1
                bEnable = SC_R2BYTEMSK(0, u32Addr[u8Layer], BIT(8)) == BIT(8);
#elif (VIDEO_OSD_SWITCH_VER < 3)    // ver 2
                bEnable = SC_R2BYTEMSK(0, u32Addr[u8Layer], BIT(8)|BIT(9)) == (BIT(8)|BIT(9));
#endif
            }
            else
            {
                bEnable = SC_R2BYTEMSK(0, u32Addr[u8Layer], BIT(8)|BIT(10)) == (BIT(8)|BIT(10));
            }
            if (!bEnable)
            {
                break;
            }
            u8Alpha = SC_R2BYTEMSK(0, u32Addr[u8Layer], BITMASK(7:0));
            if ((SC_R2BYTE(0, REG_SC_BK10_23_L) & BIT(5)) || (SC_R2BYTE(0, REG_SC_BK10_1C_L) & BIT(u8Layer)))
            {
                u8Alpha = 0xFF - u8Alpha;
            }
            // check mode1. If on, alpha inverse
            if ((SC_R2BYTE(0, REG_SC_BK10_5B_L) & u16BlendingModeMask[u8Layer]) == u16BlendingModeMask[u8Layer])
            {
                u8Alpha = 0xFF - u8Alpha;
            }
            if (u8Alpha != 0)
            {
                break;
            }
        }
        *penLayer = (E_VIDEO_ON_OSD_LAYER)u8Layer;
        return E_APIXC_RET_OK;
#else
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
    }
    else // STB zorder
    {
        MS_U16 u16RegVal = SC_R2BYTEMSK(0, REG_SC_BK2F_36_L, BITMASK(2:0));;
        if (eWindow == SUB_WINDOW)
        {
            // do nothing
            printf("[GetVideoOnOSD] Not support sub window on this chip \n");
            return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
#if (VIDEO_OSD_SWITCH_VER == 1)
        E_VOP_OSD_LAYER_SEL enVopOsdLayer ;
        enVopOsdLayer = Hal_SC_Get_OSDLayer(pInstance, eWindow);
        switch (enVopOsdLayer)
        {
            case E_VOP_LAYER_VIDEO_MUX1_MUX2 :
            case E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2 :
            case E_VOP_LAYER_FRAME_VIDEO_MUX2_MUX1 :
                *penLayer = E_VIDEO_ON_OSD_LAYER_0;
                break;
            case E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2 :
            case E_VOP_LAYER_FRAME_MUX2_VIDEO_MUX1 :
                *penLayer = E_VIDEO_ON_OSD_LAYER_1;
                break;
            case E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO :
            case E_VOP_LAYER_FRAME_MUX2_MUX1_VIDEO :
                *penLayer = E_VIDEO_ON_OSD_LAYER_2;
                break;
            default:
                return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
#endif
#if(VIDEO_OSD_SWITCH_VER >= 2) // clippers mode
        switch (u16RegVal) // translate layer to register value
        {
            case 0x0:
            case 0x1:
                *penLayer = E_VIDEO_ON_OSD_LAYER_DEAFULT;
                break;
            case 0x2:
                *penLayer = E_VIDEO_ON_OSD_LAYER_0;
                break;
            case 0x3:
                *penLayer = E_VIDEO_ON_OSD_LAYER_1;
                break;
            case 0x4:
                *penLayer = E_VIDEO_ON_OSD_LAYER_2;
                break;
            case 0x5:
                *penLayer = E_VIDEO_ON_OSD_LAYER_3;
                break;
            default:
                return E_APIXC_RET_FAIL;
                break;
        }
        return E_APIXC_RET_OK;
#else
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
    }
}
#endif

E_APIXC_ReturnValue MDrv_SC_SetVideoOnOSD(void *pInstance, E_VIDEO_ON_OSD_LAYER enlayer, SCALER_WIN eWindow)
{
#if (VIDEO_OSD_SWITCH_VER > 0)
    if (MDrv_SYS_GetChipType() == E_SYS_CHIP_TV)
    {
        if ((enlayer >= E_VIDEO_ON_OSD_LAYER_NUM) || (eWindow >= MAX_WINDOW))
        {
            return E_APIXC_RET_FAIL;
        }

        switch (enlayer) // enable bleanding layers
        {
            case E_VIDEO_ON_OSD_LAYER_4:
                MDrv_SC_SetOSDLayerAlpha(pInstance, 4, 0);
                MDrv_SC_SetOSDLayerBlending(pInstance, 4, TRUE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_3:
                MDrv_SC_SetOSDLayerAlpha(pInstance, 3, 0);
                MDrv_SC_SetOSDLayerBlending(pInstance, 3, TRUE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_2:
                MDrv_SC_SetOSDLayerAlpha(pInstance, 2, 0);
                MDrv_SC_SetOSDLayerBlending(pInstance, 2, TRUE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_1:
                MDrv_SC_SetOSDLayerAlpha(pInstance, 1, 0);
                MDrv_SC_SetOSDLayerBlending(pInstance, 1, TRUE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_0:
                MDrv_SC_SetOSDLayerAlpha(pInstance, 0, 0);
                MDrv_SC_SetOSDLayerBlending(pInstance, 0, TRUE, eWindow);
            default:
                break;
        }
        switch (enlayer) // disable bleanding layers
        {
            default:
                MDrv_SC_SetOSDLayerBlending(pInstance, 0, FALSE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_0:
                MDrv_SC_SetOSDLayerBlending(pInstance, 1, FALSE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_1:
                MDrv_SC_SetOSDLayerBlending(pInstance, 2, FALSE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_2:
                MDrv_SC_SetOSDLayerBlending(pInstance, 3, FALSE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_3:
                MDrv_SC_SetOSDLayerBlending(pInstance, 4, FALSE, eWindow);
            case E_VIDEO_ON_OSD_LAYER_4:
                break;
        }
        return E_APIXC_RET_OK;
    }
    else // BOX zorder
    {
        MS_U16 u16RegVal = 0;
        if ((enlayer >= E_VIDEO_ON_OSD_LAYER_NUM) || (eWindow >= MAX_WINDOW))
        {
            return E_APIXC_RET_FAIL_INVALID_PARAMETER;
        }
        if (eWindow == SUB_WINDOW)
        {
            // do nothing
            printf("[SetVideoOnOSD] Not support sub window on this chip \n");
            return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
        }
#if (VIDEO_OSD_SWITCH_VER == 1) // kaiser mode
        E_APIXC_ReturnValue eResult = E_APIXC_RET_FAIL;
        E_VOP_OSD_LAYER_SEL enVopOsdLayer = Hal_SC_Get_OSDLayer(pInstance, eWindow);
        if (enVopOsdLayer >= E_VOP_LAYER_RESERVED)
        {
            // do nothing
            printf("[%s] error layer id[%u]\n",__FUNCTION__,enVopOsdLayer);
            return E_APIXC_RET_FAIL;
        }
        MS_BOOL bMux1Top = (enVopOsdLayer == E_VOP_LAYER_FRAME_VIDEO_MUX2_MUX1)
                        || (enVopOsdLayer == E_VOP_LAYER_FRAME_MUX2_VIDEO_MUX1)
                        || (enVopOsdLayer == E_VOP_LAYER_FRAME_MUX2_MUX1_VIDEO);
        switch (enlayer) // translate layer to register value
        {
            case E_VIDEO_ON_OSD_LAYER_DEAFULT:
                if ((enVopOsdLayer == E_VOP_LAYER_VIDEO_MUX1_MUX2)
                 || (enVopOsdLayer == E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2)
                 || (enVopOsdLayer == E_VOP_LAYER_FRAME_VIDEO_MUX2_MUX1))
                {
                    // do nothing, layer right
                }
                else
                {
                    if (bMux1Top)
                    {
                        eResult = Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_VIDEO_MUX2_MUX1, eWindow);
                    }
                    else
                    {
                        eResult = Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_VIDEO_MUX1_MUX2, eWindow);
                    }
                }
                break;
            case E_VIDEO_ON_OSD_LAYER_0:
                if ((enVopOsdLayer == E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2)
                 || (enVopOsdLayer == E_VOP_LAYER_FRAME_MUX2_VIDEO_MUX1))
                {
                    // do nothing, layer right
                }
                else
                {
                    if (bMux1Top)
                    {
                        eResult = Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_MUX2_VIDEO_MUX1, eWindow);
                    }
                    else
                    {
                        eResult = Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_MUX1_VIDEO_MUX2, eWindow);
                    }
                }
                break;
            case E_VIDEO_ON_OSD_LAYER_1:
            case E_VIDEO_ON_OSD_LAYER_2:
            case E_VIDEO_ON_OSD_LAYER_3:
            case E_VIDEO_ON_OSD_LAYER_4:
            default:
                if ((enVopOsdLayer == E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO)
                 || (enVopOsdLayer == E_VOP_LAYER_FRAME_MUX2_MUX1_VIDEO))
                {
                    // do nothing, layer right
                }
                else
                {
                    if (bMux1Top)
                    {
                        eResult = Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_MUX2_MUX1_VIDEO, eWindow);
                    }
                    else
                    {
                        eResult = Hal_SC_Set_OSDLayer(pInstance, E_VOP_LAYER_FRAME_MUX1_MUX2_VIDEO, eWindow);
                    }
                }
                break;
        }
        return E_APIXC_RET_OK;
#endif
#if (VIDEO_OSD_SWITCH_VER >= 2) // clippers mode
        switch (enlayer) // translate layer to register value
        {
            case E_VIDEO_ON_OSD_LAYER_DEAFULT:
                u16RegVal = 0;
                break;
            case E_VIDEO_ON_OSD_LAYER_0:
                u16RegVal = 2;    //FrameColor->GOP1->Video->GOP2->GOP3->GOP4
                break;
            case E_VIDEO_ON_OSD_LAYER_1:
                u16RegVal = 3;    //FrameColor->GOP1->GOP2->Video->GOP3->GOP4
                break;
            case E_VIDEO_ON_OSD_LAYER_2:
                u16RegVal = 4;    //FrameColor->GOP1->GOP2->GOP3->Video->GOP4
                break;
            case E_VIDEO_ON_OSD_LAYER_3:
                u16RegVal = 5;    //FrameColor->GOP1->GOP2->GOP3->GOP4->Video
                break;
            case E_VIDEO_ON_OSD_LAYER_4:
            default:
                u16RegVal = 5;    //FrameColor->GOP1->GOP2->GOP3->GOP4->Video
                break;
        }
    #if (VIDEO_OSD_SWITCH_VER == 3) // kano mode
        MS_BOOL bCtrlByGop = (SC_R2BYTEMSK(0, REG_SC_BK2F_36_L, BIT(5))) == BIT(5);
        if (bCtrlByGop)
        {
            GOP_W2BYTEMSK(_PK_L_(0x0,0x6B), 1<<13, 1<<13);         //double buffer
            GOP_W2BYTEMSK(_PK_L_(0x0,0x6A), u16RegVal, 0x0007);
            GOP_W2BYTEMSK(_PK_L_(0x0,0x6B), 1<<15, 1<<15);         //trigger
        }
        else
    #endif
        {
            SC_W2BYTEMSK(0, REG_SC_BK2F_36_L, u16RegVal, 0x7);
        }
#else
        return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
        return E_APIXC_RET_OK;
    }
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
    return E_APIXC_RET_OK;

}

E_APIXC_ReturnValue MDrv_SC_SetOSDLayerBlending(void *pInstance, MS_U8 u8Layer, MS_BOOL bEnable, SCALER_WIN eWindow)
{
#if (VIDEO_OSD_SWITCH_VER > 0)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32Addr[] = {REG_SC_BK10_6B_L, REG_SC_BK10_60_L,
                        REG_SC_BK10_61_L, REG_SC_BK10_62_L,
                        REG_SC_BK10_1B_L};
    if ((u8Layer >= OSD_LAYER_NUM) || (eWindow >= MAX_WINDOW))
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }

#if (VIDEO_OSD_SWITCH_VER > 1) // correct register init value
    if (0 == (SC_R2BYTE(psXCInstPri->u32DeviceID, u32Addr[u8Layer]) & BIT(8)))
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], BIT(8), BIT(10)|BIT(9)|BIT(8));
    }
#endif

    if (MAIN_WINDOW == eWindow)
    {
    #if (VIDEO_OSD_SWITCH_VER < 2)  // ver 1
        if(_bZorderMLoadEn)
        {
            _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
            _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=(bEnable<<8);
            _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BIT(8);
            _u16ZorderMLoadCmdCnt++;
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], (bEnable<<8), BIT(8));
        }
    #elif (VIDEO_OSD_SWITCH_VER < 3)    // ver 2
        if(_bZorderMLoadEn)
        {
            _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
            _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=(bEnable<<9)|BIT(8);
            _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BIT(9)|BIT(8);
            _u16ZorderMLoadCmdCnt++;
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], (bEnable<<9)|BIT(8), BIT(9)|BIT(8));
        }
    #endif
    }
    #if (VIDEO_OSD_SWITCH_VER > 1)  // ver > 1, with sub blending
    else
    {
        if(_bZorderMLoadEn)
        {
            _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
            _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=(bEnable<<10)|BIT(8);
            _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BIT(10)|BIT(8);
            _u16ZorderMLoadCmdCnt++;
        }
        else
        {
            SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], (bEnable<<10)|BIT(8), BIT(10)|BIT(8));
        }
    }
    #endif
    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MDrv_SC_SetOSDLayerAlpha(void *pInstance, MS_U8 u8Layer, MS_U8 u8Alpha)
{
#if (VIDEO_OSD_SWITCH_VER > 0)
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U32 u32Addr[] = {REG_SC_BK10_6B_L, REG_SC_BK10_60_L,
                        REG_SC_BK10_61_L, REG_SC_BK10_62_L,
                        REG_SC_BK10_1B_L};
    MS_U16 u16BlendingModeMask[] = {BIT(5)|BIT(4), BIT(7)|BIT(6),
                                    BIT(9)|BIT(8), BIT(13)|BIT(12),
                                    BIT(15)|BIT(14)};
    if ((u8Layer >= OSD_LAYER_NUM))
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }
    if ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_23_L) & BIT(5)) || (SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_1C_L) & BIT(u8Layer)))
    {
        u8Alpha = 0xFF - u8Alpha;
    }
    // check mode1. If on, alpha inverse
    if ((SC_R2BYTE(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L) & u16BlendingModeMask[u8Layer]) == u16BlendingModeMask[u8Layer])
    {
        u8Alpha = 0xFF - u8Alpha;
    }
    if(_bZorderMLoadEn)
    {
        _u32ZorderMLoadCmd[_u16ZorderMLoadCmdCnt]=u32Addr[u8Layer];
        _u16ZorderMLoadVal[_u16ZorderMLoadCmdCnt]=u8Alpha;
        _u16ZorderMLoadMsk[_u16ZorderMLoadCmdCnt]=BITMASK(7:0);
        _u16ZorderMLoadCmdCnt++;
    }
    else
    {
        SC_W2BYTEMSK(psXCInstPri->u32DeviceID, u32Addr[u8Layer], u8Alpha, BITMASK(7:0));
    }

    return E_APIXC_RET_OK;
#else
    return E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
#endif
}

E_APIXC_ReturnValue MDrv_SC_SetOSDBlendingFormula(void *pInstance, E_XC_OSD_INDEX enOsdIndex, E_XC_OSD_BlENDING_TYPE enType, SCALER_WIN eWindow)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    E_APIXC_ReturnValue bRet = E_APIXC_RET_OK;
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if ((enOsdIndex >= OSD_LAYER_NUM) || (enType >= E_XC_OSD_BlENDING_TYPE_NUM) || (eWindow >= MAX_WINDOW))
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }
    bRet = Hal_SC_SetOSDBlendingFormula(pInstance, enOsdIndex, enType, eWindow);
    return bRet;
#if 0
    if (eWindow == MAIN_WINDOW)
    {
        switch(enOsdIndex)
        {
            case E_XC_OSD_0:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(5)|BIT(4), BIT(5)|BIT(4));
                    //osd check alpha define
                    //1: for define 8'hFF is transparent
                    //0: for define 8'h00 is transparent
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, BIT(0), BIT(0));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_6B_L, BIT(15), BIT(15));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef GOP_SUPPORT_DUALRATE
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BKC9_50_L, BIT(10), BIT(10));
#endif
#endif
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(5)|BIT(4));
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, 0x00, BIT(0));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_6B_L, 0, BIT(15));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef GOP_SUPPORT_DUALRATE
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BKC9_50_L, 0, BIT(10));
#endif
#endif
                }
                break;

            case E_XC_OSD_1:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(7)|BIT(6), BIT(7)|BIT(6));
                    //osd check alpha define
                    //1: for define 8'hFF is transparent
                    //0: for define 8'h00 is transparent
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, BIT(1), BIT(1));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_60_L, BIT(15), BIT(15));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef GOP_SUPPORT_DUALRATE
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BKC9_51_L, BIT(10), BIT(10));
#endif
#endif
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(7)|BIT(6));
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, 0x00, BIT(1));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_60_L, 0, BIT(15));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef GOP_SUPPORT_DUALRATE
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BKC9_51_L, 0, BIT(10));
#endif
#endif
                }
                break;

            case E_XC_OSD_2:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(9)|BIT(8), BIT(9)|BIT(8));
                    //osd check alpha define
                    //1: for define 8'hFF is transparent
                    //0: for define 8'h00 is transparent
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, BIT(2), BIT(2));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_61_L, BIT(15), BIT(15));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef GOP_SUPPORT_DUALRATE
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BKC9_52_L, BIT(10), BIT(10));
#endif
#endif
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(9)|BIT(8));
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, 0x00, BIT(2));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_61_L, 0, BIT(15));

#if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
#ifdef GOP_SUPPORT_DUALRATE
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BKC9_52_L, 0, BIT(10));
#endif
#endif
                }
                break;
#if (OSD_LAYER_NUM > 3)
            case E_XC_OSD_3:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(13)|BIT(12), BIT(13)|BIT(12));
                    //osd check alpha define
                    //1: for define 8'hFF is transparent
                    //0: for define 8'h00 is transparent
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, BIT(3), BIT(3));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_62_L, BIT(15), BIT(15));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(13)|BIT(12));
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, 0x00, BIT(3));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_62_L, 0, BIT(15));
                }
                break;
#endif
#if (OSD_LAYER_NUM > 4)
            case E_XC_OSD_4:
                if(E_XC_OSD_BlENDING_MODE1 == enType) //OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, BIT(15)|BIT(14), BIT(15)|BIT(14));
                    //osd check alpha define
                    //1: for define 8'hFF is transparent
                    //0: for define 8'h00 is transparent
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, BIT(4), BIT(4));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1B_L, BIT(15), BIT(15));
                }
                else //a*OSD + (1-a)*Video
                {
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_5B_L, 0x00, BIT(15)|BIT(14));
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK0F_6B_L, 0x00, BIT(4));
                    // for zorder
                    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK10_1B_L, 0, BIT(15));
                }
                break;
#endif
            default:
                printf("[MDrv_SC_SetOSDBlendingFormula]Invalid Parameter!enOsdIndex=%u\n", enOsdIndex);
                bRet = E_APIXC_RET_FAIL_INVALID_PARAMETER;
                break;
        }
    }
    else
    {
        // do nothing
        printf("[MDrv_SC_SetOSDBlendingFormula] Not support sub window now \n");
        bRet = E_APIXC_RET_FAIL_FUN_NOT_SUPPORT;
    }

    return bRet;
#endif
}


void MDrv_SC_SetMCDIBufferAddress(void *pInstance, MS_PHY u32FBAddress, MS_PHY u32FBSize, E_XC_MCDI_TYPE eType)
{
    MS_U8 u8MIUSel;
    SCALER_WIN _eWindow = MAIN_WINDOW;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    MS_PHY phyFBAddressFromReg;

    if (eType >= E_XC_MCDI_MAX)
    {
        XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"the MDCI type(%d) is invalid for frame buffer setting \n",eType);
        return;
    }

    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    phyFBAddressFromReg = Hal_SC_get_mcdi_memoryaddressfromreg(pInstance, eType);
    if((pXCResourcePrivate->stdrvXC_Display._u32MCDIFBAddress[eType]==u32FBAddress)
        && (pXCResourcePrivate->stdrvXC_Display._u32MCDIFBSize[eType]==u32FBSize)
        && (phyFBAddressFromReg == u32FBAddress))
   {
        return;
    }

    pXCResourcePrivate->stdrvXC_Display._u32MCDIFBAddress[eType]=u32FBAddress;
    pXCResourcePrivate->stdrvXC_Display._u32MCDIFBSize[eType]=u32FBSize;

    if (eType <= E_XC_MCDI_BOTH)
    {
        _eWindow = MAIN_WINDOW;
    }
    else if ( (eType > E_XC_MCDI_BOTH) && (eType <= E_XC_MCDI_SUB_BOTH))
    {
        _eWindow = SUB_WINDOW;
    }

    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBAddress_Suspend[eType] = u32FBAddress;
    pXCResourcePrivate->stdrvXC_MVideo_Context.g_u32MCDIFBSize_Suspend[eType] = u32FBSize;

    _phy_to_miu_offset(u8MIUSel, u32FBAddress, u32FBAddress);

    Hal_SC_Enable_MiuMask(pInstance);

    Hal_SC_set_mrq_miusel(pInstance, u8MIUSel);

    Hal_SC_set_mcdi_memoryaddress(pInstance, u32FBAddress, eType);

    Hal_SC_set_mcdi_write_limit(pInstance, ENABLE, (u32FBAddress + u32FBSize), eType);

    Hal_SC_Disable_MiuMask(pInstance);
#if defined (__aarch64__)
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change MCDI mode = %d  FB_Start_Addr=%lx, Size=%lx\n",eType ,u32FBAddress, u32FBSize);
#else
    XC_LOG_TRACE(XC_DBGLEVEL_SETWINDOW,"Change MCDI mode = %d  FB_Start_Addr=%tx, Size=%tx\n",eType ,(ptrdiff_t)u32FBAddress, (ptrdiff_t)u32FBSize);
#endif
}

void MDrv_SC_EnableMCDI(void *pInstance, MS_BOOL bEnable, E_XC_MCDI_TYPE eType)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if (E_POWER_SUSPEND != pXCResourcePrivate->stdrvXC_MVideo._enPrevPowerState)
    {
        pXCResourcePrivate->stdrvXC_MVideo_Context.g_bEnableMCDI_Suspend[eType] = bEnable;
    }
    Hal_SC_enable_mcdi(pInstance, bEnable, eType);
}

//-------------------------------------------------------------------------------------------------
/// Adjust OSD DE reference position
/// @param bEnable allow OSD position reference enable or not
/// @param VStart the vertical starting position for OSD reference
/// @param VEnd the vertical ending position for OSD reference
/// @param HStart the horizontal starting position for OSD reference
/// @param HEnd the horizontal ending position for OSD reference
//-------------------------------------------------------------------------------------------------
void MDrv_XC_3D_AdjustOSDVDE(void *pInstance, MS_BOOL bEnable, MS_U16 VStart, MS_U16 VEnd, MS_U16 HStart, MS_U16 HEnd)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, HStart, BMASK(12:0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_11_L, HEnd, BMASK(12:0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_12_L, VStart, BMASK(11:0));
    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_13_L, VEnd, BMASK(11:0));

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK2D_10_L, bEnable?BIT(15):0, BIT(15));
}

E_APIXC_ReturnValue MDrv_SC_ForceReadFrame(void *pInstance, MS_BOOL bEnable, MS_U16 u16FrameIndex)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    MS_U16 u16value = (bEnable << 11) | ((u16FrameIndex & 0x07) << 8);

    SC_W2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK12_07_L, u16value, 0x0F00);

    return E_APIXC_RET_OK;
}

//-------------------------------------------------------------------------------------------------
/// Get XC free run status
/// @return TRUE if it is free run, FALSE if not
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_XC_GetFreeRunStatus_U2(void* pInstance)
{
    MS_BOOL bSourceSyncEn = FALSE, bFrameLPLLEn = FALSE;

    _XC_ENTRY(pInstance);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    bSourceSyncEn = (MS_BOOL)(SC_R2BYTEMSK(psXCInstPri->u32DeviceID, REG_SC_BK01_03_L, BIT(15)) >> 15);

    LPLL_BK_STORE;
    LPLL_BK_SWITCH(0x00);
    bFrameLPLLEn = (MS_BOOL)(MDrv_ReadRegBit(L_BK_LPLL(0x0C), BIT(3)) >> 3);
    LPLL_BK_RESTORE;

    _XC_RETURN(pInstance);

    if (!bSourceSyncEn || !bFrameLPLLEn)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

MS_BOOL MApi_XC_GetFreeRunStatus(void)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stXC_GET_FREERUN_STATUS XCArgs;
    XCArgs.bReturnValue = FALSE;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_GET_FREERUN_STATUS, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
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
/// MApi_XC_BYPASS_SetCSC
/// @param  bEnable                              \b IN: ENABLE/DISABLE the color space convert
//-------------------------------------------------------------------------------------------------
void MApi_XC_BYPASS_SetCSC_U2(void* pInstance, MS_BOOL bEnable)
{
    _XC_ENTRY(pInstance);
    MHal_XC_BYPASS_SetCSC(pInstance, bEnable);
    _XC_RETURN(pInstance);
}

void MApi_XC_BYPASS_SetCSC(MS_BOOL bEnable)
{
    if (pu32XCInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }

    stXC_SET_BYPASS_CSC XCArgs;
    XCArgs.bEnable = bEnable;

    if(UtopiaIoctl(pu32XCInst, E_XC_CMD_SET_BYPASS_CSC, (void*)&XCArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("Obtain XC engine fail\n");
        return;
    }
    else
    {
        return;
    }
}

void MDrv_SC_EnableReadBankSwitchControl(void* pInstance, MS_BOOL bEnable, MS_U16 u16SwitchCnt, SCALER_WIN eWindow)
{
    MHal_SC_set_manual_rbank_switch_cnt(pInstance, bEnable, u16SwitchCnt, eWindow);
}

E_APIXC_ReturnValue MDrv_XC_SetPixelShift(void* pInstance, MS_S8 s8H, MS_S8 s8V)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if((s8V != 0) && (gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming.u16VStart < pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange))
    {
        //If first adjust V, and default vstart <gu16VPixelShiftRange, reset all XC window setting
        MS_WINDOW_TYPE stDEWin;
        memcpy(&stDEWin, &(gSrcInfo[MAIN_WINDOW].Status2.stXCPanelDefaultTiming), sizeof(MS_WINDOW_TYPE));
        gSrcInfo[MAIN_WINDOW].stDispWin.y -= stDEWin.y;
        gSrcInfo[SUB_WINDOW].stDispWin.y -= stDEWin.y;
        stDEWin.y = pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange;
        gSrcInfo[MAIN_WINDOW].stDispWin.y += stDEWin.y;
        gSrcInfo[SUB_WINDOW].stDispWin.y += stDEWin.y;
        MDrv_XC_UpdateWindow(pInstance, &stDEWin, &(gSrcInfo[MAIN_WINDOW].stDispWin), &(gSrcInfo[SUB_WINDOW].stDispWin), &stDEWin, NULL);
    }

    if ((s8H >= -(pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange/2))
        && (s8H <= (pXCResourcePrivate->stdrvXC_MVideo.gu16HPixelShiftRange/2))
        && (s8V >= -(pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange/2)) &&
        (s8V <= (pXCResourcePrivate->stdrvXC_MVideo.gu16VPixelShiftRange/2)))
    {
        if (MDrv_XC_GetMirrorModeTypeEx(pInstance, MAIN_WINDOW) == MIRROR_HV)
        {
            s8H = s8H * -1;
            s8V = s8V * -1;
            printf("Mirror! H=%d,V=%d\n", s8H, s8V);
        }

        eRet = MDrv_SC_ShiftDisplayWindowDirectly(pInstance, s8H, s8V, pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift, pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift);
        pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift = s8H;
        pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift = s8V;
    }
    else
    {
        pXCResourcePrivate->stdrvXC_MVideo.s8HPixelShift = 0;
        pXCResourcePrivate->stdrvXC_MVideo.s8VPixelShift = 0;
        printf("Error: %s (%d, %d): Invalid value range.\n", __FUNCTION__, s8H, s8V);
    }

    return eRet;
}

void MDrv_SC_Get_GOPOffset(MS_U16 *u16OffsetH, MS_U16 *u16OffsetV)
{
    *u16OffsetH = 0;
    *u16OffsetV = 0;

    #if (FRC_INSIDE && (HW_DESIGN_4K2K_VER == 6))
    void *pInstance = pu32XCInst_private;
    if (pu32XCInst_private == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MApi_XC_Init() first\n",__FUNCTION__,__LINE__);
        return;
    }
    _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_VAR);
    _XC_ENTRY(pInstance);
    MHal_FRC_GetGOPOffset(pInstance, u16OffsetH, u16OffsetV);
    _XC_RETURN(pInstance);
    _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_VAR);
    #endif
}

#ifdef UFO_XC_AUTO_DOWNLOAD
static MS_BOOL _MDrv_XC_AutoDownload_Convert2KDrvClient(EN_XC_AUTODOWNLOAD_CLIENT enClient, EN_KDRV_XC_AUTODOWNLOAD_CLIENT *penClient)
{
    if (penClient == NULL)
    {
        return FALSE;
    }

    switch(enClient)
    {
        case E_XC_AUTODOWNLOAD_CLIENT_HDR:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_HDR;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_OP2GAMMA;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_FRCOP2GAMMA;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_XVYCC:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_XVYCC;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE1:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_ODTABLE1;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE2:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_ODTABLE2;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_ODTABLE3:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_ODTABLE3;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_DEMURA:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_DEMURA;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_OP2LUT:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_OP2LUT;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_0:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_T3D_0;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_T3D_1:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_T3D_1;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_FRCSPTPOPM;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_FOOPM:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_FOOPM;
            break;
        }
        case E_XC_AUTODOWNLOAD_CLIENT_MAX:
        {
            *penClient = E_KDRV_XC_AUTODOWNLOAD_CLIENT_MAX;
            break;
        }
        default:
        {
            return FALSE;
            break;
        }

    }

    return TRUE;
}

static MS_BOOL _MDrv_XC_AutoDownload_Convert2KDrvMode(EN_XC_AUTODOWNLOAD_MODE enMode, EN_KDRV_XC_AUTODOWNLOAD_MODE *penMode)
{
    if (penMode == NULL)
    {
        return FALSE;
    }

    switch (enMode)
    {
        case E_XC_AUTODOWNLOAD_TRIGGER_MODE:
        {
            *penMode = E_KDRV_XC_AUTODOWNLOAD_TRIGGER_MODE;
            break;
        }
        case E_XC_AUTODOWNLOAD_ENABLE_MODE:
        {
            *penMode = E_KDRV_XC_AUTODOWNLOAD_ENABLE_MODE;
            break;
        }
        default:
        {
            return FALSE;
            break;
        }
    }

    return TRUE;
}

static MS_BOOL _MDrv_XC_AutoDownload_Convert2KDrvSubClient(EN_XC_AUTODOWNLOAD_SUB_CLIENT enSubClient, EN_KDRV_XC_AUTODOWNLOAD_SUB_CLIENT *penSubClient)
{
    if (penSubClient == NULL)
    {
        return FALSE;
    }

    switch(enSubClient)
    {
        case E_XC_AUTODOWNLOAD_HDR_SUB_TMO:
        {
            *penSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_TMO;
            break;
        }
        case E_XC_AUTODOWNLOAD_HDR_SUB_3DLUT:
        {
            *penSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_3DLUT;
            break;
        }
        case E_XC_AUTODOWNLOAD_HDR_SUB_GAMMA:
        {
            *penSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_GAMMA;
            break;
        }
        case E_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA:
        {
            *penSubClient = E_KDRV_XC_AUTODOWNLOAD_HDR_SUB_DEGAMMA;
            break;
        }
        default:
        {
            return FALSE;
            break;
        }

    }

    return TRUE;
}

E_APIXC_ReturnValue MDrv_XC_AutoDownload_Config(void* pInstance, XC_AUTODOWNLOAD_CONFIG_INFO *pstConfigInfo)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;

#if (defined AUTO_DOWNLOAD_HDR_IN_KERNEL) || (defined AUTO_DOWNLOAD_XVYCC_IN_KERNEL)
    if ((pstConfigInfo->enClient == E_XC_AUTODOWNLOAD_CLIENT_HDR) || (pstConfigInfo->enClient == E_XC_AUTODOWNLOAD_CLIENT_XVYCC))
    {
        EN_KDRV_XC_AUTODOWNLOAD_CLIENT enKDrvClient;
        if (_MDrv_XC_AutoDownload_Convert2KDrvClient(pstConfigInfo->enClient, &enKDrvClient) != TRUE)
        {
            printf("_MDrv_XC_AutoDownload_Convert2KDrvClient fail.\n");
            return E_APIXC_RET_FAIL;
        }

        EN_KDRV_XC_AUTODOWNLOAD_MODE enKDrvMode;
        if (_MDrv_XC_AutoDownload_Convert2KDrvMode(pstConfigInfo->enMode, &enKDrvMode) != TRUE)
        {
            printf("_MDrv_XC_AutoDownload_Convert2KDrvMode fail.\n");
            return E_APIXC_RET_FAIL;
        }

        ST_KDRV_XC_AUTODOWNLOAD_CONFIG_INFO stAutoDownloadConfigInfo;
        memset(&stAutoDownloadConfigInfo, 0, sizeof(ST_KDRV_XC_AUTODOWNLOAD_CONFIG_INFO));
        stAutoDownloadConfigInfo.bEnable = pstConfigInfo->bEnable;
        stAutoDownloadConfigInfo.enClient = enKDrvClient;
        stAutoDownloadConfigInfo.enMode = enKDrvMode;
        stAutoDownloadConfigInfo.phyBaseAddr = pstConfigInfo->phyBaseAddr;
        stAutoDownloadConfigInfo.u32Size = pstConfigInfo->u32Size;
#ifdef HAL_MIU2_BASE
        if (pstConfigInfo->phyBaseAddr >=  HAL_MIU2_BASE)
        {
            stAutoDownloadConfigInfo.u32MiuNo = 2;
        }
        else
#endif
#ifdef HAL_MIU1_BASE
        if (pstConfigInfo->phyBaseAddr >= HAL_MIU1_BASE)
        {
            stAutoDownloadConfigInfo.u32MiuNo = 1;
        }
        else
#endif
        {
            stAutoDownloadConfigInfo.u32MiuNo = 0;
        }


#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
                return E_APIXC_RET_FAIL;
            }
        }

        if (ioctl(_s32FdScaler, MDRV_XC_IOC_CONFIG_AUTO_DOWNLOAD, &stAutoDownloadConfigInfo))
        {
            printf("MDRV_XC_IOC_CONFIG_AUTO_DOWNLOAD fail!!!!\n");
            return E_APIXC_RET_FAIL;
        }

        eRet = E_APIXC_RET_OK;
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
        if (MDrv_XC_ConfigAutoDownload(&stAutoDownloadConfigInfo) != TRUE)
        {
            return E_APIXC_RET_FAIL;
        }
#endif
    }
    else
#endif
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        MS_BOOL bEnable = pXCResourcePrivate->stdrvXC_Display._stClientInfo[pstConfigInfo->enClient].bEnable;

        if ((pstConfigInfo->bEnable == TRUE) && (pstConfigInfo->bEnable != bEnable))
        {
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[pstConfigInfo->enClient].phyBaseAddr = pstConfigInfo->phyBaseAddr;
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[pstConfigInfo->enClient].u32Size = pstConfigInfo->u32Size;
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[pstConfigInfo->enClient].bEnable = pstConfigInfo->bEnable;

            MS_U8* pu8BaseAddr = (MS_U8 *)MS_PA2KSEG1(pstConfigInfo->phyBaseAddr);
            memset(pu8BaseAddr, 0, pstConfigInfo->u32Size);

            eRet = MHal_XC_AutoDownload_Config(pInstance, pstConfigInfo->enClient, pstConfigInfo->phyBaseAddr, pstConfigInfo->enMode);
        }
        else if (pstConfigInfo->bEnable == FALSE)
        {
            pXCResourcePrivate->stdrvXC_Display._stClientInfo[pstConfigInfo->enClient].bEnable = pstConfigInfo->bEnable;
        }
    }

    return eRet;
}

E_APIXC_ReturnValue MDrv_XC_AutoDownload_Write(void* pInstance, XC_AUTODOWNLOAD_DATA_INFO *pstDataInfo)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;

#if (defined AUTO_DOWNLOAD_HDR_IN_KERNEL) || (defined AUTO_DOWNLOAD_XVYCC_IN_KERNEL)
    if ((pstDataInfo->enClient == E_XC_AUTODOWNLOAD_CLIENT_HDR) || (pstDataInfo->enClient == E_XC_AUTODOWNLOAD_CLIENT_XVYCC))
    {
        EN_KDRV_XC_AUTODOWNLOAD_CLIENT enKDrvClient;
        if (_MDrv_XC_AutoDownload_Convert2KDrvClient(pstDataInfo->enClient, &enKDrvClient) != TRUE)
        {
            printf("_MDrv_XC_AutoDownload_Convert2KDrvClient fail.\n");
            return E_APIXC_RET_FAIL;
        }

        XC_AUTODOWNLOAD_FORMAT_INFO *pstFormatInfo = (XC_AUTODOWNLOAD_FORMAT_INFO *)pstDataInfo->pParam;

        EN_KDRV_XC_AUTODOWNLOAD_SUB_CLIENT enKDrvSubClient;
        if (_MDrv_XC_AutoDownload_Convert2KDrvSubClient(pstFormatInfo->enSubClient, &enKDrvSubClient) != TRUE)
        {
            printf("_MDrv_XC_AutoDownload_Convert2KDrvSubClient fail.\n");
            return E_APIXC_RET_FAIL;
        }

        ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO stAutoDownloadForamtInfo;
        memset(&stAutoDownloadForamtInfo, 0, sizeof(ST_KDRV_XC_AUTODOWNLOAD_FORMAT_INFO));
        stAutoDownloadForamtInfo.bEnableRange = pstFormatInfo->bEnableRange;
        stAutoDownloadForamtInfo.enSubClient = enKDrvSubClient;
        stAutoDownloadForamtInfo.u16StartAddr = pstFormatInfo->u16StartAddr;
        stAutoDownloadForamtInfo.u16EndAddr = pstFormatInfo->u16EndAddr;

        ST_KDRV_XC_AUTODOWNLOAD_DATA_INFO stAutoDownloadDataInfo;
        memset(&stAutoDownloadDataInfo, 0, sizeof(ST_KDRV_XC_AUTODOWNLOAD_DATA_INFO));
        stAutoDownloadDataInfo.enClient = enKDrvClient;
        stAutoDownloadDataInfo.pu8Data = pstDataInfo->pu8Data;
        stAutoDownloadDataInfo.u32Size = pstDataInfo->u32Size;
        stAutoDownloadDataInfo.pParam = &stAutoDownloadForamtInfo;

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
                return E_APIXC_RET_FAIL;
            }
        }
        if (ioctl(_s32FdScaler, MDRV_XC_IOC_WRITE_AUTO_DOWNLOAD, &stAutoDownloadDataInfo))
        {
            printf("MDRV_XC_IOC_WRITE_AUTO_DOWNLOAD fail!!!!\n");
            return E_APIXC_RET_FAIL;
        }

        eRet = E_APIXC_RET_OK;
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
        if (MDrv_XC_WriteAutoDownload(&stAutoDownloadDataInfo) != TRUE)
        {
            return E_APIXC_RET_FAIL;
        }
#endif
    }
    else
#endif
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        MS_BOOL bEnable = pXCResourcePrivate->stdrvXC_Display._stClientInfo[pstDataInfo->enClient].bEnable;

        if (!bEnable)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_PARAMETER_TRACE, "auto download is not enabled(client: %d).\n", pstDataInfo->enClient);
            return E_APIXC_RET_FAIL;
        }

        eRet = MHal_XC_AutoDownload_Write(pInstance, pstDataInfo->enClient, pstDataInfo->pu8Data, pstDataInfo->u32Size, pstDataInfo->pParam);
    }

    return eRet;
}

E_APIXC_ReturnValue MDrv_XC_AutoDownload_Fire(void* pInstance, EN_XC_AUTODOWNLOAD_CLIENT enClient)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    XC_LOG_TRACE(XC_DBGLEVEL_PARAMETER_TRACE, "Fire auto download(client: %d).\n", enClient);

#if (defined AUTO_DOWNLOAD_HDR_IN_KERNEL) || (defined AUTO_DOWNLOAD_XVYCC_IN_KERNEL)
    if ((enClient == E_XC_AUTODOWNLOAD_CLIENT_HDR) || (enClient == E_XC_AUTODOWNLOAD_CLIENT_XVYCC))
    {
        EN_KDRV_XC_AUTODOWNLOAD_CLIENT enKDrvClient;
        if (_MDrv_XC_AutoDownload_Convert2KDrvClient(enClient, &enKDrvClient) != TRUE)
        {
            printf("_MDrv_XC_AutoDownload_Convert2KDrvClient fail.\n");
            return E_APIXC_RET_FAIL;
        }
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
                return E_APIXC_RET_FAIL;
            }
        }

        if (ioctl(_s32FdScaler, MDRV_XC_IOC_FIRE_AUTO_DOWNLOAD, &enKDrvClient))
        {
            printf("MDRV_XC_IOC_FIRE_AUTO_DOWNLOAD fail!!!!\n");
            return E_APIXC_RET_FAIL;
        }

        eRet = E_APIXC_RET_OK;
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
        if (MDrv_XC_FireAutoDownload(enKDrvClient) != TRUE)
        {
            return E_APIXC_RET_FAIL;
        }
#endif
    }
    else
#endif
    {
        XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
        UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
        XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
        UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
        MS_BOOL bEnable = pXCResourcePrivate->stdrvXC_Display._stClientInfo[enClient].bEnable;

        if (!bEnable)
        {
            XC_LOG_TRACE(XC_DBGLEVEL_PARAMETER_TRACE, "auto download is not enabled(client: %d).\n", enClient);
            return E_APIXC_RET_FAIL;
        }

        eRet = MHal_XC_AutoDownload_Fire(pInstance, enClient);
    }

    return eRet;
}

E_APIXC_ReturnValue MDrv_XC_GetAutoDownloadCaps(EN_XC_AUTODOWNLOAD_CLIENT enClient, MS_BOOL *pbSupported)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
#if (defined AUTO_DOWNLOAD_HDR_IN_KERNEL) || (defined AUTO_DOWNLOAD_XVYCC_IN_KERNEL)
    if ((enClient == E_XC_AUTODOWNLOAD_CLIENT_HDR) || (enClient == E_XC_AUTODOWNLOAD_CLIENT_XVYCC))
    {
        EN_KDRV_XC_AUTODOWNLOAD_CLIENT enKDrvClient;
        if (_MDrv_XC_AutoDownload_Convert2KDrvClient(enClient, &enKDrvClient) != TRUE)
        {
            printf("_MDrv_XC_AutoDownload_Convert2KDrvClient fail.\n");
            return E_APIXC_RET_FAIL;
        }

        ST_KDRV_XC_AUTODOWNLOAD_CLIENT_SUPPORTED_CAPS stAutoDownloadCaps;
        memset(&stAutoDownloadCaps, 0, sizeof(ST_KDRV_XC_AUTODOWNLOAD_CLIENT_SUPPORTED_CAPS));
        stAutoDownloadCaps.enClient = enKDrvClient;
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
        if(0 > _s32FdScaler)
        {
            _s32FdScaler = open("/dev/scaler", O_RDWR);

            if(0 > _s32FdScaler)
            {
                printf("Scaler device not opened!!!!\n");
                return E_APIXC_RET_FAIL;
            }
        }
        if (ioctl(_s32FdScaler, MDRV_XC_IOC_GET_AUTO_DOWNLOAD_CAPS, &stAutoDownloadCaps))
        {
            printf("MDRV_XC_IOC_GET_AUTO_DOWNLOAD_CAPS fail!!!!\n");
            return E_APIXC_RET_FAIL;
        }

        *pbSupported = stAutoDownloadCaps.bSupported;
        eRet = E_APIXC_RET_OK;
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
        if (KDrv_XC_GetAutoDownloadCaps(&stAutoDownloadCaps) != TRUE)
        {
            return E_APIXC_RET_FAIL;
        }

        *pbSupported = stAutoDownloadCaps.bSupported;
        eRet = E_APIXC_RET_OK;
#endif
    }
    else
#endif
    {
        eRet = MHal_XC_GetAutoDownloadCaps(enClient, pbSupported);
    }
    XC_LOG_TRACE(XC_DBGLEVEL_PARAMETER_TRACE, "Get auto download caps(client: %d, bSupport: %d).\n", enClient, *pbSupported);

    return eRet;
}
#endif

#ifdef UFO_XC_HDR
#if (UFO_XC_HDR_VERSION == 2)
static MS_BOOL _MDrv_XC_AutoDownload_Convert2KDrvColorFormat(EN_XC_HDR_COLOR_FORMAT enColorFormat, EN_KDRV_XC_HDR_COLOR_FORMAT *penColorFormat)
{
    if (penColorFormat == NULL)
    {
        return FALSE;
    }

    switch (enColorFormat)
    {
        case E_XC_HDR_COLOR_NONE:
        {
            *penColorFormat = E_KDRV_XC_HDR_COLOR_NONE;
            break;
        }
        case E_XC_HDR_COLOR_YUV420:
        {
            *penColorFormat = E_KDRV_XC_HDR_COLOR_YUV420;
            break;
        }
        case E_XC_HDR_COLOR_YUV422:
        {
            *penColorFormat = E_KDRV_XC_HDR_COLOR_YUV422;
            break;
        }
        case E_KDRV_XC_HDR_COLOR_YUV444:
        {
            *penColorFormat = E_KDRV_XC_HDR_COLOR_YUV444;
            break;
        }
        case E_KDRV_XC_HDR_COLOR_RGB:
        {
            *penColorFormat = E_KDRV_XC_HDR_COLOR_RGB;
            break;
        }
        case E_XC_HDR_COLOR_MAX:
        {
            *penColorFormat = E_KDRV_XC_HDR_COLOR_MAX;
            break;
        }
        default:
        {
            return FALSE;
            break;
        }
    }

    return TRUE;
}

E_APIXC_ReturnValue MDrv_XC_HDR_Control(void* pInstance, EN_XC_HDR_CTRL_TYPE enCtrlType, void *pParam)
{
    if ((enCtrlType <= E_XC_HDR_CTRL_NONE) || (enCtrlType >= E_XC_HDR_CTRL_MAX))
    {
        XC_LOG_TRACE(XC_DBGLEVEL_PARAMETER_TRACE, "HDR ctrl fail.\n");
        return E_APIXC_RET_FAIL;
    }

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return E_APIXC_RET_FAIL;
        }
    }
#endif

    switch(enCtrlType)
    {
        case E_XC_HDR_CTRL_ENABLE:
        {
            MS_BOOL bEnable = *(MS_BOOL *)pParam;

            pXCResourcePrivate->stdrvXC_Display._bEnableHDR = bEnable;

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
            if (ioctl(_s32FdScaler, MDRV_XC_IOC_ENABLE_HDR, &bEnable))
            {
                printf("MDRV_XC_IOC_ENABLE_HDR fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_EnableHDR(bEnable) != TRUE)
            {
                return E_APIXC_RET_FAIL;
            }
#endif

            break;
        }
        case E_XC_HDR_CTRL_SET_TYPE:
        {
            EN_XC_HDR_TYPE enHDRType = *(EN_XC_HDR_TYPE *)pParam;

            pXCResourcePrivate->stdrvXC_Display._enHDRType = enHDRType;
            EN_KDRV_XC_HDR_TYPE enXCHDRType = E_KDRV_XC_HDR_NONE;

            if (enHDRType == E_XC_HDR_TYPE_OPEN)
            {
                enXCHDRType = E_KDRV_XC_HDR_OPEN;
            }
            else if (enHDRType == E_XC_HDR_TYPE_DOLBY)
            {
                enXCHDRType = E_KDRV_XC_HDR_DOLBY;
            }
            else if (enHDRType == E_XC_HDR_TYPE_TECHNICOLOR)
            {
                enXCHDRType = E_KDRV_XC_HDR_TECHNICOLOR;
            }

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
            if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_HDR_TYPE, &enXCHDRType))
            {
                printf("MDRV_XC_IOC_SET_HDR_TYPE fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_SetHDRType(enHDRType) != TRUE)
            {
                return E_APIXC_RET_FAIL;
            }
#endif
            break;
        }
        case E_XC_HDR_CTRL_GET_TYPE:
        {
            EN_XC_HDR_TYPE *penHDRType = (EN_XC_HDR_TYPE *)pParam;
            EN_KDRV_XC_HDR_TYPE enXCHDRType = E_KDRV_XC_HDR_NONE;
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
            if (ioctl(_s32FdScaler, MDRV_XC_IOC_GET_HDR_TYPE, &enXCHDRType))
            {
                printf("MDRV_XC_IOC_GET_HDR_TYPE fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_GetHDRType(&enXCHDRType) != TRUE)
            {
                return E_APIXC_RET_FAIL;
            }
#endif
            switch (enXCHDRType)
            {
                default:
                case E_KDRV_XC_HDR_MAX:
                case E_KDRV_XC_HDR_NONE:
                    *penHDRType = E_XC_HDR_TYPE_NONE;
                break;
                case E_KDRV_XC_HDR_OPEN:
                    *penHDRType = E_XC_HDR_TYPE_OPEN;
                break;
                case E_KDRV_XC_HDR_DOLBY:
                    *penHDRType = E_XC_HDR_TYPE_DOLBY;
                break;
            }
            break;
        }
        case E_XC_HDR_CTRL_SET_3DLUT:
        {
            XC_HDR_3DLUT_INFO st3DLutInfo = *(XC_HDR_3DLUT_INFO *)pParam;
            ST_KDRV_XC_3DLUT_INFO stXC3DLutInfo;
            memset(&stXC3DLutInfo, 0, sizeof(ST_KDRV_XC_3DLUT_INFO));
            stXC3DLutInfo.pu8Data = st3DLutInfo.pu8Data;
            stXC3DLutInfo.u32Size = st3DLutInfo.u32Size;

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)

            if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_3DLUT_INFO, &stXC3DLutInfo))
            {
                printf("MDRV_XC_IOC_SET_3DLUT_INFO fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_Set3DLutInfo(&stXC3DLutInfo) < 0)
            {
                printf("MDRV_XC_IOC_SET_3DLUT_INFO fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif
            break;
        }
        case E_XC_HDR_CTRL_CONFIG_DMA:
        {
            XC_HDR_DMA_CONFIG_INFO stDMAConfigInfo = *(XC_HDR_DMA_CONFIG_INFO *)pParam;

            pXCResourcePrivate->stdrvXC_Display._stDMAConfigInfo = stDMAConfigInfo;
            break;
        }
        case E_XC_HDR_CTRL_INIT_DMA:
        {
            XC_HDR_DMA_INIT_INFO stDMAInitInfo = *(XC_HDR_DMA_INIT_INFO *)pParam;

            pXCResourcePrivate->stdrvXC_Display._stDMAInitInfo = stDMAInitInfo;
            break;
        }
#ifdef UFO_XC_HDMI_4K2K_DMA
        case E_XC_HDR_CTRL_INIT_DMA_HDMI:
        {
            XC_HDR_DMA_INIT_INFO stDMAInitInfo = *(XC_HDR_DMA_INIT_INFO *)pParam;

            pXCResourcePrivate->stdrvXC_Display._stDMAInitInfo_Hdmi = stDMAInitInfo;
            break;
        }
#endif
        case E_XC_HDR_CTRL_SET_OTT_SHARE_MEMORY:
        {
            XC_HDR_OTT_SHARE_MEMORY_INFO stShareMemInfo = *(XC_HDR_OTT_SHARE_MEMORY_INFO *)pParam;
            ST_KDRV_XC_SHARE_MEMORY_INFO stXCShareMemInfo;
            memset(&stXCShareMemInfo, 0, sizeof(ST_KDRV_XC_SHARE_MEMORY_INFO));
            stXCShareMemInfo.phyBaseAddr = stShareMemInfo.phyBaseAddr;
            stXCShareMemInfo.u32Size = stShareMemInfo.u32Size;

#ifdef HAL_MIU2_BASE
            if (stShareMemInfo.phyBaseAddr >=  HAL_MIU2_BASE)
            {
                stXCShareMemInfo.u32MiuNo = 2;
            }
            else
#endif
#ifdef HAL_MIU1_BASE
            if (stShareMemInfo.phyBaseAddr >= HAL_MIU1_BASE)
            {
                stXCShareMemInfo.u32MiuNo = 1;
            }
            else
#endif
            {
                stXCShareMemInfo.u32MiuNo = 0;
            }

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
            if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_SHMEM_INFO, &stXCShareMemInfo))
            {
                printf("MDRV_XC_IOC_SET_SHMEM_INFO fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_SetShareMemInfo(&stXCShareMemInfo) != TRUE)
            {
                return E_APIXC_RET_FAIL;
            }
#endif
            break;
        }
        case E_XC_HDR_CTRL_SET_OPEN_METADATA:
        {
            //XC_HDR_OPEN_METADATA_INFO stOpenInfo = *(XC_HDR_OPEN_METADATA_INFO *)pParam;
            // to do
            break;
        }
        case E_XC_HDR_CTRL_SET_COLOR_FORMAT:
        {
            EN_XC_HDR_COLOR_FORMAT enColorFormat = *(EN_XC_HDR_COLOR_FORMAT *)pParam;

            pXCResourcePrivate->stdrvXC_Display._enColorFormat = enColorFormat;
            EN_KDRV_XC_HDR_COLOR_FORMAT enXCKDrvColorFormat;
            if (_MDrv_XC_AutoDownload_Convert2KDrvColorFormat(enColorFormat, &enXCKDrvColorFormat) != TRUE)
            {
                printf("_MDrv_XC_AutoDownload_Convert2KDrvColorFormat fail.\n");
                return E_APIXC_RET_FAIL;
            }
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
            if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_COLOR_FORMAT, &enXCKDrvColorFormat))
            {
                printf("MDRV_XC_IOC_SET_COLOR_FORMAT fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_SetColorFormat(enXCKDrvColorFormat) != TRUE)
            {
                return E_APIXC_RET_FAIL;
            }
#endif
            break;
        }
        case E_XC_HDR_CTRL_CFD_CONTROL:
        {
            XC_CFD_CONTROL_INFO stCfdControlInfo = *(XC_CFD_CONTROL_INFO *)pParam;
            ST_KDRV_XC_CFD_CONTROL_INFO stXCCfdControlInfo;
            memset(&stXCCfdControlInfo, 0, sizeof(ST_KDRV_XC_CFD_CONTROL_INFO));
            stXCCfdControlInfo.enCtrlType = stCfdControlInfo.enCtrlType;
            stXCCfdControlInfo.pParam = stCfdControlInfo.pParam;
            stXCCfdControlInfo.u32ParamLen = stCfdControlInfo.u32ParamLen;
            stXCCfdControlInfo.u16ErrCode = stCfdControlInfo.u16ErrCode;
#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
            if (ioctl(_s32FdScaler, MDRV_XC_IOC_CFDCONCTRL, pParam))
            {
                printf("MDRV_XC_IOC_CFDCONCTRL fail!!!!\n");
                return E_APIXC_RET_FAIL;
            }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
            if (MDrv_XC_CFDControl(&stXCCfdControlInfo) != TRUE)
            {
                return E_APIXC_RET_FAIL;
            }
#endif
            break;
        }
        default:
        {
            break;
        }
    }

    return MHal_XC_HDR_Control(pInstance, enCtrlType, pParam);
}

E_APIXC_ReturnValue MDrv_XC_HDR_SetInputSource(void* pInstance, INPUT_SOURCE_TYPE_t enInputSourceType)
{
    EN_XC_INPUT_SOURCE_TYPE enInputSource = E_XC_INPUT_SOURCE_TYPE_NONE;
    EN_KDRV_XC_INPUT_SOURCE_TYPE enXCInputSource = E_KDRV_XC_INPUT_SOURCE_NONE;

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);

    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));

    if(IsSrcTypeHDMI(enInputSourceType))
    {
        enInputSource = E_XC_INPUT_SOURCE_TYPE_HDMI;
        enXCInputSource = E_KDRV_XC_INPUT_SOURCE_HDMI;
    }
    else if (IsSrcTypeStorage(enInputSourceType))
    {
        enInputSource = E_XC_INPUT_SOURCE_TYPE_OTT;
        enXCInputSource = E_KDRV_XC_INPUT_SOURCE_OTT;
    }
    pXCResourcePrivate->stdrvXC_Display._enInputSourceType = enInputSource;

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return E_APIXC_RET_FAIL;
        }
    }

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_INPUT_SOURCE_TYPE, &enXCInputSource))
    {
        printf("MDRV_XC_IOC_SET_INPUT_SOURCE_TYPE fail!!!!\n");
        return E_APIXC_RET_FAIL;
    }
#endif

#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
    if (MDrv_XC_SetInputSourceType(enInputSourceType) != TRUE)
    {
        return E_APIXC_RET_FAIL;
    }
#endif

    return E_APIXC_RET_OK;
}

E_APIXC_ReturnValue MDrv_XC_HDR_GetCaps(void *pInstance, XC_HDR_SUPPORTED_CAPS *pstHDRCaps)
{
    E_APIXC_ReturnValue eRet = E_APIXC_RET_FAIL;
    EN_XC_HDR_TYPE enHDRType = pstHDRCaps->enHDRType;

    if ((enHDRType <= E_XC_HDR_TYPE_NONE) || (enHDRType >= E_XC_HDR_TYPE_MAX))
    {
        return E_APIXC_RET_FAIL_INVALID_PARAMETER;
    }

    eRet = MHal_XC_HDR_GetCaps(pInstance, pstHDRCaps);
    XC_LOG_TRACE(XC_DBGLEVEL_PARAMETER_TRACE, "Get HDR caps(type: %d, bSupport: %d).\n", enHDRType, pstHDRCaps->bSupported);

    return eRet;
}

E_APIXC_ReturnValue MDrv_XC_HDR_SetWindow(void* pInstance, MS_WINDOW_TYPE stCapWin)
{
    ST_KDRV_XC_WINDOW_INFO stXCWindowInfo;

    stXCWindowInfo.u16X = stCapWin.x;
    stXCWindowInfo.u16Y = stCapWin.y;
    stXCWindowInfo.u16Width = stCapWin.width;
    stXCWindowInfo.u16Height = stCapWin.height;

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return E_APIXC_RET_FAIL;
        }
    }

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_HDR_WINDOW, &stXCWindowInfo))
    {
        printf("MDRV_XC_IOC_SET_INPUT_SOURCE_TYPE fail!!!!\n");
        return E_APIXC_RET_FAIL;
    }
#endif
#if (defined(MSOS_TYPE_LINUX_KERNEL) && defined(CONFIG_MSTAR_XC_HDR_SUPPORT))
    if (MDrv_XC_SetHDRWindow(&stXCWindowInfo) != TRUE)
    {
        return E_APIXC_RET_FAIL;
    }
#endif
    return E_APIXC_RET_OK;
}
#endif
#endif

MS_BOOL MDrv_XC_SetSWDRInfo(void* pInstance, XC_SWDR_INFO *pSWDR_INFO)
{
#if defined(UFO_XC_SWDR)
    ST_KDRV_XC_SWDR_INFO stKdrvXcSwdrInfo;
    memset(&stKdrvXcSwdrInfo, 0, sizeof(ST_KDRV_XC_SWDR_INFO));

    stKdrvXcSwdrInfo.bDRE_En        = pSWDR_INFO->bDRE_En;
    stKdrvXcSwdrInfo.bDRE_SWDR_En   = pSWDR_INFO->bDRE_SWDR_En;
    stKdrvXcSwdrInfo.u8DRE_DR_Set_BasicStrength     = pSWDR_INFO->u8DRE_DR_Set_BasicStrength;
    stKdrvXcSwdrInfo.u8DRE_SWDR_Set_BasicStrength   = pSWDR_INFO->u8DRE_SWDR_Set_BasicStrength;
    stKdrvXcSwdrInfo.u16DRE_SWDR_Set_StrengthDk     = pSWDR_INFO->u16DRE_SWDR_Set_StrengthDk;
    stKdrvXcSwdrInfo.u16DRE_SWDR_Set_StrengthBr     = pSWDR_INFO->u16DRE_SWDR_Set_StrengthBr;
    stKdrvXcSwdrInfo.u8DRE_SWDR_Set_SceneProtectLevel = pSWDR_INFO->u8DRE_SWDR_Set_SceneProtectLevel;
    stKdrvXcSwdrInfo.u8DRE_Set_TemporalFilterLevel  = pSWDR_INFO->u8DRE_Set_TemporalFilterLevel;
    stKdrvXcSwdrInfo.u8DRE_Set_ColorCorrectLevel    = pSWDR_INFO->u8DRE_Set_ColorCorrectLevel;

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return E_APIXC_RET_FAIL;
        }
    }

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_SET_SWDR_INFO, &stKdrvXcSwdrInfo))
    {
        printf("MDRV_XC_IOC_SET_SWDR_INFO fail!!!!\n");
        return E_APIXC_RET_FAIL;
    }
#endif
#if defined(MSOS_TYPE_LINUX_KERNEL)
    if (KDrv_XC_SetSWDRInfo(&stKdrvXcSwdrInfo) != TRUE)
    {
        return E_APIXC_RET_FAIL;
    }
#endif
#endif
    return TRUE;

}
MS_BOOL MDrv_XC_GetSWDRInfo(void* pInstance, XC_SWDR_INFO *pSWDR_INFO)
{
#if defined(UFO_XC_SWDR)
    ST_KDRV_XC_SWDR_INFO stKdrvXcSwdrInfo;
    memset(&stKdrvXcSwdrInfo, 0, sizeof(ST_KDRV_XC_SWDR_INFO));

#if (defined MSOS_TYPE_LINUX) || (defined ANDROID)
    if(0 > _s32FdScaler)
    {
        _s32FdScaler = open("/dev/scaler", O_RDWR);

        if(0 > _s32FdScaler)
        {
            printf("Scaler device not opened!!!!\n");
            return E_APIXC_RET_FAIL;
        }
    }

    if (ioctl(_s32FdScaler, MDRV_XC_IOC_GET_SWDR_INFO, &stKdrvXcSwdrInfo))
    {
        printf("MDRV_XC_IOC_GET_SWDR_INFO fail!!!!\n");
        return E_APIXC_RET_FAIL;
    }
#endif
#if defined(MSOS_TYPE_LINUX_KERNEL)
    if (KDrv_XC_GetSWDRInfo(&stKdrvXcSwdrInfo) != TRUE)
    {
        return E_APIXC_RET_FAIL;
    }
#endif

    pSWDR_INFO->bDRE_En         = stKdrvXcSwdrInfo.bDRE_En;
    pSWDR_INFO->bDRE_SWDR_En    = stKdrvXcSwdrInfo.bDRE_SWDR_En;
    pSWDR_INFO->u8DRE_DR_Set_BasicStrength      = stKdrvXcSwdrInfo.u8DRE_DR_Set_BasicStrength;
    pSWDR_INFO->u8DRE_SWDR_Set_BasicStrength    = stKdrvXcSwdrInfo.u8DRE_SWDR_Set_BasicStrength;
    pSWDR_INFO->u16DRE_SWDR_Set_StrengthDk      = stKdrvXcSwdrInfo.u16DRE_SWDR_Set_StrengthDk;
    pSWDR_INFO->u16DRE_SWDR_Set_StrengthBr      = stKdrvXcSwdrInfo.u16DRE_SWDR_Set_StrengthBr;
    pSWDR_INFO->u8DRE_SWDR_Set_SceneProtectLevel = stKdrvXcSwdrInfo.u8DRE_SWDR_Set_SceneProtectLevel;
    pSWDR_INFO->u8DRE_Set_TemporalFilterLevel   = stKdrvXcSwdrInfo.u8DRE_Set_TemporalFilterLevel;
    pSWDR_INFO->u8DRE_Set_ColorCorrectLevel     = stKdrvXcSwdrInfo.u8DRE_Set_ColorCorrectLevel;
#endif

    return TRUE;

}

MS_U32 MDrv_SC_GetHDMIPolicy(void *pInstance)
{
    MS_U32 u32HDMIPolicyInfo = 0;
    u32HDMIPolicyInfo = Hal_SC_get_HDMIpolicy(pInstance);
    return u32HDMIPolicyInfo;
}
