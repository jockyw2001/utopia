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
// [mhal_mux.c]
// Date: 20090220
// Descriptions: Add a new mux layer for HW setting
//==============================================================================

#define  MHAL_MUX_C


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

// Common Definition
#include "MsCommon.h"
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
#if FRC_INSIDE
#include "mdrv_frc.h"
#include "mhal_frc.h"
#endif
#include "XC_private.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


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


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------


/******************************************************************************/
/// MUX
/******************************************************************************/
void Hal_SC_mux_set_dvi_mux(void *pInstance, MS_U8 PortId, E_OUTPUT_PORT_TYPE enDstPort)
{
    //DVI port 0: input A
    //DVI port 1: input B
    //DVI port 2: input D
    //DVI port 3: input C

    //to fix 0229636: DMS01221311 issue for clear frame packet
    if(((PortId==HAL_DVI_IP_A)||(PortId==HAL_DVI_IP_B)||(PortId==HAL_DVI_IP_C)||(PortId==HAL_DVI_IP_D))
       && ((enDstPort == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW) || (enDstPort == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1)))
    {
        MDrv_HDMI_pkt_reset_internal(pInstance, REST_HDMI_STATUS);
    }
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIsIMMESWITCH)
    {
        if(PortId==HAL_DVI_IP_A) // DVI port 0
        {
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING)
            {
                // DVI slowly update

                #if 0
                // Must turn off when the input port is at off-line status.
                W2BYTEMSK( REG_DVI_DTOP_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTEMSK( REG_DVI_DTOP_2A_L, 0xE3E3, HBMASK|LBMASK); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #else //Sync from SEC
                // Disable DVI update phase slowly for X5 X6 HDCP test
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTE(REG_DVI_DTOP_DUAL_P0_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #endif
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
            }
            // Setup display port
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, 0, BMASK(1:0)); // switch display port to port 0
        }
        else if(PortId==HAL_DVI_IP_B) // DVI port 1
        {
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING)
            {
                // DVI slowly update
                #if 0 // Disable DVI update phase slowly for X5 X6 HDCP test
                W2BYTEMSK( REG_DVI_DTOP1_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTEMSK( REG_DVI_DTOP1_2A_L, 0xE3E3, HBMASK|LBMASK); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #else //Sync from SEC
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTE(REG_DVI_DTOP_DUAL_P1_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #endif

                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
            }
            // Setup display port
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, BIT(0), BMASK(1:0)); // switch display port to port 1
        }
        else if(PortId==HAL_DVI_IP_C) // DVI port 2
        {
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING)
            {
                // DVI slowly update
                #if 0 // Disable DVI update phase slowly for X5 X6 HDCP test
                W2BYTEMSK( REG_DVI_DTOP2_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTEMSK( REG_DVI_DTOP2_2A_L, 0xE3E3, HBMASK|LBMASK); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #else //Sync from SEC
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTE(REG_DVI_DTOP_DUAL_P2_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #endif

                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
            }
            // Setup display port
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, BIT(1), BMASK(1:0)); // switch display port to port 2
        }
        else if(PortId==HAL_DVI_IP_D) // DVI port 3
        {
            if(pXCResourcePrivate->stdrvXC_MVideo_Context.g_bIMMESWITCH_DVI_POWERSAVING)
            {
                // DVI slowly update
                #if 0 // Disable DVI update phase slowly for X5 X6 HDCP test
                W2BYTEMSK( REG_DVI_DTOP3_29_L, 0xE300, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTEMSK( REG_DVI_DTOP3_2A_L, 0xE3E3, HBMASK|LBMASK); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #else //Sync from SEC
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_29_L, 0, HBMASK); // [15]:update Bch slowly; [14:8]: 0x63(100 lines)
                W2BYTE(REG_DVI_DTOP_DUAL_P3_2A_L, 0); // [15:8]: update Rch slowly, [7:0]:update Gch slowly
                #endif

                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_29_L, 0, BIT(15)); // [15]: turn off slowly updated
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2A_L, 0, BIT(15)|BIT(7)); // [15]: turn off slowly updated, [7]: turn off slowly updated
            }
            // Setup display port
            W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, BMASK(1:0), BMASK(1:0)); // switch display port to port 3
        }
    }

    else
    {
        //power and port control
        switch (PortId)
        {
            case HAL_DVI_IP_A: // DVI0 (port A)
                // RX clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_07_L, 0x00, 0xFF); // misc clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_11_L, 0x00, 0xFF); // P0 dec_hdcp clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_19_L, 0xFF, 0xFF); // P1 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_21_L, 0xFF, 0xFF); // P2 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_29_L, 0xFF, 0xFF); // P3 dec_hdcp clock disable

                // Enable RX Phy clk gen
                W2BYTE(REG_COMBO_PHY0_P0_0B_L, 0xFFFF); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P0_0C_L, 0x3FFF);
                W2BYTE(REG_COMBO_PHY0_P1_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P1_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P2_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P2_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P3_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P3_0C_L, 0x1008);

                // LDO
                W2BYTE(REG_COMBO_PHY1_P0_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P1_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P2_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P3_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc

                // Pd
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x0000, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);

                // DVI port enable
                W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, 0, BMASK(1:0)); // switch display port to port 0

                break;
            case HAL_DVI_IP_B: // DVI1 (port B)
                // RX clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_07_L, 0x00, 0xFF); // misc clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_11_L, 0xFF, 0xFF); // P0 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_19_L, 0x00, 0xFF); // P1 dec_hdcp clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_21_L, 0xFF, 0xFF); // P2 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_29_L, 0xFF, 0xFF); // P3 dec_hdcp clock disable

                // Enable RX Phy clk gen
                W2BYTE(REG_COMBO_PHY0_P0_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P0_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P1_0B_L, 0xFFFF); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P1_0C_L, 0x3FFF);
                W2BYTE(REG_COMBO_PHY0_P2_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P2_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P3_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P3_0C_L, 0x1008);

                // LDO
                W2BYTE(REG_COMBO_PHY1_P0_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P1_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P2_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P3_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc

                // Pd
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x0000, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);

                // DVI port enable
                W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, BIT(0), BMASK(1:0)); // switch display port to port 1

                break;
            case HAL_DVI_IP_C: // DVI2 (port C)
                // RX clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_07_L, 0x00, 0xFF); // misc clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_11_L, 0xFF, 0xFF); // P0 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_19_L, 0xFF, 0xFF); // P1 dec_hdcp clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_21_L, 0x00, 0xFF); // P2 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_29_L, 0xFF, 0xFF); // P3 dec_hdcp clock disable

                // Enable RX Phy clk gen
                W2BYTE(REG_COMBO_PHY0_P0_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P0_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P1_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P1_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P2_0B_L, 0xFFFF); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P2_0C_L, 0x3FFF);
                W2BYTE(REG_COMBO_PHY0_P3_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P3_0C_L, 0x1008);

                // LDO
                W2BYTE(REG_COMBO_PHY1_P0_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P1_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P2_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P3_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc

                // Pd
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x0000, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);

                // DVI port enable
                W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, BIT(1), BMASK(1:0)); // switch display port to port 2

                break;
            case HAL_DVI_IP_D: // DVI3 (port D)
                // RX clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_07_L, 0x00, 0xFF); // misc clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_11_L, 0xFF, 0xFF); // P0 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_19_L, 0xFF, 0xFF); // P1 dec_hdcp clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_21_L, 0xFF, 0xFF); // P2 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_29_L, 0x00, 0xFF); // P3 dec_hdcp clock disable

                // Enable RX Phy clk gen
                W2BYTE(REG_COMBO_PHY0_P0_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P0_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P1_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P1_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P2_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P2_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P3_0B_L, 0xFFFF); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P3_0C_L, 0x3FFF);

                // LDO
                W2BYTE(REG_COMBO_PHY1_P0_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P1_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P2_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P3_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc

                // Pd
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x0000, 0x3FFF);

                // DVI port enable
                W2BYTEMSK(REG_HDMI2_DUAL_0_50_L, BMASK(1:0), BMASK(1:0)); // switch display port to port 3
                break;
            default: // turn off all DVI path
                // RX clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_07_L, 0x00, 0xFF); // misc clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_11_L, 0xFF, 0xFF); // P0 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_19_L, 0xFF, 0xFF); // P1 dec_hdcp clock enable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_21_L, 0xFF, 0xFF); // P2 dec_hdcp clock disable
                MDrv_WriteByteMask(REG_COMBO_GP_TOP_29_L, 0xFF, 0xFF); // P3 dec_hdcp clock disable

                // Enable RX Phy clk gen
                W2BYTE(REG_COMBO_PHY0_P0_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P0_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P1_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P1_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P2_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P2_0C_L, 0x1008);
                W2BYTE(REG_COMBO_PHY0_P3_0B_L, 0x0001); // Clock enable
                W2BYTE(REG_COMBO_PHY0_P3_0C_L, 0x1008);

                // LDO
                W2BYTE(REG_COMBO_PHY1_P0_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P1_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P2_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc
                W2BYTE(REG_COMBO_PHY1_P3_00_L, 0x0007);// [3]: reg_atop_pd_ldo; [2:0]: reg_atop_pd_osc

                // Pd
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0x2FFF, 0x3FFF);
                break;
        }
    }
}

void Hal_SC_mux_set_analog_mux(MS_U8 PortId)
{
    // 0x2501
    //W2BYTEMSK(REG_ADC_ATOP_01_L, PortId & 0x03, 0x03);
    return;
}

void Hal_SC_mux_set_adc_y_mux(void *pInstance, MS_U8 PortId)
{
    UNUSED(pInstance);
    UNUSED(PortId);
    // ADC VD Ymux
    //W2BYTEMSK(REG_ADC_ATOP_02_L, PortId , BITMASK(3:0) );
    return;
}

void Hal_SC_mux_set_adc_c_mux(void *pInstance, MS_U8 PortId)
{
    UNUSED(pInstance);
    UNUSED(PortId);
    // ADC VD Cmux
    //W2BYTEMSK(REG_ADC_ATOP_02_L, (PortId<<4), BITMASK(7:4) ); // ADC_VD_CMUX_MASK
    return;
}

void Hal_SC_mux_set_mainwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    ///pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt = 0;
    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        MS_U16 u16Delayms = 0;
        while((pXCResourcePrivate->sthal_Optee.op_tee_mux.bfire == TRUE) && u16Delayms < 200)
        {
            MsOS_DelayTask(1);
            u16Delayms++;
        }
        if (pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt <=OP_TEE_XC_REG_MAX_CNT)
        {
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].bk = REG_IPMUX_01_L;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].value = (u8Data_Mux << 4) | (MDrv_ReadRegBit(REG_IPMUX_01_L, 0x0F));
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].mask = 0xFF;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt++;
        }
        else
        {
            printf("op_tee_mux.regs_cnt too small\n");
        }
    }
    MDrv_WriteByteMask(REG_IPMUX_01_L, u8Data_Mux << 4, 0xF0);
    MDrv_WriteByteMask(REG_CKG_IDCLK2, u8Clk_Mux << 2, CKG_IDCLK2_MASK);
}

MS_BOOL Hal_SC_mux_get_mainwin_ip_mux(void *pInstance, MS_U8 *pU8Data_Mux, MS_U8 *pU8Clk_Mux)
{
    MS_BOOL bRet = FALSE;
    if(pU8Data_Mux && pU8Clk_Mux)
    {
        *pU8Data_Mux = (MDrv_ReadRegBit(REG_IPMUX_01_L, 0xF0)) >> 4;
        *pU8Clk_Mux = (MDrv_ReadRegBit(REG_CKG_IDCLK2, CKG_IDCLK2_MASK)) >> 2;
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }
    return bRet;
}

void Hal_SC_set_subwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{
    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    ///pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt = 0;
    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        MS_U16 u16Delayms = 0;
        while((pXCResourcePrivate->sthal_Optee.op_tee_mux.bfire == TRUE) && u16Delayms < 200)
        {
            MsOS_DelayTask(1);
            u16Delayms++;
        }
        if (pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt <=OP_TEE_XC_REG_MAX_CNT)
        {
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].bk = REG_IPMUX_01_L;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].value = (u8Data_Mux | ((MDrv_ReadRegBit(REG_IPMUX_01_L, 0xF0))<<4));
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].mask = 0xFF;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt++;
        }
        else
        {
            printf("op_tee_mux.regs_cnt too small\n");
        }
    }
    MDrv_WriteByteMask(REG_IPMUX_01_L, u8Data_Mux, 0x0F);
    MDrv_WriteByteMask(REG_CKG_IDCLK1, u8Clk_Mux << 2, CKG_IDCLK1_MASK);
}

void Hal_SC_mux_set_dipwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{
    MDrv_WriteByteMask(REG_IPMUX_02_L, u8Data_Mux << 4, 0xF0);
    MDrv_WriteByteMask(REG_CKG_IDCLK3, u8Clk_Mux << 2, CKG_IDCLK3_MASK);
}

void Hal_SC2_mux_set_mainwin_ip_mux(void *pInstance, MS_U8 u8Data_Mux, MS_U8 u8Clk_Mux)
{

    printf("[%s][%d]U8Data_Mux=%x, U8Clk_Mux=%x\n", __FUNCTION__, __LINE__, u8Data_Mux, u8Clk_Mux);

    XC_INSTANCE_PRIVATE *psXCInstPri = NULL;
    UtopiaInstanceGetPrivate(pInstance, (void**)&psXCInstPri);
    XC_RESOURCE_PRIVATE* pXCResourcePrivate = NULL;
    UtopiaResourceGetPrivate(g_pXCResource[_XC_SELECT_INTERNAL_VARIABLE(psXCInstPri->u32DeviceID)],(void**)(&pXCResourcePrivate));
    ////pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt = 0;
    if ((pXCResourcePrivate->sthal_Optee.op_tee_xc[MAIN_WINDOW].isEnable == TRUE) ||
        (pXCResourcePrivate->sthal_Optee.op_tee_xc[SUB_WINDOW].isEnable == TRUE))
    {
        MS_U16 u16Delayms = 0;
        while((pXCResourcePrivate->sthal_Optee.op_tee_mux.bfire == TRUE) && u16Delayms < 200)
        {
            MsOS_DelayTask(1);
            u16Delayms++;
        }
        if (pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt <=OP_TEE_XC_REG_MAX_CNT)
        {
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].bk = REG_IPMUX_61_L;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].value = (u8Data_Mux << 4);
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_data[pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt].mask = 0xF0;
            pXCResourcePrivate->sthal_Optee.op_tee_mux.regs_cnt++;
        }
        else
        {
            printf("op_tee_mux.regs_cnt too small\n");
        }
    }
    MDrv_WriteByteMask(REG_IPMUX_61_L, u8Data_Mux << 4, 0xF0);
    MDrv_WriteByteMask(REG_CKG_S2_IDCLK2, u8Clk_Mux << 2, CKG_S2_IDCLK2_MASK);
}

MS_BOOL Hal_SC2_mux_get_mainwin_ip_mux(void *pInstance, MS_U8 *pU8Data_Mux, MS_U8 *pU8Clk_Mux)
{
    MS_BOOL bRet = FALSE;

    if(pU8Data_Mux && pU8Clk_Mux)
    {
        *pU8Data_Mux = (MDrv_ReadRegBit(REG_IPMUX_61_L, 0xF0)) >> 4;
        *pU8Clk_Mux = (MDrv_ReadRegBit(REG_CKG_S2_IDCLK2, CKG_S2_IDCLK2_MASK)) >> 2;
        bRet = TRUE;
    }
    else
    {
        bRet = FALSE;
    }

    return bRet;
}

// Used for backward compatibility
void Hal_SC_set_sync_port_by_dataport(void *pInstance, E_MUX_INPUTPORT src_port )
{
    E_MUX_INPUTPORT sync_port;
    switch ( src_port)
    {
        case INPUT_PORT_ANALOG0:
            sync_port = INPUT_PORT_ANALOG0_SYNC;
            break;

        case INPUT_PORT_ANALOG1:
            sync_port = INPUT_PORT_ANALOG1_SYNC;
            break;

        case INPUT_PORT_ANALOG2:
            sync_port = INPUT_PORT_ANALOG2_SYNC;
            break;
        default:
            sync_port = INPUT_PORT_ANALOG0_SYNC;
    }
        // Src -> ( IPmux )
        Hal_ADC_set_mux(pInstance, sync_port);
}
/******************************************************************************/
///Dispatch Port
///@param src \b IN: source type
///@param dest \b IN: pixel clock
/******************************************************************************/
void Hal_SC_mux_dispatch(void *pInstance, E_MUX_INPUTPORT src_port , E_MUX_OUTPUTPORT dest_port)
{
    MS_U8 _src = (MS_U8) src_port;
    MS_U8 _dest = (MS_U8) dest_port;

    // Hal level in T2 is following MsCommon.h directly.
    // It is unnecessary to add a mapping function.
    E_INPUT_PORT_TYPE src = (E_INPUT_PORT_TYPE) _src;
    E_OUTPUT_PORT_TYPE dest = (E_OUTPUT_PORT_TYPE) _dest;

    printf("[%s][%d]src_port=%u, dest_port=%u\n", __FUNCTION__, __LINE__, src_port , dest_port);

    // If src is sync port. set adc only.
    if ( src >= HAL_INPUTPORT_ANALOG_SYNC0 && src < (HAL_INPUTPORT_ANALOG_SYNC0 + NUMBER_OF_ANALOG_PORT ) )
    {
        Hal_ADC_set_mux(pInstance, src_port);
    }

    // Src = Analog 0ort 0 ~ Max number of analog port
    else if ( src >= HAL_INPUTPORT_ANALOG0  && src < (HAL_INPUTPORT_ANALOG0 + NUMBER_OF_ANALOG_PORT ))
    {

        // Src -> ( IPmux )
        Hal_ADC_set_mux(pInstance, src_port);

        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_ADC_A, SC_CLK_ADC_A);
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_ADC_A, SC_CLK_ADC_A);
        }
        else if (dest == HAL_OUTPUTPORT_DIP_WINDOW)
        {
            Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_ADC_A, SC_CLK_ADC_A);
        }
        else if ( dest == HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW )
        {
            Hal_SC2_mux_set_mainwin_ip_mux(pInstance, SC2_MAINWIN_IPMUX_ADC_A, SC2_CLK_ADC_A);
        }
        else
        {
            //Undefined
        }

    }

    // Src = Y mux port 0 ~ Max number of Y mux port
    else if (  src >= HAL_INPUTPORT_YMUX_CVBS0 && src < (HAL_INPUTPORT_YMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_VD, SC_CLK_VD);
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_VD , SC_CLK_VD);
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
        }
        else if ( dest == HAL_OUTPUTPORT_CVBS1 )
        {
            Hal_ADC_set_CVBSOMux(pInstance, E_ADC_CVBSO_PORT_1, src);
        }
        else if ( dest == HAL_OUTPUTPORT_CVBS2 )
        {
            Hal_ADC_set_CVBSOMux(pInstance, E_ADC_CVBSO_PORT_2, src);
        }
        else if (dest == HAL_OUTPUTPORT_DIP_WINDOW)
        {
            Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_VD, SC_CLK_VD);
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
        }
        else if ( dest == HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW )
        {
            Hal_SC2_mux_set_mainwin_ip_mux(pInstance, SC2_MAINWIN_IPMUX_VD, SC2_CLK_VD);
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
        }
        else
        {
            Hal_ADC_set_mux(pInstance, src_port);
        }
    }

    // Src = C mux port 0 ~ Max number of C mux port
    else if ( src >= HAL_INPUTPORT_CMUX_CVBS0 && src < (HAL_INPUTPORT_CMUX_CVBS0 + NUMBER_OF_CVBS_PORT) )
    {
        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
            ; // Controlled by Ymux
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
            ; // Controlled by Ymux
        }
        else if ( dest == HAL_OUTPUTPORT_CVBS1 )
        {
            Hal_ADC_set_CVBSOMux(pInstance, E_ADC_CVBSO_PORT_1, src);
        }
        else if ( dest == HAL_OUTPUTPORT_CVBS2 )
        {
            Hal_ADC_set_CVBSOMux(pInstance, E_ADC_CVBSO_PORT_2, src);
        }
        else if ( dest == HAL_OUTPUTPORT_DIP_WINDOW )
        {
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
            // Controlled by Ymux
        }
		else if ( dest == HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW )
		{
			// Src -> ( IPmux )
			Hal_ADC_set_mux(pInstance, src_port);
			; // Controlled by Ymux
		}
        else
        {
            // Src -> ( IPmux )
            Hal_ADC_set_mux(pInstance, src_port);
            //Undefined
        }

    }

    // Src = DVI port 0 ~ Max number of DVI port
    else if ( src >= HAL_INPUTPORT_DVI0 && src < (HAL_INPUTPORT_DVI0 + NUMBER_OF_DVI_PORT) )
    {
        printf("[%s][%d]\n", __FUNCTION__, __LINE__);
        // Src -> ( IPmux )
        Hal_SC_mux_set_dvi_mux(pInstance, src - HAL_INPUTPORT_DVI0, dest); //alex_tung

        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_HDMI_DVI, SC_CLK_HDMI_DVI);
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_HDMI_DVI, SC_CLK_HDMI_DVI);
        }
        else if (dest == HAL_OUTPUTPORT_DIP_WINDOW )
        {
            Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_HDMI_DVI , SC_CLK_HDMI_DVI);
        }
        else if ( dest == HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW )
        {
            Hal_SC2_mux_set_mainwin_ip_mux(pInstance, SC2_MAINWIN_IPMUX_HDMI_DVI, SC2_CLK_HDMI_DVI);
        }
        else
        {
            // Undefeined
        }
    }

    else if ( src >= HAL_INPUTPORT_MVOP && src < (HAL_INPUTPORT_MVOP + NUMBER_OF_MVOP_PORT) )
    {
        // Src -> ( IPmux )
        // No source mux need to set in T2 (only one MVD).

        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            if (src == HAL_INPUTPORT_MVOP)
                Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_MVOP, SC_CLK_DC0);
            else if (src == HAL_INPUTPORT_MVOP2)
                Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_MVOP2, SC_CLK_DC1);

        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            if (src == HAL_INPUTPORT_MVOP)
                Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_MVOP , SC_CLK_DC0);
            else if (src == HAL_INPUTPORT_MVOP2)
                Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_MVOP2 , SC_CLK_DC1);

        }
        else if (dest == HAL_OUTPUTPORT_DIP_WINDOW )
        {
            if (src == HAL_INPUTPORT_MVOP)
                Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_MVOP , SC_CLK_DC0);
            else if (src == HAL_INPUTPORT_MVOP2)
                Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_MVOP2 , SC_CLK_DC1);
        }
        else if ( dest == HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW )
        {
            if (src == HAL_INPUTPORT_MVOP)
                Hal_SC2_mux_set_mainwin_ip_mux(pInstance, SC2_MAINWIN_IPMUX_MVOP, SC2_CLK_DC0);
            else if (src == HAL_INPUTPORT_MVOP2)
                Hal_SC2_mux_set_mainwin_ip_mux(pInstance, SC2_MAINWIN_IPMUX_MVOP2, SC2_CLK_DC1);

        }
        else
        {
            // Undefeined ( set in ADC table )
        }

    }
    else if ( src >= HAL_INPUTPORT_SCALER_OP && src < (HAL_INPUTPORT_SCALER_OP + NUMBER_OF_SCALER_OP_PORT) )
    {
        // Src -> ( IPmux )

        // IPmux -> Dest
        if ( dest == HAL_OUTPUTPORT_SCALER_MAIN_WINDOW )
        {
            Hal_SC_mux_set_mainwin_ip_mux(pInstance, SC_MAINWIN_IPMUX_CAPTURE, SC_CLK_ODCLK);
        }
        else if (dest == HAL_OUTPUTPORT_SCALER_SUB_WINDOW1 )
        {
            Hal_SC_set_subwin_ip_mux(pInstance, SC_SUBWIN_IPMUX_CAPTURE, SC_CLK_ODCLK);
        }
        else if (dest == HAL_OUTPUTPORT_DIP_WINDOW )
        {
            Hal_SC_mux_set_dipwin_ip_mux(pInstance, SC_DWIN_IPMUX_CAPTURE , SC_CLK_ODCLK);
        }
        else if ( dest == HAL_OUTPUTPORT_SCALER2_MAIN_WINDOW )
        {
            Hal_SC2_mux_set_mainwin_ip_mux(pInstance, SC2_MAINWIN_IPMUX_CAPTURE, SC2_CLK_ODCLK);
        }
        else
        {
            // Undefeined
        }
    }
}
void Hal_SC_EnableCLK_for_DIP(void *pInstance, MS_BOOL bEnable)
{
    // Enable DIP window clock
     if(bEnable)
     {
         // Enable IDClk3
         MDrv_WriteRegBit(REG_CKG_IDCLK3, DISABLE, CKG_IDCLK3_INVERT);                   // Not Invert
         MDrv_WriteRegBit(REG_CKG_IDCLK3, DISABLE, CKG_IDCLK3_GATED);                    // Enable clock
     }
     else
     {
         // Disable IDClk3
         MDrv_WriteRegBit(REG_CKG_IDCLK3, ENABLE, CKG_IDCLK3_INVERT);                   // Not Invert
         MDrv_WriteRegBit(REG_CKG_IDCLK3, ENABLE, CKG_IDCLK3_GATED);                    // Enable clock
     }
}


#undef MHAL_MUX_C
