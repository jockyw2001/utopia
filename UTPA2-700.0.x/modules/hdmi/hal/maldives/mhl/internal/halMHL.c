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
///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    mhal_mhl.c
/// @author MStar Semiconductor Inc.
/// @brief  MHL driver Function
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MHL_C_
#define _MHAL_MHL_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsVersion.h"
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
#include "MsIRQ.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"
#include "mhl_hwreg_utility2.h"
#include "hwreg_pm_sleep.h"
#include "hwreg_hdmi.h"
#include "hwreg_hdcp.h"
#include "hwregMHL.h"
#include "mdrv_mhl_st.h"
#include "halMHL.h"

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define msg_mhl(x) x

#define DMHLInit    0

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U32 addr;
    MS_U16 mask;
    MS_U16 databuf;
} msLoadTbl_S;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL bMHLSignalStable = FALSE;
MS_U8 ucMHLSupportPort = E_MUX_NOT_SUPPORT_MHL;
MS_U8 ucIControlValue = MHL_ICONTROL_VALUE;
MS_U8 ucImpedanceValue = MHL_IMPEDANCE_VALUE;
MS_U8 ucChipIDValue = MHL_CHIP_ID_MUNICH;

//-------------------------------------------------------------------------------------------------
//  MHL initial table
//-------------------------------------------------------------------------------------------------
static msLoadTbl_S tMHL_INITIAL_TABLE[] =
{
    // Set accepted discover pulse high pulse width to ignore USB pulse
    {REG_PM_MHL_CBUS_0C, BMASK(15:0), 0x0046}, // reject cbus discovery pulse below this limit
    {REG_PM_MHL_CBUS_0D, BMASK(15:0), 0x0082}, // reject cbus discovery pulse above this limit
    // CTS 4.3.7.1
    {REG_MHL_CBUS_55, BMASK(7:4), BIT(4)}, // [7:4]: cbus requester transmit opportunity after arbitration
    {REG_MHL_CBUS_70, BIT(14), BIT(14)}, // [14]: let edid current read initial address add 1

    {REG_PM_MHL_CBUS_21, BIT(1), BIT(1)}, // [1]: output mhl_zxsense_tmds inv

    // For CTS 6.3.10.7
    {REG_MHL_CBUS_59, BMASK(15:0), 0xADB0}, // timeout for a device receiving a packet within a command
    {REG_MHL_CBUS_5A, BMASK(15:0), 0x0001}, // modify to 110ms

    {REG_PM_MHL_CBUS_01, BMASK(5:2), 0}, // Not overwrite pull down resistor
    
#if(MHL_CBUS_OPERATION_MODE >= MHL_CBUS_HW_ISR_MODE)
    {REG_MHL_CBUS_23, BMASK(10:8) |BIT(4), BIT(10) |BIT(4)}, // [10]: enable HW auto response read_devcap request
    {REG_MHL_CBUS_6D, BMASK(11:0), BMASK(11:0)}, // Enable MHL HW mode
    {REG_MHL_CBUS_71, BMASK(15:14), BMASK(15:14)},
    {REG_MHL_CBUS_50, BIT(13), BIT(13)}, // [13]: MSC send command keep mode
#else
    {REG_MHL_CBUS_23, BMASK(10:8) |BIT(4), 0}, // [10]: enable HW auto response read_devcap request
    {REG_MHL_CBUS_6D, BMASK(11:0), 0}, // Enable MHL HW mode
    {REG_MHL_CBUS_71, BMASK(15:14), 0},
    {REG_MHL_CBUS_50, BIT(13), 0}, // [13]: MSC send command keep mode
#endif

#if(MHL_CBUS_OPERATION_MODE == MHL_CBUS_HW_REPLY_MODE)
    {REG_MHL_CBUS_0B, BIT(14), BIT(14)}, // [14]: MSC send command keep mode
#endif

#if DMHL_INT_ENABLE
    // Mask unused interrupt events
    {REG_PM_MHL_CBUS_16, BIT(13)|BIT(9), BIT(13)|BIT(9)}, // [13]: lnk_lay_en_chg_int mask, [9]: conn_state_chg_int mask
    {REG_PM_MHL_CBUS_18, BIT(9)|BIT(5)|BIT(1), BIT(9)|BIT(5)|BIT(1)}, // [9]: cbus_discover_pul_confirm_int mask, [5]: wakeup_pul_confirm_int mask, [1]: cbus_stuck_to_low_int mask.
    {REG_PM_MHL_CBUS_30, BIT(1), BIT(1)}, // [1]: cbus conflict_int mask
    {REG_PM_MHL_CBUS_38, BIT(13)|BIT(9)|BIT(5)|BIT(1), BIT(13)|BIT(9)|BIT(5)|BIT(1)}, // [13]: unused mask, [9]: unused mask, [5]: unused mask, [1]: unused mask.
    {REG_MHL_CBUS_14, BIT(13), BIT(13)}, // [13]: int mask for monitor_sram_full
    {REG_MHL_CBUS_18, BIT(13), BIT(13)}, // [13]: send rcv_pkt_ddc_sw_overwrite_err_in mask
    {REG_MHL_CBUS_19, BIT(5)|BIT(1), BIT(5)|BIT(1)}, // [5]: send ddc send complete interrupt mask, [1]: send ddc error interrupt mask
    {REG_MHL_CBUS_1B, BIT(1), BIT(1)}, // [1]: receive ddc packet valid mask
    {REG_MHL_CBUS_1F, BIT(5)|BIT(1), BIT(5)|BIT(1)}, // [5]: ddc access edid timeout int mask, [1]: client_wrt_ddc_ram interrupt mask
    {REG_MHL_CBUS_21, BIT(5), BIT(5)}, // [5]: ddc error interrupt mask
    {REG_MHL_CBUS_22, BIT(9)|BIT(5)|BIT(1), BIT(9)|BIT(5)|BIT(1)}, // [9]: ddc access hdcp timeout int mask, [5]: receive nack pkt int_mask, [1]: receive abort pkt int mask
    {REG_MHL_CBUS_23, BIT(13), BIT(13)}, // [13]: send rcv_pkt_msc_sw_overwrite_err_in mask
    {REG_MHL_CBUS_24, BIT(1), BIT(1)}, // [1]: send error interrupt mask
    {REG_MHL_CBUS_25, BIT(9)|BIT(5)|BIT(1), BIT(9)|BIT(5)|BIT(1)}, // [9]: response_pkt_msc_hw_int mask, [5]: snd_pkt_msc_hw_int mask, [1]: msc sw send complete interrupt mask
    {REG_MHL_CBUS_63, BIT(9), BIT(9)}, // [9]: dytycycle_bad_int mask
    {REG_MHL_CBUS_65, BIT(9)|BIT(5)|BIT(1), BIT(9)|BIT(5)|BIT(1)}, // [9]: rcv_parity_err_int mask, [5]: rcv_data_err_int mask, [1]: rcv_sync_err_int mask
    {REG_MHL_CBUS_78, BIT(13)|BIT(9)|BIT(5)|BIT(1), BIT(13)|BIT(9)|BIT(5)|BIT(1)}, // [13]: unused mask, [9]: unused mask, [5]: unused mask, [1]: unused mask.
#endif
};

//-------------------------------------------------------------------------------------------------
//  MHL power on table
//-------------------------------------------------------------------------------------------------
static msLoadTbl_S tMHL_POWER_ON_TABLE[] =
{
    {REG_PM_MHL_CBUS_01, BMASK(5:2), 0}, // Not overwrite pull down resistor
};

//-------------------------------------------------------------------------------------------------
//  MHL power down table
//-------------------------------------------------------------------------------------------------
static msLoadTbl_S tMHL_POWER_DOWN_TABLE[] =
{
    {REG_PM_MHL_CBUS_01, BMASK(5:2), BIT(5)|BMASK(3:2)},
};

//-------------------------------------------------------------------------------------------------
//  MHL power saving table
//-------------------------------------------------------------------------------------------------
static msLoadTbl_S tMHL_POWER_SAVING_TABLE[] =
{
    {REG_PM_MHL_CBUS_01, BMASK(5:2), BIT(5)|BMASK(3:2)},
};

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_IsCbusBusy()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mhal_mhl_IsCbusBusy(void)
{
    return ((R2BYTE(REG_MHL_CBUS_5D) & 0x00F0) != 0 ? TRUE : FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_DviAutoEQSwitch()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_DviAutoEQSwitch(MS_U8 ucCbusSelect, MS_BOOL bFlag)
{
    static MS_BOOL bAutoEQFlag = FALSE;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(!bAutoEQFlag)
                {
                    if((R2BYTE(REG_DVI_DTOP_2F_L) &BIT(0)) == BIT(0))
                    {
                        bAutoEQFlag = TRUE;
                    }
                }
                
                if(bAutoEQFlag)
                {
                    if(bFlag)
                    {
                        W2BYTEMSK(REG_DVI_DTOP_00_L, BIT(4), BIT(4)); // enable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP_00_L, 0, BIT(4)); // disable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP_2F_L, 0, BIT(0)); // disable autoEQ controller
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                if(!bAutoEQFlag)
                {
                    if((R2BYTE(REG_DVI_DTOP1_2F_L) &BIT(0)) == BIT(0))
                    {
                        bAutoEQFlag = TRUE;
                    }
                }
                
                if(bAutoEQFlag)
                {
                    if(bFlag)
                    {
                        W2BYTEMSK(REG_DVI_DTOP1_00_L, BIT(4), BIT(4)); // enable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP1_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP1_00_L, 0, BIT(4)); // disable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP1_2F_L, 0, BIT(0)); // disable autoEQ controller
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(!bAutoEQFlag)
                {
                    if((R2BYTE(REG_DVI_DTOP3_2F_L) &BIT(0)) == BIT(0))
                    {
                        bAutoEQFlag = TRUE;
                    }
                }
                
                if(bAutoEQFlag)
                {
                    if(bFlag)
                    {
                        W2BYTEMSK(REG_DVI_DTOP3_00_L, BIT(4), BIT(4)); // enable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP3_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP3_00_L, 0, BIT(4)); // disable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP3_2F_L, 0, BIT(0)); // disable autoEQ controller
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(!bAutoEQFlag)
                {
                    if((R2BYTE(REG_DVI_DTOP2_2F_L) &BIT(0)) == BIT(0))
                    {
                        bAutoEQFlag = TRUE;
                    }
                }
                
                if(bAutoEQFlag)
                {
                    if(bFlag)
                    {
                        W2BYTEMSK(REG_DVI_DTOP2_00_L, BIT(4), BIT(4)); // enable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP2_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                    }
                    else
                    {
                        W2BYTEMSK(REG_DVI_DTOP2_00_L, 0, BIT(4)); // disable EQ new mode
                        W2BYTEMSK(REG_DVI_DTOP2_2F_L, 0, BIT(0)); // disable autoEQ controller
                    }
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_HdmiBypassModeSetting()
//  [Description]
//                  MHL HDMI bypass setting
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_HdmiBypassModeSetting(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_DVI_DTOP_27_L, 0, BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_DTOP_20_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP_0E_L, BIT(4), BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP_3B_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP_3D_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP_3F_L, 0, BIT(4)); // overwirte enable
                W2BYTE(REG_DVI_DTOP_30_L, 0);
                W2BYTEMSK(REG_DVI_DTOP_2F_L, 0, BMASK(15:4)| BMASK(3:2));
                W2BYTEMSK(REG_DVI_ATOP_32_L, BIT(8), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP_38_L, 0, BIT(10) |BIT(8)); // [10]: auto acdr mode selection, [8]: enable auto acdr function
                W2BYTEMSK(REG_DVI_ATOP_5E_L, 0, BMASK(4:0)); //[4]: overwrtie DPL ICTL value, [3:0]: ictl overwrtie value
                W2BYTEMSK(REG_DVI_DTOP_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_HDCP_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP_0C_L, MHL_IMPEDANCE_VALUE, BIT(9)| BMASK(3:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                W2BYTEMSK(REG_DVI_DTOP1_27_L, 0, BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_DTOP1_20_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP1_0E_L, BIT(4), BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP1_3B_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP1_3D_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP1_3F_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_ATOP1_32_L, BIT(8), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP1_5E_L, 0, BMASK(4:0)); //[4]: overwrtie DPL ICTL value, [3:0]: ictl overwrtie value
                W2BYTEMSK(REG_DVI_ATOP1_60_L, 0, BIT(7)); // power on DVI PLL
                W2BYTEMSK(REG_DVI_DTOP1_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_ATOP1_60_L, 0, BMASK(2:1)); // [2:0]: power down RD
                W2BYTEMSK(REG_DVI_ATOP1_74_L, 0, BMASK(5:0)); // [2:0]: power down DPLPHI, [5:3]: power down DPLPHQ
                W2BYTEMSK(REG_HDCP1_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP1_0C_L, MHL_IMPEDANCE_VALUE, BIT(9) |BMASK(3:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_DVI_DTOP3_27_L, 0, BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_DTOP3_20_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP3_0E_L, BIT(4), BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP3_3B_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP3_3D_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP3_3F_L, 0, BIT(4)); // overwirte enable
                W2BYTE(REG_DVI_DTOP3_30_L, 0);
                W2BYTEMSK(REG_DVI_DTOP3_2F_L, 0, BMASK(15:4)| BMASK(3:2));
                W2BYTEMSK(REG_DVI_ATOP3_32_L, BIT(8), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP3_35_L, 0, BIT(10) |BIT(8)); // [10]: auto acdr mode selection, [8]: enable auto acdr function
                W2BYTEMSK(REG_DVI_ATOP3_5E_L, 0, BMASK(4:0)); //[4]: overwrtie DPL ICTL value, [3:0]: ictl overwrtie value
                W2BYTEMSK(REG_DVI_DTOP3_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_HDCP3_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP3_0C_L, MHL_IMPEDANCE_VALUE, BIT(9)| BMASK(3:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_DVI_DTOP2_27_L, 0x2C6C, BMASK(14:0)); // [7]: MHL HW mode, [1]: MHL pack-pixel mode, [0]: MHL enable
                W2BYTEMSK(REG_DVI_DTOP2_20_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP2_0E_L, BIT(4), BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP2_3B_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP2_3D_L, 0, BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP2_3F_L, 0, BIT(4)); // overwirte enable
                W2BYTE(REG_DVI_DTOP2_30_L, 0);
                W2BYTEMSK(REG_DVI_DTOP2_2F_L, 0, BMASK(15:4)| BMASK(3:2));
                W2BYTEMSK(REG_DVI_ATOP2_32_L, BIT(8), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP2_35_L, 0, BIT(10) |BIT(8)); // [10]: auto acdr mode selection, [8]: enable auto acdr function
                W2BYTEMSK(REG_DVI_ATOP2_5E_L, 0, BMASK(4:0)); //[4]: overwrtie DPL ICTL value, [3:0]: ictl overwrtie value
                W2BYTEMSK(REG_DVI_DTOP2_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_HDCP2_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP2_0C_L, MHL_IMPEDANCE_VALUE, BIT(9) |BMASK(3:0));
                }
            }
            
            break;
#endif

        default:
        
            break;
    };

    _mhal_mhl_DviAutoEQSwitch(ucCbusSelect, TRUE);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_Mhl24bitsModeSetting()
//  [Description]
//                  MHL 24 bits mode setting
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_Mhl24bitsModeSetting(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_DVI_DTOP_27_L, BIT(2), BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_DTOP_20_L, MHL_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP_3A_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP_3B_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP_3C_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP_3D_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP_3E_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP_3F_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTE(REG_DVI_DTOP_30_L, (MHL_UNLOCK_RESOLUTION << 8) |MHL_LOCK_RESOLUTION);
                W2BYTEMSK(REG_DVI_DTOP_2F_L, (MHL_DIGITAL_UNLOCK_RANGE << 4)| BMASK(3:2), BMASK(15:4)| BMASK(3:2));
                W2BYTEMSK(REG_DVI_ATOP_32_L, BIT(8) |BIT(4) |BIT(0), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP_38_L, BIT(10) |BIT(8), BIT(10) |BIT(8)); // [10]: auto acdr mode selection, [8]: enable auto acdr function
                W2BYTEMSK(REG_DVI_ATOP_5E_L, BIT(4)| ucIControlValue, BMASK(4:0)); //[4]: overwrtie DPL ICTL value, [3:0]: ictl overwrtie value
                W2BYTEMSK(REG_HDCP_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP_0C_L, BIT(9)| ucImpedanceValue, BIT(9)| BMASK(3:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                W2BYTEMSK(REG_DVI_DTOP1_27_L, BIT(2), BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_DTOP1_20_L, MHL_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP1_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP1_3A_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP1_3B_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP1_3C_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP1_3D_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP1_3E_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP1_3F_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_ATOP1_32_L, BIT(8) |BIT(4) |BIT(0), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP1_5E_L, BIT(4) |ucIControlValue, BMASK(4:0)); // enable to overwrtie DPL ICTL value
                W2BYTEMSK(REG_DVI_ATOP1_60_L, BMASK(2:1), BMASK(2:1)); // [2:0]: power down RD
                W2BYTEMSK(REG_DVI_ATOP1_74_L, BMASK(5:0), BMASK(5:0)); // [2:0]: power down DPLPHI, [5:3]: power down DPLPHQ
                W2BYTEMSK(REG_HDCP1_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP1_0C_L, BIT(9)| ucImpedanceValue, BIT(9)| BMASK(3:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_DVI_DTOP3_27_L, BIT(2), BMASK(2:1)); // [7]: MHL HW mode, [1]: MHL pack-pixel mode, [0]: MHL enable
                W2BYTEMSK(REG_DVI_DTOP3_20_L, MHL_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP3_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP3_3A_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP3_3B_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP3_3C_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP3_3D_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP3_3E_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP3_3F_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTE(REG_DVI_DTOP3_30_L, (MHL_UNLOCK_RESOLUTION << 8) |MHL_LOCK_RESOLUTION);
                W2BYTEMSK(REG_DVI_DTOP3_2F_L, (MHL_DIGITAL_UNLOCK_RANGE << 4)| BMASK(3:2), BMASK(15:4)| BMASK(3:2));
                W2BYTEMSK(REG_DVI_ATOP3_32_L, BIT(8) |BIT(4) |BIT(0), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP3_35_L, BIT(10) |BIT(8), BIT(10) |BIT(8)); // [10]: auto acdr mode selection, [8]: enable auto acdr function
                W2BYTEMSK(REG_DVI_ATOP3_5E_L, BIT(4)| ucIControlValue, BMASK(4:0)); //[4]: overwrtie DPL ICTL value, [3:0]: ictl overwrtie value
                W2BYTEMSK(REG_HDCP3_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP3_0C_L, BIT(9)| ucImpedanceValue, BIT(9)| BMASK(3:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_DVI_DTOP2_27_L, BIT(2), BMASK(2:1)); // [7]: MHL HW mode, [1]: MHL pack-pixel mode, [0]: MHL enable
                W2BYTEMSK(REG_DVI_DTOP2_20_L, MHL_EQ_SETTING_VALUE, BMASK(3:0)); // EQ strength
                W2BYTEMSK(REG_DVI_DTOP2_0E_L, 0, BIT(4)); // auto clear phase accumulator
                W2BYTEMSK(REG_DVI_DTOP2_3A_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP2_3B_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP2_3C_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP2_3D_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTEMSK(REG_DVI_DTOP2_3E_L, MHL_PHASE_CODE_VALUE, 0xFFFF); // phase code = 0x0A
                W2BYTEMSK(REG_DVI_DTOP2_3F_L, BIT(4), BIT(4)); // overwirte enable
                W2BYTE(REG_DVI_DTOP2_30_L, (MHL_UNLOCK_RESOLUTION << 8) |MHL_LOCK_RESOLUTION);
                W2BYTEMSK(REG_DVI_DTOP2_2F_L, (MHL_DIGITAL_UNLOCK_RANGE << 4)| BMASK(3:2), BMASK(15:4)| BMASK(3:2));
                W2BYTEMSK(REG_DVI_ATOP2_32_L, BIT(8) |BIT(4) |BIT(0), BMASK(9:8) |BMASK(6:4) |BMASK(2:0)); // HF, Enable ACDR, Bypass CLK
                W2BYTEMSK(REG_DVI_ATOP2_35_L, BIT(10) |BIT(8), BIT(10) |BIT(8)); // [10]: auto acdr mode selection, [8]: enable auto acdr function
                W2BYTEMSK(REG_DVI_ATOP2_5E_L, BIT(4) |ucIControlValue, BMASK(4:0)); // enable to overwrtie DPL ICTL value
                W2BYTEMSK(REG_HDCP2_09_L, 0, BIT(0)); // PP mode + HDCP eanble

                if(ucImpedanceValue != MHL_IMPEDANCE_VALUE)
                {
                    W2BYTEMSK(REG_DVI_DTOP2_0C_L, BIT(9)| ucImpedanceValue, BIT(9)| BMASK(3:0));
                }
            }
            
            break;
#endif

        default:
        
            break;
    };

    _mhal_mhl_DviAutoEQSwitch(ucCbusSelect, FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_MhlPackedPixelModeSetting()
//  [Description]
//                  MHL packed pixel mode setting
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_MhlPackedPixelModeSetting(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_DVI_DTOP_27_L, BMASK(2:1), BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_ATOP_32_L, BMASK(9:8), BMASK(9:8)); // HF
                W2BYTEMSK(REG_HDCP_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                W2BYTEMSK(REG_DVI_DTOP1_27_L, BMASK(2:1), BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_ATOP1_32_L, BMASK(9:8), BMASK(9:8)); // HF
                W2BYTEMSK(REG_HDCP1_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_DVI_DTOP3_27_L, BMASK(2:1), BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_ATOP3_32_L, BMASK(9:8), BMASK(9:8)); // HF
                W2BYTEMSK(REG_HDCP3_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_DVI_DTOP2_27_L, BMASK(2:1), BMASK(2:1)); // [2:1]: MHL_SEL
                W2BYTEMSK(REG_DVI_ATOP2_32_L, BMASK(9:8), BMASK(9:8)); // HF
                W2BYTEMSK(REG_HDCP2_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_RxRtermControl()
//  [Description]
//                  MHL TMDS termination resistor control
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_RxRtermControl(MS_U8 ucCbusSelect, RXRtermControl_T rctrl)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if (rctrl == RX_HDMI_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP_60_L, 0, BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(8));// clock R-term
                }
                else if (rctrl == RX_MHL_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP_60_L, BMASK(13:12), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(8));// clock R-term
                }
                else if (rctrl == RX_RTERM_OFF)
                {
                    W2BYTEMSK(REG_DVI_ATOP_60_L, BMASK(13:11), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, BIT(8), BIT(8));// clock R-term
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                if (rctrl == RX_HDMI_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_60_L, 0, BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(9));// clock R-term
                }
                else if (rctrl == RX_MHL_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_60_L, BMASK(13:12), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(9));// clock R-term
                }
                else if (rctrl == RX_RTERM_OFF)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_60_L, BMASK(13:11), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, BIT(9), BIT(9));// clock R-term
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if (rctrl == RX_HDMI_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_60_L, 0, BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(11));// clock R-term
                }
                else if (rctrl == RX_MHL_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_60_L, BMASK(13:12), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(11));// clock R-term
                }
                else if (rctrl == RX_RTERM_OFF)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_60_L, BMASK(13:11), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, BIT(11), BIT(11));// clock R-term
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if (rctrl == RX_HDMI_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_60_L, 0, BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(10));// clock R-term
                }
                else if (rctrl == RX_MHL_RTERM)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_60_L, BMASK(13:12), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, 0, BIT(10));// clock R-term
                }
                else if (rctrl == RX_RTERM_OFF)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_60_L, BMASK(13:11), BMASK(13:11));// data R-term
                    W2BYTEMSK(REG_PM_SLEEP_4B_L, BIT(10), BIT(10));// clock R-term
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusForceToStandby()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_CbusForceToStandby(void)
{
    W2BYTEMSK(REG_MHL_CBUS_3A, BIT(4), BIT(4)); // clear received FIFO
    W2BYTEMSK(REG_PM_MHL_CBUS_17, BIT(15), BIT(15)); // force enter PM mode
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_MHLForceToAttach()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_MHLForceToAttach(void)
{
    W2BYTEMSK(REG_PM_MHL_CBUS_17, BIT(14), BIT(14));
}

#if(DMHL_LG_PRADA_PATCH)
//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_AdjustCommonModeResistor()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_AdjustCommonModeResistor(MS_U8 ucCbusSelect, MS_BOOL bflag)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_DVI_DTOP_27_L, bflag ?0 :BIT(0), BIT(0)); // [0]: MHL enable
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                W2BYTEMSK(REG_DVI_DTOP1_27_L, bflag ?0 :BIT(0), BIT(0)); // [0]: MHL enable
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_DVI_DTOP3_27_L, bflag ?0 :BIT(0), BIT(0)); // [0]: MHL enable
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_DVI_DTOP2_27_L, bflag ?0 :BIT(0), BIT(0)); // [0]: MHL enable
            }
            
            break;
#endif

        default:
        
            break;
    };
}

#endif

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ClockBigChangeFlag()
//  [Description]:
//                  
//  [Arguments]:
//                  
//  [Return]:
//                 
//**************************************************************************
MS_BOOL _mhal_mhl_ClockBigChangeFlag(MS_U8 ucCbusSelect)
{
    MS_BOOL bindex = FALSE;
    
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                bindex = ((R2BYTE(REG_DVI_ATOP_70_L) &BIT(0)) ?TRUE: FALSE);

                if(bindex)
                {
                    W2BYTEMSK(REG_DVI_ATOP_71_L, BIT(8), BIT(8));
                    W2BYTEMSK(REG_DVI_ATOP_71_L, 0, BIT(8));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                bindex = ((R2BYTE(REG_DVI_ATOP1_70_L) &BIT(0)) ?TRUE: FALSE);

                if(bindex)
                {
                    W2BYTEMSK(REG_DVI_ATOP1_71_L, BIT(8), BIT(8));
                    W2BYTEMSK(REG_DVI_ATOP1_71_L, 0, BIT(8));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                bindex = ((R2BYTE(REG_DVI_ATOP3_70_L) &BIT(0)) ?TRUE: FALSE);

                if(bindex)
                {
                    W2BYTEMSK(REG_DVI_ATOP3_71_L, BIT(8), BIT(8));
                    W2BYTEMSK(REG_DVI_ATOP3_71_L, 0, BIT(8));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                bindex = ((R2BYTE(REG_DVI_ATOP2_70_L) &BIT(0)) ?TRUE: FALSE);

                if(bindex)
                {
                    W2BYTEMSK(REG_DVI_ATOP2_71_L, BIT(8), BIT(8));
                    W2BYTEMSK(REG_DVI_ATOP2_71_L, 0, BIT(8));
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
    
    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckClockStatus()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mhal_mhl_CheckClockStatus(MS_U8 ucCbusSelect)
{
    MS_BOOL bStable = FALSE;
    MS_U16 usClkCount = 0;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                usClkCount = R2BYTE(REG_DVI_DTOP_17_L) & 0x0FFF;
                
                if((R2BYTE(REG_DVI_DTOP_16_L) &BIT(9)) == BIT(9))
                {
                    bStable = TRUE;

                    if((usClkCount < CBUS_CLOCK_DETECT_LEVEL) || _mhal_mhl_ClockBigChangeFlag(ucCbusSelect))
                    {
                        bStable = FALSE;
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                usClkCount = R2BYTE(REG_DVI_DTOP1_17_L) & 0x0FFF;
                
                if((R2BYTE(REG_DVI_DTOP1_16_L) &BIT(9)) == BIT(9))
                {
                    bStable = TRUE;

                    if((usClkCount < CBUS_CLOCK_DETECT_LEVEL) || _mhal_mhl_ClockBigChangeFlag(ucCbusSelect))
                    {
                        bStable = FALSE;
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                usClkCount = R2BYTE(REG_DVI_DTOP3_17_L) & 0x0FFF;
                
                if((R2BYTE(REG_DVI_DTOP3_16_L) &BIT(9)) == BIT(9))
                {
                    bStable = TRUE;

                    if((usClkCount < CBUS_CLOCK_DETECT_LEVEL) || _mhal_mhl_ClockBigChangeFlag(ucCbusSelect))
                    {
                        bStable = FALSE;
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                usClkCount = R2BYTE(REG_DVI_DTOP2_17_L) & 0x0FFF;
                
                if((R2BYTE(REG_DVI_DTOP2_16_L) &BIT(9)) == BIT(9))
                {
                    bStable = TRUE;

                    if((usClkCount < CBUS_CLOCK_DETECT_LEVEL) || _mhal_mhl_ClockBigChangeFlag(ucCbusSelect))
                    {
                        bStable = FALSE;
                    }
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
    
    return bStable;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_RtermHWControl()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_RtermHWControl(MS_U8 ucCbusSelect, MS_BOOL bFlag)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_DVI_ATOP_7F_L, bFlag ?BIT(14) :0, BIT(14));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_DVI_ATOP3_01_L, bFlag ?BIT(0) :0, BIT(0));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_DVI_ATOP2_00_L, bFlag ?BIT(0) :0, BIT(0));
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ChangeScalerMainMux()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ChangeScalerMainMux(MS_BOOL bFlag)
{
    MS_U8 ucScalerMainMux = (R2BYTE(0x102E02) &BMASK(7:4)) >> 4;

    if(bFlag)
    {
        if(ucScalerMainMux == MHL_SCALER_MUX_SELECT_DVI)
        {
            W2BYTEMSK(0x102E02, (MHL_SCALER_MUX_SELECT_MHL << 4), BMASK(7:4));
        }
    }
    else
    {
        if(ucScalerMainMux == MHL_SCALER_MUX_SELECT_MHL)
        {
            W2BYTEMSK(0x102E02, (MHL_SCALER_MUX_SELECT_DVI << 4), BMASK(7:4));
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_AudioPathSelect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_AudioPathSelect(MS_BOOL bMHLPath)
{
    W2BYTEMSK(REG_HDMI2_08_L, bMHLPath ?BIT(0) :0, BIT(0)); // [0]: audio source selection, 0: HDMI / 1: MHL
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusAndClockSelect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusAndClockSelect(void)
{
    MS_U8 ucClockSelect = MHL_DVI_PORT_A;
    
    if(GET_MHL_PATH_SUPPORT_PORTA())
    {
        ucClockSelect = MHL_DVI_PORT_A;
    }
    else if(GET_MHL_PATH_SUPPORT_PORTB())
    {
        ucClockSelect = MHL_DVI_PORT_B;
    }
    else if(GET_MHL_PATH_SUPPORT_PORTC())
    {
        W2BYTEMSK(REG_PM_SLEEP_73_L, BIT(0), BIT(6)| BIT(8)| BIT(0)); // [0]: reg_hplugc_mhl_en
        ucClockSelect = MHL_DVI_PORT_C;
    }
    else if(GET_MHL_PATH_SUPPORT_PORTD())
    {
        W2BYTEMSK(REG_PM_SLEEP_73_L, BIT(8), BIT(14)| BIT(8)| BIT(0)); // [8]: reg_hpluga_mhl_en
        ucClockSelect = MHL_DVI_PORT_D;
    }

    W2BYTEMSK(REG_DVI_ATOP_6A_L, (ucClockSelect << 2), BMASK(3:2)); // [3:2]: HDCP clock select
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_PhyInitialSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_PhyInitialSetting(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_DVI_DTOP_27_L, BIT(15), BIT(15)); // [15]: Enable MHL packed-pixel mode criteria
                W2BYTEMSK(REG_DVI_DTOP_28_L, BIT(0), BIT(0)); // [0]: MHL v1.2
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_DVI_DTOP3_27_L, BIT(15), BIT(15)); // [15]: Enable MHL packed-pixel mode criteria
                W2BYTEMSK(REG_DVI_DTOP3_28_L, BIT(0), BIT(0)); // [0]: MHL v1.2
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_DVI_DTOP2_27_L, BIT(15), BIT(15)); // [15]: Enable MHL packed-pixel mode criteria
                W2BYTEMSK(REG_DVI_DTOP2_28_L, BIT(0), BIT(0)); // [0]: MHL v1.2
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetCbusSelect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mhal_mhl_GetCbusSelect(MS_U8 ucPort)
{
    MS_U8 uctemp = MHL_CBUS_SELECT_MASK;
    
    switch(ucPort)
    {
        case MHL_DVI_PORT_A:
            uctemp = MHL_CBUS_SELECT_PORTA;
            break;

        case MHL_DVI_PORT_B:
            uctemp = MHL_CBUS_SELECT_PORTB;
            break;

        case MHL_DVI_PORT_C:
            uctemp = MHL_CBUS_SELECT_PORTC;
            break;

        case MHL_DVI_PORT_D:
            uctemp = MHL_CBUS_SELECT_PORTD;
            break;
            
        default:
        
            break;
    };

    return uctemp;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetInputPort()
//  [Description]
//                  MHL get current input port
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_U8 mhal_mhl_GetInputPort(void)
{
    return (R2BYTE(REG_DVI_ATOP_6A_L) &BMASK(1:0));
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CheckInputPort()
//  [Description]
//                  MHL check current input port
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_BOOL mhal_mhl_CheckInputPort(MS_U8 ucCbusSelect)
{
    MS_BOOL bindex = FALSE;

    if(mhal_mhl_GetCbusSelect(mhal_mhl_GetInputPort()) == ucCbusSelect)
    {
        bindex = TRUE;
    }
    
    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CheckPIPWindow()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_BOOL mhal_mhl_CheckPIPWindow(void)
{
    MS_BOOL bFlag = FALSE;
    MS_U8 ucScalerMainMux = R2BYTE(0x102E02) &BMASK(7:0);
    MS_U8 ucScalerSubMux = ucScalerMainMux &BMASK(3:0);

    ucScalerMainMux = (ucScalerMainMux &BMASK(7:4)) >> 4;
    
    if((ucScalerMainMux == MHL_SCALER_MUX_SELECT_DVI) || (ucScalerMainMux == MHL_SCALER_MUX_SELECT_MHL)) // Check scaler main window mux
    {
        bFlag = TRUE;
    }
    else if((ucScalerSubMux == MHL_SCALER_MUX_SELECT_DVI) || (ucScalerSubMux == MHL_SCALER_MUX_SELECT_MHL)) // Check scaler sub window mux
    {
        bFlag = TRUE;
    }
    
    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_MHLSupportPath()
//  [Description]
//                  MHL support path
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_MHLSupportPath(MS_U8 ucSelect)
{
    ucMHLSupportPort = ucSelect;

    if(GET_MHL_PATH_SUPPORT_PORTC() && GET_MHL_PATH_SUPPORT_PORTD())
    {
        msg_mhl(printf("** MHL Cbus illegal support path T^T\r\n"));
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CheckEfuseControlFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CheckEfuseControlFlag(MS_BOOL bEfuseFlag)
{
    MS_BOOL bFlag = FALSE;

#if(MHL_EFUSE_FUNCTION_CONTROL)
    MS_U8 uctemp = 0;

    if(!bEfuseFlag)
    {
        for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK; uctemp++)
        {
            mhal_mhl_CbusIsolate(uctemp, TRUE);
        }

        bFlag = TRUE;
    }
    
#endif

    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_InsertChipIDforCheck()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_InsertChipIDforCheck(MS_U8 ucChipID)
{
    ucChipIDValue = ucChipID;

    msg_mhl(printf("** MHL Cbus index value %x\r\n", ucChipID));
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusSelectSetMux()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CbusSelectSetMux(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {

            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableDetect()
//  [Description]
//                  MHL cable detection
//  [Arguments]:
//
//  [Return]:
//                  TRUE: MHL cable plugged
//                  FALSE: MHL cable unplugged
//**************************************************************************
MS_BOOL mhal_mhl_CableDetect(MS_U8 ucCbusSelect)
{
    MS_BOOL bFlag = FALSE;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
            }
            
            break;
#endif

        default:
        
            break;
    };
    
    return bFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_Accumulator_Clr()
//  [Description]
//                  to clear accumulator when input is not MHL
//  [Arguments]:
//
//  [Return]:
//**************************************************************************
void mhal_mhl_Accumulator_Clr(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(R2BYTE(REG_DVI_DTOP_16_L) & BIT(9)) // clk stable
    			{
    				if((R2BYTE(REG_DVI_DTOP_01_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP_01_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP_02_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP_02_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP_03_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP_03_L) == 0x7FF7))
    				{
    					W2BYTEMSK(REG_DVI_DTOP_0E_L, BIT(4), BIT(4)); // clear accumulator
    					W2BYTEMSK(REG_DVI_DTOP_0E_L, 0, BIT(4));
    				}
    			}
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                if(R2BYTE(REG_DVI_DTOP1_16_L) & BIT(9)) // clk stable
    			{
    				if((R2BYTE(REG_DVI_DTOP1_01_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP1_01_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP1_02_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP1_02_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP1_03_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP1_03_L) == 0x7FF7))
    				{
    					W2BYTEMSK(REG_DVI_DTOP1_0E_L, BIT(4), BIT(4)); // clear accumulator
    					W2BYTEMSK(REG_DVI_DTOP1_0E_L, 0, BIT(4));
    				}
    			}
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(R2BYTE(REG_DVI_DTOP3_16_L) & BIT(9)) // clk stable
    			{
    				if((R2BYTE(REG_DVI_DTOP3_01_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP3_01_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP3_02_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP3_02_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP3_03_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP3_03_L) == 0x7FF7))
    				{
    					W2BYTEMSK(REG_DVI_DTOP3_0E_L, BIT(4), BIT(4)); // clear accumulator
    					W2BYTEMSK(REG_DVI_DTOP3_0E_L, 0, BIT(4));
    				}
    			}
            }
			
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(R2BYTE(REG_DVI_DTOP2_16_L) & BIT(9)) // clk stable
    			{
    				if((R2BYTE(REG_DVI_DTOP2_01_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP2_01_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP2_02_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP2_02_L) == 0x7FF7) ||
    				(R2BYTE(REG_DVI_DTOP2_03_L) == 0x8008) || (R2BYTE(REG_DVI_DTOP2_03_L) == 0x7FF7))
    				{
    					W2BYTEMSK(REG_DVI_DTOP2_0E_L, BIT(4), BIT(4)); // clear accumulator
    					W2BYTEMSK(REG_DVI_DTOP2_0E_L, 0, BIT(4));
    				}
    			}
            }
			
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CDRModeMonitor()
//  [Description]:
//                  MHL CDR mode monitor
//  [Arguments]:
//                  isCbusConnected: Cbus is at connected state or not
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CDRModeMonitor(MS_U8 ucCbusSelect, MS_BOOL bPathEnable)
{
    MS_BOOL bindex = FALSE;
    static MS_U8 ucStableCount = 0;
    static MS_BOOL bPLLPower[MHL_CBUS_SELECT_MASK] = {TRUE, TRUE, TRUE, TRUE};
    
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
                {
                    if(ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                    {
                        ucStableCount++;
                    }
                    
                    if((R2BYTE(REG_DVI_DTOP_31_L) &BIT(6)) == BIT(6)) // DE stable
                    {
                        if(!bPLLPower[ucCbusSelect])
                        {
                            W2BYTEMSK(REG_HDMI2_06_L, BIT(12), BIT(12));
                            W2BYTEMSK(REG_HDMI2_06_L, 0, BIT(12));

                            bPLLPower[ucCbusSelect] = TRUE;
                        }
                    }
                    else // DE unstable
                    {
                        if(ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                        {
                            ucStableCount = 0;
                            bPLLPower[ucCbusSelect] = FALSE;
                        }
                        else if(ucStableCount == (MHL_CDR_STABLE_THRESHOLD -MHL_CDR_STABLE_OFFSET))
                        {
                            bPLLPower[ucCbusSelect] = TRUE;
                        }
                    }
                }
                else
                {
                    if(bPLLPower[ucCbusSelect])
                    {
                        bPLLPower[ucCbusSelect] = FALSE;
                        ucStableCount = 0;
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
                {
                    if(ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                    {
                        ucStableCount++;
                    }
                    
                    if((R2BYTE(REG_DVI_DTOP3_31_L) &BIT(6)) == BIT(6)) // DE stable
                    {
                        if(!bPLLPower[ucCbusSelect])
                        {
                            W2BYTEMSK(REG_HDMI2_06_L, BIT(12), BIT(12));
                            W2BYTEMSK(REG_HDMI2_06_L, 0, BIT(12));

                            bPLLPower[ucCbusSelect] = TRUE;
                        }
                    }
                    else // DE unstable
                    {
                        if(ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                        {
                            ucStableCount = 0;
                            bPLLPower[ucCbusSelect] = FALSE;
                        }
                        else if(ucStableCount == (MHL_CDR_STABLE_THRESHOLD -MHL_CDR_STABLE_OFFSET))
                        {
                            bPLLPower[ucCbusSelect] = TRUE;
                        }
                    }
                }
                else
                {
                    if(bPLLPower[ucCbusSelect])
                    {
                        bPLLPower[ucCbusSelect] = FALSE;
                        ucStableCount = 0;
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
                {
                    if(ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                    {
                        ucStableCount++;
                    }
                    
                    if((R2BYTE(REG_DVI_DTOP2_31_L) &BIT(6)) == BIT(6)) // DE stable
                    {
                        if(!bPLLPower[ucCbusSelect])
                        {
                            W2BYTEMSK(REG_HDMI2_06_L, BIT(12), BIT(12));
                            W2BYTEMSK(REG_HDMI2_06_L, 0, BIT(12));

                            bPLLPower[ucCbusSelect] = TRUE;
                        }
                    }
                    else // DE unstable
                    {
                        if(ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                        {
                            ucStableCount = 0;
                            bPLLPower[ucCbusSelect] = FALSE;
                        }
                        else if(ucStableCount == (MHL_CDR_STABLE_THRESHOLD -MHL_CDR_STABLE_OFFSET))
                        {
                            bPLLPower[ucCbusSelect] = TRUE;
                        }
                    }
                }
                else
                {
                    if(bPLLPower[ucCbusSelect])
                    {
                        bPLLPower[ucCbusSelect] = FALSE;
                        ucStableCount = 0;
                    }
                }
            }
            
            break;
#endif

        default:
        
            break;
    };

    if(ucStableCount >= MHL_CDR_FORCE_THRESHOLD)
    {
        bMHLSignalStable = TRUE;
    }
    else if(bMHLSignalStable)
    {
        bMHLSignalStable = FALSE;
    }
    
    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusIsolate()
//  [Description]
//                  MHL cable isolate
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CbusIsolate(MS_U8 ucCbusSelect, MS_BOOL bFlag)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(bFlag)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
                }
                else
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(bFlag)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
                }
                else
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(bFlag)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
                }
                else
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_VbusCharge()
//  [Description]
//                  MHL Vbus charge
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_VbusCharge(MS_U8 ucCbusSelect, MS_U8 bState)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(bState == VBUS_SW_CHARGE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, BMASK(1:0), BMASK(1:0));
                }
                else if(bState == VBUS_SW_UNCHARGE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, BIT(1), BMASK(1:0));
                }
                else
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, 0, BMASK(1:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(bState == VBUS_SW_CHARGE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, BMASK(1:0), BMASK(1:0));
                }
                else if(bState == VBUS_SW_UNCHARGE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, BIT(1), BMASK(1:0));
                }
                else
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, 0, BMASK(1:0));
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(bState == VBUS_SW_CHARGE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, BMASK(1:0), BMASK(1:0));
                }
                else if(bState == VBUS_SW_UNCHARGE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, BIT(1), BMASK(1:0));
                }
                else
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_01, 0, BMASK(1:0));
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusFloating()
//  [Description]
//                  MHL cable floating
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusFloating(MS_BOOL bFlag)
{
    if(bFlag)
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_17, BIT(5), BIT(5));
    }
    else
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_17, 0, BIT(5));
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusStucktoLow()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusStucktoLow(MS_BOOL bFlag)
{
    if(bFlag)
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_18, 0, BIT(1)); // cbus stuck to low int mask
    }
    else
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_18, BIT(1), BIT(1)); // cbus stuck to low int mask
    }

    W2BYTEMSK(REG_PM_MHL_CBUS_18, BIT(0), BIT(0)); // Clear cbus stuck to low int flag
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusWakeupInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_CbusWakeupInterrupt(MS_BOOL bFlag)
{
    if(bFlag)
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_18, 0, BIT(5)); // wake up pulse int mask
    }
    else
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_18, BIT(5), BIT(5)); // wake up pulse int mask
    }

    W2BYTEMSK(REG_PM_MHL_CBUS_18, BIT(4), BIT(4)); // Clear wake up pulse int flag
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetVenderID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetVenderID(MS_U8 ucVenderID)
{
    W2BYTEMSK(REG_MHL_CBUS_00, (ucVenderID << 8), BMASK(15:8));
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_LoadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_LoadEDID(MS_U8 *edid)
{
    MS_U16 ustemp = 0;
    
    if(edid != NULL)
    {
        // Load EDID
        msg_mhl(printf("** Munich Load MHL EDID...\r\n"));

        W2BYTEMSK(REG_MHL_CBUS_52, BIT(1), BIT(1)); // CPU write enable

        for(ustemp = 0; ustemp <256; ustemp++)
        {
            W2BYTEMSK(REG_MHL_CBUS_52, ustemp <<8, 0xFF00); // address
            W2BYTEMSK(REG_MHL_CBUS_53, edid[ustemp], 0x00FF); // data
            W2BYTEMSK(REG_MHL_CBUS_52, BIT(0), BIT(0)); // write trigger
            W2BYTEMSK(REG_MHL_CBUS_52, 0, BIT(0));
            while(R2BYTE(REG_MHL_CBUS_52) & BIT(5));
        }

        W2BYTEMSK(REG_MHL_CBUS_52, 0, BIT(1)); // CPU write disable
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ReadEDID()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ReadEDID(MS_U16 usSize, MS_U8 *edid)
{
    MS_U16 ustemp = 0;
    
    if(edid != NULL)
    {
        // Read EDID
        msg_mhl(printf("** Munich Read MHL EDID...\r\n"));

        for(ustemp = 0; ustemp < usSize; ustemp++)
        {
            W2BYTEMSK(REG_MHL_CBUS_52, ustemp <<8, 0xFF00); // address
            W2BYTEMSK(REG_MHL_CBUS_52, BIT(3), BIT(3)); // read trigger
            W2BYTEMSK(REG_MHL_CBUS_52, 0, BIT(0));
            while(R2BYTE(REG_MHL_CBUS_52) & BIT(4));
            
            edid[ustemp] = (MS_U8)((R2BYTE(REG_MHL_CBUS_53) & 0xFF00) >> 8); // data
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_LoadDeviceCapability()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_LoadDeviceCapability(MS_U8 *devcap)
{
    MS_U8 ucIndex = 0;
    MS_U8 uctemp = 0;
    
    if(devcap != NULL)
    {
        msg_mhl(printf("** Munich Load DevCap...\r\n"));

        // Load MHL device capability
        for(uctemp = 0; uctemp <8; uctemp++)
        {
            if(uctemp == 2)
            {
                ucIndex = 1;
            }

            W2BYTE(REG_MHL_CBUS_01 +(uctemp *2), (devcap[(uctemp *2) +1 -ucIndex] <<8) | devcap[uctemp *2 -ucIndex]);
        }

        W2BYTE(REG_MHL_CBUS_03, (devcap[3] <<8) | devcap[4]);
        W2BYTE(REG_MHL_CBUS_07, (devcap[11] <<8) | devcap[12]);

        W2BYTEMSK(REG_MHL_CBUS_09, devcap[15], BMASK(7:0));
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_initial()
//  [Description]
//                  MHL init
//  [Arguments]:
//                  *edid: MHL EDID data
//                  *devcap: MHL device capability
//  [Return]:
//
//**************************************************************************
MS_U8 mhal_mhl_initial(MS_U8 *edid, MS_U8 *devcap, MS_U8 ucVenderID)
{
    MS_U16 uctemp = 0;

    // Initial setting
    for(uctemp = 0; uctemp <(sizeof(tMHL_INITIAL_TABLE) /sizeof(msLoadTbl_S)); uctemp++)
    {
        W2BYTEMSK(tMHL_INITIAL_TABLE[uctemp].addr, tMHL_INITIAL_TABLE[uctemp].databuf, tMHL_INITIAL_TABLE[uctemp].mask);
    }

    W2BYTEMSK(REG_PM_SLEEP_72_L, BMASK(7:6), BMASK(8:6)); // [8]: reg_cbus_debug_sel, [7]: reg_vbus_en_sel , [6]: reg_mhl_cable_detect_sel
    
    _mhal_mhl_CbusAndClockSelect();
    
#if(MHL_INTERRUPT_USE_PM_IRQ)
    // Open PM irq mask
    W2BYTEMSK(0x2B28, 0, BIT(11));
#endif

    // Load EDID
    mhal_mhl_LoadEDID(edid);

    // Load vendor ID
    mhal_mhl_SetVenderID(ucVenderID);
    
    // Load DevCap
    mhal_mhl_LoadDeviceCapability(devcap);

    // Clear Cbus received interrupt status
    W2BYTEMSK(REG_MHL_CBUS_3A, BIT(4)|BIT(0), BIT(4)|BIT(1)|BIT(0));  // [1]: receive packet valid mask

    mhal_mhl_CbusStucktoLow(FALSE);
    mhal_mhl_CbusWakeupInterrupt(FALSE);

    _mhal_mhl_MHLForceToAttach();

    for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK; uctemp++)
    {
        _mhal_mhl_RtermHWControl(uctemp, FALSE);
        _mhal_mhl_PhyInitialSetting(uctemp);
    }

#if(DMHL_TEST_SIGNAL_SUPPORT)
    W2BYTEMSK(0x001102, BIT(7), BIT(7));
    W2BYTEMSK(0x001128, BIT(0), BIT(0));

#endif

    mhal_mhl_CbusFloating(TRUE);

    return MHL_CHIP_FUNCTION_CAPABILITY;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_InvertCableDetect()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_InvertCableDetect(MS_U8 ucCbusSelect, MS_BOOL bCableDetectInvert)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_VbusConfigSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_VbusConfigSetting(MS_U8 ucCbusSelect, MS_U8 ucState)
{
    MS_U8 ucOutputState = ucState &(MHL_VBUS_LOW_ENABLE_MODE | MHL_VBUS_HIGH_ENABLE_MODE);
    
    ucState = ucState &(MHL_VBUS_OUTPUT_MODE | MHL_VBUS_INVERSE_MODE);

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(ucState == MHL_VBUS_OUTPUT_MODE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_31, BIT(0), BIT(0));
                    W2BYTEMSK(REG_PM_MHL_CBUS_21, BIT(7), BIT(7));
                }
                else if(ucState > 0)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_31, BIT(0), BIT(0));
                }

                if(ucOutputState > 0)
                {
                    if(ucOutputState == MHL_VBUS_HIGH_ENABLE_MODE)
                    {
                        W2BYTEMSK(REG_PM_MHL_CBUS_20, 0, BMASK(9:8));
                    }
                    else
                    {
                        W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(9), BMASK(9:8));
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(ucState == MHL_VBUS_OUTPUT_MODE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_31, BIT(0), BIT(0));
                    W2BYTEMSK(REG_PM_MHL_CBUS_21, BIT(7), BIT(7));
                }
                else if(ucState > 0)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_31, BIT(0), BIT(0));
                }

                if(ucOutputState > 0)
                {
                    if(ucOutputState == MHL_VBUS_HIGH_ENABLE_MODE)
                    {
                        W2BYTEMSK(REG_PM_MHL_CBUS_20, 0, BMASK(9:8));
                    }
                    else
                    {
                        W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(9), BMASK(9:8));
                    }
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(ucState == MHL_VBUS_OUTPUT_MODE)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_31, BIT(0), BIT(0));
                    W2BYTEMSK(REG_PM_MHL_CBUS_21, BIT(7), BIT(7));
                }
                else if(ucState > 0)
                {
                    W2BYTEMSK(REG_PM_MHL_CBUS_31, BIT(0), BIT(0));
                }

                if(ucOutputState > 0)
                {
                    if(ucOutputState == MHL_VBUS_HIGH_ENABLE_MODE)
                    {
                        W2BYTEMSK(REG_PM_MHL_CBUS_20, 0, BMASK(9:8));
                    }
                    else
                    {
                        W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(9), BMASK(9:8));
                    }
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableDetectPadSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CableDetectPadSetting(MS_U8 ucSelect)
{
    if((ucSelect &MHL_CABLE_DETECT_SELECT_PORTC) && (ucSelect &MHL_CABLE_DETECT_SELECT_PORTD))
    {
        msg_mhl(printf("** MHL cable detect only select one port T^T\r\n"));
    }
    else
    {
        // Do nothing
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusPadConfigSwitch()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CbusPadConfigSwitch(MS_U8 ucCbusSelect, MS_BOOL bFlag)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(GET_MHL_PATH_SUPPORT_PORTA())
            {
                if(bFlag)
                {
                    _mhal_mhl_MHLForceToAttach();
                    mhal_mhl_VbusCharge(ucCbusSelect, VBUS_HW_DETECT);
                }
                else
                {
                    _mhal_mhl_CbusForceToStandby();
                    mhal_mhl_VbusCharge(ucCbusSelect, VBUS_SW_UNCHARGE);
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(GET_MHL_PATH_SUPPORT_PORTB())
            {
                
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(GET_MHL_PATH_SUPPORT_PORTC())
            {
                if(bFlag)
                {
                    _mhal_mhl_MHLForceToAttach();
                    mhal_mhl_VbusCharge(ucCbusSelect, VBUS_HW_DETECT);
                }
                else
                {
                    _mhal_mhl_CbusForceToStandby();
                    mhal_mhl_VbusCharge(ucCbusSelect, VBUS_SW_UNCHARGE);
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(GET_MHL_PATH_SUPPORT_PORTD())
            {
                if(bFlag)
                {
                    _mhal_mhl_MHLForceToAttach();
                    mhal_mhl_VbusCharge(ucCbusSelect, VBUS_HW_DETECT);
                }
                else
                {
                    _mhal_mhl_CbusForceToStandby();
                    mhal_mhl_VbusCharge(ucCbusSelect, VBUS_SW_UNCHARGE);
                }
            }
            
            break;
#endif

        default:
        
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusStatus()
//  [Description]:
//                  MHL Cbus status
//  [Arguments]:
//
//  [Return]:
//                  Cbus status value
//**************************************************************************
MS_U16 mhal_mhl_CbusStatus(void)
{
    return (R2BYTE(REG_PM_MHL_CBUS_17));
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusIsMscMsgReceived()
//  [Description]
//                  MHL Cbus check whether msc message is received or not
//  [Arguments]:
//
//  [Return]:
//                  TRUE: recieved
//                  FALSE: not yet
//**************************************************************************
MS_BOOL mhal_mhl_CbusIsMscMsgReceived(void)
{
    MS_BOOL bindex = ((R2BYTE(REG_MHL_CBUS_3A) &BIT(3)) ?TRUE: FALSE);

    if(bindex)
    {
        W2BYTEMSK(REG_MHL_CBUS_3A, BIT(0), BIT(0));
    }

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusStucktoLowFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CbusStucktoLowFlag(void)
{
    MS_BOOL bindex = ((R2BYTE(REG_PM_MHL_CBUS_18) &BIT(3)) ?TRUE: FALSE);

    if(bindex)
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_18, BIT(0), BIT(0));
    }

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusWakeupIntFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CbusWakeupIntFlag(void)
{
    MS_BOOL bindex = ((R2BYTE(REG_PM_MHL_CBUS_18) &BIT(7)) ?TRUE: FALSE);

    if(bindex)
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_18, BIT(4), BIT(4));
    }

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CBusWrite()
//  [Description]
//                  MHL Cbus write trigger
//  [Arguments]:
//                  *pdatabuf: Cbus tx data
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CBusWrite(mhalCbusFifo_S *pdatabuf)
{
    MS_U8 uctemp = 0;

    if(_mhal_mhl_IsCbusBusy())
    {
        return FALSE;
    }
    
    for(uctemp = 0; uctemp < pdatabuf->lens; uctemp++)
    {
        W2BYTE(REG_MHL_CBUS_26 +uctemp *2, pdatabuf->databuf[uctemp]);
    }

    // clear the unsed parts
    W2BYTE(REG_MHL_CBUS_26 +((pdatabuf->lens) *2), 0);

    W2BYTEMSK(REG_MHL_CBUS_25, BIT(12), BIT(12)); // trigger to send

    return TRUE;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_Cbus_SetPathEn()
//  [Description]:
//                  MHL Cbus set path enable
//  [Arguments]:
//                  TRUE: Enable
//                  FALSE: Disable
//  [Return]:
//
//**************************************************************************
void mhal_mhl_Cbus_SetPathEn(MS_BOOL bflag)
{
    if(bflag) // set state to PATH_EN
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_17, BIT(12), BIT(12));
    }
    else // clear state to not PATH_EN
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_17, BIT(13), BIT(13));
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusIntCB()
//  [Description]
//                  MHL Cbus Interrupt Call Back function
//  [Arguments]:
//                  *rcstate: recevied state, 0:normal / 1:timeout
//                  *rccmd: recevied command
//                  *rcdata: recevied data
//                  *rclen: received length
//                  *bIsCmdInData: Is command in data field
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CbusIntCB(MS_U8 *rcstate, MS_U8 *rccmd, MS_U8 *rcdata, MS_U8 *rclen, MS_U8 *bIsCmdInData)
{
    MS_BOOL bindex = FALSE;
    MS_U8 uctemp = 0;
    MS_U16 reg_val;

    *rcstate = R2BYTE(REG_MHL_CBUS_3B) & 0x000F; // received state, 0: normal, 1: timeout

    //W2BYTEMSK(REG_MHL_CBUS_3A, BIT(0), BIT(0)); // clear INT

    reg_val = R2BYTE(REG_MHL_CBUS_3C);

    if(!(reg_val &BIT(8))) // Received data
    {
        bindex = TRUE;
    }

    *rccmd = reg_val & 0x00FF;
    *bIsCmdInData = FALSE;

    for(uctemp = 0; uctemp <=(MHL_CBUS_DATA_SIZE +1); uctemp++) // offset+16bytes+EOF
    {
        reg_val = R2BYTE(REG_MHL_CBUS_3D +(uctemp *2));

        if(reg_val & BIT(15))
        {
            rcdata[uctemp] = reg_val & 0x00FF;

            if(((uctemp <= 2) ||(uctemp == (MHL_CBUS_DATA_SIZE +1))) && !(*bIsCmdInData))
            {
                *bIsCmdInData = (reg_val & BIT(8)) ? TRUE : FALSE;
            }
        }
        else
        {
            *rclen = uctemp;
            break;
        }
    }

    // CTS 6.3.11.19
    if(uctemp >(MHL_CBUS_DATA_SIZE +1))
    {
        *rclen = MHL_CBUS_DATA_SIZE +2;
    }

    W2BYTEMSK(REG_MHL_CBUS_3A, BIT(4), BIT(4)); // clear received FIFO

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mdrv_mhl_CBusCheckBCHError()
//  [Description]:
//                  MHL Cbus check BCH error
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CBusCheckBCHError(void)
{
    MS_BOOL bindex = FALSE;
    
    if(R2BYTE(REG_HDMI_04_L) &BIT(1))
    {
        //bindex = TRUE;

        W2BYTEMSK(REG_HDMI_04_L, BIT(1), BIT(1));
    }

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CablePlugProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CablePlugProc(MS_U8 ucCbusSelect)
{
    _mhal_mhl_Mhl24bitsModeSetting(ucCbusSelect);
    
    _mhal_mhl_RxRtermControl(ucCbusSelect, RX_RTERM_OFF);

#if(DMHL_LG_PRADA_PATCH)
    _mhal_mhl_AdjustCommonModeResistor(ucCbusSelect, TRUE);
#endif

    if(mhal_mhl_CheckInputPort(ucCbusSelect))
    {
        //_mhal_mhl_ChangeScalerMainMux(TRUE);
        //_mhal_mhl_AudioPathSelect(TRUE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CableRemoveProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CableRemoveProc(MS_U8 ucCbusSelect)
{
    _mhal_mhl_HdmiBypassModeSetting(ucCbusSelect);
    _mhal_mhl_RxRtermControl(ucCbusSelect, RX_HDMI_RTERM);

    if(mhal_mhl_CheckInputPort(ucCbusSelect))
    {
        //_mhal_mhl_ChangeScalerMainMux(FALSE);
        //_mhal_mhl_AudioPathSelect(FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusConnectProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CbusConnectProc(MS_U8 ucCbusSelect)
{
    _mhal_mhl_RxRtermControl(ucCbusSelect, RX_MHL_RTERM);

#if(DMHL_LG_PRADA_PATCH)
    _mhal_mhl_AdjustCommonModeResistor(ucCbusSelect, FALSE);
#endif

    mhal_mhl_CbusStucktoLow(TRUE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusStucktoLowProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CbusStucktoLowProc(MS_U8 ucCbusSelect)
{
    _mhal_mhl_RxRtermControl(ucCbusSelect, RX_RTERM_OFF);

#if(DMHL_LG_PRADA_PATCH)
    _mhal_mhl_AdjustCommonModeResistor(ucCbusSelect, TRUE);
#endif

    mhal_mhl_CDRModeMonitor(ucCbusSelect, FALSE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SourceChangeProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SourceChangeProc(MS_U8 ucCbusSelect)
{
    if(mhal_mhl_CheckInputPort(ucCbusSelect))
    {
        //_mhal_mhl_ChangeScalerMainMux(TRUE);
        //_mhal_mhl_AudioPathSelect(TRUE);
    }
    else
    {
        //_mhal_mhl_ChangeScalerMainMux(FALSE);
        //_mhal_mhl_AudioPathSelect(FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ClockModeSwitchProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ClockModeSwitchProc(MS_U8 ucCbusSelect, MS_BOOL bPPmode)
{
    if(bPPmode)
    {
        _mhal_mhl_MhlPackedPixelModeSetting(ucCbusSelect);
    }
    else
    {
        _mhal_mhl_Mhl24bitsModeSetting(ucCbusSelect);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CbusWakeupIntSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_CbusWakeupIntSetting(MS_U8 ucCbusSelect, MS_BOOL bFlag)
{
    if(bFlag)
    {
        _mhal_mhl_CbusForceToStandby();
        mhal_mhl_CbusWakeupInterrupt(TRUE);
        mhal_mhl_VbusCharge(ucCbusSelect, VBUS_SW_CHARGE);
    }
    else
    {
        _mhal_mhl_MHLForceToAttach();
        mhal_mhl_CbusWakeupInterrupt(FALSE);
        mhal_mhl_VbusCharge(ucCbusSelect, VBUS_HW_DETECT);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_RtermControlHWMode()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_RtermControlHWMode(MS_U8 ucCbusSelect, MS_BOOL bFlag)
{
    MS_U16 ustemp = mhal_mhl_CbusStatus();
    
    if(!bFlag) // HW to SW control rterm
    {
        if((ustemp & BMASK(1:0)) == 0x03)
        {
            _mhal_mhl_RxRtermControl(ucCbusSelect, RX_MHL_RTERM);
        }
        else
        {
            _mhal_mhl_RxRtermControl(ucCbusSelect, RX_RTERM_OFF);
        }
    }

    _mhal_mhl_RtermHWControl(ucCbusSelect, bFlag);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_AdjustSettingIControl()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_AdjustSettingIControl(MS_U8 ucIControl)
{
    ucIControlValue = ucIControl;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_AdjustImpedanceSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void mhal_mhl_AdjustImpedanceSetting(MS_U8 ucImpedance)
{
    ucImpedanceValue = ucImpedance;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetSignalStableFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_BOOL mhal_mhl_GetSignalStableFlag(void)
{
    return bMHLSignalStable;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_PowerControl()
//  [Description]
//                  MHL power control
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_LoadPowerOnTbl(void)
{
    MS_U8 uctemp = 0;
    
    for(uctemp = 0; uctemp <(sizeof(tMHL_POWER_ON_TABLE) /sizeof(msLoadTbl_S)); uctemp++)
    {
        W2BYTEMSK(tMHL_POWER_ON_TABLE[uctemp].addr, tMHL_POWER_ON_TABLE[uctemp].databuf, tMHL_POWER_ON_TABLE[uctemp].mask);
    }

    for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK; uctemp++)
    {
        _mhal_mhl_RxRtermControl(uctemp, RX_HDMI_RTERM);
    }
}

void mhal_mhl_LoadPowerStandbyTbl(void)
{
    MS_U8 uctemp = 0;

    //_mhal_mhl_CbusForceToStandby();
    
    for(uctemp = 0; uctemp <(sizeof(tMHL_POWER_SAVING_TABLE) /sizeof(msLoadTbl_S)); uctemp++)
    {
        W2BYTEMSK(tMHL_POWER_SAVING_TABLE[uctemp].addr, tMHL_POWER_SAVING_TABLE[uctemp].databuf, tMHL_POWER_SAVING_TABLE[uctemp].mask);
    }

    for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK; uctemp++)
    {
        _mhal_mhl_RxRtermControl(uctemp, RX_RTERM_OFF);
        mhal_mhl_VbusCharge(uctemp, VBUS_SW_CHARGE);
    }
}

void mhal_mhl_LoadPowerDownTbl(void)
{
    MS_U8 uctemp = 0;

    //_mhal_mhl_CbusForceToStandby();
    
    for(uctemp = 0; uctemp <(sizeof(tMHL_POWER_DOWN_TABLE) /sizeof(msLoadTbl_S)); uctemp++)
    {
        W2BYTEMSK(tMHL_POWER_DOWN_TABLE[uctemp].addr, tMHL_POWER_DOWN_TABLE[uctemp].databuf, tMHL_POWER_DOWN_TABLE[uctemp].mask);
    }

    for(uctemp = 0; uctemp < MHL_CBUS_SELECT_MASK; uctemp++)
    {
        _mhal_mhl_RxRtermControl(uctemp, RX_RTERM_OFF);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetHPD()
//  [Description]
//                  config HPD in combo(MHL/HDMI) port A
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetHPD(MS_BOOL bflag)
{
    W2BYTEMSK(REG_PM_SLEEP_73_L, BIT(0), BIT(0)); // [0]: reg_hplugc_mhl_en
    
    if(bflag) // HPD is high
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(3), BMASK(4:3)); // [4]: output val, [3]: oen
        //msg_mhl(printf("**MHL_HPD is High"));
    }
    else // hpd is low
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_20, 0, BMASK(4:3)); // [4]: output val, [3]: oen
        //msg_mhl(printf("**MHL_HPD is Low"));
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetDDCErrorCode()
//  [Description]
//                  Get DDC error code
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mhal_mhl_GetDDCErrorCode(void)
{
    return (R2BYTE(REG_MHL_CBUS_21)>>8);
}

#if(MHL_CBUS_OPERATION_MODE == MHL_CBUS_HW_REPLY_MODE)
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_CheckSRAMReceiveBuffer()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_CheckSRAMReceiveBuffer(void)
{
    return ((R2BYTE(REG_MHL_CBUS_10) &BIT(4)) ?FALSE: TRUE);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetSRAMReceiveData()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 mhal_mhl_GetSRAMReceiveData(void)
{
    W2BYTEMSK(REG_MHL_CBUS_10, BIT(15), BIT(15));
    
    while((R2BYTE(REG_MHL_CBUS_10) & BIT(14)) == BIT(14));
    
    return R2BYTE(REG_MHL_CBUS_0F);
}

#endif

#if(DMHL_TEST_SIGNAL_SUPPORT)
//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_AdjustCommonModeResistor()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_TestSignal(MS_BOOL bflag)
{
    if(bflag)
    {
        W2BYTEMSK(0x001106, BIT(10), BIT(10));
    }
    else
    {
        W2BYTEMSK(0x001106, 0, BIT(10));
    }
}

#endif

#endif // _MHAL_MHL_C_

