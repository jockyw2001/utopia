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
#if(defined(CONFIG_MLOG))
#include "ULog.h"

#define MHAL_MHL_MSG_INFO(format, args...)      ULOGI("MHL", format, ##args)
#define MHAL_MHL_MSG_WARNING(format, args...)   ULOGW("MHL", format, ##args)
#define MHAL_MHL_MSG_DEBUG(format, args...)     ULOGD("MHL", format, ##args)
#define MHAL_MHL_MSG_ERROR(format, args...)     ULOGE("MHL", format, ##args)
#define MHAL_MHL_MSG_FATAL(format, args...)     ULOGF("MHL", format, ##args)

#else
#define MHAL_MHL_MSG_INFO(format, args...)      printf(format, ##args)
#define MHAL_MHL_MSG_WARNING(format, args...)   printf(format, ##args)
#define MHAL_MHL_MSG_DEBUG(format, args...)     printf(format, ##args)
#define MHAL_MHL_MSG_ERROR(format, args...)     printf(format, ##args)
#define MHAL_MHL_MSG_FATAL(format, args...)     printf(format, ##args)

#endif

#define MHL_DEBUG_SEND_MSC          0
#define MHL_DEBUG_RECEIVE_EMSC      0

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
MS_U8 ucChipIDValue = MHL_CHIP_ID_MAXIM;
MS_VIRT MHL_XC_RIU_BASE;
MS_VIRT MHL_PM_RIU_BASE;

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
    {REG_MHL_CBUS_59, BMASK(15:0), 0xFBD0}, // timeout for a device receiving a packet within a command
    {REG_MHL_CBUS_5A, BMASK(15:0), 0x0001}, // modify to 130ms
    
    {REG_PM_MHL_CBUS_01, BMASK(5:2), 0}, // Not overwrite pull down resistor
    
#if(MHL_CBUS_OPERATION_MODE >= MHL_CBUS_HW_ISR_MODE)
    {REG_MHL_CBUS_23, BMASK(10:8) |BIT(4), BIT(10) |BIT(4)}, // [10]: enable HW auto response read_devcap request
    {REG_MHL_CBUS_6D, BMASK(11:0), BMASK(11:0)}, // Enable MHL HW mode
    {REG_MHL_CBUS_71, BMASK(15:14)| BMASK(11:10), BMASK(15:14)| BMASK(11:10)},
    {REG_MHL_CBUS_50, BIT(13), BIT(13)}, // [13]: MSC send command keep mode
#else
    {REG_MHL_CBUS_23, BMASK(10:8) |BIT(4), 0}, // [10]: enable HW auto response read_devcap request
    {REG_MHL_CBUS_6D, BMASK(11:0), 0}, // Enable MHL HW mode
    {REG_MHL_CBUS_71, BMASK(15:14)| BMASK(11:10), 0},
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
MS_BOOL _mhal_mhl_IsCbusBusy(MS_BOOL bECbusEnableFlag)
{
    MS_BOOL bBusyFlag = FALSE;
    
    if(!bECbusEnableFlag)
    {
        bBusyFlag = ((R2BYTE(REG_MHL_CBUS_5D) & 0x00F0) != 0 ? TRUE : FALSE);
    }
    return bBusyFlag;
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
            if(!bAutoEQFlag)
            {
                //if((R2BYTE(REG_DVI_DTOP_DUAL_P0_2F_L) &BIT(0)) == BIT(0))
                {
                    bAutoEQFlag = TRUE;
                }
            }

            if(bAutoEQFlag)
            {
                if(bFlag)
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_00_L, BIT(4), BIT(4)); // enable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                }
                else
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_00_L, 0, BIT(4)); // disable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_2F_L, 0, BIT(0)); // disable autoEQ controller
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(!bAutoEQFlag)
            {
                //if((R2BYTE(REG_DVI_DTOP_DUAL_P1_2F_L) &BIT(0)) == BIT(0))
                {
                    bAutoEQFlag = TRUE;
                }
            }

            if(bAutoEQFlag)
            {
                if(bFlag)
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_00_L, BIT(4), BIT(4)); // enable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                }
                else
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_00_L, 0, BIT(4)); // disable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_2F_L, 0, BIT(0)); // disable autoEQ controller
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(!bAutoEQFlag)
            {
                //if((R2BYTE(REG_DVI_DTOP_DUAL_P2_2F_L) &BIT(0)) == BIT(0))
                {
                    bAutoEQFlag = TRUE;
                }
            }

            if(bAutoEQFlag)
            {
                if(bFlag)
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_00_L, BIT(4), BIT(4)); // enable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                }
                else
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_00_L, 0, BIT(4)); // disable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_2F_L, 0, BIT(0)); // disable autoEQ controller
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(!bAutoEQFlag)
            {
                //if((R2BYTE(REG_DVI_DTOP_DUAL_P3_2F_L) &BIT(0)) == BIT(0))
                {
                    bAutoEQFlag = TRUE;
                }
            }

            if(bAutoEQFlag)
            {
                if(bFlag)
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_00_L, BIT(4), BIT(4)); // enable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2F_L, BIT(0), BIT(0)); // enable autoEQ controller
                }
                else
                {
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_00_L, 0, BIT(4)); // disable EQ new mode
                    //W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_2F_L, 0, BIT(0)); // disable autoEQ controller
                }
            }
            
            break;
#endif

        default:
            if(bAutoEQFlag)
            {

            }

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
void _mhal_mhl_HdmiBypassModeSetting(MS_U8 ucCbusSelect, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_27_L, 0, BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, BIT(13)| BMASK(3:0), BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P0_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P0_6F_L, 0, BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P0_45_L, (MHL_HDMI_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P0_11_L, 0, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L, BIT(8), BIT(8));

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P0_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                W2BYTEMSK(REG_COMBO_PHY0_P0_6B_L, (pMHLSignalStatus->ucImpedanceValue << 12), BMASK(15:12));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_27_L, 0, BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P1_73_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P1_5B_L, BIT(13)| BMASK(3:0), BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P1_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P1_6F_L, 0, BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P1_45_L, (MHL_HDMI_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P1_11_L, 0, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY0_P1_6E_L, BIT(8), BIT(8));

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P1_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                W2BYTEMSK(REG_COMBO_PHY0_P1_6B_L, (pMHLSignalStatus->ucImpedanceValue << 12), BMASK(15:12));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_27_L, 0, BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P2_73_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P2_5B_L, BIT(13)| BMASK(3:0), BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P2_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P2_6F_L, 0, BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P2_45_L, (MHL_HDMI_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P2_11_L, 0, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY0_P2_6E_L, BIT(8), BIT(8));

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P2_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                W2BYTEMSK(REG_COMBO_PHY0_P2_6B_L, (pMHLSignalStatus->ucImpedanceValue << 12), BMASK(15:12));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_27_L, 0, BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P3_73_L, MHL_HDMI_EQ_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P3_5B_L, BIT(13)| BMASK(3:0), BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P3_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P3_6F_L, 0, BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P3_45_L, (MHL_HDMI_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P3_11_L, 0, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY0_P3_6E_L, BIT(8), BIT(8));

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P3_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                W2BYTEMSK(REG_COMBO_PHY0_P3_6B_L, (pMHLSignalStatus->ucImpedanceValue << 12), BMASK(15:12));
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
void _mhal_mhl_Mhl24bitsModeSetting(MS_U8 ucCbusSelect, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    MS_U16 usImpedanceSetting = 0;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_27_L, BIT(6), BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P0_5B_L, 0, BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P0_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P0_6F_L, BIT(10), BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P0_6C_L, (MHL_ICONTROL_PD_20_VALUE << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P0_45_L, (MHL_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P0_11_L, BIT(12) |0x30, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_10_L, 0, BMASK(1:0));
            W2BYTE(REG_COMBO_PHY0_P0_60_L, 0);
            W2BYTE(REG_COMBO_PHY0_P0_61_L, 0);
            W2BYTE(REG_COMBO_PHY0_P0_62_L, 0);

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P0_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                if(pMHLSignalStatus->ucImpedanceOffset &BIT(7))
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue +pMHLSignalStatus->ucImpedanceOffset) << 12;
                }
                else
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue -pMHLSignalStatus->ucImpedanceOffset) << 12;
                }

                W2BYTEMSK(REG_COMBO_PHY0_P0_6B_L, usImpedanceSetting, BMASK(15:12));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_27_L, BIT(6), BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P1_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P1_5B_L, 0, BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P1_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P1_6F_L, BIT(10), BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P1_6C_L, (MHL_ICONTROL_PD_20_VALUE << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P1_45_L, (MHL_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P1_11_L, BIT(12) |0x30, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_10_L, 0, BMASK(1:0));
            W2BYTE(REG_COMBO_PHY0_P1_60_L, 0);
            W2BYTE(REG_COMBO_PHY0_P1_61_L, 0);
            W2BYTE(REG_COMBO_PHY0_P1_62_L, 0);

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P1_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                if(pMHLSignalStatus->ucImpedanceOffset &BIT(7))
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue +pMHLSignalStatus->ucImpedanceOffset) << 12;
                }
                else
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue -pMHLSignalStatus->ucImpedanceOffset) << 12;
                }

                W2BYTEMSK(REG_COMBO_PHY0_P1_6B_L, usImpedanceSetting, BMASK(15:12));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_27_L, BIT(6), BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P2_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P2_5B_L, 0, BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P2_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P2_6F_L, BIT(10), BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P2_6C_L, (MHL_ICONTROL_PD_20_VALUE << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P2_45_L, (MHL_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P2_11_L, BIT(12) |0x30, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_10_L, 0, BMASK(1:0));
            W2BYTE(REG_COMBO_PHY0_P2_60_L, 0);
            W2BYTE(REG_COMBO_PHY0_P2_61_L, 0);
            W2BYTE(REG_COMBO_PHY0_P2_62_L, 0);

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P2_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                if(pMHLSignalStatus->ucImpedanceOffset &BIT(7))
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue +pMHLSignalStatus->ucImpedanceOffset) << 12;
                }
                else
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue -pMHLSignalStatus->ucImpedanceOffset) << 12;
                }

                W2BYTEMSK(REG_COMBO_PHY0_P2_6B_L, usImpedanceSetting, BMASK(15:12));
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_27_L, BIT(6), BIT(6)| BIT(1)); // [6]: align, [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_COMBO_PHY0_P3_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P3_5B_L, 0, BIT(13)| BMASK(3:0));
            W2BYTEMSK(REG_HDCP_DUAL_P3_09_L, 0, BIT(0)); // PP mode + HDCP eanble
            W2BYTEMSK(REG_COMBO_PHY0_P3_6F_L, BIT(10), BIT(10)); // I-control PD overwrite write
            W2BYTEMSK(REG_COMBO_PHY0_P3_6C_L, (MHL_ICONTROL_PD_20_VALUE << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P3_45_L, (MHL_LOCK_TIME_VALUE << 8), BMASK(14:8)); // Digital lock time value
            W2BYTEMSK(REG_COMBO_PHY0_P3_11_L, BIT(12) |0x30, BMASK(12:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_10_L, 0, BMASK(1:0));
            W2BYTE(REG_COMBO_PHY0_P3_60_L, 0);
            W2BYTE(REG_COMBO_PHY0_P3_61_L, 0);
            W2BYTE(REG_COMBO_PHY0_P3_62_L, 0);

            if(pMHLSignalStatus->ucImpedanceValue == MHL_IMPEDANCE_VALUE)
            {
                pMHLSignalStatus->ucImpedanceValue = R2BYTE(REG_COMBO_PHY0_P3_6B_L) >> 12;
            }

            if(pMHLSignalStatus->ucImpedanceOffset > 0)
            {
                if(pMHLSignalStatus->ucImpedanceOffset &BIT(7))
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue +pMHLSignalStatus->ucImpedanceOffset) << 12;
                }
                else
                {
                    usImpedanceSetting = (pMHLSignalStatus->ucImpedanceValue -pMHLSignalStatus->ucImpedanceOffset) << 12;
                }

                W2BYTEMSK(REG_COMBO_PHY0_P3_6B_L, usImpedanceSetting, BMASK(15:12));
            }
            
            break;
#endif

        default:
            usImpedanceSetting = 0;
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
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_27_L, BIT(1), BIT(1)); // [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_HDCP_DUAL_P0_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_27_L, BIT(1), BIT(1)); // [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_HDCP_DUAL_P1_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_27_L, BIT(1), BIT(1)); // [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_HDCP_DUAL_P2_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_27_L, BIT(1), BIT(1)); // [1]: MHL pack-pixel mode
            W2BYTEMSK(REG_HDCP_DUAL_P3_09_L, BIT(0), BIT(0)); // PP mode + HDCP eanble

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
            if (rctrl == RX_HDMI_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BMASK(3:1));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(0));// clock R-term
            }
            else if (rctrl == RX_MHL_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(2), BMASK(3:1));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(0));// clock R-term
            }
            else if (rctrl == RX_RTERM_OFF)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BMASK(3:1), BMASK(3:1));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(0), BIT(0));// clock R-term
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if (rctrl == RX_HDMI_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BMASK(7:5));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(4));// clock R-term
            }
            else if (rctrl == RX_MHL_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(6), BMASK(7:5));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(4));// clock R-term
            }
            else if (rctrl == RX_RTERM_OFF)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BMASK(7:5), BMASK(7:5));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(4), BIT(4));// clock R-term
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if (rctrl == RX_HDMI_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BMASK(11:9));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(8));// clock R-term
            }
            else if (rctrl == RX_MHL_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(10), BMASK(11:9));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(8));// clock R-term
            }
            else if (rctrl == RX_RTERM_OFF)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BMASK(11:9), BMASK(11:9));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(8), BIT(8));// clock R-term
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if (rctrl == RX_HDMI_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BMASK(15:13));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(12));// clock R-term
            }
            else if (rctrl == RX_MHL_RTERM)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(14), BMASK(15:13));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, 0, BIT(12));// clock R-term
            }
            else if (rctrl == RX_RTERM_OFF)
            {
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BMASK(15:13), BMASK(15:13));// data R-term
                W2BYTEMSK(REG_PM_SLEEP_4C_L, BIT(12), BIT(12));// clock R-term
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
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_27_L, bflag ? 0 : BIT(0), BIT(0)); // [0]: MHL mac enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_27_L, bflag ? 0 : BIT(0), BIT(0)); // [0]: MHL mac enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_27_L, bflag ? 0 : BIT(0), BIT(0)); // [0]: MHL mac enable
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_27_L, bflag ? 0 : BIT(0), BIT(0)); // [0]: MHL mac enable
            break;
#endif

        default:

            break;
    };
}

#endif

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
            usClkCount = R2BYTE(REG_COMBO_PHY0_P0_3C_L) & 0x0FFF;

            if((R2BYTE(REG_COMBO_PHY0_P0_41_L) &BIT(9)) == BIT(9))
            {
                bStable = TRUE;

                if(usClkCount < CBUS_CLOCK_DETECT_LEVEL)
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            usClkCount = R2BYTE(REG_COMBO_PHY0_P1_3C_L) & 0x0FFF;

            if((R2BYTE(REG_COMBO_PHY0_P1_41_L) &BIT(9)) == BIT(9))
            {
                bStable = TRUE;

                if(usClkCount < CBUS_CLOCK_DETECT_LEVEL)
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            usClkCount = R2BYTE(REG_COMBO_PHY0_P2_3C_L) & 0x0FFF;

            if((R2BYTE(REG_COMBO_PHY0_P2_41_L) &BIT(9)) == BIT(9))
            {
                bStable = TRUE;

                if(usClkCount < CBUS_CLOCK_DETECT_LEVEL)
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            usClkCount = R2BYTE(REG_COMBO_PHY0_P3_3C_L) & 0x0FFF;

            if((R2BYTE(REG_COMBO_PHY0_P3_41_L) &BIT(9)) == BIT(9))
            {
                bStable = TRUE;

                if(usClkCount < CBUS_CLOCK_DETECT_LEVEL)
                {
                    bStable = FALSE;
                }
            }
            
            break;
#endif

        default:
            usClkCount = 0;
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
            W2BYTEMSK(REG_PM_SLEEP_4A_L, bFlag? BIT(8): 0, BIT(8));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_PM_SLEEP_4A_L, bFlag? BIT(9): 0, BIT(9));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_PM_SLEEP_4A_L, bFlag? BIT(10): 0, BIT(10));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_PM_SLEEP_4A_L, bFlag? BIT(11): 0, BIT(11));
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
    //W2BYTEMSK(REG_HDMI2_08_L, bMHLPath ?BIT(0) :0, BIT(0)); // [0]: audio source selection, 0: HDMI / 1: MHL
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
void _mhal_mhl_CbusAndClockSelect(MS_U8 ucCbusSelect)
{
    MS_U8 bSupportFlag = FALSE;
    MS_U8 ucClockSelect = MHL_DVI_PORT_A;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            ucClockSelect = MHL_DVI_PORT_A;
            bSupportFlag = TRUE;

            W2BYTEMSK(REG_COMBO_PHY0_P0_45_L, 0x6, BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY0_P0_47_L, 0x33, BMASK(9:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_18_L, 0x361, BMASK(9:0));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            ucClockSelect = MHL_DVI_PORT_B;
            bSupportFlag = TRUE;

            W2BYTEMSK(REG_COMBO_PHY0_P1_45_L, 0x6, BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY0_P1_47_L, 0x33, BMASK(9:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_18_L, 0x361, BMASK(9:0));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            ucClockSelect = MHL_DVI_PORT_C;
            bSupportFlag = TRUE;

            W2BYTEMSK(REG_COMBO_PHY0_P2_45_L, 0x6, BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY0_P2_47_L, 0x33, BMASK(9:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_18_L, 0x361, BMASK(9:0));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            ucClockSelect = MHL_DVI_PORT_D;
            bSupportFlag = TRUE;

            W2BYTEMSK(REG_COMBO_PHY0_P3_45_L, 0x6, BMASK(3:0));
            W2BYTEMSK(REG_COMBO_PHY0_P3_47_L, 0x33, BMASK(9:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_18_L, 0x361, BMASK(9:0));

            break;
#endif

        default:

            break;
    };

    if(bSupportFlag)
    {
        W2BYTEMSK(REG_COMBO_GP_TOP_33_L, ucClockSelect, BMASK(2:0)); // [2:0]: mhl port select
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ForcePullDown100K()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ForcePullDown100K(MS_BOOL bEnable)
{
    W2BYTEMSK(REG_PM_MHL_CBUS_20, bEnable? BIT(7): 0, BIT(7)); 
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetEMSCOneByteCRC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mhal_mhl_GetEMSCOneByteCRC(MS_U8 ucOpCode)
{
    MS_U8 ucAValue = 0;
    MS_U8 ucBValue = 0;
    MS_U8 ucCommand = BIT(7) |BIT(6) |((ucOpCode &BMASK(2:0)) << 2);

    ucAValue = GET_BIT(ucCommand &BIT(7)) +GET_BIT(ucCommand &BIT(5)) +GET_BIT(ucCommand &BIT(3)) +2;
    ucBValue = GET_BIT(ucCommand &BIT(6)) +GET_BIT(ucCommand &BIT(4)) +GET_BIT(ucCommand &BIT(2)) +2;

    if(ucAValue &BIT(0))
    {
        ucCommand |= BIT(1);
    }

    if(ucBValue &BIT(0))
    {
        ucCommand |= BIT(0);
    }

    return ucCommand;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetEMSCTwoByteCRC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mhal_mhl_GetEMSCTwoByteCRC(MS_U8 ucOpCode, MS_U8 ucValue)
{
    MS_U8 ucAValue = 0;
    MS_U8 ucBValue = 0;
    MS_U8 ucCValue = 0;
    MS_U16 ucCommand = BIT(13) |(((MS_U16)ucOpCode &BMASK(1:0)) << 11) |(((MS_U16)ucValue &BMASK(7:0)) << 3);

    ucAValue = GET_BIT(ucCommand &BIT(15)) +GET_BIT(ucCommand &BIT(13)) +GET_BIT(ucCommand &BIT(12)) +GET_BIT(ucCommand &BIT(11)) +GET_BIT(ucCommand &BIT(8));
    ucAValue = ucAValue +GET_BIT(ucCommand &BIT(6)) +GET_BIT(ucCommand &BIT(5)) +GET_BIT(ucCommand &BIT(4)) +3;
    ucBValue = GET_BIT(ucCommand &BIT(15)) +GET_BIT(ucCommand &BIT(14)) +GET_BIT(ucCommand &BIT(13)) +GET_BIT(ucCommand &BIT(10)) +GET_BIT(ucCommand &BIT(8));
    ucBValue = ucBValue +GET_BIT(ucCommand &BIT(7)) +GET_BIT(ucCommand &BIT(6)) +GET_BIT(ucCommand &BIT(3)) +2;
    ucCValue = GET_BIT(ucCommand &BIT(14)) +GET_BIT(ucCommand &BIT(13)) +GET_BIT(ucCommand &BIT(12)) +GET_BIT(ucCommand &BIT(9)) +GET_BIT(ucCommand &BIT(7));
    ucCValue = ucCValue +GET_BIT(ucCommand &BIT(6)) +GET_BIT(ucCommand &BIT(5)) +2;

    if(ucAValue &BIT(0))
    {
        ucCommand |= BIT(2);
    }

    if(ucBValue &BIT(0))
    {
        ucCommand |= BIT(1);
    }

    if(ucCValue &BIT(0))
    {
        ucCommand |= BIT(0);
    }

    return (ucCommand &BMASK(7:0));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetEMSCPayloadCRC()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 _mhal_mhl_GetEMSCPayloadCRC(MS_U8 ucOpCode, MS_U16 usCRCValus)
{
    MS_U8 ucAValue = 0;
    MS_U8 ucBValue = 0;
    MS_U8 ucCValue = GET_BIT(ucOpCode &BIT(7)) +GET_BIT(ucOpCode &BIT(6)) +GET_BIT(usCRCValus &BIT(6)) +GET_BIT(usCRCValus &BIT(7));
    MS_U8 ucDValue = GET_BIT(ucOpCode &BIT(6)) +GET_BIT(ucOpCode &BIT(5)) +GET_BIT(usCRCValus &BIT(5)) +GET_BIT(usCRCValus &BIT(6));
    MS_U8 ucEValue = GET_BIT(ucOpCode &BIT(5)) +GET_BIT(ucOpCode &BIT(4)) +GET_BIT(usCRCValus &BIT(4)) +GET_BIT(usCRCValus &BIT(5));
    MS_U8 ucFValue = GET_BIT(ucOpCode &BIT(4)) +GET_BIT(ucOpCode &BIT(3)) +GET_BIT(usCRCValus &BIT(3)) +GET_BIT(usCRCValus &BIT(4));
    MS_U8 ucGValue = GET_BIT(ucOpCode &BIT(3)) +GET_BIT(ucOpCode &BIT(2)) +GET_BIT(usCRCValus &BIT(2)) +GET_BIT(usCRCValus &BIT(3));
    MS_U8 ucHValue = GET_BIT(ucOpCode &BIT(2)) +GET_BIT(ucOpCode &BIT(1)) +GET_BIT(usCRCValus &BIT(1)) +GET_BIT(usCRCValus &BIT(2));
    MS_U8 ucIValue = GET_BIT(ucOpCode &BIT(1)) +GET_BIT(ucOpCode &BIT(0)) +GET_BIT(usCRCValus &BIT(0)) +GET_BIT(usCRCValus &BIT(1)) +GET_BIT(usCRCValus &BIT(15));
    MS_U8 ucJValue = GET_BIT(ucOpCode &BIT(0)) +GET_BIT(usCRCValus &BIT(0)) +GET_BIT(usCRCValus &BIT(14));
    MS_U8 ucKValue = GET_BIT(usCRCValus &BIT(13));
    MS_U8 ucLValue = GET_BIT(usCRCValus &BIT(12));
    MS_U8 ucMValue = GET_BIT(usCRCValus &BIT(11));
    MS_U8 ucNValue = GET_BIT(usCRCValus &BIT(10));
    MS_U8 ucOValue = GET_BIT(usCRCValus &BIT(9));
    MS_U8 ucPValue = 0;
    MS_U16 usCommand = 0;

    ucAValue = GET_BIT(ucOpCode &BIT(7)) +GET_BIT(ucOpCode &BIT(6)) +GET_BIT(ucOpCode &BIT(5)) +GET_BIT(ucOpCode &BIT(4)) +GET_BIT(ucOpCode &BIT(3)) +GET_BIT(ucOpCode &BIT(2)) +GET_BIT(ucOpCode &BIT(1)) +GET_BIT(ucOpCode &BIT(0));
    ucAValue = ucAValue +GET_BIT(usCRCValus &BIT(0)) +GET_BIT(usCRCValus &BIT(1)) +GET_BIT(usCRCValus &BIT(2)) +GET_BIT(usCRCValus &BIT(3)) +GET_BIT(usCRCValus &BIT(4)) +GET_BIT(usCRCValus &BIT(5)) +GET_BIT(usCRCValus &BIT(6)) +GET_BIT(usCRCValus &BIT(7));
    ucBValue = GET_BIT(ucOpCode &BIT(6)) +GET_BIT(ucOpCode &BIT(5)) +GET_BIT(ucOpCode &BIT(4)) +GET_BIT(ucOpCode &BIT(3)) +GET_BIT(ucOpCode &BIT(2)) +GET_BIT(ucOpCode &BIT(1)) +GET_BIT(ucOpCode &BIT(0));
    ucBValue = ucBValue +GET_BIT(usCRCValus &BIT(0)) +GET_BIT(usCRCValus &BIT(1)) +GET_BIT(usCRCValus &BIT(2)) +GET_BIT(usCRCValus &BIT(3)) +GET_BIT(usCRCValus &BIT(4)) +GET_BIT(usCRCValus &BIT(5)) +GET_BIT(usCRCValus &BIT(6));
    ucPValue = ucAValue +GET_BIT(usCRCValus &BIT(8));

    if(ucAValue &BIT(0))
    {
        usCommand |= BIT(15);
    }

    if(ucBValue &BIT(0))
    {
        usCommand |= BIT(14);
    }

    if(ucCValue &BIT(0))
    {
        usCommand |= BIT(13);
    }

    if(ucDValue &BIT(0))
    {
        usCommand |= BIT(12);
    }

    if(ucEValue &BIT(0))
    {
        usCommand |= BIT(11);
    }

    if(ucFValue &BIT(0))
    {
        usCommand |= BIT(10);
    }

    if(ucGValue &BIT(0))
    {
        usCommand |= BIT(9);
    }

    if(ucHValue &BIT(0))
    {
        usCommand |= BIT(8);
    }

    if(ucIValue &BIT(0))
    {
        usCommand |= BIT(7);
    }

    if(ucJValue &BIT(0))
    {
        usCommand |= BIT(6);
    }

    if(ucKValue &BIT(0))
    {
        usCommand |= BIT(5);
    }

    if(ucLValue &BIT(0))
    {
        usCommand |= BIT(4);
    }

    if(ucMValue &BIT(0))
    {
        usCommand |= BIT(3);
    }

    if(ucNValue &BIT(0))
    {
        usCommand |= BIT(2);
    }

    if(ucOValue &BIT(0))
    {
        usCommand |= BIT(1);
    }

    if(ucPValue &BIT(0))
    {
        usCommand |= BIT(0);
    }

    return usCommand;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusInitialSetting()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusInitialSetting(void)
{
    W2BYTEMSK(REG_MHL_ECBUS_PHY_69, 0x35, BMASK(6:4)| BMASK(2:0)); // [6:4]: rasing pattern type, [2:0]: deglitch after reorder

    W2BYTEMSK(REG_MHL_ECBUS_PHY_4C, 0x8880, BMASK(15:4)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4D, 0x8B14, BMASK(15:1)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4E, 0x001C, BMASK(15:0)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_51, 0x0100, BMASK(15:0)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_56, 0x0200, BMASK(15:0));  
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BMASK(14:13), BMASK(14:13)); // [14:13]
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BMASK(14:13), BMASK(14:13)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_55, 0, BMASK(15:4)); //[7:0]
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4F, 0x00F0, BMASK(15:0)); // [7:4]:reg_ecbss_afe_tx_dri_step
    W2BYTEMSK(REG_MHL_ECBUS_PHY_78, BIT(0), BIT(0)); 

    W2BYTEMSK(REG_MHL_ECBUS_0E, 8, BMASK(4:0));
    W2BYTEMSK(REG_MHL_ECBUS_2D, BIT(3), BIT(3));

    //auto reset turn on
    W2BYTEMSK(REG_MHL_ECBUS_PHY_7F, BIT(2), BIT(2)); // [2]:reg_rst_aft_clklck_en
    W2BYTEMSK(REG_MHL_ECBUS_00, BMASK(4:3), BMASK(4:3)); // [4]:reg_rst_aft_fail_en, [3]:reg_rst_aft_clklck_en

    // ECbus initial
    W2BYTEMSK(REG_MHL_ECBUS_02, BIT(7), BMASK(13:8)| BIT(7)); // [7]
    W2BYTEMSK(REG_MHL_ECBUS_PHY_6D, BIT(5), BMASK(6:5)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_57, 0x700, BMASK(11:8)); // [11:8]:
    W2BYTEMSK(REG_MHL_ECBUS_PHY_68, 0x30, BMASK(6:4)); // [6:4]: pre-deglitch 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_69, 0x50, BMASK(6:4)| BMASK(2:0)); // [6:4]: rasing pattern type, [2:0]: deglitch after reorder
    W2BYTEMSK(REG_MHL_ECBUS_PHY_65, 0x08, BMASK(5:0)); // [5:0]: reg_dat_lsb_mask; after analog rwa data deglicth,re-order,the lsb data 1 will be mask

    //U02 ECO item setting
    W2BYTEMSK(REG_MHL_ECBUS_77, (_mhal_mhl_GetEMSCOneByteCRC(5) << 8)| _mhal_mhl_GetEMSCOneByteCRC(4), BMASK(15:0)); // [15:8]:EMSC neck, [7:0]:EMSC ack
    W2BYTEMSK(REG_MHL_ECBUS_78, (_mhal_mhl_GetEMSCOneByteCRC(7) << 8)| _mhal_mhl_GetEMSCOneByteCRC(6), BMASK(15:0)); // [15:8]:Dstart, [7:0]:grant
    W2BYTEMSK(REG_MHL_ECBUS_79, BIT(11)| (_mhal_mhl_GetEMSCTwoByteCRC(1, 1) << 3)| (_mhal_mhl_GetEMSCTwoByteCRC(1, 2) << 6), BIT(11)| BMASK(8:6)| BMASK(5:3)); // [8:6]:EMSC errsum cmd CRC, [5:3]:EMSC errCRC cmd CRC
    W2BYTEMSK(REG_MHL_ECBUS_PHY_13, BIT(4), BIT(4)); // 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_51, BIT(11), BIT(11)); // 

    //eCBUS fsm timing 
    W2BYTEMSK(REG_MHL_ECBUS_04, 0xE0E, BMASK(14:8)| BMASK(6:0)); // [14:8]: reg_t_sink_cal, [6:0]: reg_t_snk_cal 1; sink sends eCBUS clock after detecting eCBUS clock form source.
    W2BYTEMSK(REG_MHL_ECBUS_06, 0x1C00, BMASK(14:8)); // [14:8]: reg_t_snk_tdm; 
    W2BYTEMSK(REG_MHL_ECBUS_03, BMASK(15:14), BMASK(15:14)); // [15]:reg_wait_tdm_timer_dis [14]:reg_wait_comma2_timer_dis
    W2BYTEMSK(REG_MHL_ECBUS_07, 0x104, BMASK(14:8)| BMASK(6:0)); // [14:8]: reg_t_snk_tdm; 
    W2BYTEMSK(REG_MHL_ECBUS_3F, 0, BMASK(1:0)); //  
    W2BYTEMSK(REG_MHL_ECBUS_48, 0, BMASK(2:0)); //

    //clock detect setting 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_70, 0x032B, BMASK(15:0)); // [15:0]:reg_crlock_mid; the CR lock reference value
    W2BYTEMSK(REG_MHL_ECBUS_PHY_71, 0x0C04, BMASK(15:0)); // [15:8]:reg_crlock_thr, the CR lock reference failure threshold range; [7:0]:reg_crlock_thr, the CR lock reference correct threshold range;
    W2BYTEMSK(REG_MHL_ECBUS_PHY_72, 0x0007, BMASK(15:0)); // [12:8]:reg_cr_unlock_num, clock unlock threshold; [4:0]:reg_cr_lock_num; CR lock success threshold time.

#if(MHL_ECBUS_AUTO_TRAINING)
    //auto training testing
    W2BYTEMSK(REG_MHL_ECBUS_PHY_5C, 0x010A, BMASK(15:0)); // [10:8]:reg_txloc_golden_lowr_tol,    [5:0]:reg_txloc_golden_lower, 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_66, 0x2117, BMASK(15:0)); // [14:12]:reg_txloc_golden2_tol,  [10:8]:reg_txloc_rev_golden_tol,    [5:0]:reg_txloc_rev_golden
    W2BYTEMSK(REG_MHL_ECBUS_PHY_67, 0x0403, BMASK(15:0)); // [14:12]:reg_val_aft_adj_ok, [10:8]:reg_txloc_txdelay_add,   [6:4]:reg_flow_retry_r, [2:0]:reg_flow_retry
    W2BYTEMSK(REG_MHL_ECBUS_PHY_69, BIT(10), BMASK(11:10)); // [11:10]:reg_txloc_set_time
    W2BYTEMSK(REG_MHL_ECBUS_PHY_6A, 0xf117, BMASK(15:0)); // [15:12]:reg_txloc_lck_thr, [10:8]:reg_txloc_golden_tol, [5:0]:ret_txloc_golden
#else
    // Skip atop training
    // REG_PM_MHL_CBUS2_4A  [14]tx_rep_pd_ove  = 1 [13]tx_rep_pd_ov  [8]:tx_width_ove = 1 [7:4]:tx_width_ov
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BIT(8)| 0x22, BIT(8)| BMASK(7:4));

    // REG_PM_MHL_CBUS2_4B ,[14]tx_pd_ove  = 1        [13]tx_pd_ov         [8]:tx_delay_ove = 1  [7:4]:tx_delay_ov
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BIT(8)| 0x00, BIT(8)| BMASK(7:4)); 

    //W2BYTEMSK(REG_MHL_ECBUS_PHY_6C, 0x8000, BMASK(15:8)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_6D, BIT(8), BIT(8)); 

    W2BYTEMSK(REG_MHL_ECBUS_PHY_6C, 0x0000, BMASK(15:0)); 
    //W2BYTEMSK(REG_MHL_ECBUS_04, (30 << 8), BMASK(14:8));
#endif
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetECbusStateChangeInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetECbusStateChangeInterrupt(MS_BOOL bEnableFlag)
{
    if(bEnableFlag)
    {
        W2BYTEMSK(REG_MHL_ECBUS_36, 0, BIT(0)); // Ecbus state change int mask
    }
    else
    {
        W2BYTEMSK(REG_MHL_ECBUS_36, BIT(0), BIT(0)); // Ecbus state change int mask
    }

    W2BYTEMSK(REG_MHL_ECBUS_34, BIT(0), BIT(0)); // Ecbus state change int clear
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetEMSCReceiveInterrupt()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetEMSCReceiveInterrupt(MS_BOOL bEnableFlag)
{
    W2BYTEMSK(REG_MHL_ECBUS_36, bEnableFlag? 0: BIT(12), BIT(12)); // eMSC receive int mask
    W2BYTEMSK(REG_MHL_ECBUS_34, BIT(12), BIT(12)); // eMSC receive int clear
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusConnectCheckEnable()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusConnectCheckEnable(MS_BOOL bEnableFlag)
{
    W2BYTEMSK(REG_PM_MHL_CBUS_0B, bEnableFlag? MHL_CBUS_CONNECT_CHECK_VALUE: 0, BMASK(15:0)); // [15:0]: Disable connection check
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusEnableSetting()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusEnableSetting(MS_BOOL bEnableFlag)
{
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4D, bEnableFlag? 0: BIT(0), BIT(0)); // ECbus PLL pd
    //W2BYTEMSK(REG_MHL_ECBUS_PHY_55, bEnableFlag? 0: BIT(14), BIT(14)); // Rx power down
    W2BYTEMSK(REG_MHL_ECBUS_PHY_57, bEnableFlag? BIT(3): BIT(2), BMASK(3:2)); //  
    W2BYTEMSK(REG_MHL_ECBUS_PHY_6F, bEnableFlag? BIT(8): 0, BIT(8)); // ECbus clock detect
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, bEnableFlag? 0: BIT(13), BIT(13));
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, bEnableFlag? 0: BIT(13), BIT(13));

    if(bEnableFlag)
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(13), BIT(13)); // [13]: ECbus on
    }
    else
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(14), BIT(14)); // [14]: ECbus off
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusBISTSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ECbusBISTSetting(MS_BOOL bEnableFlag, stMHL_BIST_MODE_INFO *pBISTParameterInfo)
{
    if(bEnableFlag)
    {
        if(pBISTParameterInfo->bBISTeCbusRxFlag || pBISTParameterInfo->bBISTeCbusTxFlag)
        {
#if(MHL_ECBUS_AUTO_TRAINING)
            W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, 0, BIT(13));
            W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, 0, BIT(13));
#else
            W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BIT(14), BMASK(14:13));
            W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BIT(14), BMASK(14:13));
#endif
        }
        else
        {
            switch(pBISTParameterInfo->ucBISTImpedanceMode)
            {
                case MHL_BIST_IMPEDANCE_AV_LINK_TX_LOW:
                case MHL_BIST_IMPEDANCE_ECBUS_S_RX:
                    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BMASK(14:13), BMASK(14:13));
                    W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BMASK(14:13), BMASK(14:13));
                    break;

                case MHL_BIST_IMPEDANCE_AV_LINK_TX_HIGH:
                    W2BYTEMSK(REG_MHL_ECBUS_PHY_56, BIT(8), BIT(8));
                    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BMASK(14:13), BMASK(14:13));
                    W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, 0, BMASK(14:13));
                    break;

                default:
                    break;
            };
        }
    }
    else
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(14), BIT(14)); // [14]: ECbus off

#if(MHL_ECBUS_AUTO_TRAINING)
        W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BIT(13), BIT(13));
        W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BIT(13), BIT(13));
#else
        W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BMASK(14:13), BMASK(14:13));
        W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BMASK(14:13), BMASK(14:13));
#endif
        W2BYTEMSK(REG_MHL_ECBUS_PHY_56, 0, BIT(8));
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusDmuxEnable()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusDmuxEnable(MS_BOOL bEnableFlag)
{
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4F, bEnableFlag? BIT(3): 0, BIT(3)); // [3]: Enable ECbus dmux
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CbusEngineReset()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_CbusEngineReset(void)
{
    W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(0), BIT(0));
    W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(0));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusStateReset()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusStateReset(void)
{
    W2BYTEMSK(REG_MHL_ECBUS_00, BMASK(1:0), BMASK(1:0)); // ECbus state reset
    W2BYTEMSK(REG_MHL_ECBUS_00, 0, BMASK(1:0)); // ECbus state reset
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetShortReadAddress()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_SetShortReadAddress(MS_BOOL bECbusEnable)
{
    W2BYTEMSK(REG_MHL_CBUS_17, bECbusEnable? 0x7000: 0x0800, BMASK(15:8)); // [15:8]: reg_ddc_hdcp_short_read_address
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusStateOverwrite()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ECbusStateOverwrite(MS_BOOL bEnableFlag)
{
    //W2BYTEMSK(REG_MHL_ECBUS_03, bEnableFlag? (BIT(8)| 0x8): 0, BIT(8)| BMASK(3:0));
    W2BYTEMSK(REG_MHL_ECBUS_0F, bEnableFlag? (BIT(8)| 0x4): 0, BIT(8)| BMASK(3:0));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_Version3PhyEnable()
//  [Description]
//                  
//  [Arguments]:
//                  
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_Version3PhyEnable(MS_U8 ucCbusSelect, MS_BOOL bEnableFlag)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_60_L, bEnableFlag? BMASK(10:8): 0, BMASK(10:8)); // [8]: MHL3 engine enable
            W2BYTEMSK(REG_COMBO_PHY0_P0_73_L, bEnableFlag? MHL_EQ_30_SETTING_VALUE: MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P0_6C_L, ((bEnableFlag? MHL_ICONTROL_PD_30_VALUE: MHL_ICONTROL_PD_20_VALUE) << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P0_60_L, bEnableFlag? 0: BIT(5), BIT(5));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_60_L, bEnableFlag? BMASK(10:8): 0, BMASK(10:8)); // [8]: MHL3 engine enable
            W2BYTEMSK(REG_COMBO_PHY0_P1_73_L, bEnableFlag? MHL_EQ_30_SETTING_VALUE: MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P1_6C_L, ((bEnableFlag? MHL_ICONTROL_PD_30_VALUE: MHL_ICONTROL_PD_20_VALUE) << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P1_60_L, bEnableFlag? 0: BIT(5), BIT(5));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_60_L, bEnableFlag? BMASK(10:8): 0, BMASK(10:8)); // [8]: MHL3 engine enable
            W2BYTEMSK(REG_COMBO_PHY0_P2_73_L, bEnableFlag? MHL_EQ_30_SETTING_VALUE: MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P2_6C_L, ((bEnableFlag? MHL_ICONTROL_PD_30_VALUE: MHL_ICONTROL_PD_20_VALUE) << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P2_60_L, bEnableFlag? 0: BIT(5), BIT(5));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_60_L, bEnableFlag? BMASK(10:8): 0, BMASK(10:8)); // [8]: MHL3 engine enable
            W2BYTEMSK(REG_COMBO_PHY0_P3_73_L, bEnableFlag? MHL_EQ_30_SETTING_VALUE: MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            W2BYTEMSK(REG_COMBO_PHY0_P3_6C_L, ((bEnableFlag? MHL_ICONTROL_PD_30_VALUE: MHL_ICONTROL_PD_20_VALUE) << 9), BMASK(13:9)); // I-control PD
            W2BYTEMSK(REG_COMBO_PHY0_P3_60_L, bEnableFlag? 0: BIT(5), BIT(5));

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_SetECbusBISTTrigger()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_SetECbusBISTTrigger(MS_BOOL bEnableFlag, MS_BOOL bBISTeCbusTxFlag, MS_BOOL bBISTeCbusRxFlag)
{
    if(bBISTeCbusTxFlag)
    {
        if(bEnableFlag)
        {
            W2BYTEMSK(REG_MHL_ECBUS_3D, BIT(1), BIT(1));
            W2BYTEMSK(REG_MHL_ECBUS_2A, BIT(12), BMASK(13:12));
        }
        else
        {
            W2BYTEMSK(REG_MHL_ECBUS_2A, BIT(13), BMASK(13:12));
            W2BYTEMSK(REG_MHL_ECBUS_3D, 0, BIT(1));
        }
    }

    if(bBISTeCbusRxFlag)
    {
        W2BYTEMSK(REG_MHL_ECBUS_2E, bEnableFlag? BIT(12): BIT(13), BMASK(13:12));
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusStateChangeProc()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_ECbusStateChangeProc(stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus, stMHL_BIST_MODE_INFO *pBISTParameterInfo)
{
    MS_U8 ucECbusTrainState = (R2BYTE(REG_MHL_ECBUS_03) &0xF0) >> 4; // [7:4]: ECbus state

    if(ucECbusTrainState != pMHLSignalStatus->ucECbusTrainState)
    {
        if(ucECbusTrainState == MHL_ECBUS_STATE_ACTIVE)
        {
            if(pMHLSignalStatus->bBISTEnableFlag)
            {
                _mhal_mhl_SetECbusBISTTrigger(TRUE, pBISTParameterInfo->bBISTeCbusTxFlag, pBISTParameterInfo->bBISTeCbusRxFlag);
            }
        }

        pMHLSignalStatus->ucECbusTrainState = ucECbusTrainState;
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusModeUpProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ECbusModeUpProc(void)
{
    W2BYTEMSK(REG_MHL_ECBUS_34, BIT(4), BIT(4)); // [4]: clear PLL lock status
    W2BYTEMSK(REG_MHL_ECBUS_34, 0, BIT(4)); // [4]: clear PLL lock status 

    //W2BYTEMSK(REG_MHL_ECBUS_PHY_56, 0x0207, BMASK(15:0));
#if(MHL_ECBUS_AUTO_TRAINING)
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BIT(13), BIT(13));
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4B, BMASK(14:12), BMASK(14:12)); // delay
#endif
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ECbusModeDownProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ECbusModeDownProc(void)
{
    //W2BYTEMSK(REG_MHL_ECBUS_PHY_56, 0x0207, BMASK(15:0)); 
    W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, BIT(13), BIT(13));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_MHL3MuxSetting0()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
void _mhal_mhl_MHL3MuxSetting0(MS_BOOL bLinkRate6GFlag)
{
    W2BYTEMSK(REG_HDMI2_DUAL_0_54_L, bLinkRate6GFlag? 0: BIT(1), BMASK(1:0)); // [1:0]: reg_avg_ctrl_case
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetSRAMReceiveEMSCData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 _mhal_mhl_GetSRAMReceiveEMSCData(void)
{
    W2BYTEMSK(REG_MHL_ECBUS_23, BIT(15), BIT(15));

    while((R2BYTE(REG_MHL_ECBUS_23) & BIT(14)) == BIT(14));

    return (R2BYTE(REG_MHL_ECBUS_22) &BMASK(7:0));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetSRAMReceiveEMSCData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_InsertSRAMSendEMSCData(MS_U8 ucSendEMSC)
{
    W2BYTEMSK(REG_MHL_ECBUS_20, ucSendEMSC, BMASK(7:0));

    W2BYTEMSK(REG_MHL_ECBUS_21, BIT(15), BIT(15));

    while((R2BYTE(REG_MHL_ECBUS_21) & BIT(14)) == BIT(14));
}

#if(MHL_ECBUS_COMMAND_PARSING)
//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_ParsingECbusCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_ParsingECbusCommand(MS_U16 *usECbusData)
{
    MS_U8 ucTimes = 0;
    MS_U16 ustemp = 0;
    MS_U16 usCounnter = 0;
    MS_U16 usValue = 0;

    for(ustemp = 0; ustemp < MHL_BUFFER_ECBUS_DATA_LENGTH; ustemp++)
    {
        if((usECbusData[ustemp] &BMASK(7:0)) == 0x39) // Find CBUS1_START
        {
            ucTimes = 0;

            if(usECbusData[ustemp] &BIT(15)) // Send case
            {
                for(usCounnter = ustemp +1; usCounnter < MHL_BUFFER_ECBUS_DATA_LENGTH; usCounnter++)
                {
                    if(usECbusData[usCounnter] &BIT(15))
                    {
                        usValue = usValue << 8;
                        usValue = usValue |(usECbusData[usCounnter] &BMASK(7:0));

                        ucTimes++;
                    }

                    if(ucTimes == 2)
                    {
                        usValue = usValue >> 5;
                        ucTimes = usValue >> 8;
                        
                        if(ucTimes == 5)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SK][MSC][C] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else if(ucTimes == 4)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SK][MSC][D] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else if(ucTimes == 1)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SK][DDC][C] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else if(ucTimes == 0)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SK][DDC][D] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SK] unknow = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        
                        break;
                    }
                }
            }
            else // Receive case
            {
                for(usCounnter = ustemp +1; usCounnter < MHL_BUFFER_ECBUS_DATA_LENGTH; usCounnter++)
                {
                    if((usECbusData[usCounnter] &BIT(15)) == 0)
                    {
                        usValue = usValue << 8;
                        usValue = usValue |(usECbusData[usCounnter] &BMASK(7:0));

                        ucTimes++;
                    }

                    if(ucTimes == 2)
                    {
                        usValue = usValue >> 5;
                        ucTimes = usValue >> 8;
                        
                        if(ucTimes == 5)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SO][MSC][C] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else if(ucTimes == 4)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SO][MSC][D] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else if(ucTimes == 1)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SO][DDC][C] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else if(ucTimes == 0)
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SO][DDC][D] = %x\r\n", (usValue &BMASK(7:0)));
                        }
                        else
                        {
                            MHAL_MHL_MSG_DEBUG(" MHL [SO] unknow = %x\r\n", (usValue &BMASK(7:0)));
                        }

                        break;
                    }
                }
            }
        }
    }
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetECbusCommand()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_GetECbusCommand(void)
{
    MS_BOOL bReDoFlag = TRUE;
    MS_U16 usReceiveData = 0;
    MS_U16 usPreReceiveData = 0;
    MS_U16 usECbusCData[MHL_BUFFER_ECBUS_DATA_LENGTH] = {0};
    MS_U32 ulTimerValue[MHL_BUFFER_ECBUS_DATA_LENGTH] = {0};
    MS_U16 ustemp = 0;
    MS_U16 usCounnter = 0;
    static MS_U32 ulTimeout = 0;

    if(ulTimeout < 40000000)
    {
        do
        {
            usReceiveData = R2BYTE(REG_MHL_ECBUS_7C);

            if(usReceiveData != usPreReceiveData)
            {
                usECbusCData[ustemp] = usReceiveData;
                ulTimerValue[ustemp] = ulTimeout;
                ustemp++;
                
                usPreReceiveData = usReceiveData;
                bReDoFlag = TRUE;
                usCounnter = 0;
            }

            if(ulTimeout < 40000000)
            {
                ulTimeout++;
            }
            else
            {
                for(usCounnter = 0; usCounnter < ustemp; usCounnter++)
                {
                    MHAL_MHL_MSG_DEBUG(" MHL eCbus data = %x, %d\r\n", usECbusCData[usCounnter], ulTimerValue[usCounnter]);
                }
                
                bReDoFlag = FALSE;
                ustemp = 0;

                //_mhal_mhl_ParsingECbusCommand(usECbusCData);
            }
        }while(bReDoFlag);
    }
}

#endif

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_BISTECbusEnable()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_BISTECbusEnable(MS_BOOL bEnableFlag)
{
    W2BYTEMSK(REG_MHL_ECBUS_7A, bEnableFlag? BIT(5)| BIT(4)| BMASK(3:0): 0, BIT(5)| BIT(4)| BMASK(3:0));
    W2BYTEMSK(REG_MHL_ECBUS_06, bEnableFlag? 0x1800: 0x1C00, BMASK(14:8));
    W2BYTEMSK(REG_MHL_ECBUS_2D, bEnableFlag? BIT(4): BIT(5), BMASK(5:4));
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_BISTAVLinkEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_BISTAVLinkEnable(MS_U8 ucCbusSelect, MS_BOOL bEnableFlag)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_76_L, bEnableFlag? BIT(0): BIT(2), BIT(2)| BIT(0));

            if(bEnableFlag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_76_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_76_L, 0, BIT(1));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_76_L, bEnableFlag? BIT(0): BIT(2), BIT(2)| BIT(0));

            if(bEnableFlag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_76_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_76_L, 0, BIT(1));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_76_L, bEnableFlag? BIT(0): BIT(2), BIT(2)| BIT(0));

            if(bEnableFlag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_76_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_76_L, 0, BIT(1));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_76_L, bEnableFlag? BIT(0): BIT(2), BIT(2)| BIT(0));

            if(bEnableFlag)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_76_L, BIT(1), BIT(1));
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_76_L, 0, BIT(1));
            }

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetBISTECbusErrorCount()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 _mhal_mhl_GetBISTECbusErrorCount(void)
{
    MS_U16 ucECbusErrorCount = 0;

    if(R2BYTE(REG_MHL_ECBUS_30) &BIT(15))
    {
        ucECbusErrorCount = R2BYTE(REG_MHL_ECBUS_31);
        MHAL_MHL_MSG_DEBUG("** MHL eCbus error count vaild %x\r\n", ucECbusErrorCount);
    }

    return ucECbusErrorCount;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetBISTAVLinkErrorCount()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U16 _mhal_mhl_GetBISTAVLinkErrorCount(MS_U8 ucCbusSelect)
{
    MS_U16 ucAVLinkErrorCount = 0;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            ucAVLinkErrorCount = R2BYTE(REG_DVI_DTOP_DUAL_P0_73_L);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            ucAVLinkErrorCount = R2BYTE(REG_DVI_DTOP_DUAL_P1_73_L);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            ucAVLinkErrorCount = R2BYTE(REG_DVI_DTOP_DUAL_P2_73_L);

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            ucAVLinkErrorCount = R2BYTE(REG_DVI_DTOP_DUAL_P3_73_L);

            break;
#endif

        default:

            break;
    };

    return ucAVLinkErrorCount;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_CheckDEChangeFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mhal_mhl_CheckDEChangeFlag(void)
{
    MS_BOOL bDEChangeFlag = ((R2BYTE(REG_HDMI_DUAL_0_02_L) & BIT(5)) ? TRUE : FALSE);
    static MS_U8 ucStableCount = 0;

    if(bDEChangeFlag)
    {
        W2BYTE(REG_HDMI_DUAL_0_02_L, BIT(5));

        ucStableCount = 0;
    }
    else if(ucStableCount <= 10)
    {
        ucStableCount++;
    }

    if(ucStableCount == 10)
    {
        bDEChangeFlag = TRUE;
    }
    else
    {
        bDEChangeFlag = FALSE;
    }

    return bDEChangeFlag;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_MHL30AutoEQSetting()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_MHL30AutoEQSetting(MS_U8 ucCbusSelect)
{
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_COMBO_PHY1_P0_45_L, MHL_COARSE_TUNE_30_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P0_29_L, BIT(8), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_11_L, BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P0_42_L, MHL_COARSE_TUNE_30_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_4A_L, MHL_COARSE_TUNE_30_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_4B_L, MHL_FINE_TUNE_AABA_30_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_4C_L, MHL_FINE_TUNE_UNDER_30_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P0_26_L, BIT(11)| BIT(9), BMASK(13:8));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_COMBO_PHY1_P1_45_L, MHL_COARSE_TUNE_30_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P1_29_L, BIT(8), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_11_L, BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P1_42_L, MHL_COARSE_TUNE_30_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_4A_L, MHL_COARSE_TUNE_30_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_4B_L, MHL_FINE_TUNE_AABA_30_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_4C_L, MHL_FINE_TUNE_UNDER_30_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P1_26_L, BIT(11)| BIT(9), BMASK(13:8));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_COMBO_PHY1_P2_45_L, MHL_COARSE_TUNE_30_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P2_29_L, BIT(8), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_11_L, BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P2_42_L, MHL_COARSE_TUNE_30_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_4A_L, MHL_COARSE_TUNE_30_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_4B_L, MHL_FINE_TUNE_AABA_30_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_4C_L, MHL_FINE_TUNE_UNDER_30_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P2_26_L, BIT(11)| BIT(9), BMASK(13:8));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_COMBO_PHY1_P3_45_L, MHL_COARSE_TUNE_30_MIN, BMASK(4:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_49_L, BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY0_P3_29_L, BIT(8), BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_11_L, BIT(4), BIT(4));
            W2BYTEMSK(REG_COMBO_PHY1_P3_42_L, MHL_COARSE_TUNE_30_DETECT_TIME, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_4A_L, MHL_COARSE_TUNE_30_AABA_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_4B_L, MHL_FINE_TUNE_AABA_30_NUMBER, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_4C_L, MHL_FINE_TUNE_UNDER_30_THRESHOLD, BMASK(15:0));
            W2BYTEMSK(REG_COMBO_PHY0_P3_26_L, BIT(11)| BIT(9), BMASK(13:8));

            break;
#endif

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_MHL3AutoEQTrigger()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_MHL3AutoEQTrigger(MS_U8 ucCbusSelect)
{
    MS_U16 ustemp = 1000;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_COMBO_PHY0_P0_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P0_33_L, 0, BIT(2));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_COMBO_PHY0_P1_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P1_33_L, 0, BIT(2));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_COMBO_PHY0_P2_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P2_33_L, 0, BIT(2));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_COMBO_PHY0_P3_33_L, BIT(2), BIT(2));

            while(ustemp--);

            W2BYTEMSK(REG_COMBO_PHY0_P3_33_L, 0, BIT(2));

            break;
#endif

        default:
            ustemp = 0;
            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_GetAutoEQDoneFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL _mhal_mhl_GetAutoEQDoneFlag(MS_U8 ucCbusSelect)
{
    MS_BOOL bAutoEQDone = FALSE;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTEMSK(REG_COMBO_PHY0_P0_41_L, 0, BMASK(15:14));

            if(R2BYTE(REG_COMBO_PHY0_P0_41_L) &BIT(8))
            {
                bAutoEQDone = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_COMBO_PHY0_P1_41_L, 0, BMASK(15:14));

            if(R2BYTE(REG_COMBO_PHY0_P1_41_L) &BIT(8))
            {
                bAutoEQDone = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_COMBO_PHY0_P2_41_L, 0, BMASK(15:14));

            if(R2BYTE(REG_COMBO_PHY0_P2_41_L) &BIT(8))
            {
                bAutoEQDone = TRUE;
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_COMBO_PHY0_P3_41_L, 0, BMASK(15:14));

            if(R2BYTE(REG_COMBO_PHY0_P3_41_L) &BIT(8))
            {
                bAutoEQDone = TRUE;
            }

            break;
#endif

        default:

            break;
    };

    return bAutoEQDone;
}

//**************************************************************************
//  [Function Name]:
//                  _mhal_mhl_MHL30AutoEQEnable()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void _mhal_mhl_MHL30AutoEQEnable(MS_U8 ucCbusSelect, MS_BOOL bAutoEQEnable)
{
    MS_BOOL bAutoEQTrigger = FALSE;

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
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
                W2BYTEMSK(REG_COMBO_PHY0_P0_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            }

            W2BYTEMSK(REG_COMBO_PHY0_P0_15_L, bAutoEQEnable? BIT(15): 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P0_44_L, bAutoEQEnable? 0: BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P0_49_L, bAutoEQEnable? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY1_P0_11_L, bAutoEQEnable? 0: BIT(4), BIT(4));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
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
                W2BYTEMSK(REG_COMBO_PHY0_P1_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            }

            W2BYTEMSK(REG_COMBO_PHY0_P1_15_L, bAutoEQEnable? BIT(15): 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P1_44_L, bAutoEQEnable? 0: BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P1_49_L, bAutoEQEnable? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY1_P1_11_L, bAutoEQEnable? 0: BIT(4), BIT(4));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
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
                W2BYTEMSK(REG_COMBO_PHY0_P2_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            }

            W2BYTEMSK(REG_COMBO_PHY0_P2_15_L, bAutoEQEnable? BIT(15): 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P2_44_L, bAutoEQEnable? 0: BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P2_49_L, bAutoEQEnable? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY1_P2_11_L, bAutoEQEnable? 0: BIT(4), BIT(4));

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
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
                W2BYTEMSK(REG_COMBO_PHY0_P3_73_L, MHL_EQ_20_SETTING_VALUE, BMASK(4:0)); // B channel EQ
            }

            W2BYTEMSK(REG_COMBO_PHY0_P3_15_L, bAutoEQEnable? BIT(15): 0, BIT(15));
            W2BYTEMSK(REG_COMBO_PHY1_P3_44_L, bAutoEQEnable? 0: BIT(0), BIT(0));
            W2BYTEMSK(REG_COMBO_PHY1_P3_49_L, bAutoEQEnable? 0: BIT(5), BIT(5));
            W2BYTEMSK(REG_COMBO_PHY1_P3_11_L, bAutoEQEnable? 0: BIT(4), BIT(4));

            break;
#endif

        default:

            break;
    };

    if(bAutoEQTrigger)
    {
        _mhal_mhl_GetAutoEQDoneFlag(ucCbusSelect);
        _mhal_mhl_MHL3AutoEQTrigger(ucCbusSelect);
    }
}

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetRegisterBaseAddress()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetRegisterBaseAddress(MS_VIRT ulRiuBassAddress, MS_VIRT ulPMRiuBassAddress)
{
    MHL_XC_RIU_BASE = ulPMRiuBassAddress;
    MHL_PM_RIU_BASE = ulPMRiuBassAddress;

    ulRiuBassAddress = ulPMRiuBassAddress;
}

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
    return (R2BYTE(REG_HDMI2_DUAL_0_50_L) &BMASK(1:0));
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

    if((ucScalerMainMux == MHL_SCALER_MUX_SELECT_DVI) || (ucScalerSubMux == MHL_SCALER_MUX_SELECT_DVI))
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
    if(ucSelect > 0)
    {
        W2BYTEMSK(REG_PM_SLEEP_72_L, BMASK(7:6), BMASK(8:6)); // [8]: reg_cbus_debug_sel, [7]: reg_vbus_en_sel , [6]: reg_mhl_cable_detect_sel
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

    MHAL_MHL_MSG_INFO("** MHL Cbus index value %x\r\n", ucChipID);
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
            bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            bFlag = ((R2BYTE(REG_PM_MHL_CBUS_00) & BIT(10)) ? TRUE : FALSE);
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

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:

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
MS_BOOL mhal_mhl_CDRModeMonitor(MS_U8 ucCbusSelect, MS_BOOL bPathEnable, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    MS_BOOL bindex = FALSE;
    
    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
            {
                if(pMHLSignalStatus->ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                {
                    pMHLSignalStatus->ucStableCount++;
                }

                if(!pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L, 0, BIT(8));

                    pMHLSignalStatus->bClockStableFlag = TRUE;

                    if(pMHLSignalStatus->bECbusEnableFlag)
                    {
                        pMHLSignalStatus->bAutoEnableFlag = TRUE;
                        pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_START;
                    }
                }

                if((R2BYTE(REG_DVI_DTOP_DUAL_P0_31_L) &BIT(6)) == BIT(6)) // DE stable
                {
                    if(pMHLSignalStatus->ucStableCount < MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = MHL_CDR_STABLE_THRESHOLD;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_FORCE_THRESHOLD -1))
                    {
                        if(pMHLSignalStatus->bECbusEnableFlag)
                        {
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_72_L, BIT(13), BIT(13));
                            //MsOS_DelayTaskUs(1);
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_72_L, 0, BIT(13));

                            MHAL_MHL_MSG_INFO("** MHL toggle reset patch ~~~\r\n");
                        }

                        pMHLSignalStatus->bSignalStableFlag = TRUE;
                    }

                    if(pMHLSignalStatus->bBISTEnableFlag)
                    {
                        if(_mhal_mhl_CheckDEChangeFlag())
                        {
                            MHAL_MHL_MSG_INFO("** MHL trigger AV link BIST yo~~\r\n");

                            _mhal_mhl_BISTAVLinkEnable(ucCbusSelect, TRUE);
                        }
                    }
                }
                else // DE unstable
                {
                    if(pMHLSignalStatus->ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = 0;
                        pMHLSignalStatus->bSignalStableFlag = FALSE;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_STABLE_THRESHOLD -1))
                    {
                        MHAL_MHL_MSG_INFO("** MHL time change toggle CDR mode\r\n");

                        pMHLSignalStatus->ucStableCount = 0;
                    }
                }
            }
            else
            {
                if(pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P0_6E_L, BIT(8), BIT(8));

                    pMHLSignalStatus->bClockStableFlag = FALSE;
                    pMHLSignalStatus->ucStableCount = 0;
                    pMHLSignalStatus->bAutoEnableFlag = FALSE;
                    pMHLSignalStatus->bSignalStableFlag = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
            {
                if(pMHLSignalStatus->ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                {
                    pMHLSignalStatus->ucStableCount++;
                }

                if(!pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P1_6E_L, 0, BIT(8));

                    pMHLSignalStatus->bClockStableFlag = TRUE;

                    if(pMHLSignalStatus->bECbusEnableFlag)
                    {
                        pMHLSignalStatus->bAutoEnableFlag = TRUE;
                        pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_START;
                    }
                }

                if((R2BYTE(REG_DVI_DTOP_DUAL_P1_31_L) &BIT(6)) == BIT(6)) // DE stable
                {
                    if(pMHLSignalStatus->ucStableCount < MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = MHL_CDR_STABLE_THRESHOLD;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_FORCE_THRESHOLD -1))
                    {
                        if(pMHLSignalStatus->bECbusEnableFlag)
                        {
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_72_L, BIT(13), BIT(13));
                            //MsOS_DelayTaskUs(1);
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_72_L, 0, BIT(13));

                            MHAL_MHL_MSG_INFO("** MHL toggle reset patch ~~~\r\n");
                        }

                        pMHLSignalStatus->bSignalStableFlag = TRUE;
                    }

                    if(pMHLSignalStatus->bBISTEnableFlag)
                    {
                        if(_mhal_mhl_CheckDEChangeFlag())
                        {
                            MHAL_MHL_MSG_INFO("** MHL trigger AV link BIST yo~~\r\n");

                            _mhal_mhl_BISTAVLinkEnable(ucCbusSelect, TRUE);
                        }
                    }
                }
                else // DE unstable
                {
                    if(pMHLSignalStatus->ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = 0;
                        pMHLSignalStatus->bSignalStableFlag = FALSE;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_STABLE_THRESHOLD -1))
                    {
                        MHAL_MHL_MSG_INFO("** MHL time change toggle CDR mode\r\n");

                        pMHLSignalStatus->ucStableCount = 0;
                    }
                }
            }
            else
            {
                if(pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P1_6E_L, BIT(8), BIT(8));

                    pMHLSignalStatus->bClockStableFlag = FALSE;
                    pMHLSignalStatus->ucStableCount = 0;
                    pMHLSignalStatus->bAutoEnableFlag = FALSE;
                    pMHLSignalStatus->bSignalStableFlag = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
            {
                if(pMHLSignalStatus->ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                {
                    pMHLSignalStatus->ucStableCount++;
                }

                if(!pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P2_6E_L, 0, BIT(8));

                    pMHLSignalStatus->bClockStableFlag = TRUE;

                    if(pMHLSignalStatus->bECbusEnableFlag)
                    {
                        pMHLSignalStatus->bAutoEnableFlag = TRUE;
                        pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_START;
                    }
                }

                if((R2BYTE(REG_DVI_DTOP_DUAL_P2_31_L) &BIT(6)) == BIT(6)) // DE stable
                {
                    if(pMHLSignalStatus->ucStableCount < MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = MHL_CDR_STABLE_THRESHOLD;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_FORCE_THRESHOLD -1))
                    {
                        if(pMHLSignalStatus->bECbusEnableFlag)
                        {
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_72_L, BIT(13), BIT(13));
                            //MsOS_DelayTaskUs(1);
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_72_L, 0, BIT(13));

                            MHAL_MHL_MSG_INFO("** MHL toggle reset patch ~~~\r\n");
                        }

                        pMHLSignalStatus->bSignalStableFlag = TRUE;
                    }

                    if(pMHLSignalStatus->bBISTEnableFlag)
                    {
                        if(_mhal_mhl_CheckDEChangeFlag())
                        {
                            MHAL_MHL_MSG_INFO("** MHL trigger AV link BIST yo~~\r\n");

                            _mhal_mhl_BISTAVLinkEnable(ucCbusSelect, TRUE);
                        }
                    }
                }
                else // DE unstable
                {
                    if(pMHLSignalStatus->ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = 0;
                        pMHLSignalStatus->bSignalStableFlag = FALSE;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_STABLE_THRESHOLD -1))
                    {
                        MHAL_MHL_MSG_INFO("** MHL time change toggle CDR mode\r\n");

                        pMHLSignalStatus->ucStableCount = 0;
                    }
                }
            }
            else
            {
                if(pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P2_6E_L, BIT(8), BIT(8));

                    pMHLSignalStatus->bClockStableFlag = FALSE;
                    pMHLSignalStatus->ucStableCount = 0;
                    pMHLSignalStatus->bAutoEnableFlag = FALSE;
                    pMHLSignalStatus->bSignalStableFlag = FALSE;
                }
            }
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(bPathEnable && (_mhal_mhl_CheckClockStatus(ucCbusSelect)))
            {
                if(pMHLSignalStatus->ucStableCount < (MHL_CDR_FORCE_THRESHOLD +1))
                {
                    pMHLSignalStatus->ucStableCount++;
                }

                if(!pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P3_6E_L, 0, BIT(8));

                    pMHLSignalStatus->bClockStableFlag = TRUE;

                    if(pMHLSignalStatus->bECbusEnableFlag)
                    {
                        pMHLSignalStatus->bAutoEnableFlag = TRUE;
                        pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_START;
                    }
                }

                if((R2BYTE(REG_DVI_DTOP_DUAL_P3_31_L) &BIT(6)) == BIT(6)) // DE stable
                {
                    if(pMHLSignalStatus->ucStableCount < MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = MHL_CDR_STABLE_THRESHOLD;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_FORCE_THRESHOLD -1))
                    {
                        if(pMHLSignalStatus->bECbusEnableFlag)
                        {
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_72_L, BIT(13), BIT(13));
                            //MsOS_DelayTaskUs(1);
                            W2BYTEMSK(REG_DVI_DTOP_DUAL_P3_72_L, 0, BIT(13));

                            MHAL_MHL_MSG_INFO("** MHL toggle reset patch ~~~\r\n");
                        }

                        pMHLSignalStatus->bSignalStableFlag = TRUE;
                    }

                    if(pMHLSignalStatus->bBISTEnableFlag)
                    {
                        if(_mhal_mhl_CheckDEChangeFlag())
                        {
                            MHAL_MHL_MSG_INFO("** MHL trigger AV link BIST yo~~\r\n");

                            _mhal_mhl_BISTAVLinkEnable(ucCbusSelect, TRUE);
                        }
                    }
                }
                else // DE unstable
                {
                    if(pMHLSignalStatus->ucStableCount >= MHL_CDR_STABLE_THRESHOLD)
                    {
                        pMHLSignalStatus->ucStableCount = 0;
                        pMHLSignalStatus->bSignalStableFlag = FALSE;
                    }
                    else if(pMHLSignalStatus->ucStableCount == (MHL_CDR_STABLE_THRESHOLD -1))
                    {
                        MHAL_MHL_MSG_INFO("** MHL time change toggle CDR mode\r\n");

                        pMHLSignalStatus->ucStableCount = 0;
                    }
                }
            }
            else
            {
                if(pMHLSignalStatus->bClockStableFlag)
                {
                    W2BYTEMSK(REG_COMBO_PHY0_P3_6E_L, BIT(8), BIT(8));

                    pMHLSignalStatus->bClockStableFlag = FALSE;
                    pMHLSignalStatus->ucStableCount = 0;
                    pMHLSignalStatus->bAutoEnableFlag = FALSE;
                    pMHLSignalStatus->bSignalStableFlag = FALSE;
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
            if(bFlag)
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
            }
            else
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(bFlag)
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
            }
            else
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(bFlag)
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
            }
            else
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(bFlag)
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, BIT(9), BIT(9) | BIT(8));
            }
            else
            {
                W2BYTEMSK(REG_PM_MHL_CBUS_00, 0, BIT(9) | BIT(8));
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

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
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

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
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

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
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
        MHAL_MHL_MSG_INFO("** Maxim Load MHL EDID...\r\n");

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
        MHAL_MHL_MSG_INFO("** Maxim Read MHL EDID...\r\n");

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
        MHAL_MHL_MSG_INFO("** Maxim Load DevCap...\r\n");

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

        for(uctemp = 0; uctemp <2; uctemp++)
        {
            W2BYTE(REG_MHL_CBUS_19 +(uctemp *2), (devcap[(uctemp *2) +17] <<8) | devcap[(uctemp *2) +16]); // [15:0], MHL_ECBUS_SPEEDS
        }
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
MS_U8 mhal_mhl_initial(MS_U8 ucCbusSelect, MS_U8 *edid, MS_U8 *devcap, MS_U8 ucVenderID)
{
    MS_U16 uctemp = 0;

    // Initial setting
    for(uctemp = 0; uctemp <(sizeof(tMHL_INITIAL_TABLE) /sizeof(msLoadTbl_S)); uctemp++)
    {
        W2BYTEMSK(tMHL_INITIAL_TABLE[uctemp].addr, tMHL_INITIAL_TABLE[uctemp].databuf, tMHL_INITIAL_TABLE[uctemp].mask);
    }

    // ECbus initial setting
    _mhal_mhl_ECbusInitialSetting();
    
    _mhal_mhl_CbusAndClockSelect(ucCbusSelect);

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
    }

    _mhal_mhl_ForcePullDown100K(TRUE);
    
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
            W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTEMSK(REG_PM_MHL_CBUS_00, bCableDetectInvert? BIT(11): 0, BIT(11));
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
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
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
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
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
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
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
            
            break;
#endif

        default:
            ucOutputState = 0;
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
    // Nothing
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
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
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
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
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
            
            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
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
//                  mhal_mhl_GetECbusStateChangeFlag()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_BOOL mhal_mhl_GetECbusStateChangeFlag(stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus, stMHL_BIST_MODE_INFO *pBISTParameterInfo)
{
    MS_BOOL bindex = ((R2BYTE(REG_MHL_ECBUS_3A) &BIT(0)) ?TRUE: FALSE);

    if(bindex)
    {
        W2BYTEMSK(REG_MHL_ECBUS_34, BIT(0), BIT(0));

        if((pMHLSignalStatus != NULL) && (pBISTParameterInfo != NULL))
        {
            _mhal_mhl_ECbusStateChangeProc(pMHLSignalStatus, pBISTParameterInfo);
        }
    }

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCReceiveFlag()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_BOOL mhal_mhl_GetEMSCReceiveFlag(void)
{
    MS_BOOL bindex = ((R2BYTE(REG_MHL_ECBUS_3A) &BIT(12)) ?TRUE: FALSE);

    if(bindex)
    {
        W2BYTEMSK(REG_MHL_ECBUS_34, BIT(12), BIT(12));
    }

    return bindex;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCSendStatus()
//  [Description]
//                  
//  [Arguments]:
//
//  [Return]:
//                  
//**************************************************************************
MS_U8 mhal_mhl_GetEMSCSendStatus(void)
{
    MS_U8 ucSendStatus = MHL_EMSC_SEND_NONE;
    
    if(R2BYTE(REG_MHL_ECBUS_3A) &BIT(10)) // eMSC send pass interrupt.
    {
        W2BYTEMSK(REG_MHL_ECBUS_34, BIT(10), BIT(10)); // eMSC send pass clear
        
        ucSendStatus = MHL_EMSC_SEND_PASS;
    }
    else if(R2BYTE(REG_MHL_ECBUS_3A) &BIT(9)) // eMSC send fail interrupt.
    {
        W2BYTEMSK(REG_MHL_ECBUS_34, BIT(9), BIT(9)); // eMSC send fail clear

        ucSendStatus = MHL_EMSC_SEND_FAIL;
    }

    return ucSendStatus;
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
MS_BOOL mhal_mhl_CBusWrite(MS_BOOL bECbusEnableFlag, mhalCbusFifo_S *pdatabuf)
{
    MS_BOOL bSendDondFlag = FALSE;
    MS_U8 uctemp = 0;

    if(!_mhal_mhl_IsCbusBusy(bECbusEnableFlag))
    {
        for(uctemp = 0; uctemp < pdatabuf->lens; uctemp++)
        {
            W2BYTE(REG_MHL_CBUS_26 +uctemp *2, pdatabuf->databuf[uctemp]);
        }

        // clear the unsed parts
        W2BYTE(REG_MHL_CBUS_26 +((pdatabuf->lens) *2), 0);

        W2BYTEMSK(REG_MHL_CBUS_25, BIT(12), BIT(12)); // trigger to send

        bSendDondFlag = TRUE;

#if(MHL_DEBUG_SEND_MSC)
        for(uctemp = 0; uctemp < pdatabuf->lens; uctemp++)
        {
            if((pdatabuf->databuf[uctemp]) &BIT(8))
            {
                MHAL_MHL_MSG_DEBUG(" MHL send MSG command = %x\r\n", (pdatabuf->databuf[uctemp] &BMASK(7:0)));
            }
            else
            {
                MHAL_MHL_MSG_DEBUG(" MHL send MSG data = %x\r\n", (pdatabuf->databuf[uctemp] &BMASK(7:0)));
            }
        }

#endif
    }

    return bSendDondFlag;
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
void mhal_mhl_CablePlugProc(MS_U8 ucCbusSelect, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    _mhal_mhl_RxRtermControl(ucCbusSelect, RX_RTERM_OFF);

#if(DMHL_LG_PRADA_PATCH)
    _mhal_mhl_AdjustCommonModeResistor(ucCbusSelect, TRUE);
#endif

    _mhal_mhl_Mhl24bitsModeSetting(ucCbusSelect, pMHLSignalStatus);

    if(mhal_mhl_CheckInputPort(ucCbusSelect))
    {
        _mhal_mhl_AudioPathSelect(TRUE);
    }

    _mhal_mhl_ForcePullDown100K(FALSE);
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
void mhal_mhl_CableRemoveProc(MS_U8 ucCbusSelect, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    _mhal_mhl_RxRtermControl(ucCbusSelect, RX_HDMI_RTERM);

    _mhal_mhl_HdmiBypassModeSetting(ucCbusSelect, pMHLSignalStatus);

#if(DMHL_LG_PRADA_PATCH)
    _mhal_mhl_AdjustCommonModeResistor(ucCbusSelect, TRUE);
#endif

    if(mhal_mhl_CheckInputPort(ucCbusSelect))
    {
        _mhal_mhl_AudioPathSelect(FALSE);
    }

    _mhal_mhl_ForcePullDown100K(TRUE);
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

    // Set short read address to 0x08
    _mhal_mhl_SetShortReadAddress(FALSE);
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
void mhal_mhl_SourceChangeProc(MS_U8 ucCbusSelect, MS_U8 ucLinkRate)
{
    MS_BOOL bLinkRate6GFlag = FALSE;

    if(ucLinkRate == MHL_AV_LINK_600)
    {
        bLinkRate6GFlag = TRUE;
    }

    if(mhal_mhl_CheckInputPort(ucCbusSelect))
    {
        _mhal_mhl_AudioPathSelect(TRUE);
    }
    else
    {
        _mhal_mhl_AudioPathSelect(FALSE);
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
void mhal_mhl_ClockModeSwitchProc(MS_U8 ucCbusSelect, MS_BOOL bPPmode, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    if(bPPmode)
    {
        _mhal_mhl_MhlPackedPixelModeSetting(ucCbusSelect);
    }
    else
    {
        _mhal_mhl_Mhl24bitsModeSetting(ucCbusSelect, pMHLSignalStatus);
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
    if(bflag) // HPD is high
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_20, BIT(3), BMASK(4:3)); // [4]: output val, [3]: oen
    }
    else // hpd is low
    {
        W2BYTEMSK(REG_PM_MHL_CBUS_20, 0, BMASK(4:3)); // [4]: output val, [3]: oen
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

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetMainLinkRate()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetMainLinkRate(MS_U8 ucCbusSelect, MS_U8 ucLinkRate)
{
    MS_BOOL bLinkRate6GFlag = FALSE;

    _mhal_mhl_Version3PhyEnable(ucCbusSelect, TRUE);

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            if(ucLinkRate == MHL_AV_LINK_600)
            {
                bLinkRate6GFlag = TRUE;
            }
            else // MHL_AV_LINK_150 or MHL_AV_LINK_300
            {

            }

            W2BYTEMSK(REG_COMBO_PHY1_P0_0D_L, (ucLinkRate << 12), BMASK(13:12)); // [13:12]: Select MHL3 data rate

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            if(ucLinkRate == MHL_AV_LINK_600)
            {
                bLinkRate6GFlag = TRUE;
            }
            else // MHL_AV_LINK_150 or MHL_AV_LINK_300
            {

            }

            W2BYTEMSK(REG_COMBO_PHY1_P1_0D_L, (ucLinkRate << 12), BMASK(13:12)); // [13:12]: Select MHL3 data rate

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            if(ucLinkRate == MHL_AV_LINK_600)
            {
                bLinkRate6GFlag = TRUE;
            }
            else // MHL_AV_LINK_150 or MHL_AV_LINK_300
            {

            }

            W2BYTEMSK(REG_COMBO_PHY1_P2_0D_L, (ucLinkRate << 12), BMASK(13:12)); // [13:12]: Select MHL3 data rate

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            if(ucLinkRate == MHL_AV_LINK_600)
            {
                bLinkRate6GFlag = TRUE;
            }
            else // MHL_AV_LINK_150 or MHL_AV_LINK_300
            {

            }

            W2BYTEMSK(REG_COMBO_PHY1_P3_0D_L, (ucLinkRate << 12), BMASK(13:12)); // [13:12]: Select MHL3 data rate

            break;
#endif

        default:
            bLinkRate6GFlag = FALSE;
            break;
    };

    _mhal_mhl_MHL30AutoEQSetting(ucCbusSelect);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetECbusStatusFlag()
//  [Description]:
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
MS_U8 mhal_mhl_GetECbusStatusFlag(void)
{
    MS_U8 ucTrainState = (R2BYTE(REG_MHL_ECBUS_PHY_69) >> 12);
    MS_U8 ucECbusStatusFlag = 0;

    if(R2BYTE(REG_MHL_ECBUS_PHY_6F) &BIT(13)) // [13]: ECbus clock lock flag
    {
        ucECbusStatusFlag |= MHL_ECBUS_STATUS_CLOCK_LOCK;
    }

    if((ucTrainState >= MHL_ECBUS_TRAIN_WAIT_RRCEIVE_COMMA2) && (ucTrainState <= MHL_ECBUS_TRAIN_DONE))//Whan state of REG_PM_MHL_CBUS2_56 is equla 3, than state of REG_PM_MHL_CBUS2_69[51:12] will from 1 to 7. 
    {
        //W2BYTEMSK(REG_MHL_ECBUS_PHY_56, 0x0200, BMASK(15:0)); // for auto train
#if(MHL_ECBUS_AUTO_TRAINING)
        W2BYTEMSK(REG_MHL_ECBUS_PHY_4A, 0, BIT(13));
#endif
    }

    if(R2BYTE(REG_MHL_ECBUS_3A) &BIT(7)) // REG_MHL_CBUS2_3A[7]
    {
        ucECbusStatusFlag |= MHL_ECBUS_STATUS_SLOT_SYNC_DONE;
    }

    if(R2BYTE(REG_MHL_ECBUS_3A) &BIT(15)) // REG_MHL_CBUS2_3A[15]
    {
        ucECbusStatusFlag |= MHL_ECBUS_STATUS_EMSC_SYNC_DONE;
    }

    return ucECbusStatusFlag;
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_ECbusEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_ECbusEventProc(MS_U8 ucCbusSelect, MS_U8 ucECbusEvent, MS_BOOL bBISTEnableFlag, stMHL_BIST_MODE_INFO *pBISTParameterInfo)
{
    switch(ucECbusEvent)
    {
        case MHL_ECBUS_EVENT_PREPARE_MODE_UP:
            // Disable stuck to low interrupt
            mhal_mhl_CbusStucktoLow(FALSE);
            // Disable connect detect
            _mhal_mhl_CbusConnectCheckEnable(FALSE);

            break;

        case MHL_ECBUS_EVENT_MODE_UP_PROCESS:
            // Enable ECbus
            _mhal_mhl_ECbusEnableSetting(TRUE);

            if(bBISTEnableFlag)
            {
                //_mhal_mhl_ECbusBISTSetting(TRUE, pBISTParameterInfo);
                _mhal_mhl_BISTECbusEnable(TRUE);
                //_mhal_mhl_ECbusStateOverwrite(TRUE);
            }

            // Set short read address to 0x70
            _mhal_mhl_SetShortReadAddress(TRUE);
            // Enable ECbus state change IRQ
            _mhal_mhl_SetECbusStateChangeInterrupt(TRUE);
            // Enable eMSC receive IRQ
            _mhal_mhl_SetEMSCReceiveInterrupt(TRUE);

            _mhal_mhl_ECbusModeUpProc();

            mhal_mhl_SetMainLinkRate(ucCbusSelect, MHL_AV_LINK_150);

            break;

        case MHL_ECBUS_EVENT_PREPARE_MODE_DOWN:

            break;
            
        case MHL_ECBUS_EVENT_MODE_DOWN_PROCESS:
        case MHL_ECBUS_EVENT_STUCK_TO_LOW:
            // Enable connect detect
            _mhal_mhl_CbusConnectCheckEnable(TRUE);
            // Disable ECbus
            _mhal_mhl_ECbusEnableSetting(FALSE);

            if(bBISTEnableFlag)
            {
                //_mhal_mhl_ECbusBISTSetting(FALSE, pBISTParameterInfo);
            }

            // Clear short read address to 0x08
            _mhal_mhl_SetShortReadAddress(FALSE);
            // Disable ECbus state change IRQ
            _mhal_mhl_SetECbusStateChangeInterrupt(FALSE);
            // Disable eMSC receive IRQ
            _mhal_mhl_SetEMSCReceiveInterrupt(FALSE);
            // Disable MHL3 phy
            _mhal_mhl_Version3PhyEnable(ucCbusSelect, FALSE);
            // Reset all status
            W2BYTE(REG_MHL_ECBUS_34, 0xFFFF);

            _mhal_mhl_CbusEngineReset();

            break;

        case MHL_ECBUS_EVENT_UNLOCK_RESET:
            //_mhal_mhl_ECbusDmuxEnable(FALSE);
            _mhal_mhl_ECbusStateReset();

            break;

#if(MHL_ECBUS_COMMAND_PARSING)
        case MHL_ECBUS_EVENT_ECBUS_COMMAND_PARSING:
            _mhal_mhl_GetECbusCommand();

            break;

#endif

        case MHL_ECBUS_EVENT_BIST_TRIGGER:
            //_mhal_mhl_BISTECbusEnable(TRUE);
            //_mhal_mhl_SetECbusBISTTrigger(TRUE, pBISTParameterInfo->bBISTeCbusTxFlag, pBISTParameterInfo->bBISTeCbusRxFlag);

            break;

        default:

            break;
    };
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_SetBISTParameterInfo()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_SetBISTParameterInfo(MS_U8 ucCbusSelect, stMHL_BIST_MODE_INFO *pBISTParameterInfo)
{
    MS_U8 ucBISTeCbusPattern = pBISTParameterInfo->ucBISTeCbusPattern;
    MS_U8 ucBISTAVLinkRate = pBISTParameterInfo->ucBISTAVLinkRate;
    MS_U16 usBISTeCbusSettingValue = 0;
    MS_U16 usBISTAVLinkSettingValue = (pBISTParameterInfo->ucBISTAVLinkPattern << 8)| 0xFF;

    if(ucBISTeCbusPattern > 0)
    {
        ucBISTeCbusPattern = ucBISTeCbusPattern -1;
    }

    usBISTeCbusSettingValue = (ucBISTeCbusPattern << 12)| pBISTParameterInfo->usBISTeCbusPatternFixed;

    W2BYTEMSK(REG_MHL_ECBUS_2B, usBISTeCbusSettingValue, BMASK(13:12)| BMASK(7:0)); // ecbus bist send pattern type, ecbus bist send fix pattern value
    W2BYTEMSK(REG_MHL_ECBUS_2F, usBISTeCbusSettingValue, BMASK(13:12)| BMASK(7:0)); // ecbus bist receive pattern type, ecbus bist receive fix pattern value

    if(pBISTParameterInfo->ucBISTAVLinkRateRandomizer > 0)
    {
        usBISTAVLinkSettingValue = usBISTAVLinkSettingValue| BIT(15);
    }

    switch(ucCbusSelect)
    {
#if(MHL_FUNCTION_SUPPORT_PORTA)
        case MHL_CBUS_SELECT_PORTA:
            W2BYTE(REG_DVI_DTOP_DUAL_P0_74_L, pBISTParameterInfo->usBISTAVLinkFixed);
            W2BYTE(REG_DVI_DTOP_DUAL_P0_75_L, usBISTAVLinkSettingValue);

            if(pBISTParameterInfo->ucBISTAVLinkPattern == MHL_BIST_LINK_PATTERN_FIXED10)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_71_L, 0, BIT(0));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_71_L, BIT(0), BIT(0));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTB)
        case MHL_CBUS_SELECT_PORTB:
            W2BYTE(REG_DVI_DTOP_DUAL_P1_74_L, pBISTParameterInfo->usBISTAVLinkFixed);
            W2BYTE(REG_DVI_DTOP_DUAL_P1_75_L, usBISTAVLinkSettingValue);

            if(pBISTParameterInfo->ucBISTAVLinkPattern == MHL_BIST_LINK_PATTERN_FIXED10)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_71_L, 0, BIT(0));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P1_71_L, BIT(0), BIT(0));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTC)
        case MHL_CBUS_SELECT_PORTC:
            W2BYTE(REG_DVI_DTOP_DUAL_P2_74_L, pBISTParameterInfo->usBISTAVLinkFixed);
            W2BYTE(REG_DVI_DTOP_DUAL_P2_75_L, usBISTAVLinkSettingValue);

            if(pBISTParameterInfo->ucBISTAVLinkPattern == MHL_BIST_LINK_PATTERN_FIXED10)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_71_L, 0, BIT(0));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P2_71_L, BIT(0), BIT(0));
            }

            break;
#endif

#if(MHL_FUNCTION_SUPPORT_PORTD)
        case MHL_CBUS_SELECT_PORTD:
            W2BYTE(REG_DVI_DTOP_DUAL_P3_74_L, pBISTParameterInfo->usBISTAVLinkFixed);
            W2BYTE(REG_DVI_DTOP_DUAL_P3_75_L, usBISTAVLinkSettingValue);

            if(pBISTParameterInfo->ucBISTAVLinkPattern == MHL_BIST_LINK_PATTERN_FIXED10)
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_73_L, 0, BIT(0));
            }
            else
            {
                W2BYTEMSK(REG_DVI_DTOP_DUAL_P0_73_L, BIT(0), BIT(0));
            }

            break;
#endif

        default:

            break;
    };

    if(ucBISTAVLinkRate > 0)
    {
        ucBISTAVLinkRate = ucBISTAVLinkRate -1;
    }

    mhal_mhl_SetMainLinkRate(ucCbusSelect, ucBISTAVLinkRate);
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetBISTErrorCount()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_GetBISTErrorCount(MS_U8 ucCbusSelect, stMHL_BIST_MODE_INFO *pBISTParameterInfo)
{
    if(pBISTParameterInfo != NULL)
    {
        pBISTParameterInfo->usBISTeCbusErrorCount = _mhal_mhl_GetBISTECbusErrorCount();
        pBISTParameterInfo->usBISTAVLinkErrorCount = _mhal_mhl_GetBISTAVLinkErrorCount(ucCbusSelect);

        _mhal_mhl_SetECbusBISTTrigger(FALSE, pBISTParameterInfo->bBISTeCbusTxFlag, pBISTParameterInfo->bBISTeCbusRxFlag);
        //_mhal_mhl_ECbusStateOverwrite(FALSE);
        _mhal_mhl_BISTECbusEnable(FALSE);
        _mhal_mhl_BISTAVLinkEnable(ucCbusSelect, FALSE);
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_GetEMSCReceiveData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_GetEMSCReceiveData(MS_U8 *bReveiceEMSC)
{
    MS_U8 uctemp = 0;
    MS_U16 usEMSCFreeBuffer = 0;

    usEMSCFreeBuffer = (R2BYTE(REG_MHL_ECBUS_24) &BMASK(9:0)); // REG_MHL_ECBUS_24[9:0]

    usEMSCFreeBuffer = MHL_EMSC_DATA_SIZE - usEMSCFreeBuffer;

    bReveiceEMSC[0] = usEMSCFreeBuffer;

#if(MHL_DEBUG_RECEIVE_EMSC)
    MHAL_MHL_MSG_INFO("** MHL eMSC length %d\r\n", usEMSCFreeBuffer);
#endif

    for(uctemp = 0; uctemp < usEMSCFreeBuffer; uctemp++)
    {
        bReveiceEMSC[uctemp +1] = _mhal_mhl_GetSRAMReceiveEMSCData();

        if(uctemp == 1) // Check length remaining
        {
            usEMSCFreeBuffer = bReveiceEMSC[uctemp +1] +2;
        }

#if(MHL_DEBUG_RECEIVE_EMSC)
        MHAL_MHL_MSG_INFO("** MHL eMSC data %x\r\n", bReveiceEMSC[uctemp +1]);
#endif
    }

    W2BYTEMSK(REG_MHL_ECBUS_1C, usEMSCFreeBuffer, BMASK(9:0));
    W2BYTEMSK(REG_MHL_ECBUS_1C, BIT(15), BIT(15)); 
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_InsertEMSCSendData()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_InsertEMSCSendData(MS_U8 ucLength, MS_U8 *bSendEMSC)
{
    MS_U8 uctemp = 0;
    MS_U16 usPayloadCRC = 0xFFFF;

    if((ucLength > 0) && (bSendEMSC != NULL))
    {
        W2BYTEMSK(REG_MHL_ECBUS_1B, (ucLength -1), BMASK(9:0)); // Request command byte count
        W2BYTEMSK(REG_MHL_ECBUS_79, _mhal_mhl_GetEMSCTwoByteCRC(0, (ucLength -1)) , BMASK(2:0)); // byte count CRC

        W2BYTEMSK(REG_MHL_ECBUS_21, 0, BMASK(9:0)); // SRAM address set 0
        W2BYTEMSK(REG_MHL_ECBUS_21, BIT(13), BIT(13)); // Trigger address

        for(uctemp = 0; uctemp < ucLength; uctemp++)
        {
            _mhal_mhl_InsertSRAMSendEMSCData(bSendEMSC[uctemp]);

            MHAL_MHL_MSG_INFO("** MHL payload value %x\r\n", bSendEMSC[uctemp]);

            usPayloadCRC = _mhal_mhl_GetEMSCPayloadCRC(bSendEMSC[uctemp], usPayloadCRC);
        }

        W2BYTEMSK(REG_MHL_ECBUS_19, BIT(15), BIT(15)); // eMSC payload CRC ove
        W2BYTE(REG_MHL_ECBUS_1A, (usPayloadCRC ^0xFFFF)); // eMSC payload CRC value

        MHAL_MHL_MSG_INFO("** MHL payload CRC value %x\r\n", (usPayloadCRC ^0xFFFF));

        W2BYTEMSK(REG_MHL_ECBUS_1B, BIT(15), BIT(15)); // REG_MHL_ECBUS2_1B[15]
    }
}

//**************************************************************************
//  [Function Name]:
//                  mhal_mhl_AutoEQEventProc()
//  [Description]
//
//  [Arguments]:
//
//  [Return]:
//
//**************************************************************************
void mhal_mhl_AutoEQEventProc(MS_U8 ucCbusSelect, stMHL_SIGNAL_STATUS_INFO *pMHLSignalStatus)
{
    MS_BOOL bAutoEQTrigger = FALSE;
    MS_BOOL bAutoEQContinueFlag = FALSE;

    if(pMHLSignalStatus->bAutoEnableFlag)
    {
        do
        {
            switch(pMHLSignalStatus->ucAutoEQState)
            {
                case MHL_AUTO_EQ_START:
                    if(pMHLSignalStatus->ucStableCount >= MHL_AUTO_EQ_ENABLE_COUNTER)
                    {
                        pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_ENABLE;
                    }
                    break;

                case MHL_AUTO_EQ_ENABLE:
                    _mhal_mhl_MHL30AutoEQEnable(ucCbusSelect, TRUE);

                    pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_WAIT_DONE;
                    bAutoEQTrigger = TRUE;

                    MHAL_MHL_MSG_INFO("** MHL auto EQ enable port %d\r\n", ucCbusSelect);

                    break;

                case MHL_AUTO_EQ_WAIT_DONE:
                    if(_mhal_mhl_GetAutoEQDoneFlag(ucCbusSelect))
                    {
                        MHAL_MHL_MSG_INFO("** MHL auto EQ done flag port %d\r\n", ucCbusSelect);

                        pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_STABLE_DONE;
                    }
                    break;

                case MHL_AUTO_EQ_STABLE_DONE:
                    MHAL_MHL_MSG_INFO("** MHL auto EQ done completely port %d\r\n", ucCbusSelect);

                    pMHLSignalStatus->bAutoEnableFlag = FALSE;

                    break;

                default:

                    break;
            };
        }while(bAutoEQContinueFlag);

        if(bAutoEQTrigger)
        {
            _mhal_mhl_MHL3AutoEQTrigger(ucCbusSelect);

            pMHLSignalStatus->ucAutoEQState = MHL_AUTO_EQ_WAIT_DONE;
        }
    }
}

#endif // _MHAL_MHL_C_

