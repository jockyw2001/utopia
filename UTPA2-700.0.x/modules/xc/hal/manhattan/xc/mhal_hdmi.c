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
// (!MStar Confidential Information!L) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////
//==============================================================================
// [mhal_hdmi.c]
// Date: 20081203
// Descriptions: Add a new layer for HW setting
//==============================================================================
#define  MHAL_HDMI_C

// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "utopia_dapi.h"
#include "xc_Analog_Reg.h"

#include "drvXC_IOPort.h"
#include "xc_hwreg_utility2.h"
#include "hwreg_dvi_atop.h"//alex_tung
#include "hwreg_pm_sleep.h"//alex_tung
#include "apiXC.h"
#include "apiXC_Adc.h"
#include "apiXC_Auto.h"
#include "apiXC_ModeParse.h"
#include "apiXC_PCMonitor.h"
#include "drvXC_HDMI_if.h"
#include "drv_sc_ip.h"
#include "drv_sc_display.h"
#include "drv_sc_isr.h"
#include "mvideo_context.h"
#include "drvXC_HDMI_Internal.h"
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
#include "mhal_hdmi.h"
#include "apiHDMIRx_private.h"

#include "hwreg_mhl.h"

#define PM_REG_READ(reg)            PM_R1BYTE(reg, 7:0)
#define PM_REG_WRITE(reg, data)     PM_W1BYTE(reg, data, 7:0)
#define MST_XTAL_CLOCK_HZ           12000000UL

#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#define MHAL_HDMI_HDR_DBG(x) //x

#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MHAL_HDMIRX_MSG_INFO(format, args...)       ULOGI("HDMIRX", format, ##args)
#define MHAL_HDMIRX_MSG_WARNING(format, args...)    ULOGW("HDMIRX", format, ##args)
#define MHAL_HDMIRX_MSG_DEBUG(format, args...)      ULOGD("HDMIRX", format, ##args)
#define MHAL_HDMIRX_MSG_ERROR(format, args...)      ULOGE("HDMIRX", format, ##args)
#define MHAL_HDMIRX_MSG_FATAL(format, args...)      ULOGF("HDMIRX", format, ##args)

#else
#define MHAL_HDMIRX_MSG_INFO(format, args...)       printf(format, ##args)
#define MHAL_HDMIRX_MSG_WARNING(format, args...)    printf(format, ##args)
#define MHAL_HDMIRX_MSG_DEBUG(format, args...)      printf(format, ##args)
#define MHAL_HDMIRX_MSG_ERROR(format, args...)      printf(format, ##args)
#define MHAL_HDMIRX_MSG_FATAL(format, args...)      printf(format, ##args)

#endif

//==============================================================================
// For MCU access HDCP key SRAM issue.
// Need to read timer once to cover the period of HDCP busy bit's transition.
#define TIMER_0_CNT_REG             0x3028//BIT0-BIT31, RO
//==============================================================================
// HDMI Black Level
#define HDMI_BLACK_LEVEL_HIGH_YC_OFFSET     0x00F0
#define HDMI_BLACK_LEVEL_HIGH_YC_GAIN       0x403B

#define HDMI_BLACK_LEVEL_LOW_YC_OFFSET      0x00E0
#define HDMI_BLACK_LEVEL_LOW_YC_GAIN        0x4045

//==============================================================================
/******************************************************************************/
/// for Patch Flag
/******************************************************************************/
MS_BOOL bReceiveAVIPortSelect0 = FALSE;
MS_BOOL bReceiveAVIPortSelect1 = FALSE;

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_CheckInputIsMHL()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_CheckInputIsMHL(E_MUX_INPUTPORT enInputPortType, MS_U8 ucMHLSupportPath)
{
    MS_BOOL bMHLAttach = FALSE;
    MS_BOOL bCableDetect = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);

    if(bCableDetect)
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0:
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI0)
                {
                    bMHLAttach = TRUE;
                }

                break;

            case INPUT_PORT_DVI2:
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI2)
                {
                    bMHLAttach = TRUE;
                }

                break;

            default:

                break;
        };
    }

    return bMHLAttach;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_CheckPortSelectMux0()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_CheckPortSelectMux0(MS_U8 ucPortSelect)
{
    MS_BOOL bOnLinePort = FALSE;

    if((R2BYTE(REG_HDMI2_DUAL_0_50_L) &BMASK(1:0)) == ucPortSelect)
    {
        bOnLinePort = TRUE;
    }

    return bOnLinePort;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_CheckYUV420PortSelect0()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_CheckYUV420PortSelect0(void)
{
    MS_BOOL bYUV420Flag = FALSE;

    if(R2BYTEMSK(REG_HDMI_DUAL_0_40_L, BMASK(6:5)) == 0x60)
    {
        bYUV420Flag = TRUE;
    }

    return bYUV420Flag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetPacketReceiveStatus0()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 _Hal_tmds_GetPacketReceiveStatus0(void)
{
    MS_U16 usPacketInfoPortSelect0 = R2BYTE(REG_HDMI_DUAL_0_01_L);

    if(usPacketInfoPortSelect0 &BIT(3))
    {
        bReceiveAVIPortSelect0 = TRUE;
    }

    return usPacketInfoPortSelect0;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetClockValidFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetClockValidFlag(E_MUX_INPUTPORT enInputPortType, MS_BOOL bCheckClockStable)
{
    MS_BOOL bStatusFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if(R2BYTE(REG_PM_SLEEP_04_L) &BIT(1)) // Get clock status
            {
                bStatusFlag = TRUE;

                W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(1), BIT(1)); // Clear clock status
                W2BYTEMSK(REG_PM_SLEEP_03_L, bCheckClockStable? 0: BIT(1), BIT(1));
            }

            break;

        case INPUT_PORT_DVI1:
            if(R2BYTE(REG_PM_SLEEP_04_L) &BIT(2)) // Get clock status
            {
                bStatusFlag = TRUE;

                W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(2), BIT(2)); // Clear clock status
                W2BYTEMSK(REG_PM_SLEEP_03_L, bCheckClockStable? 0: BIT(2), BIT(2));
            }

            break;

        case INPUT_PORT_DVI2:
            if(R2BYTE(REG_PM_SLEEP_04_L) &BIT(3)) // Get clock status
            {
                bStatusFlag = TRUE;

                W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(3), BIT(3)); // Clear clock status
                W2BYTEMSK(REG_PM_SLEEP_03_L, bCheckClockStable? 0: BIT(3), BIT(3));
            }

            break;

        case INPUT_PORT_DVI3:
            if(R2BYTE(REG_PM_SLEEP_04_L) &BIT(4)) // Get clock status
            {
                bStatusFlag = TRUE;

                W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(4), BIT(4)); // Clear clock status
                W2BYTEMSK(REG_PM_SLEEP_03_L, bCheckClockStable? 0: BIT(4), BIT(4));
            }

            break;

        default:

            break;
    };

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetClockStableFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetClockStableFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bStatusFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            bStatusFlag = ((R2BYTE(REG_PM_SLEEP_3F_L) &BIT(14)) ?FALSE: TRUE);
            break;

        case INPUT_PORT_DVI1:
            bStatusFlag = ((R2BYTE(REG_PM_SLEEP_42_L) &BIT(14)) ?FALSE: TRUE);
            break;

        case INPUT_PORT_DVI2:
            bStatusFlag = ((R2BYTE(REG_PM_SLEEP_45_L) &BIT(14)) ?FALSE: TRUE);
            break;

        case INPUT_PORT_DVI3:
            bStatusFlag = ((R2BYTE(REG_PM_SLEEP_48_L) &BIT(14)) ?FALSE: TRUE);
            break;

        default:

            break;
    };

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetMHL3ClockStableFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetMHL3ClockStableFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bStatusFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P0_60_L) &BIT(8))) // MHL3 enable
            {
                bStatusFlag = ((R2BYTE(REG_COMBO_PHY0_P0_41_L) &BIT(9)) ?TRUE: FALSE);
            }
            break;

        case INPUT_PORT_DVI1:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P1_60_L) &BIT(8))) // MHL3 enable
            {
                bStatusFlag = ((R2BYTE(REG_COMBO_PHY0_P1_41_L) &BIT(9)) ?TRUE: FALSE);
            }
            break;

        case INPUT_PORT_DVI2:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P2_60_L) &BIT(8))) // MHL3 enable
            {
                bStatusFlag = ((R2BYTE(REG_COMBO_PHY0_P2_41_L) &BIT(9)) ?TRUE: FALSE);
            }
            break;

        case INPUT_PORT_DVI3:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P3_60_L) &BIT(8))) // MHL3 enable
            {
                bStatusFlag = ((R2BYTE(REG_COMBO_PHY0_P3_41_L) &BIT(9)) ?TRUE: FALSE);
            }
            break;

        default:

            break;
    };

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ResetHDMI20VersionFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_ResetHDMI20VersionFlag(E_MUX_INPUTPORT enInputPortType)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_PM_SCDC0_0C_L, BMASK(3:2), BMASK(3:2));
            W2BYTEMSK(REG_PM_SCDC0_0C_L, 0, BMASK(3:2));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_PM_SCDC1_0C_L, BMASK(3:2), BMASK(3:2));
            W2BYTEMSK(REG_PM_SCDC1_0C_L, 0, BMASK(3:2));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_PM_SCDC2_0C_L, BMASK(3:2), BMASK(3:2));
            W2BYTEMSK(REG_PM_SCDC2_0C_L, 0, BMASK(3:2));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_PM_SCDC3_0C_L, BMASK(3:2), BMASK(3:2));
            W2BYTEMSK(REG_PM_SCDC3_0C_L, 0, BMASK(3:2));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ClearSCDCStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_ClearSCDCStatusFlag(E_MUX_INPUTPORT enInputPortType, MS_U8 ucStatusMask)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_PM_SCDC0_07_L, (ucStatusMask << 8), (ucStatusMask << 8));
            W2BYTEMSK(REG_PM_SCDC0_07_L, 0, (ucStatusMask << 8));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_PM_SCDC1_07_L, (ucStatusMask << 8), (ucStatusMask << 8));
            W2BYTEMSK(REG_PM_SCDC1_07_L, 0, (ucStatusMask << 8));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_PM_SCDC2_07_L, (ucStatusMask << 8), (ucStatusMask << 8));
            W2BYTEMSK(REG_PM_SCDC2_07_L, 0, (ucStatusMask << 8));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_PM_SCDC3_07_L, (ucStatusMask << 8), (ucStatusMask << 8));
            W2BYTEMSK(REG_PM_SCDC3_07_L, 0, (ucStatusMask << 8));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetSCDCStatusFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetSCDCStatusFlag(E_MUX_INPUTPORT enInputPortType, MS_U8 ucStatusMask)
{
    MS_BOOL bStatusFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            bStatusFlag = ((R2BYTE(REG_PM_SCDC0_07_L) &ucStatusMask) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI1:
            bStatusFlag = ((R2BYTE(REG_PM_SCDC1_07_L) &ucStatusMask) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI2:
            bStatusFlag = ((R2BYTE(REG_PM_SCDC2_07_L) &ucStatusMask) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI3:
            bStatusFlag = ((R2BYTE(REG_PM_SCDC3_07_L) &ucStatusMask) ?TRUE: FALSE);
            break;

        default:

            break;
    };

    if(bStatusFlag)
    {
        _Hal_tmds_ClearSCDCStatusFlag(enInputPortType, ucStatusMask);
    }

    return bStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_PowerDownControl()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_PowerDownControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bPowerOn)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, bPowerOn? 0: (BIT(13)| BMASK(3:0)), BIT(13)| BMASK(3:0));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_5B_L, bPowerOn? 0: (BIT(13)| BMASK(3:0)), BIT(13)| BMASK(3:0));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_5B_L, bPowerOn? 0: (BIT(13)| BMASK(3:0)), BIT(13)| BMASK(3:0));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_5B_L, bPowerOn? 0: (BIT(13)| BMASK(3:0)), BIT(13)| BMASK(3:0));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ResetClockDetect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_ResetClockDetect(E_MUX_INPUTPORT enInputPortType)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_32_L, 0, BIT(12)| BIT(8)| BIT(4)| BIT(0));
            W2BYTEMSK(REG_COMBO_PHY0_P0_32_L, BIT(12)| BIT(8)| BIT(4)| BIT(0), BIT(12)| BIT(8)| BIT(4)| BIT(0));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_32_L, 0, BIT(12)| BIT(8)| BIT(4)| BIT(0));
            W2BYTEMSK(REG_COMBO_PHY0_P1_32_L, BIT(12)| BIT(8)| BIT(4)| BIT(0), BIT(12)| BIT(8)| BIT(4)| BIT(0));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_32_L, 0, BIT(12)| BIT(8)| BIT(4)| BIT(0));
            W2BYTEMSK(REG_COMBO_PHY0_P2_32_L, BIT(12)| BIT(8)| BIT(4)| BIT(0), BIT(12)| BIT(8)| BIT(4)| BIT(0));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_32_L, 0, BIT(12)| BIT(8)| BIT(4)| BIT(0));
            W2BYTEMSK(REG_COMBO_PHY0_P3_32_L, BIT(12)| BIT(8)| BIT(4)| BIT(0), BIT(12)| BIT(8)| BIT(4)| BIT(0));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ClockStatusInitial()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_ClockStatusInitial(E_MUX_INPUTPORT enInputPortType, MS_U8 ucMHLSupportPath)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_PM_SLEEP_3A_L, BMASK(7:4)| BIT(1), BMASK(7:0));
            W2BYTEMSK(REG_PM_SLEEP_3D_L, TMDS_CLOCK_CMP_VALUE0, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_3E_L, TMDS_CLOCK_CMP_VALUE1, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_00_L, 0, BIT(1)); // Clock status mask
            W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(1), BIT(1)); // Clear clock status
            W2BYTEMSK(REG_PM_SLEEP_03_L, 0, BIT(1));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_PM_SLEEP_3A_L, BMASK(15:12)| BIT(9), BMASK(15:8));
            W2BYTEMSK(REG_PM_SLEEP_40_L, TMDS_CLOCK_CMP_VALUE0, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_41_L, TMDS_CLOCK_CMP_VALUE1, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_00_L, 0, BIT(2)); // Clock status mask
            W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(2), BIT(2)); // Clear clock status
            W2BYTEMSK(REG_PM_SLEEP_03_L, 0, BIT(2));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_PM_SLEEP_3B_L, BMASK(7:4)| BIT(1), BMASK(7:0));
            W2BYTEMSK(REG_PM_SLEEP_43_L, TMDS_CLOCK_CMP_VALUE0, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_44_L, TMDS_CLOCK_CMP_VALUE1, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_00_L, 0, BIT(3)); // Clock status mask
            W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(3), BIT(3)); // Clear clock status
            W2BYTEMSK(REG_PM_SLEEP_03_L, 0, BIT(3));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_PM_SLEEP_3B_L, BMASK(15:12)| BIT(9), BMASK(15:8));
            W2BYTEMSK(REG_PM_SLEEP_46_L, TMDS_CLOCK_CMP_VALUE0, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_47_L, TMDS_CLOCK_CMP_VALUE1, BMASK(11:0));
            W2BYTEMSK(REG_PM_SLEEP_00_L, 0, BIT(4)); // Clock status mask
            W2BYTEMSK(REG_PM_SLEEP_02_L, BIT(4), BIT(4)); // Clear clock status
            W2BYTEMSK(REG_PM_SLEEP_03_L, 0, BIT(4));
            break;

        default:

            break;
    };

    // Initial power down
    if((!_Hal_tmds_CheckInputIsMHL(enInputPortType, ucMHLSupportPath)) && (!_Hal_tmds_GetClockStableFlag(enInputPortType)))
    {
        _Hal_tmds_PowerDownControl(enInputPortType, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetHDMI20VersionFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetHDMI20VersionFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bHDMI20Flag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            bHDMI20Flag = ((R2BYTE(REG_COMBO_PHY1_P0_30_L) &BIT(3)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI1:
            bHDMI20Flag = ((R2BYTE(REG_COMBO_PHY1_P1_30_L) &BIT(3)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI2:
            bHDMI20Flag = ((R2BYTE(REG_COMBO_PHY1_P2_30_L) &BIT(3)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI3:
            bHDMI20Flag = ((R2BYTE(REG_COMBO_PHY1_P3_30_L) &BIT(3)) ?TRUE: FALSE);
            break;

        default:

            break;
    };

    return bHDMI20Flag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_HDMI20PHYSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_HDMI20PHYSetting(E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMI20Flag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_6B_L, bHDMI20Flag? BIT(1): BMASK(2:1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_PHY0_P0_5A_L, bHDMI20Flag? 0: BIT(2), BIT(2)); // [2]: disable reg_atop_en_clko_tmds2x
            W2BYTEMSK(REG_COMBO_PHY0_P0_0A_L, BIT(6), BIT(6)); // [6]: reg_af_ls_20out_sel=1
            W2BYTEMSK(REG_COMBO_GP_TOP_10_L, bHDMI20Flag? 0: (BIT(2)| BIT(0)), BMASK(3:0)); // [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_6B_L, bHDMI20Flag? BIT(1): BMASK(2:1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_PHY0_P1_5A_L, bHDMI20Flag? 0: BIT(2), BIT(2)); // [2]: disable reg_atop_en_clko_tmds2x
            W2BYTEMSK(REG_COMBO_PHY0_P1_0A_L, BIT(6), BIT(6)); // [6]: reg_af_ls_20out_sel=1
            W2BYTEMSK(REG_COMBO_GP_TOP_18_L, bHDMI20Flag? 0: (BIT(2)| BIT(0)), BMASK(3:0)); // [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_6B_L, bHDMI20Flag? BIT(1): BMASK(2:1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_PHY0_P2_5A_L, bHDMI20Flag? 0: BIT(2), BIT(2)); // [2]: disable reg_atop_en_clko_tmds2x
            W2BYTEMSK(REG_COMBO_PHY0_P2_0A_L, BIT(6), BIT(6)); // [6]: reg_af_ls_20out_sel=1
            W2BYTEMSK(REG_COMBO_GP_TOP_20_L, bHDMI20Flag? 0: (BIT(2)| BIT(0)), BMASK(3:0)); // [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_6B_L, bHDMI20Flag? BIT(1): BMASK(2:1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_PHY0_P3_5A_L, bHDMI20Flag? 0: BIT(2), BIT(2)); // [2]: disable reg_atop_en_clko_tmds2x
            W2BYTEMSK(REG_COMBO_PHY0_P3_0A_L, BIT(6), BIT(6)); // [6]: reg_af_ls_20out_sel=1
            W2BYTEMSK(REG_COMBO_GP_TOP_28_L, bHDMI20Flag? 0: (BIT(2)| BIT(0)), BMASK(3:0)); // [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_YUV420PHYSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_YUV420PHYSetting(E_MUX_INPUTPORT enInputPortType, MS_BOOL bYUV420Flag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_6B_L, bYUV420Flag? BMASK(2:1): BIT(1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_GP_TOP_10_L, bYUV420Flag? BIT(2): 0, BMASK(4:0)); // [4]: reg_pix_420t_clk_div2_en_p0;  [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_6B_L, bYUV420Flag? BMASK(2:1): BIT(1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_GP_TOP_18_L, bYUV420Flag? BIT(2): 0, BMASK(4:0)); // [4]: reg_pix_420t_clk_div2_en_p0;  [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_6B_L, bYUV420Flag? BMASK(2:1): BIT(1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_GP_TOP_20_L, bYUV420Flag? BIT(2): 0, BMASK(4:0)); // [4]: reg_pix_420t_clk_div2_en_p0;  [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_6B_L, bYUV420Flag? BMASK(2:1): BIT(1), BMASK(2:1)); // [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
            W2BYTEMSK(REG_COMBO_GP_TOP_28_L, bYUV420Flag? BIT(2): 0, BMASK(4:0)); // [4]: reg_pix_420t_clk_div2_en_p0; [3]: reg_pix_sd_clk_div2_en; [2]: reg_pix_clk_div2_en; [1]: reg_xtal_clk_div2_en; [0]: reg_tmds_clk_div2_en

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetClockRatePort()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 _Hal_tmds_GetClockRatePort(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 usClockRate = 0;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            usClockRate = R2BYTE(REG_COMBO_PHY0_P0_3C_L) * 12 / 128;
            break;

        case INPUT_PORT_DVI1:
            usClockRate = R2BYTE(REG_COMBO_PHY0_P1_3C_L) * 12 / 128;
            break;

        case INPUT_PORT_DVI2:
            usClockRate = R2BYTE(REG_COMBO_PHY0_P2_3C_L) * 12 / 128;
            break;

        case INPUT_PORT_DVI3:
            usClockRate = R2BYTE(REG_COMBO_PHY0_P3_3C_L) * 12 / 128;
            break;

        default:

            break;
    };

    return usClockRate;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_EQBandWidthSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_EQBandWidthSetting(E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMI20Flag)
{
    MS_U16 usClockRate = _Hal_tmds_GetClockRatePort(enInputPortType);

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if(bHDMI20Flag)
            {
                // B/G/R channel EQ parameter
                W2BYTEMSK(REG_COMBO_PHY0_P0_60_L, 0, BIT(11)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P0_61_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P0_62_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));

                // GC_ICTRL_PFD = 5'b01111
                W2BYTEMSK(REG_COMBO_PHY0_P0_6F_L, BIT(11), BIT(11)); // atop_gc_rctrl_pll_ove, overwirte enable
                W2BYTEMSK(REG_COMBO_PHY0_P0_6B_L, BMASK(11:8), BMASK(11:8)); // reg_atop_gc_ictrl_pfd_ov
                W2BYTEMSK(REG_COMBO_PHY1_P0_04_L, 0, BIT(12)); // reg_atop_gc_ictrl_pfd_h_ov
            }
            else
            {
                // B/G/R channel EQ parameter
                if(usClockRate >= 250)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P0_60_L, BIT(11)|BIT(4)|BIT(1), BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_61_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_62_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P0_60_L, 0, BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_61_L, 0, BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_62_L, 0, BIT(10)|BIT(4)|BIT(1));
                }

                // GC_ICTRL_PFD
                W2BYTEMSK(REG_COMBO_PHY0_P0_6F_L, 0, BIT(11)); // atop_gc_rctrl_pll_ove, overwrite disable
            }
            break;

        case INPUT_PORT_DVI1:
            if(bHDMI20Flag)
            {
                // B/G/R channel EQ parameter
                W2BYTEMSK(REG_COMBO_PHY0_P1_60_L, 0, BIT(11)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P1_61_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P1_62_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));

                // GC_ICTRL_PFD = 5'b01111
                W2BYTEMSK(REG_COMBO_PHY0_P1_6F_L, BIT(11), BIT(11)); // atop_gc_rctrl_pll_ove, overwirte enable
                W2BYTEMSK(REG_COMBO_PHY0_P1_6B_L, BMASK(11:8), BMASK(11:8)); // reg_atop_gc_ictrl_pfd_ov
                W2BYTEMSK(REG_COMBO_PHY1_P1_04_L, 0, BIT(12)); // reg_atop_gc_ictrl_pfd_h_ov
            }
            else
            {
                // B/G/R channel EQ parameter
                if(usClockRate >= 250)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P1_60_L, BIT(11)|BIT(4)|BIT(1), BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_61_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_62_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P1_60_L, 0, BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_61_L, 0, BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_62_L, 0, BIT(10)|BIT(4)|BIT(1));
                }

                // GC_ICTRL_PFD
                W2BYTEMSK(REG_COMBO_PHY0_P1_6F_L, 0, BIT(11)); // atop_gc_rctrl_pll_ove, overwrite disable
            }
            break;

        case INPUT_PORT_DVI2:
            if(bHDMI20Flag)
            {
                // B/G/R channel EQ parameter
                W2BYTEMSK(REG_COMBO_PHY0_P2_60_L, 0, BIT(11)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P2_61_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P2_62_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));

                // GC_ICTRL_PFD = 5'b01111
                W2BYTEMSK(REG_COMBO_PHY0_P2_6F_L, BIT(11), BIT(11)); // atop_gc_rctrl_pll_ove, overwirte enable
                W2BYTEMSK(REG_COMBO_PHY0_P2_6B_L, BMASK(11:8), BMASK(11:8)); // reg_atop_gc_ictrl_pfd_ov
                W2BYTEMSK(REG_COMBO_PHY1_P2_04_L, 0, BIT(12)); // reg_atop_gc_ictrl_pfd_h_ov
            }
            else
            {
                // B/G/R channel EQ parameter
                if(usClockRate >= 250)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P2_60_L, BIT(11)|BIT(4)|BIT(1), BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_61_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_62_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P2_60_L, 0, BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_61_L, 0, BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_62_L, 0, BIT(10)|BIT(4)|BIT(1));
                }

                // GC_ICTRL_PFD
                W2BYTEMSK(REG_COMBO_PHY0_P2_6F_L, 0, BIT(11)); // atop_gc_rctrl_pll_ove, overwrite disable
            }
            break;

        case INPUT_PORT_DVI3:
            if(bHDMI20Flag)
            {
                // B/G/R channel EQ parameter
                W2BYTEMSK(REG_COMBO_PHY0_P3_60_L, 0, BIT(11)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P3_61_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));
                W2BYTEMSK(REG_COMBO_PHY0_P3_62_L, 0, BIT(10)|BIT(5)|BIT(4)|BIT(1));

                // GC_ICTRL_PFD = 5'b01111
                W2BYTEMSK(REG_COMBO_PHY0_P3_6F_L, BIT(11), BIT(11)); // atop_gc_rctrl_pll_ove, overwirte enable
                W2BYTEMSK(REG_COMBO_PHY0_P3_6B_L, BMASK(11:8), BMASK(11:8)); // reg_atop_gc_ictrl_pfd_ov
                W2BYTEMSK(REG_COMBO_PHY1_P3_04_L, 0, BIT(12)); // reg_atop_gc_ictrl_pfd_h_ov
            }
            else
            {
                // B/G/R channel EQ parameter
                if(usClockRate >= 250)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P3_60_L, BIT(11)|BIT(4)|BIT(1), BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_61_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_62_L, BIT(10)|BIT(4)|BIT(1), BIT(10)|BIT(4)|BIT(1));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P3_60_L, 0, BIT(11)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_61_L, 0, BIT(10)|BIT(4)|BIT(1));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_62_L, 0, BIT(10)|BIT(4)|BIT(1));
                }

                // GC_ICTRL_PFD
                W2BYTEMSK(REG_COMBO_PHY0_P3_6F_L, 0, BIT(11)); // atop_gc_rctrl_pll_ove, overwrite disable
            }
            break;

        default:

            break;
    };

}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_HDMI20AutoEQSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_HDMI20AutoEQSetting(E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMI20Flag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY1_P0_45_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_MIN: TMDS_COARSE_TUNE_14_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_49_L, bHDMI20Flag? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P0_29_L, bHDMI20Flag? BIT(8): BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_11_L, bHDMI20Flag? 0: BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P0_42_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_DETECT_TIME: TMDS_COARSE_TUNE_14_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_4A_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_AABA_NUMBER: TMDS_COARSE_TUNE_14_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_4B_L, bHDMI20Flag? TMDS_FINE_TUNE_AABA_20_NUMBER: TMDS_FINE_TUNE_AABA_14_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_4C_L, bHDMI20Flag? TMDS_FINE_TUNE_UNDER_20_THRESHOLD: TMDS_FINE_TUNE_UNDER_14_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P0_26_L, bHDMI20Flag? (BIT(11)| BIT(9)): (BIT(11)| BIT(9)), BMASK(13:8));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY1_P1_45_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_MIN: TMDS_COARSE_TUNE_14_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_49_L, bHDMI20Flag? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P1_29_L, bHDMI20Flag? BIT(8): BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_11_L, bHDMI20Flag? 0: BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P1_42_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_DETECT_TIME: TMDS_COARSE_TUNE_14_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_4A_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_AABA_NUMBER: TMDS_COARSE_TUNE_14_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_4B_L, bHDMI20Flag? TMDS_FINE_TUNE_AABA_20_NUMBER: TMDS_FINE_TUNE_AABA_14_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_4C_L, bHDMI20Flag? TMDS_FINE_TUNE_UNDER_20_THRESHOLD: TMDS_FINE_TUNE_UNDER_14_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P1_26_L, bHDMI20Flag? (BIT(11)| BIT(9)): (BIT(11)| BIT(9)), BMASK(13:8));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY1_P2_45_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_MIN: TMDS_COARSE_TUNE_14_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_49_L, bHDMI20Flag? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P2_29_L, bHDMI20Flag? BIT(8): BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_11_L, bHDMI20Flag? 0: BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P2_42_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_DETECT_TIME: TMDS_COARSE_TUNE_14_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_4A_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_AABA_NUMBER: TMDS_COARSE_TUNE_14_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_4B_L, bHDMI20Flag? TMDS_FINE_TUNE_AABA_20_NUMBER: TMDS_FINE_TUNE_AABA_14_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_4C_L, bHDMI20Flag? TMDS_FINE_TUNE_UNDER_20_THRESHOLD: TMDS_FINE_TUNE_UNDER_14_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P2_26_L, bHDMI20Flag? (BIT(11)| BIT(9)): (BIT(11)| BIT(9)), BMASK(13:8));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY1_P3_45_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_MIN: TMDS_COARSE_TUNE_14_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_49_L, bHDMI20Flag? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P3_29_L, bHDMI20Flag? BIT(8): BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_11_L, bHDMI20Flag? 0: BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P3_42_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_DETECT_TIME: TMDS_COARSE_TUNE_14_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_4A_L, bHDMI20Flag? TMDS_COARSE_TUNE_20_AABA_NUMBER: TMDS_COARSE_TUNE_14_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_4B_L, bHDMI20Flag? TMDS_FINE_TUNE_AABA_20_NUMBER: TMDS_FINE_TUNE_AABA_14_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_4C_L, bHDMI20Flag? TMDS_FINE_TUNE_UNDER_20_THRESHOLD: TMDS_FINE_TUNE_UNDER_14_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P3_26_L, bHDMI20Flag? (BIT(11)| BIT(9)): (BIT(11)| BIT(9)), BMASK(13:8));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_DEFilterSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_DEFilterSetting(E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMI20Flag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_31_L, bHDMI20Flag? (TMDS_DE_FILTER_HDMI20_VALUE << 8): (TMDS_DE_FILTER_HDMI14_VALUE << 8), BMASK(15:8));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_31_L, bHDMI20Flag? (TMDS_DE_FILTER_HDMI20_VALUE << 8): (TMDS_DE_FILTER_HDMI14_VALUE << 8), BMASK(15:8));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_31_L, bHDMI20Flag? (TMDS_DE_FILTER_HDMI20_VALUE << 8): (TMDS_DE_FILTER_HDMI14_VALUE << 8), BMASK(15:8));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_31_L, bHDMI20Flag? (TMDS_DE_FILTER_HDMI20_VALUE << 8): (TMDS_DE_FILTER_HDMI14_VALUE << 8), BMASK(15:8));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetDEStableFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetDEStableFlag(E_MUX_INPUTPORT enInputPortType, MS_U8 ucStableCount)
{
    MS_BOOL bDEStableFlag = TRUE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            while(ucStableCount--)
            {
                if((R2BYTE(REG_DVI_DTOP_DUAL_P0_31_L) &BIT(6)) != BIT(6))
                {
                    bDEStableFlag = FALSE;
                }
            }

            break;

        case INPUT_PORT_DVI1:
            while(ucStableCount--)
            {
                if((R2BYTE(REG_DVI_DTOP_DUAL_P1_31_L) &BIT(6)) != BIT(6))
                {
                    bDEStableFlag = FALSE;
                }
            }

            break;

        case INPUT_PORT_DVI2:
            while(ucStableCount--)
            {
                if((R2BYTE(REG_DVI_DTOP_DUAL_P2_31_L) &BIT(6)) != BIT(6))
                {
                    bDEStableFlag = FALSE;
                }
            }

            break;

        case INPUT_PORT_DVI3:
            while(ucStableCount--)
            {
                if((R2BYTE(REG_DVI_DTOP_DUAL_P3_31_L) &BIT(6)) != BIT(6))
                {
                    bDEStableFlag = FALSE;
                }
            }

            break;

        default:

            break;
    };

    return bDEStableFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ReceiveAVIInfoFrame()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_ReceiveAVIInfoFrame(E_MUX_INPUTPORT enInputPortType, MS_BOOL bPortSelect0)
{
    MS_BOOL bReceiveAVIInfo = FALSE;

    if(bPortSelect0)
    {
        _Hal_tmds_GetPacketReceiveStatus0();

        if(bReceiveAVIPortSelect0)
        {
            bReceiveAVIInfo = TRUE;

            bReceiveAVIPortSelect0 = FALSE;
        }
    }

    return bReceiveAVIInfo;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_HDMI20MuxSetting0()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_HDMI20MuxSetting0(MS_BOOL bHDMI20Flag)
{
    W2BYTEMSK(REG_HDMI2_DUAL_0_54_L, bHDMI20Flag? 0: BIT(1), BMASK(1:0)); // [1:0]: reg_avg_ctrl_case
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_AutoEQInitialSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_AutoEQInitialSetting(E_MUX_INPUTPORT enInputPortType)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_15_L, 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P0_45_L, (TMDS_COARSE_TUNE_20_MAX << 8)| TMDS_COARSE_TUNE_14_MIN, BMASK(12:8)| BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY0_P0_33_L, (TMDS_COARSE_TUNE_20_START << 8)| BIT(3), BMASK(13:8)| BIT(3));
            W2BYTEMSK(REG_COMBO_PHY1_P0_44_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P0_28_L, 0, BIT(10));
            W2BYTEMSK(REG_COMBO_PHY0_P0_29_L, BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_11_L, BIT(4) | TMDS_CONTINUE_START, BIT(4) | BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_12_L, TMDS_CONTINUOUS_NUMBER, BMASK(15:0));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_15_L, 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P1_45_L, (TMDS_COARSE_TUNE_20_MAX << 8)| TMDS_COARSE_TUNE_14_MIN, BMASK(12:8)| BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY0_P1_33_L, (TMDS_COARSE_TUNE_20_START << 8)| BIT(3), BMASK(13:8)| BIT(3));
            W2BYTEMSK(REG_COMBO_PHY1_P1_44_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P1_28_L, 0, BIT(10));
            W2BYTEMSK(REG_COMBO_PHY0_P1_29_L, BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_11_L, BIT(4) | TMDS_CONTINUE_START, BIT(4) | BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_12_L, TMDS_CONTINUOUS_NUMBER, BMASK(15:0));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_15_L, 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P2_45_L, (TMDS_COARSE_TUNE_20_MAX << 8)| TMDS_COARSE_TUNE_14_MIN, BMASK(12:8)| BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY0_P2_33_L, (TMDS_COARSE_TUNE_20_START << 8)| BIT(3), BMASK(13:8)| BIT(3));
            W2BYTEMSK(REG_COMBO_PHY1_P2_44_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P2_28_L, 0, BIT(10));
            W2BYTEMSK(REG_COMBO_PHY0_P2_29_L, BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_11_L, BIT(4) | TMDS_CONTINUE_START, BIT(4) | BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_12_L, TMDS_CONTINUOUS_NUMBER, BMASK(15:0));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_15_L, 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P3_45_L, (TMDS_COARSE_TUNE_20_MAX << 8)| TMDS_COARSE_TUNE_14_MIN, BMASK(12:8)| BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY0_P3_33_L, (TMDS_COARSE_TUNE_20_START << 8)| BIT(3), BMASK(13:8)| BIT(3));
            W2BYTEMSK(REG_COMBO_PHY1_P3_44_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P3_28_L, 0, BIT(10));
            W2BYTEMSK(REG_COMBO_PHY0_P3_29_L, BIT(15), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_11_L, BIT(4) | TMDS_CONTINUE_START, BIT(4) | BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_12_L, TMDS_CONTINUOUS_NUMBER, BMASK(15:0));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_AutoEQFunctionTrigger()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_AutoEQFunctionTrigger(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 ustemp = 1000;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P0_33_L, 0, BIT(2));

            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P1_33_L, 0, BIT(2));

            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P2_33_L, 0, BIT(2));

            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P3_33_L, 0, BIT(2));

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetAutoEQDoneFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetAutoEQDoneFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bAutoEQDone = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if(R2BYTE(REG_COMBO_PHY1_P0_40_L) &BIT(7))
            {
                bAutoEQDone = TRUE;

                W2BYTEMSK(REG_COMBO_PHY1_P0_41_L, BIT(15), BIT(15));
                W2BYTEMSK(REG_COMBO_PHY1_P0_41_L, 0, BIT(15));
            }

            break;

        case INPUT_PORT_DVI1:
            if(R2BYTE(REG_COMBO_PHY1_P1_40_L) &BIT(7))
            {
                bAutoEQDone = TRUE;

                W2BYTEMSK(REG_COMBO_PHY1_P1_41_L, BIT(15), BIT(15));
                W2BYTEMSK(REG_COMBO_PHY1_P1_41_L, 0, BIT(15));
            }

            break;

        case INPUT_PORT_DVI2:
            if(R2BYTE(REG_COMBO_PHY1_P2_40_L) &BIT(7))
            {
                bAutoEQDone = TRUE;

                W2BYTEMSK(REG_COMBO_PHY1_P2_41_L, BIT(15), BIT(15));
                W2BYTEMSK(REG_COMBO_PHY1_P2_41_L, 0, BIT(15));
            }

            break;

        case INPUT_PORT_DVI3:
            if(R2BYTE(REG_COMBO_PHY1_P3_40_L) &BIT(7))
            {
                bAutoEQDone = TRUE;

                W2BYTEMSK(REG_COMBO_PHY1_P3_41_L, BIT(15), BIT(15));
                W2BYTEMSK(REG_COMBO_PHY1_P3_41_L, 0, BIT(15));
            }

            break;

        default:

            break;
    };

    return bAutoEQDone;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_SetErrorCountStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_SetErrorCountStatus(E_MUX_INPUTPORT enInputPortType, MS_BOOL bHDMI20Flag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_15_L, BIT(10), BIT(10));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_0B_L, BIT(3), BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_0B_L, 0, BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_40_L, BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_40_L, 0, BIT(15));

            if(bHDMI20Flag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_63_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_63_L, 0, BIT(0));
            }

            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_15_L, BIT(10), BIT(10));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_0B_L, BIT(3), BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_0B_L, 0, BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_40_L, BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_40_L, 0, BIT(15));

            if(bHDMI20Flag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_63_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_63_L, 0, BIT(0));
            }

            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_15_L, BIT(10), BIT(10));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_0B_L, BIT(3), BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_0B_L, 0, BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_40_L, BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_40_L, 0, BIT(15));

            if(bHDMI20Flag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_63_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_63_L, 0, BIT(0));
            }

            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_15_L, BIT(10), BIT(10));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_0B_L, BIT(3), BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_0B_L, 0, BIT(3));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_40_L, BIT(15), BIT(15));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_40_L, 0, BIT(15));

            if(bHDMI20Flag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_63_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_63_L, 0, BIT(0));
            }

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetErrorCountStatus14()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetErrorCountStatus14(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bErrorFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P0_07_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P0_08_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P0_09_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_58_L, 0xFE00, BMASK(15:0));

                if(R2BYTE(REG_DVI_DTOP_DUAL_P0_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                {
                    bErrorFlag = TRUE;
                }
                else
                {
                    W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_58_L, 0xFD00, BMASK(15:0));

                    if(R2BYTE(REG_DVI_DTOP_DUAL_P0_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                    {
                        bErrorFlag = TRUE;
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_58_L, 0xFC00, BMASK(15:0));

                        if(R2BYTE(REG_DVI_DTOP_DUAL_P0_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                        {
                            bErrorFlag = TRUE;
                        }
                    }
                }
            }

            break;

        case INPUT_PORT_DVI1:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P1_07_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P1_08_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P1_09_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_58_L, 0xFE00, BMASK(15:0));

                if(R2BYTE(REG_DVI_DTOP_DUAL_P1_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                {
                    bErrorFlag = TRUE;
                }
                else
                {
                    W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_58_L, 0xFD00, BMASK(15:0));

                    if(R2BYTE(REG_DVI_DTOP_DUAL_P1_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                    {
                        bErrorFlag = TRUE;
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_58_L, 0xFC00, BMASK(15:0));

                        if(R2BYTE(REG_DVI_DTOP_DUAL_P1_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                        {
                            bErrorFlag = TRUE;
                        }
                    }
                }
            }

            break;

        case INPUT_PORT_DVI2:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P2_07_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P2_08_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P2_09_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_58_L, 0xFE00, BMASK(15:0));

                if(R2BYTE(REG_DVI_DTOP_DUAL_P2_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                {
                    bErrorFlag = TRUE;
                }
                else
                {
                    W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_58_L, 0xFD00, BMASK(15:0));

                    if(R2BYTE(REG_DVI_DTOP_DUAL_P2_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                    {
                        bErrorFlag = TRUE;
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_58_L, 0xFC00, BMASK(15:0));

                        if(R2BYTE(REG_DVI_DTOP_DUAL_P2_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                        {
                            bErrorFlag = TRUE;
                        }
                    }
                }
            }

            break;

        case INPUT_PORT_DVI3:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P3_07_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P3_08_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else if((R2BYTE(REG_DVI_DTOP_DUAL_P3_09_L) &BMASK(3:0)) > 0)
            {
                bErrorFlag = TRUE;
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_58_L, 0xFE00, BMASK(15:0));

                if(R2BYTE(REG_DVI_DTOP_DUAL_P3_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                {
                    bErrorFlag = TRUE;
                }
                else
                {
                    W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_58_L, 0xFD00, BMASK(15:0));

                    if(R2BYTE(REG_DVI_DTOP_DUAL_P3_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                    {
                        bErrorFlag = TRUE;
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_58_L, 0xFC00, BMASK(15:0));

                        if(R2BYTE(REG_DVI_DTOP_DUAL_P3_59_L) > HDMI_DECORD_ERROR_THRESHOLD)
                        {
                            bErrorFlag = TRUE;
                        }
                    }
                }
            }

            break;

        default:

            break;
    };

    return bErrorFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetErrorCountStatus20()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetErrorCountStatus20(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bErrorFlag = FALSE;
    MS_U16 usSteerErrorStatus = 0;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            usSteerErrorStatus = R2BYTE(REG_DVI_DTOP_DUAL_P0_18_L) &BMASK(11:0);
            break;

        case INPUT_PORT_DVI1:
            usSteerErrorStatus = R2BYTE(REG_DVI_DTOP_DUAL_P1_18_L) &BMASK(11:0);
            break;

        case INPUT_PORT_DVI2:
            usSteerErrorStatus = R2BYTE(REG_DVI_DTOP_DUAL_P2_18_L) &BMASK(11:0);
            break;

        case INPUT_PORT_DVI3:
            usSteerErrorStatus = R2BYTE(REG_DVI_DTOP_DUAL_P3_18_L) &BMASK(11:0);
            break;

        default:

            break;
    };

    if((usSteerErrorStatus &BMASK(3:0)) != 0x4)
    {
        if((usSteerErrorStatus &BMASK(7:4)) != 0x40)
        {
            if((usSteerErrorStatus &BMASK(11:8)) != 0x400)
            {
                bErrorFlag = TRUE;
            }
        }
    }

    return bErrorFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_AutoEQ14ModeChange()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_AutoEQ14ModeChange(E_MUX_INPUTPORT enInputPortType, MS_U8 ucModeType)
{
    MS_BOOL bSettingFlag0 = FALSE;
    MS_BOOL bSettingFlag1 = FALSE;
    MS_BOOL bSettingFlag2 = FALSE;

    if(ucModeType == HDMI_AUTO_EQ_14_MODE1)
    {
        bSettingFlag0 = TRUE;
        bSettingFlag1 = TRUE;
        bSettingFlag2 = TRUE;
    }

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_COMBO_PHY0_P0_60_L, (bSettingFlag0? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P0_61_L, (bSettingFlag1? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P0_62_L, (bSettingFlag2? BIT(5): 0), BIT(5));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_COMBO_PHY0_P1_60_L, (bSettingFlag0? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P1_61_L, (bSettingFlag1? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P1_62_L, (bSettingFlag2? BIT(5): 0), BIT(5));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_COMBO_PHY0_P2_60_L, (bSettingFlag0? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P2_61_L, (bSettingFlag1? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P2_62_L, (bSettingFlag2? BIT(5): 0), BIT(5));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_COMBO_PHY0_P3_60_L, (bSettingFlag0? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P3_61_L, (bSettingFlag1? BIT(5): 0), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P3_62_L, (bSettingFlag2? BIT(5): 0), BIT(5));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_AutoEQFunctionEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_AutoEQFunctionEnable(E_MUX_INPUTPORT enInputPortType, MS_BOOL bAutoEQEnable)
{
    MS_BOOL bAutoEQTrigger = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if(bAutoEQEnable) // Disable overwrite EQ, then enable auto EQ and trigger auto EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L, 0, BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P0_25_L, BIT(2), BIT(2));

                bAutoEQTrigger = TRUE;
            }
            else // Disable auto EQ, then overwrite EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P0_25_L, 0, BIT(2));
                W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L, BMASK(7:4), BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P0_73_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
                W2BYTEMSK(REG_COMBO_PHY0_P0_74_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
            }

            break;

        case INPUT_PORT_DVI1:
            if(bAutoEQEnable) // Disable overwrite EQ, then enable auto EQ and trigger auto EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P1_6E_L, 0, BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P1_25_L, BIT(2), BIT(2));

                bAutoEQTrigger = TRUE;
            }
            else // Disable auto EQ, then overwrite EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P1_25_L, 0, BIT(2));
                W2BYTEMSK(REG_COMBO_PHY0_P1_6E_L, BMASK(7:4), BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P1_73_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
                W2BYTEMSK(REG_COMBO_PHY0_P1_74_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
            }

            break;

        case INPUT_PORT_DVI2:
            if(bAutoEQEnable) // Disable overwrite EQ, then enable auto EQ and trigger auto EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P2_6E_L, 0, BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P2_25_L, BIT(2), BIT(2));

                bAutoEQTrigger = TRUE;
            }
            else // Disable auto EQ, then overwrite EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P2_25_L, 0, BIT(2));
                W2BYTEMSK(REG_COMBO_PHY0_P2_6E_L, BMASK(7:4), BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P2_73_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
                W2BYTEMSK(REG_COMBO_PHY0_P2_74_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
            }

            break;

        case INPUT_PORT_DVI3:
            if(bAutoEQEnable) // Disable overwrite EQ, then enable auto EQ and trigger auto EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P3_6E_L, 0, BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P3_25_L, BIT(2), BIT(2));

                bAutoEQTrigger = TRUE;
            }
            else // Disable auto EQ, then overwrite EQ
            {
                W2BYTEMSK(REG_COMBO_PHY0_P3_25_L, 0, BIT(2));
                W2BYTEMSK(REG_COMBO_PHY0_P3_6E_L, BMASK(7:4), BMASK(7:4));
                W2BYTEMSK(REG_COMBO_PHY0_P3_73_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
                W2BYTEMSK(REG_COMBO_PHY0_P3_74_L, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, BMASK(12:8)| BMASK(4:0));
            }

            break;

        default:

            break;
    };

    if(bAutoEQTrigger)
    {
        _Hal_tmds_GetAutoEQDoneFlag(enInputPortType);
        _Hal_tmds_AutoEQFunctionTrigger(enInputPortType);
    }
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetHDMIModeFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetHDMIModeFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bHDMIModeFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            bHDMIModeFlag = ((R2BYTE(REG_HDCP_DUAL_P0_01_L) &BIT(0)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI1:
            bHDMIModeFlag = ((R2BYTE(REG_HDCP_DUAL_P1_01_L) &BIT(0)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI2:
            bHDMIModeFlag = ((R2BYTE(REG_HDCP_DUAL_P2_01_L) &BIT(0)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI3:
            bHDMIModeFlag = ((R2BYTE(REG_HDCP_DUAL_P3_01_L) &BIT(0)) ?TRUE: FALSE);
            break;

        default:

            break;
    };

    return bHDMIModeFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetHDCP22Flag()
//  [Description]
//                  Get input is HDCP2.2 or not
//  [Arguments]:
//
//  [Return]:
//                 TRUE: HDCP2.2, FALSE: HDCP1.4
//**************************************************************************
MS_BOOL _Hal_tmds_GetHDCP22Flag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bHDCP22Flag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            bHDCP22Flag = ((R2BYTE(REG_HDCP_DUAL_P0_4F_L) &BIT(0)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI1:
            bHDCP22Flag = ((R2BYTE(REG_HDCP_DUAL_P1_4F_L) &BIT(0)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI2:
            bHDCP22Flag = ((R2BYTE(REG_HDCP_DUAL_P2_4F_L) &BIT(0)) ?TRUE: FALSE);
            break;

        case INPUT_PORT_DVI3:
            bHDCP22Flag = ((R2BYTE(REG_HDCP_DUAL_P3_4F_L) &BIT(0)) ?TRUE: FALSE);
            break;

        default:

            break;
    };

    return bHDCP22Flag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetHDCP22IntStatus()
//  [Description]
//                  Get HDCP2.2 interrupt status
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 _Hal_tmds_GetHDCP22IntStatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 ulHDCP22Status = 0;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            ulHDCP22Status = R2BYTE(REG_HDCP_DUAL_P0_66_L);

            if(ulHDCP22Status & BIT(1))
            {
                W2BYTE(REG_HDCP_DUAL_P0_66_L, BIT(1));
            }

            break;

        case INPUT_PORT_DVI1:
            ulHDCP22Status = R2BYTE(REG_HDCP_DUAL_P1_66_L);

            if(ulHDCP22Status & BIT(1))
            {
                W2BYTE(REG_HDCP_DUAL_P1_66_L, BIT(1));
            }

            break;

        case INPUT_PORT_DVI2:
            ulHDCP22Status = R2BYTE(REG_HDCP_DUAL_P2_66_L);

            if(ulHDCP22Status & BIT(1))
            {
                W2BYTE(REG_HDCP_DUAL_P2_66_L, BIT(1));
            }

            break;

        case INPUT_PORT_DVI3:
            ulHDCP22Status = R2BYTE(REG_HDCP_DUAL_P3_66_L);

            if(ulHDCP22Status & BIT(1))
            {
                W2BYTE(REG_HDCP_DUAL_P3_66_L, BIT(1));
            }

            break;

        default:

            break;
    };

    return ulHDCP22Status;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_SetHDCPWindowEnd()
//  [Description]
//                  Set HDCP EESS mode window of opportunity end configuration
//  [Arguments]:
//                   TRUE: HDCP2.2; FALSE: HDCP1.4
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_SetHDCPWindowEnd(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnableFlag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_HDCP_DUAL_P0_02_L, (bEnableFlag?(TMDS_HDCP_WINDOW_END_VALUE << 8): 0), BMASK(15:8));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_HDCP_DUAL_P1_02_L, (bEnableFlag?(TMDS_HDCP_WINDOW_END_VALUE << 8): 0), BMASK(15:8));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_HDCP_DUAL_P2_02_L, (bEnableFlag?(TMDS_HDCP_WINDOW_END_VALUE << 8): 0), BMASK(15:8));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_HDCP_DUAL_P3_02_L, (bEnableFlag?(TMDS_HDCP_WINDOW_END_VALUE << 8): 0), BMASK(15:8));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_CheckTMDSDataInFlag()
//  [Description]
//                  Check TMDS data receive
//  [Arguments]:
//
//  [Return]:
//                  TRUE or FALSE
//**************************************************************************
MS_BOOL _Hal_tmds_CheckTMDSDataInFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bDataInFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P0_18_L) &BMASK(2:0)) > 0)
            {
                if(R2BYTE(REG_PM_SCDC0_05_L) &BIT(4))
                {
                    bDataInFlag = TRUE;
                }
            }
            break;

        case INPUT_PORT_DVI1:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P1_18_L) &BMASK(2:0)) > 0)
            {
                if(R2BYTE(REG_PM_SCDC1_05_L) &BIT(4))
                {
                    bDataInFlag = TRUE;
                }
            }
            break;

        case INPUT_PORT_DVI2:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P2_18_L) &BMASK(2:0)) > 0)
            {
                if(R2BYTE(REG_PM_SCDC2_05_L) &BIT(4))
                {
                    bDataInFlag = TRUE;
                }
            }
            break;

        case INPUT_PORT_DVI3:
            if((R2BYTE(REG_DVI_DTOP_DUAL_P3_18_L) &BMASK(2:0)) > 0)
            {
                if(R2BYTE(REG_PM_SCDC3_05_L) &BIT(4))
                {
                    bDataInFlag = TRUE;
                }
            }
            break;

        default:

            break;
    };

    return bDataInFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_CheckScrambleStatus()
//  [Description]
//                  Check TMDS scramble status
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_CheckScrambleStatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bScrambleFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_PM_SCDC0_02_L, 0x21, BIT(13)| BMASK(7:0));
            W2BYTEMSK(REG_PM_SCDC0_02_L, BIT(10), BIT(10));

            bScrambleFlag = ((R2BYTE(REG_PM_SCDC0_04_L) &BIT(0)) ?TRUE: FALSE);

            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_PM_SCDC1_02_L, 0x21, BIT(13)| BMASK(7:0));
            W2BYTEMSK(REG_PM_SCDC1_02_L, BIT(10), BIT(10));

            bScrambleFlag = ((R2BYTE(REG_PM_SCDC1_04_L) &BIT(0)) ?TRUE: FALSE);

            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_PM_SCDC2_02_L, 0x21, BIT(13)| BMASK(7:0));
            W2BYTEMSK(REG_PM_SCDC2_02_L, BIT(10), BIT(10));

            bScrambleFlag = ((R2BYTE(REG_PM_SCDC2_04_L) &BIT(0)) ?TRUE: FALSE);

            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_PM_SCDC3_02_L, 0x21, BIT(13)| BMASK(7:0));
            W2BYTEMSK(REG_PM_SCDC3_02_L, BIT(10), BIT(10));

            bScrambleFlag = ((R2BYTE(REG_PM_SCDC3_04_L) &BIT(0)) ?TRUE: FALSE);

            break;

        default:

            break;
    };

    return bScrambleFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_DisableScrambleFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_DisableScrambleFlag(E_MUX_INPUTPORT enInputPortType, MS_BOOL bDisableFlag)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_PM_SCDC0_05_L, bDisableFlag? BIT(5): 0, BMASK(5:4));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_PM_SCDC1_05_L, bDisableFlag? BIT(5): 0, BMASK(5:4));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_PM_SCDC2_05_L, bDisableFlag? BIT(5): 0, BMASK(5:4));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_PM_SCDC3_05_L, bDisableFlag? BIT(5): 0, BMASK(5:4));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ClearErrorCheck()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_ClearErrorCheck(E_MUX_INPUTPORT enInputPortType)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_63_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_63_L, 0, BIT(0));
            break;

        case INPUT_PORT_DVI1:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_63_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_63_L, 0, BIT(0));
            break;

        case INPUT_PORT_DVI2:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_63_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_63_L, 0, BIT(0));
            break;

        case INPUT_PORT_DVI3:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_63_L, BIT(0), BIT(0));
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_63_L, 0, BIT(0));
            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_CheckHDMI20Status()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_CheckHDMI20Status(E_MUX_INPUTPORT enInputPortType, stHDMI_POLLING_INFO *stHDMIPollingInfo)
{
    MS_BOOL bPortSelect0 = FALSE;
    MS_BOOL bYUV420PortSelect0 = FALSE;
    MS_BOOL bAutoEQTrigger = FALSE;
    MS_BOOL bDEStableFlag = _Hal_tmds_GetDEStableFlag(enInputPortType, 1);
    MS_U16 usClockCount = 0;

    if(_Hal_tmds_CheckPortSelectMux0(HDMI_GET_PORT_SELECT(enInputPortType)))
    {
        bPortSelect0 = TRUE;
        bYUV420PortSelect0 = _Hal_tmds_CheckYUV420PortSelect0();
    }

    if(_Hal_tmds_GetHDMI20VersionFlag(enInputPortType)) // Get HDMI 2.0 flag
    {
        if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag)
        {
            //_Hal_tmds_DEFilterSetting(enInputPortType, TRUE);
            _Hal_tmds_HDMI20AutoEQSetting(enInputPortType, TRUE);

            MHAL_HDMIRX_MSG_INFO("** TMDS switch to HDMI 2.0 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag = TRUE;
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = FALSE;
        }

        if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag)
        {
            _Hal_tmds_EQBandWidthSetting(enInputPortType, TRUE);

            bAutoEQTrigger = TRUE;

            MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ enable in HDMI 2.0 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = TRUE;
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckScrambleCounter = 0;
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion = HDMI_SOURCE_VERSION_NOT_SURE;
            //stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_NO_ENCRYPTION;

            _Hal_tmds_DisableScrambleFlag(enInputPortType, FALSE);
            _Hal_tmds_ClearErrorCheck(enInputPortType);
        }

        if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckScrambleCounter <= HDMI_CHECK_SCRAMBLE_INTERVAL)
        {
            if(_Hal_tmds_CheckTMDSDataInFlag(enInputPortType)) // Check TMDS data in
            {
                if(_Hal_tmds_CheckScrambleStatus(enInputPortType))
                {
                    MHAL_HDMIRX_MSG_INFO("** TMDS get scramble time %d port %d\r\n", stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckScrambleCounter, HDMI_GET_PORT_SELECT(enInputPortType));

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckScrambleCounter = HDMI_CHECK_SCRAMBLE_INTERVAL +1;
                }
                else
                {
                    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckScrambleCounter == HDMI_CHECK_SCRAMBLE_INTERVAL)
                    {
                        _Hal_tmds_DisableScrambleFlag(enInputPortType, TRUE);

                        MHAL_HDMIRX_MSG_INFO("** TMDS not get scramble port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));
                    }

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckScrambleCounter++;
                }
            }
        }
    }
    else // HDMI 1.4 setting
    {
        if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag)
        {
            //_Hal_tmds_DEFilterSetting(enInputPortType, FALSE);
            _Hal_tmds_HDMI20AutoEQSetting(enInputPortType, FALSE);

            MHAL_HDMIRX_MSG_INFO("** TMDS switch to HDMI 1.4 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag = FALSE;
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = FALSE;
        }

        usClockCount = _Hal_tmds_GetClockRatePort(enInputPortType);

        if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usClockCount != usClockCount)
        {
            if(ABS_MINUS(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usClockCount, usClockCount) > HDMI_CLOCK_UNSTABLE_OFFSET)
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = FALSE;
            }

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usClockCount = usClockCount;
        }

        if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag)
        {
            _Hal_tmds_EQBandWidthSetting(enInputPortType, FALSE);
            _Hal_tmds_DisableScrambleFlag(enInputPortType, FALSE);

            if(usClockCount > HDMI_AUTO_EQ_ENABLE_THRESHOLD)
            {
                bAutoEQTrigger = TRUE;

                MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ enable in HDMI 1.4 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));
            }
            else
            {
                _Hal_tmds_AutoEQFunctionEnable(enInputPortType, FALSE);

                MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ disable in HDMI 1.4 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable = FALSE;
            }

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = TRUE;
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion = HDMI_SOURCE_VERSION_NOT_SURE;
            //stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_NO_ENCRYPTION;
        }
    }

    if(bAutoEQTrigger)
    {
        _Hal_tmds_AutoEQFunctionEnable(enInputPortType, TRUE);

        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable = TRUE;
        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_START;
    }

    // Check HDMI mode to control HDCP frame clear
    if(bDEStableFlag)
    {
        if(_Hal_tmds_ReceiveAVIInfoFrame(enInputPortType, bPortSelect0))
        {
            if(bYUV420PortSelect0)
            {
                if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag)
                {
                    _Hal_tmds_YUV420PHYSetting(enInputPortType, TRUE);

                    MHAL_HDMIRX_MSG_INFO("** HDMI get YUV420 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag = TRUE;
                }
            }
            else
            {
                if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag)
                {
                    _Hal_tmds_YUV420PHYSetting(enInputPortType, FALSE);

                    MHAL_HDMIRX_MSG_INFO("** HDMI disable YUV420 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag = FALSE;
                }
            }
        }
        else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag)
        {
            _Hal_tmds_YUV420PHYSetting(enInputPortType, FALSE);

            MHAL_HDMIRX_MSG_INFO("** HDMI disable YUV420 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag = FALSE;
        }

        if(_Hal_tmds_GetHDMIModeFlag(enInputPortType)) // HDMI mode
        {
            if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMIModeFlag)
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMIModeFlag = TRUE;
            }
        }
        else // DVI mode
        {
            if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMIModeFlag)
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMIModeFlag = FALSE;
            }
        }

        if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bIsRepeater) // not Repeater mode
        {
            // patch for +LGUplus STB settop box
            if(Hal_HDCP_getstatus(enInputPortType) & BIT(8)) // AKSV interrupt status: HDCP1.4
            {
                _Hal_tmds_SetHDCPWindowEnd(enInputPortType, FALSE);

                Hal_HDCP_ClearStatus(enInputPortType, BIT(0));

                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_1_4;

                //MHAL_HDMIRX_MSG_INFO("** HDMI HDCP state 1.4, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
            else if(_Hal_tmds_GetHDCP22IntStatus(enInputPortType) & BIT(1)) // AKE_Init interrupt status: HDCP2.2
            {
                _Hal_tmds_SetHDCPWindowEnd(enInputPortType, TRUE);

                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_2_2;

                //MHAL_HDMIRX_MSG_INFO("** HDMI HDCP state 2.2, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
        }
        else // is Repeater mode
        {
            if (stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPInt & BIT(0)) // AKSV interrupt status: HDCP1.4
            {
                _Hal_tmds_SetHDCPWindowEnd(enInputPortType, FALSE);

                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_1_4;

                //MHAL_HDMIRX_MSG_INFO("** Repeater mode, HDMI HDCP state 1.4, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
            else if(_Hal_tmds_GetHDCP22IntStatus(enInputPortType) & BIT(1)) // AKE_Init interrupt status: HDCP2.2
            {
                _Hal_tmds_SetHDCPWindowEnd(enInputPortType, TRUE);

                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_2_2;

                //MHAL_HDMIRX_MSG_INFO("** Repeater mode, HDMI HDCP state 2.2, port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
        }

        if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion == HDMI_SOURCE_VERSION_NOT_SURE)
        {
            if(_Hal_tmds_GetSCDCStatusFlag(enInputPortType, TMDS_SCDC_ACCESS_ADDERSS_A8))
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion = HDMI_SOURCE_VERSION_HDMI20;

                MHAL_HDMIRX_MSG_INFO("** TMDS source type HDMI 2.0 port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
            else
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion = HDMI_SOURCE_VERSION_HDMI14;

                MHAL_HDMIRX_MSG_INFO("** TMDS source type HDMI 1.4 port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
            }
        }
        else if(_Hal_tmds_GetSCDCStatusFlag(enInputPortType, TMDS_SCDC_ACCESS_ADDERSS_A8))
        {
            // Do nothing, only for clear status
        }
    }
    else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag) // DE unstabe then back to HDMI 2.0 setting
    {
        _Hal_tmds_YUV420PHYSetting(enInputPortType, FALSE);

        MHAL_HDMIRX_MSG_INFO("** HDMI back to HDMI 2.0 port %d\r\n", HDMI_GET_PORT_SELECT(enInputPortType));

        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bYUV420Flag = FALSE;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_AutoEQFunctionProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_AutoEQFunctionProc(E_MUX_INPUTPORT enInputPortType, stHDMI_POLLING_INFO *stHDMIPollingInfo)
{
    MS_BOOL bAutoEQTrigger = FALSE;
    MS_BOOL bAutoEQContinueFlag = FALSE;

    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable)
    {
        do
        {
            switch(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState)
            {
                case HDMI_AUTO_EQ_START:
                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_WAIT_DONE;
                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQRetrigger = FALSE;
                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode = HDMI_AUTO_EQ_14_MODE0;
                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usCheckErrorTimes = 0;
                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usAutoEQProcCounter = 0;
                    break;

                case HDMI_AUTO_EQ_WAIT_DONE:
                    if(_Hal_tmds_GetAutoEQDoneFlag(enInputPortType)) // Auto EQ done
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_CHECK_STABLE;

                        bAutoEQContinueFlag = TRUE;
                    }
                    else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usAutoEQProcCounter >= TMDS_AUTO_EQ_PROCESS_INTERVAL)
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_FINISH_PROCESS;
                    }

                    break;

                case HDMI_AUTO_EQ_FINISH_PROCESS:
                    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag)
                    {
                        Hal_HDMI_Set_EQ(enInputPortType, MS_HDMI_EQ_USER_DEFINE, HDMI_EQ_20_SETTING_VALUE);
                    }
                    else
                    {
                        Hal_HDMI_Set_EQ(enInputPortType, MS_HDMI_EQ_USER_DEFINE, HDMI_EQ_14_SETTING_VALUE);
                    }

                    MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ not done finish port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable = FALSE;
                    bAutoEQContinueFlag = FALSE;

                    break;

                case HDMI_AUTO_EQ_CHECK_STABLE:
                    if(_Hal_tmds_GetDEStableFlag(enInputPortType, 5)) // DE stable, auto EQ done
                    {
                        if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQRetrigger)
                        {
                            bAutoEQTrigger = TRUE;

                            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQRetrigger = TRUE;
                        }
                        else
                        {
                            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_CHECK_DONE;
                        }
                    }
                    else // DE unstable, trigger auto EQ again and add auto EQ trigger count
                    {
                        if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag)
                        {
                            _Hal_tmds_AutoEQ14ModeChange(enInputPortType, stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode);

                            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode = (stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode +1) %HDMI_AUTO_EQ_14_MODE_MASK;
                        }

                        bAutoEQTrigger = TRUE;

                        MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ done but retry port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
                    }

                    bAutoEQContinueFlag = FALSE;

                    break;

                case HDMI_AUTO_EQ_CHECK_DONE:
                    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bHDMI20Flag)
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_20_CLEAR_STATUS;
                    }
                    else
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_14_CLEAR_STATUS;
                    }

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval = 0;

                    bAutoEQContinueFlag = TRUE;

                    break;

                case HDMI_AUTO_EQ_14_CLEAR_STATUS:
                    //_Hal_tmds_ResetHDMI20VersionFlag(enInputPortType);
                    _Hal_tmds_SetErrorCountStatus(enInputPortType, FALSE);

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_14_CHECK_STATUS;

                    bAutoEQContinueFlag = FALSE;

                    MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ clear HDMI 1.4 error status port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));

                    break;

                case HDMI_AUTO_EQ_14_CHECK_STATUS:
                    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval < TMDS_14_CHECK_ERROR_INTERVAL)
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval++;
                    }
                    else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usCheckErrorTimes < TMDS_14_CHECK_ERROR_TIMES)
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval = 0;
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usCheckErrorTimes++;

                        if((!_Hal_tmds_GetDEStableFlag(enInputPortType, 1)) || _Hal_tmds_GetErrorCountStatus14(enInputPortType)) // Get error
                        {
                            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_14_CHANGE_SETTING;

                            bAutoEQContinueFlag = TRUE;

                            MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ HDMI 1.4 error happen port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
                        }
                    }
                    else
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_STABLE_DONE;
                    }

                    break;

                case HDMI_AUTO_EQ_14_CHANGE_SETTING:
                    _Hal_tmds_AutoEQ14ModeChange(enInputPortType, stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode);

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode = (stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQ14Mode +1) %HDMI_AUTO_EQ_14_MODE_MASK;

                    bAutoEQTrigger = TRUE;
                    bAutoEQContinueFlag = FALSE;

                    break;

                case HDMI_AUTO_EQ_20_CLEAR_STATUS:
                    _Hal_tmds_SetErrorCountStatus(enInputPortType, TRUE);

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_20_CHECK_STATUS;

                    bAutoEQContinueFlag = FALSE;

                    MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ clear HDMI 2.0 error status port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));

                    break;

                case HDMI_AUTO_EQ_20_CHECK_STATUS:
                    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval < TMDS_20_CHECK_ERROR_INTERVAL)
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval++;
                    }
                    else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usCheckErrorTimes < TMDS_20_CHECK_ERROR_TIMES)
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucCheckErrorInterval = 0;
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usCheckErrorTimes++;

                        if((!_Hal_tmds_GetDEStableFlag(enInputPortType, 1)) || _Hal_tmds_GetErrorCountStatus20(enInputPortType)) // Get error
                        {
                            bAutoEQTrigger = TRUE;

                            MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ HDMI 2.0 error happen port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
                        }
                    }
                    else
                    {
                        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_STABLE_DONE;
                    }

                    break;

                case HDMI_AUTO_EQ_STABLE_DONE:
                    MHAL_HDMIRX_MSG_INFO("** TMDS auto EQ done completely port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));

                    stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable = FALSE;
                    bAutoEQContinueFlag = FALSE;

                    break;

                default:

                    break;
            };
        }while(bAutoEQContinueFlag);

        if(bAutoEQTrigger)
        {
            _Hal_tmds_AutoEQFunctionTrigger(enInputPortType);

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucAutoEQState = HDMI_AUTO_EQ_WAIT_DONE;
        }

        stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].usAutoEQProcCounter++;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_PowerControlHandler()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_PowerControlHandler(E_MUX_INPUTPORT enInputPortType, stHDMI_POLLING_INFO *stHDMIPollingInfo)
{
    if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bNoInputFlag) // Wait clock stable
    {
        if(_Hal_tmds_GetClockValidFlag(enInputPortType, FALSE) || _Hal_tmds_GetClockStableFlag(enInputPortType)) // Clock stable, then set wait clock unstable
        {
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bNoInputFlag = FALSE;

            //_Hal_tmds_PowerDownControl(enInputPortType, TRUE);

            if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bPowerOnLane)
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bPowerOnLane = TRUE;

                _Hal_tmds_PowerDownControl(enInputPortType, FALSE);
                _Hal_tmds_PowerDownControl(enInputPortType, TRUE);
            }

            _Hal_tmds_ResetClockDetect(enInputPortType);

            MHAL_HDMIRX_MSG_INFO("** TMDS clock stable power on port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
        }
    }
    else // Wait clock unstable
    {
        if(_Hal_tmds_GetClockValidFlag(enInputPortType, TRUE)) // Clock unstable, then set wait clock stable
        {
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bNoInputFlag = TRUE;
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucSourceVersion = HDMI_SOURCE_VERSION_NOT_SURE;
            //stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].ucHDCPState = HDMI_HDCP_NO_ENCRYPTION;

            //if(!_Hal_tmds_GetClockStableFlag(enInputPortType))
            //{
                //_Hal_tmds_PowerDownControl(enInputPortType, FALSE);
            //}

            //_Hal_tmds_ClearSCDCStatusFlag(enInputPortType, TMDS_SCDC_ACCESS_ADDERSS_A8);
            _Hal_tmds_SetHDCPWindowEnd(enInputPortType, FALSE);
            Hal_HDCP_GetEncryptionFlag(enInputPortType);

            if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable)
            {
                _Hal_tmds_AutoEQFunctionEnable(enInputPortType, FALSE);

                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bAutoEQEnable = FALSE;
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enInputPortType)].bTimingStableFlag = FALSE;
            }

            MHAL_HDMIRX_MSG_INFO("** TMDS clock unstable power down port %d\r\n", (enInputPortType -INPUT_PORT_DVI0));
        }
    }
}

#if(TMDS_POWER_SWITCH_IRQ_ENABLE)
//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_PowerControlCallBackFunc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
static void _Hal_tmds_PowerControlCallBackFunc(InterruptNum eIntNum)
{
    E_MUX_INPUTPORT enPortIndex = INPUT_PORT_DVI0;

    UNUSED(eIntNum);

    for(enPortIndex = INPUT_PORT_DVI0; enPortIndex <= INPUT_PORT_DVI_END; enPortIndex++)
    {
        _Hal_tmds_PowerControlHandler(enPortIndex);
    }

    MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
}

#endif

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_ReadEfuseData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U32 _Hal_tmds_ReadEfuseData(MS_U8 ucEfuseBank)
{
    MS_U8 ucCounter = 0;
    MS_U32 ulEfuseData = 0;

    W2BYTEMSK(REG_PM_EFUSE_28_L, BIT(13)| (ucEfuseBank << 2), BIT(13)| BMASK(8:2));

    while(R2BYTE(REG_PM_EFUSE_28_L) &BIT(13))
    {
        MsOS_DelayTask(1);
        ucCounter ++;

        if(ucCounter > 10)
        {
            break;
        }
    }

    ulEfuseData = R4BYTE(REG_PM_EFUSE_2C_L);

    return ulEfuseData;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_GetEQCalibrateFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _Hal_tmds_GetEQCalibrateFlag(void)
{
    MS_BOOL bEQCalibrateFlag = FALSE;
    MS_U32 ulEfuseData = _Hal_tmds_ReadEfuseData(0x62);

    if(ulEfuseData &BIT(0))
    {
        bEQCalibrateFlag = TRUE;

        MHAL_HDMIRX_MSG_INFO("** TMDS EQ calibration enable by efuse\r\n");
    }

    return bEQCalibrateFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _Hal_tmds_EQCalibrationProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _Hal_tmds_EQCalibrationProc(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bFinishFlag = FALSE;
    MS_U8 ucEQCalCodeLane0 = 0;
    MS_U8 ucEQCalCodeLane1 = 0;
    MS_U8 ucEQCalCodeLane2 = 0;
    MS_U16 ustemp = 0;
    MS_U32 ulEfuseData = 0;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            ulEfuseData = _Hal_tmds_ReadEfuseData(0x60);
            ucEQCalCodeLane0 = ulEfuseData &BMASK(4:0);
            ucEQCalCodeLane1 = (ulEfuseData &BMASK(9:5)) >> 5;
            ucEQCalCodeLane2 = (ulEfuseData &BMASK(14:10)) >> 10;

            if(ulEfuseData &BIT(15)) // Finish flag
            {
                W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BIT(9));
                W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BIT(9), BIT(9));

                ustemp = HDMI_EQ_CAL_TRIGGER_COUNT;
                while(ustemp--);

                W2BYTEMSK(REG_COMBO_PHY1_P0_03_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_COMBO_PHY1_P0_04_L, BIT(8), BMASK(9:8));

                if((R2BYTE(REG_COMBO_PHY1_P0_05_L) &BMASK(6:4)) == BMASK(6:4))
                {
                    // Calibration lane 0
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, ucEQCalCodeLane0, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BIT(5));

                    // Calibration lane 1
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BIT(6), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, ucEQCalCodeLane1, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BIT(5));

                    // Calibration lane 2
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BIT(7), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, ucEQCalCodeLane2, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BIT(5));
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, BMASK(7:6), BMASK(7:6));

                    if(((R2BYTE(REG_COMBO_PHY0_P0_76_L) &BMASK(9:5)) >> 5) == ucEQCalCodeLane0)
                    {
                        if(((R2BYTE(REG_COMBO_PHY0_P0_76_L) &BMASK(14:10)) >> 10) == ucEQCalCodeLane1)
                        {
                            if((R2BYTE(REG_COMBO_PHY0_P0_78_L) &BMASK(4:0)) == ucEQCalCodeLane2)
                            {
                                bFinishFlag = TRUE;
                            }
                        }
                    }
                }

                if(bFinishFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BMASK(7:6));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P0_7E_L, 0, BMASK(9:0));
                }

                W2BYTEMSK(REG_COMBO_PHY1_P0_03_L, 0, BIT(1));
            }

            break;

        case INPUT_PORT_DVI1:
            ulEfuseData = (_Hal_tmds_ReadEfuseData(0x60)) >> 16;
            ucEQCalCodeLane0 = ulEfuseData &BMASK(4:0);
            ucEQCalCodeLane1 = (ulEfuseData &BMASK(9:5)) >> 5;
            ucEQCalCodeLane2 = (ulEfuseData &BMASK(14:10)) >> 10;

            if(ulEfuseData &BIT(15)) // Finish flag
            {
                W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BIT(9));
                W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BIT(9), BIT(9));

                ustemp = HDMI_EQ_CAL_TRIGGER_COUNT;
                while(ustemp--);

                W2BYTEMSK(REG_COMBO_PHY1_P1_03_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_COMBO_PHY1_P1_04_L, BIT(8), BMASK(9:8));

                if((R2BYTE(REG_COMBO_PHY1_P1_05_L) &BMASK(6:4)) == BMASK(6:4))
                {
                    // Calibration lane 0
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, ucEQCalCodeLane0, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BIT(5));

                    // Calibration lane 1
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BIT(6), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, ucEQCalCodeLane1, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BIT(5));

                    // Calibration lane 2
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BIT(7), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, ucEQCalCodeLane2, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BIT(5));
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, BMASK(7:6), BMASK(7:6));

                    if(((R2BYTE(REG_COMBO_PHY0_P1_76_L) &BMASK(9:5)) >> 5) == ucEQCalCodeLane0)
                    {
                        if(((R2BYTE(REG_COMBO_PHY0_P1_76_L) &BMASK(14:10)) >> 10) == ucEQCalCodeLane1)
                        {
                            if((R2BYTE(REG_COMBO_PHY0_P1_78_L) &BMASK(4:0)) == ucEQCalCodeLane2)
                            {
                                bFinishFlag = TRUE;
                            }
                        }
                    }
                }

                if(bFinishFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BMASK(7:6));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P1_7E_L, 0, BMASK(9:0));
                }

                W2BYTEMSK(REG_COMBO_PHY1_P1_03_L, 0, BIT(1));
            }

            break;

        case INPUT_PORT_DVI2:
            ulEfuseData = _Hal_tmds_ReadEfuseData(0x61);
            ucEQCalCodeLane0 = ulEfuseData &BMASK(4:0);
            ucEQCalCodeLane1 = (ulEfuseData &BMASK(9:5)) >> 5;
            ucEQCalCodeLane2 = (ulEfuseData &BMASK(14:10)) >> 10;

            if(ulEfuseData &BIT(15)) // Finish flag
            {
                W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BIT(9));
                W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BIT(9), BIT(9));

                ustemp = HDMI_EQ_CAL_TRIGGER_COUNT;
                while(ustemp--);

                W2BYTEMSK(REG_COMBO_PHY1_P2_03_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_COMBO_PHY1_P2_04_L, BIT(8), BMASK(9:8));

                if((R2BYTE(REG_COMBO_PHY1_P2_05_L) &BMASK(6:4)) == BMASK(6:4))
                {
                    // Calibration lane 0
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, ucEQCalCodeLane0, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BIT(5));

                    // Calibration lane 1
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BIT(6), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, ucEQCalCodeLane1, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BIT(5));

                    // Calibration lane 2
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BIT(7), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, ucEQCalCodeLane2, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BIT(5));
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, BMASK(7:6), BMASK(7:6));

                    if(((R2BYTE(REG_COMBO_PHY0_P2_76_L) &BMASK(9:5)) >> 5) == ucEQCalCodeLane0)
                    {
                        if(((R2BYTE(REG_COMBO_PHY0_P2_76_L) &BMASK(14:10)) >> 10) == ucEQCalCodeLane1)
                        {
                            if((R2BYTE(REG_COMBO_PHY0_P2_78_L) &BMASK(4:0)) == ucEQCalCodeLane2)
                            {
                                bFinishFlag = TRUE;
                            }
                        }
                    }
                }

                if(bFinishFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BMASK(7:6));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P2_7E_L, 0, BMASK(9:0));
                }

                W2BYTEMSK(REG_COMBO_PHY1_P2_03_L, 0, BIT(1));
            }

            break;

        case INPUT_PORT_DVI3:
            ulEfuseData = (_Hal_tmds_ReadEfuseData(0x61)) >> 16;
            ucEQCalCodeLane0 = ulEfuseData &BMASK(4:0);
            ucEQCalCodeLane1 = (ulEfuseData &BMASK(9:5)) >> 5;
            ucEQCalCodeLane2 = (ulEfuseData &BMASK(14:10)) >> 10;

            if(ulEfuseData &BIT(15)) // Finish flag
            {
                W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BIT(9));
                W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BIT(9), BIT(9));

                ustemp = HDMI_EQ_CAL_TRIGGER_COUNT;
                while(ustemp--);

                W2BYTEMSK(REG_COMBO_PHY1_P3_03_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_COMBO_PHY1_P3_04_L, BIT(8), BMASK(9:8));

                if((R2BYTE(REG_COMBO_PHY1_P3_05_L) &BMASK(6:4)) == BMASK(6:4))
                {
                    // Calibration lane 0
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, ucEQCalCodeLane0, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BIT(5));

                    // Calibration lane 1
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BIT(6), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, ucEQCalCodeLane1, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BIT(5));

                    // Calibration lane 2
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BIT(7), BMASK(7:6));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, ucEQCalCodeLane2, BMASK(4:0));

                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BIT(5), BIT(5));

                    ustemp = HDMI_EQ_CAL_MEASURE_COUNT;
                    while(ustemp--);

                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BIT(5));
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, BMASK(7:6), BMASK(7:6));

                    if(((R2BYTE(REG_COMBO_PHY0_P3_76_L) &BMASK(9:5)) >> 5) == ucEQCalCodeLane0)
                    {
                        if(((R2BYTE(REG_COMBO_PHY0_P3_76_L) &BMASK(14:10)) >> 10) == ucEQCalCodeLane1)
                        {
                            if((R2BYTE(REG_COMBO_PHY0_P3_78_L) &BMASK(4:0)) == ucEQCalCodeLane2)
                            {
                                bFinishFlag = TRUE;
                            }
                        }
                    }
                }

                if(bFinishFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BMASK(7:6));
                }
                else
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P3_7E_L, 0, BMASK(9:0));
                }

                W2BYTEMSK(REG_COMBO_PHY1_P3_03_L, 0, BIT(1));
            }

            break;

        default:

            break;
    };

    MHAL_HDMIRX_MSG_INFO("** TMDS EQ calibration port %d value %x\r\n", (enInputPortType -INPUT_PORT_DVI0), (MS_U16)ulEfuseData);
}

//****************************************************************
// Initial HDCP22 relative register, structure...etc
//****************************************************************
void Hal_HDCP22_PortInit(MS_U8 ucPortIdx)
{
    MS_U32 dwBKOffset = 0x300 * ucPortIdx;

    MHAL_HDMIRX_MSG_INFO("[HDCP22] PortInit(), port idx = %d\r\n", ucPortIdx);

    W2BYTEMSK(REG_COMBO_GP_TOP_40_L, 0x05, 0x07); // [2]: Enable CPU write; [1]: disable SRAM read; [0]: enable xiu2hdcpkey

    //wirte 74Ram for HDCP22 version
    W2BYTEMSK(REG_HDCP_DUAL_P0_17_L + dwBKOffset, BIT(10), BIT(10)); //HDCP enalbe for DDC
    W2BYTEMSK(REG_HDCP_DUAL_P0_19_L + dwBKOffset, BIT(15), BMASK(15:14)); //write enable

    W2BYTEMSK(REG_HDCP_DUAL_P0_17_L + dwBKOffset, 0x50, BMASK(9:0)); //CPU r/w address
    W2BYTEMSK(REG_HDCP_DUAL_P0_19_L + dwBKOffset, BIT(5), BIT(5)); //HDCP load address pulse

    W2BYTEMSK(REG_HDCP_DUAL_P0_18_L + dwBKOffset, 0x04, BMASK(7:0)); //bit2: HDCP22 version
    W2BYTEMSK(REG_HDCP_DUAL_P0_19_L + dwBKOffset, BIT(4), BIT(4)); //HDCP data write port pulse generate

    //clear RxStatus
    W2BYTEMSK(REG_HDCP_DUAL_P0_63_L + dwBKOffset, 0x00, BMASK(9:0));
    W2BYTEMSK(REG_HDCP_DUAL_P0_64_L + dwBKOffset, BIT(15), BIT(15));

    //clear hdcp_enable cipher bit
    //W2BYTEMSK(REG_HDCP_DUAL_P0_4E_L + dwBKOffset, 0, BIT(0));

    //clear xiu status
    W2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset, 0x3D);
    W2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset, 0x00);

    //Enable write offset
    W2BYTEMSK(REG_HDCP_DUAL_P0_68_L +dwBKOffset, BIT(15)| TMDS_HDCP2_SOURCE_READ_OFFSET, BIT(15)| BMASK(9:0)); //b'[0..9]offset; b'[15]: enable bit
}

//****************************************************************
// Check If there is any message received;
// @param ucPortIdx    [IN] HDMI port index
//****************************************************************
MS_BOOL Hal_HDCP22_PollingWriteDone(MS_U8 ucPortIdx)
{
    MS_BOOL bWriteDoneFlag = FALSE;
    MS_U32 dwBKOffset = 0;

    dwBKOffset = 0x300 * ucPortIdx;

    if(R2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset) & BIT(2)) //check write done
    {
        //clear status
        W2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset, BIT(4)| BIT(2)); //clear write done, write start
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3C_L + dwBKOffset, BIT(14));
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3C_L + dwBKOffset, 0);

        bWriteDoneFlag = TRUE;
    }

    return bWriteDoneFlag;
}

//****************************************************************
// Check If Tx already finish its read process
// @param ucPortIdx    [IN] HDMI port index
//****************************************************************
MS_BOOL Hal_HDCP22_PollingReadDone(MS_U8 ucPortIdx)
{
    MS_BOOL bReadDoneFlag = FALSE;
    MS_U32 dwBKOffset = 0;

    dwBKOffset = 0x300 * ucPortIdx;

    if(R2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset) & BIT(3)) //check read done
    {
        //clear status
        W2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset, BIT(3)); //clear read done
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3C_L + dwBKOffset, BIT(14));
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3C_L + dwBKOffset, 0);

        bReadDoneFlag = TRUE;
    }

    return bReadDoneFlag;
}

//****************************************************************
// Enable or Disable Cipher process
// @param ucPortType    [IN] HDCP type, ex: HDMI or Miracast
// @param ucPortIdx    [IN] HDMI port index
// @param bIsEnable    [IN] Enable/Disable Cipher
//****************************************************************
void Hal_HDCP22_EnableCipher(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_BOOL bIsEnable)
{
    MS_U32 dwBKOffset = 0;

    MHAL_HDMIRX_MSG_INFO("[HDCP22] EnableCipher(), isEnable = %d\r\n", bIsEnable);

    dwBKOffset = (ucPortIdx * 0x300);

    //W2BYTEMSK(REG_HDCP_DUAL_P0_4E_L + dwBKOffset, bIsEnable? BIT(0): 0, BIT(0));
}

//****************************************************************
// Fetch message content
// @param ucPortIdx    [IN] HDMI port index
// @param pucRiv    [IN] Data pointer of riv
// @param pucSessionKey    [IN] Data pointer of ks^LC128
//****************************************************************
void Hal_HDCP22_FillCipherKey(MS_U8 ucPortIdx, MS_U8 *pucSessionKey, MS_U8 *pucRiv)
{
#define EN_DBG_CIPHER_KEY 0
    MS_U8 uctemp = 0;
    MS_U8 ucRegOffset = 0;
    MS_U32 dwBKOffset = 0;

    MHAL_HDMIRX_MSG_INFO("[HDCP22] FillCipherKey\r\n");

    if(ucPortIdx < 10) //to avoid overflow a byte size after multiple with 0x0C
    {
        ucRegOffset = ucPortIdx * 0x18;
        dwBKOffset = ucPortIdx * 0x300;

        //fill riv: size 8 bytes
#if (EN_DBG_CIPHER_KEY == 1)
        MHAL_HDMIRX_MSG_DEBUG("\r\n[HDCP22]Riv\r\n");
#endif

        for(uctemp = 0; uctemp < 8; uctemp++)
        {
            MDrv_WriteByteMask(REG_SECURE_TZPC_38_L + ucRegOffset + 8 - uctemp - 1, *(MS_U8*)(pucRiv + uctemp), BMASK(7:0));

#if (EN_DBG_CIPHER_KEY == 1)
            MHAL_HDMIRX_MSG_DEBUG("0x%02X : ", *(MS_U8*)(pucRiv + uctemp));
            MHAL_HDMIRX_MSG_DEBUG("(0x%x)0x%02X\r\n", REG_SECURE_TZPC_38_L + ucRegOffset + 8 - uctemp - 1, MDrv_ReadByte(REG_SECURE_TZPC_38_L + ucRegOffset + 8 - uctemp - 1));
#endif
        }

        //fill ks^LC128: size 16 bytes
#if (EN_DBG_CIPHER_KEY == 1)
        MHAL_HDMIRX_MSG_DEBUG("\r\n[HDCP22]SessionKey\r\n");
#endif

        for(uctemp = 0; uctemp < 16; uctemp++)
        {
            MDrv_WriteByteMask(REG_SECURE_TZPC_30_L + ucRegOffset + 16 - uctemp - 1, *(MS_U8*)(pucSessionKey + uctemp), BMASK(7:0));

#if (EN_DBG_CIPHER_KEY == 1)
            MHAL_HDMIRX_MSG_DEBUG("0x%02X : ", *(MS_U8*)(pucSessionKey + uctemp));
            MHAL_HDMIRX_MSG_DEBUG("(0x%x)0x%02X\r\n", REG_SECURE_TZPC_30_L + ucRegOffset + 16 - uctemp - 1, MDrv_ReadByte(REG_SECURE_TZPC_30_L + ucRegOffset + 16 - uctemp - 1));
#endif
        }

        //enable cipher
        MHAL_HDMIRX_MSG_INFO("\r\n[HDCP22]Enable Cipher\r\n");

        //W2BYTEMSK(REG_HDCP_DUAL_P0_4E_L + dwBKOffset, BIT(0), BIT(0));
    }
    else
    {
        MHAL_HDMIRX_MSG_INFO("[HDCP22]Hal_HDCP22_FillCipherKey:: Invalid Port Index!!\r\n");
    }

#undef EN_DBG_CIPHER_KEY
}

//****************************************************************
// Fetch message content
// @param ucPortIdx    [IN] HDMI port index
// @param pucData    [IN] Data pointer to store received message
// @param dwDataLen    [IN] message length
//****************************************************************
void Hal_HDCP22_FetchMsg(MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen)
{
    MS_U8 ucRegOffset = ( ucPortIdx * 4 );
    MS_U32 ultemp = 0;

    W2BYTEMSK(REG_COMBO_GP_TOP_40_L, BIT(0), BMASK(2:0)); //enable XIU read
    W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, 0x00, BMASK(9:0)); //start addr

    for(ultemp = 0; ultemp < dwDataLen; ultemp++)
    {
        *(pucData + ultemp) = (R2BYTE(REG_HDCPKEY_0E_L + 2*(ucRegOffset/0x04)) &BMASK(7:0));
    }
}

//****************************************************************
// Receive message and transmitt message to TEE
// @param ucPortIdx    [IN] HDMI port index
//****************************************************************
MS_BOOL Hal_HDCP22_RecvMsg(MS_U8 ucPortIdx, MS_U8 *ucMsgData)
{
    MS_BOOL bReceiveFlag = FALSE;
    MS_U16 ustemp = 0;
    MS_U32 dwMsgLen = 0;
    MS_U32 dwBKOffset = 0;

    dwBKOffset = 0x300 * ucPortIdx;

    Hal_HDCP22_PollingReadDone(ucPortIdx);

    //fetch msg length
    dwMsgLen = R2BYTE(REG_HDCP_DUAL_P0_64_L + dwBKOffset) & BMASK(9:0);

    if(dwMsgLen != 0x00)
    {
        //allocate and init buffer
        for(ustemp = 0; ustemp < HDMI_HDCP22_MESSAGE_LENGTH; ustemp++)
        {
            ucMsgData[ustemp] = 0x00;
        }

        ucMsgData[HDMI_HDCP22_MESSAGE_LENGTH] = dwMsgLen;

        //fetch msg
        Hal_HDCP22_FetchMsg(ucPortIdx, ucMsgData, dwMsgLen);

        //MHAL_HDMIRX_MSG_INFO("[HDCP22] PrtIdx = %d, msgID = 0x%x, len= %td\r\n", ucPortIdx, ucMsgData[0],(ptrdiff_t) dwMsgLen);

        bReceiveFlag = TRUE;
    }

    return bReceiveFlag;
}

//****************************************************************
// Sending message back to TX
// @param ucPortType    [IN] HDCP type, ex: HDMI or Miracast
// @param ucPortIdx    [IN] HDMI port index
// @param pucData    [IN] received message from HDMI port
// @param dwDataLen    [IN] message lenght
// @param pDummy    [IN] dummy pointer for API layer structure
//****************************************************************
void Hal_HDCP22_SendMsg(MS_U8 ucPortType, MS_U8 ucPortIdx, MS_U8* pucData, MS_U32 dwDataLen, void* pDummy, MS_U8* ubRecIDListSetDone)
{
    MS_U8 ucRegOffset = 0;
    MS_U32 ultemp = 0;
    MS_U32 dwBKOffset = 0;

    //MHAL_HDMIRX_MSG_INFO("[HDCP22] Send, PrtIdx = %d\r\n", ucPortIdx);
    ucRegOffset = ucPortIdx * 0x04;
    dwBKOffset = ucPortIdx * 0x300;

    if( (*(pucData) == 0x0C) || (*(pucData) == 0x07) )
    {
        //W2BYTE(REG_HDCP_DUAL_P0_66_L + dwBKOffset, BIT(3)); //clear read done
        Hal_HDCP22_PollingReadDone(ucPortIdx);
    }

    if (*(MS_U8*)(pucData) == 0x15)
    {
        //set rxstatus
        if (dwDataLen >= 0x03) //avoid invalid memory address access
        {
            Hal_HDCP22_SetReadyBit(ucPortIdx, (*(pucData + 1) == 0x01) ? TRUE : FALSE); // 1st byte: READY bit
            _Hal_HDCP22_SetReAuthBit(ucPortIdx, (*(pucData + 2) == 0x01) ? TRUE : FALSE); // 2nd byte: re-auth bit
        }
    }
    else
    {
        if (*(MS_U8*)(pucData) == 0xC) //RepeaterAuth_Send_ReceiverID_List
        {
            *ubRecIDListSetDone = TRUE;
        }

        W2BYTEMSK(REG_COMBO_GP_TOP_40_L, BIT(2)| BIT(0), BMASK(2:0)); //enable XIU write

        for(ultemp = 0; ultemp < dwDataLen; ultemp++)
        {
            W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp +TMDS_HDCP2_SOURCE_READ_OFFSET, 0x7FF); //set address
            W2BYTEMSK(REG_HDCPKEY_05_L + ucRegOffset, *(pucData + ultemp), BMASK(7:0));
            W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp +TMDS_HDCP2_SOURCE_READ_OFFSET, 0x7FF); //set address

            while((R2BYTE(REG_HDCPKEY_0E_L + ucPortIdx * 0x02) &BMASK(7:0)) != *(pucData + ultemp))
            {
                //Wilson: retry, software patch for XIU write fail issue;
                W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp +TMDS_HDCP2_SOURCE_READ_OFFSET, 0x7FF); //set address
                W2BYTEMSK(REG_HDCPKEY_05_L + ucRegOffset, *(pucData + ultemp), BMASK(7:0));
                W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, ultemp +TMDS_HDCP2_SOURCE_READ_OFFSET, 0x7FF); //set address
            }
        }

        W2BYTEMSK(REG_HDCP_DUAL_P0_63_L + dwBKOffset, dwDataLen, 0x7FF); // write length

        //patch for CTS
        if(*(pucData) == 0x03) //AKE_Send_Cert
        {
            //MHAL_HDMIRX_MSG_INFO("[HDCP22] check DDC free!!\r\n");

            while((R2BYTE(REG_HDCP_DUAL_P0_19_L + dwBKOffset) & BIT(10))); //polling DDC free
        }

        W2BYTEMSK(REG_HDCP_DUAL_P0_64_L + dwBKOffset, BIT(15), BIT(15)); // address pulse
    }
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_GetEncryptionFlag()
//  [Description]
//                  Get input is encryption or not
//  [Arguments]:
//
//  [Return]:
//                 TRUE: encryption, FALSE: no encryption
//**************************************************************************
MS_BOOL Hal_HDCP_GetEncryptionFlag(E_MUX_INPUTPORT enInputPortType)
{
    MS_BOOL bEncryptionFlag = FALSE;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            if(R2BYTE(REG_HDCP_DUAL_P0_1B_L) &BIT(9))
            {
                bEncryptionFlag = TRUE;

                W2BYTE(REG_HDCP_DUAL_P0_1B_L, BIT(9));
            }

            break;

        case INPUT_PORT_DVI1:
            if(R2BYTE(REG_HDCP_DUAL_P1_1B_L) &BIT(9))
            {
                bEncryptionFlag = TRUE;

                W2BYTE(REG_HDCP_DUAL_P1_1B_L, BIT(9));
            }

            break;

        case INPUT_PORT_DVI2:
            if(R2BYTE(REG_HDCP_DUAL_P2_1B_L) &BIT(9))
            {
                bEncryptionFlag = TRUE;

                W2BYTE(REG_HDCP_DUAL_P2_1B_L, BIT(9));
            }

            break;

        case INPUT_PORT_DVI3:
            if(R2BYTE(REG_HDCP_DUAL_P3_1B_L) &BIT(9))
            {
                bEncryptionFlag = TRUE;

                W2BYTE(REG_HDCP_DUAL_P3_1B_L, BIT(9));
            }

            break;

        default:

            break;
    };

    return bEncryptionFlag;
}

//=========================================================//
// Function : Hal_HDMI_Func_Caps
// Description: report which patch function hdmi needs
//=========================================================//
MS_U16 Hal_HDMI_Func_Caps(void)
{
    return 0;
}

/******************************************************************************/
/// for HDCP
/******************************************************************************/
//=========================================================//
// Function : Hal_HDCP_clearflag
// Description: clear hdcp status, must during hpd low
//=========================================================//
void Hal_HDCP_clearflag(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x171200
        case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171800
        case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171B00
    }
    HDCP_W2BYTE(REG_HDCP_DUAL_P0_01_L+u16bank_offset, 0xFF00 );
}

//=========================================================//
// Function : Hal_HDCP_getstatus
// Description: get HDCP status
//=========================================================//
MS_U16 Hal_HDCP_getstatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16hdcp_status, u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x171200
        case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171800
        case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171B00
    }

    u16hdcp_status = HDCP_R2BYTE(REG_HDCP_DUAL_P0_01_L + u16bank_offset);

    return u16hdcp_status;
}

/******************************************************************************/
/// for HDMI
/******************************************************************************/
//=========================================================//
// Function : Hal_HDMI_pullhpd
// Description: Pull DVI Hot-Plug-Detection pin
//=========================================================//
void Hal_HDMI_pullhpd(MS_BOOL bHighLow, E_MUX_INPUTPORT enInputPortType, MS_BOOL bInverse, MS_U8 ucMHLSupportPath)
{
    if(!bInverse)
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0://A
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI0)
                {
                    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }
                else
                {
                    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                    W2BYTEMSK(REG_PM_MHL_CBUS_34, bHighLow? BIT(1): 0, BIT(1));
                }
                break;

            case INPUT_PORT_DVI1://B
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(5)|BIT(1) : 0, BIT(5)|BIT(1));
                break;

            case INPUT_PORT_DVI2://C
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(6)|BIT(2) : 0, BIT(6)|BIT(2));
                break;

            case INPUT_PORT_DVI3://D
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? BIT(7)|BIT(3) : 0, BIT(7)|BIT(3));
                break;

            default: // For system first power on with the unknow HDMI port.
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? LBMASK : 0, LBMASK);

                if(ucMHLSupportPath != E_INPUT_NOT_SUPPORT_MHL)
                {
                    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }

                break;
        }
    }
    else
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0://A
                if(ucMHLSupportPath == E_INPUT_SUPPORT_MHL_PORT_DVI0)
                {
                    //PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? 0 : BIT(4), BMASK(4:3)); // [4]: output val, [3]: oen
                }
                else
                {
                    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? 0: BIT(4), BMASK(4:3)); // [4]: output val, [3]: oen
                    W2BYTEMSK(REG_PM_MHL_CBUS_34, bHighLow? BIT(1): 0, BIT(1));
                }
                break;

            case INPUT_PORT_DVI1://B
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(5), BIT(5)|BIT(1));
                break;

            case INPUT_PORT_DVI2://C
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(6), BIT(6)|BIT(2));
                break;

            case INPUT_PORT_DVI3://D
                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BIT(7), BIT(7)|BIT(3));
                break;

            default: // For system first power on with the unknow HDMI port.

                PM_W2BYTE(REG_PM_SLEEP_27_L, bHighLow ? 0 : BMASK(7:4), LBMASK);
                if(ucMHLSupportPath != E_INPUT_NOT_SUPPORT_MHL)
                {
                    PM_W2BYTE(REG_PM_MHL_CBUS_20, bHighLow ? BIT(4)|BIT(3) : 0, BMASK(4:3)); // [4]: output val, [3]: oen
                }

                break;
        }
    }

    if(!bHighLow)
    {
        _Hal_tmds_ResetHDMI20VersionFlag(enInputPortType);
    }
}

//=========================================================//
// Function : Hal_HDCP_initproductionkey
// Description:
//=========================================================//
// Aaron comment:  HDCP OESS mode in U3
void Hal_HDCP_initproductionkey( MS_U8 * pu8HdcpKeyData )
{
    MS_U16 i, delay, u16bank_offset=0, u16loop;
    MS_U8 * pu8HdcpKeyData_bk;

    pu8HdcpKeyData_bk = pu8HdcpKeyData;
    MHAL_HDMIRX_MSG_INFO("[%s][%d]Start!\n", __FUNCTION__, __LINE__);
    // write BKSV/Bcaps into 4 HDCP engines
    for(u16loop=0; u16loop<4; u16loop++)
    {
        switch(u16loop)
        {
            default:
            case 0: u16bank_offset = 0x0000; break; // BK 0x171200
            case 1: u16bank_offset = 0x0300; break; // BK 0x171500
            case 2: u16bank_offset = 0x0600; break; // BK 0x171800
            case 3: u16bank_offset = 0x0900; break; // BK 0x171B00
        }

        pu8HdcpKeyData = pu8HdcpKeyData_bk;
        // Bksv
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, BIT(10), BIT(10)); // [10]: HDCP enable for DDC
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(15), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM

        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, 0x00, BMASK(9:0)); // address
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(5), BIT(5)); // trigger latch address
        for ( i = 0; i < 5; i++ )
        {
            HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_18_L+u16bank_offset, *pu8HdcpKeyData, BMASK(7:0)); // data
            HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(4), BIT(4)); // trigger latch data

            delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
            pu8HdcpKeyData += 1;
            while(R2BYTE(REG_HDCP_DUAL_P0_19_L+u16bank_offset) & BIT(7)); // wait write ready
        }

        // Bcaps = 0x80
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, 0x40, BMASK(9:0)); // address
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(5), BIT(5)); // trigger latch address

        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_18_L+u16bank_offset, 0x80, BMASK(7:0)); // data
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(4), BIT(4)); // trigger latch data
        delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
        while(R2BYTE(REG_HDCP_DUAL_P0_19_L+u16bank_offset) & BIT(7)); // wait write ready
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM

        // [10:8]: 3'b111 determine Encrp_En during Vblank in DVI mode; [5]:HDCP enable; [0]: EESS mode deglitch Vsync mode
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_00_L+u16bank_offset, (BIT(10)|BIT(9)|BIT(8)|BIT(5)|BIT(0)), (BIT(10)|BIT(9)|BIT(8)|BIT(5)|BIT(0)));
    }
#if 0  // this is old version, it can work now, but prefer using new version, so we can use new sersion after einstion 2.0 code stable
    // HDCP key
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L, BIT(15)|BIT(14), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM
    // burst write from address 0x00
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L, 0x00, BMASK(9:0)); // address
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L, BIT(5), BIT(5)); // trigger latch address

    for ( i = 0; i < 284; i++ )
    {
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_18_L, *pu8HdcpKeyData, BMASK(7:0)); // data
        HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L, BIT(4), BIT(4)); // trigger latch data
        delay=PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
        pu8HdcpKeyData += 1;
        while(R2BYTE(REG_HDCP_DUAL_P0_19_L) & BIT(7)); // wait write ready
    }

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L, 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM
#endif
  // this is new version, please use this after einstein 2.0 code stable

    HDCP_W2BYTEMSK(REG_HDCPKEY_02_L, BIT(8), BIT(8));

    // HDCP key
    HDCP_W2BYTEMSK(REG_COMBO_GP_TOP_40_L, BIT(3)|BIT(2)|BIT(0), BIT(3)|BIT(2)|BIT(0)); // [2]: CPU write enable, [3]: 0: 74 RAM, 1 :HDCP RAM
    // burst write from address 0x05
    HDCP_W2BYTEMSK(REG_HDCPKEY_00_L, 0x05, BMASK(9:0)); // address

    for( i = 0; i < 284; i++ )
    {
        HDCP_W2BYTEMSK(REG_HDCPKEY_01_L, *pu8HdcpKeyData, BMASK(7:0)); // data
        pu8HdcpKeyData += 1;
    }
    HDCP_W2BYTEMSK(REG_COMBO_GP_TOP_40_L, 0, BIT(3)|BIT(2)|BIT(0)); // [2]: CPU write enable, [3]: 0: 74 RAM, 1 :HDCP RAM

    // to resolve the coverity issue
    u16loop = delay;
    MHAL_HDMIRX_MSG_INFO("[%s][%d]End!\n", __FUNCTION__, __LINE__);
}

//=========================================================//
// Function : Hal_HDCP_ddc_en
// Description:
//=========================================================//
void Hal_HDCP_ddc_en(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000; break; // BK 0x171200
        case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171800
        case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171B00
    }

    W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, (bEnable ? BIT(10):0), BIT(10)); // HDCP enable for ddc
}

//=========================================================//
// Function : Hal_HDCP_Vsync_end_en
// Description:
//=========================================================//
// Aaron comment: doesn't need in U3
void Hal_HDCP_Vsync_end_en( MS_BOOL bEnalbe )
{
     //do nothing in U3
}

//=========================================================//
// Function : Hal_MHL_TMDS_pkt_reset
// Description: reset the MHL packet status
//=========================================================//
void Hal_MHL_TMDS_pkt_reset(E_MUX_INPUTPORT enInputPortType, HDMI_REST_t breset)
{
    // reset off-line AVMUTE status

    if(breset & REST_AVMUTE)
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0:
                W2BYTEMSK(REG_HDMI2_11_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_HDMI2_11_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI1:
                W2BYTEMSK(REG_HDMI2_11_L, BIT(8), BIT(8));
                W2BYTEMSK(REG_HDMI2_11_L, 0, BIT(8));
                break;
            case INPUT_PORT_DVI3:
                W2BYTEMSK(REG_HDMI2_12_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_HDMI2_12_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI2:
                W2BYTEMSK(REG_HDMI2_12_L, BIT(8), BIT(8));
                W2BYTEMSK(REG_HDMI2_12_L, 0, BIT(8));
                break;
            default:
                break;
        }
    }

    W2BYTEMSK(REG_MHL_TMDS_5F_L, breset, HBMASK);
    MsOS_DelayTask(1);
    W2BYTEMSK(REG_MHL_TMDS_5F_L, 0x0000, HBMASK);
}

//=========================================================//
// Function : Hal_HDMI_hw_reset
// Description: reset the HDMI packet status
//=========================================================//
void Hal_HDMI_pkt_reset(E_MUX_INPUTPORT enInputPortType, HDMI_REST_t breset)
{
    // reset off-line AVMUTE status
    if(breset & REST_AVMUTE)
    {
        switch(enInputPortType)
        {
            case INPUT_PORT_DVI0:
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2D_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2D_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI1:
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2D_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2D_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI2:
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2D_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2D_L, 0, BIT(0));
                break;
            case INPUT_PORT_DVI3:
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2D_L, BIT(0), BIT(0));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2D_L, 0, BIT(0));
                break;
            default:
                break;
        }
    }

    W2BYTEMSK(REG_HDMI_DUAL_0_5F_L, breset, HBMASK);
    MsOS_DelayTask(1);
    W2BYTEMSK(REG_HDMI_DUAL_0_5F_L, 0x0000, HBMASK);
}

void Hal_HDMI_exit(E_MUX_INPUTPORT enInputPortType)
{
    // Disable all DDC
    W2BYTEMSK(REG_DDC_4A_L, 0 , 0x80);
    W2BYTEMSK(REG_DDC_4B_L, 0 , 0x8080);
    W2BYTEMSK(REG_DDC_4C_L, 0 , 0x80);

    // Disable audio
    Hal_HDMI_audio_output(DISABLE);

    // HPD low
    Hal_HDMI_hpd_en(DISABLE);

    // Disable HDCP
    Hal_HDCP_ddc_en(enInputPortType, DISABLE);

}

//=========================================================//
// Function : Hal_HDMI_init
// Description:
//=========================================================//
void Hal_HDMI_init(MS_BOOL bImmeswitchSupport, MS_U8 ucMHLSupportPath)// void )
{
    E_MUX_INPUTPORT port_index;
    MS_BOOL bEQCalibrateFlag = _Hal_tmds_GetEQCalibrateFlag();
    MS_U16 u16bank_offset = 0;

    // DVI setting
    MHAL_HDMIRX_MSG_INFO(">>[%s][%d]HDMI Immeswitch support flag = %x\n", __FUNCTION__, __LINE__, bImmeswitchSupport);

    if(bImmeswitchSupport)
    {
        // RX clock enable
        MDrv_WriteByteMask(REG_COMBO_GP_TOP_07_L, 0x00, 0xFF); // misc clock enable
        MDrv_WriteByteMask(REG_COMBO_GP_TOP_11_L, 0x00, 0xFF); // P0 dec_hdcp clock enable
        MDrv_WriteByteMask(REG_COMBO_GP_TOP_19_L, 0x00, 0xFF); // P1 dec_hdcp clock enable
        MDrv_WriteByteMask(REG_COMBO_GP_TOP_21_L, 0x00, 0xFF); // P2 dec_hdcp clock enable
        MDrv_WriteByteMask(REG_COMBO_GP_TOP_29_L, 0x00, 0xFF); // P3 dec_hdcp clock enable

        for(port_index = INPUT_PORT_DVI0; port_index <= INPUT_PORT_DVI_END; port_index++)
        {
            switch(port_index)
            {
                default:
                case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x170200
                case INPUT_PORT_DVI1:  u16bank_offset = 0x0200;  break; // BK 0x170400
                case INPUT_PORT_DVI2:  u16bank_offset = 0x0400;  break; // BK 0x170600
                case INPUT_PORT_DVI3:  u16bank_offset = 0x0600;  break; // BK 0x170800
            }

            // Enable RX Phy clk gen
            W2BYTE(REG_COMBO_PHY0_P0_0B_L + u16bank_offset, 0xFFFF); // Clock enable
            W2BYTE(REG_COMBO_PHY0_P0_0C_L + u16bank_offset, 0x3FFF);

            // LDO
            W2BYTE(REG_COMBO_PHY1_P0_00_L + u16bank_offset, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc

            // Pd
            W2BYTE(REG_COMBO_PHY0_P0_5B_L + u16bank_offset, 0x0000);

            // PM Pd
            if(port_index == INPUT_PORT_DVI0)
            {
                PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0x0001); //  [0]: port0 reg_pd_clkin
                PM_W2BYTE(REG_PM_SLEEP_4C_L, 0x0000, 0x000F); //  [3:0]: port0 reg_pd_rt
            }
            else if(port_index == INPUT_PORT_DVI1)
            {
                PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0x0002); //  [1]: port1 reg_pd_clkin
                PM_W2BYTE(REG_PM_SLEEP_4C_L, 0x0000, 0x00F0); //  [7:4]: port1 reg_pd_rt
            }
            else if(port_index == INPUT_PORT_DVI2)
            {
                PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0x0004); //  [2]: port2 reg_pd_clkin
                PM_W2BYTE(REG_PM_SLEEP_4C_L, 0x0000, 0x0F00); //  [11:8]: port2 reg_pd_rt
            }
            else if(port_index == INPUT_PORT_DVI3)
            {
                PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0x0008); //  [3]: port3 reg_pd_clkin
                PM_W2BYTE(REG_PM_SLEEP_4C_L, 0x0000, 0xF000); //  [15:12]: port3 reg_pd_rt
            }

            switch(port_index)
            {
                default:
                case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x171000
                case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171300
                case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171600
                case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171900
            }

            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_0B_L + u16bank_offset, BIT(1), BMASK(1:0)); // [1:0]: DVI select channel2(R) as small DE
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_19_L + u16bank_offset, BIT(5), BMASK(5:4)); // [5:4]: DVI select channel2(R) as align DE
        }

        // IMMESWITCH enhance setting
      //  W2BYTEMSK(REG_HDMI2_20_L, BIT(8), BIT(8)); // [8]: 0: disable 1:enable (mask ISLAND_EN while power saving mode)

    }

    else
    {
    //    #ifdef DVI_OFFLINE_CLK_DETECTION
        PM_W2BYTE(REG_PM_SLEEP_4A_L, 0x0000, 0x000F); // power on DVI off-line detection clock
        PM_W2BYTE(REG_PM_SLEEP_4C_L, 0x0000, 0xFFFF); // DVI clock power on
    //    #endif

        for(port_index = INPUT_PORT_DVI0; port_index <= INPUT_PORT_DVI_END; port_index++)
        {
            switch(port_index)
            {
                default:
                case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x170200
                case INPUT_PORT_DVI1:  u16bank_offset = 0x0200;  break; // BK 0x170400
                case INPUT_PORT_DVI2:  u16bank_offset = 0x0400;  break; // BK 0x170600
                case INPUT_PORT_DVI3:  u16bank_offset = 0x0600;  break; // BK 0x170800
            }

            // Enable RX Phy clk gen
            W2BYTE(REG_COMBO_PHY0_P0_0B_L + u16bank_offset, 0x0001); // enable [0]: main link clock
            W2BYTE(REG_COMBO_PHY0_P0_0C_L + u16bank_offset, 0x1008);// enable [19]: lock detect, [28]: XTAL clock

            // Pd
            W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L + u16bank_offset, 0x0000, 0x1000); // [12]: power on clock amplifier
        }
    }

    MDrv_WriteByteMask(REG_COMBO_GP_TOP_12_L, 0x00, 0xFF); // PIP0 dc_depack clock enable
    MDrv_WriteByteMask(REG_COMBO_GP_TOP_1A_L, 0x00, 0xFF); // PIP1 dc_depack clock enable

    W2BYTE(REG_HDMI_DUAL_0_07_L, 0x0001); // PIP0 enable deep color

    for(port_index = INPUT_PORT_DVI0; port_index <= INPUT_PORT_DVI_END; port_index++)
    {
        switch(port_index)
        {
            default:
            case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x170200
            case INPUT_PORT_DVI1:  u16bank_offset = 0x0200;  break; // BK 0x170400
            case INPUT_PORT_DVI2:  u16bank_offset = 0x0400;  break; // BK 0x170600
            case INPUT_PORT_DVI3:  u16bank_offset = 0x0600;  break; // BK 0x170800
        }

        // phy to mac setting
        W2BYTEMSK(REG_COMBO_PHY0_P0_6B_L + u16bank_offset, 0x0706, BMASK(11:0));// [2]: enable clko_pix 2x; [1]: reg_atop_en_clko_pix=1
        W2BYTE(REG_COMBO_PHY0_P0_5A_L + u16bank_offset, 0x0004);// [2]: enable reg_atop_en_clko_tmds2x
        W2BYTE(REG_COMBO_PHY0_P0_0A_L + u16bank_offset, 0x0040);// [6]: reg_af_ls_20out_sel=1

        // Initial Swap
        W2BYTE(REG_COMBO_PHY0_P0_00_L + u16bank_offset, 0x0080);

        // Full Rate Mode
        W2BYTE(REG_COMBO_PHY0_P0_57_L + u16bank_offset, 0x010C);

        // Other ATOP initialize
        W2BYTE(REG_COMBO_PHY0_P0_7A_L + u16bank_offset, 0x0000);
        W2BYTE(REG_COMBO_PHY0_P0_34_L + u16bank_offset, 0x0040);

        // If HDMI2 ACDR mode
        W2BYTE(REG_COMBO_PHY0_P0_35_L + u16bank_offset, 0x0480); // [10]: reg_hdmi2_acdr_mode = 1

        // Clock Detect
        W2BYTE(REG_COMBO_PHY0_P0_48_L + u16bank_offset, 0x0702);// [15:8]: reg_mainlink_crlose_thr; [7:0]: reg_mainlink_crlock_th
        W2BYTE(REG_COMBO_PHY0_P0_3B_L + u16bank_offset, 0x1F00);// [12:0]: reg_clk_valid_u
        // phy0_45[7] frequency lock: PLL lock (DIG lock) changes to raw clock lock
        if(MDrv_ReadByte(REG_CHIP_REVISION) < 1) // U01 : raw clock lock
        {
            W2BYTE(REG_COMBO_PHY0_P0_45_L + u16bank_offset, 0x1F03);// [14:8]: reg_dig_lock_time; [7]: reg_dig_lock_mode; [3:0]: reg_dig_lck_cnt_power
        }
        else // > U02 : PLL lock (DIG lock)
        {
            W2BYTE(REG_COMBO_PHY0_P0_45_L + u16bank_offset, 0x1F83);// [14:8]: reg_dig_lock_time; [7]: reg_dig_lock_mode; [3:0]: reg_dig_lck_cnt_power
        }

        // cr_lock_cnt
        W2BYTE(REG_COMBO_PHY0_P0_49_L + u16bank_offset, 0x0A6B); // [15:0]: reg_hdmi_clk_thr4
        W2BYTE(REG_COMBO_PHY0_P0_4A_L + u16bank_offset, 0x0640); // [15:0]: reg_hdmi_clk_thr3
        W2BYTE(REG_COMBO_PHY0_P0_4B_L + u16bank_offset, 0x0320); // [15:0]: reg_hdmi_clk_thr2
        W2BYTE(REG_COMBO_PHY0_P0_4C_L + u16bank_offset, 0x01C5); // [15:0]: reg_hdmi_clk_thr1
        W2BYTE(REG_COMBO_PHY0_P0_4D_L + u16bank_offset, 0x038B); //[15:0]: reg_mhlpp_clk_thr2 (85Mhz)
        W2BYTE(REG_COMBO_PHY0_P0_4E_L + u16bank_offset, 0x0190); //[15:0]: reg_mhlpp_clk_thr1
        W2BYTE(REG_COMBO_PHY0_P0_4F_L + u16bank_offset, 0x0215); //[15:0]: reg_mhl24_clk_thr1

        // EQ setting initialize
        W2BYTE(REG_COMBO_PHY0_P0_25_L + u16bank_offset, 0x0AC8);
        W2BYTE(REG_COMBO_PHY0_P0_2B_L + u16bank_offset, 0x0018); // [15:0]: reg_eq_under_over_check_duration
        W2BYTE(REG_COMBO_PHY0_P0_2C_L + u16bank_offset, 0x0004); // [15:0]: reg_eq_update_duration
        W2BYTE(REG_COMBO_PHY0_P0_28_L + u16bank_offset, 0x3A83);
        W2BYTE(REG_COMBO_PHY0_P0_26_L + u16bank_offset, 0x0A0A); // [13:8]: reg_eq_fine_tune_time; [5:0]: reg_phase_check_time

        // PHASE
        //W2BYTE(REG_COMBO_PHY0_P0_11_L + u16bank_offset, 0x0000); // [12]: reg_dvi_ovch0; [11:0]: reg_dvi_ch0_ph
        //W2BYTE(REG_COMBO_PHY0_P0_12_L + u16bank_offset, 0x0000); // [12]: reg_dvi_ovch1; [11:0]: reg_dvi_ch1_ph
        //W2BYTE(REG_COMBO_PHY0_P0_13_L + u16bank_offset, 0x0000); // [12]: reg_dvi_ovch2; [11:0]: reg_dvi_ch2_ph
        W2BYTE(REG_COMBO_PHY0_P0_1D_L + u16bank_offset, 0x0100); // dvi phase update threshold for accumulator, [7]enable, [6:0]value
        W2BYTE(REG_COMBO_PHY0_P0_1A_L + u16bank_offset, 0x009A);
        W2BYTE(REG_COMBO_PHY0_P0_1B_L + u16bank_offset, 0x009A);
        W2BYTE(REG_COMBO_PHY0_P0_1C_L + u16bank_offset, 0x009A);

        // ATOP mode
        W2BYTE(REG_COMBO_PHY0_P0_5C_L + u16bank_offset, 0x0000); // [3:0]: reg_atop_sel_phdclk
        W2BYTE(REG_COMBO_PHY0_P0_10_L + u16bank_offset, 0x0008); // [4]: reg_swap_dcdr_updn_polity; [3]: reg_enable_phase_step

        // HDMI2 range 170~85Mhz
        W2BYTE(REG_COMBO_PHY1_P0_09_L + u16bank_offset, 0x0715); // 170Mhz

        // HDMI EQ sweep time high bit for auto detect hdmi2
        W2BYTE(REG_COMBO_PHY1_P0_0C_L + u16bank_offset, 0x0001); // [3:0]: reg_hdmi_eq_sweep_times_h

        W2BYTEMSK(REG_COMBO_PHY0_P0_59_L + u16bank_offset, BIT(1), BIT(1)); // [1]:atop_en_pd_phdac_ove=1
        W2BYTE(REG_COMBO_PHY1_P0_13_L + u16bank_offset, 0x0010); // [3:0]:reg_atop_pd_phdac_q_ov=0, [4]:reg_en_pd_phdac_q_ove=1

        W2BYTE(REG_COMBO_PHY0_P0_58_L + u16bank_offset, 0x0001);
        W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L + u16bank_offset, 0x0F00, 0x0F00);

        // HDMI Auto Detect
        W2BYTE(REG_COMBO_PHY1_P0_30_L + u16bank_offset, 0x0026);
        W2BYTE(REG_COMBO_PHY1_P0_38_L + u16bank_offset, 0x00FF); // [15:0]: reg_hdmi_symbol_det_times
        W2BYTE(REG_COMBO_PHY1_P0_37_L + u16bank_offset, 0x001F); // [7:4]: reg_hdmi_auto_det_times; [3:0]: reg_hdmi_eq_sweep_times
        W2BYTE(REG_COMBO_PHY1_P0_33_L + u16bank_offset, 0x001F); //[ 15:0]: reg_hdmi_symbol_det_duration

        // EQ setting
        W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L + u16bank_offset, 0x00F0, 0x00F0);
        W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, 0x1F1F);

        if(_Hal_tmds_CheckInputIsMHL(port_index, ucMHLSupportPath))
        {
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_MHL_EQ_SETTING_VALUE, 0x1F1F);
        }
        else
        {
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, (HDMI_EQ_14_SETTING_VALUE << 8)| HDMI_EQ_14_SETTING_VALUE, 0x1F1F);
        }

        W2BYTEMSK(REG_COMBO_PHY1_P0_3E_L + u16bank_offset, BIT(2), BIT(2)); // [2]: SCDC bypass clock tmds ratio
        W2BYTEMSK(REG_COMBO_PHY0_P0_45_L + u16bank_offset, TMDS_DIGITAL_LOCK_CNT_POWER, BMASK(3:0)); // [3:0]: reg_dig_lck_cnt_power
        W2BYTEMSK(REG_COMBO_PHY0_P0_47_L + u16bank_offset, (TMDS_DIGITAL_LOSE_RANGE << 4), BMASK(9:4)); // [9:4]: reg_dig_lose_range

        if(bEQCalibrateFlag)
        {
            _Hal_tmds_EQCalibrationProc(port_index);
        }

        _Hal_tmds_ClockStatusInitial(port_index, ucMHLSupportPath);
        _Hal_tmds_AutoEQInitialSetting(port_index);
        _Hal_tmds_HDMI20AutoEQSetting(port_index, FALSE);
        _Hal_tmds_ClearSCDCStatusFlag(port_index, TMDS_SCDC_ACCESS_ADDERSS_A8);
        _Hal_tmds_HDMI20PHYSetting(port_index, TRUE); // Always set 2P mode
    }

    for(port_index = INPUT_PORT_DVI0; port_index <= INPUT_PORT_DVI_END; port_index++)
    {
        switch(port_index)
        {
            default:
            case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x171000
            case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171300
            case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171600
            case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171900
        }

        W2BYTE(REG_DVI_DTOP_DUAL_P0_1F_L + u16bank_offset, 0x071F); // [10:8]: reg_ck_unstable_num; [7:0]: reg_ck_stable

        //set TIMER V Divider for div report
        if(MST_XTAL_CLOCK_HZ==12000000UL)
        {
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_1E_L + u16bank_offset, BIT(3), BMASK(3:0)); // timer baased on Xtal = 12Mhz
        }

        // Enable DVI phase accumulator extension and clock stable gate
        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_04_L + u16bank_offset, 0, BIT(1)|BIT(0)); // [1]: Enable of acc8lsb extension ; [0]: Enable of acc8lsb extension select
        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_21_L + u16bank_offset, 0, BIT(1)); // [1]: enable of clk stable gate

        // Enable DVI clock tolerance: 4*0.7 = 2.8 MHz
        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_13_L + u16bank_offset, (0x2<<12)|0xC80,  BMASK(14:0)); //the comp value is reg_comp_tol * 4
        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_12_L + u16bank_offset, (0xF<<12),  BMASK(15:12)); //0x110A25=0xB0, DVI clock detection counter tolerance set to 7
        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_19_L + u16bank_offset, BIT(3), BIT(3)); // reg_dbg_tst

        // For MHL3 PHY setting always on
        W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_60_L + u16bank_offset, BIT(9), BIT(9));

        W2BYTEMSK(REG_HDCP_DUAL_P0_17_L + u16bank_offset, BIT(13), BIT(13)); // [13]: when detect no input, rest HDCP FSM to Idle state
    }
#if 0

    //by pass phace dac spread control
    W2BYTEMSK( REG_DVI_DTOP_23_L, BIT(3), BIT(3));
    W2BYTEMSK( REG_DVI_DTOP1_23_L, BIT(3), BIT(3));
    W2BYTEMSK( REG_DVI_DTOP2_23_L, BIT(3), BIT(3));
    W2BYTEMSK( REG_DVI_DTOP3_23_L, BIT(3), BIT(3));
#endif

    // GPIO disable
    W2BYTEMSK(REG_PM_MHL_CBUS_21, 0, BIT(11)); // reg_cbus_gpio_en = 0

    // Always set pull down 100K
    W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(7), BIT(7));

    //Hal_HDCP_ddc_en(ENABLE);  // HDCP enable for ddc
    // Enable DVI function: REG_COMBO_GP_TOP_10_L[12] should be always enable if one of HDMI ports is used.
    W2BYTEMSK(REG_COMBO_GP_TOP_10_L, BIT(12), BIT(12)); // [12]: enable DVI function (P0)
    W2BYTEMSK(REG_COMBO_GP_TOP_18_L, BIT(12), BIT(12)); // [12]: enable DVI function (P1)
    W2BYTEMSK(REG_COMBO_GP_TOP_20_L, BIT(12), BIT(12)); // [12]: enable DVI function (P2)
    W2BYTEMSK(REG_COMBO_GP_TOP_28_L, BIT(12), BIT(12)); // [12]: enable DVI function (P3)


    // [15]: output black level, [14]: DC info, [13]: Audio flat bit check, [11:10]: Blanking when AVMUTE, [9]: HDMI/DVI mode HW detect
    W2BYTEMSK(REG_HDMI_DUAL_0_06_L, (0x6E<<8), HBMASK);
    // [1:0]: enable HDMI deep color mode
    W2BYTEMSK(REG_HDMI_DUAL_0_07_L, 0x03, LBMASK);
    // [2]: Deep color FIFO type 1
    W2BYTEMSK(REG_HDMI_DUAL_0_08_L, BIT(2), BIT(2));
    // [7]: Frame repetition manual mode, [1]: auto non-PCM detection
    W2BYTEMSK(REG_HDMI_DUAL_0_64_L, BIT(1)|BIT(7), LBMASK);

    // turn off all the interrupt
    W2BYTE(REG_HDMI_DUAL_0_63_L, 0xFFFF); // HDMI interrupt clear
    W2BYTE(REG_HDMI_DUAL_0_63_L, 0);
    W2BYTE(REG_HDMI_DUAL_0_60_L, 0xFFFF);  //REG_HDMI_INT_MASK

    W2BYTEMSK(REG_HDMI2_DUAL_0_27_L, BMASK(15:8), BMASK(15:8)); // HDMI2 interrupt clear
    W2BYTEMSK(REG_HDMI2_DUAL_0_27_L, 0, BMASK(15:8));
    W2BYTEMSK(REG_HDMI2_DUAL_0_26_L, BMASK(15:8), BMASK(15:8)); // HDMI2 interrupt mask

    //W2BYTEMSK(REG_CHIP_GPIO1_10_L, BIT(3), BIT(3)); // PAD_ARC MODE
    W2BYTEMSK(REG_HDMI_DUAL_0_67_L, BIT(8), BIT(8)); // reg_cts_div2_en
    W2BYTEMSK(REG_COMBO_GP_TOP_44_L, BIT(7)|BIT(0), BIT(7)|BIT(0)); // [7] reg_en_cts_n_synth : enable CTS/N synth;  [0] reg_synth_sel_cts_ref: CTS[19:0]
    W2BYTEMSK(REG_COMBO_GP_TOP_08_L, BIT(2), BIT(2)); // reg_clk_for_ctsn_div2_en

    // [1] Enable auto-clear SKE status when receiving ake_init; [2] Enable auto-clear SKE status when no hdcp22 capability
    //W2BYTEMSK(REG_HDCP_DUAL_P0_4E_L, BIT(2)|BIT(1), BIT(2)|BIT(1));
    //W2BYTEMSK(REG_HDCP_DUAL_P1_4E_L, BIT(2)|BIT(1), BIT(2)|BIT(1));
    //W2BYTEMSK(REG_HDCP_DUAL_P2_4E_L, BIT(2)|BIT(1), BIT(2)|BIT(1));
    //W2BYTEMSK(REG_HDCP_DUAL_P3_4E_L, BIT(2)|BIT(1), BIT(2)|BIT(1));
    //-----------------------------------------------------------------------------------------------//
#if 0

    //DVIPLL auto protection setting
    //PORTA/C
    W2BYTEMSK(REG_DVI_ATOP_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.15v, 0x2 = 1.2v
    //PORTB
    W2BYTEMSK(REG_DVI_ATOP1_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.15v, 0x2 = 1.2v
    //PORTD
    W2BYTEMSK(REG_DVI_ATOP2_65_L, 0,  BMASK(13:12));//Voltage threshold setting: 0x0 = 1.1v, 0x1 = 1.5v, 0x2 = 1.2v

    // enable deep color clock 2x
    W2BYTEMSK( REG_HDMI2_03_L, BMASK(7:4), BMASK(7:4));

    // pixel clock from atop pixel clock and HDMI 3D settings
    W2BYTEMSK( REG_HDMI2_20_L, BMASK(7:6)|BIT(4)|BIT(0), BMASK(7:6)|BIT(4)|BIT(0)); // [7]: 0,TMDS 1,pixel clock ; [6] : 0,deep color 1,reg, [4]: enable AVG scale down FIFO, [0]: Bypass 422 to 444



    // Setting pixel clock comparator for 3D timing which is large than 166MHz
    W2BYTE(REG_HDMI2_33_L, 0x0215); // 50MHz count value
    W2BYTE(REG_HDMI2_34_L, 0x042A); // 100MHz count value
    W2BYTE(REG_HDMI2_35_L, 0x06EA); // 166MHz count value

    // Word boundary setting
    W2BYTEMSK( REG_DVI_DTOP_23_L, BMASK(15:8), HBMASK);
    W2BYTEMSK( REG_DVI_DTOP1_23_L, BMASK(15:8), HBMASK);
    W2BYTEMSK( REG_DVI_DTOP2_23_L, BMASK(15:8), HBMASK);
    W2BYTEMSK( REG_DVI_DTOP3_23_L, BMASK(15:8), HBMASK);

    // Disable data Rterm HW control
    W2BYTEMSK(REG_DVI_ATOP_7F_L, 0, BMASK(15:14));

    for(i=0;i<4;i++)
    {
        switch(i)
        {
            case 0: u16bank_offset = 0x0000; break; // BK 0x110A00
            case 1: u16bank_offset = 0x2900; break; // BK 0x113300
            case 2: u16bank_offset = 0x2B00; break; // BK 0x113500
            case 3: u16bank_offset = 0x2D00; break; // BK 0x113700
        }

        W2BYTEMSK( REG_DVI_DTOP_32_L+u16bank_offset, 0xF000, 0xF000); // DE change tolerance
        //EQ setting
        W2BYTEMSK( REG_DVI_DTOP_20_L+u16bank_offset, 0x0CCF, 0x0FFF); // B_CH = 0xF, G and R_CH = 0xC

        //R-term value setting
        W2BYTEMSK( REG_DVI_DTOP_0C_L+u16bank_offset, 0x0200, 0x021F); // [9]:overwrite, [4:0]:R-tern value = 0x0

        //compvalue setting
        W2BYTEMSK( REG_DVI_DTOP_0F_L+u16bank_offset, 0x1215, 0xFFFF); //50m
        W2BYTEMSK( REG_DVI_DTOP_10_L+u16bank_offset, 0x042A, 0xFFFF); //100m
        W2BYTEMSK( REG_DVI_DTOP_11_L+u16bank_offset, 0x0855, 0xFFFF); //200m

        if (pXCResourcePrivate->stdrvXC_MVideo_Context.g_bDVI_AUTO_EQ)
        {
            MDrv_WriteByteMask(REG_DVI_DTOP_00_L+u16bank_offset, BIT(4), BIT(4)); // enable EQ new mode
            MDrv_WriteByte(REG_DVI_EQ_10_L+u16bank_offset, 0x80); // [7]: angle overwirte mode, [1]:disable manual EQ mode
            MDrv_WriteByte(REG_DVI_EQ_10_H+u16bank_offset, 0xA0); // [7]: check good phase and error rate, [5:4]: 40 phases
            MDrv_WriteByteMask(REG_DVI_EQ_12_H+u16bank_offset, 0x30, 0xF0); // [7:4]: Continuous good phases threshold = 3
            MDrv_WriteByte(REG_DVI_EQ_17_L+u16bank_offset, 0x2F); // [7:6]: 64ms timeout, [5]: Time out enable, [3]: waive enable, [2]: 2nd eye, [1]: unused DE, [0]: DE de-glitch
            MDrv_WriteByte(REG_DVI_EQ_17_H+u16bank_offset, 0x3F); // de-glitch length = 0x3F(pixels)
            //MDrv_WriteByteMask(REG_DVI_DTOP_30_H, BIT(4), BIT(4)); // Auto EQ trigger only > 100MHz
            MDrv_WriteByteMask(REG_DVI_EQ_02_H+u16bank_offset, 0x10, 0x10); // make EQ phase smoothly when pixel rate is under 100MHz
            MDrv_WriteByte(REG_DVI_DTOP_2F_L+u16bank_offset, 0x89); // enable auto EQ trigger
            }
        }
#endif

    _Hal_tmds_HDMI20MuxSetting0(TRUE); // Always set 2P mode

    W2BYTEMSK(REG_HDMI2_DUAL_0_20_L, 0, BIT(0)); // Disable bypass

#if(TMDS_POWER_SWITCH_IRQ_ENABLE)
    MsOS_AttachInterrupt(E_INT_IRQ_PMSLEEP, _Hal_tmds_PowerControlCallBackFunc);
    MsOS_EnableInterrupt(E_INT_IRQ_PMSLEEP);
#endif
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI DE
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 Hal_HDMI_GetHDEInfo(MS_U8 ucHDMIInfoSource)
{
    return Hal_HDMI_GetDataInfo(E_HDMI_GET_HDE, ucHDMIInfoSource);
}

//-------------------------------------------------------------------------------------------------
/// Get the HDMI HDE, VDE, HTT, VTT
/// @return @ref MS_U16
//-------------------------------------------------------------------------------------------------
MS_U16 Hal_HDMI_GetDataInfo(E_HDMI_GET_DATA_INFO enInfo, MS_U8 ucHDMIInfoSource)
{
    MS_U16 usHDMIDataInfo = 0;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    switch(enInfo)
    {
        case E_HDMI_GET_HDE:
            usHDMIDataInfo = R2BYTEMSK(REG_HDMI_DUAL_0_65_L, 0x3FFF);
            break;
        case E_HDMI_GET_VDE:
            usHDMIDataInfo = R2BYTEMSK(REG_HDMI2_DUAL_0_31_L, 0x3FFF);
            break;
        case E_HDMI_GET_HTT:
            usHDMIDataInfo = R2BYTEMSK(REG_HDMI2_DUAL_0_32_L, 0x3FFF);
            break;
        case E_HDMI_GET_VTT:
            usHDMIDataInfo = R2BYTEMSK(REG_HDMI2_DUAL_0_30_L, 0x3FFF);
            break;

        default:

            break;
    };

    return usHDMIDataInfo;
}

//=========================================================//
// Function : Hal_HDMI_set_EQ
// Description:
//=========================================================//
void Hal_HDMI_Set_EQ(E_MUX_INPUTPORT enInputPortType, MS_HDMI_EQ enEq, MS_U8 u8EQValue)
{
    MS_U16 u16bank_offset;
    MS_U16 temp;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x170200
        case INPUT_PORT_DVI1:  u16bank_offset = 0x0200;  break; // BK 0x170400
        case INPUT_PORT_DVI2:  u16bank_offset = 0x0400;  break; // BK 0x170600
        case INPUT_PORT_DVI3:  u16bank_offset = 0x0600;  break; // BK 0x170800
    }

    temp = u8EQValue & 0x1F;

    W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L + u16bank_offset, 0x00F0, 0x00F0);

    switch (enEq)
    {
        case MS_HDMI_EQ_2M:
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, 0x0505, 0x1F1F);
            W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, 0x0505, 0x1F1F);
            break;
        case MS_HDMI_EQ_10M:
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, 0x0A0A, 0x1F1F);
            W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, 0x0A0A, 0x1F1F);
            break;
        case MS_HDMI_EQ_15M:
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, 0x0C0C, 0x1F1F);
            W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, 0x0C0C, 0x1F1F);
            break;
        case MS_HDMI_EQ_MAX:
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, 0x0F0F, 0x1F1F);
            W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, 0x0F0F, 0x1F1F);
            break;
        case MS_HDMI_EQ_USER_DEFINE:
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, (temp)|(temp<<8), 0x1F1F);
            W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, (temp)|(temp<<8), 0x1F1F);
            break;
        default:
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L + u16bank_offset, 0x0A0A, 0x1F1F);
            W2BYTEMSK(REG_COMBO_PHY0_P0_74_L + u16bank_offset, 0x0A0A, 0x1F1F);
            break;
    }

}

//=========================================================//
// Function : Hal_HDMI_Audio_MUTE_Enable
// Description:
//=========================================================//
void Hal_HDMI_Audio_MUTE_Enable(MS_U16 u16MuteEvent, MS_U16 u16MuteMask)
{
    //Reserved
}

//=========================================================//
// Function : Hal_HDMI_Audio_Status_Clear
// Description:
//=========================================================//
void Hal_HDMI_Audio_Status_Clear(void)
{
    //Reserved
}

//=========================================================//
// Function : Hal_HDMI_gcontrol_info
// Description:
//#[15:12]: PP_VAL[3:0]
//#[11:8]: CD_VAL[3:0]
//#[7:5]: PRE_LPP[2:0]
//#[4:2]: LAST_PP[2:0]
//#[1]: Default_Phase
//#[0]: AVMUTE
//=========================================================//
MS_U16 Hal_HDMI_gcontrol_info(HDMI_GControl_INFO_t gcontrol, MS_U8 ucHDMIInfoSource)
{
    MS_U16 u16regvalue;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    u16regvalue = R2BYTE(REG_HDMI_DUAL_0_15_L);

    switch(gcontrol)
    {
        default:
        case G_CTRL_AVMUTE://#[0]: AVMUTE
            u16regvalue &= BIT(0);
            break;
        case G_Ctrl_DEFAULT_PHASE://#[1]: Default_Phase
            u16regvalue &= BIT(1);
            break;
        case G_Ctrl_LAST_PP://#[4:2]: LAST_PP[2:0]
            u16regvalue &= (BIT(4)|BIT(3)|BIT(2));
            break;
        case G_Ctrl_PRE_LPP://#[7:5]: PRE_LPP[2:0]
            u16regvalue &= (BIT(7)|BIT(6)|BIT(5));
            break;
        case G_Ctrl_CD_VAL://#[11:8]: CD_VAL[3:0]
            u16regvalue &= (BIT(11)|BIT(10)|BIT(9)|BIT(8));
            break;
        case G_Ctrl_PP_VAL://#[15:12]: PP_VAL[3:0]
            u16regvalue &= (BIT(15)|BIT(14)|BIT(13)|BIT(12));
            break;
        case G_Ctrl_ALL:
            u16regvalue &=(LBMASK|HBMASK);
            break;
    }

    return u16regvalue;
}

// T3 doesn't have HPLL
//=========================================================//
// Function : Hal_HDMI_pll_ctrl1/2
// Description:
// bread = 1-> read or write
//=========================================================//
MS_U16 Hal_HDMI_pll_ctrl1(HDMI_PLL_CTRL_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    MS_U16 u16regvalue = 0;
#if 0
    if(bread)
        u16regvalue = R2BYTE(REG_HDMI_5C_H);

    switch(pllctrl)
    {
        case PLL_CTRL_KN:                      //#KN[1:0]    15:14
            if(bread)
                u16regvalue &= (BIT(15)|BIT(14));
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & (BIT(15)|BIT(14))), (BIT(15)|BIT(14)));
            break;
        case PLL_CTRL_RCTRL:               //#RCTRL    13:12
            if(bread)
                u16regvalue &= (BIT(13)|BIT(12)|BIT(11));
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & (BIT(13)|BIT(12)|BIT(11))), (BIT(13)|BIT(12)|BIT(11)));
            break;
        case PLL_CTRL_ICTRL:               //#ICTRL    11:8
            if(bread)
                u16regvalue &= (BIT(10)|BIT(9)|BIT(8));
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & (BIT(10)|BIT(9)|BIT(8))), (BIT(10)|BIT(9)|BIT(8)));
            break;
        case PLL_CTRL_MANUAL_DIV:   //#MANUAL_PLL_DIV     7
            if(bread)
                u16regvalue &= BIT(7);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(7)), BIT(7));
            break;
        case PLL_CTRL_PORST:               //#PLL_PORST    6
            if(bread)
                u16regvalue &= BIT(6);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(6)), BIT(6));

            break;
        case PLL_CTRL_RESET_TP:        //#RESET_TP    5
            if(bread)
                u16regvalue &= BIT(5);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(5)), BIT(5));
            break;
        case PLL_CTRL_RESET_TF:        //#RESET_TF    4
            if(bread)
                u16regvalue &= BIT(4);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(4)), BIT(4));
            break;
        case PLL_CTRL_RESET_TI:        //#RESET_TI    3
            if(bread)
                u16regvalue &= BIT(3);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(3)), BIT(3));
            break;
        case PLL_CTRL_VCO_OFFSET:   //#VCO_OFFSET    2
            if(bread)
                u16regvalue &= BIT(2);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(2)), BIT(2));
            break;
        case PLL_CTRL_RESET:               //#PLL_RESET    1
            if(bread)
                u16regvalue &= BIT(1);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(1)), BIT(1));
            break;
        case PLL_CTRL_PWRDOWN:        //#PWRDOWN    0
            if(bread)
                u16regvalue &= BIT(0);
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & BIT(0)), BIT(0));
            break;
        case PLL_CTRL_L:
            if(bread)
                u16regvalue &= LBMASK;
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & LBMASK), LBMASK);
            break;
        case PLL_CTRL_H:
            if(bread)
                u16regvalue &= HBMASK;
            else
                W2BYTEMSK(REG_HDMI_5C_H, (u16value & HBMASK), HBMASK);
            break;
    }
    //if write, return 0
    if(!bread)
        u16regvalue = 0;
#endif

    return u16regvalue;
}

MS_U16 Hal_HDMI_pll_ctrl2(HDMI_PLL_CTRL2_t pllctrl, MS_BOOL bread, MS_U16 u16value)
{
    MS_U16 u16regvalue = 0;
#if 0
    if(bread)
        u16regvalue = R2BYTE(REG_HDMI_5D_H);

    switch(pllctrl)
    {

        case PLL_CTRL2_DDIV:               //#DDIV[3:0]    15:12
            if(bread)
                u16regvalue &= (BIT(15)|BIT(14)|BIT(13)|BIT(12));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(15)|BIT(14)|BIT(13)|BIT(12))), (BIT(15)|BIT(14)|BIT(13)|BIT(12)));
            break;
        case PLL_CTRL2_FBDIV:               //#FBDIV[3:0]    11:8
            if(bread)
                u16regvalue &= (BIT(11)|BIT(10)|BIT(9)|BIT(8));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(11)|BIT(10)|BIT(9)|BIT(8))), (BIT(11)|BIT(10)|BIT(9)|BIT(8)));
            break;
        case PLL_CTRL2_KP:               //#KP[3:0]    7:4
            if(bread)
                u16regvalue &= (BIT(7)|BIT(6)|BIT(5)|BIT(4));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(7)|BIT(6)|BIT(5)|BIT(4))), (BIT(7)|BIT(6)|BIT(5)|BIT(4)));
            break;
        case PLL_CTRL2_KM:               //#KM[3:0]    3:0
            if(bread)
                u16regvalue &= (BIT(3)|BIT(2)|BIT(1)|BIT(0));
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16regvalue & (BIT(3)|BIT(2)|BIT(1)|BIT(0))), (BIT(3)|BIT(2)|BIT(1)|BIT(0)));
            break;
        case PLL_CTRL2_L:
            if(bread)
                u16regvalue &= LBMASK;
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16value & LBMASK), LBMASK);
            break;
        case PLL_CTRL2_H:
            if(bread)
                u16regvalue &= HBMASK;
            else
                W2BYTEMSK(REG_HDMI_5D_H, (u16value & HBMASK), HBMASK);
            break;


    }
    //if write, return 0
    if(!bread)
        u16regvalue = 0;
#endif
    return u16regvalue;
}


void Hal_HDMI_hpd_en(MS_BOOL benable)
{
    MS_U16 u16value;

    if(benable) // 0 is output enable
        u16value= 0x0000;
    else
        u16value= 0xFFFF;

    //PM_W2BYTE(REG_PM_SLEEP_27_L, u16value & BMASK(7:4), BMASK(7:4));
    PM_W2BYTE(REG_PM_SLEEP_27_L, u16value, 0x00FF);
}

//=========================================================//
// Function : Hal_HDMI_packet_info
// Description:
//=========================================================//
MS_U32 Hal_HDMI_packet_info(MS_U8 ucHDMIInfoSource)
{
    MS_U16 usPacketStatus = 0;
    MS_U32 ulPacketStatus = 0;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    usPacketStatus = _Hal_tmds_GetPacketReceiveStatus0();

    if(usPacketStatus > 0)
    {
        ulPacketStatus = usPacketStatus;

        Hal_HDMI_packet_ctrl(ucHDMIInfoSource, usPacketStatus);
    }

    usPacketStatus = R2BYTE(REG_HDMI_DUAL_0_02_L) &BMASK(15:14);

    if(usPacketStatus > 0)
    {
        if(usPacketStatus &BIT(14))
        {
            ulPacketStatus = ulPacketStatus| HDMI_STATUS_HDR_PACKET_RECEIVE_FLAG;
        }

        if(usPacketStatus &BIT(15))
        {
            ulPacketStatus = ulPacketStatus| HDMI_STATUS_RESERVED_PACKET_RECEIVE_FLAG;
        }

        // Clear status
        W2BYTE(REG_HDMI_DUAL_0_02_L, usPacketStatus);
    }

    return ulPacketStatus;
}

//=========================================================//
// Function : Hal_HDMI_packet_ctrl
// Description:
// HDMI_PACKET_INFO_t
//=========================================================//
void Hal_HDMI_packet_ctrl(MS_U8 ucHDMIInfoSource, MS_U16 u16pkt)
{
    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    W2BYTE(REG_HDMI_DUAL_0_01_L, u16pkt);
}

//=========================================================//
// Function : Hal_HDMI_avi_infoframe_info_ver
// Description: avi info frame version
//=========================================================//
EN_AVI_INFOFRAME_VERSION Hal_HDMI_avi_infoframe_info_ver(MS_U8 ucHDMIInfoSource)
{
    EN_AVI_INFOFRAME_VERSION enPktVersion= E_AVI_INFOFRAME_VERSION_NON;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if (R2BYTEMSK(REG_HDMI_DUAL_0_46_L, BMASK(9:8)) >> 8 == 0x00)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION_NON;
    }
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_46_L, BMASK(9:8)) >> 8 == 0x01)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION1;
    }
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_46_L, BMASK(9:8)) >> 8 == 0x02)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION2;
    }
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_46_L, BMASK(9:8)) >> 8 == 0x03)
    {
        enPktVersion = E_AVI_INFOFRAME_VERSION3;
    }

    return enPktVersion;
}

//=========================================================//
// Function : Hal_HDMI_avi_infoframe_info
// Description: avi info frame byte 1~13
//=========================================================//
MS_U8 Hal_HDMI_avi_infoframe_info(MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    MS_U16 u16Reg = 0;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if(u8byte<1) //u8byte = 0 is invalide
        u8byte = 1;

    u16Reg = R2BYTE(REG_HDMI_DUAL_0_40_L + ((u8byte - 1)/2)*2);

    return (MS_U8)(((u8byte-1)&0x01) ? (u16Reg>>8):(u16Reg&0xFF));
}

//=========================================================//
// Function : Hal_HDMI_get_packet_value
// Description:
//=========================================================//
MS_BOOL Hal_HDMI_get_packet_value(MS_U8 ucHDMIInfoSource, MS_HDMI_PACKET_STATE_t u8state, MS_U8 u8byte_idx, MS_U8 *pu8Value)
{
    MS_U32 u32Base_add = 0;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    switch(u8state)
    {
        case PKT_MPEG:
            //1~5, 57
            if(u8byte_idx > 5)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_57_L;

            break;
        case PKT_AUI:
            //1~5, 54
            if(u8byte_idx > 5)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_54_L;

            break;
        case PKT_SPD:
            //1~25, 47
            if(u8byte_idx > 25)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_47_L;

            break;
        case PKT_AVI:
            //1~13, 40
            if(u8byte_idx > 13)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_40_L;

            break;
        case PKT_GC:
            //2, 15[15:0]
            if(u8byte_idx > 2)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_15_L;

            break;
        case PKT_ASAMPLE:
            break;
        case PKT_ACR:
            break;
        case PKT_VS:
            //0~28, 30
            u8byte_idx ++;
            if(u8byte_idx > 29)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_30_L;

            break;
        case PKT_NULL:
            break;
        case PKT_ISRC2:
            //0~31, 20
            u8byte_idx ++;
            if(u8byte_idx > 32)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_20_L;

            break;
        case PKT_ISRC1:
            //1, 1F
            if(u8byte_idx > 1)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_1F_L;

            break;
        case PKT_ACP:
            //0~15, 17
            u8byte_idx ++;
            if(u8byte_idx > 16)
                return FALSE;
            u32Base_add = REG_HDMI_DUAL_0_17_L;

            break;
        case PKT_ONEBIT_AUD:
            break;
        case PKT_HDR:
            u32Base_add = REG_HDMI2_DUAL_0_61_L;
            break;
        default:
            break;
    }

    *pu8Value = MDrv_ReadByte(u32Base_add +u8byte_idx);

    return TRUE;
}

//=========================================================//
// Function : Hal_HDMI_acp_info
// Description: ACP_HDR1[7:0]
//=========================================================//
MS_U8 Hal_HDMI_audio_content_protection_info(MS_U8 ucHDMIInfoSource)
{
    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    return R2BYTEMSK(REG_HDMI_DUAL_0_16_L, LBMASK);
}

//=========================================================//
// Function : Hal_HDMI_audio_payload_info
// Description: ACP packet payload byte 0~15
//=========================================================//
MS_U8 Hal_HDMI_audio_payload_info(MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if(u8byte%2)
    {
        return ((R2BYTEMSK(REG_HDMI_DUAL_0_17_L +(u8byte/2)*2, HBMASK)) >> 8);
    }
    else
    {
        return R2BYTEMSK(REG_HDMI_DUAL_0_17_L +(u8byte/2)*2, LBMASK);
    }
}

//=========================================================//
// Function : Hal_HDMI_err_status_update
// Description: ACP packet payload byte 0~15
//=========================================================//
MS_U8 Hal_HDMI_err_status_update(MS_U8 ucHDMIInfoSource, MS_U8 u8value, MS_BOOL bread)
{
    MS_U8 u8regvalue = 0xFF;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if(bread)
    {
        u8regvalue = R2BYTEMSK(REG_HDMI_DUAL_0_04_L, LBMASK);
    }
    else
    {
        W2BYTEMSK(REG_HDMI_DUAL_0_04_L, u8value, LBMASK);
    }
    return u8regvalue;
}
/******************************************************************************/
/// for HDMI AUDIO
/******************************************************************************/
//=========================================================//
// Function : Hal_HDMI_audio_output
// Description:
//=========================================================//
void Hal_HDMI_audio_output( MS_BOOL bEnable )
{
#if 0 // not need in T7
    if ( bEnable )
    {
        W2BYTEMSK( REG_HDMI_08_L, (BIT(2) | BIT(0))<<8, (BIT(2) | BIT(0))<<8);
    }
    else
    {
        W2BYTEMSK( REG_HDMI_08_L, 0x0000, HBMASK);
    }
#endif
}

//=========================================================//
// Function : Hal_HDMI_audio_channel_status
// Description: channel status byte 0~4
//=========================================================//
MS_U8 Hal_HDMI_audio_channel_status(MS_U8 ucHDMIInfoSource, MS_U8 u8byte)
{
    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if(u8byte%2)
    {
        return ((R2BYTEMSK(REG_HDMI_DUAL_0_5A_L +(u8byte/2)*2, HBMASK)) >> 8);
    }
    else
    {
        return R2BYTEMSK(REG_HDMI_DUAL_0_5A_L +(u8byte/2)*2, LBMASK);
    }
}

/******************************************************************************/
/// for DVI
/******************************************************************************/
// Aaron comment: doesn't need in U3 because IC already has auto mode
//=========================================================//
// Function : Hal_DVI_clk_lessthan70mhz_usingportc
// Description:
//=========================================================//
MS_BOOL Hal_DVI_clk_lessthan70mhz_usingportc( MS_BOOL bIsPortC )
{
    return 0xFF;
}

//=========================================================//
// Function : Hal_DVI_clk_stable_usingportc
// Description:
//=========================================================//
MS_BOOL Hal_DVI_clk_stable_usingportc( MS_BOOL bIsPortC )
{
    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Returne Phase status
/// @param  bClockLessThan70MHz                \b IN: u8Channel
//-------------------------------------------------------------------------------------------------
MS_U16 Hal_DVI_ChannelPhaseStatus( MS_U8 u8Channel)
{
    switch (u8Channel)
    {
        case 0: // Channl R
            return R2BYTE(REG_DVI_DTOP_DUAL_P0_01_L);
            break;
        case 1: // Channl G
            return R2BYTE(REG_DVI_DTOP_DUAL_P0_02_L);
            break;
        case 2:// Channl B
            return R2BYTE(REG_DVI_DTOP_DUAL_P0_03_L);
            break;
        default:
            return 0;
            break;
    }
}

//=========================================================//
// Function : Hal_DVI_Accumulator_Monitor
// Description:
//=========================================================//
void Hal_DVI_Accumulator_Monitor( void )
{
    // do nothing
}


//=========================================================//
// Function : Hal_DVI_HF_adjust
// Description:
//=========================================================//
void Hal_DVI_HF_adjust( void )
{
#if 0
    MS_U8 temp = 0;

    // port A
    if(R2BYTE(REG_COMBO_PHY0_P0_32_L) & BIT(3)& BIT(7)& BIT(11)& BIT(15)) // clk stable
    {
        temp = (R2BYTE(REG_DVI_ATOP_61_L) & 0x0300)>>8;
        if(temp == 0x00)
        {
            W2BYTEMSK(REG_DVI_ATOP_32_L, BMASK(9:8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x01)
        {
            W2BYTEMSK(REG_DVI_ATOP_32_L, BIT(8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x02)
        {
            W2BYTEMSK(REG_DVI_ATOP_32_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else
        {
            W2BYTEMSK(REG_DVI_ATOP_32_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4)|BMASK(2:0), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        if(R2BYTE(REG_DVI_DTOP_31_L) & BIT(6))
        {
            W2BYTEMSK(REG_DVI_DTOP_24_L, BIT(13)|BIT(5), BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP_25_L, BIT(5), BIT(5));
        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP_24_L, 0, BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP_25_L, 0, BIT(5));
        }
    }
    else
    {
        W2BYTEMSK(REG_DVI_DTOP_24_L, 0, BIT(13)|BIT(5));
        W2BYTEMSK(REG_DVI_DTOP_25_L, 0, BIT(5));
    }

    // port B
    if(R2BYTE(REG_COMBO_PHY0_P1_32_L) & BIT(3)& BIT(7)& BIT(11)& BIT(15)) // clk stable
    {
        temp = (R2BYTE(REG_DVI_ATOP1_61_L) & 0x0300)>>8;
        if(temp == 0x00)
        {
            W2BYTEMSK(REG_DVI_ATOP1_32_L, BMASK(9:8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x01)
        {
            W2BYTEMSK(REG_DVI_ATOP1_32_L, BIT(8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x02)
        {
            W2BYTEMSK(REG_DVI_ATOP1_32_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else
        {
            W2BYTEMSK(REG_DVI_ATOP1_32_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4)|BMASK(2:0), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        if(R2BYTE(REG_DVI_DTOP1_31_L) & BIT(6))
        {
            W2BYTEMSK(REG_DVI_DTOP1_24_L, BIT(13)|BIT(5), BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP1_25_L, BIT(5), BIT(5));
        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP1_24_L, 0, BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP1_25_L, 0, BIT(5));
        }
    }
    else
    {
        W2BYTEMSK(REG_DVI_DTOP1_24_L, 0, BIT(13)|BIT(5));
        W2BYTEMSK(REG_DVI_DTOP1_25_L, 0, BIT(5));
    }

    // port C
    if(R2BYTE(REG_COMBO_PHY0_P2_32_L) & BIT(3)& BIT(7)& BIT(11)& BIT(15)) // clk stable
    {
        temp = (R2BYTE(REG_DVI_ATOP_6A_L) & 0x0300)>>8;
        if(temp == 0x00)
        {
            W2BYTEMSK(REG_DVI_ATOP_35_L, BMASK(9:8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_6D_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x01)
        {
            W2BYTEMSK(REG_DVI_ATOP_35_L, BIT(8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_6D_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x02)
        {
            W2BYTEMSK(REG_DVI_ATOP_35_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_6D_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else
        {
            W2BYTEMSK(REG_DVI_ATOP_35_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP_6D_L, BIT(4)|BMASK(2:0), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        if(R2BYTE(REG_DVI_DTOP3_31_L) & BIT(6))
        {
            W2BYTEMSK(REG_DVI_DTOP3_24_L, BIT(13)|BIT(5), BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP3_25_L, BIT(5), BIT(5));
        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP3_24_L, 0, BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP3_25_L, 0, BIT(5));
        }
    }
    else
    {
        W2BYTEMSK(REG_DVI_DTOP3_24_L, 0, BIT(13)|BIT(5));
        W2BYTEMSK(REG_DVI_DTOP3_25_L, 0, BIT(5));
    }

    // port D
    if(R2BYTE(REG_COMBO_PHY0_P3_32_L) & BIT(3)& BIT(7)& BIT(11)& BIT(15)) // clk stable
    {
        temp = (R2BYTE(REG_DVI_ATOP2_61_L) & 0x0300)>>8;
        if(temp == 0x00)
        {
            W2BYTEMSK(REG_DVI_ATOP2_32_L, BMASK(9:8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x01)
        {
            W2BYTEMSK(REG_DVI_ATOP2_32_L, BIT(8), BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else if(temp == 0x02)
        {
            W2BYTEMSK(REG_DVI_ATOP2_32_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        else
        {
            W2BYTEMSK(REG_DVI_ATOP2_32_L, 0, BMASK(9:8));
            W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4)|BMASK(2:0), BMASK(4:0)); //[4]: ICtrl overwrite, [3:0]: ICtrl value
        }
        if(R2BYTE(REG_DVI_DTOP2_31_L) & BIT(6))
        {
            W2BYTEMSK(REG_DVI_DTOP2_24_L, BIT(13)|BIT(5), BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP2_25_L, BIT(5), BIT(5));
        }
        else
        {
            W2BYTEMSK(REG_DVI_DTOP2_24_L, 0, BIT(13)|BIT(5));
            W2BYTEMSK(REG_DVI_DTOP2_25_L, 0, BIT(5));
        }
    }
    else
    {
        W2BYTEMSK(REG_DVI_DTOP2_24_L, 0, BIT(13)|BIT(5));
        W2BYTEMSK(REG_DVI_DTOP2_25_L, 0, BIT(5));
    }
#endif
}

//=========================================================//
// Function : Hal_DVI_adc_adjust
// Description:
//=========================================================//
void Hal_DVI_adc_adjust( MS_BOOL bClockLessThan70MHz )
{
    // do nothing
}

//=========================================================//
// Function : Hal_DVI_clklose_det
// Description:
//=========================================================//
MS_BOOL Hal_DVI_clklose_det(E_MUX_INPUTPORT enInputPortType) // 4M clock
{
    MS_BOOL bClockStable = FALSE;

    if(_Hal_tmds_GetClockStableFlag(enInputPortType))
    {
        bClockStable = TRUE;
    }
    else if(_Hal_tmds_GetMHL3ClockStableFlag(enInputPortType))
    {
        bClockStable = TRUE;
    }

    return (!bClockStable);
}

void Hal_DVI_IMMESWITCH_PS_SW_Path(void)
{
#if 0

    MS_U8 i, u8reg_offset = 0;
    MS_U16 u16bank_offset =0;

    for(i=0;i<4;i++)
    {
        switch(i)
        {
            case 0:  u16bank_offset = 0x0000; u8reg_offset = 0x00; break;
            case 1:  u16bank_offset = 0x2900; u8reg_offset = 0x40; break;
            case 2:  u16bank_offset = 0x2B00; u8reg_offset = 0x80; break;
            case 3:  u16bank_offset = 0x2D00; u8reg_offset = 0xC0; break;
            default:
                break;
        }
        // Input clock is stable but DE is idel or unstable
        if( (R2BYTE(REG_DVI_DTOP_16_L+u16bank_offset) & BIT(10)) && (R2BYTE(REG_DVI_DTOP_31_L+u16bank_offset) & 0x30) )
        {
            // Turn off Ch2 as DE align out and cycle align delay
            W2BYTEMSK(REG_DVI_PS_01_L+u8reg_offset, BIT(8), BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
            MsOS_DelayTask(1);
            W2BYTEMSK(REG_DVI_PS_01_L+u8reg_offset, BIT(9)|BIT(8), BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
        }
        // Not turn on power saving mode and DE is stable, but PS still stay in saving state
        if( !(R2BYTE(REG_DVI_PS_00_L+u8reg_offset) & BIT(0)) && (R2BYTE(REG_DVI_DTOP_31_L+u16bank_offset) & BIT(6)) && (R2BYTE(REG_DVI_PS_12_L+u8reg_offset) & BIT(4)) )
        {
            W2BYTEMSK(REG_DVI_PS_00_L+u8reg_offset, BIT(9), BIT(9)); //[9]: PS FSM reset
            MsOS_DelayTask(1);
            W2BYTEMSK(REG_DVI_PS_00_L+u8reg_offset, 0, BIT(9));
        }
    }
#endif

}

/******************************************************************************/
///DVI soft reset
/******************************************************************************/
void Hal_DVI_sw_reset(MS_U16 u16Reset)
{
}

//=========================================================//
// Function : Hal_DVI_irq_info
// Description:
//=========================================================//
MS_U16 Hal_DVI_irq_info(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U16 u16reg_val = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_val = R2BYTEMSK(REG_COMBO_PHY1_P0_40_L, irqbit); break;
        case INPUT_PORT_DVI1:  u16reg_val = R2BYTEMSK(REG_COMBO_PHY1_P1_40_L, irqbit); break;
        case INPUT_PORT_DVI2:  u16reg_val = R2BYTEMSK(REG_COMBO_PHY1_P2_40_L, irqbit); break;
        case INPUT_PORT_DVI3:  u16reg_val = R2BYTEMSK(REG_COMBO_PHY1_P3_40_L, irqbit); break;
    }

    return u16reg_val;
}

//=========================================================//
// Function : Hal_DVI_irq_clear
// Description:
//=========================================================//
void Hal_DVI_irq_mask(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U32 u16reg_add = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_add = REG_COMBO_PHY1_P0_40_L;  break;
        case INPUT_PORT_DVI1:  u16reg_add = REG_COMBO_PHY1_P1_40_L;  break;
        case INPUT_PORT_DVI2:  u16reg_add = REG_COMBO_PHY1_P2_40_L;  break;
        case INPUT_PORT_DVI3:  u16reg_add = REG_COMBO_PHY1_P3_40_L;  break;
    }

    W2BYTEMSK(u16reg_add, (irqbit <<8), (IRQ_ALL_BIT<<8));
}

//=========================================================//
// Function : Hal_DVI_irq_forcemode
// Description:
//=========================================================//
void Hal_DVI_irq_forcemode(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U32 u16reg_add = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_add = REG_COMBO_PHY1_P0_41_L;  break;
        case INPUT_PORT_DVI1:  u16reg_add = REG_COMBO_PHY1_P1_41_L;  break;
        case INPUT_PORT_DVI2:  u16reg_add = REG_COMBO_PHY1_P2_41_L;  break;
        case INPUT_PORT_DVI3:  u16reg_add = REG_COMBO_PHY1_P3_41_L;  break;
    }

    W2BYTEMSK(u16reg_add, irqbit, IRQ_ALL_BIT);
}

//=========================================================//
// Function : Hal_DVI_irq_clear
// Description:
//=========================================================//
void Hal_DVI_irq_clear(E_MUX_INPUTPORT enInputPortType, MS_U16 irqbit)
{
    MS_U32 u16reg_add = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16reg_add = REG_COMBO_PHY1_P0_41_L;  break;
        case INPUT_PORT_DVI1:  u16reg_add = REG_COMBO_PHY1_P1_41_L;  break;
        case INPUT_PORT_DVI2:  u16reg_add = REG_COMBO_PHY1_P2_41_L;  break;
        case INPUT_PORT_DVI3:  u16reg_add = REG_COMBO_PHY1_P3_41_L;  break;
    }

    W2BYTEMSK(u16reg_add, (irqbit<<8), (IRQ_ALL_BIT<<8));
}

//=========================================================//
// Function : Hal_DVI_ClkPullLow
// Description:
//=========================================================//
void Hal_DVI_ClkPullLow(MS_BOOL bPullLow, E_MUX_INPUTPORT enInputPortType)
{
#if 0//defined(MSTAR_IMMESWITCH)

    // DVI clock is always set to high becasue of Instaport
    PM_W2BYTE(REG_PM_SLEEP_4B_L, FALSE ? BMASK(11:8) : 0, BMASK(11:8));

#else

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0://A
            PM_W2BYTE(REG_PM_SLEEP_4C_L, bPullLow ? BIT(0) : 0, BIT(0));
            break;
        case INPUT_PORT_DVI1://B
            PM_W2BYTE(REG_PM_SLEEP_4C_L, bPullLow ? BIT(4) : 0, BIT(4));
            break;
        case INPUT_PORT_DVI2://C
            PM_W2BYTE(REG_PM_SLEEP_4C_L, bPullLow ? BIT(8) : 0, BIT(8));
            break;
        case INPUT_PORT_DVI3://D
            PM_W2BYTE(REG_PM_SLEEP_4C_L, bPullLow ? BIT(12) : 0, BIT(12));
            break;
        default: // For system first power on with the unknow HDMI port.
            PM_W2BYTE(REG_PM_SLEEP_4C_L, bPullLow ? BIT(12)|BIT(8)|BIT(4)|BIT(0) : 0, BIT(12)|BIT(8)|BIT(4)|BIT(0));
            break;
    }

#endif // #if defined(MSTAR_IMMESWITCH)
}

//=========================================================//
// Function : Hal_DVI_SwitchSrc
// Description:
//=========================================================//
void Hal_DVI_SwitchSrc(E_MUX_INPUTPORT enInputPortType, MS_U8 ucHDMIInfoSource)
{
    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0: // A
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, 0, BMASK(1:0));
            break;

        case INPUT_PORT_DVI1: // B
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, 1, BMASK(1:0));
            break;

        case INPUT_PORT_DVI2: // C
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, 2, BMASK(1:0));
            break;

        case INPUT_PORT_DVI3: // D
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, 3, BMASK(1:0));
            break;
        default:
            //ASSERT("Not support!\n");
            break;
    }
}

void HAL_HDMI_DDCRam_SelectPort(E_XC_DDCRAM_PROG_TYPE eDDCRamType)
{
    switch(eDDCRamType)
    {
        case E_XC_PROG_DVI0_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI0),REG_DDC_SRAM_SEL_MASK);
            DDC_WRITE_MASK(REG_DDC_D0_SRAM_BASEADDR,BITS(4:0,DDC_OFFSET_SRAM_DVI0),REG_DDC_D0_BASEADDR_MASK);
            break;
        case E_XC_PROG_DVI1_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI1),REG_DDC_SRAM_SEL_MASK);
            DDC_WRITE_MASK(REG_DDC_D1_SRAM_BASEADDR,BITS(12:8,DDC_OFFSET_SRAM_DVI1),REG_DDC_D1_BASEADDR_MASK);
            break;
        case E_XC_PROG_DVI2_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI2),REG_DDC_SRAM_SEL_MASK);
            DDC_WRITE_MASK(REG_DDC_D2_SRAM_BASEADDR,BITS(4:0,DDC_OFFSET_SRAM_DVI2),REG_DDC_D2_BASEADDR_MASK);
            break;
        case E_XC_PROG_DVI3_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_DVI),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(12:11,DDC_RAM_SRAM_DVI3),REG_DDC_SRAM_SEL_MASK);
            DDC_WRITE_MASK(REG_DDC_D3_SRAM_BASEADDR,BITS(12:8,DDC_OFFSET_SRAM_DVI3),REG_DDC_D3_BASEADDR_MASK);
            break;
        case E_XC_PROG_VGA_EDID:
            DDC_WRITE_MASK(REG_DDC_SRAM_SEL,BITS(14:14,DDC_RAM_SRAM_ADC),BMASK(14:14));
            DDC_WRITE_MASK(REG_DDC_A_SRAM_BASEADDR,BITS(12:8,DDC_OFFSET_SRAM_ADC),REG_DDC_A_BASEADDR_MASK);
            break;
        default:
            //ASSERT("Not support!\n");
            break;
    }
}

void HAL_HDMI_DDCRAM_SetPhyAddr(XC_DDCRAM_PROG_INFO *pstDDCRam_Info)
{
    UNUSED(pstDDCRam_Info);
}


//=========================================================//
// Function : Hal_DVI_GetPowerSavingStatus
// Description: Get power saving mode status
// return:  TRUE: stable, FALSE: unstable
//=========================================================//
MS_BOOL Hal_DVI_GetPowerSavingStatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U8 u8offset;
    MS_BOOL result;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u8offset = 0x00;  break;
        case INPUT_PORT_DVI1:  u8offset = 0x40;  break;
        case INPUT_PORT_DVI2:  u8offset = 0x80;  break;
        case INPUT_PORT_DVI3:  u8offset = 0xC0;  break;
    }

    result = (R2BYTEMSK(REG_DVI_PS_00_L+u8offset, BIT(0)) & BIT(0)) ? TRUE : FALSE;

    return result;
}

//=========================================================//
// Function : Hal_DVI_GetDEStableStatus
// Description: Get DE stable status for Immeswitch power saving mode
// return:  TRUE: stable, FALSE: unstable
//=========================================================//
MS_BOOL Hal_DVI_GetDEStableStatus(E_MUX_INPUTPORT enInputPortType)
{
    MS_U16 u16bank_offset;
    MS_BOOL result;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x171000
        case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171300
        case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171600
        case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171900
    }

    result = (R2BYTEMSK(REG_DVI_DTOP_DUAL_P0_31_L + u16bank_offset, 0x0070) & BIT(6)) ? TRUE : FALSE;

    return result;
}

//=========================================================//
// Function : Hal_DVI_EnhanceImmeswitch
// Description: Enable / Disable Immeswitch enhancement
//=========================================================//
void Hal_DVI_EnhanceImmeswitch(E_MUX_INPUTPORT enInputPortType, MS_BOOL bflag)
{
    MS_U8 u8offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u8offset = 0x00;  break;
        case INPUT_PORT_DVI1:  u8offset = 0x40;  break;
        case INPUT_PORT_DVI2:  u8offset = 0x80;  break;
        case INPUT_PORT_DVI3:  u8offset = 0xC0;  break;
    }

    W2BYTEMSK(REG_DVI_PS_01_L+u8offset, bflag ? BMASK(9:8) : 0, BMASK(9:8)); // [9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
}

//=========================================================//
// Function : Hal_DVI_ForceAllPortsEnterPS
// Description: Force all DVI ports enter power saving mode
//=========================================================//
void Hal_DVI_ForceAllPortsEnterPS(void)
{
    E_MUX_INPUTPORT port_index;
    MS_U16 u16bank_offset;
 //   MS_BOOL result;

    for(port_index = INPUT_PORT_DVI0; port_index <= INPUT_PORT_DVI_END; port_index++)
    {
        switch(port_index)
        {
            default:
            case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x171000
            case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171300
            case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171600
            case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171900
        }

        // DVI slowly update
        W2BYTEMSK( REG_DVI_DTOP_DUAL_P0_29_L, 0, BIT(15)); // [15]: turn off slowly updated (b-ch)
        W2BYTEMSK( REG_DVI_DTOP_DUAL_P0_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated (r-ch), [7]: turn off slowly updated (g-ch)

    }
#if 0
    // DVI power saving mode
    W2BYTEMSK(REG_DVI_PS_00_L, BIT(0), BIT(0)); // turn on DVI power saving mode
    W2BYTEMSK(REG_DVI_PS1_00_L, BIT(0), BIT(0)); // turn on DVI1 power saving mode
    W2BYTEMSK(REG_DVI_PS2_00_L, BIT(0), BIT(0)); // turn on DVI2 power saving mode
    W2BYTEMSK(REG_DVI_PS3_00_L, BIT(0), BIT(0)); // turn on DVI3 power saving mode
    // Turn off Ch2 as DE align out and cycle align delay
    W2BYTEMSK(REG_DVI_PS_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
    W2BYTEMSK(REG_DVI_PS1_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
    W2BYTEMSK(REG_DVI_PS2_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
    W2BYTEMSK(REG_DVI_PS3_01_L, 0, BMASK(9:8)); //[9]: DE cycle align delay, [8]: no select Ch2 DE as DE align out
#endif
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                HDMI 1.4 new feature:
//                                1. 3D format
//                                2. 4K x 2K format
//                                3. ARC
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// HDMI_Video_Format: Vendor Specifc Info-frame, PB4[7:5]
//   000: No additional HDMI video format is presented in this packet
//   001: Extended resolution format (e.g. 4Kx2K video) present
//   010: 3D format indication present
//   011~111: Reserved
E_HDMI_ADDITIONAL_VIDEO_FORMAT Hal_HDMI_Check_Additional_Format(MS_U8 ucHDMIInfoSource)
{
    E_HDMI_ADDITIONAL_VIDEO_FORMAT val;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(7:5)) == 0)
        val = E_HDMI_NO_ADDITIONAL_FORMAT;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(7:5)) == BIT(5))
        val = E_HDMI_4Kx2K_FORMAT;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(7:5)) == BIT(6))
        val = E_HDMI_3D_FORMAT;
    else
        val = E_HDMI_RESERVED;

    return val;
}

// 3D_Structure: Vendor Specifc Info-frame, PB5[7:4]
//   0000: Frame packing
//   0001: Field alternative
//   0010: Line alternative
//   0011: Side-by-Side(Full)
//   0100: L+depth
//   0101: L+depth+graphics+graphics-depth
//   0110: Top-and-Bottom
//   0111: Reserved
//   1000: Side-by-Side(Half)
//   1001 ~ 1111: Reserved
E_XC_3D_INPUT_MODE Hal_HDMI_Get_3D_Structure(MS_U8 ucHDMIInfoSource)
{
    E_XC_3D_INPUT_MODE val;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == 0)
        val = E_XC_3D_INPUT_FRAME_PACKING;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == BIT(12))
        val = E_XC_3D_INPUT_FIELD_ALTERNATIVE;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == BIT(13))
        val = E_XC_3D_INPUT_LINE_ALTERNATIVE;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == (BIT(13)|BIT(12)))
        val = E_XC_3D_INPUT_SIDE_BY_SIDE_FULL;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == BIT(14))
        val = E_XC_3D_INPUT_L_DEPTH;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == (BIT(14)|BIT(12)))
        val = E_XC_3D_INPUT_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == (BIT(14)|BIT(13)))
        val = E_XC_3D_INPUT_TOP_BOTTOM;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:12)) == BIT(15))
        val = E_XC_3D_INPUT_SIDE_BY_SIDE_HALF;
    else
        val = E_XC_3D_INPUT_MODE_NONE;

    return val;
}

// 3D_Ext_Data: Vendor Specifc Info-frame, PB6[7:4]
//   0000 ~ 0011 : Horizontal sub-sampling
//   0100: Quincunx matrix - Odd/Left  picture, Odd/Right  picture
//   0101: Quincunx matrix - Odd/Left  picture, Even/Right picture
//   0110: Quincunx matrix - Even/Left picture, Odd/Right  picture
//   0111: Quincunx matrix - Even/Left picture, Even/Right picture
//   1000 ~ 1111: Reserved
E_HDMI_3D_EXT_DATA_T Hal_HDMI_Get_3D_Ext_Data(MS_U8 ucHDMIInfoSource)
{
    MS_U16 u16temp;
    E_HDMI_3D_EXT_DATA_T e_3d_extdata;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    u16temp = R2BYTEMSK(REG_HDMI_DUAL_0_35_L, BMASK(7:4));

    switch(u16temp)
    {
        case 0x0000:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_0;
            break;
        case 0x0010:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_1;
            break;
        case 0x0020:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_2;
            break;
        case 0x0030:
            e_3d_extdata = E_3D_EXT_DATA_HOR_SUB_SAMPL_3;
            break;
        case 0x0040:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_0;
            break;
        case 0x0050:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_1;
            break;
        case 0x0060:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_2;
            break;
        case 0x0070:
            e_3d_extdata = E_3D_EXT_DATA_QUINCUNX_MATRIX_3;
            break;
        default:
            e_3d_extdata = E_3D_EXT_DATA_MODE_MAX;
            break;
    };

    return e_3d_extdata;
}

// 3D_Meta_Field: Vendor Specifc Info-frame, PB5[3], PB7 ~ PB7+N
//   - 3D_Meta_Present: PB5[3]
//   - 3D_Metadata_Type: PB7[7:5]
//       000: parallax information defined in ISO23002-3
//       001 ~ 111: reserved
//   - 3D_Metadata_Length(N, N <= 31-8 = 23): PB7[4:0]
//   - 3D_Metadata[] : PB8 ~ PB7+N
void Hal_HDMI_Get_3D_Meta_Field(MS_U8 ucHDMIInfoSource, sHDMI_3D_META_FIELD *pdata)
{
    MS_U8 i;
    MS_U16 u16temp;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    pdata->b3D_Meta_Present = R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BIT(11)) > 0 ? TRUE : FALSE;
    u16temp = R2BYTEMSK(REG_HDMI_DUAL_0_35_L, BMASK(15:13));

    switch(u16temp)
    {
        case 0x0000:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_PARALLAX_ISO23002_3;
            break;
        case 0x2000:
        case 0x3000:
        case 0x4000:
        case 0x5000:
        case 0x6000:
        case 0x7000:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_RESERVE;
            break;
        default:
            pdata->t3D_Metadata_Type = E_3D_META_DATA_MAX;
            break;
    }

    pdata->u83D_Metadata_Length = (MS_U8)(R2BYTEMSK(REG_HDMI_DUAL_0_35_L, BMASK(12:8)) >> 8);

    for(i=0;i<pdata->u83D_Metadata_Length;i++)
    {
        pdata->u83D_Metadata[i] = R2BYTEMSK(REG_HDMI_DUAL_0_36_L +i, BMASK(7:0));
    }
}

// VIC_CODE: Auxiliary Video Information Info-frame, PB4[6:0]
MS_U8 Hal_HDMI_Get_VIC_Code(MS_U8 ucHDMIInfoSource)
{
    MS_U8 val;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    val = R2BYTEMSK(REG_HDMI_DUAL_0_41_L, BMASK(14:8)) >> 8;

    return val;
}

// 4Kx2K VIC code: Vendor Specifc Info-frame, PB5[7:0]
//   0x01: 1920(x2)x1080(x2) @ 29.97/30Hz
//   0x02: 1920(x2)x1080(x2) @ 25Hz
//   0x03: 1920(x2)x1080(x2) @ 23.976/24Hz
//   0x04: 2048(x2)x1080(x2) @ 24Hz
//   0x00, 0x05 ~ 0xFF: Reserved
E_HDMI_VIC_4Kx2K_CODE Hal_HDMI_Get_4Kx2K_VIC_Code(MS_U8 ucHDMIInfoSource)
{
    E_HDMI_VIC_4Kx2K_CODE val;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:8)) >> 8 == 0x01)
        val = E_VIC_4Kx2K_30Hz;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:8)) >> 8 == 0x02)
        val = E_VIC_4Kx2K_25Hz;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:8)) >> 8 == 0x03)
        val = E_VIC_4Kx2K_24Hz;
    else if (R2BYTEMSK(REG_HDMI_DUAL_0_34_L, BMASK(15:8)) >> 8 == 0x04)
        val = E_VIC_4Kx2K_24Hz_SMPTE;
    else
        val = E_VIC_RESERVED;

    return val;
}

// It must set to non-bypass(0) when input source is detected as HDMI 3D format
// 0: no bypass, 1: bypass
void Hal_HDMI_Set_YUV422to444_Bypass(MS_BOOL btrue)
{
    btrue = FALSE;
}

// Check whether input pixel rate is large 166MHz or not.
MS_BOOL Hal_HDMI_Is_Input_Large_166MHz(MS_U8 ucHDMIInfoSource)
{
    MS_BOOL bflag = FALSE;
    MS_U16 reg_val;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    reg_val = R2BYTEMSK(REG_HDMI2_DUAL_0_36_L, BMASK(12:0));

    if ((reg_val/128)*12 >= 166)
        bflag = TRUE;

    return bflag;
}

// If input format is 3D or 4Kx2K and pixel rate large than 166MHz, it need to do the horizontal scaling down.
void Hal_HDMI_AVG_ScaleringDown(MS_U8 ucHDMIInfoSource, MS_BOOL btrue)
{
    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    if(btrue)
    {
        return; //Einstein support 300Mhz
        //W2BYTEMSK(REG_HDMI2_20_L, BIT(5), BIT(5)|BIT(3)|BIT(2)); // [5]: 1: WR_Clock_Freq/2, [3:2]: 2'b 00: AVG and FIFO enable
    }
    else
    {
        //W2BYTEMSK(REG_HDMI2_20_L, BIT(2), BIT(5)|BIT(3)|BIT(2)); // [5]: 0: WR_Clock_Freq, [3:2]: 2'b 01: AVG bypass and FIFO enable
        W2BYTEMSK(REG_HDMI2_DUAL_0_20_L, 0, BIT(5)); // [5]: 0: WR_Clock_Freq
    }
}

// HDMI ARC pin control
//     - enInputPortType: INPUT_PORT_DVI0 / INPUT_PORT_DVI1 / INPUT_PORT_DVI2 / INPUT_PORT_DVI3
//     - bEnable: ARC enable or disable
//     - bDrivingHigh: ARC driving current high or low
void Hal_HDMI_ARC_PINControl(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnable, MS_BOOL bDrivingHigh)
{
    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0: // A
        case INPUT_PORT_DVI1: // B
        case INPUT_PORT_DVI2: // C
        case INPUT_PORT_DVI3: // D
            W2BYTEMSK(REG_CHIP_GPIO1_10_L, bEnable ? BIT(3) : 0, BIT(3)); // PAD_ARC MODE
            break;
        default:
            break;
    }

    // Driving current
    W2BYTEMSK(REG_CHIP_05_L, bDrivingHigh ? BIT(11) : 0, BIT(11));
}

/******************************************************************************/
///DVI soft reset
/******************************************************************************/
//[6]: soft-reset hdmi
//[5]: soft-reset hdcp
//[4]: soft-reset dvi
void Hal_DVI_Software_Reset(E_MUX_INPUTPORT enInputPortType, MS_U16 u16Reset)
{
    MS_U16 u16DTOPOffset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16DTOPOffset = 0x0000;  break; // BK 0x171000
        case INPUT_PORT_DVI1:  u16DTOPOffset = 0x0300;  break; // BK 0x171300
        case INPUT_PORT_DVI2:  u16DTOPOffset = 0x0600;  break; // BK 0x171600
        case INPUT_PORT_DVI3:  u16DTOPOffset = 0x0900;  break; // BK 0x171900
    }

    if(u16Reset & BIT(4))
    {
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3E_L + u16DTOPOffset, BIT(0)); // DTOP_3E[0]: dvi sw reset
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3E_L + u16DTOPOffset, 0);
    }
    if(u16Reset & BIT(5))
    {
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3E_L + u16DTOPOffset, BIT(2)); // DTOP_3E[2]: hdcp sw reset
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3E_L + u16DTOPOffset, 0);
    }
    if(u16Reset & BIT(6))
    {
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3E_L + u16DTOPOffset, BIT(3)); // DTOP_3E[3]: hdmi sw reset
        W2BYTE(REG_DVI_DTOP_DUAL_P0_3E_L + u16DTOPOffset, 0);
    }
    if(u16Reset & BIT(10))
    {
        W2BYTE(REG_COMBO_GP_TOP_34_L, BIT(3)); // COMBP_GP_TOP_34[3]: reset dvi2miu
        W2BYTE(REG_COMBO_GP_TOP_34_L, 0);
    }
}

MS_U16 Hal_HDMI_GetTMDSFreq(void)
{
    return 0;
}

// Hal_HDMI_CheckHDMI20_Setting
//     - enInputPortType: INPUT_PORT_DVI0 / INPUT_PORT_DVI1 / INPUT_PORT_DVI2 / INPUT_PORT_DVI3
//     - Check input is over/under 3.4G and color format, and do the related setting
MS_BOOL Hal_HDMI_CheckHDMI20_Setting(E_MUX_INPUTPORT enInputPortType)
{
    return TRUE; // Always report 2P mode
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_StablePolling()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDMI_StablePolling(MS_U8 ucMHLSupportPath, stHDMI_POLLING_INFO *stHDMIPollingInfo)
{
    E_MUX_INPUTPORT enPortIndex = INPUT_PORT_DVI0;

    for(enPortIndex = INPUT_PORT_DVI0; enPortIndex <= INPUT_PORT_DVI_END; enPortIndex++)
    {
        if(!_Hal_tmds_CheckInputIsMHL(enPortIndex, ucMHLSupportPath))
        {
#if(!TMDS_POWER_SWITCH_IRQ_ENABLE)
            _Hal_tmds_PowerControlHandler(enPortIndex, stHDMIPollingInfo);
#endif

            if(_Hal_tmds_GetClockStableFlag(enPortIndex))
            {
                _Hal_tmds_CheckHDMI20Status(enPortIndex, stHDMIPollingInfo);
            }
            else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enPortIndex)].bTimingStableFlag)
            {
                stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enPortIndex)].bTimingStableFlag = FALSE;
            }
        }
        else if(!stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enPortIndex)].bNoInputFlag)
        {
            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enPortIndex)].bNoInputFlag = TRUE;
        }
        else if(stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enPortIndex)].bYUV420Flag)
        {
            _Hal_tmds_YUV420PHYSetting(enPortIndex, FALSE);

            stHDMIPollingInfo[HDMI_GET_PORT_SELECT(enPortIndex)].bYUV420Flag = FALSE;
        }

        _Hal_tmds_AutoEQFunctionProc(enPortIndex, stHDMIPollingInfo);
    }
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_Set5VDetectGPIOSelect()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDMI_Set5VDetectGPIOSelect(MS_U32 ul5VDetectGPIOIndex)
{
    ul5VDetectGPIOIndex = 0;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_SwitchVSDBtoHDRPacket()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDMI_SwitchVSDBtoHDRPacket(MS_BOOL bEnableHDR)
{
    bEnableHDR = FALSE;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDMI_Get_InfoFrame()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL Hal_HDMI_Get_InfoFrame(MS_U8 ucHDMIInfoSource, MS_HDMI_PACKET_STATE_t u8state, void *pData)
{
    MS_BOOL bRet = FALSE;
    MS_U8 uctemp = 0;
    MS_U16 usPacketContent = 0;

    if(ucHDMIInfoSource == HDMI_INFO_SOURCE0)
    {

    }

    switch(u8state)
    {
        case PKT_HDR:
            {
                MS_U16 *pInfFrame = (MS_U16*)pData;

                for(uctemp = 0; uctemp < HDMI_HDR_PACKET_SIZE; uctemp++)
                {
                    usPacketContent = R2BYTE(REG_HDMI2_DUAL_0_61_L +uctemp *2);

                    pInfFrame[uctemp] = (usPacketContent << 8) |(usPacketContent >> 8);
                }

                bRet = TRUE;
            }
            break;

        default:
            break;
    };

    return bRet;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_WriteX74()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL Hal_HDCP_WriteX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ubOffset, MS_U8 ubData)
{
    MS_U16 u16bank_offset = 0;
    MS_U16 delay = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0: u16bank_offset = 0x0000; break; // BK 0x171200
        case INPUT_PORT_DVI1: u16bank_offset = 0x0300; break; // BK 0x171500
        case INPUT_PORT_DVI2: u16bank_offset = 0x0600; break; // BK 0x171800
        case INPUT_PORT_DVI3: u16bank_offset = 0x0900; break; // BK 0x171B00
    }

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, BIT(10), BIT(10)); // [10]: HDCP enable for DDC
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(15), BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, ubOffset, BMASK(9:0)); // address
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(5), BIT(5)); // trigger latch address

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_18_L+u16bank_offset, ubData, BMASK(7:0)); // data
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(4), BIT(4)); // trigger latch data

    delay = PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
    while(R2BYTE(REG_HDCP_DUAL_P0_19_L+u16bank_offset) & BIT(7)); // wait write ready

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, 0, BIT(15)|BIT(14)); // [15]: CPU write disable, [14]: 0: 74 RAM, 1 :HDCP RAM

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_ReadX74()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 Hal_HDCP_ReadX74(E_MUX_INPUTPORT enInputPortType, MS_U8 ubOffset)
{
    MS_U16 u16bank_offset = 0;
    MS_U16 usRData = 0xFFFF;
    MS_U8 ucX74Data = 0xFF;
    MS_U16 delay = 0;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0: u16bank_offset = 0x0000; break; // BK 0x171200
        case INPUT_PORT_DVI1: u16bank_offset = 0x0300; break; // BK 0x171500
        case INPUT_PORT_DVI2: u16bank_offset = 0x0600; break; // BK 0x171800
        case INPUT_PORT_DVI3: u16bank_offset = 0x0900; break; // BK 0x171B00
    }

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, BIT(10), BIT(10)); // [10]: HDCP enable for DDC
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, 0, BIT(15)|BIT(14)); // [15]: CPU write enable, [14]: 0: 74 RAM, 1 :HDCP RAM

    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_17_L+u16bank_offset, ubOffset, BMASK(9:0)); // address
    HDCP_W2BYTEMSK(REG_HDCP_DUAL_P0_19_L+u16bank_offset, BIT(5), BIT(5)); // trigger latch address

    delay = PM_R2BYTE(TIMER_0_CNT_REG,BMASK(15:0));
    while(R2BYTE(REG_HDCP_DUAL_P0_19_L+u16bank_offset) & BIT(6)); // wait read ready

    usRData = HDCP_R2BYTEMSK(REG_HDCP_DUAL_P0_18_L+u16bank_offset, BMASK(15:0)); // read data
    ucX74Data = (MS_U8) ((usRData & 0xFF00) >> 8);

    return ucX74Data;
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_SetReady()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_SetReady(E_MUX_INPUTPORT enInputPortType, MS_BOOL bIsReady)
{
    MS_U8 ucBcaps = 0x00;

    ucBcaps = Hal_HDCP_ReadX74(enInputPortType, 0x40);//Read Bcaps

    if(bIsReady)
    {
        ucBcaps = ucBcaps | 0x20;
    }
    else
    {
        ucBcaps = ucBcaps & 0xDF;
    }

    Hal_HDCP_WriteX74(enInputPortType, 0x40, ucBcaps);
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_WriteKSVList()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_WriteKSVList(E_MUX_INPUTPORT enInputPortType, MS_U8* pucData, MS_U32 ulDataLen)
{
    MS_U8 ucPortIdx = 0;
    MS_U8 ucRegOffset = 0;
    MS_U32 ultemp = 0;
    MS_U32 dwBKOffset = 0;

    ucPortIdx = (MS_U8)(enInputPortType - INPUT_PORT_DVI0);

	  ucRegOffset = ucPortIdx * 0x04;
	  dwBKOffset = ucPortIdx * 0x300;

	  W2BYTEMSK(REG_COMBO_GP_TOP_40_L, BIT(2)| BIT(0), BMASK(2:0)); //enable XIU write
    W2BYTEMSK(REG_HDCPKEY_04_L + ucRegOffset, 0x00, 0x7FF); //set address

	  for(ultemp = 0; ultemp < ulDataLen; ultemp++)
    {
        W2BYTEMSK(REG_HDCPKEY_05_L + ucRegOffset, *(pucData + ultemp), BMASK(7:0));
    }

	  W2BYTEMSK(REG_HDCP_DUAL_P0_63_L + dwBKOffset, ulDataLen, 0x7FF); // write length

	  W2BYTEMSK(REG_HDCP_DUAL_P0_64_L + dwBKOffset, BIT(15), BIT(15)); // address pulse
    W2BYTEMSK(REG_HDCP_DUAL_P0_65_L + dwBKOffset, BIT(15) | BIT(14), BIT(15) | BIT(14));

    W2BYTEMSK(REG_COMBO_GP_TOP_40_L, 0, BMASK(2:0)); //disable XIU write
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_ClearStatus()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_ClearStatus(E_MUX_INPUTPORT enInputPortType, MS_U16 usInt)
{
    MS_U16 u16bank_offset;

    switch(enInputPortType)
    {
        default:
        case INPUT_PORT_DVI0:  u16bank_offset = 0x0000;  break; // BK 0x171200
        case INPUT_PORT_DVI1:  u16bank_offset = 0x0300;  break; // BK 0x171500
        case INPUT_PORT_DVI2:  u16bank_offset = 0x0600;  break; // BK 0x171800
        case INPUT_PORT_DVI3:  u16bank_offset = 0x0900;  break; // BK 0x171B00
    }

    HDCP_W2BYTE(REG_HDCP_DUAL_P0_01_L+u16bank_offset, usInt << 8);
}

//**************************************************************************
//  [Function Name]:
//                  Hal_HDCP_WriteDoneInterruptEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void Hal_HDCP_WriteDoneInterruptEnable(E_MUX_INPUTPORT enInputPortType, MS_BOOL bEnableIRQ)
{
    MS_U32 ulMACBankOffset = 0;

    switch(enInputPortType)
    {
        case INPUT_PORT_DVI0:
            ulMACBankOffset = 0x0000; // BK 0x171000

            //W2BYTEMSK(REG_COMBO_GP_TOP_38_L, bEnableIRQ? 0: BIT(3), BIT(3));

            break;

        case INPUT_PORT_DVI1:
            ulMACBankOffset = 0x0300; // BK 0x171300

            //W2BYTEMSK(REG_COMBO_GP_TOP_38_L, bEnableIRQ? 0: BIT(2), BIT(2));

            break;

        case INPUT_PORT_DVI2:
            ulMACBankOffset = 0x0600; // BK 0x171600

            //W2BYTEMSK(REG_COMBO_GP_TOP_38_L, bEnableIRQ? 0: BIT(1), BIT(1));

            break;

        case INPUT_PORT_DVI3:
            ulMACBankOffset = 0x0900; // BK 0x171900

            //W2BYTEMSK(REG_COMBO_GP_TOP_38_L, bEnableIRQ? 0: BIT(0), BIT(0));

            break;

        default:

            break;
    };

    W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_3B_L +ulMACBankOffset, bEnableIRQ? 0: BIT(14), BIT(14));
    W2BYTEMSK(REG_HDCP_DUAL_P0_15_L +ulMACBankOffset, bEnableIRQ? 0: BIT(15), BIT(15));
    W2BYTEMSK(REG_HDCP_DUAL_P0_67_L +ulMACBankOffset, bEnableIRQ? 0: BIT(3)| BIT(2), BIT(3)| BIT(2));
}

